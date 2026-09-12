/*=========================================================================== RRC  WCDMA  FREE FLOATING MANAGER   FILE
DESCRIPTION

  This file contains RRC FREE FLOATING  procedure

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2009 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcfreefloating.c_v   1.25   19 Jun 2002 10:01:34   kpatil  $
$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcfreefloating.c#2 $    $DateTime: 2020/07/03 03:22:23 $ $ $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/16/11   gv      Fixed compilation issue
12/12/11   gv      Added code for RRC free floating task
===========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#include "rex.h"
#include "msg.h"
#include "err.h"

#include "tdsrrcwrm.h"

#include "tdsrrcsigs.h"
#include "tdsrrctmr.h"

#include "tdsrrcscmgr.h"
#include "tdsrrcdata_v.h"
#include "tdsrrccmdi.h"
#include "tdsrrccspi.h"

#include "tdsrrcmmif.h"
#include "tdsrrc_rr_if.h"
#include "tdsl1rrcif.h"
#include "tdsl1mtask.h"
#include "tdsrrcfreefloating.h"
//#include "l1task.h"


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
  tdsrrc_cmd_e_type tdsrrc_expected_cmd = TDSRRC_MAX_CMDS;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


typedef struct
{
  /* Command header common for all commands */
  tdsrrc_cmd_type    *cmd_ptr;

  boolean keep_waiting;
}tdsrrc_ff_wait_type;

extern void tdsl1_put_ff_cmd(tdsl1_ext_cmd_type* cmd_ptr);

/* -----------------------------------------------------------------------
** Fuction Definitions
** ----------------------------------------------------------------------- */

/*===========================================================================

FUNCTION                RRCFF_SET_EXPECTED_CMD_ID

DESCRIPTION
                        This function sets the expected cmd id to be received
                        in response to the requested high priority command

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None
===========================================================================*/
void tdsrrcff_set_expected_cmd_id( tdsrrc_cmd_e_type cmd_id)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCFF:Set tdsrrc_expected_cmd = %d",tdsrrc_expected_cmd);
  tdsrrc_expected_cmd = cmd_id;
}

/*===========================================================================

FUNCTION                RRCFF_CHECK_FOR_L1_CMDS

DESCRIPTION
                        This function checks the L1 commands received by RRC (when waiting 
                        for response to high priority command).Process the L1 command and 
                        return the command process status

DEPENDENCIES
                        None

RETURN VALUE
                        Returns the command process status

SIDE EFFECTS
                        None

===========================================================================*/
tdsrrc_ff_wait_type tdsrrcff_check_for_l1_cmds( void)
{

  tdsrrc_cmd_type *cmd_ptr;
  tdsrrc_ff_wait_type cmd_process_status;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  while((cmd_ptr = q_get(&tdsrrc_l1_ff_cmd_q)) != NULL)
  {
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case TDSRRC_CPHY_CHECK_SLEEP_REQ:
        tdsrrcff_check_l1_sleep_req(cmd_ptr);
        tdsrrc_free_cmd_buf(cmd_ptr);
        break;
      default:
        if(tdsrrc_expected_cmd == cmd_ptr->cmd_hdr.cmd_id)
        {
          cmd_process_status.keep_waiting = FALSE;
          cmd_process_status.cmd_ptr = cmd_ptr;
        }
        else
        {
          ERR_FATAL("Unexpected Message %d",cmd_ptr->cmd_hdr.cmd_id,0,0);
        }
        break;
    }
  }
  return cmd_process_status;
}

/*===========================================================================

FUNCTION                RRCFF_WAIT_HIGH_PRIORITY_RSP

DESCRIPTION
                        This function is the wait-loop after RRC posts a 
                        high priority request. It continues to wait till the
                        expected response for the requested command is received

DEPENDENCIES
                        None
 
RETURN VALUE
                        Returns the command process status

SIDE EFFECTS
                        None

===========================================================================*/
tdsrrc_ff_wait_type tdsrrcff_wait_high_priority_rsp( void)
{
  rex_sigs_type sigs;             /* hold signals from rex_wait() */

  boolean keep_waiting = TRUE;

  tdsrrc_ff_wait_type cmd_process_status;
  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  do
  {                     /* until we get the specified signal */
    sigs = rex_wait( TDSRRC_L1_FF_CMD_Q_SIG | TDSRRC_RPT_TIMER_SIG);

    /* Need to kick the watchdog */
    if( sigs & TDSRRC_RPT_TIMER_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RPT_TIMER_SIG );
      tdsrrc_dog_report_and_start_timer();
    }
    else if (sigs & TDSRRC_L1_FF_CMD_Q_SIG) 
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_L1_FF_CMD_Q_SIG );
      cmd_process_status = tdsrrcff_check_for_l1_cmds();
      keep_waiting = cmd_process_status.keep_waiting;
    }

  } while (keep_waiting == TRUE);
  return cmd_process_status;
}
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
/*===========================================================================

FUNCTION                RRCFF_L1_GET_DATA_FOR_UI_DISPLAY

DESCRIPTION
                        This function sends a command to L1 to get the RSSI, RSCP, Ec/Io
                        for use by the UI and Call Manager subsystems

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

===========================================================================*/
void tdsrrcff_l1_get_data_for_ui_display(tdsl1_ui_display_info_struct_type *l1_data_for_ui_display)
{
  tdsrrc_ff_wait_type  cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending TDSCPHY_DATA_FOR_UI_DISP_REQ",0,0,0);
  }

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSCPHY_DATA_FOR_UI_DISP_REQ;

  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(RRC_CPHY_DATA_FOR_UI_DISP_CNF);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();

  *l1_data_for_ui_display = cmd_process_status.cmd_ptr->cmd.data_for_ui_disp_cnf;

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
}

