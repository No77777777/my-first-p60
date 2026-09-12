#ifndef TDSRRCRBCOMMON_H
#define TDSRRCRBCOMMON_H
/*===========================================================================
         R R C   R A D I O   B E A R E R   C O M M O N   M O D U L E 

DESCRIPTION

  This module is the header module for the Radio Bearer Common module.
  This contains the external interface functions to be used by
  by other RRC Radio Bearers modules. This must not be used by modules 
  outside RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrbcommon.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/17/12   zy      Change DMO interface: measurement_purpose and timeslot_bitmap, from uint8 to macro.
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/18/11   rm      Added prototype of rrcrb_proc_waiting_for_l2_ack
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/03/10   rl      Fixed compiler warnings
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
11/11/10   yzh     (3GPP: 10.3.6.39a)Add tdsrrcget_add_ext_info() for r4\r5-add-ext.
11/05/10   ysh      C-RNTI and H-RNTI remove condition.
                                New function for get OTA msg's version. 3GPP-8.2.2.3(3.2.3).
11/10/10   bj       Deleted IE "RNC support for change of UE capability".
                          (3GPP: 8.6.2.3)
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRRC_SDU_LIFE_TIME_GUARD_TIME 6000

/* Enumerated type of status of validating an RB message                   */
typedef enum
{
  TDSRRCRB_MSG_VALID,              /* Message is valid                        */
  TDSRRCRB_MSG_UNSUPPORTED_CONFIG, /* Message has un-supported configuration  */
  TDSRRCRB_PHY_CHAN_FAILURE,       /* Physical channel setup failed           */
  TDSRRCRB_SIMULTANEOUS_RECONFIG,  /* Incompatible simultaneous reconfig      */
  TDSRRCRB_MSG_PROTOCOL_ERROR,     /* Message has protocol error              */
  TDSRRCRB_MSG_INVALID_CONFIG,     /* Message has invalid configuration       */
  TDSRRCRB_MSG_INCOMPLETE_CONFIG,  /* Message has incomplete configuration    */
  TDSRRCRB_CELL_UPDATE_OCCURED     /* Cell update with oss occured            */
}tdsrrcrb_msg_status_e_type;

/* Enumerated type of the different substates for RB procedures           */
typedef enum
{
  TDSRRCRB_INITIAL,                   /* Initial Substate                    */
  TDSRRCRB_WAIT_FOR_CELL_UPDATE_CNF,  /* Wait for cell update                */
  TDSRRCRB_WAIT_FOR_CELL_SELECTION_CNF,  /* wait for cell selection      */
  TDSRRCRB_WAIT_FOR_CHAN_CFG_CNF,     /* Waiting for confirmation of channel  
                                    config from LLC                        */                      
  TDSRRCRB_WAIT_FOR_CRNTI_UPDATE_CNF,     /* Waiting for CRNTI update cnf   */                      
  TDSRRCRB_WAIT_FOR_L2_ACK,           /* Waiting for Layer 2 ack for uplink
                                    message transmission                   */
  TDSRRCRB_WAIT_FOR_REESTABLISH_SRB2, /*waiting for re-establishment of srb2 */
  TDSRRCRB_MAX_SUBSTATES
}tdsrrcrb_substate_e_type;

/* structure to be used for RNTI update request by rb procedures. */
typedef struct
{
  tdsrrc_proc_e_type            procedure;            /* procedure requesting rnti update       */
  tdsrrc_rnti_cmd_status_e_type crnti_action;    /* indicates C-RNTI needs tobe updated    */
  tdsrrc_rnti_cmd_status_e_type urnti_action;    /* indicates U-RNTI nees tobe updated     */
  uint16                     crnti;                /* C-RNTI value                           */
  tdsrrc_int_u_rnti_type        urnti;                /* U-RNTI value                           */
  boolean                    rlc_re_establish_srb; /* indicates rlc reestablishment for srbs */
  boolean                    rlc_re_establish_rab; /* indicates rlc reestablishment for rab  */
  boolean                    cnf_reqd;             /* indicates confirmation reqd for update */
} tdsrrcrb_rnti_update_info_type;

/* structure to be used for directed cell info (freq and scr) */
typedef struct
{
  boolean                    freq_info_present; /* indicates whether freq info present */
  uint32                     uarfcn_dl;
  /* Indicates if Destination cell_param_id is present in OTA msg */
  boolean                    cpid_present;
  uint8                      cell_param_id;
} tdsrrcrb_directed_cell_info_type;

