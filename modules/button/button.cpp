/** @file midi_uart.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "button.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

/*!
 * \enum BUTTON_STATE
 * \brief Enumeración de los estados de los pulsadores.
 * 
 */
typedef enum{
    
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING

}BUTTON_STATE;

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================
void debounceButtonInit(button_t *button) 
{
    /** En función del estado fisico del 
    *  pulsador defino el estado para la
    *  FSM e inicializo los parámetros
    *  del pulsador
    */
    if (1 == button->alias->read()) {
        button->currentState = BUTTON_DOWN;
    } else {
        button->currentState = BUTTON_UP;
    }
    button->accumulatedDebounceTime = 0;
    button->releasedEvent = false;
}

/** FSM del libro adaptada para  
*  poder utilizarse con más de
*  un pulsador
*/
bool debounceButtonUpdate(button_t *button) 
{
    button->releasedEvent = false;
    switch (button->currentState) {
        case BUTTON_UP:
            if (1 == button->alias->read()) {
                button->currentState = BUTTON_FALLING;
                button->accumulatedDebounceTime = 0;
            }
            break;

        case BUTTON_FALLING:
            if (button->accumulatedDebounceTime >= DEBOUNCE_BUTTON_TIME_MS) {
                if (1 == button->alias->read()) {
                    button->currentState = BUTTON_DOWN;
                } else {
                    button->currentState = BUTTON_UP;
                }
            }
            button->accumulatedDebounceTime += TIME_INCREMENT_MS;
            break;

        case BUTTON_DOWN:
            if (0 == button->alias->read()) {
                button->currentState = BUTTON_RISING;
                button->accumulatedDebounceTime = 0;
            }
            break;

        case BUTTON_RISING:
            if (button->accumulatedDebounceTime >= DEBOUNCE_BUTTON_TIME_MS) {
                if (0 == button->alias->read()) {
                    button->currentState = BUTTON_UP;
                    button->releasedEvent = true;
                } else {
                    button->currentState = BUTTON_DOWN;
                }
            }
            button->accumulatedDebounceTime += TIME_INCREMENT_MS;
            break;

        default:
            debounceButtonInit(button);
            break;
    }
    return button->releasedEvent;
}

//=====[Implementations of private functions]==================================


/*** end of file ***/