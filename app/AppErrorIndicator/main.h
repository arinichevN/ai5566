#ifndef APP_ERROR_INDICATOR_H
#define APP_ERROR_INDICATOR_H

#include "../../lib/common.h"

typedef struct apperrorindicator_st AppErrorIndicator;
struct apperrorindicator_st{
	int pin;
	void (*control)(err_t);
};

extern AppErrorIndicator app_error_indicator;

extern void appei_begin(int pin);

extern void appei_control(err_t error_id);

#endif
