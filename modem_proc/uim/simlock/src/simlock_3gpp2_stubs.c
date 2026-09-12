/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M   L O C K   3 G P P 2   S T U B S   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock 3GPP2 stub functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_3gpp2_stubs.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/19/17   vdc     Initial revision
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_common.h"
#include "simlock_config.h"
#include "simlock_3gpp2.h"

/*===========================================================================
FUNCTION SIMLOCK_3GPP2_RUN_ALGORITHM

DESCRIPTION
  This function validates the 3GPP2 subscription on a card by verifying the
  card data against the lock data. It checks the codes for each enabled
  category for validating the subscription. Also, it checks if a code is
  categorized as a blacklist or whitelist and accordingly updates the
  validity of the subscription.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_3gpp2_run_algorithm
(
  const simlock_subscription_data_type    * sim_data_ptr,
  simlock_slot_enum_type                    slot,
  const simlock_config_data_type          * config_data_ptr,
  simlock_subscription_validity_info_type * subs_validity_ptr
)
{
  (void)sim_data_ptr;
  (void)slot;
  (void)config_data_ptr;

  if(subs_validity_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* SimLock engine for 3GPP2 is disabled, so mark the subscription as valid */
  subs_validity_ptr->is_subscription_valid = TRUE;

  return SIMLOCK_SUCCESS;
} /* simlock_3gpp2_run_algorithm */

#endif /* FEATURE_SIMLOCK */
