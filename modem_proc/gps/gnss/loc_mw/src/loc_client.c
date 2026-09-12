/******************************************************************************
  @file:  loc_client.c
  @brief: Location Middleware client

  DESCRIPTION
  This module defines the loc API routines for a client that connects to 
  loc-mw

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2008-2012 by Qualcomm Technologies Incorporated.
  All Rights Reserved.QUALCOMM Proprietary and Confidential.
  Copyright (c) 2013-2015 QUALCOMM Atheros Incorporated.
  All Rights Reserved.QUALCOMM Atheros Proprietary and Confidential.
  Copyright (c) 2015-2025 Qualcomm Technologies, Inc. All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  -----------------------------------------------------------------------------
 ******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      ------------------------------------------------------- 
06/18/24   bpk		FR99502:Incorporating latest location fix in emergency INVITE
01/29/24   bpk      Adding support for TBM feature for Divar
07/08/20   skm      ShortCodes SpecialRequest support for START_REQ and GET_BEST_POS 
11/15/18   nath     Added a NV to set accuracy when the preferred accuracy is HIGH.
06/22/18   ap       Adding defensive F3 messages to debug Non Duplicable crash
06/17/18   sj       Added custom SLP for MO AGNSS requests.
05/04/17   yh       Debug NMEA
03/31/17   yh       Loc_Conn and Loc_NI clean up
10/28/16   yh       Set t_LastFixReportTime for every QMI_LOC_START call
10/25/16   yh       LocMW was not sending out final position report in Single shot MO location session
08/26/16   yh       Initialize t_LastFixReportTime to the MAX value.
08/17/16   yh       Fix "1HZ client reports position at TBF 1.1s rather than 1s"
06/10/16   yh       Fix QMI_LOC Ind message not sent out bug
04/26/16   yh       Single shot fix criteria bug fix
04/01/16   yh       Schedule after interval expired for > 1HZ clients
03/23/16   yh       Multiple client position report
03/14/16   yh       Fix QMI_LOC Ind message not sent out bug
01/28/16   yh       Broadcast intermediate fix
12/10/15   yh       Remove timer when fix interval is one second
09/03/15   yh       Add loc_client_set_fix_min_interval
06/11/15   sjk      LB 2.0 Integration 
04/27/11   spn      Updated for Loc-Api 2.0

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_client.c#6 $
$DateTime: 2025/06/19 05:09:19 $
$Author: pwbldsvc $
======================================================================*/


/*===================================================================
This module manages location api client.
====================================================================*/

#include "comdef.h"
#include "customer.h"
#include "target.h"

#include "limits.h"
#include "msg.h"
#include "queue.h"
#include "time_svc.h"

#include "aries_os_api.h"
#include "pdapi.h"
#include "pdsm_atl.h"
#include "loc_api_2.h"

#include "loc_api_internal.h"
#include "loc_client.h"
#include "loc_pd.h"
#include "loc_conn.h"
#include "loc_xtra.h"
#include "loc_pa.h"
#include "loc_ni.h"
#include "loc_wifi.h"
#include "loc_batching.h"
#include "loc_utils.h"
#include "loc_geofence.h"
#include "loc_nv_efs.h"
#include "loc_qmi_shim.h"

#define LOC_CLIENT_MAX_NUM                   (50)
#define DEFAULT_MODE                         eQMI_LOC_OPER_MODE_STANDALONE_V02
#define MAXIMUM_OVERHEAD_TIME                (400)
#define LOC_CLIENT_POS_OFFSET                (50)
#define LOC_CLIENT_MEAS_OFFSET               (50)
#define LOC_CLIENT_TBF_THRESHOLD             (5000)
#define LOC_CLIENT_MASTER_VALUE              (0xBAABCDEF)
#define LOC_CLIENT_TEST_MASTER_VALUE         (0xF88FABCD)
#define LOC_CLIENT_FIX_ACCURACY_HIGH_MAX     (250)

static const struct {
  qmiLocEventRegMaskT_v02 t_QmiEventRegMask;
  locClnt_EventMaskType   t_LocEventRegMask;
}locClnt_EventRegMaskMap[] =
{
  { QMI_LOC_EVENT_MASK_POSITION_REPORT_V02,                LOC_EVENT_PARSED_POSITION_REPORT        },
  { QMI_LOC_EVENT_MASK_GNSS_SV_INFO_V02,                   LOC_EVENT_SATELLITE_REPORT              },
  { QMI_LOC_EVENT_MASK_NMEA_V02,                           LOC_EVENT_NMEA_POSITION_REPORT          },
  { QMI_LOC_EVENT_MASK_NI_NOTIFY_VERIFY_REQ_V02,           LOC_EVENT_NI_NOTIFY_VERIFY_REQUEST      },
  { QMI_LOC_EVENT_MASK_INJECT_POSITION_REQ_V02,            LOC_EVENT_ASSISTANCE_DATA_REQUEST       },
  { QMI_LOC_EVENT_MASK_INJECT_TIME_REQ_V02,                LOC_EVENT_INJECT_TIME_REQUEST           },
  { QMI_LOC_EVENT_MASK_INJECT_PREDICTED_ORBITS_REQ_V02,    LOC_EVENT_INJECT_XTRA_DATA_REQUEST      },
  { QMI_LOC_EVENT_MASK_ENGINE_STATE_V02,                   LOC_EVENT_STATUS_REPORT                 },
  { QMI_LOC_EVENT_MASK_FIX_SESSION_STATE_V02,              LOC_EVENT_FIX_SESSION_STATE_REPORT      },
  { QMI_LOC_EVENT_MASK_WIFI_REQ_V02,                       LOC_EVENT_WPS_NEEDED_REQUEST            },
  { QMI_LOC_EVENT_MASK_LOCATION_SERVER_CONNECTION_REQ_V02, LOC_EVENT_LOCATION_SERVER_REQUEST       },
  { QMI_LOC_EVENT_MASK_SENSOR_STREAMING_READY_STATUS_V02,  LOC_EVENT_SENSOR_STREAMING_READY_STATUS },
  { QMI_LOC_EVENT_MASK_TIME_SYNC_REQ_V02,                  LOC_EVENT_TIME_SYNC_REQ                 },
  { QMI_LOC_EVENT_MASK_SET_SPI_STREAMING_REPORT_V02,       LOC_EVENT_SPI_STREAMING_REPORT          },
  { QMI_LOC_EVENT_MASK_GNSS_MEASUREMENT_REPORT_V02,        LOC_EVENT_GNSS_MEASUREMENT_REPORT       },
  { QMI_LOC_EVENT_MASK_GNSS_SV_POLYNOMIAL_REPORT_V02,      LOC_EVENT_SV_POLY_REPORT                },
  { QMI_LOC_EVENT_MASK_BATCHING_STATUS_V02,                LOC_EVENT_MASK_BATCHING_STATUS          },
  { QMI_LOC_EVENT_MASK_BATCH_FULL_NOTIFICATION_V02,        LOC_EVENT_MASK_BATCH_FULL_NOTIFICATION  },
  { QMI_LOC_EVENT_MASK_LIVE_BATCHED_POSITION_REPORT_V02,   LOC_EVENT_MASK_LIVE_BATCHED_POSITION_REPORT },
  { QMI_LOC_EVENT_MASK_GET_BAND_MEASUREMENT_METRICS_V02,   LOC_EVENT_MASK_BAND_MEASUREMENT_METRICS },
  { QMI_LOC_EVENT_MASK_GNSS_NHZ_MEASUREMENT_REPORT_V02,    LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT},
  { QMI_LOC_EVENT_MASK_QUERY_XTRA_INFO_V02,                LOC_EVENT_QUERY_XTRA_INFO_STATUS        },
  { QMI_LOC_EVENT_MASK_DBH_POSITION_V02,                   LOC_EVENT_MASK_DBH_POSITION             },
};

typedef enum {
  eLOC_CLNT_OPER_MODE_PRIORITY_UNKNOWN    = 0,
  eLOC_CLNT_OPER_MODE_PRIORITY_CELL_ID    = 1,
  eLOC_CLNT_OPER_MODE_PRIORITY_STANDALONE = 2,
  eLOC_CLNT_OPER_MODE_PRIORITY_MSB        = 3,
  eLOC_CLNT_OPER_MODE_PRIORITY_MSA        = 4,
}locClntOperationPriorityEnumT;

typedef enum {
  eLOC_CLNT_TMCF_QUERY = 0,
  eLOC_CLNT_TMCF_SET = 1,
  eLOC_CLNT_TMCF_RESET = 2
}locClntTestMasterClientFuncMode;

typedef const struct
{
  locClntOperationPriorityEnumT       e_prority;
  qmiLocOperationModeEnumT_v02        e_OperationMode;
} locClntOperationModeRuleStructT;

static locClntOperationModeRuleStructT pz_locClntOperationModeRule[] =
{
  { eLOC_CLNT_OPER_MODE_PRIORITY_CELL_ID,   eQMI_LOC_OPER_MODE_CELL_ID_V02 },
  { eLOC_CLNT_OPER_MODE_PRIORITY_STANDALONE, eQMI_LOC_OPER_MODE_STANDALONE_V02 },
  { eLOC_CLNT_OPER_MODE_PRIORITY_MSB,       eQMI_LOC_OPER_MODE_DEFAULT_V02 },
  { eLOC_CLNT_OPER_MODE_PRIORITY_MSB,       eQMI_LOC_OPER_MODE_MSB_V02 },
  { eLOC_CLNT_OPER_MODE_PRIORITY_MSA,       eQMI_LOC_OPER_MODE_MSA_V02 },
  { eLOC_CLNT_OPER_MODE_PRIORITY_MSA,       eQMI_LOC_OPER_MODE_WWAN_V02 },
};

// Table which contains commands which can only be performed by the master client,
// IF there is a master client registered. Please ensure that the commands are
// listed in sorted ascending order, if not LocMW will have to sort it
static uint32 z_locClntMasterCmds[] =
{
  QMI_LOC_INJECT_POSITION_REQ_V02,
  QMI_LOC_SET_ENGINE_LOCK_REQ_V02,
  QMI_LOC_SET_SBAS_CONFIG_REQ_V02,
  QMI_LOC_SET_NMEA_TYPES_REQ_V02,
  QMI_LOC_SET_LOW_POWER_MODE_REQ_V02,
  QMI_LOC_SET_SERVER_REQ_V02,
  QMI_LOC_DELETE_ASSIST_DATA_REQ_V02,
  QMI_LOC_SET_XTRA_T_SESSION_CONTROL_REQ_V02,
  QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_REQ_V02,
  QMI_LOC_SET_SENSOR_CONTROL_CONFIG_REQ_V02,
  QMI_LOC_SET_SENSOR_PROPERTIES_REQ_V02,
  QMI_LOC_SET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_REQ_V02,
  QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ_V02,
  QMI_LOC_DELETE_SUPL_CERTIFICATE_REQ_V02,
  QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02,
  QMI_LOC_INJECT_SUBSCRIBER_ID_REQ_V02,
  QMI_LOC_SET_PREMIUM_SERVICES_CONFIG_REQ_V02,
  QMI_LOC_SET_GNSS_CONSTELL_REPORT_CONFIG_V02,
  QMI_LOC_DELETE_GNSS_SERVICE_DATA_REQ_V02,
  QMI_LOC_SET_BLACKLIST_SV_REQ_V02,
  QMI_LOC_SET_CONSTELLATION_CONTROL_REQ_V02
};

// Function declaration
static int locClnt_SetFixCriteria
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocStartReqMsgT_v02 *pz_StartReq
   );

static int locClnt_GetHighAccuracy(void);

/* Function to process the event registration mask set request for this location client. */
static int locClnt_SetEventRegMaskLocked
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocRegEventsReqMsgT_v02 *pz_RegEventsReq
   );

static int locClnt_GetFixCriteria
(
   const loc_client_info_s_type *pz_ClientInfo
   );

/* Function to process the custom server address set request for this location client.
 * Custom server will be used when doing MSA/MSB fixes. */
static int locClnt_SetCustomServerAddr
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocSetServerReqMsgT_v02 *p_SetServerAddr
   );

static int locClnt_GetCustomPdeServerAddr
(
   const loc_client_info_s_type *pz_ClientInfo
   );

/* Function to process the operation mode set request for this location client. */
static int locClnt_SetOperationMode
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocSetOperationModeReqMsgT_v02 *p_SetOpModeReq
   );

static int locClnt_GetOperationMode
(
   const loc_client_info_s_type *pz_ClientInfo
   );

static uint32 locClnt_GetFixInterval
(
   const locClnt_FixCriteriaType *pz_FixCriteria
   );

/* Function declaration to set master client */
static int locClnt_SetMasterClient
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocRegisterMasterClientReqMsgT_v02 *pz_RegMstrClntReq
   );

/** @brief Supported TBF interval for 20Hz */
#define LOC_CLIENT_INTERVAL_20HZ_MS (50)
/** @brief Supported TBF interval for 10Hz */
#define LOC_CLIENT_INTERVAL_10HZ_MS (100)
/** @brief Supported TBF interval for 5Hz */
#define LOC_CLIENT_INTERVAL_5HZ_MS (200)
/** @brief Supported TBF interval for 2Hz */
#define LOC_CLIENT_INTERVAL_2HZ_MS (500)
/** @brief Supported TBF interval for 1Hz */
#define LOC_CLIENT_INTERVAL_1HZ_MS (1000)

/*============================================================================
 *                    U T I L I T Y    F U N C T I O N S 
 * =========================================================================*/

/*---------------------------------------------------------------------------
@brief
   This utility function checks to see if the passed handle associated with
   a client is a test master client. Based on the e_FuncMode, if found either
   just query, sets it as test master client or reset it.

@param[in] l_ClientHandle - handle associated with the client to be searched

@return For eLOC_CLNT_TMCF_QUERY if found TRUE is returned. 
        Otherwise, FALSE is returned.
        For eLOC_CLNT_TMCF_SET, if set successfully, TRUE is returned,
        otherwise, FALSE is returned - meaning full array
        For eLOC_CLNT_TMCF_RESET, if found then reset, TRUE is returned.
        Otherwise, FALSE is returned.
---------------------------------------------------------------------------*/
static boolean locClnt_TestMasterClientCheck
(
   loc_client_handle_type l_ClientHandle,
   locClntTestMasterClientFuncMode e_FuncMode
   )
{
  locClnt_ModuleDataType *p_LocClntData = &(loc_middleware_data.z_locClnt_Data);
  uint32 q_ClientCnt = 0;

  if (l_ClientHandle == LOC_CLIENT_HANDLE_INVALID)
  {
    return FALSE;
  }

  // ---------------------
  // Only allow up to LOC_CLIENT_MAX_TEST_MASTER_CLIENTS test master clients
  // -- Implemented as array, as linked list is an overkill for test features
  // -- and array size is not expected to be big
  // ---------------------
  for (q_ClientCnt = 0; q_ClientCnt < LOC_CLIENT_MAX_TEST_MASTER_CLIENTS; q_ClientCnt++)
  {
    // --------
    // For SET operation, we just care for the first empty spot
    // --------
    if (e_FuncMode == eLOC_CLNT_TMCF_SET)
    {
      // Grab the first spot that is empty for SET operation, and we're DONE!
      if (p_LocClntData->lz_TestMasterClientHandles[q_ClientCnt] == LOC_CLIENT_HANDLE_INVALID)
      {
        p_LocClntData->lz_TestMasterClientHandles[q_ClientCnt] = l_ClientHandle;
        return TRUE;
      }
    }

    // --------
    // For QUERY or RESET operation, we care about finding the client in the array
    // --------
    else if (p_LocClntData->lz_TestMasterClientHandles[q_ClientCnt] == l_ClientHandle)
    {
      // Reset the client handle if it is a RESET operation
      if (e_FuncMode == eLOC_CLNT_TMCF_RESET)
      {
        p_LocClntData->lz_TestMasterClientHandles[q_ClientCnt] = LOC_CLIENT_HANDLE_INVALID;
      }

      // In either case, it's found, we're DONE!
      return TRUE;
    }
  }

  // --------
  // If this point is reached, does not matter what operation mode, it is a failure
  // -- for SET: could not find empty spot, array is FULL
  // -- for QUERY/RESET: could not find the client in the array.
  // --------
  return FALSE;
}

/*---------------------------------------------------------------------------
@brief
   This function is the compare function for ascending sort of uint32 value

@param[in] p_ValA - first uint32 value to be compared
           p_ValB - second uint32 value to be compared

@return positive number if ValA > ValB, otherwise -1 is returned
---------------------------------------------------------------------------*/
int locClnt_AscendSortUint32CmpFunc(const void *p_ValA, const void *p_ValB)
{
  uint32 q_ValA = *((uint32 *)p_ValA);
  uint32 q_ValB = *((uint32 *)p_ValB);

  return (((q_ValA > q_ValB) ? (q_ValA - q_ValB) : (-1)));
}

/*---------------------------------------------------------------------------
@brief
   This function sorts the given uint32 array in ascending order

@param[in] z_Array[] array to be sorted
           q_ArrSz array size

@return None
---------------------------------------------------------------------------*/
void locClnt_SortUint32(uint32 z_Array[], uint32 q_ArrSz)
{
  qsort(z_Array, q_ArrSz, sizeof(uint32), locClnt_AscendSortUint32CmpFunc);

  // if qsort not available just perform bubble sort
}

