
extern ChannelLList channels;

Channel *chplr_last = NULL;
Channel *chplr_first = NULL;

static int chplr_conflictExists(Channel *caller){
	FOREACH_CHANNEL {
		if(channel_portIntersectionExists(channel, caller)){
			if(channel->port_lock.state == CHPL_RUN){
				return 1;
			}
		}
	}
	return 0;
}

static void chplr_pushInQueue(Channel *caller){
	if(chplr_first == NULL){
		caller->port_lock.before = NULL;
		caller->port_lock.after = NULL;
		caller->port_lock.state = CHPL_WAIT;
		chplr_first = caller;
	} else{
		chplr_last->port_lock.after = caller;
		caller->port_lock.before = chplr_last;
		caller->port_lock.after = NULL;
		caller->port_lock.state = CHPL_WAIT;
	}
	chplr_last = caller;
}

static void chplr_removeFromQueue(Channel *caller){
	if(caller->port_lock.before != NULL){
		caller->port_lock.before->port_lock.after = caller->port_lock.after;
	}
	if(caller->port_lock.after != NULL){
		caller->port_lock.after->port_lock.before = caller->port_lock.before;
	}
	if(caller == chplr_last){
		chplr_last = caller->port_lock.before;
	}
	if(caller == chplr_first){
		chplr_first = caller->port_lock.after;
	}
	caller->port_lock.after = NULL;
	caller->port_lock.before = NULL;
	caller->port_lock.state = CHPL_IDLE;
}

static void chplr_tryRunItems(){
	for(Channel *channel = chplr_first; channel != NULL; channel = channel->port_lock.after) {
		if(!chplr_conflictExists(channel)){
			chplr_removeFromQueue(channel);
			channel->port_lock.state = CHPL_RUN;
		}
	}
}

int chplr_lock(Channel *caller){
	switch(caller->port_lock.state){
		case CHPL_IDLE:
			if(chplr_conflictExists(caller)){
				chplr_pushInQueue(caller);
				return 0;
			}else{
				caller->port_lock.state = CHPL_RUN;
				return 1;
			}
			break;
		case CHPL_WAIT:
			return 0;
		case CHPL_RUN:
			return 1;
	}
	printdln("chplr_lock unknown state");
	return 0;
}

void chplr_unlock(Channel *caller){
	switch(caller->port_lock.state){
		case CHPL_IDLE:
			return;
		case CHPL_WAIT:
			chplr_removeFromQueue(caller);
			chplr_tryRunItems();
			return;
		case CHPL_RUN:
			caller->port_lock.state = CHPL_IDLE;
			chplr_tryRunItems();
			return;
	}
}

void chplr_begin(){
	chplr_first = NULL;
	chplr_last = NULL;
}
