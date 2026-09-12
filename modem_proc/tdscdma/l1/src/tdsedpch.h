#ifndef EULEDPCH_H
#define EULEDPCH_H

/*==========================================================================
             WCDMA L1 HSUPA UL protocol operation related header file

DESCRIPTION
  This file contains definition, declaration required for HSUPA UL protocol
  procedures and algorithms

  Copyright (c) 2006-2007 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsedpch.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/10/10   sa      MPR 7.1 support
04/09/10    ksr     Changes to move logging variables to dynamic memory approach
09/02/09   sa      EDPCCH power boost feature support: Added a structure type
                   for the new Aec semistatic table.
08/28/09   gv      Checkin EUL PP changes protected within the feature
                   DEBUG_FEATURE_WCDMA_PP_HW_UL
06/23/09   oh      Fixed declaration issue for SGP table.
05/26/09   oh      Fixed a merge issue.
05/22/09   oh      Support for Rel-7 EDPDCH power interpolation feature.
04/08/09   ka      Changed code to fill up MDSP MPR bins in
                   tdsedpch_write_mpr_bins_to_fw
01/30/09   ka      Added Support for Enhanced MPR algorithm
11/04/08   gnk     Added support for EDCH to continue TX over seamless reconfig.
01/16/09   oh      Replacing INTLOCK in EUL ISR with REX_ISR_LOCK.
10/13/08   oh      EUL IRQ f3 messages enabled under a debug flag.
10/03/08   hk      Bringing in 7k Mailine fixes
11/19/07   am      Mainlining athena rev2 changes & removing rev1 hw workarounds
09/26/07   oh      Fix for SGP as per the Spec 25.321 CR 0325.
08/13/07   oh      Added support to exclude DTX slots during CM for DPCCH, UPH meas.
08/13/07   oh      Fix to handle the case where NW sends higher ref (forbidden) ETFCI
02/21/07   am      Added code for applying SG at activation time
02/02/07   oh      Enhanced interface to enable various f3 messages for EUL.
01/30/07   yiz     Fix ETFC PR to use next frame MTPL_no_mpr
01/04/07   gnk     Lint Fixes
12/18/06   am      Add tdsedpch_uph_meas_update() to fix compiler warning
12/12/06   rgn     Added test_beta_ed_sf2 to the list of beta overrides for plt
12/06/06   am      Fix EUL L1/MAC log packet
11/20/06   yiz     Add runtime detection of HW EUL cipher support
11/15/06   yiz     Add tdsedpch_save_ss_tables()
11/10/06   am      Added code for enabling/disabling Pwr restrction through QXDM
09/29/06   rgn     Added logic to override the beta_ed calculated for plt
08/14/06   yiz     Mainline HSUPA code
02/28/06   yiz     Initial version

===========================================================================*/


/* ---------------------------- */
/* Define/unde local features   */
/* ---------------------------- */

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_TDSCDMA_HSUPA

#include "tdsl1macdata.h"
#include "tdsl1rrcif.h"
#include "tdseulcfg.h"
#include "tdseulsg.h"

/* ---------------------------- */
/* Check for necessary features */
/* ---------------------------- */


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


#define TDSEDPCH_MAX_NUM_CM_PATNS_PER_CFG  1


/* Invalid CFN to suspend build_frame_ind()  */
#define TDSEDPCH_CFN_TO_SUSPEND_BFI_INVALID  ((uint16)-1)

/*if this value is set then EDPCH is dtxed in the next opportunity.So in FIQ,
if this value is encountered then euledpch module starts dtxing the EDPCH*/
#define TDSEDPCH_CFN_TO_SUSPEND_BFI_NOW  ((uint16)-2)

/* Chipx8 per sub-frame */
#define TDSEDPCH_CHIPX8_PER_SUB_FRAME 61440


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



