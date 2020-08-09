#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "../acp/loop/main.h"
#include "../acp/loop/server.h"
#include "../util/common.h"
#include "../util/serial.h"
#include "../model/channel.h"
#include "serial.h"
#include "serial_config.h"
#include "config.h"

typedef struct {
	int id;
	int state;
	int error_id;
	int ectl_state;
	int next_state;
} App;


extern int appc_checkId(int v);
extern int appc_checkSerialRate(int v);
extern int appc_checkSerialConfig(int v);

extern void app_init(App *item);
extern void app_reset(App *item);
extern void app_control(App *item, AppSerial serials[], ChannelLList *channels);

extern const char *app_getErrorStr(App *item);
extern const char *app_getStateStr(App *item);

extern App app;

#endif 
