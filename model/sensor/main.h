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
	int device_kind;
	int (*read_func) (int, int, int, double *);
} Sensor;

extern void sensor_begin(Sensor *item);
extern int sensor_read(Sensor *item, double *result);
extern int sensor_checkParam(int device_kind, int cs, int sclk, int miso);
extern void sensor_setParam(Sensor *item, int device_kind, int cs, int sclk, int miso);
extern int sensor_getDeviceKind(Sensor *item);

#endif 
