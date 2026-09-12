#ifndef TDSRRCSMC_H
#define TDSRRCSMC_H
/*===========================================================================
                  R R C  SECURITY MODE COMMAND

DESCRIPTION
  This header file contains function prototypes for Security Mode control
  procedure

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsmc.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/10/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.51
02/08/11   rl      Fixed the compilation warnings
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   sks     Added support for LTOW PSHO.
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/02/10   su      Provided fix to process TFCC message while another 
                   Reconfiguration is on going.
11/03/10   rl      Fixed compiler warnings
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsrrcasn1.h"
#include "tdsrrcllc.h"
#include "tdsrrcllci.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcintif.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcdt.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSMAC_D_HFN_LENGTH 24
#define TDSRRCSMC_INVALID_HFN_VALUE  0xffffffff /* invalid hfn value */

#define TDSRRC_MAX_SRB                  5
#define TDSRRC_START_VALUE_LENGTH       20    /* Length of the START Value in 
                                              bits */
#define TDSRRC_MAX_START_LENGTH         3     /* Length of the START Value in 
                                              bytes */
#define TDSRRC_MAX_MAC_LENGTH           4     /* Length in bytes of MAC */

#define TDSRRC_FRESH_LENGTH_IN_BITS     32    /* Len. of FRESH in bits*/
#define TDSRRC_MAC_LENGTH_IN_BITS       32    /* Len. of MAC in bits*/
#define TDSRRC_SEQ_NUM_LENGTH_IN_BITS   4     /* Len. of SEQ_NUM in bits*/


#define TDSKASUMI_SUPPORTED         0x0002    /* Const. Define for Kasumi algo.*/
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
#define TDSSNOW_3G_SUPPORTED          0x0004    /* Const. dfn. for UEA 2*/
#endif


#define TDSUEA_0_SUPPORTED          0x0001    /* Const. dfn. for UEA 0*/
#define TDSUEA_1_SUPPORTED          0x0002    /* Const. dfn. for UEA 1*/
#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
#define TDSUEA_2_SUPPORTED          0x0004    /* Const. dfn. for UEA 2*/
#endif

#define TDSRRC_HFN_BIT_MASK         0xFFFFFFF0
#define TDSRRC_SEQ_NUM_BIT_MASK     0xF
#define TDSRRC_UL_ACT_TIME_OFFSET   0x4
#define TDSRRC_MAX_UM_AM_SRB        0x4
#define TDSRLC_CIPH_ACT_SN          0x0
#define TDSRRCSMC_SUSPEND_UM_AM_0   0x0
#define TDSRRCSMC_MAX_KEYS_TO_L1    0x4
#define TDSMAX_CIPH_KEYS_PER_DOMAIN 0x2

#define TDSRRC_RLC_AM_HFN_OFFSET    12
#define TDSRRC_RLC_UM_HFN_OFFSET    7
#define TDSRRC_MAC_HFN_OFFSET       8

#define TDSRRC_RLC_UM_START_OFFSET    5
#define TDSRRC_RLC_TM_START_OFFSET    4

#ifdef FEATURE_LARGE_CIPH_ACT_SN_FOR_SRB2
  /* This feature was added as a workaround for a network issue.Since we always move to the 
     new cipher config immediately after SecurityModeComplete PDUs(spec recommended),any UL 
	 messages(Meas reports) can go out immediately after SecurityModeComplete on SRB2, since it 
	 is not suspended and we dont need to wait for the L2 Ack for SecurityModeComplete.
	 However in one of the networks it was noticed that the network was not able to handle 
	 these PDUs as it was in the middle of reconfiguring the lower layers with new ciphering 
	 config.This resulted in call drops due to network side RRC-RLC ciphering issues.
	 If this feature is turned ON,instead of starting the ciphering immediately after 
	 SecurityModeComplete,we would give an additional 2 PDU buffer which is 2 tti delay  
	 This would give network the time to finish configuring the lower layers.
	 It is NOT recommended to turn ON this feature.
  */
  #define TDSRLC_CIPH_ACT_SN_RB_2 0x4
#else
  #define TDSRLC_CIPH_ACT_SN_RB_2 0x2
#endif

/* 0x2C = 44. [1(integrity check info bit indicator)+36 (integrity check info)+
              5(msg type)+1(r3 indicator)+1(non-critical ext in r3 IE)] */
#define TDSDCCH_R3_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 44
                                          /* Bit position for integrity 
                                             protection mode info present in msgs*/
#define TDSNUM_OF_BITS_FOR_DCCH_R3_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1
                                          /* Number of bits for integrity
                                             prot bit-mask                */

#define TDSDL_DCCH_R3_INDICATOR_POS 42 /* 0x2A = 42. */
                              /* Bit position to indicate whether 
                              message contains 'r3' IE or 'later-than-r3' IE */
#define TDSNUM_OF_BITS_FOR_DL_DCCH_R3_INDICATOR 1

#define TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5_POS 45

#define TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R7_POS 46

#define TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_R5 1

/* 45. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_POS 45

#define TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN 1

/* 47. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(choice value '0' indicates r4)+
         1(mask bit for v4d0NonCriticalExt)
       ] */
#define TDSDL_DCCH_R4_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 47

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R4_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1


