#ifndef HAL_COMDEF_H
#define HAL_COMDEF_H
/*
==============================================================================

FILE:         HALcomdef.h

DESCRIPTION:  
  Hardware IO abstraction APIs.

$Header: //components/rel/dataipa.mpss/3.2/emu/IPA/platform/windows/common/HALcomdef.h#1 $

==============================================================================

==============================================================================
             Copyright @ 2012 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/


/*============================================================================

                     INCLUDE FILES FOR MODULE

============================================================================*/


/*
 * Assembly wrapper
 */
#ifndef _ARM_ASM_

/* 
 * C++ wrapper
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"


/*============================================================================

             DEFINITIONS AND DECLARATIONS FOR MODULE

=============================================================================*/

/*
 * Macro to allow forcing an enum to 32 bits.  The argument should be
 * an identifier in the namespace of the enumeration in question, i.e.
 * for the clk HAL we might use HAL_ENUM_32BITS(CLK_xxx).
 */
#define HAL_ENUM_32BITS(x) HAL_##x##_FORCE32BITS = 0x7FFFFFFF

#ifdef __cplusplus
}
#endif

#endif /* !_ARM_ASM_ */

#endif /* HAL_COMDEF_H */

