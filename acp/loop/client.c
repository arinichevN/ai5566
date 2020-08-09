#include "client.h"

#define ACPLC_START(MODE) item->mode = MODE; item->state = ACP_SEND_REQUEST;


int acplc_sendII(ACPL *item, char sign, int v1, int v2){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendII: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_sendIF(ACPL *item, char sign, int v1, double v2){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackIF(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendII: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_sendIII(ACPL *item, char sign, int v1, int v2, int v3){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendIII: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_sendIIII(ACPL *item, char sign, int v1, int v2, int v3, int v4){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackIIII(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3, v4);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendIIII: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_sendIIUl(ACPL *item, char sign, int v1, int v2, unsigned long v3){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_sendIIUl: start)");
			acpl_reset(item);
			int r = acp_buildPackIIUl(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			printdln(item->buf);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendIIUl: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_sendIIF(ACPL *item, char sign, int v1, int v2, double v3){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_sendIIF: start)");
			acpl_reset(item);
			int r = acp_buildPackIIF(item->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			printdln(item->buf);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_sendIIF: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_getBrII(ACPL *item, int cmd, int *v1, int *v2){
	//printd("(acplc_getII: ");
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int _v1;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &_v1)){
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM1, v2)){
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			acpl_reset(item);
			int r = acp_buildPackI(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd);
			if(!r) {printd("error) "); return ACP_ERROR;}
			ACPLC_START(ACPL_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("error) ");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIS(ACPL *item, int cmd, int channel_id, char *out, size_t slen){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellS (item->buf, ACP_IND_PARAM1, out, slen)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getIS: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_getFTS(ACPL *item, int cmd, int channel_id, FTS *out){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				printd("(acplc_getFTS: bad crc)");
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			if(!acp_packGetFTS(item->buf, channel_id, out)){
				printd("(acplc_getFTS: bad format)");
				return ACP_ERROR_FORMAT;
			}
			item->state = ACP_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_getFTS: start)");
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getFTS: error)");
			return ACP_ERROR;
	}
	return item->state;
}

int acplc_getII(ACPL *item, int cmd, int channel_id, int *out){
	//printd("(acplc_getII: ");
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) {printd("error) "); return ACP_ERROR;}
			ACPLC_START(ACPL_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("error) ");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIF(ACPL *item, int cmd, int channel_id, double *out){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellF (item->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getIF: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}
int acplc_getIII(ACPL *item, int cmd, int channel_id, int id2, int *out){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM1, &tid2)){
				return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM2, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getIII: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIIII(ACPL *item, int cmd, int channel_id, int id2, int *v1, int *v2){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM1, &tid2)){
				return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int _v1;
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM2, &_v1)){
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->buf, ACP_IND_PARAM3, v2)){
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getIII: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIUl(ACPL *item, int cmd, int channel_id, unsigned long *out){
	switch(item->state){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellUl (item->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item);
			int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPL_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->state = ACP_IDLE;
			printd("(acplc_getIUl: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

void acpl_client(ACPL *item, HardwareSerial *serial){
	ton_reset(&item->busy_tmr);
	switch(item->state){
		case ACP_READ_RESPONSE:{
			//printd("(acpl_client read: ");
			int r = acpl_readResponse(item, serial);
			switch(r){
				case ACP_DONE:
					//printd("done ");
					item->state = ACP_DONE;
					break;
				case ACP_BUSY:
					//printd("busy ");
					break;
				default:
				//	printd("err ");
					item->state = r;
					break;
			}
			//printd(")\n");
			break;}
		case ACP_SEND_REQUEST:{
			//printd("(acpl_client write: ");
			int r = acpl_write(item, serial);
			switch(r){
				case ACP_DONE:
					//printd("done ");
					switch(item->mode){
						case ACPL_MODE_SEND:
							item->state = ACP_DONE;
							break;
						case ACPL_MODE_SEND_READ:
							acpl_prepRead(item);
							item->state = ACP_READ_RESPONSE;
							break;
						default:
							item->state = ACP_ERROR;
							break;
					}
					break;
				case ACP_BUSY:
				//	printd("busy ");
					break;
				default:
				//	printd("err ");
					item->state = r;
					break;
			}
		//	printd(")\n");
			break;}
		case ACP_IDLE:
			break;
		case ACP_DONE://caller has only one cycle to get result from buffer
			//printd("(acpl_client: from done to idle)\n");
			item->state = ACP_IDLE;
			break;
		case ACP_INIT:
		//	printd("(acpl_client: from init to idle)\n");
			item->state = ACP_IDLE;
			break;
		default://error
		//	printd("(acpl_client: from error to idle)\n");
			item->state = ACP_IDLE;
			break;
	}
}
