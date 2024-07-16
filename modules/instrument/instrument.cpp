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
 * Este arreglo almacena los nombres para imprimir sobre un display 
 * 
 */
const char * instrumentNoteName[] = {

    (char * )"KICK",                  /**< Bombo */
    (char * )"SNARE",                 /**< Caja */
    (char * )"SIDE_STICK",            /**< Golpe en el aro */
    (char * )"HI_HAT_CLOSED",         /**< Hi-Hat cerrado */
    (char * )"HI_HAT_HALF_OPEN",      /**< Hi-Hat medio abierto */
    (char * )"HI_HAT_OPEN",           /**< Hi-Hat abierto */
    (char * )"HH_Pedal_CHICK",        /**< Pedal de Hi-Hat */
    (char * )"TOM_HI",                /**< Tom alto */
    (char * )"TOM_MID",               /**< Tom medio */
    (char * )"TOM_LOW",               /**< Tom bajo */
    (char * )"RIDE",                  /**< Platillo Ride */
    (char * )"BELL",                  /**< Campana */
    (char * )"CRASH_L",               /**< Platillo Crash izquierdo */
    (char * )"CRASH_R",               /**< Platillo Crash derecho */
    (char * )"CRASH_R_CHOKED",        /**< Platillo Crash derecho muteado*/
    (char * )"CHINA",                 /**< Platillo China */
    (char * )"SPLASH"                 /**< Platillo Splash */

};


uint8_t  getNumOfInstrumentNotes()
{
    uint8_t numOfInstrumentNotes = sizeof(instrumentNote) / sizeof(instrumentNote[0]);  //Calculo el número total de notas midi de instrumentos percusivos disponibles
    return numOfInstrumentNotes;
}