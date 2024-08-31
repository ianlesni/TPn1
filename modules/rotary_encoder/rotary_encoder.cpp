/** @file rotary_encoder.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "rotary_encoder.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================
rotaryEncoder::rotaryEncoder(PinName CLKPin, PinName DTPin, Ticker * encoderTicker)
    : CLKInterruptPin(CLKPin),       
      DTDigitalPin(DTPin),
      encoderDebounceTicker(encoderTicker)
{
    CLKInterruptPin.fall(callback(this, &rotaryEncoder::rotaryEncoderCallback));
}

void rotaryEncoder::rotaryEncoderInit(uint8_t maxCountValue)
{
    rotationCounter = 0;
    auxCounter = 0;
    debounceCounter = 0;
    encoderDebounceStatus = READY;
    maxRotationCounterValue = maxCountValue;
}

void rotaryEncoder::encoderDeounceCallback()
{
    encoderDebounceStatus = READY;              //Una vez que transcurren los 100ms el encoder vuelve a ser utilizable
    encoderDebounceTicker->detach();            //A partir de acá no debe interrumpir más el ticker
}

void rotaryEncoder::rotaryEncoderDebounceResetCount()
{
    debounceCounter = 0;
}

void rotaryEncoder::rotaryEncoderCallback()
{

    if(encoderDebounceStatus == READY)
    {
        uint8_t dtStatus = DTDigitalPin.read();
        if(0 == dtStatus)
        {          
            (*rotationCounter) --;
            if((*rotationCounter) <= 0)
            {
                (*rotationCounter) = maxRotationCounterValue - 1;
            }
        }
        else
        {
            (*rotationCounter)++;
            if((*rotationCounter)  >= maxRotationCounterValue)
            {
                (*rotationCounter) = 0;
            }
        }
        encoderDebounceStatus = DEBOUNCING; //A partir de este momento seguramente este haciendo bouncing
        encoderDebounceTicker->attach(callback(this,&rotaryEncoder::encoderDeounceCallback),100ms); // Hasta que no transcurran 100ms, se ignoran los flancos

    }

}

void rotaryEncoder::handleMenuNavigation(int8_t * submenuIndex, int8_t maxIndex)
{
    rotationCounter = submenuIndex;
    maxRotationCounterValue = maxIndex;
}

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================


/*** end of file ***/