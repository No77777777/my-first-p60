/*===========================================================================
         R R C   I N T E R - R A T   H A N D O V E R   M O D U L E

DESCRIPTION

  This module initiates and coordinats inter-RAT handover procedure including
  both TDS to GSM and GSM to TDS CS handovrs.


EXTERNALIZED FUNCTIONS

  tdsrrciho_event_handler            Main event handler for this module

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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrciho.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/30/13   hx      Made chagnes to send TDSRR_CIPHER_SYNC_IND to G whenever new keys are applied 
11/13/12   ZZ      Add EVENT for inter-RAT HO from UTRAN End.
08/16/12   hx      Merge to M9615ACEHWTAAM4100184.1
03/14/12   as      Added code to support NB/WB handover 
03/12/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
11/28/11   hx      Mainline FEATURE_IRAT_HO_INFO_COMPRESSED for tdsrrc_get_classmark_change_handover_info 
                   Rename FEATURE_IRAT_HO_INFO_COMPRESSED to FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED 
09/08/11   mkh     Events renamed to TDS
08/09/11   mkh     Compile fix on 9x15 due to enabling of FEATURE_TDSCDMA_VOC_AMR_WB
06/10/11   ttl     Change client id to MVS_CLIENT_TDSCDMA for ghdi_mvs_acquire()
03/19/11   hx      Fix: tdsrrc_ProtocolErrorInformation_type1 malloc and checking of the NULL ptr
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/17/11   hx      Fix: change index gsm_commands[gsm_cmd_ct-1] to gsm_commands[gsm_cmd_ct] 
                   to fix the index error. 
02/16/11   hx      Fix: change the tdsrrc_malloc to rtxMemAllocTypeZ for tdsgsm_commands, to fix 
                   the memory release bug 
01/21/11   hx      Fix: gsm_cmd_ptr->n > gsm_cmd_ct to gsm_cmd_ptr->n > TDSRR_MAX_GSM_COMMANDS 
                   in tdsrrciho_verify_and_extract_handover_from_utran_command
01/18/11   hx      Bugfix for tdsrrciho_verify_and_extract_handover_from_utran_command: tdsrrc_RAB_Info_list -> tdsrrc_RAB_Info
01/18/11   hx      Bugfix for tdsrrciho_verify_and_extract_handover_from_utran_command_r6: RAB ps domain error.  
12/16/10   hx      Change ignoring handover_to_utran command to sending back unsupported configuration 
                   to RR.  
12/10/10   hx      Change the switch case from T_tdsrrc_UL_DCCH_MessageType_handoverFromUTRANFailure 
                   to T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM 
                   in tdsrrciho_send_interrat_handover_req 
12/08/10   hx      Change tdsrrc_free to tdsrrcasn1_free_pdu, when ul_lc_id is not ready to 
                   send handover_from_utran_failure OTA back to N\W, to avoid memory leak.
11/24/10   yzh     Add ((FEATURE_TDSCDMA_64QAM) || (FEATURE_TDSCDMA_MIMO)) in tdsrrc_macehs_cat
                   which set value in FEATURE_TDSCDMA_64QAM or FEATURE_TDSCDMA_MIMO.
11/17/10   bj      Made changes to input parameters of function 
                   "tdsrrcueci_append_ue_radio_access_capability"
11/11/10   yzh     (3GPP: 8.6.6.27)del tdsrrcllcpcie_initialise_sync_a_post_veri_info()
                          when IE Downlink information common for all radio links(FDD->TDD).
11/10/10   bj       Deleted IE "RNC support for change of UE capability".
                          (3GPP: 8.6.2.3)

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "bit.h"
#include "comdef.h"
#include "err.h"
#include "msg.h"
#include "customer.h"
#include "memheap.h"
#include "tdsrrcasn1util.h"
#include "tdsrrcdata.h"
#include "tdsrrcsend.h"
#include "tdsrrclcm.h"
#include "tdsrrcmcm.h"
#include "tdsrrcscmgr.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd.h"
#include "tdsrrcintif.h"
#include "tdsrrcllc.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcdt.h"
#include "tdsrrcrce.h"
#include "tdsrrcsmc.h"
#include "tdsrrciho.h"
#include "tdsrrcueci.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
#include "tdsrrcccm.h"
#endif
#include "mm_umts.h"
#include "tdsrrccommon.h"

#include "tdsrrcnv.h"

#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif

#include "tdsl1rrcif.h"

#include "tdsrrcmm_v.h"

#include "tdsseq.h"
#include "tdsrrclogging.h"
#include "event.h"
#include "tdscdmaselfhosting.h"


#include "tdsrrcllcoc.h"

#include "tdsrrcsibproc.h"



/*Added to query RAb type.*/
#include "rrcmnif.h"

#include "tdsrrcmisc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsrrcwrm.h"
#include "tdsCFAParser.h"

#include "tdsrrcdormancy.h"

#include "tdsCFAParser_i.h"

#ifdef FEATURE_LTE_TO_TDSCDMA
#include "rabmtask.h"
#endif /* FEATURE_LTE_TO_TDSCDMA */

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
#include "tdsrrcmeas.h"
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
#include "tdsl1cmmeas.h"
#endif

#include "trm.h"
#include "tdsrrcqsh.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* UMTS->CDMA handover parameters.
*/

#ifndef FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT

typedef enum
{
  IHO_G2TD_COMPLETE_CONFIG,
  IHO_G2TD_PREDEF_CONFIG,
  IHO_G2TD_DEFAULT_CONFIG
}rrciho_g2td_config_e_type;

/* Procedure status */
typedef struct
{
  uint16  count_c_act_time;                  /*count_c activation time sent
                                               in HO to UTRAN complete msg*/
  boolean gsm_ciph_status;                   /*flag to indicate if ciph is
                                               present in GSM RAT         */

  boolean ciph_algo_present;                 /*flag to indicate if ciph
                                               algo is present            */
  uecomdef_umts_cipher_algo_e_type cipheringAlgorithm; /*var to store ciph algo sent
                                               in HO to utran cmd         */

  rrciho_g2td_config_e_type config_type;     /*enum to indicate what config
                                               to use for Handover        */
  uint8               predef_config_id;      /*predefined configuration ID*/
  uint32              default_config_id;     /*default configuration ID   */

  struct tdsrrc_SysInfoType16 * sib16_ptr;      /*ptr to sysInfoType16       */
  boolean             def_config_ptr_present;/*flag to indicate that def
                                               config ptr has been expanded
                                               to SIB16 type.             */

  uint32              maxAllowedUL_TX_Power; /*max allowed UL Tx Power sent
                                               in HOToUtran Command       */

  boolean             act_time_present;      /*flag to indicate if act
                                               time is present in the msg */
  uint32              activation_time;       /*activation time sent in
                                               HOToUtran Command          */

  tdsrrc_HandoverToUTRANCommand *ho_ptr;        /* Pointer to the HO message */
  uint32              ho_msg_len;            /* length of the HO message  */
  boolean             ho_msg_present;        /* boolean to indicate that
                                                we have currently stored
                                                a hoToUtranMsg            */

  tdsrrc_cpid_type  cpid;               /*scrambling code info from
                                               HO to UTRAN cmd            */
  tdsrrc_freq_type      freq;                   /*UARFCN info from the HO to
                                               UTRAN cmd                  */
  int16              rscp;          /* HO to UTRAN target cell pre-measured RSCP by other RAT
                                               used by L1 to skip step 0 and do reAcq */

  boolean            rab_info_included;      /*boolean to indicate if rab
                                               info was included in msg   */

  uint8              num_srb;
  uint8              srb_id[UE_MAX_SRB_SETUP];

#ifndef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_est_rab_info_type  est_rabs;
#else
  uint8  number_of_est_rabs;
  tdsrrc_est_rab_info_type  est_rabs[MAX_RAB_TO_SETUP];
  uint8 cs_rab_index;
  boolean is_cs_rab_present;
  boolean is_ps_rab_present;
#endif /* FEATURE_LTE_TO_TDSCDMA */

  boolean            class_mark_info_present;
  byte *             class_mark_ptr;

#if 0  //FDD only, not needed
  boolean            pn_offset_present;      /*indicates pn_offset present */
  uint16             pn_offset;              /*actual pn_offset            */

  boolean            diversity_info_present; /*indicates if div present    */
  boolean            diversity_info;         /*indicates if known or not   */
#endif

  /* Indicates how a PLMN is to be selected. */
  sys_network_selection_mode_e_type network_select_mode;

  tdsrrc_msg_ver_enum_type  msg_rel_version;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  struct tdsrrc_DL_DCCH_Message *rbs_ptr;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */


#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  /* InterRAT Handover request type, either TDSINTERRAT_HO_TYPE_CS or 
   * TDSINTERRAT_HO_TYPE_PS
   *
   * Replaced by is_cs_rab_present and is_ps_rab_present
   */
  /*tdsrrciho_interrat_handover_req_e_type irat_ho_type;*/
  lte_irat_umts_sec_info_s psho_security_keys;
  /* timing info for re-acq on the target freq */
  uint8                num_of_jds_cells; /* number of JDS cells for interference cancellation */
  uint8                lna_state; /* current LNA state */
  uint16               ts0_position; /* ts0 start position from step0 result */
  uint8                cell_list[TDSIRAT_MAX_TDS_CELLS_PER_FREQ]; /* JDS cell parameter id */

#endif /* FEATURE_LTE_TO_TDSCDMA */

  /*Flag to L1 in the cphy_setup_req*/
  boolean is_x2td_irat_ho;

} tdsrrciho_g2td_info_type;

#endif

/*declare a local object of type g2td_info*/
tdsrrciho_g2td_info_type g2td_info;

#ifdef FEATURE_LTE_TO_TDSCDMA
typedef enum
{
  TDSRRCIHO_INTERRAT_GSM,
#ifdef FEATURE_LTE_TO_TDSCDMA
  TDSRRCIHO_INTERRAT_LTE,
#endif
  TDSRRCIHO_INTERRAT_NONE
}tdsrrciho_interrat_e_type;

tdsrrciho_interrat_e_type tdsrrciho_interrat_type;  
#endif /* FEATURE_LTE_TO_TDSCDMA */

/* Enumerated type of the different inter-RAT from UTRAN failure cause */
typedef enum
{
  TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_HO_MSG_INVALID                 = 0,
  TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED           = 1,
  TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_PHY_CHANNEL_CONFIG_FAILURE     = 2,
  TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_HO_ABORT                       = 3,
  TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_MAX                            = 0xFF
} tdsrrciho_event_interrat_ho_failure_cause_e_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* following two variables are defined in tdsrrcsend.c and used for debugging purpose.
   Whenever there is an encoding or decoding failure, these variables will get
   populated with error_status and other useful information. */
extern tdsrrc_encode_info_type tdsrrc_encode_info;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* MCM state */
tdsrrciho_state_e_type tdsrrciho_state = TDSRRCIHO_INITIAL;

/* Abort indicator */
boolean tdsrrciho_abort = FALSE;

/* Procedure failure statue */
tdsrrciho_procedure_info_type tdsprocedure_info;

/* GSM Command list */
tdsrrc_GSM_MessageList tdsgsm_commands;

#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif

/* should not be in featrue  */
/* #ifdef FEATURE_INTERRAT_HANDOVER_G2TD  */

/* Flag to indicate that RRC is in charge of the Vocoder */
boolean tdsrrciho_mvs_control_flag = FALSE;

/* Flag to indicate that mvs_acquire issued by RRC has succeeded */
boolean tdsrrciho_mvs_acquired_flag = FALSE;
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
extern tdsrrc_ccm_cell_camping_info_type   tdsrrc_ccm_cell_camping_info;

extern rrc_plmn_identity_type tdsrrcccm_get_curr_camped_plmn(void);
#endif

extern boolean tdsrrc_tdrm_exchange_success;
extern trm_reason_enum_t tdsrrc_synced_reason;

/* #endif   */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================

FUNCTION  RRCIHO_INIT_DATA

DESCRIPTION

  This function clears all the module variables and puts the initial
  values wherever needed.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_init_data( void )
{
#ifdef FEATURE_LTE_TO_TDSCDMA
  uint8 idx;
#endif

  /* Reset variables */
  tdsrrciho_state = TDSRRCIHO_INITIAL;
  /* free GSM_commands if not do so */
  if(tdsgsm_commands.n !=0)
    {
      tdsrrc_free(tdsgsm_commands.elem);
    }

  tdsgsm_commands.n=0;
  
  /* Set Abort indication to False */
  tdsrrciho_abort = FALSE;
  tdsprocedure_info.act_time_present=FALSE;
/* initialise the nas_Synchronisation_Indicator to default value */
#ifdef FEATURE_LTE_TO_TDSCDMA
  for(idx =0 ; idx < MAX_RAB_TO_SETUP ; idx++)
  {
    g2td_info.est_rabs[idx].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;
  }
#elif defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_NONE;
#endif /*FEATURE_LTE_TO_TDSCDMA*/

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT)
  if((tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)&&
       (tdsprocedure_info.parms.eutra.eutra_container_size != 0))
  {
    tdsrrc_free(tdsprocedure_info.parms.eutra.eutra_container_ptr);
    tdsprocedure_info.parms.eutra.eutra_container_ptr = NULL;
    tdsprocedure_info.parms.eutra.eutra_container_size = 0;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L_PSHO: freed the EUTRA container ptr", 0, 0, 0);
  }
#endif
  memset(&tdsprocedure_info, 0x00, sizeof(tdsrrciho_procedure_info_type));
  /*Clear the IRAT HO flag*/
  g2td_info.is_x2td_irat_ho = FALSE;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  /*Clear X->T measurement*/
  g2td_info.num_of_jds_cells = 0;
  g2td_info.lna_state = 0;
  g2td_info.ts0_position = 0;
#endif

  return;
}




/*===========================================================================

FUNCTION  SEND_NAS_DATA_TRANSFER_IND

DESCRIPTION

  This function sends TDSRRC_NAS_DATA_TRANSFER_IND to request DT to forward all
  bufferred NAS signaling messages to the GSM RR.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_nas_data_transfer_ind(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_NAS_DATA_TRANSFER_IND", 0, 0, 0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_NAS_DATA_TRANSFER_IND;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_NAS_DATA_TRANSFER_IND sent to DT", 0, 0, 0);

  return;
} /* end of tdsrrciho_send_nas_data_transfer_ind */



/*===========================================================================

FUNCTION   SEND_HANDOVER_FROM_UTRAN_FAILURE

DESCRIPTION

  This function sends a Handover From UTRAN Failure to UTRAN to indicate that
  inter-RAT TDS to GSM handover is failure.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_handover_from_utran_failure( void )
{
  tdsrrc_UL_DCCH_Message *ptr;     /* Pointer to uplink DCCH message type */
  rlc_lc_id_type ul_lc_id;      /* Logical Channel Id for the message  */
  tdsrrcsend_status_e_type status; /* Status from send chain module       */

#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsrrc_HandoverFromUtranFailure_v860ext_IEs*  eutra_ho_msg_ptr;
#endif

  /* Allocate memory for this message. Note that this buffer is released
  by the SEND chain */
  ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                                          sizeof(tdsrrc_UL_DCCH_Message));
  if (ptr == NULL)
  {
    ERR_FATAL("No memory for Handover Failure!", 0, 0, 0);
  }

  ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_handoverFromUTRANFailure;

  /* Initialize the bitmask */
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.handoverFromUTRANFailure);

  /* Set failure cause present. Note: not include GSM messages for 8/1 */
 TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->message.u.handoverFromUTRANFailure,interRAT_HO_FailureCause);

  /* Fill the transaction identifier */
  ptr->message.u.handoverFromUTRANFailure.rrc_TransactionIdentifier =
    tdsprocedure_info.transaction_id;

  /* Fill the failure cause */
  ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.t =
    tdsprocedure_info.failure_cause;

  /* Check the failure cause for the protocol error */
  if (tdsprocedure_info.failure_cause == T_tdsrrc_InterRAT_HO_FailureCause_protocolError)
  {

    ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError=
        rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);
    if(ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    } 
    /* Set the specific protocol error */
    ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError->diagnosticsType.t =
      T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

    ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError->
          diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

      if(ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError->
        diagnosticsType.u.type1 == NULL)
    {
      ERR_FATAL("Memory allocation failure",0,0,0);
    } 

      
    ptr->message.u.handoverFromUTRANFailure.interRAT_HO_FailureCause.u.protocolError->diagnosticsType.u.type1->protocolErrorCause =
      tdsprocedure_info.protocol_error;


    /*log the protocol error*/
    tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                            (uint8)TDSRRC_PROCEDURE_IHO,
                            (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                            tdsrrc_log_map_protocol_cause(tdsprocedure_info.protocol_error)
                            );

  }

if(tdsprocedure_info.failure_cause == T_tdsrrc_InterRAT_HO_FailureCause_interRAT_ProtocolError)
  {
    /* Fill in the inter-RAT message for protocol error. */
#ifdef FEATURE_TDSCDMA_TO_LTE
    if((tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
         && (tdsprocedure_info.parms.eutra.eutra_container_size != 0))
    {
    
       /* Set failure cause present. Note: not include GSM messages for 8/1 */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->message.u.handoverFromUTRANFailure,laterNonCriticalExtensions);
    
      /* Initialize the bitmask */
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.handoverFromUTRANFailure.laterNonCriticalExtensions);
      
      /* Set failure cause present. Note: not include GSM messages for 8/1 */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->message.u.handoverFromUTRANFailure.laterNonCriticalExtensions,v590NonCriticalExtensions);
    
      /* Initialize the bitmask */
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->message.u.handoverFromUTRANFailure.laterNonCriticalExtensions.v590NonCriticalExtensions);
      
      /* Set failure cause present. Note: not include GSM messages for 8/1 */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->message.u.handoverFromUTRANFailure.laterNonCriticalExtensions.v590NonCriticalExtensions
           ,v860NonCriticalExtensions);
    
      eutra_ho_msg_ptr = &ptr->message.u.handoverFromUTRANFailure.laterNonCriticalExtensions.v590NonCriticalExtensions.
                           v860NonCriticalExtensions.handoverFromUTRANFailure_v860ext;
    
      eutra_ho_msg_ptr->m.eutra_MessagePresent = 1;
    
      eutra_ho_msg_ptr->eutra_Message.numocts = tdsprocedure_info.parms.eutra.eutra_container_size;
      eutra_ho_msg_ptr->eutra_Message.data = tdsprocedure_info.parms.eutra.eutra_container_ptr;
    
    }
    else
#endif

    /* Check if GSM message list is present */
    if (tdsgsm_commands.n != 0)
    {
      if ((tdsgsm_commands.elem[0].numbits > TDSRRCIHO_MAX_GSM_MSG_LIST_STRING_BIT_LENGTH) ||
          (tdsgsm_commands.elem[0].numbits == 0)) 
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"numbits %d in GSM command invalid, discard", tdsgsm_commands.elem[0].numbits, 0, 0);
      }
      else
      {
      /* Set inter-RAT message preset flag */
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->message.u.handoverFromUTRANFailure,interRATMessage);
      ptr->message.u.handoverFromUTRANFailure.interRATMessage.t = T_tdsrrc_HandoverFromUTRANFailure_interRATMessage_gsm;
  
      /*The List could not be assigned to the other list by using "=" 
        This is different as in OSS case, where the List is actually a pointer*/
      /*ptr->message.u.handoverFromUTRANFailure.interRATMessage.u.gsm.gsm_MessageList = tdsgsm_commands;*/
      ptr->message.u.handoverFromUTRANFailure.interRATMessage.u.gsm.gsm_MessageList.n = tdsgsm_commands.n;
      ptr->message.u.handoverFromUTRANFailure.interRATMessage.u.gsm.gsm_MessageList.elem = tdsgsm_commands.elem;
      }
    }
  }

  /* Get the RLC logical channel id for this msg from Logical Channel Manager */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         DCCH_AM_RADIO_BEARER_ID,
                                         UE_MODE_ACKNOWLEDGED_DATA);

  /* If the logical channel id is not invalid, send the message to the
  send chain module */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Sending HO Failure message", 0, 0, 0);
    status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_IHO,  /* Procedure is RBE    */
                                 (void *)ptr,        /* Message pointer     */
                                 ul_lc_id,           /* RLC id              */
                                 TDSRRCSEND_L2ACK_NOT_REQUIRED  /* ACK is not required */
                                );
    if( status != TDSRRCSEND_SUCCESS )
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Failed to send HO failure: err %d", status, 0, 0);
    }

  }
  else
  {
    /* Free memory */


/*potential memory leak if using tdsrrc_free*/
    /*tdsrrc_free(ptr);*/
    if(tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, ptr)) 
    {
        /* This is a memory leak and it should never happen. */
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);
    }

    /* Unexpected failure. ERR for now. May need to ERR_FATAL here */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Can't send DCCH msg! invalid lc: %d", ul_lc_id , 0, 0);
  }

  /* free memory */
  if(tdsgsm_commands.n != 0)
  {

    tdsrrc_free( tdsgsm_commands.elem);
    tdsgsm_commands.n =0;
  }

#ifdef FEATURE_TDSCDMA_TO_LTE
  if((tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)&&
      (tdsprocedure_info.parms.eutra.eutra_container_size != 0))
  {
    tdsrrc_free(tdsprocedure_info.parms.eutra.eutra_container_ptr);
    tdsprocedure_info.parms.eutra.eutra_container_ptr = NULL;
    tdsprocedure_info.parms.eutra.eutra_container_size = 0;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L_PSHO: freed the EUTRA container ptr", 0, 0, 0);
  }
#endif
  memset(&tdsprocedure_info, 0x00, sizeof(tdsrrciho_procedure_info_type));
  /*clear up some static vars. as HO procedure is complete for this attempt*/
  tdsprocedure_info.act_time_present=FALSE;

  return;
} /* end of tdsrrciho_send_handover_from_utran_failure */



/*===========================================================================

FUNCTION   tdsrrciho_send_rr_interrat_handover_req

DESCRIPTION

  This function sends a TDSRR_INTERRAT_HANDOVER_REQ to GSM RR to initiate the
  TDS to GSM handover.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_rr_interrat_handover_req(void)
{

  ASSERT( tdsprocedure_info.handover_type ==
                               T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM );

  /* Get inter-RAT measurement data from L1 */
  tdsl1_get_interrat_measurement_data(&tdsprocedure_info.parms.gsm.interrat_measurement_data);

  /*if ciphering is turned on then set cipher_enabled flag to true
    else set it to false*/
  if (tdsrrcsmc_is_ciphering_enabled() == TDSRRCSMC_SECURITY_STARTED)
  {
    tdsprocedure_info.parms.gsm.cipher_enabled = TRUE;
  }
  else
  {
    tdsprocedure_info.parms.gsm.cipher_enabled = FALSE;
  }

  if (tdsrrc_csp_int_data.csfb_extended_status)
  {
    tdsprocedure_info.parms.gsm.csfb_call_status = TRUE;
  }
  else
  {
    tdsprocedure_info.parms.gsm.csfb_call_status = FALSE;
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_HANDOVER_REQ sent to GSM RR with csfb_call_status %d", 
        tdsprocedure_info.parms.gsm.csfb_call_status, 0, 0);

  tdsrrc_csp_int_data.csfb_extended_status = FALSE;

  tdsrrccsp_fill_info_for_rr(&(tdsprocedure_info.parms.gsm.network_select_mode));

  /* Fill in pseudo fast return info if it's turned on */
  if((TRUE == tdsrrccsp_fill_pfr_info_for_rr(&(tdsprocedure_info.parms.gsm.pseudo_fr_info))) &&
     (tdsprocedure_info.parms.gsm.csfb_call_status == FALSE))
  {
    tdsprocedure_info.parms.gsm.pseudo_fr_active = TRUE;
  }
  else
  {
    tdsprocedure_info.parms.gsm.pseudo_fr_active = FALSE;
  }

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_get_rat_pri_list_info(&tdsprocedure_info.parms.gsm.rat_pri_list_info);
#endif

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
  /*copying the dedicated priority info from global structure(rrc_dedicated_priority_info) to rr_interrat_handover_req_type_req*/
  memset(&tdsprocedure_info.parms.gsm.dedicated_priority_info,0,sizeof(lte_irat_dedicated_priority_info_s));
  tdsrrcmeas_populate_dedicated_priority_info_for_irat_req(&tdsprocedure_info.parms.gsm.dedicated_priority_info);
#endif

  /* Send the TDSRR_INTERRAT_HANDOVER_REQ message to RR */
      tdsrr_send_rrc_message_to_rr_internal((void *)(&(tdsprocedure_info.parms.gsm)), TDSRR_INTERRAT_HANDOVER_REQ,
                sizeof(tdsrr_interrat_handover_req_type));

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_HANDOVER_REQ sent to GSM RR", 0, 0, 0);

  return;

} /* end of tdsrrciho_send_rr_interrat_handover_req */



#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT)
/*===========================================================================

FUNCTION   tdsrrciho_send_lte_interrat_handover_req

DESCRIPTION

  This function sends a LTE_INTERRAT_HANDOVER_REQ to LTE RRC to initiate the
  TDSCDMA to LTE handover.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_lte_ps_ho_req( void )
{
  msgr_attach_struct_type    *msgr_attach_ptr = NULL;
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
  rrc_plmn_identity_type plmn_id = tdsrrcccm_get_curr_camped_plmn();
  tdsrrc_cpid_type cpid = tdsrrcccm_get_curr_camped_cpid();
#endif
  ASSERT( tdsprocedure_info.handover_type ==
        T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA );

  // Build and send out the message request, including the HO Container as first attachment
/*HX TBD MSGR*/
  msgr_init_hdr_attach(&tdsprocedure_info.parms.eutra.msg.msg_hdr, MSGR_LTE_RRC, LTE_RRC_TDSCDMA_PSHO_REQ, 0, 1);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&tdsprocedure_info.parms.eutra.msg.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&tdsprocedure_info.parms.eutra.msg.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  // Set the HandoverToUTRACommand as a DSM attachment
  msgr_attach_ptr = msgr_get_attach(&tdsprocedure_info.parms.eutra.msg.msg_hdr, 0);
  ASSERT(msgr_attach_ptr != NULL);
  msgr_set_dsm_attach(msgr_attach_ptr, tdsprocedure_info.parms.eutra.dsm_ptr);

  ASSERT(tdsprocedure_info.parms.eutra.dsm_ptr != NULL);

  tdsrrccsp_fill_info_for_rr(&(tdsprocedure_info.parms.eutra.msg.irat_nas_info.network_select_mode));

  tdsprocedure_info.parms.eutra.msg.irat_nas_info.req_plmn = 
      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(tdsrrccsp_get_curr_nas_requested_plmn());

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_get_rat_pri_list_info(&tdsprocedure_info.parms.eutra.msg.irat_nas_info.rat_pri_list);
#endif

  tdsprocedure_info.parms.eutra.msg.timing_tx_info.number_of_cells = 0;

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  /* Get EUTRA timing tx info from L1. This API is available only when the CM features are on */
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"LTE PSHO: tdsl1_get_eutra_measurement_data",0, 0, 0);;
  tdsl1_get_eutra_measurement_data(&tdsprocedure_info.parms.eutra.msg.timing_tx_info);
#endif

  if(tdsprocedure_info.parms.eutra.msg.timing_tx_info.number_of_cells > 0)
  {
    tdsprocedure_info.parms.eutra.msg.timing_tx_info_present = TRUE;
  }
  else
  {
    tdsprocedure_info.parms.eutra.msg.timing_tx_info_present = FALSE;
  }

  tdsprocedure_info.parms.eutra.msg.tdstol_fdd_supported = FALSE;
  tdsprocedure_info.parms.eutra.msg.tdstol_tdd_supported = FALSE;

  if(lte_rrc_lte_fdd_is_supported(tdsrrccsp_get_lte_band_pref()))
  {
    tdsprocedure_info.parms.eutra.msg.tdstol_fdd_supported = TRUE;
  }
  if(lte_rrc_lte_tdd_is_supported(tdsrrccsp_get_lte_band_pref()))
  {
    tdsprocedure_info.parms.eutra.msg.tdstol_tdd_supported = TRUE;
  }
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
  tdsprocedure_info.parms.eutra.msg.utra_cell_info.rat = SYS_RAT_TDS_RADIO_ACCESS;
  tdsprocedure_info.parms.eutra.msg.utra_cell_info.frequency = tdsrrcccm_get_curr_camped_freq();
  tdsprocedure_info.parms.eutra.msg.utra_cell_info.phy_cell_id = cpid; 
  if(TRUE == tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
  {
      tdsprocedure_info.parms.eutra.msg.utra_cell_info.is_cell_global_id_present = TRUE;
      tdsprocedure_info.parms.eutra.msg.utra_cell_info.serving_cell_global_id.cell_id = tdsrrcccm_get_curr_camped_cell_id();
      tdsprocedure_info.parms.eutra.msg.utra_cell_info.serving_cell_global_id.plmn_id = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
  }
  else
  {
      tdsprocedure_info.parms.eutra.msg.utra_cell_info.is_cell_global_id_present = FALSE;
  }
#endif  	
  msgr_send(&tdsprocedure_info.parms.eutra.msg.msg_hdr, sizeof(lte_rrc_tdscdma_psho_req_s));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&tdsprocedure_info.parms.eutra.msg.msg_hdr, sizeof(lte_rrc_tdscdma_psho_req_s));
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L: LTE_RRC_TDSCDMA_PSHO_REQ sent to LTE RRC", 0, 0, 0);

  return;
}

/*===========================================================================

FUNCTION   rrciho_send_abort_handover_to_lrrc

DESCRIPTION

  This function sends a LTE_RRC_WCDMA_ABORT_PSHO_REQ to LTE-RRC.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_abort_handover_to_lte( void )
{
  lte_rrc_tdscdma_abort_psho_req_s abort_psho_req;
  
  memset(&abort_psho_req,0,sizeof(lte_rrc_tdscdma_abort_psho_req_s));
 
  msgr_init_hdr(&abort_psho_req.msg_hdr,MSGR_TDSCDMA_RRC,LTE_RRC_TDSCDMA_ABORT_PSHO_REQ);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&abort_psho_req.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&abort_psho_req.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&abort_psho_req.msg_hdr, sizeof(lte_rrc_tdscdma_abort_psho_req_s));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&abort_psho_req.msg_hdr, sizeof(lte_rrc_tdscdma_abort_psho_req_s));
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L: LTE_RRC_TDSCDMA_ABORT_PSHO_REQ sent to LTE RRC", 0, 0, 0);
}
#endif /*FEATURE_TDSCDMA_TO_LTE && FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT*/



/*===========================================================================

FUNCTION   tdsrrciho_send_interrat_handover_req

DESCRIPTION

  This function sends Interrat handover request to either RR for TDS -> GSM
  or CM for TDS -> CDMA2000 handover.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_interrat_handover_req( void )
{

  switch ( tdsprocedure_info.handover_type )
  {
    case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
      tdsrrciho_send_rr_interrat_handover_req();
#ifdef FEATURE_QSH_EVENT_METRIC
      TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_T2G_HO_START);
#endif
      break;

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT)
    case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
      tdsrrciho_send_lte_ps_ho_req();
#ifdef FEATURE_QSH_EVENT_METRIC
      TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_T2L_HO_START);
#endif
      break;
#endif
    default:
      ASSERT( 0 );
      break;

  } /* switch ( tdsprocedure_info.handover_type ) */

} /* end of tdsrrciho_send_interrat_handover_req */


/*===========================================================================

FUNCTION   SEND_ABORT_HANDOVER_REQ

DESCRIPTION

  This function sends a TDSRR_INTERRAT_HANDOVER_ABORT_REQ to GSM RR to abort the
  TDS to GSM handover.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_abort_handover_req( void )
{
  tdsrr_interrat_handover_abort_req_type abort_req;

  memset(&abort_req,0,sizeof(tdsrr_interrat_handover_abort_req_type));

  /* Send the TDSRR_INTERRAT_HANDOVER_REQ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(abort_req)), TDSRR_INTERRAT_HANDOVER_ABORT_REQ,
                sizeof(tdsrr_interrat_handover_abort_req_type));

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRR_INTERRAT_HANDOVER_ABORT_REQ sent to GSM RR", 0, 0, 0);

  return;

} /* end of tdsrrciho_send_abort_handover_req */


/*===========================================================================

FUNCTION   SEND_CHANGE_MODE_REQ

DESCRIPTION

  This function sends a TDSRRC_CHANGE_MODE_REQ to MCM to request to change the
  RRC mode to Standby.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_change_mode_req( tdsrrc_mode_e_type mode )
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CHANGE_MODE_REQ", 0, 0, 0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANGE_MODE_REQ;

  /* Fill in procedure id    */
  cmd_ptr->cmd.change_mode_req.procedure = TDSRRC_PROCEDURE_IHO;

  /* Fill in the requested mode */
  cmd_ptr->cmd.change_mode_req.mode = mode;

  /* Fill the reason for the change mode request */
  cmd_ptr->cmd.change_mode_req.reason = TDSINTERRAT_IHO;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_CHANGE_MODE_REQ sent to MCM", 0, 0, 0);

  return;
} /* end of tdsrrciho_send_change_mode_req */


/*===========================================================================

FUNCTION   SEND_CHANNEL_CONFIG_REQ_TO_LLC

DESCRIPTION

  This function sends a TDSRRC_CHANNEL_CONFIG_REQ to the other RRC LLC to revert
  physical channels to previous UTRAN configuration in TDS L1.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_channel_config_req_to_llc(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CHANNEL_CONFIG_CNF", 0, 0, 0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Fill in procedure id */
  cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_IHO;

  /* Indicate that state change is not required */
  cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;

  /* Indicate that no confirmation is required from LLC */
  cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_REQ sent to LLC", 0, 0, 0);

  return;
} /* end of tdsrrciho_send_channel_config_req_to_llc */

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*===========================================================================

FUNCTION   tdsrrciho_delete_stored_g2td_info

DESCRIPTION

  Clean up all allocated pointers, clear all variables and delete all the
  stuff stored in the static variable g2td_info.

  This may be called due to a successful HO or an unsuccessful HO or due
  to receiving a HandoverAbortReq


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_delete_stored_g2td_info(void)
{
  /*delete the HO to Utran msg by calling tdsrrcasn1_free_pdu*/
  if(g2td_info.ho_msg_present == TRUE)
  {

    /*free the msg_ptr previously allocated.. if failure, flag an error*/
    if(tdsrrcasn1_free_pdu((int)tdsrrc_HandoverToUTRANCommand_PDU, g2td_info.ho_ptr))
    {
      // This is a memory leak and it should never happen.
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);
    }
    else
    {
      /*clean the msg_ptr data structure*/
      g2td_info.ho_msg_len = 0;
      g2td_info.ho_msg_present = FALSE;
    }
  }

  /*delete default config info if it was expanded*/
  if(g2td_info.def_config_ptr_present == TRUE)
  {
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if(g2td_info.rbs_ptr != NULL)
    {
      if(tdsrrcasn1_free_pdu((int)tdsrrc_DL_DCCH_Message_PDU, g2td_info.rbs_ptr))
      {
        /* This is a memory leak and it should never happen.*/
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);    
      }
      else
      {
        /*clean the msg_ptr data structure*/
        g2td_info.def_config_ptr_present = FALSE;
      } 
    }
    else
 #endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    {
      /*free the msg_ptr previously allocated.. if failure, flag an error*/
      if(tdsrrciho_free_sib16() == FAILURE)
      {
        // This is a memory leak and it should never happen.
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);
      }
      else
      {
        /*clean the msg_ptr data structure*/
        g2td_info.def_config_ptr_present = FALSE;
      }
    }
  } 
  /*delete class mark information*/
  if(g2td_info.class_mark_info_present == TRUE)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory leak.. Classmark info undeleted",0,0,0);

    // memory freeing for encoded_sdu should be done here using OSS compiler
    // functions.
    tdsrrc_free(g2td_info.class_mark_ptr);

    /*reset the static variables*/
    g2td_info.class_mark_info_present = FALSE;
    g2td_info.class_mark_ptr = NULL;
  }

  /*set the other stored vars to false or default.  do not delete rab_info_included
    as it is needed to send SYNC indication later.  It will be deleted after sending
    SYNC_IND to MM*/
  g2td_info.act_time_present = FALSE;
  g2td_info.ciph_algo_present = FALSE;
  g2td_info.num_srb = 0;
  g2td_info.maxAllowedUL_TX_Power = 0;
#if 0 //FDD only, not needed 
  g2td_info.diversity_info_present = FALSE;
  g2td_info.diversity_info = FALSE;
  g2td_info.pn_offset_present = FALSE;
  g2td_info.pn_offset = 0;
#endif

}/*tdsrrciho_delete_stored_g2td_info*/
/*===========================================================================

FUNCTION   tdsrrciho_initialize_srb_list

DESCRIPTION

  Initializes GTOW information for Signalling Radio Bearer list


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_srb_list(tdsrrc_SRB_InformationSetupList *srb_ptr)
{
  uint8 srb_count=0;
  rb_id_type rb_id = CCCH_RADIO_BEARER_ID;

  while ((srb_ptr!=NULL )&& (srb_ptr->n > srb_count ))
  {
    /* Get the SRB ID from the message if present */
    if(srb_ptr->elem[srb_count].m.rb_IdentityPresent)
    {
      rb_id = srb_ptr->elem[srb_count].rb_Identity;
    }
    else
    {
      /* Else, increment from the previous SRB Id */
      rb_id++;
    }
    

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */
    if ( srb_count >= UE_MAX_SRB_SETUP )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX SRB count exceeded!", 0, 0, 0);
      break;
    }

    g2td_info.srb_id[srb_count] = (uint8)rb_id;

    /* Go to the next RB */

    /* Increment count of RBs */
    srb_count++;

  }/* end while */

  /* Save the number of SRBs */
  g2td_info.num_srb = srb_count;
}/*tdsrrciho_initialize_srb_list*/
/*===========================================================================

FUNCTION   tdsrrciho_initialize_srb_list_r5

DESCRIPTION

  Initializes GTOW information for Signalling Radio Bearer list


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_srb_list_r5(tdsrrc_SRB_InformationSetupList_r5 *srb_ptr)
{
  uint8 srb_count=0;
  rb_id_type rb_id = CCCH_RADIO_BEARER_ID;

  while ((srb_ptr!=NULL )&& (srb_ptr->n > srb_count ))
  {
    /* Get the SRB ID from the message if present */
    if(srb_ptr->elem[srb_count].m.rb_IdentityPresent)
    {
      rb_id = srb_ptr->elem[srb_count].rb_Identity;
    }
    else
    {
      /* Else, increment from the previous SRB Id */
      rb_id++;
    }
    
    if ( srb_count >= UE_MAX_SRB_SETUP )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX SRB count exceeded!", 0, 0, 0);
      break;
    }
    g2td_info.srb_id[srb_count] = (uint8)rb_id;

    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */


    /* Increment count of RBs */
    srb_count++;

  }/* end while */

  /* Save the number of SRBs */
  g2td_info.num_srb = srb_count;
}/*tdsrrciho_initialize_srb_list_r5*/
/*===========================================================================

FUNCTION   tdsrrciho_initialize_srb_list_r6

DESCRIPTION

  Initializes GTOW information for Signalling Radio Bearer list


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_srb_list_r6(tdsrrc_SRB_InformationSetupList_r6 *srb_ptr)
{
  uint8 srb_count=0;
  rb_id_type rb_id = CCCH_RADIO_BEARER_ID;

  while ( (srb_ptr!=NULL )&& (srb_ptr->n > srb_count))
  {
    /* Get the SRB ID from the message if present */
    if(srb_ptr->elem[srb_count].m.rb_IdentityPresent)
    {
      rb_id = srb_ptr->elem[srb_count].rb_Identity;
    }
    else
    {
      /* Else, increment from the previous SRB Id */
      rb_id++;
    }
    
    if ( srb_count >= UE_MAX_SRB_SETUP )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX SRB count exceeded!", 0, 0, 0);
      break;
    }

    g2td_info.srb_id[srb_count] = (uint8)rb_id;

    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */


    /* Increment count of RBs */
    srb_count++;

  }/* end while */

  /* Save the number of SRBs */
  g2td_info.num_srb = srb_count;
}/*tdsrrciho_initialize_srb_list_r6*/
/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list(tdsrrc_RB_InformationSetupList *rb_ptr
                               #ifdef FEATURE_LTE_TO_TDSCDMA
                                    , uint8 index
                               #endif
                               )
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL)&& (rb_ptr->n > rb_count ))
  {
    if ( rb_count >= MAX_RB_PER_RAB )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX RB count exceeded!", 0, 0, 0);
      break;
    }

