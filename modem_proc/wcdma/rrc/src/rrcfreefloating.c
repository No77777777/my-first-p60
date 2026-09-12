/*=========================================================================== RRC  WCDMA  FREE FLOATING MANAGER   FILE
DESCRIPTION

  This file contains RRC FREE FLOATING  procedure

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcfreefloating.c_v   1.25   19 Jun 2002 10:01:34   kpatil  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcfreefloating.c#2 $    $DateTime: 2020/01/24 03:56:36 $ $ $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/20   sp      Mini dump phase-2 changes
05/04/16   sn      f3 density reduction changes
03/10/16   nr      Added local variable to hold return value of function rrcff_l1_get_utran_nmr_info
08/26/15   gv      Made changes to process the alternate/free floating sub command queue till 
                   it reaches NULL before exiting the loop
06/04/15   bc      Changes to fix compilation errors.
05/28/15   bc      Changes to share SIBDB and ACQDB among two SUBs in a W+W solution
02/19/15   sa      Made changes to enable the FREE FLOATING TASK feature for QTF
12/30/14   bc      Replacing ERR_FATAL with MSG_ERR0_FATAL, MSG_ERR1_FATAL, MSG_ERR2_FATAL.
05/28/14   vi      Made changes to add freq index in function  rrcff_l1_get_additional_intra_freq_msmt_results
05/07/14   vi      Made changes to use Mutex instead of enable/disable preemption under critical section to avoid race condition in FF cmds.
05/06/14   geg     Use safer versions of memcpy() and memmove()
04/02/14   vg      Made changes to Systematically log all the internal and external primitive handled in RRC and corresponding top level action.
01/29/14   sr      Made changes to update bplmn variables properly when RRC is wating for lock and
                   sleep request is receuved from L1
01/29/14   sr      Made changes to handle FF commands from L1 when waiting for lock
01/28/14   vi      Made changes to clear freefloating signal correctly if there are more than 2 freefloating commands in freefloating queue
04/15/13   as      Made changes to pass correct pointer to memcpy
04/09/13   gv      Fixed compilation issue
03/25/13   sn      Changes to replace timer based dog reporting with signal based reporting
02/06/12   gv      Made changes to use the right structure type while doing a memcpy
12/16/11   gv      Fixed compilation issue
12/12/11   gv      Added code for RRC free floating task
===========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "wcdma_variation.h"
#include "comdef.h"

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
#include "rex.h"
#include "msg.h"
#include "err.h"

#include "rrcwrm.h"

#include "rrcsigs.h"
#include "rrctmr.h"

#include "rrcscmgr.h"
#include "rrcdata_v.h"
#include "rrccmdi.h"
#include "rrccspi.h"
#include "rrcmmif.h"
#include "rrcrrif.h"
#include "l1rrcif.h"
#include "rrcfreefloating.h"
#include "l1task.h"
#include "rrcdispatcher.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "rrcsibdbi.h"
/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_DUAL_WCDMA 
LOCAL rrc_cmd_e_type expected_cmd_sim[MAX_RRC_STACKS] = {RRC_MAX_CMDS, RRC_MAX_CMDS};
#define expected_cmd expected_cmd_sim[WAS_ID_M]
#else
LOCAL rrc_cmd_e_type expected_cmd = RRC_MAX_CMDS;
#endif

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_DUAL_WCDMA 
extern  rrc_csp_wtog_bplmn_int_data_type  rrc_csp_wtog_bplmn_int_data_sim[MAX_RRC_STACKS];
#define rrc_csp_wtog_bplmn_int_data rrc_csp_wtog_bplmn_int_data_sim[WAS_ID_M]
#else
extern  rrc_csp_wtog_bplmn_int_data_type  rrc_csp_wtog_bplmn_int_data;
#endif

typedef struct
{
  /* Command header common for all commands */
  rrc_cmd_type    *cmd_ptr;

  boolean keep_waiting;
}rrc_ff_wait_type;

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
void rrcff_set_expected_cmd_id(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_e_type cmd_id)
{
  expected_cmd = cmd_id;
  WRRC_MSG1_HIGH("RRCFF:Set expected_cmd = 0x%x",expected_cmd);
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
rrc_ff_wait_type rrcff_check_for_l1_cmds(sys_modem_as_id_e_type wrrc_as_id)
{

  rrc_cmd_type *cmd_ptr;
  rrc_ff_wait_type cmd_process_status;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  while((cmd_ptr = q_get(&rrc_cmd_q.rrc_l1_ff_cmd_q)) != NULL)
  {
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case RRC_CPHY_CHECK_SLEEP_REQ:
        rrcff_check_l1_sleep_req(wrrc_as_id, cmd_ptr);
        wrrc_free_cmd_buf( wrrc_as_id, cmd_ptr);
        break;
      default:
        /* Command saved here will be freed after processing the response */
        if(expected_cmd == cmd_ptr->cmd_hdr.cmd_id)
        {
          cmd_process_status.keep_waiting = FALSE;
          cmd_process_status.cmd_ptr = cmd_ptr;
        }
        else
        {
          WRRC_ERR1_FATAL("Unexpected Message %d",cmd_ptr->cmd_hdr.cmd_id);
        }
        break;
    }
  }
  return cmd_process_status;
}

