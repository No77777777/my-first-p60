 /******************************************************************************
 @file    ds_3gpp_profile_api.c
 @brief

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009 - 2015 Qualcomm Technologies Incorporated.
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

#include "ds_3gpp_profile_api.h"
#include "modem_mem.h"
#include "ds_3gpp_profile_cache_hdlr.h"
#include "ds_3gpp_profile_validator.h"
#include "ds_profile_db.h"
#include "ds_profile_db_internal.h"
#include "dsumtspdpregint.h"
#include "dsumtspdpreg.h"
#include "ds_profile_db_handler.h"
#include "ds3gmgr.h"
#include "ds_3gppi_utils.h"
#include "dstask_v.h"
#include "ds3gcfgmgr.h"
#include "ds_3gpp_nv_manager.h"
#include "ds_dsd_apm_ext_i.h"
#include "ds3gpp_api.h"
#ifdef FEATURE_DUAL_SIM
#include "dsumts_subs_mgr.h"
#include "ds3gmgr.h"
#endif

#include "mcfg_fs.h"
#include <stringl/stringl.h>

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*------------------------------------------------------------------------
  Maximum size in bytes for profile parameters.
  Currently maximum size is 128 bytes i.e. for password. Setting this
  value to be 256 bytes to be future safe. Also no macro is defined in DS
  profile, hence defining one here for local use.
-------------------------------------------------------------------------*/
#define DS_3GPP_PROFILE_MAX_PARAM_SIZE 256

/*------------------------------------------------------------------------
   Callback function pointer
-------------------------------------------------------------------------*/
typedef void (*ds_3gpp_profile_ev_func_type)(
                ds_profile_db_event_type  profile_event,
                uint16                    profile_num,
                ds_umts_pdp_subs_e_type   subs_id);

typedef struct
{
  ds_3gpp_profile_ev_func_type  prof_ev_func_ptr;
} ds_3gpp_profile_def_prof_info_type;

/*===========================================================================

                           STATIC/GLOBAL variables

===========================================================================*/
static ds_profile_db_callback_id_type ds_3gpp_callback_id = 0;

/*---------------------------------------------------------------
  Global variable storing profile change callback info
---------------------------------------------------------------*/
static ds_3gpp_profile_def_prof_info_type ds_3gpp_profile_def_prof_info = {0};

/*===========================================================================

                    Forward declarations/prototypes

===========================================================================*/
/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_MODEM_GET_VALIDATION_FUNC

DESCRIPTION
  This function returns the validation function pointer corresponding
  to the identifier passed

PARAMETERS
  ident : identifier for which index is to be returned

DEPENDENCIES

RETURN VALUE
  returns validate_fn_type

SIDE EFFECTS

