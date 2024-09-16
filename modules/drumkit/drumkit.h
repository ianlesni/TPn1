/** @file drumkit.h
*
* @brief Modulo para la implementación de los drumkits. 
*/

//=====[#include guards - begin]===============================================
#ifndef _DRUMKIT_H_
#define _DRUMKIT_H_

#include <cstdint>
#include "drumpad.h"
#include "midi_serial.h"
#include "mbed.h"
//=====[Declaration of public defines]=========================================
#define NUMBER_OF_DRUMPADS_MAX 3

typedef enum{
    
    UART = 0,     
    BT = 1

}COMM_MODE; 

//=====[Declaration of public data types]======================================
class drumkit {
    public:
        drumkit(int numPads, drumpad** pads, UnbufferedSerial * UARTserialPort, UnbufferedSerial * BTserialPort, bool commMode); 
        void init(); 
        void processHits(void); 
        void updateDrumkit(uint8_t drumkitNum, uint8_t drumpadNum, uint8_t drumpadNote);
        void drumkitVolumeUpdate();
        uint8_t drumkitNumber;
        uint8_t drumkitVolume;
        uint8_t drumkitChannel;
        uint8_t communicationMode;
        drumpad** drumPads;          
    private:
        int numOfPads;               
        UnbufferedSerial * drumkitUARTSerial;    
        UnbufferedSerial * drumkitBTSerial;    
};

//=====[#include guards - end]=================================================

#endif // _DRUMKIT_H_

/*** end of file ***/