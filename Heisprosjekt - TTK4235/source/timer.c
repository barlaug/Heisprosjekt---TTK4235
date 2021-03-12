#include "timer.h"
#include <time.h>

/**
	@file
	@brief .c-file for the timer system.
*/

/**
    @brief Counter variable for the timer system.
 */
static time_t timer_counter;


void timer_start_timer(){
    timer_counter = time(NULL);
}

int timer_timer_has_expried(double seconds){
    time_t current_time = time(NULL);
    if(difftime(current_time, timer_counter) < seconds){
        return 0;
    }
    return 1;
}