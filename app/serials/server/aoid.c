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

static int aoidServer_supportedNone(int command){
	return 0;
}

static int aoidServer_supportedGGS(int command){
	switch(command){
		case CMD_AOID_GET_RAM_VALUE: case CMD_AOID_GET_NVRAM_VALUE: case CMD_AOID_SET_NVRAM_VALUE: 
			return 1;
	}
	return 0;
}

static int aoidServer_supportedGG(int command){
	switch(command){
		case CMD_AOID_GET_RAM_VALUE: case CMD_AOID_GET_NVRAM_VALUE: 
			return 1;
	}
	return 0;
}

static int aoidServer_supportedGr(int command){
	switch(command){
		case CMD_AOID_GET_RAM_VALUE:
			return 1;
	}
	return 0;
}

static int aoidServer_supportedSSR(int command){
	switch(command){
		case CMD_AOID_START: case CMD_AOID_STOP: case CMD_AOID_RESET: 
			return 1;
	}
	return 0;
}

static int aoidServer_supportedR(int command){
	switch(command){
		case CMD_AOID_RESET: 
			return 1;
	}
	return 0;
}

static int aoidServer_supportedAD(int command){
	switch(command){
		case CMD_AOID_ADD: case CMD_AOID_DELETE:
			return 1;
	}
	return 0;
}

void aoidServer_sendSupportedCommand(Aoid *oid, Acpls *server, int (*checkFunction)(int)){
	int command;
	if(acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &command)){
		int supported = checkFunction(command);
		if(acp_buildPackIIII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid->id, command, supported)){
			acpls_beginWrite(server);
			return;
		}
	}
	acpls_reset(server);
}

void aoidServer_sendSupportedNone(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedNone);
}

void aoidServer_sendSupportedGGS(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedGGS);
}

void aoidServer_sendSupportedGG(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedGG);
}

void aoidServer_sendSupportedGr(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedGr);
}

void aoidServer_sendSupportedSSR(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedSSR);
}

void aoidServer_sendSupportedR(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedR);
}

void aoidServer_sendSupportedAD(Aoid *oid, Acpls *server){
	aoidServer_sendSupportedCommand(oid, server, aoidServer_supportedAD);
}
