#include "main.h"

AppSerial serial_buf[SERIAL_COUNT];

AppSerialLList serials = LLIST_INITIALIZER;

void serials_reset(){
	FOREACH_SERIAL {
		appSerial_reset(serial);
	}
}

void serials_setStaticParam(){
	AppSerial *serial = serials.top;
	size_t ind = 0;
#ifdef USE_SERIAL0
	serial->device = &Serial;
	serial->id = SERIAL_ID_0; 
	serial->ind = ind;
	serial = serial->next;
	ind++;
#endif
#ifdef USE_SERIAL1
	serial->device = &Serial1;
	serial->id = SERIAL_ID_1;
	serial->ind = ind;
	serial = serial->next;
	ind++;
#endif
#ifdef USE_SERIAL2
	serial->device = &Serial2;
	serial->id = SERIAL_ID_2;
	serial->ind = ind;
	serial = serial->next;
	ind++;
#endif
#ifdef USE_SERIAL3
	serial->device = &Serial3;
	serial->id = SERIAL_ID_3;
	serial->ind = ind;
	serial = serial->next;
	ind++;
#endif
}

#define SET_DEFAULT_SERIAL_PARAM(RATE, DPS, MODE) param[i].rate = RATE; param[i].dps = DPS; param[i].mode = MODE; i++;

int serials_saveDefaultParam(){
	AppSerialParam param[SERIAL_COUNT];
	size_t i = 0;
#ifdef USE_SERIAL0
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_DPS_8N1, SERIAL_MODE_DEBUG)
	
#endif
#ifdef USE_SERIAL1
	
	//-user_config (see ../util/serial.h for available options)
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_DPS_8N1, SERIAL_MODE_SERVER)
	
#endif
#ifdef USE_SERIAL2
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_DPS_8N1, SERIAL_MODE_IDLE)
	
#endif
#ifdef USE_SERIAL3
	
	//-user_config (see ../util/serial.h for available options):
	SET_DEFAULT_SERIAL_PARAM(SERIAL_RATE_9600, SERIAL_DPS_8N1, SERIAL_MODE_IDLE)
#endif
	FOREACH_SERIAL {
		size_t ind = serial->ind;
		if(!pmem_saveSerialParam(&param[ind], ind)){
			return 0;
		}
	}
	return 1;
}

int serials_begin(int default_btn){
	LLIST_BUILD_FROM_ARRAY_N(serials, serial_buf, SERIAL_COUNT)
	serials_reset();
	serials_setStaticParam();
	if(default_btn == BUTTON_DOWN){
		if(!serials_saveDefaultParam()){
			return 0;
		}
		printd("set default app param\n");
	}
	FOREACH_SERIAL {
		AppSerialParam param;
		if(!pmem_getSerialParam(&param, serial->ind)){
			return 0;
		}
		if(appSerialParam_check(&param) != ERROR_NO){
			return 0;
		}
		if(appSerial_beginMode(serial, &param, &DEBUG_SERIAL_DEVICE) != ERROR_NO){
			return 0;
		}
	}
	return 1;
}


void serials_control(){
	FOREACH_SERIAL {
		CONTROL(serial);
	}
}

void serials_free(){
	FOREACH_SERIAL {
		appSerial_free(serial);
	}
}

Acplcm *serials_getClientById(int id){
	FOREACH_SERIAL {
		if(id == serial->id && serial->mode == SERIAL_MODE_CLIENT){
			return (Acplcm *) serial->controller;
		}
	}
	return NULL;
}

Acply *serials_getSpyById(int id){
	FOREACH_SERIAL {
		if(id == serial->id && serial->mode == SERIAL_MODE_SPY){
			return (Acply *)serial->controller;
		}
	}
	return NULL;
}

size_t serials_getIndById(int serial_id){
	FOREACH_SERIAL {
		if(serial_id == serial->id){
			return serial->ind;
		}
	}
	return -1;
}

#ifdef USE_AOIDS
void serials_buildAoids(Aoid *next_oid, Aoid *parent_oid, size_t *id){
	appSerialLList_buildAoids(&serials, next_oid, parent_oid, id);
}
#endif
