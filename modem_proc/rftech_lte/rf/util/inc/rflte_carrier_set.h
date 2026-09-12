/*!
  @file
  rflte_carrier_set.h

  @brief
  Set abstraction for carriers.
 
  @details
  This header file provdes the C99 inlined definitions for the functions
  provided by this module.  To be C99 compliant, a non-inlined version
  of the function must be present as well, and this is provided by the
  companion rflte_carrier_set.c file.
*/
/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/


/*===========================================================================


                EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/rf/util/inc/rflte_carrier_set.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
08/07/15   bmg     Initial version

===========================================================================*/

#ifndef RFLTE_CARRIER_SET_H
#define RFLTE_CARRIER_SET_H

#include "comdef.h"
#include "err.h"
#if __hexagon__
#include <hexagon_protos.h>
#endif /* __hexagon__ */

typedef uint64 carrier_set;

inline void
rflte_carrier_set_init(
  carrier_set* set
)
{
  *set = 0;
}

inline void
rflte_carrier_set_clear(
  carrier_set* set
)
{
  *set = 0;
}

inline void
rflte_carrier_set_add(
  carrier_set* set,
  int carrier
)
{
  if (carrier < 64)
  {
    *set |= (1ULL << carrier);
  }
  else
  {
    ERR_FATAL("Carrier too large: %d", carrier, 0, 0);
  }
}

inline int
rflte_carrier_set_size(
  carrier_set* set
)
{
#if __hexagon__
  return Q6_R_popcount_P(*set);
#else
  int i;
  int size = 0;
  for (i = 0; i < 8*sizeof(carrier_set); i++)
  {
    if (*set & (1ULL << i))
    {
      size += 1;
    }
  }
  return size;
#endif /* __hexagon__ */
}

#endif /* RFLTE_CARRIER_SET_H */