#ifdef FEATURE_DUAL_WCDMA 
/*===========================================================================

FUNCTION                RRCFF_CHECK_FOR_ALT_SUB_CMDS

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
rrc_ff_wait_type rrcff_check_for_alt_sub_cmds(sys_modem_as_id_e_type wrrc_as_id)
{
  rrc_cmd_type *cmd_ptr;
  rrc_ff_wait_type cmd_process_status;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;
  WRRC_MSG0_MED("waiting for SIBDB_ENTRY_FOR_CELL_CNF from alternate SUB");
  while((cmd_ptr = q_get(&rrc_cmd_q.rrc_alt_sub_ff_cmd_q)) != NULL)
  {
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
#ifdef FEATURE_DUAL_WCDMA_SIBDB_SHARING
      case RRC_GET_SIBDB_ENTRY_FOR_CELL:
        WRRC_MSG0_HIGH("DUAL_W:RRC_GET_SIBDB_ENTRY_FOR_CELL is received from alternate SUB while waiting for RRC_GET_SIBDB_ENTRY_FOR_CELL_CNF");
        rrc_sib_event_controller(wrrc_as_id, cmd_ptr);
        wrrc_free_cmd_buf(wrrc_as_id,cmd_ptr);
        break;
      case RRC_GET_SIBDB_ENTRY_FOR_CELL_CNF:
        /* Command received here will be freed after processing the response */
        WRRC_MSG0_HIGH("DUAL_W:RRC_GET_SIBDB_ENTRY_FOR_CELL_CNF is received");
        if(expected_cmd == cmd_ptr->cmd_hdr.cmd_id)
        {
          cmd_process_status.keep_waiting = FALSE;
          cmd_process_status.cmd_ptr = cmd_ptr;
        }
        break;
#endif
      default:
        WRRC_ERR1_FATAL("DUAL_W:Unexpected Message %d",cmd_ptr->cmd_hdr.cmd_id);
        break;
    }
  }
  return cmd_process_status;
}
#endif
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
rrc_ff_wait_type rrcff_wait_high_priority_rsp(sys_modem_as_id_e_type wrrc_as_id)
{
  rex_sigs_type sigs;             /* hold signals from rex_wait() */

  boolean keep_waiting = TRUE;

  rrc_ff_wait_type cmd_process_status;
  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  do
  {                     /* until we get the specified signal */
    sigs = rex_wait( RRC_L1_FF_CMD_Q_SIG | RRC_RPT_TIMER_SIG);

    /* Need to kick the watchdog */
    if( sigs & RRC_RPT_TIMER_SIG )
    {
      rrc_dog_report_and_clear_sig(wrrc_as_id);
    }
    else if (sigs & RRC_L1_FF_CMD_Q_SIG) 
    {
      (void) rex_clr_sigs( rex_self(), RRC_L1_FF_CMD_Q_SIG );
      cmd_process_status = rrcff_check_for_l1_cmds(wrrc_as_id);
      keep_waiting = cmd_process_status.keep_waiting;
    }

  } while (keep_waiting == TRUE);
  return cmd_process_status;
}

