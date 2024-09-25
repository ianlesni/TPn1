/** @file bt.h
*
* @brief Modulo para implementar la comunicacion Bluetooth
*
*/
#ifndef _BT_H_
#define _BT_H_
#include <cstdint>


namespace mbed {
    class UnbufferedSerial;
}

/** Seteo las propiedades de la comuniación serie 
*  acorde a las preferencias configuradas en el 
*  software Hariless MIDI<->Serial Bridge
*  (38400-8-N-1) 
*  @param alias Puntero al objeto responsable de la comunicación serie.
*/
void initializateBlePort(mbed::UnbufferedSerial * alias);

void BLESetATConfig (mbed::UnbufferedSerial * alias);


#endif // _BT_H_

/*** end of file ***/