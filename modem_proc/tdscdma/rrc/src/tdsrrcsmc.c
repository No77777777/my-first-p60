/*===========================================================================
                  R R C  SECURITY MODE COMMAND

DESCRIPTION
  This  file implements functions for Security Mode Command
  procedure.  Ciphering and Integrity related IE's received
  in the SMC message are received and processed.


EXTERNALIZED FUNCTIONS

  tdsrrcsmc_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Security Mode Command procedure.

  tdsrrcsmc_init_procedure
    This function initializes required parameters for Security Mode
    procedure.

  tdsrrcsmc_append_hfns_to_rel_ind 
    This function appends PS and CS domain HFN's to RRC_REL_IND
    
  tdsrrcsmc_append_ul_integrity_prot_act_info
    The function appends UL Integrity Activation Info IE to the client
    functions.
    
  tdsrrcsmc_process_integrity_protection_mode_info
    This function processes the Integrity Protection Mode Info IE sent
    to it.
    
  tdsrrcsmc_append_start_list
    This function appends the latest START_LIST.
    
  tdsrrcsmc_append_uplink_integrity_check_info
    This function appends Integrity Check Info IE to UL messages.
    
  tdsrrcsmc_check_downlink_integrity
    This function verifies Integrity on messages received on 
    downlink.
    
  tdsrrcsmc_init_integrity_ciphering_config
    This function initializes Integrity Protection Information.

  tdsrrcsmc_update_integrity_ciphering_config
    This function updates Integrity Protection Information.

  tdsrrcsmc_is_ciphering_enabled
    This function checks to see if Ciphering is enabled or not.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsmc.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/19/13   hx      Adding the separation for PS and CS in tdsrrcf9_init_key_value, 
                   to correct the wrong revert issue if UE gets SMC failure. 
06/10/13   hx      Adding support for feature FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO 
04/30/13   hx      Made chagnes to send TDSRR_CIPHER_SYNC_IND to G whenever new keys are applied  
01/03/13   hx      Buffering the DL DT message that arrives UE during SRNS reloc pending state on source cell.
12/28/12   hx      Handle the second SMC arrives before L2ACK received for first SMC after SRNS reloc.
11/08/12   hx      Fix the issue CS/PS keys are moved to old due to one TDSRRC_SIM_UPDATE_REQ
09/07/12   hx      Fix the activation for non-SRB set to 0 without any messages on that RB, 
                   after SRNS relocation, the SN was wrongly reversed.                   
08/20/12   hx      Fix the SN sync between CS/PS domain during activation pending state.
                   Align old_1, old, and latest SN part. 
08/20/12   hx      Merge W CR343287
06/12/12   hx      Change the call flow to avoid race condition for sending UL data during 
                   SRNS relocation.
06/01/12   hx      Fix the SRB1 IP check problem after SRNS relocation
03/15/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
01/05/12   hx      Add fix to hold the valid old_1 domain by seperating the 
                   tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1 to per SRB based.
12/29/11   hx      Add fix for non-SRB2 SN jumping in SMC "modify" in current domain
10/27/11   hx      Up-Merged LTE support. Up-Merged WCDMA CR: 289288, 292410
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
09/08/10   ss      Added code changes to process R7 security mode command cipherring
                   and Integrity I.E's if NV is enabled for them.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"              
#include "tdsrrcsmc.h"
#include "msg.h"
#include "err.h"
#include "bit.h"
#include "tdsrrcasn1.h"
#include "tdsrrclcm.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcsend.h"
#include "tdsrrcf9driver.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcscmgr.h"
#include "tdsrrcnv.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrccu.h"
#include "tdsrrcrbe.h"
#include "tdsrrcueci.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcdt.h"
#include "tdsseq.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrclogging.h"
#include "tdscdmaselfhosting.h"
#include "tdsmacrrcif.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcmcm.h"
#include "tdsrrcllc.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrctmr.h"

#include "mmgsdilib.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/*===================================================================
                            MACROS AND FUNCTIONS
=====================================================================*/

/*Macro used to indicate for printing integrity key*/
#define  TDSRRC_PRINT_INTEGRITY_KEY    0

/*Macro used to indicate for printing ciphering key*/
#define  TDSRRC_PRINT_CIPHERING_KEY    1

/*Macro indicates the max start val used to saturate at threshold*/
#define TDSSTART_THRESHOLD 0x000FFFFF
 
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* boolean flags to read NV items and check to see if Integrity 
   and Ciphering are enabled from QPST
*/
boolean    tdsnv_integrity_enabled = TRUE;     
/*tdsnv_ciphering_enabled should be always set to TRUE 
  since we have to support Ciphering with UEA0 even if 
  the NV ciphering is set to FALSE through QPST.As 
  most of the code for ciphering is under this flag*/
boolean    tdsnv_ciphering_enabled = TRUE;     
/*tdsnv_ciphering_enabled_status gives the exact
  Status of ciphering enabled through QPST.
  This NV item is used, when we have to look for 
  Security Capability's i.e.
  if tdsnv_ciphering_enabled_status = TRUE, then UE supports both UEA0/UEA1
  if tdsnv_ciphering_enabled_status = FALSE, then UE supports only UEA0*/
boolean    tdsnv_ciphering_enabled_status;

static  tdsrlc_ul_stop_suspend_cnf_type rrcsmc_ul_cnf;

extern boolean    tdsnv_fake_security_enabled;    

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
 * Global Data Declarations
 * ----------------------------------------------------------------------- */
boolean tdsfeature_rollback_security_config = TRUE;

tdsrrcsmc_ciphering_config_info_type tdscipher_config_db;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/*maintain state of SMC procedure*/
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcsmc_substate_e_type  tdsrrcsmc_substate;  

/*maintain status of failure cause*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_failure_type;
static tdsrrc_ProtocolErrorCause tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;

/* file global var contains info about suspend status and info of rlc rb's*/
static tdsrrcsmc_suspend_stop_rlc_type tdsrrcsmc_rb_suspend_or_stop_info;

/*Temporary variable to store the max count c values*/
static tdsrrcsmc_rel_rbs_max_count_c tdsrrcsmc_max_count_c;

/*struct to save Security Mode Command message till we have received
  Suspend_Cnf from LLC*/
static tdsrrcsmc_security_mode_command_type tdsrrcsmc_saved_security_mode_command_msg;

/*struct to save SRNS Security Info till we have received
  Suspend_Cnf from LLC*/
static tdsrrcsmc_srns_info tdsrrcsmc_saved_srns_info;
 
static tdsrrcsmc_integrity_protection_info_type tdsrrcsmc_integrity_protection_info;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
static tdsrrcsmc_integrity_protection_info_type tdsrrcsmc_integrity_protection_info_internal[TDSCDMA_NUM_SUBS];
#endif

/*the temporary storage for tdsrrcsmc_integrity_protection_info, when that RB is in pending state*/
static tdsrrcsmc_integrity_protection_info_type tdsrrcsmc_integrity_protection_info_dl_check_backup;

/*declare the struct tdsrrcsmc_integrity_protection_info*/
static tdsrrcsmc_ciph_info_type tdsrrcsmc_ciphering_info;

static tdsrrcsmc_security_config_info_type  tdsrrcsmc_int_ciph_info;

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
static tdsrrcsmc_security_config_info_type  tdsrrcsmc_int_ciph_info_internal[TDSCDMA_NUM_SUBS];
#endif

/* contains info about ciphering status/radio bearer*/
static tdsrrcsmc_rb_ciphering_config tdsrrcsmc_ciph_rb_info;

/* contains the ciphering activation time passed to SMC procedure by other
   procedures which are setting up TM radio bearers*/
static uint8 tdsrrcsmc_tm_ciph_act_cfn;

/*boolean to check is START_VALUE_TO_TRANSMIT is set or not*/
boolean TDSSTART_VALUE_TO_TRANSMIT_FLAG;

/*uint32 value to hold START*/
uint32 tdsstart_value_to_transmit;

/*uint32 value to hold START_CS for HHO */
uint32 tdsstart_value_to_transmit_cs_hho;

/*Value to store tdsstart_value_to_transmit for CR1466 implementation*/
uint32 tdsmac_start_value_to_transmit;

/*new tm radio bearer indication flag*/
boolean TDSNEW_TM_RB_IND_FLAG;      

/*domain id of latest updated tdsstart_value_to_transmit */
rrc_cn_domain_identity_e_type tdsstart_value_to_transmit_domain;

boolean tdsl2_ack_rcvd_for_smc_complete_flag=FALSE;
boolean tdsrrcsmc_send_l2_ack_ind_to_ddt_flag=FALSE;
boolean tdsextract_cause_rel_ind=FALSE;

/* This variable is supposed to be used to take back-up of all security related
   global variables. This will be used to revert back to old configuration when
   cell update is triggered during secruity reconfiguration or during physical
   channel establishment failure */
tdsrrcsmc_security_variables_backup tdsbackup_of_security_variables;

/* This variable is used to determine whether RRC needs to revert back
  ciphering related variables or not. If it is set to FALSE then it indicates 
  that SMC dont have to roll back ciphering variables. If it is set to
  TRUE then it indicates that ONLY ciphering related global smc variables needs
  to be reverted back because integrity as well as structures common to
  ciphering and integrity are already rolled back
*/
boolean tdsis_revert_to_old_cipher_variable_reqd = FALSE;

/* This variable is used to store security key set generated in other RAT
  for CS domain .This is done to support spec cr-2567, which say security
  key set to be used at Inter RAT handover to UTRAN will be the one which
  other RAT is being currently using i.e RR activated ciphering key Kc using 
  ciphering mode command message, corresponding UMTS security key set 
  is considered as key set used in other RAT. So to support this save the 
  security key set generated in G-RAT in this variable, Once RR receives 
  ciphering mod command message in G it will send a command to RRC, 
  then RRCSMC will move these saved security variables to its security database.*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
tdsrrcsmc_security_keyset_generated_in_grat_type tdssaved_security_key_set_internal[TDSCDMA_NUM_SUBS];
#endif

tdsrrcsmc_security_keyset_generated_in_grat_type tdssaved_security_key_set;

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
  static tdsrrc_cmd_type * tdsrrcsmc_temp_l2ack_cmd_ptr = NULL;  
  static boolean smc_cmp_mui_drop_status = FALSE;
  static uint8   smc_cmp_mui_drop = 0;
#endif

#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
  boolean tdsrrcsmc_srns_fresh_pending_state = FALSE;     /*for using old fresh for the DL msg in SRNS pending state*/
  boolean tdsrrcsmc_srb3_4_rlc_pending_state = FALSE;     /*for buffereing DDT during SRNS relocatino to avoid UDT loss*/
#endif

/*stores SMC committed IP key value*/
static byte tdsrrcsmc_last_committed_ip_key[RRC_MAX_CN_DOMAINS][RRC_MAX_INTEGRITY_KEY_LENGTH];



#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  boolean tdsrrcsmc_is_fresh_in_ota = FALSE;
#endif


  /*Replacing above old_1, and will phase out old_1 if it is not used*/
  /*tdsrrcsmc_latest_used_ip_key holds the key that the corresponding SRB should currently use*/
static byte tdsrrcsmc_latest_used_ip_key[TDSRRC_MAX_SRB][RRC_MAX_INTEGRITY_KEY_LENGTH];

/*0xFF means invalid*/
static uint8 tdsrrcsmc_non_srb2_srns_ota_pending_key_rb_id = 0xFF; 

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
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ?. 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*===========================================================================

FUNCTION  tdsrrcsmc_print_cipher_int_key

DESCRIPTION
  Prints integrity or ciphering key

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_print_cipher_int_key
(
uint32 key_type,
byte *integrity_key,
byte *ciphering_key
)
{
  uint32 cnt=0;/*local variable use for counting in for loop*/
  uint32 temp_key[4];/*local var used for printing keys*/
  
  /* UIM will own a new EFS and expose this new API to TECHs, which will take SUB ID as input and return TRUE or FALSE 
     based on the SIM type and the new EFS setting. TECH teams need to log security related debug F3s 
     only when the UIM API returns TRUE. */
  if (!mmgsdi_is_test_mode_logging_enabled_for_subscription(tdsrrc_get_as_id()))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"cannot print key for type %d ",key_type);
    return;
  }

  /* Switch on key type for printing key */
  switch (key_type){

    /* Print INTEGRITY KEY */
    case TDSRRC_PRINT_INTEGRITY_KEY:
      for (cnt=0; cnt < (RRC_MAX_INTEGRITY_KEY_LENGTH /4); cnt++)
      {
        temp_key[cnt] = b_unpackd(&(integrity_key[cnt*4]), 0, 32);
      }
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"INT KEY 1: %x 2: %x",temp_key[0],temp_key[1]);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"INT KEY 3: %x 4: %x",temp_key[2],temp_key[3]);
      break;

    /*  Print CIPHERING KEY */
    case TDSRRC_PRINT_CIPHERING_KEY:
      for (cnt=0; cnt < (RRC_MAX_CIPHERING_KEY_LENGTH /4); cnt++)
      {
        temp_key[cnt] = b_unpackd(&(ciphering_key[cnt*4]), 0, 32);
      }
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"CIPHER KEY 1: %x 2: %x",temp_key[0],temp_key[1]);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"CIPHER KEY 3: %x 4: %x",temp_key[2],temp_key[3]);
      break;

    /* Invalid Key type  */
    default:
      /* Log an error message */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid TTI key type %d ",key_type);
      break;
  }
}

/*===========================================================================

FUNCTION rrc_get_start_value_to_transmit

DESCRIPTION
    Temporary solution to fix for our implementatioin of CR1466 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

uint32 tdsrrc_get_start_value_to_transmit
(
void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO debug. tdsmac_start_value_to_transmit is 0x%x",tdsmac_start_value_to_transmit >> TDSRRC_MAC_HFN_OFFSET);
  return (tdsmac_start_value_to_transmit >> TDSRRC_MAC_HFN_OFFSET);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/* This function sends a TDSL1_CPHY_CIPHERING_KEY_REQ to L1, Its defined in
  LLC.We use here to update UL cipher keys.This is done to bypass LLC & posts 
  command directly yo L1
*/

/* =======================================================================
**                            Function Implementation
** ======================================================================= */


/*===========================================================================

FUNCTION tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind

DESCRIPTION
  This function extracts Ciphering Algo from the OTA message to
  internal format of RRC.

DEPENDENCIES
  None.

RETURN VALUE
  Cipher Algo in RRC internal format
  
SIDE EFFECTS
  None

===========================================================================*/
uecomdef_umts_cipher_algo_e_type tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind
(
  void *cipher_algo,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  /*Internal format of ciphering Algo*/
  uecomdef_umts_cipher_algo_e_type cipher_algo_int = UE_UMTS_CIPHER_ALGO_NONE;

  if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
  || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
  )
  {
    tdsrrc_CipheringAlgorithm_r7 *cipher_algo_r7;
    cipher_algo_r7 = (tdsrrc_CipheringAlgorithm_r7 *)cipher_algo;
    switch(*cipher_algo_r7)
    {
      case tdsrrc_CipheringAlgorithm_r7_uea0:
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA0;
        break;
        
      case tdsrrc_CipheringAlgorithm_r7_uea1:
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA1;
        break;
        
      case tdsrrc_CipheringAlgorithm_r7_uea2:
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA2;
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cipher Algo command unknown =%d",*cipher_algo_r7);
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA0;
      break;
    }
  }
  else
  {
    tdsrrc_CipheringAlgorithm *cipher_algo_r3;
    cipher_algo_r3 = (tdsrrc_CipheringAlgorithm *) cipher_algo;
    switch(*cipher_algo_r3)
    {
      case tdsrrc_CipheringAlgorithm_uea0:
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA0;
        break;
        
      case tdsrrc_CipheringAlgorithm_uea1:
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA1;
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cipher Algo command unknown =%d",*cipher_algo_r3);
        cipher_algo_int = UE_UMTS_CIPHER_ALGO_UEA0;
      break;
    }
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ciphering Algo internal format = %d,msg_version = %d[Value 0-R99,3-R7]",cipher_algo_int,msg_version);
  return cipher_algo_int;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind

DESCRIPTION
  This function extracts Integrity Algo from the OTA message to
  internal format of RRC.

DEPENDENCIES
  None.

RETURN VALUE
  Integrity Algo in RRC internal format
  
SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_umts_integrity_protection_algorithm_e_type tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind
(
  void * integrity_algo,
  tdsrrc_msg_ver_enum_type msg_version
)
{
  /*Internal format of Integrity Algo*/
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;

  if(msg_version == TDSMSG_REL7)
  {
    tdsrrc_IntegrityProtectionAlgorithm_r7 *integrity_algo_r7;
    integrity_algo_r7 = (tdsrrc_IntegrityProtectionAlgorithm_r7 *) integrity_algo;
    switch(*integrity_algo_r7)
    {
      case tdsrrc_IntegrityProtectionAlgorithm_r7_uia1:
        integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;
        break;
        
      case tdsrrc_IntegrityProtectionAlgorithm_r7_uia2:
        integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA2;
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Integrity Algo command unknown =%d",*integrity_algo_r7);
        integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;
      break;
    }
  }
  else
  {
    tdsrrc_IntegrityProtectionAlgorithm *integrity_algo_r3;
    integrity_algo_r3 = (tdsrrc_IntegrityProtectionAlgorithm *) integrity_algo;
    switch(*integrity_algo_r3)
    {
      case tdsrrc_IntegrityProtectionAlgorithm_uia1:
        integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Integrity Algo command unknown =%d",*integrity_algo_r3);
        integrity_algo_int = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;
      break;
    }
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Integrity Algo internal format = %d, msg_version =%d[Value 0-R99,3-R7]",integrity_algo_int,msg_version);
  return integrity_algo_int;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r6

DESCRIPTION
  This function extracts ip check info/ integrity protection mode info and
  ciphering mode info from SRNS relocation triggering messages for Release 6 version
  of specs

  NOTE:  There is no R6 utranMobilityInformation message

DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r7
(
  tdsrrcsmc_srns_relocation_security_info_r7_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                               /*Local variable to store message type */             
  
  /* Get the OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
  
  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          
  
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  { 
  case tdsrrc_DL_DCCH_Message_PDU :
    
    msg_type = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    switch (msg_type)
    {

    case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
     {
      tdsrrc_CellUpdateConfirm_r7_IEs *cucnf_ptr = 
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr, cellUpdateConfirm, cellUpdateConfirm_r7);
      
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(cucnf_ptr,tdsrrc_CellUpdateConfirm_r7_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(cucnf_ptr,tdsrrc_CellUpdateConfirm_r7_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
      }
    }
      break;

    case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
     {
      tdsrrc_RadioBearerReconfiguration_r7_IEs *rbrreconfig_ptr = 
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr,radioBearerReconfiguration,radioBearerReconfiguration_r7);

      if(TDSRRCRBR_R7_MSG_IE_PRESENT(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rbrreconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r7_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(rbrreconfig_ptr->cipheringModeInfo);
      }
    }
      break;
 
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
      {
      tdsrrc_RadioBearerRelease_r7_IEs* rb_release_ptr = 
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr,radioBearerRelease,radioBearerRelease_r7);
        
      if( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,integrityProtectionModeInfo))
        {
          srns_info->integrity_info_present = TRUE;
          srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
        }
      if( TDSRRCRBR_R7_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r7_IEs,cipheringModeInfo))
        {
          srns_info->ciphering_mode_info_present = TRUE;
          srns_info->ciphering_mode_info =  &(rb_release_ptr->cipheringModeInfo);
        }
      }
      break;
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
    {
      tdsrrc_RadioBearerSetup_r7_IEs *rb_setup_ptr = 
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr,radioBearerSetup,radioBearerSetup_r7);

      if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_setup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(rb_setup_ptr,tdsrrc_RadioBearerSetup_r7_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(rb_setup_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
     {
      tdsrrc_TransportChannelReconfiguration_r7_IEs *tcreconfig_ptr =
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr,transportChannelReconfiguration,transportChannelReconfiguration_r7);
      
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r7_IEs,integrityProtectionModeInfo) )
        {
          srns_info->integrity_info_present = TRUE;
          srns_info->integrity_protection_mode_info =   &(tcreconfig_ptr->integrityProtectionModeInfo);
        }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r7_IEs,cipheringModeInfo) )
        {
          srns_info->ciphering_mode_info_present = TRUE;
          srns_info->ciphering_mode_info =  &(tcreconfig_ptr->cipheringModeInfo);
        }
     }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
    {
      tdsrrc_PhysicalChannelReconfiguration_r7_IEs *pc_reconfig_ptr = 
        TDSRRC_EXTRACT_R7_MSG_POINTER(dcch_msg_ptr,physicalChannelReconfiguration,physicalChannelReconfiguration_r7);

      if(TDSRRCRBR_R7_MSG_IE_PRESENT(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r7_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(pc_reconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r7_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(pc_reconfig_ptr->cipheringModeInfo);
      }
      }
      break;      
            
    case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
    {
      tdsrrc_UTRANMobilityInformation_r7_IEs *utran_mob_info = 
        TDSRRC_EXTRACT_R7_MSG_POINTER_UMI_URA(dcch_msg_ptr,utranMobilityInformation,utranMobilityInformation_r7);
      
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(utran_mob_info,tdsrrc_UTRANMobilityInformation_r7_IEs,integrityProtectionModeInfo))
        {
          srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(utran_mob_info->integrityProtectionModeInfo);
        }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(utran_mob_info,tdsrrc_UTRANMobilityInformation_r7_IEs,cipheringModeInfo))
        {
          srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(utran_mob_info->cipheringModeInfo);
      }
        }
        break;
        
    case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
    {
      tdsrrc_URAUpdateConfirm_r7_IEs *ura_update_cfn =  
        TDSRRC_EXTRACT_R7_MSG_POINTER_UMI_URA(dcch_msg_ptr,uraUpdateConfirm,uraUpdateConfirm_r7);
      
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(ura_update_cfn,tdsrrc_URAUpdateConfirm_r7_IEs,integrityProtectionModeInfo))
        {
          srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(ura_update_cfn->integrityProtectionModeInfo);
        }
      if(TDSRRCRBR_R7_MSG_IE_PRESENT(ura_update_cfn,tdsrrc_URAUpdateConfirm_r7_IEs,cipheringModeInfo))
        {
          srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(ura_update_cfn->cipheringModeInfo);
      }
        }
        break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_MED,"MSG not supported for SRNS-reloc");
      status = FAILURE;
      break;
      }
      break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc");
    status = FAILURE;
    break;
  }
  
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);      
  }

  return status;
}
/*===========================================================================

FUNCTION tdsrrcsmc_is_integrity_prot_mode_info_present

DESCRIPTION
  This function returns TRUE if IE "integrityProtectionModeInfo" is present in
  configuration message(byte stream), otherwise it returns FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_is_integrity_prot_mode_info_present
(
  uint8 *dl_sdu_ptr,
  uint8  msg_type
)
{
  /* Check whether Integrity Check info is present */
  if (0x00 != b_unpackb(dl_sdu_ptr, TDSINTEGRITY_CHECK_INFO_PRESENT_POS,
                        TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT))
  {
    /* Check whether message contains 'r3' IE or not */
    if (0x00 == b_unpackb(dl_sdu_ptr, TDSDL_DCCH_R3_INDICATOR_POS,
                        TDSNUM_OF_BITS_FOR_DL_DCCH_R3_INDICATOR))
    {
      /* Check whether IE 'integrityProtectionModeInfo' is present or not */
      if (0x00 != b_unpackb(dl_sdu_ptr, 
                  TDSDCCH_R3_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                        TDSNUM_OF_BITS_FOR_DCCH_R3_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
      {
        return(TRUE);
      }
    }
    else /* indicates later-than-r3 */
    {
     /* if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
      {*/
        if((msg_type == T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation) || 
           (msg_type == T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm))
        {
          /* indicates later_than_r3_criticalExtensions_r5_chosen */
          if(0x00 == b_unpackb(dl_sdu_ptr, 
                TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5_POS,
                TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5))
          {
            if(0x00 != b_unpackb(dl_sdu_ptr, 
                TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5_POS + 2,
                TDSNUM_OF_BITS_FOR_DL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
            {
              return (TRUE);
            }
          }
          else
          {
            /* indicates later_than_r3_criticalExtensions_r5_chosen */
            if(0x00 == b_unpackb(dl_sdu_ptr, 
                  TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R7_POS,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5))
            {
              if(0x00 != b_unpackb(dl_sdu_ptr, 
                  TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R7_POS + 3,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
              {
                return (TRUE);
              }
            }
          }
        }
        else
        {
          /* indicates later_than_r3_criticalExtensions_criticalExtensions_chosen */
          if (0x00 != b_unpackb(dl_sdu_ptr, 
          TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_POS,
                TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN))
          {
            
            /* indicates later_than_r3_criticalExtensions_criticalExtensions_r5_chosen */
            if (0x00 == b_unpackb(dl_sdu_ptr, 
                TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R5_POS,
                TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R5))
            { 
              if (0x00 != b_unpackb(dl_sdu_ptr, 
                  TDSDL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
              { 
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR7: SRNS relocation case detected - intProtModeInfo present");
                return(TRUE);
              } 
            } 
            else /* indicates  later_than_r3_criticalExtn_criticalExtn_criticalExtns_chosen */
            {
              /* indicates later_than_r3_criticalExtensions_criticalExtensions_criticalExtensions_r6_chosen */
              if (0x00 == b_unpackb(dl_sdu_ptr, 
                  TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R6_POS,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R6))
              {
                /* check the value of bit which whether integrity protection
                   mode info is included or not */
                if (0x00 != b_unpackb(dl_sdu_ptr, 
                    TDSDL_DCCH_R6_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                    TDSNUM_OF_BITS_FOR_DL_DCCH_R6_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
                { 
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC R6: SRNS relocation case detected - intProtModeInfo present");
                  return(TRUE);
                } 
              } 
              else /* indicates  later_than_r3_critExtn_critExtn_critExtn_critExtn_chosen */
              {
                /* indicates later_than_r3_criticalExtensions_criticalExtensions_criticalExtensions_criticalExtensions_r7_chosen */
                if (0x00 == b_unpackb(dl_sdu_ptr, 
                    TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R7_POS,
                    NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R7))
                {
                  /* check the value of bit which whether integrity protection
                     mode info is included or not */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel-7");
                  if(msg_type == T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration)
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Check integrity_prot_mode_info for rrc_radioBearerReconfiguration_chosen");
                    if (0x00 != b_unpackb(dl_sdu_ptr, 
                       TDSDL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                       TDSNUM_OF_BITS_FOR_DL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR7: SRNS relocation case detected - intProtModeInfo present");
                      return(TRUE);
                    }
                    else
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Integrity_prot_mode_info not present in radioBearerReconfiguration");
                    }
                  }
                  else 
                  {
                    if (0x00 != b_unpackb(dl_sdu_ptr, 
                       TDSDL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS + 1,
                       TDSNUM_OF_BITS_FOR_DL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR7: SRNS relocation case detected - intProtModeInfo present");
                      return(TRUE);
                    } 
                  } 
                } 
                else
                {
#ifdef FEATURE_TDSCDMA_REL8
                   /* indicates later_than_r3_criticalExtensions_criticalExtensions_criticalExtensions_criticalExtensions_criticalExtensions_r8_chosen */
                  if (0x00 == b_unpackb(dl_sdu_ptr, 
                      TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R8_POS,
                      NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R8))
                  {
                    /* check the value of bit which whether integrity protection
                                   mode info is included or not */
                    if (0x00 != b_unpackb(dl_sdu_ptr, 
                      TDSDL_DCCH_R8_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                      TDSNUM_OF_BITS_FOR_DL_DCCH_R8_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
                    { 
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR8: SRNS relocation case detected - intProtModeInfo present");
                      return(TRUE);
                    }
                    else
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR8: Integrity_prot_mode_info not present");
                    }
                  }
                  else
                  {
#ifdef FEATURE_TDSCDMA_REL9
                    /* indicates R9 is choosen*/
                    if (0x00 == b_unpackb(dl_sdu_ptr, 
                        TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_ETC_R9_POS,
                        NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_ETC_R9))
                    {
                      /* check the value of bit which whether integrity protection
                                     mode info is included or not */
                      if (0x00 != b_unpackb(dl_sdu_ptr, 
                        TDSDL_DCCH_R9_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                        TDSNUM_OF_BITS_FOR_DL_DCCH_R9_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
                      { 
                        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR9: SRNS relocation case detected - intProtModeInfo present");
                        return(TRUE);
                      }
                      else
                      {
                        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCR9: Integrity_prot_mode_info not present");
                      }
                    }
#endif
                  }

#endif
                }
              }
            } 
          } 
          else /* indicates later_than_r3_criticalExtensions_r4_chosen */
          { 
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Check R4 integrity_prot_mode_info for rrc_radioBearerReconfiguration_chosen");
            if (0x00 != b_unpackb(dl_sdu_ptr, 
                  TDSDL_DCCH_R4_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_R4_INTEGRITY_PROTECTION_MODE_INFO_PRESENT))
            { 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC R4: SRNS relocation case detected - intProtModeInfo present");
              return(TRUE);
            }

          } 
        }
      /*}*/
    } 
  }

  return (FALSE);
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_latest_configured_cn_domain

DESCRIPTION
  Returns the latest Configured Cn Domain value which is set based on the cnDomainIdentity IE that comes
  in last received Security Mode Command.

DEPENDENCIES
  None.

RETURN VALUE
  Cn Domain

SIDE EFFECTS
  None

===========================================================================*/
rrc_cn_domain_identity_e_type tdsrrcsmc_get_latest_configured_cn_domain(void)
{
  if (FALSE == tdsis_revert_to_old_cipher_variable_reqd)
  {
    return(tdsrrcsmc_int_ciph_info.latest_config_cn_id);
  }
  else
  {
    /* use the latest_config_cn_domain from backup variable because
       structure "tdsrrcsmc_int_ciph_info" is already reverted back to old config
    */
    return(tdsbackup_of_security_variables.rrcsmc_int_ciph_info.latest_config_cn_id);
  }
  
}

/*===========================================================================

FUNCTION tdsrrcsmc_clear_cipher_config_db

DESCRIPTION
  Clears config information in the global ciphering database.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_clear_cipher_config_db(void)
{
  /*reset contents of global ciphering database*/
  tdscipher_config_db.l1_cipher_info.cipher_key_req.req_mask = 0x00;

  /*reset MAC information in global database*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_enable = FALSE;
  tdscipher_config_db.mac_cipher_info[RRC_PS_DOMAIN_CN_ID].cipher_enable = FALSE;

  /*always set mac's cipher config type to NON_HO, only for brief periods of time
    when we are doing Step1 and Step2 ciphering config.. we need to change it*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_cfg_type = 
  TDSNON_HO_CIPHER_CFG;  
  tdscipher_config_db.mac_cipher_info[RRC_PS_DOMAIN_CN_ID].cipher_cfg_type = 
  TDSNON_HO_CIPHER_CFG;   
  tdscipher_config_db.mac_ciph_config_affected = FALSE;
    
  /*reset the number of Ciphered UL and DL RLC channels*/
  tdscipher_config_db.rlc_dl_cipher_info.nchan = 0;
  tdscipher_config_db.rlc_ul_cipher_info.nchan = 0;

  /*case initial start of ciphering due to reception of smc message*/
  /*checking in if just to suppress lint.  will remove once Ciphering
    feature defn. is removed*/
  if(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb != 0)
  {
    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb = 0;
  }

  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb=0;

  /*reset the number of ciphered TM radio bearers*/
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[RRC_CS_DOMAIN_CN_ID].num_rb=0;
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[RRC_PS_DOMAIN_CN_ID].num_rb=0;

  /*set the ciphering affected flag for all domains false*/
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[RRC_CS_DOMAIN_CN_ID].affected=FALSE;
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[RRC_PS_DOMAIN_CN_ID].affected=FALSE;
}
/*===========================================================================

FUNCTION tdsrrcsmc_is_security_reconfig_active

DESCRIPTION
  checks to see if SMC status is in INITIAL.  

DEPENDENCIES
  None.

RETURN VALUE
  Return's TRUE is SMC status is not initial
  Return's FALSE is SMC status is initial

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_is_security_reconfig_active 
(
  void
)
{
  //return's true is SMC substate is not initial, else return's false..
  if(tdsrrcsmc_substate == TDSRRCSMC_INITIAL)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_ciph_key_idx

DESCRIPTION
  Gets the Key Index in L1 representation form.
  CS-KEY0 -> 0 L1 Key Index
  CS-KEY1 -> 1 L1 Key Index
  PS-KEY0 -> 2 L1 Key Index
  PS-KEY1 -> 3 L1 Key Index

DEPENDENCIES
  None.

RETURN VALUE
  Key Index in the form L1 needs

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
static uint8 tdsrrcsmc_get_ciph_key_idx
(
rrc_cn_domain_identity_e_type domain_id
)
{
  /*convert CS or PS key index to L1 key index
    CS-KEY0 -> 0 L1 Key Index
    CS-KEY1 -> 1 L1 Key Index
    PS-KEY0 -> 2 L1 Key Index
    PS-KEY1 -> 3 L1 Key Index
  */
  if (domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
        ciph_keys_info.key_used == 0)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  }
  else
  {
    if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
        ciph_keys_info.key_used == 0)
    {
      return 2;
    }
    else
    {
      return 3;
    }
  }
}
/*lint +e715*/
/*===========================================================================

FUNCTION tdsrrcsmc_init_start_value_at_power_up

DESCRIPTION
  Resets START values upon powerup.  This is done in this function and not
  in the go_to_smc_initial_substate function because START values are
  statically stored by the UE from power-up till power-down, even though
  they are updated to the SIM at every connection release.
  
  So we don't want to reset START values at every connection release.  Hence
  this function is called only at power-up.  On a side note: the need to
  initialize START values outside of SIM_INSERT_REQ or SIM_UPDATE_REQ
  arose because an emergency call may be attempted, in this case the sim is
  not present.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_init_start_value_at_power_up
(
void
)
{
  uint8 i;

  /*copy number of cn domains into int_ciph_info struct*/  
  tdsrrcsmc_int_ciph_info.num_cn_domains = RRC_MAX_CN_DOMAINS;

  /*Initialize the domain Ids*/
  for (i=0; i<RRC_MAX_CN_DOMAINS; i++)
  {
    if( i == 0)
    {
      tdsrrcsmc_int_ciph_info.int_ciph_info[i].domain_id = RRC_CS_DOMAIN_CN_ID;
      
    }
    else
    {
      tdsrrcsmc_int_ciph_info.int_ciph_info[i].domain_id = RRC_PS_DOMAIN_CN_ID;
    }
    /* Set the HFNs to 0 for emergency calls when SIM or USIM is not inserted */
    tdsrrcsmc_int_ciph_info.int_ciph_info[i].hfn[0] = 0x00;
    tdsrrcsmc_int_ciph_info.int_ciph_info[i].hfn[1] = 0x00;
    tdsrrcsmc_int_ciph_info.int_ciph_info[i].hfn[2] = 0x00;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciph_keys_used

DESCRIPTION
  This function updates the ciphering key indices when a Security
  Mode command with START/RESTART is used.  If authentication is
  done for the CN domain for which Ciphering is being START/RESTART
  then the key indices and keys for that cn domain are updated

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_update_ciph_keys_used
(
rrc_cn_domain_identity_e_type domain_id
)
{

  /* update the global variable common to both ciphering and integrity 
     only when 'tdsis_revert_to_old_cipher_variable_reqd' is set to FALSE i.e.
     only when we dont have to revert back to old configuration.
     Else no need to change any data structure because we have already
     reverted back to old data-structure */
  if (FALSE == tdsis_revert_to_old_cipher_variable_reqd)
  {
    /*if a new key is received for this domain id then start using the new key,
      set the new key flag to false and start using the new key.  New keys should
      be only if ciphering is START/RESTART.  For a reconfig we should not move to
      a new key*/
    if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
        ciph_keys_info.new_key == TRUE)
    {
      tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
      ciph_keys_info.new_key = FALSE;
  
      if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
          ciph_keys_info.key_used == 0)
      {
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
        ciph_keys_info.key_used = 1;
      }
      else
      {
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
        ciph_keys_info.key_used = 0;  
      }
    }
  }

}

/*===========================================================================

FUNCTION tdsrrcsmc_go_to_smc_initial_substate

DESCRIPTION
  Resets all stored info from previous SMC messages.  Called when going
  to IDLE state.  START values and keys are not reset, as these are
  statically stored by the UE from power-up to power-down, they can only
  be RESET as power-up, can be updated by SIM_INSERT_REQ or by 
  SIM_UPDATE_REQ only.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_go_to_smc_initial_substate
(
void
)
{
  uint8 i = 0;
  tdsrrcsmc_substate = TDSRRCSMC_INITIAL;

  /*set the reconfig and status flags to false and not started for integrity
    and ciphering global flags*/
  tdsrrcsmc_integrity_protection_info.reconfig = FALSE;
  tdsrrcsmc_integrity_protection_info.ip_status = TDSRRCSMC_SECURITY_NOT_STARTED;        
  tdsrrcsmc_integrity_protection_info.modify_received = FALSE;
  
  /*set latest_config_cn_id_old_1 to CS*/
  for(i=0; i<TDSRRC_MAX_SRB; i++)
  {
    tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i] = RRC_CS_DOMAIN_CN_ID;
  }


  /*set ciphering reconfig info to false*/
  tdsrrcsmc_ciphering_info.reconfig = FALSE;
  
  /*set ciphering status for both cn domains to false*/
  tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] = 
  TDSRRCSMC_SECURITY_NOT_STARTED;
  tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] = 
  TDSRRCSMC_SECURITY_NOT_STARTED;

  /*set is_sync_ind_sent_to_nas status for both cn domains to false*/
  tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_CS_DOMAIN_CN_ID] = FALSE;
  tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_PS_DOMAIN_CN_ID] = FALSE;

  //if authentication was done during a call and SMC not received, then we should update
  //keys indices correctly right here.. when we go to initial substate, we reset all the new_key
  //info and such, hence we might lose the info that we got new keys during a call, and subsequent
  //call ciphering may be affected if auth not done in subsequent call or no power-down
  tdsrrcsmc_update_ciph_keys_used(RRC_CS_DOMAIN_CN_ID);
  tdsrrcsmc_update_ciph_keys_used(RRC_PS_DOMAIN_CN_ID);

  /*set the new keys flag to false initially*/
  tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.
  new_key = FALSE;
  tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].ciph_keys_info.
  new_key = FALSE;

  tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.
  new_key_for_start_value = FALSE;
  tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].ciph_keys_info.
  new_key_for_start_value = FALSE;

  /*set the previous calls ciphering flags to false*/
  TDSNEW_TM_RB_IND_FLAG = FALSE;
  TDSSTART_VALUE_TO_TRANSMIT_FLAG=FALSE;
  tdsstart_value_to_transmit=0;
  tdsstart_value_to_transmit_cs_hho=0;
  tdsmac_start_value_to_transmit=0;
  tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID] = UE_UMTS_CIPHER_ALGO_UEA0;
  tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID] = UE_UMTS_CIPHER_ALGO_UEA0;

 tdsrrcsmc_int_ciph_info.saved_integrity_algo[RRC_CS_DOMAIN_CN_ID] = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;
 tdsrrcsmc_int_ciph_info.saved_integrity_algo[RRC_PS_DOMAIN_CN_ID] = UE_UMTS_INTEGRITY_PROT_ALGO_UIA1;

  /*flag to indicate that START value was updated due to Initial Direct Transfer
  message*/
  tdsrrcsmc_int_ciph_info.start_value_update_flag[RRC_CS_DOMAIN_CN_ID] = FALSE;
  tdsrrcsmc_int_ciph_info.start_value_update_flag[RRC_PS_DOMAIN_CN_ID] = FALSE;

  tdsrrcsmc_int_ciph_info.int_start_status_for_conn[RRC_CS_DOMAIN_CN_ID] = FALSE;
  tdsrrcsmc_int_ciph_info.int_start_status_for_conn[RRC_PS_DOMAIN_CN_ID] = FALSE;

  tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID] = 0;
  tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID] = 0;
  
  tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present = FALSE;

  tdsl2_ack_rcvd_for_smc_complete_flag = FALSE;
  tdsrrcsmc_send_l2_ack_ind_to_ddt_flag= FALSE;

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
  smc_cmp_mui_drop_status = FALSE;
  smc_cmp_mui_drop = 0;
#endif

#ifdef  FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
  tdsrrcsmc_srns_fresh_pending_state = FALSE;     
  tdsrrcsmc_srb3_4_rlc_pending_state = FALSE;  
#endif

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  tdsrrcsmc_is_fresh_in_ota = FALSE;
#endif

  /*clear cipher configuration database as we are going to idle state*/
  tdsrrcsmc_clear_cipher_config_db();
}
/*===========================================================================

FUNCTION tdsrrcsmc_reset_smc_ip_started_flag

DESCRIPTION
  Resets the STATUS flag to  TDSRRCSMC_IP_NOT_STARTED.  This function will be
  called by rrcrce procedure on entering connected mode.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_reset_smc_ip_started_flag(void)
{
  tdsrrcsmc_integrity_protection_info.ip_status = TDSRRCSMC_SECURITY_NOT_STARTED;
}
/*===========================================================================
FUNCTION tdsrrcsmc_is_ciphering_enabled

DESCRIPTION
  This function indicates if ciphering is started or not.
  Right now we return SECURITY started if either cn domains is started,
  because we are not supporting concurrent services.  But when concurrent
  services is supported, this function will take cn domain as argument
  and only STATUS of the queried CN domain will be sent.
  
DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS or FAILURE whether ciphering is started or not respectively.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcsmc_security_status_e_type tdsrrcsmc_is_ciphering_enabled
(
void
)
{
  if ((tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] == 
       TDSRRCSMC_SECURITY_STARTED) ||
      (tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] == 
       TDSRRCSMC_SECURITY_STARTED))
  {
    return TDSRRCSMC_SECURITY_STARTED;
  }
  else
  {
    return TDSRRCSMC_SECURITY_NOT_STARTED;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_tm_ciph_active

DESCRIPTION
  Checks to see if TM-Ciphering is active or not.

DEPENDENCIES
  None.

RETURN VALUE
  True:If TM ciphering is active
  False: If TM ciphering is inactive

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_tm_ciph_active()
{
  return tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_enable;
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_activation_cfn

DESCRIPTION
  Update Ciphering Activation CFN global var.  This value is updated by
  other procedures if they receive a Ciphering Activation CFN from the
  N/W or with the current CFN which they query from L1

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_update_ciphering_activation_cfn
(
uint8 tdsciphering_activation_cfn
)
{
  /*update value of tdsrrcsmc_tm_ciph_act_cfn by RBE or other reconfig procedures*/
  tdsrrcsmc_tm_ciph_act_cfn = tdsciphering_activation_cfn;

  /*foll. 3 lines of code in here just to suppress lint error.
    will remove once Ciphering Feature defn. is removed */
  if(tdsrrcsmc_tm_ciph_act_cfn == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Ciph Act Time set to 0");
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_is_new_tm_rb_setup

DESCRIPTION
  If a new TM Radio Bearer is being added, then send TM RB ciphering act
  CFN to the procedure which configured the TM radio bearers.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_is_new_tm_rb_setup
(
void
)
{
  /*if tm rb's are being setup for the first time return true and reset the
    flag else return false*/
  if (TDSNEW_TM_RB_IND_FLAG == TRUE)
  {
    TDSNEW_TM_RB_IND_FLAG = FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_convert_START_to_count_c_i

DESCRIPTION
  This function converts a byte stream containing START to
  uint32 representation of count_c_i

DEPENDENCIES
  None.

RETURN VALUE
  Returns uint32 which is the count_c_i
===========================================================================*/
static uint32 tdsrrcsmc_convert_START_to_count_c_i
(
byte      * start_byte_stream,
uint32    seq_num
)
{
  uint32 local_count_c_i=0;/*local var to store count_i*/

  /*unpack the 20bits of start to a uint32 local var*/
  local_count_c_i = b_unpackd(start_byte_stream, TDSRRC_DEFAULT_BIT_POS, 
                              TDSRRC_START_VALUE_LENGTH);

  /*move the 20bits left 12 times to generate count_c_i*/
  local_count_c_i <<= 12;

  /*mask the sequence number part of the seq_num var and set the rest 
    of the bits to 0's*/      
  seq_num &= 0xF;

  /*generate count_c_i by bitwise ORing masked HFN and SEQ_NUM*/
  local_count_c_i |= seq_num;

  return local_count_c_i;

}/*tdsrrcsmc_convert_START_to_count_c_i*/
/*===========================================================================

FUNCTION tdsrrcsmc_convert_count_c_i_to_START

DESCRIPTION
  This function extracts to a byte stream the START part of
  uint32 representation of count_c_i

DEPENDENCIES
  None.

RETURN VALUE
  None.
===========================================================================*/
static void tdsrrcsmc_convert_count_c_i_to_START
(
byte      * start_byte_stream,
uint32    tdscount_c_i
)
{
  uint32 local_count_c_i;

  /*copy the contents of count_c_i to a local variable and convert the
    count_c_i variable to an integer value of START by right shifting
    12 bits*/
  local_count_c_i = tdscount_c_i;
  local_count_c_i >>=12;

  b_packd(local_count_c_i, start_byte_stream, TDSRRC_DEFAULT_BIT_POS, 
          TDSRRC_START_VALUE_LENGTH);

}/*tdsrrcsmc_convert_count_c_i_to_START*/
/*===========================================================================

FUNCTION tdsrrcsmc_get_latest_transmitted_start_values

DESCRIPTION
  This interface returns the START values that are updated latest to the
  N/W in any Uplink message.
  This function populates the start value for Signalling RB and for RAB's.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_get_latest_transmitted_start_values
(
tdsrrcsmc_re_est_start_list * start_list
)
{
  /*get the PS domains' HFN to be sent to RLC*/
  if(tdsrrcsmc_int_ciph_info.start_value_update_flag[RRC_PS_DOMAIN_CN_ID] == TRUE)
  {
    start_list->hfn_rab =
    tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn_latest, 0);
  }
  else
  {
    start_list->hfn_rab =
    tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn, 0);
  }

  /*if latest config cn domain is also PS domain, then use same HFN for
    SRB too.. else query the CS domain's HFN and initialize the structure*/
  if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_PS_DOMAIN_CN_ID)
  {
    start_list->hfn_srb = start_list->hfn_rab;

    //RAB is for PS domain, latest config cn domain is also PS domain, so copy
    //PS domain algo for RAB and SRB..

    start_list->ciph_algo_srb = tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];
    start_list->ciph_algo_urb = tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];
  }
  else
  {
    if(tdsrrcsmc_int_ciph_info.start_value_update_flag[RRC_CS_DOMAIN_CN_ID] == TRUE)
    {
      start_list->hfn_srb =
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn_latest, 0);
    }
    else
    {
      start_list->hfn_srb =
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn, 0);
    }

    //RAB is for PS domain, but latest config cn domain is for CS domain, so copy
    //PS domain algo for RAB and CS algo for SRB..
    start_list->ciph_algo_srb = tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID];
    start_list->ciph_algo_urb = tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];
  }

  //convert the count_c's to HFN's by right shifting 12 times for AM RB's
  start_list->hfn_srb >>= TDSRRC_RLC_AM_HFN_OFFSET;
  start_list->hfn_rab >>= TDSRRC_RLC_AM_HFN_OFFSET;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_latest_transmitted_start_value_per_domain

DESCRIPTION
  This interface returns the START value per domain.

DEPENDENCIES
  None.

RETURN VALUE
  START value per domain

SIDE EFFECTS
  None

===========================================================================*/
static uint32 tdsrrcsmc_get_latest_transmitted_start_value_per_domain
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  uint32 local_start;

  /*get the PS domains' HFN to be sent to RLC*/
  if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE)
  {
    local_start =
    tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, 0);
  }
  else
  {
    local_start =
    tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, 0);
  }

  //convert the count_c's to START by right shifting 12 times
  local_start >>= TDSRRC_RLC_AM_HFN_OFFSET;
  return local_start;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_and_reset_start_value_to_transmit

DESCRIPTION
  If a new UM or AM Radio Bearer is being added, we need to send start
  values to the n/w side.  The reconfig procedure queries this start
  value to transmit and also this flag is reset here bcos once this
  start value is sent, the flag has to be reset.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_and_reset_start_value_to_transmit
(
byte * start_value
)
{
  /*if start value to transmit is set, then sent the start value, else set
    flag to false*/
  if (TDSSTART_VALUE_TO_TRANSMIT_FLAG == TRUE)
  {
    tdsrrcsmc_convert_count_c_i_to_START(start_value, tdsstart_value_to_transmit);

    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[tdsstart_value_to_transmit_domain].hfn_latest,
            TDSRRC_MAX_START_LENGTH*sizeof(byte),
           start_value,        
            TDSRRC_MAX_START_LENGTH*sizeof(byte));

    tdsrrcsmc_int_ciph_info.start_value_update_flag[tdsstart_value_to_transmit_domain] 
      = TRUE;
    
    TDSSTART_VALUE_TO_TRANSMIT_FLAG = FALSE;
    return TRUE;    
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_calc_start_value

DESCRIPTION
  Returns the calculated tdsstart_value_to_transmit

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_calc_start_value
(
byte * start_value
)
{
  boolean ret_val = FALSE;
 
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"GET SVTF:%d SVT:0x%x",TDSSTART_VALUE_TO_TRANSMIT_FLAG,tdsstart_value_to_transmit);
  
  if (TDSSTART_VALUE_TO_TRANSMIT_FLAG == TRUE)
  {
    tdsrrcsmc_convert_count_c_i_to_START(start_value, tdsstart_value_to_transmit);
    ret_val = TRUE;    
  }
  return ret_val;
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_start_value_to_transmit

DESCRIPTION
  Returns the calculated start_value_to_transmit

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
uint32 tdsrrcsmc_get_start_value_to_transmit
(
  void
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"GET SVTF:%d SVT:0x%x",TDSSTART_VALUE_TO_TRANSMIT_FLAG,tdsstart_value_to_transmit);
  
  if (TDSSTART_VALUE_TO_TRANSMIT_FLAG == TRUE)
  {
    return tdsstart_value_to_transmit;    
  }

  return 0;
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_latest_transmitted_start_values

DESCRIPTION
  This interface returns the START values that are updated latest to the
  N/W in any Uplink message.
  This function populates the start value for Signalling RB and for RAB's.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_update_start_list_with_latest_transmitted_start_value
(
tdsrrcsmc_re_est_start_list * start_list
)
{
  start_list->hfn_rab = tdsstart_value_to_transmit;    
  start_list->hfn_rab >>= TDSRRC_RLC_AM_HFN_OFFSET;

  if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == tdsrrcsmc_get_start_value_to_transmit_domain())
  {
    start_list->hfn_srb = tdsstart_value_to_transmit; 
    start_list->hfn_srb >>= TDSRRC_RLC_AM_HFN_OFFSET;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_start_value_to_transmit_flag_status

DESCRIPTION
  Returns the status of TDSSTART_VALUE_TO_TRANSMIT_FLAG

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_start_value_to_transmit_flag_status
(
  void
)
{
  if (TDSSTART_VALUE_TO_TRANSMIT_FLAG == TRUE)
    return TRUE;    
  else
    return FALSE;
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_l2_ack_ind_to_dt_procedures

DESCRIPTION
If DT procedures have registered for L2_ACK IND of SMC, then send the IND
cmd to DDT procedure.

DEPENDENCIES
  TRUE/FALSE

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_process_l2_ack_ind_to_dt_procedures
(
  void
)
{
  tdsrrc_cmd_type *int_cmd_ptr;

  if(tdsrrcsmc_send_l2_ack_ind_to_ddt_flag == TRUE)
  {

    /*this cmd indicates to ddt procedure that l2 ack has been received for smc
    command*/
    if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
    {
      /* Fill in the command parameters for TDSRRC_SMC_DDT_L2_ACK_IND */
      int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SMC_DDT_L2_ACK_IND;

      tdsrrc_put_int_cmd(int_cmd_ptr);  

      tdsrrcsmc_send_l2_ack_ind_to_ddt_flag=FALSE;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send L2 Ack Ind to DDT Procedure");
    }
    else
    {
      ERR_FATAL("Unable to allocate memory for: 0x%x cmd",TDSRRC_SMC_DDT_L2_ACK_IND,0,0);
    }  
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_smc_procedure_complete_status

DESCRIPTION
  This status is queried by UDT procedure.  If procedure is waiting for L2_ACK
  for SMC complete or for lower layer ciphering_configuration, then return
  TRUE.  UDT procedure will wait for indication from SMC when L2_ACK is rcvd.
DEPENDENCIES
  TRUE/FALSE

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_smc_procedure_complete_status
(
  rrc_cn_domain_identity_e_type cn_id
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DT queried L2 ACK State");

  if((tdsrrcsmc_substate == TDSRRCSMC_WAIT_FOR_L2_ACK) ||
     (tdsrrcsmc_substate == TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"CN ID=%d,sync_ind_sent status for CS=%d,PS=%d",
      cn_id,
    tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_CS_DOMAIN_CN_ID],
    tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_PS_DOMAIN_CN_ID]);

   if(tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[cn_id] == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"sync_ind for CN ID-%d sent once,so we can send DDT messages to NAS",cn_id);
      return FALSE;
    }
    else
    {
       tdsrrcsmc_send_l2_ack_ind_to_ddt_flag=TRUE;
       return TRUE;
    }
  }
  else
  {
    return FALSE;
  }
}


#ifdef  FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
/*===========================================================================

FUNCTION tdsrrcsmc_get_smc_srns_pending_status_for_dt

DESCRIPTION
  This status is queried by POP DDT msg to NAS
  If the SRB3 is during SRNS pending state, return TRUE
  Otherwise, return false. 
 
  DDT procedure will wait for indication from SMC
  TDSRRC_SMC_SRNS_RLC_RE_EST_CMP
  to pop the saved DDT msg.
 
DEPENDENCIES
  TRUE/FALSE

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_smc_srns_pending_status_for_dt(void)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DDT enquires SRNS pending State");
  return tdsrrcsmc_srb3_4_rlc_pending_state;
}




/*===========================================================================

FUNCTION tdsrrcsmc_send_rlc_re_est_cmp

DESCRIPTION
  Send TDSRRC_SMC_SRNS_RLC_RE_EST_CMP to pop up the buffered DDT
  msgs due to SRNS relocation

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_send_rlc_re_est_cmp (void)
{
    /*this cmd indicates to ddt procedure that l2 ack has been received for smc
    command*/
    tdsrrc_cmd_type *int_cmd_ptr;

    if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
    {
      /* Fill in the command parameters for TDSRRC_SMC_DDT_L2_ACK_IND */
      int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SMC_SRNS_RLC_RE_EST_CMP;
      tdsrrc_put_int_cmd(int_cmd_ptr);  

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send TDSRRC_SMC_SRNS_RLC_RE_EST_CMP to DDT Procedure");
    }
    else
    {
      ERR_FATAL("Unable to allocate memory for: 0x%x cmd",TDSRRC_SMC_SRNS_RLC_RE_EST_CMP,0,0);
    } 

}

#endif /*FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT*/

/*===========================================================================

FUNCTION tdsrrcsmc_update_cphy_key_req

DESCRIPTION
  Copy the contents of locally stored keys into cipher global db.
  This function updates the keys stored locally into the keys to
  be used by L1.  This function is only called when Ciphering is
  START/RESTART, because keys are updated only at this time after
  an authentication is done.  This function is not to be called
  when Radio-Bearers are reconfigured.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_update_cphy_key_req
(
  tdsrrc_ciph_config_e_type config_cause
)
{
  uint32 key_id=0, i=0, j=0; /*local vars for count and key_id count*/

  /*set the L1 STRUCT's for UL and DL ciphering keys*/
  tdscipher_config_db.l1_cipher_info.cipher_key_req.req_mask |= TDSL1_DL_CIPHER_KEY_REQ;
  tdscipher_config_db.l1_cipher_info.cipher_key_req.req_mask |= TDSL1_UL_CIPHER_KEY_REQ;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Updating cphy_key_req with config_cause=%d",config_cause);
  if(config_cause == TDSCIPH_CONFIG_UL_KEYS)
  {
    /*set the number of keys being given to l1 to 4 for UL,as we will be configuring 
      UL keys only.  Two keys per cn domain.2 current/old and 2 current/new
      (depending on the activation time)*/
    tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys = 4;
    tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys = 0;
  }
  else if(config_cause == TDSCIPH_CONFIG_DL_KEYS)
  {
    /*set the number of keys being given to l1 to 4 for DL,as we will be configuring 
      DL keys only.  Two keys per cn domain.2 current/old and 2 current/new
      (depending on the activation time)*/
    tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys = 0;
    tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys = 4;
  }
  else
  {
    /*set the number of keys being given to l1 to 4.  Two keys per cn domain.
      2 current/old and 2 current/new(depending on the activation time)*/
    tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys = 4;
    tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys = 4;
  }
  /*Set the key_id's and key_algorithms and the key values in the global ciphering 
    database for uplink cipher keys. key_id values are from 0-3
    Note: UL and DL keys are the same, but L1 struct requires them in different
    struct variables     */

  /*ul key initialization*/
  if(config_cause != TDSCIPH_CONFIG_DL_KEYS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cipher ul key initialization");
    key_id=0; 
    for (i=0; i<RRC_MAX_CN_DOMAINS; i++)/*for cs and ps cn domain id's*/
    {
      for (j=0; j<TDSMAX_CIPH_KEYS_PER_DOMAIN; j++)/*num of keys per domain*/
      {
        tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.
        key_info[key_id].key_id = (uint8) key_id;
        tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.
        key_info[key_id].algorithm = UE_UMTS_CIPHER_ALGO_UEA1;

        memscpy(tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.key_info[key_id].key_val, 
               TDSL1_NUM_BYTES_CIPHER_KEY*sizeof(byte),
               tdsrrcsmc_int_ciph_info.int_ciph_info[i].ciph_keys_info.key[j],
               RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte)); 
        key_id++;      
      }
    }
  }
  /*dl key initialization*/
  if(config_cause != TDSCIPH_CONFIG_UL_KEYS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cipher dl key initialization");
    key_id=0; 
    for (i=0; i<RRC_MAX_CN_DOMAINS; i++)/*for cs and ps cn domain id's*/
    {
      for (j=0; j<TDSMAX_CIPH_KEYS_PER_DOMAIN; j++)/*num of keys per domain*/
      {
        tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.
        key_info[key_id].key_id = (uint8) key_id;
        tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.
        key_info[key_id].algorithm = UE_UMTS_CIPHER_ALGO_UEA1;
        memscpy(tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.key_info[key_id].key_val,
               TDSL1_NUM_BYTES_CIPHER_KEY*sizeof(byte),
               tdsrrcsmc_int_ciph_info.int_ciph_info[i].ciph_keys_info.key[j],
               RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte));
        key_id++;
      }
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_revert_max_count_c

DESCRIPTION
  This function reverts the maximum count c stored in the temporary variables 
  to the older value, i.e, the value in permanent variable
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_revert_max_count_c
(
  void
)
{
  tdsrrcsmc_max_count_c.count_c_cs = tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID];
  tdsrrcsmc_max_count_c.count_c_ps = tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID];
}

/*===========================================================================

FUNCTION tdsrrcsmc_update_count_c_released_rbs

DESCRIPTION
  This function updates the maximum count c stored in the temporary variables to the permanent one.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

void tdsrrcsmc_update_count_c_released_rbs
(
void
)
{
  tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID]=tdsrrcsmc_max_count_c.count_c_cs;
  tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID]=tdsrrcsmc_max_count_c.count_c_ps;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_max_start_per_domain

DESCRIPTION
  This function gets the max start value on a domain basis

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static uint32 tdsrrcsmc_get_max_start_per_domain
(
rrc_cn_domain_identity_e_type domain_id
)
{
  uint32 i=0;                         /*local variables used for counting 
                                        in for loop*/

  uint32 max_start=0;
  
  /*rlc_hfn_info_type to calculate tdsstart_value_to_transmit*/
  tdsrlc_hfn_req_type local_hfn_type;
  
  /*tdsmac_hfn_info_type to calculate tdsstart_value_to_transmit*/
  tdsmac_hfn_info_type mac_hfn;      
  
  /*local vars to store count_i, tdsrrcllc_seq_num, START list used for 
    manipulations in this function only*/
  uint32 max_count_i_ul=0, max_count_i_dl=0, max_count_i = 0, max_start_i=0;  
  uint32 max_start_c_ul=0, max_start_c_dl=0, max_start_c=0, max_update=0;    
  uint32 local_lc_id;


  local_hfn_type.ul_nchan =0;
  local_hfn_type.dl_nchan =0;

  memset(local_hfn_type.hfn_per_rb_ul,0,((TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY) * sizeof(tdsrlc_hfn_type)));

  memset(local_hfn_type.hfn_per_rb_dl,0,((TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY) * sizeof(tdsrlc_hfn_type)));


  for ( i=0; i < TDSRRC_MAX_SRB; i++)
  {
    if(tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
       dl_count_i_act_time_pending[i] != TRUE)
    {
      max_count_i_dl = 
        (max_count_i_dl < tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[i])?
        tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[i]:max_count_i_dl; 
    }
  }        

  /*get the max of count_i's for uplink by looping through ul count_i's*/    
  for ( i=0; i < TDSRRC_MAX_SRB; i++)
  {
    if(tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
       ul_count_i_act_time_pending[i] != TRUE)
    {
      max_count_i_ul = 
        (max_count_i_ul < tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].ul_count_i[i])?
        tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].ul_count_i[i]:max_count_i_ul; 
    }
  }        

  /*get the max of ul or dl count_i for this domain*/    
  max_count_i = (max_count_i_ul > max_count_i_dl)? max_count_i_ul:max_count_i_dl;

  /*START is 20MSB of count_i which is 32bits.
  */
  max_start_i = max_count_i >> 12;
  
  if(tdsrrcsmc_int_ciph_info.ciph_status[domain_id] == TDSRRCSMC_SECURITY_STARTED)
  {
    /*get the latest RLC HFN's*/
    (void) tdsrlc_rrc_get_hfn_req(&local_hfn_type);

    for (i=0; i < local_hfn_type.ul_nchan; i++)
    {
      local_lc_id = local_hfn_type.hfn_per_rb_ul[i].rlc_id;
      /*check if lc_id of the Radio Bearer belongs to UM Mode*/           
      if (local_lc_id >= TDSRRCLCM_UL_UM_START_IDX &&
          local_lc_id < TDSRRCLCM_UL_AM_DATA_START_IDX)
      {
        /*since UM mode RB's HFN is 25 bits, remove the LSB 5 bits
          to get START value*/      
        local_hfn_type.hfn_per_rb_ul[i].hfn >>= 5;
      }
      
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_CS_DOMAIN_CN_ID)
        {
          switch(tdsrrclcm_get_ul_rb_id((uint8)local_lc_id))
          {
          case DCCH_AM_RADIO_BEARER_ID:
          case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
          case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          case DCCH_UM_RADIO_BEARER_ID:
            break;
            
          default:
            {
              local_hfn_type.hfn_per_rb_ul[i].hfn = 0;

            }

            break;
          }
        }
        else
        {
          local_hfn_type.hfn_per_rb_ul[i].hfn = 0;        
        }
      }
      else if(domain_id == RRC_PS_DOMAIN_CN_ID)
      {
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_CS_DOMAIN_CN_ID)
        {
          switch(tdsrrclcm_get_ul_rb_id((uint8)local_lc_id))
          {
          case DCCH_AM_RADIO_BEARER_ID:
          case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
          case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          case DCCH_UM_RADIO_BEARER_ID:
            local_hfn_type.hfn_per_rb_ul[i].hfn = 0;          
            break;
            
          default:
            break;
          }
        }
      }
    }
  
  
    for (i=0; i < local_hfn_type.dl_nchan; i++)
    {
      local_lc_id = local_hfn_type.hfn_per_rb_dl[i].rlc_id;
      /*check if lc_id of the Radio Bearer belongs to UM Mode*/           
      if (local_lc_id >= TDSRRCLCM_DL_UM_START_IDX &&
          local_lc_id < TDSRRCLCM_DL_AM_DATA_START_IDX)
      {
        /*since UM mode RB's HFN is 25 bits, remove the LSB 5 bits
          to get START value*/      
        local_hfn_type.hfn_per_rb_dl[i].hfn >>= 5;
      }
      
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_CS_DOMAIN_CN_ID)
        {
          switch(tdsrrclcm_get_dl_rb_id((uint8)local_lc_id))
          {
          case DCCH_AM_RADIO_BEARER_ID:
          case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
          case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          case DCCH_UM_RADIO_BEARER_ID:
            break;
            
          default:
            local_hfn_type.hfn_per_rb_dl[i].hfn = 0;
            break;
          }
        }
        else
        {
          local_hfn_type.hfn_per_rb_dl[i].hfn = 0;        
        }
      }
      else if(domain_id == RRC_PS_DOMAIN_CN_ID)
      {
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_CS_DOMAIN_CN_ID)
        {
          switch(tdsrrclcm_get_dl_rb_id((uint8)local_lc_id))
          {
          case DCCH_AM_RADIO_BEARER_ID:
          case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
          case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
          case DCCH_UM_RADIO_BEARER_ID:
            local_hfn_type.hfn_per_rb_dl[i].hfn = 0;          
            break;
            
          default:
            break;
          }
        }
      }      
    }

    if(local_hfn_type.ul_nchan > 0)
    {
      /*get the max of all the rlc UL HFN's*/
      max_start_c_ul = local_hfn_type.hfn_per_rb_ul[0].hfn;
    }
  
    for (i=1; ((i < local_hfn_type.ul_nchan) && (i < (TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))); i++)
    {
      max_start_c_ul = (max_start_c_ul < local_hfn_type.hfn_per_rb_ul[i].hfn)?
                        local_hfn_type.hfn_per_rb_ul[i].hfn: max_start_c_ul;
    }
  
    if(local_hfn_type.dl_nchan > 0)
    {    
      /*get the max of all the rlc DL HFN's*/  
      max_start_c_dl = local_hfn_type.hfn_per_rb_dl[0].hfn;
    }

    for (i=1; ((i < local_hfn_type.dl_nchan) && (i < (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))); i++)
    {
      max_start_c_dl = (max_start_c_dl < local_hfn_type.hfn_per_rb_dl[i].hfn)?
                        local_hfn_type.hfn_per_rb_dl[i].hfn: max_start_c_dl;
    }
  
    /*select the greater and copy to max_start_c*/
    max_start_c = (max_start_c_dl > max_start_c_ul)? max_start_c_dl: max_start_c_ul;
  
    if((domain_id == RRC_CS_DOMAIN_CN_ID) && 
     (tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_enable == TRUE))
    {
      /*get the latest MAC HFN's*/
      (void) tdsmac_rrc_get_hfn_info(&mac_hfn);

      /*since MAC HFN is 24 bit number, right shift by 4 to get START value
        from MAC COUNT-C*/
      mac_hfn.cs_hfn >>= 4;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MAC HFN CS : %x ,MAC HFN PS : %x",mac_hfn.cs_hfn,mac_hfn.ps_hfn);
      
      max_start_c = (max_start_c < mac_hfn.cs_hfn)? mac_hfn.cs_hfn : max_start_c;

    }   
  }

  if(domain_id==RRC_CS_DOMAIN_CN_ID)
  {
    max_start_c = (max_start_c<tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID])?
                 tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID] : max_start_c;
    /*Reset the HFN value*/
    tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_CS_DOMAIN_CN_ID]=0;
    tdsrrcsmc_max_count_c.count_c_cs=0;
  }
  else
  {
    max_start_c = (max_start_c<tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID])?
                 tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID] : max_start_c;
    /*Reset the HFN value*/
    tdsrrcsmc_int_ciph_info.max_count_c_rel_rbs[RRC_PS_DOMAIN_CN_ID]=0;
    tdsrrcsmc_max_count_c.count_c_ps=0;
  }
  
  max_start = (max_start_c > max_start_i)? max_start_c:max_start_i;
  
  if((tdsextract_cause_rel_ind == TRUE) && 
     (tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE))
  {
    max_update = tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, 0);
    
    /*since COUNT-C is 32 bit number, right shift by 12 to get START value
      from COUNT-C*/
    max_update >>= 12;

    max_start = (max_start > max_update) ? max_start:max_update;
  }

  return max_start;
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_global_ciph_db_for_ciph_start

DESCRIPTION
  This function updates the global ciphering database when a reconfig
  message is received.  This function is called due to addition of
  RB's or deletion of RB's due to RBE or RBR procedures.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_update_global_ciph_db_for_ciph_start
(
  rrc_cn_domain_identity_e_type latest_config_cn_domain,
  tdsrrc_ciph_config_e_type config_cause
)
{
  uint32 i=0;

  /*copy the contents of locally stored keys into cipher global db*/
  tdsrrcsmc_update_cphy_key_req(config_cause);

  /*update the key indices if incase authentication is done.*/
  tdsrrcsmc_update_ciph_keys_used(latest_config_cn_domain);

  /*reset nchan's for rlc to zero, as we only send incremental ciphering info
    for RLC RB's*/
  tdscipher_config_db.rlc_ul_cipher_info.nchan = 0;
  tdscipher_config_db.rlc_dl_cipher_info.nchan = 0;

  /* NOTE: there is no need to check for 'tdsis_revert_to_old_cipher_variable_reqd' when we 
     use variables <like tdsrrcsmc_int_ciph_info> which are already rolled back because
     immediately after this we are going to revert back ciphering related variable to old 
     configuration and whatever configuration we are going to send RLC will  be aborted, so 
     even if this configuration is slightly corrupted then also it is fine */

  /*loop through the number of AM radiobearers configured and update global
    database used by LLC to configure RLC with ciphering parameters*/
  for (i=0; i< tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb; i++)
  {
    /*if domain id is same as domain id of latest configured cn domain only then
      update ciphering parameters*/
    if(
#ifdef FEATURE_LTE_TO_TDSCDMA
        (TDSCIPH_CONFIG_CS_PLUS_PS == config_cause)||
#endif        
        (latest_config_cn_domain == 
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id))
    {
      /*set HFN*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn = 
      tdsrrcsmc_convert_START_to_count_c_i(
        tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id].hfn, 0);

      /*change HFN offset for RLC AM*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn >>= TDSRRC_RLC_AM_HFN_OFFSET;

      /*set rb_id and lc_id*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].rb_id = 
      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_id;

      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_id);

      /*set ciphering activation time*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn = 
      tdsrrcsmc_ciphering_info.ul_ciph_act_info.sq_num[tdscipher_config_db.rlc_ul_cipher_info.
      ciphering[i].lc_id];

      /*set cipher mode to true*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_mode = TRUE;
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_algo = 
          tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id];

      /*set key index*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_key_id = 
      tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id);

      /*set flag to ciph_started*/
      if (tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_status == TDSCIPH_FIRST_TIME)
      {
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_status=TDSCIPH_STARTED;
      }

      /*increment the number of channels by 1*/
      tdscipher_config_db.rlc_ul_cipher_info.nchan++;
    }
  }

  /*loop through the number of UM radiobearers configured and update global
    database used by LLC to configure RLC with ciphering parameters*/
  for (i=0; i < tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb; i++)
  {
    /*if domain id is same as domain id of latest configured cn domain only then
      update ciphering parameters*/
    if (latest_config_cn_domain == 
        tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id)
    {
      /*set HFN*/
      if(tdscipher_config_db.rlc_ul_cipher_info.nchan < (TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].hfn = 
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id].hfn, 0);

        /*change HFN offset for RLC UM rb*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].hfn >>= TDSRRC_RLC_UM_HFN_OFFSET;

        /*set rb_id and lc_id*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].rb_id = 
        tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_id;

        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.rlc_ul_cipher_info.
        nchan].lc_id = 
        tdsrrclcm_get_ul_rlc_lc_id(tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_id);

        /*set ciphering activation sequence number*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].ciph_act_sn = 
        tdsrrcsmc_ciphering_info.ul_ciph_act_info.sq_num[tdscipher_config_db.rlc_ul_cipher_info.
          ciphering[tdscipher_config_db.rlc_ul_cipher_info.nchan].lc_id];

        /*set cipher mode to true*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].cipher_mode = TRUE;
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
          rlc_ul_cipher_info.nchan].ciph_algo = 
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id];

        /*set key index*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[tdscipher_config_db.
        rlc_ul_cipher_info.nchan].cipher_key_id = 
        tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id);
      }
      else
      {
        ERR_FATAL("Invalid number of channels received",0,0,0);
      }

      /*set flag to ciph_started*/
      if (tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_status == TDSCIPH_FIRST_TIME)
      {
        tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_status=TDSCIPH_STARTED;
      }

      /*increment number of channels*/
      tdscipher_config_db.rlc_ul_cipher_info.nchan++;
    }
  }

  /*if affected status of TM RB's for the latest CN domain is true*/
  if (tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[latest_config_cn_domain].
      affected == TRUE)
  {
    /*set cipher mode to true*/
      tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].cipher_algo = 
           tdsrrcsmc_int_ciph_info.saved_ciph_algo[latest_config_cn_domain];
    
    /* The config type should be set to SMC_INITIATED_NON_HO_CIPHER_CFG only for the 
       following scenarios:
       1. Ciphering change for TM bearers due to SMC that would result in a second
       pending config for MAC.i.e MAC already has a pending configuration(due to 
       Step-2) and before that Act time is reached there is a SMC that would result 
       in a second pending config.To determine whether something is already pending in
       MAC,use the API provided by MAC   
       2. When we already have TM bearers established and this SMC is starting ciphering
       on TM bearers for the first time.

       The config type should NOT be set to SMC_INITIATED_NON_HO_CIPHER_CFG for the 
       following scenarios:For these scenarios,the config type is already set 
       appropriately before we reach this point
       1. Ciphering start during a GTOW Handover
       2. TM bearers are established and are already ciphered.There is no pending config
       in MAC.Now we get a SMC that modifies the ciphering configuration for TM bearers.
    */
    if((tdsrrcsmc_substate == TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF) &&
       (TRUE == tdsmac_rrc_get_cipher_pending_status()))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSSMC_INITIATED_NON_HO_CIPHER_CFG to MAC (Second Pending)");
      tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].cipher_cfg_type = 
        TDSSMC_INITIATED_NON_HO_CIPHER_CFG;
    }

    /* if 'cipher_enable' is FALSE then it means that ciphering(UEA0 or UEA1) is 
       going to start in TM due to this SMC i.e. during the on-going RRC connection
       this is first time UE received SMC with 'ciphering mode info'. Hence 
       indicate MAC with config_type set to 'SMC_INITIATED_NON_HO_CIPHER_CFG' 
    */ 
    if (FALSE == tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].cipher_enable)
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cipher Mode Info received in SMC for the first time during this rrc connection.");
       tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].cipher_cfg_type = 
         TDSSMC_INITIATED_NON_HO_CIPHER_CFG;
    }

    tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].cipher_enable = TRUE;

    /*check if dpch ciph activation time is present or not and flag error*/
    if (tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present)
    {
      tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].
      ciph_act_time = tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time; 
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC did not contain DPCH info");      
    }

    /*set HFN and offset with MAC requirement*/
    tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].hfn = 
    tdsrrcsmc_convert_START_to_count_c_i(
      tdsrrcsmc_int_ciph_info.int_ciph_info[latest_config_cn_domain].hfn, 0);     

    tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].hfn >>= 
    TDSRRC_MAC_HFN_OFFSET;

    /*set key index*/
    tdscipher_config_db.mac_cipher_info[latest_config_cn_domain].key_idx =
    tdsrrcsmc_get_ciph_key_idx(latest_config_cn_domain);

    //set this flag to indicate that mac's ciphering configuration is not affected.
    //configuration to mac should not be sent by LLC if this flag is FALSE.
    tdscipher_config_db.mac_ciph_config_affected = TRUE;

  }
  else
  {
    //set this flag to indicate that mac's ciphering configuration is not affected.
    //configuration to mac should not be sent by LLC if this flag is FALSE.
    tdscipher_config_db.mac_ciph_config_affected = FALSE;
  }

  /*copy the uplink information to dl information*/
  tdscipher_config_db.rlc_dl_cipher_info.nchan = 
  tdscipher_config_db.rlc_ul_cipher_info.nchan;

  for (i=0; i<tdscipher_config_db.rlc_ul_cipher_info.nchan; i++)
  {
    /*set key id, hfn, cipher_mode, rb_id and activation time for UM/AM*/
    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].cipher_key_id = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_key_id;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].cipher_mode =
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_mode;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn =
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].rb_id = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].rb_id;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_algo = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_algo;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].lc_id =
    tdsrrclcm_get_dl_rlc_lc_id(tdscipher_config_db.rlc_dl_cipher_info.
                            ciphering[i].rb_id);

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn =
    tdsrrcsmc_ciphering_info.dl_ciph_act_info.sq_num_for_complete
    [tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].rb_id];
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_global_ciph_db_for_reconfig

DESCRIPTION
  This function updates the global ciphering database when a reconfig
  message is received.  This function is called due to addition of
  RB's or deletion of RB's due to RBE or RBR procedures.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_update_global_ciph_db_for_reconfig
(
void
)
{
  /*local vars to maintain count*/
  uint32 i;
  uint32 num_ul_rb_added;
  boolean is_start_val_reached_threshold = FALSE;

  /*reset nchan's for rlc to zero, as we only send incremental ciphering info
    for RLC RB's*/
  tdscipher_config_db.rlc_ul_cipher_info.nchan = 0;
  tdscipher_config_db.rlc_dl_cipher_info.nchan = 0;

  tdsstart_value_to_transmit = tdsrrcsmc_get_max_start_per_domain(tdsstart_value_to_transmit_domain);
  if(TDSSTART_THRESHOLD == tdsstart_value_to_transmit)
  {
    //don't increment the start value, ref TS25.331 ch 8.5.9
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max Start Value=0x%x reached the Threshold",tdsstart_value_to_transmit);
    is_start_val_reached_threshold = TRUE;
  }
  else
  {
    /*increment by 2*/
    tdsstart_value_to_transmit = (tdsrrcsmc_get_max_start_per_domain(tdsstart_value_to_transmit_domain))+2;
  }
  tdsstart_value_to_transmit = (tdsstart_value_to_transmit << 12);

  //only if tm bearers are being setup, update tdsmac_start_value_to_transmit, as MAC is interesed only
  //in TM bearer start value.. use regular tdsstart_value_to_transmit for all other SRB's or RAB's which
  //are committed by RRC itself and not used by MAC separately..
  if(tdsstart_value_to_transmit_domain == RRC_CS_DOMAIN_CN_ID)
  {
    tdsmac_start_value_to_transmit = tdsstart_value_to_transmit;
    /* Implementation of Spec CR 2284R1. When TM bearers are getting established and if the same 
       message (RBSetup) is also used to perform a Timing Re-initialized HHO, then at activation time
       the HFN component of Count-C should be incremented by 1. In our implementation, for the first 
       time TM setup, MAC is given a NON_HO_CIPHER_CFG and at activation time, MAC queries RRC for 
       the new HFN. RRC will return 'tdsmac_start_value_to_transmit' which is populated at this point.
       Hence, check for HHO condition and if it is true, increment the value by 1 here itself so that
       later the the correct value will be given to MAC in the query function. Make sure the increment
       is done for "Timing re-initialised HHO" cases only i.e. only for DCH->DCH scenarios */
    if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && 
       (tdsrrcllc_timing_init_in_progress()) &&
       (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()) &&
       (TDSOC_SET_FOR_CELL_DCH == tdsrrcllc_get_ordered_config_status()) &&
        !(is_start_val_reached_threshold))
    {
      tdsmac_start_value_to_transmit = tdsmac_start_value_to_transmit >> TDSRRC_MAC_HFN_OFFSET;
      tdsmac_start_value_to_transmit += 1;
      tdsmac_start_value_to_transmit = tdsmac_start_value_to_transmit << TDSRRC_MAC_HFN_OFFSET;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incremented tdsmac_start_value_to_transmit by 1 for TI HHO condition. Mac Start Value 0x%x",tdsmac_start_value_to_transmit);
    }
  }

  for (i=0; i< tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb; i++)
  {
    /*only if starting ciphering for this RB, we should set ciphering parameters.
      for RLC we only give ciphering information for RB's for which ciphering is
      being STARTED or RESTARTED.*/
    if ((tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_status == TDSCIPH_FIRST_TIME) &&
        (tdsrrcsmc_int_ciph_info.ciph_status[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id] 
         == TDSRRCSMC_SECURITY_STARTED))
    {
      /*set rb id and lc_id*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].rb_id = 
      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_id;

      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_id);

      /*we will start ciphering immediately for rlc rb's, so set act sequence
        number to zero*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn = 0;

      /*set cipher mode to true*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_mode = TRUE;
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_algo = 
          tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id];

      /*set HFN and offset for AM RB's*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn = 
      (tdsstart_value_to_transmit >> TDSRRC_RLC_AM_HFN_OFFSET);

      /*if SRB get cipher key id of latest configured cn domain, otherwise
        set key id of the domain for which this RB belongs*/
      if (tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].is_rb_srb == TRUE)
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_key_id = 
        tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_int_ciph_info.latest_config_cn_id);
      }
      else
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_key_id = 
        tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].domain_id);
      }

      /*set ciphering status to STARTED and increment number of channels*/
      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[i].rb_status = TDSCIPH_STARTED;
      tdscipher_config_db.rlc_ul_cipher_info.nchan++;
    }
  }

  num_ul_rb_added = tdscipher_config_db.rlc_ul_cipher_info.nchan;

  for (i=0; i<tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb; i++,num_ul_rb_added++)
  {

    /*only if starting ciphering for this RB, we should set ciphering parameters.
      for RLC we only give ciphering information for RB's for which ciphering is
      being STARTED or RESTARTED.*/
    if ((tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_status == TDSCIPH_FIRST_TIME) &&
        (tdsrrcsmc_int_ciph_info.ciph_status[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id] 
         == TDSRRCSMC_SECURITY_STARTED))    
    {
      /*set rb_id and lc_id*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].rb_id = 
      tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_id;
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_id);

      /*we will start ciphering immediately for rlc rb's, so set act sequence
            * number to zero*/
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].ciph_act_sn = 0;

      /*set cipher mode to true*/
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].cipher_mode = TRUE;
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].ciph_algo = 
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id];

      /* Set HFN and offset form UM RB's*/
      {
        /* Legacy Code */
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].hfn = 
        (tdsstart_value_to_transmit >> TDSRRC_RLC_UM_HFN_OFFSET);
      }

      /*if SRB get cipher key id of latest configured cn domain, otherwise
        set key id of the domain for which this RB belongs*/
      if (tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].is_rb_srb == TRUE)
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].cipher_key_id = 
        tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_int_ciph_info.latest_config_cn_id);
      }
      else
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[num_ul_rb_added].cipher_key_id = 
        tdsrrcsmc_get_ciph_key_idx(tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].domain_id);
      }

      /*set ciphering status to STARTED and increment number of channels*/
      tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[i].rb_status=TDSCIPH_STARTED;
      tdscipher_config_db.rlc_ul_cipher_info.nchan++;
    }
  }

  /*set the global database for TM radio bearers*/
  for (i=0; i < RRC_MAX_CN_DOMAINS; i++)
  {
    /*only if the ciphering status is affected*/
    if ((tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[i].affected == TRUE) &&
        (tdsrrcsmc_int_ciph_info.ciph_status[i] == TDSRRCSMC_SECURITY_STARTED))
    {
      /*set ciphering enable flag to true*/
        tdscipher_config_db.mac_cipher_info[i].cipher_enable = TRUE;
      tdscipher_config_db.mac_cipher_info[i].cipher_algo = 
               tdsrrcsmc_int_ciph_info.saved_ciph_algo[i];

      /*set tm ciphering activation time*/
      tdscipher_config_db.mac_cipher_info[i].ciph_act_time = 
      tdsrrcsmc_tm_ciph_act_cfn;

      /*set hfn to latest HFN sent in the last OTA message*/
      //if start value is updated, then send start value from hfn_latest
      if(tdsrrcsmc_int_ciph_info.start_value_update_flag[i] == TRUE)
      {
        
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Start-increment btwn SMC and RBS???");

        /*copy the contents of hfn_latest into mac ciphering database*/
        tdscipher_config_db.mac_cipher_info[i].hfn = 
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[i].hfn_latest, 0);
      }
      else
      {
        /*copy the contents of hfn into mac ciphering database*/
        tdscipher_config_db.mac_cipher_info[i].hfn = 
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[i].hfn, 0);
      }

      /*offset HFN for TM rb*/
      tdscipher_config_db.mac_cipher_info[i].hfn = 
      (tdscipher_config_db.mac_cipher_info[i].hfn >> TDSRRC_MAC_HFN_OFFSET);

      /*get key index*/
      tdscipher_config_db.mac_cipher_info[i].key_idx =
      tdsrrcsmc_get_ciph_key_idx((rrc_cn_domain_identity_e_type) i);
    }
  }

  /*set the number of channels in downlink*/
  tdscipher_config_db.rlc_dl_cipher_info.nchan = 
  tdscipher_config_db.rlc_ul_cipher_info.nchan;

  /*populate ciphering config db for dl database too, convert UL ciphering
    config information to DL ciphering config information*/
  for (i=0; i<tdscipher_config_db.rlc_ul_cipher_info.nchan; i++)
  {
    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].cipher_key_id = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_key_id;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].cipher_mode =
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].cipher_mode;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn =
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].rb_id = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].rb_id;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_algo = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_algo;

    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].lc_id =
    tdsrrclcm_get_dl_rlc_lc_id(tdscipher_config_db.rlc_dl_cipher_info.
                            ciphering[i].rb_id);

    /*set ciph act sn for dl to be same as UL, as we do not receive
      any ciphering activation time for RLC radio bearers in a 
      reconfig message*/
    tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn = 
    tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_sync_ind_info_for_signalling_release

DESCRIPTION
  Function to update ciphering database if signalling connection release is
  received.  This cleans up the sync indication variable .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_update_sync_ind_info_for_signalling_release
( 
  rrc_cn_domain_identity_e_type domain_id 
)
{
    tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[domain_id] = FALSE;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SYNC IND being initialized for domain id %d ",domain_id);
}
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_config_info_for_signalling_release

DESCRIPTION
  Function to update ciphering database if signalling connection release is
  received.  This cleans up the ciphering database, as RB-Release will/may
  not be released in this case.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_update_ciphering_config_info_for_signalling_release
( 
  rrc_cn_domain_identity_e_type domain_id 
)
{
  /*reset the tm ciphering info*/
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[domain_id].affected = FALSE;
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[domain_id].num_rb = 0;
  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[domain_id].rb_status = TDSCIPH_NOT_STARTED;
  
  /* set ciphering enabled flag to false */
  tdscipher_config_db.mac_cipher_info[domain_id].cipher_enable = FALSE;
  
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_cfg_type = 
  TDSNON_HO_CIPHER_CFG;  
}

/*===========================================================================

FUNCTION tdsrrcsmc_calc_max_count_c_released_rb

DESCRIPTION
  This function calculates and stores the maximum count_c values for all released rbs for both domains in a temporary variable.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_calc_max_count_c_released_rb
(
  tdsrrcllc_rlc_config_req_list_type *rlc_req_type_ptr,
  rrc_cn_domain_identity_e_type  domain_id_list[TDS_MAX_RB],
  tdsrrc_proc_e_type proc_id
)
{
  uint32 max_count_c_cs=0;
  uint32 max_count_c_ps=0;
  uint32 i=0;
  tdsrrc_RB_Identity local_rb_id;
  uint32 local_hfn_per_ul_rb[TDS_MAX_RB],local_hfn_per_dl_rb[TDS_MAX_RB];

  uecomdef_logch_mode_e_type local_mode;
  rlc_lc_id_type local_lc_id;
  
  /*rlc_hfn_info_type to calculate tdsstart_value_to_transmit*/
  tdsrlc_hfn_req_type local_hfn_type;
  
  /*tdsmac_hfn_info_type to calculate tdsstart_value_to_transmit*/
  tdsmac_hfn_info_type mac_hfn;      
  
  rrc_cn_domain_identity_e_type local_cn_domain_id;

  local_hfn_type.ul_nchan =0;
  local_hfn_type.dl_nchan =0;

  memset(local_hfn_type.hfn_per_rb_ul,0,((TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY) * sizeof(tdsrlc_hfn_type)));

  memset(local_hfn_type.hfn_per_rb_dl,0,((TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY) * sizeof(tdsrlc_hfn_type)));



  (void) tdsrlc_rrc_get_hfn_req(&local_hfn_type);
  
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Max count c before releasing RB for cs domain = %x ps domain = %x ul_nchan: %d,dl_nchan: %d",
                               tdsrrcsmc_max_count_c.count_c_cs,tdsrrcsmc_max_count_c.count_c_ps,
                               local_hfn_type.ul_nchan,local_hfn_type.dl_nchan);

  /*store the hfn values for each dl UM/AM RBs*/
  for(i=0;i<TDS_MAX_RB;i++)
  {
    local_hfn_per_dl_rb[i] = 0;
    local_hfn_per_ul_rb[i] = 0;
  }
  
  
  for(i=0;i<local_hfn_type.dl_nchan;i++)
  {
    local_lc_id=local_hfn_type.hfn_per_rb_dl[i].rlc_id;
    local_rb_id=tdsrrclcm_get_dl_rb_id(local_lc_id);
    
    /* Processing SRB's first skipping SRB0 */
    switch(local_rb_id)
    {
      case CCCH_RADIO_BEARER_ID:
        local_hfn_per_dl_rb[local_rb_id]=0;
        break;
      case DCCH_AM_RADIO_BEARER_ID:
      case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
      case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
      case DCCH_UM_RADIO_BEARER_ID:
        local_hfn_per_dl_rb[local_rb_id]=local_hfn_type.hfn_per_rb_dl[i].hfn;
        break;
      default:
      {
        if(local_rb_id < TDS_MAX_RB)
        {
          /* Now processing RB's */
          if(tdsrrc_find_cn_domain_id_for_rb_in_est_rabs(local_rb_id, &local_cn_domain_id) == TRUE)
          {
            if(tdsrrcsmc_int_ciph_info.ciph_status[local_cn_domain_id] == TDSRRCSMC_SECURITY_STARTED)
             {
               local_hfn_per_dl_rb[local_rb_id]=local_hfn_type.hfn_per_rb_dl[i].hfn;
             }
             else
             {
               local_hfn_per_dl_rb[local_rb_id]=0;
             }
          }
          else
          {
            local_hfn_per_dl_rb[local_rb_id]=0;
          }
          if(tdsrrclcm_get_dl_rlc_mode(local_lc_id, &local_mode)==TDSRRCLCM_SUCCESS)
          {
            if(local_mode==UE_MODE_UNACKNOWLEDGED)
            {
              local_hfn_per_dl_rb[local_rb_id]=local_hfn_per_dl_rb[local_rb_id]>>TDSRRC_RLC_UM_START_OFFSET;
            }
          }
        }
      }
    }
  }

  /*store the hfn values for each ul UM/AM RBs*/
  for(i=0;i<local_hfn_type.ul_nchan;i++)
  {
    local_lc_id=local_hfn_type.hfn_per_rb_ul[i].rlc_id;
    local_rb_id=tdsrrclcm_get_ul_rb_id(local_lc_id);
    
    /* Processing SRB's first */
    switch(local_rb_id)
    {
     case CCCH_RADIO_BEARER_ID:
         local_hfn_per_dl_rb[local_rb_id]=0;
         break;
      case DCCH_AM_RADIO_BEARER_ID:
      case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
      case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
      case DCCH_UM_RADIO_BEARER_ID:
        local_hfn_per_ul_rb[local_rb_id]=local_hfn_type.hfn_per_rb_ul[i].hfn;
        break;
      default:
      {
        if(local_rb_id < TDS_MAX_RB)
        {
          /* Now processing RB's */
          if(tdsrrc_find_cn_domain_id_for_rb_in_est_rabs((rb_id_type)local_rb_id, &local_cn_domain_id) == TRUE)
          {
            if(tdsrrcsmc_int_ciph_info.ciph_status[local_cn_domain_id] == TDSRRCSMC_SECURITY_STARTED)
            {
              local_hfn_per_ul_rb[local_rb_id]=local_hfn_type.hfn_per_rb_ul[i].hfn;
            }
            else
            {
              local_hfn_per_ul_rb[local_rb_id]=0;
            }
          }
          else
          {
            local_hfn_per_ul_rb[local_rb_id]=0;
          }
          if(tdsrrclcm_get_ul_rlc_mode(local_lc_id, &local_mode)==TDSRRCLCM_SUCCESS)
          {
            if(local_mode==UE_MODE_UNACKNOWLEDGED)
            {
            local_hfn_per_ul_rb[local_rb_id]=local_hfn_per_ul_rb[local_rb_id]>>TDSRRC_RLC_UM_START_OFFSET;
            }
          }
        }
      }
    }
  }

  /*Get the hfn values for TM RBs*/

  (void) tdsmac_rrc_get_hfn_info(&mac_hfn);

  mac_hfn.cs_hfn=mac_hfn.cs_hfn>>TDSRRC_RLC_TM_START_OFFSET;
  mac_hfn.ps_hfn=mac_hfn.ps_hfn>>TDSRRC_RLC_TM_START_OFFSET;

  TDSRRC_MSG2(MSG_LEGACY_MED,"MAC HFN CS : %x ,MAC HFN PS : %x",mac_hfn.cs_hfn,mac_hfn.ps_hfn);


  if(proc_id == TDSRRC_PROCEDURE_RBE || proc_id == TDSRRC_PROCEDURE_RBR || proc_id == TDSRRC_PROCEDURE_CU)
  {
    /*Find the maximum HFN for UL TM RBs to be released*/
    for(i=0;i<rlc_req_type_ptr->ul_tm.nchan;i++)
    {
      local_rb_id = tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_tm.chan_info[i].lc_id);
      /* Processing SRB's first skipping SRB0*/
      switch(local_rb_id)
      {
        case CCCH_RADIO_BEARER_ID:
          break;
        default:
        {
          if((local_rb_id < TDS_MAX_RB)&&(rlc_req_type_ptr->ul_tm.ul_tm_act[i].action==TDSRLC_RELEASE) &&
            (
              rlc_req_type_ptr->ul_tm.chan_info[i].lc_type == UE_LOGCHAN_DCCH ||
              rlc_req_type_ptr->ul_tm.chan_info[i].lc_type == UE_LOGCHAN_DTCH 
            ))
          {
            if((domain_id_list[local_rb_id]==RRC_CS_DOMAIN_CN_ID) &&
               (tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
            {
              max_count_c_cs=(max_count_c_cs<mac_hfn.cs_hfn)?mac_hfn.cs_hfn:max_count_c_cs;
            }
            else if((domain_id_list[local_rb_id]==RRC_PS_DOMAIN_CN_ID) &&
                    (tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
            {
              max_count_c_ps=(max_count_c_ps<mac_hfn.ps_hfn)?mac_hfn.ps_hfn:max_count_c_ps;
            }
          }
        }
      }
    }
    /*Find the maximum HFN for DL TM RBs to be released*/
    for(i=0;i<rlc_req_type_ptr->dl_tm.nchan;i++)
    {
      local_rb_id = tdsrrclcm_get_dl_rb_id(rlc_req_type_ptr->dl_tm.chan_info[i].lc_id);
      /* Processing SRB's first skipping SRB0*/
      switch(local_rb_id)
      {
        case CCCH_RADIO_BEARER_ID:
        default:
        {
          if((local_rb_id < TDS_MAX_RB) && (rlc_req_type_ptr->dl_tm.dl_tm_act[i].action==TDSRLC_RELEASE) &&
          (
              rlc_req_type_ptr->dl_tm.chan_info[i].lc_type == UE_LOGCHAN_DCCH ||
              rlc_req_type_ptr->dl_tm.chan_info[i].lc_type == UE_LOGCHAN_DTCH 
            #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
               ||rlc_req_type_ptr->dl_tm.chan_info[i].lc_type == UE_LOGCHAN_TM_DCCH_DL
            #endif
          ))
          {
            if((domain_id_list[local_rb_id]==RRC_CS_DOMAIN_CN_ID) &&
                 (tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
            {
              max_count_c_cs=(max_count_c_cs<mac_hfn.cs_hfn)?mac_hfn.cs_hfn:max_count_c_cs;
            }
            else if((domain_id_list[local_rb_id]==RRC_PS_DOMAIN_CN_ID) &&
                      (tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
            {
              max_count_c_ps=(max_count_c_ps<mac_hfn.ps_hfn)?mac_hfn.ps_hfn:max_count_c_ps;
            }
          }
        }
      }
    }
    /*Find the maximum HFN for UL UM RBs to be released*/
    for(i=0;i<rlc_req_type_ptr->ul_um.nchan;i++)
    {
      local_rb_id=tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_um.chan_info[i].lc_id);
      if((local_rb_id < TDS_MAX_RB) && (rlc_req_type_ptr->ul_um.ul_um_act[i].action==TDSRLC_RELEASE))
      {
        if((domain_id_list[local_rb_id]==RRC_CS_DOMAIN_CN_ID) &&
           (tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
        {
          max_count_c_cs=(max_count_c_cs<local_hfn_per_ul_rb[local_rb_id])?local_hfn_per_ul_rb[local_rb_id]:max_count_c_cs;
        }
        else if((domain_id_list[local_rb_id]==RRC_PS_DOMAIN_CN_ID) &&
                (tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
        {
          max_count_c_ps=(max_count_c_ps<local_hfn_per_ul_rb[local_rb_id])?local_hfn_per_ul_rb[local_rb_id]:max_count_c_ps;
        }
      }
    }
    /*Find the maximum HFN for DL UM RBs to be released*/
    for(i=0;i<rlc_req_type_ptr->dl_um.nchan;i++)
    {
      local_rb_id=tdsrrclcm_get_dl_rb_id(rlc_req_type_ptr->dl_um.chan_info[i].lc_id);
      if((local_rb_id < TDS_MAX_RB) && (rlc_req_type_ptr->dl_um.dl_um_act[i].action==TDSRLC_RELEASE))
      {
        if((domain_id_list[local_rb_id]==RRC_CS_DOMAIN_CN_ID) &&
           (tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
        {
          max_count_c_cs=(max_count_c_cs<local_hfn_per_dl_rb[local_rb_id])?local_hfn_per_dl_rb[local_rb_id]:max_count_c_cs;
        }
        else if((domain_id_list[local_rb_id]==RRC_PS_DOMAIN_CN_ID) &&
                (tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] == TDSRRCSMC_SECURITY_STARTED))
        {
          max_count_c_ps=(max_count_c_ps<local_hfn_per_dl_rb[local_rb_id])?local_hfn_per_dl_rb[local_rb_id]:max_count_c_ps;
        }
      }
    }
    /*Find the maximum HFN for AM RBs to be released*/
    for(i=0;i<rlc_req_type_ptr->am.nentity;i++)
    {
      if(rlc_req_type_ptr->am.am_act[i].action==TDSRLC_RELEASE)
      {
        local_rb_id=rlc_req_type_ptr->am.am_info[i]->common_config.rb_id;
        if(local_rb_id < TDS_MAX_RB)
        {
          if(domain_id_list[local_rb_id]==RRC_CS_DOMAIN_CN_ID)
          {
          /*Get the maximum hfn for DL AM for CS domain*/
            max_count_c_cs=(max_count_c_cs<local_hfn_per_dl_rb[local_rb_id])?local_hfn_per_dl_rb[local_rb_id]:max_count_c_cs;

          /*Get the maximum hfn for UL AM for CS domain*/
            max_count_c_cs=(max_count_c_cs<local_hfn_per_ul_rb[local_rb_id])?local_hfn_per_ul_rb[local_rb_id]:max_count_c_cs;
          }
          else
          {
          /*Get the maximum hfn for DL AM for PS domain*/
            max_count_c_ps=(max_count_c_ps<local_hfn_per_dl_rb[local_rb_id])?local_hfn_per_dl_rb[local_rb_id]:max_count_c_ps;

          /*Get the maximum hfn for UL AM for PS domain*/
          max_count_c_ps=(max_count_c_ps<local_hfn_per_ul_rb[local_rb_id])?local_hfn_per_ul_rb[local_rb_id]:max_count_c_ps;
          }
        }
      }
    }
  }

  /*Now store the max count c for released rbs in the temporary variable*/
  tdsrrcsmc_max_count_c.count_c_cs=(tdsrrcsmc_max_count_c.count_c_cs>max_count_c_cs)?
                          tdsrrcsmc_max_count_c.count_c_cs:max_count_c_cs;
  tdsrrcsmc_max_count_c.count_c_ps=(tdsrrcsmc_max_count_c.count_c_ps>max_count_c_ps)?
                          tdsrrcsmc_max_count_c.count_c_ps:max_count_c_ps;

  TDSRRC_MSG2(MSG_LEGACY_MED,"Max count c for released RB for cs domain = %x ps domain = %x ",tdsrrcsmc_max_count_c.count_c_cs,tdsrrcsmc_max_count_c.count_c_ps);
  
}

/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_config_info_for_reconfig

DESCRIPTION
  This function is called by LLC procedure before it sends ciphering
  information to RLC, MAC and L1 layers.  When this function is called,
  SMC procedure updates the global DB for ciphering configuration with
  the latest ciphering information. 

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS: if UM or AM RB's are added
  FAILURE: if UM or AM RB's are not added

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
uecomdef_status_e_type tdsrrcsmc_update_ciphering_config_info_for_reconfig
(
tdsrrcllc_rlc_config_req_list_type *rlc_req_type_ptr,
tdsrrc_proc_e_type proc_id
)
{
if(tdsnv_ciphering_enabled)
{
  tdsrrcrb_rab_info_per_msg_type *local_est_rab_ptr; /*store rab info*/

  /*local array to store domain id's for all rb's*/
  rrc_cn_domain_identity_e_type  local_domain_id_list[TDS_MAX_RB];

  /*local var to store domain id in for loop*/
  rrc_cn_domain_identity_e_type  local_domain_id;

  /*local vars to hold count and other misc info*/
  uint32 count,i, local_rb_id;

  /*status to indicate if am or um rb's are reconfigured*/
  uecomdef_status_e_type am_um_reconfig_status;

  /*This Variable will be set to TRUE when SRB-4 is established & 
    LATEST CONFIGURED CN DOMAIN is initialized */
  boolean is_srb4_est_and_security_started = FALSE;

  am_um_reconfig_status = FAILURE;

  /**************
    updation of ciphering configuration can be done only be RBE is new RB's are
    added or by RBR if RB's are deleted.  In future when SRNS relocation is
    supported, we will revisit this area
   **************/

  /*ciphering info for rlc is incremental.  so set UM and AM info
    to zero and only initialize the new radio bearers*/
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb = 0;
  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb = 0;

  if(proc_id == TDSRRC_PROCEDURE_RBE || proc_id == TDSRRC_PROCEDURE_RBR || proc_id == TDSRRC_PROCEDURE_CU)
  {
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"N_RB %d, S_V_T %d, N_TM %d, CS_TM_AFF %d",
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].num_rb, 
              TDSSTART_VALUE_TO_TRANSMIT_FLAG,
              TDSNEW_TM_RB_IND_FLAG,
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].affected);                           
    
  }
  /*First act on RB release info & then act on RB est. */ 
  if (proc_id == TDSRRC_PROCEDURE_RBR || proc_id == TDSRRC_PROCEDURE_CU || proc_id == TDSRRC_PROCEDURE_RBE)
  {
    //reset the flags that checks to see if all SIB's are received  
    memset (local_domain_id_list, (int)RRC_CS_DOMAIN_CN_ID, TDS_MAX_RB * sizeof(rrc_cn_domain_identity_e_type));

    /*get the domain id of all the newly established radio bearers*/
    for (count = 0; count < MAX_RAB_TO_SETUP; count++)
    {
      if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
      {
        for (i=0; i<tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
        {
          if(tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            local_domain_id_list[tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id] =
            RRC_CS_DOMAIN_CN_ID;
          }
          else
          {
            local_domain_id_list[tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id] =
            RRC_PS_DOMAIN_CN_ID;
          }
        }
      }
    }
    /*Calculate and store the tdsrrcsmc_max_count_c for released RBs */
    tdsrrcsmc_calc_max_count_c_released_rb(rlc_req_type_ptr,local_domain_id_list,proc_id);
    
    /* Update struct for tm radio bearer id */
    for (i=0; i < rlc_req_type_ptr->ul_tm.nchan; i++)
    {
      local_rb_id = tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_tm.chan_info[i].lc_id);

      if(local_rb_id < TDS_MAX_RB)
      {
        local_domain_id = local_domain_id_list[local_rb_id];

        /* Update the TM RB info only if they exist.There is a possibility that these RBs
           could have been removed before the RadioBearerRelease during a 
           SignallingConnectionReleaseIndication */
        
            if((local_domain_id != RRC_CS_DOMAIN_CN_ID) && 
               (local_domain_id != RRC_PS_DOMAIN_CN_ID))
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid Domain ID %d to delete RB_ID %d",local_domain_id,local_rb_id);
            }
				
	else if(tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb != 0)
        {
          //rb_id 0 is TM as well unciphered.. we don't do ciphering/deciphering on RB0 which is
          //a common channel, so in case RB0 is being released, don't change ciphering database
          if(local_rb_id != 0)
            {
              if(((proc_id == TDSRRC_PROCEDURE_CU)||
                  (proc_id == TDSRRC_PROCEDURE_RBR)||
                  (proc_id == TDSRRC_PROCEDURE_RBE))&& 
                  (TDSRLC_RELEASE == rlc_req_type_ptr->ul_tm.ul_tm_act[i].action)
                )
              {
                tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb--;
  
                if (tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb == 0)
                {
                  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].affected = FALSE;
                  tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].rb_status = TDSCIPH_NOT_STARTED;
                  tdscipher_config_db.mac_cipher_info[local_domain_id].cipher_enable = FALSE;
                }
              }
            }
          }
        }
      }
    }

  /*check if this update is due to Radio Bearer Establishment*/
  if (proc_id == TDSRRC_PROCEDURE_RBE) 
  {
    /*get the established rabs due to RBE from RRCRBE procedure*/
    local_est_rab_ptr = tdsrrcrbe_get_est_rab_ptr();

    /*get the domain id of all the newly established radio bearers*/
    for (count = 0; count < local_est_rab_ptr->num_rabs_in_msg; count++)
    {
      if (local_est_rab_ptr->rabs_in_msg[count].rab_id != TDSRRC_INVALID_RAB_ID)
      {
        for (i=0; i<local_est_rab_ptr->rabs_in_msg[count].num_rbs_for_rab; i++)
        {
          
          if(local_est_rab_ptr->rabs_in_msg[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            local_domain_id_list[local_est_rab_ptr->rabs_in_msg[count].rb_for_rab[i].rb_id] = 
            RRC_CS_DOMAIN_CN_ID;
          }
          else
          {
            local_domain_id_list[local_est_rab_ptr->rabs_in_msg[count].rb_for_rab[i].rb_id] = 
            RRC_PS_DOMAIN_CN_ID;
          }
        }/* for loop for num of rb id for each rab*/
      }/*if not INVALID_RAB_ID*/
    }/*for */


    /*update struct for am radio bearer id*/
    for (i=0; ((i < rlc_req_type_ptr->am.nentity) && (tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb < TDSUE_MAX_AM_ENTITY)); i++)
    {
      if(rlc_req_type_ptr->am.am_act[i].action != TDSRLC_ESTABLISH)
      {
        continue;
      }

      local_rb_id = rlc_req_type_ptr->am.am_info[i]->common_config.rb_id;

      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].rb_id =
      (uint8) rlc_req_type_ptr->am.am_info[i]->common_config.rb_id;

      /*check if the AM rb id is a SRB, and set SRB flag to TRUE*/
      if ((rlc_req_type_ptr->am.am_info[i]->common_config.rb_id == 
           DCCH_AM_RADIO_BEARER_ID) ||
          (rlc_req_type_ptr->am.am_info[i]->common_config.rb_id == 
           DCCH_DT_HIGH_PRI_RADIO_BEARER_ID) ||
          (rlc_req_type_ptr->am.am_info[i]->common_config.rb_id ==  
           DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
         )
      {
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
        is_rb_srb=TRUE;
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
          domain_id = tdsrrcsmc_int_ciph_info.latest_config_cn_id;
      }
      /*if not a SRB, set is_rb_srb flag to false and get the domain id for the RB ID*/
      else
      {
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
        is_rb_srb = FALSE;
        /*lint -e644*/
        tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
        domain_id = local_domain_id_list[local_rb_id];
        /*lint +e644*/
      }
      /*If Signalling RB is getting set up, Set TDSSTART_VALUE_TO_TRANSMIT_FLAG 
        only if the variable LATEST_CONFIGURED_CN_DOMAIN has been initialised.
        LATEST_CONFIGURED_CN_DOMAIN will be initialised when either integrity 
        and/or ciphering is started. So check security status here*/
      if((tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
          is_rb_srb == TRUE)&&
         (tdsrrcsmc_get_security_start_status() == TRUE)
        )
      {
        /*this flag is queried by the reconfig procedure and indicates that new 
          START values need to be transmitted as AM/UM radio bearers are setup*/
        TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
        tdsstart_value_to_transmit_domain =  tdsrrcsmc_int_ciph_info.latest_config_cn_id;
        /*Variable is_srb4_est_and_security_started is used to keep track 
          Of below section of the spec.
          8.2.2.2 Initiation
          - if signalling radio bearer RB4 is setup with this procedure and signalling radio bearers 
             RB1-RB3 were already established prior to the procedure:
          - if the variable "LATEST_CONFIGURED_CN_DOMAIN" has been initialised:
             connect any radio bearers setup by the same message as signalling radio bearer RB4
             to the CN domain indicated in the variable "LATEST CONFIGURED CN DOMAIN".*/
        if(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].rb_id
           == DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          is_srb4_est_and_security_started = TRUE;
        }
      }
      
      /*If SRB-4 is getting set up through this RBsetup msg & LATEST CONFIGURED CN DOMAIN
        Is initialized then N/W should take care of associating CN domain of any RB getting set 
        up with this message with  LATEST CONFIGURED CN DOMAIN as per 8.2.2.2. 
        If so skip setting tdsstart_value_to_transmit_domain as its taken care in the handling 
        for SRB-4 above */
      if((tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
          is_rb_srb != TRUE)&&
          (is_srb4_est_and_security_started == FALSE))
      {
        /*this flag is queried by the reconfig procedure and indicates that new 
          START values need to be transmitted as AM/UM radio bearers are setup*/
        TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
        tdsstart_value_to_transmit_domain = RRC_PS_DOMAIN_CN_ID;
      }

      /*initialize ciph first time and increment the number of rb's counter*/
      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.
      num_rb].rb_status = TDSCIPH_FIRST_TIME;
      tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb++;
      /*return STATUS is set to SUCCESS to indicate to LLC that we have to send
        RLC UL/DL cipher config requests*/
      am_um_reconfig_status = SUCCESS;
      if(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb == UE_MAX_AM_ENTITY)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reached the max value for num_rb");
        break;
      }
    }

    /*update struct for um radio bearer id*/
    for (i=0; ((i< rlc_req_type_ptr->ul_um.nchan) && (tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb < TDSUE_MAX_UL_UM_CHANNEL)); i++)
    {
      if(rlc_req_type_ptr->ul_um.ul_um_act[i].action != TDSRLC_ESTABLISH)
      {
        continue;
      }

      local_rb_id = tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_um.chan_info[i].lc_id);

      tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].rb_id = 
      (uint8) tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_um.chan_info[i].lc_id);

      /*check if SRB or not and set the flag appropriately*/
      if (tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
          rb_id == DCCH_UM_RADIO_BEARER_ID )
      {
        tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
        is_rb_srb=TRUE;
        /*this will only be changing config. of SRB's.  Support for checking RLC size if
          not currently present.  Continue without changing SRB's ciphering information*/
        continue;
      }
      else
      {
        tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
        is_rb_srb=FALSE;
        if(local_rb_id < TDS_MAX_RB)
        {
          /*lint -e644*/
          tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
          domain_id = local_domain_id_list[local_rb_id];
          /*lint +e644*/
        }
      }
      /*If Signalling RB is getting set up, Set TDSSTART_VALUE_TO_TRANSMIT_FLAG 
        only if the variable LATEST_CONFIGURED_CN_DOMAIN has been initialised.
        LATEST_CONFIGURED_CN_DOMAIN will be initialised when either integrity 
        and/or ciphering is started. So check security status here*/
      if((tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
          is_rb_srb == TRUE)&&
         (tdsrrcsmc_get_security_start_status() == TRUE)
        )
      {
        /*this flag is queried by the reconfig procedure and indicates that new 
          START values need to be transmitted as AM/UM radio bearers are setup*/
        TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
        tdsstart_value_to_transmit_domain =  tdsrrcsmc_int_ciph_info.latest_config_cn_id;
      }
      
      /*If SRB-4 is getting set up through this RBsetup msg & LATEST CONFIGURED CN DOMAIN
        Is initialized then N/W should take care of associating CN domain of any RB getting set 
        up with this message with  LATEST CONFIGURED CN DOMAIN as per 8.2.2.2. 
        If so skip setting tdsstart_value_to_transmit_domain as its taken care in the handling 
        for SRB-4 above */
      if((tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
          is_rb_srb != TRUE)&&
          (is_srb4_est_and_security_started == FALSE))
      {
        /*this flag is queried by the reconfig procedure and indicates that new 
          START values need to be transmitted as AM/UM radio bearers are setup*/
        TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
        tdsstart_value_to_transmit_domain = RRC_PS_DOMAIN_CN_ID;
      }

      /*initialize ciph first time and increment the number of rb's counter*/
      tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
      rb_status = TDSCIPH_FIRST_TIME;
      tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb++;

      /*return STATUS is set to SUCCESS to indicate to LLC that we have to send
        RLC UL/DL cipher config requests*/
      am_um_reconfig_status = SUCCESS;
    }

    /*update struct for tm radio bearer id*/
    for (i=0; i< rlc_req_type_ptr->ul_tm.nchan; i++)
    {
      if(rlc_req_type_ptr->ul_tm.ul_tm_act[i].action != TDSRLC_ESTABLISH)
      {
        continue;
      }

      local_rb_id=tdsrrclcm_get_ul_rb_id(rlc_req_type_ptr->ul_tm.chan_info[i].lc_id);

      if(local_rb_id < TDS_MAX_RB)
      {
        local_domain_id = local_domain_id_list[local_rb_id];
  
        
        if(tdsrrcsmc_int_ciph_info.ciph_status[local_domain_id] == TDSRRCSMC_SECURITY_STARTED)
        {
          /*if TM rb's are being configured for the first time, set affected flag to
            TRUE*/
          if (tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].affected != TRUE)
          {
            tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].affected = TRUE;
            tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].rb_status = TDSCIPH_FIRST_TIME;
          }
  
          /*increment the number of rb's counter*/
          tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb++;
  
          /*if this is the first RB for this CN domain, then set NEW flag to TRUE*/
  
          /*If SRB-4 is getting set up through this RBsetup msg & LATEST CONFIGURED CN DOMAIN
           Is initialized then N/W should take care of associating CN domain of any RB getting set 
           up with this message with  LATEST CONFIGURED CN DOMAIN as per 8.2.2.2. 
           If so skip setting tdsstart_value_to_transmit_domain as its taken care in the handling 
           for SRB-4 above */
          if ((tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb == 1)&&
              (is_srb4_est_and_security_started == FALSE))
          {
            /*If a new TM Radio Bearer is being added, then set this flag for the
              reconfig procedure to query TM RB ciphering act CFN.
             */
            TDSNEW_TM_RB_IND_FLAG = TRUE;      
    
            /*this flag is queried by the reconfig procedure and indicates that new 
              START values need to be transmitted as AM/UM radio bearers are setup*/
            TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
            tdsstart_value_to_transmit_domain = RRC_CS_DOMAIN_CN_ID;
          }
        }
        else
        {
          /*If prior to this procedure there exists no transparent mode radio bearer for the 
            CN domain included in the IE "CN domain identity" and at least one transparent
            mode radio bearer is included in the IE "RB information to setup" then,
            set TDSSTART_VALUE_TO_TRANSMIT_FLAG */

          /*If SRB-4 is getting set up through this RBsetup msg & LATEST CONFIGURED CN DOMAIN
           Is initialized then N/W should take care of associating CN domain of any RB getting set 
           up with this message with  LATEST CONFIGURED CN DOMAIN as per 8.2.2.2. 
           If so skip setting tdsstart_value_to_transmit_domain as its taken care in the handling 
           for SRB-4 above */
          if((tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb == 0)&&
             (is_srb4_est_and_security_started == FALSE))
          {
            /*this flag is queried by the reconfig procedure and indicates that new 
              START values need to be transmitted as first time TM radio bearers are setup*/
            TDSSTART_VALUE_TO_TRANSMIT_FLAG = TRUE;
            tdsstart_value_to_transmit_domain = RRC_CS_DOMAIN_CN_ID;
          }
          tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb++;
        }
      }
    }

    /*update the global ciphering database which will be used by LLC to configure
      RLC/MAC/L1*/
    tdsrrcsmc_update_global_ciph_db_for_reconfig();

  }
  

  if(proc_id == TDSRRC_PROCEDURE_RBE || proc_id == TDSRRC_PROCEDURE_RBR || proc_id == TDSRRC_PROCEDURE_CU)
  {
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"N_RB %d, S_V_T %d, N_TM %d, CS_TM_AFF %d",
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].num_rb, 
              TDSSTART_VALUE_TO_TRANSMIT_FLAG,
              TDSNEW_TM_RB_IND_FLAG,
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].affected);                 
    
  }

  return am_um_reconfig_status;
}
else
{
  return FAILURE;  /*if NV item is not turned on return failure,
                     LLC will not configure RLC with new ciph config*/
}
}/*tdsrrcsmc_update_ciphering_config_info_for_reconfig*/
/*lint +e715*/
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_config_info

DESCRIPTION
  Update ciphering config info for START of ciphering.  This function
  is called by RRCSMC procedure.  This is called to update ciphering
  parameters when ciphering is being first started for a particular
  CN domain.
  
DEPENDENCIES
  None

RETURN VALUE
  SUCCESS or FAILURE

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_update_ciphering_config_info
(
  tdsrrc_ciph_config_e_type config_cause
)
{
if(tdsnv_ciphering_enabled)
{

  /*local vars to store count and other misc local values*/
  uint32 count=0,i=0;

  /*local var to store cn domain id*/
  rrc_cn_domain_identity_e_type  local_domain_id, local_latest_config_cn_domain;

  /*local var to store logical channel ID and radio bearer ID*/
  uint32                 local_lc_id, local_rb_id;


  /***********BEGINNING OF CIPH CONFIGURATION FOR SRB's************/

  /*ciphering info for rlc is incremental.  so set UM and AM info
    to zero and only initialize the new radio bearers*/
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb = 0;
  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb = 0;

  local_latest_config_cn_domain = tdsrrcsmc_get_latest_configured_cn_domain();
  
  /*hardcode ciphering configuration for RB 2*/
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].is_rb_srb = TRUE;
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].domain_id = local_latest_config_cn_domain;

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_id = 
  DCCH_AM_RADIO_BEARER_ID; 

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_status = 
  TDSCIPH_FIRST_TIME;

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb++;
  count = tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb;

  /*hardcode ciphering configuration for RB 3*/
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].is_rb_srb = TRUE;
  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].domain_id = 
                                                            local_latest_config_cn_domain;

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_id = 
  DCCH_DT_HIGH_PRI_RADIO_BEARER_ID; 

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_status = 
  TDSCIPH_FIRST_TIME;

  tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb++;
  count = tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb;

  /*check if RB4 is setup or not and then config ciphering config for RB4*/
  if (TDSRRCLCM_RLC_LC_ID_NOT_FOUND != 
      tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                UE_MODE_ACKNOWLEDGED_DATA))
  {

    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].is_rb_srb = TRUE;
    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].domain_id = 
                                                    local_latest_config_cn_domain;

    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_id = 
    DCCH_DT_LOW_PRI_RADIO_BEARER_ID; 

    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[count].rb_status = 
    TDSCIPH_FIRST_TIME;

    tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb++;
    count = tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb;
  }

  /*count is set to 0 as RB 1 is UM radiobearer, different array is
    used to store UM RB's*/
  count=0;

  /*hardcode ciphering configuration for RB 1*/
  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[count].is_rb_srb = TRUE;
  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[count].domain_id = 
                                                     local_latest_config_cn_domain;

  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[count].rb_id = 
  DCCH_UM_RADIO_BEARER_ID; 

  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[count].rb_status = 
  TDSCIPH_FIRST_TIME;

  tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb++;

  /***********END OF CIPH CONFIGURATION FOR SRB's************/

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"N_RB %d, S_V_T %d, N_TM %d",
            tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].num_rb, 
            TDSSTART_VALUE_TO_TRANSMIT_FLAG,
            TDSNEW_TM_RB_IND_FLAG);                       
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_TM_AFF %d",tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].affected);

  /***********BEGINNING OF CIPH CONFIGURATION FOR UM/AM/TM RB's************/

  /*loop through established_rabs struct and configure ciphering info. for
    non-SRB Radio Bearers*/
  for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    {
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if (tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_status ==
            TDSRRC_RB_STARTED)
        {

          /*copy to local vars to avoid multiple indirections in each loop and
                generally lengthy variable names and confusion */
          local_rb_id = tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id;
          local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[count].
                                                rb_for_rab[i].rb_id);
          if(tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            local_domain_id = RRC_CS_DOMAIN_CN_ID;
          }
          else
          {
            local_domain_id = RRC_PS_DOMAIN_CN_ID;
          }

          /*initialize value of TM radio bearers.  set affected status to TRUE
            cn domain and increment num_rb*/
          if (local_lc_id < TDSRRCLCM_UL_UM_START_IDX)
          {
            if(local_domain_id ==  local_latest_config_cn_domain)
            {

              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].affected = TRUE;
  
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].rb_status =
              TDSCIPH_FIRST_TIME;
              if((CIPH_CONFIG_G2TD == config_cause) || (TDSCIPH_CONFIG_CS == config_cause) || 
                (TDSCIPH_CONFIG_CS_PLUS_PS == config_cause))
              {
                 tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].num_rb++;
              }
            }
            else
            {
              tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[local_domain_id].affected = FALSE;
            }
          }
          /*check if lc_id of the Radio Bearer belongs to AM Mode*/
          else if (local_lc_id >= TDSRRCLCM_UL_AM_DATA_START_IDX &&
                   local_lc_id < TDSRRCLCM_UL_AM_DATA_START_IDX+TDSUE_MAX_UL_AM_DATA_CHANNEL)
          {
            /*initialize value of AM radio bearers.  set SRB true to false.  set rb id
              cn domain and increment num_rb*/
            if(tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb < TDSUE_MAX_AM_ENTITY)
            {
              tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
              is_rb_srb = FALSE;

              tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
              rb_id = (uint8) local_rb_id;

              tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
              rb_status = TDSCIPH_FIRST_TIME;      

              
            if(tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
              {
                tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
                        domain_id = RRC_CS_DOMAIN_CN_ID;
              }
              else
              {
                tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb].
                        domain_id =  RRC_PS_DOMAIN_CN_ID;
              }  
              tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb++;          
            }
            else
            {
              ERR_FATAL("AM num_rb is out or range: %d",tdsrrcsmc_ciph_rb_info.am_rb_ciph_config.num_rb,0,0);
            }
          }
          /*check if lc_id of the Radio Bearer belongs to UM Mode*/
          else if (local_lc_id >= TDSRRCLCM_UL_UM_START_IDX &&
                   local_lc_id < TDSRRCLCM_UL_AM_DATA_START_IDX)
          {

            /*initialize value of UM radio bearers.  set SRB true to false.  set rb id
              cn domain and increment num_rb*/
            if(tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb < TDSUE_MAX_UL_UM_CHANNEL)
            {
              tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
              is_rb_srb = FALSE;

              tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
              rb_id = (uint8) local_rb_id;

              tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
              rb_status= TDSCIPH_FIRST_TIME;

            if(tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)
              {
                tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
                  domain_id = RRC_CS_DOMAIN_CN_ID;
              }
              else
              {
                tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.config_info[tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb].
                  domain_id =  RRC_PS_DOMAIN_CN_ID;
              }  
              tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb++;          
            }
            else
            {
              ERR_FATAL("UM num_rb is out or range: %d",tdsrrcsmc_ciph_rb_info.um_rb_ciph_config.num_rb,0,0);
            }
          }/*else if check for UM RB's*/
        }/*if check for RB STARTED*/
      }/*for loop for number of RB's in RAB*/
    }/*check for INVALID_RAB_ID*/
  }/*for loop for MAX_RAB_TO_SETUP*/

  /***********END OF CIPH CONFIGURATION FOR UM/AM/TM RB's************/

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"N_RB %d, S_V_T %d, N_TM %d",
            tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].num_rb, 
            TDSSTART_VALUE_TO_TRANSMIT_FLAG,
            TDSNEW_TM_RB_IND_FLAG);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_TM_AFF %d",tdsrrcsmc_ciph_rb_info.tm_rb_ciph_config[0].affected);

  /*update the global ciphering database which will be used by LLC to configure
    RLC/MAC/L1*/
  tdsrrcsmc_update_global_ciph_db_for_ciph_start(local_latest_config_cn_domain
                                              ,config_cause
  );

  return SUCCESS;
}
else
{
  return SUCCESS;  /*to ward off warnings*/
}
}
/*===========================================================================

FUNCTION tdsrrcsmc_init_integrity_protection_info

DESCRIPTION
  Initialize the data structure tdsrrcsmc_integrity_protection_info with default
  values.  count_c and count_i values to default hfn's and 0's for tdsrrcllc_seq_num
  also initialize status and reconfig flags to false.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
static void tdsrrcsmc_init_integrity_protection_info
(
  sys_modem_as_id_e_type sub_id
)
{
  uint32 i=0, j=0;             /*local variables used for counting in for loop*/

  /*local var to store domain_id*/  
  rrc_cn_domain_identity_e_type domain_id=RRC_CS_DOMAIN_CN_ID; 
  
  uint32 init_seq_num=0;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type curr_sub_id = 0;
  if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
    return;
  }
  curr_sub_id = tdsrrc_get_as_id();
  tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms(curr_sub_id);
  tdsrrcsmc_set_smc_integrity_protection_to_global_params(sub_id);
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
#endif

  /*initializer the status, modify and reconfig flag to false*/
  tdsrrcsmc_integrity_protection_info.reconfig = FALSE;
  tdsrrcsmc_integrity_protection_info.ip_status = TDSRRCSMC_SECURITY_NOT_STARTED;        
  tdsrrcsmc_integrity_protection_info.modify_received = FALSE;

  /*cycle through both cn domains*/  
  for (j=0; j<RRC_MAX_CN_DOMAINS; j++)
  {
    if (j == 0)
    {
      domain_id = RRC_CS_DOMAIN_CN_ID;
    }
    else
    {
      domain_id = RRC_PS_DOMAIN_CN_ID;    
    }

    /*initialize count_i for cs and ps domain*/
    for ( i=0; i < TDSRRC_MAX_SRB; i++)
    {
      /*initialize uplink count_i with tdsrrcllc_seq_num = 0 and the default HFN received
        for this domain */
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[i] = 
      tdsrrcsmc_convert_START_to_count_c_i(
                                       tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,
                                       init_seq_num);

      /*initialize the downlink init_flag to false*/
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i_init_flag[i]=FALSE;

      /*initialize downlink count_i with tdsrrcllc_seq_num = 0 and the default HFN received
        for this domain */      
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].ul_count_i[i] =       
      tdsrrcsmc_convert_START_to_count_c_i(
                                       tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,
                                       init_seq_num);

      /*init the UL/DL act_time_pending_flag to false*/
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
      dl_count_i_act_time_pending[i] = FALSE;
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
      ul_count_i_act_time_pending[i] = FALSE;

      /*init the UL/DL act_time_pending_flag to false*/
      tdsrrcsmc_integrity_protection_info.dl_act_seq_num[i] = 0;
      tdsrrcsmc_integrity_protection_info.ul_act_seq_num[i] = 0;

      /*init the new_key_flag to false*/
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
      dl_new_key_flag_per_rb[i]=FALSE;
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
      ul_new_key_flag_per_rb[i]=FALSE;

      tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i] = FALSE;
    }/*for loop for number of SRB*/  
  }/*for loop for cn_domain_id*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms(sub_id);
  tdsrrcsmc_set_smc_integrity_protection_to_global_params(curr_sub_id);
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
#endif
}/*tdsrrcsmc_init_integrity_protection_info*/
/*===========================================================================

FUNCTION tdsrrcsmc_init_integrity_ciphering_config

DESCRIPTION
  This function initializes the rrcsmc_integrity_and_ciphering_info with 
  the values received from MM in the RRC_SIM_INSERT_REQ. Also a copy is made 
  in the rrcsmc_integrity_and_ciphering_info_old.  MISC procedure will call 
  this function in SMC.


DEPENDENCIES
  None.

RETURN VALUE
  Returns SUCCESS if Initialization of Integrity and Ciphering 
  Configuration is successful.
  
  Returns FAILURE if Initialization of Integrity and Ciphering 
  Configuration is not successful.


SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_init_integrity_ciphering_config
(
rrc_sim_inserted_req_type  *sim_insert_req_ptr
)
{
  uint32 i=0, count;           /*local variable use for counting in for loop*/
  uint32 key[4];               /*local var to print keys received*/
  rrc_cn_domain_identity_e_type domain_id; /*local var to keep track of cn_domain*/



  sys_modem_as_id_e_type sub_id,curr_sub_id;
  sub_id = sim_insert_req_ptr->as_id;
  if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
    return SUCCESS;
  }
  curr_sub_id = tdsrrc_get_as_id();
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);

  tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
#endif
  /*copy number of cn domains into int_ciph_info struct*/  
  tdsrrcsmc_int_ciph_info.num_cn_domains = RRC_MAX_CN_DOMAINS;

  /*copy the contents of cn_domain into int_ciph_info struct*/
  for (i=0; i<RRC_MAX_CN_DOMAINS; i++)
  {

    if (sim_insert_req_ptr->cipher_info[i].domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      /* Domain Index should always be 0 for CS Domain and 1 for 
         PS Domain when using the structure tdsrrcsmc_int_ciph_info */
      domain_id = RRC_CS_DOMAIN_CN_ID;
    }
    else
    {
      domain_id = RRC_PS_DOMAIN_CN_ID;    
    }

    /*copy cipher algo enum and domain_id*/    
    tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].cipher_algo = 
    sim_insert_req_ptr->cipher_info[i].cipher_algo;

    tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].domain_id = 
    sim_insert_req_ptr->cipher_info[i].domain_id;


    /*copy contents of ciphering_key to int_ciph_info struct
     *for this CN DOMAIN. Always copy to key location 0, then after
   *authentication use 1
     */  
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key[0], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte), 
           sim_insert_req_ptr->cipher_info[i].ciphering_key,
           RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );

    tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key_used=0;
    tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key=FALSE;
    tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key_for_start_value=FALSE;

    /*copy contents of integrity_key to int_ciph_info struct
     *for this CN DOMAIN
     */
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].integrity_key, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
           sim_insert_req_ptr->cipher_info[i].integrity_key,
           RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );

    for (count=0; count < (RRC_MAX_INTEGRITY_KEY_LENGTH /4); count++)
    {
      key[count] = b_unpackd( &(sim_insert_req_ptr->cipher_info[i].
                                integrity_key[count*4] ), 0, 32 );
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"smc_init_integrity: domain ID = %d:",domain_id);
    
    /* UIM will own a new EFS and expose this new API to TECHs, which will take SUB ID as input and return TRUE or FALSE 
     based on the SIM type and the new EFS setting. TECH teams need to log security related debug F3s 
     only when the UIM API returns TRUE. */
    if (mmgsdi_is_test_mode_logging_enabled_for_subscription(tdsrrc_get_as_id()))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"INT KEY 1: 0x%x 2: 0x%x",key[0],key[1]);
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"INT KEY 3: 0x%x 4: 0x%x",key[2],key[3]);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"cannot print INT key");
    }    

    /*copy contents of HFN to int_ciph_info struct
     *for this CN DOMAIN
     */
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
           sim_insert_req_ptr->cipher_info[i].hfn,
           RRC_MAX_HFN_LENGTH*sizeof(byte) );

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"EST START: 0x%x 0x%x 0x%x", sim_insert_req_ptr->cipher_info[i].hfn[0],
             sim_insert_req_ptr->cipher_info[i].hfn[1],
             sim_insert_req_ptr->cipher_info[i].hfn[2]);
  }

  /*initialize integrity protection info as we know the valid values of START
    and other information upon insertion of sim*/ 
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
#endif
  /*initialize integrity protection info as we know the valid values of START
    and other information upon insertion of sim*/ 
  tdsrrcsmc_init_integrity_protection_info(sub_id);
  return SUCCESS;
}/*tdsrrcsmc_init_integrity_ciphering_config*/

/*===========================================================================

FUNCTION tdsrrcsmc_ul_purge_wm

DESCRIPTION
   This function sends RLC_UL_PURGE_WM_REQ with the required information so that RLC purges the WM

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_ul_purge_wm(boolean srb4_purge)
{
  uint8 count=0;
  
  /* local cmd var to send to L2 */
  tdsl2_ul_cmd_type  *req_cmd_ptr;

  /* Get L2 UL command buffer */
  if ((req_cmd_ptr= tdsl2_ul_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending TDSRLC_UL_PURGE_WM_REQ ",0,0,0);
  }

  /*Set the command to RLC_UL_PURGE_WM_REQ*/
  req_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_UL_PURGE_WM_REQ;

  /*Add SRB1 Information to the LCs to be purged*/
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.lc_id[count] = tdsrrclcm_get_ul_rlc_lc_id(DCCH_UM_RADIO_BEARER_ID);
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.num_sdus[count++] = TDSMAX_SRB1_SDUS_TOBE_PURGED;

  /*Add SRB2 Information to the LCs to be purged*/
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.lc_id[count] = tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.num_sdus[count++] = TDSMAX_SRB2_SDUS_TOBE_PURGED;

  /*Add SRB3 Information to the LCs to be purged*/
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.lc_id[count] = tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.num_sdus[count++] = TDSMAX_SRB2_SDUS_TOBE_PURGED;

  if(srb4_purge == TRUE)
  {
  /*Add SRB4 Information to the LCs to be purged*/
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.lc_id[count] = tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
  req_cmd_ptr->cmd_data.ul_purge_wm_ind.num_sdus[count++] = TDSMAX_SRB2_SDUS_TOBE_PURGED;
  }

  req_cmd_ptr->cmd_data.ul_purge_wm_ind.num_lc_id = count;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRLC_UL_PURGE_WM_REQ to L2");

  tdsl2_ul_put_cmd(req_cmd_ptr);
  
}

/*===========================================================================

FUNCTION tdsrrcsmc_perform_stop_or_suspend_functionality

DESCRIPTION
   This function suspend's or stops all RLC UM and AM radio bearers by calling
   RLC function crlc_ul_perform_suspend_or_stop_functionality().process suspend 
   cnf for SMC & send SMC complete or process stop cnf for 
   SRNS relocation based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_perform_stop_or_suspend_functionality
(
  tdsrrcsmc_suspend_cmd_type suspend_cmd
)
{
  /* Local variables */
  uint32 i, count;  
  uint32 num_ch_to_suspend_ul = 0;
  uint32 num_ch_to_stop_dl = 0;
  uint32 local_lc_id;
  rlc_lc_id_type srb2_lc_id = 0xFF; //initialize to keep lint happy
  /*Structure to hold info abut suspend Request which will be filled 
      by rrcsmc & RLC will use it for updating its state variables*/
  tdsrlc_ul_stop_suspend_request_type rrcsmc_ul_req;

  tdsrlc_dl_stop_request_type rrcsmc_dl_stop_req;
  
  /*Structure to hold info abut suspend confirmation from RLC*/

  /*Move out of the function*/
  /*static  tdsrlc_ul_stop_suspend_cnf_type rrcsmc_ul_cnf ;*/
  
  boolean srb4_purge = FALSE;
  
  boolean status = FALSE;
  
  /* Suspend SRB1 */
  tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
    tdsrrclcm_get_ul_rlc_lc_id(DCCH_UM_RADIO_BEARER_ID);
  num_ch_to_suspend_ul++;

  /* Suspend SRB2 if this suspension is due to SMC */
  if(TDSRRCSMC_SMC_CMD == suspend_cmd)
  {
    srb2_lc_id = tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = srb2_lc_id;
    num_ch_to_suspend_ul++;
  }

  /* Suspend SRB3 */
  tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
    tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
  num_ch_to_suspend_ul++;

  if(suspend_cmd == TDSRRCSMC_SRNS_CMD)
  {
    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
      tdsrrclcm_get_dl_rlc_lc_id(DCCH_UM_RADIO_BEARER_ID);
    num_ch_to_stop_dl++;
  
    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
      tdsrrclcm_get_dl_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
    num_ch_to_stop_dl++;
  }

  /* Suspend SRB4 if established */
  if (TDSRRCLCM_RLC_LC_ID_NOT_FOUND !=
      tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                 DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                 UE_MODE_ACKNOWLEDGED_DATA))
  {
    srb4_purge = TRUE;
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
    num_ch_to_suspend_ul++;

    if(suspend_cmd == TDSRRCSMC_SRNS_CMD)
    {
      tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
        tdsrrclcm_get_dl_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
      num_ch_to_stop_dl++;
    }
  }

  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    {
      /* If the suspension is due to a Security Mode Command
         and the SMC was for CS domain, we do not have to suspend
         the PS RABs */
      
      if( (suspend_cmd == TDSRRCSMC_SMC_CMD) && 
          (tdsrrcsmc_saved_security_mode_command_msg.domain_id == RRC_CS_DOMAIN_CN_ID) &&
          (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID) )
      {
        // Do not suspend the radio bearers associated with this RAB
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not suspending PS RAB during CS SMC");
        break;
      }
        
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if(suspend_cmd==TDSRRCSMC_SRNS_CMD)
        {
          local_lc_id = tdsrrclcm_get_dl_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);
          if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
          {
            if ( local_lc_id >= TDSRRCLCM_DL_UM_START_IDX && 
                 local_lc_id < (TDSRRCLCM_DL_AM_DATA_START_IDX + TDSUE_MAX_DL_AM_DATA_CHANNEL))
            {
              tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl]= (uint8) local_lc_id;
              num_ch_to_stop_dl++;
            }
          }
        }

        local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);

        /*check if it is a AM or UM RLC RB only*/
        if ( local_lc_id >= TDSRRCLCM_UL_UM_START_IDX && 
             local_lc_id < (TDSRRCLCM_UL_AM_DATA_START_IDX + TDSUE_MAX_UL_AM_DATA_CHANNEL) )
        {
          tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = (uint8) local_lc_id;
          num_ch_to_suspend_ul++;
        }
      }
    }
  }

  //This code is to satify KW error only.
  if (num_ch_to_suspend_ul > (TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: num_ch_to_suspend_ul is %d,out of bound",num_ch_to_suspend_ul);
    num_ch_to_suspend_ul = TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY;
  }
  tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan= (uint8) num_ch_to_suspend_ul;

  //This code is to satify KW error only.
  if (num_ch_to_stop_dl > (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"num_ch_to_stop_dl is %d,out of bound",num_ch_to_stop_dl);
    num_ch_to_stop_dl = TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY;
  }
  tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan = (uint8) num_ch_to_stop_dl;

  /*this cmd initiates suspension of RLC radio bearers*/

  /* Fill in the command parameters for SUSPENDING RLC RB's */
  rrcsmc_ul_req.nchan = 
      tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;

  rrcsmc_dl_stop_req.nchan= 
      tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan;

  /* In a loop set the contents of the suspend_req */
  for (i = 0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
  {
    rrcsmc_ul_req.suspend_stop[i].lc_id = 
     tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id;

    /* For SRB2,the offset calculation is done based on the algorithm 
      *  1. Call RLC interface to get the following SRB2 values
      *     - Ciph pending status
      *     - VT_S
      *     - Act_SN if there is a pending config
      *     - WM size in terms of RLC PDUs
      *  2. If Ciph pending status is FALSE
      *       Set suspend_offset = 5
      *     Else
      *       If ((Act_SN - VT_S) >= WM RLC PDUs + TDSRLC_CIPH_ACT_SN_RB_2)
      *         Set suspend_offset = Act_SN - VT_S - WM RLC PDUs 
      *       Else
      *         Set suspend_offset = 5
    */ 
    if((rrcsmc_ul_req.suspend_stop[i].lc_id == srb2_lc_id) &&
       (TDSRRCSMC_SMC_CMD == suspend_cmd))
    {
      /* Local variables */
      uint8 suspend_offset;            
      uint32 vt_s = 0;
      uint32 act_sn = 0;
      uint32 wm_num_rlc_pdus = 0;
      uint32 act_pdu_buffer = 0;
      tdsrlc_cipher_cfg_pending_e_type pending_status;        

      /* Query RLC for the srb2 parameters */
      pending_status = tdsrlc_get_srb_ul_ciph_status
                       (
                           srb2_lc_id,
                           &vt_s,
                           &act_sn,
                           &wm_num_rlc_pdus
                         );
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SRB2 Ciph Status: pending_status %d",pending_status);  
      
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"SRB2 Ciph Status: vt_s %d, act_sn %d, wm_num_rlc_pdus %d",
                  vt_s,act_sn,wm_num_rlc_pdus);  
     
      /* Check the Pending status and act accordingly */
      if(pending_status != TDSRLC_CIPHER_CFG_PENDING)
      {
        suspend_offset = TDSRLC_CIPH_SRB_2_ACT_SN_OFFSET;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No cipher cfg pending. Setting srb2 suspend offset to %d",suspend_offset);
      }
      else // there is a pending config
      {
        /* Calculate the num pdus to activation time.Take care of rollovers */
        act_pdu_buffer = 
          (act_sn < vt_s) ? ((4096 - vt_s) + act_sn) : (act_sn - vt_s);

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Pending status TRUE. Time to Activation %d",act_pdu_buffer);

        /* Check if the WM PDUs and SMComplete can be accomadated in the old 
           config itself i.e. before the act SN is reached.If so,then specify 
           the same pending activation SN in this SMComplete also.Else fall back 
           to the old algo and give an offset of TDSRLC_CIPH_SRB_2_ACT_SN_OFFSET */
         
        if(tdsrrc_set_srb2_act_time_nv == FALSE)
        {
          if(act_pdu_buffer >= (wm_num_rlc_pdus + TDSRLC_NUM_PDUS_FOR_SMCOMPLETE))
          {
            status = TRUE;
          }
        }
        else
        {
          if(act_pdu_buffer > (wm_num_rlc_pdus + TDSRLC_NUM_PDUS_FOR_SMCOMPLETE))
          {
            status = TRUE;
          }
        }
        
       if(status == TRUE)
       {
          suspend_offset = (uint8)act_pdu_buffer;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"WM & SMComplete can be sent before Act_SN.Setting srb2 suspend_offset to %d",suspend_offset); 
        }
        else
        {
          suspend_offset = TDSRLC_CIPH_SRB_2_ACT_SN_OFFSET;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"WM & SMComplete cannot be sent before Act_SN.Setting srb2 suspend_offset to %d",suspend_offset); 
        }
      }

      /* Set the offset in the Suspend request */
      rrcsmc_ul_req.suspend_stop[i].n = suspend_offset;
    }
    /* For other SRBs, set the offset to 0 - immediate suspension */
    else 
    {
      rrcsmc_ul_req.suspend_stop[i].n = TDSRRCSMC_SUSPEND_UM_AM_0;
    } 
  }

  /*in a loop set the contents of the stop_req*/
  
  for (i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan; i++)
  {
    rrcsmc_dl_stop_req.lc_id[i] = 
    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[i];
  }
  

  if(suspend_cmd == TDSRRCSMC_SRNS_CMD)
  {
    /*Purge the UL WM*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Purge UL WM. SRB4 Purge Status : %d",srb4_purge);
    tdsrrcsmc_ul_purge_wm(srb4_purge);
  }
  
  if(suspend_cmd == TDSRRCSMC_SMC_CMD)
  {
    /*process suspend req for SMC*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Call RLC call back fun for processing SUSPEND_REQ");
    rrcsmc_ul_req.ul_cmd_type = TDSRLC_SUSPEND_CMD;
  }
  else
  {
    /*process stop req for SRNS*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Call RLC call back fun for processing STOP_REQ for SRNS");
    rrcsmc_ul_req.ul_cmd_type = TDSRLC_STOP_CMD;
  }

  /*Call RLC fun here  for  suspending or stoping UL(AM & UM) RB's & SRB's*/
  tdscrlc_ul_perform_suspend_or_stop_functionality(&rrcsmc_ul_req,
                                   &rrcsmc_ul_cnf);

  /*Call RLC fun here  for  stoping DL(AM & UM) RB's & SRB's*/
  if(suspend_cmd == TDSRRCSMC_SRNS_CMD)
  {
    tdscrlc_dl_perform_stop_functionality(&rrcsmc_dl_stop_req);
  }
  
  if(suspend_cmd == TDSRRCSMC_SMC_CMD)
  {
    /*process suspend cnf for SMC & send SMC complete*/
    tdsrrcsmc_process_suspend_cnf(&rrcsmc_ul_cnf);
  }
  else
  {
    /*process suspend cnf for SRNS relocation*/
    tdsrrcsmc_process_suspend_srns_cnf();
  }  
}


/*===========================================================================

FUNCTION tdsrrcsmc_perform_stop_or_suspend_srns

DESCRIPTION
   This function suspend's or stops all RLC UM and AM radio bearers by calling
   RLC function crlc_ul_perform_suspend_or_stop_functionality().process suspend 
   cnf for SMC & send SMC complete or process stop cnf for 
   SRNS relocation based on the request.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_perform_stop_or_suspend_srns
(
  tdsrrcsmc_stop_or_resume_rlc_cmd_type rlc_cmd
)
{
  /* Local variables */
  uint32 i, count;  
  uint32 num_ch_to_suspend_ul = 0;
  uint32 num_ch_to_stop_dl = 0;
  uint32 local_lc_id;
  rlc_lc_id_type srb2_lc_id = 0xFF; //initialize to keep lint happy
  /*Structure to hold info abut suspend Request which will be filled 
      by rrcsmc & RLC will use it for updating its state variables*/
  tdsrlc_ul_stop_suspend_request_type rrcsmc_ul_req;

  tdsrlc_dl_stop_request_type rrcsmc_dl_stop_req;
  
  /*Structure to hold info abut suspend confirmation from RLC*/
  /*move out of the function*/
  /*static  tdsrlc_ul_stop_suspend_cnf_type rrcsmc_ul_cnf;*/
  /*
  boolean srb4_purge = FALSE;
  
  boolean status = FALSE;
  */  


  /* Suspend SRB2 if this suspension is due to SMC */
  if((rlc_cmd == TDSRRCSMC_STOP_SRB2) || (rlc_cmd == TDSRRCSMC_STOP_NON_TM_RB))
  {
    srb2_lc_id = tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = srb2_lc_id;
    num_ch_to_suspend_ul++;
    /*adding stop DL fro SRB2 to block any ACK data on UL during the HO phase*/
    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
      tdsrrclcm_get_dl_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
    num_ch_to_stop_dl++;
  }

  if((rlc_cmd == TDSRRCSMC_STOP_NON_TM_RB)||(rlc_cmd == TDSRRCSMC_STOP_NON_SRB2))
  {
    /* Suspend SRB1 */
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(DCCH_UM_RADIO_BEARER_ID);
    num_ch_to_suspend_ul++;

    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
      tdsrrclcm_get_dl_rlc_lc_id(DCCH_UM_RADIO_BEARER_ID);
    num_ch_to_stop_dl++;

    /* Suspend UL SRB3 */
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
      tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
    num_ch_to_suspend_ul++;

    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
      tdsrrclcm_get_dl_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
    num_ch_to_stop_dl++;
      
    /* Suspend SRB4 if established */
    if (TDSRRCLCM_RLC_LC_ID_NOT_FOUND !=
        tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                   DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                   UE_MODE_ACKNOWLEDGED_DATA))
    {
      /*srb4_purge = TRUE;*/ /*Purge will be handled after SRNS to target RNC*/
      tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = 
        tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
      num_ch_to_suspend_ul++;

      tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl] = 
        tdsrrclcm_get_dl_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
      num_ch_to_stop_dl++;
    }
  }
  

  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    {              
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if((rlc_cmd == TDSRRCSMC_STOP_NON_TM_RB)||(rlc_cmd == TDSRRCSMC_STOP_NON_SRB2))
        {
          local_lc_id = tdsrrclcm_get_dl_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);
          if(local_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
          {
            if ( local_lc_id >= TDSRRCLCM_DL_UM_START_IDX && 
                 local_lc_id < (TDSRRCLCM_DL_AM_DATA_START_IDX + TDSUE_MAX_DL_AM_DATA_CHANNEL))
            {
              tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[num_ch_to_stop_dl]= (uint8) local_lc_id;
              num_ch_to_stop_dl++;
            }
          }
        
  
          local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);
  
          /*check if it is a AM or UM RLC RB only*/
          if ( local_lc_id >= TDSRRCLCM_UL_UM_START_IDX && 
               local_lc_id < (TDSRRCLCM_UL_AM_DATA_START_IDX + TDSUE_MAX_UL_AM_DATA_CHANNEL) )
          {
            tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[num_ch_to_suspend_ul].lc_id = (uint8) local_lc_id;
            num_ch_to_suspend_ul++;
          }
        }
      }
    }
  }


  /*record in the smc structure*/
  //This code is to satify KW error only.
  if (num_ch_to_suspend_ul > (TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: num_ch_to_suspend_ul is %d,out of bound",num_ch_to_suspend_ul);
    num_ch_to_suspend_ul = TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY;
  }
  tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan= (uint8) num_ch_to_suspend_ul;

  //This code is to satify KW error only.
  if (num_ch_to_stop_dl > (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: num_ch_to_stop_dl is %d,out of bound",num_ch_to_stop_dl);
    num_ch_to_stop_dl = TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY;
  }
  tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan = (uint8) num_ch_to_stop_dl;

  /*this cmd initiates suspension of RLC radio bearers*/

  /* Fill in the command parameters for SUSPENDING RLC RB's */
  rrcsmc_ul_req.nchan = 
      tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;

  rrcsmc_dl_stop_req.nchan= 
      tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan;

  /* In a loop set the contents of the suspend_req */
  for (i = 0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
  {
    rrcsmc_ul_req.suspend_stop[i].lc_id = 
     tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id; 
  }

  /*in a loop set the contents of the stop_req*/
  
  for (i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan; i++)
  {
    rrcsmc_dl_stop_req.lc_id[i] = 
    tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[i];
  }


/* Do not purge SRB4 here*/
/*
  if(suspend_cmd == TDSRRCSMC_SRNS_CMD)
  {
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Purge UL WM. SRB4 Purge Status : %d",srb4_purge);
    tdsrrcsmc_ul_purge_wm(srb4_purge);
  }
*/  

  /*process stop req for SRNS*/
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Call RLC call back fun for processing STOP_REQ for SRNS");
  rrcsmc_ul_req.ul_cmd_type = TDSRLC_STOP_CMD;


   /*Call RLC fun here  for  suspending or stoping UL(AM & UM) RB's & SRB's*/
   tdscrlc_ul_perform_suspend_or_stop_functionality(&rrcsmc_ul_req,
                                   &rrcsmc_ul_cnf);

  /*Call RLC fun here  for  stoping DL(AM & UM) RB's & SRB's*/

   tdscrlc_dl_perform_stop_functionality(&rrcsmc_dl_stop_req);

   TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process STOP CNF from LLC");
   tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = TRUE;    
   /*Mark SRB2 stop flag. Using it for second round stop flag*/
   if((rlc_cmd == TDSRRCSMC_STOP_NON_TM_RB)||(rlc_cmd == TDSRRCSMC_STOP_SRB2) )
   {
     tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag = TRUE;
   }

   

   /*Do not need to call following function, since it is for re-est SRB2.*/
  /*process suspend cnf for SRNS relocation*/
  /*tdsrrcsmc_process_suspend_srns_cnf();*/

}

/*===========================================================================

FUNCTION tdsrrcsmc_purge_srb_and_process_suspend_srns_cnf

DESCRIPTION
  This function continues what is pending in the funciton
  tdsrrcsmc_perform_stop_or_suspend_srns. It is suppose SMC run this
  one after LLC getting ChannelEstablishedInd

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_purge_srb_and_process_suspend_srns_cnf
(
  void
)
{
  boolean srb4_purge = FALSE;

  /* check if SRB4 established */
  if (TDSRRCLCM_RLC_LC_ID_NOT_FOUND !=
      tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                 DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                 UE_MODE_ACKNOWLEDGED_DATA))
  {
    srb4_purge = TRUE;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Purge UL WM. SRB4 Purge Status : %d",srb4_purge);
  tdsrrcsmc_ul_purge_wm(srb4_purge);
  

  /*process suspend cnf for SRNS relocation*/
  tdsrrcsmc_process_suspend_srns_cnf();
}

/*===========================================================================

FUNCTION tdsrrcsmc_update_integrity_ciphering_config

DESCRIPTION
  This function updates the Integrity and Ciphering Keys received from
  MM in TDSRRC_SIM_UPDATE_REQ command

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_update_integrity_ciphering_config
(
  rrc_sim_update_req_type *sim_update_req_ptr 
)
{


  /*local ciphering info type used to init global vars*/
  tdsrrcsmc_security_config_info_type *local_int_ciph_info_ptr; 
  uint32 j=0;       /*local variable use for counting in for loop*/
  
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state(); /* local var to store rrc state*/

  /*local var to keep track of cn_domain*/
  rrc_cn_domain_identity_e_type domain_id=RRC_CS_DOMAIN_CN_ID; 

  sys_modem_as_id_e_type sub_id = sim_update_req_ptr->as_id;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type curr_sub_id = 0;
  if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
  	TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
  	return;
  }
  curr_sub_id = tdsrrc_get_as_id();


#endif

  /* Update security information only if SIM_UPDATE_REQ contained any changes to
   security information*/
  if ((sim_update_req_ptr->security_update_info.security_update_status 
      == RRC_SIM_UPDATE_VALUE_CHANGED)
#ifdef FEATURE_LTE_TO_TDSCDMA
     ||
     (sim_update_req_ptr->security_update_info.security_update_status 
        == RRC_SIM_UPDATE_VALUE_LTE_MAPPED)
#endif        
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SIM_UPDATE_REQ");
  
    domain_id = sim_update_req_ptr->security_update_info.ciphering_info.domain_id;
    #ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    if((((tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_INACTIVE)&&(curr_sub_id == sub_id)) ||
          (curr_sub_id != sub_id))&&
    #else
    if((tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_INACTIVE)&&
    #endif
       (domain_id == RRC_CS_DOMAIN_CN_ID))
    {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
      tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
#endif
      /*save the security key set generated in G-RAT in this variable, 
       *Once RR receives ciphering mod command message in G it will 
       *send a command TDSRR_CIPHER_SYNC to RRC, then RRCSMC 
       *will move these saved security variables to its security database.
       */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Save security keys generated in G-RAT for CS domain");
      /*Mark saved security keys as valid*/
      tdssaved_security_key_set.grat_key_set_valid = TRUE;
      /*Save ciphering key here*/
      memscpy(tdssaved_security_key_set.ciphering_key, RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
             sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key,
             RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
      /*Save integrity key here*/
      memscpy(tdssaved_security_key_set.integrity_key, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
             sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
             RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );

      /*Print integrity key here*/
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                     sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
                                     sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key);

      /*Print ciphering key here*/
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                     sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
                                     sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key);
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
      tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
#endif
    }
    else
    {
      /*If domain I.D is CS check whether we have any saved security keys which 
        where generated in other RAT, If so the mark them as invalid*/
      if(tdsrrcsmc_return_saved_security_key_set_status(domain_id,sub_id) == TRUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New CS key rexed invalidating saved security key set=%d",tdssaved_security_key_set.grat_key_set_valid);
        tdsrrcsmc_reset_saved_security_key_set_status(sub_id);
      }
       
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
      tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms(curr_sub_id);
      tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
      tdsrrcsmc_set_smc_integrity_protection_to_global_params(sub_id);
#endif

      local_int_ciph_info_ptr = &(tdsrrcsmc_int_ciph_info);
        
    /*save the earlier keys in to a var.  We need this to calculate MAC for some
          SRB's whose activation time has not yet equalled the new activation times*/
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) ,
               local_int_ciph_info_ptr->int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Move CS INT KEY to CS old");
      }
      else if(domain_id == RRC_PS_DOMAIN_CN_ID)
      {
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               local_int_ciph_info_ptr->int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key,
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Move PS INT KEY to PS old");
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Wrong domain_id %d",domain_id);
      }
      

  
      /*when we change from the CN domain, we could be authenticated for the new
        CN domain and get new keys for that domain */
      
  
      /*copy number of cn domains into int_ciph_info struct*/  
      local_int_ciph_info_ptr->num_cn_domains = RRC_MAX_CN_DOMAINS;
  
  
      /*if authentication is being done after we have started Integrity, then
          set this flag for use by other procedures*/
      if ( tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_STARTED )
      {
         /*When SIM_UPDATE_REQ always doesnt mean new authentication.
          Incase of CS only SRVCC handover NAS can send SIM_UPDATE_REQ with
          PS mapped keys after handover is done. In this case just store the keys 
          and apply them.*/
#ifdef FEATURE_LTE_TO_TDSCDMA
       if((sim_update_req_ptr->security_update_info.security_update_status 
            == RRC_SIM_UPDATE_VALUE_LTE_MAPPED) && (RRC_PS_DOMAIN_CN_ID == domain_id))
        {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TDS:Updating Security key sets from SIM_UPDATE_REQ to security DB");                                                  
            /*any previously sent start value is obliterated.. we should start using 0's from
              now onwards.. hence set start_value_update_flag to false*/
            tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;  
        }        
       else
#endif
       {
         for (j=0; j < TDSRRC_MAX_SRB; j++)
          {
            tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
            dl_new_key_flag_per_rb[j]=TRUE;
   
            tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
            ul_new_key_flag_per_rb[j]=TRUE;
          }
        }
      }
      else
      {
        /*any previously sent start value is obliterated.. we should start using 0's from
          now onwards.. hence set start_value_update_flag to false*/
        tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;      
  
        /*Only if rrc state is dch/fach.. i.e. RAT is tds, update HFN's
          If in GSM rat do not update rat.. spec. CR 2001. Sep 02 */
        if ((rrc_state == TDSRRC_STATE_CELL_DCH) ||
            (rrc_state == TDSRRC_STATE_CELL_FACH)  
#ifdef FEATURE_LTE_TO_TDSCDMA
           ||
          ((sim_update_req_ptr->security_update_info.security_update_status 
              == RRC_SIM_UPDATE_VALUE_LTE_MAPPED) &&
                (rrc_state == TDSRRC_STATE_DISCONNECTED))           
#endif  
          )
        {
          /*reset the hfn values to 0's if authentication is done and UE is
           *in TDS mode in dch/fach
           * OR keys received from LTE */
          tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[0]=0x0;
          tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[1]=0x0;
          tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[2]=0x0;

          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Domain:%d RRC State:%d SIM Update Status:%d", 
            domain_id,
            rrc_state,
            sim_update_req_ptr->security_update_info.security_update_status);
        }
    
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CIPH START VAL: 0x%x 0x%x 0x%x", 
               local_int_ciph_info_ptr->int_ciph_info[domain_id].hfn[0],
               local_int_ciph_info_ptr->int_ciph_info[domain_id].hfn[1],
               local_int_ciph_info_ptr->int_ciph_info[domain_id].hfn[2]);      
      }    
  
      /*copy cipher algo and domain_id*/
      local_int_ciph_info_ptr->int_ciph_info[domain_id].cipher_algo = 
      sim_update_req_ptr->security_update_info.ciphering_info.cipher_algo;
  
      local_int_ciph_info_ptr->int_ciph_info[domain_id].domain_id = 
      sim_update_req_ptr->security_update_info.ciphering_info.domain_id;
  
      /*copy contents of ciphering_key to int_ciph_info struct
       *for this CN DOMAIN.  If we are currently using ciph key 1 then copy
     *new keys to key0.  else copy to 1 and set new_key flag for this
     *domain id to true
      */
      if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key_used == 1)
      {
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key[0],RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
               sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key,
               RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
      }
      else
      {
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key[1], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
               sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key,
               RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
      } 
      tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key=TRUE;
  
      if ((rrc_state == TDSRRC_STATE_CELL_DCH) ||
          (rrc_state == TDSRRC_STATE_CELL_FACH))
      {
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.
        new_key_for_start_value = TRUE;
      }
      /*copy contents of integrity_key to int_ciph_info struct for this CN DOMAIN
       */
      memscpy(local_int_ciph_info_ptr->int_ciph_info[domain_id].integrity_key,RRC_MAX_INTEGRITY_KEY_LENGTH *sizeof(byte), 
             sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
             RRC_MAX_INTEGRITY_KEY_LENGTH *sizeof(byte) );
  
      /*Print integrity key here*/
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                     sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
                                     sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key);
  
      /*Print ciphering key here*/
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                     sim_update_req_ptr->security_update_info.ciphering_info.integrity_key,
                                     sim_update_req_ptr->security_update_info.ciphering_info.ciphering_key);
  
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
      tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms(sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
      tdsrrcsmc_set_smc_integrity_protection_to_global_params(curr_sub_id);
#endif
    }/*if only security info has changed*/
  }
}/*rrcsmc_update_integrity_ciphering_info*/
/*===========================================================================

FUNCTION tdsrrcsmc_extract_latest_start_list_per_cn_domain

DESCRIPTION
  Extract the latest_start_list from count_c_i for both CS and PS
  domains.  START value = 20 MSB of count_c_i for both domains

DEPENDENCIES
  None.

RETURN VALUE
  None.
===========================================================================*/
static void tdsrrcsmc_extract_latest_start_list_per_cn_domain
(
rrc_cn_domain_identity_e_type domain_id,
tdsrrcsmc_start_list_update_e_type start_update
)
{
  uint32 max_start=0, prev_max_start, init_seq_num=0;
  byte   local_start_value[TDSRRC_MAX_START_LENGTH] = {0x00,0x00,0x00};
 

  //get the max start value and convert it to count_c_i type
  max_start = tdsrrcsmc_get_max_start_per_domain(domain_id);

  if(domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    if((tdsstart_value_to_transmit_cs_hho) &&
        (tdsstart_value_to_transmit_cs_hho >= max_start))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Updating max_start:0x%x with SVTHHO:0x%x",max_start,tdsstart_value_to_transmit_cs_hho);
      max_start = tdsstart_value_to_transmit_cs_hho;
    }
    /* Reset SVTHHO */
    tdsstart_value_to_transmit_cs_hho = 0;
  }

  if(TDSSTART_THRESHOLD == max_start)
  {
    //don't increment the start value, ref TS25.331 ch 8.5.9
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Max Start Value=0x%x reached the Threshold",max_start);
  }  
  else
  {
    /*increment by 2*/
    max_start += 2;
  }  
  max_start <<= 12;

  /*get START value stored previously for this CN domain and convert it to 
  count_i with tdsrrcllc_seq_num 0*/
  if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE)
  {
    prev_max_start = 
    tdsrrcsmc_convert_START_to_count_c_i(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, 
                                      init_seq_num);
  }
  else
  {
    prev_max_start = 
    tdsrrcsmc_convert_START_to_count_c_i(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, 
                                      init_seq_num);
  }

  /*if the previously stored START value is greater than the currently 
  extracted START value then do not change the latest_start value.
  this can happen if all the count_i's in both uplink and downlink for
  this domain have rolled over*/
  if (prev_max_start <= max_start)
  {
    tdsrrcsmc_convert_count_c_i_to_START(local_start_value, max_start);

    if(start_update == TDSRRCSMC_UPDATE_START_LIST)
    {
      /*copy the contents of local_start_value to latest_start_list for this
        cn domain*/
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
             local_start_value,        
             TDSRRC_MAX_START_LENGTH *sizeof(byte));
    }
    else
    {
      /*copy the contents of local_start_value to latest_start_list for this
        cn domain*/
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, RRC_MAX_HFN_LENGTH*sizeof(byte),
             local_start_value,        
             TDSRRC_MAX_START_LENGTH);
    }
  }
  else
  {
    if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == FALSE)
    {
      /*sync up HFN_LATEST to HFN as start_value_update_flag is not set*/
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, RRC_MAX_HFN_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,        
             RRC_MAX_HFN_LENGTH*sizeof(byte));
    }
  }
}/*rrcsmc_extract_latest_start_list_from_count_c_i*/
/*===========================================================================

FUNCTION tdsrrcsmc_append_hfns_to_rel_ind

DESCRIPTION
  This procedure appends the CS and PS domain HFNs to RRC_REL_IND message
  ptr which will be sent to MM.

DEPENDENCIES
  None.

RETURN VALUE
  Returns SUCCESS if PS and CS HFNs are successfully appended to rrc_rel_ind_type.
  Returns SUCCESS if PS and CS HFNs are not appended to rrc_rel_ind_type.


SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_hfns_to_rel_ind 
(
byte *hfncs, 
byte *hfnps
)
{
  /*if Integrity has already started, then we have to extract the START values
    from COUNT_C and COUNT_I values according to 8.5.9 of 25.331 v3a0*/
  if (tdsrrcsmc_integrity_protection_info.ip_status != TDSRRCSMC_SECURITY_STARTED)
  {
    /*if STATUS is not started then we never started Integrity Prot.
        we were in connected state only to do location update etc.
        so init tdsrrcsmc_integrity_protection_info with the recieved 
        integrity_ciphering_config and extract the latest start list*/
    tdsrrcsmc_init_integrity_protection_info(tdsrrc_get_as_id());
  }

  tdsextract_cause_rel_ind=TRUE;
  if(tdsrrcsmc_int_ciph_info.int_start_status_for_conn[RRC_CS_DOMAIN_CN_ID])
  {
    tdsrrcsmc_extract_latest_start_list_per_cn_domain(RRC_CS_DOMAIN_CN_ID,
                                                   TDSRRCSMC_UPDATE_START_LIST);  

  }

  if(tdsrrcsmc_int_ciph_info.int_start_status_for_conn[RRC_PS_DOMAIN_CN_ID])
  {
    tdsrrcsmc_extract_latest_start_list_per_cn_domain(RRC_PS_DOMAIN_CN_ID,
                                                   TDSRRCSMC_UPDATE_START_LIST);     
  }

  tdsextract_cause_rel_ind=FALSE;

  /* Before copying the calculated Start value to the REL_IND,check to see
     if there are any new keys pending.If new keys are pending,then we 
     is supposed to write 0 to the SIM.One way to check is to see the pending
     flag of SRB2 because this flag is set on SIM_UPDATE_REQ and is reset on 
     the receipt of SMC,since SMC always comes with the new Integrity config */

  if(tdsrrcsmc_integrity_protection_info.count_i_info[RRC_CS_DOMAIN_CN_ID].
     dl_new_key_flag_per_rb[DCCH_AM_RADIO_BEARER_ID] == TRUE)
  {
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn[0]=0x0;
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn[1]=0x0;
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn[2]=0x0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"New Keys pending for CS domain.Setting HFN to zero to write to SIM");
  }

  if(tdsrrcsmc_integrity_protection_info.count_i_info[RRC_PS_DOMAIN_CN_ID].
     dl_new_key_flag_per_rb[DCCH_AM_RADIO_BEARER_ID] == TRUE)
  {
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn[0]=0x0;
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn[1]=0x0;
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn[2]=0x0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"New Keys pending for PS domain.Setting HFN to zero to write to SIM");
  }

  /*copy the contents of the latest_start_list just extracted into hfncs and
    hfnps byte ptrs received.*/
  memscpy(hfncs, RRC_MAX_HFN_LENGTH *sizeof(byte), 
         tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn,
         RRC_MAX_HFN_LENGTH *sizeof(byte));

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"REL CS START: 0x%x 0x%x 0x%x", hfncs[0], hfncs[1], hfncs[2]);

  memscpy(hfnps, RRC_MAX_HFN_LENGTH *sizeof(byte),
         tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].hfn,
         RRC_MAX_HFN_LENGTH *sizeof(byte));      

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"REL PS START: 0x%x 0x%x 0x%x", hfnps[0], hfnps[1], hfnps[2]);

  /*since we are going to idle mode, set the integrity started flag to
    NOT_STARTED*/
  tdsrrcsmc_integrity_protection_info.ip_status = TDSRRCSMC_SECURITY_NOT_STARTED;        
  tdsrrcsmc_init_integrity_protection_info(tdsrrc_get_as_id());

  tdsrrcsmc_go_to_smc_initial_substate();
  TDSRRC_MSG0(MSG_LEGACY_MED,"SMC going to INITIAL_STATE");

  return SUCCESS;
}/*tdsrrcsmc_append_hfns_to_rel_ind*/

/*===========================================================================

FUNCTION tdsrrcsmc_check_new_keys_and_reset_append_start_per_domain

DESCRIPTION
  This function checks whether RRC received new keys from NAS and accordingly
  initializes the a byte *value with START values for the domain requested.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_check_new_keys_and_reset_append_start_per_domain 
(
byte  *value,
rrc_cn_domain_identity_e_type domain_id,
tdsrrcsmc_append_start_e_type start_type
)
{

  if(TRUE == tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key_for_start_value)
  {
    /* Consider the case of SMC being initiated and new keys are received from MM.
     * Before UE gets L2 ACK for SM complete, CU procedure started (because of cell reselction)
     * So SMC will be aborted as per spec section 8.1.12.4b and UE reverted to old configuration including IK
     * In this case UE has to sent START = 0x0 so that UTRAN uses this START to initialize HFN = 0x0 in Count_I 
     */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"new_key_for_start_value is TRUE for domain:%d",domain_id);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set START = 0x00 0x00 0x00");
    /* This flag will be set to FALSE when SMC arrives and rcvd_new_keys_flag == TRUE */

    memset(value, 0x0,TDSRRC_MAX_START_LENGTH);

    if(domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended CS-START: 0x%x 0x%x 0x%x", value[0], value[1], value[2]);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended PS-START: 0x%x 0x%x 0x%x", value[0], value[1], value[2]);
    }  
  }
  else
  {
    tdsrrcsmc_append_start_per_domain(value,
             domain_id,
             start_type);
  }
}/*tdsrrcsmc_append_start_per_domain*/


/*===========================================================================

FUNCTION tdsrrcsmc_append_start_per_domain

DESCRIPTION
  This function initializes the a byte ptr with START values for the domain
  requested.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_append_start_per_domain 
(
byte  *value,
rrc_cn_domain_identity_e_type domain_id,
tdsrrcsmc_append_start_e_type start_type
)
{
  //if SMC is not received, start should never be re-calculated since 
  //count_c and count_i are not valid till a SMC is received..
  //so just return the START value read from the SIM
  if(tdsrrcsmc_int_ciph_info.int_start_status_for_conn[domain_id] == FALSE)
  {
    if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE)
    {
      /*There might be a race condition in the N/W where it forgets to start ciphering 
       *and or modify integrity for a domain & its very much possible that we can have 
       *the call for this domain gets released, Start value gets updated here are part of 
       *RB establishment procedure. On the same RRC connection we may have another 
       *signaling connection establish. To have Start values in sync for IDT & the one used 
       *in processing for SMC we need to check & use updated start value if available.
       */
      memscpy(value, RRC_MAX_HFN_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,
             RRC_MAX_HFN_LENGTH*sizeof(byte));
    }
    else
    {
      memscpy(value, RRC_MAX_HFN_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,    
            RRC_MAX_HFN_LENGTH*sizeof(byte));         
    }
  }
  //otherwise, START needs to be re-calculated based on whether the caller
  //is requesting new START to be extracted and calculated or just the old
  //start needs to be sent..
  else
  {
    if(start_type == TDSRRCSMC_EXTRACT_AND_APPEND_START)
    {
      tdsrrcsmc_extract_latest_start_list_per_cn_domain(domain_id,
                                                     TDSRRCSMC_UPDATE_START_LIST_LATEST);
  
      /*copy the contents of latest_start_list into start_list*/
      memscpy(value, RRC_MAX_HFN_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,    
             RRC_MAX_HFN_LENGTH*sizeof(byte));         
  
      tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = TRUE;
    }
    else
    {
      //if start value is updated, then send start value from hfn_latest
      if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE)
      {
        /*copy the contents of hfn_latest into start_list*/
        memscpy(value, RRC_MAX_HFN_LENGTH*sizeof(byte),
               tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,    
               RRC_MAX_HFN_LENGTH*sizeof(byte));         
      }
      else
      {
        /*copy the contents of hfn into start_list*/
        memscpy(value, RRC_MAX_HFN_LENGTH*sizeof(byte),
               tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,    
               RRC_MAX_HFN_LENGTH*sizeof(byte));         
      }
    }
  }

  if(domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended CS-START: 0x%x 0x%x 0x%x", value[0], value[1], value[2]);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended PS-START: 0x%x 0x%x 0x%x", value[0], value[1], value[2]);
  }
}/*tdsrrcsmc_append_start_per_domain*/
/*===========================================================================

FUNCTION tdsrrcsmc_append_start_list

DESCRIPTION
  This function initializes the START_LIST with the latest values to 
  be sent to the UTRAN by the RCE, CU  and ASU procedures. The START
  values for all CN domains for which we have received Integrity
  Protection Info will be sent.
  
  RCE, CU and ASU procedures will call this function in SMC.

DEPENDENCIES
  None.

RETURN VALUE
  This function returns SUCCESS if STARTlist is appended.
  This function returns FAILURE if STARTlist is not appended.

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_start_list 
(
 tdsrrc_STARTList *start_list_ext,
tdsrrcsmc_append_start_e_type start_type
)
{
  uecomdef_status_e_type status;

  status =  tdsrrcsmc_append_start_value_to_start_list (start_list_ext,
                                                     start_type
                                                     ,TRUE                                         
                                                     ,RRC_CS_DOMAIN_CN_ID            /*dummy value. Doesn't matter b'coz both 
                                                                                                      *domain are considered to be open anyways*/
                                                      );

  return status;
}/*tdsrrcsmc_append_start_list*/
/*===========================================================================

FUNCTION tdsrrcsmc_send_sync_ind_to_mm

DESCRIPTION
   This function processes SMC proecdure in wait for l2 ack state.  We can
   be waiting for an L2 ACK for a SMC complete command

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_send_sync_ind_to_mm
(
rrc_sync_cause_e_type sync_cause
)
{
  mm_cmd_type *mm_cmd_ptr;                      /* Pointer to MM command */

  /*set the reconfig flag to false as we got the l2_ack for Security Complete*/
  tdsrrcsmc_integrity_protection_info.reconfig        = FALSE;

  if(tdsrrcidt_is_idt_waiting_for_l2ack_per_domain(tdsrrcsmc_int_ciph_info.latest_config_cn_id))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT L2_ACK after SYNC_IND for %d",tdsrrcsmc_int_ciph_info.latest_config_cn_id);

    tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_SUCCESS,
                                 tdsrrcsmc_int_ciph_info.latest_config_cn_id);

  }

  /* Allocate memory for RRC_SYNC_IND command */
  if ( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) != NULL)
  {
#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND  
    #error code not present
#endif
    {
      /* No paramters to fill in for RRC_SYNC_IND command */

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRC_SYNC_IND is sent %d", tdsrrcsmc_int_ciph_info.
                 latest_config_cn_id,0,0);

      mm_cmd_ptr->cmd.rrc_sync_ind.cn_domain_id = 
        tdsrrcsmc_int_ciph_info.latest_config_cn_id;
    }
    
#ifdef FEATURE_DUAL_SIM
    mm_cmd_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif

    mm_cmd_ptr->cmd.rrc_sync_ind.rab_id_present = FALSE;
    mm_cmd_ptr->cmd.rrc_sync_ind.cause = sync_cause;

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
    #error code not present
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_cmd_ptr);
    }
    mm_put_cmd(mm_cmd_ptr);

    tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[tdsrrcsmc_int_ciph_info.latest_config_cn_id] = TRUE;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Latest CN ID=%d,sync_ind_sent status for CS=%d,PS=%d",
        tdsrrcsmc_int_ciph_info.latest_config_cn_id,
        tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_CS_DOMAIN_CN_ID],
        tdsrrcsmc_int_ciph_info.is_sync_ind_sent_to_nas[RRC_PS_DOMAIN_CN_ID]);

  }
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Failed to allocate memory for MM cmd: %d",RRC_SYNC_IND,0,0);
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_dl_integrity_check_info

DESCRIPTION
  This function retrieves the MAC and SEQ_NO from the integrity_check_info
  Then populates MAC field to RB_ID on which the message was received and
  the SEQ_NO to zero.

  In downlink when we receive Integrity Check Info, we first extract the
  MAC and SEQ_NO from Integrity Check Info struct.  Then MAC is assigned
  to RB_ID of the received message and SEQ_NUM field is init to zero before
  we run it through the f9 engine.
  
DEPENDENCIES
  None.

RETURN VALUE
  Returns SUCCESS if Integrity Check Info is present and 
  int_check_info is populated with mac and seq. number
  Returns FAILURE if Integrity Check Info is not present.

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_process_dl_integrity_check_info
(
byte* dl_sdu_ptr,                         /* encoded byte stream*/
uint32 dl_sdu_length,                     /* Length of SDU received */
tdsrrcsmc_integrity_check_info_type * int_check_info,/*integrity check info struct*/
tdsrrc_RB_Identity rb_id                     /*rb_id of the received message*/
)
{
  /* Local Variable to store mac type position, seq num position*/
  uint32 mac_type_pos, sn_type_pos;                     

  /*local var to init Message bit stream with sequence number set to 0*/
  uint32 seq_num_zero=0x0;

  /* Check whether Integrity Check info is present */
  if (0x00 == b_unpackb(dl_sdu_ptr, TDSINTEGRITY_CHECK_INFO_PRESENT_POS,               
                        TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Integrity Check Info missing");
    return FAILURE;
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if ( 0x01 == b_unpackb(dl_sdu_ptr, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                              TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if ( (dl_sdu_length * 8) < 
         (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
          TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Msg Len.");
      return FAILURE;
    }

    /*peek into byte stream and get the MAC */
    mac_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS+
                   TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    int_check_info->msg_auth_code =
    b_unpackd(dl_sdu_ptr, (uint16)mac_type_pos, TDSRRC_MAC_LENGTH_IN_BITS);

    /*peek into byte stream and get the Sequence Number */
    sn_type_pos =  TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                   TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                   TDSRRC_MAC_LENGTH_IN_BITS;

    int_check_info->msg_seq_num=b_unpackb(dl_sdu_ptr, (uint16)sn_type_pos, 
                                          TDSRRC_SEQ_NUM_LENGTH_IN_BITS);


    /*pack mac with rb_id */
    mac_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS + 
                   TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    b_packd(rb_id, dl_sdu_ptr, (uint16)mac_type_pos, TDSRRC_MAC_LENGTH_IN_BITS);

    /*pack seq num with 0's */
    sn_type_pos =  TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                   TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                   TDSRRC_MAC_LENGTH_IN_BITS;

    b_packb((uint8)seq_num_zero, dl_sdu_ptr, (uint16)sn_type_pos, TDSRRC_SEQ_NUM_LENGTH_IN_BITS);

    return SUCCESS;
  }

  /*we should never come here.  for future use*/
  return FAILURE;
}/*tdsrrcsmc_process_dl_integrity_check_info*/

/*===========================================================================

FUNCTION  tdsrrcsmc_get_rlc_ul_info

DESCRIPTION
  This function extracts the hfn and ciph_act_time for lc id

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
static void tdsrrcsmc_get_rlc_ul_info
(
  tdsrlc_hfn_req_type  *local_hfn_type,
  rlc_lc_id_type    lc_id,
  uint32            *hfn,
  uint16            *ciph_sn,
  boolean           *hfn_valid
)
{
  uint32 i=0;
  *hfn = 0xFFFFFFFF;
  *ciph_sn = 0xFFFF;
  *hfn_valid = FALSE;
  
  for(i=0; i<local_hfn_type->ul_nchan; i++)
  {
    if(local_hfn_type->hfn_per_rb_ul[i].rlc_id == lc_id)
    {
      *hfn = local_hfn_type->hfn_per_rb_ul[i].hfn;  
      //*ciph_sn = local_hfn_type->hfn_per_rb_ul[i].rlc_sn;   by jiangbing 
      //*hfn_valid = local_hfn_type->hfn_per_rb_ul[i].hfn_valid;  by jiangbing 

      /*check if lc_id of the Radio Bearer belongs to UM Mode*/           
      if((lc_id >= TDSRRCLCM_UL_UM_START_IDX) && (lc_id < TDSRRCLCM_UL_AM_DATA_START_IDX))
      {
        /*since UM mode RB's HFN is 25 bits, remove the LSB 5 bits  to get START value*/      
        *hfn >>= 5;
      }
      break;
    }
  }
}

/*===========================================================================

FUNCTION  tdsrrcsmc_get_rlc_dl_info

DESCRIPTION
  This function extracts the hfn and ciph_act_time for lc id

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
static void tdsrrcsmc_get_rlc_dl_info
(
  tdsrlc_hfn_req_type  *local_hfn_type,
  rlc_lc_id_type    lc_id,
  uint32            *hfn,
  uint16            *ciph_sn,
  boolean           *hfn_valid
)
{
  uint32 i=0;
  *hfn = 0xFFFFFFFF;
  *ciph_sn = 0xFFFF;
  *hfn_valid = FALSE;
  
  for(i=0; i<local_hfn_type->dl_nchan; i++)
  {
    if(local_hfn_type->hfn_per_rb_dl[i].rlc_id == lc_id)
    {
      *hfn = local_hfn_type->hfn_per_rb_dl[i].hfn;  
      //*ciph_sn = local_hfn_type->hfn_per_rb_dl[i].rlc_sn;  by jiangbing
      //*hfn_valid = local_hfn_type->hfn_per_rb_dl[i].hfn_valid;  by jiangbing
      
      /*check if lc_id of the Radio Bearer belongs to UM Mode*/           
      if((lc_id >= TDSRRCLCM_DL_UM_START_IDX) && (lc_id < TDSRRCLCM_DL_AM_DATA_START_IDX))
      {
        /*since UM mode RB's HFN is 25 bits, remove the LSB 5 bits  to get START value*/      
        *hfn >>= 5;
      }
      break;
    }
  }
}

/*===========================================================================

FUNCTION  tdsrrcsmc_revert_to_old_cipher_config_db

DESCRIPTION
  This function reverts the value of global variable 'tdscipher_config_db' from its back-up value.
  This function is supposed to be called whenever RRC has to revert back to old configuration
  during HHO(timing initialized) or SRNS relocation. 

  NOTE: Make sure that "tdscipher_config_db" value is backed-up before this function is 
        called. By default backup variable will have junk value (its value is never cleared) 

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_revert_to_old_cipher_config_db
(
  void
)
{
  uint32 i=0, hfn=0;
  uint16  ciph_act_sn;  
  tdsrlc_hfn_req_type local_hfn_type;
  boolean hfn_valid = FALSE;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Reverting to back-up of tdscipher_config_db. Before reverting MAC-config-type:(CS : %d,PS: %d) ",tdscipher_config_db.mac_cipher_info[TDSCS_DOMAIN].cipher_cfg_type,tdscipher_config_db.mac_cipher_info[TDSPS_DOMAIN].cipher_cfg_type);
  /* Taking back up of global variable which is used to send ciphering configuraiton to MAC */
  tdscipher_config_db = tdsbackup_of_security_variables.tdscipher_config_db;

  memset(&local_hfn_type, 0, sizeof(tdsrlc_hfn_req_type));
  (void) tdsrlc_rrc_get_hfn_req(&local_hfn_type);

  for(i=0; i<tdscipher_config_db.rlc_ul_cipher_info.nchan; i++)
  {
    hfn = 0;
    ciph_act_sn = 0;
    hfn_valid = FALSE;

    tdsrrcsmc_get_rlc_ul_info(&local_hfn_type, 
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].lc_id, 
      &hfn,
      &ciph_act_sn,
      &hfn_valid);

    if((hfn == 0xFFFFFFFF) && (hfn_valid == FALSE))
    {
      continue;
    }
    
    /* If sn at RLC > RRC then activation time already hit. Send act sn as Immediate. */
    if(tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn <= ciph_act_sn)
    {
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn = ciph_act_sn;
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn = hfn;
    }
   /* If sn at RLC < RRC then two cases arrive
    *  1. Roll over happened. In this case HFN at RRC and RLC are NOT equal. Send act sn as immediate
    *     Change the HFN to the latest one that RLC has returned.
    *  2. Configuration is pending. In this case HFN at RRC and RLC are equal.
    *     No need to change anything. Backup of cipher_config_db will take care.
    */  
    else if(tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn > ciph_act_sn)
    {
      //if hfn at RLC and RRC are NOT equal then it is roll over case. Need to apply immediate config
      if((tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn != hfn) &&
         (hfn_valid == TRUE))
      {
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn = ciph_act_sn;
        tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn = hfn;
      }
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"UL_DBG:Cipher DB: LC:%d HFN:0x%x SN:%d",tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].lc_id,
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].hfn, 
      tdscipher_config_db.rlc_ul_cipher_info.ciphering[i].ciph_act_sn);
    
  }

  for(i=0; i<tdscipher_config_db.rlc_dl_cipher_info.nchan; i++)
  {
    hfn = 0;
    ciph_act_sn = 0;
    hfn_valid = FALSE;

    tdsrrcsmc_get_rlc_dl_info(&local_hfn_type, 
      tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].lc_id,
      &hfn,
      &ciph_act_sn,
      &hfn_valid);
     
    if((hfn == 0xFFFFFFFF) && (hfn_valid == FALSE))
    {
      continue;
    }
    
    /* If sn at RLC > RRC then activation time already hit. Send act sn as Immediate. */
    if(tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn <= ciph_act_sn)
    {
      tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn = ciph_act_sn;
      tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn = hfn;
    }
    else if(tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn > ciph_act_sn)
    {
      //if hfn at RLC and RRC are NOT equal then it is roll over case. Need to apply immediate config
      if((tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn != hfn) &&
         (hfn_valid == TRUE))
      {
        tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn = ciph_act_sn;
        tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn = hfn;
      }
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DL_DBG:Cipher DB: LC:%d HFN:0x%x SN:%d",tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].lc_id,
      tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].hfn, 
      tdscipher_config_db.rlc_dl_cipher_info.ciphering[i].ciph_act_sn);    
  }
}

/*===========================================================================

FUNCTION  tdsrrcsmc_backup_cipher_config_db

DESCRIPTION
  This function back-up the global variable 'tdscipher_config_db'. This function
  is supposed to be called immediately before updating this variable for
  STEP1 cipher config for HHO.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_backup_cipher_config_db
(
  void
)
{

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Taking back-up of tdscipher_config_db. Old MAC-config-type:(CS : %d,PS: %d) ",tdscipher_config_db.mac_cipher_info[TDSCS_DOMAIN].cipher_cfg_type,tdscipher_config_db.mac_cipher_info[TDSPS_DOMAIN].cipher_cfg_type);
  /* Taking back up of global variable which is used to send ciphering configuraiton to MAC */
  tdsbackup_of_security_variables.tdscipher_config_db = tdscipher_config_db;

}

/*===========================================================================

FUNCTION  tdsrrcsmc_backup_global_security_variables

DESCRIPTION
  This function back-up all security related global variables. This function
  is supposed to be called before processing any config/re-config request.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_backup_global_security_variables
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Taking back-up of global security variables ");
  /* Taking back up of global variable which is used to send ciphering configuraiton to MAC */
  tdsbackup_of_security_variables.ciphering_info = tdsrrcsmc_ciphering_info;
  tdsbackup_of_security_variables.tdscipher_config_db = tdscipher_config_db;
  tdsbackup_of_security_variables.ciph_rb_info = tdsrrcsmc_ciph_rb_info;
  tdsbackup_of_security_variables.integrity_protection_info = tdsrrcsmc_integrity_protection_info;
  tdsbackup_of_security_variables.new_tm_rb_ind_flag = TDSNEW_TM_RB_IND_FLAG;
  tdsbackup_of_security_variables.rrcsmc_int_ciph_info = tdsrrcsmc_int_ciph_info;
  tdsbackup_of_security_variables.start_value_to_transmit_flag = TDSSTART_VALUE_TO_TRANSMIT_FLAG;
  /* reset the global variable */
  tdsis_revert_to_old_cipher_variable_reqd = FALSE;
}

/*===========================================================================

FUNCTION  tdsrrcsmc_revert_to_old_security_variables

DESCRIPTION
  This function reverts the security variables to their back-up values. This
  function will also take care of 

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_revert_to_old_security_variables
(
  void
)
{
  rrc_cn_domain_identity_e_type local_cn_domain;
  uint32 new_sn, old_sn, old_ul_count_i;
  tdsrrcsmc_ciphering_config_info_type temp_cipher_config_db;
  tdsrrcsmc_security_config_info_type temp_rrcsmc_int_ciph_info;
 
  /* LLC might have processed the TDSRRC_CIPH_CONFIG_REQ, and TDSRRC_CIPH_CONFIG_CNF
   * might be present in the cmd queue when cell update is triggered. Hence, 
   * check whether revert back to old ciphering variables is required or not. 
   * If LLC is not in suspended state then it indicates that LLC has already sent
   * TDSRRC_CIPH_CONFIG_CNF and it is in queue if smc substate is still rrcsmc-wait-
   * for-cipher-cnf, in this case act as if all the global variables are updated
   * and safe to revert back to old configuration.
   */  
  if ((TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF != tdsrrcsmc_substate) || 
      (TRUE == tdsis_revert_to_old_cipher_variable_reqd)
     )
  {
    /* tdsis_revert_to_old_cipher_variable_reqd set to TRUE indicates that integrity
        variables are already reverted back, so no need to revert back the integrity
        variables else if it is set to FALSE then it means that all the gloabal 
        variables are safe to revert back to old config */
    if (FALSE == tdsis_revert_to_old_cipher_variable_reqd)
    {
      /* There is a possibility that RRC might have already sent number of messages
         on SRB 2 (say measurement reports) before L2 ack, RRC should not send 
         messages with any SN for which RRC message is already been sent. Hence, 
         move the RRC-SN in backed-up security variable to current SN for SRB-2. 
         Special care needs to be taken for roll-over case.
    
         NOTE: SRBs (other than SRB2) are not taken care to avoid extra complexity
                   of handling corner scenarios like activation time expiry etc for those SRBs.
                   SRB2 always has activation time now, hence no need to look for activation
                   time expiry and possibility of messages going on SRB2 is more than any other SRBs.
      */
      local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id;
    
      new_sn = tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].ul_count_i[2] & 0xF;
    
      local_cn_domain = tdsbackup_of_security_variables.rrcsmc_int_ciph_info.latest_config_cn_id;
    
      old_ul_count_i = tdsbackup_of_security_variables.integrity_protection_info.
                       count_i_info[local_cn_domain].ul_count_i[2];
      old_sn =  old_ul_count_i & 0xF;
    
      /* Assumption: RRC wont send more than 15 RRC Messages on on SRB2 before L2-Ack is 
          received for SM Complete message */
      old_ul_count_i = (((old_ul_count_i >> 4) + (uint8)(new_sn < old_sn))<<4) | new_sn;
    
      
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reverting to old Integrity variables. cnt-i-SRB2: prev: %d, cur: %d, after: %d", 
        tdsbackup_of_security_variables.integrity_protection_info.
                                       count_i_info[local_cn_domain].ul_count_i[2], 
           tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].ul_count_i[2], old_ul_count_i);
    
      /* updating count-I for SRB2 appropriately with new-SN */
      tdsbackup_of_security_variables.integrity_protection_info.
                                       count_i_info[local_cn_domain].ul_count_i[2] = old_ul_count_i;
  
      /* save tdsrrcsmc_int_ciph_info received in last security mode command */
      temp_rrcsmc_int_ciph_info = tdsrrcsmc_int_ciph_info;
  
      tdsrrcsmc_int_ciph_info = tdsbackup_of_security_variables.rrcsmc_int_ciph_info;

      /* copy the saved tdsrrcsmc_int_ciph_info to backup variable */
      tdsbackup_of_security_variables.rrcsmc_int_ciph_info = temp_rrcsmc_int_ciph_info;

      tdsrrcsmc_integrity_protection_info = tdsbackup_of_security_variables.integrity_protection_info;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reverting to old Ciphering variables");

    /* Save the value of current tdscipher_config_db because this will be put into backup_* 
       structure. Variable backup.tdscipher_config_db will be used while sending ABORT_CIPH
       to RLC */
    temp_cipher_config_db = tdscipher_config_db;
  
    tdsrrcsmc_ciphering_info = tdsbackup_of_security_variables.ciphering_info;
    tdscipher_config_db =  tdsbackup_of_security_variables.tdscipher_config_db;
    tdsrrcsmc_ciph_rb_info =  tdsbackup_of_security_variables.ciph_rb_info;
    TDSNEW_TM_RB_IND_FLAG = tdsbackup_of_security_variables.new_tm_rb_ind_flag;
    TDSSTART_VALUE_TO_TRANSMIT_FLAG = tdsbackup_of_security_variables.start_value_to_transmit_flag;
  
    /* restoring the value of tdscipher_config_db in backup variable - this may not be optimal
       but seems easier way of doing it */
    tdsbackup_of_security_variables.tdscipher_config_db = temp_cipher_config_db;


    /*Check if the num_keys match*/
    if(tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys
         != tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL/DL num_keys mis-match due to half config: UL:%d DL:%d. Make all to 4",tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys,tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys);
      tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.num_keys = 4;
      tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.num_keys = 4;
    }

    /*Print out all the debugging informations*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Ciphering db Key-0:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.key_info[0].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Ciphering db Key-1:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.key_info[1].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Ciphering db Key-3:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.key_info[2].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL Ciphering db Key-4:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.ul_cipher_info.key_info[3].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Ciphering Key-0:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.key_info[0].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Ciphering Key-2:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.key_info[1].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Ciphering Key-3:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.key_info[2].key_val);

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Ciphering Key-4:");
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                 tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old,
                                 tdscipher_config_db.l1_cipher_info.cipher_key_req.dl_cipher_info.key_info[3].key_val);

  }
  else
  {
    /* Reverting of integrity variable is requried because cell update goes out of RRC
       before this function is called....we can modularize following by putting into a function */

    /* There is a possibility that RRC might have already sent number of messages
       on SRB 2 (say measurement reports) before L2 ack, RRC should not send 
       messages with any SN for which RRC message is already been sent. Hence, 
       move the RRC-SN in backed-up security variable to current SN for SRB-2. 
       Special care needs to be taken for roll-over case.
  
       NOTE: SRBs (other than SRB2) are not taken care to avoid extra complexity
             of handling corner scenarios like activation time expiry etc for those SRBs.
             SRB2 always has activation time now, hence no need to look for activation
             time expiry and possibility of messages going on SRB2 is more than any other SRBs.
    */
    local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id;
  
    new_sn = tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].ul_count_i[2] & 0xF;
  
    local_cn_domain = tdsbackup_of_security_variables.rrcsmc_int_ciph_info.latest_config_cn_id;
  
    old_ul_count_i = tdsbackup_of_security_variables.integrity_protection_info.
                     count_i_info[local_cn_domain].ul_count_i[2];
    old_sn =  old_ul_count_i & 0xF;
  
    /* Assumption: RRC wont send more than 15 RRC Messages on on SRB2 before L2-Ack is 
        received for SM Complete message */
    old_ul_count_i = (((old_ul_count_i >> 4) + (uint8)(new_sn < old_sn))<<4) | new_sn;
  
    
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reverting to old Integrity variables. cnt-i-SRB2: prev: %d, cur: %d, after: %d", 
        tdsbackup_of_security_variables.integrity_protection_info.
                                     count_i_info[local_cn_domain].ul_count_i[2], 
         tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].ul_count_i[2], old_ul_count_i);
  
    /* updating count-I for SRB2 appropriately with new-SN */
    tdsbackup_of_security_variables.integrity_protection_info.
                                     count_i_info[local_cn_domain].ul_count_i[2] = old_ul_count_i;
 
    /* save tdsrrcsmc_int_ciph_info received in last security mode command */
    temp_rrcsmc_int_ciph_info = tdsrrcsmc_int_ciph_info;

    tdsrrcsmc_int_ciph_info = tdsbackup_of_security_variables.rrcsmc_int_ciph_info;

    /* copy the saved tdsrrcsmc_int_ciph_info to backup variable */
    tdsbackup_of_security_variables.rrcsmc_int_ciph_info = temp_rrcsmc_int_ciph_info;

    tdsrrcsmc_integrity_protection_info = tdsbackup_of_security_variables.integrity_protection_info;

    /* Avoid reverting to old secrutiy global variables because there is a possiblity
       that all the global variable are not updated at this point. Hence wait till global
       variables are updated */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsis_revert_to_old_cipher_variable_reqd to TRUE");
    tdsis_revert_to_old_cipher_variable_reqd = TRUE;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_step_2_cipher_config_for_first_tm_rb_setup

DESCRIPTION
  Process Ciph Config Cnf for first TM RBs being setup

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
uint32 tdsrrcsmc_step_2_cipher_config_for_first_tm_rb_setup
( 
void
)
{
  uint32 step_2_start, step_2_ciph_act_time, step_2_hfn;
  
  /*256 is the CFN rollover length*/
  /*250 is used to activate ciphering after 250 10ms time interval,
    this should give sufficient time for n/w side and ue side to have
    configured ciphering on the rb's successfully*/
  step_2_ciph_act_time = (uint8)((tdsseq_get_cfn() + 250) % 256);
  /*round it to 80ms boundary */
  step_2_ciph_act_time = (step_2_ciph_act_time/8)*8; 
  

  //set ul/dl rlc cipher info to 0's
  tdscipher_config_db.rlc_ul_cipher_info.nchan=0;
  tdscipher_config_db.rlc_dl_cipher_info.nchan=0;

  /*get the HFN for Step_2 HFN calculation*/
  step_2_start = tdsrrcsmc_get_latest_transmitted_start_value_per_domain(RRC_CS_DOMAIN_CN_ID);

  //convert start to count_i
  step_2_hfn = step_2_start << 4;

  /*set the HFN value for MAC to the step_2 HFN value.. also set the config
    type enum to STEP_2.. and also copy the ciph_act_time for mac to the new
    activation time sent in HO_TO_UTRAN_COMPLETE message
    we don't need to increment HFN+1 for MAC, as MAC layer takes care of it
    once we hit activation time*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_STEP2_CIPHER_CFG;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn = 
    step_2_hfn;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].ciph_act_time = 
    step_2_ciph_act_time;
      
  /*call llc function to update rlc/mac ciphering configuration for
    starting STEP_2 ciphering*/
  tdsrrcllc_update_ciphering_config_info_for_first_tm_rb();

  /*reset the cipher config type to non-HO*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_CIPHER_CFG;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ciph act time is %d HFN is 0x%x",step_2_ciph_act_time,step_2_hfn);
  
  return step_2_ciph_act_time;
}

/*===========================================================================
  FUNCTION tdsrrcsmc_process_cipher_config_for_first_tm_setup

DESCRIPTION
  Process Step 1 ciphering for First TM RB Setup

DEPENDENCIES
  None.

RETURN VALUE
  uecomdef_status_e_type SUCCESS if Ciphering is started on TM RB's
                         FAILURE if Ciphering is not started on TM RB's  

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_process_cipher_config_for_first_tm_setup
(
   uint32 mac_d_hfn
)
{

  if(tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] !=  TDSRRCSMC_SECURITY_STARTED)
  {
    return FAILURE;
  }
  
  tdsrrcsmc_backup_cipher_config_db();
  
  /*set cipher config type to HO_STEP_1*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_CIPHER_CFG;
      
  //update MAC cipher DB for ciph algo
    tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_algo = 
          tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID];
  
  //left shift by 4 to make the START value to HFN for TM RB's

  if(mac_d_hfn == TDSRRCSMC_INVALID_HFN_VALUE)
  {
    tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn = 
    (tdsrrcsmc_get_latest_transmitted_start_value_per_domain(RRC_CS_DOMAIN_CN_ID) << 4);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"mac d tdshfn presetn, updating cipher db,%d",mac_d_hfn);
    tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn =mac_d_hfn;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"First TM RB setup");

  return SUCCESS;
}/*tdsrrcsmc_process_cipher_config_for_hho*/

/*===========================================================================

FUNCTION tdsrrcsmc_process_cipher_config_for_hho

DESCRIPTION
  Process Step 1 ciphering for Handover

DEPENDENCIES
  None.

RETURN VALUE
  uecomdef_status_e_type SUCCESS if Ciphering is started on TM RB's
                         FAILURE if Ciphering is not started on TM RB's  

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_process_cipher_config_for_hho
(
   uint32 mac_d_hfn
)
{
  //if ciphering is not started for TM rb, HHO is being done in idle mode or
  //for PS RB.. hence do not add ciphering configuration..
  if(tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_enable != TRUE)
  {
    return FAILURE;
  }

  tdsrrcsmc_backup_cipher_config_db();
  
  /*set cipher config type to HO_STEP_1*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSHO_STEP1_CIPHER_CFG;
      
  //update MAC cipher DB for ciph algo
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].cipher_algo = 
    tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID];

  //left shift by 4 to make the START value to HFN for TM RB's

  if(mac_d_hfn == TDSRRCSMC_INVALID_HFN_VALUE)
  {
    tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn = 
    (tdsrrcsmc_get_latest_transmitted_start_value_per_domain(RRC_CS_DOMAIN_CN_ID) << 4);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"mac d tdshfn presetn, updating cipher db,%d",mac_d_hfn);
    tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn =mac_d_hfn;
  }
  
  /* Calculate and save START based on current MAX HFN so that it can be used at Step-2 */
  (void)tdsrrcsmc_update_start_value_to_transmit_cs_hho();

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"HHO Step1 HFN:0x%x",tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn);

  return SUCCESS;
}/*tdsrrcsmc_process_cipher_config_for_hho*/
/*===========================================================================

FUNCTION tdsrrcsmc_step_2_cipher_config_for_hho

DESCRIPTION
  Process Ciph Config Cnf for Hard Handover

DEPENDENCIES
  None.

RETURN VALUE
  Returns Count_C Activation Time

===========================================================================*/
uint32 tdsrrcsmc_step_2_cipher_config_for_hho
( 
void
)
{
  uint32 step_2_start, step_2_ciph_act_time, step_2_hfn;
  
  /*256 is the CFN rollover length*/
  /*250 is used to activate ciphering after 250 10ms time interval,
    this should give sufficient time for n/w side and ue side to have
    configured ciphering on the rb's successfully*/
  step_2_ciph_act_time = (uint8)((tdsseq_get_cfn() + 250) % 256);
  /*round it to 80ms boundary */
  step_2_ciph_act_time = (step_2_ciph_act_time/8)*8; 
  

  //set ul/dl rlc cipher info to 0's
  tdscipher_config_db.rlc_ul_cipher_info.nchan=0;
  tdscipher_config_db.rlc_dl_cipher_info.nchan=0;

  /*get the HFN for Step_2 HFN calculation*/
  step_2_start = tdsrrcsmc_get_latest_transmitted_start_value_per_domain(RRC_CS_DOMAIN_CN_ID);

  //convert start to count_i
  step_2_hfn = step_2_start << 4;

  /*set the HFN value for MAC to the step_2 HFN value.. also set the config
    type enum to STEP_2.. and also copy the ciph_act_time for mac to the new
    activation time sent in HO_TO_UTRAN_COMPLETE message
    we don't need to increment HFN+1 for MAC, as MAC layer takes care of it
    once we hit activation time*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSHO_STEP2_CIPHER_CFG;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn = 
    step_2_hfn;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].ciph_act_time = 
    step_2_ciph_act_time;
      
  /*call llc function to update rlc/mac ciphering configuration for
    starting STEP_2 ciphering*/
  tdsrrcllc_update_ciphering_config_info_for_handover();

  /*reset the cipher config type to non-HO*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_CIPHER_CFG;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ciph act time is %d HFN is 0x%x",step_2_ciph_act_time,step_2_hfn);
  
  return step_2_ciph_act_time;
}

/*===========================================================================

FUNCTION tdsrrcsmc_update_start_value_to_transmit_cs_hho

DESCRIPTION
  Updates tdsstart_value_to_transmit_cs_hho

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_update_start_value_to_transmit_cs_hho
(
void
)
{
  tdsstart_value_to_transmit_cs_hho = (tdsrrcsmc_get_max_start_per_domain(RRC_CS_DOMAIN_CN_ID));
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"updating SVTHHO:0x%x",tdsstart_value_to_transmit_cs_hho);
}

/*===========================================================================

FUNCTION tdsrrcsmc_reset_start_value_to_transmit_cs_hho

DESCRIPTION
  Reset tdsstart_value_to_transmit_cs_hho to 0

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcsmc_reset_start_value_to_transmit_cs_hho
(
boolean need_log
)
{ 
  if(need_log)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Resetting SVTHHO from 0x%x -> 0",tdsstart_value_to_transmit_cs_hho);
  }
  
  tdsstart_value_to_transmit_cs_hho = 0;
}


/*===========================================================================

FUNCTION rrcsmc_validate_and_save_srns_info_r7

DESCRIPTION
  This function saves Security Info received in SRNS relocation
  message for later use.


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type rrcsmc_validate_and_save_srns_info_r7
(
  tdsrrcsmc_srns_relocation_security_info_r7_type * srns_info,
  tdsrrc_msg_ver_enum_type msg_version
)
{

  tdsrrcsmc_saved_srns_info.ciph_algo_present     = FALSE;
  tdsrrcsmc_saved_srns_info.fresh_present         = FALSE;
  tdsrrcsmc_saved_srns_info.dpch_act_time_present = FALSE;    
  
  if(srns_info->integrity_info_present)
  {
    if(srns_info->integrity_protection_mode_info->integrityProtectionModeCommand.
       t != T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      return FAILURE;
    }

    tdsrrcsmc_saved_srns_info.fresh_present = TRUE;
    
    /*get the fresh value*/
    tdsrrcsmc_saved_srns_info.fresh = b_unpackd(
      srns_info->integrity_protection_mode_info->integrityProtectionModeCommand.u.
      startIntegrityProtection->integrityProtInitNumber.data,
      TDSRRC_DEFAULT_BIT_POS,
      TDSRRC_FRESH_LENGTH_IN_BITS);  
    
    if(TDSRRCRBR_R7_MSG_IE_PRESENT(srns_info->integrity_protection_mode_info,
      rrc_IntegrityProtectionModeInfo_r7_,integrityProtectionAlgorithm))
    {
      tdsrrcsmc_saved_srns_info.integrity_algo_present = TRUE;
      tdsrrcsmc_saved_srns_info.integrity_algo = tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &srns_info->integrity_protection_mode_info->integrityProtectionAlgorithm,
                TDSMSG_REL7);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Integrity Algo present in SRNS info,Algo=%d[0:Invalid Algo,1:UIA1,2:UIA2]",tdsrrcsmc_saved_srns_info.integrity_algo);
    }
  }
    
  if (srns_info->ciphering_mode_info_present)
  {
    if(tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind
        (&(srns_info->ciphering_mode_info->cipheringModeCommand.startRestart),
             msg_version)
        == tdsrrcsmc_saved_security_mode_command_msg.ciph_algo)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ciph Algo same. Invalid Config");
      return FAILURE;
    }
    else
    {
      tdsrrcsmc_saved_srns_info.ciph_algo_present = TRUE;
      tdsrrcsmc_saved_srns_info.ciph_algo = tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind
        (&(srns_info->ciphering_mode_info->cipheringModeCommand.startRestart),
             msg_version);
    }
    
    if(srns_info->ciphering_mode_info->m.activationTimeForDPCHPresent)
    {
      tdsrrcsmc_saved_srns_info.dpch_act_time_present = TRUE;
      tdsrrcsmc_saved_srns_info.dpch_act_time_present = 
        (uint8)srns_info->ciphering_mode_info->activationTimeForDPCH;
    }
  }
  return SUCCESS;
}
/*===========================================================================

FUNCTION tdsrrcsmc_validate_and_save_srns_info

DESCRIPTION
  This function saves Security Info received in SRNS relocation
  message for later use.


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_validate_and_save_srns_info
(
  tdsrrcsmc_srns_relocation_security_info_type * srns_info,
  tdsrrc_msg_ver_enum_type msg_version
)
{

  tdsrrcsmc_saved_srns_info.ciph_algo_present     = FALSE;
  tdsrrcsmc_saved_srns_info.fresh_present         = FALSE;
  tdsrrcsmc_saved_srns_info.dpch_act_time_present = FALSE;    
  
  if(srns_info->integrity_info_present)
  {
    if(srns_info->integrity_protection_mode_info->integrityProtectionModeCommand.
       t != T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      return FAILURE;
    }

    tdsrrcsmc_saved_srns_info.fresh_present = TRUE;
    
    /*get the fresh value*/
    tdsrrcsmc_saved_srns_info.fresh = b_unpackd(
      srns_info->integrity_protection_mode_info->integrityProtectionModeCommand.u.
      startIntegrityProtection->integrityProtInitNumber.data,
      TDSRRC_DEFAULT_BIT_POS,
      TDSRRC_FRESH_LENGTH_IN_BITS);  
    
    if(srns_info->integrity_protection_mode_info->m.integrityProtectionAlgorithmPresent)
    {
      tdsrrcsmc_saved_srns_info.integrity_algo_present = TRUE;
      tdsrrcsmc_saved_srns_info.integrity_algo = tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &srns_info->integrity_protection_mode_info->integrityProtectionAlgorithm,
                TDSMSG_REL99);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Integrity Algo present in SRNS info,Algo=%d[0:Invalid Algo,1:UIA1]",tdsrrcsmc_saved_srns_info.integrity_algo);
    }
  }
    
  if (srns_info->ciphering_mode_info_present)
  {
    if(tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind
        (&(srns_info->ciphering_mode_info->cipheringModeCommand.u.startRestart),
             msg_version)
        == tdsrrcsmc_saved_security_mode_command_msg.ciph_algo)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ciph Algo same. Invalid Config");
      return FAILURE;
    }
    else
    {
      tdsrrcsmc_saved_srns_info.ciph_algo_present = TRUE;
      tdsrrcsmc_saved_srns_info.ciph_algo = 
        tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind
        (&(srns_info->ciphering_mode_info->cipheringModeCommand.u.startRestart),
             msg_version);
    }
    
    if(srns_info->ciphering_mode_info->m.activationTimeForDPCHPresent)
    {
      tdsrrcsmc_saved_srns_info.dpch_act_time_present = TRUE;
      tdsrrcsmc_saved_srns_info.dpch_activation_time = 
        (uint8)srns_info->ciphering_mode_info->activationTimeForDPCH;
    }
  }
  return SUCCESS;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind

DESCRIPTION
  This function extracts ip check info/ integrity protection mode info and
  ciphering mode info from SRNS relocation triggering messages.


DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind
(
  tdsrrcsmc_srns_relocation_security_info_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{

  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                               /*Local variable to store message type */             

  /* Get the OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          
  
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {

    case tdsrrc_DL_DCCH_Message_PDU :

      msg_type = 
        tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      switch (msg_type)
      {
        case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        {
          tdsrrc_CellUpdateConfirm_r4_IEs *cucnf_ptr = 
            &(dcch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.cellUpdateConfirm_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r4_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r4_IEs,cipheringModeInfo))
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
          }
          }
          break;

        case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
        {
          tdsrrc_RadioBearerReconfiguration_r4_IEs *rbrreconfig_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.r4.radioBearerReconfiguration_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,integrityProtectionModeInfo))  
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rbrreconfig_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r4_IEs,cipheringModeInfo))  
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rbrreconfig_ptr->cipheringModeInfo);
          }
          }
          break;
    
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
        {
          tdsrrc_RadioBearerRelease_r4_IEs *rb_release_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.radioBearerRelease_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r4_IEs,integrityProtectionModeInfo))            
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r4_IEs,cipheringModeInfo))            
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rb_release_ptr->cipheringModeInfo);
          }
          }
          break;
    
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
        {
          tdsrrc_RadioBearerSetup_r4_IEs *rb_setup_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,integrityProtectionModeInfo))      
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r4_IEs,cipheringModeInfo))      
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rb_setup_ptr->cipheringModeInfo);
          }
          }
          break;
    
        case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
        {
          tdsrrc_TransportChannelReconfiguration_r4_IEs *tcreconfig_ptr =
            &(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.r4.transportChannelReconfiguration_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r4_IEs,integrityProtectionModeInfo))     
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(tcreconfig_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r4_IEs,cipheringModeInfo))     
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(tcreconfig_ptr->cipheringModeInfo);
          }
          }
          break;

        case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
        {
          tdsrrc_PhysicalChannelReconfiguration_r4_IEs *pc_reconfig_ptr = 
            &(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.r4.physicalChannelReconfiguration_r4);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r4_IEs,integrityProtectionModeInfo))               
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(pc_reconfig_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r4_IEs,cipheringModeInfo))               
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(pc_reconfig_ptr->cipheringModeInfo);
          }
          }
          break;      
      
        case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
        {
          
          tdsrrc_UTRANMobilityInformation_r3_IEs *utran_mob_info = 
            TDSRRC_EXTRACT_R3_MSG_POINTER(dcch_msg_ptr,utranMobilityInformation,utranMobilityInformation_r3);

          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(utran_mob_info,tdsrrc_UTRANMobilityInformation_r3_IEs,integrityProtectionModeInfo))               
            {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(utran_mob_info->integrityProtectionModeInfo);
          }
          if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(utran_mob_info,tdsrrc_UTRANMobilityInformation_r3_IEs,cipheringModeInfo))               
            {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(utran_mob_info->cipheringModeInfo);
          }
          }
          break;
        case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
          /* For uraUpdateConfirm, I.E integrityProtectionModeInfo is supported in R5/R7 messages only. */
          if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_r3.m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = 
              &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_r3.integrityProtectionModeInfo);
          }
          if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_r3.m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = 
              &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_r3.cipheringModeInfo);
          }
        break; 

        default:
          TDSRRC_MSG0(MSG_LEGACY_MED,"MSG not supported for SRNS-reloc");
          status = FAILURE;
          break;
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc");
      status = FAILURE;
      break;
  }
      
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);      
  }
  
  return status;
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r6

DESCRIPTION
  This function extracts ip check info/ integrity protection mode info and
  ciphering mode info from SRNS relocation triggering messages for Release 6 version
  of specs

  NOTE:  There is no R6 utranMobilityInformation message

DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r6
(
  tdsrrcsmc_srns_relocation_security_info_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                               /*Local variable to store message type */             
  
  /* Get the OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
  
  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          
  
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  { 
  case tdsrrc_DL_DCCH_Message_PDU :
    
    msg_type = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    switch (msg_type)
    {
    case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
    {
      tdsrrc_CellUpdateConfirm_r6_IEs *cucnf_ptr = 
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,cellUpdateConfirm,cellUpdateConfirm_r6);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r6_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r6_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
    {
      tdsrrc_RadioBearerReconfiguration_r6_IEs *rbrreconfig_ptr =  
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,radioBearerReconfiguration,radioBearerReconfiguration_r6);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,integrityProtectionModeInfo))      
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rbrreconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r6_IEs,cipheringModeInfo))      
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(rbrreconfig_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
    {
      tdsrrc_RadioBearerRelease_r6_IEs* rb_release_ptr =  
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,radioBearerRelease,radioBearerRelease_r6);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r6_IEs,integrityProtectionModeInfo))      
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r6_IEs,cipheringModeInfo))      
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(rb_release_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
    {
      tdsrrc_RadioBearerSetup_r6_IEs *rb_setup_ptr = 
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,radioBearerSetup,radioBearerSetup_r6);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r6_IEs,integrityProtectionModeInfo))               
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r6_IEs,cipheringModeInfo))               
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(rb_setup_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
    {
      tdsrrc_TransportChannelReconfiguration_r6_IEs *tcreconfig_ptr =
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,transportChannelReconfiguration,transportChannelReconfiguration_r6);
            
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r6_IEs,integrityProtectionModeInfo))               
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info =  &(tcreconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r6_IEs,cipheringModeInfo))               
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(tcreconfig_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
    {
      
      tdsrrc_PhysicalChannelReconfiguration_r6_IEs *pc_reconfig_ptr = 
        TDSRRC_EXTRACT_R6_MSG_POINTER(dcch_msg_ptr,physicalChannelReconfiguration,physicalChannelReconfiguration_r6);
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r6_IEs,integrityProtectionModeInfo))               
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(pc_reconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r6_IEs,cipheringModeInfo))               
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(pc_reconfig_ptr->cipheringModeInfo);
      }
      }
      break;      

    case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
      /* For uraUpdateConfirm, I.E integrityProtectionModeInfo is supported in R5/R7 messages only. */
      if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.r5.uraUpdateConfirm_r5.m.integrityProtectionModeInfoPresent)
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = 
          &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.
            r5.uraUpdateConfirm_r5.integrityProtectionModeInfo);
      }
      if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.r5.uraUpdateConfirm_r5.m.cipheringModeInfoPresent   )
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = 
          &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.
             r5.uraUpdateConfirm_r5.cipheringModeInfo);
      }
      break;      
            
    default:
      TDSRRC_MSG0(MSG_LEGACY_MED,"MSG not supported for SRNS-reloc");
      status = FAILURE;
      break;
      }
      break;
      
  default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc");
    status = FAILURE;
    break;
  }
  
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);      
  }
  
  return status;
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r5

DESCRIPTION
  This function extracts ip check info/ integrity protection mode info and
  ciphering mode info from SRNS relocation triggering messages for Release 5 version
  of specs


DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r5
(
  tdsrrcsmc_srns_relocation_security_info_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                               /*Local variable to store message type */             
  
  /* Get the OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
  
  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          
  
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {
    
  case tdsrrc_DL_DCCH_Message_PDU :
    
    msg_type = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
      cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    switch (msg_type)
    {
    case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
    {
      tdsrrc_CellUpdateConfirm_r5_IEs *cucnf_ptr = 
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,cellUpdateConfirm,cellUpdateConfirm_r5);
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r5_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cucnf_ptr,tdsrrc_CellUpdateConfirm_r5_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
    {
      tdsrrc_RadioBearerReconfiguration_r5_IEs *rbrreconfig_ptr = 
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,radioBearerReconfiguration,radioBearerReconfiguration_r5); 

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rbrreconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rbrreconfig_ptr,tdsrrc_RadioBearerReconfiguration_r5_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(rbrreconfig_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
    {
      tdsrrc_RadioBearerRelease_r5_IEs* rb_release_ptr = 
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,radioBearerRelease,radioBearerRelease_r5);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r5_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_release_ptr,tdsrrc_RadioBearerRelease_r5_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info =  &(rb_release_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
    {
      tdsrrc_RadioBearerSetup_r5_IEs *rb_setup_ptr = 
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,radioBearerSetup,radioBearerSetup_r5);
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rb_setup_ptr,tdsrrc_RadioBearerSetup_r5_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(rb_setup_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
    {
      tdsrrc_TransportChannelReconfiguration_r5_IEs *tcreconfig_ptr =
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,transportChannelReconfiguration,transportChannelReconfiguration_r5);
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r5_IEs,integrityProtectionModeInfo))
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(tcreconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r5_IEs,cipheringModeInfo))
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(tcreconfig_ptr->cipheringModeInfo);
      }
      }
      break;
      
    case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
    {
      tdsrrc_PhysicalChannelReconfiguration_r5_IEs *pc_reconfig_ptr =  
        TDSRRC_EXTRACT_R5_MSG_POINTER(dcch_msg_ptr,physicalChannelReconfiguration,physicalChannelReconfiguration_r5);

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r5_IEs,integrityProtectionModeInfo))      
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(pc_reconfig_ptr->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(pc_reconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r5_IEs,cipheringModeInfo))      
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(pc_reconfig_ptr->cipheringModeInfo);
      }
      }
      
      
      break;      
      
    case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
    {
      tdsrrc_UTRANMobilityInformation_r5_IEs *utran_mob_info = 
        TDSRRC_EXTRACT_R5_MSG_POINTER_UMI(dcch_msg_ptr,utranMobilityInformation,utranMobilityInformation_r5);
      
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(utran_mob_info,tdsrrc_UTRANMobilityInformation_r5_IEs,integrityProtectionModeInfo))      
      {
        srns_info->integrity_info_present = TRUE;
        srns_info->integrity_protection_mode_info = &(utran_mob_info->integrityProtectionModeInfo);
      }
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(utran_mob_info,tdsrrc_UTRANMobilityInformation_r5_IEs,cipheringModeInfo))      
      {
        srns_info->ciphering_mode_info_present = TRUE;
        srns_info->ciphering_mode_info = &(utran_mob_info->cipheringModeInfo);
      }
      }
      
      break;
      case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
        /* For uraUpdateConfirm, I.E integrityProtectionModeInfo is supported in R5/R7 messages only. */
        if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.r5.uraUpdateConfirm_r5.m.integrityProtectionModeInfoPresent)
        {
          srns_info->integrity_info_present = TRUE;
          srns_info->integrity_protection_mode_info = 
            &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.
               r5.uraUpdateConfirm_r5.integrityProtectionModeInfo);
        }
        if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.r5.uraUpdateConfirm_r5.m.cipheringModeInfoPresent)
        {
          srns_info->ciphering_mode_info_present = TRUE;
          srns_info->ciphering_mode_info = 
            &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.
              r5.uraUpdateConfirm_r5.cipheringModeInfo);
        }
      break;
      
    default:
      TDSRRC_MSG0(MSG_LEGACY_MED,"MSG not supported for SRNS-reloc");
      status = FAILURE;
      break;
      }
      break;
      
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc");
      status = FAILURE;
      break;
  }
  
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);      
  }
  
  return status;
}



/*===========================================================================

FUNCTION tdsrrcsmc_commit_fresh_for_srns_relocation

DESCRIPTION
  This function commits the FRESH value received in SRNS Relocation
  triggering message.
 
DEPENDENCIES
  None.

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_commit_fresh_for_srns_relocation
(
  void
)
{
  //check if fresh flag is present and commit the fresh values
  if(tdsrrcsmc_saved_srns_info.fresh_present == TRUE)
  {
    //commit the new fresh value received and set ip reconfig to true
    tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh);

    //save the comitted fresh value as old fresh value to be used in
    //the next connection setup
    tdsrrcsmc_saved_srns_info.fresh_old1 = tdsrrcsmc_saved_srns_info.fresh_old;    
    tdsrrcsmc_saved_srns_info.fresh_old = tdsrrcsmc_saved_srns_info.fresh;

#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
    tdsrrcsmc_srns_fresh_pending_state = TRUE; /*depends on fresh value present*/

  }
  tdsrrcsmc_srb3_4_rlc_pending_state = TRUE;  /*unconditional to the fresh present as this is for buffering DT data*/
#else
  }
#endif
}

/*===========================================================================

FUNCTION tdsrrcsmc_revert_back_to_old_fresh_for_srns_relocation

DESCRIPTION
  This function reverts back to old stored FRESH value which UE was using before
  receipt of SRNS relocation confiruation message(for which UE has to revert back
  to old configuration).
 
DEPENDENCIES
  None.

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_revert_back_to_old_fresh_for_srns_relocation
(
  void
)
{
  //check if fresh flag is present and commit the fresh values
  if(tdsrrcsmc_saved_srns_info.fresh_present == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Revert back to old FRESH value (SRNS Relocation scenario)");
    //commit the new fresh value received and set ip reconfig to true
    tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh_old1);

    //save the comitted fresh value as old fresh value to be used in
    //the next connection setup
    tdsrrcsmc_saved_srns_info.fresh_old = tdsrrcsmc_saved_srns_info.fresh_old1;

    tdsrrcsmc_saved_srns_info.fresh_present = FALSE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No need to revert back to old FRESH value");
  }

#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT

  tdsrrcsmc_send_rlc_re_est_cmp(); /*Pop possible buffered DDT to NAS*/
  /*set to FALSE unconditionally as this may be triggered by LLC revert procedure.*/
  tdsrrcsmc_srns_fresh_pending_state = FALSE;     
  tdsrrcsmc_srb3_4_rlc_pending_state = FALSE;     
#endif 

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  tdsrrcsmc_is_fresh_in_ota = FALSE;
#endif


}

/*===========================================================================

FUNCTION tdsrrcsmc_check_integrity_for_srns_relocation

DESCRIPTION
  This functions checks for integrity for SRNS relocation triggering
  messages.


DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if Integrity check was successful
  The function returns FAILURE if Integrity check was unsuccessful or config 
  sent was bad.

SIDE EFFECTS
  None

===========================================================================*/
tdsrrcsmc_srns_ip_check_status_e_type tdsrrcsmc_check_integrity_for_srns_relocation
(
  tdsrrc_cmd_type *cmd_ptr
  ,tdsrrc_msg_ver_enum_type msg_version
)
{
  tdsrrc_RB_Identity rb_id;
  tdsrrcsmc_srns_relocation_security_info_type srns_security_info;
  tdsrrcsmc_srns_relocation_security_info_r7_type srns_security_info_7;
  boolean integrity_info_present = FALSE;
    
  /*local copy of integrity_check_info variable*/  
  tdsrrcsmc_integrity_check_info_type int_check_info; 
  
  uint32 local_hfn = 0, local_seq_no = 0, local_count_i = 0, local_mac = 0;

  boolean is_dl_integrity_info_backed_up = FALSE;
  boolean is_dl_count_i_init_flag_changed = FALSE;

  memset(&srns_security_info,0,sizeof(tdsrrcsmc_srns_relocation_security_info_type));
  memset(&srns_security_info_7,0,sizeof(tdsrrcsmc_srns_relocation_security_info_r7_type));

  
#ifdef FEATURE_TDSCDMA_REL9
  if (msg_version == TDSMSG_REL9)
  { 
    if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r9(&(srns_security_info_7), cmd_ptr) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else
#endif /*FEATURE_TDSCDMA_REL9 */
#ifdef FEATURE_TDSCDMA_REL8
  if (msg_version == TDSMSG_REL8)
  { 
    if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r8(&(srns_security_info_7), cmd_ptr) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else
#endif /*FEATURE_TDSCDMA_REL8 */
  if (msg_version == TDSMSG_REL7)
  { 
    if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r7(&(srns_security_info_7), cmd_ptr) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else

  if (msg_version == TDSMSG_REL6)
  { 
    if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r6(&(srns_security_info), cmd_ptr) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else
  {
    if (msg_version == TDSMSG_REL5)
    {    
      if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r5(&(srns_security_info), cmd_ptr) != SUCCESS)
      {
        return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
      }
      // KW error
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
      }
    }
    else
    {
      if(tdsrrcsmc_get_srns_info_from_dl_sdu_ind(&(srns_security_info), cmd_ptr) != SUCCESS)
      {
        return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
      }
      // KW error
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
      }
    }
  }
  
  /*check if integrity check info and integrity protection mode info is 
    present, if it isn't then return invalid configuration*/
  if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
  || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/

  )
  {
    if((rrcsmc_validate_and_save_srns_info_r7(&srns_security_info_7,msg_version)) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else
  {
    if(tdsrrcsmc_validate_and_save_srns_info(&(srns_security_info),TDSMSG_REL99) != SUCCESS)
    {
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
 
  if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
  || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
  )
  {
    if((srns_security_info_7.ciphering_mode_info_present == TRUE || 
        srns_security_info_7.integrity_info_present == TRUE) &&
       (tdsrrcsmc_integrity_protection_info.reconfig == TRUE ||
        tdsrrcsmc_ciphering_info.reconfig == TRUE))
    {
      return TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  else
  {
    if((srns_security_info.ciphering_mode_info_present == TRUE || 
        srns_security_info.integrity_info_present == TRUE) &&
       (tdsrrcsmc_integrity_protection_info.reconfig == TRUE ||
        tdsrrcsmc_ciphering_info.reconfig == TRUE))
    {
      return TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION;  
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }
  }
  
  /*only if integrity info is present, we need to do IP check again and
    sync up to new FRESH value*/

  if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
  || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
  )
  {
    if(srns_security_info_7.integrity_info_present)
    {
      integrity_info_present = TRUE;
    }
  }
  else 
  {
  if(srns_security_info.integrity_info_present)  
  {
      integrity_info_present = TRUE;
    }
  }
  
  /*reset the flag for remembering the OTA triggered SRNS on Non-SRB2*/  
  tdsrrcsmc_non_srb2_srns_ota_pending_key_rb_id = 0xFF;    

  if(integrity_info_present)
  {
  
    rb_id =  tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);

    /*so this message should be integrity protected. check if IntegrityCheckInfo IE
     is present in the message, also Integrity Check Info contents should be copied
    into the local variable int_check_info*/
    if ( tdsrrcsmc_process_dl_integrity_check_info(
                                             cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                             cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                             &int_check_info,
                                             rb_id) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL Integrity Info failed");
      return TDSRRCSMC_SRNS_SECURITY_INFO_INVALID;
    }
    // KW error
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
    }

    /*check if we received modify, then also check if activation time is pending
      for this rb_id, if yes for both, then sync to new configuration immediately,
      no need to wait until activation time*/
    if ( (rb_id < TDSRRC_MAX_SRB) && ( tdsrrcsmc_integrity_protection_info.modify_received == TRUE) &&
         ( tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
           dl_count_i_act_time_pending[rb_id] == TRUE) )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR, "This is SRNS on non-SRB2,rb:%d",rb_id);
      /*back up the tdsrrcsmc_integrity_protection_info in case a wrongly encoded DL msg received during pending state*/
      is_dl_integrity_info_backed_up = TRUE;
      tdsrrcsmc_integrity_protection_info_dl_check_backup = tdsrrcsmc_integrity_protection_info;

      /*reset the activation time pending flag*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      dl_count_i_act_time_pending[rb_id] = FALSE;
      tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[rb_id] = FALSE;

      /*set the default init flag*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      dl_count_i_init_flag[rb_id] = FALSE;

      /*if new keys have been received for this domain, then set the HFN part of
          count_i to 0, and continue using tdsrrcllc_seq_num*/
      if (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_new_key_flag_per_rb[rb_id]==TRUE)
      {
        /*reset new keys flag*/
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        dl_new_key_flag_per_rb[rb_id]=FALSE;
      }

      /*generate count_i with HFN part set to 0 and old domain SEQ_NUM*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      dl_count_i[rb_id] = 
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].hfn,
          (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
           latest_config_cn_id_old].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK) );

      /*Do not Update latest used key before sucessful*/

    }
    if(rb_id < TDSRRC_MAX_SRB)
    {    
    
    /*copy hfn to a local variable for further processing*/
      local_hfn= tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                 dl_count_i[rb_id];
  
      /*this is not the first message on this rb_id, get the sequence num
        from the stored count_i*/
      local_seq_no = (local_hfn & TDSRRC_SEQ_NUM_BIT_MASK);
  
      /*if the rcvd sequence number is the same as the previous message sequence
        number stored, then do not process this message further.  just return*/
      if (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_count_i_init_flag[rb_id] == TRUE)
      {
      if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
      || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
        )
        {
          if (local_seq_no == srns_security_info_7.integrity_check_info->rrc_MessageSequenceNumber)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Duplicate SN %d for RB_ID %d",local_seq_no,rb_id);
            return TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
          }
        }
        else
        {
          if (local_seq_no == srns_security_info.integrity_check_info->rrc_MessageSequenceNumber)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Duplicate SN %d for RB_ID %d",local_seq_no,rb_id);
            return TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
          }
        }
      }
      else
      {
        /*flag for the revert in case the DL IP check failed*/
        is_dl_count_i_init_flag_changed = TRUE;

        /*set the init_flag to TRUE*/  
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_count_i_init_flag[rb_id] = TRUE;
      }
    }

    /*update fresh value temporarily to calculate MAC-I for DL msg*/
    tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh);
    
    /*if the stored seq number is greater than the one received in the msg
      then a rollover has occured.  Increment the hfn part of the number
      only*/
    if (local_seq_no > int_check_info.msg_seq_num)
    {
      /*increment hfn only*/
      local_hfn >>=4;
      local_hfn++;
      local_hfn <<=4;
    }

    /*generate the new count_i with the received sequence number and the hfn*/
    if((msg_version == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
    || (msg_version == TDSMSG_REL8)
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    || (msg_version == TDSMSG_REL9)
#endif /*FEATURE_TDSCDMA_REL9*/
    )
    {
      local_count_i = 
      (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
               (srns_security_info_7.integrity_check_info->rrc_MessageSequenceNumber & 
               TDSRRC_SEQ_NUM_BIT_MASK));
    }
    else
    {
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (srns_security_info.integrity_check_info->rrc_MessageSequenceNumber & 
             TDSRRC_SEQ_NUM_BIT_MASK));
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",tdsrrcsmc_int_ciph_info.latest_config_cn_id,tdsrrcsmc_saved_srns_info.integrity_algo);

    /*call the f9 engine to generate MAC for the dl message*/
    local_mac = tdsrrcf9_generate_mac_for_message(
                  local_count_i,
                  TDSRRC_INTEGRITY_DIR_DOWNLINK,
                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                  (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8),
                  tdsrrcsmc_int_ciph_info.latest_config_cn_id,
                  tdsrrcsmc_saved_srns_info.integrity_algo
                  );

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"maci gen = 0x%x int_check_info.msg_auth_code = 0x%x",local_mac,int_check_info.msg_auth_code);

    /*Restore the FRESH value to previously stored value*/
    tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh_old);

    /*if the MAC's match then we have successfully checked the integrity of
      this message.  update the count_i for this rb_id*/
    if (local_mac == int_check_info.msg_auth_code)
    {
      /*update count_i stored in tdsrrcsmc_integrity_protection_info global var*/
      // KW error
      if (rb_id < TDSRRC_MAX_SRB)
      {
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_count_i[rb_id] = local_count_i;
      }

     
     /*rb_id = 2 does not update here, should be at SMC*/
         

      if((rb_id != DCCH_AM_RADIO_BEARER_ID)&&(is_dl_integrity_info_backed_up))
      {

        /*should update tdsrrcsmc_latest_used_ip_key by using last commit key on this domain after re-est */
        /*
         memscpy(tdsrrcsmc_latest_used_ip_key[rb_id], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               tdsrrcsmc_last_committed_ip_key[tdsrrcsmc_int_ciph_info.latest_config_cn_id], 
                RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));*/

        /*remember the rb_id and update latest used after re-est*/
         tdsrrcsmc_non_srb2_srns_ota_pending_key_rb_id = rb_id;
         TDSRRC_MSG1(MSG_LEGACY_ERROR, "This is rare case! SRNS on non-SRB2,rb:%d pending IK need to be current after SRNS",rb_id);
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS-DL IP check successful");
      return TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS;
    }    
    else
    {
      TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							     TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							     TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS-DL IP check fail");  


      /*Revert the tdsrrcsmc_integrity_protection_info that is changed during pending state*/
      if(is_dl_integrity_info_backed_up)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Revert the tdsrrcsmc_integrity_protection_info that is changed during pending state");
        tdsrrcsmc_integrity_protection_info = tdsrrcsmc_integrity_protection_info_dl_check_backup;
      }
      else if(is_dl_count_i_init_flag_changed)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Revert the dl_count_i_init_flag only. rb_id:%d,SN: 0x%X",rb_id,local_seq_no);
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].dl_count_i_init_flag[rb_id] = FALSE; 
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No revert for tdsrrcsmc_integrity_protection_info is needed");
      }

      return TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE;
    }
  }
  
  return TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS;  
}
/*===========================================================================

FUNCTION tdsrrcsmc_check_downlink_integrity

DESCRIPTION
  This function checks the Integrity of the DL message received and returns 
  the status.  It receives the pointer to the Command and runs Integrity Check 
  on the message.
  The dispatcher procedure will call this function.


DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if DL Integrity check was successful
  The function returns FAILURE if DL Integrity check was unsuccessful

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_check_downlink_integrity 
(
tdsrrc_cmd_type *cmd_ptr 
)
{
  boolean is_dl_act_time_expired = FALSE;
  boolean is_dl_integrity_info_backed_up = FALSE;
  boolean is_dl_count_i_init_flag_changed = FALSE;

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  /*Reset the flag in case next CUConfirm is on CCCH*/
  tdsrrcsmc_is_fresh_in_ota = FALSE;
#endif
  if (tdsnv_integrity_enabled)
  {

    tdsrrc_RB_Identity rb_id;           /*local var to store rb_id*/
    uint8 msg_type;                  /*Local variable to store message type */             

    /*local copy of integrity_check_info variable*/  
    tdsrrcsmc_integrity_check_info_type int_check_info; 

    uint32 local_hfn = 0, local_seq_no = 0, local_count_i, local_mac;              
    /*local var to store hfn, count_i and
      sequence number*/

    rrc_cn_domain_identity_e_type  local_cn_domain;
    boolean                        using_old_keys_flag=FALSE;
    boolean                        using_old_config_flag=FALSE;
    boolean                        use_latest_config_cn_id_old_1_keys_flag = FALSE;

    /*check if integrity protection is started or not*/
    if (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_NOT_STARTED)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Integrity Protection Not Started");
      return SUCCESS;
    }

    /*check to see if the messages will be integrity protected.  SIBS, MIBS,
    PCCH messages and most CCCH messages are not integrity protected. Just
    return success if one of these messages.
    Also if Integrity is already started and we receive a SECURITY MODE COMMAND
    second time(with MODIFY) then return SUCCESS.  We will check IP for SMC later*/
/*lint -e527*/    
    switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
    {
      case tdsrrc_BCCH_FACH_Message_PDU:
      case tdsrrc_BCCH_BCH_Message_PDU:
      case tdsrrc_PCCH_Message_PDU:    
        return SUCCESS;

      case tdsrrc_DL_CCCH_Message_PDU:

        msg_type = 
        tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
        switch (msg_type )
        {
          /*Integrity Protection not reqd. for the foll. messages. Return success
            without doing any processing*/
          case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
          case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:
          case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
            return SUCCESS;

          default:
            TDSRRC_MSG0(MSG_LEGACY_MED,"Integrity Check Info Needed");
            break;
        }
        break;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      case TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring DL IP check for TFCC TM procedure");
          return SUCCESS;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      case tdsrrc_DL_DCCH_Message_PDU :
        msg_type = 
        tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
        switch (msg_type)
        {
          /*Integrity Protection not checked here for SMC cmd. Will be checked later.
            Return success without doing any processing*/
          case T_tdsrrc_DL_DCCH_MessageType_securityModeCommand:
            return SUCCESS;

          /* If NAS messages are not integrity protected,do not perform IP check on them
             NAS will decide whether this message needs to be honoured or not.This is an
             optimization to counter a race condition on the network side where Authentication  
             and SMC processes are going on parallely for 2 domains in a MultiRAB call
           */  
          case T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer:
            if (0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                        TDSINTEGRITY_CHECK_INFO_PRESENT_POS,
                        TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
            {
              /* message not integrity protected */
              /* Check whether message contains 'r3' IE or not */
              if (0x00 == b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                  TDSDL_DCCH_R3_INDICATOR_WITHOUT_INT_CHECK_INFO_POS,
                  TDSNUM_OF_BITS_FOR_DL_DCCH_R3_INDICATOR))
              { /* r3 is selected */
                rrc_cn_domain_identity_e_type ddt_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
                /* store the CN domain received in message in a temp variable
                   0 --> CS Domain, 1 --> PS Domain */
		            ddt_cn_domain_id = ((0x00 != b_unpackb(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                      TDSDL_DCCH_R3_DDT_CN_DOMAIN_IE_POS,
                      TDSNUM_OF_BITS_FOR_DL_DCCH_R3_DDT_CN_DOMAIN_IE))
                      ? RRC_PS_DOMAIN_CN_ID : RRC_CS_DOMAIN_CN_ID);

                TDSRRC_MSG1(MSG_LEGACY_HIGH,"R3 DDT received with cnDomain: %d (0:CS,1:PS)",ddt_cn_domain_id);

                /* if SMC was never received for this domain then return success */
                if (FALSE == tdsrrcsmc_int_ciph_info.int_start_status_for_conn[ddt_cn_domain_id])
		            {
		              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Returning success to IP Check to forward msg to NAS as SMC is not recieved for cnDomain: %d",ddt_cn_domain_id);
                  return(SUCCESS);
                }
              }
             /* From 24.008:
                If any layer 3 signalling message is received, in either PS or CS domains, 
                as not integrity protected even though the integrity protection has been 
                activated in the MS by that domain in the network, then the lower layers 
                shall discard this message (see 3GPP TS 25.331). */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring NOT integrity protected DDT msg: either r3 not selected or SMC already recieved for cnDomain in DDT");
              return (FAILURE);
            }
            break;

          case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
          case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
          case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
          case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
          case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
          case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
          case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
          case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:

            /* Check whether Integrity Protection Mode  info is present for the messages
               that can carry SRNS relocation info.. if they are present, the don't check
               integrity for the message */

            if (TRUE == 
                         tdsrrcsmc_is_integrity_prot_mode_info_present(
                                             cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, msg_type))
            {
               /* Call the ASN.1 decoder to decode the message. */
              cmd_ptr->cmd.downlink_sdu_ind.decoded_msg =
                    tdsrrcasn1_decode_pdu(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,(int )
                                       cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,
                                       cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length
                          );

              if (NULL != cmd_ptr->cmd.downlink_sdu_ind.decoded_msg)
              {
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
                tdsrrcsmc_is_fresh_in_ota = TRUE;
#endif
                /* if it reaches here then it means that its a SRNS relocation scenario
                   and needs to be integrity checked later */
                /* No need to release the memory allocated to 'decode_msg' because it will be freed
                   at the end while returning control to dispatcher. */
                /* Special care required (check for 'decode_msg' not equal to NULL while decoding it
                   later. */
                if ((TDSOC_NOT_SET == tdsrrcllc_get_ordered_config_status()) &&
                    (FALSE == tdsrrcsmc_is_security_reconfig_active()))
                {
                  /* Take back-up of security variables only if security as well as reconfig 
                     is not in progress because this message will result in failure and backup would 
                     have already been taken */
                  tdsrrcsmc_backup_global_security_variables();
                }

                return SUCCESS;
              }
              else
              {
                /* if 'decoded_msg' is NULL then it means its a ASN1 decode failure, so go ahead and
                  dont consider it a SRNS relocation scenario (this condition can happen because of
                  ciphering issue....integrity check for this message is expected to fail. */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"ASN1 decode failure. Probable garbled message. Integrity check is expected to fail");
              }
            }
            else
            {
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
              tdsrrcsmc_is_fresh_in_ota = FALSE;
#endif
                /* save the security variables when its a configuration message, 
                   back needs to be restored while reverting back to old configuration */
              if ((TDSOC_NOT_SET == tdsrrcllc_get_ordered_config_status()) &&
                  (FALSE == tdsrrcsmc_is_security_reconfig_active()))
              {
                /* NOTE: we are not taking back up of security variables when config message is received
                   in emergency call + security is not enabled. Since it looks like taking a backup will result
                   in some code duplication and unnecessary exectution of that code everytime when
                   message is not integrity protected. Since its very rare scenario <need to check?>, so 
                   we can avoid taking back up of security variables for this scenario. */
                tdsrrcsmc_backup_global_security_variables();
              }
            }
            break;

          default:
            break;
        }
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_MED,"Integrity Check Info Needed");
        return FAILURE;
    } 
/*lint +e527*/    

    /*get the rb_id from lc_id*/
    rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);


    /*so this message should be integrity protected. check if IntegrityCheckInfo IE
     is present in the message, also Integrity Check Info contents should be copied
     into the local variable int_check_info*/
    if ( tdsrrcsmc_process_dl_integrity_check_info(
                                               cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                               cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                               &int_check_info,
                                               rb_id) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cant do integrity check as either integrity check info is missing or msg len is invalid");
      return FAILURE;
    }


    local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id;

    /*check if we received modify, then also check if activation time is pending
      for this rb_id, if yes for both*/
  if(rb_id < TDSRRC_MAX_SRB)
  {
    if ( ( tdsrrcsmc_integrity_protection_info.modify_received == TRUE) &&
         ( tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
           dl_count_i_act_time_pending[rb_id] == TRUE) )
    {
      is_dl_act_time_expired = TDSRRC_CHECK_FOR_IP_ACT_TIME_EXPIRY(
                (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                latest_config_cn_id_old].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK),
                       tdsrrcsmc_integrity_protection_info.dl_act_seq_num[rb_id], 
                       int_check_info.msg_seq_num);

      /*check to see if activation time has been reached, this is checked whether act time
        matches or if SN > Act-time by a value of 8 or less(half the SN range)*/
      if (TRUE == is_dl_act_time_expired)
      {
        /*back up the tdsrrcsmc_integrity_protection_info in case a wrongly encoded DL msg received during pending state*/
        is_dl_integrity_info_backed_up = TRUE;
        tdsrrcsmc_integrity_protection_info_dl_check_backup = tdsrrcsmc_integrity_protection_info;

        /*reset the activation time pending flag*/
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        dl_count_i_act_time_pending[rb_id] = FALSE;

        tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[rb_id] = FALSE;

        /*set the default init flag*/
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        dl_count_i_init_flag[rb_id] = FALSE;

        /*if new keys have been received for this domain, then set the HFN part of
            count_i to 0, and continue using tdsrrcllc_seq_num*/
        /* check if new keys has already been processed because of receipt of SMC on SRB2 or not, 
           if it is TRUE then it means that SMC is not received for last Authentication Request, 
           hence no need to touch new key flag for UL and
           also check whether new keys have been received and not activated for this SRB */
        if ((FALSE == tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_new_key_flag_per_rb[DCCH_AM_RADIO_BEARER_ID]) &&
          (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            dl_new_key_flag_per_rb[rb_id]==TRUE))
        {
          /*reset new keys flag*/
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_new_key_flag_per_rb[rb_id]=FALSE;
        }
        
        /*generate count_i with HFN part set to 0 and old domain SEQ_NUM                  *
         *Getting from old domain to make sure the SN is shared                           *
         *Sync between old and old_1 has been taken care at the last part of this function * 
         */
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        dl_count_i[rb_id] = 
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].hfn,
            (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
             latest_config_cn_id_old].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK) );

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"dl_act_time_expired, rb_id: %d, dl_count_i 0x%x", rb_id, 
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
              dl_count_i[rb_id], 0);

        /*Do not update the tdsrrcsmc_latest_used_ip_key here, wait until IP pass*/

      }
      else
      {
        using_old_config_flag = TRUE;
        /*if activation time has not been reached for this rb_id, continue using old
            cn domain */
        if (FALSE == tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[rb_id])
        {
          local_cn_domain     = tdsrrcsmc_int_ciph_info.latest_config_cn_id_old;
          /* if old and new cn domain are the same, say due to authentication foll. by SMC,
             then we need to use old keys stored in backup.. else don't bother switching
             between old and new keys, because for a different domain, after auth, latest
             keys will be in current db and hence no need to set this flag*/
          if(local_cn_domain == tdsrrcsmc_int_ciph_info.latest_config_cn_id)
          {
            using_old_keys_flag = TRUE;
	          use_latest_config_cn_id_old_1_keys_flag = FALSE;

            /* For non-SRB2 SN jumping in SMC "modify" in current domain.              *
             * In this case, we need keep using current key, instead of revert back to *
             * old key for this domain.                                                *   
             * This is for the scenario:                                               *   
             *         New IP key has been Authed                                      *   
             *         The new IP key has been applied for non-SRB2, e.g. SRB3/4       *   
             *         While, a new SMC with modify move the activation SN at least    * 
             *         two ahead of current SNc, marked as SNa                         * 
             *         We need to apply current IP check for SNc < SN < SNa            */
  
             /* The logic is: if the new KEY has already been used for this SRB, then
                keep using it until next ACT time. We don't need to revert back.       */
            if(tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                      dl_new_key_flag_per_rb[rb_id] == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SN for non-SRB2 jumping in SMC-modify for current domain,use current IP ");
              using_old_keys_flag = FALSE;
            }           
          }
          /*If a modify received, and old domain has a pending new key*
           *Using old domain old key in this case */
          else if(tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].dl_new_key_flag_per_rb[rb_id] == TRUE)
          {
            using_old_keys_flag = TRUE;
            use_latest_config_cn_id_old_1_keys_flag = FALSE;

          }
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"using_old_config_flag = TRUE, local domain=%d,using_old_keys_flag=%d",local_cn_domain,using_old_keys_flag);

        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Using latest_config_cn_id_old_1 and old1-IK-key: %d[0:CS, 1:PS],rb: %d",tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id],rb_id);
          local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id];
	        use_latest_config_cn_id_old_1_keys_flag = TRUE;
        }      
      }
    }

    /*copy hfn to a local variable for further processing*/
    local_hfn= tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
               dl_count_i[rb_id];

    /*this is not the first message on this rb_id, get the sequence num
      from the stored count_i*/
    local_seq_no = (local_hfn & TDSRRC_SEQ_NUM_BIT_MASK);

    /*if this is the first time we are receiving a message on this SRB then
      generate count_i with the stored hfn and the received sequence number*/
    if (tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
        dl_count_i_init_flag[rb_id] == FALSE)
    {
      /*flag for the revert in case the DL IP check failed*/
      is_dl_count_i_init_flag_changed = TRUE;
      /*set the init_flag to TRUE*/  
      tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
      dl_count_i_init_flag[rb_id] = TRUE;

      /*Due to SRNS will revert back local_seq_no for active the pending Algorithm*/
      /*Should not do the sync as the dl_count_i_init_flag is per domain while the SN is shared*/
      /*local_seq_no = int_check_info.msg_seq_num;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"First msg on RB: %d,and set the local_seq_no=0x%X",rb_id,local_seq_no); */
    }
    else
    {
      /*if the rcvd sequence number is the same as the previous message sequence
        number stored, then do not process this message further.  just return*/
      if (local_seq_no == int_check_info.msg_seq_num)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Duplicate SN %d for RB_ID %d",local_seq_no,rb_id);
        return FAILURE;
      }
    }
  }

    /*if the stored seq number is greater than the one received in the msg
      then a rollover has occured.  Increment the hfn part of the number
      only.
      HFN should NOT be incremented if 'is_dl_act_time_expired' is set to
      TRUE, AND activation time is NOT equal to zero AND activation time is 
      less than current RRC-SN
      */

    /* the above second logic check is removed in latest WCDMA code. 03/15/12*/
    if (local_seq_no > int_check_info.msg_seq_num)
    {
      /*increment hfn only*/
      local_hfn >>=4;
      local_hfn++;
      local_hfn <<=4;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Local SN %d MSN %d HFN after rollover 0x%x",local_seq_no, int_check_info.msg_seq_num,local_hfn);			
    }

    /*generate the new count_i with the received sequence number and the hfn*/
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (int_check_info.msg_seq_num & TDSRRC_SEQ_NUM_BIT_MASK) 
             );

    /*set the keys to old*/
#if 0
    if (using_old_keys_flag == TRUE)
    {
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
    }
    else if ((rb_id < TDSRRC_MAX_SRB) && (TRUE == use_latest_config_cn_id_old_1_keys_flag))
    {
      
      /* write integrity keys associated with cn_id_old_1 in both the domains (to avoid unnecessary checks) */
      tdsrrcf9_init_key_values(
        tdsrrcsmc_integrity_protection_info.integrity_key_for_cn_id_old_1[rb_id],
        tdsrrcsmc_integrity_protection_info.integrity_key_for_cn_id_old_1[rb_id]);
    }

#endif

    if((rb_id < TDSRRC_MAX_SRB)&&((using_old_keys_flag == TRUE)||(TRUE == use_latest_config_cn_id_old_1_keys_flag)))
    {
      /*unify the old and old_1 usage to tdsrrcsmc_latest_used_ip_key*/
      tdsrrcf9_init_key_values(
         tdsrrcsmc_latest_used_ip_key[rb_id],
         tdsrrcsmc_latest_used_ip_key[rb_id]);
 
    }

    /*following is a protection for wrongly revert due to failed SMC 
      and should not make difference if SMC handles correctly.
    */
/*
    else if(tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].dl_new_key_flag_per_rb[rb_id] == FALSE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"No activation time pending for NEW Key for domain %d:", );
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID];
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID];
      );

    } 
*/

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",local_cn_domain,tdsrrcsmc_int_ciph_info.saved_integrity_algo[local_cn_domain]);

    /*call the f9 engine to generate MAC for the dl message*/
    local_mac = tdsrrcf9_generate_mac_for_message(
                  local_count_i,
                  TDSRRC_INTEGRITY_DIR_DOWNLINK,
                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                  (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8),
                  local_cn_domain,
                  tdsrrcsmc_int_ciph_info.saved_integrity_algo[local_cn_domain]
                  );

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"maci gen = 0x%x int_check_info.msg_auth_code = 0x%x",local_mac,int_check_info.msg_auth_code);


    /*set the keys back to new*/
    if ((using_old_keys_flag == TRUE) || (TRUE == use_latest_config_cn_id_old_1_keys_flag))
    {
      /*
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
        */
      /*revert to last committed one*/
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
    }
    
    /*additional check for fault-tolerance*/
#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT 

    if((tdsrrcsmc_srns_fresh_pending_state)&& (local_mac != int_check_info.msg_auth_code))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Fault-tolerance for DL msgs during SRNS relocation by using old FRESH");
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Fresh: 0x%x, Old Fresh: 0x%x, Old_1 Fresh: 0x%x", tdsrrcsmc_saved_srns_info.fresh, tdsrrcsmc_saved_srns_info.fresh_old, tdsrrcsmc_saved_srns_info.fresh_old1);

      tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh_old1);
      local_mac = tdsrrcf9_generate_mac_for_message(
                  local_count_i,
                  TDSRRC_INTEGRITY_DIR_DOWNLINK,
                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                  (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length * 8),
                  local_cn_domain,
                  tdsrrcsmc_int_ciph_info.saved_integrity_algo[local_cn_domain]
                  );

     TDSRRC_MSG2(MSG_LEGACY_HIGH,"maci gen = 0x%x int_check_info.msg_auth_code = 0x%x",local_mac,int_check_info.msg_auth_code);
     tdsrrcf9_init_fresh_per_connection(tdsrrcsmc_saved_srns_info.fresh);

    }

#endif

    /*if the MAC's match then we have successfully checked the integrity of
      this message.  update the count_i for this rb_id*/
    if (local_mac == int_check_info.msg_auth_code)
    {
      if(rb_id < TDSRRC_MAX_SRB)
      {
        /*update count_i stored in tdsrrcsmc_integrity_protection_info global var*/
        tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
        dl_count_i[rb_id] = local_count_i;
  
        /*since we r still using old domain.. update sn of the new domain too
          as SN is common to both domains.. only HFN part is different and will
          be taken care of when activation time is reached

         Made changes to store the received MSN instead of the local_seq_no
         in dl count-i because local_seq_no is not re-initialized incase of 
         rollover */        

        if(using_old_config_flag == TRUE)
        {
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            dl_count_i[rb_id] =
            ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
             dl_count_i[rb_id] & TDSRRC_HFN_BIT_MASK) |
             (int_check_info.msg_seq_num & TDSRRC_SEQ_NUM_BIT_MASK));

          /*HX: Also need consider old_1 domain != old domain, for the SN warp around case*/
          if(use_latest_config_cn_id_old_1_keys_flag)
          {
            if(tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id] != tdsrrcsmc_int_ciph_info.latest_config_cn_id_old)
            {
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                dl_count_i[rb_id] =
                ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                 dl_count_i[rb_id] & TDSRRC_HFN_BIT_MASK) |
                 (int_check_info.msg_seq_num & TDSRRC_SEQ_NUM_BIT_MASK));
            }
          }
          else
          {
            if(tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id] != tdsrrcsmc_int_ciph_info.latest_config_cn_id_old)
            {
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id]].
                dl_count_i[rb_id] =
                ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[rb_id]].
                 dl_count_i[rb_id] & TDSRRC_HFN_BIT_MASK) |
                 (int_check_info.msg_seq_num & TDSRRC_SEQ_NUM_BIT_MASK));
            }
          }
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Sync SN for SRB: %d, Count-I[CS]=0x%X, Count-I[PS]=0x%X", rb_id,
                             tdsrrcsmc_integrity_protection_info.count_i_info[RRC_CS_DOMAIN_CN_ID].dl_count_i[rb_id],
                             tdsrrcsmc_integrity_protection_info.count_i_info[RRC_PS_DOMAIN_CN_ID].dl_count_i[rb_id]); 
        }

        /*update tdsrrcsmc_latest_used_ip_key by using last commit key on this domain */
        /*rb_id = 2 does not update here, should be at SMC*/
        if(is_dl_integrity_info_backed_up)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"update tdsrrcsmc_latest_used_ip_key for rb_id=%d,by using domain:%d last committed key",rb_id,tdsrrcsmc_int_ciph_info.latest_config_cn_id);
          memscpy(tdsrrcsmc_latest_used_ip_key[rb_id], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
                 tdsrrcsmc_last_committed_ip_key[tdsrrcsmc_int_ciph_info.latest_config_cn_id], 
                 RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
        }

  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL IP check successful");
     } 
      return SUCCESS;
    }
    else
    {
      TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							     TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							     TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"DL IP check failed. FRESH used: 0x%x, COUNT-I used: 0x%x, old_keys_used: %d", 
        tdsrrcsmc_saved_srns_info.fresh_old, local_count_i, using_old_keys_flag);
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"use_latest_config_cn_id_old_1_keys_flag: %d",use_latest_config_cn_id_old_1_keys_flag);

      /*Print out all the debugging informations*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS Current Key:");
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[0]);


      TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS Old Key:");
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[0]);


      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PS Current Key:");
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].ciph_keys_info.key[0]);


      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PS Old Key:");
      tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old,
                                   tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].ciph_keys_info.key[0]);

      //This code is to satify KW error only.
      if (rb_id < TDSRRC_MAX_SRB)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS/PS latest_used_ip_key:");
        tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                     tdsrrcsmc_latest_used_ip_key[rb_id],
                                     tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[0]);
/*
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS/PS Old_1 Key:");
        tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                     tdsrrcsmc_integrity_protection_info.integrity_key_for_cn_id_old_1[rb_id],
                                     tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[0]);*/
      }
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Fresh: 0x%x, Old Fresh: 0x%x, Old_1 Fresh: 0x%x", tdsrrcsmc_saved_srns_info.fresh, tdsrrcsmc_saved_srns_info.fresh_old, tdsrrcsmc_saved_srns_info.fresh_old1);


      /*Revert the tdsrrcsmc_integrity_protection_info that is changed during pending state*/
      if(is_dl_integrity_info_backed_up)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Revert the tdsrrcsmc_integrity_protection_info that is changed during pending state");
        tdsrrcsmc_integrity_protection_info = tdsrrcsmc_integrity_protection_info_dl_check_backup;
      }
      else if(is_dl_count_i_init_flag_changed)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Revert the dl_count_i_init_flag only. rb_id:%d,SN: 0x%X",rb_id,local_seq_no);
        tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].dl_count_i_init_flag[rb_id] = FALSE; 
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No revert for tdsrrcsmc_integrity_protection_info is needed");
      }

      return FAILURE;
    }
  }
  else
  {
    return(SUCCESS);
  }
}/*tdsrrcsmc_check_downlink_integrity*/
/*===========================================================================

FUNCTION tdsrrcsmc_append_mac_seq_no_to_ul_sdu

DESCRIPTION
  RRCSMC receives MAC and seq_no to be appended to UL SDU, appends the same
  to the UL SDU 

DEPENDENCIES
  None.

RETURN VALUE
  None. 

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcsmc_append_mac_seq_no_to_ul_sdu
(
byte* ul_sdu, 
uint32 mac, 
uint32 seq_num
)
{
  uint32 sn_type_pos;                     /* Local Variable to store seq num
                                             type position*/                                 
  uint32 mac_type_pos;                    /* Local Variable to store mac 
                                             type position*/                                 

  /*peek into byte stream pack the MAC in appropriate position*/
  mac_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS + 
                 TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

  b_packd(mac, ul_sdu, (uint16)mac_type_pos, TDSRRC_MAC_LENGTH_IN_BITS);

  /*peek into byte stream and pack seq num in appropriate position*/
  sn_type_pos =  TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                 TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT+
                 TDSRRC_MAC_LENGTH_IN_BITS;

  b_packb((uint8)seq_num, ul_sdu, (uint16)sn_type_pos, TDSRRC_SEQ_NUM_LENGTH_IN_BITS);

}/*tdsrrcsmc_append_mac_seq_no_to_ul_sdu*/
/*===========================================================================

FUNCTION tdsrrcsmc_append_uplink_integrity_check_info

DESCRIPTION
  RRCSMC receives the encoded uplink SDU and adds Integrity Check Info to
  the UPLINK SDU.
  The send chain will call this function in SMC before sending a 
  message on a UL Signaling Radio Bearer to append the Integrity Check
  Info in the message.

DEPENDENCIES
  None.

RETURN VALUE
  This function returns SUCCESS if IntegrityCheckInfo is appended successfully
  This function returns FAILURE if if it is failed to append IntegrityCheckInfo .


SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_uplink_integrity_check_info 
(
byte*  ul_sdu,
uint32 ul_sdu_length,
tdsrrc_RB_Identity rb_id
)
{
  uint32 local_seq_num;              /*local vars for sdu length and seq num*/
  uint32 local_mac;                  /*local var to store mac*/
  rrc_cn_domain_identity_e_type  local_cn_domain;
  boolean                        using_old_keys_flag=FALSE;

  /*initialize local var it to latest cn domain*/
  local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id;

  //This code is to satify KW error only.
  if (rb_id >= TDSRRC_MAX_SRB)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: rb_id is %d,out of bound",rb_id);
    return FAILURE;
  }

  /*if we have received a modify command and activation time is pending*/
  if ( ( tdsrrcsmc_integrity_protection_info.modify_received == TRUE) &&
       ( tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
         ul_count_i_act_time_pending[rb_id] == TRUE) )
  {
    /*check if next SEQ_NUM is equal to pending activation time*/
    if (((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
          ul_count_i[rb_id]+1) & TDSRRC_SEQ_NUM_BIT_MASK ) == 
        tdsrrcsmc_integrity_protection_info.ul_act_seq_num[rb_id])
    {
      /*reset pending flag*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i_act_time_pending[rb_id] = FALSE;

      /* check if new keys has already been processed because of receipt of SMC on SRB2 or not, 
         if it is TRUE then it means that SMC is not received for last Authentication Request, 
         hence no need to touch new key flag for UL and
         also check whether new keys have been received and not activated for this SRB */
      if ((FALSE == tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          dl_new_key_flag_per_rb[DCCH_AM_RADIO_BEARER_ID]) && 
          (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          ul_new_key_flag_per_rb[rb_id]==TRUE))
      {
        /*reset new keys flag*/
        tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        ul_new_key_flag_per_rb[rb_id]=FALSE;

      }
      
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i[rb_id] = 
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].hfn,
          (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
           latest_config_cn_id_old].ul_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK) );

    }
    else
    {
      /*if we haven't reached activation time yet, set local_cn_domain 
        and flag to old*/
      local_cn_domain = tdsrrcsmc_int_ciph_info.latest_config_cn_id_old;

      /*update new cn domain's seq num to take care of condition when act time
        is not reached for old.. but we get another modify.. hence old and new
        SN's are out of sync*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        ul_count_i[rb_id]++;

      /* if old and new cn domain are the same, say due to authentication foll. by SMC,
         then we need to use old keys stored in backup.. else don't bother switching
         between old and new keys, because for a different domain, after auth, latest
         keys will be in current db and hence no need to set this flag*/
      /*Merged WCDMA CR343287*/
   
      if((local_cn_domain == tdsrrcsmc_int_ciph_info.latest_config_cn_id) &&
         (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          ul_new_key_flag_per_rb[rb_id]==TRUE))
      {
        using_old_keys_flag = TRUE;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Using old key local_cn_domain: %d tdsrrcsmc_int_ciph_info.latest_config_cn_id: %d key: for local_cn_domain: %d ", 
         local_cn_domain, tdsrrcsmc_int_ciph_info.latest_config_cn_id, tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          ul_new_key_flag_per_rb[rb_id]);
      }
      else
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"Dont use old keys local_cn_domain: %d tdsrrcsmc_int_ciph_info.latest_config_cn_id: %d key for local_cn_domain: %d ", 
          local_cn_domain, tdsrrcsmc_int_ciph_info.latest_config_cn_id, tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          ul_new_key_flag_per_rb[rb_id]);
      }
    }
  }



  /*increment the ul sn*/
  tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
  ul_count_i[rb_id]++;
  #ifdef FEATURE_TDSCDMA_RRC_SN_INCREASE_IN_SRNS

  if ((rb_id == DCCH_AM_RADIO_BEARER_ID) && (T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete
       == tdsrrc_get_ul_dcch_message_type((uint8 *)ul_sdu,(uint32)(ul_sdu_length>>3))))
  {
    if (tdsrrcrb_is_srns_in_progress())
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"SRNS relocation complete message Count-I 0x%x",tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].ul_count_i[rb_id]);
      if ((tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
        ul_count_i[rb_id]&0x0000000F) == 0x0000000F)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRNS relocation complete SN+1 when SN=15 ");
        tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].ul_count_i[rb_id]++;
      }
    }
  }
  #endif

  /*set the keys to old*/
  if (using_old_keys_flag == TRUE)
  {
    /*
    tdsrrcf9_init_key_values(
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);*/

    /*using the latest used key per rb*/
    tdsrrcf9_init_key_values(
         tdsrrcsmc_latest_used_ip_key[rb_id],
         tdsrrcsmc_latest_used_ip_key[rb_id]);

  }

TDSRRC_MSG2(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",local_cn_domain,tdsrrcsmc_int_ciph_info.saved_integrity_algo[local_cn_domain]);

  /*call the f9 driver function to generate MAC for the received UL SDU*/
  local_mac= tdsrrcf9_generate_mac_for_message(
               tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
               ul_count_i[rb_id],
               TDSRRC_INTEGRITY_DIR_UPLINK,
               ul_sdu,
               ul_sdu_length,
               local_cn_domain,
               tdsrrcsmc_int_ciph_info.saved_integrity_algo[local_cn_domain]);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"maci gen = 0x%x ",local_mac);

  /*set the keys back to new*/
  if (using_old_keys_flag == TRUE)
  {
    /*Do not revert to latest key as they may not be activated yet*/
    /*
    tdsrrcf9_init_key_values(
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
      */

    /*revert to latest committed keys*/
    tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);

  }

  /*get the tdsrrcllc_seq_num for this message from count_i by doing a bitwise and with 0xF
    to get the tdsrrcllc_seq_num.  count_i  = hfn[28bits]tdsrrcllc_seq_num[4 bits]*/
  local_seq_num = tdsrrcsmc_integrity_protection_info.count_i_info[local_cn_domain].
                  ul_count_i[rb_id];
  local_seq_num &= 0xF;

  tdsrrcsmc_append_mac_seq_no_to_ul_sdu(ul_sdu, local_mac, local_seq_num);

  return SUCCESS;
}/*tdsrrcsmc_append_uplink_integrity_check_info*/
/*===========================================================================

FUNCTION tdsrrcsmc_check_integrity_status

DESCRIPTION
  This function checks if Integrity Protection is already started or not.
  Other procedures may call this function to check the Start/NotStart
  Status of Integrity Protection.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TDSRRCSMC_IP_STARTED if Integrity Protection is started.
  Returns TDSRRCSMC_IP_NOT_STARTED if Integrity Protection is not started.


SIDE EFFECTS
  None

===========================================================================*/
tdsrrcsmc_ip_status_e_type tdsrrcsmc_check_integrity_status
(
void
)
{
  if (tdsnv_integrity_enabled)
  {
    /*check whether integrity is started or not*/
    if (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_STARTED )
    {
      return TDSRRCSMC_IP_STARTED;
    }
    else
    {
      return TDSRRCSMC_IP_NOT_STARTED;
    }
  }
  else
  {
    return(TDSRRCSMC_IP_NOT_STARTED);
  }
}/*tdsrrcsmc_check_integrity_status*/
/*===========================================================================

FUNCTION tdsrrcsmc_check_integrity_for_security_mode_command

DESCRIPTION
  This function validates Integrity Protection for Security Mode Command.
  It also updates the FRESH and KEYS if it is the START of Integrity.
  It also updates the KEY values if we have received new keys.
  
DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type : 
  Returns SUCCESS if Integrity Check for Security Mode Command 
  is successful.
  Returns FAILURE if Integrity Check for Security Mode Command
  is not successful
  
SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_check_integrity_for_security_mode_command
(
tdsrrc_cmd_type *cmd_ptr                    /* Pointer to the RRC Command */
)
{  
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr;         /* Pointer to Downlink DCCH Msg */

  tdsrrc_RB_Identity rb_id;/*local var to store rb_id*/
  tdsrrc_IntegrityProtectionModeInfo* integrityProtModeInfo;

  /*flag to indicate whether new keys are being*/  
  boolean rcvd_new_keys_flag=FALSE;

  /*local vars to store bunch of values used for manipulation in this func.only*/
  uint32 local_fresh, local_seq_no, old_seq_no; 
  uint32 local_count_i, local_hfn;
  uint32 received_mac, local_mac;
  tdsrrcsmc_integrity_check_info_type int_check_info;

  /*local copy of domain id. Set it to cs domain to suppress warning*/
  rrc_cn_domain_identity_e_type domain_id=RRC_CS_DOMAIN_CN_ID;  

  boolean local_start_recv = FALSE;

  // KW error
  int_check_info.msg_auth_code = 0;
  local_count_i = 0;


  /*get the dcch_msg_ptr for SMC as this message is already decoded*/
  dcch_msg_ptr =  (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /*copy integrityProtectionModeInfo * to a local copy to make easy readability*/
  integrityProtModeInfo = &(dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                            securityModeCommand_r3.integrityProtectionModeInfo);

  /*get the domain id*/
  if (dcch_msg_ptr->message.u.securityModeCommand.u.r3.
      securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else if (dcch_msg_ptr->message.u.securityModeCommand.u.r3.
           securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
  {
    domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  /*get the rb_id on which the message was received*/
  rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);

  if(rb_id < TDSRRC_MAX_SRB)
  {

    local_seq_no = dcch_msg_ptr->integrityCheckInfo.rrc_MessageSequenceNumber;

    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_modify)
    {
      if (tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
          dl_new_key_flag_per_rb[rb_id] == TRUE)
      {
        /*set this local flag to true*/
        rcvd_new_keys_flag=TRUE;
        /*set hfn to zero, as we have to reset HFN part of start value to 0*/
        local_hfn = 0x0;
      }
      else if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
              == TRUE)
      {
        /*if the start_value_update flag is set then we have updated the start value
          due to an IDT, this value needs to be used for COUNT_I calculation*/
        local_hfn =  
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,
            0);      
      }
      else
      {
        local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
                    dl_count_i[rb_id];
      }

      /*get the seq num of the old domain*/
      old_seq_no = (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                    latest_config_cn_id].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK);


      /*check if old sequence number is greater than current seq. no.  if that is the
        case, then there has been a rollover.. so increment the HFN by 1, as per 1808*/
      if (old_seq_no > local_seq_no)
      {
        /*increment hfn only*/
        local_hfn >>=4;
        local_hfn++;
        local_hfn <<=4;
      }
    }
    else
    {
      local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id];
    }

    /*generate count_i from hfn and sequence number*/
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (local_seq_no & TDSRRC_SEQ_NUM_BIT_MASK) 
             );

    /*get the value of fresh from the received message and initialize F9 engine.
      We can receive the value of FRESH only if START chosen*/
    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      local_start_recv = TRUE;
      local_fresh = b_unpackd(
        integrityProtModeInfo->integrityProtectionModeCommand.u.startIntegrityProtection->
        integrityProtInitNumber.data, 
        TDSRRC_DEFAULT_BIT_POS,
        TDSRRC_FRESH_LENGTH_IN_BITS);  

      /*update fresh value for this connection in F9 engine*/
      tdsrrcf9_init_fresh_per_connection(local_fresh);
      /*init the IK values for CS and PS domains and the fresh value for
      this connection*/
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
    }
    else
    {
        local_start_recv = FALSE;
        /*Do not init with the other domain's latest as it may not be activated yet*/
        if(domain_id == RRC_CS_DOMAIN_CN_ID)
        {
          tdsrrcf9_init_key_values(
            tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
            tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
        }
        else
        {
          tdsrrcf9_init_key_values(
            tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
            tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
        }

    }

    (void) tdsrrcsmc_process_dl_integrity_check_info(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                                  &int_check_info,rb_id);
  }
  /*Update the received message*/
  received_mac = int_check_info.msg_auth_code;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",
                  domain_id,
                  tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &integrityProtModeInfo->integrityProtectionAlgorithm,
                TDSMSG_REL99),0);

  /*call the f9 engine to generate MAC for the dl message*/  
  local_mac = tdsrrcf9_generate_mac_for_message(
                local_count_i,
                TDSRRC_INTEGRITY_DIR_DOWNLINK,
                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length*8),
                domain_id,
                tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &integrityProtModeInfo->integrityProtectionAlgorithm,
                TDSMSG_REL99));

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"maci gen = 0x%x received_mac = 0x%x",local_mac,received_mac);


  /*if the generated and received mac are the same, update our copy of count_i
    and set the init_flag to true*/
  if ( local_mac == received_mac)
  {
    /*SMC Modify received*/
    if(local_start_recv == FALSE)
    {
      /*store the committed new key for current domain*/
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
       memscpy(tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));      

      }
      else
      {
        memscpy(tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      }

      /*update for SRB2: DCCH_AM_RADIO_BEARER_ID*/
      memscpy(tdsrrcsmc_latest_used_ip_key[DCCH_AM_RADIO_BEARER_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               tdsrrcsmc_last_committed_ip_key[(uint8)domain_id], 
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));

    }
    else  /*SMC Start received*/
    {
      /*store the committed key for both domains*/
      uint8 i = 0;

      memscpy(tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      memscpy(tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));

      /*init the tdsrrcsmc_latest_used_ip_key*/
      for(i=0; i<TDSRRC_MAX_SRB; i++)
      {
        memscpy(tdsrrcsmc_latest_used_ip_key[i], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               tdsrrcsmc_last_committed_ip_key[(uint8)domain_id], 
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      }
    }

    if(rb_id < TDSRRC_MAX_SRB)
    {
      /* new keys flag wont be set to TRUE is received type is integrity STARTED */
      if (rcvd_new_keys_flag == TRUE)
      {
        /*reset new keys flag.  we are changing it here to be sure that the SMC
            command was successfully integrity checked*/
        tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
        dl_new_key_flag_per_rb[rb_id] = FALSE;
  
        /*any previously sent start value is obliterated.. we should start using 0's from
          now onwards.. hence set start_value_update_flag to false*/
        tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;      
  
        /*reset the hfn values to 0's if authentication is done and UE is
         *in TDS mode in dch/fach
         */
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[0]=0x0;
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[1]=0x0;
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[2]=0x0;
  
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Started using new keys, CIPH START VAL: 0x%x 0x%x 0x%x", 
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[0],
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[1],
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[2]);
        
      }
  
      /*Inform RR for the Key set change at TDS side in case a New Key is AKAed just before IRAT HO 
        So RR knows which is the latest activated key set*/
      if((domain_id == RRC_CS_DOMAIN_CN_ID) && (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
              ciph_keys_info.new_key == TRUE))
      {
        tdsrrciho_send_cipher_sync_ind();
      }

      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id] = local_count_i;
  
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
      dl_count_i_init_flag[rb_id] = TRUE;

      /*reset the start_value_update flag as we already passed integrity check
      successfully for this domain, also copy the updated start value to the
      database and initialize rb2, as there is no act time for rb2..
      also start_value_update_flag will be true only for Integrity Modify,
      as START value will be updated for integrityStart in smc_initial substate
      function*/
      if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE &&
         !(rcvd_new_keys_flag))
      {
        /*copy the latest start value sent in IDT or CU message*/
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
               tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,        
               RRC_MAX_HFN_LENGTH*sizeof(byte));
        
        /*since there is no pending activation time for RB2, update COUNT_I info for new 
          CN domain here when successfully check for SMC is done*/
        tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].ul_count_i[2] =         
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,
            tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            ul_count_i[2]);
  
        tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;
      }
  
      //set the Integrity START status for this domain ID to true.
      //only if this flag is set, then we will extract latest start values
      //at connection release time.
      tdsrrcsmc_int_ciph_info.int_start_status_for_conn[domain_id] = TRUE;
    }
    return SUCCESS;
  }
  else
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
	    	                   TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
    /*if we have received new keys and this SMC message failed Integrity
        test, then we revert back to the old keys as we will not be using 
        the new keys or new Security Configuration*/
    /*
    if (rcvd_new_keys_flag == TRUE)
    {
      tdsrrcf9_init_key_values(
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
    } 
    */
    
    /*Update to revert based on last committed for modify case*/
    if(local_start_recv == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL IP check failed for SMC itself,revert to last committed key");
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
    }
    return FAILURE;
  }
}/*tdsrrcsmc_check_integrity_for_security_mode_command*/

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
/*===========================================================================

FUNCTION tdsrrcsmc_try_integrity_for_smc

DESCRIPTION
  This function validates Integrity Protection for Security Mode Command
  when there is parallel SMC sending to UE. 
  It does NOT updates the FRESH and KEYS if it is the START of Integrity.
  It does NOT updates the KEY values if we have received new keys.
  
DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type : 
  Returns SUCCESS if Integrity Check for Security Mode Command 
  is successful.
  Returns FAILURE if Integrity Check for Security Mode Command
  is not successful
  
SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_try_integrity_for_smc
(
tdsrrc_cmd_type *cmd_ptr                    /* Pointer to the RRC Command */
)
{  
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr;         /* Pointer to Downlink DCCH Msg */

  tdsrrc_RB_Identity rb_id;/*local var to store rb_id*/
  tdsrrc_IntegrityProtectionModeInfo* integrityProtModeInfo;

  /*flag to indicate whether new keys are being*/  
  //boolean rcvd_new_keys_flag=FALSE;

  /*local vars to store bunch of values used for manipulation in this func.only*/
  uint32 local_seq_no, old_seq_no; 
  uint32 local_count_i, local_hfn;
  uint32 received_mac, local_mac;
  tdsrrcsmc_integrity_check_info_type int_check_info;
  tdsrrc_downlink_sdu_ind_type local_msg_sdu;  /*hold the original bit stream*/

  boolean local_start_recv = FALSE;

  /*local copy of domain id. Set it to cs domain to suppress warning*/
  rrc_cn_domain_identity_e_type domain_id = RRC_CS_DOMAIN_CN_ID;  

  // KW error
  int_check_info.msg_auth_code = 0;
  local_count_i = 0;

  local_msg_sdu.dl_sdu = NULL; 
  
  /*get the dcch_msg_ptr for SMC as this message is already decoded*/
  dcch_msg_ptr =  (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /*copy integrityProtectionModeInfo * to a local copy to make easy readability*/
  integrityProtModeInfo = &(dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                            securityModeCommand_r3.integrityProtectionModeInfo);

  /*get the domain id*/
  if (dcch_msg_ptr->message.u.securityModeCommand.u.r3.
      securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else if (dcch_msg_ptr->message.u.securityModeCommand.u.r3.
           securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
  {
    domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  /*get the rb_id on which the message was received*/
  rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);

  if(rb_id < TDSRRC_MAX_SRB)
  {

    local_seq_no = dcch_msg_ptr->integrityCheckInfo.rrc_MessageSequenceNumber;

    /*No need to worry about the new key flag / start_value_update_flag  as the last SMC will mark it if it was correct*/
    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_modify)
    {
      if (tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
          dl_new_key_flag_per_rb[rb_id] == TRUE)
      {
        /*set this local flag to true*/
        //rcvd_new_keys_flag=TRUE;
        /*set hfn to zero, as we have to reset HFN part of start value to 0*/
        local_hfn = 0x0;
      }
      else if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
              == TRUE)
      {
        /*if the start_value_update flag is set then we have updated the start value
          due to an IDT, this value needs to be used for COUNT_I calculation*/
        local_hfn =  
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,
            0);      
      }
      else
      {
        local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
                    dl_count_i[rb_id];
      }

      /*get the seq num of the old domain*/
      old_seq_no = (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                    latest_config_cn_id].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK);


      /*check if old sequence number is greater than current seq. no.  if that is the
        case, then there has been a rollover.. so increment the HFN by 1, as per 1808*/
      if (old_seq_no > local_seq_no)
      {
        /*increment hfn only*/
        local_hfn >>=4;
        local_hfn++;
        local_hfn <<=4;
      }
    }
    else
    {
      local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id];
    }

    /*generate count_i from hfn and sequence number*/
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (local_seq_no & TDSRRC_SEQ_NUM_BIT_MASK) 
             );

    /*get the value of fresh from the received message and initialize F9 engine.
      We can receive the value of FRESH only if START chosen*/
    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      local_start_recv = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should NOT be a SMC with START"); 
      /*No need to check the fresh as it should not be SMC with START*/

      /*init the IK values for CS and PS domains and the fresh value for
        this connection*/
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);

    }
    else
    {
      local_start_recv = FALSE;
      /*Do not init with the other domain's latest as it may not be activated yet*/
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        tdsrrcf9_init_key_values(
          tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
          tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
      }
      else
      {
        tdsrrcf9_init_key_values(
          tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
          tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
      }
    }



    /*back up the dl sdu as the content will be updated*/
    local_msg_sdu.dl_sdu = tdsrrc_malloc(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    if(local_msg_sdu.dl_sdu == NULL)
    {
      ERR_FATAL("Mem allocation failure for local_msg.dl_sdu",0,0,0);
    }
    memscpy(local_msg_sdu.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    (void) tdsrrcsmc_process_dl_integrity_check_info(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                                  &int_check_info,rb_id);
  }
  /*Update the received message*/
  received_mac = int_check_info.msg_auth_code;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",
                  domain_id,
                  tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &integrityProtModeInfo->integrityProtectionAlgorithm,
                TDSMSG_REL99),0);

  /*call the f9 engine to generate MAC for the dl message*/  
  local_mac = tdsrrcf9_generate_mac_for_message(
                local_count_i,
                TDSRRC_INTEGRITY_DIR_DOWNLINK,
                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length*8),
                domain_id,
                tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
                &integrityProtModeInfo->integrityProtectionAlgorithm,
                TDSMSG_REL99));

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"maci gen = 0x%x received_mac = 0x%x",local_mac,received_mac);


  /*Restore the dl sdu and free the allocated memory*/
   if(local_msg_sdu.dl_sdu != NULL)
   {
      memscpy(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
             local_msg_sdu.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      /*free the temp buffer */
      tdsrrc_free(local_msg_sdu.dl_sdu);
   }

   

  /*if the generated and received mac are the same, update our copy of count_i
    and set the init_flag to true*/
  if ( local_mac == received_mac)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"The second SMC passed IP check before L2ACK received for the first SMC");
    return SUCCESS;
  }
  else
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							   TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
    /*if we have received new keys and this SMC message failed Integrity
        test, then we revert back to the old keys as we will not be using 
        the new keys or new Security Configuration*/

    /*Do not turn back as the tdsrrcsmc_check_downlink_integrity 
      will take care of the new key */

    /*
    if (rcvd_new_keys_flag == TRUE)
    {
      tdsrrcf9_init_key_values(
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
    } 
    */ 

    /*Update to revert based on last committed for modify case*/
    if(local_start_recv == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL IP check failed for SMC itself,revert to last committed key");
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
    }


    TDSRRC_MSG0(MSG_LEGACY_ERROR,"The second SMC CANNOT passed IP check before L2ACK received for the first SMC");
    return FAILURE;
  }
}/*tdsrrcsmc_try_integrity_for_smc*/

/*===========================================================================

FUNCTION tdsrrcsmc_try_integrity_for_smc_r7

DESCRIPTION
  This function validates Integrity Protection for Security Mode Command
  when there is parallel SMC sending to UE. 
  It does NOT updates the FRESH and KEYS if it is the START of Integrity.
  It does NOT updates the KEY values if we have received new keys.
  
DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type : 
  Returns SUCCESS if Integrity Check for Security Mode Command 
  is successful.
  Returns FAILURE if Integrity Check for Security Mode Command
  is not successful
  
SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_try_integrity_for_smc_r7
(
  tdsrrc_cmd_type *cmd_ptr                    /* Pointer to the RRC Command */
)
{  
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr;         /* Pointer to Downlink DCCH Msg */

  tdsrrc_RB_Identity rb_id;/*local var to store rb_id*/
  tdsrrc_IntegrityProtectionModeInfo_r7* integrityProtModeInfo;

  /*flag to indicate whether new keys are being*/  
  //boolean rcvd_new_keys_flag=FALSE;

  /*local vars to store bunch of values used for manipulation in this func.only*/
  uint32 local_seq_no, old_seq_no; 
  uint32 local_count_i, local_hfn;
  uint32 received_mac, local_mac;
  tdsrrcsmc_integrity_check_info_type int_check_info;
  tdsrrc_downlink_sdu_ind_type local_msg_sdu;  /*hold the original bit stream*/

  boolean local_start_recv = FALSE;

  /*local copy of domain id. Set it to cs domain to suppress warning*/
  rrc_cn_domain_identity_e_type domain_id = RRC_CS_DOMAIN_CN_ID;  

  // KW error
  local_count_i = 0;
  int_check_info.msg_auth_code = 0;

  local_msg_sdu.dl_sdu = NULL; 

  /*get the dcch_msg_ptr for SMC as this message is already decoded*/
  dcch_msg_ptr =  (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;


  /*copy integrityProtectionModeInfo * to a local copy to make easy readability*/
  integrityProtModeInfo = &(dcch_msg_ptr->message.u.securityModeCommand.u.
    later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.integrityProtectionModeInfo);


  /*get the domain id*/
  if (dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.
            cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else if (dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.
            cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
  {
    domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  /*get the rb_id on which the message was received*/
  rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);



  if(rb_id < TDSRRC_MAX_SRB)
  {
    local_seq_no = dcch_msg_ptr->integrityCheckInfo.rrc_MessageSequenceNumber;

    /*No need to worry about the new key flag / start_value_update_flag  as the last SMC will mark it if it was correct*/
    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_modify)
    {
      if (tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
          dl_new_key_flag_per_rb[rb_id] == TRUE)
      {
        /*set this local flag to true*/
        //rcvd_new_keys_flag=TRUE;
        /*set hfn to zero, as we have to reset HFN part of start value to 0*/
        local_hfn = 0x0;
      }
      else if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
              == TRUE)
      {
        /*if the start_value_update flag is set then we have updated the start value
          due to an IDT, this value needs to be used for COUNT_I calculation*/
        local_hfn =  
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,
            0);      
      }
      else
      {
        local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
                    dl_count_i[rb_id];
      }

      /*get the seq num of the old domain*/
      old_seq_no = (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                    latest_config_cn_id].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK);


      /*check if old sequence number is greater than current seq. no.  if that is the
        case, then there has been a rollover.. so increment the HFN by 1, as per 1808*/
      if (old_seq_no > local_seq_no)
      {
        /*increment hfn only*/
        local_hfn >>=4;
        local_hfn++;
        local_hfn <<=4;
      }
    }
    else
    {
      local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id];
    }

    /*generate count_i from hfn and sequence number*/
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (local_seq_no & TDSRRC_SEQ_NUM_BIT_MASK) 
             );

  /*get the value of fresh from the received message and initialize F9 engine.
    We can receive the value of FRESH only if START chosen*/
  if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
      T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
  {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should NOT be a SMC with START"); 
      /*No need to check the fresh as it should not be SMC with START*/

      /*init the IK values for CS and PS domains and the fresh value for
      this connection*/
    tdsrrcf9_init_key_values(
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
  }
  else
  {
    local_start_recv = FALSE;
    /*Do not init with the other domain's latest as it may not be activated yet*/
    if(domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
        tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
    }
    else
    {
      tdsrrcf9_init_key_values(
        tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
    }
  }

    /*back up the dl sdu as the content will be updated*/
    local_msg_sdu.dl_sdu = tdsrrc_malloc(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    if(local_msg_sdu.dl_sdu == NULL)
    {
      ERR_FATAL("Mem allocation failure for local_msg.dl_sdu",0,0,0);
    }
    memscpy(local_msg_sdu.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

  (void) tdsrrcsmc_process_dl_integrity_check_info(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                                &int_check_info,rb_id);
  } /*if(rb_id < TDSRRC_MAX_SRB)*/

  /*Update the received message*/
  received_mac = int_check_info.msg_auth_code;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",
                  domain_id,
                   tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &integrityProtModeInfo->integrityProtectionAlgorithm,
             TDSMSG_REL7),0);

  /*call the f9 engine to generate MAC for the dl message*/  
  local_mac = tdsrrcf9_generate_mac_for_message(
                local_count_i,
                TDSRRC_INTEGRITY_DIR_DOWNLINK,
                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length*8),
                domain_id,
                tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &integrityProtModeInfo->integrityProtectionAlgorithm,
             TDSMSG_REL7));


   TDSRRC_MSG2(MSG_LEGACY_HIGH,"local_mac = 0x%x,received_mac = 0x%x",local_mac,received_mac);

  /*Restore the dl sdu and free the allocated memory*/
   if(local_msg_sdu.dl_sdu != NULL)
   {
      memscpy(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
             local_msg_sdu.dl_sdu, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      /*free the temp buffer */
      tdsrrc_free(local_msg_sdu.dl_sdu);
   }


  /*if the generated and received mac are the same, update our copy of count_i
    and set the init_flag to true*/
  if ( local_mac == received_mac)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"The second SMC passed IP check before L2ACK received for the first SMC");
    return SUCCESS;
  }
  else
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							   TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
    /*if we have received new keys and this SMC message failed Integrity
        test, then we revert back to the old keys as we will not be using 
        the new keys or new Security Configuration*/

    /*
    if (rcvd_new_keys_flag == TRUE)
    {
      tdsrrcf9_init_key_values(
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
    }
    */ 

    /*Update to revert based on last committed for modify case*/
    if(local_start_recv == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL IP check failed for SMC itself,revert to last committed key");
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);

    }


    TDSRRC_MSG0(MSG_LEGACY_ERROR,"The second SMC CANNOT passed IP check before L2ACK received for the first SMC");
    return FAILURE;
  }
}/*tdsrrcsmc_try_integrity_for_smc_r7*/

#endif

/*===========================================================================

FUNCTION tdsrrcsmc_update_int_activation_time

DESCRIPTION
  This function processes a Security Mode Command with Modify. Copies the
  DL Activation Info time in count_i_new var and also UL activatioin time
  in count_i_new
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcsmc_update_int_activation_time
(
void
)
{
  uint32 i;    /*counter in for loop*/

  /*initialize count_i for NEW latest CN domain*/
  for ( i=0; i < TDSRRC_MAX_SRB; i++)
  {
    if ((TRUE == tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].dl_count_i_act_time_pending[i]) &&
		    (FALSE == tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i]))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"dl_latest_config_cn_id_old_1_valid for SRB: %d,saving keys for domain latest_config_cn_id_old_1:%d(0:CS)  ",i,tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]);

      tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i] = TRUE;

/*
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ciph_keys_info.new_key: %d,if 1 using old IK keys else latest IK keys",tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1].ciph_keys_info.new_key);


      if (TRUE ==
  	  	tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1].ciph_keys_info.new_key)
      {
*/
        /* check whether new keys are received for this cn domain or not - CK and IK will be received at the same time for a given domain  
		       NOTE: we may need some other check above for new keys ????? */


      /*The original logic above seems problematic since the new_key is not updated in normal case*/
      /*Change it to per SRB related. Logic: if the domain cn_id_old_1 has pending key, then we need use the old key in that domain */

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"dl_new_key_flag_per_rb: %d, if 1 using old IK keys else latest IK keys in domain old_1",
	 	           tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]].
                         dl_new_key_flag_per_rb[i], 0, 0);

      if (TRUE == tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]].
                         dl_new_key_flag_per_rb[i])
      {
        
        memscpy(tdsrrcsmc_integrity_protection_info.integrity_key_for_cn_id_old_1[i], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]].integrity_key_old,
             RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );      	
      }
      else
      {
        memscpy(tdsrrcsmc_integrity_protection_info.integrity_key_for_cn_id_old_1[i],RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]].integrity_key,
             RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
      }
    }
    else
    {
      /*disable the following reset. Do not FIFO the old_1 if it is in valid state*/
      /*tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i] = FALSE;*/
      TDSRRC_MSG3(MSG_LEGACY_MED,"dl_latest_config_cn_id_old_1_valid is %d, for SRB%d, domain=%d",
                   tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i],
                   i, 
                   tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]);
    }

    /*initialize downlink count_i dl_activation_info time sent by utran
    for this domain */
    tdsrrcsmc_integrity_protection_info.dl_act_seq_num[i] = 
    tdsrrcsmc_saved_security_mode_command_msg.srb_int_dl_act_time[i].seq_num;

    /*initialize uplink count_i activation_info for this domain.  Add 
      TDSRRC_UL_ACT_TIME_OFFSET to the current value of SEQ NUMBER for all rb's 
      other than RB_0.  for RB0 add (N302+2).  For RB2 add just 1 as
      we want to start Integrity Protection on the new configuration
      immediately.  TDSRRC_UL_ACT_TIME_OFFSET is set to 1 currently..*/
    if (i == CCCH_RADIO_BEARER_ID)
    {
      tdsrrcsmc_integrity_protection_info.ul_act_seq_num[i] =  
      (( (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
          ul_count_i[i] & TDSRRC_SEQ_NUM_BIT_MASK) + TDSRRC_GET_N302() + 2) % 16);
    }
    else if( i == DCCH_AM_RADIO_BEARER_ID )
    {
      tdsrrcsmc_integrity_protection_info.ul_act_seq_num[i] =  
      (( (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
          ul_count_i[i] & TDSRRC_SEQ_NUM_BIT_MASK) + 1) % 16);
    }
    else if (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
             latest_config_cn_id_old].ul_count_i_act_time_pending[i] != TRUE)
    {
      tdsrrcsmc_integrity_protection_info.ul_act_seq_num[i] =  
      (( (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
          ul_count_i[i] & TDSRRC_SEQ_NUM_BIT_MASK) + 1) % 16);
    }

    /*for rb_id 2 do not set the act time pending flag.  no activation time
      is required for rb_id 2*/
    if (i != DCCH_AM_RADIO_BEARER_ID)
    {
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i_act_time_pending[i] = TRUE; 
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      dl_count_i_act_time_pending[i] = TRUE; 
    }
    else
    {
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i_act_time_pending[i] = FALSE; 
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      dl_count_i_act_time_pending[i] = FALSE; 
    }
    
    /*copy old cn domain's Seq_Num to new domain's ul_count_i as tdsrrcllc_seq_num is common across
      domains.. only HFN part differs.. the HFN will be adjusted when we hit activation
      time in the uplink/downlink integrity processing*/
    tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i[i] =
      ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
       ul_count_i[i] & TDSRRC_HFN_BIT_MASK) |
       (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
       ul_count_i[i] & TDSRRC_SEQ_NUM_BIT_MASK));
    
    //don't update this for rb_id = 2 on downlink, as this will already be done in smc.. 
    //rb_id 2 is handled differently, as count_i's are already calculated at reception
    // of SMC itself
    if(i != DCCH_AM_RADIO_BEARER_ID)
    {
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        dl_count_i[i] =
        ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
         dl_count_i[i] & TDSRRC_HFN_BIT_MASK) |
         (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
         dl_count_i[i] & TDSRRC_SEQ_NUM_BIT_MASK));  
    }
  }
}/*tdsrrcsmc_update_int_activation_time*/
/*===========================================================================

FUNCTION tdsrrcsmc_validate_security_mode_command

DESCRIPTION
  This function validates security mode command for critical xtensions
  and also to check to see if either ciphering or integrity mode info
  is present in the security mode command
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_security_mode_command
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{
  /* Check if critical extensions are present */
  if (msg_ptr->message.u.securityModeCommand.t == 
      T_tdsrrc_SecurityModeCommand_later_than_r3)
  {
    tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR;   
    tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;   
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");
    return TDSRRCSMC_INVALID_CONFIGURATION;
  }

  /*if any security reconfiguration is currently in progress return failure
    indicating that this is a Simultaneous Reconfiguration*/
  if( (tdsrrcsmc_integrity_protection_info.reconfig == TRUE) || 
      (tdsrrcsmc_ciphering_info.reconfig == TRUE)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incompatible Simultaneous Ciph Reconfig");
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG;
    return TDSRRCSMC_INVALID_CONFIGURATION;
  }

  /*if both nv items are not enabled, then do not check any more IE's just
    return success*/
  if((!tdsnv_ciphering_enabled) && (!tdsnv_integrity_enabled))
  {
    return TDSRRCSMC_VALID_CONFIG;
  }

  /*Either ciphering or integrity protection mode info ie's should be present
    in message received by UE*/
  if((msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.m.cipheringModeInfoPresent) == 0)
  {
    tdsrrcsmc_int_ciph_info.ciphering_info_received = FALSE;
  }
  else
  {
    tdsrrcsmc_int_ciph_info.ciphering_info_received = TRUE;  
  }
  
  if((msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.m.integrityProtectionModeInfoPresent) == 0)
  {  
    tdsrrcsmc_int_ciph_info.integrity_info_received = FALSE;  
  }
  else
  {
    tdsrrcsmc_int_ciph_info.integrity_info_received = TRUE;    
  }

  /* if Ciphering Mode IE's and Integrity Protection Mode IE's
     are not included, then send SMC Failure message */  
  if( (tdsrrcsmc_int_ciph_info.integrity_info_received == FALSE ) &&
      (tdsrrcsmc_int_ciph_info.ciphering_info_received == FALSE )
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplte Configuration");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;
}/* tdsrrcsmc_validate_security_mode_command */
/*===========================================================================

FUNCTION tdsrrcsmc_validate_ciph_security_mode_command

DESCRIPTION
  This function validates ciphering part of SECURITY MODE COMMAND.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_ciph_security_mode_command
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{
  uint32 count, i, local_lc_id;            /*vars used for count purposes*/
  boolean tm_rb_present;                   /*flag used to check if tm rb's are present*/
  tdsrrc_CN_DomainIdentity local_domain_id;   /*domain id for which ciph config is being changed*/
  uint32 num_srbs_established = 3;         /*SRBs 1,2 and 3 are mandatory*/
  uint32 index=0,n_value=0;

  /*local ptr to Ciph. Mode Info. */
  struct tdsrrc_CipheringModeInfo* ciphModeInfo;

  /* local ptr to activation time list */
  struct tdsrrc_RB_ActivationTimeInfoList  *local_rb_ciph_act_time_list = NULL;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity srb5_rb_id = TDSRRCLLC_INVALID_RB_ID;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  /*set to FALSE*/
  tm_rb_present = FALSE;

  /* if Ciphering Mode IE's are not included, then send SMC Failure message */
  if ( (msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.m.cipheringModeInfoPresent) == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplte Configuration");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  /*check if activation times are present in do ciph activation.. they have to be present
    as SRB's are already started and have to be ciphered*/
  if ( (msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.cipheringModeInfo.m.
        rb_DL_CiphActivationTimeInfoPresent) == 0 )
  {
    /*lint -e793*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rrc_CipheringModeInfo_rb_DL_CiphActivationTimeInfo_present not present"); 
    /*lint +e793*/
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  /*get ptr to cipheringModeInfo. copy this to a local ptr to avoid 
    multiple indirections in later manipulations*/
  ciphModeInfo = &(msg_ptr->message.u.securityModeCommand.u.r3.
                   securityModeCommand_r3.cipheringModeInfo);

  /* Check to see if Ciph Activation time is included for all SRBs.Ignore SRB0
     since it is not ciphered.Check for SRBs 1,2 and 3 since they are mandatory.
     Check for RB4 only if it is already established */
  if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                   UE_LOGCHAN_DCCH,
                                   DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                   UE_MODE_ACKNOWLEDGED_DATA))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"SRB4 established");
    num_srbs_established = 4;
  }

  for(count = 1; count <= num_srbs_established; count++)
  {
    uint32 srb_found = 0;

    /* get the activation time list for RBs */
    local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

    /* Check to see if this RB info is included in the ciph activation times list */
    n_value = local_rb_ciph_act_time_list->n;
    index = 0;
    while (n_value != 0)
    {
      if(count == local_rb_ciph_act_time_list->elem[index].rb_Identity)
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"SRB id: %d included in ciph activation info in SMC",count);
        srb_found++;
      }
      index++;
      n_value--;
    }

    if(srb_found != 1)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRB id: %d is included %d times in ciph activation info in SMC",count,srb_found);
      tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
      return TDSRRCSMC_INVALID_CONFIGURATION;
    }
  }
  
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  index = 0;
  if(local_rb_ciph_act_time_list != NULL)
  {
    n_value = local_rb_ciph_act_time_list->n;
  }
  if(tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &srb5_rb_id) == TDSRRCLCM_SUCCESS)
  {
    /* get the activation time list for RBs */
    local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

    /* Check to see if this RB info is included in the ciph activation times list */
    while (n_value != 0)
    {
      if(srb5_rb_id == local_rb_ciph_act_time_list->elem[index].rb_Identity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"SMC validation failed; SRB#5 Id = %d,RB ID: %d",srb5_rb_id,local_rb_ciph_act_time_list->elem[index].rb_Identity);
        tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
        return TDSRRCSMC_INVALID_CONFIGURATION;
      }
      index++;
      n_value--;
    }
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  /*get the domain id of the latest configured CN domain for which we are
    configuring ciphering*/
  local_domain_id = msg_ptr->message.u.securityModeCommand.u.r3.
                    securityModeCommand_r3.cn_DomainIdentity;

  /*check to see if TM radio bearers are present.  only if present will we
    mandatorily check to see DPCH activation time is present.
    Also if the domain id is PS,check if SMC has 
    included all the PS RBs activation time in the message.If not
    UE should send a security mode failure and not honor the SMC */
  for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    {
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if (tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_status ==
            TDSRRC_RB_STARTED)
        {
          /*check if domain id is same as domain for which ciphering
            configuration is received for*/
          if(tdsrrc_est_rabs.rabs[count].cn_domain == local_domain_id)
          {
            if(local_domain_id == tdsrrc_CN_DomainIdentity_ps_domain) 
            {
              uint32 ps_rb_found = 0;

              /* get the activation time list for RBs */
              local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

              /* Check to see if this RB info is included in the ciph activation times list */
              /* Initializing index and n_value to scan whole list */
              index = 0;
              n_value = local_rb_ciph_act_time_list->n;
              while (n_value != 0)
              {
                if(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id ==
                    local_rb_ciph_act_time_list->elem[index].rb_Identity)
                {
                  ps_rb_found++;
                }
                index++;
                n_value--;
              }
              if(ps_rb_found != 1)
              {
                TDSRRC_MSG2(MSG_LEGACY_ERROR,"RB id: %d is included %d times in ciph activation info in SMC",tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id,ps_rb_found);
                tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
                return TDSRRCSMC_INVALID_CONFIGURATION;
              }
            }
            else /* CS domain */
            {
              /*copy to local vars to avoid multiple indirections*/
              local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);
  
              /*set TM radio bearers flag present to true*/
              if (local_lc_id < TDSRRCLCM_UL_UM_START_IDX)
              {
                tm_rb_present=TRUE;
                break;
              }
            }
          }
        }/*if*/
      }/*for*/
    }/*if*/

    /*if TM radio bearers are present, then break right now as we don't have
      to loop through all rabs in est_rabs*/
    if (tm_rb_present == TRUE)
    {
      break;
    }
  }/*for loop till MAX_RAB_TO_SETUP*/

  /*if only TM radio bearers are present then mandatorily DPCH activation
    information should be present.*/
  if ( ((ciphModeInfo->m.activationTimeForDPCHPresent) == 0) && 
       (tm_rb_present == TRUE )
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No DPCH Activation Time");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;
}/* tdsrrcsmc_validate_ciph_security_mode_command */
/*===========================================================================

FUNCTION tdsrrcsmc_validate_int_security_mode_command

DESCRIPTION
  This function validates integrity part of SECURITY MODE COMMAND.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_int_security_mode_command
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{  
  /*local ptr to Int. Prot. Mode Info.  */
  struct tdsrrc_IntegrityProtectionModeInfo* integrityProtModeInfo;


  /* if Integrity Protection Mode Info is not included, then send
     SMC Failure message
   */
  if ( (msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.m.
        integrityProtectionModeInfoPresent) == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplete Configuration");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  integrityProtModeInfo = &(msg_ptr->message.u.securityModeCommand.u.r3.
                            securityModeCommand_r3.integrityProtectionModeInfo);

  /*if we have already started integrity protection, then we should not
   *receive another start command*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t ==
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection) &&
       (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_STARTED) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Start Cmd ");    
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;
  }

  /*if we receive a START cmd and do not receive the integrity 
    protection algorithm, return invalid config*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection) &&  
       (integrityProtModeInfo->m.integrityProtectionAlgorithmPresent == 0 ) 
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplete Configuration");      
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;  
  }

  /*if we receive a MODIFY cmd and integrity is not yet started
    return invalid config*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t ==
        T_tdsrrc_IntegrityProtectionModeCommand_modify) &&
       (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_NOT_STARTED) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Configuration");      
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;          

}/*tdsrrcsmc_validate_int_security_mode_command*/

/*===========================================================================

FUNCTION tdsrrcsmc_send_security_mode_complete

DESCRIPTION
  This function sends the SECURITY MODE COMPLETE command on UL DCCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcsmc_send_security_mode_complete
(
uint32 tdstransaction_id
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr;         /* Pointer to uplink DCCH message */ 

  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */

  tdsrrcsend_status_e_type send_status;    /* The status of sending the Initial
                                         * Security Mode Complete */

  uint32 i=0, num_srb=0, n_index=0;                  /*used for counter purposes*/

  struct tdsrrc_RRC_MessageSequenceNumberList  *local_msg_seq_num_list=NULL;             
  /*local ptr to store msg seq num list
    used to populate SMC Complete msg*/

  tdsrrcsmc_ciph_act_type  local_suspend_act_sn;
  struct tdsrrc_RB_ActivationTimeInfoList *local_ciph_act_info;

  tdsrrc_RB_Identity local_rb_id=0;            

  tdsrrc_cmd_type *int_cmd_ptr;                    /*cmd ptr to send ciph config req*/

  memset(&local_suspend_act_sn, 0, sizeof(local_suspend_act_sn));

  tdsl2_ack_rcvd_for_smc_complete_flag = FALSE;
  tdsrrcsmc_send_l2_ack_ind_to_ddt_flag= FALSE;

  
  /* Determine the logical channel id for the Uplink DCCH
   */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                       DCCH_AM_RADIO_BEARER_ID, 
                                       UE_MODE_ACKNOWLEDGED_DATA);


  /*check if RB4 is setup or not and then config ciphering config for RB4*/
  if (TDSRRCLCM_RLC_LC_ID_NOT_FOUND != 
      tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                UE_MODE_ACKNOWLEDGED_DATA))
  {
    num_srb = TDSRRC_MAX_SRB;
  }
  else
  {
    num_srb = (TDSRRC_MAX_SRB-1);
  }

  /* Ensure that LCM found a valid logical channel id
   */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Found a valid RLC Logical Channel Id */
    msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(struct tdsrrc_UL_DCCH_Message));

    /* Call error fatal if the memory allocation failed*/
    if (msg_ptr == NULL)
    {
      ERR_FATAL("tdsrrc_malloc for smc complete failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
    }

    memset((void *)msg_ptr, 0, sizeof(tdsrrc_UL_DCCH_Message));

    /* Identify this message as a SECURITY MODE COMPLETE
     */
    msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_securityModeComplete;

    /* Initialize bit_mask to zero */
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.securityModeComplete);

    if ( (tdsnv_integrity_enabled) &&
         (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
    {
      /*set the bit_mask to indicate that we are sending the integrity
              protection activation info*/
      msg_ptr->message.u.securityModeComplete.m.ul_IntegProtActivationInfoPresent = 1;

      /*initialize the UL Integrity Protection Activation Info*/

      /*allocate memory for the local var local_msg_seq_num_list and then
              assign this address to the msg_ptr if alloc successful*/
      local_msg_seq_num_list = &(msg_ptr->message.u.securityModeComplete.
      ul_IntegProtActivationInfo.rrc_MessageSequenceNumberList);

      local_msg_seq_num_list->n = num_srb;
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt, local_msg_seq_num_list, tdsrrc_RRC_MessageSequenceNumber);

      if (local_msg_seq_num_list->elem == NULL)
      {
        ERR_FATAL("Memory Allocation Failed",0,0,0);
      }

      /*Initialize all the sequence number in the UL for all SRB's to 0*/
      for (i=0; i < num_srb; i++)
      {
        /* if we have received a modify command then copy the UL activation
                * time info instead of setting it to zero*/
        if (tdsrrcsmc_integrity_protection_info.modify_received == TRUE)
        {
          local_msg_seq_num_list->elem[i] = (uint8)tdsrrcsmc_integrity_protection_info.ul_act_seq_num[i];
        }
        else
        {
          local_msg_seq_num_list->elem[i] = 0;
        }
      }
    }/*check if Integrity IE's present*/
    
    if ((tdsnv_ciphering_enabled) &&
        (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) )
    {

      /*set the bit_mask to indicate that we are sending the integrity
        protection activation info*/
      msg_ptr->message.u.securityModeComplete.m.rb_UL_CiphActivationTimeInfoPresent = 1;

      /*allocate memory for the local var local_msg_seq_num_list and then
        assign this address to the msg_ptr if alloc successful*/
      local_ciph_act_info = &(msg_ptr->message.u.securityModeComplete.rb_UL_CiphActivationTimeInfo);

      local_ciph_act_info->n = tdsrrcsmc_ciphering_info.dl_ciph_act_info.nchan;
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt, local_ciph_act_info, tdsrrc_RB_ActivationTimeInfo);

      if (local_ciph_act_info->elem == NULL)
      {
        ERR_FATAL("Memory Allocation Failed",0,0,0);
      }

      /*loop through all the channels which we suspended and have already
        calculated activation times for..  save them in the local struct
        indexed by Radio Bearer index.. This is done to sort the activation
        times based on Radio Bearers efficiently while sending Uplink 
        Activation Times..*/
      for (i=0; i<tdsrrcsmc_ciphering_info.ul_ciph_act_info.nchan; i++)
      {
        local_rb_id = tdsrrclcm_get_ul_rb_id(tdsrrcsmc_ciphering_info.ul_ciph_act_info.
        lc_id_for_complete[i]);
        if (local_rb_id < TDS_MAX_RB)
        {
          local_suspend_act_sn.sq_num[local_rb_id] = 
          tdsrrcsmc_ciphering_info.ul_ciph_act_info.sq_num_for_complete[i];
        }
      }


      /*go thru the list of radio bearers.. append activation times only for
      the radio bearers for which we received activation times in DL.. we
      suspend all UM/AM Radio Bearers but may not need to send activation
      times for all UM/AM Radio Bearers.. we only send Activation Times for
      the UM/AM radio bearers for the ciphering changed domain*/

      n_index = 0;
      for (i=0; i<TDS_MAX_RB; i++)
      {
        if(tdsrrcsmc_ciphering_info.dl_ciph_act_info.act_time_present[i] == TRUE)
        {
           local_ciph_act_info->elem[n_index].rb_Identity = (tdsrrc_RB_Identity)i;
          /*lint -e771*/
           local_ciph_act_info->elem[n_index].rlc_SequenceNumber =             
            local_suspend_act_sn.sq_num[i];
          /*lint +e771*/
          n_index++;
        }
      }/*check if Ciphering IE's present*/
    }
    /* Copy the transaction ID of SECURITY MODE COMMAND to COMPLETE Msg*/
    msg_ptr->message.u.securityModeComplete.rrc_TransactionIdentifier = 
    (uint8)tdstransaction_id; 

    /* Send the SECURITY MODE COMPLETE on the Uplink DCCH
     */

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
    send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_SMC,
                                      msg_ptr,
                                      ul_lc_id,
                                      TDSRRCSEND_L2ACK_REQUIRED,
                                      &smc_cmp_mui_drop);
    /*the status should be cleared when receiving the corresponding L2ACK*/
    /*smc_cmp_mui_drop_status = FALSE;*/ 
    
#else
    send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_SMC,
                                      msg_ptr,
                                      ul_lc_id,
                                      TDSRRCSEND_L2ACK_REQUIRED);
#endif

    if (send_status == TDSRRCSEND_SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SECURITY MODE COMPLETE Message Sent");

        /*if feature is defined and nv item is turned off, then we need to just go
          to SMC idle without sending the config info to lower layers*/
      if((tdsnv_ciphering_enabled) && 
         (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) )
      {
        /*send cipher config req to llc, llc configures lower layers
          with ciphering information*/
        if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {
          /* Fill in the command parameters for TDSRRC_CIPH_CONFIG_REQ*/
          int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CIPH_CONFIG_REQ;
          /*Fill config_cause = TDSCIPH_CONFIG_DL_KEYS, which will update 
            L1 with only DL keys.UL Keys will be updated to l1 after receiving
            L2 Ack of SMC message*/
          int_cmd_ptr->cmd.rrc_ciph_config_req.config_cause = TDSCIPH_CONFIG_DL_KEYS;
  
          tdsrrc_put_int_cmd(int_cmd_ptr);  
          tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF;
  
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending CIPH_CONFIG_REQ to LLC");
        }
        else
        {
          ERR_FATAL("Unable to allocate memory for: %d cmd",TDSRRC_CIPH_CONFIG_REQ,0,0);
        }  
      }
      else
      {
        tdsrrcsmc_substate=TDSRRCSMC_WAIT_FOR_L2_ACK;  
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending SECURITY MODE COMPLETE",send_status);
      tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
    }

  }/* Found a valid RLC Logical Channel Id */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for RB 2");
  }
} /* tdsrrcsmc_send_security_mode_complete */

/*===========================================================================

FUNCTION tdsrrcsmc_send_security_mode_failure

DESCRIPTION
  This function sends the SECURITY MODE FAILURE command on UL DCCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcsmc_send_security_mode_failure
(
uint32 tdstransaction_id
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr;         /* Pointer to uplink DCCH message */ 

  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */

  tdsrrcsend_status_e_type send_status;    /* The status of sending the Initial
                                         * Security Mode Failure */

  /* Determine the logical channel id for the Uplink DCCH
     Ensure that LCM found a valid logical channel id
   */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                       DCCH_AM_RADIO_BEARER_ID, 
                                       UE_MODE_ACKNOWLEDGED_DATA);

  if (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't find RLC ID for RB 2");
    return;
  }

  /* Found a valid RLC Logical Channel Id */
  msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(struct tdsrrc_UL_DCCH_Message));

  /* Call error fatal if the memory allocation failed
       */
  if (msg_ptr == NULL)
  {
    ERR_FATAL("rrcmalloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
  }

  /*set message type to failure, non-critical errors to false and trans_id*/
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_securityModeFailure;
  msg_ptr->message.u.securityModeFailure.m.laterNonCriticalExtensionsPresent = 0;
  msg_ptr->message.u.securityModeFailure.rrc_TransactionIdentifier = 
  (OSUINT8)tdstransaction_id;

  /*set the correct failure cause and fill in the failure struct with
    info if any*/
  switch (tdsrrcsmc_failure_type)
  {

    /*set bitmask and failure causes to the appropriate types*/
    
    case TDSRRCSMC_PROTOCOL_ERROR:
      msg_ptr->message.u.securityModeFailure.failureCause.t = 
      T_tdsrrc_FailureCauseWithProtErr_protocolError;

      msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError=
	  	rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);

      if(msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError == NULL)
      {
        ERR_FATAL("Memory allocation failure",0,0,0);
      }
	
      msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError->
        diagnosticsType.t = T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;
	  
      msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError->
      diagnosticsType.u.type1 = rtxMemAllocTypeZ (&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation_type1);

      if(msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError->
          diagnosticsType.u.type1 == NULL)
      {
        ERR_FATAL("Memory allocation failure",0,0,0);
      }

      msg_ptr->message.u.securityModeFailure.failureCause.u.protocolError->
      diagnosticsType.u.type1->protocolErrorCause = tdsrrcsmc_prot_err_cause;

      /*log the protocol error*/
      tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                              (uint8)TDSRRC_PROCEDURE_SMC,
                              (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                              tdsrrc_log_map_protocol_cause(tdsrrcsmc_prot_err_cause)
                              );

      break;

    case TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG:
      msg_ptr->message.u.securityModeFailure.failureCause.t = 
      T_tdsrrc_FailureCauseWithProtErr_incompatibleSimultaneousReconfiguration;
      break;  

    case TDSRRCSMC_INCOMPLETE_CONFIGURATION:
    case TDSRRCSMC_INVALID_CONFIGURATION:
      msg_ptr->message.u.securityModeFailure.failureCause.t = 
      T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      break;  

    case TDSRRCSMC_CELL_UPDATE_OCCURED:
      msg_ptr->message.u.securityModeFailure.failureCause.t =   
      T_tdsrrc_FailureCauseWithProtErr_cellUpdateOccurred;
      break;

    case TDSRRCSMC_UNSUPPORTED_CONFIG:
      msg_ptr->message.u.securityModeFailure.failureCause.t = 
      T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      break;
  
    default:
      msg_ptr->message.u.securityModeFailure.failureCause.t = 
      T_tdsrrc_FailureCauseWithProtErr_spare1;  

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown SMC Failure Type");
      break;
  }  

  /*Send the SECURITY MODE COMPLETE on the Uplink DCCH
   */
  send_status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_SMC,
                                    msg_ptr,
                                    ul_lc_id,
                                    TDSRRCSEND_L2ACK_NOT_REQUIRED);

  if (send_status == TDSRRCSEND_SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SECURITY MODE FAILURE Message Sent");      
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending SECURITY MODE FAILURE",send_status);
  }
} /* tdsrrcsmc_send_security_mode_failure */

/*===========================================================================

FUNCTION tdsrrcsmc_fill_ul_resume_req

DESCRIPTION
   This function fills ul_resume_req with LC ids which were suspended while 
   processing security mode command.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE or FALSE

===========================================================================*/
boolean tdsrrcsmc_fill_ul_resume_req
(
  tdsrlc_ul_resume_type *rrc_ul_resume_req_ptr
)
{
  uint32 i;
  rrc_ul_resume_req_ptr->nchan = 0;

  if (tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE)
  {
    rrc_ul_resume_req_ptr->nchan = tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;

    /*in a loop set the contents of the suspend_req*/
    for (i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
    {
      rrc_ul_resume_req_ptr->channel[i] = 
         tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id;
    }

    /* mark RBs as not in suspended state */
    tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = FALSE;

    return(TRUE);
  }

  return(FALSE);
}
/*===========================================================================

FUNCTION tdsrrcsmc_send_resume_req

DESCRIPTION
  This function constructs the CRLC_UL_RESUME_REQ primitive and
  sends it to L2 UL Task.
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_send_resume_req
(
void
)
{
  /*counter*/
  uint32 i;  
  tdsl2_ul_cmd_type  *rlc_ptr = NULL;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resume suspended RB's (TDSCRLC_UL_RESUME_REQ)");

  if (tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE )
  {
    /*Send RESUME Req to RLC */
    /* Get L2 UL command buffer */
    if ((rlc_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending TDSCRLC_UL_RESUME_REQ ",0,0,0);
    }
  
    /* Fill in the Command Header */
    rlc_ptr->cmd_hdr.cmd_id = TDSCRLC_UL_RESUME_REQ;
  
    /* Fill in the Command Data */
    rlc_ptr->cmd_data.ul_resume.nchan =
      tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;
  
    for(i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
    {
      rlc_ptr->cmd_data.ul_resume.channel[i] =
        tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id;
    }
    /* Send command to L2 UL Task */
    tdsl2_ul_put_cmd(rlc_ptr);

    tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = FALSE;           
    tdsrrcsmc_ciphering_info.reconfig = FALSE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error: No suspended RLC RB's to resume");
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_till_standby_ends_substate

DESCRIPTION
   This function processes SMC proecdure in wait till standby ends state.  We can
   be waiting for rrc mode to change from standby to active or inactive mode

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_till_standby_ends_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_MODE_CHANGE_IND:

      if(tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_ACTIVE)
      {	
        /* set "Uplink RRC Message sequence number" for signalling radio bearer
           RB0 in the variable INTEGRITY_PROTECTION_INFO to a value such that
           next RRC message to be sent on uplink RB0 will use the new integrity 
           protection configuration and the "RRC Message sequence number" in the IE
           "Integrity check info" in the next RRC message will equal to the activation
           time for RB0 as indicated in IE "Uplink integrity protection activation info"
           included in the transmitted SECURITY MODE COMPLETE;
           -- following IF check is to meet the above requirement */
        if ((TRUE == tdsrrcsmc_integrity_protection_info.modify_received) &&
            (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
             ul_count_i_act_time_pending[CCCH_RADIO_BEARER_ID] == TRUE))
        {
          uint32 rb_0_act_time_jump;         /*local var to store rb_0 jump act time*/

          //set the UL SN to one less than the act-time, as we will increment the count_i
          //after adjusting for HFN at the very end.  This will take care of N302+1 as well
          //as any roll-oer adjustment due to act-time becoming 0
          if(0 == tdsrrcsmc_integrity_protection_info.ul_act_seq_num[CCCH_RADIO_BEARER_ID])
          {
            rb_0_act_time_jump = 0xF;
          }
          else
          {
            rb_0_act_time_jump = 
              (tdsrrcsmc_integrity_protection_info.ul_act_seq_num[CCCH_RADIO_BEARER_ID] - 1);
          }
    
          //set the UL SN to one less than the act-time, as we will increment the count_i
          //after adjusting for HFN at the very end.  This will take care of N302+1 as well
          //as any roll-oer adjustment due to act-time becoming 0
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
            ul_count_i[CCCH_RADIO_BEARER_ID] =
            ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
             ul_count_i[CCCH_RADIO_BEARER_ID] & TDSRRC_HFN_BIT_MASK) | 
            ( rb_0_act_time_jump & TDSRRC_SEQ_NUM_BIT_MASK));
    
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"SMComp L2 Ack rcvd; SRB0 -> updated RRC-SN: 0x%x; latest-count-I: 0x%x; previousDomain-count-I: 0x%x)",
            rb_0_act_time_jump, tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            ul_count_i[CCCH_RADIO_BEARER_ID], tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
            ul_count_i[CCCH_RADIO_BEARER_ID]);

          /* we need to copy the computed count-I<we need only SN> of _old domain
             to latest domain because if SMC is triggered before cell update 
             then activation SN for SRB0 need to be set using the SN of this latest
			 domain, which will become old domain with next SMC. */
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            ul_count_i[CCCH_RADIO_BEARER_ID] =
            tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
              ul_count_i[CCCH_RADIO_BEARER_ID];
        }
        if(tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE)
        {
          /*Lets Update L1 with UL cipher keys now.
            Note: We have already updated L1 with DL cipher keys*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Config UL cipher keys");
          tdsrrcsmc_update_cphy_key_req(TDSCIPH_CONFIG_UL_KEYS);
          tdsrrcsmc_send_ul_ciphering_key_req();
        }
   
        /*Setting new_key = FALSE after receiving L2 ACK */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set new_key_for_start_value to FALSE for CN doamin = %d",tdsrrcsmc_int_ciph_info.latest_config_cn_id);
        tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].ciph_keys_info.new_key_for_start_value= FALSE;
  
        /*Update any pending security keys generated in other RAT right here */
        tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
         
        //send resume_req to rlc after getting l2ack_ind
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send RESUME_REQ");
        tdsrrcsmc_send_resume_req();
  
        /*check to see if integrity and ciphering are established.. if only ciphering
          is established, then send cause as ciphering only*/
        if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_CIPHERING_ESTABLISHED);
  
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received != TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
         (tdsrrcsmc_int_ciph_info.integrity_info_received != TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_CIPHERING_ESTABLISHED);
        }
        else
        {
          if(tdsnv_integrity_enabled)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity & ciphering both not started");
          }
          else
          {
            tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
          }
        }
  
        /*received a l2ack, go to initial state*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2ACK for SMC received");
  
        tdsrrcsmc_process_l2_ack_ind_to_dt_procedures();
   
       
        tdsrrcsmc_integrity_protection_info.reconfig = FALSE;
        tdsrrcsmc_substate                    = TDSRRCSMC_INITIAL;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring MODE_CHANGE_IND for rrc mode other than ACTVE.");
      }

      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }          
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_for_l2_ack_substate

DESCRIPTION
   This function processes SMC proecdure in wait for l2 ack state.  We can
   be waiting for an L2 ACK for a SMC complete command

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_for_l2_ack_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:

      if (SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
      {
        if(tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_STANDBY)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SMC state changed to %d",TDSRRCSMC_WAIT_TILL_STANDBY_ENDS);
          tdsrrcmcm_register_for_mode_change(TDSRRC_PROCEDURE_SMC,TDSRRC_MODE_STANDBY,TDSRRC_MODE_WILDCARD);
          tdsrrcsmc_substate = TDSRRCSMC_WAIT_TILL_STANDBY_ENDS;
        }
        else
        {  			
        /* set "Uplink RRC Message sequence number" for signalling radio bearer
           RB0 in the variable INTEGRITY_PROTECTION_INFO to a value such that
           next RRC message to be sent on uplink RB0 will use the new integrity 
           protection configuration and the "RRC Message sequence number" in the IE
           "Integrity check info" in the next RRC message will equal to the activation
           time for RB0 as indicated in IE "Uplink integrity protection activation info"
           included in the transmitted SECURITY MODE COMPLETE;
           -- following IF check is to meet the above requirement */
        if ((TRUE == tdsrrcsmc_integrity_protection_info.modify_received) &&
            (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
             ul_count_i_act_time_pending[CCCH_RADIO_BEARER_ID] == TRUE))
        {
          uint32 rb_0_act_time_jump;         /*local var to store rb_0 jump act time*/

          //set the UL SN to one less than the act-time, as we will increment the count_i
          //after adjusting for HFN at the very end.  This will take care of N302+1 as well
          //as any roll-oer adjustment due to act-time becoming 0
          if(0 == tdsrrcsmc_integrity_protection_info.ul_act_seq_num[CCCH_RADIO_BEARER_ID])
          {
            rb_0_act_time_jump = 0xF;
          }
          else
          {
            rb_0_act_time_jump = 
              (tdsrrcsmc_integrity_protection_info.ul_act_seq_num[CCCH_RADIO_BEARER_ID] - 1);
          }
    
          //set the UL SN to one less than the act-time, as we will increment the count_i
          //after adjusting for HFN at the very end.  This will take care of N302+1 as well
          //as any roll-oer adjustment due to act-time becoming 0
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
            ul_count_i[CCCH_RADIO_BEARER_ID] =
            ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
             ul_count_i[CCCH_RADIO_BEARER_ID] & TDSRRC_HFN_BIT_MASK) | 
            ( rb_0_act_time_jump & TDSRRC_SEQ_NUM_BIT_MASK));
    
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"SMComp L2 Ack rcvd; SRB0 -> updated RRC-SN: 0x%x; latest-count-I: 0x%x; previousDomain-count-I: 0x%x)",
            rb_0_act_time_jump, tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            ul_count_i[CCCH_RADIO_BEARER_ID], tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
            ul_count_i[CCCH_RADIO_BEARER_ID]);

          /* we need to copy the computed count-I<we need only SN> of _old domain
             to latest domain because if SMC is triggered before cell update 
             then activation SN for SRB0 need to be set using the SN of this latest
			 domain, which will become old domain with next SMC. */
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
            ul_count_i[CCCH_RADIO_BEARER_ID] =
            tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
              ul_count_i[CCCH_RADIO_BEARER_ID];
        }
        if(tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE)
        {
          /*Lets Update L1 with UL cipher keys now.
            Note: We have already updated L1 with DL cipher keys*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Config UL cipher keys");
          tdsrrcsmc_update_cphy_key_req(TDSCIPH_CONFIG_UL_KEYS);
          tdsrrcsmc_send_ul_ciphering_key_req();
        }
   
        /*Setting new_key = FALSE after receiving L2 ACK */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set new_key_for_start_value to FALSE for CN doamin = %d",tdsrrcsmc_int_ciph_info.latest_config_cn_id);
        tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].ciph_keys_info.new_key_for_start_value= FALSE;
  
        /*Update any pending security keys generated in other RAT right here */
        tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
         
        //send resume_req to rlc after getting l2ack_ind
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send RESUME_REQ");
        tdsrrcsmc_send_resume_req();
  
        /*check to see if integrity and ciphering are established.. if only ciphering
          is established, then send cause as ciphering only*/
        if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_CIPHERING_ESTABLISHED);
  
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received != TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
         (tdsrrcsmc_int_ciph_info.integrity_info_received != TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_CIPHERING_ESTABLISHED);
        }
        else
        {
          if(tdsnv_integrity_enabled)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity & ciphering both not started");
          }
          else
          {
            tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
          }
        }
  
        /*received a l2ack, go to initial state*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2ACK for SMC received");
  
        tdsrrcsmc_process_l2_ack_ind_to_dt_procedures();
   
       
        tdsrrcsmc_integrity_protection_info.reconfig = FALSE;
        tdsrrcsmc_substate                    = TDSRRCSMC_INITIAL;
      }
      } 
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring L2-Ack-Ind. Status in L2-Ack Indication set to FAILURE.");
      }
      // Stop timer TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER
      if (tdsrrctmr_get_remaining_time(TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER) > 0)
      {
        tdsrrctmr_stop_timer(TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Stop INTER_SRNS_IGNORE_MR_TIMER");
      }

      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }          
}


/* Not used, replaced by tdsrrc_transition_to_disconnected_state*/
#if 0
/*===========================================================================

FUNCTION tdsrrcsmc_send_rrc_channel_config_req

DESCRIPTION
  This function builds TDSRRC_CHANNEL_CONFIG_REQ command and  requests RRC LLC
  to bring down RRC connection.

DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type: It returns SUCCESS if it successfully sends
  TDSRRC_CHANNEL_CONFIG_REQ command to the RRC LLC. Otherwise it returns
  FAILURE

SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_send_rrc_channel_config_req
(
void
)
{
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to the RRC Command */

  uecomdef_status_e_type status = FAILURE;
  /* The status is initialized to
     Failure */
  /* Allocates the buffer to RRC Internal command */
  if ( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {

    /* Fill in the command parameters for TDSRRC_CHANNEL_CONFIG_REQ */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_SMC;


    /* Fill in parameters for TDSRRC_CHANNEL_CONFIG_REQ command */
    cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
    cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;

    /* No need to receive Channel_config_cnf*/
    cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;

    tdsrrc_put_int_cmd(cmd_ptr);          /* sends the command to RRC LLC. */
    status = SUCCESS;                  /* Status is initialized to Success */
  }
  else  /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Memory alloc failed for Internal Cmd: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
  }
  return(status);
}/*tdsrrcsmc_send_rrc_channel_config_req */
#endif
/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_for_reconfig_cnf_substate

DESCRIPTION
   This function processes the all commands and events to SMC procedure
   in the WAIT_FOR_RECONFIG_CNF_SUBSTATE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_for_reconfig_cnf_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_RECONFIG_COMPLETE_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcvd RECONFIG CNF from LLC");
      
      if (tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
      {

        /*register for a cnf when reconfiguration is completed*/
        tdsrrcllc_register_for_reconfig_cnf();
        /* Wait until Reconfig procedure is completed */
        tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_RECONFIG_CNF;  
      }
      else
      {
        tdsrrcsmc_perform_stop_or_suspend_functionality(TDSRRCSMC_SMC_CMD);
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_ciph_security_mode_cmd_msg

DESCRIPTION
   This function processes the Integrity related IE's of security mode cmd

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_ciph_security_mode_cmd_msg
(
void
)
{
  uint32 i; /*counter in for loops*/

  /*set the latest_configured_cn_domain to the domain_id for which we 
    received security mode command*/
  tdsrrcsmc_int_ciph_info.latest_config_cn_id =  
  tdsrrcsmc_saved_security_mode_command_msg.domain_id;

  /*ciphering on smc message passed.  so set reconfig to true
    and status to started.  send smc complete and wait for l2ack*/
  tdsrrcsmc_int_ciph_info.ciph_status[tdsrrcsmc_int_ciph_info.latest_config_cn_id] = 
  TDSRRCSMC_SECURITY_STARTED;


  if (tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present)
  {
    /*save dpch ciph act_time here*/
    tdsrrcsmc_tm_ciph_act_cfn = (uint8)tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time;
  }

  if (tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time_present)
  {

    tdsrrcsmc_ciphering_info.dl_ciph_act_info.nchan = tdsrrcsmc_saved_security_mode_command_msg.
      num_rb_ciph_dl_act_time;

    memset(tdsrrcsmc_ciphering_info.dl_ciph_act_info.act_time_present, FALSE, TDS_MAX_RB);

    for (i=0; i<tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time; i++)
    {
      /*copy the saved security mode command's Activation times received for
        downlink.. also update the flag which indicates that activation time
        is present for this Radio Bearer*/
      tdsrrcsmc_ciphering_info.dl_ciph_act_info.sq_num_for_complete
        [tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[i].rb_id] =
      tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[i].seq_num;

      tdsrrcsmc_ciphering_info.dl_ciph_act_info.act_time_present
        [tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[i].rb_id] = TRUE;
    }
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_int_security_mode_cmd_msg

DESCRIPTION
   This function processes the Integrity related IE's of security mode cmd

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_int_security_mode_cmd_msg
(
void
)
{

  /*set the activation time pending flag to true if we receive a modify*/
  if (tdsrrcsmc_saved_security_mode_command_msg.modify_chosen)
  {
    /*update activation time info structure*/
    tdsrrcsmc_update_int_activation_time();

    /*set the modify flag to TRUE*/
    tdsrrcsmc_integrity_protection_info.modify_received = TRUE;

    /*Check to see if we got new keys or not and initialize count_i_info for
      RB2 only*/
    if (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        ul_new_key_flag_per_rb[2] == TRUE)
    {
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i[2] = 
      (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
       ul_count_i[2] & TDSRRC_SEQ_NUM_BIT_MASK);

      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_new_key_flag_per_rb[2] = FALSE;
    }
    else
    {
      /*set count_i for the new cn domain by &ing HFN of new CN domain and 
      SEQ_NUM last used for the previous CN domain*/
      tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
      ul_count_i[2] =
      (uint32)((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
        ul_count_i[2] & TDSRRC_HFN_BIT_MASK) |
       (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
        ul_count_i[2] & TDSRRC_SEQ_NUM_BIT_MASK));
    }
  }

  /* integrity check on smc message passed.  So, status to started.
   * Send smc complete and wait for l2ack*/
  tdsrrcsmc_integrity_protection_info.ip_status = TDSRRCSMC_SECURITY_STARTED;        
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_suspend_cnf

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
/*lint -e715*/
void tdsrrcsmc_process_suspend_cnf(tdsrlc_ul_stop_suspend_cnf_type *rrcsmc_suspend_cnf)
{
  uint32 i; 
  if ( (tdsnv_integrity_enabled) &&
       (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE) )
  {
    tdsrrcsmc_process_int_security_mode_cmd_msg();
  }

  /*check if ciphering IE's are sent process this only if present*/
  if(tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE)
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process SUSPEND RLC RB CNF ");

    tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = TRUE;           

    tdsrrcsmc_process_ciph_security_mode_cmd_msg();

    //get the suspend cnf information and set the activation time for all existing
    //rlc am and um channels
    tdsrrcsmc_ciphering_info.ul_ciph_act_info.nchan = rrcsmc_suspend_cnf->nchan;

    for (i=0; i< tdsrrcsmc_ciphering_info.ul_ciph_act_info.nchan; i++)
    {
      tdsrrcsmc_ciphering_info.ul_ciph_act_info.sq_num[rrcsmc_suspend_cnf->cnf[i].lc_id] = 
      rrcsmc_suspend_cnf->cnf[i].rlc_sn + TDSRLC_CIPH_ACT_SN;
  
      tdsrrcsmc_ciphering_info.ul_ciph_act_info.sq_num_for_complete[i] = 
      rrcsmc_suspend_cnf->cnf[i].rlc_sn + TDSRLC_CIPH_ACT_SN;
  
      tdsrrcsmc_ciphering_info.ul_ciph_act_info.lc_id_for_complete[i] = 
      rrcsmc_suspend_cnf->cnf[i].lc_id;
    }

    /* Send SECURITY MODE COMPLETE command */
    tdsrrcsmc_send_security_mode_complete(tdsrrcsmc_saved_security_mode_command_msg.transaction_id);

  }/*check for ciphering IE's are present*/
}
/*lint +e715*/

/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_for_ciph_config_cnf_substate

DESCRIPTION
   This function processes the all commands and events to SMC procedure
   in the WAIT_FOR_CIPH_CONFIG_SUBSTATE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_for_ciph_config_cnf_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:
      if (SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
      {
        tdsl2_ack_rcvd_for_smc_complete_flag = TRUE;
        /*received a l2ack, set l1_ack ind rcvd. flag to true.. this will be processed
        lates when ciph_config_cnf is received by rrc*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2ACK for SMC received");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring L2-Ack Indication from RLC as l2-ack-status set to failure");
      }
      break;

    case TDSRRC_CIPH_CONFIG_CNF:

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcvd CIPH CNF from LLC");
      
      /* check whether we need to revert back to old ciphering variables or not */
      if (TRUE == tdsis_revert_to_old_cipher_variable_reqd)
      {
        /* Revert the ciphering variables */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reverting to old ciphering variables in ciph_config_cnf substate");
        tdsrrcsmc_revert_to_old_security_variables();

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting tdsis_revert_to_old_cipher_variable_reqd to FALSE");
        tdsis_revert_to_old_cipher_variable_reqd = FALSE;

        if(cmd_ptr->cmd.rrc_cipher_config_cnf.cause == TDSRRC_CIPHER_CONFIG_FAIL_DIDNT_CONFIG_LL)
        {
          /*This case is cell reselction clash with smc, LLC would have not configured LL with 
          *cipher config req, So we need to simply resume suspended RB's
          */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcvd CIPH CNF with cause FAIL_DIDNT_CONFIG_LL from LLC");
          tdsrrcsmc_send_resume_req();
        }
        else
        {
          /* Send Abort cipher config req to llc */
          if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
          {
            /* Fill in the command parameters for TDSRRC_CIPH_CONFIG_REQ*/
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_ABORT_CIPH_CONFIG_REQ;
            int_cmd_ptr->cmd.rrc_abort_ciph_config_req.abort_action = TDSRRC_ABORT_CIPH_ACTION_L1KEY_RLC_RESUME;
  
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending TDSRRC_ABORT_CIPH_CONFIG_REQ to LLC in Ciph_Config_Cnf substate. Abort-action: %d",int_cmd_ptr->cmd.rrc_abort_ciph_config_req.abort_action);
      
            tdsrrc_put_int_cmd(int_cmd_ptr);
          }
          else
          {
            ERR_FATAL("Unable to allocate memory for: %d cmd",TDSRRC_ABORT_CIPH_CONFIG_REQ,0,0);
          }
        }

        tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
        break;
      }
      
      /*if l2_ack has already been received while in wait_for_ciph_config_cnf substate
        then take smc to initial and update dt procedures with l2_ack ind for smc*/
      if(tdsl2_ack_rcvd_for_smc_complete_flag == TRUE)
      {
        if(tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE)
        {
          /*Lets Update L1 with UL cipher keys now.
            Note: We have already updated L1 with DL cipher keys*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Config UL cipher keys");
          tdsrrcsmc_update_cphy_key_req(TDSCIPH_CONFIG_UL_KEYS);
          tdsrrcsmc_send_ul_ciphering_key_req();
        }
        /*Update any pending security keys generated in other RAT right here */
        tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
        //send resume req if already received l2_ack for smc-complete
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send RESUME_REQ to RLC");
        tdsrrcsmc_send_resume_req();

        /*check to see if integrity and ciphering are established.. if only ciphering
          is established, then send cause as ciphering only*/
        if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
        (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_CIPHERING_ESTABLISHED);
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received != TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
        }
        else if((tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE) &&
          (tdsrrcsmc_int_ciph_info.integrity_info_received != TRUE))
        {
          tdsrrcsmc_send_sync_ind_to_mm(RRC_CIPHERING_ESTABLISHED);
        }
        else
        {
          if(tdsnv_integrity_enabled)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity & ciphering both not started");
          }
          else
          {
            tdsrrcsmc_send_sync_ind_to_mm(RRC_INTEGRITY_ESTABLISHED);
          }
        }

        tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
        
        /*set the reconfig flag to false as we got the l2_ack for 
          already in ciph_config_substate */
        tdsrrcsmc_integrity_protection_info.reconfig        = FALSE;
    
        /*send ind to dt procedures that l2 ack for smc has been rcvd. and dt
        procedures can send uplink messages*/
        tdsrrcsmc_process_l2_ack_ind_to_dt_procedures();
      }
      else
      {
        tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_L2_ACK;
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_re_establish_rlc_rb_for_srns_relocation

DESCRIPTION
   This function re-establishes rlc radio bearers.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_re_establish_rlc_rb_for_srns_relocation
(
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type, 
  uint32 start_value_srb,
  uint32 start_value_urb
)
{
  tdsrrc_cmd_type *cmd_ptr;
    
  switch(rb_type)
  {
    case TDSRRC_RE_EST_RB_2:

      if ( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {
        /* Fill in the command parameters  */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_RE_EST_RLC_REQ;
        
        cmd_ptr->cmd.rrc_re_est_req.am_config.rb_type = TDSRRC_RE_EST_RB_2;
        cmd_ptr->cmd.rrc_re_est_req.am_config.hfn_srb = start_value_srb;    
        
        //if latest domain is PS, then copy ciph algo for CS and PS to CS
        //domain
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_PS_DOMAIN_CN_ID)
        {
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb =
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];
        }
        //else copy URB to PS domain and SRB to CS domain
        else
        {
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb =
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID];
        }

        tdsrrc_put_int_cmd(cmd_ptr);  

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Re-est for RB2 Algo SRB %d URB %d",cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb,cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_urb);
      }
      else
      {
        ERR_FATAL("Unable to allocate mem for RLC_FOR_SRNS_REQ",0,0,0);
      }  

      break;
    
    case TDSRRC_RE_EST_NON_RB_2_UM_AM:

      if ( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {
        /* Fill in the command parameters  */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_RE_EST_RLC_REQ;

        cmd_ptr->cmd.rrc_re_est_req.am_config.rb_type = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        
        cmd_ptr->cmd.rrc_re_est_req.am_config.hfn_srb = start_value_srb;    
        cmd_ptr->cmd.rrc_re_est_req.am_config.hfn_urb = start_value_urb;          
      
        cmd_ptr->cmd.rrc_re_est_req.ul_um_config.rb_type = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        
        cmd_ptr->cmd.rrc_re_est_req.ul_um_config.hfn_srb = start_value_srb<<5;    
        cmd_ptr->cmd.rrc_re_est_req.ul_um_config.hfn_urb = start_value_urb<<5;                
      
        cmd_ptr->cmd.rrc_re_est_req.dl_um_config.rb_type = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        
        cmd_ptr->cmd.rrc_re_est_req.dl_um_config.hfn_srb = start_value_srb<<5;    
        cmd_ptr->cmd.rrc_re_est_req.dl_um_config.hfn_urb = start_value_urb<<5;                    
        
        //if latest domain is PS, then copy ciph algo for CS and PS to CS
        //domain
        if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_PS_DOMAIN_CN_ID)
        {
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_urb =
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb =
          cmd_ptr->cmd.rrc_re_est_req.ul_um_config.ciph_algo_srb =
          cmd_ptr->cmd.rrc_re_est_req.dl_um_config.ciph_algo_srb =
          cmd_ptr->cmd.rrc_re_est_req.ul_um_config.ciph_algo_urb =
          cmd_ptr->cmd.rrc_re_est_req.dl_um_config.ciph_algo_urb =
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];
        }
        //else copy URB to PS domain and SRB to CS domain
        else
        {
          cmd_ptr->cmd.rrc_re_est_req.ul_um_config.ciph_algo_urb =
          cmd_ptr->cmd.rrc_re_est_req.dl_um_config.ciph_algo_urb =
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_urb =
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID];

          cmd_ptr->cmd.rrc_re_est_req.ul_um_config.ciph_algo_srb =
          cmd_ptr->cmd.rrc_re_est_req.dl_um_config.ciph_algo_srb =
          cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb =
            tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID];
        }

        tdsrrc_put_int_cmd(cmd_ptr);  
        
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Re-est for Non-RB2 Algo SRB %d URB %d",cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_srb,cmd_ptr->cmd.rrc_re_est_req.am_config.ciph_algo_urb);
      }
      else
      {
        ERR_FATAL("Unable to allocate mem for RLC_FOR_SRNS_REQ",0,0,0);
      }  
        
      break;
    
    default:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported re-est cause");
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_srns_reloc_l2_ack_ind_substate    

DESCRIPTION
   This function processes the all commands and events to SMC procedure
   in the WAIT_SRNS_RELOC_L2_ACK_IND_SUBSTATE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_srns_reloc_l2_ack_ind_substate    
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  tdsrrcsmc_re_est_start_list  start_list;

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_RE_EST_RLC_FOR_SRNS_REQ:
    
        //check if case is for re-est rb2 only, in this state non-rb-2 
        //re-establishment is not supported.
        if(cmd_ptr->cmd.rrc_re_est_srns_req.rb_type != TDSRRC_RE_EST_NON_RB_2_UM_AM)
        {
          //send cnf with failure
          if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
          {
            /* Fill in the command parameters for TDSRRC_RE_EST_RLC_FOR_SRNS_CNF */
            int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RE_EST_RLC_FOR_SRNS_CNF;
            int_cmd_ptr->cmd.rrc_re_est_srns_cnf.status = FAILURE;
            tdsrrc_put_int_cmd(int_cmd_ptr);  
      
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Re-Est Fail-RB2");
          }      
        }
        else
        {
          tdsrrcsmc_saved_srns_info.rb_type = TDSRRC_RE_EST_NON_RB_2_UM_AM;
        }
        
        //if re_est cnf was needed, set flag to true, else false
        if(cmd_ptr->cmd.rrc_re_est_srns_req.cnf_reqd == TRUE)
        {
          tdsrrcsmc_saved_srns_info.reconfig_cnf_req = TRUE;
          //save the procedure name to send cnf
          tdsrrcsmc_saved_srns_info.proc_type = cmd_ptr->cmd.rrc_re_est_srns_req.rrc_proc;
        }
        else
        {
          tdsrrcsmc_saved_srns_info.reconfig_cnf_req = FALSE;    
        }        
        
        tdsrrcsmc_get_latest_transmitted_start_values(&start_list);
        
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Re-EST Non-RB2 HFN_SRB %d,HFN_URB %d",start_list.hfn_srb,start_list.hfn_rab);

        //START value and hfn are same for RB2      
        tdsrrcsmc_re_establish_rlc_rb_for_srns_relocation(
          TDSRRC_RE_EST_NON_RB_2_UM_AM, 
          start_list.hfn_srb,
          start_list.hfn_rab); 

        // Start TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER
        tdsrrctmr_start_timer(TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER, (unsigned long)TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER_VAL);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start INTER_SRNS_IGNORE_MR_TIMER %d",TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER_VAL);

        tdsrrcsmc_substate = TDSRRCSMC_WAIT_RE_EST_SRNS_CNF;
      break;


    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_re_est_srns_substate

DESCRIPTION
   This function processes the all commands and events to SMC procedure
   in the WAIT_RE_EST_SRNS_SUBSTATE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_re_est_srns_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  tdsrrc_RB_Identity rb_id;
  uint32 local_sn = 0;
  uint32 local_old_sn = 0;
  uint32 local_hfn = 0;
  /*boolean local_sn_rollover = FALSE;*/
  tdsrrc_cmd_type *int_cmd_ptr;
  
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_LLC_RE_EST_RLC_CNF:
      //if rsp was requested, then send the cnf to calling reconfig procedure.
      if(tdsrrcsmc_saved_srns_info.reconfig_cnf_req == TRUE)
      {
        if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {
          /* Fill in the command parameters for TDSRRC_RE_EST_RLC_FOR_SRNS_CNF */
          int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RE_EST_RLC_FOR_SRNS_CNF;
          int_cmd_ptr->cmd.rrc_re_est_srns_cnf.status = SUCCESS;
          int_cmd_ptr->cmd.rrc_re_est_srns_cnf.rrc_proc = tdsrrcsmc_saved_srns_info.proc_type;
          tdsrrc_put_int_cmd(int_cmd_ptr);  
    
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Re-Est Success");
          
          tdsrrcsmc_saved_srns_info.reconfig_cnf_req = FALSE;
        }      
        else
        {
          ERR_FATAL("Malloc Failed for RE_EST_RLC_CNF",0,0,0);
        }
      }
      
      //if cause of doing re_est was for all rb's send resume cnf
      if(tdsrrcsmc_saved_srns_info.rb_type == TDSRRC_RE_EST_RB_2)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Re-EST of RB-2 done,Continue SRB2");

        /*Continue SRB2 as we stopped all RBs when LLC get activation time ind*/
        tdsrrcsmc_send_continue_req_srns(TDSRRCSMC_RESUME_SRB2);
        //for rb2 procedure go to wait_for_srns_reloc_ind
        tdsrrcsmc_substate = TDSRRCSMC_WAIT_SRNS_RELOC_L2_ACK_IND;      
           
#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clear tdsrrcsmc_srns_fresh_pending_state");
        tdsrrcsmc_srns_fresh_pending_state = FALSE;
#endif 

      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Re-EST of other RB's done");
        //for other rb's send continue_req and go to initial substate
        tdsrrcsmc_send_continue_req(); 
        /*equivalent to tdsrrcsmc_send_continue_req_srns(TDSRRCSMC_RESUME_NON_TM_RB). replace later if needed*/
        
#ifdef FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clear tdsrrcsmc_srb3_4_rlc_pending_state,and pop NAS buffered DDT");
        tdsrrcsmc_srb3_4_rlc_pending_state = FALSE;
        tdsrrcsmc_send_rlc_re_est_cmp();
#endif         
                
#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
        #error code not present
#endif

        if(tdsrrcsmc_saved_srns_info.ciph_algo_present == TRUE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"S_SMC_Algo:%d,S_SRNS_Algo:%d",tdsrrcsmc_saved_security_mode_command_msg.ciph_algo,tdsrrcsmc_saved_srns_info.ciph_algo);

          tdsrrcsmc_saved_security_mode_command_msg.ciph_algo = tdsrrcsmc_saved_srns_info.ciph_algo;
        }
        tdsrrcsmc_ciphering_info.reconfig = FALSE;

      /*Due to separated SRB cn domain management, insert advance pending SRB1, SRB3~4 SMC config here. 
        SRB2's pendign config has been advanced to non-pending in tdsrrcsmc_check_integrity_for_srns_relocation.

        Before, the cn domain will be updated with SRB2. However, it fails the DT test, where the non-SRB2
        SN is advanced by more than 2 in the SMC modify.
        */



     /*check if we received modify, then also check if activation time is pending
      for this rb_id, if yes for both, then sync to new configuration immediately,
      no need to wait until activation time*/
         for(rb_id = 0; rb_id < TDSRRC_MAX_SRB; rb_id++)
         {
            /*No need for SRB2, since it has been processed in tdsrrcsmc_check_integrity_for_srns_relocation already*/
            if ( (rb_id != DCCH_AM_RADIO_BEARER_ID) && ( tdsrrcsmc_integrity_protection_info.modify_received == TRUE) &&
                 ( tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                   dl_count_i_act_time_pending[rb_id] == TRUE) )
            {
              /*reset the activation time pending flag*/
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
              dl_count_i_act_time_pending[rb_id] = FALSE;
              tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[rb_id] = FALSE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Pending SMC modify before SRNS relocation. RESET the dl_count_i_act_time_pending for SRB%d",rb_id);

              /*set the default init flag at later part*/

        
              /*if new keys have been received for this domain, then set the HFN part of
                  count_i to 0, and continue using tdsrrcllc_seq_num*/
              if (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                  dl_new_key_flag_per_rb[rb_id]==TRUE)
              {
                /*reset new keys flag*/
                tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                dl_new_key_flag_per_rb[rb_id]=FALSE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Pending SMC New Key before SRNS relocation. RESET the dl_new_key_flag_per_rb for SRB%d",rb_id);
              }
        
              /*generate count_i with HFN part set to 0 and old domain SEQ_NUM*/
              
              /*Advance the SN for non-SRB2 in latest_config_cn_id_old to pending activation time -1.
               *This is based on spec: 8.6.3.5.2
               *Do Not touch HFN since SRB2 will make it for the latest cn domain. 
               *Rollover type 1: advanced to SN=13 -> recv SN = 1 will be taken care of by the next DL integrity check* 
               *Rollover type 2: advanced to SN=0  -> recv SN = 1 (advance causing rollover) need to be taken care of here* 
               *For type2, need to update the latest_config_cn_id.dl_count_i. Basic idea:* 
               *assuming the advance of SN is similar to receiving msg on the pending SMC config */


              /*step 1. NULL.
               *step 2. check rollover for SN in latest_config_cn_id_old (may not be necessary).
               *step 3. update the SN in latest_config_cn_id also. */

              local_old_sn =  tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                  dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK;

              local_hfn= tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                  dl_count_i[rb_id]; 


              local_sn = tdsrrcsmc_integrity_protection_info.dl_act_seq_num[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK;

              /*Very special case that the act_seq_num for never used SRB maintains 0 , should not revert in that case*/
              if((tdsrrcsmc_integrity_protection_info.count_i_info[RRC_CS_DOMAIN_CN_ID].
                   dl_count_i_init_flag[rb_id] == FALSE)&&
                 (tdsrrcsmc_integrity_protection_info.count_i_info[RRC_PS_DOMAIN_CN_ID].
                   dl_count_i_init_flag[rb_id] == FALSE)&&
                 (local_sn == 0)&&(local_old_sn == 0))
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending SMC modify before SRNS relocation: DO NOT check rollover in activation time not move and always = 0 case");
              }
              else
              {
                /*do the revert of SN only if the SRB has been used at least once*/
              local_sn = (local_sn == 0)?0xF:(local_sn - 1);
              }
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Pending SMC modify before SRNS relocation, rb_id: %d, old count-I: 0x%x, local_sn: 0x%x", rb_id, local_hfn, local_sn);
              
              /*set the default init flag, put here due to above check*/
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
              dl_count_i_init_flag[rb_id] = FALSE;              
                            
              if (local_old_sn > local_sn ) 
              {
                /*increment hfn only*/
                local_hfn >>=4;
                local_hfn++;
                local_hfn <<=4;
                /*local_sn_rollover = TRUE;*/
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Pending SMC modify before SRNS relocation, old SN %d, advanced to SN %d old HFN after rollover 0x%x",local_old_sn, local_sn, local_hfn);			
              }

              /*update old count-I*/
              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                  dl_count_i[rb_id] =(uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
                           (local_sn & TDSRRC_SEQ_NUM_BIT_MASK) 
                           );
              TDSRRC_MSG3(MSG_LEGACY_HIGH, "Pending SMC modify before SRNS relocation. Set latest_config_cn_id_old.dl_count_i[rb_id: %d]: 0x%x, CN:%d", 
                    rb_id,
                    tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id_old].
                         dl_count_i[rb_id], tdsrrcsmc_int_ciph_info.latest_config_cn_id_old); 

              /*Passing the SN only into the latest_config_cn_id.dl_count_i 
                Do not touch the HFN part, which will be update when the activation time reached*/

              /*tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                  dl_count_i[rb_id] =
              ((tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                   dl_count_i[rb_id] & TDSRRC_HFN_BIT_MASK) |
               (local_sn & TDSRRC_SEQ_NUM_BIT_MASK)); */


              tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                  dl_count_i[rb_id] = 
                tdsrrcsmc_convert_START_to_count_c_i(
                  tdsrrcsmc_int_ciph_info.int_ciph_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].hfn,
                  (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                   latest_config_cn_id_old].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK) );

              /*Downlink check will take care of such rollover*/
			  /*
              if(local_sn_rollover)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH, "Updating HFN part for latest_config_cn_id due to rollover,CN:%d",tdsrrcsmc_int_ciph_info.latest_config_cn_id);
                local_hfn= tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                  dl_count_i[rb_id];
                
                local_hfn >>=4;
                local_hfn++;
                local_hfn <<=4;
                tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                  dl_count_i[rb_id] =(uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
                           (local_sn & TDSRRC_SEQ_NUM_BIT_MASK) 
                           );

              }       
			  */


             /*init the tdsrrcsmc_latest_used_ip_key for rb_id != 2, as the pending is removed*/
             memscpy(tdsrrcsmc_latest_used_ip_key[rb_id], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
                tdsrrcsmc_last_committed_ip_key[tdsrrcsmc_int_ciph_info.latest_config_cn_id], 
                RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));

              TDSRRC_MSG1(MSG_LEGACY_HIGH, "Update the tdsrrcsmc_latest_used_ip_key,rb:%d pending IK becomes current",rb_id);


              TDSRRC_MSG3(MSG_LEGACY_HIGH, "Pending SMC modify before SRNS relocation. Set latest_config_cn_id.dl_count_i[rb_id: %d]: 0x%x, CN:%d", 
                    rb_id,
                    tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
                    dl_count_i[rb_id],tdsrrcsmc_int_ciph_info.latest_config_cn_id);             
                      
            }
            /*pending removed when checking srns integrity on that SRB (!=2)*/
            else if((tdsrrcsmc_non_srb2_srns_ota_pending_key_rb_id == rb_id) && (rb_id != DCCH_AM_RADIO_BEARER_ID))
            {
              tdsrrcsmc_non_srb2_srns_ota_pending_key_rb_id = 0xFF;
              memscpy(tdsrrcsmc_latest_used_ip_key[rb_id], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
                tdsrrcsmc_last_committed_ip_key[tdsrrcsmc_int_ciph_info.latest_config_cn_id], 
                RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
              TDSRRC_MSG1(MSG_LEGACY_ERROR, "This is rare case! Update the tdsrrcsmc_latest_used_ip_key,rb:%d srns triggered on non-SRB2",rb_id);
 
            }
         }

        tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_suspend_srns_cnf

DESCRIPTION
  This function performs all the functionality needed for SRNS relocation case 
  after suspending all AM & UM Uplink RLC RB's except-SRB-2/SRB-0.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_process_suspend_srns_cnf
(
  void
)
{
  /*rlc_hfn_info_type to calculate tdsstart_value_to_transmit*/
  tdsrlc_hfn_req_type local_hfn_type;
  tdsrlc_pending_hfn_req_type local_pending_hfn_type;
  uint32 ul_rb2_hfn=0;
  uint32 dl_rb2_hfn=0;
  uint32 i, rb2_hfn;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Process STOP CNF from LLC");
  tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = TRUE;           

  if (SUCCESS == tdsrlc_rrc_get_latest_pending_hfn(
      tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID),
      &local_pending_hfn_type))
  {
    /* inside this if check indicates that either UL or DL or both
       have a pending ciphering configuration */
    if (TRUE == local_pending_hfn_type.ul_pending_hfn_present)
    {
      ul_rb2_hfn = local_pending_hfn_type.ul_pending_hfn;
    }
    if (TRUE == local_pending_hfn_type.dl_pending_hfn_present)
    {
      dl_rb2_hfn = local_pending_hfn_type.dl_pending_hfn;
    }
  }

  /*get the latest RLC HFN's*/
  (void) tdsrlc_rrc_get_hfn_req(&local_hfn_type);

  /* update HFN only if there is no pending UL HFN */
  if (FALSE == local_pending_hfn_type.ul_pending_hfn_present)
  {
    //get ul HFN
    for (i=0; i < local_hfn_type.ul_nchan; i++)
    {
      if(local_hfn_type.hfn_per_rb_ul[i].rlc_id == 
          tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID))
      {
        ul_rb2_hfn = local_hfn_type.hfn_per_rb_ul[i].hfn;
        break;          
      }
    }
  }

  /* update HFN only if there is no pending DL HFN */
  if (FALSE == local_pending_hfn_type.dl_pending_hfn_present)
  {
    //get dl HFN
    for (i=0; i < local_hfn_type.dl_nchan; i++)
    {
      if(local_hfn_type.hfn_per_rb_dl[i].rlc_id == 
         tdsrrclcm_get_dl_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID))
      {
        dl_rb2_hfn = local_hfn_type.hfn_per_rb_dl[i].hfn;
        break;
      }
    }
  }

  //get the max of ul/dl and add 1 to it.. HFN calculation
  //according to 25.331
  if(ul_rb2_hfn > dl_rb2_hfn)
  {
    rb2_hfn = ul_rb2_hfn+1;
  }
  else
  {
    rb2_hfn = dl_rb2_hfn+1;      
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RB2 re-est: SRB2-HFNs in UL 0x%x DL 0x%x Config HFN 0x%x", ul_rb2_hfn, dl_rb2_hfn, rb2_hfn);
      
  //START value and hfn are same for RB2      
  tdsrrcsmc_re_establish_rlc_rb_for_srns_relocation(TDSRRC_RE_EST_RB_2, rb2_hfn, rb2_hfn);
  tdsrrcsmc_substate = TDSRRCSMC_WAIT_RE_EST_SRNS_CNF;
  
}

/*===========================================================================

FUNCTION tdsrrcsmc_is_rlc_re_est_for_srns_active

DESCRIPTION
  This function returns the status of SMC doing RLC re-establishment.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcsmc_is_rlc_re_est_for_srns_active()
{
  if((tdsrrcsmc_substate == TDSRRCSMC_WAIT_SRNS_RELOC_L2_ACK_IND) ||
     (tdsrrcsmc_substate == TDSRRCSMC_WAIT_RE_EST_SRNS_CNF))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_save_contents_of_security_mode_command

DESCRIPTION
   This function processes the SECURITY MODE COMMAND received on DL DCCH
   and extracts the Transaction ID.  Sends the SYNC_IND cmd to MM and
   also calls the function to send SECURITY MODE COMPLETE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_save_contents_of_security_mode_command
(
tdsrrc_DL_DCCH_Message *dcch_msg_ptr
)
{

  uint32 i, n_index=0, n_value=0, rb_index = 0, count=0; /*used for counter purposes*/
  struct tdsrrc_SecurityModeCommand_r3_IEs * local_smc_msg_ptr;

  /*local var to store act time linked list ptr*/
  struct tdsrrc_RRC_MessageSequenceNumberList  *local_smc_act_list;

  struct tdsrrc_RB_ActivationTimeInfoList  *local_rb_ciph_act_time;

  /*copy indirected link to a local ptr*/
  local_smc_msg_ptr = &(dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                        securityModeCommand_r3);

  /*save transaction id*/
  tdsrrcsmc_saved_security_mode_command_msg.transaction_id = local_smc_msg_ptr->
                                                   rrc_TransactionIdentifier;

  /*copy domain id*/
  if(local_smc_msg_ptr->cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) 
  {
    tdsrrcsmc_saved_security_mode_command_msg.domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else
  {
    tdsrrcsmc_saved_security_mode_command_msg.domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  if(tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE)
  {

    /*check if start or modify is chosen and update accordingly*/
    if (local_smc_msg_ptr->integrityProtectionModeInfo.
        integrityProtectionModeCommand.t ==  T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection )
    {
      tdsrrcsmc_saved_security_mode_command_msg.start_chosen = TRUE;
      tdsrrcsmc_saved_security_mode_command_msg.modify_chosen = FALSE;
  
      /*get the fresh value*/
      tdsrrcsmc_saved_security_mode_command_msg.start = 
      b_unpackd(local_smc_msg_ptr->integrityProtectionModeInfo.
                integrityProtectionModeCommand.u.startIntegrityProtection->
                integrityProtInitNumber.data,
                TDSRRC_DEFAULT_BIT_POS,
                TDSRRC_FRESH_LENGTH_IN_BITS);  

      //save the fresh value for srns relocation purposes..
      tdsrrcsmc_saved_srns_info.fresh_old = tdsrrcsmc_saved_security_mode_command_msg.start;

      /* Since its the first time FRESH value is provided to the UE, hence
         there is no old1 fresh value. Hence, saving the received FRESH 
         value in fresh_old1 as well. This stored value is needed to revert back to old 
         configuration after SRNS relocation physChannelEstFailure */
      tdsrrcsmc_saved_srns_info.fresh_old1 = tdsrrcsmc_saved_security_mode_command_msg.start;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saved FRESH is 0x%x",tdsrrcsmc_saved_security_mode_command_msg.start);

      tdsrrcsmc_saved_security_mode_command_msg.start_chosen = FALSE;
      tdsrrcsmc_saved_security_mode_command_msg.modify_chosen = TRUE;
    
      /*initialize seqNumList*/
      local_smc_act_list = &(local_smc_msg_ptr->integrityProtectionModeInfo.
        integrityProtectionModeCommand.u.modify->dl_IntegrityProtActivationInfo.
        rrc_MessageSequenceNumberList);

      /*initialize count_i for NEW latest CN domain*/
      n_index = 0;
      n_value = local_smc_act_list->n;
      
      for ( i=0; i < TDSRRC_MAX_SRB; i++)
      {
        if(n_value != 0)
        {
        /*initialize downlink count_i dl_activation_info time sent by utran
          for this domain */
        tdsrrcsmc_saved_security_mode_command_msg.srb_int_dl_act_time[i].seq_num = 
          (uint8)(local_smc_act_list->elem[n_index]);
  
          n_value--;
          n_index++;
        }
      }
    }
    
    /*save the integrity algorithm sent*/
    tdsrrcsmc_saved_security_mode_command_msg.integrity_algo = 
          tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &local_smc_msg_ptr->integrityProtectionModeInfo.integrityProtectionAlgorithm,
             TDSMSG_REL99);

    tdsrrcsmc_int_ciph_info.saved_integrity_algo[tdsrrcsmc_saved_security_mode_command_msg.domain_id] 
       = tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &local_smc_msg_ptr->integrityProtectionModeInfo.integrityProtectionAlgorithm,
             TDSMSG_REL99);

  }

  if((tdsnv_ciphering_enabled) &&
     (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE))
   {
  
    /*save the ciphering algorithm sent*/
    tdsrrcsmc_saved_security_mode_command_msg.ciph_algo = 
          tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
              &(local_smc_msg_ptr->cipheringModeInfo.cipheringModeCommand.u.startRestart),
              TDSMSG_REL99);
  
    /*save to temp var*/
    tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_saved_security_mode_command_msg.domain_id] = 
          tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
              &(local_smc_msg_ptr->cipheringModeInfo.cipheringModeCommand.u.startRestart),
              TDSMSG_REL99);
  
    /*copy activation time for DPCH if sent*/
    if (local_smc_msg_ptr->cipheringModeInfo.m.activationTimeForDPCHPresent)
    {
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present = TRUE;
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time = 
        local_smc_msg_ptr->cipheringModeInfo.activationTimeForDPCH;
    }
    else
    {
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present = FALSE;
    }
  
    if (local_smc_msg_ptr->cipheringModeInfo.m.rb_DL_CiphActivationTimeInfoPresent)
    {
      /*copy Radio Bearer DL activation time for all UM/AM RB's*/
      local_rb_ciph_act_time = &(local_smc_msg_ptr->cipheringModeInfo.
                               rb_DL_CiphActivationTimeInfo);
  
      /*set the flag to true*/
      tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time_present = TRUE;
  
      /*init local vars used for counter purposes to zero*/
      tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time = 0;
  
      n_index= 0;
  
      /*in a while loop, copy the number of DL activation time for each
        radio bearer*/
      // 2010/09/10 KW
	  while((local_rb_ciph_act_time->n > n_index) && 
              (n_index < (UE_MAX_DL_UM_CHANNEL + UE_MAX_AM_ENTITY)) &&
			  (tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time < (UE_MAX_DL_UM_CHANNEL + UE_MAX_AM_ENTITY)))
      {
        if((local_rb_ciph_act_time->elem[n_index].rb_Identity == DCCH_UM_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[n_index].rb_Identity == DCCH_AM_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[n_index].rb_Identity == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[n_index].rb_Identity == DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
        {
          tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].rb_id = (uint8) 
            local_rb_ciph_act_time->elem[n_index].rb_Identity;
          tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].seq_num = (uint16)
            local_rb_ciph_act_time->elem[n_index].rlc_SequenceNumber;           
          tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time++;
        }
        else
        {
          for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
          {
            if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
            {
              for (rb_index = 0; rb_index < tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; rb_index++)
              {
                if (tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_status ==
                    TDSRRC_RB_STARTED)
                {
                  /* Check to see if this RB info is included in the ciph activation times list */
                  // for KW
                  if((tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id ==
                        local_rb_ciph_act_time->elem[n_index].rb_Identity)&&
                         (tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time < 
                      (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY)))
                  {
                    tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].rb_id = (uint8) 
                      local_rb_ciph_act_time->elem[n_index].rb_Identity;
                    tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].seq_num = (uint16)
                      local_rb_ciph_act_time->elem[n_index].rlc_SequenceNumber;           
                    tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time++;
                  }
                }
              }
            }
          }           
        }
        n_index++;
      }

    }
  }
}
#if defined (FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*===========================================================================

FUNCTION tdsrrcsmc_step_2_ho_ciph_req

DESCRIPTION
  Process Ciph Config Cnf in Step_2_Ciph_config_cnf substate

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_step_2_ho_ciph_req
( 
  uint16 step_2_ciph_act_time
)
{
  uint32 step_2_hfn;

  //set ul/dl rlc cipher info to 0's
  tdscipher_config_db.rlc_ul_cipher_info.nchan=0;
  tdscipher_config_db.rlc_dl_cipher_info.nchan=0;

  /*get the HFN for Step_2 HFN calculation*/
  step_2_hfn = tdsrrcsmc_convert_START_to_count_c_i(
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].hfn, 0);

  /*set the HFN value for MAC to the step_2 HFN value.. also set the config
    type enum to STEP_2.. and also copy the ciph_act_time for mac to the new
    activation time sent in HO_TO_UTRAN_COMPLETE message
  we don't need to increment HFN+1 for MAC, as MAC layer takes care of it
  once we hit activation time*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSHO_STEP2_CIPHER_CFG;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].hfn = 
    step_2_hfn >> TDSRRC_MAC_HFN_OFFSET;
    
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].ciph_act_time = 
    step_2_ciph_act_time;
      
  /*call llc function to update rlc/mac ciphering configuration for
    starting STEP_2 ciphering*/
  tdsrrcllc_update_ciphering_config_info_for_handover();

  /*reset the cipher config type to non-HO*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_CIPHER_CFG;

  return SUCCESS;
}
/*===========================================================================

FUNCTION tdsrrcsmc_send_ho_ciph_config_cnf

DESCRIPTION
  Process Ciph Config Cnf in Step_1_Ciph_config_cnf substate

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE

===========================================================================*/
static void tdsrrcsmc_send_ho_ciph_config_cnf
(
  uecomdef_status_e_type status
)
{
  tdsrrc_cmd_type *int_cmd_ptr;        /*cmd ptr to send ciph config req*/

  /*send cipher config req to llc, llc configures lower layers
    with ciphering information*/
  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for TDSRRC_CIPH_CONFIG_REQ*/
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_HO_CIPH_CNF;

    /*set the status to what was received*/
    int_cmd_ptr->cmd.rrc_ho_ciph_cnf.status = status;
    
    /*put it in internal queue*/
    tdsrrc_put_int_cmd(int_cmd_ptr);  

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_HO_CIPH_CNF to IHO");
  }
  else
  {
    ERR_FATAL("Unable to allocate memory for: %d cmd",TDSRRC_HO_CIPH_CNF,0,0);
  }  
  
  /*reset the cipher config info for CS domain..  for all cases we should always
    have NON_HO_CIPHER_CFG except for brief periods when we are doing Step1 and
    Step2 Ciphering Configuration for Handover*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSNON_HO_CIPHER_CFG;  
}/*tdsrrcsmc_send_ho_ciph_config_cnf*/
/*===========================================================================

FUNCTION tdsrrcsmc_process_wait_for_step_1_ciph_config_cnf_substate 

DESCRIPTION
  Process Ciph Config Cnf in Step_1_Ciph_config_cnf substate

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_wait_for_step_1_ciph_config_cnf_substate 
(
tdsrrc_cmd_type * cmd_ptr
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CIPH_CONFIG_CNF:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"cvd CIPH CNF from LLC");
      tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
      
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
      /*send ciphering config success to IHO procedure*/
      tdsrrcsmc_send_ho_ciph_config_cnf(SUCCESS);      
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcsmc_process_step_1_ho_ciph_req

DESCRIPTION
  Process Step 1 ciphering for Handover

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_step_1_ho_ciph_req
(
tdsrrc_ho_ciph_req_type  ho_ciph_req
)
{
  tdsrrc_cmd_type *int_cmd_ptr;        /*cmd ptr to send ciph config req*/

  /*reset any previous activation times to 0.. */
  memset(&tdsrrcsmc_ciphering_info.ul_ciph_act_info, 0, 
         sizeof(tdsrrcsmc_ciphering_info.ul_ciph_act_info));
  memset(&tdsrrcsmc_ciphering_info.dl_ciph_act_info, 0, 
         sizeof(tdsrrcsmc_ciphering_info.dl_ciph_act_info));  

#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
if ((CIPH_CONFIG_L2TD == ho_ciph_req.config_cause) || (TDSCIPH_CONFIG_CS_PLUS_PS == ho_ciph_req.config_cause))
  {
      //update latest domain to CS domain as it is a Handover, also update 
    //ciphering algorithm.
    tdsrrcsmc_int_ciph_info.latest_config_cn_id = 
      ho_ciph_req.domain_id;
    tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID] = 
      ho_ciph_req.ciph_algo;
    tdsrrcsmc_int_ciph_info.ciph_status[RRC_PS_DOMAIN_CN_ID] = TDSRRCSMC_SECURITY_STARTED;
    tdsrrcsmc_save_security_key_set_from_lte(ho_ciph_req,RRC_PS_DOMAIN_CN_ID);
  }
#endif

  if((CIPH_CONFIG_G2TD == ho_ciph_req.config_cause)
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
    ||(TDSCIPH_CONFIG_CS == ho_ciph_req.config_cause)||(TDSCIPH_CONFIG_CS_PLUS_PS == ho_ciph_req.config_cause)
#endif
    )
  {
      
  /*set cipher config type to HO_STEP_1*/
  tdscipher_config_db.mac_cipher_info[RRC_CS_DOMAIN_CN_ID].
    cipher_cfg_type = TDSHO_STEP1_CIPHER_CFG;
      
  //update latest domain to CS domain as it is a Handover, also update 
  //ciphering algorithm.
  tdsrrcsmc_int_ciph_info.latest_config_cn_id = RRC_CS_DOMAIN_CN_ID;
  tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID] = ho_ciph_req.ciph_algo;
  tdsrrcsmc_int_ciph_info.ciph_status[RRC_CS_DOMAIN_CN_ID] = TDSRRCSMC_SECURITY_STARTED;

  //set cipher_enable to mac to true, as for HO IHO will call this interface
  //only when ciphering is enabled
  tdscipher_config_db.mac_cipher_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
    cipher_enable = TRUE;
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
    if ((TDSCIPH_CONFIG_CS == ho_ciph_req.config_cause) || (TDSCIPH_CONFIG_CS_PLUS_PS == ho_ciph_req.config_cause))
    {
      tdsrrcsmc_save_security_key_set_from_lte(ho_ciph_req,RRC_CS_DOMAIN_CN_ID);
    }
#endif
  }

  /*send cipher config req to llc, llc configures lower layers
    with ciphering information*/
  if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for TDSRRC_CIPH_CONFIG_REQ*/
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CIPH_CONFIG_REQ;
    int_cmd_ptr->cmd.rrc_ciph_config_req.config_cause = ho_ciph_req.config_cause;

    tdsrrc_put_int_cmd(int_cmd_ptr);  

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending CIPH_CONFIG_REQ to LLC");
  }
  else
  {
    ERR_FATAL("Unable to allocate memory for: %d cmd",TDSRRC_CIPH_CONFIG_REQ,0,0);
  }  

  /*go to wait for ho ciph config cnf substate*/
  tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_HO_STEP_1_CIPH_CONFIG_CNF;
  
}/*tdsrrcsmc_process_step_1_ho_ciph_req*/
#endif /*FEATURE_INTERRAT_HANDOVER_G2TD*/

/*===========================================================================

FUNCTION tdsrrcsmc_fill_crlc_dl_abort_cipher_req

DESCRIPTION
   This function fills the 'crlc_dl_abort_cipher_req' with the DL LC ids for which
   new ciphering configuraiton was given to RLC on receipt of SMC. 

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_fill_crlc_dl_abort_cipher_req
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info_ptr
)
{
  uint32 index;

  abort_cipher_info_ptr->nchan = 
    tdsbackup_of_security_variables.tdscipher_config_db.rlc_dl_cipher_info.nchan;

  for (index=0; index<abort_cipher_info_ptr->nchan; index++)
  {
    abort_cipher_info_ptr->lc_id[index] = 
      tdsbackup_of_security_variables.tdscipher_config_db.rlc_dl_cipher_info.ciphering[index].lc_id;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"ABORT CIPHER REQ for DL-LC ID: %d <total channels: %d>",abort_cipher_info_ptr->lc_id[index],abort_cipher_info_ptr->nchan);
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_fill_crlc_ul_abort_cipher_req

DESCRIPTION
   This function fills the 'crlc_ul_abort_cipher_req' with the UL LC ids for which
   new ciphering configuraiton was given to RLC on receipt of SMC. 

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_fill_crlc_ul_abort_cipher_req
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info_ptr
)
{
  uint32 index;

  abort_cipher_info_ptr->nchan = 
    tdsbackup_of_security_variables.tdscipher_config_db.rlc_ul_cipher_info.nchan;

  for (index=0; index<abort_cipher_info_ptr->nchan; index++)
  {
    abort_cipher_info_ptr->lc_id[index] = 
      tdsbackup_of_security_variables.tdscipher_config_db.rlc_ul_cipher_info.ciphering[index].lc_id;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"ABORT CIPHER REQ for UL-LC ID: %d <total channels: %d>",abort_cipher_info_ptr->lc_id[index],abort_cipher_info_ptr->nchan);
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_check_and_abort_ongoing_security_configuration

DESCRIPTION
   This function checks whether security configuration is in progress or not. 
   If yes, then it will abort the ongoing security configuration by indicating
   LLC to revert to old configuration. This will also indicate LLC as what action
   needs to be taken to abort the security configuration.   

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_check_and_abort_ongoing_security_configuration
(
  void
)
{
  boolean is_ciphering_info_received = FALSE;
  /*local copy of domain id. Set it to cs domain to suppress warning*/
  rrc_cn_domain_identity_e_type domain_id=RRC_CS_DOMAIN_CN_ID;  
  tdsrrc_cmd_type *int_cmd_ptr = NULL;                    /*cmd ptr to send ciph config req*/

  if (TRUE == tdsfeature_rollback_security_config)
  {
    if (TRUE == tdsrrcsmc_is_security_reconfig_active())
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SecReconfig is active. smcSubstate: %d (waitL2Ack: %d )",tdsrrcsmc_substate,TDSRRCSMC_WAIT_FOR_L2_ACK);
    
      if ((TDSRRCSMC_WAIT_FOR_L2_ACK == tdsrrcsmc_substate) || 
          (TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF == tdsrrcsmc_substate))
      {
        /* save the information from the ongoing security configuration, which are
           going to be used for rolling-back to old configuration */
        is_ciphering_info_received = tdsrrcsmc_int_ciph_info.ciphering_info_received;
        domain_id = tdsrrcsmc_int_ciph_info.latest_config_cn_id;
    
        /* revert to saved security variables */
        tdsrrcsmc_revert_to_old_security_variables();

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"latest_config_cn_id new = %d,prev =%d",domain_id,tdsrrcsmc_int_ciph_info.latest_config_cn_id);
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"dl_new_key_flag_per_rb= %d,ciph new_key =%d",tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_new_key_flag_per_rb[2],tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key);
        /* If required revert to old integrity keys */
        if ((TRUE == tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
            dl_new_key_flag_per_rb[2])
           /* In case of security keys received in other RAT dl_new_key_flag_per_rb flag for integrity keys    
              *will not be set to FALSE reason being our current design will set HFN part of count_i to zero 
              *by looking at this flag which is not desirable action as per spec section (8.3.6.3)Reception of HANDOVER TO UTRAN 
              *COMMAND. How ever ciph_keys_info.new_key flag will be set to TRUE for new keys generated in other RAT. With this 
              *spec cr implementation we may have the keys generated in other RAT not used at GtoW hand over.
              *that key may be used in this SMC, so we need make sure that we need to revert back to old I.K. CK is taken care while 
              *reverting back variable tdsrrcsmc_int_ciph_info in above function tdsrrcsmc_revert_to_old_security_variables().This spec cr is valid 
              *for CS domain, so revert I.K for CS domain is ciph_keys_info.new_key flag is TRUE only when previous 
              *tdsrrcsmc_int_ciph_info.latest_config_cn_id & latest  tdsrrcsmc_int_ciph_info.latest_config_cn_id CN domain I.D are CS.
              */
            ||((domain_id == tdsrrcsmc_int_ciph_info.latest_config_cn_id)&&
                 (domain_id == RRC_CS_DOMAIN_CN_ID)&&
                 (tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].
            ciph_keys_info.new_key == TRUE))
        )
        {
          /*
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reverting to old integrity keys");
          tdsrrcf9_init_key_values(
            tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
            tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
          */

          /*Only revert to the affected domain. 
            tdsrrcsmc_revert_to_old_security_variables takes care of the key
            revert.
            Here is for a race condition as there may be concurent NEW key
            arrives during SMC procedure before the L2ACK returns. So revert the
            affected to 
            */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reverting to old integrity keys");
          if(domain_id == RRC_CS_DOMAIN_CN_ID)
          {
            tdsrrcf9_init_key_values(
             tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
             tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
          }
          else
          {
            tdsrrcf9_init_key_values(
             tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
             tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
          }
        }
       
        /* Do not send the Abort_Cipher_Req if the SMC substate is wait_ciph_config_cnf
             It would be sent when SMC gets the ciph_config_cnf in that handler */
        if( TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF != tdsrrcsmc_substate)
        {
          if ((tdsnv_ciphering_enabled) && (TRUE == is_ciphering_info_received))
          {
            /* Send Abort cipher config req to llc */
            if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            {
              /* Fill in the command parameters for TDSRRC_CIPH_CONFIG_REQ*/
              int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_ABORT_CIPH_CONFIG_REQ;
              int_cmd_ptr->cmd.rrc_abort_ciph_config_req.abort_action = 
                                                     TDSRRC_ABORT_CIPH_ACTION_L1KEY_RLC_RESUME;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending TDSRRC_ABORT_CIPH_CONFIG_REQ to LLC. Abort-action: %d",int_cmd_ptr->cmd.rrc_abort_ciph_config_req.abort_action);
    
              tdsrrc_put_int_cmd(int_cmd_ptr);
            }
            else
            {
              ERR_FATAL("Unable to allocate memory for: %d cmd",TDSRRC_ABORT_CIPH_CONFIG_REQ,0,0);
            }
          }
    
          tdsrrcsmc_substate = TDSRRCSMC_INITIAL;
        }
      }
      else
      {
        /* If we reach here that means SMC is in different sub-state NOT because
             of security mode procedure */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No need to revert to old configuration in SMC sub-state: %d",tdsrrcsmc_substate);
      }
    }
  }

}

/*===========================================================================
FUNCTION tdsrrcsmc_update_latest_cn_id

DESCRIPTION
   This function updates the latest received CN domain id in the global.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_update_latest_cn_id(void)
{
  uint8 i = 0;
  /*save the previous CN_DOMAIN we were working with*/
  if (tdsrrcsmc_saved_security_mode_command_msg.modify_chosen)
  {
    /* dont change latest_config_cn_domain_old_1 when it is already in use */
    for (i=0; i<TDSRRC_MAX_SRB;i++)
    {
      if (TRUE == tdsrrcsmc_integrity_protection_info.dl_latest_config_cn_id_old_1_valid[i])
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"dl_latest_config_cn_id_old_1_valid already valid for SRB: %d",i);
        /*break;*/
      }
      else
      {
        tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i] = tdsrrcsmc_int_ciph_info.latest_config_cn_id_old;
        TDSRRC_MSG2(MSG_LEGACY_HIGH, "SRB: %d,latest_config_cn_id_old_1 is updated to: %d",i,tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i]);
      }
      

    }
    /* dont change latest_config_cn_domain_old_1 when it is already in use */
    /*
    if (i == TDSRRC_MAX_SRB)
    {
      tdsrrcsmc_int_ciph_info.latest_config_cn_id_old_1[i] = tdsrrcsmc_int_ciph_info.latest_config_cn_id_old;
    }
    */

    /*save the previous CN_DOMAIN we were working with*/
    tdsrrcsmc_int_ciph_info.latest_config_cn_id_old=tdsrrcsmc_int_ciph_info.latest_config_cn_id;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"latest_cn_id_old: %d,latest_cn_id: %d",tdsrrcsmc_int_ciph_info.latest_config_cn_id_old,tdsrrcsmc_saved_security_mode_command_msg.domain_id);  /*latest_cn_id to be updated to*/
  }

  /*set the tdsrrcsmc_int_ciph_info.latest_config_cn_id to the domain_id for which we
    received security mode command*/
  tdsrrcsmc_int_ciph_info.latest_config_cn_id =  tdsrrcsmc_saved_security_mode_command_msg.domain_id;
}
/*===========================================================================

FUNCTION tdsrrcsmc_validate_security_mode_command_r7

DESCRIPTION
  This function validates security mode command for critical xtensions
  and also to check to see if either ciphering or integrity mode info
  is present in the security mode command
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_security_mode_command_r7
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{
  /*if any security reconfiguration is currently in progress return failure
    indicating that this is a Simultaneous Reconfiguration*/
  if( (tdsrrcsmc_integrity_protection_info.reconfig == TRUE) || 
      (tdsrrcsmc_ciphering_info.reconfig == TRUE)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incompatible Simultaneous Ciph Reconfig");
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG;
    return TDSRRCSMC_INVALID_CONFIGURATION;
  }

  /*if both nv items are not enabled, then do not check any more IE's just
    return success*/
  if((!tdsnv_ciphering_enabled) && (!tdsnv_integrity_enabled))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"NV cipher = %d & NV integrity = %d not enabled",tdsnv_ciphering_enabled,tdsnv_integrity_enabled);
    return TDSRRCSMC_VALID_CONFIG;
  }

  /*Either ciphering or integrity protection mode info ie's should be present
    in message received by UE*/
  if (msg_ptr->message.u.securityModeCommand.u.later_than_r3.criticalExtensions.
        u.r7.securityModeCommand_r7.m.cipheringModeInfoPresent)
  {
    tdsrrcsmc_int_ciph_info.ciphering_info_received = TRUE;
  }
  else
  {
    tdsrrcsmc_int_ciph_info.ciphering_info_received = FALSE;
  }

  if (msg_ptr->message.u.securityModeCommand.u.later_than_r3.criticalExtensions.
        u.r7.securityModeCommand_r7.m.integrityProtectionModeInfoPresent)
  {  
    tdsrrcsmc_int_ciph_info.integrity_info_received = TRUE;
  }
  else
  {
    tdsrrcsmc_int_ciph_info.integrity_info_received = FALSE;
  }

  /* if Ciphering Mode IE's and Integrity Protection Mode IE's
     are not included, then send SMC Failure message */  
  if( (tdsrrcsmc_int_ciph_info.integrity_info_received == FALSE ) &&
      (tdsrrcsmc_int_ciph_info.ciphering_info_received == FALSE )
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplte Configuration NO cipher & integrity info in OTA msg");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;
}/* tdsrrcsmc_validate_security_mode_command */
/*===========================================================================

FUNCTION tdsrrcsmc_validate_ciph_security_mode_command_r7

DESCRIPTION
  This function validates ciphering part of SECURITY MODE COMMAND.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_ciph_security_mode_command_r7
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{
  uint32 count, i, local_lc_id;            /*vars used for count purposes*/
  boolean tm_rb_present;                   /*flag used to check if tm rb's are present*/
  tdsrrc_CN_DomainIdentity local_domain_id;   /*domain id for which ciph config is being changed*/
  uint32 num_srbs_established = 3;         /*SRBs 1,2 and 3 are mandatory*/
  uint32 index=0,n_value=0;

  /*local ptr to Ciph. Mode Info. */
  struct tdsrrc_CipheringModeInfo_r7* ciphModeInfo;

  /* local ptr to activation time list */
  struct tdsrrc_RB_ActivationTimeInfoList  *local_rb_ciph_act_time_list;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity srb5_rb_id = TDSRRCLLC_INVALID_RB_ID;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  /*set to FALSE*/
  tm_rb_present = FALSE;

  /* if Ciphering Mode IE's are not included, then send SMC Failure message */
  if ( (msg_ptr->message.u.securityModeCommand.u.later_than_r3.criticalExtensions.
        u.r7.securityModeCommand_r7.m.cipheringModeInfoPresent) == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplte Configuration");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  /*check if activation times are present in do ciph activation.. they have to be present
    as SRB's are already started and have to be ciphered*/
  if ( (msg_ptr->message.u.securityModeCommand.u.later_than_r3.criticalExtensions.
        u.r7.securityModeCommand_r7.cipheringModeInfo.m.rb_DL_CiphActivationTimeInfoPresent) == 0 )
  {
    /*lint -e793*/
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rrc_CipheringModeInfo_r7_rb_DL_CiphActivationTimeInfo_present not present"); 
    /*lint +e793*/
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  /*get ptr to cipheringModeInfo. copy this to a local ptr to avoid 
    multiple indirections in later manipulations*/
  ciphModeInfo = &(msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.cipheringModeInfo);

  /* Check to see if Ciph Activation time is included for all SRBs.Ignore SRB0
     since it is not ciphered.Check for SRBs 1,2 and 3 since they are mandatory.
     Check for RB4 only if it is already established */
  if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND != tdsrrclcm_check_dl_rlc_lc_id(
                                   UE_LOGCHAN_DCCH,
                                   DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                   UE_MODE_ACKNOWLEDGED_DATA))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"SRB4 established");
    num_srbs_established = 4;
  }

  for(count = 1; count <= num_srbs_established; count++)
  {
    uint32 srb_found = 0;

    /* get the activation time list for RBs */
    local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

    index = 0;
    n_value = local_rb_ciph_act_time_list->n;

    /* Check to see if this RB info is included in the ciph activation times list */
    while (n_value != 0)
    {
      if(count == local_rb_ciph_act_time_list->elem[index].rb_Identity)
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"SRB id: %d included in ciph activation info in SMC",count);
        srb_found++;
      }
      n_value--;
      index++;
    }

    if(srb_found != 1)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"SRB id: %d is included %d times in ciph activation info in SMC",count,srb_found);
      tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
      return TDSRRCSMC_INVALID_CONFIGURATION;
    }
  }
  
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  if(tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &srb5_rb_id) == TDSRRCLCM_SUCCESS)
  {
    /* get the activation time list for RBs */
    local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

    index = 0;
    n_value = local_rb_ciph_act_time_list->n;

    /* Check to see if this RB info is included in the ciph activation times list */
    while (n_value != 0)
    {
      if(srb5_rb_id == local_rb_ciph_act_time_list->elem[index].rb_Identity)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"SMC validation failed; SRB#5 Id = %d,RB ID: %d",srb5_rb_id,local_rb_ciph_act_time_list->elem[index].rb_Identity);
        tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
        return TDSRRCSMC_INVALID_CONFIGURATION;
      }
      n_value--;
      index++;
    }
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  /*get the domain id of the latest configured CN domain for which we are
    configuring ciphering*/
  local_domain_id = msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.cn_DomainIdentity;

  /*check to see if TM radio bearers are present.  only if present will we
    mandatorily check to see DPCH activation time is present.
    Also if the domain id is PS,check if SMC has 
    included all the PS RBs activation time in the message.If not
    UE should send a security mode failure and not honor the SMC */
  for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    {
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if (tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_status ==
            TDSRRC_RB_STARTED)
        {
          /*check if domain id is same as domain for which ciphering
            configuration is received for*/
          if(tdsrrc_est_rabs.rabs[count].cn_domain == local_domain_id)
          {
            if(local_domain_id == tdsrrc_CN_DomainIdentity_ps_domain) 
            {
              uint32 ps_rb_found = 0;

              /* get the activation time list for RBs */
              local_rb_ciph_act_time_list = &(ciphModeInfo->rb_DL_CiphActivationTimeInfo);

              index = 0;
              n_value = local_rb_ciph_act_time_list->n;

              /* Check to see if this RB info is included in the ciph activation times list */
              while (n_value != 0)
              {
                if(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id ==
                    local_rb_ciph_act_time_list->elem[index].rb_Identity)
                {
                  ps_rb_found++;
                }
                n_value--;
                index++;
              }
              if(ps_rb_found != 1)
              {
                TDSRRC_MSG2(MSG_LEGACY_ERROR,"RB id: %d is included %d times in ciph activation info in SMC",tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id,ps_rb_found);
                tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
                return TDSRRCSMC_INVALID_CONFIGURATION;
              }
            }
            else /* CS domain */
            {
              /*copy to local vars to avoid multiple indirections*/
              local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_id);
  
              /*set TM radio bearers flag present to true*/
              if (local_lc_id < TDSRRCLCM_UL_UM_START_IDX)
              {
                tm_rb_present=TRUE;
                break;
              }
            }
          }
        }/*if*/
      }/*for*/
    }/*if*/

    /*if TM radio bearers are present, then break right now as we don't have
      to loop through all rabs in est_rabs*/
    if (tm_rb_present == TRUE)
    {
      break;
    }
  }/*for loop till MAX_RAB_TO_SETUP*/

  /*if only TM radio bearers are present then mandatorily DPCH activation
    information should be present.*/
  if ( ((ciphModeInfo->m.activationTimeForDPCHPresent) == 0) && 
       (tm_rb_present == TRUE )
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No DPCH Activation Time");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPLETE_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;
}/* tdsrrcsmc_validate_ciph_security_mode_command */
/*===========================================================================

FUNCTION tdsrrcsmc_validate_int_security_mode_command_r7

DESCRIPTION
  This function validates integrity part of SECURITY MODE COMMAND.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcsmc_config_status_e_type : It returns INVALID_CONFIG if Critical 
  extensions or any other checks fail.  Else VALID_CONFIG is returned
  
SIDE EFFECTS
  None
===========================================================================*/
static tdsrrcsmc_config_status_e_type tdsrrcsmc_validate_int_security_mode_command_r7
(
tdsrrc_DL_DCCH_Message *msg_ptr              /* Pointer to Downlink DCCH Msg */
)
{  
  /*local ptr to Int. Prot. Mode Info.  */
  struct tdsrrc_IntegrityProtectionModeInfo_r7* integrityProtModeInfo;


  /* if Integrity Protection Mode Info is not included, then send
     SMC Failure message
   */
  if ((msg_ptr->message.u.securityModeCommand.u.later_than_r3.
        criticalExtensions.u.r7.securityModeCommand_r7.m.integrityProtectionModeInfoPresent) == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplete Configuration");  
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION; 
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  integrityProtModeInfo = &(msg_ptr->message.u.securityModeCommand.u.later_than_r3.
        criticalExtensions.u.r7.securityModeCommand_r7.integrityProtectionModeInfo);

  /*if we have already started integrity protection, then we should not
   *receive another start command*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t ==
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection) &&
       (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_STARTED) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Start Cmd ");    
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;
  }

  /*if we receive a START cmd and do not receive the integrity 
    protection algorithm, return invalid config*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection) &&  
       (integrityProtModeInfo->m.integrityProtectionAlgorithmPresent == 0 ) 
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Incomplete Configuration");      
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;  
  }

  /*if we receive a MODIFY cmd and integrity is not yet started
    return invalid config*/
  if ( (integrityProtModeInfo->integrityProtectionModeCommand.t ==
        T_tdsrrc_IntegrityProtectionModeCommand_modify) &&
       (tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_NOT_STARTED) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Configuration");      
    tdsrrcsmc_failure_type = TDSRRCSMC_INVALID_CONFIGURATION;  
    return TDSRRCSMC_INVALID_CONFIGURATION;    
  }

  return TDSRRCSMC_VALID_CONFIG;          

}/*tdsrrcsmc_validate_int_security_mode_command*/
/*===========================================================================

FUNCTION tdsrrcsmc_check_integrity_for_security_mode_command_r7

DESCRIPTION
  This function validates Integrity Protection for Security Mode Command.
  It also updates the FRESH and KEYS if it is the START of Integrity.
  It also updates the KEY values if we have received new keys.
  
DEPENDENCIES
  None

RETURN VALUE
  uecomdef_status_e_type : 
  Returns SUCCESS if Integrity Check for Security Mode Command 
  is successful.
  Returns FAILURE if Integrity Check for Security Mode Command
  is not successful
  
SIDE EFFECTS
  None
===========================================================================*/
static uecomdef_status_e_type tdsrrcsmc_check_integrity_for_security_mode_command_r7
(
  tdsrrc_cmd_type *cmd_ptr                    /* Pointer to the RRC Command */
)
{  
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr;         /* Pointer to Downlink DCCH Msg */

  tdsrrc_RB_Identity rb_id;/*local var to store rb_id*/
  tdsrrc_IntegrityProtectionModeInfo_r7* integrityProtModeInfo;

  /*flag to indicate whether new keys are being*/  
  boolean rcvd_new_keys_flag=FALSE;

  /*local vars to store bunch of values used for manipulation in this func.only*/
  uint32 local_fresh, local_seq_no, old_seq_no; 
  uint32 local_count_i, local_hfn;
  uint32 received_mac, local_mac;
  tdsrrcsmc_integrity_check_info_type int_check_info;
  boolean local_start_recv = FALSE;

  /*local copy of domain id. Set it to cs domain to suppress warning*/
  rrc_cn_domain_identity_e_type domain_id=RRC_CS_DOMAIN_CN_ID;  

  // KW error
  local_count_i = 0;
  int_check_info.msg_auth_code = 0;

  /*get the dcch_msg_ptr for SMC as this message is already decoded*/
  dcch_msg_ptr =  (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;


  /*copy integrityProtectionModeInfo * to a local copy to make easy readability*/
  integrityProtModeInfo = &(dcch_msg_ptr->message.u.securityModeCommand.u.
    later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.integrityProtectionModeInfo);


  /*get the domain id*/
  if (dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.
            cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else if (dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.
            cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
  {
    domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  /*get the rb_id on which the message was received*/
  rb_id = tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);



  if(rb_id < TDSRRC_MAX_SRB)
  {
    local_seq_no = dcch_msg_ptr->integrityCheckInfo.rrc_MessageSequenceNumber;

    if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
        T_tdsrrc_IntegrityProtectionModeCommand_modify)
    {
      if (tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
          dl_new_key_flag_per_rb[rb_id] == TRUE)
      {
        /*set this local flag to true*/
        rcvd_new_keys_flag=TRUE;
        /*set hfn to zero, as we have to reset HFN part of start value to 0*/
        local_hfn = 0x0;
      }
      else if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
              == TRUE)
      {
        /*if the start_value_update flag is set then we have updated the start value
          due to an IDT, this value needs to be used for COUNT_I calculation*/
        local_hfn =  
          tdsrrcsmc_convert_START_to_count_c_i(
            tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,
            0);      
      }
      else
      {
        local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
                    dl_count_i[rb_id];
      }

      /*get the seq num of the old domain*/
      old_seq_no = (tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.
                    latest_config_cn_id].dl_count_i[rb_id] & TDSRRC_SEQ_NUM_BIT_MASK);


      /*check if old sequence number is greater than current seq. no.  if that is the
        case, then there has been a rollover.. so increment the HFN by 1, as per 1808*/
      if (old_seq_no > local_seq_no)
      {
        /*increment hfn only*/
        local_hfn >>=4;
        local_hfn++;
        local_hfn <<=4;
      }
    }
    else
    {
      local_hfn = tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id];
    }

    /*generate count_i from hfn and sequence number*/
    local_count_i = 
    (uint32)((local_hfn & TDSRRC_HFN_BIT_MASK) | 
             (local_seq_no & TDSRRC_SEQ_NUM_BIT_MASK) 
             );
  }
  /*get the value of fresh from the received message and initialize F9 engine.
    We can receive the value of FRESH only if START chosen*/
  if (integrityProtModeInfo->integrityProtectionModeCommand.t == 
      T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
  {
    local_start_recv = TRUE;
    local_fresh = b_unpackd(
      integrityProtModeInfo->integrityProtectionModeCommand.u.startIntegrityProtection->
      integrityProtInitNumber.data, 
      TDSRRC_DEFAULT_BIT_POS,
      TDSRRC_FRESH_LENGTH_IN_BITS);  

    /*update fresh value for this connection in F9 engine*/
    tdsrrcf9_init_fresh_per_connection(local_fresh);
  

  /*init the IK values for CS and PS domains and the fresh value for
    this connection*/
  tdsrrcf9_init_key_values(
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);

  }
  else
  {
    local_start_recv = FALSE;
    /*Do not init with the other domain's latest as it may not be activated yet*/
    if(domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      tdsrrcf9_init_key_values(
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
        tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);
    }
    else
    {
      tdsrrcf9_init_key_values(
        tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
        tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key);
    }

  }

  (void) tdsrrcsmc_process_dl_integrity_check_info(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                                                &int_check_info,rb_id);

  /*Update the received message*/
  received_mac = int_check_info.msg_auth_code;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CN domain %d[0=CS,1=PS], Integrity Algo = %d[0=Invalid,1=UIA1,2=UIA2]",
                  domain_id,
                   tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &integrityProtModeInfo->integrityProtectionAlgorithm,
             TDSMSG_REL7),0);

  /*call the f9 engine to generate MAC for the dl message*/  
  local_mac = tdsrrcf9_generate_mac_for_message(
                local_count_i,
                TDSRRC_INTEGRITY_DIR_DOWNLINK,
                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length*8),
                domain_id,
                tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &integrityProtModeInfo->integrityProtectionAlgorithm,
             TDSMSG_REL7));


  TDSRRC_MSG2(MSG_LEGACY_HIGH,"local_mac = 0x%x,received_mac = 0x%x",local_mac,received_mac);

  /*if the generated and received mac are the same, update our copy of count_i
    and set the init_flag to true*/
  if ( local_mac == received_mac)
  {
    
    /*SMC Modify received*/
    if(local_start_recv == FALSE)
    {
      /*store the committed new key for current domain*/
      if(domain_id == RRC_CS_DOMAIN_CN_ID)
      {
       memscpy(tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      }
      else
      {
        memscpy(tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      }
      /*update for SRB2: DCCH_AM_RADIO_BEARER_ID*/
      memscpy(tdsrrcsmc_latest_used_ip_key[DCCH_AM_RADIO_BEARER_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               tdsrrcsmc_last_committed_ip_key[(uint8)domain_id], 
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));

    }
    else /*SMC Start received*/
    {
      uint8 i = 0;
      /*store the committed key for both domains*/
      memscpy(tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      memscpy(tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
              tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key, 
              RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));

      /*init the tdsrrcsmc_latest_used_ip_key*/
      for(i=0; i<TDSRRC_MAX_SRB; i++)
      {
        memscpy(tdsrrcsmc_latest_used_ip_key[i], RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
               tdsrrcsmc_last_committed_ip_key[(uint8)domain_id], 
               RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte));
      }

    }


    if(rb_id < TDSRRC_MAX_SRB)
    {
      /* new keys flag wont be set to TRUE is received type is integrity STARTED */
      if (rcvd_new_keys_flag == TRUE)
      {
        /*reset new keys flag.  we are changing it here to be sure that the SMC
            command was successfully integrity checked*/
        tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
        dl_new_key_flag_per_rb[rb_id] = FALSE;

        /*any previously sent start value is obliterated.. we should start using 0's from
          now onwards.. hence set start_value_update_flag to false*/
        tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;      

        /*reset the hfn values to 0's if authentication is done and UE is
         *in TDS mode in dch/fach
         */
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[0]=0x0;
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[1]=0x0;
        tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[2]=0x0;

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Started using new keys, CIPH START VAL: 0x%x 0x%x 0x%x", 
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[0],
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[1],
           tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn[2]);
      
    }


      /*Inform RR for the Key set change at TDS side in case a New Key is AKAed just before IRAT HO 
        So RR knows which is the latest activated key set*/
      if((domain_id == RRC_CS_DOMAIN_CN_ID) && (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].
              ciph_keys_info.new_key == TRUE))
      {
        tdsrrciho_send_cipher_sync_ind();
      }

    tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].dl_count_i[rb_id] = local_count_i;

    tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].
    dl_count_i_init_flag[rb_id] = TRUE;

    /*reset the start_value_update flag as we already passed integrity check
      successfully for this domain, also copy the updated start value to the
      database and initialize rb2, as there is no act time for rb2..
      also start_value_update_flag will be true only for Integrity Modify,
      as START value will be updated for integrityStart in smc_initial substate
      function*/
    if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] == TRUE &&
       !(rcvd_new_keys_flag))
    {
      /*copy the latest start value sent in IDT or CU message*/
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
             tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,        
             RRC_MAX_HFN_LENGTH*sizeof(byte));
      
      /*since there is no pending activation time for RB2, update COUNT_I info for new 
        CN domain here when successfully check for SMC is done*/
      tdsrrcsmc_integrity_protection_info.count_i_info[domain_id].ul_count_i[2] =         
        tdsrrcsmc_convert_START_to_count_c_i(
          tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn,
          tdsrrcsmc_integrity_protection_info.count_i_info[tdsrrcsmc_int_ciph_info.latest_config_cn_id].
          ul_count_i[2]);

      tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;
    }

    //set the Integrity START status for this domain ID to true.
    //only if this flag is set, then we will extract latest start values
    //at connection release time.
    tdsrrcsmc_int_ciph_info.int_start_status_for_conn[domain_id] = TRUE;
   }
    return SUCCESS;
  }
  else
  {
    TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
							   TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
							   TDS_SELF_HOSTING_MINOR_DL_IP_CHECK_FAIL);
    /*if we have received new keys and this SMC message failed Integrity
        test, then we revert back to the old keys as we will not be using 
        the new keys or new Security Configuration*/
    
    /*if (rcvd_new_keys_flag == TRUE)
    {
      tdsrrcf9_init_key_values(
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old,
       tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_PS_DOMAIN_CN_ID].integrity_key_old);
    } 
    */

    /*Update to revert based on last committed for modify case*/
    if(local_start_recv == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL IP check failed for SMC itself,revert to last committed key");
      tdsrrcf9_init_key_values(
         tdsrrcsmc_last_committed_ip_key[RRC_CS_DOMAIN_CN_ID],
         tdsrrcsmc_last_committed_ip_key[RRC_PS_DOMAIN_CN_ID]);

    }
    
     
    return FAILURE;
  }
}/*tdsrrcsmc_check_integrity_for_security_mode_command*/
/*===========================================================================

FUNCTION tdsrrcsmc_save_contents_of_security_mode_command_r7

DESCRIPTION
   This function processes the SECURITY MODE COMMAND received on DL DCCH
   and extracts the Transaction ID.  Sends the SYNC_IND cmd to MM and
   also calls the function to send SECURITY MODE COMPLETE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_save_contents_of_security_mode_command_r7
(
tdsrrc_DL_DCCH_Message *dcch_msg_ptr
)
{

  uint32 i, index=0, n_value=0, rb_index=0, count=0; /*used for counter purposes*/
  struct tdsrrc_SecurityModeCommand_r7_IEs * local_smc_msg_ptr;

  /*local var to store act time linked list ptr*/
  struct tdsrrc_RRC_MessageSequenceNumberList *local_smc_act_list;

  struct tdsrrc_RB_ActivationTimeInfoList    *local_rb_ciph_act_time;

  /*copy indirected link to a local ptr*/
  local_smc_msg_ptr = &(dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7);

  /*save transaction id*/
  tdsrrcsmc_saved_security_mode_command_msg.transaction_id = (OSUINT8)
            dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.rrc_TransactionIdentifier;

  /*copy domain id*/
  if(local_smc_msg_ptr->cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) 
  {
    tdsrrcsmc_saved_security_mode_command_msg.domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else
  {
    tdsrrcsmc_saved_security_mode_command_msg.domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  if(tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE)
  {

    /*check if start or modify is chosen and update accordingly*/
    if (local_smc_msg_ptr->integrityProtectionModeInfo.
        integrityProtectionModeCommand.t ==  T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      tdsrrcsmc_saved_security_mode_command_msg.start_chosen = TRUE;
      tdsrrcsmc_saved_security_mode_command_msg.modify_chosen = FALSE;
  
      /*get the fresh value*/
      tdsrrcsmc_saved_security_mode_command_msg.start = 
      b_unpackd(local_smc_msg_ptr->integrityProtectionModeInfo.
                integrityProtectionModeCommand.u.startIntegrityProtection->
                integrityProtInitNumber.data,
                TDSRRC_DEFAULT_BIT_POS,
                TDSRRC_FRESH_LENGTH_IN_BITS);  

      //save the fresh value for srns relocation purposes..
      tdsrrcsmc_saved_srns_info.fresh_old = tdsrrcsmc_saved_security_mode_command_msg.start;

      /* Since its the first time FRESH value is provided to the UE, hence
         there is no old1 fresh value. Hence, saving the received FRESH 
         value in fresh_old1 as well. This stored value is needed to revert back to old 
         configuration after SRNS relocation physChannelEstFailure */
      tdsrrcsmc_saved_srns_info.fresh_old1 = tdsrrcsmc_saved_security_mode_command_msg.start;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Saved FRESH is 0x%x",tdsrrcsmc_saved_security_mode_command_msg.start);

      tdsrrcsmc_saved_security_mode_command_msg.start_chosen = FALSE;
      tdsrrcsmc_saved_security_mode_command_msg.modify_chosen = TRUE;
    
      /*initialize seqNumList*/
      local_smc_act_list = &(local_smc_msg_ptr->integrityProtectionModeInfo.
        integrityProtectionModeCommand.u.modify->dl_IntegrityProtActivationInfo.
        rrc_MessageSequenceNumberList);

      /*initialize count_i for NEW latest CN domain*/
      index = 0;
      n_value = local_smc_act_list->n;
        
      for ( i=0; i < TDSRRC_MAX_SRB; i++)
      {
        
        if(n_value == 0)
        {
          break;
        }
  
        /*initialize downlink count_i dl_activation_info time sent by utran
          for this domain */
        tdsrrcsmc_saved_security_mode_command_msg.srb_int_dl_act_time[i].seq_num = 
        (uint8)local_smc_act_list->elem[index];
        n_value--;
        index++;
      }
    }
    
    /*save the integrity algorithm sent*/
    tdsrrcsmc_saved_security_mode_command_msg.integrity_algo = 

    tdsrrcsmc_int_ciph_info.saved_integrity_algo[tdsrrcsmc_saved_security_mode_command_msg.domain_id] 
       = tdsrrcsmc_get_int_integrity_algo_from_dl_sdu_ind(
            &local_smc_msg_ptr->integrityProtectionModeInfo.integrityProtectionAlgorithm,
             TDSMSG_REL7);

  }

  if(tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE)
   {
  
    /*save the ciphering algorithm sent*/
    tdsrrcsmc_saved_security_mode_command_msg.ciph_algo = 
          tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
              &(local_smc_msg_ptr->cipheringModeInfo.cipheringModeCommand.startRestart),
              TDSMSG_REL7);
  
    /*save to temp var*/
    tdsrrcsmc_int_ciph_info.saved_ciph_algo[tdsrrcsmc_saved_security_mode_command_msg.domain_id] = 
          tdsrrcsmc_get_int_cipher_algo_from_dl_sdu_ind(
              &(local_smc_msg_ptr->cipheringModeInfo.cipheringModeCommand.startRestart),
              TDSMSG_REL7);
  
    /*copy activation time for DPCH if sent*/
    if (local_smc_msg_ptr->cipheringModeInfo.m.activationTimeForDPCHPresent)
    {
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present = TRUE;
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time = 
        local_smc_msg_ptr->cipheringModeInfo.activationTimeForDPCH;
    }
    else
    {
      tdsrrcsmc_saved_security_mode_command_msg.dpch_ciph_act_time_present = FALSE;
    }
  
    if (local_smc_msg_ptr->cipheringModeInfo.m.rb_DL_CiphActivationTimeInfoPresent)
    {
      /*copy Radio Bearer DL activation time for all UM/AM RB's*/
      local_rb_ciph_act_time = &(local_smc_msg_ptr->cipheringModeInfo.
                               rb_DL_CiphActivationTimeInfo);
  
      /*set the flag to true*/
      tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time_present = TRUE;
  
      /*init local vars used for counter purposes to zero*/
      tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time = 0;
  
      /*in a while loop, copy the number of DL activation time for each
        radio bearer*/
      index = 0;
	  
      // 2012/09/10 KW
      while((local_rb_ciph_act_time->n > index) && 
             (index < (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY)) &&
			 (tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time < (UE_MAX_DL_UM_CHANNEL + UE_MAX_AM_ENTITY)))
      {
        if((local_rb_ciph_act_time->elem[index].rb_Identity == DCCH_UM_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[index].rb_Identity == DCCH_AM_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[index].rb_Identity == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID) ||
           (local_rb_ciph_act_time->elem[index].rb_Identity == DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
        {
          tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].rb_id = (uint8) 
            local_rb_ciph_act_time->elem[index].rb_Identity;
          tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].seq_num = (uint16)
            local_rb_ciph_act_time->elem[index].rlc_SequenceNumber;           
          tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time++;
        }
        else
        {
          for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
          {
            if (tdsrrc_est_rabs.rabs[count].rab_id != RRC_INVALID_RAB_ID)
            {
              for (rb_index = 0; rb_index < tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; rb_index++)
              {
                if (tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_status ==
                    TDSRRC_RB_STARTED)
                {
                  /* Check to see if this RB info is included in the ciph activation times list */
                  // for KW
                  if((tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id ==
                        local_rb_ciph_act_time->elem[index].rb_Identity) && 
                       (tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time < 
                    (TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY)))
                  {
                    tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].rb_id = (uint8) 
                      local_rb_ciph_act_time->elem[index].rb_Identity;
                    tdsrrcsmc_saved_security_mode_command_msg.rb_ciph_dl_act_time[tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time].seq_num = (uint16)
                      local_rb_ciph_act_time->elem[index].rlc_SequenceNumber;           
                    tdsrrcsmc_saved_security_mode_command_msg.num_rb_ciph_dl_act_time++;
                  }                
                }
              }
            }
          }           
        }
        index++;
      }

    }
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_process_smc_initial_substate

DESCRIPTION
   This function processes the SECURITY MODE COMMAND received on DL DCCH
   and extracts the Transaction ID.  Sends the SYNC_IND cmd to MM and
   also calls the function to send SECURITY MODE COMPLETE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_security_mode_command_r7
(
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr,  /* Pointer to Downlink DCCH */
  tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  uint32 rrcsmc_trans_id = 0;                   /* local copy of trans id*/

  tdsrrc_CN_DomainIdentity domain_id;              /* local copy of domain id*/


  /*Processing for R3 security mode command message*/
  
  /* Get the transaction identified from the SECURITY MODE COMMAND and
   store it in local copy*/
  rrcsmc_trans_id = dcch_msg_ptr->message.u.securityModeCommand.u.
         later_than_r3.rrc_TransactionIdentifier;  

  /*check to see that no critical extensions are added and no reconfig
     is currently being done*/
  if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_security_mode_command_r7(dcch_msg_ptr) )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC MSG VALIDATION FAILED");
    tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
    return;
  }
  
  /*check if TDS security capablities are correct with UECI procedure*/
  if(tdsrrcueci_verify_security_capability((&dcch_msg_ptr->message.u.
    securityModeCommand.u.later_than_r3.criticalExtensions.u.r7.
    securityModeCommand_r7.securityCapability)) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC Sec. Cap. Check failed");
    //TBD use CSP to take UE to IDLE
    (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_SMC,TDSRRC_TX_TO_DISCON_OTHER);
    return;
  }
  
  /*if interrat security capabilities are present in the message then check its's
    validity with UECI procedure*/
  if(dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.
      criticalExtensions.u.r7.securityModeCommand_r7.m.ue_SystemSpecificSecurityCapPresent)
    {
      if(tdsrrcueci_verify_inter_rat_security_capability(
        ( (struct tdsrrc_InterRAT_UE_SecurityCapList *) 
          &dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.
           criticalExtensions.u.r7.securityModeCommand_r7.
           ue_SystemSpecificSecurityCap)) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC-IRAT Sec. Cap. Check failed");
        (void) tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_SMC,TDSRRC_TX_TO_DISCON_OTHER);
        return;
      }
    }
  if((tdsnv_ciphering_enabled) &&
     (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE))
  {
  
    /* Validate the SECURITY MODE COMMAND.  If not valid then return without 
        processing the message further. Send SMC failure*/
    if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_ciph_security_mode_command_r7(dcch_msg_ptr) )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ciphering SMC MSG VALIDATION FAILED");
      tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
      return;
    }
  }
  
  if ((tdsnv_integrity_enabled)  &&
      (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
  {
    /* Validate the RRC SECURITY MODE COMMAND.  If validation not successful 
       then return without processing the message further. 
       Send the security mode failureis set to 0*/
    if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_int_security_mode_command_r7(dcch_msg_ptr) )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity SMC MSG VALIDATION FAILED");
      tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
      return;
    }
  
    if (dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.
        criticalExtensions.u.r7.securityModeCommand_r7.integrityProtectionModeInfo.
        integrityProtectionModeCommand.t ==
          T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
    {
      domain_id = dcch_msg_ptr->message.u.securityModeCommand.u.
            later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.cn_DomainIdentity;
         
      /*if we have updated START value in IDT or CU and not have done authentication,
         then start_value_update_flag will be true. in this case, copy the start hfn_latest
         to the main database and initialize the integrity protection configuration*/
      if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
             == TRUE)
      {
        /*copy the latest start value sent in IDT or CU message*/
        memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
                     tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,        
                    RRC_MAX_HFN_LENGTH*sizeof(byte));
  
        tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;
      }
  
      /*initializes tdsrrcsmc_integrity_protection_info count_i values for ul and dl
         first time when we are starting integrity*/
      tdsrrcsmc_init_integrity_protection_info(tdsrrc_get_as_id());
    }
  
    /*check downlink integrity for the security mode command which 
       we just received. Only if integrity check passed for this message shall
       we continue further.  Otherwise just return and do not process this
      message*/
  
    if (tdsrrcsmc_check_integrity_for_security_mode_command_r7(cmd_ptr) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity Check for SMC cmd Failed");
      /*just return. Unsuccessful integrity check for security mode cmd*/
      return;
    }
    else
    {
      /* SMC will receive "TDSRRC_DOWNLINK_SDU_IND" only for SecurityModeCommand */
      /*check if cell update is going on, if yes, then release the connection */
      /* if 'tdsfeature_rollback_security_config' is set to TRUE then UE will not
          reach this point */
      if (TDSRRCCU_NOT_COMPLETED  == 
           tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_SMC))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update is active. Ignoring SMC");
        return;          
      }
    }
  }/*if integrity protection info received and nv item is enabled*/
  
  /*save the contents of security mode command*/
  tdsrrcsmc_save_contents_of_security_mode_command_r7(dcch_msg_ptr);
  if(tdsnv_integrity_enabled &&
     tdsrrcsmc_int_ciph_info.integrity_info_received)
  {
    /* Update all integrity related info. */
    tdsrrcsmc_update_latest_cn_id();
    /* Integrity reconfiguration started */
    tdsrrcsmc_integrity_protection_info.reconfig = TRUE;
  }
  
  /*if feature is defined and nv item is turned on, then process the ciph
      parameters..*/
  if( (tdsnv_ciphering_enabled) &&
      (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE))
  {
    /* Ciphering reconfig started. */
    tdsrrcsmc_ciphering_info.reconfig = TRUE;
  
    /*check if any reconfig is going on.  if it is go to wait state*/
    if (tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
    {
      /*register for a cnf when reconfiguration is completed*/
      tdsrrcllc_register_for_reconfig_cnf();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WAIT_FOR_RECONFIG_CNF state");
      /* Wait until Reconfig procedure is completed */
      tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_RECONFIG_CNF;  
    }
    else
    {
      /*if cell update or reconfig is not currently in progress,
         send suspend req to rlc*/
      tdsrrcsmc_perform_stop_or_suspend_functionality(TDSRRCSMC_SMC_CMD);
    }
  }
  else
  {
    /*ciphering is not present.. therefore integrity is present..
      just process the integrity message and return*/
    tdsrrcsmc_process_int_security_mode_cmd_msg();
     
    /* Send SECURITY MODE COMPLETE command */
    tdsrrcsmc_send_security_mode_complete(tdsrrcsmc_saved_security_mode_command_msg.
                                      transaction_id);
  }
}



/*===========================================================================

FUNCTION tdsrrcsmc_process_smc_initial_substate

DESCRIPTION
   This function processes the SECURITY MODE COMMAND received on DL DCCH
   and extracts the Transaction ID.  Sends the SYNC_IND cmd to MM and
   also calls the function to send SECURITY MODE COMPLETE

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcsmc_process_smc_initial_substate
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */

  uint8 message_choice;                         /* Local variable for message
                                                   choice */

  uint32 rrcsmc_trans_id = 0;                   /* local copy of trans id*/

  tdsrrc_CN_DomainIdentity domain_id;              /* local copy of domain id*/

  rrc_cn_domain_identity_e_type cn_domain_id;   /*domain id                */
  boolean is_cn_domain_id_valid =FALSE;

  tdsrrc_cmd_type *int_cmd_ptr;                    /* to send re-est srns failure */

  tdsrrc_state_e_type rrc_state = tdsrrc_get_state(); /* local var to store rrc state*/

  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
    case TDSRRC_HO_CIPH_REQ:
      /*we can do GtoW HO only in DCH state.. so if we r not in DCH state and
        receive this message.. flag error and return*/
      if(rrc_state != TDSRRC_STATE_CELL_DCH)
      {

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"G2TD/L2TD HO Ciph only in DCH");      
          tdsrrcsmc_send_ho_ciph_config_cnf(FAILURE);        
        return;
      }
       /*process the Ciphering request for GtoW Handover*/
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Step 1 Ciph for GtoW HO");
       tdsrrcsmc_process_step_1_ho_ciph_req(cmd_ptr->cmd.ho_ciph_req);

      break;
#endif

  case TDSRRC_RE_EST_RLC_FOR_SRNS_REQ:
    //check if case is for re-est rb2 only, in this state non-rb-2 
    //re-establishment is not supported.
    if(cmd_ptr->cmd.rrc_re_est_srns_req.rb_type != TDSRRC_RE_EST_RB_2)
    {
      //send cnf with failure
      if ( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {
        /* Fill in the command parameters for TDSRRC_RE_EST_RLC_FOR_SRNS_CNF */
        int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RE_EST_RLC_FOR_SRNS_CNF;
        int_cmd_ptr->cmd.rrc_re_est_srns_cnf.status = FAILURE;
        tdsrrc_put_int_cmd(int_cmd_ptr);  
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send Re-Est Fail-Non-RB2");

        /*resume stopped RBs*/
        if(tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send RLC Continue for Non-TM RBs");
          tdsrrcsmc_send_continue_req_srns(TDSRRCSMC_RESUME_NON_TM_RB);
        }
        else if(tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send RLC Continue for SRB2");
          tdsrrcsmc_send_continue_req_srns(TDSRRCSMC_RESUME_SRB2);
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"NO RLC to Continue, stop flag=%d,SRB2 stop flag=%d",tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag,tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag);
        }

      }      
    }
    else
    {
      tdsrrcsmc_saved_srns_info.rb_type = TDSRRC_RE_EST_RB_2;
    }
    
    //set integrity reconfiguration to false
    tdsrrcsmc_integrity_protection_info.reconfig = FALSE;

    //commit the new ciphering algorithm
    if(tdsrrcsmc_saved_srns_info.ciph_algo_present == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"At SRNS save Cipher Algo:%d[0=Invalid,1=UEA0,2=UEA1,4=UEA2] in tdsrrcsmc_int_ciph_info",tdsrrcsmc_saved_srns_info.ciph_algo);
      tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_CS_DOMAIN_CN_ID] = 
        tdsrrcsmc_int_ciph_info.saved_ciph_algo[RRC_PS_DOMAIN_CN_ID] =
        tdsrrcsmc_saved_srns_info.ciph_algo;
    }

    //commit the new integrity algorithm
    if(tdsrrcsmc_saved_srns_info.integrity_algo_present == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"AT SRNS save Integrity Algo = %x[0=Invalid,1=UIA1,2=UIA2] in tdsrrcsmc_int_ciph_info",tdsrrcsmc_saved_srns_info.integrity_algo);

      tdsrrcsmc_int_ciph_info.saved_integrity_algo[RRC_CS_DOMAIN_CN_ID] = 
      tdsrrcsmc_int_ciph_info.saved_integrity_algo[RRC_PS_DOMAIN_CN_ID] =
        tdsrrcsmc_saved_srns_info.integrity_algo;
    }

    /*set ciphering reconfig info to TRUE*/
    tdsrrcsmc_ciphering_info.reconfig = TRUE;
    
    //if re_est cnf was needed, set flag to true, else false
    if(cmd_ptr->cmd.rrc_re_est_srns_req.cnf_reqd == TRUE)
    {
      tdsrrcsmc_saved_srns_info.reconfig_cnf_req = TRUE;
      
      //save the procedure name to send cnf
      tdsrrcsmc_saved_srns_info.proc_type = cmd_ptr->cmd.rrc_re_est_srns_req.rrc_proc;
    }
    else
    {
      tdsrrcsmc_saved_srns_info.reconfig_cnf_req = FALSE;    
    }
    
    //send Stop req
    if(tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == FALSE)
    {
      /*use the function means the RLC was not suspend correctly by LLC*/
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "RLC was not suspend correctly by LLC, Stop Now. rb stop flag = %d,srb2 stop flag =%d",tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag,tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag);
      tdsrrcsmc_perform_stop_or_suspend_functionality(TDSRRCSMC_SRNS_CMD);   
     
    }
    else
    {  
      /*We need to do the above old stop_or_suspend_functionality without stop RLC*/
      tdsrrcsmc_purge_srb_and_process_suspend_srns_cnf();
    }
    break;

    case TDSRRC_DOWNLINK_SDU_IND:
      /* SMC will receive "TDSRRC_DOWNLINK_SDU_IND" only for SecurityModeCommand */
      /*check if cell update is going on, if yes, then ignore the Security Mode Procedure */
      if (TDSRRCCU_NOT_COMPLETED  == 
             tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_SMC))
      {
        if (TRUE == tdsfeature_rollback_security_config)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Security Mode Command because cell update is in progress");
          return;
        }
      }

      /*if we get a downlink sdu ind in any other state then we should
       *report an error and not process it.
       */
      if ( (rrc_state != TDSRRC_STATE_CELL_FACH )&&
           (rrc_state != TDSRRC_STATE_CELL_DCH ) )
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unsupported Event: %d for RRC in state: %d",cmd_ptr->cmd_hdr.cmd_id,rrc_state); 
        return;
      }

      /* Set the protocol error reject to FALSE */
      tdsrrc_protocol_error_reject = FALSE;

      /* Get the message type */    
      message_choice = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                   cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

      /* Check if the received message is Security Mode Command message */
      if ( message_choice != T_tdsrrc_DL_DCCH_MessageType_securityModeCommand)
      {
/*lint -e793*/
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid msg rcvd SMC Init State");
/*lint +e793*/
        return;
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received Security Mode Command Message");

      /* Get the OTA message pointer */
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                       cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

      if (dcch_msg_ptr == NULL) /* DCCH ASN1 decoding failed */
      {
        tdsrrc_protocol_error_reject = TRUE ;
#if(!defined(T_WINNT))

        /*log the protocol error*/
        tdsrrc_log_protocol_error((uint8)tdsrrc_get_state(),
                               (uint8)TDSRRC_PROCEDURE_SMC,
                                (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                (uint8)TDSRRCLOG_ASN1_ENC_ERR
                              );
#endif

        /*send a security mode failure command if there is a protocol error
         *in the received security mode command
         */
        tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR; 
        tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
        tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);
      }
      else /* ASN1 decoding is successful */
      {
        /* Get the transaction identified from the SECURITY MODE COMMAND and
           store it in local copy*/
        if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                  T_tdsrrc_SecurityModeCommand_r3)
        {           
          rrcsmc_trans_id = dcch_msg_ptr->message.u.securityModeCommand.u.
                          r3.securityModeCommand_r3.rrc_TransactionIdentifier;
        }
        else if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                  T_tdsrrc_SecurityModeCommand_later_than_r3)
        {
          rrcsmc_trans_id = dcch_msg_ptr->message.u.securityModeCommand.u.
                            later_than_r3.rrc_TransactionIdentifier;
        }
        else
        {
          rrcsmc_trans_id = 0;
        }
   
        /*check if any reconfig is going on.  if it is go to wait state*/
        if (tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
        {
          if((tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_FACH_CELL_PCH_TRANS)||
            (tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS)||
            (tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_FACH_URA_PCH_TRANS)||
            (tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_DCH_URA_PCH_TRANS))
          {
            /*send a security mode failure command if there is a protocol error
             *in the received security mode command
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"We are in middle of transition to PCH,send SM failure");
            tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR; 
            tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState;
            tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);
            return;
          }

          if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                       T_tdsrrc_SecurityModeCommand_later_than_r3)
          {
            /*copy domain id*/
            if(dcch_msg_ptr->message.u.securityModeCommand.u.
                      later_than_r3.criticalExtensions.u.r7.securityModeCommand_r7.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) 
            {
              cn_domain_id = RRC_CS_DOMAIN_CN_ID;
            }
            else
            {
              cn_domain_id = RRC_PS_DOMAIN_CN_ID;
            }
            is_cn_domain_id_valid = TRUE;
          }
          else
          {
            /*copy domain id*/
            if(dcch_msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain) 
            {
              cn_domain_id = RRC_CS_DOMAIN_CN_ID;
            }
            else
            {
              cn_domain_id = RRC_PS_DOMAIN_CN_ID;
            }
            is_cn_domain_id_valid = TRUE;
          }
          if((is_cn_domain_id_valid == TRUE)&&
            (tdsrrcsmc_int_ciph_info.latest_config_cn_id != cn_domain_id)&&
              (tdsrrcsmc_get_security_start_status() == TRUE))
          {
#ifdef FEATURE_TDSCDMA_SMC_DURING_RECFG_OPT
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"We are in middle of Reconfig,do FEATURE_TDSCDMA_SMC_DURING_RECFG_OPT");
            /*check if it is during a SRNS relocation*/
            if(tdsrrcrb_is_srns_in_progress())
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"We are in process of SRNS relocation,reject the SMC from source cell");
              /* Set the failure cause and send the failure message */
              tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG; 
              tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
              return;
            }
#else
            /*send a security mode failure command if there is a protocol error
             *in the received security mode command
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"We are in middle of Reconfig send SM failure,so that NW can retry");
            /* Set the failure cause and send the failure message */
            tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG; 
            tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
            return;
#endif

          }
        }

        /* ASN1 decoding passed, hence take a back up of security variables because 
           some of the variables are updated while validating the SMC message */
        tdsrrcsmc_backup_global_security_variables();
        if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                  T_tdsrrc_SecurityModeCommand_later_than_r3)
        {
          if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            if(dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.
              criticalExtensions.t == T_tdsrrc_SecurityModeCommand_criticalExtensions_1_r7)
            {
              /*Processing for R7 security mode command message*/
              tdsrrcsmc_process_security_mode_command_r7(dcch_msg_ptr,cmd_ptr);
              
            }
            else
            {
              tdsrrc_print_supported_asn1_rel_version();
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid non critical ext for Security mode Command message");
              tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR;
              tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
              tdsrrcsmc_send_security_mode_failure(dcch_msg_ptr->message.u.
                securityModeCommand.u.later_than_r3.rrc_TransactionIdentifier);
            }
          }
          else
          {
            tdsrrc_print_supported_asn1_rel_version();
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"nv_rel_ind is non Rel7 ,so cant support R7 SMC  Send SM failure");
            tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR;
            tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended;
            tdsrrcsmc_send_security_mode_failure(dcch_msg_ptr->message.u.
              securityModeCommand.u.later_than_r3.rrc_TransactionIdentifier);
          }
          return;
        }
        else
        {
          /*Processing for R3 security mode command message*/
      

        /*check to see that no critical extensions are added and no reconfig
          is currently being done*/
        if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_security_mode_command(dcch_msg_ptr) )
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC MSG VALIDATION FAILED");
          tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
          return;
        }

        /*check if TDS security capablities are correct with UECI procedure*/
        if(tdsrrcueci_verify_security_capability((&dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                                                securityModeCommand_r3.securityCapability)) == FALSE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC Sec. Cap. Check failed");
          (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_SMC,TDSRRC_TX_TO_DISCON_OTHER);
          return;
        }

        /*if interrat security capabilities are present in the message then check its's
          validity with UECI procedure*/
        if(dcch_msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.
              m.ue_SystemSpecificSecurityCapPresent)
          {
            if(tdsrrcueci_verify_inter_rat_security_capability(
              ( (struct tdsrrc_InterRAT_UE_SecurityCapList *) 
                &(dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                securityModeCommand_r3.ue_SystemSpecificSecurityCap))) == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"SMC-IRAT Sec. Cap. Check failed");
              (void) tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_SMC,TDSRRC_TX_TO_DISCON_OTHER);
              return;
            }
          }

        if( (tdsnv_ciphering_enabled) &&
            (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE))
        {

          /* Validate the SECURITY MODE COMMAND.  If not valid then return without 
             processing the message further. Send SMC failure*/
          if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_ciph_security_mode_command(dcch_msg_ptr) )
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Ciphering SMC MSG VALIDATION FAILED");
            tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
            return;
          }
        }

        if ((tdsnv_integrity_enabled)  &&
            (tdsrrcsmc_int_ciph_info.integrity_info_received == TRUE))
        {

          /* Validate the RRC SECURITY MODE COMMAND.  If validation not successful 
             then return without processing the message further. 
             Send the security mode failureis set to 0*/
          if (TDSRRCSMC_VALID_CONFIG != tdsrrcsmc_validate_int_security_mode_command(dcch_msg_ptr) )
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity SMC MSG VALIDATION FAILED");
            tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
            return;
          }

          if (dcch_msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.
              integrityProtectionModeInfo.integrityProtectionModeCommand.t ==
              T_tdsrrc_IntegrityProtectionModeCommand_startIntegrityProtection)
          {
            domain_id = dcch_msg_ptr->message.u.securityModeCommand.u.r3.
              securityModeCommand_r3.cn_DomainIdentity;
            
            /*if we have updated START value in IDT or CU and not have done authentication,
              then start_value_update_flag will be true. in this case, copy the start hfn_latest
              to the main database and initialize the integrity protection configuration*/
            if(tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id]
               == TRUE)
            {
              /*copy the latest start value sent in IDT or CU message*/
              memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, RRC_MAX_HFN_LENGTH*sizeof(byte),
                     tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest,        
                     RRC_MAX_HFN_LENGTH*sizeof(byte));

              tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;
            }

            /*initializes tdsrrcsmc_integrity_protection_info count_i values for ul and dl
              first time when we are starting integrity*/
            tdsrrcsmc_init_integrity_protection_info(tdsrrc_get_as_id());
          }

          /*check downlink integrity for the security mode command which 
            we just received. Only if integrity check passed for this message shall
            we continue further.  Otherwise just return and do not process this
            message*/

          if (tdsrrcsmc_check_integrity_for_security_mode_command(cmd_ptr) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Integrity Check for SMC cmd Failed");
            /*just return. Unsuccessful integrity check for security mode cmd*/
            return;
          }
          else
          {
            /* SMC will receive "TDSRRC_DOWNLINK_SDU_IND" only for SecurityModeCommand */
            /*check if cell update is going on, if yes, then release the connection */
            /* if 'tdsfeature_rollback_security_config' is set to TRUE then UE will not
               reach this point */
            if (TDSRRCCU_NOT_COMPLETED  == 
               tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_SMC))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Update is active. Ignoring SMC");
              return;          
            }
          }
        }/*if integrity protection info received and nv item is enabled*/

        if ((tdsnv_ciphering_enabled) || (tdsnv_integrity_enabled))
        {
          /*save the contents of security mode command*/
          tdsrrcsmc_save_contents_of_security_mode_command(dcch_msg_ptr);
          if(tdsnv_integrity_enabled &&
             tdsrrcsmc_int_ciph_info.integrity_info_received)
          {
            /* Update all integrity related info. */
            tdsrrcsmc_update_latest_cn_id();
            /* Integrity reconfiguration started */
            tdsrrcsmc_integrity_protection_info.reconfig = TRUE;
          }

          /*if feature is defined and nv item is turned on, then process the ciph
            parameters..*/
          if( (tdsnv_ciphering_enabled) &&
              (tdsrrcsmc_int_ciph_info.ciphering_info_received == TRUE))
          {
            /* Ciphering reconfig started. */
            tdsrrcsmc_ciphering_info.reconfig = TRUE;

            /*check if any reconfig is going on.  if it is go to wait state*/
            if (tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
            {
              /*register for a cnf when reconfiguration is completed*/
              tdsrrcllc_register_for_reconfig_cnf();
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WAIT_FOR_RECONFIG_CNF state");
              /* Wait until Reconfig procedure is completed */
              tdsrrcsmc_substate = TDSRRCSMC_WAIT_FOR_RECONFIG_CNF;  
            }
            else
            {
              /*if cell update or reconfig is not currently in progress,
                send suspend req to rlc*/
              tdsrrcsmc_perform_stop_or_suspend_functionality(TDSRRCSMC_SMC_CMD);
            }
          }
          else
          {
            /*ciphering is not present.. therefore integrity is present..
            just process the integrity message and return*/
            tdsrrcsmc_process_int_security_mode_cmd_msg();
          
            /* Send SECURITY MODE COMPLETE command */
            tdsrrcsmc_send_security_mode_complete(tdsrrcsmc_saved_security_mode_command_msg.
                                               transaction_id);
          }
        }

        if (!(tdsnv_ciphering_enabled) && 
            !(tdsnv_integrity_enabled))
        {
          /*set the tdsrrcsmc_int_ciph_info.latest_config_cn_id to the domain_id for which we 
            received security mode command*/
          if (dcch_msg_ptr->message.u.securityModeCommand.
              u.r3.securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
          {
            tdsrrcsmc_int_ciph_info.latest_config_cn_id =  RRC_CS_DOMAIN_CN_ID;
          }
          else if ( dcch_msg_ptr->message.u.securityModeCommand.u.r3.
                    securityModeCommand_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
          {
            tdsrrcsmc_int_ciph_info.latest_config_cn_id = RRC_PS_DOMAIN_CN_ID;
          }

          /* Send SECURITY MODE COMPLETE command */
          tdsrrcsmc_send_security_mode_complete(rrcsmc_trans_id);

        }
        }
      }/*case DOWNLINK_SDU_IND*/
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SMC in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcsmc_substate);
      break;
  }
} /* tdsrrcsmc_process_smc_initial_substate */
/*===========================================================================

FUNCTION tdsrrcsmc_process_cell_dch_state

DESCRIPTION
  This procedure handles smc procedure in CELL_DCH state

  Checks if SMC procedure is in Initial state or in Wait_for_l2_ack state
  and calls those functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcsmc_process_cell_dch_state
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (tdsrrcsmc_substate)
  {
    case TDSRRCSMC_INITIAL: 
      tdsrrcsmc_process_smc_initial_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_L2_ACK:
      tdsrrcsmc_process_wait_for_l2_ack_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_RECONFIG_CNF:
      tdsrrcsmc_process_wait_for_reconfig_cnf_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF:
      tdsrrcsmc_process_wait_for_ciph_config_cnf_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_HO_STEP_1_CIPH_CONFIG_CNF:
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
      tdsrrcsmc_process_wait_for_step_1_ciph_config_cnf_substate(cmd_ptr);
#endif
      break;
    
    case TDSRRCSMC_WAIT_RE_EST_SRNS_CNF:
      tdsrrcsmc_process_wait_re_est_srns_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_SRNS_RELOC_L2_ACK_IND:
      tdsrrcsmc_process_wait_srns_reloc_l2_ack_ind_substate(cmd_ptr);
      break;
	  
    case TDSRRCSMC_WAIT_TILL_STANDBY_ENDS:
      tdsrrcsmc_process_wait_till_standby_ends_substate(cmd_ptr);
      break;

    default:     
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid substate in CELL_DCH state: %d",tdsrrcsmc_substate);
      break;
  }  
}/*tdsrrcsmc_process_cell_dch_state*/
/*===========================================================================

FUNCTION tdsrrcsmc_process_cell_fach_state

DESCRIPTION
  This procedure handles smc procedure in CELL_FACH state

  Checks if SMC procedure is in Initial state or in Wait_for_l2_ack state
  and calls those functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcsmc_process_cell_fach_state
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (tdsrrcsmc_substate)
  {
    case TDSRRCSMC_INITIAL: 
      tdsrrcsmc_process_smc_initial_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_L2_ACK:
      tdsrrcsmc_process_wait_for_l2_ack_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_RECONFIG_CNF:
      tdsrrcsmc_process_wait_for_reconfig_cnf_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF:
      tdsrrcsmc_process_wait_for_ciph_config_cnf_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_RE_EST_SRNS_CNF:
      tdsrrcsmc_process_wait_re_est_srns_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_SRNS_RELOC_L2_ACK_IND:
      tdsrrcsmc_process_wait_srns_reloc_l2_ack_ind_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_TILL_STANDBY_ENDS:
      tdsrrcsmc_process_wait_till_standby_ends_substate(cmd_ptr);
      break;
			
    default:     
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid substate in CELL_FACH state: %d",tdsrrcsmc_substate);
      break;
  }  
}/*tdsrrcsmc_process_cell_fach_state*/
/*===========================================================================

FUNCTION tdsrrcsmc_process_pch_state

DESCRIPTION
  This procedure handles smc procedure in CELL_PCH state

  Checks if SMC procedure is in Initial state and calls the appropriate
  function.  

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcsmc_process_pch_state
(
tdsrrc_cmd_type *cmd_ptr                         /* Pointer to the RRC Command */
)
{
  switch (tdsrrcsmc_substate)
  {
    case TDSRRCSMC_INITIAL: 
      tdsrrcsmc_process_smc_initial_substate(cmd_ptr);
      break;

    case TDSRRCSMC_WAIT_RE_EST_SRNS_CNF:
      tdsrrcsmc_process_wait_re_est_srns_substate(cmd_ptr);
      break;

    default:     
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid substate in CELL_PCH state: %d",tdsrrcsmc_substate);
      break;
  }  
}/*tdsrrcsmc_process_pch_state*/

/*===========================================================================

FUNCTION tdsrrcsmc_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for SECURITY MODE COMMAND
  The following events are handled by this procedure:

  TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing Security
      Mode Command message.
  TDSRRC_STATE_CHANGE_IND Primitive informs transition from Connected mode to
    Idle Disconnected state.
  TDSRRC_DOWNLINK_L2ACK_IND Primitive indicates success or failure of 
    transmission of Security Mode Complete Msg.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcsmc_procedure_event_handler
(
tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
  tdsrrc_state_e_type rrc_state;                   /* Local variable to store the rrc state */
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr = NULL;     /* Pointer to Downlink DCCH */
  uint8 message_choice;                         /* Local variable for message choice */
  uint32 rrcsmc_trans_id = 0;                   /* Local copy of trans id */

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
  uecomdef_status_e_type second_smc_initial_trial = FAILURE;  
#endif  

  //process state_change_ind right here just to be sure that we don't
  //miss going to idle at any cost.. only for HO step 1 do additional
  //step, else for all other states, send SMC to initial substate
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rexd State ChangeInd to %d RRC state",cmd_ptr->cmd.state_change_ind.new_state);
    switch(cmd_ptr->cmd.state_change_ind.new_state)
    {
      case TDSRRC_STATE_DISCONNECTED:
        //if we r waiting for HO Step 1 cnf, send failure
        if(tdsrrcsmc_substate == TDSRRCSMC_WAIT_FOR_HO_STEP_1_CIPH_CONFIG_CNF)
        {
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
          tdsrrcsmc_send_ho_ciph_config_cnf(FAILURE);
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */
        }
        tdsrrcsmc_go_to_smc_initial_substate();
        TDSRRC_MSG0(MSG_LEGACY_MED,"SMC going to INITIAL_STATE");
      break;
   
      case TDSRRC_STATE_CONNECTING:
        /*On receiving state change to connecting check & update any keys 
         *received in other-RAT right here
         */
        tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
      break;
   
      default :
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDSRRC_STATE_CHANGE_IND to RRC state %d not handled",cmd_ptr->cmd.state_change_ind.new_state);
      break;
    }
  }
  //If we get SMC in a non-initial substate send a security
  //mode failure because we are in the process of configuring
  else if( (tdsrrcsmc_substate != TDSRRCSMC_INITIAL) &&
           (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_SDU_IND) )
  {
    /* Get the message type */    
    message_choice = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                    cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

    /* Check if the received message is Security Mode Command message */
    if ( message_choice != T_tdsrrc_DL_DCCH_MessageType_securityModeCommand )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid msg rcvd in SMC in non-initial substate");
      return;
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received Security Mode Command Message in non-initial substate");

    /* Get the OTA message pointer */
    dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

    if (dcch_msg_ptr == NULL) /* DCCH ASN1 decoding failed */
    {
      tdsrrc_protocol_error_reject = TRUE ;
#if(!defined(T_WINNT))
      /* log the protocol error */
      tdsrrc_log_protocol_error((uint8)tdsrrc_get_state(),
                             (uint8)TDSRRC_PROCEDURE_SMC,
                             (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                             (uint8)TDSRRCLOG_ASN1_ENC_ERR
                            );
#endif

      /* Send a security mode failure command if there is a protocol error
       * in the received security mode command.Note that the trans id would be 
       * zero here which is fine because the network would ignore it for ASN1
       * decoding error cause
       */
      tdsrrcsmc_failure_type = TDSRRCSMC_PROTOCOL_ERROR; 
      tdsrrcsmc_prot_err_cause = tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError;
      tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);
    }
    else /* ASN1 decoding is successful */
    {
      /* Extract the transaction id */
      if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                T_tdsrrc_SecurityModeCommand_r3)
      {           
        rrcsmc_trans_id = dcch_msg_ptr->message.u.securityModeCommand.u.
                        r3.securityModeCommand_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                T_tdsrrc_SecurityModeCommand_later_than_r3)
      {
        rrcsmc_trans_id = dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.rrc_TransactionIdentifier;
      }
      else
      {
        rrcsmc_trans_id = 0;
      }
#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
     /* Need to process the SRNS relocatio fault tolerance      *
      * At this scenario: two adjacent SMC will be sent to UE   *
      * due to parallel RAU and LAU. The second one will be     *
      * dropped in the non-fault tolerance case                 */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"NW sends SMC msg in parallel,need to do fault tolerance");
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send L2ACK mannually,and drop the coming L2ACK with MUI=%d",smc_cmp_mui_drop);

      /*Need to try out the second SMC and make sure it is correct*/


        if(dcch_msg_ptr->message.u.securityModeCommand.t == 
                  T_tdsrrc_SecurityModeCommand_later_than_r3)
        {
          if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
          {
            if(dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.
              criticalExtensions.t == T_tdsrrc_SecurityModeCommand_criticalExtensions_1_r7)
            {
              second_smc_initial_trial = tdsrrcsmc_try_integrity_for_smc_r7(cmd_ptr);
            }
          }
        }
        else
        {
          second_smc_initial_trial = tdsrrcsmc_try_integrity_for_smc(cmd_ptr);
        }



      if(second_smc_initial_trial == SUCCESS)
      {

        /*Need to consider ciphering, so not call tdsrrcsmc_process_wait_for_l2_ack_substate only*/
        /*create internal cmd to emulate the L2ACK*/
        tdsrrcsmc_temp_l2ack_cmd_ptr = (tdsrrc_cmd_type *)tdsrrc_get_int_cmd_buf();
        tdsrrcsmc_temp_l2ack_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DOWNLINK_L2ACK_IND;
        tdsrrcsmc_temp_l2ack_cmd_ptr->cmd.downlink_l2ack_ind.status = SUCCESS;
        tdsrrcsmc_temp_l2ack_cmd_ptr->cmd.downlink_l2ack_ind.mui = smc_cmp_mui_drop;
        /*Filling the temp command ptr and call DCH/FACH handler manually*/
        rrc_state = tdsrrc_get_state();
        if(rrc_state == TDSRRC_STATE_CELL_DCH)
        {
          tdsrrcsmc_process_cell_dch_state(tdsrrcsmc_temp_l2ack_cmd_ptr);
        }
        else if(rrc_state == TDSRRC_STATE_CELL_FACH)
        {
          tdsrrcsmc_process_cell_fach_state(tdsrrcsmc_temp_l2ack_cmd_ptr); 
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong state for receiving the L2ACK with MUI=%d",smc_cmp_mui_drop);
          
        }

        /*mark MUI for drop here*/
        smc_cmp_mui_drop_status = TRUE;  /*sending of second SMC Complete needs to check the status*/

        /*release the ptr*/
        if(tdsrrcsmc_temp_l2ack_cmd_ptr != NULL)
        {
          tdsrrc_free_cmd_buf(tdsrrcsmc_temp_l2ack_cmd_ptr);
          tdsrrcsmc_temp_l2ack_cmd_ptr = NULL;
        }

        /*Now processing the TDSRRC_DOWNLINK_SDU_IND for second SMC*/
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"After emulate the L2ACK for first SMC,sub-state is %d",tdsrrcsmc_substate);
        if(rrc_state == TDSRRC_STATE_CELL_DCH)
        {
          tdsrrcsmc_process_cell_dch_state(cmd_ptr);
        }
        else if(rrc_state == TDSRRC_STATE_CELL_FACH)
        {
          tdsrrcsmc_process_cell_fach_state(cmd_ptr); 
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrong state for processing the TDSRRC_DOWNLINK_SDU_IND");
        }

      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"NW sends SMC msg in parallel,but IP check failure for second SMC");
      }
#else

      /* Set the failure cause and send the failure message */
      tdsrrcsmc_failure_type = TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG; 
      tdsrrcsmc_send_security_mode_failure(rrcsmc_trans_id);          
          
#endif  /*FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE*/
    }
  }
  else
  {
    switch(cmd_ptr->cmd_hdr.cmd_id )
    {
/*lint -e408*/
      case TDSRR_CIPHER_SYNC:
/*lint +e408*/
        /*On receiving TDSRR_CIPHER_SYNC from GSM-RR check & update any keys 
         *received in other-RAT right here
         */
        tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat(RRC_CS_DOMAIN_CN_ID);
        break;

#ifdef FEATURE_TDSCDMA_SRNS_SMC_FAULT_TOLERANCE
      case TDSRRC_DOWNLINK_L2ACK_IND:
        if(smc_cmp_mui_drop_status == TRUE)
        {
          if(smc_cmp_mui_drop == cmd_ptr->cmd.downlink_l2ack_ind.mui)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"!!!Ignore and drop the L2ACK for with MUI=%d. Return now!!!",smc_cmp_mui_drop);
            smc_cmp_mui_drop_status = FALSE;
            return;
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"!!!This L2ACK with MUI=%d is for second SMC,and the first one is missing!!!",smc_cmp_mui_drop);
          }
        }
        /*do not break here and needs to fall into following*/
#endif
       
      default:
      {
        //get the rrc state
        rrc_state = tdsrrc_get_state();
    
        switch (rrc_state)
        {
          case TDSRRC_STATE_CELL_DCH:  
            /* CELL_FACH and CELL_DCH only valid states to receive 
               SECURITY MODE COMMAND message */
            tdsrrcsmc_process_cell_dch_state(cmd_ptr); 
            break;
      
          case TDSRRC_STATE_CELL_FACH:    
            /* CELL_FACH and CELL_DCH only valid states to receive 
               SECURITY MODE COMMAND message */
            tdsrrcsmc_process_cell_fach_state(cmd_ptr); 
            break;
      
          case TDSRRC_STATE_CELL_PCH:
          case TDSRRC_STATE_URA_PCH:
            tdsrrcsmc_process_pch_state(cmd_ptr); 
            break;
      
          case TDSRRC_STATE_DISCONNECTED:
          case TDSRRC_STATE_CONNECTING:
            /* SECURITY MODE COMMAND is valid only in CELL_DCH and CELL_FACH state */
            TDSRRC_MSG1(MSG_LEGACY_MED,"RRCSMC Msg received in Invalid State: %d",rrc_state); 
            break;
      
          default: 
            /* This should never happen */
            ERR_FATAL("RRCSMC Msg received in Invalid State: %d",rrc_state,0,0);
            break;
        }  
      }
      break;
    }
  }
} /* tdsrrcsmc_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcsmc_get_nv_items

DESCRIPTION
  This  function gets NV  item NV_RC_INTEGRITY_ENABLED  and 
  NV_RRC_CIPHERING_ENABLED and stores it in local variables
  used to determine if Integrity/Ciphering are supported or
  not

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcsmc_get_nv_items
(
void
)
{

  /*read integrity and ciphering ENABLE/DISABLE flags from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_RRC_INTEGRITY_ENABLED, 
                        (byte *) &tdsnv_integrity_enabled,
                        sizeof(tdsnv_integrity_enabled)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RRC_INTEGRITY_ENABLED is %d",tdsnv_integrity_enabled);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RRC_INTEGRITY_ENABLED,
                                   (byte *) &tdsnv_integrity_enabled,
                                   sizeof(tdsnv_integrity_enabled),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tdsnv_integrity_enabled = 1;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_INTEGRITY_ENABLED item error. Setting to default: %d",tdsnv_integrity_enabled);
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_RRC_INTEGRITY_ENABLED,set default %d",tdsnv_integrity_enabled);
  }

  if (tdsrrcnv_efs_read(TDS_RRC_NV_RRC_CIPHERING_ENABLED, 
                        (byte *) &tdsnv_ciphering_enabled_status,
                        sizeof(tdsnv_ciphering_enabled_status)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RRC_CIPHERING_ENABLED is %d",tdsnv_ciphering_enabled_status);
    /*set tdsnv_ciphering_enabled to 1, as most of the code 
      is under this flag, We have to support UEA0 even if this Flag is set OFF */
    tdsnv_ciphering_enabled = 1;
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RRC_CIPHERING_ENABLED,
                                   (byte *) &tdsnv_ciphering_enabled_status,
                                   sizeof(tdsnv_ciphering_enabled_status),
                                   TRUE))
    {
       tdsnv_ciphering_enabled = 1;  /*set ciphering enabled to 1 as 
                                       reading nv was unsuccessful*/

       tdsnv_ciphering_enabled_status = 0;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_CIPHERING_ENABLED item error. Setting to default: %d",tdsnv_ciphering_enabled_status);
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_RRC_CIPHERING_ENABLED,set default %d",tdsnv_ciphering_enabled_status);
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_init_procedure

DESCRIPTION
  This function initializes required parameters for SECURITY MODE COMMAND
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcsmc_init_procedure
(
void
)
{
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id;
  
  for (sub_id =0; (sub_id >= SYS_MODEM_AS_ID_1) && (sub_id < (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS); sub_id++)
  {
    tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
    tdsrrcsmc_go_to_smc_initial_substate();
    
    /*reset START values to 0's at power-up, we may not receive a SIM insert
      REQ sometimes if SIM is not present, needed for an emergency call*/
    tdsrrcsmc_init_start_value_at_power_up();
    tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
  }
#else
  tdsrrcsmc_go_to_smc_initial_substate();
  /*reset START values to 0's at power-up, we may not receive a SIM insert
    REQ sometimes if SIM is not present, needed for an emergency call*/
  tdsrrcsmc_init_start_value_at_power_up();
#endif

  /* register for state change ind when UE goes to idle disconnected*/
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_SMC,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);     

  /* register for state change ind when UE goes to CONNECTING*/
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_SMC,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_CONNECTING);     
} /* tdsrrcsmc_init_procedure */
/*===========================================================================

FUNCTION tdsrrcsmc_process_integrity_protection_mode_info

DESCRIPTION
  This procedure process Integrity Protection Mode Info.  It is called by 
  the foll. procedures.  RRCSMC will copy the IntegrityProtectionModeInfo and
  processes them.If IntegrityProtectionModeInfo indicates to start Integrity
  Potection, it will update the necessary paramters and start the Integrity.
  Otherwise it will declares invalid configuration.
  
  UMI, Cell Update, Cell Update Confirm, URA Update Confirm, Physical Channel 
  Reconfiguration, Radio Bearer Setup, Radio Bearer Reconfiguration  
  
  The above procedures will call this function in SMC to notify the SMC
  procedure of the Integrity Protection Info and start Integrity Protection.


DEPENDENCIES
  None.

RETURN VALUE
  Returns TDSRRCSMC_VALID_CONFIG if IntegrityProtectionModeInfo is 
  valid(Start only).
  Returns RRCSMC_INVALID_CONFIG if IntegrityProtectionModeInfo is invalid(Modify)

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
tdsrrcsmc_config_status_e_type tdsrrcsmc_process_integrity_protection_mode_info
(
struct tdsrrc_IntegrityProtectionModeInfo* integrityProtModeInfo
)
{
  /*Template for future use when we support SRNS relocation*/
  return TDSRRCSMC_INVALID_CONFIGURATION;
}/*tdsrrcsmc_process_integrity_protection_mode_info*/
/*lint +e715*/
/*===========================================================================

FUNCTION tdsrrcsmc_reset_smc_config_flag

DESCRIPTION
  This procedure resets the tdsrrcsmc_integrity_protection_info.reconfig FLAG.
  This is called by UMI, CU, RB, ASU procedures to indicate that they
  received an L2 Ack for their complete message which included the
  UL Activation Info Time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_reset_smc_config_flag
(
void
)
{
  /*Template for future use when we support SRNS relocation*/

}/*tdsrrcsmc_reset_smc_config_flag*/
/*===========================================================================

FUNCTION tdsrrcsmc_append_ul_integrity_prot_act_info

DESCRIPTION
  This procedure populates the UL Activation Info times into the ptr passed
  to it by the foll. procedures.
  
  UMI, Cell Update, Cell Update Confirm, URA Update Confirm, Physical Channel 
  Reconfiguration, Radio Bearer Setup, Radio Bearer Reconfiguration  
  
  The above procedures will call this function in SMC to get the UL Activation 
  Info while sending a Complete message.  The UL Activation Info will be used 
  by the UTRAN to start integrity protection on the Uplink.  



DEPENDENCIES
  None.

RETURN VALUE
  Returns SUCCESS if UL Activation Info is successfully populated.
  Returns FAILURE if UL Activation Info is not populated.

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
uecomdef_status_e_type tdsrrcsmc_append_ul_integrity_prot_act_info 
(
struct tdsrrc_IntegrityProtActivationInfo * ulActivationInfo
)
{

  /*Template for future use when we support SRNS relocation*/
  return SUCCESS;
}/*tdsrrcsmc_append_ul_integrity_prot_act_info*/
/*lint +e715*/

/*===========================================================================

FUNCTION tdsrrcsmc_is_cn_associated_with_any_est_rab

DESCRIPTION
  This function checks whether a given CN domain is 
  associated with any established RAB
DEPENDENCIES
  None

RETURN VALUE
  TRUE :If given CN comain I.D is associated with EST_RAB.
  FALSE:Otherwise

SIDE EFFECTS
  None
===========================================================================*/
static boolean tdsrrcsmc_is_cn_associated_with_any_est_rab
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  uint32 rab_count;
  boolean status=FALSE;

  /*Get the Cn domain associated with first RAB which uses re_est_timer*/
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if(tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0)
    {
      if(cn_domain_id == tdsrrc_est_rabs.rabs[rab_count].cn_domain)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RAB I.D = %d associated with CN I.D = %d",tdsrrc_est_rabs.rabs[rab_count].rab_id,cn_domain_id);
        status = TRUE;
        break;
      }
    }
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cn i.d=%d,is_cn_associated_with_any_est_rab = %d",cn_domain_id,status);
  return status;
}

/*===========================================================================

FUNCTION tdsrrcsmc_append_start_list_per_active_domain

DESCRIPTION
  This function initializes the START_LIST per active cn domain
  with the latest values to be sent to the UTRAN by CU  procedure.
  

DEPENDENCIES
  None.

RETURN VALUE
  This function returns SUCCESS if STARTlist is appended.
  This function returns FAILURE if STARTlist is not appended.

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_start_list_per_active_domain 
(
tdsrrc_STARTList *start_list_ext,
tdsrrcsmc_append_start_e_type start_type
)
{
  uint8 both_cn_domain_open = FALSE;
  rrc_cn_domain_identity_e_type active_cn_domain;
  uecomdef_status_e_type status;
  boolean is_cs_domain_open,is_ps_domain_open;
  tdsrrc_proc_e_type                 proc_id;
  tdsrrcllc_oc_process_state_e_type  process_state;  
  tdsrrcllc_oc_set_status_e_type     oc_status;
  byte start_value[TDSRRC_MAX_START_LENGTH];
  active_cn_domain = RRC_CS_DOMAIN_CN_ID;
  is_cs_domain_open = FALSE;
  is_ps_domain_open = FALSE;

  /*If the GCF flag is not set for this feature, we can simply consider both domains to be open*/
  
  oc_status = tdsrrcllc_get_ordered_config_state_and_proc(&proc_id, &process_state);

  if(FALSE == mcfg_gcf_nv_get_status())
  {

    if((TRUE == tdsrrcllc_reconfig_in_progress()) &&                                        
       (oc_status != TDSOC_NOT_SET) &&                                                      
       (proc_id == TDSRRC_PROCEDURE_RBE) &&                                                 
       (TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF == tdsrrcrbe_get_current_sub_state()) &&          
       (TRUE == tdsrrcsmc_get_start_value_to_transmit_flag_status()))                       
    {                                                                                       
      if(TRUE == tdsrrcsmc_get_calc_start_value(start_value))                               
      {                                                                                     
        status = tdsrrcsmc_append_already_computed_start_value_to_start_list(start_list_ext,
                                                       start_value,                         
                                                       start_type,                          
                                                       TRUE,                                
                                                       RRC_CS_DOMAIN_CN_ID);                
      }                                                                                     
      else                                                                                  
      {                                                                                     
        status =  tdsrrcsmc_append_start_value_to_start_list (start_list_ext,               
                                                       start_type,                          
                                                       TRUE,                                
                                                       RRC_CS_DOMAIN_CN_ID);                
       }                                                                                    
    }                                                                                       
    else                                                                                    

    {
      status =  tdsrrcsmc_append_start_value_to_start_list (start_list_ext,
                                                       start_type,
                                                       TRUE,
                                                       RRC_CS_DOMAIN_CN_ID);
    }
    return status;
  }
  
  /*Check if the variable "LATEST_CONFIGURED_CN_DOMAIN" has been initialised*/
  if(tdsrrcsmc_get_security_start_status() == TRUE)
  {
    /*Check if CS domains is in EST_RAB */
    if(tdsrrcsmc_is_cn_associated_with_any_est_rab(RRC_CS_DOMAIN_CN_ID) == TRUE)
    {
      is_cs_domain_open = TRUE;
    }
    else
    {
      if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_CS_DOMAIN_CN_ID)
      {
        is_cs_domain_open = TRUE;
      }
      else
      {
        is_cs_domain_open = FALSE;
      }
    }
  
    /*Check if PS domains is in EST_RAB */
    if(tdsrrcsmc_is_cn_associated_with_any_est_rab(RRC_PS_DOMAIN_CN_ID) == TRUE)
    {
      is_ps_domain_open = TRUE;
    }
    else
    {
      if(tdsrrcsmc_int_ciph_info.latest_config_cn_id == RRC_PS_DOMAIN_CN_ID)
      {
        is_ps_domain_open = TRUE;
      }
      else
      {
        is_ps_domain_open = FALSE;
      }
    }
  }
  else
  {
    /*latest_config_cn_id is not init, so include START value for CS domain 
     *as spec say include the START value for either the CS domain or 
     *the PS domain
     */
    is_cs_domain_open = TRUE;
    is_ps_domain_open = FALSE;
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Domain status, CS=%d,PS=%d,Latest cn i.d=%d",is_cs_domain_open,
                          is_ps_domain_open,tdsrrcsmc_int_ciph_info.latest_config_cn_id);

  if((is_cs_domain_open == FALSE)||(is_ps_domain_open == FALSE))
  {
    if(is_cs_domain_open == FALSE)
    {
      active_cn_domain = RRC_PS_DOMAIN_CN_ID;
    }
    else
    {
      active_cn_domain = RRC_CS_DOMAIN_CN_ID;
    }
  }
  else
  {
    both_cn_domain_open = TRUE;
  }


  if((TRUE == tdsrrcllc_reconfig_in_progress()) &&                                        
     (oc_status != TDSOC_NOT_SET) &&                                                      
     (proc_id == TDSRRC_PROCEDURE_RBE) &&                                                 
     (TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF == tdsrrcrbe_get_current_sub_state()) &&          
     (TRUE == tdsrrcsmc_get_start_value_to_transmit_flag_status()))                       
  {                                                                                       
    if(TRUE == tdsrrcsmc_get_calc_start_value(start_value))                               
    {                                                                                     
      status = tdsrrcsmc_append_already_computed_start_value_to_start_list(start_list_ext,
                                                       start_value,                       
                                                       start_type,                        
                                                       both_cn_domain_open,               
                                                       active_cn_domain);                 
    }                                                                                     
    else                                                                                  
    {                                                                                     
      status =  tdsrrcsmc_append_start_value_to_start_list (start_list_ext,               
                                                       start_type,                        
                                                       both_cn_domain_open,               
                                                       active_cn_domain);                 
    }                                                                                     
  }                                                                                       
  else                                                                                    

  {
    status =  tdsrrcsmc_append_start_value_to_start_list (start_list_ext,
                                                       start_type,
                                                       both_cn_domain_open,
                                                       active_cn_domain);
  }

  return status;
}
/*===========================================================================

FUNCTION tdsrrcsmc_append_start_value_to_start_list

DESCRIPTION
  This function initializes the START_LIST with the latest values to 
  be sent to the UTRAN by the RCE, CU  and ASU procedures. The START
  values for all CN domains for which we have received Integrity
  Protection Info will be sent.
  
  RCE, CU and ASU procedures will call this function in SMC.

DEPENDENCIES
  None.

RETURN VALUE
  This function returns SUCCESS if STARTlist is appended.
  This function returns FAILURE if STARTlist is not appended.

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_start_value_to_start_list 
(
 tdsrrc_STARTList *start_list,
tdsrrcsmc_append_start_e_type start_type
,boolean both_cn_domain_open
,rrc_cn_domain_identity_e_type active_cn_domain
)
{
  uint32 i, index;                          /* local var to store count*/

  /*local var to keep track of cn_domain*/
  rrc_cn_domain_identity_e_type domain_id; 
  
  if(both_cn_domain_open == TRUE)
  {
    start_list->n = tdsrrcsmc_int_ciph_info.num_cn_domains;
  }
  else
  {
    start_list->n = 1;
  }
  ALLOC_ASN1ARRAY1(&tdsenc_ctxt, start_list, tdsrrc_STARTSingle);

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"both_cn_domain_open:%d,active_cn_domain:%d",both_cn_domain_open,active_cn_domain);
  if (start_list->elem == NULL)
  {
    ERR_FATAL("Memory allocation failure",0,0,0);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"both_cn_domain_open:%d,active_cn_domain:%d",both_cn_domain_open,active_cn_domain);

  index = 0;

  /*copy the contents of cn_domain into int_ciph_info struct*/
  for (i=0; i<tdsrrcsmc_int_ciph_info.num_cn_domains; i++)
  {
    domain_id = tdsrrcsmc_int_ciph_info.int_ciph_info[i].domain_id;
    if(both_cn_domain_open == FALSE)
    {
      if(active_cn_domain != domain_id)
      {
        continue;
      }
    }
    if (domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      /* Update START values for CS domain */
      start_list->elem[index].cn_DomainIdentity = tdsrrc_CN_DomainIdentity_cs_domain;
    }
    else
    {
      /* Update START values for CS domain */
      start_list->elem[index].cn_DomainIdentity = tdsrrc_CN_DomainIdentity_ps_domain;
    }

    start_list->elem[index].start_Value.numbits =  TDSRRC_START_VALUE_LENGTH;

    //append start value for this domain
    tdsrrcsmc_append_start_per_domain ((byte *) (start_list->elem[index].start_Value.data),
                                    domain_id,
                                    start_type);

    if(i == (uint32) RRC_CS_DOMAIN_CN_ID)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended CS-START: 0x%x 0x%x 0x%x",start_list->elem[index].start_Value.data[0],
               start_list->elem[index].start_Value.data[1],
               start_list->elem[index].start_Value.data[2]);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended PS-START: 0x%x 0x%x 0x%x", start_list->elem[index].start_Value.data[0],
               start_list->elem[index].start_Value.data[1],
               start_list->elem[index].start_Value.data[2]);
    }
    if(both_cn_domain_open == FALSE ) 
    {
      break;
    }

    index++;
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION RRCSMC_EXTRACT_RETURN_HFN_PER_DOMAIN_AND_UPDATE_START_FLAG

DESCRIPTION
  This procedure does start value calculation for a given domain and 
  set the start value update flag.
  
  DEPENDENCIES
  None.

RETURN VALUE
  Updated start value. Calling func should apply offset to this value to use it for AM/UM/TM

SIDE EFFECTS
  None

===========================================================================*/
uint32 tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  uint32 updated_hfn;

  //if SMC is not received, start should never be re-calculated since 
  //count_c and count_i are not valid till a SMC is received..
  //so just return the START value read from the SIM
  if(tdsrrcsmc_int_ciph_info.int_start_status_for_conn[domain_id] == FALSE)
  {
    updated_hfn = 
    tdsrrcsmc_convert_START_to_count_c_i(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn, 0);
  }
  //otherwise, START needs to be re-calculated
  else
  {
    // Do start value calculation for this domain
    tdsrrcsmc_extract_latest_start_list_per_cn_domain(domain_id, TDSRRCSMC_UPDATE_START_LIST_LATEST);
  
    // Set the start value update flag as the start value has been re-calculated
    tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = TRUE;
  
    // Convert Start value to HFN
    updated_hfn = 
    tdsrrcsmc_convert_START_to_count_c_i(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].hfn_latest, 0);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Extracted new HFN %d for domain %d",updated_hfn,domain_id);
  return updated_hfn;
}

/*===========================================================================

FUNCTION tdsrrcsmc_update_hfn_for_rlc_rb

DESCRIPTION
  This procedure updates HFN for RLC size change feature
  
  DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_update_hfn_for_rlc_rb
(
  tdsrrc_RB_Identity rb_id, 
  tdsrlc_reestablish_e_type direction
)
{
  uint32 hfn_update;
  rlc_lc_id_type rlc_id;

  // Update Start value and get LC Id for this RB
  hfn_update = tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag(RRC_PS_DOMAIN_CN_ID);
  
  // Since this is for AM channel, shift bits to make it a AM HFN
  hfn_update >>= TDSRRC_RLC_AM_HFN_OFFSET;

  if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == (rlc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_id)))
  {
    ERR_FATAL("lc_id not found for rb_id %d",rb_id,0,0);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Updated HFN is %d for LC_ID %d, direction %d", hfn_update, rlc_id, direction);

  // Call RLC interface function to overwrite HFN value
  if(FAILURE == tdsrlc_rrc_overwrite_hfn(rlc_id, hfn_update, direction))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RLC HFN Update failed for lc_id %d",rlc_id);
  }
}  
/*===========================================================================

FUNCTION tdsrrcsmc_send_ul_ciphering_key_req

DESCRIPTION
   This function sends cmd to L1 for configuring UL cipher 
   keys.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_send_ul_ciphering_key_req
(
void
)
{
  if ((tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE )&&
      (tdsrrcsmc_ciphering_info.reconfig == TRUE)
     )
  {
    /* Send commands to L1 */
    (void) tdsrrcllc_cphy_ciphering_key_req();
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Error: cant config ul cipher key's,rlc_suspend=%d,cipher_reconfig=%d",tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag,tdsrrcsmc_ciphering_info.reconfig);
  }
}

/*===========================================================================

FUNCTION tdsrrcsmc_get_start_value_to_transmit_domain

DESCRIPTION
  This function returns the value of global 'tdsstart_value_to_transmit_domain'
  
DEPENDENCIES
  None.

RETURN VALUE
  CN Domain Identity

SIDE EFFECTS
  None

===========================================================================*/
rrc_cn_domain_identity_e_type tdsrrcsmc_get_start_value_to_transmit_domain(void)
{
  return(tdsstart_value_to_transmit_domain);
}
/*===========================================================================

FUNCTION tdsrrcsmc_get_security_start_status

DESCRIPTION
  This function will look to see if security is started.
  This includes check for ciphering or integrity active 
  for any CN domain.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If security is already stated for any CN domain
  FALSE: other wise.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_get_security_start_status(void)
{
  uint8 cn_id;
  boolean status = FALSE;
  
  /*Check to see if I.P is started*/
  if(tdsrrcsmc_integrity_protection_info.ip_status == TDSRRCSMC_SECURITY_STARTED)
  {
    status = TRUE;
  }
  
  if(status == FALSE)
  {
    /*loop through to see is ciphering is started for any CN domain*/
    for(cn_id = 0;cn_id <RRC_MAX_CN_DOMAINS;cn_id++ )
    {
      if(tdsrrcsmc_int_ciph_info.ciph_status[cn_id] == TDSRRCSMC_SECURITY_STARTED)
      {
        status = TRUE;
        break;
      }
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Security started status=%d",status);
  return status;
}

/*===========================================================================

FUNCTION tdsrrcsmc_return_saved_security_key_set_status

DESCRIPTION
  This function will return status of security key set 
  generated in other RAT & saved in a buffer for CS domain.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcsmc_return_saved_security_key_set_status
(
  rrc_cn_domain_identity_e_type domain_id,
  sys_modem_as_id_e_type        sub_id
)
{
  boolean status = FALSE;

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type curr_sub_id;
  curr_sub_id = tdsrrc_get_as_id();
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
#endif
  if((domain_id == RRC_CS_DOMAIN_CN_ID)&&
    (tdssaved_security_key_set.grat_key_set_valid == TRUE))
  {
    /*We have one key stored in buffer, so return status as TRUE*/
    status = TRUE;
  }
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
#endif
  return(status);
}
/*===========================================================================

FUNCTION tdsrrcsmc_reset_saved_security_key_set_status

DESCRIPTION
  This function will reset status of security key set 
  generated in other RAT & saved in a buffer for CS domain.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_reset_saved_security_key_set_status
(
  sys_modem_as_id_e_type        sub_id
)
{
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type curr_sub_id;
  curr_sub_id = tdsrrc_get_as_id();
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
#endif
  /*Reset saved security key set status flag*/
  tdssaved_security_key_set.grat_key_set_valid = FALSE;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrcsmc_set_smc_security_info_to_related_sub_params(sub_id);
  tdsrrcsmc_set_smc_security_info_to_global_params(curr_sub_id);
#endif
}
/*===========================================================================

FUNCTION tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat

DESCRIPTION
  This function will move security key sets(generated in other RAT for CS domain ) from
  buffer to ciphering & integrity db if any.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_check_and_update_security_key_set_gen_in_other_rat
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  if(tdsrrcsmc_return_saved_security_key_set_status(
       domain_id,tdsrrc_get_as_id()) == TRUE)
  {
    tdsrrc_state_e_type rrc_state = tdsrrc_get_state(); /* local var to store rrc state*/
    /*local ciphering info type used to init global vars*/
    tdsrrcsmc_security_config_info_type *local_int_ciph_info_ptr; 
  
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Security key sets from buffer to security DB");
    local_int_ciph_info_ptr = &(tdsrrcsmc_int_ciph_info);
        
    /*save the earlier keys in to a var.  We need this to calculate MAC for some
        SRB's whose activation time has not yet equalled the new activation times*/
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key_old, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
           local_int_ciph_info_ptr->int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key,
           RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
   
    /*copy contents of integrity_key to int_ciph_info struct for this CN DOMAIN
     */
    memscpy(local_int_ciph_info_ptr->int_ciph_info[RRC_CS_DOMAIN_CN_ID].integrity_key, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
           tdssaved_security_key_set.integrity_key,
           RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
  
   
    /*copy contents of ciphering_key to int_ciph_info struct
     *for this CN DOMAIN.  If we are currently using ciph key 1 then copy
    new keys to key0.  else copy to 1 and set new_key flag for this
    domain id to true
    */
    if (tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key_used == 1)
    {
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[0], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
             tdssaved_security_key_set.ciphering_key,
             RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
    }
    else
    {
      memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.key[1], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
             tdssaved_security_key_set.ciphering_key,
             RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
    } 
    tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.new_key=TRUE;
  
    if ((rrc_state == TDSRRC_STATE_CELL_DCH) ||
        (rrc_state == TDSRRC_STATE_CELL_FACH))
    {/*This fucntion should be called only in IDLE state, but good to have this code*/
      tdsrrcsmc_int_ciph_info.int_ciph_info[RRC_CS_DOMAIN_CN_ID].ciph_keys_info.
      new_key_for_start_value = TRUE;
    }

    /*Print integrity key here*/
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY,
                                   tdssaved_security_key_set.integrity_key,
                                   tdssaved_security_key_set.ciphering_key);
  
    /*Print ciphering key here*/
    tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY,
                                   tdssaved_security_key_set.integrity_key,
                                   tdssaved_security_key_set.ciphering_key);

    tdsrrcsmc_reset_saved_security_key_set_status(tdsrrc_get_as_id());
    /*any previously sent start value is obliterated.. we should start using 0's from
      now onwards.. hence set start_value_update_flag to false*/
    //  tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;      ??????
    /* Fix for CR 195046 */
    if((rrc_state != TDSRRC_STATE_CELL_FACH) &&
        (rrc_state != TDSRRC_STATE_CELL_DCH)
      )
    {
      /*any previously sent start value is obliterated.. we should start using 0's from
       now onwards.. hence set start_value_update_flag to false*/
      tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;  
    }
  }
  else
  {
    /*Do nothing here, as there are no security keys generated in other RAT */
  }
}


/*===========================================================================

FUNCTION tdsrrcsmc_send_continue_req

DESCRIPTION
  This function constructs the CRLC_UL_CONTINUE_REQ primitive and
  sends it to L2 UL Task.
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_send_continue_req
(
void
)
{
  /*counter*/
  uint32 i;  
  tdsl2_ul_cmd_type  *rlc_ptr = NULL;
  tdsl2_dl_cmd_type  *rlc_ptr_dl = NULL;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue STOPPED RLC RB's (TDSCRLC_UL_CONTINUE_REQ)");

  if (tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE )
  {
    /*Send CONTINUE Req to RLC */
    /* Get L2 UL command buffer */
    if ((rlc_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending TDSCRLC_UL_RESUME_REQ ",0,0,0);
    }
  
    /* Fill in the Command Header */
    rlc_ptr->cmd_hdr.cmd_id = TDSCRLC_UL_CONTINUE_REQ;
  
    /* Fill in the Command Data */
    rlc_ptr->cmd_data.ul_continue.nchan =
      tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;
  
    for(i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
    {
      rlc_ptr->cmd_data.ul_continue.channel[i] =
        tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id;
    }
    /* Send command to L2 UL Task */
    tdsl2_ul_put_cmd(rlc_ptr);


    if ((rlc_ptr_dl = tdsl2_dl_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending CRLC_DL_RESUME_REQ ",0,0,0);
    }


    /*Now send the DL continue request*/
    
    /* Fill in the Command Header */
    rlc_ptr_dl->cmd_hdr.cmd_id = TDSCRLC_DL_CONTINUE_REQ;
  
    /* Fill in the Command Data */
    rlc_ptr_dl->cmd_data.dl_continue.nchan =
      tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan;

    for(i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan; i++)
    {
      rlc_ptr_dl->cmd_data.dl_continue.channel[i] =
        tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[i];
    }
    /* Send command to L2 UL Task */
    tdsl2_dl_put_cmd(rlc_ptr_dl);

    /*Now set the flag to FALSE*/
    
    tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = FALSE;           
    tdsrrcsmc_ciphering_info.reconfig = FALSE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error: No suspended RLC RB's to resume");
  }
}


/*===========================================================================

FUNCTION tdsrrcsmc_send_continue_req_srns

DESCRIPTION
  This function constructs the CRLC_UL_CONTINUE_REQ primitive and
  sends it to L2 UL Task.
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_send_continue_req_srns
(
  tdsrrcsmc_stop_or_resume_rlc_cmd_type rlc_cmd 
)
{
  /*counter*/
  uint32 i;  
  tdsl2_ul_cmd_type  *rlc_ptr_ul = NULL;
  tdsl2_dl_cmd_type  *rlc_ptr_dl = NULL;

  if((tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag != TRUE ) && (tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag != TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Stopped RLC RB to continue for SRNS");
    return; 
  }


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Continue STOPPED RLC RB's (TDSCRLC_UL_CONTINUE_REQ) for SRNS,with rlc_cmd = 0x%x",rlc_cmd);

  if(tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan <= 0)
  {
    tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan = 0;
    tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = FALSE;
    tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No suspended/stopped RB to continue for SRNS");
    return;
  }

  if ( (tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag == TRUE ) ||(tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag == TRUE))
  {
    /*Send CONTINUE Req to RLC */
    /* Get L2 UL command buffer */
    if ((rlc_ptr_ul = tdsl2_ul_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending TDSCRLC_UL_RESUME_REQ ",0,0,0);
    }
    if ((rlc_ptr_dl = tdsl2_dl_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending CRLC_DL_RESUME_REQ ",0,0,0);
    }
    /* Fill in the Command Header */
    rlc_ptr_ul->cmd_hdr.cmd_id = TDSCRLC_UL_CONTINUE_REQ;

    /* Fill in the Command Header */
    rlc_ptr_dl->cmd_hdr.cmd_id = TDSCRLC_DL_CONTINUE_REQ;  

    /*RESUME */
    if(rlc_cmd == TDSRRCSMC_RESUME_NON_TM_RB)
    {
      /* Fill in the Command Data */
      rlc_ptr_ul->cmd_data.ul_continue.nchan =
        tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan;
    
      for(i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan; i++)
      {
        rlc_ptr_ul->cmd_data.ul_continue.channel[i] =
          tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.suspend_stop[i].lc_id;
      }
  
      /*Now send the DL continue request*/
      /* Fill in the Command Data */
      rlc_ptr_dl->cmd_data.dl_continue.nchan =
        tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan;
  
      for(i=0; i < tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan; i++)
      {
        rlc_ptr_dl->cmd_data.dl_continue.channel[i] =
          tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.lc_id[i];
      }

  
      /*Now set the flag to FALSE*/
      
      tdsrrcsmc_rb_suspend_or_stop_info.rlc_rb_suspended_or_stopped_flag = FALSE;  
      tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag = FALSE;         
      tdsrrcsmc_ciphering_info.reconfig = FALSE;
    }
    else if(rlc_cmd == TDSRRCSMC_RESUME_SRB2)
    {
            /* Fill in the Command Data */
      rlc_ptr_ul->cmd_data.ul_continue.nchan = 1;

/*We don't change the number and SRB2 will be continue again when continue all AM RBs.
 *This will save us to shift the whole structure for "suspend_or_stopped_info"
 *RLC could handle the SRB2 re-continue case
 */
/*
      if(tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan > 0)
      {
        tdsrrcsmc_rb_suspend_or_stop_info.suspend_or_stopped_info.nchan--;
      }
*/


      /*only one change got continued*/
      rlc_ptr_ul->cmd_data.ul_continue.channel[0] = tdsrrclcm_get_ul_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);
      
      /*Now send the DL continue request*/
      /* Fill in the Command Data */
      rlc_ptr_dl->cmd_data.dl_continue.nchan = 1;
/*
      if(tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan > 0)
      {
        tdsrrcsmc_rb_suspend_or_stop_info.dl_stop_info.nchan--;
      }
*/  
      rlc_ptr_dl->cmd_data.dl_continue.channel[0] = tdsrrclcm_get_dl_rlc_lc_id(DCCH_AM_RADIO_BEARER_ID);    

      /*Now set the flag to FALSE*/
      tdsrrcsmc_rb_suspend_or_stop_info.rlc_srb2_suspended_or_stopped_flag = FALSE;           
      /*Do not do following unless needed.*/
      /*tdsrrcsmc_ciphering_info.reconfig = FALSE;*/

    }

    /* Send command to L2 UL Task */
    tdsl2_ul_put_cmd(rlc_ptr_ul);
  
    /* Send command to L2 UL Task */
    tdsl2_dl_put_cmd(rlc_ptr_dl);

  
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error: No suspended RLC RB's to resume");
  }
}


/*===========================================================================

FUNCTION   tdsrrcsmc_append_already_computed_start_value_to_start_list

DESCRIPTION

  This function appends start list and takes care of the already calculated start value of 
  a given domain which will be included in msg and hence it doesnt re-compute the 
  start value for that domain.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_already_computed_start_value_to_start_list
(
  tdsrrc_STARTList *start_list_ext,
  byte start_value[TDSRRC_MAX_START_LENGTH], /* start_value should always have a valid value */
  tdsrrcsmc_append_start_e_type append_start_type
  ,boolean both_cn_domain_open
  ,rrc_cn_domain_identity_e_type active_cn_domain
)
{
  /* local var to store count*/
  uint32 i,index;              
  
  rrc_cn_domain_identity_e_type domain_id; 
  /* Update the pointer to the list and allocate memory */


  if(both_cn_domain_open == FALSE ) 
  {
    start_list_ext->n = 1;
    ALLOC_ASN1ARRAY(&tdsenc_ctxt,start_list_ext,tdsrrc_STARTSingle);
  }
  else
  {
    start_list_ext->n = tdsrrcsmc_int_ciph_info.num_cn_domains;
    ALLOC_ASN1ARRAY(&tdsenc_ctxt,start_list_ext,tdsrrc_STARTSingle);
  }
  index = 0;
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"start_type:%d, both_cn_domain_open:%d active_cn_domain:%d",
            append_start_type,
            both_cn_domain_open,
            active_cn_domain);

  /*copy the contents of cn_domain into int_ciph_info struct*/
  for (i=0; i<tdsrrcsmc_int_ciph_info.num_cn_domains; i++)
  {
    domain_id = tdsrrcsmc_int_ciph_info.int_ciph_info[i].domain_id;
    if(both_cn_domain_open == FALSE)
    {
      if(active_cn_domain != domain_id)
      {
        continue;
      }
    }
    if (domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      /* Update START values for CS domain */
      start_list_ext->elem[index].cn_DomainIdentity = tdsrrc_CN_DomainIdentity_cs_domain;
    }
    else
    {
      /* Update START values for CS domain */
      start_list_ext->elem[index].cn_DomainIdentity = tdsrrc_CN_DomainIdentity_ps_domain;
    }

    start_list_ext->elem[index].start_Value.numbits =  TDSRRC_START_VALUE_LENGTH;

    //append start value for this domain
    if ((domain_id == RRC_CS_DOMAIN_CN_ID) &&
        (RRC_CS_DOMAIN_CN_ID == tdsrrcsmc_get_start_value_to_transmit_domain()))
    {
      /* Copy the contents of the start value that was calculated for ps domain */
      memscpy(start_list_ext->elem[index].start_Value.data, sizeof(start_list_ext->elem[index].start_Value.data),
             start_value,
             TDSRRC_MAX_START_LENGTH);
    }
    else if ((domain_id == RRC_PS_DOMAIN_CN_ID)&&
             (RRC_PS_DOMAIN_CN_ID == tdsrrcsmc_get_start_value_to_transmit_domain()))
    {
      /* Copy the contents of the start value that was calculated for ps domain */
      memscpy(start_list_ext->elem[index].start_Value.data, sizeof(start_list_ext->elem[index].start_Value.data),
             start_value,
             TDSRRC_MAX_START_LENGTH);
    }
    else
    {
      tdsrrcsmc_append_start_per_domain(
        (byte *)(start_list_ext->elem[index].start_Value.data),
        domain_id,
        append_start_type);
    }

    if(i == (uint32) RRC_CS_DOMAIN_CN_ID)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended CS-START: 0x%x 0x%x 0x%x", start_list_ext->elem[index].start_Value.data[0],
               start_list_ext->elem[index].start_Value.data[1],
               start_list_ext->elem[index].start_Value.data[2]);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Appended PS-START: 0x%x 0x%x 0x%x", start_list_ext->elem[index].start_Value.data[0],
               start_list_ext->elem[index].start_Value.data[1],
               start_list_ext->elem[index].start_Value.data[2]);
    }
	index++;
    if(both_cn_domain_open == FALSE ) 
    {
 
      break;
    }

  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrcsmc_append_start_list_with_already_computed_start

DESCRIPTION

  This function appends start list and takes care of the already calculated start value of 
  a given domain which will be included in msg and hence it doesnt re-compute the 
  start value for that domain.

DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS or FAILURE

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_start_list_with_already_computed_start
(
  tdsrrc_STARTList *start_list_ext,
  byte start_value[TDSRRC_MAX_START_LENGTH], /* start_value should always have a valid value */
  tdsrrcsmc_append_start_e_type append_start_type
)
{
  uecomdef_status_e_type status;

  status =  tdsrrcsmc_append_already_computed_start_value_to_start_list (start_list_ext,
                                                     start_value,
                                                     append_start_type
                                                     ,TRUE                                         
                                                     ,RRC_CS_DOMAIN_CN_ID    /*dummy value. Doesn't matter b'coz both 
                                                              *domain are considered to be open anyways*/
                                                      );
  return status;  
}

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r8

DESCRIPTION
  This function extracts ip check info/integrity protection mode info/ciphering mode info from SRNS 
  relocation triggering messages for Release 8 version of OTAs. In summary, input srns_info has to be filled
  from OTA in cmd_ptr.

  NOTE:  There is no R8 UTRANMobilityInformation and URAUpdateConfirm messages in ASN 8.6.
  Right now, only 5 Reconfig OTAs + CU CNF can do SRNS relocation for R8 OTA.

DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None
===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r8
(
  tdsrrcsmc_srns_relocation_security_info_r7_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;           /*By default, return status is SUCCEES*/
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                                                    /*Local variable to store message type */             
  
  /* Get the decoded OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /*By default, set integrity and ciphering info status as FALSE in srns_info*/
  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          

  /*SRNS-Reloc is posssible through DCCH messages only, so check for DL SDU type.*/
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  { 
    case tdsrrc_DL_DCCH_Message_PDU :

      /*Extract actual OTA type from cmd_ptr*/
      msg_type = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
          cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

      switch (msg_type)
      {
        case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        {
          tdsrrc_CellUpdateConfirm_r8_IEs *cucnf_ptr = 
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,cellUpdateConfirm,cellUpdateConfirm_r8);
      
          if(TDSRRCRB_R8_MSG_IE_PRESENT(cucnf_ptr,tdsrrc_CellUpdateConfirm_r8_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(cucnf_ptr,tdsrrc_CellUpdateConfirm_r8_IEs,cipheringModeInfo))
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
          }
        }
        break;

        case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
        {
          tdsrrc_RadioBearerReconfiguration_r8_IEs *rbrc_ptr = 
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,radioBearerReconfiguration,radioBearerReconfiguration_r8);

          if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rbrc_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(rbrc_ptr,tdsrrc_RadioBearerReconfiguration_r8_IEs,cipheringModeInfo))
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rbrc_ptr->cipheringModeInfo);
          }
        }
        break;
 
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
        {
          tdsrrc_RadioBearerRelease_r8_IEs* rb_release_ptr = 
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,radioBearerRelease,radioBearerRelease_r8);
 
          if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_release_ptr,tdsrrc_RadioBearerRelease_r8_IEs,cipheringModeInfo))
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info =  &(rb_release_ptr->cipheringModeInfo);
          }
        }
        break;

        case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
        {
          tdsrrc_RadioBearerSetup_r8_IEs* rb_setup_ptr = 
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,radioBearerSetup,radioBearerSetup_r8);

          if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_setup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(rb_setup_ptr,tdsrrc_RadioBearerSetup_r8_IEs,cipheringModeInfo))
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rb_setup_ptr->cipheringModeInfo);
          }
        }
        break;
      
        case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
        {
          tdsrrc_TransportChannelReconfiguration_r8_IEs *tcreconfig_ptr =
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,transportChannelReconfiguration,transportChannelReconfiguration_r8);

          if(TDSRRCRB_R8_MSG_IE_PRESENT(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r8_IEs,integrityProtectionModeInfo) )
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info =   &(tcreconfig_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(tcreconfig_ptr,tdsrrc_TransportChannelReconfiguration_r8_IEs,cipheringModeInfo) )
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info =  &(tcreconfig_ptr->cipheringModeInfo);
          }
        }
        break;
      
        case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
        {
          tdsrrc_PhysicalChannelReconfiguration_r8_IEs *pcreconfig_ptr =
            TDSRRC_EXTRACT_R8_MSG_POINTER(dcch_msg_ptr,physicalChannelReconfiguration,physicalChannelReconfiguration_r8);

          if(TDSRRCRB_R8_MSG_IE_PRESENT(pcreconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,integrityProtectionModeInfo))
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(pcreconfig_ptr->integrityProtectionModeInfo);
          }
          if(TDSRRCRB_R8_MSG_IE_PRESENT(pcreconfig_ptr,tdsrrc_PhysicalChannelReconfiguration_r8_IEs,cipheringModeInfo))
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(pcreconfig_ptr->cipheringModeInfo);
          }
        }
        break;      
        case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
        {
           /* For uraUpdateConfirm, I.E integrityProtectionModeInfo is supported in R5/R7 messages only. */
           if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
              uraUpdateConfirm_r7.m.integrityProtectionModeInfoPresent )
           {
             srns_info->integrity_info_present = TRUE;
             srns_info->integrity_protection_mode_info = 
               &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
                 uraUpdateConfirm_r7.integrityProtectionModeInfo);
           }
           if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
              uraUpdateConfirm_r7.m.cipheringModeInfoPresent )
           {
             srns_info->ciphering_mode_info_present = TRUE;
             srns_info->ciphering_mode_info = 
               &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
                 uraUpdateConfirm_r7.cipheringModeInfo);
           }
         }
         break;
            
        default:
          TDSRRC_MSG1(MSG_LEGACY_MED,"DL DCCH MSG %d not supported for SRNS-reloc",msg_type);
          status = FAILURE;
          break;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc But DL SDU Type is %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      status = FAILURE;
      break;
  }

  /*Set Integrity Check-info only if Integrity info is present in the OTA.*/
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);      
  }

  return status;
}

#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r8

DESCRIPTION
  This function extracts ip check info/integrity protection mode info/ciphering mode info from SRNS 
  relocation triggering messages for Release 8 version of OTAs. In summary, input srns_info has to be filled
  from OTA in cmd_ptr.

  NOTE:  There is no R8 UTRANMobilityInformation and URAUpdateConfirm messages in ASN 8.6.
  Right now, only 5 Reconfig OTAs + CU CNF can do SRNS relocation for R8 OTA.

DEPENDENCIES
  None.

RETURN VALUE
  The function returns SUCCESS if extraction was successful
  The function returns FAILURE if extraction was unsucceful.

SIDE EFFECTS
  None
===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r9
(
  tdsrrcsmc_srns_relocation_security_info_r7_type * srns_info,
  tdsrrc_cmd_type *cmd_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;           /*By default, return status is SUCCEES*/
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;       /* Pointer to Downlink DCCH */
  uint8 msg_type;                                                    /*Local variable to store message type */             
  
  /* Get the decoded OTA message pointer */
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  /*By default, set integrity and ciphering info status as FALSE in srns_info*/
  srns_info->integrity_info_present = FALSE;
  srns_info->ciphering_mode_info_present = FALSE;          

  /*SRNS-Reloc is posssible through DCCH messages only, so check for DL SDU type.*/
  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  { 
    case tdsrrc_DL_DCCH_Message_PDU :

      /*Extract actual OTA type from cmd_ptr*/
      msg_type = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
          cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

      switch (msg_type)
      {
        case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        {
          tdsrrc_CellUpdateConfirm_r9_IEs *cucnf_ptr = 
            &(dcch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.cellUpdateConfirm_r9);
          if(cucnf_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(cucnf_ptr->integrityProtectionModeInfo);
          }
          if(cucnf_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(cucnf_ptr->cipheringModeInfo);
          }
        }
        break;

        case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
        {
          tdsrrc_RadioBearerReconfiguration_r9_IEs *rbrc_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.radioBearerReconfiguration_r9);

          if(rbrc_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rbrc_ptr->integrityProtectionModeInfo);
          }
          if(rbrc_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rbrc_ptr->cipheringModeInfo);
          }
        }
        break;
 
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
        {
          tdsrrc_RadioBearerRelease_r9_IEs* rb_release_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.radioBearerRelease_r9);
 
          if(rb_release_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_release_ptr->integrityProtectionModeInfo);
          }
          if(rb_release_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info =  &(rb_release_ptr->cipheringModeInfo);
          }
        }
        break;

        case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
        {
          tdsrrc_RadioBearerSetup_r9_IEs* rb_setup_ptr = 
            &(dcch_msg_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.radioBearerSetup_r9);

          if(rb_setup_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(rb_setup_ptr->integrityProtectionModeInfo);
          }
          if(rb_setup_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(rb_setup_ptr->cipheringModeInfo);
          }
        }
        break;
      
        case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
        {
          tdsrrc_TransportChannelReconfiguration_r9_IEs *tcreconfig_ptr =
            &(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.transportChannelReconfiguration_r9);

          if(tcreconfig_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info =   &(tcreconfig_ptr->integrityProtectionModeInfo);
          }
          if(tcreconfig_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info =  &(tcreconfig_ptr->cipheringModeInfo);
          }
        }
        break;
      
        case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
        {
          tdsrrc_PhysicalChannelReconfiguration_r9_IEs *pcreconfig_ptr =
            &(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.
              criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
              u.criticalExtensions.u.r9.physicalChannelReconfiguration_r9);

          if(pcreconfig_ptr->m.integrityProtectionModeInfoPresent)
          {
            srns_info->integrity_info_present = TRUE;
            srns_info->integrity_protection_mode_info = &(pcreconfig_ptr->integrityProtectionModeInfo);
          }
          if(pcreconfig_ptr->m.cipheringModeInfoPresent)
          {
            srns_info->ciphering_mode_info_present = TRUE;
            srns_info->ciphering_mode_info = &(pcreconfig_ptr->cipheringModeInfo);
          }
        }
        break;      
        case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
        {
           /* For uraUpdateConfirm, I.E integrityProtectionModeInfo is supported in R5/R7 messages only. */
           if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
              uraUpdateConfirm_r7.m.integrityProtectionModeInfoPresent )
           {
             srns_info->integrity_info_present = TRUE;
             srns_info->integrity_protection_mode_info = 
               &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
                 uraUpdateConfirm_r7.integrityProtectionModeInfo);
           }
           if(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
              uraUpdateConfirm_r7.m.cipheringModeInfoPresent )
           {
             srns_info->ciphering_mode_info_present = TRUE;
             srns_info->ciphering_mode_info = 
               &(dcch_msg_ptr->message.u.uraUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r7.
                 uraUpdateConfirm_r7.cipheringModeInfo);
           }
         }
         break;
            
        default:
          TDSRRC_MSG1(MSG_LEGACY_MED,"DL DCCH MSG %d not supported for SRNS-reloc",msg_type);
          status = FAILURE;
          break;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL-DCCH msg only for SRNS-reloc But DL SDU Type is %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      status = FAILURE;
      break;
  }

  /*Set Integrity Check-info only if Integrity info is present in the OTA.*/
  if(srns_info->integrity_info_present == TRUE)
  {
    srns_info->integrity_check_info = &(dcch_msg_ptr->integrityCheckInfo);
  }

  return status;
}

#endif /*FEATURE_TDSCDMA_REL9*/

/*===========================================================================

FUNCTION  tdsrrcsmc_is_reconfig_in_progress

DESCRIPTION

  This function will check if SMC Procedure substate is reconfig in progress
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE: if SMC procedure is wating for reconfig in progress
  FALSE: othewise.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsmc_is_reconfig_in_progress
(
  void
)
{
  boolean status = FALSE;

  if(tdsrrcsmc_substate == TDSRRCSMC_WAIT_FOR_RECONFIG_CNF)
  {
    status = TRUE;
  }
  return status;
}


#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER

/*===========================================================================

FUNCTION tdsrrcsmc_save_security_key_set_from_lte

DESCRIPTION
  This function will move security key sets (generated in LTE for PS domain ) from
  buffer to ciphering & integrity db if any.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcsmc_save_security_key_set_from_lte
(
  tdsrrc_ho_ciph_req_type  ho_ciph_req,
  rrc_cn_domain_identity_e_type domain_id
)
{
  tdsrrcsmc_security_config_info_type *local_int_ciph_info_ptr = NULL; 

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2W:Updating Security key sets from buffer to security DB");
  local_int_ciph_info_ptr = &(tdsrrcsmc_int_ciph_info);
  
  /* Take a backup of keys and update */
  memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].integrity_key_old, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
         local_int_ciph_info_ptr->int_ciph_info[domain_id].integrity_key,
         RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
  
  memscpy(local_int_ciph_info_ptr->int_ciph_info[domain_id].integrity_key, RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte),
          ho_ciph_req.security_keys[domain_id].integrity_key,
         RRC_MAX_INTEGRITY_KEY_LENGTH*sizeof(byte) );
  
  /* Copy contents of ciphering_key to int_ciph_info struct for this CN DOMAIN.  
     * If we are currently using ciph key 1 then copy new keys to key0  
     * else copy to 1 and set new_key flag for this domain id to true
     */
  if (tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key_used == 1)
  {
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key[0], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
           ho_ciph_req.security_keys[domain_id].ciphering_key,
           RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
  }
  else
  {
    memscpy(tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.key[1], RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte),
           ho_ciph_req.security_keys[domain_id].ciphering_key,
           RRC_MAX_CIPHERING_KEY_LENGTH*sizeof(byte) );
    /*Print ciphering key here*/
  } 
  
  /* Need to check this flag in detail */
  tdsrrcsmc_int_ciph_info.int_ciph_info[domain_id].ciph_keys_info.new_key = TRUE;
  
  /*Print integrity key here*/
  tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_INTEGRITY_KEY, ho_ciph_req.security_keys[domain_id].integrity_key, ho_ciph_req.security_keys[domain_id].ciphering_key);
  tdsrrcsmc_print_cipher_int_key(TDSRRC_PRINT_CIPHERING_KEY, ho_ciph_req.security_keys[domain_id].integrity_key, ho_ciph_req.security_keys[domain_id].ciphering_key);

  /*any previously sent start value is obliterated.. we should start using 0's from
    now onwards.. hence set start_value_update_flag to false*/
  tdsrrcsmc_int_ciph_info.start_value_update_flag[domain_id] = FALSE;  
}

#endif /* FEATURE_LTE_TO_TDSCDMA */



/*===========================================================================

FUNCTION tdsrrcsmc_get_smc_substate

DESCRIPTION
The function returns the SMC sub-state for other procedures. 

DEPENDENCIES
  

RETURN VALUE
  SMC sub-state.

SIDE EFFECTS
  None

===========================================================================*/
tdsrrcsmc_substate_e_type tdsrrcsmc_get_smc_substate(void)
{
  /*Do not suggest to access the global directly*/
  return tdsrrcsmc_substate;
}
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
/*===========================================================================

FUNCTION tdsrrcsmc_set_smc_security_info_to_global_params

DESCRIPTION
The function set the SMC security info to global params according to sub_id.

tdssaved_security_key_set  = tdssaved_security_key_set_internal[sub_id];


DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_set_smc_security_info_to_global_params
(
  sys_modem_as_id_e_type sub_id
)
{
  tdssaved_security_key_set  = tdssaved_security_key_set_internal[sub_id];
  tdsrrcsmc_int_ciph_info = tdsrrcsmc_int_ciph_info_internal[sub_id];
}

/*===========================================================================

FUNCTION tdsrrcsmc_set_smc_security_info_to_related_sub_params

DESCRIPTION
The function set the SMC security info to related sub params according to sub_id.

tdssaved_security_key_set_internal[sub_id] = tdssaved_security_key_set;

DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_set_smc_security_info_to_related_sub_params
(
  sys_modem_as_id_e_type sub_id
)
{
  tdssaved_security_key_set_internal[sub_id] = tdssaved_security_key_set;
  tdsrrcsmc_int_ciph_info_internal[sub_id] = tdsrrcsmc_int_ciph_info;
}

/*===========================================================================

FUNCTION tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms

DESCRIPTION
The function set the SMC integrity protection info to related sub params according to sub_id.

  tdsrrcsmc_integrity_protection_info_internal[sub_id] = tdsrrcsmc_integrity_protection_info;

DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms
(
  sys_modem_as_id_e_type sub_id
)
{
  tdsrrcsmc_integrity_protection_info_internal[sub_id] = tdsrrcsmc_integrity_protection_info;
}

/*===========================================================================

FUNCTION tdsrrcsmc_set_smc_integrity_protection_to_global_params

DESCRIPTION
The function set the SMC integrity protection info to global params according to sub_id.

tdsrrcsmc_integrity_protection_info = tdsrrcsmc_integrity_protection_info_internal[sub_id];

DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_set_smc_integrity_protection_to_global_params
(
  sys_modem_as_id_e_type sub_id
)
{
  tdsrrcsmc_integrity_protection_info = tdsrrcsmc_integrity_protection_info_internal[sub_id];
}
#endif
