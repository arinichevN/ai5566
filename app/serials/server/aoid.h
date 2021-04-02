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

#define CASE_AOID_GET_ACP_COMMAND_SUPPORTED(name) \
	case CMD_AOID_GET_ACP_COMMAND_SUPPORTED_NEXT:\
		aoidServer_sendSupportedNext##name(oid, server);\
		return;\
	case CMD_AOID_GET_ACP_COMMAND_SUPPORTED_FIRST:\
		aoidServer_sendSupportedFirst##name(oid, server);\
		return;

#define DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(name) \
	extern void aoidServer_sendSupportedFirst##name(Aoid *oid, Acpls *server);\
	extern void aoidServer_sendSupportedNext##name(Aoid *oid, Acpls *server);

DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(None)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(GGS)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(GG)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(Gr)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(SSR)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(R)
DEC_FUN_AOID_SERVER_SEND_SUPPORTED_COMMAND(AD)

#endif
