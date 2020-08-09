#ifndef ACPL_CLIENT_H
#define ACPL_CLIENT_H

#include "main.h"

extern int acplc_sendII(ACPL *item, char sign, int v1, int v2);
extern int acplc_sendIF(ACPL *item, char sign, int v1, double v2);
extern int acplc_sendIII(ACPL *item, char sign, int v1, int v2, int v3);
extern int acplc_sendIIII(ACPL *item, char sign, int v1, int v2, int v3, int v4);
extern int acplc_sendIIUl(ACPL *item, char sign, int v1, int v2, unsigned long v3);
extern int acplc_sendIIF(ACPL *item, char sign, int v1, int v2, double v3);

extern int acplc_getBrII(ACPL *item, const char *cmd, int *v1, int *v2);
	
extern int acplc_getIS(ACPL *item, int cmd, int channel_id, char *out, size_t slen);
extern int acplc_getFTS(ACPL *item, int cmd, int channel_id, FTS *out);
extern int acplc_getII(ACPL *item, int cmd, int channel_id, int *out);
extern int acplc_getIF(ACPL *item, int cmd, int channel_id, double *out);
extern int acplc_getIII(ACPL *item, int cmd, int channel_id, int id2, int *out);
extern int acplc_getIIII(ACPL *item, int cmd, int channel_id, int id2, int *v1, int *v2);
extern int acplc_getIUl(ACPL *item, int cmd, int channel_id, unsigned long *out);

extern void acpl_client(ACPL *item, HardwareSerial *serial);


#endif
