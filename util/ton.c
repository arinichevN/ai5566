#include "ton.h"

void ton_setInterval(Ton *item, unsigned long interval_ms){
	item->interval = interval_ms;
}

unsigned long ton_getInterval(Ton *item){
	return item->interval;
}

void ton_reset(Ton *item){
	unsigned long now = millis();
	item->start = now;
	item->end = item->start + item->interval;
	item->active = 1;
}

void ton_expire(Ton *item){
	item->active = 0;
}

unsigned long ton_getRest(Ton *item){
	unsigned long r = item->end - millis();
	if(r > item->interval){
		return 0;
	}
	return r;
}

//returns 1 when time is over
//returns 0 while waiting for an interval  
int ton(Ton *item){
	if(!item->active) return 1;
	unsigned long now = millis();
	if(item->start < item->end){
		if(now >= item->start && now < item->end){
			return 0;
		}
		item->active = 0;
		return 1;
	}
	if(now >= item->end && now < item->start){
		item->active = 0;
		return 1;
	}
	return 0;
}

//reset on timeout
int tonr(Ton *item){
	unsigned long now = millis();
	if(item->start < item->end){
		if(now >= item->start && now < item->end){
			return 0;
		}
		ton_reset(item);
		return 1;
	}
	if(now >= item->end && now < item->start){
		ton_reset(item);
		return 1;
	}
	return 0;
}

