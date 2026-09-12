#ifndef PS_INET_MEM_POOL_H
#define PS_INET_MEM_POOL_H
/*===========================================================================

          I N E T   M E M O R Y  P O O L  H E A D E R  F I L E

DESCRIPTION
  The inet module memory pool management header file.

EXTERNAL FUNCTIONS
  PS_INET_MEM_POOL_INIT()
    Initialized memory for inet module

Copyright (c) 2007 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_inet_mem_pool.h#2 $
  $DateTime: 2019/10/22 02:53:17 $

===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"


#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_INET_MEM_POOL_INIT()

DESCRIPTION
  Initializes inet memories.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_inet_mem_pool_init
(
  void
);

#ifdef __cplusplus
}
#endif
#endif /* PS_INET_MEM_POOL_H */
