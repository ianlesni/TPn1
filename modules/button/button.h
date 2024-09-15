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

#define DEBOUNCE_BUTTON_TIME_MS 30       /**< Tiempo de espera asociado al rebote típico de los pulsadores */
#define TIME_INCREMENT_MS 2              /**< Intervalo incremental del contador de tiempo de rebote  */
#define NUM_BUTTONS 2                    /**< Número de pulsadores activos de mi sistema  */  
//=====[Declaration of public data types]======================================

namespace mbed{
    class DigitalIn;
}

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
    uint8_t accumulatedDebounceTime;         /**< Estado actual del pulsador */
    bool releasedEvent;                      /**< Flag para indicar si el pulsador ha sido liberado desde el ultimo chequeo */

} button_t; 


typedef struct {
    button_t button[NUM_BUTTONS];           /**< Arreglo de pulsadores */
} buttonsArray_t;
//=====[Declarations (prototypes) of public functions]=========================

void debounceButtonInit (buttonsArray_t * buttonsArray);

void debounceButtonUpdate (buttonsArray_t * buttonsArray);
//=====[#include guards - end]=================================================

#endif // _BUTTON_H_

/*** end of file ***/