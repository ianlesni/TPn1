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
rotaryEncoder::rotaryEncoder(PinName CLKPin,PinName DTPin)
    : CLKInterruptPin(CLKPin),       
      DTDigitalPin(DTPin)
{
    CLKInterruptPin.fall(callback(this, &rotaryEncoder::rotaryEncoderCallback));
}

void rotaryEncoder::rotaryEncoderInit(uint8_t maxCountValue)
{
    rotationCounter = 0;
    auxCounter = 0;
    maxRotationCounterValue = maxCountValue;
}

void rotaryEncoder::rotaryEncoderResetCount()
{
    rotationCounter = 0;
}

void rotaryEncoder::rotaryEncoderCallback()
{
    uint8_t dtStatus = DTDigitalPin.read();
    if(0 == dtStatus)
    {   
        auxCounter --;
        rotationCounter = auxCounter/2;
        if(rotationCounter <= 0)
        {
            rotationCounter = maxRotationCounterValue - 1;
        }
    }
    else
    {
        auxCounter++;
        rotationCounter = auxCounter/2;
        if(rotationCounter  >= maxRotationCounterValue)
        {
            rotationCounter = 0;
        }
    }

}

uint8_t rotaryEncoder::rotaryEncoderGetCount()
{
    return rotationCounter;
}
//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================


/*** end of file ***/