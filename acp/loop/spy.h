#ifndef ACPL_SPY_H
#define ACPL_SPY_H

#include "main.h"

typedef struct {
	int last_id;
	VoidList client_list;
	int (*onRequestFunction) (void *, int, int);
	void (*onResponseFunction)(void *, char *, int, int);
} ACPLSpy;

extern int acpl_spyInitClients(ACPLSpy *item, size_t count);

extern void acpl_spyAddClient(ACPLSpy *item, void *client);

extern int acpl_spyDelClient(ACPLSpy *item, void *client);

extern void acpl_spyFree(ACPLSpy *item);

extern void acpl_spyReset(ACPLSpy *item);

extern int acpl_spyInit(ACPLSpy *item, size_t client_count, int (*onRequestFunction) (void *, int, char*), void (*onResponseFunction)(void *, char *, int, int));

extern void acpl_spy(ACPLSpy *item, ACPL *acpl, HardwareSerial *serial);

#endif
