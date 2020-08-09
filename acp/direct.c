#include "direct.h"

#define ACPD_BUF buf
#define ACPD_BUF_LEN len
#define ACPD_BUF_IND i
#define ACPD_BUF_PUSH(V) ACPD_BUF[ACPD_BUF_IND] = (char) V; ACPD_BUF_IND++;

#define ACPD_CHECK_BUF_LEN if(ACPD_BUF_IND >= ACPD_BUF_LEN-2) return ACP_ERROR_BUFFER_OVERFLOW;

#define ACPD_WAIT_RESPONSE if (c == -1) {if(ton(&resp_tmr)){return ACP_ERROR_WAIT_RESPONSE_TIMEOUT;}continue;}
#define ACPD_PACK_START if(c == ACP_DELIMITER_START){ton_reset(&pack_tmr);state = 2;}else{return ACP_ERROR_NO_START_DELIMITER;}

#define ACPD_CHECK_PACK_TIME if(ton(&pack_tmr)){return ACP_ERROR_PACK_TIMEOUT;}

#define ACPD_PREP_BUF char ACPD_BUF[ACP_BUF_MAX_LENGTH];memset(ACPD_BUF, 0, sizeof ACPD_BUF);
#define ACPD_WRITE_BUF size_t l = strlen(ACPD_BUF);size_t n = serial->write(ACPD_BUF, l);if(n != l) return 0;

int acpd_readResponse(HardwareSerial *serial, char *buf, size_t len){
	Ton resp_tmr;
	ton_setInterval(&resp_tmr, ACP_RESP_TIMEOUT_MS);
	Ton pack_tmr;
	ton_setInterval(&pack_tmr, ACP_PACK_TIMEOUT_MS);
	ton_reset(&resp_tmr);
	int state = 1;
	size_t i = 0;
	while(1){
		ACPD_CHECK_BUF_LEN
		int c = serial->read();
		printdln(c);
		switch(state){
			case 1://waiting package
				if(c == ACP_DELIMITER_START){
					ACPD_BUF_PUSH(c)
					ton_reset(&pack_tmr);
					state = 2;
				}else{
					if(ton(&resp_tmr)){
						return ACP_ERROR_WAIT_RESPONSE_TIMEOUT;
					}
					continue;
				}
				break;
			case 2://reading package
				ACPD_CHECK_PACK_TIME
				ACPD_BUF_PUSH(c)
				if (c == ACP_DELIMITER_END) {
			      if (i < ACP_MIN_PACK_LENGTH) {return ACP_ERROR_SHORT_PACKAGE;}
				      return ACP_DONE;
				}
				break;
		}
		
	}
	return ACP_ERROR;
}

int acpd_sendSI(HardwareSerial *serial, char sign, const char *v1, int v2){
	ACPD_PREP_BUF
	int r = acp_buildPackSI(ACPD_BUF, sizeof ACPD_BUF, sign, v1, v2);
	if(!r) return r;
	ACPD_WRITE_BUF
	return 1;
}

int acpd_sendSIF(HardwareSerial *serial, char sign, const char *v1, int v2, double v3){
	ACPD_PREP_BUF
	int r = acp_buildPackSIF(ACPD_BUF, sizeof ACPD_BUF, sign, v1, v2, v3);
	if(!r) return r;
	ACPD_WRITE_BUF
	return 1;
}

int acpd_sendSII(HardwareSerial *serial, char sign, const char *v1, int v2, int v3){
	ACPD_PREP_BUF
	int r = acp_buildPackSII(ACPD_BUF, sizeof ACPD_BUF, sign, v1, v2, v3);
	if(!r) return r;
	ACPD_WRITE_BUF
	return 1;
}

int acpd_sendSIUl(HardwareSerial *serial, char sign, const char *v1, int v2, unsigned long v3){
	ACPD_PREP_BUF
	int r = acp_buildPackSIUl(ACPD_BUF, sizeof ACPD_BUF, sign, v1, v2, v3);
	if(!r) return r;
	ACPD_WRITE_BUF
	return 1;
}

int acpd_getFTS(HardwareSerial *serial, const char *cmd, int channel_id, FTS *out){
	int r = acpd_sendSI(serial, ACP_SIGN_REQUEST, cmd, channel_id);
	if(!r) return r;
	ACPD_PREP_BUF
	if(acpd_readResponse(serial, ACPD_BUF, ACP_BUF_MAX_LENGTH) != ACP_DONE){
		return 0;
	}
	if(!acp_packCheckCRC(ACPD_BUF)){
		return 0;
	}
	if(!acp_packGetFTS(ACPD_BUF, channel_id, out)){
		return 0;
	}
	return 1;
}

int acpd_getII(HardwareSerial *serial, const char *cmd, int channel_id, int *out){
	int r = acpd_sendSI(serial, ACP_SIGN_REQUEST, cmd, channel_id);
	if(!r) return r;
	ACPD_PREP_BUF
	if(acpd_readResponse(serial, ACPD_BUF, ACP_BUF_MAX_LENGTH) != ACP_DONE){
		return 0;
	}
	if(!acp_packCheckCRC(ACPD_BUF)){
		return 0;
	}
	if(ACPD_BUF[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
		return 0;
	}
	int tchannel_id;
	if(!acp_packGetCellI (ACPD_BUF, ACP_IND_ID, &tchannel_id)){
		return 0;
	}
	if(tchannel_id != channel_id){
		return 0;
	}
	if(!acp_packGetCellI (ACPD_BUF, ACP_IND_PARAM1, out)){
		return 0;
	}
	return 1;
}

int acpd_getIUl(HardwareSerial *serial, const char *cmd, int channel_id, unsigned long *out){
	int r = acpd_sendSI(serial, ACP_SIGN_REQUEST, cmd, channel_id);
	if(!r) return r;
	ACPD_PREP_BUF
	if(acpd_readResponse(serial, ACPD_BUF, ACP_BUF_MAX_LENGTH) != ACP_DONE){
		return 0;
	}
	if(!acp_packCheckCRC(ACPD_BUF)){
		return 0;
	}
	if(ACPD_BUF[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
		return 0;
	}
	int tchannel_id;
	if(!acp_packGetCellI (ACPD_BUF, ACP_IND_ID, &tchannel_id)){
		return 0;
	}
	if(tchannel_id != channel_id){
		return 0;
	}
	if(!acp_packGetCellUl (ACPD_BUF, APC_IND_PARAM1, out)){
		return 0;
	}
	return 1;
}

int acpd_getIS(HardwareSerial *serial, const char *cmd, int channel_id, char *out, size_t slen){
	int r = acpd_sendSI(serial, ACP_SIGN_REQUEST, cmd, channel_id);
	if(!r) return r;
	ACPD_PREP_BUF
	r = acpd_readResponse(serial, ACPD_BUF, ACP_BUF_MAX_LENGTH);
	printdln(r);
	if(r != ACP_DONE){
		return 0;
	}
	printdln(ACPD_BUF);
	if(!acp_packCheckCRC(ACPD_BUF)){
		return 0;
	}
	if(ACPD_BUF[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
		return 0;
	}
	int tchannel_id;
	if(!acp_packGetCellI (ACPD_BUF, ACP_IND_ID, &tchannel_id)){
		return 0;
	}
	if(tchannel_id != channel_id){
		printd(tchannel_id);
		return 0;
	}
	if(!acp_packGetCellS (ACPD_BUF, ACP_IND_PARAM1, out, slen)){
		return 0;
	}
	return 1;
}


