#ifndef HSRACHCNTRLR_H
#define HSRACHCNTRLR_H

/*===========================================================================
                 WCDMA L1 HS RACH Controller (HEADER)

GENERAL DESCRIPTION
  This is a header file that has the APIs with which the HS RACH controller
  can interface with the different modules within WCDMA L1.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2006 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1hsrachcntrlr.h#4 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---   ----------------------------------------------------------
11/20/19   gp     API to inform the common EDCH resource allocation status
12/04/15   raj    Backout debug code for SS Tables delay flag not being cleared
12/03/15   raj    De-link the activation of semi static parameters for EUL in FACH(HSRACH) and DCH
10/07/15   oh     Add debug code for SS Tables delay flag not being cleared
08/18/15   ar     Doubling uplink state variables to maintain per subscription state
07/19/15   vr     Code cleanup to move all UL/EUL/EDL/HSRACH states to common struct.
06/30/15   tsk    FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
06/27/15   vr     Code cleanup to use MACROs for HSRACH state globals.
02/28/15   rsr    W + W Phase 2 Checkin.
02/03/15   vr     Remove inits of some globals & cleanup some unused globals.
01/16/15   vr     Global reorg for W + W.
01/15/15   vr     Removing unused globals.
08/18/14   vr     Cleanup code for HSRACH 'conventional' seamless recfg.
12/18/13   ash    Fix for HSRACH corner case scenario where SS tables are not ready
09/19/13   vr     Optimization for HSRACH non-seamless reconfig timeline
09/12/13   jd     Searcher redesign
08/15/13   vr     For HSRACH F2D SL Recfg, added changes to ensure correct EDL Stop SubFn Calculation
05/20/13   sad    Support for Enhanced Layer-1 Manager
12/11/12   vr     HSRACH code cleanup
09/13/12   vr     Created prototype for indicating EDCH resource
                  is allocated. 
09/05/12   vr     Moved logging and edrx_hs_notify to task context as HSRACH done
                  ISR was getting severely delayed.
08/13/12   ash    Linux compilation error and merge error
07/10/12   vr     Added UPH calculation specific to HSRACH
07/06/12   vr     HSRACH fixes
06/19/12   vr     HSRACH code update
06/04/12   gv     Feature HS RACH Initial code check in -- File Creation
*/

#include "wcdma_variation.h"
#ifdef FEATURE_WCDMA_HS_RACH

#include "l1rrcif.h"
#include "srchset.h"
#include "l1macdata.h"
#include "l1const.h"
#include "ulcmd.h"
#include "dlphch.h"
#include "wenc.h"

 /*==============================================
                                       Header Inclusions
 **==============================================*/



/*==============================================
                                        MACROS
**==============================================*/
#ifdef FEATURE_WCDMA_EFACH_MSIM
  /* To see if RRC MACRO can be used here */
  #define WL1_HS_RACH_RRC_TIMER WL1_W2G_TUNEAWAY_OVERHEAD_DEFAULT_MSEC
  /* For 10ms, timer will expire 30ms prior to W suspend */
  #define WL1_HS_RACH_WL1_SI0_TIMER_10MS (WL1_HS_RACH_RRC_TIMER + 30)
  /* For 2ms, timer will expire 30ms prior to W suspend */
  #define WL1_HS_RACH_WL1_SI0_TIMER_2MS (WL1_HS_RACH_RRC_TIMER + 8)

  #define WL1_HS_RACH_EUL_10MS_SI_ONLY_TX_OFFSET 1
  #define WL1_HS_RACH_EUL_2MS_SI_ONLY_TX_OFFSET 2

  #define WL1_HS_RACH_EUL_10MS_STOP_PDU_BUILD 1
  #define WL1_HS_RACH_EUL_2MS_STOP_PDU_BUILD 2

#endif /* FEATURE_WCDMA_EFACH_MSIM */

/* HS RACH Debugging log mask*/
#define WL1_HS_RACH_EUL_ISR_DEBUG_MASK 0x00000001
#define WL1_HS_RACH_CFN_TIMING_DEBUG_MASK 0x00000002
#define WL1_HS_RACH_EUL_DL_RESULTS_DEBUG_MASK 0x00000004
/* Validation related MACROs*/

/* S_Offset*/
#define WL1_HS_RACH_S_OFFSET_MIN_VALUE 0
#define WL1_HS_RACH_S_OFFSET_MAX_VALUE 9

