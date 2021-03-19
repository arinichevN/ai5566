/*
 * Use this file if you want to know
 * exection time of code block.
 * How to use:
 * - include this file
 * - begin Serial
 * - loop begin
 * - call stw_start();
 * - place your code
 * - call stw_stop();
 * - loop end
 * stw_stop() function will print to 
 * serial output interface minimum and
 * maximum execution time of your code
 * after STOPWATCH_COUNT repetitions.
 */

#include <limits.h>

#define STOPWATCH_COUNT 10000UL
unsigned long stopwatch_min = ULONG_MAX;
unsigned long stopwatch_max = 0UL;
unsigned long stopwatch_c = 0UL;
unsigned long stopwatch_t1;
unsigned long stopwatch_t2;

void stw_start(){
	stopwatch_t1 = micros();
}

void stw_stop(){
	stopwatch_t2 = micros();
	if(stopwatch_c < STOPWATCH_COUNT){
		unsigned long td = stopwatch_t2 - stopwatch_t1;
		if(td < stopwatch_min) stopwatch_min = td;
		if(td > stopwatch_max) stopwatch_max = td;
		stopwatch_c++;
	}else{
		Serial.print("\nMIN: ");Serial.print(stopwatch_min);Serial.print(", MAX: ");Serial.print(stopwatch_max);Serial.println(" (us)");
		stopwatch_c=0UL;
		stopwatch_min = ULONG_MAX;
		stopwatch_max = 0UL;
	} 
}
