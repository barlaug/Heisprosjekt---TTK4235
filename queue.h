#ifndef QUEUE_H
#define QUEUE_H
#include "driver/hardware.h"
#include "driver/channels.h"

//enum til heisposisjon
typedef enum {
    first = 0,
    second = 1,
    third = 2,
    fourth = 3,
    first_second = 4,
    second_third = 5,
    third_fourth = 6
} elevator_position;

//matrise med orders på tilhørende plass
static int queue_matrix[HARDWARE_NUMBER_OF_FLOORS][HARDWARE_NUMBER_OF_BUTTONS];

static elevator_position position = first; //må være static for å beholde verdien sin når utenfor scopet

void init_queue();

void print_queue(); //for testing

//1 hvis elementer i kø, 0 hvis ikke
int queue_exists();

//oppdaterer kø på korresponderende knapp hvis trykket inn
void iterate_and_update_queue();

//1 hvis elementer i kø over, 0 hvis ikke 
int order_over_current_position(elevator_position position);

//1 hvis elementer i kø under, 0 hvis ikke
int order_under_current_position(elevator_position position);

//Slett hele køen
void delete_queue();

//true hvis bestilling på input-etg
int order_at_floor_number(int floor);

//fjerner bestilling på input-etg
void remove_order_at_floor_number(int floor);

//returnerer buttoncommand hvis i gyldig etasje
int button_command_at_floor(elevator_position position);

//returnerer button_call_down hvis i gyldig etasje
int button_down_at_floor(elevator_position position);

//returnerer button_call_up hvis i gyldig etasje
int button_up_at_floor(elevator_position position);

elevator_position get_position();

void set_position(elevator_position pos);

#endif