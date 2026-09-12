/******************************************************************************
  @file: loc_mgp_iface.h
  @brief:   This module handles interface between location middleware and MGP.
  

  DESCRIPTION
   Qualcomm Location API for MGP

  INITIALIZATION AND SEQUENCING REQUIREMENTS
   N/A

  -----------------------------------------------------------------------------
  Copyright (c) 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_mgp_iface.h#1 $
$DateTime: 2019/10/24 01:30:36 $
******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
08/31/19             Initial version
======================================================================*/

#ifndef LOC_MGP_H
#define LOC_MGP_H

#include "loc_client.h"
#include "gnss_common.h"
#include "aries_os_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*=============================================================================
FUNCTION
locMgp_ReportEphemerisIndProxy

DESCRIPTION
This function sends ephemeris report to SM/LocMW
Called in the context of MGP/PE task and sends an
IPC to LocMW

DEPENDENCIES
None.

PARAMETERS
Pointer to the ephemeris information

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void locMgp_ReportEphemerisIndProxy(const gnss_EphemerisDataType *pz_GnssEphemerisData);

/*===========================================================================
FUNCTION locMgp_HandleEphemerisReport

DESCRIPTION
IPC handler for LM_MIDDLEWARE_MSG_ID_SEND_GET_FDCL2_BS_OBS_DATA_IND.
This function handles the request made by MGP and sends indication
messages for reporting GNSS ephemeris information to client.

PARAMETERS
Pointer to the received IPC message containing ephemeris information

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void locMgp_HandleEphemerisReport(const os_IpcMsgType *cpz_IpcMsg);

/*=============================================================================
FUNCTION
locMw_ReportEvent

DESCRIPTION
This function sends report of GNSS engine events to LocMW.
Runs in the context of calling task and sends an IPC to LocMW.

DEPENDENCIES
None.

PARAMETERS
Pointer to Event Data Payload

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void locMw_ReportEvent(gnss_QmiEventReportT* pz_EventData);

/*=============================================================================
FUNCTION
loc_HandleEventReport

DESCRIPTION
This function handles the GNSS engine event notification and sends 
corresponding QMI indication to client

DEPENDENCIES
None.

PARAMETERS
IPC message containing bitmask of events

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void loc_HandleEventReport(const os_IpcMsgType *cpz_IpcMsg);

#ifdef __cplusplus
}
#endif

#endif /* LOC_GTP_H */
