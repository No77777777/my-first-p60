/*===========================================================================

                            TM_API

DESCRIPTION: TM's APIs.


  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2018,2020 Qualcomm Technologies, Inc.
  Confidential and Proprietary - Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_api.c#3 $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
  01/28/20    anksha  Added support for navic assist download
  08/10/16    mj      Handle Leap Second Updates from ME
  07/07/16    gurdars Updated XTRA3.x indication API's to pass the constellation info
  06/06/16    mj      Add GERA support for AP-MP
  05/01/15    rh      Added get gps lock TM API
  11/18/14    ah      Added support for PrisonerSV persistent blacklist SV masks
  10/27/14    rk      Added LPP RSTD >24 measurement processing support
  01/31/13    mj      Modify tm_api_reg_task_init_notifier to handle race condition
  01/12/13    gk      LPP ECID changes
  04/18/12    ssu     Added tm_api_update_best_avail_pos()
  03/01/12    rh      Added tm_api_send_engine_error_recovery_report()
  06/10/09    gk      Added Get UTC offset from NV
  01/12/09    atien   XTRA-T support
  09/15/08    gk      Rename the on-demand struct to sm_ from cgps_
  04/25/08    GK      On-Demand Changes.
  07/10/07    LT      Initial check-in.
===========================================================================*/

#include "gps_variation.h"
#include "comdef.h"
#include "aries_os_api.h"
#include "gps_common.h"
#include "tm_common.h"
#include "tm_api.h"
#include "tm_data.h"
#include "tm_xtra.h"



#include "tm_xspi.h"


#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
/* Pointer to NMEA variable length debug string. The lifetime of this heap storage
*  is from NMEA debug string (PQWP7) generation to until NMEA debug sentence type is disabled
*  (through QMI).
*/
static uint8 *pu_NmeaVarLenBuff = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                        GLOBAL DATA DECLARATIONS                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                          FUNCTION DECLARATIONS                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                          FUNCTION DEFINITIONS                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

uint8 tm_map_client_threadid_to_index(os_ThreadIdEnumType clientThreadId)
{
  switch (clientThreadId)
  {
    case THREAD_ID_LOC_MIDDLEWARE:
      return (uint8)TM_LOC_MW_CLIENT_INDEX;

    case THREAD_ID_SDP:
      return (uint8)TM_SDP_CLIENT_INDEX;

    case THREAD_ID_LBS:
      return (uint8)TM_LBS_CLIENT_INDEX;

    default:
      return (uint8)TM_UNSUPPORTED_CLIENT_INDEX;
  }

}






/*******************************************************************************
 * tm_api_store_ftcal_tt_data
 *
 * Function description:
 *
 * Parameters:
 *
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/
void tm_api_store_ftcal_tt_data(cgps_FtcalTTStruct *p_TTData)
{
  os_IpcMsgType                  *ipc_msg_ptr;
  cgps_FtcalTTStruct             *ipc_data_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Send TT data to TM", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(cgps_FtcalTTStruct), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (cgps_FtcalTTStruct *)ipc_msg_ptr->p_Data; /*lint !e826 */

    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_FTCAL_TT_DATA;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(cgps_FtcalTTStruct);

    *ipc_data_ptr = *p_TTData;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */

}

/*******************************************************************************
 * tm_api_store_ftcal_ppm_data
 *
 * Function description:
 *
 * Parameters:
 *
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/
void tm_api_store_ftcal_ppm_data(cgps_FtcalPpmStruct *p_PpmData)
{
  os_IpcMsgType                  *ipc_msg_ptr;
  cgps_FtcalPpmStruct             *ipc_data_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Send PPM data to TM", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(cgps_FtcalPpmStruct), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (cgps_FtcalPpmStruct *)ipc_msg_ptr->p_Data; /*lint !e826 */

    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_FTCAL_PPM_DATA;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(cgps_FtcalPpmStruct);

    *ipc_data_ptr = *p_PpmData;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */

}


/*===========================================================================
FUNCTION tm_api_update_cell_info

DESCRIPTION: for entities outside TM to provide TM with Cell info update.

RETURN VALUE

DEPENDENCIES
=============================================================================*/

void tm_api_update_cell_info(const cgps_ActiveRATEnumType e_TriggerRAT, const cgps_CellInfoCachedStruct *const p_CellInfoCached)
{
  os_IpcMsgType                  *ipc_msg_ptr;
  tm_cell_info_update_type       *ipc_data_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Cached Cell ID Info updated", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(tm_cell_info_update_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (tm_cell_info_update_type *)ipc_msg_ptr->p_Data; /*lint !e826 */

    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_MC_CELLDB_UPDATE;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(tm_cell_info_update_type);

    memscpy((void *)&(ipc_data_ptr->cellInfoCached), sizeof(ipc_data_ptr->cellInfoCached), (void *)p_CellInfoCached, sizeof(*p_CellInfoCached));
    ipc_data_ptr->e_TriggerRAT = e_TriggerRAT;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */

}


/*===========================================================================
FUNCTION tm_api_update_gps_state_info

DESCRIPTION: for entities outside TM to provide TM with position and its
associated state info update.

RETURN VALUE

DEPENDENCIES
=============================================================================*/

void tm_api_update_gps_state_info(const sm_FixReportStructType *const p_PosInfo)
{

  os_IpcMsgType                  *ipc_msg_ptr;
  sm_FixReportStructType         *ipc_data_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: POS PUNC TUNC updated", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(sm_FixReportStructType), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (sm_FixReportStructType *)ipc_msg_ptr->p_Data; /*lint !e826 */

    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_BEST_AVAIL_POS_REPORT;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(sm_FixReportStructType);

    *ipc_data_ptr = *p_PosInfo;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */

}

/*******************************************************************************
 * tm_api_report_utc_offset
 *
 * Function description:
 *  Get UTC offset from TM
 *
 * Parameters:
 *
 *  p_UtcOffset - Pointer to UTC offset information
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/
void tm_api_report_utc_offset(uint8 *p_UtcOffset)
{
  (void)tm_core_get_utc_offset(p_UtcOffset);
}


/**
@brief dispatches an asynchronous request for TM to raise the external spi
       injection event and request SPI injection
@param
@return
*/
void tm_api_xspi_start_injection(void)
{
  /* send TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST ipc message to tm */
  os_IpcMsgType  *p_ipcMsg = NULL;
  tm_xspi_request_type *p_reqData = NULL;

  p_ipcMsg = os_IpcCreate(sizeof(tm_xspi_request_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);
  if (p_ipcMsg)
  {
    p_reqData = (tm_xspi_request_type *)p_ipcMsg->p_Data;
    p_ipcMsg->q_MsgId = TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST;
    p_reqData->q_stream_status = TM_XSPI_STREAM_STATUS_ENABLE;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to create IPC message", 0, 0, 0);
  }
  return;
}

/**
@brief dispatches an asynchronous request for TM to raise the external spi
       injection event and stop SPI injection
@param
@return
*/
void tm_api_xspi_stop_injection(void)
{
  /* send TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST ipc message to tm */
  os_IpcMsgType  *p_ipcMsg = NULL;
  tm_xspi_request_type *p_reqData = NULL;

  p_ipcMsg = os_IpcCreate(sizeof(tm_xspi_request_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);
  if (p_ipcMsg)
  {
    p_reqData = (tm_xspi_request_type *)p_ipcMsg->p_Data;
    p_ipcMsg->q_MsgId = TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST;
    p_reqData->q_stream_status = TM_XSPI_STREAM_STATUS_DISABLE;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to create IPC message", 0, 0, 0);
  }
}

/*===========================================================================
  tm_loc_lte_otoda_recv_measurements

  Description:

     This function is used by LOC_LTE_OTDOA module to send the OTDOA measurements

  Parameters:
    Reason: stop reason

  Return value:
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_loc_lte_otdoa_recv_measurements(sm_loc_lte_otdoa_meas_info_type *p_otdoa_measurement_info)
{

  os_IpcMsgType   *ipc_msg_ptr  = NULL;
  sm_loc_lte_otdoa_meas_info_type *ipc_data_ptr;

  ipc_msg_ptr = os_IpcCreate(sizeof(sm_loc_lte_otdoa_meas_info_type),
                             IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_MGP_MC);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (sm_loc_lte_otdoa_meas_info_type *)ipc_msg_ptr->p_Data;
    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_SM_LOC_LTE_OTDOA_MEASUREMENTS;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(sm_loc_lte_otdoa_meas_info_type);
    /* Internal p_meas_info pointer storage is persistant until the SM LPP CP session ends */
    *ipc_data_ptr = *p_otdoa_measurement_info;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message (OTDOA Measurements)", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return FALSE;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: successfully sent an IPC message (OTDOA measurements)", 0, 0, 0);
      return TRUE;
    }
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to create IPC message (tm_api_xtm_nv_status_report)", 0, 0, 0);
    return FALSE;
  } /* End ipc_msg_ptr */
}



