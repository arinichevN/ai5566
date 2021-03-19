#include "param.h"

err_t channelParam_check(const ChannelParam *self){
	switch(self->device_kind){
		case DEVICE_KIND_MAX6675: case DEVICE_KIND_MAX31855:
			break;
		default:
			return ERROR_DEVICE_KIND;
	}
	return ERROR_NO;
}
