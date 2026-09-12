#ifndef TDSRRCCHO_H
#define TDSRRCCHO_H
/*===========================================================================
         R R C  C E L L  C H A N G E   O R D E R   P R O C E D U R E 

DESCRIPTION
  This header file contains function prototypes for the RRC Cell Change Order
  Procedure. 

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccho.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/15/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdsrrccmd_v.h"

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

FUNCTION tdsrrccho_init_procedure

DESCRIPTION
  This function initializes everything that the RRC Cell Change Order procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrccho_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrccho_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC Cell Change Order procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrccho_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);


/*===========================================================================

FUNCTION   tdsrrccho_get_activation_time_for_td2g_cho

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
uecomdef_status_e_type tdsrrccho_get_activation_time_for_td2g_cho
(
uint16 * act_time
);

/*===========================================================================

FUNCTION   tdsrrccho_is_g2td_pcco_in_progress

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
boolean tdsrrccho_is_g2td_pcco_in_progress(void);

/*===========================================================================

FUNCTION   tdsrrccho_is_td2g_cco_in_progress

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
boolean tdsrrccho_is_td2g_cco_in_progress(void);

/*===========================================================================

FUNCTION   tdsrrccho_is_td2g_cco_active

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
boolean tdsrrccho_is_td2g_cco_active
(
  void
);

/*===========================================================================

FUNCTION   RRCCHO_FILL_GSM_CELL_INFO_FOR_CELL_BAR

DESCRIPTION

  This function is called by CSP when WtoG CCO fails because the GSM cell is
  barred. This function populates the GSM cell bar structure with the arfcn 
  and bsic values from tdsrrccho_proc_info global structure so that the GSM cell
  can be appropriately barred. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccho_fill_gsm_cell_info_for_cell_bar 
(
  W_ARFCN_T *gsm_arfcn,
  uint8     *gsm_ncc,
  uint8     *gsm_bcc 
);
/*===========================================================================

FUNCTION tdsrrccho_map_cell_change_order_failure_reason

DESCRIPTION
    This function map the CCO failure reason.

DEPENDENCIES
    None.

RETURN VALUE
    Void

===========================================================================*/

void tdsrrccho_map_cell_change_order_failure_reason
(
  tdsrr_interrat_cc_order_fail_reason_type failure_reason    
);

#endif /* TDSRRCCHO_H */

