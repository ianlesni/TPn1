/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "piezo.h"
#include "drumpad.h"
#include "fixedpoint.h"
#include "instrument.h"
#include <cstdint>

//=====[Declaration of private defines]========================================                                  

//=====[Declaration of public classes]=====================================
drumpad::drumpad(PinName drumpadLedPin, piezoTransducer * drumpadPiezoTransducer, midiMessage_t * midiMessageStruct)
      : drumpadLed(drumpadLedPin),drumpadPiezo(drumpadPiezoTransducer), drumpadmidiMessage(midiMessageStruct)
{}          

void drumpad::drumpadInit()
{ 
    drumpadStatus = IDLE;
    drumpadmidiMessage->note = SNARE;
    drumpadmidiMessage->velocity = 0x94;
    drumpadLed.write(0);
}

void drumpad::drumpadProcessHit()
{
    drumpadLedOn();
    drumpadPiezo->piezoMaxVelocity = piezoConvertVoltToVel(adcToMilliVolts(drumpadPiezo->piezoMaxSampleValue)); 
    drumpadmidiMessage->velocity = drumpadPiezo->piezoMaxVelocity;                  //Cargo la velocity del mensaje               
    drumpadPiezo->piezoTransducerInit();                                            //Re inicializo el piezo para reestablecer los valores
    drumpadLed.write(0);                                                            //Apago el Led para indicar que se envió el mensaje correspondiente

}

void drumpad::drumpadLedOn()
{
    drumpadLed.write(1);                                                            //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
}

void drumpad::drumpadLedOff()
{
    drumpadLed.write(0);                                                            //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
}

DRUMPAD_STATE drumpad::getDrumpadCheck()
{
    if(PIEZO_FINISHED == drumpadPiezo->getPiezoStatus())
    {
        drumpadProcessHit();
        return ACTIVE;
    }
    else
    {
        return IDLE; 
    }
}
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============
   

//=====[Declarations (prototypes) of private functions]========================


//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================


/*** end of file ***/
