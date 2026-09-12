 /***********************************************************************
 * osal_rex_micro.c
 * Description: This file contains the implementation of functions that will be
 * used in both normal mode and island mode.
 *
 * Copyright (C) 2015 QUALCOMM Technologies, Inc.
 *
 *
 ***********************************************************************/

/*===========================================================================

                         EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/services/osal/rex/src/osal_rex_micro.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   --------------------------------------------------------- 
08/13/15     xy    Created

===========================================================================*/


#include "osal.h"
#include "msg.h"  /* For Diag messages */
#if defined(DIAG_DOG_SUPPORT)
#include "dog_hb_rex.h"
#endif



#ifdef DIAG_REX_QDSP_EMULATION
int osal_atomic_compare_and_set(osal_atomic_word_t *target,
                       osal_atomic_plain_word_t old_val,
                       osal_atomic_plain_word_t new_val )
{
   return atomic_compare_and_set(target,old_val,new_val);
 
}	
#else
int osal_atomic_compare_and_set(osal_atomic_word_t *target,
                       osal_atomic_plain_word_t old_val,
                       osal_atomic_plain_word_t new_val )
{
    int ret_value = 1;

	INTLOCK();

	if (*target == old_val) {
		 *target = new_val;  
	}
	else {
        ret_value = 0;
	} 
	INTFREE();

	return ret_value;
}
#endif