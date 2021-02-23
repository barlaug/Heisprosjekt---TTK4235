#ifndef STATE_MACHINE
#define STATE_MACHINE

#include "queue.h"

/*funksjoner:
1) endre heisens elevator_position når den treffer nye etasjer/er mellom etasjer
2) stopp heis på etasje getposition() dersom order_at_floor_number(getposition()) er true
3) req-handler (kanskje ha denne i egen fil) som, hvis motor i ro - heis stille, sjekker request og setter motorretning til opp/ned

states:
init: initialiserer hardware, queue, motordirection, og skrur på motor(?)
idle/tomgang: set motor off, lukk dører, vent på request. kommer hit etter dør lukket på etg eller stop sluppet opp mellom etg, også etter init
bevegelse: skal sette motordirection, skru på motor, etter å ha lukket dør
open door/at floor: komme hit hvis er i etasje og funk 2) er true, skal sette motor lav, starte timer og åpne dør(sette dørlys høy), i tillegg slette queue på den etg.
STOP_between_floors: Hvis STOP trykkes og holdes inne mellom etg. Skal slette queue, motor lav, stopplys høy, nullstille queue. når sluppet går heis til idle
STOP_at_floor: samme som mellom etg men denne i en etasje, i tillegg skal dør=åpen så lenge stop holdes inne. Når STOP slippes gå til åpendør state å ha dør åpen i ytterligere 3 sek.
*/


//states:
typedef enum {INIT, IDLE, MOVING, OPEN_DOOR, STOP_BETWEEN_FLOORS, STOP_AT_FLOOR} elevator_state;

//variabel til nåværende state, starter som init
extern elevator_state now_state;

extern HardwareMovement last_direction;

extern HardwareMovement current_direction;

//statemachine funksjon med switch
void state_machine_switch_case();

//oppdaterer position til floor, egen funksjon til de mellom
void update_elevator_position_at_floor();

//oppdaterer position til between-statsene
void update_elevator_position_between_floors();

//2 returnerer 1 hvis vi skal stoppe på etasjen
int stop_elevator_at_floor();

//3 for når vi har stått stille og skal bevege oss igjen. skal vi da opp eller ned?
void choose_motor_direction();

#endif //STATE_MACHINE
