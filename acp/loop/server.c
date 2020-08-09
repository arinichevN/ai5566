#include "server.h"

extern ACPLCommandNode acnodes[];

void acpls_resetNodes(){
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		ACPLCommandNode *node = &acnodes[i];
		node->command = 0;
		node->func = NULL;
	}
}

void acpl_server(ACPL *item, HardwareSerial *serial) {
	ton_reset(&item->busy_tmr);
	switch(item->state){
		case ACP_INIT:
		case ACP_READ_REQUEST:{
			int r = acpl_readRequest(item, serial);
			switch(r){
				case ACP_BUSY: return;
				case ACP_DONE: item->state = ACP_CONSIDER_REQUEST; return;
				default: ACPLS_RESET return;
			}
			break;}
		case ACP_CONSIDER_REQUEST:
			//printdln("consider pack");
			if(acp_packCheckCRC(item->buf)){
				if(item->buf[ACP_BUF_IND_SIGN] == ACP_SIGN_REQUEST){
					int cmd;
					if(acp_packGetCellI(item->buf, ACP_IND_CMD, &cmd)){
						int command_found = 0;
						for(size_t i = 0; i < ACPL_CNODE_COUNT; i++) {
							ACPLCommandNode *cnode = &acnodes[i];
							//printd(cmd);printdln(cnode->command);
							if(cmd == cnode->command){
								command_found = 1;
								if(cnode->func != NULL){
									cnode->func(item, serial);
									//printdln("server function done");
									break;
								}else{
									printdln("no function");
									ACPLS_RESET return;
								}
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
					printd("bad sign:");printdln(item->buf);
					ACPLS_RESET return;
				}
			}else{
				printdln("bad crc");
				ACPLS_RESET return;
			}
			break;
		case ACP_SEND_RESPONSE:{
			int r = acpl_write(item, serial);
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
			break;}
		default:
			ACPLS_RESET
			break;
		
	}
}

