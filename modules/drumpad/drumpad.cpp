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
drumpad::drumpad(PinName drumpadLedPin, piezoTransducer * drumpadPiezoTransducer, midiMessage_t * midiMessageStruct, UnbufferedSerial * drumpadSerialPort)
      : drumpadLed(drumpadLedPin),drumpadPiezo(drumpadPiezoTransducer), drumpadmidiMessage(midiMessageStruct), drumpadSerial(drumpadSerialPort)
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
    drumpadPiezo->piezoMaxVelocity = piezoConvertVoltToVel(adcToMilliVolts(drumpadPiezo->piezoMaxSampleValue)); 
    drumpadLed.write(1);                                                            //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
    drumpadmidiMessage->velocity = drumpadPiezo->piezoMaxVelocity;                  //Cargo la velocity del mensaje               
    midiSendNoteOn(drumpadmidiMessage, drumpadSerial);                                   //Envío el mensaje de Note On con el parámetro velocity proporcional a la intensidad del golpe    
    drumpadLed.write(0);                                                            //Apago el Led para indicar que se envió el mensaje correspondiente
    drumpadPiezo->piezoTransducerInit();
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
