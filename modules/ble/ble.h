/** @file ble.h
*
* @brief A description of the module’s purpose.
*
* 
* 
*/

//=====[#include guards - begin]===============================================

#ifndef _BLE_H_
#define _BLE_H_
#include <cstdint>

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================
namespace mbed {
    class UnbufferedSerial;
}

//=====[Declarations (prototypes) of public functions]=========================

/** Seteo las propiedades de la comuniación serie 
*  acorde a las preferencias configuradas en el 
*  software Hariless MIDI<->Serial Bridge
*  (9600-8-N-1) 
*  @param alias Puntero al objeto responsable de la comunicación serie.
*/
void initializateBlePort(mbed::UnbufferedSerial * alias);

void BLESetATConfig (mbed::UnbufferedSerial * alias);

//=====[#include guards - end]=================================================

#endif // _BLE_H_

/*** end of file ***/