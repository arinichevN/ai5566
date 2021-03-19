#ifndef APP_SERVER_CONFIG_H
#define APP_SERVER_CONFIG_H

#include "../../main.h"
#include "../../../lib/acp/loop/server/main.h"
#ifdef USE_AOIDS
#include "../../aoids/main.h"
#endif
#ifdef USE_NOIDS
#include "../../noids/main.h"
#endif

extern AcplsCommandNodeList server_nodes;


#endif
