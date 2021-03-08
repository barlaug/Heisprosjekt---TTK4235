#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "driver/hardware.h"
#include "queue.h"
#include "stateMachine.h"

int main(){    
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    //hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    ///*
    now_state = INIT;

    while(1){

        state_machine_switch_case();
    }
    ///*/

    //testing:
    //init_queue();
    
    //print_queue();
    /*
    printf("%d",queue_exists());
    delete_queue();
    printf("%d",get_position());
    */
    return 0;
}
