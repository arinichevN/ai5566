#include "serial.h"

long int serial_getRate (uint8_t v){
	switch(v){
		case SR_300:return 300;
		case SR_1200:return 1200;
		case SR_2400:return 2400;
		case SR_4800:return 4800;
		case SR_9600:return 9600;
		case SR_14400:return 14400;
		case SR_19200:return 19200;
		case SR_28800:return 28800;
		case SR_38400:return 38400;
		case SR_57600:return 57600;
		case SR_115200:return 115200;
	}
	return -1;
}

uint8_t serial_getRateKind (long int v){
	switch(v){
		case 300:return SR_300;
		case 1200:return SR_1200;
		case 2400:return SR_2400;
		case 4800:return SR_4800;
		case 9600:return SR_9600;
		case 14400:return SR_14400;
		case 19200:return SR_19200;
		case 28800:return SR_28800;
		case 38400:return SR_38400;
		case 57600:return SR_57600;
		case 115200:return SR_115200;
	}
	return 0;
}

int serial_checkConfig(uint8_t v){
	switch(v){
		case SERIAL_5N1:
		case SERIAL_6N1:
		case SERIAL_7N1:
		case SERIAL_8N1:
		case SERIAL_5N2:
		case SERIAL_6N2:
		case SERIAL_7N2:
		case SERIAL_8N2:
		case SERIAL_5E1:
		case SERIAL_6E1:
		case SERIAL_7E1:
		case SERIAL_8E1:
		case SERIAL_5E2:
		case SERIAL_6E2:
		case SERIAL_7E2:
		case SERIAL_8E2:
		case SERIAL_5O1:
		case SERIAL_6O1:
		case SERIAL_7O1:
		case SERIAL_8O1:
		case SERIAL_5O2:
		case SERIAL_6O2:
		case SERIAL_7O2:
		case SERIAL_8O2:
			return 1;
	}
	return 0;
}

int serial_checkMode(int v){
	switch(v){
		case SERIAL_MODE_IDLE:
		case SERIAL_MODE_SERVER:
		case SERIAL_MODE_CLIENT:
		case SERIAL_MODE_DEBUG:
		case SERIAL_MODE_SPY:
			return 1;
	}
	return 0;
}

const char *serial_getConfigStr(uint8_t v){
	switch(v){
		case SERIAL_5N1:return "5N1";
		case SERIAL_6N1:return "6N1";
		case SERIAL_7N1:return "7N1";
		case SERIAL_8N1:return "8N1";
		case SERIAL_5N2:return "5N2";
		case SERIAL_6N2:return "6N2";
		case SERIAL_7N2:return "7N2";
		case SERIAL_8N2:return "8N2";
		case SERIAL_5E1:return "5E1";
		case SERIAL_6E1:return "6E1";
		case SERIAL_7E1:return "7E1";
		case SERIAL_8E1:return "8E1";
		case SERIAL_5E2:return "5E2";
		case SERIAL_6E2:return "6E2";
		case SERIAL_7E2:return "7E2";
		case SERIAL_8E2:return "8E2";
		case SERIAL_5O1:return "5O1";
		case SERIAL_6O1:return "6O1";
		case SERIAL_7O1:return "7O1";
		case SERIAL_8O1:return "8O1";
		case SERIAL_5O2:return "5O2";
		case SERIAL_6O2:return "6O2";
		case SERIAL_7O2:return "7O2";
		case SERIAL_8O2:return "8O2";
	}
	return "?";
}

int serial_checkId(int id){
#ifdef USE_SERIAL0
	if(id == SERIAL_ID0) return 1;
#endif
#ifdef USE_SERIAL1
	if(id == SERIAL_ID1) return 1;
#endif
#ifdef USE_SERIAL2
	if(id == SERIAL_ID2) return 1;
#endif
#ifdef USE_SERIAL3
	if(id == SERIAL_ID3) return 1;
#endif
	return 0;
}

HardwareSerial *serial_getDeviceById(int id){
#ifdef USE_SERIAL0
	if(id == SERIAL_ID0) return &Serial;
#endif
#ifdef USE_SERIAL1
	if(id == SERIAL_ID1) return &Serial1;
#endif
#ifdef USE_SERIAL2
	if(id == SERIAL_ID2) return &Serial2;
#endif
#ifdef USE_SERIAL3
	if(id == SERIAL_ID3) return &Serial3;
#endif
	return NULL;
}

const char *serial_getIdStr(int id){
	switch(id){
		case SERIAL_IDN: return "serialNo";
		case SERIAL_ID0: return "serial0";
		case SERIAL_ID1: return "serial1";
		case SERIAL_ID2: return "serial2";
		case SERIAL_ID3: return "serial3";
	}
	return "?";
}

const char *serial_getModeStr(int v){
	switch(v){
		case SERIAL_MODE_IDLE:		return "idle";
		case SERIAL_MODE_SERVER:	return "server";
		case SERIAL_MODE_CLIENT:	return "client";
		case SERIAL_MODE_DEBUG:		return "debug";
		case SERIAL_MODE_SPY:		return "spy";
	}	
	return "?";
}

