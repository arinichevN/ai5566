#include "server.h"

void acpls_resetNodes(ACPLCommandNode *nodes, size_t length){
	for(size_t i = 0; i<length; i++){
		ACPLCommandNode *node = &nodes[i];
		node->command = "";
		node->func = NULL;
	}
}

void acpl_server(ACPL *item, ACPLCommandNode *cnodes, size_t cnode_count, HardwareSerial *serial) {
	ton_reset(&item->busy_tmr);
	switch(item->state){
		case ACP_INIT:
		case ACP_READ_REQUEST:{
			int r = acpl_readRequest(item, serial);
			switch(r){
				case ACP_BUSY: return;
				case ACP_DONE: item->state = ACP_CONSIDER_PACK; return;
				default: ACPLS_RESET return;
			}
			break;}
		case ACP_CONSIDER_PACK:
		//	printdln("consider pack");
			if(acp_packCheckCRC(item->buf)){
				char cmd[ACP_CMD_MAX_LENGTH];
				memset(cmd, 0, sizeof cmd);
				if(acp_packGetCmd(item->buf, cmd, ACP_CMD_MAX_LENGTH)){
					int command_found = 0;
					for(size_t i = 0; i < cnode_count; i++) {
						ACPLCommandNode *cnode = &cnodes[i];
						//printd(cmd);printdln(cnode->command);
						if(strcmp(cmd, cnode->command)==0){
							command_found = 1;
							if(cnode->func != NULL){
								cnode->func(item, serial);
								break;
							}else{
								//printdln("no function");
								ACPLS_RESET return;
							}
							break;
						}
					}
					if(!command_found){
						//printdln("command not found");
						ACPLS_RESET return;
					}
				}else{
					//printdln("failed to read cmd");
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

