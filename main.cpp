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
#include "button.h"
#include "arm_book_lib.h"
#include "ble.h"
#include <cstdint>

//=====[Declaration of defines]================================================

//=====[Declaration and initialization of public global objects]===============
Ticker piezoAConvertionTimer;
AnalogIn piezoA(A0);
piezo_t piezoAStruct;
    
InterruptIn piezoADetector(PF_9);
//=====[Declaration of  public global variables]===============================

int8_t noteIndex = 0;        /**< Indice para la navegación del arreglo de notas de intrumento */
uint8_t sampleCount = 0;
uint8_t status = 0;

    uint16_t piezoMaxSample = 0;                                         /**< Valor máximo del golpe registrado por el transductor piezoeléctrico*/
    uint16_t piezoSample = 0; 
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

    piezoInit(&piezoA, &piezoAStruct);

    piezoADetector.fall(piezoDetectorACallback);   

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
    UnbufferedSerial uartSerialPort(USBTX, USBRX);   
    UnbufferedSerial bleSerialPort(PD_5, PD_6);     //PD_5 UART2_TX to RXD ; PD_6 UART2_RX to TXD 

    midiMessage_t midiMessageStruct; 

    initializaMIDISerial(&uartSerialPort, &midiMessageStruct);

    //initializateBlePort(&bleSerialPort);

    visualInterfaceInit(&ledPad);                                       //Inicializo el led del drum pad y display
       
    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles
                                            
    while (true)
    {
        /** Actualizo la maquina de estados
        *   que gestiona el debounce de los
        *   pulsadores   
        */
        debounceButtonUpdate(&drumPadButtons);

        if(1 == status)                          //Actualizo y verifico el estado del transductor piezoeléctrico
        {  
            uint16_t piezoMili = 0;
            piezoMili = adcToMilliVolts(piezoAStruct.MaxValue);
            piezoAStruct.MaxVelocity = piezoConvertVoltToVel(piezoMili); 
            ledPad = 1;                                                         //Enciendo el Led para confirmar que se realizó un golpe que superó el umbral de activación
            midiSendNoteOff(&midiMessageStruct, &uartSerialPort);                   //Envío el mensaje de Note Off para no superponer notas
            midiMessageStruct.note = instrumentNote[noteIndex];                 //Cargo la nota del mensaje
            midiMessageStruct.velocity = piezoAStruct.MaxVelocity;              //Cargo la velocity del mensaje               
            midiSendNoteOn(&midiMessageStruct, &uartSerialPort);                    //Envío el mensaje de Note On con el parámetro velocity proporcional a la intensidad del golpe
            ledPad = 0;                                                         //Apago el Led para indicar que se envió el mensaje correspondiente
            status = 0;
        }

        if(true == drumPadButtons.button[0].releasedEvent)                      //Verifico si el pulsador upButton fué presionado
        {
            noteIndex++;                                                        //Incremento el indice de navegación de notas
            if (noteIndex >= numOfInstrumentNotes) noteIndex = 0;               //Controlo que el indice no se vaya de rango     
            BLESetATConfig(&bleSerialPort);
            visualInterfaceUpdate();
        }

        if(true == drumPadButtons.button[1].releasedEvent)                      //Verifico si el pulsador downButton fué presionado
        {
            noteIndex--;                                                        //Decremento el indice de navegación de notas
            if (noteIndex < 0) noteIndex = numOfInstrumentNotes - 1;            //Controlo que el indice no se vaya de rango
            visualInterfaceUpdate();
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
    visualInterfaceUpdate();
}

void visualInterfaceUpdate()
{
    displayCharPositionWrite (0,1);                                     
    displayStringWrite("                ");                             //Limpio la pantalla del display
    displayCharPositionWrite (0,1);
    displayStringWrite(instrumentNoteName[noteIndex]);                  //Imprimo el nombre de la nota a ejecutar
}

void piezoDetectorACallback()
{
    piezoAConvertionTimer.attach(piezoReadAndGetMax, 100us);
}


void piezoReadAndGetMax()
{
    piezoSample = piezoA.read_u16();                        //Tomo una lectura del transductor piezoeléctrico     
    
    if(piezoSample > piezoMaxSample)                                //Verifico si el nuevo valor leido es mayor al máximo valor registrado en este muestreo
    {
        piezoMaxSample = piezoSample;                                //Actualizo el máximo valor registrado hasta el momento
    }
    piezoAStruct.MaxValue = piezoMaxSample; //Guardo el valor más alto medido

    sampleCount++; // SERÍA MEJOR LLAMRLO "ELAPSED CONVERTION TIME" tiempo de conversión transcurrido
    if(4 == sampleCount) //transcurrieron 400us
    {
        if(1 == piezoADetector.read()) //Subió a alto la señal del detector porque era un pico
        {
            sampleCount = 0;
            piezoSample = 0;
            piezoMaxSample = 0;
            piezoAConvertionTimer.detach();
            status = 0;//Conversion anulada
        }
        
    }
    if(20 == sampleCount) //tomé las muestras necesarias para los 2ms de señal (esto lo podría hacer mirando si el nuevo valor es menor al maáximo -> estaría bajando)
    {
        sampleCount = 0;
        piezoSample = 0;
        piezoMaxSample = 0;
        piezoAConvertionTimer.detach();
        status = 1;//conversion finalizda
    }

}

