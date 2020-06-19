
extern App app;
extern AppSerial serials[];
extern ChannelLList channels;
extern void print_var(HardwareSerial *serial);

#include "../acp/command/app.h"
#include "../acp/command/channel.h"
#include "../acp/command/common.h"
#include "../acp/command/remote.h"
#include "../app/main.h"

Channel *srvc_getChannel(ACPL *item){
	int v;
	if(acp_packGetCellI(item->buf, 1, &v)){
		FOREACH_CHANNEL(&channels)
			if(channel->id == v){
				return channel;
			}
		}
		printdln("\tchannel not found");
		return NULL;

	}
	printdln("\tfailed to get channel_id");
	return NULL;
}

int srvc_forThisApp(ACPL *item){
	int v;
	if(acp_packGetCellI(item->buf, 1, &v)){
		if(v == app.id){
			return 1;
		}else{
			printdln("\tnot for this app");
			return 0;
		}
	}
	printdln("\tfailed to get app_id");
	return 0;
}

void acnf_getFTS (ACPL *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	if(acp_buildPackIFTS(item->buf, ACP_BUF_MAX_LENGTH, channel->id, &channel->out)){ACPL_PREP_SEND} else {ACPL_RESET}
}

void acnf_getExists(ACPL *item, HardwareSerial *serial){
	int v;
	if(acp_packGetCellI(item->buf, 1, &v)){
		FOREACH_CHANNEL(&channels)
			if(v == channel->id){
				goto success;
			}
		}
		if(v == app.id)	goto success;
	}
	ACPL_RESET
	return;
	success:
	if(acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, v, 1)){ACPL_PREP_SEND}else{ACPL_RESET}
}

void fgetAppPmemParamI(ACPL *item, int app_id, int (*getfunc)(AppConfig *)){
	AppConfig pdata;
	int f = 0;int out = 0;
	if(pmem_getAppConfig(&pdata)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, app_id, out, f);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}


