/** @file system_control.cpp
*
* @brief Modulo para el control del sistema
*
*/

#include "mbed.h"
#include "arm_book_lib.h"
#include "midi_serial.h"
#include "system_control.h"
#include "drumkit.h"
#include "display.h"
#include "instrument.h"
#include "rotary_encoder.h"

/** 
* Creo el encoder rotativo 
*/ 
Ticker encoderDebounceticker;
rotaryEncoder encoder(PinName::PF_8, PinName::PE_3, &encoderDebounceticker);


// Estados iniciales de control de maquina de estado
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
int8_t selectedDrumpad = 0;
int8_t selectedNote = 0;
int8_t selectedSensibility = 0;
int8_t selectedConn = 0;
int8_t selectedChannel = 0;
int8_t selectedVolume = 0;
bool   volumeFromMainMenu = true; //variable auxiliar para utilizar la función de volumen en 2 menus

// variables auxiliares para debugear la navegacion por el menu
int8_t drumkitChannel = 0;      // 0 a 10
int8_t drumkitVolume = 0;       // 0 a 127
int8_t drumpadNumber = 0;       // 0 a 4
int8_t drumpadNote = 0;         // 0 a 14
int8_t drumpadSensibility = 0;  // 1 a 10

// Strings auxiliares para mostrar en el display los valores seleccionados
char drumkitchannelstr[3] = " ";
char drumkitVolumestr[4] = " "; 
char drumpadNumberstr[3] = " ";
char numOfInstrumentNotesstr [3] = " ";
char drumpadSensibilitystr [3] = " ";
char Note[3] = " "; 


void initDisplay()
{
    displayInit(DISPLAY_TYPE_GLCD_ST7920, DISPLAY_CONNECTION_SPI); 
    displayModeWrite(DISPLAY_MODE_CHAR);    
    displayClear();
}

void handleButtonEvents(drumkit * activedrumkit,buttonsArray_t * drumPadButtons)
{
    if (drumPadButtons->button[1].releasedEvent)  // Pulsador Encoder
    {
        confirmSelection(activedrumkit);
    }

    if (drumPadButtons->button[0].releasedEvent)  // Pulsador USER
    {
        returnToPreviousMenu(activedrumkit);
    }
}

void updateMenuAndDisplay(drumkit * activedrumkit) 
{
    handleMenuNavigation();
    updateDisplay(activedrumkit);
}