/* 46. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R5_POS 46

#define TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRITICAL_EXTN_CRITICAL_EXTN_R5 1

/* 49. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '0' indicates Critical extensions for r5)+
         2(mask bits for r5_add_ext_presen, and r5_nonCriticalExtensions_present)
       ] */
#define TDSDL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 49

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R5_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1

/* 47. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R6_POS 47

#define TDSNUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R6 1
/* 50. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '0' indicates Critical extensions for r6)+
         2(mask bits for r6_add_ext_presen, and r6_nonCriticalExtensions_present)
       ] */
#define TDSDL_DCCH_R6_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 50

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R6_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1

/* 48. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions)
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R7_POS 48

#define NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R7 1

/* 49. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions)  //later-than-r7
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R8_POS 49

#define NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_CRIT_EXTN_R8 1

/* 50. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '0' indicates Critical extensions for r7)+
         1(mask bits for r7_nonCriticalExtensions_present)
       ] */
#define TDSDL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 50

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R7_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1


/* 52. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '0' indicates Critical extensions for r8)+
         1(mask bits for r8_nonCriticalExtensions_present)
         1(mask bits for r8_nonCriticalExtensions_present)
       ] */
#define TDSDL_DCCH_R8_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 52

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R8_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1


/* 50. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions)
         1(value '1' indicates Critical extensions) //later-than-r8
       ] */
#define TDSDL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_ETC_R9_POS 50

#define NUM_OF_BITS_FOR_DL_DCCH_LATER_THAN_R3_CRIT_EXTN_CRIT_EXTN_ETC_R9 1

/* 53. [ 1(integrity check info bit indicator)+
        36(integrity check info)+
         5(msg type)+
         1(value '1' indicates later-than-r3 indicator)+
         2(transaction id)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '1' indicates Critical extensions)+
         1(value '0' indicates Critical extensions for r9)+
         1(mask bits for r9_nonCriticalExtensions_present)
         1(mask bits for r9_nonCriticalExtensions_present)
       ] */
#define TDSDL_DCCH_R9_INTEGRITY_PROTECTION_MODE_INFO_PRESENT_POS 53

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R9_INTEGRITY_PROTECTION_MODE_INFO_PRESENT 1



/* R3 indicator bit position without integrity check info:
   [ 1: integrity check info bit indicator
     5: msg type
   ] */
#define TDSDL_DCCH_R3_INDICATOR_WITHOUT_INT_CHECK_INFO_POS  6

/* IE 'CN Domain' position in R3 DDT message:
   [ 1: integrity check info bit indicator
     5: msg type
     1: r3 indicator bit
     1: non-critical extenstion in r3 branch
     2: transaction identifier
   ] */
#define TDSDL_DCCH_R3_DDT_CN_DOMAIN_IE_POS 10

#define TDSNUM_OF_BITS_FOR_DL_DCCH_R3_DDT_CN_DOMAIN_IE 1

#define TDSRRC_SN_MOD_VAL           16

/* S: Stored RRC-SN, A: Activation SN, R: received RRC-SN
   If this macro returns true then it means that activation time has expired.
 */
#define TDSRRC_CHECK_FOR_IP_ACT_TIME_EXPIRY(S,A,R)  (((16+(A)-(S)) & 0xF) <= ((16+(R)-(S)) & 0xF))

/* Number of RLC PDUs for SecurityModeComplete message */
#define TDSRLC_NUM_PDUS_FOR_SMCOMPLETE 2

/* Ciph Act SN for SRB2 : 2 PDUs for SMComplete + 2 PDUs for Measurement report */
#define TDSRLC_CIPH_SRB_2_ACT_SN_OFFSET 4

#define TDSRRC_DEFAULT_BIT_POS   0            /* Points to 0th position of
                                              bit string */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*==========================================================================
  Type for defining the substate of RRC SMC Procedure.
===========================================================================*/

/*Enum for different states of SMC procedure*/
typedef enum
{
  TDSRRCSMC_INITIAL,                 /* Initial Substate */
  TDSRRCSMC_WAIT_FOR_L2_ACK,         /* Wait for L2_ack state */
  TDSRRCSMC_WAIT_FOR_RECONFIG_CNF,   /* Wait for any Reconfig to be completed */
  TDSRRCSMC_WAIT_FOR_CIPH_CONFIG_CNF,/* Wait for LLC to configure lower layers
                                     with new ciphering information */
  TDSRRCSMC_WAIT_FOR_HO_STEP_1_CIPH_CONFIG_CNF,
  TDSRRCSMC_WAIT_RE_EST_SRNS_CNF,
  TDSRRCSMC_WAIT_SRNS_RELOC_L2_ACK_IND,
  TDSRRCSMC_WAIT_TILL_STANDBY_ENDS
}tdsrrcsmc_substate_e_type;

/*SMC Success or failure type*/
typedef enum
{
  TDSRRCSMC_SUCCESS,               /* Successful Status */
  TDSRRCSMC_FAILURE                /* Failure status */
}tdsrrcsmc_status_e_type;

/*ENUM to indicate to external procedures about the type of failure
  for Config received in Integrity_Protection_Info or 
  Cipher_Mode_Info*/
