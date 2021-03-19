#include "main.h"

static int rtmutex_pushInQueue(Rtmutex *self, MutexClient *client){
	if(self->first == client){
		return 0;
	} 
	if(self->first == NULL){
		client->before = NULL;
		client->after = NULL;
		client->state = STATE_RUN;
		self->first = client;
	} else {
		self->last->after = client;
		client->before = self->last;
		client->after = NULL;
		client->state = STATE_WAIT;
	}
	self->last = client;
	return 1;
}

static void rtmutex_removeFromQueue(Rtmutex *self, MutexClient *client){
	if(client->before != NULL){
		client->before->after = client->after;
	}
	if(client->after != NULL){
		client->after->before = client->before;
	}
	if(client == self->last){
		self->last = client->before;
	}
	if(client == self->first){
		self->first = client->after;
		ton_reset(&self->tmr);
	}
	client->after = NULL;
	client->before = NULL;
	client->state = STATE_IDLE;
}

int rtmutex_lock(Rtmutex *self, MutexClient *client){
	switch(client->state){
		case STATE_IDLE:
			rtmutex_pushInQueue(self, client);
			if(client->state == STATE_RUN){
				return 1;
			}
			return 0;
		case STATE_WAIT:
			if(client == self->first){
				if(ton(&self->tmr)){
					client->state = STATE_RUN;
					return 1;
				}
			}
			return 0;
		case STATE_RUN:
			return 1;
		default:
			break;
	}
	printdln("rtmutex_lock unknown state");
	return 0;
}

void rtmutex_unlock(Rtmutex *self, MutexClient *client){
	switch(client->state){
		case STATE_IDLE:
			return;
		case STATE_WAIT:
		case STATE_RUN:
			rtmutex_removeFromQueue(self, client);
			return;
		default:
			break;
	}
}

void rtmutex_begin(Rtmutex *self, unsigned long timeout_ms){
	ton_setInterval(&self->tmr, timeout_ms);
	ton_expire(&self->tmr);
	self->first = NULL;
	self->last = NULL;
}
