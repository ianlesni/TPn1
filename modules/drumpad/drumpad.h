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
        drumpad(PinName drumpadLedPin, piezoTransducer * drumpadPiezo, midiMessage_t * midiMessageStruct, UnbufferedSerial * drumpadSerialPort);   
        void drumpadInit();
        DRUMPAD_STATE getDrumpadStatus(); 
        void drumpadProcessHit(); 
        uint8_t drumpadSens;
    private:
        uint8_t drumpadStatus;
        DigitalOut drumpadLed;
        piezoTransducer * drumpadPiezo;
        midiMessage_t * drumpadmidiMessage;
        UnbufferedSerial * drumpadSerial;
};


//=====[#include guards - end]=================================================

#endif // _DRUMPAD_H_

/*** end of file ***/