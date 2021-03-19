#ifndef ACPL_CLIENT_MULTI_H
#define ACPL_CLIENT_MULTI_H

/*
 * These functions are intended to be called in loop until they return
 * ACP_DONE. In case of failure they return failure id and
 * reset object. Each call of these functions will take less or
 * a bit more than ACP_BUSY_TIMEOUT_MS of time. This allows your chip
 * to make its main job better. 
 * Additionally, these functions put callers in queue and serve them
 * one by one.
 * They return:
 *  ACP_DONE - caller request has been served successfully
 *  ACP_WAIT - function is serving other caller
 *  ACP_BUSY - function is serving this caller
 *  ACP_ERROR* - caller request failed
 * Use these functions when you have many objects
 * concurrently using UART.
 */

#include "../main.h"
#include "../../../../lib/caller_queue.h"
#include "main.h"

typedef struct acplcm_st Acplcm;
struct acplcm_st {
	Acplc *acplc;
	CallerQueue *queue;
	void (*control) (Acplcm *, HardwareSerial *);
};

extern int acplcm_sendII(Acplcm *self, void *caller, char sign, int cmd, int v);
extern int acplcm_sendIIF(Acplcm *self, void *caller, char sign, int cmd, int channel_id, double v);
extern int acplcm_getFts(Acplcm *self, void *caller, int cmd, int channel_id, Fts *out);
extern int acplcm_getIS(Acplcm *self, void *caller, int cmd, int channel_id, char *out, size_t slen);
extern int acplcm_getII(Acplcm *self, void *caller, int cmd, int channel_id, int *out);
extern int acplcm_begin(Acplcm **self);
extern void acplcm_free(Acplcm *self);
extern void acplcm_control(Acplcm *self, HardwareSerial *serial);

#endif 
