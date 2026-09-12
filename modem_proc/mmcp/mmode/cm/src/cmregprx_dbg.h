#ifndef CMREGPRX_DBG_H
#define CMREGPRX_DBG_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C M    R E G P R O X Y   D E B U G   M O D U L E   H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains the debug related functionality of CM REG Proxy.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 1991 - 2014 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmregprx_dbg.h#1 $

===========================================================================*/

/*lint -save -e656 -e641
** Turn off enum to int / operationuses compatible enums
*/

/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/

#include "mmcp_variation.h"
#include "comdef.h"    /* Definition for basic types and macros */
#include "target.h"    /* Target configuration file */
#include "err.h"       /* Interface to error log services */
#include "msg.h"       /* Interface to F3 messages services */

#include "cm.h"        /* External interface to cm.c */
#include "cm_v.h"
#include "cmi.h"
#include "cm_i.h"
#include "cmcall.h"
#include "cmph.h"
#include "cminband.h"
#include "cmss.h"
#include "cmidbg.h"
#include "cmpmprx.h"
#include "mmoc_v.h"
#include "cmmsc.h"

#ifdef CM_DEBUG
#error code not present
#endif

#include "cmregprxi.h"/* Regproxy implementation related declarations. */

#ifdef MMODE_ADDITIONAL_DEBUG_INFO

/**--------------------------------------------------------------------------
** Defines
** --------------------------------------------------------------------------
*/
#define CMREGPRX_DEBUG_BUFFER_SIZE 50

#define CMREGPRX_DBG_MEM_ALLOC( VAR, TYPE) \
  TYPE* VAR = (TYPE *)cm_mem_malloc(sizeof(TYPE));

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

typedef enum cmregprx_buffer_type_e
{
  CMREGPRX_BUFF_TYPE_NONE = -1,
  CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE,
  CMREGPRX_BUFF_RPT_TYPE,
  CMREGPRX_BUFF_MSG_RTR_TYPE,
  CMREGPRX_BUFF_CMD_TO_NAS_TYPE,
  CMREGPRX_BUFF_TYPE_MAX
} cmregprx_buffer_type_e_type;

typedef enum cmregprx_dbg_buff_msg_rtr_e
{
  CMREGPRX_BUFF_MSG_RTR_NONE = -1,
  CMREGPRX_BUFF_MSG_RTR_LTE_RRC_TO_CM_ACTIVATION_IND,
  CMREGPRX_BUFF_MSG_RTR_MAX
} cmregprx_dbg_buff_msg_rtr_e_type;

/* A type for messages [commands/reports] store in cmregprx debug buffer
*/
//GKG - Later check for best packing in struct
typedef struct
{
  dword                               timestamp;

  uint8                               idx;

  /* Current state of CMREGPRX per stack.
  */
  uint8                               cmregprx_state[MAX_SIMS][CMREGPRX_NUM_MAX_STACKS];

  /* Current substate of CMREGPRX per stack.
  */
  uint8                               cmregprx_substate[MAX_SIMS][CMREGPRX_NUM_MAX_STACKS];

  /* sub and stack id */
  mm_sub_stk_id_s_type                mm_id;

  /*Indicated whether message received is a command or report */
  cmregprx_buffer_type_e_type         msg_type;

  union cmregprx_buff_u_type
  {
    prot_cmd_e_type                   cmd_from_mmoc;
    cm_name_type                      rpt;
    cmregprx_dbg_buff_msg_rtr_e_type  msgr_rpt;
    cm_to_reg_cmd_type                cmd_to_nas;
  } msg_name;

  /*Payload of the buffer message*/
  void                               *msg_payload;

} cmregprx_dbg_buffer_message_s_type;

/* A type for cmregprx debug buffer */
typedef struct
{

  cmregprx_dbg_buffer_message_s_type  dbg_buf_msg[CMREGPRX_DEBUG_BUFFER_SIZE];

  /* Index refering to latest filled msg in  dbg_buf[]. Value of 255 (i.e. 0xFF) is reserved for initialization*/
  uint8                            dbg_buf_idx;

  sys_modem_stack_id_e_type        stack_id_for_current_report;

  sys_modem_as_id_e_type           asubs_id_for_current_report;
} cmregprx_dbg_buffer_s_type;

