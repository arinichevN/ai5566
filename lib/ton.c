#include "ton.h"

void ton_setInterval(Ton *self, unsigned long interval){
	self->interval = interval;
}

void ton_reset(Ton *self){
	unsigned long now = millis();
	self->start = now;
	self->end = self->start + self->interval;
	self->active = 1;
}



void ton_expire(Ton *self){
	self->active = 0;
}

unsigned long ton_getRest(Ton *self){
	unsigned long r = self->end - millis();
	if(r > self->interval){
		return 0;
	}
	return r;
}

//returns 1 when time is over
//returns 0 while waiting for an interval  
int ton(Ton *self){
	if(!self->active) return 1;
	unsigned long now = millis();
	if(self->start < self->end){
		if(now >= self->start && now < self->end){
			return 0;
		}
		self->active = 0;
		return 1;
	}
	if(now >= self->end && now < self->start){
		self->active = 0;
		return 1;
	}
	return 0;
}

//reset on timeout
int tonr(Ton *self){
	unsigned long now = millis();
	if(self->start < self->end){
		if(now >= self->start && now < self->end){
			return 0;
		}
		ton_reset(self);
		return 1;
	}
	if(now >= self->end && now < self->start){
		ton_reset(self);
		return 1;
	}
	return 0;
}

void tonu_reset(Ton *self){
	unsigned long now = micros();
	self->start = now;
	self->end = self->start + self->interval;
	self->active = 1;
}

unsigned long tonu_getRest(Ton *self){
	unsigned long r = self->end - micros();
	if(r > self->interval){
		return 0;
	}
	return r;
}

//returns 1 when time is over
//returns 0 while waiting for an interval  
int tonu(Ton *self){
	if(!self->active) return 1;
	unsigned long now = micros();
	if(self->start < self->end){
		if(now >= self->start && now < self->end){
			return 0;
		}
		self->active = 0;
		return 1;
	}
	if(now >= self->end && now < self->start){
		self->active = 0;
		return 1;
	}
	return 0;
}

//reset on timeout
int tonru(Ton *self){
	unsigned long now = micros();
	if(self->start < self->end){
		if(now >= self->start && now < self->end){
			return 0;
		}
		tonu_reset(self);
		return 1;
	}
	if(now >= self->end && now < self->start){
		tonu_reset(self);
		return 1;
	}
	return 0;
}
