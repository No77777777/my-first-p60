#ifndef RRCFF_H
#define RRCFF_H
/*===========================================================================
            RRC  WCDMA  FREE FLOATING MANAGER   FILE
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

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcfreefloating.h_v   1.25   19 Jun 2002 10:01:34   kpatil  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcfreefloating.h#2 $    $DateTime: 2020/01/24 03:56:36 $ $ $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/28/15   bc      Changes to share SIBDB and ACQDB among two SUBs in a W+W solution
05/28/14   vi      Made changes to add freq index in function  rrcff_l1_get_additional_intra_freq_msmt_results
01/29/14   sr      Made changes to handle FF commands from L1 when waiting for lock
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
#include "rrcmmif.h"
#include "rrcrrif.h"
#include "l1rrcif.h"
#include "rrcsibdbi.h"


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Fuction Declarations
** ----------------------------------------------------------------------- */
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

void rrcff_l1_get_data_for_ui_display
(
  sys_modem_as_id_e_type wrrc_as_id,
  l1_ui_display_info_struct_type  *l1_data_for_ui_display
);

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
boolean rrcff_get_sibdb_entry_from_alt_sub
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_plmn_identity_type plmn,
  rrc_int_cell_id_type cell_id,
  rrc_sibdb_cell_header_type *cell
);
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
void rrcff_l1_get_rssi
(
  sys_modem_as_id_e_type wrrc_as_id,
  l1_rssi_info_struct_type  *rssi_data
);

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

boolean rrcff_l1_is_s_criteria_met
(
  sys_modem_as_id_e_type wrrc_as_id,
  int16  q_qualmin,
  int16  q_rxlevmin,
  int16  ue_tx_max_pwr
);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION  RRC_DISPATCH_L1_FF_COMMANDS_DURING_WAIT_LOCK

DESCRIPTION
 
  This function dispatches all commands received in RRC's free floating
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void rrc_dispatch_l1_ff_commands_during_wait_lock(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION                RRCFF_FAIL_L1_SLEEP_REQ

DESCRIPTION
                        This function processes the sleep request command sent by L1
                        in every DRX cycle when RRC is waiting for lock

DEPENDENCIES
                        None

RETURN VALUE
                        None

SIDE EFFECTS
                        None

 ===========================================================================*/

void rrcff_fail_l1_sleep_req
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type  *cmd_ptr
);
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

void rrcff_check_l1_sleep_req
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type  *cmd_ptr
);

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
);

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
);

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
);

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
);

/*===========================================================================

FUNCTION                RRCFF_L1_GET_UTRAN_NMR_INFO

DESCRIPTION             This function is called to get
                        6best intra/inter-freq/inter-rat cells in response to a
                        NMR request from L1.


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
);


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
);

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

void rrcff_l1_process_immediate_interf_meas_req
(
  sys_modem_as_id_e_type wrrc_as_id,
  l1_immediate_meas_type  *immed_meas
);

#endif /* FEATURE_WCDMA_FREE_FLOATING_TASK */
#endif /* RRCFF_H */
