/** @file midi_uart.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "midi_uart.h"

//=====[Declaration of private defines]========================================



//=====[Declaration of private data types]=====================================

/*!
 * \enum MIDI_COMMAND
 * \brief Enumeración de comandos MIDI. 
 *
 * Enumeración de los comandos necesarios para formar los mensajes MIDI para el canal 0.
 */
typedef enum{

        NOTE_ON = 0x90,     /**< Byte de comando Note On */
        NOTE_OFF = 0x80     /**< Byte de comando Note Off */

}MIDI_COMMAND; 

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

/** Seteo las propiedades de la comuniación serie 
*  acorde a las preferencias configuradas en el 
*  software Hariless MIDI<->Serial Bridge
*  (9600-8-N-1).
*  @param alias Puntero al objeto .
*/
static void initializaMIDISerialPort(mbed::UnbufferedSerial * alias);

//=====[Implementations of public functions]===================================
void initializaMIDISerial(mbed::UnbufferedSerial * alias, midiMessage_t *midiMessageStruct)
{
    initializaMIDISerialPort(alias);
    midiMessageStruct->command = 0x00;
    midiMessageStruct->note = 0x00;
    midiMessageStruct->velocity = 0x00;
}

void midiSendNoteOn (midiMessage_t * message, UnbufferedSerial * alias)
{               
    message->command = NOTE_ON;             /**< <Comando de Note On en canal 0> */
    alias->write(&message->command, 1);     //Envío el comando y su canal
    alias->write(&message->note, 1);        //Envío la nota       
    alias->write(&message->velocity, 1);    //Envío el valor de velocity 
}

void midiSendNoteOff (midiMessage_t * message, UnbufferedSerial * alias)
{
    message->command = NOTE_OFF;                  /**< <Comando de Note Off en canal 0> */
    alias->write(&message->command, 1);           //Envío el comando y su canal
    alias->write(&message->note, 1);              //Envío la nota
    alias->write(&message->velocity, 1);          //Envío el valor de velocity         
}

//=====[Implementations of private functions]==================================
static void initializaMIDISerialPort(UnbufferedSerial * alias)
{
    alias->baud(38400);
    alias->format(8,SerialBase::None,1);
}

/*** end of file ***/