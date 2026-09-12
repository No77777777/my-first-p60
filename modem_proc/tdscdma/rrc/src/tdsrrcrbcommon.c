/*===========================================================================
         R R C   R A D I O   B E A R E R   C O M M O N   M O D U L E 

DESCRIPTION

  This module performs common functions related to the Radio Bearer
  procedures.
  
EXTERNALIZED FUNCTIONS

  tdsrrcrb_translate_gsm_map_rab_id    Translates a RAB Id from ASN format to 
                                    internal format.


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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbcommon.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
03/14/12   as      Added code to support feature NB/WB handover support
10/17/11   zwj   Modify default value of FPACH and upPCH.
08/09/11   mkh    Compile fix on 9x15 due to enabling of FEATURE_TDSCDMA_VOC_AMR_WB
06/16/11   bj     Added code to handle receive R9 OTA Message.
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/11/10   yzh    (3GPP: 10.3.6.39a)Add tdsrrcget_add_ext_info() for r4\r5-add-ext.
11/05/10   ysh    C-RNTI and H-RNTI remove condition.
                  New function for get OTA msg's version. 3GPP-8.2.2.3(3.2.3).
11/10/10   bj     Deleted IE "RNC support for change of UE capability".
                  (3GPP: 8.6.2.3)
10/30/10   ysh    IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).

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
#include "tdsrrcasn1.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrcintif.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcsmc.h"
#include "tdsrrcccm.h"
#include "tdsrrclcm.h"
#include "tdsrrcrbe.h"
#include "tdsrrcrbr.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrctcreconfig.h"
#include "tdsrrcpcreconfig.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcllc.h"
#include "tdsrrcumi.h"
#include "tdsrrccu.h"
#include "tdsrrcllcoc.h"
#include "tdsrrccommon.h"

#include "rrcmnif.h"
#include "tdsrrclbt.h"
#include "tdsrrccu.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
#include "tdsrrcnv.h"
#endif


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
                                                                   
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

extern tdsmac_rab_status_e_type tdsmac_rab_status;
tdsrrc_rb_info_for_amr_type tdsamr_rb_info;



#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP*/

/* stores if the l2 ack is received by reconfig procedures or not for DCH*/
tdsrrc_proc_e_type tdsrrc_wait_for_l2ack_for_reconfig = TDSMAX_NUM_OF_PROCEDURES;

extern boolean tdsrrcrbe_send_initiate_cu_flag;
extern boolean tdsrrcrbrc_send_initiate_cu_flag;
extern boolean tdsrrcrbr_send_initiate_cu_flag;
extern boolean tdsrrcpcrc_send_initiate_cu_flag;
extern boolean tdsrrctcrc_send_initiate_cu_flag;
/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/*define sdu delay and recv time here to avoid interface change*/
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  uint32 tdsrrc_sdu_recv_delay;   /*the delay for receiving the whole SDU*/  
  uint32 tdsrrc_sdu_recv_time;    /*the time when received the first PDU */
  uint32 tdsrrc_cell_update_cnf_time = 0;    /*the time of for cell update confirm*/
#endif

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*===========================================================================

FUNCTION          RRCRB_QUERY_MAC_STATUS


DESCRIPTION

  This function returns the value of tdsmac_rab_status indicating to MAC what kind
  of RABS are currently in est_rab + reconfig msg.

DEPENDENCIES

  None

RETURN VALUE

  Return value is of rrcrb_mac_rab_status_e_type type

SIDE EFFECTS

  None

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsmac_rab_status_e_type tdsrrc_query_mac_rab_status
(
  void
)
{
  return tdsmac_rab_status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/* add function for r4\r5-add-ext-info(fpach frequency and uppch shift) */
/*============================================================================
FUNCTION: tdsrrcget_add_ext_info()

DESCRIPTION:
  This function save fpach frequency and uppch shift info and return
  SUCCESS or FAILURE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcget_add_ext_info
(
  uint8 fPachFreqInfoPresent,
  tdsrrc_FrequencyInfo fPachFreqInfo,
  uint8 upPCHposInfoPresent,
  tdsrrc_UpPCHposition_LCR upPCHposInfo
)
  {
    /* Get Fpach frequency info*/
    if (fPachFreqInfoPresent)
    {
      if (TDSRRC_CHECK_MSG_TYPE(fPachFreqInfo.modeSpecificInfo,
        T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.fpach_freq =
          fPachFreqInfo.modeSpecificInfo.u.tdd->uarfcn_Nt;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"fPachFrequencyInfo choice is not TDD");
        return FAILURE;
      }
    }
    
    /* Get upPCHpositionInfo */
    if (upPCHposInfoPresent)
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parms.upPch_shift = upPCHposInfo;
      /* UpPCH Position Info, if UpPCH shifting is not used, 
            and UpPCH only occupies UpPTS, set it to 0xFF,
            If UpPCH shifting is in use, range is from 0~127 */
#ifdef FEATURE_TDSCDMA_HSUPA
      tdsordered_config_ptr->l1_e_info.e_ul_info.e_rucch_info.upPch_shift = upPCHposInfo;
#endif
    }
    return SUCCESS;
  }



/*============================================================================
FUNCTION: tdsrrcllc_validate_rb_info_reconfig_list_r6()

DESCRIPTION:
  This function validates the IE tdsrrc_RB_InformationReconfigList_r6 and return
  SUCCESS or FAILURE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_validate_rb_info_reconfig_list_r6
(
  tdsrrc_RB_InformationReconfigList_r6 *rrc_rb_information_reconfig_list
)
{
  boolean msg_not_valid = FALSE;
#ifdef FEATURE_UMTS_PDCP
  struct tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;  
#endif /* FEATURE_UMTS_PDCP*/
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;  
  uint32 rb_idx=0;
  tdsrrc_RB_InformationReconfig_r6 * rb_info_reconfig_ptr;
    
  do
  { 
   rb_info_reconfig_ptr = &rrc_rb_information_reconfig_list->elem[rb_idx];
    /* Check whether rb id in rb reconfig list is in the established rabs list.  If not set msg_not_valid variable
       to true to resend cell update msg with failure cause  */
    if (TDSRRC_RB_NOT_PRESENT == tdsrrc_validate_rb_id_in_reconfig_list(
        rb_info_reconfig_ptr->rb_Identity))
    {
      /* RB Message was invalid. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in rb_InformationReconfigList",rb_info_reconfig_ptr->rb_Identity );
      msg_not_valid = TRUE;
      break;
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr, 
        tdsrrc_RB_InformationReconfig_r6,rb_StopContinue))
    {
      if ((uint16)(rb_info_reconfig_ptr->rb_Identity) <= 2)
      {
        /* not sure why this check is present */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"indicating stop/continue for RB %d <= 2",(uint16)(rb_info_reconfig_ptr->rb_Identity));
        msg_not_valid = TRUE;
        break;
      }
      
      lc_id = tdsrrclcm_get_ul_rlc_lc_id(
              rb_info_reconfig_ptr->rb_Identity);
      
      if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"No corresponding LC id for the RB %d",(uint16)(rb_info_reconfig_ptr->rb_Identity));
        break;
      }
      else if (tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode) == TDSRRCLCM_FAILURE)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid lc_mode for RB %d",rb_info_reconfig_ptr->rb_Identity);
        break;
      }
      else if (lc_mode == UE_MODE_TRANSPARENT)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Transparent mode RB %d lc_id %d can't be stopped/contd",(uint16)(rb_info_reconfig_ptr->rb_Identity),lc_id);
        break;
      }
    }

#ifdef FEATURE_UMTS_PDCP
    if (tdsrrc_pdcp_enabled)  
    { 
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
          tdsrrc_RB_InformationReconfig_r6,pdcp_SN_Info)) 
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid: PDCP SN info not supported CU CNF");
        break;
      }
      
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
          tdsrrc_RB_InformationReconfig_r6,pdcp_Info))
      {
        /* Validate PDCP info */
        if (tdsrrc_get_rab_type_for_given_rb_id(
            rb_info_reconfig_ptr->rb_Identity) != TDSRRCRB_PS_DATA_RAB)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"NW incld PDCP info for non-ps rb id %d: Invalid",rb_info_reconfig_ptr->rb_Identity);
          msg_not_valid = TRUE;
          break;
        }
        
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_info_reconfig_ptr,
            tdsrrc_RB_InformationReconfig_r6,pdcp_Info))
        {
          /* check whether lossless SRNS relocation flag is set or   
           * and header compression algo is present in the message 
           */
          if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info,
               tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
              (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
               rrc_LosslessSRNS_RelocSupport_supported)))
          {
            if (rb_info_reconfig_ptr->pdcp_Info.
                pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
              msg_not_valid = TRUE;
              break;
            }
         
          } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
        }

        if (rb_info_reconfig_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) 
        {
          if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info,
               tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
              (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
               rrc_LosslessSRNS_RelocSupport_notSupported)))
          {
            if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
              msg_not_valid = TRUE;
              break;
            }
          }
        }
        
        if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_info_reconfig_ptr->pdcp_Info.pdcp_Info,
            tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) 
        {
         uint32 local_idx=0;
          while (rb_info_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.n >local_idx) 
          {
            pdcpHeaderCompressionInfo = &rb_info_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.elem[local_idx];
            if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == 
                T_tdsrrc_AlgorithmSpecificInfo_r4_rfc2507_Info)
            {
              /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
               * failure with cause "Invalid Config"
               */
              if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max header comprss context space exceeded : %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                msg_not_valid = TRUE;
                break;
              }
            }
            else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == 
                     T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
            {
              /*    the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
               *  indicated in the IE "PDCP Capability"
               */
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,ul_RFC3095))
              {
                if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->
                    ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                  msg_not_valid = TRUE;
                  break;
                }
              }
              
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                dl_RFC3095))
              {
                if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                  msg_not_valid = TRUE;
                  break;
                }
              }

            }/* else if (pdcpHeaderCompressionInfoList->value.algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

            local_idx++;
          } /*  while (pdcpHeaderCompressionInfoList) */
        } /*  if (rb_list_ptr->value.pdcp_Info.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */         
      }
    }
#endif /* FEATURE_UMTS_PDCP */

    /* Advance the Linked List pointer */
    rb_idx++;
  } while(rrc_rb_information_reconfig_list->n > rb_idx) ;

  if (TRUE == msg_not_valid)
  {
    return(FAILURE);
  }
  else
  {
    return(SUCCESS);
  }
}


/*===========================================================================

FUNCTION   RRCRB_SEND_RB_PROC_COMPL_IND

DESCRIPTION

  This function is called by all RB procedures and it sends TDSRRC_RB_PROC_COMPL_IND to 
  CELL UPDATE procedure.
  
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.
 
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrb_send_rb_proc_compl_ind(void)
{
  tdsrrc_cmd_type *cmd_ptr;
  
  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_RB_PROC_COMPL_IND",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RB_PROC_COMPL_IND;

  /* No need to indicate the proc ID etc... This can be expanded in future to 
     include the proc id etc... for now, only CU is using this 
   */
  
  /* Put the command in the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_RB_PROC_COMPL_IND sent to CU");

  return;
} /* end of tdsrrcrb_send_rb_proc_compl_ind */

