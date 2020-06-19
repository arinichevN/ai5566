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
