#include "llist.h"

int channelLList_activeExists(ChannelLList *self){
	FOREACH_LLIST(channel, self, Channel){
		if(channel->control != channel_OFF){
			return 1;
		}
	}
	return 0;
}

int channelLList_getIdFirst(ChannelLList *self, int *out){
	int success = 0;
	int f = 0;
	int v;
	FOREACH_LLIST(channel, self, Channel){
		if(!f) { v=channel->id; f=1; success=1;}
		if(channel->id < v) v = channel->id;
	}
	*out = v;
	return success;
}

#ifdef USE_AOIDS
static void channelLList_serveAoidRequestSelf(void *vself, Aoid *oid, void *vserver, int command){
	//ChannelLList *self = (ChannelLList *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		CASE_AOID_GET_ACP_COMMAND_SUPPORTED(None)
	}
	acpls_reset(server);
}

void channelLList_buildAoids(ChannelLList *self, Aoid *next_oid, Aoid *parent_oid, size_t *id){
	Aoid *next_oid_l = next_oid;
	if(self->top != NULL){
		next_oid_l = &self->top->aoid.main;
	}
	//AOID_SET_PARAM(OID, 			NEXT,			PARENT,			KIND,				DESCR,							FUNCTION, 							DATA,	ID)
	AOID_SET_PARAM(&self->aoid,		next_oid_l,		parent_oid,		AOID_KIND_LIST,		AOID_DESCRIPTION_CHANNELS,		channelLList_serveAoidRequestSelf,	self,	*id)	
	FOREACH_LLIST(item, self, Channel){
		next_oid_l = next_oid;
		if(item->next != NULL){
			next_oid_l = &item->next->aoid.main;
		}
		channel_buildAoids(item, next_oid_l, &self->aoid, id);
	}
}
#endif

#ifdef USE_NOIDS
Noid *channelLList_buildNoids(ChannelLList *self, Noid *prev){
	Noid *last_oid = NULL;
	Noid *prev_oid = prev;
	FOREACH_LLIST(item, self, Channel){
		last_oid = channel_buildNoids(item, prev_oid);
		prev_oid = last_oid;
	}
	return last_oid;
}
#endif
