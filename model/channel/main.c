#include "main.h"

Channel channel_buf[CHANNEL_COUNT];
extern const ChannelParam CHANNEL_DEFAULT_PARAMS[];

static void channel_INIT(Channel *item);
static void channel_RUN(Channel *item);
static void channel_OFF(Channel *item);
static void channel_FAILURE(Channel *item);

static int channelParam_check(const ChannelParam *item){
	if(!common_checkBlockStatus(item->enable)){
		return ERROR_BLOCK_STATUS;
	}
	int r = sensor_checkParam(item->device_kind, item->cs, item->sclk, item->miso);
	if(r != ERROR_NO){
		return r;
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

static int channel_setParam(Channel *item, const ChannelParam *param){
	int r = channelParam_check(param);
	if(r != ERROR_NO){
		return r;
	}
	sensor_setParam(&item->sensor, param->device_kind, param->cs, param->sclk, param->miso);
	ton_setInterval(&item->tmr, param->poll_interval);
	item->id = param->id;
	item->enable = param->enable;
	return ERROR_NO;
}

static int channel_setDefaults(Channel *item, size_t ind){
	const ChannelParam *param = &CHANNEL_DEFAULT_PARAMS[ind];
	int r = channel_setParam(item, param);
	if(r == ERROR_NO){
		pmem_savePChannel(param, ind);
	}
	return r;
}

static int channel_setFromNVRAM(Channel *item, size_t ind){
	ChannelParam param;
	if(!pmem_getPChannel(&param, ind)){
		printdln("   failed to get channel from NVRAM");
		return ERROR_PMEM_READ;
	}
	return channel_setParam(item, &param);
}

static int channel_setParamAlt(Channel *item, size_t ind, int default_btn){
	if(default_btn == BUTTON_DOWN){
		int r = channel_setDefaults(item, ind);
		if(r != ERROR_NO){
			return r;
		}
		printd("\tdefault param\n");
	}else{
		int r = channel_setFromNVRAM(item, ind);
		if(r != ERROR_NO){
			return r;
		}
		printd("\tNVRAM param\n");
	}
	item->ind = ind;
	return ERROR_NO;
}

void channel_begin(Channel *item, size_t ind, int default_btn){
	printd("beginning channel ");printd(ind); printdln(":");
	item->error_id = channel_setParamAlt(item, ind, default_btn);
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

void channel_free(Channel *item){
	;
}

int channel_start(Channel *item){
	if(item->control == channel_OFF || item->control == channel_FAILURE){
		printd("starting channel ");printd(item->ind);printdln(":");
		item->enable = YES;
		item->control = channel_INIT;
		CHANNEL_SAVE_FIELD(enable)
		return 1;
	}
	return 0;
}

int channel_stop(Channel *item){
	printd("stopping channel ");printdln(item->ind); 
	item->enable = NO;
	item->out.state = 0;
	item->error_id = ERROR_NO;
	item->control = channel_OFF;
	CHANNEL_SAVE_FIELD(enable)
	return 1;
}

int channel_disconnect(Channel *item){
	printd("disconnecting channel ");printdln(item->ind);
	item->out.state = 0;
	item->error_id = ERROR_NO;
	item->control = channel_OFF;
	return 1;
}

int channel_reset(Channel *item){
	printd("resetting channel ");printd(item->ind); printdln(":");
	channel_free(item);
	channel_begin(item, item->ind, digitalRead(DEFAULT_CONTROL_PIN));
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
	return sensor_getDeviceKind(&item->sensor);
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



