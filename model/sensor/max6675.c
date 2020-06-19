#include "max6675.h"

int max6675_read(int cs, int sclk, int miso, double *result) {
	uint16_t v=0;
    digitalWrite(cs, LOW);
    delay(1);
	for (int i = 15; i >= 0; i--) {
		digitalWrite(sclk, HIGH);
		delay(1);
		if (digitalRead(miso)) {
			v |= (1 << i);
		}
		digitalWrite(sclk, LOW);
		delay(1);
	}
    digitalWrite(cs, HIGH);
    if (v & 0x4) {
        return ERROR_TERMOCOUPLE_OPEN;
    }
    v >>= 3;
    *result = v * 0.25;
    return ERROR_NO;
}
