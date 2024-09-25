/** @file midi_uart.cpp
*
* @brief A description of the module’s purpose.
*
*/

#include "mbed.h"
#include "arm_book_lib.h"
#include "midi_serial.h"

#define DEFAULT_CHANNEL 0           

uint8_t noteOn = 0x90;                  /**< Byte de comando Note On */
uint8_t noteOff = 0x80;                 /**< Byte de comando Note Off */
uint8_t controlChangeVolume = 0xB0;     /**< Byte de comando de control de volumen de track */

    UnbufferedSerial serialPort(USBTX, USBRX);                          
    midiMessage_t midiMessageStruct; 

/*
* Modifica los bytes de comando en función del canal seleccionado
*/
void setMIDIChannel(uint8_t channel)
{
    noteOn = 0x90 + channel;
    noteOff = 0x80 + channel;
    controlChangeVolume = 0xB0 + channel;
}

void initializaMIDISerial(mbed::UnbufferedSerial * alias, midiMessage_t * midiMessage)
{
    initializaMIDISerialPort(alias);
    midiMessage->command = 0x00;
    midiMessage->note = 0x00;
    midiMessage->velocity = 0x00;
}

void midiControlChangeVolume(uint8_t volume, int8_t channel, mbed::UnbufferedSerial * alias)
{
    if (volume > 127)
    {
        volume = 127;
    }
    if (volume < 0)
    {
        volume = 0;
    }
    channel = channel - 1;
    if(channel < 0)
    {
        channel = 0;
    }
    controlChangeVolume = 0xB0 + channel;
    uint8_t controllerNumber = 0x3F;       /**< Numero de comando de control de Reaper */ 

    alias->write(&controlChangeVolume, 1);     
    alias->write(&controllerNumber, 1);            
    alias->write(&volume, 1);    
}

void midiSendNoteOn(midiMessage_t * midiMessage, mbed::UnbufferedSerial * alias)
{               
    midiMessage->command = noteOn;              //Asigno comando de Note On en canal 0
    alias->write(&midiMessage->command, 1);     //Envío el comando y su canal
    alias->write(&midiMessage->note, 1);        //Envío la nota       
    alias->write(&midiMessage->velocity, 1);    //Envío el valor de velocity 
}

void midiSendNoteOff(midiMessage_t * midiMessage, mbed::UnbufferedSerial * alias)
{
    midiMessage->command = noteOff;                  //Asigno comando de Note Off en canal 0
    alias->write(&midiMessage->command, 1);           //Envío el comando y su canal
    alias->write(&midiMessage->note, 1);              //Envío la nota
    alias->write(&midiMessage->velocity, 1);          //Envío el valor de velocity de apagado        
}


void initializaMIDISerialPort(UnbufferedSerial * alias)
{
    alias->baud(38400);
    alias->format(8,SerialBase::None,1);
}

/*** end of file ***/