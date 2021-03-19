#ifndef APP_SERVER_NOID_H
#define APP_SERVER_NOID_H

#include "../../../lib/acp/loop/server/main.h"
#include "../../../lib/common.h"
#include "../../../lib/noid.h"

extern void noidServer_sendI(Noid *oid, Acpls *server, int v);

extern void noidServer_sendII(Noid *oid, Acpls *server, int v1, int v2);

extern void noidServer_sendUl(Noid *oid, Acpls *server, unsigned long v);

extern void noidServer_sendF(Noid *oid, Acpls *server, double v);

extern void noidServer_sendFts(Noid *oid, Acpls *server, Fts *v);

extern void noidServer_sendS(Noid *oid, Acpls *server, const char *v);

extern void noidServer_sendSupportedCommand(Noid *oid, Acpls *server, int (*checkFunction)(int));

extern void noidServer_sendSupportedNone(Noid *oid, Acpls *server);

extern void noidServer_sendSupportedSSR(Noid *oid, Acpls *server);

extern void noidServer_sendSupportedR(Noid *oid, Acpls *server);

void noidServer_sendSupportedE(Noid *oid, Acpls *server);

#endif
