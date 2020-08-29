#include "app.h"
#include "app_include.h"

App app;
ChannelLList channels = LLIST_INITIALIZER;
AppSerial serials[SERIAL_COUNT];

void setup(){
	app_init(&app);
}

//#include "util/stopwatch.c"

void loop() {
	//stw_start();
	CONTROL_N(app);
	//stw_stop();
}
