#ifndef ACPL_SERVER_H
#define ACPL_SERVER_H

#include "main.h"

typedef struct {
	int command;
	void (*func) (ACPL *, HardwareSerial *serial);
} ACPLCommandNode;

#define ACPLS_RESET acpl_reset(item);

extern void acpls_resetNodes();
extern void acpl_server(ACPL *item, HardwareSerial *serial);


#endif
