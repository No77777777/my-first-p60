/******************************************************************************
  @file:  loc_pa.h
  @brief: Location Middleware PA module header

  DESCRIPTION
      This module configures/retrieves the NV setting of location
  engine on the modem, and processes EFS data.

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2018 Qualcomm Technologies, Inc. 
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
05/19/16   yh       LocMW refactoring
10/06/15   yh       QZSS
01/01/09   wc/etc   Initial version and development

$Id: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_pa.h#1 $
$DateTime: 2019/10/24 01:30:36 $
======================================================================*/

#ifndef LOC_PA_H

#define LOC_PA_H

#include "loc_api_2.h"
#include "aries_os_api.h"

/* Loc PA module data */
typedef struct
{
  boolean                v_PaCmdInProgress;
  uint32                 q_PaCmdCounterIn;
  uint32                 q_PaCmdCounterOut;
  loc_client_handle_type l_ClientHandle;    /* Loc API client handle */
  uint32                 q_QmiLocMsgId;     /* Corresponding QMI_LOC Id for PDSM Parameter */
  pdsm_pa_cmd_e_type     e_PaCmdType;       /* PDSM Command: GET or SET */
  pdsm_pa_e_type         e_ParamType;       /* PDSM Parameter to GET/SET */
  os_TimerMsgType*       pz_ProcessTimer;

  pdsm_pa_mo_method_e_type  e_PaMoMethod; // 0: CP, 1: UP
  int                       e_EfsFileOperation;
} locPa_ModuleDataType;

/* IPC data sent from PDAPI to LocMW for Send QMI Indication */
typedef struct
{
  loc_client_handle_type l_ClientHandle;
  pdsm_pa_cmd_e_type     e_PaCmdType;    /* PDSM Command: GET or SET */
  pdsm_pa_info_s_type    z_ParamInfo;    /* PDSM Parameter Data */
  pdsm_pa_cmd_err_e_type e_ParamStatus;
} locPa_PdapiIndMsg;

/*---------------------------------------------------------------------------
@brief
  Function to initialize location PA module

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locPa_Init(void);

/*---------------------------------------------------------------------------
@brief
  Function to process Timer callback.

  When the timer fires, the location engine has not received the PA callback.
  Notify client of the failed status and reset the state.
---------------------------------------------------------------------------*/
void locPa_ProcessTimerCb(void);

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC request from client that belongs to PA module
  (related to PDAPI Get/Set_Parameters)

@param[in] l_ClientHandle  : Loc API client handle
@param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
@param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
@param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length
@param[in/out] pe_QmiErr   : QMI_LOC Response error value

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locPa_ProcessQmiRequest
(
  loc_client_handle_type l_ClientHandle,
  uint32                 q_QmiLocMsgId,
  const void*            p_QmiLocMsgData,
  uint32                 q_QmiLocMsgLen,
  qmi_error_type_v01     *pe_QmiErr
);

/*---------------------------------------------------------------------------
@brief
  Function to send QMI_LOC indication to client for previous QMI_LOC request
  related to PDAPI Get/Set_Parameters

@param[in] p_IpcMsg : IPC Message containing the QMI_LOC indication data

@retval    TRUE    if IPC posted to LocMW successfully
@retval    FALSE   if IPC posted to LocMW unsuccessfully
---------------------------------------------------------------------------*/
boolean locPa_SendQmiIndication
(
  const os_IpcMsgType* p_IpcMsg
);

/*---------------------------------------------------------------------------
@brief
  Function to reset the PA module. This is called when in progress pa cmd is
  done. This is called when PA cmd error fails, or PA event arrives.

  NOTE: This function is always called within LocMW critical section.
---------------------------------------------------------------------------*/
void locPa_ResetModule(void);

#endif // LOC_PA_H
