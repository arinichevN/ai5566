#include "main.h"

#define OWIRE_DEF_PIN			IO_REG_TYPE mask IO_REG_MASK_ATTR = self->bitmask;\
								volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = self->basereg;
#define OWIRE_PIN_MODE_INPUT	DIRECT_MODE_INPUT(reg, mask);
#define OWIRE_PIN_MODE_OUTPUT	DIRECT_MODE_OUTPUT(reg, mask);
#define OWIRE_PIN_WRITE_LOW		DIRECT_WRITE_LOW(reg, mask);
#define OWIRE_PIN_WRITE_HIGH	DIRECT_WRITE_HIGH(reg, mask);
#define OWIRE_PIN_READ			DIRECT_READ(reg, mask)
//=============================RESET====================================
static int owire_RESET1(Owire *self);
static int owire_RESET2(Owire *self);
static int owire_RESET3(Owire *self);
static int owire_RESET4(Owire *self);
static int owire_RESET5(Owire *self);

static int owire_RESET1(Owire *self){
	OWIRE_DEF_PIN
	OWIRE_PIN_MODE_INPUT
	self->reset = owire_RESET2;
	return OWIRE_BUSY;
}

static int owire_RESET2(Owire *self){
	OWIRE_DEF_PIN
	if(OWIRE_PIN_READ == LOW){
		self->reset = owire_RESET1;
		printdln("1wire: no pullup");
		return OWIRE_ERROR_NO_PULLUP;
	}
	OWIRE_PIN_WRITE_LOW
	OWIRE_PIN_MODE_OUTPUT
	ton_setInterval(&self->tmr, 480);
	tonu_reset(&self->tmr);
	self->reset = owire_RESET3;
	return OWIRE_BUSY;
}

static int owire_RESET3(Owire *self){
	if(tonu(&self->tmr)){
		OWIRE_DEF_PIN
		noInterrupts();
		OWIRE_PIN_MODE_INPUT
		ton_setInterval(&self->tmr, 60);//time for slave to answer
		tonu_reset(&self->tmr);
		while(!tonu(&self->tmr)){
			if(OWIRE_PIN_READ == LOW){//presence pulse detected
				interrupts();
				ton_setInterval(&self->tmr, 450);//time for slave to free line
				tonu_reset(&self->tmr);
				self->reset = owire_RESET4;
				return OWIRE_BUSY;
			}
		}
		interrupts();
		printdln("Owire: no device");
		self->reset = owire_RESET1;
		return OWIRE_ERROR_NO_DEVICE;
	}
	return OWIRE_BUSY;
}

static int owire_RESET4(Owire *self){
	OWIRE_DEF_PIN
	if(OWIRE_PIN_READ == HIGH){
		self->reset = owire_RESET5;
		return OWIRE_BUSY;
	}
	if(tonu(&self->tmr)){
		self->reset = owire_RESET1;
		printdln("1wire: high expected");
		return OWIRE_ERROR_TIMEOUT;
	}
	return OWIRE_BUSY;
}

static int owire_RESET5(Owire *self){
	if(tonu(&self->tmr)){
		self->reset = owire_RESET1;
		return OWIRE_DONE;
	}
	return OWIRE_BUSY;
}

//=============================WRITE====================================
static int owire_WRITE1(Owire *self, const uint8_t *v, size_t length);
static int owire_WRITE2(Owire *self, const uint8_t *v, size_t length);
static int owire_WRITE3(Owire *self, const uint8_t *v, size_t length);

static int owire_WRITE1(Owire *self, const uint8_t *v, size_t length){
	self->ind_arr = 0;
	self->write = owire_WRITE2;
	return OWIRE_BUSY;
}

static int owire_WRITE2(Owire *self, const uint8_t *v, size_t length){
	if(self->ind_arr < length){
		self->data = v[self->ind_arr];
		self->ind_byte = 8;
		self->write = owire_WRITE3;
		return OWIRE_BUSY;
	}
	self->write = owire_WRITE1;
	return OWIRE_DONE;
}

static int owire_WRITE3(Owire *self, const uint8_t *v, size_t length){
	if(!self->ind_byte){
		self->ind_arr++;
		self->write = owire_WRITE2;
		return OWIRE_BUSY;
	}
	OWIRE_DEF_PIN
	if (self->data & 1) {
		noInterrupts();
		OWIRE_PIN_WRITE_LOW
		OWIRE_PIN_MODE_OUTPUT
		delayMicroseconds(10);
		OWIRE_PIN_WRITE_HIGH
		interrupts();
		delayMicroseconds(55);
	} else {
		noInterrupts();
		OWIRE_PIN_WRITE_LOW
		OWIRE_PIN_MODE_OUTPUT
		delayMicroseconds(65);
		OWIRE_PIN_WRITE_HIGH
		interrupts();
		delayMicroseconds(5);
	}
	self->ind_byte--;
	self->data >>= 1;
	return OWIRE_BUSY;
}

//==============================READ====================================
static int owire_READ1(Owire *self, uint8_t *output, size_t length);
static int owire_READ2(Owire *self, uint8_t *output, size_t length);
static int owire_READ3(Owire *self, uint8_t *output, size_t length);
static int owire_READ4(Owire *self, uint8_t *output, size_t length);

