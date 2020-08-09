#include "multi.h"


int acplm_sendII(ACPL *acpl, CallerQueue *queue, void *caller, char sign, int cmd, int v){
	int r = cq_control(queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendII(acpl, sign, cmd, v);
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

int acplm_sendIIF(ACPL *acpl, CallerQueue *queue, void *caller, char sign, int cmd, int channel_id, double v){
	int r = cq_control(queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendIIF(acpl, sign, cmd, channel_id, v);
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


int acplm_getFTS(ACPL *acpl, CallerQueue *queue, void *caller, int cmd, int channel_id, FTS *out){
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

int acplm_getIS(ACPL *acpl, CallerQueue *queue, void *caller, int cmd, int channel_id, char *out, size_t slen){
	int r = cq_control(queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getIS(acpl, cmd, channel_id, out, slen);
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
