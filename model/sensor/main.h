#ifndef MODEL_SENSOR_MAIN_H
#define MODEL_SENSOR_MAIN_H

#include "../../util/ton.h"
#include "../../util/common.h"
#include "max6675.h"
#include "max31855.h"

typedef struct {
	int cs;
	int sclk;
	int miso;
	int (*read_func) (int, int, int, double *);
} Sensor;

extern void sensor_begin(Sensor *item);
extern int sensor_read(Sensor *item, double *result);

#endif 
