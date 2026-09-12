#ifndef L1MCMD_H
#define L1MCMD_H
/*===========================================================================

              L 1 M    C O M M A N D    H A N D L E R S

GENERAL DESCRIPTION
  This file contains function declarations of the L1 Manager external
  command handlers.
  
EXTERNALIZED FUNCTIONS

l1m_cphy_capability_req_cmd
l1m_cphy_idle_req_cmd
l1m_cphy_acq_req_cmd
l1m_cphy_cell_parm_req_cmd
l1m_cphy_hard_handoff_req_cmd
l1m_bch_schedule_cmd
l1m_cphy_drx_req_cmd
l1m_cphy_ciphering_key_req_cmd
l1m_cphy_integrity_key_req_cmd
l1m_cphy_integrity_key_mauc_req_cmd
l1m_cphy_ul_tx_control_req_cmd
l1m_cphy_action_cancel_req_cmd
l1m_cphy_cell_selection_req_cmd
l1m_cphy_cell_reselection_rsp_cmd
l1m_cphy_immediate_reselection_req_cmd
l1m_cphy_measurement_req_cmd
l1_process_immediate_meas_req
l1m_cphy_immediate_reselection_req_cmd
l1m_cphy_freq_scan_req_cmd
l1m_cphy_deact_req_cmd
l1m_cphy_stop_req_cmd
l1m_cphy_suspend_req_cmd

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2000 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/l1mcmd.h_v   1.16   18 Jul 2002 16:43:50   shawnm  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/l1mcmd.h#4 $ $DateTime: 2020/07/15 00:02:21 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
6/22/20    vb      FR61612: Cell Deprioritization fake cell
01/10/20   ac      Mini dump framework checkin
08/16/17   ss     Changes for LTA timeline optimization
08/08/15   db      Dont reset l1m_suspended_w2g_bplmn in suspended cmd processor 
                   while handling ext cmds that dont cause state transition
02/28/15   rsr     W + W Phase 2 Checkin.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/11/12   vs      MBMS feature cleanup.
02/10/12   rgn     Nikel feature cleanup
01/28/12   vs      Nikel feature cleanup.
05/03/10   hk      Fixed compiler warnings
01/19/09   stk     Added support for R7 spec CR High Mobility Indicator( spec CR 2948(RP-060712))
10/03/08   hk      Bringing in 7k Mailine fixes
02/20/06   gv      Declared extern enum l1m_suspend_cause to hold the cause for
				   which WL1 is suspended.
02/20/06   gv	   Declared extern boolean l1m_suspended_w2g_bplmn
				   which is used to indicate whether WL1 was suspended for 
				   W2G BPLMN.
11/08/05  gv/nd    Added code for WtoW BPLMN feature.
07/06/05   vp      Code changes to support new DL sync parms received in a 
                   CPHY_UPD_DL_SYNC_PARMS message. RRC sends this message
                   when it has received a UTRAN Mobility Information message.
05/04/05   sk/sh   Added T-barred Feature support. Support for new cmd CELL_BAR_REQ
02/27/05   scm     Move BMC related prototypes into dlstates.h.
02/17/05   scm     Support commands from BMC (Broadcast Multicast Control).
12/01/04   scm     Fix compiler warning and comment.
11/16/04   vb      Added code to handle hfn rollover update before and after HHO. 
                   Merged the function l1m_compute_suspend_duration to handle 
                   rollover update after HHO function
09/09/03   m       Modifications to support Inter Freq HHO
03/13/03   scm     Move l1m_compute_suspend_duration() from l1msetup.h to here.
11/06/02   scm     Change l1m_cphy_stop_req_cmd() comment for L1M_DEEP_SLEEP support.
10/07/02   scm     SUSPEND with activation time support.
09/09/02   scm     Change l1m_cphy_suspend_req_cmd() to take l1_ext_cmd_type*
                   instead of l1_suspend_cmd_type* for SUSPEND with act time.
08/30/02   scm     More work on SUSPEND/RESUME.
08/21/02   scm     Add l1m_cphy_mac_ul_config_cnf() and l1m_cphy_mac_dl_config_cnf().
08/07/02   scm     Adding l1m_cphy_mac_config_cnf for MAC activation time support.
07/18/02   scm     Initial pass at SUSPEND/RESUME support, partly functional.
05/18/02   sh      Updated documentation for l1m_cphy_drx_req_cmd().
05/06/02   scm     Change FEATURE_WCDMA_L1_START_STOP to
                   FEATURE_CHEETAH_BASELINE, to match RRC.
05/03/02   scm     Implementing support of START/STOP primitives from RRC.
                   Code wrapped in FEATURE_WCDMA_L1_START_STOP define.
02/12/02   sh      Changed proto for l1m_cphy_action_cancel_req_cmd().
                   Added FEATURE_L1_ACT_TIME flag.
10/19/01   sh      Added l1m_cphy_deact_req_cmd() for L1 deactivate cmd.
08/31/01   sh      Added l1m_cphy_freq_scan_req_cmd() to support freq scan.
08/10/01   sh      Removed l1m_cphy_immediate_meas_req_cmd() and
                   l1m_cphy_aset_update_req_cmd().
03/29/01   sh      Removed l1m_cphy_cell_transition_req_cmd().
01/03/01   sh      Removed l1m_cphy_immediate_cell_eval_req_cmd(),
                   l1_cphy_sfn_req(), l1m_cphy_set_sfn_req_cmd().
                   l1m_set_cfn_req_cmd().
12/21/00   sh      Added prototype for function
                   l1m_cphy_ul_tx_control_req_cmd()
11/16/00   sh      Moved l1m_cphy_setup_req_cmd() to l1msetup.h.
09/25/00   yus     Created file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "l1rrcif.h"
#include "srchset.h"

typedef struct
{
  /* A temporary variable to keep track whether the L1 is suspended for W2G/W2L BPLMN*/
  boolean l1m_suspended_w2x_bplmn;
  /* A variable used to save the cause for suspend to be used later in the code for some
** search related clean up.*/
  l1_suspend_resume_cause_enum_type l1m_suspend_cause;
  /* Indicates that L1 CPHY_SETUP is over and waiting for MAC */
  boolean l1_phychan_waiting_for_mac;
  /*flag to indicate whether or not L1 is suspended due to HHO*/
  boolean l1_suspended_due_to_hho;
   /*If set W has to perform fast SUSPEND operations */
  boolean enable_fast_suspend;
  boolean clean_req_fast_suspend;
}l1mcmd_ext_cntrl_params_type;