#endif

/*===========================================================================

FUNCTION                RRCFF_L1_GET_RSSI

DESCRIPTION
                        This function sends a command to L1 to get the Received 
                        Signal Strength Indication for use by the UI and Call 
                        Manager subsystems

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

 ===========================================================================*/
void tdsrrcff_l1_get_rssi(tdsl1_rssi_info_struct_type *rssi_data)
{
  tdsrrc_ff_wait_type  cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending TDSCPHY_RSSI_REQ",0,0,0);
  }

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSCPHY_RSSI_REQ;

  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_RSSI_CNF);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  *rssi_data = cmd_process_status.cmd_ptr->cmd.rssi_cnf;

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
}

/*===========================================================================

FUNCTION                RRCFF_L1_IS_S_CRITERIA_MET

DESCRIPTION
                        This function sends a command to L1 to compute S criteria
                        based on the values provided by RRC

DEPENDENCIES
                        None

RETURN VALUE
                        TRUE: If S Criteria is met
                        FALSE: Otherwise

SIDE EFFECTS
                        None

 ===========================================================================*/
boolean tdsrrcff_l1_is_s_criteria_met( /*int16 q_qualmin, */ int16 q_rxlevmin, int16 ue_tx_max_pwr)
{
  tdsrrc_ff_wait_type  cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;
  boolean ret_val;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_CHECK_S_CRIT_REQ",0,0,0);
  }

  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_CHECK_S_CRIT_REQ;
  /*l1_cmd_ptr->cmd.s_criteria_req.q_qualmin = q_qualmin;*/  /*need to sync with L1's updated change*/
  l1_cmd_ptr->cmd.s_criteria_req.q_rxlevmin = q_rxlevmin;
  l1_cmd_ptr->cmd.s_criteria_req.ue_tx_max_pwr = ue_tx_max_pwr;

  /* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_CHECK_S_CRIT_CNF);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  ret_val = cmd_process_status.cmd_ptr->cmd.s_criteria_cnf.status;

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);

  return(ret_val);
}

/*===========================================================================

FUNCTION                RRCFF_CHECK_L1_SLEEP_REQ

DESCRIPTION
                        This function processes the sleep request command sent by L1
                        in every DRX cycle to determine if L1 should go to sleep or not

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

 ===========================================================================*/
void tdsrrcff_fail_l1_sleep_req(tdsrrc_cmd_type *cmd_ptr)
{
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  l1_cmd_ptr = tdsl1_get_cmd_buf();

  l1_cmd_ptr->cmd_hdr.cmd_id = TDSCPHY_CHECK_SLEEP_RSP;

  l1_cmd_ptr->cmd.l1_sleep_rsp.go_to_sleep = TRUE;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtog_bplmn_in_progress = FALSE;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtow_bplmn_in_progress = FALSE;

  tdsl1_put_ff_cmd(l1_cmd_ptr);
}

/*===========================================================================

FUNCTION                RRCFF_CHECK_L1_SLEEP_REQ

DESCRIPTION
                        This function processes the sleep request command sent by L1
                        in every DRX cycle to determine if L1 should go to sleep or not

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

 ===========================================================================*/
