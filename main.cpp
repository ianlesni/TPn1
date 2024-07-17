/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

 /*************************************************
 *=====[Bibliotecas]===============================
 *************************************************/
#include "mbed.h" 
#include "display.h"
#include "midi_uart.h"
#include "instrument.h"
#include "piezo.h"
#include "button.h"
#include "arm_book_lib.h"
#include <cstdint>

 /*************************************************
 *=====[Definiciones]==============================
 *************************************************/

 /*******************************************************************
 *=====[Declaración e inicialización de objetos globales]============
 *******************************************************************/

DigitalOut ledPad(LED1);                                    //Creo un objeto DigitalOut como led testigo de interacción con el drum pad
  
/********************************************************************
 *=======[Declaración e inicialización de variables globales]========
 ********************************************************************/

/*!
 * \enum LED_STATE
 * \brief Enumeración de los estados de los leds.
 *
 */
typedef enum{

    LED_ON = 1,     /**< Led encendido */
    LED_OFF = 0     /**< Led apagado */

} LED_STATE; 

/*********************************************************************
 *=====[Declaración (prototipos) de funciones públicas]===============
 ********************************************************************/

/**
 * Inicialización los dispositivos de salida.
 * 
 * Esta función inicializa el led y el display del drum pad.
 */
void visualInterfaceInit (void);

/*************************************************************************
 *======================[Función main]====================================
 ************************************************************************/

int main(void)
{
    /** Creo el transductor piezo eléctrico  
    */
    AnalogIn piezoA(A0);
    piezo_t piezoAStruct;
    piezoInit(&piezoA, &piezoAStruct);
    /** Creo los pulsadores necesarios para configurar el 
    *  sonido del drum pad. 
    *  Estos pulsadores permiten navegar de manera ascendente y 
    *  descendente por el arreglo de notas de instrumentos
    *  disponibles.
    */

    DigitalIn upButton(BUTTON1);                                                //Creo un objeto DigitalIn para la navegación ascendente del arreglo de notas midi disponibles
    button_t  upButtonStruct;
    DigitalIn downButton(D1);                                                   //Creo un objeto DigitalIn para la navegación descendente del arreglo de notas midi disponibles
    button_t  downButtonStruct;

    upButtonStruct.alias = &upButton;
    debounceButtonInit(&upButtonStruct);

    downButtonStruct.alias = &downButton;
    debounceButtonInit(&downButtonStruct);

    UnbufferedSerial serialPort(USBTX, USBRX);   //Creo un objeto UnbufferedSerial para realizar la comunicación serie con la PC.
    midiMessage_t midiMessageStruct; 
    initializaMIDISerial(&serialPort, &midiMessageStruct);

    visualInterfaceInit();                                              //Inicializo el led del drum pad
       
    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles
    uint8_t noteIndex = 0;                                              /**< Indice para la navegación del arreglo de notas de intrumento */

    while (true)
    {
        bool downButtonReleased = debounceButtonUpdate(&downButtonStruct);
        bool upbuttonReleased = debounceButtonUpdate(&upButtonStruct);

        if(PIEZO_ACTIVE == piezoUpdate(&piezoAStruct))                  //Actualizo y verifico el estado del transductor piezoeléctrico
        {  
            ledPad = LED_ON;                                            //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
            midiMessageStruct.note = instrumentNote[noteIndex];         //Cargo la nota del mensaje
            midiMessageStruct.velocity = piezoAStruct.MaxVelocity;      //Cargo la velocity del mensaje              
            midiSendNoteOff(&midiMessageStruct,&serialPort);            //Envío el mensaje de Note Off para no superponer notas 
            midiSendNoteOn(&midiMessageStruct,&serialPort);             //Envío el mensaje de Note On con el parámetro velocity proporcional a la intensidad del golpe
            ledPad = LED_OFF;                                           //Apago el Led para indicar que se envió el mensaje correspondiente
        }

        if(true == upbuttonReleased)             //Verifico si el pulsador upButton fué presionado
        {
            noteIndex++;                                                //Incremento el indice de navegación de notas
            if (noteIndex >= numOfInstrumentNotes) noteIndex = 0;       //Controlo que el indice no se vaya de rango
            displayCharPositionWrite (0,1);
            displayStringWrite("                ");
            displayCharPositionWrite (0,1);
            displayStringWrite(instrumentNoteName[noteIndex]);
        }

        if(true == downButtonReleased)           //Verifico si el pulsador downButton fué presionado
        {
            noteIndex--;                                                //Decremento el indice de navegación de notas
            if (noteIndex < 0) noteIndex = numOfInstrumentNotes - 1;    //Controlo que el indice no se vaya de rango
            displayCharPositionWrite (0,1);
            displayStringWrite("                ");
            displayCharPositionWrite (0,1);
            displayStringWrite(instrumentNoteName[noteIndex]);
        }
        delay(TIME_INCREMENT_MS);
    }

}

/*********************************************************************
 *=====[Implementación de funciones públicas]=========================
 ********************************************************************/

void visualInterfaceInit ()
{
    ledPad = LED_OFF;   //Inicializo el led del drum pad apagado
    displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);
    displayCharPositionWrite(0,0);
    displayStringWrite("MIDI Drum Pad v0");
}







