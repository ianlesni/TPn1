/** @file drumpad.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/
//=====[#include guards - begin]===============================================

#ifndef _DRUMPAD_H_
#define _DRUMPAD_H_

#include <cstdint>
#include "piezo.h"
#include "midi_serial.h"
#include "mbed.h"

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

/*!
 * \enum DRUMPAD_STATE
 * 
 */
typedef enum{
    
    IDLE = 0,     
    ACTIVE = 1

}DRUMPAD_STATE; 



class drumpad{
    public:

        drumpad(PinName drumpadLedPin, piezoTransducer * drumpadPiezo, midiMessage_t * midiMessageStruct);   
        void drumpadInit(uint8_t dpNumber);
        DRUMPAD_STATE getDrumpadCheck(); 
        void drumpadProcessHit(); 
        void drumpadLedOn();
        void drumpadLedOff();
        void drumpadSetNote(uint8_t note);
        uint8_t drumpadSens;
        uint8_t drumpadNumber;
        midiMessage_t * drumpadmidiMessage;
    private:

        uint8_t drumpadStatus;
        DigitalOut drumpadLed;
        piezoTransducer * drumpadPiezo;

};


//=====[#include guards - end]=================================================

#endif // _DRUMPAD_H_

/*** end of file ***/