===========================================================================*/
extern validate_3gpp_fn_type dsi_profile_3gpp_modem_get_validation_func
(
  ds_profile_db_ident_type  ident
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_MODEM_WRITE_IDENT_INFO

DESCRIPTION
  This function updates the identifier info in the cache

PARAMETERS
   uint16                            profile_number,
   ds_profile_db_ident_type          ident,
   ds_profile_db_info_type           *info
   ds_profile_db_supported_tech_mask tech_mask
   boolean                           apn_param_check
   old pdp profile data

DEPENDENCIES

RETURN VALUE
  cb_mask  -- mask to identify cb mask

SIDE EFFECTS

===========================================================================*/
extern ds_3gpp_profile_cb_mask_e dsi_profile_3gpp_modem_write_ident_info
(
  uint16                                      profile_number,
  ds_profile_db_ident_type                    ident,
  const ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr,
  const ds_profile_db_gen_params_type        *gen_params,
  ds_profile_db_supported_tech_mask           tech_mask,
  boolean                                     apn_param_check,
  ds_umts_pdp_profile_type                   *old_pdp_profile_data
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_GET_DEFAULT_PROFILE

DESCRIPTION
  This function is used to get default profile number for a particular
  family

PARAMETERS
  family : type of profile (socket, rmnet, atcop)
  num : profile number

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern ds_profile_status_etype dsi_profile_3gpp_get_default_profile
(
  uint32                  family,
  ds_profile_num_type    *num,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_SET_DEFAULT_PROFILE

DESCRIPTION
  This function is used to set default profile number for a particular
  family

PARAMETERS
  family : type of profile (socket, rmnet, atcop)
  num : profile number

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern ds_profile_status_etype dsi_profile_3gpp_set_default_profile
(
  uint32               family,
  ds_profile_num_type  num
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_GET_DEFAULT_PROFILE_PER_SUBS

DESCRIPTION
  This function is used to get default profile number form the specified
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
extern ds_profile_status_etype dsi_profile_3gpp_get_default_profile_per_subs
(
  uint32                  family,
  ds_profile_subs_etype   subs_id,
  ds_profile_num_type    *num
);

/*===========================================================================
FUNCTION DS_PROFILE_3GPP_SET_DEFAULT_PROFILE_NUMBER_PER_SUBS

DESCRIPTION
  This function sets the profile number for tethered or embedded family for
  the specified subscription. The value is written to either
  embedded_call_prof_nums_dsds or tethered_call_prof_nums_dsds file.

PARAMETERS
  family           : Denotes embedded or tetherd profile
  subs             : Denotes one of two active subscriptions
  profile_number   : Profile number

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
dsi_profile_3gpp_set_default_profile_number_per_subs
(
  ds_umts_profile_family_e_type  family,
  ds_umts_pdp_subs_e_type        subs_id,
  uint16                         profile_number
);

/*===========================================================================
FUNCTION DS_PROFILE_3GPP_SET_AP_DEF_PROFILE_NUMBER_PER_SUBS

DESCRIPTION
  This function sets the AP default profile number for tethered or
  embedded family for the specified subscription. The value is written to
  either embedded_call_prof_nums_dsds or tethered_call_prof_nums_dsds file.

PARAMETERS
  family           : Denotes embedded or tetherd profile
  subs             : Denotes one of two active subscriptions
  profile_number   : Profile number

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
dsi_profile_3gpp_set_ap_def_profile_number_per_subs
(
  ds_umts_profile_family_e_type  family,
  ds_umts_pdp_subs_e_type        subs_id,
  uint16                         profile_number
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_MODEM_GET_APN_PARAM_CHANGED

DESCRIPTION
  This function returns apn_param_changed

PARAMETERS
  void

DEPENDENCIES

RETURN VALUE
  TRUE         If apn_params has changed
  FALSE        Otherwise

SIDE EFFECTS

===========================================================================*/
extern boolean dsi_profile_3gpp_get_apn_changed_param
( 
  ds_umts_pdp_subs_e_type  subs_id
);

/*===========================================================================
FUNCTION DSI_PROFILE_3GPP_MODEM_RESET_APN_PARAM_CHANGED

DESCRIPTION
  This function resets apn_param_changed

PARAMETERS
  profile_number       -- Profile number 
  old_pdp_profile_data -- Pointer to old profile data
  cb_mask              -- Mask of cb's to be called
  subs_id              -- Subscription ID

DEPENDENCIES

RETURN VALUE
void

SIDE EFFECTS

===========================================================================*/
extern void dsi_profile_3gpp_reset_apn_changed_param
( 
  ds_umts_pdp_subs_e_type  subs_id
);


extern void ds_profile_3gpp_dispatch_cb
(
  uint16                       profile_number,
  ds_umts_pdp_profile_type    *old_pdp_profile_data,
  ds_3gpp_profile_cb_mask_e    cb_mask,
  sys_modem_as_id_e_type       subs_id
);


/*===========================================================================

                            FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_CREATE_CMD_HDLR

DESCRIPTION
   Posted by MH in response to profile_create event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_create_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                              profile_count
)
{
  boolean                               ret_val = FALSE;
  uint16                                profile_number = 0;
  ds_profile_db_ident_type             *ident_ptr = NULL;
  ds_profile_db_ident_type             *gen_ident_ptr = NULL;
  uint16                                ident_count = 0;
  uint16                                gen_ident_count = 0;
  int                                   ident_index = 0;
  ds_profile_db_result_type             result_type = DB_RESULT_FAIL;
  ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr = NULL;
  ds_profile_db_gen_params_type        *ds_profile_db_gen_params_ptr = NULL;
  ds_profile_db_tech_type               tech_type = DB_TECH_3GPP;
  ds_profile_db_supported_tech_mask     tech_mask = 0;
  ds_umts_pdp_subs_e_type               subs_id
                                         = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE;
  boolean                               apn_param_check = FALSE;
  sys_modem_as_id_e_type as_subs_id = SYS_MODEM_AS_ID_NONE; 
  ds_3gpp_profile_cb_mask_e             cb_mask = DS_3GPP_PROFILE_NO_MASK;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
     Get the array of changed identifiers and for every changed identifier
     get the changed parameters
  -------------------------------------------------------------------------*/

  if(profiles == NULL)
  {
    goto func_exit;
  }

  ds_3gpp_tech_params_ptr =
    (ds_profile_db_3gpp_tech_params_type*)modem_mem_alloc(
                                   sizeof(ds_profile_db_3gpp_tech_params_type),
                                   MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_3gpp_tech_params_ptr == NULL)
  {
    goto func_exit;
  }
  memset(ds_3gpp_tech_params_ptr, 0, sizeof(ds_profile_db_3gpp_tech_params_type));

  ds_profile_db_gen_params_ptr =
        (ds_profile_db_gen_params_type*)modem_mem_alloc(
                                        sizeof(ds_profile_db_gen_params_type),
                                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_profile_db_gen_params_ptr == NULL)
  {
    goto func_exit;
  }

  memset(ds_profile_db_gen_params_ptr, 0, sizeof(ds_profile_db_gen_params_type));
  result_type = ds_profile_db_get_tech_params(*profiles,
                                              tech_type,
                                      (void *)ds_3gpp_tech_params_ptr,
                                                  ds_profile_db_gen_params_ptr);

  // F3 reduction: no need to report if result_type if failure

  subs_id =  dsumts_subs_mgr_get_subs_id_from_db_subs_id(
                                ds_profile_db_gen_params_ptr->subscription_id);

  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
     goto func_exit;
  }

  ret_val = ds_3gpp_profile_cache_entry_create(
            ds_profile_db_gen_params_ptr->profile_number,
            subs_id,
            !ds_profile_db_gen_params_ptr->persistent);

  /* Notify callback if present */
  if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr != NULL )
  {
    ds_3gpp_profile_def_prof_info.prof_ev_func_ptr(
       DB_CREATE_PROFILE_EVENT,
       ds_profile_db_gen_params_ptr->profile_number,
       subs_id);
  }

  if(ret_val)
  {
    ds_3gpp_profile_cache_write_readonly_gen_params(
                                                 ds_profile_db_gen_params_ptr);

    profile_number = ds_profile_db_gen_params_ptr->profile_number;

    result_type = ds_profile_db_get_changed_idents(*profiles,
                                                   DB_TECH_TO_TECHMASK(tech_type),
                                                   &ident_ptr,
                                                   &ident_count,
                                                   &gen_ident_ptr,
                                                   &gen_ident_count);

    if(result_type != DB_RESULT_SUCCESS)
    {
      DS_3GPP_MSG1_LOW("Cannot get the identifiers from Profile DB, DB Error %d",
                          result_type);
    }

    /*-------------------------------------------------------------------------
      Handle ident ptr for 3gpp common params
    --------------------------------------------------------------------------*/
    if(ident_ptr != NULL)
    {
      for(ident_index = 0; ident_index < ident_count; ident_index++)
      {
        cb_mask |= dsi_profile_3gpp_modem_write_ident_info(profile_number,
                                               * (ident_ptr + ident_index),
                                                 ds_3gpp_tech_params_ptr,
                                                 ds_profile_db_gen_params_ptr,
                                                 tech_mask,
                                                 apn_param_check,
                                                 NULL);
      }
      as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
    }

    /*-------------------------------------------------------------------------
      Handle gen ident ptr
    --------------------------------------------------------------------------*/
    if(gen_ident_ptr != NULL)
    {
      for(ident_index = 0; ident_index < gen_ident_count; ident_index++)
      {
        cb_mask |= dsi_profile_3gpp_modem_write_ident_info(profile_number,
                                               *(gen_ident_ptr + ident_index),
                                                 ds_3gpp_tech_params_ptr,
                                                 ds_profile_db_gen_params_ptr,
                                                 tech_mask,
                                                 apn_param_check,
                                                 NULL);
       }
      as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
    }

    /*-------------------------------------------------------------------------
      Get the tech_mask, If the profile is of EPC type (tech_mask ==
      EPC_PROFILE_TECH_MASK), fetch the tech common parameters for non 3gpp
      portions too
    --------------------------------------------------------------------------*/
    tech_mask = ds_profile_db_get_supported_tech_mask(*profiles);

    if(tech_mask == EPC_PROFILE_TECH_MASK)
    {
      tech_type = DB_TECH_COMMON;
      result_type = ds_profile_db_get_tech_params(*profiles,
                                                   tech_type,
                        (void *)&(ds_3gpp_tech_params_ptr->tech_common_params),
                                                      ds_profile_db_gen_params_ptr);

      if(result_type != DB_RESULT_SUCCESS)
      {
        DS_3GPP_MSG1_LOW("Cannot get the C profile params from DB, DB Error: %d",
                           result_type);
      }

      ds_profile_db_free_idents(ident_ptr);
      ds_profile_db_free_idents(gen_ident_ptr);

      result_type = ds_profile_db_get_changed_idents(*profiles,
                                                       DB_TECH_TO_TECHMASK(tech_type),
                                                       &ident_ptr,
                                                       &ident_count,
                                                       &gen_ident_ptr,
                                                       &gen_ident_count);

      if(result_type != DB_RESULT_SUCCESS)
      {
        DS_3GPP_MSG1_LOW("Cannot get the identifiers from Profile DB, DB Error: %d",
                            result_type);
      }

      /*----------------------------------------------------------------------
        Handle ident ptr for tech common params(non 3gpp part)
      -----------------------------------------------------------------------*/
      if(ident_ptr != NULL)
      {
        for(ident_index = 0; ident_index < ident_count; ident_index++)
        {
          cb_mask |= dsi_profile_3gpp_modem_write_ident_info(profile_number,
                                                *(ident_ptr + ident_index),
                                                  ds_3gpp_tech_params_ptr,
                                                  ds_profile_db_gen_params_ptr,
                                                  tech_mask,
                                                  apn_param_check,
                                                  NULL);
        }
      }
    }
  }

/*-------------------------------------------------------------------------------
Clean up function
-----------------------------------------------------------------------------*/
  func_exit:
  ds_profile_3gpp_dispatch_cb(profile_number, NULL, cb_mask, as_subs_id);

  if( ds_3gpp_tech_params_ptr == NULL ||
      ds_profile_db_gen_params_ptr == NULL ||
      result_type != DB_RESULT_SUCCESS )
  {
    DS_3GPP_ERROR_LOG_3_EX(subs_id, "ds_3gpp_profile_create_hdlr cleanup: techparam 0xx, "
                       "dbgenparam 0xx, result d",
                       ds_3gpp_tech_params_ptr,
                       ds_profile_db_gen_params_ptr,
                       result_type);
  }

     /*--------------------------------------------------------------------------
       Free the identifiers and managed profile
      --------------------------------------------------------------------------*/
    if (ds_3gpp_tech_params_ptr != NULL)
    {
      modem_mem_free(ds_3gpp_tech_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }

    if (ds_profile_db_gen_params_ptr != NULL)
    {
      modem_mem_free(ds_profile_db_gen_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }
     ds_profile_db_free_idents(ident_ptr);
     ds_profile_db_free_idents(gen_ident_ptr);
     ds_profile_db_free_profiles(profiles,profile_count);

} /*ds_3gpp_profile_create_hdlr*/

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
)
{
   ds_cmd_type                        *cmd_ptr = NULL;
   ds_3gpp_profile_db_param_chg_type  *param_chg_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_profile_db_param_chg_type));

  if((cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PROFILE_DB_PARAM_CHG;

  param_chg_ptr =
      (ds_3gpp_profile_db_param_chg_type*)cmd_ptr->cmd_payload_ptr;

  param_chg_ptr->user_data = user_data;
  param_chg_ptr->profile_num = profile_num;
  param_chg_ptr->subs_id = (uint8)subs_id;
  param_chg_ptr->mask = mask;

  ds_put_cmd_ext2(cmd_ptr);

}/*ds_3gpp_profile_post_param_chg_cmd_to_ds_task*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_MODIFY_CMD_HDLR

DESCRIPTION
   Posted by MH in response to profile_modify event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;
   uint16 profile_count

DEPENDENCIES
  None.

RETURN VALUE
  ds_profile_db_supported_tech_mask tech_mask = 0;
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_modify_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count
)
{
  uint16                                profile_number = 0;
  ds_profile_db_ident_type             *ident_ptr = NULL;
  ds_profile_db_ident_type             *gen_ident_ptr = NULL;
  uint16                                ident_count = 0;
  uint16                                gen_ident_count = 0;
  int                                   ident_index = 0;
  ds_profile_db_info_type               db_info_type_s;
  ds_profile_db_result_type             result_type = DB_RESULT_FAIL;
  ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr = NULL;
  ds_profile_db_gen_params_type        *ds_profile_db_gen_params_ptr = NULL;
  ds_profile_db_tech_type               tech_type = DB_TECH_3GPP;
  boolean                               apn_param_changed = FALSE;
  ds_umts_pdp_subs_e_type               subs_id
                                         = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE;
  boolean                               apn_param_check = TRUE;
  ds_umts_pdp_profile_type             *old_pdp_profile_data_ptr = NULL;
  ds_umts_pdp_profile_status_etype      result = DS_UMTS_PDP_FAIL;
  ds_umts_pdp_type_enum_type            old_pdp_type =  DS_UMTS_PDP_MAX;
  ds_profile_db_supported_tech_mask     tech_mask = 0;
  sys_modem_as_id_e_type as_subs_id = SYS_MODEM_AS_ID_NONE; 
  ds_3gpp_profile_cb_mask_e             cb_mask = DS_3GPP_PROFILE_NO_MASK;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
     Get the array of changed identifiers and for every changed identifier
     get the changed parameters
  -------------------------------------------------------------------------*/

  if(profiles == NULL)
  {
    goto func_exit;
  }

  ds_3gpp_tech_params_ptr = (ds_profile_db_3gpp_tech_params_type*)
                        modem_mem_alloc(sizeof(ds_profile_db_3gpp_tech_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_3gpp_tech_params_ptr == NULL)
  {
    goto func_exit;
  }

  memset(ds_3gpp_tech_params_ptr, 0, sizeof(ds_profile_db_3gpp_tech_params_type));

  ds_profile_db_gen_params_ptr = (ds_profile_db_gen_params_type*)
                        modem_mem_alloc(sizeof(ds_profile_db_gen_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_profile_db_gen_params_ptr == NULL)
  {
    goto func_exit;
  }
  memset(ds_profile_db_gen_params_ptr, 0, sizeof(ds_profile_db_gen_params_type));
  memset((void *)&db_info_type_s,0,sizeof(ds_profile_db_info_type));

  result_type = ds_profile_db_get_tech_params(*profiles,
                                               tech_type,
                                      (void *) ds_3gpp_tech_params_ptr,
                                                  ds_profile_db_gen_params_ptr);

  if(result_type != DB_RESULT_SUCCESS)
  {
    goto func_exit;
  }

  subs_id =  dsumts_subs_mgr_get_subs_id_from_db_subs_id(
                                ds_profile_db_gen_params_ptr->subscription_id);

  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
     goto func_exit;
  }

  profile_number = ds_profile_db_gen_params_ptr->profile_number;
  result_type = ds_profile_db_get_changed_idents(*profiles,
                                                 DB_TECH_TO_TECHMASK(tech_type),
                                                 &ident_ptr,
                                                 &ident_count,
                                                 &gen_ident_ptr,
                                                 &gen_ident_count);

  if(result_type != DB_RESULT_SUCCESS)
  {
    DS_3GPP_MSG1_LOW("Cannot get the identifiers from Profile DB, DB Error: %d",
                       result_type);
    goto func_exit;
  }

    old_pdp_profile_data_ptr = (ds_umts_pdp_profile_type*)
                        modem_mem_alloc(sizeof(ds_umts_pdp_profile_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
   Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(old_pdp_profile_data_ptr == NULL)
  {
    goto func_exit;
  }

  memset(old_pdp_profile_data_ptr, 0, sizeof(ds_umts_pdp_profile_type));

   result = ds_umts_get_pdp_profile_all_data_per_subs(profile_number,
                                                      subs_id,
                                                     old_pdp_profile_data_ptr);

   if(( result != DS_UMTS_PDP_SUCCESS ) ||
     (!old_pdp_profile_data_ptr->context.valid_flg))
   {
     DS_3GPP_ERROR_LOG_3_EX(subs_id, "Error in retrieving definition for prof number:d"
                        "result:d, valid_flag:d",
                        profile_number,result,
                       old_pdp_profile_data_ptr->context.valid_flg);
   }
   else
   {
     old_pdp_type = old_pdp_profile_data_ptr->context.pdp_type;
   }

  /*--------------------------------------------------------------------------
    Handle ident ptr for 3gpp common params
  ---------------------------------------------------------------------------*/
  if(ident_ptr != NULL)
  {
    for(ident_index = 0; ident_index < ident_count; ident_index++)
    {
      cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                                               profile_number,
                                             *(ident_ptr + ident_index),
                                               ds_3gpp_tech_params_ptr,
                                               ds_profile_db_gen_params_ptr,
                                               tech_mask,
                                               apn_param_check,
old_pdp_profile_data_ptr);
    }
    as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
  }

  /*-------------------------------------------------------------------------
    Handle gen ident ptr
  ---------------------------------------------------------------------------*/
  if(gen_ident_ptr != NULL)
  {
    for(ident_index = 0; ident_index < gen_ident_count; ident_index++)
    {
      cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                                               profile_number,
                                              *(gen_ident_ptr + ident_index),
                                               ds_3gpp_tech_params_ptr,
                                               ds_profile_db_gen_params_ptr,
                                               tech_mask,
                                               apn_param_check,
old_pdp_profile_data_ptr);
    }
    as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
  }


  /*---------------------------------------------------------------------------
    Get the tech_mask, If the profile is of EPC type(tech_mask ==
    EPC_PROFILE_TECH_MASK), fetch the tech common parameters for non 3gpp
    portions too
  --------------------------------------------------------------------------*/
  tech_mask = ds_profile_db_get_supported_tech_mask(*profiles);

  if(tech_mask == EPC_PROFILE_TECH_MASK)
  {
    tech_type = DB_TECH_COMMON;
    result_type = ds_profile_db_get_tech_params(
                        *profiles,
                        tech_type,
                        (void *)&(ds_3gpp_tech_params_ptr->tech_common_params),
                        ds_profile_db_gen_params_ptr);

    if(result_type != DB_RESULT_SUCCESS)
    {
      goto func_exit;
    }

    ds_profile_db_free_idents(ident_ptr);
    ds_profile_db_free_idents(gen_ident_ptr);

    result_type = ds_profile_db_get_changed_idents(*profiles,
                                                       DB_TECH_TO_TECHMASK(tech_type),
                                                       &ident_ptr,
                                                       &ident_count,
                                                       &gen_ident_ptr,
                                                       &gen_ident_count);

    if(result_type != DB_RESULT_SUCCESS)
    {
      DS_3GPP_MSG1_LOW ("Cannot get the identifiers from Profile DB. DB Result: %d",
                         result_type);
      goto func_exit;
    }

    /*-------------------------------------------------------------------------------
      Handle ident ptr for tech common params(non 3gpp part)
    -------------------------------------------------------------------------------*/
    if(ident_ptr != NULL)
    {
      for(ident_index = 0; ident_index < ident_count; ident_index++)
      {
        cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                      profile_number,
                     *(ident_ptr + ident_index),
                      ds_3gpp_tech_params_ptr,
                      ds_profile_db_gen_params_ptr,
                      tech_mask, 
                      apn_param_check,
                      old_pdp_profile_data_ptr);
      }
    }
  }

  /* Notify callback if present */
  if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr != NULL )
  {
    ds_3gpp_profile_def_prof_info.prof_ev_func_ptr(
       DB_MODIFY_PROFILE_EVENT,
       ds_profile_db_gen_params_ptr->profile_number,
       subs_id);
  }

  /*-------------------------------------------------------------------------
    Check if apn_param_changed has been set to TRUE, If it has been set
    post a cmd for 3gpp MH to handle it
    -------------------------------------------------------------------------*/
  apn_param_changed = dsi_profile_3gpp_get_apn_changed_param(subs_id);

  if( apn_param_changed )
  {
    ds_3gpp_profile_post_param_chg_cmd_to_ds_task(profile_number,
                                                  subs_id,
                                                  (void *)old_pdp_type,
                                                  DS_3GPP_PROFILE_APN_PARAM_CHG);
  }

  dsi_profile_3gpp_reset_apn_changed_param(subs_id);

  func_exit:
  ds_profile_3gpp_dispatch_cb(profile_number, old_pdp_profile_data_ptr, cb_mask, as_subs_id);

  if( ds_3gpp_tech_params_ptr == NULL ||
      ds_profile_db_gen_params_ptr == NULL ||
      result_type != DB_RESULT_SUCCESS )
  {
    DS_3GPP_ERROR_LOG_3_EX(subs_id, "ds_3gpp_profile_modify_hdlr cleanup: techparam 0xx, "
                       "dbgenparam 0xx, result d",
                       ds_3gpp_tech_params_ptr,
                       ds_profile_db_gen_params_ptr,
                       result_type);
  }

  /*--------------------------------------------------------------------------
     Free the identifiers and managed profile
  --------------------------------------------------------------------------*/
    if (old_pdp_profile_data_ptr != NULL)
    {
      modem_mem_free(old_pdp_profile_data_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }

    if (ds_3gpp_tech_params_ptr != NULL)
    {
      modem_mem_free(ds_3gpp_tech_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }

    if (ds_profile_db_gen_params_ptr != NULL)
    {
      modem_mem_free(ds_profile_db_gen_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }
    ds_profile_db_free_idents(ident_ptr);
    ds_profile_db_free_idents(gen_ident_ptr);
    ds_profile_db_free_profiles(profiles,profile_count);
} /*ds_3gpp_profile_modify_hdlr*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_RESET_HDLR

DESCRIPTION
   Posted by MH in response to profile_delete event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;
   uint16 profile_count

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_reset_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count
)
{
  ds_profile_db_result_type             result_type = DB_RESULT_FAIL;
  ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr = NULL;
  ds_profile_db_gen_params_type        *ds_profile_db_gen_params_ptr = NULL;
  ds_profile_db_tech_type               tech_type = DB_TECH_3GPP;
  ds_umts_pdp_profile_type             *ds_umts_pdp_profile_ptr = NULL;
  ds_profile_db_supported_tech_mask     tech_mask;
  ds_umts_pdp_subs_e_type               subs_id
                                         = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(profiles == NULL)
  {
    goto func_exit;
  }

  ds_3gpp_tech_params_ptr =
     (ds_profile_db_3gpp_tech_params_type*)modem_mem_alloc
                                  (sizeof(ds_profile_db_3gpp_tech_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_3gpp_tech_params_ptr == NULL)
  {
    goto func_exit;
  }
  memset(ds_3gpp_tech_params_ptr, 0, sizeof(ds_profile_db_3gpp_tech_params_type));

  ds_profile_db_gen_params_ptr =
     (ds_profile_db_gen_params_type*)modem_mem_alloc
                                   (sizeof(ds_profile_db_gen_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_profile_db_gen_params_ptr == NULL)
  {
    goto func_exit;
  }

  memset(ds_profile_db_gen_params_ptr, 0, sizeof(ds_profile_db_gen_params_type));
  result_type = ds_profile_db_get_tech_params(*profiles,tech_type,(void *)
                        ds_3gpp_tech_params_ptr,ds_profile_db_gen_params_ptr);

  if(result_type != DB_RESULT_SUCCESS)
  {
    DS_3GPP_MSG1_LOW("Cannot reset the cache entry. DB Result: %d", result_type);
    goto func_exit;
  }

  subs_id =  dsumts_subs_mgr_get_subs_id_from_db_subs_id
                               (ds_profile_db_gen_params_ptr->subscription_id);

  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
     DS_3GPP_MSG1_LOW("Invalid Subs id:%d",subs_id);
     goto func_exit;
  }

  ds_umts_pdp_profile_ptr =  ds_3gpp_profile_cache_convert_profile_db_params
                              ((void *)ds_3gpp_tech_params_ptr,
                               ds_profile_db_gen_params_ptr);

  if(ds_umts_pdp_profile_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("Cache ptr not populated");
    goto func_exit;
  }

  tech_mask = ds_profile_db_get_supported_tech_mask(*profiles);
  ds_3gpp_profile_cache_convert_profile_db_common_tech_params
                              ((void *)ds_3gpp_tech_params_ptr,
                               ds_umts_pdp_profile_ptr,tech_mask);

  func_exit:
  if( ds_3gpp_tech_params_ptr == NULL ||
      ds_profile_db_gen_params_ptr == NULL ||
      result_type != DB_RESULT_SUCCESS )
  {
    DS_3GPP_ERROR_LOG_3_EX(subs_id, "ds_3gpp_profile_reset_hdlr cleanup: techparam 0xx, "
                       "dbgenparam 0xx result = d",
                       ds_3gpp_tech_params_ptr,
                       ds_profile_db_gen_params_ptr,
                       result_type);
  }

    if (ds_3gpp_tech_params_ptr != NULL)
    {
      modem_mem_free(ds_3gpp_tech_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }

    if (ds_profile_db_gen_params_ptr != NULL)
    {
      modem_mem_free(ds_profile_db_gen_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }
    ds_profile_db_free_profiles(profiles,profile_count);

} /*ds_3gpp_profile_reset_hdlr*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_CLEAR_SUBS_HDLR

DESCRIPTION
   Posted by MH in response to profile_delete event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;
   uint16 profile_count

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_clear_subs_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count,
  ds_umts_pdp_subs_e_type               subs_id
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
     return;
  }
  ds_3gpp_profile_cache_entry_delete_all(subs_id);
} /* ds_3gpp_profile_clear_subs_hdlr */

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_DELETE_CMD_HDLR

DESCRIPTION
   Posted by MH in response to profile_delete event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;
   uint16 profile_count

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_delete_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count
)
{
  ds_profile_db_result_type             result_type = DB_RESULT_FAIL;
  ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr = NULL;
  ds_profile_db_gen_params_type        *ds_profile_db_gen_params_ptr = NULL;
  ds_profile_db_tech_type               tech_type = DB_TECH_3GPP;
  ds_umts_pdp_subs_e_type               subs_id
                                         = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(profiles == NULL)
  {
    goto func_exit;
  }

   ds_3gpp_tech_params_ptr =
      (ds_profile_db_3gpp_tech_params_type*)modem_mem_alloc(
                                   sizeof(ds_profile_db_3gpp_tech_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_3gpp_tech_params_ptr == NULL)
  {
    goto func_exit;
  }
  memset(ds_3gpp_tech_params_ptr, 0, sizeof(ds_profile_db_3gpp_tech_params_type));

  ds_profile_db_gen_params_ptr =
     (ds_profile_db_gen_params_type*)modem_mem_alloc(
                                  sizeof(ds_profile_db_gen_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_profile_db_gen_params_ptr == NULL)
  {
    goto func_exit;
  }
  memset(ds_profile_db_gen_params_ptr, 0, sizeof(ds_profile_db_gen_params_type));


  result_type = ds_profile_db_get_tech_params(*profiles,tech_type,(void *)
                        ds_3gpp_tech_params_ptr,ds_profile_db_gen_params_ptr);

  if(result_type != DB_RESULT_SUCCESS)
  {
    goto func_exit;
  }

  subs_id =  dsumts_subs_mgr_get_subs_id_from_db_subs_id(
                                ds_profile_db_gen_params_ptr->subscription_id);

  if(ds_umts_pdp_check_subscription_id(subs_id) ==
               DS_UMTS_PDP_INVALID_SUBS_ID)
  {
     DS_3GPP_ERROR_LOG_1_EX(subs_id, "Invalid Subs id:d",subs_id);
     goto func_exit;
  }

  ds_3gpp_profile_cache_entry_delete(
     ds_profile_db_gen_params_ptr->profile_number,
     subs_id,
     !ds_profile_db_gen_params_ptr->persistent);

  /* Notify callback if present */
  if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr != NULL )
  {
    ds_3gpp_profile_def_prof_info.prof_ev_func_ptr(
       DB_DELETE_PROFILE_EVENT,
       ds_profile_db_gen_params_ptr->profile_number,
       subs_id);
  }

  func_exit:
  if( ds_3gpp_tech_params_ptr == NULL ||
      ds_profile_db_gen_params_ptr == NULL ||
      result_type != DB_RESULT_SUCCESS )
  {
    DS_3GPP_ERROR_LOG_3 ("ds_3gpp_profile_delete_hdlr cleanup: techparam 0xx, "
                        "dbgenparam 0xx, result d",
                        ds_3gpp_tech_params_ptr,
                        ds_profile_db_gen_params_ptr,
                        result_type);
  }

    if (ds_3gpp_tech_params_ptr != NULL)
    {
      modem_mem_free(ds_3gpp_tech_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }

    if (ds_profile_db_gen_params_ptr != NULL)
    {
      modem_mem_free(ds_profile_db_gen_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
    }
    ds_profile_db_free_profiles(profiles,profile_count);

    DS_3GPP_MSG0_HIGH("Delete complete and successful");
} /*ds_3gpp_profile_delete_hdlr*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_COPY_PROFILE_CONTENTS

DESCRIPTION
  The utility function copies the profile parameters from from_profile to to_profile

PARAMETERS:
  to_subs_id: subscription id of the profile to be copied to
  to_profile_num: profile of the profile to be copied to
  from_subs_id: subscription id of the profile to be copied from
  from_profile_num: profile num of the profile to be copied from

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  profile copy successful
  FALSE: profile copy failed

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_profile_copy_profile_contents
(
  ds_umts_pdp_subs_e_type  to_subs_id,
  uint16                   to_profile_num,
  ds_umts_pdp_subs_e_type  from_subs_id,
  uint16                   from_profile_num
)
{
  boolean                     ret_val = FALSE;
  ds_profile_status_etype     profile_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_hndl_type        from_profile_handle = NULL;
  ds_profile_hndl_type        to_profile_handle = NULL;
  char                       *buffer_p = NULL;
  ds_profile_info_type        info = {0};
  ds_profile_identifier_type  ident;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DATA_3GPP_MSG4(MSG_LEGACY_HIGH,
                 "copy_profile_contents to_subs_id:%d to_profile_num:%d "
                 "from_subs_id:%d from_profile_num:%d",
                 to_subs_id, to_profile_num, from_subs_id, from_profile_num);

  if( (to_profile_num == from_profile_num) && (to_subs_id == from_subs_id) )
  {
    return FALSE;
  }

  buffer_p = (char*)modem_mem_alloc(DS_3GPP_PROFILE_MAX_PARAM_SIZE,
                                    MODEM_MEM_CLIENT_DATA);
  if( buffer_p == NULL )
  {
    return FALSE;
  }

  do
  {
    profile_status = ds_profile_begin_transaction_per_sub(
                         DS_PROFILE_TRN_READ,
                         DS_PROFILE_TECH_3GPP,
                         (ds_profile_num_type)from_profile_num,
                         dsumts_subs_mgr_get_ds_profile_subs_id(from_subs_id),
                         &from_profile_handle);
    if( profile_status != DS_PROFILE_REG_RESULT_SUCCESS )
    {
      ret_val = FALSE;
      break;
    }

    profile_status = ds_profile_begin_transaction_per_sub(
                         DS_PROFILE_TRN_RW,
                         DS_PROFILE_TECH_3GPP,
                         (ds_profile_num_type)to_profile_num,
                         dsumts_subs_mgr_get_ds_profile_subs_id(to_subs_id),
                         &to_profile_handle);
    if( profile_status != DS_PROFILE_REG_RESULT_SUCCESS )
    {
      ret_val = FALSE;
      break;
    }

    for(ident = DS_PROFILE_3GPP_PROFILE_PARAM_MIN;
        ident < DS_PROFILE_3GPP_PROFILE_PARAM_MAX;
        ident++ )
    {
      memset(buffer_p, 0, DS_3GPP_PROFILE_MAX_PARAM_SIZE);
      info.buf = (void*)buffer_p;
      info.len = DS_3GPP_PROFILE_MAX_PARAM_SIZE;
      /* Do not copy Profile Name */
      if( ident == DS_PROFILE_3GPP_PROFILE_PARAM_PROFILE_NAME )
      {
        continue;
      }

      if( ds_profile_get_param(from_profile_handle, ident, &info) ==
          DS_PROFILE_REG_RESULT_SUCCESS )
      {
	  /*-------------------------------------------------------------------
	   	Overwrite the pdp context number to be the same as the new profile
	  	number
	  ---------------------------------------------------------------------*/
        if(ident == DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_NUMBER)
        {
           memscpy(info.buf, info.len,(void *)&to_profile_num,sizeof(uint16));
        }

        profile_status = ds_profile_set_param(to_profile_handle,
                                              ident,
                                              &info);
      }
    }

    for(ident = DS_PROFILE_GENERAL_PROFILE_PARAM_IPV6_PREFIX_DELEGATION_FLAG;
        ident > DS_PROFILE_GENERAL_PROFILE_PARAM_SUBS_ID;
        ident-- )
    {
      memset(buffer_p, 0, DS_3GPP_PROFILE_MAX_PARAM_SIZE);
      info.buf = (void*)buffer_p;
      info.len = DS_3GPP_PROFILE_MAX_PARAM_SIZE;
      if( ds_profile_get_param(from_profile_handle, ident, &info) ==
          DS_PROFILE_REG_RESULT_SUCCESS )
      {
        profile_status = ds_profile_set_param(to_profile_handle,
                                              ident,
                                              &info);
      }
    }

    profile_status = ds_profile_end_transaction(to_profile_handle,
                                                DS_PROFILE_ACTION_COMMIT);
    if (profile_status != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      ret_val = FALSE;
      break;
    }

    profile_status = ds_profile_end_transaction(from_profile_handle,
                                                DS_PROFILE_ACTION_CANCEL);
    if (profile_status != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      ret_val = FALSE;
      break;
    }
  }while(0);

  /* Free buffer if allocated */
  if( buffer_p != NULL )
  {
    modem_mem_free(buffer_p, MODEM_MEM_CLIENT_DATA);
  }

  return TRUE;
} /* ds_3gpp_profile_copy_profile_contents */

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
)
{
  ds_profile_hndl_type     profile_handle = NULL;
  ds_profile_status_etype  profile_status = DS_PROFILE_REG_RESULT_FAIL;
  ds_profile_info_type     info = {0};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  profile_status = ds_profile_begin_transaction_per_sub(
                       DS_PROFILE_TRN_RW,
                       DS_PROFILE_TECH_3GPP,
                       (ds_profile_num_type)profile_num,
                       dsumts_subs_mgr_get_ds_profile_subs_id(subs_id),
                       &profile_handle);
  if( profile_status != DS_PROFILE_REG_RESULT_SUCCESS )
  {
    return FALSE;
  }

  info.buf = param_ptr;
  info.len = param_len;

  profile_status = ds_profile_set_param(profile_handle,ident,&info);
  if( profile_status != DS_PROFILE_REG_RESULT_SUCCESS )
  {
    return FALSE;
  }

  profile_status = ds_profile_end_transaction(profile_handle,
                                              DS_PROFILE_ACTION_COMMIT);
  if (profile_status != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    return FALSE;
  }

  return TRUE;
} /* ds_3gpp_profile_update_profile_param */

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
)
{
  boolean                              emb_def_prof_present = FALSE;
  boolean                              tet_def_prof_present = FALSE;
  uint16                               embedded_profile_num = DS_3GPP_DEF_EMBEDDED_PROFILE;
  uint16                               tethered_profile_num =  DS_3GPP_DEF_TETHERED_PROFILE;
  ds_umts_pdp_profile_status_etype     status = DS_UMTS_PDP_FAIL;
  uint16                               profile_num = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
    return;
  }

  /*Reset global cache and already read variable for nv refresh cases*/
  cached_embedded_call_profile_number[subs_id] = 0;
  embedded_call_profile_number_read[subs_id]  = FALSE;
  cached_tethered_call_profile_number[subs_id] = 0;
  tethered_call_profile_number_read[subs_id] = FALSE;
  /*---------------------------------------------------------------------------
     Read the embedded/tethered default profile numbers from the EFS
     IF the values don't exist init them to 1

   Now check if these profiles are present in the cache and if they were
   initialized as part of powerup event, If they are not present update both
   the values to 1 and invoke Profile DB to create the profile
  ---------------------------------------------------------------------------*/
   status = ds_umts_get_default_profile_number_internal(
      DS_UMTS_EMBEDDED_PROFILE_FAMILY,subs_id, &embedded_profile_num);

   if (status != DS_UMTS_PDP_SUCCESS)
   {
      embedded_profile_num = DS_3GPP_DEF_EMBEDDED_PROFILE;
      cached_embedded_call_profile_number[subs_id] = DS_3GPP_DEF_EMBEDDED_PROFILE;
      embedded_call_profile_number_read[subs_id]  = TRUE;
   }

   status = ds_umts_get_default_profile_number_internal(
      DS_UMTS_TETHERED_PROFILE_FAMILY, subs_id, &tethered_profile_num);

   if (status != DS_UMTS_PDP_SUCCESS)
   {
      tethered_profile_num = DS_3GPP_DEF_TETHERED_PROFILE;
      cached_tethered_call_profile_number[subs_id] = DS_3GPP_DEF_TETHERED_PROFILE;
      tethered_call_profile_number_read[subs_id] = TRUE;
   }

   emb_def_prof_present = ds_3gpp_profile_cache_check_if_default_profile_exists
      (embedded_profile_num, subs_id);

   if (!emb_def_prof_present)
   {
      cached_embedded_call_profile_number[subs_id] = DS_3GPP_DEF_EMBEDDED_PROFILE;
      embedded_call_profile_number_read[subs_id]  = TRUE;
   }

   tet_def_prof_present = ds_3gpp_profile_cache_check_if_default_profile_exists
                     (tethered_profile_num, subs_id);

   if (!tet_def_prof_present)
   {
      cached_tethered_call_profile_number[subs_id] = DS_3GPP_DEF_TETHERED_PROFILE;
      tethered_call_profile_number_read[subs_id] = TRUE;
   }

  DS_3GPP_MSG3_HIGH("Profile setup: subsid = %d embedded = %d tethered = %d",
                    subs_id,
                    cached_embedded_call_profile_number[subs_id],
                    cached_tethered_call_profile_number[subs_id]);

   /*----------------------------------------------------------------------------
     At this point, also check if DS_3GPP_DEF_EMBEDDED_PROFILE and
     DS_3GPP_DEF_TETHERED_PROFILE are present. Only if it is not present
     do attempt to create a profile
     ----------------------------------------------------------------------------*/

   if((!emb_def_prof_present) &&
      (ds_3gpp_profile_cache_is_entry_valid(DS_3GPP_DEF_EMBEDDED_PROFILE, subs_id)))
   {
     emb_def_prof_present = TRUE;
   }

   if((!tet_def_prof_present) &&
      (ds_3gpp_profile_cache_is_entry_valid(DS_3GPP_DEF_TETHERED_PROFILE, subs_id)))
   {
     tet_def_prof_present = TRUE;
   }

   if ((!emb_def_prof_present) || (!tet_def_prof_present))
   {
     if (emb_def_prof_present)
     {
       profile_num = DS_3GPP_DEF_TETHERED_PROFILE;
     }
     else
     {
       profile_num = DS_3GPP_DEF_EMBEDDED_PROFILE;
     }

     if(DS_UMTS_PDP_SUCCESS != ds_3gpp_profile_create_profile(subs_id,
                                                              TRUE,
                                                              profile_num))
     {
        DS3GPP_ASSERT(0);
        return;
     }
   }
} /* ds_3gpp_profile_setup_default_profiles */

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_CREATE_PROFILE

