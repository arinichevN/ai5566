#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../util/dstructure.h"
#include "../../util/common.h"
#include "../../util/ton.h"
#include "../sensor/main.h"
#include "../../app/serial.h"


struct channel_st {
	int id;
	int device_kind;
	Sensor sensor;
	FTS out;
	int enable;
	int error_id;
	Ton tmr;
	size_t ind;
	void (*control)(struct channel_st *);
	struct channel_st *next;
};
typedef struct channel_st Channel;

DEC_LLIST(Channel)

#define FOREACH_CHANNEL(LIST) FOREACH_LLIST(channel, LIST, Channel)
#define CHANNEL_SAVE_FIELD(F) PmemChannel pchannel;	if(pmem_getPChannel(&pchannel, item->ind)){pchannel.F = item->F; pmem_savePChannel(&pchannel, item->ind);}
#define CHANNEL_FUN_GET(param) channel_get_ ## param

extern void channel_setDeviceKind(Channel *item, int kind);
extern const char *channel_getStateStr(Channel *item);
extern const char *channel_getErrorStr(Channel *item);
extern void channel_setParam(Channel *item, int id, unsigned long poll_period_ms, int cs, int sclk, int miso);
extern void channel_begin(Channel *item);
extern int channel_start(Channel *item);
extern int channel_stop(Channel *item);
extern int channels_getIdFirst(ChannelLList *channels, int *out);


#endif 
