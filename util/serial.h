#ifndef SERIAL1_H
#define SERIAL1_H


enum SerialIdE{
	SERIAL_IDN,
	SERIAL_ID0,
	SERIAL_ID1,
	SERIAL_ID2,
	SERIAL_ID3
};

enum SerialRateE{
	SR_300=1,
	SR_1200,
	SR_2400,
	SR_4800, 
	SR_9600,
	SR_14400,
	SR_19200,
	SR_28800,
	SR_38400,
	SR_57600, 
	SR_115200
};

enum AppSerialKindE{
	APP_SERIAL_KIND_IDLE = 0,
	APP_SERIAL_KIND_SERVER,
	APP_SERIAL_KIND_CLIENT,
	APP_SERIAL_KIND_DEBUG,
	APP_SERIAL_KIND_SPY
};

extern long int serial_getRate (uint8_t v);
extern uint8_t serial_getRateKind (long int v);
extern int serial_checkConfig(uint8_t v);
extern int serial_checkAppKind(int v);
extern const char *serial_getConfigStr(uint8_t v);
extern int serial_checkId(int id);
extern HardwareSerial *serial_getDeviceById(int id);
extern const char *serial_getNameStr(int id);
extern const char *serial_getAppKindStr(int v);

#endif 
