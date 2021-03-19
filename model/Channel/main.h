#ifndef MODEL_CHANNEL_H
#define MODEL_CHANNEL_H

#include "../../lib/dstructure.h"
#include "../../lib/common.h"
#include "../../lib/ton.h"
#include "../../lib/acp/main.h"
#include "../../lib/acp/loop/main.h"
#include "../../lib/acp/loop/server/main.h"
#include "../../lib/acp/command/main.h"
#include "../interface/iSensor.h"
#include "../MAX6675/main.h"
#include "../MAX31855/main.h"

#ifdef USE_AOIDS
#include "../../lib/aoid.h"
#include "../../app/serials/server/aoid.h"
#include "aoid.h"
#endif
#ifdef USE_NOIDS
#include "../../lib/noid.h"
#include "../../app/serials/server/noid.h"
#endif
#include "../../app/serials/main.h"
#include "../../pmem/main.h"
#include "param.h"

typedef struct channel_st Channel;

typedef enum {
	CHPL_IDLE,
	CHPL_WAIT,
	CHPL_RUN
} ChannelPortLockState;

typedef struct {
	Channel *before;
	Channel *after;
	ChannelPortLockState state;
} ChannelPortLock;


struct channel_st {
	int id;
	int cs;
	int sclk;
	int miso;
	dk_t device_kind;
	iSensor *sensor;
	Ton tmr;
	Fts output;
	ChannelPortLock port_lock;
#ifdef USE_AOIDS
	ChannelAoid aoid;
#endif
#ifdef USE_NOIDS
	Noid noid;
#endif
	void (*control)(Channel *);
	err_t error_id;
	size_t ind;
	Channel *next;
};

extern state_t channel_getState(Channel *self);
extern err_t channel_getError(Channel *self);
extern void channel_begin(Channel *self, size_t ind);
extern int channel_start(Channel *self);
extern int channel_stop(Channel *self);
extern int channel_disconnect(Channel *self);
extern int channel_reset(Channel *self);
extern void channel_free(Channel *self);

extern int channel_portIntersectionExists(Channel *channel1, Channel *channel2);

#ifdef USE_AOIDS
extern void channel_buildAoids(Channel *self, Aoid *next_oid, Aoid *parent_oid, size_t *id);
#endif

#ifdef USE_NOIDS
extern Noid *channel_buildNoids(Channel *self);
#endif

#endif 
