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

#define ACPL_RESET acpl_reset(item);
#define ACPL_WBUF_PREP item->wi = 0UL; item->wbuf_size = strlen(item->buf) * sizeof (*item->buf);

typedef struct {
	char buf[ACP_BUF_MAX_LENGTH];
	size_t len;
	size_t wbuf_size;
	size_t wi;
	Ton busy_tmr;
	Ton pack_tmr;
	int pack;
} ACPL;


extern int acpl_begin(ACPL **item);

extern void acpl_free(ACPL *item);

extern void acpl_reset(ACPL *item);

extern int acpl_readResponse(ACPL *item, HardwareSerial *serial);

extern int acpl_readRequest(ACPL *item, HardwareSerial *serial);

extern inline void acpl_prepWrite(ACPL *item);

extern inline void acpl_prepRead(ACPL *item);

extern int acpl_write(ACPL *item, HardwareSerial *serial);


#endif 
