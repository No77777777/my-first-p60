/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                      ESTK  BEARER INDEPENDENT PROTOCOL


GENERAL DESCRIPTION : ENHANCED STK layer BIP support for 3gpp2

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_bip_3gpp2.c#1 $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------
06/25/18   dd      Modify persistence mask value
06/01/17   gs      Don't depend on DS Sys Status for establishing BIP on non-DDS
03/17/16   gm      Initial version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_msg.h"
#ifdef FEATURE_ESTK
#include "estk_bip.h"
#include "estk_bip_3gpp2.h"
#include "nv.h"
#include "ds_profile.h"
#include "ds_profile_3gpp.h"
#include "ds_profile_3gpp2.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

static estk_result_enum_type estk_set_net_policy_3gpp2_profile(
  uint32                       ch_idx,
  ds_profile_num_type         *profile_num_ptr
);

static estk_result_enum_type estk_get_3gpp2_profile_to_use(
  uint32                        ch_idx,
  ds_profile_tech_etype         tech_type,
  ds_profile_num_type          *profile_num_ptr,
  ds_profile_subs_etype         sub_id
);

/*===========================================================================

FUNCTION: estk_set_net_policy_3gpp2_profile

DESCRIPTION :
  This function creates profiles with APN and 3GPP2 bearer parameters

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  profile_num_ptr: [Output] The profile num created

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static estk_result_enum_type estk_set_net_policy_3gpp2_profile(
  uint32                       ch_idx,
  ds_profile_num_type         *profile_num_ptr
)
{
  ds_profile_hndl_type                profile_handler;
  ds_profile_info_type                profile_info;
  ds_profile_3gpp2_pdn_type_enum_type pdn_type        = DS_PROFILE_3GPP2_PDN_TYPE_V4;
  ds_profile_action_etype             commit_act      = DS_PROFILE_ACTION_COMMIT;
  ds_profile_3gpp2_rat_type_enum_type rat_type        = DS_PROFILE_3GPP2_RAT_TYPE_HRPD_EHRPD;
  struct ps_sockaddr_in               *p_dest_addr_v4 = NULL;
  ds_profile_config_type              profile_cfg     = {DS_PROFILE_CONFIG_MASK_PERSISTENCE, FALSE};
  gstk_open_ch_req_type              *open_ch_cmd     = NULL;
  uint32                              i               = 0;
  ds_profile_subs_etype               sub_id          = DS_PROFILE_ACTIVE_SUBSCRIPTION_NONE;
  
  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  open_ch_cmd = &estk_curr_inst_ptr->curr_cmd.cmd.open_ch_pro_cmd_req;

  if(ch_idx >= ESTK_MAX_BIP_CH ||
     profile_num_ptr == NULL)
  {
    UIM_MSG_ERR_2("invalid params: 0x%x 0x%x 0x%x", ch_idx, profile_num_ptr);
    return ESTK_BAD_INPUT_PARAM;
  }

  if (gstk_io_ctrl(
        GSTK_IO_SLOT_ID_TO_DS_SUBSCRIPTION_ID,
        &estk_curr_inst_ptr->slot_id,
        &sub_id) != GSTK_SUCCESS)
  {
    return ESTK_ERROR;
  }

  p_dest_addr_v4 =
    (struct ps_sockaddr_in*)estk_curr_inst_ptr->bip_channel_table[ch_idx].dest_addr;
  if (open_ch_cmd != NULL && !open_ch_cmd->dns_requested && p_dest_addr_v4 == NULL)
  {
    UIM_MSG_ERR_0("invalid dest addr!");
    return ESTK_ERROR;
  }

  /* Search for EPC profile, if success set EPC profile as 3gpp2 profile */
  if(estk_get_3gpp2_profile_to_use(ch_idx, DS_PROFILE_TECH_EPC, profile_num_ptr, sub_id) == ESTK_SUCCESS)
  {
    /* No need to search profiles created by ESTK, ESTK will not create EPC profiles */
    UIM_MSG_HIGH_1("ESTK_BIP: Found EPC profile %d", *profile_num_ptr);
    return ESTK_SUCCESS;
  }  

  /* Check if profile_num_ptr can be reused */
  if(estk_get_3gpp2_profile_to_use(ch_idx, DS_PROFILE_TECH_3GPP2, profile_num_ptr, sub_id) != ESTK_SUCCESS)
  {
    profile_cfg.config_mask = DS_PROFILE_CONFIG_MASK_SUBS_ID|DS_PROFILE_CONFIG_MASK_PERSISTENCE;
    profile_cfg.is_persistent = FALSE;
    profile_cfg.subs_id = sub_id;

    /* Get profile to use */
    if (ds_profile_create_ex(
          DS_PROFILE_TECH_3GPP2,
          &profile_cfg,
          profile_num_ptr) != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to create profile!");
      return ESTK_ERROR;
    }
    else
    {
      /* Save profile number to delete on close channel */
      UIM_MSG_HIGH_1("ESTK_BIP: Profile number created %d", *profile_num_ptr);
      estk_curr_inst_ptr->bip_channel_table[ch_idx].profile_3gpp2 = uint16toint16(*profile_num_ptr);
    }
  }
  else
  {
    UIM_MSG_HIGH_0("ESTK_BIP: 3GPP2 Profile Reuse - no parameters to set");
    /*
      Check if we are re-using a profile created by ESTK. If not created
      by ESTK we do not cache the profile number to avoid deleting the profile
    */
    for (i = 0; i < ESTK_MAX_BIP_CH; i++)
    {
      if (estk_curr_inst_ptr->bip_channel_table[i].profile_3gpp2 == uint16toint16(*profile_num_ptr) &&
          estk_curr_inst_ptr->bip_channel_table[i].status != ESTK_CH_CLOSED)
      {
        estk_curr_inst_ptr->bip_channel_table[ch_idx].profile_3gpp2 = uint16toint16(*profile_num_ptr);
        UIM_MSG_HIGH_1("ESTK_BIP: Reusing profile %d created by ESTK",
                       estk_curr_inst_ptr->bip_channel_table[ch_idx].profile_3gpp2);
      }
    }
    return ESTK_SUCCESS;
  }

  if (ds_profile_begin_transaction_per_sub(
        DS_PROFILE_TRN_RW,
        DS_PROFILE_TECH_3GPP2,
        *profile_num_ptr,
        sub_id,
        &profile_handler) != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    UIM_MSG_ERR_0("failed to start modifying profile!");
    return ESTK_ERROR;
  }

  if (estk_curr_inst_ptr->bip_channel_table[ch_idx].apn_len != 0)
  {
    profile_info.len = int32touint16(estk_curr_inst_ptr->bip_channel_table[ch_idx].apn_len);
    profile_info.buf = estk_curr_inst_ptr->bip_channel_table[ch_idx].apn;
    if (ds_profile_set_param(
          profile_handler,
          (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_APN_STRING,
          &profile_info) != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to set APN!");
      commit_act = DS_PROFILE_ACTION_CANCEL;
      goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
    }
  }

  /* set login name */
  if (estk_curr_inst_ptr->bip_channel_table[ch_idx].username_len > 0)
  {
    profile_info.len =
      uint32touint16(estk_curr_inst_ptr->bip_channel_table[ch_idx].username_len);
    profile_info.buf = estk_curr_inst_ptr->bip_channel_table[ch_idx].username;
    if (ds_profile_set_param(
          profile_handler,
          (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_USER_ID,
          &profile_info) != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to set USERNAME!");
      commit_act = DS_PROFILE_ACTION_CANCEL;
      goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
    }
  }

  /* set password */
  if (estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd_len > 0)
  {
    profile_info.len =
      uint32touint16(estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd_len);
    profile_info.buf = estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd;
    if (ds_profile_set_param(
          profile_handler,
          (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_AUTH_PASSWORD,
          &profile_info) != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to set PASSWORD!");
      commit_act = DS_PROFILE_ACTION_CANCEL;
      goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
    }
  }

  /* set profile to IPV4 or IPV6 accordingly */
  if (open_ch_cmd != NULL && open_ch_cmd->dns_requested)
  {
    pdn_type = DS_PROFILE_3GPP2_PDN_TYPE_V4_V6;
  }
  else
  {
    switch (p_dest_addr_v4->ps_sin_family)
    {
    case DSS_AF_INET:
      pdn_type = DS_PROFILE_3GPP2_PDN_TYPE_V4_V6;
      if(gstk_nv_get_feature_status(
           GSTK_CFG_FEATURE_SLOT_ESTK_BIP_IP_DEFAULT,
           estk_curr_inst_ptr->slot_id) == TRUE)
      {
        UIM_MSG_HIGH_0("GSTK_CFG_FEATURE_ESTK_BIP_IP_DEFAULT NV set - set PDN type v4");
        pdn_type = DS_PROFILE_3GPP2_PDN_TYPE_V4;
      }
      break;
    case DSS_AF_INET6:
      pdn_type = DS_PROFILE_3GPP2_PDN_TYPE_V4_V6;
      break;
    default:
      UIM_MSG_ERR_1("invalid ps_sin_family 0x%x!",
                    p_dest_addr_v4->ps_sin_family);
      commit_act = DS_PROFILE_ACTION_CANCEL;
      goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
    }
  }
  profile_info.len = sizeof(pdn_type);
  profile_info.buf = &pdn_type;
  if (ds_profile_set_param(
          profile_handler,
          DS_PROFILE_3GPP2_PROFILE_PARAM_PDN_TYPE,
          &profile_info) != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    UIM_MSG_ERR_0("failed to set PDP type!");
    commit_act = DS_PROFILE_ACTION_CANCEL;
    goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
  }

  profile_info.len = sizeof(rat_type);
  profile_info.buf = &rat_type;
  if (ds_profile_set_param(
          profile_handler,
          DS_PROFILE_3GPP2_PROFILE_PARAM_RAT_TYPE,
          &profile_info) != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    UIM_MSG_ERR_0("failed to set RAT type!");
    commit_act = DS_PROFILE_ACTION_CANCEL;
    goto ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN;
  }

ESTK_SET_NET_POLICY_3GPP2_PROFILE_RETURN:
  if (ds_profile_end_transaction(
        profile_handler,
        commit_act) != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    UIM_MSG_ERR_0("failed to commit profile!");
    return ESTK_ERROR;
  }

  if (commit_act == DS_PROFILE_ACTION_COMMIT)
  {
    return ESTK_SUCCESS;
  }
  else
  {
    return ESTK_ERROR;
  }
}

/*===========================================================================

FUNCTION: estk_get_3gpp2_profile_to_use

DESCRIPTION :
  This function iterates throught the list of profiles and tries to reuse a
  profile number if possible for 3gpp2

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  tech_type:       [Input]  Tech type 3GPP2
  profile_num_ptr: [Output] The profile num to be reused
  sub_id :         [Input] DS Sub Id

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static estk_result_enum_type estk_get_3gpp2_profile_to_use(
  uint32                        ch_idx,
  ds_profile_tech_etype         tech_type,
  ds_profile_num_type          *profile_num_ptr,
  ds_profile_subs_etype         sub_id
)
{
  ds_profile_status_etype             api_ret_status;
  ds_profile_list_type                profile_list;
  ds_profile_itr_type                 profile_itr;
  ds_profile_list_info_type           profile_list_info;
  ds_profile_info_type                profile_list_name;
  ds_profile_hndl_type                profile_hndl;
  ds_profile_info_type                profile_info;
  char                                profile_apn[DS_PROFILE_3GPP2_APN_MAX_VAL_LEN + 1];
  char                                profile_user_id[DS_PROFILE_3GPP2_PPP_MAX_USER_ID_LEN + 1];
  char                                profile_user_pwd[DS_PROFILE_3GPP2_PPP_MAX_PASSWD_LEN + 1];
  int32                               param_len = -1;
  int                                 i = 0;
  char                                profile_name[DS_PROFILE_3GPP_MAX_PROFILE_NAME_LEN];

  UIM_MSG_HIGH_3("estk_get_3gpp2_profile_to_use: ch_idx=0x%x, tech_type=0x%x, "
                 "profile_num_ptr=0x%x", ch_idx, tech_type, profile_num_ptr);

  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  if(ch_idx >= ESTK_MAX_BIP_CH)
  {
    return ESTK_ERROR;
  }

  if((profile_num_ptr == NULL)||((tech_type != DS_PROFILE_TECH_3GPP2)&&(tech_type != DS_PROFILE_TECH_EPC)))
  {
    return ESTK_ERROR;
  }

  memset((void *)&profile_list, 0, sizeof(ds_profile_list_type));
  memset((void *)&profile_itr, 0, sizeof(ds_profile_itr_type));
  memset((void *)&profile_list_info, 0, sizeof(ds_profile_list_info_type));
  memset((void *)&profile_info, 0, sizeof(ds_profile_info_type));
  memset((void *)profile_apn, 0, sizeof(profile_apn));
  memset((void *)profile_user_id, 0, sizeof(profile_user_id));
  memset((void *)profile_user_pwd, 0, sizeof(profile_user_pwd));

  profile_list.dfn = DS_PROFILE_LIST_ALL_PROFILES;
  profile_list_info.name = &(profile_list_name);
  profile_list_info.name->len = DS_PROFILE_3GPP_MAX_PROFILE_NAME_LEN;
  profile_list_info.name->buf = profile_name;

  /* Get an iterator first */
  api_ret_status = ds_profile_get_list_itr_per_sub(tech_type,
                                           &profile_list,
                                           &profile_itr, sub_id);

  if(api_ret_status != DS_PROFILE_REG_RESULT_SUCCESS)
  {
    UIM_MSG_ERR_1("ds_profile_get_list_itr_per_sub failed %d", api_ret_status);
    return ESTK_ERROR;
  }

  do
  {
    /* reset the len as DS API ds_profile_get_info_by_itr) might have changed it during
       prior iteration */
    profile_list_info.name->len = DS_PROFILE_3GPP_MAX_PROFILE_NAME_LEN;
    /* Get the profile number for current profile using iterator */
    api_ret_status = ds_profile_get_info_by_itr(profile_itr,
                                                &profile_list_info);

    ESTK_BIP_DESTROY_ITR_IF_FAIL(api_ret_status, profile_itr);

    /* Get handle on the profile */
    api_ret_status = ds_profile_begin_transaction_per_sub(DS_PROFILE_TRN_READ,
                                                          tech_type,
                                                          profile_list_info.num,
                                                          sub_id,
                                                          &profile_hndl);

    ESTK_BIP_DESTROY_ITR_IF_FAIL(api_ret_status, profile_itr);

    /* Get the profile APN parameter to check if profile can be resused */
    profile_info.buf = profile_apn;
    profile_info.len = DS_PROFILE_3GPP2_APN_MAX_VAL_LEN + 1;

    ESTK_BIP_GET_PROFILE_PARAM(api_ret_status, profile_hndl, profile_itr, profile_info,
       (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_APN_STRING);

    UIM_MSG_HIGH_3("profile number %d, Profile Info APN Length = %d , Open Channel APN length = %d",
                    profile_list_info.num,
                    profile_info.len,
                    estk_curr_inst_ptr->bip_channel_table[ch_idx].apn_len);

    for(i=0; i < profile_info.len; i++)
    {
      UIM_MSG_HIGH_3("Profile %d APN[%d]=0x%x",
                     profile_list_info.num, i, profile_apn[i]);
    }

    /* Check if APN is terminated by NULL */
    param_len = estk_curr_inst_ptr->bip_channel_table[ch_idx].apn_len;
    if((param_len > 0) &&
       (estk_curr_inst_ptr->bip_channel_table[ch_idx].apn[param_len - 1] == 0))
    {
      UIM_MSG_HIGH_0("Open Channel APN terminated by NULL");
      param_len = param_len - 1;
    }

    /*Check if the APN matches*/
    if((profile_info.len == param_len) &&
       (profile_info.len == 0 ||
       strncasecmp(profile_info.buf,
                   (char *)estk_curr_inst_ptr->bip_channel_table[ch_idx].apn,
                   profile_info.len) == 0))
    {
       UIM_MSG_HIGH_1("Found matching profile number for apn%d",
                      profile_list_info.num);
       *profile_num_ptr = profile_list_info.num;
    }
    else  /* Iterate on the iterator */
    {
      ESTK_BIP_ITERATOR_ITERATE(api_ret_status, profile_itr, profile_hndl);
      continue;
    }

    /* Check if User ID is terminated by NULL */
    param_len = estk_curr_inst_ptr->bip_channel_table[ch_idx].username_len;
    if((param_len > 0) &&
       (estk_curr_inst_ptr->bip_channel_table[ch_idx].username[param_len - 1] == 0))
    {
      UIM_MSG_HIGH_0("Open Channel User ID terminated by NULL");
      param_len = param_len - 1;
    }
    if(param_len > 0)
    {
      /* Get the profile User ID parameter to check if profile can be resused */
      profile_info.buf = profile_user_id;
      profile_info.len = DS_PROFILE_3GPP2_PPP_MAX_USER_ID_LEN + 1;

      if(tech_type == DS_PROFILE_TECH_EPC)
      {
        ESTK_BIP_GET_EPC_PROFILE_PARAM(api_ret_status, profile_hndl, profile_itr, profile_info,
           DS_PROFILE_TECH_3GPP2, (ds_profile_identifier_type)DS_PROFILE_TECH_COMMON_PROFILE_PARAM_USER_ID, sub_id);
      }
      else
      {
        ESTK_BIP_GET_PROFILE_PARAM(api_ret_status, profile_hndl, profile_itr, profile_info,
           (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_USER_ID);
      }

      UIM_MSG_HIGH_2("Profile Info User ID Length = %d , Open Channel User ID length = %d",
                     profile_info.len,
                     estk_curr_inst_ptr->bip_channel_table[ch_idx].username_len);

      for(i=0; i < profile_info.len; i++)
      {
        UIM_MSG_HIGH_3("Profile %d User ID[%d]=0x%x",
                       profile_list_info.num, i, profile_user_id[i]);
      }

      /*Check if the User ID matches*/
      if((profile_info.len == param_len) &&
        (memcmp(profile_info.buf, estk_curr_inst_ptr->bip_channel_table[ch_idx].username, profile_info.len) == 0))
      {
        UIM_MSG_HIGH_1("Found matching profile number for user ID %d",
                       profile_list_info.num);
        *profile_num_ptr = profile_list_info.num;
      }
      else  /* Iterate on the iterator */
      {
        ESTK_BIP_ITERATOR_ITERATE(api_ret_status, profile_itr, profile_hndl);
        continue;
      }
    }

    /* Check if Password is terminated by NULL */
    param_len = estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd_len;
    if((param_len > 0) &&
       (estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd[param_len - 1] == 0))
    {
      UIM_MSG_HIGH_0("Open Channel Password terminated by NULL");
      param_len = param_len - 1;
    }
    if(param_len > 0)
    {
      /* Get the profile User Password parameter to check if profile can be resused */
      profile_info.buf = profile_user_pwd;
      profile_info.len = DS_PROFILE_3GPP2_PPP_MAX_PASSWD_LEN + 1;

      if(tech_type == DS_PROFILE_TECH_EPC)
      {
        ESTK_BIP_GET_EPC_PROFILE_PARAM(api_ret_status, profile_hndl, profile_itr, profile_info,
           DS_PROFILE_TECH_3GPP2, (ds_profile_identifier_type)DS_PROFILE_TECH_COMMON_PROFILE_PARAM_AUTH_PASSWORD, sub_id);
      }
      else
      {
        ESTK_BIP_GET_PROFILE_PARAM(api_ret_status, profile_hndl, profile_itr, profile_info,
           (ds_profile_identifier_type)DS_PROFILE_3GPP2_PROFILE_PARAM_AUTH_PASSWORD);
      }

      UIM_MSG_HIGH_2("Profile Info Pwd Length = %d , Open Channel Pwd length = %d",
                profile_info.len,
                estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd_len);

      for(i=0; i < profile_info.len; i++)
      {
        UIM_MSG_HIGH_3("Profile %d Pwd[%d]=0x%x",
                       profile_list_info.num, i, profile_user_pwd[i]);
      }

      /*Check if the Password matches*/
      if((profile_info.len == param_len) &&
         (memcmp(profile_info.buf,
                 estk_curr_inst_ptr->bip_channel_table[ch_idx].passwd,
                 profile_info.len) == 0))
      {
        UIM_MSG_HIGH_1("Found matching profile number for Passwd %d",
                       profile_list_info.num);
        *profile_num_ptr = profile_list_info.num;
      }
      else  /* Iterate on the iterator */
      {
        ESTK_BIP_ITERATOR_ITERATE(api_ret_status, profile_itr, profile_hndl);
        continue;
      }
    }

    /* Release handle */
    if (ds_profile_end_transaction(
      profile_hndl,
      DS_PROFILE_ACTION_CANCEL) != DS_PROFILE_REG_RESULT_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to commit profile!");
      (void)ds_profile_itr_destroy(profile_itr);
      return ESTK_ERROR;
    }
    (void)ds_profile_itr_destroy(profile_itr);
    return ESTK_SUCCESS;
  } while(api_ret_status != DS_PROFILE_REG_RESULT_LIST_END);

  (void)ds_profile_itr_destroy(profile_itr);
  UIM_MSG_HIGH_0("Did not find a re-usable profile");
  return ESTK_ERROR;
} /* estk_get_3gpp2_profile_to_use */

/*===========================================================================

FUNCTION: estk_bip_get_3gpp2_profile

DESCRIPTION :
  This function gets the 3GPP2 profile to use

PARAMETERS:
  ch_idx :         [Input]  Index of the active channel
  profile_num_ptr: [Output] The profile num created/selected

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS
    ESTK_ERROR
    ESTK_BAD_INPUT_PARAM

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_bip_get_3gpp2_profile(
  uint32                           ch_idx,
  estk_bip_apn_select_method_type  apn_select_method,
  ds_profile_num_type             *profile_num_ptr
)
{
  const gstk_ds_system_status_type *sys_status_ptr = NULL;

  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  if((ch_idx >= ESTK_MAX_BIP_CH) ||
     (apn_select_method < ESTK_BIP_APN_OPEN_CH) ||
     (apn_select_method > ESTK_BIP_APN_DS_SHARING) ||
     (profile_num_ptr == NULL))
  {
    UIM_MSG_ERR_3("estk_bip_get_3gpp2_profile, ch_idx:0x%x apn_select_method:0x%x profile_num_ptr:0x%x",
                  ch_idx, apn_select_method, profile_num_ptr);

    return ESTK_BAD_INPUT_PARAM;
  }

  /* If APN select method is DS profile sharing, use profile 0xFE always */
  if(apn_select_method == ESTK_BIP_APN_DS_SHARING)
  {
    *profile_num_ptr = DS_PROFILE_ALWAYS_SHARE_PROFILE_NUM;
    return ESTK_SUCCESS;
  }

  *profile_num_ptr = DS_PROFILE_UNSPECIFIED_NUM;

  /* For APN selection methods other than profile sharing, a valid
     APN is expected */
  if(estk_curr_inst_ptr->bip_channel_table[ch_idx].apn_len == 0)
  {
    UIM_MSG_ERR_1("APN length is zero, not valid for 3GPP2 with APN selection method: 0x%x",
                  apn_select_method);
    return ESTK_ERROR;
  }

  if(gstk_io_ctrl(GSTK_IO_GET_DS_SYSTEM_RAT_INFO,
                  (void *)&estk_curr_inst_ptr->slot_id,
                  (void *)&sys_status_ptr)!= GSTK_SUCCESS)
  {
    UIM_MSG_ERR_0("DS system info retrieval failed");
    return ESTK_ERROR;
  }

  /* If valid RATs are available */
  if(sys_status_ptr != NULL && sys_status_ptr->num_avail_sys > 0)
  {
    uint32  sys_index          = 0;
    boolean is_1x_hrpd_present = FALSE;
    boolean other_rats_present = FALSE;

    for(sys_index = 0; sys_index < sys_status_ptr->num_avail_sys; sys_index++)
    {
      /* Check the DS RATs available */
      switch(sys_status_ptr->avail_sys[sys_index].rat_value)
      {
        case GSTK_DS_SYS_RAT_EX_3GPP2_1X:
        case GSTK_DS_SYS_RAT_EX_3GPP2_HRPD:
          is_1x_hrpd_present = TRUE;
          break;
        case GSTK_DS_SYS_RAT_EX_3GPP_WCDMA:
        case GSTK_DS_SYS_RAT_EX_3GPP_GERAN:
        case GSTK_DS_SYS_RAT_EX_3GPP_LTE:
        case GSTK_DS_SYS_RAT_EX_3GPP_TDSCDMA:
        case GSTK_DS_SYS_RAT_EX_3GPP2_EHRPD:
          other_rats_present = TRUE;
          break;
        case GSTK_DS_SYS_RAT_EX_3GPP_WLAN:
        case GSTK_DS_SYS_RAT_EX_3GPP2_WLAN:
        case GSTK_DS_SYS_RAT_EX_WLAN:
        case GSTK_DS_SYS_RAT_EX_NULL_BEARER:
        default:
          break;
      }
    }
    /* Use profile '0' only if prefered rat is 1x/HRPD */
    if((is_1x_hrpd_present == TRUE) &&
       (other_rats_present == FALSE))
    {
      *profile_num_ptr = 0;
    }
  }

  /* Get profile using the APN */
  if(*profile_num_ptr == DS_PROFILE_UNSPECIFIED_NUM)
  {
    if (estk_set_net_policy_3gpp2_profile(ch_idx, profile_num_ptr) != ESTK_SUCCESS)
    {
      UIM_MSG_ERR_0("failed to set 3GPP2 profile!");
      return ESTK_ERROR;
    }
  }

  return ESTK_SUCCESS;
} /* estk_bip_get_3gpp2_profile */
#endif /* FEATURE_ESTK */