typedef enum
{
  TDSRRCSMC_VALID_CONFIG,
  TDSRRCSMC_UNSUPPORTED_CONFIG,
  TDSRRCSMC_PHYSICAL_CHANNEL_FAILURE,
  TDSRRCSMC_INCOMPATIBLE_SIMULTANEOUS_RECONFIG,
  TDSRRCSMC_PROTOCOL_ERROR,
  TDSRRCSMC_CELL_UPDATE_OCCURED,
  TDSRRCSMC_INVALID_CONFIGURATION,
  TDSRRCSMC_INCOMPLETE_CONFIGURATION
}tdsrrcsmc_config_status_e_type;

/*enum to indicate if SECURITY for Integrity/Ciphering has started or not*/
typedef enum
{
  TDSRRCSMC_SECURITY_STARTED,            /* Indicates IP/CIPH started */
  TDSRRCSMC_SECURITY_NOT_STARTED         /* Indicates IP/CIPH not started */
}tdsrrcsmc_security_status_e_type;

/*returns status to indicate if integrity protection is started or not*/
typedef enum
{
  TDSRRCSMC_IP_STARTED,            /* Indicates IP started */
  TDSRRCSMC_IP_NOT_STARTED         /* Indicates IP not started */
}tdsrrcsmc_ip_status_e_type;

/* Ciphering affected Type*/
typedef enum
{
  TDSCIPH_FIRST_TIME,
  TDSCIPH_NOT_STARTED,
  TDSCIPH_STARTED,
  TDSCIPH_RESTART
}tdsrrcsmc_affected_rb_e_type;

/* Appending Start Type*/
typedef enum
{
  TDSRRCSMC_EXTRACT_AND_APPEND_START,
  TDSRRCSMC_APPEND_START
}tdsrrcsmc_append_start_e_type;

/* Ciphering affected Type*/
typedef enum
{
  TDSRRCSMC_UPDATE_START_LIST,
  TDSRRCSMC_UPDATE_START_LIST_LATEST
}tdsrrcsmc_start_list_update_e_type;

/* SRNS Relocation IP-Check Status type */
typedef enum
{
  TDSRRCSMC_SRNS_SECURITY_INFO_INVALID,
  TDSRRCSMC_INCOMPATIBLE_SECURITY_RECONFIGURATION,
  TDSRRCSMC_SRNS_INTEGRITY_CHECK_SUCCESS,
  TDSRRCSMC_SRNS_INTEGRITY_CHECK_FAILURE
}tdsrrcsmc_srns_ip_check_status_e_type;

/* Type to indicate the command that triggers the RLC suspension*/
typedef enum
{
  TDSRRCSMC_SMC_CMD,
  TDSRRCSMC_SRNS_CMD
}tdsrrcsmc_suspend_cmd_type;


typedef enum
{
  /*stop all RLC for UM and AM RB*/
  TDSRRCSMC_STOP_NON_TM_RB,  
  /*stop RLC for SRB2 only*/
  TDSRRCSMC_STOP_SRB2,
  /*stop RLC for non-SRB2*/
  TDSRRCSMC_STOP_NON_SRB2,
  /*resume all RLC for UM and AM RB*/
  TDSRRCSMC_RESUME_NON_TM_RB,  
  /*resume RLC for SRB2 only*/
  TDSRRCSMC_RESUME_SRB2,
  /*resume RLC for non-SRB2*/
  TDSRRCSMC_RESUME_NON_SRB2
}tdsrrcsmc_stop_or_resume_rlc_cmd_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

extern boolean    tdsnv_integrity_enabled;     
extern boolean    tdsnv_ciphering_enabled;
extern boolean    tdsnv_ciphering_enabled_status;
/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/*global data struct which maintains ciphering information needed by
  rlc, mac and l1 layers
 */
typedef struct
{
  tdsrlc_ul_cipher_config_type rlc_ul_cipher_info;
  tdsrlc_dl_cipher_config_type rlc_dl_cipher_info;
  tdsmac_cipher_config_type    mac_cipher_info[RRC_MAX_CN_DOMAINS];
  tdsl1_ciphering_key_cmd_type l1_cipher_info;
  boolean                   mac_ciph_config_affected;
}tdsrrcsmc_ciphering_config_info_type;

/*type to hold rb_id and sequence number*/
typedef struct
{
  uint8 rb_id;        /*rb id*/
  uint16 seq_num;      /*seq num to activate ciphering*/
} tdsrrcsmc_rb_id_type;


/*type to save contents of security mode command message.
  Security Mode Command may not be acted upon by immediately if
  Ordered_Config is set or if Cell_Update is in progress*/
typedef struct 
{
  uint32 transaction_id;                     /*transaction id of message*/
  rrc_cn_domain_identity_e_type domain_id;   /*domain id                */


  /*ciphering related information*/
  uecomdef_umts_cipher_algo_e_type ciph_algo;          /*ciphering algo being used*/
  boolean dpch_ciph_act_time_present;        /*flag to indicate if dpch 
                                               act time present         */
  boolean rb_ciph_dl_act_time_present;       /*flag to indicate if dl rb
                                               act time present         */
  uint32 dpch_ciph_act_time;                 /*dl dpch act time         */
  uint8 num_rb_ciph_dl_act_time;             /*num dl rb activation time*/
  tdsrrcsmc_rb_id_type rb_ciph_dl_act_time[TDSUE_MAX_DL_UM_CHANNEL + 
                                        TDSUE_MAX_AM_ENTITY];
  /*array to hold dl rb act 
    time                     */

  /*integrity related information*/
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo; 
  /*integrity algo being used*/
  boolean start_chosen;                      /*start chosen             */
  boolean modify_chosen;                     /*modify chosen            */
  uint32 start;                              /*start value if start     */
  tdsrrcsmc_rb_id_type srb_int_dl_act_time[TDSRRC_MAX_SRB];
  /*activation time for dl 
integrity                */
} tdsrrcsmc_security_mode_command_type;

