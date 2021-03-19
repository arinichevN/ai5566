#include "slave_list.h"

void owpsl_begin(OwpollerSlaveList *self){
	self->top = NULL;
	self->last = NULL;
}

static int owpsl_itemExists(OwpollerSlaveList *self, iOwpollerSlave *item){
	for(iOwpollerSlave *i = self->top; i != NULL; i = i->next){
		if(i == item) return 1;
	}
	return 0;
}

int owpsl_addItem(OwpollerSlaveList *self, iOwpollerSlave *item){
	if(item->next != NULL){
		printdln("owpsl: bad item");
		return 0;
	}
	if(owpsl_itemExists(self, item)){
		return 1;
	}
	if(self->top == NULL){
		self->top = item;
	}
	if(self->last != NULL){
		self->last->next = item;
	}
	self->last = item;
	return 1;
}

int owpsl_deleteItem(OwpollerSlaveList *self, iOwpollerSlave *item){
	iOwpollerSlave *prev = NULL;
	for(iOwpollerSlave *i = self->top; i != NULL; i = i->next){
		if(i == item){
			if(i == self->top){
				self->top = i->next;
			}
			if(i == self->last){
				self->last = NULL;
			}
			if(prev != NULL){
				prev->next = i->next;
				i->next = NULL;
			}
			return 1;
		}
		prev = i;
	}
	return 0;
}
