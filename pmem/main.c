#include "main.h"

#define PMEM_APP_SIZE sizeof(AppParam)
#define PMEM_SERIAL_SIZE sizeof(AppSerialParam)
#define PMEM_CRC_SIZE sizeof(uint8_t)
#define PMEM_CHANNEL_SIZE sizeof(ChannelParam)

#define PMEM_APP_ADDR PMEM_CRC_SIZE
#define PMEM_APP_CRC_ADDR 0

#define PMEM_SERIAL_ADDR(ind) 		PMEM_CRC_SIZE + PMEM_APP_SIZE + ind*(PMEM_CRC_SIZE + PMEM_SERIAL_SIZE) + PMEM_CRC_SIZE
#define PMEM_SERIAL_CRC_ADDR(ind)	PMEM_CRC_SIZE + PMEM_APP_SIZE + ind*(PMEM_CRC_SIZE + PMEM_SERIAL_SIZE)

#define PMEM_CHANNEL_ADDR(ind) 			PMEM_CRC_SIZE + PMEM_APP_SIZE + serials.length*(PMEM_CRC_SIZE + PMEM_SERIAL_SIZE) + ind*(PMEM_CRC_SIZE + PMEM_CHANNEL_SIZE) + PMEM_CRC_SIZE
#define PMEM_CHANNEL_CRC_ADDR(ind)		PMEM_CRC_SIZE + PMEM_APP_SIZE + serials.length*(PMEM_CRC_SIZE + PMEM_SERIAL_SIZE) + ind*(PMEM_CRC_SIZE + PMEM_CHANNEL_SIZE)

#define PMEM_CRC_INI 17

extern ChannelLList channels;
extern AppSerialLList serials;

uint8_t pmem_calcCrc(const void *data, size_t sz){
	uint8_t crc = PMEM_CRC_INI;
	uint8_t *pt =(uint8_t *) data;
	for (size_t i = 0; i < sz; ++i) {
		crc_update(&crc, pt[i]);
	}
	return crc;
}

void pmem_setAppCrc(const AppParam *item){
	uint8_t crc = pmem_calcCrc(item, PMEM_APP_SIZE);
	EEPROM.put(PMEM_APP_CRC_ADDR, crc);
}

void pmem_setChannelCrc(const ChannelParam *item, size_t ind){
	uint8_t crc = pmem_calcCrc(item, PMEM_CHANNEL_SIZE);	
	//printd("   channel save crc: ");printdln(crc);
	EEPROM.put(PMEM_CHANNEL_CRC_ADDR(ind), crc);
}

void pmem_setSerialCrc(const AppSerialParam *item, size_t ind){
	uint8_t crc = pmem_calcCrc(item, PMEM_SERIAL_SIZE);	
	//printd("   serial save crc: ");printdln(crc);
	EEPROM.put(PMEM_SERIAL_CRC_ADDR(ind), crc);
}

int pmem_checkSize(){
	size_t total_bytes = PMEM_APP_SIZE + PMEM_CRC_SIZE + serials.length*(PMEM_SERIAL_SIZE + PMEM_CRC_SIZE) + channels.length*(PMEM_CHANNEL_SIZE + PMEM_CRC_SIZE) + 1;
	if(EEPROM.length() <= total_bytes){
		return 0;
	}
	return 1;
}

int pmem_checkAppCrc(const AppParam *item){
	uint8_t crc1 = EEPROM[0];
	uint8_t crc2 = pmem_calcCrc(item, PMEM_APP_SIZE);
	if(crc1 != crc2){
		return 0;
	}
	return 1;
}

int pmem_checkChannelCrc(const ChannelParam *item, int ind){
	uint8_t crc1 = EEPROM[PMEM_CHANNEL_CRC_ADDR(ind)];
	//printd("   channel get crc: ");printdln(crc1);
	uint8_t crc2 = pmem_calcCrc(item, PMEM_CHANNEL_SIZE);
	if(crc1 != crc2){
		printd("   bad crc: ");printd(crc1);printd(" ");printdln(crc2);
		return 0;
	}
	return 1;
}

int pmem_checkSerialCrc(const AppSerialParam *item, int ind){
	uint8_t crc1 = EEPROM[PMEM_SERIAL_CRC_ADDR(ind)];
	//printd("   serial get crc: ");printdln(crc1);
	uint8_t crc2 = pmem_calcCrc(item, PMEM_SERIAL_SIZE);
	if(crc1 != crc2){
		printd("   bad crc: ");printd(crc1);printd(" ");printdln(crc2);
		return 0;
	}
	return 1;
}

int pmem_hasSpaceForChannel(size_t ind){
	size_t required_bytes = PMEM_CHANNEL_ADDR(ind) + sizeof(ChannelParam) + 1;
	if(EEPROM.length() < required_bytes){
		return 0;
	}
	return 1;
}

int pmem_hasSpaceForSerial(size_t ind){
	size_t required_bytes = PMEM_SERIAL_ADDR(ind) + PMEM_SERIAL_SIZE + PMEM_CRC_SIZE;
	if(EEPROM.length() < required_bytes){
		return 0;
	}
	return 1;
}

int pmem_hasSpaceForAppParam(){
	size_t required_bytes = PMEM_APP_ADDR + sizeof(AppParam) + 1;
	if(EEPROM.length() < required_bytes){
		printdln("no space for app_config");
		return 0;
	}
	return 1;
}

int pmem_getSerialParam(AppSerialParam *item, size_t ind) {
	if(!pmem_hasSpaceForSerial(ind)){
		return 0;
	}
	AppSerialParam t_item;
	EEPROM.get(PMEM_SERIAL_ADDR(ind), t_item);
	if(!pmem_checkSerialCrc(&t_item, ind)){
		return 0;
	}
	*item = t_item;
	return 1;
}

int pmem_saveSerialParam(const AppSerialParam *item, size_t ind) {
	//printd("(saving serial: ");printd(ind);
	if(!pmem_hasSpaceForSerial(ind)){
		printd("no space) ");
		return 0;
	}
	EEPROM.put(PMEM_SERIAL_ADDR(ind), *item);
	pmem_setSerialCrc(item, ind);
	printd("done) ");
	return 1;
}

int pmem_getChannelParam(ChannelParam *item, size_t ind) {
	if(!pmem_hasSpaceForChannel(ind)){
		return 0;
	}
	ChannelParam t_item;
	EEPROM.get(PMEM_CHANNEL_ADDR(ind), t_item);
	if(!pmem_checkChannelCrc(&t_item, ind)){
		return 0;
	}
	*item = t_item;
	return 1;
}

int pmem_saveChannelParam(const ChannelParam *item, size_t ind){
	//printd("(saving channel: ");printd(ind);
	if(!pmem_hasSpaceForChannel(ind)){
		printd("no space) ");
		return 0;
	}
	EEPROM.put(PMEM_CHANNEL_ADDR(ind), *item);
	pmem_setChannelCrc(item, ind);
	printd("done) ");
	return 1;
}

int pmem_getAppParam(AppParam *item) {
	if(!pmem_hasSpaceForAppParam()){
		return 0;
	}
	AppParam tout;
	EEPROM.get(PMEM_APP_ADDR, tout);
	if(!pmem_checkAppCrc(&tout)){
		return 0;
	}
	*item = tout;
	return 1;
}

int pmem_saveAppParam(const AppParam *item){
	//printdln("saving device");
	if(!pmem_hasSpaceForAppParam()){
		printdln("pmem: no space for device");
		return 0;
	}
	EEPROM.put(PMEM_APP_ADDR, *item);
	pmem_setAppCrc(item);
	return 1;
}




