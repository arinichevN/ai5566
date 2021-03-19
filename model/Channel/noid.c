
static int channel_noidSupportedCommands(int command){
	switch(command){
		case CMD_NOID_GET_FTS: case CMD_NOID_GET_STATE: case CMD_NOID_GET_ERROR:case CMD_NOID_START:case CMD_NOID_STOP:case CMD_NOID_RESET:case CMD_NOID_GET_EXISTS:case CMD_NOID_GET_DEVICE_KIND:
			return 1;
	}
	return 0;
}

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
		case CMD_NOID_GET_ACP_COMMAND_SUPPORTED:
			noidServer_sendSupportedCommand(oid, server, channel_noidSupportedCommands);
			return;
	}
	acpls_reset(server);
}

Noid *channel_buildNoids(Channel *self, Noid *prev){
	prev->next = &self->noid;
	//OBJE_ID_SET_PARAM(OID, 			NEXT,		KIND,					FUNCTION, 							DATA,	ID)
	NOID_SET_PARAM(&self->noid,			NULL,		NOID_KIND_REGULAR,		channel_serveNoidRequestSelf,		self,	&self->id)
	return &self->noid;
}
