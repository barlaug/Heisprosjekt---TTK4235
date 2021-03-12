#ifndef QUEUE_H
#define QUEUE_H
#include "driver/hardware.h"
#include "driver/channels.h"
#include "fsm.h"

/**
	@file
	@brief .h-file for handling orders and managing the elevator queue.
*/

/**
    @brief Enum containing the different positions of the elevator.
 */
typedef enum {
    first = 0,
    second = 1,
    third = 2,
    fourth = 3,
    first_second = 4,
    second_third = 5,
    third_fourth = 6
} elevator_position;

/**
    @brief 4x3 matrix containing all current orders in the system.
 */
extern int queue_matrix[HARDWARE_NUMBER_OF_FLOORS][HARDWARE_NUMBER_OF_BUTTONS];

/**
    @brief Variable for storing the posistion of the elevator.
 */
extern elevator_position position; 

void queue_init_queue();

//1 hvis elementer i kø, 0 hvis ikke
int queue_queue_exists();

//oppdaterer kø på korresponderende knapp hvis trykket inn, setter lys
void queue_iterate_and_update_queue();

//1 hvis elementer i kø over, 0 hvis ikke 
int queue_order_over_current_position(elevator_position position);

//1 hvis elementer i kø under, 0 hvis ikke
int queue_order_under_current_position(elevator_position position);

//Slett hele køen
void queue_delete_queue();

//true hvis bestilling på input-etg
int queue_order_at_floor_number(int floor);

//fjerner bestilling på input-etg
void queue_remove_order_at_floor_number(int floor);

//returnerer buttoncommand hvis i gyldig etasje
int queue_button_inside_at_floor(elevator_position position);

//returnerer button_call_down hvis i gyldig etasje
int queue_button_down_at_floor(elevator_position position);

//returnerer button_call_up hvis i gyldig etasje
int queue_button_up_at_floor(elevator_position position);

elevator_position queue_get_position();

void queue_set_position(elevator_position pos);

#endif //QUEUE_H