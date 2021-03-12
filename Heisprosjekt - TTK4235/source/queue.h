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

/**
    @brief Initializes the queue matrix.
*/
void queue_init_queue();

/**
    @brief Checks if the queue contains order(s).
    @return 1 If order(s) in queue.
    @return 0 If no orders in queue.
*/
int queue_queue_exists();

/**
    @brief Checks the hardware for orders and inserts them in the queue. Sets order-lights in hardware. 
*/
void queue_iterate_and_update_queue();

/**
    @brief Checks the queue for orders above the elevators current position.
    @param position Current position of the elevator.
    @return 1 If there exists orders above current elevator position.
    @return 0 If there are no orders above current elevator position.
*/
int queue_order_above_current_position(elevator_position position);

/**
    @brief Checks the queue for orders below the elevators current position.
    @param position Current position of the elevator.
    @return 1 If there exists orders below current elevator position.
    @return 0 If there are no orders below current elevator position.
*/
int queue_order_below_current_position(elevator_position position);

/**
    @brief Removes all orders from queue.
*/
void queue_delete_queue();

/**
    @brief Checks if there exists orders at input floor.
    @param floor Floor to check for orders.
    @return 1 If there exists orders at input floor.
    @return 0 If there are no orders at input floor.
*/
int queue_order_at_floor_number(int floor);

/**
    @brief Removes all orders at the input floor from the queue.
    @param floor The floor which should be cleared for orders in the queue. 
*/
void queue_remove_order_at_floor_number(int floor);

/**
    @brief Checks if the button inside of the elevator is pressed at the input position.
    @param position Current position of the elevator.
    @return 1 If the inside-button i pressed.
    @return 0 If the inside-button is not pressed.
*/
int queue_button_inside_at_floor(elevator_position position);

/**
    @brief Checks if the down-button outside of the elevator is pressed at the input position.
    @param position Current position of the elevator.
    @return 1 If the down-button outside of the elevator is pressed.
    @return 0 If the down-button outside of the elevator is not pressed.
*/
int queue_button_down_at_floor(elevator_position position);

/**
    @brief Checks if the up-button outside of the elevator is pressed at the input position.
    @param position Current position of the elevator.
    @return 1 If the up-button outside of the elevator is pressed.
    @return 0 If the up-button outside of the elevator is not pressed.
*/
int queue_button_up_at_floor(elevator_position position);

/**
    @brief Get function for the elevators position.
    @return The current position of the elevator.
*/
elevator_position queue_get_position();

/**
    @brief Set function for the elevators position.
    @param pos The position elevator position is changed to.
*/
void queue_set_position(elevator_position pos);

#endif //QUEUE_H