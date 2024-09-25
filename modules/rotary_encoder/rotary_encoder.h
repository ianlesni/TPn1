/** @file rotary_encoder.h
*
* @brief Modulo para el manejo del encoder rotativo.
*
*/

#ifndef _ROTARY_ENCODER_H_
#define _ROTARY_ENCODER_H_
#include <cstdint>
#include "mbed.h"

/**
 * @brief Estados del encoder rotativo.
*/
typedef enum{
    READY = 0,      //Encoder listo
    DEBOUNCING = 1  //Procesando antirebote
}ENCODER_STATUS;

/**
 * @class rotaryEncoder
 * 
 * @brief Clase para gestionar el encoder rotativo.
 * 
 * Proporciona funciones para inicializar el encoder, realizar la 
 * rutina antirebote y gestionar la navegación de menús.
 */
class rotaryEncoder{
    public:
    rotaryEncoder(PinName CLKPin,PinName DTPin, Ticker * encoderDebounceTicker);
    void rotaryEncoderInit(uint8_t maxCountValue);                      //Inicializa el encoder con un valor máximo de conteo    
    void handleMenuNavigation(int8_t * submenuIndex, int8_t maxIndex);  //Recibe por referencia la variable a incrementar/decrementar y el valor máximo que puede tomar
    int8_t * rotationCounter;                                           //Contador del encoder
    private:
    uint8_t encoderDebounceStatus;
    void rotaryEncoderCallback();
    void encoderDeounceCallback();
    DigitalIn DTDigitalPin;
    InterruptIn CLKInterruptPin;
    Ticker * encoderDebounceTicker;
    uint8_t maxRotationCounterValue;
};

/** Defino el estado inicial de cada pulsador y lo incializo
*  para su posterior uso en la máquina de estados que
*  gestiona la rutina antirebote.
*  @param buttons Estructura para la representación de los pulsadores.
*/
void rotaryEncoderInit();

#endif // _ROTARY_ENCODER_H_

/*** end of file ***/