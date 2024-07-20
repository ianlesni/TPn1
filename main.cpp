/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

//=====[Libraries]=============================================================

#include "mbed.h" 
#include "display.h"
#include "midi_uart.h"
#include "instrument.h"
#include "piezo.h"
#include "button.h"
#include "arm_book_lib.h"
#include <cstdint>

//=====[Declaration of defines]================================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of  public global variables]===============================

int8_t noteIndex = 0;        /**< Indice para la navegación del arreglo de notas de intrumento */

//=====[Declarations (prototypes) of public functions]=========================

/**
 * Inicialización los dispositivos de salida.
 * 
 * Esta función inicializa el led y el display del drum pad.
 */
void visualInterfaceInit (DigitalOut * Led);

//=====[Main function]=========================================================
int main(void)
{
    DigitalOut ledPad(LED1);                                        //Creo un objeto DigitalOut como led testigo de interacción con el drum pad
    
    /** Creo el transductor piezo eléctrico  
    */
    AnalogIn piezoA(A0);
    piezo_t piezoAStruct;
    piezoInit(&piezoA, &piezoAStruct);

    /** Creo los pulsadores necesarios para configurar el 
    *   sonido del drum pad    
    */
    DigitalIn upButton(BUTTON1);                                                
    button_t  upButtonStruct;
    upButtonStruct.alias = &upButton;
    debounceButtonInit(&upButtonStruct);

    DigitalIn downButton(D1);                                                   
    button_t  downButtonStruct;
    downButtonStruct.alias = &downButton;
    debounceButtonInit(&downButtonStruct);

    /** Creo objeto UnbufferedSerial para realizar
    *   la comunicación serie con la PC   
    */
    UnbufferedSerial serialPort(USBTX, USBRX);                          
    midiMessage_t midiMessageStruct; 
    initializaMIDISerial(&serialPort, &midiMessageStruct);

    visualInterfaceInit(&ledPad);                                       //Inicializo el led del drum pad y display
       
    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles
                                            
    while (true)
    {
        /** Actualizo la maquina de estados
        *   que gestiona el debounce de los
        *   pulsadores   
        */
        bool downButtonReleased = debounceButtonUpdate(&downButtonStruct);
        bool upbuttonReleased = debounceButtonUpdate(&upButtonStruct);

        if(PIEZO_ACTIVE == piezoUpdate(&piezoAStruct))                          //Actualizo y verifico el estado del transductor piezoeléctrico
        {  
            ledPad = 1;                                                         //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
            midiSendNoteOff(&midiMessageStruct, &serialPort);                   //Envío el mensaje de Note Off para no superponer notas
            midiMessageStruct.note = instrumentNote[noteIndex];                 //Cargo la nota del mensaje
            midiMessageStruct.velocity = piezoAStruct.MaxVelocity;              //Cargo la velocity del mensaje               
            midiSendNoteOn(&midiMessageStruct, &serialPort);                    //Envío el mensaje de Note On con el parámetro velocity proporcional a la intensidad del golpe
            ledPad = 0;                                                         //Apago el Led para indicar que se envió el mensaje correspondiente
        }

        if(true == upbuttonReleased)                                            //Verifico si el pulsador upButton fué presionado
        {
            noteIndex++;                                                        //Incremento el indice de navegación de notas
            if (noteIndex >= numOfInstrumentNotes) noteIndex = 0;               //Controlo que el indice no se vaya de rango
            displayCharPositionWrite (0,1);                                     
            displayStringWrite("                ");                             //Limpio la pantalla del display
            displayCharPositionWrite (0,1);
            displayStringWrite(instrumentNoteName[noteIndex]);                  //Imprimo el nombre de la nota a ejecutar
        }

        if(true == downButtonReleased)                                          //Verifico si el pulsador downButton fué presionado
        {
            noteIndex--;                                                        //Decremento el indice de navegación de notas
            if (noteIndex < 0) noteIndex = numOfInstrumentNotes - 1;            //Controlo que el indice no se vaya de rango
            displayCharPositionWrite (0,1);
            displayStringWrite("                ");                             //Limpio la pantalla del display
            displayCharPositionWrite (0,1);
            displayStringWrite(instrumentNoteName[noteIndex]);                  //Imprimo el nombre de la nota a ejecutar
        }
        
        delay(TIME_INCREMENT_MS);
    }

}


//=====[Implementations of public functions]===================================
void visualInterfaceInit(DigitalOut * Led)
{
    Led->write(0);                                                  //Inicializo el led del drum pad apagado

    displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);        //Inicializo el display
    /** Genero un mensaje de bienvenida
    *   y el retardo para que pueda 
    *   leerse
    */
    displayCharPositionWrite(0,0);
    displayStringWrite("MIDI Drum Pad v0");                         
    displayCharPositionWrite (0,1);
    displayStringWrite("¡WELCOME!"); 
    delay(1000);                                                   
    /** Limpio el display
    *
    */
    displayCharPositionWrite(0,0);
    displayStringWrite("                ");                         
    displayCharPositionWrite (0,1);
    displayStringWrite("                ");     
    /** Imprimo la pantalla 
    *   de uso del instrumento
    */
    displayCharPositionWrite(0,0);
    displayStringWrite("Drum Pad note:");
    displayCharPositionWrite(0,1);
    displayStringWrite(instrumentNoteName[noteIndex]);
}







