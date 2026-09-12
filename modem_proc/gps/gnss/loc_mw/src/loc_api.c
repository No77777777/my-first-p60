/******************************************************************************
  @file:  loc_api.c
  @brief: Location Middleware task

  DESCRIPTION
  This module defines the loc API routines

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Copyright (c) 2013-2014 QUALCOMM Atheros, Inc.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  Copyright (c) 2015-2019 Qualcomm Technologies, Inc.
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
05/19/16   yh       LocMW refactoring 1
01/11/16   yh       LocMW refactoring 0
06/11/15   sjk      LB 2.0 Integration
09/05/14   ssu      GM LOWI Integration
04/15/14   yh       XTRA version check added   
03/10/14   ah       Added support for Set Premium Services Cfg & Get Avail WWAN Pos
05/02/11   pn       Added handler for OC_IOCTL_ERROR_ESTIMATE_CONFIG.
04/07/10   ns       Make sure that invalid client handle is returned if
                    the loc mw task is not up or if initialization
                    fails during loc_open   

$Id: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_api.c#1 $
$DateTime: 2019/10/24 01:30:36 $
======================================================================*/

#include "gps_variation.h"
#include "comdef.h"
#include "customer.h"
#include "target.h"

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
#include "gnss_api.h"
#include "loc_geofence.h"
#include "loc_batching.h"
#include "loc_aon.h"
#include "loc_utils.h"
#include "loc_ot_batching.h"

// Global data variable

// zero-initialize the global variable, so this flag loc_mw_task_started would be FALSE at start up,
// even before our lock-less RPC server gets to run. This works only if we live in the same process so data initialization is
// always done by compiler generated code before any C code.
// what we really want is {.loc_mw_task_started = FALSE}, but that would require a C99 compiler, which we may or may not have
// on various platforms.
loc_middleware_data_s_type loc_middleware_data = { 0 };

/*===========================================================================
FUNCTION loc_initialize_modules

DESCRIPTION
  This function initializes the sub-modules of loc-mw based on the events that 
  need to be generated

PARAMETERS
  t_EventRegMask:          Events which the client is interested in receiving.
                           The modules which generate these events will be 
                           initialized.

DEPENDENCIES

RETURN VALUE
  TRUE: If modules are initialized successfully
  FALSE: If modules are not initialized successfully

SIDE EFFECTS
  race condition could happen if more than one LocAPI is calling loc_open.
  loc_middleware_data.crit_sec is mostly used for data protection 
  we need some mutex in safe guarding state machines or procedures, 
  which implies redesigning of LocMW

===========================================================================*/

static boolean loc_initialize_modules(const locClnt_EventMaskType t_EventRegMask)
{
  boolean result = FALSE;

  // poor man's exception handling: do...while with break
  do
  {
    LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);

    // Location sub-module will be initialzied when the first location client
    // registers for the corresponding event
    if ((t_EventRegMask & LOC_EVENT_PARSED_POSITION_REPORT) ||
        (t_EventRegMask & LOC_EVENT_NMEA_1HZ_REPORT) ||
        (t_EventRegMask & LOC_EVENT_NMEA_POSITION_REPORT) ||
        (t_EventRegMask & LOC_EVENT_WPS_NEEDED_REQUEST))
    {
      if (loc_middleware_data.pd_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locPd_Init())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.pd_module_initialized = TRUE;
      }

      if (loc_middleware_data.pa_module_initialized  == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locPa_Init())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.pa_module_initialized = TRUE;
      }
    }

    if (t_EventRegMask & LOC_EVENT_NMEA_POSITION_REPORT ||
        t_EventRegMask & LOC_EVENT_NMEA_1HZ_REPORT)
    {
      if (loc_middleware_data.nmea_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locPd_InitNmea())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.nmea_module_initialized = TRUE;
      }
    }

    if (t_EventRegMask & LOC_EVENT_LOCATION_SERVER_REQUEST)
    {
      if (loc_middleware_data.conn_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locConn_Init())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.conn_module_initialized = TRUE;
      }
    }

    if (t_EventRegMask & LOC_EVENT_ASSISTANCE_DATA_REQUEST)
    {
      if (loc_middleware_data.xtra_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locXtra_Init())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.xtra_module_initialized = TRUE;
      }
    }

    if (t_EventRegMask & LOC_EVENT_NI_NOTIFY_VERIFY_REQUEST)
    {
      if (loc_middleware_data.ni_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        if (FALSE == locNi_Init())
        {
          break;
        }
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.ni_module_initialized = TRUE;
      }
    }

    if (t_EventRegMask & LOC_EVENT_WPS_NEEDED_REQUEST)
    {
      if (loc_middleware_data.wifi_module_initialized == FALSE)
      {
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
        loc_middleware_data.wifi_module_initialized = locWifi_Init();
        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
      }
    }

    if (FALSE == loc_middleware_data.pa_module_initialized)
    {
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
      if (FALSE == locPa_Init())
      {
        break;
      }
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
      loc_middleware_data.pa_module_initialized = TRUE;
    }

    if (loc_middleware_data.geofence_module_initialized == FALSE)
    {
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
      if (FALSE == locGeofenceInit())
      {
        break;
      }
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
      loc_middleware_data.geofence_module_initialized = TRUE;
    }

    if (loc_middleware_data.aon_module_initialized == FALSE)
    {
      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
      if (FALSE == locAonInit())
      {
        break;
      }
      LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
      loc_middleware_data.aon_module_initialized = TRUE;
    }
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

    // finally, we're done with initialization
    result = TRUE;
  }while (0);

  return result;
}