DESCRIPTION
   Creates the profile num passed in params in profile db

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
)
{
  ds_profile_db_result_type            status_type = DB_RESULT_FAIL;
  ds_umts_pdp_profile_status_etype     status = DS_UMTS_PDP_SUCCESS;
  ds_profile_db_profile_managed_type  *prf_ptr = NULL;
  ds_profile_db_creation_params_type   params;
  ds_3gppi_local_err_val_e_type        local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto clean_up;
  }

  /* Create profiles*/
      ds_profile_db_init_creation_params(&params);
      params.subscription_id = (ds_profile_db_subs_type)(subs_id+1);
    prf_ptr = ds_profile_db_alloc(DB_TECH_TO_TECHMASK(DS_PROFILE_TECH_3GPP),
                                  params.subscription_id);

  if (prf_ptr == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto clean_up;
  }

  params.persistent = is_persistent;
      /*----------------------------------------------------------------------
  Always Create 
      -----------------------------------------------------------------------*/
  params.profile_num = profile_num;

  status_type = ds_profile_db_create(&params, prf_ptr);

  /*----------------------------------------------------------------- 
     Retry with persistence = FALSE if File access error
  ----------------------------------------------------------------*/
  if (status_type != DB_RESULT_SUCCESS)
    {
      status_type = ds_profile_db_dealloc(prf_ptr);

      if (status_type != DB_RESULT_SUCCESS)
      {
        DS3GPP_ASSERT(0);
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        goto clean_up;
      }

       ds_profile_db_init_creation_params(&params);
       params.subscription_id = (ds_profile_db_subs_type)(subs_id+1);
       params.profile_num = profile_num;

        prf_ptr = ds_profile_db_alloc(DB_TECH_TO_TECHMASK(DS_PROFILE_TECH_3GPP),
                                      params.subscription_id);
        params.persistent = FALSE;
        status_type = ds_profile_db_create(&params, prf_ptr);
      }

         if (status_type != DB_RESULT_SUCCESS)
         {
            DS3GPP_ASSERT(0);
            local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
            goto clean_up;
         }

