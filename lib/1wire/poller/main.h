#ifndef ONE_WIRE_MULTITASK_POLLER_H
#define ONE_WIRE_MULTITASK_POLLER_H

#include "../../ton.h"
#include "../../common.h"
#include "../../Mutex/client/main.h"
#include "../main.h"
#include "i_slave.h"
#include "slave_list.h"

typedef struct owpoller_st OwPoller;
struct owpoller_st{
	uint8_t command;
	unsigned long poll_interval_ms;
	unsigned long wait_interval_max_ms;
	Owire *owire;
	Ton tmr;
	MutexClient ow_rmutex_cl;
	iOwpollerSlave *current_slave;
	OwpollerSlaveList slaves;
	void (*control)(OwPoller *);
};

extern int owpoller_addSlave(OwPoller *self, iOwpollerSlave *slave);

extern int owpoller_deleteSlave(OwPoller *self, iOwpollerSlave *slave);

extern void owpoller_setParam(OwPoller *self, Owire *owire, unsigned int poll_interval_ms, uint8_t command);

extern void owpoller_begin(OwPoller *self);

extern unsigned long owpoller_getPollInterval(OwPoller *self);

#endif