void srvc_getChannelParamUl(ACPL *item, unsigned long (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	unsigned long out = getfunc(channel);
	int r = acp_buildPackIUl(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getChannelParamI(ACPL *item, int (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	int out = getfunc(channel);
	int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getChannelParamF(ACPL *item, double (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	double out = getfunc(channel);
	int r = acp_buildPackIF(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getChannelParamStr(ACPL *item, const char *(*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	const char *out = getfunc(channel);
	int r = acp_buildPackIS(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_setChannelParamI(ACPL *item, void (*func)(PmemChannel *, int )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	int v;
	if(acp_packGetCellI(item->buf, 2, &v)){
		PmemChannel pchannel;
		if(pmem_getPChannel(&pchannel, channel->ind)){
			func(&pchannel, v);
			pmem_savePChannel(&pchannel, channel->ind);
		}
	}
	ACPL_RESET
}

void srvc_setChannelParamUl(ACPL *item, void (*func)(PmemChannel *, unsigned long )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	unsigned long v;
	if(acp_packGetCellUl(item->buf, 2, &v)){
		PmemChannel pchannel;
		if(pmem_getPChannelForce(&pchannel, channel->ind)){
			func(&pchannel, v);
			pmem_savePChannel(&pchannel, channel->ind);
		}
	}
	ACPL_RESET
}

void fgetChannelPmemParamI(ACPL *item, int (*getfunc)(PmemChannel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	PmemChannel pdata;
	int f = 0;int out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out, f);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void fgetChannelPmemParamUl(ACPL *item, unsigned long (*getfunc)(PmemChannel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPL_RESET return;}
	PmemChannel pdata;
	int f = 0;unsigned long out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIUlI(item->buf, ACP_BUF_MAX_LENGTH, channel->id, out, f);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void acnf_getErrorStr(ACPL *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getErrorStr);}
void acnf_getStateStr(ACPL *item, HardwareSerial *serial){srvc_getChannelParamStr(item, &channel_getStateStr);}
void acnf_getrEnable(ACPL *item, HardwareSerial *serial){srvc_getChannelParamI(item, &channel_getEnable);}

void acnf_setDeviceKind(ACPL *item, HardwareSerial *serial){srvc_setChannelParamI(item, &PMEMCHANNEL_SET_FIELD_FUNC(device_kind));}
void acnf_getDeviceKind(ACPL *item, HardwareSerial *serial){fgetChannelPmemParamI(item, &PMEMCHANNEL_GET_FIELD_FUNC(device_kind));}
void acnf_getrDeviceKind(ACPL *item, HardwareSerial *serial){srvc_getChannelParamI(item, &channel_getDeviceKind);}

void acnf_setPollInterval(ACPL *item, HardwareSerial *serial){srvc_setChannelParamUl(item, &PMEMCHANNEL_SET_FIELD_FUNC(poll_interval));}
void acnf_getPollInterval(ACPL *item, HardwareSerial *serial){fgetChannelPmemParamUl(item, &PMEMCHANNEL_GET_FIELD_FUNC(poll_interval));}
void acnf_getrPollInterval(ACPL *item, HardwareSerial *serial){srvc_getChannelParamUl(item, &channel_getPollInterval);}

void acnf_getAppVar(ACPL *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	print_var(serial);
	ACPL_RESET
}

void acnf_start(ACPL *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_start(channel);
	}
	ACPL_RESET
}

void acnf_stop(ACPL *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_stop(channel);
	}
	ACPL_RESET
}

void acnf_reload(ACPL *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_reload(channel);
	}
	ACPL_RESET
}

//void srvc_getrAppConfigField(ACPL *item, int (*getfunc)(AppConfig *)){
	//if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	//int v = getfunc(&config);
	//int r = acp_buildPackI(item->buf, ACP_BUF_MAX_LENGTH, v);
	//if(r){ACPL_PREP_SEND}else{ACPL_RESET}
//}

void srvc_getrAppFieldSF(ACPL *item, const char * (*getfunc)(App *)){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	extern App app;
	const char *v = getfunc(&app);
	int r = acp_buildPackIS(item->buf, ACP_BUF_MAX_LENGTH, app.id, v);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getAppConfigField(ACPL *item, int (*getfunc)(AppConfig *)){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	AppConfig conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppConfig(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, v, result);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getAppConfigFieldBr(ACPL *item, int (*getfunc)(AppConfig *)){
	AppConfig conf;
	int v = 0;
	int result = 0;
	if(pmem_getAppConfig(&conf)){
		v = getfunc(&conf);
		result = 1;
	}
	int r = acp_buildPackII(item->buf, ACP_BUF_MAX_LENGTH, v, result);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void srvc_getAppConfigSrField(ACPL *item, int (*getfunc)(AppSerialConfig *)){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	int serial_id;
	if(!acp_packGetCellI(item->buf, 2, &serial_id)){ACPL_RESET return;}
	AppConfig conf;
	if(!pmem_getAppConfig(&conf)){ACPL_RESET return;}
	int v;
	int result = 0;
	FOREACH_SERIAL(i)
		AppSerialConfig *serial = &conf.serial[i];
		if(serial_id == serial->id){
			v = getfunc(serial);
			result = 1;
			goto done;
		}
	}
	done:
	if(result && acp_buildPackIIII(item->buf, ACP_BUF_MAX_LENGTH, app.id, serial_id, v, result)){
		ACPL_PREP_SEND
		return;
	}
	ACPL_RESET
}

void acnf_getAppSerialRate(ACPL *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(rate));}
void acnf_getAppSerialConfig(ACPL *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(config));}
void acnf_getAppSerialKind(ACPL *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(kind));}

void acnf_getAppId(ACPL *item, HardwareSerial *serial){srvc_getAppConfigFieldBr(item, &APPCONFIG_GET_FIELD_FUNC(id));}
void acnf_getAppState(ACPL *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getStateStr);}
void acnf_getAppError(ACPL *item, HardwareSerial *serial){srvc_getrAppFieldSF(item, &app_getErrorStr);}

void srvc_setAppConfigField(ACPL *item, int (*checkfunc)(int ), void (*setfunc)(AppConfig *, int v)){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	int v;
	if(acp_packGetCellI(item->buf, 2, &v)){
		AppConfig conf;
		if(pmem_getAppConfig(&conf)){
			if(checkfunc(v)){
				setfunc(&conf, v);
				pmem_saveAppConfig(&conf);
			}
		}
	}
	ACPL_RESET
}

void srvc_setAppSerialConfigField(ACPL *item, int (*checkfunc)(int ), void (*setfunc)(AppSerialConfig *, int v)){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	int serial_id;
	if(!acp_packGetCellI(item->buf, 2, &serial_id)){
		goto done;
	}
	int v;
	if(!acp_packGetCellI(item->buf, 3, &v)){
		goto done;
	}
	if(!checkfunc(v)){
		goto done;
	}
	AppConfig conf;
	if(!pmem_getAppConfig(&conf)){
		goto done;
	}
	FOREACH_SERIAL(i)
		if(serial_id == conf.serial[i].id){
			setfunc(&conf.serial[i], v);
			pmem_saveAppConfig(&conf);
			goto done;
		}
	}
	done:
	ACPL_RESET
}

void acnf_setAppId(ACPL *item, HardwareSerial *serial){
	int v;
	if(acp_packGetCellI(item->buf, 1, &v)){
		AppConfig conf;
		if(pmem_getAppConfig(&conf)){
			if(appc_checkId(v)){
				app.id = v;
				pmem_saveAppConfig(&conf);
			}
		}
	}
	ACPL_RESET
}

void acnf_getIdFirst(ACPL *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	int out;
	int success = channels_getIdFirst(&channels, &out);
	int r = acp_buildPackIII(item->buf, ACP_BUF_MAX_LENGTH, app.id, out, success);
	if(r){ACPL_PREP_SEND}else{ACPL_RESET}
}

void acnf_setIdFirst(ACPL *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPL_RESET return;}
	int v;
	if(acp_packGetCellI(item->buf, 2, &v)){
		FOREACH_CHANNEL(&channels)
			PmemChannel pchannel;
			if(pmem_getPChannel(&pchannel, channel->ind)){
				pchannel.id = v; v++;
				pmem_savePChannel(&pchannel, channel->ind);
			}
		}
	}
	ACPL_RESET
}

void acnf_setAppSerialConfig(ACPL *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &appc_checkSerialConfig, &APPCONFIGSR_SET_FIELD_FUNC(config));}
void acnf_setAppSerialRate(ACPL *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &appc_checkSerialRate, &APPCONFIGSR_SET_FIELD_FUNC(rate));}
void acnf_setAppSerialKind(ACPL *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &serial_checkAppKind, &APPCONFIGSR_SET_FIELD_FUNC(kind));}

ACPLCommandNode acnodes[] = {
	{CMD_GET_CHANNEL_FTS,				&acnf_getFTS},
	{CMD_GETR_CHANNEL_STATE,			&acnf_getStateStr},
	{CMD_GETR_CHANNEL_ERROR,			&acnf_getErrorStr},
	{CMD_GET_ID_EXISTS,					&acnf_getExists},
	
	{CMD_CHANNEL_START,					&acnf_start},
	{CMD_CHANNEL_STOP,					&acnf_stop},
	{CMD_CHANNEL_RELOAD,				&acnf_reload},
	
	{CMD_GET_APP_STATE,					&acnf_getAppState},
	{CMD_GET_APP_ERROR,					&acnf_getAppError},

#ifdef SERIAL_INTERFACE_FULL

	{CMD_SET_CHANNEL_DEVICE_KIND,		&acnf_setDeviceKind},
	{CMD_GET_CHANNEL_DEVICE_KIND,		&acnf_getDeviceKind},
	{CMD_GETR_CHANNEL_DEVICE_KIND,		&acnf_getrDeviceKind},
	
	{CMD_SET_RSENSOR_INTERVAL,			&acnf_setPollInterval},
	{CMD_GET_RSENSOR_INTERVAL,			&acnf_getPollInterval},
	{CMD_GETR_RSENSOR_INTERVAL,			&acnf_getrPollInterval},
	
	{CMD_GET_APP_VARIABLES,				&acnf_getAppVar},
	
	{CMD_GETR_CHANNEL_ENABLE,			&acnf_getrEnable},

	{CMD_SET_APP_ID,					&acnf_setAppId},
	{CMD_GET_APP_ID,					&acnf_getAppId},
	
	{CMD_GET_APP_CHANNEL_ID_FIRST,		&acnf_getIdFirst},
	{CMD_SET_APP_CHANNEL_ID_FIRST,		&acnf_setIdFirst},
	
	{CMD_SET_APP_SERIAL_KIND,			&acnf_setAppSerialKind},
	{CMD_SET_APP_SERIAL_RATE,			&acnf_setAppSerialRate},
	{CMD_SET_APP_SERIAL_CONFIG,			&acnf_setAppSerialConfig},

	{CMD_GET_APP_SERIAL_RATE,			&acnf_getAppSerialRate},
	{CMD_GET_APP_SERIAL_CONFIG,			&acnf_getAppSerialConfig},
	{CMD_GET_APP_SERIAL_KIND,			&acnf_getAppSerialKind}
#endif
};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])


