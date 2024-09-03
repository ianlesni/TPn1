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

//=====[Declaration of defines]================================================

// Definición de estados del sistema
enum DrumPadState {
    MAIN_MENU,
    PLAY_SCREEN,
    CONFIG_MENU,
    DRUMKIT_MENU,
    CONNECTION_MENU,
    SET_DRUMKIT_NUMBER,
    DRUMPAD_MENU,
    MIDI_DRUMKIT_MENU,
    SAVE_OPTION,
    SET_DRUMPAD_NUMBER,
    SET_DRUMPAD_NOTE,
    SET_DRUMPAD_SENSIBILITY,
    SET_DRUMKIT_CHANNEL,
    SET_DRUMKIT_VOLUME,
    SET_USB_CONN,
    SET_BT_CONN,
    IDLE_STATE
};

// Estados de control de maquinas de estado
DrumPadState currentState = MAIN_MENU;
DrumPadState previousState = IDLE_STATE;

// Indices para la navegacion por los menus y submenus
int8_t mainMenuIndex = 0;
int8_t configMenuIndex = 0;
int8_t drumkitMenuIndex = 0;
int8_t drumpadMenuIndex = 0;
int8_t midiDrumkitMenuIndex = 0;
int8_t connectionMenuIndex = 0;
int8_t setUSBConnIndex = 0;
int8_t setBTConnIndex = 0;

// variables auxiliares para debugear la navegacion por el menu
int8_t drumkitChannel = 0; //de 0 a 10
int8_t drumkitVolume = 0;  //de 0 a 127
int8_t drumpadNumber = 0;  //de 0 a 4
int8_t drumpadNote = 0;  //de 0 a 14
int8_t drumpadSensibility = 0;  //de 1 a 10

char drumkitchannelstr[3] = " ";
char drumkitVolumestr[4] = " "; 
char drumpadNumberstr[3] = " ";
char numOfInstrumentNotesstr [3] = " ";
char drumpadSensibilitystr [3] = " ";
char Note[3] = " "; 


//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of  public global variables]===============================

int8_t noteIndex = 0;        /**< Indice para la navegación del arreglo de notas de intrumento */

/** 
* Creo el encoder rotativo 
*/ 
Ticker encoderDebounceticker;
rotaryEncoder encoder(PinName::PF_8, PinName::PE_3, &encoderDebounceticker);


//=====[Declarations (prototypes) of public functions]=========================

/**
 * Inicialización los dispositivos de salida.
 * 
 * Esta función inicializa el led y el display del drum pad.
 */
void visualInterfaceInit (void);

/**
*   Esta función borra imprime en el display
*  la nota actual con la que se configuró el 
*  drum pad.
 */
void visualInterfaceUpdate (void);

void piezoDetectorACallback (void);

void piezoReadAndGetMax(void);

//Menu
void updateDisplay(void);
void handleMenuNavigation(void);
void confirmButtonPressed(void);
void confirmSelection(void);
void returnToPreviousMenu(void); 

//=====[Main function]=========================================================
int main(void)
{
    displayInit(DISPLAY_TYPE_GLCD_ST7920, DISPLAY_CONNECTION_SPI); 
    displayModeWrite(DISPLAY_MODE_CHAR);    
    displayClear();
    displayCharPositionWrite(0,0);
    displayStringWrite("MIDI Drum Pad v1");

    UnbufferedSerial uartBle(PD_5, PD_6, 9600);
    UnbufferedSerial uartSerialPort(USBTX, USBRX, 9600); 
  
    Ticker piezoAConvertionTicker;
    piezoTransducer piezo1(PinName::A0, PinName::PF_9, &piezoAConvertionTicker);
    midiMessage_t midiMessage1;
    drumpad pad1(LED1,&piezo1, &midiMessage1);
    //hiHat hiHatA(PinName::A1,PinName::PF_7, &piezoA);  

    // Arreglo de punteros a drumpads
    drumpad* pads[] = { &pad1};
    bool drumkitCommMode = 0;
    drumkit kit(1, pads, &uartSerialPort, &uartBle, drumkitCommMode);

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

    void visualInterfaceInit();
    //initializaMIDISerial(&uartSerialPort, &midiMessageStruct);

    uint8_t numOfInstrumentNotes = getNumOfInstrumentNotes();           //Obtengo el número total de notas midi de instrumentos percusivos disponibles

    while (true)
    {
        /*
        * Actualizo la maquina de estados
        *   que gestiona el debounce de los
        *   pulsadores   
        */
        debounceButtonUpdate(&drumPadButtons);

        handleMenuNavigation();
        updateDisplay();

        if (true == drumPadButtons.button[1].releasedEvent)  //Pulsador Encoder
        {
            confirmSelection();
        }

        if (true == drumPadButtons.button[0].releasedEvent)  //Pulsador USER
        {
            returnToPreviousMenu();
        }

        kit.processHits();

        delay(2);
    }

}

