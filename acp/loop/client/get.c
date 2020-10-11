
int acplc_getBrII(ACPLC *item, int cmd, int *v1, int *v2){
	//printd("(acplc_getII: ");
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(item->acpl->buf)){
				acplc_reset(item);
				return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);
				return ACP_ERROR_SIGN;
			}
			int _v1;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &_v1)){
				acplc_reset(item);
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, v2)){
				acplc_reset(item);
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			acplc_reset(item);
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			int r = acp_buildPackI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd);
			if(!r) {printd("error) "); acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			printd("error) ");
			acplc_reset(item);
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
				acplc_reset(item); return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item); return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item); return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id); printd(" != "); printdln(channel_id);
				acplc_reset(item); return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellS (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, out, slen)){
				acplc_reset(item); return ACP_ERROR_FORMAT;
			}
			acplc_reset(item); return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIS: error)");
			acplc_reset(item); return ACP_ERROR;
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
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			if(!acp_packGetFTS(item->acpl->buf, channel_id, out)){
				printd("(acplc_getFTS: bad format)");
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			acplc_reset(item);
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_getFTS: start)");
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getFTS: error)");
			acplc_reset(item);return ACP_ERROR;
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
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			acplc_reset(item);return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			printd("error) ");
			acplc_reset(item);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIF(ACPLC *item, int cmd, int channel_id, double *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellF (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			acplc_reset(item);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIF: error)");
			acplc_reset(item);return ACP_ERROR;
	}
	return ACP_ERROR;
}
int acplc_getIII(ACPLC *item, int cmd, int channel_id, int id2, int *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, &tid2)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM2, out)){
				return ACP_ERROR_FORMAT;
			}
			acplc_reset(item);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIII: error)");
			acplc_reset(item);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIIII(ACPLC *item, int cmd, int channel_id, int id2, int *v1, int *v2){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(item->acpl->buf)){
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, &tid2)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int _v1;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM2, &_v1)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_PARAM3, v2)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			acplc_reset(item);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id, id2);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIII: error)");
			acplc_reset(item);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIUl(ACPLC *item, int cmd, int channel_id, unsigned long *out){
	switch(acplc_getState(item)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			item->control = acplc_IDLE;
			if(!acp_packCheckCRC(item->acpl->buf)){
				acplc_reset(item);return ACP_ERROR_CRC;
			}
			if(item->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(item);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (item->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(item);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellUl (item->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(item);return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(item); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIUl: error)");
			acplc_reset(item);return ACP_ERROR;
	}
	return ACP_ERROR;
}
