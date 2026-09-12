/******************************************************************************
@file:  loc_conn.h
@brief: Location Middleware Connection Module

DESCRIPTION
This module defines the loc API routines for a client that connects to 
loc-mw

INITIALIZATION AND SEQUENCING REQUIREMENTS

-----------------------------------------------------------------------------
  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc. 
-----------------------------------------------------------------------------
******************************************************************************/

/*=====================================================================
                      EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
03/31/17   yh       loc_conn clean up
04/27/11   spn      Updated for Loc-Api 2.0

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_conn.h#1 $
$DateTime: 2019/10/24 01:30:36 $
$Author: pwbldsvc $
======================================================================*/


#ifndef LOC_CONN_H
#define LOC_CONN_H

#include "loc_api_2.h"
#include "pdsm_atl.h"
//#include "aries_os_api.h"

typedef struct
{
   // Information being cached from PDSM_ATL request 
   pdsm_atl_session_handle_type  l_SessionHandle;
   pdsm_atl_open_params_type     z_OpenParam;
} locConn_ModuleDataType;

/*---------------------------------------------------------------------------
@brief
  Function to initialize location connection module
  This function will be called when LocMW initialize all modules.

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locConn_Init (void);

/*---------------------------------------------------------------------------
@brief
  Function to deinitialize location connection module

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locConn_DeInit (void);

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC request from client that belongs to connection module

@param[in] l_ClientHandle  : Loc API client handle
@param[in] q_QmiLocMsgId   : QMI_LOC Request Message ID
@param[in] p_QmiLocMsgData : QMI_LOC Request Message Data
@param[in] q_QmiLocMsgLen  : QMI_LOC Request Message Data Length

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locConn_ProcessQmiRequest
(
  loc_client_handle_type l_ClientHandle,
  uint32                 q_QmiLocMsgId,
  const void*            p_QmiLocMsgData,
  uint32                 q_QmiLocMsgLen
);

#endif // LOC_CONN_H
