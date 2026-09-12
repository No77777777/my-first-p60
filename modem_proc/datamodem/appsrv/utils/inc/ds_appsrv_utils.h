#ifndef DS_APPSRV_UTILS_H
#define DS_APPSRV_UTILS_H
/*===========================================================================

                           DS_APPSRV_UTILS.H

DESCRIPTION

   Collection of utility functions being used by various modules in APPSRV.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/utils/inc/ds_appsrv_utils.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/17/15   hr      Initial version.
===========================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#include "datamodem_variation.h"
#include "ds_sys.h"
#include "ds_profile.h"

#define DS_APPSRV_MMGSDI_ICCID_LEN 10
#define DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN  50
#define DS_APPSRV_CFG_MAX_BUFFER_SIZE     255
#define DS_APPSRV_UTILS_UINT8_HEX_LEN     2

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/

/*-------------------------------------------------------------------
  Gives whether data is enabled or not while UE is in roaming.
  is_data_enabled = TRUE  : data is enabled while UE is in roaming
  is_data_enabled = FALSE : data is disabled while UE is in roaming
---------------------------------------------------------------------*/
typedef struct
{
  ds_sys_subscription_enum_type subs_id;
  boolean                       is_data_enabled;
}ds_appsrv_data_roaming_setting_s_type;

/*--------------------------------------------------------------
  Struct of iccid information for each sub
---------------------------------------------------------------*/
typedef struct
{
  ds_sys_subscription_enum_type subs_id;
  boolean                       is_valid;
  uint8                         iccid_len;
  uint8                         iccid[DS_APPSRV_MMGSDI_ICCID_LEN];
}ds_appsrv_iccid_info_s_type;

typedef struct
{
  char   apn_name[DS_SYS_MAX_APN_LEN];
  uint32 apn_len;
}ds_appsrv_apn_name_type;


typedef void (*ds_appsrv_utils_cfg_set_token_content_fnc)(
                                                          uint32 token_id,
                                                          char*  from,
                                                          char*  to,
                                                          void*  user_data_ptr
                                                          );

typedef void (*ds_appsrv_utils_cfg_generate_token_content_fnc)(
                                                              uint32 token_id,
                                                              char*  buff,
                                                              uint32 bufflength,
                                                              void*  user_data_ptr
                                                              );


/*===========================================================================
                    EXTERNALIZED FUNCTIONS DEFINITIONS
===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================
FUNCTION      DS_APPSRV_EFS_ITEM_FILE_PATH_SIZE

DESCRIPTION   Returns the total APPSRV EFS item file paths size

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  Total APPSRV EFS item file paths size

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_appsrv_efs_item_file_path_size
(
  void
);

/*===========================================================================
FUNCTION      DS_APPSRV_UPDATE_DATA_CONFIG_INFO

DESCRIPTION   Puts all the required nv item file paths in the
              data_config_info.conf file. Would be called during powerup.

DEPENDENCIES  None

RETURN VALUE  length of string added to buffer for success.
              -1 for failure.

SIDE EFFECTS  None
===========================================================================*/
int32 ds_appsrv_update_data_config_info
(
  char   *file_paths_buf
);


boolean ds_appsrv_utils_parse_cfg_file(
                                       const char*                               cfg_file_path,
                                       uint32                                    num_token_name,
                                       const char                                token_name_list[][DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN],
                                       ds_appsrv_utils_cfg_set_token_content_fnc cfg_set_param_fnc,
                                       void*                                     user_data_ptr
                                       );
boolean ds_appsrv_utils_generate_cfg_file(
                                          const char*                                    cfg_file_path,
                                          uint32                                         num_token_name,
                                          const char                                     token_name_list[][DS_APPSRV_CFG_MAX_TOKEN_NAME_LEN],
                                          ds_appsrv_utils_cfg_generate_token_content_fnc cfg_get_param_str_fnc,
                                          void*                                          user_data_ptr
                                          );
void uctohex(uint8 value, char dst[DS_APPSRV_UTILS_UINT8_HEX_LEN]);

// Defined in ASUtility
void ds_appsrv_utils_init_log(const char* filename, uint32 max_log_count);
boolean ds_appsrv_utils_update_log(const char* filename, const char* log_content);

/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_INIT

DESCRIPTION   This function initializes all APPSRV common services.
              The function is called during power up.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_appsrv_utils_init(void);

/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_GET_PROFILE_ID_FROM_APN

DESCRIPTION   This function searches for profile with specified APN. If 
              an existing one is not found, it creates a non-persistent
              profile and modify the APN.
 
DEPENDENCIES  None

RETURN VALUE  -1             if failed to obtain usable profile ID
              int:profile_id otherwise
              TRUE  if non-persistent profile is created
              FALSE if no profile is created

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_get_profile_id_from_apn
(
  ds_appsrv_apn_name_type *apn_ptr,
  int                     *profile_id,
  ds_profile_tech_etype    profile_tech,
  ds_profile_subs_etype    subs_id,
  boolean                  allow_creation  
);

/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_IS_PDN_FAILURE_TEMPORARY

DESCRIPTION   This function checks the netdown reason and determines if soft failure or hard failure.
 
PARAMETERS 
              failure_reason[in] - indicate the netdown reason 
 
DEPENDENCIES  None 

RETURN VALUE  boolean
              TRUE  - soft failure
              FALSE - hard failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_is_pdn_failure_temporary(uint32 failure_reason);



/*===========================================================================
FUNCTION      DS_APPSRV_UTILS_IS_PDN_FAILURE_TEMPORARY_EX

DESCRIPTION   This function checks the netdown reason, and according to the preconfiged hard failure list, 
              determines if soft failure or hard failure.
 
PARAMETERS 
              cust_hard_failure_list[in]
              hard_failure_list_size[in]
              failure_reason[in] - indicate the netdown reason 
 
DEPENDENCIES  None 

RETURN VALUE  boolean
              TRUE  - soft failure
              FALSE - hard failure

SIDE EFFECTS  None
===========================================================================*/
boolean ds_appsrv_utils_is_pdn_failure_temporary_ex
( 
  uint32* cust_hard_failure_list, 
  uint32 cust_hard_failure_list_size, 
  uint32 failure_reason
);

#ifdef __cplusplus
}
#endif

#endif /* DS_APPSRV_UTILS_H */