clean_up:
  if (prf_ptr != NULL)
  {
         status_type = ds_profile_db_dealloc(prf_ptr);
         if (status_type != DB_RESULT_SUCCESS)
         {
            DS3GPP_ASSERT(0);
            status = DS_UMTS_PDP_FAIL;
         }
      }

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE )
      {
    DS_3GPP_MSG1_HIGH("ds_3gpp_profile_create_default_profile err %d",
                      local_err);
    status = DS_UMTS_PDP_FAIL;
   }

  return status;
} /* ds_3gpp_profile_create_profile */

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_POWER_UP_CMD_HDLR

DESCRIPTION
   Posted by MH in response to power_up event received from Profile DB

PARAMETERS:
   ds_profile_db_profile_managed_type* profiles
   ds_profile_db_tech_type tech_type;
   ds_profile_db_3gpp_tech_params_type *ds_3gpp_tech_params_p;
   ds_profile_db_gen_params_type *ds_profile_db_gen_params_p;
   uint16 profile_count

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_power_up_hdlr
(
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count,
  ds_umts_pdp_subs_e_type               subs_id
)
{
  ds_profile_db_profile_managed_type  **temp_profiles = profiles;
  ds_profile_db_ident_type             *ident_ptr = NULL;
  ds_profile_db_ident_type             *gen_ident_ptr = NULL;
  uint16                                ident_count = 0;
  uint16                                gen_ident_count = 0;
  int                                   ident_index = 0;
  uint16                                profile_number = 0;
  int                                   boot_up_profile_index = 0;
  boolean                               ret_val = FALSE;
  ds_profile_db_result_type             result_type = DB_RESULT_FAIL;
  ds_profile_db_3gpp_tech_params_type  *ds_3gpp_tech_params_ptr = NULL;
  ds_profile_db_gen_params_type        *ds_profile_db_gen_params_ptr = NULL;
  ds_profile_db_supported_tech_mask     tech_mask = 0;
  ds_profile_db_tech_type               tech_type = DB_TECH_3GPP;
  boolean                               apn_param_check = FALSE;
  sys_modem_as_id_e_type as_subs_id = SYS_MODEM_AS_ID_NONE; 
  ds_3gpp_profile_cb_mask_e             cb_mask = DS_3GPP_PROFILE_NO_MASK;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_umts_pdp_check_subscription_id(subs_id) == DS_UMTS_PDP_INVALID_SUBS_ID)
  {
    ds_profile_db_free_profiles(profiles,profile_count); 
    return;
  }

  if (profile_count >
       (DS_UMTS_MAX_PDP_PROFILE_NUM - DS_UMTS_MAX_PDP_TRANSIENT_PROFILE_NUM))
  {
     DS_3GPP_MSG2_HIGH("profile_count: %d, max_supported: %d",profile_count,
                       DS_UMTS_MAX_PDP_PROFILE_NUM -
                       DS_UMTS_MAX_PDP_TRANSIENT_PROFILE_NUM);
  }

  ds_3gpp_tech_params_ptr =
    (ds_profile_db_3gpp_tech_params_type*)modem_mem_alloc(
                                   sizeof(ds_profile_db_3gpp_tech_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_3gpp_tech_params_ptr == NULL)
  {
    goto func_exit;
  }

  ds_profile_db_gen_params_ptr =
    (ds_profile_db_gen_params_type*)modem_mem_alloc(
                                  sizeof(ds_profile_db_gen_params_type),
                        MODEM_MEM_CLIENT_DATA);

  /*-------------------------------------------------------------------------
    Allocate and Initialize memory before filling in the report
  -------------------------------------------------------------------------*/
  if(ds_profile_db_gen_params_ptr == NULL)
  {
    goto func_exit;
  }

  for(boot_up_profile_index = 0;boot_up_profile_index < profile_count;
      boot_up_profile_index++, temp_profiles++)
  {
    tech_type = DB_TECH_3GPP;
    tech_mask = 0;
    ident_count = 0;
    gen_ident_count = 0;
    memset((void *)ds_3gpp_tech_params_ptr,0,
                   sizeof(ds_profile_db_3gpp_tech_params_type));

    memset((void *)ds_profile_db_gen_params_ptr,0,
                  sizeof(ds_profile_db_gen_params_type));


    tech_type = DB_TECH_3GPP;

    /*------------------------------------------------------------------------
     Now query the Profile DB to get the C specific structures and thereby get
     the profile number to update the cache
    --------------------------------------------------------------------------*/
    result_type = ds_profile_db_get_tech_params(*temp_profiles,tech_type,(void *)
                    ds_3gpp_tech_params_ptr,ds_profile_db_gen_params_ptr);

    if(result_type != DB_RESULT_SUCCESS)
    {
       continue;
    }

    if (dsumts_subs_mgr_get_subs_id_from_db_subs_id(ds_profile_db_gen_params_ptr->subscription_id) != subs_id)
    {
       DS_3GPP_ERROR_LOG_2_EX(subs_id, "Subscription ID in profile does not match"
                          "Subs for event: d, profile subs_id: d",subs_id,
                          ds_profile_db_gen_params_ptr->subscription_id - 1);
       continue;
    }

    ret_val = ds_3gpp_profile_cache_entry_create(
               ds_profile_db_gen_params_ptr->profile_number,
               dsumts_subs_mgr_get_subs_id_from_db_subs_id(ds_profile_db_gen_params_ptr->subscription_id),
               !ds_profile_db_gen_params_ptr->persistent);

    if(ret_val)
    {
      ds_3gpp_profile_cache_write_readonly_gen_params
                           (ds_profile_db_gen_params_ptr);

      profile_number = ds_profile_db_gen_params_ptr->profile_number;

      result_type = ds_profile_db_get_changed_idents(*temp_profiles,
                                                     DB_TECH_TO_TECHMASK(tech_type),
                                                     &ident_ptr,
                                                     &ident_count,
                                                     &gen_ident_ptr,
                                                     &gen_ident_count);

      if(result_type != DB_RESULT_SUCCESS)
      {
        continue;
      }

      /*-------------------------------------------------------------------------------
        Handle ident ptr for 3gpp common params
        -------------------------------------------------------------------------------*/
      if(ident_ptr != NULL)
      {
        for(ident_index = 0; ident_index < ident_count; ident_index++)
        {
          cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                        profile_number,
                       *(ident_ptr + ident_index),
                        ds_3gpp_tech_params_ptr,
                        ds_profile_db_gen_params_ptr,
                        tech_mask, 
                        apn_param_check,
                        NULL);
        }
        as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
        ds_profile_db_free_idents(ident_ptr);
        ident_ptr = NULL;
      }

        /*-------------------------------------------------------------------------------
        Handle gen ident ptr
        -------------------------------------------------------------------------------*/

      if(gen_ident_ptr != NULL)
      {
        for(ident_index = 0; ident_index < gen_ident_count; ident_index++)
        {
          cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                        profile_number,
                       *(gen_ident_ptr + ident_index),
                        ds_3gpp_tech_params_ptr,
                        ds_profile_db_gen_params_ptr,
                        tech_mask,
                        apn_param_check,
                        NULL);
        }
        as_subs_id = (sys_modem_as_id_e_type)(ds_profile_db_gen_params_ptr->subscription_id - 1);
        ds_profile_db_free_idents(gen_ident_ptr);
        gen_ident_ptr = NULL;
      }
      /*---------------------------------------------------------------------------------
        Get the tech_mask, If the profile is of EPC type(tech_mask == 7), fetch the tech
        common parameters for non 3gpp portions too
        -------------------------------------------------------------------------------------------*/
      tech_mask = ds_profile_db_get_supported_tech_mask(*temp_profiles);

      if(tech_mask == EPC_PROFILE_TECH_MASK)
      {
        tech_type = DB_TECH_COMMON;
        result_type = ds_profile_db_get_tech_params(*temp_profiles,tech_type,(void *)
                                  &(ds_3gpp_tech_params_ptr->tech_common_params),
                                                      ds_profile_db_gen_params_ptr);

        if(result_type != DB_RESULT_SUCCESS)
        {
          continue;
        }


        result_type = ds_profile_db_get_changed_idents(*temp_profiles,
                                                       DB_TECH_TO_TECHMASK(tech_type),
                                                       &ident_ptr,
                                                       &ident_count,
                                                       &gen_ident_ptr,
                                                       &gen_ident_count);


        if(result_type != DB_RESULT_SUCCESS)
        {
          continue;
        }

        /*-------------------------------------------------------------------------------
          Handle ident ptr for tech common params(non 3gpp part)
        -------------------------------------------------------------------------------*/
        if(ident_ptr != NULL)
        {
          for(ident_index = 0; ident_index < ident_count; ident_index++)
          {
            cb_mask |= dsi_profile_3gpp_modem_write_ident_info(
                           profile_number,
                          *(ident_ptr + ident_index),
                           ds_3gpp_tech_params_ptr,
                           ds_profile_db_gen_params_ptr,
                           tech_mask, 
                           apn_param_check,
                           NULL);
           }
           ds_profile_db_free_idents(ident_ptr);
           ident_ptr = NULL;
        }
      }
    }
    else
    {
      continue;
    }
    ds_profile_3gpp_dispatch_cb(profile_number, NULL, cb_mask, as_subs_id);
  }

  ds_3gpp_profile_setup_default_profiles(subs_id);

