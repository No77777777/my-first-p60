/******************************************************************************
  @file:  loc_task.c
  @brief: Location Middleware task

  DESCRIPTION
  This module handles loc middleware taks startup and processes the command queue
  for the loc mw task

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  -----------------------------------------------------------------------------
 ******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
02/22/19   mj       Location Privacy Feature
05/30/18   sum      DogHb sequence upfate
02/17/17   skm      BLE and Barometer support for LPPE Phase 2 
01/11/16   yh       LocMW refactoring
10/21/15   sudj     Fix Crash caused by bad AON configuration
06/11/15   sjk      LB 2.0 Integration
04/02/15   ssu      Batch Dwell notification for geofence.
02/08/13   ssu      Move to the DOG Heartbeat mechanism on DIME and beyond
04/12/10   ns       change loc_mw task to dynamically register with dog
04/07/10   ns       Set the loc_mw_task_started flag to indicate that
                    loc mw task is up

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_task.c#1 $
$DateTime: 2019/10/24 01:30:36 $
$Author: pwbldsvc $

======================================================================*/


#include "gps_variation.h"
#include "comdef.h"
#include "customer.h"
#include "target.h"

#include "task.h"
#include "msg.h"

#include "queue.h"
#include "aries_os_api.h"
#include "pdapi.h"
#include "pdsm_atl.h"
#include "loc_api_2.h"

#include "loc_client.h"
#include "loc_pd.h"
#include "loc_conn.h"
#include "loc_xtra.h"
#include "loc_pa.h"
#include "loc_ni.h"
#include "loc_wifi.h"
#include "loc_api_internal.h"
#include "loc_qmi_shim.h"
#include "loc_geofence.h"
// Mac: sdp message handler
#include "location_service_v02.h"
#include "loc_sdp.h"

#include "loc_slim.h"
#include "slim_for_loc.h"
#include "loc_aon.h"
#include "loc_gdt.h"
#include "loc_gtp.h"
#include "loc_wle.h"
#include "loc_nv_efs.h"
#include "loc_qmi_shim.h"
#include "loc_utils.h"
#include "loc_csm.h"
#include "loc_for_gtp.h"
#include "loc_mgp_iface.h"

#ifdef __GRIFFON__
#include "mqcsi_log.h"
#endif

static os_MutexBlockType cs_memory_record = { 0 };

static int highest_water_mark = 0;
#define SIZE_MEMORY_RECORD_ARRAY 64
static void *array_allocated_blocks[SIZE_MEMORY_RECORD_ARRAY] = { 0 };

extern void LocFM_HandleRegisterCb(const os_IpcMsgType *p_IpcMsg);
extern void locFM_Init(void);

/*
 *  e_mw_task_state -
 *      This variable is used to stop a task from running inside a Griffon build.
 *      A new API allows an external thread to change the task state from RUN to STOP.
 *      The LocMW task will poll this state variable each iteration through its
 *      task loop and will exit when it is set to "STOP".  This variable is "volatile"
 *      because it can be changed by another thread.
*/
static volatile enum
{
  MW_TASK_STATE_RUN, MW_TASK_STATE_STOP
} e_mw_task_state;

static void flush_memory_records(void)
{
  LOC_MW_ENTER_CRIT_SECTION(&cs_memory_record);
  {
    int i = 0;
    for (i = 0; i < SIZE_MEMORY_RECORD_ARRAY; i++)
    {
      array_allocated_blocks[i] = NULL;
    }
  }
  highest_water_mark = 0;
  LOC_MW_LEAVE_CRIT_SECTION(&cs_memory_record);
}

