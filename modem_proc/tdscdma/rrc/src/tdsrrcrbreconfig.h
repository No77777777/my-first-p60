#ifndef TDSRRCRBRRC_H
#define TDSRRCRBRRC_H
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbreconfig.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
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

#include "tdsrrcdt.h"
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

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
typedef enum
{
  TDSRB_STOP_CONTINUE_ABSENT=0,
  TDSRB_STOP,
  TDSRB_CONTINUE
}tdsrrcrbrc_rlc_stop_cont_e_type;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

typedef struct
{
  boolean is_valid;     /* indicates if there are RBs that need to be
                           released/affected. If this is FALSE, the two
                           items below are not used                       */
  uint32 num_rbs;       /* Number of RBs released by the message          */
  tdsrrc_RB_Identity rb_id[TDS_MAX_RB]; /* Hold the ids of the RBs that are
                           released/affected by the RBR message           */
  /* Indicate if for this ie, stop or continue is specified. */
  tdsrrcrbrc_rlc_stop_cont_e_type  rb_stop_continue[TDS_MAX_RB];
  uint16 ul_cfg_mask[TDS_MAX_RB];
  uint16 dl_cfg_mask[TDS_MAX_RB];
}tdsrrcrbrc_rb_info_type;


/* Structure to store all internal data related to RBRC procedure           */
typedef struct
{
  tdsrrcrb_substate_e_type         curr_substate;  /* Store the current substate     */
  tdsrrcrb_transaction_id_type     tr_ids;      /* Transaction ids for RR msg    */
  tdsrrc_state_e_type              state_indicated_by_msg; /* RRC State given by RR msg     */
  tdsrrcrb_msg_failure_cause_type  status;   /* Status of message validation   */
  tdsrrcrbrc_rb_info_type          rbs_tobe_reconfigured;/* RBs to be reconfigured*/
  tdsrrcrbrc_rb_info_type          rbs_tobe_affected; /* RBs to be affected       */
  tdsrrcrb_rab_info_per_msg_type   rabs;      /* Hold info about each rab in one
                                         RB RC message till they're processed*/
  boolean                       directed_cell_info; /* Indicates if a valid C-RNTI was recd */
  boolean                       new_crnti_valid; /* Indicates if a valid C-RNTI was recd 
                                                  * in the message transitioning from CELL_DCH
                                                  * to CELL_FACH */
  uint16                        new_crnti;
  tdsrrc_int_u_rnti_type           new_urnti;
  boolean                       rbrc_for_srns_relocation;
  boolean                       cn_info_saved;
  mm_cmd_type *                 mm_cmd_ptr;
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER    
  boolean                       srns_reallocation_trigger;
#endif
  boolean                       hho_ciph_status;

  boolean                       trans_to_dch_ciph_config_needed;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
 /* Indicates if Fre-redirection is to be triggered in FACH->PCH case */
  boolean initiate_cell_selection_trans_from_fach;
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  boolean high_mobility_ind;

}tdsrrcrbrc_int_data_type;
#endif

/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION  RRCRBRC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the RBR procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbrc_init_procedure( void );

/*===========================================================================

FUNCTION   PROCESS_RBRC_MESSAGE_R4

DESCRIPTION

  This function processes the received RB Reconfig Message. It first
  validates the message. If the message has an invalid or unsupported
  configuration, a Radio Bearer Reconfig Failure message will be sent to
  the UTRAN. If the message is valid, it will be processed as follows -
  
    TDSORDERED_CONFIG will be set by calling LLC. 
    
    If TDSORDERED_CONFIG could be set without any conflicts, a RRC_CHANNEL_CONFIG
    _REQ will be sent to LLC to configure the Radio Bearers given by the message.
    
    If TDSORDERED_CONFIG could not be set, a Radio Bearer Reconfig Failure message
    will be sent to UTRAN.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcrbrc_process_rbrc_message_r4
(
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION   RRCRBRC_UPDATE_ORDERED_CONFIG_R4

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. If the required Radio-bearers have been reconfig succesfully,
  and if any of the radio-bearers map to radio-access-bearers, this
  information needs to be given to the NAS entity for that RAB.
  
  Currently Circuit-Switched voice and Circuit-Switched data RABs are 
  supported. If a voice RAB has been assigned by the UTRAN, this function
  gets the RLC ids and the AMR mode information from LLC and sends this
  information to voice services. If a CS Data RAB has been assigned, the
  RLC ids for those RBs are sent to the data services entity. Then this
  function sends the RRC_SYNC_IND primitive to MM.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrbrc_update_ordered_config_r4
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,      /* Pointer to the RB Reconfig message */
  tdsrrc_msg_ver_enum_type                   msg_version
);
/*===========================================================================

FUNCTION   VALIDATE_RBRC_MESSAGE_R4

DESCRIPTION

  This function validates the received RB Reconfig Message. This includes
  checking for unsupported or invalid configurations, unknown extensions
  and invalid IEs.

  If a Radio Bearer Reconfig Failure message needs to be sent due to
  a problem with the message, this procedure stores the failure cause
  to be used while sending the failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.s

===========================================================================*/
void tdsrrcrbrc_validate_rbrc_message_r4
(
  tdsrrc_RadioBearerReconfiguration *msg_ptr /* Pointer to the RB RC message */
);



/*===========================================================================

FUNCTION   RRCRBRC_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbrc_get_new_crnti_valid
(
  void
);

/*===========================================================================

FUNCTION   RRCRBRC_GET_CURRENT_SUB_STATE()

DESCRIPTION
 This function is used to return current substate


        
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcrb_substate_e_type current_substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrbrc_get_current_sub_state
(
   void
) ;

/*===========================================================================

FUNCTION tdsrrcrbrc_build_rbrc_complete_message

DESCRIPTION
  This function builds the RBRC Complete Message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcrbrc_build_rbrc_complete_message
(    
  tdsrrc_RRC_TransactionIdentifier trans_id,        /* Transaction Id */
  tdsrrc_UL_DCCH_Message *msg_ptr,                  /* Pointer to Uplink DCCH Msg */
  boolean              srns_reloc,               /* Indicates whether SRNS reloc is involved */ 
  boolean              cipher_update_required    /* Indicates whether TM ciphering is present */
);





/*===========================================================================

FUNCTION RRCRBRC_PROCEDURE_EVENT_HANDLER


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
void tdsrrcrbrc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);


#endif /* TDSRRCRBRRC_H */

