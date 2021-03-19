#include "main.h"

#define ACPLC_START(MODE)  self->mode = MODE; ton_reset(&self->acpl->pack_tmr); self->control = acplc_SEND_REQUEST;

void acplc_READ_RESPONSE(Acplc *self, HardwareSerial *serial);
void acplc_SEND_REQUEST(Acplc *self, HardwareSerial *serial);
void acplc_IDLE(Acplc *self, HardwareSerial *serial);
void acplc_DONE(Acplc *self, HardwareSerial *serial);
void acplc_ERROR(Acplc *self, HardwareSerial *serial);
	
	
Acplc *acplc_new(){
	size_t sz = sizeof (Acplc);
	Acplc *out = (Acplc *) malloc(sz);
	if(out == NULL){ printdln("acplc_new: failed"); return out;}
	printd("acplc_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void acplc_reset(Acplc *self){
	if(self == NULL) return;
	acpl_reset(self->acpl);
	self->control = acplc_IDLE;
}

int acplc_begin(Acplc **self){
	Acplc *tself = acplc_new();
	if(tself == NULL) {printdln("acplc: failed to allocate memory"); return 0;}
	tself->acpl = NULL;
	if(!acpl_begin(&tself->acpl)) return 0;
	acplc_reset(tself);
	*self = tself;
	return 1;
}



void acplc_free(Acplc *self){
	if(self == NULL) return;
	acpl_free(self->acpl);
	free(self);
}

int acplc_getState(Acplc *self){
	if(self->control == acplc_IDLE){return ACP_IDLE;}
	else if(self->control == acplc_SEND_REQUEST){ return ACP_SEND_REQUEST;}
	else if(self->control == acplc_READ_RESPONSE){return ACP_READ_RESPONSE;}
	else if(self->control == acplc_DONE){return ACP_DONE;}
	else if(self->control == acplc_ERROR){return ACP_ERROR;}
	printdln("state not found");
	return ACP_NOT_FOUND;
}


void acplc_READ_RESPONSE(Acplc *self, HardwareSerial *serial){
	int r = acpl_readResponse(self->acpl, serial);
	switch(r){
		case ACP_DONE:
			self->control = acplc_DONE;
			return;
		case ACP_BUSY:
			return;
	}
	printdln("acplc read: error ");
	self->control = acplc_ERROR;
}

void acplc_SEND_REQUEST(Acplc *self, HardwareSerial *serial){
	int r = acpl_write(self->acpl, serial);
	switch(r){
		case ACP_DONE:
			switch(self->mode){
				case ACPLC_MODE_SEND:
					self->control = acplc_DONE;
					return;
				case ACPLC_MODE_SEND_READ:
					acpl_beginRead(self->acpl);
					self->control = acplc_READ_RESPONSE;
					return;
				default:
					printdln("acplc write: unknown mode");
					self->control = acplc_ERROR;
					return;
			}
			break;
		case ACP_BUSY:
			return;
	}
	printd("acplc_SEND_REQUEST: write returned "); printdln(acp_getStateStr(r));
	self->control = acplc_ERROR;
}

void acplc_IDLE(Acplc *self, HardwareSerial *serial){
	;
}

void acplc_DONE(Acplc *self, HardwareSerial *serial){
	acplc_reset(self);
}

void acplc_ERROR(Acplc *self, HardwareSerial *serial){
	self->control = acplc_IDLE;
}

#include "send.c"
#include "get.c"
