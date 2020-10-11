#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "serial.h"
#include "serial_config.h"

/*
 * -user_config:
 * here you can set some default parameters for
 * application
 */
#define DEFAULT_APP_ID				20
#define DEFAULT_APP_SERIAL_RATE		SR_9600
#define DEFAULT_APP_SERIAL_CONFIG	SERIAL_8N1

typedef struct {
	int id;
	AppSerialConfig serial[SERIAL_COUNT];
} AppConfig;

#define APPCONFIG_GET_FIELD_FUNC(FIELD) appcgff ## FIELD
#define APPCONFIG_DEF_GET_FIELD_FUNC(FIELD) int APPCONFIG_GET_FIELD_FUNC(FIELD)(AppConfig *item){return item->FIELD;}
#define APPCONFIG_DEC_GET_FIELD_FUNC(FIELD) extern int APPCONFIG_GET_FIELD_FUNC(FIELD)(AppConfig *item);

#define APPCONFIG_SET_FIELD_FUNC(FIELD) appcsff ## FIELD
#define APPCONFIG_DEF_SET_FIELD_FUNC(FIELD) void APPCONFIG_SET_FIELD_FUNC(FIELD)(AppConfig *item, int v){item->FIELD = v;}
#define APPCONFIG_DEC_SET_FIELD_FUNC(FIELD) extern void APPCONFIG_SET_FIELD_FUNC(FIELD)(AppConfig *item, int v);

APPCONFIG_DEC_GET_FIELD_FUNC(id)
APPCONFIG_DEC_SET_FIELD_FUNC(id)

extern int appConfig_begin(AppConfig *item, int btn);

#endif
