#ifndef TON_H
#define TON_H

typedef struct {
    unsigned long end;
    unsigned long interval;
} Ton;

extern void ton_setInterval(Ton *item, unsigned long interval_ms);

extern void ton_reset(Ton *item);

extern unsigned long ton_timePassed(Ton *item);

extern int ton(Ton *item);

extern int tonr(Ton *item);

#endif 
