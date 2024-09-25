/** @file drumpad.h
*
* @brief A description of the module’s purpose.
*
*/
#ifndef _DRUMPAD_H_
#define _DRUMPAD_H_

#include <cstdint>
#include "piezo.h"
#include "hi_hat.h"
#include "midi_serial.h"
#include "mbed.h"

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

        drumpad(PinName drumpadLedPin, piezoTransducer * drumpadPiezo, midiMessage_t * midiMessageStruct, hiHat * hiHatController);   
        void drumpadInit(uint8_t dpNumber);
        DRUMPAD_STATE getDrumpadCheck(); 
        void drumpadProcessHit(); 
        void drumpadLedOn();
        void drumpadLedOff();
        void drumpadSetNote(uint8_t note);
        uint8_t drumpadSens;
        uint8_t drumpadNumber;
        bool hiHatControl;
        midiMessage_t * drumpadmidiMessage;
        piezoTransducer * drumpadPiezo;
        hiHat * hiHatControllerPedal;
    private:

        uint8_t drumpadStatus;
        DigitalOut drumpadLed;

};

#endif // _DRUMPAD_H_

/*** end of file ***/