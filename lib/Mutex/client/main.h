#ifndef MUTEX_CLIENT_H
#define MUTEX_CLIENT_H

#include "../../common.h"

typedef struct mutexclient_st MutexClient;
struct mutexclient_st{
	MutexClient *before;
	MutexClient *after;
	state_t state;
};

extern void mutexcl_reset(MutexClient *self);


#endif 
