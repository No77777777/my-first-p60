 /******************************************************************************
 @file    ds_3gpp_profile_api.h
 @brief

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009 - 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
****************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/09/13   vb      API to talk to Profile DB
=============================================================================*/

#include "ds_profile_db.h"
#include "ds_profile_3gpp.h"
#include "ds_3gpp_device_ev_hdlr.h"

/*-------------------------------------------------------------------------
  Enum masks that need to be populated when PROFILE DB param changes
  ------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PROFILE_APN_PARAM_CHG = 0x01,
  DS_3GPP_PROFILE_CLR_THROT = 0x02
}ds_3gpp_profile_mask_e;

typedef struct
{
  uint16                      profile_num;
  int8                        subs_id;
  void                       *user_data;
  ds_3gpp_profile_mask_e      mask;
}ds_3gpp_profile_db_param_chg_type;

typedef struct
{
  uint16  profile_num;
  boolean apn_disable_flag;
  sys_modem_as_id_e_type    subs_id;
}ds_profile_db_apn_disable_info_type;


typedef struct
{
  uint16                        profile_num;
  ds_umts_pdp_type_enum_type    new_pdp_type;
  int                           subs_id;
}ds_profile_db_pdp_chg_info_type;

typedef struct
{
  uint16                        profile_num;
  byte                          old_apn_name[DS_UMTS_MAX_APN_STRING_LEN+1];
  uint64                        old_apn_type_mask;
  sys_modem_as_id_e_type        subs_id;
}ds_profile_db_apn_name_chg_info_type;

typedef struct
{
  uint16                        profile_num;
  uint64                        old_apn_type_mask;
  sys_modem_as_id_e_type        subs_id;
}ds_profile_db_apn_type_chg_info_type;

/*------------------------------------------------------------------------- 
  Enum masks to identify cb mask
  ------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_PROFILE_NO_MASK = 0,
  DS_3GPP_PROFILE_PDP_CONTEXT_PDP_TYPE = 1,
  DS_3GPP_PROFILE_APN_DISABLE_FLAG = 2,
  DS_3GPP_PROFILE_APN_NAME = 4,
  DS_3GPP_PROFILE_PDN_REQ_WAIT_TIMER = 8,
  DS_3GPP_PROFILE_MAX_PDN_CONN_PER_BLOCK = 16,
  DS_3GPP_PROFILE_MAX_PDN_CONN_TIMER = 32,
  DS_3GPP_PROFILE_ROAMING_DISALLOWED = 64,
  DS_3GPP_PROFILE_APN_TYPE = 128
}ds_3gpp_profile_cb_mask_e;

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_VALIDATION_FUNC

DESCRIPTION
  The function retrieves the function to look up based on the identifier passed
  and validates appropriate parameters.

PARAMETERS:
  ds_profile_db_ident_type  - Identifier type
  ds_profile_db_info_type   - Profile DB info type
  profile_params          - Parameters to validate

DEPENDENCIES
  None.

RETURN VALUE
  ds_profile_db_result_type

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_validation_func
(
  ds_profile_db_ident_type        ident,
  const ds_profile_db_info_type  *info,
  const void*                     profile_params,
  ...
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_SET_DEFAULT_PROFILE_NUM_PER_SUBS

DESCRIPTION
  The function sets the default profile number based on the family
  type passed

PARAMETERS:
 void

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_set_default_profile_num_per_subs
(
  ds_profile_db_profile_num_type  num,
  ds_profile_db_family_type       family,
  ds_profile_db_subs_type         subs
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_GET_DEFAULT_PROFILE_NUM_PER_SUBS

DESCRIPTION
  The function fetches the default profile number based on the family
  type passed and subs id

PARAMETERS:
 void

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_get_default_profile_num_per_subs
(
  ds_profile_db_profile_num_type  *num,
  ds_profile_db_family_type        family,
  ds_profile_db_subs_type          subs
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_VALIDATION_GEN

DESCRIPTION
  The function validates general tech specific parameters

PARAMETERS:
  ds_profile_db_3gpp_tech_params_type* profile

DEPENDENCIES
  None.

RETURN VALUE
  ds_profile_db_result_type

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_validation_gen
(
  const ds_profile_db_3gpp_tech_params_type  *profile
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_SETUP_DEFAULT_PROFILES

DESCRIPTION
   initialize default profiles by reading configuration file . Create default profiles
   if already does not exist.

PARAMETERS:
      subs_id: Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_setup_default_profiles
(
   ds_umts_pdp_subs_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_EVENT_CB

DESCRIPTION
  Called by Profile DB to notify any event.

PARAMETERS:
   ds_profile_db_event_type event,
   ds_profile_db_profile_managed_type *profiles,
   uint16 profiles_count,
   void* user_data

DEPENDENCIES
  None.

RETURN VALUE
 void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_event_cb
(
   ds_profile_db_event_type              event_name,
   ds_profile_db_profile_managed_type  **profiles,
   uint16                                profile_count,
   void                                *user_data,
   ds_profile_db_subs_type              subs
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_GET

DESCRIPTION
  The function extracts from cache the tech and gen params based on the profile
  number passed.

PARAMETERS:
  ds_profile_db_profile_num_type num,
  ds_profile_db_3gpp_tech_params_type* tech_params,
  ds_profile_db_gen_params_type* gen_params

DEPENDENCIES
  None.

RETURN VALUE
  ds_profile_db_result_type

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_get
(
  ds_profile_db_profile_num_type        num,
  ds_profile_db_subs_type               subs_id,
  ds_profile_db_3gpp_tech_params_type  *tech_params,
  ds_profile_db_gen_params_type        *gen_params
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_GET_DEFAULT_PARAMS

DESCRIPTION
  The function extracts from the default tech params

PARAMETERS:
   ds_profile_db_3gpp_tech_params_type* tech_profile_ptr

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_get_default_values
(
  ds_profile_db_3gpp_tech_params_type  *tech_profile_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_GET_DEFAULT_SUB_ID

DESCRIPTION
  The function retrieves the subscription ID

PARAMETERS:
  void

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_subs_type ds_3gpp_profile_get_default_sub_id( void );

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_API_INIT

DESCRIPTION
  The function registers for callback events with profile DB

PARAMETERS:
 void

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_api_init( void );

typedef ds_profile_db_result_type (*validate_3gpp_fn_type)
(
  ds_profile_db_ident_type        ident,
  const ds_profile_db_info_type  *info,
  const void                     *profile_params
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_IS_EPC_TECH_PARAM_SET

DESCRIPTION
  The function extracts from cache if the tech param was set for a given EPC
  common parameter

PARAMETERS:
  ds_profile_db_profile_num_type num,
  ds_profile_db_subs_type subs_id
  ds_profile_db_ident_type ident
  boolean* is_tech_param_set

DEPENDENCIES
  None.

RETURN VALUE
  ds_profile_db_result_type

SIDE EFFECTS
  None

===========================================================================*/
ds_profile_db_result_type ds_3gpp_profile_is_epc_tech_param_set
(
  ds_profile_db_profile_num_type  num,
  ds_profile_db_subs_type         subs_id,
  ds_profile_db_ident_type        ident,
  boolean                        *is_tech_param_set
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_POST_PARAM_CHG_CMD_TO_DS_TASK

DESCRIPTION
   This would post a command which would be processed in DS task

PARAMETERS:

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_post_param_chg_cmd_to_ds_task
(
  uint16                   profile_num,
  int8                     subs_id,
  void                    *user_data,
  ds_3gpp_profile_mask_e   mask
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_CREATE_PROFILE

DESCRIPTION
   Creates the profile number passed in input in profile db

PARAMETERS:
   subs_id: Subscription ID
   is_persistent: profile is persistence or non persistence
   profile_num : the profile number to create

DEPENDENCIES
  None.

RETURN VALUE
  DS_UMTS_PDP_SUCCESS = if profile created successfully
  DS_UMTS_PDP_FAIL = otherwise

SIDE EFFECTS
  None
===========================================================================*/
ds_umts_pdp_profile_status_etype ds_3gpp_profile_create_profile
(
  ds_umts_pdp_subs_e_type  subs_id,
  boolean                  is_persistent,
  uint16                   profile_num
);

/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_UPDATE_PROFILE_PARAM

DESCRIPTION   This utility function updates specified profile parameters
              for the given profile

PARAMETERS    subs_id: subscription id

DEPENDENCIES  None

RETURN VALUE  TRUE: Update APN name successful
              FALSE: Update APN name unsuccessful

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_update_profile_param
(
  ds_umts_pdp_subs_e_type          subs_id,
  uint16                           profile_num,
  ds_profile_3gpp_param_enum_type  ident,
  void                            *param_ptr,
  uint8                            param_len
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_GET_AP_DEF_PROFILE_PER_SUBS

DESCRIPTION
  This function is used to get AP default profile number form the specified
  family and subscription.

PARAMETERS
  family  : type of profile (embedded or tethered)
  subs_id : subscription id (subs 1 or subs 2)
  num     : profile number

DEPENDENCIES
  None

RETURN VALUE
  DS_PROFILE_REG_RESULT_SUCCESS   : On successful operation
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE : Profile type is invalid
  DS_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM  : Invalid profile number
  DS_PROFILE_REG_RESULT_ERR_INVAL_OP : Operation not supported for tech type
  DS_PROFILE_REG_RESULT_FAIL      : On general errors. This return
                                    code provides blanket coverage
  DS_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID : Invalid subscription id passed

SIDE EFFECTS
  None
===========================================================================*/
ds_profile_status_etype dsi_profile_3gpp_get_ap_def_profile_per_subs
(
  uint32                    family,
  ds_umts_pdp_subs_e_type   subs_id,
  ds_profile_num_type      *num
);

/*===========================================================================
FUNCTION DS_PROFILE_3GPP_CLEAR_AP_DEF_PROFILE_NUMBER_PER_SUBS

DESCRIPTION
  This function resets the AP default profile number for tethered or
  embedded family for the specified subscription.

PARAMETERS
  family           : Denotes embedded or tetherd profile
  subs             : Denotes one of two active subscriptions

DEPENDENCIES


RETURN VALUE
  DS_UMTS_PDP_SUCCESS              : The operation completed successfully
  DS_UMTS_PDP_NOT_INITIALIZED      : Registry API library not initialized
  DS_UMTS_PDP_INVALID_PROFILE_NUM  : Invalid profile number
  DS_UMTS_PDP_ACCESS_ERROR         : Error in updating the file

SIDE EFFECTS
  None
===========================================================================*/
extern ds_umts_pdp_profile_status_etype
dsi_profile_3gpp_clear_ap_def_profile_number_per_subs
(
  ds_umts_profile_family_e_type  family,
  ds_umts_pdp_subs_e_type        subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_RESET_AP_DEFAULT_APN

DESCRIPTION   This function resets the AP default APN name

PARAMETERS    subs_id: subscription id

DEPENDENCIES  None

RETURN VALUE  TRUE: Reset AP default APN successful
              FALSE: Reset AP default APN unsuccessful

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_reset_ap_default_apn
(
  ds_umts_pdp_subs_e_type  subs_id
);
/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_CREATE_MODEM_DEF_PROFILE

DESCRIPTION   This function creates the modem_def_profile

PARAMETERS    Pointer to the device settings info

DEPENDENCIES  None

RETURN VALUE  TRUE, if successful
              FALSE, if failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_create_modem_def_profile
(
  ds3gdevmgr_device_settings_info_type  *device_settings_info_p
);
/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_SET_AP_DEFAULT_APN

DESCRIPTION   This function sets the AP default APN name

PARAMETERS    subs_id : Subscription ID
              apn_configured: apn_name field is configured or not
              new_apn_name : new APN Name provided

DEPENDENCIES  None

RETURN VALUE  TRUE, if successful
              FALSE, if failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_set_ap_default_apn
(
  ds_umts_pdp_subs_e_type  subs_id,
  boolean                  apn_configured,
  byte                    *new_apn_name
);

