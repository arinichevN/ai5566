#ifndef PMEM_H
#define PMEM_H

#include <EEPROM.h>
#include "../model/channel/main.h"
#include "../model/channel/param.h"
#include "../app/main.h"
#include "../app.h"
#include "../util/crc.h"

extern int pmem_getPChannel(ChannelParam *item, size_t ind) ;

extern int pmem_getPChannelForce(ChannelParam *item, size_t ind) ;

extern int pmem_getAppConfig(AppConfig *item) ;

extern int pmem_getAppConfigForce(AppConfig *item);

extern int pmem_savePChannel(const ChannelParam *item, size_t ind);

extern int pmem_saveAppConfig(const AppConfig *item);

#define PMEMCHANNEL_GET_FIELD_FUNC(FIELD) pmcgff ## FIELD
#define PMEMCHANNEL_DEF_GET_FIELD_FUNC(FIELD, T) T PMEMCHANNEL_GET_FIELD_FUNC(FIELD)(ChannelParam *item){return item->FIELD;}

#define PMEMCHANNEL_SET_FIELD_FUNC(FIELD) pmcsff ## FIELD
#define PMEMCHANNEL_DEF_SET_FIELD_FUNC(FIELD, T) void PMEMCHANNEL_SET_FIELD_FUNC(FIELD)(ChannelParam *item, T v){item->FIELD = v;}

#endif 
