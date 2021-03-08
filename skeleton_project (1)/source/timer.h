#ifndef TIMER
#define TIMER

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//static time_t timer_counter = 0; //tellevariabelen 

//starter en timer
void start_timer();

//returnerer 1 når timer har stått i seconds sekunder
int timer_has_expried(double seconds);


//resetter timeren
void reset_timer();

 

#endif //TIMER