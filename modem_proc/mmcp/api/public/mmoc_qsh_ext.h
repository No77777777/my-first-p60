#ifndef MMOC_QSH_EXT_H
#define MMOC_QSH_EXT_H
/*===========================================================================

                    M U L T I M O D E   C O N T R O L L E R ( MMoC )   D E B U G   H E A D E R   F I L E

DESCRIPTION
  This header file contains debug macros and definitions necessary to
  interface with mmocdbg.C


Copyright (c) 1991 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/mmoc_qsh_ext.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/15   KC      Initial release.

===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "qsh.h"
#include "sys.h"
#include "mmode_internal.h"

#define MMOC_QSH_MAJOR_VER 1

#define MMOC_QSH_MINOR_VER 1


/* Enumeration for the type of Dump */
typedef enum {
  MMOC_QSH_DUMP_TAG_MINI, /* Denotes Mini dump */
  MMOC_QSH_DUMP_TAG_MAX
}mmoc_qsh_dump_tag_e;


/* this is evolved from MMOC_MAX_DEBUG_BUFFER_SIZE  */
#define MMOC_DBG_MAX_DEBUG_BUFFER_SIZE         10

/* this is evolved from MMOC_MAX_RPT_COUNT  */
#define MMOC_DBG_MAX_RPT_COUNT                 10

/* this is evolved from MMOC_MAX_TASK_NAME_LEN  */
#define MMOC_DBG_MAX_TASK_NAME_LEN             10

/* this is evolved from MMOC_DBG_BUF_TRANS_ADDL_INFO_SIZE  */
#define MMOC_DBG_BUF_DEBUG_TRANS_ADDL_INFO_SIZE  8

/* this is evolved from SD_SS_MAX  */
#define MMOC_DBG_SS_MAX 4

/* this is evolved from MAX_AS_IDS  */
#define MMOC_DBG_MAX_AS_IDS 2

/* this is evolved from SYS_MAX_ACQ_PREF_RAT_LIST_NUM */
#define MMOC_DBG_MAX_ACQ_PREF_RAT_LIST_NUM 10
    /**< Maximum number of acquisition lists. */

/* Enumeration for the type of SUBS */
typedef enum {
  MMOC_DBG_SUBS_NONE,
  MMOC_DBG_SUBS_3GPP,
  MMOC_DBG_SUBS_3GPP2,
  MMOC_DBG_SUBS_MAX
}mmoc_subs_data_type_e;

typedef PACK(struct) {

  int8    asubs_id;

  int8    stk_id;

} mm_dbg_sub_stk_id_s_type;



/* this structure is evolved from mmoc_cmd_deact_from_dormant_s_type  */
typedef PACK(struct)
{
  /* Indicates the subs/stack for which protocol has to be deactivated if dormant */
  mm_dbg_sub_stk_id_s_type        mm_id;

  /* Indicates the modes (protocols) which has to be deactivated */
  uint16             prot;

  /* Boolean flag to indicate if SS_MAIN needs to be
  ** activated at the end of transaction. */
  boolean                            is_activate_main;
  
}mmoc_dbg_cmd_deact_from_dormant_s_type;


/* this structure is evolved from sys_addtl_action_s_type  */
typedef PACK(struct)
{
  /* Addtional action to MMOC */
  uint8 action;


  int8 ue_mode;
  /* additional payload */
   /* sys_ue_mode_e_type */

  boolean is_ue_mode_substate_srlte;
  /* to remember if we are operation in SRLTE mode or not */
}mmoc_dbg_addtl_action_s_type;


/* this structure is evolved from sd_rat_acq_order_s_type */
typedef PACK(struct)
{
  int8 acq_sys_mode[MMOC_DBG_MAX_ACQ_PREF_RAT_LIST_NUM];
  /* Version field can be used to distinguish between different TOT
  ** tables. It is not used in code for any purpose
  */
   /* sys_sys_mode_e_type */

  uint16 version;
  /* The number of RATs that are part of the technology order table
  ** This should be equal to the number of cellular technologies
  ** supported by the phone
  */


  uint16 num_rat;
  /* This array will list the cellular technologies in order of priority
  */
}mmoc_dbg_rat_acq_order_s_type;