/*===========================================================================

FUNCTION   RRCRB_GET_NEW_CRNTI_VALID()

DESCRIPTION
 This function returns if reconfig msg has valid CRNTI


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrb_get_new_crnti_valid
(
  void
)
{
  return (tdsrrcrbe_get_new_crnti_valid() ||
          tdsrrcrbr_get_new_crnti_valid() ||
          tdsrrcrbrc_get_new_crnti_valid() ||
          tdsrrcpcrc_get_new_crnti_valid() ||
          tdsrrctcrc_get_new_crnti_valid() );
}


/*===========================================================================

FUNCTION   RRCRB_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrb_is_srns_in_progress
(
  void
)
{
  return (tdsrrcrbe_is_srns_in_progress() || tdsrrcrbrc_is_srns_in_progress() ||
          tdsrrcpcrc_is_srns_in_progress() || tdsrrctcrc_is_srns_in_progress() ||
          tdsrrcrbr_is_srns_in_progress() || tdsrrccu_is_srns_in_progress()
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          || tdsrrcumi_is_srns_in_progress()
#endif
         );
}
/*===========================================================================

FUNCTION   tdsrrcrb_is_cu_required()

DESCRIPTION
 This function returns TRUE if cell update is required after
 the reconfig msg.  Else returns FALSE
      
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrb_is_cu_required
(
  void
)
{

  return (   tdsrrcrbe_send_initiate_cu_flag 
          || tdsrrcrbrc_send_initiate_cu_flag 
          || tdsrrcrbr_send_initiate_cu_flag
          || tdsrrcpcrc_send_initiate_cu_flag
          || tdsrrctcrc_send_initiate_cu_flag );
}


/*===========================================================================
FUNCTION   RRCRB_UPDATE_MAC_RAB_STATUS

DESCRIPTION

  This function updates variable tdsmac_rab_status. Parameters to this func 
  are proc id, which identifies the calling procedure, cn_domain, used by RBE
  to identify which domain RABS are being setup for, RB_InformationReleaseList,
  used by RBE and CU procedures, and boolean go_back_to_old_status, which
  indicates to update tdsmac_rab_status from scratch by looking into est_rabs only.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrb_update_mac_rab_status
(
  tdsrrc_proc_e_type proc_id,
  rrc_cn_domain_identity_e_type cn_domain,
  tdsrrc_RB_InformationReleaseList  *rb_info_rel_list_ptr,
  boolean go_back_to_old_status
)
{
  tdsrrc_established_rabs_type est_rabs_copy;
  
  uint32 count;  /* Local Counter */
  uint32 count1; /* Local Counter */
  uint32 count2; /* Local Counter */

  /* In case of a channel config req fails, need to roll back mac rab status *
   * to old value.  update the variable by going thru established rabs list. */
  if (go_back_to_old_status) 
  {
    tdsmac_rab_status = TDSNO_RABS;
    for (count = 0; count < MAX_RAB_TO_SETUP; count ++) 
    {
      if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
          (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0))
      {
        if (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID) 
        {
          if (tdsmac_rab_status == TDSNO_RABS) 
          {
            tdsmac_rab_status = TDSCS_RABS;
          }
          else if (tdsmac_rab_status == TDSPS_RABS)
          {
            tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
            /* once we have CS+PS, no need to go thru entire est_rabs list */
            break;
          }
        }
        else if (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID) 
        {
          if (tdsmac_rab_status == TDSNO_RABS) 
          {
            tdsmac_rab_status = TDSPS_RABS;
          }
          else if (tdsmac_rab_status == TDSCS_RABS) 
          {
            tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
            /* once we have CS+PS, no need to go thru entire est_rabs list */
            break;
          }
        }
      }    
    } /* for (count = 0; count < MAX_RAB_TO_SETUP; count ++) */
  } /* (go_back_to_old_status)  */ 

  /* If proc is IHO, blindly set tdsmac_rab_status to CS_RABS since IHO can only
   * happen for CS domain 
   */
  else if (proc_id == TDSRRC_PROCEDURE_IHO)
  {
#ifdef FEATURE_LTE_TO_TDSCDMA
    if (cn_domain == RRC_CS_DOMAIN_CN_ID)
    {
      tdsmac_rab_status = TDSCS_RABS;
    }
    else if (cn_domain == RRC_PS_DOMAIN_CN_ID)
    {
      if(tdsmac_rab_status == TDSNO_RABS)
      {
      tdsmac_rab_status = TDSPS_RABS;
    }
      else if(tdsmac_rab_status == TDSCS_RABS)
      {
        tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
      }
    }
#else
    tdsmac_rab_status = TDSCS_RABS;
#endif /* FEATURE_LTE_TO_TDSCDMA */
  }
  else if (proc_id == TDSRRC_PROCEDURE_RBE)
  {
    if (tdsmac_rab_status == TDSNO_RABS)
    {
      if (cn_domain == RRC_CS_DOMAIN_CN_ID)
      {
        tdsmac_rab_status = TDSCS_RABS;
      }
      else if (cn_domain == RRC_PS_DOMAIN_CN_ID)
      {
        tdsmac_rab_status = TDSPS_RABS;
      }
    }
    else if (tdsmac_rab_status == TDSCS_RABS)
    {
      if (cn_domain == RRC_PS_DOMAIN_CN_ID)
      {
        tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
      }
    }
    else if (tdsmac_rab_status == TDSPS_RABS)
    {
      if (cn_domain == RRC_CS_DOMAIN_CN_ID)
      {
        tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
      }
    }
  } // if (proc_id == TDSRRC_PROCEDURE_RBE
  else if ((rb_info_rel_list_ptr != NULL) && (rb_info_rel_list_ptr->n != 0) && 
           (proc_id == TDSRRC_PROCEDURE_RBR || proc_id == TDSRRC_PROCEDURE_CU))
  {
    uint32 rb_idx=0;
    tdsmac_rab_status = TDSNO_RABS;
    memscpy(&est_rabs_copy, sizeof(tdsrrc_established_rabs_type), &tdsrrc_est_rabs, sizeof(tdsrrc_established_rabs_type));

      do
      {
        for( count1 =0; count1 < MAX_RAB_TO_SETUP; count1++ )
        {
          /* If the RAB is valid and it has atleast one RB
          mapped to it, we should try to find a match for this RB */
          if( (est_rabs_copy.rabs[count1].rab_id != TDSRRC_INVALID_RAB_ID) &&
              (est_rabs_copy.rabs[count1].num_rbs_for_rab > 0)
              )
          {
            /* This RAB is valid. Look at each RB within the RAB */
            for( count2=0; count2<MAX_RB_PER_RAB; count2++ )
            {
              if( est_rabs_copy.rabs[count1].rb_for_rab[count2].rb_id == rb_info_rel_list_ptr->elem[rb_idx])
              {
                /* Match is found. Remove this RB and decrement
                the number of RBs for this RAB */
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAC_RAB_STATUS: RB removed rbid:%d, rabid:%d, domain:%d",
                     rb_info_rel_list_ptr->elem[rb_idx], //rbr.rbs_tobe_released.rb_id[count],
                     est_rabs_copy.rabs[count1].rab_id,
                     est_rabs_copy.rabs[count1].cn_domain
                   );
                est_rabs_copy.rabs[count1].rb_for_rab[count2].rb_id = TDSRRC_INVALID_RB_ID;
                est_rabs_copy.rabs[count1].num_rbs_for_rab--;
                if( est_rabs_copy.rabs[count1].num_rbs_for_rab == 0 )
                {
                  est_rabs_copy.rabs[count1].rab_id = TDSRRC_INVALID_RAB_ID;
                }
              }
            }
          }
        } 
        rb_idx++;
      } while (rb_info_rel_list_ptr->n > rb_idx);
    /* Now travers thru est_rabs_copy to assign the right value to tdsmac_rab_status */

    for (count=0; count < MAX_RAB_TO_SETUP; count++)
    {
      if(est_rabs_copy.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
      {
        if (tdsmac_rab_status == TDSNO_RABS)
        {
          if (est_rabs_copy.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID )
          {
            tdsmac_rab_status = TDSCS_RABS;
          }
          else if (est_rabs_copy.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID)
          {
            tdsmac_rab_status = TDSPS_RABS;
          }
        }
        else if (tdsmac_rab_status == TDSCS_RABS)
        {
          if (est_rabs_copy.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID)
          {
            tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
            break;
          }
        }
        else if (tdsmac_rab_status == TDSPS_RABS)
        {
          if (est_rabs_copy.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            tdsmac_rab_status = TDSCS_PLUS_PS_RABS;
            break;
          }
        }
      }
    }
  } /* else if RBR */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MAC_RAB_STATUS %d",tdsmac_rab_status);
}
/*===========================================================================

FUNCTION          RRCRB_TRANSLATE_GSM_MAP_RAB_ID


DESCRIPTION       

  This function translates a GSM MAP RAB Id type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  None
  
RETURN VALUE
                  
  The GSM MAP RAB Id converted to the internal type rrc_rab_id_type

SIDE EFFECTS
                  
  None

===========================================================================*/
rrc_rab_id_type tdsrrcrb_translate_gsm_map_rab_id
(
  ASN1BitStr32  *ext_rab_id         /* Pointer to external RAB ID */
)
{
  uint32 count;                    /* Local counter       */
  uint32 num_bytes;                /* number of full bytes in string */
  uint32 num_shift;                /* number of bits to shift the result.
                                   This is based on the number of signigicant
                                   bits in the string */
  rrc_rab_id_type rab_id=0;        /* To save the RAB id  */

  /* ossBitString represents an item by indicating the
  number of significant bits in the "length" and putting
  the value starting with the MSB 8 bits, in a unsigned
  char array. The length may not be octet alligned. For
  example if the length is 20 bits, value[1] will contain
  8 MSB bits, value[2] will contain the 2nd 8 MSB bits
  and value[3] will have the 2 lsb bits in the ms and 2
  ms bit positions. */

  /* First find the number of bits that don't
  contain any data. Since the length may not be
  octet alligned, there may be some bits that
  don't have any data and need to be shifted out. */

  num_shift = (8 - ((ext_rab_id->numbits)%8));
  if( num_shift == 8 )
  {
    /* Bits are octet alligned, no shifting needed */ 
    num_shift = 0;
    num_bytes = ((ext_rab_id->numbits)/8);
  }
  else
  {
    num_bytes = ((ext_rab_id->numbits)/8) + 1;
  }

  if( num_bytes > 4)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rab_id longer than 32 bits!");
    return( rab_id );
  }

  /* Now loop through and copy all the given bits.
  NOTE: Little Endian is assumed here. If Big Endian
  is used, we may need to change this. */
  for(count=0; count < num_bytes; count++)
  {
    /* Left shift each byte from the char pointer
    to the uint32 item. */
    rab_id = rab_id << 8;
    rab_id = (rab_id | (ext_rab_id->data[count]));
  }

  /* Finally right shift to remove the bits with no data */
  rab_id = (rab_id >> num_shift);

  return( rab_id );
}

/*===========================================================================

FUNCTION tdsrrcrb_validate_directed_cell_info

DESCRIPTION
  This function validates the directed cell info and returns a boolean
  that indicates whether a cellupdate is required or not. 

DEPENDENCIES
  None.

RETURN VALUE
  Boolean TRUE  : Cell update is required.
          FALSE : Cell update is not required required.

===========================================================================*/
boolean  tdsrrcrb_validate_directed_cell_info
(
  tdsrrcrb_directed_cell_info_type  directed_cell_info
)
{
  tdsrrc_freq_type rrc_freq;
  boolean       directed_cell_match = FALSE;

  if (!(directed_cell_info.freq_info_present || directed_cell_info.cpid_present))
  {
     directed_cell_match = TRUE;
  }

  else if (directed_cell_info.freq_info_present)
  {
    rrc_freq = tdsrrcccm_get_curr_camped_freq();
    if(rrc_freq == directed_cell_info.uarfcn_dl) 
                  
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Camped and msg Freq %d FACH->FACH",rrc_freq);
      /*check whether primary scrambling code is also present */
      if (directed_cell_info.cpid_present)
      {
        if (tdsrrcccm_get_curr_camped_cpid() == directed_cell_info.cell_param_id)
        {
          /* both scr and freq match no need to do cellupdate */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Both Freq%d,SCR%d match No CU reqd",rrc_freq,tdsrrcccm_get_curr_camped_cpid());
          directed_cell_match = TRUE;
        }
        else
        {   
          /* both CPID will not match need to do cell update */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"SCR%d From Msg:%d,CU reqd",tdsrrcccm_get_curr_camped_cpid(),directed_cell_info.cell_param_id);
          directed_cell_match = FALSE;
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq%d,No SCR,CU reqd",rrc_freq);
        /*no CPID in the message. Since only frequency info is present
        need to do Cell update */
        directed_cell_match = FALSE;
      }
    }
    else
    {
      /* control will not come here as this check is already made in set_ordered_config
      and it returns failure if the frequencies are different. */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Camped freq %d,in msg %d FACH->FACH",rrc_freq,directed_cell_info.uarfcn_dl);
      directed_cell_match = FALSE;
    }
  } /* if frequency info is present */
  else
  {
    /* frequency info is not present. Treat the current frequency as the destination
    frequency. check for the scrambling code */
    /*check whether primary scrambling code is also present */
    if (directed_cell_info.cpid_present)
    {
      if (tdsrrcccm_get_curr_camped_cpid() == directed_cell_info.cell_param_id)
      {
        /* both scr  match no need to do cellupdate */
        directed_cell_match = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No Freq,SCR%d match No CU reqd",tdsrrcccm_get_curr_camped_cpid());
      }
      else
      {
        /* both scr and freq match no need to do cellupdate */
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"No Freq,SCR%d From Msg:%d ",tdsrrcccm_get_curr_camped_cpid(),directed_cell_info.cell_param_id);
        directed_cell_match = FALSE;
      }
    } /* if CPID is present */

  } /* no frequency info*/
  return directed_cell_match;
}  /* tdsrrcrb_validate_directed_cell_info */

