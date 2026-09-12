#ifndef DS_3GPP_SRV_REQ_THROT_SM_H
#define DS_3GPP_SRV_REQ_THROT_SM_H

/*!
  @file
  ds_3gpp_srv_req_throt_sm.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_srv_req_throt_sm.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/11   ss      Changes to support RPM.
08/30/10   hs      Initial version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "sys.h"
#include "ds_3gppi_utils.h"
#include "dsumtspdpreg.h"
#include "ds_3gpp_hdlr.h"
#include "ps_sys_event.h"
#include "ds_3gpp_pdn_context.h"
#include "dsumtspdpreg.h"
#include "ds_3gpp_throttle_sm.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

#define DS_3GPP_THROT_MAX_LOWER_LAYER_CNT 5

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*------------------------------------------------------------------------------
  This is the structure to be used to implement the system wide srv req throt 
  state machine. This structure holds an array of pointers to the core throt sm.

  We will instantiate only one object of this type. However, inside this object
  we have an array of 8 pointers to core state machines. This means we will hold
  throttling state of 8 PLMNS simultaneously. 

  If all throt sm's are in use and there is a PLMN
  reselection, we will not be able to throttle on the PLMN that the UE 
  reselected to.
------------------------------------------------------------------------------*/
typedef struct
{
  ds_3gppi_throttle_sm_s *sm_ptr[DS_3GPP_MAX_PLMN_SUPPORTED]; 
                                       // array of pointers to the core throt sm
  uint8                  free_count;   // counter to hold the number of free 
                                       // core sm
} ds_3gpp_srv_req_throt_sm_s;

/*------------------------------------------------------------------------------
This enum specifies the possible values of the NV DS_3GPP_CFG_AUTH_TYPE_FALLBACK. 
------------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_SRV_REQ_THROT_BLOCK_PLMN = 0, /* Service req throttling with PLMN blocking */
  DS_3GPP_SRV_REQ_THROT_BLOCK_PLMN_PREF_T3402, /* Service request throttling with PLMN blocking for T3402 duration */
  DS_3GPP_SRV_REQ_THROT_NO_PLMN_BLOCK, /* Service req throt with no plmn blocking */
  DS_3GPP_NO_SRV_REQ_THROT_NO_BLOCK_PLMN,/* No service req throt and no plmn blocking*/
  DS_3GPP_NO_SRV_REQ_THROT_MAX
} ds_3gpp_srv_req_throt_behavior_e;
 
