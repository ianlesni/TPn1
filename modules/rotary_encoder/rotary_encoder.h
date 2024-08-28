/** @file rotary_encoder.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _ROTARY_ENCODER_H_
#define _ROTARY_ENCODER_H_
#include <cstdint>
#include "mbed.h"

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================
typedef enum{
    READY = 0,
    DEBOUNCING = 1
}ENCODER_STATUS;


class rotaryEncoder{
    public:
    rotaryEncoder(PinName CLKPin,PinName DTPin, Ticker * encoderDebounceTicker);
    void rotaryEncoderInit(uint8_t maxCountValue);
    void rotaryEncoderDebounceResetCount();
    uint8_t rotaryEncoderGetCount();
    void handleMenuNavigation(int8_t * submenuIndex, int8_t maxIndex);
    int8_t * rotationCounter;
    private:
    uint8_t encoderDebounceStatus;
    uint8_t debounceCounter;
    int8_t auxCounter;
    void rotaryEncoderCallback();
    void encoderDeounceCallback();
    DigitalIn DTDigitalPin;
    InterruptIn CLKInterruptPin;
    Ticker * encoderDebounceTicker;
    uint8_t maxRotationCounterValue;
};


//=====[Declarations (prototypes) of public functions]=========================

/** Defino el estado inicial de cada pulsador y lo incializo
*  para su posterior uso en la máquina de estados que
*  gestiona la rutina antirebote.
*  @param buttons Estructura para la representación de los pulsadores.
*/
void rotaryEncoderInit();

/** Defino el estado inicial de cada pulsador y lo incializo
*  para su posterior uso en la máquina de estados que
*  gestiona la rutina antirebote.
*  @param buttons Estructura para la representación del pulsador.
*/
void debounceButtonUpdate ();
//=====[#include guards - end]=================================================

#endif // _ROTARY_ENCODER_H_

/*** end of file ***/