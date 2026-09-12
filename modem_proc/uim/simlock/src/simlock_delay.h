#ifndef SIMLOCK_DELAY_H
#define SIMLOCK_DELAY_H
/*===========================================================================


            S I M L O C K   D E L A Y   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_delay.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
05/08/18   nr      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"

#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock_common.h"

/*===========================================================================
FUNCTION SIMLOCK_DELAY_DELETE_DELAY_TIMER

DESCRIPTION
  This function is to delete delay timer. This gets called upon receiving card error event from MMGSDI or
  upon receiving blob from unlock server.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_delay_delete_delay_timer
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_DELAY_INITIALIZE_DELAY_TIMER

DESCRIPTION
  Creates/starts the delay timer.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_delay_initialize_delay_timer
(
  uint16    *delay_timer_value_in_sec_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_DELAY_IS_DELAY_TIMER_RUNNING

DESCRIPTION
  Check if simlock delay timer is running or not.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_delay_is_delay_timer_running
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_DELAY_STOP_DELAY_TIMER

DESCRIPTION
  This function takes care of stopping delay timer.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_delay_stop_delay_timer
(
  void
);

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */

#endif /* SIMLOCK_DELAY_H */