typedef struct 
{
  boolean                  ciph_algo_present;
  uecomdef_umts_cipher_algo_e_type  ciph_algo;
  boolean                  dpch_act_time_present;  
  uint32                   dpch_activation_time;
  boolean                  fresh_present;
  uint32                   fresh;
  uint32                   fresh_old;
  uint32                   fresh_old1;/* stores the old FRESH value, needed for 
                                         reverting back to old configuration after
                                         PhyChEstFailure(SRNS Relocation) */
  boolean                  reconfig_cnf_req;
  tdsrrc_rlc_re_est_for_srns_rb_e_type rb_type;  
  tdsrrc_proc_e_type          proc_type;
  boolean                  integrity_algo_present;
  uecomdef_umts_integrity_protection_algorithm_e_type  integrity_algo;
} tdsrrcsmc_srns_info;

/*struct contains info about SRNS relocation integrity and ciphering info
  passed from reconfig-procedures to SMC*/
typedef struct
{
  boolean                          integrity_info_present;
  boolean                          ciphering_mode_info_present;
  tdsrrc_IntegrityCheckInfo           *integrity_check_info;
  tdsrrc_IntegrityProtectionModeInfo  *integrity_protection_mode_info;
  tdsrrc_CipheringModeInfo            *ciphering_mode_info;
} tdsrrcsmc_srns_relocation_security_info_type;


/*Temporary structure to hold the max tdscount_c values for released rbs */
typedef struct
{
  uint32 count_c_cs;
  uint32 count_c_ps;
}tdsrrcsmc_rel_rbs_max_count_c;

/*struct contains info about SRNS relocation integrity and ciphering info
  passed from reconfig-procedures to SMC*/
typedef struct
{
  boolean                          integrity_info_present;
  boolean                          ciphering_mode_info_present;
  tdsrrc_IntegrityCheckInfo           *integrity_check_info;
  tdsrrc_IntegrityProtectionModeInfo_r7 *integrity_protection_mode_info;
  tdsrrc_CipheringModeInfo_r7 *ciphering_mode_info;
} tdsrrcsmc_srns_relocation_security_info_r7_type;

/*struct contains info if rlc rb's are suspended by SMC procedure, also
  contains info about which rb's are suspended*/
typedef struct
{
  boolean rlc_rb_suspended_or_stopped_flag;               /* whether suspended/stopped or not */
  boolean rlc_srb2_suspended_or_stopped_flag;
  tdsrlc_ul_stop_suspend_request_type  suspend_or_stopped_info;      /* which rb's suspended */
  tdsrlc_dl_stop_request_type dl_stop_info;
} tdsrrcsmc_suspend_stop_rlc_type;

/*type to hold rb_id and sequence number*/
typedef struct
{
  uint8                          rb_id;
  tdsrrcsmc_affected_rb_e_type      rb_status;
  boolean                        is_rb_srb;
  rrc_cn_domain_identity_e_type  domain_id;
} tdsrrcsmc_um_am_config_info;

/*type to hold UL AM ciphering config info*/
typedef struct
{
  uint8                      num_rb;
  tdsrrcsmc_um_am_config_info   config_info[TDSUE_MAX_AM_ENTITY];
}tdsrrcsmc_ul_am_ciph_config;

/*type to hold UL UM ciphering config info*/
typedef struct
{
  uint8                      num_rb;
  tdsrrcsmc_um_am_config_info   config_info[TDSUE_MAX_UL_UM_CHANNEL];
}tdsrrcsmc_ul_um_ciph_config;

/*type to hold TM ciphering config info*/
typedef struct
{
  tdsrrcsmc_affected_rb_e_type      rb_status;
  boolean                        affected;
  uint8                          num_rb;
}tdsrrcsmc_tm_ciph_config;

/*type to hold GLBOBAL CIPHERING Database used by MAC/RLC/L1*/
typedef struct
{
  tdsrrcsmc_ul_um_ciph_config    um_rb_ciph_config;
  tdsrrcsmc_ul_am_ciph_config    am_rb_ciph_config;
  tdsrrcsmc_tm_ciph_config       tm_rb_ciph_config[RRC_MAX_CN_DOMAINS];
}tdsrrcsmc_rb_ciphering_config;

/*type which holds list of ciphering keys*/
typedef struct
{
  /*0 indicates cs_1 used, 1 indicates cs_2 used*/
  boolean key_used;

  /*indication that a new key is received*/  
  boolean new_key;  

  /* Indication that new key is received from previous uncompleted SMC
     * This new key will be operation after IDT carries START = 0x0,
     * (Instead of sending generated START as per spec 25.331 section 8.5.9 
     */
  boolean new_key_for_start_value; 

  /*cs and ps keys that will be used.  Index given to MAC and RLC will
    be between 0-3*/
  byte  key[TDSMAX_CIPH_KEYS_PER_DOMAIN][RRC_MAX_CIPHERING_KEY_LENGTH];
}tdsrrcsmc_ciph_keys_type;

