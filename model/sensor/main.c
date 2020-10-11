#include "main.h"

void sensor_begin(Sensor *item){
	pinMode(item->cs, OUTPUT);
	pinMode(item->sclk, OUTPUT);
	pinMode(item->miso, INPUT);
	digitalWrite(item->cs, HIGH);
	digitalWrite(item->sclk, LOW);
}

int sensor_read(Sensor *item, double *result) {
	return item->read_func(item->cs, item->sclk, item->miso, result);
}

static int sensor_dummyRead (int cs, int sclk, int miso, double *result){
	return 0;
}

static void sensor_setDeviceKind (Sensor *item, int device_kind){
	switch(device_kind){
		case DEVICE_KIND_MAX6675:
			item->device_kind = device_kind;
			item->read_func = &max6675_read;
			printdln("max6675");
			return;
		case DEVICE_KIND_MAX31855:
			item->device_kind = device_kind;
			item->read_func = &max31855_read;
			printdln("max31855");
			return;
	}
	item->device_kind = DEVICE_KIND_UNKNOWN;
	item->read_func = sensor_dummyRead;
}

int sensor_checkParam(int device_kind, int cs, int sclk, int miso){
	switch(device_kind){
		case DEVICE_KIND_MAX6675:
		case DEVICE_KIND_MAX31855:
			break;
		default:
			printdln("WARNING: unknown device");
			return ERROR_DEVICE_KIND;
	}
    return ERROR_NO;
}

void sensor_setParam(Sensor *item, int device_kind, int cs, int sclk, int miso){
	item->cs = cs;
	item->sclk = sclk;
	item->miso = miso;
	sensor_setDeviceKind(item, device_kind);
}

int sensor_getDeviceKind(Sensor *item){
	return item->device_kind;
}
