#ifndef APP_INCLUDE_H
#define APP_INCLUDE_H

#include "util/debug.h"
#include "util/common.c"
#include "util/crc.c"
#include "util/check.c"
#include "util/ton.c"
#include "util/serial.c"
#include "util/pmem.c"

#include "model/sensor/max6675.c"
#include "model/sensor/max31855.c"
#include "model/sensor/main.c"
#include "model/channel.c"

#include "acp/main.c"
#include "acp/loop/main.c"

#include "app/serial_config.c"
#include "app/config.c"
#include "app/serial.c"
#include "app/main.c"

#ifdef SERIAL_SERVER
#include "app/server_config.c"
#include "acp/loop/server.c"
#endif

#include "util/print_var.c"


#endif 
