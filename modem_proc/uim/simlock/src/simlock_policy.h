#ifndef SIMLOCK_POLICY_H
#define SIMLOCK_POLICY_H
/*===========================================================================


            S I M L O C K   P O L I C Y  H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_policy.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/26/17   bcho    Support for slot policy valid card must on all slots
11/03/16   bcho    Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_common.h"
#include "simlock_config.h"
#include "simlock_platform.h"

/*===========================================================================

                        FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_APPLY_CONFIG_POLICY

DESCRIPTION
  This function updates the validity of a subscription on a card, based on
  the slot and the subscription policy set in the SIMLock feature.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_apply_config_policy
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_config_data_type                   * config_data_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_POLICY_USES_SLOT1_CONFIG_FOR_ALL_SLOTS

DESCRIPTION
  This function checks whether slot policy uses slot1 configuration also for
  all additional slots.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE   : If policy uses slot1 config
  FALSE  : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_policy_uses_slot1_config_for_all_slots
(
  simlock_slot_policy_enum_type slot_policy
);

#endif /* FEATURE_SIMLOCK */

#endif /* SIMLOCK_POLICY_H */

