#ifndef APP_SERIAL_H
#define APP_SERIAL_H

#include "../util/serial.h"
#include "../acp/loop/main.h"
#include "../acp/loop/server.h"
#include "../acp/loop/client.h"
#include "serial_config.h"

#define S0 0
#define S1 0
#define S2 0
#define S3 0
#if defined(HAVE_HWSERIAL0) && defined(WANT_SERIAL0)
#define USE_SERIAL0
#undef S0
#define S0 1
#endif
#if defined(HAVE_HWSERIAL1) && defined(WANT_SERIAL1)
#define USE_SERIAL1
#undef S1
#define S1 1
#endif
#if defined(HAVE_HWSERIAL2) && defined(WANT_SERIAL2)
#define USE_SERIAL2
#undef S2
#define S2 1
#endif
#if defined(HAVE_HWSERIAL3) && defined(WANT_SERIAL3)
#define USE_SERIAL3
#undef S3
#define S3 1
#endif
#define SERIAL_COUNT S0 + S1 + S2 + S3

#define FOREACH_SERIAL(I) for(size_t I = 0; I<SERIAL_COUNT; I++)
	
typedef struct app_serial_st {
	int id; //SerialIdE from util/serial.h
	HardwareSerial *device;
	void *controller;
	int kind;
	void (*control) (struct app_serial_st *);
	void (*free) (struct app_serial_st *);
} AppSerial;


extern void appSerials_init(AppSerial serials[]);
extern int appSerial_beginKind(AppSerial *serial, AppSerialConfig *config, HardwareSerial **serial_debug);
extern void appSerials_control(AppSerial serials[]);
extern AppSerial *appSerials_getClientSerialById(AppSerial serials[], int id);
#define appSerial_free(item) item->free(item)

#endif
