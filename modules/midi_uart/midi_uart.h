/** @file midi_uart.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _MIDI_UART_H_
#define _MIDI_UART_H_
#include <cstdint>

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================
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

/**
 * Transmisión a través de UART del mensaje midi de Note On
 *
 * Esta función permite enviar mensajes para que la nota comience a sonar en función de 
 * los parámetros de la estructura del mensaje
 *  @param message Puntero a la estructura que representa el mensaje.
 */
void midiSendNoteOn (midiMessage_t * message);

/**
 * Transmisión a través de UART del mensaje midi de Note Off
 *
 * Esta función permite enviar mensajes para que la nota deje de sonar en función de 
 * los parámetros de la estructura del mensaje
 *  @param message Puntero a la estructura que representa el mensaje.
 */
void midiSendNoteOff (midiMessage_t * message);

//=====[#include guards - end]=================================================

#endif // _MIDI_UART_H_

/*** end of file ***/