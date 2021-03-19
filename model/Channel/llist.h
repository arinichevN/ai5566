#ifndef MODEL_CHANNEL_LLIST_H
#define MODEL_CHANNEL_LLIST_H

#ifdef USE_AOIDS
#include "../../lib/aoid.h"
#include "../../app/serials/server/aoid.h"
#endif
#include "main.h"

typedef struct {
	Channel *top; 
	Channel *last; 
	size_t length;
#ifdef USE_AOIDS
	Aoid aoid;
#endif
} ChannelLList;

extern int channelLList_activeExists(ChannelLList *self);

extern int channelLList_getIdFirst(ChannelLList *self, int *out);

#ifdef USE_AOIDS
void channelLList_buildAoids(ChannelLList *self, Aoid *next_oid, Aoid *parent_oid, size_t *id);
#endif

#ifdef USE_NOIDS
extern Noid *channelLList_buildNoids(ChannelLList *self, Noid *prev);
#endif

#endif 
