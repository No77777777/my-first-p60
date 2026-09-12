#ifndef WL1_FET_CTRL_H
#define WL1_FET_CTRL_H
/*===========================================================================
                 WCDMA L1 FET Controller

GENERAL DESCRIPTION
  This file contains the code for controlling the operation of Frame Early Termination feature

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/wl1fetctrl.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    ---------------------------------------------------------
05/24/16   ac     Disable FET during DSR transitions
05/19/16   svh    Added new client for HSUPA.
11/04/15   ac     Disable LVA for HLS CM
06/29/15   ac     Save local copy of NV for both subs
02/27/15   ap     Initial check-in for WL1 QSH framework 
04/28/15   ac     Add 0x41D0 - DPCH demod enhancement log packet
03/19/15   ac     W + W datapath init cleanup
01/26/15   ac     Initial checkin for X+W
===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "customer.h"

#include "l1const.h"
#include "l1def.h"

#include "l1cmmgr.h"
#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
#include "wl1_qsh_ext.h"
#endif /* defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* DPCH demod enhancements log packet */
#define WL1_DPCH_DEMOD_ENH_NUM_SAMPLES 50
#define WL1_DPCH_DEMOD_ENH_NUM_BUFFERS 2

/* Data for each sample */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 fet_attempted;
  uint8 fet_enabled;
  uint8 fet_slot;
  uint8 dcch_det;
} wl1_dpch_demod_enh_sample;

/* Full log packet definition */
LOG_RECORD_DEFINE(WL1_DPCH_DEMOD_ENH_LOG_PKT)
  uint8                      version_number;
  uint8                      num_samples;
  uint16                     reserved;
  wl1_dpch_demod_enh_sample  samples[WL1_DPCH_DEMOD_ENH_NUM_SAMPLES];
LOG_RECORD_END

/* Internal database structure for log packet handling*/
typedef struct
{
  WL1_DPCH_DEMOD_ENH_LOG_PKT_type log_buffer[WL1_DPCH_DEMOD_ENH_NUM_BUFFERS];
  boolean                         log_pkt_avail[WL1_DPCH_DEMOD_ENH_NUM_BUFFERS];
  wl1_dpch_demod_enh_sample      *sample_ptr;
  uint8                           cur_log_idx;
  uint8                           log_idx_to_submit;
  boolean                         qxdm_logging_enabled;
  uint32                          read_idx;
}wl1_dpch_enh_log_pkt_db_type;

/* List of various modules/call types that register their status with FET */
typedef enum
{
  WL1_FET_CLIENT_NONE = 0,
  WL1_FET_CLIENT_2TF_BTFD = 1,
  WL1_FET_CLIENT_NON_2TF_BTFD = 2,
  WL1_FET_CLIENT_SINGLE_RATE_BTFD_AMR = 3,
  WL1_FET_CLIENT_MULTI_RATE_BTFD_AMR = 4,
  WL1_FET_CLIENT_SYNC = 5,
  WL1_FET_CLIENT_HS = 6,
  WL1_FET_CLIENT_RXD = 7,
  WL1_FET_CLIENT_CM = 8,
  WL1_FET_CLIENT_SF = 9,
  WL1_FET_CLIENT_PO3 = 10,
  WL1_FET_CLIENT_GSM_TX = 11,
  WL1_FET_CLIENT_VOICE_CALL = 12,
  WL1_FET_CLIENT_TUNED_TO_MULTI_CARR = 13,
  WL1_FET_CLIENT_EUL = 14,
  WL1_FET_CLIENT_DSR = 15
}wl1_fet_client_enum_type;

/* Describes the status of each algorithm 
   as set through NV or QXDM cmd */
typedef enum
{
  ALGO_OFF = 0,
  ALGO_ON = 1,
  ALGO_DYNAMIC
}wl1_fet_algo_state_enum_type;

/* Describes the status of DCCH Aware ILPC algorithm 
   as set through NV or QXDM cmd */
typedef enum
{
  DAI_ALGO_OFF = 0,
  DAI_ALGO_ON = 1,
  DAI_FR_ALGO_ON = 2,
  DAI_BOTH_ALGO_ON
}wl1_fet_dcch_aware_ilpc_algo_state_enum_type;