#ifdef FEATURE_DUAL_WCDMA 
/*===========================================================================

FUNCTION                RRCFF_WAIT_HIGH_PRIORITY_RSP_FROM_ALT_SUB

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
rrc_ff_wait_type rrcff_wait_high_priority_rsp_from_alt_sub(sys_modem_as_id_e_type wrrc_as_id)
{
  rex_sigs_type sigs;             /* hold signals from rex_wait() */

  boolean keep_waiting = TRUE;

  rrc_ff_wait_type cmd_process_status;
  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  do
  {                     /* until we get the specified signal */
    sigs = rex_wait( RRC_ALT_SUB_FF_CMD_Q_SIG | RRC_RPT_TIMER_SIG);

    /* Need to kick the watchdog */
    if( sigs & RRC_RPT_TIMER_SIG )
    {
      rrc_dog_report_and_clear_sig(wrrc_as_id);
    }
    else if (sigs & RRC_ALT_SUB_FF_CMD_Q_SIG) 
    {
      (void) rex_clr_sigs( rex_self(), RRC_ALT_SUB_FF_CMD_Q_SIG );
      cmd_process_status = rrcff_check_for_alt_sub_cmds(wrrc_as_id);
      keep_waiting = cmd_process_status.keep_waiting;
    }
  } while (keep_waiting == TRUE);
  return cmd_process_status;
}
#endif
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
void rrcff_l1_get_data_for_ui_display(sys_modem_as_id_e_type wrrc_as_id, l1_ui_display_info_struct_type *l1_data_for_ui_display)
{
  rrc_ff_wait_type  cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_DATA_FOR_UI_DISP_REQ;

  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_DATA_FOR_UI_DISP_CNF);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  *l1_data_for_ui_display = cmd_process_status.cmd_ptr->cmd.data_for_ui_disp_cnf;

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
}

#ifdef FEATURE_DUAL_WCDMA_SIBDB_SHARING 
/*===========================================================================
FUNCTION                RRCFF_GET_SIBDB_ENTRY_FROM_ALT_SUB

DESCRIPTION
                        This function sends a command to ALT SUB to get the SIBDB entry for a given
                        PLMN ID, CELL ID combination.


DEPENDENCIES
                        None

RETURN VALUE
                        TRUE if SIB DB entry for the PLMN ID, CELL ID combination exists.
                        FALSE if the SIB DB entry is not present.

SIDE EFFECTS
                        None

===========================================================================*/
boolean rrcff_get_sibdb_entry_from_alt_sub(sys_modem_as_id_e_type wrrc_as_id,rrc_plmn_identity_type plmn, rrc_int_cell_id_type cell_id,rrc_sibdb_cell_header_type *cell_ptr)
{
  rrc_ff_wait_type  cmd_process_status;
  rrc_cmd_type *ff_cmd_ptr;
  boolean cell_found = FALSE;
  sys_modem_as_id_e_type alt_sub = WRRC_GET_ALT_SUB_ID(wrrc_as_id);
  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  ff_cmd_ptr = rrc_get_alt_sub_ff_cmd_buf(alt_sub);

  /* Fill in the Command Header, and indicate to do it Now! */
  ff_cmd_ptr->cmd_hdr.cmd_id = RRC_GET_SIBDB_ENTRY_FOR_CELL;
  ff_cmd_ptr->cmd.rrc_get_sibdb_entry.cell_id = cell_id;
  ff_cmd_ptr->cmd.rrc_get_sibdb_entry.plmn_id = plmn;

  rrc_put_alt_sub_ff_cmd(alt_sub, ff_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_GET_SIBDB_ENTRY_FOR_CELL_CNF);

  cmd_process_status = rrcff_wait_high_priority_rsp_from_alt_sub(wrrc_as_id);
  cell_found =  cmd_process_status.cmd_ptr->cmd.rrc_get_sibdb_entry_cnf.cell_found;
  if(cell_found)
  {
    WRRC_MSG0_HIGH("DUAL_W:A cell is found in alternate SUB for the given PLMN ID cell ID combo");
    WCDMA_MEMCPY(cell_ptr,
                 sizeof(rrc_sibdb_cell_header_type),
                 cmd_process_status.cmd_ptr->cmd.rrc_get_sibdb_entry_cnf.cell,
                 sizeof(rrc_sibdb_cell_header_type));
  }
  wrrc_free_cmd_buf(wrrc_as_id,cmd_process_status.cmd_ptr);
  return cell_found;
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
void rrcff_l1_get_rssi(sys_modem_as_id_e_type wrrc_as_id,l1_rssi_info_struct_type *rssi_data)
{
  rrc_ff_wait_type  cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_RSSI_REQ;

  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_RSSI_CNF);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  *rssi_data = cmd_process_status.cmd_ptr->cmd.rssi_cnf;

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
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
boolean rrcff_l1_is_s_criteria_met(sys_modem_as_id_e_type wrrc_as_id, int16 q_qualmin, int16 q_rxlevmin, int16 ue_tx_max_pwr)
{
  rrc_ff_wait_type  cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;
  boolean ret_val;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_CHECK_S_CRIT_REQ;
  l1_cmd_ptr->cmd.s_criteria_req.q_qualmin = q_qualmin;
  l1_cmd_ptr->cmd.s_criteria_req.q_rxlevmin = q_rxlevmin;
  l1_cmd_ptr->cmd.s_criteria_req.ue_tx_max_pwr = ue_tx_max_pwr;

  /* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_CHECK_S_CRIT_CNF);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  ret_val = cmd_process_status.cmd_ptr->cmd.s_criteria_cnf.status;

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);

  return(ret_val);
}

#ifdef FEATURE_DUAL_SIM
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
void rrcff_fail_l1_sleep_req(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr)
{
  l1_ext_cmd_type   *l1_cmd_ptr;

  l1_cmd_ptr = l1_get_cmd_buf();

   WRRC_MSG0_HIGH("DSIM : can L1 go to sleep called when RRC waiting for lock: function rrcff_fail_l1_sleep_req is called");

  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_CHECK_SLEEP_RSP;

  l1_cmd_ptr->cmd.l1_sleep_rsp.go_to_sleep = TRUE;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtog_bplmn_in_progress = FALSE;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtow_bplmn_in_progress = FALSE;

  /* Checking if bplmn is in progress */
  if(WTOW_SEARCH_RESUME  == rrc_csp_bplmn_srch_in_progress(wrrc_as_id))
  {
    if(!rrc_csp_wtog_bplmn_int_data.wtog_bplmn_srch_active)
    {
      l1_cmd_ptr->cmd.l1_sleep_rsp.wtow_bplmn_in_progress = TRUE;
    }
    else
    {
      l1_cmd_ptr->cmd.l1_sleep_rsp.wtog_bplmn_in_progress = TRUE;
    }
  }
  
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);
}
#endif


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
void rrcff_check_l1_sleep_req(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr)
{
  uint32 timer_val;
  boolean go_to_sleep;
  boolean wtog_bplmn_in_progress;
#ifdef FEATURE_DUAL_SIM
  boolean wtow_bplmn_in_progress;
#endif

  l1_ext_cmd_type   *l1_cmd_ptr;

  timer_val = cmd_ptr->cmd.l1_sleep_req.timer_val;

#ifdef FEATURE_DUAL_SIM
  go_to_sleep = rrc_can_l1_go_to_sleep(wrrc_as_id,timer_val,&wtog_bplmn_in_progress,&wtow_bplmn_in_progress);
#else
  go_to_sleep = rrc_can_l1_go_to_sleep(wrrc_as_id,timer_val,&wtog_bplmn_in_progress);
#endif

  l1_cmd_ptr = l1_get_cmd_buf();

  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_CHECK_SLEEP_RSP;

  l1_cmd_ptr->cmd.l1_sleep_rsp.go_to_sleep = go_to_sleep;

  l1_cmd_ptr->cmd.l1_sleep_rsp.wtog_bplmn_in_progress = wtog_bplmn_in_progress;

#ifdef FEATURE_DUAL_SIM
  l1_cmd_ptr->cmd.l1_sleep_rsp.wtow_bplmn_in_progress = wtow_bplmn_in_progress;
#endif

  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);
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

