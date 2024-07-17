/** @file button.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <cstdint>

//=====[Declaration of public defines]=========================================
#define DEBOUNCE_BUTTON_TIME_MS 30
#define TIME_INCREMENT_MS 1
//=====[Declaration of public data types]======================================
namespace mbed{
    class DigitalIn;
}

typedef enum{
    
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING

}buttonState_t;

/*!
 * \struct button_t
 * \brief Estructura de un pulsador
 *
 *Estructura para la representación de un pulsador y sus 
 *estados necesarios para ejecutar la rutina anti rebote
 */
typedef struct{

    mbed::DigitalIn * alias;                 /**< Puntero a un objeto DigitalIn para implementar un pulsador */
    uint8_t currentState;                    /**< Estado actual del pulsador */
    uint8_t accumulatedDebounceTime;
    bool relasedEvent;

} button_t; 


//=====[Declarations (prototypes) of public functions]=========================
void debounceButtonInit (button_t * button);

bool debounceButtonUpdate(button_t * button);
//=====[#include guards - end]=================================================

#endif // _BUTTON_H_

/*** end of file ***/