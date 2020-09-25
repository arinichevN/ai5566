
extern App app;
extern AppSerial serials[];
extern ChannelLList channels;
extern void print_var(HardwareSerial *serial);

#include "../app/main.h"
#include "../acp/command/main.h"

Channel *srvc_getChannel(ACPLS *item){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		FOREACH_CHANNEL(&channels){
			if(id == channel->id){
				return channel;
			}
		}
		printdln("\tchannel not found");
		return NULL;

	}
	printdln("\tfailed to get channel_id");
	return NULL;
}

int srvc_forThisApp(ACPLS *item){
	int id;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &id)){
		if(id == app.id){
			return 1;
		}else{
			printdln("\tnot for this app");
			return 0;
		}
	}
	printdln("\tfailed to get app_id");
	return 0;
}

void srvc_setChannelParamUl(ACPLS *item, void (*func)(PmemChannel *, unsigned long )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	unsigned long v;
	if(acp_packGetCellUl(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		PmemChannel pchannel;
		if(pmem_getPChannelForce(&pchannel, channel->ind)){
			func(&pchannel, v);
			pmem_savePChannel(&pchannel, channel->ind);
		}
	}
	ACPLS_RESET
}

void srvc_setChannelParamI(ACPLS *item, void (*func)(PmemChannel *, int )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		PmemChannel pchannel;
		if(pmem_getPChannel(&pchannel, channel->ind)){
			func(&pchannel, v);
			pmem_savePChannel(&pchannel, channel->ind);
		}
	}
	ACPLS_RESET
}