/*===========================================================================

FUNCTION tdsrrcrb_send_rnti_update_req

DESCRIPTION
  This function sends the RNTI Update request to LLC to update
  with RNTIs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void  tdsrrcrb_send_rnti_update_req
(
  tdsrrcrb_rnti_update_info_type  rnti_update_info
)
{
  tdsrrc_cmd_type  *cmd_ptr;

  /* Allocates the buffer to RRC Internal command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  { 
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RNTI_UPDATE_REQ;

    /* Fill parameters for TDSRRC_RNTI_UPDATE_REQ command */
    cmd_ptr->cmd.rnti_update_req.procedure = rnti_update_info.procedure;

    /*check whether c-rnti needs tobe updated */
    if (rnti_update_info.crnti_action == TDSRRC_RNTI_UPDATE)
    {
      /* Indicate that C-RNTI needs to be updated */
      cmd_ptr->cmd.rnti_update_req.crnti_action = TDSRRC_RNTI_UPDATE;
      cmd_ptr->cmd.rnti_update_req.crnti = rnti_update_info.crnti;
    }
    else
    {
      /* It could be either no change or
      delete request. Copy the action */
      cmd_ptr->cmd.rnti_update_req.crnti_action = rnti_update_info.crnti_action;
    }

    /*check whether U-rnti needs tobe updated */
    if (rnti_update_info.urnti_action == TDSRRC_RNTI_UPDATE)
    {
      /* Indicate that U-RNTI needs to be updated */
      cmd_ptr->cmd.rnti_update_req.urnti_action = TDSRRC_RNTI_UPDATE;
      cmd_ptr->cmd.rnti_update_req.urnti = rnti_update_info.urnti;
    }

    else
    {
      /* It could be either no change or
      delete request. Copy the action */
      cmd_ptr->cmd.rnti_update_req.urnti_action = rnti_update_info.urnti_action;
    }
    
    cmd_ptr->cmd.rnti_update_req.rlc_re_establish_srb = rnti_update_info.rlc_re_establish_srb;
    cmd_ptr->cmd.rnti_update_req.rlc_re_establish_rab = rnti_update_info.rlc_re_establish_rab;

    /* Confirmation from LLC is not needed */
    cmd_ptr->cmd.rnti_update_req.cnf_required = rnti_update_info.cnf_reqd;
    
    tdsrrc_put_int_cmd(cmd_ptr);       /* sends the command to RRC  LLC. */

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RNTI_UPDATE_REQ CRNTI:%d, URNTI:%d rrc_proc: %d",
              rnti_update_info.crnti_action, 
             rnti_update_info.urnti_action,
             rnti_update_info.procedure);

  }
  else  /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for cmd: %d, rrc_proc: %d",TDSRRC_RNTI_UPDATE_REQ,rnti_update_info.procedure,0);
  }  
}  /* rrcrbrc_send_rnti_update_req */

/*===========================================================================

FUNCTION rrcrbrc_send_rlc_re_est_req_to_smc

DESCRIPTION
  This function sends the rlc_reestablishment request to 
  SMC procedure when reconfig message has SRNS relocation info
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrb_send_rlc_re_est_req_to_smc
(
  tdsrrc_proc_e_type rrc_proc,
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type,
  boolean cnf_reqd
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  /* Get the command buffer */
  if((int_cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
    /* Fill in the command id  */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RE_EST_RLC_FOR_SRNS_REQ;
    int_cmd_ptr->cmd.rrc_re_est_srns_req.rrc_proc = rrc_proc;
    int_cmd_ptr->cmd.rrc_re_est_srns_req.rb_type  = rb_type;
    int_cmd_ptr->cmd.rrc_re_est_srns_req.cnf_reqd = cnf_reqd;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RLC Reestablish req for %d from proc %d",rb_type,rrc_proc); 

    tdsrrc_put_int_cmd(int_cmd_ptr);             

  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd from proc: %d",TDSRRC_RE_EST_RLC_FOR_SRNS_REQ,rrc_proc,0);
  }
}/*rrcrbrc_send_rlc_re_est_req_to_smc*/
 /*===========================================================================

FUNCTION          RRCRB_SEND_L1_DRX_REQ

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1.

DEPENDENCIES
                  

RETURN VALUE
                  Void
SIDE EFFECTS      None.
===========================================================================*/
void tdsrrcrb_send_l1_drx_req(tdsrrc_proc_e_type rrc_proc)
{

  tdsrrc_cmd_type *cmd_ptr;

  /* Send DRX command to L1, no confirm to wait for */

  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
  
    /* Initialize the LLC command header first */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = rrc_proc;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
  
    /* Now fill the L1 cmd header */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_DRX_REQ;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    /* Populate the cmd data */
    rrc_cmd_l1_req->drx.drx_cycle_len = tdsrrcllc_get_drx_info();
    
    /* Send the Internal RRC command - LLC should directly handle it */
    tdsrrc_put_int_cmd(cmd_ptr);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"DRX_REQ to L1 cycle:%d proc:%d",rrc_cmd_l1_req->drx.drx_cycle_len,rrc_proc);
  }
  else
  {
    ERR_FATAL("Couldn't get internal command buffer",0,0,0);
  }

} /*rrrb_send_l1_drx_req */

 /*===========================================================================

FUNCTION          tdsrrcrb_send_upd_ota_parms_ind

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_UPD_OTA_PARMS_IND
                  to L1.

DEPENDENCIES
                  

RETURN VALUE
                  Void
SIDE EFFECTS      None.
===========================================================================*/
void tdsrrcrb_send_upd_ota_parms_ind(tdsrrc_proc_e_type rrc_proc, uint8 bit_mask_ind)
{
#ifdef FEATURE_TDSCDMA_REL7_SPECRS   
  tdsrrc_cmd_type *cmd_ptr;

  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
  

    /* Initialize the LLC command header first */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = rrc_proc;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
  
    /* Now fill the L1 cmd header */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_UPD_OTA_PARMS_IND;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    /* Populate the cmd data */
    rrc_cmd_l1_req->upd_ota_parms.bit_mask = bit_mask_ind;
    rrc_cmd_l1_req->upd_ota_parms.high_mobility_ind = TRUE;
    
    /* Send the Internal RRC command - LLC should directly handle it */
    tdsrrc_put_int_cmd(cmd_ptr);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSL1_CPHY_UPD_OTA_PARMS_IND to L1. Bit mask:%d proc:%d",rrc_cmd_l1_req->upd_ota_parms.bit_mask,rrc_proc);
  }
  else
  {
    ERR_FATAL("Couldn't get internal command buffer",0,0,0);
  }  
#else /*FEATURE_TDSCDMA_REL7_SPECRS*/
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"High Mobility Ind to L1 not supported.");
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/
}

/*===========================================================================

FUNCTION tdsrrcrb_send_cell_update_initiate_req

DESCRIPTION
  This function sends the cell update initiate request 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrb_send_cell_update_initiate_req
(
tdsrrc_proc_e_type              procedure,
tdsrrc_cell_update_cause_e_type cause,
boolean                      cnf_reqd
)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
    int_cmd_ptr->cmd_hdr.cmd_id                                    = TDSRRC_CELL_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_cell_update.cause                    = cause;
    int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = cnf_reqd;
    int_cmd_ptr->cmd.initiate_cell_update.procedure                = procedure;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE Proc:%d, Cause:%d, cnf_reqd:%d", 
              procedure, cause, cnf_reqd);
    tdsrrc_put_int_cmd(int_cmd_ptr);             
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
  }
}/*tdsrrcrb_send_cell_update_initiate_req*/


/*===========================================================================

FUNCTION rrcrbrc_send_ura_update_initiate_req

DESCRIPTION
  This function sends the ura update initiate request 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrb_send_ura_update_initiate_req
(
tdsrrc_proc_e_type              procedure,
tdsrrc_ura_update_cause_e_type  cause,
boolean                      cnf_reqd
)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
    int_cmd_ptr->cmd_hdr.cmd_id                                   = TDSRRC_URA_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_ura_update.cause                    = cause;
    int_cmd_ptr->cmd.initiate_ura_update.uu_complete_cnf_required = cnf_reqd;
    int_cmd_ptr->cmd.initiate_ura_update.procedure                = procedure;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"URA_UPDATE_INITIATE Proc:%d, Cause:%d, cnf_reqd:%d", 
              procedure, cause, cnf_reqd);
    tdsrrc_put_int_cmd(int_cmd_ptr);             
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_URA_UPDATE_INITIATE_REQ,0,0);
  }
}/*tdsrrcrb_send_ura_update_initiate_req*/

/*===========================================================================

FUNCTION tdsrrcrb_send_initiate_cell_selection_req

DESCRIPTION
  This function send the cell selection initiate request.
DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrb_send_initiate_cell_selection_req
(
   tdsrrc_proc_e_type              procedure,
   tdsrrc_cell_sel_cause_e_type    cause,
   tdsrrc_state_e_type             dest_state
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for RRC_INITIATE_CELL_SELECTION */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_CELL_SELECTION_IND;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.cause = cause;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.proc  = procedure;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = dest_state;

    if(tdsrrcllc_return_freq_for_fach_pch(&int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq)
       == SUCCESS)
    {
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = TRUE;
    }
    else
    {
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = FALSE;
    }

    if(tdsrrcllc_return_psc_for_fach_pch(&int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid)
       == SUCCESS)
    {
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present = TRUE;
    }
    else
    {
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present = FALSE;
    }

    int_cmd_ptr->cmd.initiate_cell_sel_ind.tx_to_discon_reason = TDSRRC_TX_TO_DISCON_OTHER;


    TDSRRC_MSG3(MSG_LEGACY_HIGH,"INITIATE_CELL_SELECTION Proc:%d, Cause:%d, Dest_st:%d", 
              procedure, cause, dest_state);
    tdsrrc_put_int_cmd(int_cmd_ptr); 
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_INITIATE_CELL_SELECTION_IND,0,0);
  }
}/*tdsrrcrb_send_initiate_cell_selection_req*/

/*===========================================================================

FUNCTION   RRCRB_REGISTER_WITH_RLC_FOR_UL_DATA_IND

DESCRIPTION

  This function sends a command to RLC to register for uplink data indication
  when UE moves to CELL_PCH/URA_PCH state. It always indicates that registration
  is required for signalling RBs. To include user plane RBs it looks 
  at established_rabs variable 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcrb_register_with_rlc_for_ul_data_ind
(
  tdsrrc_proc_e_type              procedure
)
{
  tdsrrc_cmd_type    *llc_cmd_ptr;
  uint32           count      = 0;
  uint32           rab_count  = 0;
  uint32           rb_count   = 0;
  rlc_lc_id_type ul_rlc_lc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  /*send CRLC DATA IND REQ to RLC via rrcllc */
  if((llc_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl2_ul_req_cmd * rrc_cmd_l2_req = NULL;
    tdsrrc_get_pointer_l2_req_value(&llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &rrc_cmd_l2_req);

    /* Initialize the LLC command header first */
    llc_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    llc_cmd_ptr->cmd.rrc_llc_req.procedure = procedure;
    llc_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_RLC_UL;

    /* Now fill the L2 cmd header */
     llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = TDSCRLC_UL_REG_FOR_DATA_IND_REQ;
     rrc_cmd_l2_req->ul_reg_data_ind.block_status_timer=TRUE;

     /*Include the lc_ids for signalling RBs with RLC AM mode. Eventhough RRC can monitor
     the UL data in the uplink on signalling RBs this is required to suspend the periodic status
     timer at RLC */
     rrc_cmd_l2_req->ul_reg_data_ind.lc_id[count++] =
       tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
     rrc_cmd_l2_req->ul_reg_data_ind.lc_id[count++] =
       tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID); 
     ul_rlc_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH, 
                                 DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                 UE_MODE_ACKNOWLEDGED_DATA);
     
     if(ul_rlc_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
     {
       rrc_cmd_l2_req->ul_reg_data_ind.lc_id[count++] =
         tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
     }

     /* get the lc_ids for user plane RBs */
     for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
     {
       if ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID) 
           &&
           (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0)
          )
       {
         TDSRRC_MSG2(MSG_LEGACY_ERROR,"No reg for CS domain,rabid:%d,rbs:%d",tdsrrc_est_rabs.rabs[rab_count].rab_id,tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab);

       }
       else if ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID)
               &&
               (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0)
               )
       {
         /*it is a valid established rab, now compare each rb-id in this established rab with 
         the rb-id received in the message */
         for (rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
         {
           rrc_cmd_l2_req->ul_reg_data_ind.lc_id[count++] =
              tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id);
         }
       } /*end of if*/
     }/* rab_count loop */
     rrc_cmd_l2_req->ul_reg_data_ind.num_lc = (uint16)count;
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"registering with RLC for %d RBs Proc:%d",count,procedure);
     tdsrrc_put_int_cmd(llc_cmd_ptr); 
  }
  else
  {
    ERR_FATAL("RRC Unable to get cmd buf for RLC cmd",0,0,0);
  }
}    /* tdsrrcrb_register_with_rlc_for_ul_data_ind */

/*===========================================================================

FUNCTION tdsrrcrb_get_proc_sub_state

DESCRIPTION
  This function returns the respective procedure substate.
  
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcrb_substate_e_type proc_sub_state.
===========================================================================*/
tdsrrcrb_substate_e_type tdsrrcrb_get_proc_sub_state
(
  tdsrrc_proc_e_type proc_id
)
{
     tdsrrcrb_substate_e_type proc_sub_st = TDSRRCRB_MAX_SUBSTATES;
     switch (proc_id)
     {
       case  TDSRRC_PROCEDURE_RBE:
         proc_sub_st = tdsrrcrbe_get_current_sub_state();
         break;
       case  TDSRRC_PROCEDURE_RBR:
         proc_sub_st = tdsrrcrbr_get_current_sub_state();
         break;
       case  TDSRRC_PROCEDURE_RBRC:
         proc_sub_st = tdsrrcrbrc_get_current_sub_state();
         break;
       case  TDSRRC_PROCEDURE_TCR:
         proc_sub_st = tdsrrctcrc_get_current_sub_state();
         break;
       case  TDSRRC_PROCEDURE_PCR:
         proc_sub_st = tdsrrcpcrc_get_current_sub_state();
         break;
       default:
         break;
     }
     return proc_sub_st;
}/*rrcrbrc_send_rlc_re_est_req_to_smc*/

