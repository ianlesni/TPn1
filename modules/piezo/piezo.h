/** @file piezo.h
*
* @brief Módulo para gestionar el transductor piezoeléctrico.
*
*/
#ifndef _PIEZO_H_
#define _PIEZO_H_

#include <cstdint>
#include "mbed.h"

#define SENSIBILITY_LEVELS 4            // Cantidad de niveles de sensibilidad para el transductor piezoeléctrico

/*!
 * \enum PIEZO_STATE
 * \brief Enumeración de los estados del transductor piezoeléctrico. 
 */
typedef enum{
    PIEZO_IDLE = 0,         /**< El transductor no recibió ningun golpe */
    PIEZO_ACTIVE = 1,       /**< El transductor recibió un golpe que supera el umbral */
    PIEZO_FINISHED =2       /**< Proceso de medición finalizado */
}PIEZO_STATE; 

/*!
 * \enum piezoSensibility_t
 * \brief Niveles de sensibilidad del transductor.
 */
typedef enum{
    SENSITIVITY_LOW = 0,
    SENSITIVITY_MEDIUM = 1,
    SENSITIVITY_HIGH = 2,
    SENSITIVITY_VERY_HIGH = 3
}piezoSensibility_t; 

// Niveles de sensibilidad disponibles
const uint8_t piezoSensibility[]
{
    SENSITIVITY_LOW,
    SENSITIVITY_MEDIUM,
    SENSITIVITY_HIGH,
    SENSITIVITY_VERY_HIGH 
};

/*!
 * \class piezoTransducer
 * \brief Clase para manejar un transductor piezoeléctrico.
 * 
 * Proporciona métodos para inicialización, ajuste de sensibilidad,
 * y consulta del estado del piezo.
 */
class piezoTransducer{
    public:
        piezoTransducer(PinName piezoADPin, PinName piezoIntPin,Ticker * piezoConvertionTicker);   
        void piezoTransducerInit();                     // Inicializa el transductor
        void setPiezoSensibility(uint8_t sensibility);  // Configura la sensibilidad
        PIEZO_STATE getPiezoStatus();                   // Devuelve el estado actual del transductor piezoeléctrico
        
        uint16_t piezoMaxSampleValue;                   // Valor máximo de muestra registrada
        uint16_t piezoMaxVelocity;                      // Máxima velocidad calculada
        int16_t piezoThresholdmV;                       // Umbral de detección en [mV]
        int16_t piezoMaxPeakVoltmV;                     // Pico máximo registrado en [mV]

    private:
        void piezoIntCallback();                        // Callback de interrupción del comparador del transductor piezoeléctrico
        void piezoReadAndGetMax();                      // Lee el ADC y calcula el valor máximo
        void piezoTransducerReset();                    // Resetea las variables del transductor
        void calculateSlopeIntercept();                 // Calcula pendiente e intersección para conversión

        Ticker * piezoConvertionTicker;                 // Ticker para controlar las conversiones del ADC
        AnalogIn piezoAD;                               // Objeto AnalogIn para leer el transductor piezoeléctrico
        InterruptIn piezoInterruptPin;                  // Pin de interrupción del comparador del transductor piezoeléctrico

        uint16_t elapsedADConvertionTime = 0;           //Contador necesario para determinar si se trata de un golpe de interes o un rebote espurio
        PIEZO_STATE piezoStatus;                        // Estado actual del transductor
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


/*!
 * \brief Convierte un valor ADC en milivoltios [mV].
 */
uint16_t adcToMilliVolts (uint16_t adcValue);

/*!
 * \brief Convierte el voltaje máximo en un valor de velocidad (0-127).
 */
 uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue);

#endif // _PIEZO_H_

/*** end of file ***/