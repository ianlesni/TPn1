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
#define PIEZO_THRESHOLD_mV 120                                       /**< Umbral de voltaje para la detección del golpe [mv] *///Nivel por encima del piso de ruido
#define DELTA_VOLT (PIEZO_MAX_PEAK_VOLT_mV - PIEZO_THRESHOLD_mV)    /**< Variación entre el valor máximo y mínimo de voltaje registrado por el transductor piezoelectrico*/

#define NUMBER_OF_PIEZO_SAMPLES 20                                  /**< Número de total de muestras para el proceso de muestreo de la señal adquirida por el transductor piezoeléctrico */
#define SAMPLE_FREQ_Hz 2000                                         /**< Frecuencia de muestreo [Hz] */
#define SAMPLE_TIME_INTERVAL_us 500                                 /**< Intervalo de muestreo [us] */

#define ADC_MAX_VALUE 65535                                         /**< valor máximo que devuelve read_u16() */
#define ADC_VOLTAGE_SCALE 3300                                      /**< Valor de voltaje máximo [mV] que corresponde al valor máximo que devuelve read_u16() (65535) */

#define SPURIOUS_PEAK_DURATION_US 400                               /**< Duración típica [us] de los picos espurios producto de las propagaciones del golpe sobre el pad  */
#define PIEZO_SAMPLING_DURATION_US 2000                             /**< Duración [us] del muestreo del pico de interes  */

//=====[Declaration of public classes]=====================================
piezoTransducer::piezoTransducer(PinName piezoADPin, PinName piezoIntPin, Ticker* ticker)
    : piezoAD(piezoADPin),       
      piezoInterruptPin(piezoIntPin),
      piezoConvertionTicker(ticker) 
{
    piezoInterruptPin.fall(callback(this, &piezoTransducer::piezoIntCallback));
}

int32_t slopeFixedPoint;            /**< Pendiente de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity */
int32_t interceptFixedPoint;        /**< Ordenada al origen de la recta de conversión de voltaje [mV] del transductor piezoeléctrico a velocity  */

static void calculateSlopeIntercept()
{
    slopeFixedPoint = TO_FIXED_POINT((float)DELTA_VEL / DELTA_VOLT);                            /**< Pendiente de la recta de conversión */
    interceptFixedPoint = TO_FIXED_POINT(MIN_VEL) - PIEZO_THRESHOLD_mV * slopeFixedPoint;       /**< Ordenada al origen de la recta de conversión */ 
}

void piezoTransducer::piezoTransducerInit() 
{
    calculateSlopeIntercept(); 
    piezoMaxSampleValue = 0;
    piezoMaxVelocity = 0;
    elapsedADConvertionTime = 0;
    piezoStatus = PIEZO_IDLE;
}

void piezoTransducer::piezoTransducerReset() 
{
    elapsedADConvertionTime = 0;
    piezoConvertionTicker->detach();
}

PIEZO_STATE piezoTransducer::getPiezoStatus()
{
    return piezoStatus;
}

void piezoTransducer::piezoIntCallback()
{
    piezoConvertionTicker->attach(callback(this,&piezoTransducer::piezoReadAndGetMax),100us);
}

void piezoTransducer::piezoReadAndGetMax()
{
    uint16_t piezoSample = piezoAD.read_u16();

    if(piezoSample > piezoMaxSampleValue)
    {
        piezoMaxSampleValue = piezoSample;
    }

    elapsedADConvertionTime++;

    if(4 == elapsedADConvertionTime && 1 == piezoInterruptPin.read())
    {
        piezoMaxSampleValue = 0;
        piezoTransducerReset();
        piezoStatus = PIEZO_IDLE;
    }
    if(20 == elapsedADConvertionTime)
    {
        piezoTransducerReset();
        piezoStatus = PIEZO_FINISHED; 
    }
}
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============
   

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
 * @return  Valor de velocity correspondiente ajustado dentro del rango permitido(0-127).
 */
 uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue);

/**
 * Busqueda y devolución del valor máximo del golpe registrado por el transductor piezoeléctrico.
 * 
 * Esta función realiza un muestreo de la señal analógica proveniente del transductor piezoeléctrico
 * y determina el valor máximo de voltaje [mV] registrado durante el proceso de muestreo.
 * 
 * @param piezo Puntero a la estructura que representa el un transductor piezoeléctrico.
 * @return Valor máximo de voltaje [mV] registrado durante el muestreo.
 */
 uint16_t piezoSearchMax (piezo_t * piezo);

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================
uint16_t adcToMilliVolts(uint16_t adcValue) {
    return ((adcValue * ADC_VOLTAGE_SCALE) / ADC_MAX_VALUE);
}

 uint8_t piezoConvertVoltToVel (uint16_t piezoMaxValue)
{
    uint8_t vel = 0;                                                        /**< Valor entero de velocity de la nota midi */
                          
    int32_t velFixedPoint;                                                  /**< Valor flotante de velocity luego de la conversión */
    velFixedPoint = piezoMaxValue * slopeFixedPoint + interceptFixedPoint;  //Calculo el valor de velocity correspondiente al valor de voltaje [mV] registrado por el transductor piezoeléctrico
           
    vel = FROM_FIXED_POINT(velFixedPoint);                                  //Convierto y redondeo el valor para obtener un valor de velocity entre 0 y 127 
                
    if (vel > MAX_VEL) vel = MAX_VEL;                   
    if (vel < MIN_VEL) vel = MIN_VEL;

    return vel;                                                             //Devuelvo el valor de velocity
}   

/*** end of file ***/
