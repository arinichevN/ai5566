#include "config.h"

int srvc_forThisApp(Acpls *server){
	int id;
	if(acp_packGetCellI(server->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		if(id == app_id){
			return 1;
		}else{
			printdln("\tnot for this app");
			return 0;
		}
	}
	printdln("\tfailed to get app_id");
	return 0;
}

#ifdef USE_AOIDS
Aoid *srvc_getAoid(Acpls *server){
	int id;
	if(acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_ID, &id)){
		FOREACH_APP_AOID{
			if(id == oid->id){
				return oid;
			}
		}
		printdln("\tOID not found");
		return NULL;

	}
	printdln("\tfailed to get oid_id");
	return NULL;
}

void acnf_aoidGetFirst(Acpls *server, int command){
	if(!srvc_forThisApp(server)) {acpls_reset(server); return;}
	int oid_id = AOID_ID_UNKNOWN;
	int oid_parent_id = AOID_ID_UNKNOWN;
	int oid_kind = AOID_KIND_UNKNOWN;
	int oid_description = AOID_DESCRIPTION_UNKNOWN;
	Aoid *oid = &aoid;
	if(oid != NULL){
		oid_id = oid->id;
		oid_kind = oid->kind;
		oid_description = oid->description;
		if(oid->parent != NULL){
			oid_parent_id = oid->parent->id;
		}
	}
	if(acp_buildPack5I(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid_id, oid_parent_id, oid_kind, oid_description)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void acnf_aoidGetNext(Acpls *server, int command){
	if(!srvc_forThisApp(server)) {acpls_reset(server); return;}
	int oid_id = AOID_ID_UNKNOWN;
	int oid_parent_id = AOID_ID_UNKNOWN;
	int oid_kind = AOID_KIND_UNKNOWN;
	int oid_description = AOID_DESCRIPTION_UNKNOWN;
	Aoid *prev_oid = srvc_getAoid(server);
	if(prev_oid != NULL){
		if(prev_oid->next != NULL){
			Aoid *oid = prev_oid->next;
			oid_id = oid->id;
			oid_kind = oid->kind;
			oid_description = oid->description;
			if(oid->parent != NULL){
				oid_parent_id = oid->parent->id;
			}
		}
	}
	if(acp_buildPack5I(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id, oid_id, oid_parent_id, oid_kind, oid_description)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void acnf_aoidServeRequest(Acpls *server, int command){
	if(!srvc_forThisApp(server)) {acpls_reset(server); return;}
	Aoid *oid = srvc_getAoid(server);
	if(oid != NULL){
		if(oid->serveRequest != NULL){
			oid->serveRequest(oid->data, oid, server, command);
			return;
		}
	}
	acpls_reset(server);
}
#endif

#ifdef USE_NOIDS
Noid *srvc_getNoid(Acpls *server){
	int id;
	if(acp_packGetCellI(server->acpl->buf, NOID_ACP_REQUEST_IND_ID, &id)){
		FOREACH_APP_NOID{
			if(id == *oid->id){
				return oid;
			}
		}
		printdln("\tOID not found");
		return NULL;

	}
	printdln("\tfailed to get oid_id");
	return NULL;
}

void acnf_noidServeRequest(Acpls *server, int command){
	Noid *oid = srvc_getNoid(server);
	if(oid != NULL){
		if(oid->serveRequest != NULL){
			oid->serveRequest(oid->data, oid, server, command);
			return;
		}
	}
	acpls_reset(server);
}

void acnf_noidExists(Acpls *server, int command){
	int id;
	if(acp_packGetCellI(server->acpl->buf, NOID_ACP_REQUEST_IND_ID, &id)){
		if(id == NOID_ID_BROADCAST){
			goto found;
		}
		FOREACH_APP_NOID{
			if(id == *oid->id){
				goto found;
			}
		}
		printdln("\tOID not found");
		return NULL;

	}
	printdln("\tfailed to get oid_id");
	acpls_reset(server);
	return;
	found:
	if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, id, 1)){
		acpls_beginWrite(server);
		return;
	}
	printdln("\tfailed to build package");
	acpls_reset(server);
}

void acnf_noidGetFirst(Acpls *server, int command){
	if(noid_top == NULL){
		acpls_reset(server);
		return;
	}
	if(noid_top->kind != NOID_KIND_FIRST){
		acpls_reset(server);
		return;
	}
	int next_id = NOID_ID_UNKNOWN;
	if(noid_top->next != NULL){
		next_id = *noid_top->next->id;
	}
	if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *noid_top->id, next_id)){
		acpls_beginWrite(server);
		return;
	}
	acpls_reset(server);
}

