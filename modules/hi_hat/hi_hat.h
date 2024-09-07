/** @file piezo.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/
//=====[#include guards - begin]===============================================

#ifndef _HI_HAT_H_
#define _HI_HAT_H_

#include <cstdint>
#include "piezo.h"
#include "mbed.h"

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

/*!
 * \enum HI_HAT_STATE
 * 
 */
typedef enum{
    
    OPEN = 0,     
    HALF_OPEN = 1,     
    CLOSE =2

}HI_HAT_STATE; 



class hiHat{
    public:
        hiHat(PinName hiHatAD, PinName hiHatIntPin);   
        void hiHatInit();
        HI_HAT_STATE gethiHatStatus(); 
        HI_HAT_STATE hiHatGetAperture();   
        uint16_t hiHatAperture;
    private:
        void hiHatIntCallback();
        AnalogIn hiHatAD;               
        InterruptIn hiHatInterruptPin;
        HI_HAT_STATE hiHatStatus;
};


//=====[#include guards - end]=================================================

#endif // _HI_HAT_H_

/*** end of file ***/