#ifndef ONE_WIRE_MULTITASK_SLAVE_LIST_H
#define ONE_WIRE_MULTITASK_SLAVE_LIST_H

#include "i_slave.h"

typedef struct{
	iOwpollerSlave *top;
	iOwpollerSlave *last;
} OwpollerSlaveList;

extern void owpsl_begin(OwpollerSlaveList *self);

extern int owpsl_addItem(OwpollerSlaveList *self, iOwpollerSlave *item);

extern int owpsl_deleteItem(OwpollerSlaveList *self, iOwpollerSlave *item);


#endif
