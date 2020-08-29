#include "error_indicator.h"

#define APPEI_ERROR_SHOW digitalWrite(item->pin, HIGH);
#define APPEI_ERROR_HIDE digitalWrite(item->pin, LOW);

static void appei_WAIT_FAILURE(AppErrorIndicator *item, int error_id);

static void appei_WAIT_SUCCESS(AppErrorIndicator *item, int error_id){
	if(error_id == ERROR_NO){
		APPEI_ERROR_HIDE
		item->control = appei_WAIT_FAILURE;
	}
}

static void appei_WAIT_FAILURE(AppErrorIndicator *item, int error_id){
	if(error_id != ERROR_NO){
		printd("app error detected: "); printdln(getErrorStr(error_id));
		APPEI_ERROR_SHOW
		item->control = appei_WAIT_SUCCESS;
	}
}

static void appei_INIT(AppErrorIndicator *item, int error_id){
	if(error_id == ERROR_NO){
		APPEI_ERROR_HIDE
		item->control = appei_WAIT_FAILURE;
	}else{
		APPEI_ERROR_SHOW
		item->control = appei_WAIT_SUCCESS;
	}
}

void appei_begin(AppErrorIndicator *item, int pin){
	item->pin = pin;
	pinMode(item->pin, OUTPUT);
	item->control = appei_INIT;
}
