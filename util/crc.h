
#ifndef CRC_H
#define CRC_H

extern void crc_update(uint8_t *crc, uint8_t b) ;

extern void crc_updateByStr(uint8_t *crc, const char *str);

extern void crc_updateByBuf(uint8_t *crc, const char *buf, size_t buf_len);
#endif

