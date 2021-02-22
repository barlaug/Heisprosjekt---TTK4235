//#include "channels.h"
//#include "hardware.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int queue_matrix[HARDWARE_NUMBER_OF_FLOORS][HARDWARE_NUMBER_OF_BUTTONS];
elevator_position position = first;

void init_queue(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            queue_matrix[i][j] = 0;
        }
    }
}

void print_queue(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        printf("\n");
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            printf("%d",queue_matrix[i][j]);
            printf("    ");
        }
    }
    printf("\n");
}

int queue_exists(){
    int result = 0;
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            if (queue_matrix[i][j]){
                result = 1;
                break;
            }
        }
    }
    return result;
}

//oppdaterer kø på korresponderende knapp hvis trykket inn, setter også lys
void iterate_and_update_queue(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            //queue_matrix[i][j] = hardware_read_order(i,j);

            if (hardware_read_order(i,j)){
                hardware_command_order_light(i, j, 1);
                queue_matrix[i][j] = 1;
            }
        }
    }
}


int order_over_current_position(elevator_position position){
    for (int i = position; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        if (order_at_floor_number(i)){
            return 1;
        }
    }
    return 0;
}

int order_under_current_position(elevator_position position){
    for (int i = 0; i < position; ++i){
        if (order_at_floor_number(i)){
            return 1;
        }
    }
    return 0;
}

void delete_queue(){
    init_queue();
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
        hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
    }
}

int order_at_floor_number(int floor){
        for (int j = 0; j < HARDWARE_NUMBER_OF_BUTTONS; ++j){
            if (queue_matrix[floor][j]){
                return 1;
            }
        }
    return 0;
}

void remove_order_at_floor_number(int floor){
    for(int i = 0; i < HARDWARE_NUMBER_OF_BUTTONS; ++i){
        queue_matrix[floor][i] = 0;
    }
}

//returnerer buttoncommand hvis i gyldig etasje
int button_command_at_floor(elevator_position position);

//returnerer button_call_down hvis i gyldig etasje
int button_down_at_floor(elevator_position position);

//returnerer button_call_up hvis i gyldig etasje
int button_up_at_floor(elevator_position position);

elevator_position get_position(){
    return position;
}

void set_position(elevator_position pos){
    position = pos;
}
