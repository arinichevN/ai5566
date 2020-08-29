#include "main.h"


extern ChannelLList channels;
extern AppSerial serials[];

void app_OFF(App *item);
void app_FAILURE(App *item);
void app_RESET(App *item);
void app_DISABLE(App *item);
void app_DSTEP1(App *item);
void app_DSTEP2(App *item);
void app_RUN(App *item);
void app_INIT(App *item);

void app_begin(App *item);

void app_OFF(App *item){
	;
}

void app_FAILURE(App *item){
	;
}

void app_RESET(App *item){
	channels_stop(&channels);
	item->next_control = app_INIT;
	item->control = app_DSTEP1;
}

void app_DISABLE(App *item){
	channels_stop(&channels);
	item->next_control = app_OFF;
	item->control = app_DSTEP1;
}

void app_DSTEP1(App *item){
	FOREACH_CHANNEL(&channels){
		CONTROL(channel);
	}
	appSerials_control(serials);
	if(!channels_activeExists(&channels)){
		item->control = app_DSTEP2;
	}
}

void app_DSTEP2(App *item){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		appSerial_free(serial);
	}
	item->control = item->next_control;
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
	appei_control(&item->error_indicator, item->error_id);
}

void app_INIT(App *item){
	app_begin(item);
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
	if(item->control == app_RUN)			return "RUN";
	else if(item->control == app_FAILURE)	return "FAILURE";
	else if(item->control == app_OFF)		return "OFF";
	else if(item->control == app_RESET)		return "RESET";
	else if(item->control == app_DISABLE)	return "DISABLE";
	else if(item->control == app_DSTEP1)	return "DSTEP1";
	else if(item->control == app_DSTEP2)	return "DSTEP2";
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
	item->next_control = app_FAILURE;
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
		item->error_id = appSerial_beginKind(&serials[i], &config.serial[i], &DEBUG_SERIAL_DEVICE);
		if(item->error_id != ERROR_NO) goto err;
	}
	channels_begin(&channels, btn);
	
	item->control = app_RUN;
	return;
	
	err:
	appei_control(&item->error_indicator, item->error_id);
	item->control = app_FAILURE;
}

void app_stop(App *item){
	item->control = app_DISABLE;
}

void app_reset(App *item){
	item->control = app_RESET;
}

void app_init(App *item){
	item->control = app_INIT;
}

//void app_control(App *item, AppSerial serials[], ChannelLList *channels) { 
	//switch(item->state){
		//case RUN:{
			//item->error_id = ERROR_NO;
			//FOREACH_CHANNEL(channels)
				////printd("channel id: "); printdln(channel->id);
				//channel_control(channel);
				//if(channel->error_id != ERROR_NO){
					//item->error_id = ERROR_SUBBLOCK;
				//}
			//}
			//appSerials_control(serials);
			//app_errControl(item);
			//break;}
		//case INIT:
			//app_begin(item, serials, channels);
			//break;
		//case DSTEP1:
			//FOREACH_CHANNEL(channels)
				//channel_control(channel);
			//}
			//appSerials_control(serials);
			//if(!channels_activeExists(channels)){
				//item->state = DSTEP2;
			//}
			//break;
		//case DSTEP2:{
			//FOREACH_SERIAL(i)
				//AppSerial *serial = &serials[i];
				//free(serial->acpl);
			//}
			//item->state = item->next_state;
			//break;}
		//case RESET:
			//channels_stop(channels);
			//item->next_state = INIT;
			//item->state = DSTEP1;
			//break;
		//case DISABLE:
			//channels_stop(channels);
			//item->next_state = OFF;
			//item->state = DSTEP1;
			//break;
		//case FAILURE:
			//break;
		//case OFF:
			//break;
		//default:
			//break;
	//}
//}
 