/*===========================================================================
FUNCTION tdsrrcrb_update_amr_cctrch_id_for_reconfig
DESCRIPTION
DEPENDENCIES
  None.
RETURN VALUE
  None.
===========================================================================*/
void tdsrrcrb_update_amr_cctrch_id_for_reconfig
( 
  uint32 codec_type 
)
{
  tdsrrcllc_amr_mapping_info_type amr_info; /* To store AMR mode
                                            info incase a voice RAB has been setup */

  uint32 prev_dl_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  uint32 new_dl_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  boolean dl_mode_changed = FALSE;
  uint32 i, j, voice_rab_idx = 0;

  /* Channel config was succesful. Check if any
     RABs were setup, or only Signalling RBs were setup */
  if(!tdsrrcllc_check_if_new_tfcs_present())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Skipping Configuring Vocoder as TFC's are not changed");
    return;
  }
  if(TRUE == tdsrrc_is_voice_rab_present())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"AMR-Update for voice RAB");
    /* We need to indicate to the higher layers that
      a voice RAB has been setup. First call LLC to
      get the necessary information to send to voice
      services. The structure amr_info will be filled
      by this function. */
     
    if( codec_type != tdsrrcdata_codec_info.rrc_active_codec)
    {
      tdsrrc_set_current_codec_mode(codec_type);
   
      memset(&tdsamr_rb_info, 0, sizeof(tdsrrc_rb_info_for_amr_type));
        /* Initialize RAB type to unknown as Circuit Switched Voice RAB maps to value 0 */
      tdsamr_rb_info.rab_type = TDSRRCRB_UNKNOWN_RAB;
    
      for(i=0; i<MAX_RAB_TO_SETUP; i++)
      {
        if(tdsrrc_est_rabs.rabs[i].rab_type == TDSRRCRB_CS_VOICE_RAB)
        {
          tdsamr_rb_info.rab_type = TDSRRCRB_CS_VOICE_RAB;
          tdsamr_rb_info.num_rbs_for_rab= tdsrrc_est_rabs.rabs[i].num_rbs_for_rab;
   
          for(j=0; j< tdsamr_rb_info.num_rbs_for_rab; j++)
          {
            tdsamr_rb_info.rb_id[j] = tdsrrc_est_rabs.rabs[i].rb_for_rab[j].rb_id;
          }
          break; /* Since a voice RAB has been found; no need to iterate */
        }
      }
        
      tdsrrc_change_codec_configuration(i);
        
      /* This is just to silence lint */
      if(i == MAX_RAB_TO_SETUP)
      {
        return;
      }
      return;  
    }

    tdsrrc_set_current_codec_mode(codec_type);
    if( codec_type == TDSCDMA_IVOCODER_ID_AMRWB)
    {
      amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMRWB;
      if(FAILURE == tdsrrcllc_get_amr_wb_mapping(&(amr_info)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No AMR Mapping");
        return;
      } 
    }
    else
    {
      amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMR;
      if(FAILURE == tdsrrcllc_get_amr_mapping(&(amr_info)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No AMR Mapping");
        return;
      }
    }
    /* The following piece of code ensures that
       "tdsamr_rb_info" is correctly populated with right
       RB Ids corresponding to voice RAB. This is 
       further useful to calculate DL rate 
       per RB Id. Ofcourse, the effective AMR rate
       is the sum of all of them put together */
    memset(&tdsamr_rb_info, 0, sizeof(tdsrrc_rb_info_for_amr_type));
    /* Initialize RAB type to unknown as Circuit Switched Voice RAB maps to value 0 */
    tdsamr_rb_info.rab_type = TDSRRCRB_UNKNOWN_RAB;

    for(i=0; i<MAX_RAB_TO_SETUP; i++)
    {
      if(tdsrrc_est_rabs.rabs[i].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        tdsamr_rb_info.rab_type = TDSRRCRB_CS_VOICE_RAB;
        tdsamr_rb_info.num_rbs_for_rab= tdsrrc_est_rabs.rabs[i].num_rbs_for_rab;
        voice_rab_idx = i;
        for(j=0; j< tdsamr_rb_info.num_rbs_for_rab; j++)
        {
          tdsamr_rb_info.rb_id[j] = tdsrrc_est_rabs.rabs[i].rb_for_rab[j].rb_id;
        }
        break; /* Since a voice RAB has been found; no need to iterate */
      }
    }
    
    /* This is just to silence lint */
    if(i == MAX_RAB_TO_SETUP)
    {
      return;
    }

    amr_info.mode_type = codec_type;

    /************************************/
    /* Uplink AMR rate processing logic */
    /************************************/
    if((TDSCDMA_IVOCODER_ID_AMRWB == codec_type) || (TDSCDMA_IVOCODER_ID_AMR == codec_type))
    {
      tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE);
      tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE);
    }

    if(TDSCDMA_IVOCODER_ID_AMR == codec_type)
    {
      tdsrrc_configure_voice_adapter(&amr_info, TRUE, FALSE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
    }
    /**************************************/
    /* Downlink AMR rate processing logic */
    /**************************************/
    if(codec_type == TDSCDMA_IVOCODER_ID_AMR)
    {
      dl_mode_changed = tdsrrc_get_dl_amr_mode_given_rbs(&prev_dl_amr_mode, &new_dl_amr_mode, &tdsamr_rb_info);    
      TDSRRC_MSG2(MSG_LEGACY_HIGH, "TV_TRRC: DL -> prev mode=%d,new mode=%d",prev_dl_amr_mode,new_dl_amr_mode);
      /* Register/Deregister DL TDS Voice Adapter only when there is a rate change that is noticed
           between current config & ordered config information */
      if(dl_mode_changed == TRUE)
      {
        amr_info.dl_amr_mode = new_dl_amr_mode;
      }
      else
      {
        amr_info.dl_amr_mode = prev_dl_amr_mode;
      }

      if (new_dl_amr_mode >= TDSCDMA_IVOCAMR_CODEC_MODE_1020)
      { 
        if(tdsrrc_est_rabs.rabs[i].num_rbs_for_rab == 3)
        { 
          if(tdsrrc_get_amr_mode_bitrate(prev_dl_amr_mode) == TDSRRC_AMR_MODE_LOW_BITRATE)
          {
            tdsrrc_configure_voice_adapter(&amr_info, FALSE, TRUE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid TB size or num of RBs");
          return;
        } 
      }
      else
      {
        if(tdsrrc_get_amr_mode_bitrate(prev_dl_amr_mode) == TDSRRC_AMR_MODE_HIGH_BITRATE)
        {
          tdsrrc_configure_voice_adapter(&amr_info, FALSE, TRUE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
        }
      }
    }
  }
}

/*====================================================================
FUNCTION       tdsrrcrb_get_mac_d_hfn()

DESCRIPTION    This function extract mac_d_hfn field from mac_d_hfn_intial_value_ptr
               if "mac_d_hfn_intial_value_present" indicates TRUE.

REUSABILITY: R5/R6
DEPENDENCIES   None

RETURN VALUE   value of mac_d_hfn if present else, invalid value

====================================================================*/

uecomdef_status_e_type  tdsrrcrb_get_mac_d_hfn
(
  uint32  mac_d_hfn_intial_value_present,
  ASN1BitStr32 *mac_d_hfn_intial_value_ptr,
  uint32 *mac_d_val
)
{

  uint32 mac_d_hfn_l = TDSRRCSMC_INVALID_HFN_VALUE;
  uecomdef_status_e_type ret_val=SUCCESS;
    
  if (mac_d_hfn_intial_value_present)
  {
    if(TDSMAC_D_HFN_LENGTH != mac_d_hfn_intial_value_ptr->numbits) 
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid MAC-D-HFN length: 0x%x",mac_d_hfn_intial_value_ptr->numbits);
      ret_val = FAILURE;
    } 
    else 
    {
      uint32 idx = 0;
      mac_d_hfn_l = 0;    
      for(; idx < TDSMAC_D_HFN_LENGTH / 8; idx++) 
      {
        mac_d_hfn_l |= (uint8)mac_d_hfn_intial_value_ptr->data[idx];
        mac_d_hfn_l <<= 8;
      }
      mac_d_hfn_l = (mac_d_hfn_l >> 8);
    }
  }
  *mac_d_val = mac_d_hfn_l;
  return (ret_val);
}

/*===========================================================================
FUNCTION tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch()

DESCRIPTION
  This function fills chan_config_req with RBs in tdsrrc_rb_list_mapped_to_dch 
  if they are not already present in chan_config_req.
 
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrc_fill_rb_id_chan_req_from_rb_list_mapped_to_dch
(
  uint32 *rb_count,
  tdsrrc_channel_config_req_type *chan_config_req
)
{
  uint8 rb_idx = 0;
  uint8 am_entity_idx = 0;
  chan_config_req->num_rb_to_config = (int) *rb_count;
  
  /*Loop thru all the RBs that are present in tdsrrc_rb_list_mapped_to_dch*/
  for(rb_idx = 0;((rb_idx < tdsrrc_rb_list_mapped_to_dch.num_of_RBs)&&(*rb_count < TDS_MAX_RB));rb_idx++)
  {
    /*Check whether the RB is already part of chan_config_req*/
    if(TDS_MAX_RB == tdsrrcllc_check_if_rbid_present_in_chan_config(chan_config_req,
        tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[rb_idx]))
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Fill RB %d from tdsrrc_rb_list_mapped_to_dch", tdsrrc_rb_list_mapped_to_dch.
            rrc_rb_mapped_to_dch[rb_idx],0,0);
      chan_config_req->rb[(*rb_count)].rb_id = tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[rb_idx];
      
      if((am_entity_idx =tdsrrc_get_am_index(tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[rb_idx])) 
          < TDSUE_MAX_AM_ENTITY)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Re-establish RB-ID %d at AM index %d in OC",tdsrrc_rb_list_mapped_to_dch.
              rrc_rb_mapped_to_dch[rb_idx],am_entity_idx,0);
        tdsordered_config_ptr->am_config[am_entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_DL_UL;
      }

      chan_config_req->rb[(*rb_count)].rb_config = TDSRECONFIG_RB_OTA;
      chan_config_req->rb[(*rb_count)].ul_cfg_mask = chan_config_req->rb[(*rb_count)].
            dl_cfg_mask = TDSMODIFY_RB_CFG;
      (*rb_count)++;        
    }
  }
  chan_config_req->num_rb_to_config = (int) *rb_count;
  /*Since we have used all the RBs in tdsrrc_rb_list_mapped_to_dch, we can init it.*/
  tdsrrcllc_init_rb_list_mapped_to_dch();
}


