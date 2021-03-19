#include "main.h"

static int max31855_INIT(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_WAIT1(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_HIGH(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_CONSIDER(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_WAIT2(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_READ_LOW(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max31855_WAIT3(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);

static int max31855_INIT(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	self->v = 0UL;
	self->i = 31;
	digitalWrite(cs, LOW);
	tonu_reset(&self->tmr);
	self->control = max31855_WAIT1;
	return 0;
}

static int max31855_WAIT1(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->control = max31855_HIGH;
	}
	return 0;
}

static int max31855_HIGH(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(self->i >= 0){
		digitalWrite(sclk, HIGH);
		tonu_reset(&self->tmr);
		self->control = max31855_WAIT2;
	}else{
		digitalWrite(cs, HIGH);
		self->control = max31855_CONSIDER;
	}
	return 0;
}

static int max31855_CONSIDER(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	self->control = max31855_INIT;
	MAX31855_V_TYPE v  = self->v;
	if (v & 0x20000) {
		printd("warning: bit 18 should be 0 where CS pin is "); printdln(cs);
		*error_id = WARNING_BAD_DATA;
		return 1;
	}
	if (v & 0x8) {
		printd( "warning: bit 4 should be 0 where CS pin is "); printdln(cs);
		*error_id = WARNING_BAD_DATA;
		return 1;
	}
	if (v & 0x4) {
		printd( "error: short to VCC where CS pin is "); printdln(cs);
		*error_id = ERROR_TERMOCOUPLE_SC_VCC;
		return 1;
	}
	if (v & 0x2) {
		printd( "warning: short to GND where CS pin is "); printdln(cs);
		*error_id = ERROR_TERMOCOUPLE_SC_GND;
		return 1;
	}
	if (v & 0x1) {
		printd( "error: open where CS pin is "); printdln(cs);
		*error_id = ERROR_TERMOCOUPLE_OPEN;
		return 1;
	}
	if (v & 0x8000) {
		printd( "error: error where CS pin is "); printdln(cs);
		*error_id = ERROR_SOME;
		return 1;
	}
	if (v & 0x80000000) {// -
		v = ~v;
		v = (v >> 18);
		*result = (v*0.25+0.25)*-1;
	} else {// +
		v >>= 18;
		*result = v*0.25;
	}
	*error_id = ERROR_NO;
	return 1;
}

static int max31855_WAIT2(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->control = max31855_READ_LOW;
	}
	return 0;
}

static int max31855_READ_LOW(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if (digitalRead(miso)) {
		self->v |= ((MAX31855_V_TYPE)1 << self->i);
	}
	digitalWrite(sclk, LOW);
	tonu_reset(&self->tmr);
	self->control = max31855_WAIT3;
	return 0;
}

static int max31855_WAIT3(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->i--;
		self->control = max31855_HIGH;
	}
	return 0;
}

static void max31855_start(void *vself){
	Max31855 *self = (Max31855 *) vself;
	self->control = max31855_INIT;
}

static int max31855_read(void *vself, int cs, int sclk, int miso, double *result, err_t *error_id){
	Max31855 *self = (Max31855 *) vself;
	return self->control(self, cs, sclk, miso, result, error_id);
}

static void max31855_free(void *vself){
	free(vself);
}

static void max31855_buildInterfaces(Max31855 *self){
	self->im_sensor.self = self;
	self->im_sensor.start = max31855_start;
	self->im_sensor.read = max31855_read;
	self->im_sensor.free = max31855_free;
}

Max31855 *max31855_new(){
	size_t sz = sizeof (Max31855);
	Max31855 *out = (Max31855 *) malloc(sz);
	if(out != NULL){
		max31855_buildInterfaces(out);
		ton_setInterval(&out->tmr, 1000);
	}else{
		printdln("max31855_new: failed");
	}
	printd("max31855_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}


//err_t max31855_read(int cs, int sclk, int miso, double *result) {
	//MAX31855_V_TYPE v = 0UL;
	//digitalWrite(cs, LOW);
	//delay(1);
	//for (int i = 31; i >= 0; i--) {
		//digitalWrite(sclk, HIGH);
		//delay(1);
		//if (digitalRead(miso)) {
			//v |= ((MAX31855_V_TYPE)1 << i);
		//}
		//digitalWrite(sclk, LOW);
		//delay(1);
	//}
	//digitalWrite(cs, HIGH);
	//if ( v & 0x20000 ) {
		//printd("warning: bit 18 should be 0 where CS pin is "); printdln(cs);
		//return WARNING_BAD_DATA;
	//}
	//if ( v & 0x8 ) {
		//printd( "warning: bit 4 should be 0 where CS pin is "); printdln(cs);
		//return WARNING_BAD_DATA;
	//}
	//if (v & 0x4) {
		//printd( "error: short to VCC where CS pin is "); printdln(cs);
		//return ERROR_TERMOCOUPLE_SC_VCC;
	//}
	//if (v & 0x2) {
		//printd( "warning: short to GND where CS pin is "); printdln(cs);
		//return ERROR_TERMOCOUPLE_SC_GND;
	//}
	//if (v & 0x1) {
		//printd( "error: open where CS pin is "); printdln(cs);
		//return ERROR_TERMOCOUPLE_OPEN;
	//}
	//if (v & 0x8000) {
		//printd( "error: error where CS pin is "); printdln(cs);
		//return ERROR_SOME;
	//}
	//if (v & 0x80000000) {// -
		//v = ~v;
		//v = (v >> 18);
		//*result = (v *0.25+0.25) *-1;
	//} else {// +
		//v >>= 18;
		//*result = v *0.25;
	//}
	//return ERROR_NO;
//}
