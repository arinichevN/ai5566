#include "main.h"

/*
 * -user_config:
 * put default parameters for each channel in this array
 * ChannelParam structure members:
 * id,
 * device_kind,
 * poll_interval,
 * cs (should be unique),
 * sclk,
 * miso;
 */
const ChannelParam CHANNEL_DEFAULT_PARAMS[] = {
	{21, DEVICE_KIND_MAX31855, 550UL, 9,11,5},
	{22, DEVICE_KIND_MAX31855, 570UL, 6,11,5},
	{23, DEVICE_KIND_MAX31855, 590UL, 3,11,5}
};

#define CHANNEL_COUNT (sizeof CHANNEL_DEFAULT_PARAMS / sizeof CHANNEL_DEFAULT_PARAMS[0])

Channel channel_buf[CHANNEL_COUNT];

ChannelLList channels = LLIST_INITIALIZER;

int channels_activeExists(){
	return channelLList_activeExists(&channels);
}

int channels_getIdFirst(int *out){
	return channelLList_getIdFirst(&channels, out);
}

void channels_begin(int default_btn){
	LLIST_BUILD_FROM_ARRAY_N(channels, channel_buf, CHANNEL_COUNT)
	size_t ind = 0;
	FOREACH_CHANNEL {
		if(default_btn == BUTTON_DOWN){
			pmem_saveChannelParam(&CHANNEL_DEFAULT_PARAMS[ind], ind);
			printd("default param has been saved for channel ind \n"); printdln(ind);
		}
		channel_begin(channel, ind); ind++;
	}
}

#ifdef USE_AOIDS
void channels_buildAoids(Aoid *next_oid, Aoid *parent_oid, size_t *id){
	return channelLList_buildAoids(&channels, next_oid, parent_oid, id);
}
#endif

#ifdef USE_NOIDS
Noid *channels_buildNoids(Noid *prev){
	return channelLList_buildNoids(&channels, prev);
}
#endif