#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#endif /* FEATURE_LTE_TO_TDSCDMA */    
    /* Increment count of RBs */
    rb_count++;
  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif /* FEATURE_LTE_TO_TDSCDMA */
}/*rrciho_initialize_rb_list*/

/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list_r4

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list_r4(tdsrrc_RB_InformationSetupList_r4 *rb_ptr
                               #ifdef FEATURE_LTE_TO_TDSCDMA
                               , uint8 index
                               #endif
                               )
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL)&& (rb_ptr->n > rb_count ))
  {
    if ( rb_count >= MAX_RB_PER_RAB )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX RB count exceeded!", 0, 0, 0);
      break;
    }

#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#endif /* FEATURE_LTE_TO_TDSCDMA */

    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */


    /* Increment count of RBs */
    rb_count++;

  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif /* FEATURE_LTE_TO_TDSCDMA */
}/*tdsrrciho_initialize_rb_list_r4*/
/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list_r5

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list_r5(tdsrrc_RB_InformationSetupList_r5 *rb_ptr
                                  #ifdef FEATURE_LTE_TO_TDSCDMA
                                  , uint8 index
                                  #endif
                                  )
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL )&&( rb_ptr->n > rb_count ))
  {

    if ( rb_count >= MAX_RB_PER_RAB )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX RB count exceeded!", 0, 0, 0);
      break;
    }

#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#endif /* FEATURE_LTE_TO_TDSCDMA */
    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */

    /* Increment count of RBs */
    rb_count++;

  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif /* FEATURE_LTE_TO_TDSCDMA */
}/*tdsrrciho_initialize_rb_list_r5*/

/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list_r6

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list_r6(tdsrrc_RB_InformationSetupList_r6 *rb_ptr
                                  #ifdef FEATURE_LTE_TO_TDSCDMA
                                  , uint8 index
                                  #endif
                                  )
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL )&& (rb_ptr->n > rb_count ))
  {

    if ( rb_count >= MAX_RB_PER_RAB )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX RB count exceeded!", 0, 0, 0);
      break;
    }

#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#endif /* FEATURE_LTE_TO_TDSCDMA */
    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */

    /* Increment count of RBs */
    rb_count++;

  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif /* FEATURE_LTE_TO_TDSCDMA */
}/*tdsrrciho_initialize_rb_list_r6*/

/*===========================================================================

FUNCTION   tdsrrciho_initialize_srb_list_r7

DESCRIPTION

  Initializes GTOW information for Signalling Radio Bearer list


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_srb_list_r7
(
   tdsrrc_SRB_InformationSetupList_r7 *srb_ptr
)
{
  uint8 srb_count=0;
  rb_id_type rb_id = CCCH_RADIO_BEARER_ID;

  while ((srb_ptr!=NULL )&& (srb_ptr->n > srb_count))
  {
    /* Get the SRB ID from the message if present */
    if(srb_ptr->elem[srb_count].m.rb_IdentityPresent)
    {
      rb_id = srb_ptr->elem[srb_count].rb_Identity;
    }
    else
    {
      /* Else, increment from the previous SRB Id */
      rb_id++;
    }
    
    if ( srb_count >= UE_MAX_SRB_SETUP )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX SRB count exceeded!", 0, 0, 0);
      break;
    }

    g2td_info.srb_id[srb_count] = (uint8)rb_id;

    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */


    /* Increment count of RBs */
    srb_count++;

  }/* end while */

  /* Save the number of SRBs */
  g2td_info.num_srb = srb_count;
}
/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list_r7

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list_r7
(
  tdsrrc_RB_InformationSetupList_r7 *rb_ptr
#ifdef FEATURE_LTE_TO_TDSCDMA
, uint8 index
#endif
)
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL )&&( rb_ptr->n > rb_count ) && (rb_count < MAX_RB_PER_RAB))
  {
#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = rb_ptr->elem[rb_count].rb_Identity;
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = (uint8)(rb_ptr->elem[rb_count].rb_Identity);
#endif
    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */

    /* Increment count of RBs */
    rb_count++;

  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif
}/*tdsrrciho_initialize_rb_list_r7*/

#ifndef FEATURE_LTE_TO_TDSCDMA

/*===========================================================================

FUNCTION   tdsrrciho_update_rab_info_r7

DESCRIPTION

  updates GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static tdsrrc_RAB_InformationSetupList_r7 *  tdsrrciho_update_rab_info_r7
(
    uint32 * rab_identity_choice
)
{
  tdsrrc_RB_InformationSetupList_r7  *rb_list_r7 = NULL;
      tdsrrc_HandoverToUTRANCommand_r7_IEs * ho_utran_r7;
  tdsrrc_RAB_InformationSetupList_r7 *rab_list_r7 = NULL;

      ho_utran_r7 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7
             .handoverToUTRANCommand_r7;
   if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == ho_utran_r7->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
   {
     *rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;
   
     /* GSM_MAP RAB Id. Check if it's valid by calling the
        appropriate NAS entity. First convert it to internal
        format. */
   
     g2td_info.est_rabs.rab_id =
        tdsrrcrb_translate_gsm_map_rab_id(ho_utran_r7->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);
   
     g2td_info.est_rabs.re_est_timer = ho_utran_r7->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;
   
     rb_list_r7 =&ho_utran_r7->specificationMode.u.complete.rab_InformationSetupList.elem[0].rb_InformationSetupList;
   
     tdsrrciho_initialize_rb_list_r7(rb_list_r7);
   
     rab_list_r7 = &ho_utran_r7->specificationMode.u.complete.rab_InformationSetupList;
   
     if(rab_list_r7->elem[0].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
     {
       g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
        /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
       if(rab_list_r7->elem[0].rab_Info.m.nas_Synchronisation_IndicatorPresent)
       {
         g2td_info.est_rabs.nas_Synchronisation_Indicator = 
             tdsrrc_translate_nas_sync_indicator(&(rab_list_r7->elem[0].rab_Info.nas_Synchronisation_Indicator));
         
         if( g2td_info.est_rabs.nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
         {
           tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
           TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs.nas_Synchronisation_Indicator , 0, 0);
           return NULL;
         }
       }
       tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs.nas_Synchronisation_Indicator);
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSoHS:Codec in tdsrrc_ota_codec_mode_status %d", tdsrrcdata_codec_info.rrc_ota_codec_mode_status, 0, 0);   
     }
     else
     {
       tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
     }
   

   return rab_list_r7;
     /*lint +e740 */
   }
   return NULL;
}

#endif /* FEATURE_LTE_TO_TDSCDMA */

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION   tdsrrciho_initialize_rb_list_r8

DESCRIPTION

  Initializes GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS?

  None.

===========================================================================*/
static void tdsrrciho_initialize_rb_list_r8
(
   tdsrrc_RB_InformationSetupList_r8 *rb_ptr
#ifdef FEATURE_LTE_TO_TDSCDMA
,  uint8 index
#endif   
)
{

  uint8 rb_count=0;

  while ((rb_ptr!=NULL )&& (rb_ptr->n > rb_count))
  {
    /* Do a safety check on the count. We can't do more than 32
    configs in a command */
    if ( rb_count >= MAX_RB_PER_RAB )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX RB count exceeded!", 0, 0, 0);
      break;
    }

#ifndef FEATURE_LTE_TO_TDSCDMA
    g2td_info.est_rabs.rb_for_rab[rb_count].rb_id = (uint8)(rb_ptr->elem[rb_count].rb_Identity);
#else
    g2td_info.est_rabs[index].rb_for_rab[rb_count].rb_id = (uint8)(rb_ptr->elem[rb_count].rb_Identity);
#endif

    /* Increment count of RBs */
    rb_count++;

  }/* end while */

  /* Save the number of RBs for this RAB */
#ifndef FEATURE_LTE_TO_TDSCDMA
  g2td_info.est_rabs.num_rbs_for_rab = rb_count;
#else
  g2td_info.est_rabs[index].num_rbs_for_rab = rb_count;
#endif
}/*tdsrrciho_initialize_rb_list_r8*/

/*===========================================================================

FUNCTION   tdsrrciho_initialize_srb_list_r8

DESCRIPTION

  Initializes GTOW information for Signalling Radio Bearer list


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initialize_srb_list_r8
(
  tdsrrc_SRB_InformationSetupList_r8 *srb_ptr
)
{
  rb_id_type srb_count=0;
  rb_id_type rb_id = CCCH_RADIO_BEARER_ID;

  while ((srb_ptr!=NULL )&& (srb_ptr->n > srb_count ))
  {
    /* Get the SRB ID from the message if present */
    if(srb_ptr->elem[srb_count].m.rb_IdentityPresent)
    {
      rb_id = srb_ptr->elem[srb_count].rb_Identity;
    }
    else
    {
      /* Else, increment from the previous SRB Id */
      rb_id++;
    }
    
    

    /* Go to the next RB */

    /* Do a safety check on the count. We can't do more than 32
    configs in a command */
    if ( srb_count >= UE_MAX_SRB_SETUP )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"MAX SRB count exceeded!", 0, 0, 0);
      break;
    }

    g2td_info.srb_id[srb_count] = rb_id;

    /* Increment count of RBs */
    srb_count++;

  }/* end while */

  /* Save the number of SRBs */
  g2td_info.num_srb = srb_count;
}
#ifndef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION   tdsrrciho_update_rab_info_r8

DESCRIPTION

  updates GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static tdsrrc_RAB_InformationSetupList_r8 *  tdsrrciho_update_rab_info_r8
(
    uint32 * rab_identity_choice
)
{
      tdsrrc_RB_InformationSetupList_r8  *rb_list_r8 = NULL;
      tdsrrc_HandoverToUTRANCommand_r8_IEs * ho_utran_r8;
         tdsrrc_RAB_InformationSetupList_r8 *rab_list_r8 = NULL;

      ho_utran_r8 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8
             .handoverToUTRANCommand_r8;
   if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == ho_utran_r8->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
   {
     *rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;
   
     /* GSM_MAP RAB Id. Check if it's valid by calling the
        appropriate NAS entity. First convert it to internal
        format. */
   
     g2td_info.est_rabs.rab_id =
        tdsrrcrb_translate_gsm_map_rab_id(ho_utran_r8->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);
   
     g2td_info.est_rabs.re_est_timer = ho_utran_r8->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;
   
     rb_list_r8 =&ho_utran_r8->specificationMode.u.complete.rab_InformationSetupList.elem[0].rb_InformationSetupList;
   
     tdsrrciho_initialize_rb_list_r8(rb_list_r8);
   
     rab_list_r8 = &ho_utran_r8->specificationMode.u.complete.rab_InformationSetupList;
   
     if(rab_list_r8->elem[0].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
     {
       g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
        /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
       if(rab_list_r8->elem[0].rab_Info.m.nas_Synchronisation_IndicatorPresent)
       {
         g2td_info.est_rabs.nas_Synchronisation_Indicator = 
             tdsrrc_translate_nas_sync_indicator(&(rab_list_r8->elem[0].rab_Info.nas_Synchronisation_Indicator));
         
         if( g2td_info.est_rabs.nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
         {
           tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
           TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs.nas_Synchronisation_Indicator , 0, 0);
           return NULL;
         }
       }
       tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs.nas_Synchronisation_Indicator);
     }
     else
     {
       tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
     }
   

   return rab_list_r8;
     /*lint +e740 */
   }
   return NULL;
}
#endif /*FEATURE_LTE_TO_TDSCDMA */
#ifdef FEATURE_TDSCDMA_REL9
#ifndef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION   tdsrrciho_update_rab_info_r9

DESCRIPTION

  updates GTOW information for Radio Bearer list

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static tdsrrc_RAB_InformationSetupList_r8 *  tdsrrciho_update_rab_info_r9
(
    uint32 * rab_identity_choice
)
{
      tdsrrc_RB_InformationSetupList_r8  *rb_list_r9 = NULL;
      tdsrrc_HandoverToUTRANCommand_r9_IEs * ho_utran_r9;
         tdsrrc_RAB_InformationSetupList_r8 *rab_list_r9 = NULL;

      ho_utran_r9 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.handoverToUTRANCommand_r9;
   if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == ho_utran_r9->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
   {
     *rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;
   
     /* GSM_MAP RAB Id. Check if it's valid by calling the
        appropriate NAS entity. First convert it to internal
        format. */
   
     g2td_info.est_rabs.rab_id =
        tdsrrcrb_translate_gsm_map_rab_id(ho_utran_r9->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);
   
     g2td_info.est_rabs.re_est_timer = ho_utran_r9->specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;
   
     rb_list_r9 =&ho_utran_r9->specificationMode.u.complete.rab_InformationSetupList.elem[0].rb_InformationSetupList;
   
     tdsrrciho_initialize_rb_list_r8(rb_list_r9);
   
     rab_list_r9 = &ho_utran_r9->specificationMode.u.complete.rab_InformationSetupList;
   
     if(rab_list_r9->elem[0].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
     {
       g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
        /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
       if(rab_list_r9->elem[0].rab_Info.m.nas_Synchronisation_IndicatorPresent)
       {
         g2td_info.est_rabs.nas_Synchronisation_Indicator = 
             tdsrrc_translate_nas_sync_indicator(&(rab_list_r9->elem[0].rab_Info.nas_Synchronisation_Indicator));
         
         if( g2td_info.est_rabs.nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
         {
           tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
           TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs.nas_Synchronisation_Indicator , 0, 0);
           return NULL;
         }
       }
       tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs.nas_Synchronisation_Indicator);
     }
     else
     {
       tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
     }
   

   return rab_list_r9;
     /*lint +e740 */
   }
   return NULL;
}
#endif /*FEATURE_LTE_TO_TDSCDMA */

#endif /*FEATURE_TDSCDMA_REL9*/
#endif /*FEATURE_TDSCDMA_REL8*/





/*===========================================================================

FUNCTION   tdsrrciho_update_nas_sync_ind

DESCRIPTION Updates the NAS Syncronisation Indicator field received in the handover to utran message.
            In case the NAS Syncronisation Indicator IE is not present , then a default value of 
            NB-AMR codec is used.

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_update_nas_sync_ind
(
  tdsrrc_RAB_Info rab_info
#ifdef FEATURE_LTE_TO_TDSCDMA
  , uint8 index
#endif
)
{
  tdsrrc_est_rab_info_type  *est_rabs;

#ifdef FEATURE_LTE_TO_TDSCDMA
  est_rabs = &(g2td_info.est_rabs[index]);
#else
  est_rabs = &(g2td_info.est_rabs);
#endif /* FEATURE_LTE_TO_TDSCDMA */

  if(rab_info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
  {
    est_rabs->nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
     /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rab_info,tdsrrc_RAB_Info,nas_Synchronisation_Indicator))
    {
      est_rabs->nas_Synchronisation_Indicator = 
          tdsrrc_translate_nas_sync_indicator(&(rab_info.nas_Synchronisation_Indicator));
      
      if( est_rabs->nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
      {
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d", est_rabs->nas_Synchronisation_Indicator , 0, 0);
        return FAILURE;
      }
    }
    tdsrrc_set_ordered_codec_mode(est_rabs->nas_Synchronisation_Indicator);
  }
  else
  {
    tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrciho_update_def_config_nas_sync_ind

DESCRIPTION Updates the NAS Syncronisation Indicator field received in the handover to utran message for default
            configurations .
            In case the NAS Syncronisation Indicator IE is not present , then a default value of 
            NB-AMR codec is used.

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_update_def_config_nas_sync_ind
(
  tdsrrc_RAB_Info_Post * rab_info
#ifdef FEATURE_LTE_TO_TDSCDMA
  , uint8 index
#endif /* FEATURE_LTE_TO_TDSCDMA */
)
{
  tdsrrc_est_rab_info_type  *est_rabs;

#ifdef FEATURE_LTE_TO_TDSCDMA
  est_rabs = &(g2td_info.est_rabs[index]);
#else
  est_rabs = &(g2td_info.est_rabs);
#endif /* FEATURE_LTE_TO_TDSCDMA */

  if(rab_info->cn_DomainIdentity  == tdsrrc_CN_DomainIdentity_cs_domain )
  {
    est_rabs->nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
     /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rab_info,tdsrrc_RAB_Info_Post,
        nas_Synchronisation_Indicator))
    {
      est_rabs->nas_Synchronisation_Indicator =  tdsrrc_translate_nas_sync_indicator(
          &(rab_info->nas_Synchronisation_Indicator));
      
      if( est_rabs->nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
      {
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d", est_rabs->nas_Synchronisation_Indicator , 0, 0);
        return FAILURE;
      }
    }
    tdsrrc_set_ordered_codec_mode(est_rabs->nas_Synchronisation_Indicator);
  }
  else
  {
    tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
  }
  return SUCCESS;
}


#ifdef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION   tdsrrciho_extract_and_fill_in_rab_rb_info

DESCRIPTION

  Extracts rab, rb info and updates est_rabs

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_extract_and_fill_in_rab_rb_info(
                                                              void *rab_info_list,
                                                              tdsrrc_msg_ver_enum_type release
                                                              )
{
  tdsrrc_RB_InformationSetupList_r4  *rb_list = NULL;
  tdsrrc_RAB_InformationSetupList_r4 *rab_list = NULL;
  tdsrrc_RB_InformationSetupList_r5  *rb_list_r5 = NULL;
  tdsrrc_RAB_InformationSetupList_r5 *rab_list_r5 = NULL;
  tdsrrc_RB_InformationSetupList_r6  *rb_list_r6 = NULL;
  tdsrrc_RAB_InformationSetupList_r6 *rab_list_r6 = NULL;
  tdsrrc_RB_InformationSetupList_r7  *rb_list_r7 = NULL;
  tdsrrc_RAB_InformationSetupList_r7 *rab_list_r7 = NULL;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RB_InformationSetupList_r8  *rb_list_r8 = NULL;
  tdsrrc_RAB_InformationSetupList_r8  *rab_list_r8 = NULL;
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    tdsrrc_RB_InformationSetupList_r8  *rb_list_r9 = NULL;
    tdsrrc_RAB_InformationSetupList_r8  *rab_list_r9 = NULL;
#endif /*FEATURE_TDSCDMA_REL9*/

  uint8 number_of_rabs = 0;
  uint8 idx = 0;

  tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);

  if(release == TDSMSG_REL4)
  {
    rab_list = (tdsrrc_RAB_InformationSetupList_r4 *) rab_info_list;

    do 
    {
      /* Return failure to LTE-RRC in case CS RAB is included in handover info msg. */
      /* Removed above check ??? HX: TBD confirmed*/

      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list->elem[idx].rab_Info.rab_Identity.t)
      {

        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list->elem[idx].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);

        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list->elem[idx].rab_Info.re_EstablishmentTimer;

        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list->elem[idx].rab_Info.cn_DomainIdentity;

        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;

        rb_list = &rab_list->elem[idx].rb_InformationSetupList;

        tdsrrciho_initialize_rb_list_r4(rb_list,number_of_rabs);

        (void)tdsrrc_update_rab_type_in_rab_info_r4(
                                                 &(rab_list->elem[idx]),
                                                 &(g2td_info.est_rabs[number_of_rabs]));

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                                                     g2td_info.est_rabs[number_of_rabs].rab_id,
                                                     g2td_info.est_rabs[number_of_rabs].cn_domain,
                                                     g2td_info.est_rabs[number_of_rabs].re_est_timer);

        if(rab_list->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
        }
        else if(rab_list->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }

         /* in the G-> W handover check the Nas-Sync-indicator for the codec type */
        if((rab_list->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) && 
           (tdsrrciho_update_nas_sync_ind(rab_list->elem[idx].rab_Info, number_of_rabs) == FAILURE))
        {
          return FAILURE;
        }
        number_of_rabs++;
      }
      else
      {
        return FAILURE;
      }
      idx++;
    }while((idx < rab_list->n) && (number_of_rabs < MAX_RAB_TO_SETUP));

  }
  else if(release == TDSMSG_REL5)
  {
    rab_list_r5 = (tdsrrc_RAB_InformationSetupList_r5 *) rab_info_list;

    do 
    {

      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list_r5->elem[idx].rab_Info.rab_Identity.t)
      {

        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list_r5->elem[idx].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);

        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list_r5->elem[idx].rab_Info.re_EstablishmentTimer;

        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list_r5->elem[idx].rab_Info.cn_DomainIdentity;

        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;

        rb_list_r5 = &rab_list_r5->elem[idx].rb_InformationSetupList;

        tdsrrciho_initialize_rb_list_r5(rb_list_r5,number_of_rabs);

        (void)tdsrrc_update_rab_type_in_rab_info_r5(
                                                 &(rab_list_r5->elem[idx]),
                                                 &(g2td_info.est_rabs[number_of_rabs]));

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                                                     g2td_info.est_rabs[number_of_rabs].rab_id,
                                                     g2td_info.est_rabs[number_of_rabs].cn_domain,
                                                     g2td_info.est_rabs[number_of_rabs].re_est_timer);

        if(rab_list_r5->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
        }
        else if(rab_list_r5->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }

         /* in the G-> W handover check the Nas-Sync-indicator for the codec type */
        if((rab_list_r5->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) && 
           (tdsrrciho_update_nas_sync_ind(rab_list_r5->elem[idx].rab_Info, number_of_rabs) == FAILURE))
        {
          return FAILURE;
        }
        number_of_rabs++;
      }
      else
      {
        return FAILURE;
      }
      idx++;
    }while((idx < rab_list_r5->n) && (number_of_rabs < MAX_RAB_TO_SETUP));

  }
  else if(release == TDSMSG_REL6)
  {
    rab_list_r6 = (tdsrrc_RAB_InformationSetupList_r6*) rab_info_list;

    do 
    {

      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list_r6->elem[idx].rab_Info.rab_Identity.t)
      {

        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list_r6->elem[idx].rab_Info.
                                       rab_Identity.u.gsm_MAP_RAB_Identity);

        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list_r6->elem[idx].rab_Info.re_EstablishmentTimer;

        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list_r6->elem[idx].rab_Info.cn_DomainIdentity;

        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;

        rb_list_r6 = &rab_list_r6->elem[idx].rb_InformationSetupList;

        tdsrrciho_initialize_rb_list_r6(rb_list_r6,number_of_rabs);

        (void)tdsrrc_update_rab_type_in_rab_info_r6(
                                                 &(rab_list_r6->elem[idx]),
                                                 &(g2td_info.est_rabs[number_of_rabs]));

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                                                     g2td_info.est_rabs[number_of_rabs].rab_id,
                                                     g2td_info.est_rabs[number_of_rabs].cn_domain,
                                                     g2td_info.est_rabs[number_of_rabs].re_est_timer);

        if(rab_list_r6->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
        }
        else if(rab_list_r6->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }


          if(rab_list_r6->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
          {
            g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
         /* in the G-> W handover check the Nas-Sync-indicator for the codec type */
            if(rab_list_r6->elem[idx].rab_Info.m.nas_Synchronisation_IndicatorPresent)
            {
              g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = 
                  tdsrrc_translate_nas_sync_indicator(&(rab_list_r6->elem[idx].rab_Info.nas_Synchronisation_Indicator));
              
              if( g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
        {
                tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator , 0, 0);
          return FAILURE;
        }
            }
            tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
          }
          else
          {
            tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
          }

        number_of_rabs++;
      }
      else
      {
        return FAILURE;
      }
      idx++;
    }while((idx < rab_list_r6->n) && (number_of_rabs < MAX_RAB_TO_SETUP));

  }
  else if(release == TDSMSG_REL7)
  {
    rab_list_r7 = (tdsrrc_RAB_InformationSetupList_r7 *) rab_info_list;

    do 
    {

      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list_r7->elem[idx].rab_Info.rab_Identity.t)
      {
        /* GSM_MAP RAB Id. Check if it's valid by calling the
           appropriate NAS entity. First convert it to internal
           format. */
      
        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list_r7->elem[idx].rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list_r7->elem[idx].rab_Info.re_EstablishmentTimer;

        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list_r7->elem[idx].rab_Info.cn_DomainIdentity;

        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;

        rb_list_r7 = &rab_list_r7->elem[idx].rb_InformationSetupList;

        tdsrrciho_initialize_rb_list_r7(rb_list_r7, number_of_rabs);

        (void)tdsrrc_update_rab_type_in_rab_info_r7(
           &(rab_list_r7->elem[idx]),
           &(g2td_info.est_rabs[number_of_rabs]));

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                          g2td_info.est_rabs[number_of_rabs].rab_id,
                          g2td_info.est_rabs[number_of_rabs].cn_domain,
                          g2td_info.est_rabs[number_of_rabs].re_est_timer);

        if(rab_list_r7->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
        }
        else if(rab_list_r7->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }


        if(rab_list_r7->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
        {
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
           /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
          if(rab_list_r7->elem[idx].rab_Info.m.nas_Synchronisation_IndicatorPresent)
          {
            g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = 
                tdsrrc_translate_nas_sync_indicator(&(rab_list_r7->elem[idx].rab_Info.nas_Synchronisation_Indicator));
            
            if( g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator , 0, 0);
              return FAILURE;
            }
          }
          tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSoHS:Codec in tdsrrc_ota_codec_mode_status %d", tdsrrcdata_codec_info.rrc_ota_codec_mode_status, 0, 0);   
        }
        else
        {
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
       }

        number_of_rabs++;
      }


      idx++;
    }while((idx < rab_list_r7->n) && (number_of_rabs < MAX_RAB_TO_SETUP));
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if(release == TDSMSG_REL8)
  {
    rab_list_r8 = (tdsrrc_RAB_InformationSetupList_r8 *) rab_info_list;

    do 
    {


      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list_r8->elem[idx].rab_Info.rab_Identity.t)
      {
        /* GSM_MAP RAB Id. Check if it's valid by calling the
           appropriate NAS entity. First convert it to internal
           format. */
      
        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list_r8->elem[idx].rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list_r8->elem[idx].rab_Info.re_EstablishmentTimer;
        // Nitin Change from rab_list_r6 to rab_list_r8
        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list_r8->elem[idx].rab_Info.cn_DomainIdentity;
    
        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;
  
        rb_list_r8 = &rab_list_r8->elem[idx].rb_InformationSetupList;
      
        tdsrrciho_initialize_rb_list_r8(rb_list_r8, number_of_rabs);
  
        (void)tdsrrc_update_rab_type_in_rab_info_r8(
           &(rab_list_r8->elem[idx]),
           &(g2td_info.est_rabs[number_of_rabs]));
  
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                          g2td_info.est_rabs[number_of_rabs].rab_id,
                          g2td_info.est_rabs[number_of_rabs].cn_domain,
                          g2td_info.est_rabs[number_of_rabs].re_est_timer);
  
        if(rab_list_r8->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
          if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
          {
            g2td_info.est_rabs[number_of_rabs].rab_type = TDSRRCRB_CS_VOICE_RAB;
          }
        }
        else if(rab_list_r8->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }

        if(rab_list_r8->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
        {
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
           /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
          if(rab_list_r8->elem[idx].rab_Info.m.nas_Synchronisation_IndicatorPresent)
          {
            g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = 
                tdsrrc_translate_nas_sync_indicator(&(rab_list_r8->elem[idx].rab_Info.nas_Synchronisation_Indicator));
            
            if( g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator , 0, 0);
              return NULL;
            }
          }
          tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
        }
        else
        {
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
        }

        number_of_rabs++;
      }
      else
      {
        return FAILURE;
      }
      idx++;
    }while((idx < rab_list_r8->n) && (number_of_rabs < MAX_RAB_TO_SETUP));
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  else if(release == TDSMSG_REL9)
  {
    rab_list_r9 = (tdsrrc_RAB_InformationSetupList_r8 *) rab_info_list;

    do 
    {


      if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == rab_list_r9->elem[idx].rab_Info.rab_Identity.t)
      {
        /* GSM_MAP RAB Id. Check if it's valid by calling the
           appropriate NAS entity. First convert it to internal
           format. */
      
        g2td_info.est_rabs[number_of_rabs].rab_id =
           tdsrrcrb_translate_gsm_map_rab_id(rab_list_r9->elem[idx].rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
        g2td_info.est_rabs[number_of_rabs].re_est_timer = rab_list_r9->elem[idx].rab_Info.re_EstablishmentTimer;
        // Nitin Change from rab_list_r6 to rab_list_r8
        g2td_info.est_rabs[number_of_rabs].cn_domain = rab_list_r9->elem[idx].rab_Info.cn_DomainIdentity;
    
        g2td_info.est_rabs[number_of_rabs].rab_id_is_gsm_map = TRUE;
  
        rb_list_r9 = &rab_list_r9->elem[idx].rb_InformationSetupList;
      
        tdsrrciho_initialize_rb_list_r8(rb_list_r9, number_of_rabs);
  
        (void)tdsrrc_update_rab_type_in_rab_info_r8(
           &(rab_list_r9->elem[idx]),
           &(g2td_info.est_rabs[number_of_rabs]));
  
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                          g2td_info.est_rabs[number_of_rabs].rab_id,
                          g2td_info.est_rabs[number_of_rabs].cn_domain,
                          g2td_info.est_rabs[number_of_rabs].re_est_timer);
  
        if(rab_list_r9->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          g2td_info.cs_rab_index = number_of_rabs;
          g2td_info.is_cs_rab_present = TRUE;
          if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
          {
            g2td_info.est_rabs[number_of_rabs].rab_type = TDSRRCRB_CS_VOICE_RAB;
          }
        }
        else if(rab_list_r9->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
        {
          g2td_info.is_ps_rab_present = TRUE;
        }

        if(rab_list_r9->elem[idx].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
        {
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
           /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
          if(rab_list_r9->elem[idx].rab_Info.m.nas_Synchronisation_IndicatorPresent)
          {
            g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = 
                tdsrrc_translate_nas_sync_indicator(&(rab_list_r9->elem[idx].rab_Info.nas_Synchronisation_Indicator));
            
            if( g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
            {
              tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator , 0, 0);
              return NULL;
            }
          }
          tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
        }
        else
        {
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
        }

        number_of_rabs++;
      }
      else
      {
        return FAILURE;
      }
      idx++;
    }while((idx < rab_list_r9->n) && (number_of_rabs < MAX_RAB_TO_SETUP));
  }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR," Invalid Release Indicator",0,0,0);
    return FAILURE;
  }

  g2td_info.number_of_est_rabs = number_of_rabs;

  /*set rab included flag to true.  also domain id is always cn domain*/
  g2td_info.rab_info_included = TRUE;

  return SUCCESS;
}
#endif /* FEATURE_LTE_TO_TDSCDMA */

/*===========================================================================

FUNCTION   tdsrrciho_validate_and_extract_srb_rab_info

DESCRIPTION

  Validates and calls functions to extract srb and rab info

DEPENDENCIES

  None.

RETURN VALUE

  Success or Failure.

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_validate_and_extract_srb_rab_info(void)
{
  tdsrrc_SRB_InformationSetupList *srb_list = NULL;
  tdsrrc_RB_InformationSetupList  *rb_list = NULL;
  /* REL-R4 */
  tdsrrc_SRB_InformationSetupList *srb_list_r4 = NULL;
  tdsrrc_RB_InformationSetupList_r4  *rb_list_r4 = NULL;

/*REL-R5 is mainlined*/
  tdsrrc_SRB_InformationSetupList_r5 *srb_list_r5 = NULL;
  tdsrrc_RB_InformationSetupList_r5  *rb_list_r5 = NULL;
  
  tdsrrc_SRB_InformationSetupList_r6 *srb_list_r6 = NULL;
  tdsrrc_RB_InformationSetupList_r6  *rb_list_r6 = NULL;


  tdsrrc_SRB_InformationSetupList_r7 *srb_list_r7 = NULL;
  tdsrrc_RB_InformationSetupList_r7  *rb_list_r7 = NULL;


#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_SRB_InformationSetupList_r8 *srb_list_r8 = NULL;
  tdsrrc_RB_InformationSetupList_r8  *rb_list_r8 = NULL;

#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_SRB_InformationSetupList_r8 *srb_list_r9 = NULL;
  tdsrrc_RB_InformationSetupList_r8  *rb_list_r9 = NULL;

#endif /*FEATURE_TDSCDMA_REL9*/

#ifndef FEATURE_LTE_TO_TDSCDMA
   tdsrrc_RAB_InformationSetupList_r4 *rab_list = NULL;
   tdsrrc_RAB_InformationSetupList_r5 *rab_list_r5 = NULL;
   tdsrrc_RAB_InformationSetupList_r6 *rab_list_r6 = NULL;
   tdsrrc_RAB_InformationSetupList_r7 *rab_list_r7 = NULL;
#ifdef FEATURE_TDSCDMA_REL8
   tdsrrc_RAB_InformationSetupList_r8 *rab_list_r8 = NULL;
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
   tdsrrc_RAB_InformationSetupList_r8 *rab_list_r9 = NULL;
#endif /*FEATURE_TDSCDMA_REL9*/

  boolean rab_info_list_present = FALSE;
#endif /*FEATURE_LTE_TO_TDSCDMA*/

  uint32  rab_identity_choice = T_tdsrrc_RAB_Identity_ansi_41_RAB_Identity;
  uint32 rab_bit_mask = 0;

#ifdef FEATURE_LTE_TO_TDSCDMA
  g2td_info.cs_rab_index = 0;
  g2td_info.is_cs_rab_present = FALSE;
  g2td_info.is_ps_rab_present = FALSE;
#endif /* FEATURE_LTE_TO_TDSCDMA */

  /*if the configuration received is complete then process the struct for
    handover complete */
  if(g2td_info.config_type == IHO_G2TD_COMPLETE_CONFIG)
  {
    if(g2td_info.msg_rel_version == TDSMSG_REL99)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR," Receive Rel3 Message, return failure",0,0,0);
      return FAILURE;
    }
    else if(g2td_info.msg_rel_version == TDSMSG_REL4)
    {
      /* Assign signalling radio bearers list to a local list */
      srb_list_r4 = &g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
        specificationMode.u.complete.srb_InformationSetupList;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
    {
      /* Assign signalling radio bearers list to a local list */
      srb_list_r5 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
        specificationMode.u.complete.srb_InformationSetupList;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL6) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
    {
      srb_list_r6 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r6.handoverToUTRANCommand_r6.specificationMode.u.complete.srb_InformationSetupList;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL7) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
    {
      srb_list_r7 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.r7.handoverToUTRANCommand_r7.specificationMode.u.complete.srb_InformationSetupList;
    }
#ifdef FEATURE_TDSCDMA_REL8
    else if((g2td_info.msg_rel_version == TDSMSG_REL8) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
    {
      srb_list_r8 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.handoverToUTRANCommand_r8.specificationMode.u.complete.srb_InformationSetupList;
    }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
    else if((g2td_info.msg_rel_version == TDSMSG_REL9) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
    {
      srb_list_r9 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.
        handoverToUTRANCommand_r9.specificationMode.u.complete.srb_InformationSetupList;
    }
#endif /* FEATURE_TDSCDMA_REL9 */

    else
    {
      /* Shouldn't reach here - check should have already been made in the caller */
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
      return FAILURE;
    }


    if(g2td_info.msg_rel_version == TDSMSG_REL4)
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
        specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.r4.
              handoverToUTRANCommand_r4.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL4)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

        /* Check if the RAB id is GSM_MAP type. We don't support any
           other type */
        if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
        {
          rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;

          /* GSM_MAP RAB Id. Check if it's valid by calling the
             appropriate NAS entity. First convert it to internal
             format. */

          g2td_info.est_rabs.rab_id =
             tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
                                         specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                         rab_Identity.u.gsm_MAP_RAB_Identity);

          g2td_info.est_rabs.re_est_timer = g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
                                         specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;

          g2td_info.est_rabs.cn_domain = g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
                                         specificationMode.u.complete.rab_InformationSetupList.
                                         elem[0].rab_Info.cn_DomainIdentity;

          rb_list_r4 = &g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
            specificationMode.u.complete.rab_InformationSetupList.elem[0].rb_InformationSetupList;

          tdsrrciho_initialize_rb_list_r4(rb_list_r4);

          rab_list = &g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.specificationMode.u.complete.
           rab_InformationSetupList;


          
          /* in the G-> W handover check the Nas-Sync-indicator for the codec type */
          if(tdsrrciho_update_nas_sync_ind(rab_list->elem[0].rab_Info) == FAILURE)
          {
            tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
            return FAILURE;
          }
         
        }

#endif /* FEATURE_LTE_TO_TDSCDMA */
      }
    }

    else if((g2td_info.msg_rel_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
        r5.handoverToUTRANCommand_r5.specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.
              handoverToUTRANCommand_r5.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL5)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

        /* Check if the RAB id is GSM_MAP type. We don't support any
           other type */
        if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
        {
          rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;

          /* GSM_MAP RAB Id. Check if it's valid by calling the
             appropriate NAS entity. First convert it to internal
             format. */

          g2td_info.est_rabs.rab_id =
             tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
                                         specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                         rab_Identity.u.gsm_MAP_RAB_Identity);

          g2td_info.est_rabs.re_est_timer = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
                                         specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;

          g2td_info.est_rabs.cn_domain = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
                                         specificationMode.u.complete.rab_InformationSetupList.
                                         elem[0].rab_Info.cn_DomainIdentity;

          rb_list_r5 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
            specificationMode.u.complete.rab_InformationSetupList.elem[0].
            rb_InformationSetupList;

          tdsrrciho_initialize_rb_list_r5(rb_list_r5);

          rab_list_r5 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.specificationMode.u.complete.
           rab_InformationSetupList;


          
          /* in the G-> W handover check the Nas-Sync-indicator for the codec type */
          if(tdsrrciho_update_nas_sync_ind(rab_list_r5->elem[0].rab_Info) == FAILURE)
          {
            tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
            return FAILURE;
          }
         
        }