// Debug Data Structures for MMOC Cmds

typedef struct
{
  prot_gen_cmd_e_type                  cmd_type;
  sd_ss_mode_pref_e_type               mode_pref;
  sd_network_list_type_e_type          network_list_type;

  sd_ss_pref_reas_e_type               pref_reas;
  sd_ss_orig_mode_e_type               orig_mode;
  sd_ss_srv_domain_pref_e_type         domain_pref;
  sd_ss_pref_update_reason_e_type      pref_update_reas;
} cmregprx_dbg_prot_gen_cmd_s_type;

// Debug Data Structures for MSG RTR

typedef struct
{
  lte_rrc_act_reason_e act_reason; /*!< Activation reason */
  lte_rrc_irat_rat_type_e src_rat; /* Source RAT information */
} cmregprx_dbg_lte_rrc_to_CM_act_ind_s_type;

// Debug Data Structures for NAS Commands


typedef struct
{
  sys_network_selection_mode_e_type          network_selection_mode;
  sys_mode_pref_e_type                       mode_pref;
  sys_srv_domain_e_type                      req_service_domain;
  sys_scan_scope_e_type                      scan_scope;
} cmregprx_dbg_cm_service_req_s_type;

typedef struct
{
  sys_mode_pref_e_type                       mode_pref;
  sys_srv_domain_e_type                      req_service_domain;
  reg_cm_network_list_req_e_type             list_search_type;
  sys_scan_scope_e_type                      scan_scope;
} cmregprx_dbg_cm_network_list_req_s_type;

typedef struct
{
  reg_cm_transaction_id_type                 transaction_id;
  sys_stop_mode_reason_e_type                stop_mode_reason;
} cmregprx_dbg_cm_stop_mode_req_s_type;

// Debug Data Structures for CM reports

typedef struct
{
  reg_cm_transaction_id_type                transaction_id;
  sys_srv_status_e_type                     service_status;
  sys_radio_access_tech_e_type              active_rat;
  sys_acq_status_e_type                     acq_status;
  reg_cm_scan_status_e_type                 scan_status;
} cmregprx_dbg_cm_service_cnf_s_type;


typedef struct
{
  sys_srv_status_e_type                     service_status;
  sys_radio_access_tech_e_type              active_rat;
  sys_acq_status_e_type                     acq_status;
  sys_no_svc_cause_e_type                   no_service_cause;
} cmregprx_dbg_cm_service_ind_s_type;

typedef struct
{
  reg_cm_transaction_id_type                transaction_id;
  uint32                                    length;
  sys_plmn_id_s_type                        rplmn;
  reg_cm_network_list_status_e_type         status;
} cmregprx_dbg_cm_network_list_cnf_s_type;

typedef struct
{
  reg_cm_transaction_id_type                transaction_id;
} cmregprx_dbg_generic_cnf_s_type;

typedef struct
{
  reg_cm_transaction_id_type                transaction_id;
  sys_ph_status_chgd_status_e_type          ph_status_chgd_status;
  // may be add as_id as well because anyway, 3 bytes are going to be wasted after packing
} cmregprx_dbg_cm_ph_status_chgd_cnf_s_type;

typedef struct
{
  sys_plmn_id_s_type              plmn;
  sys_radio_access_tech_e_type    active_rat;
} cmregprx_dbg_cm_camped_ind_s_type;

typedef struct
{
  sys_network_selection_mode_e_type        nw_sel_mode;
} cmregprx_dbg_cm_csg_nw_sel_mode_update_ind_s_type;

typedef struct
{
  sys_sim_state_e_type                     sim_state;
} cmregprx_dbg_cm_sim_state_update_ind_s_type;

typedef struct
{
  sys_radio_access_tech_e_type            rat;
} cmregprx_dbg_cm_bearer_ctxt_transfer_ind_s_type;

