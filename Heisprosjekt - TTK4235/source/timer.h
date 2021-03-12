#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
	@file
	@brief .h-file for the timer system.
*/

//starter en timer
void timer_start_timer();

//returnerer 1 når timer har stått i seconds sekunder
int timer_timer_has_expried(double seconds);


#endif //TIMER_H