#include "main.h"

const char *acp_getStateStr(int v){
	switch(v){
		case ACP_DONE:return "ACP_DONE";
		case ACP_BUSY:return "ACP_BUSY";
		case ACP_WAIT:return "ACP_WAIT";
		case ACP_SUCCESS:return "ACP_SUCCESS";
		case ACP_NOT_FOUND:return "ACP_NOT_FOUND";
		
		case ACP_INIT:return "ACP_INIT";
		case ACP_READ_REQUEST:return "ACP_READ_REQUEST";
		case ACP_CONSIDER_PACK:return "ACP_CONSIDER_PACK";
		case ACP_READ_RESPONSE:return "ACP_READ_RESPONSE";
		case ACP_SEND_RESPONSE:return "ACP_SEND_RESPONSE";
		case ACP_SEND_REQUEST:return "ACP_SEND_REQUEST";
		case ACP_IDLE:return "ACP_IDLE"; 
		
		case ACP_ERROR:return "ACP_ERROR";
		case ACP_ERROR_BAD_CHANNEL_ID:return "ACP_ERROR_BAD_CHANNEL_ID";
		case ACP_ERROR_WAIT_RESPONSE_TIMEOUT:return "ACP_ERROR_WAIT_RESPONSE_TIMEOUT";
		case ACP_ERROR_PACK_TIMEOUT:return "ACP_ERROR_PACK_TIMEOUT";
		case ACP_ERROR_BUFFER_OVERFLOW:return "ACP_ERROR_BUFFER_OVERFLOW";
		case ACP_ERROR_NO_START_DELIMITER:return "ACP_ERROR_NO_START_DELIMITER";
		case ACP_ERROR_SHORT_PACKAGE:return "ACP_ERROR_SHORT_PACKAGE";
		case ACP_ERROR_FORMAT:return "ACP_ERROR_FORMAT";
		case ACP_ERROR_WRITE:return "ACP_ERROR_WRITE";
		case ACP_ERROR_CRC:return "ACP_ERROR_CRC";
		case ACP_ERROR_BUSY:return "ACP_ERROR_BUSY";
		case ACP_ERROR_QUEUE:return "ACP_ERROR_QUEUE";
		case ACP_ERROR_ACPL_IS_NULL:return "ACP_ERROR_ACPL_IS_NULL";
	}
	return "?";
}


int acp_excludeDefinedCRC(uint8_t *v){
	uint8_t out = *v;
	int f = 0;
	while(out == ACP_DELIMITER_START || out == ACP_DELIMITER_COLUMN ||	out == ACP_DELIMITER_END || out == '\0'){
		crc_update(&out, ACP_CRC_EXTRA);
		f = 1;
	}
	*v = out;
	return f;
}

uint8_t acp_calcStrCRC(const char *str){
	uint8_t out = ACP_CRC_INI;
	crc_updateByStr(&out, str);
	acp_excludeDefinedCRC(&out);
	return out;
}

uint8_t acp_calcPackCRC(const char *pack_str){
	uint8_t out = ACP_CRC_INI;
	size_t l = strlen(pack_str) - 2;
	crc_updateByBuf(&out, pack_str, l); 
	acp_excludeDefinedCRC(&out);
	return out;
}

uint8_t acp_getPackCRC(const char *pack_str){
	uint8_t out = ACP_CRC_INI;
	out = (uint8_t) pack_str[strlen(pack_str) - 2];
	return out;
}

int acp_addCRC(char *buf, size_t buf_len){
	uint8_t crc = acp_calcStrCRC(buf);
	size_t l = strlen(buf);
	if((l + 3) > buf_len) {printdln("acp: failed to add crc"); return 0;}
	buf[l] = crc;
	buf[l+1] = ACP_DELIMITER_END;
	buf[l+2] = '\0';
	return 1;
}