func_exit:
  if( ds_3gpp_tech_params_ptr == NULL ||
      ds_profile_db_gen_params_ptr == NULL ||
      result_type != DB_RESULT_SUCCESS )
  {
    DS_3GPP_ERROR_LOG_3_EX(subs_id, "ds_3gpp_profile_power_up_hdlr cleanup: techparam 0xx,"
                       " dbgenparam 0xx, result d",
                       ds_3gpp_tech_params_ptr,
                       ds_profile_db_gen_params_ptr,
                       result_type);
  }

  if (ds_3gpp_tech_params_ptr != NULL)
  {
    modem_mem_free(ds_3gpp_tech_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
  }

  if (ds_profile_db_gen_params_ptr != NULL)
  {
    modem_mem_free(ds_profile_db_gen_params_ptr, MODEM_MEM_CLIENT_DATA_CRIT);
  }

   ds_profile_db_free_profiles(profiles,profile_count); 
   
} /*ds_3gpp_profile_power_up_hdlr*/

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_EVENT_CB

DESCRIPTION
  Called by Profile DB to notify any event.

PARAMETERS:
  ds_profile_db_event_type event,
  ds_profile_db_profile_managed_type* profiles,
  uint16 profiles_count,
  void* user_data

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_event_cb
(
  ds_profile_db_event_type              event_name,
  ds_profile_db_profile_managed_type  **profiles,
  uint16                                profile_count,
  void                                 *user_data,
  ds_profile_db_subs_type               subs
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG4_HIGH("3gpp_profile_event_cb: event = %d, profile ptr = 0x%x"
                    "pf cnt = %d subs = %d",
                    event_name, profiles, profile_count, subs);

  /*-------------------------------------------------------------------------
   Check the profile count for all events except for powerup event
   -------------------------------------------------------------------------*/
  if(event_name != DB_POWER_UP_EVENT &&
     event_name != DB_REFRESH_PROFILE_EVENT)
  {
    if(profile_count > 1)
    { 
      ds_profile_db_free_profiles(profiles,profile_count); 
      return;
    }
  }

  switch(event_name)
  {
    case DB_CREATE_PROFILE_EVENT:
     ds_3gpp_profile_create_hdlr(profiles,profile_count);
     break;

    case DB_MODIFY_PROFILE_EVENT:
     ds_3gpp_profile_modify_hdlr(profiles,profile_count);
     break;

    case DB_DELETE_PROFILE_EVENT:
     ds_3gpp_profile_delete_hdlr(profiles,profile_count);
     break;

    case DB_POWER_UP_EVENT:
     ds_3gpp_profile_power_up_hdlr(profiles,profile_count,
                                   dsumts_subs_mgr_get_subs_id_from_db_subs_id(subs));
     break;

    case DB_RESET_PROFILE_EVENT:
      ds_3gpp_profile_reset_hdlr(profiles,profile_count);
      break;

   case DB_REFRESH_PROFILE_EVENT:
     ds_3gpp_profile_clear_subs_hdlr(profiles,profile_count,
                                     dsumts_subs_mgr_get_subs_id_from_db_subs_id(subs));
     ds_3gpp_profile_power_up_hdlr(profiles,profile_count,
                                   dsumts_subs_mgr_get_subs_id_from_db_subs_id(subs));
     break;

    default:
	 ds_profile_db_free_profiles(profiles,profile_count); 
	 
     break;
  }

  return;
} /*ds_3gpp_profile_event_cb*/

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
)
{
  ds_profile_db_result_type  result_type = DB_RESULT_FAIL;
  ds_profile_status_etype    status = DS_PROFILE_REG_RESULT_FAIL;
  uint32                     profile_family = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  profile_family = family;

  status = dsi_profile_3gpp_get_default_profile_per_subs(
             profile_family, (ds_profile_subs_etype)subs, num);

  if (status == DS_UMTS_PDP_FAIL || DS_UMTS_PDP_INVALID_PROFILE_NUM == status)
  {
    result_type = DB_RESULT_FAIL;
  }
  else
  {
    result_type = DB_RESULT_SUCCESS;
  }
  return result_type;
} /*ds_3gpp_profile_get_default_profile_num_per_subs*/

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
)
{
  ds_profile_db_result_type         result_type = DB_RESULT_FAIL;
  ds_umts_pdp_profile_status_etype  status = DS_UMTS_PDP_FAIL;
  uint32                            profile_family = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  profile_family = family;
  status = dsi_profile_3gpp_set_default_profile_number_per_subs(
             profile_family,
             dsumts_subs_mgr_get_subs_id_from_db_subs_id(subs),
             num);

  if (status == DS_UMTS_PDP_FAIL || DS_UMTS_PDP_INVALID_PROFILE_NUM == status)
  {
    result_type = DB_RESULT_FAIL;
  }
  else
  {
    result_type = DB_RESULT_SUCCESS;
  }
  return result_type;
} /*ds_3gpp_profile_set_default_profile_num_per_subs*/

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
void ds_3gpp_profile_api_init( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds_3gpp_callback_id = ds_profile_db_register_for_notifications_MH
               (DB_UNSPECIFIED_PROFILE_NUM,(DB_TECH_TO_TECHMASK(DB_TECH_3GPP)|
                DB_TECH_TO_TECHMASK(DB_TECH_COMMON)),TRUE,
                ds_3gpp_profile_event_cb,
                NULL);

  if(ds_3gpp_callback_id == 0)
  {
    DS_3GPP_ERROR_FATAL("Unable to register with Profile DB");
  }

  return;
} /*ds_3gpp_profile_api_init*/

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
)
{
  ds_profile_db_result_type  ret_val = DB_RESULT_SUCCESS;
  validate_3gpp_fn_type      validate_fn = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(info == NULL)
  {
    ret_val = DB_RESULT_FAIL;
    return ret_val;
  }

  validate_fn = dsi_profile_3gpp_modem_get_validation_func(ident);

  if(validate_fn != NULL)
  {
    ret_val = validate_fn(ident,info,profile_params);
  }

  return ret_val;
} /* ds_3gpp_profile_validation_func */

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
)
{
  ds_profile_db_result_type  ret_val = DB_RESULT_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ret_val = ds_3gpp_profile_validate_gen_params(profile);

  return ret_val;
} /* ds_3gpp_profile_validation_gen */

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
)
{
  ds_profile_db_result_type  ret_val = DB_RESULT_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ret_val = ds_3gpp_profile_cache_convert_to_profile_db(num,
                                                        subs_id,
                                                        tech_params,gen_params);
  return ret_val;
} /* ds_3gpp_profile_get */

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
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds_3gpp_profile_cache_get_default_values(tech_profile_ptr);
  return;
} /* ds_3gpp_profile_get_default_values */

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
ds_profile_db_subs_type ds_3gpp_profile_get_default_sub_id( void )
{
  ds_umts_pdp_subs_e_type  data_subs = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE;
/*- - - - - - - - - - - -  - - - - - - - - - -  - - - - - - - - - - - - - -*/
  data_subs = dsumts_subs_mgr_get_subs_id(ds3g_get_ps_subs_id());

  if (data_subs ==DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_NONE)
  {
    data_subs = DS_UMTS_PDP_ACTIVE_SUBSCRIPTION_1;
  }

  DS_3GPP_MSG1_HIGH("Data Subscription is : %d", data_subs);
  return (ds_profile_db_subs_type)(data_subs+1);
} /* ds_3gpp_profile_get_default_sub_id */

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
)
{
  ds_profile_db_result_type  ret_val = DB_RESULT_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret_val = ds_3gpp_profile_cache_is_epc_tech_param_set(num,
                             dsumts_subs_mgr_get_subs_id_from_db_subs_id(subs_id),
                             ident, is_tech_param_set);
  return ret_val;
} /* ds_3gpp_profile_is_epc_tech_param_set */

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_DELETE_ATTACH_PROFILES

