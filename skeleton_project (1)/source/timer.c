#include "timer.h"
#include <time.h>

static time_t timer_counter;

//starter en timer
void start_timer(){
    timer_counter = time(NULL);
}

//returnerer 1 når timer har stått i seconds sekunder
int timer_has_expried(double seconds){
    time_t current_time = time(NULL);
    if(difftime(current_time, timer_counter) < seconds){
        return 0;
    }
    return 1;
}


//resetter timeren
void reset_timer(){
    timer_counter = 0; //vet ikk eom jeg trenger denne, tror den nullstiller seg i start_timer??
}