int acp_packCheckCRC(const char *pack_str){
	uint8_t crc1 = acp_getPackCRC(pack_str);
	uint8_t crc2 = acp_calcPackCRC(pack_str);
//	printd("(crc p c: ");printd(crc1);printd(" ");printd(crc2);printd(") ");
	if(!ACP_CHECK_CRC) return 1;
	if(crc2 != crc1) {
		return 0;
	}
	return 1;
}

static size_t acp_gotoCell(const char *pack_str, int cell_ind){
	int c = 0;
	size_t l = strlen(pack_str);
	size_t i;
	for(i = 1; i < l; i++){
		if(c == cell_ind){
			break;
		}
		if(pack_str[i] == ACP_DELIMITER_COLUMN){
			c++;
		}
	}
	return i;
}

int acp_packGetCellS (const char *pack_str, int cell_ind, char *out, size_t len){
	size_t si = acp_gotoCell(pack_str, cell_ind);
	size_t plen = strlen(pack_str);
	for(size_t i = 0;i<len;i++){
		size_t pi = si + i;
		if(pi >= plen){printd("(getS: pi>=plen) "); return 0;}
		char c = pack_str[pi];
		writed(c);
		if(c == ACP_DELIMITER_COLUMN || c == ACP_DELIMITER_END){
			return 1;
		}
		out[i] = c;
	}
	printd("(getS: end ret0) ");
	return 0;
}

int acp_packGetCellI (const char *pack_str, int cell_ind, int *out){
	size_t i = acp_gotoCell(pack_str, cell_ind);
	int n = sscanf(&pack_str[i], "%d" ACP_DELIMITER_COLUMN_STR, out);
	if (n == 1){
		return 1;
	}
	return 0;
}

int acp_packGetCellF (const char *pack_str, int cell_ind, double *out){
	char s[ACP_FLOAT_BUF_LEN];
	memset(s, 0, sizeof s);
	if(!acp_packGetCellS (pack_str, cell_ind, s, ACP_FLOAT_BUF_LEN)){
		return 0;
	}
	*out = strtod(s, NULL);
	return 1;
}

int acp_packGetCellUl(const char *pack_str, int cell_ind, unsigned long *out){
	size_t i = acp_gotoCell(pack_str, cell_ind);
	int n = sscanf(&pack_str[i], "%lu" ACP_DELIMITER_COLUMN_STR, out);
	if (n == 1){
		return 1;
	}
	return 0;
}

//int acp_packGetFTS(const char *pack_str, int channel_id, FTS *out){
	//int tchannel_id;
	//FTS tout;
	//int n = sscanf(pack_str, ACP_DELIMITER_START_STR "%d" ACP_DELIMITER_COLUMN_STR "%f" ACP_DELIMITER_COLUMN_STR "%lu" ACP_DELIMITER_COLUMN_STR "%lu" ACP_DELIMITER_COLUMN_STR "%d" ACP_DELIMITER_COLUMN_STR, &tchannel_id, &tout.value, &tout.tm.tv_sec, &tout.tm.tv_nsec, &tout.state );
	//if (n == 5){
		//if(channel_id == tchannel_id){
			//*out = tout;
			//return 1;
		//}
	//}
	//return 0;
//}

