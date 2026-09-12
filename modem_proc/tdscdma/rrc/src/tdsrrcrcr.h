#ifndef TDSRRCRCR_H
#define TDSRRCRCR_H
/*===========================================================================
              R R C R C R  H E A D E R  F I L E

DESCRIPTION
 The RRCRCR module consists of RRC CONNECTION RELEASE PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation.
  
Copyright (c) 2010-2012  Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrcr.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdsrrccmd_v.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern tdsrrc_ProtocolErrorInformation  tdsrrc_protocol_error_information;

extern unsigned short tdsrrc_v_308;

extern tdsrrc_redirect_req_type tdsrrcrcr_saved_redirect_info;
extern boolean tdsrrcrcr_redirection_info_present;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrcrcr_init_procedure

DESCRIPTION
  This function will initialize the RRC Connection Release procedure
  substate to TDSRRCRCR_INITIAL Substate. This will be called in the RRCTASK 
  modules along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcrcr_init_procedure
(    
  void  
);
/*===========================================================================

FUNCTION tdsrrcrcr_procedure_event_handler

DESCRIPTION
  This is the entry point for RRC Connection Release procedure. All
  events for RCR come to this function from RRC Dispatcher. Depending on the
  UE state of RRC Connection Release Procedure, it calls the corresponding state 
  handler. These state handlers call substate handlers depending on the 
  the current substate of RRC Connection Release procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the RRC command */
);


/*===========================================================================

FUNCTION tdsrrcrcr_process_690non_critical_ext

DESCRIPTION
  Processes 690 non critical extension of RRC Conn Rel OTA.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_690non_critical_ext
(
  tdsrrc_RRCConnectionRelease_v690NonCriticalExtensions_1 * rcr_msg_ptr
);

/*===========================================================================

FUNCTION tdsrrcrcr_is_proc_active

DESCRIPTION
  Returns rcr sub-state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrcrcr_is_proc_active
(
  void
);

/*===========================================================================

FUNCTION tdsrrcrcr_update_release_cause

DESCRIPTION
  This function converts the release cause from ASN1 format into RRC-MM 
  interface format.

DEPENDENCIES
  None

RETURN VALUE
  rrc_rel_cause_e_type: 

SIDE EFFECTS
  None
===========================================================================*/
rrc_rel_cause_e_type tdsrrcrcr_update_release_cause
(
  tdsrrc_ReleaseCause release_cause  
);
#ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
/*===========================================================================
FUNCTION tdsrrcrcr_is_wait_t308_state

DESCRIPTION
  This function is used to check RCR is at wait T308 state.
  
DEPENDENCIES
  None

RETURN VALUE
 TRUE: RCR is waiting T308 expire.
 FALSE: RCR is not waiting T308 expier.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcrcr_is_wait_t308_state
(
  void
);
#endif

/*===========================================================================

FUNCTION tdsrrcrcr_is_interrat_redirection

DESCRIPTION 
  This function is used to check whether rcr msg indicating interrat redirection

DEPENDENCIES
    None.

RETURN VALUE
   TRUE:  rcr msg indicating interrat redirection
   FALSE: rcr msg not indicating interrat redirection

===========================================================================*/
boolean tdsrrcrcr_is_interrat_redirection(void);

/*===========================================================================
FUNCTION tdsrrcrcr_end_rcr_procedure

DESCRIPTION
  This function is used to end RCR procedure immediately.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_end_rcr_procedure(void);


#endif /* TDSRRCRCR_H */

