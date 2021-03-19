#ifndef APP_INCLUDE_H
#define APP_INCLUDE_H

#include "lib/debug.h"
#include "lib/common.c"
#include "lib/crc.c"
#include "lib/check.c"
#include "lib/ton.c"
#include "lib/serial.c"
#include "lib/caller_queue.c"
#include "lib/stopwatch.c"

#include "lib/acp/main.c"
#include "lib/acp/loop/main.c"
#include "lib/acp/loop/client/main.c"
#include "lib/acp/loop/client/multi.c"
#include "lib/acp/loop/spy/main.c"
#ifdef SERIAL_SERVER
#include "lib/acp/loop/server/main.c"
#endif

#include "pmem/main.c"

#include "model/MAX6675/main.c"
#include "model/MAX31855/main.c"
#include "model/Channel/param.c"
#include "model/Channel/main.c"
#include "model/Channel/llist.c"

#ifdef SERIAL_SERVER
#include "app/serials/server/config.c"
#endif
#ifdef USE_AOIDS
#include "app/aoids/main.c"
#include "app/serials/server/aoid.c"
#endif
#ifdef USE_NOIDS
#include "app/noids/param.c"
#include "app/noids/main.c"
#include "app/serials/server/noid.c"
#endif
#include "app/AppErrorIndicator/main.c"
#include "app/AppSerial/param.c"
#include "app/AppSerial/main.c"
#include "app/AppSerial/llist.c"
#include "app/serials/main.c"
#include "app/channels/main.c"
#include "app/channels/locker.c"
#include "app/param.c"
#include "app/main.c"

#endif 