#endif /* FEATURE_LTE_TO_TDSCDMA */
      }
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL6) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.handoverToUTRANCommand_r6.
         specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {

#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.
              handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL6)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

        /* Check if the RAB id is GSM_MAP type. We don't support any
           other type */
        if(T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity == g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6
           .handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.rab_Identity.t)
        {
          rab_identity_choice = T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity;

          /* GSM_MAP RAB Id. Check if it's valid by calling the
             appropriate NAS entity. First convert it to internal
             format. */

          g2td_info.est_rabs.rab_id =
             tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.
                                            handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.
                                            rab_Identity.u.gsm_MAP_RAB_Identity);

          g2td_info.est_rabs.re_est_timer = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
            u.r6.handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.re_EstablishmentTimer;

          g2td_info.est_rabs.cn_domain = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
            u.r6.handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList.elem[0].rab_Info.cn_DomainIdentity;

          rb_list_r6 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
            u.r6.handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList.elem[0].rb_InformationSetupList;

          tdsrrciho_initialize_rb_list_r6(rb_list_r6);

          rab_list_r6 = &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
            criticalExtensions.u.r6.handoverToUTRANCommand_r6.specificationMode.u.complete.rab_InformationSetupList;


          if(rab_list_r6->elem[0].rab_Info.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain )
          {
            g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMR;
             /* in the G-> W handover check the Nas-Sync-indicator for the codec type */        
            if(rab_list_r6->elem[0].rab_Info.m.nas_Synchronisation_IndicatorPresent)
            {
              g2td_info.est_rabs.nas_Synchronisation_Indicator = 
                  tdsrrc_translate_nas_sync_indicator((rab_list_r6->elem[0].rab_Info.nas_Synchronisation_Indicator));
              
              if( g2td_info.est_rabs.nas_Synchronisation_Indicator == TDSCDMA_IVOCODER_ID_NONE )
              {
                tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid configuration .. codec not supported %d",g2td_info.est_rabs.nas_Synchronisation_Indicator , 0, 0);
                return FAILURE;
              }
            }
            tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs.nas_Synchronisation_Indicator);
          }
          else
          {
            tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
          }


          /*lint +e740 */
        }
#endif /* FEATURE_LTE_TO_TDSCDMA */
      }
    }

    else if((g2td_info.msg_rel_version == TDSMSG_REL7) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.handoverToUTRANCommand_r7.
         specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r7.handoverToUTRANCommand_r7.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL7)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

        rab_list_r7 = tdsrrciho_update_rab_info_r7(&rab_identity_choice);

#endif /* FEATURE_LTE_TO_TDSCDMA */
      }
    }
#ifdef FEATURE_TDSCDMA_REL8
    else if((g2td_info.msg_rel_version == TDSMSG_REL8) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.handoverToUTRANCommand_r8.
         specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r8.handoverToUTRANCommand_r8.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL8)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

       rab_list_r8 = tdsrrciho_update_rab_info_r8(&rab_identity_choice);
#endif /*FEATURE_LTE_TO_TDSCDMA */
      }
    }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
    else if((g2td_info.msg_rel_version == TDSMSG_REL9) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
    {
      if(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.
         handoverToUTRANCommand_r9.specificationMode.u.complete.m.rab_InformationSetupListPresent)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if((tdsrrciho_extract_and_fill_in_rab_rb_info((void *) &g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
            u.criticalExtensions.u.criticalExtensions.u.r9.handoverToUTRANCommand_r9.specificationMode.u.complete.rab_InformationSetupList, TDSMSG_REL9)) == FAILURE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
#else
        rab_info_list_present = TRUE;

       rab_list_r9 = tdsrrciho_update_rab_info_r9(&rab_identity_choice);
#endif /*FEATURE_LTE_TO_TDSCDMA */
      }
    }
#endif /* FEATURE_TDSCDMA_REL9 */

/*For NON LTE PSHO*/
#ifndef FEATURE_LTE_TO_TDSCDMA
    /* ASN1 structure 'tdsrrc_RAB_InformationSetup' is validated at this point,
         hence update rab_type in structure 'rbe.rabs.rabs_in_msg[rab_index]'.
    */
    if((rab_list!=NULL) && (rab_list->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r4(
         &(rab_list->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
    else if((rab_list_r5!=NULL) &&(rab_list_r5->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r5(
         &(rab_list_r5->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
    else if((rab_list_r6 !=NULL) &&(rab_list_r6->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r6(
         &(rab_list_r6->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
    else if((rab_list_r7 !=NULL) &&(rab_list_r7->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r7(
         &(rab_list_r7->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
#ifdef FEATURE_TDSCDMA_REL8
    else if((rab_list_r8!=NULL) && (rab_list_r8->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r8(
         &(rab_list_r8->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    else if((rab_list_r9!=NULL) && (rab_list_r9->n != 0))
    {
      (void)tdsrrc_update_rab_type_in_rab_info_r8(
         &(rab_list_r9->elem[0]),
         &(g2td_info.est_rabs));

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
    }
#endif /*FEATURE_TDSCDMA_REL9*/
#endif /* ifndef FEATURE_LTE_TO_TDSCDMA */

  }

  else
  {
    tdsrrcrb_rab_e_type rab_type = TDSRRCRB_UNKNOWN_RAB;      /* Type of RAB - PS, CS-DATA, CS-VOICE */ 
    rrc_rab_id_type rab_id = 0;          /* GSM-MAP RAB Identity                 */
    tdsrrc_Re_EstablishmentTimer re_est_timer;         /* Re-est timer associated with RAB    */

/* Processing for default configs received in HO msg*/
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    g2td_info.rbs_ptr = NULL;
#endif
    g2td_info.sib16_ptr = NULL;

    switch(g2td_info.default_config_id)
    {
      /* Fall through cases that have to processed */
      case TDSRRC_DEF_CONFIG_0:
      case TDSRRC_DEF_CONFIG_1:
      case TDSRRC_DEF_CONFIG_3:
      case TDSRRC_DEF_CONFIG_4:
      case TDSRRC_DEF_CONFIG_7:
      case TDSRRC_DEF_CONFIG_8:
      case TDSRRC_DEF_CONFIG_9:
      case TDSRRC_DEF_CONFIG_10:  
      /* default config 22,15,18,19 is valid only for release 6  */
      case TDSRRC_DEF_CONFIG_15:
      case TDSRRC_DEF_CONFIG_18:
      case TDSRRC_DEF_CONFIG_19:
      case TDSRRC_DEF_CONFIG_22:
      case TDSRRC_DEF_CONFIG_21:
      case TDSRRC_DEF_CONFIG_16:
      { 

        if((g2td_info.default_config_id == TDSRRC_DEF_CONFIG_0 ) ||
           (g2td_info.default_config_id == TDSRRC_DEF_CONFIG_1 ) 
           ||(g2td_info.default_config_id == TDSRRC_DEF_CONFIG_22 )
          )
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"G->TD Signalling only default config Id %d chosen",g2td_info.default_config_id,0,0);
        }
        /*first get the sib16 pointer.. currently only supporting default configuration.  will support
          real sib's soon*/
        g2td_info.sib16_ptr =
          (tdsrrc_SysInfoType16 *)
        tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[g2td_info.default_config_id].default_config_ptr,
                            tdsrrc_SysInfoType16_PDU,
                            tdsdefault_config_info[g2td_info.default_config_id].default_config_len
                            );
    
        /*check for null case*/
        if(g2td_info.sib16_ptr == NULL)
        {
          ERR_FATAL("No memory for SIB16 ptr",0,0,0);
        }
        else
        {
          g2td_info.def_config_ptr_present = TRUE;
        }
    
        /*initialize signalling radio bearer list*/
        srb_list = &g2td_info.sib16_ptr->preDefinedRadioConfiguration.
                                   predefinedRB_Configuration.srb_InformationList;
      
        rb_list = &g2td_info.sib16_ptr->preDefinedRadioConfiguration.
                                  predefinedRB_Configuration.rb_InformationList;
      }
      break;
  
  #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      case TDSRRC_DEF_CONFIG_11:
      case TDSRRC_DEF_CONFIG_12:
      case TDSRRC_DEF_CONFIG_13:
      case TDSRRC_DEF_CONFIG_14:
      {
        if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Default Configuration %d received",g2td_info.default_config_id,0,0);
          g2td_info.rbs_ptr =
          (tdsrrc_DL_DCCH_Message *)
          tdsrrcasn1_decode_pdu( (void *)tdsdefault_config_info[g2td_info.default_config_id].default_config_ptr,
                            tdsrrc_DL_DCCH_Message_PDU,
                            tdsdefault_config_info[g2td_info.default_config_id].default_config_len
                            );
    
          /*check for null case*/
          if(g2td_info.rbs_ptr == NULL)
          {
            ERR_FATAL("No memory for RBS ptr",0,0,0);
          }
          else
          {
            g2td_info.def_config_ptr_present = TRUE;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Decoded DEFAULT CONFIG %d Successfully from internal RB setup message", g2td_info.default_config_id ,0,0);
          }
    
          /*initialize signalling radio bearer list*/
          srb_list_r5 = &g2td_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
            criticalExtensions.u.r5.radioBearerSetup_r5.srb_InformationSetupList;
        
          rb_list_r5 = &g2td_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
            criticalExtensions.u.r5.radioBearerSetup_r5.rab_InformationSetupList.elem[0].rb_InformationSetupList;   
        }
      }
      break;
  #endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */ 
  
  
      default:
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported Def Config ID %d", g2td_info.default_config_id,0,0);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        return FAILURE;
    }

    

    if(g2td_info.msg_rel_version == TDSMSG_REL99)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR," Receive Rel3 Message, ignore",0,0,0);
    }
    else if(g2td_info.msg_rel_version == TDSMSG_REL4)
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.specificationMode.u.
       preconfiguration.m.rab_InfoPresent;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.specificationMode.u.
       preconfiguration.m.rab_InfoPresent;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL6) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.handoverToUTRANCommand_r6.
        specificationMode.u.preconfiguration.m.rab_InfoPresent;
    }
    else if((g2td_info.msg_rel_version == TDSMSG_REL7) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.handoverToUTRANCommand_r7.
        specificationMode.u.preconfiguration.m.rab_InfoPresent;
    }
#ifdef FEATURE_TDSCDMA_REL8
    else if((g2td_info.msg_rel_version == TDSMSG_REL8) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.handoverToUTRANCommand_r8.
        specificationMode.u.preconfiguration.m.rab_InfoPresent;
    }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
    else if((g2td_info.msg_rel_version == TDSMSG_REL9) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
    {
      rab_bit_mask = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.
        handoverToUTRANCommand_r9.specificationMode.u.preconfiguration.m.rab_InfoPresent;
    }
#endif /* FEATURE_TDSCDMA_REL9 */


    /*if rab information is present then initialize rab information*/
    if(rab_bit_mask)
    {
#ifndef FEATURE_LTE_TO_TDSCDMA
      rab_info_list_present = TRUE;
#endif

      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */

      if(g2td_info.msg_rel_version == TDSMSG_REL99)
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR," Receive Rel3 Message, ignore",0,0,0);
      }
      else if(g2td_info.msg_rel_version == TDSMSG_REL4)
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.specificationMode.u.
           preconfiguration.rab_Info.rab_Identity.t;
   
        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
                       specificationMode.u.preconfiguration.rab_Info)
  #ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
       ) == FAILURE)
      
        {
           tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
           return FAILURE;
        }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        if( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
#endif
      
      
      }

      else if((g2td_info.msg_rel_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.specificationMode.u.
           preconfiguration.rab_Info.rab_Identity.t;
   
        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
                       specificationMode.u.preconfiguration.rab_Info)
#ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
 ) == FAILURE)

        {
           tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
           return FAILURE;
        }
     
     
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        if( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
#endif


      }

      else if((g2td_info.msg_rel_version == TDSMSG_REL6) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.
          handoverToUTRANCommand_r6.specificationMode.u.preconfiguration.rab_Info.rab_Identity.t;

        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.
            handoverToUTRANCommand_r6.specificationMode.u.preconfiguration.rab_Info)
#ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
             ) == FAILURE)
        {
          return FAILURE;
        }
         
        if( (g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_16) ||
            ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_21)
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        || ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
#endif
         )
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
      }
      else if((g2td_info.msg_rel_version == TDSMSG_REL7) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.
          handoverToUTRANCommand_r7.specificationMode.u.preconfiguration.rab_Info.rab_Identity.t;

        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r7.
            handoverToUTRANCommand_r7.specificationMode.u.preconfiguration.rab_Info)
#ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
            ) == FAILURE)
        {
          return FAILURE;
        }
         
        if( (g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_16) ||
            ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_21)
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        || ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
#endif
         )
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
      }

#ifdef FEATURE_TDSCDMA_REL8
      else if((g2td_info.msg_rel_version == TDSMSG_REL8) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
          handoverToUTRANCommand_r8.specificationMode.u.preconfiguration.rab_Info.rab_Identity.t;

        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.
            handoverToUTRANCommand_r8.specificationMode.u.preconfiguration.rab_Info)
#ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
            ) == FAILURE)
        {
          return FAILURE;
        }
         
        if( (g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_16) ||
            ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_21)
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        || ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
#endif
         )
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
      }
#endif /* FEATURE_TDSCDMA_REL8 */

#ifdef FEATURE_TDSCDMA_REL9
      else if((g2td_info.msg_rel_version == TDSMSG_REL9) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
      {
        rab_identity_choice = g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
          criticalExtensions.u.criticalExtensions.u.r9.handoverToUTRANCommand_r9.specificationMode.u.preconfiguration.rab_Info.rab_Identity.t;

        if( tdsrrciho_update_def_config_nas_sync_ind(
            &(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
            criticalExtensions.u.r9.handoverToUTRANCommand_r9.specificationMode.u.preconfiguration.rab_Info)
#ifdef FEATURE_LTE_TO_TDSCDMA
            , g2td_info.cs_rab_index
#endif /* FEATURE_LTE_TO_TDSCDMA */
            ) == FAILURE)
        {
          return FAILURE;
        }
         
        if( (g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_16) ||
            ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_21)
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        || ( g2td_info.default_config_id ==  TDSRRC_DEF_CONFIG_13)
#endif
         )
        {
#ifndef FEATURE_LTE_TO_TDSCDMA
          g2td_info.est_rabs.nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#else
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = TDSCDMA_IVOCODER_ID_AMRWB;
#endif
          tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_AMRWB);
        }
      }
#endif /* FEATURE_TDSCDMA_REL9 */


      /* Check if the RAB id is GSM_MAP type. We don't support any
         other type */
      if (rab_identity_choice == T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
      {
        /* GSM_MAP RAB Id. Check if it's valid by calling the
         appropriate NAS entity. First convert it to internal
         format. */

        if(g2td_info.msg_rel_version == TDSMSG_REL99)
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR," Receive Rel3 Message, ignore",0,0,0);
        }
        else if(g2td_info.msg_rel_version == TDSMSG_REL4)
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4.
                                         specificationMode.u.preconfiguration.rab_Info.
                                         rab_Identity.u.gsm_MAP_RAB_Identity);
        }
        else if((g2td_info.msg_rel_version == TDSMSG_REL5) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5.
                                         specificationMode.u.preconfiguration.rab_Info.
                                         rab_Identity.u.gsm_MAP_RAB_Identity);
        }
        else if((g2td_info.msg_rel_version == TDSMSG_REL6) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.
                                            criticalExtensions.u.r6.handoverToUTRANCommand_r6.specificationMode.u.
                                            preconfiguration.rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
        }
        else if((g2td_info.msg_rel_version == TDSMSG_REL7) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                            criticalExtensions.u.r7.handoverToUTRANCommand_r7.specificationMode.u.
                                            preconfiguration.rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
        }
#ifdef FEATURE_TDSCDMA_REL8
        else if((g2td_info.msg_rel_version == TDSMSG_REL8) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                            criticalExtensions.u.r8.handoverToUTRANCommand_r8.specificationMode.u.
                                            preconfiguration.rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
        }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
        else if((g2td_info.msg_rel_version == TDSMSG_REL9) && (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
        {
          /*g2td_info.est_rabs.rab_id =*/
             rab_id = tdsrrcrb_translate_gsm_map_rab_id(g2td_info.ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                            criticalExtensions.u.criticalExtensions.u.r9.handoverToUTRANCommand_r9.specificationMode.u.
                                            preconfiguration.rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
        }
#endif /* FEATURE_TDSCDMA_REL9 */

        else
        {
          /* Shouldn't reach here - check should have already been made in the caller */
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
/*g2td_info.est_rabs.re_est_timer = tdsrrc_Re_EstablishmentTimer_useT314;*/
        re_est_timer = tdsrrc_Re_EstablishmentTimer_useT314;

/*tdsrrciho_initialize_rb_list for r3~r8 has been 
changed location by later WCDMA code*/


        /* Currently we support def-config 0,1,3,4,7,8,9..  0 & 1 is for sig only, 3 for voice.. so update
           est_rabs info for 3 only..  this is done here on a hard-coded basis, since the
           rab info is not a composite struct as we get in rb_setup/ho_complete_config
           right now anything other than 3 is set to UNKNOWN_RAB, when we support CS-DATA,
           we need to update the rab_type based on def-config-info
        */
        switch(g2td_info.default_config_id)
        {
        case TDSRRC_DEF_CONFIG_3:
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
        case TDSRRC_DEF_CONFIG_11:
        case TDSRRC_DEF_CONFIG_12:
        case TDSRRC_DEF_CONFIG_14:
        case  TDSRRC_DEF_CONFIG_13:
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

        case TDSRRC_DEF_CONFIG_21:
        case TDSRRC_DEF_CONFIG_16:

        case TDSRRC_DEF_CONFIG_10:
        case TDSRRC_DEF_CONFIG_15:
        case TDSRRC_DEF_CONFIG_18:
        case TDSRRC_DEF_CONFIG_19:
          rab_type = TDSRRCRB_CS_VOICE_RAB;
          break;

        case TDSRRC_DEF_CONFIG_0:
        case TDSRRC_DEF_CONFIG_1:
        case TDSRRC_DEF_CONFIG_22:

         rab_type = TDSRRCRB_UNKNOWN_RAB;
          break;

        case TDSRRC_DEF_CONFIG_4:
        case TDSRRC_DEF_CONFIG_7:
        case TDSRRC_DEF_CONFIG_8:
        case TDSRRC_DEF_CONFIG_9:      
          rab_type = TDSRRCRB_CS_DATA_RAB;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported Def Config ID %d", g2td_info.default_config_id,0,0);
          break;
        }

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received default config -> %d: RAB type -> %d",
                      g2td_info.default_config_id, rab_type,0);

/*        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,Domain:%d,re_est_tmr:%d",
                        g2td_info.est_rabs.rab_id,
                        g2td_info.est_rabs.cn_domain,
                        g2td_info.est_rabs.re_est_timer);
*/
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB_ID:%d,re_est_tmr:%d",
                        rab_id,
                        re_est_timer,
                        0);

#ifndef FEATURE_LTE_TO_TDSCDMA

        g2td_info.est_rabs.rab_id = rab_id;
        g2td_info.est_rabs.re_est_timer = re_est_timer;
        g2td_info.est_rabs.rab_type = rab_type;

        if (rb_list != NULL) 
        {
          tdsrrciho_initialize_rb_list(rb_list);
        }
        /* Rel-4 */
        else if (rb_list_r4!= NULL) 
        {
          tdsrrciho_initialize_rb_list_r4(rb_list_r4);
        }
        else if (rb_list_r5!= NULL) 
        {
          tdsrrciho_initialize_rb_list_r5(rb_list_r5);
        }
        else if (rb_list_r6 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r6(rb_list_r6);
        }
        else if (rb_list_r7 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r7(rb_list_r7);
        }
#ifdef FEATURE_TDSCDMA_REL8
        else if (rb_list_r8 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r8(rb_list_r8);
        }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        else if (rb_list_r9 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r8(rb_list_r9);
        }
#endif /*FEATURE_TDSCDMA_REL9*/

#else
        /* As per spec, for PS handover complete configuration is given by the UTRAN
         * In future this code needs to be extended to support PS Handover */

        g2td_info.is_cs_rab_present = TRUE;
        g2td_info.est_rabs[g2td_info.cs_rab_index].rab_id = rab_id;
        g2td_info.est_rabs[g2td_info.cs_rab_index].re_est_timer = re_est_timer;
        g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type = rab_type;

        g2td_info.rab_info_included = TRUE;
        g2td_info.est_rabs[g2td_info.cs_rab_index].cn_domain = RRC_CS_DOMAIN_CN_ID;

        /* Store this info locally so that ESTABLISHED_RABS can
           be updated later, when these RABs are established */
        g2td_info.est_rabs[g2td_info.cs_rab_index].rab_id_is_gsm_map = TRUE;

        g2td_info.number_of_est_rabs = 1;

        if (rb_list != NULL) 
        {
          tdsrrciho_initialize_rb_list(rb_list, g2td_info.cs_rab_index);
        }
        else if (rb_list_r4 != NULL) 
        {
          tdsrrciho_initialize_rb_list((struct tdsrrc_RB_InformationSetupList *)rb_list_r4, g2td_info.cs_rab_index);
        }
        else if (rb_list_r5!= NULL) 
        {
          tdsrrciho_initialize_rb_list_r5(rb_list_r5, g2td_info.cs_rab_index);
        }
        else if (rb_list_r6 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r6(rb_list_r6, g2td_info.cs_rab_index);
        }
        else if (rb_list_r7 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r7(rb_list_r7, g2td_info.cs_rab_index);
        }
#ifdef FEATURE_TDSCDMA_REL8
        else if (rb_list_r8 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r8(rb_list_r8, g2td_info.cs_rab_index);
        }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        else if (rb_list_r9 != NULL) 
        {
          tdsrrciho_initialize_rb_list_r8(rb_list_r9, g2td_info.cs_rab_index);
        }
#endif /*FEATURE_TDSCDMA_REL9*/


#endif /* FEATURE_LTE_TO_TDSCDMA */
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB ID is not GSM_MAP!", 0, 0, 0);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        return FAILURE;
      }
    }
  }

  if(NULL != srb_list)
  {
    tdsrrciho_initialize_srb_list(srb_list);
  }
  else if(NULL != srb_list_r4)
  {
    tdsrrciho_initialize_srb_list(srb_list_r4);
  }
  else if (NULL != srb_list_r5) 
  {
    tdsrrciho_initialize_srb_list_r5(srb_list_r5);
  }
  else if (NULL != srb_list_r6) 
  {
    tdsrrciho_initialize_srb_list_r6(srb_list_r6);
  }
  else if (NULL != srb_list_r7) 
  {
    tdsrrciho_initialize_srb_list_r7(srb_list_r7);
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (NULL != srb_list_r8) 
  {
    tdsrrciho_initialize_srb_list_r8(srb_list_r8);
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if (NULL != srb_list_r9) 
  {
    tdsrrciho_initialize_srb_list_r8(srb_list_r9);
  }
#endif /*FEATURE_TDSCDMA_REL9*/

  else
  {
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
    return FAILURE;
  }

#ifndef FEATURE_LTE_TO_TDSCDMA
  if(rab_info_list_present == TRUE)
  {
    if (rab_identity_choice != T_tdsrrc_RAB_Identity_gsm_MAP_RAB_Identity)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB ID is not GSM_MAP!", 0, 0, 0);
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
      return FAILURE;
    }

    /*set rab included flag to true.  also domain id is always cn domain*/
    g2td_info.rab_info_included = TRUE;
    g2td_info.est_rabs.cn_domain = RRC_CS_DOMAIN_CN_ID;

    /* Store this info locally so that ESTABLISHED_RABS can
       be updated later, when these RABs are established */
    g2td_info.est_rabs.rab_id_is_gsm_map = TRUE;     
  }

#else
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CS RAB Present: %d, CS RAB Index : %d",g2td_info.is_cs_rab_present, g2td_info.cs_rab_index,0);
#endif /* FEATURE_LTE_TO_TDSCDMA */

  return SUCCESS;
}/*tdsrrciho_validate_and_extract_srb_rab_info*/

/*===========================================================================

FUNCTION   tdsrrciho_verify_and_extract_handover_to_utran_command

DESCRIPTION

  This function verifies the handover to UTRAN command.  If an error is
  detected sends a failure. If the command is valid, it extracts necessary
  information from the command.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_verify_and_extract_handover_to_utran_command
(
  tdsrrc_HandoverToUTRANCommand *ho_ptr     /* Pointer to the HO message */
)
{
  boolean ciphering_algo_present = FALSE;
  tdsrrc_CipheringAlgorithm ciphering_algo = tdsrrc_CipheringAlgorithm_uea0;
  rrciho_g2td_config_e_type config_type = IHO_G2TD_PREDEF_CONFIG;
  tdsrrc_DL_InformationPerRL_List_r4 *dl_info_per_rl_r4 = NULL;
  tdsrrc_DL_InformationPerRL_PostTDD_LCR_r4 *dl_info_per_rl_r4_LCR = NULL;
  tdsrrc_DL_InformationPerRL_List_r5 *dl_info_per_rl_r5 = NULL;
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_r6 = NULL;
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_r7 = NULL;

  tdsrrc_DefaultConfigMode default_config_mode = tdsrrc_DefaultConfigMode_fdd;
  tdsrrc_cpid_type cpid_code_type;
  tdsrrc_FrequencyInfoTDD * freq_info =NULL;
  tdsrrc_FrequencyInfoTDD * w_freq_info =NULL;
  uint32 default_config_id = TDSRRC_DEF_CONFIG_INVALID;
  tdsrrc_CipheringAlgorithm_r7 ciphering_algo_r7 = tdsrrc_CipheringAlgorithm_r7_uea0;
  g2td_info.msg_rel_version = TDSMSG_INVALID;

  memset((void *)&cpid_code_type, 0, sizeof(tdsrrc_cpid_type));

/*FDD->TDD update TBD*/
  if(ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_r3)
  {
    tdsrrc_print_supported_asn1_rel_version();
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_TDS_PROTOCOL_ERROR;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unsupported message version ... rejecting ...", 0, 0, 0);
    return FAILURE;
  }
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
            T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_6_r4))
  {
    tdsrrc_HandoverToUTRANCommand_r4_IEs * ho_to_utran_ptr_r4;
    g2td_info.msg_rel_version = TDSMSG_REL4;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO HandoverToUtran - Rel4 message received", 0, 0, 0);
    ho_to_utran_ptr_r4 = &ho_ptr->u.criticalExtensions.u.r4.handoverToUTRANCommand_r4;


    /*Check if Ciphering algo is present and validate the same*/
    if(ho_to_utran_ptr_r4->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo = ho_to_utran_ptr_r4->cipheringAlgorithm;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel4: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

    /*Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r4->specificationMode.t == 
        T_tdsrrc_HandoverToUTRANCommand_r4_IEs_specificationMode_complete)
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel4: complete configuration received", 0, 0, 0);

      dl_info_per_rl_r4 = &ho_to_utran_ptr_r4->specificationMode.u.complete.dl_InformationPerRL_List;

      if((dl_info_per_rl_r4->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd) &&
          (dl_info_per_rl_r4->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r4->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the cell parameter id */
          cpid_code_type = dl_info_per_rl_r4->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;

          freq_info = ho_to_utran_ptr_r4->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;

          /* second freqency info */
          if (TRUE == ho_ptr->u.criticalExtensions.u.r4.m.laterNonCriticalExtensionsPresent)
          {
            if (TRUE == 
              ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.
              m.frequencyInfoPresent)
            {
              if (T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd == 
                ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.
                frequencyInfo.modeSpecificInfo.t)
              {
                w_freq_info = 
                  ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.
                  frequencyInfo.modeSpecificInfo.u.tdd;
              }
            }
          }

          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
      }
    }
    else if(ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r4_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel4: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.modeSpecificInfo.t ==
            T_tdsrrc_HandoverToUTRANCommand_r4_IEs_modeSpecificInfo_1_tdd)
        {
          if(ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
            T_tdsrrc_HandoverToUTRANCommand_r4_IEs_tdd_tdd128)
          {
            /*lint -e740 */
            /* type-casting to R3 type as we are interested to see if this list has only one RL */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;

          /* second freqency info */
          if (TRUE == 
            ho_ptr->u.criticalExtensions.u.r4.m.laterNonCriticalExtensionsPresent)
          {
            if (TRUE == 
              ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.m.frequencyInfoPresent)
            {
              if (T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd == 
                ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.frequencyInfo.modeSpecificInfo.t)
              {
                w_freq_info = 
                  ho_ptr->u.criticalExtensions.u.r4.laterNonCriticalExtensions.handoverToUTRANCommand_r4_TDD128_ext.frequencyInfo.modeSpecificInfo.u.tdd;
              }
            }
          }

          }
          else
          {
            /*default_config_mode = tdsrrc_DefaultConfigMode_fdd;*/
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r4->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r4_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
            T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_criticalExtensions)&&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.t == 
            T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_r5) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5))
  {
    tdsrrc_HandoverToUTRANCommand_r5_IEs * ho_to_utran_ptr_r5;
    g2td_info.msg_rel_version = TDSMSG_REL5;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO HandoverToUtran - Rel5 message received", 0, 0, 0);
    ho_to_utran_ptr_r5 = &ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.handoverToUTRANCommand_r5;


    if(ho_to_utran_ptr_r5->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo = ho_to_utran_ptr_r5->cipheringAlgorithm;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel5: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

    /* Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r5->specificationMode.t == 
       T_tdsrrc_HandoverToUTRANCommand_r5_IEs_specificationMode_complete)
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - complete configuration received", 0, 0, 0);

      /*lint -e740 */
      /* type-casting to R3 type as we are interested to see if this list has only one RL */
      dl_info_per_rl_r5 = &ho_to_utran_ptr_r5->specificationMode.u.complete.dl_InformationPerRL_List;
      /*lint +e740 */

      if((dl_info_per_rl_r5->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd) &&
          (dl_info_per_rl_r5->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r5->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the primary scrambling code */
          cpid_code_type = dl_info_per_rl_r5->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          freq_info = ho_to_utran_ptr_r5->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;

          /* second freqency info */
          if (TRUE == 
            ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.m.laterNonCriticalExtensionsPresent)
          {
            if (TRUE == 
              ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.m.frequencyInfoPresent)
            {
              if (T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd == 
                ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.frequencyInfo.modeSpecificInfo.t)
              {
                w_freq_info = 
                  ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.frequencyInfo.modeSpecificInfo.u.tdd;
              }
            }
          }

          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.preConfigMode.t == 
              T_tdsrrc_HandoverToUTRANCommand_r5_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel5: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.modeSpecificInfo.t == 
              T_tdsrrc_HandoverToUTRANCommand_r5_IEs_modeSpecificInfo_1_tdd)
        {
          if(ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
            T_tdsrrc_HandoverToUTRANCommand_r5_IEs_tdd_tdd128)
          {
            /* The R5 downlink info per radio link is type-casted to R3 type as we are interested to see if this list has only one RL */
            /*lint -e740 */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;

          /* second freqency info */
          if (TRUE == 
            ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.m.laterNonCriticalExtensionsPresent)
          {
            if (TRUE == 
              ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.m.frequencyInfoPresent)
            {
              if (T_tdsrrc_FrequencyInfo_modeSpecificInfo_tdd == 
                ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.frequencyInfo.modeSpecificInfo.t)
              {
                w_freq_info = 
                  ho_ptr->u.criticalExtensions.u.criticalExtensions.u.r5.laterNonCriticalExtensions.handoverToUTRANCommand_r5_TDD128_ext.frequencyInfo.modeSpecificInfo.u.tdd;
              }
            }
          }

          }
          else
          {
            /*default_config_mode = tdsrrc_DefaultConfigMode_fdd;*/
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Default config info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r5->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r5_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_4_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_4_r6) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6))
  {
    tdsrrc_HandoverToUTRANCommand_r6_IEs * ho_to_utran_ptr_r6;
    g2td_info.msg_rel_version = TDSMSG_REL6;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO HandoverToUtran - Rel6 message received", 0, 0, 0);
    ho_to_utran_ptr_r6 = &ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r6.handoverToUTRANCommand_r6;


    if(ho_to_utran_ptr_r6->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo = ho_to_utran_ptr_r6->cipheringAlgorithm;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel6: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

    /* Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r6->specificationMode.t == 
       T_tdsrrc_HandoverToUTRANCommand_r6_IEs_specificationMode_complete )
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - complete configuration received", 0, 0, 0);

      dl_info_per_rl_r6 = &ho_to_utran_ptr_r6->specificationMode.u.complete.dl_InformationPerRL_List;

      if((dl_info_per_rl_r6->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd) &&
          (dl_info_per_rl_r6->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r6->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the primary scrambling code */
          cpid_code_type = dl_info_per_rl_r6->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          freq_info = ho_to_utran_ptr_r6->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;
          /* R6 no second frequency info */
          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.t == 
            T_tdsrrc_HandoverToUTRANCommand_r6_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel6: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.modeSpecificInfo.t == 
             T_tdsrrc_HandoverToUTRANCommand_r6_IEs_modeSpecificInfo_tdd)
        {
          if(ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
            T_tdsrrc_HandoverToUTRANCommand_r6_IEs_tdd_tdd128)
          {
            /* The R5 downlink info per radio link is type-casted to R3 type as we are interested to see if this list has only one RL */
            /*lint -e740 */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;
            /* R6 no second frequency info */
          }
          else
          {
            /* default_config_mode = tdsrrc_DefaultConfigMode_fdd;*/
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Default config info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r6->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r6_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_4_criticalExtensions) &&
            (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_3_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_3_r7) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))
  {
    tdsrrc_HandoverToUTRANCommand_r7_IEs * ho_to_utran_ptr_r7;
    g2td_info.msg_rel_version = TDSMSG_REL7;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO HandoverToUTRANCommand - Rel7 message received", 0, 0, 0);
    ho_to_utran_ptr_r7 = &ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        r7.handoverToUTRANCommand_r7;


    if(ho_to_utran_ptr_r7->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo_r7 = ho_to_utran_ptr_r7->cipheringAlgorithm;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel7: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
   /*In R7 IHO, IE "supportForChangeOfUE_Capability" mandatory. We need to store it in
        "rrc_ordered_rnc_cap_change_support" so that later once the procedure is over successfully, global
        "rrc_rnc_cap_change_support" can be updated accordingly. */  
    if(ho_to_utran_ptr_r7->supportForChangeOfUE_Capability == TRUE)
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_SUPPORTED;
    }
    else
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_UNSUPPORTED;
    }

#endif
    /* Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r7->specificationMode.t == 
       T_tdsrrc_HandoverToUTRANCommand_r7_IEs_specificationMode_complete)
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - complete configuration received", 0, 0, 0);

      dl_info_per_rl_r7 =&ho_to_utran_ptr_r7->specificationMode.u.complete.dl_InformationPerRL_List;

      if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd) &&
            (dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the primary scrambling code */
          cpid_code_type = dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          freq_info = ho_to_utran_ptr_r7->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;

          /* second frequency info */
          if (TRUE == 
            ho_to_utran_ptr_r7->specificationMode.u.complete.m.multi_frequencyInfoPresent)
          {
            if (TRUE == 
              ho_to_utran_ptr_r7->specificationMode.u.complete.multi_frequencyInfo.m.secondFrequencyInfoPresent)
            {
              w_freq_info = 
                &ho_to_utran_ptr_r7->specificationMode.u.complete.multi_frequencyInfo.secondFrequencyInfo;
            }
          }

          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.t == 
            T_tdsrrc_HandoverToUTRANCommand_r7_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel7: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.t
            == T_tdsrrc_HandoverToUTRANCommand_r7_IEs_modeSpecificInfo_tdd)
        {
          if(ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
              T_tdsrrc_HandoverToUTRANCommand_r7_IEs_tdd_tdd128)
          {
            /* The R5 downlink info per radio link is type-casted to R3 type as we are interested to see if this list has only one RL */
            /*lint -e740 */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;

            /* second frequency info */
            if (TRUE == 
              ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.m.multi_frequencyInfoPresent)
            {
              if (TRUE == 
                ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.multi_frequencyInfo.m.secondFrequencyInfoPresent)
              {
                w_freq_info = 
                  &ho_to_utran_ptr_r7->specificationMode.u.complete.multi_frequencyInfo.secondFrequencyInfo;
              }
            }

          }
          else
          {
            /*default_config_mode = tdsrrc_DefaultConfigMode_fdd;*/
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Default config info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r7->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r7_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_4_criticalExtensions) &&
            (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_3_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_2_criticalExtensions) &&
           (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_2_r8) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8))
  {
    tdsrrc_HandoverToUTRANCommand_r8_IEs * ho_to_utran_ptr_r8;
    g2td_info.msg_rel_version = TDSMSG_REL8;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel8 message received", 0, 0, 0);
    ho_to_utran_ptr_r8 = &ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r8.handoverToUTRANCommand_r8;


    if(ho_to_utran_ptr_r8->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo_r7 = ho_to_utran_ptr_r8->cipheringAlgorithm;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel8: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
   /*In R7 IHO, IE "supportForChangeOfUE_Capability" mandatory. We need to store it in
        "rrc_ordered_rnc_cap_change_support" so that later once the procedure is over successfully, global
        "rrc_rnc_cap_change_support" can be updated accordingly. */  
    if(ho_to_utran_ptr_r8->supportForChangeOfUE_Capability == TRUE)
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_SUPPORTED;
    }
    else
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_UNSUPPORTED;
    }

