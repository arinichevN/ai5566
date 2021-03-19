#ifndef PMEM_H
#define PMEM_H

#include <EEPROM.h>
#include "../lib/crc.h"
#include "../model/Channel/param.h"
#include "../model/Channel/llist.h"
#include "../app/AppSerial/param.h"
#include "../app/AppSerial/main.h"
#include "../app/param.h"
#include "../app.h"


extern int pmem_checkSize();

extern int pmem_getAppParam(AppParam *item) ;
extern int pmem_saveAppParam(const AppParam *item);

extern int pmem_getSerialParam(AppSerialParam *item, size_t ind) ;
extern int pmem_saveSerialParam(const AppSerialParam *item, size_t ind);

extern int pmem_getChannelParam(ChannelParam *item, size_t ind) ;
extern int pmem_saveChannelParam(const ChannelParam *item, size_t ind);

#endif 
