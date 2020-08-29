#ifndef ACPL_SERVER_H
#define ACPL_SERVER_H

#include "main.h"

#define ACPLS_PREP_SEND acpls_prepWrite(item);
#define ACPLS_RESET acpls_reset(item);

typedef struct acpls_st{
	ACPL *acpl;
	void (*control) (struct acpls_st *, HardwareSerial *);
} ACPLS;

typedef struct {
	int command;
	void (*func) (ACPLS *, HardwareSerial *serial);
} ACPLSCommandNode;


extern int acpls_begin(ACPLS **item);

extern void acpls_resetNodes();

extern void acpls_reset(ACPLS *item);

extern void acpls_prepWrite(ACPLS *item);

extern void acpls_free(ACPLS *item);

#define acpls_control(item, serial) (item)->control(item, serial)

#endif
