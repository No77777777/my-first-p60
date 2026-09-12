#ifndef RRCCHO_H
#define RRCCHO_H
/*===========================================================================
         R R C  C E L L  C H A N G E   O R D E R   P R O C E D U R E 

DESCRIPTION
  This header file contains function prototypes for the RRC Cell Change Order
  Procedure. 

Copyright (c) 2003-2008 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrccho.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/23/15   as      Made changes to allow QTA when CHO procedure is waiting for CU
12/18/14   vi      Fixed compilation errors.
09/03/14   sp      Made changes to save procedure substates in crash debug info
08/14/13   vg      Made changes to implement a new diag event EVENT_WCDMA_RRC_FAILURE
04/26/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.96.50
04/20/11   vg      added support for mdsp halt recovery
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
05/08/09   ss      Updated Copyright Information
01/04/06   vm      Added support for barring the correct GSM cell when WtoG CCO
                   fails because GSM cell is barred. Added protoype for new function 
                   rrccho_fill_gsm_cell_info_for_cell_bar(). 
10/25/05   vr      Added prototype for rrccho_is_wtog_cco_active()
08/05/04   kc      Added prototype for rrccho_is_wtog_cco_in_progress().
03/22/04   kc      Added prototype to query gtow_pcco status.
08/12/03   bu      Added prototype for rrccho_get_activation_time_for_wtog_cho.
04/10/03   rj      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "rrccmd_v.h"
#include "rrcsibdb.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION rrccho_indicate_ps_close

DESCRIPTION
  This function is called by DT when PS session is closed

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrccho_indicate_ps_close(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrccho_init_procedure

DESCRIPTION
  This function initializes everything that the RRC Cell Change Order procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrccho_init_procedure(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrccho_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC Cell Change Order procedure.
  The following events are handled by this procedure:

    RRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrccho_procedure_event_handler(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr);


/*===========================================================================

FUNCTION   rrccho_get_activation_time_for_wtog_cho

DESCRIPTION

  This function populates activation time for WtoG cell change order into the received
  ptr and also returns success/failure if activation is present or not.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS/FAILURE depending if Activation Time was appended or not.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type rrccho_get_activation_time_for_wtog_cho
(
  sys_modem_as_id_e_type wrrc_as_id,
uint16 * act_time
);

/*===========================================================================

FUNCTION   rrccho_is_gtow_pcco_in_progress

DESCRIPTION

  This function returns true/false based on whether GtoW PCCO is in progress
  or not.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE/FALSE depending if Activation Time was appended or not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrccho_is_gtow_pcco_in_progress(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION   rrccho_is_wtog_cco_in_progress

DESCRIPTION

  This function returns true/false based on whether WtoG PCCO is in progress
  or not.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE/FALSE depending on whether WtoG CellChangeOrder was in progress or not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrccho_is_wtog_cco_in_progress(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION   RRCCHO_IS_WTOG_CCO_ACTIVE

DESCRIPTION

  This function returns true/false based on whether WtoG CCO is active
  or not.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE/FALSE depending on whether WtoG CellChangeOrder was in active or not

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrccho_is_wtog_cco_active
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION   RRCCHO_FILL_GSM_CELL_INFO_FOR_CELL_BAR

DESCRIPTION

  This function is called by CSP when WtoG CCO fails because the GSM cell is
  barred. This function populates the GSM cell bar structure with the arfcn 
  and bsic values from rrccho_proc_info global structure so that the GSM cell
  can be appropriately barred. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrccho_fill_gsm_cell_info_for_cell_bar 
(
  sys_modem_as_id_e_type wrrc_as_id,
  W_ARFCN_T *gsm_arfcn,
  uint8     *gsm_ncc,
  uint8     *gsm_bcc 
);
#ifdef FEATURE_WCDMA_HANDLE_MDSP_HALT
/*===========================================================================

FUNCTION   rrccho_handle_mdsp_halt
DESCRIPTION
  This function takes care of mDSP halt recovery when CCO
   is in progress.
          
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
===========================================================================*/
void rrccho_handle_mdsp_halt(sys_modem_as_id_e_type wrrc_as_id);
#endif

/*===========================================================================

FUNCTION rrcho_map_inter_rat_cause_to_diag_cause

DESCRIPTION
  This function maps interrat failure causes to failure cause to be reported to diag.

DEPENDENCIES
  None.

RETURN VALUE
  Failure cause to Diag.

===========================================================================*/
uint8 rrcho_map_inter_rat_cause_to_diag_cause(sys_modem_as_id_e_type wrrc_as_id, uint8 inter_rat_failure_cause);
/*===========================================================================
FUNCTION rrccho_get_current_substate()

DESCRIPTION
  This function will return current RRC Connection Release 
  procedure's sub state.
DEPENDENCIES
  None

RETURN VALUE
  return current rcr sub state.
SIDE EFFECTS
none
===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
uint8  rrccho_get_current_substate(sys_modem_as_id_e_type wrrc_as_id );

/*===========================================================================
FUNCTION   rrccho_return_cho_status

DESCRIPTION
returns cho status

DEPENDENCIES
  None.

RETURN VALUE
rr_interrat_cc_order_status_e_type


SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
uint8 rrccho_return_cho_status(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION   rrccho_return_cho_failure_reason

DESCRIPTION
returns cho failure reason

DEPENDENCIES
  None.

RETURN VALUE
rrc_interrat_reject_reason_T

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
uint8 rrccho_return_cho_failure_reason(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION   rrc_qsh_log_cco_params

DESCRIPTION
returns cho failure reason

DEPENDENCIES
  None.

RETURN VALUE
rrc_interrat_reject_reason_T

SIDE EFFECTS
  None.
===========================================================================*/
void rrc_qsh_log_cco_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_cco_ota *cco_params);


/*===========================================================================
FUNCTION   rrc_qsh_log_cco_fail_params

DESCRIPTION
returns cho failure reason

DEPENDENCIES
  None.

RETURN VALUE
rrc_interrat_reject_reason_T

SIDE EFFECTS
  None.
===========================================================================*/
void rrc_qsh_log_cco_fail_params(sys_modem_as_id_e_type wrrc_as_id, uint32 fail_cause);
/*===========================================================================

FUNCTION   rrccho_is_procedure_waiting_for_cu_completion

DESCRIPTION

  This function returns true if procedure is waiting for CU

DEPENDENCIES

  None.

RETURN VALUE


SIDE EFFECTS

  None.

===========================================================================*/
boolean rrccho_is_procedure_waiting_for_cu_completion
(
  sys_modem_as_id_e_type wrrc_as_id
);

#endif /* RRCCHO_H */

