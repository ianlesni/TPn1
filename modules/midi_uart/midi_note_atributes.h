#include <cstdint>
/*!
 * \enum INSTRUMENT_NOTES
 * \brief Enumeración notas midi
 *
 *Enumeración de las notas midi correspondientes a los instrumentos percusivos
 *para el correcto mapeo en el plugin PowerDrumKit instalado en Reaper
 */
typedef enum{

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


/*!
 * \brief Arreglo que contiene las notas de diferentes instrumentos.
 *
 * Este arreglo almacena valores que representan distintas notas midi para 
 * generar sonidos de instrumentos de percusión.
 * Cada valor corresponde a una constante que representa
 * una nota de un instrumento específico.
 */
uint8_t instrumentNote[] = {

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
char * instrumentNoteName[] = {

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