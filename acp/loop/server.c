#include "server.h"

extern ACPLSCommandNode acnodes[];

void acpls_nodeFunctionIdle (ACPLS *item, HardwareSerial *serial){
	printdln("acpls: idle function");
	ACPLS_RESET
}

void acpls_resetNodes(){
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		ACPLSCommandNode *node = &acnodes[i];
		node->command = 0;
		node->func = acpls_nodeFunctionIdle;
	}
}

void acpls_READ_REQUEST(ACPLS *item, HardwareSerial *serial);
void acpls_CONSIDER_REQUEST(ACPLS *item, HardwareSerial *serial);
void acpls_SEND_RESPONSE(ACPLS *item, HardwareSerial *serial);

ACPLS *acpls_new(){
	size_t sz = sizeof (ACPLS);
	ACPLS *out = (ACPLS *) malloc(sz);
	if(out == NULL){ printdln("acpls_new: failed");}
	printd("acpls_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

int acpls_begin(ACPLS **item){
	ACPLS *titem = acpls_new();
	if(titem == NULL) return 0;
	titem->acpl = NULL;
	if(!acpl_begin(&titem->acpl)) return 0;
	titem->control = acpls_READ_REQUEST;
	*item = titem;
	return 1;
}

void acpls_reset(ACPLS *item){
	if(item == NULL) return;
	acpl_reset(item->acpl);
	item->control = acpls_READ_REQUEST;
}

void acpls_prepWrite(ACPLS *item){
	acpl_prepWrite(item->acpl);
	item->control = acpls_SEND_RESPONSE;
}

void acpls_free(ACPLS *item){
	if(item == NULL) return;
	acpl_free(item->acpl);
	free(item);
}

void acpls_READ_REQUEST(ACPLS *item, HardwareSerial *serial) {
	int r = acpl_readRequest(item->acpl, serial);
	switch(r){
		case ACP_BUSY: return;
		case ACP_DONE: item->control = acpls_CONSIDER_REQUEST; return;
		default: ACPLS_RESET return;
	}
}

void acpls_CONSIDER_REQUEST(ACPLS *item, HardwareSerial *serial) {
	if(acp_packCheckCRC(item->acpl->buf)){
		char sign = item->acpl->buf[ACP_IND_SIGN];
		if(sign == ACP_SIGN_REQUEST_GET || sign == ACP_SIGN_REQUEST_SET || sign == ACP_SIGN_REQUEST_SET_BROADCAST || sign == ACP_SIGN_REQUEST_GET_BROADCAST){
			int cmd;
			if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_CMD, &cmd)){
				int command_found = 0;
				for(size_t i = 0; i < ACPL_CNODE_COUNT; i++) {
					ACPLSCommandNode *cnode = &acnodes[i];
					//printd(cmd);printdln(cnode->command);
					if(cmd == cnode->command){
						command_found = 1;
						cnode->func(item, serial);
						//printdln("server function done");
						break;
					}
				}
				if(!command_found){
					printdln("command not found");
					ACPLS_RESET return;
				}
			}else{
				printdln("failed to read cmd");
				ACPLS_RESET return;
			}
		}else{
			printd("bad sign:");printdln(item->acpl->buf);
			ACPLS_RESET return;
		}
	}else{
		printdln("bad crc");
		ACPLS_RESET return;
	}
}

void acpls_SEND_RESPONSE(ACPLS *item, HardwareSerial *serial) {
	int r = acpl_write(item->acpl, serial);
	switch(r){
		case ACP_DONE:
			ACPLS_RESET
			break;
		case ACP_BUSY:
			break;
		default:
			ACPLS_RESET
			break;
	}
}