/* F-DPCH OVSF Code number*/
#define WL1_HS_RACH_FDPCH_CODE_NUM_MIN_VALUE 0
#define WL1_HS_RACH_FDPCH_CODE_NUM_MAX_VALUE 255

/* RGCH Validation*/
#define WL1_HS_RACH_RG_SIG_IDX_MIN_VALUE 0
#define WL1_HS_RACH_RG_SIG_IDX_MAX_VALUE 39

/*HICH Validation*/
/* HICH Signature sequence index*/
#define WL1_HS_RACH_HICH_SIG_IDX_MIN_VALUE 0
#define WL1_HS_RACH_HICH_SIG_IDX_MAX_VALUE 39

/* HICH OVSF CODE*/
#define WL1_HS_RACH_HICH_OVSF_MIN_VALUE 0
#define WL1_HS_RACH_HICH_OVSF_MAX_VALUE 127

/* UL SCRAMBLING CODE*/
#define WL1_HS_RACH_UL_DPCH_SCR_CODE_MIN_VALUE 0
#define WL1_HS_RACH_UL_DPCH_SCR_CODE_MAX_VALUE 16777215

#define WL1_HS_RACH_CHECK_PARAM_WITHIN_LIMIT(param, min_limit, max_limit) \
     ((param >= min_limit) ||(param <= max_limit))

/* HS RACH API to set the HS RACH state */
#define WL1_HS_RACH_SET_STATE_VAR(state) (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wl1_hs_rach_state = state)
/* HS RACH macro to get the HS RACH STATE*/
#define WL1_HS_RACH_GET_STATE_VAR()      (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wl1_hs_rach_state)

/* HS RACH API to set the HS RACH sub state */
#define WL1_HS_RACH_SET_SUBSTATE_VAR(substate) (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wl1_hs_rach_sub_state = substate)
/* HS RACH macro to get the HS RACH sub state*/
#define WL1_HS_RACH_GET_SUBSTATE_VAR()         (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wl1_hs_rach_sub_state)

/* HS RACH macro to set the common EDCH release cause*/
#define WL1_HS_RACH_SET_EDCH_REL_CAUSE(edch_rel_cause) (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_edch_rsrc_rel_cause = edch_rel_cause)
/* HS RACH macro to get the common EdCH release cause*/
#define WL1_HS_RACH_GET_EDCH_REL_CAUSE()               (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_edch_rsrc_rel_cause)

#define WL1_HS_RACH_CONFIGURED()     (WL1_HS_RACH_GET_STATE_VAR() != WL1_HS_RACH_INIT)
#define WL1_HS_RACH_IS_IN_PROGRESS() (WL1_HS_RACH_GET_STATE_VAR() == WL1_HS_RACH_IN_PROGRESS)

/* The events which can be enabled once the SET MOD TIMING COMMAND is sent
** and a call back is invoked.*/
#define WL1_HS_RACH_TX_EVENTS_MASK (MCALWCDMA_EVT_EVENT_ID_TO_MASK(TX_GEN_GET_DATA_EVENT) |    \
                                    MCALWCDMA_EVT_EVENT_ID_TO_MASK(TX_GEN_PC_ENABLE_EVENT))

/* MACRO to get the current PSC so that it can be populated in the HS RACH EDL setup proc*/
#define WL1_HS_RACH_GET_CURR_PSC()  (SCR_CODE_TO_PRI_SCR_CODE(srchsetutil_get_serving_cell(CARR_0 ,wsub_id)->scr_code))

/* Computing the Tau FDPCH after the common EDCH resource index is computed*/
#define WL1_HS_RACH_COMPUTE_TAU_FDPCH(tau_fdpch,eai_succ_pn_cnt,sOffset) \
  (tau_fdpch = (eai_succ_pn_cnt + 40 + sOffset)% STMR_TICKS_PER_FRAME)

#define WL1_HS_RACH_GET_ADDTNL_EDCH_TX_BACKOFF()     (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_config_params.mac_cfg_info.addtnl_edch_backoff)
#define WL1_HS_RACH_GET_EDCH_TX_TYPE()               (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_config_params.mac_cfg_info.edch_trans_type)
#define WL1_HS_RACH_GET_TAU_FDPCH_IN_BPG()           (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_config_params.edch_rsrc_info.tau_fdpch)
#define WL1_HS_RACH_GET_ADDTNL_EDCH_TX_BO_IN_MS()    (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_config_params.addtnl_edch_tx_bo_in_ms)
#define WL1_HS_RACH_SET_ADDTNL_EDCH_TX_BO_IN_MS(val) (wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_config_params.addtnl_edch_tx_bo_in_ms = val)

