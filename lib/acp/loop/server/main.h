#ifndef ACPL_SERVER_H
#define ACPL_SERVER_H

#include "../../../../lib/dstructure.h"
#include "../main.h"

typedef struct acpls_st Acpls;

typedef struct {
	int command;
	void (*func) (Acpls *, int);
} AcplsCommandNode;

DEC_LIST(AcplsCommandNode)

struct acpls_st {
	Acpl *acpl;
	AcplsCommandNodeList *nodes;
	void (*control) (Acpls *, HardwareSerial *);
};


extern int acpls_begin(Acpls **self, AcplsCommandNodeList *nodes);

extern void acpls_reset(Acpls *self);

extern void acpls_beginWrite(Acpls *self);

extern void acpls_free(Acpls *self);

#define acpls_control(self, serial) (self)->control(self, serial)

#endif