typedef struct
{
  /* CN Domain Identity */
  rrc_cn_domain_identity_e_type       domain_id;

  tdsrrcsmc_ciph_keys_type               ciph_keys_info;

  byte  integrity_key[RRC_MAX_INTEGRITY_KEY_LENGTH];
  byte  integrity_key_old[RRC_MAX_INTEGRITY_KEY_LENGTH];

  rrc_cipher_algo_e_type  cipher_algo; 
  /* Hyper Frame Number */
  byte                  hfn[RRC_MAX_HFN_LENGTH];
  byte                  hfn_latest[RRC_MAX_HFN_LENGTH];
}tdsrrcsmc_security_config_type;

  /*This structure is used to store the security key set generated 
    for CS domain in other RAT(i.e-G-RAT).*/
typedef struct
{
  /*This field is used to indicate whether the security key set 
    in this structure are valid*/
  boolean grat_key_set_valid;
  /*used to hold Integrity key*/
  byte  integrity_key[RRC_MAX_INTEGRITY_KEY_LENGTH];
  /*used to hold Ciphering key*/
  byte  ciphering_key[RRC_MAX_CIPHERING_KEY_LENGTH];
}tdsrrcsmc_security_keyset_generated_in_grat_type;

/*local SMC representation of cn_ciphering_info_type*/
typedef struct
{
  uint32                         num_cn_domains;

  /* latest and little less latest CN Domain Id */
  rrc_cn_domain_identity_e_type  latest_config_cn_id;     
  rrc_cn_domain_identity_e_type  latest_config_cn_id_old;     

  /* latest_config_cn_id_old_1 will hold the value of latest_config_cn_id_old whenever
      security mode command with "modify" is received */
  /*rrc_cn_domain_identity_e_type  latest_config_cn_id_old_1;     */

  /*make the old_1 for per SRB based. */
  rrc_cn_domain_identity_e_type  latest_config_cn_id_old_1[TDSRRC_MAX_SRB];


  boolean ciphering_info_received;
  boolean integrity_info_received;

  boolean start_value_update_flag[RRC_MAX_CN_DOMAINS];

  /*whether ciphering is started or not per cn domain*/
  tdsrrcsmc_security_status_e_type  ciph_status[RRC_MAX_CN_DOMAINS];   

  /*whether sync_ind is sent to NAS or not per cn domain*/
  boolean  is_sync_ind_sent_to_nas[RRC_MAX_CN_DOMAINS];   

  tdsrrcsmc_security_config_type    int_ciph_info[RRC_MAX_CN_DOMAINS];

  /*save latest ciphering algorithm sent for both domains*/
  uecomdef_umts_cipher_algo_e_type saved_ciph_algo[RRC_MAX_CN_DOMAINS];

  /*save latest Integrity algorithm sent for both domains*/
  uecomdef_umts_integrity_protection_algorithm_e_type  saved_integrity_algo[RRC_MAX_CN_DOMAINS];

  boolean                        int_start_status_for_conn[RRC_MAX_CN_DOMAINS];

  /*max count c for released RBs per domain*/
  uint32 max_count_c_rel_rbs[RRC_MAX_CN_DOMAINS];

}tdsrrcsmc_security_config_info_type;

/*struct to hold activation times for UL and DL ciphering*/
typedef struct
{
  uint8 nchan;
  uint16 sq_num[TDS_MAX_RB];
  uint8 act_time_present[TDS_MAX_RB];
  uint16 sq_num_for_complete[TDS_MAX_RB];
  uint8 lc_id_for_complete[TDS_MAX_RB];
} tdsrrcsmc_ciph_act_type;

/*All information with regards to the current Ciphering status
  is saved in this struct*/
typedef struct
{
  boolean                            reconfig; /*reconfig true or false*/

  /*local value to store suspend_cnf information*/
  tdsrrcsmc_ciph_act_type ul_ciph_act_info;

  /*local value to store dl act time info*/
  tdsrrcsmc_ciph_act_type dl_ciph_act_info;

} tdsrrcsmc_ciph_info_type;

/*struct to hold Uplink and Downlink count_i information. will add cound_c
  struct later when we support ciphering*/
typedef struct
{
  uint32  dl_count_i[TDSRRC_MAX_SRB];
  boolean dl_count_i_init_flag[TDSRRC_MAX_SRB];
  boolean dl_count_i_act_time_pending[TDSRRC_MAX_SRB];
  boolean dl_new_key_flag_per_rb[TDSRRC_MAX_SRB];

  uint32  ul_count_i[TDSRRC_MAX_SRB];
  boolean ul_count_i_act_time_pending[TDSRRC_MAX_SRB];
  boolean ul_new_key_flag_per_rb[TDSRRC_MAX_SRB];

} tdsrrcsmc_count_i_info_type;


/*All information with regards to the current Integrity Protection status
  is saved in this struct*/
