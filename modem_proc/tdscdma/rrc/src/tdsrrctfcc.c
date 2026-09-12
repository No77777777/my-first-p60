/*===========================================================================
              R R C TRANSPORT FORMAT COMBINATION CONTROL M O D U L E 

DESCRIPTION

  This module performs the Transport Format combination Control Procedure. This
  module processes the TFCC Message and onfigures the transport format
  combination subset.
  
  
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctfcc.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/06/11   ttl     Modified OTA TFCI according to the way we stored.
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/17/11   ad      Made changes in RRC related to L2 SW Decoupling
01/12/11   ad      Added changes for RRC SW decoupling
01/04/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.87.50
12/15/10   su      Fixed Compilation warnings.
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/02/10   su      Provided fix to process TFCC message while another 
                   Reconfiguration is on going.
11/29/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.85.50
11/19/10   ssg     Added changes for new WCDMA-L2 interface.
11/11/10   yzh     Modify TFCC modeSpecificInfo from FDD to TDD.
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "err.h"
#include "msg.h"
#include "memheap.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "rex.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccommon.h"
#include "tdsrrcintif.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrclcm.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsend.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"

#include "tdsrrctfcc.h"
#ifdef TEST_FRAMEWORK
#error code not present
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
tdsmac_tfc_subset_restriction_info_type tdsmac_tfc_subset_info;
tdsrrctfcc_substates_e_type tdsrrctfcc_substate;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION   RRCTFCC_SEND_TFCC_CONFIG_FAILURE

DESCRIPTION
  This function prepares TFCC config failure message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void  tdsrrctfcc_send_tfcc_config_failure
(
  uint8 tdstransaction_id 
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr;         /* Pointer to uplink DCCH message */ 
  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */
  tdsrrcsend_status_e_type send_status;    /* The status of sending the Initial
                                         * Security Mode Failure */

  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                       DCCH_AM_RADIO_BEARER_ID, 
                                       UE_MODE_ACKNOWLEDGED_DATA);

  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    if ((msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                                                    sizeof(struct tdsrrc_UL_DCCH_Message))) != NULL)
    {
      msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_transportFormatCombinationControlFailure;

      /* No non Critical Extensions present.  */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.transportFormatCombinationControlFailure);

      msg_ptr->message.u.transportFormatCombinationControlFailure.
                                          rrc_TransactionIdentifier = tdstransaction_id;

      msg_ptr->message.u.transportFormatCombinationControlFailure.
                                failureCause.t = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_TFC,
                                           msg_ptr,
                                           ul_lc_id,
                                           TDSRRCSEND_L2ACK_NOT_REQUIRED);

      if (send_status == TDSRRCSEND_SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC Failure Message Sent");      
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending TFCC FAILURE",send_status);
      }
    }
    else
    {
      ERR_FATAL("Mem alloc failed",0,0,0);
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"LC ID not found in tx'n TFCC failure");
  }

}


/*===========================================================================

FUNCTION   RRCTFCC_GET_DURATION

DESCRIPTION
  Convert TFCC duration value into msec.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static uint16 tdsrrctfcc_get_duration(tdsrrc_TFC_ControlDuration tfcc_duration)
{
  uint16 duration = 0;    /* the default duration is 0. */

  switch (tfcc_duration)
  {
    case tdsrrc_TFC_ControlDuration_tfc_cd1:
      duration = 10;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd2:
      duration = 20;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd4:
      duration = 40;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd8:
      duration = 80;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd16:
      duration = 160;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd24:
      duration = 240;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd32:
      duration = 320;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd48:
      duration = 480;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd64:
      duration = 640;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd128:
      duration = 1280;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd192:
      duration = 1920;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd256:
      duration = 2560;
      break;

    case tdsrrc_TFC_ControlDuration_tfc_cd512:
      duration = 5120;
      break;
  }

  return duration;
}


#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION   RRCTFCC_UPDATE_TFCC_RESTRICTION_IN_MAC

DESCRIPTION
  This function updates the TFCC restriction in MAC.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/  
