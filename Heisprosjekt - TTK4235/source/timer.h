#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
	@file
	@brief .h-file for the timer system.
*/

/**
    @brief Starts a timer.
*/
void timer_start_timer();

/**
    @brief Checks for how long the timer_counter variable has been running for.
	@param seconds The time you want to let go by before the function returns True.
    @return 1 if it has gone by more than or equal to seconds seconds since the timer started in timer_start_timer().
	@return 0 if it has gone by less than seconds seconds since the timer started in timer_start_timer().
*/
int timer_timer_has_expried(double seconds);


#endif //TIMER_H