/* Describes the rate of AMR call */
typedef enum
{
  FET_INVALID_RATE_BTFD_AMR,
  FET_SINGLE_RATE_BTFD_AMR,
  FET_MULTI_RATE_BTFD_AMR
}wl1_fet_btfd_amr_call_enum_type;

/* Describes the type of CM pattern */
typedef enum
{
  FET_CM_TYPE_NONE = 0,
  FET_CM_TYPE_SFR = 1,
  FET_CM_TYPE_HLS = 2
}wl1_fet_cm_enum_type;

/* Describes the type of DCCH in a BTFD call
   (Two Transport Format DCCH, or not) */
typedef enum
{
  FET_NON_BTFD,
  FET_TWO_TF_BTFD_DCCH,
  FET_NON_TWO_TF_BTFD_DCCH
}wl1_fet_btfd_dcch_enum_type;

/* Main Database Structure */
typedef struct
{
  /* Raw values read from NV for each sub */
  uint16 raw_nv_values[WCDMA_NUM_SUBS];

  /* Value read from NV */
  uint16 nv_value;
  
  /* Variables that hold the status of various algorithms
     As decided by NV or QXDM commands */
  wl1_fet_algo_state_enum_type dcch_erasure_det_v2_oride_state;
  wl1_fet_dcch_aware_ilpc_algo_state_enum_type dcch_aware_ilpc_oride_state;
  wl1_fet_algo_state_enum_type dcch_fr_eng_comp_oride_state;
  
  wl1_fet_algo_state_enum_type fet_oride_state;
  wl1_fet_algo_state_enum_type msrch_oride_state;
  wl1_fet_algo_state_enum_type dape_oride_state;
  wl1_fet_algo_state_enum_type lva_oride_state;

  /* Variables that hold the status of various clients/call types
     as registered by those entities */
  wl1_fet_btfd_amr_call_enum_type btfd_amr_call_rate;
  wl1_fet_btfd_dcch_enum_type btfd_dcch_type;
  wl1_fet_cm_enum_type cm_status;
  boolean sync_status;
  boolean hs_status;
  boolean eul_status;
  boolean rxd_status;
  boolean spread_factor_ok;
  boolean po3_ok;
  boolean gsm_tx_status;
  boolean voice_call_status;
  boolean tuned_to_multi_carr;
  boolean dsr_status;

  /* Variables that hold latest SW decision on enable/disable of the 
     various algorithms. Decision depends on NV/QXDM cmd and status of
     various entities listed above */
  boolean dcch_erasure_det_v2_enable;
  uint8 dcch_aware_ilpc_mode;
  boolean dcch_fr_eng_comp_enable;
  boolean fet_enable;
  boolean msrch_enable;
  boolean dape_enable;
  boolean lva_enable;
}wl1_fet_db_type;

/* Demback Extended Log Packet */
/* Maximum number of samples per log packet. */
#define WL1_DEMBACK_EXT_NUM_MAX_SAMPLES  (50)

/* Number of log packet buffers. */
#define WL1_DEMBACK_EXT_NUM_LOG_BUFFERS      (2)

/* Demback extended log packet. */
LOG_RECORD_DEFINE(WL1_DEMBACK_EXT_LOG_PKT)
  uint8                      version_number;
  uint8                      num_samples;
  uint16                     reserved;
  WfwDembackDpchFrameStruct samples[WL1_DEMBACK_EXT_NUM_MAX_SAMPLES];
LOG_RECORD_END


/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Function Definitions
** ----------------------------------------------------------------------- */