typedef struct
{
  boolean           status;
} cmregprx_dbg_cm_disable_bplmn_ind_s_type;

/*lint -save -e656 -e641
** Turn off enum to int / operationuses compatible enums
*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/**--------------------------------------------------------------------------
** Externs
** --------------------------------------------------------------------------
*/



/**--------------------------------------------------------------------------
** Functions
** --------------------------------------------------------------------------
*/

/*===========================================================================
FUNCTION cmregprx_dbg_ptr

DESCRIPTION
  Return a pointer to the one and only CM REGPRX debug buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
cmregprx_dbg_buffer_s_type  *cmregprx_dbg_ptr( void );


/*===========================================================================
FUNCTION cmregprx_dbg_init

DESCRIPTION
  Initialize the CM REGPRX Debug Buffer

  This function must be called before any CM submodule init,
  in order to capture the commands

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_init( void );

/*===========================================================================


FUNCTION cmregprx_dbg_set_stack_id

DESCRIPTION
  This function stores the value of stack_id and as_id received in current
  report so it can be used to determine the ss on which report is received,
  at time of adding the report to debug buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                    cmregprx_dbg_set_stack_id
(

  sys_modem_as_id_e_type                 as_id,

  sys_modem_stack_id_e_type              stack_id

);


/*===========================================================================
FUNCTION cmregprx_dbg_add_mmoc_cmd_to_buffer

DESCRIPTION
  Add MMOC command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_mmoc_cmd_to_buffer
(
  const cmregprx_cmd_msg_s_type  *cmd_ptr
);

/*===========================================================================
FUNCTION cmregprx_dbg_add_rpt_to_buffer

DESCRIPTION
  Add CM report to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmregprx_dbg_add_rpt_to_buffer
(
  const cm_rpt_type           *cm_rpt_ptr
);


/*===========================================================================
FUNCTION cmregprx_dbg_add_msg_rtr_to_buffer

DESCRIPTION
    Add MSG RTR command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_msg_rtr_to_buffer
(
  const cm_msgr_cmd_s_type   *rpt_ptr,
  sys_modem_as_id_e_type      asubs_id
);

/*===========================================================================
FUNCTION cmregprx_dbg_add_nas_cmd_to_buffer

DESCRIPTION
  Add NAS command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_nas_cmd_to_buffer
(
  cm_to_reg_cmd_type    msg_type,
  mm_sub_stk_id_s_type  mm_id,
  const void           *msg_payload
);

/*===========================================================================
FUNCTION cmregprx_dbg_create_new_entry

DESCRIPTION
  This function creates new entry in CM REGPRX Debug Buffer and initializes this entry

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_create_new_entry(void);

/*===========================================================================
FUNCTION cmregprx_dbg_add_new_entry

DESCRIPTION
  This function adds new Debug buffer Entry in CM REGPRX Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_new_entry
(
  cmregprx_buffer_type_e_type msg_type, /* Type of Buffer to be used */
  uint16                      msg_name, /* Specific cmd/report inside buffer mentioned above */
  mm_sub_stk_id_s_type        mm_id,  /* Sub/stack on which this new dbg msg has come/gone*/
  void                       *msg_payload /* Actual payload */
);

/*===========================================================================
FUNCTION cmregprx_dbg_if_same_mmoc_cmd

DESCRIPTION
  This function returns a boolean indicating whether the new MMOC command is identical to the most recently
  filled Debug message in the CMREGPRX debug buffer.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmregprx_dbg_if_same_mmoc_cmd
(
  const cmregprx_cmd_msg_s_type  *cmd_ptr
);

/*===========================================================================

FUNCTION cmregprx_dbg_buf_print_info

DESCRIPTION
  This function should be invoked by the MMOC when sanity timer expired to
  print value of cmregprx_debug_buffer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern  void     cmregprx_dbg_buf_print_info
(
  void
);


#endif


/*lint -restore */
#endif /* CMREGPRX_DBG_H */