/* Invalid value for the common EDCH resource Index*/
#define WL1_HS_RACH_INVALID_EDCH_RSRC_IDX 0xFF
#define FDPCH_INTR_TIMEOUT_MSEC           30
#define UL_DPCCH_DROP_INTR_TIMEOUT_MSEC   50

/* Local Mutex for the protection of HS RACH state sub state variables in ISR and task context*/
#define WL1_HS_RACH_STATE_SUBSTATE_LOCK()   REX_ISR_LOCK(&wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_state_substate_mutex)
#define WL1_HS_RACH_STATE_SUBSTATE_UNLOCK() REX_ISR_UNLOCK(&wl1hsrachcntrlr_ext_cntrl_params.wl1_hs_rach_state_substate_mutex)

/* ===============================================
**                                   ENUMERATED TYPES DEFINTIONS
** ===============================================*/
typedef l1_mac_hs_rach_edch_rsrc_rel_cause_enum_type wl1_hs_rach_edch_rsrc_rel_cause_enum_type;

typedef l1_mac_hs_rach_config_params wl1_hs_rach_cfg_info_from_mac_struct_type ;

/* Below is the enum which will encapsulate all the HS RACH local
** commands within the HS RACH Controller itself.*/
typedef enum{
 /* Local command code to fill the common EDCH info to FW*/
 WL1_HS_RACH_FILL_EDCH_INFO_TO_FW,

 /* Local command code to trigger the EDCH release proc*/
 WL1_HS_RACH_EDCH_RELEASE_PROC,

 /* Local command code to execute logging handler of HS RACH*/
 WL1_HS_RACH_LOGGING_PROC_CMD,

 /*Local command code to push functionality from the HS RACH done
 ** ISR to task context*/
 WL1_HS_RACH_FW_DONE_ISR_CMD,

 WL1_HS_RACH_CLEANUP,

 /* INVALID MAX NUM enum*/
 WL1_HS_RACH_LOCAL_CMD_INVALID

}wl1_hs_rach_local_cmd_code_enum_type;

/* Different logging codes for HS RACH.  */
typedef enum {
 /* HS RACH configuration log packet ... covers the Higher layer config info*/
 WL1_HS_RACH_CONFIG_INFO_LOG_PKT,

 /* Allocated EDCH resource log packet*/
 WL1_HS_RACH_COMMON_EDCH_RSRC_INFO_LOG_PKT,

 /* Different channel timing log packet. Most of information is exported
 ** from FW*/
 WL1_HS_RACH_CHANNELS_TIMING_LOG_PKT,

 /* INVALID ENUM */
 WL1_HS_RACH_LOG_PKT_MAX_NUM
}wl1_hs_rach_log_pkt_enum_type;


/*! @brief eAiDecodeStatus maps to the following enum */
/*  WFW_RACH_EAICH_NOT_ATTEMPTED =0, (in case of AI silence)
  WFW_RACH_EAICH_NOT_CONFIGURED_AND_NACK = 1, (in case of AI = -1 and HS RACH not cfg'ed)
  WFW_RACH_EAICH_DEFAULT_RSRC_IDX = 2, ( in case of AI = +1)
  WFW_RACH_EAICH_CONFIGURED_DEC_RESULT_PLUS_ONE = 3, (in case of AI = -1, HS RACH cfg'ed)
  WFW_RACH_EAICH_CONFIGURED_DEC_RESULT_MINUS_ONE = 4, (in case of AI = -1, HS RACH cfg'ed)
  WFW_RACH_EAICH_ABORT = 5*/

typedef enum{
 WL1_HS_RACH_E_AI_NOT_ATTEMPTED,
 WL1_HS_RACH_E_AI_NOT_CFGD_AND_NACK,
 WL1_HS_RACH_DFLT_RSRC_IDX,
 WL1_HS_RACH_E_AI_PLUS_ONE,
 WL1_HS_RACH_E_AI_MINUS_ONE,
 WL1_HS_RACH_E_AI_ABORT
}wl1_hs_rach_eai_status_enum_type;

/* ===============================================
**                                   STRUCTURE TYPES DEFINTIONS
** ===============================================*/
typedef struct
{
  l1_local_cmd_hdr_type hdr;

  wl1_hs_rach_local_cmd_code_enum_type    cmd_code;

  wl1_hs_rach_log_pkt_enum_type         log_pkt_code;

} wl1_hs_rach_local_cmd_type;


