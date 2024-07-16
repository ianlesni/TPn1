/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "piezo.h"

//=====[Declaration of private defines]========================================
#define MAX_VEL 127                                                 /**< Máximo valor de velocity permitido */
#define MIN_VEL 35                                                  /**< Máximo valor de velocity permitido (para valores menores se escucha muy poco) */
#define DELTA_VEL (MAX_VEL - MIN_VEL)                               /**< Variacion entre el máximo y mínimo valor de velocity permitido*/
#define PIEZO_MAX_PEAK_VOLT_mV 2000                                 /**< Máximo valór de voltaje pico [mV] generado por el transductor piezoelectrico para el circuito de adqiusición actual*/
#define PIEZO_THRESHOLD_mV 90                                       /**< Umbral de voltaje para la detección del golpe [mv] *///Nivel por encima del piso de ruido
#define DELTA_VOLT (PIEZO_MAX_PEAK_VOLT_mV - PIEZO_THRESHOLD_mV)    /**< Variación entre el valor máximo y mínimo de voltaje registrado por el transductor piezoelectrico*/

#define NUMBER_OF_PIEZO_SAMPLES 400     /**< Número de total de muestras para el proceso de muestreo de la señal adquirida por el transductor piezoeléctrico */
#define SAMPLE_FREQ_Hz 40000            /**< Frecuencia de muestreo [Hz] */
#define SAMPLE_TIME_INTERVAL_us 25      /**< Intervalo de muestreo [us] */
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

/** Variables utilizadas para la conversión del valor de voltaje
 *  generado por el transductor piezoelectrico luego del golpe
 *  a un valor proporcional de velocity 
 */
float slope = 0.0;                  /**< Pendiente de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity */
float intercept = 0.0;              /**< Ordenada al origen de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity  */


//=====[Declarations (prototypes) of private functions]========================

/**
 * Calculo de la pendiente y la ordenada al origen de la recta de conversión.
 * 
 * Esta función calcula la pendiente y la ordenada al origen de la recta de conversión
 * entre voltaje y velocity. Utiliza las constantes DELTA_VEL, DELTA_VOLT, MIN_VEL y PIEZO_THRESHOLD_mV. 
 */
 static void calculateSlopeIntercept (void);

 /**
 * Conviersión de un valor de voltaje [mV] en un valor de velocity.
 * 
 * Esta función calcula y convierte un valor de voltaje [mV] registrado por el transductor
 * piezoeléctrico en un valor de celocity MIDI. El valor de velocity se calcula utilizando
 * la pendiente y la ordenada al origen previamente calculadas (slope e intercept).
 * 
 * @param piezoMaxValue Valor máximo de voltaje [mV] registrado por el transductor piezoeléctrico.
 * @return  Valor de velocity correspondiente, redondeado y ajustado dentro del rango permitido(0-127).
 */
static uint8_t piezoConvertVoltToVel (float piezoMaxValue);

/**
 * Busqueda y devolución del valor máximo del golpe registrado por el transductor piezoeléctrico.
 * 
 * Esta función realiza un muestreo de la señal analógica proveniente del transductor piezoeléctrico
 * y determina el valor máximo de voltaje [mV] registrado durante el proceso de muestreo.
 * 
 * @param piezo Puntero a la estructura que representa el un transductor piezoeléctrico.
 * @return Valor máximo de voltaje [mV] registrado durante el muestreo.
 */
static  float piezoSearchMax (piezo_t * piezo);

//=====[Implementations of public functions]===================================
 void piezoInit(mbed::AnalogIn * alias, piezo_t * piezoStruct)
 {
    piezoStruct->alias = alias;
    piezoStruct->currentState = PIEZO_INACTIVE;
    piezoStruct->MaxVelocity = 0x00;
    calculateSlopeIntercept();
 }

uint8_t piezoUpdate(piezo_t * piezo)
{
    float piezoRead = 0.0;                                              /**< Valor leido del transductor piezoeléctrico  */
    float piezoMax = 0.0;                                               /**< Máximo valor leido del transductor piezoeléctrico */

    piezoRead = piezo->alias->read();                                   //Tomo una lectura del transductor piezoeléctrico
    piezoRead = piezoRead * 3.3 * 1000;                                 //Convierto la lectura a [mV]
                                     
    if(piezoRead  > PIEZO_THRESHOLD_mV)                                 //Comparo la lectura en mV con el umbral de activación
    {
                                                    
        piezoMax = piezoSearchMax(piezo);                               //Busco el valor máximo del golpe detectado
        piezo->MaxVelocity = piezoConvertVoltToVel(piezoMax);           //Transformo el valor máximo en velocity
                 
        piezo->currentState = PIEZO_ACTIVE;                             //Actualizo el estado del piezoeléctrico a activo
        return piezo->currentState;                                     //Devuelvo el estado del transductor                  
    }
    piezo->currentState = PIEZO_INACTIVE;                               //Actualizo el estado del piezoeléctrico a inactivo
    return piezo->currentState;                                         //Devuelvo el estado del transductor 
}

//=====[Implementations of private functions]==================================

static void calculateSlopeIntercept()
{
    slope = (float)DELTA_VEL / DELTA_VOLT;                  /**< Pendiente de la recta de conversión */
    intercept = MIN_VEL - PIEZO_THRESHOLD_mV * slope;       /**< Ordenada al origen de la recta de conversión */ 
}

static uint8_t piezoConvertVoltToVel (float piezoMaxValue)
{
    uint8_t vel = 0;                                    /**< Valor entero de velocity de la nota midi */
    float velFloat = 0.0;                               /**< Valor flotante de velocity luego de la conversión */

    velFloat = piezoMaxValue * slope + intercept;        //Calculo el valor de velocity correspondiente al valor de voltaje [mV] registrado por el transductor piezoeléctrico
    
    vel = (uint8_t)roundf(velFloat);                    //Convierto y redondeo el valor para obtener un valor de velocity entre 0 y 127 
    if (vel > MAX_VEL) vel = MAX_VEL;                   
    if (vel < MIN_VEL) vel = MIN_VEL;

    return vel;                                         //Devuelvo el valor de velocity
}   

static float piezoSearchMax (piezo_t * piezo)
{
    float piezoMaxValue = 0.0;                          /**< Valor máximo del golpe registrado por el transductor piezoeléctrico*/
    float piezoSample = 0.0;                            /**< Valor muestreado del transductor piezoeléctrico */

    for(int sample = 0; sample < NUMBER_OF_PIEZO_SAMPLES; sample++)    //Realizo un muestreo de la señal analógica proveniente del transductor piezoeléctrico
    {
         piezoSample = piezo->alias->read();            //Tomo una lectura del transductor piezoeléctrico     
         piezoSample = piezoSample * 3.3 * 1000;        //Convierto la lectura a [mV]

         if(piezoSample > piezoMaxValue)                //Verifico si el nuevo valor leido es mayor al máximo valor registrado en este muestreo
        {
            piezoMaxValue = piezoSample;                //Actualizo el máximo valor registrado hasta el momento
        }
        wait_us(SAMPLE_TIME_INTERVAL_us);               //Genero el intervalo de tiempo entre muestras     
    }  
    return piezoMaxValue;                               //Devuelvo el máximo valor de voltaje [mV] leido del golpe ejecutado
}

/*** end of file ***/
