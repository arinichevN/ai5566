#ifndef APP_NOIDS_H
#define APP_NOIDS_H

#include "../../lib/noid.h"
#include "../channels/main.h"
#include "param.h"

extern Noid *noid_top;

extern void noids_setParam(const NoidsParam *param);

extern void noids_begin();

#define FOREACH_APP_NOID FOREACH_NOID(noid_top, oid)

#endif
