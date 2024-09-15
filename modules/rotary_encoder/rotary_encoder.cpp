/** @file rotary_encoder.cpp
*
* @brief Modulo para el manejo del encoder rotativo.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "rotary_encoder.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============
rotaryEncoder::rotaryEncoder(PinName CLKPin, PinName DTPin, Ticker * encoderTicker)
    : CLKInterruptPin(CLKPin),       
      DTDigitalPin(DTPin),
      encoderDebounceTicker(encoderTicker)
{
    CLKInterruptPin.fall(callback(this, &rotaryEncoder::rotaryEncoderCallback));
}
//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================
void rotaryEncoder::rotaryEncoderInit(uint8_t maxCountValue)
{
    rotationCounter = 0;
    encoderDebounceStatus = READY;
    maxRotationCounterValue = maxCountValue;
}

/**
* Incrementa o decrementa el contador de rotación en funcion del sentido de giro del encoder rotativo
*/
void rotaryEncoder::rotaryEncoderCallback()
{
    if(encoderDebounceStatus == READY)
    {
        uint8_t dtStatus = DTDigitalPin.read();  //En función dle estado del pin DT se define el sentido de la rotación
        if(0 == dtStatus) //Rotación antihoraria
        {          
            (*rotationCounter) --;
            if((*rotationCounter) < 0)
            {
                (*rotationCounter) = maxRotationCounterValue - 1;
            }
        }
        else               //Rotación horaria
        {
            (*rotationCounter)++;
            if((*rotationCounter) >= maxRotationCounterValue)
            {
                (*rotationCounter) = 0;
            }
        }
        //A partir de este momento se produce el rebote mecanico del encoder
        encoderDebounceStatus = DEBOUNCING; 
        encoderDebounceTicker->attach(callback(this,&rotaryEncoder::encoderDeounceCallback),100ms); // Permanece en estado DEBOUNCING hasta que transcurran 100ms
    }
}

/**
* Resetea el estado de rebote y desvincula la interrupción del ticker de rebote
* para procesar nuevamente los flancos del pin CLK
*/
void rotaryEncoder::encoderDeounceCallback()
{
    encoderDebounceStatus = READY;              //Una vez que transcurren los 100ms el encoder vuelve a ser utilizable
    encoderDebounceTicker->detach();            //A partir de acá no debe interrumpir más el ticker
}

/**
* Recibe el inidice de navegacion y el maximo valor que puede tomar, y lo asigna a 
* los atributos del encoder
*/
void rotaryEncoder::handleMenuNavigation(int8_t * submenuIndex, int8_t maxIndex)
{
    rotationCounter = submenuIndex;
    maxRotationCounterValue = maxIndex;
}
//=====[Implementations of private functions]==================================


/*** end of file ***/