#include "param.h"

err_t appParam_check (AppParam *self){
	err_t err = ERROR_NO;
#ifdef USE_NOIDS
	err = noidsParam_check(&self->noids);
	if(err!=ERROR_NO){
		return err;
	}
#endif
	return err;
}

/*
 * -user_config:
 * in this function you can set default parameters for application:
 */
void appParam_setDefault(AppParam *self){
	self->id = 40;
}


APP_PARAM_DEF_GET_FIELD_FUNC(id)
APP_PARAM_DEF_SET_FIELD_FUNC(id)
