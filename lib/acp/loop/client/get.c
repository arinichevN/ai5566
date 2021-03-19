
int acplc_getBrII(Acplc *self, int cmd, int *v1, int *v2){
	//printd("(acplc_getII: ");
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);
				return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);
				return ACP_ERROR_SIGN;
			}
			int _v1;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &_v1)){
				acplc_reset(self);
				return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, v2)){
				acplc_reset(self);
				return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			acplc_reset(self);
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			int r = acp_buildPackI(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd);
			if(!r) {printd("error) "); acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			printd("error) ");
			acplc_reset(self);
			return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIS(Acplc *self, int cmd, int channel_id, char *out, size_t slen){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self); return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self); return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self); return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id); printd(" != "); printdln(channel_id);
				acplc_reset(self); return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellS (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, out, slen)){
				acplc_reset(self); return ACP_ERROR_FORMAT;
			}
			acplc_reset(self); return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIS: error)");
			acplc_reset(self); return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_getFts(Acplc *self, int cmd, int channel_id, Fts *out){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(self->acpl->buf)){
				printd("(acplc_getFts: bad crc)");
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			if(!acp_packGetFts(self->acpl->buf, channel_id, out)){
				printd("(acplc_getFts: bad format)");
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			acplc_reset(self);
			return ACP_DONE;
		case ACP_IDLE:{
			//printd("(acplc_getFts: start)");
			int r = acp_buildPackII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getFts: error)");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_NOT_FOUND;
}

int acplc_getII(Acplc *self, int cmd, int channel_id, int *out){
	//printd("(acplc_getII: ");
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			//printd("done ret ok or err) ");
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			acplc_reset(self);return ACP_DONE;
		case ACP_IDLE:{
			//printd("try start: ");
			int r = acp_buildPackII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			//printd("busy) ");
			return ACP_BUSY;}
		default:
			printd("error) ");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIF(Acplc *self, int cmd, int channel_id, double *out){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellF (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			acplc_reset(self);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIF: error)");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_ERROR;
}
int acplc_getIII(Acplc *self, int cmd, int channel_id, int id2, int *out){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, &tid2)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM2, out)){
				return ACP_ERROR_FORMAT;
			}
			acplc_reset(self);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackIII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id, id2);
			if(!r) return ACP_ERROR;
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIII: error)");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIIII(Acplc *self, int cmd, int channel_id, int id2, int *v1, int *v2){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int tid2;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, &tid2)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tid2 != id2){
				printd(tid2);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			int _v1;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM2, &_v1)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_PARAM3, v2)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			*v1 = _v1;
			acplc_reset(self);return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackIII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id, id2);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIII: error)");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_ERROR;
}

int acplc_getIUl(Acplc *self, int cmd, int channel_id, unsigned long *out){
	switch(acplc_getState(self)){
		case ACP_READ_RESPONSE:case ACP_SEND_REQUEST:
			return ACP_BUSY;
		case ACP_DONE:
			self->control = acplc_IDLE;
			if(!acp_packCheckCRC(self->acpl->buf)){
				acplc_reset(self);return ACP_ERROR_CRC;
			}
			if(self->acpl->buf[ACP_IND_SIGN] != ACP_SIGN_RESPONSE){
				acplc_reset(self);return ACP_ERROR_SIGN;
			}
			int tchannel_id;
			if(!acp_packGetCellI (self->acpl->buf, ACP_RESPONSE_IND_ID, &tchannel_id)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			if(tchannel_id != channel_id){
				printd(tchannel_id);
				acplc_reset(self);return ACP_ERROR_BAD_CHANNEL_ID;
			}
			if(!acp_packGetCellUl (self->acpl->buf, ACP_RESPONSE_IND_PARAM1, out)){
				acplc_reset(self);return ACP_ERROR_FORMAT;
			}
			return ACP_DONE;
		case ACP_IDLE:{
			int r = acp_buildPackII(self->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_REQUEST_GET, cmd, channel_id);
			if(!r) {acplc_reset(self); return ACP_ERROR;}
			ACPLC_START(ACPLC_MODE_SEND_READ)
			return ACP_BUSY;}
		default:
			printd("(acplc_getIUl: error)");
			acplc_reset(self);return ACP_ERROR;
	}
	return ACP_ERROR;
}
