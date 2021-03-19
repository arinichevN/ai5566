#include "main.h"



static void appSerial_controlIdle(AppSerial *item){
	;
}

static void appSerial_freeIdle(AppSerial *serial){
	;
}

void appSerial_reset(AppSerial *self){
	self->id = SERIAL_ID_UNKNOWN;
	self->rate = SERIAL_RATE_UNKNOWN;
	self->dps = SERIAL_DPS_UNKNOWN;
	self->mode = SERIAL_MODE_IDLE;
	self->ind = 0;
	self->device = NULL;
	self->controller = NULL;
	self->control = appSerial_controlIdle;
	self->free = appSerial_freeIdle;
}

static void appSerial_beginDevice(AppSerialParam *item, HardwareSerial *serial){
	if(item->mode != SERIAL_MODE_IDLE){
		unsigned long rate = serial_getRate(item->rate);
		int dps = serial_getArduinoDps(item->dps);
		serial->begin(rate, dps);
		while(!(*serial)){};
	}
}

#ifdef SERIAL_SERVER

extern AcplsCommandNodeList server_nodes;

static void appSerial_controlServer(AppSerial *item){
	Acpls *controller = (Acpls *) item->controller;
	acpls_control(controller, item->device);
}

static void appSerial_freeServer(AppSerial *serial){
	Acpls *item = (Acpls *) serial->controller;
	acpls_free(item);
}

static int appSerial_beginServer(AppSerial *item){
	Acpls *controller = NULL;
	if(!acpls_begin(&controller, &server_nodes)) {
		printd("server begin failed where id = "); printdln(item->id);
		return 0;
	}
	item->controller = (void *) controller;
	item->control = appSerial_controlServer;
	item->free = appSerial_freeServer;
	return 1;
}
#endif

static void appSerial_controlClient(AppSerial *item){
	Acplcm *controller = (Acplcm *) item->controller;
	acplcm_control(controller, item->device);
}

static void appSerial_controlSpy(AppSerial *item){
	Acply *controller = (Acply *) item->controller;
	acply_control(controller, item->device);
}

static void appSerial_freeSpy(AppSerial *serial){
	Acply *item = (Acply *) serial->controller;
	acply_free(item);
}

static void appSerial_freeClient(AppSerial *serial){
	Acplcm *item = (Acplcm *) serial->controller;
	acplcm_free(item);
}

