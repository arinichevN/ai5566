#ifndef APP_PARAM_H
#define APP_PARAM_H

#ifdef USE_NOIDS
#include "noids/param.h"
#endif

typedef struct {
	int id;
#ifdef USE_NOIDS
	NoidsParam noids;
#endif
} AppParam;

#define APP_PARAM_GET_FIELD_FUNC(FIELD) appcgff ## FIELD
#define APP_PARAM_DEF_GET_FIELD_FUNC(FIELD) int APP_PARAM_GET_FIELD_FUNC(FIELD)(AppParam *self){return self->FIELD;}
#define APP_PARAM_DEC_GET_FIELD_FUNC(FIELD) extern int APP_PARAM_GET_FIELD_FUNC(FIELD)(AppParam *self);

#define APP_PARAM_SET_FIELD_FUNC(FIELD) appcsff ## FIELD
#define APP_PARAM_DEF_SET_FIELD_FUNC(FIELD) void APP_PARAM_SET_FIELD_FUNC(FIELD)(AppParam *self, int v){self->FIELD = v;}
#define APP_PARAM_DEC_SET_FIELD_FUNC(FIELD) extern void APP_PARAM_SET_FIELD_FUNC(FIELD)(AppParam *self, int v);

APP_PARAM_DEC_GET_FIELD_FUNC(id)
APP_PARAM_DEC_SET_FIELD_FUNC(id)

extern err_t appParam_check(AppParam *self);

extern void appParam_setDefault(AppParam *self);

#endif
