#ifndef MODEL_SENSOR_MAX31855_H
#define MODEL_SENSOR_MAX31855_H

#include "../../lib/debug.h"
#include "../../lib/common.h"
#include "../interface/iSensor.h"

#define MAX31855_V_TYPE uint32_t

typedef struct max31855_st Max31855;
struct max31855_st {
	MAX31855_V_TYPE v;
	int i;
	Ton tmr;
	int (*control)(Max31855 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
	iSensor im_sensor;
};

extern Max31855 *max31855_new();

#endif 