static void memory_watermark_check(void)
{
  int count = 0;
  int highest_copy = 0;
  LOC_MW_ENTER_CRIT_SECTION(&cs_memory_record);
  {
    int i = 0;
    for (i = 0; i < SIZE_MEMORY_RECORD_ARRAY; i++)
    {
      if (NULL != array_allocated_blocks[i])
      {
        ++count;
      }
    }
    if (count > highest_water_mark)
    {
      highest_water_mark = count;
    }
    highest_copy = highest_water_mark;
  }
  LOC_MW_LEAVE_CRIT_SECTION(&cs_memory_record);
  LOC_MSG_HIGH("memory_watermark_check: %d memory blocks allocated, highest %d", count, highest_copy, 0);
}

void* loc_calloc(size_t block_size)
{
  void *ptr_return = NULL;
  int cause = 0;

  if (block_size > 0)
  {
    cause = 1;
    LOC_MW_ENTER_CRIT_SECTION(&cs_memory_record);
    {
      boolean fg_continue = TRUE;
      int i = 0;
      for (i = 0; (i < SIZE_MEMORY_RECORD_ARRAY) && (TRUE == fg_continue); i++)
      {
        if (NULL == array_allocated_blocks[i])
        {
          cause = 2;
          // asked to switch to a specific heap manager
          ptr_return = os_MemAlloc(block_size, OS_MEM_SCOPE_TASK);
          if (NULL != ptr_return)
          {
            memset(ptr_return, 0, block_size);
          }
          array_allocated_blocks[i] = ptr_return;
          fg_continue = FALSE;
        }
      }
    }
    LOC_MW_LEAVE_CRIT_SECTION(&cs_memory_record);
  }
  else
  {
    cause = 4;
  }

  if (NULL == ptr_return)
  {
    LOC_MSG_ERROR("loc_calloc: %d bytes, failed, cause %d", block_size, cause, 0);
  }
  else
  {
    LOC_MSG_HIGH("loc_calloc: %d bytes, 0x%p returned.", block_size, ptr_return, 0);
  }

  return ptr_return;
}

void loc_free(void *ptr)
{
  int cause = 0;
  if (NULL != ptr)
  {
    cause = 1;
    LOC_MW_ENTER_CRIT_SECTION(&cs_memory_record);
    {
      int i = 0;
      for (i = 0; i < SIZE_MEMORY_RECORD_ARRAY; i++)
      {
        if (ptr == array_allocated_blocks[i])
        {
          void *ptr_for_modify = (void *)ptr;
          cause = 2;
          // asked to switch to a specific heap manager
          // os_MemFree would set the pointer to NULL
          os_MemFree(&ptr_for_modify);
          array_allocated_blocks[i] = NULL;
        }
      }
    }
    LOC_MW_LEAVE_CRIT_SECTION(&cs_memory_record);
  }
  else
  {
    cause = 4;
  }

  if (2 != cause)
  {
    LOC_MSG_ERROR("loc_free: 0x%p failed, cause %d", ptr, cause, 0);
  }
  else
  {
    LOC_MSG_HIGH("loc_free: 0x%p freed okay", ptr, 0, 0);
  }
}

boolean loc_middleware_initialized(void)
{

  if (FALSE == loc_middleware_data.loc_mw_task_started)
  {
    // failed
    LOC_MSG_ERROR("loc_middleware is not initialized: FAIL", 0, 0, 0);
  }
  else
  {
    LOC_MSG_HIGH("loc_middleware is initialized", 0, 0, 0);
  }

  return loc_middleware_data.loc_mw_task_started;
}

static void loc_middleware_start_up(void)
{
  (void)os_IpcStart((uint32)THREAD_ID_LOC_MIDDLEWARE);

} /* End tm_StartUp() */

void loc_middleware_offline_handler(void)
{

  LOC_MSG_HIGH("TASK OFFLINE received", 0, 0, 0);

}

/*
 ******************************************************************************
 * loc_middleware_task_stop_handler
 *
 * Description:
 *
 * This function is used to handle Stop IPC
 *
 * Parameters:
 *
 * None
 *
 * Return value:
 *
 * None
 *
 ******************************************************************************
*/
void loc_middleware_stop_handler(void)
{
  LOC_MSG_HIGH("TASK STOP received", 0, 0, 0);

  /* Deregister with the Dog Heartbeat before stopping task */
  os_DogHeartbeatDeregister();

  os_TaskStopAck();
}


