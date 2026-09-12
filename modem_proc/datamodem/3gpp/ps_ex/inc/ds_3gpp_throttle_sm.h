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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_throttle_sm.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/11   ss      Changes to support RPM.
08/30/10   hs      Initial version

===========================================================================*/

#ifndef DS_3GPP_THROTTLE_SM_H
#define DS_3GPP_THROTTLE_SM_H

#define DS_THROT_LOG_PARAM_NA          0x0F

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

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define DS_3GPP_THROT_TIMER_MIN_SECOND 1

#ifndef TEST_FRAMEWORK
#define DS_3GPP_THROT_TIMER_MAX_SECOND 15
#else
#error code not present
#endif

#define DS_3GPP_THROT_SECOND_TO_MILISECOND_MULTIPLIER 1000
#define DS_3GPP_MINUTE_SECOND_MULTIPLIER 60
#define DS_3GPP_HOUR_MINUTE_MULTIPLIER   60
#define DS_3GPP_THROT_SM_COOKIE 0xbeabeabe

/*-----------------------------------------------------------------------
  This enumeration will indicate the kind of Throttling (PDN or Service Request) 
  being applied. 
------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_THROT_TYPE_MIN = 0,
  DS_3GPP_THROT_TYPE_PDN = DS_3GPP_THROT_TYPE_MIN,
  DS_3GPP_THROT_TYPE_SRV_REQ,
  DS_3GPP_THROT_TYPE_MAX
} ds_3gpp_throt_sm_type;

/*------------------------------------------------------------------------------
  This enumeration lists the conditions under which throttling maybe reset
------------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_THROT_RESET_TYPE_LPM,
  DS_3GPP_THROT_RESET_TYPE_CARD_REFRESH,
  DS_3GPP_THROT_RESET_TYPE_CARD_MAX
} ds_3gpp_throt_sm_reset_type;


/*------------------------------------------------------------------------------
  Function pointer type for core SM timer start/stop callbacks 
------------------------------------------------------------------------------*/
typedef void (*ds_3gppi_throttle_sm_timer_ev_cb_f) (void* throt_sm_ptr, 
                                                    void* user_data);

/*------------------------------------------------------------------------------
  Functon pointer type for fetching the correct Throttling Timer Value
------------------------------------------------------------------------------*/
typedef uint32 (*ds_3gppi_throttle_sm_get_value_f)  (void* throt_sm_ptr, 
                                                     void* user_data);

/*------------------------------------------------------------------------------
  Functon pointer type for incrementing throttle count
------------------------------------------------------------------------------*/
typedef void (*ds_3gppi_throttle_sm_inc_cnt_f)  (void* throt_sm_ptr, 
                                                 void* user_data);

/*------------------------------------------------------------------------------ 
  Function pointer for module-specific checks on whether
  throttling can be cleared upon LPM/PLMN Change/USIM Removal/Detach
  ------------------------------------------------------------------------------*/
typedef boolean (*ds_3gppi_throttle_clear_on_trigger_f) (void* throt_sm_ptr,
                                                     void* user_data);

/*------------------------------------------------------------------------------
  Struct to contain information about user_data
  ------------------------------------------------------------------------------*/
typedef struct
{
  ps_iface_net_down_reason_type                  cause_code;
}ds_3gpp_throt_user_data;

