#ifndef MODEL_SENSOR_MAX31855_H
#define MODEL_SENSOR_MAX31855_H

#include "../../util/debug.h"
#include "../../util/common.h"

extern int max31855_read(int cs, int sclk, int miso, double *result);

#endif 
