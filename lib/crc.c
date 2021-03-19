#include "crc.h"

void crc_update(uint8_t *crc, uint8_t b) {
    int i;
    for (i = 8; i; i--) {
         *crc = ((*crc ^ b) & 1) ? (*crc >> 1) ^ 0x8c : (*crc >> 1);
        b >>= 1;
    }
}

void crc_updateByStr(uint8_t *crc, const char *str) {
    size_t sz = strlen(str);
    for (size_t i = 0; i < sz; i++) {
        crc_update(crc, (uint8_t) str[i]);
    }
}

void crc_updateByBuf(uint8_t *crc, const char *buf, size_t buf_len) {
    for (size_t i = 0; i < buf_len; i++) {
        crc_update(crc, (uint8_t) buf[i]);
    }
}
