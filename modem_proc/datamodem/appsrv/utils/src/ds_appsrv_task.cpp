/*==============================================================================

                              ds_appsrv_task.h

GENERAL DESCRIPTION
  Utility functions for appsrv modules to send commands to dstask

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2014 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/05/14    ml     Created file/Initial version.
==============================================================================*/
#include "datamodem_variation.h"

#include "ds_appsrv_task.h"

#include "ds_atp_api.h"
#include "ds_edu_mgr_api.h"
#include "ds_andsf_api.h"
#include "ds_andsf_ran_meas.h"
#include "ds_andsf_query_data_manager.h"
#include "ds_http_data_mgr.h"
#include "ds_ssl_api.h"
#include "ds_wlan_meas_ext.h"
#include "ds_abss_mgr.h"

#ifdef FEATURE_DATA_WCDMA_LATENCY_OPT
#include "ds_ul_monitor_wcdma_latency_opt.h"
#endif /* FEATURE_DATA_WCDMA_LATENCY_OPT */

#include "ds_keepalive_assist.h"

#include "data_msg.h"
extern "C"
{
  #include "dstask_v.h"
}

#include "ds_autoreg.h"
#include "ds_lowlatency_dispatcher.h"
#ifdef QWES_FEATURE_ENABLE
#include "ds_caas_mgr_ext.h"
#endif /* QWES_FEATURE_ENABLE */

/*==============================================================================
                            Internal Object Types
==============================================================================*/
struct ds_appsrv_cmd_msg
{
  ds_appsrv_process_fcn process_fcn;
  uint32*               data_ptr;
};

/*==============================================================================
                          Static Function Declaration
==============================================================================*/
static void ds_appsrv_process_cmd(ds_cmd_type *cmd_ptr);

/*==============================================================================
                          Static Variable Declaration
==============================================================================*/
static ds_appsrv_process_fcn process_fcn_list[DS_APPSRV_MODULE_MAX];



/*==============================================================================
                                  Initializers
==============================================================================*/
void ds_appsrv_cmd_init()
{
  // register appsrv process function to dstask
  ds_cmd_process_register(DS_MODULE_APPSRV, &ds_appsrv_process_cmd);

  for(uint32 i = 0; i < DS_APPSRV_MODULE_MAX; ++i)
  {
    process_fcn_list[i] = NULL;
  }
}


void ds_andsf_rule_init()
{
#ifdef FEATURE_DATA_ANDSF
  ds_andsf_cfg_init();
  ds_andsf_msg_init();
  ds_andsf_event_handler_init();

#ifdef FEATURE_DATA_ANDSF_RULE_MGR
  ds_andsf_rule_mgr_data_init();
  ds_andsf_ran_meas_dsd_init();
#endif /* FEATURE_DATA_ANDSF_RULE_MGR */

#endif /* FEATURE_DATA_ANDSF */
}


void ds_appsrv_init()
{
  /* appsrv module init */
  ds_ssl_init(); /* ssl need to be initialized before http */
  ds_http_init();

#ifdef FEATURE_DATA_ATP
  ds_atp_init();
#endif /* FEATURE_DATA_ATP */

#ifdef FEATURE_DATA_EDU_MGR
  ds_edu_mgr_init(); /* initialize edu mgr module*/
#endif /* FEATURE_DATA_EDU_MGR */

/*Initialize wlan meas module*/
  ds_wlan_meas_init();

#ifdef FEATURE_DATA_ANDSF
  ds_andsf_init();
#endif /* FEATURE_DATA_ANDSF*/

#ifdef FEATURE_DATA_WCDMA_LATENCY_OPT
  ds_ul_monitor_wcdma_latency_opt_event_init();
#endif /* FEATURE_DATA_WCDMA_LATENCY_OPT */

  ds_keepalive_assist_init();
  ds_autoreg_init();

  ds_lowlatency_dispatcher_init();
  ds_abss_init();
#ifdef QWES_FEATURE_ENABLE
  ds_caas_mgr_init();
#endif /* QWES_FEATURE_ENABLE */
}



/*==============================================================================
                          DSTask Command processing
==============================================================================*/
void ds_appsrv_register_process_fcn(
                                    ds_appsrv_module_type module_id,
                                    ds_appsrv_process_fcn process_fcn
                                    )
{
  if(DS_APPSRV_MODULE_MAX > module_id)
    process_fcn_list[module_id] = process_fcn;
}




boolean ds_appsrv_put_cmd(ds_appsrv_module_type module_id, uint32* msg_content_ptr)
{
  if(DS_APPSRV_MODULE_MAX <= module_id || NULL == process_fcn_list[module_id])
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "ds_appsrv_put_cmd - Invalid or unregistered module ID %d", module_id);
    return FALSE;
  }

  ds_cmd_type* cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_appsrv_cmd_msg));

  if (NULL == cmd_ptr)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_put_cmd - alloc ds_cmd_type failed");
    return FALSE;
  }

  cmd_ptr->hdr.cmd_id           = DS_APPSRV_CMD;
  ds_appsrv_cmd_msg* appsrv_msg = (ds_appsrv_cmd_msg*)cmd_ptr->cmd_payload_ptr;
  appsrv_msg->process_fcn       = process_fcn_list[module_id];
  appsrv_msg->data_ptr          = msg_content_ptr;

  ds_put_cmd(cmd_ptr);
  return TRUE;
}




boolean ds_appsrv_put_cmd_ex(ds_appsrv_process_fcn process_fcn, uint32* msg_content_ptr)
{
  if(NULL == process_fcn)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_put_cmd_ext - NULL process fcn");
    return FALSE;
  }

  ds_cmd_type* cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_appsrv_cmd_msg));

  if (NULL == cmd_ptr)
  {
    DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "ds_appsrv_put_cmd_ext - alloc ds_cmd_type failed");
    return FALSE;
  }

  cmd_ptr->hdr.cmd_id           = DS_APPSRV_CMD;
  ds_appsrv_cmd_msg* appsrv_msg = (ds_appsrv_cmd_msg*)cmd_ptr->cmd_payload_ptr;
  appsrv_msg->process_fcn       = process_fcn;
  appsrv_msg->data_ptr          = msg_content_ptr;

  ds_put_cmd(cmd_ptr);
  return TRUE;
}




void ds_appsrv_process_cmd(ds_cmd_type *cmd_ptr)
{
  if (NULL == cmd_ptr || DS_APPSRV_CMD != cmd_ptr->hdr.cmd_id)
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "ds_appsrv_process_cmd - NULL cmd_ptr %d or invalid cmd_id", cmd_ptr);
    return;
  }

  ds_appsrv_cmd_msg* appsrv_msg = (ds_appsrv_cmd_msg*)cmd_ptr->cmd_payload_ptr;

  if(NULL == appsrv_msg || NULL == appsrv_msg->process_fcn)
  {
    DATA_APPSRV_MSG1(MSG_LEGACY_ERROR, "ds_appsrv_process_cmd - NULL process msg info %d", appsrv_msg);
    return;
  }

  (appsrv_msg->process_fcn)(appsrv_msg->data_ptr);
}