static void tdsrrctfcc_update_tfcc_restriction_in_mac
(
  tdsmac_tfc_subset_restriction_info_type *mac_info, 
  tdstfc_subset_info_type                 *rrc_tfc_info
)
{
  uint32 i, j;

  switch(rrc_tfc_info->restriction_choice)
  {
    case TDSMINIMUM_ALLOWED_TFC:
      mac_info->tfci_restriction_choice = TDSMIN_ALLOWED_TFCI;
      tdsmac_tfc_subset_info.restriction_info.tfci = 
        (uint16)rrc_tfc_info->tfc_choice_info.minimum_allowed_tfc_number;
      break;
  
    case TDSALLOWED_TFC_LIST:
      mac_info->tfci_restriction_choice = TDSALLOWED_TFCI;
      tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc = 
        rrc_tfc_info->tfc_choice_info.allowed_tfc_list.total_allowed_tfc_count;
      
      i = 0;
      while(i < tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc)
      {
        tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[i] = 
          rrc_tfc_info->tfc_choice_info.allowed_tfc_list.allowed_tfc_number[i];
        i++;
      }
      break;
  
    case TDSNON_ALLOWED_TFC_LIST:
      mac_info->tfci_restriction_choice = TDSNOT_ALLOWED_TFCI;
      tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc = 
        rrc_tfc_info->tfc_choice_info.non_allowed_tfc_list.total_non_allowed_tfc_count;
  
      i = 0;
      while(i < tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc)
      {
        tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[i] = 
          rrc_tfc_info->tfc_choice_info.non_allowed_tfc_list.non_allowed_tfc_number[i];
        i++;
      }
      break;
  
    case TDSRESTRICTED_TRCH_LIST:
      mac_info->tfci_restriction_choice = TDSRESTRICTED_TFCI;
      tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.ntrch = 
        rrc_tfc_info->tfc_choice_info.restricted_trch_list.no_of_transport_channels;
      
      i = 0;
      while(i < tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.ntrch)
      {
        tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.trch_restriction_info[i].trch_id = 
          (tr_ch_id_type)rrc_tfc_info->tfc_choice_info.restricted_trch_list.trch_info[i].trch_id;
        if(rrc_tfc_info->tfc_choice_info.restricted_trch_list.trch_info[i].dch_present)
        {
          tdsmac_tfc_subset_info.restriction_info.
            trch_restriction_info_list.trch_restriction_info[i].trch_type = TDSDCH;
        }
        else
        {
          tdsmac_tfc_subset_info.restriction_info.
            trch_restriction_info_list.trch_restriction_info[i].trch_type = TDSINVALID_UL_CCTRCH;
        }
        
        if( rrc_tfc_info->tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info_present)
        {
          tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.trch_restriction_info[i].ntf = 
            rrc_tfc_info->tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info.no_of_allowed_tfis;
         
          j = 0;
          while(j < tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.trch_restriction_info[i].ntf)
          {
            tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.trch_restriction_info[i].tfi[j] = 
              rrc_tfc_info->tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info.tfi[j];
            j++;
          }
        }
        ++i;
      }
      break;
    
    case TDSTFCS_FULL:
      tdsmac_tfc_subset_info.tfci_restriction_choice = TDSFULL_TFCI;
      break;
    
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Err t .. should not come this far");
      break;
  }
} /* tdsrrctfcc_update_tfcc_restriction_in_mac */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
/*===========================================================================

FUNCTION   RRCTFCC_PROCESS_MESSAGE

DESCRIPTION
  This function process the received TFCC message.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrctfcc_process_message
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  tdsrrc_TFC_Subset *tfc_subset = NULL;
  int ntfci = 0, ntrch = 0, ntfi = 0;
  boolean invalid_config = FALSE;
  tdsrrc_AllowedTFC_List *allow_tfc_list = NULL;
  tdsrrc_Non_allowedTFC_List *non_allow_tfc_list = NULL;
  tdsrrc_RestrictedTrChInfoList *trch_list = NULL;
  tdsrrc_AllowedTFI_List *allow_tfi_list = NULL;
  tdsl2_ul_cmd_type  *l2_ul_cmd_ptr;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  boolean tm_tfcc_msg_recvd = FALSE;
  uint8 tfc_subset_idx = 0;
  tdsrrc_RB_Identity tfcc_rb_id;  
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
  uint32 idx=0,idy=0;

  if(tdsrrctfcc_substate == TDSRRCTFCC_WAIT_FOR_MAC_CFG_CNF)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Recvd TFCC message when waiting for CNF from MAC");
  }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tfcc_rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);

  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU)
  {
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */    
    if ( cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL )
    {
      /* Get the OTA message pointer */
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                      cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Recvd OTA msg for TFCC ");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rx'd empty TFCC msg");
      return;
    }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */      
  
  
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU)
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if((tfcc_rb_id == DCCH_AM_RADIO_BEARER_ID) ||
       (tfcc_rb_id == DCCH_UM_RADIO_BEARER_ID))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC received on UM/AM .. RBId - %d",tfcc_rb_id);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      if (!(TDSRRC_MSG_COMMON_BITMASK_IE(dcch_msg_ptr->message.u.transportFormatCombinationControl,
          tdsrrc_TransportFormatCombinationControl,rrc_TransactionIdentifier)))
      { 
        invalid_config = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No TransID presents in TFCC MSG");
      } 

      /* Make sure that TDD option is not presented.  */
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo,
          rrc_TransportFormatCombinationControl_modeSpecificInfo_fdd))
      {
        invalid_config = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Do not support FDD option in TFCC");
      }
      if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
           ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.transportFormatCombinationControl,
             activationTimeForTFCSubset)) || 
             (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.transportFormatCombinationControl,
              tfc_ControlDuration))))
      {
         invalid_config = TRUE;
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCC msg received in Cell FACH state with Action Time/Control duration");
      }
      if(tdsrrctfcc_substate == TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
      {
        invalid_config = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Recvd 2nd TFCC message when we are procesing one TFCC");
      }
      /* Check TFCS ID in TFCC OTA message is legal */
      if(dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.m.tfcs_IDPresent)
      {
        if ((dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID < 1) ||
           (dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID > 8))
        {
          invalid_config = TRUE;
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Recvd invalid TFCS ID,out of range",dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID);
        }
		TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID is %d in TFCC OTA",dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID);
        /* Only support one TFCS current */
	    /* add check tfcs id in OC is for the situation: TFCC OTA is receive during reconfig procedure is running */
		/* add check tfcs id in TOC is for save trch info in CELL_FACH  */
		if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
        {
          /*TFCC message is received in FACH state*/
          /* Trch info present in TOC */
 		  if((tdsordered_config.set_status == TDSOC_SET_FOR_CELL_DCH) &&
			    (tdsordered_config_ptr->ul_tfcs_id[0] == 
               dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID))
		  {
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID match:%d in OC in CELL_FACH",tdsordered_config_ptr->ul_tfcs_id[0]);
		  }
          else if((tdstransition_config.toc_usage == TDSTOC_FOR_DCH) && 
             (tdstransition_config.toc_ptr->ul_tfcs_id[0] == 
             dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID))
          {
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID match:%d in TOC in CELL_FACH",tdstransition_config.toc_ptr->ul_tfcs_id[0]);
          }
		  else if(tdscurrent_config_ptr->ul_tfcs_id[0] == 
               dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID)
		  {
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID match:%d in CC in CELL_FACH",tdscurrent_config_ptr->ul_tfcs_id[0]);
		  }
		  else
		  {
           invalid_config = TRUE;
           TDSRRC_MSG2(MSG_LEGACY_ERROR,"TFCS ID not match in CELL_FACH CC = %d,TOC = %d",tdscurrent_config_ptr->ul_tfcs_id[0],tdstransition_config.toc_ptr->ul_tfcs_id[0]);
		  }
        }
        else
        {
		  if((tdsordered_config.set_status != TDSOC_NOT_SET) && 
			  (tdsordered_config_ptr->ul_tfcs_id[0] == 
             dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID))
		  {
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID match:%d in OC in CELL_DCH",tdsordered_config_ptr->ul_tfcs_id[0]);
		  }
		  else if(tdscurrent_config_ptr->ul_tfcs_id[0] == 
               dcch_msg_ptr->message.u.transportFormatCombinationControl.modeSpecificInfo.u.tdd.tfcs_ID.tfcs_ID)
		  {
			  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCS ID match:%d in CC in CELL_DCH",tdscurrent_config_ptr->ul_tfcs_id[0]);
		  }
		  else
		  {
              invalid_config = TRUE;
			  TDSRRC_MSG2(MSG_LEGACY_ERROR,"TFCS ID not match in CELL_DCH: CC = %d,OC = %d",tdscurrent_config_ptr->ul_tfcs_id[0],tdsordered_config_ptr->ul_tfcs_id[0]);
		  }
		}
      }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  else if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE)
  {
    if((tdsrrcllc_has_config_srb5(TDSCURRENT_CONFIG, (uint16)tfcc_rb_id) == TRUE)
      &&(tdsrrctfcc_substate != TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
    )
    {  
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC received on TM mode for RBId - %d",tfcc_rb_id);

      tfc_subset_idx = (*((uint8 *)cmd_ptr->cmd.downlink_sdu_ind.dl_sdu)) >> 5;
  
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current config has %d subset lists; TFC subset index -> %d",tdscurrent_config_ptr->ul_tfc_subset_list.no_of_subset_lists,tfc_subset_idx);

      /* Only entertain TFC subset index for which we have
         TFC subset list information */
      if(tfc_subset_idx < tdscurrent_config_ptr->ul_tfc_subset_list.no_of_subset_lists)
      {
        tm_tfcc_msg_recvd = TRUE;
        /* Retrieve the corresponding TFC subset information to
           be passed to MAC */

        /* Reset local config memory to have activation time now
           & transaction ID as ZERO */
        memset(&tdsmac_tfc_subset_info, 0, sizeof(tdsmac_tfc_subset_restriction_info_type));
        tdsmac_tfc_subset_info.rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);
        tdsmac_tfc_subset_info.control_duration = 0; /* ZERO signifies that control duration is applicable till next configuration */ 
        tdsmac_tfc_subset_info.activation_time = 0;

        /* copy the relevant TFC choices * restrictions here */
        tdsrrctfcc_update_tfcc_restriction_in_mac(&tdsmac_tfc_subset_info, &tdscurrent_config_ptr->ul_tfc_subset_list.tfc_subset_info[tfc_subset_idx]);
      }
      else
      {
        invalid_config = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid TFCC recvd on TM .. rejecting");
      }
    }
    else
    {
      invalid_config = TRUE;
      if(tdsrrctfcc_substate == TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Recvd 2nd TFCC message when we are procesing one TFCC");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 doesn't exist ... TFCC is recvd on TM .. rejecting");
      }
    }
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
  else
  {
    invalid_config = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCC received on unknown RLC mode");
  }



  if (!invalid_config
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    && tm_tfcc_msg_recvd == FALSE
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    )
  {
    {
      /* Reset local config memory  */
      memset(&tdsmac_tfc_subset_info, 0, sizeof(tdsmac_tfc_subset_restriction_info_type));

      tfc_subset = &(dcch_msg_ptr->message.u.transportFormatCombinationControl.dpch_TFCS_InUplink);

      /* Pass RB ID and Transaction ID to MAC. So that once MAC identifies that
       * the TFCC config is invalid, MAC can send the failure report to RRC along
       * w/ these info. RRC then can decide to send a Config Failure report to NW 
       * if the TFCC message is received in SRB2.
       */
      tdsmac_tfc_subset_info.rb_id = tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);
      tdsmac_tfc_subset_info.transaction_id = 
          (uint8)dcch_msg_ptr->message.u.transportFormatCombinationControl.rrc_TransactionIdentifier;

      /* Save Activation time.  */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.transportFormatCombinationControl,
          activationTimeForTFCSubset))
      {
        tdsmac_tfc_subset_info.activation_time = 
          (uint16)dcch_msg_ptr->message.u.transportFormatCombinationControl.activationTimeForTFCSubset;
      }
      else
      {
        tdsmac_tfc_subset_info.activation_time = 0;
      }

      /* TFCC duration is presented. Save the control Duration. */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(dcch_msg_ptr->message.u.transportFormatCombinationControl,
          tfc_ControlDuration))
      {
        tdsmac_tfc_subset_info.control_duration = 
          tdsrrctfcc_get_duration(dcch_msg_ptr->message.u.
                               transportFormatCombinationControl.tfc_ControlDuration);
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RBId %d, TransId %d t %d", tdsmac_tfc_subset_info.rb_id, 
             tdsmac_tfc_subset_info.transaction_id,tfc_subset->t);

      /* There are five options of TFC subset representations. */
      switch (tfc_subset->t)
      {
        case T_tdsrrc_TFC_Subset_minimumAllowedTFC_Number:
          tdsmac_tfc_subset_info.tfci_restriction_choice = TDSMIN_ALLOWED_TFCI;

          /* The config'ing minimum TFCI > the higest of the current config'd tfci. */
          /* Since we start from TFCI 0, the OTA value is always 1 bigger */
          if (tfc_subset->u.minimumAllowedTFC_Number >= (tdscurrent_config_ptr->ul_tfcs.ntfc + 1))
          {
            invalid_config = TRUE;
            break;
          }
          tdsmac_tfc_subset_info.restriction_info.tfci = (uint16)tfc_subset->u.minimumAllowedTFC_Number;
          /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
          if (tdsmac_tfc_subset_info.restriction_info.tfci > 0)
          {
            tdsmac_tfc_subset_info.restriction_info.tfci --;
          }
          else 
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in Min_allowedTFC",tdsmac_tfc_subset_info.restriction_info.tfci);
          }

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MInAllowed TFCI %d",tdsmac_tfc_subset_info.restriction_info.tfci);
          break;

        case T_tdsrrc_TFC_Subset_allowedTFC_List:
          tdsmac_tfc_subset_info.tfci_restriction_choice = TDSALLOWED_TFCI;

          ntfci = 0;
          allow_tfc_list = tfc_subset->u.allowedTFC_List;
          idx=0;

          while (allow_tfc_list->n > idx)
          {
            /* For each tfci, it should not greater than the highest config'd TFCI  */
            /* Since we start from TFCI 0, the OTA value is always 1 bigger */
            if (allow_tfc_list->elem[idx] >= (tdscurrent_config_ptr->ul_tfcs.ntfc + 1))
            {
              invalid_config = TRUE;
              break;
            }
            tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] = 
                                                                      (uint16)allow_tfc_list->elem[idx];
            /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
            if (tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] > 0)
            {
              tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] --;
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in allowedTFC",allow_tfc_list->elem[idx]);
            }

            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Allowed TFCI %d,ntfc %d",tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci],ntfci);
            ntfci ++;
            if (ntfci == TDSUE_MAX_TFC)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many allow TFCI");
              invalid_config = TRUE;
              break;
            }
            idx++;
          }
          tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc = (uint16)ntfci;
          break;

        case T_tdsrrc_TFC_Subset_non_allowedTFC_List:
          tdsmac_tfc_subset_info.tfci_restriction_choice = TDSNOT_ALLOWED_TFCI;

          ntfci = 0;
          non_allow_tfc_list = tfc_subset->u.non_allowedTFC_List;
          idx=0;

          while (non_allow_tfc_list->n > idx)
          {
            /* For each tfci, it should not greater than the highest config'd TFCI  */
            /* Since we start from TFCI 0, the OTA value is always 1 bigger */
            if (non_allow_tfc_list->elem[idx] >= (tdscurrent_config_ptr->ul_tfcs.ntfc + 1))
            {
              invalid_config = TRUE;
              break;
            }
            tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] = 
              (uint16)non_allow_tfc_list->elem[idx];
            /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
            if (tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] > 0)
            {
              tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci] --;
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in Non_allowedTFC",non_allow_tfc_list->elem[idx]);
            }

            TDSRRC_MSG2(MSG_LEGACY_HIGH,"NotAllowed TFCI %d,ntfc %d",tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.tfci_list[ntfci],ntfci);
            ntfci ++;
            if (ntfci == TDSUE_MAX_TFC)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Too many non_allow TFCI");
              invalid_config = TRUE;
              break;
            }
            idx++;
          }
          tdsmac_tfc_subset_info.restriction_info.tfci_restriction_info.ntfc = (uint16)ntfci;
          break;

        case T_tdsrrc_TFC_Subset_restrictedTrChInfoList:
          tdsmac_tfc_subset_info.tfci_restriction_choice = TDSRESTRICTED_TFCI;

          /* For each Transport Channel, store the restriction info */
          ntrch = 0;
          trch_list = tfc_subset->u.restrictedTrChInfoList;
          idx=0;

          while (trch_list->n > idx)
          {
            tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.
              trch_restriction_info[ntrch].trch_id = (uint16)trch_list->elem[idx].restrictedTrChIdentity;

            if(trch_list->elem[idx].ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
            {
              tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.
                trch_restriction_info[ntrch].trch_type = TDSDCH;
            }
            else
            {
              invalid_config = TRUE;
              break;
            }
            ntfi = 0;
            if (trch_list->elem[idx].m.allowedTFI_ListPresent)
            {
              /* Store the TFI list.  */
              allow_tfi_list = &trch_list->elem[idx].allowedTFI_List;
              idy=0;
              while (allow_tfi_list->n > idy)
              {
                tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.
                trch_restriction_info[ntrch].tfi[ntfi] = (uint8)allow_tfi_list->elem[idy];
                ntfi ++;
                if (ntfi == TDSUE_MAX_TF)
                {
                  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Too many tf %d in a TrCH %d",ntfi,trch_list->elem[idx].restrictedTrChIdentity);
                  invalid_config = TRUE;
                  break;
                }
                idy++;
              }
              tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.
              trch_restriction_info[ntrch].ntf = (uint16)ntfi;
            }
            ntrch ++;
            if (invalid_config || ntrch == TDSUE_MAX_TRCH)
            {
              if (ntrch == TDSUE_MAX_TRCH)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Too many TrCH in TFCC %d",ntrch);
                invalid_config = TRUE;
              }
              break;
            }
            idx++;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Restricted TFI %d,ntrch %d",ntfi,ntrch);
          }
          tdsmac_tfc_subset_info.restriction_info.trch_restriction_info_list.ntrch = (uint8)ntrch;
          break;

        case T_tdsrrc_TFC_Subset_fullTFCS:
          /*For FEATURE_TDSCDMA_CS_VOICE_OVER_HSPA, this will happen so we may need to avoid sending TFCS CFG REQ to MAC,
          if there's no DCH or there's no TFCS restrictions already?*/
          tdsmac_tfc_subset_info.tfci_restriction_choice = TDSFULL_TFCI;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"FULL TFCI LIST chosen");
          break;

        default:break;
      }
    }
  }

  if (invalid_config)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid CFG hit");
    if (tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id) == DCCH_AM_RADIO_BEARER_ID)
    {
      if(dcch_msg_ptr != NULL)
      {
        tdsrrctfcc_send_tfcc_config_failure((uint8)dcch_msg_ptr->message.u.
                                         transportFormatCombinationControl.rrc_TransactionIdentifier);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send TFCC failure message");
      }
    }
    if(tdsrrctfcc_substate != TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
    {
      tdsrrctfcc_clear_procedure();
    }
  }
  else if(tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
  {
    /* Some config is on the way. Do not config TFCC at this time.  */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Some other config is going,wait for reconfig complete.");
    tdsrrcllc_register_for_reconfig_cnf();

    tdsrrctfcc_substate = TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF;
  }
  else if(tdsrrc_get_state()== TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: TFCC message received in FACH state");
    if(tdsmac_tfc_subset_info.control_duration == 0)
    {
      tdsrrcllc_update_tfcc_restriction_in_cc_oc(&tdsmac_tfc_subset_info);
    }
    tdsrrctfcc_clear_procedure();
  }
  else
  {
    if ((l2_ul_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending TDSCMAC_UL_TFC_SUBSET_CFG_REQ ",0,0,0);
    }
    /* Fill in the Command Header */
    l2_ul_cmd_ptr->cmd_hdr.cmd_id = TDSCMAC_UL_TFC_SUBSET_CFG_REQ;

    l2_ul_cmd_ptr->cmd_data.mac_tfc_subset_config_info = tdsmac_tfc_subset_info;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSCMAC_UL_TFC_SUBSET_CFG_REQ");

    /* Send command to L2 UL Task */
    tdsl2_ul_put_cmd(l2_ul_cmd_ptr);
    tdsrrctfcc_substate = TDSRRCTFCC_WAIT_FOR_MAC_CFG_CNF;
  }
}

/*===========================================================================

FUNCTION   tdsrrctfcc_process_reconfig_cnf

DESCRIPTION
  This function process the received reconfig CNF
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
static void tdsrrctfcc_process_reconfig_cnf
(
  void
)
{
  tdsl2_ul_cmd_type  *l2_ul_cmd_ptr;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: Recvd reconfig CNF ");

  if(tdsrrctfcc_substate != TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: Recvd reconfig CNF in wrong state[%d],ignore this cmd",tdsrrctfcc_substate);
    return;
  }

  if(tdsrrc_get_state()== TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: TFCC message received in FACH state");

    if(tdsmac_tfc_subset_info.control_duration == 0)
    {
      tdsrrcllc_update_tfcc_restriction_in_cc_oc(&tdsmac_tfc_subset_info);
    }

    tdsrrctfcc_clear_procedure();
  }
  else
  {
    if ((l2_ul_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending CMAC_UL_TFC_SUBSET_CFG_REQ ",0,0,0);
    }
    /* Fill in the Command Header */
    l2_ul_cmd_ptr->cmd_hdr.cmd_id = TDSCMAC_UL_TFC_SUBSET_CFG_REQ;
    l2_ul_cmd_ptr->cmd_data.mac_tfc_subset_config_info = tdsmac_tfc_subset_info;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending CMAC_UL_TFC_SUBSET_CFG_REQ");
  
    /* Send command to L2 UL Task */
    tdsl2_ul_put_cmd(l2_ul_cmd_ptr);
    tdsrrctfcc_substate = TDSRRCTFCC_WAIT_FOR_MAC_CFG_CNF;
  }
}

