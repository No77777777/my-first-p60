/*!
  @file
  ds_3gpp_throttle_sm.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014-2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_pdn_throttle_sm.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/11   ss      Changes to support RPM.
08/16/10   hs      Initial version

===========================================================================*/

#ifndef DS_3GPP_PDN_THROTTLE_SM_H
#define DS_3GPP_PDN_THROTTLE_SM_H


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_throttle_sm.h"
#include "ds_3gpp_pdn_context.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
#include "ps_iface_defs.h"
#include "ps_sys_ioctl.h"
#include "cm.h"
#include "ds_3gpp_apn_table.h"
#include "ds3gcmif_ex.h"
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define DS_3GPP_PDN_THROT_INFINITE_THROT 0xFFFFFFFF
#define DS_PDN_THROT_LOG_PARAM_NA   0x0F
#define DS_PDN_THROT_MAX_APN_LOG_LENGTH  6
#define DS_PDN_THROT_LOG_LIST_TYPE_MAX (DS_MAX_APN_NUM * 2)
#define DS_3GPP_PDN_THROT_MIN_FAILURE_CNT 3

/*-------------------------------------------------------------------------- 
  These RAT Masks are used to indicate the RATs on which Throttling
  operations need to be performed. 
  Examples: 
  1) When SIM refresh happens we are supposed to clear  
     throttling information for all RATs, so 
     DS_3GPP_THROT_RAT_MASK_ALL is used. 
  2) If throttling applies to LTE and Global RATs,
     DS_3GPP_THROT_RAT_MASK_GLOBAL|DS_3GPP_THROT_RAT_MASK_LTE
     is used.
  3) If throttling applies to all UMTS RATs, then 
     DS_3GPP_THROT_RAT_MASK_UMTS is used.
  4) If throttling applies to all all RATs, DS_3GPP_THROT_RAT_MASK_GLOBAL
     is used
  -------------------------------------------------------------------------*/
#define DS_3GPP_THROT_RAT_MASK_ALL  DS_3GPP_THROT_RAT_MASK_GLOBAL| \
                                    DS_3GPP_THROT_RAT_MASK_UMTS|   \
                                    DS_3GPP_THROT_RAT_MASK_LTE     \

#define DS_3GPP_THROT_RAT_MASK_NONE   0x00000000 /* Not Set */
#define DS_3GPP_THROT_RAT_MASK_GLOBAL 0x00000001 /* Global Rat Mask. Indicates 
                                                    throttling which 
                                                    applies across all RATs */
#define DS_3GPP_THROT_RAT_MASK_UMTS   0x00000002 /* UMTS Rat Mask */
#define DS_3GPP_THROT_RAT_MASK_LTE    0x00000004 /* LTE Rat Mask */

/*-------------------------------------------------------------------------- 
  These PROC Masks are used to indicate the Procedures on which Throttling
  operations need to be performed. 
  Example: When User togggles Airplane mode, PDN Throttling corresponding
  to all procedures should be cleared. Hence DS_3GPP_THROT_PROC_MASK_ALL
  should be used when performing the throttle operation.
 -------------------------------------------------------------------------*/
#define DS_3GPP_THROT_PROC_MASK_ALL  DS_3GPP_THROT_PROC_MASK_GLOBAL| \
                                    DS_3GPP_THROT_PROC_MASK_PDN_CONN|   \
                                    DS_3GPP_THROT_PROC_MASK_BEARER_MOD|     \
                                    DS_3GPP_THROT_PROC_MASK_BEARER_ALLOC


#define DS_3GPP_THROT_PROC_MASK_NONE          0x00000000 /* Not Set */
#define DS_3GPP_THROT_PROC_MASK_GLOBAL        0x00000001 /* Global Rat Mask. Indicates 
                                                            throttling which 
                                                            applies across all RATs */
#define DS_3GPP_THROT_PROC_MASK_PDN_CONN      0x00000002 /* UMTS Rat Mask */
#define DS_3GPP_THROT_PROC_MASK_BEARER_MOD    0x00000004 /* LTE Rat Mask */
#define DS_3GPP_THROT_PROC_MASK_BEARER_ALLOC  0x00000008

#define DS_3GPP_THROT_REATTEMPT_RATC_BMASK    0x01
#define DS_3GPP_THROT_REATTEMPT_EPLMN_BMASK   0x02

/*-------------------------------------------------------------------------- 
  This enum defines the various Procedure Types. Proc Based PDN Throttle
  Table will have an array entry for each of the below Proc Types
 -------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PDN_THROT_PROC_MIN = -1,
  DS_3GPP_PDN_THROT_PROC_GLOBAL,
  DS_3GPP_PDN_THROT_PROC_PDN_CONN,
  DS_3GPP_PDN_THROT_PROC_BEARER_ALLOC,
  DS_3GPP_PDN_THROT_PROC_BEARER_MOD,
  DS_3GPP_PDN_THROT_PROC_MAX
} ds_3gpp_pdn_throt_proc_e_type;

typedef enum 
{
     DS_3GPP_UNTHROTTLED = 0,
     DS_3GPP_THROTTLED_V4,
     DS_3GPP_THROTTLED_V6,
     DS_3GPP_THROTTLED_V4V6
} ds_3gpp_pdn_throttle_state_e;

/*------------------------------------------------------------------------------
  This enumeration indicates the PDN Throttling type. 
------------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PDN_THROT_TYPE_NOT_SPECIFIED = 0,                         
  DS_3GPP_PDN_THROT_TYPE_DATA_RETRY,
  DS_3GPP_PDN_THROT_TYPE_DEFAULT = DS_3GPP_PDN_THROT_TYPE_DATA_RETRY,
  DS_3GPP_PDN_THROT_TYPE_PDN_CONN_LIMIT,                        
  DS_3GPP_PDN_THROT_TYPE_PDN_REQ_WAIT_TIME,
  DS_3GPP_PDN_THROT_TYPE_SPEC,
  DS_3GPP_PDN_THROT_TYPE_CC_SP,
  DS_3GPP_PDN_THROT_TYPE_RPM,
  DS_3GPP_PDN_THROT_TYPE_GLOBAL,
  DS_3GPP_PDN_THROT_TYPE_DAM,
  DS_3GPP_PDN_THROT_TYPE_CONFIG , /*for generic throttling*/
  DS_3GPP_PDN_THROT_TYPE_MAX
} ds_3gpp_pdn_throttle_sm_type;

/*------------------------------------------------------------------------------
  This Macro validates the PDN Throttle Type value
------------------------------------------------------------------------------*/
#define DS_3GPP_PDN_THROT_VALIDATE_SM_TYPE(sm_type) \
          (sm_type >= DS_3GPP_PDN_THROT_TYPE_MAX || \
		       sm_type <= DS_3GPP_PDN_THROT_TYPE_NOT_SPECIFIED)? FALSE: TRUE

/*-------------------------------------------------------------------------
  The mask will be used to indicate if any Global PDN Throttling is                          
  in progress. 
--------------------------------------------------------------------------*/

typedef enum
{
  DS_3GPP_PDN_THROT_APN_DISABLED_MASK              = 0x00000001,
  DS_3GPP_PDN_THROT_UI_DATA_DISABLED_MASK          = 0x00000002,
  DS_3GPP_PDN_THROT_UI_DATA_ROAMING_DISABLED_MASK  = 0x00000004,
  DS_3GPP_PDN_THROT_ROAMING_DISABLED_MASK          = 0x00000008,
  DS_3GPP_PDN_THROT_APN_PREF_SYS_MASK              = 0x00000010,
  DS_3GPP_PDN_THROT_IMS_DISABLE_MASK               = 0x00000020
} ds_3gpp_pdn_throt_global_mask_type;

