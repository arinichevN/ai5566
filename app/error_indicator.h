#ifndef APP_ERROR_INDICATOR_H
#define APP_ERROR_INDICATOR_H

typedef struct apperrorindicator_st{
	int pin;
	void (*control)(struct apperrorindicator_st *, int);
} AppErrorIndicator;

extern void appei_begin(AppErrorIndicator *item, int pin);

#define APPEI_CONTROL(item, error_id) (item)->control(item, error_id);

#endif
