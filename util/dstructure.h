#ifndef DSTRUCTURE_H
#define DSTRUCTURE_H

#include <limits.h>

#define CONTROL(OPT) (OPT)->control(OPT)
#define CONTROL_N(O) O.control(&O)

#define LIST_ITEM_SIZE(list) sizeof (*(list)->items)

#define DEC_PLIST(T) typedef struct {T **items; size_t length;size_t max_length;} T##PList;
#define DEC_LIST(T) typedef struct {T *items; size_t length;size_t max_length;} T##List;
#define NULL_LIST(list) memset((list)->items,0,LIST_ITEM_SIZE(list) * (list)->max_length );(list)->length=0;
#define RESET_LIST(list) (list)->items=NULL; (list)->length=0; (list)->max_length=0;
#define FREE_LIST(list) free((list)->items); RESET_LIST(list)
#define RESIZE_M_LIST(list, new_length)  if((list)->max_length < new_length){(list)->items = realloc((list)->items, LIST_ITEM_SIZE(list) * new_length );if ((list)->items == NULL) {(list)->max_length=0;(list)->length=0;}else{(list)->max_length=new_length;}}
#define LIST_PUSH(list, min_alloc_length, new_item) if((list)->max_length<(list)->length+1){if(min_alloc_length<1)return 0;(list)->items=realloc((list)->items, LIST_ITEM_SIZE(list) * ((list)->max_length+min_alloc_length));if((list)->items==NULL){(list)->max_length=0;(list)->length=0;return 0;}(list)->max_length+=min_alloc_length;}(list)->items[(list)->length]=*(new_item);(list)->length++;
//call it for clear lists
#define ALLOC_LIST(list, length) if(length>0){(list)->items = calloc(length, LIST_ITEM_SIZE(list));if ((list)->items != NULL)(list)->max_length=length;}



#define LIST_RESET(list) (list)->items=NULL; (list)->length=0; (list)->max_length=0;
#define LIST_FREE(list) free((list)->items); LIST_RESET(list)
#define RESIZE_M_LIST(list, new_length)  if((list)->max_length < new_length){(list)->items = realloc((list)->items, LIST_ITEM_SIZE(list) * new_length );if ((list)->items == NULL) {(list)->max_length=0;(list)->length=0;}else{(list)->max_length=new_length;}}
#define LIST_PUSH(list, min_alloc_length, new_item) if((list)->max_length<(list)->length+1){if(min_alloc_length<1)return 0;(list)->items=realloc((list)->items, LIST_ITEM_SIZE(list) * ((list)->max_length+min_alloc_length));if((list)->items==NULL){(list)->max_length=0;(list)->length=0;return 0;}(list)->max_length+=min_alloc_length;}(list)->items[(list)->length]=*(new_item);(list)->length++;
//call it for empty lists
#define LIST_ALLOC(list, length) if(length>0){(list)->items = calloc(length, LIST_ITEM_SIZE(list));if ((list)->items != NULL)(list)->max_length=length;}

#define LIST_GETBYFIELD(FIELD, DEST,LIST,NEEDLE) (DEST) = NULL;for (size_t I = 0; I < (LIST)->length; I++) {if ((LIST)->items[I].FIELD == (NEEDLE)) {(DEST) = (LIST)->items + I;}}
#define LIST_GETBYID(DEST,LIST,ID) LIST_GETBYFIELD(id, DEST,LIST,ID)
#define LIST_GETBYIDSTR(DEST,LIST,ID) (DEST) = NULL;for (size_t I = 0; I < (LIST)->length; I++) {if (strcmp((LIST)->items[I].id, ID)==0) {(DEST) = (LIST)->items + I;}}


#define DEC_LLIST(T) typedef struct {T *top; T *last; size_t length;} T##LList;
#define LIST_INITIALIZER {.item = NULL, .length = 0, .max_length = 0}
#define LLIST_INITIALIZER {.top = NULL, .last = NULL, .length = 0}
#define LLIST_RESET(list) (list)->top = NULL; (list)->last = NULL; (list)->length = 0;
#define LLIST_GETBYID(DEST,LIST,ID)for((DEST) = (LIST)->top;(DEST)!=NULL;(DEST)=(DEST)->next)if((DEST)->id==ID)break;

#define FOREACH_LLIST(ITEM,LIST,T) for(T * ITEM=(LIST)->top;ITEM!=NULL;ITEM=ITEM->next)
#define FORLLi(T) FOREACH_LLIST(item,list,T)
#define LLIST_DEL_ITEM(ITEM, LIST, T) {T *prev = NULL;FOREACH_LLIST ( curr, LIST, T ) {if ( curr == (ITEM) ) {if ( prev != NULL ) {prev->next = curr->next;} else {(LIST)->top = curr->next;}if ( curr == (LIST)->last ) {(LIST)->last = prev;}(LIST)->length--; break;}prev = curr;}}
#define LLIST_ADD_ITEM(ITEM, LIST) if ((LIST)->length < INT_MAX ) {if ( (LIST)->top == NULL ) {(LIST)->top = ITEM;} else {(LIST)->last->next = ITEM;}(LIST)->last = ITEM;(LIST)->length++;}
#define LLIST_FREE(LIST, T) for (T *_item=(LIST)->top; _item!=NULL; ){T *_temp = _item; _item = _item->next; free(_temp);} LLIST_RESET(LIST)