/*------------------------------------------------------------------------
  Structure to hold information about pdn throttling for logging
-------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8         subs_id;
  unsigned int  is_throttled:4;
  uint8         throttle_ip_type:4;
  uint8         profile_id;
  uint8         counter;  
  uint16        duration;
  uint16        mcc;
  uint16        mnc;
  char          apn[6];
  char          reserved[4];
} ds_3gpp_pdn_throttle_event_type;

/*---------------------------------------------------------------------------
Enum's - overridden network ip type
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PDN_THROT_PDP_UNDEFINED,
  DS_3GPP_PDN_THROT_PDP_IPV4,
  DS_3GPP_PDN_THROT_PDP_IPV6,
}ds_3gpp_pdn_throt_ntw_o_ridden_ip_type_e;

/*---------------------------------------------------------------------------
Enum's - network overridden cause
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PDN_THROT_BEARER_ACT_ACCEPT       = 0,
  DS_3GPP_PDN_THROT_CONN_REJ                = 1,
  DS_3GPP_PDN_THROT_NTW_O_RIDE_CAUSE_MAX    = 2
}ds_3gpp_pdn_throt_ntw_o_ride_cause;

/*---------------------------------------------------------------------------
Enum's - Throttling SM table type - whether throttling is due to ntw override 
or for other reasons  
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PDN_THROT_PLMN_BASED_THROT_TBL,
  DS_3GPP_PDN_THROT_NTW_O_RIDE_THROT_TBL,
  DS_3GPP_PDN_THROT_MAX 
}ds_3gpp_pdn_throt_type_e;

/*---------------------------------------------------------------------------
Per-rat Dynamic pointer structure containing the ip type and override cause
-----------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pdn_throt_ntw_o_ridden_ip_type_e      ntw_overridden_ip;
  ds_3gpp_pdn_throt_ntw_o_ride_cause            override_cause;
  sys_plmn_id_s_type                            plmn_id;
  sys_plmn_list_s_type                         *eplmn_list_ptr;
                                                /*Stores the list of eplmns for 
                                                  which throttling has to be applied */
  boolean                                       ip_fallback_enabled; 
}ds_3gpp_pdn_throt_ntw_o_ride_ip_s;

/*------------------------------------------------------------------------------
  This structure stores the PDN Throttling information for a given RAT
------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gppi_throttle_sm_s                   *v4_throttle_sm;
                                               /* V4 Throttle SM */
  ds_3gppi_throttle_sm_s                   *v6_throttle_sm;
                                               /* V6 Throttle SM */
                                           
  ds_3gpp_pdn_throttle_sm_type              sm_type;
                                               /* type of PDN Throttling */
  void                                      *proc_based_info_p;
                                               /* back pointer to parent */
  ds_3gpp_throt_rat_e_type                  rat_type;
                                               /*RAT value*/
  boolean                                   init;
                                             /* indicates whether throttling 
                                                info is valid */
  sys_plmn_list_s_type                      *eplmn_list_ptr;
                                                /*Stores the list of eplmns for 
                                                  which throttling has to be applied */

}ds_3gpp_pdn_throt_rat_based_s;

/*------------------------------------------------------------------------------
  This structure stores the PDN Throttling information for a given Procedure
------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pdn_throt_rat_based_s   per_rat[DS_3GPP_THROT_RAT_MAX];
                                       /* Holds PDN Throttling Information
                                          for all RATs */
  uint8                           num_active_rats;
                                       /* Number of RATs whose throttling 
                                          is valid */
  void                           *plmn_based_info_p;

  ds_3gpp_pdn_throt_proc_e_type   proc_type;
}ds_3gpp_pdn_throt_proc_based_s;

/*------------------------------------------------------------------------------
  This structure stores the PDN Throttling information on a particular PLMN
------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pdn_throt_proc_based_s *per_proc[DS_3GPP_PDN_THROT_PROC_MAX];

  uint8                           num_active_procs;

  sys_plmn_id_s_type              plmn_id;
                                       /* PLMN Id */

  boolean                         eplmn_blocked;    
  /* indicates if PLMN based throt SM tbl contains blocked EPLMN list*/

  void                           *apn_throt_sm_ptr;  
                                       /* Back Pointer to parent */

} ds_3gpp_pdn_throt_sm_plmn_based_s;

/*------------------------------------------------------------------------------
  This structure stores the PDN Throttling information on a particular APN
------------------------------------------------------------------------------*/
typedef struct
{
  byte                                  apn[DS_UMTS_MAX_APN_STRING_LEN+1]; 
  ds_3gpp_pdn_throt_sm_plmn_based_s    *plmn_based_pdn_throt_tbl
                                         [DS_3GPP_MAX_PLMN_SUPPORTED];
  ds_3gpp_pdn_throt_ntw_o_ride_ip_s            *network_override_ip_info_p
                                              [DS_3GPP_THROT_RAT_MAX];
  boolean                               global_plmn_is_blocked; 
  uint32                                global_throttle_mask;
  boolean                               valid;
} ds_3gpp_pdn_throttle_sm_apn_based_dyn_s;

/*------------------------------------------------------------------------------
  This table maintains PDN Throttling Info for all APNs
------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pdn_throttle_sm_apn_based_dyn_s 
                   *apn_based_dyn_tbl[DS_MAX_APN_NUM];
} ds_3gpp_pdn_throttle_sm_apn_based_s;

typedef struct 
{
  byte       apn[DS_UMTS_MAX_APN_STRING_LEN+1];
  uint8      len;
  boolean    attach_allowed; 
}ds_3gpp_pdn_throt_apn_list_type;

/*------------------------------------------------------------------------------
  This structure stores the identification information used to fetch the
  handle corresponding to the correct PDN Throttle SM and 
  performs actions on that SM
------------------------------------------------------------------------------*/
typedef struct
{
  byte                              *apn;
  boolean                            permanent_throt;
  /* Throttle for infinite time */
  boolean                            globally_blocked;
  /* Block for all plmns for specified apn */
  boolean                            throt_all_eplmns;
  /* Block for all home plmns for specified apn */
  sys_modem_as_id_e_type             subs_id;
  sys_plmn_id_s_type                 plmn_id;
  ds_3gpp_throt_rat_e_type           rat_type;
  ds_3gpp_pdn_throttle_sm_type       sm_type;
  ds_umts_pdp_type_enum_type         pdp_type;
  ps_iface_net_down_reason_type      cause_code;
  ds_pdn_context_s                  *pdn_cntx_p;
  ds_3gpp_throttle_sm_cb_data_type  *cb_data_p;
  ds_3gpp_pdn_throt_proc_e_type      proc_type;
} ds_3gpp_pdn_throt_ident_info_type;

/*------------------------------------------------------------------------------ 
  This structure stores the profile number only if a particule apn is PDN
  throttled. This is needed to log the throttle information when timer expires
  We will not have access to the throttled profile information otherwise
  ------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gppi_throttle_sm_s *throttle_sm_ptr;
  uint16                  profile_num;
}ds_3gpp_pdn_throt_log_list_type;

typedef struct
{
  boolean                 v4_throttle_status;
  boolean                 v6_throttle_status;
}ds_3gpp_pdn_throt_temp_status_type;

/*------------------------------------------------------------------------------ 
  Structure used when posting commands related to PDN Throttle SM
  ------------------------------------------------------------------------------*/
typedef struct
{
  int   subs_id;
  void  *throt_sm_ptr;
}ds_3gpp_pdn_throt_sm_info_type;

/*------------------------------------------------------------------------------ 
  Defines a function pointer which can be registered by various PDN Throttle
  modules to define module-specific actions to be taken when 
  Throttle timer is started or stopped
  ------------------------------------------------------------------------------*/
typedef ds_3gppi_throttle_sm_timer_ev_cb_f ds_3gpp_pdn_throt_sm_timer_ev_f_type;

/*------------------------------------------------------------------------------ 
  Defines a function pointer which can be registered by various PDN Throttle
  modules to define module-specific action needed to fetch the correct Throttle Timer
  value
  ------------------------------------------------------------------------------*/
typedef ds_3gppi_throttle_sm_get_value_f   ds_3gpp_pdn_throt_sm_get_value_f_type;

/*------------------------------------------------------------------------------ 
  Defines a function pointer which can be registered by various PDN Throttle
  modules to define module-specific checks on whether throttling can be cleared
  upon certain condidions being met
  ------------------------------------------------------------------------------*/
typedef ds_3gppi_throttle_clear_on_trigger_f  ds_3gppi_throttle_clear_on_trigger_f_type;

typedef boolean (*ds_3gpp_pdn_throt_validate_f_type) (void * throt_sm_ptr, 
                                                      void* cb_data);