/* Define an invalid transaction id - This is used to initialize the local
transaction ids. This should not be a valid transaction id and should be
replaced by the first received transaction id for a message                */
#define TDSRRCRB_INVALID_TRANSACTION_ID  10


/* Maximum RABs that can be contained in a Radio Bearer message. Although
this number can be larger, it's defined as 2 now. We support only one RAB
per message currently. */
#define TDSRRCRB_MAX_RAB_PER_MSG 8

/* This define time in ms for which the rb procedure waits to send cell update
 * after sending the complete message, for scenarios where there has been 
   an RLC unrecoverable error on non srb2 during a pending configuration*/
#define TDSRRCRB_PND_CFG_DELAY 120


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Structure to store transaction Ids. for any Radio Bearer Procedure      */
typedef struct
{
  uint8 accepted_transaction;   /* The latest accepted transaction id     */
  uint8 rejected_transaction;   /* The latest rejected transaction id     */
}tdsrrcrb_transaction_id_type;  

/* Structure to store failure cause and protocol error cause for a msg     */
typedef struct
{
  tdsrrcrb_msg_status_e_type failure_status; /* Failure cause                 */
  tdsrrc_ProtocolErrorCause  prot_err_cause; /* Protocol Error cause          */
}tdsrrcrb_msg_failure_cause_type;

/* For each Radio Bearer message, the following structure can hold
the information related to the RABs contained in that message till they
are processed.                                                             */
typedef struct
{
  uint32 num_rabs_in_msg;      /* Number of RABs contained in the message  */
  boolean chan_config_needed[TDSRRCRB_MAX_RAB_PER_MSG]; /* TRUE if channel
                                                     config is needed for
                                                     this RAB              */
  tdsrrc_est_rab_info_type rabs_in_msg[TDSRRCRB_MAX_RAB_PER_MSG]; /* Info for each
                                                            RAB            */
}tdsrrcrb_rab_info_per_msg_type;


#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
typedef struct
{
  boolean sdu_valid;
  boolean act_time_valid;
  boolean sdu_discard;
}tdsrrcrb_sdu_lifetime_act_time_valid_type;
#endif

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/*define sdu delay and recv time here to avoid interface change*/
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION

extern uint32 tdsrrc_sdu_recv_delay;   /*the delay for receiving the whole SDU*/  
extern uint32 tdsrrc_sdu_recv_time;    /*the time when received the first PDU */               
extern uint32 tdsrrc_cell_update_cnf_time; 	 /*the time of for cell update confirm*/
#endif


/* =======================================================================
**                        MACRO Declarations
** ======================================================================= */

