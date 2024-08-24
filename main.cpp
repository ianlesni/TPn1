/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

//=====[Libraries]=============================================================

#include "mbed.h" 
#include "display.h"
#include "midi_serial.h"
#include "instrument.h"
#include "piezo.h"
#include "hi_hat.h"
#include "button.h"
#include "arm_book_lib.h"
#include "ble.h"
#include <cstdint>
#include "GLCD_bitmaps.h"
#include "rotary_encoder.h"

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

/**
*   Esta función borra imprime en el display
*  la nota actual con la que se configuró el 
*  drum pad.
 */
void visualInterfaceUpdate (void);

void piezoDetectorACallback (void);

void piezoReadAndGetMax(void);


//=====[Main function]=========================================================
int main(void)
{
    DigitalOut ledPad(LED1);                                        //Creo un objeto DigitalOut como led testigo de interacción con el drum pad
    /** Creo el transductor piezo eléctrico  
    */
    Ticker piezoAConvertionTicker;
    piezoTransducer piezoA(PinName::A0, PinName::PF_9, &piezoAConvertionTicker);

    hiHat hiHatA(PinName::A1,PinName::PF_7, &piezoA);   
    /** Creo el encoder rotativo 
    */ 
    rotaryEncoder encoder(PinName::PF_8, PinName::PE_3);
    
    /** Creo los pulsadores necesarios para configurar el 
    *   sonido del drum pad    
    */
    DigitalIn upButton(BUTTON1);                                                
    DigitalIn downButton(D1);                                                   

    buttonsArray_t drumPadButtons;
    drumPadButtons.button[0].alias = &upButton;
    drumPadButtons.button[1].alias = &downButton;
    debounceButtonInit(&drumPadButtons);

    /** Creo objeto UnbufferedSerial para realizar
    *   la comunicación serie con la PC   
    */
    UnbufferedSerial uartBle(PD_5, PD_6, 9600);

    UnbufferedSerial uartSerialPort(USBTX, USBRX, 9600);   

    midiMessage_t midiMessageStruct; 

    //initializaMIDISerial(&uartSerialPort, &midiMessageStruct);

    visualInterfaceInit(&ledPad);                                       //Inicializo el led del drum pad y display
       
    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles
    char Note[3] = "";  
    encoder.rotaryEncoderInit(numOfInstrumentNotes);

    while (true)
    {
        /** Actualizo la maquina de estados
        *   que gestiona el debounce de los
        *   pulsadores   
        */
        debounceButtonUpdate(&drumPadButtons);

        if(PIEZO_FINISHED == piezoA.getPiezoStatus())                          //Actualizo y verifico el estado del transductor piezoeléctrico
        {  
            uint16_t piezoMili = 0;

            piezoMili = adcToMilliVolts(piezoA.piezoMaxSampleValue);
            piezoA.piezoMaxVelocity = piezoConvertVoltToVel(piezoMili); 
            ledPad = 1;                                                         //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
            
            midiSendNoteOff(&midiMessageStruct, &uartBle);               //Envío el mensaje de Note Off para no superponer notas

            uint8_t hiHatPos = hiHatA.hiHatGetAperture();
            switch(hiHatPos)
            {
                case OPEN:
                    midiMessageStruct.note = HI_HAT_OPEN;                 
                    break;

                case HALF_OPEN:
                    midiMessageStruct.note = HI_HAT_HALF_OPEN;
                    break;

                case CLOSE:
                    midiMessageStruct.note = HI_HAT_CLOSED;
                    break;

                default:
                    midiMessageStruct.note = CRASH_R_CHOKED; 
                    break;                
            }

            midiMessageStruct.velocity = piezoA.piezoMaxVelocity;               //Cargo la velocity del mensaje               
            midiSendNoteOn(&midiMessageStruct, &uartBle);                //Envío el mensaje de Note On con el parámetro velocity proporcional a la intensidad del golpe
           
            ledPad = 0;                                                         //Apago el Led para indicar que se envió el mensaje correspondiente
            piezoA.piezoTransducerInit();
        }

        if(true == drumPadButtons.button[0].releasedEvent)                      //Verifico si el pulsador upButton fué presionado
        {
            noteIndex++;                                                        //Incremento el indice de navegación de notas
            
            if (noteIndex >= numOfInstrumentNotes) noteIndex = 0;               //Controlo que el indice no se vaya de rango     
            visualInterfaceUpdate();   
        }

        if(true == drumPadButtons.button[1].releasedEvent)                      //Verifico si el pulsador downButton fué presionado
        {
            noteIndex--;                                                        //Decremento el indice de navegación de notas
            if (noteIndex < 0) noteIndex = numOfInstrumentNotes - 1;            //Controlo que el indice no se vaya de rango
            visualInterfaceUpdate();
            
            displayCharPositionWrite (7,2);
            displayStringWrite("  "); 
            sprintf(Note, "%.0hhu", encoder.rotaryEncoderGetCount());
            displayCharPositionWrite (7,2);
            displayStringWrite(Note); 
        }

        
        delay(TIME_INCREMENT_MS);
    }

}

//=====[Implementations of public functions]===================================
void visualInterfaceInit(DigitalOut * Led)
{
    Led->write(0);                                                  //Inicializo el led del drum pad apagado

    displayInit( DISPLAY_TYPE_GLCD_ST7920, DISPLAY_CONNECTION_SPI); 
    

    displayModeWrite( DISPLAY_MODE_CHAR );  
    displayCharPositionWrite(0,0);
    displayStringWrite("MIDI Drum Pad v0");                         
    displayCharPositionWrite (0,1);
    displayStringWrite("WELCOME!..."); 
    delay(1000);                                                   
  
    /** Limpio el display
    *
    */
    displayCharPositionWrite(0,0);
    displayStringWrite("                ");                            
    /** Imprimo la pantalla 
    *   de uso del instrumento
    */
    displayCharPositionWrite(0,0);
    displayStringWrite("Drum Pad note:");
    displayCharPositionWrite (0,2);
    displayStringWrite("COUNT:"); 
    visualInterfaceUpdate();
}

void visualInterfaceUpdate()
{
    displayCharPositionWrite (0,1);                                     
    displayStringWrite("                ");                             //Limpio la pantalla del display
    displayCharPositionWrite (0,1);
    displayStringWrite(instrumentNoteName[noteIndex]);                  //Imprimo el nombre de la nota a ejecutar
}


