/*===========================================================================

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2009 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdiag/src/uimdiag.c#3 $$ $DateTime: 2020/08/04 23:12:22 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   -----------------------------------------------------------
07/23/20   cj    Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD
09/10/19   vdc   SimLock solution for activation at Point Of Sale
12/03/18   vgd   Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
07/13/17   nr    Remote SIM Unlock supported in common build
02/20/17   vdc   Added support to probe CK for correctness
12/29/16   dt    Support for QXDM sim data collection logs
12/02/16   vdc   Added support to check RSA public key for remote lock
05/20/16   vdc   Remove F3 messages for memory allocation failure
03/11/16   ar    Remove FEATURE_UIMDIAG
11/02/15   stv   Rel 12 refresh enforcement policy support
07/10/15   stv   Deprecate UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_CMD
05/15/15   yt    Support for mapping sessions to slots
04/25/15   av    Support for hiding CSIM from modem clients
02/25/15   yt    Reduction of UIMDIAG functionality
02/20/15   vv    Remove perso related code
12/26/14   vv    Support for whitelist and blacklist codes in same category
07/10/14   tl    Remove ALWAYS OFF feature FEATURE_MMGSDI_PKCS15
06/12/14   vv    Remove FEATURE_SIMLOCK
03/18/14   tl    Introduce new SIM Lock feature
01/27/14   df    Featurise file with FEATURE_UIMDIAG
01/10/14   df    Use v2.0 diag macros
12/11/13   df    Remove unused memory paging feature
11/05/13   df    Remove unnecessary include tmc.h
05/06/13   sw    Changes made for secure version of memcpy
04/24/13   rr    Featurized optional logs
11/09/12   yt    Remove parsing of length field for PERSO_GET_DATA_CMD
09/25/12   tl    Adding nvruim_data_req_enum_type to sessionid_type
11/17/11   kk    Added get all available apps async support
10/24/11   adp   Removing support for UIMDIAG_MMGSDI_SAP_PIN_VERIFY_CMD,
                 UIMDIAG_MMGSDI_SAP_INITIALIZE_CMD and
                 UIMDIAG_MMGSDI_SAP_DEREGISTER_CMD
10/18/11   bcho  Adding support for mmgsdi_session_open_ext API
10/04/11   adp   Adding support for is service available sync cmd
09/29/11   kk    Added support for get all available apps sync cmd
10/14/11   bcho  Added support for Get Operator Name API
09/23/11   nb    Support for SAP CONNECT/SAP DISCONNECT
08/05/11   adp   Adding support to trigger refresh in uimdiag
09/15/11   nb    Support for new refresh stage
08/22/11   av    Added support for pdown,pup,activate onchip,send apdu commands
04/26/11   yt    Replaced references to TMC heap with Modem heap
12/29/10   ms    Adding support for BT SAP APIs
12/03/10   adp   Adding support for a number of commands
12/22/10   yt    Removed FEATURE_UIM_ZI_MEMORY_REDUCTION
11/30/10   yt    Including private UIMDRV header
09/07/10   adp   Adding Support for dck_num_retries,lock_down_enable_ltd_access,
                 seek and search commands
08/09/10   adp   Adding support for service enable, disable, get perso key,
                 is service available, perso unblock.
06/09/10   adp   Fixing Compiler Warnings
04/07/10   yt    Fixed compilation warnings
03/25/10   adp   Adding support for mmgsdi_session_deactivate_provisioning,
                 mmgsdi_session_get_app_capabilities,get_cphs_info
                 isim_authenticate,send_card_status,
                 create_pkcs15_lookup_table
03/24/10   adp   Fixing Compiler Warnings, the function call to check whether
                 the pointer is in the queue or not is not needed for uimdiag
03/19/10   rk    Added support for JCDMA get info
02/22/10   adp   Adding support for Perso Commands
02/04/10   yt    Fixed compilation warnings
12/27/09   ap    Adding support for Refresh Commands
12/17/09   rk    Fix for uimdiag small heap memory alloction issue
12/03/09   rk    Adding support for get file attributes, read and write commands
10/19/09   adp   Adding support for pin commands
10/18/09   adp   Moving MMGSDI specific util functions to uimdiag_mmgsdi_util.c
                 Adding support for Activate_or_switch_prov, session_get_info
                 Adding functionality for uimdiag_free
09/02/09   adp   Initial Revision -Added initial mmgsdi session based commands


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "diagcmd.h"
#include "uimdiag.h"
#include "uimdiag_mmgsdi.h"
#include "uimdiag_util.h"
#include "diagpkt.h"

/*=============================================================================

                       GLOBAL DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   UIMDIAG_SUBSYS_DELAYED_TBL

   DESCRITPION:
     Table contains uimdiag subsystem command ids and corresponding
     call back functions. This table is used during uimdiag initialization
     for registering

   ------------------------------------------------------------------------- */