static int owire_READ1(Owire *self, uint8_t *output, size_t length){
	self->ind_arr = 0;
	self->read = owire_READ2;
	return OWIRE_BUSY;
}

static int owire_READ2(Owire *self, uint8_t *output, size_t length){
	if(self->ind_arr < length){
		self->data = 0;
		self->ind_byte = 8;
		self->read = owire_READ3;
		return OWIRE_BUSY;
	}
	self->read = owire_READ1;
	return OWIRE_DONE;
}

static int owire_READ3(Owire *self, uint8_t *output, size_t length){
	if(!self->ind_byte){//one byte has been read
		output[self->ind_arr] = self->data;
		self->ind_arr++;
		self->read = owire_READ2;
		return OWIRE_BUSY;
	}
	self->data >>= 1;
	OWIRE_DEF_PIN
	uint8_t bit;
	noInterrupts();
	OWIRE_PIN_MODE_OUTPUT
	OWIRE_PIN_WRITE_LOW
	delayMicroseconds(1);
	OWIRE_PIN_MODE_INPUT
	delayMicroseconds(5);
	bit = OWIRE_PIN_READ;
	interrupts();
	if(bit){
		self->data |= 0x80;
	}
	ton_setInterval(&self->tmr, 60);
	tonu_reset(&self->tmr);
	self->read = owire_READ4;
	return OWIRE_BUSY;
}

static int owire_READ4(Owire *self, uint8_t *output, size_t length){
	if(tonu(&self->tmr)){
		self->ind_byte--;
		self->read = owire_READ3;
	}
	return OWIRE_BUSY;
}

//==========================BROADCAST_COMMAND===========================
static int owire_BROADCAST_COMMAND1(Owire *self, uint8_t command);
static int owire_BROADCAST_COMMAND2(Owire *self, uint8_t command);
static int owire_BROADCAST_COMMAND3(Owire *self, uint8_t command);

static int owire_BROADCAST_COMMAND1(Owire *self, uint8_t command){
	int r = self->reset(self);
	switch(r){
		case OWIRE_BUSY:
			return OWIRE_BUSY;
		case OWIRE_DONE:
			self->broadcastCommand = owire_BROADCAST_COMMAND2;
			return OWIRE_BUSY;
	}
	self->broadcastCommand = owire_BROADCAST_COMMAND1;
	return r;
}

//sending skip rom command
static int owire_BROADCAST_COMMAND2(Owire *self, uint8_t command){
	uint8_t v = OW_CMD_SKIP_ROM;
	int r = self->write(self, &v, 1);
	switch(r){
		case OWIRE_BUSY:
			return OWIRE_BUSY;
		case OWIRE_DONE:
			self->broadcastCommand = owire_BROADCAST_COMMAND3;
			return OWIRE_BUSY;
	}
	self->broadcastCommand = owire_BROADCAST_COMMAND1;
	return r;
}

//sending command
static int owire_BROADCAST_COMMAND3(Owire *self, uint8_t command){
	int r = self->write(self, &command, 1);
	switch(r){
		case OWIRE_BUSY:
			return OWIRE_BUSY;
		case OWIRE_DONE:
			self->broadcastCommand = owire_BROADCAST_COMMAND1;
			return OWIRE_DONE;
	}
	self->broadcastCommand = owire_BROADCAST_COMMAND1;
	return r;
}

Owire *owire_new(){
	size_t sz = sizeof (Owire);
	Owire *out = (Owire *) malloc(sz);
	if(out == NULL){ printdln("owire_new: failed"); return out;}
	printd("owire_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

void owire_free(Owire *self){
	if(self == NULL) return;
	free(self);
}

static void owire_init(Owire *self){
	pinMode(self->pin, INPUT);
	self->reset = owire_RESET1;
	self->write = owire_WRITE1;
	self->read = owire_READ1;
	self->broadcastCommand = owire_BROADCAST_COMMAND1;
}

void owire_setParam(Owire *self, int pin){
	self->pin = pin;
	self->bitmask = PIN_TO_BITMASK(pin);
	self->basereg = PIN_TO_BASEREG(pin);
}

void owire_begin(Owire *self){
	owire_init(self);
	rmutex_begin(&self->rmutex);
}

void owire_restart(Owire *self){
	OWIRE_DEF_PIN
	OWIRE_PIN_MODE_INPUT
	self->reset = owire_RESET1;
	self->write = owire_WRITE1;
	self->read = owire_READ1;
	self->broadcastCommand = owire_BROADCAST_COMMAND1;
}

int owire_lock(Owire *self, MutexClient *client){
	return rmutex_lock(&self->rmutex, client);
}

void owire_unlock(Owire *self, MutexClient *client){
	rmutex_unlock(&self->rmutex, client);
}

int owire_getPin(Owire *self){
	return self->pin;
}
//int owire_skipRom(Owire *self){
	//return self->write(self, 0xCC); 
//}

//int owire_matchRom(Owire *self, const uint8_t rom[8]){
	//self->write(self, 0x55);
    //for (int i = 0; i < 8; i++) self->write(self, rom[i]);
//}
