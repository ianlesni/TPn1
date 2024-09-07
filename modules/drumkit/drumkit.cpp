/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "drumkit.h"
#include "instrument.h"
#include <cstdint>

//=====[Declaration of private defines]========================================

//=====[Declaration of public classes]=====================================
drumkit::drumkit(int numPads, drumpad** pads, UnbufferedSerial * UARTserialPort, UnbufferedSerial * BTserialPort, bool commMode)
    : numOfPads(numPads), drumPads(pads), drumkitUARTSerial(UARTserialPort), drumkitBTSerial(BTserialPort){}


void drumkit::init() {

    communicationMode = UART;
    drumkitChannel = 0;
    setMIDIChannel(drumkitChannel);

    for (uint8_t initIndex = 0; initIndex < numOfPads; initIndex++)
    {
        drumPads[initIndex]->drumpadInit(initIndex);

        if (initIndex == 0)
        {
            drumPads[initIndex]->drumpadmidiMessage->note = RIDE;
        }
    }
}

void drumkit::updateDrumkit(uint8_t drumkitNum, uint8_t drumpadNum, uint8_t drumpadNote)
{
    drumkitNumber = drumkitNum;
    drumPads[drumpadNum]->drumpadmidiMessage->note = instrumentNote[drumpadNote];
}

void drumkit::drumkitVolumeUpdate()
{
        switch(communicationMode)
    {
        case UART:

        midiControlChangeVolume(drumkitVolume, drumkitChannel, drumkitUARTSerial);
        break;

        case BT:
        midiControlChangeVolume(drumkitVolume, drumkitChannel, drumkitBTSerial);
        break;
        
    }
}

void drumkit::processHits() 
{
    for (int i = 0; i < numOfPads; i++) 
    {
        if (drumPads[i]->getDrumpadCheck() == ACTIVE) 
        {
            switch(communicationMode)
            {
                case UART:
                midiSendNoteOn(drumPads[i]->drumpadmidiMessage, drumkitUARTSerial);

                break;

                case BT:
                midiSendNoteOn(drumPads[i]->drumpadmidiMessage, drumkitBTSerial);
                break;
                
            }

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
