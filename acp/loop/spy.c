#include "spy.h"

int acply_initClients(ACPLY *item, size_t count){
	VoidList *list = &item->client_list;
	FREE_LIST(list);
	ALLOC_LIST(list, count);
	if(list->max_length != count) return 0;
	list->length = list->max_length;
	for(size_t i=0; i<list->length; i++){
		list->item[i] = NULL;
	}
	return 1;
}

int acply_addClient(ACPLY *item, void *client){
	VoidList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->item[i] == NULL){
			list->item[i] = client;
			return 1;
		}
	}
	return 0;
}

int acply_delClient(ACPLY *item, void *client){
	VoidList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->item[i] == client){
			list->item[i] = NULL;
			return 1;
		}
	}
	return 0;
}

void acply_free(ACPLY *item) {
	if(item == NULL) return;
	acpl_free(item->acpl);
	FREE_LIST(&item->client_list);
	free(item);
}

void acply_init(ACPLY *item){
	NULL_LIST(&item->client_list)
	item->last_id = -63;
	item->onRequestFunction = NULL;
	item->onResponseFunction = NULL;
}

void acply_READ_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) ;

ACPLY *acply_new(){
	ACPLY *out = (ACPLY *) malloc(sizeof (ACPLY));
	return out;
}

int acply_begin(ACPLY **item){
	ACPLY *titem = acply_new();
	if(titem == NULL) return 0;
	titem->acpl = NULL;
	if(!acpl_begin(&titem->acpl)) return 0;
	acply_init(titem);
	titem->control = acply_READ_REQUEST;
	*item = titem;
	return 1;
}

void acply_reset(ACPLY *item){
	acpl_reset(item->acpl);
	item->control = acply_READ_REQUEST;
}

int acply_setParam(ACPLY *item, size_t client_count, int (*onRequestFunction) (void *, int, int), void (*onResponseFunction)(void *, char *, int, int)) {
	if(!acply_initClients(item, client_count)){
		printdln("acply_Init: failed");
		return 0;
	}
	item->onRequestFunction = onRequestFunction;
	item->onResponseFunction = onResponseFunction;
	return 1;
}

void acply_READ_REQUEST(ACPLY *item, HardwareSerial *serial) {
	int r = acpl_readRequest(item->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: item->control = acply_CONSIDER_REQUEST; return;
		default: acply_reset(item); return;
	}
}

void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial) {
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_REQUEST){
			int cmd;
			if(!acp_packGetCellI(acpl->buf, ACP_IND_CMD, &cmd)){
				//printdln("request: failed to read cmd");
				acply_reset(item); return;
			}
			int id;
			if(!acp_packGetCellI(acpl->buf, ACP_IND_ID, &id)){
				//printdln("request: failed to read channel id");
				acply_reset(item); return;
			}
			item->last_id = id;
			int f = 0;
			if(item->onRequestFunction != NULL){
				for(size_t i=0; i<item->client_list.length; i++){
					if(item->client_list.item[i]!= NULL){
						f = f || item->onRequestFunction(item->client_list.item[i], id, cmd);
					}
				}
			}
			if(!f){
				printdln("request: no channels need this request");
				acply_reset(item); return;
			}
		}else{
			printdln("request: not request sign");
			acply_reset(item); return;
		}
	}else{
		printdln("request: bad crc");
		acply_reset(item); return;
	}
	acply_reset(item);
	item->control = acply_READ_RESPONSE;
}

void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int r = acpl_readResponse(item->acpl, serial);
	switch(r){
		case ACP_DONE: item->control = acply_CONSIDER_RESPONSE; return;
		case ACP_BUSY: return;
	}
	acply_reset(item);
}

void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int id;
	int success = 0;
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_RESPONSE){
			if(acp_packGetCellI(acpl->buf, ACP_IND_ID, &id)){
				if(item->last_id == id){
					success = 1;
				}else{
				printdln("response: bad id");
			}
			}else{
				printdln("response: failed to get id");
			}
		}else{
			printdln("response: not response sign");
		}
	}else{
		printdln("response: bad crc");
	}
	if(item->onResponseFunction != NULL){
		for(size_t i=0; i<item->client_list.length; i++){
			if(item->client_list.item[i]!= NULL){
				item->onResponseFunction(item->client_list.item[i], acpl->buf, id, success);
			}
		}
	}
	acply_reset(item);
}
//void acply_control(ACPLY *item, ACPL *acpl, HardwareSerial *serial) {
	//ton_reset(&acpl->busy_tmr);
	//switch(acpl->state){
		//case ACP_INIT:
		//case ACP_READ_REQUEST:{
			//int r = acpl_readRequest(acpl, serial);
			//switch(r){
				//case ACP_BUSY: return;
				//case ACP_DONE: acpl->state = ACP_CONSIDER_REQUEST; return;
				//default: acpl_reset(acpl); return;
			//}
			//break;}
		//case ACP_CONSIDER_REQUEST:
		////	printdln("consider pack");
			//if(acp_packCheckCRC(acpl->buf)){
				//if(acpl->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_REQUEST){
					//int cmd;
					//if(!acp_packGetCellI(acpl->buf, ACP_IND_CMD, &cmd)){
						////printdln("request: failed to read cmd");
						//acpl_reset(acpl); return;
					//}
					//int id;
					//if(!acp_packGetCellI(acpl->buf, ACP_IND_ID, &id)){
						////printdln("request: failed to read channel id");
						//acpl_reset(acpl); return;
					//}
					//item->last_id = id;
					//int f = 0;
					//if(item->onRequestFunction != NULL){
						//for(size_t i=0; i<item->client_list.length; i++){
							//if(item->client_list.item[i]!= NULL){
								//f = f || item->onRequestFunction(item->client_list.item[i], id, cmd);
							//}
						//}
					//}
					//if(!f){
						//printdln("request: no channels need this request");
						//acpl_reset(acpl); return;
					//}
				//}else{
					//printdln("request: not request sign");
					//acpl_reset(acpl); return;
				//}
			//}else{
				//printdln("request: bad crc");
				//acpl_reset(acpl); return;
			//}
			//acpl_reset(acpl);
			//acpl->state = ACP_READ_RESPONSE;
			//return;
		//case ACP_READ_RESPONSE:{
			//int r = acpl_readResponse(acpl, serial);
			//switch(r){
				//case ACP_DONE: acpl->state = ACP_CONSIDER_RESPONSE; return;
				//case ACP_BUSY: return;
				//default: acpl_reset(acpl); return;
			//}
			//break;}
		//case ACP_CONSIDER_RESPONSE:{
			//int id;
			//int success = 0;
			//if(acp_packCheckCRC(acpl->buf)){
				//if(acpl->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_RESPONSE){
					//if(acp_packGetCellI(acpl->buf, ACP_IND_ID, &id)){
						//if(item->last_id == id){
							//success = 1;
						//}else{
						//printdln("response: bad id");
					//}
					//}else{
						//printdln("response: failed to get id");
					//}
				//}else{
					//printdln("response: not response sign");
				//}
			//}else{
				//printdln("response: bad crc");
			//}
			//if(item->onResponseFunction != NULL){
				//for(size_t i=0; i<item->client_list.length; i++){
					//if(item->client_list.item[i]!= NULL){
						//item->onResponseFunction(item->client_list.item[i], acpl->buf, id, success);
					//}
				//}
			//}
			//}
			//acpl_reset(acpl); return;
		//default: acpl_reset(acpl); return;
	//}
//}
