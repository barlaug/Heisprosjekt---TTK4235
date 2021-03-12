#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>

/**
	@file
	@brief .c-file for setting up the elevators finite state machine (fsm)
*/

elevator_state now_state = INIT;

HardwareMovement previous_direction = HARDWARE_MOVEMENT_DOWN; 
HardwareMovement current_direction = HARDWARE_MOVEMENT_DOWN;


void fsm_update_elevator_position_at_floor(){ 
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        if (hardware_read_floor_sensor(i)){
            queue_set_position(i);
        }
    }
}


int fsm_elevator_is_at_floor(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        if (hardware_read_floor_sensor(i)){
            return 1;
        }
    }
    return 0;
}


void fsm_update_elevator_position_between_floors(){ 
    if(queue_get_position() <= fourth){
        switch (current_direction)
        {
        case (HARDWARE_MOVEMENT_DOWN):
            queue_set_position(queue_get_position() + 3); //while moving downwards: difference = 3 in the elevator_position enum
            break;

        case (HARDWARE_MOVEMENT_UP):
            queue_set_position(queue_get_position() + 4); //while moving upwards: difference = 4 in the elevator_position enum
            break;

        case (HARDWARE_MOVEMENT_STOP):
            break;
        }
    }
}


int fsm_stop_elevator_at_floor(){ 
    if (queue_button_inside_at_floor(queue_get_position())){
        return 1;
    }
    if(queue_button_down_at_floor(queue_get_position()) && current_direction == HARDWARE_MOVEMENT_DOWN){
        return 1;
    }
    if(queue_button_up_at_floor(queue_get_position()) && current_direction == HARDWARE_MOVEMENT_UP){
        return 1;
    }
    if ((current_direction == HARDWARE_MOVEMENT_DOWN && queue_button_down_at_floor(queue_get_position()))
    || (current_direction == HARDWARE_MOVEMENT_UP && queue_button_up_at_floor(queue_get_position()))){
        return 1;
    }
    if (queue_order_under_current_position(queue_get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_DOWN){
        return 1;
    }
    if (queue_order_over_current_position(queue_get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_UP){
        return 1;
    }
    if (current_direction == HARDWARE_MOVEMENT_UP && queue_get_position() == fourth){
        return 1;
    }
    if (current_direction == HARDWARE_MOVEMENT_DOWN && queue_get_position() == first){
        return 1;
    }
    return 0;
}


void fsm_choose_motor_direction(){
    if (current_direction != HARDWARE_MOVEMENT_STOP){
        return;
    }
    if (previous_direction == HARDWARE_MOVEMENT_UP && queue_order_over_current_position(queue_get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        previous_direction = HARDWARE_MOVEMENT_UP;
        return;

    }
    if ((previous_direction == HARDWARE_MOVEMENT_DOWN && queue_get_position() == first) && queue_order_over_current_position(queue_get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        previous_direction = HARDWARE_MOVEMENT_UP;
        return;
    }
    if (previous_direction == HARDWARE_MOVEMENT_DOWN && queue_order_under_current_position(queue_get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        previous_direction = HARDWARE_MOVEMENT_DOWN;
        return;

    }
    if ((previous_direction == HARDWARE_MOVEMENT_UP && queue_get_position() == fourth) && queue_order_under_current_position(queue_get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        previous_direction = HARDWARE_MOVEMENT_DOWN;
        return;
    }
    if (queue_order_under_current_position(queue_get_position()) && queue_get_position()<= fourth){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        previous_direction = HARDWARE_MOVEMENT_DOWN;
        return;
    }
    if(queue_order_over_current_position(queue_get_position()) && queue_get_position()<= fourth){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        previous_direction = HARDWARE_MOVEMENT_UP;
        return;
    }
    if(queue_order_over_current_position(queue_get_position()-3)){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        previous_direction = HARDWARE_MOVEMENT_UP;
        return;
    }
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //none of the ifs above are true - we choose to let the elevator go downwards
    current_direction = HARDWARE_MOVEMENT_DOWN;
    previous_direction = HARDWARE_MOVEMENT_DOWN;
    return;
}


void fsm_state_machine(){

    queue_iterate_and_update_queue();

    switch (now_state)
    {

    case (INIT):
        queue_init_queue();

        //the elevator starts at the first floor:
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        while(!hardware_read_floor_sensor(0)){
            queue_delete_queue();
        }
        if(hardware_read_floor_sensor(0)){
            queue_set_position(first);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            current_direction = HARDWARE_MOVEMENT_STOP;
            previous_direction = HARDWARE_MOVEMENT_DOWN;
            now_state = IDLE;
            break;
        }
        break;
    

    case (IDLE):
        if (hardware_read_stop_signal()){
            if(queue_get_position() > fourth){
                now_state = STOP_BETWEEN_FLOORS;
                break;
            }
            if(queue_get_position() <= fourth){
                now_state = STOP_AT_FLOOR;
                break;
            }
        }

        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        hardware_command_door_open(0);
        current_direction = HARDWARE_MOVEMENT_STOP;
        queue_iterate_and_update_queue();
        if (queue_queue_exists()){
            now_state = MOVING;
            break;
        }
        break;


    case (MOVING):
        fsm_choose_motor_direction();

        if(queue_get_position() <= third_fourth){
            fsm_update_elevator_position_between_floors();
            while(1){
                if(hardware_read_stop_signal()){ 
                    if(fsm_elevator_is_at_floor()){
                        now_state = STOP_AT_FLOOR;
                        break;
                    }
                    now_state = STOP_BETWEEN_FLOORS;
                    break;
                }

                fsm_update_elevator_position_at_floor();
                queue_iterate_and_update_queue(); 

                if(fsm_elevator_is_at_floor() && fsm_stop_elevator_at_floor()){ 
                    now_state = OPEN_DOOR;
                    break;
                }
            }
        }
        break;


    case (OPEN_DOOR):
        hardware_command_floor_indicator_on(queue_get_position());

        if(hardware_read_stop_signal()){
            now_state = STOP_AT_FLOOR;
            break;
        }

        previous_direction = current_direction;
        current_direction = HARDWARE_MOVEMENT_STOP;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        queue_remove_order_at_floor_number(queue_get_position());

        timer_start_timer();
        while(!timer_timer_has_expried(3)){
            while(hardware_read_obstruction_signal()){
                timer_start_timer();
                hardware_command_door_open(1);
                queue_iterate_and_update_queue(); //the queue has to be able to handle incoming orders while the obstruction signal is high
            }
            hardware_command_door_open(1);
            queue_iterate_and_update_queue(); //må kunne ta inn bestillinger mens døra er åpen i etg
        }

        queue_remove_order_at_floor_number(queue_get_position());
        now_state=IDLE;
        break;


    case (STOP_AT_FLOOR):
        previous_direction = HARDWARE_MOVEMENT_STOP;
        fsm_update_elevator_position_at_floor(queue_get_position());
        queue_delete_queue();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        while(hardware_read_stop_signal()){
            hardware_command_stop_light(1);
            hardware_command_door_open(1);
        }
        hardware_command_stop_light(0);

        now_state = OPEN_DOOR;
        break;


    case (STOP_BETWEEN_FLOORS):
        previous_direction = HARDWARE_MOVEMENT_STOP;
        fsm_update_elevator_position_between_floors();
        queue_delete_queue();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        while(hardware_read_stop_signal()){
            hardware_command_stop_light(1);
        }
        hardware_command_stop_light(0);

        now_state = IDLE;
        break;


    default:
        break;
    }
}