void srvc_setChannelParamF(ACPLS *item, void (*func)(PmemChannel *, double )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	double v;
	if(acp_packGetCellF(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		PmemChannel pchannel;
		if(pmem_getPChannel(&pchannel, channel->ind)){
			func(&pchannel, v);
			pmem_savePChannel(&pchannel, channel->ind);
		}
	}
	ACPLS_RESET
}



void fgetChannelPmemParamUl(ACPLS *item, unsigned long (*getfunc)(PmemChannel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	PmemChannel pdata;
	int f = 0;unsigned long out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIUlI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void fgetChannelPmemParamI(ACPLS *item, int (*getfunc)(PmemChannel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	PmemChannel pdata;
	int f = 0;int out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}
void fgetChannelPmemParamF(ACPLS *item, double (*getfunc)(PmemChannel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	PmemChannel pdata;
	int f = 0;double out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIFI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void fgetAppPmemParamI(ACPLS *item, int app_id, int (*getfunc)(AppConfig *)){
	AppConfig pdata;
	int f = 0;int out = 0;
	if(pmem_getAppConfig(&pdata)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app_id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}


void srvc_getChannelParamUl(ACPLS *item, unsigned long (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	unsigned long out = getfunc(channel);
	int r = acp_buildPackIUl(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getChannelParamI(ACPLS *item, int (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	int out = getfunc(channel);
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getChannelParamF(ACPLS *item, double (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	double out = getfunc(channel);
	int r = acp_buildPackIF(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getChannelParamStr(ACPLS *item, const char *(*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	const char *out = getfunc(channel);
	int r = acp_buildPackIS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

#define SC_FUN_SET(param) acnf_set__ ## param
#define SC_DEF_FUN_SET_UL(param) void SC_FUN_SET(param) (ACPLS *item, HardwareSerial *serial){srvc_setChannelParamUl(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}
#define SC_DEF_FUN_SET_I(param) void SC_FUN_SET(param) (ACPLS *item, HardwareSerial *serial){srvc_setChannelParamI(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}
#define SC_DEF_FUN_SET_F(param) void SC_FUN_SET(param) (ACPLS *item, HardwareSerial *serial){srvc_setChannelParamF(item, &PMEMCHANNEL_SET_FIELD_FUNC(param));}

#define SC_FUN_GET(param) acnf_get__ ## param
#define SC_DEF_FUN_GET_UL(param) void SC_FUN_GET(param) (ACPLS *item, HardwareSerial *serial){fgetChannelPmemParamUl(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}
#define SC_DEF_FUN_GET_I(param) void SC_FUN_GET(param) (ACPLS *item, HardwareSerial *serial){fgetChannelPmemParamI(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}
#define SC_DEF_FUN_GET_F(param) void SC_FUN_GET(param) (ACPLS *item, HardwareSerial *serial){fgetChannelPmemParamF(item, &PMEMCHANNEL_GET_FIELD_FUNC(param));}

#define SC_FUN_GETR(param) acnf_getr__ ## param
#define SC_DEF_FUN_GETR_UL(param) void SC_FUN_GETR(param) (ACPLS *item, HardwareSerial *serial){srvc_getChannelParamUl(item, &CHANNEL_FUN_GET(param));}
#define SC_DEF_FUN_GETR_I(param) void SC_FUN_GETR(param) (ACPLS *item, HardwareSerial *serial){srvc_getChannelParamI(item, &CHANNEL_FUN_GET(param));}
#define SC_DEF_FUN_GETR_F(param) void SC_FUN_GETR(param)(ACPLS *item, HardwareSerial *serial){srvc_getChannelParamF(item, &CHANNEL_FUN_GET(param));}


SC_DEF_FUN_SET_UL(poll_interval)
SC_DEF_FUN_SET_I(device_kind)

SC_DEF_FUN_GET_UL(poll_interval)
SC_DEF_FUN_GET_I(device_kind)

SC_DEF_FUN_GETR_UL(poll_interval)
SC_DEF_FUN_GETR_I(device_kind)
SC_DEF_FUN_GETR_I(enable)

void acnf_getFTS (ACPLS *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	if(acp_buildPackIFTS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, &channel->out)){ACPLS_PREP_SEND} else {ACPLS_RESET}
}

void acnf_getExists(ACPLS *item, HardwareSerial *serial){
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &v)){
		FOREACH_CHANNEL(&channels){
			if(v == channel->id){
				goto success;
			}
		}
		if(v == app.id)	goto success;
	}
	ACPLS_RESET
	return;
	success:
	if(acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, v, 1)){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_getErrorStr(ACPLS *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getErrorStr);}
void acnf_getStateStr(ACPLS *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getStateStr);}

void acnf_getAppVar(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	print_var(serial);
	ACPLS_RESET
}

void acnf_start(ACPLS *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_start(channel);
	}
	ACPLS_RESET
}

void acnf_stop(ACPLS *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_stop(channel);
	}
	ACPLS_RESET
}

void acnf_reload(ACPLS *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_reload(channel);
	}
	ACPLS_RESET
}

//void srvc_getrAppConfigField(ACPLS *item, int (*getfunc)(AppConfig *)){
	//if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	//int v = getfunc(&config);
	//int r = acp_buildPackI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, v);
	//if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
//}

void srvc_getrAppFieldSF(ACPLS *item, const char * (*getfunc)(App *)){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	extern App app;
	const char *v = getfunc(&app);
	int r = acp_buildPackIS(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, v);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getAppConfigField(ACPLS *item, int (*getfunc)(AppConfig *)){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	AppConfig conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppConfig(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, v, result);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getAppConfigFieldBr(ACPLS *item, int (*getfunc)(AppConfig *)){
	AppConfig conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppConfig(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, v, result);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void srvc_getAppConfigSrField(ACPLS *item, int (*getfunc)(AppSerialConfig *)){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int serial_id;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &serial_id)){ACPLS_RESET return;}
	AppConfig conf;
	if(!pmem_getAppConfig(&conf)){ACPLS_RESET return;}
	int v;
	int result = 0;
	FOREACH_SERIAL(i){
		AppSerialConfig *serial = &conf.serial[i];
		if(serial_id == serial->id){
			v = getfunc(serial);
			result = 1;
			goto done;
		}
	}
	done:
	if(result && acp_buildPackIIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, serial_id, v, result)){
		ACPLS_PREP_SEND
		return;
	}
	ACPLS_RESET
}

void acnf_getAppSerialRate(ACPLS *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(rate));}
void acnf_getAppSerialConfig(ACPLS *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(config));}
void acnf_getAppSerialKind(ACPLS *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(kind));}

void acnf_getAppId(ACPLS *item, HardwareSerial *serial){srvc_getAppConfigFieldBr(item, &APPCONFIG_GET_FIELD_FUNC(id));}
void acnf_getAppState(ACPLS *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getStateStr);}
void acnf_getAppError(ACPLS *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getErrorStr);}

void srvc_setAppConfigField(ACPLS *item, int (*checkfunc)(int ), void (*setfunc)(AppConfig *, int v)){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		AppConfig conf;
		if(pmem_getAppConfig(&conf)){
			if(checkfunc(v)){
				setfunc(&conf, v);
				pmem_saveAppConfig(&conf);
			}
		}
	}
	ACPLS_RESET
}

void srvc_setAppSerialConfigField(ACPLS *item, int (*checkfunc)(int ), void (*setfunc)(AppSerialConfig *, int v)){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int serial_id;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &serial_id)){
		goto done;
	}
	int v;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM2, &v)){
		goto done;
	}
	if(!checkfunc(v)){
		goto done;
	}
	AppConfig conf;
	if(!pmem_getAppConfig(&conf)){
		goto done;
	}
	FOREACH_SERIAL(i){
		if(serial_id == conf.serial[i].id){
			setfunc(&conf.serial[i], v);
			pmem_saveAppConfig(&conf);
			goto done;
		}
	}
	done:
	ACPLS_RESET
}