/*******************************************************************************
 * tm_api_XTM_NV_status_report
 *
 * Function description:
 *  Called by XTRAT module to report the success of NV write to TM thread.
 *
 * Parameters:
 * which nv item has been set
 *
 * Return value:
 * none
 *
 *******************************************************************************/

boolean tm_api_xtm_nv_status_report(tm_xtm_struct_nv_write_status *xtm_nv_set)
{
  os_IpcMsgType                  *ipc_msg_ptr;
  tm_xtm_struct_nv_write_status   *ipc_data_ptr;

  ipc_msg_ptr = os_IpcCreate(sizeof(tm_xtm_struct_nv_write_status), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (tm_xtm_struct_nv_write_status *)ipc_msg_ptr->p_Data;
    switch (xtm_nv_set->xtm_nv_item_set)
    {
      case TM_XTM_SET_CLIENT_TOKEN:
        ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_XTM_CLIENT_TOKEN;
        break;

      case TM_XTM_SESSION_CONTROL:
        ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_XTM_SESSION_CONTROL;
        break;
    }

    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(tm_xtm_struct_nv_write_status);
    *ipc_data_ptr = *xtm_nv_set;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message (tm_api_xtm_nv_status_report)", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return FALSE;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: successfully sent an IPC message (tm_api_xtm_nv_status_report)", 0, 0, 0);
      return TRUE;
    }
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to create IPC message (tm_api_xtm_nv_status_report)", 0, 0, 0);
    return FALSE;
  } /* End ipc_msg_ptr */
}

/*=============================================================================

FUNCTION
  tm_api_reg_task_init_notifier

DESCRIPTION
  Called by potential clients to register their callback. The callback is
  used to notify the clients that the API/Engine is now ready to accept clients

  If the API/Engine is ready to accept clients, the callback will be called
  inline.  The caller should be ready to handle this and queue an IPC to itself.

DEPENDENCIES
  None.

PARAMETERS
  pNotifierCb - The callback of the client to be registered
  clientThreadId - Client Thread ID to identify the index within the array

RETURN VALUE
  TRUE:  registration has succeeded, or if Engine is ready the callback is
         called immediately;
  FALSE:  registration failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean tm_api_reg_task_init_notifier(sm_ApiInitNotifierCb pNotifierCb, os_ThreadIdEnumType clientThreadId)
{
  os_IpcMsgType             *ipc_msg_ptr = NULL;
  tm_api_notifier_type      *ipc_data_ptr = NULL;
  uint8 errCode = 0;
  boolean ipc_Status = FALSE;
  uint8 index;

  if (pNotifierCb)
  {
    index = tm_map_client_threadid_to_index(clientThreadId);

    if (index < TM_MAX_NOTIFIERS)
    {
      if (FALSE == pdsm_get_ready_flag())
      { /* TM not ready to call the callback, so save the callbacks to call later when TM is up */
        pz_SmNotifierCb[index] = pNotifierCb;
      }
      else
      {
        /* TM is ready, so send an IPC to TM with index and callback */
        ipc_msg_ptr = os_IpcCreate(sizeof(tm_api_notifier_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

        if (ipc_msg_ptr)
        {
          ipc_data_ptr = (tm_api_notifier_type *)ipc_msg_ptr->p_Data;

          ipc_data_ptr->pNotifierCb = pNotifierCb;
          ipc_data_ptr->clientIndex = index;
          ipc_data_ptr->broadcast = FALSE;

          ipc_msg_ptr->q_MsgId  = TM_CORE_MSG_ID_API_REGISTRATION;
          ipc_Status  = os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM);
          if (ipc_Status == FALSE)
          {
            ipc_Status = os_IpcDelete(ipc_msg_ptr);
            ipc_msg_ptr = NULL;
            return FALSE;
          }
        }
        else
        { /* ipc_msg_ptr == NULL */
          errCode = 1;
        }
      }
    }
    else
    { /* (index >= TM_MAX_NOTIFIERS) */
      errCode = 2;
    }
  }
  else
  { /* pNotifierCb == NULL */
    errCode = 3;
  }

  if (errCode > 0)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_api_reg_task_init_notifier failed with error code = %d", errCode, 0, 0);
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/**
@brief Calls the tm_xtra function for indicating xtra injection
       has completed
@param q_AcceptedMask:Mask indicating the accpeted constellation's by PE.
@return None
*/
void tm_api_xtra_inj_done(uint32  q_AcceptedMask)
{
  tm_xtra_inj_done(q_AcceptedMask);
}

/*******************************************************************************
 * tm_api_report_xooffset_info
 *
 * Function description:
 *  Called by LM to update TM with the latest XO offset info
 *
 * Parameters: 
 *
 *  pz_ReportXOOffsetInfo - XO offset Info
 *
 * Return value: 
 *
 *  void
 *
 *******************************************************************************/

void tm_api_report_xooffset_info
(
   const sm_ReportXOOffsetInfoStructType *pz_ReportXOOffsetInfo
   )
{
  os_IpcMsgType *pz_IpcMsgPtr  = NULL;

  pz_IpcMsgPtr = os_IpcCreate(sizeof(sm_ReportXOOffsetInfoStructType),
                              IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if ((pz_IpcMsgPtr != NULL) && (pz_ReportXOOffsetInfo != NULL))
  {
    sm_ReportXOOffsetInfoStructType *pz_IpcDataPtr;
    pz_IpcDataPtr = (sm_ReportXOOffsetInfoStructType *)pz_IpcMsgPtr->p_Data;

    pz_IpcMsgPtr->q_MsgId          = (uint32)TM_CORE_MSG_ID_XO_OFFSET_INFO;
    pz_IpcMsgPtr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    pz_IpcMsgPtr->q_Size           = sizeof(*pz_IpcDataPtr);

    /* Copy all the data over */
    pz_IpcDataPtr->l_RefOffsetPpb = pz_ReportXOOffsetInfo->l_RefOffsetPpb;
    pz_IpcDataPtr->l_ClkFreqBiasPpb = pz_ReportXOOffsetInfo->l_ClkFreqBiasPpb;
    pz_IpcDataPtr->l_ClkFreqBiasUncPpb = pz_ReportXOOffsetInfo->l_ClkFreqBiasUncPpb;
    pz_IpcDataPtr->v_IsDataValid = pz_ReportXOOffsetInfo->v_IsDataValid;

    if (!os_IpcSend(pz_IpcMsgPtr, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "tm_api_report_xooffset_info: Failed to send IPC message");
      (void)os_IpcDelete(pz_IpcMsgPtr);
    }
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "pz_IpcMsgPtr NULL? %d,  pz_ReportXOOffsetInfo NULL? %d",
          (pz_IpcMsgPtr == NULL), (pz_ReportXOOffsetInfo == NULL));
  }
}