boolean rrcff_l1_get_periodic_inter_rat_msmt_results
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32  meas_id,
  l1_inter_sys_meas_result_struct_type  *p_msmtres
)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = L1_INTER_SYS;

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);
  WCDMA_MEMCPY((void *)p_msmtres, 
               sizeof(l1_inter_sys_meas_result_struct_type),
               (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.inter_sys_rpt.measurements),
               sizeof(l1_inter_sys_meas_result_struct_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
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

boolean rrcff_l1_get_additional_intra_freq_msmt_results
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32  meas_id,
  l1_intra_freq_meas_rpt_struct_type  *p_msmtres,
  uint8 freq_idx
)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();
  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = L1_INTRA_FREQ;
  l1_cmd_ptr->cmd.add_meas_info_req.carr_idx = freq_idx;

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  WCDMA_MEMCPY((void *)p_msmtres,
               sizeof(l1_intra_freq_meas_rpt_struct_type), 
               (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.intra_f_rpt),
               sizeof(l1_intra_freq_meas_rpt_struct_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
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

boolean rrcff_l1_get_additional_interf_msmt_results
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32  meas_id,
  l1_inter_freq_meas_rpt_struct_type  *p_msmtres
)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = L1_INTER_FREQ;

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  WCDMA_MEMCPY((void *)p_msmtres, 
               sizeof(l1_inter_freq_meas_rpt_struct_type),
               (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.inter_f_rpt),
               sizeof(l1_inter_freq_meas_rpt_struct_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
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

boolean rrcff_l1_get_additional_internal_msmt_results
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32  meas_id,
  l1_ue_internal_meas_rpt_struct_type  *p_msmtres
)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_ADDITIONAL_MEAS_DATA_REQ;

  l1_cmd_ptr->cmd.add_meas_info_req.meas_id =  meas_id;
  l1_cmd_ptr->cmd.add_meas_info_req.meas_type = L1_UE_INTERNAL;

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_ADDITIONAL_MEAS_DATA_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);
  WCDMA_MEMCPY((void *)p_msmtres, 
               sizeof(l1_ue_internal_meas_rpt_struct_type),
               (void *)(&cmd_process_status.cmd_ptr->cmd.add_meas_info_rsp.u.ue_internal_meas_rpt),
               sizeof(l1_ue_internal_meas_rpt_struct_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
  if(p_msmtres->cell_measured_results_included == FALSE)
    return FALSE;
  else
    return TRUE;
}

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

boolean rrcff_l1_get_utran_nmr_info
(
  sys_modem_as_id_e_type wrrc_as_id, 
  l1_meas_utran_nmr_info_union_type  *l1_meas_utran_nmr_info,
  l1_rrc_utran_nmr_report_type  report_type
)
{
  boolean status;
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_NMR_INFO_REQ;

  l1_cmd_ptr->cmd.utran_nmr_info_req.report_type = report_type;

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_UTRAN_NMR_INFO_CNF);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);
  WCDMA_MEMCPY((void *)l1_meas_utran_nmr_info, 
               sizeof(l1_meas_utran_nmr_info_union_type),
               (void *)(cmd_process_status.cmd_ptr->cmd.nmr_cnf.l1_meas_utran_nmr_info),
               sizeof(l1_meas_utran_nmr_info_union_type));

  status = cmd_process_status.cmd_ptr->cmd.nmr_cnf.status;
  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
  return status;
}

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

void rrcff_l1_process_immediate_meas_req
(
  sys_modem_as_id_e_type wrrc_as_id, 
  l1_immediate_meas_type  *immed_meas,
  l1_meas_quan_enum_type  meas_quan
)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr = l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_GET_IMM_MEAS_REQ;


  l1_cmd_ptr->cmd.imm_meas_req.meas_quan = meas_quan;
  l1_cmd_ptr->cmd.imm_meas_req.meas_type = L1_INTRA_FREQ;
  

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_GET_IMM_MEAS_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  WCDMA_MEMCPY((void *)immed_meas, 
               sizeof(l1_immediate_meas_type),
               (void *)(cmd_process_status.cmd_ptr->cmd.imm_meas_cnf.immed_meas),
               sizeof(l1_immediate_meas_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
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

void rrcff_l1_process_immediate_interf_meas_req(sys_modem_as_id_e_type wrrc_as_id, l1_immediate_meas_type *immed_meas)
{
  rrc_ff_wait_type cmd_process_status;
  l1_ext_cmd_type   *l1_cmd_ptr;

  cmd_process_status.keep_waiting = TRUE;
  cmd_process_status.cmd_ptr = NULL;

  l1_cmd_ptr =l1_get_cmd_buf();

  /* Fill in the Command Header, and indicate to do it Now! */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_GET_IMM_MEAS_REQ;


  l1_cmd_ptr->cmd.imm_meas_req.meas_type = L1_INTER_FREQ;
  

/* Now send the primitive to L1 */
  rrc_send_l1_ff_cmd(wrrc_as_id, l1_cmd_ptr);

  rrcff_set_expected_cmd_id(wrrc_as_id, RRC_CPHY_GET_IMM_MEAS_RSP);

  cmd_process_status = rrcff_wait_high_priority_rsp(wrrc_as_id);

  WCDMA_MEMCPY((void *)immed_meas, 
               sizeof(l1_immediate_meas_type),
               (void *)(cmd_process_status.cmd_ptr->cmd.imm_meas_cnf.immed_meas),
               sizeof(l1_immediate_meas_type));

  wrrc_free_cmd_buf( wrrc_as_id, cmd_process_status.cmd_ptr);
}
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcff_data

DESCRIPTION
This function dumps the free floating globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcff_data(sys_modem_as_id_e_type wrrc_as_id)
{
  qsh_mdump_collect_high(&expected_cmd,sizeof(expected_cmd));
}
#endif

#endif /* FEATURE_WCDMA_FREE_FLOATING_TASK */
