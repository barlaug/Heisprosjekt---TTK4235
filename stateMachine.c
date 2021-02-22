#include "stateMachine.h"
#include <stdio.h>
#include <stdlib.h>

elevator_state now_state = INIT;

HardwareMovement last_direction;

HardwareMovement current_direction;

//1
void update_elevator_position(){
    

    if ((position <= 3) && (position >= 0)){
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
            if (hardware_read_floor_sensor(i)){
                set_position(i);
            }
        }
    }
    
    /*
    switch (current_direction)
    {
    case (HARDWARE_MOVEMENT_DOWN):
        set_position(get_position() + 3); //differanse = 3 i elevator_position ved bevegelse nedover
        break;

    case (HARDWARE_MOVEMENT_UP):
        set_position(get_position() + 4); //differanse = 4 i elevator_position ved bevegelse oppover
        break;

    case (HARDWARE_MOVEMENT_STOP):
        break;
    }
    */
}

//2
void stop_elevator_at_floor(){
    if (order_at_floor_number(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

//3
void choose_motor_direction();

//statemachine funksjon med switch
void state_machine_switch_case();