#endif
    /* Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r8->specificationMode.t == 
       T_tdsrrc_HandoverToUTRANCommand_r8_IEs_specificationMode_complete)
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - complete configuration received", 0, 0, 0);

      dl_info_per_rl_r7 =&ho_to_utran_ptr_r8->specificationMode.u.complete.dl_InformationPerRL_List;

      if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd) &&
            (dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the primary scrambling code */
          cpid_code_type = dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          freq_info = ho_to_utran_ptr_r8->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;

          /* second frequency info */
          if (TRUE == 
            ho_to_utran_ptr_r8->specificationMode.u.complete.m.multi_frequencyInfoPresent)
          {
            if (TRUE == 
              ho_to_utran_ptr_r8->specificationMode.u.complete.multi_frequencyInfo.m.secondFrequencyInfoPresent)
            {
              w_freq_info = 
                &ho_to_utran_ptr_r8->specificationMode.u.complete.multi_frequencyInfo.secondFrequencyInfo;
            }
          }

          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.t == 
            T_tdsrrc_HandoverToUTRANCommand_r8_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel8: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.t == 
            T_tdsrrc_HandoverToUTRANCommand_r8_IEs_modeSpecificInfo_tdd)
        {
          if(ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
              T_tdsrrc_HandoverToUTRANCommand_r8_IEs_tdd_tdd128)
          {
            /* The R5 downlink info per radio link is type-casted to R3 type as we are interested to see if this list has only one RL */
            /*lint -e740 */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;

            /* second frequency info */
            if (TRUE == 
              ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.m.multi_frequencyInfoPresent)
            {
              if (TRUE == 
                ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.multi_frequencyInfo.m.secondFrequencyInfoPresent)
              {
                w_freq_info = 
                  &ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.multi_frequencyInfo.secondFrequencyInfo;
              }
            }

          }
          else
          {
            default_config_mode = tdsrrc_DefaultConfigMode_fdd;
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Default config info is for FDD mode ... rejecting ...", 0, 0, 0);
      }
    }
    else if(ho_to_utran_ptr_r8->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r8_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  else if((ho_ptr->t == T_tdsrrc_HandoverToUTRANCommand_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_5_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_4_criticalExtensions) &&
            (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_3_criticalExtensions) &&
          (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_2_criticalExtensions) &&
           (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_2_criticalExtensions) &&
           (ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t == 
           T_tdsrrc_HandoverToUTRANCommand_criticalExtensions_1_r9) &&
          (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9))
  {
    tdsrrc_HandoverToUTRANCommand_r9_IEs * ho_to_utran_ptr_r9;
    g2td_info.msg_rel_version = TDSMSG_REL9;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel9 message received", 0, 0, 0);
    ho_to_utran_ptr_r9 = &ho_ptr->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.r9.handoverToUTRANCommand_r9;


    if(ho_to_utran_ptr_r9->m.cipheringAlgorithmPresent)
    {
      ciphering_algo_present = TRUE;
      /*Copy Ciphering Algo to local struct*/
      ciphering_algo_r7 = ho_to_utran_ptr_r9->cipheringAlgorithm;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel9: ciphering algorithm -%d signaled", ciphering_algo, 0, 0);
    }

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
   /*In R7 IHO, IE "supportForChangeOfUE_Capability" mandatory. We need to store it in
        "rrc_ordered_rnc_cap_change_support" so that later once the procedure is over successfully, global
        "rrc_rnc_cap_change_support" can be updated accordingly. */  
    if(ho_to_utran_ptr_r9->supportForChangeOfUE_Capability == TRUE)
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_SUPPORTED;
    }
    else
    {
      rrc_ordered_rnc_cap_change_support = TDSRNC_CAP_CHANGE_UNSUPPORTED;
    }

#endif
    /* Check if Complete Configuration is sent and set that t in the
      procedure data*/
    if(ho_to_utran_ptr_r9->specificationMode.t == 
       T_tdsrrc_HandoverToUTRANCommand_r9_IEs_specificationMode_complete)
    {
      config_type = IHO_G2TD_COMPLETE_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - complete configuration received", 0, 0, 0);

      dl_info_per_rl_r7 =&ho_to_utran_ptr_r9->specificationMode.u.complete.dl_InformationPerRL_List;

      if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.t == T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd) &&
            (dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->t == T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
      {
        if((dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent))
        {
          /* get the primary scrambling code */
          cpid_code_type = dl_info_per_rl_r7->elem[0].modeSpecificInfo.u.tdd->u.tdd->cellParametersID;
          freq_info = ho_to_utran_ptr_r9->specificationMode.u.complete.frequencyInfo.modeSpecificInfo.u.tdd;

          /* second frequency info */
          if (TRUE == 
            ho_to_utran_ptr_r9->specificationMode.u.complete.m.multi_frequencyInfoPresent)
          {
            if (TRUE == 
              ho_to_utran_ptr_r9->specificationMode.u.complete.multi_frequencyInfo.m.secondFrequencyInfoPresent)
            {
              w_freq_info = 
                &ho_to_utran_ptr_r9->specificationMode.u.complete.multi_frequencyInfo.secondFrequencyInfo;
            }
          }

          default_config_mode = tdsrrc_DefaultConfigMode_tdd;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info have no CPID ", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.t == 
            T_tdsrrc_HandoverToUTRANCommand_r8_IEs_preConfigMode_defaultConfig)
    {
      config_type = IHO_G2TD_DEFAULT_CONFIG;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO - Rel9: default configuration received", 0, 0, 0);

      default_config_mode = ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigMode;

      if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
      {
        default_config_id = ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.u.defaultConfig.defaultConfigIdentity;

        if(ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.t == 
            T_tdsrrc_HandoverToUTRANCommand_r9_IEs_modeSpecificInfo_tdd)
        {
          if(ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.t ==
              T_tdsrrc_HandoverToUTRANCommand_r9_IEs_tdd_tdd128)
          {
            /* The R5 downlink info per radio link is type-casted to R3 type as we are interested to see if this list has only one RL */
            /*lint -e740 */
            dl_info_per_rl_r4_LCR = &ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.dl_InformationPerRL;
            /*lint +e740 */

            /* get the primary scrambling code */
            cpid_code_type = dl_info_per_rl_r4_LCR->primaryCCPCH_Info.cellParametersID;
            /* Check if the UARFCN is supported */
            freq_info = &ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.frequencyInfo;

            /* second frequency info */
            if (TRUE == 
              ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.m.multi_frequencyInfoPresent)
            {
              if (TRUE == 
                ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.multi_frequencyInfo.m.secondFrequencyInfoPresent)
              {
                w_freq_info = 
                  &ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.modeSpecificInfo.u.tdd.u.tdd128.multi_frequencyInfo.secondFrequencyInfo;
              }
            }

          }
          else
          {
            /* default_config_mode = tdsrrc_DefaultConfigMode_fdd; */
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for 3.84TDD mode ... rejecting ...", 0, 0, 0);
            return FAILURE;
          }
        }
        else
        {
          default_config_mode = tdsrrc_DefaultConfigMode_fdd;
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for FDD mode ... rejecting ...", 0, 0, 0);
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Default config info is for FDD mode ... rejecting ...", 0, 0, 0);
        return FAILURE;
      }
    }
    else if(ho_to_utran_ptr_r9->specificationMode.u.preconfiguration.preConfigMode.t 
      ==  T_tdsrrc_HandoverToUTRANCommand_r9_IEs_preConfigMode_predefinedConfigIdentity)
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is for predefinedConfig mode ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Mode specific info is absent ... rejecting ...", 0, 0, 0);
      return FAILURE;
    }
  }
#endif /* FEATURE_TDSCDMA_REL9 */
  else
  {
    tdsrrc_print_supported_asn1_rel_version();
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_TDS_PROTOCOL_ERROR;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unsupported message version ... rejecting ...", 0, 0, 0);
    return FAILURE;
  }

  if(config_type == IHO_G2TD_PREDEF_CONFIG)
  {
    /* Preconfiguration mode - predefined not supported */
    tdsprocedure_info.failure_cause =   (uint32)TDSRRC_INVALID_CONFIGURATION;
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Preconfiguration mode - predefined not supported", 0, 0, 0);
    return FAILURE;
  }

  if(default_config_mode == tdsrrc_DefaultConfigMode_tdd)
  {
    /*Should not go into below clause for LTE->TDS PSHO*/
    if(config_type == IHO_G2TD_DEFAULT_CONFIG)
    {
      switch(default_config_id)
      {
        /* Fall through cases that have to processed */
      case TDSRRC_DEF_CONFIG_0:
      case TDSRRC_DEF_CONFIG_1:
      case TDSRRC_DEF_CONFIG_3:
      case TDSRRC_DEF_CONFIG_4:
      case TDSRRC_DEF_CONFIG_7:
      case TDSRRC_DEF_CONFIG_8:
      case TDSRRC_DEF_CONFIG_9:
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      case TDSRRC_DEF_CONFIG_11:
      case  TDSRRC_DEF_CONFIG_13:
      case TDSRRC_DEF_CONFIG_12:
      case TDSRRC_DEF_CONFIG_14:
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */ 
      case TDSRRC_DEF_CONFIG_10:  
        g2td_info.default_config_id = default_config_id;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO: Config ID is - %d", default_config_id, 0, 0);
        break;
      /* default config 22,15,18,19 is valid only for release 6  */
      case TDSRRC_DEF_CONFIG_15:
      case TDSRRC_DEF_CONFIG_18:
      case TDSRRC_DEF_CONFIG_19:
      case TDSRRC_DEF_CONFIG_22:
      case TDSRRC_DEF_CONFIG_21:
      case TDSRRC_DEF_CONFIG_16:
        if( tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6 )
        {
          g2td_info.default_config_id = default_config_id;
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO: Config ID is - %d", default_config_id, 0, 0);
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported Def Config ID %d", default_config_id,0,0);
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
          return FAILURE;
        }
        
        break;

      default:
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported Def Config ID %d", default_config_id,0,0);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        return FAILURE;
      }
    }
  }
  else
  {
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
    return FAILURE;
  }

  /*we only support single cell handover from GtoW.. If Utran wants us to do
    Soft-Handover as we Handover from GtoW we error out and  return failure*/
  if(g2td_info.msg_rel_version == TDSMSG_REL6) 
  {
    if(((config_type == IHO_G2TD_COMPLETE_CONFIG) &&
       ((dl_info_per_rl_r6 == NULL) || (dl_info_per_rl_r6->n != 1)) )
        || ((config_type == IHO_G2TD_DEFAULT_CONFIG) &&
       (dl_info_per_rl_r4_LCR == NULL)))
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Single Pri-SCR supported for GtoTD HO", 0,0,0);
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
      return FAILURE;
    }
  }
  else
  if((g2td_info.msg_rel_version == TDSMSG_REL7) 
#ifdef FEATURE_TDSCDMA_REL8
    || (g2td_info.msg_rel_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        || (g2td_info.msg_rel_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
    )
  {
    if(((config_type == IHO_G2TD_COMPLETE_CONFIG) &&
       ((dl_info_per_rl_r7 == NULL) || (dl_info_per_rl_r7->n != 1)) )
        || ((config_type == IHO_G2TD_DEFAULT_CONFIG) &&
       (dl_info_per_rl_r4_LCR == NULL)))
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Single Pri-SCR supported for GtoTD HO", 0,0,0);
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
      return FAILURE;
    }
  }
  else
  {
    if(g2td_info.msg_rel_version == TDSMSG_REL5)
    {
      if(((config_type == IHO_G2TD_COMPLETE_CONFIG) &&
         ((dl_info_per_rl_r5 == NULL) || (dl_info_per_rl_r5->n != 1)) )
          )
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Single Pri-SCR supported for GtoTD HO", 0,0,0);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        return FAILURE;
      }
    }
    else if(g2td_info.msg_rel_version == TDSMSG_REL4)
    {
      if(((config_type == IHO_G2TD_COMPLETE_CONFIG) &&
         ((dl_info_per_rl_r4 == NULL) || (dl_info_per_rl_r4->n != 1)) )
          )
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Single Pri-SCR supported for GtoW HO", 0,0,0);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
        return FAILURE;
      }
    }
  }

  /* TBD Multi-frequency Info needs also been processed*/



  if(tdsrrc_validate_tds_frequency(freq_info->uarfcn_Nt) == FAILURE)
  {
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_FREQUENCY_NOT_IMPLEMENTED;
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported frequency band", 0,0,0);
    return FAILURE;
  }
  else
  {
    /*get the UARFCN on which to do Acquisition*/
    g2td_info.freq = freq_info->uarfcn_Nt;
  }

  /* second frequency info */
  if (NULL != w_freq_info)
  {
    if ((tdsrrc_validate_tds_frequency(w_freq_info->uarfcn_Nt) == FAILURE))
    {
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_FREQUENCY_NOT_IMPLEMENTED;
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported frequency band in second frequncy", 0,0,0);
      return FAILURE;
    }
  }

  if(ciphering_algo_present)
  {
    g2td_info.ciph_algo_present  = ciphering_algo_present;
    if((g2td_info.msg_rel_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
     || (g2td_info.msg_rel_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
         || (g2td_info.msg_rel_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
    )
    {
      g2td_info.cipheringAlgorithm = tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
                                              &ciphering_algo_r7,
                                              TDSMSG_REL7);
    }
    else
    {
      g2td_info.cipheringAlgorithm = tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
                                              &ciphering_algo,
                                              TDSMSG_REL99);
    }
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"I.E ciph_algo not present", 0, 0, 0);
    g2td_info.ciph_algo_present  = FALSE;
  }

  g2td_info.config_type = config_type;
  g2td_info.cpid    = cpid_code_type;

  /* check if we are successful in extracting the SRB and RB information */
  return tdsrrciho_validate_and_extract_srb_rab_info();
} /* end of tdsrrciho_verify_and_extract_handover_to_utran_command */

/*===========================================================================

FUNCTION  tdsrrciho_process_meas_data_for_handover_to_utran_command

DESCRIPTION

  This function processes measurement data received in
  TDSRRC_INTERRAT_HANDOVER_REQ sent by RR.

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_meas_data_for_handover_to_utran_command
(
  tdsrrc_interrat_measurement_data_type * meas_data
)
{
  uint32 freq_count=0;
  uint32 cell_count=0; /* for count in for loop*/

  g2td_info.num_of_jds_cells = 0;

  for(freq_count = 0;
      (freq_count < meas_data->num_of_freq) && (g2td_info.num_of_jds_cells == 0);
      freq_count ++)
  {
      
    if (meas_data->utran_tds_freq_measurements[freq_count].uarfcn == g2td_info.freq)
    {
	  for (cell_count = 0;
	       cell_count < meas_data->utran_tds_freq_measurements[freq_count].num_of_jds_cells;
		   cell_count ++)
      {
        g2td_info.cell_list[cell_count] = meas_data->utran_tds_freq_measurements[freq_count].cell_list[cell_count].cpid;
		/*if freq and cpid matche then measurement for the cell to which
              we have to handover is known.. copy timing information*/
        if(meas_data->utran_tds_freq_measurements[freq_count].cell_list[cell_count].cpid == g2td_info.cpid)
        {
          g2td_info.lna_state = meas_data->utran_tds_freq_measurements[freq_count].lna_state;
	      g2td_info.ts0_position = meas_data->utran_tds_freq_measurements[freq_count].ts0_position;
	      g2td_info.num_of_jds_cells = meas_data->utran_tds_freq_measurements[freq_count].num_of_jds_cells;

		  g2td_info.rscp = meas_data->utran_tds_freq_measurements[freq_count].cell_list[cell_count].rscp;
        }
      }
    }
  }
}

#ifdef FEATURE_LTE_TO_TDSCDMA

/*===========================================================================

FUNCTION   tdsrrciho_send_srvcc_handover_failure_ind

DESCRIPTION

  This function sends SRVCC HO failure indication to NAS. So that IMS can send re-INVITE to NW and resume
  IMS call on LTE.
  This SRVCC failure indication is a requirement from upper layers, and SRVCC success will be derived by
  NAS from RRC_ACTIVATION_IND and no seperate indication is needed.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_srvcc_handover_failure_ind(void)
{

  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SRVCC_HANDOVER_FAILURE_IND)) == NULL)
  {
    ERR_FATAL("RRC Unable to send RRC_SRVCC_HANDOVER_FAILURE_IND to MM", 0, 0, 0);
  }
#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_srvcc_handover_failure_ind.as_id = tdsrrc_get_as_id();
#endif
  /* Put the command on MM queue */
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: Sending MM RRC_SRVCC_HANDOVER_FAILURE_IND to NAS", 0, 0, 0);

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
}

/*===========================================================================

FUNCTION   tdsrrciho_send_l2td_ps_handover_to_utran_success

DESCRIPTION

  This function sends a HO success to UTRAN as well as a HO success indication
  to RR.  We should have successfully done Acquisition on WCDMA side and
  configured WCDMA lower layers to go to DCH state.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_l2td_ps_handover_to_utran_success(void)
{
  /*declare var of struct handover confirm type*/
  tds_rrc_lte_psho_rsp_type l2td_ho_cnf;

  memset(&l2td_ho_cnf,0,sizeof(tds_rrc_lte_psho_rsp_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

  /*set HO status to success*/
  l2td_ho_cnf.handover_status = L2TD_PSHO_SUCCESS;

  msgr_init_hdr(&l2td_ho_cnf.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_PSHO_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&l2td_ho_cnf.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&l2td_ho_cnf.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&l2td_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_psho_rsp_type));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&l2td_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_psho_rsp_type));
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_PSHO_RSP success sent to LTE RRC", 0, 0, 0);

  return;
}/*tdsrrciho_send_l2td_ps_handover_to_utran_success*/

/*===========================================================================

FUNCTION   rrciho_send_handover_to_utran_failure

DESCRIPTION

  This function sends handover to UTRAN failure to RR.  We could have failed
  Handover due to ACQ failure, Channel Config failure or due to HO_ABORT_REQ
  from RR

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_l2td_ps_handover_to_utran_failure(void)
{
  /*declare var of struct handover confirm type*/
  tds_rrc_lte_psho_rsp_type l2td_ho_cnf;
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  //tdsrrc_tdrm_exchange_success will be reset to FALSE when change priority as it will release lock
  boolean tdsrrc_tdrm_exchange_success_backup = tdsrrc_tdrm_exchange_success;
#endif

  memset(&l2td_ho_cnf,0,sizeof(tds_rrc_lte_psho_rsp_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

  /*set HO status to success*/
  l2td_ho_cnf.handover_status = L2TD_PSHO_FAILURE;

  switch( tdsprocedure_info.failure_cause)
  {
    case 0: l2td_ho_cnf.failure_cause = L2TD_PSHO_INVALID_STATE;
            break;
    case 1: l2td_ho_cnf.failure_cause = L2TD_PSHO_TDSCDMA_PROTOCOL_ERROR;
      break;
    case 2: l2td_ho_cnf.failure_cause = L2TD_PSHO_INVALID_CONFIGURATION;
      break;
    case 3: l2td_ho_cnf.failure_cause = L2TD_PSHO_PHYSICAL_CHANNEL_FAILURE;
      break;
    case 4: l2td_ho_cnf.failure_cause = L2TD_PSHO_INVALID_CIPHER_ALGORITHM;
      break;
    case 5: l2td_ho_cnf.failure_cause = L2TD_PSHO_FREQUENCY_NOT_IMPLEMENTED;
      break;
    case 6: l2td_ho_cnf.failure_cause = L2TD_PSHO_MAX_FAILURE_CAUSE;
      break;
    default: /* no action */
      break;
  }

#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  if (tdsrrc_tdrm_exchange_success)
  {
    /*Change TRM priority to BG_TRAFFIC before return lock to LTE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to BG_TRAFFIC for L2T psho fail rsp");
    tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC,TRUE);
    tdsrrc_synced_reason = TRM_BG_TRAFFIC;//Don't needs to use the original synced reason, use this specific one
    //TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: unsync reason for IRAT");
    //tdrm_unsync_reason();
    /* psho is a failure hence the lock needs to be exchanged with IRAT client if TRM exchange
       was success earlier */   
    tdsrrc_tdrm_exchange_success = tdsrrc_tdrm_exchange_success_backup;
    tdsrrc_check_and_perform_trm_exchange(tdsrrc_synced_reason);
  }
#endif

  /* Lock has already been exchanged so releasing the lock will not affect anything 
        This is just done to update WRM variables */
  tdsrrcwrm_release_lock();

  msgr_init_hdr(&l2td_ho_cnf.msg_hdr,MSGR_TDSCDMA_RRC, TDSCDMA_RRC_LTE_PSHO_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&l2td_ho_cnf.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&l2td_ho_cnf.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&l2td_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_psho_rsp_type));
  TDSRRC_MSG3(MSG_LEGACY_HIGH, "L2TD: TDSCDMA_RRC_LTE_PSHO_RSP failure sent to LTE RRC", 0, 0, 0);

  if(g2td_info.is_cs_rab_present)
  {
    /* Send SRVCC failure indication to NAS, so that IMS call can resume on LTE
       This SRVCC failure indication is a requirement from upper layers, and SRVCC success will 
       be derived by NAS from RRC_ACTIVATION_IND and no seperate indication is needed. */
    tdsrrciho_send_srvcc_handover_failure_ind();
  }


  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&l2td_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_psho_rsp_type));
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_PSHO_RSP failure sent to LTE RRC", 0, 0, 0);

  return;
}/*rrciho_send_handover_to_utran_failure*/

/*===========================================================================

FUNCTION   tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf

DESCRIPTION

  This function sends handover to UTRAN Abort Cnf to RR.  This msg is in
  response  to HO_ABORT_REQ from RR

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf(void)
{
  /*declare var of struct handover confirm type*/
  tds_rrc_lte_abort_psho_rsp_type l2td_abort_ho_cnf;
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  //tdsrrc_tdrm_exchange_success will be reset to FALSE when change priority as it will release lock
  boolean tdsrrc_tdrm_exchange_success_backup = tdsrrc_tdrm_exchange_success;
#endif

  memset(&l2td_abort_ho_cnf,0,sizeof(tds_rrc_lte_abort_psho_rsp_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  if (tdsrrc_tdrm_exchange_success)
  {
    /*Change TRM priority to BG_TRAFFIC before return lock to LTE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to BG_TRAFFIC for L2T psho abort cnf");
    tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC,TRUE);
    tdsrrc_synced_reason = TRM_BG_TRAFFIC;//Don't needs to use the original synced reason, use this specific one
    //TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: unsync reason for IRAT");
    //tdrm_unsync_reason();
    /* psho is a failure hence the lock needs to be exchanged with IRAT client if TRM exchange
       was success earlier */   
    tdsrrc_tdrm_exchange_success = tdsrrc_tdrm_exchange_success_backup;
    tdsrrc_check_and_perform_trm_exchange(tdsrrc_synced_reason);
  }
#endif

  /* Lock has already been exchanged so releasing the lock will not affect anything 
        This is just done to update WRM variables */
  tdsrrcwrm_release_lock();

  msgr_init_hdr(&l2td_abort_ho_cnf.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_ABORT_PSHO_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&l2td_abort_ho_cnf.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&l2td_abort_ho_cnf.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&l2td_abort_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_abort_psho_rsp_type));
  TDSRRC_MSG3(MSG_LEGACY_HIGH, "L2TD: TDSCDMA_RRC_LTE_ABORT_PSHO_RSP sent to LTE RRC", 0, 0, 0);

  if(g2td_info.is_cs_rab_present)
  {
    /* Send SRVCC failure indication to NAS, so that IMS call can resume on LTE
       This SRVCC failure indication is a requirement from upper layers, and SRVCC success will 
       be derived by NAS from RRC_ACTIVATION_IND and no seperate indication is needed. */
    tdsrrciho_send_srvcc_handover_failure_ind();
  }

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&l2td_abort_ho_cnf.msg_hdr, sizeof(tds_rrc_lte_abort_psho_rsp_type));
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_ABORT_PSHO_RSP sent to LTE RRC", 0, 0, 0);

  return;
}/*tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf*/
#endif


/*===========================================================================

FUNCTION   tdsrrciho_get_sib16_ptr

DESCRIPTION

  This function gets the Sib16 ptr either from the actual sib16 or the
  SIB16 version of Default Configuration

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_SysInfoType16 * tdsrrciho_get_sib16_ptr()
{

  /*if the config type is complete, then we don't need SIB16 ptr*/
  if(g2td_info.config_type == IHO_G2TD_COMPLETE_CONFIG)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"No SIB16 Info reqd.",0,0,0);
  }

  /*return the ptr to the sib16*/
  return g2td_info.sib16_ptr;
}

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION   tdsrrciho_get_rbs_ptr

DESCRIPTION

  This function gets the RBS ptr either from the actual RBS or the
  RBS version of Default Configuration

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrciho_get_rbs_ptr
(  
  uint32 *df_config_id
)
{
  void *df_iho_ptr = NULL;
  
  /*if the config type is complete, then we don't need RBS ptr*/
  if(g2td_info.config_type == IHO_G2TD_COMPLETE_CONFIG)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"No Radio Bearer setup Info reqd.",0,0,0);
  }

  if((g2td_info.msg_rel_version == TDSMSG_REL4)
     ||(g2td_info.msg_rel_version == TDSMSG_REL5)
     || (g2td_info.msg_rel_version == TDSMSG_REL6)
     || (g2td_info.msg_rel_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
     || (g2td_info.msg_rel_version == TDSMSG_REL8)
#endif/* FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
     ||(g2td_info.msg_rel_version == TDSMSG_REL9)
#endif/* FEATURE_TDSCDMA_REL9*/
)
  { 
    if(g2td_info.rbs_ptr != NULL)
    {
      df_iho_ptr = (void *)&g2td_info.rbs_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5;
      *df_config_id = g2td_info.default_config_id;
    }
  }
  /*return the ptr to the sib16*/
  return df_iho_ptr;  
} /* tdsrrciho_get_rbs_ptr */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*===========================================================================

FUNCTION   tdsrrciho_send_channel_config_for_x2td

DESCRIPTION

  This function first calls set_ordered_config.

  This function channel config req to LLC to perform transition to DCH state
  with the configuration in ordered config.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrciho_send_channel_config_for_x2td(void)
{
  /*ptr to internal command for chan config req*/
  tdsrrc_cmd_type *cmd_ptr;

  uint32 i;

  tdsrrcllc_oc_status_e_type oc_status;/* To hold tdsordered_config status          */
#ifdef FEATURE_LTE_TO_TDSCDMA
  uint8 number_of_rbs = 0;
  uint8 j;
#endif /* FEATURE_LTE_TO_TDSCDMA */


  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Set Ordered Config Sent",0,0,0);

  /*set ordered config before sending channel config request*/
  oc_status =   tdsrrcllc_set_ordered_config(TDSRRC_PROCEDURE_IHO,
                                          TDSRRC_STATE_CELL_DCH,
                                          tdsrrc_HandoverToUTRANCommand_PDU,
                                          g2td_info.ho_ptr);


  if(oc_status != TDSORDERED_CONFIG_SET)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"OC Set failed for HO to UTRAN",0,0,0);
    return FAILURE;
  }

  /* This check is done , so that if the NAS Sync ind is not received
   * in the OTA msg, but the Trch blocks are configured for Wb-AMR rate.
   * Then in this case we can derive the codec used from the Transport 
   * Channel blocks size used
   */
#ifndef FEATURE_LTE_TO_TDSCDMA
  if(TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs.rab_type)
#else
  if(g2td_info.is_cs_rab_present)
#endif 
 {
    uint32 codec_info = TDSCDMA_IVOCODER_ID_NONE;
    {
      tdsdch_id_mapping dch_mapping[TDSMAX_DCH_VOICE_CHANNEL];
      (void)tdsrrcllc_derive_amr_rate(&codec_info,dch_mapping);
    }
    if(codec_info != TDSCDMA_IVOCODER_ID_NONE )
    {
      tdsrrcllcoc_update_oc_orderd_codec_mode(codec_info);
#ifndef FEATURE_LTE_TO_TDSCDMA
      g2td_info.est_rabs.nas_Synchronisation_Indicator = codec_info;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Setting Codec Info as %d in g2td_info",g2td_info.est_rabs.nas_Synchronisation_Indicator,0,0);  
#else
      if(TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type)
      {
        g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator = codec_info;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Setting Codec Info as %d in g2td_info",
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator,0,0);  
      }
#endif
    }
  }

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for CHANNEL_CONFIG_REQ", 0, 0, 0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

  /* Fill in the next state    */
  cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_DCH;

  /*fill in the procedure name*/
  cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_IHO;

  /* Fill in the state change indication required flag */
  cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;

  /* Set the channel config cnf required to true */
  cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

  /*set the srb's that need to be initialized */

  for(i=0; i < g2td_info.num_srb; i++)
  {
    cmd_ptr->cmd.chan_config_req.rb[i].rb_id = g2td_info.srb_id[i];
    cmd_ptr->cmd.chan_config_req.rb[i].rb_config = TDSESTABLISH_RB;
  }

#ifdef FEATURE_LTE_TO_TDSCDMA
  for(j = 0; (j < g2td_info.number_of_est_rabs) && (j < MAX_RAB_TO_SETUP); j++)
  {
    for(i=0;(i < g2td_info.est_rabs[j].num_rbs_for_rab) && (i < MAX_RB_PER_RAB) && ((number_of_rbs + g2td_info.num_srb) < TDS_MAX_RB); i++)
    {
      /*set the rb's that need to be initialized */
      cmd_ptr->cmd.chan_config_req.rb[(number_of_rbs + g2td_info.num_srb)].rb_id =
        (uint16)(g2td_info.est_rabs[j].rb_for_rab[i].rb_id);

      cmd_ptr->cmd.chan_config_req.rb[(number_of_rbs + g2td_info.num_srb)].rb_config =
        TDSESTABLISH_RB;

      number_of_rbs++;
    }
  }

  if((number_of_rbs + g2td_info.num_srb) > (TDS_MAX_RB - 1))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Ordered Config Set failed", 0, 0, 0);
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_PHYSICAL_CHANNEL_FAILURE;
    tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;

    /* Return as SUCCESS otherwise it will be considered as chan_config failure */
    return SUCCESS;
  }

  /* Set the channel config cnf required to true */
  cmd_ptr->cmd.chan_config_req.num_rb_to_config =
    (int)(g2td_info.num_srb + number_of_rbs);

#else

  for(i=0;i < g2td_info.est_rabs.num_rbs_for_rab; i++)
  {
    /*set the rb's that need to be initialized */
    cmd_ptr->cmd.chan_config_req.rb[(i+g2td_info.num_srb)].rb_id =
      (g2td_info.est_rabs.rb_for_rab[i].rb_id);

    cmd_ptr->cmd.chan_config_req.rb[(i+g2td_info.num_srb)].rb_config =
      TDSESTABLISH_RB;
  }

  /* Set the channel config cnf required to true */
  cmd_ptr->cmd.chan_config_req.num_rb_to_config =
    (int)(g2td_info.num_srb + g2td_info.est_rabs.num_rbs_for_rab);
#endif /* FEATURE_LTE_TO_TDSCDMA */

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC_CHAN_CONFIG_REQ sent", 0, 0, 0);

  /* Change the procedure state to wait for change mode confirmation */
  tdsrrciho_state = TDSRRCIHO_WAIT_CHAN_CONFIG_G2TD_CNF;

  return SUCCESS;

}/*tdsrrciho_send_channel_config_for_x2td*/
/*===========================================================================

FUNCTION   tdsrrciho_send_acq_req

DESCRIPTION

  This function sends Acquisition request to LLC which funnels it to L1.
  After we have successfully transitioned TDS mode to
  INTER_RAT_IN_PROGRESS we send this command to acquire on TDS cell
  based on the measurement reports we receive from GSM L1(MPH)

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_acq_req(void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CSP_ACQ_REQ", 0, 0, 0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CSP_ACQ_REQ;

  /* Fill in the Frequence on which we want to acquire    */
  cmd_ptr->cmd.rrc_csp_acq_req.freq = g2td_info.freq;

  /* Fill in the scrambling code */
  cmd_ptr->cmd.rrc_csp_acq_req.cpid = g2td_info.cpid;

  /* Fill in the timing information */
  if (g2td_info.num_of_jds_cells > 0)
  {
    cmd_ptr->cmd.rrc_csp_acq_req.rscp = g2td_info.rscp;
	cmd_ptr->cmd.rrc_csp_acq_req.lna_state = g2td_info.lna_state;
    cmd_ptr->cmd.rrc_csp_acq_req.ts0_position = g2td_info.ts0_position;
    cmd_ptr->cmd.rrc_csp_acq_req.num_of_jds_cells = g2td_info.num_of_jds_cells;
    memscpy(cmd_ptr->cmd.rrc_csp_acq_req.cell_list,sizeof(cmd_ptr->cmd.rrc_csp_acq_req.cell_list),g2td_info.cell_list,g2td_info.num_of_jds_cells*sizeof(uint8));
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"X2T HO: num_of_jds_cells %d, lna_state %d, ts0_position, %d", 
		        g2td_info.num_of_jds_cells, g2td_info.lna_state,g2td_info.ts0_position);
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"X2T HO: other RAT measured target cell rscp %d", 
		        g2td_info.rscp, 0, 0);
  }
  else
  {
    cmd_ptr->cmd.rrc_csp_acq_req.num_of_jds_cells = 0;
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"X2T HO: No JDS cell, timing info is not provided by source RAT",0,0,0);
  }

  cmd_ptr->cmd.rrc_csp_acq_req.network_select_mode = g2td_info.network_select_mode;
  
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_CSP_ACQ_REQ sent to CSP", 0, 0, 0);

  /* Change the procedure state to wait for change mode confirmation */
  tdsrrciho_state = TDSRRCIHO_WAIT_ACQ_CNF;

  return;
}/*tdsrrciho_send_acq_req*/
/*===========================================================================

FUNCTION   tdsrrciho_send_handover_to_utran_abort_cnf

DESCRIPTION

  This function sends handover to UTRAN Abort Cnf to RR.  This msg is in
  response  to HO_ABORT_REQ from RR

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_handover_to_utran_abort_cnf(void)
{
  /*declare var of struct abort confirm type*/
  tdsrrc_interrat_handover_abort_cnf_type gtow_abort_cnf;

  memset(&gtow_abort_cnf,0,sizeof(tdsrrc_interrat_handover_abort_cnf_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

  /* Send the TDSRR_INTERRAT_HANDOVER_ABORT_REQ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(gtow_abort_cnf)), TDSRRC_INTERRAT_HANDOVER_ABORT_CNF,
                            sizeof(tdsrrc_interrat_handover_abort_cnf_type));

#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
    /*Send  GtoW hand over failure   */
    tdsrrc_send_hho_cmd_to_cm(SYS_HO_FAIL);

#endif

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"INTERRAT_HANDOVER_CNF Failure sent to RR", 0, 0, 0);

  tdsrrcwrm_release_lock();

  return;
}/*tdsrrciho_send_handover_to_utran_abort_cnf*/
/*===========================================================================

FUNCTION   tdsrrciho_send_handover_to_utran_failure

DESCRIPTION

  This function sends handover to UTRAN failure to RR.  We could have failed
  Handover due to ACQ failure, Channel Config failure or due to HO_ABORT_REQ
  from RR

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_handover_to_utran_failure(void)
{
  /*declare var of struct handover confirm type*/
  tdsrrc_interrat_handover_cnf_type gtotd_ho_cnf;

  memset(&gtotd_ho_cnf,0,sizeof(tdsrrc_interrat_handover_cnf_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

  /*set HO status to success*/
  gtotd_ho_cnf.handover_status = TDSRRC_RR_FAILURE;

  switch( tdsprocedure_info.failure_cause)
  {
    case 0: gtotd_ho_cnf.failure_cause = TDSRRC_INVALID_STATE;
            break;
    case 1: gtotd_ho_cnf.failure_cause = TDSRRC_TDS_PROTOCOL_ERROR;
      break;
    case 2: gtotd_ho_cnf.failure_cause = TDSRRC_INVALID_CONFIGURATION;
      break;
    case 3: gtotd_ho_cnf.failure_cause = TDSRRC_PHYSICAL_CHANNEL_FAILURE;
      break;
    case 4: gtotd_ho_cnf.failure_cause = TDSRRC_INVALID_CIPHER_ALGORITHM;
      break;
    case 5: gtotd_ho_cnf.failure_cause = TDSRRC_FREQUENCY_NOT_IMPLEMENTED;
      break;
    case 6: gtotd_ho_cnf.failure_cause = TDSRRC_MAX_FAILURE_CAUSE;
      break;
    default: /* no action */
      break;
  }

  /* Send the TDSRR_INTERRAT_HANDOVER_REQ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(gtotd_ho_cnf)), TDSRRC_INTERRAT_HANDOVER_CNF,
                            sizeof(tdsrrc_interrat_handover_cnf_type));

#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
    /*Send  GtoW hand over failure   */
    tdsrrc_send_hho_cmd_to_cm(SYS_HO_FAIL);
#endif

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"INTERRAT_HANDOVER_CNF Failure sent to RR", 0, 0, 0);

  tdsrrcwrm_release_lock();

  tdsrrc_set_ordered_codec_mode(tdsrrc_get_ordered_codec_mode());

  return;
}/*tdsrrciho_send_handover_to_utran_failure*/
/*===========================================================================

FUNCTION   tdsrrciho_send_handover_to_utran_complete

DESCRIPTION

  This function sends a HO success to UTRAN complete message to
  UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_handover_to_utran_complete(void)
{
  tdsrrc_UL_DCCH_Message *msg_ptr=NULL;    /* Pointer to uplink DCCH message */

  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */

  tdsrrcsend_status_e_type send_status;    /* The status of sending the Initial
                                         * Direct Transfer message */


  /* Build a Handover To Utran Complete msg. Start by allocating an Uplink
   * DCCH message. This memory is released by tdsrrcsend_send_ul_sdu().
   */

  /* Determine the logical channel id for the Uplink DCCH */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                       DCCH_AM_RADIO_BEARER_ID,
                                       UE_MODE_ACKNOWLEDGED_DATA);

  /* Ensure that LCM found a valid logical channel id  */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Found a valid RLC Logical Channel Id */
    msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(
                sizeof(struct tdsrrc_UL_DCCH_Message));

    /* Call error fatal if the memory allocation failed
     */
    if (msg_ptr == NULL)
    {
      ERR_FATAL("mem_malloc(%d) failure", sizeof(struct tdsrrc_UL_DCCH_Message),
                 0, 0);
    }

    /*Identify this message as HO to Utran Complete Message*/
    msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_handoverToUTRANComplete;

    /* Initialize bit_mask to zero */
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.handoverToUTRANComplete);

    if((g2td_info.gsm_ciph_status == TRUE)&&
         (tdsrrcsmc_tm_ciph_active() == TRUE))
    {
     TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.handoverToUTRANComplete,
        tdsrrc_HandoverToUTRANComplete,count_C_ActivationTime);

      /*256 is the CFN rollover length*/
      /*250 is used to activate ciphering after 250 10ms time interval,
        this should give sufficient time for n/w side and ue side to have
        configured ciphering on the rb's successfully*/
      /*Also make sure that the activation time is a multiple of 8 */
      g2td_info.count_c_act_time = (tdsseq_get_cfn() + 250) & 0xF8;

      msg_ptr->message.u.handoverToUTRANComplete.count_C_ActivationTime =
        (uint8)g2td_info.count_c_act_time;
    }

    /* Set bit_mask to include START_list */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.handoverToUTRANComplete,
     startList);


    /* Appends START list to the HO to UTRAN Complete message */
    (void)tdsrrcsmc_append_start_list(
      &msg_ptr->message.u.handoverToUTRANComplete.startList,
      TDSRRCSMC_APPEND_START);

   /* Send the HO to UTRAN on the Uplink DCCH
    */
    send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_IHO,
                                      msg_ptr,
                                      ul_lc_id,
#ifdef FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK
                                      TDSRRCSEND_L2ACK_REQUIRED);
#else
                                      TDSRRCSEND_L2ACK_NOT_REQUIRED);
#endif

    if (send_status == TDSRRCSEND_SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"HO to UTRAN complete Msg Sent", 0, 0, 0);
     /*FEATURE_WCDMA_DYNAMIC_POWER_CLASS has been removed in*/
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Error %d sending HO to UTRAN Complete",
                send_status, 0, 0);
    }
  }/* Found a valid RLC Logical Channel Id */
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Can't find RLC ID for RB 2",0,0,0);
  }

#ifdef FEATURE_TDSCDMA_TO_LTE
  if (tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
  {
    tdsrrcueci_send_eutra_capabilitites_req_to_lte();
  }
#endif


} /* tdsrrciho_send_handover_to_utran_complete */

