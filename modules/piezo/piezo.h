/** @file piezo.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/
//=====[#include guards - begin]===============================================

#ifndef _PIEZO_H_
#define _PIEZO_H_

#include <cstdint>

// Declaración adelantada de la clase AnalogIn dentro del namespace mbed



//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================
namespace mbed {
    class AnalogIn;
}

/*!
 * \enum PIEZO_STATE
 * \brief Enumeración de los estados del transductor piezoeléctrico.
 * 
 */
typedef enum{
    
    PIEZO_INACTIVE = 0,     /**< El transductor no recibió ningun golpe */
    PIEZO_ACTIVE = 1,       /**< El transductor recibió un golpe que supera el umbral */

}PIEZO_STATE; 

/*!
 * \struct piezo_t
 * \brief Estructura de un transductor piezoeléctrico
 *
 *Estructura para la representación de un transductor piezoeléctrico 
 *y sus valores asociados
 */
typedef struct{

    mbed::AnalogIn * alias;         /**< Puntero a un objeto AnalogIn para implementar un transductor */
    uint8_t currentState;           /**< Estado actual del transductor */
    uint8_t MaxVelocity;            /**< Máximo valor de velocity registrado */

} piezo_t; 

//=====[Declarations (prototypes) of public functions]=========================
/**
 * Calculo de la pendiente y la ordenada al origen de la recta de conversión.
 * 
 * Esta función calcula la pendiente y la ordenada al origen de la recta de conversión
 * entre voltaje y velocity. Utiliza las constantes DELTA_VEL, DELTA_VOLT, MIN_VEL y PIEZO_THRESHOLD_mV. 
 */
 void piezoInit (mbed::AnalogIn * alias, piezo_t * piezoStruct);


/**
 * Actualizción del estado del transductor piezoeléctrico y envío de mensajes MIDI si se detecta un golpe.
 * 
 * Esta función realiza las siguientes operaciones:
 * 1- Lee el valor actual del transductor piezoeléctrico y lo convierte a [mV].
 * 2- Compara la lectura con el umbral de activación.
 * 3- Si el valor supera el umbral, busca y registra el valor máximo del golpe detectado.
 * 4- Convierte este valor máximo en un valor de velocity y guarda.
 * 5- Devuelve el estado actual del transductor
 *
 *  @param piezo Puntero a la estructura que representa el un transductor piezoeléctrico.
 *  @return Estado actual del transductor, `PIEZO_ACTIVE` o `PIEZO_INACTIVE` .
 */
uint8_t piezoUpdate (piezo_t * piezo);

//=====[#include guards - end]=================================================

#endif // _PIEZO_H_

/*** end of file ***/