/** @file contrl_interface.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/
//=====[#include guards - begin]===============================================

#ifndef _CONTROL_INTERFACE_H_
#define _CONTROL_INTERFACE_H_

#include <cstdint>
#include "piezo.h"
#include "mbed.h"

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================
void updateDisplay(void);
void handleMenuNavigation(void);
void confirmButtonPressed(void);
void confirmSelection(void);
void returnToPreviousMenu(void); 
//=====[#include guards - end]=================================================

#endif // _CONTROL_INTERFACE_H_

/*** end of file ***/