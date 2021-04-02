
DEF_FUN_NOID_SERVER_SEND_SUPPORTED_COMMAND(Chn, VARGS({CMD_NOID_GET_FTS, CMD_NOID_GET_STATE, CMD_NOID_GET_ERROR, CMD_NOID_START, CMD_NOID_STOP, CMD_NOID_RESET, CMD_NOID_GET_EXISTS, CMD_NOID_GET_DEVICE_KIND}))

static void channel_serveNoidRequestSelf(void *vself, Noid *oid, void *vserver, int command){
	Channel *self = (Channel *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_NOID_GET_FTS:
			noidServer_sendFts(oid, server, &self->output);
			return;
		case CMD_NOID_GET_STATE:
			noidServer_sendI(oid, server, (int) channel_getState(self));
			return;
		case CMD_NOID_GET_ERROR:
			noidServer_sendI(oid, server, (int) channel_getError(self));
			return;
		case CMD_NOID_START:
			channel_start(self);
			acpls_reset(server);
			return;
		case CMD_NOID_STOP:
			channel_stop(self);
			acpls_reset(server);
			return;
		case CMD_NOID_RESET:
			channel_reset(self);
			acpls_reset(server);
			return;
		case CMD_NOID_GET_DEVICE_KIND:
			noidServer_sendI(oid, server, self->device_kind);
			return;
		CASE_NOID_GET_ACP_COMMAND_SUPPORTED(Chn)
	}
	acpls_reset(server);
}

Noid *channel_buildNoids(Channel *self, Noid *prev){
	prev->next = &self->noid;
	//OBJE_ID_SET_PARAM(OID, 			NEXT,		KIND,					FUNCTION, 							DATA,	ID)
	NOID_SET_PARAM(&self->noid,			NULL,		NOID_KIND_REGULAR,		channel_serveNoidRequestSelf,		self,	&self->id)
	return &self->noid;
}