/*===========================================================================
FUNCTION tdsrrc_fill_rb_id_chan_req_from_est_rabs()

DESCRIPTION
  This function fills existing AM PS RBs in chan_config_req if they are 
  already not part of chan_config_req.
 
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void tdsrrc_fill_rb_id_chan_req_from_est_rabs
(
  uint32 *rb_count,
  tdsrrc_channel_config_req_type *chan_config_req
)
{
  uint32 chan_idx;
  uint8 rab_idx = 0, rb_idx = 0;
  uint8 am_entity_idx = 0;
  uint8 um_ul_entity_idx = UE_MAX_UL_UM_CHANNEL;
  uint8 um_dl_entity_idx = UE_MAX_DL_UM_CHANNEL;
  #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
  uint8 i=0;
  #endif

  chan_config_req->num_rb_to_config = (int) *rb_count;

  for(rab_idx = 0; ((rab_idx < MAX_RAB_TO_SETUP) && (*rb_count < TDS_MAX_RB)); rab_idx++)
  {
    /*Check for only PS RBs*/
    if((tdsrrc_est_rabs.rabs[rab_idx].cn_domain == RRC_PS_DOMAIN_CN_ID) && (tdsrrc_est_rabs.rabs[rab_idx].
          num_rbs_for_rab >0))
    {
      for(rb_idx = 0; ((rb_idx < tdsrrc_est_rabs.rabs[rab_idx].num_rbs_for_rab) && (*rb_count < TDS_MAX_RB)); rb_idx++)
      {
        if((tdsrrcllc_check_if_rlc_mode_is_am(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) == TRUE)
        {
           chan_idx = tdsrrcllc_check_if_rbid_present_in_chan_config(chan_config_req,
                    tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
         /*Check whether the RB is already part of chan_config_req*/      
          if( TDS_MAX_RB == chan_idx )
          {
            tdsrrc_state_e_type dest_state;
            tdsordered_config_type * config_ptr;

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Fill PS AM RB %d from tdsrrc_est_rabs", tdsrrc_est_rabs.rabs[rab_idx].
                  rb_for_rab[rb_idx].rb_id,0,0);
            chan_config_req->rb[(*rb_count)].rb_id = tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id;
            /*Get TOC ptr for DCh->FCAH, as the RLC params are taken from TOC in this case
                    for DCH->FACH transition */
            if(tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH)
            {
              dest_state = TDSRRC_STATE_CELL_FACH;
              config_ptr= tdstransition_config.toc_ptr;
              if((am_entity_idx =tdsrrc_get_am_index_in_toc(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                >= TDSUE_MAX_AM_ENTITY)
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in TOC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                break;
              }
            }
            else if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) || (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
            {
              #ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
              if (TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
              {
              dest_state = TDSRRC_STATE_CELL_FACH;
              config_ptr= tdsordered_config_ptr;
              /* at this time the ul_nchan and dl_nchan is zero , we need handle such case here, after PCRC/RBR/...
                 to  LLC, LLC will put the ul_nchan and dl_nchan to 1 automatically*/
              if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && (TDSRRC_STATE_CELL_FACH == tdsrrcllc_get_target_state()))
              {
                for(i=0;i < TDSUE_MAX_AM_ENTITY;i++)
                {
                  if (tdsordered_config_ptr->rlc_am_parms.am_info[i]->common_config.rb_id == tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)
                  {
                    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Find one PS AM RB %d from tdsrrc_est_rabs", tdsrrc_est_rabs.rabs[rab_idx].
                             rb_for_rab[rb_idx].rb_id,0,0);
                    am_entity_idx = i;
                    break;
                  }
                }
                if(i >= TDSUE_MAX_AM_ENTITY)
                {
                  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Can't find one PS AM RB %d from common configuration", tdsrrc_est_rabs.rabs[rab_idx].
                             rb_for_rab[rb_idx].rb_id,0,0);
                  break;
                }
              }
              else
              {
              
                dest_state = TDSRRC_STATE_CELL_DCH;
                config_ptr= tdsordered_config_ptr;
                if((am_entity_idx =tdsrrc_get_am_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                            >= TDSUE_MAX_AM_ENTITY)
                {
                   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                   break;
                }
                }
              }
              else
              {
                dest_state = TDSRRC_STATE_CELL_DCH;
                config_ptr= tdsordered_config_ptr;
                if((am_entity_idx =tdsrrc_get_am_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                            >= TDSUE_MAX_AM_ENTITY)
                {
                   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                   break;
                }
              }
              #else
                dest_state = TDSRRC_STATE_CELL_DCH;
                config_ptr= tdsordered_config_ptr;
                if((am_entity_idx =tdsrrc_get_am_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                            >= TDSUE_MAX_AM_ENTITY)
                {
                   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                   break;
                }
              #endif
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"didnt expect this function to be called in %d state",tdsrrc_get_state());
              break;
            }
            if(config_ptr->rlc_am_parms.am_info[am_entity_idx]->reestab_entity == TDSRLC_RE_ESTABLISH_NONE )
            {
            
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Re-establish RB-ID %d at old size  %d in OC/TOC at state %d",
                   tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id,
                   config_ptr->rlc_am_parms.am_info[am_entity_idx]->
                rlc_dl_am_config.dl_rlc_size ,dest_state);
              /* No need to set the UL_DL reestablish flag, as it may not reestablished by LLC.  */
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
              if (TDSCHECK_RLC_ALL_RB_REEST_ENABLE)
              {
                /* No need to set the UL_DL reestablish flag, as it may not reestablished by LLC.  */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"No need to set the ul dl reestablish flag if the rb was already setup");
                
              }
              else 
              {
                config_ptr->am_config[am_entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_DL_UL;
              }
              
#endif
  
            }
  
            chan_config_req->rb[(*rb_count)].rb_config = TDSRECONFIG_RB_OTA;
            chan_config_req->rb[(*rb_count)].ul_cfg_mask = chan_config_req->rb[(*rb_count)].
                  dl_cfg_mask = TDSMODIFY_RB_CFG;
            (*rb_count)++;      
          }
          /* If rlc size has changed then set the re-establish flag*/
          else 
          {

            if(chan_idx < TDS_MAX_RB)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"Chan req has rb-id %d action %d",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id,chan_config_req->rb[chan_idx].rb_config);

              if(chan_config_req->rb[chan_idx].rb_id == tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id )
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Chan req has rb-id %d",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                if(((chan_config_req->rb[chan_idx].rb_config == TDSRECONFIG_RB )
                  || (chan_config_req->rb[chan_idx].rb_config == TDSRECONFIG_RB_OTA )
                )  &&(tdsrrcllc_get_am_dl_rlc_size_ota(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)
                  == TDSRRCLLC_INVALID_RLC_SIZE)
                )
                {
                  tdsrrc_state_e_type dest_state;
                  tdsordered_config_type * config_ptr;
                  /*Get TOC ptr for DCh->FCAH, as the RLC params are taken from TOC in this case
                    for DCH->FACH transition */
                  if(tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH)
                  {
                    dest_state = TDSRRC_STATE_CELL_FACH;
                    config_ptr= tdstransition_config.toc_ptr;
                    if((am_entity_idx =tdsrrc_get_am_index_in_toc(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                      >= TDSUE_MAX_AM_ENTITY)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in TOC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                      break;
                    }
                  }
                  else if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) || (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
                  {
                    dest_state = TDSRRC_STATE_CELL_DCH;
                    config_ptr= tdsordered_config_ptr;
                    if((am_entity_idx =tdsrrc_get_am_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                      >= TDSUE_MAX_AM_ENTITY)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get AM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
                      break;
                    }
                  }
                  else
                  {
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"didnt expect this function to be called in %d state",tdsrrc_get_state());
                    break;
                  }
                  TDSRRC_MSG2(MSG_LEGACY_HIGH," contents restablish %d idx %d",config_ptr->rlc_am_parms.am_info[am_entity_idx]->reestab_entity,am_entity_idx);
                  if(config_ptr->rlc_am_parms.am_info[am_entity_idx]->reestab_entity == TDSRLC_RE_ESTABLISH_NONE )
                  {
                    
                    if((config_ptr->rlc_am_parms.am_info[am_entity_idx]->
                      rlc_dl_am_config.dl_rlc_size != tdsrrcllc_get_dl_ded_rlc_size(dest_state, 
                      tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) || (tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH) )
                    {
                       TDSRRC_MSG3(MSG_LEGACY_HIGH,"Re-establish RB-ID %d at old size  %d in OC/TOC at state %d",
                         tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id,
                         config_ptr->rlc_am_parms.am_info[am_entity_idx]->
                      rlc_dl_am_config.dl_rlc_size ,dest_state);
                      config_ptr->am_config[am_entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_DL_UL;
                    }
                  }
                }
              }
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid chan_idx %d",chan_idx);
            }
          }
        }
        else if ((tdsrrcllc_check_if_rlc_mode_is_um(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) == TRUE)
        {
           chan_idx = tdsrrcllc_check_if_rbid_present_in_chan_config(chan_config_req,
                    tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
         /*Check whether the RB is already part of chan_config_req*/      
          if( TDS_MAX_RB == chan_idx )
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Fill PS UM RB %d from tdsrrc_est_rabs", tdsrrc_est_rabs.rabs[rab_idx].
                  rb_for_rab[rb_idx].rb_id,0,0);
            chan_config_req->rb[(*rb_count)].rb_id = tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id;
            /*Get TOC ptr for DCh->FCAH, as the RLC params are taken from TOC in this case
                    for DCH->FACH transition */
            if(tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH)
            {
              if((um_ul_entity_idx = tdsrrcllc_get_um_ul_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id, tdstransition_config.toc_ptr)) 
                >= UE_MAX_UL_UM_CHANNEL)
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get UL UM index for rb-id %d in TOC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
              }
            }
            else if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) || (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
            {
              if((um_ul_entity_idx = tdsrrcllc_get_ul_um_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id)) 
                          >= UE_MAX_UL_UM_CHANNEL)
              {
                 TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get UL UM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
              }
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"didnt expect this function to be called in %d state",tdsrrc_get_state());
            }
						
            if(tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH)
            {
              if((um_dl_entity_idx =  tdsrrcllc_get_dl_um_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id,tdstransition_config.toc_ptr)) 
                >= UE_MAX_UL_UM_CHANNEL)
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get UM index for rb-id %d in TOC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
              }
            }
            else if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) || (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
            {
              if((um_dl_entity_idx = tdsrrcllc_get_dl_um_index(tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id,tdsordered_config_ptr)) 
                          >= UE_MAX_DL_UM_CHANNEL)
              {
                 TDSRRC_MSG1(MSG_LEGACY_ERROR,"Didnt get UM index for rb-id %d in OC",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
              }
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"didnt expect this function to be called in %d state",tdsrrc_get_state());
            }

            if((um_ul_entity_idx < UE_MAX_UL_UM_CHANNEL) && (um_dl_entity_idx < UE_MAX_DL_UM_CHANNEL))
            {
                chan_config_req->rb[(*rb_count)].rb_config = TDSRECONFIG_RB_OTA;
                chan_config_req->rb[(*rb_count)].ul_cfg_mask = chan_config_req->rb[(*rb_count)].dl_cfg_mask = TDSMODIFY_RB_CFG;
                (*rb_count)++;		
            }
            else if (um_dl_entity_idx < UE_MAX_DL_UM_CHANNEL)
            {
                chan_config_req->rb[(*rb_count)].rb_config = TDSRECONFIG_RB_OTA;
                chan_config_req->rb[(*rb_count)].dl_cfg_mask = TDSMODIFY_RB_CFG;
                chan_config_req->rb[(*rb_count)].ul_cfg_mask = 0;												
                (*rb_count)++;		
            }
            else if (um_ul_entity_idx < UE_MAX_UL_UM_CHANNEL)
            {
                chan_config_req->rb[(*rb_count)].rb_config = TDSRECONFIG_RB_OTA;
                chan_config_req->rb[(*rb_count)].ul_cfg_mask = TDSMODIFY_RB_CFG;
                chan_config_req->rb[(*rb_count)].dl_cfg_mask = 0;	
                (*rb_count)++;		
            }						
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"UM Mode RB ID %d already present in chan_config_req",tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_idx].rb_id);
          }
        }			
      }
    }
  }
  chan_config_req->num_rb_to_config = (int) *rb_count;
}

/*===========================================================================

FUNCTION   tdsrrc_fill_rb_id_chan_req_from_standalone_db

DESCRIPTION
  Function to Extract the RB affected list based on Current and Ordered Standalone database

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_fill_rb_id_chan_req_from_standalone_db
(
uint32  *rb_count , 
tdsrrc_channel_config_req_type *chan_config_req
)
{
  uint8 i;
  boolean fill_rb_info;

  chan_config_req->num_rb_to_config = (int) *rb_count;
  for(i=0;((i<TDSMAX_HANGING_RB_MAPPING_INFO)&& (*rb_count < TDS_MAX_RB));i++)
  {
    if(tdsrrc_current_hanging_rb_mapping[i].in_use == TRUE)
    {
      fill_rb_info = FALSE;
      if(tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE)
      {
        /* The Transport Channel got added for the Standalone RBs resulting in
         *  modification of Standalone Database.
         */
        if(FALSE == tdsrrcllc_check_ordered_current_mapping_same(i))
        {
          /* Skipping the case when the RB-ID was already present in the OTA message. */
          if( TDS_MAX_RB == tdsrrcllc_check_if_rbid_present_in_chan_config(
                 chan_config_req, tdsrrc_current_hanging_rb_mapping[i].rb_id ))
          {
            fill_rb_info = TRUE;
            TDSRRC_MSG1(MSG_LEGACY_HIGH," fill rb-id %d in chan req",tdsrrc_current_hanging_rb_mapping[i].rb_id);
          }
        }
      }
      else
      { 
        /* The Transport Channel got added for the Standalone RBs resulting in deleting of
        * of the RB-ID from the Standalone database.
        */
        /* Skipping the case when the RB-ID was already present in the OTA message. */
        if( TDS_MAX_RB == tdsrrcllc_check_if_rbid_present_in_chan_config(
                     chan_config_req, tdsrrc_current_hanging_rb_mapping[i].rb_id ))
        {
          fill_rb_info = TRUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH," Rb not in ordered,fill rb-id %d in  chan req",tdsrrc_current_hanging_rb_mapping[i].rb_id);
        }
      }
      if( fill_rb_info == TRUE)
      {
        chan_config_req->rb[(*rb_count)].rb_id = tdsrrc_current_hanging_rb_mapping[i].rb_id;
        
        if( (TDSRRCLCM_RLC_LC_ID_NOT_FOUND == tdsrrclcm_get_dl_rlc_lc_id
                (   tdsrrc_current_hanging_rb_mapping[i].rb_id )) &&
                (TDSRRCLCM_RLC_LC_ID_NOT_FOUND == tdsrrclcm_get_ul_rlc_lc_id
                (   tdsrrc_current_hanging_rb_mapping[i].rb_id )))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Adding RB-ID %d from Standalone Database in Chan req ",tdsrrc_current_hanging_rb_mapping[i].rb_id);
          chan_config_req->rb[(*rb_count)++].rb_config  = TDSESTABLISH_RB;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Adding RB-ID %d from Standalone Database in Chan req",tdsrrc_current_hanging_rb_mapping[i].rb_id);
          chan_config_req->rb[(*rb_count)++].rb_config  = TDSRECONFIG_RB;
        }
      }
    }
  }
    /* Extracting the RB affected list based o Current and Ordered Standalone database. */
  chan_config_req->num_rb_to_config = (int) *rb_count;
}
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION tdsrrcrb_check_if_proc_waiting_for_new_cell_ind


