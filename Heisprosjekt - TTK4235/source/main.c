#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "driver/hardware.h"
#include "queue.h"
#include "fsm.h"

/**
	@file
	@brief Main file.
*/

int main(){    
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    now_state = INIT;
    while(1){
        fsm_state_machine();
    }
    return 0;
}