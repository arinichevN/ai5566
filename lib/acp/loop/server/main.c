#include "main.h"

void acpls_nodeFunctionIdle (Acpls *self, HardwareSerial *serial){
	printdln("acpls: idle function");
	acpls_reset(self);
}

void acpls_READ_REQUEST(Acpls *self, HardwareSerial *serial);
void acpls_CONSIDER_REQUEST(Acpls *self, HardwareSerial *serial);
void acpls_SEND_RESPONSE(Acpls *self, HardwareSerial *serial);

Acpls *acpls_new(){
	size_t sz = sizeof (Acpls);
	Acpls *out = (Acpls *) malloc(sz);
	if(out == NULL){ printdln("acpls_new: failed"); return out;}
	printd("acpls_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acpls_begin(Acpls **self, AcplsCommandNodeList *nodes){
	Acpls *tself = acpls_new();
	if(tself == NULL) return 0;
	tself->acpl = NULL;
	tself->nodes = nodes;
	if(!acpl_begin(&tself->acpl)) return 0;
	tself->control = acpls_READ_REQUEST;
	*self = tself;
	return 1;
}

void acpls_reset(Acpls *self){
	if(self == NULL) return;
	acpl_reset(self->acpl);
	self->control = acpls_READ_REQUEST;
}

void acpls_beginWrite(Acpls *self){
	acpls_beginWrite(self->acpl);
	self->control = acpls_SEND_RESPONSE;
}

void acpls_free(Acpls *self){
	if(self == NULL) return;
	acpl_free(self->acpl);
	free(self);
}

void acpls_READ_REQUEST(Acpls *self, HardwareSerial *serial) {
	int r = acpl_readRequest(self->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: self->control = acpls_CONSIDER_REQUEST; return;
		default: acpls_reset(self); return;
	}
}

void acpls_CONSIDER_REQUEST(Acpls *self, HardwareSerial *serial) {
	if(acp_packCheckCRC(self->acpl->buf)){
		char sign = self->acpl->buf[ACP_IND_SIGN];
		if(sign == ACP_SIGN_REQUEST_GET || sign == ACP_SIGN_REQUEST_SET || sign == ACP_SIGN_REQUEST_SET_BROADCAST || sign == ACP_SIGN_REQUEST_GET_BROADCAST){
			int cmd;
			if(acp_packGetCellI(self->acpl->buf, ACP_REQUEST_IND_COMMAND, &cmd)){
				int command_found = 0;
				for(size_t i = 0; i < self->nodes->length; i++) {
					AcplsCommandNode *cnode = &self->nodes->items[i];
					//printd(cmd);printdln(cnode->command);
					if(cmd == cnode->command){
						command_found = 1;
						cnode->func(self, cmd);
						//printdln("server function done");
						break;
					}
				}
				if(!command_found){
					printdln("command not found");
					acpls_reset(self); return;
				}
			}else{
				printdln("failed to read cmd");
				acpls_reset(self); return;
			}
		}else{
			printd("bad sign:");printdln(self->acpl->buf);
			acpls_reset(self); return;
		}
	}else{
		printdln("bad crc");
		acpls_reset(self); return;
	}
}

void acpls_SEND_RESPONSE(Acpls *self, HardwareSerial *serial) {
	int r = acpl_write(self->acpl, serial);
	switch(r){
		case ACP_DONE:
			acpls_reset(self);
			break;
		case ACP_BUSY:
			break;
		default:
			acpls_reset(self);
			break;
	}
}

