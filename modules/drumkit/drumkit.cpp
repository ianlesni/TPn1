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
#include "midi_serial.h"
#include <cstdint>

//=====[Declaration of private defines]========================================

//=====[Declaration of public classes]=====================================
drumkit::drumkit(int numPads, drumpad** pads, UnbufferedSerial * UARTserialPort, UnbufferedSerial * BTserialPort, bool commMode)
    : numOfPads(numPads), drumPads(pads), drumkitUARTSerial(UARTserialPort), drumkitBTSerial(BTserialPort){}


void drumkit::init() 
{
    communicationMode = UART;
    drumkitChannel = 1;
    drumkitVolume = 64;
    setMIDIChannel(drumkitChannel);
    midiControlChangeVolume(drumkitVolume, drumkitChannel, drumkitUARTSerial);

    for (uint8_t initIndex = 0; initIndex < numOfPads; initIndex++)
    {
        drumPads[initIndex]->drumpadInit(initIndex);

        if (initIndex == 0) //el drumpad 0 es el hihat
        {
            drumPads[initIndex]->hiHatControl = 1;
        }
    }
    drumPads[0]->drumpadmidiMessage->note = HI_HAT_HALF_OPEN;
    drumPads[1]->drumpadmidiMessage->note = SNARE;
    drumPads[2]->drumpadmidiMessage->note = KICK;    
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
    for (int drumpadIndex = 0; drumpadIndex < numOfPads; drumpadIndex++) 
    {
            if(drumPads[0]->hiHatControllerPedal->pedalChick == true) //Verifico si se pisó el pedal
            {
                drumPads[0]->drumpadmidiMessage->note = HH_Pedal_CHICK;
                drumPads[0]->drumpadmidiMessage->velocity = 0x2D;
                drumPads[0]->hiHatControllerPedal->resetPedalChick();

                    switch(communicationMode)
                {
                    case UART:
                        midiSendNoteOn(drumPads[0]->drumpadmidiMessage, drumkitUARTSerial);
                    break;

                    case BT:
                        midiSendNoteOn(drumPads[0]->drumpadmidiMessage, drumkitBTSerial);
                    break;               
                }
            }

        if (drumPads[drumpadIndex]->getDrumpadCheck() == ACTIVE) 
        {
            if(drumpadIndex == 0) //Si es el hiHat actualizo la nota en funcion de la apertura del pedal de control
            {   
                uint8_t hiHatPos = drumPads[drumpadIndex]->hiHatControllerPedal->hiHatGetAperture();
                switch(hiHatPos)
                {
                    case OPEN:
                        drumPads[drumpadIndex]->drumpadmidiMessage->note = HI_HAT_OPEN;                 
                        break;

                    case HALF_OPEN:
                        drumPads[drumpadIndex]->drumpadmidiMessage->note = HI_HAT_HALF_OPEN;
                        break;

                    case CLOSE:
                        drumPads[drumpadIndex]->drumpadmidiMessage->note = HI_HAT_CLOSED;
                        break;

                    default:
                        drumPads[drumpadIndex]->drumpadmidiMessage->note = CRASH_R_CHOKED; 
                        break;                
                }
            }

            switch(communicationMode)
            {
                case UART:
                    midiSendNoteOn(drumPads[drumpadIndex]->drumpadmidiMessage, drumkitUARTSerial);
                break;

                case BT:
                    midiSendNoteOn(drumPads[drumpadIndex]->drumpadmidiMessage, drumkitBTSerial);
                break;               
            }

            drumPads[drumpadIndex]->drumpadLedOff();

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
