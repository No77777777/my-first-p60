#ifndef TDSRRCRBR_H
#define TDSRRCRBR_H
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbr.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
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

#include "tdsrrcrbcommon.h"

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
#include "mmtask_v.h"
#endif

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

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* Structure to hold information about the RBs to be released.             */
typedef struct
{
  boolean is_valid;     /* indicates if there are RBs that need to be
                           released/affected. If this is FALSE, the two
                           items below are not used                       */
  uint32 num_rbs;       /* Number of RBs released by the message          */
  tdsrrc_RB_Identity rb_id[TDS_MAX_RB]; /* Hold the ids of the RBs that are
                           released/affected by the RBR message           */
}tdsrrcrbr_rb_info_type;

typedef struct {
  tdsrrc_RB_Identity rb_id; /* Hold the ids of the RBs */
  tdsrrc_rb_cfg_e_type rb_config;
  uint16 ul_cfg_mask;
  uint16 dl_cfg_mask;
}tdsrrcrbr_rb_id_action;

typedef struct {
  boolean is_valid;
  uint32 num_rbs;       /* Number of RBs tobe configured by the message          */
  tdsrrcrbr_rb_id_action rb_info[TDS_MAX_RB];
}tdsrrcrbr_rb_reconfig_info_type;

/* Structure to store all internal data related to RBR procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;      /* Transaction ids for RBR msg    */
  tdsrrc_state_e_type state_indicated_by_msg; /* RRC State given by RBR msg   */
  tdsrrcrb_msg_failure_cause_type status;   /* Status of message validation   */
  tdsrrcrbr_rb_info_type rbs_tobe_released; /* Information on the RBs to be
                                            released                       */
  tdsrrcrbr_rb_reconfig_info_type rbs_tobe_reconfigured; /* Information on the RBs
                                             to be reconfigured              */
  tdsrrcrbr_rb_info_type rbs_tobe_affected; /* Information on the RBs to be
                                            affected                       */
  tdsrrcrb_rab_info_per_msg_type rabs;      /* Hold info about each rab in one
                                         RBR message till they're processed.
                                         This is used when RABs need to be
                                         reconfigured.                     */
  boolean need_to_release_cn_domain;     /* TRUE if a signalling connection
                                         for a CN domain is to be released */
  rrc_cn_domain_identity_e_type cn_to_release;   /* The domain to be released      */

  boolean    new_crnti_valid;            /* Indicates if a valid C-RNTI was recd
                                          * in the message transitioning from CELL_DCH
                                          * to CELL_FACH
                                          */
  uint16     new_crnti;
  boolean    directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */
  boolean                      rbr_for_srns_relocation;  /* Indicates SRNS reloc involved with
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

}tdsrrcrbr_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/


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
void tdsrrcrbr_init_procedure( void );

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
boolean tdsrrcrbr_is_srns_in_progress
(
  void
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
boolean tdsrrcrbr_get_new_crnti_valid
(
  void
);
/*===========================================================================

FUNCTION   RRCRBR_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbr_get_current_sub_state
(
   void
) ;
/*===========================================================================

FUNCTION   VALIDATE_RBRELEASE_R4

DESCRIPTION

  This function validates the received Rel 5 RB Release Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Release Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbr_validate_rbrelease_r4
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Release message */
);

/*===========================================================================

FUNCTION   RRCRBR_PROCESS_RBRELEASE_R4

DESCRIPTION

  This function processes the received Rel 4 RB Release Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Release Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to release the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Release Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbr_process_rbrelease_r4
(
  tdsrrc_cmd_type *cmd_ptr
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
void tdsrrcrbr_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
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

void tdsrrcrbr_build_rbr_complete_message(
  tdsrrc_RRC_TransactionIdentifier tdstransaction_id,  /* Transaction Identifier to be sent in Uplink */
  tdsrrc_UL_DCCH_Message *ptr,                      /* Uplink DCCH message pointer */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
);

#endif /* TDSRRCRBR_H */