/*******************************************************************************
 * tm_api_report_wbiq_info
 *
 * Function description:
 *  Called by LM to update TM with the latest WBIQ FFT info
 *
 * Parameters:
 *
 *  p_ReportWBIQInfo - WBIQ Info
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/

void tm_api_report_wbiq_info(const sm_ReportWBIQInfoStructType *p_ReportWBIQInfo)
{
  os_IpcMsgType                  *ipc_msg_ptr;
  int i;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Report WBIQ Info", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(sm_ReportWBIQInfoStructType), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if ((ipc_msg_ptr != NULL) && (p_ReportWBIQInfo != NULL))
  {
    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MGP_WBIQ_INFO;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(sm_ReportWBIQInfoStructType);

    /* Copy all the data over */
    memscpy((void *)ipc_msg_ptr->p_Data, sizeof(sm_ReportWBIQInfoStructType),
            (void *)p_ReportWBIQInfo, sizeof(sm_ReportWBIQInfoStructType));
   
    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "tm_api_report_wbiq_info: Failed to send IPC message");
      (void)os_IpcDelete(ipc_msg_ptr);
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "tm_api_report_wbiq_info: Input parameter check failed");
  }

}

/*******************************************************************************
 * tm_api_nav_config_change_resp
 *
 * Function description:
 *  Called by LM to update TM with the result of the NAV config change cmd.
 *
 * Parameters:
 *
 *  q_GnssConfig - Current GNSS configuration
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/
void tm_api_nav_config_change_resp(const uint32 q_GnssConfig)
{
  os_IpcMsgType                       *p_IpcMsg;

  p_IpcMsg = os_IpcCreate(sizeof(uint32), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_IpcMsg != NULL)
  {
    p_IpcMsg->q_MsgId          = (uint32)TM_CORE_MGP_NAV_CONFIG_CHANGE_RESP;
    memscpy(p_IpcMsg->p_Data, sizeof(uint32), (void *)&q_GnssConfig, sizeof(uint32));

    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC message");
      (void)os_IpcDelete(p_IpcMsg);
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC message");
  }
}


#ifdef FEATURE_WCDMA_GPS_CELL_ID_READING
/*******************************************************************************
*   tm_rrc_iface_cell_info_dch_report_callback
*
* Function description:
*   This function is registered as a callback function with RRC module and
*   called whenever RRC responds to cell id request sent by TM.
*
* Parameters:
*   Cell ID information:
*      rrcmeas_plmn_cell_info_struct_type cell_id_info
*   Status: (RRC_NEW_CELL_ID_AVAIL/RRC_NO_CELL_ID_AVAIL)
*      rrc_cell_id_status_type            status
*
* RETURN VALUE
*   None
*
 *******************************************************************************/

void tm_rrc_iface_cell_info_dch_report_callback(
   rrcmeas_plmn_cell_info_struct_type cell_id_info,
   rrc_cell_id_status_type            status)
{
  /*Pass the cell information from RRC to modules to TM CORE*/
  os_IpcMsgType                       *ipc_msg_ptr;
  tm_rrc_cell_info_dch_report_type    *ipc_data_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: tm_rrc_iface_cell_info_dch_report_callback", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(tm_rrc_cell_info_dch_report_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (tm_rrc_cell_info_dch_report_type *)ipc_msg_ptr->p_Data; /*lint !e826 */

    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_RRC_CELL_INFO_DCH_REPORT;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(tm_rrc_cell_info_dch_report_type);

    memscpy((void *)&(ipc_data_ptr->cell_id_info), sizeof(ipc_data_ptr->cell_id_info), (void *)&(cell_id_info), sizeof(cell_id_info));
    ipc_data_ptr->status = status;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */
}
#endif /*FEATURE_WCDMA_GPS_CELL_ID_READING  */

/*=============================================================================

FUNCTION
  tm_api_reset_location_service_done

DESCRIPTION
  Indication from MGP that location service reset id completed.

DEPENDENCIES
  None.

PARAMETERS
 status: Status of reset location service.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_reset_location_service_done(pdsm_pd_ext_reset_location_service_status_e_type status)
{
  /*Pass the reset done indication to TM CORE*/
  os_IpcMsgType                       *ipc_msg_ptr;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Report Reset Location Done", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(pdsm_pd_ext_reset_location_service_status_e_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_RESET_LOCATION_SERVICE_DONE;
    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(pdsm_pd_ext_reset_location_service_status_e_type);

    memscpy(ipc_msg_ptr->p_Data, sizeof(status), (void *)&status, sizeof(status));

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return;
    }
    return; /*lint !e506 */
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
    return;
  } /* End ipc_msg_ptr */
}

/*******************************************************************************
 * tm_api_ftcal_request
 *
 * Function description:
 *  Called by LM to update TM that FTCal is enabled
 *
 * Parameters:
 *
 *  b_Enable - TRUE to enable FTCal
 *
 * Return value:
 *
 *  void
 *
 *******************************************************************************/

void tm_api_ftcal_request(boolean b_Enable)
{
  os_IpcMsgType                  *ipc_msg_ptr;

  /* Create a message with no payload */
  ipc_msg_ptr = os_IpcCreate(0, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    if (b_Enable == TRUE)
    {
      /* Send Enable F-TCal message */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Enable F-TCal", 0, 0, 0);
      ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_FTCAL_ENABLE;
    }
    else
    {
      /* Send Disable F-TCal message */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Disable F-TCal", 0, 0, 0);
      ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_FTCAL_DISABLE;
    }

    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
    }
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
  } /* End ipc_msg_ptr */
}
/*=============================================================================

FUNCTION
  tm_api_send_engine_error_recovery_report

DESCRIPTION
  TM API to send engine error recovery report to QMI

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

=============================================================================*/