void tdsrrcff_check_l1_sleep_req(tdsrrc_cmd_type *cmd_ptr)
{
  uint32 timer_val;
  boolean is_new_drx_cycle;
  boolean go_to_sleep;
  boolean wtog_bplmn_in_progress;
  boolean wtow_bplmn_in_progress;

  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  timer_val = cmd_ptr->cmd.l1_sleep_req.timer_val;

  is_new_drx_cycle = cmd_ptr->cmd.l1_sleep_req.is_new_drx_cycle;

  go_to_sleep = tdsrrc_can_l1_go_to_sleep(timer_val,&wtog_bplmn_in_progress
                                          ,&wtow_bplmn_in_progress,is_new_drx_cycle
                                         );

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_CHECK_L1_SLEEP_REQ",0,0,0);
  }

  l1_cmd_ptr->cmd_hdr.cmd_id = TDSCPHY_CHECK_SLEEP_RSP;

  l1_cmd_ptr->cmd.l1_sleep_rsp.go_to_sleep = go_to_sleep;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtog_bplmn_in_progress = wtog_bplmn_in_progress;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtow_bplmn_in_progress = wtow_bplmn_in_progress;

  tdsl1_put_ff_cmd(l1_cmd_ptr);
}

/*===========================================================================

FUNCTION                RRCFF_L1_GET_PERIODIC_INTER_RAT_MSMT_RESULTS

DESCRIPTION             This function send a high priority cmd to L1 to get the 
                        additonal meas of IRAT type which can be appended to 
                        meas reporting going in UL.

DEPENDENCIES
                        None

RETURN VALUE
                        TRUE: if additional meas result is present
                        FALSE  if not


SIDE EFFECTS
                        None

===========================================================================*/

boolean tdsrrcff_l1_get_periodic_inter_rat_msmt_results
(
  uint32  meas_id,
  tdsl1_inter_sys_meas_result_struct_type  *p_msmtres
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending L1 cmd",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = TDSL1_INTER_SYS;

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }
  memscpy((void *)p_msmtres, sizeof(tdsl1_inter_sys_meas_result_struct_type), 
          (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.inter_sys_rpt.measurements), sizeof(tdsl1_inter_sys_meas_result_struct_type));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
  if(p_msmtres->num_msmt_results == 0)
    return FALSE;
  else
    return TRUE;
}

/*===========================================================================

FUNCTION                RRCFF_L1_GET_ADDITIONAL_INTRA_FREQ_MSMT_RESULTS

DESCRIPTION
                        This function send a high priority cmd to L1 to get the 
                        additonal meas of Intra-freq type which can be appended to 
                        meas reporting going in UL.


DEPENDENCIES
                        None

RETURN VALUE
                        TRUE :if additional meas result is present
                        FALSE: if not present


SIDE EFFECTS
                        None

===========================================================================*/

boolean tdsrrcff_l1_get_additional_intra_freq_msmt_results
(
  uint32  meas_id,
  tdsl1_intra_freq_meas_rpt_struct_type  *p_msmtres
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending L1 cmd",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = TDSL1_INTRA_FREQ;

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  memscpy((void *)p_msmtres, sizeof(tdsl1_intra_freq_meas_rpt_struct_type),
         (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.intra_f_rpt), sizeof(tdsl1_intra_freq_meas_rpt_struct_type));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
  if(p_msmtres->cell_meas_result_incl == FALSE)
    return FALSE;
  else
    return TRUE;
}

/*===========================================================================

FUNCTION                RRCFF_L1_GET_ADDITIONAL_INTERF_MSMT_RESULTS

DESCRIPTION
                        This function send a high priority cmd to L1 to get the 
                        additonal meas of Inter-freq type which can be appended to 
                        meas reporting going in UL.


DEPENDENCIES
                        None

RETURN VALUE
                        TRUE:if additional meas result is present
                        FALSE:if not present


SIDE EFFECTS
                        None

===========================================================================*/

boolean tdsrrcff_l1_get_additional_interf_msmt_results
(
  uint32  meas_id,
  tdsl1_inter_freq_meas_rpt_struct_type  *p_msmtres
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending L1 cmd",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = TDSL1_INTER_FREQ;

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  memscpy((void *)p_msmtres, sizeof(tdsl1_inter_freq_meas_rpt_struct_type),
         (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.inter_f_rpt), sizeof(tdsl1_inter_freq_meas_rpt_struct_type));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
  if(p_msmtres->meas_results_included == FALSE)
    return FALSE;
  else
    return TRUE;
}

/*===========================================================================

FUNCTION                RRCFF_L1_GET_ADDITIONAL_INTERNAL_MSMT_RESULTS

DESCRIPTION
                        This function send a high priority cmd to L1 to get the 
                        additonal meas of UE-Internal type which can be appended to 
                        meas reporting going in UL.


DEPENDENCIES
                        None

RETURN VALUE
                        TRUE:if additional meas result is present
                        FALSE: if not present
                        

SIDE EFFECTS
                        None

===========================================================================*/

