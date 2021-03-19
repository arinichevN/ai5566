#include "main.h"

#define ACPL_BUF self->buf
#define ACPL_BUF_LEN self->len
#define ACPL_BUF_PUSH(V) ACPL_BUF[ACPL_BUF_LEN] = (char) V; ACPL_BUF_LEN++;

#define ACPL_CHECK_BUF_LEN if (ACPL_BUF_LEN >= (ACP_BUF_MAX_LENGTH - 2)) {return ACP_ERROR_BUFFER_OVERFLOW;}
#define ACPL_READ_CHAR int c = serial->read(); if (c == -1) {return ACP_BUSY; }

#define ACPL_CHECK_BUSY_TIME if (tonu(&self->busy_tmr)) {return ACP_BUSY;}
#define ACPL_CHECK_PACK_TIME if (ton(&self->pack_tmr)) { return ACP_ERROR_PACK_TIMEOUT; }

#define ACPL_PACK_START if(!self->pack){if(c == ACP_DELIMITER_START){self->pack = 1;ton_reset(&self->pack_tmr);}else{return ACP_ERROR_NO_START_DELIMITER;}}

void acpl_reset(Acpl *self){
	memset(self->buf, 0, sizeof (*self->buf) * ACP_BUF_MAX_LENGTH); 
	ton_reset(&self->pack_tmr);
	self->len = 0; 
	self->pack = 0; 
	self->wi = 0UL; 
	self->wbuf_size = 0UL; 
}

Acpl *acpl_new(){
	size_t sz = sizeof (Acpl);
	Acpl *out = (Acpl *) malloc(sz);
	if(out == NULL){ printdln("acpl_new: failed"); return out;}
	printd("acpl_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acpl_begin(Acpl **self) {
	Acpl *tacpl = acpl_new();
	if(tacpl == NULL) return 0;
	ton_setInterval(&tacpl->pack_tmr, ACP_PACK_TIMEOUT_MS);
	ton_setInterval(&tacpl->busy_tmr, ACP_BUSY_TIMEOUT_US);
	ton_reset(&tacpl->pack_tmr);
	tonu_reset(&tacpl->busy_tmr);
	acpl_reset(tacpl);
	*self = tacpl;
	return 1;
}

void acpl_free(Acpl *self){
	free(self);	
}

inline void acpls_beginWrite(Acpl *self){
	self->wi = 0UL; 
	self->wbuf_size = strlen(self->buf) * sizeof (*self->buf);
}

inline void acpl_beginRead(Acpl *self){
	memset(self->buf, 0, sizeof (*self->buf) * ACP_BUF_MAX_LENGTH); 
	self->len = 0; 
	ton_reset(&self->pack_tmr);
}

int acpl_readResponse(Acpl *self, HardwareSerial *serial){
	//printd("(acpl_readPack: ");
	tonu_reset(&self->busy_tmr);
	while (1) {
		ACPL_CHECK_BUSY_TIME
		ACPL_CHECK_PACK_TIME
	    ACPL_CHECK_BUF_LEN
	    ACPL_READ_CHAR
	    ACPL_BUF_PUSH(c)
	    if (c == ACP_DELIMITER_END) {
	      if (ACPL_BUF_LEN < ACP_MIN_PACK_LENGTH) {
			 // printd(" short pack)"); 
			  return ACP_ERROR_SHORT_PACKAGE;
			 }
		 // printd("buf: ");printd(ACPL_BUF);
		  //printd("success) ");
	      return ACP_DONE;
	    }
	}
	//printd("error) ");
	return ACP_ERROR;
}

int acpl_readRequest(Acpl *self, HardwareSerial *serial){
	//printd("(acpl_readPack: ");
	tonu_reset(&self->busy_tmr);
	while (1) {
		ACPL_CHECK_BUSY_TIME
	    ACPL_CHECK_BUF_LEN
	    ACPL_READ_CHAR
	    ACPL_PACK_START
	    ACPL_CHECK_PACK_TIME
	    ACPL_BUF_PUSH(c)
	    if (c == ACP_DELIMITER_END) {
	      if (ACPL_BUF_LEN < ACP_MIN_PACK_LENGTH) {
			  //printd(" short pack)"); 
			  return ACP_ERROR_SHORT_PACKAGE;
			 }
		  //printd("buf: ");printd(ACPL_BUF);
		 // printd("success) ");
	      return ACP_DONE;
	    }
	}
	//printd("error) ");
	return ACP_ERROR;
}

int acpl_write(Acpl *self, HardwareSerial *serial){
	//printd("(acpl_write: ");
	tonu_reset(&self->busy_tmr);
	while(1){
		ACPL_CHECK_BUSY_TIME
		ACPL_CHECK_PACK_TIME
		if(self->wi >= ACP_BUF_MAX_LENGTH) return ACP_ERROR_FORMAT;
		if(serial->availableForWrite() < 1) { printd("cant write) ");return ACP_BUSY;}
		size_t n = serial->write(&self->buf[self->wi], 1);
		if(n != 1) {return ACP_ERROR_WRITE;}
		//printd(self->buf[self->wi]);
		if(self->buf[self->wi] == ACP_DELIMITER_END){
			 //printd("done) "); 
			 return ACP_DONE;
		}
		self->wi++;
	}
	return ACP_DONE;
}

