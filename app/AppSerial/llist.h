#ifndef APP_SERIAL_LLIST_H
#define APP_SERIAL_LLIST_H

#ifdef USE_AOIDS
#include "../../lib/aoid.h"
#include "../serials/server/aoid.h"
#endif
#include "main.h"

typedef struct {
	AppSerial *top; 
	AppSerial *last; 
	size_t length;
#ifdef USE_AOIDS
	Aoid aoid;
#endif
} AppSerialLList;

#ifdef USE_AOIDS
extern void appSerialList_buildAoids(AppSerialLList *self, Aoid *next_oid,  Aoid *parent_oid, size_t *id);
#endif

#endif
