#include "ton.h"

void ton_setInterval(Ton *item, unsigned long interval_ms){
	item->interval = interval_ms;
}

void ton_reset(Ton *item){
	item->end = millis() + item->interval;
}

unsigned long ton_timePassed(Ton *item){
	unsigned long now = millis();
	return item->end - now;
}

int ton(Ton *item){
	if(millis() > item->end){
		return 1;
	}
	return 0;
}

//reset on timeout
int tonr(Ton *item){
	unsigned long now = millis();
	if(now > item->end){
		item->end = now + item->interval;
		return 1;
	}
	return 0;
}

