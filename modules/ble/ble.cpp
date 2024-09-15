/** @file midi_uart.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "midi_serial.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================

//=====[Implementations of public functions]===================================
 void initializateBlePort(UnbufferedSerial * alias)
{
    alias->baud(38400);
    alias->format(8,SerialBase::None,1);
}
void BLESetATConfig (UnbufferedSerial * alias)
{
    alias->write("AT+NAME=MIDI\r\n",14);
    alias->write("AT+ROLE=0",9);                //Lo configuro como slave 
    alias->write("AT+PWD=1234\r\n",13);         //PWD = 1234
    alias->write("AT+UART=38400,0,0,\r\n",20);  //Baud rate 38400, 1 stop bit, sin bit de paridad
}
/*** end of file ***/