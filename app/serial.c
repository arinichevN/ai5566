#include "serial.h"

void appSerials_reset(AppSerial serials[]){
	FOREACH_SERIAL(i)
		AppSerial *serial = &serials[i];
		serial->acpl = NULL;
		serial->device = NULL;
		serial->id = SERIAL_IDN;
		serial->kind = APP_SERIAL_KIND_IDLE;
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
	if(item->kind != APP_SERIAL_KIND_IDLE){
		unsigned long rate = serial_getRate(item->rate);
		serial->begin(rate, item->config);
		while(!(*serial)){};
	}
}

int appSerial_beginKind(AppSerial *serial, AppSerialConfig *config, HardwareSerial **serial_debug){
	switch (config->kind){
#ifdef SERIAL_SERVER
		case APP_SERIAL_KIND_SERVER:{
			appSerial_beginDevice(config, serial->device);
			ACPL *acpl = acpl_new();
			if(acpl == NULL) return ERROR_SERIAL_BEGIN;
			acpl_begin(acpl);
			serial->acpl = acpl;
#ifdef MODE_DEBUG
			//serial->device->print("serial_server\n");
#endif
			break;}
#endif
		case APP_SERIAL_KIND_DEBUG:
			if(*serial_debug == NULL){
				appSerial_beginDevice(config, serial->device);
				*serial_debug = serial->device;
	#ifdef MODE_DEBUG
				//serial->device->print("serial_debug\n");
	#endif
			}
			break;
	}
	serial->kind = config->kind;
	return ERROR_NO;
}

void appSerials_print(AppSerial serials[]){
	FOREACH_SERIAL(i)
#ifdef MODE_DEBUG
		AppSerial *serial = &serials[i];
#endif
		printd(serial_getNameStr(serial->id)); printd(": "); printd(serial_getAppKindStr(serial->kind)); printd("\n");
	}
	printd("\n");
}

void appSerials_control(AppSerial serials[] ){
	FOREACH_SERIAL(i)
		AppSerial *serial = &serials[i];
		switch(serial->kind) {
#ifdef SERIAL_SERVER
			case APP_SERIAL_KIND_SERVER: 
				acpl_server(serial->acpl, serial->device);
				break;
#endif
		}
	}
}

AppSerial *appSerials_getClientSerialById(AppSerial serials[], int id){
	FOREACH_SERIAL(i)
		AppSerial *serial = &serials[i];
		if(id == serial->id && serial->kind == APP_SERIAL_KIND_CLIENT){
			return serial;
		}
	}
	return NULL;
}
