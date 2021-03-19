#ifndef MODEL_ISENSOR_H
#define MODEL_ISENSOR_H

#include "../../lib/common.h"

typedef struct {
	void *self;
	void (*start)(void *);
	int (*read)(void *, int, int, int, double *, err_t *);
	void (*free)(void *);
} iSensor;

#endif 
