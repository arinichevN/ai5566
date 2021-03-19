#ifndef APP_SERIAL_H
#define APP_SERIAL_H

#include "../../lib/serial.h"
#include "../../lib/acp/loop/main.h"
#include "../../lib/acp/loop/server/main.h"
#ifdef USE_AOIDS
#include "../../lib/aoid.h"
#include "../serials/server/aoid.h"
#endif
#include "param.h"


typedef struct app_serial_st AppSerial;
struct app_serial_st {
	int id;
	int mode;
	int rate;
	int dps;
	HardwareSerial *device;
	void *controller;
#ifdef USE_AOIDS
	Aoid aoid;
	Aoid id_aoid;
	Aoid rate_aoid;
	Aoid dps_aoid;
	Aoid mode_aoid;
#endif
	size_t ind;
	void (*control) (AppSerial *);
	void (*free) (AppSerial *);
	AppSerial *next;
};

extern void appSerial_reset(AppSerial *self);
extern int appSerial_beginKind(AppSerial *serial, AppSerialParam *param, HardwareSerial **serial_debug);
#define appSerial_free(item) item->free(item)

#ifdef USE_AOIDS
extern void appSerial_buildAoids(AppSerial *self, Aoid *next_oid, Aoid *parent_oid, size_t *id);
#endif

#endif
