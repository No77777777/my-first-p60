#ifndef DS_MPPM_EXT_I_H
#define DS_MPPM_EXT_I_H
/*===========================================================================

                      DS_MPPM_EXT_I_H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2012-2019 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/wlan/mapcon/inc/ds_mppm_ext_i.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/08/19    my     Added support for QSH MDUMP collection.
05/23/18    vl     Added changes for FR 47777: IWLAN refactoring for AndroidP.
04/11/18    vl     Added support for FR 46730: (Rel-14)Unauth e911 over IWLAN.
03/14/15    vl     Add support for FR: No handover in roaming LTE network
10/13/14    vl     Added changes for DS3G API for MH - MPPM interaction to 
                   avoid race conditions
06/10/14    scb    Added support for MMGSDI_SUBSCRIPTION events
03/13/14    scb    Support for MMGSDI subscription events to report IWLAN
10/08/13    fn     Added support for DSDS/TSTS
09/16/13    scb    Added support for LTE_ATTACH_IN_PROGRESS
10/11/12    scb    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_WLAN_MAPCON
#include "sys.h"

#include "mmgsdilib_common.h"
#include "ds3geventmgr.h"
#include "dstask_v.h"
#include "ds_dsd_event_defs.h"
#include "ds_dsd_ext_i.h"
#include "ds_mppm_efs_parser.h"
#include "ds_wlan_util.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                                  MACROS

===========================================================================*/

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
    MPPM Transaction ID type
---------------------------------------------------------------------------*/
typedef uint32 ds_mppm_tid_type;

/*---------------------------------------------------------------------------
    MPPM Client radios. 
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_RADIO_INVALID = DS_DSD_RADIO_INVALID,
  DS_MPPM_RADIO_OOS = DS_DSD_RADIO_INVALID,
  DS_MPPM_RADIO_LTE = DS_DSD_RADIO_LTE,
  DS_MPPM_RADIO_MIN = DS_DSD_RADIO_MIN,
  DS_MPPM_RADIO_HDR = DS_DSD_RADIO_HDR,
  DS_MPPM_RADIO_1X = DS_DSD_RADIO_1X,
  DS_MPPM_RADIO_GSM =  DS_DSD_RADIO_GSM, 
  DS_MPPM_RADIO_TDSCDMA = DS_DSD_RADIO_TDSCDMA,
  DS_MPPM_RADIO_WCDMA = DS_DSD_RADIO_WCDMA,
#ifdef FEATURE_LAPP
  DS_MPPM_RADIO_5G = DS_DSD_RADIO_5G,
#endif /*FEATURE_LAPP_UFS*/
  DS_MPPM_RADIO_WLAN_LB = DS_DSD_RADIO_WLAN,
  DS_MPPM_RADIO_IWLAN,
  DS_MPPM_RADIO_MAX=DS_MPPM_RADIO_IWLAN
}mppm_radio_type;

/*---------------------------------------------------------------------------
    MPPM EXTERNAL EVENTS. CLIENTS can register to get these events
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_INVALID_EV      = 0,
  /*------------------------------------------------------------------------
  AGGREGATED PREF_SYS_CHANGE_EV [for upto DS_MAX_PDN_COUNT number 
  of PDNs]
  -------------------------------------------------------------------------*/
  DS_MPPM_AGGR_PREF_SYS_CHANGED_EV,

  DS_MPPM_MAX_EV = DS_MPPM_AGGR_PREF_SYS_CHANGED_EV
}ds_mppm_event_type;

