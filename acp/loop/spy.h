#ifndef ACPL_SPY_H
#define ACPL_SPY_H

#include "../../util/dstructure.h"
#include "main.h"

typedef struct acply_client_st{
	void *self;
	int (*onRequestFunction) (void *, char *, int, int);
	void (*onResponseFunction)(void *, char *, int, int);
} ACPLYClient;

DEC_LIST(ACPLYClient)

typedef struct acply_st{
	int last_id;
	ACPLYClientList client_list;
	int (*onRequestFunction) (void *, char *, int, int);
	void (*onResponseFunction)(void *, char *, int, int);
	ACPL *acpl;
	void (*control) (struct acply_st *, HardwareSerial *);
} ACPLY;

extern int acply_initClients(ACPLY *item, size_t count);

extern int acply_addClient(ACPLY *item, void *client, int (*onRequestFunction) (void *, char *, int, int), void (*onResponseFunction)(void *, char *, int, int));

extern int acply_delClient(ACPLY *item, void *client);

extern void acply_free(ACPLY *item);

extern int acply_begin(ACPLY **item);

extern int acply_setParam(ACPLY *item, size_t client_count);

#define acply_control(item, serial) (item)->control(item, serial)

#endif
