#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "../acp/loop/main.h"
#include "../acp/loop/server.h"
#include "../util/common.h"
#include "../util/serial.h"
#include "../model/channel/main.h"
#include "serial.h"
#include "serial_config.h"
#include "config.h"
#include "error_indicator.h"

typedef struct app_st{
	int id;
	int error_id;
	AppErrorIndicator error_indicator;
	void (*control)(struct app_st *);
	void (*next_control)(struct app_st *);
} App;

extern int appc_checkId(int v);
extern int appc_checkSerialRate(int v);
extern int appc_checkSerialConfig(int v);

extern void app_init(App *item);
extern void app_reset(App *item);

extern const char *app_getErrorStr(App *item);
extern const char *app_getStateStr(App *item);

extern App app;

#endif 
