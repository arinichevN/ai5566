#ifndef SERIAL1_H
#define SERIAL1_H

enum SerialIdE{
	SERIAL_ID_UNKNOWN,
	SERIAL_ID_0,
	SERIAL_ID_1,
	SERIAL_ID_2,
	SERIAL_ID_3
};

enum SerialRateE{
	SERIAL_RATE_UNKNOWN,
	SERIAL_RATE_300,
	SERIAL_RATE_1200,
	SERIAL_RATE_2400,
	SERIAL_RATE_4800, 
	SERIAL_RATE_9600,
	SERIAL_RATE_14400,
	SERIAL_RATE_19200,
	SERIAL_RATE_28800,
	SERIAL_RATE_38400,
	SERIAL_RATE_57600, 
	SERIAL_RATE_115200
};

enum SerialDPSE{
	SERIAL_DPS_UNKNOWN,
	SERIAL_DPS_5N1,
	SERIAL_DPS_6N1,
	SERIAL_DPS_7N1,
	SERIAL_DPS_8N1,
	SERIAL_DPS_5N2,
	SERIAL_DPS_6N2,
	SERIAL_DPS_7N2,
	SERIAL_DPS_8N2,
	SERIAL_DPS_5E1,
	SERIAL_DPS_6E1,
	SERIAL_DPS_7E1,
	SERIAL_DPS_8E1,
	SERIAL_DPS_5E2,
	SERIAL_DPS_6E2,
	SERIAL_DPS_7E2,
	SERIAL_DPS_8E2,
	SERIAL_DPS_5O1,
	SERIAL_DPS_6O1,
	SERIAL_DPS_7O1,
	SERIAL_DPS_8O1,
	SERIAL_DPS_5O2,
	SERIAL_DPS_6O2,
	SERIAL_DPS_7O2,
	SERIAL_DPS_8O2
};

enum SerialKindE{
	SERIAL_MODE_IDLE,
	SERIAL_MODE_SERVER,
	SERIAL_MODE_CLIENT,
	SERIAL_MODE_DEBUG,
	SERIAL_MODE_SPY
};

extern long int serial_getRate (int v);
extern int serial_getRateKind (long int v);
extern int serial_checkDps(int v);
extern int serial_checkMode(int v);
extern int serial_checkRate(int v);
extern int serial_getArduinoDps(int v);
extern const char *serial_getDpsStr(int v);
extern int serial_checkId(int id);
extern HardwareSerial *serial_getDeviceById(int id);
extern const char *serial_getIdStr(int id);
extern const char *serial_getModeStr(int v);

#endif 
