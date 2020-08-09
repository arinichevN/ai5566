#ifndef TON_H
#define TON_H

typedef struct {
	unsigned long interval;
	unsigned long start;
	unsigned long end;
	int active;
} Ton;

extern void ton_setInterval(Ton *item, unsigned long interval_ms);

extern unsigned long ton_getInterval(Ton *item);

extern void ton_reset(Ton *item);

extern void ton_expire(Ton *item);

extern unsigned long ton_getRest(Ton *item);

extern int ton(Ton *item);

extern int tonr(Ton *item);

#endif 