typedef struct
{
  tdsrrcsmc_security_status_e_type            ip_status;/*started or not*/

  boolean                                  reconfig; /*reconfig true/false*/

  boolean                                  modify_received;   /*true or false*/

  /*ul or dl act time if modify received*/
  uint32                                   dl_act_seq_num[TDSRRC_MAX_SRB];
  uint32                                   ul_act_seq_num[TDSRRC_MAX_SRB];

  /* to indicate whether latest_config_cn_id_old_1 is valid for a given SRB or not */
  boolean                                dl_latest_config_cn_id_old_1_valid[TDSRRC_MAX_SRB];

  /* keys below will hold the integrity key for a SRB when cn_id_old_1 is valid  for that SRB */
  byte  integrity_key_for_cn_id_old_1[TDSRRC_MAX_SRB][RRC_MAX_INTEGRITY_KEY_LENGTH];

  tdsrrcsmc_count_i_info_type                 count_i_info[RRC_MAX_CN_DOMAINS];
  /*holds ul and dl count_i*/
} tdsrrcsmc_integrity_protection_info_type;

/*Local representation of Integrity Check Info IE.*/
typedef struct
{
  uint32 msg_auth_code;
  uint32 msg_seq_num;
} tdsrrcsmc_integrity_check_info_type;

/*start_list for hfn's*/
typedef struct
{
  uint32 hfn_srb;
  uint32 hfn_rab;
  uecomdef_umts_cipher_algo_e_type ciph_algo_srb;
  uecomdef_umts_cipher_algo_e_type ciph_algo_urb;
}tdsrrcsmc_re_est_start_list;

typedef struct
{
  /*static var to hold all kinds of security config which is not calculated
    by SMC procedure,which is essentially received from other procedures or
    OTA message */
  tdsrrcsmc_security_config_info_type  rrcsmc_int_ciph_info;
  
  /*declare the struct integrity_protection_info*/
  tdsrrcsmc_integrity_protection_info_type integrity_protection_info;
  
  /*declare the struct integrity_protection_info*/
  tdsrrcsmc_ciph_info_type ciphering_info;

  tdsrrcsmc_ciphering_config_info_type tdscipher_config_db;

  tdsrrcsmc_rb_ciphering_config ciph_rb_info;

  /*boolean to check is START_VALUE_TO_TRANSMIT is set or not*/
  boolean start_value_to_transmit_flag;

  /*new tm radio bearer indication flag*/
  boolean new_tm_rb_ind_flag;

} tdsrrcsmc_security_variables_backup;
/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcsmc_is_integrity_prot_mode_info_present

DESCRIPTION
  This function returns TRUE if IE "integrityProtectionModeInfo" is present in
  configuration message, otherwise it returns FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsmc_is_integrity_prot_mode_info_present(uint8 *dl_sdu_ptr, uint8 msg_type);

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
);
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
);

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
);

/*===========================================================================

FUNCTION tdsrrcsmc_send_resume_req

DESCRIPTION
   This function sends resume req to LLC which resumes transmission of  all 
   RLC UM and AM radio bearers(except for RB2)

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcsmc_fill_ul_resume_req
(
  tdsrlc_ul_resume_type *rrc_ul_resume_req_ptr
);

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
);

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
);

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
);
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
);

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
rrc_cn_domain_identity_e_type tdsrrcsmc_get_latest_configured_cn_domain(void);

/*===========================================================================

FUNCTION tdsrrcsmc_init_procedure

DESCRIPTION
  This function initializes required parameters for Security Mode Command
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcsmc_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrcsmc_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for Security Mode Command procedure
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing SECURITY
      Mode Command message.
    RRC_CIPHER_REQ Primitive with new C-key and I-key
    TDSRRC_STATE_CHANGE_IND Primitive informs transition from Connected mode to
      Idle Disconnected state.
    TDSRRC_DOWNLINK_L2ACK_IND Primitive indicates success or failure of 
      transmission of Security Mode Complete Msg.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void tdsrrcsmc_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcsmc_init_integrity_ciphering_config

DESCRIPTION
  This function initializes the rrcsmc_integrity_and_ciphering_info with 
  the values received from MM in the TDSRRC_EST_REQ. Also a copy is made in 
  the rrcsmc_integrity_and_ciphering_info_old.
  RCE procedure will call this function in SMC.


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
);
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
);
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
);
/*===========================================================================

FUNCTION tdsrrcsmc_append_uplink_integrity_check_info

DESCRIPTION
  RRCSMC receives the encoded uplink SDU and adds Integrity Check Info to
  the Uplink SDU.
  The send chain will call this function in SMC before sending a 
  message on a UL Signaling Radio Bearer to append the Integrity Check
  Info in the message.

DEPENDENCIES
  None.

RETURN VALUE
  This function returns SUCCESS if IntegrityCheckInfo is appended successfully
  This function returns FAILURE if if it is failed to append IntegrityCheckInfo.


SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_uplink_integrity_check_info 
(
  byte*  ul_sdu,
  uint32 ul_sdu_length,
  tdsrrc_RB_Identity rb_id
);

/*===========================================================================

FUNCTION tdsrrcsmc_append_start_list

DESCRIPTION
  This function initializes the START_LIST with the latest values to 
  be sent to the UTRAN by the RCE, CU  and ASU procedures.
  
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
);

/*===========================================================================

FUNCTION tdsrrcsmc_process_integrity_protection_mode_info

DESCRIPTION
  This procedure process Integrity Protection Mode Info.  It is called by 
  the foll. procedures.  
  
  UMI, Cell Update, URA Update, RBE, ASU
  
  RRCSMC will copy the IntegrityProtectionModeInfo IE.
  If IntegrityProtectionModeInfo indicates to start Integrity
  Protection, it will update the necessary paramters and start Integrity.
  Otherwise it returns invalid configuration.
    
  The above procedures will call this function in SMC to notify the SMC
  procedure of the Integrity Protection Info and to start Integrity Protection.


DEPENDENCIES
  None.

RETURN VALUE
  Returns TDSRRCSMC_VALID_CONFIG if IntegrityProtectionModeInfo is 
  valid(Start only).
  Returns RRCSMC_INVALID_CONFIG if IntegrityProtectionModeInfo is invalid(Modify)

SIDE EFFECTS
  None

===========================================================================*/
tdsrrcsmc_config_status_e_type tdsrrcsmc_process_integrity_protection_mode_info
(
  struct tdsrrc_IntegrityProtectionModeInfo* integrityProtModeInfo
);