void acnf_noidGetNext(Acpls *server, int command){
	Noid *noid = srvc_getNoid(server);
	if(noid != NULL){
		if(noid->kind != NOID_KIND_EXTERN){
			int next_id = NOID_ID_UNKNOWN;
			if(noid->next != NULL){
				next_id = *noid->next->id;
			}
			if(acp_buildPackII(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, *noid->id, next_id)){
				acpls_beginWrite(server);
				return;
			}
		}
	}
	acpls_reset(server);
}

void acnf_getAppId(Acpls *server, int command){
	int id;
	if(acp_packGetCellI(server->acpl->buf, NOID_ACP_REQUEST_IND_ID, &id)){
		if(id == NOID_ID_BROADCAST){
			if(acp_buildPackI(server->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app_id)){
				acpls_beginWrite(server);
				return;
			}
			printdln("\tfailed to build package");
			acpls_reset(server);
			return;
		}
		printdln("\tbroadcast id expected");
		acpls_reset(server);
		return;
	}
	printdln("\tfailed to get oid_id");
	acpls_reset(server);
}

#endif

AcplsCommandNode acnodes[] = {
#ifdef USE_NOIDS
	{CMD_NOID_GET_FTS,							acnf_noidServeRequest},
	{CMD_NOID_GET_STATE,						acnf_noidServeRequest},
	{CMD_NOID_GET_ERROR,						acnf_noidServeRequest},
	{CMD_NOID_GET_DEVICE_KIND,					acnf_noidServeRequest},
	{CMD_NOID_SET_GOAL,							acnf_noidServeRequest},
	{CMD_NOID_SET_TEXT,							acnf_noidServeRequest},
	{CMD_NOID_SET_TEXT_BLINK,					acnf_noidServeRequest},
	{CMD_NOID_START,							acnf_noidServeRequest},
	{CMD_NOID_STOP,								acnf_noidServeRequest},
	{CMD_NOID_RESET,							acnf_noidServeRequest},
	{CMD_NOID_GET_EXISTS,						acnf_noidExists},
	{CMD_NOID_GET_ACP_COMMAND_SUPPORTED,		acnf_noidServeRequest},
	{CMD_NOID_GET_NEXT,							acnf_noidGetNext},
	{CMD_NOID_GET_FIRST,						acnf_noidGetFirst},
	{CMD_APP_GET_ID,							acnf_getAppId},
#endif

#ifdef USE_AOIDS
	{CMD_AOID_GET_ACP_COMMAND_SUPPORTED,		acnf_aoidServeRequest},
	{CMD_AOID_GET_NEXT,							acnf_aoidGetNext},
	{CMD_AOID_GET_FIRST,						acnf_aoidGetFirst},
	{CMD_AOID_START,							acnf_aoidServeRequest},
	{CMD_AOID_STOP,								acnf_aoidServeRequest},
	{CMD_AOID_RESET,							acnf_aoidServeRequest},
	{CMD_AOID_SET_NVRAM_VALUE,					acnf_aoidServeRequest},
	{CMD_AOID_GET_RAM_VALUE,					acnf_aoidServeRequest},
	{CMD_AOID_GET_NVRAM_VALUE,					acnf_aoidServeRequest},
	{CMD_AOID_DELETE,							acnf_aoidServeRequest},
	{CMD_AOID_ADD,								acnf_aoidServeRequest},
#endif
	
};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])

AcplsCommandNodeList server_nodes = {.items = acnodes, .length = ACPL_CNODE_COUNT, .max_length = ACPL_CNODE_COUNT};
//<?CMD_GET_OBJECT_FIRST, app_id, prev_oid>
//<?CMD_GET_OBJECT_NEXT, app_id, prev_oid>
//<app_id; oid; parent_oid; kind; description>

//<?CMD_GET_OBJECT_ACP_COMMAND_SUPPORTED, app_id, oid, cmd>
//<app_id; oid; cmd; supported>

//<?CMD_SET_OBJECT_PARAM; app_id; oid; param>

//<?CMD_GET_OBJECT_RAM_PARAM; app_id; oid>
//<app_id; oid; param>

//<?CMD_GET_OBJECT_NVRAM_PARAM; app_id; oid>
//<app_id; oid; param; success>

//<?CMD_DELETE_OBJECT; app_id; del_oid>

//<?CMD_ADD_OBJEC; app_id; parent_oid>