/*===========================================================================

FUNCTION   tdsrrciho_send_handover_to_utran_success

DESCRIPTION

  This function sends a HO success to UTRAN as well as a HO success indication
  to RR.  We should have successfully done Acquisition on TDS side and
  configured TDS lower layers to go to DCH state.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_send_handover_to_utran_success(void)
{
  /*declare var of struct handover confirm type*/
  tdsrrc_interrat_handover_cnf_type gtow_ho_cnf;

  memset(&gtow_ho_cnf,0,sizeof(tdsrrc_interrat_handover_cnf_type));

  /*delete all the stored handover to utran IE's as handover is successful*/
  tdsrrciho_delete_stored_g2td_info();

  /*initialize the HO parameters to default*/
  tdsrrciho_init_data();

  /*set HO status to success*/
  gtow_ho_cnf.handover_status = TDSRRC_RR_SUCCESS;

  /* Send the TDSRR_INTERRAT_HANDOVER_REQ message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(gtow_ho_cnf)), TDSRRC_INTERRAT_HANDOVER_CNF,
                            sizeof(tdsrrc_interrat_handover_cnf_type));

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Successful INTERRAT_HANDOVER_CNF sent to RR", 0, 0, 0);

#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
      /*Send G2W complete to CM*/
  tdsrrc_send_hho_cmd_to_cm(SYS_HO_COMPLETE);
#endif

  return;
}/*tdsrrciho_send_handover_to_utran_success*/

/*===========================================================================

FUNCTION   tdsrrciho_event_log_handover_to_utran

DESCRIPTION

    Event Logging for HO to UTRAN command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_event_log_handover_to_utran(void)
{
  /* declare an event log type for handover start*/
  tdsrrclog_handover_to_utran_start_event_type start_event_log;

  /*update freq and scrambling code info*/
  start_event_log.uarfcn = g2td_info.freq;
  start_event_log.cpid = g2td_info.cpid;

  if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
  {

    /*HX TBD: Needs a new EVENT*/
    event_report_payload(EVENT_TDSCDMA_RRC_GSM_TO_TDSCDMA_HANDOVER_START,
                       sizeof(start_event_log),
                       (void *)&start_event_log);

  }
  else
  {


    event_report_payload(EVENT_TDSCDMA_RRC_GSM_TO_TDSCDMA_HANDOVER_START,
                       sizeof(start_event_log),
                       (void *)&start_event_log);
  }

}/*tdsrrciho_event_log_handover_to_utran*/

/*===========================================================================

FUNCTION   RRCIHO_GET_LOCK_FOR_IHO

DESCRIPTION

  This function tries to acquire lock for IHO. If lock is not acquired
  it sends a failure message to RR.

DEPENDENCIES

  None.

RETURN VALUE

    SUCCESS/FAILURE - if lock is acquired or not.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrciho_get_lock_for_iho(boolean force_to_send_out)
{
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_out_of_lock_wait_e_type  lock_status;

  lock_status = tdsrrcwrm_acquire_lock_for_IHO_or_CCO(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,force_to_send_out);
  tdsrrcwrm_stop_lock_wait_timer();

  if(lock_status == TDSRRC_ABORT_FROM_RR)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM: Lock not acquired for IHO. Abort from RR.", 0, 0, 0);
#ifdef FEATURE_LTE_TO_TDSCDMA
    if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
    {
      tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf();
    }
    else
#endif
    {
    tdsrrciho_send_handover_to_utran_abort_cnf();
    }
    status = FAILURE;
  }
  else if(lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM: Lock not acquired for IHO.", 0, 0, 0);
#ifdef FEATURE_LTE_TO_TDSCDMA
    if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
    {
      tdsprocedure_info.failure_cause = (uint32)L2TD_PSHO_INVALID_STATE;
      /*send a handover to utran failure and reset IHO procedure's state
        machine*/
      tdsrrciho_send_l2td_ps_handover_to_utran_failure();
    }
    else
#endif
    {
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
        /*send a handover to utran failure and reset IHO procedure's state
         machine*/
        tdsrrciho_send_handover_to_utran_failure();
     }

    status = FAILURE;
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM: Lock acquired for IHO.", 0, 0, 0);
  }
  return status;
}
#if 0
/*===========================================================================

FUNCTION   dummy_check_handover_to_utran

DESCRIPTION

  This function returns false and do any necessary cleanup to
  reject a handover_to_utran_command

DEPENDENCIES

  None.

RETURN VALUE

    FALSE always

SIDE EFFECTS

  None.

===========================================================================*/

static boolean tdsrrciho_dummy_check_handover_to_utran_command()
{
  tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_CONFIGURATION;
  return FALSE;
}

#endif


/*===========================================================================

FUNCTION   PROCESS_HANDOVER_TO_UTRAN_COMMAND

DESCRIPTION

  This function processes the handover to UTRAN command. It
  decodes the HandoverToUTRANCommand and checks all the IE's
  and starts the process of acquiring a TDS cell.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_handover_to_utran_command(
  tdsrrc_rr_cmd_type * cmd_ptr
)
{

  byte*  ho_msg_ptr;                     /* ptr to encoded HO msg     */

  tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SELECT_OWNER);

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
  tdsrrciho_interrat_type = TDSRRCIHO_INTERRAT_GSM;
#else
  /*Reject HO to Utran here after get the lock, and 
    reuse the tdsrrciho_send_handover_to_utran_failure
    */
  if(tdsrrciho_dummy_check_handover_to_utran_command() == FALSE)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"HO To UTRAN from GSM NOT supported", 0, 0, 0);
    /* Send handover To UTRAN faliure to RR.*/
    g2td_info.ho_msg_present = FALSE;
    tdsrrciho_send_handover_to_utran_failure();
    return;
  }
#endif



  /*the HO message is the second byte from the beginning of the message.
    So initialize the pointer to the second byte */
   ho_msg_ptr = &(cmd_ptr->cmd.interrat_handover_req.ho_cmd[1]);

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_update_rat_pri_list_info(&cmd_ptr->cmd.interrat_handover_req.rat_pri_list_info);
#endif

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
  /*updating the dedicated priority info received in rrc_interrat_handover_req into global structure (rrc_dedicated_priority_info)*/
  tdsrrcmeas_update_dedicated_priority_info_from_irat_req(&cmd_ptr->cmd.interrat_handover_req.dedicated_priority_info,
                             mmumts_convert_nas_plmn_id_to_rrc_plmn_id(cmd_ptr->cmd.interrat_handover_req.plmn_id));
#endif
  if(tdsrrciho_get_lock_for_iho(FALSE) == FAILURE)
  {
    return;
  }
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
  /*init the default config here.. we have to do this for every call
    because the spoof poke byte can be set at any time*/
  tdsrrcllc_init_default_configuration();
#endif
  /*the HO message is the 1st byte from the beginning of the message.
    So initialize the length of the message to the contents of the first
    byte*/
    g2td_info.ho_msg_len = cmd_ptr->cmd.interrat_handover_req.ho_cmd[0];

    g2td_info.network_select_mode = cmd_ptr->cmd.interrat_handover_req.network_select_mode;

  /*get the ptr to the byte stream of the HandoverToUTRANCommand*/
    g2td_info.ho_ptr =
      (tdsrrc_HandoverToUTRANCommand *) tdsrrcasn1_decode_pdu(  (void *)ho_msg_ptr,
                                                          tdsrrc_HandoverToUTRANCommand_PDU,
                                                          g2td_info.ho_msg_len
                                                          );
#ifdef FEATURE_QSH_EVENT_METRIC
    tdsrrcqsh_save_ota(ho_msg_ptr, 
                       g2td_info.ho_msg_len, 
                       TDSRRC_QSH_HO_TO_UTRAN, 
                       0xFF);
#endif

  if(g2td_info.ho_ptr == NULL)
  {
    /*unable to successfully decode a asn1 PDU, set failure cause to
      Protocol_Error
     */
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_TDS_PROTOCOL_ERROR;

    /*set to flag that HO to UTRAN msg ptr is not present*/
    g2td_info.ho_msg_present = FALSE;

    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unable to decode HO to Utran Msg",0,0,0);
    {
      /* Send handover To UTRAN faliure to RR*/
      tdsrrciho_send_handover_to_utran_failure();
    }
    return;
  }
  else
  {
    /*set the flag to indicate that HO to UTRAN msg is present*/
    g2td_info.ho_msg_present = TRUE;
  }

  /* Verify the handover from UTRAN command */
  if (tdsrrciho_verify_and_extract_handover_to_utran_command(g2td_info.ho_ptr
      ) == FAILURE)
  {
    {
      /* Send handover To UTRAN faliure to RR*/
      tdsrrciho_send_handover_to_utran_failure();
      tdsrrc_free_dynamic_ptrs();
#ifdef FEATURE_WRLF_SYSTEM_SEL
      if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
      {
        tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
      }
#endif
    }
  }
  else
  {
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
    tdsrrc_send_codec_info_to_cm(tdsrrc_get_ordered_codec_mode(), TDSRRC_SAMPLING_RATE_NB);
#endif
/*the TDSINTERRAT_HO_TYPE_CS/PS is not needed, as g2td_info.is_cs/ps_rab_present is used instead*/
/*
#ifdef FEATURE_LTE_TO_TDSCDMA
    g2td_info.irat_ho_type = TDSINTERRAT_HO_TYPE_CS;
#endif
*/
    /*ho cmd is kosher.. so log the event that HO is starting*/
    tdsrrciho_event_log_handover_to_utran();

    /*process measurement data received in the handover to utran command*/
    tdsrrciho_process_meas_data_for_handover_to_utran_command(&cmd_ptr->cmd.interrat_handover_req.interrat_measurement_data);

    /* Request MCM to change the RRC mode Inactive to Interrat in Progress */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_INTERRAT_IN_PROGRESS);

    /* Change the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_IRAT_IN_PROGRESS_CNF;
  }

  /*if verification of the Handover message is successful, then copy the ciphering
    status of GSM mode to the gtow information database.. also copy the ciphering
    status only if nv item for ciphering is set.. otherwise set it to false*/
  if((cmd_ptr->cmd.interrat_handover_req.cipher_enabled == TRUE) &&
     (tdsnv_ciphering_enabled))
  {
    g2td_info.gsm_ciph_status = TRUE;
  }
  else
  {
    g2td_info.gsm_ciph_status = FALSE;
  }

  return;
} /* end of tdsrrciho_process_handover_to_utran_command*/
#endif


/* Take out from FEATURE_INTERRAT_HANDOVER_G2TD */


/*===========================================================================

FUNCTION   tdsrrciho_process_channel_config_cnf

DESCRIPTION

  This function does post processing after Channel Config Cnf is received.
  Sets up the vocoder

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_channel_config_cnf(uint32 current_mode)
{

  tdsrrcllc_amr_mapping_info_type amr_info = {0}; /* To store AMR mode
                                            info incase a voice RAB has
                                            been setup */
#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
  tdsrrc_save_rnc_cap_change_support();
#endif
  /* We need to indicate to the higher layers that a voice RAB has been setup. */
  /* First check what is the current codec that needs to be set */
  tdsrrc_set_current_codec_mode(current_mode);
  
  if( current_mode == TDSCDMA_IVOCODER_ID_AMRWB)
  {
    /* Configure the codec for WB AMR */
    amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMRWB;
    (void) tdsrrcllc_get_amr_wb_mapping(&(amr_info));
  }
  else
  {
    amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMR;
    /*Now TDS Voice Adapter needs to be configured. The structure amr_info will be filled by this function. */
    (void)tdsrrcllc_get_amr_mapping(&(amr_info));
  }
  /* Configure voice adapter to register RLC queues for AMR call */
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_START);

  return;
}

/*===========================================================================

FUNCTION   tdsrrciho_get_activation_time_for_td2g_ho

DESCRIPTION

  This function populates activation time for WtoG handover into the received
  ptr and also returns success/failure if activation is present or not.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS/FAILURE depending if Activation Time was appended or not.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrciho_get_activation_time_for_td2g_ho
(
uint16 * act_time
)
{
  /*if activation time was present in the message then send the same*/
  if(tdsprocedure_info.act_time_present == TRUE)
  {
    *act_time = (uint16)(tdsprocedure_info.activation_time);
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}
/*===========================================================================

FUNCTION   RRCIHO_EXTRACT_SINGLE_GSM_MESSAGE

DESCRIPTION

  This function extracts single GSM message from the HandoverFromUtran
  command.  This message is part of the padding bits.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrciho_extract_single_gsm_message(
  tdsrrc_cmd_type *cmd_ptr                      /* Received command */
)
{

  uint32 total_pad_bits;              /* number of gsm-padding bits that make up gsm msg*/
  uint32 last_octet_pad_len;          /* number of tds-padding bits in last octet of msg*/
  uint32 num_bits_in_msg;             /* number of bits in ota msg without padding*/
  /*tdsrrc_DL_DCCH_Message *dcch_ptr;  */    /* Pointer to the DCCH message */
  uint32 local_encoded_sdu_len;       /* local var to hold encoded sdu length*/
  uint8 index=0;

// PSR . in  Objective sys. we can get the encoded ptr lenght even after decode is called
  #if 0
  /* Get the downlink message from the SDU */
  dcch_ptr=(tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /* Encodes the OTA message using ASN1. We do this because we can get the message
     length in bits only after encoding.  OSS currently does not support getting
     message length in bits after decoding. */
  encoded_sdu = tdsrrcasn1_encode_pdu(dcch_ptr, tdsrrc_DL_DCCH_Message_PDU,NULL);

  /*get the padding bits in the last octet of tds message*/
  last_octet_pad_len = tdsrrcasn1_get_number_of_pad_bits();

  /*copy the len to a local variable as we want to free global heap as soon as
    possible.. the local var is used for further comparisons later*/
  local_encoded_sdu_len = (uint32)(encoded_sdu.length);

  /*free the buffer which we encoded for the sake of getting padding bits*/
  tdsrrcasn1_free_buf(encoded_sdu.value);

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Pad_Bit_Len %d, Decode_Msg_len %d", last_octet_pad_len,
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,0);

  
 
  /*check to see that the message was successfully encoded*/
  if(local_encoded_sdu_len == 0)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"ASN1 encoding failed for HO Single GSM",0,0,0);
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
    tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
    return FALSE;
  }

  /*check to see if padding is present.. only if padding is present received
    OTA message's sdu length will be greater than the newly encoded_sdu which
    will not have the gsm message as padding*/
  if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length <= local_encoded_sdu_len )
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Padding not present in HO msg",0,0,0);
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
    tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
    return FALSE;
  }
  #endif
  
  index =tdsrrc_get_used_ctxt_idx_for_asn1_ptr(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg);
  if (index >= TDSRRC_MAX_NUM_ASN1_CTXT)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"ASN1 ptr index over max",0,0,0);
    return FALSE;
  }

  local_encoded_sdu_len = pu_getMsgLen(&tdsdec_ctxt_list[index].dec_context);
  last_octet_pad_len = PU_GETPADBITS(&tdsdec_ctxt_list[index].dec_context);
  /*total number of bits in the message is equal to the number of bytes in message
    multiplied by 8 and the total subtracted by the number of pad bits in the last
    octet*/
  num_bits_in_msg = ( (local_encoded_sdu_len*8) - last_octet_pad_len);

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Pad_Bit_Len %d, Decode_Msg_len %d", last_octet_pad_len,
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,0);
  /*total number of bits in the padding part of dl sdu received.. subtract new
    encoded length from the downlink sdu length and multiply by 8.  no need to do
    any kind of modulo 8 division to copy till last bit because GSM messages are
    byte aligned and cannot*/
  total_pad_bits = ((cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length - local_encoded_sdu_len)*8);

  if (total_pad_bits > (TDSRR_MAX_GSM_MESSAGE_LENGTH)*8 )
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"The HO message exceed the MAX length.",0,0,0);
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
    tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
    return FALSE;
  }
  /*copy the message length and payload into the existing gsm message command list..
    just copy it to the first buffer of the gsm message list.. we want to use
    the same existing interface between rrc-rr.. single message essentially means
    a list with length 1*/
  tdsprocedure_info.parms.gsm.gsm_command_list.number_of_commands = 1;
  tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[0].message_length =
    (uint8)(total_pad_bits/8);

  /*total number of bytes*/
  b_copy( cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
          (uint16)(num_bits_in_msg),
          tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[0].payload,
          0,
          (uint16)(total_pad_bits)
        );

  return TRUE;
}

/*===========================================================================

FUNCTION   VERIFY_AND_EXTRACT_HANDOVER_FROM_UTRAN_COMMAND_R6

DESCRIPTION

  This function verifies the handover from UTRAN command and sets the failure
  cause properly if an error is detected. If the command is valid, it extracts
  necessary information from the command.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrciho_verify_and_extract_handover_from_utran_command_r6(
  tdsrrc_HandoverFromUTRANCommand_GSM *msg_ptr,
  tdsrrc_cmd_type *cmd_ptr                      /* Received command */
)
{
  tdsrrc_GSM_MessageList *gsm_cmd_ptr;
  uint8 gsm_cmd_ct;
  tdsrrc_RAB_InformationList_r6 * rab_info_list = NULL;

  tdsprocedure_info.handover_type = T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM;

  /* Check IEs in the command */
  /* If non-critical extensions are present, ignore for now */
  if (msg_ptr->u.later_than_r3.criticalExtensions.u.r6.m.handoverFromUTRANCommand_GSM_r6_add_extPresent)
  {
    /* Ignore Non-critical extension */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Ignore R6 nonCriticalExtensions in CMD", 0, 0, 0);
  }
#ifdef FEATURE_TDSCDMA_REL8
      if(msg_ptr->u.later_than_r3.criticalExtensions.u.r6.m.v860NonCriticalExtensionsPresent)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"REL8: v860NonCriticalExtensions not supported",0,0,0);
    }
#endif /*FEATURE_TDSCDMA_REL8*/


  /* For now reject any PS handover as it's not supported */

  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r6.handoverFromUTRANCommand_GSM_r6,
      tdsrrc_HandoverFromUTRANCommand_GSM_r6_IEs,toHandoverRAB_Info)) 
  {
    uint32 local_idx=0;
    rab_info_list = 
      &msg_ptr->u.later_than_r3.criticalExtensions.u.r6.handoverFromUTRANCommand_GSM_r6.toHandoverRAB_Info;
    while (rab_info_list->n > local_idx) 
    {
      if (rab_info_list->elem[local_idx].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain) 
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"PS handover not supported",0,0,0);
        tdsprocedure_info.transaction_id =  msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;
        return(FALSE);
      }
      local_idx++;
    }
  }

  /*check if activation time is present and save the same in tdsprocedure_info*/
  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.later_than_r3.criticalExtensions.u.r6.handoverFromUTRANCommand_GSM_r6,
      tdsrrc_HandoverFromUTRANCommand_GSM_r6_IEs,activationTime))
  {
    /*copy the activation time and set the flag to true*/
    tdsprocedure_info.activation_time =
      msg_ptr->u.later_than_r3.criticalExtensions.u.r6.handoverFromUTRANCommand_GSM_r6.activationTime;
    tdsprocedure_info.act_time_present = TRUE;
  }
  else
  {
    /*else set activation time to false*/
    tdsprocedure_info.act_time_present = FALSE;
  }

  /* Extract the transaction identifier from the command */
  tdsprocedure_info.transaction_id =
    msg_ptr->u.later_than_r3.rrc_TransactionIdentifier;

  /* Fail handover if Ordered Config is set */
  if(TDSOC_NOT_SET != tdsrrcllc_get_ordered_config_status())
  {
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;
    return (FALSE);
  }

  /* Extract frequency band */
  if (msg_ptr->u.later_than_r3.criticalExtensions.u.
    r6.handoverFromUTRANCommand_GSM_r6.frequency_band == tdsrrc_Frequency_Band_dcs1800BandUsed)
  {
    tdsprocedure_info.parms.gsm.frequency_band = TDSRR_DCS_1800;
  }
  else
  {
    tdsprocedure_info.parms.gsm.frequency_band = TDSRR_PCS_1900;
  }

  /* Extract GSM commands */
  if (msg_ptr->u.later_than_r3.criticalExtensions.u.r6.
    handoverFromUTRANCommand_GSM_r6.gsm_message.t
       == T_tdsrrc_HandoverFromUTRANCommand_GSM_r6_IEs_gsm_message_single_GSM_Message)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Single GSM CMD received", 0, 0, 0);

    /*if extracting single gsm message failed for any reason.. return failure*/
    if(!(tdsrrciho_extract_single_gsm_message(cmd_ptr)))
    {
      return (FALSE);
    }
  }
  else
  {
    /* Extract the GSM command list: For handover, list should only contain 1 command */
    gsm_cmd_ct = 0;
    gsm_cmd_ptr = &msg_ptr->u.later_than_r3.criticalExtensions.u.r6.handoverFromUTRANCommand_GSM_r6.gsm_message.u.gsm_MessageList.gsm_Messages;

    /* Check if these is no GSM command present */
    if ( (gsm_cmd_ptr == NULL ) || (gsm_cmd_ptr->n ==0))
    {
      /* Set failure cause and return failure */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
      tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"GSM Command list is empty!", 0, 0, 0);
      return (FALSE);
    }

    while (( gsm_cmd_ptr->n > gsm_cmd_ct) && (gsm_cmd_ct < TDSRR_MAX_GSM_COMMANDS ))
    {
      
      if (gsm_cmd_ptr->elem[gsm_cmd_ct].numbits > (TDSRR_MAX_GSM_MESSAGE_LENGTH)*8 )
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"The HO message exceed the MAX length.",0,0,0);
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
        tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        return FALSE;
      }

      /* Convert message length from the number of bits to number of bytes */
      tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length =
        (uint8) (gsm_cmd_ptr->elem[gsm_cmd_ct].numbits/ 8);

      /* Check if the command is lined up with byte boundary */
      if ((gsm_cmd_ptr->elem[gsm_cmd_ct].numbits% 8) != 0)
      {
        /* not in byte boundary: indicate a warning and continue the process */
        tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length++;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"GSM CMD is not on byte boundary", 0, 0, 0);
      }

      if (tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length > 64)
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Array 'data' of size 64 may use index value(s) 0..255.",0,0,0);
      }
      else
      {
        /* Extract the GSM command */
        memscpy((tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].payload),
        /* Fix the KW error */
         (size_t)tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length,
         gsm_cmd_ptr->elem[gsm_cmd_ct].data,
         (size_t)tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length);
      }

      /* Move to the next command */
      gsm_cmd_ct++;

    }

    /* Check if the command list is more than TDSRR_MAX_GSM_COMMANDS, indicate a warning */
    if ( gsm_cmd_ptr->n >  TDSRR_MAX_GSM_COMMANDS )
    {
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"More than %d GSM CMDs Received", TDSRR_MAX_GSM_COMMANDS, 0, 0);
    }

    /* Set the command counter */
    tdsprocedure_info.parms.gsm.gsm_command_list.number_of_commands = gsm_cmd_ct;
  }

  return (TRUE);

} /* end of tdsrrciho_verify_and_extract_handover_from_utran_command_r6 */

/*===========================================================================

FUNCTION   VERIFY_AND_EXTRACT_HANDOVER_FROM_UTRAN_COMMAND

DESCRIPTION

  This function verifies the handover from UTRAN command and sets the failure
  cause properly if an error is detected. If the command is valid, it extracts
  necessary information from the command.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrciho_verify_and_extract_handover_from_utran_command(
  tdsrrc_HandoverFromUTRANCommand_GSM *msg_ptr,
  tdsrrc_cmd_type *cmd_ptr                      /* Received command */
)
{
  tdsrrc_GSM_MessageList *gsm_cmd_ptr;
  uint8 gsm_cmd_ct;
  tdsrrc_RAB_Info * rab_info = NULL;

  tdsprocedure_info.handover_type = T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM;


  /* Check the message if the critical extension is present */
  if (msg_ptr->t == T_tdsrrc_HandoverFromUTRANCommand_GSM_later_than_r3)
  {
    /* Check if R6 Msg is received */
    if (msg_ptr->u.later_than_r3.criticalExtensions.t == 
        T_tdsrrc_HandoverFromUTRANCommand_GSM_criticalExtensions_1_r6) 
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rcvd R6 HO from Utran msg",0,0,0);
      return tdsrrciho_verify_and_extract_handover_from_utran_command_r6(msg_ptr, cmd_ptr);
    }
    else
    {
      /* Critical extension is not supported in this release, send handover from
         UTRAN failure to UTRAN */
      tdsprocedure_info.transaction_id = 0; /* transaction id unknown */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
      tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"CriticalExtensions not supported!", 0, 0, 0);
      return (FALSE);
    }
  }


  /*check if activation time is present and save the same in tdsprocedure_info*/
  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3,
        tdsrrc_HandoverFromUTRANCommand_GSM_r3_IEs,activationTime))
  {
    /*copy the activation time and set the flag to true*/
    tdsprocedure_info.activation_time =
      msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.activationTime;
    tdsprocedure_info.act_time_present = TRUE;
  }
  else
  {
    /*else set activation time to false*/
    tdsprocedure_info.act_time_present = FALSE;
  }

  /* Check IEs in the command */
  /* If non-critical extensions are present, ignore for now */
  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3,
    tdsrrc_HandoverFromUTRANCommand_GSM_r3,laterNonCriticalExtensions))
  {
    /* Ignore Non-critical extension */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Ignore nonCriticalExtensions in CMD", 0, 0, 0);
  }

  /* Reject any PS handover as it has not been supported */

  if (TDSRRC_MSG_COMMON_BITMASK_IE(msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3,
      tdsrrc_HandoverFromUTRANCommand_GSM_r3_IEs,toHandoverRAB_Info)) 
  {
    
    rab_info = 
      &msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.toHandoverRAB_Info;
    if(rab_info->cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain) 
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"PS handover not supported",0,0,0);
      tdsprocedure_info.transaction_id =  msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.rrc_TransactionIdentifier;
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;
      return(FALSE);
    }
      
  }



  /* Extract the transaction identifier from the command */
  tdsprocedure_info.transaction_id =
    msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.rrc_TransactionIdentifier;

  /* Fail handover if Ordered Config is set */
  if(TDSOC_NOT_SET != tdsrrcllc_get_ordered_config_status())
  {
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;
    return (FALSE);
  }

  /*-------------------------------------------------------------------------
   Ignore activation time: assume the activation time is NOW and ignore the
   RAB info for now
  -------------------------------------------------------------------------*/

  /* Extract frequency band */
  if (msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.frequency_band == tdsrrc_Frequency_Band_dcs1800BandUsed)
  {
    tdsprocedure_info.parms.gsm.frequency_band = TDSRR_DCS_1800;
  }
  else
  {
    tdsprocedure_info.parms.gsm.frequency_band = TDSRR_PCS_1900;
  }

  /* Extract GSM commands */
  if (msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.gsm_message.t 
    == T_tdsrrc_HandoverFromUTRANCommand_GSM_r3_IEs_gsm_message_single_GSM_Message)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Single GSM CMD received", 0, 0, 0);

    /*if extracting single gsm message failed for any reason.. return failure*/
    if(!(tdsrrciho_extract_single_gsm_message(cmd_ptr)))
    {
      return (FALSE);
    }
  }
  else
  {
    /* Extract the GSM command list: For handover, list should only contain 1 command */
    gsm_cmd_ct = 0;
    gsm_cmd_ptr = &msg_ptr->u.r3.handoverFromUTRANCommand_GSM_r3.gsm_message.u.gsm_MessageList.gsm_Messages;

    /* Check if these is no GSM command present */
    if ( (gsm_cmd_ptr == NULL ) ||gsm_cmd_ptr->n ==0 )
    {
      /* Set failure cause and return failure */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
      tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"GSM Command list is empty!", 0, 0, 0);
      return (FALSE);
    }

    while ( gsm_cmd_ptr->n >  gsm_cmd_ct && gsm_cmd_ct < TDSRR_MAX_GSM_COMMANDS )
    {
      
      if (gsm_cmd_ptr->elem[gsm_cmd_ct].numbits > (TDSRR_MAX_GSM_MESSAGE_LENGTH)*8 )
      {
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
        tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"The HO message exceed the MAX length.",0,0,0);
        return FALSE;
      }
      /* Convert message length from the number of bits to number of bytes */
      tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length =
        (uint8) (gsm_cmd_ptr->elem[gsm_cmd_ct].numbits / 8);

      /* Check if the command is lined up with byte boundary */
      if ((gsm_cmd_ptr->elem[gsm_cmd_ct].numbits % 8) != 0)
      {
        /* not in byte boundary: indicate a warning and continue the process */
        tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length++;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"GSM CMD is not on byte boundary", 0, 0, 0);
      }

      if (tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length > 64)
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Array 'data' of size 64 may use index value(s) 0..255.",0,0,0);
      }
      else
      {
        /* Extract the GSM command */
        memscpy((tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].payload),
        /* Fix the KW error */
         (size_t)tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length,
         gsm_cmd_ptr->elem[gsm_cmd_ct].data,
         (size_t)tdsprocedure_info.parms.gsm.gsm_command_list.gsm_commands[gsm_cmd_ct].message_length);
      }

      /* Move to the next command */
      gsm_cmd_ct++;;

    }

    /* Check if the command list is more than TDSRR_MAX_GSM_COMMANDS, indicate a warning */
    if ( gsm_cmd_ptr->n > TDSRR_MAX_GSM_COMMANDS )
    {
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"More than %d GSM CMDs Received", TDSRR_MAX_GSM_COMMANDS, 0, 0);
    }

    /* Set the command counter */
    tdsprocedure_info.parms.gsm.gsm_command_list.number_of_commands = gsm_cmd_ct;
  }

  return (TRUE);

} /* end of tdsrrciho_verify_and_extract_handover_from_utran_command */


/*===========================================================================

FUNCTION   tdsrrciho_event_log_handover_from_utran

DESCRIPTION

    Event Logging for HO from UTRAN command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void          tdsrrciho_event_log_handover_from_utran
(
       uint8         msg_type
         /* Handover message type.
         */
)
{
  /* declare an event log type for handover start*/
  tdsrrclog_inter_rat_handover_start_event_type start_event_log;

  /*update event type to tds_handover type to GSM.  GSM = 0, CDMA2000 = 1.
  */
  switch ( msg_type )
  {
    case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
      start_event_log.tdscdma_handover_type = 0;
      break;

    case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000:
      start_event_log.tdscdma_handover_type = 1;
      break;

    case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
      start_event_log.tdscdma_handover_type = 2;
      break;

    default:
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown handover msg_type %d", msg_type, 0, 0 );
      start_event_log.tdscdma_handover_type = 0;
      break;
  }

  /* we do not support activation time.. will update this field when we support
   * activation time in Handover From Utran command
   */
  start_event_log.activation = 0x01;
  start_event_log.activation_cfn = 0x00;

  /*get current cfn from L1*/
  start_event_log.receive_cfn =tdsseq_get_cfn();

  event_report_payload(EVENT_TDSCDMA_RRC_INTER_RAT_HANDOVER_START,
                       sizeof(start_event_log),
                       (void *)&start_event_log);
}/*tdsrrciho_event_log_handover_from_utran*/

/*===========================================================================

FUNCTION   tdsrrciho_event_log_handover_from_utran_end

DESCRIPTION

    Event Logging for HO from UTRAN end.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void          tdsrrciho_event_log_handover_from_utran_end
(
       boolean handover_status,
       tdsrrciho_event_interrat_ho_failure_cause_e_type failure_cause
)
{
  /* declare an event log type for handover end */
  tdsrrclog_inter_rat_handover_end_event_type end_event_log;

  /* handover_status - 0: failure, 1: success */
  end_event_log.tdscdma_handover_success = (uint8)handover_status;
  end_event_log.tdscdma_handover_failure = (uint8)failure_cause;

  event_report_payload(EVENT_TDSCDMA_RRC_INTER_RAT_HANDOVER_END,
                       sizeof(end_event_log),
                       (void *)&end_event_log);

  if (handover_status == FALSE)
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
	                           TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							   (TDS_SELF_HOSTING_MINOR_RRC_INTER_RAT_HANDOVER_FAILURE_BASE +
							    (uint8)failure_cause));
  }
}/*tdsrrciho_event_log_handover_from_utran_end*/


/*===========================================================================

FUNCTION   PROCESS_HANDOVER_FROM_UTRAN_COMMAND_GSM

DESCRIPTION

  This function verifies and processes the handover from UTRAN command. It
  initiates an inter-RAT handover procedure if it is required.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_handover_from_utran_command_gsm(
  tdsrrc_DL_DCCH_Message *dcch_ptr,    /* Pointer to the DCCH message */
  tdsrrc_cmd_type *cmd_ptr             /* Received command */
)
{
  tdsrrc_HandoverFromUTRANCommand_GSM *msg_ptr;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rvd Handover From UTRAN Command GSM", 0, 0, 0);

  msg_ptr = &(dcch_ptr->message.u.handoverFromUTRANCommand_GSM);

  /* Verify the handover from UTRAN command */
  if (tdsrrciho_verify_and_extract_handover_from_utran_command(msg_ptr, cmd_ptr) == FALSE)
  {
    /* Send handover from UTRAN faliure */
    tdsrrciho_send_handover_from_utran_failure();
  }
  /*must check after validation*/
  else if(TRUE == tdsrrcrce_get_rrc_abort_cnf_status())
  {
    /*Check the race condition for Soft-Abort pending on confirm*/
    TDSRRC_MSG3(MSG_LEGACY_ERROR, "There is L2ACK pending for Soft-Abort, return physical channel failure ", 0, 0, 0);
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_physicalChannelFailure;
    /* Send handover from UTRAN faliure */
    tdsrrciho_send_handover_from_utran_failure();
    return;
  }
  else
  {
    /* Check if RRC is in CELL_DCH state */
    if (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
    {
      /* RRC is not the correct state to process the command, send failure to UTRAN */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
      tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState;
      tdsrrciho_send_handover_from_utran_failure();
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"UE is not in CELL_DCH", 0, 0, 0);
      return;
    }

    /* Check if UE is dual-mode enabled */
    if (tdsrrcmcm_is_dualmode_enabled() == FALSE)
    {
      /* UE is not enabled for dual-mode, send failure to UTRAN */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;
      tdsrrciho_send_handover_from_utran_failure();
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"UE is not dual-mode enabled", 0, 0, 0);
      return;
    }

    /* Check if GSM band is supported by UE */
    /* RR will provide a I/F function to verify this */

    
#if 0 /* Move to after suspend succeeds */
    /* Get inter-RAT measurement data from L1 */
    tdsl1_get_interrat_measurement_data(&tdsprocedure_info.parms.gsm.interrat_measurement_data);
#endif

    /*Event logging for HO From UTRAN command*/
    tdsrrciho_event_log_handover_from_utran( T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM );

    if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
    {
      tdsrrccsp_cleanup_bplmn_state();
    }

    /* Request MCM to change the RRC mode from Active to Standby */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_STANDBY);
    
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
    if(tdsrrcsib_is_current_event_sib_read_in_dch())
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Force SIB Init in DCH ",0,0,0);
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH_NO_PCCPCH_DROP);
    }
#endif

    /* Change the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_STANDBY_CNF;
    return;
  }
} /* end of tdsrrciho_process_handover_from_utran_command_gsm*/


#ifdef FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT
/*===========================================================================

FUNCTION   tdsrrciho_process_td2l_ps_handover_cnf

DESCRIPTION

  This function processes the TD2L PS handover confirm message from LRRC.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
void static tdsrrciho_process_td2l_ps_handover_cnf
(
  lte_rrc_tdscdma_psho_rsp_s  interrat_ho_cnf
)
{
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;
#endif

  /* Check if the status is successful */
  if (interrat_ho_cnf.ho_status == LTE_RRC_IRAT_TO_LTE_PSHO_SUCCESS)
  {
     /* Log inter-RAT handover end */
    tdsrrciho_event_log_handover_from_utran_end(TRUE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_MAX);

#if defined(FEATURE_3GPP_FAST_DORMANCY)
    /* Abort Dormancy if IHO proceed to successful processing. */
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_IHO);
#endif   

    /* Procedure ends */
    tdsrrciho_init_data();
  }
  else
  {
  
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
    if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
   	{
      tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
   	  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TD2L: TDRM exchange success for T2L PSHO cnf failure rsp");
   	}

    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
      tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
      wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
      tdsrrcwrm_stop_lock_wait_timer();
      if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2L PSHO cnf failure rsp, wrm_lock_status: %d", wrm_lock_status);
      }
    }
    tdsrrc_tdrm_exchange_success = FALSE;
#endif
  
    /* Inter-RAT handover failed: Resume the WCDMA mode and revert back to
       UTRAN configuration. */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);
  
    /* Set the failure cause */
    switch(interrat_ho_cnf.failure_cause)
    {
      /* Invalid Configuration */
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_INVALID_CONFIG:
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
        break;
  
      /* Invalid RR state or GSM protocol error */
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_INVALID_STATE:
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_PROTOCOL_ERROR:
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_interRAT_ProtocolError;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
        break;
  
      /* Physical channel setup failure */
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_ACQ_FAILURE:
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_FREQ_NOT_IMPLEMENTED:
      case LTE_RRC_IRAT_TO_LTE_PSHO_FAILURE_CAUSE_RACH_FAILED:
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_physicalChannelFailure;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_PHY_CHANNEL_CONFIG_FAILURE);
        break;

      default:
      /* Invalid failure cause */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid Failure Cause for IHO:%d", interrat_ho_cnf.failure_cause, 0, 0);
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;
        break;
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L: Failure Cause for IHO:%d", interrat_ho_cnf.failure_cause, 0, 0);

#ifdef FEATURE_QSH_EVENT_METRIC
    TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_IRAT_HO_FAILURE);
    tdsrrcqsh_update_ho_failure(SYS_HANDOVER_FAILURE_T_TO_L_FAILURE);
#endif

    /* Set the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
  }
  return;

}

/*===========================================================================

FUNCTION   TDSRRCIHO_VERIFY_AND_EXTRACT_HANDOVER_FROM_UTRAN_COMMAND_EUTRA

DESCRIPTION

  This function verifies the handover from UTRAN command and sets the failure
  cause properly if an error is detected. If the command is valid, it extracts
  necessary information from the command.

DEPENDENCIES

  None.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
static boolean tdsrrciho_verify_and_extract_handover_from_utran_command_eutra
(
  tdsrrc_HandoverFromUTRANCommand_EUTRA *msg_ptr
)
{
  uint16 container_size = 0, num_bytes = 0;

  tdsprocedure_info.handover_type = T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA;

  if(msg_ptr->criticalExtensions.t != T_tdsrrc_HandoverFromUTRANCommand_EUTRA_criticalExtensions_1_r8)
  {
    /* Critical extension is not supported, send handover from
       UTRAN failure to UTRAN */
    tdsprocedure_info.transaction_id = 0; /* transaction id unknown */
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
    tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"CriticalExtensions not supported!", 0, 0, 0);
    return (FALSE);
  }

  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(msg_ptr->criticalExtensions.u.r8.handoverFromUTRANCommand_EUTRA_r8, activationTime))
  {
    /*copy the activation time and set the flag to true*/
    tdsprocedure_info.activation_time =
      (uint32)msg_ptr->criticalExtensions.u.r8.handoverFromUTRANCommand_EUTRA_r8.activationTime;
    tdsprocedure_info.act_time_present = TRUE;
  }
  else
  {
    /*else set activation time to false*/
    tdsprocedure_info.act_time_present = FALSE;
  }

  /* Extract the transaction identifier from the command */
  tdsprocedure_info.transaction_id = msg_ptr->rrc_TransactionIdentifier;

  /* Fail handover if Ordered Config is set */
  if(TDSOC_NOT_SET != tdsrrcllc_get_ordered_config_status())
  {
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;
    return (FALSE);
  }

  /* Store handoverFromUTRANCommand_EUTRA container as a DSM ptr */
  container_size = (uint16)msg_ptr->criticalExtensions.u.r8.handoverFromUTRANCommand_EUTRA_r8.eutra_Message.numocts;

  tdsprocedure_info.parms.eutra.dsm_ptr = NULL;
  num_bytes = dsm_pushdown (&tdsprocedure_info.parms.eutra.dsm_ptr, 
      (void *)msg_ptr->criticalExtensions.u.r8.handoverFromUTRANCommand_EUTRA_r8.eutra_Message.data, 
      container_size, 
      TDSRRC_DSM_POOL_ID(container_size));

  if((num_bytes != container_size) || (tdsprocedure_info.parms.eutra.dsm_ptr == NULL))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L_PSHO: dsm_pushdown failed num_bytes: (%d), TD2L HO failed!", num_bytes, 0, 0);
    tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;
    return FALSE;
  }

  /* Store the container as it we need to sent it in HO failure message */
  tdsprocedure_info.parms.eutra.eutra_container_size = (uint32)container_size;
  tdsprocedure_info.parms.eutra.eutra_container_ptr = (byte *) tdsrrc_malloc(container_size);
  memscpy(tdsprocedure_info.parms.eutra.eutra_container_ptr,
  	     (size_t)tdsprocedure_info.parms.eutra.eutra_container_size,
         msg_ptr->criticalExtensions.u.r8.handoverFromUTRANCommand_EUTRA_r8.eutra_Message.data,
         (size_t)container_size );

  return TRUE;

}