//=====[Implementations of public functions]===================================
void visualInterfaceInit()
{
    displayInit(DISPLAY_TYPE_GLCD_ST7920, DISPLAY_CONNECTION_SPI); 
    displayModeWrite(DISPLAY_MODE_CHAR);    
    displayClear();
    displayCharPositionWrite(0,0);
    displayStringWrite("MIDI Drum Pad v1");
    displayCharPositionWrite(4,2);
    displayStringWrite("  PLAY");
    displayCharPositionWrite(4,3);
    displayStringWrite("  CONFIG");
}

void updateDisplay() {
    // Actualizar la pantalla según el estado actual y el índice del menú
    switch (currentState)
    {
        case MAIN_MENU:
            if(currentState != previousState) //Imprimo la pantalla completa solamente si es la primera vez que estoy en este estado
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("MIDI Drum Pad v1");
                displayCharPositionWrite(4,2);
                displayStringWrite("  PLAY");
                displayCharPositionWrite(4,3);
                displayStringWrite("  CONFIG");
            }
            if (mainMenuIndex == 0) 
            {   
                displayCharPositionWrite(4,3);
                displayStringWrite("  ");
                displayCharPositionWrite(4,2);
                displayStringWrite("> ");
            }
            if (mainMenuIndex == 1) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("> CONFIG");
            }
            previousState = MAIN_MENU;
        break;

        case PLAY_SCREEN:
            if(currentState != previousState) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("Playing...");
            }
           previousState = PLAY_SCREEN; 
        break;

        case CONFIG_MENU:
            if(currentState != previousState) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("CONFIG MENU");
                displayCharPositionWrite(4,2);
                displayStringWrite("  DRUMKIT");
                displayCharPositionWrite(4,3);
                displayStringWrite("  CONNECTION");
            }

            if (configMenuIndex == 0) 
            {
                displayCharPositionWrite(4,3);
                displayStringWrite("  ");
                displayCharPositionWrite(4,2);
                displayStringWrite("> ");
            }          
            if (configMenuIndex == 1) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("> ");
            }
            previousState = CONFIG_MENU;
        break;

        case DRUMKIT_MENU:
            if(currentState != previousState) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("  DrumKit N:__");
                displayCharPositionWrite(4,1);
                displayStringWrite("  DrumPads");
                displayCharPositionWrite(4,2);
                displayStringWrite("  MIDI");
                displayCharPositionWrite(4,3);
                displayStringWrite("  Save");
            }

            if (drumkitMenuIndex == 0) 
            {   
                displayCharPositionWrite(0,0);
                displayStringWrite("> ");
                displayCharPositionWrite(4,1);
                displayStringWrite("  ");
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("  "); 
            }  
            if (drumkitMenuIndex == 1) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");
                displayCharPositionWrite(4,1);
                displayStringWrite("> ");
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("  "); 
            }          
            if (drumkitMenuIndex == 2) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");                
                displayCharPositionWrite(4,1);
                displayStringWrite("  ");
                displayCharPositionWrite(4,2);
                displayStringWrite("> ");
                displayCharPositionWrite(4,3);
                displayStringWrite("  ");
            }
            if (drumkitMenuIndex == 3) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");
                displayCharPositionWrite(4,1);
                displayStringWrite("  ");
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("> ");
            }   
            previousState = DRUMKIT_MENU;        
        break;

        case DRUMPAD_MENU:
            if(currentState != previousState && previousState != SET_DRUMPAD_NUMBER && previousState != SET_DRUMPAD_NOTE && previousState != SET_DRUMPAD_SENSIBILITY) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("  DrumPad N:__");
                displayCharPositionWrite(0,2);
                displayStringWrite("  Note N:___");
                displayCharPositionWrite(0,3);
                displayStringWrite("  SensibiLity:__");
            }

            if (drumpadMenuIndex == 0) 
            {   
                displayCharPositionWrite(0,0);
                displayStringWrite("> ");
                displayCharPositionWrite(0,2);
                displayStringWrite("  ");
                displayCharPositionWrite(0,3);
                displayStringWrite("  ");
            }  
            if (drumpadMenuIndex == 1) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");
                displayCharPositionWrite(0,2);
                displayStringWrite("> ");
                displayCharPositionWrite(0,3);
                displayStringWrite("  ");
            }          
            if (drumpadMenuIndex == 2) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");                
                displayCharPositionWrite(0,2);
                displayStringWrite("  ");
                displayCharPositionWrite(0,3);
                displayStringWrite("> ");
            }
            previousState = DRUMPAD_MENU; 
        break;

        case SET_DRUMPAD_NUMBER:
            displayCharPositionWrite(0,0);
            displayStringWrite("  DrumPad N:  ");
            sprintf(drumpadNumberstr, "%.0hhu", drumpadNumber);
            displayCharPositionWrite (13,0);
            displayStringWrite(drumpadNumberstr); 
            previousState = SET_DRUMPAD_NUMBER; 
        break;

        case SET_DRUMPAD_NOTE:
            displayCharPositionWrite(0,2);
            displayStringWrite("  Note N:   ");
            sprintf(numOfInstrumentNotesstr, "%.0hhu", drumpadNote);
            displayCharPositionWrite (10,2);
            displayStringWrite(numOfInstrumentNotesstr); 
            previousState = SET_DRUMPAD_NOTE;         
        break;
            
        case SET_DRUMPAD_SENSIBILITY:
            displayCharPositionWrite(0,3);
            displayStringWrite("  SensibiLity:  ");
            sprintf(drumpadSensibilitystr, "%.0hhu", drumpadSensibility);
            displayCharPositionWrite (14,3);
            displayStringWrite(drumpadSensibilitystr); 
            previousState = SET_DRUMPAD_SENSIBILITY;  
        break;

        case MIDI_DRUMKIT_MENU:
            if(currentState != previousState && previousState != SET_DRUMKIT_CHANNEL && previousState != SET_DRUMKIT_VOLUME) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("MIDIDrumkit Conf");
                displayCharPositionWrite(4,2);
                displayStringWrite("  Channel:__");
                displayCharPositionWrite(4,3);
                displayStringWrite("  Volume:___");
            }

            if (midiDrumkitMenuIndex == 0) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("> ");
                displayCharPositionWrite(4,3);
                displayStringWrite("  ");
            }          
            if (midiDrumkitMenuIndex == 1) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("> ");
            }
            previousState = MIDI_DRUMKIT_MENU;
        break;

        case SET_DRUMKIT_CHANNEL:
            displayCharPositionWrite(4,2);
            displayStringWrite("  Channel:  ");
            sprintf(drumkitchannelstr, "%.0hhu", drumkitChannel);
            displayCharPositionWrite (14,2);
            displayStringWrite(drumkitchannelstr); 
            previousState = SET_DRUMKIT_CHANNEL; 
        break;

        case SET_DRUMKIT_VOLUME:
            displayCharPositionWrite(4,3);
            displayStringWrite("  Volume:  ");
            sprintf(drumkitVolumestr, "%.0hhu", drumkitVolume);
            displayCharPositionWrite (13,3);
            displayStringWrite(drumkitVolumestr); 
            previousState = SET_DRUMKIT_VOLUME; 
        break;        

        case CONNECTION_MENU:
            if(currentState != previousState && previousState != SET_USB_CONN && previousState != SET_BT_CONN) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("CONNECTION Conf");
                displayCharPositionWrite(4,2);
                displayStringWrite("  USB:OFF");
                displayCharPositionWrite(4,3);
                displayStringWrite("  BT:OFF");
            }
            if (connectionMenuIndex == 0) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("> ");
                displayCharPositionWrite(4,3);
                displayStringWrite("  ");
            }          
            if (connectionMenuIndex == 1) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  ");
                displayCharPositionWrite(4,3);
                displayStringWrite("> ");
            }
            previousState = CONNECTION_MENU;
        break;

        case SET_USB_CONN:
            if (setUSBConnIndex == 0) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  USB:OFF");
            }          
            if (setUSBConnIndex == 1) 
            {
                displayCharPositionWrite(4,2);
                displayStringWrite("  USB:ON ");
            }
            previousState = SET_USB_CONN;            
        break;

        case SET_BT_CONN:
            if (setBTConnIndex == 0) 
            {
                 displayCharPositionWrite(4,3);
                displayStringWrite("  BT:OFF");
            }          
            if (setBTConnIndex == 1) 
            {
                displayCharPositionWrite(4,3);
                displayStringWrite("  BT:ON ");
            }
            previousState = SET_USB_CONN; 
        break;

        default:
        break;
    }
}

