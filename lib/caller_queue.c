#include "caller_queue.h"

CallerQueue *cq_new(){
	size_t sz = sizeof (CallerQueue);
	printd("cq_new: "); printd(sz); printdln(" bytes allocated");
	CallerQueue *out = (CallerQueue *) malloc(sz);
	if(out == NULL){ printdln("cq_new: failed");}
	return out;
}

int cq_extend(CallerQueue *queue) ;

int cq_begin(CallerQueue **queue){
	CallerQueue *tqueue = cq_new();
	if(tqueue == NULL) return 0;
	tqueue->item = NULL;
	tqueue->length = 0;
	tqueue->out_item = NULL; 
	tqueue->in_item = NULL; 
	cq_extend(tqueue);
	*queue = tqueue;
	return 1;
}

void cq_free(CallerQueue *queue){
	if(queue == NULL) return;
	free(queue);
}

void cq_resetNewItems(CallerQueue *queue) {
	if(queue->length < CQ_MIN_ALLOC_LENGTH){
		return;
	}
	size_t len = queue->length;
	size_t start = queue->length - CQ_MIN_ALLOC_LENGTH;
	for (size_t i = start; i < len; i++){
		queue->item[i].free = 1; 
		queue->item[i].touch = 0; 
		queue->item[i].caller = NULL; 
	}
	if(queue->in_item == NULL){
		queue->in_item = &queue->item[start]; 
	}
}

ptrdiff_t cq_transformPPPointer (void *ptr, void *obase, void *nbase){
	if(ptr != NULL){
		ptrdiff_t dif = (ptrdiff_t)obase - (ptrdiff_t) ptr;
		ptrdiff_t out = (ptrdiff_t)nbase + (ptrdiff_t)dif;
		return out;
	}
	return (ptrdiff_t)ptr;
}

void cq_bindItems(CallerQueue *queue){
	size_t len = queue->length;
	for (size_t i = 0; i < len - 1; i++){
		queue->item[i].next = &queue->item[i + 1]; 
	}
	for (size_t i = 1; i < len; i++){
		queue->item[i].prev = &queue->item[i - 1]; 
	}
	queue->item[0].prev = &queue->item[len - 1];
	queue->item[len - 1].next = &queue->item[0];
}

int cq_extend(CallerQueue *queue) {
	printd("extending: ");
	size_t isz = sizeof *queue->item;
	CallerQueueItem *nitem = (CallerQueueItem *) realloc((void *) queue->item, isz * queue->length + isz * CQ_MIN_ALLOC_LENGTH);
	if(nitem == NULL){
		printd("failed.");
		return 0;
	}
	queue->length += CQ_MIN_ALLOC_LENGTH;
	if(queue->item != nitem){
		queue->in_item = (struct caller_queue_item *)cq_transformPPPointer((void *)queue->in_item, (void *) queue->item, (void *) nitem);
		queue->out_item = (struct caller_queue_item *)cq_transformPPPointer((void *)queue->out_item, (void *) queue->item, (void *) nitem);
		queue->item = nitem;
	}
	cq_bindItems(queue);
	cq_resetNewItems(queue);
	printd("len: ");printd(queue->length);printd(", ");
	printd("success.");
	return 1;
}

int cq_itemExists(CallerQueue *queue, void *caller){
//	printd("(cq item exists: ");
	for (size_t i = 0; i < queue->length; i++){
		if(queue->item[i].free == 0 && queue->item[i].caller == caller){
			queue->item[i].touch = 1;
	//		printd("yes) ");
			return 1;
		}
	}
//	printd("no) ");
	return 0;
}

struct caller_queue_item * cq_getNextInItem(CallerQueue *queue, struct caller_queue_item *current_item){
	struct caller_queue_item *item = current_item->next;
	for(size_t i = 0; i<queue->length; i++){
		if(item->free){
			return item;
		}
		item = item->next;
	}
//	printd("(cq_getNextInItem: no in candidate) ");
	return NULL;
}

struct caller_queue_item * cq_getNextOutItem(CallerQueue *queue, struct caller_queue_item *current_item){
	struct caller_queue_item *item = current_item->next;
	for(size_t i = 0; i<queue->length; i++){
		if(!item->free){
			return item;
		}
		item = item->next;
	}
//	printd("(cq_getNextOutItem: no out candidate) ");
	return NULL;
}

int cq_in(CallerQueue *queue, void *caller) {
//	printd("(cq in: ");
	if (queue->in_item == NULL) {
	//	printdln(" no free space) ");
		return 0;
	}
	queue->in_item->caller = caller;
	queue->in_item->free = 0;
	queue->in_item->touch = 1;
	if(queue->out_item == NULL){
		queue->out_item = queue->in_item;
	}
	queue->in_item = cq_getNextInItem(queue, queue->in_item);
	if (queue->in_item == NULL) {
		cq_extend(queue);
	}
	//printd("success) ");
	return 1;
}

int cq_out(CallerQueue *queue) {
//	printd("(cq out: ");
	if (queue->out_item == NULL) {
	//	printd("failed) ");
		return 0;
	}
	queue->out_item->free = 1;
	if (queue->in_item == NULL) {
		queue->in_item = queue->out_item;
	}
	queue->out_item = cq_getNextOutItem(queue, queue->out_item);
//	printd("success) ");
	return 1;
}

int cq_control(CallerQueue *queue, void *caller){
//	printd("cq_control:");
	if(cq_itemExists(queue, caller)){
		if(queue->out_item != NULL){
			if(queue->out_item->caller == caller){
		//		printd("caller first in queue. ");
				return CINQ_FIRST;
			 }
		}
		
	//	int v = (int ) caller; printd(" caller exists in queue: ");printd(v);
		return CINQ_EXISTS;
	}
	int is_first = 0;
	if(queue->out_item == NULL){
		is_first = 1;
	}
	if(cq_in(queue, caller)){
		if(is_first){
	//		printd("caller first in queue. ");
			return CINQ_FIRST;
		}else{
		//	printd("caller exists in queue (just added). ");
			return CINQ_EXISTS;
		}
	}
	printd("caller rejected. ");
	return CINQ_REJECT;
}

void cq_delMissingCallers(CallerQueue *queue){
	for(size_t i = 0; i<queue->length; i++){
		struct caller_queue_item *item = &queue->item[i];
		if(!item->free && !item->touch){
			item->free = 1;
			if(queue->out_item == item){
				queue->out_item = cq_getNextOutItem(queue, queue->in_item);
			}
			if(queue->in_item == NULL){
				queue->in_item = item;
			}
		}
	}
}

void cq_resetTouch(CallerQueue *queue){
	for(size_t i = 0; i<queue->length; i++){
		struct caller_queue_item *item = &queue->item[i];
		item->touch = 0;
	}
}
