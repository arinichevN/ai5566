#include "main.h"

Channel channel_buf[CHANNEL_COUNT];
extern const ChannelParam CHANNEL_DEFAULT_PARAMS[];

void channel_INIT(Channel *item);
void channel_RUN(Channel *item);
void channel_OFF(Channel *item);
void channel_FAILURE(Channel *item);

void channel_setDeviceKind(Channel *item, int kind){
	switch(kind){
		case DEVICE_KIND_MAX6675:
			item->device_kind = kind;
			item->sensor.read_func = &max6675_read;
			printdln("max6675");
			break;
		case DEVICE_KIND_MAX31855:
			item->device_kind = kind;
			item->sensor.read_func = &max31855_read;
			printdln("max31855");
			break;
		default:
			item->device_kind = DEVICE_KIND_UNKNOWN;
			item->sensor.read_func = NULL;
			printdln("WARNING: unknown device");
			break;
	}
}

int channel_check(Channel *item){
	if(!common_checkBlockStatus(item->enable)){
		return ERROR_BLOCK_STATUS;
	}
	switch(item->device_kind){
		case DEVICE_KIND_MAX6675:
		case DEVICE_KIND_MAX31855:
			break;
		default:
			return ERROR_DEVICE_KIND;
	}
	if(item->sensor.read_func == NULL){
		return ERROR_DEVICE_KIND;
	}
    return ERROR_NO;
}

const char *channel_getStateStr(Channel *item){
	if(item->control == channel_RUN)			return "RUN";
	else if(item->control == channel_OFF)		return "OFF";
	else if(item->control == channel_FAILURE)	return "FAILURE";
	else if(item->control == channel_INIT)		return "INIT";
	return "?";
}

const char *channel_getErrorStr(Channel *item){
	return getErrorStr(item->error_id);
}

void channel_setDefaults(Channel *item, size_t ind){
	const ChannelParam *param = &CHANNEL_DEFAULT_PARAMS[ind];
	item->sensor.cs = param->cs;
	item->sensor.sclk = param->sclk;
	item->sensor.miso = param->miso;
	channel_setDeviceKind(item, param->device_kind);
	ton_setInterval(&item->tmr, param->poll_interval);
	item->id = param->id;
	item->enable = param->enable;
}

static void channel_setFromNVRAM(Channel *item, size_t ind){
	if(!pmem_getChannel(item, ind)){
		printdln("   failed to get channel");
		item->error_id = ERROR_PMEM_READ;
		return;
	}
}

void channel_setParam(Channel *item, size_t ind, int default_btn){
	if(default_btn == BUTTON_DOWN){
		channel_setDefaults(item, ind);
		pmem_saveChannel(item, ind);
		printd("\tdefault param\n");
	}else{
		channel_setFromNVRAM(item, ind);
		printd("\tNVRAM param\n");
	}
	item->ind = ind;
}

void channel_begin(Channel *item, size_t ind, int default_btn){
	printd("beginning channel ");printd(ind); printdln(":");
	item->error_id = ERROR_NO;
	channel_setParam(item, ind, default_btn);
	item->control = channel_INIT;
	printd("\tid: ");printdln(item->id);
	printd("\n");
}

void channels_buildFromArray(ChannelLList *channels, Channel arr[]){
	if(CHANNEL_COUNT <= 0) return;
	channels->length = CHANNEL_COUNT;
	channels->top = &arr[0];
	channels->last = &arr[CHANNEL_COUNT - 1];
	for(size_t i = 0;i<CHANNEL_COUNT;i++){
		arr[i].next = NULL;
	}
	for(size_t i = 0; i<CHANNEL_COUNT-1; i++){
		arr[i].next = &arr[i+1];
	}
}

void channels_begin(ChannelLList *channels, int default_btn){
	extern Channel channel_buf[CHANNEL_COUNT];
	channels_buildFromArray(channels, channel_buf);
	size_t i = 0;
	FOREACH_CHANNEL(channels){
		channel_begin(channel, i, default_btn); i++;
	}
}