void tm_api_send_engine_error_recovery_report(gnss_engine_err_recovery_rpt *pErrReport)
{
  os_IpcMsgType   *ipc_msg_ptr  = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_send_engine_error_recovery_report", 0, 0, 0);

  ipc_msg_ptr = os_IpcCreate(sizeof(gnss_engine_err_recovery_rpt), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (ipc_msg_ptr != NULL)
  {
    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_ENGINE_ERROR_RECOVERY_REPORT;

    memscpy((void *)(ipc_msg_ptr->p_Data), sizeof(*pErrReport), (void *)(pErrReport), sizeof(*pErrReport));

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC mesage", 0, 0, 0);
  }
}


/*=============================================================================

FUNCTION
  tm_api_send_dpo_status_report

DESCRIPTION
  TM API to send engine error recovery report to SM .

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

=============================================================================*/

void tm_api_send_dpo_status_report(sm_DpoStatusReportStructType *p_DpoStatusReport)
{
  os_IpcMsgType   *ipc_msg_ptr  = NULL;
  uint32          msg_id        = 0;
  uint32          size          = 0;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_send_dpo_status_report", 0, 0, 0);

  msg_id = (uint32)TM_CORE_MSG_ID_ENGINE_DPO_STATUS_REPORT;
  size   = sizeof(sm_DpoStatusReportStructType);

  /* Send the MSG to SM LM thread */
  ipc_msg_ptr = sm_send_message(msg_id, size,
                                (uint32)THREAD_ID_SM_TM, (void *)p_DpoStatusReport);

  if (ipc_msg_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send error report IPC mesage", 0, 0, 0);
  }
}

/*
******************************************************************************
* tm_read_qmi_services_configuration
*
* Function description:
*
*  This function is called by QMI services to read which QMI services are
*  enabled. This function can be called only after SM has been initialized.
*
* Parameters:
*  output_cfg : QMI Configuration read from NV
*
* Return value:
*  NONE
*
* ******************************************************************************
*/
void tm_read_qmi_services_configuration(pdsm_QMIServiceConfig *output_cfg)
{
  if (NULL == output_cfg)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer passed!!", 0, 0, 0);
    return;
  }

  memscpy(output_cfg, sizeof(*output_cfg), &tm_core_info.config_info.qmi_cfg, sizeof(tm_core_info.config_info.qmi_cfg));
  return;
}


/*===========================================================================
  tm_loc_lte_ecid_recv_measurements

  Description:

     This function is used by LOC_LTE_ECID module to send the ECID measurements

  Parameters:
    Reason: stop reason

  Return value:
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean tm_loc_lte_ecid_recv_measurements(gnss_lpp_ecid_meas_info_type *p_otdoa_measurement_info)
{

  os_IpcMsgType   *ipc_msg_ptr  = NULL;
  gnss_lpp_ecid_meas_info_type *ipc_data_ptr;

  ipc_msg_ptr = os_IpcCreate(sizeof(gnss_lpp_ECID_ProvideLocationInformation),
                             IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_MGP_MC);

  if (ipc_msg_ptr != NULL)
  {
    ipc_data_ptr = (gnss_lpp_ecid_meas_info_type *)ipc_msg_ptr->p_Data;
    ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_GNSS_LPP_ECID_MEASUREMENTS;

    ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    ipc_msg_ptr->q_Size           = sizeof(gnss_lpp_ECID_ProvideLocationInformation);
    *ipc_data_ptr = *p_otdoa_measurement_info;

    if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message (ECID Measurements)", 0, 0, 0);
      (void)os_IpcDelete(ipc_msg_ptr);
      return FALSE;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: successfully sent an IPC message (ECID measurements)", 0, 0, 0);
      return TRUE;
    }
  } /* End ipc_msg_ptr */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to create IPC message (tm_api_xtm_nv_status_report)", 0, 0, 0);
    return FALSE;
  } /* End ipc_msg_ptr */
}

/*
******************************************************************************
* tm_retrieve_IMSI
*
* Function description:
*
*  This function is called by XTM to request TM core to retrieve the IMSI
*
* Parameters:
*  NONE
*
* Return value:
*  NONE
*
* ******************************************************************************
*/
void tm_retrieve_IMSI(uint8 sub, tm_RetrieveIMSICb p_RetrieveImsiInfoCb)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  tm_RetrieveIMSIType z_retrieveImsiInfo;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_retrieve_IMSI()", 0, 0, 0);

  z_retrieveImsiInfo.sub = sub;
  z_retrieveImsiInfo.pz_RetrieveIMSICb = p_RetrieveImsiInfoCb;

  p_ipcMsg = os_IpcCreate(sizeof(z_retrieveImsiInfo), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);
  if (p_ipcMsg)
  {
    p_ipcMsg->q_MsgId = TM_CORE_MSG_ID_RETRIEVE_IMSI;
    memscpy((void *)(p_ipcMsg->p_Data), sizeof(z_retrieveImsiInfo), (void *)(&z_retrieveImsiInfo), sizeof(z_retrieveImsiInfo));

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_core_handle_wifi_fix_response

  Description:

     This function is used by SM to provide Wifi fix response to TM
 Parameters:
   sm_FixInfoStructType: Pointer to Position Report
                                        Note that this may be non Wifi position
                                        if there was a better
                                        position available in ALE.
    wifi_ResultFlagsType: Wifi Results Flag
          The result flag indicates any error during Wifi pos fix calculation
          like no modem cache, no measurements, etc. It also indicates
          whether a better non Wifi fix was sent instead.
    sm_WifiPosReqSrcType: Wifi Position fix source (originiator)
  Return value:
   VOID
=============================================================================*/
void tm_handle_wifi_fix_response(const sm_FixInfoStructType *pz_PosFixRpt,
                                 wl_ResultFlagsType z_wifi_result,
                                 sm_ReqEnumType e_wifi_pos_src)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  tm_wifi_fix_response_type *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_wifi_fix_response()", 0, 0, 0);

  if (NULL == pz_PosFixRpt)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer in handling Wifi fix response", 0, 0, 0);
    return;
  }

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (tm_wifi_fix_response_type *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_WIFI_FIX_RESP;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(tm_wifi_fix_response_type);

    p_hdr_ptr->z_PosFixRpt = (*pz_PosFixRpt);
    p_hdr_ptr->z_wifi_result = z_wifi_result;
    p_hdr_ptr->e_wifi_pos_src = e_wifi_pos_src;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }

}

/*===========================================================================
  tm_handle_gnss_le_fix

  Description:

     This function is used by SM to provide GNSS LE fixes to TM
 Parameters:
   sm_FixReportStructType: Pointer to Position Report
  Return value:
   VOID
=============================================================================*/
void tm_handle_gnss_le_fix(const sm_FixInfoStructType *pz_PosFixRpt)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  sm_FixInfoStructType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_gnss_le_fix()", 0, 0, 0);

  if (NULL == pz_PosFixRpt)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer in handling GNSS LE Fix", 0, 0, 0);
    return;
  }

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (sm_FixInfoStructType *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_GNSS_LE_FIX;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(sm_FixInfoStructType);

    (*p_hdr_ptr) = (*pz_PosFixRpt);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }

}


/*===========================================================================

FUNCTION tm_api_get_gps_lock_status()

DESCRIPTION
  TM module can call this function to get current GPS lock status

  This is a synchronous API, will return immediately

DEPENDENCIES

RETURN VALUE
  Current GPS lock status

SIDE EFFECTS

===========================================================================*/
pdsm_gps_lock_e_type tm_api_get_gps_lock_status(void)
{
  return TM_CORE_CONFIG_DD(gps_lock);
}