/*===========================================================================
FUNCTION loc_open2

DESCRIPTION
  This function connects a location client to the location engine.
  If connection is successful, it will return a handle which the location client
  should use for future location operations. If connection fails, the return 
  value will be LOC_CLIENT_HANDLE_INVALID.

  This is different from loc_open2, as it does not require the event-
  registration mask.

PARAMETERS
  event_callback:          a function pointer which will be invoked by the GPS
                           server when an event of interest happens.

DEPENDENCIES

RETURN VALUE
  -1 (LOC_CLIENT_HANDLE_INVALID) if connection fails.
  Otherwise, a number >= 0.

SIDE EFFECTS

===========================================================================*/

loc_client_handle_type loc_open2
(
   loc_event_cb_f_type    *event_callback
   )
{
  loc_client_handle_type client_handle = LOC_CLIENT_HANDLE_INVALID;

  // If location middleware task hasn't started return invalid handle
  if (loc_middleware_initialized() != TRUE)
  {
    return LOC_CLIENT_HANDLE_INVALID;
  }

  // try to initialize all modules (except Conn) at client registration
  // this slightly reduces complexity when client shall want to change
  // event registration later. loc_conn will be initialized later, if
  // the client indicates that it can process LOCATION_SERVER requests.
  if (TRUE == loc_initialize_modules((locClnt_EventMaskType)~LOC_EVENT_LOCATION_SERVER_REQUEST))
  {
    client_handle = locClnt_Init(event_callback);
  }
  return client_handle;
}

/*===========================================================================
FUNCTION loc_close

DESCRIPTION
  This function discconect the client from the location engine.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  the closed client handle

SIDE EFFECTS

===========================================================================*/
int32 loc_close(loc_client_handle_type handle)
{
  int32                      l_RetVal = LOC_API_INVALID_HANDLE;
  loc_client_info_s_type    *pz_LocClient = NULL;
  locClnt_EventMaskType      t_EventRegMask = 0;

  pz_LocClient = locClnt_FindHandle(handle);

  if (pz_LocClient == NULL)
  {
    LOC_MSG_ERROR("loc_close, invalid client handle %d", handle, 0, 0);
    return l_RetVal;
  }
  t_EventRegMask = pz_LocClient->t_EventRegMask;

  /* first stop the session */
  (void)loc_stop_fix(handle);

  /* stop any TBB requests from this client */
  /* check if this is batching client and do cleanup */
  if (TRUE == pz_LocClient->batching_info.is_batching_client)
  {
    // force stop all AON requests
    locAonForceStopAll(handle);

    /* free the buffer and reset client info */
    (void)loc_batching_free_and_reset(pz_LocClient);

    LOC_MSG_HIGH("loc_close : cleaned up TBB client", 0, 0, 0);
  }

  /* stop any OTB requests from this client */
  /* check if this is OTB client and do cleanup */
  if (TRUE == pz_LocClient->z_OtbInfo.v_OtbClient)
  {
    // force stop all AON requests
    locAonForceStopAll(handle);

    /* free the buffer and reset client info */
    (void)locOtb_FreeAndReset(pz_LocClient);

    LOC_MSG_HIGH("loc_close : cleaned up OTB client", 0, 0, 0);
  }

  /* check if it is the master client */
  if (handle == loc_middleware_data.z_locClnt_Data.l_MasterClientHandle)
  {
    loc_middleware_data.z_locClnt_Data.l_MasterClientHandle = LOC_CLIENT_HANDLE_INVALID;
  }

  l_RetVal = locClnt_Release(handle);

  if (LOC_API_SUCCESS == l_RetVal &&
      (t_EventRegMask & LOC_EVENT_LOCATION_SERVER_REQUEST))
  {
    /* Dereg pdsm atl proxy if there is no client registers for LOC_EVENT_LOCATION_SERVER_REQUEST. */
    if (FALSE == locClnt_IsServerRequestHandlerRegistered())
    {
      if (FALSE == locConn_DeInit())
      {
        LOC_MSG_ERROR("loc_close, Dereg pdsm atl proxy failed!", 0, 0, 0);
      }
      else
      {
        LOC_MSG_MED("loc_close, Dereg pdsm atl proxy succeed!", 0, 0, 0);
      }
    }
    else
    {
      LOC_MSG_LOW("loc_close, Do not dereg pdsm atl proxy", 0, 0, 0);
    }
  }

  LOC_MSG_HIGH("loc_close handle %d returned %d", handle, l_RetVal, 0);
  return l_RetVal;
}

/*===========================================================================
FUNCTION loc_close

DESCRIPTION
  This function discconects the client from the location engine.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  On success: it returns 0 (LOC_API_SUCCESS).
  On failure, it will return one-zero error code.

SIDE EFFECTS

===========================================================================*/
int32 loc_start_fix
(
   loc_client_handle_type handle
   )
{
  int                       ret_val;

  ret_val = locClnt_StartFix(handle);
  LOC_MSG_HIGH("loc_start_fix, client = %d, ret_val = %d", handle, ret_val, 0);

  return ret_val;
}

/*===========================================================================
FUNCTION loc_stop_fix

DESCRIPTION
  This function stops positioning process for this client. If client is not
  currently requesting fixes, it will return LOC_API_SUCCESS.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  On success: it returns 0 (LOC_API_SUCCESS).
  On failure, it will return one-zero error code.

SIDE EFFECTS

===========================================================================*/
int32 loc_stop_fix
(
   loc_client_handle_type handle
   )
{
  int  ret_val;

  ret_val = locClnt_StopFix(handle);
  LOC_MSG_HIGH("loc_stop_fix, client = %d, ret_val = %d", handle, ret_val, 0);
  return ret_val;
}