DESCRIPTION
 
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/

boolean tdsrrcrb_check_if_proc_waiting_for_new_cell_ind
(
  tdsrrc_proc_e_type *                proc_id        
)
{

    
  tdsrrcllc_oc_process_state_e_type  process_state;  

  (void) tdsrrcllc_get_ordered_config_state_and_proc(proc_id, &process_state);
  if (((*proc_id == TDSRRC_PROCEDURE_RBE || *proc_id == TDSRRC_PROCEDURE_RBRC || 
                * proc_id == TDSRRC_PROCEDURE_TCR || *proc_id == TDSRRC_PROCEDURE_PCR ||
                 *proc_id == TDSRRC_PROCEDURE_RBR) &&
      ( tdsrrcrb_get_proc_sub_state(*proc_id) == TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF ))||
      ((*proc_id == TDSRRC_PROCEDURE_CU) && (tdsrrccu_return_cu_substate() == TDSRRCCU_WAIT_FOR_UE_TO_CAMP_ON)&&
        tdsrrccu_check_if_cell_selection_in_progress() == TRUE)
      )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Proc waiting for new cell id %d",*proc_id);
    return TRUE;
  }
  return FALSE;
}
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/


/*============================================================================
FUNCTION: tdsrrcllc_validate_rb_info_reconfig_list_r7()

DESCRIPTION:
  This function validates the IE tdsrrc_RB_InformationReconfigList_r7 and return
  SUCCESS or FAILURE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_validate_rb_info_reconfig_list_r7
(
  tdsrrc_RB_InformationReconfigList_r7 *rrc_rb_information_reconfig_list
)
{
  boolean msg_not_valid = FALSE;
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;  
#endif /* FEATURE_UMTS_PDCP*/
  rlc_lc_id_type                          lc_id;
  uecomdef_logch_mode_e_type              lc_mode;  
  uint32 rb_idx=0;
  tdsrrc_RB_InformationReconfig_r7 * rb_information_reconfig_ptr;
  do
  { 
   rb_information_reconfig_ptr = &rrc_rb_information_reconfig_list->elem[rb_idx];
    /* Check whether rb id in rb reconfig list is in the established rabs list.  If not set msg_not_valid variable
       to true to resend cell update msg with failure cause  */
    if (TDSRRC_RB_NOT_PRESENT == tdsrrc_validate_rb_id_in_reconfig_list(
        rb_information_reconfig_ptr->rb_Identity))
    {
      /* RB Message was invalid. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in rb_InformationReconfigList",rb_information_reconfig_ptr->rb_Identity );
      msg_not_valid = TRUE;
      break;
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_information_reconfig_ptr,
        tdsrrc_RB_InformationReconfig_r7,rb_StopContinue))
    {
      if ((rb_information_reconfig_ptr->rb_Identity) <= 2)
      {
        /* not sure why this check is present */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"indicating stop/continue for RB %d <= 2",(uint16)(rb_information_reconfig_ptr->rb_Identity));
        msg_not_valid = TRUE;
        break;
      }
      
      lc_id = tdsrrclcm_get_ul_rlc_lc_id(
              rb_information_reconfig_ptr->rb_Identity);
      
      if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"No corresponding LC id for the RB %d",(uint16)(rb_information_reconfig_ptr->rb_Identity));
        break;
      }
      else if (tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode) == TDSRRCLCM_FAILURE)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid lc_mode for RB %d",rb_information_reconfig_ptr->rb_Identity);
        break;
      }
      else if (lc_mode == UE_MODE_TRANSPARENT)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Transparent mode RB %d lc_id %d can't be stopped/contd",(uint16)(rb_information_reconfig_ptr->rb_Identity),lc_id);
        break;
      }
    }

#ifdef FEATURE_UMTS_PDCP
    if (tdsrrc_pdcp_enabled)  
    { 
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_information_reconfig_ptr,
          tdsrrc_RB_InformationReconfig_r7,pdcp_SN_Info)) 
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid: PDCP SN info not supported CU CNF");
        break;
      }
      
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_information_reconfig_ptr,
          tdsrrc_RB_InformationReconfig_r7,pdcp_Info))
      {
        /* Validate PDCP info */
        if (tdsrrc_get_rab_type_for_given_rb_id(
            rb_information_reconfig_ptr->rb_Identity) != TDSRRCRB_PS_DATA_RAB)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"NW incld PDCP info for non-ps rb id %d: Invalid",rb_information_reconfig_ptr->rb_Identity);
          msg_not_valid = TRUE;
          break;
        }
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_information_reconfig_ptr,
            rrc_RB_InformationReconfig_r7,pdcp_Info))
        {
          /* check whether lossless SRNS relocation flag is set or   
           * and header compression algo is present in the message 
           */
          if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info,
               tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
              (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
               rrc_LosslessSRNS_RelocSupport_supported)))
          {
            if (rb_information_reconfig_ptr->pdcp_Info.
                pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP pdu header is absent.  Invalid config");
              msg_not_valid = TRUE;
              break;
            }
         
          } /* if rrc_PDCP_Info_r4_losslessSRNS_RelocSupport_present */
        }

        if (rb_information_reconfig_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) 
        {
          if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info,
               tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
              (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport, 
               rrc_LosslessSRNS_RelocSupport_notSupported)))
          {
            if (!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP header compress info list not present");
              msg_not_valid = TRUE;
              break;
            }
          }
        }
        
        if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_information_reconfig_ptr->pdcp_Info.pdcp_Info,
            tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) 
        {
          uint32 local_idx =0;
          while (rb_information_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.n > local_idx) 
          {
            pdcpHeaderCompressionInfo = &rb_information_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.elem[local_idx];
            if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
                rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
            {
              /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
               * failure with cause "Invalid Config"
               */
              if (
                  (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE))
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max header comprss context space exceeded : %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
                msg_not_valid = TRUE;
                break;
              }
            }
            else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == 
                     T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
            {
              /*    the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
               *  indicated in the IE "PDCP Capability"
               */
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.
                rfc3095_Info,ul_RFC3095))
              {
                if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.
                    rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
                  msg_not_valid = TRUE;
                  break;
                }
              }
              
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                dl_RFC3095))
              {
                if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.
                    u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
                  msg_not_valid = TRUE;
                  break;
                }
              }

            }/* else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == rrc_rfc3095_Info)*/

            local_idx++;
          } /*  while (pdcpHeaderCompressionInfoList) */
        } /*  if (rb_list_ptr->value.pdcp_Info.pdcp_Info.bit_mask & rrc_PDCP_Info_r4_headerCompressionInfoList_present) */         
      }
    }
#endif /* FEATURE_UMTS_PDCP */

    /* Advance the Linked List pointer */
    rb_idx++;
  } while(rrc_rb_information_reconfig_list->n > rb_idx) ;

  if (TRUE == msg_not_valid)
  {
    return(FAILURE);
  }
  else
  {
    return(SUCCESS);
  }
}

/*===========================================================================

FUNCTION   rrcrbrc_update_nas_sync_ind_r7

DESCRIPTION Updates the NAS Syncronisation Indicator field received in the RB Reconfig message.
            In case the NAS Syncronisation Indicator IE is not present , then a default value of 
            current active  codec is used.

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrc_update_nas_sync_ind_r7
(
   uint32 rab_index,
   tdsrrcrb_rab_info_per_msg_type   * rabs_info,
   tdsrrc_NAS_Synchronisation_Indicator *nas_Synchronisation_Indicator 
)
{
  rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator = 
  tdsrrc_translate_nas_sync_indicator(nas_Synchronisation_Indicator); 
   
  if( rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE)
  {

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unsupported  configuration .. codec not supported %d",rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator );
    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrc_set_ordered_codec_status(TRUE);
#endif
  tdsrrc_set_ordered_codec_mode(rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator);
  return SUCCESS;
}



/*===========================================================================

FUNCTION tdsrrc_update_rab_info_reconfig_param

DESCRIPTION


  
DEPENDENCIES


RETURN VALUE
  Success/Failure
 

SIDE EFFECTS

  None

===========================================================================*/
tdsrrcrb_msg_status_e_type 
tdsrrc_update_rab_info_reconfig_param
(
  tdsrrc_RAB_InformationReconfigList *rab_list_ptr,
  tdsrrcrb_rab_info_per_msg_type   * rabs_info,
  uint32 * nas_sync_ind
)
{
  uint32 rab_index=0; 
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type; 
  tdsrrc_rab_search_e_type tdsrab_status; 
  TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R7 Reconfig OTA.");

  for(rab_index = 0; (rab_list_ptr->n > rab_index) && ( rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
  {
    /* Check if the RAB id is GSM_MAP type. We don't support any other type */
    if (rab_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RABid t%d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Identity.t);
      return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    }

    /* GSM_MAP RAB Id. Check if it's valid by calling the appropriate NAS entity. 
     First convert it to internal format. */
    rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));

    /* Check if the RAB id is already in ESTABLISHED_RABS */
    tdsrab_status = tdsrrc_find_rab_in_established_rabs(rab_id);
    if ( tdsrab_status != TDSRRC_RAB_FOUND )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RAB ID 0X%x not in ESTABLISHED_RABS",rab_id);
      return TDSRRCRB_MSG_INVALID_CONFIG;
    }

    /* Store this info locally so that ESTABLISHED_RABS can
     be updated later, when these RABs are established */
    rabs_info->rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
    rabs_info->rabs_in_msg[rab_index].rab_id = rab_id;

    /* Check if this RAB is CS Domain or PS Domain */
    if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      rabs_info->rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;

      /* Check if it is in loopback test */
      if ( tdsrrclbt_is_lb_on() == TRUE )
      {
        /* Set Rab type to Data */
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
        rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
      }
      else
      {
        /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
        rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
        rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"For RAB-Id %d,RAB type returned from NAS = %d",rab_id,rab_type);

        switch ( rab_type )
        {
          case RRCMN_CS_DATA_CALL:
            rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
            break;
  
          case RRCMN_CS_VOICE_CALL:
            rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;
  
#ifdef FEATURE_SCUDIF_ENHANCEMENT
            #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
            break;
  
          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
            return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        } /* end switch */

        /*As a general approach we should use/update the NAS Sync Indicator even for CS DATA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#else
        if(rabs_info->rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
        {
          if(tdsrrc_update_nas_sync_ind_r7(rab_index, rabs_info, 
              &(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator)) == FAILURE)
          {
            return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;    
          }
          *nas_sync_ind = rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
        }
      }
    }
    /* Else check for PS Domain */
    else if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      rabs_info->rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS RAB-Id %d to be Reconfig",rab_id);
      rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
    }
  } // for each RAB info

  rabs_info->num_rabs_in_msg = rab_index;
  return TDSRRCRB_MSG_VALID;
}