/*===========================================================================
  tm_report_lppm_status

  Description:

     This function handles LPPM status report

 Parameters:

   pz_LppmStatus:  pointer to the LPPM status struct

  Return value:
    void

=============================================================================*/
void tm_report_lppm_status(const sm_LppmStatusStructType *pz_LppmStatus)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  sm_LppmStatusStructType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_report_lppm_status()", 0, 0, 0);

  if ((NULL == pz_LppmStatus) || (FALSE == pz_LppmStatus->v_IsQueryResponse))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Ignoring tm_report_lppm_status %p", pz_LppmStatus);
    return;
  }

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (sm_LppmStatusStructType *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_LPPM_STATUS;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(sm_LppmStatusStructType);

    *(p_hdr_ptr) = (*pz_LppmStatus);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_handle_ped_status

  Description:

     This function is used to handle MGP Ped Status

 Parameters:

   e_PedStatus:  Pedestrian status enum

  Return value:
    void

=============================================================================*/
void tm_handle_ped_status(gnss_MgpPedStatusEnumType e_PedStatus)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  gnss_MgpPedStatusEnumType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_ped_status()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (gnss_MgpPedStatusEnumType *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_PED_STATUS;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(gnss_MgpPedStatusEnumType);

    *(p_hdr_ptr) = e_PedStatus;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_handle_gnss_qual_ind

  Description:

     This function is used to handle GNSS Qual Indication

 Parameters:

   e_GnssQualityInd:  GNSS Quality indication enum

  Return value:
    void

=============================================================================*/
void tm_handle_gnss_qual_ind(const gnss_QualIndEnumType e_GnssQualityInd)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  gnss_QualIndEnumType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_gnss_qual_ind()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (gnss_QualIndEnumType *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_GNSS_QUAL_IND;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(gnss_QualIndEnumType);

    *(p_hdr_ptr) = e_GnssQualityInd;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_handle_ped_dev_ctx

  Description:

     This function is used  to handle pedestrian device context

 Parameters:

   e_PedDevCtx:   Pedestrian Device Context enum

  Return value:
    void

=============================================================================*/
void tm_handle_ped_dev_ctx(const gnss_PedDevCxtEnumType e_PedDevCtx)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  gnss_PedDevCxtEnumType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_ped_dev_ctx()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (gnss_PedDevCxtEnumType *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_PED_DEV_CTX_IND;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(gnss_PedDevCxtEnumType);

    *(p_hdr_ptr) = e_PedDevCtx;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_report_ped_align_availibility

  Description:

     This function is used to handle pedestrial alignement availability indication

 Parameters:

   v_PedAlignSensorAvailable: whether Pedestrian alignement is available or not

  Return value:
    void

=============================================================================*/
void tm_handle_ped_align_availibility(boolean v_PedAlignSensorAvailable)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  boolean *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_ped_align_availibility()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (boolean *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_PED_ALIGN_AVAIL_IND;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(boolean);

    *(p_hdr_ptr) = v_PedAlignSensorAvailable;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_handle_ped_sensor_assist_availibility

  Description:

     This function is used to handle Pedestrian Sensor Assistance
     availibility indication

 Parameters:

   v_PedSensorAssistAvailable: whether Pedestrian Sensor Assistance is available or not

  Return value:
    void

=============================================================================*/
void tm_handle_ped_sensor_assist_availibility(boolean v_PedSensorAssistAvailable)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  boolean *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_ped_sensor_assist_availibility()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (boolean *)p_ipcMsg->p_Data; /*lint !e826*/

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_PED_SENSOR_ASSIST_AVAIL_IND;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(boolean);

    *(p_hdr_ptr) = v_PedSensorAssistAvailable;

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_api_report_mgp_state_to_techsel

  Description:

     This function is used to send current MGP state to TechSel

 Parameters:

   p_rcvrState: Reciever State change info pointer

  Return value:
    void

=============================================================================*/
void tm_api_report_mgp_state_to_techsel(sm_RcvrStateChangeStructType *p_rcvrState)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  sm_RcvrStateChangeStructType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_report_mgp_state_to_techsel()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (sm_RcvrStateChangeStructType *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_MGP_STATE_CHANGE_IND_TECHSEL;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(sm_RcvrStateChangeStructType);

    *(p_hdr_ptr) = *(p_rcvrState);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

/*=============================================================================

FUNCTION
  tm_api_leap_second_report

DESCRIPTION
  Leap Second information from ME to SM

DEPENDENCIES
  None.

PARAMETERS
  mgp_LeapSecondInfoStructType - Pointer to leap second info

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_leap_second_report(const mgp_LeapSecondInfoStructType *pz_LeapSecInfo)
{
  os_IpcMsgType *p_IpcMsg = NULL;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Leap Second Info report from ME");

  if (NULL == pz_LeapSecInfo)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_api_leap_second_report: Null Leap Second Info");
    return;
  }

  p_IpcMsg = os_IpcCreate(sizeof(mgp_LeapSecondInfoStructType), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL != p_IpcMsg)
  {
    p_IpcMsg->q_MsgId        = (uint32)TM_CORE_MSG_ID_LEAP_SEC_INFO;

    memscpy(p_IpcMsg->p_Data, sizeof(*pz_LeapSecInfo), (void *)pz_LeapSecInfo, sizeof(*pz_LeapSecInfo));

    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage");
      (void)os_IpcDelete(p_IpcMsg);
      return;
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC message");
  }
}

/*===========================================================================
FUNCTION tm_api_notify_prtl_nv_op

DESCRIPTION
  Queue an IPC message to handle the next NV read.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_api_notify_prtl_nv_op(uint32 u_nvGroup)
{

  os_IpcMsgType *p_IpcMsg = NULL;
  prtl_ruim_nv_read_group *p_hdr_ptr = NULL;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: NV Operation Complete from TM PDAPI IFACE");

  p_IpcMsg = os_IpcCreate(sizeof(prtl_ruim_nv_read_group), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL != p_IpcMsg)
  {
    p_hdr_ptr = (prtl_ruim_nv_read_group *)p_IpcMsg->p_Data;

    p_IpcMsg->q_MsgId        = (uint32)TM_CORE_MSG_ID_NV_OP_COMPLETE;
    p_IpcMsg->q_DestThreadId = THREAD_ID_SM_TM;
    p_IpcMsg->q_Size         = sizeof(prtl_ruim_nv_read_group);

    *(p_hdr_ptr) = u_nvGroup;

    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to send IPC mesage");
      (void)os_IpcDelete(p_IpcMsg);
      return FALSE;
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC message");
    return FALSE;
  }
  return TRUE;
}
/*===========================================================================

FUNCTION tm_api_PfaTestPrescDwellResults()

DESCRIPTION
  Pass the prescribed dwell results to TM DIAG
 
DEPENDENCIES

RETURN VALUE

  
SIDE EFFECTS

===========================================================================*/
void tm_api_PfaTestPrescDwellResults(const sm_PfaTestPresDwellResultsType
                                     *p_PrescDwellResults)
{
  os_IpcMsgType *p_IpcMsg;

  if (p_PrescDwellResults != NULL)
  {
    p_IpcMsg = os_IpcCreate(sizeof(sm_PfaTestPresDwellResultsType),
                            IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

    if (p_IpcMsg != NULL)
    {
      p_IpcMsg->q_MsgId          = (uint32)TM_CORE_MGP_PRESC_DWELL_RESULTS;

      /* Copy all the data over */
      memscpy(p_IpcMsg->p_Data, sizeof(sm_PfaTestPresDwellResultsType),
              (void *)p_PrescDwellResults, sizeof(*p_PrescDwellResults));

      if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
            "tm_api_PfaTestPrescDwellResults: IPC Send failed");
        (void)os_IpcDelete(p_IpcMsg);
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "tm_api_PfaTestPrescDwellResults: IPC Create failed");
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "tm_api_PfaTestPrescDwellResults: Input parameter check failed");
  }
}


/*===========================================================================

FUNCTION tm_fusion_csm_handle_control_req

DESCRIPTION
  This function handles Crowd Source Manager Control Request

DEPENDENCIES

RETURN VALUE Boolean

SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_control_req(fusion_csm_ControlReqType *p_csmControlReq)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  fusion_csm_ControlReqType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_fusion_csm_handle_control_req()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (fusion_csm_ControlReqType *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_FUSION_CSM_CONTROL_REQ;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(fusion_csm_ControlReqType);

    *(p_hdr_ptr) = *(p_csmControlReq);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_fusion_csm_handle_read_cs_data_req

DESCRIPTION
  This function handles request to read crowd sourced data

DEPENDENCIES

RETURN VALUE Boolean

SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_read_cs_data_req(qmiLocCrowdSourceManagerReadDataReqMsgT_v02 *const pz_Req)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  qmiLocCrowdSourceManagerReadDataReqMsgT_v02 *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_fusion_csm_handle_read_cs_data_req()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (qmiLocCrowdSourceManagerReadDataReqMsgT_v02 *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_FUSION_CSM_READ_CS_DATA_REQ;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(qmiLocCrowdSourceManagerReadDataReqMsgT_v02);

    *(p_hdr_ptr) = *(pz_Req);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}




/*===========================================================================
  tm_api_report_wifi_scan_to_techsel

  Description:

     This function is used to send Wifi Scan to TechSel

 Parameters:

   p_response: Pointer to Lowi Response

  Return value:
    void

=============================================================================*/
void tm_api_report_wifi_scan_to_techsel(t_wiper_ap_set_struct_type *p_wifiScan)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  t_wiper_ap_set_struct_type *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_report_lowi_respose_to_techsel()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (t_wiper_ap_set_struct_type *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_WIFI_SCAN_TECHSEL;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(t_wiper_ap_set_struct_type);

    *(p_hdr_ptr) = *(p_wifiScan);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
  tm_api_report_lowi_error_to_techsel

  Description:

     This function is used to send Lowi Error to TechSel

 Parameters:

  void

  Return value:
    void

=============================================================================*/
void tm_api_report_lowi_error_to_techsel(void)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  boolean *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_report_lowi_respose_to_techsel()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipcMsg)
  {
    p_hdr_ptr = (boolean *)p_ipcMsg->p_Data;
    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_LOWI_ERROR_TECHSEL;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(boolean);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}


/*============================================================================
FUNCTION
    send_ipc_msg_to_sm_tm

DESCRIPTION
    Send an IPC Msg to TM task

PARAMETERS
   p_Data - pointer to the input data which will be put into the
                       message payload
   q_Size - size of data in bytes
   e_Msg  - Messge Id

DEPENDENCIES
    None

RETURN VALUE
    TRUE - If the data is successfully sent to TM
    FALSE - Otherwise

SIDE EFFECTS
    None

===========================================================================*/
static boolean send_ipc_msg_to_sm_tm(const void *p_Data, uint32 q_Size,
                                     uint32 e_Msg)
{
  os_IpcMsgType *p_Msg;

  if ((q_Size > 0) && (NULL == p_Data))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot generate TM msg with data ptr uninitialized", 0, 0, 0);
    return FALSE;
  }

  p_Msg = os_IpcCreate(q_Size, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL == p_Msg)
  {
    return FALSE;
  }

  p_Msg->q_MsgId = (uint32)e_Msg;
  p_Msg->q_DestThreadId = (uint32)THREAD_ID_SM_TM;
  p_Msg->q_Size = q_Size;

  if (q_Size > 0)
  {
    memscpy(p_Msg->p_Data, q_Size, p_Data, q_Size);
  }

  if (!os_IpcSend(p_Msg, (uint32)THREAD_ID_SM_TM))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
    (void)os_IpcDelete(p_Msg);
    return FALSE;
  }

  return TRUE;
}




/*=============================================================================

  FUNCTION      tm_fusion_csm_rcvd_send_ack

  DESCRIPTION
    This function is called by GDT as an ACK for a received record.

  PARAMETERS
    z_Ack  - The ack received
    (input)

  DEPENDENCIES  None

  PARAMETERS

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_rcvd_send_ack(gdt_SendAckArgumentType z_Ack)
{
  if (NULL == z_Ack.pz_AckResponse)
  {
    return FALSE;
  }

  return send_ipc_msg_to_sm_tm(z_Ack.pz_AckResponse,
                               sizeof(*(z_Ack.pz_AckResponse)),
                               TM_CORE_CSM_UL_MSG_ID_GDT_SEND_ACK);
}

/*=============================================================================

  FUNCTION      tm_fusion_csm_proc_send_end_response

  DESCRIPTION
    This API is called by GDT to confirm the session end.

   PARAMETERS
    z_EndStatus  - Indicates whether the session succeeded or not
    (input)

  DEPENDENCIES  None

  PARAMETERS

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_end_response(gdt_EndArgumentType z_EndStatus)
{
  if (NULL == z_EndStatus.pz_EndResponse)
  {
    return FALSE;
  }

  return send_ipc_msg_to_sm_tm(z_EndStatus.pz_EndResponse,
                               sizeof(*(z_EndStatus.pz_EndResponse)),
                               TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_END_RESPONSE);
}


/*=============================================================================

  FUNCTION     tm_fusion_csm_proc_send_begin_response

  DESCRIPTION
    This function is called by GDT to send a upload session request status.

  DEPENDENCIES  None

  PARAMETERS
    z_Response - The permission status for the requested upload session
    (input)

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_begin_response(gdt_StartArgumentType z_Response)
{
  if (NULL == z_Response.pz_StartResponse)
  {
    return FALSE;
  }

  return send_ipc_msg_to_sm_tm(z_Response.pz_StartResponse,
                               sizeof(*(z_Response.pz_StartResponse)),
                               TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_BEGIN_RESPONSE);
}

/*=============================================================================

  FUNCTION      tm_fusion_csm_proc_send_open_response

  DESCRIPTION
    This function is called by GDT to send an open request status.

  DEPENDENCIES  None

  PARAMETERS
    z_Response - The status of the requested open request
    (input)

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_open_response(gdt_OpenArgumentType z_Response)
{
  if (NULL == z_Response.pz_OpenResponse)
  {
    return FALSE;
  }

  return send_ipc_msg_to_sm_tm(z_Response.pz_OpenResponse,
                               sizeof(*(z_Response.pz_OpenResponse)),
                               TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_OPEN_RESPONSE);
}


/*=============================================================================

  FUNCTION      tm_fusion_csm_proc_send_close_response

  DESCRIPTION
    This function is called by GDT to send a close request status.

  DEPENDENCIES  None

  PARAMETERS
    z_Response - The status for the requested close request
    (input)

  RETURN VALUE
    TRUE  - If the IPC is sent to TM
    FALSE - Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tm_fusion_csm_proc_send_close_response(gdt_CloseArgumentType z_Response)
{
  if (NULL == z_Response.pz_CloseResponse)
  {
    return FALSE;
  }
  return send_ipc_msg_to_sm_tm(z_Response.pz_CloseResponse,
                               sizeof(*(z_Response.pz_CloseResponse)),
                               TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_CLOSE_RESPONSE);
}


/*===========================================================================

FUNCTION tm_fusion_csm_handle_set_external_power_config_req

DESCRIPTION
  This function handles external power configuration information from HLOS to Fusion CSM

DEPENDENCIES

RETURN VALUE Boolean

SIDE EFFECTS

===========================================================================*/
boolean tm_fusion_csm_handle_set_external_power_config_req(const qmiLocSetExternalPowerConfigReqMsgT_v02 *pz_Req)
{
  if (NULL == pz_Req)
  {
    return FALSE;
  }

  return send_ipc_msg_to_sm_tm(pz_Req,
                               sizeof(*(pz_Req)),
                               TM_CORE_CSM_SET_EXT_POWER_CONFIG_REQ);
}

/*=============================================================================

FUNCTION
  tm_api_report_var_len_debug_nmea

DESCRIPTION
  Accepts formatted proprietary NMEA string for debug purpose if the NMEA debug
  string is enabled. NMEA string is sent as IPC to the TM task by this API, for
  forwarding to HLOS for debugging purpose.

DEPENDENCIES
  None.

PARAMETERS
  pDebugNMEA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_report_var_len_debug_nmea(sm_DebugNMEAVarLen const *const cpz_DebugNMEA)
{
  os_IpcMsgType   *ipc_msg_ptr  = NULL;

  /* Send IPC only if NMEA debug string is enabled */
  if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_DEBUG)
  {
    sm_DebugNMEAVarLen z_DebugNMEAVarLen = { 0 };

    if ((NULL == cpz_DebugNMEA) || (0 == cpz_DebugNMEA->q_Length) ||
        (NULL == cpz_DebugNMEA->pu_NmeaBuff) || (cpz_DebugNMEA->q_Length > C_DBUG_NMEA_PQWP7_BUFER_SIZE))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "invalid param var_len_debug_nmea %p", cpz_DebugNMEA);
      return;
    }

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Var len %lu NMEA %p", cpz_DebugNMEA->q_Length,
          cpz_DebugNMEA->pu_NmeaBuff);

    /* Allow once when the very first variable length NMEA debug string is reported. Memory will be
       released in tm_api_set_nmea_debug() when NMEA debug sentence type is disabled. */
    if (NULL == pu_NmeaVarLenBuff)
    {
      pu_NmeaVarLenBuff = os_MemAlloc(C_DBUG_NMEA_PQWP7_BUFER_SIZE, OS_MEM_SCOPE_TASK);
      if (NULL == pu_NmeaVarLenBuff)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM var len nmea mem alloc failed");
        return;
      }
    }
    /* Copy NMEA string */
    memscpy(pu_NmeaVarLenBuff, C_DBUG_NMEA_PQWP7_BUFER_SIZE, cpz_DebugNMEA->pu_NmeaBuff, cpz_DebugNMEA->q_Length);
    z_DebugNMEAVarLen.nmea_type = PDAPI_NMEA_VAR_LEN_DEBUG;
    z_DebugNMEAVarLen.q_Length = cpz_DebugNMEA->q_Length;
    z_DebugNMEAVarLen.pu_NmeaBuff = pu_NmeaVarLenBuff;

    /* Send the MSG to SM TM thread */
    ipc_msg_ptr = sm_send_message(TM_CORE_MSG_ID_NMEA_VAR_LEN_DEBUG_INFO, sizeof(z_DebugNMEAVarLen),
                                  (uint32)THREAD_ID_SM_TM, (void *)&z_DebugNMEAVarLen);

    if (ipc_msg_ptr != NULL)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SM_API: IPC msg send to SM_TM, id = %d", ipc_msg_ptr->q_MsgId);
    }
  }
}

