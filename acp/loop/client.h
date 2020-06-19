#ifndef ACPL_CLIENT_H
#define ACPL_CLIENT_H

#include "main.h"

extern int acplc_sendSI(ACPL *item, const char *v1, int v2);
extern int acplc_sendSF(ACPL *item, const char *v1, double v2);
extern int acplc_sendSII(ACPL *item, const char *v1, int v2, int v3);
extern int acplc_sendSIII(ACPL *item, const char *v1, int v2, int v3, int v4);
extern int acplc_sendSIUl(ACPL *item, const char *v1, int v2, unsigned long v3);
extern int acplc_sendSIF(ACPL *item, const char *v1, int v2, double v3);

extern int acplc_getBrII(ACPL *item, const char *cmd, int *v1, int *v2);
	
extern int acplc_getIS(ACPL *item, const char *cmd, int channel_id, char *out, size_t slen);
extern int acplc_getFTS(ACPL *item, const char *cmd, int channel_id, FTS *out);
extern int acplc_getII(ACPL *item, const char *cmd, int channel_id, int *out);
extern int acplc_getIF(ACPL *item, const char *cmd, int channel_id, double *out);
extern int acplc_getIII(ACPL *item, const char *cmd, int channel_id, int id2, int *out);
extern int acplc_getIIII(ACPL *item, const char *cmd, int channel_id, int id2, int *v1, int *v2);
extern int acplc_getIUl(ACPL *item, const char *cmd, int channel_id, unsigned long *out);

extern void acpl_client(ACPL *item);


#endif
