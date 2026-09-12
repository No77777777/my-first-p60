 /******************************************************************************
 @file    ds_3gpp_profile_validator.c
 @brief   

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management  

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009 - 2016 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
****************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when       who     what, where, why
--------   ---     ----------------------------------------------------------
9/9/16     vb      Optimize profiles (persistent and non persistent)
=============================================================================*/

#include "ds_profile_db.h"
#include "ds_3gppi_utils.h"
#include <string.h>
#include "dsumtspdpreg.h"
#include "ds_profile_3gpp.h"
#include "ds_3gpp_profile_cache_hdlr.h"
#include "ds_3gppi_utils.h"
#include "ds_3gpp_profile_optimizer.h"
#include "ds_dsd_apm_ext_i.h"

/*--------------------------------------------------------------------------- 
  Threshold value set at 75% of max value
  ---------------------------------------------------------------------------*/
#define DS_3GPP_PERSISTENT_PROFILE_THRESHOLD \
((DS_UMTS_MAX_PDP_PROFILE_NUM - DS_UMTS_MAX_PDP_TRANSIENT_PROFILE_NUM) * 0.75)


/*===========================================================================
FUNCTION      DS_DSD_APM_READ_ATTACH_PROFILE_LIST_FROM_EFS

DESCRIPTION   This function will read attach profiles from EFS

PARAMETERS    subs_id
              attach_q
              pending_attach_list

DEPENDENCIES  None

RETURN VALUE  boolean True/False

SIDE EFFECTS  None
===========================================================================*/
extern boolean ds_dsd_apm_read_attach_profile_list_from_efs
(
  sys_modem_as_id_e_type   subs_id,
  q_type                  *q
);

/*=========================================================================
FUNCTION DS_3GPP_PROFILE_RUN_PERSISTENT_OPTIMIZER

DESCRIPTION
  Post a cmd to run persistent profile optimizer
 
PARAMETERS:
  None
 
DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_profile_run_persistent_optimizer
(
  sys_modem_as_id_e_type   subs_id
)
{
  ds_cmd_type                        *cmd_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type));

  if((cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PROFILE_PERSISTENT_OPTIMIZER;
  cmd_ptr->cmd_payload_ptr = (void *)subs_id;
  ds_put_cmd_ext2(cmd_ptr);
}/*ds_3gpp_profile_run_persistent_optimizer*/

