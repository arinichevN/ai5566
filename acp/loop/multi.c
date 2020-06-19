#include "multi.h"

int acplm_sendSIF(ACPL *acpl, CallerQueue *queue, void *caller, const char *cmd, int channel_id, double v){
	int r = cq_control(queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendSIF(acpl, cmd, channel_id, v);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(queue);
			break;
		default:
			cq_out(queue);
			break;
	}
	return r;
}


int acplm_getFTS(ACPL *acpl, CallerQueue *queue, void *caller, const char *cmd, int channel_id, FTS *out){
	int r = cq_control(queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getFTS(acpl, cmd, channel_id, out);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(queue);
			break;
		default:
			cq_out(queue);
			break;
	}
	return r;
}

void acplm_client(ACPL *acpl, CallerQueue *queue, HardwareSerial *serial){
	cq_delMissingCallers(queue);
	cq_resetTouch(queue);
	acpl_client(acpl, serial);
}