/* this structure is evolved from sd_ho_sys_param_s_type */
typedef PACK(struct)
{

  int8            band;
  /* sd_band_e_type */
  /**< Serving system's band class.
      */

  uint16              chan;
  /* sys_channel_num_type */
  /**< Serving system's CDMA channel.
      */

  word                      sid;
      /**< Serving system's SID.
      */

  word                      nid;
      /**< Serving system's NID.
      */

  word                      mcc;
      /**< Serving system's MCC.
      */
  byte                      imsi_11_12;
      /**< Serving system's IMISI_11_12.
      */

  byte         srch_win_n;
  /* sys_srch_win_type */
  /**< Neighbor set window search size.
      */

  int32         base_lat;
  /* sys_base_lat_type */
      /**< Base station latitude.
      */

  int32        base_long;
  /* sys_base_long_type */
      /**< Base station latitude.
      */

  word          base_id;
  /* sys_base_id_type */
      /**< Base station Id.
      */

  byte                      ltm_off;
      /* LTM Offset
      */

  int8  daylt_savings;
  /* sys_daylt_savings_e_type */
  /* Daylight savings ind
      */

  byte                      leap_secs;
      /* Leap seconds
      */

}mmoc_dbg_ho_sys_param_s_type;


/* this structure is evolved from mmoc_cmd_prot_ho_ind_s_type */
typedef PACK(struct)
{
  int8         asubs_id;
  
  /* Reason for the protocol HO.
  */
  uint8                      actd_reason;

  /* Protocol which sent the autonomous activation for redirection.
  */
  uint8                    prot_state;

  mmoc_dbg_ho_sys_param_s_type               sys_param;
  
}mmoc_dbg_cmd_prot_ho_ind_s_type;


/* this structure is evolved from mmoc_ho_ind_trans_info_s_type */
typedef PACK(struct)
{
  /* Subs/stack on which Redirected protocol is activated */
  mm_dbg_sub_stk_id_s_type         mm_id;

  /* Protocol state of System Selection instance prior to redirection */
  uint8                   prev_prot_state;

  /* Protocol HO command received */
  mmoc_dbg_cmd_prot_ho_ind_s_type         cmd_info;
  
}mmoc_dbg_ho_ind_trans_info_s_type;



/* this structure is evolved from prot_cmd_pref_sys_chgd_s_type */
typedef PACK(struct)
{
  /* Reason for changing the SS-Preference.  */
  uint8               pref_reas;

  /* New origination mode (normal, OTASP, emergency, other, etc.).
  */
  uint8               orig_mode;

  /* New mode preference (AMPS, CDMA, any, etc.) - ignored when
  ** orig_mode = OTASP or emergency.
  */
  uint16               mode_pref;

  /* New band preference (BC0, BC1, any, etc.) - ignored when
  ** orig_mode = OTASP  or emergency.
  */
  uint64               band_pref;

  /* New LTE band preference - ignored when
  ** orig_mode = OTASP  or emergency.
  */
  sys_lte_band_mask_e_type             lte_band_pref;

  /*
  ** TDS-CDMA band preference
  */
  uint64               tds_band_pref;

  /* New prl preference.
  */
  uint16                prl_pref;

  /* New roam preference (any, home, affiliated, etc.) - ignored
  ** when orig_mode = OTASP or emergency.
  */
  uint16               roam_pref;

  /* New hybrid preference (none, CDMA/HDR).
  */
  uint8               hybr_pref;

  /* The band-class that is associated with OTASP origination mode.
  ** Note that this parameter is ignored if orig_mode != OTASP.
  */
  uint16                       otasp_band;

  /* The PCS frequency block/Cellular system that is associated
  ** with OTASP origination mode. Note that this parameter is
  ** ignored if orig_mode != OTASP.
  */
  uint8                     otasp_blksys;

  /* Type of the system to be avoided, valid only if
  ** pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */
  uint8               avoid_type;

  /* Time in seconds for which the system is to be avoided,
  ** valid only if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */
  dword                                avoid_time;

  /* New service domain preference (CS, PS, CS_PS etc).
  ** Applicable for GSM/WCDMA modes.
  */
  uint8         domain_pref;

  /* Preference for the order of acquisition between modes.
  ** ( WCDMA before GSM, GSM before WCDMA etc ).
  */
  uint8          acq_order_pref;

  /* Indicate ss pref update reason
  */
  uint8      pref_update_reas;

  /*  To indicate sub action to be performed by MMOC during
  **  pref sys chgd cmd
  */
  mmoc_dbg_addtl_action_s_type               addl_action;

  /* The user/phone mode preference
  */
  uint16                user_mode_pref;

  /* Rat acquisition preference order including LTE
  */
  mmoc_dbg_rat_acq_order_s_type               rat_acq_order;

  /* Request Id
  */
  uint16                                sys_sel_pref_req_id;

  /* Indicate ss pref camp mode
  */
  int8           camp_mode_pref;

  /* CSG identifier
  */
  uint32                       csg_id;

  /* RAT specified for CSG
  */
  int8          csg_rat;

  int8          voice_domain_pref;
  /* voice domain pref 
  */
    
}mmoc_dbg_prot_cmd_pref_sys_chgd_s_type;


