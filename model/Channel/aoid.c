#include "aoid.h"

static void channel_serveAoidRequestSelf(void *vself, Aoid *oid, void *vserver, int command){
	Channel *self = (Channel *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_START:
			channel_start(self);
			acpls_reset(server);
			return;
		case CMD_AOID_STOP:
			channel_stop(self);
			acpls_reset(server);
			return;
		case CMD_AOID_RESET:
			channel_reset(self);
			acpls_reset(server);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedSSR(oid, server);
			return;
	}
	acpls_reset(server);
}

//static void channel_serveRequestNone(void *vself, Aoid *oid, void *vserver, int command) {
	//Acpls *server = (Acpls *) vserver;
	//switch(command){
		//case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			//aoidServer_sendSupportedNone(oid, server);
			//return;
	//}
	//acpls_reset(server);
//}

static void channel_serveAoidRequestGGS(void *vself, Aoid *oid, void *vserver, int command, void (*sendRamParam)(Aoid *, Acpls *, Channel *), void (*sendNvramParam)(Aoid *, Acpls *, const ChannelParam *, yn_t), int (*setNvramParam)(Acpls *, ChannelParam *)){
	Channel *self = (Channel *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_GET_RAM_VALUE:
			sendRamParam(oid, server, self);
			return;
		case CMD_AOID_GET_NVRAM_VALUE:{
				yn_t success = NO;
				ChannelParam param;
				memset(&param, 0, sizeof param);
				if(pmem_getChannelParam(&param, self->ind)){
					success = YES;
				}
				sendNvramParam(oid, server, &param, success);
			}
			return;
		case CMD_AOID_SET_NVRAM_VALUE:{
				ChannelParam param;
				if(!pmem_getChannelParam(&param, self->ind)){
					acpls_reset(server);
					return;
				}
				if(!setNvramParam(server, &param)){
					acpls_reset(server);
					return;
				}
				if(channelParam_check(&param) == ERROR_NO){
					pmem_saveChannelParam(&param, self->ind);
				}
			}
			acpls_reset(server);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedGGS(oid, server);
			return;
	}
	acpls_reset(server);
}

static void channel_sendRamParamId(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->id, YES);}
static void channel_sendNvramParamId(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->id, success);}
static int channel_setNvramParamId(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){	return 0;	}	param->id = v;	return 1;}
static void channel_serveAoidRequestId(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamId, channel_sendNvramParamId, channel_setNvramParamId);}

static void channel_sendRamParamDeviceKind(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->device_kind, YES);}
static void channel_sendNvramParamDeviceKind(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->device_kind, success);}
static int channel_setNvramParamDeviceKind(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->device_kind = (dk_t) v; return 1;}
static void channel_serveAoidRequestDeviceKind(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDeviceKind, channel_sendNvramParamDeviceKind, channel_setNvramParamDeviceKind);}

static void channel_sendRamParamPollInterval(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendUlI(oid, server, channel->tmr.interval, YES);}
static void channel_sendNvramParamPollInterval(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendUlI(oid, server, param->poll_interval, success);}
static int channel_setNvramParamPollInterval(Acpls *server, ChannelParam *param){unsigned long v; if(!acp_packGetCellUl(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->poll_interval = v; return 1;}
static void channel_serveAoidRequestPollInterval(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamPollInterval, channel_sendNvramParamPollInterval, channel_setNvramParamPollInterval);}

static void channel_sendRamParamPinCs(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->cs, YES);}
static void channel_sendNvramParamPinCs(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->cs, success);}
static int channel_setNvramParamPinCs(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->cs = v; return 1;}
static void channel_serveAoidRequestPinCs(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamPinCs, channel_sendNvramParamPinCs, channel_setNvramParamPinCs);}

static void channel_sendRamParamPinSclk(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->sclk, YES);}
static void channel_sendNvramParamPinSclk(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->sclk, success);}
static int channel_setNvramParamPinSclk(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->sclk = v; return 1;}
static void channel_serveAoidRequestPinSclk(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamPinSclk, channel_sendNvramParamPinSclk, channel_setNvramParamPinSclk);}

static void channel_sendRamParamPinMiso(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->miso, YES);}
static void channel_sendNvramParamPinMiso(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->miso, success);}
static int channel_setNvramParamPinMiso(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->miso = v; return 1;}
static void channel_serveAoidRequestPinMiso(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamPinMiso, channel_sendNvramParamPinMiso, channel_setNvramParamPinMiso);}


void channelAoid_build(ChannelAoid *self, Aoid *next, Aoid *parent, void *vchannel, size_t *id){
	//OBJE_ID_SET_PARAM(OID, 					NEXT,						PARENT,			KIND,							DESCR,									FUNCTION, 								DATA,		ID)
	AOID_SET_PARAM(&self->main,					&self->id,					parent,			AOID_KIND_COMPLEX,				AOID_DESCRIPTION_CHANNEL,				channel_serveAoidRequestSelf,			vchannel,	*id)
	AOID_SET_PARAM(&self->id,					&self->device_kind,			&self->main,	AOID_KIND_ID_PARAM,				AOID_DESCRIPTION_ID,					channel_serveAoidRequestId,				vchannel,	*id)
	AOID_SET_PARAM(&self->device_kind,			&self->poll_interval,		&self->main,	AOID_KIND_DEVICE_KIND_PARAM,	AOID_DESCRIPTION_DEVICE_KIND,			channel_serveAoidRequestDeviceKind,		vchannel,	*id)
	AOID_SET_PARAM(&self->poll_interval,		&self->cs,					&self->main,	AOID_KIND_TIMEMS_PARAM,			AOID_DESCRIPTION_POLL_INTERVAL_MS,		channel_serveAoidRequestPollInterval,	vchannel,	*id)
	AOID_SET_PARAM(&self->cs,					&self->sclk,				&self->main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN_CS,				channel_serveAoidRequestPinCs,			vchannel,	*id)
	AOID_SET_PARAM(&self->sclk,					&self->miso,				&self->main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN_SCLK,				channel_serveAoidRequestPinSclk,		vchannel,	*id)
	AOID_SET_PARAM(&self->miso,					next,						&self->main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN_MISO,				channel_serveAoidRequestPinMiso,		vchannel,	*id)
}