static const diagpkt_user_table_entry_type uimdiag_subsys_delayed_tbl[] =
{
  {(word)UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD,
   (word)UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD,
   uimdiag_mmgsdi_client_id_and_evt_reg_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_OPEN_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_OPEN_CMD,
   uimdiag_mmgsdi_session_open_cmd },
  {(word)UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD,
   (word)UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD,
   uimdiag_mmgsdi_client_id_and_evt_dereg_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_CLOSE_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_CLOSE_CMD,
   uimdiag_mmgsdi_session_close_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD,
   uimdiag_mmgsdi_session_act_or_sw_prov_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD,
   uimdiag_mmgsdi_session_get_info_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD,
   (word) UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD,
   uimdiag_mmgsdi_session_verify_pin_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD,
   uimdiag_mmgsdi_session_get_file_attr_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD,
   uimdiag_mmgsdi_session_read_record_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_READ_FROM_CACHE_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_READ_FROM_CACHE_CMD,
   uimdiag_mmgsdi_session_read_from_cache_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD,
   uimdiag_mmgsdi_session_read_transparent_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD,
   uimdiag_mmgsdi_session_write_record_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD,
   uimdiag_mmgsdi_session_write_transparent_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD,
   uimdiag_mmgsdi_session_deact_prov_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD,
   uimdiag_mmgsdi_session_get_app_cap_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CMD,
   uimdiag_mmgsdi_session_get_info_sync_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_GET_APP_CAPABILITIES_SYNC_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_GET_APP_CAPABILITIES_SYNC_CMD,
   uimdiag_mmgsdi_session_get_app_capabilities_sync_cmd},
  {(word)UIMDIAG_MMGSDI_GET_ALL_AVAIL_APPS_SYNC_EXT_CMD ,
   (word)UIMDIAG_MMGSDI_GET_ALL_AVAIL_APPS_SYNC_EXT_CMD,
   uimdiag_mmgsdi_get_all_avail_apps_sync_ext_cmd},
  {(word)UIMDIAG_MMGSDI_CARD_PDOWN_CMD,
   (word)UIMDIAG_MMGSDI_CARD_PDOWN_CMD,
   uimdiag_mmgsdi_card_pdown_cmd},
  {(word)UIMDIAG_MMGSDI_CARD_PUP_CMD,
   (word)UIMDIAG_MMGSDI_CARD_PUP_CMD,
   uimdiag_mmgsdi_card_pup_cmd},
  {(word)UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD,
   (word)UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD,
   uimdiag_mmgsdi_activate_onchip_sim_cmd},
  {(word)UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD,
   (word)UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD,
   uimdiag_mmgsdi_activate_onchip_usim_cmd},
  {(word)UIMDIAG_MMGSDI_SEND_APDU_CMD,
   (word)UIMDIAG_MMGSDI_SEND_APDU_CMD,
   uimdiag_mmgsdi_send_apdu_cmd},
  {(word)UIMDIAG_MMGSDI_GET_ATR_CMD,
   (word)UIMDIAG_MMGSDI_GET_ATR_CMD,
   uimdiag_mmgsdi_get_atr_cmd },
  {(word)UIMDIAG_SIMLOCK_SET_POLICY_CMD,
   (word)UIMDIAG_SIMLOCK_SET_POLICY_CMD,
   uimdiag_simlock_set_policy_cmd },
  {(word)UIMDIAG_SIMLOCK_CK_LOCK_CMD,
   (word)UIMDIAG_SIMLOCK_CK_LOCK_CMD,
   uimdiag_simlock_ck_lock_cmd },
  {(word)UIMDIAG_SIMLOCK_HCK_LOCK_CMD,
   (word)UIMDIAG_SIMLOCK_HCK_LOCK_CMD,
   uimdiag_simlock_hck_lock_cmd },
  {(word)UIMDIAG_SIMLOCK_UNLOCK_CMD,
   (word)UIMDIAG_SIMLOCK_UNLOCK_CMD,
   uimdiag_simlock_unlock_cmd },
  {(word)UIMDIAG_SIMLOCK_GET_STATUS_CMD,
   (word)UIMDIAG_SIMLOCK_GET_STATUS_CMD,
   uimdiag_simlock_get_status_cmd },
  {(word)UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD,
   (word)UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD,
   uimdiag_simlock_add_lock_codes_ck_cmd },
  {(word)UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD,
   (word)UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD,
   uimdiag_simlock_add_lock_codes_hck_cmd },
  {(word)UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD,
   (word)UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD,
   uimdiag_simlock_get_category_data_ext_cmd },
   {(word)UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD,
   (word)UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD,
   uimdiag_simlock_remote_lock_check_public_key_cmd },
   {(word)UIMDIAG_SIMLOCK_PROBE_CK_CMD,
   (word)UIMDIAG_SIMLOCK_PROBE_CK_CMD,
   uimdiag_simlock_probe_ck_cmd },
   {(word)UIMDIAG_SIMLOCK_SET_RSU_MODE_CMD,
   (word)UIMDIAG_SIMLOCK_SET_RSU_MODE_CMD,
   uimdiag_simlock_mode_config_cmd },
   {(word)UIMDIAG_SIMLOCK_SET_RSU_KEY_CMD,
    (word)UIMDIAG_SIMLOCK_SET_RSU_KEY_CMD,
   uimdiag_simlock_device_key_config_cmd },
   {(word)UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD,
    (word)UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD,
   uimdiag_simlock_set_configuration_cmd },
   {(word)UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD,
   (word)UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD,
   uimdiag_simlock_get_rsu_mode_cmd },
   /* The following commands are only available in internal builds
      where this feature flag is defined. */
#ifdef FEATURE_UIMDIAG_INTERNAL_API
  {(word)UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD,
   (word)UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD,
   uimdiag_mmgsdi_session_reg_for_refresh_proceed_voting_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD,
   uimdiag_mmgsdi_session_ok_to_proceed_with_refresh_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD,
   uimdiag_mmgsdi_session_reg_for_refresh_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD,
   uimdiag_mmgsdi_session_dereg_for_refresh_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD,
   uimdiag_mmgsdi_session_ok_to_refresh_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD,
   uimdiag_mmgsdi_session_refresh_complete_cmd },
  {(word)UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CMD,
   (word)UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CMD,
   uimdiag_mmgsdi_get_all_available_apps_async_cmd},
  {(word) UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD,
   (word) UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD,
   uimdiag_mmgsdi_session_disable_pin_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD,
   (word) UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD,
   uimdiag_mmgsdi_session_enable_pin_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD,
   (word) UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD,
   uimdiag_mmgsdi_session_change_pin_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD,
   (word) UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD,
   uimdiag_mmgsdi_session_unblock_pin_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD,
   uimdiag_mmgsdi_session_get_all_pin_status_cmd },
  {(word)UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD,
   uimdiag_mmgsdi_session_disable_service_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD,
   uimdiag_mmgsdi_session_enable_service_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD,
   uimdiag_mmgsdi_session_is_service_avail_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_CMD,
   (word)UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_CMD,
   uimdiag_mmgsdi_session_get_operator_name_table_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD,
   uimdiag_mmgsdi_session_prov_app_init_complete_cmd},
  {(word)UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD ,
   (word)UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD,
   uimdiag_mmgsdi_session_read_prl_cmd},
  {(word)UIMDIAG_MMGSDI_HIDE_CSIM_CMD,
   (word)UIMDIAG_MMGSDI_HIDE_CSIM_CMD,
   uimdiag_mmgsdi_hide_csim_cmd},
  {(word)UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD,
   (word)UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD,
   uimdiag_mmgsdi_map_sessions_to_slots_cmd},
  {(word)UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD,
   (word)UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD,
   uimdiag_mmgsdi_naa_refresh_ext_cmd}
#endif /* FEATURE_UIMDIAG_INTERNAL_API */
};

uimdiag_req_table_elem_type *uimdiag_req_table_ptr = NULL;

/*=============================================================================

                              FUNCTIONS

=============================================================================*/

/*===========================================================================
FUNCTION UIMDIAG_REQ_TABLE_INIT

DESCRIPTION
    Initializes the req_table to NULL.

ARGUMENTS
    None

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None
===========================================================================*/
static void uimdiag_req_table_init(void)
{
  /* Initialize table. Set it to NULL */
  uint32 max_client_req_tbl_size          = UIMDIAG_MAX_CLIENT_REQ_TBL_SIZE;

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(
    uimdiag_req_table_ptr,
    (sizeof(uimdiag_req_table_elem_type) * max_client_req_tbl_size));
}/* uimdiag_req_table_init */


/*===========================================================================

FUNCTION UIMDIAG_INIT

DESCRIPTION
  The uimdiag init function initializes sub system commands to DIAG framework.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void uimdiag_init(void)
{
  DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY(
    DIAG_SUBSYS_CMD_VER_2_F,
    (uint8)DIAG_SUBSYS_UIM,
    uimdiag_subsys_delayed_tbl);
    uimdiag_req_table_init();
}/* uimdiag_init */

