#ifndef TON_H
#define TON_H

typedef struct {
	unsigned long interval;
	unsigned long start;
	unsigned long end;
	int active;
} Ton;

extern void ton_setInterval(Ton *self, unsigned long interval_ms);

extern void ton_expire(Ton *self);

//millis
extern void ton_reset(Ton *self);
extern unsigned long ton_getRest(Ton *self);
extern int ton(Ton *self);
extern int tonr(Ton *self);

//micros
extern void tonu_reset(Ton *self);
extern unsigned long tonu_getRest(Ton *self);
extern int tonu(Ton *self);
extern int tonru(Ton *self);

#endif 
