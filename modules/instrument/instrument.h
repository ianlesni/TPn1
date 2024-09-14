/** @file instrument.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _INSTRUMET_H_
#define _INSTRUMET_H_
#include <cstdint>

//=====[Declaration of public defines]=========================================
#define NUMBER_OF_NOTES 17

//=====[Declaration of public data types]======================================
extern int8_t noteIndex;        /**< Indice para la navegación del arreglo de notas de intrumento */
/*!
 * \enum INSTRUMENT_NOTES
 * \brief Enumeración notas midi
 *
 *Enumeración de las notas midi correspondientes a los instrumentos percusivos
 *para el correcto mapeo en el plugin PowerDrumKit instalado en Reaper
 */
const typedef enum{

    KICK = 36,              /**< Bombo */
    SNARE = 38,             /**< Caja */
    SIDE_STICK = 37,        /**< Golpe en el aro */
    HI_HAT_CLOSED = 42,     /**< Hi-Hat cerrado */
    HI_HAT_HALF_OPEN = 44,  /**< Hi-Hat medio abierto */
    HI_HAT_OPEN = 46,       /**< Hi-Hat abierto */
    HH_Pedal_CHICK = 65,    /**< Pedal de Hi-Hat */
    TOM_HI = 48,            /**< Tom alto */
    TOM_MID = 45,           /**< Tom medio */
    TOM_LOW = 41,           /**< Tom bajo */
    RIDE = 51,              /**< Platillo Ride */
    BELL = 53,              /**< Campana */
    CRASH_L = 49,           /**< Platillo Crash izquierdo */
    CRASH_R = 57,           /**< Platillo Crash derecho */
    CRASH_R_CHOKED = 58,    /**< Platillo Crash derecho muteado */
    CHINA = 52,             /**< Platillo China */
    SPLASH = 55             /**< Platillo Splash */

} INSTRUMENT_NOTES; 

//=====[Declaration of external public global variables]=======================

extern const uint8_t instrumentNote[];
extern const char * instrumentNoteName[];

//=====[Declarations (prototypes) of public functions]=========================
uint8_t getNumOfInstrumentNotes (void);
uint8_t getNoteIndex(uint8_t midiNote);


#endif // _INSTRUMET_H_

/*** end of file ***/