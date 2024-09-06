/** @file midi_uart.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================
#include "instrument.h"

//=====[Declaration of external public global variables]=======================

/*!
 * \brief Arreglo que contiene las notas de diferentes instrumentos.
 *
 * Este arreglo almacena valores que representan distintas notas midi para 
 * generar sonidos de instrumentos de percusión.
 * Cada valor corresponde a una constante que representa
 * una nota de un instrumento específico.
 */
const uint8_t instrumentNote[] = {
    
    0,                     /**< Nada */                
    KICK,                  /**< Bombo */
    SNARE,                 /**< Caja */
    SIDE_STICK,            /**< Golpe en el aro */
    HI_HAT_CLOSED,         /**< Hi-Hat cerrado */
    HI_HAT_HALF_OPEN,      /**< Hi-Hat medio abierto */
    HI_HAT_OPEN,           /**< Hi-Hat abierto */
    HH_Pedal_CHICK,        /**< Pedal de Hi-Hat */
    TOM_HI,                /**< Tom alto */
    TOM_MID,               /**< Tom medio */
    TOM_LOW,               /**< Tom bajo */
    RIDE,                  /**< Platillo Ride */
    BELL,                  /**< Campana */
    CRASH_L,               /**< Platillo Crash izquierdo */
    CRASH_R,               /**< Platillo Crash derecho */
    CRASH_R_CHOKED,        /**< Platillo Crash derecho muteado*/
    CHINA,                 /**< Platillo China */
    SPLASH                 /**< Platillo Splash */

};

/*!
 * \brief Arreglo que contiene los nombres de las notas de diferentes instrumentos.
 *
 */
const char * instrumentNoteName[] = {

    "",                      /**< Nada */
    "KICK",                  /**< Bombo */
    "SNARE",                 /**< Caja */
    "SIDE_STICK",            /**< Golpe en el aro */
    "HI_HAT_CLOSED",         /**< Hi-Hat cerrado */
    "HI_HAT_HALF_OPEN",      /**< Hi-Hat medio abierto */
    "HI_HAT_OPEN",           /**< Hi-Hat abierto */
    "HH_Pedal_CHICK",        /**< Pedal de Hi-Hat */
    "TOM_HI",                /**< Tom alto */
    "TOM_MID",               /**< Tom medio */
    "TOM_LOW",               /**< Tom bajo */
    "RIDE",                  /**< Platillo Ride */
    "BELL",                  /**< Campana */
    "CRASH_L",               /**< Platillo Crash izquierdo */
    "CRASH_R",               /**< Platillo Crash derecho */
    "CRASH_R_CHOKED",        /**< Platillo Crash derecho muteado*/
    "CHINA",                 /**< Platillo China */
    "SPLASH"                 /**< Platillo Splash */

};

//=====[Declaration and initialization of private global variables]============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

/** Calculo el número total de instrumentos mini disponibles
*  para asignarle al pad.
*  @return Numero de instrumentos disponibles para el pad.
*/
uint8_t  getNumOfInstrumentNotes()
{
    uint8_t numOfInstrumentNotes = sizeof(instrumentNote) / sizeof(instrumentNote[0]);  //Calculo el número total de notas midi de instrumentos percusivos disponibles
    return numOfInstrumentNotes;
}

/*** end of file ***/