/*===========================================================================

FUNCTION tdsrrc_update_reconfig_waiting_for_l2ack_dch

DESCRIPTION
Updates the procedure waiting for L2 Ack for the reconfig complete msg sent  in DCH  state

  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_update_reconfig_waiting_for_l2ack_dch
(
    tdsrrc_proc_e_type proc_id
)
{
  tdsrrc_wait_for_l2ack_for_reconfig = proc_id;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Updated wait for proc id %d",proc_id);
}

/*===========================================================================

FUNCTION tdsrrc_check_any_proc_waiting_for_l2_ack

DESCRIPTION
Determines, if any procedure is waiting for the L2 ACk msg for the
reconfig  complete msg sent in DCH

  
DEPENDENCIES


RETURN VALUE
  TRUE:  Some prcocedure is waiting for l2 ack
  else FALSE
 

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrc_check_any_proc_waiting_for_l2_ack
(
  void
)
{
  boolean status = FALSE;
  switch(tdsrrc_wait_for_l2ack_for_reconfig)
  {
    case TDSRRC_PROCEDURE_RCE:
    case TDSRRC_PROCEDURE_RBE:
    case TDSRRC_PROCEDURE_RBRC:
    case TDSRRC_PROCEDURE_RBR:
    case  TDSRRC_PROCEDURE_TCR:
    case  TDSRRC_PROCEDURE_PCR:
    case TDSRRC_PROCEDURE_CU:
      status= TRUE;
    break;
    
    default:
      status= FALSE;
    break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Proc waiting for l2 ack %d",status);
  return status;
}

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrc_update_rab_info_reconfig_param_r8

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE
  Success/Failure

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrb_msg_status_e_type tdsrrc_update_rab_info_reconfig_param_r8
(
  tdsrrc_RAB_InformationReconfigList_r8 *rab_list_ptr,
  tdsrrcrb_rab_info_per_msg_type   * rabs_info,
  uint32 * nas_sync_ind
)
{
  uint32 rab_index=0; 
  rrc_rab_id_type rab_id;           /* To store RAB id in internal RRC format */
  rrcmn_rab_status_T rab_type;
  TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB Info Reconfig in R8 Reconfig OTA.");
  
  for (rab_index = 0; (rab_list_ptr->n > rab_index) && (rab_index < TDSRRCRB_MAX_RAB_PER_MSG); rab_index++)
  {
    /* Check if the RAB id is GSM_MAP type. We don't support any other type */
    if (rab_list_ptr->elem[rab_index].rab_Identity.t != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: RAB-Id t %d is not GSM_MAP!",rab_list_ptr->elem[rab_index].rab_Identity.t);
      return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
    }

    /* GSM_MAP RAB Id. Check if it's valid by calling the appropriate NAS entity. First convert it to internal format. */
    rab_id = tdsrrcrb_translate_gsm_map_rab_id((rab_list_ptr->elem[rab_index].rab_Identity.u.gsm_MAP_RAB_Identity));

    /* Check if the RAB id is already in ESTABLISHED_RABS */
    if (tdsrrc_find_rab_in_established_rabs(rab_id) != TDSRRC_RAB_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: RAB-Id 0X%x not in EST_RABS!",rab_id);
      return TDSRRCRB_MSG_INVALID_CONFIG;
    }

    /* Store this info locally so that ESTABLISHED_RABS can be updated later, when these RABs are established */
    rabs_info->rabs_in_msg[rab_index].rab_id_is_gsm_map = TRUE;
    rabs_info->rabs_in_msg[rab_index].rab_id = rab_id;

    /* Check if this RAB is CS Domain or PS Domain */
    if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      rabs_info->rabs_in_msg[rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;

      /* Check if it is in loopback test */
      if(tdsrrclbt_is_lb_on() == TRUE)
      {
        /* Set Rab type to Data */
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Loopback Test");
        rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
      }
      else
      {
        /* For CS RABs, Call MN layer to verify this RAB */
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
        rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
        rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"For RAB-Id %d,RAB type returned from NAS = %d",rab_id,rab_type);

        switch (rab_type)
        {
          case RRCMN_CS_DATA_CALL:
            rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_DATA_RAB;
            break;

          case RRCMN_CS_VOICE_CALL:
            rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_CS_VOICE_RAB;

#ifdef FEATURE_SCUDIF_ENHANCEMENT
            #error code not present
#endif /*FEATURE_SCUDIF_ENHANCEMENT*/
            break;

          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS RAB validation failed!");
            return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;
        } /* end switch */

        /*As a general approach we should use/update the NAS Sync Indicator even for CS DATA*/
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#else
        if(rabs_info->rabs_in_msg[rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/
        {
          if(tdsrrc_update_nas_sync_ind_r7(rab_index, rabs_info, 
              &(rab_list_ptr->elem[rab_index].nas_Synchronisation_Indicator)) == FAILURE)
          {
            return TDSRRCRB_MSG_UNSUPPORTED_CONFIG;    
          }
          *nas_sync_ind = rabs_info->rabs_in_msg[rab_index].nas_Synchronisation_Indicator;
        }
      }
    }
    /* Else check for PS Domain */
    else if ( rab_list_ptr->elem[rab_index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain )
    {
      /* Store the domain identity. This will be later added to ESTABLISHED_RABS */
      rabs_info->rabs_in_msg[rab_index].cn_domain = RRC_PS_DOMAIN_CN_ID;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: PS RAB-Id %d to be reconfigured.",rab_id);
      rabs_info->rabs_in_msg[rab_index].rab_type = TDSRRCRB_PS_DATA_RAB;
    }

  } // for each RAB info

  rabs_info->num_rabs_in_msg = rab_index;
  return TDSRRCRB_MSG_VALID;
}

/*============================================================================
FUNCTION: tdsrrcllc_validate_rb_info_reconfig_list_r8()

DESCRIPTION:
  This function validates the IE tdsrrc_RB_InformationReconfigList_r8 and return
  SUCCESS or FAILURE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_validate_rb_info_reconfig_list_r8
(
  tdsrrc_RB_InformationReconfigList_r8 *rrc_rb_information_reconfig_list
)
{
  boolean msg_not_valid = FALSE;
  rlc_lc_id_type                           lc_id;
  uecomdef_logch_mode_e_type   lc_mode;  
  tdsrrc_RB_InformationReconfig_r8 *rb_information_reconfig_ptr =NULL;
  uint32 rb_index=0;
  
  do
  { 
    rb_information_reconfig_ptr = &rrc_rb_information_reconfig_list->elem[rb_index];
    /* Check whether rb id in rb reconfig list is in the established rabs list.  If not set msg_not_valid variable
       to true to resend cell update msg with failure cause  */
    if (TDSRRC_RB_NOT_PRESENT == tdsrrc_validate_rb_id_in_reconfig_list(
        rb_information_reconfig_ptr->rb_Identity))
    {
      /* RB Message was invalid. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RB id:%d in rb_InformationReconfigList",rb_information_reconfig_ptr->rb_Identity );
      msg_not_valid = TRUE;
      break;
    }

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_information_reconfig_ptr,
      tdsrrc_RB_InformationReconfig_r8,rb_StopContinue))
    {
      if ((uint16)(rb_information_reconfig_ptr->rb_Identity) <= 2)
      {
        /* not sure why this check is present */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"indicating stop/continue for RB %d <= 2",(uint16)(rb_information_reconfig_ptr->rb_Identity));
        msg_not_valid = TRUE;
        break;
      }
      
      lc_id = tdsrrclcm_get_ul_rlc_lc_id(
              rb_information_reconfig_ptr->rb_Identity);
      
      if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"No corresponding LC id for the RB %d",(uint16)(rb_information_reconfig_ptr->rb_Identity));
        break;
      }
      else if (tdsrrclcm_get_ul_rlc_mode(lc_id, &lc_mode) == TDSRRCLCM_FAILURE)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid lc_mode for RB %d",rb_information_reconfig_ptr->rb_Identity);
        break;
      }
      else if (lc_mode == UE_MODE_TRANSPARENT)
      {
        msg_not_valid = TRUE;
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Transparent mode RB %d lc_id %d can't be stopped/contd",(uint16)(rb_information_reconfig_ptr->rb_Identity),lc_id);
        break;
      }
    }

#ifdef FEATURE_UMTS_PDCP
    if (tdsrrc_pdcp_enabled)  
    {
      /*Validate PDCP related info for this RB-Id*/
      if(tdsrrcllc_validate_pdcp_info_in_rb_info_reconfig_list_r8
        (rb_information_reconfig_ptr) == FALSE)
      {
        msg_not_valid = TRUE;
        break;
      }
    }
#endif /* FEATURE_UMTS_PDCP */

    /* Advance the Linked List pointer */
   rb_index++;
  } while(rrc_rb_information_reconfig_list->n > rb_index) ;

  if (TRUE == msg_not_valid)
  {
    return(FAILURE);
  }
  else
  {
    return(SUCCESS);
  }
}

#ifdef FEATURE_UMTS_PDCP
/*============================================================================
FUNCTION: tdsrrcllc_validate_pdcp_info_in_rb_info_reconfig_list_r8()

DESCRIPTION:
  This function validates the IE tdsrrc_RB_InformationReconfig_r8 for PDCP Info return
  TRUE or FALSE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE/FALSE

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_validate_pdcp_info_in_rb_info_reconfig_list_r8
(
  tdsrrc_RB_InformationReconfig_r8 * rb_reconfig_ptr
)
{ 
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;  
#endif /* FEATURE_UMTS_PDCP*/

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_reconfig_ptr,
    tdsrrc_RB_InformationReconfig_r8,pdcp_SN_Info)) 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PDCP SN info not supported.");
    return FALSE;
  }

  /*RRC_GKG: Find a reference what type of PDCP validation is done by QCOM.*/
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_reconfig_ptr,
    tdsrrc_RB_InformationReconfig_r8,pdcp_Info))
  {
    /*RRC_GKG: Later a check will be added for CSoHSPA. Beacuse CS RAB can have PDCP Info.*/
    if(tdsrrc_get_rab_type_for_given_rb_id(rb_reconfig_ptr->rb_Identity) != TDSRRCRB_PS_DATA_RAB)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: NW incld PDCP info for non-PS rb id %d",rb_reconfig_ptr->rb_Identity);
      return FALSE;
    }
    
    /*Check whether lossless SRNS relocation flag is set and PDCP header is absent in the message */
    if((TDSRRC_MSG_COMMON_BITMASK_IE(rb_reconfig_ptr->pdcp_Info.pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
        (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
         rrc_LosslessSRNS_RelocSupport_supported)))
    {
      if(rb_reconfig_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PDCP pdu header is absent for Lossless SRNS Relocation");
        return FALSE;
      }
    }

    /*Check whether PDCP header is present and Lossless SRNS relocation flag isn't set and Header compression 
    Lits is absent.*/
    if(rb_reconfig_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) 
    {
      if ((TDSRRC_MSG_COMMON_BITMASK_IE(rb_reconfig_ptr->pdcp_Info.pdcp_Info,
        tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) && 
        (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_reconfig_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
         rrc_LosslessSRNS_RelocSupport_notSupported)))
      {
        if(!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_reconfig_ptr->pdcp_Info.pdcp_Info,
            tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) )
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PDCP header compression info list not present");
          return FALSE;
        }
      }
    }
    
    if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_reconfig_ptr->pdcp_Info.pdcp_Info,
        tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) 
    {
      uint32 local_idx=0;
      while (rb_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.n > local_idx) 
      {
         pdcpHeaderCompressionInfo = &rb_reconfig_ptr->pdcp_Info.pdcp_Info.headerCompressionInfoList.elem[local_idx];
        if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
            rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
        {
          /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
           * failure with cause "Invalid Config" */
          if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Max header comprss context space exceeded : %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
            return FALSE;
          }
        }
        else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == 
                 T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
        {
          /*The chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" 
            as indicated in the IE "PDCP Capability" */
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
                ul_RFC3095))
          {
            if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
              return FALSE;
            }
          }
          
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,
               dl_RFC3095))
          {
            if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
              return FALSE;
            }
          }

        }/* else if (rrc_rfc3095_Info)*/

        local_idx++;
      } /*  while (pdcpHeaderCompressionInfoList) */
    } /*  if (rrc_PDCP_Info_r4_headerCompressionInfoList_present) */         
  }/*if (rrc_RB_InformationReconfig_r8_pdcp_Info_present)*/

  return TRUE;
}

/*============================================================================
FUNCTION: tdsrrcllc_validate_pdcp_info_in_rb_info_setup_list_r8()

DESCRIPTION:
  This function validates the IE tdsrrc_RB_InformationSetupList_r8 for PDCP Info return
  TRUE or FALSE accordingly
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE/FALSE

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_validate_pdcp_info_in_rb_info_setup_list_r8
(
  tdsrrc_RB_InformationSetup_r8 * rb_setup_ptr
)
{
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_HeaderCompressionInfo_r4 *pdcpHeaderCompressionInfo = NULL;
#endif /* FEATURE_UMTS_PDCP*/

    /*Check whether lossless SRNS relocation flag is set and PDCP header is absent in the message */
  if((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
    (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_ptr->pdcp_Info.losslessSRNS_RelocSupport,
    rrc_LosslessSRNS_RelocSupport_supported)))
  {
    if (rb_setup_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_absent)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PDCP pdu header is absent for Lossless SRNS Relocation");
      return FALSE;
    }
  } 

  if(rb_setup_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
      (TDSRRC_CHECK_COMMON_MSG_TYPE(rb_setup_ptr->pdcp_Info.losslessSRNS_RelocSupport,
      rrc_LosslessSRNS_RelocSupport_notSupported)))
    {
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_ptr->pdcp_Info,
        tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: PDCP header compression info list not present");
        return FALSE;
      }
    }
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE(rb_setup_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList))
  {
  uint32 local_idx=0;

    while(rb_setup_ptr->pdcp_Info.headerCompressionInfoList.n > local_idx)
    {
        pdcpHeaderCompressionInfo = &rb_setup_ptr->pdcp_Info.headerCompressionInfoList.elem[local_idx];
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(pdcpHeaderCompressionInfo->algorithmSpecificInfo,
          rrc_AlgorithmSpecificInfo_r4_rfc2507_Info))
      {
        /* if the UE capability "Maximum header compression context space", is exceeded with this configuration send
         * failure with cause "Invalid Config"  */
        if(
          (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER > IPHC_MAX_HC_CONTEXT_SPACE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Max header comprss context space exceeded: %d",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc2507_Info->max_HEADER);
          return FALSE;
        }
      }
      else if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.t == T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info)
      {
        /*  the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as 
         *  indicated in the IE "PDCP Capability" */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,ul_RFC3095))
        {
          if (pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID);
            return FALSE;
          }
        }

        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info,dl_RFC3095))
        {
          if(pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID > ROHC_MAX_CONTEXT_SESSIONS)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: Invalid config.  MAX_CID %d is invalid",pdcpHeaderCompressionInfo->algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID);
            return FALSE;
          }
        }
      }  /*rrc_rfc3095_Info*/

      local_idx++;

    } /*  while (pdcpHeaderCompressionInfoList) */
  } /* rrc_PDCP_Info_r4_headerCompressionInfoList_present */
  return TRUE;
}
#endif /*FEATURE_UMTS_PDCP*/
#endif /*FEATURE_TDSCDMA_REL8*/

