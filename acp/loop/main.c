#include "main.h"

#define ACPL_BUF item->buf
#define ACPL_BUF_LEN item->len
#define ACPL_BUF_PUSH(V) ACPL_BUF[ACPL_BUF_LEN] = (char) V; ACPL_BUF_LEN++;

#define ACPL_CHECK_BUF_LEN if (ACPL_BUF_LEN >= (ACP_BUF_MAX_LENGTH - 2)) {return ACP_ERROR_BUFFER_OVERFLOW;}
#define ACPL_READ_CHAR int c = serial->read(); if (c == -1) {return ACP_BUSY; }

#define ACPL_CHECK_BUSY_TIME if (ton(&item->busy_tmr)) {return ACP_BUSY;}
#define ACPL_CHECK_PACK_TIME if (ton(&item->pack_tmr)) { return ACP_ERROR_PACK_TIMEOUT; }

#define ACPL_PACK_START if(!item->pack){if(c == ACP_DELIMITER_START){item->pack = 1;ton_reset(&item->pack_tmr);}else{return ACP_ERROR_NO_START_DELIMITER;}}

void acpl_reset(ACPL *item){
	memset(item->buf, 0, sizeof (*item->buf) * ACP_BUF_MAX_LENGTH); 
	ton_reset(&item->pack_tmr);
	item->len = 0; 
	item->pack = 0; 
	item->wi = 0UL; 
	item->wbuf_size = 0UL; 
}

ACPL *acpl_new(){
	size_t sz = sizeof (ACPL);
	ACPL *out = (ACPL *) malloc(sz);
	if(out == NULL){ printdln("acpl_new: failed");}
	printd("acpl_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acpl_begin(ACPL **item) {
	ACPL *tacpl = acpl_new();
	if(tacpl == NULL) return 0;
	ton_setInterval(&tacpl->pack_tmr, ACP_PACK_TIMEOUT_MS);
	ton_setInterval(&tacpl->busy_tmr, ACP_BUSY_TIMEOUT_MS);
	ton_reset(&tacpl->pack_tmr);
	ton_reset(&tacpl->busy_tmr);
	acpl_reset(tacpl);
	*item = tacpl;
	return 1;
}

void acpl_free(ACPL *item){
	free(item);	
}

inline void acpl_prepWrite(ACPL *item){
	item->wi = 0UL; 
	item->wbuf_size = strlen(item->buf) * sizeof (*item->buf);
}

inline void acpl_prepRead(ACPL *item){
	memset(item->buf, 0, sizeof (*item->buf) * ACP_BUF_MAX_LENGTH); 
	item->len = 0; 
	ton_reset(&item->pack_tmr);
}

int acpl_readResponse(ACPL *item, HardwareSerial *serial){
	//printd("(acpl_readPack: ");
	ton_reset(&item->busy_tmr);
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

int acpl_readRequest(ACPL *item, HardwareSerial *serial){
	//printd("(acpl_readPack: ");
	ton_reset(&item->busy_tmr);
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

int acpl_write(ACPL *item, HardwareSerial *serial){
	//printd("(acpl_write: ");
	ton_reset(&item->busy_tmr);
	while(1){
		ACPL_CHECK_BUSY_TIME
		ACPL_CHECK_PACK_TIME
		if(item->wi >= ACP_BUF_MAX_LENGTH) return ACP_ERROR_FORMAT;
		if(serial->availableForWrite() < 1) { printd("cant write) ");return ACP_BUSY;}
		size_t n = serial->write(&item->buf[item->wi], 1);
		if(n != 1) {return ACP_ERROR_WRITE;}
		//printd(item->buf[item->wi]);
		if(item->buf[item->wi] == ACP_DELIMITER_END){
			 //printd("done) "); 
			 return ACP_DONE;
		}
		item->wi++;
	}
	return ACP_DONE;
}

