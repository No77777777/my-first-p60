/*===========================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/api/emm_ext_msg.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/03/2011 sar     moved to API folder and removed the ifdef
===========================================================================*/
#ifndef _EMM_EXT_MSG_H_
#define _EMM_EXT_MSG_H_

#include <customer.h>

#include "comdef.h"
//#include "lte_rrc_ext_msg.h"
#include "msgr_umid.h"
#include "msgr_types.h"
#include "msgr_lte.h"
#include "msgr_nas.h"
#include "sys.h"

/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/


/*===========================================================================

                        TYPES DEFINITION

===========================================================================*/
enum
{
  MSGR_DEFINE_UMID(NAS, EMM, IND, PLMN_CHANGE, 0x05, emm_plmn_change_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND,ATTACH_COMPLETE, 0x06, emm_attach_complete_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, T3402_CHANGED, 0x0C, emm_t3402_changed_ind_s_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, DEACT_NON_EMC_BEARER, 0x0D, emm_deact_non_emc_bearer_ind_s_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, RESET_APN_SWITCH, 0x0E, emm_reset_apn_switch_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, TAI_LIST, 0x0F, emm_tai_list_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, TAU_COMPLETE, 0x10, emm_tau_complete_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, RA_CAP_UPDATE_FAIL, 0x11, emm_ra_cap_update_fail_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, ATTACH_FAILED, 0x12, emm_attach_failed_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, TAU_FAILED, 0x13, emm_tau_failed_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, SEC_CTX_SET, 0x14, emm_sec_ctxt_set_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, SEC_CTX_DEL, 0x15, emm_sec_ctxt_del_ind_type),
  MSGR_DEFINE_UMID(NAS, EMM, IND, CELL_CHANGE, 0x16, emm_ssgccs_cell_change_ind_type)
};
#define LTE_RRC_MAX_NUM_TRACKING_AREAS 40 

#define MAX_NO_OF_TAI 16  /* Refer Section: 9.9.3.29; There can be a maximum of 16 TAIs*/
#define MAX_TAI_LST_LENGTH 16

typedef enum
{
  NAS_EMM_TAI_LST0,
  NAS_EMM_TAI_LST1,
  NAS_EMM_TAI_LST2

} lte_nas_tai_lst_type ;

typedef enum
{
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_SUCCESS,
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_TXN,
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_HO,
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_CONN_REL,
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_RLF,
  TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN,
  CONN_CNF_LTE_RRC_CONN_EST_SUCCESS,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_ABORTED,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_CELL_RESEL,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_CONFIG_FAILURE,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_TIMER_EXPIRED,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_LINK_FAILURE,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_NOT_CAMPED,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_SI_FAILURE,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_CONN_REJECT,
  CONN_REL_LTE_RRC_CONN_REL_NORMAL,
  CONN_REL_LTE_RRC_CONN_REL_RLF,
  CONN_REL_LTE_RRC_CONN_REL_CRE_FAILURE,
  CONN_REL_LTE_RRC_CONN_REL_OOS_DURING_CRE,
  CONN_REL_LTE_RRC_CONN_REL_ABORTED,
  CONN_REL_LTE_RRC_CONN_REL_SIB_READ_ERROR,
  CONN_REL_LTE_RRC_CONN_REL_ABORTED_IRAT_SUCCESS,
  EMM_SRV_REQ_FAILURE_NO_THROTTLE,
  EMM_SRV_REQ_FAILURE_THROTTLE,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_TAI_CHANGED,
  CONN_CNF_LTE_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE
} lte_emm_connection_cause_type ;


typedef struct
{
  sys_plmn_id_s_type      plmn;
  byte                    tac_lst_length ;
  word                    tac_lst[MAX_NO_OF_TAI]; /*(section 9.9.3.29.4) There can be a maximum of 16 elements, with two octets each*/
}lte_nas_tai_lst0_type; /*lte_nas_tai_...*/

/*Required for tai_type */
typedef struct
{
  sys_plmn_id_s_type     plmn;
  word                   tac; /*(section 9.9.3.29.4) Each TAC takes two octets*/
  boolean                t3247_specific; /*added temporarily while T3247 timer is running */
}lte_nas_tai_lst1_type; /*lte_nas_tai_list_type*/ 

/*Required for tai_type */
typedef struct{
  byte                    tai_lst_length ;
  lte_nas_tai_lst1_type   diff_plmn[LTE_RRC_MAX_NUM_TRACKING_AREAS]; /*(section 9.9.3.33) we can have 40 entries */
}lte_nas_tai_lst2_type; /*lte_nas_tai_plmns*/

/*Required for emm_tai_entry_type */
typedef union{
    lte_nas_tai_lst0_type    tai_lst0;
    lte_nas_tai_lst1_type    tai_lst1;
    lte_nas_tai_lst2_type    tai_lst2;
}tai_type;              

typedef struct  
{
  lte_nas_tai_lst_type tai_lst;
  tai_type             tai;   /* particular structure*/

} emm_tai_lst_elem_type ; 


