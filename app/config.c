#include "config.h"

static int appConfig_check (AppConfig *item){
	int r = ERROR_NO;
	FOREACH_SERIAL(i)
		r = appSerialConfig_check(&item->serial[i]);
		if(r != ERROR_NO) return r;
	}
	return r;
}

#define APP_SET_DEFAULT_SERIAL_CONFIG_SERVER	item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].kind = APP_SERIAL_KIND_SERVER; item->serial[i].id = sid; i++;
#define APP_SET_DEFAULT_SERIAL_CONFIG_DEBUG		item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].kind = APP_SERIAL_KIND_DEBUG; item->serial[i].id = sid; i++;
#define APP_SET_DEFAULT_SERIAL_CONFIG_IDLE		item->serial[i].rate = DEFAULT_APP_SERIAL_RATE; item->serial[i].config = DEFAULT_APP_SERIAL_CONFIG; item->serial[i].kind = APP_SERIAL_KIND_IDLE; item->serial[i].id = sid; i++;

/*
 * -user_config:
 * in this function you can set default configuration for application:
 */
static void appConfig_setDefault(AppConfig *item){
	item->id = DEFAULT_APP_ID;
	size_t i = 0;
	int sid = SERIAL_IDN;
#ifdef USE_SERIAL0
	sid = SERIAL_ID0;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_DEBUG
	
#endif
#ifdef USE_SERIAL1
	sid = SERIAL_ID1;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_SERVER
	
#endif
#ifdef USE_SERIAL2
	sid = SERIAL_ID2;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_SERVER
	
#endif
#ifdef USE_SERIAL3
	sid = SERIAL_ID3;
	
	//-user_config:
	APP_SET_DEFAULT_SERIAL_CONFIG_SERVER
	
#endif
}

int appConfig_begin(AppConfig *item, int btn){
	if(btn == BUTTON_DOWN){
		appConfig_setDefault(item);
		pmem_saveAppConfig(item);
		printd("set default app\n");
	}else{
		int r = pmem_getAppConfig(item);
		if(!r){
			printd("failed to get app\n");
			return ERROR_PMEM_READ;
		}
	}
	return appConfig_check(item);
}

APPCONFIG_DEF_GET_FIELD_FUNC(id)
APPCONFIG_DEF_SET_FIELD_FUNC(id)