/*------------------------------------------------------------------------------
  This structure is used to implement the throttling state machine
  This is a generic state machine and anyone who is interested in using the
  throttling algorithm can make use of this module.
------------------------------------------------------------------------------*/
typedef struct
{
  boolean                            is_throttled;  /* boolean indicating if throttling is ON */
  uint8                              failure_count; /* Numer of attempts rejected by the 
                                                                                      network */
  rex_timer_type                     throttle_timer;/* Timer to start when the SM is in 
                                                                                     throttle */
  uint32                             throt_tmr_val; /* The value that the throt timer has to be
                                                                                        set to*/
  sys_plmn_id_s_type                 plmn_id;       /* PLMN that this SM is applicable to     */
                                    
  byte                               apn[DS_UMTS_MAX_APN_STRING_LEN + 1]; /* APN for this SM */


  ds_3gpp_throt_user_data            user_data; /*To contain any particular user_data*/

  uint16                             throttle_sys_mask;/* current sys-mode mask */

  sys_modem_as_id_e_type             subs_id;

  uint32                             cookie;

  uint32                             min_random_timer_val;/*user configured min/max random timer values*/

  uint32                             max_random_timer_val;/*user configured min/max random timer values*/

  void                              *tmr_exp_cb_data_p;

  void                              *parent_ptr;

  ds_3gpp_throt_sm_type              throt_sm_type;

  uint8                              lwr_lyr_failure_cnt;

} ds_3gppi_throttle_sm_s;

/*-------------------------------------------------------------------------
  This structure is used to store the Generic Callback tables registered
  by PDN Throttle SM and Service Request Throttle SM
-------------------------------------------------------------------------*/

typedef struct
{
  ds_3gppi_throttle_sm_timer_ev_cb_f   tmr_exp_cb;                                   
  ds_3gppi_throttle_sm_timer_ev_cb_f   tmr_start_cb;                                   
  ds_3gppi_throttle_sm_get_value_f     get_tmr_val_cb;
  ds_3gppi_throttle_sm_inc_cnt_f       inc_cntr_cb;                                   
} ds_3gpp_throt_sm_info_tbl_s;

/*-------------------------------------------------------------------------
  This structure is used to store the Callback data when 
  feeding failure  into the Core Throttle State machine.
  Core Throttle SM will pass the callback data to the 
  relevant throttling module (PDN or Service Request Throttle 
  module) when invoking the timer start/expiry/get timer callbacks.
-------------------------------------------------------------------------*/
typedef struct
{
  void* tmr_start_cb_data_p;
  void* tmr_exp_cb_data_p;
  void* get_tmr_val_cb_data_p;
} ds_3gpp_throttle_sm_cb_data_type;

typedef struct
{
  boolean  throttle_needed;
  boolean  lower_layer_failure;
}ds_3gpp_throt_rab_info_s;

extern rex_crit_sect_type ds_3gppi_throttle_sm_crit_sect;


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_throt_qsh_mdump_collect
  
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
void ds_3gppi_throt_sm_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION DS_3GPP_THROT_REGISTER_SM_INFO_TABLE

DESCRIPTION
  This function is used by PDN and Service request throttling modules
  to register their respective info tables, which specify attributes 
  such as start timer callback, stop timer callback, Get Timer Value Callback

PARAMETERS   :
  cb_tbl_ptr        - Pointer to the Info Table
  pdn_throt_sm_type - Type of Throttling (PDN or Service request)
  subs_id           - Subscription Id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_throt_register_sm_info_tbl
