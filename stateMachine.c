#include "stateMachine.h"
#include <stdio.h>
#include <stdlib.h>

elevator_state now_state = INIT;

HardwareMovement last_direction; //(?)

HardwareMovement current_direction;

//1
void update_elevator_position_at_floor(){ //denne fungerer som den skal tror jeg
    if ((position <= 3) && (position >= 0)){
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
            if (hardware_read_floor_sensor(i)){
                set_position(i);
            }
        }
    }
}

//oppdaterer position til between-statsene
void update_elevator_position_between_floors(){ //trenger at currentdirection har gyldig verdi
    switch (current_direction)
    {
    case (HARDWARE_MOVEMENT_DOWN):
        set_position(get_position() + 3); //differanse = 3 i enum elevator_position ved bevegelse nedover
        break;

    case (HARDWARE_MOVEMENT_UP):
        set_position(get_position() + 4); //differanse = 4 i enum elevator_position ved bevegelse oppover
        break;

    case (HARDWARE_MOVEMENT_STOP):
        break;
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

//2
/*
void stop_elevator_at_floor(){ //trengs logikk for gjøre at den ikke stopper på ALLE floors
    if (order_at_floor_number(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}
*/

//forslag:
int stop_elevator_at_floor(){ //sjekker hvilket floor vi er på og avgjør om vi skal stoppe eller fortsette å kjøre, returnerer 1 hvis stoppe
    //1 hvis order på floor
    if (order_at_floor_number(get_position())){
        return 1;
    }
    //1 hvis på vei ned og det skal noen ned eller hvis på vei opp og det skal noen opp
    if ((current_direction == HARDWARE_MOVEMENT_DOWN && button_down_at_floor(get_position()))
     || (current_direction == HARDWARE_MOVEMENT_UP && button_up_at_floor(get_position()))){
        return 1;
    }
    //1 hvis det er tomt for orders under og vi er på vei ned (kan da begynne oppover, men må stoppe først)
    if (order_under_current_position(get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_DOWN){
        return 1;
    }
    //samme for opp
    if (order_over_current_position(get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_UP){
        return 1;
    }
    //else det er ikke noen grunn til å stoppe
    return 0;
}



//3
void choose_motor_direction();

//statemachine funksjon med switch
void state_machine_switch_case();
