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
	if(item == NULL) return;
	memset(item->buf, 0, sizeof (*item->buf) * ACP_BUF_MAX_LENGTH); 
	ton_reset(&item->pack_tmr);
	item->len = 0; 
	item->pack = 0; 
	item->wi = 0UL; 
	item->wbuf_size = 0UL; 
	item->state = ACP_INIT;
}

ACPL *acpl_new(){
	ACPL *out = (ACPL *) malloc(sizeof (ACPL));
	return out;
}

void acpl_begin(ACPL *item) {
	if(item == NULL) return;
	ton_setInterval(&item->pack_tmr, ACP_PACK_TIMEOUT_MS);
	ton_setInterval(&item->busy_tmr, ACP_BUSY_TIMEOUT_MS);
	acpl_reset(item);
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

