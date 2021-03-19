#ifndef APP_NOIDS_PARAM_H
#define APP_NOIDS_PARAM_H

#include "../../lib/debug.h"
#include "../../lib/common.h"

typedef struct {
	yn_t is_first;
	int next_external_noid_id;
} NoidsParam;

extern void noidsParam_setDefault(NoidsParam *self);

extern err_t noidsParam_check(const NoidsParam *self);

#endif
