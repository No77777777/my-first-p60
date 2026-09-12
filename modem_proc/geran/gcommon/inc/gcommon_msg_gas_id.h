#ifndef GCOMMON_MSG_GAS_ID_H
#define GCOMMON_MSG_GAS_ID_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               G A S _ I D   I N   M S G S   G E T / S E T

GENERAL DESCRIPTION
   This module provides functions to get the gas_id out of internal messages
   or set them in messages.

EXTERNALIZED FUNCTIONS
  gcommon_msg_l2_gas_id_get / set

INITIALIZATION AND SEQUENCING REQUIREMENTS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014 by QUALCOMM Technologies, Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gcommon/inc/gcommon_msg_gas_id.h#1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
2014-10-23 tjw      New file

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "geran_dual_sim.h"

/*===========================================================================

FUNCTION  gcommon_msg_gas_id_get / set

DESCRIPTION
  Get or set the gas_id in a msg

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
gas_id_t gcommon_msg_gas_id_get(const void *msg);
void gcommon_msg_gas_id_set(void *msg, const gas_id_t gas_id);

#endif