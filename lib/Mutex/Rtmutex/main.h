#ifndef RTMUTEX_H
#define RTMUTEX_H

#include "../../ton.h"
#include "../client/main.h"

typedef struct rtmutex_st Rtmutex;
struct rtmutex_st{
	Ton tmr;
	MutexClient *first;
	MutexClient *last;
};

extern void rtmutex_begin(Rtmutex *self, unsigned long timeout_ms);

extern int rtmutex_lock(Rtmutex *self, MutexClient *client);

extern void rtmutex_unlock(Rtmutex *self, MutexClient *client);

#endif 
