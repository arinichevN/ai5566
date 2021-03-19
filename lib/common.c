#include "common.h"

int checkBlockStatus(yn_t v){
	switch(v){
		case YES:return 1;
		case NO:return 1;
	}
	return 0;
}

void snprinttime(unsigned long v, char *buf, size_t len){
	unsigned long r=0.0, h, m, s, ms;
    h =  v / 3600000;
    r = v - h * 3600000;
    m = r / 60000;
    r =  r - m * 60000;
    s = r / 1000;
    ms =  r - s * 1000;
    
    snprintf(buf, len, "%02lu:%02lu:%02lu.%03lu", h, m, s, ms);
}

struct timespec getCurrentTs(){
	struct timespec out;
	unsigned long now = millis();
	out.tv_sec = (unsigned long) (now / 1000);
	out.tv_nsec = (unsigned long) ((now % 1000) * 1000000);
	return out;
}

double tsToDouble(struct timespec v){
	double out = (double) v.tv_sec + (double) v.tv_nsec / 1000000000.0;
	return out;
}

const char *getErrorStr(err_t v){
	switch(v){
		case ERROR_NO:						return "no";
		case ERROR_SOME:					return "Esome";
		case WARNING_READ:					return "Wread";
		case WARNING_BAD_DATA:				return "Wdata";
		case ERROR_PARAM:					return "Eparam";
		case ERROR_SUBBLOCK:				return "Esubb";
		case ERROR_BLOCK_STATUS:			return "Ebstat";
		case ERROR_DEVICE_KIND:				return "Edevkn";
		case ERROR_GOAL:					return "Egoal";
		case ERROR_SAVE_GOAL:				return "Esgoal";
		case ERROR_METHOD:					return "Emeth";
		case ERROR_OUT:						return "Eout";
		case ERROR_PID_MODE:				return "Epidm";
		case ERROR_PID_KP:					return "Epidkp";
		case ERROR_PID_KI:					return "Epidki";
		case ERROR_PID_KD:					return "Epidkd";
		case ERROR_POS2_MODE:				return "Ep2md";
		case ERROR_POS2_HYS:				return "Ep2hys";
		case ERROR_PWM:						return "Epwm";
		case ERROR_NVRAM_READ:				return "Envread";
		case ERROR_READ:					return "Eread";
		case ERROR_CHANNELS:				return "Echns";
		case ERROR_SERVER_NODES:			return "Esrvnds";
		case ERROR_SERIAL:					return "Eserial";
		case ERROR_SERIAL_DEVICE:			return "Eseriald";
		case ERROR_SERIAL_RATE:				return "Eserialr";
		case ERROR_SERIAL_DPS:				return "Eseriald";
		case ERROR_SERIAL_MODE:				return "Eserialm";
		case ERROR_SERIAL_BEGIN:			return "Eserialb";
		case ERROR_NO_SERIAL:				return "Enoser";
		case ERROR_SEND:					return "Esend";
		case ERROR_BAD_RESULT:				return "Ebadres";
		case ERROR_PATIENCE_LOST:			return "Epl";
		case ERROR_SLAVE_START:				return "Eslst";
		case ERROR_SLAVE_STOP:				return "Eslsp";
		case ERROR_RETRY:					return "Eretry";
		case ERROR_TERMOCOUPLE_OPEN:		return "Etmco";
		case ERROR_TERMOCOUPLE_SC_VCC:		return "Etmcsv";
		case ERROR_TERMOCOUPLE_SC_GND:		return "Etmcsg";
		case ERROR_RTC:						return "Ertc";
		case ERROR_NVRAM:					return "Envram";
		case ERROR_NOID:					return "Enoid";
		case ERROR_AOID:					return "Eaoid";
		case ERROR_1WIRE:					return "E1wire";
		default:break;
	}
	return "?";
}

const char *getStateStr(state_t v){
	switch(v){
		case STATE_UNKNOWN:		return "UNKNOWN";
		case STATE_BUSY:		return "BUSY";
		case STATE_IDLE:		return "IDLE";
		case STATE_RUN:			return "RUN";
		case STATE_OFF:			return "OFF";
		case STATE_DONE:		return "DONE";
		case STATE_FAILURE:		return "FAILURE";
		case STATE_WAIT:		return "WAIT";
		case STATE_INIT:		return "INIT";
	}
	return "?";
}
