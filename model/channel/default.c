#include "param.h"
#include "../../util/common.h"
/*
 * -user_config:
 * put parameters for each channel in this array
 * enable: YES or NO
 * device_kind: DEVICE_KIND_MAX6675 or DEVICE_KIND_MAX31855
 * {id, enable, device_kind, poll_interval_ms, cs,sclk,miso}
 */
const ChannelParam CHANNEL_DEFAULT_PARAMS[] = {
	{21, YES, DEVICE_KIND_MAX31855, 300, 2,6,5},
	{22, YES, DEVICE_KIND_MAX31855, 300, 3,6,5},
	{23, YES, DEVICE_KIND_MAX31855, 300, 7,6,5}
};

#define CHANNEL_COUNT (sizeof CHANNEL_DEFAULT_PARAMS / sizeof CHANNEL_DEFAULT_PARAMS[0])