/*===========================================================================

FUNCTION   TDSRRCIHO_PROCESS_HANDOVER_FROM_UTRAN_COMMAND_EUTRA

DESCRIPTION

  This function verifies and processes the handover to EUTRAN from UTRAN
  command. It initiates an inter-RAT handover procedure if it is required.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_handover_from_utran_command_eutra
(
  tdsrrc_DL_DCCH_Message    *dcch_ptr,
    /* Pointer to the DCCH message. */
  
  tdsrrc_cmd_type	   *cmd_ptr
    /* Received command. */
)
{
  tdsrrc_HandoverFromUTRANCommand_EUTRA *msg_ptr;
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rvd Handover From UTRAN Command EUTRA", 0, 0, 0);
  
  msg_ptr = &(dcch_ptr->message.u.handoverFromUTRANCommand_EUTRA);
  
  /* Verify the handover from UTRAN command */
  if ((tdsrrciho_verify_and_extract_handover_from_utran_command_eutra(msg_ptr) == FALSE) ||
  	  (!TDSCHECK_OPTIONAL_FEATURE_ENABLE_TD2L_PSHO))
  {
    if (!TDSCHECK_OPTIONAL_FEATURE_ENABLE_TD2L_PSHO)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L PSHO feature not enabled!", 0, 0, 0);
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;
    }
    /* Send handover from UTRAN faliure */
    tdsrrciho_send_handover_from_utran_failure();
  }
  else
  {
    /* Check if RRC is in CELL_DCH state */
    if (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
    {
      /* RRC is not the correct state to process the command, send failure to UTRAN */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
      tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState;
      tdsrrciho_send_handover_from_utran_failure();
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"UE is not in CELL_DCH", 0, 0, 0);
      return;
    }
    
    /* Check if UE is dual-mode enabled */
    if (tdsrrcmcm_is_lte_mode_enabled() == FALSE)
    {
      /* UE is not enabled for dual-mode, send failure to UTRAN */
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;
      tdsrrciho_send_handover_from_utran_failure();
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "UE is not dual-mode enabled", 0, 0, 0);
      return;
    }

    /* Check if TDSRRC_ABORT_REQ is processing*/
    if (TRUE == tdsrrcrce_get_rrc_abort_cnf_status())
    {
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_physicalChannelFailure;
      tdsrrciho_send_handover_from_utran_failure();
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "TDSRRC_ABORT_REQ is processing, return physical channel failure ", 0, 0, 0);
      return;
    }

    /*Event logging for HO From UTRAN command*/
    tdsrrciho_event_log_handover_from_utran( T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA );

    if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
    {
      tdsrrccsp_cleanup_bplmn_state();
    }

    /* Request MCM to change the RRC mode from Active to Standby */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_STANDBY);

    if(tdsrrcsib_is_current_event_sib_read_in_dch())
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "Force SIB Init in DCH ",0,0,0);
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH_NO_PCCPCH_DROP);
    }
 
    /* Change the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_STANDBY_CNF;
    return;
  }
}
#endif



/*===========================================================================

FUNCTION   PROCESS_INTERRAT_HANDOVER_CNF

DESCRIPTION

  This function processes the interrat handover confirmation received from the
  GSM RR and initiates the proper actions.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_interrat_handover_cnf(
  tdsrr_interrat_handover_cnf_type interrat_ho_cnf /* Received command */
)
{
  uint8 number_of_commands;
  uint8 idx =0;
  tdsrrc_GSM_MessageList *gsm_command_ptr;

  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;

  /* Check if the status is successful */
  if (interrat_ho_cnf.handover_status == TDSRRC_RR_SUCCESS)
  {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
    /* Abort Dormancy if IHO proceed to successful processing. */
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco(TDSRRC_PROCEDURE_IHO);
#endif   
    /* Interrat handover succeeded: inform DT to forward bufferred NAS signaling
       messages to the GSM RR */
    tdsrrciho_send_nas_data_transfer_ind();

    /* Log inter-RAT handover end */
    tdsrrciho_event_log_handover_from_utran_end(TRUE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_MAX);

    /* Procedure ends */
    tdsrrciho_init_data();
  }
  else
  {
    /* TDS Voice Adapter needs to know the SELECT OWNER and it doesn't have any params */

    tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SELECT_OWNER);

    tdsrrc_tdrm_exchange_success = FALSE;
    if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
    {
      tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G: TDRM exchange success for T2G Handover cnf failure");
    }

    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
      tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
      wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_TRAFFIC,100,TRUE);
      tdsrrcwrm_stop_lock_wait_timer();
      if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G Handover cnf failure, wrm_lock_status: %d", wrm_lock_status);
      }
    }
    /* Inter-RAT handover failed: Resume the TDS mode and revert back to
       UTRAN configuration. */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

    /* Set the failure cause */
    switch(interrat_ho_cnf.failure_cause)
    {
      /* Invalid Configuration */
      case TDSRRC_RR_INVALID_CONFIGURATION:
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_configurationUnacceptable;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
        break;

    /* Invalid RR state or GSM protocol error */
    case TDSRRC_RR_INVALID_STATE:
    case TDSRRC_RR_GSM_PROTOCOL_ERROR:
      tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_interRAT_ProtocolError;
      /* Save the inter-RAT message */
      number_of_commands = interrat_ho_cnf.gsm_command_list.number_of_commands;

#if 0 //Below code is useless as uint8 is not possiblely bigger than 256
      for(idx = 0; (idx < number_of_commands)&&(idx < TDSRR_MAX_GSM_COMMANDS);idx++)
      {
        if (interrat_ho_cnf.gsm_command_list.gsm_commands[idx].message_length > TDSRR_MAX_GSM_MESSAGE_LENGTH)
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"The HO CNF message exceed the MAX length.",0,0,0);
          tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
          tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
          return;
        }
      }
#endif
      idx = 0;
      if (number_of_commands > 0)
      {
        tdsgsm_commands.elem = tdsrrc_malloc(number_of_commands* sizeof(tdsrrc_GSM_MessageList_element));
        if (tdsgsm_commands.elem == NULL)
        {
          /* Error: can't allocate memory for the GSM message list */
          ERR_FATAL("Can't allocate memory for GSM Msg List", 0, 0, 0);
        }
        else
        {
          /* Extract and save original GSM commands */
          gsm_command_ptr = &tdsgsm_commands;

          /*if more than one command is present then scroll thru the list of messages and add it to the
         gsm message list*/
          while ((idx < number_of_commands) && (idx < TDSRR_MAX_GSM_COMMANDS))
          {
         

              /*set the length in bits*/
              gsm_command_ptr->elem[idx].numbits = interrat_ho_cnf.gsm_command_list.gsm_commands[idx].message_length * 8;



              memscpy(gsm_command_ptr->elem[idx].data, sizeof(gsm_command_ptr->elem[idx].data), interrat_ho_cnf.gsm_command_list.gsm_commands[idx].payload,
                     (size_t)interrat_ho_cnf.gsm_command_list.gsm_commands[idx].message_length);
              idx++;
            }
          }
          gsm_command_ptr->n = number_of_commands;
      }
      
      /* Log inter-RAT handover end */
      tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
      
      break;

      /* Physical channel setup failure */
      case TDSRRC_RR_PHYSICAL_CHANNEL_FAILURE:
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_physicalChannelFailure;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_PHY_CHANNEL_CONFIG_FAILURE);
        break;

      default:
        /* Invalid failure cause */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid Failure Cause for IHO:%d", interrat_ho_cnf.failure_cause, 0, 0);
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_unspecified;

        /* Log inter-RAT handover end */
        tdsrrciho_event_log_handover_from_utran_end(FALSE, TDSRRCIHO_INTERRAT_HO_FAILURE_CAUSE_CONFIG_NOT_SUPPORTED);
        break;
    }

#ifdef FEATURE_QSH_EVENT_METRIC
    TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_IRAT_HO_FAILURE);
    tdsrrcqsh_update_ho_failure(SYS_HANDOVER_FAILURE_T_TO_G_FAILURE);
#endif

    /* Set the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
  }
  return;

} /* end of tdsrrciho_process_interrat_handover_cnf */



#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
/*===========================================================================

FUNCTION    tdsrrciho_log_ho_to_utran_cmd 

DESCRIPTION
    This function will log HandoverToUTRAN OTA messages into QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrciho_log_ho_to_utran_cmd
(
  byte* ho_msg_ptr,
  uint16 ho_msg_len
)
{
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  tdscdma_rrc_signalling_message_log_packet_type *log_record_ptr;
                                            /* Local variable to store Log
                                               record */
 



  /*Calculate the record length */
  log_record_length = 
    FPOS(tdscdma_rrc_signalling_message_log_packet_type, signalling_message) + ho_msg_len;

  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_signalling_message_log_packet_type *)log_alloc( TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET, log_record_length);

  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    log_record_ptr->version = TDSRRC_LOG_VERSION;

    /* Update RB Id.*/
    log_record_ptr->rb_id = 0xFF;   /* Variable for radio bearer ID, make it 255 for HOtoUtranCmd */

    /*Update Logical Channel Type */
    log_record_ptr->channel_type = TDSRRCLOG_SIG_HANDOVERToUTRANCOMMAND;

    /* Update length of the signalling message */
    if(ho_msg_len <= TDSRRCLOG_MAX_SIG_MSG_SIZE)
    {
      log_record_ptr->signalling_msg_length = ho_msg_len;
    }
    else
    {
      ERR_FATAL("dl_sdu_length > TDSRRCLOG_MAX_SIG_MSG_SIZE!", 0, 0, 0);
    }

    /* Copy the Signalling mesaage payload to the log record */
    memscpy((void *)log_record_ptr->signalling_message, TDSRRCLOG_MAX_SIG_MSG_SIZE,
           ho_msg_ptr,
           log_record_ptr->signalling_msg_length);

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Logging HANDOVERToUTRANCOMMAND LOG_PACKET, rb_id:%d, length:%d, first byte:0x%X", 
                log_record_ptr->rb_id
                , log_record_ptr->signalling_msg_length
                , log_record_ptr->signalling_message[0]);
    /*commit the log record */
    log_commit(log_record_ptr);
  }
}

/*===========================================================================

FUNCTION   PROCESS_HANDOVER_TO_UTRAN_COMMAND

DESCRIPTION

  This function processes the handover to UTRAN command. It
  decodes the HandoverToUTRANCommand and checks all the IE's
  and starts the process of acquiring a WCDMA cell.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_process_handover_to_utran_command_from_eutran(
  tdsrrc_cmd_type * cmd_ptr
)
{

  byte*  ho_msg_ptr = NULL;                     /* ptr to encoded HO msg     */
  tds_rrc_lte_psho_req_type *lte_psho_req = 
    &cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_lte_psho_req;

  dsm_item_type *dsm_ptr;
  msgr_attach_struct_type *msgr_attach_ptr;
  uint16 dsm_pkt_len, len;
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
  boolean irat_ex_success;
  boolean irat_tx_ex_success;
  trm_reason_enum_t tdsrrc_synced_reason_tmp;
#endif

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD_PSHO:Rcvd HO To UTRAN from EUTRAN", 0, 0, 0);

  /* TDS Voice Adapter needs to know the SELECT OWNER and it doesn't have any params */
    tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SELECT_OWNER);

  tdsrrciho_interrat_type = TDSRRCIHO_INTERRAT_LTE;

  /*reset any previous saved security keys */
  memset(g2td_info.psho_security_keys.ck, 0, 
         LTE_IRAT_MAX_CK_LENGTH);
  memset(g2td_info.psho_security_keys.ik, 0, 
         LTE_IRAT_MAX_IK_LENGTH);  
  memset(g2td_info.psho_security_keys.ck_srvcc, 0, 
         LTE_IRAT_MAX_CK_LENGTH);
  memset(g2td_info.psho_security_keys.ik_srvcc, 0, 
         LTE_IRAT_MAX_IK_LENGTH); 

#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
  /*init the default config here.. we have to do this for every call
    because the spoof poke byte can be set at any time*/
  tdsrrcllc_init_default_configuration();
#endif

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_update_rat_pri_list_info(&lte_psho_req->rat_pri_list_info);
#endif

  g2td_info.network_select_mode = lte_psho_req->network_select_mode;

  /* Extract HO continer from DSM pointer */
  msgr_attach_ptr = msgr_get_attach((msgr_hdr_struct_type *)&lte_psho_req->msg_hdr, 0);
  ASSERT(msgr_attach_ptr != NULL);
  msgr_get_dsm_attach(msgr_attach_ptr, &dsm_ptr);
  ASSERT(dsm_ptr != NULL);
    
  dsm_pkt_len = dsm_length_packet(dsm_ptr);
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD:HO container len %d ",dsm_pkt_len,0,0);
    
  g2td_info.ho_msg_len = dsm_pkt_len;
  ho_msg_ptr = (byte *) tdsrrc_malloc(dsm_pkt_len);
  if( ho_msg_ptr == NULL )
  {
    ERR_FATAL("TDSRRCIHO: Failed to allocate memory ",0,0,0);
  }
  
  len = dsm_pullup(&dsm_ptr, ho_msg_ptr, dsm_pkt_len);
  ASSERT(len == dsm_pkt_len);

  /*Log the HandoverToUtranCommand before decoding*/
  tdsrrciho_log_ho_to_utran_cmd(ho_msg_ptr, dsm_pkt_len);

  /*get the ptr to the byte stream of the HandoverToUTRANCommand*/
  g2td_info.ho_ptr =
      (tdsrrc_HandoverToUTRANCommand *) tdsrrcasn1_decode_pdu(  (void *)ho_msg_ptr,
                                                          tdsrrc_HandoverToUTRANCommand_PDU,
                                                          g2td_info.ho_msg_len
                                                          );
#ifdef FEATURE_QSH_EVENT_METRIC
  tdsrrcqsh_save_ota(ho_msg_ptr, 
                     g2td_info.ho_msg_len, 
                     TDSRRC_QSH_HO_TO_UTRAN, 
                     0xFF);
#endif

  if(ho_msg_ptr != NULL)
  {
    tdsrrc_free(ho_msg_ptr);
  }

  if(g2td_info.ho_ptr == NULL)
  {
    /*unable to successfully decode a asn1 PDU, set failure cause to
      Protocol_Error
     */
    tdsprocedure_info.failure_cause = (uint32)TDSRRC_TDS_PROTOCOL_ERROR;

    /*set to flag that HO to UTRAN msg ptr is not present*/
    g2td_info.ho_msg_present = FALSE;

    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unable to decode HO to Utran Msg",0,0,0);

    /*send a handover to utran failure and reset IHO procedure's state
      machine*/
    tdsrrciho_send_l2td_ps_handover_to_utran_failure();
    return;
  }
  else
  {
    /*set the flag to indicate that HO to UTRAN msg is present*/
    g2td_info.ho_msg_present = TRUE;
  }

  /* Verify the handover from UTRAN command */
  if (tdsrrciho_verify_and_extract_handover_to_utran_command(g2td_info.ho_ptr) == FAILURE)
  {
    /*send a handover to utran failure and reset IHO procedure's state
      machine*/
    /*the HandoverToUTRANCommand message ptr is freed in the function*/
    tdsrrciho_send_l2td_ps_handover_to_utran_failure();

    tdsrrc_free_dynamic_ptrs();
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
    {
      tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
    }
#endif
  }
  else
  {
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
    if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
    {
      tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: TDRM exchange success for psho");
      tdsrrc_synced_reason = tdrm_get_reason();
      tdsrrc_synced_reason_tmp = tdsrrc_synced_reason;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: inherited priority %d from source RAT",tdsrrc_synced_reason);
      tdsrrc_tdrm_exchange_success = TRUE;
    }
    else
    {
      tdsrrc_tdrm_exchange_success = FALSE;
      /* source RAT reason not available, apply default */
      if(tdsrrc_csfb_call_status)
      {
        tdsrrc_synced_reason_tmp = TRM_TRAFFIC;
      }
      else
      {
        tdsrrc_synced_reason_tmp = TRM_BG_TRAFFIC;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: TDRM exchange fail for psho, force inherited priority to %d",tdsrrc_synced_reason_tmp);
    }
    /* Even after TDSCDMA has received the lock, call the below API to update TDSRRC client lock status 
          For non CSFB, acquire lock here */
    if(tdsrrciho_get_lock_for_iho(TRUE) == FAILURE)
    {
      return;
    }

    tdsrrc_synced_reason_tmp = tdrm_get_reason();
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: current priority %d after tdsrrciho_get_lock_for_iho",tdsrrc_synced_reason_tmp);
    tdrm_retain_lock(TDRM_CLIENT_RRC);
    //tdrm_sync_reason(tdsrrc_synced_reason_tmp);
#endif

    memscpy(g2td_info.psho_security_keys.ck,
		   sizeof(g2td_info.psho_security_keys.ck),
           lte_psho_req->security_keys.ck,
           (size_t)LTE_IRAT_MAX_CK_LENGTH );
    memscpy(g2td_info.psho_security_keys.ik,
		   sizeof(g2td_info.psho_security_keys.ik),
           lte_psho_req->security_keys.ik,
           (size_t)LTE_IRAT_MAX_IK_LENGTH );
    memscpy(g2td_info.psho_security_keys.ck_srvcc,
		   sizeof(g2td_info.psho_security_keys.ck_srvcc),
           lte_psho_req->security_keys.ck_srvcc,
           (size_t)LTE_IRAT_MAX_CK_LENGTH );
    memscpy(g2td_info.psho_security_keys.ik_srvcc,
		   sizeof(g2td_info.psho_security_keys.ik_srvcc),
           lte_psho_req->security_keys.ik_srvcc,
           (size_t)LTE_IRAT_MAX_IK_LENGTH );

    g2td_info.is_x2td_irat_ho = TRUE;
    /*process measurement data received in the handover to utran command*/
    tdsrrciho_process_meas_data_for_handover_to_utran_command(&lte_psho_req->interrat_measurement_data);


    /*g2td_info.irat_ho_type = TDSINTERRAT_HO_TYPE_PS;*/

    /* Request MCM to change the RRC mode Inactive to Interrat in Progress */
    tdsrrciho_send_change_mode_req(TDSRRC_MODE_INTERRAT_IN_PROGRESS);

    /* Change the procedure state to wait for change mode confirmation */
    tdsrrciho_state = TDSRRCIHO_WAIT_IRAT_IN_PROGRESS_CNF;
  }

  /*Set cipher status depending on cipher algo present or not and whether cipher NV is set*/
  if(g2td_info.ciph_algo_present && tdsnv_ciphering_enabled)
  {
    g2td_info.gsm_ciph_status = TRUE;
  }
  else
  {
    g2td_info.gsm_ciph_status = FALSE;
  }

  return;
} /* end of process_handover_to_utran_command*/

#endif

/*===========================================================================

FUNCTION   RRCIHO_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the IHO is in the TDSRRCIHO_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_initial_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr /* Received command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr;      /* Pointer to the DCCH message */

  /* Check the type of the command received */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_DOWNLINK_SDU_IND:
      /* Get the downlink message from the SDU */
      dcch_msg_ptr=(tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

      /* Check if the ASN1 decoding was successful */
      if( NULL == dcch_msg_ptr )
      {
        tdsprocedure_info.failure_cause = T_tdsrrc_InterRAT_HO_FailureCause_protocolError;
        tdsprocedure_info.protocol_error = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        tdsrrciho_send_handover_from_utran_failure();
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Handover to UTRAN failed because of ASN1 decoding error ", 0, 0, 0);
        return;
      }
      /* Process the DCCH message: Handover From UTRAN Command.
      */
      switch ( dcch_msg_ptr->message.t )
      {
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
          tdsrrciho_process_handover_from_utran_command_gsm(dcch_msg_ptr, cmd_ptr);
          break;

#ifdef FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
          tdsrrciho_process_handover_from_utran_command_eutra(dcch_msg_ptr, cmd_ptr);
          break;
#endif

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid handover command %d", dcch_msg_ptr->message.t, 0, 0 );
          break;

      } /* switch ( dcch_msg_ptr->message.t ) */
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
      break;

    case TDSRRC_ABORT_REQ:
      /* IHO is not active, forward the command to RCE for processing */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"ABORT is forwarded to RCE", 0, 0, 0);
      tdsrrcrce_procedure_event_handler( cmd_ptr );
      break;

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
/*lint -e408*/
    case TDSRRC_INTERRAT_HANDOVER_REQ:
/*lint +e408*/
      /* Received an Interrat Handover Request from RR to HO to TDS*/
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "Handover to TDSCDMA rcvd.", 0, 0, 0);
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
      /*Send G2W started to CM*/
      tdsrrc_send_hho_cmd_to_cm(SYS_HO_START);
#endif
      tdsrrciho_process_handover_to_utran_command( (tdsrrc_rr_cmd_type *)cmd_ptr);

      break;

#endif

#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
          TDSCDMA_RRC_LTE_PSHO_REQ)
      {
        if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_L2TD_PSHO)
        {
          tdsrrciho_process_handover_to_utran_command_from_eutran(cmd_ptr);
        }
        else
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_TDS_PROTOCOL_ERROR;

          /*set to flag that HO to UTRAN msg ptr is not present*/
          g2td_info.ho_msg_present = FALSE;

          TDSRRC_MSG3(MSG_LEGACY_ERROR,"L2TD PSHO feature not enabled!",0,0,0);

          /*send a handover to utran failure and reset IHO procedure's state machine*/
          tdsrrciho_send_l2td_ps_handover_to_utran_failure();
        }
      }
      else if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        /*process this race condition:
            1. receive PSHO_REQ, ACQ or channel config failure, enter inactive, send PSHO_RSP
            2. receive ABORT_PSHO_REQ.*/
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);
        tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf();
      }
      break;
#endif

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
      /*process this race condition:
          1. receive PSHO_REQ, ACQ or channel config failure, enter inactive, send PSHO_RSP
          2. receive ABORT_PSHO_REQ.*/
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);
      tdsrrciho_send_handover_to_utran_abort_cnf();
      break;
#endif

    default:
      /* umexpected enevts */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected IHO CMD %x in substate %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
}



/*===========================================================================

FUNCTION   RRCIHO_WAIT_STANDBY_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the IHO procedure is in the RRCMCM_WAIT_STANDBY_CNF substate.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_standby_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Change mode confirmation from the MCM */
    case TDSRRC_CHANGE_MODE_CNF:
      /* Check if abort is received */
      if (tdsrrciho_abort == TRUE)
      {
        /* Resume the TDS operation */
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

        /* Set the procedure state to wait for change mode confirmation */
        tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
      }
      else
      {
        /* Get inter-RAT measurement data from L1 is done in tdsrrciho_process_handover_from_utran_command_gsm*/


        /* Initiate inter-RAT handover.
        */

        if(!tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"DSIM: RRC does not have lock before sending T2X HO req!",0,0,0);          
        }
		if(
            (tds_is_ue_in_dual_standby) && 
            (tdsrrciho_is_iho_td2g_active()))
        {
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
		  if(tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_BG_TRAFFIC for T2L PS Handover");
            tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC,TRUE);
            tdsrrc_trm_exchange_out(TDSRRC_CSP_INTERRAT_LTE);
            tdsrrcwrm_release_lock();
          }
#endif
          if(tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_TRAFFIC for T2G CS Handover");
            tdsrrc_wrm_change_priority(TRM_TRAFFIC,TRUE);
            tdsrrc_trm_exchange_out(TDSRRC_CSP_INTERRAT_GSM);
            tdsrrcwrm_release_lock();
          }
        }

        tdsrrciho_send_interrat_handover_req();


        /* Change the procedure state to wait for handover confirmation */
        tdsrrciho_state = TDSRRCIHO_WAIT_INTERRAT_HO_CNF;
      }
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
      /* RRC state change out of DCH indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      tdsrrciho_init_data();

      break;

    case TDSRRC_ABORT_REQ:
      /* Set abort to true */
      tdsrrciho_abort = TRUE;
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",
           cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
}




/*===========================================================================

FUNCTION   RRCIHO_WAIT_INTERRAT_HO_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for inter-RAT handover Confirmation substate.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_interrat_ho_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Inter-RAT handover confirmation from the GSM RR */
/*lint -e408*/
    case TDSRR_INTERRAT_HANDOVER_CNF:
/*lint +e408*/
      /* Process the interrat handover confirmation command */
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "Inter-RAT HO CNF received from RR", 0, 0, 0);
/*lint -e740*/
      tdsrrciho_process_interrat_handover_cnf(((tdsrrc_rr_cmd_type *)cmd_ptr)->cmd.interrat_handover_cnf);
/*lint +e740*/
      break;

#ifdef FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == LTE_RRC_TDSCDMA_PSHO_RSP)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L: Inter-RAT HO CNF received from LRRC with status: %d", 
                  cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.lte_rrc_tdscdma_psho_rsp.ho_status, 0, 0);
        tdsrrciho_process_td2l_ps_handover_cnf(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.lte_rrc_tdscdma_psho_rsp);
      }
      break;
#endif

    case TDSRRC_STATE_CHANGE_IND:
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
      /* RRC state change to IDLE indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      tdsrrciho_init_data();

      break;

    case TDSRRC_ABORT_REQ:
      /* Request RR to abort the inter-RAT handover.
      */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"ABORT rvd, Abort Inter-RAT HO", 0, 0, 0);
      tdsrrciho_abort = TRUE;
      switch ( tdsprocedure_info.handover_type )
      {
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
          tdsrrciho_send_abort_handover_req();
          tdsrrciho_state = TDSRRCIHO_WAIT_ABORT_CNF;
          break;

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_TD2L_PSHO_SUPPORT)
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
          tdsrrciho_send_abort_handover_to_lte();
          tdsrrciho_state = TDSRRCIHO_WAIT_ABORT_CNF;
          break;
#endif
        default:
          /* Resume TDS operation.
          */
          tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

          /* Set the procedure state to wait for change mode confirmation.
          */
          tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
          break;
      } /* switch() */
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",
           cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
}



/*===========================================================================

FUNCTION   RRCIHO_WAIT_ACTIVE_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for active Confirmation substate.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_active_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  tdsrrc_cmd_type abort_cmd;
  /*cmd ptr to send initiate_Cell_update_req*/
  tdsrrc_cmd_type *int_cmd_ptr;  

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Inter-RAT handover confirmation from the GSM RR */
    case TDSRRC_CHANGE_MODE_CNF:
      TDSRRC_MSG3(MSG_LEGACY_MED,"Change Mode CNF received from MCM", 0, 0, 0);

      /* Check if abort is true */
      if (tdsrrciho_abort == TRUE)
      {
        /* Send abort request to RCE for processing */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"ABORT Rvd, Process Abort in RCE", 0, 0, 0);
        abort_cmd.cmd_hdr.cmd_id = TDSRRC_ABORT_REQ;
        tdsrrcrce_procedure_event_handler(&abort_cmd);

        /* Reset procedure data */
        tdsrrciho_init_data();
      }
      else
      {
        if( cmd_ptr->cmd.change_mode_cnf.status == TRUE )
        {
          //if we are resuming, and voice rab's are present in est-rabs, then we need to indicate
          //to l1 of a change in power-class, if the feature is defined
          if(TRUE == tdsrrc_is_voice_rab_present())
          {
            //call wrapper fn. that calls L1 to lower it's pwr class to 4 for voice
            tdsrrc_indicate_layer1_next_config_is_for_voice();
          }
          /* Send Channel Config Request to LLC to revert back to UTRAN configuration */
          tdsrrciho_send_channel_config_req_to_llc();

          /* Set procedure state to wait for Channel Config Confirmation */
          tdsrrciho_state = TDSRRCIHO_WAIT_CHANNEL_CONFIG_CNF;
        }
        else /* change mode failure */
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANGE_MODE_CNF (Failure) ", 0, 0, 0);

          /* Initiate a cell update with Radio link failure */
          if ( (int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
          {
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
            int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_RADIO_LINK_FAILURE;
            int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
            int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_IHO;

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by IHO on cphy resume failure",0,0,0);
            tdsrrc_put_int_cmd(int_cmd_ptr);

            tdsrrciho_state = TDSRRCIHO_WAIT_CELL_UPDATE_CNF;
          }
          else
          {
            ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ", 0, 0, 0);
          }
        }
      }
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
      /* RRC state change to IDLE indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      tdsrrciho_init_data();

      break;

    case TDSRRC_ABORT_REQ:
      /* Set abort indicator to TRUE */
      tdsrrciho_abort = TRUE;
      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",
           cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_active_cnf_substate_event_handler */



/*===========================================================================

FUNCTION   RRCIHO_WAIT_ABORT_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for abort Confirmation substate.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_abort_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  tdsrrc_out_of_lock_wait_e_type wrm_lock_status;
  boolean irat_ex_success;
  boolean irat_tx_ex_success;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* Inter-RAT handover confirmation from the GSM RR */
/*lint -e408*/
    case TDSRR_INTERRAT_HANDOVER_ABORT_CNF:
/*lint +e408*/
      /* Resume TDS operation */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"ABORT_CNF Rvd from RR", 0, 0, 0);
      
      tdsrrc_tdrm_exchange_success = FALSE;
      if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
      {
        tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G: TDRM exchange success for T2G Handover Abort cnf");
      }
 
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
        tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
        wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_TRAFFIC,100,TRUE);
        tdsrrcwrm_stop_lock_wait_timer();
        if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2G Handover Abort cnf, wrm_lock_status: %d", wrm_lock_status);
        }
      }
      tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);

      /* Set the procedure state to wait for change mode confirmation */
      tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;

      break;

#ifdef FEATURE_TDSCDMA_TO_LTE
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == LTE_RRC_TDSCDMA_ABORT_PSHO_RSP)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"TD2L: ABORT_CNF Rvd from LTE-RRC", 0, 0, 0);

#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
        tdsrrc_tdrm_exchange_success = FALSE;
        if(tdsrrc_trm_exchange(&irat_ex_success, &irat_tx_ex_success))
        {
          tdscfa_dsds_add_event(TDSRRC_DSDS_EXCHANGE_LOCK);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L: TDRM exchange success for T2L Handover Abort cnf");
        }

        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: R&N after TRM_exchange, exchange flag: (%d, %d)",irat_ex_success, irat_tx_ex_success);
          tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_INTERRAT_IN_MS,TRUE);
          wrm_lock_status = tdsrrcwrm_acquire_wrm_lock(TRM_BG_TRAFFIC,100,TRUE);
          tdsrrcwrm_stop_lock_wait_timer();
          if(wrm_lock_status != TDSRRC_WRM_LOCK_ACQUIRED)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC cannot acquire lock before resume L1 on T2L Handover Abort cnf, wrm_lock_status: %d", wrm_lock_status);
          }
        }
#endif
        /* Resume TDSCDMA operation */
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_ACTIVE);
        
        /* Set the procedure state to wait for change mode confirmation */
        tdsrrciho_state = TDSRRCIHO_WAIT_ACTIVE_CNF;
      }
      break;
#endif

    case TDSRRC_STATE_CHANGE_IND:
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
      /* RRC state change to IDLE indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      tdsrrciho_init_data();

      break;

    default:
      /* No other events are normal in this
      substate */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected event %x in state %d",
             cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_abort_cnf_substate_event_handler */


/*===========================================================================

FUNCTION   RRCIHO_WAIT_CHANNEL_CONFIG_CNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the Wait for channel configuration confirmation substate.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_channel_config_cnf_substate_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  tdsrrc_cmd_type abort_cmd;
  tdsrrc_cmd_type *int_cmd_ptr;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_CHANNEL_CONFIG_CNF */
    case TDSRRC_CHANNEL_CONFIG_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF", 0, 0, 0); 

      /* Check the status of the channel config confirmation */
      if (cmd_ptr->cmd.chan_config_cnf.chan_cfg_status == TDSRRCLLC_CHAN_CFG_SUCCESS)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF (Success) ", 0, 0, 0);

        /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
        tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();


        /* Revert back to UTRAN is successful, send Handover from UTRAN failure */
        tdsrrciho_send_handover_from_utran_failure();

#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS	   
        if(tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
        {
          tdrm_retain_lock(TDRM_CLIENT_RRC);  
        }
#endif
        /* Check if Abort is received */
        if (tdsrrciho_abort == TRUE)
        {
          /* Invoke RCE to process the TDSRRC_ABORT_REQ */
          abort_cmd.cmd_hdr.cmd_id = TDSRRC_ABORT_REQ;
          tdsrrcrce_procedure_event_handler(&abort_cmd);
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Channel config success,ABORT Rvd,forwarding it to RCE", 0, 0, 0);
        }

        /* Reset the procedure data, the procedure ends */
        tdsrrciho_init_data();
      }
      else // Reverting to UTRAN failure
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF (Failure) ", 0, 0, 0);
        /* If there is a pending abort,forward it to RCE and end the procedure */
        if (tdsrrciho_abort == TRUE)
        {
          /* Invoke RCE to process the TDSRRC_ABORT_REQ */
          abort_cmd.cmd_hdr.cmd_id = TDSRRC_ABORT_REQ;
          tdsrrcrce_procedure_event_handler(&abort_cmd);

          /* Reset the procedure data, the procedure ends */
          tdsrrciho_init_data();
        }

        /* Reverting to UTRAN fails, UE should perform cell update procedure */
        else
        {
          if ( (int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
          {
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
            int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_RADIO_LINK_FAILURE;
            int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
            int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_IHO;

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by IHO on resume failure",0,0,0);
            tdsrrc_put_int_cmd(int_cmd_ptr);

            tdsrrciho_state = TDSRRCIHO_WAIT_CELL_UPDATE_CNF;
          }
          else
          {
            ERR_FATAL("No CMD buf CELL_UPDATE_INITIATE_REQ", 0, 0, 0);
          }
        }
      }
      break;

    /* State change Indication */
    case TDSRRC_STATE_CHANGE_IND:
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);

      /* RRC state change to IDLE indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      tdsrrciho_init_data();

      break;

    /* TDSRRC_ABORT_REQ */
    case TDSRRC_ABORT_REQ:
      /* Set abort to true */
      tdsrrciho_abort = TRUE;
      break;

    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_channel_config_cnf_substate_event_handler */


#if defined (FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*===========================================================================

FUNCTION   tdsrrciho_wait_acq_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Acquisition Confirmation from L1

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_acq_cnf_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_CSP_ACQ_CNF */
    case TDSRRC_CSP_ACQ_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_CSP_ACQ_CNF", 0, 0, 0);
      /* if an abort command is received, send a handover failure stop
         the handover procedure and initialize all handover data*/
      if( ( tdsrrciho_abort == TRUE ) ||
          ( cmd_ptr->cmd.rrc_csp_acq_cnf.acq_status == FAILURE) )
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSP_ACQ_CNF FAILED", 0, 0, 0);
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);

        tdsprocedure_info.failure_cause = (uint32)TDSRRC_PHYSICAL_CHANNEL_FAILURE;
        tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
      }
      else
      {

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSP_ACQ_CNF SUCCESS", 0, 0, 0);

#ifdef FEATURE_LTE_TO_TDSCDMA
          if(g2td_info.is_cs_rab_present)
          {
            if((TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type)
            )
            {
              //call wrapper fn. that calls L1 to lower it's pwr class to 4 for voice
              tdsrrc_indicate_layer1_next_config_is_for_voice();
            }
            /* Update mac_rab_Status blindly to CS_RABS since IHO can set up only CS rabs  */
            tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_IHO, RRC_CS_DOMAIN_CN_ID,
                                        NULL, FALSE);
          }
        
        if(g2td_info.is_ps_rab_present)
        {
          /* Update mac_rab_Status based on the type of handover */
          tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_IHO, RRC_PS_DOMAIN_CN_ID,
                                      NULL, FALSE);
        }
#else 
        /*By this time voice_config is updated.*/
        if((TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs.rab_type)
        )
        {
          //call wrapper fn. that calls L1 to lower it's pwr class to 4 for voice
          tdsrrc_indicate_layer1_next_config_is_for_voice();

        }

        if(TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs.rab_type ||
           TDSRRCRB_CS_DATA_RAB == g2td_info.est_rabs.rab_type )
        {
          /* Update mac_rab_Status blindly to CS_RABS since IHO can set up only CS rabs  */
          tdsrrcrb_update_mac_rab_status(TDSRRC_PROCEDURE_IHO, RRC_CS_DOMAIN_CN_ID,
                                      NULL, FALSE);
        }

#endif /* FEATURE_LTE_TO_TDSCDMA */

        /*send channel config req to LLC to go to DCH state, if OC set
          is not successful for any reason, go to inactive and abandon
          the Handover */
        if(tdsrrciho_send_channel_config_for_x2td() != SUCCESS)
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Ordered Config Set failed", 0, 0, 0);
          tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_PHYSICAL_CHANNEL_FAILURE;
          tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
        }

        /* By now, IHO procedure will be waiting in either RRCIHO_WAIT_CHAN_CONFIG_G2TD_CNF or 
        TDSRRCIHO_WAIT_INACTIVE_CNF substate based on whether Channel Config Req is sent or not.*/

      }

      break;

    /* TDSRRC_INTERRAT_HANDOVER_ABORT_REQ */
/*lint -e408*/
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
/*lint +e408*/
      /* Set abort to true */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);
      tdsrrciho_abort = TRUE;

      break;


#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
        TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);

        /* Set abort to true */
        tdsrrciho_abort = TRUE;

        break;
      }
#endif


    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_acq_cnf_event_handler */


/*===========================================================================

FUNCTION   tdsrrciho_wait_inactive_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Inactive Confirmation from MCM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_inactive_cnf_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_CHANGE_MODE_CNF */
    case TDSRRC_CHANGE_MODE_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_CHANGE_MODE_CNF", 0, 0, 0);

      if(tdsrrciho_abort == TRUE)
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
        {
          tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf();
        }
        else
