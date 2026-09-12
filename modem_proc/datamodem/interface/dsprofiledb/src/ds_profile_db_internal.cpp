/******************************************************************************
  @file    ds_profile_db_internal.cpp
  @brief   Database 

  DESCRIPTION
  Internal APIs for profiles database. Should be used by mode handlers and other internal
  components. 
  ! Not for external client usage !  

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2013-2015 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/04/13   ez      Created the module. First version of the file.

===========================================================================*/


/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include <stdlib.h>
#include "ds_profile_db.h"
#include "ds_Profile_Profile.h"
#include "ds_Profile_DBMgr.h"
#include "ps_system_heap.h"
#include "ds_Profile_CallbackMgrModem.h"



using namespace ds::Profile;

extern "C"
{
void ds_profile_db_cmd_hdlr(
  ds_cmd_type  *cmd_ptr
  )
{
  switch(cmd_ptr->hdr.cmd_id)
  {
    case DS_CMD_PROFILE_DB_EXT:
      CallbackMgrModem::CommandHandler(cmd_ptr);
      break;
    case DS_CMD_PROFILE_DB_NV_REFRESH_EXT:
      DBMgr::Instance()->CommandHandler(cmd_ptr);
      break;
    case DS_CMD_PROFILE_DB_DEVICE_MODE_EXT:
      DBMgr::Instance()->CommandHandler(cmd_ptr);
      break;
    default:
      PROFILE_DB_LOG_ERROR("Unknown Command");
      break;
  }
}

void ds_profile_db_ds3g_cmd_hdlr(
  ds3geventmgr_event_type        event_id,
  ds3geventmgr_filter_type       *filter_info_ptr,
  void                           *event_info_ptr,
  void                           *data_ptr
  )
{
  ds_cmd_type                         *cmd_ptr = NULL;
  ds_profile_nv_refresh_cmd_type      *cmd_payload = NULL;
  ds3geventmgr_nv_refresh_info_type   *nv_refresh_event_info_ptr = NULL;
  ds3geventmgr_attach_pdn_list_type   *attach_profile_info_ptr = NULL;
  typedef enum
  {
    DS_PROFILE_DB_LOCAL_ERR_NONE            = 0,
    DS_PROFILE_DB_LOCAL_ERR_INVALID_PARAMS  = 1,
    DS_PROFILE_DB_LOCAL_ERR_INVALID_SUBS    = 2,
    DS_PROFILE_DB_LOCAL_ERR_INVALID_COMMAND = 3
  } ds_profile_db_local_err_type;

  ds_profile_db_local_err_type         local_err = 
    DS_PROFILE_DB_LOCAL_ERR_NONE;
/*-------------------------------------------------------------------------------*/

  if(filter_info_ptr == NULL ||
     event_info_ptr == NULL)
  {
    local_err = 
      DS_PROFILE_DB_LOCAL_ERR_INVALID_PARAMS;
    goto bail;
  }


  if((filter_info_ptr->ps_subs_id < PS_SYS_PRIMARY_SUBS || 
     filter_info_ptr->ps_subs_id > PS_SYS_TERTIARY_SUBS))
  {
    local_err = 
      DS_PROFILE_DB_LOCAL_ERR_INVALID_SUBS;
    goto bail;
  }

  switch (event_id)
  {
    case DS3GEVENTMGR_NV_REFRESH_EV:
    {
      cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_profile_nv_refresh_cmd_type));
      if ((cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL))
      {
        ASSERT(0);
        return;
      }

      nv_refresh_event_info_ptr = (ds3geventmgr_nv_refresh_info_type *)event_info_ptr;

      cmd_ptr->hdr.cmd_id = DS_CMD_PROFILE_DB_NV_REFRESH_EXT;
      cmd_payload = (ds_profile_nv_refresh_cmd_type *)cmd_ptr->cmd_payload_ptr;
      cmd_payload->slot_id = nv_refresh_event_info_ptr->slot;
      cmd_payload->subs = filter_info_ptr->ps_subs_id;
       
      ds_put_cmd_ext(cmd_ptr);
      break;
    }

    case DS3GEVENTMGR_APM_NEW_ATTACH_PROFILE_EV:
    {
      attach_profile_info_ptr = (ds3geventmgr_attach_pdn_list_type *)event_info_ptr;
      DBMgr::Instance()->AttachProfileCommandHandler(attach_profile_info_ptr, 
                                                     (ds_profile_db_subs_type)
                                                      filter_info_ptr->ps_subs_id);
      break;
    }
    default:
      local_err = 
        DS_PROFILE_DB_LOCAL_ERR_INVALID_COMMAND;
      break;
  }

bail:
  PROFILE_DB_LOG_HIGH_1("DS3G Command Handler local_res %d", local_err);
  return;
}

void ds_profile_db_ds3g_subsmgr_event_cb(
  ds3gsubsmgr_event_type        event_id,
  ds3gsubsmgr_event_info_type  *event_info_ptr,
  void                         *data_ptr
  )
{
/*-------------------------------------------------------------------------
    Check if callback pointer is NULL
  -------------------------------------------------------------------------*/
  if (event_info_ptr == NULL)
  {
    return;
  }

  PROFILE_DB_LOG_HIGH_1("ds3gdsdif_subsmgr_event_cb event_id:%d", 
                        event_id);
  DBMgr::Instance()->DeviceModeCommandHandler(event_info_ptr);
}

}