/*
* Actauliza el display en función del estado del sistema y los valores asociados al drumkit
*/
void updateDisplay(drumkit * activedrumkit) {

    switch (currentState)
    {
        case MAIN_MENU:
            if(currentState != previousState) //Imprimo la pantalla completa solamente si es la primera vez que estoy en este estado
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("MIDI Drum Pad v1");

                displayCharPositionWrite(2,1);
                displayStringWrite("  PLAY");

                displayCharPositionWrite(2,2);
                displayStringWrite("  CONFIG");

                displayCharPositionWrite(2,3);
                displayStringWrite("  Volume:    ");
                sprintf(drumkitVolumestr, "%.0hhu", activedrumkit->drumkitVolume);
                displayCharPositionWrite (12,3);             
                displayStringWrite(drumkitVolumestr);
            }
            if (mainMenuIndex == MAIN_MENU_PLAY) 
            {   
                displayCharPositionWrite(2,1);
                displayStringWrite("> ");
                displayCharPositionWrite(2,2);
                displayStringWrite("  ");
                displayCharPositionWrite(2,3);
                displayStringWrite("  ");                
            }
            if (mainMenuIndex == MAIN_MENU_CONFIG) 
            {
                displayCharPositionWrite(2,1);
                displayStringWrite("  ");
                displayCharPositionWrite(2,2);
                displayStringWrite("> ");
                displayCharPositionWrite(2,3);
                displayStringWrite("  ");  
            }
            if (mainMenuIndex == MAIN_MENU_SET_VOLUME) 
            {
                displayCharPositionWrite(2,1);
                displayStringWrite("  ");
                displayCharPositionWrite(2,2);
                displayStringWrite("  ");
                displayCharPositionWrite(2,3);
                displayStringWrite("> ");  
            }
            previousState = MAIN_MENU;
        break;

        case PLAY_SCREEN:
            if(currentState != previousState) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("MIDI Drumpad V1");
                displayCharPositionWrite(0,1);
                displayStringWrite("----------------");
                displayCharPositionWrite(0,2);
                displayStringWrite("|| LET'S PLAY ||");
                displayCharPositionWrite(0,3);
                displayStringWrite("----------------");
            }
           previousState = PLAY_SCREEN; 
        break;

        case CONFIG_MENU:
            if(currentState != previousState) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("CONFIG MENU");

                displayCharPositionWrite(0,2);
                displayStringWrite("  DRUMKIT");

                displayCharPositionWrite(0,3);
                displayStringWrite("  CONN:   ");
                switch(selectedConn)
                {
                    case UART:
                        displayCharPositionWrite(8,3);
                        displayStringWrite("UART");
                    break;
                    case BT:
                        displayCharPositionWrite(8,3);
                        displayStringWrite("BT  ");
                    break;
                }
            }

            if (configMenuIndex == CONFIG_MENU_DRUMKIT) 
            {
                displayCharPositionWrite(0,3);
                displayStringWrite("  ");
                displayCharPositionWrite(0,2);
                displayStringWrite("> ");
            }          
            if (configMenuIndex == CONFIG_MENU_CONNECTION) 
            {
                displayCharPositionWrite(0,2);
                displayStringWrite("  ");
                displayCharPositionWrite(0,3);
                displayStringWrite("> ");
            }
            previousState = CONFIG_MENU;
        break;

        case CONNECTION_MENU:
            if (connectionMenuIndex == USB_CONN) 
            {
                displayCharPositionWrite(8,3);
                displayStringWrite("UART");
            }          
            if (connectionMenuIndex == BT_CONN) 
            {
                displayCharPositionWrite(8,3);
                displayStringWrite("BT  ");
            }

            previousState = CONNECTION_MENU;
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

            if (drumkitMenuIndex == DRUMKIT_MENU_NUMBER) 
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
            if (drumkitMenuIndex == DRUMKIT_MENU_PADS) 
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
            if (drumkitMenuIndex == DRUMKIT_MENU_MIDI) 
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
            if (drumkitMenuIndex == DRUMKIT_MENU_SAVE) 
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
            if(currentState != previousState && previousState != SET_DRUMPAD_NOTE && previousState != SET_DRUMPAD_SENSIBILITY) 
            {
                displayClear();
                displayCharPositionWrite(0,0);
                displayStringWrite("  DrumPad N:  ");
                sprintf(drumpadNumberstr, "%.0hhu", selectedDrumpad);
                displayCharPositionWrite (13,0);
                displayStringWrite(drumpadNumberstr);

                displayCharPositionWrite(0,1);
                displayStringWrite("  Note N:  ");
                sprintf(numOfInstrumentNotesstr, "%.0hhu", getNoteIndex(activedrumkit->drumPads[selectedDrumpad]->drumpadmidiMessage->note));
                displayCharPositionWrite (10,1);
                displayStringWrite(numOfInstrumentNotesstr); 
                displayCharPositionWrite(0,2);
                displayStringWrite("                ");
                displayCharPositionWrite(0,2);
                displayStringWrite(instrumentNoteName[getNoteIndex(activedrumkit->drumPads[selectedDrumpad]->drumpadmidiMessage->note)]);                  //Imprimo el nombre de la nota a ejecutar
                
                displayCharPositionWrite(0,3);
                displayStringWrite("  SensibiLity:  ");
                sprintf(drumpadSensibilitystr, "%.0hhu", activedrumkit->drumPads[selectedDrumpad]->drumpadSens);
                displayCharPositionWrite (14,3);
                displayStringWrite(drumpadSensibilitystr); 
            }

            if (drumpadMenuIndex == DRUMPAD_MENU_NUMBER) 
            {   
                displayCharPositionWrite(0,0);
                displayStringWrite("> ");
                displayCharPositionWrite(0,1);
                displayStringWrite("  ");
                displayCharPositionWrite(0,3);
                displayStringWrite("  ");
            }  
            if (drumpadMenuIndex == DRUMPAD_MENU_NOTE) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");
                displayCharPositionWrite(0,1);
                displayStringWrite("> ");
                displayCharPositionWrite(0,3);
                displayStringWrite("  ");
            }          
            if (drumpadMenuIndex == DRUMPAD_MENU_SENSIBILITY) 
            {
                displayCharPositionWrite(0,0);
                displayStringWrite("  ");                
                displayCharPositionWrite(0,1);
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
            displayCharPositionWrite(0,1);
            displayStringWrite("  Note N:   ");
            sprintf(drumpadNumberstr, "%.0hhu", selectedDrumpad);
            displayCharPositionWrite (13,0);
            displayStringWrite(drumpadNumberstr);

            sprintf(numOfInstrumentNotesstr, "%.0hhu", drumpadNote);
            displayCharPositionWrite (10,1);
            displayStringWrite(numOfInstrumentNotesstr); 

            displayCharPositionWrite(0,2);
            displayStringWrite("                ");
            displayCharPositionWrite(0,2);
            displayStringWrite(instrumentNoteName[drumpadNote]);                  
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
                displayCharPositionWrite(2,2);
                displayStringWrite("  Channel:  ");
                sprintf(drumkitchannelstr, "%.0hhu", activedrumkit->drumkitChannel);
                displayCharPositionWrite (12,2);
                displayStringWrite(drumkitchannelstr); 

                displayCharPositionWrite(2,3);
                displayStringWrite("  Volume:    ");
                sprintf(drumkitVolumestr, "%.0hhu", activedrumkit->drumkitVolume);
                displayCharPositionWrite (12,3);             
                displayStringWrite(drumkitVolumestr); 
            }

            if (midiDrumkitMenuIndex == MIDI_DRUMKIT_MENU_CHANNEL) 
            {
                displayCharPositionWrite(2,2);
                displayStringWrite("> ");
                displayCharPositionWrite(2,3);
                displayStringWrite("  ");
            }          
            if (midiDrumkitMenuIndex == MIDI_DRUMKIT_MENU_VOLUME) 
            {
                displayCharPositionWrite(2,2);
                displayStringWrite("  ");
                displayCharPositionWrite(2,3);
                displayStringWrite("> ");
            }
            previousState = MIDI_DRUMKIT_MENU;
        break;

        case SET_DRUMKIT_CHANNEL:
            displayCharPositionWrite(2,2);
            displayStringWrite("  Channel:  ");
            sprintf(drumkitchannelstr, "%.0hhu", drumkitChannel =  drumkitChannel);
            displayCharPositionWrite (12,2);
            displayStringWrite(drumkitchannelstr); 
            previousState = SET_DRUMKIT_CHANNEL; 
        break;

        case SET_DRUMKIT_VOLUME:
            displayCharPositionWrite(2,3);
            displayStringWrite("  Volume:    ");
            sprintf(drumkitVolumestr, "%.0hhu", drumkitVolume = drumkitVolume);
            displayCharPositionWrite (12,3);             
            displayStringWrite(drumkitVolumestr); 
            previousState = SET_DRUMKIT_VOLUME; 
        break;        

        default:
        break;
    }
}

