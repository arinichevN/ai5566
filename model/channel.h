#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../util/dstructure.h"
#include "../util/common.h"
#include "../util/ton.h"
#include "sensor/main.h"
#include "../app/serial.h"


/*
 * -user_config:
 * set number of channels you want to use:
 */
#define CHANNEL_COUNT 3

/*
 * -user_config:
 * here you can set some default parameters for
 * channels
 */
#define DEFAULT_CHANNEL_FIRST_ID			11
#define DEFAULT_CHANNEL_POLL_INTERVAL_MS	300 //conversion time for MAX6675: 220 ms, for MAX31855: 100 ms
#define DEFAULT_CHANNEL_DEVICE_KIND			DEVICE_KIND_MAX31855
#define DEFAULT_CHANNEL_ENABLE				YES

struct channel_st {
	int id;
	int device_kind;
	Sensor sensor;
	FTS out;
	int enable;
	int error_id;
	Ton tmr;
	int state;
	size_t ind;
	struct channel_st *next;
};
typedef struct channel_st Channel;

DEC_LLIST(Channel)

#define FOREACH_CHANNEL(LIST) FOREACH_LLIST(channel, LIST, Channel){
#define CHANNEL_SAVE_FIELD(F) PmemChannel pchannel;	if(pmem_getPChannel(&pchannel, item->ind)){pchannel.F = item->F; pmem_savePChannel(&pchannel, item->ind);}
#define CHANNEL_FUN_GET(param) channel_get_ ## param

extern void channel_setDeviceKind(Channel *item, int kind);
extern const char *channel_getStateStr(Channel *item);
extern const char *channel_getErrorStr(Channel *item);
extern void channel_setParam(Channel *item, int id, unsigned long poll_period_ms, int cs, int sclk, int miso);
extern void channel_begin(Channel *item);
extern int channel_start(Channel *item);
extern int channel_stop(Channel *item);
extern int channel_control(Channel *item);
extern int channels_getIdFirst(ChannelLList *channels, int *out);


#endif 
