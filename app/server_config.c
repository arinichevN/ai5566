
extern App app;
extern AppSerial serials[];
extern ChannelLList channels;

#include "../app/main.h"
#include "../acp/command/main.h"

ACPLSCommandNode *srvc_getServerCommandNext(int prev_command);
ACPLSCommandNode *srvc_getServerCommand(int command);
int srvc_getServerCommandCount();

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

Channel *srvc_getNextAppChannel(ACPLS *item){
	int id;
	int found = 0;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &id)){
		FOREACH_CHANNEL(&channels){
			if(found) return channel;
			if(id == channel->id){
				found = 1;
				continue;
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

void srvc_setChannelParamUl(ACPLS *item, void (*func)(ChannelParam *, unsigned long )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	unsigned long v;
	if(acp_packGetCellUl(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getPChannelForce(&param, channel->ind)){
			func(&param, v);
			pmem_savePChannel(&param, channel->ind);
		}
	}
	ACPLS_RESET
}

void srvc_setChannelParamI(ACPLS *item, void (*func)(ChannelParam *, int )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	int v;
	if(acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getPChannel(&param, channel->ind)){
			func(&param, v);
			pmem_savePChannel(&param, channel->ind);
		}
	}
	ACPLS_RESET
}

void srvc_setChannelParamF(ACPLS *item, void (*func)(ChannelParam *, double )){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	double v;
	if(acp_packGetCellF(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &v)){
		ChannelParam param;
		if(pmem_getPChannel(&param, channel->ind)){
			func(&param, v);
			pmem_savePChannel(&param, channel->ind);
		}
	}
	ACPLS_RESET
}



void fgetChannelPmemParamUl(ACPLS *item, unsigned long (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	ChannelParam pdata;
	int f = 0;unsigned long out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIUlI(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void fgetChannelPmemParamI(ACPLS *item, int (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	ChannelParam pdata;
	int f = 0;int out = 0;
	if(pmem_getPChannel(&pdata, channel->ind)){
		out = getfunc(&pdata);
		f = 1;
	}
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 channel->id, out, f);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}
void fgetChannelPmemParamF(ACPLS *item, double (*getfunc)(ChannelParam *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	ChannelParam pdata;
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

void srvc_getConfirmChannelParamI(ACPLS *item, int (*getfunc)(Channel *)){
	Channel *channel = srvc_getChannel(item);
	if(channel == NULL) {ACPLS_RESET return;}
	int out = getfunc(channel);
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, channel->id, out, 1);
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

#define SC_FUN_GETRC(param) acnf_getrc__ ## param
#define SC_DEF_FUN_GETRC_I(param) void SC_FUN_GETRC(param) (ACPLS *item, HardwareSerial *serial){srvc_getConfirmChannelParamI(item, &CHANNEL_FUN_GET(param));}


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

void acnf_reset(ACPLS *item, HardwareSerial *serial){
	Channel *channel = srvc_getChannel(item);
	if(channel!=NULL){
		channel_reset(channel);
	}
	ACPLS_RESET
}

void acnf_getAppChannelCount (ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	if(acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, CHANNEL_COUNT)){
		ACPLS_PREP_SEND
	}else{
		ACPLS_RESET
	}
}

void acnf_getChannelIdFirst(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int out;
	int success = channels_getIdFirst(&channels, &out);
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, out, success);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_getChannelIdNext (ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	Channel *channel = srvc_getNextAppChannel(item);
	int success = 0;
	int id = 0;
	if(channel != NULL) {
		success = 1;
		id = channel->id;
	}
	if(acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, id, success)){
		ACPLS_PREP_SEND
	}else{
		ACPLS_RESET
	}
}

void acnf_getAppAcpCommandExists(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int command;
	int success = 0;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &command)){
		goto done;
	}
	{ACPLSCommandNode *node = srvc_getServerCommand(command);
	if(node != NULL) {
		success = 1;
	}}
	done:
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, command, success);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_getAppServerCommandCount(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int c = srvc_getServerCommandCount();
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app.id, c);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_getAppServerCommandFirst(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	extern ACPLSCommandNode acnodes[];
	int first_command = acnodes[0].command;
	int r = acp_buildPackII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE,	 app.id, first_command);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
}

void acnf_getAppServerCommandNext(ACPLS *item, HardwareSerial *serial){
	if(!srvc_forThisApp(item)) {ACPLS_RESET return;}
	int prev_command;
	int next_command = 0;
	int success = 0;
	if(!acp_packGetCellI(item->acpl->buf, ACP_REQUEST_IND_PARAM1, &prev_command)){
		goto done;
	}
	{ACPLSCommandNode *node = srvc_getServerCommandNext(prev_command);
	if(node != NULL){
		success = 1;
		next_command = node->command;
	}}
	done:
	int r = acp_buildPackIII(item->acpl->buf, ACP_BUF_MAX_LENGTH, ACP_SIGN_RESPONSE, app.id, next_command, success);
	if(r){ACPLS_PREP_SEND}else{ACPLS_RESET}
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
void acnf_getAppSerialMode(ACPLS *item, HardwareSerial *serial){srvc_getAppConfigSrField(item, &APPCONFIGSR_GET_FIELD_FUNC(mode));}

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
			ChannelParam param;
			if(pmem_getPChannel(&param, channel->ind)){
				param.id = v; v++;
				pmem_savePChannel(&param, channel->ind);
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
void acnf_setAppSerialMode(ACPLS *item, HardwareSerial *serial){srvc_setAppSerialConfigField(item, &serial_checkMode, &APPCONFIGSR_SET_FIELD_FUNC(mode));}

#define SS_GETR		SERIAL_SERVER_RAM_GET_COMMANDS
#define SS_GET		SERIAL_SERVER_NVRAM_GET_COMMANDS
#define SS_SET		SERIAL_SERVER_NVRAM_SET_COMMANDS

ACPLSCommandNode acnodes[] = {
	{CMD_GETR_CHANNEL_FTS,						&acnf_getFTS},
	
#ifdef SS_GETR
	{CMD_GETR_CHANNEL_STATE,					&acnf_getStateStr},
	{CMD_GETR_CHANNEL_ERROR,					&acnf_getErrorStr},
#endif

	{CMD_GET_ID_EXISTS,							&acnf_getExists},

	{CMD_GET_APP_CHANNEL_COUNT,					&acnf_getAppChannelCount},
	{CMD_GET_APP_ACP_COMMAND_EXISTS,			&acnf_getAppAcpCommandExists},
	{CMD_GET_APP_CHANNEL_ID_FIRST,				&acnf_getChannelIdFirst},
	{CMD_GET_APP_CHANNEL_ID_NEXT,				&acnf_getChannelIdNext},
	
	{CMD_CHANNEL_START,							&acnf_start},
	{CMD_CHANNEL_STOP,							&acnf_stop},
	{CMD_CHANNEL_RESET,						&acnf_reset},

#ifdef SS_GET
	{CMD_GET_CHANNEL_DEVICE_KIND,				&SC_FUN_GET(device_kind)},
	{CMD_GET_RSENSOR_INTERVAL,					&SC_FUN_GET(poll_interval)},
#endif

#ifdef SS_SET
	{CMD_SET_CHANNEL_DEVICE_KIND,				&SC_FUN_SET(device_kind)},
	{CMD_SET_RSENSOR_INTERVAL,					&SC_FUN_SET(poll_interval)},
#endif

#ifdef SS_GETR
	{CMD_GETR_CHANNEL_DEVICE_KIND,				&SC_FUN_GETR(device_kind)},
	{CMD_GETR_RSENSOR_INTERVAL,					&SC_FUN_GETR(poll_interval)},
#endif

#ifdef SS_SET
	{CMD_SET_CHANNEL_ID,						&acnf_setId},
#endif

#ifdef SS_SET
	{CMD_SET_APP_ID,							&acnf_setAppId},
	{CMD_SET_APP_CHANNEL_ID_FIRST,				&acnf_setIdFirst},
#endif

	{CMD_APP_RESET,								&acnf_appReset},
	
#ifdef SS_GETR
	{CMD_GET_APP_CHANNEL_ID_FIRST,				&acnf_getIdFirst},
	{CMD_GET_APP_ID,							&acnf_getAppId},
	{CMD_GET_APP_STATE,							&acnf_getAppState},
	{CMD_GET_APP_ERROR,							&acnf_getAppError},
	{CMD_GET_APP_SERVER_COMMAND_NEXT,			&acnf_getAppServerCommandNext},
	{CMD_GET_APP_SERVER_COMMAND_COUNT,			&acnf_getAppServerCommandCount},
	{CMD_GET_APP_SERVER_COMMAND_FIRST,			&acnf_getAppServerCommandFirst},
#endif

#ifdef SS_SET
	{CMD_SET_APP_SERIAL_MODE,					&acnf_setAppSerialMode},
	{CMD_SET_APP_SERIAL_RATE,					&acnf_setAppSerialRate},
	{CMD_SET_APP_SERIAL_CONFIG,					&acnf_setAppSerialConfig},
#endif

#ifdef SS_GET
	{CMD_GET_APP_SERIAL_RATE,					&acnf_getAppSerialRate},
	{CMD_GET_APP_SERIAL_CONFIG,					&acnf_getAppSerialConfig},
	{CMD_GET_APP_SERIAL_MODE,					&acnf_getAppSerialMode}
#endif

};

#define ACPL_CNODE_COUNT (sizeof acnodes / sizeof acnodes[0])

int srvc_getServerCommandCount(){
	return ACPL_CNODE_COUNT;
}

ACPLSCommandNode *srvc_getServerCommandNext(int prev_command){
	int found = 0;
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		if(found) return &acnodes[i];
		if(acnodes[i].command == prev_command){
			found = 1;
			continue; 
		}
	}
	return NULL;
}

ACPLSCommandNode *srvc_getServerCommand(int command){
	for(size_t i = 0; i<ACPL_CNODE_COUNT; i++){
		if(acnodes[i].command == command){
			return &acnodes[i];
		}
	}
	return NULL;
}
