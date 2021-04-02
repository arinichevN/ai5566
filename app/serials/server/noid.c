#include "noid.h"

void noidServer_sendI(Noid *oid, Acpls *server, int v){
	if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendII(Noid *oid, Acpls *server, int v1, int v2){
	if(acp_buildPackIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v1, v2)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendUl(Noid *oid, Acpls *server, unsigned long v){
	if(acp_buildPackIUl(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendF(Noid *oid, Acpls *server, double v){
	if(acp_buildPackIF(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendFts(Noid *oid, Acpls *server, Fts *v){
	if(acp_buildPackIFts(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendS(Noid *oid, Acpls *server, const char *v){
	if(acp_buildPackIS(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendSupportedCommandFirst(Noid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t)){
	int command = getFunction(cmds, len);
	if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, command)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void noidServer_sendSupportedCommandNext(Noid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t, int)){
	int prev_command;
	if(acp_packGetCellI(server->acpl->buf, NOID_ACP_REQUEST_IND_PARAM1, &prev_command)){
		int command = getFunction(cmds, len, prev_command);
		if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, command)){
			acpls_beginWrite(server);
			return;
		}
	}
	acpls_reset(server);
}

int noidServer_supportedFirst(int *cmds, size_t len){
	if(len > 0){
		return cmds[0];
	}
	return CMD_NONE;
}

int noidServer_supportedNext(int *cmds, size_t len, int prev_command){
	int prev_found = 0;
	for(size_t i=0; i<len; i++){
		if(prev_found) return cmds[i];
		if(cmds[i] == prev_command){
			prev_found = 1;
			continue;
		}
	}
	return CMD_NONE;
}

DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(None, VARGS({CMD_NONE}))
DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(SSR, VARGS({ CMD_NOID_START, CMD_NOID_STOP, CMD_NOID_RESET}))
DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(R, VARGS({CMD_NOID_RESET}))
DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(E, VARGS({CMD_NOID_GET_EXISTS}))