#endif
        {
           tdsrrciho_send_handover_to_utran_abort_cnf();
         }
      }
      else
      {
#ifdef FEATURE_LTE_TO_TDSCDMA
        if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
        {
#ifdef FEATURE_LTE_TO_TDSCDMA_DSDS
          /*tdsrrc_tdrm_exchange_success will be reset to FALSE when change priority as it will release lock*/
          boolean tdsrrc_tdrm_exchange_success_backup = tdsrrc_tdrm_exchange_success;

          if (tdsrrc_tdrm_exchange_success)
          {
            /*Change TRM priority to BG_TRAFFIC before return lock to LTE */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to BG_TRAFFIC for L2T psho fail rsp");
            tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC,TRUE);
            tdsrrc_synced_reason = TRM_BG_TRAFFIC;//Don't needs to use the original synced reason, use this specific one
            //TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: unsync reason for IRAT");
            //tdrm_unsync_reason();
            /* psho is a failure hence the lock needs to be exchanged with IRAT client if TRM exchange
                      was success earlier */ 	
            tdsrrc_tdrm_exchange_success = tdsrrc_tdrm_exchange_success_backup;
            tdsrrc_check_and_perform_trm_exchange(tdsrrc_synced_reason);
            /* Lock has already been exchanged so releasing the lock will not affect anything 
                   This is just done to update WRM variables */
            tdsrrcwrm_release_lock();
          }
#endif
          if(tdsrrciho_abort == TRUE)
          {
            tdsrrciho_send_l2td_ps_handover_to_utran_abort_cnf();
          }
          else
          {
            /*send a handover to utran failure and reset IHO procedure's state
                  machine*/
            tdsrrciho_send_l2td_ps_handover_to_utran_failure();
          }
        }
        else
#endif
        {
            /*send a handover to utran failure and reset IHO procedure's state
              machine*/
            tdsrrciho_send_handover_to_utran_failure();
         }
      }
      break;
      
#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);     
      tdsrrciho_abort = TRUE;
      break;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
          TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);
        tdsrrciho_abort = TRUE;
      }
      break;
#endif

    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_inactive_cnf_event_handler */


/*===========================================================================

FUNCTION  tdsrrciho_wait_irat_in_progress_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Interrat in Progress Cnf from MCM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_irat_in_progress_cnf_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_CHANGE_MODE_CNF */
    case TDSRRC_CHANGE_MODE_CNF:

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANGE_MODE_CNF IRAT_PROG", 0, 0, 0);

      /* if an abort command is received, send change mode request to
         inactivate L1 and set IHO state to wait for inactive state*/
      if(tdsrrciho_abort == TRUE)
      {
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
        tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
      }
      else
      {
        /*send acquisition request to LLC */
        tdsrrciho_send_acq_req();

        /* Set the procedure state to wait for Acquisition confirmation */
        tdsrrciho_state = TDSRRCIHO_WAIT_ACQ_CNF;
      }

      break;

    /* TDSRRC_INTERRAT_HANDOVER_ABORT_REQ */
/*lint -e408*/
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
/*lint +e408*/
      /* Set abort to true */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);
      tdsrrciho_abort = TRUE;

      break;

#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
        TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);

        /* Set abort to true */
        tdsrrciho_abort = TRUE;

        break;
      }
#endif

    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_irat_in_progress_cnf_event_handler */
/*===========================================================================

FUNCTION  tdsrrciho_wait_ciph_config_gtow_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Ciph Cnf from SMC procedure

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_ciph_config_gtow_cnf_event_handler
(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{

#ifdef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_cmd_type *int_cmd_ptr;
#endif

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_HO_CIPH_CNF */
    case TDSRRC_HO_CIPH_CNF:

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_HO_CIPH_CNF in CIPH_CNF", 0, 0, 0);
      
      /* if an abort command is received, send change mode request to
         inactivate L1 and set IHO state to wait for inactive state*/
      if(tdsrrciho_abort == TRUE)
      {
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
        tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
      }
      else
      {
        /*send handover to utran success command to RR layer*/
#ifdef FEATURE_LTE_TO_TDSCDMA
        if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
        {
          tdsrrciho_send_l2td_ps_handover_to_utran_success();

          /* Send RRC_IHO_OPEN_DOMAIN_IND to DT */
          /* Get a RRC internal command buffer */
          if((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
          {
            ERR_FATAL("No CMD buffer for RRC_IHO_OPEN_DOMAIN_IND", 0, 0, 0);
          }

          /* Fill in the command id  */
          int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_IHO_OPEN_DOMAIN_IND;

          if(g2td_info.is_cs_rab_present && g2td_info.is_ps_rab_present)
          {
            int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_CS_PLUS_PS_DOMAIN;
          }
          else if(g2td_info.is_cs_rab_present)
          {
            int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_CS_DOMAIN;
          }
          else if(g2td_info.is_ps_rab_present)
          {
            int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_PS_DOMAIN;
          }

          /* Put the command on the internal queue */
          tdsrrc_put_int_cmd( int_cmd_ptr );

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC_IHO_OPEN_DOMAIN_IND sent to DT", 0, 0, 0);
        }
        else
#endif
        {
            tdsrrciho_send_handover_to_utran_success();
        }

        /*set Connection Setup status to true*/
        tdsrrcrce_change_substate_to_rrcrce_final();
        /* Reduce the TRM priority to BG TRAFFIC post successful L2W PS HO to facilitate lock
        * release for page read on GSM sub2. This has to be done after RCE substate is updated
        * to RRCRCE_FINAL
        */

        /* Set the procedure state to wait for activation rsp */
        tdsrrciho_state = TDSRRCIHO_WAIT_ACT_RSP;
      }

      break;

    /* TDSRRC_INTERRAT_HANDOVER_ABORT_REQ */
/*lint -e408*/
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
/*lint +e408*/
      /* Set abort to true */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);
      tdsrrciho_abort = TRUE;

      break;
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
        TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);

        /* Set abort to true */
        tdsrrciho_abort = TRUE;

        break;
      }
#endif

    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
}

/*===========================================================================

FUNCTION   tdsrrciho_event_phychan_cfg_status_handover_to_utran

DESCRIPTION

    Event Logging for PHY_CHAN cnf for HO to UTRAN command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_event_phychan_cfg_status_handover_to_utran
(
  boolean status
)
{
  /* declare an event log type for phychan cfg status*/
  tdsrrclog_phychan_cfg_status_event_type phychan_cfg_event_log;

  phychan_cfg_event_log.phychan_cfg_status = status;

  /*update freq and scrambling code info*/

  event_report_payload(EVENT_TDSCDMA_RRC_PHYCHAN_CFG_CHANGED,
                       sizeof(phychan_cfg_event_log),
                       (void *)&phychan_cfg_event_log);
}/*tdsrrciho_event_log_handover_to_utran*/

/*===========================================================================

FUNCTION   tdsrrciho_wait_chan_config_gtow_cnf_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for Chan Config confirmation from LLC

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_chan_config_gtow_cnf_event_handler(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr;  /*cmd ptr to send ciph config if ciphering
                                was enabled in GSM RAT*/
#ifdef FEATURE_LTE_TO_TDSCDMA
  uint8 j;
  tdsrrc_ho_ciph_req_type ho_ciph_req;
#endif

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_CHANNEL_CONFIG_CNF */
    case TDSRRC_CHANNEL_CONFIG_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSRRC_CHANNEL_CONFIG_CNF", 0, 0, 0);

      /* if an abort command is received, send change mode request to
         inactivate L1 and set IHO state to wait for inactive state or
         if the channel config confirmation is not successful then send a
         failure message and reset all the procedure data*/

      if( ( tdsrrciho_abort == TRUE ) ||
          ( cmd_ptr->cmd.chan_config_cnf.chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS) )
      {
        /* Why do we want to do the following. Anyway, tdsrrc_active_codec wouldn't have been updated so far.*/      
#ifndef FEATURE_LTE_TO_TDSCDMA
        g2td_info.est_rabs.nas_Synchronisation_Indicator= tdsrrc_get_current_codec_mode();
        tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs.nas_Synchronisation_Indicator);
#else
        if((g2td_info.is_cs_rab_present) &&
           (TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type))
        {
          g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator= tdsrrc_get_current_codec_mode();
          tdsrrc_set_ordered_codec_mode(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
        }
#endif /* FEATURE_LTE_TO_TDSCDMA */
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;

        /* Set the failure cause */
        if(tdsrrciho_abort == TRUE)
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
        }
        else
        {
          tdsprocedure_info.failure_cause = (uint32)TDSRRC_PHYSICAL_CHANNEL_FAILURE;
          tdsrrciho_event_phychan_cfg_status_handover_to_utran(FALSE);
        }
      }
      else
      {
        /*Notify the Cell_Id to registered entities*/
        tdsrrc_notify_cell_id_in_dch();
        
        /* Copy the Ordered RB Mapping into the current RB-Mapping database*/
        tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current();

        /*log an event in phy chan config is SUCCESSFUL*/
        tdsrrciho_event_phychan_cfg_status_handover_to_utran(TRUE);

        if(TRUE == g2td_info.rab_info_included)
        {
#ifdef FEATURE_LTE_TO_TDSCDMA
          for(j = 0; j < g2td_info.number_of_est_rabs; j++)
          {
            /*add code here to update established rabs*/
            tdsrrc_add_rab_to_established_rabs(&(g2td_info.est_rabs[j]));
          }
          if(g2td_info.is_cs_rab_present)
          {
            /*send activation command to vocoder*/
            tdsrrciho_process_channel_config_cnf(g2td_info.est_rabs[g2td_info.cs_rab_index].nas_Synchronisation_Indicator);
          }
#else
          /*add code here to update established rabs*/
          tdsrrc_add_rab_to_established_rabs(&(g2td_info.est_rabs));

          if(TDSRRCRB_CS_VOICE_RAB == g2td_info.est_rabs.rab_type)
          {
            /*send activation command to vocoder*/
            tdsrrciho_process_channel_config_cnf(g2td_info.est_rabs.nas_Synchronisation_Indicator);
          }
#endif /* FEATURE_LTE_TO_TDSCDMA */
        }

        /*clear the ordered config flag.. since we have successfully acquired and
          setup channels in TDS mode*/
        tdsrrcllc_clear_ordered_config();

        /*if ciphering is started in GSM side then we have to start ciphering
          on TDS side too.. so send Ciphering Req to SMC and wait for ciph
          config.. if Ciphering is not started on GSM side then send handover
          to UTRAN success to RR and set rce's state to final*/
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"GSM Ciph Status: %d, Ciph Algo IE Present: %d",
           g2td_info.gsm_ciph_status,
           g2td_info.ciph_algo_present,0);
        
        if((g2td_info.gsm_ciph_status != TRUE)||
           (g2td_info.ciph_algo_present == FALSE ))
        {
          /*send handover to utran success command to RR layer*/
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
          if(tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE)
          {
            tdsrrciho_send_l2td_ps_handover_to_utran_success();

            /* Send RRC_IHO_OPEN_DOMAIN_IND to DT */
            /* Get a RRC internal command buffer */
            if((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
            {
              ERR_FATAL("No CMD buffer for RRC_IHO_OPEN_DOMAIN_IND", 0, 0, 0);
            }

            /* Fill in the command id  */
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_IHO_OPEN_DOMAIN_IND;
            
            if(g2td_info.is_cs_rab_present && g2td_info.is_ps_rab_present)
            {
              int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_CS_PLUS_PS_DOMAIN;
            }
            else if(g2td_info.is_cs_rab_present)
            {
              int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_CS_DOMAIN;
            }
            else if(g2td_info.is_ps_rab_present)
            {
              int_cmd_ptr->cmd.rrc_open_domain_ind.domain = TDSRRC_PS_DOMAIN;
            }

            /* Put the command on the internal queue */
            tdsrrc_put_int_cmd( int_cmd_ptr );

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC_IHO_OPEN_DOMAIN_IND sent to DT", 0, 0, 0);
            /* Get a RRC internal command buffer */
            /*Store the ps keys only if PS rab is present and CS keys when CS rab is present*/
            if(g2td_info.is_ps_rab_present)
            {

              memscpy(ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key,
			  	   sizeof(ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
              memscpy(ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key,
                   sizeof(ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );

              tdsrrcsmc_save_security_key_set_from_lte(ho_ciph_req, RRC_PS_DOMAIN_CN_ID);
            }

            if(g2td_info.is_cs_rab_present)
            {
              memscpy(ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key,
                   sizeof(ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck_srvcc,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
              memscpy(ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key,
                   sizeof(ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik_srvcc,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );

              tdsrrcsmc_save_security_key_set_from_lte(ho_ciph_req, RRC_CS_DOMAIN_CN_ID);
            }
          }
          else
#endif
          {
              tdsrrciho_send_handover_to_utran_success();
          }

          /*set Connection Setup status to true*/
          tdsrrcrce_change_substate_to_rrcrce_final();
          



   
          /*If ciphering is not started in G then at GtoW HandOver check 
           *Check if any new keys got generated, if yes then update these
           *keys from buffer to ciphering data base, right here.
           */
          tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
   
          /* Set the procedure state to wait for activation rsp */
          tdsrrciho_state = TDSRRCIHO_WAIT_ACT_RSP;
        }
        else
        {
          /* Get a RRC internal command buffer */
          if((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
          {
            ERR_FATAL("No CMD buffer for TDSRRC_HO_CIPH_REQ", 0, 0, 0);
          }

          /* Fill in the command id  and ciphering algorithm*/
          int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_HO_CIPH_REQ;
          int_cmd_ptr->cmd.ho_ciph_req.ciph_algo = g2td_info.cipheringAlgorithm;

#ifdef FEATURE_LTE_TO_TDSCDMA
         if((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE) && 
              ((g2td_info.is_ps_rab_present)&&(g2td_info.is_cs_rab_present)))
          {
            /*As per section 8.3.6.3 since both CS and PS rabs are present set the domain id to CS.
            set the variable LATEST_CONFIGURED_CN_DOMAIN to the value indicated in the IE "CN domain
            identity" of the IE "RAB info" of the IE "RAB information to setup" if all instances of the IE indicate the
            same CN domain, or to the CS domain when this IE is either not present or different instances indicate
            different CN domains */
            
            int_cmd_ptr->cmd.ho_ciph_req.domain_id = RRC_CS_DOMAIN_CN_ID;
            int_cmd_ptr->cmd.ho_ciph_req.config_cause = TDSCIPH_CONFIG_CS_PLUS_PS;

            /*Copy the CS and PS keys*/						
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );
						
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck_srvcc,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik_srvcc,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );						
          }
          else if ((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE) && 
              (g2td_info.is_ps_rab_present))
          {
            int_cmd_ptr->cmd.ho_ciph_req.domain_id = RRC_PS_DOMAIN_CN_ID;
            int_cmd_ptr->cmd.ho_ciph_req.config_cause = CIPH_CONFIG_L2TD;
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_PS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );
          }
          else if ((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE) && 
              (g2td_info.is_cs_rab_present))		
          {
            int_cmd_ptr->cmd.ho_ciph_req.domain_id = RRC_CS_DOMAIN_CN_ID;
            int_cmd_ptr->cmd.ho_ciph_req.config_cause = TDSCIPH_CONFIG_CS;
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].ciphering_key),
                   g2td_info.psho_security_keys.ck_srvcc,
                   (size_t)LTE_IRAT_MAX_CK_LENGTH );
            memscpy(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key,
                   sizeof(int_cmd_ptr->cmd.ho_ciph_req.security_keys[RRC_CS_DOMAIN_CN_ID].integrity_key),
                   g2td_info.psho_security_keys.ik_srvcc,
                   (size_t)LTE_IRAT_MAX_IK_LENGTH );	
          }
          else if ((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_GSM) && 
              (g2td_info.is_cs_rab_present))
          {
            int_cmd_ptr->cmd.ho_ciph_req.domain_id = RRC_CS_DOMAIN_CN_ID;
            int_cmd_ptr->cmd.ho_ciph_req.config_cause = CIPH_CONFIG_G2TD;
          }
#endif

          /* Put the command on the internal queue */
          tdsrrc_put_int_cmd( int_cmd_ptr );
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_HO_CIPH_REQ sent to SMC", 0, 0, 0);

#if defined(FEATURE_G2TD_PS_HANDOVER)
          /*Currently we dont have the domain I.D support in TDSRRC_HO_CIPH_REQ, which will be introduced in PS hand over
            *So when we support PS-HandOver, we need to check whether the GtoW hand over is for PS domain, if so 
            *then check & update CS keys if any generated in other RAT here by calling below function
            *if(GtoW HandOver is for PS domain)
            *{
            *  tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
            *}
            *else
            *{
            *  donothing here
            *}
            */
#endif

          tdsrrciho_state = TDSRRCIHO_WAIT_CIPH_CONFIG_G2TD_CNF;
        }
      }

      break;

    /* TDSRRC_INTERRAT_HANDOVER_ABORT_REQ */
/*lint -e408*/
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
/*lint +e408*/
      /* Set abort to true */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);     
      tdsrrciho_abort = TRUE;

      break;

#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == 
        TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);

        /* Set abort to true */
        tdsrrciho_abort = TRUE;

        break;
      }
#endif
    default:
      /* Unexpected command */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
  return;
} /* end of tdsrrciho_wait_chan_config_gtow_cnf_event_handler */

/*===========================================================================

FUNCTION   tdsrrciho_wait_act_rsp

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for TDSRRC_ACTIVATION_RSP.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_act_rsp(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
    case TDSRRC_MSGR_CMD:
      if(cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_ABORT_PSHO_REQ)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: TDSCDMA_RRC_LTE_ABORT_PSHO_REQ", 0, 0, 0);
        tdsrrciho_abort = TRUE;
        tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
        tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
        tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
      }
      break;
#endif

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
    case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rx: RRC_INTERRAT_HANDOVER_ABORT_REQ", 0, 0, 0);
      tdsrrciho_abort = TRUE;
      tdsrrciho_send_change_mode_req(TDSRRC_MODE_INACTIVE);
      tdsprocedure_info.failure_cause = (uint32)TDSRRC_INVALID_STATE;
      tdsrrciho_state = TDSRRCIHO_WAIT_INACTIVE_CNF;
      break;
 #endif

    case TDSRRC_MODE_CHANGE_IND:
      /*When MCM received TDSRRC_ACTIVATION_RSP, it will send TDSRRC_MODE_CHANGE_IND to this module.*/
      if(cmd_ptr->cmd.mode_change_ind.to_mode == TDSRRC_MODE_ACTIVE)
      {
          
        /*send sync indication to mm layer to indicate that HO is successful
              and we need to switch the vocoder to TDS and continue to TDS call*/
        tdsrrciho_send_sync_ind_for_g2td_ho();

#ifdef FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK
        if(g2td_info.is_ps_rab_present)
        {
          tdsrrciho_state = TDSRRCIHO_WAIT_L2ACK_FOR_HO_TO_UTRAN_COMPLETE;
        }
        else
#endif /* FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK */
        {
          tdsrrciho_state = TDSRRCIHO_INITIAL;
        }
      }
      break;

    default:
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
}

#endif

/*HX TBD check if following is still needed*/

#ifdef FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK
/*===========================================================================

FUNCTION   tdsrrciho_wait_l2ack_for_ho_to_utran_complete

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for L2 ACK for handover to utran complete msg.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_l2ack_for_ho_to_utran_complete(
  tdsrrc_cmd_type *cmd_ptr  /* Received command */
)
{
  uint8 i;

  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    /* TDSRRC_DOWNLINK_L2ACK_IND */
    case TDSRRC_DOWNLINK_L2ACK_IND:
      for(i = 0; i < g2td_info.number_of_est_rabs; i++)
      {
       if(g2td_info.est_rabs[i].cn_domain == RRC_PS_DOMAIN_CN_ID)
       {
        rabm_cmd_type *rabm_ptr;
        /* It's a PS Handover. Send the RABMAS_RAB_ESTABLISH_IND primitive to RABM. */
        if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
        {
          ERR_FATAL("Can't send RABM cmd - out of memory!", 0, 0, 0);
        }
        rabm_ptr->header.cmd_id = RABMAS_RAB_ESTABLISH_IND;
        rabm_ptr->header.message_set = MS_RABM_RRC;
        rabm_ptr->cmd.rrc_establish_ind.rabid = (rabid_T)(g2td_info.est_rabs[i].rab_id);
#ifdef FEATURE_DUAL_SIM
        rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif
        /* Put the command on RABM queue */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD_PSHO: Sending RAB_ESTABLISH_IND to RABM RABid %d",
                                                              g2td_info.est_rabs[i].rab_id, 0, 0);
        rabm_put_cmd(rabm_ptr);
       } 
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD_PSHO: TDSRRC_DOWNLINK_L2ACK_IND in L2ACK wait state.", 0, 0, 0);

      tdsrrciho_state = TDSRRCIHO_INITIAL;
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* When waiting for L2 ack, if NAS sends abort req to RRC, IHO to reset its state.*/
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD_PSHO: TDSRRC_STATE_CHANGE_IND recd in L2ACK wait state.", 0, 0, 0);

      tdsrrciho_state = TDSRRCIHO_INITIAL;
      break;

    default:
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unexpected CMD %x in state %d", cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);
      break;
  }
}
#endif /* FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK */

/*===========================================================================

FUNCTION   tdsrrciho_wait_cell_update_cnf_substate_event_handler

DESCRIPTION

  This function handles the events received by this module when
  the procedure is Waiting for cell update confirmation from CU

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrciho_wait_cell_update_cnf_substate_event_handler(tdsrrc_cmd_type *cmd_ptr)
{
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received RRC_CELL_UPDATE_CNF from CU after resume failure ",0,0,0);

      if(tdsrrc_is_voice_rab_present()) 
      {
        /* TDS Voice Adapter needs to know the SELECT OWNER and it doesn't have any params */
        tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SELECT_OWNER);
		tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);

        /* Set the amr parameters */
        tdsrrciho_process_channel_config_cnf(tdsrrcdata_codec_info.rrc_active_codec);
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"AMR settings complete after T->G handover failure in IHO CU_CNF substate",0,0,0);
      }
      /* Send handover from utran failure to UTRAN */
      tdsrrciho_send_handover_from_utran_failure();

      /* Reset the procedure data, the procedure ends */
      tdsrrciho_init_data();


      break;

    case TDSRRC_ABORT_REQ:

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received TDSRRC_ABORT_REQ in wait_cell_update_cnf state,forwarding to RCE",0,0,0);

      /* Option 1: Forward the req to RCE */
      tdsrrcrce_procedure_event_handler( cmd_ptr );

      /* Reset the procedure data, the procedure ends */
      tdsrrciho_init_data();
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* RRC state change to IDLE indication is received */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC State Change to IDLE in %d", tdsrrciho_state, 0, 0);
      /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
      tdsrrc_configure_voice_adapter(NULL, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);

      /* Init procedure data */
      tdsrrciho_init_data();
      break;

    default:

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Received unexpected command in wait_cell_update_cnf_substate",0,0,0);
      break;
  }
} /* end of tdsrrciho_wait_cell_update_cnf_substate_event_handler */


/* take the common function shared by LTE and GeRAN out */
/*===========================================================================

FUNCTION RRC_GET_UTRAN_CLASSMARK_CHANGE_HANDOVER_INFO

DESCRIPTION

  This function builds a INTERRAT HANDOVER INFO message type which will be
  sent by GSM to UTRAN thru GERAN.  An ASN1 encoded message and message
  length is populated by RRC, the message and msg_length is used by RR to
  send this as part of Classmark Change to UTRAN via GERAN
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

===========================================================================*/
static void tdsrrc_get_utran_classmark_change_handover_info
(
  byte ** msg_ptr,    /* The command to be sent to UTRAN */
  uint8 * msg_len    /* Length of the message           */
#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
  , boolean irat_ho_info_compressed
#endif /* FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED */
#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
  ,sys_band_mask_e_type lte_band_pref
  ,boolean l2t_psho_enabled
#endif
)
{
  tdspdu_buf_type encoded_sdu;          /* encoded SDU */
  tdsrrc_InterRATHandoverInfo *ptr;     /* Pointer to Interrat HO Info PDU type */
  uint32 error_stat=0;
  tdsrrc_UE_RadioAccessCapabilityInfo *cap_ptr;

  boolean use_compressed_irat_ho_info = FALSE;

  tdsrrc_UE_RadioAccessCapabBandFDDList *ue_RadioAccessCapabBandFDDList;
  tdsrrc_CompressedModeMeasCapabFDDList *compressedModeMeasCapabFDDList;
  tdsrrcueci_band_storage_type  ueci_band;         /* type to get supported bands from csp */
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RadioFrequencyBandTDDextList *tdd128RF_Capability;
  uint8 i = 0;
#endif  

#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsrrc_UE_RadioAccessCapabilityInfo_TDD128_v8b0ext *v8b0_RadioAccessCapabilityInfo_ptr;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_START_Value   *start_ps;
#endif

#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
  use_compressed_irat_ho_info = irat_ho_info_compressed;
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RR indicates support for HO Info Compressed : %d",irat_ho_info_compressed,0,0);

#endif /* FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED */

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Get UTRAN Classmark Info",0,0,0);

  /* Allocate memory for this message. Note that this buffer is released
     by the delete command called from RR */
  ptr = (tdsrrc_InterRATHandoverInfo *)tdsrrc_malloc(sizeof(tdsrrc_InterRATHandoverInfo));

  if (ptr == NULL)
  {
    ERR_FATAL("Memory alloc failed Handover Info!", 0, 0, 0);
  }

  /* reset the ueci_band type */
  memset (&ueci_band, 0, sizeof(tdsrrcueci_band_storage_type));

  /* get the supported band list from csp */
  tdsrrccsp_get_supported_band_list(&ueci_band);

  /* Currently set all the fields to absent.. We will support this when ciphering
     support is added */
  ptr->predefinedConfigStatusList.t = T_tdsrrc_InterRATHandoverInfo_predefinedConfigStatusList_absent;

  ptr->uE_SecurityInformation.t = T_tdsrrc_InterRATHandoverInfo_uE_SecurityInformation_present;
  /*append start value for CS domain */
  ptr->uE_SecurityInformation.u.present.start_CS.numbits = TDSRRC_START_VALUE_LENGTH;

  tdsrrcsmc_append_start_per_domain((byte *) (ptr->uE_SecurityInformation.u.present.
                                           start_CS.data),
                                 RRC_CS_DOMAIN_CN_ID,
                                 TDSRRCSMC_APPEND_START);

  ptr->ue_CapabilityContainer.t = T_tdsrrc_InterRATHandoverInfo_ue_CapabilityContainer_absent;

  if(!use_compressed_irat_ho_info)
  {
    rtxMemSetDefBlkSize(2048);
    /* Allocate memory for this message. Note that this buffer is released
       by the delete command called from RR */
    cap_ptr = (tdsrrc_UE_RadioAccessCapabilityInfo *)tdsrrc_malloc(sizeof(tdsrrc_UE_RadioAccessCapabilityInfo));

    if (cap_ptr == NULL)
    {
      ERR_FATAL("Memory alloc failed Handover Info!", 0, 0, 0);
    }

    ptr->ue_CapabilityContainer.t = T_tdsrrc_InterRATHandoverInfo_ue_CapabilityContainer_present;

    (void)tdsrrcueci_append_ue_radio_access_capability(&(cap_ptr->ue_RadioAccessCapability),TRUE,
                        TDSRRCUECI_CAP_HO_INFO_TRANSFER);

    
    ue_RadioAccessCapabBandFDDList = &(cap_ptr->ue_RadioAccessCapability_v370ext.ue_RadioAccessCapabBandFDDList);
    
    ue_RadioAccessCapabBandFDDList->n = 1;

/*HX TBD: check where the alloc is freed*/

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt, ue_RadioAccessCapabBandFDDList, tdsrrc_UE_RadioAccessCapabBandFDD);
    // for KW
    if (ue_RadioAccessCapabBandFDDList->elem == NULL)
    {
      ERR_FATAL("Memory alloc failed ue_RadioAccessCapabBandFDDList->elem!", 0, 0, 0);
    }
    ue_RadioAccessCapabBandFDDList->elem[0].m.fddRF_CapabilityPresent = 0;
    ue_RadioAccessCapabBandFDDList->elem[0].radioFrequencyBandFDD = tdsrrc_RadioFrequencyBandFDD_bandIV;
    ue_RadioAccessCapabBandFDDList->elem[0].measurementCapability.m.compressedModeMeasCapabTDDListPresent = 0;
    ue_RadioAccessCapabBandFDDList->elem[0].measurementCapability.m.compressedModeMeasCapabGSMListPresent = 0;
    ue_RadioAccessCapabBandFDDList->elem[0].measurementCapability.m.compressedModeMeasCapabMCPresent = 0;

    compressedModeMeasCapabFDDList = &(ue_RadioAccessCapabBandFDDList->elem[0].measurementCapability.compressedModeMeasCapabFDDList);
    compressedModeMeasCapabFDDList->n = 1;

/*HX TBD: check where the alloc is freed*/
    ALLOC_ASN1ARRAY1(&tdsenc_ctxt, compressedModeMeasCapabFDDList, tdsrrc_CompressedModeMeasCapabFDD);
    // for KW
    if (compressedModeMeasCapabFDDList->elem == NULL)
    {
      ERR_FATAL("Memory alloc failed compressedModeMeasCapabFDDList->elem!", 0, 0, 0);
    }
    compressedModeMeasCapabFDDList->elem[0].m.radioFrequencyBandFDDPresent = 0;
    compressedModeMeasCapabFDDList->elem[0].dl_MeasurementsFDD = FALSE;
    compressedModeMeasCapabFDDList->elem[0].ul_MeasurementsFDD = FALSE;

    /* Encodes the OTA message using ASN1. The memory for msg_ptr is allocated
       by the RRC procedure and the memory for encoded_sdu will be allocated
       by the following function */

    if ((error_stat= tdsrrc_enc_UE_RadioAccessCapabilityInfo_PDU(cap_ptr,&ptr->ue_CapabilityContainer.u.present))
        !=0)
    {
        ERR_FATAL("Error in ASN1 encoding %d  error %d",
                          tdsrrc_UE_RadioAccessCapabilityInfo_PDU,
                          error_stat, 0);
    }



    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Class Mark Cap Len %d, Info",ptr->ue_CapabilityContainer.u.present.numocts,0,0);

    // The memory freeing for msg_ptr should be done here. using OSS compiler
    // functions. This function should free memory allocated for msg_ptr and as
    // well as linked lists in the message if they are any allocated.
   //if(tdsrrcasn1_free_pdu( (int)tdsrrc_UE_RadioAccessCapabilityInfo_PDU , cap_ptr))
    //{
      // This is a memory leak and it should never happen.
    //  TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);
    //}

    // memory freeing for encoded_sdu should be done here using OSS compiler
    // functions.
    //tdsrrcasn1_free_buf(encoded_sdu.value);
    tdsrrc_free(cap_ptr);
	rtxMemSetDefBlkSize(1024);
  }
#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"IRAT HO INFO Compression is supported",0,0,0);
    ptr->ue_CapabilityContainer.t = T_tdsrrc_InterRATHandoverInfo_ue_CapabilityContainer_absent;
  
  }
#endif /* FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED */

  /* Set rx_tx_TimeDifferenceType2Capable to FALSE. Revisit when this is supported */
  ptr->v390NonCriticalExtensions.t = T_tdsrrc_InterRATHandoverInfo_v390NonCriticalExtensions_present; 
  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.interRATHandoverInfo_v390ext);
 TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->v390NonCriticalExtensions.u.present.interRATHandoverInfo_v390ext,
   ue_RadioAccessCapability_v380ext);

  ptr->v390NonCriticalExtensions.u.present.interRATHandoverInfo_v390ext.ue_RadioAccessCapability_v380ext.
  ue_PositioningCapabilityExt_v380.rx_tx_TimeDifferenceType2Capable = FALSE;

  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.interRATHandoverInfo_v390ext.dl_PhysChCapabilityFDD_v380ext);

  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present);
  
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    /* Set v3a0NonCriticalExtensions */
    ptr->v390NonCriticalExtensions.u.present.m.v3a0NonCriticalExtensionsPresent =1;
    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.interRATHandoverInfo_v3a0ext);

    /* Set v3a0NonCriticalExtensions_laterNonCriticalExtensions */
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.m.
    laterNonCriticalExtensionsPresent =1;

    /* Set later non criticalExtensions bitmask to 0 initially */
   TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
      laterNonCriticalExtensions);

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
    laterNonCriticalExtensions.interRATHandoverInfo_v3d0ext);

  }
  
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
	if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
	{
	  /* Set interRATHandoverInfo_r3_add_extPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.m.interRATHandoverInfo_r3_add_extPresent =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext);
      
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.interRATHandoverInfo_v690ext1);	  

	  /* Set v7e0NonCriticalExtensionsPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.m.v7e0NonCriticalExtensionsPresent =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions);
	  
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.interRATHandoverInfo_v7e0ext);

	  /* Set v7f0NonCriticalExtensionsPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.m.v7f0NonCriticalExtensionsPresent =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions);

	  /* Set ue_RadioAccessCapabilityPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.m.ue_RadioAccessCapabilityPresent =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability);


	  /* Set physicalChannelCapability_LCRPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.m.physicalChannelCapability_LCRPresent =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.m.tdd128_PhysChCapabilityPresent =1;

	  /* DL physical channel capability */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
		downlinkPhysChCapability.maxTS_PerSubFrame = 4;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
		downlinkPhysChCapability.maxPhysChPerFrame = 64;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
		downlinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_DL_sf1;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        downlinkPhysChCapability.supportOfPDSCH = FALSE;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        downlinkPhysChCapability.maxPhysChPerTS = 16;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        downlinkPhysChCapability.supportOf8PSK = FALSE;

      /* UL physical channel capability */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        uplinkPhysChCapability.maxTS_PerSubFrame = 5;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        uplinkPhysChCapability.maxPhysChPerTimeslot = tdsrrc_MaxPhysChPerTimeslot_ts2;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        uplinkPhysChCapability.minimumSF = tdsrrc_MinimumSF_UL_sf1;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        uplinkPhysChCapability.supportOfPUSCH = FALSE;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
        interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.physicalChannelCapability_LCR.tdd128_PhysChCapability.
        uplinkPhysChCapability.supportOf8PSK = FALSE;


	  /* Set ue_RadioAccessCapabilityComp_TDD128Present */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.m.ue_RadioAccessCapabilityComp_TDD128Present =1;
	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128);

		/* Set hSDSCH_physical_layer_categoryPresent */
		ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		  laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.m.hSDSCH_physical_layer_categoryPresent =1;
		
		/* Set the value of hSDSCH_physical_layer_category */
		if (tdsnv_hsdpa_category > 15)
		{
		  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		  laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.hSDSCH_physical_layer_category = 15;
		}
		else
		{
	      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		    laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		    interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.hSDSCH_physical_layer_category = tdsnv_hsdpa_category;
	    }

#ifdef FEATURE_TDSCDMA_HSUPA
	  if (TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED)
	  { 

	  /* Set tdd_edch_PhysicalLayerCategoryPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.m.tdd_edch_PhysicalLayerCategoryPresent =1;

	  /* Set the value of tdd_edch_PhysicalLayerCategory */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.tdd_edch_PhysicalLayerCategory =(uint8)tds_nv_hsupa_category;
      }
#endif

      /* Set dl_CapabilityWithSimultaneousHS_DSCHConfigPresent */
	  ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.m.dl_CapabilityWithSimultaneousHS_DSCHConfigPresent =1; 
      /* CapabilityWithSimultaneousHS_DSCHConfig_present is 64 k */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.dl_CapabilityWithSimultaneousHS_DSCHConfig = tdsrrc_DL_CapabilityWithSimultaneousHS_DSCHConfig_kbps64;


	  /* Fill in RLC info. */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		  interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.m.totalRLC_AM_BufferSizePresent =1;
	  /*The relation between HSDPA categories and Total RLC AM buffer size is taken from Spec 25.306 8.10.0
		(Table 5.1n - Total RLC and MAC-hs parameters for 1.28 Mcps TDD HS-DSCH and E-DCH physical layer categories) */
	  if(tdsnv_hsdpa_category >= 13)
	  {
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		  laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		    interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_r5_ext_kb400;
      }
	  else
	  {
		ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
		  laterNonCriticalExtensions.interRATHandoverInfo_r3_add_ext.v7e0NonCriticalExtensions.v7f0NonCriticalExtensions.
		    interRATHandoverInfo_v7f0ext.ue_RadioAccessCapability.ue_RadioAccessCapabilityComp_TDD128.totalRLC_AM_BufferSize = tdsrrc_TotalRLC_AM_BufferSize_r5_ext_kb300;
      }
	}

    /* Set v3g0NonCriticalExtensions_present */
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.m.v3g0NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.v3g0NonCriticalExtensions);

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
    laterNonCriticalExtensions.v3g0NonCriticalExtensions.interRATHandoverInfo_v3g0ext);

    /* Set v4b0NonCriticalExtensions */
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
    laterNonCriticalExtensions.v3g0NonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions);
    
#ifdef FEATURE_TDSCDMA_REL9
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
    {
      /* Set the access stratum indicator */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.interRATHandoverInfo_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_9;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-9 in Handover Classmark info",0,0,0);
    }
    else
#endif /*FEATURE_TDSCDMA_REL9*/
#ifdef FEATURE_TDSCDMA_REL8
    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
    {
      /* Set the access stratum indicator */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.interRATHandoverInfo_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_8;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-8 in Handover Classmark info",0,0,0);
    }
    else
#endif /*FEATURE_TDSCDMA_REL8*/
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      /* Set the access stratum indicator */
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.interRATHandoverInfo_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_7;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-7 in Handover Classmark info",0,0,0);
    }
    else
    {
      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
      {
        /* Set the access stratum indicator */
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.interRATHandoverInfo_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_6;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-6 in Handover Classmark info",0,0,0);
      }
      else
      {
        /* Set the access stratum indicator */
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.interRATHandoverInfo_v4b0ext.accessStratumReleaseIndicator = tdsrrc_AccessStratumReleaseIndicator_rel_5;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"AccessStratumIndicator indicator set to Rel-5 in Handover Classmark info",0,0,0);
      }
    }
    /* Set v4d0NonCriticalExtensions */
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
    laterNonCriticalExtensions.v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.m.v4d0NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions);

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
    v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.interRATHandoverInfo_v4d0ext);

    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.v3g0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.interRATHandoverInfo_v4d0ext.m.tdd128_RF_CapabilityPresent = 1;
    
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.v3g0NonCriticalExtensions.
      v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.interRATHandoverInfo_v4d0ext.tdd128_RF_Capability = tdsrrc_RadioFrequencyBandTDDList_a;

    /* Set v590NonCriticalExtensions */
    ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.
    laterNonCriticalExtensions.v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.
    v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
    v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions);

    TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
    v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
    interRATHandoverInfo_v590ext);