/*Define TAI entry Type*/
typedef struct{

  byte                  tai_lst_length; /* Number of elements*/
  emm_tai_lst_elem_type tai_lst_elem[MAX_TAI_LST_LENGTH];

}lte_nas_emm_tai_lst_info_type;

typedef struct
{
   msgr_hdr_s     msg_hdr;    
   lte_nas_emm_tai_lst_info_type  tai_lst_info;
}emm_tai_list_ind_type;

/* PLMN change ind */
typedef struct
{
  /* Message router header */
  msgr_hdr_s          msg_hdr;  

  /* New PLMN */
  sys_plmn_id_s_type  plmn;

}emm_plmn_change_ind_type;

/*APN reset ind*/
typedef struct
{
  /* Message router header */
  msgr_hdr_s          msg_hdr;  
}emm_reset_apn_switch_ind_type;



typedef struct
{
  /* Message router header */
  msgr_hdr_s          msg_hdr; 
  boolean             null_algo_used;
}emm_attach_complete_ind_type;


typedef struct
{
  /* Message router header */
  msgr_hdr_s          msg_hdr;  
}emm_ra_cap_update_fail_ind_type;

typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr; 
  lte_emm_connection_cause_type   cause;
}emm_attach_failed_ind_type; 

typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr; 
  lte_emm_connection_cause_type   cause;
}emm_tau_failed_ind_type;


typedef enum 
{
  EMM_SSG_SEC_CONTEXT_TYPE_NONE,
  EMM_SSG_SEC_CONTEXT_TYPE_FULL_NATIVE_EPS,      /* context created with AKA and SMC complete or from USIM/EFS at power up */
  EMM_SSG_SEC_CONTEXT_TYPE_MAPPED_EPS           /* context created for Idle / HO from UTRAN/GERAN  to E-UTRAN */
}emm_nas_ssg_context_type;


typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr; 
  emm_nas_ssg_context_type   ctxt_est;
}emm_sec_ctxt_set_ind_type;


typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr; 
  emm_nas_ssg_context_type   ctxt_est;
}emm_sec_ctxt_del_ind_type; 

typedef enum 
{
  NAS_3GPP_INVALID_TYPE = -1,
  NAS_3GPP_PERIODIC_TYPE,    // Currently we need only this event. Added other for future cases.
  NAS_3GPP_TAU_CHANGE_TYPE,   //   When TAU changes
  NAS_3GPP_LOCAL_REL_TAU,
  NAS_3GPP_OTHER_TYPE,
  NAS_3GPP_MAX
}nas_tau_type;

 

typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr; 
  sys_plmn_id_s_type      plmn;
  sys_lac_type                  tac;
  nas_tau_type            tau_type;
}emm_tau_complete_ind_type;


/* T3402 change ind */
typedef struct emm_t3402_changed_ind_s
{
  /* Message router header */
  msgr_hdr_s  msg_hdr;

  /* T3402 value */  
  dword  t3402_value;

} emm_t3402_changed_ind_s_type;

/* Deactivate non emergency bearer ind */
typedef struct emm_deact_non_emc_bearer_ind_s
{
  /* Message router header */
  msgr_hdr_s  msg_hdr;

} emm_deact_non_emc_bearer_ind_s_type;

/* Sends cell change ind to ssgccs */
typedef struct
{
  /* Message router header */
  msgr_hdr_s                    msg_hdr;
  uint32                        cell_identity;
  sys_mcc_type                  mcc;
  sys_mnc_type                  mnc;
  sys_modem_as_id_e_type        as_id;
}emm_ssgccs_cell_change_ind_type;


/*===========================================================================

                        EXTERN FUNCTION PROTOTYPES

===========================================================================*/

extern boolean emm_search_tai_list
(
  /* Provided TAI */
  lte_nas_tai_lst1_type *provided_tai,
  /* Stored TAI list */
  lte_nas_emm_tai_lst_info_type *tai_list
);

/*===========================================================================

FUNCTION    EMM_DELETE_TAI_FROM_LIST

DESCRIPTION
  This function deletes provided TAI from the TAI list

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void emm_delete_tai_from_list
(
  /* Provided TAI */
  lte_nas_tai_lst1_type *provided_tai,

  /* Stored TAI list */
  lte_nas_emm_tai_lst_info_type *tai_list
);


extern
byte emm_get_srv_req_attempt_counter
(
  void
);
/*===========================================================================

FUNCTION  EMM_CHK_CAMP_FOR_EMC_ALLOWED

DESCRIPTION
  This function checks if registration is allowed on this PLMN
  
DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean emm_chk_camp_for_emc_allowed
(
  sys_plmn_id_s_type selected_plmn,
  uint16 tac,
  sys_modem_as_id_e_type sub_id
);

/*===========================================================================
FUNCTION    emm_get_sim_access_class

DESCRIPTION
  This function is called by DS to get access class information

DEPENDENCIES
  None

RETURN VALUE
  Access class information is returned
  
SIDE EFFECTS
  None
===========================================================================*/

word emm_get_sim_access_class
(
 sys_modem_as_id_e_type as_id
);
#endif /* _EMM_EXT_MSG_H_ */