#define FIFO_LIST_INITIALIZER {.items = NULL, .length = 0, .pop_item = NULL, .push_item = NULL}
#define DEC_FIFO_LIST(T) struct fifo_item_ ## T {T data;int free;struct fifo_item_ ## T *prev;struct fifo_item_ ## T *next;};typedef struct fifo_item_ ## T FIFOItem_ ## T;typedef struct {FIFOItem_ ## T *item;size_t length;FIFOItem_ ## T *push_item;FIFOItem_ ## T *pop_item;Mutex mutex;} FIFOItemList_ ## T;
#define FUN_FIFO_PUSH(T) int T ## _fifo_push(T item, FIFOItemList_ ## T *list) {if (!lockMutex(&list->mutex)) {return 0;}if (list->push_item == NULL) {unlockMutex(&list->mutex);return 0;}list->push_item->data = item;list->push_item->free = 0;if(list->pop_item==NULL){list->pop_item=list->push_item;}if (list->push_item->next->free) {list->push_item = list->push_item->next;} else {list->push_item = NULL;}unlockMutex(&list->mutex);return 1;}
#define FUN_FIFO_POP(T) int T ## _fifo_pop(T * item, FIFOItemList_ ## T *list) {if (!lockMutex(&list->mutex)) {return 0;}if (list->pop_item == NULL) {unlockMutex(&list->mutex);return 0;}*item = list->pop_item->data;list->pop_item->free = 1;if (list->push_item == NULL) {list->push_item = list->pop_item;}if (!list->pop_item->next->free) {list->pop_item = list->pop_item->next;} else {list->pop_item = NULL;}unlockMutex(&list->mutex);return 1;}
#define FREE_FIFO(fifo) free((fifo)->itemss); (fifo)->itemss=NULL; (fifo)->length=0; (fifo)->pop_item = NULL;(fifo)->push_item = NULL;
#define DEC_FUN_FIFO_PUSH(T) extern int T ## _fifo_push(T item, FIFOItemList_ ## T *list);
#define DEC_FUN_FIFO_POP(T) extern int T ## _fifo_pop(T * item, FIFOItemList_ ## T *list);
#define FIFO_INIT(list, count) (list)->items = malloc(LIST_ITEM_SIZE(list) * count);if ((list)->items == NULL) {(list)->length = 0;}(list)->length = count;for (size_t i = 0; i < (list)->length; i++){if (i == ((list)->length-1)) {(list)->items[i].next = &(list)->items[0];} else {(list)->items[i].next = &(list)->items[i + 1];} if (i == 0) {(list)->items[i].prev = &(list)->items[(list)->length-1];} else {(list)->items[i].prev = &(list)->items[i - 1];} (list)->items[i].free = 1; } (list)->pop_item = NULL; (list)->push_item = &(list)->items[0]; if(!initMutex(&(list)->mutex)){FREE_FIFO(list)}

#define FUN_PIPE_POP(T) T pipe_pop(T ## List *list) {return list->items[list->length-1];}
#define FUN_PIPE_PUSH(T) void pipe_push(T ## List *list, T value) {for (int i = list->length - 1; i > 0; i--) {list->items[i] = list->items[i - 1];}list->items[0] = value;}
#define FUN_PIPE_MOVE(T) void pipe_move (T ## List *list) {for ( int i = list->max_length - 1; i > 0; i-- ) {list->items[i] = list->items[i - 1];}   list->items[0] = 0.0;}

//round list. we will push first to free place, if no free place, we will update oldest items
#define DEC_RLIST(T) typedef struct {T *item;size_t next_ind;size_t length;} T ## RList;
#define FUN_RLIST_PUSH(T) void push ## T ## RList(T ## RList *list, T value) {if (list->length<=0){return;}list->items[list->next_ind] = value;if (list->next_ind < list->length - 1) {list->next_ind++;} else {list->next_ind = 0;}}
#define FUN_RLIST_INIT(T) int init ## T ## RList(T ## RList *list, size_t n) {list->length=0;list->items=NULL;if(n<=0){return 1;}list->items = calloc(n, LIST_ITEM_SIZE(list));if (list->items == NULL) {return 0;}list->length=n;return 1;}
#define RESET_RLIST(list) (list)->items=NULL; (list)->length=0;
#define FREE_RLIST(list) free((list)->items); RESET_RLIST(list)

typedef struct {
	void **item; 
	size_t length;
	size_t max_length;
} VoidList;

#endif 

