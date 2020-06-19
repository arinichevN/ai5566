#include "channel.h"

Channel channel_buf[CHANNEL_COUNT];

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
	switch(item->state){
		case RUN: return "RUN";
		case OFF: return "OFF";
		case FAILURE: return "FAILURE";
		case DISABLE: return "DISABLE";
		case INIT: return "INIT";
	}
	return "?";
}

const char *channel_getErrorStr(Channel *item){
	return getErrorStr(item->error_id);
}

void channel_setDefaults(Channel *item, size_t ind){
	channel_setDeviceKind(item, DEFAULT_CHANNEL_DEVICE_KIND);
	ton_setInterval(&item->tmr, DEFAULT_CHANNEL_POLL_INTERVAL_MS);
	item->id = ind + DEFAULT_CHANNEL_FIRST_ID;
	item->ind = ind;
	item->enable = DEFAULT_CHANNEL_ENABLE;
}

static void channel_setFromNVRAM(Channel *item, size_t ind){
	if(!pmem_getChannel(item, ind)){
		printdln("   failed to get channel");
		item->error_id = ERROR_PMEM_READ;
		return;
	}
	item->ind = ind;
}

void channel_setStaticParam(Channel *item, int cs, int sclk, int miso){
	item->sensor.cs = cs;
	item->sensor.sclk = sclk;
	item->sensor.miso = miso;
}

void channel_begin(Channel *item, size_t ind, int default_btn){
	printd("beginning channel ");printd(ind); printdln(":");
	if(default_btn == BUTTON_DOWN){
		channel_setDefaults(item, ind);
		pmem_saveChannel(item, ind);
		printd("\tdefault param\n");
	}else{
		channel_setFromNVRAM(item, ind);
		printd("\tNVRAM param\n");
	}
	item->state = INIT;
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

#define SET_CHANNEL_STATIC_PARAM(CS, SCLK, MISO) if(chn != NULL){channel_setStaticParam(chn, CS, SCLK, MISO);chn = chn->next;} else {printdln("call SET_CHANNEL_STATIC_PARAM for each channel"); return 0;}
int channels_begin(ChannelLList *channels, int default_btn){
	extern Channel channel_buf[CHANNEL_COUNT];
	channels_buildFromArray(channels, channel_buf);
	Channel *chn = channels->top;
	
	/* 
	 * -user_config:
	 * call
	 * SET_CHANNEL_STATIC_PARAM(int cs, int sclk, int miso)
	 * for each channel:
	*/
	SET_CHANNEL_STATIC_PARAM(2, 6, 5);
	SET_CHANNEL_STATIC_PARAM(3, 6, 5);
	SET_CHANNEL_STATIC_PARAM(7, 6, 5);
	
	if(chn != NULL){
		printd("number of calles of SET_CHANNEL_STATIC_PARAM() should be equal to CHANNEL_COUNT");
		return 0;
	}
	size_t i = 0;
	FOREACH_CHANNEL(channels)
		channel_begin(channel, i, default_btn); i++;
		if(channel->error_id != ERROR_NO) return 0;
	}
	return 1;
}

int channel_start(Channel *item){
	printd("starting channel ");printd(item->ind);printdln(":");
	item->enable = YES;
	item->state = INIT;
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
	item->state = INIT;
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
	item->state = INIT;
	return 1;
}

int channels_activeExists(ChannelLList *channels){
	FOREACH_CHANNEL(channels)
		if(channel->state != OFF){
			return 1;
		}
	}
	return 0;
}

void channels_stop(ChannelLList *channels){
	FOREACH_CHANNEL(channels)
		channel_stop(channel);
	}
}

int channels_getIdFirst(ChannelLList *channels, int *out){
	int success = 0;
	int f = 0;
	int v;
	FOREACH_CHANNEL(channels)
		if(!f) { v=channel->id; f=1; success=1;}
		if(channel->id < v) v = channel->id;
	}
	*out = v;
	return success;
}

int channel_getDeviceKind(Channel *item){
	return item->device_kind;
}

unsigned long channel_getPollInterval(Channel *item){
	return item->tmr.interval;
}

int channel_getEnable (Channel *item){return item->enable;}

int channel_control(Channel *item){
	switch(item->state){
		case RUN:
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
			break;
		case OFF:
			break;
		case FAILURE:
			break;
		case INIT:
			item->error_id = ERROR_NO;
			item->out.state = 0;
			item->out.tm = getCurrentTs();
			item->error_id = channel_check(item);
		    if(item->error_id != ERROR_NO){
		        item->state = FAILURE;
		        break;
		    }
			item->state = OFF;
			if(item->enable == YES){
				sensor_begin(&item->sensor);
				ton_reset(&item->tmr);
				item->state = RUN;
			}
			break;
		default:
			break;
	}
	return item->state;
}


