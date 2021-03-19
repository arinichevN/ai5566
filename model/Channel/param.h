#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H

#include "../../lib/common.h"

typedef struct {
	int id;
	dk_t device_kind;
	unsigned long poll_interval;
	int cs;
	int sclk;
	int miso;
} ChannelParam;

extern err_t channelParam_check(const ChannelParam *self);

#endif 