/*---------------------------------------------------------------------------------
  This structure maintains information specific to each PDN Throttle Type.
  tmr_exp_f:                 Function to be invoked when Throttle Timer expires
  tmr_start_f:               Function to be invoked when Throttle Timer starts
  get_tmr_val_f:             Function to fetch the Throttle timer value
  clear_on_condition_f:      Function to check whether throttling can be cleared
                             on receipt of the passed in reset_type. The throt modules
                             must provide their own implementation of this and
                             cannot be NULL.
  attach_is_allowed          Flag which specifies whether Attach is allowed even
                             if PDN throttling is in progress
  flush_plmn_allowed         Set when APN is blocked for a specified PLMN, when PLMN
                             is unblocked then the flag should be set to TRUE.
-----------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pdn_throt_sm_timer_ev_f_type     tmr_exp_f;                                   
  ds_3gpp_pdn_throt_sm_timer_ev_f_type     tmr_start_f;                                   
  ds_3gpp_pdn_throt_sm_get_value_f_type    get_tmr_val_f;    
  ds_3gppi_throttle_clear_on_trigger_f_type 
                                           clear_on_condition_f;
  boolean                                  attach_is_allowed;
  boolean                                  flush_plmn_allowed;
} ds_3gpp_pdn_throt_sm_tbl_s;

/*---------------------------------------------------------------------------------
  Defines the Global PLMN Id
-----------------------------------------------------------------------------------*/
extern const sys_plmn_id_s_type global_plmn_id;

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_umts_tlb_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_3gpp_pdn_throt_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_REGISTER_SM_INFO_TBL

DESCRIPTION
  This function is used by various PDN Throttling modules to register
  their respective info tables, which specify attributes such as the
  start timer callback, stop timer callback, Get Timer Value Callback, 
  attach_is_allowed flag and the reset_mask.

PARAMETERS   :
  info_tbl_p        - Pointer to the Info Table
  pdn_throt_sm_type - Type of PDN Throttling
  subs_id           - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  uint32.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_register_sm_info_tbl
