#include "main.h"

void mutexcl_reset(MutexClient *self){
	self->after = NULL;
	self->before = NULL;
	self->state = STATE_IDLE;
}