static int appSerial_beginClient(AppSerial *item){
	Acplcm *controller = NULL;
	if(!acplcm_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlClient;
	item->free = appSerial_freeClient;
	return 1;
}

static int appSerial_beginSpy(AppSerial *item){
	Acply *controller = NULL;
	if(!acply_begin(&controller)) return 0;
	item->controller = (void *) controller;
	item->control = appSerial_controlSpy;
	item->free = appSerial_freeSpy;
	return 1;
}

int appSerial_beginMode(AppSerial *serial, AppSerialParam *param, HardwareSerial **serial_debug){
	printd("serial "); printd(serial->id);printd(": ");
	switch (param->mode){
#ifdef SERIAL_SERVER
		case SERIAL_MODE_SERVER:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginServer(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			serial->rate = param->rate;
			serial->dps = param->dps;
			printdln(": server");
			break;
#endif
		case SERIAL_MODE_CLIENT:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginClient(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			serial->rate = param->rate;
			serial->dps = param->dps;
			printdln(" client");
			break;
		case SERIAL_MODE_SPY:
			appSerial_beginDevice(param, serial->device);
			if(!appSerial_beginSpy(serial)){
				 return ERROR_SERIAL_BEGIN;
			}
			serial->mode = param->mode;
			serial->rate = param->rate;
			serial->dps = param->dps;
			printdln(" spy");
			break;
		case SERIAL_MODE_DEBUG:
			if(*serial_debug == NULL){
				appSerial_beginDevice(param, serial->device);
				*serial_debug = serial->device;
				serial->mode = param->mode;
				serial->rate = param->rate;
				serial->dps = param->dps;
				printdln(" debug");
			}
			break;
		case SERIAL_MODE_IDLE:
			printdln(" idle");
			break;
		default:
			printdln(" unknown");
			break;
	}
	return ERROR_NO;
}

#ifdef USE_AOIDS

void appSerial_serveAoidRequestSelf(void *vself, Aoid *oid, void *vserver, int command){
	AppSerial *self = (AppSerial *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_RESET:
			appSerial_reset(self);
			acpls_reset(server);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedR(oid, server);
			return;
	}
	acpls_reset(server);
}

void appSerial_serveAoidRequestId(void *vself, Aoid *oid, void *vserver, int command){
	AppSerial *self = (AppSerial *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_GET_RAM_VALUE:
			aoidServer_sendII(oid, server, self->id, 1);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedGr(oid, server);
			return;
	}
	acpls_reset(server);
}

#define APPSERIAL_FUN_SERVE_AOID_REQUEST_I_GGS(T) appSerial_serveAoidRequest_##T
#define APPSERIAL_DEF_FUN_SERVE_AOID_REQUEST_I_GGS(T) void APPSERIAL_FUN_SERVE_AOID_REQUEST_I_GGS(T)(void *vself, Aoid *oid, void *vserver, int command){ \
	AppSerial *self = (AppSerial *) vself;\
	Acpls *server = (Acpls *) vserver;\
	switch(command){\
		case CMD_AOID_GET_RAM_VALUE:\
			aoidServer_sendII(oid, server, self->T, 1);\
			return;\
		case CMD_AOID_GET_NVRAM_VALUE:{\
				int success = 0;\
				AppSerialParam param;\
				param.T = 0;\
				if(pmem_getSerialParam(&param, self->ind)){\
					success = 1;\
				}\
				aoidServer_sendII(oid, server, param.T, success);\
			}\
			return;\
		case CMD_AOID_SET_NVRAM_VALUE:{\
				int v;\
				if(acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){\
					AppSerialParam param;\
					if(pmem_getSerialParam(&param, self->ind)){\
						param.T = v;\
						if(appSerialParam_check(&param) == ERROR_NO){\
							pmem_saveSerialParam(&param, self->ind);\
						}\
					}\
				}\
			}\
			acpls_reset(server);\
			return;\
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:\
			aoidServer_sendSupportedGGS(oid, server);\
			return;\
	}\
	acpls_reset(server);\
}

APPSERIAL_DEF_FUN_SERVE_AOID_REQUEST_I_GGS(rate)
APPSERIAL_DEF_FUN_SERVE_AOID_REQUEST_I_GGS(dps)
APPSERIAL_DEF_FUN_SERVE_AOID_REQUEST_I_GGS(mode)

void appSerial_buildAoids(AppSerial *self, Aoid *next_oid, Aoid *parent_oid, size_t *id){
	//AOID_SET_PARAM(OID, 					NEXT,					PARENT,					KIND,								DESCR,						FUNCTION, 										DATA,	ID)
	AOID_SET_PARAM(&self->aoid,				&self->id_aoid,			parent_oid,				AOID_KIND_COMPLEX,					AOID_DESCRIPTION_SERIAL,	appSerial_serveAoidRequestSelf,					self,	*id)
	AOID_SET_PARAM(&self->id_aoid,			&self->rate_aoid,		&self->aoid,			AOID_KIND_SERIAL_ID_PARAM,			AOID_DESCRIPTION_ID,		appSerial_serveAoidRequestId,					self,	*id)
	AOID_SET_PARAM(&self->rate_aoid,		&self->dps_aoid,		&self->aoid,			AOID_KIND_SERIAL_RATE_PARAM,		AOID_DESCRIPTION_RATE,		APPSERIAL_FUN_SERVE_AOID_REQUEST_I_GGS(rate),	self,	*id)
	AOID_SET_PARAM(&self->dps_aoid,			&self->mode_aoid,		&self->aoid,			AOID_KIND_SERIAL_DPS_PARAM,			AOID_DESCRIPTION_DPS,		APPSERIAL_FUN_SERVE_AOID_REQUEST_I_GGS(dps),	self,	*id)
	AOID_SET_PARAM(&self->mode_aoid,		next_oid,				&self->aoid,			AOID_KIND_SERIAL_MODE_PARAM,		AOID_DESCRIPTION_MODE,		APPSERIAL_FUN_SERVE_AOID_REQUEST_I_GGS(mode),	self,	*id)
}

#endif

