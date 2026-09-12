#ifndef TDSRRCTCRC_H
#define TDSRRCTCRC_H
/*===========================================================================
         R R C   R A D I O   B E A R E R   R E L E A S E   M O D U L E 

DESCRIPTION

  This module is the header module for the Radio Bearer Release module.
  This contains the external interface functions to be used by
  by other RRC modules. This must not be used by modules outside RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctcreconfig.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/02/10   zwj     Added code to handle REL4 OTA Message.

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
#include "mmtask_v.h"
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  /* Enumerated type of the different substates for this procedure           */
  
  /* Enumerated type of status of validating an RB message                   */
  typedef enum
  {
    TDSRRCTC_MSG_VALID,              /* Message is valid                        */
    TDSRRCTC_MSG_UNSUPPORTED_CONFIG, /* Message has un-supported configuration  */
    TDSRRCTC_PHY_CHAN_FAILURE,       /* Physical channel setup failed           */
    TDSRRCTC_SIMULTANEOUS_RECONFIG,  /* Incompatible simultaneous reconfig      */
    TDSRRCTC_MSG_PROTOCOL_ERROR,     /* Message has protocol error              */
    TDSRRCTC_MSG_INVALID_CONFIG,     /* Message has invalid configuration       */
    TDSRRCTC_MSG_INCOMPLETE_CONFIG,   /* Message has incomplete configuration    */
    TDSRRCTC_CELL_UPDATE_OCCURED
  }tdsrrctc_msg_status_e_type;
  
  typedef struct {
    tdsrrctc_msg_status_e_type failure_status; /* Failure cause                 */
    tdsrrc_ProtocolErrorCause  prot_err_cause; /* Protocol Error cause          */
  }tdsrrctc_msg_failure_cause_type;
#endif
/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* Structure to store all internal data related to TCRC procedure           */
typedef struct {
  tdsrrcrb_substate_e_type      curr_substate; /* Store the current substate     */
  tdsrrcrb_transaction_id_type    tr_ids;      /* Transaction ids for RR msg    */
  tdsrrc_state_e_type             state_indicated_by_msg; /* RRC State given by RR msg     */
  tdsrrctc_msg_failure_cause_type status;   /* Status of message validation   */
  boolean                      new_crnti_valid; /* Indicates if a valid C-RNTI was recd 
                                          * in the message transitioning from CELL_DCH
                                          * to CELL_FACH
                                          */
  uint16                       new_crnti;
  boolean                      directed_cell_info; /* Indicates if a valid C-RNTI was recd */

  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */ 
  boolean                      tcrc_for_srns_relocation;  /* Indicates SRNS reloc involved with 
                                                          this msg                             */

  boolean                       hho_ciph_status;

  boolean                       cn_info_saved;           /*flag to indicate that CN info is saved */
  mm_cmd_type *                 mm_cmd_ptr;              /*pointer to hold mm_cmd with cn info */

  boolean                      trans_to_dch_ciph_config_needed; /*indicates that we are going from
                                                                  non-dch to dch and needs ciph
                                                                  config when tm bearers are present
                                                                  */
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Indicates if Fre-redirection is to be triggered in FACH->PCH case */
  boolean initiate_cell_selection_trans_from_fach;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


  boolean high_mobility_ind;

}tdsrrctcrc_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/


/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCTCRC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBR procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctcrc_init_procedure( void );


/*===========================================================================

FUNCTION   RRCTC_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrctcrc_get_new_crnti_valid
(
  void
);

/*===========================================================================

FUNCTION   RRCTCRC_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrctcrc_get_current_sub_state
(
   void
);

/*===========================================================================

FUNCTION   VALIDATE_TCRECONFIG_R4

DESCRIPTION

  This function validates the received R4 TCRC Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a TCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctcrc_validate_tcreconfig_r4
(
  tdsrrc_TransportChannelReconfiguration *msg_ptr /* Pointer to the TCRC message */
);
/*===========================================================================

FUNCTION   PROCESS_TCRECONFIG_R4

DESCRIPTION

  This function processes the Rel 4 TCRC Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a TCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the transport channels given by the message.
    
    If TDSORDERED_CONFIG could not be set, a TCRC Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctcrc_process_tcreconfig_r4
(
  tdsrrc_cmd_type *cmd_ptr      /* Pointer to the TCRC message */
);





/*===========================================================================

FUNCTION tdsrrctcrc_build_tcr_complete_message

DESCRIPTION
  This function builds TCR Complete message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrctcrc_build_tcr_complete_message
(    
  tdsrrc_RRC_TransactionIdentifier trans_id,        /* Transaction Id */
  tdsrrc_UL_DCCH_Message *msg_ptr,                  /* Pointer to Uplink DCCH Msg */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
);




/*===========================================================================

FUNCTION RRCTCRC_PROCEDURE_EVENT_HANDLER


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
void tdsrrctcrc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);


#endif /* TDSRRCTCRC_H */