/*====================================================================
FUNCTION: tdsrrc_ccch_msg_com_critical_ext_present

DESCRIPTION:
  This function is to check whether the required critical extension is present in the message.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if msg's release correct, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrc_ccch_msg_com_critical_ext_present
(
  void *msg_ptr,
  int msg_name,
  int msg_version
)
{
  tdsrrc_DL_CCCH_Message *ccch_ptr;

  ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

  switch (msg_name)
  {
    case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_r4 == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_r5 == 
                    ccch_ptr->message.u.rrcConnectionSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.rrcConnectionSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_r6 == 
                      ccch_ptr->message.u.rrcConnectionSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.rrcConnectionSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.rrcConnectionSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_r7 == 
                        ccch_ptr->message.u.rrcConnectionSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.rrcConnectionSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.rrcConnectionSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_criticalExtensions == 
                        ccch_ptr->message.u.rrcConnectionSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_2_r8 == 
                        ccch_ptr->message.u.rrcConnectionSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_RRCConnectionSetup_later_than_r3 
                == ccch_ptr->message.u.rrcConnectionSetup.t)
          {
            if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.rrcConnectionSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.rrcConnectionSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.rrcConnectionSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_3_criticalExtensions == 
                        ccch_ptr->message.u.rrcConnectionSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_2_criticalExtensions == 
                        ccch_ptr->message.u.rrcConnectionSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_RRCConnectionSetup_criticalExtensions_1_r9 == 
                          ccch_ptr->message.u.rrcConnectionSetup.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid rrcConnectionSetup version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup */
    case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_r4 == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_r5 == 
                    ccch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_r6 == 
                      ccch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_r7 == 
                        ccch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_criticalExtensions == 
                        ccch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_2_r8 == 
                        ccch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3 
                == ccch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
                  ccch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
                    ccch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
                      ccch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_criticalExtensions == 
                        ccch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_2_criticalExtensions == 
                        ccch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_1_r9 == 
                          ccch_ptr->message.u.cellUpdateConfirm.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid cellUpdateConfirm version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm */
    default:
      ERR_FATAL("Invalid CCCH Message %d rcv'd",msg_name,0,0);
      break;
  }

  return FAILURE;
}

/*====================================================================
FUNCTION: tdsrrc_dcch_msg_com_critical_ext_present

DESCRIPTION:
  This function is to check whether the required critical extension is present in the message.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if msg's release correct, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrc_dcch_msg_com_critical_ext_present
(
  void *msg_ptr,
  int msg_name,
  int msg_version
)
{
  tdsrrc_DL_DCCH_Message *dcch_ptr;

  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  switch (msg_name)
  {
    case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.physicalChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.physicalChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.physicalChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.physicalChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.physicalChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.physicalChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.physicalChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.physicalChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.physicalChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.physicalChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.physicalChannelReconfiguration.t)
          {
            if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.physicalChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.physicalChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.physicalChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.physicalChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.physicalChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_PhysicalChannelReconfiguration_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.physicalChannelReconfiguration.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid physicalChannelReconfiguration version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration */
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.radioBearerReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_RadioBearerReconfiguration_later_than_r3 
                == dcch_ptr->message.u.radioBearerReconfiguration.t)
          {
            if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_RadioBearerReconfiguration_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.radioBearerReconfiguration.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid radioBearerReconfiguration version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration */
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerRelease_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.radioBearerRelease.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerRelease_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerRelease.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerRelease_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.radioBearerRelease.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerRelease_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerRelease.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerRelease_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerRelease.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerRelease_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.radioBearerRelease.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerRelease_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerRelease.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerRelease_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerRelease.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerRelease_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerRelease.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerRelease_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.radioBearerRelease.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_RadioBearerRelease_later_than_r3 
                == dcch_ptr->message.u.radioBearerRelease.t)
          {
            if (T_tdsrrc_RadioBearerRelease_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerRelease.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerRelease_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerRelease.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerRelease_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerRelease.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerRelease_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerRelease.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerRelease_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerRelease.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_RadioBearerRelease_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.radioBearerRelease.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid radioBearerRelease version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease */
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerSetup_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.radioBearerSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerSetup_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerSetup_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.radioBearerSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerSetup_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerSetup_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerSetup_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.radioBearerSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerSetup_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerSetup_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerSetup_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerSetup_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.radioBearerSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_RadioBearerSetup_later_than_r3 
                == dcch_ptr->message.u.radioBearerSetup.t)
          {
            if (T_tdsrrc_RadioBearerSetup_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.radioBearerSetup.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_RadioBearerSetup_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.radioBearerSetup.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_RadioBearerSetup_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.radioBearerSetup.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_RadioBearerSetup_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_RadioBearerSetup_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.radioBearerSetup.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_RadioBearerSetup_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.radioBearerSetup.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                      
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid radioBearerSetup version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup */
    case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.transportChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.transportChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.transportChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.transportChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.transportChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.transportChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.transportChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.transportChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.transportChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.transportChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_TransportChannelReconfiguration_later_than_r3 
                == dcch_ptr->message.u.transportChannelReconfiguration.t)
          {
            if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.transportChannelReconfiguration.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.transportChannelReconfiguration.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.transportChannelReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.transportChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.transportChannelReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_TransportChannelReconfiguration_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.transportChannelReconfiguration.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid transportChannelReconfiguration version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration */
    case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
      switch (msg_version)
      {
        case TDSMSG_REL4:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_r4 == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              return SUCCESS;
            }
          }
          break;
        case TDSMSG_REL5:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_r5 == 
                    dcch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                return SUCCESS;
              }
            }
          }
          break;
        case TDSMSG_REL6:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_r6 == 
                      dcch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  return SUCCESS;
                }
              }
            }
          }
          break;
        case TDSMSG_REL7:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_r7 == 
                        dcch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    return SUCCESS;
                  }
                }
              }
            }
          }
          break;
#ifdef FEATURE_TDSCDMA_REL8
        case TDSMSG_REL8:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_2_r8 == 
                        dcch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      return SUCCESS;
                    }
                  }
                }
              }
            }
          }
          break;
#endif
#ifdef FEATURE_TDSCDMA_REL9
        case TDSMSG_REL9:
          if (T_tdsrrc_CellUpdateConfirm_later_than_r3 
                == dcch_ptr->message.u.cellUpdateConfirm.t)
          {
            if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
                  dcch_ptr->message.u.cellUpdateConfirm.
                  u.later_than_r3.criticalExtensions.t)
            {
              if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
                    dcch_ptr->message.u.cellUpdateConfirm.
                    u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.t)
              {
                if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
                      dcch_ptr->message.u.cellUpdateConfirm.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.t)
                {
                  if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_criticalExtensions == 
                        dcch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                  {
                    if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_2_criticalExtensions == 
                        dcch_ptr->message.u.cellUpdateConfirm.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.t)
                    {
                      if (T_tdsrrc_CellUpdateConfirm_criticalExtensions_1_r9 == 
                          dcch_ptr->message.u.cellUpdateConfirm.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.t)
                      {
                        return SUCCESS;
                      }
                    }
                  }
                }
              }
            }
          }
          break;
#endif

        default:
          ERR_FATAL("Invalid cellUpdateConfirm version %d rcv'd",msg_version,0,0);
          break;
      }
      break; /* end T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm */
    default:
      ERR_FATAL("Invalid DCCH Message %d rcv'd",msg_name,0,0);
      break;
  }

  return FAILURE;
}
/*============================================================================
FUNCTION: tdsrrcrb_proc_waiting_for_l2_ack()

DESCRIPTION:
  This function checks is the passed procedure is in waiting for L2 ACK substate
  This function will check for RB procedures only
DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if passed procedure is in waiting for L2 ACK substate
  FALSE otherwise

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcrb_proc_waiting_for_l2_ack
(
  tdsrrc_proc_e_type procedure
)
{
  boolean proc_waiting_for_l2ack = FALSE;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Checking if proc %d is waiting for L2 ACK",procedure);
  switch (procedure)
  {
    case TDSRRC_PROCEDURE_RBE:
    case TDSRRC_PROCEDURE_RBR:
    case TDSRRC_PROCEDURE_RBRC:
    case  TDSRRC_PROCEDURE_TCR:
    case  TDSRRC_PROCEDURE_PCR:
      if(TDSRRCRB_WAIT_FOR_L2_ACK == tdsrrcrb_get_proc_sub_state(procedure))
      {
        proc_waiting_for_l2ack = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Proc is waiting for L2 ACK");
      }
      break;
    case TDSRRC_PROCEDURE_CU:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Procedure is CU,treat as not waiting for L2 ACK");
      break;
    default :
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Procedure");
      break;
  }
  return proc_waiting_for_l2ack;
}

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
/*===========================================================================

FUNCTION   tdsrrc_is_srncid_within_urnti_same

DESCRIPTION

  This function compares the received urnti with the stored value.
  If SRNC ID matches then it returns TRUE else returns FALSE.
          
DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_is_srncid_within_urnti_same
(
  struct tdsrrc_U_RNTI  *ext_urnti     /* Pointer to external U-RNTI type */
)
{
  tdsrrc_int_u_rnti_type                 u_rnti_rcvd;              /* Local varibale to store U-RNTI */
  tdsrrc_int_u_rnti_type                 u_rnti_stored;        /* Local varibale to store U-RNTI */

  /* Translate the U-RNTI */
  tdsrrc_translate_urnti(ext_urnti, &u_rnti_rcvd);

  if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_stored) )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"URNTI is not present");
    return(FALSE);
  }

  if((u_rnti_stored&0xFFF00000) == (u_rnti_rcvd&0xFFF00000))
  {
    return(TRUE);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNC-ID in U-RNTI Mismatch.");
    return(FALSE);
  }
} /* tdsrrc_is_srncid_within_urnti_same */

#endif /*FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO*/



#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION

/*===========================================================================

FUNCTION  tdsrrc_validate_sdu_lifetime_acttime

DESCRIPTION
  This function is used to validate the SDU lifetime for
  an OTA message. 
 
  The function also update the activation time if the recv
  delay is large.

DEPENDENCIES


RETURN VALUE
  TRUE: the OTA has a valid sdu lifetime or activation time. 
  FALSE: Otherwise
 

SIDE EFFECTS
  None
===========================================================================*/

tdsrrcrb_sdu_lifetime_act_time_valid_type tdsrrc_validate_sdu_lifetime_acttime(
  uint32 act_time_present, 
  tdsrrc_ActivationTime act_time
)
{
  tdsrrcrb_sdu_lifetime_act_time_valid_type valid_result;
  uint16 act_offset = 0;
  uint16 first_pdu_cfn = 0;
  uint16 current_cfn = 0; 

  valid_result.act_time_valid = TRUE;
  valid_result.sdu_valid = TRUE;
  valid_result.sdu_discard = FALSE;

  if (tdsrrc_sdu_recv_time < tdsrrc_cell_update_cnf_time)
  {
    valid_result.sdu_discard = TRUE;
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"SDU is transmitted before cell update, Receive time of SDU: %d, time of CU confirm: %d",
                tdsrrc_sdu_recv_time,
                tdsrrc_cell_update_cnf_time);
    return valid_result;
  }
  /*Activatin Offset < Td, then the activation time already passed*/
  if (act_time_present)
  {
    current_cfn = tdsseq_get_cfn();
    first_pdu_cfn = ( 256 + current_cfn - ((tdsrrc_sdu_recv_delay/10) & 0xFF))&0xFF; 
    /*activatio offset = act_time - first_pdu_cfn */
    act_offset = (256 + (uint8)act_time - first_pdu_cfn) & 0xFF;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"current cfn:%d, first pdu cfn: %d, activation time:%d",
                current_cfn, first_pdu_cfn, act_time);

    
    /*if recv delay is larger than act_offset, then it should be act NOW*/
    if (((tdsrrc_sdu_recv_delay/10) & 0xFF) >= act_offset)
    {
      valid_result.act_time_valid = FALSE;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SDU delay is larger than act time offset:%d,ACT NOW!",act_offset);
    }

    /*check the sdu delay > act offset + guard time*/
    if (tdsrrc_sdu_recv_delay > (act_offset*10 + TDSRRC_SDU_LIFE_TIME_GUARD_TIME))
    {
      valid_result.sdu_valid = FALSE;
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SDU delay: %d is too large,discard/ignore this message. Guard time:%d",tdsrrc_sdu_recv_delay,TDSRRC_SDU_LIFE_TIME_GUARD_TIME);

    }
  }
  else
  {
    if (tdsrrc_sdu_recv_delay > TDSRRC_SDU_LIFE_TIME_GUARD_TIME)
    {
      valid_result.sdu_valid = FALSE;
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SDU delay: %d is too large,discard/ignore this message. Guard time:%d",tdsrrc_sdu_recv_delay,TDSRRC_SDU_LIFE_TIME_GUARD_TIME);

    }

  }
  return valid_result;
}

#endif/*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/
