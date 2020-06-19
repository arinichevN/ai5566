#ifndef ACP_LOOP_MULTI_H
#define ACP_LOOP_MULTI_H

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

#include "main.h"
#include "../../util/caller_queue.h"
//enum ACPLMstateE {
	//ACPLM_INIT,
	//ACPLM_READ_ID,
	//ACPLM_READ_COMMAND,
	//ACPLM_READ_DATA1,
	//ACPLM_READ_DATA2,
	//ACPLM_READ_DATA3,
	//ACPLM_CHECK_CRC,
	//ACPLM_SEND_RESPONSE
//};


extern int acplm_sendSIF(ACPL *acpl, CallerQueue *queue, void *caller, const char *cmd, int channel_id, double v);
extern int acplm_getFTS(ACPL *acpl, CallerQueue *queue, void *caller, const char *cmd, int channel_id, FTS *out);
extern void acplm_client(ACPL *acpl, CallerQueue *queue, HardwareSerial *serial);

#endif 
