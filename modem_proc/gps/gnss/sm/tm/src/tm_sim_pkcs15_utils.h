#ifndef TM_SIM_PKCS15_UTILS_H
#define TM_SIM_PKCS15_UTILS_H


/*===========================================================================

                            TM_SIM_PKCS15_UTILS  h e a d e r

DESCRIPTION


Copyright (c) 2015 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_sim_pkcs15_utils.h#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
===========================================================================*/

#include "comdef.h"
#include "pd_comms_api.h"
#include "aries_os_api.h"


/*===========================================================================
FUNCTION tm_sim_mmgsdi_pkcs15_init

DESCRIPTION
  Initialize the PKCS15 utils Module

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tm_sim_mmgsdi_pkcs15_init(void);

/*===========================================================================
FUNCTION tm_sim_mmgsdi_pkcs15_get_supl_slp_address

DESCRIPTION
  Return the SLP address read from the SIM card.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  : If SLP address being returned
  FALSE : If SLP address is not valid


SIDE EFFECTS
  None
===========================================================================*/
boolean tm_sim_mmgsdi_pkcs15_get_supl_slp_address
(
  pd_comms_tcp_connect_params_type  *p_connect_params
);

/*===========================================================================
FUNCTION tm_sim_mmgsdi_pkcs15_handle_timer_expiry

DESCRIPTION
  Handle a timer expiry

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tm_sim_mmgsdi_pkcs15_handle_timer_expiry
(
  const os_TimerExpiryType *p_timerData
);

/*===========================================================================
FUNCTION tm_sim_mmgsdi_process_pkcs15_evt

DESCRIPTION
  Process events for the PKCS15 state machine. We process the events:
      TM_MMGSDI_CARD_INSERTED
      TM_MMGSDI_CARD_REMOVED
      TM_MMGSDI_SESSION_OPEN_SUCCESS
      TM_MMGSDI_SESSION_READ_SUCCESS

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void tm_sim_mmgsdi_process_pkcs15_evt
(
  void* p_data
);

#endif
