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
#include "drumpad.h"
#include "drumkit.h"
#include <cstdint>
#include "rotary_encoder.h"
#include "midi_serial.h"
#include "system_control.h"

//=====[Declaration of defines]================================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of  public global variables]===============================

//=====[Declarations (prototypes) of public functions]=========================

void piezoDetectorACallback (void);
void piezoReadAndGetMax(void);

//=====[Main function]=========================================================
int main(void)
{
    initDisplay();

    UnbufferedSerial uartBle(PD_5, PD_6, 9600);
    UnbufferedSerial uartSerialPort(USBTX, USBRX, 115200); 
  
    Ticker hHChickPedalTicker;
    hiHat hiHatController(PinName::A1,PinName::PF_7,&hHChickPedalTicker);
    Ticker piezo0ConvertionTicker;
    piezoTransducer piezo0(PinName::A2, PinName::PE_6, &piezo0ConvertionTicker);
    midiMessage_t midiMessage0;
    drumpad pad0(LED1,&piezo0, &midiMessage0,&hiHatController);
    
    Ticker piezoAConvertionTicker;
    piezoTransducer piezo1(PinName::A0, PinName::PF_9, &piezoAConvertionTicker);
    midiMessage_t midiMessage1;
    drumpad pad1(LED2,&piezo1, &midiMessage1,NULL);
    //hiHat hiHatA(PinName::A1,PinName::PF_7, &piezoA);  
    Ticker piezoBConvertionTicker;
    piezoTransducer piezo2(PinName::A3, PinName::PG_1, &piezoBConvertionTicker);
    midiMessage_t midiMessage2;
    drumpad pad2(LED3,&piezo2, &midiMessage2,NULL);

    // Arreglo de punteros a drumpads
    drumpad* pads[] = { &pad0,&pad1,&pad2};
    bool drumkitCommMode = 0;
    drumkit kit(3, pads, &uartSerialPort, &uartBle, drumkitCommMode);

    kit.init();
    
    /** Creo los pulsadores necesarios para configurar el 
    *   sonido del drum pad    
    */
    DigitalIn upButton(BUTTON1);                                                
    DigitalIn downButton(D1);                                                   

    buttonsArray_t drumPadButtons;
    drumPadButtons.button[0].alias = &upButton;
    drumPadButtons.button[1].alias = &downButton;
    debounceButtonInit(&drumPadButtons);

    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles

    while (true)
    {
        debounceButtonUpdate(&drumPadButtons);
        switch (currentState)
        {
            case PLAY_SCREEN:
            handleButtonEvents(&kit,&drumPadButtons);
            kit.processHits();
            break;

            case DRUMPAD_MENU:
            case SET_DRUMPAD_NOTE:
            case SET_DRUMPAD_SENSIBILITY:
            case SET_DRUMKIT_VOLUME:
                updateMenuAndDisplay(&kit);
                handleButtonEvents(&kit,&drumPadButtons);
                kit.processHits();
            break;

            default:
                updateMenuAndDisplay(&kit);
                handleButtonEvents(&kit,&drumPadButtons);
            break;
        }

        delay(1);
    }

}

//=====[Implementations of public functions]===================================