/*=============================================================================

FUNCTION
  tm_api_set_nmea_debug

DESCRIPTION
  Receive the NMEA string type configuration when they are modified through QMI.
  If the NMEA debug string is disabled, the large internal heap memory can be released,
  which is used only to store the NMEA variable length debug string.

DEPENDENCIES
  None.

PARAMETERS
  cq_NmeaType - Current NMEA sentence type config status

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_set_nmea_debug(pdsm_pa_nmea_type const cq_NmeaType)
{
  /* Release the heap memory when NMEA debug type is disabled */
  if ((0 == (cq_NmeaType & PDAPI_NMEA_DEBUG)) && (pu_NmeaVarLenBuff != NULL))
  {
    os_MemFree((void **)(&pu_NmeaVarLenBuff));
  }
}


/*===========================================================================
  tm_api_report_wifi_scan_to_techsel

  Description:

     This function is used to send Wifi Scan to TechSel

 Parameters:

   p_response: Pointer to Lowi Response

  Return value:
    void

=============================================================================*/
void tm_api_report_ranging_wifi_scan_to_techsel(pdsm_RangingScanRespType *p_wifiScan)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  pdsm_RangingScanRespType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_api_report_ranging_wifi_scan_to_techsel()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL == p_ipcMsg)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to allocate memory for IPC message", 0, 0, 0);
  }
  if (p_ipcMsg)
  {
    p_hdr_ptr = (pdsm_RangingScanRespType *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_WIFI_RANGING_SCAN_TECHSEL;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(pdsm_RangingScanRespType);

    *(p_hdr_ptr) = *(p_wifiScan);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*=============================================================================

FUNCTION
  tm_handle_sensor_le_status

DESCRIPTION
  This function is to report Sensor LE State Change

DEPENDENCIES
  None.

PARAMETERS
  p_SensorLEStatus -Pointer to Sensor LE Status struct

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_handle_sensor_le_status(const sm_SensorLEStatusStructType *const p_SensorLEStatus)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  sm_SensorLEStatusStructType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_sensor_le_status()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL == p_ipcMsg)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to allocate memory for IPC message", 0, 0, 0);
  }
  if (p_ipcMsg)
  {
    p_hdr_ptr = (sm_SensorLEStatusStructType *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_SENSOR_LE_STATUS;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(sm_SensorLEStatusStructType);

    *(p_hdr_ptr) = *(p_SensorLEStatus);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}


/*=============================================================================

FUNCTION
  tm_handle_sensor_le_rel_disp

DESCRIPTION
  This function is to report Sensor Relative Displacement from Sensor LE

DEPENDENCIES
  None.

PARAMETERS
  p_RelDisp -Pointer to Relative Displacement Struct.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_handle_sensor_le_rel_disp(const sm_SensorLERelDispStructType *const p_RelDisp)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  sm_SensorLERelDispStructType *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "tm_handle_sensor_le_rel_disp()", 0, 0, 0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL == p_ipcMsg)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to allocate memory for IPC message", 0, 0, 0);
  }
  if (p_ipcMsg)
  {
    p_hdr_ptr = (sm_SensorLERelDispStructType *)p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_SENSOR_LE_REL_DISP;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(sm_SensorLERelDispStructType);

    *(p_hdr_ptr) = *(p_RelDisp);

    if (!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_API: Failed to send IPC message", 0, 0, 0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*=============================================================================

FUNCTION
  tm_api_disable_APMP_at_GERA

DESCRIPTION
  APMP disable from MC to TM

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_disable_APMP_at_GERA(void)
{
#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to create IPC message");
  }
}
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*=============================================================================

FUNCTION
  tm_api_persistent_sv_mask_report

DESCRIPTION
  Response from MGP regarding GNSS Persistent SV Mask request

DEPENDENCIES
  None.

PARAMETERS
  p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
  TRUE  : IPC message sent successfully
  FALSE : IPC message sent failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean tm_api_persistent_sv_mask_report(const gnss_SvIdMaskStructType *p_SvMask)
{
  os_IpcMsgType *p_IpcMsg;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Report Persistent SV Mask");

  if (NULL == p_SvMask)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "tm_api_persistent_sv_mask_report: Null SV Mask");
    return FALSE;
  }

  p_IpcMsg = os_IpcCreate(sizeof(gnss_SvIdMaskStructType), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL != p_IpcMsg)
  {
    p_IpcMsg->q_MsgId        = (uint32)TM_CORE_MSG_ID_PERSISTENT_SV_MASK_REPORT;
    p_IpcMsg->q_DestThreadId = (uint32)THREAD_ID_SM_TM;
    p_IpcMsg->q_Size         = sizeof(gnss_SvIdMaskStructType);

    memscpy(p_IpcMsg->p_Data, sizeof(*p_SvMask), (void *)p_SvMask, sizeof(*p_SvMask));

    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "Failed to send IPC mesage");

      (void)os_IpcDelete(p_IpcMsg);
      return FALSE;
    }
    return TRUE;
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "Failed to create IPC message");
    return FALSE;
  }
}

/*=============================================================================

FUNCTION
  tm_api_persistent_sv_mask_to_filter

DESCRIPTION
  Unsolicited GNSS Persistent SV Mask from MGP

DEPENDENCIES
  None.

PARAMETERS
  p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
  TRUE  : IPC message sent successfully
  FALSE : IPC message sent failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean tm_api_persistent_sv_mask_to_filter(const gnss_SvIdMaskStructType *p_SvMask)
{
  os_IpcMsgType *p_IpcMsg;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_API: Report Persistent SV Mask to Filter");

  if (NULL == p_SvMask)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "tm_api_persistent_sv_mask_to_filter: Null SV Mask");
    return FALSE;
  }

  p_IpcMsg = os_IpcCreate(sizeof(gnss_SvIdMaskStructType), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL != p_IpcMsg)
  {
    p_IpcMsg->q_MsgId = (uint32)TM_CORE_MSG_ID_PERSISTENT_SV_MASK_TO_FILTER;
    p_IpcMsg->q_DestThreadId = (uint32)THREAD_ID_SM_TM;
    p_IpcMsg->q_Size = sizeof(gnss_SvIdMaskStructType);

    memscpy(p_IpcMsg->p_Data, sizeof(*p_SvMask), (void *)p_SvMask, sizeof(*p_SvMask));

    if (!os_IpcSend(p_IpcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "Failed to send IPC mesage");

      (void)os_IpcDelete(p_IpcMsg);
      return FALSE;
    }
    return TRUE;
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "Failed to create IPC message");
    return FALSE;
  }
}

/*=============================================================================
FUNCTION
tm_ReportConstellationCtrlCfg

DESCRIPTION
This function is used to send GNSS Constellation Control Configuration to SM.

DEPENDENCIES
None.

PARAMETERS
pz_MgpConfig - GNSS Constellation Control Configuration in MGP

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void tm_ReportConstellationCtrlCfg(const gnss_ConstellationConfigT *pz_MgpConfig)
{
  os_IpcMsgType   *ipc_msg_ptr = NULL;
  uint32          msg_id = 0;
  uint32          size = 0;
  if (NULL == pz_MgpConfig)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "pz_ConstellCfgFromMgp is NULL");
    return;
  }

  if (pz_MgpConfig->e_CfgCmd != GNSS_CONT_CFG_GET)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "GET API: Invalid Cmd %d.", pz_MgpConfig->e_CfgCmd);
    return;
  }

  MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_LOW,
        "tm_ReportConstellationCtrlCfg: GPS %d, GLO %d, BDS %d, QZSS %d, GAL %d",
        pz_MgpConfig->z_GnssStatus.gps_status,
        pz_MgpConfig->z_GnssStatus.glo_status, pz_MgpConfig->z_GnssStatus.bds_status,
        pz_MgpConfig->z_GnssStatus.qzss_status, pz_MgpConfig->z_GnssStatus.gal_status);

  msg_id = (uint32)TM_CORE_MSG_ID_CONSTELLATION_CONTROL;
  size = sizeof(*pz_MgpConfig);

  // Send the MSG to SM TM thread
  ipc_msg_ptr = sm_send_message(msg_id, size, (uint32)THREAD_ID_SM_TM, (void *)pz_MgpConfig);

  if (ipc_msg_ptr != NULL)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SM_API: IPC msg send to SM, id = %d", ipc_msg_ptr->q_MsgId);
  }
}

/*=============================================================================

FUNCTION
  tm_api_report_rc_on_threshold_period

DESCRIPTION
  This function is to report RC ON Threshold period to TM

DEPENDENCIES
  None.

PARAMETERS
  q_rcOnThresholdMsec -RC ON Threshild period in MilliSeconds

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_report_rc_on_threshold_period(uint32 q_rcOnThresholdMsec)
{
  os_IpcMsgType  *p_ipcMsg = NULL;
  uint32 *p_hdr_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"tm_api_report_rc_on_threshold_period()", 0,0,0);

  p_ipcMsg = os_IpcCreate(sizeof(*p_hdr_ptr), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (NULL == p_ipcMsg)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_API: Failed to allocate memory for IPC message",0,0,0);
  }
  if (p_ipcMsg)
  {
    p_hdr_ptr = (uint32*) p_ipcMsg->p_Data;

    p_ipcMsg->q_MsgId          = (uint32)TM_CORE_MSG_ID_RC_ON_THRESHOLD_INFO;
    p_ipcMsg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
    p_ipcMsg->q_Size           = sizeof(uint32);

    *(p_hdr_ptr) = q_rcOnThresholdMsec;

    if(!os_IpcSend(p_ipcMsg, (uint32)THREAD_ID_SM_TM))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_API: Failed to send IPC message",0,0,0);
      (void)os_IpcDelete(p_ipcMsg);
    }
  }
}

/*===========================================================================
FUNCTION tm_pdapi_send_otdoa_event

DESCRIPTION
  This function sends a PDAPI event when OTDOA session starts/stops
  emergency positioning

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_pdapi_send_otdoa_event(sm_loc_lte_otdoa_pdapi_event_type sess_event_type)
{
  tm_core_send_otdoa_ext_status_event(sess_event_type);
} 

/*===========================================================================
FUNCTION tm_api_allow_navic_assist_download

DESCRIPTION
  This function checks if the navic assist download request should be allowed to
  be sent to TM task.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_api_allow_navic_assist_download()
{
  return tm_xtra3_navic_assist_allow_download();
}

/*===========================================================================
FUNCTION tm_api_cancel_navic_assist_download

DESCRIPTION
  This function processes a cancel req for Navic assist download 

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None

===========================================================================*/
void tm_api_cancel_navic_assist_download()
{
  tm_xtra3_navic_assist_cancel_download();
}

/*=============================================================================

FUNCTION
  tm_api_req_navic_assist_download

DESCRIPTION
  This function sends the Navic assist download req IPC to TM task.

DEPENDENCIES
  None.

PARAMETERS
  pz_xtraAssistReq -Navic assist download req type

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void tm_api_req_navic_assist_download(const sm_ReqXtraStructType *pz_xtraAssistReq)
{
  os_IpcMsgType   *ipc_msg_ptr = NULL;
  uint32          msg_id = 0;
  uint32          size = 0;

  if (NULL == pz_xtraAssistReq)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "pz_ConstellCfgFromMgp is NULL");
    return;
  }

  if (pz_xtraAssistReq->e_XtraType != SM_REQ_TYPE_GET_NAVIC_EPH_ASSIST)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "GET API: Invalid Cmd %d.", pz_xtraAssistReq->e_XtraType);
    return;
  }
  
  msg_id = (uint32)TM_CORE_MSG_ID_XTRA_NAVIC_ASSIST_DOWNLOAD_REQ;
  size = sizeof(*pz_xtraAssistReq);

  // Send the MSG to SM TM thread
  ipc_msg_ptr = sm_send_message(msg_id, size, (uint32)THREAD_ID_SM_TM, (void *)pz_xtraAssistReq);

  if (ipc_msg_ptr != NULL)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SM_API: IPC msg send to SM, id = %d", ipc_msg_ptr->q_MsgId);
  }
}