/*---------------------------------------------------------------------------
@brief
   This function checks to see if the given command is one of the Master
   command

@param[in] q_QmiLocMsgId Msg ID associated with the command to be checked

@return TRUE if passed q_Msg is a master cmd, otherwise FALSE
---------------------------------------------------------------------------*/
static boolean locClnt_IsMasterCmd(uint32 q_QmiLocMsgId)
{
  uint32 q_ArrSz = sizeof(z_locClntMasterCmds) / sizeof(z_locClntMasterCmds[0]);
  int32 l_Min = 0;
  int32 l_Max = q_ArrSz - 1;
  int32 l_Idx;

  if (q_ArrSz == 0)
  {
    return FALSE;
  }

  // Do binary search on the Master Cmd list which has been sorted during
  // the initialization
  while (l_Min <= l_Max)
  {
    l_Idx = (l_Min + l_Max) / 2;

    // Robustness - will never happen unless there's grave SW bug or memory corruption
    if ((l_Idx < 0) || (l_Idx >= q_ArrSz))
    {
      LOC_MSG_ERROR("locClnt_IsMasterCmd out of bound l_Idx = %d - Array Sz = %lu", l_Idx, q_ArrSz, 0);
      break;
    }

    if (q_QmiLocMsgId == z_locClntMasterCmds[l_Idx])
    {
      return TRUE;
    }
    // Move to the left half if the value searched is smaller than the midpoint
    else if (q_QmiLocMsgId < z_locClntMasterCmds[l_Idx])
    {
      l_Max = l_Idx - 1;
    }
    // Move to the right half if the value searched is larger than the midpoint
    else
    {
      l_Min = l_Idx + 1;
    }
  }

  // If this point is reached that means the entry was not found
  return FALSE;
}

/*---------------------------------------------------------------------------
@brief
   This function checks to see if the given command is allowed for the given
   client.

@param[in] l_ClientHandle client handle of the client to be checked
           q_QmiLocMsgId Msg ID associated with the command to be checked

@return TRUE if command is allowed for this client, otherwise FALSE
---------------------------------------------------------------------------*/
boolean locClnt_IsCommandAllowed
(
   loc_client_handle_type l_ClientHandle,
   uint32                 q_QmiLocMsgId
   )
{
  locClnt_ModuleDataType *p_LocClntData = &(loc_middleware_data.z_locClnt_Data);
  uint32 q_ClntCnt = 0;

  // If there's no master client registered, then all commands are allowed
  // to be processed from any client - legacy behavior
  if (p_LocClntData->l_MasterClientHandle == LOC_CLIENT_HANDLE_INVALID)
  {
    for (q_ClntCnt = 0; q_ClntCnt < LOC_CLIENT_MAX_TEST_MASTER_CLIENTS; q_ClntCnt++)
    {
      if (p_LocClntData->lz_TestMasterClientHandles[q_ClntCnt] != LOC_CLIENT_HANDLE_INVALID)
      {
        break;
      }
    }

    // If q_ClntCnt is MAX then there isn't any test master client, so commands are allowed
    if (q_ClntCnt >= LOC_CLIENT_MAX_TEST_MASTER_CLIENTS)
    {
      return TRUE;
    }
  }

  // If this is Master client or test master client, then all commands are allowed as well
  if ((p_LocClntData->l_MasterClientHandle == l_ClientHandle) ||
      (locClnt_TestMasterClientCheck(l_ClientHandle, eLOC_CLNT_TMCF_QUERY) == TRUE))
  {
    return TRUE;
  }

  // Otherwise, for a non-Master client, make sure commands is *not* one of the
  // Master commands
  if (locClnt_IsMasterCmd(q_QmiLocMsgId))
  {
    return FALSE;
  }

  return TRUE;
}

/*---------------------------------------------------------------------------------------------
@brief
   This function checks if the interediate/final fix report should be reported to the client
   based on the clients intermediate or final fix reporting interval. 

@param[in] pz_Client Info of the client to which fix reporting interval criteria has to be checked
           l_ClientHandle client handle of the scheduled active client
           t_CurrentTimeMs Current system time in millisec
           b_FinalFix  flag to check if the fix is intermediate/Final .
           

@return    TRUE if the fix can be reported to the client .Otherwise,FALSE.
-------------------------------------------------------------------------------------------------*/
static boolean LocClnt_IsFixReportable(loc_client_info_s_type *pz_Client,
                                       loc_client_handle_type  l_ActiveClientHandle,
                                       uint64  t_CurrentTimeMs,boolean v_FinalFix )
{
 uint64                  t_TimeDeltaMs = 0ULL;
 loc_client_info_s_type  *pz_ActiveClient = NULL;
  
  /* Intermediate position interval check for client */
  if(FALSE == v_FinalFix)
  {
    /*Client hasnt enabled intermediate fix, drop the intermediate fix for client */ 
    if (FALSE == pz_Client->z_FixCriteria.v_IsIntermPosRptOn)
    {
      LOC_MSG_LOW("LocClnt_IsFixReportable, supress the intermediate position report for client %d",
                  pz_Client->client_handle, 0, 0);
      return FALSE;
    }
    else if (0 == pz_Client->z_FixCriteria.q_MinIntervalIntermediate)
    {
      /*Client hasn't mentioned the IntermediatePosInterval,report at default rate */
      LOC_MSG_LOW("locClnt_PositionCallback, q_MinIntervalIntermediate = 0 for client %d, report fix",
                  pz_Client->client_handle, 0, 0);
      return TRUE;
    }
	else
	{
      if (t_CurrentTimeMs > pz_Client->t_LastIntermediateFixReportTime)
      {
        t_TimeDeltaMs = t_CurrentTimeMs - pz_Client->t_LastIntermediateFixReportTime;
      }
      else
      {
         /* In case of time service reset or error case, report this fix */
         t_TimeDeltaMs = (uint64)(pz_Client->z_FixCriteria.q_MinIntervalIntermediate);
      }

      LOC_MSG_LOW("LocClnt_IsFixReportable, intermediate position last_report_time=%u, current_time=%u, interval=%u",
                   pz_Client->t_LastIntermediateFixReportTime, t_CurrentTimeMs,
                   pz_Client->z_FixCriteria.q_MinIntervalIntermediate);
      if (t_TimeDeltaMs < (uint64)(pz_Client->z_FixCriteria.q_MinIntervalIntermediate - LOC_CLIENT_POS_OFFSET))
      {
         LOC_MSG_LOW("LocClnt_IsFixReportable, drop intermediate position for client %d",
                     pz_Client->client_handle, 0, 0);
         return FALSE;
      }
      else
      {
        LOC_MSG_LOW("LocClnt_IsFixReportable, minIntervalIntermediate satisfied, fix reportable for client %d",
                     pz_Client->client_handle, 0, 0);
	    return TRUE;
       }
     }
   }
   else /*final fix */
   {
      /* Final position or unpropogated position */
      if ((pz_Client->client_handle == l_ActiveClientHandle) && (CLIENT_STATE_INACTIVE != pz_Client->e_ClientState))
      {
          /* Do not check time eclapsed for the scheduled client */
         LOC_MSG_LOW("LocClnt_IsFixReportable, always report position for the scheduled client", 0, 0, 0);
         return TRUE;
      }
	  /* Final position report for other clients need interval time check */
      else
      {
        LOC_MSG_LOW("LocClnt_IsFixReportable, final position last_report_time=%u, current_time=%u, interval=%u",
                   pz_Client->t_LastFixReportTime, t_CurrentTimeMs,
                   pz_Client->z_FixCriteria.q_TbfMs);
	  	if (t_CurrentTimeMs > pz_Client->t_LastFixReportTime)
        {
           t_TimeDeltaMs = t_CurrentTimeMs - pz_Client->t_LastFixReportTime;
        }
        else
        {
          /* In case of time service reset or error case, report this fix */
          t_TimeDeltaMs = (uint64)(pz_Client->z_FixCriteria.q_TbfMs);
        }
        pz_ActiveClient = locClnt_FindHandle(l_ActiveClientHandle);
        
		/*For clients TBF greater than 5secs, it's possible to see t_TimeDeltaMs much smaller than expected. Once issue happens, 
		 position loss would be observed and user has to wait until next session. So report final fix as we get for the 
		 tbf is >= 5 seconds for scheduling client. This may cause issues if all the clients have tbf >5 sec . 
		 Currently this is a rare scenario so checking only for the active client */
		 
        if (((NULL != pz_ActiveClient) && (pz_ActiveClient->z_FixCriteria.q_TbfMs >= LOC_CLIENT_TBF_THRESHOLD)) || 
			(t_TimeDeltaMs >= (uint64)(pz_Client->z_FixCriteria.q_TbfMs - LOC_CLIENT_POS_OFFSET)))
        {
           return TRUE;
        }
      }
    }
   return FALSE;
}

/*---------------------------------------------------------------------------
@brief
   This function reads the Supported Minimum Interval millisecond from
   the NV item.

@param[in] None

@return    Time interval in milliseconds set by NV.
---------------------------------------------------------------------------*/
uint32 locClnt_ReadNvMinInterval(void)
{
  uint32  q_SupportedMinInterval = GNSS_NV_EFS_LOC_SUPPORTED_MIN_INTERVAL_DEFAULT; //default value;

  /* read the default NV's */
  if (FALSE == locNvEfsRead(GNSS_NV_EFS_LOC_SUPPORTED_MIN_INTERVAL,
                            (void *)&q_SupportedMinInterval,
                            sizeof(q_SupportedMinInterval)))
  {
    q_SupportedMinInterval = GNSS_NV_EFS_LOC_SUPPORTED_MIN_INTERVAL_DEFAULT; //default value
    LOC_MSG_HIGH("locClnt_ReadNvMinInterval could not read nv for SpportedMinInterval, defaulting it to %u ms",
                 q_SupportedMinInterval, 0, 0);
  }
  else
  {
    LOC_MSG_HIGH("locClnt_ReadNvMinInterval read nv for SpportedMinInterval Success %u ms", q_SupportedMinInterval, 0, 0);
  }

  return q_SupportedMinInterval;
}

/*---------------------------------------------------------------------------
@brief
   Select closest supported time interval for TBF

   The method selects appropriate time interval in milliseconds for time between
   fixes. All intervals are rounded up to the next supported value if they are
   lower, than 1000ms.

   Values over 1000ms are rounded up to the next integer second interval.

   When NHz feature is enabled, the following values are allowed:
   - 250 milliseconds - 4Hz rate;
   - 500 milliseconds - 2Hz rate;
   - N*1000 millseconds - 1Hz rate and lower with one second boundaries.

@param[in] q_IntervalMs Time interval in milliseconds to round up.

@return Time interval in milliseconds supported by engine.
---------------------------------------------------------------------------*/
static uint32 locClnt_MatchFixInterval
(
   uint32 q_IntervalMs
   )
{
  /**
   * @brief Table of supported intervals.
   *
   * This table has to be monotonic with growing values up to 1000ms. When
   * NHz feature is not enabled, only 1000ms is supported.
   */
  static const uint32 pq_IntervalsMs[] =
  {
#ifdef FEATURE_LOCTECH_NHZ
    LOC_CLIENT_INTERVAL_20HZ_MS,
    LOC_CLIENT_INTERVAL_10HZ_MS,
    LOC_CLIENT_INTERVAL_5HZ_MS,
    LOC_CLIENT_INTERVAL_2HZ_MS,
#endif /* FEATURE_LOCTECH_NHZ */
    LOC_CLIENT_INTERVAL_1HZ_MS
  };

  static boolean v_LocNvInit = FALSE;
  static uint32  q_SupportedMinInterval = LOC_CLIENT_INTERVAL_1HZ_MS; // 1 Sec
  uint32         q_RetVal = q_IntervalMs;

  /* Only read NV supported min interval once */
  if (FALSE == v_LocNvInit)
  {
    if (TRUE == locNvEfsInit())
    {
      q_SupportedMinInterval = locClnt_ReadNvMinInterval();
      v_LocNvInit = TRUE;
    }
    else
    {
      LOC_MSG_ERROR("locClnt_MatchFixInterval: locNvEfsInit Failed!", 0, 0, 0);
    }
  }

  /* Restrict the interval to the MIN supported value */
  if (q_RetVal < q_SupportedMinInterval)
  {
    q_RetVal = q_SupportedMinInterval;
  }

  if (q_IntervalMs >= LOC_CLIENT_INTERVAL_1HZ_MS)
  {
    if (0 != q_IntervalMs % LOC_CLIENT_INTERVAL_1HZ_MS)
    {
      q_RetVal = (q_IntervalMs / LOC_CLIENT_INTERVAL_1HZ_MS + 1) *
         LOC_CLIENT_INTERVAL_1HZ_MS;
    }
  }
  else
  {
    uint32 q_Idx = 0;

    for (q_Idx = ARR_SIZE(pq_IntervalsMs); q_Idx; q_Idx--)
    {
      if (q_RetVal >= pq_IntervalsMs[q_Idx - 1])
      {
        q_RetVal = pq_IntervalsMs[q_Idx - 1];
        break;
      }
    }
  }

  LOC_MSG_MED("locClnt_MatchFixInterval: input=%ums output=%ums",
              q_IntervalMs,
              q_RetVal,
              0);

  return q_RetVal;

}

/*---------------------------------------------------------------------------
@brief
   This function compares l_ClientHandle

@param[in] p_ItemPtr    loc_client_info_s_type pointer.
           p_CompareVal l_ClientHandle pointer

@return TRUE if loc_client_info_s_type pointer contains l_ClientHandle value
---------------------------------------------------------------------------*/
static int locClnt_CompareHandle
(
   void *p_ItemPtr,
   void *p_CompareVal
   )
{
  loc_client_info_s_type  *pz_Client = (loc_client_info_s_type *)p_ItemPtr;

  /* item_ptr contains an element of the Client Q i.e. Client Handle, 
    and compare val is also a Client Handle */
  return ((pz_Client->client_handle == *((int *)p_CompareVal)) ? 1 : 0);
}

/*---------------------------------------------------------------------------
@brief
   This function is used to check whether a client with the specific handle exists or not.

@param[in] l_ClientHandle  l_ClientHandle value

@return A pointer to the client.
        If it does not exist, NULL will be returned.
---------------------------------------------------------------------------*/
loc_client_info_s_type* locClnt_FindHandle
(
   loc_client_handle_type l_ClientHandle
   )
{
  loc_client_info_s_type  *pz_Client = NULL;

  pz_Client = (loc_client_info_s_type *)q_linear_search(
     &loc_middleware_data.z_locClnt_Data.z_LocClientQueue,
     locClnt_CompareHandle,
     &l_ClientHandle);

  return pz_Client;
}

