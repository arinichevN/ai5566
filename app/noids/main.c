#include "main.h"


Noid noid = NOID_INITIALIZER;
Noid *noid_top = &noid;
Noid noid_extern = NOID_INITIALIZER;
int noid_extern_id = NOID_ID_UNKNOWN;

extern int app_id;

static void noids_serveRequestSelf(void *vself, Noid *oid, void *vserver, int command){
	Acpls *server = (Acpls *) vserver;
	switch(command){
		CASE_NOID_GET_ACP_COMMAND_SUPPORTED(E)
	}
	acpls_reset(server);
}

int noids_check(){
	FOREACH_APP_NOID {
		if(oid->kind != NOID_KIND_EXTERN && oid->id == NOID_ID_UNKNOWN){
			printdln("bad network object ID");
			return 0;
		}
	}
	return 1;
}

void noids_setParam(const NoidsParam *param){
	if(param->is_first == YES){
		noid.kind = NOID_KIND_FIRST;
	}else{
		noid.kind = NOID_KIND_REGULAR;
	}
	noid_extern_id = param->next_external_noid_id;
	printd("set default noids param\n");
}
 
int noids_begin(int default_btn){
	NOID_SET_PARAM(&noid,				NULL,		noid.kind,	noids_serveRequestSelf,		NULL,	&app_id)
	Noid *last_oid = channels_buildNoids(&noid);
	if(last_oid != NULL){
		last_oid->next = &noid_extern;
	}else{
		noid.next = &noid_extern;
	}
	NOID_SET_PARAM(&noid_extern,		NULL,		NOID_KIND_EXTERN,	NULL,		NULL,	&noid_extern_id);
	if(!noids_check()) {
		return 0;
	}
	return 1;
}


