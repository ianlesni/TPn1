/** @file drumkit.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/
//=====[#include guards - begin]===============================================

#ifndef _DRUMKIT_H_
#define _DRUMKIT_H_

#include <cstdint>
#include "drumpad.h"
#include "midi_serial.h"
#include "mbed.h"


//=====[Declaration of public defines]=========================================
#define NUMBER_OF_DRUMPADS_MAX 4


typedef enum{
    
    UART = 0,     
    BT = 1

}COMM_MODE; 

//=====[Declaration of public data types]======================================

class drumkit {
    public:
        
        drumkit(int numPads, drumpad** pads, UnbufferedSerial * UARTserialPort, UnbufferedSerial * BTserialPort, bool commMode); // Constructor: recibe la cantidad de pads, un arreglo de drumpads y dos puertos serial  
        void init(); // Inicializo todos los drumpads del drumkit
        void processHits(void); // Proceso los golpes de todos los drumpads
        void updateDrumkit(uint8_t drumkitNum, uint8_t drumpadNum, uint8_t drumpadNote);
        uint8_t drumkitNumber;
        uint8_t drumkitVolume;
        uint8_t drumkitChannel;
        uint8_t communicationMode;
        drumpad** drumPads;          // Arreglo de punteros a drumpads
    private:
        int numOfPads;               // Número de drumpads en el drumkit
        UnbufferedSerial * drumkitUARTSerial;    // Puerto serial para enviar los mensajes MIDI
        UnbufferedSerial * drumkitBTSerial;    // Puerto serial para enviar los mensajes MIDI
};

//=====[#include guards - end]=================================================

#endif // _DRUMKIT_H_

/*** end of file ***/