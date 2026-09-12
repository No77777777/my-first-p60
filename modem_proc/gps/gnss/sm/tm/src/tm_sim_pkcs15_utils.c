/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   TM SIM PKCS15 UTILS module .


GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2015 -2022 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_sim_pkcs15_utils.c#3 $
  $DateTime: 2022/10/27 22:07:40 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  10/24/21   jv     Fixing double free bug
  03/09/18   py     Remove "FEATURE_MMGSDI_SESSION_LIB" dependency. True always.
===========================================================================*/
#include "customer.h"   /* Customer configuration file */
#include "gps_variation.h"

#include "tm_sim_pkcs15_utils.h"
#include "msg.h"

#if defined(FEATURE_CGPS_UMTS_UP_SUPL)

#if defined(FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM)
#error code not present
#else /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

/*===========================================================================
FUNCTION tm_sim_mmgsdi_pkcs15_get_supl_slp_address

DESCRIPTION

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
)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI Lib not available or reading HSLP and certificates from USIM not set", 0, 0, 0);
  return FALSE;
}

/*===========================================================================
FUNCTION tm_sim_mmgsdi_pkcs15_init

DESCRIPTION

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void tm_sim_mmgsdi_pkcs15_init(void)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI Lib not available or reading HSLP and certificates from USIM not set", 0, 0, 0);
  return;
}

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
)
{
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI Lib not available or reading HSLP and certificates from USIM not set", 0, 0, 0);
    return;
}

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
  const os_TimerExpiryType *p_timer_data
)
{
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI Lib not available or reading HSLP and certificates from USIM not set", 0, 0, 0);
    return;
}

#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM*/
#endif /* FEATURE_CGPS_UMTS_UP_SUPL */
