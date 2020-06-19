#include "max31855.h"

int max31855_read(int cs, int sclk, int miso, double *result) {
    uint32_t v = 0UL;
    digitalWrite(cs, LOW);
    delay(1);
    for (int i = 31; i >= 0; i--) {
        digitalWrite(sclk, HIGH);
        delay(1);
        if (digitalRead(miso)) {
            v |= ((uint32_t)1 << i);
        }
        digitalWrite(sclk, LOW);
        delay(1);
    }
    digitalWrite(cs, HIGH);
    if ( v & 0x20000 ) {
        printd("warning: bit 18 should be 0 where CS pin is "); printdln(cs);
        return WARNING_BAD_DATA;
    }
    if ( v & 0x8 ) {
        printd( "warning: bit 4 should be 0 where CS pin is "); printdln(cs);
        return WARNING_BAD_DATA;
    }
    if (v & 0x4) {
		printd( "error: short to VCC where CS pin is "); printdln(cs);
		return ERROR_TERMOCOUPLE_SC_VCC;
	}
    if (v & 0x2) {
		printd( "warning: short to GND where CS pin is "); printdln(cs);
		return ERROR_TERMOCOUPLE_SC_GND;
	}
    if (v & 0x1) {
		printd( "error: open where CS pin is "); printdln(cs);
		return ERROR_TERMOCOUPLE_OPEN;
	}
    if (v & 0x8000) {
		printd( "error: error where CS pin is "); printdln(cs);
		return ERROR_SOME;
	}
    if (v & 0x80000000) {// -
        v = ~v;
        v = (v >> 18);
        *result = (v *0.25+0.25) *-1;
    } else {// +
        v >>= 18;
        *result = v *0.25;
    }
    return ERROR_NO;
}
