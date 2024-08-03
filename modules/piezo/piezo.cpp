/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "piezo.h"
#include "fixedpoint.h"
#include <cstdint>

//=====[Declaration of private defines]========================================

#define MAX_VEL 127                                                 /**< Máximo valor de velocity permitido */
#define MIN_VEL 35                                                  /**< Máximo valor de velocity permitido (para valores menores se escucha muy poco) */
#define DELTA_VEL (MAX_VEL - MIN_VEL)                               /**< Variacion entre el máximo y mínimo valor de velocity permitido*/
#define PIEZO_MAX_PEAK_VOLT_mV 2000                                 /**< Máximo valór de voltaje pico [mV] generado por el transductor piezoelectrico para el circuito de adqiusición actual*/
#define PIEZO_THRESHOLD_mV 90                                       /**< Umbral de voltaje para la detección del golpe [mv] *///Nivel por encima del piso de ruido
#define DELTA_VOLT (PIEZO_MAX_PEAK_VOLT_mV - PIEZO_THRESHOLD_mV)    /**< Variación entre el valor máximo y mínimo de voltaje registrado por el transductor piezoelectrico*/

#define NUMBER_OF_PIEZO_SAMPLES 20                                  /**< Número de total de muestras para el proceso de muestreo de la señal adquirida por el transductor piezoeléctrico */
#define SAMPLE_FREQ_Hz 2000                                         /**< Frecuencia de muestreo [Hz] */
#define SAMPLE_TIME_INTERVAL_us 500                                 /**< Intervalo de muestreo [us] */


#define ADC_MAX_VALUE 65535                                         /**< valor máximo que devuelve read_u16() */
#define ADC_VOLTAGE_SCALE 3300                                      /**< Valor de voltaje máximo [mV] que corresponde al valor máximo que devuelve read_u16() (65535) */

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

    
int32_t slopeFixedPoint;            /**< Pendiente de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity */
int32_t interceptFixedPoint;        /**< Ordenada al origen de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity  */

//=====[Declarations (prototypes) of private functions]========================

 /**
 * Conviersión de un valor de lectura 
 * 
 * Esta función realiza la conversión del valor de lectura del ADC 
 * obtenido en uint16_t a [mV]
 * 
 * 
 * @param adcValue Valor de adc (uint16_t producto de read_u16)
 * @return  Valor de ADC en [mV].
 */
uint16_t adcToMilliVolts (uint16_t adcValue);
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
 * @return  Valor de velocity correspondiente ajustado dentro del rango permitido(0-127).
 */
static uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue);

/**
 * Busqueda y devolución del valor máximo del golpe registrado por el transductor piezoeléctrico.
 * 
 * Esta función realiza un muestreo de la señal analógica proveniente del transductor piezoeléctrico
 * y determina el valor máximo de voltaje [mV] registrado durante el proceso de muestreo.
 * 
 * @param piezo Puntero a la estructura que representa el un transductor piezoeléctrico.
 * @return Valor máximo de voltaje [mV] registrado durante el muestreo.
 */
static uint16_t piezoSearchMax (piezo_t * piezo);

//=====[Implementations of public functions]===================================
 void piezoInit(mbed::AnalogIn * alias, piezo_t * piezo)
 {
    piezo->alias = alias;                                               
    piezo->currentState = PIEZO_INACTIVE;                               
    piezo->MaxVelocity = 0x00;                                          
    calculateSlopeIntercept();                  
 }

uint8_t piezoUpdate(piezo_t * piezo)
{
    uint16_t piezoRead = 0;                                             /**< Valor leido del transductor piezoeléctrico  */
    uint16_t piezoMax = 0;                                              /**< Máximo valor leido del transductor piezoeléctrico */

    piezoRead = piezo->alias->read_u16();                               //Tomo una lectura del transductor piezoeléctrico
    piezoRead = adcToMilliVolts(piezoRead);                             //Convierto la lectura a [mV]
                                     
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
uint16_t adcToMilliVolts(uint16_t adcValue) {
    return ((adcValue * ADC_VOLTAGE_SCALE) / ADC_MAX_VALUE);
}

static void calculateSlopeIntercept()
{
    slopeFixedPoint = TO_FIXED_POINT((float)DELTA_VEL / DELTA_VOLT);                            /**< Pendiente de la recta de conversión */
    interceptFixedPoint = TO_FIXED_POINT(MIN_VEL) - PIEZO_THRESHOLD_mV * slopeFixedPoint;       /**< Ordenada al origen de la recta de conversión */ 
}

static uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue)
{
    uint8_t vel = 0;                                                        /**< Valor entero de velocity de la nota midi */
                          
    int32_t velFixedPoint;                                                  /**< Valor flotante de velocity luego de la conversión */
    velFixedPoint = piezoMaxValue * slopeFixedPoint + interceptFixedPoint;  //Calculo el valor de velocity correspondiente al valor de voltaje [mV] registrado por el transductor piezoeléctrico
           
    vel = FROM_FIXED_POINT(velFixedPoint);                                  //Convierto y redondeo el valor para obtener un valor de velocity entre 0 y 127 
                
    if (vel > MAX_VEL) vel = MAX_VEL;                   
    if (vel < MIN_VEL) vel = MIN_VEL;

    return vel;                                                             //Devuelvo el valor de velocity
}   

static uint16_t piezoSearchMax (piezo_t * piezo)
{
    uint16_t piezoMaxValue = 0;                                         /**< Valor máximo del golpe registrado por el transductor piezoeléctrico*/
    uint16_t piezoSample = 0;                                           /**< Valor muestreado del transductor piezoeléctrico */

    for(int sample = 0; sample < NUMBER_OF_PIEZO_SAMPLES; sample++)     //Realizo un muestreo de la señal analógica proveniente del transductor piezoeléctrico
    {
         piezoSample = piezo->alias->read_u16();                        //Tomo una lectura del transductor piezoeléctrico     
         piezoSample = adcToMilliVolts(piezoSample);                    //Convierto la lectura a [mV]

         if(piezoSample > piezoMaxValue)                                //Verifico si el nuevo valor leido es mayor al máximo valor registrado en este muestreo
        {
            piezoMaxValue = piezoSample;                                //Actualizo el máximo valor registrado hasta el momento
        }
        wait_us(SAMPLE_TIME_INTERVAL_us);                               //Genero el intervalo de tiempo entre muestras     
    }  
    return piezoMaxValue;                                               //Devuelvo el máximo valor de voltaje [mV] leido del golpe ejecutado
}

/*** end of file ***/
