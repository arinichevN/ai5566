#ifndef ACPL_SERVER_H
#define ACPL_SERVER_H

#include "main.h"

typedef struct {
	const char *command;
	void (*func) (ACPL *, HardwareSerial *serial);
} ACPLCommandNode;

#define ACPLS_RESET acpl_reset(item);

extern void acpls_resetNodes(ACPLCommandNode *nodes, size_t length);
extern void acpl_server(ACPL *item, ACPLCommandNode *cnodes, size_t cnode_count, HardwareSerial *serial);

#endif