/*===========================================================================

FUNCTION tdsrrcsmc_append_ul_integrity_prot_act_info

DESCRIPTION
  This procedure populates the UL Activation Info times into the ptr passed
  to it by the foll. procedures.
  
  UMI, Cell Update, URA Update, RBE, ASU
  
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
uecomdef_status_e_type tdsrrcsmc_append_ul_integrity_prot_act_info 
(
  struct tdsrrc_IntegrityProtActivationInfo * ulActivationInfo
);

/*===========================================================================

FUNCTION tdsrrcsmc_append_hfns_to_rel_ind

DESCRIPTION
  This procedure appends the CS and PS domain HFNs to RRC_REL_IND message
  ptr which will be sent to MM.

DEPENDENCIES
  None.

RETURN VALUE
  Returns SUCCESS if PS and CS HFNs are successfully appended to rrc_rel_ind_type.
  Returns FAILURE if PS and CS HFNs are not appended to rrc_rel_ind_type.


SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_append_hfns_to_rel_ind 
(
  byte *hfncs, 
  byte *hfnps
);
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
tdsrrcsmc_ip_status_e_type tdsrrcsmc_check_integrity_status(void);

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
void tdsrrcsmc_reset_smc_ip_started_flag(void);

/*===========================================================================

FUNCTION rrcsmc_reset_smc_configure_flag

DESCRIPTION
  This function is called by UMI, Cell Update, URA Update, RBE, ASU 
  procedures when they receive an L2Ack for the Complete message they
  sent out on the uplink.  This is to indicate to the SMC that
  changing the configuration of Security is complete and it should
  reset its internal flag

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcsmc_reset_smc_config_flag(void);
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_config_info

DESCRIPTION
  This function is called by LLC procedure before it sends ciphering
  information to RLC, MAC and L1 layers.  When this function is called,
  SMC procedure updates the global DB for ciphering configuration with
  the latest ciphering information. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_update_ciphering_config_info
(
  tdsrrc_ciph_config_e_type config_cause
);
/*===========================================================================

FUNCTION tdsrrcsmc_update_ciphering_config_info

DESCRIPTION
  This function is called by LLC procedure before it sends ciphering
  information to RLC, MAC and L1 layers.  When this function is called,
  SMC procedure updates the global DB for ciphering configuration with
  the latest ciphering information. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_update_ciphering_config_info_for_reconfig
(
  tdsrrcllc_rlc_config_req_list_type *rlc_req_type_ptr,
  tdsrrc_proc_e_type proc_id
);
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
void tdsrrcsmc_get_nv_items(void);
/*===========================================================================
FUNCTION tdsrrcsmc_is_ciphering_enabled

DESCRIPTION
  This function indicates if ciphering is started or not.
  
DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS or FAILURE whether ciphering is started or not respectively.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcsmc_security_status_e_type tdsrrcsmc_is_ciphering_enabled(void);
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
void tdsrrcsmc_update_ciphering_activation_cfn( uint8 tdsciphering_activation_cfn );
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
);

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
);

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
);

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
);


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
);

/*===========================================================================

FUNCTION rrcsmc_get_and_reset_new_tm_rb_ind

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
boolean tdsrrcsmc_is_new_tm_rb_setup(void);
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
);

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
);

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
);
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
);
/*===========================================================================

FUNCTION tdsrrcsmc_get_smc_procedure_complete_status

DESCRIPTION
  This status is queried by UDT procedure.  If procedure is waiting for L2_ACK
  for SMC complete or for lower layer ciphering_configuration, then return
  FALSE.  UDT procedure will wait for indication from SMC when L2_ACK is rcvd.
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
);
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
);
/*===========================================================================

FUNCTION tdsrrcsmc_check_integrity_for_srns_relocation

DESCRIPTION
  This function checks integrity for an SRNS triggering relocation
  message.
 
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcsmc_srns_ip_check_status_e_type

SIDE EFFECTS
  None

===========================================================================*/
tdsrrcsmc_srns_ip_check_status_e_type tdsrrcsmc_check_integrity_for_srns_relocation
(
  tdsrrc_cmd_type *cmd_ptr
  ,tdsrrc_msg_ver_enum_type msg_version
);
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
);

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
);


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
);

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
);
/*===========================================================================

FUNCTION tdsrrcsmc_process_step_1_ho_ciph_req

DESCRIPTION
  Process Step 1 ciphering for Handover

DEPENDENCIES
  None.

RETURN VALUE
  uecomdef_status_e_type SUCCESS if Ciphering is started on TM RB's
                         FAILURE if Ciphering is not started on TM RB's  
===========================================================================*/
uecomdef_status_e_type tdsrrcsmc_process_cipher_config_for_hho( uint32 mac_d_hfn);
/*===========================================================================

FUNCTION tdsrrcsmc_step_2_cipher_config_for_hho

DESCRIPTION
  Process Ciph Config Cnf for Hard Handover

DEPENDENCIES
  None.

RETURN VALUE
  Returns Count_C Activation Time

===========================================================================*/
uint32 tdsrrcsmc_step_2_cipher_config_for_hho ( void );

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
);

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
);

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
);
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
boolean tdsrrcsmc_tm_ciph_active
(
 void
);
/*===========================================================================

FUNCTION tdsrrcsmc_append_start_list_per_active_domain

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

uecomdef_status_e_type tdsrrcsmc_append_start_list_per_active_domain 
(
tdsrrc_STARTList *start_list_ext,
tdsrrcsmc_append_start_e_type start_type
);
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
tdsrrc_STARTList *start_list_ext,
tdsrrcsmc_append_start_e_type start_type
,boolean both_cn_domain_open
,rrc_cn_domain_identity_e_type active_cn_domain
);

/*===========================================================================

FUNCTION RRCSMC_EXTRACT_RETURN_HFN_PER_DOMAIN_AND_UPDATE_START_FLAG

DESCRIPTION
  This procedure does start value calculation for a given domain and 
  sets the start value update flag.
  
  DEPENDENCIES
  None.

RETURN VALUE
  Updated Start value. Calling func should apply offset to this value to use it for AM/UM/TM

SIDE EFFECTS
  None

===========================================================================*/
uint32 tdsrrcsmc_extract_return_hfn_per_domain_and_update_start_flag
(
  rrc_cn_domain_identity_e_type domain_id
);

