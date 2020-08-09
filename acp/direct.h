#ifndef ACP_DIRECT_H
#define ACP_DIRECT_H

#include "main.h"

/*
 * These functions will take a lot of time, especially when serial rate
 * is low.
 */

extern int acpd_readResponse(HardwareSerial *serial, char *buf, size_t len);

extern int acpd_sendSI(HardwareSerial *serial, char sign, const char *v1, int v2);

extern int acpd_sendSIF(HardwareSerial *serial, char sign, const char *v1, int v2, double v3);

extern int acpd_sendSII(HardwareSerial *serial, char sign, const char *v1, int v2, int v3);

extern int acpd_sendSIUl(HardwareSerial *serial, char sign, const char *v1, int v2, unsigned long v3);

extern int acpd_getFTS(HardwareSerial *serial, const char *cmd, int channel_id, FTS *out);

extern int acpd_getII(HardwareSerial *serial, const char *cmd, int channel_id, int *out);

extern int acpd_getIUl(HardwareSerial *serial, const char *cmd, int channel_id, unsigned long *out);

extern int acpd_getIS(HardwareSerial *serial, const char *cmd, int channel_id, char *out, size_t slen);

#endif
