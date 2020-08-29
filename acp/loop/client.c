#include "client.h"

#define ACPLC_START(MODE) item->mode = MODE; item->control = acplc_SEND_REQUEST;

void acplc_READ_RESPONSE(ACPLC *item, HardwareSerial *serial);
void acplc_SEND_REQUEST(ACPLC *item, HardwareSerial *serial);
void acplc_IDLE(ACPLC *item, HardwareSerial *serial);
void acplc_DONE(ACPLC *item, HardwareSerial *serial);
void acplc_ERROR(ACPLC *item, HardwareSerial *serial);
	
	
ACPLC *acplc_new(){
	ACPLC *out = (ACPLC *) malloc(sizeof (ACPLC));
	return out;
}

int acplc_begin(ACPLC **item){
	ACPLC *titem = acplc_new();
	if(titem == NULL) return 0;
	titem->acpl = NULL;
	if(!acpl_begin(&titem->acpl)) return 0;
	titem->control = acplc_IDLE;
	*item = titem;
	return 1;
}

void acplc_reset(ACPLC *item){
	if(item == NULL) return;
	acpl_reset(item->acpl);
	item->control = acplc_IDLE;
}

void acplc_free(ACPLC *item){
	if(item == NULL) return;
	acpl_free(item->acpl);
	free(item);
}

int acplc_getState(ACPLC *item){
	if(item->control == acplc_IDLE) return ACP_IDLE;
	else if(item->control == acplc_SEND_REQUEST) return ACP_SEND_REQUEST;
	else if(item->control == acplc_READ_RESPONSE) return ACP_READ_RESPONSE;
	else if(item->control == acplc_DONE) return ACP_DONE;
	else if(item->control == acplc_ERROR) return ACP_ERROR;
	return ACP_NOT_FOUND;
}

