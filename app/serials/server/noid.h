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

#define CASE_NOID_GET_ACP_COMMAND_SUPPORTED(name) \
	case CMD_NOID_GET_ACP_COMMAND_SUPPORTED_NEXT:\
			noidServer_sendSupportedNext##name(oid, server);\
			return;\
		case CMD_NOID_GET_ACP_COMMAND_SUPPORTED_FIRST:\
			noidServer_sendSupportedFirst##name(oid, server);\
			return;

extern void noidServer_sendSupportedCommandFirst(Noid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t));

extern void noidServer_sendSupportedCommandNext(Noid *oid, Acpls *server, int *cmds, size_t len, int (*getFunction)(int *, size_t, int));

extern int noidServer_supportedFirst(int *cmds, size_t len);

extern int noidServer_supportedNext(int *cmds, size_t len, int prev_command);

#define DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(name, commands) \
	void noidServer_sendSupportedFirst##name(Noid *oid, Acpls *server){\
		int cmds[] = commands;\
		size_t len = sizeof cmds / sizeof cmds[0];\
		noidServer_sendSupportedCommandFirst(oid, server, cmds, len, noidServer_supportedFirst);\
	}\
	\
	void noidServer_sendSupportedNext##name(Noid *oid, Acpls *server){\
		int cmds[] = commands;\
		size_t len = sizeof cmds / sizeof cmds[0];\
		noidServer_sendSupportedCommandNext(oid, server, cmds, len, noidServer_supportedNext);\
	}

#define DEC_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(name) \
	extern void noidServer_sendSupportedFirst##name(Noid *oid, Acpls *server);\
	extern void noidServer_sendSupportedNext##name(Noid *oid, Acpls *server);

DEC_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(None)
DEC_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(SSR)
DEC_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(R)
DEC_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(E)

#endif
