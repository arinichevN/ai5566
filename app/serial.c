#include "serial.h"


static void appSerial_controlIdle(AppSerial *item){
	;
}

static void appSerial_freeIdle(AppSerial *serial){
	;
}

#ifdef SERIAL_SERVER
static void appSerial_controlServer(AppSerial *item){
	ACPLS *controller = (ACPLS *) item->controller;
	acpls_control(controller, item->device);
}

static void appSerial_freeServer(AppSerial *serial){
	ACPLS *item = (ACPLS *) serial->controller;
	acpls_free(item);
}

static int appSerial_beginServer(AppSerial *item){
	ACPLS *controller = NULL;
	if(!acpls_begin(&controller)) {
		printd("server begin failed where id = "); printdln(item->id);
		return 0;
	}
	item->controller = (void *) controller;
	item->control = appSerial_controlServer;
	item->free = appSerial_freeServer;
	return 1;
}
#endif


void appSerials_reset(AppSerial serials[]){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		serial->id = SERIAL_IDN;
		serial->mode = SERIAL_MODE_IDLE;
		serial->device = NULL;
		serial->controller = NULL;
		serial->control = appSerial_controlIdle;
		serial->free = appSerial_freeIdle;
	}
}

void appSerials_init(AppSerial serials[]){
	appSerials_reset(serials);
	size_t i = 0;
#ifdef USE_SERIAL0
	serials[i].device = &Serial;
	serials[i].id = SERIAL_ID0; i++;
#endif
#ifdef USE_SERIAL1
	serials[i].device = &Serial1;
	serials[i].id = SERIAL_ID1; i++;
#endif
#ifdef USE_SERIAL2
	serials[i].device = &Serial2;
	serials[i].id = SERIAL_ID2; i++;
#endif
#ifdef USE_SERIAL3
	serials[i].device = &Serial3;
	serials[i].id = SERIAL_ID3; i++;
#endif
}

static void appSerial_beginDevice(AppSerialConfig *item, HardwareSerial *serial){
	if(item->mode != SERIAL_MODE_IDLE){
		unsigned long rate = serial_getRate(item->rate);
		serial->begin(rate, item->config);
		while(!(*serial)){};
	}
}

int appSerial_beginMode(AppSerial *serial, AppSerialConfig *config, HardwareSerial **serial_debug){
	printd("serial"); printd(serial->id);
	switch (config->mode){
#ifdef SERIAL_SERVER
		case SERIAL_MODE_SERVER:
			appSerial_beginDevice(config, serial->device);
			if(!appSerial_beginServer(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = config->mode;
			printdln(": server");
			break;
#endif
		case SERIAL_MODE_DEBUG:
			if(*serial_debug == NULL){
				appSerial_beginDevice(config, serial->device);
				*serial_debug = serial->device;
				serial->mode = config->mode;
				printdln(": debug");
			}
			break;
		default:
			printdln(": unknown");
			break;
	}
	return ERROR_NO;
}

void appSerials_control(AppSerial serials[] ){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		CONTROL(serial);
	}
}

AppSerial *appSerials_getClientSerialById(AppSerial serials[], int id){
	FOREACH_SERIAL(i){
		AppSerial *serial = &serials[i];
		if(id == serial->id && serial->mode == SERIAL_MODE_CLIENT){
			return serial;
		}
	}
	return NULL;
}