/*===========================================================================
FUNCTION loc_middleware_proc

DESCRIPTION

RETURN VALUE

DEPENDENCIES
=============================================================================*/
static void loc_middleware_proc(os_IpcMsgType *p_msg)
{
  // lm_task calls this function whenever it recieves a message
  if (p_msg == NULL)
  {
    LOC_MSG_ERROR("Received NULL IPC Message ", 0, 0, 0);
    return;
  }

  LOC_MSG_MED("Received IPC Message with id %d. Loc enabled %d", p_msg->q_MsgId, loc_middleware_data.qmi_loc_enabled, 0);

  switch (p_msg->q_MsgId)
  {
    /*Handle task maintenance messages*/
    case C_OS_MSG_ID_OFFLINE:
      loc_middleware_offline_handler();
      break;

    case C_OS_MSG_ID_STOP:
      loc_middleware_stop_handler();
      break;

    case C_OS_MSG_ID_DOG_HB:
      {
        // Respond to the dog heart beat. Pat the dog.
        os_DogHeartbeatReport();
        break;
      }
      /* Handle timer expiry messages*/
    case C_OS_MSG_ID_TIMER_EXPIRY:
      {
        os_TimerExpiryType z_TimerMsg;
        memscpy(&z_TimerMsg, sizeof(z_TimerMsg),
                (void *)&p_msg->p_Data[0], p_msg->q_Size);

        /*lint -e826 */
        switch (z_TimerMsg.q_TimerId)
        {
          case LOC_MIDDLEWARE_TIMER_ID_FIX:
            {
              loc_pd_process_fix_request(eLOC_PD_FIX_TIMEOUT);
              break;
            }
          case LOC_MIDDLEWARE_TIMER_ID_XTRA:
            {
              locXtra_ProcessTimerCb();
              break;
            }
          case LOC_MIDDLEWARE_TIMER_ID_PA:
            {
              locPa_ProcessTimerCb();
              break;
            }
          case C_OS_MSG_ID_DOG_HB:
            {
              // Respond to the dog heart beat. Pat the dog.
              os_DogHeartbeatReport();
              break;
            }
          default:
            {
              if (LOC_MIDDLEWARE_TIMER_ID_SLIM_RESERVED_MIN <= z_TimerMsg.q_TimerId &&
                  LOC_MIDDLEWARE_TIMER_ID_SLIM_RESERVED_MAX >= z_TimerMsg.q_TimerId)
              {
                slim_TaskTimerHandler(z_TimerMsg.q_TimerId, z_TimerMsg.q_Param);
              }

              if (LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_MIN <= z_TimerMsg.q_TimerId &&
                  LOC_MIDDLEWARE_TIMER_ID_RESERVED_AON_MAX >= z_TimerMsg.q_TimerId)
              {
                locAonTimerHandler(z_TimerMsg.q_TimerId, z_TimerMsg.q_Param);
              }

              break;
            }
        }
        break;
      }

    case C_OS_MSG_ID_QMI_EVENT:
      locQmiShimHandleEvent();
      break;

    case LM_MIDDLEWARE_MSG_ID_PDAPI:
      (void)locMW_HandleLocCmd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SDP_SET_SPI_STATUS_IND:
      (void)loc_qmiloc_set_spi_status_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_INJECT_SENSOR_DATA_IND:
      (void)loc_qmiloc_inject_sensor_data_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_INJECT_TIME_SYNC_DATA_IND:
      (void)loc_qmiloc_inject_time_sync_data_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_GET_CRADLE_MOUNT_CONFIG_IND:
      (void)loc_qmiloc_get_cradle_mount_config_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_GET_EXTERNAL_POWER_CONFIG_IND:
      (void)loc_qmiloc_get_external_power_config_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_SET_CRADLE_MOUNT_CONFIG_IND:
      (void)loc_qmiloc_set_cradle_mount_config_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_SET_EXTERNAL_POWER_CONFIG_IND:
      (void)loc_qmiloc_set_external_power_config_ind_handler(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_SDP_EVENT_SWITCH_SPI_STREAMING_REPORT_IND:
      (void)loc_sdp_event_switch_spi_streaming_report_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SET_SENSOR_CONTROL_CONFIG_IND:
      loc_sdp_set_sensor_control_config_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GET_SENSOR_CONTROL_CONFIG_IND:
      loc_sdp_get_sensor_control_config_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SET_SENSOR_PROPERTIES_IND:
      loc_sdp_set_sensor_properties_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GET_SENSOR_PROPERTIES_IND:
      loc_sdp_get_sensor_properties_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_IND:
      loc_sdp_set_sensor_performance_control_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_IND:
      loc_sdp_get_sensor_performance_control_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_NI_IND:
      locGeofenceHandleNiInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_GEN_ALERT_IND :
      locGeofenceHandleGenAlertInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_BREACH_IND :
      locGeofenceHandleBreachInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_PROXIMITY_IND :
      locGeofenceHandleProximityInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_ADD_IND :
      locGeofenceHandleAddInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_DELETE_IND :
      locGeofenceHandleDeleteInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_QUERY_IND :
      locGeofenceHandleQueryInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_EDIT_IND :
      locGeofenceHandleEditInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_PEDOMTER_REPORT_IND:
      loc_slim_pedometer_report_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_INJECT_MOTION_DATA_IND:
      loc_slim_inject_motion_data_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_SET_ENGINE_CONFIG_IND:
      locGeofenceHandleSetEngineConfigInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SDP_INJECT_VEHICLE_SENSOR_DATA_IND:
      (void)loc_slim_inject_vehicle_sensor_data_ind_handler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_BATCH_BREACH_IND :
      locGeofenceHandleBatchBreachInd(p_msg);
      break;
    case LM_MIDDLEWARE_MSG_ID_PD_EVENT_WIFI_AP_SCAN_INJECT_REQ_IND:
      locPd_EventWifiApScanInfoInjectIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_PD_INJECT_WIFI_AP_SCAN_IND:
      locPd_InjectWifiApScanInfoIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_PD_EVENT_SRN_AP_SCAN_INJECT_REQ_IND:
      locPd_EventSrnApScanInfoInjectIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_PD_INJECT_SRN_AP_SCAN_IND:
      locPd_InjectSrnApScanInfoIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GTP_CLIENT_DLD_DATA_IND:
      (void)loc_gtp_WwanClientDloadedDataIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_BEGIN_IND:
      (void)loc_gdt_BeginIndicationHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_END_IND:
      (void)loc_gdt_EndIndicationHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_BEGIN_REQ_IND:
      loc_gdt_BeginReqIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_END_REQ_IND:
      loc_gdt_EndReqIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_BEGIN_STATUS_IND:
      loc_gtp_DownloadBeginStatusIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_WLE_APCACHE_REQ_IND:
      (void)loc_wle_ApCacheIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_WLE_NOAPCACHE_REQ_IND:
      (void)loc_wle_NoApCacheIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_TZ_REQ_IND:
      locGeofenceHandleGetTzReq();
      break;

    case LM_MIDDLEWARE_MSG_ID_GEOFENCE_BATCH_DWELL_IND :
      locGeofenceHandleBatchDwellInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_QMI_LOC_MW_IND:
      locMW_HandleQmiIndication(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_QMI_LOC_PA_IND:
      locPa_SendQmiIndication(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GTP_AP_STATUS_REQ_IND:
      loc_gtp_ApStatusIndicationdHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_BEGIN_REQ_IND:
      loc_gtp_DownloadBeginStatusReqIndicationHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_READY_STATUS_IND:
      loc_gdt_DownloadReadyStatusIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_RECEIVE_DONE_STATUS_IND:
      loc_gdt_ReceiveDoneStatusIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_END_STATUS_IND:
      loc_gdt_DownloadEndStatusIndHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_RECEIVE_DONE_REQ_IND:
      loc_gdt_RcvDoneReqIndicationHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GDT_DOWNLOAD_END_REQ_IND:
      loc_gdt_DownloadEndReqIndicationHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_CROWDSOURCE_MANAGER_CONTROL_IND:
      locCsm_ControlReqIndicationdHandler(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SEND_FDCL_SERVICE_REQ_IND:
      loc_tle_sendFDCLServiceReqInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SEND_GET_FDCL_BS_LIST_IND:
      loc_tle_sendGetFdclBsListInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SEND_INJECT_FDCL_DATA_IND:
      loc_tle_sendInjectFdclDataInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_LFM_REG_CB:
      LocFM_HandleRegisterCb(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SEND_FDCL2_SERVICE_REQ_IND:
      loc_tle_Fdcl2_SendBsObsDataServiceReqInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_SEND_GET_FDCL2_BS_OBS_DATA_IND:
      loc_tle_Fdcl2_sendGetBsObsDataInd(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT:
      locMgp_HandleEphemerisReport(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_GNSS_EVENT_REPORT:
      loc_HandleEventReport(p_msg);
      break;

    case LM_MIDDLEWARE_MSG_ID_E911_STATE_IND:
      (void)locClnt_HandleE911StateInd(p_msg);
      break;


    default:
      /* Forward IPC messages targeted to SLIM */
      if (p_msg->q_MsgId >= LM_MIDDLEWARE_MSG_ID_SLIM_RESERVERD_MIN &&
          p_msg->q_MsgId <= LM_MIDDLEWARE_MSG_ID_SLIM_RESERVERD_MAX)
      {
        slim_TaskIpcHandler(p_msg);
      }

      /* Forward IPC messages targeted to AON */
      else if (((p_msg->q_MsgId >= LM_MIDDLEWARE_MSG_ID_AON_MIN &&
                 p_msg->q_MsgId <= LM_MIDDLEWARE_MSG_ID_AON_MAX)) ||
               ((p_msg->q_MsgId == LM_MIDDLEWARE_MSG_ID_AON_STATUS_CB   ||
                 p_msg->q_MsgId == LM_MIDDLEWARE_MSG_ID_AON_LOCATION_CB)))
      {
        locAonIpcHandler(p_msg);
      }

      break;
  }

  memory_watermark_check();
}

static void loc_middlware_module_init(void)
{
  uint32 q_Cnt = 0;

  // Zero initialize the loc_middleware data
  memset((void *)&(loc_middleware_data), 0, sizeof(loc_middleware_data));
  loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle = LOC_CLIENT_HANDLE_INVALID;
  loc_middleware_data.z_locClnt_Data.l_MasterClientHandle = LOC_CLIENT_HANDLE_INVALID;

  for (q_Cnt = 0; q_Cnt < LOC_CLIENT_MAX_TEST_MASTER_CLIENTS; q_Cnt++)
  {
    loc_middleware_data.z_locClnt_Data.lz_TestMasterClientHandles[q_Cnt] = LOC_CLIENT_HANDLE_INVALID;
  }

  (void)q_init(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue));

  loc_middleware_data.qmi_loc_enabled          = FALSE;
  loc_middleware_data.pd_module_initialized    = FALSE;
  loc_middleware_data.nmea_module_initialized  = FALSE;
  loc_middleware_data.pa_module_initialized    = FALSE;
  loc_middleware_data.xtra_module_initialized  = FALSE;
  loc_middleware_data.conn_module_initialized  = FALSE;
  loc_middleware_data.ni_module_initialized    = FALSE;
  loc_middleware_data.geofence_module_initialized  = FALSE;
  loc_middleware_data.aon_module_initialized  = FALSE;
  loc_middleware_data.pdapi_client_id          = -1;
  loc_middleware_data.xtra_client_id           = -1;
  loc_middleware_data.ni_client_id             = -1;
  loc_middleware_data.v_NvAllowNfwPosReq    = TRUE;
  loc_middleware_data.v_NvAllowNfwPosReqNotification = FALSE;
  loc_middleware_data.v_AfwAllowNfwPosReq   = TRUE;
  loc_middleware_data.v_AfwAllowAfwPosReq   = TRUE;
  loc_middleware_data.v_AfwAllowPriPosReq   = TRUE;
  loc_middleware_data.v_E911Session         = FALSE;

  LOC_MW_INIT_CRIT_SECTION(&loc_middleware_data.crit_sec);
  LOC_MW_INIT_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  LOC_MW_INIT_CRIT_SECTION(&loc_middleware_data.xtra_crit_sec);
  LOC_MW_INIT_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  LOC_MW_INIT_CRIT_SECTION(&cs_memory_record);
  flush_memory_records();
  slim_TaskInit();
  locClnt_TaskInit();

  //indicate that the loc middleware task is up
  loc_middleware_data.loc_mw_task_started = TRUE;
}

/*=============================================================================

  FUNCTION      loc_middleware_task_stop()

  DESCRIPTION   Exit loc_mw task loop

  DEPENDENCIES  None

  PARAMETERS    None

  RETURN VALUE  None

  SIDE EFFECTS  Calling this API will cause the LocMW task to "exit"

=============================================================================*/
void loc_middleware_task_stop(void)
{
  e_mw_task_state = MW_TASK_STATE_STOP;
}


/*===========================================================================
FUNCTION LOC_MIDDLWARE_TASK

DESCRIPTION


RETURN VALUE

DEPENDENCIES
=============================================================================*/
void loc_middleware_task
(
   dword dummy                     /* Rex requirement, not used */
   )
{
  os_IpcMsgType *p_msg;

  (void)dummy;

  LOC_MSG_HIGH("Starting loc middleware task ...", 0, 0, 0);

  /* location middleware module initialization */
  loc_middlware_module_init();

  /* Set up IPC and timers */
  loc_middleware_start_up();

  /* Register for DOG heartbeat monitoring. Make sure we do this after the IPC is set up . */
  os_DogHeartbeatRegister();

  /* Put LocFM_Init() before os_TaskReadyAck(), so LFM knows it is ready to serve feature registreation */
  locFM_Init();

  os_TaskReadyAck();
  os_TaskStopRegister();

  /*
   * Initialize the MQSCI module on Griffon only
   * The mqcsi_powerup_init() function is handled by the MPROC team
   * in QCT modem SW builds, but the GPS team is responsible in Griffon (SOC) builds
   */
#ifdef __GRIFFON__
  mqcsi_powerup_init();
#endif

  /* Initialize QMI data structures. Dont register QMI-Loc with QMI framework
   * yet. Resgiter with SM a callback for PDAPI initialization notification */
  locQmiShimInit();
  slim_TaskStart();

  e_mw_task_state = MW_TASK_STATE_RUN;

  while (e_mw_task_state == MW_TASK_STATE_RUN)
  {
    p_msg = os_IpcReceive();

    if (p_msg == NULL)
    {
      LOC_MSG_ERROR("NULL message received", 0, 0, 0);
    }
    else
    {
      loc_middleware_proc(p_msg);
      if (!os_IpcDelete(p_msg))
      {
        LOC_MSG_ERROR("Unable to delete received IPC message", 0, 0, 0);
      }
    }
  }    /* while(task in the RUN state)*/
} /* End tm_Task() */
