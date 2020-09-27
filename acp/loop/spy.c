#include "spy.h"

int acply_initClients(ACPLY *item, size_t count){
	ACPLYClientList *list = &item->client_list;
	list->items = NULL;
	list->length = 0;
	list->items = (void **) malloc(sizeof (*list->items) * count);
	if(list->items == NULL){
		printdln("acply_initClients failed to allocate memory");
		return 0;
	}
	list->length = count;
	printd("acply clients array length = "); printdln(list->length);
	for(size_t i=0; i<list->length; i++){
		list->items[i] = NULL;
	}
	return 1;
}

int acply_addClient(ACPLY *item, void *client){
	ACPLYClientList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->items[i] == NULL){
			list->items[i] = client;
			return 1;
		}
	}
	return 0;
}

int acply_delClient(ACPLY *item, void *client){
	ACPLYClientList *list = &item->client_list;
	for(size_t i=0; i<list->length; i++){
		if(list->items[i] == client){
			list->items[i] = NULL;
			return 1;
		}
	}
	return 0;
}

void acply_free(ACPLY *item) {
	if(item == NULL) return;
	acpl_free(item->acpl);
	free(item->client_list.items);
	item->client_list.items = NULL;
	free(item);
}

void acply_init(ACPLY *item){
	item->client_list.items = NULL;
	item->client_list.length = 0;
	item->last_id = -63;
	item->onRequestFunction = NULL;
	item->onResponseFunction = NULL;
}

void acply_READ_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial);
void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial);
void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) ;

ACPLY *acply_new(){
	size_t sz = sizeof (ACPLY);
	ACPLY *out = (ACPLY *) malloc(sz);
	if(out == NULL){ printdln("acply_new: failed");}
	printd("acply_new: "); printd(sz); printdln(" bytes allocated");
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

int acply_setParam(ACPLY *item, size_t client_count, int (*onRequestFunction) (void *, char *, int, int), void (*onResponseFunction)(void *, char *, int, int)) {
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
	}
	printd("acply_READ_REQUEST failed: "); printdln(acp_getStateStr(r));
	acply_reset(item); 
}

void acply_CONSIDER_REQUEST(ACPLY *item, HardwareSerial *serial) {
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_IND_SIGN] == ACP_SIGN_REQUEST){
			int cmd;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_CMD, &cmd)){
				//printdln("request: failed to read cmd");
				acply_reset(item); return;
			}
			int id;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_ID, &id)){
				//printdln("request: failed to read channel id");
				acply_reset(item); return;
			}
			item->last_id = id;
			int response_required = 0;
			if(item->onRequestFunction != NULL){
				for(size_t i=0; i<item->client_list.length; i++){
					if(item->client_list.items[i] != NULL){
						response_required = response_required || item->onRequestFunction(item->client_list.items[i], acpl->buf, id, cmd);
					}
				}
			}
			if(!response_required){
				printdln("acply: no channels need response to this request");
				acply_reset(item); return;
			}
		}else{
			printdln("acply: not request sign");
			acply_reset(item); return;
		}
	}else{
		printdln("acply: bad request crc");
		acply_reset(item); return;
	}
	acply_reset(item);
	item->control = acply_READ_RESPONSE;
}

void acply_READ_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int r = acpl_readResponse(item->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: item->control = acply_CONSIDER_RESPONSE; return;
	}
	printd("acply_READ_RESPONSE failed: "); printdln(acp_getStateStr(r));
	acply_reset(item); 
}

void acply_CONSIDER_RESPONSE(ACPLY *item, HardwareSerial *serial) {
	int id;
	int success = 0;
	ACPL *acpl = item->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_IND_SIGN] == ACP_SIGN_RESPONSE){
			if(acp_packGetCellI(acpl->buf, ACP_RESPONSE_IND_ID, &id)){
				if(item->last_id == id){
					success = 1;
				}else{
				printdln("acply: bad response id");
			}
			}else{
				printdln("acply: failed to get response id");
			}
		}else{
			printdln("acply: not response sign");
		}
	}else{
		printdln("acply: bad response crc");
	}
	if(item->onResponseFunction != NULL){
		for(size_t i=0; i <item->client_list.length; i++){
			if(item->client_list.items[i] != NULL){
				item->onResponseFunction(item->client_list.items[i], acpl->buf, id, success);
			}
		}
	}
	acply_reset(item);
}
