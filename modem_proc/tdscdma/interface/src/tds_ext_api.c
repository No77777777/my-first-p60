/*============================================================================
                            WCDMA_EXT_API . C
Description
  This file contains the definitions for the external APIs used by non Modem
  modules.  

Copyright (c) 2001 - 2014 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/4.2.0/interface/src/wcdma_ext_api.c#1
  $DateTime: 2019/08/19 10:51:23 $


  when        who     what, where, why
  ---------   ---     ------------------------------------------------------------


===========================================================================*/

#include "tds_ext_api.h"

#define TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE( type , params, size) \
  type* arg = ( ( type* ) params ); \
  \
  if ( ( arg == NULL ) || ( size != sizeof( type ) ) ) { \
     return TDSCDMA_EBADPARAM; \
  }

/*===========================================================================
FUNCTION tds_ext_api

DESCRIPTION
  This function looks at the cmd id provided by external modules and calls the needed api as necessary
  depending on if feature TDSCDMA is enabled or not.

DEPENDENCIES
  None

RETURN VALUE
  Status

SIDE EFFECTS
  None
===========================================================================*/
uint32 tds_ext_audio_api(uint32 cmd_id, void* params, uint32 size)
{
  uint32  status    = TDSCDMA_EBADPARAM;
  boolean ret_value = FALSE;

  if (size > 0)
  {
    if (params == NULL)
    {
      MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR,"Null Params received for cmd_id 0x%x",cmd_id);
      return TDSCDMA_EBADPARAM;
    }
  }
  else if (params != NULL)
  {
    MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR,"Invalid Size for non NULL params, cmd_id 0x%x",cmd_id);
    return TDSCDMA_EBADPARAM;
  }

  switch (cmd_id)
  {
#ifdef FEATURE_TDSCDMA
    case TDSCDMA_IVOICEL2_CMD_REGISTER_UL_SERVICE:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoicel2_cmd_register_ul_service_t, params, size);
      ret_value = tdsl2_ul_service_register(arg->service);
      status = (ret_value ? TDSCDMA_EOK : TDSCDMA_EFAILED);
      break;
    }

    case TDSCDMA_IVOICEL2_CMD_REGISTER_DL_SERVICE:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoicel2_cmd_register_dl_service_t, params, size);
      ret_value = tdsl2_dl_service_register(arg->service);
		  status = (ret_value ? TDSCDMA_EOK : TDSCDMA_EFAILED);
      break;
    }

    case TDSCDMA_IVOICE_CMD_OPEN:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoice_cmd_open_t, params, size);
      tdsrrc_register_tv_adapter_info_cb(arg);
      ret_value = tdsl2_register_deregister_ivoice_cb(arg->event_cb, TRUE);
      status = (ret_value ? TDSCDMA_EOK : TDSCDMA_EFAILED);
      break;
    }
    
    case TDSCDMA_IVOICE_CMD_CLOSE:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoice_cmd_close_t, params, size);		
      tdsrrc_deregister_tv_adapter_info_cb(arg);
      ret_value = tdsl2_register_deregister_ivoice_cb(NULL, FALSE);
      status = (ret_value ? TDSCDMA_EOK : TDSCDMA_EFAILED);
      break;
    }
    
    case TDSCDMA_IVOICEL2_CMD_SET_VFR_NOTIFICATION:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoicel2_cmd_set_vfr_notification_t, params, size);
      ret_value = tdsl2_set_vfr_notify(arg->as_id, arg->enable_flag);
      status = (ret_value ? TDSCDMA_EOK : TDSCDMA_EFAILED);
      break;
    }
    
    case TDSCDMA_IVOICE_CMD_START:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoice_cmd_start_t, params, size);	
      tdsrrc_tv_session_process_start(arg);
      status = TDSCDMA_EOK;
      break;
    }
    
    case TDSCDMA_IVOICE_CMD_STOP:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoice_cmd_stop_t, params, size);	
      tdsrrc_tv_session_process_stop(arg);
      status = TDSCDMA_EOK;
      break;
    }

    case TDSCDMA_IVOICE_CMD_SEND_SAMPLE_RATE:
    {
      TDS_DEFINE_ARGS_AND_VERIFY_PARAMS_SIZE(tdscdma_ivoice_cmd_send_sample_rate_t, params, size);	
      tdsrrc_tv_send_sample_rate(arg);
      status = TDSCDMA_EOK;
      break;
    }
#endif
    default:
    {
      status = TDSCDMA_EUNSUPPORTED;
      MSG_ERROR("Unsupported command (0x%x, 0x%x) size %d", cmd_id, (uint32)(params), size);
      break;
    }
  }
  
  return status;
}