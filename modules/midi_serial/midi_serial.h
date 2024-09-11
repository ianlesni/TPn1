/** @file midi_serial.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _MIDI_SERIAL_H_
#define _MIDI_SERIAL_H_
#include <cstdint>

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================
namespace mbed {
    class UnbufferedSerial;
}

/*!
 * \struct midiMessage_t
 * \brief Estructura de un mensaje MIDI
 *
 *Estructura que contiene los campos necesarios para los 
 *mensajes MIDI Note On y Note Off en channel 0
 */
typedef struct{

    uint8_t command;        /**< Comando a transmitir en el mensaje por el canal 0 */
    uint8_t note;           /**< Nota del mensaje */
    uint8_t velocity;       /**< Parámetro velocity del mensaje */

} midiMessage_t; 

//=====[Declarations (prototypes) of public functions]=========================

/** Seteo las propiedades de la comuniación serie 
*  acorde a las preferencias configuradas en el 
*  software Hariless MIDI<->Serial Bridge
*  (9600-8-N-1) y asigno valores iniciales al mensaje midi.
*  @param alias Puntero al objeto responsable de la comunicación serie.
*  @param midimessage Puntero a la estructura que representa el mensaje.
*/
void initializaMIDISerial (mbed::UnbufferedSerial * alias, midiMessage_t * midiMessage);

/**
 * Transmisión a través de serial del mensaje midi de Note On
 *
 * Esta función permite enviar mensajes para que la nota comience a sonar en función de 
 * los parámetros de la estructura del mensaje
 *  @param alias Puntero al objeto responsable de la comunicación serie.
 *  @param midiMessage Puntero a la estructura que representa el mensaje.
 */
void midiSendNoteOn (midiMessage_t * midiMessage, mbed::UnbufferedSerial * alias);

/**
 * Transmisión a través de serial del mensaje midi de Note Off
 *
 * Esta función permite enviar mensajes para que la nota comience a sonar en función de 
 * los parámetros de la estructura del mensaje
 *  @param alias Puntero al objeto responsable de la comunicación serie.
 *  @param midiMessage Puntero a la estructura que representa el mensaje.
 */
void midiSendNoteOff (midiMessage_t * midiMessage, mbed::UnbufferedSerial * alias);

void setMIDIChannel(uint8_t channel);
void midiControlChangeVolume(uint8_t volume, int8_t channel, mbed::UnbufferedSerial * alias);
//=====[#include guards - end]=================================================

#endif // _MIDI_SERIAL_H_

/*** end of file ***/