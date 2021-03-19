#ifndef CALLER_QUEUE_H
#define CALLER_QUEUE_H

#include "../app.h"

#define CQ_MIN_ALLOC_LENGTH 3

enum CallerInqueueE {
	CINQ_FIRST,
	CINQ_EXISTS,
	CINQ_REJECT
};

/*
 * FIFO queue
 */

struct caller_queue_item {
	void *caller;
	int free;
	int touch;//each caller should touch its place in queue, in case of miss, caller will be removed from queue
	struct caller_queue_item *prev;
	struct caller_queue_item *next;
};
typedef struct caller_queue_item CallerQueueItem;

typedef struct {
	CallerQueueItem *item;
	size_t length;
	struct caller_queue_item *in_item;
	struct caller_queue_item *out_item;
} CallerQueue;

extern CallerQueue *cq_new();
extern int cq_begin(CallerQueue **queue);
extern void cq_free(CallerQueue *queue);
extern int cq_out(CallerQueue *queue);
extern int cq_in(CallerQueue *queue, void *caller);
extern void cq_resetTouch(CallerQueue *queue);
extern void cq_delMissingCallers(CallerQueue *queue);
extern int cq_control(CallerQueue *queue, void *caller);

#endif
