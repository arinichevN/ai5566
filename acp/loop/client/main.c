#include "main.h"

#define ACPLC_START(MODE) item->mode = MODE; item->control = acplc_SEND_REQUEST;

void acplc_READ_RESPONSE(ACPLC *item, HardwareSerial *serial);
void acplc_SEND_REQUEST(ACPLC *item, HardwareSerial *serial);
void acplc_IDLE(ACPLC *item, HardwareSerial *serial);
void acplc_DONE(ACPLC *item, HardwareSerial *serial);
void acplc_ERROR(ACPLC *item, HardwareSerial *serial);
	
	
ACPLC *acplc_new(){
	size_t sz = sizeof (ACPLC);
	ACPLC *out = (ACPLC *) malloc(sz);
	if(out == NULL){ printdln("acplc_new: failed");}
	printd("acplc_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void acplc_reset(ACPLC *item){
	if(item == NULL) return;
	acpl_reset(item->acpl);
	item->control = acplc_IDLE;
}

int acplc_begin(ACPLC **item){
	ACPLC *titem = acplc_new();
	if(titem == NULL) {printdln("acplc: failed to allocate memory"); return 0;}
	titem->acpl = NULL;
	if(!acpl_begin(&titem->acpl)) return 0;
	acplc_reset(titem);
	*item = titem;
	return 1;
}



void acplc_free(ACPLC *item){
	if(item == NULL) return;
	acpl_free(item->acpl);
	free(item);
}

int acplc_getState(ACPLC *item){
	if(item->control == acplc_IDLE){return ACP_IDLE;}
	else if(item->control == acplc_SEND_REQUEST){ return ACP_SEND_REQUEST;}
	else if(item->control == acplc_READ_RESPONSE){return ACP_READ_RESPONSE;}
	else if(item->control == acplc_DONE){return ACP_DONE;}
	else if(item->control == acplc_ERROR){return ACP_ERROR;}
	printdln("state not found");
	return ACP_NOT_FOUND;
}


void acplc_READ_RESPONSE(ACPLC *item, HardwareSerial *serial){
	int r = acpl_readResponse(item->acpl, serial);
	switch(r){
		case ACP_DONE:
			item->control = acplc_DONE;
			return;
		case ACP_BUSY:
			return;
	}
	printdln("acplc read: error ");
	item->control = acplc_ERROR;
}

void acplc_SEND_REQUEST(ACPLC *item, HardwareSerial *serial){
	int r = acpl_write(item->acpl, serial);
	switch(r){
		case ACP_DONE:
			switch(item->mode){
				case ACPLC_MODE_SEND:
					item->control = acplc_DONE;
					return;
				case ACPLC_MODE_SEND_READ:
					acpl_prepRead(item->acpl);
					item->control = acplc_READ_RESPONSE;
					return;
				default:
					printdln("acplc write: unknown mode");
					item->control = acplc_ERROR;
					return;
			}
			break;
		case ACP_BUSY:
			return;
	}
	printdln("acplc write: impossible");
	item->control = acplc_ERROR;
}

void acplc_IDLE(ACPLC *item, HardwareSerial *serial){
	;
}

void acplc_DONE(ACPLC *item, HardwareSerial *serial){
	item->control = acplc_IDLE;
}

void acplc_ERROR(ACPLC *item, HardwareSerial *serial){
	item->control = acplc_IDLE;
}

#include "send.c"
#include "get.c"