/*===========================================================================
FUNCTION wl1_fet_init

DESCRIPTION
This function initializes all the one time and control parameters.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_dch_init

DESCRIPTION
This function initializes FET params needed at DCH init

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_dch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_reset_database

DESCRIPTION
This function initializes the database

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_reset_database(void);

/*===========================================================================
FUNCTION wl1_fet_read_nv_item

DESCRIPTION
This function reads FET NV

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_read_nv_item( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_fet_update_db_from_saved_nv

DESCRIPTION
  This function updates FET params from the saved NV value

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void wl1_fet_update_db_from_saved_nv( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_record_client_state

DESCRIPTION
This function is called by clients of FET to record their current state when it changes

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_record_client_state(wl1_fet_client_enum_type client, boolean status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_record_active_cctrch

DESCRIPTION
This function keeps track of the CCTRCH index that is active

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_record_active_cctrch(uint8 cctrch_table_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_get_active_cctrch

DESCRIPTION
This function returns the CCTRCH index that is active

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
uint8 wl1_fet_get_active_cctrch(void);

/*===========================================================================
FUNCTION wl1_fet_update_status

DESCRIPTION
This function updates FET state based on current UE status

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_update_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_dch_cleanup

DESCRIPTION
This function cleans up FET state

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_fet_dch_cleanup(boolean conn_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_is_dcch_erasure_det_v2_enabled

DESCRIPTION
This function returns status of DCCH erasure detection V2 feature

DEPENDENCIES
None

RETURN VALUE
Boolean: TRUE if DCCH erasure detection V2 is enabled; Else FALSE;

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_fet_is_dcch_erasure_det_v2_enabled(void);

/*===========================================================================
FUNCTION wl1_fet_is_dcch_erasure_det_enabled_in_fw

DESCRIPTION
This function status of DED feature enable in FW

DEPENDENCIES
None

RETURN VALUE
Boolean: TRUE if DED is enabled; Else FALSE;

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_fet_is_dcch_erasure_det_enabled_in_fw(void);

/*===========================================================================
FUNCTION wl1_fet_is_fet_enabled_in_fw

DESCRIPTION
This function status of FET feature enable in FW

DEPENDENCIES
None

RETURN VALUE
Boolean: TRUE if FET is enabled; Else FALSE;

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_fet_is_fet_enabled_in_fw(void);

/*===========================================================================
FUNCTION wl1_fet_is_dape_enabled_in_fw

DESCRIPTION
This function status of DAPE feature enable in FW

DEPENDENCIES
None

RETURN VALUE
Boolean: TRUE if DAPE is enabled; Else FALSE;

SIDE EFFECTS
None
===========================================================================*/
extern boolean wl1_fet_is_dape_enabled_in_fw(void);

/*===========================================================================
FUNCTION wl1_fet_oride_cmd

DESCRIPTION
  API used to update FET params through QXDM command 

DEPENDENCIES
  None

RETURN VALUE
  Return value of API called

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_fet_oride_cmd(uint8 api_identifier, uint32 input_arg_1, uint32 input_arg_2, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_set_power_offsets

DESCRIPTION
This function populates the power offsets to FW-SW interface

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_set_power_offsets(uint32 po3, uint8 slot_format, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_cm_state_cb

DESCRIPTION
This function is registered with CM module

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_cm_state_cb(l1_cm_active_deactive_enum_type cm_status,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_demback_ext_submit_log

DESCRIPTION
  Called when the local command is processed, submits the Demback (Extended) log packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_demback_ext_submit_log( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_demback_ext_update_logging

DESCRIPTION
  Updates the current Demback (Extended) log packet by getting data from the FW/SW interface.
  Prepares to submit the log packet if it fills up 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_demback_ext_update_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_record_spread_factor

DESCRIPTION
This function is called by DL to record the SF

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_fet_record_spread_factor(l1_sf_enum_type spread_factor, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_fet_resume_cphy_upn_fet_disable

DESCRIPTION
  This API de-configures and de-registers the allocated Gen TL event. This API
  would post a local command to resume processing the CPHY SETUP REQ.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_fet_resume_cphy_upn_fet_disable(uint8 evt, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_dpch_demod_enh_update_logging

DESCRIPTION
  Updates the current DPCH demod enhancement log packet by getting data from
  the FW/SW interface. Prepares to submit the log packet if it fills up .
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_dpch_demod_enh_update_logging(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_dpch_demod_enh_submit_log

DESCRIPTION
  Called when the local command is processed, submits the DPCH demod
  enhancement log packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_dpch_demod_enh_submit_log(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION wl1_fet_debug_dump

DESCRIPTION
  Dumps info into wl1_debug_fet_db_info_struct_type
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_fet_debug_dump(wl1_qsh_datapath_info_struct_type* dump_ptr);
#endif /* FEATURE_WCDMA_QSH_DUMP*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  wl1_fet_qsh_dump

DESCRIPTION
  Collect FET variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_fet(wsub_id_e_type wsub_id);
#endif


#endif /* WL1_QICE_CTRL_H */


