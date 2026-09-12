#ifndef DS_DSD_APM_EFS_PARSER_H
#define DS_DSD_APM_EFS_PARSER_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
         
                       D A T A   S E R V I C E S 
                        DSD EFS PARSER HEADER FILE

GENERAL DESCRIPTION

  All the declarations and definitions necessary to support parsing  
  of DSD APM related EFS file.

Copyright (c) 2012-2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==================================================================
                        EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_apm_efs_parser.h#2 $

when       who         what, where, why
--------   -----       ----------------------------------------------------------
fjia       05/12/12    Create APM EFS module

===========================================================================*/

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "ds_dsd_apm_ext_i.h"
#include "ds_dsd_apm.h"
#include "lte.h"
#include "queue.h"
#include "ds_profile_3gpp.h"
#include "ds_profile_3gppi.h"

#ifdef FEATURE_DATA_LTE
#define DSD_APM_MAX_PDN_NUM                      2
#define DSD_APM_RULE_MAX_PARAMS_NUM              10
#define DSD_APM_RULE_MAX_PARAM_NAME_SIZE         42
#define DSD_APM_ATTACH_PROF_MAX_PARAM_NAME_SIZE  47
#define DSD_APM_ATTACH_PROFILES_MAX              2

#define DSD_APM_ROOT_DIR                 "/data"
#define DSD_APM_RULES_FILE               "/data/ds_dsd_apm_rules.txt"
#define DSD_APM_TMP_RULES_FILE           "/data/ds_dsd_apm_rules.tmp"
#define DSD_APM_ATTACH_PROFILE_FILE      "/data/ds_dsd_attach_profile.txt"
#define DSD_APM_TMP_ATTACH_PROFILE_FILE  "/data/ds_dsd_attach_profile.tmp"

#define DSD_APM_MAX_TAG_VAL_LENGTH \
  DSD_APM_RULE_MAX_PARAM_NAME_SIZE+DSD_APM_MAX_APN_STRING_LEN+4

#define DSD_APM_PROFILE_MAX_RULE_BUF_LENGTH \
  (ATTACH_APN_MAX)*(DSD_APM_RULE_MAX_PARAMS_NUM*DSD_APM_MAX_TAG_VAL_LENGTH)

typedef struct
{
  uint8                               apn_string_len;
  char                                apn_string[DS_DSD_APM_MAX_APN_STRING_LEN];
}dsd_apm_apn_name_type;

/*-----------------------------------------------------------------------------
      Attach Profile Status
------------------------------------------------------------------------------*/
#define ATTACH_PROF_READ_FAIL     0x0000
#define ATTACH_PROF_READ_SUCCESS  0x0001
#define ATTACH_PROF_CHANGED       0x0002

/*===========================================================================
FUNCTION      DS_DSD_APM_READ_PDN_RULE_LIST_FROM_EFS

DESCRIPTION   This function will read PDN rules from EFS

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_dsd_apm_read_pdn_rule_list_from_efs
(
  sys_modem_as_id_e_type   subs_id,
  q_type                  *q
);

/*===========================================================================
FUNCTION      DS_DSD_APM_GET_PROFILE_PARAM_INFO

DESCRIPTION   This function gets the profile param info from profile cache.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_dsd_apm_get_profile_param_info
(
  uint16                      profile_id,
  ds_profile_identifier_type  profile_param_id,
  ds_profile_info_type       *profile_param_info_ptr,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_APM_IF_VZW_ATTACH_PDN_RETRY_ENABLE

DESCRIPTION   This function get the NV configuration to know if feature
              enable or not.

DEPENDENCIES  None.

RETURN VALUE

===========================================================================*/
void ds_dsd_apm_efs_init( void );

/*===========================================================================
FUNCTION      DSD_APM_READ_ATTACH_PROFILE_LIST_FROM_EFS

DESCRIPTION   This function will read attach profiles from EFS

PARAMETERS    subs_id
              attach_q
              pending_attach_list

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_dsd_apm_read_attach_profile_list_from_efs
(
  sys_modem_as_id_e_type   subs_id,
  q_type                  *q
);

/*===========================================================================
FUNCTION      DS_DSD_APM_READ_DEFAULT_ATTACH_PROF

DESCRIPTION   This function will read attach profiles from the PDP registry

PARAMETERS    subs_id: Current active Data subs
              attach_prof_q: Queue that contains the attach profiles

DEPENDENCIES  None

RETURN VALUE  TRUE: If we were able to obtain default attach prof
              FALSE: Otherwise

SIDE EFFECTS  Modification of global attach profile queue of APM
===========================================================================*/
boolean ds_dsd_apm_read_default_attach_prof
(
  sys_modem_as_id_e_type   subs_id,
   q_type                 *attach_prof_q
);

/*===========================================================================
FUNCTION      DS_DSD_APM_READ_LTE_REL_VER_NV

DESCRIPTION   This function get the NV configuration to know the LTE release
              version.

DEPENDENCIES  None.

RETURN VALUE

===========================================================================*/
boolean  ds_dsd_apm_read_lte_release10_nv
(
  lte_3gpp_release_ver_e  *buf
);