DESCRIPTION
  This function when invoked reads the attach profiles configured and deletes all of them
  If the attach profile happens to be a default profile, then the contents of the profile
  will be reset

PARAMETERS:
   subs_id

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_delete_attach_profiles
(
  sys_modem_as_id_e_type subs_id
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds_dsd_delete_all_attach_profiles(subs_id);
  return;

}/*ds_3gpp_profile_delete_attach_profiles*/

/*===========================================================================
FUNCTION DS_3GPP_RESET_ATTACH_PROFILE_LIST_TO_DEFAULT_PROFILE

DESCRIPTION
  This function when invoked resets the attach profile list i.e
  sets the new attach profile list to default embedded profile number.
  This will be picked up in the next attach procedure
 
  This api will also accept a fn ptr which will be called when
  updating the attach profile list is successful
    
PARAMETERS: 
  cb_fn_ptr 
  subs_id
 
DEPENDENCIES
  None.
  
RETURN VALUE 
  boolean
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
void ds_3gpp_reset_attach_profile_list_to_default_profile
(
   ds_3gpp_reset_attach_profile_list_cb_fptr cb_fn_ptr,
   sys_modem_as_id_e_type                    subs_id
)
{
  int16                                 ret_val = -1;
  int16                                 ps_errno = -1;
  ps_sys_3gpp_lte_attach_pdn_list_type  attach_list = {0};
  ds_profile_num_type                   def_emb_profile = 0;
  ds_profile_status_etype               profile_status = DS_PROFILE_REG_RESULT_FAIL;
  uint8                                 dbg_err_val = 0;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {
    profile_status = dsi_profile_3gpp_get_default_profile(DS_UMTS_EMBEDDED_PROFILE_FAMILY,
                                                          &def_emb_profile, subs_id);

    if(profile_status != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      dbg_err_val = 1;
      break;
    }

    attach_list.attach_pdn_profile_list_num = 1;
    attach_list.attach_pdn_profile_list[0] = def_emb_profile;
     
    ret_val = ds_dsd_apm_set_attach_pdn_list((void *)&attach_list,
                                             subs_id,
                                             &ps_errno,
                                             cb_fn_ptr);

    if(ret_val != 0)
    {              
      dbg_err_val = 2;
      break;
    }

  }while(0); 

  if(dbg_err_val != 0)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_3gpp_reset_attach_profile_list_to_default_profile err:d",
                       dbg_err_val);
  }
}/*ds_3gpp_reset_attach_profile_list_to_default_profile*/