/* this structure is evolved from prot_cmd_gw_get_net_s_type */
typedef PACK(struct)
{
  /* Mode preference for the Get networks command. Can be GSM_ONLY,
  ** WCDMA_ONLY or GW_ONLY only.
  */
  uint16               mode_pref;


  /* Band preference that is associated with the Get networks
  ** command.
  */
  uint64               band_pref;

  /* Band preference that is associated with the Get networks
  ** command for LTE
  */
  sys_lte_band_mask_e_type             lte_band_pref;

  /* Band preference that is associated with the Get networks
  ** command for TD-SCDMA
  */
  uint64               tds_band_pref;

  /* Network list type for Get networks command
  */
  int8         network_list_type;

  /*Ignore MANUAL BST Band Restriction
   */
  boolean                             ignore_manual_bst;

  boolean                             is_requeue;

}mmoc_dbg_prot_cmd_gw_get_net_s_type;



/* this structure is evolved from prot_cmd_msm_sys_chgd_s_type */
typedef PACK(struct)
{
  int8 band;
   /* sys_band_class_e_type */

  uint16 chan;
   /* sys_channel_num_type */

  uint16 sid;


  uint16 nid;
}mmoc_dbg_prot_cmd_msm_sys_chgd_s_type;


/* this structure is evolved from mmoc_subs_data_s_type */
typedef PACK(struct)
{

  int8                     as_id;
  uint8                    active_stacks;
  uint16                   nv_context;
  boolean                  is_perso_locked;
  uint8                    sub_avail;
  uint8                    prot_subsc_chg;
  uint32                   subs_capability;
  uint8                    orig_mode;
  uint16                   mode_pref;
  uint64                   band_pref;
  uint16                   roam_pref;
  sys_lte_band_mask_e_type lte_band_pref;
  uint64                   tds_band_pref;
  boolean                  only_subs_cap_change;

  /* 3GPP preferences */
  uint32   gpp_session_type;
  uint8    acq_order_pref;
  int8     srv_domain_pref;

  /* 3GPP2 preferences */
  uint32   gpp2_session_type;
  uint16   prl_pref;
  uint8    hybr_pref;
  int8     ue_mode;
  boolean  is_ue_mode_substate_srlte;

}mmoc_dbg_subs_data_s_type;

/*===========================================================================
**              D E V I C E - L E V E L  P R O P E R T I E S               **
===========================================================================*/

typedef PACK(struct)
{
  uint8        chg_type;
  byte         nam;
  uint8        active_subs;
  int8         device_mode;

} mmoc_dbg_subsc_chgd_dev_prop_s_type;

/*===========================================================================
**              S U B S C - L E V E L  P R O P E R T I E S                 **
===========================================================================*/

typedef mmoc_dbg_subs_data_s_type mmoc_dbg_subsc_chgd_sub_prop_s_type;



