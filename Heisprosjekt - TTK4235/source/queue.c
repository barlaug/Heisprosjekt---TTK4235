#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/**
	@file
	@brief .c-file that contains all the logic for queue management in the elevator queue.
*/


int queue_matrix[HARDWARE_NUMBER_OF_FLOORS][HARDWARE_NUMBER_OF_BUTTONS];
elevator_position position;


void queue_init_queue(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            queue_matrix[i][j] = 0;
        }
    }
}


int queue_queue_exists(){
    int queue_exists = 0;
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            if (queue_matrix[i][j]){
                queue_exists = 1;
                break;
            }
        }
    }
    return queue_exists;
}


void queue_iterate_and_update_queue(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            if (hardware_read_order(i,j)){
                hardware_command_order_light(i, j, 1);
                queue_matrix[i][j] = 1;
            }
        }
    }
}


int queue_order_over_current_position(elevator_position position){
    for (int i = position; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        if (queue_order_at_floor_number(i)){
            return 1;
        }
    }
    return 0;
}


int queue_order_under_current_position(elevator_position position){
    for (int i = 0; i < position; ++i){
        if (queue_order_at_floor_number(i)){
            return 1;
        }
    }
    return 0;
}


void queue_delete_queue(){
    queue_init_queue();
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
    }
}


int queue_order_at_floor_number(int floor){
    for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
        if (queue_matrix[floor][j] == 1){
            return 1;
        }
    }
    return 0;
}


void queue_remove_order_at_floor_number(int floor){
    for(int i = 0; i < HARDWARE_NUMBER_OF_BUTTONS; ++i){
        queue_matrix[floor][i] = 0;
        hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
    }
}


int queue_button_inside_at_floor(elevator_position position){
    if (queue_matrix[position][HARDWARE_ORDER_INSIDE]){
        return 1;
    }
    return 0;
}


int queue_button_down_at_floor(elevator_position position){
    if (queue_matrix[position][HARDWARE_ORDER_DOWN]){
        return 1;
    }
    return 0;
}


int queue_button_up_at_floor(elevator_position position){
    if (queue_matrix[position][HARDWARE_ORDER_UP]){
        return 1;
    }
    return 0;
}


elevator_position queue_get_position(){
    return position;
}


void queue_set_position(elevator_position pos){
    position = pos;
}