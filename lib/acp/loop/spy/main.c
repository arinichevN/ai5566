#include "main.h"

#define ACPLY_CLIENT_LIST clients
#define ACPLY_DEF_CLIENT_LIST AcplyClientList *ACPLY_CLIENT_LIST = &self->clients;
#define ACPLY_CLIENT ACPLY_CLIENT_LIST->items[i]
#define ACPLY_FOREACH_CLIENT  for(AcplyClient *client = list->top; client != NULL; client = client->next) {

AcplyClient *acply_newClient(){
	size_t sz = sizeof (AcplyClient);
	AcplyClient *out = (AcplyClient *) malloc(sz);
	if(out == NULL){ printdln("acply_newClient: failed"); return out;}
	printd("acply_newClient: "); printd(sz); printdln(" bytes allocated");
	out->data = NULL;
	out->next = NULL;
	return out;
}

int acply_addClient(Acply *self, iAcplyClient *client){
	AcplyClientLList *list = &self->clients;
	if (list->length < ACPLY_MAX_CLIENTS ) {
		AcplyClient *new_item = acply_newClient();
		if(new_item == NULL){
			return 0;
		}
		new_item->data = client;
		if (list->top == NULL) {
			list->top = new_item;
		} else {
			list->last->next = new_item;
		}
		list->last = new_item;
		list->length++;
		return 1;
	}
	return 0;
}

int acply_delClient(Acply *self, iAcplyClient *client){
	AcplyClientLList *list = &self->clients;
	AcplyClient *prev = NULL;
	for(AcplyClient * curr = list->top; curr != NULL; curr = curr->next){
		if (curr->data == client) {
			if (prev != NULL) {
				prev->next = curr->next;
			} else {
				list->top = curr->next;
			}
			if (curr == list->last) {
				list->last = prev;
			}
			list->length--;
			free(curr);
			return 1;
		}
		prev = curr;
	}
	return 0;
}

void acply_freeClients(Acply *self) {
	AcplyClientLList *list = &self->clients;
	AcplyClient *item = list->top, *temp;
	while(item != NULL) {
		temp = item;
		item = item->next;
		free(temp);
	}
	list->top = NULL;
	list->last = NULL;
	list->length = 0;
}

void acply_resetClients(Acply *self) {
	AcplyClientLList *list = &self->clients;
	list->top = NULL;
	list->last = NULL;
	list->length = 0UL;
}



void acply_free(Acply *self) {
	if(self == NULL) return;
	acpl_free(self->acpl);
	acply_freeClients(self);
	free(self);
}

void acply_init(Acply *self){
	acply_resetClients(self);
	self->last_id = -63;
}

void acply_READ_REQUEST(Acply *self, HardwareSerial *serial);
void acply_CONSIDER_REQUEST(Acply *self, HardwareSerial *serial);
void acply_READ_RESPONSE(Acply *self, HardwareSerial *serial);
void acply_CONSIDER_RESPONSE(Acply *self, HardwareSerial *serial) ;

Acply *acply_new(){
	size_t sz = sizeof (Acply);
	Acply *out = (Acply *) malloc(sz);
	if(out == NULL){ printdln("acply_new: failed");}
	memset(out, 0, sz);
	printd("acply_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acply_begin(Acply **self){
	Acply *tself = acply_new();
	if(tself == NULL) return 0;
	tself->acpl = NULL;
	if(!acpl_begin(&tself->acpl)) return 0;
	acply_init(tself);
	tself->control = acply_READ_REQUEST;
	*self = tself;
	return 1;
}

void acply_reset(Acply *self){
	acpl_reset(self->acpl);
	self->control = acply_READ_REQUEST;
}

void acply_READ_REQUEST(Acply *self, HardwareSerial *serial) {
	int r = acpl_readRequest(self->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: self->control = acply_CONSIDER_REQUEST; return;
	}
	printd("acply_READ_REQUEST failed: "); printdln(acp_getStateStr(r));
	acply_reset(self); 
}

void acply_CONSIDER_REQUEST(Acply *self, HardwareSerial *serial) {
	Acpl *acpl = self->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		char sign = acpl->buf[ACP_IND_SIGN];
		if(sign == ACP_SIGN_REQUEST_GET || sign == ACP_SIGN_REQUEST_SET || sign == ACP_SIGN_REQUEST_SET_BROADCAST || sign == ACP_SIGN_REQUEST_GET_BROADCAST){
			int cmd;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_COMMAND, &cmd)){
				//printdln("request: failed to read cmd");
				acply_reset(self); return;
			}
			int id;
			if(!acp_packGetCellI(acpl->buf, ACP_REQUEST_IND_ID, &id)){
				//printdln("request: failed to read channel id");
				acply_reset(self); return;
			}
			self->last_id = id;
			int response_required = 0;
			AcplyClientLList *list = &self->clients;
			for(AcplyClient *client = list->top; client != NULL; client = client->next) {
				response_required = response_required || client->data->onRequestFunction(client->data->self, acpl->buf, id, cmd);
			}
			if(!response_required){
				//printdln("acply: no channels need response to this request");
				acply_reset(self); return;
			}
		}else{
			//printdln("acply: not request sign");
			acply_reset(self); return;
		}
	}else{
		printdln("acply: bad request crc");
		acply_reset(self); return;
	}
	acply_reset(self);
	self->control = acply_READ_RESPONSE;
}

void acply_READ_RESPONSE(Acply *self, HardwareSerial *serial) {
	int r = acpl_readResponse(self->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: self->control = acply_CONSIDER_RESPONSE; return;
	}
	printd("acply_READ_RESPONSE failed: "); printdln(acp_getStateStr(r));
	acply_reset(self); 
}

void acply_CONSIDER_RESPONSE(Acply *self, HardwareSerial *serial) {
	int id;
	int success = 0;
	Acpl *acpl = self->acpl;
	if(acp_packCheckCRC(acpl->buf)){
		if(acpl->buf[ACP_IND_SIGN] == ACP_SIGN_RESPONSE){
			if(acp_packGetCellI(acpl->buf, ACP_RESPONSE_IND_ID, &id)){
				if(self->last_id == id){
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
	AcplyClientLList *list = &self->clients;
	for(AcplyClient *client = list->top; client != NULL; client = client->next) {
		client->data->onResponseFunction(client->data->self, acpl->buf, id, success);
	}
	acply_reset(self);
}
