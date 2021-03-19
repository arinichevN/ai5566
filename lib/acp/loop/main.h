#ifndef ACPL_H
#define ACPL_H

/*
 * These functions are intended to be called in loop until they return
 * ACP_DONE. In case of failure they return failure id and
 * reset object. Each call of these functions will take less or
 * a bit more than ACP_BUSY_TIMEOUT_MS of time. This allows your application
 * to do its main job better. 
 * 
 */

#include "../main.h"

#define ACP_PACK_TIMEOUT_MS			500UL
#define ACP_BUSY_TIMEOUT_US			100UL

typedef struct {
	char buf[ACP_BUF_MAX_LENGTH];
	size_t len;
	size_t wbuf_size;
	size_t wi;
	Ton busy_tmr;
	Ton pack_tmr;
	int pack;
} Acpl;


extern int acpl_begin(Acpl **self);

extern void acpl_free(Acpl *self);

extern void acpl_reset(Acpl *self);

extern int acpl_readResponse(Acpl *self, HardwareSerial *serial);

extern int acpl_readRequest(Acpl *self, HardwareSerial *serial);

extern inline void acpls_beginWrite(Acpl *self);

extern inline void acpl_beginRead(Acpl *self);

extern int acpl_write(Acpl *self, HardwareSerial *serial);


#endif 
