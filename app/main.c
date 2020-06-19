#include "main.h"

#define APP_ERROR_SHOW digitalWrite(INDICATOR_PIN, HIGH);
#define APP_ERROR_HIDE digitalWrite(INDICATOR_PIN, LOW);

//time for attempt to upload sketch in case of error
void app_uploadDelay(){
	pinMode(INDICATOR_PIN, OUTPUT);
	digitalWrite(INDICATOR_PIN, HIGH);
	delay(5000);
	digitalWrite(INDICATOR_PIN, LOW);
}

const char *app_getErrorStr(App *item){
	return getErrorStr(item->error_id);
} 

const char *app_getStateStr(App *item){
	switch(item->state){
		case RUN:		return "RUN";
		case FAILURE:	return "FAILURE";
		case OFF:		return "OFF";
		case RESET:		return "RESET";
		case DISABLE:	return "DISABLE";
		case DSTEP1:	return "DSTEP1";
		case DSTEP2:	return "DSTEP2";
	}
	return "?";
}

int appc_checkId(int v){
	return 1;
}

int appc_checkSerialRate(int v){
	unsigned long r = serial_getRate(v);
	if(r > 0){
		return 1;
	}
	return 0;
}

int appc_checkSerialConfig(int v){
	int r = serial_checkConfig(v);
    if(r){
		return 1;
	}
	return 0;
}


int app_begin(App *item, AppSerial serials[], ChannelLList *channels){
	item->error_id = ERROR_NO;
	item->ectl_state = INIT;
	item->next_state = OFF;
	app_uploadDelay();
	pinMode(INDICATOR_PIN, OUTPUT);
	pinMode(DEFAULT_CONTROL_PIN, INPUT_PULLUP);
	int btn = digitalRead(DEFAULT_CONTROL_PIN);
	//Serial.begin(9600, SERIAL_8N1);while(!Serial){;}DEBUG_SERIAL_DEVICE = &Serial;
	appSerials_init(serials);
	AppConfig config;
	item->error_id = appConfig_begin(&config, btn);
	if(item->error_id != ERROR_NO){goto err;}
	item->id = config.id;
	FOREACH_SERIAL(i)
		item->error_id = appSerial_beginKind(&serials[i], &config.serial[i], &DEBUG_SERIAL_DEVICE);
		if(item->error_id != ERROR_NO) goto err;
	}
	if(!channels_begin(channels, btn)){
		item->error_id = ERROR_CHANNELS;
		goto err;
	}
	appSerials_print(serials);
	printdln("app_begin: success");
	return RUN;
	err:
	APP_ERROR_SHOW
	printdln("app_begin: failure");
	return FAILURE;
}

void app_stop(App *item){
	item->state = DISABLE;		
}

void app_reset(App *item){
	item->state = RESET;		
}

void app_init(App *item){
	item->state = INIT;
}

void app_errControl(App *item){
	int error_id = item->error_id;
	int state = item->ectl_state;
	switch(state){
		case WAIT_LOW:
			if(error_id == ERROR_NO){
				printdln("app error detected: no error");
				state = OFF;
			}
			break;
		case WAIT_HIGH:
			if(error_id != ERROR_NO){
				printd("app error detected: ");printdln(app_getErrorStr(item));
				state = ON;
			}
			break;
		case ON:
			APP_ERROR_SHOW
			state = WAIT_LOW;
			break;
		case OFF:
			APP_ERROR_HIDE
			state = WAIT_HIGH;
			break;
		case INIT:
			state = WAIT_HIGH;
			break;
	}
	item->ectl_state = state;
}

void app_control(App *item, AppSerial serials[], ACPLCommandNode acnodes[], size_t acnodes_count, ChannelLList *channels) { 
	switch(item->state){
		case RUN:{
			item->error_id = ERROR_NO;
			FOREACH_CHANNEL(channels)
				//printd("channel id: "); printdln(channel->id);
				channel_control(channel);
				if(channel->error_id != ERROR_NO){
					item->error_id = ERROR_SUBBLOCK;
				}
			}
			appSerials_control(serials, acnodes, acnodes_count);
			app_errControl(item);
			break;}
		case INIT:
			item->ectl_state = INIT;
			item->state = app_begin(item, serials, channels);
			break;
		case DSTEP1:
			FOREACH_CHANNEL(channels)
				channel_control(channel);
			}
			appSerials_control(serials, acnodes, acnodes_count);
			if(channels_activeExists(channels)){
				item->state = DSTEP2;
			}
			break;
		case DSTEP2:{
			FOREACH_SERIAL(i)
				AppSerial *serial = &serials[i];
				free(serial->acpl);
			}
			item->state = item->next_state;
			break;}
		case RESET:
			channels_stop(channels);
			item->next_state = INIT;
			item->state = DSTEP1;
			break;
		case DISABLE:
			channels_stop(channels);
			item->next_state = OFF;
			item->state = DSTEP1;
			break;
		case FAILURE:
			break;
		case OFF:
			break;
		default:
			break;
	}
}
 