void handleMenuNavigation() 
{
        switch (currentState) 
        {
            case MAIN_MENU:
                encoder.handleMenuNavigation(&mainMenuIndex, 2);    
            break;

            case CONFIG_MENU:
                encoder.handleMenuNavigation(&configMenuIndex, 2);                
            break;

            case DRUMKIT_MENU:
                encoder.handleMenuNavigation(&drumkitMenuIndex, 4);               
            break;

            case DRUMPAD_MENU:
                encoder.handleMenuNavigation(&drumpadMenuIndex, 3);               
            break;

            case SET_DRUMPAD_NUMBER:
                encoder.handleMenuNavigation(&drumpadNumber, 4);  //Con define 
            break;

            case SET_DRUMPAD_NOTE:
                encoder.handleMenuNavigation(&drumpadNote, 14);  //Que sea con un define          
            break;

            case SET_DRUMPAD_SENSIBILITY:
                encoder.handleMenuNavigation(&drumpadSensibility, 9);  
            break;

            case MIDI_DRUMKIT_MENU:
                encoder.handleMenuNavigation(&midiDrumkitMenuIndex, 2);               
            break;

            case CONNECTION_MENU:
                encoder.handleMenuNavigation(&connectionMenuIndex, 2);               
            break;

            case SET_USB_CONN:
                encoder.handleMenuNavigation(&setUSBConnIndex, 2);               
            break;

            case SET_BT_CONN:
                encoder.handleMenuNavigation(&setBTConnIndex, 2);               
            break;

            case SET_DRUMKIT_CHANNEL:
                encoder.handleMenuNavigation(&drumkitChannel, 10);
            break;

            case SET_DRUMKIT_VOLUME:
                encoder.handleMenuNavigation(&drumkitVolume, 127);
            break;

            default:
                encoder.handleMenuNavigation(&mainMenuIndex, 2);
            break;
    }

}