/*===========================================================================

FUNCTION   RRCTFC_PROCEDURE_EVENT_HANDLER

DESCRIPTION
  
  Transport Format Combination Control procedure handler.

        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
void tdsrrctfcc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  switch ( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      tdsrrctfcc_process_message( cmd_ptr );
      break;
    case TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF:
      tdsrrctfcc_process_mac_config_cnf((tdsrrc_mac_cmd_type *)cmd_ptr);
      break;
    case TDSRRC_STATE_CHANGE_IND:
      tdsrrctfcc_clear_procedure();
      break;
    case TDSRRC_RECONFIG_COMPLETE_IND:
      tdsrrctfcc_process_reconfig_cnf();
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Un Expected Command %x",cmd_ptr->cmd_hdr.cmd_id);
      break;
  }
}


/*===========================================================================

FUNCTION  RRCTFCC_CLEAR_PROCEDURE

DESCRIPTION

  This function clears the TFCC Procedure
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctfcc_clear_procedure
(
  void
)
{
  tdsrrctfcc_substate = TDSRRCTFCC_INITIAL;
  memset(&tdsmac_tfc_subset_info, 0, sizeof(tdsmac_tfc_subset_restriction_info_type));
}


/*===========================================================================

FUNCTION   RRCTFCC_PROCESS_MAC_CONFIG_CNF

DESCRIPTION
  This function process the received MAC CFG CNF
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/     
void tdsrrctfcc_process_mac_config_cnf
(
  tdsrrc_mac_cmd_type * cmd_ptr
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: Recvd CNF from MAC UL status: %d",cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.status);
  if(tdsrrctfcc_substate != TDSRRCTFCC_WAIT_FOR_MAC_CFG_CNF)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"MAC CNF is not expected in substate %d",tdsrrctfcc_substate);
    tdsrrctfcc_clear_procedure();
    return;
  }
  if(cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.status == TRUE)
  {
    /*MAC has been succesfully configured */
    if((cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.transaction_id == tdsmac_tfc_subset_info.transaction_id)&&
        (tdsmac_tfc_subset_info.control_duration == 0))
    {
      /* Update CC with tdsmac_tfc_subset_info */
      tdsrrcllc_update_tfcc_restriction_in_cc_oc(&tdsmac_tfc_subset_info);
    }
  }
  else
  {
    /* The RRC then sends config failure info in AM only when the rx'd
       * config is from RRC AM.
       */
    /* MAC has not been configured Correctly */
    if (cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.rb_id == DCCH_AM_RADIO_BEARER_ID)
    {
      tdsrrctfcc_send_tfcc_config_failure(cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.transaction_id);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TFCC failed for the RB_id %d",cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.rb_id);
    }
  }
  tdsrrctfcc_clear_procedure();
}

/*===========================================================================

FUNCTION  RRCTFCC_INIT_PROCEDURE

DESCRIPTION

  This function initializes the TFCC Procedure
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctfcc_init_procedure
(
  void
)
{
  tdsrrctfcc_substate = TDSRRCTFCC_INITIAL;
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_TFC,  /* Procedure name */
                                TDSRRC_STATE_WILDCARD, /* From State     */
                                TDSRRC_STATE_DISCONNECTED /* To State    */
                                );
  memset(&tdsmac_tfc_subset_info, 0, sizeof(tdsmac_tfc_subset_restriction_info_type));
}

/*===========================================================================

FUNCTION  tdsrrctfcc_is_reconfig_in_progress

DESCRIPTION

  This function will check if TFCC Procedure substate is reconfig in progress
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE: if TFCC procedure is wating for reconfig in progress
  FALSE: othewise.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrctfcc_is_reconfig_in_progress
(
  void
)
{
  boolean status = FALSE;

  if(tdsrrctfcc_substate == TDSRRCTFCC_WAIT_FOR_RECONFIG_CFG_CNF)
  {
    status = TRUE;
  }
  return status;
}



