#include "aoid.h"

void aoidServer_sendI(Aoid *oid, Acpls *server, int v){
	if(acp_buildPackIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendII(Aoid *oid, Acpls *server, int v1, int v2){
	if(acp_buildPackIIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v1, v2)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendUl(Aoid *oid, Acpls *server, unsigned long v){
	if(acp_buildPackIIUl(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendUlI(Aoid *oid, Acpls *server, unsigned long v1, int v2){
	if(acp_buildPackIIUlI(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v1, v2)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendF(Aoid *oid, Acpls *server, double v){
	if(acp_buildPackIIF(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendFI(Aoid *oid, Acpls *server, double v1, int v2){
	if(acp_buildPackIIFI(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, v1, v2)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}


void aoidServer_sendSupportedCommandFirst(Aoid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t)){
	int command = getFunction(cmds, len);
	if(acp_buildPackIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, command)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void aoidServer_sendSupportedCommandNext(Aoid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t, int)){
	int prev_command;
	if(acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &prev_command)){
		int command = getFunction(cmds, len, prev_command);
		if(acp_buildPackIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, command)){
			acpls_beginWrite(server);
			return;
		}
	}
	acpls_reset(server);
}

static int aoidServer_supportedFirst(int *cmds, size_t len){
	if(len > 0){
		return cmds[0];
	}
	return CMD_NONE;
}

static int aoidServer_supportedNext(int *cmds, size_t len, int prev_command){
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

#define DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(name, commands) \
	void aoidServer_sendSupportedFirst##name(Aoid *oid, Acpls *server){\
		int cmds[] = commands;\
		size_t len = sizeof cmds / sizeof cmds[0];\
		aoidServer_sendSupportedCommandFirst(oid, server, cmds, len, aoidServer_supportedFirst);\
	}\
	\
	void aoidServer_sendSupportedNext##name(Aoid *oid, Acpls *server){\
		int cmds[] = commands;\
		size_t len = sizeof cmds / sizeof cmds[0];\
		aoidServer_sendSupportedCommandNext(oid, server, cmds, len, aoidServer_supportedNext);\
	}

DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(None, VARGS({CMD_NONE}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(GGS, VARGS({CMD_AOID_GET_RAM_VALUE, CMD_AOID_GET_NVRAM_VALUE, CMD_AOID_SET_NVRAM_VALUE}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(GG, VARGS({CMD_AOID_GET_RAM_VALUE, CMD_AOID_GET_NVRAM_VALUE}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(Gr, VARGS({CMD_AOID_GET_RAM_VALUE}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(SSR, VARGS({CMD_AOID_START, CMD_AOID_STOP, CMD_AOID_RESET}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(R, VARGS({CMD_AOID_RESET}))
DEF_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(AD, VARGS({CMD_AOID_ADD, CMD_AOID_DELETE}))