int acp_packGetFTS(const char *pack_str, int channel_id, FTS *out){
	int tchannel_id;
	if(pack_str[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
		return 0;
	}
	if(!acp_packGetCellI (pack_str, ACP_IND_ID, &tchannel_id)){
		return 0;
	}
	if(channel_id != tchannel_id){
		return 0;
	}
	FTS tout;
	if(!acp_packGetCellF (pack_str, ACP_IND_PARAM1, &tout.value)){
		return 0;
	}
	if(!acp_packGetCellUl (pack_str, ACP_IND_PARAM2, &tout.tm.tv_sec)){
		return 0;
	}
	if(!acp_packGetCellUl (pack_str, ACP_IND_PARAM3, &tout.tm.tv_nsec)){
		return 0;
	}
	if(!acp_packGetCellI (pack_str, ACP_IND_PARAM4, &tout.state)){
		return 0;
	}
	*out = tout;
	return 1;
}

int acp_buildPackS(char *buf, size_t buf_max_len, char sign, const char *v1){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS, sign, v1 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSI(char *buf, size_t buf_max_len, char sign, const char *v1, int v2){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%d" ADCS, sign, v1, v2 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSF(char *buf, size_t buf_max_len, char sign, const char *v1, double v2){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%f" ADCS, sign, v1, v2 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackI(char *buf, size_t buf_max_len, char sign, int v){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS, sign, v );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackUl(char *buf, size_t buf_max_len, char sign, unsigned long v){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%lu" ADCS, sign, v );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackF(char *buf, size_t buf_max_len, char sign, double v){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%f" ADCS, sign, v );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIS(char *buf, size_t buf_max_len, char sign, int v1, const char *v2){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%s" ADCS, sign, v1, v2 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackII(char *buf, size_t buf_max_len, char sign, int v1, int v2){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%d" ADCS, sign, v1, v2 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIIII(char *buf, size_t buf_max_len, char sign, int v1, int v2, int v3, int v4){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%d" ADCS "%d" ADCS "%d" ADCS, sign, v1, v2, v3, v4 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIUl(char *buf, size_t buf_max_len, char sign, int v1, unsigned long v2){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%lu" ADCS, sign, v1, v2 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIIUl(char *buf, size_t buf_max_len, char sign, int v1, int v2, unsigned long v3){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%d" ADCS "%lu" ADCS, sign, v1, v2, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIF(char *buf, size_t buf_max_len, char sign, int v1, double v2){
	FLOAT_SURROGATE_DEF(wv, fv, v2)
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS FLOAT_SURROGATE_FORMAT ADCS, sign, v1, wv, fv );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIIF(char *buf, size_t buf_max_len, char sign, int v1, int v2, double v3){
	FLOAT_SURROGATE_DEF(wv, fv, v3)
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%d" ADCS FLOAT_SURROGATE_FORMAT ADCS, sign, v1, v2, wv, fv );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIFTS(char *buf, size_t buf_max_len, char sign, int v1, FTS *v2){
	double dv = v2->value;
	FLOAT_SURROGATE_DEF(wv, fv, dv)
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS FLOAT_SURROGATE_FORMAT ADCS "%lu" ADCS "%lu" ADCS "%d" ADCS, sign, v1, wv, fv, v2->tm.tv_sec, v2->tm.tv_nsec, v2->state );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIUlI(char *buf, size_t buf_max_len, char sign, int v1, unsigned long v2, int v3){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%lu" ADCS "%d" ADCS, sign, v1, v2, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackIII(char *buf, size_t buf_max_len, char sign, int v1, int v2, int v3){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS "%d" ADCS "%d" ADCS, sign, v1, v2, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}


int acp_buildPackIFI(char *buf, size_t buf_max_len, char sign, int v1, double v2, int v3){
	FLOAT_SURROGATE_DEF(wv, fv, v2)
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%d" ADCS FLOAT_SURROGATE_FORMAT ADCS "%d" ADCS, sign, v1, wv, fv, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSII(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, int v3){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%d" ADCS "%d" ADCS, sign, v1, v2, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSIII(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, int v3, int v4){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%d" ADCS "%d" ADCS "%d" ADCS, sign, v1, v2, v3, v4 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSIUl(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, unsigned long v3){
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%d" ADCS "%lu" ADCS, sign, v1, v2, v3 );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}

int acp_buildPackSIF(char *buf, size_t buf_max_len, char sign, const char *v1, int v2, double v3){
	FLOAT_SURROGATE_DEF(wv, fv, v3)
	int r = snprintf(buf, buf_max_len, ADSS "%c" ADCS "%s" ADCS "%d" ADCS FLOAT_SURROGATE_FORMAT ADCS, sign, v1, v2, wv, fv );
	if(r > 0 && r < (int) buf_max_len){
		return acp_addCRC(buf, buf_max_len);
	}
	return 0;
}
