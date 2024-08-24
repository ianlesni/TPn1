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
#include "mbed.h"


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



class piezoTransducer{
    public:
        piezoTransducer(PinName piezoADPin, PinName piezoIntPin,Ticker * piezoConvertionTicker);   
        void piezoTransducerInit();
        PIEZO_STATE getPiezoStatus();    
        uint16_t piezoMaxSampleValue;  
        uint16_t piezoMaxVelocity;  
    private:
        void piezoIntCallback();
        void piezoReadAndGetMax();
        void piezoTransducerReset();

        Ticker * piezoConvertionTicker;
        AnalogIn piezoAD;
        InterruptIn piezoInterruptPin;

        uint16_t elapsedADConvertionTime = 0;
        PIEZO_STATE piezoStatus;
};



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