extern l1mcmd_ext_cntrl_params_type l1mcmd_ext_cntrl_params[WCDMA_NUM_SUBS];


typedef struct
{
  tlm_tfn_struct_type l1_wrc_at_start_of_handover;
  /* For saving sequence number to identify SUSPEND command on confirmation. */
  uint8 l1m_suspend_cmd_seq_num;
}l1mcmd_int_cntrl_params_type;
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/



/******************************************************************************
*******************************************************************************
                       EXTERNAL L1M COMMAND HANDLERS
*******************************************************************************
******************************************************************************/
/*===========================================================================
FUNCTION        L1M_CPHY_CAPABILITY_REQ_CMD

FILE NAME       

DESCRIPTION     This funtion handles the CPHY_CAPABILITY_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_capability_req_cmd
(
  /* command parameters struct pointer */
  l1_cap_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_IDLE_REQ_CMD_PRE_START

FILE NAME       

DESCRIPTION     This function suspends all periodic ops before proceeding with CPHY processing if fast suspend feature is enabled

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_idle_req_cmd_pre_start(l1_idle_cmd_type * cmd,wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        L1M_CPHY_IDLE_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_IDLE_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_idle_req_cmd
(
  /* command parameters struct pointer */
  l1_idle_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_DEACT_REQ_CMD

FILE NAME

DESCRIPTION     This function handles the CPHY_DEACTIVATE_REQ primitive

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void l1m_cphy_deact_req_cmd
(
  /* command parameters struct pointer */
  l1_deactivate_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_STOP_REQ_CMD

DESCRIPTION
  This function handles the CPHY_STOP_WCDMA_MODE_REQ primitive from RRC.
  If WCDMA is no longer the active RAT, L1 goes to L1M_STOPPED state.  If
  service has not been found for some period of time, L1 goes to L1M_DEEP_SLEEP
  state.  In both cases WCDMA L1 ceases all meaningful processing.  However
  in L1M_DEEP_SLEEP the hardware is also configured for power conservation mode.
  For both cases a START command causes initialization and a return to IDLE.

PARAMETERS
  cmd->cause - L1_RAT_CHANGE: Means WCDMA no longer the active RAT.
             \ L1_DEEP_SLEEP: Means no service.  Enter low power mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Changes state to L1M_STOPPED or L1M_DEEP_SLEEP.
===========================================================================*/
extern void l1m_cphy_stop_req_cmd
(
  /* command parameters struct pointer */
  l1_stop_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_HANDLE_HFN_ROLLOVER_BEFORE_HHO

FILE NAME

DESCRIPTION     This function update the current CFN to mac for 
                hfn computation and it store the current sfn, which 
                will be used for computation of duration of th 
                suspension state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void l1m_handle_hfn_rollover_before_hho( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_HANDLE_HFN_ROLLOVER_AFTER_HHO

FILE NAME

DESCRIPTION     This function processes suspension duration
                and send the rollover hfn to mac if necessary by 
                looking at the input argument.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void l1m_handle_hfn_rollover_after_hho(boolean send_rollover_to_mac, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_SUSPEND_REQ_CMD

DESCRIPTION
  We got the CPHY_SUSPEND_WCDMA_MODE_REQ primitive from RRC, meaning we're
  attempting an inter-RAT handover or cell reselection.  Check if it has an
  activation time.  If so, schedule it for later.  If not, suspend now.

PARAMETERS
  l1_ext_cmd_type *cmd_ptr - Pointer to RRC SUSPEND command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  May change state to L1M_SUSPENDED and shut down WCDMA L1, if the
  activation time is now.
===========================================================================*/
extern void l1m_cphy_suspend_req_cmd
(
  /* command parameters struct pointer */
  l1_ext_cmd_type *cmd_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_ACQ_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_ACQ_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_acq_req_cmd
(
  /* command parameters struct pointer */
  l1_acq_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_FREQ_SCAN_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_FREQ_SCAN_REQ primitive.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_freq_scan_req_cmd
(
  /* command parameters struct pointer */
  l1_freq_scan_cmd_type *cmd
, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_REL7
extern void l1m_cphy_upd_ota_parm_cmd
(
  /*command parameters struct pointer */
  l1_upd_ota_parms_ind_cmd_type *cmd
, wsub_id_e_type wsub_id);
#endif
/*===========================================================================
FUNCTION        L1M_CPHY_CELL_PARM_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_CELL_PARM_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_cell_parm_req_cmd
(
  /* command parameters struct pointer */
  l1_cell_parm_cmd_type *cmd
, wsub_id_e_type wsub_id);    

/*===========================================================================
FUNCTION        L1M_CPHY_HARD_HANDOFF_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_HARD_HANDOFF_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_hard_handoff_req_cmd
(
  l1_hard_handoff_req_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_BCH_SCHEDULE_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_BCH_SCHEDULE_REQ primitive.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_bch_schedule_cmd
(
  l1_bch_schedule_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_DRX_REQ_CMD

DESCRIPTION
  This function handles the CPHY_DRX_REQ primitive. This function will
  first send a command to searcher to search SYNC_NSET and ASYNC_NSET
  neighbor cells and sort them. Once the searcher reports search/sort
  done, L1M will proceed to send the DRX command to the DRX module.

DEPENDENCIES    
  None
 
RETURN VALUE    
  None
 
SIDE EFFECTS    
  Search heartbeat is turned off until the nset srch/sort is done.
	DRX Command is saved into l1_pending_drx_cmd.
===========================================================================*/
extern void l1m_cphy_drx_req_cmd
(
  /* command parameters struct pointer */
  l1_drx_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_CIPHERING_KEY_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_CIPHERING_KEY_REQ primitive.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void l1m_cphy_ciphering_key_req_cmd
(
  l1_ciphering_key_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_MEASUREMENT_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_MEASUREMENT_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_measurement_req_cmd
(
  /* command parameters struct pointer */
  l1_meas_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_CELL_SELECTION_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_CELL_SELECTION_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_cell_selection_req_cmd
(
  /* command parameters struct pointer */
  l1_cell_selec_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_INTEGRITY_KEY_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_INTEGRITY_KEY_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_integrity_key_req_cmd
(
  /* command parameters struct pointer */
  l1_intg_key_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_INTEGRITY_KEY_MAUC_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_INTEGRITY_KEY_MAUC_REQ
                primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_integrity_key_mauc_req_cmd
(
  l1_intg_key_mauc_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_IMMEDIATE_RESELECTION_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_IMMEDIATE_RESELECTION_REQ
                primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_immediate_reselection_req_cmd
(
  l1_immediate_reselec_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_CELL_RESELECTION_RSP

FILE NAME       

DESCRIPTION     This function handles the CPHY_CELL_RESELECTION_RSP primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_cell_reselection_rsp_cmd
(
  l1_cell_reselect_rsp_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_ACTION_CANCEL_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_ACTION_CANCEL_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_act_time_cancel_req_cmd
(
  l1_act_time_cancel_cmd_type *cmd
, wsub_id_e_type wsub_id);

#ifdef FEATURE_MAC_ACTIVATION_TIME
/*=========================================================================

FUNCTION l1m_cphy_mac_ul_config_cnf

DESCRIPTION
  This function handles the CPHY_MAC_UL_CONFIG_CNF primitive from MAC,
  providing activation time support to MAC and RRC.  L1 signals MAC at the
  activation time to process a UL config request from RRC.  Once MAC finishes,
  it responds with the CPHY_MAC_UL_CONFIG_CNF primitive, which includes a
  completion status.  L1 then sends the RRC_CPHY_SETUP_CNF primitive to
  RRC, which includes a combined L1 and MAC config status, if both UL and
  DL configurations are complete.

DEPENDENCIES
  None.

PARAMETERS
  l1_mac_config_cnf_type *cmd - Structure containing only the boolean
    "mac_config_status".  TRUE means success.  FALSE means failure.

RETURN VALUE
  None.

SIDE EFFECTS
  Possibly sends RRC_CPHY_SETUP_CNF primitive to RRC.

=========================================================================*/
extern void l1m_cphy_mac_ul_config_cnf
(
  l1_mac_config_cnf_type *cmd
, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION l1m_cphy_mac_dl_config_cnf

DESCRIPTION
  This function handles the CPHY_MAC_DL_CONFIG_CNF primitive from MAC,
  providing activation time support to MAC and RRC.  L1 signals MAC at the
  activation time to process a DL config request from RRC.  Once MAC finishes,
  it responds with the CPHY_MAC_DL_CONFIG_CNF primitive, which includes a
  completion status.  L1 then sends the RRC_CPHY_SETUP_CNF primitive to
  RRC, which includes a combined L1 and MAC config status, if both UL and
  DL configurations are complete.

DEPENDENCIES
  None.

PARAMETERS
  l1_mac_config_cnf_type *cmd - Structure containing only the boolean
    "mac_config_status".  TRUE means success.  FALSE means failure.

RETURN VALUE
  None.

SIDE EFFECTS
  Possibly sends RRC_CPHY_SETUP_CNF primitive to RRC.

=========================================================================*/
extern void l1m_cphy_mac_dl_config_cnf
(
  l1_mac_config_cnf_type *cmd
, wsub_id_e_type wsub_id);
#endif  /* #ifdef FEATURE_MAC_ACTIVATION_TIME */


/*===========================================================================
FUNCTION        L1M_CPHY_UL_TX_CONTROL_REQ_CMD

FILE NAME       

DESCRIPTION     This function handles the CPHY_UL_TX_CONTROL_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_ul_tx_control_req_cmd
(
  l1_ul_tx_ctrl_req_type *cmd
, wsub_id_e_type wsub_id);

/*==============================================================================
FUNCTION        L1M_SUSPEND

DESCRIPTION
  Unconditionally suspends L1

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  May change state to L1M_SUSPENDED
==============================================================================*/
void l1m_suspend( wsub_id_e_type wsub_id);

/*==============================================================================
FUNCTION        L1M_PRE_SUSPEND

DESCRIPTION
 Any periodic operations if needs to be suspended is done before actually performing the cleanup

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
==============================================================================*/
void l1m_pre_suspend(wsub_id_e_type wsub_id);

/*==============================================================================
FUNCTION        L1M_CPHY_CELL_BAR_REQ_CMD

DESCRIPTION
 Handler for CELL_BAR_REQ from RRC

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
==============================================================================*/
extern void l1m_cphy_cell_bar_req_cmd 
(
 l1_cell_bar_cmd_type * cmd
, wsub_id_e_type wsub_id); 

/*==============================================================================
FUNCTION        L1M_CPHY_CELL_DEPRIORITIZE_REQ_CMD

DESCRIPTION
 Handler for CELL_DEPRIORITIZE_REQ from RRC

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
==============================================================================
*/

extern void l1m_cphy_cell_depri_req_cmd
(l1_depri_parm_type * cmd
 , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_UPD_DL_SYNC_PARMS_CMD

FILE NAME        l1mcmd.c

DESCRIPTION     This function is the processor for message CPHY_UPD_DL_SYNC_PARMS.
               
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void l1m_cphy_upd_dl_sync_parms_cmd(l1_upd_dl_sync_parms_cmd_type *cmd, wsub_id_e_type wsub_id);

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
/*===========================================================================
FUNCTION     L1M_CPHY_GO_TO_SLEEP_IND

DESCRIPTION


DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_cphy_go_to_sleep_ind( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_BPLMN_CPHY_SETUP_REQ_CMD

DESCRIPTION
  THis function handles the CPHY_SETUP_REQ from RRC when the L1 is in 
  PCH_BPLMN state. This set up request from RRC triggers the L1 to set up 
  a neighbor PCCPCH. RRC currently sends L1 a set up request for S_PCCPCH
  and this command handler internally converts it into a PCCPCH_N.


DEPENDENCIES handle pccpch setup req in BPLMN
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_bplmn_cphy_setup_req_cmd(l1_ext_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

#endif


/*===========================================================================
FUNCTION        L1M_MEASUREMENT_IS_FOR_INTERNAL_MEAS

FILE NAME       

DESCRIPTION     This function determines if the CPHY_MEASUREMENT_REQ received
                is for internal measurements or not

DEPENDENCIES    

RETURN VALUE    
             boolean
SIDE EFFECTS    
===========================================================================*/
extern boolean l1m_measurement_is_for_internal_meas(l1_meas_cmd_type *cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_MEASUREMENT_IS_FOR_QUALITY_MEAS

FILE NAME       

DESCRIPTION     This function determines if the CPHY_MEASUREMENT_REQ received
                is for quality measurements or not

DEPENDENCIES    

RETURN VALUE    
             boolean
SIDE EFFECTS    
===========================================================================*/
extern boolean l1m_measurement_is_for_quality_meas(l1_meas_cmd_type *cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_GET_HANDOVER_SUSPEND_TIME

DESCRIPTION  This function computes suspended time during handover in terms of frames

DEPENDENCIES 
  None.

INPUT PARAMS
             psc: psc of the cell under consideration
             freq: frequency of the cell under consideration
OUTPUT PARAMS
             suspended_sfn: the structure that holds the computed suspended sfn of the
                                             channel
             curr_sfn: Current sfn 
             susended_time_in_frames: computed suspended time in frames
RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_get_handover_suspended_time( uint16 psc,  uint16 freq, uint16 *suspended_sfn, 
  uint16 *curr_sfn, uint16 *suspended_time_in_frames, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_GET_SUSPENDED_CMD_SEQ_NUM

DESCRIPTION  This function returns suspended command seq number

DEPENDENCIES 
  None.

INPUT PARAMS

OUTPUT PARAMS

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 l1m_get_suspend_cmd_seq_num( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_GET_SUSPEND_CAUSE

DESCRIPTION  This function returns L1 suspend cause

DEPENDENCIES 
  None.

INPUT PARAMS

OUTPUT PARAMS

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern l1_suspend_resume_cause_enum_type l1m_get_suspend_cause( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        L1M_CPHY_START_REQ_CMD

FILE NAME       

DESCRIPTION     This funtion handles the CPHY_CAPABILITY_REQ primitive

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void l1m_cphy_start_req_cmd(l1_ext_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     l1m_is_fast_w_suspend_enabled

DESCRIPTION  This function tells whether W is getting SUSPENDED for LTA

DEPENDENCIES 
  None.

INPUT PARAMS
 
RETURN VALUE
    TRUE if W is getting SUSPENDED for LTA 
    FALSE if W is getting SUSPENDED for any other reasons
    
SIDE EFFECTS
  None.
===========================================================================*/
extern boolean l1m_is_fast_w_suspend_enabled(wsub_id_e_type wsub_id);
extern boolean  l1m_is_this_cleanup_req_by_fast_suspend( wsub_id_e_type wsub_id);
extern void  l1m_set_cleanup_req_by_fast_suspend( wsub_id_e_type wsub_id);
extern void  l1m_reset_cleanup_req_by_fast_suspend( wsub_id_e_type wsub_id);


#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION        l1m_qsh_mdump_collect_sub

DESCRIPTION     Dump some structures

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
QSH_MDUMP_FN_ATTR void  l1m_cmd_qsh_mdump_collect_sub( wsub_id_e_type wsub_id);
#endif
#endif /* L1MCMD_H */
