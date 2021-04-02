#ifndef ONE_WIRE_MULTITASK_PORT_H
#define ONE_WIRE_MULTITASK_PORT_H

#include <OneWire.h>
#include "../dstructure.h"
#include "../ton.h"
#include "../Mutex/Rmutex/main.h"
#include "common.h"
#include "util/OneWire_direct_gpio.h"
#include "util/OneWire_direct_regtype.h"

typedef struct owire_st Owire;
struct owire_st{
	int pin;
	IO_REG_TYPE bitmask;
	volatile IO_REG_TYPE *basereg;
	Ton tmr;
	uint8_t ind_byte;
	size_t ind_arr;
	uint8_t data;
	int (*reset) (Owire *);
	int (*write) (Owire *, const uint8_t *, size_t);
	int (*read) (Owire *, uint8_t *, size_t);
	int (*broadcastCommand)(Owire *, uint8_t);
	Rmutex rmutex;
};

#define owire_reset(port) (port)->reset((port))
#define owire_write(port, arr, arr_length) (port)->write((port), arr, arr_length)
#define owire_read(port, arr, arr_length) (port)->read((port), arr, arr_length)
#define owire_broadcastCommand(port, command) (port)->broadcastCommand((port), command)

extern void owire_setParam(Owire *self, int pin);
extern void owire_begin(Owire *self);
extern void owire_restart(Owire *self);
extern int owire_lock(Owire *self, MutexClient *client);
extern void owire_unlock(Owire *self, MutexClient *client);
extern int owire_getPin(Owire *self);

#endif