/*---------------------------------------------------------------------------
    MPPM CLIENT TYPES - INTERNAL and EXTERNAL
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_TYPE_INTERNAL_CLIENT      = 0,
  DS_MPPM_TYPE_EXTERNAL_CLIENT,
  DS_MPPM_TYPE_MAX_CLIENT           = DS_MPPM_TYPE_EXTERNAL_CLIENT
}ds_mppm_client_type;

/*---------------------------------------------------------------------------
    MPPM SUBSCRIPTION ERROR CODE TYPE
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_UNKNOWN_STATUS_ERR = -1,
  DS_MPPM_NO_ERR = 0,
  DS_MPPM_INVALID_INPUT_PARAM,
  DS_MPPM_ERR_SUBS_INFO_NOT_AVAILABLE,
  DS_MPPM_ERR_MEM_NOT_AVAILABLE
} ds_mppm_error_code_type; 

/*---------------------------------------------------------------------------
    MPPM SUBSCRIPTION INPUT TYPE
    Extension in the future, clients should initialize to struct to zero
---------------------------------------------------------------------------*/
typedef struct
{
   ps_sys_subscription_enum_type        dsd_subs_id;   
} ds_mppm_pref_in_s_type;

/*---------------------------------------------------------------------------
    MPPM SUBSCRIPTION RETURN TYPE
---------------------------------------------------------------------------*/
typedef struct
{
   ds_mppm_error_code_type  err_code;
} ds_mppm_pref_out_s_type;

/*---------------------------------------------------------------------------
  MPPM DSD event info type structure
---------------------------------------------------------------------------*/
typedef struct
{
  dsd_radio_type                 dsd_pref_index;
  uint32                         dsd_changed_radio_mask;
  ds_dsd_ext_radio_info_type     dsd_radio_info[DS_MPPM_RADIO_MAX];
  ps_sys_subscription_enum_type  dsd_subs_id;
  boolean                        is_lte_roaming;
}ds_mppm_dsd_event_info_type;

/*---------------------------------------------------------------------------
    MPPM EXTERNAL EVENTS. CLIENTS can register to get these events
---------------------------------------------------------------------------*/
typedef struct
{
  mppm_radio_type               pref_rat_index;
  uint8                         num_avail_rats;
  mppm_radio_type               avail_rat_index[DS_MPPM_RADIO_MAX];
  char                          apn_string[DS_SYS_MAX_APN_LEN]; 
                                /* PDN name */
  uint32                        apn_string_len;
  boolean                       lte_attach_in_progress;
                                /* PDN name length */
  mppm_radio_type               recommended_pref_rat_idx;
                                /* recommended pref rat */
}ds_mppm_per_pdn_rat_info_s_type;

/*---------------------------------------------------------------------------
  MPPM event info type structure //FN debug outgoing event structure
---------------------------------------------------------------------------*/
typedef struct
{
  ds_mppm_tid_type                  mppm_tid;
  uint8                             num_pdns_config;  
                 /* Number of PDNs configured in the db */
  uint8                             fallback_apn_idx;
  boolean                           pref_rat_chg;
/*
  APN index to be used in case of no-match, if this is
  INVALID, then the dsd_pref_index is used.
*/
  ds_mppm_dsd_event_info_type       *mppm_dsd_event_info_ptr;
                      /* Pointer to DSD ext event info */
  ds_mppm_per_pdn_rat_info_s_type   *rat_aggr_info[DS_MAX_PDN_COUNT];
}ds_mppm_event_info_s_type;

/*---------------------------------------------------------------------------
  MPPM Registers a callback function with internal clients. This callback 
  needs to be called when the internal clients finish processing events
  posted by MPPM.
---------------------------------------------------------------------------*/
typedef void (*ds_mppm_event_proc_complete_cb_type)
(
  ds_mppm_event_type        mppm_event,
  ds_mppm_tid_type          mppm_tid,
  void                      *data_ptr
);

/*---------------------------------------------------------------------------
  MPPM Client Registers a callback function with the MPPM module
---------------------------------------------------------------------------*/
typedef void (*ds_mppm_notify_func_type)
(
  ds_mppm_event_type         mppm_event,
  ds_mppm_event_info_s_type  *mppm_event_info_ptr,
  void                       *data_ptr
);

/*===========================================================================

                      GLOBAL VARIABLES

===========================================================================*/

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION       DS_MPPM_INIT

