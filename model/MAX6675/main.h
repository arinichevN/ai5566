#ifndef MODEL_SENSOR_MAX6675_H
#define MODEL_SENSOR_MAX6675_H

#include "../../lib/debug.h"
#include "../../lib/common.h"
#include "../interface/iSensor.h"

#define MAX6675_V_TYPE uint16_t

typedef struct max6675_st Max6675;
struct max6675_st {
	MAX6675_V_TYPE v;
	int i;
	Ton tmr;
	int (*control)(Max6675 *self, int cs, int sclk, int miso, double *result, err_t *error_id);
	iSensor im_sensor;
};

extern Max6675 *max6675_new();

#endif 