(
  ds_3gpp_throt_sm_info_tbl_s    *cb_tbl_ptr,
  ds_3gpp_throt_sm_type            throt_sm_type,
  sys_modem_as_id_e_type           subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_INIT

DESCRIPTION
  This function initializes the core throttling state machine
  
PARAMETERS  
  None.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.

SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_throt_sm_init
(
  void
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_CREATE_CORE_THROTTLE_SM

DESCRIPTION
  This function gets a new throttle state machine instance to a client. It
  also initializes all the fields appropriately.

  This is the internals of state machine implementation - Will also be refered
  to as the core state machine
  
PARAMETERS  
  plmn_id:         PLMN Id
  subs_id:         Subscription Id
  throt_sm_type:   Throttle SM Type (PDN or SRV request)
  parent_ptr       Pointer to the parent of the Core Throttle SM 
                   being allocated

DEPENDENCIES 
  None.
  
RETURN VALUE 
  Pointer to the newly obtained throttle SM.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_3gppi_throttle_sm_s* ds_3gppi_throt_sm_create_core_throttle_sm
(
  sys_plmn_id_s_type           plmn_id,
  sys_modem_as_id_e_type       subs_id,
  ds_3gpp_throt_sm_type        throt_sm_type,
  void                        *parent_ptr
);



/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_FREE_CORE_STATE_MACHINE

DESCRIPTION
  This function destroys a throttle SM that a client has been using. This can
  be called whenever throttling is stopped.

  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr - Pointer to the throttling SM to be released.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gppi_throt_sm_free_core_state_machine
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_FEED_FAILURE_TO_CORE_SM

DESCRIPTION
  This function is used to apply throttling. This performs the following
  operations:
  1. Set the throttle SM to THROTTLED state
  2. Increment the counter corresponding to the number of rejects
  3. Get the throttle timer value based on the number of rejects
  4. Start the throttling timer
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr              Pointer to the throttling SM to be released.
  cb_data_p                 CB Data to be passed when performing throttling
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gppi_throt_sm_feed_failure_to_core_sm
(
  ds_3gppi_throttle_sm_s             *throt_sm_ptr,
  ds_3gpp_throt_rab_info_s           *rab_throt_info,
  ds_3gpp_throttle_sm_cb_data_type   *cb_data_p
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_GET_IS_THROTTLED_FLAG

DESCRIPTION
  This function examines if a throttling SM is in throttled or unthrottled
  state
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - If the SM is in throttled state
  FALSE - If the SM is not in throttled state
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gppi_throt_sm_get_is_throttled_flag
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_GET_PLMN_ID

DESCRIPTION
  This function returns the plmn_id for the core SM

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  sys_plmn_id_s_type - plmn_id of the core SM. 

SIDE EFFECTS 
  None.
  
===========================================================================*/
sys_plmn_id_s_type ds_3gppi_throt_sm_get_plmn_id
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr  
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_GET_FAILURE_COUNT

DESCRIPTION
  This function is used to get the count of failed attempts that is maintained
  by the core state machine. Note that such an API will not be exposed from
  any wrapper. Failures will always be fed into the wrapper. The wrapper has
  the logic needed to increment counter values.
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
  failure_count_ptr - out param pointing to the failure count
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - if the incoming params were non-NULL
  FALSE - incoming params NULL
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gppi_throt_sm_get_failure_count
(
  ds_3gppi_throttle_sm_s *throt_sm_ptr,
  uint8                  *failure_count_ptr
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_SET_FAILURE_COUNT

DESCRIPTION
  This function is used to set the count of failed attempts that is maintained
  by the core state machine. Note that such an API will not be exposed from
  any wrapper. Failures will always be fed into the wrapper. The wrapper has
  the logic needed to increment counter values.
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr  - pointer to the throttling state machine
  failure_count - failure_count to be stored in the core sm
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - On success
  FALSE - On any failure
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gppi_throt_sm_set_failure_count
(
  ds_3gppi_throttle_sm_s *throt_sm_ptr,
  uint8                  failure_count
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_GET_THROTTLE_TIMER_VAL

DESCRIPTION
  This function returns the timer value for which throttling is to be done.
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  uint32 - the value that the throttling timer was started with.  

SIDE EFFECTS 
  None.
  
===========================================================================*/
uint32 ds_3gppi_throt_sm_get_throttle_timer_val
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr  
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_GET_REMAINING_THROTTLE_TIME

DESCRIPTION
  This function returns the remaining time in milliseconds for which
  throttling occurs
  
  This is the internals of state machine implementation - Will also be refered
  to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  uint32 - remaining throttle time.  

SIDE EFFECTS 
  None.
  
===========================================================================*/
uint32 ds_3gppi_throt_sm_get_remaining_throttle_time
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr  
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_CLEAR_THROTTLE_TIMER

DESCRIPTION
  This function clears the throttling timer for the specified core throt_sm.

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gppi_throt_sm_clear_throttle_timer
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_SM_IS_PLMN_NON_NULL

DESCRIPTION
  This function checks if the PLMN passed into this function is non-NULL
  
PARAMETERS  
  plmn_id - PLMN ID to be checked
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE  - If PLMN is non-NULL
  FALSE - otherwise

SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_throt_sm_is_plmn_non_null
(
  sys_plmn_id_s_type plmn_id
);

/*========================================================================= 
  FUNCTION ds_3gpp_throt_sm_reset_core_throt_sm

DESCRIPTION
  This function resets the throttle statae machine 
  paramaters asscoiated with release 10 paramters 
  
PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine

DEPENDENCIES 
  None.

RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.

===========================================================================*/


void ds_3gpp_throt_sm_reset_core_throt_sm
(
  void*   throt_sm_ptr
);

/*===========================================================================
FUNCTION DS_3GPPI_THROT_SM_SET_THROTTLE_TIMER_VAL

DESCRIPTION
  This function sets the timer value for which throttling is to be done.
  
  This is the internals of state machine implementation - Will also be
  referred to as the core state machine
 
PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
  timer_val    - Value to be set
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gppi_throt_sm_set_throttle_timer_val
(
  ds_3gppi_throttle_sm_s* throt_sm_ptr,
  uint32                  timer_val 
);

/*===========================================================================
FUNCTION DS_3GPPI_SET_IS_THROTTLED_FLAG

DESCRIPTION
  This function sets a throttling SM to throttled or unthrottled
  state
  
  This is the internals of state machine implementation - Will also be
  referred to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
  is_throttled - boolean value to set the is_throttled flag
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None. 
 
SIDE EFFECTS 
  None.
  
===========================================================================*/

void ds_3gppi_throt_sm_set_is_throttled_flag
(
  ds_3gppi_throttle_sm_s *throt_sm_ptr,
  boolean                 is_throttled
);

/*===========================================================================
FUNCTION DS_3GPPI_GET_USER_DATA

DESCRIPTION
  This function returns the user_data contained in the core throttle sm.
  
  This is the internals of state machine implementation - Will also be
  referred to as the core state machine

PARAMETERS  
  throt_sm_ptr - pointer to the throttling state machine
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  ds_3gpp_throt_user_data 
 
SIDE EFFECTS 
  None.
  
===========================================================================*/

ds_3gpp_throt_user_data *ds_3gppi_throt_sm_get_user_data
(
  ds_3gppi_throttle_sm_s *throt_sm_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_PROCESS_CMDS

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
void ds_3gpp_throt_sm_process_cmds
(
  const ds_cmd_type *cmd_ptr 
);

/*===========================================================================
FUNCTION DS_3GPP_THROT_FILL_CB_DATA

DESCRIPTION
  This function is used to populate the Callback Data structure that will be
  passed on to Core throttle SM when performing throttling. Core Throttle
  SM will pass the callback data to the relevant throttling module (PDN or
  Service Request Throttle module) when invoking the timer
  start/expiry/get timer callbacks.

PARAMETERS
  cb_data_p:             CB Data to be populated
  tmr_start_cb_data_p:   Timer Start Callback Data
  tmr_exp_cb_data_p:     Timer Expiry Callback Data
  tmr_get_val_cb_data_p: Get Timer Callback Data 
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
 
SIDE EFFECTS 
  None
  
===========================================================================*/
void ds_3gpp_throt_fill_cb_data
(
  ds_3gpp_throttle_sm_cb_data_type *cb_data_p,
  void                             *tmr_start_cb_data_p,
  void                             *tmr_exp_cb_data_p,
  void                             *tmr_get_val_cb_data_p 
);

/*===========================================================================
FUNCTION DS_3GPP_THROTTLE_SM_INCREMENT_FAILURE_COUNTER

DESCRIPTION
  This function is used to increment the failure timer

PARAMETERS   :

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void ds_3gpp_throttle_sm_increment_failure_counter
(
   ds_3gppi_throttle_sm_s              *throt_sm_ptr
);

#endif /* DS_3GPP_THROTTLE_SM_H */