void acnf_setAppId(ACPLS *item, HardwareSerial *serial){
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_ID, &v)){
		AppConfig conf;
		if(pmem_getAppConfig(&conf)){
			if(appc_checkId(v)){
				app.id = v;
				pmem_saveAppConfig(&conf);
			}
		}
	}
	ACPLS_RESET
}

void acnf_getIdFirst(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int out;
	int success = channels_getIdFirst(&channels, &out);
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, out, success);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_setId(ACPLS *item, HardwareSerial *serial){srvc_setChannelParamI(item, &PMEMCHANNEL_SET_FIELD_FUNC(id));}

void acnf_setIdFirst(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		FOREACH_CHANNEL(&channels){
			PmemChannel pchannel;
			if(pmem_getPChannel(&pchannel, channel->ind)){
				pchannel.id = v; v++;
				pmem_savePChannel(&pchannel, channel->ind);
			}
		}
	}
	ACPLS_RESET
}

void acnf_appReset(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	app_reset(&app);
	ACPLS_RESET
}

void acnf_setAppSerialConfig(ACPLS *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &appc_checkSerialConfig, &APPCONFIGSR_SET_FIELD_FUNC(config));}
void acnf_setAppSerialRate(ACPLS *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &appc_checkSerialRate, &APPCONFIGSR_SET_FIELD_FUNC(rate));}
void acnf_setAppSerialKind(ACPLS *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &serial_checkAppKind, &APPCONFIGSR_SET_FIELD_FUNC(kind));}

ACPLSCommandNode acnodes[] = {
	{CMD_GET_ID_EXISTS,					&acnf_getExists},
	
#ifdef SERIAL_SERVER_RAM_GET_COMMANDS
	{CMD_GETR_CHANNEL_FTS,				&acnf_getFTS},
	{CMD_GETR_CHANNEL_STATE,			&acnf_getStateStr},
	{CMD_GETR_CHANNEL_ERROR,			&acnf_getErrorStr},
#endif

	{CMD_CHANNEL_START,					&acnf_start},
	{CMD_CHANNEL_STOP,					&acnf_stop},
	{CMD_CHANNEL_RELOAD,				&acnf_reload},

#ifdef SERIAL_SERVER_NVRAM_GET_COMMANDS
	{CMD_GET_CHANNEL_DEVICE_KIND,		&SC_FUN_GET(device_kind)},
	{CMD_GET_RSENSOR_INTERVAL,			&SC_FUN_GET(poll_interval)},
#endif

#ifdef SERIAL_SERVER_NVRAM_SET_COMMANDS
	{CMD_SET_CHANNEL_DEVICE_KIND,		&SC_FUN_SET(device_kind)},
	{CMD_SET_RSENSOR_INTERVAL,			&SC_FUN_SET(poll_interval)},
#endif

#ifdef SERIAL_SERVER_RAM_GET_COMMANDS
	{CMD_GETR_CHANNEL_DEVICE_KIND,		&SC_FUN_GET(device_kind)},
	{CMD_GETR_RSENSOR_INTERVAL,			&SC_FUN_GET(poll_interval)},
#endif

#ifdef SERIAL_SERVER_NVRAM_SET_COMMANDS
	{CMD_SET_CHANNEL_ID,				&acnf_setId},
#endif

#ifdef SERIAL_SERVER_RAM_GET_COMMANDS
	{CMD_GET_APP_VARIABLES,				&acnf_getAppVar},
#endif


#ifdef SERIAL_SERVER_NVRAM_SET_COMMANDS
	{CMD_SET_APP_ID,					&acnf_setAppId},
	{CMD_SET_APP_CHANNEL_ID_FIRST,		&acnf_setIdFirst},
#endif

	{CMD_APP_RESET,						&acnf_appReset},
	
#ifdef SERIAL_SERVER_RAM_GET_COMMANDS
	{CMD_GET_APP_CHANNEL_ID_FIRST,		&acnf_getIdFirst},
	{CMD_GET_APP_ID,					&acnf_getAppId},
	{CMD_GET_APP_STATE,					&acnf_getAppState},
	{CMD_GET_APP_ERROR,					&acnf_getAppError},
#endif

#ifdef SERIAL_SERVER_NVRAM_SET_COMMANDS
	{CMD_SET_APP_SERIAL_KIND,			&acnf_setAppSerialKind},
	{CMD_SET_APP_SERIAL_RATE,			&acnf_setAppSerialRate},
	{CMD_SET_APP_SERIAL_CONFIG,			&acnf_setAppSerialConfig},
#endif

#ifdef SERIAL_SERVER_NVRAM_GET_COMMANDS
	{CMD_GET_APP_SERIAL_RATE,			&acnf_getAppSerialRate},
	{CMD_GET_APP_SERIAL_CONFIG,			&acnf_getAppSerialConfig},
	{CMD_GET_APP_SERIAL_KIND,			&acnf_getAppSerialKind}
#endif

};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])


