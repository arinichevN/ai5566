#include "main.h"

static int rmutex_pushInQueue(Rmutex *self, MutexClient *client){
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

static void rmutex_removeFromQueue(Rmutex *self, MutexClient *client){
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
		self->first->state = STATE_RUN;
	}
	client->after = NULL;
	client->before = NULL;
	client->state = STATE_IDLE;
}

int rmutex_lock(Rmutex *self, MutexClient *client){
	switch(client->state){
		case STATE_IDLE:
			rmutex_pushInQueue(self, client);
			if(client->state == STATE_RUN){
				return 1;
			}
			return 0;
		case STATE_WAIT:
			return 0;
		case STATE_RUN:
			return 1;
		default:
			break;
	}
	printdln("rmutex_lock unknown state");
	return 0;
}

void rmutex_unlock(Rmutex *self, MutexClient *client){
	switch(client->state){
		case STATE_IDLE:
			return;
		case STATE_WAIT:
		case STATE_RUN:
			rmutex_removeFromQueue(self, client);
			return;
		default:
			break;
	}
}

void rmutex_begin(Rmutex *self){
	self->first = NULL;
	self->last = NULL;
}
