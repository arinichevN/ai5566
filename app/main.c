#include "main.h"

extern ChannelLList channels;
extern AppSerial serials[];

void app_OFF(App *item);
void app_FAILURE(App *item);
void app_RESET(App *item);
void app_RESET_WAIT_CHANNELS(App *item);
void app_RESET_FREE(App *item);
void app_RUN(App *item);
void app_INIT(App *item);
void app_begin(App *item);

void app_INIT(App *item){
	app_begin(item);
}

void app_OFF(App *item){
	;
}

void app_FAILURE(App *item){
	;
}

void app_RESET_FREE(App *item){
	FOREACH_CHANNEL(&channels){
		channel_free(channel);
	}
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		appSerial_free(serial);
	}
	item->control = app_INIT;
}

void app_RESET_WAIT_CHANNELS(App *item){
	FOREACH_CHANNEL(&channels){
		CONTROL(channel);
	}
	appSerials_control(serials);
	if(!channels_activeExists(&channels)){
		item->control = app_RESET_FREE;
	}
}

void app_RESET(App *item){
	FOREACH_CHANNEL(&channels){
		channel_disconnect(channel);
	}
	item->control = app_RESET_WAIT_CHANNELS;
}

void app_RUN(App *item){
	item->error_id = ERROR_NO;
	FOREACH_CHANNEL(&channels){
		//printd("channel id: "); printdln(channel->id);
		CONTROL(channel);
		if(channel->error_id != ERROR_NO){
			item->error_id = ERROR_SUBBLOCK;
		}
	}
	appSerials_control(serials);
	APPEI_CONTROL(&item->error_indicator, item->error_id);
}

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
	if(item->control == app_RUN)						return "RUN";
	else if(item->control == app_FAILURE)				return "FAILURE";
	else if(item->control == app_OFF)					return "OFF";
	else if(item->control == app_RESET)					return "RESET";
	else if(item->control == app_RESET_FREE)			return "RESET";
	else if(item->control == app_RESET_WAIT_CHANNELS)	return "RESET";
	else if(item->control == app_INIT)					return "INIT";
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

void app_begin(App *item){
	item->error_id = ERROR_NO;
	app_uploadDelay();
	appei_begin(&item->error_indicator, INDICATOR_PIN);
	pinMode(DEFAULT_CONTROL_PIN, INPUT_PULLUP);
	int btn = digitalRead(DEFAULT_CONTROL_PIN);
	//Serial.begin(9600, SERIAL_8N1);while(!Serial){;}DEBUG_SERIAL_DEVICE = &Serial;
	appSerials_init(serials);
	AppConfig config;
	item->error_id = appConfig_begin(&config, btn);
	if(item->error_id != ERROR_NO){goto err;}
	item->id = config.id;
	FOREACH_SERIAL(i){
		item->error_id = appSerial_beginMode(&serials[i], &config.serial[i], &DEBUG_SERIAL_DEVICE);
		if(item->error_id != ERROR_NO) goto err;
	}
	channels_begin(&channels, btn);
	
	item->control = app_RUN;
	return;
	
	err:
	APPEI_CONTROL(&item->error_indicator, item->error_id);
	item->control = app_FAILURE;
}

void app_reset(App *item){
	item->control = app_RESET;
}

void app_init(App *item){
	item->control = app_INIT;
}










