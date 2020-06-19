#ifndef MODEL_SENSOR_MAX6675_H
#define MODEL_SENSOR_MAX6675_H

#include "../../util/debug.h"
#include "../../util/common.h"

extern int max6675_read(int cs, int sclk, int miso, double *result);

#endif 
