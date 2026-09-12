#ifndef TDSRRCRBE_H
#define TDSRRCRBE_H
/*===========================================================================
  R R C   R A D I O   B E A R E R   E S T A B L I S H M E N T   M O D U L E 

DESCRIPTION

  This module is the header module for the Radio Bearer Establishment module.
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbe.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/11   bj       Added code to handle REL9 OTA Message.
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/03/10   rl      Fixed compiler warnings
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
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

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* Structure to store all internal data related to RBE procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type tr_ids;      /* Transaction ids for RBS msg    */
  tdsrrc_state_e_type state_from_msg;       /* RRC State given by RBS msg     */
  tdsrrcrb_msg_failure_cause_type status;   /* Status of message validation   */
  tdsrrcrb_rab_info_per_msg_type  rabs;      /* Hold info about each rab in one
                                         RBS message till they're processed*/
  boolean                      new_crnti_valid;           /* Indicates if a valid C-RNTI was
                                                            recd in the message transitioning
                                                            from CELL_DCH to CELL_FACH       */
  uint16                       new_crnti;
  boolean                      directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  tdsrrc_int_u_rnti_type          new_urnti;
  boolean                      new_urnti_valid;          /* Indicates if a valid U-RNTI was recd */
  boolean                      rbe_for_srns_relocation;  /* Indicates SRNS reloc involved with
                                                            this msg                             */
  boolean                       cn_info_saved;           /*flag to indicate that CN info is saved */
  mm_cmd_type *                 mm_cmd_ptr;              /*pointer to hold mm_cmd with cn info */
  boolean                       hho_ciph_status;
  boolean                       trans_to_dch_ciph_config_needed;


  boolean first_tm_rb_setup;
}tdsrrcrbe_int_data_type;
#endif/*FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE*/


/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCRBE_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBE procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_init_procedure( void );

#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION   RRCRBE_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbe_is_srns_in_progress
(
  void 
);
#endif
/*===========================================================================

FUNCTION   RRCRBE_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbe_get_new_crnti_valid
(
  void
);
/*===========================================================================

FUNCTION   PROCESS_RBSETUP_R4

DESCRIPTION

  This function processes the Rel 4 RB Setup Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Setup Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -

    TDSORDERED_CONFIG will be set by calling LLC.

    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.

    If TDSORDERED_CONFIG could not be set, a Radio Bearer Setup Failure message
    will be sent to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_process_rbsetup_r4
(
tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION   VALIDATE_RBSETUP_R4

DESCRIPTION

  This function validates the received R4 RB Setup Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Setup Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_validate_rbsetup_r4
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
);

/*===========================================================================

FUNCTION   RRCRBE_SEND_CHANNEL_CONFIG_REQ_R4

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to setup the necessary RBs associated with a RAB. If signalling
  Radio bearers are affected and included in the Radio Bearer
  Setup Message, they will also be sent in the same command.

  Currently the messege is not expected to have Signalling
  Radio Bearers for setup. If these are added in the future,
  they will also need to be sent in the same command.

DEPENDENCIES

  TDSORDERED_CONFIG must be set before calling this function.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbe_send_channel_config_req_r4
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
);


/*===========================================================================

FUNCTION   RRCRBE_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbe_get_current_sub_state
(
   void
);

/*===========================================================================

FUNCTION tdsrrcrbe_get_est_rab_ptr

DESCRIPTION

  This function returns ptr to established rabs to requesting
  procedures
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Pointer to rab info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_info_per_msg_type *tdsrrcrbe_get_est_rab_ptr(void);
/*===========================================================================

FUNCTION RRCRBE_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Radio Bearer Establishment procedure. This function is called by the
  dispatcher whenever an event is received for this module. This function
  checks the current RRC state and calls the event handler for
  that state. 
  
  Note that the Radio Bearer Establishment procedure can be initiated
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
void tdsrrcrbe_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================
FUNCTION   tdsrrcrbe_validate_rbs_in_given_rab

DESCRIPTION

DEPENDENCIES
  YTBD
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsrrcrbe_validate_rbs_in_given_rab
(
  tdsrrc_RB_InformationSetupList_r7 *rb_setup_list,
  uint8 rab_idx
);

/*===========================================================================
FUNCTION   tdsrrcrbe_prevalidate_rab_setup_list_r7

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcrbe_prevalidate_rab_setup_list_r7
(
  tdsrrc_RAB_InformationSetupList_r7 *rab_list_ptr
);

/*===========================================================================
FUNCTION   tdsrrcrbe_validate_rab_setup_list_r7

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcrbe_validate_rab_setup_list_r7
(
  tdsrrc_RAB_InformationSetupList_r7 * rab_list_ptr,
  uint32 no_of_rabs,
  uint32 nas_sync_ind
);

/*===========================================================================

FUNCTION   tdsrrcrbe_send_rabm_est_ind_to_upper_layers

DESCRIPTION
  This function sends RABMAS_RAB_ESTABLISH_IND primitive
  to the RABM layer of NAS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void tdsrrcrbe_send_rabm_est_ind_to_upper_layers
(
  void
);

/*===========================================================================

FUNCTION   RRCRBE_VALIDATE_SRNS_RELOCATION_INFO()

DESCRIPTION

  This function checks whether SRNS relocation info is present or not

DEPENDENCIES

  None.

RETURN VALUE

  Boolean. TRUE : SRNS relocation info is present
           FALSE : SRNS relocation info is not present

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbe_validate_srns_relocation_info
(
tdsrrc_RadioBearerSetup *msg_ptr      /* Pointer to the RB Setup message */
,tdsrrc_msg_ver_enum_type msg_version
);

#endif /* TDSRRCRBE_H */