(
  ds_3gpp_pdn_throt_sm_tbl_s    *info_tbl_p,
  ds_3gpp_pdn_throttle_sm_type   pdn_throt_sm_type,
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_THROTTLE_STATE

DESCRIPTION
  This function clears throttling state for a pdn context and a given ip
  type.

PARAMETERS   :
   ds_pdn_context_s *         - pdn context ptr
   ds_umts_pdp_type_enum_type - pdp type
   rat_mask                   - Mask Indicating the RAT Types for which
                                throttling needs to be cleared
   proc_mask                  - Mask Indicating the Procedures for which
                                throttling needs to be cleared
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_clear_throttle_state
(
  byte                       *apn,
  ds_umts_pdp_type_enum_type  pdn_pdp_type,
  sys_modem_as_id_e_type      subs_id,
  uint32                      rat_mask,
  uint32                      proc_mask
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_THROTTLE_STATE_EX

DESCRIPTION
  This function clears throttling state for a pdn context and a given ip
  type.

PARAMETERS   :
   ds_pdn_context_s *         - pdn context ptr
   ds_umts_pdp_type_enum_type - pdp type
   rat_mask                   - Mask Indicating the RAT Types for which
                                throttling needs to be cleared
   proc_mask                  - Mask Indicating the Procedures for which
                                throttling needs to be cleared
  throt_plmn_id_ptr           - Throttled PLMN ID
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_clear_throttle_state_ex
(
  byte                                   *apn,
  ds_umts_pdp_type_enum_type              pdn_pdp_type,
  sys_modem_as_id_e_type                  subs_id,
  uint32                                  rat_mask,
  uint32                                  proc_mask,
  sys_plmn_id_s_type                     *throt_plmn_id_ptr,
  ds_3gpp_pdn_throttle_sm_type            sm_type
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_THROTTLE_PER_THROTTLE_SM_TYPE

DESCRIPTION
  This function clears throttling state for a given throttle type and APN and
  based on other inout params

PARAMETERS   : 
   apn                        - apn name for which throttling needs to be cleared
   proc_type                  - Procedure type for which
                                throttling needs to be cleared
   ds_umts_pdp_type_enum_type - pdp type
   rat_type                   - the RAT Type for which
                                throttling needs to be cleared
   clear_for_global_plmn      - Clear throttle globally across all PLMNs?
   sm_type                    - Throttle SM type
   subs_id                    - Subs id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_clear_throttle_per_throttle_sm_type
(
  byte                              *apn,
  ds_3gpp_pdn_throt_proc_e_type      proc_type,
  ds_umts_pdp_type_enum_type         pdn_pdp_type,
  ds_3gpp_throt_rat_e_type           rat_type,
  boolean                            clear_for_global_plmn,
  /*meaning do you want to clear the thrttle globally across all PLMNs*/
  ds_3gpp_pdn_throttle_sm_type       sm_type,
  sys_modem_as_id_e_type             subs_id
) ;
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_PER_APN_POWER_UP_INIT

DESCRIPTION
  This function initializes the data structure needed to perform PDN throt
  on a per APN basis.

  This initializes the APN names to NULL and initializes the pointer array
  of PDN throt SM per APN to NULL

PARAMETERS
   None

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_sm_per_apn_power_up_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_INFO_FOR_PLMN

DESCRIPTION
  This function retrieves the pdn throttle SM info for a specific
  plmn_id. It is used by IOCTL handler
  DS_3GPP_PDN_CNTX_GET_THR0TTLE_INFO

PARAMETERS   :
   plmn_id        - plmn_id
   *throttle_info - retrieved throttle info is put here
   rat_mask       - Mask indicating the RATs for which PDN
                    Throttling Info needs to be obtained
   subs id        - Subscription Id
   client_type    - Internal or External Client
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_sm_get_throttle_info_for_plmn
(
  sys_plmn_id_s_type                   *plmn_ptr,
  ps_sys_ioctl_pdn_throttle_info_type  *throttle_info,
  uint32                                rat_mask,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_INFO_FOR_PLMN_EXT

DESCRIPTION
  This function retrieves the pdn throttle SM info for a specific
  plmn_id. It is used by IOCTL handler
  DS_3GPP_PDN_CNTX_GET_THR0TTLE_INFO

PARAMETERS   :
   plmn_id                   - plmn_id
   *throttle_info            - retrieved throttle info is put here
   rat_mask                  - Mask indicating the RATs for which PDN
                               Throttling Info needs to be obtained
   subs id                   - Subscription Id
   is_internal_client        - Internal throttle information requested
                                i.e. MPPM client does not want our
                                     APN_PREF_SYS throttling info so will
                                     set this to FALSE as an external client
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_sm_get_throttle_info_for_plmn_ext
(
  sys_plmn_id_s_type                   *plmn_ptr,
  ps_sys_ioctl_pdn_throttle_info_type  *throttle_info,
  uint32                                rat_mask,
  sys_modem_as_id_e_type                subs_id,
  boolean                               is_internal_client
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_SM_FROM_PLMN

DESCRIPTION
  This function retrieves the pdn throttle_sm for a specific plmn_id and apn
  for the global rat mask

PARAMETERS   :
   plmn_id - plmn_id
   *apn    - the apn for which the pdn throttle_sm is to be retrieved.
   proc_id - Procedure for which pdn throttle sm is to be retrieved

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void *ds_3gpp_pdn_throt_sm_get_throttle_sm_from_plmn
(
  sys_plmn_id_s_type            plmn_id,
  byte                          *apn,
  sys_modem_as_id_e_type        subs_id,
  ds_3gpp_pdn_throt_proc_e_type proc_id,
  ds_3gpp_throt_rat_e_type      rat_type
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_SM_FROM_PLMN

DESCRIPTION
  This function retrieves the pdn throttle_sm_info pointer for a specific 
  plmn_id and apn.

PARAMETERS   :
   plmn_id - plmn_id
   *apn    - the apn for which the pdn throttle_sm is to be retrieved.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/


void *ds_3gpp_pdn_throt_sm_get_throttle_sm_info_from_plmn
(
  sys_plmn_id_s_type     plmn_id,
  byte                   *apn,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_APN_INDEX_PTR

DESCRIPTION
  This function retrieves the pdn throttle_sm for a apn and subs id
  for the global rat mask

PARAMETERS   :
  apn          - APN Name
  allow_create - Whether Entry can be created if not already present
  subs_id      - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
 Pointer to the Throttling state machine

SIDE EFFECTS
  None

===========================================================================*/
void* ds_3gpp_pdn_throt_sm_get_apn_index_ptr
(
  byte                          *apn,
  boolean                        allow_create,
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_SM

DESCRIPTION
  This function gets a Throttling state machine instance ptr for a apn on the 
  current plmn. If the pointer does not exist ;an entry will be allocated.
  NOTE: Use this function if you want to allocate/fetch a PDN Throttling 
  Pointer for a particular RAT

PARAMETERS   :
  throt_ident_info_p - Information to identify/create the correct PDN Throttle 
                       SM
  allow_create       - Flag indicating whether creation is allowed

DEPENDENCIES
  None.

RETURN VALUE
 Pointer to the Throttling state machine

SIDE EFFECTS
  None

===========================================================================*/
void* ds_3gpp_pdn_throt_sm_get_throttle_sm_per_rat
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p,
  boolean                            allow_create
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_SM

DESCRIPTION
  This function gets a Throttling state machine instance ptr for a apn on the 
  current plmn. If the pointer does not exist ;an entry will be allocated.
  NOTE: Use this function if you do not need to allocate/fetch a PDN Throttling 
  Pointer for any particular RAT

PARAMETERS   :
  apn              - 
  globally_blocked - Flag indicating if globally blocked throttling state machine
                     is needed
  allow_create     - Flag indicating whether creation is allowed
  sm_type          - Throttling state machine type requested
  subs_id          - Subscription id of the client
DEPENDENCIES
  None.

RETURN VALUE
 Pointer to the Throttling state machine

SIDE EFFECTS
  None

===========================================================================*/
void* ds_3gpp_pdn_throt_sm_get_throttle_sm
(
  byte                          *apn,
  boolean                        globally_blocked,
  boolean                        allow_create,
  ds_3gpp_pdn_throttle_sm_type   sm_type,
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_THROTTLE_SM_PLMN_BASED

DESCRIPTION
  This function gets a Throttling state machine instance ptr for a apn, plmn,
  subscription id pair. If the pointer does not exist ;an entry will be allocated

PARAMETERS   :
  throt_ident_info_p - Information to identify the correct PDN Throttle SM
  allow_create       - Flag indicating whether creation is allowed

DEPENDENCIES
  None.

RETURN VALUE
 Pointer to the Throttling state machine

SIDE EFFECTS
  None

===========================================================================*/

void* ds_3gpp_pdn_throt_sm_get_throttle_sm_plmn_based
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p,
  boolean                           allow_create
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_PDN_THROTTLED

DESCRIPTION
  This function clears throttling state for a pdn context and a given ip
  type.

PARAMETERS   :
   byte* - apn name ptr
   ds_umts_pdp_type_enum_type - pdp type
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pdn_throt_is_apn_throttled
(
  byte                       *apn,
  ds_pdn_context_type_e       call_type,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_APN_THROTTLED_SM_TYPE

DESCRIPTION
  This function checks throttling state for an apn, given ip
  type, subscription, and a given throttle type

PARAMETERS   :
   byte* - apn name ptr
   ds_pdn_context_type_e - pdp type
   sys_modem_as_id_e_type - subscription id
   sm_type                - sm type
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pdn_throt_is_apn_throttled_sm_type
(
  byte                         *apn,
  ds_pdn_context_type_e         call_type,
  sys_modem_as_id_e_type        subs_id,
  ds_3gpp_pdn_throttle_sm_type  sm_type
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_HANDLE_CONN_RESPONSE

DESCRIPTION
  This function handles the PDP/PDN connection response. Delegates the
  response to the RPM handler function for appropriate action.

PARAMETERS   :
  conn_status    - whether network accepted or rejected.
  *pdn_context_p - pdn_context_ptr.
  down_reason    - the ps_iface_cause code for reject.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_handle_conn_response
(
  boolean                       conn_status,
  ds_pdn_context_s*             pdn_context_p,
  ps_iface_net_down_reason_type down_reason
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_REMAINING_THROTTLE_TIMER_VAL

DESCRIPTION
  This function is a utility that returns remaining throttle timer val for a
  particular apn and RAT type

PARAMETERS   :
  *apn    - APN for which remaining throttle timer value is retrieved.

DEPENDENCIES
  None.

RETURN VALUE
  uint32.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 ds_3gpp_pdn_throt_get_remaining_throttle_timer_val
(
  byte                      *apn,
  sys_modem_as_id_e_type     subs_id,
  ds_3gpp_throt_rat_e_type   rat_type
);
#ifdef FEATURE_DATA_RPM
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

#endif /* FEATURE_DATA_RPM */

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_PREV_CAUSE_CODE

DESCRIPTION
  This function returns the previous IPV4 and IPV6 cause codes stored in
  the core throttle state machine
 
PARAMETERS   : 
 
  pdn_throt_p       -  PDN throttle state machine pointer to fetch the cause
                       code from the core throttle state machine
  v4_cause_code_ptr -  Failure cause code ptr to populate V4 causecode
  v6_cause_code_ptr -  Failure cause code ptr to populate V6 causecode 

DEPENDENCIES
  None.

RETURN VALUE
  boolean -  TRUE indicating SUCCESS
		     FALSE indicating pdn_throt sm ptr has not been allocated

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_pdn_throt_get_prev_cause_code
(
  void                               *pdn_throt_ptr,
  ps_iface_net_down_reason_type      *v4_cause_code_ptr,
  ps_iface_net_down_reason_type      *v6_cause_code_ptr,
  sys_modem_as_id_e_type              subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_SM_TYPE

DESCRIPTION
  This function gets the PDN Throttle SM Type.
 
PARAMETERS   : 
  apn:              APN Name
  globally_blocked: Whether the APN is globally blocked
  proc_type:        Procedure Type
  rat_type:         Throttling info for the rat specified
  throt_sm_type_p:  SM Type to be returned
  
DEPENDENCIES
  None.

RETURN VALUE
  boolean -  TRUE indicating SUCCESS
             FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_pdn_throt_get_sm_type
(
  byte                          *apn,
  sys_modem_as_id_e_type         subs_id,
  boolean                        globally_blocked,
  ds_3gpp_pdn_throt_proc_e_type  proc_type, 
  ds_3gpp_throt_rat_e_type       rat_type,
  ds_3gpp_pdn_throttle_sm_type  *throt_sm_type_p
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SET_FAILURE_CAUSE_CODE

DESCRIPTION
  This function sets the failure cause based on the PDP type
 
PARAMETERS   : 
 
  pdn_throt_p: PDN throttle state machine pointer to set the cause code
               in the core throttle state machine
  cause_code:  Failure cause code to be set
  pdn_pdp_type:PDP type

DEPENDENCIES
  None.

RETURN VALUE
  boolean -  TRUE indicating SUCCESS
             FALSE indicating pdn_throt sm ptr has not been allocated

SIDE EFFECTS
  None.
============================================================================*/
boolean ds_3gpp_pdn_throt_set_failure_cause_code
(
  void                          *pdn_throt_p,
  void                          *cause_code,
  ds_umts_pdp_type_enum_type    pdn_pdp_type
);


/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_HANDLE_PERM_CONN_FAILURE

DESCRIPTION
  This function sets INFINITE THROTTLING for a PDN context.
  Set the is_throttled flag to TRUE, Set the Timer value to 0xFFFFFFFF

PARAMETERS   : 
  pdn_throt_p:        PDN throttle state machine pointer to set the 
                       cause code in the core throttle state machine
  throt_ident_info_p:  PDN Throttle Parameters
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_pdn_throt_handle_perm_conn_failure
(
  void                               *pdn_throt_p,
  ds_3gpp_pdn_throt_ident_info_type  *throt_ident_info_p
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_INFINITE_THROTTLING

DESCRIPTION
  This function clears INFINITE THROTTLING for a PDN context.
  The is_throttled flag is set to FALSE and the Timer value is cleared.
  

PARAMETERS: 
 subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_pdn_throt_clear_infinite_throttling
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_SET_SM_TYPE

DESCRIPTION
  This function sets the pdn throttling state machine type
  
PARAMETERS   : 
  rat_based_pdn_throt_p - pdn throttling pointer
  sm_type                - state machine type

DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_sm_set_sm_type
(
  ds_3gpp_pdn_throt_rat_based_s   *rat_based_pdn_throt_p,
  ds_3gpp_pdn_throttle_sm_type     sm_type
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_RESET_PDN_THROTTLING

DESCRIPTION
  This function clears cause code specific THROTTLING for a PDN context.
  The is_throttled flag is set to FALSE and the Timer value is cleared.
  

PARAMETERS: 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_reset_pdn_throttle
(
  sys_modem_as_id_e_type subs_id,
  uint32                 rat_mask
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_UNBLOCK_THROTTLING_ON_NV_REFRESH

DESCRIPTION
  This function clears UE specific throttling upon NV refresh
  
PARAMETERS: 
  subs_id = subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_unblock_throttling_on_nv_refresh
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_READ_DEVICE_SETTINGS_INFO

DESCRIPTION
  This function reads the device settings info again and sets/unsets the
  throttling
  
PARAMETERS: 
  subs_id = subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_read_device_settings_info
(
  sys_modem_as_id_e_type       subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_RESET_PDN_THROTTLING_PER_PLMN

DESCRIPTION
  This function clears cause code specific THROTTLING for a PDN context.
  The is_throttled flag is set to FALSE and the Timer value is cleared.
  

PARAMETERS: 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_reset_pdn_throttle_ex
  (sys_modem_as_id_e_type, uint32, sys_plmn_id_s_type);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_PERFORM_REG_THROTTLING

DESCRIPTION
  This function performs regular throttling and sets failure cause code
  

PARAMETERS   : 
  throt_ident_info_p - Throttling parameters
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_perform_reg_throttling
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_VALIDATE_MIN_FAILURE_COUNT

DESCRIPTION
  This function ensures that the throttle failure count is atleast
  equal to the minimum throttle failure count passed
  

PARAMETERS   : 
  ds_pdn_context_s           -  Rejected PDN cntxt pointer
  min_throt_failure_cnt      -  The min failure count of V4/V6 core
                                throttle SM
  pdn_pdp_type               -  PDP type
  rat_type                   -  RAT on which throttling needs to be performed
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_validate_min_failure_count
(
  ds_3gppi_throttle_sm_s  *throt_sm_ptr,
  uint8                    min_throt_failure_cnt
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_PERFORM_APN_THROTTLING

DESCRIPTION
  This function performs PDN throttling based on Release 10 requirements
  

PARAMETERS   : 
  pdn_throt_sm          - PDN throttle state machine pointer
  throt_ident_info_p    - Throttling parameters
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_perform_apn_throttling
(
  void                                *pdn_throt_p,
  ds_3gpp_pdn_throt_ident_info_type   *throt_ident_info_p

);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_APN_DISABLE_POWERUP_THROT

DESCRIPTION
  This function performs apn disable specifc throttling at power up

PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_apn_disable_powerup_throttle
(
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_UNBLOCK_APN_DISABLED_THROTTLING

DESCRIPTION
  This function unblocks PDN throttling because APN was re-enabled.
  
PARAMETERS
  1. APN name
  2. Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_unblock_apn_disabled_throttling
(
  byte*                                apn,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_RESET_PDN_THROT_PER_PLMN

DESCRIPTION
  This function resets the pdn throttling state machine
  
PARAMETERS   : 
  rat_based_pdn_throt_p - pdn throttling pointer

DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_pdn_throt_sm_reset_pdn_throt_per_plmn
(
  ds_3gpp_pdn_throt_rat_based_s *rat_based_pdn_throt_p
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SET_GLOBAL_THROTTLE_FLAG

DESCRIPTION
  This function sets the global throttle flag for the given APN

PARAMETERS   : 
  apn - APN name
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_set_global_throttle_flag
(
  byte                     *apn,
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_BLOCKED_APN_LIST_ON_PLMN_ID

DESCRIPTION
  This function gets the blocked list of of APNs from the PDN throt
  SM on the passed PLMN ID
  
PARAMETERS   : 
 
  list - list of blocked APNs (output parameter)

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_pdn_throt_get_blocked_apn_list_on_plmn_id
(
  ds_3gpp_pdn_throt_apn_list_type *blocked_apn_list,
  uint8                           *num_blocked_apn,
  sys_modem_as_id_e_type           subs_id,
  sys_plmn_id_s_type               plmn_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_ADVERTISE_GLOBALLY_BLOCKED_APNS_to_APM

DESCRIPTION
  This function sends the currently blocked list of of APNs to APM
  
PARAMETERS   : 
  globally_blocked_apn_list
  num_blocked_apns
 
DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_advertise_globally_blocked_apns_to_apm
(
  ds_3gpp_pdn_throt_apn_list_type *globally_blocked_apn_list,
  uint8                            num_blocked_apns,
  sys_modem_as_id_e_type           subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_GLOBAL_PLMN_BLOCKED

DESCRIPTION
  This function checks if the global PLMN is blocked.

PARAMETERS   :
   byte* - apn name ptr
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pdn_throt_is_global_plmn_blocked
(
  byte                   *apn,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_APN_GLOBALLY_BLOCKED

DESCRIPTION
  This function checks if given APN is blocked due to any of the below reasons:
  1. Global PLMN blocking 
  2. Global throttle mask enabled(This mask is set when APN is disabled, 
     UE is disabled or Roaming is disabled)

PARAMETERS   :
   byte* - apn name ptr
   Subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if throttled
  FALSE, otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pdn_throt_is_apn_globally_blocked
(
  byte                   *apn,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_HANLDE_NW_INITED_UNBLOCK_APN 
 
DESCRIPTION
  This function handles the network initiated to unblock a globally blocked 
  APN based on Release 10 requirements
  

PARAMETERS   : 
  apn                        -  APN name on the PDN connection
  subs_id                    -  Subscription Id
  rat_mask                   -  Mask Indicating the RAT Types for which
                                throttling needs to be unblocked

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_handle_nw_inited_unblock_apn
(
  byte                           *apn,
  sys_modem_as_id_e_type         subs_id,
  uint32                         rat_mask
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_APN_REJECT_GET_APN_NAME

DESCRIPTION
  This function obtains the list of APNs throttled on
  current PLMN and advertises this list to APM.

PARAMETERS
  Subscription Id            
 
DEPENDENCIES
  None.

RETURN VALUE 
  None 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_advertise_blocked_apns_current_plmn
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  FUNCTION DS_3GPP_PDN_THROT_ADVERTISE_BLOCKED_APNS_WITH_PLMN

DESCRIPTION
  This function obtains the list of APNs throttled on
  PLMN passed as argument and advertises this list to APM.

PARAMETERS
  Subscription Id            
  PLMN
 
DEPENDENCIES
  None.

RETURN VALUE 
  None 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_advertise_blocked_apns_with_plmn
(
  sys_modem_as_id_e_type    subs_id,
  sys_plmn_id_s_type        plmn_id
);


/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_HDL_GEN_EVENT

DESCRIPTION
  This function would populate the structure and generate an event
  when pdn throttling happens

PARAMETERS
   uint8                            failure_count
   uint16                           timer_val
   boolean                          is_throttled
   sys_modem_as_id_e_type           subs_id
   sys_plmn_id_s_type               *plmn_id
   uint8                            throttle_ip_type
   char                             *apn
   uint8                            profile_id
 
DEPENDENCIES

RETURN VALUE 
  void
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_hdl_gen_event
(
   uint8                            failure_count,
   uint32                           timer_val,
   boolean                          is_throttled,
   sys_modem_as_id_e_type           subs_id,
   sys_plmn_id_s_type               *plmn_id,
   uint8                            throttle_ip_type,
   char                             *apn,
   uint8                            profile_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_ADD_LOG_ENTRY

DESCRIPTION
  This function would add profile number to pdn_throt_profile_num_list

PARAMETERS
   ds_3gppi_throttle_sm_s*     throt_sm_ptr,
   uint16                      profile_num
 
DEPENDENCIES
  None

RETURN VALUE 
  void
 
SIDE EFFECTS

===========================================================================*/

void ds_3gpp_pdn_throt_add_log_entry
(
   ds_3gppi_throttle_sm_s     *throt_sm_ptr,
   uint16                      profile_num  
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_EV_ALLOC_MEM

DESCRIPTION
  This function would allocate memory for pdn_throt_ev_ptr from heap

PARAMETERS
  Void
 
DEPENDENCIES
  None.

RETURN VALUE 
  ds_3gpp_pdn_throttle_event_type ptr
 
SIDE EFFECTS
  None.

===========================================================================*/

ds_3gpp_pdn_throttle_event_type *ds_3gpp_pdn_throt_ev_alloc_mem
(
   void 
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_GEN_EVENT

DESCRIPTION
  This function would generate an event when pdn throttling
  happens

PARAMETERS
  ds_3gpp_pdn_throttle_event_type ptr
 
DEPENDENCIES
  The ptr must be populated with appropriate values

RETURN VALUE 
  void
 
SIDE EFFECTS
  None.

===========================================================================*/

void  ds_3gpp_pdn_throt_gen_event
(
   ds_3gpp_pdn_throttle_event_type *pdn_throt_ev_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_UPDATE_LOG_LIST

DESCRIPTION
  This function would update the logging list from apn and ip type

PARAMETERS
   ds_3gppi_throttle_sm_s*     throt_sm_ptr,
  ds_umts_pdp_type_enum_type  pdn_pdp_type   
 
DEPENDENCIES
  None

RETURN VALUE 
  void
 
SIDE EFFECTS/DEPENDENCIES 
PDP type must be IPV4 or IPV6 
 

===========================================================================*/

void ds_3gpp_pdn_throt_update_log_list
(
   ds_3gppi_throttle_sm_s     *throt_sm_ptr,
   ds_umts_pdp_type_enum_type  pdn_pdp_type 
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_LTE_DISABLE_THROTTLE_TIMER_VALUE

DESCRIPTION
  This function calculates the amount time PLMN needs to be blocked.
  
PARAMETERS
  Subscription Id            
 
DEPENDENCIES
  None.

RETURN VALUE 
  Timer value in seconds

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_3gpp_pdn_throt_lte_disable_throttle_timer_value
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_LOG_THROT_INFO

DESCRIPTION
  This function would check if the pdn throttling information would needed
  to be logged based on the is_throttled flag

PARAMETERS
   ds_3gppi_throttle_sm_s*     throt_sm_ptr,
   ds_umts_pdp_type_enum_type  pdn_pdp_type
 
DEPENDENCIES
  None

RETURN VALUE 
  void
 
SIDE EFFECTS
  PDN PDP type can only be IPV4 or IPV6, It cannot be V4V6 here.

===========================================================================*/

void ds_3gpp_pdn_throt_log_throt_info
(
   ds_3gppi_throttle_sm_s     *throt_sm_ptr,
   byte                       *apn,
   ds_umts_pdp_type_enum_type  pdn_pdp_type
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_CLR_LOG_ENTRY

DESCRIPTION
  This function would delete profile number from pdn_throt_profile_num_list

PARAMETERS
   ds_3gppi_throttle_sm_s*     throt_sm_ptr,
 
DEPENDENCIES
  None

RETURN VALUE 
  void 
 
SIDE EFFECTS

===========================================================================*/

void ds_3gpp_pdn_throt_clr_log_entry
(
   ds_3gppi_throttle_sm_s     *throt_sm_ptr
);


/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_GET_PROFILE_NUMBER

DESCRIPTION
  This function would get profile number to pdn_throt_profile_num_list

PARAMETERS
   ds_3gppi_throttle_sm_s*     throt_sm_ptr,
 
DEPENDENCIES
  None

RETURN VALUE 
  uint16                       profile_number
 
SIDE EFFECTS

===========================================================================*/

uint16 ds_3gpp_pdn_throt_get_profile_number
(
   ds_3gppi_throttle_sm_s     *throt_sm_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_FILL_IDENT_INFO

DESCRIPTION
  This function fills the information used to identify the correct 
  PDN Throttle SM that needs to be allocated / fetched

PARAMETERS
  *throt_ident_info_p : Structure to be filled.
  *apn                : APN Name
  sm_type             : PDN Throttle SM Type
  subs_id             : Subscription Id
  globally_blocked    : Globally Blocked or Not
  rat_type            : RAT to be throttled
  
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_fill_ident_info
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p,
  byte                              *apn,
  sys_modem_as_id_e_type             subs_id,
  boolean                            globally_blocked,
  ds_3gpp_throt_rat_e_type           rat_type,
  ds_3gpp_pdn_throt_proc_e_type      proc_type
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_FILL_IDENT_INFO_EX

DESCRIPTION
  This function fills the information used to identify the correct 
  PDN Throttle SM that needs to be allocated / fetched

PARAMETERS
  *throt_ident_info_p : Structure to be filled.
  *apn                : APN Name
  sm_type             : PDN Throttle SM Type
  subs_id             : Subscription Id
  globally_blocked    : Globally Blocked or Not
  rat_type            : RAT to be throttled
  
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_fill_ident_info_ex
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p,
  byte                              *apn,
  sys_modem_as_id_e_type             subs_id,
  boolean                            globally_blocked,
  ds_3gpp_throt_rat_e_type           rat_type,
  ds_3gpp_pdn_throt_proc_e_type      proc_type,
  ds_3gpp_pdn_throttle_sm_type       sm_type,
  ds_umts_pdp_type_enum_type         pdp_type,
  boolean                            block_all_eplmns,
  ps_iface_net_down_reason_type      cause_code,
  ds_pdn_context_s                  *pdn_cntx_p,
  ds_3gpp_throttle_sm_cb_data_type  *cb_data_p
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_GET_NEXT_RAT_TYPE

DESCRIPTION
  This function decodes the RAT Mask and returns a RAT Type. The RAT Mask
  might have multiple bits set. The function will return the first 
  RAT Type it decodes and will clear the bit correponding to that RAT Type. 
  To fetch all the RAT Types that are set in the mask, the caller should 
  invoke this function in a loop, until RAT Mask becomes
  DS_3GPP_THROT_RAT_MASK_NONE.

PARAMETERS
  *rat_mask_p : RAT Mask to be decoded

DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  The bit corresponding to the RAT Type being returned will be 
  cleared from the RAT Mask
===========================================================================*/
ds_3gpp_throt_rat_e_type ds_3gpp_pdn_throt_get_next_rat_type
(
  uint32  *rat_mask_p
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_GET_NEXT_PROC_TYPE

DESCRIPTION
  This function decodes the PROC Mask and returns a PROC Type. The PROC Mask
  might have multiple bits set. The function will return the first 
  PROC Type it decodes and will clear the bit correponding to that PROC Type. 
  To fetch all the PROC Types that are set in the mask, the caller should 
  invoke this function in a loop, until PROC Mask becomes
  DS_3GPP_THROT_PROC_MASK_NONE.

PARAMETERS
  *proc_mask_p : PROC Mask to be decoded

DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  The bit corresponding to the PROC Type being returned will be 
  cleared from the PROC Mask
===========================================================================*/
ds_3gpp_pdn_throt_proc_e_type ds_3gpp_pdn_throt_get_next_proc_type
(
  uint32  *proc_mask_p
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_UNTHROTTLE_AND_CLEAR_USER_DATA

DESCRIPTION
  This function clears throttling info for a pdn context and a given ip
  type and cleans up the user data 

PARAMETERS   :
   ds_3gpp_pdn_throt_rat_based_s  *rat_based_pdn_throt_p
   throt_user_data_p               Throttle user data
   pdn_pdp_type                    PDP Type
   sys_modem_as_id_e_type          subs_id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_unthrottle_and_clear_user_data
(
  ds_3gpp_pdn_throt_rat_based_s  *rat_based_pdn_throt_p,
  ds_3gpp_throt_user_data        *throt_user_data_p,
  ds_umts_pdp_type_enum_type      pdn_pdp_type,
  sys_modem_as_id_e_type          subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_DETERMINE_MODE

DESCRIPTION
  This function determines the mode for which throttling information
  needs to be published.

PARAMETERS
   subs_id: Subscription Id            
 
DEPENDENCIES
  None.

RETURN VALUE 
  Network Mode 

SIDE EFFECTS
  None.

===========================================================================*/
sys_sys_mode_e_type ds_3gpp_pdn_throt_determine_mode
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_THROTTLING_ON_CONDITION

DESCRIPTION
  This function clears THROTTLING for a particular APN/PDN.
  The is_throttled flag is set to FALSE and the Timer value is cleared.

PARAMETERS: 
  reset_type : The type of reset for which the throttlin is to be claered
  subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  This should not be called to clear T3396 timer throttling

===========================================================================*/
void ds_3gpp_pdn_throt_clear_throttling_on_condition
(
  reset_condition_enum_type                 reset_type,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SET_GLOBAL_THROT_MASK

DESCRIPTION
  This function sets the global throttle mask corresponding to the APN. 
  Possible values for the mask are defined in ds_3gpp_pdn_throt_global_mask_type.

PARAMETERS: 
  apn
  subs_id
  mask
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  This function will trigger global throttling on the given APN
===========================================================================*/
void ds_3gpp_pdn_throt_set_global_throt_mask
(
  byte                        *apn,
  sys_modem_as_id_e_type       subs_id,
  uint32                       mask
); 

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_RESET_GLOBAL_THROT_MASK

DESCRIPTION
  This function resets the global throttle mask corresponding to the APN. 

PARAMETERS: 
  apn
  subs_id
  mask
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
===========================================================================*/
void ds_3gpp_pdn_throt_reset_global_throt_mask
(
  byte                        *apn,
  sys_modem_as_id_e_type       subs_id,
  uint32                       mask
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_GLOBAL_THROT_MASK

DESCRIPTION
  This function gets the global throttle mask corresponding to the APN. 
  Possible values for the mask are defined in ds_3gpp_pdn_throt_global_mask_type.

PARAMETERS: 
  apn
  subs_id
  mask
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  This function will trigger global throttling on the given APN
===========================================================================*/
uint32 ds_3gpp_pdn_throt_get_global_throt_mask
(
  byte                        *apn,
  sys_modem_as_id_e_type       subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_FEED_FAILURE

DESCRIPTION
  This function is used to feed failure into the PDN Throttle SM

PARAMETERS:
  rat_based_pdn_throt_p: RAT based PDN Throttle Ptr
  throt_ident_info_p:    Throttle Information Ptr
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
===========================================================================*/
void ds_3gpp_pdn_throt_sm_feed_failure
(
  ds_3gpp_pdn_throt_rat_based_s     *rat_based_pdn_throt_p,
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p
);


/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_ADVERTISE_BLOCKED_APNS_TO_MPPM

DESCRIPTION
  This function sends the currently blocked list of of APNs to MPPM
  
PARAMETERS 
  void

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_sm_advertise_blocked_apns_to_mppm
(
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_ADVERTISE_BLOCKED_APNS_TO_EXTERNAL_CLIENTS

DESCRIPTION
  This function sends the currently blocked list of of APNs to
  external clients
  
PARAMETERS 
  subs_id: Susbcription Id

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_sm_advertise_blocked_apns_to_external_clients
(
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_SYS_MODE_TO_RAT_MASK

DESCRIPTION
  This function returns the RAT Mask corresponding to the given CM Sys Mode.
 
PARAMETERS  
  mode: CM Sys Mode
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  RAT Mask
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
uint32 ds_3gpp_pdn_throt_sm_sys_mode_to_rat_mask
(
  sys_sys_mode_e_type mode
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_SYS_MODE_TO_RAT_TYPE

DESCRIPTION
  This function returns the RAT Type corresponding to the given CM Sys Mode.
 
PARAMETERS  
  mode: CM Sys Mode
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  RAT Type
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_3gpp_throt_rat_e_type ds_3gpp_pdn_throt_sm_sys_mode_to_rat_type
(
 sys_sys_mode_e_type sys_mode
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_APN_BASED_TBL
 
DESCRIPTION
  This function fetches the APN Based Tbl for a particular subscription.
  
PARAMETERS   : 
  1. Subs Id
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the APN based Tbl Info 
 
SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_pdn_throttle_sm_apn_based_s* ds_3gpp_pdn_throt_get_apn_based_tbl
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_GET_TIMER_VAL_CB
 
DESCRIPTION
  This is the basic Get Timer Value Callback that different 
  throttle modules can use to fetch the throttle timer value. For more 
  specialized cases of PDN Throttling, the throttling module should define 
  its own timer value callback.
  
PARAMETERS   : 
  1. Subs Id
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the APN based Tbl Info 
 
SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_3gpp_pdn_throt_get_timer_val_cb
(
  void* throt_sm_void_ptr,
  void* cb_data
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_POWER_UP_INIT
 
DESCRIPTION
  Performs power up initialization of the main PDN Throttling Module. Kick starts
  the power up initialization of all PDN Throttling modules such as Data Retry, DAM,
  Global throttling and so on.
  
PARAMETERS   : 
  
DEPENDENCIES
  None.

RETURN VALUE
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_power_up_init
(
  void
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_PROCESS_CMDS

DESCRIPTION
  This function processes the commands sent to Throttle SM.

PARAMETERS  
  CMD Ptr
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_pdn_throt_sm_process_cmds
(
  const ds_cmd_type *cmd_ptr 
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_VALIDATE_AND_UNBLOCK_PLMN

DESCRIPTION
  This function checks if the passed plmn id is global or not and unblocks
  the plmn accordingly. For global plmn id all blocked plmn's are unblocked

PARAMETERS   : 
  PLMN ID 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_validate_and_unblock_plmn
(
   sys_plmn_id_s_type             plmn_id,
   sys_modem_as_id_e_type         subs_id,
   boolean                        unblock_all_plmns  
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLR_ON_PLMN_CHANGE

DESCRIPTION
  This Handler is triggered while PLMN  changes. this is to reset the 
  all forms of throttling

PARAMETERS
  subs_id     - Subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_clr_on_plmn_change
(
  sys_sys_mode_e_type     network_mode,  
  sys_plmn_id_s_type      current_plmn_id,
  sys_modem_as_id_e_type  subs_id
);

/*=======================================================================
FUNCTION DS_3GPP_THROT_HANDLE_CONN_FAILURE

DESCRIPTION
  This function handles connection failure for a PDN context.

PARAMETERS   :
   *throt_ident_info_p        - Information used to identify the type
                                of throttling
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_handle_conn_failure
(
  ds_3gpp_pdn_throt_ident_info_type *throt_ident_info_p
);

uint32 ds_3gpp_pdn_throt_convert_proc_type_to_proc_mask
(
   ds_3gpp_pdn_throt_proc_e_type proc_type
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_CONVERT_RAT_TO_RAT_MASK

DESCRIPTION
  This function is used to fetch the RAT Mask bit corresponding
  to the given RAT Type  

PARAMETERS: 
  RAT Type
 
DEPENDENCIES
  None

RETURN VALUE
  RAT Mask
 
SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_3gpp_pdn_throt_convert_rat_to_rat_mask
(
   ds_3gpp_throt_rat_e_type temp_rat_mask
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_CHECK_IF_THROT_PER_PLMN

DESCRIPTION
  This function checks whether Spec Throttling needs to be performed Per PLMN 

PARAMETERS: 
  rat_based_pdn_throt pointer 
  down_reason - PS Net Down Reason
  subs_id     - Subs Id
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE, if Spec Throttling should be per PLMN
  FALSE, otherwise
 
SIDE EFFECTS
  None
===========================================================================*/
boolean ds_3gpp_pdn_throt_check_if_throt_per_plmn
(
  ds_3gpp_pdn_throt_rat_based_s  *rat_based_pdn_throt_p,
  ps_iface_net_down_reason_type  down_reason,
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEANUP_APN_INDEX_ENTRY_IF_NEEDED

DESCRIPTION
  This function cleans up APN Index Entry if no throttling is active for
  that APN.

PARAMETERS   
   APN index ptr
   Subscription Id
   advertise_blocked_apn - If set to TRUE, blocked apn's for the current plmn
                           will be advertised even if the apn_entry is not
                           cleaned up
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_cleanup_apn_index_entry_if_needed
(
  void                    *apn_index_ptr,
  boolean                  advertise_blocked_apn,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_THROTTLE_INFO

DESCRIPTION
  This function clears throttling info for a pdn context and a given ip
  type.

PARAMETERS   :
   ds_pdn_context_s * - pdn context ptr
   pdn_pdp_type       - PDP Type

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pdn_throt_clear_throttle_info
(
  ds_3gpp_pdn_throt_rat_based_s  *rat_based_pdn_throt_p,
  ds_umts_pdp_type_enum_type      pdn_pdp_type,
  sys_modem_as_id_e_type          subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_ADVERTISE_BLOCKED_APNS_TO_MPPM

DESCRIPTION
  This function sends the currently blocked list of of APNs to MPPM
  
PARAMETERS 
  void

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_throt_sm_advertise_blocked_apns_to_mppm_on_plmn
(
  sys_modem_as_id_e_type           subs_id,
  sys_plmn_id_s_type               plmn_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_ENABLE_LTE_PLMN_IF_ATTACH_PROF_AVAILABLE

DESCRIPTION
  This function enables LTE PLMN if attach profile available
  
PARAMETERS 
  subs id
  PLMN Id

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_enable_lte_plmn_if_attach_prof_available
(
  sys_modem_as_id_e_type           subs_id,
  sys_plmn_id_s_type               plmn_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_SM_GET_APN_BASED_TBL_INDEX

DESCRIPTION
  This function retrieves the APN based table index for a specific APN.

PARAMETERS   :
   apn    - the apn for which the APN table index is to be retrieved.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
ds_3gpp_pdn_throttle_sm_apn_based_dyn_s* 
ds_3gpp_pdn_throt_sm_get_apn_based_tbl_index
(
  byte                          *apn,
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_DEALLOC_PER_RAT_SM

DESCRIPTION
  This function frees per rat throttling info and sets the init value to FALSE

PARAMETERS   :
   *rat_based_pdn_throt_p      - TCM ptr
   *pdn_throt_sm_info_ptr      - Parent of TCM Ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_pdn_throt_dealloc_per_rat_sm
(
  ds_3gpp_pdn_throt_rat_based_s       *rat_based_pdn_throt_p,
  void                                *proc_based_info_p,
  sys_modem_as_id_e_type               subs_id   
);


/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_STORE_PREV_ADV_BLOCKED_APNS_TO_CM_APM

DESCRIPTION
  This function stores the previously advertized blocked apn's to CM
  
PARAMETERS 

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_throt_sm_store_prev_adv_blocked_apns_to_cm
(
  ds_3gpp_pdn_throt_apn_list_type *blocked_apn_list,
  sys_modem_as_id_e_type           subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_CMP_PREV_ADV_BLOCKED_APNS_TO_CM_APM

DESCRIPTION
  This function compares the previously advertized blocked apn's to cm/apm
  TRUE   - If the previously advertized apn's are the same
  FALSE  - If the previously advertized apn's are not the same
 
  
PARAMETERS 

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_throt_sm_cmp_prev_adv_blocked_apns_to_cm
(
  ds_3gpp_pdn_throt_apn_list_type *blocked_apn_list,
  sys_modem_as_id_e_type           subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_CLR_PREV_ADV_BLOCKED_APNS

DESCRIPTION
This function clears previously advertised blocked apn's
 
  
PARAMETERS 

DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_throt_sm_clr_prev_adv_blocked_apns
(
  sys_modem_as_id_e_type           subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_THROT_CONVERT_RAT_MASK_TO_RAT

DESCRIPTION
  This function is used to fetch the RAT from RAT mask
 
PARAMETERS: 
  RAT Type
 
DEPENDENCIES
  None

RETURN VALUE
  RAT Mask
 
SIDE EFFECTS
  None
===========================================================================*/

ds_3gpp_throt_rat_e_type ds_3gpp_pdn_throt_convert_rat_mask_to_rat
(
   uint32 rat_mask
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_GET_EPLMN_LIST_PER_PLMN

DESCRIPTION
  API used to get the list of throttled ePLMNs on a particular RAT for a
  PLMN ID that is passed to this function. 

PARAMETERS  
  eplmn_list_ptr - Pointer to the ePLMN list 
  rat_type       - Enum that indicates the RAT on which ePLMN list info
                   is needed.
                   
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns a pointer to the ePLMN list in a particular RAT
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
sys_plmn_list_s_type* ds_3gppi_throt_get_eplmn_list_per_plmn
(
  sys_plmn_id_s_type       plmn_id,
  ds_3gpp_throt_rat_e_type rat_type,
  sys_modem_as_id_e_type       subs_id
);

/*===========================================================================
FUNCTION    DS_3GPP_CHECK_THROT_SM_IF_PLMN_PRESENT_IN_EPLMN_LIST

DESCRIPTION
   This function checks if a particular PLMN ID is present in the ePLMN
   list of the PLMN based throttle SM entry passed to this function. 
 
  
PARAMETERS 
  plmn_id                  - pointer to the Current PLMN ID
  plmn_based_pdn_throt_ptr - Pointer to the PLMN based throt SM
  subs_id                  - Subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE 
  TRUE  - If PLMN present in the ePLMN list
  FALSE - Otherwise
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_check_throt_sm_if_plmn_present_in_eplmn_list
(
  sys_plmn_id_s_type                  *plmn_id, 
  ds_3gpp_pdn_throt_sm_plmn_based_s   *plmn_based_pdn_throt_ptr,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_CREATE_NEW_EPLMN_ENTRY

DESCRIPTION
  This function takes care of creating and/or updating the ePLMN list
  for a particular PLMN throttle SM table when network rejects the
  PDN connection. 
  This function will be called only when there are some ePLMNs that
  need to be throttled. 
 
PARAMETERS 
  *eplmn_list_p          - Pointer to the list of ePLMNs if already
                           present
  *rat_based_pdn_throt_p - Pointer to the RAT on which ePLMNs
                           are getting throttled
  *plmn_ptr              - Pointer to the PLMN that is being throttled
  throt_all_eplmns       - Indicates if the PDN reject included EPLMN
                           bit was set to 1
  subs_id                - Subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_throt_create_new_eplmn_entry
(  
  sys_plmn_list_s_type              *eplmn_list_p,
  void                              *rat_based_pdn_throt_p,
  sys_plmn_id_s_type                *plmn_ptr,
  boolean                            throt_all_eplmns,
  ds_3gpp_pdn_throt_type_e           throt_tbl_type,
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_IS_PLMN_EPLMN

DESCRIPTION
  This function is used to check if a given PLMN is in the ePLMN list
  of the current camped PLMN. 

PARAMETERS 
  plmn_id                  - Current PLMN which is getting throttled
  throttled_plmn_id_p      - Pointer to the parent PLMN ID in the throt SM
  subs_id                  - Subscription ID
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - if PLMN is in the ePLMN list
  FALSE - Otherwise
   
SIDE EFFECTS 
  None.
===========================================================================*/
boolean ds_3gppi_throt_is_plmn_eplmn
(
  sys_plmn_id_s_type                plmn_id,
  sys_plmn_id_s_type                throttled_plmn_id_p,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_DETACH_INFO_CB

DESCRIPTION
  This function takes care of handling all PDN throttling related
  functionality when detach is triggered.

PARAMETERS 
  sys_mode   -- System mode on which detach is triggered
  subs_id    -- Subscription ID
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_pdn_throt_detach_info_cb
(
  sys_sys_mode_e_type         sys_mode,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_MGR_READ_IMS_SETTINGS_INFO

DESCRIPTION
  This function reads the IMS settings info again and sets/unsets the
  throttling
  
PARAMETERS: 
  subs_id  -- subscription id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_throt_mgr_read_ims_settings_info
(
  sys_modem_as_id_e_type       subs_id
);

#endif /* DS_3GPP_PDN_THROTTLE_SM_H */