/* this structure is evolved from mmoc_cmd_subsc_chgd_s_type */
typedef PACK(struct)
{
  mmoc_dbg_subsc_chgd_dev_prop_s_type      dev_prop;
  mmoc_dbg_subsc_chgd_sub_prop_s_type      sub_prop[MAX_SIMS];
  byte                                     nSubs;

} mmoc_dbg_cmd_subsc_chgd_s_type;

typedef union
{

  mmoc_dbg_prot_cmd_pref_sys_chgd_s_type        pref_sys_chgd;

  mmoc_dbg_prot_cmd_gw_get_net_s_type           gw_get_net;

  mmoc_dbg_prot_cmd_msm_sys_chgd_s_type         msm_sys_chgd;

  int8          gw_sim_state;

} mmoc_dbg_prot_gen_cmd_param_u_type;


/* this structure is evolved from prot_gen_cmd_s_type */
typedef PACK(struct)
{
  /* Generic command type (reselection timeout, get networks etc). */
  uint8                    cmd_type;

  /* Transaction Id for the generic command.
  ** Note! trans_id is used only when communicating with the protocols.
  ** Invalid when it is in the MMoC's command queue. */
  byte                     trans_id;

  /* Generic command to this SD SS Instance. */
  int8                     asubs_id;
  int8                     stk_id;

  /* MMoC's current protocol state for ss. */
  uint8                    prot_state;

  /* Parameters associated with the above ss command type. */
  mmoc_dbg_prot_gen_cmd_param_u_type param;

}mmoc_dbg_prot_gen_cmd_s_type;


/* this structure is evolved from mmoc_gen_cmd_trans_info_s_type */
typedef PACK(struct)
{
  /* Boolean flag to indicate if SS_MAIN needs to be
  ** activated at the end of transaction. */
  boolean                             is_activate_main;

  /* Info for protocol generic command transaction. */
  mmoc_dbg_prot_gen_cmd_s_type                 cmd_info;
  
} mmoc_dbg_gen_cmd_trans_info_s_type;


/* A type for parameters that are associated with protocol redirection
** command, coming from protocol
*/
typedef PACK(struct) {

  int8               asubs_id;
  
  /* Reason for the protocol redirection.
  */
  uint8                      actd_reason;

  /* Protocol which sent the autonomous activation for redirection.
  */
  uint8                    prot_state;

} mmoc_dbg_cmd_prot_redir_ind_s_type;


/* A type for parameters that holds the information regarding the transaction
** MMOC_TRANS_PROT_REDIR_IND being processed.
*/
typedef PACK(struct) {

  /* Subs/stack on which Redirected protocol is activated */
  mm_dbg_sub_stk_id_s_type         mm_id;

  /* Protocol state of System Selection instance prior to redirection */
  uint8                   prev_prot_state;

  /* Protocol redirection command received */
  mmoc_dbg_cmd_prot_redir_ind_s_type      cmd_info;

} mmoc_dbg_redir_ind_trans_info_s_type;


/* A type for parameters that are associated with suspend ss
** command to MMoC task.
*/
typedef PACK(struct) {

  /* Indicates the instance to be suspended. */
  mm_dbg_sub_stk_id_s_type          mm_id;

  /* Indicates whether to suspend or resume */
  boolean                              is_suspend;

  /* Indicates whether to activate protocol or not */
  boolean                              ignore_protocol_activate;

  /* Indicates suspension reason */
  uint8                    susp_reason;

} mmoc_dbg_cmd_suspend_stack_s_type;


/* this structure is evolved from mmoc_trans_info_u_type */
typedef PACK(union)
{
  mmoc_dbg_gen_cmd_trans_info_s_type     gen_prot_cmd;

  mmoc_dbg_ho_ind_trans_info_s_type      ho_ind_info;

  mmoc_dbg_cmd_deact_from_dormant_s_type deact_from_dormant;

  mmoc_dbg_redir_ind_trans_info_s_type   redir_ind_info;

  mmoc_dbg_cmd_suspend_stack_s_type      suspend_stack;

}mmoc_dbg_trans_info_u_type;


/*===========================================================================
**              D E V I C E - L E V E L  P R O P E R T I E S               **
===========================================================================*/

