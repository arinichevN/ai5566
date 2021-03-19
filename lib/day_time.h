#ifndef DAYTIME_H
#define DAYTIME_H

#define SECONDS_PER_DAY 86400L 
#define SECONDS_FROM_1970_TO_2000 946684800

typedef struct {
	uint16_t y;
	uint8_t m;
	uint8_t d;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
} DateTime;

#endif 
