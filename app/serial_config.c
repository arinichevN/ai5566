#include "serial_config.h"

static int appSerialConfig_check (AppSerialConfig *item){
	int r = serial_checkConfig(item->config);
	if(!r){
		return ERROR_SERIAL_CONFIG;
	}
	unsigned long rate = serial_getRate(item->rate);
	if(rate <= 0){
		return ERROR_SERIAL_RATE;
	} 
	if(!serial_checkMode(item->mode)){
		return ERROR_SERIAL_MODE;
	}
	return ERROR_NO;
}

APPCONFIGSR_DEF_GET_FIELD_FUNC(rate)
APPCONFIGSR_DEF_GET_FIELD_FUNC(config)
APPCONFIGSR_DEF_GET_FIELD_FUNC(mode)

APPCONFIGSR_DEF_SET_FIELD_FUNC(rate)
APPCONFIGSR_DEF_SET_FIELD_FUNC(config)
APPCONFIGSR_DEF_SET_FIELD_FUNC(mode)
