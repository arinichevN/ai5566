#ifndef APP_AOIDS_H
#define APP_AOIDS_H

#include "../../lib/aoid.h"
#include "../channels/main.h"
#include "../serials/main.h"

extern Aoid aoid;
extern Aoid aoid_id;
extern Aoid aoid_device_kind;
#ifdef USE_NOIDS
extern Aoid aoid_noid_first;
extern Aoid aoid_noid_extern_next_id;
#endif

extern int aoids_begin();

#define FOREACH_APP_AOID FOREACH_AOID(&aoid, oid)

#endif
