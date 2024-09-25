/** @file ble.cpp
*
* @brief Modulo para implementar la comunicacion Bluetooth
*
*/


#include "mbed.h"
#include "arm_book_lib.h"
#include "midi_serial.h"


 void initializateBlePort(UnbufferedSerial * alias)
{
    alias->baud(38400);
    alias->format(8,SerialBase::None,1);
}

/*** end of file ***/