#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
    if(use_compressed_irat_ho_info)
    {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      interRATHandoverInfo_v590ext,tdsrrc_InterRATHandoverInfo_v590ext_IEs,ue_RadioAccessCapabilityComp);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      interRATHandoverInfo_v590ext.ue_RadioAccessCapabilityComp.totalAM_RLCMemoryExceeds10kB = TRUE;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      interRATHandoverInfo_v590ext.ue_RadioAccessCapabilityComp.rf_CapabilityComp.fdd.t = T_tdsrrc_RF_CapabilityComp_fdd_notSupported;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      interRATHandoverInfo_v590ext.ue_RadioAccessCapabilityComp.rf_CapabilityComp.tdd384_RF_Capability.t =T_tdsrrc_RF_CapabilityComp_tdd384_RF_Capability_notSupported;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      interRATHandoverInfo_v590ext.ue_RadioAccessCapabilityComp.rf_CapabilityComp.tdd128_RF_Capability.t = T_tdsrrc_RF_CapabilityComp_tdd128_RF_Capability_supported;

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        interRATHandoverInfo_v590ext.ue_RadioAccessCapabilityComp.rf_CapabilityComp.tdd128_RF_Capability.u.supported = tdsrrc_RadioFrequencyBandTDDList_a;
    }
#endif /* FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED*/

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
        v690NonCriticalExtensionsPresent =1;
    
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions);
    
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.interRATHandoverInfo_v690ext);
    
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.interRATHandoverInfo_v690ext.ue_RadioAccessCapabilityComp2);
    
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.
        v590NonCriticalExtensions.v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent =1;
    
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions);
    
        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.interRATHandoverInfo_v6b0ext);
   
        TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.interRATHandoverInfo_v6b0ext,
        tdsrrc_InterRATHandoverInfo_v6b0ext_IEs,supportForSIB11bis);
    
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.interRATHandoverInfo_v6b0ext.supportForSIB11bis = 
        rrc_true_1;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
        if (l2t_psho_enabled)
        {
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      v690NonCriticalExtensions.interRATHandoverInfo_v690ext, 
        ue_SecurityInformation2);

      start_ps = &(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
      v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
      v690NonCriticalExtensions.interRATHandoverInfo_v690ext.ue_SecurityInformation2.start_PS);
        
      start_ps->numbits= TDSRRC_START_VALUE_LENGTH;      
        
      tdsrrcsmc_append_start_per_domain((byte *) (start_ps->data),
                                       RRC_PS_DOMAIN_CN_ID,
                                       TDSRRCSMC_APPEND_START);
        }
#endif /* FEATURE_LTE_TO_TDSCDMA */

    }
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
    {
      /*TBD PSR to be updated in 7.6*/
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent =1;
      
      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.interRATHandoverInfo_v6b0ext);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v6e0NonCriticalExtensionsPresent =1 ;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.interRATHandoverInfo_v6e0ext);

     ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.m.v770NonCriticalExtensionsPresent = 1;

       TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext);

       TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext,
       tdsrrc_InterRATHandoverInfo_v770ext_IEs,ue_RadioAccessCapabilityInfo);

       TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo);

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.m.rf_CapabilityPresent = 1;

       TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.rf_Capability);

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.rf_Capability.m.tdd128RF_CapabilityPresent = 1;

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.rf_Capability.tdd128RF_Capability = tdsrrc_RadioFrequencyBandTDDList_r7_a;

       TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.physicalChannelCapability);

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.physicalChannelCapability.m.tddPhysChCapability_128Present = 1;

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.physicalChannelCapability.tddPhysChCapability_128.
        uplinkPhysChCapability.maxPhysChPerTimeslot = 2;

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.m.ue_SpecificCapabilityInformationPresent = 1;

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.ue_SpecificCapabilityInformation = tdsrrc_UE_SpecificCapabilityInformation_LCRTDD_nF;


#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS

       TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo,
         tdsrrc_UE_RadioAccessCapabilityInfo_v770ext,mac_ehsSupport);

       ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityInfo.mac_ehsSupport = tdsrrc_UE_RadioAccessCapability_v770ext_IEs_mac_ehsSupport_true_;
#endif


	   TDSRRC_MSG_COMMON_SET_BITMASK_IE(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
	   v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
	   v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
	   interRATHandoverInfo_v770ext,
	   tdsrrc_InterRATHandoverInfo_v770ext_IEs,ue_RadioAccessCapabilityComp2);
	
	   TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
	   v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
	   v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
	   interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityComp2);

	   ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
	   v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
	   v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
	   interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityComp2.m.hsdsch_physical_layer_category_extPresent = 1;
	
	   ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
	   v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
	   v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
	   interRATHandoverInfo_v770ext.ue_RadioAccessCapabilityComp2.hsdsch_physical_layer_category_ext = tdsnv_hsdpa_category;

	   TDSRRC_MSG3(MSG_LEGACY_HIGH,"HSDPA CAT is %d in iRATHoInfo",tdsnv_hsdpa_category,0,0);

    }
#ifdef FEATURE_TDSCDMA_REL8
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
    {
#ifndef FEATURE_TDSCDMA_SUPPORT_LTE_FDD_TDD
        uint8 band_idx = 0;
        sys_band_mask_type lte_band_chk = 0;
#endif

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.m.
       v790nonCriticalExtensionsPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
      v790nonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.interRATHandoverInfo_v790ext);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
      v790nonCriticalExtensions.m.v860NonCriticalExtensionsPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.interRATHandoverInfo_v860ext);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.m.v880NonCriticalExtensionsPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.interRATHandoverInfo_v880ext);

      if (0 != ueci_band.num_supported_bands_tddv860)
      {
        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.interRATHandoverInfo_v880ext.m.ue_RadioAccessCapabilityInfo_TDD128Present = 1;


        TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.interRATHandoverInfo_v880ext.
          ue_RadioAccessCapabilityInfo_TDD128);

        ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.interRATHandoverInfo_v880ext.
          ue_RadioAccessCapabilityInfo_TDD128.m.tdd128RF_CapabilityPresent = 1;

        tdd128RF_Capability = &(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
          v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.interRATHandoverInfo_v880ext.
          ue_RadioAccessCapabilityInfo_TDD128.tdd128RF_Capability);

        tdd128RF_Capability->n = ueci_band.num_supported_bands_tddv860;

        ALLOC_ASN1ARRAY1(&tdsenc_ctxt, tdd128RF_Capability, tdsrrc_RadioFrequencyBandTDDext);
        
        if (tdd128RF_Capability->elem == NULL)
        {
          ERR_FATAL("Unable to allocate memory",0,0,0);
        }

        for (i = 0; i < tdd128RF_Capability->n; i++)
        {
            tdd128RF_Capability->elem[i] = ueci_band.band_type_tddv860[i];
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"No band E and band F selected",0,0,0);
      }

#ifdef FEATURE_TDSCDMA_TO_LTE
      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.m.v920NonCriticalExtensionsPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        interRATHandoverInfo_v920ext);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        m.v8b0NonCriticalExtensionsPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        v8b0NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        v8b0NonCriticalExtensions.interRATHandoverInfo_v8b0ext);

      ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        v8b0NonCriticalExtensions.interRATHandoverInfo_v8b0ext.m.ue_RadioAccessCapabilityInfoPresent = 1;

      TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
        v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
        v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
        v8b0NonCriticalExtensions.interRATHandoverInfo_v8b0ext.ue_RadioAccessCapabilityInfo);

	  TDSRRC_RESET_MSG_IE_PRESENT(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
		v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
		v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
		v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
		v8b0NonCriticalExtensions.interRATHandoverInfo_v8b0ext.ue_RadioAccessCapabilityInfo.measurementCapabilityTDD);

	  v8b0_RadioAccessCapabilityInfo_ptr = &(ptr->v390NonCriticalExtensions.u.present.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
		v3g0NonCriticalExtensions.v4b0NonCriticalExtensions.v4d0NonCriticalExtensions.v590NonCriticalExtensions.
		v690NonCriticalExtensions.v6b0NonCriticalExtensions.v6e0NonCriticalExtensions.v770NonCriticalExtensions.
		v790nonCriticalExtensions.v860NonCriticalExtensions.v880NonCriticalExtensions.v920NonCriticalExtensions.
		v8b0NonCriticalExtensions.interRATHandoverInfo_v8b0ext.ue_RadioAccessCapabilityInfo);

#ifndef FEATURE_TDSCDMA_SUPPORT_LTE_FDD_TDD
        /*uint8 band_idx = 0;
        sys_band_mask_type lte_band_chk = 0;*/ /*for QTF*/
#ifdef LTE_BAND_NUM
        for(band_idx =0 ; lte_band_pref && (band_idx < MIN(SYS_SBAND_LTE_EUTRAN_BAND_MAX, 0x3F)); band_idx++)
        {
          lte_band_chk = (sys_band_mask_type)(((uint64)1) << band_idx);
          if(lte_band_chk & lte_band_pref)
          {
            /*We should have no extend LTE band for OTA msg, extend here to be compatible with interface 
                   of lte_rrc_lte_fdd_is_supported() and  lte_rrc_lte_tdd_is_supported*/
            sys_lte_band_mask_e_type lte_band_pref_extend;
            memset(&lte_band_pref_extend, 0x00, sizeof(sys_lte_band_mask_e_type));
            #if(LTE_BAND_NUM == 256)
              lte_band_pref_extend.bits_1_64 = (uint64)lte_band_chk;
            #else
              lte_band_pref_extend = lte_band_chk;
            #endif
            if(!lte_rrc_lte_tdd_is_supported(lte_band_pref_extend))
            {
              lte_band_pref &= ~lte_band_chk;
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Only support LTE-TDD, removing LTE-FDD band %d from lte_band_pref provided by LTE",band_idx+1,0,0);
            }
          }
        }
#else
        for(band_idx =0 ; lte_band_pref && (band_idx < SYS_SBAND_LTE_EUTRAN_BAND_MAX); band_idx++)
        {
          lte_band_chk = (sys_band_mask_type)(((uint64)1) << band_idx);
          if(lte_band_chk & lte_band_pref)
          {
            if(!lte_rrc_lte_tdd_is_supported(lte_band_chk))
            {
              lte_band_pref &= ~lte_band_chk;
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Only support LTE-TDD, removing LTE-FDD band %d from lte_band_pref provided by LTE",band_idx+1,0,0);
            }
          }
        }
#endif/*LTE_BAND_NUM*/        
#endif

      if(tdsrrcueci_append_ue_radio_access_capability_tdd
		   (&v8b0_RadioAccessCapabilityInfo_ptr->measurementCapabilityTDD,lte_band_pref))
	  {
	    v8b0_RadioAccessCapabilityInfo_ptr->m.measurementCapabilityTDDPresent = 1;
	  }
#endif

    }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
    {
      /* IE 'hSDSCH_physical_layer_category_extension ' don't to report to GSM, so we shouldn't open IE 'v920NonCriticalExtensions' */

    }
#endif /*FEATURE_TDSCDMA_REL9*/

  }
  else
  {
    ptr->v390NonCriticalExtensions.t = T_tdsrrc_InterRATHandoverInfo_v390NonCriticalExtensions_absent;
  }

  /* Encodes the OTA message using ASN1. The memory for msg_ptr is allocated
     by the RRC procedure and the memory for encoded_sdu will be allocated
     by the following function */
  encoded_sdu = tdsrrcasn1_encode_pdu(ptr, tdsrrc_InterRATHandoverInfo_PDU);

  if(encoded_sdu.value == NULL)              /* Checks here whether encoding is*/
                                             /* ASN1 Encoding is failed        */
  {
    if (tdsrrc_encode_info.encode_status == TDSASN1_OUT_MEMORY)
    {
      ERR_FATAL("No memory for encoding %d %d",
                tdsrrc_encode_info.encode_status,
                tdsrrc_encode_info.pdu_num, 0);
    }
    else if (tdsrrc_encode_info.encode_status == TDSASN1_CONSTRAINT_VIOLATED)
    {
      ERR_FATAL("Constraint violation in ASN1 encoding %d %d",
                        tdsrrc_encode_info.encode_status,
                        tdsrrc_encode_info.pdu_num, 0);
    }
    else
    {
      ERR_FATAL("ASN1 encoding failed %d %d",
                        tdsrrc_encode_info.encode_status,
                        tdsrrc_encode_info.pdu_num, 0);
    }
  }

  /*check to see that the message was successfully encoded*/
  if(encoded_sdu.length != 0)
  {

    g2td_info.class_mark_ptr = tdsrrc_malloc(encoded_sdu.length);
    memscpy(g2td_info.class_mark_ptr,(size_t)encoded_sdu.length,encoded_sdu.value,(size_t)encoded_sdu.length);

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"IHO_DEBUG: Encoded msg 1st two bytes = %x, %x ",*(g2td_info.class_mark_ptr),*(g2td_info.class_mark_ptr+1),0);
    
    /*copy the message pointer and the message length to the RR parameters*/
    *(msg_ptr) = g2td_info.class_mark_ptr;
    *(msg_len) = (uint8)encoded_sdu.length;

    /*set the local flags to indicate that memory is allocated for the encoded buf
      and has to be deleted*/
    g2td_info.class_mark_info_present = TRUE;

    /* Allocate mem here , so that the buffer is not cleared when the grneral CnTX is freed.*/
  

  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"ASN1 encoding failed",0,0,0);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Class Mark Len %d, Info",encoded_sdu.length,0,0);

  // The memory freeing for msg_ptr should be done here. using OSS compiler
  // functions. This function should free memory allocated for msg_ptr and as
  // well as linked lists in the message if they are any allocated.
  if(tdsrrcasn1_free_pdu( (int) tdsrrc_InterRATHandoverInfo_PDU, ptr))
  {
    // This is a memory leak and it should never happen.
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Memory freeing is unsuccessful",0,0,0);
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"IHO_DEBUG: Encoded msg 1st two bytes = %x, %x ",*(*(msg_ptr)),*((*msg_ptr)+1),0);

}

/*===========================================================================

FUNCTION TDSRRC_DELETE_CLASSMARK_CHANGE_HANDOVER_INFO

DESCRIPTION

  This function deletes the CLASSMARK_CHANGE_HANDOVER_INFO asn1 type
  which was previously allocated to send to UTRAN via GERAN.
  RR calls this function after it has copied the contents of the
  Handover To Utran Info contents locally.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrc_delete_classmark_change_handover_info(void)
{
  if(g2td_info.class_mark_info_present == TRUE)
  {
    // memory freeing for encoded_sdu should be done here using OSS compiler
    // functions.
    tdsrrc_free(g2td_info.class_mark_ptr);

    /*reset the static variables*/
    g2td_info.class_mark_info_present = FALSE;
    g2td_info.class_mark_ptr = NULL;
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"No Classmark msg saved",0,0,0);
  }
}

/*===========================================================================

FUNCTION TDSRRC_DELETE_CLASSMARK_CHANGE_HANDOVER_INFO_SUB_ID

DESCRIPTION

  This function serves for the same purpose as tdsrrc_delete_classmark_change_handover_info, with TDS
  as_id provided for Dual_SIM.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrc_delete_classmark_change_handover_info_sub_id( sys_modem_as_id_e_type as_id)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"AS-id mismatch %d, internal %d",as_id, tdsrrc_get_as_id(),0);
  }
  tdsrrc_delete_classmark_change_handover_info();
}

/*===========================================================================

FUNCTION TDSRRC_GET_CLASSMARK_CHANGE_HANDOVER_INFO

DESCRIPTION

  This function builds a INTERRAT HANDOVER INFO message type which will be
  sent by GSM to UTRAN thru GERAN.  An ASN1 encoded message and message
  length is populated by RRC, the message and msg_length is used by RR to
  send this as part of Classmark Change to UTRAN via GERAN

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

===========================================================================*/
void tdsrrc_get_classmark_change_handover_info
(
  byte ** msg_ptr,    /* The command to be sent to UTRAN */
  uint8 * msg_len    /* Length of the message           */
  , boolean irat_ho_info_compressed

)
{
#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
  tdsrrcmcm_update_lte_mode_enabled_status(FALSE);
#endif

  {
    tdsrrc_get_utran_classmark_change_handover_info(
        msg_ptr
        ,msg_len
#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
       ,irat_ho_info_compressed
#endif
#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
       ,0
	   ,0
#endif
       );
  }
}

/*===========================================================================

FUNCTION TDSRRC_GET_CLASSMARK_CHANGE_HANDOVER_INFO_SUB_ID

DESCRIPTION

  This function serves for the same purpose as tdsrrc_get_classmark_change_handover_info, with TDS
  as_id provided for Dual_SIM.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

===========================================================================*/
void tdsrrc_get_classmark_change_handover_info_sub_id
(
  byte ** msg_ptr,    /* The command to be sent to UTRAN */
  uint8 * msg_len,    /* Length of the message           */
  boolean irat_ho_info_compressed,
  sys_modem_as_id_e_type as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"AS-id mismatch %d, internal %d",as_id, tdsrrc_get_as_id(),0);
  }
  tdsrrc_get_classmark_change_handover_info
  (
    msg_ptr,/* The command to be sent to UTRAN */
    msg_len, /* Length of the message */
    irat_ho_info_compressed
  );
}

/*===========================================================================

FUNCTION tdsrrciho_is_td2g_ho_active


DESCRIPTION

  API to indicate if WTOG HO is active and ongoing.  Used by other procedures
  to query if WTOG HO is ongoing

DEPENDENCIES

  None

RETURN VALUE

TRUE: If WTOG HO is active
FALSE: IF WTOG is not active

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrciho_is_td2g_ho_active
(
 void
)
{
  /* Check the procedure substate and procedure the event accordingly */
  switch( tdsrrciho_state )
  {
    case TDSRRCIHO_WAIT_STANDBY_CNF:
    case TDSRRCIHO_WAIT_INTERRAT_HO_CNF:
    case TDSRRCIHO_WAIT_ACTIVE_CNF:
    case TDSRRCIHO_WAIT_CHANNEL_CONFIG_CNF:
    case TDSRRCIHO_WAIT_CELL_UPDATE_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IHO proc has ongoing WtoG HO iho substate %d", tdsrrciho_state,0,0);
      return TRUE;

    default:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IHO proc has NO WtoG HO ongoing iho substate %d",tdsrrciho_state,0,0);
      return FALSE;
  }
}


#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*===========================================================================

FUNCTION   tdsrrciho_send_sync_ind_for_g2td_ho

DESCRIPTION
  Sends SYNC Ind to MM after handover to utran is successful.
  Also sends HandoverToUTRANComplete to the network to indicate that we have
  successfully Handed Over to UTRAN from GSM and all GSM buffered messages
  have already been sent to the N/W.  This is also done after indication is
  sent to NAS to inform that HO is complete and Vocoder should be switched
  on for TDS

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrciho_send_sync_ind_for_g2td_ho(void)
{

#ifdef FEATURE_LTE_TO_TDSCDMA
  if(g2td_info.is_cs_rab_present)
  {
#endif /* FEATURE_LTE_TO_TDSCDMA */
  mm_cmd_type *mm_cmd_ptr;                      /* Pointer to the MM Command */

  /* Allocate memory for SYNC_IND command */
  if( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) != NULL)
  {
#ifdef FEATURE_DUAL_SIM
    mm_cmd_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif

    /* Fill in parameters fro RRC_SYNC_IND command */
    mm_cmd_ptr->cmd.rrc_sync_ind.cn_domain_id= RRC_CS_DOMAIN_CN_ID;

    mm_cmd_ptr->cmd.rrc_sync_ind.cause = RRC_HANDOVER;

    /*set the rab info if it was included in the handover to utran
      message*/
    if(g2td_info.rab_info_included == TRUE)
    {
      mm_cmd_ptr->cmd.rrc_sync_ind.rab_id_present = TRUE;
#ifndef FEATURE_LTE_TO_TDSCDMA
      mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.rab_id = g2td_info.est_rabs.rab_id;

      /*mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.action = RAB_ESTABLISHED;*/

      if(g2td_info.est_rabs.rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
      }
      else if(g2td_info.est_rabs.rab_type == TDSRRCRB_CS_DATA_RAB)
      {
        /*HO of CS Data call from G to W is for futuristic use. Right now we don't use it.*/
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;        
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Something wrong with RAB Type for RAB-id %d",
              mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.rab_id,0,0);
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
      }
#else
      mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.rab_id = g2td_info.est_rabs[g2td_info.cs_rab_index].rab_id;

      if(g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type == TDSRRCRB_CS_VOICE_RAB)
      {
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_VOICE_CALL;
      }
      else if(g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type == TDSRRCRB_CS_DATA_RAB)
      {
        /*HO of CS Data call from G to W is for futuristic use. Right now we don't use it.*/
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_CS_DATA_CALL;        
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Something wrong with RAB Type for RAB-id %d",
              mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.rab_id,0,0);
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type = RRCMN_INVALID_RAB_ID;
      }
#endif /* FEATURE_LTE_TO_TDSCDMA */

      mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.action = RAB_ESTABLISHED;

      /*reset the g2td_info static variable for rab info*/
      g2td_info.rab_info_included = FALSE;

      /* Fill in the codec type used in RRC_SYNC_IND*/
      if(mm_cmd_ptr->cmd.rrc_sync_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        if(tdsrrc_get_current_codec_mode() == TDSCDMA_IVOCODER_ID_AMRWB )
        {
          mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_WB_AMR;
        }
        else
        {
          mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NB_AMR;
        }
      }
      else
      {
        mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Codec %d in RRC_SYNC_IND",mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.codec,0,0);    

    }
    /*else set it to false*/
    else
    {
      mm_cmd_ptr->cmd.rrc_sync_ind.rab_id_present = FALSE;
    }

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
    #error code not present
#endif
    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_cmd_ptr);
    }
    mm_put_cmd(mm_cmd_ptr);

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Sending RRC_SYNC_IND to MM for RAB type %d RAB-id %d", 
            mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.cs_rab_type, 
            mm_cmd_ptr->cmd.rrc_sync_ind.rab_info.rab_id, 0);
  }
  else   /* RRC Task failed to allocate memory */
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unable to allocate memory for MM Cmd: %d",
              RRC_SYNC_IND,0,0);
  }

#ifdef FEATURE_LTE_TO_TDSCDMA
  }
#endif /* FEATURE_LTE_TO_TDSCDMA */

#if (defined FEATURE_LTE_TO_TDSCDMA) && (!defined FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK)
  if(g2td_info.is_ps_rab_present)
  {
    uint16 i;
    for(i = 0; i < g2td_info.number_of_est_rabs; i++)
    {
      if(g2td_info.est_rabs[i].cn_domain == RRC_PS_DOMAIN_CN_ID)
      {
        rabm_cmd_type *rabm_ptr;
        /* It's a PS Handover. Send the RABMAS_RAB_ESTABLISH_IND primitive to RABM. */
        if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
        {
          ERR_FATAL("Can't send RABM cmd - out of memory!", 0, 0, 0);
        }
        rabm_ptr->header.cmd_id = RABMAS_RAB_ESTABLISH_IND;
        rabm_ptr->header.message_set = MS_RABM_RRC;
        rabm_ptr->cmd.rrc_establish_ind.rabid = (rabid_T)(g2td_info.est_rabs[i].rab_id);
#ifdef FEATURE_DUAL_SIM
        rabm_ptr->cmd.rrc_establish_ind.as_id = tdsrrc_get_as_id();
#endif    
        /* Put the command on RABM queue */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD_PSHO: Sending RAB_ESTABLISH_IND to RABM RABid %d",
                                                              g2td_info.est_rabs[i].rab_id, 0, 0);
        rabm_put_cmd(rabm_ptr);
      }
    }
  }
#endif /* FEATURE_LTE_TO_TDSCDMA */


  /*send the OTA message to UTRAN*/
  tdsrrciho_send_handover_to_utran_complete();

  /*only if ciphering is started on gsm side do we want to start ciphering
    on tds side*/
  if((g2td_info.gsm_ciph_status == TRUE)
#ifdef FEATURE_LTE_TO_TDSCDMA
    && ((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_GSM)||
         ((tdsrrciho_interrat_type == TDSRRCIHO_INTERRAT_LTE) &&
        (g2td_info.is_cs_rab_present)))
#endif
    )
  {
    /*send step 2 ciphering configuration to SMC procedure*/
    TDSRRC_MSG3(MSG_LEGACY_HIGH," Processing step 2 HO request ",0,0,0);	
    (void)tdsrrcsmc_step_2_ho_ciph_req(g2td_info.count_c_act_time);
  }

}

#endif

#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
/*===========================================================================

FUNCTION RRCIHO_HANDLE_UTRA_CAP_REQ_FROM_LTE


DESCRIPTION

  This function handle the TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ from LTE RRC

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrciho_handle_utra_cap_req_from_lte
(
  tds_rrc_lte_utra_capabilities_req_type *utra_cap_req
)
{
  tds_rrc_lte_utra_capabilities_rsp_type utra_cap_rsp;
  byte *cap_msg_ptr = NULL;
  uint8 cap_msg_len;
  uint16 idx=0;

  memset(&utra_cap_rsp,0,sizeof(tds_rrc_lte_utra_capabilities_rsp_type));

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: Recvd TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ from LTE RRC",0,0,0);

  /*update the modes and band preferences*/
  tdsrrc_update_band_preference(utra_cap_req->rat_capabilities.tdscdma_band_preference);
  tdsrrcmcm_update_lte_mode_enabled_status(TRUE);

  if(!utra_cap_req->rat_capabilities.geran_is_supported)
  {
    tdsrrcmcm_update_dual_mode_enabled_status(FALSE);
  }
  else
  {
    tdsrr_set_band_pref_internal(utra_cap_req->rat_capabilities.geran_band_preference);
    tdsrrcmcm_update_dual_mode_enabled_status(TRUE);
  }

  /*
    Call the existing function
    In spec, predifined configurations are needed to be filled in different ways for E-UTRA and other RATS.
    As we dont fill the predefined configuration stuff, there is not need of change in arguments
  */
#if defined(LTE_BAND_NUM)&&(LTE_BAND_NUM == 256)
  tdsrrc_get_utran_classmark_change_handover_info(
    &cap_msg_ptr,&cap_msg_len
#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
    ,utra_cap_req->compressed_mode_required
#endif
#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
    ,(sys_band_mask_e_type)(utra_cap_req->rat_capabilities.lte_band_preference.bits_1_64)/*Downgrade to bits_1_64, L-RRC should not use extended LTE band*/
	  ,utra_cap_req->l2t_psho_enabled
#endif
  );
#else
  tdsrrc_get_utran_classmark_change_handover_info(
       &cap_msg_ptr,&cap_msg_len
#ifdef FEATURE_TDSCDMA_IRAT_HO_INFO_COMPRESSED
       ,utra_cap_req->compressed_mode_required
#endif
#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
       ,utra_cap_req->rat_capabilities.lte_band_preference
	   ,utra_cap_req->l2t_psho_enabled
#endif
    );
#endif
  utra_cap_rsp.capabilities_container_length = 0;
  if(cap_msg_ptr != NULL)
  {
    for(idx=0;idx < cap_msg_len && idx < TDSCDMA_MAX_CAP_CONTAINER_LENGTH;idx++)
    {
      utra_cap_rsp.capabilities_container[idx] = *(cap_msg_ptr+idx);
    }
    utra_cap_rsp.capabilities_container_length = cap_msg_len;
  }

  msgr_init_hdr(&utra_cap_rsp.msg_hdr,MSGR_TDSCDMA_RRC,TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&utra_cap_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&utra_cap_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&utra_cap_rsp.msg_hdr, sizeof(tds_rrc_lte_utra_capabilities_rsp_type));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_lte_rrc(&utra_cap_rsp.msg_hdr, sizeof(tds_rrc_lte_utra_capabilities_rsp_type));
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2TD: TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_RSP sent to LTE RRC", 0, 0, 0);

  tdsrrc_delete_classmark_change_handover_info();
}
#endif


/*===========================================================================

FUNCTION RRCIHO_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Inter-RAT Handover module. This function is called by the
  dispatcher whenever an event is received for this module.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrciho_event_handler(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Event is received by IHO: %d at: %d",
     cmd_ptr->cmd_hdr.cmd_id, tdsrrciho_state, 0);

  /*we have added support to go from connected mode to inactive mode using
    MODE_CHANGE_REQ primitive..  When this happens MCM sends mode_change_ind,
    we honour this message before all other messages clear rrciho's initial
    database and go to TDSRRCIHO_INITIAL substate, then process nothing else*/
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_CHANGE_IND) &&
     (cmd_ptr->cmd.mode_change_ind.from_mode == TDSRRC_MODE_CHANGE) &&
     (cmd_ptr->cmd.mode_change_ind.to_mode == TDSRRC_MODE_INACTIVE))
  {
    tdsrrciho_init_data();
    return;
  }

#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MSGR_CMD) &&
      (cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id == TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ))
  {
    tdsrrciho_handle_utra_cap_req_from_lte(&cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_lte_utra_capabilities_req);
    return;
  }
#endif

  /* Check the procedure substate and procedure the event accordingly */
  switch( tdsrrciho_state )
  {
    case TDSRRCIHO_INITIAL:
      /* Call the event handler for Initial substate */
      tdsrrciho_initial_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_STANDBY_CNF:
      /* Call the event handler for waiting for standby cnf substate */
      tdsrrciho_wait_standby_cnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_INTERRAT_HO_CNF:
      /* Call the event handler for wait for inter-RAT handover cnf substate */
      tdsrrciho_wait_interrat_ho_cnf_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_ACTIVE_CNF:
      /* Call the event handler for waiting for active confirmation substate */
      tdsrrciho_wait_active_cnf_substate_event_handler(cmd_ptr);
      break;

    case TDSRRCIHO_WAIT_CHANNEL_CONFIG_CNF:
      /* Call the event handler for waiting for wait for channel config confirmation substate */
      tdsrrciho_wait_channel_config_cnf_substate_event_handler(cmd_ptr);
      break;

    case TDSRRCIHO_WAIT_CELL_UPDATE_CNF:
      /* Call the event handler for waiting for wait for cell update confirmation substate */
      tdsrrciho_wait_cell_update_cnf_substate_event_handler(cmd_ptr);
      break;

    case TDSRRCIHO_WAIT_ABORT_CNF:
      /* Call the event handler for wait for abort cnf substate */
      tdsrrciho_wait_abort_cnf_substate_event_handler( cmd_ptr );
      break;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)

    case TDSRRCIHO_WAIT_ACQ_CNF:
      /* Call the event handler for wait for acq cnf substate */
      tdsrrciho_wait_acq_cnf_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_IRAT_IN_PROGRESS_CNF:
      /* Call the event handler for wait for interrat in progress cnf substate */
      tdsrrciho_wait_irat_in_progress_cnf_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_INACTIVE_CNF:
      /* Call the event handler for wait for interrat in progress cnf substate */
      tdsrrciho_wait_inactive_cnf_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_CHAN_CONFIG_G2TD_CNF:
      /* Call the event handler for wait for channel config state to go to dch in
         case of a GSM to TDS transition */
      tdsrrciho_wait_chan_config_gtow_cnf_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_CIPH_CONFIG_G2TD_CNF:
      /* Call the event handler for wait for ciph config state to go to dch in
         case of a GSM to TDS transition */
      tdsrrciho_wait_ciph_config_gtow_cnf_event_handler( cmd_ptr );
      break;

    case TDSRRCIHO_WAIT_ACT_RSP:
      /* Call the event handler for wait for activation rsp.
          Add this state for this race condition:
          1. receive PSHO REQ, PSHO success, send PSHO RSP
          2. receive PSHO ABORT REQ*/     
      tdsrrciho_wait_act_rsp( cmd_ptr );
      break;

#endif

/*HX TBD: check if the following is still needed*/
#ifdef FEATURE_L2TD_PS_HANDOVER_WAIT_L2ACK
    case TDSRRCIHO_WAIT_L2ACK_FOR_HO_TO_UTRAN_COMPLETE:
      /* Call the event handler for wait for ciph config state to go to dch in
         case of a GSM to TDSCDMA transition */
      tdsrrciho_wait_l2ack_for_ho_to_utran_complete( cmd_ptr );
      break;
#endif
     
    default:
      /* Invalid substate - should never happen. Clear
         all procedure vars */
      tdsrrciho_init_data();
      ERR_FATAL("Invalid IHO Substate %d", tdsrrciho_state, 0, 0);
      break;
  }/* end switch */
  return;
} /* end of tdsrrciho_event_handler */


/*===========================================================================

FUNCTION   RRCIHO_INIT_PROCEDURE

DESCRIPTION

  This function initializes the inter-RAT handover procedure.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrciho_init_procedure(void)
{
  tdsgsm_commands.n =0;

#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsprocedure_info.parms.eutra.eutra_container_size = 0;
  tdsprocedure_info.parms.eutra.eutra_container_ptr = NULL;
#endif

  /* due to tdsrrciho_init_procedure was called by task initial and  tdsrrciho_init_data was called by many functions
   so set it to zero before call tdsrrciho_init_data*/
  tdsprocedure_info.handover_type = 0;
  /* Initialize the procedure data */
  tdsrrciho_init_data();

#if defined (FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  //predefined config's are not supported, set it to zero for now
  g2td_info.predef_config_id = 0;
  g2td_info.config_type = IHO_G2TD_PREDEF_CONFIG;
#endif

  /* Register the state change from any state to Idle */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_IHO,      /* Procedure name */
                             TDSRRC_STATE_WILDCARD,     /* From State     */
                             TDSRRC_STATE_DISCONNECTED  /* To State       */
                           );
}



#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*===========================================================================

FUNCTION   tdsrrciho_is_iho_td2g_active

DESCRIPTION

 Returns whether IRAT W2G is active or not

DEPENDENCIES

  None.

RETURN VALUE

  boolean

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrciho_is_iho_td2g_active()
{
  if((tdsrrciho_state != TDSRRCIHO_INITIAL) &&
     ((tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM)
#ifdef FEATURE_TDSCDMA_TO_LTE
     || (tdsprocedure_info.handover_type == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
#endif
    )
  )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }  
}
#endif 


//#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER) 
/*===========================================================================

FUNCTION   rrciho_free_sib16

DESCRIPTION

 Frees the ptr to sysInfoType16 previously allocated.

DEPENDENCIES

  None.

RETURN VALUE

   FAILURE if an error occurred while freeing else
   SUCCESS.


SIDE EFFECTS

  None.

===========================================================================*/

uecomdef_status_e_type tdsrrciho_free_sib16 
(
  void
)
{
  uecomdef_status_e_type status = FAILURE;
  if(g2td_info.sib16_ptr != NULL)
  {
    if(tdsrrcasn1_free_pdu((int)tdsrrc_SysInfoType16_PDU, g2td_info.sib16_ptr))
    {
      // This is a memory leak and it should never happen.
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"RRCIHO FREE SIB16:Memory freeing is unsuccessful",0,0,0);
      status = FAILURE;
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO FREE SIB16:free g2td_info.sib16_ptr : successful ",0,0,0);
      status = SUCCESS;
    }
    g2td_info.sib16_ptr = NULL;
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCIHO FREE SIB16: g2td_info.sib16_ptr is NULL, no need to free",0,0,0);
    status = SUCCESS;
  }
  return status;
}

/*===========================================================================

FUNCTION tdsrrciho_is_g2td_ho_active

DESCRIPTION 
    This function returns status of GTOTDS HO.

DEPENDENCIES
    None.

RETURN VALUE
    None.

===========================================================================*/
boolean tdsrrciho_is_g2td_ho_active(void)
{
  /* Check the procedure substate and procedure the event accordingly */
  switch( tdsrrciho_state )
  {
    case TDSRRCIHO_WAIT_ACQ_CNF:
    case TDSRRCIHO_WAIT_IRAT_IN_PROGRESS_CNF:
    case TDSRRCIHO_WAIT_INACTIVE_CNF:
    case TDSRRCIHO_WAIT_CHAN_CONFIG_G2TD_CNF:
    case TDSRRCIHO_WAIT_CIPH_CONFIG_G2TD_CNF:
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"IHO proc has ongoing GTOW HO iho substate %d", tdsrrciho_state,0,0);
      return TRUE;

    default:
      TDSRRC_MSG1(MSG_LEGACY_MED,"IHO proc has NO GTOW HO ongoing iho substate %d",tdsrrciho_state);
      return FALSE;
  }
}

/*===========================================================================

FUNCTION   tdsrrciho_is_cs_data_rab_present

DESCRIPTION
  This function indicates whether any voice RAB is present or not in g2td est_rabs database.

      
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE: if CS DATA RAB exist in g2td est_rabs database
  FALSE: if CS DATA RAB doesnt exist g2td in est_rabs database


SIDE EFFECTS

  None.

===========================================================================*/ 


boolean tdsrrciho_is_cs_data_rab_present (void)
{
  boolean status = FALSE;
#ifdef FEATURE_LTE_TO_TDSCDMA
  if(g2td_info.is_cs_rab_present)
  {
    if ( TDSRRCRB_CS_DATA_RAB == g2td_info.est_rabs[g2td_info.cs_rab_index].rab_type )
    {
      status = TRUE;
    }
  }
#else
  if(g2td_info.est_rabs.rab_type == TDSRRCRB_CS_DATA_RAB)
  { 
    status =  TRUE;
  }
#endif /* FEATURE_LTE_TO_TDSCDMA */  
  return status;
}

/*===========================================================================

FUNCTION  tdsrrc_is_x2td_irat_hho_pending

DESCRIPTION
  This function get the state from iho to check if there is x2td irat
  hho is pending channel configuration. 

DEPENDENCIES


RETURN VALUE
  TRUE: there is pending channel configuration from IHO
  FALSE: no pending channel configuration from IHO

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_is_x2td_irat_hho_pending(void)
{
  return g2td_info.is_x2td_irat_ho;
}

/*===========================================================================

FUNCTION  tdsrrc_is_x2td_irat_hho_timing

DESCRIPTION
  This function get the status from iho to check if there is x2td irat
  hho ongoing with timing info from x rat. 

DEPENDENCIES


RETURN VALUE
  TRUE: there is ongoing x2td irat hho with timing info from x rat
  FALSE: otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_is_x2td_irat_hho_timing(void)
{
  return (g2td_info.num_of_jds_cells > 0) ? TRUE : FALSE;
}
//#endif


/*===========================================================================

FUNCTION   tdsrrciho_send_cipher_sync_ind

DESCRIPTION

  This function sends indication to RR that the new CK/IK keys are activated

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrciho_send_cipher_sync_ind(void)
{
  /*declare var of struct abort confirm type*/
  tdsrr_cipher_sync_ind_type td2g_cipher_sync_ind;

  memset(&td2g_cipher_sync_ind,0,sizeof(tdsrr_cipher_sync_ind_type));

  /* Send the RRC_CIPHER_SYNC_IND message to RR */
  tdsrr_send_rrc_message_to_rr_internal((void *)(&(td2g_cipher_sync_ind)), TDSRR_CIPHER_SYNC_IND,
                            sizeof(tdsrr_cipher_sync_ind_type));
  TDSRRC_MSG3(MSG_LEGACY_HIGH, "Sent the TDSRR_CIPHER_SYNC_IND to RR",0,0,0);
  return;
}

/*===========================================================================

FUNCTION   tdsrrciho_is_l2t_ho

DESCRIPTION

  This function checks whether the source RAT is LTE for X2T HO.

DEPENDENCIES

  This function can only be used when the X2T HO is in progress, otherwise 
  the status is not reliable.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrciho_is_l2t_ho (void)
{
#ifdef FEATURE_LTE_TO_TDSCDMA
  if (TDSRRCIHO_INTERRAT_LTE == tdsrrciho_interrat_type)
  {
    return TRUE;
  }
  else
#endif
  {
    return FALSE;
  }
}