typedef PACK(struct) {

  /* Current transaction that the MMoC is processing. Set to MMOC_TRANS_NULL
  ** if no transactions are being processed.
  */
  uint8                             trans;
  
  /* Current state of the transaction that the MMoC is processing.
  */
  uint8                             trans_state;

  /* Information related to the transaction being processed. Valid only
  ** when curr_trans != MMOC_TRANS_NULL.
  */
  mmoc_dbg_trans_info_u_type        trans_info;

  /* Info for subscription changed transaction. */
  mmoc_dbg_cmd_subsc_chgd_s_type    subsc_chgd;
  
  /* Flag to indicate if SD was initialized with subscription
  ** information.
  */
  boolean                           is_sd_initialized;

  /* Flag to indicate if sd_init is called after starting of subs transaction
  */
  boolean                           is_sd_init_called;

  /* Last trans Id for the command issued by MMoC. Incremented after
  ** sending every acknowledgeable command.
  ** Cannot be set to MMOC_TRANS_AUTONOMOUS or MMOC_TRANS_PENDING.
  */
  byte                              trans_id;
  
  /* Current operating mode status of the phone.
  */
  uint8                             oprt_mode;

  /* Flag to indicate if RESET_MODEM cmd is being processed.
  */
  int8                              true_oprt_mode;

  /* Indicates if phone status chgd command to all supported protocols
  ** have already been sent.
  */
  boolean                           is_ph_stat_sent;

  /* Context for current protocol to which the deactivate request was
  ** sent.
  */
  mm_dbg_sub_stk_id_s_type          deact_req_idx;


  /* Insanity count to sanity check if the MMoC is stuck in a transaction.
  */
  byte                              insanity_count;

  /* Flag to indicate if phone is in standby sleep.
  */
  boolean                           is_standby_sleep;
  
  /* Flag to indicate if redirection is allowed
  */
  boolean                           is_redir_allowed;

  /* Previous Standby Preference - Dual standby/Single standby
  */
  uint8                             prev_standby_pref;

  /* Current Standby Preference - Dual standby/Single standby
  */
  uint8                             standby_pref;
  
  /* Previous active subs, this needs to be in sync with standby pref
  */
  uint8                             prev_active_subs;

  /* Current active subs, this needs to be in sync with standby pref
  */
  uint8                             active_subs;
  
  /* Current device mode */
  int8                              device_mode;

  /* Previous device mode */
  int8                              prev_device_mode;

  /* Sanity timer value depending on the standby preference
  */
  uint8                             max_sanity_time_multiple;

  /* Scan permission allow MDM/MSM to scan. This is used in
  ** SVLTE2
  */
  boolean                           is_scan_permission;
  

  /* Indicates whether AUTO DEACT IND is buffered or not */
  boolean                           is_buffer_auto_deact_ind;

  dword                             last_stop_req_sent_timestamp;

  /* Identifies feature that is enabled in this instance of common build */
  uint8                             onebuild_feature;
  
  /* Bitmask of subs which get deactivated with dual-switch */
  uint8                             dual_switch_subs;
  
  /* Mapping of session to corresponding slot, used for cross mapping, reading MLPL/MSPL */
  uint32                            pri_slot;
  
  /* Indicates if PS sub needs to be activated first */
  boolean                           is_ps_priority;

  /* Indicates which sub is enabled for 3gpp2. For this sub,
  ** Stack 1 is for CDMA
  ** Stack 2 is for HDR
  ** Stack 3 can be for GWL
  */
  int8                              sub_with_3gpp2;

  int8                              prev_sub_with_3gpp2;

  int8                              sub_with_cap_chg;

}mmoc_dbg_state_info_dev_prop_s_type;
  
/*===========================================================================
**              S T A C K - L E V E L  P R O P E R T I E S                 **
===========================================================================*/
  
typedef PACK(struct) {
  
  /* Current protocol state */
  uint8                    prot_state;
  
  /* Current dormant protocols */
  uint16                   prot_dormant;

  /* Indicate if GWL deactivate command has been sent */
  boolean                  is_gwl_deact_sent;

  /* Stack suspend status */
  boolean                  is_suspend;
  
  uint32                   suspend_reas_mask;
  /* Mask to hold Stack suspend reason */
  
  /* Last protocol state */
  uint8                    last_prot_state;
  
} mmoc_dbg_state_info_stack_prop_s_type;

