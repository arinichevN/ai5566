#ifndef APP_SERVER_AOID_H
#define APP_SERVER_AOID_H

#include "../../../lib/acp/loop/server/main.h"
#include "../../../lib/aoid.h"
#include "../../main.h"

extern void aoidServer_sendI(Aoid *oid, Acpls *server, int v);

extern void aoidServer_sendII(Aoid *oid, Acpls *server, int v1, int v2);

extern void aoidServer_sendUl(Aoid *oid, Acpls *server, unsigned long v);

extern void aoidServer_sendUlI(Aoid *oid, Acpls *server, unsigned long v1, int v2);

extern void aoidServer_sendF(Aoid *oid, Acpls *server, double v);

extern void aoidServer_sendFI(Aoid *oid, Acpls *server, double v1, int v2);

extern void aoidServer_sendSupportedCommand(Aoid *oid, Acpls *server, int (*checkFunction)(int));

extern void aoidServer_sendSupportedNone(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedGGS(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedGG(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedGr(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedSSR(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedR(Aoid *oid, Acpls *server);

extern void aoidServer_sendSupportedAD(Aoid *oid, Acpls *server);

#endif
