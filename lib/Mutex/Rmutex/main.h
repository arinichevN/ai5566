#ifndef RMUTEX_H
#define RMUTEX_H

#include "../client/main.h"

typedef struct rmutex_st Rmutex;
struct rmutex_st{
	MutexClient *first;
	MutexClient *last;
};

extern void rmutex_begin(Rmutex *self);

extern int rmutex_lock(Rmutex *self, MutexClient *client);

extern void rmutex_unlock(Rmutex *self, MutexClient *client);

#endif 
