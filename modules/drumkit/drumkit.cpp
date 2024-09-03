/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "drumkit.h"
#include <cstdint>

//=====[Declaration of private defines]========================================

//=====[Declaration of public classes]=====================================
drumkit::drumkit(int numPads, drumpad** pads, UnbufferedSerial * UARTserialPort, UnbufferedSerial * BTserialPort, bool commMode)
    : numOfPads(numPads), drumPads(pads), drumkitUARTSerial(UARTserialPort), drumkitBTSerial(BTserialPort){}


void drumkit::init() {
    for (int i = 0; i < numOfPads; i++)
    {
        drumPads[i]->drumpadInit();
    }
}

void drumkit::processHits() 
{
    for (int i = 0; i < numOfPads; i++) 
    {
        if (drumPads[i]->getDrumpadCheck() == ACTIVE) 
        {
            midiSendNoteOn(drumPads[i]->drumpadmidiMessage, drumkitUARTSerial);
            midiSendNoteOn(drumPads[i]->drumpadmidiMessage, drumkitBTSerial);
            drumPads[i]->drumpadLedOff();

        }
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
