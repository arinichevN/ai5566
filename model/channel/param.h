#ifndef MODEL_CHANNEL_PARAM_H
#define MODEL_CHANNEL_PARAM_H


typedef struct {
	int id;
	int enable;
	int device_kind;
	unsigned long poll_interval;
	int cs;
	int sclk;
	int miso;
} ChannelParam;

#define CHANNEL_DEFAULT_PARAMS chdefps
#endif 
