/** @file piezo.cpp
*
* @brief A description of the module’s purpose.
*
*/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "piezo.h"
#include "hi_hat.h"
#include "fixedpoint.h"
#include <cstdint>

//=====[Declaration of private defines]========================================

#define OPEN_THRESHOLD_mV 2100                                       /**< Umbral de voltaje para considerar que el hi hat esá abierto [mv] */                            
#define CLOSE_THRESHOLD_mV 220                                      

#define ADC_MAX_VALUE 65535                                         /**< valor máximo que devuelve read_u16() */
#define ADC_VOLTAGE_SCALE 3300                                      /**< Valor de voltaje máximo [mV] que corresponde al valor máximo que devuelve read_u16() (65535) */

//=====[Declaration of public classes]=====================================
hiHat::hiHat(PinName hiHatADPin, PinName hiHatIntPin, piezoTransducer * hiHatPiezo)
    : hiHatAD(hiHatADPin),       
      hiHatInterruptPin(hiHatIntPin)
      //Acá no cargo ninguna cosa del hiHatPiezo

{
    hiHatInterruptPin.fall(callback(this, &hiHat::hiHatIntCallback));
}

void hiHat::hiHatInit()
{ 
    hiHatStatus = OPEN;
    hiHatAperture = ADC_MAX_VALUE;
}

HI_HAT_STATE hiHat::gethiHatStatus()
{
    return hiHatStatus;
}

void hiHat::hiHatIntCallback()
{
    //mandar el comando midi de hi hat close
}


//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============
   

//=====[Declarations (prototypes) of private functions]========================

HI_HAT_STATE hiHat::hiHatGetAperture()
{
    hiHatAperture = hiHatAD.read_u16();
    uint16_t hiHatAperturemV = adcToMilliVolts(hiHatAperture);

    if (hiHatAperturemV >= OPEN_THRESHOLD_mV)
    {
        hiHatStatus = OPEN;
    }
    if(hiHatAperturemV > CLOSE_THRESHOLD_mV && hiHatAperturemV < OPEN_THRESHOLD_mV)
    {
        hiHatStatus = HALF_OPEN;
    }
    if(hiHatAperturemV <= CLOSE_THRESHOLD_mV)
    {
        hiHatStatus = CLOSE;
    }
    return hiHatStatus;
}

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================


/*** end of file ***/
