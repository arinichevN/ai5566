#ifndef ONE_WIRE_MULTITASK_ISLAVE_H
#define ONE_WIRE_MULTITASK_ISLAVE_H

#include "../main.h"

typedef struct iowpollerslave_st iOwpollerSlave;
struct iowpollerslave_st{
	void *self;
	iOwpollerSlave *next;
	void *poller;
	void (*pollerOnWaitIntervalChanged)(void *, iOwpollerSlave *);
	unsigned long wait_interval_ms;
	int (*poll) (void *, Owire *);
	void (*setTimestamp)(void *, struct timespec);
	void (*pollFailed)(void *);
};

#endif
