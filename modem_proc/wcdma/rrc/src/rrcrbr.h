#ifndef RRCRBR_H
#define RRCRBR_H
/*===========================================================================
         R R C   R A D I O   B E A R E R   R E L E A S E   M O D U L E 

DESCRIPTION

  This module is the header module for the Radio Bearer Release module.
  This contains the external interface functions to be used by
  by other RRC modules. This must not be used by modules outside RRC.
  
Copyright (c) 2001-2004, 2006-2008 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcrbr.h_v   1.1   06 Mar 2002 10:01:38   upuntamb  $   
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcrbr.h#2 $    $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/28/16   nr      Added code to support LTA during activation time
06/17/14   sp      Made changes to mainline RLC-restablihment redesign changes
06/02/14   sp      Made changes for FR19997 - UL rlc re-establishment redesign
09/29/13   db      Made changes to update failure status of procedure upon registration with CU and OC reset
07/19/13   db      Made changes to Swap OC and CC after channel config by CU when
                   a procedure is registered and it fails
05/08/09   ss      Updated Copyright Information
09/11/06   da      Added support for FEATURE_RLC_ONE_SIDE_RE_ESTABLISHMENT
09/05/06   da      Added prototype for rrcrbr_get_new_crnti_valid()
03/10/06   da      Added Initial support for PDCP under FEATURE_UMTS_PDCP
08/09/04   kc      Interface change for CU-build-rbr msg to check fo
                   RB-Re-est of TM-bearers when transitioning from non-DCH state 
                   to DCH state.
07/14/04   sk      Added prototype for function to get the current substate
02/03/04   sk      SRNS relocation support in rbr complete message triggered by 
                   CU proc. Added a new field.
03/05/02   upn     Added prototype for rrcrbr_build_rbr_complete_message
05/10/01   ram     Created file.


===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "comdef.h"

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
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCRBR_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBR procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrcrbr_init_procedure(sys_modem_as_id_e_type wrrc_as_id);

#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION   RRCRBR_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrcrbr_is_srns_in_progress
(
sys_modem_as_id_e_type wrrc_as_id
);
#endif
/*===========================================================================

FUNCTION   RRCRBR_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrcrbr_get_new_crnti_valid
(
sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION   RRCRBR_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  rrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
rrcrb_substate_e_type rrcrbr_get_current_sub_state
(
sys_modem_as_id_e_type wrrc_as_id
) ;
/*===========================================================================
 
 FUNCTION   rrcrbr_get_dest_state()
 
 DESCRIPTION
Returns the state indicated by the message
 DEPENDENCIES
 
   None.

 RETURN VALUE
 
   rrc_state_e_type
 
 SIDE EFFECTS
 
   None.

 ===========================================================================*/
rrc_state_e_type rrcrbr_get_dest_state(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION   rrcrbr_get_initiate_cu_flag()

DESCRIPTION
 This function is used to return rrcrbr_send_initiate_cu_flag 
 
DEPENDENCIES

  None.

RETURN VALUE

  rrc_state_e_type.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrcrbr_get_initiate_cu_flag(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION   rrcrbr_is_proc_successful()

DESCRIPTION
 This function is used to return failure status
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean rrcrbr_is_proc_successful
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION   rrcrbr_update_failure_status()

DESCRIPTION
  Updates the failure status
DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
void rrcrbr_update_failure_status
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrcrb_msg_status_e_type status
);

/*===========================================================================

FUNCTION RRCRBR_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Radio Bearer Release procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the Radio Bearer Release procedure can be initiated
  only in the CELL_FACH and CELL_DCH states. However, while the
  procedure is active, the RRC state may change to any of the other
  states. Hence all RRC states are checked by this module.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void rrcrbr_procedure_event_handler
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION RRCRBR_BUILD_RBR_COMPLETE_MSG


DESCRIPTION

  Builds Radio Bearer Release Complete Message to be sent on Uplink DCCH.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/

void rrcrbr_build_rbr_complete_message(sys_modem_as_id_e_type wrrc_as_id,
  rrc_RRC_TransactionIdentifier transaction_id,  /* Transaction Identifier to be sent in Uplink */
  rrc_UL_DCCH_Message *ptr,                      /* Uplink DCCH message pointer */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
);
/*===========================================================================
FUNCTION rrcrbr_return_send_initiate_cu_flag
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
boolean rrcrbr_return_send_initiate_cu_flag(sys_modem_as_id_e_type wrrc_as_id);

#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
/*===========================================================================

FUNCTION rrcrbr_set_failure_cause

DESCRIPTION

DEPENDENCIES


RETURN VALUE
  Returns 

SIDE EFFECTS

  None

===========================================================================*/
void rrcrbr_set_failure_cause(sys_modem_as_id_e_type wrrc_as_id, rrcrb_msg_status_e_type cause);

#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/

#endif /* RRCRBR_H */

