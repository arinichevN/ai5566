#ifndef MODEL_CHANNEL_AOID_H
#define MODEL_CHANNEL_AOID_H

#include "../../lib/common.h"

typedef struct {
	Aoid main;
	Aoid id;
	Aoid device_kind;
	Aoid poll_interval;
	Aoid cs;
	Aoid sclk;
	Aoid miso;
} ChannelAoid;

extern void channelAoid_build(ChannelAoid *self, Aoid *next, Aoid *parent, size_t *id, void *vchannel);

#endif 