int acplc_sendII(ACPLC *item, char sign, int v1, int v2){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendII: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_sendIF(ACPLC *item, char sign, int v1, double v2){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackIF(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendII: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_sendIII(ACPLC *item, char sign, int v1, int v2, int v3){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendIII: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_sendIIII(ACPLC *item, char sign, int v1, int v2, int v3, int v4){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackIIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3, v4);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendIIII: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_sendIIUl(ACPLC *item, char sign, int v1, int v2, unsigned long v3){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_sendIIUl: start)");
			acpl_reset(item->acpl);
			int r = acp_buildPackIIUl(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			printdln(item->acpl->buf);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendIIUl: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_sendIIF(ACPLC *item, char sign, int v1, int v2, double v3){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_sendIIF: start)");
			acpl_reset(item->acpl);
			int r = acp_buildPackIIF(item->acpl->buf, ACP_BUF_MAX_LENGTH, sign, v1, v2, v3);
			printdln(item->acpl->buf);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_sendIIF: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_getBrII(ACPLC *item, int cmd, int *v1, int *v2){
	//printd("(acplc_getII: ");
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int _v1;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &_v1)){
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM1, v2)){
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			acpl_reset(item->acpl);
			int r = acp_buildPackI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd);
			if(!r) {printd("error) "); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("error) ");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIS(ACPLC *item, int cmd, int channel_id, char *out, size_t slen){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellS (item->acpl->buf, ACP_IND_PARAM1, out, slen)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getIS: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_getFTS(ACPLC *item, int cmd, int channel_id, FTS *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				printd("(acplc_getFTS: bad crc)");
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			if(!acp_packGetFTS(item->acpl->buf, channel_id, out)){
				printd("(acplc_getFTS: bad format)");
				return ACP_ERROR_FORMAT;
			}
			item->control = acplc_IDLE;
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_getFTS: start)");
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getFTS: error)");
			return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_getII(ACPLC *item, int cmd, int channel_id, int *out){
	//printd("(acplc_getII: ");
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) {printd("error) "); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("error) ");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIF(ACPLC *item, int cmd, int channel_id, double *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellF (item->acpl->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getIF: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}
int acplc_getIII(ACPLC *item, int cmd, int channel_id, int id2, int *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM1, &tid2)){
				return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM2, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getIII: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIIII(ACPLC *item, int cmd, int channel_id, int id2, int *v1, int *v2){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM1, &tid2)){
				return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int _v1;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM2, &_v1)){
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_PARAM3, v2)){
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getIII: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIUl(ACPLC *item, int cmd, int channel_id, unsigned long *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_BUF_IND_SIGN] != ACP_SIGN_RESPONSE){
				return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_IND_ID, &tchannel_id)){
				return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellUl (item->acpl->buf, ACP_IND_PARAM1, out)){
				return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			acpl_reset(item->acpl);
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST, cmd, channel_id);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			item->control = acplc_IDLE;
			printd("(acplc_getIUl: error)");
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

void acplc_READ_RESPONSE(ACPLC *item, HardwareSerial *serial){
	//printd("(acpl_client read: ");
	int r = acpl_readResponse(item->acpl, serial);
	switch(r){
		case ACP_DONE:
			//printd("done ");
			item->control = acplc_DONE;
			break;
		case ACP_BUSY:
			//printd("busy ");
			break;
	}
	//	printd("err ");
	item->control = acplc_ERROR;
	//printd(")\n");
}

void acplc_SEND_REQUEST(ACPLC *item, HardwareSerial *serial){
	//printd("(acpl_client write: ");
	int r = acpl_write(item->acpl, serial);
	switch(r){
		case ACP_DONE:
			//printd("done ");
			switch(item->mode){
				case ACPLC_MODE_SEND:
					item->control = acplc_DONE;
					break;
				case ACPLC_MODE_SEND_READ:
					acpl_prepRead(item->acpl);
					item->control = acplc_READ_RESPONSE;
					break;
				default:
					item->control = acplc_ERROR;
					break;
			}
			break;
		case ACP_BUSY:
		//	printd("busy ");
			break;
	}
	//	printd("err ");
	item->control = acplc_ERROR;
//	printd(")\n");
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

//void acpl_client(ACPL *item, HardwareSerial *serial){
	//ton_reset(&item->busy_tmr);
	//switch(item->state){
		//case ACP_READ_RESPONSE:{
			////printd("(acpl_client read: ");
			//int r = acpl_readResponse(item, serial);
			//switch(r){
				//case ACP_DONE:
					////printd("done ");
					//item->control = acplc_DONE;
					//break;
				//case ACP_BUSY:
					////printd("busy ");
					//break;
				//default:
				////	printd("err ");
					//item->state = r;
					//break;
			//}
			////printd(")\n");
			//break;}
		//case ACP_SEND_REQUEST:{
			////printd("(acpl_client write: ");
			//int r = acpl_write(item, serial);
			//switch(r){
				//case ACP_DONE:
					////printd("done ");
					//switch(item->mode){
						//case ACPLC_MODE_SEND:
							//item->control = acplc_DONE;
							//break;
						//case ACPLC_MODE_SEND_READ:
							//acpl_prepRead(item);
							//item->control = acplc_READ_RESPONSE;
							//break;
						//default:
							//item->control = acplc_ERROR;
							//break;
					//}
					//break;
				//case ACP_BUSY:
				////	printd("busy ");
					//break;
				//default:
				////	printd("err ");
					//item->state = r;
					//break;
			//}
		////	printd(")\n");
			//break;}
		//case ACP_IDLE:
			//break;
		//case ACP_DONE://caller has only one cycle to get result from buffer
			////printd("(acpl_client: from done to idle)\n");
			//item->control = acplc_IDLE;
			//break;
		//case ACP_INIT:
		////	printd("(acpl_client: from init to idle)\n");
			//item->control = acplc_IDLE;
			//break;
		//default://error
		////	printd("(acpl_client: from error to idle)\n");
			//item->control = acplc_IDLE;
			//break;
	//}
//}
