#include "multi.h"

int acplcm_sendII(Acplcm *self, void *caller, char sign, int cmd, int v){
	int r = cq_control(self->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendII(self->acplc, sign, cmd, v);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(self->queue);
			break;
		default:
			cq_out(self->queue);
			break;
	}
	return r;
}

int acplcm_sendIIF(Acplcm *self, void *caller, char sign, int cmd, int channel_id, double v){
	int r = cq_control(self->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_sendIIF(self->acplc, sign, cmd, channel_id, v);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(self->queue);
			break;
		default:
			cq_out(self->queue);
			break;
	}
	return r;
}


int acplcm_getFts(Acplcm *self, void *caller, int cmd, int channel_id, Fts *out){
	int r = cq_control(self->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getFts(self->acplc, cmd, channel_id, out);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(self->queue);
			break;
		default:
			cq_out(self->queue);
			break;
	}
	return r;
}

int acplcm_getIS(Acplcm *self, void *caller, int cmd, int channel_id, char *out, size_t slen){
	int r = cq_control(self->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getIS(self->acplc, cmd, channel_id, out, slen);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(self->queue);
			break;
		default:
			cq_out(self->queue);
			break;
	}
	return r;
}

int acplcm_getII(Acplcm *self, void *caller, int cmd, int channel_id, int *out){
	int r = cq_control(self->queue, caller);
	if(r == CINQ_EXISTS) return ACP_WAIT;
	if(r == CINQ_REJECT) return ACP_ERROR_QUEUE;
	r = acplc_getII(self->acplc, cmd, channel_id, out);
	switch(r){
		case ACP_BUSY:
			break;
		case ACP_DONE:
			cq_out(self->queue);
			break;
		default:
			cq_out(self->queue);
			break;
	}
	return r;
}

Acplcm *acplcm_new(){
	size_t sz = sizeof (Acplcm);
	Acplcm *out = (Acplcm *) malloc(sz);
	if(out == NULL){ printdln("acplcm_new: failed");}
	printd("acplcm_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acplcm_begin(Acplcm **self){
	Acplcm *tself = acplcm_new();
	if(tself == NULL) return 0;
	tself->acplc = NULL;
	tself->queue = NULL;
	if(!acplc_begin(&tself->acplc)) return 0;
	if(!cq_begin(&tself->queue)) return 0;
	*self = tself;
	return 1;
}

void acplcm_free(Acplcm *self){
	if(self == NULL) return;
	cq_free(self->queue);
	acplc_free(self->acplc);
	free(self);
}

void acplcm_control(Acplcm *self, HardwareSerial *serial){
	cq_delMissingCallers(self->queue);
	cq_resetTouch(self->queue);
	acplc_control(self->acplc, serial);
}