/*
* Actualiza el indice a incrementar o decrementar y su valor máximo, permitiendo la 
* navegación por el menu mediante el uso del encoder rotativo
*/
void handleMenuNavigation() 
{
        switch (currentState) 
        {
            case MAIN_MENU:
                encoder.handleMenuNavigation(&mainMenuIndex, NUM_OF_MAIN_MENU_OPTIONS);    
            break;

            case CONFIG_MENU:
                encoder.handleMenuNavigation(&configMenuIndex, NUM_OF_CONFIG_MENU_OPTIONS);                
            break;

            case DRUMKIT_MENU:
                encoder.handleMenuNavigation(&drumkitMenuIndex, NUM_OF_DRUMKIT_MENU_OPTIONS);               
            break;

            case DRUMPAD_MENU:
                encoder.handleMenuNavigation(&drumpadMenuIndex, NUM_OF_DRUMPAD_MENU_OPTIONS);               
            break;

            case SET_DRUMPAD_NUMBER:
                encoder.handleMenuNavigation(&drumpadNumber, NUMBER_OF_DRUMPADS_MAX);   
            break;

            case SET_DRUMPAD_NOTE:
                encoder.handleMenuNavigation(&drumpadNote, NUMBER_OF_NOTES);           
            break;

            case SET_DRUMPAD_SENSIBILITY:
                encoder.handleMenuNavigation(&drumpadSensibility, SENSIBILITY_LEVELS);  
            break;

            case MIDI_DRUMKIT_MENU:
                encoder.handleMenuNavigation(&midiDrumkitMenuIndex, NUM_OF_MIDIDRUMKIT_MENU_OPTIONS);               
            break;

            case CONNECTION_MENU:
                encoder.handleMenuNavigation(&connectionMenuIndex, NUM_OF_CONNECTION_MENU_OPTIONS);               
            break;

            case SET_DRUMKIT_CHANNEL:
                encoder.handleMenuNavigation(&drumkitChannel, 16);
            break;

            case SET_DRUMKIT_VOLUME:
                encoder.handleMenuNavigation(&drumkitVolume, 127);
            break;

            default:
                encoder.handleMenuNavigation(&mainMenuIndex, NUM_OF_MAIN_MENU_OPTIONS);
            break;
    }

}

