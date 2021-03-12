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

/**
    @brief Checks orders, elevator position and direction, and decides whether to stop or not at the floor the elevator is passing by.
    @return 1 If the elevator should stop.
    @return 0 If the elevator should keep moving.
*/
int fsm_stop_elevator_at_floor();


/**
    @brief Checks orders, elevator position and direction, and chooses motordirection up/down when the elevator goes from still to moving.
*/
void fsm_choose_motor_direction();

/**
    @brief The finite state machine switch-case function. Sets and changes the elevator state.
*/
void fsm_state_machine();

#endif //FSM_H