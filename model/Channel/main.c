#include "main.h"

extern void chplr_unlock(Channel *caller);
extern int chplr_lock(Channel *caller);

void chpl_begin(ChannelPortLock *self){
	self->before = NULL;
	self->after = NULL;
	self->state = CHPL_IDLE;
}

static void channel_WAIT(Channel *self);
static void channel_WAIT_OTHERS(Channel *self);
static void channel_READ(Channel *self);
static void channel_OFF(Channel *self);
static void channel_INIT(Channel *self);
static void channel_FAILURE(Channel *self);


state_t channel_getState(Channel *self){
	if(self->control == channel_OFF) {
		return STATE_OFF;
	} else if(self->control == channel_WAIT || self->control == channel_WAIT_OTHERS || self->control == channel_READ) {
		return STATE_RUN;
	} else if(self->control == channel_INIT) {
		return STATE_INIT;
	} else if(self->control == channel_FAILURE) {
		return STATE_FAILURE;
	}
	return STATE_UNKNOWN;
}

err_t channel_getError(Channel *self){
	return self->error_id;
}

static void channel_buildInterfaces(Channel *self){
	;
}

static err_t channel_setParam(Channel *self, size_t ind){
	self->ind = ind;
	ChannelParam param;
	if(!pmem_getChannelParam(&param, ind)){
		printdln("   failed to get channel from NVRAM");
		return ERROR_NVRAM_READ;
	}
	err_t r = channelParam_check(&param);
	if(r != ERROR_NO){
		return r;
	}
	self->id = param.id;
	self->cs = param.cs;
	self->sclk = param.sclk;
	self->miso = param.miso;
	ton_setInterval(&self->tmr, param.poll_interval);
	self->device_kind = param.device_kind;
	self->sensor = NULL;
	switch(self->device_kind){
		case DEVICE_KIND_MAX6675:{
				Max6675 *device = max6675_new();
				if(device != NULL){
					self->sensor = &device->im_sensor;
				}
			}
			break;
		case DEVICE_KIND_MAX31855:{
				Max31855 *device = max31855_new();
				if(device != NULL){
					self->sensor = &device->im_sensor;
				}
			}
			break;
		default:
			break;
	}
	if(self->sensor == NULL){
		return ERROR_DEVICE_KIND;
	}
	return ERROR_NO;
}

void channel_begin(Channel *self, size_t ind){
	printd("beginning channel ");printd(ind); printdln(":");
	channel_buildInterfaces(self);
	self->error_id = channel_setParam(self, ind);
	self->control = channel_INIT;
	printd("\tid: ");printdln(self->id);
	printd("\n");
}


void channel_free(Channel *self){
	if(self->sensor != NULL){
		self->sensor->free(self->sensor->self);
	}
}

int channel_start(Channel *self){
	if(self->control == channel_OFF){
		printd("starting channel ");printd(self->ind);printdln(":");
		if(chplr_lock(self)){
			digitalWrite(self->cs, HIGH);
			digitalWrite(self->sclk, LOW);
			chplr_unlock(self);
		}
		ton_reset(&self->tmr);
		self->sensor->start(self->sensor->self);
		self->control = channel_WAIT;
		return 1;
	}
	return 0;
}

int channel_stop(Channel *self){
	if(self->control == channel_FAILURE){
		return 0;
	}
	printd("stopping channel ");printdln(self->ind);
	if(chplr_lock(self)){
		digitalWrite(self->cs, HIGH);
		digitalWrite(self->sclk, LOW);
		chplr_unlock(self);
	}
	self->output.success = NO;
	self->error_id = ERROR_NO;
	self->control = channel_OFF;
	return 1;
}

int channel_disconnect(Channel *self){
	printd("disconnecting channel ");printdln(self->ind);
	if(self->control != channel_FAILURE){
		if(chplr_lock(self)){
			digitalWrite(self->cs, HIGH);
			digitalWrite(self->sclk, LOW);
			chplr_unlock(self);
		}
	}
	self->output.success = NO;
	self->error_id = ERROR_NO;
	self->control = channel_OFF;
	return 1;
}

int channel_reset(Channel *self){
	printd("restarting channel ");printd(self->ind); printdln(":");
	if(self->control != channel_FAILURE){
		if(chplr_lock(self)){
			digitalWrite(self->cs, HIGH);
			digitalWrite(self->sclk, LOW);
			chplr_unlock(self);
		}
	}
	channel_free(self);
	channel_begin(self, self->ind);
	return 1;
}

static void channel_WAIT(Channel *self){
	if(ton(&self->tmr)){
		self->control = channel_WAIT_OTHERS;
	}
}

static void channel_WAIT_OTHERS(Channel *self){
	if(chplr_lock(self)){
		self->control = channel_READ;
	}
}

static void channel_READ(Channel *self){
	int done = self->sensor->read(self->sensor->self, self->cs, self->sclk, self->miso, &self->output.value, &self->error_id);
	if(done){
		self->output.tm = getCurrentTs();
		if(self->error_id == ERROR_NO){
			self->output.success = YES;
		}else{
			self->output.success = NO;
		}
		//printd("result: "); printd(self->id); printd(" "); printd(self->output.success); printd(" "); printdln(self->output.value);
		ton_reset(&self->tmr);
		chplr_unlock(self);
		self->control = channel_WAIT;
	}
}

static void channel_OFF(Channel *self){
	;
}

static void channel_FAILURE(Channel *self){
	;
}

static void channel_INIT(Channel *self){
	if(self->error_id != ERROR_NO){
		self->control = channel_FAILURE;
		return;
	}
	chpl_begin(&self->port_lock);
	self->output.success = NO;
	self->output.tm = getCurrentTs();
	ton_reset(&self->tmr);
	pinMode(self->cs, OUTPUT);
	pinMode(self->sclk, OUTPUT);
	pinMode(self->miso, INPUT);
	self->control = channel_OFF;
}

#ifdef USE_AOIDS
#include "aoid.c"

void channel_buildAoids(Channel *self, Aoid *next_oid, Aoid *parent_oid, size_t *id){
	channelAoid_build(&self->aoid, next_oid, parent_oid, self, id);
}

#endif

#ifdef USE_NOIDS
#include "noid.c"
#endif

int channel_portIntersectionExists(Channel *channel1, Channel *channel2){
	if(	channel1->cs	==	channel2->cs || 
		channel1->sclk	==	channel2->sclk || 
		channel1->miso	==	channel2->miso
	){
		return 1;
	}
	return 0; 
}
