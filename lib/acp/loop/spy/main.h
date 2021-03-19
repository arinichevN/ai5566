#ifndef ACPL_SPY_H
#define ACPL_SPY_H

#include "../main.h"

#define ACPLY_MAX_CLIENTS		32

typedef struct iacply_client_st{
	void *self;
	int (*onRequestFunction) (void *, char *, int, int);
	void (*onResponseFunction)(void *, char *, int, int);
} iAcplyClient;

typedef struct acply_client_st AcplyClient;
struct acply_client_st{
	iAcplyClient *data;
	AcplyClient *next;
};

typedef struct {
	AcplyClient *top;
	AcplyClient *last;
	size_t length;
} AcplyClientLList;

typedef struct acply_st Acply;
struct acply_st{
	int last_id;
	AcplyClientLList clients;
	Acpl *acpl;
	void (*control) (Acply *, HardwareSerial *);
};

extern int acply_addClient(Acply *self, iAcplyClient *client);

extern int acply_delClient(Acply *self, iAcplyClient *client);

extern void acply_free(Acply *self);

extern int acply_begin(Acply **self);

#define acply_control(self, serial) (self)->control(self, serial)

#endif
