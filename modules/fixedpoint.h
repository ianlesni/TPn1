/** @file fixedpoint.h
*
* @brief A description of the module’s purpose.
*
*/
//=====[#include guards - begin]===============================================

#ifndef _FIXEDPOINT_H_
#define _FIXEDPOINT_H_

#define FIXED_POINT_FRACTIONAL_BITS 12                              /**< Número de bits fraccionales */
#define FIXED_POINT_SCALE (1 << FIXED_POINT_FRACTIONAL_BITS)        /**< Factor de escala para la conversión */
#define TO_FIXED_POINT(x) ((x) * FIXED_POINT_SCALE)                 /**< Macro para convertir un número en su representacón de punto fijo */
#define FROM_FIXED_POINT(x) ((x) / FIXED_POINT_SCALE)               /**< Macro para convertir un numero de punto fijo a su representación normal */

//=====[#include guards - end]=================================================

#endif // _FIXEDPOINT_H_

/*** end of file ***/