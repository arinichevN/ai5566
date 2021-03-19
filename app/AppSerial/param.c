#include "param.h"

int appSerialParam_check (AppSerialParam *self){
	int r = serial_checkDps(self->dps);
	if(!r){
		return ERROR_SERIAL_DPS;
	}
	unsigned long rate = serial_getRate(self->rate);
	if(rate <= 0){
		return ERROR_SERIAL_RATE;
	} 
	if(!serial_checkMode(self->mode)){
		return ERROR_SERIAL_MODE;
	}
	return ERROR_NO;
}

APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(rate)
APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(dps)
APP_SERIAL_PARAM_DEF_GET_FIELD_FUNC_I(mode)

APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(rate)
APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(dps)
APP_SERIAL_PARAM_DEF_SET_FIELD_FUNC_I(mode)