boolean tdsrrcff_l1_get_additional_internal_msmt_results
(
  uint32  meas_id,
  tdsl1_ue_internal_meas_rpt_struct_type  *p_msmtres
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending L1 cmd",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = TDSL1_UE_INTERNAL;

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }
  memscpy((void *)p_msmtres, sizeof(tdsl1_ue_internal_meas_rpt_struct_type),
         (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.ue_internal_meas_rpt), sizeof(tdsl1_ue_internal_meas_rpt_struct_type));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
  if(p_msmtres->cell_measured_results_included == FALSE)
    return FALSE;
  else
    return TRUE;
}
#ifdef FEATURE_TDSCDMA_PLI_NMR_UTRAN
/*===========================================================================

FUNCTION                RRCFF_L1_GET_UTRAN_NMR_INFO

DESCRIPTION             This function is called to get
                        6best intra/inter-freq/inter-rat cells in response to a
                        NMR request from upper layer.


DEPENDENCIES
                        None

RETURN VALUE
                        TRUE: if NMR info is present.
                        FALSE: if not present


SIDE EFFECTS
                        None

===========================================================================*/

boolean tdsrrcff_l1_get_utran_nmr_info
(
  tdsl1_meas_utran_nmr_info_union_type  *l1_meas_utran_nmr_info,
  tdsl1_rrc_utran_nmr_report_type  report_type
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;
  boolean status;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_SETUP_REQ",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_NMR_INFO_REQ;

  l1_cmd_ptr->cmd.utran_nmr_info_req.report_type = report_type;

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_UTRAN_NMR_INFO_CNF);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  memscpy((void *)l1_meas_utran_nmr_info, sizeof(tdsl1_meas_utran_nmr_info_union_type),
         (void *)(cmd_process_status.cmd_ptr->cmd.nmr_cnf.l1_meas_utran_nmr_info), sizeof(tdsl1_meas_utran_nmr_info_union_type));

  status =  cmd_process_status.cmd_ptr->cmd.nmr_cnf.status;

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);

  return status;
}
#endif /*FEATURE_TDSCDMA_PLI_NMR_UTRAN*/
/*===========================================================================

FUNCTION                RRCFF_L1_PROCESS_IMMEDIATE_MEAS_REQ

DESCRIPTION             This function sends a req to L1 to get the 
                        intra-freq measurement report to append in RACH msg


DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

===========================================================================*/

void tdsrrcff_l1_process_immediate_meas_req
(
  tdsl1_immediate_meas_type_tmp  *immed_meas,
  tdsl1_intra_freq_rach_rpt_quan_struct_type  meas_quan
)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_SETUP_REQ",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_GET_IMM_MEAS_REQ;


  l1_cmd_ptr->cmd.imm_meas_req.meas_quan = meas_quan;
  l1_cmd_ptr->cmd.imm_meas_req.meas_type = TDSL1_INTRA_FREQ;
  

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_GET_IMM_MEAS_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  memscpy(immed_meas, sizeof(tdsl1_immediate_meas_type_tmp),
          cmd_process_status.cmd_ptr->cmd.imm_meas_cnf.immed_meas, sizeof(tdsl1_immediate_meas_type_tmp));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
}

/*===========================================================================

FUNCTION                RRCFF_L1_PROCESS_IMMEDIATE_INTERF_MEAS_REQ

DESCRIPTION             This function sends a req to L1 to get the 
                        inter-freq measurement report to append in RACH msg


DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

===========================================================================*/

void tdsrrcff_l1_process_immediate_interf_meas_req( tdsl1_immediate_meas_type_tmp *immed_meas)
{
  tdsrrc_ff_wait_type cmd_process_status;
  tdsl1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_SETUP_REQ",0,0,0);
  }
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_GET_IMM_MEAS_REQ;


  l1_cmd_ptr->cmd.imm_meas_req.meas_type = TDSL1_INTER_FREQ;
  

/* Now send the primitive to L1 */
  tdsl1_put_ff_cmd(l1_cmd_ptr);

  tdsrrcff_set_expected_cmd_id(TDSRRC_CPHY_GET_IMM_MEAS_RSP);

  cmd_process_status = tdsrrcff_wait_high_priority_rsp();
  if(cmd_process_status.cmd_ptr == NULL)
  {
    ERR_FATAL("NULL Ptr when sending cmd_process_status",0,0,0);
  }

  memscpy(immed_meas, sizeof(tdsl1_immediate_meas_type_tmp),
         cmd_process_status.cmd_ptr->cmd.imm_meas_cnf.immed_meas, sizeof(tdsl1_immediate_meas_type_tmp));

  tdsrrc_free_cmd_buf(cmd_process_status.cmd_ptr);
}

#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */
