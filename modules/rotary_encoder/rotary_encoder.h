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

class rotaryEncoder{
    public:
    rotaryEncoder(PinName CLKPin,PinName DTPin);
    void rotaryEncoderInit(uint8_t maxCountValue);
    void rotaryEncoderResetCount();
    uint8_t rotaryEncoderGetCount();
    int8_t rotationCounter;
    private:
    int8_t auxCounter;
    void rotaryEncoderCallback();
    DigitalIn DTDigitalPin;
    InterruptIn CLKInterruptPin;
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