/* ==========================================================================
** Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



extern uint16 tdsedpch_cfn_to_suspend_bfi;

/* Changing the cap values dynamically (Learning algo )
** will be required only for initial estimation. This
** estimation will be mostly a one time activity and
** after that the code may not be exercised.
*/
extern boolean tdseulsg_dbg_eul_isr_intlock;

/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*==========================================================================
FUNCTION:     EUL_EDPCH_IS_TRANSMITTING()

DESCRIPTION:  This function returns TRUE if the eul state is in TX state otherwise it returns false

DEPENDENCIES: None

RETURN VALUE: TRUE if eul is in Tx state FALSE otherwise

SIDE EFFECTS: None

==========================================================================*/
extern boolean tdsedpch_is_transmitting(void);

/*===========================================================================
FUNCTION  EUL_SET_VAR_THROUGH_QXDM

DESCRIPTION
  This function sets the eul uplink global variables as received
  through diag command. This function is a part of the callback function for
  diag command dispatch table.

DEPENDENCIES
  This function is called when "send_data 75 4 24 0 <val>" is issued from cmd
  line interface from QXDM

RETURN VALUE
  None

SIDE EFFECTS
  The eul pwr restriction can be disabled or enabled based on the val received
===========================================================================*/
void tdseul_set_var_through_qxdm( uint8 debug_switch_type, uint32 val );

/*===========================================================================
FUNCTION  EULEDPCH_INIT

DESCRIPTION
  This function initiliazes HSUPA UL protocol module at power up.

DEPENDENCIES
  Must be called only once at power up.

RETURN VALUE
  none

SIDE EFFECTS
  initilization to make HSUPA UL protocol module usable
===========================================================================*/
void tdsedpch_init(void);

/*===========================================================================
FUNCTION  EULEDPCH_PROCESS_CFG_DATA

DESCRIPTION
  This function processes E-DPCH cfg data and may build the semi static tables

DEPENDENCIES
  old and new confg params in tdseulcfg_info_struct_type pointer

RETURN VALUE
  None

SIDE EFFECTS
  May build semi-static tables for E-DPCH
===========================================================================*/
void tdsedpch_process_cfg_data
(
  tdseulcfg_info_struct_type *config_param_ptr
);

/*===========================================================================
FUNCTION  EULEDPCH_ACTIVATE_SS_TABLES

DESCRIPTION
  This function makes built Semi-Static tables active

DEPENDENCIES
  Non

RETURN VALUE
  None

SIDE EFFECTS
  As above
===========================================================================*/
boolean tdsedpch_activate_ss_tables(void);

/*===========================================================================
FUNCTION  EULEDPCH_PROCESS_EDCH_FOR_NEXT_TTI

DESCRIPTION
  This function is called by Serving Grant module to process E-DCH for next
  TTI's transmission in FIQ.

DEPENDENCIES
  Called by Serving Grant module in FIQ

RETURN VALUE
  void

SIDE EFFECTS
  EUL UL will do E-TFC power restriction, ask MAC/RLC to build data, and ask
  FW and HW to transmit the data for new transmission, or EUL UL will ask
  FW and HW to re-transmit the HARQ
===========================================================================*/
void tdsedpch_process_edch_for_next_tti
(
  tdseulsg_next_tti_tx_info_struct_type *next_tti_tx_info /* parameters from SG module */
);

/*===========================================================================
FUNCTION  EULEDPCH_UPH_MEAS_RESET

DESCRIPTION
  This function resets UPH measurements

DEPENDENCIES
  Non

RETURN VALUE
  None

SIDE EFFECTS
  resets UPH measurements
===========================================================================*/


/*===========================================================================
FUNCTION  EULEDPCH_TX_DISABLE

DESCRIPTION
  This function is called by ENC module to disable E-DPCH for out-of-sync

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  E-DPCH DTX
===========================================================================*/
void tdsedpch_tx_disable(void);