/*===========================================================================
**              S U B S C - L E V E L  P R O P E R T I E S                 **
===========================================================================*/

typedef PACK(struct) {

  /* Current NAM selection.
  */
  byte                                 nam;
  
  /* Current cdma/GSM/WCDMA/LTE subscription availability status. */
  uint8                                sub_avail;

  uint8                                active_stacks;

  uint8                                prev_active_stacks;
  
  /* Enumeration of protocols that have a change in
  ** subscription available status */
  uint8                                prot_subsc_chg;
  
  /* Denotes the stack that needs to be activated on getting deact CNF from
  ** HDR protocol */
  int8                                 hdr_deact_activate_stack;
  
  /* Subscription feature mode */
  uint8                                subs_feature;

  /* PM subscription feature mode */
  uint8                                pm_subs_feature;

  /* Subscription capability */
  uint32                               subs_capability; 

  /* Indicates whether sub is PS enabled */
  boolean                              ps_enabled;
  
  /* Stack properties. Dynamic size per number of stacks */
  mmoc_dbg_state_info_stack_prop_s_type   stack_prop[MAX_STACKS];

  /* Number of stacks in the stack_prop array */
  uint8                                nStacks;

}mmoc_dbg_state_info_sub_prop_s_type;


/* A type for parameters that holds the state information of the MMoC task. */
typedef PACK(struct)  {

  /* Device level properties */
  mmoc_dbg_state_info_dev_prop_s_type            dev_prop;

  /* Subscription level properties */
  mmoc_dbg_state_info_sub_prop_s_type            sub_prop[MAX_SIMS];

  /* Number of subscriptions in the sub_prop array */
  uint8                                      nSubs;

} mmoc_dbg_state_info_s_type;


/* this structure is evolved from mmoc_dbg_buf_rpt_s_type */
typedef PACK(struct)
{
  /* Name of the report received */
  uint8                               rpt_name;

  /* Name of the task from where report is queued */
  uint8                               task_name;

  /* Protocol states for each of the stack when report is received */
  uint8                      prot_state[MAX_SIMS][MAX_STACKS];
  
} mmoc_buf_rpt_dbg_s_type;

/* this structure is evolved from mmoc_dbg_buf_trans_s_type */
typedef PACK(struct)
{
  /* Transaction Id associated with the transaction.
  */
   /* prot_trans_type */
  uint8 trans_id;

  /* Set of reports received for the current transaction
  */
  mmoc_buf_rpt_dbg_s_type rpt_queue[MMOC_DBG_MAX_RPT_COUNT];
   /* mmoc_dbg_buf_rpt_s_type */


   /* Additional information stored for certain transaction types
   */
  uint8 addl_info[MMOC_DBG_BUF_DEBUG_TRANS_ADDL_INFO_SIZE];


  /* Transaction name associated with the transaction
  */
  uint8 trans_name;
}mmoc_buf_trans_dbg_s_type;

/* this structure is evolved from mmoc_debug_buffer_s_type */
typedef PACK(struct)
{
  mmoc_buf_trans_dbg_s_type dbg_buf[MMOC_DBG_MAX_DEBUG_BUFFER_SIZE];
   /* mmoc_dbg_buf_trans_s_type */
  uint8 dbg_buf_idx;
}mmoc_debug_buffer_dbg_s_type;


typedef PACK(struct) mmoc_qsh_dump_tag_mini_s{
 qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */


 /* A type for messages [commands/reports] store in cmregprx debug buffer
 */
 mmoc_debug_buffer_dbg_s_type dbg_buffer;


 /* A type for parameters that holds the state and other information of the
 ** RegProxy.
   */
 mmoc_dbg_state_info_s_type state_info;
}mmoc_qsh_dump_tag_mini_s_type;


/*===========================================================================

FUNCTION mmocdbg_qsh_init

DESCRIPTION
  Initilize CM QSH interface.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  mmocdbg_qsh_init(void);

#endif /* MMOC_QSH_EXT_H */

