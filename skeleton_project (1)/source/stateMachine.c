#include "stateMachine.h"
#include <stdio.h>
#include <stdlib.h>

elevator_state now_state = INIT;

HardwareMovement last_direction = HARDWARE_MOVEMENT_DOWN; 

HardwareMovement current_direction = HARDWARE_MOVEMENT_DOWN;


void update_elevator_position_at_floor(){ 
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
            if (hardware_read_floor_sensor(i)){
                set_position(i);
            }
        }
}

int is_at_floor(){
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i){
            if (hardware_read_floor_sensor(i)){
                return 1;
            }
        }
    return 0;

}


//oppdaterer position til between-statsene
void update_elevator_position_between_floors(){ //trenger at currentdirection har gyldig verdi
    if(get_position() <= fourth){
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
}


int stop_elevator_at_floor(){ //sjekker hvilket floor vi er på og avgjør om vi skal stoppe eller fortsette å kjøre, returnerer 1 hvis stoppe
        //1 hvis order inni på floor
        if (button_inside_at_floor(get_position())){
            printf("%d", get_position());
            printf("orderfunksjon");
            return 1;
        }

        if(button_down_at_floor(get_position()) && current_direction == HARDWARE_MOVEMENT_DOWN){
            return 1;
        }

        if(button_up_at_floor(get_position()) && current_direction == HARDWARE_MOVEMENT_UP){
            return 1;
        }
        
        //1 hvis på vei ned og det skal noen ned eller hvis på vei opp og det skal noen opp
        if ((current_direction == HARDWARE_MOVEMENT_DOWN && button_down_at_floor(get_position()))
        || (current_direction == HARDWARE_MOVEMENT_UP && button_up_at_floor(get_position()))){
            printf("orderfunksjon 1");
            return 1;
        }
        
        //1 hvis det er tomt for orders under og vi er på vei ned (kan da begynne oppover, men må stoppe følast
        if (order_under_current_position(get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_DOWN){
            printf("orderfunksjon 2");
            return 1;
        }
        //samme for opp
        
        if (order_over_current_position(get_position()) == 0 && current_direction == HARDWARE_MOVEMENT_UP){
            printf("orderfunksjon 3");
            return 1;
        }
        
        //topp og bunn stop
        if (current_direction == HARDWARE_MOVEMENT_UP && get_position() == fourth){
            return 1;
        }

        if (current_direction == HARDWARE_MOVEMENT_DOWN && get_position() == first){
            return 1;
        }
    //else det er ikke noen grunn til å stopp
    return 0;
}

void choose_motor_direction(){
    //hvis allerede i bevegelse (!stop) -> return
    //hvis forrige dir = opp og orders over, fortsett oppovover og sett motordir og current dir til opp (så return)
    //hvis orders under og forrige dir = ned fortsett ned (samme som over)
    if (current_direction != HARDWARE_MOVEMENT_STOP){ //skal kun endre motordir når heis i ro
        return;
    }

    if (last_direction == HARDWARE_MOVEMENT_UP && order_over_current_position(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        last_direction = HARDWARE_MOVEMENT_UP;
        printf("if 1");
        return;

    }

    if ((last_direction == HARDWARE_MOVEMENT_DOWN && get_position() == first) && order_over_current_position(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        last_direction = HARDWARE_MOVEMENT_UP;
        printf("if 2");
        return;
    }

    if (last_direction == HARDWARE_MOVEMENT_DOWN && order_under_current_position(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        last_direction = HARDWARE_MOVEMENT_DOWN;
        printf("if 3");
        return;

    }

    if ((last_direction == HARDWARE_MOVEMENT_UP && get_position() == fourth) && order_under_current_position(get_position())){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        last_direction = HARDWARE_MOVEMENT_DOWN;
        printf("if 4");
        return;
    }

    //ifene under 
    if (order_under_current_position(get_position()) && get_position()<= fourth){ //når mellom etg vil denne uansett være rikitg da pos > 4
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        current_direction = HARDWARE_MOVEMENT_DOWN;
        last_direction = HARDWARE_MOVEMENT_DOWN;
        printf("if 5");
        return;
    }
   
    if(order_over_current_position(get_position()) && get_position()<= fourth){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        last_direction = HARDWARE_MOVEMENT_UP;
        printf("if 6");
        return;
    }
// if ordes above pos - 3 kjør opp ellers kjør ned
    if(order_over_current_position(get_position()-3)){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        current_direction = HARDWARE_MOVEMENT_UP;
        last_direction = HARDWARE_MOVEMENT_UP;
        return;
    }

    hardware_command_movement(HARDWARE_MOVEMENT_DOWN); // hvis ingen av de andre ifene intreffer, går heisen uansett ned 
    current_direction = HARDWARE_MOVEMENT_DOWN;
    last_direction = HARDWARE_MOVEMENT_DOWN;
    printf("if 5");
    return;
}


//statemachine funksjon med switch
void state_machine_switch_case(){

    iterate_and_update_queue();

    switch (now_state)
    {
    case (INIT):
        printf("init");
        printf("\n");
        /*
        int error = hardware_init();
        if(error != 0){
            fprintf(stderr, "Unable to initialize hardware\n");
            exit(1);
        }
        */
        init_queue();
        //starter heis i 1. etg
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        printf("init");
        printf("\n");
        while(!hardware_read_floor_sensor(0)){
            delete_queue();
        }
        if(hardware_read_floor_sensor(0)){
            printf("if");
            set_position(first);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            current_direction = HARDWARE_MOVEMENT_STOP;
            last_direction = HARDWARE_MOVEMENT_DOWN;
            now_state = IDLE;
            break;
        }
        break;
    
    case (IDLE):
        printf("idle");
        printf("\n");
        printf("%d", get_position());
        if (hardware_read_stop_signal()){
            //på etasje:
            if(get_position() > fourth){
                now_state = STOP_BETWEEN_FLOORS;
                break;
            }
            if(get_position() <= fourth){
                now_state = STOP_AT_FLOOR;
                break;
            }
        }
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        hardware_command_door_open(0);
        current_direction = HARDWARE_MOVEMENT_STOP;
        iterate_and_update_queue();
        //bevegelse hvis kø eksisterer:
        if (queue_exists()){
            now_state = MOVING;
            break;
        }
        break;

    case (MOVING):
        printf("%d", get_position());
        printf("%d",order_over_current_position(get_position()));
        print_queue();
        choose_motor_direction();
        printf("moving");
        if(get_position() <= third_fourth){
            update_elevator_position_between_floors();
            //mens heisen er mellom etg, oppdateres det om heisen er i en etasje hele tiden
            while(1){

                if(hardware_read_stop_signal()){ 
                    if(is_at_floor()){
                        now_state = STOP_AT_FLOOR;
                        break;
                    }
                    now_state = STOP_BETWEEN_FLOORS;
                    break;
                }
                if(is_at_floor()){
                    update_elevator_position_at_floor();
                }

                //update_elevator_position_between_floors();      
                update_elevator_position_at_floor();
                printf("MOVING");
                printf("%d", get_position());
                iterate_and_update_queue(); 
                if(is_at_floor() && stop_elevator_at_floor()){ //hvis heisen skal stoppe i etg->open_door state
                    printf("hei");
                    now_state = OPEN_DOOR;
                    break;
                }

            }
            /*

            if((is_at_floor() && get_position() > fourth) && last_direction != HARDWARE_MOVEMENT_STOP){ 
                printf("problem");
                printf("%d", get_position());
                print_queue();
                //update_elevator_position_at_floor();

                if(stop_elevator_at_floor()){
                    printf("hei");
                    now_state = OPEN_DOOR;
                    break;
                }
            }
            */
        }
        break;

    case (OPEN_DOOR):
        hardware_command_floor_indicator_on(get_position());
        if(hardware_read_stop_signal()){
            now_state = STOP_AT_FLOOR;
            break;
        }
        last_direction = current_direction;
        current_direction = HARDWARE_MOVEMENT_STOP;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        if(order_at_floor_number(get_position())){//trengs denne ifen?
            remove_order_at_floor_number(get_position()); //tror 
        }
        printf("open door");
        print_queue();

        start_timer();
        while(!timer_has_expried(3)){

            while(hardware_read_obstruction_signal()){
                printf("obstruksjon");
                start_timer();
                hardware_command_door_open(1);
            }

            hardware_command_door_open(1);
            iterate_and_update_queue(); //må kunne håndtere bestillinger mens i etg
        }
        now_state=IDLE;
        break;

    case (STOP_AT_FLOOR):
        last_direction = HARDWARE_MOVEMENT_STOP;
        update_elevator_position_at_floor(get_position());
        delete_queue();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        while(hardware_read_stop_signal()){
            printf("stop at floor");
            hardware_command_stop_light(1);
        }
        hardware_command_stop_light(0);
        now_state = OPEN_DOOR;
        break;

    case (STOP_BETWEEN_FLOORS):
        last_direction = HARDWARE_MOVEMENT_STOP;
        update_elevator_position_between_floors();
        delete_queue();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        while(hardware_read_stop_signal()){
            printf("stop between floors");
            hardware_command_stop_light(1);
        }
        hardware_command_stop_light(0);
        now_state = IDLE;
        break;

    default:
        //now_state = INIT;
        break;
    }
}