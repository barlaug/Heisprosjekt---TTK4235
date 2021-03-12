#ifndef FSM_H
#define FSM_H

#include "queue.h"
#include "timer.h"

/**
	@file
	@brief .h-file for setting up the elevators finite state machine (fsm). 
*/

/**
    @brief Enum containing the different states of the elevator in the fsm.
 */
typedef enum {INIT, IDLE, MOVING, OPEN_DOOR, STOP_BETWEEN_FLOORS, STOP_AT_FLOOR} elevator_state;

/**
    @brief Variable for storing the current state of the elevator.
 */
extern elevator_state now_state;

/**
    @brief Variable for storing the previous direction the elevator was moving in.
 */
extern HardwareMovement previous_direction;

/**
    @brief Variable for storing the direction the elevator is currently moving in.
 */
extern HardwareMovement current_direction;


/**
    @brief Checks if the elevator receives a floor sensor signal/if the elevator is at a floor.
    @return 1 If the elevator is at a floor.
    @return 0 If the elevator is between two floors/not at a floor.
*/
int fsm_elevator_is_at_floor();

/**
    @brief Updates the position of the elevator when the elevator arrives at a new floor.
*/
void fsm_update_elevator_position_at_floor();

/**
    @brief Updates the position of the elevator when the elevator is between two floors.
*/
void fsm_update_elevator_position_between_floors();

//2 returnerer 1 hvis vi skal stoppe på etasjen
int fsm_stop_elevator_at_floor();

//3 for når vi har stått stille og skal bevege oss igjen. skal vi da opp eller ned?    //hvis allerede i bevegelse (!stop) -> return
//hvis forrige dir = opp og orders over, fortsett oppovover og sett motordir og current dir til opp (så return)
//hvis orders under og forrige dir = ned fortsett ned (samme som over)
void fsm_choose_motor_direction();

//statemachine funksjon med switch
void fsm_state_machine();

#endif //FSM_H