/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_HANDLE_PROFILE_EVENTS

DESCRIPTION   This callback function will be called when profile is created,
              modified or deleted.

PARAMETERS    profile_change_type: Create, Modify or Delete type
              profile_num: profile number
              subs_id: subscription id

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_profile_handle_profile_events
(
  ds_profile_db_event_type  profile_event,
  uint16                    profile_num,
  ds_umts_pdp_subs_e_type   subs_id
)
{
  ds_umts_pdp_profile_type  profile_info = {0};
  ds_umts_pdp_profile_type  ap_def_profile_info = {0};
  uint16                    ap_def_profile_num = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Ignore if there is no AP default profile
  -----------------------------------------------------------------------*/
  if( dsi_profile_3gpp_get_ap_def_profile_per_subs(DB_EMBEDDED_PROFILE_FAMILY,
                                                   subs_id,
                                                   &ap_def_profile_num)
        != DS_PROFILE_REG_RESULT_SUCCESS )
  {
    return;
  }

  DS_3GPP_MSG3_HIGH("handle_profile_events evt:%d profile_num:%d subs_id:%d",
                    profile_event, profile_num, subs_id);

  if( (profile_event  == DB_CREATE_PROFILE_EVENT) ||
      (profile_event  == DB_MODIFY_PROFILE_EVENT) )
  {
    /*-----------------------------------------------------------------------
      For create and modify events, ignore if the profile event is for
      AP default profile num
    -----------------------------------------------------------------------*/
    if( ap_def_profile_num == profile_num )
    {
      return;
    }

    /*-----------------------------------------------------------------------
      Get the profile contents for both the profiles
    -----------------------------------------------------------------------*/
    if( DS_UMTS_PDP_SUCCESS !=
          ds_umts_get_pdp_profile_all_data_per_subs(profile_num,
                                                    subs_id,
                                                    &profile_info) )
    {
      return;
    }
    if( DS_UMTS_PDP_SUCCESS !=
          ds_umts_get_pdp_profile_all_data_per_subs(ap_def_profile_num,
                                                    subs_id,
                                                    &ap_def_profile_info) )
    {
      return;
    }

    /*-----------------------------------------------------------------------
      Handle only if profile APN name matches
    -----------------------------------------------------------------------*/
    if( strncasecmp((char*)ap_def_profile_info.context.apn,
                    (char*)profile_info.context.apn,
                    DS_UMTS_MAX_APN_STRING_LEN+1) == 0 )
    {
      ds_3gpp_profile_copy_profile_contents(subs_id,
                                            ap_def_profile_num,
                                            subs_id,
                                            profile_num);
    }
  }
  else if( profile_event  == DB_DELETE_PROFILE_EVENT )
  {
    /*-----------------------------------------------------------------------
      For delete events, ignore if the profile event is NOT for
      AP default profile num
    -----------------------------------------------------------------------*/
    if( ap_def_profile_num != profile_num )
    {
      return;
    }

    /*-----------------------------------------------------------------------
      Clear AP default profiles
    -----------------------------------------------------------------------*/
    dsi_profile_3gpp_clear_ap_def_profile_number_per_subs(
       DS_PROFILE_EMBEDDED_PROFILE_FAMILY,
       subs_id);

    dsi_profile_3gpp_clear_ap_def_profile_number_per_subs(
       DS_PROFILE_TETHERED_PROFILE_FAMILY,
       subs_id);

    /* De-Register for profile change callback */
    if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr != NULL )
    {
      ds_3gpp_profile_def_prof_info.prof_ev_func_ptr = NULL;
    }
  }

  return;
} /* ds_3gpp_profile_handle_profile_events */

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
)
{
  ds_profile_status_etype  profile_status;
  uint16                   ap_def_profile_num = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG1_HIGH("ds_3gpp_profile_reset_ap_default_apn subs_id:%d",subs_id);


  if( dsi_profile_3gpp_get_ap_def_profile_per_subs(DB_EMBEDDED_PROFILE_FAMILY,
                                                   subs_id,
                                                   &ap_def_profile_num)
        != DS_PROFILE_REG_RESULT_SUCCESS )
  {
    ap_def_profile_num = 0;
  }

  /*-----------------------------------------------------------------------
    Clear AP default profiles
  -----------------------------------------------------------------------*/
  dsi_profile_3gpp_clear_ap_def_profile_number_per_subs(
     DS_PROFILE_EMBEDDED_PROFILE_FAMILY,
     subs_id);

  dsi_profile_3gpp_clear_ap_def_profile_number_per_subs(
     DS_PROFILE_TETHERED_PROFILE_FAMILY,
     subs_id);

  if( ap_def_profile_num != 0)
  {
    /* Delete modem profile */
    profile_status = ds_profile_delete_per_sub(DS_PROFILE_TECH_3GPP,
                                               ap_def_profile_num,
                         dsumts_subs_mgr_get_ds_profile_subs_id(subs_id));

    DS_3GPP_MSG2_HIGH("Deleting profile_num:%d status:%d",
                      ap_def_profile_num, profile_status);

    if( profile_status != DS_PROFILE_REG_RESULT_SUCCESS )
    {
      return FALSE;
    }
  }

  /* De-Register for profile change callback */
  if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr != NULL )
  {
    ds_3gpp_profile_def_prof_info.prof_ev_func_ptr = NULL;
  }

  return TRUE;
} /* ds_3gpp_profile_reset_ap_default_apn */
/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_CREATE_MODEM_DEF_PROFILE

