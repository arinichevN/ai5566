#ifndef ACPL_CLIENT_H
#define ACPL_CLIENT_H

#include "../main.h"

enum AcplcModeE {
	ACPLC_MODE_NONE,
	ACPLC_MODE_SEND,
	ACPLC_MODE_SEND_READ
};

typedef struct acplc_st{
	Acpl *acpl;
	int mode;
	void (*control) (struct acplc_st *, HardwareSerial *);
} Acplc;

extern int acplc_sendII(Acplc *self, char sign, int v1, int v2);
extern int acplc_sendIF(Acplc *self, char sign, int v1, double v2);
extern int acplc_sendIII(Acplc *self, char sign, int v1, int v2, int v3);
extern int acplc_sendIIII(Acplc *self, char sign, int v1, int v2, int v3, int v4);
extern int acplc_sendIIUl(Acplc *self, char sign, int v1, int v2, unsigned long v3);
extern int acplc_sendIIF(Acplc *self, char sign, int v1, int v2, double v3);

extern int acplc_getBrII(Acplc *self, const char *cmd, int *v1, int *v2);
	
extern int acplc_getIS(Acplc *self, int cmd, int channel_id, char *out, size_t slen);
extern int acplc_getFts(Acplc *self, int cmd, int channel_id, Fts *out);
extern int acplc_getII(Acplc *self, int cmd, int channel_id, int *out);
extern int acplc_getIF(Acplc *self, int cmd, int channel_id, double *out);
extern int acplc_getIII(Acplc *self, int cmd, int channel_id, int id2, int *out);
extern int acplc_getIIII(Acplc *self, int cmd, int channel_id, int id2, int *v1, int *v2);
extern int acplc_getIUl(Acplc *self, int cmd, int channel_id, unsigned long *out);

extern int acplc_begin(Acplc **self);

extern void acplc_free(Acplc *self);

#define acplc_control(self, serial) (self)->control(self, serial)

#endif