typedef struct{
 /**/
 l1_prach_status_enum_type ai_status;
 l1_hsrach_status_enum_type hsrach_status;
 wl1_hs_rach_eai_status_enum_type eai_status;
 /**/
 uint32 edch_rsrc_idx;
 /**/
 uint32 eai_succ_pn_cnt;

 uint16 eai_sig_value;
}wl1_hs_rach_eai_stats_struct_type;

typedef struct{
 uint16 config_cfn;
 /*This is the number of sub frames offset in the current DPCH frame
 ** if it is this DPCH frame when we are planning to drop the EUL DL*/
 int16 num_sfn_to_final_sfn;
}wl1_hs_rach_eul_final_sfn_info_struct_type;

typedef struct{
 /* Used only for HS RACH for now*/
 uint16 chan2dpchOffset_div_by_30;
 wl1_hs_rach_eul_final_sfn_info_struct_type final_sfn_info;
 uint16 apply_subfn_reference;
 uint16 delta_sub_frames;
 uint16 correction_factor;
}wl1_hs_rach_edl_hs_rach_info_struct_type;

typedef struct
{
  uint32 stmr_time_aichdecode;
  uint32 stmr_time_fdpch;
  uint32 stmr_time_ul_dpcch;
  uint32 stmr_time_agch;
  uint32 stmr_time_hich;
  uint32 stmr_time_rgch;
  uint32 agch_to_fdpch_offset;
  uint32 hich_to_fdpch_offset;
  uint32 rgch_to_fdpch_offset;
  uint32 dpcch_to_reftime_offset;
}wl1_hs_rach_fw_chan_timing_struct_type;

typedef struct
{
  uint16 edch_rsrc_idx;
  uint16 fdpch_code_num;
  uint16 s_offset;
  uint16 ul_scr_code_type;
  uint32 ul_scr_code;
  uint16 hich_ovsf_code;
  uint16 hich_sig_idx;
  uint16 rgch_sig_idx;
  boolean rgch_enable;
  /*derived value*/
  uint16 tau_fdpch;

}wl1_hs_rach_edch_rsrc_info_struct_type;

typedef struct
{
 wl1_hs_rach_cfg_info_from_mac_struct_type mac_cfg_info;
 wl1_hs_rach_edl_hs_rach_info_struct_type hich_cfg_info;
 wl1_hs_rach_edl_hs_rach_info_struct_type agch_cfg_info;
 wl1_hs_rach_edl_hs_rach_info_struct_type rgch_cfg_info;
 wl1_hs_rach_eul_final_sfn_info_struct_type eul_final_sfn_info;
 wl1_hs_rach_eai_stats_struct_type eai_stats_info;
 wl1_hs_rach_fw_chan_timing_struct_type fw_chan_timing_info;
 wl1_hs_rach_edch_rsrc_info_struct_type edch_rsrc_info;
 uint16 fdpch_cfg_cfn;
 uint16 addtnl_edch_tx_bo_in_ms;
} wl1_hs_rach_config_params_struct_type;

typedef struct
{
  /* This is a structure which L1 will be populating from the RRC populated values in
  ** e_dl_info (above). The L1 channel setup code would be used this below information.
  ** this is done to allow the channel setup code to be common for legacy and HS RACH EDL*/
  l1_e_dl_info_struct_type wl1_hs_rach_edl_info_for_cfg;
  /* One jumbo HS RACH config structure .. stores all the information about HS RACH*/
  wl1_hs_rach_config_params_struct_type wl1_hs_rach_config_params;
  /* Variable maintaining the cause for EDCH resource release*/
  wl1_hs_rach_edch_rsrc_rel_cause_enum_type wl1_hs_rach_edch_rsrc_rel_cause;

  /* Timer to detect timeouts on responses from SRCH and Channel Manager. */
  timer_type wl1_hsrach_fdpch_intr_timeout_timer;

  #ifdef FEATURE_WCDMA_EFACH_MSIM
  timer_type wl1_hsrach_lta_si_send_timer;
  #endif /* FEATURE_WCDMA_EFACH_MSIM */

  /* Local Mutex for the protection of HS RACH state sub state variables in ISR and task context*/
  rex_crit_sect_type wl1_hs_rach_state_substate_mutex;
} wl1hsrachcntrlr_ext_cntrl_params_type;

