#ifndef ACPL_CLIENT_H
#define ACPL_CLIENT_H

#include "main.h"

enum ACPLCmodeE {
	ACPLC_MODE_NONE,
	ACPLC_MODE_SEND,
	ACPLC_MODE_SEND_READ
};

typedef struct acplc_st{
	ACPL *acpl;
	int mode;
	void (*control) (struct acplc_st *, HardwareSerial *);
} ACPLC;

extern int acplc_sendII(ACPLC *item, char sign, int v1, int v2);
extern int acplc_sendIF(ACPLC *item, char sign, int v1, double v2);
extern int acplc_sendIII(ACPLC *item, char sign, int v1, int v2, int v3);
extern int acplc_sendIIII(ACPLC *item, char sign, int v1, int v2, int v3, int v4);
extern int acplc_sendIIUl(ACPLC *item, char sign, int v1, int v2, unsigned long v3);
extern int acplc_sendIIF(ACPLC *item, char sign, int v1, int v2, double v3);

extern int acplc_getBrII(ACPLC *item, const char *cmd, int *v1, int *v2);
	
extern int acplc_getIS(ACPLC *item, int cmd, int channel_id, char *out, size_t slen);
extern int acplc_getFTS(ACPLC *item, int cmd, int channel_id, FTS *out);
extern int acplc_getII(ACPLC *item, int cmd, int channel_id, int *out);
extern int acplc_getIF(ACPLC *item, int cmd, int channel_id, double *out);
extern int acplc_getIII(ACPLC *item, int cmd, int channel_id, int id2, int *out);
extern int acplc_getIIII(ACPLC *item, int cmd, int channel_id, int id2, int *v1, int *v2);
extern int acplc_getIUl(ACPLC *item, int cmd, int channel_id, unsigned long *out);

extern int acplc_begin(ACPLC **item);

extern void acplc_free(ACPLC *item);

#define acplc_control(item, serial) (item)->control(item, serial)

#endif
