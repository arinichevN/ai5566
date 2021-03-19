#include "param.h"

/*
 * -user_config:
 * in this function you can set default parameters for network objects:
 */
void noidsParam_setDefault(NoidsParam *self){
	self->is_first = YES;
	self->next_external_noid_id = NOID_ID_UNKNOWN;
}

err_t noidsParam_check(const NoidsParam *self){
	if(!checkBlockStatus(self->is_first)){
		printdln("check noid.is_first");
		return ERROR_NOID;
	}
	return ERROR_NO;
}