DESCRIPTION    This is an internal function called during powerup()
               EFS files indicating the per PDN preference data base are 
               read and parsed to a format suitable for calculation of the 
               preferred RAT per PDN.
               The MPPM module also registers as an internal client with the
               DSD module during powerup.
               The MPPM module also registers as an internal client with the
               PS_SYS module during powerup for 
               PS_SYS_EVENT_USER_IWLAN_PREFERENCE_CHANGE events.
               The MPPM module initializes the critical section and the 
               event queues.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   Internal array referencing the PDN policy databases are 
               created and updated statically with per PDN policy information
===========================================================================*/
void ds_mppm_init
(
  void
);

/*===========================================================================
FUNCTION       DS_MPPM_DEINIT

DESCRIPTION    This function cleansup all the memory allocated during 
               ds_mppm_init().

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================
FUNCTION      DS_MPPM_EVENT_REG_CLIENT

DESCRIPTION   Called by clients to register to get MPPM event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE    : if registration successful
              FALSE   : if registration unsuccessful

SIDE EFFECTS  Registered client is entered in a queue
===========================================================================*/
boolean ds_mppm_event_reg_client
(
  ds_mppm_event_type                   mppm_event,
  ds_mppm_client_type                  client_type,
  ds_mppm_notify_func_type             ds_mppm_notify_cb_func_ptr,
  ds_mppm_event_proc_complete_cb_type  *mppm_event_proc_complete_cb_ptr,
  void                                 *data_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_EVENT_DEREG_CLIENT

DESCRIPTION   Called by clients to de-register for event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: if de-registration successful
                    if client previously de-registgered

              FALSE: if de-registration unsuccessful

SIDE EFFECTS  De-registered client is dequeued
===========================================================================*/
boolean ds_mppm_event_dereg_client(
  ds_mppm_event_type         mppm_event,
  ds_mppm_notify_func_type   ds_mppm_notify_cb_func_ptr,
  void                      *data_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_GET_PREF_RAT_FOR_APN_STRING_EX

DESCRIPTION   Called by various sub-modules to determine the preferred RAT
              for a particular PDN with apn_string apn_name.

DEPENDENCIES  Memory for mppm_dsd_event_info_ptr is provided by the caller. 
              Subscription id is provided in mppm input paramaters 

PARAMETERS    APN name for which the preferred RAT is to be retrieved.
              Pointer to the ds_mppm_per_pdn_rat_info_s_type struct which
              will store information assc. with the preferred RAT returned
              along with the list of available RATs of specificed
              subscription in mppm_input_params. mppm_output_params
              specifies error codes and output information to the client

RETURN VALUE  mppm_radio_type to indicate the preferred RAT index in the
              list of RATs pointed to by mppm_dsd_event_info_ptr.
              
SIDE EFFECTS  

===========================================================================*/
mppm_radio_type ds_mppm_get_pref_rat_for_apn_string_ex
(
  ds_mppm_apn_name_type             apn_name,
  ds_mppm_pref_in_s_type            mppm_input_params,
  ds_mppm_dsd_event_info_type      *mppm_dsd_event_info_ptr,
  ds_mppm_pref_out_s_type          *mppm_output_params
);

/*===========================================================================
FUNCTION       DS_MPPM_PROCESS_CMD

DESCRIPTION    Msg router

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_mppm_process_cmd
(
  ds_cmd_type *cmd_ptr
);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_mppm_cmd_hdlr_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for mppm_iwlan_timer,mppm_wwan_timer,mppm_wlan_idi_info
  ds_mppm_wlan_capable, ds_mppm_data_roaming_ui_option, ds_mppm_lte_roaming_status static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_mppm_cmd_hdlr_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_mppm_event_hdlr_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_mppm_event_info_data_blk_q static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_mppm_event_hdlr_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_mppm_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_mppm_event_int_q,ds_mppm_event_ext_q,ds_mppm_event_free_q,ds_mppm_rat_pref_info
  ds_mppm_wifi_settings_info static and mppm_pdn_policy_db_from_efs_q,mppm_pdn_policy_db_q global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_mppm_qsh_mdump_collect();

#endif /* FEATURE_QSH_MDUMP */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

#endif /*FEATURE_DATA_WLAN_MAPCON*/
#endif /* DS_MPPM_EXT_I_H */