/*===================================================================
MACRO       :   RRCRB_VALIDATE_SRNS_RELCOATION_R6


DESCRIPTION :   This MACRO checks whether SRNS relocation info is present                                                  
                                                                        
DEPENDENCIES:   None

RETURN VALUE:   None.

SIDE EFFECTS:   None.

=====================================================================*/
#define RRCRB_VALIDATE_SRNS_RELCOATION_R6(proc_id, msg_type_prefix1, \
                                          msg_type_prefix2, srns_reloc) \
{ \
 \
  if (proc_id == RRC_PROCEDURE_RBE) \
  { \
    if ((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u. \
         criticalExtensions.u.r6.radioBearerSetup_r6.specificationMode.choice & \
         rrc_RadioBearerSetup_r6_IEs_specificationMode_complete_chosen) &&  \
        (msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u. \
         criticalExtensions.u.r6.radioBearerSetup_r6.specificationMode.complete.bitmask & \
         rrc_complete_dl_CounterSynchronisationInfo_present)) \
      { \
        srns_reloc = TRUE; \
    } \
  } \
  else if  (proc_id == RRC_PROCEDURE_RBRC)\
  { \
    if ((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions. \
         u.msg_type_prefix1##_r6.bitmask & rrc_##msg_type_prefix2##_r6_IEs_new_U_RNTI_present)) \
      { \
        srns_reloc = TRUE; \
    } \
  }\
  else \
  { \
    if ((msg_ptr->u.later_than_r3.criticalExtensions.u.criticalExtensions.u. \
         criticalExtensions.u.rrc_##msg_type_prefix1##_r6.bitmask & \
         rrc_##msg_type_prefix2##_r6_IEs_dl_CounterSynchronisationInfo_present)) \
      { \
        srns_reloc = TRUE; \
    } \
  } \
}



#define TDSRRCRB_R6_CRITICAL_EXT_HO_TO_UTRAN_PRESENT( \
  msg, msg_choice_prefix, r6_msg_choice_prefix) \
  ((T_##msg_choice_prefix##_criticalExtensions_5_criticalExtensions & \
          msg->u.criticalExtensions.t) && \
          (T_##msg_choice_prefix##_criticalExtensions_4_criticalExtensions == \
            msg->u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r6_msg_choice_prefix##_criticalExtensions_4_r6 == \
             msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

#define TDSRRCRB_R6_CRITICAL_EXT_PRESENT( \
  msg, msg_choice_prefix) \
  ((T_tds##msg_choice_prefix##criticalExtensions_5_criticalExtensions == \
          msg.u.later_than_r3.criticalExtensions.t) && \
          (T_tds##msg_choice_prefix##criticalExtensions_4_criticalExtensions == \
            msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
            (T_tds##msg_choice_prefix##criticalExtensions_4_r6 == \
             msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

#define TDSRRCRB_R6_UL_ADD_RECFG_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.ul_AddReconfTransChInfoListPresent))

#define TDSRRCRB_R6_UL_DELETED_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.ul_deletedTransChInfoListPresent))

#define TDSRRCRB_R6_UL_COMMON_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.ul_CommonTransChInfoPresent))

#define TDSRRCRB_R6_DL_ADD_RECFG_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.dl_AddReconfTransChInfoListPresent))

#define TDSRRCRB_R6_DL_DELETED_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.dl_DeletedTransChInfoListPresent))

#define TDSRRCRB_R6_DL_COMMON_TRCH_INFO_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.dl_CommonTransChInfoPresent))

#define TDSRRCRB_R6_DL_COUNTER_SYNC_INFO_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.dl_CounterSynchronisationInfoPresent))

#define TDSRRCRB_R6_SRB_INFO_SETUP_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.srb_InformationSetupListPresent))

#define TDSRRCRB_R6_RAB_INFO_SETUP_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.rab_InformationSetupListPresent))


#define TDSRRCRB_R6_RB_INFO_RECONFIG_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.rb_InformationReconfigListPresent))

#define TDSRRCRB_R6_RB_INFO_AFFECTED_LIST_PRESENT(msg_ptr, msg_type_prefix) \
        ((msg_ptr->specificationMode.u.complete.m.rb_InformationAffectedListPresent))

#define TDSRRCRB_R7_CRITICAL_EXT_HO_TO_UTRAN_PRESENT( \
  msg, msg_choice_prefix,r7_msg_choice_prefix) \
  ((T_##msg_choice_prefix##_criticalExtensions_5_criticalExtensions == \
          msg->u.criticalExtensions.t) && \
          (T_##msg_choice_prefix##_criticalExtensions_4_criticalExtensions == \
            msg->u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r7_msg_choice_prefix##_criticalExtensions_3_criticalExtensions == \
             msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
            (T_##r7_msg_choice_prefix##_criticalExtensions_3_r7 == \
             msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))


#define TDSRRCRB_R7_CRITICAL_EXT_PRESENT( \
  msg, msg_choice_prefix) \
    ((T_tds##msg_choice_prefix##criticalExtensions_5_criticalExtensions == \
                  msg.u.later_than_r3.criticalExtensions.t) && \
              (T_tds##msg_choice_prefix##criticalExtensions_4_criticalExtensions == \
                    msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
                (T_tds##msg_choice_prefix##criticalExtensions_3_criticalExtensions == \
                    msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
                (T_tds##msg_choice_prefix##criticalExtensions_3_r7 & \
                 msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

#define TDSRRCRB_R7_CRITICAL_EXT_PRESENT_PTR( \
  msg, msg_choice_prefix) \
  ((T_##msg_choice_prefix##_##criticalExtensions_5_criticalExtensions == \
                  msg->u.later_than_r3.criticalExtensions.t) && \
              (T_##msg_choice_prefix##_##criticalExtensions_4_criticalExtensions == \
                    msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
                (T_##msg_choice_prefix##_##criticalExtensions_3_criticalExtensions == \
                    msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
                (T_##msg_choice_prefix##_##criticalExtensions_3_r7 & \
                 msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
             
#define TDSRRCRBR_R7_MSG_IE_PRESENT(\
    msg,msg_type_prefix,msg_bitmask_prefix)\
    ((msg)->m.msg_bitmask_prefix##Present)

#define TDSRRCRB_UL_DPCH_POWER_CONTROL_INFO_PRESENT(msg_ptr, rel_version) \
        ((msg_ptr->bit_mask) & \
        (rrc_UL_DPCH_Info_r##rel_version##_ul_DPCH_PowerControlInfo_present))

#define TDSRRCRB_UL_DPCH_MODESPECIFIC_INFO(msg_ptr, rel_version) \
        ((msg_ptr->modeSpecificInfo.choice ) & \
        (rrc_UL_DPCH_Info_r##rel_version##_modeSpecificInfo_fdd_chosen))
        


#ifdef FEATURE_TDSCDMA_REL8
			

#define TDSRRCRB_R8_CRITICAL_EXT_PRESENT_PTR( \
  msg, msg_choice_prefix) \
  ((T_##msg_choice_prefix##_##criticalExtensions_5_criticalExtensions == \
				  msg->u.later_than_r3.criticalExtensions.t) && \
			  (T_##msg_choice_prefix##_##criticalExtensions_4_criticalExtensions == \
					msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
				(T_##msg_choice_prefix##_##criticalExtensions_3_criticalExtensions == \
					msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
					(T_##msg_choice_prefix##_##criticalExtensions_2_criticalExtensions == \
					msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
				(T_##msg_choice_prefix##_##criticalExtensions_2_r8 & \
				 msg->u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

#define TDSRRCRB_R8_CRITICAL_EXT_PRESENT( \
  msg, msg_choice_prefix) \
  ((T_tds##msg_choice_prefix##criticalExtensions_5_criticalExtensions == \
				  msg.u.later_than_r3.criticalExtensions.t) && \
			  (T_tds##msg_choice_prefix##criticalExtensions_4_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
				(T_tds##msg_choice_prefix##criticalExtensions_3_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
					(T_tds##msg_choice_prefix##criticalExtensions_2_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
				(T_tds##msg_choice_prefix##criticalExtensions_2_r8 & \
				 msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
				 

#define TDSRRCRB_R8_CU_CRITICAL_EXT_PRESENT_PTR(msg_ptr, cu_msg_prefix)  \
            ((T_tds##cu_msg_prefix##_later_than_r3 == \
            msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && \
            (T_tds##cu_msg_prefix##_criticalExtensions_4_criticalExtensions == \
            msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
            (T_tds##cu_msg_prefix##_criticalExtensions_3_criticalExtensions == \
             msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) \
             && (T_tds##cu_msg_prefix##_criticalExtensions_2_criticalExtensions == \
             msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) \
             && (T_tds##cu_msg_prefix##_criticalExtensions_2_r8 == \
             msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

#define TDSRRCRB_R8_ASU_CRITICAL_EXT_PRESENT_PTR \
  ((T_tdsrrc_ActiveSetUpdate_criticalExtensions_3_criticalExtensions == \
  dcch_ptr->message.u.activeSetUpdate.u.later_than_r3.criticalExtensions.t) \
  && (T_tdsrrc_ActiveSetUpdate_criticalExtensions_2_criticalExtensions == \
  dcch_ptr->message.u.activeSetUpdate.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) \
  && (T_tdsrrc_ActiveSetUpdate_criticalExtensions_2_r8 == \
  dcch_ptr->message.u.activeSetUpdate.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)) 

#define TDSRRCRB_R8_CRITICAL_EXT_HO_TO_UTRAN_PRESENT( \
  msg, msg_choice_prefix, r8_msg_choice_prefix) \
            ((T_##msg_choice_prefix##_criticalExtensions_5_criticalExtensions == \
            msg->u.criticalExtensions.t) && \
            (T_##msg_choice_prefix##_criticalExtensions_4_criticalExtensions ==  \
            msg->u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r8_msg_choice_prefix##_criticalExtensions_3_criticalExtensions ==  \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
            (T_##r8_msg_choice_prefix##_criticalExtensions_2_criticalExtensions == \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r8_msg_choice_prefix##_criticalExtensions_2_r8 == \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

/*The following macro is added for more readability for R8 MSG IEs.*/
#define TDSRRCRB_R8_MSG_IE_PRESENT(\
    msg,msg_type_prefix,msg_bitmask_prefix)\
    (msg->m.msg_bitmask_prefix##Present )

#endif /*FEATURE_TDSCDMA_REL8*/


#ifdef FEATURE_TDSCDMA_REL9
#define TDSRRCRB_R9_CRITICAL_EXT_HO_TO_UTRAN_PRESENT( \
  msg, msg_choice_prefix, r9_msg_choice_prefix) \
            ((T_##msg_choice_prefix##_criticalExtensions_6_criticalExtensions == \
            msg->u.criticalExtensions.t) && \
            (T_##msg_choice_prefix##_criticalExtensions_5_criticalExtensions ==  \
            msg->u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r9_msg_choice_prefix##_criticalExtensions_4_criticalExtensions ==  \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
            (T_##r9_msg_choice_prefix##_criticalExtensions_3_criticalExtensions == \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r9_msg_choice_prefix##_criticalExtensions_2_criticalExtensions == \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) && \
            (T_##r9_msg_choice_prefix##_criticalExtensions_1_r9 == \
            msg->u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))


#define TDSRRCRB_R9_CRITICAL_EXT_PRESENT( \
  msg, msg_choice_prefix) \
  ((T_tds##msg_choice_prefix##criticalExtensions_6_criticalExtensions == \
				  msg.u.later_than_r3.criticalExtensions.t) && \
			  (T_tds##msg_choice_prefix##criticalExtensions_5_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && \
				(T_tds##msg_choice_prefix##criticalExtensions_4_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
					(T_tds##msg_choice_prefix##criticalExtensions_3_criticalExtensions == \
					msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
          (T_tds##msg_choice_prefix##criticalExtensions_2_criticalExtensions == \
          msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)&& \
             (T_tds##msg_choice_prefix##criticalExtensions_1_r9 & \
				  msg.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))

/*The following macro is added for more readability for R9 MSG IEs.*/
#define TDSRRCRB_R9_MSG_IE_PRESENT(\
    msg,msg_type_prefix,msg_bitmask_prefix)\
    (msg->m.msg_bitmask_prefix##Present )

#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
#define TDS_DMO_chk_timeslot_bitmap(ota_timeslot_bitmap) \
( ((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot0) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot1) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot2) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot3) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot4) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot5) | \
((ota_timeslot_bitmap) & TDS_DMO_timeslot_bitmap_timeslot6))


#define TDS_DMO_chk_meas_purpose(ota_meas_purpose) \
( ((ota_meas_purpose) & TDS_DMO_meas_purpose_Inter_frequency_measurement) | \
((ota_meas_purpose) & TDS_DMO_meas_purpose_GSM_carrier_RSSI_measurement) | \
((ota_meas_purpose) & TDS_DMO_meas_purpose_Initial_RSSI_identification) | \
((ota_meas_purpose) & TDS_DMO_meas_purpose_BSIC_reconfirmation) | \
((ota_meas_purpose) & TDS_DMO_meas_purpose_EUTRA_measurement) )

#endif

#endif /*FEATURE_TDSCDMA_REL9*/


/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/* add function for add ext info(fpach frequency and uppch shift) */
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
);


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
);


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
void tdsrrcrb_send_rb_proc_compl_ind(void);


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
);
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
);
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
);


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
  tdsrrc_RB_InformationReleaseList  *rel_list_ptr,
  boolean go_back_to_old_status
);
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
);


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
);

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
);

/*===========================================================================

FUNCTION tdsrrcrb_send_rlc_re_est_req_to_smc

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
);
 /*===========================================================================

FUNCTION          RRCRB_SEND_L1_DRX_REQ

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1.

DEPENDENCIES
                  

RETURN VALUE
                  Void
SIDE EFFECTS      None.
===========================================================================*/
void tdsrrcrb_send_l1_drx_req
(
  tdsrrc_proc_e_type rrc_proc
);


 /*===========================================================================

FUNCTION          tdsrrcrb_send_upd_ota_parms_ind

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_UPD_OTA_PARMS_IND to L1.

DEPENDENCIES
                  
RETURN VALUE
                  
SIDE EFFECTS      None.
===========================================================================*/
void tdsrrcrb_send_upd_ota_parms_ind
(
  tdsrrc_proc_e_type rrc_proc, 
  uint8 bit_mask_ind
);

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
);

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
  tdsrrc_ura_update_cause_e_type cause,
  boolean                      cnf_reqd
);

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
);


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
);

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
  tdsrrc_proc_e_type rrc_proc
);


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
);

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
  uint32 mac_d_hfn_intial_value_present,
  ASN1BitStr32 *mac_d_hfn_intial_value_ptr,
  uint32 *mac_d_val
);

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
);

/*===========================================================================

FUNCTION tdsrrc_fill_rb_id_chan_req_from_est_rabs()

DESCRIPTION
  This function fills existing SRBs and PS RBs in chan_config_req if they are 
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
);

/*===========================================================================

FUNCTION   tdsrrc_fill_rb_id_chan_req_from_standalone_db

DESCRIPTION
  Function to Extract the RB affected list based on Current and Ordered 
    Standalone database

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_fill_rb_id_chan_req_from_standalone_db
(
  uint32 *rb_count,
  tdsrrc_channel_config_req_type *chan_config_req
);

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

);


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
);





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
);
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif/* FEATURE_SCUDIF_ENHANCEMENT*/


/*===========================================================================

FUNCTION tdsrrc_update_reconfig_waiting_for_l2ack_dch

DESCRIPTION
Updates the procedure waiting for L2 Ack for the complete msg for the reconfig  in DCH  state

  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/     
void tdsrrc_update_reconfig_waiting_for_l2ack_dch
(
    tdsrrc_proc_e_type proc_id
);

/*===========================================================================

FUNCTION tdsrrc_check_any_proc_waiting_for_l2_ack

DESCRIPTION
Determines, if any procedure is waiting for the L2 ACk msg for the
reconfig  complete msg sent in DCH

  
DEPENDENCIES


RETURN VALUE
  TRUE: Some prcocedure is waiting for l2 ack
  else FALSE
 

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrc_check_any_proc_waiting_for_l2_ack
(
  void
);

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
);

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
);

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
);
#endif /*FEATURE_UMTS_PDCP*/

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
);
#endif /*FEATURE_TDSCDMA_REL8*/
/*============================================================================
FUNCTION: tdsrrcrb_proc_waiting_for_l2_ack()

DESCRIPTION:
  This function checks is the procedure that set OC is waiting for L2 ACK
  This function will check for RB procedures only

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE is OC set procedure is waiting for L2 ACK
  FALSE otherwise

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcrb_proc_waiting_for_l2_ack
(
  tdsrrc_proc_e_type procedure
);
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
);
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
);


/*Following is moved from individual modules for LLC's visibility*/


/*===========================================================================

FUNCTION   RRCPCRC_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcpcrc_is_srns_in_progress
(
  void 
);


/*===========================================================================

FUNCTION   RRCRBRC_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrbrc_is_srns_in_progress
(
  void 
);


/*===========================================================================

FUNCTION   RRCTCRC_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrctcrc_is_srns_in_progress
(
  void 
);


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
);


/*===========================================================================

FUNCTION   TDSRRCCU_VALIDATE_SRNS_RELOCATION_INFO()

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
boolean tdsrrccu_validate_srns_relocation_info
(
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  uint32 dl_sdu_type                          /* Downlink SDU type */
);


/*===========================================================================

FUNCTION   TDSRRCCU_VALIDATE_SRNS_RELOCATION_INFO_NEW()

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
boolean tdsrrcpcrc_validate_srns_relocation_info_new
(
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  tdsrrc_msg_ver_enum_type msg_version        /* Downlink message version */
);


/*===========================================================================

FUNCTION   TDSRRCCU_VALIDATE_SRNS_RELOCATION_INFO_NEW()

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
boolean tdsrrctcrc_validate_srns_relocation_info_new
(
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  tdsrrc_msg_ver_enum_type msg_version        /* Downlink message version */
);



/*===========================================================================

FUNCTION   RRCRBR_VALIDATE_SRNS_RELOCATION_INFO()

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
boolean tdsrrcrbr_validate_srns_relocation_info
(
tdsrrc_RadioBearerRelease *msg_ptr      /* Pointer to the RB Setup message */
,tdsrrc_msg_ver_enum_type msg_version
);

/*===========================================================================

FUNCTION  tdsrrc_validate_srns_relocation_info

DESCRIPTION
  This function is used to check if the necessary IEs are all included in the
  OTA messages. 

DEPENDENCIES


RETURN VALUE
  TRUE: the OTA is used to trigger a SRNS relocation
  FALSE: Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_validate_srns_relocation_info(tdsrrc_cmd_type *cmd_ptr, uint8 msg_type);

/*===========================================================================

FUNCTION  tdsrrc_is_fresh_in_reconfiguration_message

DESCRIPTION
  This function is used to check if the FRESH value is in the
  OTA messages.
 
  The function should be called after ASN.1 decoding, i.e.
  in the procedure modules. 

DEPENDENCIES


RETURN VALUE
  TRUE: the OTA has the FRESH carried in. 
  FALSE: Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_is_fresh_in_reconfiguration_message(void);

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
);

#endif /*FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION*/


#endif /* TDSRRCRBCOMMON_H */

