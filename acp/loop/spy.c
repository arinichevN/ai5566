#include "spy.h"

int acpl_spyInitClients(ACPLSpy *item, size_t count){
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

void acpl_spyAddClient(ACPLSpy *item, void *client){
	VoidList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->item[i] == NULL){
			list->item[i] = client;
			return 1;
		}
	}
	return 0;
}

int acpl_spyDelClient(ACPLSpy *item, void *client){
	VoidList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->item[i] == client){
			list->item[i] = NULL;
			return 1;
		}
	}
	return 0;
}

void acpl_spyFree(ACPLSpy *item) {
	FREE_LIST(&item->client_list);
}

void acpl_spyReset(ACPLSpy *item){
	NULL_LIST(item->client_list)
	item->last_id = -56982;
	item->onRequestFunction = NULL;
	item->onResponseFunction = NULL;
}

int acpl_spyInit(ACPLSpy *item, size_t client_count, int (*onRequestFunction) (void *, int, char*), void (*onResponseFunction)(void *, char *, int, int)) {
	acpl_spyReset(item);
	if(!acpl_spyInitClients(item, client_count)){
		printdln("acpl_spyInit: failed");
		return 0;
	}
	item->onRequestFunction = onRequestFunction;
	item->onResponseFunction = onResponseFunction;
	return 1;
}

void acpl_spy(ACPLSpy *item, ACPL *acpl, HardwareSerial *serial) {
	ton_reset(&item->busy_tmr);
	switch(item->state){
		case ACP_INIT:
		case ACP_READ_REQUEST:{
			int r = acpl_readRequest(acpl, serial);
			switch(r){
				case ACP_BUSY: return;
				case ACP_DONE: item->state = ACP_CONSIDER_REQUEST; return;
				default: ACPLS_RESET return;
			}
			break;}
		case ACP_CONSIDER_REQUEST:
		//	printdln("consider pack");
			if(acp_packCheckCRC(acpl->buf)){
				if(acpl->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_REQUEST){
					int cmd;
					if(!acp_packGetCellI(acpl->buf, ACP_IND_CMD, &cmd)){
						//printdln("request: failed to read cmd");
						ACPLS_RESET return;
					}
					int id;
					if(!acp_packGetCellI(acpl->buf, ACP_IND_ID, &id)){
						//printdln("request: failed to read channel id");
						ACPLS_RESET return;
					}
					item->last_id = id;
					int f = 0;
					if(item->onRequestFunction != NULL){
						for(size_t i=0; i<item->client_list.length; i++){
							if(client_list.item[i]!= NULL){
								f = f || item->onRequestFunction(client_list.item[i], id, cmd);
							}
						}
					}
					if(!f){
						printdln("request: no channels need this request");
						ACPLS_RESET return;
					}
				}else{
					printdln("request: not request sign");
					ACPLS_RESET return;
				}
			}else{
				printdln("request: bad crc");
				ACPLS_RESET return;
			}
			acpl_reset(acpl);
			item->state = ACP_READ_RESPONSE;
			return;
		case ACP_READ_RESPONSE:{
			int r = acpl_readResponse(acpl, serial);
			switch(r){
				case ACP_DONE: item->state = ACP_CONSIDER_RESPONSE; return;
				case ACP_BUSY: return;
				default: ACPLS_RESET return;
			}
			break;}
		case ACP_CONSIDER_RESPONSE:{
			int id;
			int success = 0;
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
					if(client_list.item[i]!= NULL){
						item->onResponseFunction(client_list.item[i], acpl->buf, id, success);
					}
				}
			}
			}
			ACPLS_RESET return;
		default: ACPLS_RESET return;
	}
}
