#ifndef TDSRRCPCRC_H
#define TDSRRCPCRC_H
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcpcreconfig.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/11   bj       Added code to handle receive R9 PhyscialChannelReconfig Message.
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/05/10   yzh      (3GPP: 8.6.6.15)Delete DPCH Compressed mode info(FDD only, under FEATURE_CM_SUPPORTED)(3.2.20).
11/03/10   zwj     Added code to handle REL4 OTA Message.
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/22/10   ysh     When Physical channel reconfiguration, compressed mode runtime error, reconfig failure,
                                 this function is FDD only, should delete.3GPP-8.2.11.2(3.2.7)

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
  /* Enumerated type of status of validating an PCRC message                   */
  typedef enum
  {
    TDSRRCPC_MSG_VALID,              /* Message is valid                        */
    TDSRRCPC_MSG_UNSUPPORTED_CONFIG, /* Message has un-supported configuration  */
    TDSRRCPC_PHY_CHAN_FAILURE,       /* Physical channel setup failed           */
    TDSRRCPC_SIMULTANEOUS_RECONFIG,  /* Incompatible simultaneous reconfig      */
    TDSRRCPC_MSG_PROTOCOL_ERROR,     /* Message has protocol error              */
    TDSRRCPC_MSG_INVALID_CONFIG,     /* Message has invalid configuration       */
    TDSRRCPC_MSG_INCOMPLETE_CONFIG,   /* Message has incomplete configuration    */
    TDSRRCPC_CELL_UPDATE_OCCURED     /* CELL update due to OOS occured          */
  }tdsrrcpc_msg_status_e_type;
  
  typedef struct
  {
    tdsrrcpc_msg_status_e_type failure_status; /* Failure cause                 */
    tdsrrc_ProtocolErrorCause  prot_err_cause; /* Protocol Error cause          */
  }tdsrrcpc_msg_failure_cause_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/

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
/* Structure to store all internal data related to PCRC procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type   curr_substate; /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;        /* Transaction ids for RR msg    */
  tdsrrc_state_e_type          state_indicated_by_msg;/* RRC State given by RR msg     */
  tdsrrcpc_msg_failure_cause_type status;     /* Status of message validation   */
  boolean                   new_crnti_valid; /* Indicates if a valid C-RNTI was recd 
                                            * in the message transitioning from CELL_DCH
                                            * to CELL_FACH */
  uint16                    new_crnti;
  boolean                   directed_cell_info; /* Indicates if cell info was recd */
 
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */ 
  boolean                      pcrc_for_srns_relocation;  /* Indicates SRNS reloc involved with 
                                                            this msg                             */
 
  boolean                       hho_ciph_status;
  boolean                       cn_info_saved;           /*flag to indicate that CN info is saved */
  mm_cmd_type *                 mm_cmd_ptr;              /*pointer to hold mm_cmd with cn info */

  boolean                       trans_to_dch_ciph_config_needed;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Indicates if Fre-redirection is to be triggered in FACH->PCH case */
  boolean initiate_cell_selection_trans_from_fach;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  boolean high_mobility_ind;

}tdsrrcpcrc_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/


/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCPCRC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBR procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_init_procedure( void );


/*===========================================================================

FUNCTION   RRCPCRC_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcpcrc_get_new_crnti_valid
(
  void
);
/*===========================================================================

FUNCTION   RRCPCRC_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcpcrc_get_current_sub_state
(
   void
);

/*===========================================================================

FUNCTION tdsrrcpcrc_build_pcr_complete_message

DESCRIPTION
  This function builds the PCR Complete message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcpcrc_build_pcr_complete_message
(    
  tdsrrc_RRC_TransactionIdentifier trans_id,        /* Transaction Id */
  tdsrrc_UL_DCCH_Message *msg_ptr,                  /* Pointer to Uplink DCCH Msg */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
);




/*===========================================================================

FUNCTION RRCPCRC_PROCEDURE_EVENT_HANDLER


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
void tdsrrcpcrc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);




/*===========================================================================

FUNCTION   PROCESS_PCRC_R4

DESCRIPTION

  This function processes the received Rel 4 PC Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a PCRC Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to reconfigure the Physical Channel given by the message.
    
    If TDSORDERED_CONFIG could not be set, a PCRC Failure message
    will be sent to UTRAN.

    The function is copy from tdsrrcpcrc_process_pcrc_r5
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_process_pcrc_r4
(
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION   VALIDATE_PCRC_R4

DESCRIPTION

  This function validates the received PC Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a PCRC Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

  The function is copy from copy from tdsrrcpcrc_validate_pcrc_r5,delete HSDPA handle
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcpcrc_validate_pcrc_r4
(
  tdsrrc_PhysicalChannelReconfiguration_r4_IEs *msg_ptr /* Pointer to the PCRC message */
);
#endif /* TDSRRCPCRC_H */

