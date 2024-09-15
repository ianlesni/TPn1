/** @file system_control.h
*
* @brief Modulo para el control del sistema
*
*/

//=====[#include guards - begin]===============================================

#ifndef _SYSTEM_CONTROL_H_
#define _SYSTEM_CONTROL_H_
#include <cstdint>
#include "drumkit.h"
#include "button.h"
//=====[Declaration of public defines]=========================================
#define NUM_OF_MAIN_MENU_OPTIONS 3
#define NUM_OF_CONFIG_MENU_OPTIONS 2
#define NUM_OF_DRUMKIT_MENU_OPTIONS 4
#define NUM_OF_DRUMPAD_MENU_OPTIONS 3
#define NUM_OF_MIDIDRUMKIT_MENU_OPTIONS 2
#define NUM_OF_CONNECTION_MENU_OPTIONS 2

const typedef enum{
    USB_CONN = 0,
    BT_CONN = 1
}SET_USB_CONN_OPTIONS;

const typedef enum{
    BT_ON = 0,
    BT_OFF = 1
}SET_BT_CONN_OPTIONS;

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

// Enumeración para el menú principal
typedef enum {
    MAIN_MENU_PLAY = 0,
    MAIN_MENU_CONFIG = 1,
    MAIN_MENU_SET_VOLUME = 2
} MainMenuIndexOptions;

// Enumeración para el menú de configuración
typedef enum {
    CONFIG_MENU_DRUMKIT = 0,
    CONFIG_MENU_CONNECTION = 1
} ConfigMenuIndexOptions;

// Enumeración para el submenú de drumkit
typedef enum {
    DRUMKIT_MENU_NUMBER = 0,
    DRUMKIT_MENU_PADS = 1,
    DRUMKIT_MENU_MIDI = 2,
    DRUMKIT_MENU_SAVE = 3
} DrumkitMenuIndexOptions;

// Enumeración para el submenú de pads
typedef enum {
    DRUMPAD_MENU_NUMBER = 0,
    DRUMPAD_MENU_NOTE = 1,
    DRUMPAD_MENU_SENSIBILITY = 2
} DrumpadMenuIndexOptions;

// Enumeración para el submenú MIDI del drumkit
typedef enum {
    MIDI_DRUMKIT_MENU_CHANNEL = 0,
    MIDI_DRUMKIT_MENU_VOLUME = 1
} MidiDrumkitMenuIndexOptions;

// Enumeración para el menú de conexión
typedef enum {
    CONNECTION_MENU_USB = 0,
    CONNECTION_MENU_BT = 1
} ConnectionMenuIndexOptions;

// Estados de control de maquinas de estado
extern DrumPadState currentState;
extern DrumPadState previousState;

// Indices para la navegacion por los menus y submenus
extern int8_t mainMenuIndex;
extern int8_t configMenuIndex;
extern int8_t drumkitMenuIndex;
extern int8_t drumpadMenuIndex;
extern int8_t midiDrumkitMenuIndex;
extern int8_t connectionMenuIndex;
extern int8_t setUSBConnIndex;
extern int8_t setBTConnIndex;
extern int8_t selectedDrumpad;
extern int8_t selectedNote;
extern int8_t selectedSensibility;
extern int8_t selectedConn;
extern int8_t selectedChannel;
extern int8_t selectedVolume;
extern bool   volumeFromMainMenu; //variable auxiliar para utilizar la función de volumen en 2 menus

// variables auxiliares para debugear la navegacion por el menu
extern int8_t drumkitChannel; //de 0 a 10
extern int8_t drumkitVolume;  //de 0 a 127
extern int8_t drumpadNumber;  //de 0 a 4
extern int8_t drumpadNote;  //de 0 a 14
extern int8_t drumpadSensibility;  //de 1 a 10

extern char drumkitchannelstr[3];
extern char drumkitVolumestr[4]; 
extern char drumpadNumberstr[3];
extern char numOfInstrumentNotesstr [3];
extern char drumpadSensibilitystr [3];
extern char Note[3]; 

//Menu
void updateDisplay(drumkit * activedrumkit);
void handleMenuNavigation(void);
void confirmButtonPressed(void);
void confirmSelection(drumkit * activedrumkit); 
void returnToPreviousMenu(drumkit * activedrumkit); 
void handleButtonEvents(drumkit * activedrumkit,buttonsArray_t * drumPadButtons);
void updateMenuAndDisplay(drumkit * activedrumkit);
/**
 * Inicialización los dispositivos de salida.
 * 
 * Esta función inicializa el led y el display del drum pad.
 */
void initDisplay (void);

/**
*   Esta función borra imprime en el display
*  la nota actual con la que se configuró el 
*  drum pad.
 */
void visualInterfaceUpdate (void);
//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================


//=====[#include guards - end]=================================================

#endif // _SYSTEM_CONTROL_H_

/*** end of file ***/