/*===========================================================================

FUNCTION RRCSMC_UPDATE_HFN_FOR_RLC_RB

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
);

/*===========================================================================

FUNCTION tdsrrcsmc_process_suspend_cnf

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
/*lint -e715*/
void tdsrrcsmc_process_suspend_cnf
(
tdsrlc_ul_stop_suspend_cnf_type *rrcsmc_suspend_cnf
);
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
);

/*===========================================================================

FUNCTION tdsrrcsmc_is_rlc_re_est_for_srns_active

DESCRIPTION
  This function returns the status of SMC doing RLC re-establishment.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcsmc_is_rlc_re_est_for_srns_active
(
  void
);

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
);

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
rrc_cn_domain_identity_e_type tdsrrcsmc_get_start_value_to_transmit_domain(void);
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
boolean tdsrrcsmc_get_security_start_status
(
void
);
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
);
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
);
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
);
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
);


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
);

/*===========================================================================

FUNCTION tdsrrcsmc_calc_max_count_c_released_rb

DESCRIPTION
  This function calculates and stores the maximum count_c values for all 
  released rbs for both domains in a temporary variable.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

void tdsrrcsmc_calc_max_count_c_released_rb
(
  tdsrrcllc_rlc_config_req_list_type *rlc_req_type_ptr,
  rrc_cn_domain_identity_e_type  local_domain_id_list[TDS_MAX_RB],
  tdsrrc_proc_e_type proc_id
);

/*===========================================================================

FUNCTION tdsrrcsmc_update_count_c_released_rbs

DESCRIPTION
  This function updates the maximum count c stored in the temporary variables 
  to the permanent one.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

void tdsrrcsmc_update_count_c_released_rbs
(
  void
);

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
);
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
);

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
);

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
);


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
);
#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================
FUNCTION tdsrrcsmc_get_srns_info_from_dl_sdu_ind_r9

DESCRIPTION
  This function extracts ip check info/integrity protection mode info/ciphering mode info from SRNS 
  relocation triggering messages for Release 9 version of OTAs. In summary, input srns_info has to be filled
  from OTA in cmd_ptr.

  NOTE:  There is no R9 UTRANMobilityInformation and URAUpdateConfirm messages in ASN 9.6.
  Right now, only 5 Reconfig OTAs + CU CNF can do SRNS relocation for R9 OTA.

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
);
#endif /*FEATURE_TDSCDMA_REL9*/

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
);

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
);
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
/*===========================================================================

FUNCTION rrcsmc_save_security_key_set_from_lte

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
);

#endif /* FEATURE_LTE_TO_TDSCDMA */



/*For stopping the RLC during SRNS relocation HO phase*/
/*===========================================================================

FUNCTION tdsrrcsmc_perform_stop_or_suspend_rlc

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
);



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
boolean tdsrrcsmc_get_smc_srns_pending_status_for_dt(void);


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
void tdsrrcsmc_send_rlc_re_est_cmp (void);

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
tdsrrcsmc_substate_e_type tdsrrcsmc_get_smc_substate(void);

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
);

/*===========================================================================

FUNCTION tdsrrcsmc_set_smc_security_info_to_related_sub_params

DESCRIPTION
The function set the SMC security info to relatd sub params according to sub_id.

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
);

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
);

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
);
#endif
#endif /* TDSRRCSMC_H */