/*---------------------------------------------------------------------------
@brief
   This function is used to process loc_open

@param[in] p_EventCb       When loc_client receives client interested event,
                           this is the function to callback to loc_qmi_shim.c

@return A client handle (>=0) when successful.
        -1 if failed
---------------------------------------------------------------------------*/
loc_client_handle_type locClnt_Init
(
   loc_event_cb_f_type     *p_EventCb
   )
{
  locClnt_ModuleDataType *p_LocClntData = &(loc_middleware_data.z_locClnt_Data);
  loc_client_info_s_type *pz_Client = NULL;
  loc_client_handle_type         l_ClientHandle = LOC_CLIENT_HANDLE_INVALID;
  void *pz_Client_mem_uppr_bndry = NULL;
  void *mem_uppr_bndry = NULL;

  if (LOC_CLIENT_MAX_NUM == p_LocClntData->l_MaxNumClients)
  {
    LOC_MSG_ERROR("locClnt_Init l_MaxNumClients = %d",
                  p_LocClntData->l_MaxNumClients, 0, 0);
    return l_ClientHandle;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  pz_Client = (loc_client_info_s_type *)os_MemAlloc(sizeof(loc_client_info_s_type), OS_MEM_SCOPE_TASK);

  if (pz_Client != NULL)
  {
    pz_Client_mem_uppr_bndry = pz_Client + sizeof(loc_client_info_s_type);
    /* Memset to "0" for first use, it might have garbage and cause trouble */
    memset(pz_Client, 0, sizeof(*pz_Client));
    /* Initialize the link, return value is a pointer to the link, 
       not needed */
    (void)q_link(pz_Client, &pz_Client->z_Link);
    /* Add it to the queue of clients */
    q_put(&p_LocClntData->z_LocClientQueue,
          &pz_Client->z_Link);

    p_LocClntData->l_MaxNumClients++;

    pz_Client->v_InUse = TRUE;
    pz_Client->client_handle = p_LocClntData->l_GlobalClientHandle++;

    // Client handle wrapped around
    if (pz_Client->client_handle < 0)
    {
      p_LocClntData->l_GlobalClientHandle = 0;
      pz_Client->client_handle = p_LocClntData->l_GlobalClientHandle++;
    }

    pz_Client->e_ClientState = CLIENT_STATE_INACTIVE;
    pz_Client->t_EventRegMask = 0;
    pz_Client->p_EventCb = p_EventCb;
    pz_Client->v_FirstFix = TRUE;
    pz_Client->v_SingleShotInProgress = FALSE;

    //set the default valid bits in the fix criteria
    pz_Client->z_FixCriteria.q_ValidMask =
       (LOC_FIX_CRIT_VALID_INTERMEDIATE_POS_REPORT_ENABLED |
           LOC_FIX_CRIT_VALID_MIN_DIST_SAMPLE_INTERVAL |
           LOC_FIX_CRIT_VALID_MIN_DISTANCE |
           LOC_FIX_CRIT_VALID_MIN_INTERVAL |
           LOC_FIX_CRIT_VALID_NOTIFY_TYPE |
           LOC_FIX_CRIT_VALID_PREFERRED_ACCURACY |
           LOC_FIX_CRIT_VALID_PREFERRED_OPERATION_MODE |
           LOC_FIX_CRIT_VALID_PREFERRED_RESPONSE_TIME |
           LOC_FIX_CRIT_VALID_RECURRENCE_TYPE);

    // set the default values for each field in the fix criteria
    pz_Client->z_FixCriteria.e_RecurrenceType                 = LOC_CLIENT_FIX_SINGLE;
    pz_Client->z_FixCriteria.v_IsIntermPosRptOn               = TRUE;

    pz_Client->z_FixCriteria.q_TbfMs                          = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT;
    pz_Client->z_FixCriteria.q_PreferredAccuracy              = LOC_CLIENT_FIX_ACCURACY_LOW;
    pz_Client->z_FixCriteria.q_PreferredResponseTime          = LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT;
    pz_Client->z_FixCriteria.e_PreferredOperationMode         = LOC_CLIENT_FIX_MODE_DEFAULT;
    pz_Client->z_FixCriteria.e_NotifyType                     = LOC_CLIENT_NOTIFY_ON_INTERVAL;
    pz_Client->z_FixCriteria.f_MinDistance                    = 0;
    pz_Client->z_FixCriteria.q_MinIntervalIntermediate        = 0;

    mem_uppr_bndry = &pz_Client->z_FixCriteria.z_ApplicationId +
       sizeof(pz_Client->z_FixCriteria.z_ApplicationId);
    if (mem_uppr_bndry < pz_Client_mem_uppr_bndry)
    {
      memset((void *)&pz_Client->z_FixCriteria.z_ApplicationId, 0,
             sizeof(pz_Client->z_FixCriteria.z_ApplicationId));
    }
    else
    {
      ERR_FATAL("memset on z_ApplicationId corrupts adjacent memoriesmemories Client_node_upper_boundary =%d, ApplicationId upper boundary =%d",
                pz_Client_mem_uppr_bndry, mem_uppr_bndry, 0);
    }
    /* Initialize to use default server. Client will use custom server
     * if eQMI_LOC_SERVER_TYPE_CUSTOM_PDE is being called */
    mem_uppr_bndry = &(pz_Client->z_ServerInfo) + sizeof(pdsm_pd_server_info_s_type);
    if (mem_uppr_bndry < pz_Client_mem_uppr_bndry)
    {
      memset(&(pz_Client->z_ServerInfo), 0, sizeof(pdsm_pd_server_info_s_type));
    }
    else
    {
      ERR_FATAL("memset on z_ServerInfo corrupts adjacent memories Client_node_upper_boundary =%d, Server info upper boundary =%d",
                pz_Client_mem_uppr_bndry, mem_uppr_bndry, 0);
    }
    pz_Client->z_ServerInfo.server_option = PDSM_SERVER_OPTION_USE_DEFAULT;

    /* Initialize it to maximum Value, so first final FIX will always be reported */
    pz_Client->t_LastFixReportTime        = ~0;

    pz_Client->v_PositionListener = FALSE;

    /* Initialize the client ID string with its own client handle number - until later
    ** replaced optionally by string provided in REG_EVENTS. Just modulo 1000 of the 
    ** client ID shd be sufficient to keep it under 4 characters */
    (void)snprintf(pz_Client->pb_ClientIdStr, sizeof(pz_Client->pb_ClientIdStr),
                   "%d", (pz_Client->client_handle % 1000));
    pz_Client->v_ClientSetIdStr = FALSE;

    /* Intialize Location Privacy variables */
    pz_Client->e_ClientType = eQMI_LOC_CLIENT_NFW_V02;
    pz_Client->v_EnablePosRequestNotification = FALSE;
    pz_Client->q_ReportEventRecord = 0;

    l_ClientHandle = pz_Client->client_handle;
    LOC_MW_CLIENT_MSG_MED(pz_Client, "locClnt_Init returned l_ClientHandle = %d, pz_Client = 0x%x, l_MaxNumClients = %d",
                          l_ClientHandle, pz_Client, p_LocClntData->l_MaxNumClients);
  }
  else
  {
    LOC_MSG_ERROR("locClnt_Init failed to alloate loc_client_info_s_type", 0, 0, 0);
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  return l_ClientHandle;
}

/*---------------------------------------------------------------------------
@brief
   When loc_close is called, locClnt_Release will be called to free the resource
   being used by this client

@param[in] l_ClientHandle  client handle

@return LOC_API_SUCCESS if success
        other value indicates error
---------------------------------------------------------------------------*/
uint32 locClnt_Release(loc_client_handle_type  l_ClientHandle)
{
  loc_client_info_s_type *pz_Client;
  uint32                         q_RetVal = LOC_API_INVALID_HANDLE;
  locClnt_ModuleDataType *p_LocClntData = &(loc_middleware_data.z_locClnt_Data);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
  pz_Client = locClnt_FindHandle(l_ClientHandle);

  LOC_MW_CLIENT_MSG_MED(pz_Client, "locClnt_Release pz_Client = 0x%x", pz_Client, 0, 0);

  if (pz_Client == NULL)
  {
    LOC_MSG_ERROR("locClnt_Release, invalid client handle", l_ClientHandle, 0, 0);
  }
  else
  {
    /* If it IS the master client who disconnects, then we need to treat that as a
    ** de-registration of the Master client, then there will no longer be master client */
    if (l_ClientHandle == p_LocClntData->l_MasterClientHandle)
    {
      p_LocClntData->l_MasterClientHandle = LOC_CLIENT_HANDLE_INVALID;
    }

    /* Do not care about return value - just reset if found */
    (void)locClnt_TestMasterClientCheck(l_ClientHandle, eLOC_CLNT_TMCF_RESET);

    /* Remove from Queue */
    if (TRUE != q_delete_ext(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue,
                             &pz_Client->z_Link))
    {
      LOC_MSG_ERROR("locClnt_Release, could not delete the loc client from queue ", 0, 0, 0);
    }
    else
    {
      /* Free the memory if delete queue is successful */
      os_MemFree((void **)&pz_Client);
      q_RetVal = LOC_API_SUCCESS;
      loc_middleware_data.z_locClnt_Data.l_MaxNumClients--;
    }
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  LOC_MSG_MED("locClnt_Release for l_ClientHandle = %d, q_RetVal = %u, l_MaxNumClients = %d",
              l_ClientHandle, q_RetVal, loc_middleware_data.z_locClnt_Data.l_MaxNumClients);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   This function will find the higher pirority operation mode between the 
   e_OperationMode and pz_LocFixCriteria->e_OperationMode
   if the operation mode is MSA or MSB, save the applicationID to the global value

@param[in] e_OperationMode  operation mode
           pz_LocFixCriteria  A FIX criteria contains an operation mode

@return The higher priority operation mode

---------------------------------------------------------------------------*/
static qmiLocOperationModeEnumT_v02 locClnt_CompareOperationMode
(
   qmiLocOperationModeEnumT_v02      e_OperationMode,
   locClnt_FixCriteriaType           *pz_LocFixCriteria
   )
{
  locClntOperationPriorityEnumT e_Prority1 = eLOC_CLNT_OPER_MODE_PRIORITY_UNKNOWN;
  locClntOperationPriorityEnumT e_Prority2 = eLOC_CLNT_OPER_MODE_PRIORITY_UNKNOWN;
  qmiLocOperationModeEnumT_v02  e_HighMode = e_OperationMode;
  uint32                        q_Count = 0;

  if ((eQMI_LOC_OPER_MODE_DEFAULT_V02 > e_OperationMode) ||
      (eQMI_LOC_OPER_MODE_WWAN_V02 < e_OperationMode))
  {
    LOC_MSG_ERROR("locClnt_CompareOperationMode invalid e_OperationMode = %d", e_OperationMode, 0, 0);
    return eQMI_LOC_OPER_MODE_DEFAULT_V02;
  }

  if (NULL == pz_LocFixCriteria)
  {
    LOC_MSG_ERROR("locClnt_CompareOperationMode NULL pointer 0x%x", pz_LocFixCriteria, 0, 0);
    return e_OperationMode;
  }

  if (e_OperationMode == pz_LocFixCriteria->e_PreferredOperationMode)
  {
    return e_OperationMode;
  }
  else
  {
    for (q_Count = 0; q_Count < sizeof(pz_locClntOperationModeRule) / sizeof(pz_locClntOperationModeRule[0]); q_Count++)
    {
      /* found the priority for the current operation mode */
      if (pz_locClntOperationModeRule[q_Count].e_OperationMode == e_OperationMode)
      {
        e_Prority1 = pz_locClntOperationModeRule[q_Count].e_prority;
      }
      /* found the priority for this client's operation mode */
      if (pz_locClntOperationModeRule[q_Count].e_OperationMode == pz_LocFixCriteria->e_PreferredOperationMode)
      {
        e_Prority2 = pz_locClntOperationModeRule[q_Count].e_prority;
      }
      if ((eLOC_CLNT_OPER_MODE_PRIORITY_UNKNOWN != e_Prority1) && (eLOC_CLNT_OPER_MODE_PRIORITY_UNKNOWN != e_Prority2))
      {
        break;
      }
    }
  }

  /* application_ID only apply to MSA, MSB */
  if ((e_Prority2 > e_Prority1) &&
      (e_Prority2 >= eLOC_CLNT_OPER_MODE_PRIORITY_MSB) &&
      (pz_LocFixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_APPLICATION_ID))
  {
    LOC_MSG_MED("locClnt_CompareOperationMode set application_ID for e_OperationMode = %d",
                pz_LocFixCriteria->e_PreferredOperationMode, 0, 0);
    loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.z_ApplicationId = pz_LocFixCriteria->z_ApplicationId;
  }

  if (e_Prority1 > e_Prority2)
  {
    e_HighMode = e_OperationMode;
  }
  else
  {
    e_HighMode = pz_LocFixCriteria->e_PreferredOperationMode;
  }

  return e_HighMode;
}

/*---------------------------------------------------------------------------
@brief
   This function will go through all the active and "willing to share" clients,
   Loc client decide the best FIX criteria and set the value in
   loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule

@param[in] NULL

@return NULL

---------------------------------------------------------------------------*/
void locClnt_SetCommonPosCriteria(void)
{
  loc_client_info_s_type            *pz_Client = NULL;
  boolean                            v_FirstClient = TRUE;
  uint32                             q_MinFixInterval = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT;   /* 1 second */
  qmiLocOperationModeEnumT_v02       e_PreferredOperationMode = eQMI_LOC_OPER_MODE_CELL_ID_V02;
  uint32                             q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_HIGH;
  uint32                             q_PreferredResponseTime = LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT;
  locClnt_FixRecurrenceEnumType      e_PreferredRecurrence = LOC_CLIENT_FIX_SINGLE;
  qmiLocPowerModeStructT_v02         z_PowerMode = { eQMI_LOC_POWER_MODE_BACKGROUND_KEEP_WARM_V02, 0 }; /* least power consuming mode */
   qmiLocSpecialReqEnumT_v02          e_SpecialRequestType = QMILOCSPECIALREQENUMT_MAX_ENUM_VAL_V02;                       

   /* This is to ensure we unset the SpecialRequest flag from Common criteria*/
   loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_ValidMask &= ~(LOC_FIX_SPECIAL_REQUEST);

  for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
  {
    if (TRUE == pz_Client->v_InUse &&
        CLIENT_STATE_INACTIVE != pz_Client->e_ClientState &&
        TRUE == pz_Client->z_FixCriteria.v_SharePosition)
    {
      /* higher power consuming mode and less TBM */
      if ((TRUE == v_FirstClient) ||
          (pz_Client->z_FixCriteria.z_PowerMode.powerMode < z_PowerMode.powerMode))
      {
        z_PowerMode = pz_Client->z_FixCriteria.z_PowerMode;
      }
      else if (pz_Client->z_FixCriteria.z_PowerMode.powerMode == z_PowerMode.powerMode)
      {
        if (pz_Client->z_FixCriteria.z_PowerMode.timeBetweenMeasurement < z_PowerMode.timeBetweenMeasurement)
        {
          z_PowerMode.timeBetweenMeasurement = pz_Client->z_FixCriteria.z_PowerMode.timeBetweenMeasurement;
        }
      }

      /* Operation mode and Application ID */
      e_PreferredOperationMode = locClnt_CompareOperationMode(e_PreferredOperationMode,
                                                              &(pz_Client->z_FixCriteria));

      /* 1. Set q_MinFixInterval to the first client's q_TbfMs
         2. Compare q_MinFixInterval with the next client's q_TbfMs, set to the smaller value */
      if ((TRUE == v_FirstClient) ||
          (pz_Client->z_FixCriteria.q_TbfMs < q_MinFixInterval))
      {
        q_MinFixInterval = pz_Client->z_FixCriteria.q_TbfMs;
      }

      /* Accuracy and ResponseTime */
      if ((TRUE == v_FirstClient) ||
          pz_Client->z_FixCriteria.q_PreferredAccuracy < q_PreferredAccuracy)
      {
        q_PreferredAccuracy = pz_Client->z_FixCriteria.q_PreferredAccuracy;
        q_PreferredResponseTime = pz_Client->z_FixCriteria.q_PreferredResponseTime;
      }

      /* e_RecurrenceType */
      if ((TRUE == v_FirstClient) ||
          pz_Client->z_FixCriteria.e_RecurrenceType < e_PreferredRecurrence)
      {
        e_PreferredRecurrence = pz_Client->z_FixCriteria.e_RecurrenceType;
      }

      v_FirstClient = FALSE;

         /*Special request will be acepted as reqular requests during 911. Do not set the SpecialRequestType 
           if LocMW is in 911*/
         if ( ( eQMI_LOC_SPECIAL_REQUEST_SHORT_CODE_V02 == pz_Client->z_FixCriteria.e_SpecialRequestType ) &&
              ( FALSE == loc_middleware_data.v_E911Session ) ) 
         {
           loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_ValidMask |= LOC_FIX_SPECIAL_REQUEST;
           e_SpecialRequestType = eQMI_LOC_SPECIAL_REQUEST_SHORT_CODE_V02;
    }
  }
   }

  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.e_PreferredOperationMode = e_PreferredOperationMode;
  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_TbfMs = q_MinFixInterval;
  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_PreferredAccuracy = q_PreferredAccuracy;
  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_PreferredResponseTime = q_PreferredResponseTime;
  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.e_RecurrenceType = e_PreferredRecurrence;
  loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.z_PowerMode = z_PowerMode;
   loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.e_SpecialRequestType = e_SpecialRequestType;

  LOC_MSG_MED("locClnt_SetCommonPosCriteria, mode = %d,q_TbfMs = %d,q_PreferredAccuracy=%d",
              e_PreferredOperationMode, q_MinFixInterval, q_PreferredAccuracy);
  LOC_MSG_MED("locClnt_SetCommonPosCriteria, responseTime = %d, Recurrence = %d, poweMode=%d",
              q_PreferredResponseTime, e_PreferredRecurrence, z_PowerMode.powerMode);
   LOC_MSG_MED("locClnt_SetCommonPosCriteria SpecialReqType: %d, q_ValidMask 0x%X",e_SpecialRequestType,
               loc_middleware_data.z_locPd_Data.loc_fix_criteria_rule.q_ValidMask,0);

  return;
}

static bool locClnt_PowerModeUpdateDetect(loc_client_handle_type  l_ClientHandle)
{
  loc_client_info_s_type *pz_Client = NULL;
  qmiLocPowerModeEnumT_v02   e_CurrentPowerMode;
  qmiLocPowerModeEnumT_v02   e_NewPowerMode;
  boolean                    v_MoInProgress;

  pz_Client = locClnt_FindHandle(l_ClientHandle);
  if (pz_Client == NULL)
  {
    LOC_MSG_ERROR("locClnt_PowerModeUpdateDetect, invalid client handle", l_ClientHandle, 0, 0);
    return FALSE;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  /* Get the active session's power mode */
  switch (loc_middleware_data.z_locPd_Data.pdsm_option.power_mode.powerMode)
  {
    case PDSM_SESSION_POWER_MODE_IMPROVED_ACCURACY:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_IMPROVED_ACCURACY_V02;
      break;
    case PDSM_SESSION_POWER_MODE_NORMAL:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_NORMAL_V02;
      break;
    case PDSM_SESSION_POWER_MODE_BACKGROUND_DEFINED_POWER:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_POWER_V02;
      break;
    case PDSM_SESSION_POWER_MODE_BACKGROUND_DEFINED_TIME:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_TIME_V02;
      break;
    case PDSM_SESSION_POWER_MODE_BACKGROUND_KEEP_WARM:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_BACKGROUND_KEEP_WARM_V02;
      break;
    default:
      e_CurrentPowerMode = eQMI_LOC_POWER_MODE_NORMAL_V02;
      break;
  }
  v_MoInProgress = loc_middleware_data.z_locPd_Data.mo_fix_in_progress;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
  e_NewPowerMode = pz_Client->z_FixCriteria.z_PowerMode.powerMode;

  /* If power mode need to be upgrade and there is an acitve MO session
     End the active session
     If there is a NI session, pdsm_get_position will be
     rejected by SM and send CmdCb with error */
  LOC_MSG_MED("locClnt_PowerModeUpdateDetect e_NewPowerMode = %d, e_CurrentPowerMode = %d, v_MoInProgress = %u ",
              e_NewPowerMode, e_CurrentPowerMode, v_MoInProgress);
  if (e_NewPowerMode < e_CurrentPowerMode &&
      e_NewPowerMode <= eQMI_LOC_POWER_MODE_NORMAL_V02 &&
      v_MoInProgress)
  {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------
@brief
   This function is called to proces loc_start_fix.

@param[in] client handle

@return LOC_CLIENT_HANDLE_INVALID if the passed-in client handle is not valid
        LOC_API_SUCCESS if success
---------------------------------------------------------------------------*/
int locClnt_StartFix(loc_client_handle_type  l_ClientHandle)
{
  loc_client_info_s_type *pz_Client = NULL;
  time_type               z_TimeMs = { 0 };
  uint32                  q_RetVal = LOC_API_SUCCESS;
  boolean                 v_ClientStateChanged = FALSE;
  boolean                 v_PowerModeUpdateDetect = FALSE;



  LOC_MSG_MED("locClnt_StartFix for l_ClientHandle = %d", l_ClientHandle, 0, 0);

  v_PowerModeUpdateDetect = locClnt_PowerModeUpdateDetect(l_ClientHandle);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
  pz_Client = locClnt_FindHandle(l_ClientHandle);
  if (pz_Client == NULL)
  {
    LOC_MSG_ERROR("locClnt_StartFix, invalid client handle", l_ClientHandle, 0, 0);
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
    return LOC_API_INVALID_HANDLE;
  }

  if (pz_Client->z_FixCriteria.e_RecurrenceType == LOC_CLIENT_FIX_PERIODIC)
  {
    // First fix has not been requested yet when client state is either
    // INACTIVE or TRANSIENT, in this case, set up the necessary information
    if (pz_Client->e_ClientState != CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION)
    {
      pz_Client->v_FirstFix = TRUE;
      time_get_uptime_ms(z_TimeMs);
      pz_Client->t_LastFixRequestTime = QW_CVT_Q2N(z_TimeMs);
      /* Initialize it to maximum Value, so first final FIX will always be reported */
      pz_Client->t_LastFixReportTime = ~0;
      /*IntermediateFix,SvInfo,MeasInfo to be reported immediately for the client on a new start, 
        so intialize them to zero */ 
	  pz_Client->t_LastIntermediateFixReportTime = 0;
	  pz_Client->t_LastMeasInfoReportTime = 0;
	  pz_Client->t_LastSvInfoReportTime = 0;
	  
      LOC_MSG_MED("locClnt_StartFix, t_LastIntermediateFixReportTime = %u:%u",
                  (uint32)(pz_Client->t_LastIntermediateFixReportTime >> 32),
                  (uint32)(pz_Client->t_LastIntermediateFixReportTime & CGPS_MAX_UINT32), 0);
    }

    //clear the single shot started but not stopped flag
    pz_Client->v_SingleShotInProgress = FALSE;
    LOC_MSG_MED("locClnt_StartFix: periodic fix req. clearing v_SingleShotInProgress flag",
                0, 0, 0);
    pz_Client->e_ClientState = CLIENT_STATE_ACTIVE;
    v_ClientStateChanged = TRUE;

  }
  // Single shot
  else
  {
    if ((pz_Client->e_ClientState == CLIENT_STATE_INACTIVE) ||
        (pz_Client->e_ClientState == CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION) ||
        (TRUE == v_PowerModeUpdateDetect))
    {
      pz_Client->e_ClientState = CLIENT_STATE_TRANSIENT;
      time_get_uptime_ms(z_TimeMs);
      pz_Client->t_LastFixRequestTime = QW_CVT_Q2N(z_TimeMs);
      /* Initialize it to maximum Value, so first final FIX will always be reported */
      pz_Client->t_LastFixReportTime = ~0;

      //set the single shot started but not stopped flag
      pz_Client->v_SingleShotInProgress = TRUE;
      LOC_MSG_MED("locClnt_StartFix: single shot req. setting v_SingleShotInProgress flag",
                  0, 0, 0);
      v_ClientStateChanged = TRUE;

    }
    // Position being requested when client is active, ignore the request
    else
    {
      LOC_MSG_MED("locClnt_StartFix: client is already requesting fix", 0, 0, 0);
    }
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  if (TRUE == v_ClientStateChanged)
  {
    /* Set fix criteria for all clients */
    locClnt_SetCommonPosCriteria();

    if (v_PowerModeUpdateDetect)
    {
      //end_session_type
      pdsm_pd_end_session_opt_s_type z_EndSessionOpt;

      z_EndSessionOpt.receiver_off = TRUE;
      z_EndSessionOpt.v_reason_service_interaction = FALSE;
      locPd_endSession(z_EndSessionOpt);

      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      os_TimerStop(loc_middleware_data.z_locPd_Data.fix_process_timer);
      loc_middleware_data.z_locPd_Data.mo_fix_in_progress = FALSE;
      /* set a flag of end session reason, to differentiate with client's stop */
      loc_middleware_data.z_locPd_Data.v_EndSessionForPowerModeSwitch = TRUE;
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
      LOC_MSG_MED("locClnt_StartFix for client = %d, end active session for power mode change",
                  l_ClientHandle, 0, 0);
      return q_RetVal;
    }

    /* run the fix scheduler */
    loc_pd_process_fix_request(eLOC_PD_START_REQUEST);
  }

  LOC_MW_CLIENT_MSG_MED(pz_Client, "locClnt_StartFix for client = %d, return value = %d", l_ClientHandle, q_RetVal, 0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   This function is called to proces loc_stop_fix.

@param[in] client handle

@return TRUE if the client state changed, eg: from requesting position to idle
---------------------------------------------------------------------------*/
int locClnt_StopFix(loc_client_handle_type  l_ClientHandle)
{
  uint32                  q_RetVal = LOC_API_SUCCESS;
  loc_client_info_s_type *pz_Client = NULL;

  pz_Client = locClnt_FindHandle(l_ClientHandle);

  do
  {
    if (pz_Client == NULL)
    {
      LOC_MSG_ERROR("locClnt_StopFix, invalid client handle", l_ClientHandle, 0, 0);
      q_RetVal = LOC_API_INVALID_HANDLE;
      break;
    }

    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

    /* for tracking clients drop the stop if state is inactive
       but for single shot clients allow the stop to go through
       if a stop has not been received. Single shot clients become
       INACTIVE after the position report is generated, but the engine
       will be kept on unless explicitly stopped. */

    if (pz_Client->e_ClientState == CLIENT_STATE_INACTIVE &&
        FALSE == pz_Client->v_SingleShotInProgress)
    {
      LOC_MSG_MED("locClnt_StopFix client %d is already inactive,"
                  " not doing anything for the stop request",
                  l_ClientHandle, 0, 0);

      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

      break;
    }

    LOC_MSG_MED("locClnt_StopFix client = %d, state is %d ss flag is %d  "
                "scheduling a stop", pz_Client->client_handle,
                pz_Client->e_ClientState, pz_Client->v_SingleShotInProgress);

    /* When client state becomes inactive, no more position requests will
        be processed on behalf of this client */

    pz_Client->e_ClientState = CLIENT_STATE_INACTIVE;

    //clear the single shot started but not stopped flag
    pz_Client->v_SingleShotInProgress = FALSE;

    LOC_MSG_HIGH("locClnt_StopFix: clearing v_SingleShotInProgress flag", 0, 0, 0);
    pz_Client->z_ServerInfo.server_option = PDSM_SERVER_OPTION_USE_DEFAULT;

    loc_middleware_data.z_locClnt_Data.l_StopClientHandle = pz_Client->client_handle;

      // Remove SpecialRequestFixCriteria for the client calling StopFix, if the client had requested
      if ( eQMI_LOC_SPECIAL_REQUEST_SHORT_CODE_V02 == pz_Client->z_FixCriteria.e_SpecialRequestType ) 
      {
         pz_Client->z_FixCriteria.e_SpecialRequestType = QMILOCSPECIALREQENUMT_MAX_ENUM_VAL_V02;
      }

    // Recalculate fix criteria for all clients
    locClnt_SetCommonPosCriteria();

    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

    //run the fix scheduler
    loc_pd_process_fix_request(eLOC_PD_STOP_REQUEST);

  }while (0);

  LOC_MW_CLIENT_MSG_MED(pz_Client, "locClnt_StopFix for client = %d, return value = %d", l_ClientHandle, q_RetVal, 0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   This function goes through the queue and determines which client is ready to schedule fix next.
   The client with the minimal value of (last request time + q_FixInterval) will be returned.

@param[in/out] loc client info double pointer, this is the client will be scheduled next

@return The minimum wait time in millisecond among all the clients
---------------------------------------------------------------------------*/
int locClnt_GetMinWaitTime(loc_client_info_s_type **ppz_MinTimeClient)
{
  loc_client_info_s_type *pz_Client = NULL;
  time_type               z_TimeMs = { 0 };
  uint64                  t_CurrentTimeMs;
  uint64                  t_TimeDeltaMs;
  dword                   q_LastFixTtf;
  double                  d_TempWaitTime;

  // initialize variable to over range so error can be easily detected
  double                  d_WaitTime = (double)INT_MAX + 1.0;
  uint32                  q_FixInterval;
  boolean                 v_FirstClientPtr = TRUE;

  time_get_uptime_ms(z_TimeMs);
  t_CurrentTimeMs = QW_CVT_Q2N(z_TimeMs);

  LOC_MSG_MED("locClnt_GetMinWaitTime t_CurrentTimeMs = %u", t_CurrentTimeMs, 0, 0);
  if (NULL != ppz_MinTimeClient)
  {
    *ppz_MinTimeClient = NULL;
  }
  else
  {
    LOC_MSG_ERROR("locClnt_GetMinWaitTime NULL ppz_MinTimeClient", 0, 0, 0);
    return 0;
  }

  /* CRIT_SECTION taken care in the caller function */
  for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
  {
    if (TRUE == pz_Client->v_InUse &&
        (locClnt_IsPosRequestAllowed(pz_Client)||
         locClnt_IsSpecialPosRequest(pz_Client->z_FixCriteria.e_SpecialRequestType)))
    {
      if (CLIENT_STATE_ACTIVE == pz_Client->e_ClientState ||
          CLIENT_STATE_TRANSIENT == pz_Client->e_ClientState)
      {
        // Calculate wait time based on last fix request time and current time
        t_TimeDeltaMs = t_CurrentTimeMs - pz_Client->t_LastFixRequestTime;

        d_TempWaitTime = 0 - (double)t_TimeDeltaMs;

        if ((d_TempWaitTime <= INT_MAX) && (d_TempWaitTime >= INT_MIN))
        {
          LOC_MSG_MED("locClnt_GetMinWaitTime, client %d d_TempWaitTime %d, t_LastFixRequestTime %u",
                      pz_Client->client_handle, (int32)d_TempWaitTime, pz_Client->t_LastFixRequestTime);
        }
        else
        {
          LOC_MSG_MED("locClnt_GetMinWaitTime: temp wait time is over range", 0, 0, 0);
        }

        // Adjust for fix interval
        if ((CLIENT_STATE_ACTIVE == pz_Client->e_ClientState) &&
            (FALSE == pz_Client->v_FirstFix))
        {
          q_FixInterval = locClnt_GetFixInterval(&(pz_Client->z_FixCriteria));
          // to avoid overflow issue
          d_TempWaitTime += q_FixInterval;
        }

        if ((TRUE == v_FirstClientPtr) ||
            (d_TempWaitTime < d_WaitTime))

        {
          d_WaitTime    = d_TempWaitTime;
          *ppz_MinTimeClient = pz_Client;
          v_FirstClientPtr = FALSE;
        }
      }
    }
  }

  // there is no active client, wait forever
  if (TRUE == v_FirstClientPtr)
  {
    return d_WaitTime;
  }

  // If there is no expired client, we need to set waitTime to 0
  // when the client TBF < 1s(LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT)
  q_FixInterval = locClnt_GetFixInterval(&((*ppz_MinTimeClient)->z_FixCriteria));
  if ((LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT >= q_FixInterval) &&
      (d_WaitTime > 0) &&
      ((q_FixInterval - d_WaitTime) > LOC_CLIENT_POS_OFFSET))
  {
    LOC_MSG_MED("locClnt_GetMinWaitTime, client %d interval %d less than default, d_WaitTime %d",
                (*ppz_MinTimeClient)->client_handle, q_FixInterval, d_WaitTime);
    d_WaitTime = 0;
  }

  // only apply to TBF <= 1s
  if (q_FixInterval <= LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT)
  {
    // Retrieve info regarding how long it takes to process the last fix
    q_LastFixTtf = loc_middleware_data.z_locPd_Data.last_fix_ttf;

    // Shorten the wait interval to avoid the case we missed the fix from the engine,
    if ((q_LastFixTtf != 0) && (q_LastFixTtf < MAXIMUM_OVERHEAD_TIME))
    {
      d_WaitTime -= (MAXIMUM_OVERHEAD_TIME - q_LastFixTtf);
    }
  }

  LOC_MSG_MED("locClnt_GetMinWaitTime, set active client %d adjusted d_WaitTime %d \n",
              (*ppz_MinTimeClient)->client_handle, (int32)d_WaitTime, 0);

  return d_WaitTime;
}

/*---------------------------------------------------------------------------
@brief
   This function goes through the queue and determines which client is ready to schedule fix next.
   And then translate QMI_LOC FIX criteria to pdsm_get_position FIX criteria

@param[in/out] None

@return The minimum wait time in millisecond among all the clients
---------------------------------------------------------------------------*/
int locClnt_SetupRequestConfig()
{
  loc_client_info_s_type *pz_Client = NULL;
  time_type               z_TimeMs = { 0 };
  uint64                  t_CurrentTimeMs;

  // initialize variable to over range so error can be easily detected
  double                  d_WaitTime = (double)INT_MAX + 1.0;

  time_get_uptime_ms(z_TimeMs);
  t_CurrentTimeMs = QW_CVT_Q2N(z_TimeMs);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  // First, check whether if there is cell id request that can be serviced from
  // cached position report
  loc_pd_process_concurrent_cellid_request();

  // After cell id fix requests have been serviced from cached position report
  // every fix request will need to be serviced by calling pdsm_get_position

  d_WaitTime = locClnt_GetMinWaitTime(&pz_Client);
  if ((NULL != pz_Client) && (d_WaitTime <= 0))
  {
    // there is one active client whose request need to be serviced, save the
    // configure from that application into the global setting
    loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle = pz_Client->client_handle;

    pz_Client->t_LastFixRequestTime = t_CurrentTimeMs;
    pz_Client->v_FirstFix = FALSE;

    // change this client state to CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION
    if (pz_Client->e_ClientState == CLIENT_STATE_TRANSIENT)
    {
      pz_Client->e_ClientState = CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION;
    }

    // Translate pd fix config(fix criteria and server information)
    // from loc API to PD API
    locPd_ConvertFixCfg2Pdapi(pz_Client);
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  if (NULL == pz_Client)
  {
    LOC_MSG_HIGH("locClnt_SetupRequestConfig, no client is waiting, d_WaitTime %d \n",
                 (int32)d_WaitTime, 0, 0);
    return INT32_MAX;
  }
  else if (d_WaitTime >= (INT32_MAX - 1))
  {
    LOC_MSG_MED("locClnt_SetupRequestConfig, d_WaitTime %d l_ClientHandle = %d \n",
                (int32)d_WaitTime, pz_Client->client_handle, 0);
    return (INT32_MAX - 1);
  }
  else
  {
    LOC_MSG_MED("locClnt_SetupRequestConfig, d_WaitTime %d l_ClientHandle = %d \n",
                (int32)d_WaitTime, pz_Client->client_handle, 0);
    return (int32)d_WaitTime;
  }
}

/*---------------------------------------------------------------------------
@brief
   This is a helper function to tell if there is an active client or not

@param[in]  None

@return True if any of the location client is active
---------------------------------------------------------------------------*/
boolean locClnt_IsAnyRequestingFix()
{
  loc_client_info_s_type *pz_Client = NULL;
  boolean                 v_RetVal = FALSE;

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
  for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
  {
    if ((pz_Client->v_InUse == TRUE) &&
        ((pz_Client->e_ClientState == CLIENT_STATE_ACTIVE) ||
         (pz_Client->e_ClientState == CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION) ||
         (pz_Client->e_ClientState == CLIENT_STATE_TRANSIENT)))
    {
      v_RetVal = TRUE;
      break;
    }
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  LOC_MSG_MED("locClnt_IsAnyRequestingFix %d", v_RetVal, 0, 0);

  return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Update the client state after a fix is done. If a client is requesting
   a single-shot fix, after the single shot fix is serviced, the client state
   will be updated to INACTIVE.

@param[in]  None

@return TRUE
---------------------------------------------------------------------------*/
boolean locClnt_UpdateStateAfterFix(void)
{
  loc_client_info_s_type *pz_Client = NULL;

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
  pz_Client = locClnt_FindHandle(loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle);
  if (pz_Client != NULL)
  {
    if (pz_Client->e_ClientState == CLIENT_STATE_TRANSIENT_WAIT_FOR_POSITION)
    {
      LOC_MSG_MED("locClnt_UpdateStateAfterFix client = %d requested single shot"
                  "changing state from transient wait for positin to inactive ",
                  pz_Client->client_handle, 0, 0);
      pz_Client->e_ClientState = CLIENT_STATE_INACTIVE;
      pz_Client->z_ServerInfo.server_option = PDSM_SERVER_OPTION_USE_DEFAULT;
      loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle = LOC_CLIENT_HANDLE_INVALID;
      locClnt_SetCommonPosCriteria();
    }
    else if (pz_Client->e_ClientState == CLIENT_STATE_INACTIVE)
    {
      /* Set active client handle to INVALID for clients that are inactive */
      LOC_MSG_MED("locClnt_UpdateStateAfterFix client = %d was in inactive state",
                  pz_Client->client_handle, 0, 0);
      loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle = LOC_CLIENT_HANDLE_INVALID;
    }
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  return TRUE;
}

/*---------------------------------------------------------------------------
@brief
   Returns the client that is requesting the fix

@param[in]  None

@return Active client handle
---------------------------------------------------------------------------*/
loc_client_handle_type locClnt_GetActiveClient(void)
{
  return loc_middleware_data.z_locClnt_Data.l_ActiveClientHandle;
}

/*---------------------------------------------------------------------------
@brief
   This function is used to process the cmd request from the loc middleware cmd queue.
   For example, if client issues an RPC call to retrieve information, the inforamtion
   can not be sent directly via callbacks, it will have to be put in the location
   middleware cmd queue and from there, the callback can be issued via RPC callback to
   the client.

@param[in]  pz_Cmd

@return LOC_API_SUCCESS if success
        other value if failed
---------------------------------------------------------------------------*/
uint32 locClnt_ProcessCmdRequest(const LocMW_LocCmdStructT *pz_Cmd)
{
  uint32                  q_RetVal = LOC_API_GENERAL_FAILURE;
  LocMW_LocCmdEnumT       e_CmdType;
  loc_client_handle_type  l_ClientHandle;


  if (NULL == pz_Cmd)
  {
    LOC_MSG_ERROR("locClnt_ProcessCmdRequest NULL pz_Cmd", 0, 0, 0);
    return q_RetVal;
  }

  e_CmdType = pz_Cmd->e_CmdType;
  l_ClientHandle = pz_Cmd->l_ClientHandle;

  if (LOC_CMD_TYPE_NOTIFY_CLIENT == e_CmdType)
  {
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
     if ((LOC_EVENT_GNSS_MEASUREMENT_REPORT == pz_Cmd->t_EventType) ||
         (LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT == pz_Cmd->t_EventType))
    {
      qmiLocEventGnssSvMeasInfoIndMsgT_v02  *pz_GnssSvMeasIndMsg = (qmiLocEventGnssSvMeasInfoIndMsgT_v02 *)pz_Cmd->p_QmiEventIndMsg;
       LOC_MSG_HIGH_3("EVENT 0x%X: Received IPC 10 from SM Task. Sequence %d of %d",
                      pz_Cmd->t_EventType, pz_GnssSvMeasIndMsg->seqNum, pz_GnssSvMeasIndMsg->maxMessageNum);

    }
#endif
    if (TRUE == locClnt_InvokeCallback(l_ClientHandle,
                                       pz_Cmd->t_EventType,
                                       pz_Cmd->p_QmiEventIndMsg))
    {
      q_RetVal = LOC_API_SUCCESS;
    }
  }
  else
  {
    q_RetVal = LOC_API_UNSUPPORTED;
  }

  if (LOC_API_SUCCESS != q_RetVal)
  {
    LOC_MSG_HIGH("locClnt_ProcessCmdRequest event 0X%08x not sent to the client", pz_Cmd->t_EventType, 0, 0);
  }

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Helper function to check the event type, session related event
   request the client must has an active autotracking session.

@param[in]  t_EventType

@return TRUE/FALSE
---------------------------------------------------------------------------*/
static boolean locClnt_IsAutotrackSessionEventType
(
   locClnt_EventMaskType t_EventType
   )
{
  if (LOC_EVENT_SV_POLY_REPORT == t_EventType ||
      LOC_EVENT_SATELLITE_REPORT == t_EventType ||
      LOC_EVENT_GNSS_MEASUREMENT_REPORT == t_EventType ||
      LOC_EVENT_NMEA_POSITION_REPORT == t_EventType ||
      LOC_EVENT_NMEA_1HZ_REPORT == t_EventType ||
      LOC_EVENT_PARSED_POSITION_REPORT == t_EventType ||
      LOC_EVENT_GNSS_MEASUREMENT_REPORT == t_EventType)
  {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------
@brief
   This function is used to invoke the callback for the client. If the
   l_ClientHandle is LOC_HANDLE_REGISTERED_CLIENTS, then all clients registered
   for this particular event will get the callback. Otherwise, only the specified
   client handle that regiteres for the callback will get the event.

@param[in]  l_ClientHandle    client handle
            t_EventType       client interested event type
            p_EventPayload    event callback data payload

@return TRUE if success
        FALSE if failed
---------------------------------------------------------------------------*/
boolean locClnt_InvokeCallback
(
   loc_client_handle_type        l_ClientHandle,
   locClnt_EventMaskType         t_EventType,
   const void *p_EventPayload
   )
{
  loc_client_info_s_type *pz_Client = NULL;
  loc_client_handle_type  l_TempClientHandle = LOC_CLIENT_HANDLE_INVALID;
  boolean                 v_RetVal = FALSE;
  time_type               z_TimeMs = { 0 };
  uint64                  t_TimeDeltaMs = 0ULL;
  uint64                  t_CurrentTimeMs = 0ULL;

  LOC_MSG_MED("locClnt_InvokeCallback, active client = %d, t_EventType = 0X%08x, max clients = %d",
              l_ClientHandle, t_EventType, loc_middleware_data.z_locClnt_Data.l_MaxNumClients);

  if (NULL == p_EventPayload)
  {
    LOC_MSG_ERROR("locClnt_InvokeCallback, NULL pointer", 0, 0, 0);
    return v_RetVal;
  }

  time_get_uptime_ms(z_TimeMs);
  t_CurrentTimeMs = QW_CVT_Q2N(z_TimeMs);
  
  for (pz_Client = (loc_client_info_s_type *)q_check(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue));
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue), &pz_Client->z_Link))
  {
    /* go through the locClientQueue, find out the client which is registered for the t_EventType */
    if ((TRUE == pz_Client->v_InUse) &&
        (0 != (pz_Client->t_EventRegMask & t_EventType)))
    {
      l_TempClientHandle = pz_Client->client_handle;

      /* The event should be sent to the current client */
      /* The event should be broadcasted if l_ClientHandle == LOC_HANDLE_REGISTERED_CLIENTS */
      if ((l_TempClientHandle == l_ClientHandle) ||
          (l_ClientHandle == LOC_HANDLE_REGISTERED_CLIENTS))
      {
        boolean v_Report = TRUE;

        if (locClnt_IsAutotrackSessionEventType(t_EventType) &&
            CLIENT_STATE_INACTIVE == pz_Client->e_ClientState)
        {
          v_Report = FALSE;
        }
        else if (LOC_EVENT_SATELLITE_REPORT == t_EventType)
        {
          qmiLocEventGnssSvInfoIndMsgT_v02 *pz_GnssSvInfoIndMsg = (qmiLocEventGnssSvInfoIndMsgT_v02 *)p_EventPayload;
		 
          /* e_ConfigAltitudeAssumed check, if disabled, do not send SV_info which comes from position report */
          if ((eQMI_LOC_ALTITUDE_ASSUMED_IN_GNSS_SV_INFO_DISABLED_V02 == pz_Client->z_FixCriteria.e_ConfigAltitudeAssumed) && 
			  (pz_GnssSvInfoIndMsg->altitudeAssumed == TRUE))
          {
            v_Report = FALSE;
            LOC_MSG_LOW("locClnt_InvokeCallback, do not report SV_info for client %d",
                        l_TempClientHandle, 0, 0);
          }
  
          else /*TBM is mapped to TBF,Report SvInfo at TBM/TBF interval , */
	      {
            if (t_CurrentTimeMs > pz_Client->t_LastSvInfoReportTime)
            {
              t_TimeDeltaMs = t_CurrentTimeMs - pz_Client->t_LastSvInfoReportTime;
            }
            else
            {
             /* In case of time service reset or error case, report this Svinfo */
             t_TimeDeltaMs = (uint64)(pz_Client->z_FixCriteria.q_TbfMs);
            }

            LOC_MSG_HIGH("locClnt_InvokeCallback, SvInfo last_report_time=%u, current_time=%u, interval=%u",
                        pz_Client->t_LastSvInfoReportTime, t_CurrentTimeMs,
                        pz_Client->z_FixCriteria.q_TbfMs);
		  
            if (t_TimeDeltaMs < (uint64)(pz_Client->z_FixCriteria.q_TbfMs - LOC_CLIENT_MEAS_OFFSET))
            {
              LOC_MSG_MED("locClnt_InvokeCallback, drop SvInfo for client %d",
                          l_TempClientHandle, 0, 0);
              continue;
            }
            else
            {
              pz_Client->t_LastSvInfoReportTime = t_CurrentTimeMs;
              LOC_MSG_HIGH("locClnt_InvokeCallback, set pz_Client->t_LastSvInfoReportTime %u for client %d",
                          pz_Client->t_LastSvInfoReportTime, l_TempClientHandle, 0);
              v_Report = TRUE;
            }
          }
        }
		else if (LOC_EVENT_GNSS_MEASUREMENT_REPORT == t_EventType) /* report MeasInfoInd at TBF interval */
		{
		  qmiLocEventGnssSvMeasInfoIndMsgT_v02  *pz_GnssSvMeasIndMsg = (qmiLocEventGnssSvMeasInfoIndMsgT_v02 *)p_EventPayload;

		  if (t_CurrentTimeMs > pz_Client->t_LastMeasInfoReportTime)
		  {
             t_TimeDeltaMs = t_CurrentTimeMs - pz_Client->t_LastMeasInfoReportTime;
          }
		  else
		  {
             /* In case of time service reset or error case, report this Meas */
             t_TimeDeltaMs = (uint64)(pz_Client->z_FixCriteria.q_TbfMs);
          }
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG

		  LOC_MSG_HIGH_5("locClnt_InvokeCallback,Seq[%u]/[%u], MeasInfo last_report_time=%u, current_time=%u, interval=%u",
		                pz_GnssSvMeasIndMsg->seqNum, pz_GnssSvMeasIndMsg->maxMessageNum,
                        pz_Client->t_LastMeasInfoReportTime, t_CurrentTimeMs,
                        pz_Client->z_FixCriteria.q_TbfMs);
#endif
		  if (t_TimeDeltaMs < (uint64)(pz_Client->z_FixCriteria.q_TbfMs - LOC_CLIENT_MEAS_OFFSET))
          {
              LOC_MSG_MED("locClnt_InvokeCallback, drop Measurements for client %d",
                        l_TempClientHandle, 0, 0);
              continue;
          }
          else
          {
              v_Report = TRUE;
			  
			  /* update the time when last seq of meas report is received */
			  if(pz_GnssSvMeasIndMsg->seqNum == pz_GnssSvMeasIndMsg->maxMessageNum )
              {
                pz_Client->t_LastMeasInfoReportTime = t_CurrentTimeMs;
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
                LOC_MSG_HIGH("locClnt_InvokeCallback, set pz_Client->t_MeasInfoReportTime %u for client %d",
                          pz_Client->t_LastMeasInfoReportTime, l_TempClientHandle, 0);
#endif
              }
          }

		}

        if (TRUE == v_Report)
        {
          LOC_MSG_MED("locClnt_InvokeCallback event_type 0X%08x for client = %d",
                      t_EventType, l_TempClientHandle, 0);
          v_RetVal = pz_Client->p_EventCb(l_TempClientHandle,
                                          t_EventType,
                                          p_EventPayload);
        }
      }
    }
  }

  LOC_MSG_MED("locClnt_InvokeCallback done", 0, 0, 0);

  return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   This function is used to invoke the callback for the
   LOC_EVENT_PARSED_POSITION_REPORT event
   1. E911 session, POSITION will be reported to all active clients
   2. Report postion to passive listener
   3. Normal session, intermediate POSITION
      Report to all active clients and update intermediate report timestamp
   4. Normal session,
      3.1 Always report to the scheduled client
      3.2 If the scheduled client willing to share, report to interval expired clients.
      Update the last fix report timestamp for the clients whom loc_client sent the final FIX

@param[in]  l_ClientHandle    client handle
            t_EventType       client interested event type
            pz_EventPayload   event callback data payload

@return TRUE if success
        FALSE if failed
---------------------------------------------------------------------------*/
boolean locClnt_PositionCallback
(
   loc_client_handle_type                 l_ClientHandle,
   locClnt_EventMaskType                  t_EventType,
   const void *p_EventPayload
   )
{
  loc_client_info_s_type *pz_Client = NULL;
  loc_client_info_s_type *pz_ActiveClient = NULL;
  loc_client_handle_type  l_TempClientHandle = LOC_CLIENT_HANDLE_INVALID;
  boolean                 v_Report = FALSE;
  boolean                 v_FinalFix = FALSE;
  uint64                  t_CurrentTimeMs = 0ULL;
  time_type               z_TimeMs = { 0 };
  boolean                 v_E911Session = loc_middleware_data.v_E911Session;
  qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg = NULL;

  LOC_MSG_MED("locClnt_PositionCallback, active client = %d, event_type = 0X%08x, max clients = %d",
              l_ClientHandle, t_EventType, loc_middleware_data.z_locClnt_Data.l_MaxNumClients);

  if (NULL == p_EventPayload)
  {
    LOC_MSG_ERROR("locClnt_PositionCallback, NULL pointer", 0, 0, 0);
    return FALSE;
  }
  pz_PositionReportIndMsg = (qmiLocEventPositionReportIndMsgT_v02 *)p_EventPayload;

  if (l_ClientHandle < 0)
  {
    LOC_MSG_ERROR("locClnt_PositionCallback, Invalid ClientHandle %d", l_ClientHandle, 0, 0);
    return FALSE;
  }
  pz_ActiveClient = locClnt_FindHandle(l_ClientHandle);
  if (NULL == pz_ActiveClient)
  {
    LOC_MSG_ERROR("locClnt_PositionCallback, can't find active client %d", l_ClientHandle, 0, 0);
    return FALSE;
  }

  if (FALSE == v_E911Session)
  {
    time_get_uptime_ms(z_TimeMs);
    t_CurrentTimeMs = QW_CVT_Q2N(z_TimeMs);
  }

  v_FinalFix = (eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02 ==  pz_PositionReportIndMsg->sessionStatus)? FALSE: TRUE;
	    
  for (pz_Client = (loc_client_info_s_type *)q_check(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue));
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue), &pz_Client->z_Link))
  {
    v_Report = FALSE;
    if ((TRUE == pz_Client->v_InUse) &&
        (0 != (pz_Client->t_EventRegMask & t_EventType)))
    {
      l_TempClientHandle = pz_Client->client_handle;

      if (TRUE == v_E911Session &&
          CLIENT_STATE_ACTIVE == pz_Client->e_ClientState)
      {
        /* 1. E911 session, POSITION will be reported to all active clients */
        v_Report = TRUE;
      }
      else if ((LOC_EVENT_PARSED_POSITION_REPORT == t_EventType) &&
               (TRUE == pz_Client->v_PositionListener) &&
               (TRUE == pz_ActiveClient->z_FixCriteria.v_SharePosition))
      {
         /*Check if fix is reportable to active client.
         Report for position listener only if fix is reportable for the active client.*/
        v_Report = LocClnt_IsFixReportable(pz_ActiveClient,l_ClientHandle,t_CurrentTimeMs,v_FinalFix);

	    if (TRUE == v_Report)
        {
          pz_Client->t_LastFixReportTime = t_CurrentTimeMs;
	      LOC_MSG_LOW("locClnt_PositionCallback,set pz_Client->t_LastFixReportTime %u for position listener client %d",
                   pz_Client->t_LastFixReportTime,l_TempClientHandle, 0);
        }
        else
        {
          LOC_MSG_LOW("locClnt_PositionCallback,active client fix not scheduled, dropping fix for position listener client:%d",
                   pz_Client->t_LastFixReportTime,l_TempClientHandle, 0);
          continue;
        }
      }
      else if ((LOC_EVENT_PARSED_POSITION_REPORT == t_EventType) &&
               (eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02 ==  pz_PositionReportIndMsg->sessionStatus) &&
               (CLIENT_STATE_INACTIVE != pz_Client->e_ClientState))
      {
        /* 3. Intermediate position to be reported if the client satifies the minIntermediateInterval*/
        v_Report =LocClnt_IsFixReportable(pz_Client,l_ClientHandle,t_CurrentTimeMs,v_FinalFix);

		if (TRUE == v_Report)
        {
          pz_Client->t_LastIntermediateFixReportTime = t_CurrentTimeMs;
          LOC_MSG_LOW("locClnt_PositionCallback, set pz_Client->t_LastIntermediateFixReportTime %u for client %d",
                        pz_Client->t_LastIntermediateFixReportTime, l_TempClientHandle, 0);
        }
      } /* end of intermediate position report */
      else
      {
        /* 4 Final position  */
         if ((l_TempClientHandle == l_ClientHandle) && (CLIENT_STATE_INACTIVE != pz_Client->e_ClientState))
        {
          /* Do not check time eclapsed for the scheduled client */
          LOC_MSG_LOW("locClnt_PositionCallback, always report position for the scheduled client", 0, 0, 0);
          v_Report = TRUE;
        }
        else if (TRUE == pz_ActiveClient->z_FixCriteria.v_SharePosition &&
                 CLIENT_STATE_INACTIVE != pz_Client->e_ClientState &&
                 (LOC_EVENT_PARSED_POSITION_REPORT == t_EventType || LOC_EVENT_NMEA_POSITION_REPORT == t_EventType))
        {
          v_Report = LocClnt_IsFixReportable(pz_Client,l_ClientHandle,t_CurrentTimeMs,v_FinalFix);
        }
        if ((TRUE == v_Report) &&
            (LOC_EVENT_PARSED_POSITION_REPORT == t_EventType))
        {
          /* update last position report time */
          pz_Client->t_LastFixReportTime = t_CurrentTimeMs;
          LOC_MSG_LOW("locClnt_PositionCallback, set pz_Client->t_LastFixReportTime %u:%u for client %d",
                      (uint32)(pz_Client->t_LastFixReportTime >> 32),
                      (uint32)(pz_Client->t_LastFixReportTime & CGPS_MAX_UINT32),
                      l_TempClientHandle);
        }
      } /* end of final position report */
      if (v_Report)
      {
        LOC_MSG_MED("locClnt_PositionCallback event_type 0X%08x for client_handle = %d",
                    t_EventType, l_TempClientHandle, 0);
        pz_Client->p_EventCb(l_TempClientHandle,
                             t_EventType,
                             p_EventPayload);
      }
    } /* pz_Client->v_InUse */
  } /* for loop */

  LOC_MSG_MED("locClnt_PositionCallback done", 0, 0, 0);
  return TRUE;
}

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC request from client that is Client-specific.

@param[in] l_ClientHandle,  : Loc API client handle
@param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
@param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
@param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length
@param[in/out] pe_QmiErr   : QMI_LOC Response error value

@return    TRUE   Called successfully
@return    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locClnt_ProcessQmiRequest
(
   loc_client_handle_type l_ClientHandle,
   uint32                 q_QmiLocMsgId,
   const void *p_QmiLocMsgData,
   uint32                 q_QmiLocMsgLen,
   qmi_error_type_v01 *pe_QmiErr
   )
{
  loc_client_info_s_type *pz_ClientInfo = NULL;
  uint32                  q_RetVal     = LOC_API_GENERAL_FAILURE;

  if (NULL == pe_QmiErr)
  {
    LOC_MSG_ERROR("locClnt_ProcessQmiRequest: NULL pe_QmiErr", 0, 0, 0);
    return FALSE;
  }

  /* Function arguments sanity check */
  if (LOC_CLIENT_HANDLE_INVALID == l_ClientHandle)
  {
    LOC_MSG_ERROR("locClnt_ProcessQmiRequest: invalid clientHandle", 0, 0, 0);
    *pe_QmiErr = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }

  pz_ClientInfo = locClnt_FindHandle(l_ClientHandle);

  if (NULL == pz_ClientInfo)
  {
    LOC_MSG_ERROR("locClnt_ProcessQmiRequest: Invalid client handle=%d", l_ClientHandle, 0, 0);
    *pe_QmiErr = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }

  /* Check for master client and cmd restriction */
  if (locClnt_IsCommandAllowed(l_ClientHandle, q_QmiLocMsgId) == FALSE)
  {
    LOC_MW_CLIENT_MSG_HIGH(pz_ClientInfo, "locClnt_ProcessQmiRequest: client %d msg %lu not allowed", l_ClientHandle, q_QmiLocMsgId, 0);
    *pe_QmiErr = QMI_ERR_NO_PERMISSION_V01;
    return FALSE;
  }

  switch (q_QmiLocMsgId)
  {
    case QMI_LOC_REG_EVENTS_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locClnt_SetEventRegMaskLocked(pz_ClientInfo, (qmiLocRegEventsReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locClnt_ProcessQmiRequest QMI_LOC_REG_EVENTS_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_START_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locClnt_SetFixCriteria(pz_ClientInfo, (qmiLocStartReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locClnt_ProcessQmiRequest QMI_LOC_START_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_GET_FIX_CRITERIA_REQ_V02:
      {
        q_RetVal = locClnt_GetFixCriteria(pz_ClientInfo);
        break;
      }

    case QMI_LOC_SET_SERVER_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locClnt_SetCustomServerAddr(pz_ClientInfo, (qmiLocSetServerReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locClnt_ProcessQmiRequest QMI_LOC_SET_SERVER_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_GET_SERVER_REQ_V02:
      {
        q_RetVal = locClnt_GetCustomPdeServerAddr(pz_ClientInfo);
        break;
      }

    case QMI_LOC_SET_OPERATION_MODE_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locClnt_SetOperationMode(pz_ClientInfo, (qmiLocSetOperationModeReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locClnt_ProcessQmiRequest QMI_LOC_SET_OPERATION_MODE_REQ NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    case QMI_LOC_GET_OPERATION_MODE_REQ_V02:
      {
        q_RetVal = locClnt_GetOperationMode(pz_ClientInfo);
        break;
      }

    case QMI_LOC_REGISTER_MASTER_CLIENT_REQ_V02:
      {
        if (NULL != p_QmiLocMsgData)
        {
          q_RetVal = locClnt_SetMasterClient(pz_ClientInfo, (qmiLocRegisterMasterClientReqMsgT_v02 *)p_QmiLocMsgData);
        }
        else
        {
          LOC_MSG_ERROR("locClnt_ProcessQmiRequest QMI_LOC_REGISTER_MASTER_CLIENT NULL p_QmiLocMsgData", 0, 0, 0);
        }
        break;
      }

    default:
      q_RetVal = LOC_API_UNSUPPORTED;
      break;
  }

  if (LOC_API_SUCCESS == q_RetVal)
  {
    return TRUE;
  }
  else
  {
    LOC_MW_CLIENT_MSG_ERROR(pz_ClientInfo, "locClnt_ProcessQmiRequest: QMI_LOC 0X%04x returns error=%d", q_QmiLocMsgId, q_RetVal, 0);
    *pe_QmiErr = locMW_ConvertToQmiErr(q_RetVal);
    return FALSE;
  }
}

/*---------------------------------------------------------------------------
@brief
   Function to process the event registration mask set request for this location client
   Note: this function should be called when the client data stucture is locked from concurrent access

@param[in] pz_ClientInfo:   Loc client pointer
@param[in] pz_RegEventsReq: QMI_LOC Request Message

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_SetEventRegMaskLocked
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocRegEventsReqMsgT_v02 *pz_RegEventsReq
   )
{
  /* Check if any client has ever registered for event LOC_EVENT_LOCATION_SERVER_REQUEST */
  locClnt_EventMaskType t_EventMaskOld       = 0ULL;
  locClnt_EventMaskType t_EventMaskNew       = 0ULL;
  uint8                 u_I                  = 0;
  uint32                q_RetVal             = LOC_API_SUCCESS;

  /* Function arguments sanity check */
  if ((NULL == pz_ClientInfo) || (NULL == pz_RegEventsReq))
  {
    LOC_MSG_ERROR("locClnt_SetEventRegMaskLocked: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  t_EventMaskOld = pz_ClientInfo->t_EventRegMask;

  /* Convert Registered Events mask from QMI_LOC to LocClnt */
  for (u_I = 0; u_I < (sizeof(locClnt_EventRegMaskMap) / sizeof(locClnt_EventRegMaskMap[0])); u_I++)
  {
    if (locClnt_EventRegMaskMap[u_I].t_QmiEventRegMask & pz_RegEventsReq->eventRegMask)
    {
      t_EventMaskNew |= locClnt_EventRegMaskMap[u_I].t_LocEventRegMask;
    }
  }

  LOC_MW_CLIENT_MSG_LOW(pz_ClientInfo, "locClnt_SetEventRegMaskLocked: t_EventMaskOld = 0x%04x, t_EventMaskNew = 0x%04x",
                        t_EventMaskOld, t_EventMaskNew, 0);
  /* If LOC_EVENT_LOCATION_SERVER_REQUEST event registration changes, handle differently */
  if ((t_EventMaskOld ^ t_EventMaskNew) & LOC_EVENT_LOCATION_SERVER_REQUEST)
  {
    if (t_EventMaskNew & LOC_EVENT_LOCATION_SERVER_REQUEST)
    {
      /* Client trying to register for LOC_EVENT_LOCATION_SERVER_REQUEST.
         * If the connection client has previously registered, ignore the request */
      if (FALSE == locClnt_IsServerRequestHandlerRegistered())
      {
        if (FALSE == locConn_Init())
        {
          LOC_MSG_ERROR("Reg pdsm atl proxy failed!", 0, 0, 0);
          q_RetVal = LOC_API_GENERAL_FAILURE;
        }
        else
        {
          LOC_MSG_MED("Reg pdsm atl proxy succeed!", 0, 0, 0);
        }
      }
      else
      {
        LOC_MSG_MED("pdsm atl proxy already registered!", 0, 0, 0);
      }
    }
    else
    {
      /* Client trying to deregister for LOC_EVENT_LOCATION_SERVER_REQUEST. */
      if (TRUE == locClnt_IsServerRequestHandlerRegistered())
      {
        if (FALSE == locConn_DeInit())
        {
          LOC_MSG_ERROR("Dereg pdsm atl proxy failed!", 0, 0, 0);
          q_RetVal = LOC_API_GENERAL_FAILURE;
        }
        else
        {
          LOC_MSG_MED("Dereg pdsm atl proxy succeed!", 0, 0, 0);
        }
      }
      else
      {
        LOC_MSG_MED("pdsm atl proxy already deregistered!", 0, 0, 0);
      }
    }
  }
  pz_ClientInfo->t_EventRegMask = t_EventMaskNew;

  /* Check if we need to enable the batch breach notification in GM */
  if (pz_RegEventsReq->eventRegMask & QMI_LOC_EVENT_MASK_GEOFENCE_BATCH_BREACH_NOTIFICATION_V02)
  {
    locGeofenceHandleBatchBreachReq(TRUE);
  }

  /* Copy the optional string */
  if (pz_RegEventsReq->clientStrId_valid)
  {
    MSG_SPRINTF_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, "clientStrId = %s", pz_RegEventsReq->clientStrId);
    GNSS_STRLCPY(pz_ClientInfo->pb_ClientIdStr,
                 pz_RegEventsReq->clientStrId,
                 sizeof(pz_ClientInfo->pb_ClientIdStr));

    /* Null terminate just in case */
    pz_ClientInfo->pb_ClientIdStr[LOC_CLIENT_ID_STRING_LENGTH] = '\0';
    pz_ClientInfo->v_ClientSetIdStr = TRUE;
  }

  /* default values are set in locClnt_Init */
  if (TRUE == pz_RegEventsReq->clientType_valid)
  {
    if (eQMI_LOC_CLIENT_AFW_V02 != pz_RegEventsReq->clientType)
    {
      if (pz_ClientInfo->client_handle ==\
             loc_middleware_data.z_locClnt_Data.l_MasterClientHandle)
      {
        LOC_MSG_ERROR("Master client can't be set as NFW client", 0, 0, 0);
        pz_ClientInfo->e_ClientType = eQMI_LOC_CLIENT_AFW_V02;
        q_RetVal = LOC_API_INVALID_PARAMETER;
      }
      else if (FALSE == pz_RegEventsReq->enablePosRequestNotification_valid)
      {
        LOC_MSG_ERROR("enablePosRequestNotification not set for client", 0, 0, 0);
        q_RetVal = LOC_API_INVALID_PARAMETER;
      }
      else
      {
        pz_ClientInfo->v_EnablePosRequestNotification =\
           pz_RegEventsReq->enablePosRequestNotification;
      }
    }
    pz_ClientInfo->e_ClientType = pz_RegEventsReq->clientType;
  }

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Function to process the operation mode set request for this location client.

@param[in] pz_ClientInfo:   Loc client pointer
@param[in] p_SetOpModeReq:  QMI_LOC Request Message

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_SetOperationMode
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocSetOperationModeReqMsgT_v02 *p_SetOpModeReq
   )
{
  uint32             q_RetVal    = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if ((NULL == pz_ClientInfo) || (NULL == p_SetOpModeReq))
  {
    LOC_MSG_ERROR("locClnt_SetOperationMode: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    switch (p_SetOpModeReq->operationMode)
    {
      /* Range check the operation mode enum */
      case eQMI_LOC_OPER_MODE_DEFAULT_V02:
      case eQMI_LOC_OPER_MODE_MSB_V02:
      case eQMI_LOC_OPER_MODE_MSA_V02:
      case eQMI_LOC_OPER_MODE_STANDALONE_V02:
      case eQMI_LOC_OPER_MODE_CELL_ID_V02:
      case eQMI_LOC_OPER_MODE_WWAN_V02:
        {
          pz_ClientInfo->z_FixCriteria.e_PreferredOperationMode = p_SetOpModeReq->operationMode;
          pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_PREFERRED_OPERATION_MODE;
          q_RetVal = LOC_API_SUCCESS;
          LOC_MSG_HIGH("locClnt_SetOperationMode: %d", p_SetOpModeReq->operationMode, 0, 0);
          break;
        }
      default:
        {
          LOC_MSG_ERROR("locClnt_SetOperationMode: Invalid Option=%d", p_SetOpModeReq->operationMode, 0, 0);
          break;
        }
    }

    if (LOC_API_SUCCESS == q_RetVal)
    {
      /* Send the operation mode to GF */
      locGeofenceSetOperationMode(p_SetOpModeReq->operationMode);

      if (p_SetOpModeReq->minInterval_valid)
      {
        pz_ClientInfo->z_FixCriteria.q_TbfMs =  locClnt_MatchFixInterval(p_SetOpModeReq->minInterval);
      }

      if (pz_ClientInfo->z_FixCriteria.v_SharePosition &&
          LOC_CLIENT_FIX_PERIODIC == pz_ClientInfo->z_FixCriteria.e_RecurrenceType)
      {
        locClnt_SetCommonPosCriteria();
      }
    }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_SET_OPERATION_MODE_IND_V02,
          NULL, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_SetOperationMode: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
Function to get the accuracy when preferred accuracy is HIGH.
return q_PreferredAccuracy
---------------------------------------------------------------------------*/

static int locClnt_GetHighAccuracy(void)
{
  static uint32      q_PreferredAccuracy;

  q_PreferredAccuracy = GNSS_NV_EFS_LOC_ACCURACY_THRESHOLD_FOR_HIGH_ACCURACY_DEFAULT;

  if (TRUE == locNvEfsInit())
  {
    if (FALSE == locNvEfsRead(GNSS_NV_EFS_LOC_ACCURACY_THRESHOLD_FOR_HIGH_ACCURACY,
                              (void *)&q_PreferredAccuracy,
                              sizeof(q_PreferredAccuracy)))
    {
      LOC_MSG_HIGH("locClnt_GetHighAcuuracy could not read nv for q_PreferredAccuracy, defaulting it to %u",
                   q_PreferredAccuracy, 0, 0);
    }
  }
  else
  {
    LOC_MSG_HIGH("locClnt_GetHighAcuuracy locNvEfsInit failed, defaulting it to %u",
                 q_PreferredAccuracy, 0, 0);
  }

  if (q_PreferredAccuracy > LOC_CLIENT_FIX_ACCURACY_HIGH_MAX)
  {
    q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_HIGH_MAX;
  }
  else if (q_PreferredAccuracy < LOC_CLIENT_FIX_ACCURACY_HIGH)
  {
    q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_HIGH;
  }

  LOC_MSG_HIGH("locClnt_GetHighAcuuracy provided q_PreferredAccuracy %u",
               q_PreferredAccuracy, 0, 0);
  return q_PreferredAccuracy;
}

/*---------------------------------------------------------------------------
@brief
   Function to process the fix criteria set request for this location client

@param[in] pz_ClientInfo:   Loc client pointer
@param[in] pz_StartReq:     QMI_LOC Request Message

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_SetFixCriteria
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocStartReqMsgT_v02 *pz_StartReq
   )
{
  loc_client_info_s_type *pz_Client = NULL;
  uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if ((NULL == pz_ClientInfo) || (NULL == pz_StartReq))
  {
    LOC_MSG_ERROR("locClnt_SetFixCriteria: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   if ( ( FALSE == locClnt_IsPosRequestAllowed(pz_ClientInfo) ) &&
        ( FALSE == locClnt_IsSpecialPosRequest(pz_StartReq->specialReqType) ) )
  {
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_START_REQ_V02,
          NULL, LOC_API_GNSS_DISABLED))
      LOC_MSG_ERROR("locClnt_SetFixCriteria: GPS Locked for this client", 0, 0, 0);
    return LOC_API_GENERAL_FAILURE;
  }

  do
  {
    pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_RECURRENCE_TYPE;
    pz_ClientInfo->z_FixCriteria.e_RecurrenceType = LOC_CLIENT_FIX_SINGLE;
    if (pz_StartReq->fixRecurrence_valid)
    {
      switch (pz_StartReq->fixRecurrence)
      {
        case eQMI_LOC_RECURRENCE_PERIODIC_V02:
          {
            pz_ClientInfo->z_FixCriteria.e_RecurrenceType = LOC_CLIENT_FIX_PERIODIC;
            q_RetVal = LOC_API_SUCCESS;
            break;
          }
        case eQMI_LOC_RECURRENCE_SINGLE_V02:
          {
            pz_ClientInfo->z_FixCriteria.e_RecurrenceType = LOC_CLIENT_FIX_SINGLE;
            q_RetVal = LOC_API_SUCCESS;
            break;
          }
        default:
          {
            LOC_MSG_ERROR("locClnt_SetFixCriteria: Invalid RecurrenceType=%d ", pz_StartReq->fixRecurrence, 0, 0);
            q_RetVal = LOC_API_INVALID_PARAMETER;
            break;
          }
      }
    }
    else
    {
      LOC_MSG_MED("locClnt_SetFixCriteria: RecurrenceType not set, use default single", 0, 0, 0);
      q_RetVal = LOC_API_SUCCESS;
    }

    pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_PREFERRED_ACCURACY;
    pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_LOW;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->horizontalAccuracyLevel_valid)
    {
      switch (pz_StartReq->horizontalAccuracyLevel)
      {
        case eQMI_LOC_ACCURACY_LOW_V02:
          {
            pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_LOW;
            break;
          }
        case eQMI_LOC_ACCURACY_MED_V02:
          {
            pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_MED;
            break;
          }
        case eQMI_LOC_ACCURACY_HIGH_V02:
          {
            pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy = LOC_CLIENT_FIX_ACCURACY_HIGH;
            break;
          }
        default:
          {
            LOC_MSG_ERROR("locClnt_SetFixCriteria: Invalid horizontalAccuracyLevel=%d ", pz_StartReq->horizontalAccuracyLevel, 0, 0);
            q_RetVal = LOC_API_INVALID_PARAMETER;
            break;
          }
      }

      if (pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy == LOC_CLIENT_FIX_ACCURACY_HIGH)
      {
        pz_ClientInfo->z_FixCriteria.q_PreferredAccuracy = locClnt_GetHighAccuracy();
      }
    }

    /* There are other fields in the fix criteria, but are not exposed through QMI and hence not filled
       (assuming the defaults will be used by LocMW). */

    /* The control point must explicitly disable this if it does not wish to receive intermediate position reports */
    pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_INTERMEDIATE_POS_REPORT_ENABLED;
    pz_ClientInfo->z_FixCriteria.v_IsIntermPosRptOn = TRUE;

    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->intermediateReportState_valid &&
        eQMI_LOC_INTERMEDIATE_REPORTS_OFF_V02 == pz_StartReq->intermediateReportState)
    {
      pz_ClientInfo->z_FixCriteria.v_IsIntermPosRptOn = FALSE;
    }

    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->minInterval_valid &&
        eQMI_LOC_RECURRENCE_PERIODIC_V02 == pz_StartReq->fixRecurrence)
    {
      pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_MIN_INTERVAL;
      pz_ClientInfo->z_FixCriteria.q_TbfMs = locClnt_MatchFixInterval(pz_StartReq->minInterval);
    }

    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->applicationId_valid)
    {
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
      for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
           pz_Client != NULL;
           pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
      {
        if (TRUE == pz_Client->v_InUse &&
            CLIENT_STATE_ACTIVE == pz_Client->e_ClientState &&
            TRUE == pz_Client->z_FixCriteria.v_SharePosition)
        {
          LOC_MSG_ERROR("locClnt_SetFixCriteria: application_ID is already set, reject", 0, 0, 0);
          q_RetVal = LOC_API_INVALID_PARAMETER;
          break;
        }
      }
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
      if (LOC_API_SUCCESS == q_RetVal)
      {
        pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_APPLICATION_ID;
        pz_ClientInfo->z_FixCriteria.z_ApplicationId = pz_StartReq->applicationId;
      }
    }

    /* set default for e_ConfigAltitudeAssumed */
    pz_ClientInfo->z_FixCriteria.e_ConfigAltitudeAssumed = eQMI_LOC_ALTITUDE_ASSUMED_IN_GNSS_SV_INFO_ENABLED_V02;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->configAltitudeAssumed_valid)
    {
      pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_ALTITUDE_ASSUMED;
      pz_ClientInfo->z_FixCriteria.e_ConfigAltitudeAssumed = pz_StartReq->configAltitudeAssumed;
    }

    /* Between intermediate position reports, elapsed time must be longer than the interval time */
    pz_ClientInfo->z_FixCriteria.q_MinIntervalIntermediate = 0;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->minIntermediatePositionReportInterval_valid)
    {
      pz_ClientInfo->z_FixCriteria.q_MinIntervalIntermediate = pz_StartReq->minIntermediatePositionReportInterval;
    }

    /* Always reset the session timeout to 255sec, in case the same client converts from batching to tracking client */
    pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_CRIT_VALID_PREFERRED_RESPONSE_TIME;
    pz_ClientInfo->z_FixCriteria.q_PreferredResponseTime = LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->positionReportTimeout_valid)
    {
      if (LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT > pz_StartReq->positionReportTimeout ||
          LOC_CLIENT_FIX_RESPONSE_TIME_MS_DEFAULT < pz_StartReq->positionReportTimeout)
      {
        LOC_MSG_ERROR("locClnt_SetFixCriteria invalid positionReportTimeout=%u",
                      pz_StartReq->positionReportTimeout, 0, 0);
        q_RetVal = LOC_API_INVALID_PARAMETER;
      }
      else
      {
        pz_ClientInfo->z_FixCriteria.q_PreferredResponseTime = pz_StartReq->positionReportTimeout;
      }
    }

    pz_ClientInfo->z_FixCriteria.v_SharePosition = TRUE;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->sharePosition_valid)
    {
      pz_ClientInfo->z_FixCriteria.v_SharePosition = pz_StartReq->sharePosition;
    }

    pz_ClientInfo->z_FixCriteria.z_PowerMode.powerMode = eQMI_LOC_POWER_MODE_NORMAL_V02;
    pz_ClientInfo->z_FixCriteria.z_PowerMode.timeBetweenMeasurement = 0;
    if (LOC_API_SUCCESS == q_RetVal &&
        pz_StartReq->powerMode_valid)
    {
      /* TBM parameter check for M4 */
      if (eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_TIME_V02 == pz_StartReq->powerMode.powerMode)
      {
        if (pz_StartReq->powerMode.timeBetweenMeasurement != pz_StartReq->minInterval)
        {
          LOC_MSG_ERROR("locClnt_SetFixCriteria TBM %u TBF %u doesn't match for POWER_MODE_BACKGROUND_DEFINED_TIME",
                        pz_StartReq->powerMode.timeBetweenMeasurement,
                        pz_StartReq->minInterval,
                        0);
          q_RetVal = LOC_API_INVALID_PARAMETER;
        }
      }

      /* TBM parameter check for M3 and M4 */
      if ((LOC_API_SUCCESS == q_RetVal) &&
          (eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_POWER_V02 == pz_StartReq->powerMode.powerMode ||
           eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_TIME_V02 == pz_StartReq->powerMode.powerMode))
      {
        if (pz_StartReq->powerMode.timeBetweenMeasurement < LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT)
        {
          LOC_MSG_ERROR("locClnt_SetFixCriteria TBM %u for PowerMode %d",
                        pz_StartReq->powerMode.timeBetweenMeasurement,
                        pz_StartReq->powerMode.powerMode,
                        0);
          q_RetVal = LOC_API_INVALID_PARAMETER;
        }
        else
        {
          /* Set time out for M3, M4 = TBM */
          pz_ClientInfo->z_FixCriteria.q_PreferredResponseTime = pz_StartReq->powerMode.timeBetweenMeasurement;
          pz_ClientInfo->z_FixCriteria.z_PowerMode.timeBetweenMeasurement = pz_StartReq->powerMode.timeBetweenMeasurement;
        }
      }

      if (LOC_API_SUCCESS == q_RetVal)
      {
        pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_POWER_MODE;
        pz_ClientInfo->z_FixCriteria.z_PowerMode.powerMode = pz_StartReq->powerMode.powerMode;
        /* M3 and M5 are power based, not time based, so set the TBF to 1s */
        if (eQMI_LOC_POWER_MODE_BACKGROUND_DEFINED_POWER_V02 == pz_StartReq->powerMode.powerMode ||
            eQMI_LOC_POWER_MODE_BACKGROUND_KEEP_WARM_V02 == pz_StartReq->powerMode.powerMode)
        {
          pz_ClientInfo->z_FixCriteria.q_TbfMs = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT;
        }
        /* M5 time out is 10 seconds */
        if (eQMI_LOC_POWER_MODE_BACKGROUND_KEEP_WARM_V02 == pz_StartReq->powerMode.powerMode)
        {
          pz_ClientInfo->z_FixCriteria.q_PreferredResponseTime = LOC_CLIENT_BACKGROUND_KEEP_WARM_TIME;
        }
      }
    }

      /*Special request will be acepted as reqular requests during 911. Do not set the SpecialRequestType 
        if LocMW is in 911*/
      pz_ClientInfo->z_FixCriteria.e_SpecialRequestType = QMILOCSPECIALREQENUMT_MIN_ENUM_VAL_V02;
      if ((LOC_API_SUCCESS == q_RetVal) && (pz_StartReq->specialReqType_valid)&&
          (FALSE == loc_middleware_data.v_E911Session)) 
      {
         pz_ClientInfo->z_FixCriteria.q_ValidMask |= LOC_FIX_SPECIAL_REQUEST;
         pz_ClientInfo->z_FixCriteria.e_SpecialRequestType = pz_StartReq->specialReqType;
         LOC_MSG_HIGH_1("locClnt_SetFixCriteria: Special Request Type: %d",pz_StartReq->specialReqType);
      }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_START_REQ_V02,
          NULL, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_SetFixCriteria: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  LOC_MW_CLIENT_MSG_MED(pz_ClientInfo, "locClnt_SetFixCriteria: ValidMask=0x%x", pz_ClientInfo->z_FixCriteria.q_ValidMask, 0, 0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Function to get the fix criteria for this location client

@param[in] pz_ClientInfo:   Loc client pointer

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_GetFixCriteria
(
   const loc_client_info_s_type *pz_ClientInfo
   )
{
  const locClnt_FixCriteriaType *pz_FixCriteria   = NULL;
  qmiLocGetFixCriteriaIndMsgT_v02 *pz_GetFixCritInd = NULL;
  uint32                           q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if (NULL == pz_ClientInfo)
  {
    LOC_MSG_ERROR("locClnt_GetFixCriteria: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    pz_GetFixCritInd = (qmiLocGetFixCriteriaIndMsgT_v02 *)loc_calloc(sizeof(*pz_GetFixCritInd));
    if (NULL == pz_GetFixCritInd)
    {
      LOC_MSG_ERROR("locClnt_GetFixCriteria: calloc return NULL", 0, 0, 0);
      break;
    }
    q_RetVal = LOC_API_SUCCESS;

    pz_FixCriteria   = &pz_ClientInfo->z_FixCriteria;

    /* Convert Fix Criteria structure from LocClnt to QMI_LOC */
    if (pz_FixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_PREFERRED_ACCURACY)
    {
      pz_GetFixCritInd->horizontalAccuracyLevel_valid = TRUE;
      switch (pz_FixCriteria->q_PreferredAccuracy)
      {
        case LOC_CLIENT_FIX_ACCURACY_HIGH:
          {
            pz_GetFixCritInd->horizontalAccuracyLevel = eQMI_LOC_ACCURACY_HIGH_V02;
            break;
          }
        case LOC_CLIENT_FIX_ACCURACY_MED:
          {
            pz_GetFixCritInd->horizontalAccuracyLevel = eQMI_LOC_ACCURACY_MED_V02;
            break;
          }
        case LOC_CLIENT_FIX_ACCURACY_LOW:
          {
            pz_GetFixCritInd->horizontalAccuracyLevel = eQMI_LOC_ACCURACY_LOW_V02;
            break;
          }
        default:
          {
            LOC_MSG_ERROR("locClnt_GetFixCriteria: Invalid Accuracy Threshold=%d", pz_FixCriteria->q_PreferredAccuracy, 0, 0);
            pz_GetFixCritInd->horizontalAccuracyLevel_valid = FALSE;
            break;
          }
      }
    }

    pz_GetFixCritInd->intermediateReportState_valid = TRUE;
    pz_GetFixCritInd->intermediateReportState = (pz_FixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_INTERMEDIATE_POS_REPORT_ENABLED) ?
       eQMI_LOC_INTERMEDIATE_REPORTS_ON_V02 :
       eQMI_LOC_INTERMEDIATE_REPORTS_OFF_V02;

    if (pz_FixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_MIN_INTERVAL)
    {
      pz_GetFixCritInd->minInterval_valid = TRUE;
      pz_GetFixCritInd->minInterval = pz_FixCriteria->q_TbfMs;
    }

    /* Copy Application Id */
    if (pz_FixCriteria->q_ValidMask & LOC_FIX_CRIT_VALID_APPLICATION_ID)
    {
      pz_GetFixCritInd->applicationId_valid = TRUE;
      pz_GetFixCritInd->applicationId = pz_FixCriteria->z_ApplicationId;
    }

    if (pz_FixCriteria->q_ValidMask & LOC_FIX_CRIT_ALTITUDE_ASSUMED)
    {
      pz_GetFixCritInd->configAltitudeAssumed_valid = TRUE;
      pz_GetFixCritInd->configAltitudeAssumed = pz_FixCriteria->e_ConfigAltitudeAssumed;
    }

    pz_GetFixCritInd->minIntermediatePositionReportInterval_valid = TRUE;
    pz_GetFixCritInd->minIntermediatePositionReportInterval = pz_FixCriteria->q_MinIntervalIntermediate;

    pz_GetFixCritInd->positionReportTimeout_valid = TRUE;
    pz_GetFixCritInd->positionReportTimeout = pz_FixCriteria->q_PreferredResponseTime;

    pz_GetFixCritInd->sharePosition_valid = TRUE;
    pz_GetFixCritInd->sharePosition = pz_FixCriteria->v_SharePosition;

    if (pz_FixCriteria->q_ValidMask & LOC_FIX_POWER_MODE)
    {
      pz_GetFixCritInd->powerMode_valid = TRUE;
      pz_GetFixCritInd->powerMode = pz_FixCriteria->z_PowerMode;
    }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_GET_FIX_CRITERIA_IND_V02,
          pz_GetFixCritInd, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_GetFixCriteria: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Report position to concurrent cell id request

@param[in]  pz_PositionPayload   position payload
            t_CurrentTimeMs      current time stamp will be updated

@return None
---------------------------------------------------------------------------*/
void locClnt_ReportPositionToCellidRequest
(
   const void *p_PositionPayload,
   uint64            t_CurrentTimeMs
   )
{
  loc_client_info_s_type *pz_Client = NULL;

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
  for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
  {
    if ((TRUE == pz_Client->v_InUse) &&
        (eQMI_LOC_OPER_MODE_CELL_ID_V02 == pz_Client->z_FixCriteria.e_PreferredOperationMode))
    {
      if (CLIENT_STATE_TRANSIENT == pz_Client->e_ClientState)
      {
        // Report position
        // Set state
        locClnt_InvokeCallback(pz_Client->client_handle,
                               LOC_EVENT_PARSED_POSITION_REPORT,
                               p_PositionPayload);
        pz_Client->e_ClientState = CLIENT_STATE_INACTIVE;

        LOC_MSG_MED("locClnt_ReportPositionToCellidRequest for client = %d",
                    pz_Client->client_handle, 0, 0);
      }
      else if ((CLIENT_STATE_ACTIVE == pz_Client->e_ClientState) &&
               (TRUE == pz_Client->v_FirstFix))
      {
        // Report position
        // Set state
        locClnt_InvokeCallback(pz_Client->client_handle,
                               LOC_EVENT_PARSED_POSITION_REPORT,
                               p_PositionPayload);

        pz_Client->v_FirstFix = FALSE;
        pz_Client->t_LastFixRequestTime = t_CurrentTimeMs;

        LOC_MSG_MED("locClnt_ReportPositionToCellidRequest for client = %d",
                    pz_Client->client_handle, 0, 0);
      }
    }
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);
}

/*---------------------------------------------------------------------------
@brief
   The other module use this helper function to query if any client is interested
   in LOC_EVENT_LOCATION_SERVER_REQUEST

@param[in]  None

@return TRUE if server request is registered by any client
        FALSE if not
---------------------------------------------------------------------------*/
boolean locClnt_IsServerRequestHandlerRegistered(void)
{
  loc_client_info_s_type *pz_Client = NULL;
  boolean found =         FALSE;

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  for (pz_Client = (loc_client_info_s_type *)q_check(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue);
       pz_Client != NULL;
       pz_Client = (loc_client_info_s_type *)q_next(&loc_middleware_data.z_locClnt_Data.z_LocClientQueue, &pz_Client->z_Link))
  {
    if ((pz_Client->v_InUse == TRUE) &&
        ((pz_Client->t_EventRegMask & LOC_EVENT_LOCATION_SERVER_REQUEST) != 0))
    {
      found = TRUE;
    }
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.client_crit_sec);

  LOC_MSG_HIGH("locClnt_IsServerRequestHandlerRegistered: %d", (int)found, 0, 0);

  return found;
}

/*---------------------------------------------------------------------------
@brief
   Function to process the custom server address set request for this location client.
   Custom server will be used when doing MSA/MSB fixes.

@param[in] pz_ClientInfo:   Loc client pointer
@param[in] pz_StartReq:     QMI_LOC Request Message

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_SetCustomServerAddr
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocSetServerReqMsgT_v02 *pz_SetServerReq
   )
{
  uint32              q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if ((NULL == pz_ClientInfo) || (NULL == pz_SetServerReq))
  {
    LOC_MSG_ERROR("locClnt_SetCustomServerAddr: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    if ((eQMI_LOC_SERVER_TYPE_CUSTOM_PDE_V02 != pz_SetServerReq->serverType) &&
        (eQMI_LOC_SERVER_TYPE_CUSTOM_SLP_V02 != pz_SetServerReq->serverType))
    {
      LOC_MSG_ERROR("locClnt_SetCustomServerAddr: Invalid Server Type=%d",
                    pz_SetServerReq->serverType, 0, 0);
      q_RetVal = LOC_API_INVALID_PARAMETER;
      break;
    }

    if (eQMI_LOC_SERVER_TYPE_CUSTOM_PDE_V02 == pz_SetServerReq->serverType)
    {
      pz_ClientInfo->z_ServerInfo.server_option = PDSM_SERVER_OPTION_USE_LOCAL;
      memset(&(pz_ClientInfo->z_ServerInfo.server_adrs), 0,
             sizeof(pz_ClientInfo->z_ServerInfo.server_adrs));
      /* Convert custom PDE Server Address from QMI LOC to PDAPI */
      q_RetVal = locMW_ConvertAddrToPdapi(pz_SetServerReq, &(pz_ClientInfo->z_ServerInfo.server_adrs));
      if (LOC_API_SUCCESS != q_RetVal)
      {
        LOC_MSG_ERROR("locClnt_SetCustomServerAddr: Addr convert failed", 0, 0, 0);
        break;
      }
    }

    if (eQMI_LOC_SERVER_TYPE_CUSTOM_SLP_V02 == pz_SetServerReq->serverType)
    {
      pz_ClientInfo->z_SLPServerInfo.server_option = PDSM_SERVER_OPTION_USE_LOCAL;
      memset(&(pz_ClientInfo->z_SLPServerInfo.server_adrs), 0,
             sizeof(pz_ClientInfo->z_SLPServerInfo.server_adrs));
      /* Convert custom SLP Server Address from QMI LOC to PDAPI */
      q_RetVal = locMW_ConvertAddrToPdapi(pz_SetServerReq, &(pz_ClientInfo->z_SLPServerInfo.server_adrs));
      if (LOC_API_SUCCESS != q_RetVal)
      {
        LOC_MSG_ERROR("locClnt_SetCustomSLPServerAddr: Addr convert failed", 0, 0, 0);
        break;
      }
    }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_SET_SERVER_IND_V02,
          NULL, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_SetCustomServerAddr: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Function to get the custom server address  for this location client.

@param[in] pz_ClientInfo:   Loc client pointer

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_GetCustomPdeServerAddr
(
   const loc_client_info_s_type *pz_ClientInfo
   )
{
  qmiLocGetServerIndMsgT_v02 *pz_ServerAddr = NULL;
  uint32                        q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if (NULL == pz_ClientInfo)
  {
    LOC_MSG_ERROR("locClnt_GetCustomPdeServerAddr: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    pz_ServerAddr = (qmiLocGetServerIndMsgT_v02 *)loc_calloc(sizeof(*pz_ServerAddr));
    if (NULL == pz_ServerAddr)
    {
      LOC_MSG_ERROR("locClnt_GetCustomPdeServerAddr: calloc return NULL", 0, 0, 0);
      break;
    }
    q_RetVal = LOC_API_SUCCESS;
    pz_ServerAddr->serverType = eQMI_LOC_SERVER_TYPE_CUSTOM_PDE_V02;

    if (PDSM_SERVER_TYPE_IPV4_ADDRESS == pz_ClientInfo->z_ServerInfo.server_adrs.server_addr_type)
    {
      pz_ServerAddr->ipv4Addr_valid = TRUE;
      pz_ServerAddr->ipv4Addr.addr = pz_ClientInfo->z_ServerInfo.server_adrs.server_adrs.server_adrs_v4.server_adrs;
      pz_ServerAddr->ipv4Addr.port = pz_ClientInfo->z_ServerInfo.server_adrs.server_adrs.server_adrs_v4.port_id;
      LOC_MSG_MED("locClnt_GetCustomPdeServerAddr: IPV4 Address", 0, 0, 0);
    }
    else if (PDSM_SERVER_TYPE_URL_ADDRESS == pz_ClientInfo->z_ServerInfo.server_adrs.server_addr_type)
    {
      pz_ServerAddr->urlAddr_valid = TRUE;
      memscpy(pz_ServerAddr->urlAddr,
              QMI_LOC_MAX_SERVER_ADDR_LENGTH_V02,
              pz_ClientInfo->z_ServerInfo.server_adrs.server_adrs.server_adrs_url.url,
              pz_ClientInfo->z_ServerInfo.server_adrs.server_adrs.server_adrs_url.url_length);
      LOC_MSG_MED("locClnt_GetCustomPdeServerAddr: URL Address", 0, 0, 0);
    }
    else
    {
      //IPV6 is not supported yet
      LOC_MSG_ERROR("locClnt_GetCustomPdeServerAddr: not support Address type %d",
                    pz_ClientInfo->z_ServerInfo.server_adrs.server_addr_type, 0, 0);
    }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_GET_SERVER_IND_V02,
          pz_ServerAddr, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_GetCustomPdeServerAddr: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Function to get the operation mode for this location client.

@param[in] pz_ClientInfo:   Loc client pointer

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_GetOperationMode
(
   const loc_client_info_s_type *pz_ClientInfo
   )
{
  qmiLocGetOperationModeIndMsgT_v02 *pz_GetOperationModeInd = NULL;
  uint32                              q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if (NULL == pz_ClientInfo)
  {
    LOC_MSG_ERROR("locClnt_GetOperationMode: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    pz_GetOperationModeInd = (qmiLocGetOperationModeIndMsgT_v02 *)loc_calloc(sizeof(*pz_GetOperationModeInd));
    if (NULL == pz_GetOperationModeInd)
    {
      LOC_MSG_ERROR("locClnt_GetOperationMode: calloc return NULL", 0, 0, 0);
      break;
    }
    q_RetVal = LOC_API_SUCCESS;
    pz_GetOperationModeInd->operationMode_valid = TRUE;

    switch (pz_ClientInfo->z_FixCriteria.e_PreferredOperationMode)
    {
      /* Range check the operation mode */
      case eQMI_LOC_OPER_MODE_DEFAULT_V02:
      case eQMI_LOC_OPER_MODE_MSB_V02:
      case eQMI_LOC_OPER_MODE_MSA_V02:
      case eQMI_LOC_OPER_MODE_STANDALONE_V02:
      case eQMI_LOC_OPER_MODE_CELL_ID_V02:
      case eQMI_LOC_OPER_MODE_WWAN_V02:
        {
          pz_GetOperationModeInd->operationMode = pz_ClientInfo->z_FixCriteria.e_PreferredOperationMode;
          break;
        }
      default:
        {
          pz_GetOperationModeInd->operationMode = eQMI_LOC_OPER_MODE_DEFAULT_V02;
          break;
        }
    }

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    if (!locMW_SendQmiIndicationProxy(
          pz_ClientInfo->client_handle,
          QMI_LOC_GET_OPERATION_MODE_IND_V02,
          pz_GetOperationModeInd, q_RetVal))
    {
      LOC_MSG_ERROR("locClnt_GetOperationMode: Could not send IPC to LocMW", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
    }
  }while (0);

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   Return fix interval in milliseconds

@param[in] pz_FixCriteria:   FIX criteria pointer

@return    Fix interval in milliseconds
---------------------------------------------------------------------------*/
uint32 locClnt_GetFixInterval(const locClnt_FixCriteriaType *pz_FixCriteria)
{
  uint32 q_FixInterval = LOC_CLIENT_FIX_INTERVAL_MS_DEFAULT;

  if (NULL == pz_FixCriteria)
  {
    LOC_MSG_ERROR("locClnt_GetFixInterval: NULL pz_FixCriteria", 0, 0, 0);
    return q_FixInterval;
  }

  // Time between fixes
  if (LOC_CLIENT_NOTIFY_ON_DISTANCE == pz_FixCriteria->e_NotifyType)
  {
    q_FixInterval = pz_FixCriteria->q_MinDistSampleInterval;
  }
  else
  {
    q_FixInterval = pz_FixCriteria->q_TbfMs;
  }

  return q_FixInterval;
}

/*---------------------------------------------------------------------------
@brief
   Function to process the master client registration

@param[in] pz_ClientInfo:     Loc client pointer
@param[in] pz_RegMstrClntReq: Master client registration request message

@return    LOC_API_SUCCESS   Called successfully
@return    other value       Called unsuccessfully
---------------------------------------------------------------------------*/
static int locClnt_SetMasterClient
(
   loc_client_info_s_type *pz_ClientInfo,
   const qmiLocRegisterMasterClientReqMsgT_v02 *pz_RegMstrClntReq
   )
{
  locClnt_ModuleDataType *p_LocClntData = &(loc_middleware_data.z_locClnt_Data);
  qmiLocRegisterMasterClientIndMsgT_v02 *pz_RegMstrClntInd = NULL;
  uint32 q_RetVal = LOC_API_SUCCESS;
  boolean b_TestMasterClient = FALSE;
  uint32 q_ClientCnt = 0;

  // Pointers check
  if ((NULL == pz_ClientInfo) || (NULL == pz_RegMstrClntReq))
  {
    LOC_MSG_ERROR("locClnt_SetMasterClient: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  do
  {
    // ---------------------
    // Allocate indication message on the heap - it's the design of LOC_MW module. Memory will
    // be freed at other parts of the code. If the allocation for indication is unsuccessful,
    // unfortunately it means that the request has failed, client will get a failure RESP
    // ---------------------
    pz_RegMstrClntInd = (qmiLocRegisterMasterClientIndMsgT_v02 *)loc_calloc(sizeof(*pz_RegMstrClntInd));
    if (NULL == pz_RegMstrClntInd)
    {
      LOC_MSG_ERROR("locClnt_SetMasterClient: calloc return NULL", 0, 0, 0);
      q_RetVal = LOC_API_GENERAL_FAILURE;
      break;
    }

    // ---------------------
    // Find out what type of master client this is based on the master key
    // ---------------------
    if (pz_RegMstrClntReq->key == LOC_CLIENT_MASTER_VALUE)
    {
      b_TestMasterClient = FALSE;
      pz_ClientInfo->e_ClientType = eQMI_LOC_CLIENT_AFW_V02;
    }
    else if (pz_RegMstrClntReq->key == LOC_CLIENT_TEST_MASTER_VALUE)
    {
      b_TestMasterClient = TRUE;
      pz_ClientInfo->e_ClientType = eQMI_LOC_CLIENT_AFW_V02;
    }
    else
    {
      pz_RegMstrClntInd->status = eQMI_LOC_REGISTER_MASTER_CLIENT_INVALID_KEY_V02;
      break;
    }

    // ---------------------
    // If this point is reached, we are either dealing with regular master client or
    // TEST master client
    // ---------------------

    if (b_TestMasterClient == FALSE)
    {
      // ---------------------
      // Multiple registration is very uncommon, it could happen when HAL process crashes and
      // re-registers. Print out some messages.
      // ---------------------
      if (p_LocClntData->l_MasterClientHandle != LOC_CLIENT_HANDLE_INVALID)
      {
        // Treat the scenario where existing master client re-registers as a normal scenario
        if (p_LocClntData->l_MasterClientHandle != pz_ClientInfo->client_handle)
        {
          LOC_MW_CLIENT_MSG_HIGH(pz_ClientInfo, "locClnt_SetMasterClient: master client %d replaced by %d",
                                 p_LocClntData->l_MasterClientHandle, pz_ClientInfo->client_handle, 0);
        }
      }

      // ---------------------
      // If this point is reached, that means the key is valid and we're registering this client as master client
      // ---------------------
      p_LocClntData->l_MasterClientHandle = pz_ClientInfo->client_handle;
    }
    else
    {
      // ---------------------
      // If the function returns FALSE that means the array is FULL, return ERR indication
      // ---------------------
      if (locClnt_TestMasterClientCheck(pz_ClientInfo->client_handle, eLOC_CLNT_TMCF_SET) == FALSE)
      {
        LOC_MW_CLIENT_MSG_HIGH(pz_ClientInfo, "locClnt_SetMasterClient %d: Max Test clients %d reached!",
                               pz_ClientInfo->client_handle, LOC_CLIENT_MAX_TEST_MASTER_CLIENTS, 0);
        pz_RegMstrClntInd->status = eQMI_LOC_REGISTER_MASTER_CLIENT_ALREADY_HAVE_MASTER_CLIENT_V02;
        break;
      }
    }

    pz_RegMstrClntInd->status = eQMI_LOC_REGISTER_MASTER_CLIENT_SUCCESS_V02;

  } while (0);

  // Send IPC message to LocMW task to handle QMI_LOC indication
  if (!locMW_SendQmiIndicationProxy(
        pz_ClientInfo->client_handle,
        QMI_LOC_REGISTER_MASTER_CLIENT_IND_V02,
        pz_RegMstrClntInd, LOC_API_CUSTOM_ERROR_STATUS))
  {
    LOC_MW_CLIENT_MSG_ERROR(pz_ClientInfo, "locClnt_SetMasterClient: Could not send IPC to LocMW", 0, 0, 0);
    q_RetVal = LOC_API_GENERAL_FAILURE;
  }

  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
   This function is supposed to be called at power up to initialize some
   internal data structures

@param[in] None

@return None
---------------------------------------------------------------------------*/
void locClnt_TaskInit(void)
{
  // Sort the list Master commands to assist in doing binary search
  locClnt_SortUint32(z_locClntMasterCmds, sizeof(z_locClntMasterCmds) / sizeof(z_locClntMasterCmds[0]));
}

/*---------------------------------------------------------------------------
@brief
   Function to check if the postion request is locked for the client.
   There are 2 types of clients, Android framework (AFW) and Non-AFW(NFW).

@param[in] l_ClientHandle:   Loc client handle

@return    TRUE/FALSE
---------------------------------------------------------------------------*/
boolean locClnt_IsPosRequestAllowed2
(
   loc_client_handle_type l_ClientHandle
   )
{
  loc_client_info_s_type *pz_Client = locClnt_FindHandle(l_ClientHandle);

  if (!pz_Client)
    return FALSE;
  else
    return locClnt_IsPosRequestAllowed(pz_Client);

}

/*---------------------------------------------------------------------------
@brief
   Function to check if the postion request is locked for the client.
   There are 2 types of clients, Android framework (AFW) and Non-AFW(NFW).

@param[in] pz_Client:   Loc client handle

@return    TRUE/FALSE
---------------------------------------------------------------------------*/
boolean locClnt_IsPosRequestAllowed
(
   loc_client_info_s_type *pz_Client
   )
{
  if (NULL == pz_Client)
  {
    LOC_MSG_ERROR("locClnt_IsPosRequestAllowed, NULL pz_Client", 0, 0, 0);
    return FALSE;
  }

  if (loc_middleware_data.v_E911Session)
  {
    LOC_MSG_LOW("locClnt_IsPosRequestAllowed, E911 session", 0, 0, 0);
    return TRUE;
  }

  if (eQMI_LOC_CLIENT_PRIVILEGED_V02 == pz_Client->e_ClientType)
  {
    LOC_MSG_LOW("locClnt_IsPosRequestAllowed, privelege enable = %d",
                loc_middleware_data.v_AfwAllowPriPosReq, 0, 0);
    return loc_middleware_data.v_AfwAllowPriPosReq;
  }

  if (eQMI_LOC_CLIENT_AFW_V02  == pz_Client->e_ClientType)
  {
    LOC_MSG_LOW("locClnt_IsPosRequestAllowed, AFW enbale = %d",
                loc_middleware_data.v_AfwAllowAfwPosReq, 0, 0);
    return loc_middleware_data.v_AfwAllowAfwPosReq;
  }
  else
  {
    LOC_MSG_LOW("locClnt_IsPosRequestAllowed, Nv enable = %d, NFW enable = %d",
                loc_middleware_data.v_NvAllowNfwPosReq,
                loc_middleware_data.v_AfwAllowNfwPosReq, 0);
    /* IF NV == ACCEPT_ALWAYS THEN
         Allow
       ELSE IF NFW_LOCK == UNLOCKED THEN
         Allow
       ELSE
         Don't Allow */
    return (loc_middleware_data.v_NvAllowNfwPosReq ||
            loc_middleware_data.v_AfwAllowNfwPosReq);
  }
}

/*---------------------------------------------------------------------------
@brief
   Function to check if the client is a NFW client, and the position request
   notification is allowed to be sent to the master client

@param[in] pz_Client:   Loc clinet info handle

@return    TRUE/FALSE
---------------------------------------------------------------------------*/
boolean locClnt_IsNFWPosRequestNotificationAllowed
(
   loc_client_info_s_type *pz_Client
   )
{
  if (NULL == pz_Client)
  {
    LOC_MSG_ERROR("IsNFWPositionReportAllowed, NULL pz_Client", 0, 0, 0);
    return FALSE;
  }

  LOC_MSG_LOW("IsNFWPosRequestNotificationAllowed, e_ClientType = %d",
              pz_Client->e_ClientType, 0, 0);
  if (eQMI_LOC_CLIENT_AFW_V02 == pz_Client->e_ClientType)
    return FALSE;

  LOC_MSG_LOW("IsNFWPosRequestNotificationAllowed, NV allow  = %d client allow  = %d",
              loc_middleware_data.v_NvAllowNfwPosReqNotification,
              pz_Client->v_EnablePosRequestNotification, 0);
  /* IF NV==ALLOW AND THE NFW CLIENT EXPLICITLY PERMITS THEN
       Yes
     ELSE
       No */
  return (loc_middleware_data.v_NvAllowNfwPosReqNotification &&
          pz_Client->v_EnablePosRequestNotification);
}

/*---------------------------------------------------------------------------
@brief
   Function to handle E911 state IPC message from the session manager

@param[in] p_IpcMsg:        IPC Message containing the E911 status

@return    none
---------------------------------------------------------------------------*/
void locClnt_HandleE911StateInd
(
   const os_IpcMsgType *p_IpcMsg
   )
{
  if (NULL == p_IpcMsg)
  {
    LOC_MSG_ERROR("locClnt_HandleE911StateInd: NULL IPC message", 0, 0, 0);
    return;
  }
  else
  {
    boolean   v_E911Session = *(p_IpcMsg->p_Data);
    loc_client_info_s_type *pz_Client = NULL;
    loc_middleware_data.v_E911Session = v_E911Session;

    /* if E911 session is stopped, check if any
    active session shall be stopped */
    if (v_E911Session == FALSE)
    {
      for (pz_Client = (loc_client_info_s_type *)q_check(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue));
           pz_Client != NULL;
           pz_Client = (loc_client_info_s_type *)q_next(&(loc_middleware_data.z_locClnt_Data.z_LocClientQueue), &pz_Client->z_Link))
      {
        if ((TRUE == pz_Client->v_InUse ) &&
             ((FALSE == locClnt_IsPosRequestAllowed(pz_Client))&&
              (FALSE == locClnt_IsSpecialPosRequest(pz_Client->z_FixCriteria.e_SpecialRequestType))))
        {
          boolean v_NotifyClient = FALSE;
          if (CLIENT_STATE_ACTIVE == pz_Client->e_ClientState)
          {
            locClnt_StopFix(pz_Client->client_handle);
            v_NotifyClient = TRUE;
          }
          if (pz_Client->v_PositionListener)
          {
            pz_Client->v_PositionListener = FALSE;
            v_NotifyClient = TRUE;
          }
          if (TRUE == v_NotifyClient)
          {
            locQmiShimIndInfoStructT               z_IndInfo = { 0 };
            qmiLocEventPositionReportIndMsgT_v02   z_PosRptInd = { 0 };

            z_PosRptInd.sessionStatus  = eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02;
            z_IndInfo.q_Id             = QMI_LOC_EVENT_POSITION_REPORT_IND_V02;
            z_IndInfo.q_MsgLen         = sizeof(z_PosRptInd);
            z_IndInfo.p_Msg            = &z_PosRptInd;
            LOC_MSG_MED("locClnt_HandleE911StateInd: stop client_handle %d",
                        pz_Client->client_handle, 0, 0);
            locQmiShimSendInd(pz_Client->client_handle, &z_IndInfo);
          }
        }
      }
      /*At 911 call end recalculate fix Criteria for all Clients. 
      In case a SpecialReq Client was active before 911 call this will resume
      the client's session as SpecialReq in SM*/
      locClnt_SetCommonPosCriteria();
    }
  }
}

/*---------------------------------------------------------------------------
@brief
   Function to check if the postion request is needs special handling
   ex: bypass NFW/AFW locks.

@param[in] e_SpecialReq:   qmiLocSpecialReqEnumT_v02

@return    TRUE/FALSE
---------------------------------------------------------------------------*/
boolean locClnt_IsSpecialPosRequest(qmiLocSpecialReqEnumT_v02 e_SpecialReq)
{
   boolean b_retVal = FALSE;

   if (loc_middleware_data.v_E911Session)
   {
     LOC_MSG_LOW("locClnt_IsSpecialPosRequest, E911 session", 0, 0, 0);
     b_retVal = TRUE;
     return (b_retVal);
   }

   switch(e_SpecialReq)
   {
     case eQMI_LOC_SPECIAL_REQUEST_SHORT_CODE_V02:
          LOC_MSG_MED_0("locClnt_IsSpecialPosRequest: Request Type: ShortCode");
          b_retVal = TRUE;
          break;
     default:
          LOC_MSG_MED_0("locClnt_IsSpecialPosRequest: Not Supported");
          break;   
   }
   return b_retVal;
}

