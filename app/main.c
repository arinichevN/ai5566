#include "main.h"

int app_id;
dk_t app_device_kind = DEVICE_KIND_AI5566;
err_t app_error_id;
void (*app_control)();

void app_OFF();
void app_FAILURE();
void app_RESET();
void app_RESET_WAIT_CHANNELS();
void app_RESET_FREE();
void app_RUN();
void app_INIT();
void app_begin();

void app_INIT(){
	app_begin();
}

void app_OFF(){
	;
}

void app_FAILURE(){
	;
}

void app_RESET_FREE(){
	FOREACH_CHANNEL{
		channel_free(channel);
	}
	serials_free();
	app_control = app_INIT;
}

void app_RESET_WAIT_CHANNELS(){
	FOREACH_CHANNEL{
		CONTROL(channel);
	}
	serials_control();
	if(!channels_activeExists()){
		app_control = app_RESET_FREE;
	}
}

void app_RESET(){
	FOREACH_CHANNEL{
		channel_disconnect(channel);
	}
	app_control = app_RESET_WAIT_CHANNELS;
}


void app_RUN(){
	FOREACH_CHANNEL{
		CONTROL(channel);
		//printd("channel control: "); printdln(channel->id);
		if(channel->error_id != ERROR_NO){
			app_error_id = ERROR_SUBBLOCK;
		}
	}
	serials_control();
	appei_control(app_error_id);
}

//time for attempt to upload sketch in case of error
void app_uploadDelay(){
	pinMode(INDICATOR_PIN, OUTPUT);
	digitalWrite(INDICATOR_PIN, HIGH);
	delay(APP_UPLOAD_DELAY_MS);
	digitalWrite(INDICATOR_PIN, LOW);
}

const char *app_getErrorStr(){
	return getErrorStr(app_error_id);
} 

const char *app_getStateStr(){
	if(app_control == app_RUN)						return "RUN";
	else if(app_control == app_FAILURE)				return "FAILURE";
	else if(app_control == app_OFF)					return "OFF";
	else if(app_control == app_RESET)				return "RESET";
	else if(app_control == app_RESET_FREE)			return "RESET";
	else if(app_control == app_RESET_WAIT_CHANNELS)	return "RESET";
	else if(app_control == app_INIT)				return "INIT";
	return "?";
}

int appc_checkId(int v){
	return 1;
}

int app_setParam(int default_btn){
	AppParam param;
	if(default_btn == BUTTON_DOWN){
		appParam_setDefault(&param);
#ifdef USE_NOIDS
		noidsParam_setDefault(&param.noids);
#endif
		pmem_saveAppParam(&param);
		printd("set default app param\n");
	}
	int r = pmem_getAppParam(&param);
	if(!r){
		printd("failed to get app\n");
		return 0;
	}
	err_t err = appParam_check(&param);
	if(err != ERROR_NO){
		return 0;
	}
	app_id = param.id;
#ifdef USE_NOIDS
	noids_setParam(&param.noids);
#endif
	return 1;
}

void app_begin(){
	//goto err;
	app_error_id = ERROR_NO;
	app_uploadDelay();
	pinMode(DEFAULT_CONTROL_PIN, INPUT_PULLUP);
	int default_btn = digitalRead(DEFAULT_CONTROL_PIN);
	appei_begin(INDICATOR_PIN);
	if(!pmem_checkSize()){
		app_error_id = ERROR_NVRAM;
		goto err;
	}
	//int default_btn = BUTTON_DOWN;
	//Serial.begin(9600, SERIAL_8N1);while(!Serial){;}DEBUG_SERIAL_DEVICE = &Serial;
	if(!app_setParam(default_btn)){
		app_error_id = ERROR_PARAM;
		goto err;
	}
	//delay(1000);
	if(!serials_begin(default_btn)){
		app_error_id = ERROR_SERIAL;
		goto err;
	}
	//delay(1000);
	chplr_begin();
	channels_begin(default_btn);
	//delay(1000);
#ifdef USE_AOIDS
	if(!aoids_begin()){
		app_error_id = ERROR_AOID;
		goto err;
	}
#endif
#ifdef USE_NOIDS
	if(!noids_begin(default_btn)){
		app_error_id = ERROR_NOID;
		goto err;
	}
#endif
	app_control = app_RUN;
	printdln("started");
	return;
	
	err:
	appei_control(app_error_id);
	app_control = app_FAILURE;
}

void app_reset(){
	app_control = app_RESET;
}





 











