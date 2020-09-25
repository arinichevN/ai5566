#include "multi.h"

int acplcm_sendII(ACPLCM *item, void *caller, char sign, int cmd, int v){
	int r = cq_control(item->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendII(item->acplc, sign, cmd, v);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(item->queue);
			break;
		default:
			cq_out(item->queue);
			break;
	}
	return r;
}

int acplcm_sendIIF(ACPLCM *item, void *caller, char sign, int cmd, int channel_id, double v){
	int r = cq_control(item->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendIIF(item->acplc, sign, cmd, channel_id, v);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(item->queue);
			break;
		default:
			cq_out(item->queue);
			break;
	}
	return r;
}


int acplcm_getFTS(ACPLCM *item, void *caller, int cmd, int channel_id, FTS *out){
	int r = cq_control(item->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getFTS(item->acplc, cmd, channel_id, out);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(item->queue);
			break;
		default:
			cq_out(item->queue);
			break;
	}
	return r;
}

int acplcm_getIS(ACPLCM *item, void *caller, int cmd, int channel_id, char *out, size_t slen){
	int r = cq_control(item->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getIS(item->acplc, cmd, channel_id, out, slen);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(item->queue);
			break;
		default:
			cq_out(item->queue);
			break;
	}
	return r;
}

ACPLCM *acplcm_new(){
	size_t sz = sizeof (ACPLCM);
	ACPLCM *out = (ACPLCM *) malloc(sz);
	if(out == NULL){ printdln("acplcm_new: failed");}
	printd("acplcm_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acplcm_begin(ACPLCM **item){
	ACPLCM *titem = acplcm_new();
	if(titem == NULL) return 0;
	titem->acplc = NULL;
	titem->queue = NULL;
	if(!acplc_begin(&titem->acplc)) return 0;
	if(!cq_begin(&titem->queue)) return 0;
	*item = titem;
	return 1;
}

void acplcm_free(ACPLCM *item){
	if(item == NULL) return;
	cq_free(item->queue);
	acplc_free(item->acplc);
	free(item);
}

void acplcm_control(ACPLCM *item, HardwareSerial *serial){
	cq_delMissingCallers(item->queue);
	cq_resetTouch(item->queue);
	acplc_control(item->acplc, serial);
}