void confirmSelection() 
{
    switch (currentState) 
    {
        case MAIN_MENU:
            if (mainMenuIndex == 0) 
            {
                currentState = PLAY_SCREEN; 
            } 
            else if (mainMenuIndex == 1) 
            {
                currentState = CONFIG_MENU;
            }
        break;

        case CONFIG_MENU:
            if (configMenuIndex == 0) 
            {
                currentState = DRUMKIT_MENU;
            } 
            else if (configMenuIndex == 1) 
            {
                currentState = CONNECTION_MENU;
            }

        break;

        case DRUMKIT_MENU:
            if (drumkitMenuIndex == 0) 
            {
                //Selección de numero de drumkit
                currentState = SET_DRUMKIT_NUMBER;  
            } 
            else if (drumkitMenuIndex == 1) 
            {
                currentState = DRUMPAD_MENU;
            }
            else if (drumkitMenuIndex == 2) 
            {
                currentState = MIDI_DRUMKIT_MENU;
            }
            else if (drumkitMenuIndex == 3) 
            {
                //Si apreto aca tengo que guardar en la sd, mostrarlo en pantalla y nada mas
                currentState = SAVE_OPTION;
            }            
        break;

        case DRUMPAD_MENU:
            if (drumpadMenuIndex == 0) 
            {
                //Selección de numero de drumpad
                currentState = SET_DRUMPAD_NUMBER;
            } 
            else if (drumpadMenuIndex == 1) 
            {
                currentState = SET_DRUMPAD_NOTE;
            }
            else if (drumpadMenuIndex == 2) 
            {
                currentState = SET_DRUMPAD_SENSIBILITY;
            }        
        break;


        case SET_DRUMPAD_NUMBER:
            if(previousState == SET_DRUMPAD_NUMBER)
            {
                returnToPreviousMenu();
            }
            else
            {
                encoder.handleMenuNavigation(&drumpadNumber, 10);  //Sacar esto de aca
            }
        break;

        case SET_DRUMPAD_NOTE:
            if(previousState == SET_DRUMPAD_NOTE)
            {
                returnToPreviousMenu();
            }
            else
            {
                encoder.handleMenuNavigation(&drumpadNote, 14);  //Que sea con un define
            }
        break;

        case SET_DRUMPAD_SENSIBILITY:
            if(previousState == SET_DRUMPAD_SENSIBILITY)
            {
                returnToPreviousMenu();
            }
            else
            {
                encoder.handleMenuNavigation(&drumpadSensibility, 9);  
            }
        break;

        case MIDI_DRUMKIT_MENU:
            if (midiDrumkitMenuIndex == 0) 
            {
                //Selección de numero de drumpad
                currentState = SET_DRUMKIT_CHANNEL;
            } 
            else if (midiDrumkitMenuIndex == 1) 
            {
                currentState = SET_DRUMKIT_VOLUME;
            }      

        break;

        case CONNECTION_MENU:
            if (connectionMenuIndex == 0) 
            { 
                currentState = SET_USB_CONN;
            } 
            else if (connectionMenuIndex == 1) 
            {  
                currentState = SET_BT_CONN;
            }      
        break;

        case SET_USB_CONN:
            if (setUSBConnIndex == 0) 
            { 
                //le aviso al drum pad que va a transmitir por UART
                returnToPreviousMenu();
            } 
            else if (setUSBConnIndex == 1) 
            {  
                //le aviso al drum pad que no va a transmitir por UART
                returnToPreviousMenu();

            }  
        break;

        case SET_BT_CONN:
            if (setBTConnIndex == 0) 
            { 
                //lo prendo
                returnToPreviousMenu();
            } 
            else if (setBTConnIndex == 1) 
            {  
                //lo apago
                returnToPreviousMenu();
            }  
        break;

        case SET_DRUMKIT_CHANNEL:
            if(previousState == SET_DRUMKIT_CHANNEL)
            {
                returnToPreviousMenu();
            }
            else
            {
                encoder.handleMenuNavigation(&drumkitChannel, 10);  //Sacar esto de aca
            }
        break;

        case SET_DRUMKIT_VOLUME:
            if(previousState == SET_DRUMKIT_VOLUME)
            {
                returnToPreviousMenu();
            }
            else
            {
                encoder.handleMenuNavigation(&drumkitVolume, 127);  //Sacar esto de aca
            }
        break;

        default:
        break;
    }
    updateDisplay();
}

void returnToPreviousMenu() 
{
    switch (currentState) 
    {
        case PLAY_SCREEN:
        case CONFIG_MENU:
            currentState = MAIN_MENU;
        break;

        case DRUMKIT_MENU:
        case CONNECTION_MENU:
            currentState = CONFIG_MENU;
        break;

        case SET_DRUMKIT_NUMBER:
        case DRUMPAD_MENU:
        case MIDI_DRUMKIT_MENU:
        case SAVE_OPTION:
            currentState = DRUMKIT_MENU; 
        break;

        case SET_DRUMPAD_NUMBER:
        case SET_DRUMPAD_NOTE:
        case SET_DRUMPAD_SENSIBILITY:
            currentState = DRUMPAD_MENU;
        break;
            
        case SET_DRUMKIT_CHANNEL:
        case SET_DRUMKIT_VOLUME:
            currentState = MIDI_DRUMKIT_MENU;
        break;

        case SET_USB_CONN:
        case SET_BT_CONN:
            currentState = CONNECTION_MENU;
        break;

        default:
        break;
    }
    updateDisplay();
}