int channel_start(Channel *item){
	printd("starting channel ");printd(item->ind);printdln(":");
	item->enable = YES;
	item->control = channel_INIT;
	PmemChannel pchannel;
	if(pmem_getPChannel(&pchannel, item->ind)){
		pchannel.enable = item->enable;
		pmem_savePChannel(&pchannel, item->ind);
	}
	return 1;
}

int channel_stop(Channel *item){
	printd("stopping channel ");printdln(item->ind); 
	item->enable = NO;
	item->out.state = 0;
	item->control = channel_INIT;
	PmemChannel pchannel;
	if(pmem_getPChannel(&pchannel, item->ind)){
		pchannel.enable = item->enable;
		pmem_savePChannel(&pchannel, item->ind);
	}
	return 1;
}

int channel_reload(Channel *item){
	printd("reloading channel ");printd(item->ind); printdln(":");
	channel_setFromNVRAM(item, item->ind);
	item->control = channel_INIT;
	return 1;
}

int channels_activeExists(ChannelLList *channels){
	FOREACH_CHANNEL(channels){
		if(channel->control != channel_OFF){
			return 1;
		}
	}
	return 0;
}

void channels_stop(ChannelLList *channels){
	FOREACH_CHANNEL(channels){
		channel_stop(channel);
	}
}

int channels_getIdFirst(ChannelLList *channels, int *out){
	int success = 0;
	int f = 0;
	int v;
	FOREACH_CHANNEL(channels){
		if(!f) { v=channel->id; f=1; success=1;}
		if(channel->id < v) v = channel->id;
	}
	*out = v;
	return success;
}

int CHANNEL_FUN_GET(device_kind)(Channel *item){
	return item->device_kind;
}

unsigned long CHANNEL_FUN_GET(poll_interval)(Channel *item){
	return item->tmr.interval;
}

int CHANNEL_FUN_GET(enable) (Channel *item){return item->enable;}

void channel_INIT(Channel *item){
	if(item->error_id != ERROR_NO){
        item->control = channel_FAILURE;
        return;
    }
	item->out.state = 0;
	item->out.tm = getCurrentTs();
	item->error_id = channel_check(item);
    if(item->error_id != ERROR_NO){
        item->control = channel_FAILURE;
        return;
    }
	item->control = channel_OFF;
	if(item->enable == YES){
		sensor_begin(&item->sensor);
		ton_reset(&item->tmr);
		item->control = channel_RUN;
	}
}

void channel_RUN(Channel *item){
	if(tonr(&item->tmr)){
		int r = sensor_read(&item->sensor, &item->out.value);
		item->out.tm = getCurrentTs();
		item->error_id = r;
		if(r != ERROR_NO){
			item->out.state = 0;
		}else{
			item->out.state = 1;
		}
		printd("result: "); printd(item->id); printd(" "); printd(item->out.state); printd(" "); printdln(item->out.value); 
	}
}

void channel_OFF(Channel *item){
	;
}

void channel_FAILURE(Channel *item){
	;
}

//int channel_control(Channel *item){
	//switch(item->state){
		//case RUN:
			//if(tonr(&item->tmr)){
				//int r = sensor_read(&item->sensor, &item->out.value);
				//item->out.tm = getCurrentTs();
				//item->error_id = r;
				//if(r != ERROR_NO){
					//item->out.state = 0;
				//}else{
					//item->out.state = 1;
				//}
				//printd("result: "); printd(item->id); printd(" "); printd(item->out.state); printd(" "); printdln(item->out.value); 
			//}
			//break;
		//case OFF:
			//break;
		//case FAILURE:
			//break;
		//case INIT:
			//if(item->error_id != ERROR_NO){
		        //item->state = FAILURE;
		        //break;
		    //}
			//item->out.state = 0;
			//item->out.tm = getCurrentTs();
			//item->error_id = channel_check(item);
		    //if(item->error_id != ERROR_NO){
		        //item->state = FAILURE;
		        //break;
		    //}
			//item->state = OFF;
			//if(item->enable == YES){
				//sensor_begin(&item->sensor);
				//ton_reset(&item->tmr);
				//item->state = RUN;
			//}
			//break;
		//default:
			//break;
	//}
	//return item->state;
//}


