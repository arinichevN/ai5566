#include "main.h"

static int max6675_INIT(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_WAIT1(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_HIGH(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_CONSIDER(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_WAIT2(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_READ_LOW(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
static int max6675_WAIT3(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);

static int max6675_INIT(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	self->v = 0UL;
	self->i = 15;
	digitalWrite(cs, LOW);
	tonu_reset(&self->tmr);
	self->control = max6675_WAIT1;
	return 0;
}

static int max6675_WAIT1(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->control = max6675_HIGH;
	}
	return 0;
}

static int max6675_HIGH(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(self->i >= 0){
		digitalWrite(sclk, HIGH);
		tonu_reset(&self->tmr);
		self->control = max6675_WAIT2;
	}else{
		digitalWrite(cs, HIGH);
		self->control = max6675_CONSIDER;
	}
	return 0;
}

static int max6675_CONSIDER(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	self->control = max6675_INIT;
	MAX6675_V_TYPE v  = self->v;
	if (v & 0x4) {
		printd( "error: open where CS pin is "); printdln(cs);
		*error_id = ERROR_TERMOCOUPLE_OPEN;
		return 1;
	}
	v >>= 3;
	*result = v * 0.25;
	*error_id = ERROR_NO;
	return 1;
}

static int max6675_WAIT2(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->control = max6675_READ_LOW;
	}
	return 0;
}

static int max6675_READ_LOW(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if (digitalRead(miso)) {
		self->v |= ((MAX6675_V_TYPE)1 << self->i);
	}
	digitalWrite(sclk, LOW);
	tonu_reset(&self->tmr);
	self->control = max6675_WAIT3;
	return 0;
}

static int max6675_WAIT3(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id) {
	if(tonu(&self->tmr)){
		self->i--;
		self->control = max6675_HIGH;
	}
	return 0;
}

static void max6675_start(void *vself){
	Max6675 *self = (Max6675 *) vself;
	self->control = max6675_INIT;
}

static int max6675_read(void *vself, int cs, int sclk, int miso, double *result, err_t *error_id){
	Max6675 *self = (Max6675 *) vself;
	return self->control(self, cs, sclk, miso, result, error_id);
}

static void max6675_free(void *vself){
	free(vself);
}

static void max6675_buildInterfaces(Max6675 *self){
	self->im_sensor.self = self;
	self->im_sensor.start = max6675_start;
	self->im_sensor.read = max6675_read;
	self->im_sensor.free = max6675_free;
}

Max6675 *max6675_new(){
	size_t sz = sizeof (Max6675);
	Max6675 *out = (Max6675 *) malloc(sz);
	if(out != NULL){
		max6675_buildInterfaces(out);
		ton_setInterval(&out->tmr, 1000);
	}else{
		printdln("max6675_new: failed");
	}
	printd("max6675_new: "); printd(sz); printdln(" bytes allocated");
	return out;
}

//err_t max6675_read(int cs, int sclk, int miso, double *result) {
	//MAX6675_V_TYPE v=0;
	//digitalWrite(cs, LOW);
	//delay(1);
	//for (int i = 15; i >= 0; i--) {
		//digitalWrite(sclk, HIGH);
		//delay(1);
		//if (digitalRead(miso)) {
			//v |= ((MAX6675_V_TYPE)1 << i);
		//}
		//digitalWrite(sclk, LOW);
		//delay(1);
	//}
	//digitalWrite(cs, HIGH);
	//if (v & 0x4) {
		//return ERROR_TERMOCOUPLE_OPEN;
	//}
	//v >>= 3;
	//*result = v * 0.25;
	//return ERROR_NO;
//}
