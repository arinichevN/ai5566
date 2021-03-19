#ifndef NETWORK_OBJECT_ID_H
#define NETWORK_OBJECT_ID_H

#include "acp/main.h"

#define NOID_ACP_REQUEST_IND_ID			ACP_REQUEST_IND_ID
#define NOID_ACP_REQUEST_IND_PARAM1		ACP_REQUEST_IND_PARAM1
#define NOID_ACP_REQUEST_IND_PARAM2		ACP_REQUEST_IND_PARAM2
#define NOID_ACP_REQUEST_IND_PARAM3		ACP_REQUEST_IND_PARAM3

#define NOID_ID_UNKNOWN 0
#define NOID_ID_BROADCAST 255

typedef enum {
	NOID_KIND_UNKNOWN=0,
	NOID_KIND_FIRST,
	NOID_KIND_REGULAR,
	NOID_KIND_EXTERN
} NoidKind;

typedef struct noid_st Noid;
struct noid_st{
	int *id;
	NoidKind kind;
	Noid *next;
	void (*serveRequest)(void *, Noid *, void *, int);
	void *data;
};

#define NOID_INITIALIZER {.id = NULL, .kind = NOID_KIND_UNKNOWN, .next = NULL,  .serveRequest = NULL, .data = NULL}
#define NOID_RESET(OID) (OID)->id = NULL; (OID)->next = NULL; (OID)->kind = NOID_KIND_UNKNOWN; (OID)->serveRequest = NULL; (OID)->data = NULL; 
#define NOID_SET_PARAM(OID, NEXT, KIND, FUNC, DATA, ID) (OID)->next = NEXT; (OID)->kind = KIND; (OID)->serveRequest = FUNC; (OID)->data = DATA; (OID)->id = (ID);
#define FOREACH_NOID(TOP, ITEM) for(Noid * ITEM=(TOP); ITEM!=NULL; ITEM=ITEM->next)

#endif 