/*-------------------------------------------------------------------------- 
  Enum to represent various throttling actions
  --------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_SRV_REQ_THROT_NO_ACTION = 0,
  DS_3GPP_SRV_REQ_THROT_DISABLE_LTE = 1,
  DS_3GPP_SRV_REQ_THROT_DISABLE_LTE_PER_PLMN = 2,
  DS_3GPP_SRV_REQ_THROT_ENABLE_LTE = 3,
  DS_3GPP_SRV_REQ_THROT_ENABLE_LTE_PER_PLMN = 4,
} ds_3gpp_srv_req_throt_action_e_type;

/*-------------------------------------------------------------------------- 
  Structure to hold information about service request throttling
  for logging
  -------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8     subs_id;
  boolean   is_throttled;
  uint8     throttle_action;
  uint8     counter;
  uint16    duration;
  uint16    mcc;
  uint16    mnc;
  char      reserved[4];
} ds_3gpp_srv_req_throt_event_type;

extern ds_3gpp_srv_req_throt_behavior_e ds_3gpp_srv_req_throttle_behavior[DS3GSUBSMGR_SUBS_ID_MAX];

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_srv_req_qsh_mdump_collect
  
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
void ds_3gpp_srv_req_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_INIT

DESCRIPTION
  This function initializes Service Request Throttle SM for a particular
  Subscription.

PARAMETERS  
  Subs Id

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_srv_req_throt_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_GET_THROT_SM

DESCRIPTION
  This function will be invoked by clients to get a handle to the singleton
  object that will be maintained for system wide throttling of service reqs.
  
PARAMETERS  
  create_if_not_present - This flag is used to determine if we have to create
                          a core sm if the wrapper sm does not have one for
                          the current PLMN
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  void* - A non-null void pointer will be given to clients. This is an opaque
          handle for the clients

  NULL  - If the singleton object for system wide throttling of service req was
          not created during power up. 

SIDE EFFECTS 
  None.
  
===========================================================================*/
void* ds_3gpp_srv_req_get_throt_sm
(
  boolean                create_if_not_present,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_FEED_FAILURE

DESCRIPTION
  This function feeds Service reject into the throttlig machine. This func
  takes care of figuring out if the number of consecutive failures has gone
  up and uses the core sm to apply appropriate back-off

PARAMETERS  
  void*         - Pointer to the throttling SM - opaque handle from client
  failure_count - The number of consecutive failures 
   ds_pdn_context_s  - *pdn_cntxt_ptr
   rab_throt_info
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_srv_req_throt_feed_failure
(
  void                                *throt_sm_ptr,
  ds_3gpp_throt_rab_info_s            *rab_throt_info,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_SM_RESET

DESCRIPTION
  This function is used to reset to state machine. This function internally
  cleans up the core state machine.

PARAMETERS  
  void*         - Pointer to the throttling SM - opaque handle from client
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_srv_req_throt_reset
(
  void*                   throt_sm_ptr,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_GET_IS_THROTTLED_FLAG

DESCRIPTION
  This function examines the presence of a core SM. The abscence of a core
  SM means no throttling is in place. If the core sm is present, then we
  query the core sm about the throttling state.
  
PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
  subs_id      - data subs

DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE  - If the SM is in throttled state
  FALSE - If the SM is not in throttled state
   
SIDE EFFECTS 
  None.

===========================================================================*/
boolean ds_3gpp_srv_req_throt_get_is_throttled_flag
(
  void*                   throt_sm_ptr,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_CLEAR

DESCRIPTION
  This function is registered as callback for LPM event. It clears the
  service request throttling state machine.
  
PARAMETERS  
  event_info - info about the LPM event
  user_data  - ignored as of now
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.

SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_srv_req_throt_clear
(
  ds_3gpp_event_info_u_type event_info,
  uint32                    user_data
);
/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_GO_NULL

DESCRIPTION
  This function is used to clean up the service request throttling state machine.
  This function internally cleans up the core state machine.

PARAMETERS  
 subs_id
 enable_plmn
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_srv_req_throt_go_null
(
  sys_modem_as_id_e_type subs_id,
  boolean                enable_plmn
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_GET_BACK_OFF_TIMER_VAL

DESCRIPTION
  This function is used to get the time for which throttling will happen.
  The client can use this any way it wants.

PARAMETERS  
  void*         - Pointer to the throttling SM - opaque handle from client
  uint32*       - Pointer to the timer_val - OUT param 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - If all went fine
  FALSE - If anything fails.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_srv_req_throt_get_back_off_timer_val
(
  void                  *sm_ptr, 
  uint32*                timer_val_ptr,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_GET_STATE

DESCRIPTION
  This function is used to get the state of the throttling sm

PARAMETERS  
  throt_sm_ptr         - Pointer to the throttling SM - opaque handle from client
  throt_state_ptr      - Pointer to the throt_state - OUT param 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - If all went fine
  FALSE - If anything fails.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_srv_req_throt_get_state
(
  void                    *throt_sm_ptr, 
  boolean                 *throt_state_ptr,
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_T3402_IS_ENABLED

DESCRIPTION
  This function checks if T3402 throttle is enabled.

PARAMETERS  
  CMD Ptr
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  boolean TRUE :  T3402 enabled
          FALSE : T3402 disabled
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_srv_req_throt_t3402_is_enabled
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_GET_BEHAVIOR

DESCRIPTION
  This function returns the 3GPP srv req throttle behavior

PARAMETERS
   subs id - subs id for the throttle behavior            
 
DEPENDENCIES
  None.

RETURN VALUE 
   ds_3gpp_srv_req_throttle_behavior - 3GPP srv req throttle behavior

SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_srv_req_throt_behavior_e ds_3gpp_srv_req_throt_get_behavior
(
  ds3gsubsmgr_subs_id_e_type subs_index
);

/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_GET_BEHAVIOR

DESCRIPTION
  This function returns the 3GPP srv req throttle behavior

PARAMETERS
   subs id - subs id for the throttle behavior            
 
DEPENDENCIES
  None.

RETURN VALUE 
   ds_3gpp_srv_req_throttle_behavior - 3GPP srv req throttle behavior

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_srv_req_throt_set_behavior
(
  ds_3gpp_srv_req_throt_behavior_e  behavior,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_SRV_REQ_THROT_VALIDATE_MIN_FAILURE_COUNT 
 
DESCRIPTION
  This function used to set the min failure count to given value if it is less 
 than that

PARAMETERS  
  void*         - Pointer to the throttling SM - opaque handle from client
  sys_modem_as_id_e_type subs id  
uint16 fail_count
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
===========================================================================*/

void ds_3gpp_srv_req_throt_validate_min_failure_count
(
  void                   *throt_sm_ptr, 
  sys_modem_as_id_e_type  subs_id, 
  uint16                  fail_count 
);

/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_EV_ALLOC_MEM

DESCRIPTION
  This function would allocate memory for svc_throt_event_ptr from heap

PARAMETERS
  Void
 
DEPENDENCIES
  None.

RETURN VALUE 
  ds_3gpp_srv_req_throt_event_type ptr
 
SIDE EFFECTS
  None.

===========================================================================*/

ds_3gpp_srv_req_throt_event_type *ds_3gpp_srv_req_throt_ev_alloc_mem
(
  sys_modem_as_id_e_type    subs_id    
);

/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_GEN_EVENT

DESCRIPTION
  This function would generate an event when service request throttling
  happens

PARAMETERS
  ds_3gpp_srv_req_throt_event_type  ptr
 
DEPENDENCIES
  The ptr must be populated with appropriate values

RETURN VALUE 
  void
 
SIDE EFFECTS
  None.

===========================================================================*/

void  ds_3gpp_srv_req_throt_gen_event
(
  ds_3gpp_srv_req_throt_event_type *srv_throt_ev_ptr,
  sys_modem_as_id_e_type    subs_id
);


/*===========================================================================
FUNCTION  DS_3GPP_SRV_REQ_THROT_POPULATE_AND_GEN_EVENT

DESCRIPTION
  This function would populate the structure and generate an event
  when service request throttling happens

PARAMETERS
   uint8                            failure_count
   uint16                           timer_val
   boolean                          is_throttled
   ds_3gpp_throt_svc_action_e_type  svc_action_type
   sys_modem_as_id_e_type           subs_id
   sys_plmn_id_s_type               *plmn_id
 
DEPENDENCIES

RETURN VALUE 
  void
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_srv_req_throt_populate_and_gen_event
(
   uint8                                 failure_count,
   uint32                                timer_val,
   boolean                               is_throttled,
   ds_3gpp_srv_req_throt_action_e_type   svc_action_type,
   sys_modem_as_id_e_type                subs_id,
   sys_plmn_id_s_type                   *plmn_id
);

/*===========================================================================
  FUNCTION DS_3GPP_SRV_REQ_THROT_HANDLE_EMM_CAUSE

  DESCRIPTION
  This function handles service request throttling for PDN Connectivity
  rejects with EMM Cause. Currently this function is simply a wrapper
  around the service request throttle handling done for Data Retry.
  The wrapper is created so that other modules such as DAM can re-use
  the data retry functionality.
 
  If more specialized handling for service request throttling is needed,
  the caller module should define its own API.
  
  PARAMETERS
  mode_info_p            - Mode Info
  subs_id                - Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None. 
===========================================================================*/
void ds_3gpp_srv_req_throt_handle_emm_cause
(
  cm_call_mode_info_s_type         *mode_info_p,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION      DS3GI_T3346_INFO_IND_HDLR

DESCRIPTION   When T3346 timer expires and data retry is enabled clear 
              service request throttling 

DEPENDENCIES  None

RETURN VALUE  If found, returns the call instance index.
              If not found, then returns DS3GI_INVALID_CALL INSTANCE

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_srv_throt_t3346_info_ind_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION ds_3gpp_srv_req_throt_get_core_sm_for_current_plmn
DESCRIPTION
  This function gets core SM ptr for the current PLMN
  
PARAMETERS  
  subs_id     -   SUB ID
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Core SM pointer or NULL
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_3gppi_throttle_sm_s* ds_3gpp_srv_req_throt_get_core_sm_for_current_plmn
(
  sys_modem_as_id_e_type subs_id
);

#endif /*DS_3GPP_SRV_REQ_THROT_SM_H*/