extern wl1hsrachcntrlr_ext_cntrl_params_type wl1hsrachcntrlr_ext_cntrl_params;

typedef struct
{
  /* Global variable to indicate that there is a non seamless reconfig but
  ** skip sync A is happening*/
  boolean wl1_hsrach_non_seamless_skip_sync_A;
}wl1hsrachcntrlr_ext_persub_cntrl_params_type;

extern wl1hsrachcntrlr_ext_persub_cntrl_params_type wl1hsrachcntrlr_ext_persub_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  timer_type wl1_hsrach_ul_drop_intr_timeout_timer;
} wl1hsrachcntrlr_int_cntrl_params_type;

extern void wl1hsrachcntrlr_ext_cntrl_params_init();

/* ===============================================
**                                   FUNCTION PROTOTYPES
** ===============================================*/

/*===========================================================================

FUNCTION        ul_rach_proc_params_log_pkt_populate

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_hsrach_premable_status_log_pkt_populate(int16 txAgc,uint32 ul_last_aisum_val, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION    wl1hsrach_panic_sw_fw

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void wl1hsrach_panic_sw_fw( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        WL1_HS_RACH_HSDPCCH_DROP_DONE_HANDLER

DESCRIPTION     This function is called from HSDPCCH Module
                once the channel is tear down.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_hs_rach_hsdpcch_drop_done_handler(boolean status, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        hs_rach_config_log_pkt_populate

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void hs_rach_config_log_pkt_populate( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        ul_rach_alloc_edch_res_log_pkt_populate

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void ul_rach_alloc_edch_res_log_pkt_populate( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        ul_rach_chan_timing_log_pkt_populate

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void ul_rach_chan_timing_log_pkt_populate( wsub_id_e_type wsub_id);

void wl1_hsrach_ul_drop_intr_timeout_handler(timer_cb_data_type data);

/*===========================================================================
FUNCTION        WL1_HS_RACH_POST_LOCAL_CMD

DESCRIPTION     This function is called from different places within HS RACH
                controller to post a local command for some functionality of HS RACH.
                This function accepts two arguments.
                The first one is the command code and the second one is the log
                packet code. The caller of the function will ensure that the log packet
                code is correctly set when the local command code is LOGGING CMD.
                Otherwise the second argument should be set as
                WL1_HS_RACH_LOCAL_CMD_MAX_NUM

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS   A local command will be posted to L1M
===========================================================================*/
void wl1_hs_rach_post_local_cmd(
       wl1_hs_rach_local_cmd_code_enum_type local_cmd_code,
       wl1_hs_rach_log_pkt_enum_type log_pkt_code
       , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_CPHY_SETUP_CNF_PUT_STATUS

DESCRIPTION     This function is used by the HS RACH module to
                update the l1_cphy_setup_status buffer in the course of
                performing the steps of a CPHY_SETUP_REQ.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    The l1_cphy_status buffer is updated accordingly
===========================================================================*/
void wl1_hs_rach_cphy_setup_cnf_put_status
(
boolean status
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_EAI_STATS_PTR

DESCRIPTION   This function is called when processing the AICH info for
                HS RACH preamble procedure.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_eai_stats_struct_type* wl1_hs_rach_get_eai_stats_ptr(void);

/*===========================================================================

 FUNCTION        WL1_HS_RACH_VALIDATE_DL_FPDCH_PARAMS

 DESCRIPTION     This function is called from the HS RACH validate function. This
                 function validates the F-DPCH parameters configured for HS RACH setup.


 DEPENDENCIES    CPHY_SETUP_REQ command

 RETURN VALUE    Boolean indicating whether the F-DPCH validation was success

 SIDE EFFECTS    None

 ===========================================================================*/
  boolean wl1_hs_rach_validate_dl_fpdch_params(l1_hs_rach_f_dpch_info_struct_type *hs_rach_fdpch_info_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION		WL1_HS_RACH_VALIDATE_EUL_DL_PARAMS

DESCRIPTION 	This function is called from the HS RACH validate function. This
				function validates the EUL DL parameters configured for HS RACH setup.


DEPENDENCIES	CPHY_SETUP_REQ command

RETURN VALUE	Boolean indicating whether the EUL DL validation was success

SIDE EFFECTS	None

===========================================================================*/
 boolean wl1_hs_rach_validate_eul_dl_params(l1_e_dl_hs_rach_info_struct_type * hs_rach_e_dl_info_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_LOCAL_CMD_HANDLER

DESCRIPTION     This function is called as a local command handler
                for the HS RACH local command posted. This function
                based on the command code and log packet code,
                corresponding handler is called.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_hs_rach_local_cmd_handler(
       wl1_hs_rach_local_cmd_code_enum_type local_cmd_code,
       wl1_hs_rach_log_pkt_enum_type log_pkt_code
       , wsub_id_e_type wsub_id);

extern void wl1_hsrach_fdpch_intr_timeout_handler(timer_cb_data_type cb_data);

/*===========================================================================

FUNCTION        WL1_HS_RACH_VALIDATE_CPHY_SETUP_PARAMS

DESCRIPTION     This function is called from the CPHY_SETUP_REQ procedure
                when HS RACH configuration is included. This function internally will
                call other modules, like DL (F-DPCH), UL (UL DPCCH) and EUL (EUL DL
                and EUL UL) for validation of the channel specific params, for HS RACH.

DEPENDENCIES    CPHY_SETUP_REQ command

RETURN VALUE    Boolean

SIDE EFFECTS    None

===========================================================================*/
extern boolean wl1_hs_rach_validate_cphy_setup_params(l1_setup_cmd_type *setup_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_HS_RACH_CHANNEL_SETUP_EXT_PROC

DESCRIPTION     This function is called from the cphy_next_command_handler()
                as an entry function to the HS RACH channel setup procedure. Before
                calling this function the global variable wl1_hs_rach_action is set
                appropriately. Based on this action corresponding HS RACH channel
                function is called.

DEPENDENCIES    CPHY_SETUP_REQ command

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern  void wl1_hs_rach_channel_setup_ext_proc(l1_setup_cmd_type *setup_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_HS_RACH_CHANNEL_SETUP_ADD

DESCRIPTION     This function is called from the wl1_hs_rach_channel_setup_proc()
                and when the wl1_hs_rach_action global variable is set to
                WL1_HS_RACH_ACTION_START.


DEPENDENCIES    CPHY_SETUP_REQ command

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern  boolean wl1_hs_rach_channel_setup_add(dl_phychan_add_type *add_dl_phychan_ptr, 
                                         l1_ul_phychan_cfg_struct_type *add_ul_phychan_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_HS_RACH_CHANNEL_SETUP_DROP

DESCRIPTION     This function is called from the wl1_hs_rach_channel_setup_proc()
                and when the wl1_hs_rach_action global variable is set to
                WL1_HS_RACH_ACTION_STOP. The STOP is triggered due to
                a RRC CPHY_SETUP_REQ or cell transition request. But there is no need
                to pass the setup command, because the L1 needs to just clear up
                the database and send a disable command.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_hs_rach_channel_setup_drop( wsub_id_e_type wsub_id);

extern void wl1_hs_rach_channel_setup_cleanup( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        WL1_HS_RACH_INIT

DESCRIPTION     This function is called from the code flow for HS RACH
                STOP. This function will initialize all the HS RACH related variable,
                like state, sub state, release cause etc.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    HS RACH state machine and sub state machine get initialized

===========================================================================*/
extern void wl1_hs_rach_init(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_HS_RACH_FILL_EDCH_INFO_LIST_FW_ASYNC_INTF

DESCRIPTION     This function is called once the setup command, i.e. HS RACH
                START is processed and populate the common edch information
                list to FW asynchrounous buffer.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_hs_rach_fill_edch_info_list_to_fw(l1_e_dch_common_list_info_struct_type *e_dch_common_info_ptr);

/*===========================================================================

FUNCTION        WL1_HS_RACH_FDPCH_INTERRUPT_HANDLER

DESCRIPTION     This function is registered as the interrupt handler for
                the  FDPCH interrupt from the FW to L1. This interrupt is raised
                by FW when the FW has setup the F-DPCH and at the F-DPCH
                frame boundary

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_hs_rach_fdpch_interrupt_handler( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_HS_RACH_FILL_COMMON_EDCH_INFO

DESCRIPTION     This function is called from the RACH DONE ISR. This
                function is only called when the RACH status from FW is ACK
                and the common EDCH resource index is allocated. This function
                internaly calls various module specfic functions to populate the
                dedicated params like ovsf code and Soffset etc for F-DPCH.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    This function call fills the incomplete UL, DL, EUL UL and
                EUL DL databases with the information based on the common
                EDCH resource allocated

===========================================================================*/
extern void wl1_hs_rach_fill_common_edch_info(uint16 common_edch_rsrc_idx);

/*===========================================================================
FUNCTION        WL1_HS_RACH_PROCESS_TX_IND_FROM_MAC

DESCRIPTION     This function is called from the function that handles
                the external command from MAC for TX DATA IND, i.e.
                ul_tx_control_cmd().

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS   This function initiates the procedure to resume all the
                channels that are part of JUDC (DL F-DPCH, UL DPCCH,
                EUL UL and EUL DL).
===========================================================================*/
extern void wl1_hs_rach_process_tx_ind_from_mac( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_EFACH_MSIM
/*===========================================================================
FUNCTION      wl1_hsrach_postpone_edch_rsrc_release_ind

DESCRIPTION: This function checks whether UE should continue to release resource or should postpone it until QTA
                       gets over.

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern boolean wl1_hsrach_postpone_edch_rsrc_release_ind(wl1_hs_rach_edch_rsrc_rel_cause_enum_type rel_cause, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_EFACH_MSIM */
/*===========================================================================

FUNCTION wl1_hs_rach_edch_rsrc_alloc_status

DESCRIPTION This function informs the common EDCH resource allocation
            status
DEPENDENCIES
  None.

RETURN VALUE
  boolean TRUE: edch rsrc is in use.FALSE: edch rsrc is released.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean wl1_hs_rach_edch_rsrc_alloc_status(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_EDCH_RSRC_RELEASE_IND

DESCRIPTION     This function is called by different DL module
                (for RL failure and phychan est failure) and EUL module
                to indicate to HS RACH controller that a
                condition for releasing EDCH resource has occured.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS   This function will inturn update the Global variable
                that holds the release cause. After this function call, the
                EUL ISRs will be blocked and prematurely returned.
                (The ISRs would fires till the EDCH release process is
                handled and JUDC for HS RACH disable is sent).
===========================================================================*/
extern void wl1_hs_rach_edch_rsrc_release_ind(wl1_hs_rach_edch_rsrc_rel_cause_enum_type rel_cause, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_UPD_E_DL_CFG_INFO_FOR_SETUP

DESCRIPTION     This function is called from the validate params function
                specifically for HS RACH. THis is because the EUL DL setup information
                for HS RACH has to be packaged properly for the L1 code to flow
                appropriately. From RRC L1 will not get all the information which are
                present in the legacy strucuture

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_hs_rach_upd_e_dl_cfg_info_for_setup(l1_e_dl_hs_rach_info_struct_type
                                                            *hsrach_edl_setup_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_CFG_EVT_HANDLER

DESCRIPTION     This function is the config event handler registered for HS RACH
                config. This is to send the JUDC for suspend and STOP. This has to be sent
                using the config event handler because the command should be given
                to the FW correctly so that all channels can be torn down byt the end of
                the frame

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_hs_rach_cfg_evt_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_CHANNEL_DROP_INTERRUPT_HANDLER

DESCRIPTION     This function is the interrupt handler for the DROP DONE from FW.
                In this function the individual deactivate APIs are called and necessary
                channel specific deactivation is handled in there.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_hs_rach_channel_drop_interrupt_handler( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION        WL1_HS_RACH_PROCESS_EAI_STATUS

DESCRIPTION     This function is deals with the handling of the result of t
                RACH preamble procedure. Based on the outcome of this, the
                code flows to different sections and so does HS RACH state
                machine

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_hs_rach_process_eai_status(wl1_hs_rach_eai_stats_struct_type* eai_stats, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_MAC_CFG_INFO_PTR

DESCRIPTION   This function is called when processing the AICH info for
                HS RACH preamble procedure, this would populate the MAC
                configuration parameters

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_cfg_info_from_mac_struct_type* wl1_hs_rach_get_mac_cfg_info_ptr(void);
/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_HICH_CFG_INFO_PTR

DESCRIPTION   This function is called to get the HICH config information and
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_edl_hs_rach_info_struct_type* wl1_hs_rach_get_hich_cfg_info_ptr(void);

/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_AGCH_CFG_INFO_PTR

DESCRIPTION   This function is called to get the AGCH config information and
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_edl_hs_rach_info_struct_type* wl1_hs_rach_get_agch_cfg_info_ptr(void);

/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_RGCH_CFG_INFO_PTR

DESCRIPTION   This function is called to get the RGCH config information and
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_edl_hs_rach_info_struct_type* wl1_hs_rach_get_rgch_cfg_info_ptr(void);

/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_EUL_FINAL_SFN_INFO_PTR

DESCRIPTION   This function is called to get the EUL final SFN information and
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_eul_final_sfn_info_struct_type* wl1_hs_rach_get_eul_final_sfn_info_ptr(void);
/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_EDCH_RSRC_IDX_INFO_PTR

DESCRIPTION   This function is called to get the EDCH resource index information
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_edch_rsrc_info_struct_type* wl1_hs_rach_get_edch_rsrc_idx_info_ptr(void);

/*===========================================================================
FUNCTION        WL1_HS_RACH_GET_FW_TIMING_INFO_PTR

DESCRIPTION   This function is called to get the FW timing information
        for either using the same or update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern wl1_hs_rach_fw_chan_timing_struct_type* wl1_hs_rach_get_fw_timing_info_ptr(void);

/*===========================================================================
FUNCTION        WL1_HS_RACH_SRCHCR_CELL_RESEL_ALLOWED

DESCRIPTION     This function is called from the SRCHCR module to
                query HS RACH controller whether it is safe to trigger a 
                cell reselection. If this function returns FALSE, then HS RACH
                state machine cannot allow an cell resel now. If this function
                returns TRUE, then cell resel can be allowed. If this function
                returns TRUE, then RACH ABORT state machine should be
                upgraded accordingly to not allow RACH.
                                
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS   This function updates the RACH ABORT state machine, if
                it returns TRUE.
===========================================================================*/
extern boolean wl1_hs_rach_srchcr_cell_resel_allowed( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_HS_RACH_EDCH_RESRC_ALLOCATED

DESCRIPTION   This function returns TRUE if the EDCH resource is
              already allocated. 
                                
DEPENDENCIES None

RETURN VALUE  Boolean 

SIDE EFFECTS   None
===========================================================================*/
extern  boolean wl1_hs_rach_edch_resrc_allocated(void);

#endif /*FEATURE_WCDMA_HS_RACH*/

#ifdef FEATURE_WCDMA_EFACH_MSIM
/*===========================================================================
FUNCTION      wl1_hsrach_qta_request

DESCRIPTION

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void wl1_hsrach_qta_request( wsub_id_e_type wsub_id, wenc_rach_action_enum_type rach_curr_action);

/*===========================================================================
FUNCTION      e_ul_hsrach_qta_in_isr

DESCRIPTION   This function is modified version of e_ul_sg_process_qta_in_isr
              which is just being used to check whether EUL in QTA gap or not.
	      Neither doing any functionality nor setting any variable, hence
	      function arguments are removed.

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern boolean e_ul_hsrach_qta_in_isr( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION	  WL1_HSRACH_POST_QTA_ACTION

DESCRIPTION: This function will get QTA action.

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void wl1_hsrach_post_qta_action(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_HSRACH_SYNC_PROCEDURE_CNTR_UPDATE

DESCRIPTION: SYNC-AA procedure runs for 40ms and a counter is maintained to
             see how many more UL-DPCH TTI remaining to complete.
             This variable will be used to check whether QTA overlaps with
             SYNC-AA procedure or not.

DEPENDENCIES NONE

RETURN VALUE NONE

SIDE EFFECTS NONE
===========================================================================*/
extern void wl1_hsrach_sync_procedure_cntr_update(wsub_id_e_type wsub_id,
  wl1_ul_carr_id_enum_type carr_id);

/*===========================================================================
FUNCTION      wl1_hsrach_start_unlock_by_timer_for_tune_away

DESCRIPTION: This function, starts a timer that gets expired 78 ms prior to LTA

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void wl1_hsrach_start_unlock_by_timer_for_tune_away( wsub_id_e_type wsub_id, uint32 unlock_by_sclk);


/*===========================================================================
FUNCTION      wl1_hsrach_unlock_by_timer_expired_cb

DESCRIPTION: This function will get called once hsrach_start_unlock_by_timer expires

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void wl1_hsrach_unlock_by_timer_expired_cb(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION      wl1_hsrach_si0_transmission_before_lta

DESCRIPTION: This function makes sure that SI=0 gets transmitted beore stopping PDU building.

DEPENDENCIES  NONE

RETURN VALUE  None

SIDE EFFECTS  NONE
===========================================================================*/
extern void wl1_hsrach_si0_transmission_before_lta( wsub_id_e_type wsub_id );

#endif /* FEATURE_WCDMA_EFACH_MSIM */
#endif