/*=========================================================================
FUNCTION DS_3GPP_PROFILE_PERSISTENT_OPTIMIZER_CMD_HDLR

DESCRIPTION
  Perform the algorithm
  1. Get persistent profiles
  2. If persisnt profile count is >= persistent profile threshold
        Go over all the persistent profiles
        a.If the persistent profile is not a part of
           current attach profile list or pending attach profile list
         and
        b.If the persisent profile is not default profile
 
         DELETE IT
 
PARAMETERS:
  None
 
DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_persistent_optimizer_cmd_hdlr
(
   const ds_cmd_type  *cmd_ptr
)
{
  uint8                                         persistent_profile_count = 0;
  uint8                                         dbg_val                  = 0;
  ps_sys_3gpp_get_lte_attach_pdn_list_ex_type   attach_list;
  int16                                         ps_err_no                = 0;
  uint8                                         profile_count            = 0;
  boolean                                       cant_del_profile         = FALSE;
  int                                           fn_ret_val               = -1; 
  uint16                                        def_emb_profile          = 0;
  uint16                                        def_tet_profile          = 0;
  sys_modem_as_id_e_type                        subs_id                  =
                                                        SYS_MODEM_AS_ID_NONE;
  uint8                                         cache_index              = 0;
  ds_umts_pdp_profile_type                     *ds_umts_pdp_profile_ptr  = NULL;
  q_type                                        temp_profile_list_q      = {{NULL}};
  ps_sys_3gpp_lte_attach_pdn_list_type          pending_attach_info_list;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do 
  {
   
    if(cmd_ptr == NULL)
    {
      dbg_val = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }

    subs_id = (sys_modem_as_id_e_type)(int)cmd_ptr->cmd_payload_ptr;
    persistent_profile_count = ds_3gpp_profile_cache_get_persistent_count();

    /*------------------------------------------------------------------- 
      If persistent profile count is lesser than threshold value,
      no optimization is needed.
     --------------------------------------------------------------------*/
    if(persistent_profile_count < DS_3GPP_PERSISTENT_PROFILE_THRESHOLD)
    {
      dbg_val = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
    }
    memset(&attach_list,
           0,
           sizeof(ps_sys_3gpp_get_lte_attach_pdn_list_ex_type));

    memset(&pending_attach_info_list, 
           0, 
           sizeof(ps_sys_3gpp_lte_attach_pdn_list_type));

    q_init(&temp_profile_list_q);

    fn_ret_val = ds_dsd_apm_get_attach_pdn_list_ex(&attach_list,subs_id,&ps_err_no);
    
    if(ds_dsd_apm_read_attach_profile_list_from_efs(
                     subs_id,
                     &temp_profile_list_q))
    {
      ds_dsd_apm_get_profile_from_q(&temp_profile_list_q,
                                    &pending_attach_info_list);
    }

    if((fn_ret_val != 0)&&
       (pending_attach_info_list.attach_pdn_profile_list_num == 0))
    {
      dbg_val = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;
    }

    (void)ds_umts_get_default_profile_number_internal(
          DS_UMTS_EMBEDDED_PROFILE_FAMILY,dsumts_subs_mgr_get_subs_id(subs_id),&def_emb_profile);

    (void)ds_umts_get_default_profile_number_internal(
          DS_UMTS_TETHERED_PROFILE_FAMILY,dsumts_subs_mgr_get_subs_id(subs_id),&def_tet_profile);


    for(cache_index = 1;
        cache_index <= DS_UMTS_MAX_PDP_PROFILE_NUM;
        cache_index++)
    {
      cant_del_profile = FALSE;

      ds_umts_pdp_profile_ptr = DS_UMTS_PDP_PROFILE_CACHE(subs_id, cache_index);

      if((ds_umts_pdp_profile_ptr == NULL) || 
         (!ds_umts_pdp_profile_ptr->context.valid_flg) ||
         (ds_umts_pdp_profile_ptr->transience_flag) ||
         (ds_umts_pdp_profile_ptr->context.pdp_context_number == def_emb_profile) ||
         (ds_umts_pdp_profile_ptr->context.pdp_context_number == def_tet_profile)
        )
      {
        continue;
      }

      /*-------------------------------------------------------------------------------- 
        Run checks to see if the profile can be deleted
        --------------------------------------------------------------------------------*/
      for(profile_count = 0;profile_count < 
          attach_list.active_attach_pdn_list.attach_pdn_profile_list_num;
          profile_count++)
      {
        if(ds_umts_pdp_profile_ptr->context.pdp_context_number == 
           attach_list.active_attach_pdn_list.attach_pdn_profile_list[profile_count])
        {
          cant_del_profile = TRUE;
          break;
        }
      }

      if(!cant_del_profile) 
      {
        for(profile_count = 0;profile_count < 
            attach_list.pending_attach_pdn_list.attach_pdn_profile_list_num;
            profile_count++)
        {
          if(ds_umts_pdp_profile_ptr->context.pdp_context_number == 
             attach_list.pending_attach_pdn_list.attach_pdn_profile_list[profile_count])
          {
            cant_del_profile = TRUE;
            break;
          }
        }
      }
      if(!cant_del_profile)
      {
        for(profile_count = 0;
            profile_count < pending_attach_info_list.attach_pdn_profile_list_num;
            profile_count++)
        {
          if(ds_umts_pdp_profile_ptr->context.pdp_context_number == 
             pending_attach_info_list.attach_pdn_profile_list[profile_count])
          {
            cant_del_profile = TRUE;
            break;
          }
        }
      }
      /*-------------------------------------------------------------------------------- 
        If we have reached this point, It is safe to delete the profile
        Use the below event to delete the profiles.
       --------------------------------------------------------------------------------*/
      if(!cant_del_profile)
      {
        dbg_val = DS_3GPPI_LOCAL_ERR_VAL_4;

        ds_profile_delete_attach_per_sub(DS_PROFILE_TECH_3GPP,
                                        ds_umts_pdp_profile_ptr->context.pdp_context_number,
                                        dsumts_subs_mgr_get_profile_subs_id(subs_id));
      }
    }
  }while(0); 

  DS_3GPP_MSG1_HIGH("run_persistent_prf_optimizer returns %d",dbg_val);

}/*ds_3gpp_profile_run_persistent_optimizer*/