/*
 * Función para confirmar la selección de opciones en un menú interactivo.
 * Dependiendo del estado actual de la máquina de estados (currentState),
 * se realiza la acción correspondiente y se actualiza la pantalla.
*/
void confirmSelection(drumkit * activedrumkit) 
{
    switch (currentState) 
    {
        // Menu principal
        case MAIN_MENU:
            volumeFromMainMenu = true;      //Indico que el ajuste de volumen se hizo desde el menú principal
            if (mainMenuIndex == MAIN_MENU_PLAY) 
            {
                currentState = PLAY_SCREEN; 
            } 
            else if (mainMenuIndex == MAIN_MENU_CONFIG) 
            {
                currentState = CONFIG_MENU;
            }
            else if (mainMenuIndex == MAIN_MENU_SET_VOLUME) 
            {
                currentState = SET_DRUMKIT_VOLUME;
            }
        break;

        case PLAY_SCREEN:
            returnToPreviousMenu(activedrumkit);
        break;

        case CONFIG_MENU:
            if (configMenuIndex == CONFIG_MENU_DRUMKIT) 
            {
                currentState = DRUMKIT_MENU;
            } 
            else if (configMenuIndex == CONFIG_MENU_CONNECTION) 
            {
                currentState = CONNECTION_MENU;
            }

        break;

        case DRUMKIT_MENU:
            if (drumkitMenuIndex == DRUMKIT_MENU_NUMBER) 
            {
                currentState = SET_DRUMKIT_NUMBER;  
            } 
            else if (drumkitMenuIndex == DRUMKIT_MENU_PADS) 
            {
                currentState = DRUMPAD_MENU;
            }
            else if (drumkitMenuIndex == DRUMKIT_MENU_MIDI) 
            {
                currentState = MIDI_DRUMKIT_MENU;
            }
            else if (drumkitMenuIndex == DRUMKIT_MENU_SAVE) 
            {
                currentState = SAVE_OPTION;
            }            
        break;

        case DRUMPAD_MENU:
            if (drumpadMenuIndex == DRUMPAD_MENU_NUMBER) 
            {    
                currentState = SET_DRUMPAD_NUMBER;
            } 
            else if (drumpadMenuIndex == DRUMPAD_MENU_NOTE) 
            {
                currentState = SET_DRUMPAD_NOTE;
            }
            else if (drumpadMenuIndex == DRUMPAD_MENU_SENSIBILITY) 
            {
                currentState = SET_DRUMPAD_SENSIBILITY;
            }        
        break;

        case SET_DRUMPAD_NUMBER:
            if(previousState == SET_DRUMPAD_NUMBER)
            {
                selectedDrumpad = drumpadNumber;    //Selección de numero de drumpad
                returnToPreviousMenu(activedrumkit);
            }
            else
            {
                handleMenuNavigation(); 
            }
        break;

        case SET_DRUMPAD_NOTE:
            if(previousState == SET_DRUMPAD_NOTE)
            {
                selectedNote = drumpadNote;
                activedrumkit->updateDrumkit(0,selectedDrumpad, selectedNote);  //Actualizo la nota del drumpad              
                returnToPreviousMenu(activedrumkit);
            }
            else
            {
                handleMenuNavigation();
            }
        break;

        case SET_DRUMPAD_SENSIBILITY:
            if(previousState == SET_DRUMPAD_SENSIBILITY)
            {
                selectedSensibility = drumpadSensibility;
                activedrumkit->drumPads[selectedDrumpad]->drumpadPiezo->setPiezoSensibility(piezoSensibility[drumpadSensibility]);  //Actualizo la sensibilidad del drumpad
                returnToPreviousMenu(activedrumkit);
            }
            else
            {
                handleMenuNavigation(); 
            }
        break;

        case MIDI_DRUMKIT_MENU:
            volumeFromMainMenu = false;
            if (midiDrumkitMenuIndex == MIDI_DRUMKIT_MENU_CHANNEL) 
            {
                currentState = SET_DRUMKIT_CHANNEL;
            } 
            else if (midiDrumkitMenuIndex == MIDI_DRUMKIT_MENU_VOLUME) 
            {
                currentState = SET_DRUMKIT_VOLUME;
            }      
            activedrumkit->drumkitVolumeUpdate();   //Actualizo el volumen
        break;

        case CONNECTION_MENU:
            if (connectionMenuIndex == CONNECTION_MENU_USB) 
            { 
                selectedConn = UART;
                activedrumkit->communicationMode = selectedConn;    //Actualizo el modo de comunicación
                returnToPreviousMenu(activedrumkit);
            } 
            else if (connectionMenuIndex == CONNECTION_MENU_BT) 
            {  
                selectedConn = BT;
                activedrumkit->communicationMode = selectedConn;    //Actualizo el modo de comunicación
                returnToPreviousMenu(activedrumkit);
            }      
        break;

        case SET_DRUMKIT_CHANNEL:
            if(previousState == SET_DRUMKIT_CHANNEL)
            {
                activedrumkit->drumkitChannel = drumkitChannel;
                setMIDIChannel(activedrumkit->drumkitChannel);  //Defino el canal del instrumento
                returnToPreviousMenu(activedrumkit);
            }
            else
            {
                handleMenuNavigation();
            }
        break;

        case SET_DRUMKIT_VOLUME:
            if(previousState == SET_DRUMKIT_VOLUME || previousState ==MAIN_MENU)
            {
                activedrumkit->drumkitVolume = drumkitVolume;
                activedrumkit->drumkitVolumeUpdate();
                returnToPreviousMenu(activedrumkit);
            }
            else
            {
                handleMenuNavigation();
            }
        break;

        default:
        break;
    }
    updateDisplay(activedrumkit);
}


/*
* Define el estado del sistema para regresar al menu anterior
*/
void returnToPreviousMenu(drumkit * activedrumkit) 
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

        case SET_DRUMKIT_VOLUME:  
          if(volumeFromMainMenu == true)    //Si el ajuste de volumen se realizó desde el menu principal->regresa al menu principal
          {
            currentState = MAIN_MENU;              
          }
          else
          {
            currentState = MIDI_DRUMKIT_MENU;
          }

        break;
        case SET_DRUMKIT_CHANNEL:
            currentState = MIDI_DRUMKIT_MENU;
        break;

        case SET_USB_CONN:
        case SET_BT_CONN:
            currentState = CONNECTION_MENU;
        break;

        default:
        break;
    }
    updateDisplay(activedrumkit);
}


/*** end of file ***/