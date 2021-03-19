#include "main.h"

void owpoller_setSlaveTimestamp(OwPoller *self){
	struct timespec tm = getCurrentTs();
	for(iOwpollerSlave *slave = self->slaves.top; slave != NULL; slave = slave->next){
		slave->setTimestamp(slave->self, tm);
	}
}

void owpoller_slavePollFailed(OwPoller *self){
	for(iOwpollerSlave *slave = self->slaves.top; slave != NULL; slave = slave->next){
		slave->pollFailed(slave->self);
	}
}

static void owpoller_slaveWaitIntervalChanged(void *vself, iOwpollerSlave *slave){
	OwPoller *self = (OwPoller *) vself;
	if(slave->wait_interval_ms > self->wait_interval_max_ms){
		self->wait_interval_max_ms = slave->wait_interval_ms;
		printd("poller wait interval changed: "); printdln(self->wait_interval_max_ms);
	}
}

static void owpoller_setWaitIntervalMax(OwPoller *self){
	self->wait_interval_max_ms = 0;
	for(iOwpollerSlave *slave = self->slaves.top; slave != NULL; slave = slave->next){
		if(slave->wait_interval_ms > self->wait_interval_max_ms){
			self->wait_interval_max_ms = slave->wait_interval_ms;
		}
	}
	printd("poller wait interval: "); printdln(self->wait_interval_max_ms);
}

int owpoller_hasSlave(OwPoller *self){
	if(self->slaves.top == NULL){
		return 0;
	}
	return 1;
}
static void owpoller_OFF(OwPoller *self);
static void owpoller_WAIT(OwPoller *self);
static void owpoller_LOCK(OwPoller *self);
static void owpoller_BROADCAST_COMMAND(OwPoller *self);
static void owpoller_WAIT_CONVERSION(OwPoller *self);
static void owpoller_POLL_SLAVES1(OwPoller *self);
static void owpoller_POLL_SLAVES2(OwPoller *self);

void owpoller_OFF(OwPoller *self){
	;
}

void owpoller_INIT(OwPoller *self){
	ton_setInterval(&self->tmr, self->poll_interval_ms);
	ton_reset(&self->tmr);
	owpoller_setWaitIntervalMax(self);
	//printdln("poller: init");
	self->control = owpoller_OFF;
	if(owpoller_hasSlave(self)){
		self->control = owpoller_WAIT;
	}
}

static void owpoller_WAIT(OwPoller *self){
	if(tonr(&self->tmr)){
		//printdln("poller: goto lock");
		self->control = owpoller_LOCK;
		return;
	}
}

static void owpoller_LOCK(OwPoller *self){
	if(owire_lock(self->owire, &self->ow_rmutex_cl)){
		//printdln("poller: lock done");
		self->current_slave = self->slaves.top;
		self->control = owpoller_BROADCAST_COMMAND;
		return;
	}
	//printdln("poller: lock failed");
}

static void owpoller_BROADCAST_COMMAND(OwPoller *self){
	int r = owire_broadcastCommand(self->owire, self->command);
	switch(r){
		case OWIRE_BUSY:
			return;
		case OWIRE_DONE:
			owpoller_setSlaveTimestamp(self);
			ton_setInterval(&self->tmr, self->wait_interval_max_ms);
			ton_reset(&self->tmr);
			self->control = owpoller_WAIT_CONVERSION;
			//printd("poller: waiting conversion "); printdln(self->wait_interval_max_ms);
			return;
	}
	owpoller_slavePollFailed(self);
	ton_reset(&self->tmr);
	//printdln("poller: back to wait");
	owire_unlock(self->owire, &self->ow_rmutex_cl);
	self->control = owpoller_WAIT;
}

static void owpoller_WAIT_CONVERSION(OwPoller *self){
	if(tonr(&self->tmr)){
		self->current_slave = self->slaves.top;
		//printdln("poller: polling slaves");
		self->control = owpoller_POLL_SLAVES1;
		return;
	}
}

static void owpoller_POLL_SLAVES1(OwPoller *self){
	if(self->current_slave != NULL){
		//printdln("polling slave...");
		self->control = owpoller_POLL_SLAVES2;
	}else{
		//printdln("no slaves");
		ton_setInterval(&self->tmr, self->poll_interval_ms);
		ton_reset(&self->tmr);
		owire_unlock(self->owire, &self->ow_rmutex_cl);
		self->control = owpoller_WAIT;
	}
}

static void owpoller_POLL_SLAVES2(OwPoller *self){
	int r = self->current_slave->poll(self->current_slave->self, self->owire);
	switch(r){
		case STATE_BUSY:
			return;
		case STATE_DONE:
			self->current_slave = self->current_slave->next;
			self->control = owpoller_POLL_SLAVES1;
			return;
	}
	self->current_slave->pollFailed(self->current_slave->self);
	self->current_slave = self->current_slave->next;
	self->control = owpoller_POLL_SLAVES1;
}

int owpoller_addSlave(OwPoller *self, iOwpollerSlave *slave){
	int r = owpsl_addItem(&self->slaves, slave);
	if(r){
		slave->poller = self;
		slave->pollerOnWaitIntervalChanged = owpoller_slaveWaitIntervalChanged;
		if(self->control == owpoller_OFF){
			ton_setInterval(&self->tmr, self->poll_interval_ms);
			ton_reset(&self->tmr);
			self->control = owpoller_WAIT;
		}
	}
	return r;
}

int owpoller_deleteSlave(OwPoller *self, iOwpollerSlave *slave){
	if(self->control == owpoller_POLL_SLAVES2 && self->current_slave == slave){//this slave is beeing polled now
		self->current_slave = self->current_slave->next;
		self->control = owpoller_POLL_SLAVES1;
		owire_restart(self->owire);
	}
	int r = owpsl_deleteItem(&self->slaves, slave);
	if(r){
		slave->poller = NULL;
		slave->pollerOnWaitIntervalChanged = NULL;
	}
	return r;
}

void owpoller_setParam(OwPoller *self, Owire *owire, unsigned int poll_interval_ms, uint8_t command){
	self->owire = owire;
	self->poll_interval_ms = poll_interval_ms;
	self->command = command;
}

void owpoller_begin(OwPoller *self){
	owpsl_begin(&self->slaves);
	mutexcl_reset(&self->ow_rmutex_cl);
	self->control = owpoller_INIT;
}

unsigned long owpoller_getPollInterval(OwPoller *self){
	return self->poll_interval_ms;
}
