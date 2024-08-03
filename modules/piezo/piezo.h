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

namespace mbed {
    class AnalogIn;
}

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

/*!
 * \enum PIEZO_STATE
 * \brief Enumeración de los estados del transductor piezoeléctrico.
 * 
 */
typedef enum{
    
    PIEZO_IDLE = 0,     /**< El transductor no recibió ningun golpe */
    PIEZO_ACTIVE = 1,       /**< El transductor recibió un golpe que supera el umbral */
    PIEZO_FINISHED =2

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
    uint16_t MaxValue;              /**< Máximo valor de velocity registrado */
} piezo_t; 

//=====[Declarations (prototypes) of public functions]=========================

/**
 * Inicializo el piezoelectrico y llamo a la función que calcula
 * la pendiente y ordenada al origen de la recta de conversión
 *  @param alias Puntero al objeto del transductor.
 *  @param piezo Puntero a la estructura que representa el mensaje.
 */
 void piezoInit (mbed::AnalogIn * alias, piezo_t * piezo);


/**
 * Actualizción del estado del transductor piezoeléctrico y envío de mensajes MIDI si se detecta un golpe.
 * 
 * Esta función realiza las siguientes operaciones:
 * 1- Lee el valor actual del transductor piezoeléctrico y lo convierte a [mV].
 * 2- Compara la lectura con el umbral de activación.
 * 3- Si el valor supera el umbral, busca y registra el valor máximo del golpe detectado.
 * 4- Convierte este valor máximo en un valor de velocity y guarda.
 * 5- Actualiza el estado del transductor
 *
 *  @param piezo Puntero a la estructura que representa el un transductor piezoeléctrico.
 *  
 */
uint8_t piezoUpdate (piezo_t * piezo);

 /**
 * Conviersión de un valor de lectura 
 *
 * @param adcValue Valor de adc (uint16_t producto de read_u16)
 * @return  Valor de ADC en [mV].
 */
uint16_t adcToMilliVolts (uint16_t adcValue);

 /**
 * Conviersión de un valor de voltaje [mV] en un valor de velocity.
 * 
 * @param piezoMaxValue Valor máximo de voltaje [mV] registrado por el transductor piezoeléctrico.
 * @return  Valor de velocity correspondiente ajustado dentro del rango permitido(0-127).
 */
 uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue);
//=====[#include guards - end]=================================================

#endif // _PIEZO_H_

/*** end of file ***/