DESCRIPTION   This function creates a modem_def_profile.

PARAMETERS    device_settings_info_p = pointer to device settings info

DEPENDENCIES  None

RETURN VALUE  TRUE, if successful
              FALSE, if failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_profile_create_modem_def_profile
(
  ds3gdevmgr_device_settings_info_type  *device_settings_info_p
)
{
  byte                           internet_apn[DS_SYS_MAX_APN_LEN];
  byte                           old_internet_apn[DS_SYS_MAX_APN_LEN];
  boolean                        internet_apn_configured = FALSE;
  ds_3gppi_local_err_val_e_type  local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  boolean                        ret_val = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset((void*)internet_apn, 0, sizeof(internet_apn));
  memset((void*)old_internet_apn, 0, sizeof(old_internet_apn));

  do
  {
    if (NULL == device_settings_info_p)
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }

    /*-------------------------------------------------------------- 
      Fetch current and old internet apn name
      ------------------------------------------------------------*/
    strlcpy((char*)internet_apn,
            (char*)device_settings_info_p->apn_info.
              curr_apn_info[PS_SYS_APN_TYPE_INTERNET].apn_name,
            sizeof(internet_apn));

    strlcpy((char*)old_internet_apn,
            (char*)device_settings_info_p->apn_info.
              prev_apn_info[PS_SYS_APN_TYPE_INTERNET].apn_name,
            sizeof(old_internet_apn));

    DATA_3GPP_MSG_SPRINTF_2(MSG_LEGACY_HIGH,
                            "internet APN: %s old internet APN: %s",
                            internet_apn, old_internet_apn);

   /*--------------------------------------------------------------
      Set the AP default APN in all cases except when they are the
      same
   ---------------------------------------------------------------*/
    if ( strncasecmp((const char*)internet_apn,
                      (const char*)old_internet_apn,
                      PS_SYS_MAX_APN_LEN) != 0 )
    {
      internet_apn_configured = device_settings_info_p->apn_info.curr_apn_info
                                    [PS_SYS_APN_TYPE_INTERNET].is_configured;

      DS_3GPP_MSG2_HIGH("Setting AP default APN subs_id:%d, apn_configured:%d",
                          device_settings_info_p->subs_id,
                          internet_apn_configured);

      if( ds_3gpp_profile_set_ap_default_apn(
          dsumts_subs_mgr_get_subs_id(device_settings_info_p->subs_id),
          internet_apn_configured,
          internet_apn) == FALSE )
      {
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
        break;
      }
    }
  } while (0);

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    ret_val =  FALSE;
    DS_3GPP_ERROR_LOG_2("ds_3gpp_profile_create_modem_def_profile: err d "
                        "ret_val d", local_err, ret_val);
  }

  return ret_val;
}/*ds_3gpp_profile_create_modem_def_profile*/
/*===========================================================================
FUNCTION      DS_3GPP_PROFILE_SET_AP_DEFAULT_APN

DESCRIPTION   This function sets the AP default APN name

PARAMETERS    subs_id : Subscription ID
              apn_configured: if apn is configured or not
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
)
{
  uint16                   existing_profile_num = 0;
  uint16                   new_profile_num = 0;
  uint16                   modem_def_profile_num = 0;
  uint16                   ap_def_profile_num = 0;
  ds_profile_config_type   profile_config = {0};
  ds_profile_status_etype  profile_status = DS_PROFILE_REG_RESULT_FAIL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Invalid arguments, return
  -------------------------------------------------------------------------*/
  if( (apn_configured == TRUE) && (new_apn_name == NULL) )
  {
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    If APN configured is FALSE i.e. AP default profile is invalid,
     then reset the AP default APN . This is the TLV apn_invalid in QMI msg
  -------------------------------------------------------------------------*/
  if( apn_configured == FALSE )
  {
    return ds_3gpp_profile_reset_ap_default_apn(subs_id);
  }

  /*-----------------------------------------------------------------------
      Check if profile exists with given APN name
  -----------------------------------------------------------------------*/
  existing_profile_num
    = ds_3gpp_profile_cache_get_profile_num_for_apn(subs_id, new_apn_name);

  /*-----------------------------------------------------------------------
    Get AP default profile number . If ap_def_profile is not present
    (the case where modem_def_profile is getting created for first time),
    set this  0
  -----------------------------------------------------------------------*/
  if( dsi_profile_3gpp_get_ap_def_profile_per_subs(DB_EMBEDDED_PROFILE_FAMILY,
                                                   subs_id,
                                                   &ap_def_profile_num)
        != DS_PROFILE_REG_RESULT_SUCCESS )
  {
    ap_def_profile_num = 0;
  }

  /*-----------------------------------------------------------------------
    Create a modem_def_profile if doesnt already exists. This is the
    case where we create modem_def_profile for the first time
  -----------------------------------------------------------------------*/
  if( ap_def_profile_num == 0 )
  {
    profile_config.config_mask |= DS_PROFILE_CONFIG_MASK_PERSISTENCE;
    profile_config.is_persistent = TRUE;
    profile_config.config_mask |= DS_PROFILE_CONFIG_MASK_SUBS_ID;
    profile_config.subs_id =  dsumts_subs_mgr_get_ds_profile_subs_id(subs_id);

    profile_status = ds_profile_create_ex(DS_PROFILE_TECH_3GPP,
                                          &profile_config,
                                          &new_profile_num );
    if (profile_status != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      return FALSE;
    }


    if( TRUE == ds_3gpp_profile_update_profile_param(
                  subs_id,
                  new_profile_num,
                  DS_PROFILE_3GPP_PROFILE_PARAM_PROFILE_NAME,
                  (void*)DS_3GPP_PROFILE_MODEM_AP_DEF_PROF_SIG,
                  strlen(DS_3GPP_PROFILE_MODEM_AP_DEF_PROF_SIG)+1) )
    {
      DS_3GPP_MSG0_LOW("Updating Profile Name successful");
    }

    modem_def_profile_num = new_profile_num;
  }/*modem_def_profile is created*/
  else
  {
    modem_def_profile_num = ap_def_profile_num;
  }

  
  /*------------------------------------------------------------------- 
   Update the APN name here.
  ------------------------------------------------------------------*/
  if (modem_def_profile_num != 0)
  {
    if( TRUE == ds_3gpp_profile_update_profile_param(
                  subs_id,
                 modem_def_profile_num,
                 DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_APN,
                 (void*)new_apn_name,
                 strlen((char*)new_apn_name)+1) )
    {
      DS_3GPP_MSG0_LOW("Updating APN Name successful");
    }
  }

  /*-----------------------------------------------------------------------
      Copy the contents from existing qdp_profile to newly created/old
      modem_def_profile
  -----------------------------------------------------------------------*/
  if( existing_profile_num != 0 )
  {
    if( FALSE == ds_3gpp_profile_copy_profile_contents(subs_id,
                                                       modem_def_profile_num,
                                                       subs_id,
                                                       existing_profile_num) )
    {
      return FALSE;
    }
  }


    /*---------------------------------------------------------------------
    Set the AP default profile numbers if not already set
    ---------------------------------------------------------------------*/
  if (ap_def_profile_num == 0 )
  {
    dsi_profile_3gpp_set_ap_def_profile_number_per_subs(
       DS_PROFILE_EMBEDDED_PROFILE_FAMILY,
       subs_id,
       modem_def_profile_num);

    dsi_profile_3gpp_set_ap_def_profile_number_per_subs(
       DS_PROFILE_TETHERED_PROFILE_FAMILY,
       subs_id,
       modem_def_profile_num);
  }

  /* Register for profile change callback */
  if( ds_3gpp_profile_def_prof_info.prof_ev_func_ptr == NULL )
  {
    ds_3gpp_profile_def_prof_info.prof_ev_func_ptr
      = ds_3gpp_profile_handle_profile_events;
  }

  return TRUE;
} /* ds_3gpp_profile_set_ap_default_apn */