/*===========================================================================
FUNCTION    EULEDPCH_TX_DISABLE_DURING_RECFG

DESCRIPTION This function is called by l1msetup/seq module to disable
            E-DPCH at activation time when EDPCH is recfged

DEPENDENCIES This function should be called only at activation time and
             only when EDCH is recfged

RETURN VALUE None

SIDE EFFECTS E-DPCH DTXed
===========================================================================*/
void tdsedpch_tx_disable_during_recfg(void);
/*===========================================================================
FUNCTION        EULEDPCH_SUSPEND_BFI_ONE_FRAME_BEFORE_ACT_TIME

DESCRIPTION     This function sets cfn corresponding to act_time -1 to suspends
                build_fram_ind one frame before the activation time till eul new cfg is
                activated. The EDPCH is DTXed during this time

DEPENDENCIES    Must be called within TDS_INTLOCK

RETURN VALUE    None

SIDE EFFECTS    As said above
==========================================================================*/
void tdsedpch_suspend_bfi_one_frame_before_act_time(uint16 local_act_time);


/*===========================================================================
FUNCTION  EULEDPCH_TX_RE_ENABLE

DESCRIPTION
  This function is called by ENC module to re-enable E-DPCH after out-of-sync

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  E-DPCH resumes
===========================================================================*/
void tdsedpch_tx_re_enable(void);

/*===========================================================================
FUNCTION  EULEDPCH_SAVE_SS_TABLES

DESCRIPTION
  This function is called by E UL CONFIG module to save currently active SS
  tables for future possible reverting or resuming.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  E-DPCH resumes
===========================================================================*/
void tdsedpch_save_ss_tables(void);


#ifdef  FEATURE_TDSCDMA_PLT
/*===========================================================================
FUNCTION  WPLT_SET_TEST_EDPCCH_ONLY_MODE

DESCRIPTION
  This function is called by WPLT module to test E-DPCCH only by setting
  Beta_Ed to always zero, and always new TX, and overriding RSN

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  UE will hence be always in E-DPCCH only mode until power cycle.
  API to reset to normal mode is not provided.
===========================================================================*/
void wplt_set_test_edpcch_only_mode(void);
#endif /*  FEATURE_TDSCDMA_PLT */

/*===========================================================================
FUNCTION  tdsedpch_save_build_ss_table_parameters

DESCRIPTION
  This function saves all parameters to build SS table, also compute how many 
  steps to build SS table.

  SS table is build by 1 to 5 steps, it depends on how many E-PUCH timeslot
  NW configured, each step build 1 timeslot SS table. So for max E-PUCH timeslot
  condition, SS table is built by 5 steps.

  After L1 received CPHY_STEUP command, L1 validation successful, L1 post local command
  TDS_E_UL_BUILD_SS_TABLES_CMD, then function tdseulcfg_config_build_ss_tables() will 
  be called, it invoke function tdsedpch_save_build_ss_table_parameters() to save all parameters. 

  later procedure is as below:
  1) At the end of function tdseulcfg_config_build_ss_tables(), it post L1 local command 
     TDS_E_UL_BUILD_SS_TABLES_CMD. 
  2) L1 task process TDS_E_UL_BUILD_SS_TABLES_CMD, call tdsedpch_compute_ss_table_step_by_step() 
     function to build SS table by one step.
  3) after this step is done, if still not finished, register SEQ call back function 
     tdseul_compute_next_step_ss_table_seq_call_back() which will be called 2 sub frame later
  4) 2 sub frame later, SEQ invoke call back function tdseul_compute_next_step_ss_table_seq_call_back()
     post L1 local command TDS_E_UL_BUILD_SS_TABLES_CMD
  5) repeat step 2 to 4, until all steps are finished.
  
DEPENDENCIES
  confg params from higher layer

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/



void tdsedpch_save_build_ss_table_parameters(tdseulcfg_info_struct_type* config_param_ptr);

#endif /* FEATURE_TDSCDMA_HSUPA */

#endif /* EULEDPCH_H */

