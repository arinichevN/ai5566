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

static int noidServer_supportedNone(int command){
	return 0;
}

static int noidServer_supportedSSR(int command){
	switch(command){
		case CMD_NOID_START: case CMD_NOID_STOP: case CMD_NOID_RESET: 
			return 1;
	}
	return 0;
}

static int noidServer_supportedR(int command){
	switch(command){
		case CMD_NOID_RESET: 
			return 1;
	}
	return 0;
}

static int noidServer_supportedE(int command){
	switch(command){
		case CMD_NOID_GET_EXISTS: 
			return 1;
	}
	return 0;
}

void noidServer_sendSupportedCommand(Noid *oid, Acpls *server, int (*checkFunction)(int)){
	int command;
	if(acp_packGetCellI(server->acpl->buf, ACP_REQUEST_IND_PARAM1, &command)){
		int supported = checkFunction(command);
		if(acp_buildPackIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *oid->id, command, supported)){
			acpls_beginWrite(server);
			return;
		}
	}
	acpls_reset(server);
}

void noidServer_sendSupportedNone(Noid *oid, Acpls *server){
	noidServer_sendSupportedCommand(oid, server, noidServer_supportedNone);
}

void noidServer_sendSupportedSSR(Noid *oid, Acpls *server){
	noidServer_sendSupportedCommand(oid, server, noidServer_supportedSSR);
}

void noidServer_sendSupportedR(Noid *oid, Acpls *server){
	noidServer_sendSupportedCommand(oid, server, noidServer_supportedR);
}

void noidServer_sendSupportedE(Noid *oid, Acpls *server){
	noidServer_sendSupportedCommand(oid, server, noidServer_supportedE);
}