/*===========================================================================
FUNCTION      DS_DSD_APM_READ_LTE_REL_VER_NV

DESCRIPTION   This function get the NV configuration to know the LTE release
              10 throttling.

DEPENDENCIES  None.

RETURN VALUE

===========================================================================*/
boolean  ds_dsd_apm_read_release10_throttling
(
  uint8 *buf
);
/*===========================================================================
FUNCTION       DS_DSD_APM_GEN_APM_RULE_STRING

DESCRIPTION    Creates apm rule string from apm_rule_q

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_gen_apm_rule_string
(
  q_type                  *rule_q,
  char                    *str,
  uint16                   buffer_size,
  sys_modem_as_id_e_type   subs_id
);


/*===========================================================================
FUNCTION      DS_DSD_APM_UPDATE_APN_RULE_EFS_FILE

DESCRIPTION   This function will overwrite the EFS file with string passed

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_dsd_apm_update_apn_rule_efs_file
(
  sys_modem_as_id_e_type   subs_id,
  q_type                  *q,
  char*                    str
);

/*===========================================================================
FUNCTION      DS_DSD_APM_TRANSLATE_IPREQ_TO_CHAR

DESCRIPTION   This is a utility to translate IP Required enum type to char

DEPENDENCIES  None.

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
char  ds_dsd_apm_translate_ipreq_to_char
(
  ipRequiredType  ip_req
);

/*===========================================================================
FUNCTION      DS_DSD_APM_EFS_OVERWRITE_ATTACH_PDN_LIST

DESCRIPTION   This function overwrite the attach PDN list in EFS with input
              attach PDN profile ID array

DEPENDENCIES  None.

RETURN VALUE  TRUE: Operation succeeds
              FALSE: Operation fails

===========================================================================*/
boolean ds_dsd_apm_efs_overwrite_attach_pdn_list
(
  sys_modem_as_id_e_type   subs_id,
  uint16                  *profile_id_arr,
  uint8                    num_of_pdn
);

/*===========================================================================
FUNCTION      DS_DSD_APM_EFS_DEL_ATTACH_INFO

DESCRIPTION   This function clears all the attach info files from EFS

DEPENDENCIES  None.

RETURN VALUE  void
===========================================================================*/
void ds_dsd_apm_efs_del_attach_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_UPDATE_RULES_BUFFER

DESCRIPTION    This utility function will add the rule contents to a buffer
               which will be eventually dumped to EFS in a TEMP file

PARAMETERS     apm_rules_buffer: buffer to store updated rules
               param_id: Rule tag
               param_val: Value for the rule tag
               param_length: Length of the value

DEPENDENCIES   NONE

RETURN VALUE   None

SIDE EFFECTS   NONE
===========================================================================*/
void ds_dsd_apm_update_rules_buffer
(
  char                               *apm_rules_buffer,
  dsd_apm_rule_param_name_enum_type   param_id,
  void                               *param_val,
  uint8                               param_length
);

/*===========================================================================
FUNCTION       DS_DSD_APM_EFS_OVERWRITE_ATTACH_RULE_LIST

DESCRIPTION    This function will write the updated APM rules to TEMP file
               which will be read during next attach

PARAMETERS     file_buffer: Buffer containing the updated rules
               subs_id: Active PS subs

DEPENDENCIES   NONE

RETURN VALUE   None

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_efs_overwrite_attach_rule_list
(
  char                    *file_buffer,
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION       DS_DSD_APM_CHECK_APM_RULES_FILE_VALIDITY

DESCRIPTION    This function will check if APM Rules file is present or not

PARAMETERS     subs_id: Active PS subs

DEPENDENCIES   NONE

RETURN VALUE   TRUE: If File is present
               FALSE: Otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds_dsd_apm_check_apm_rules_file_validity
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DSD_APM_PARSE_ATTACH_PROFILES_FROM_EFS

DESCRIPTION   This function read attach profile from EFS and save to passed 
              profile queue. 

              EFS File Format

              Param_Name:Param_Val;

              For example -
              Attach_Profile_ID:1;

PARAMETERS    efs_db - structure that contains info about the EFS file

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_dsd_apm_parse_attach_profiles_from_efs
(
  sys_modem_as_id_e_type     subs_id,
  ds3gcfgmgr_efs_token_type *efs_db,
  q_type                    *profile_q
);

/*===========================================================================
FUNCTION      DS_DSD_APM_EFS_DEL_TEMP_ATTACH_INFO

DESCRIPTION   This function clears all the temp attach info files from EFS

DEPENDENCIES  None.

RETURN VALUE  void
===========================================================================*/
void ds_dsd_apm_efs_del_temp_attach_info
(
  sys_modem_as_id_e_type  subs_id
);

#endif /* FEATURE_DATA_LTE */

#endif /* DS_DSD_APM_EFS_PARSER_H*/
