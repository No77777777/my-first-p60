#ifndef TDSRRCFF_H
#define TDSRRCFF_H
/*===========================================================================
            RRC  TD-SCDMA  FREE FLOATING MANAGER   FILE
DESCRIPTION

   This file contains RRC FREE FLOATING  procedure

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2011-2012 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcfreefloating.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/12/12   cdf     Merged from WCDMA
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
#include "tdsrrcmmif.h"
#include "tdsrrc_rr_if.h"
#include "tdsl1rrcif.h"


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Fuction Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
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

void tdsrrcff_l1_get_data_for_ui_display
(
  tdsl1_ui_display_info_struct_type  *l1_data_for_ui_display
);
#endif /*FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY*/
#ifndef TEST_FRAMEWORK
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
void tdsrrcff_l1_get_rssi
(
  tdsl1_rssi_info_struct_type  *rssi_data
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

boolean tdsrrcff_l1_is_s_criteria_met
(
  /*int16  q_qualmin,*/
  int16  q_rxlevmin,
  int16  ue_tx_max_pwr
);
#endif

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

void tdsrrc_dispatch_l1_ff_commands_during_wait_lock( void );

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

void tdsrrcff_fail_l1_sleep_req
(
  tdsrrc_cmd_type  *cmd_ptr
);


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

void tdsrrcff_check_l1_sleep_req
(
  tdsrrc_cmd_type  *cmd_ptr
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

boolean tdsrrcff_l1_get_periodic_inter_rat_msmt_results
(
  uint32  meas_id,
  tdsl1_inter_sys_meas_result_struct_type  *p_msmtres
);
#ifndef TEST_FRAMEWORK
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

boolean tdsrrcff_l1_get_additional_interf_msmt_results
(
  uint32  meas_id,
  tdsl1_inter_freq_meas_rpt_struct_type  *p_msmtres
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

boolean tdsrrcff_l1_get_additional_internal_msmt_results
(
  uint32  meas_id,
  tdsl1_ue_internal_meas_rpt_struct_type  *p_msmtres
);
#endif
#ifdef FEATURE_TDSCDMA_PLI_NMR_UTRAN
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

boolean tdsrrcff_l1_get_utran_nmr_info
(
  tdsl1_meas_utran_nmr_info_union_type  *l1_meas_utran_nmr_info, 
  tdsl1_rrc_utran_nmr_report_type  report_type
);

#endif /*FEATURE_TDSCDMA_PLI_NMR_UTRAN*/
#ifndef TEST_FRAMEWORK
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
);
#endif
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

void tdsrrcff_l1_process_immediate_interf_meas_req
(
  tdsl1_immediate_meas_type_tmp  *immed_meas
);

#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */
#endif /* TDSRRCFF_H */
