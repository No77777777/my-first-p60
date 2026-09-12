
/*============================================================================
  @file:       loc_qmi_shim.c

  OVERVIEW:    This module is adapts the loc-api to QCSI framework. This
               file glues loc-api enabling it to be a provider of the 
               QMI-Loc Service. It has a few sections, any changes to the 
               file should be done in the appropriate sections to maintain 
               readability of the file. 

               UTILITY FUNCTIONS
                 All utility functions that are needed shall go here
                   
               MESSAGE CONVERTORS QMI --> Loc-Api
                 All functions that are used to convert QMI to Loc-Api data
                 structures shall be added to this section.
                   
               MESSAGE CONVERTORS Loc-Api --> QMI
                 All functions that are used to convert Loc-Api to QMI data
                 structures shall be added to this section.

               LOC-API CALLBACKS
                 All functions that are registered with the Loc-API shall be 
                 defined in this section
 
               MESSAGE HANDLERS 
                 All the messages handlers should be added to this section. 
                
               QCSI CALLBACKS
                 All functions that are registered with QCSI framework shall be
                 defined in this section
                
               EXTERNALIZED Functions
                 All functions that are exposed through the header file shall be
                 implemented here.
  
                
  DEPENDENCIES: QCSI Framework
 
                Copyright (c) 2011-2012, 2013 Qualcomm Technologies Incorporated.
                All Rights Reserved.
                Copyright (c) 2013-2015 QUALCOMM Atheros, Inc.
                All Rights Reserved. 
                Qualcomm Confidential and Proprietary
                Copyright (c) 2015-2025 Qualcomm Technologies, Inc. 
                All Rights Reserved.
                Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_qmi_shim.c#5 $
  $DateTime: 2025/06/19 05:09:19 $
  $Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  06/18/24     bpk  FR99502:Incorporating latest location fix in emergency INVITE
  02/10/20     py    Navic SV Blacklisting Changes
  01/17/20     jeev  Robust location implementation
  05/15/19     Lock status for location privacy feature
  05/03/19     yh  Bug fix of checking LOCK status for batching messages
  02/19/19     skm 5G NR-NSA, LPP Rel-15 and SUPL 2.0.4 support 
  10/25/18     nath  Added support for GAGNS
  09/24/18     py  QMI Delete supl certificate Indication sending correct status
  06/17/18     sj  Added custom SLP for MO AGNSS requests.
  04/30/18     rt  Add flag for xtra integrity support
  04/23/18     py  Include session ID in position report
  04/06/18     sum Added code to clear in_progress flag in LocMw context
  04/04/18     py  Carrier Phase uncertainity sent in gnss sv measurements
  03/29/18     sum Added support for Unpropagated fix reporting 
  12/12/17     muk     Add support to output numClockResets
  09/14/17     sum Added support for GPDTM NMEAs
  02/17/17     skm BLE and Barometer support for LPPE Phase 2
  06/08/16     yh  Lppe
  05/19/16     yh  LocMW refactoring
  03/16/16     yh  XTRA PCID
  02/29/16     muk Add support to output numClockResets
  02/26/16     yh  Add 3 timeSrc enum parsing
  01/21/16     yh  LocWifi cleanup
  10/06/15     yh  QZSS
  04/06/15     sj  LB 2.0 integration
  05/28/15     jv  Disabled QMI_LOC_[SET/GET]_SBAS_CONFIG_REQ_V02
  05/21/15     jv  Support for SBAS EPH & ALM Deletion
  11/07/14   ssu   GM LOWI Integration
  02/19/15     sj  Added reqId to status indications sent back by DBT
  11/06/14     ah  Fixed QMI_LOC_GET_LOW_POWER_MODE Indication failure
  04/15/14     yh  XTRA version check added
  03/10/14     ah  Added support for Set Premium Services Cfg & Get Avail WWAN Pos
  01/24/14     ah  Added GLO support for Get/Set NMEA Type Requests in QMI-LOC
  02/25/13    ssu  Adding a new WWAN type used for AGNSS Emergency connections.
  01/28/12      mj   Update sm_RegisterApiNotifier to include client thread id
  2011-03-08  spn  Initial revision.

============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "gps_variation.h"
#include "customer.h"
#include "comdef.h"

/* Module Header include */
#include "loc_qmi_shim.h"

/* Non-GPS Includes */
#include "qmi_csi_target_ext.h"
#include "qmi_csi.h"
#include "qmi_sap.h"
#include "qmi_idl_lib.h"
#include "mqcsi_log.h"
#include "msg.h"
#include "queue.h"
#include "qmi_si.h"
#include "location_service_v02.h"
#include "location_service_impl_v02.h"
#include "common_v01.h"

/* GPS Includes */
#include "aries_os_api.h"
#include "loc_api_2.h"
#include "loc_client.h"
#include "loc_api_internal.h"
#include "gnss_calendar_util.h"
#include "loc_sdp.h"
#include "loc_slim.h"
#include "loc_geofence.h"
#include "cgps_api.h"
#include "gnss_cqsvc.h"
#include "loc_pd.h"
#include "loc_batching.h"
#include "loc_aon.h"
#include "loc_gtp.h"
#include "loc_wle.h"
#include "loc_pa.h"
#include "loc_nv_efs.h"
#include "loc_utils.h"
#include "loc_csm.h"
#include"loc_for_gtp.h"
/* PDAPI - Include PDAPI to convert newly added APIs to PDAPI directly */
#include "pdapi.h"
#ifdef __GRIFFON__
#include "mgp_api.h"
#endif /* __GRIFFON__ */
#include "gnss_common.h"
#include "loc_batching.h"
#include "loc_ot_batching.h"
#include "sm_util.h"
#include "gps_nv_efs.h"

/*****************************************************************************
 * Preprocessor Definitions and Constants
 * **************************************************************************/

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/* -----------------------------------------------------------------------*//**
@brief
Signal that is used by the QMI Framework to notify the task in case there 
are pending messages for the task to handle. 
*/ /* ------------------------------------------------------------------------*/
#define LOC_QMI_SHIM_SIGNAL             C_OS_FLAG_QMI_EVENT

/* -----------------------------------------------------------------------*//**
@brief
Macro definition to set the value of a simple data type and its validity
to TRUE. Simple macro-utility to update the QMI-Loc data structures for 
simple data-types.

@param[in]  variable    the variable which needs to be set
@param[in]  value       the value that it needs to be set to
*/ /* ------------------------------------------------------------------------*/
#define LOC_QMI_SHIM_VARIABLE_VALID(variable) variable ## _valid
#define LOC_QMI_SHIM_SET_VALUE(variable, value)                               \
           LOC_QMI_SHIM_VARIABLE_VALID(variable) = TRUE;                      \
           (variable)= (value)

/*****************************************************************************
 * Type Declarations
 * *************************************************************************/

/* -----------------------------------------------------------------------*//**
@brief 
The information required for handling messages is same as that of indication.
Hence reusing the same structure. 
*/ /* ------------------------------------------------------------------------*/
typedef locQmiShimIndInfoStructT locQmiShimMsgInfoStructT;

/* Union of QMI_LOC request information saved for PA-specific get/set parameters */
typedef union
{
  /* Store the request to set/get PE Config */
  qmiLocSetPositionEngineConfigParametersReqMsgT_v02 z_SetPeCfg;
  qmiLocGetPositionEngineConfigParametersReqMsgT_v02 z_GetPeCfg;
} locQmi_PaReqUnionT;

/* -----------------------------------------------------------------------*//**
@brief
Context information for each client is stored in this structure. Elements of
this type will be stored in a queue, where context information for all 
clients will be stored. This element also stores the mapping of handles as 
allocated by the QMI-Loc Service and the Loc-API. An instance of this 
structure is created whenever a new client connects, and destroyed when a 
client deregisters. This structure is used as the connection handle that 
will be given to the QCSI-Framework for it to identify a client to the 
service provider.
*/ /* ------------------------------------------------------------------------*/
typedef struct
{
  q_link_type                  z_Link; /*< Data Structure required by the queue 
                                           library to add this as part of a 
                                           queue */
  void                        *p_QmiClientHandle; /*< Handle identifying the 
                                            client of QMI-Loc to identify the  
                                            client */
  loc_client_handle_type       l_LocClientHandle; /*< Handle provided by 
                                           loc-api when a new client 
                                           registers with loc-api, as a result 
                                           of a client connecting to QMI-Loc */
  loc_client_info_s_type *p_LocClient; /* Pointer to the Loc Client object
                                       associated with this client */
  uint64                       t_EventMask; /*< Save the event mask to filter 
                                            the events from loc-api. As 
                                            QMI-Loc has more granularity */
  uint32                       q_CltRev; /*< The Client Revision, this is 
                                             indicated by the client */
  uint8                        u_SessionId; /*< Current session Id specified by the client */
  uint32                       q_FixId;     /*< Running counter of fixes generated in a session */

  qmiLocConfigIncludeAltitudeAssumedInGnssSvInfoEnumT_v02
     e_CfgAltAssumed; /* Configuration to choose if Altitude assumed
                         is needed in SV report for this client */
  boolean                      v_IsTdpClient;    /*< For TDP related clients this feild
                                                     will be set to TRUE */
  locQmi_PaReqUnionT           z_PaReqUnion;     /*< PA-specific get/set parameters */
} locQmiShimCltInfoStructT;

/* -----------------------------------------------------------------------*//**
@brief
THIS STRUCTURE SHOULD BE INSTANTIATED ONLY ONCE. This is the service handle 
that will be given to the framework to identify the service. And contains 
the state information of the shim layer. 
*/ /* ------------------------------------------------------------------------*/
typedef struct
{
  q_type                 z_ClientQ; /**< Information about all the clients that
                               have connected to the service. This is queue 
                               of type locQmiShimCltInfoStructT */
  void *p_ServiceHandle;  /**< Handle that the framework
provides to represent this service connection */

  qmi_sap_client_handle  z_QSAPUserHandle; /**< QSAP user handle to register with 
                                QSAP.*/
} locQmiShimInfoStructT;

/* -----------------------------------------------------------------------*//**
@brief
Function to handle GetBatchSize Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the get batch size request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locGetBatchSizeReq(locQmiShimCltInfoStructT    *pz_QmiShimCltInfo,
  qmi_req_handle              z_ReqHandle,
  locQmiShimMsgInfoStructT    *pz_MsgInfo);

/* -----------------------------------------------------------------------*//**
@brief
Untility function to send Response mesasge to Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the response message
@param[in]   resp_value            Response status being sent to control point
@param[in]   e_QmiErr              QMI Error type being sent to control point


@retval    TRUE           Sending Response to control point success
@retval    FALSE          Sending Response to control point fail
*/ /* ------------------------------------------------------------------------*/
static boolean locBatchingOperationSendResp(locQmiShimCltInfoStructT    *pz_CltInfo,
                                            qmi_req_handle              z_ReqHandle,
                                            locQmiShimMsgInfoStructT    *pz_MsgInfo,
                                            boolean                     resp_value,
                                            qmi_error_type_v01          e_QmiErr);

/* -----------------------------------------------------------------------*//**
@brief
Function to handle StartBatching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   pStartBatchingReq     Request message from the control point


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartBatchingReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                                   qmi_req_handle               z_ReqHandle,
                                   locQmiShimMsgInfoStructT     *pz_MsgInfo);

/* -----------------------------------------------------------------------*//**
@brief
Function to handle ReadFromBatch Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the ReadFromBatch request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locReadFromBatchReq(locQmiShimCltInfoStructT     *pz_CltInfo,
                                   qmi_req_handle               z_ReqHandle,
                                   locQmiShimMsgInfoStructT     *pz_MsgInfo);

/* -----------------------------------------------------------------------*//**
@brief
Function to handle StopBatching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopBatching request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStopBatchingReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                                  qmi_req_handle              z_ReqHandle,
                                  locQmiShimMsgInfoStructT    *pz_MsgInfo);


/* -----------------------------------------------------------------------*//**
@brief
Function to handle ReleaseBatch Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the ReleaseBatch request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locReleaseBatchReq(locQmiShimCltInfoStructT      *pz_QmiShimCltInfo,
                                  qmi_req_handle                z_ReqHandle,
                                  locQmiShimMsgInfoStructT      *pz_MsgInfo);


/* -----------------------------------------------------------------------*//**
@brief
Function to handle a Start DBT session from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StartDBT request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartDbtReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                              qmi_req_handle              z_ReqHandle,
                              locQmiShimMsgInfoStructT    *pz_MsgInfo);

/* -----------------------------------------------------------------------*//**
@brief
Function to handle Stop a DBT session from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopDBT request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStopDbtReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                             qmi_req_handle              z_ReqHandle,
                             locQmiShimMsgInfoStructT    *pz_MsgInfo);



/* -----------------------------------------------------------------------*//**
@brief
Function to retrieve the Aon configuration 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopDBT request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locQueryAonConfigReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                                    qmi_req_handle              z_ReqHandle,
                                    locQmiShimMsgInfoStructT    *pz_MsgInfo);

/* -----------------------------------------------------------------------*//**
@brief
Function to handle Start Outdoor Trip Batching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                 to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StartOTB request



@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartOTBReq(locQmiShimCltInfoStructT     *pz_CltInfo,
                              qmi_req_handle               z_ReqHandle,
                              locQmiShimMsgInfoStructT     *pz_MsgInfo);

/*---------------------------------------------------------------------------
@brief
This function converts PDSM Constellation Status to QMI Type.

@param[in] pdsm_constellation_status_e_type

@retval    qmiLocConstellationStatusEnumT_v02
---------------------------------------------------------------------------*/
static qmiLocConstellationStatusEnumT_v02 locQmiShimConvertConstellationStatusInd(
   pdsm_constellation_status_e_type e_PdsmStatus);

/* Externing as including sm_api.h causes a lot of dependencies */
extern boolean sm_RegisterApiNotifier(void (*sm_ApiInitNotifierCb)(void), os_ThreadIdEnumType clientThreadId);

/*****************************************************************************
 * Global Data Definitions
 * *************************************************************************/

/*****************************************************************************
 * Static Variable Definitions
 * *************************************************************************/

/* -----------------------------------------------------------------------*//**
@brief
A Single instance of the module context information. This instance is also
provided as a service cookie to the QCSI framework. 

This information SHALL NOT BE EXPOSED OUT OF THIS FILE  
*/ /* ------------------------------------------------------------------------*/
static locQmiShimInfoStructT z_LocQmiShim;

/* -----------------------------------------------------------------------*//**
@brief
QMI-Loc Supported Feature Table
1. The table sequence must follow enum sequence,
enum 0 at table{0], enum 1 at table[1], etc
l. QmiLocFeature is only for tracking feature enum purpose.
2. Never remove any record from this table, if feature is not supported, set 
to FALSE
3. This table must updated when location_service_v02.h ->
   qmiLocSupportedFeatureEnumT_v02 is updated
4. eQMI_LOC_SUPPORTED_FEATURE max value shall not exceed 799

This is a singleton, there is no need to define another instance of this
*/ /* ------------------------------------------------------------------------*/
static const struct {
  qmiLocSupportedFeatureEnumT_v02     e_QmiLocFeature;
  boolean                             v_Support;
}z_LocQmiShimSupportedFeatureTable[] =
{
  {  eQMI_LOC_SUPPORTED_FEATURE_ODCPI_2_V02,                        TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_WIFI_AP_DATA_INJECT_2_V02,          TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_DEBUG_NMEA_V02,                     TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_GNSS_ONLY_POSITION_REPORT_V02,      FALSE },
  {  eQMI_LOC_SUPPORTED_FEATURE_FDCL_V02,                           TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_CONSTELLATION_ENABLEMENT_V02,       TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_AGPM_V02,                           TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_XTRA_INTEGRITY_V02,                 TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_FDCL_2_V02,                         TRUE },
  {  eQMI_LOC_SUPPORTED_FEATURE_LOCATION_PRIVACY_V02,               TRUE }
};


/* For Griffon*/
#ifdef __GRIFFON__
#include "qmi_csi_common.h"
#include "upos.h"
extern qmi_csi_xport_ops_type udp_ops;
static qmi_csi_os_params g_z_OsParams;
static UPOS_thread_t upos_thread;
static void read_udp_and_signal()
{
  fd_set fds;

  printf("created thread: in read_udp_and_signal\n");
  /* This loop calls a blocking select to read from the UDP port that was
         provided to QCSI during service registration */
  while (1)
  {
    //Wait for a signal from the mother task to resume
    UPOS_ThreadWait(&upos_thread);
    fds = g_z_OsParams.fds;
    printf("read_udp_and_signal: waiting for fd signal\n");
    select(g_z_OsParams.max_fd + 1, &fds, NULL, NULL, NULL);

    // signal the "mother" thread
    printf("read_udp_and_signal: signaling the main thread\n");
    UPOS_ThreadSignalTask(&upos_thread, C_OS_FLAG_QMI_EVENT);
  }
}
#endif /* __GRIFFON__ */

/* -----------------------------------------------------------------------*//**
@brief
A helper utility provided to the queue library. This function is intended to 
see if the  ShimCltInfo represented by "item_ptr" holds information for a 
QMI-Client represented by "p_compareVal" which is also a ShimCltInfo 
structure.

@param[in]   p_ItemPtr         A potential item of z_LocQmiShim.z_ClientQ
@param[in]   p_CompareVal      Qmi-Client handle that is being considered

@retval    0          Item was not the one we are searching for
@retval    1          Item was the one we are searching for
*/ /* ------------------------------------------------------------------------*/
static int locQmiShimCompareConnectionInfoHandle
(
   void *p_ItemPtr,
   void *p_CompareVal
   )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* item_ptr is an element of the Client Q i.e. Connection Handle, 
     and compare val is also a Connection Handle */
  return ((p_ItemPtr == p_CompareVal) ? 1 : 0);
}

/* -----------------------------------------------------------------------*//**
@brief
A helper utility provided to the queue library. This function is intended to 
see if the  ShimCltInfo represented by "item_ptr" holds information for a 
QMI-Client represented by "p_compareVal" which is 
ShimCltInfo->p_QmiClientHandle

@param[in]   p_ItemPtr         A potential item of z_LocQmiShim.z_ClientQ
@param[in]   p_CompareVal      Qmi-Client handle that is being considered

@retval    0          Item was not the one we are searching for
@retval    1          Item was the one we are searching for
*/ /* ------------------------------------------------------------------------*/
static int locQmiShimCompareQmiClientHandle
(
   void *p_ItemPtr,
   void *p_CompareVal
   )
{
  locQmiShimCltInfoStructT *pz_CltInfo = (locQmiShimCltInfoStructT *)p_ItemPtr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* item_ptr is an element of the Client Q i.e. Connection Handle, 
     and compare val is also a Connection Handle */
  return ((pz_CltInfo->p_QmiClientHandle == p_CompareVal) ? 1 : 0);
}

/* -----------------------------------------------------------------------*//**
@brief
A helper utility provided to the queue library. This function is intended to 
see if the  ShimCltInfo represented by "item_ptr" holds information for a 
Loc-Api Client Handle represented by "p_compareVal".

@param[in]   p_ItemPtr         A potential item of z_LocQmiShim.z_ClientQ
@param[in]   p_CompareVal      Loc-Api Client handle that is being considered

@retval    0          Item was not the one we are searching for
@retval    1          Item was the one we are searching for
*/ /* ------------------------------------------------------------------------*/
static int locQmiShimCompareLocApiHandle
(
   void *p_ItemPtr,
   void *p_CompareVal
   )
{
  locQmiShimCltInfoStructT *pz_Item = (locQmiShimCltInfoStructT *)p_ItemPtr;
  loc_client_handle_type   z_locHandle;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  z_locHandle = (loc_client_handle_type)p_CompareVal;
  /* item_ptr is an element of the Client Q i.e. Connection Handle, 
     and compare val is also a Connection Handle */
  return ((pz_Item->l_LocClientHandle == z_locHandle) ? 1 : 0);
}

/* -----------------------------------------------------------------------*//**
@brief
Function registered with TM. This function is called when TM is initialized 
and we are notified. On receipt of this callback the QMI-LOC Service needs 
to be initialized.

But this function will queue an command to loc-middleware task, and 
initialize the QMI-LOC service as part of processing the message.

*/ /* ------------------------------------------------------------------------*/
static void locQmiShimInitNotifierCb
(
   void
   )
{
  boolean v_RetVal = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* The only information required is the CMD Type */
  v_RetVal = locMW_SendLocCmdProxy(
     LOC_HANDLE_REGISTERED_CLIENTS,
     LOC_CMD_TYPE_INIT_QMI_LOC,
     0,
     NULL);

  if (FALSE == v_RetVal)
  {
    LOC_ERR_FATAL("Cannot initialize the QMI-LOC Service", 0, 0, 0);
  }

}

/*============================================================================
 *                    L O C - A P I    C A L L B A C K S 
 * =========================================================================*/
/* -----------------------------------------------------------------------*//**
@brief
A helper utility to create QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND message.

@param[in]   pz_LocClient   Location Client Info Handle
@param[in]   e_ResType      Indications whether the location information was provided
                         for the client or not
@param[in]   v_IsCachedLocation Is the cached location provided
@param[in/out]   pz_IndMsg      QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND to fill up

@retval   void
*/ /* ------------------------------------------------------------------------*/
static void locQmiCreateNFWPosRequestNotificationIndMsg
(
   loc_client_info_s_type *pz_LocClient,
   qmiLocRequestResponseTypeEnumT_v02  e_ResType,
   boolean v_IsCachedLocation,
   qmiLocLocationRequestNotificationIndMsgT_v02 *pz_IndMsg
   )
{
  uint32 q_ClientIdStrLen = GNSS_STRLEN(pz_LocClient->pb_ClientIdStr);
  MSG_SPRINTF_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_LOW, "CreateNFW clientStrId = %s",
                pz_LocClient->pb_ClientIdStr);
  LOC_MSG_LOW("locQmiCreateNFWPosRequestNotificationIndMsg q_ClientIdStrLen=%d",
              q_ClientIdStrLen, 0, 0);

  /* Skip the input parameter check
  because it is called within trusted source code */

  pz_IndMsg->protocolStack = eQMI_LOC_OTHER_V02;
  pz_IndMsg->clientStrId_valid = FALSE;
  if (pz_LocClient->v_ClientSetIdStr)
  {
    if (3 == q_ClientIdStrLen &&
        0 == strncmp(pz_LocClient->pb_ClientIdStr, "IMS", 3))
    {
      pz_IndMsg->protocolStack = eQMI_LOC_IMS_V02;
    }
    else if (3 == q_ClientIdStrLen &&
             0 == strncmp(pz_LocClient->pb_ClientIdStr, "SIM", 3))
    {
      pz_IndMsg->protocolStack = eQMI_LOC_SIM_V02;
    }
    else if (3 == q_ClientIdStrLen &&
             0 == strncmp(pz_LocClient->pb_ClientIdStr, "MDT", 3))
    {
      pz_IndMsg->protocolStack = eQMI_LOC_MDT_V02;
    }
    else if (4 == q_ClientIdStrLen &&
             0 == strncmp(pz_LocClient->pb_ClientIdStr, "TLOC", 4))
    {
      pz_IndMsg->protocolStack = eQMI_LOC_TLOC_V02;
    }
    pz_IndMsg->clientStrId_valid = TRUE;
    GNSS_STRLCPY(pz_IndMsg->clientStrId,
                 pz_LocClient->pb_ClientIdStr,
                 QMI_LOC_MAX_CLIENT_ID_STRING_LENGTH_V02 + 1);
  }

  /* Hardcode for requestor per google's reqirement */
  GNSS_STRLCPY(pz_IndMsg->requestorId,
               "other", QMI_LOC_MAX_REQUESTOR_ID_STRING_LENGTH_V02 + 1);
  pz_IndMsg->requestor = eQMI_LOC_REQUESTOR_OTHER_V02;

  pz_IndMsg->responseType = e_ResType;

  if (0 == pz_LocClient->q_ReportEventRecord)
    pz_IndMsg->responseProtocol = QMI_LOC_GNSS_POSITION_V02;
  else
    pz_IndMsg->responseProtocol = pz_LocClient->q_ReportEventRecord;
  pz_LocClient->q_ReportEventRecord = 0;

  pz_IndMsg->inEmergencyMode = loc_middleware_data.v_E911Session;
  pz_IndMsg->isCachedLocation = v_IsCachedLocation;

}

/* -----------------------------------------------------------------------*//**
@brief
A helper utility to send QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND message.

@param[in]   pz_LocClient   Location Client Info Handle
@param[in]   e_ResType      Indications whether the location information was provided
                         for the client or not
@param[in]   v_IsCachedLocation Is the cached location provided

@retval   void
*/ /* ------------------------------------------------------------------------*/
static void locQmiSendNFWPosRequestNotificationInd
(
   loc_client_info_s_type *pz_LocClient,
   qmiLocRequestResponseTypeEnumT_v02  e_ResType,
   boolean v_IsCachedLocation
   )
{
  qmiLocLocationRequestNotificationIndMsgT_v02 z_IndMsg = { 0 };
  loc_client_handle_type  l_MasterClientHandle = loc_middleware_data.z_locClnt_Data.l_MasterClientHandle;
  locQmiShimMsgInfoStructT   z_IndInfo = { 0 };

  /* Skip the input parameter check
  because it is called within trusted source code */

  if (LOC_CLIENT_HANDLE_INVALID == l_MasterClientHandle)
  {
    LOC_MSG_ERROR("locQmiSendNFWPosRequestNotificationInd no master client", 0, 0, 0);
    return;
  }

  if (FALSE == locClnt_IsNFWPosRequestNotificationAllowed(pz_LocClient))
  {
    LOC_MSG_LOW("locQmiSendNFWPosRequestNotificationInd not allowed", 0, 0, 0);
    return;
  }

  locQmiCreateNFWPosRequestNotificationIndMsg(pz_LocClient,
                                              e_ResType, v_IsCachedLocation, &z_IndMsg);
  z_IndInfo.q_Id = QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02;
  z_IndInfo.q_MsgLen = sizeof(qmiLocLocationRequestNotificationIndMsgT_v02);
  z_IndInfo.p_Msg = &z_IndMsg;

  /* Send IND message to the master client */
  (void)locQmiShimSendInd(l_MasterClientHandle, &z_IndInfo);

}
/* -----------------------------------------------------------------------*//**
@brief
Function registered with Loc-Api, which will be called to notify the shim of 
events from loc-api

@param[in]   z_LocHandle           Loc-Client Handle
@param[in]   e_LocEvent            Event identifier
@param[in]   pz_LocEventPayload    Payload of the event

@retval    LOC_API_SUCCESS        Indication handled
@retval    <Anything Else>        Indication could not be handled. Defined in 
                               loc_api.h
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimEventCb
(
   loc_client_handle_type           l_LocClientHandle,
   locClnt_EventMaskType            e_LocEvent,
   const void *p_LocEventPayload
   )
{
  qmi_csi_error               e_QmiErr;
  locQmiShimCltInfoStructT   *pz_CltInfo = NULL;
  locQmiShimMsgInfoStructT    z_IndInfo = { 0 };
  boolean                     v_SendInd = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  pz_CltInfo = q_linear_search(&z_LocQmiShim.z_ClientQ,
                               locQmiShimCompareLocApiHandle,
                               (void *)l_LocClientHandle);

  /* If the client was not found */
  if ((NULL == pz_CltInfo) ||
      (NULL == p_LocEventPayload))
  {
    LOC_MSG_ERROR("locQmiShimEventCb Error NULL param: locQmiShimClt = 0x%p for ClientHandle = %d, p_LocEventPayload = 0x%p",
                  pz_CltInfo, l_LocClientHandle, p_LocEventPayload);
    return FALSE;
  }

  /* Message assignment is common */
  z_IndInfo.p_Msg = (void *)p_LocEventPayload;
  switch (e_LocEvent)
  {
    case LOC_EVENT_PARSED_POSITION_REPORT:
      {
        qmiLocEventPositionReportIndMsgT_v02 *pz_PositionReportIndMsg = (qmiLocEventPositionReportIndMsgT_v02 *)p_LocEventPayload;
        pz_PositionReportIndMsg->sessionId = pz_CltInfo->u_SessionId;
        z_IndInfo.q_Id = QMI_LOC_EVENT_POSITION_REPORT_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventPositionReportIndMsgT_v02);

        if ((NULL != pz_PositionReportIndMsg) &&
            (eQMI_LOC_SESS_STATUS_SUCCESS_V02 == pz_PositionReportIndMsg->sessionStatus))
        {
          uint64 t_positionReportTime = cgps_TimeTickGetMsec();
          gnss_UpdateQmiPosRptTime(pz_PositionReportIndMsg->gpsTime.gpsWeek,
                                   pz_PositionReportIndMsg->gpsTime.gpsTimeOfWeekMs,
                                   t_positionReportTime);
          LOC_MSG_MED("LOC_EVENT_PARSED_POSITION_REPORT gpsWeek=%u, gpsTimeOfWeekMs=%u,position report time=%u,",
                      pz_PositionReportIndMsg->gpsTime.gpsWeek,
                      pz_PositionReportIndMsg->gpsTime.gpsTimeOfWeekMs,
                      t_positionReportTime);

        }
      }
      break;

    case LOC_EVENT_SATELLITE_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_GNSS_SV_INFO_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventGnssSvInfoIndMsgT_v02);
      }
      break;

    case LOC_EVENT_NMEA_1HZ_REPORT:
    case LOC_EVENT_NMEA_POSITION_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_NMEA_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventNmeaIndMsgT_v02);
      }
      break;

    case LOC_EVENT_NI_NOTIFY_VERIFY_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_NI_NOTIFY_VERIFY_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventNiNotifyVerifyReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_ASSISTANCE_DATA_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_INJECT_POSITION_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventInjectPositionReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_INJECT_TIME_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_INJECT_TIME_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventInjectTimeReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_INJECT_XTRA_DATA_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_INJECT_PREDICTED_ORBITS_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventInjectPredictedOrbitsReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_QUERY_XTRA_INFO_STATUS:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_QUERY_XTRA_INFO_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventQueryXtraInfoReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_STATUS_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_ENGINE_STATE_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventEngineStateIndMsgT_v02);
      }
      break;

    case LOC_EVENT_FIX_SESSION_STATE_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_FIX_SESSION_STATE_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventFixSessionStateIndMsgT_v02);
      }
      break;

    case LOC_EVENT_WPS_NEEDED_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_WIFI_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventWifiReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_LOCATION_SERVER_REQUEST:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_LOCATION_SERVER_CONNECTION_REQ_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventLocationServerConnectionReqIndMsgT_v02);
      }
      break;

    case LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT:
    case LOC_EVENT_GNSS_MEASUREMENT_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_GNSS_MEASUREMENT_REPORT_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventGnssSvMeasInfoIndMsgT_v02);
      }
      break;

    case LOC_EVENT_SV_POLY_REPORT:
      {
        z_IndInfo.q_Id = QMI_LOC_EVENT_SV_POLYNOMIAL_REPORT_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocEventGnssSvPolyIndMsgT_v02);
      }
      break;

    case LOC_EVENT_MASK_BAND_MEASUREMENT_METRICS:
      {
        z_IndInfo.q_Id = QMI_LOC_GET_BAND_MEASUREMENT_METRICS_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocGetBandMeasurementMetricsIndMsgT_v02);
        break;
      }

    case LOC_EVENT_MASK_DBH_POSITION:
    {
      z_IndInfo.q_Id = QMI_LOC_EVENT_DBH_POSITION_IND_V02;
      z_IndInfo.q_MsgLen = sizeof(qmiLocEventDbhPositionIndMsgT_v02);
    }
    break;

    default:
      LOC_MSG_ERROR("locQmiShimEventCb Unhandled locEvent 0x%x ", e_LocEvent, 0, 0);
      v_SendInd = FALSE;
      break;
  }

  if (v_SendInd)
  {
    e_QmiErr = mqcsi_send_ind(pz_CltInfo->p_QmiClientHandle,
                              z_IndInfo.q_Id,
                              z_IndInfo.p_Msg,
                              z_IndInfo.q_MsgLen);

    if (QMI_CSI_NO_ERR != e_QmiErr)
    {
      /* Could not send an event out, some error in QCSI framework */
      LOC_MSG_ERROR("locQmiShimEventCb: <<< QMI_LOC Msg 0x%04x ind failed for LocClient %d, QMI_CSI_ERR=%d ",
                    z_IndInfo.q_Id, l_LocClientHandle, e_QmiErr);
    }
    else
    {
      loc_client_info_s_type *pz_LocClient = locClnt_FindHandle(l_LocClientHandle);
      LOC_MSG_MED("locQmiShimEventCb: <<< QMI_LOC Msg 0x%04x ind succeed for LocClient %d",
                  z_IndInfo.q_Id, l_LocClientHandle, 0);
      if (pz_LocClient)
      {
        if (QMI_LOC_EVENT_GNSS_MEASUREMENT_REPORT_IND_V02 == z_IndInfo.q_Id)
        {
          pz_LocClient->q_ReportEventRecord |= QMI_LOC_GNSS_MEASUREMENT_V02;
    }
        else if (QMI_LOC_EVENT_POSITION_REPORT_IND_V02 == z_IndInfo.q_Id)
        {
          /* report QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02 */
          pz_LocClient->q_ReportEventRecord |= QMI_LOC_GNSS_POSITION_V02;
          switch (((qmiLocEventPositionReportIndMsgT_v02 *)(z_IndInfo.p_Msg))->sessionStatus)
          {
            case eQMI_LOC_SESS_STATUS_SUCCESS_V02:
            case eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02:
              locQmiSendNFWPosRequestNotificationInd(pz_LocClient,
                                                     eQMI_LOC_ACCEPTED_LOCATION_PROVIDED_V02, FALSE);
              break;

            case eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02:
            case eQMI_LOC_SESS_STATUS_TIMEOUT_V02:
            case eQMI_LOC_SESS_STATUS_BAD_PARAMETER_V02:
            case eQMI_LOC_SESS_STATUS_PHONE_OFFLINE_V02:
            case eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02:
              locQmiSendNFWPosRequestNotificationInd(pz_LocClient,
                                                     eQMI_LOC_ACCEPTED_NO_LOCATION_PROVIDED_V02, FALSE);
              break;

              /* User end, don't send indication */
            default:
              break;
  }

  }
      }
  else
  {
        LOC_MSG_ERROR("locQmiShimEventCb: LocClientHandle %d doesn't exist", l_LocClientHandle, 0, 0);
      }
    }
  }
  else
  {
    /* Could not send an event out, some error in translation */
    LOC_MSG_ERROR("locQmiShimEventCb: Not Send QMI_LOC_EVENT_IND locEvent 0x%x for LocClient %d",
                  e_LocEvent, l_LocClientHandle, 0);
  }

  return v_SendInd;
}

/*============================================================================
 *                       M E S S A G E    H A N D L E R S 
 * =========================================================================*/
/* -----------------------------------------------------------------------*//**
@brief
Message handler to handle the stop-request from the client

@param[in]  pz_CltInfo            Client for whom this message is intended
@param[in]  p_MsgInfo             Information regarding to the message
@param[out] pe_QmiErr             if Failure, this will be populated to 
                               identify the reason for error

@retval    TRUE                   Message successfully handled
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimStopReqHdlr
(
   const locQmiShimCltInfoStructT         *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  boolean                     v_RetVal = TRUE;
  int32                       i_LocApiErr = LOC_API_SUCCESS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Not doing check of inputs as this is called from within the module,
     and this will be called with proper parameters */

  /* Forward call to loc-api */
  i_LocApiErr = loc_stop_fix(pz_CltInfo->l_LocClientHandle);

  if (LOC_API_SUCCESS != i_LocApiErr)
  {
    v_RetVal = FALSE;

    switch (i_LocApiErr)
    {
      case LOC_API_INVALID_HANDLE:
        (*pe_QmiErr) = QMI_ERR_INVALID_HANDLE_V01;
        break;

        /* For all other values relay INTERNAL error */
      default:
        (*pe_QmiErr) = QMI_ERR_INTERNAL_V01;
        break;
    }
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Message handler to store and handle the client revision that is given 
by the application

@param[out]  pz_CltInfo            Client for whom this message is intended
@param[in]   p_MsgInfo             Information regarding to the message
@param[out]  pe_QmiErr             if Failure, this will be populated to 
                                identify the reason for error

@retval    TRUE                   Message successfully handled
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleClientRevision
(
   locQmiShimCltInfoStructT               *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  boolean                     v_RetVal = TRUE;
  const qmiLocInformClientRevisionReqMsgT_v02 *pz_Clt = pz_MsgInfo->p_Msg;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Not doing check of inputs as this is called from within the module,
     and this will be called with proper parameters */

  /* Store the revision of the client revision  */
  pz_CltInfo->q_CltRev = pz_Clt->revision;

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Message handler for the get service revision req_v02 API from the 
application. 

@param[out]  pz_CltInfo            Client for whom this message is intended
@param[in]   p_MsgInfo             Information regarding to the message
@param[out]  pe_QmiErr             if Failure, this will be populated to 
                                identify the reason for error

@retval    TRUE                   Message successfully handled
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleGetServRevisionReq_v02
(
   locQmiShimCltInfoStructT               *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  qmiLocGetServiceRevisionIndMsgT_v02 *pz_GetSvcRevInd = NULL;
  boolean     v_RetVal = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == pz_CltInfo) || (NULL == pz_MsgInfo) ||
      (NULL == pe_QmiErr))
  {
    /* These NULL condition shall never happen */
    LOC_MSG_ERROR("locQmiShimHandleGetServRevisionReq_v02 pz_CltInfo=%p, pz_MsgInfo = %p, pe_QmiErr=%p",
                  pz_CltInfo, pz_MsgInfo, pe_QmiErr);
    if (NULL != pe_QmiErr)
    {
      (*pe_QmiErr) = QMI_ERR_MALFORMED_MSG_V01;
    }
    return FALSE;
  }

  pz_GetSvcRevInd = (qmiLocGetServiceRevisionIndMsgT_v02 *)loc_calloc(sizeof(*pz_GetSvcRevInd));
  if (NULL == pz_GetSvcRevInd)
  {
    LOC_MSG_ERROR("locQmiShimHandleGetServRevisionReq_v02: calloc return NULL", 0, 0, 0);
    return v_RetVal;
  }

  pz_GetSvcRevInd->status   = eQMI_LOC_SUCCESS_V02;
  pz_GetSvcRevInd->revision = LOC_V02_IDL_MINOR_VERS;

#ifdef __GRIFFON__
  p_GetSvcRevInd->gnssMeFWVerString_valid   =
     mgp_GetGNSSSWVersion(
     C_MGP_VER_STR_ME,
     p_GetSvcRevInd->gnssMeFWVerString,
     sizeof(p_GetSvcRevInd->gnssMeFWVerString));

  p_GetSvcRevInd->gnssHostSWVerString_valid =
     mgp_GetGNSSSWVersion(
     C_MGP_VER_STR_HOST,
     p_GetSvcRevInd->gnssHostSWVerString,
     sizeof(p_GetSvcRevInd->gnssHostSWVerString));

  p_GetSvcRevInd->gnssSWVerString_valid     =
     mgp_GetGNSSSWVersion(
     C_MGP_VER_STR_FULL,
     p_GetSvcRevInd->gnssSWVerString,
     sizeof(p_GetSvcRevInd->gnssSWVerString));
#else
  pz_GetSvcRevInd->gnssMeFWVerString_valid   = FALSE;
  pz_GetSvcRevInd->gnssHostSWVerString_valid = FALSE;
  pz_GetSvcRevInd->gnssSWVerString_valid     = FALSE;
#endif /* __GRIFFON__ */

  v_RetVal = locMW_SendQmiIndicationProxy(pz_CltInfo->l_LocClientHandle,
                                          QMI_LOC_GET_SERVICE_REVISION_IND_V02,
                                          pz_GetSvcRevInd, LOC_API_SUCCESS);

  return v_RetVal;

}

static boolean locQmiShimHandleInjectTimeZoneInfoReq
(
   locQmiShimCltInfoStructT        *pz_CltInfo,
   const locQmiShimMsgInfoStructT  *pz_MsgInfo,
   qmi_error_type_v01              *pe_QmiErr,
   qmiLocGenRespMsgT_v02           *pz_QmiResp
   )
{
  boolean v_RetVal = FALSE;
  qmiLocInjectTimeZoneInfoIndMsgT_v02 z_InjectTZInfoInd = { 0 };

  if (NULL == pz_CltInfo || NULL == pz_MsgInfo ||
      NULL == pe_QmiErr || NULL == pz_QmiResp)
  {
    LOC_MSG_ERROR("locQmiShimHandleInjectTimeZoneInfoReq pz_CltInfo=%p, pz_MsgInfo = %p",
                  pz_CltInfo, pz_MsgInfo, 0);
    LOC_MSG_ERROR("locQmiShimHandleInjectTimeZoneInfoReq pe_QmiErr=%p, pz_QmiResp = %p",
                  pe_QmiErr, pz_QmiResp, 0);
    if (NULL != pe_QmiErr)
    {
      (*pe_QmiErr) = QMI_ERR_MALFORMED_MSG_V01;
    }
    return v_RetVal;
  }

  v_RetVal = loc_geofence_inject_timezone_info(
     &z_InjectTZInfoInd,
     (qmiLocInjectTimeZoneInfoReqMsgT_v02 *)pz_MsgInfo->p_Msg,
     pz_QmiResp);
  if (TRUE == v_RetVal)
  {
    LOC_MSG_MED("Send Indication to QMI Client for TimeZone info", 0, 0, 0);

    /* Queue up the indication that needs to be sent */
    v_RetVal = locMW_SendQmiIndicationProxy(pz_CltInfo->l_LocClientHandle,
                                            QMI_LOC_INJECT_TIME_ZONE_INFO_IND_V02,
                                            NULL, LOC_API_SUCCESS);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Message handler to handle the Get Supported Feature QMI_LOC message .
Ind message sent back directly from this handler function.

@param[in|out]  pz_CltInfo        Client for whom this message is intended
@param[in]      p_MsgInfo         Information regarding to the message
@param[out]     pe_QmiErr         if Failure, this will be populated to 
                               identify the reason for error

@retval    TRUE                   Message successfully handled. It means that 
                               Ind message will be sent back to the client.
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleGetSupportedFeatureReq
(
   locQmiShimCltInfoStructT               *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  qmiLocGetSupportedFeatureIndMsgT_v02   *pz_GetSupportedFeatureInd = NULL;
  uint32                                  q_Count = 0;
  uint32                                  q_NumFeature = 0;
  uint8                                   u_Index = 0;
  uint8                                   u_FeatureBit = 0;
  boolean                                 v_RetVal = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == pz_CltInfo) || (NULL == pz_MsgInfo) ||
      (NULL == pe_QmiErr))
  {
    /* These NULL condition shall never happen */
    LOC_MSG_ERROR("locQmiShimHandleGetServRevisionReq_v02 pz_CltInfo=%p, pz_MsgInfo = %p, pe_QmiErr=%p",
                  pz_CltInfo, pz_MsgInfo, pe_QmiErr);
    if (NULL != pe_QmiErr)
    {
      (*pe_QmiErr) = QMI_ERR_MALFORMED_MSG_V01;
    }
    return FALSE;
  }

  pz_GetSupportedFeatureInd = (qmiLocGetSupportedFeatureIndMsgT_v02 *)loc_calloc(sizeof(*pz_GetSupportedFeatureInd));
  if (NULL == pz_GetSupportedFeatureInd)
  {
    LOC_MSG_ERROR("locQmiShimHandleGetSupportedFeatureReq: calloc return NULL", 0, 0, 0);
    return v_RetVal;
  }

  pz_GetSupportedFeatureInd->status = eQMI_LOC_SUCCESS_V02;
  q_NumFeature = sizeof(z_LocQmiShimSupportedFeatureTable) / sizeof(z_LocQmiShimSupportedFeatureTable[0]);
  if (QMI_LOC_SUPPORTED_FEATURE_LENGTH_V02 * 8 < q_NumFeature)
  {
    LOC_MSG_ERROR("locQmiShimHandleGetSupportedFeatureReq, check feature size %d", q_NumFeature, 0, 0);
    q_NumFeature = QMI_LOC_SUPPORTED_FEATURE_LENGTH_V02 * 8;
  }
  pz_GetSupportedFeatureInd->feature_len = q_NumFeature / 8 + 1;  //feature bit is set in unit8
  for (q_Count = 0; q_Count < q_NumFeature; q_Count++)
  {
    u_Index = q_Count / 8;
    u_FeatureBit = q_Count % 8;
    if (TRUE == z_LocQmiShimSupportedFeatureTable[q_Count].v_Support)
    {
      pz_GetSupportedFeatureInd->feature[u_Index] |= (1 << u_FeatureBit);
    }
  }

  /* Queue up the indication that needs to be sent */
  v_RetVal = locMW_SendQmiIndicationProxy(pz_CltInfo->l_LocClientHandle,
                                          QMI_LOC_GET_SUPPORTED_FEATURE_IND_V02,
                                          pz_GetSupportedFeatureInd, LOC_API_SUCCESS);

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Message handler to handle QMI_LOC_GET_REGISTERED_EVENTS_REQ message .

@param[in|out]  pz_CltInfo        Client for whom this message is intended
@param[in]      p_MsgInfo         Information regarding to the message
@param[out]     pe_QmiErr         if Failure, this will be populated to 
                                identify the reason for error

@retval    TRUE                   Message successfully handled. It means that 
                                Ind message will be sent back to the client.
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleGetRegisteredEventsReq
(
   locQmiShimCltInfoStructT               *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  qmiLocGetRegisteredEventsIndMsgT_v02 *pz_GetRegMaskInd = NULL;
  boolean                                 v_RetVal = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == pz_CltInfo) || (NULL == pz_MsgInfo) ||
      (NULL == pe_QmiErr))
  {
    /* These NULL condition shall never happen */
    LOC_MSG_ERROR("locQmiShimHandleGetRegisteredEventsReqpz_CltInfo=%p, pz_MsgInfo = %p, pe_QmiErr=%p",
                  pz_CltInfo, pz_MsgInfo, pe_QmiErr);
    return v_RetVal;
  }

  pz_GetRegMaskInd = (qmiLocGetRegisteredEventsIndMsgT_v02 *)loc_calloc(sizeof(*pz_GetRegMaskInd));
  if (NULL == pz_GetRegMaskInd)
  {
    LOC_MSG_ERROR("locQmiShimHandleGetRegisteredEventsReqcalloc return NULL", 0, 0, 0);
    return v_RetVal;
  }

  LOC_QMI_SHIM_SET_VALUE(pz_GetRegMaskInd->eventRegMask, pz_CltInfo->t_EventMask);

  /* Queue up the indication that needs to be sent */
  v_RetVal = locMW_SendQmiIndicationProxy(pz_CltInfo->l_LocClientHandle,
                                          QMI_LOC_GET_REGISTERED_EVENTS_IND_V02,
                                          pz_GetRegMaskInd, LOC_API_SUCCESS);

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
*  Message handler to handle QMI_LOC_START_LISTEN_POSITION_REQ message 
*
* @param pz_ClientInfo LocAPI client handle
* @param pz_Req        QMI request
* @param pz_Resp       QMI response
*
* @return TRUE if the message has been handled successfully, else
*         FALSE
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleStartListenPositionReq
(
   loc_client_info_s_type *pz_ClientInfo,
   qmiLocStartListenPositionReqMsgT_v02 *pz_Req,
   qmiLocGenRespMsgT_v02 *pz_Resp
   )
{
  boolean                                 v_RetVal = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pz_Resp)
  {
    LOC_MSG_ERROR("locQmiShimHandleStartListenPositionReq: pResp is NULL \n", 0, 0, 0);
    return v_RetVal;
  }

  if ((NULL == pz_ClientInfo) || (NULL == pz_Req))
  {
    /** invalid parameters */
    pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
    pz_Resp->resp.error = QMI_ERR_INVALID_ARG_V01;
    LOC_MSG_ERROR("locQmiShimHandleStartListenPositionReq: Invalid params\n", 0, 0, 0);
    return v_RetVal;
  }

  if (FALSE == locClnt_IsPosRequestAllowed(pz_ClientInfo))
  {
    qmiLocEventPositionReportIndMsgT_v02   z_PosRptInd = { 0 };
    locQmiShimIndInfoStructT   z_IndInfo  = { 0 };

    LOC_MSG_HIGH("locQmiShimHandleStartListenPositionReq: POS request is not allowed", 0, 0, 0);
    /* send QMI_LOC_EVENT_POSITION_REPORT_IND to the client */
    z_PosRptInd.sessionStatus = eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02;
    z_IndInfo.q_Id             = QMI_LOC_EVENT_POSITION_REPORT_IND_V02;
    z_IndInfo.q_MsgLen         = sizeof(z_PosRptInd);
    z_IndInfo.p_Msg            = &z_PosRptInd;
    locQmiShimSendInd(pz_ClientInfo->client_handle, &z_IndInfo);

    /* send request notification to the master client */
    locQmiSendNFWPosRequestNotificationInd(pz_ClientInfo, eQMI_LOC_REJECTED_V02, FALSE);
    return v_RetVal;
  }

  pz_ClientInfo->v_PositionListener = TRUE;
  v_RetVal = TRUE;

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
*  Message handler to handle QMI_LOC_STOP_LISTEN_POSITION_REQ message 
*
* @param pz_ClientInfo LocAPI client handle
* @param pz_Req        QMI request
* @param pz_Resp       QMI response
*
* @return TRUE if the message has been handled successfully, else
*         FALSE
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimHandleStopListenPositionReq
(
   loc_client_info_s_type *pz_ClientInfo,
   qmiLocStopListenPositionReqMsgT_v02 *pz_Req,
   qmiLocGenRespMsgT_v02 *pz_Resp
   )
{
  boolean                                 v_RetVal = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pz_Resp)
  {
    LOC_MSG_ERROR("locQmiShimHandleStopListenPositionReq: pResp is NULL \n", 0, 0, 0);
    return v_RetVal;
  }

  if ((NULL == pz_ClientInfo) || (NULL == pz_Req))
  {
    /** invalid parameters */
    pz_Resp->resp.result = QMI_RESULT_FAILURE_V01;
    pz_Resp->resp.error = QMI_ERR_INVALID_ARG_V01;
    LOC_MSG_ERROR("locQmiShimHandleStopListenPositionReq: Invalid params\n", 0, 0, 0);
    return v_RetVal;
  }

  pz_ClientInfo->v_PositionListener = FALSE;
  v_RetVal = TRUE;

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Message handler to handle all new APIs added after Loc-Api 1.x. The 
expectation from these APIs is that no translation is required and they 
will handle the QMI-LOC data types directly.

@param[out]  pz_CltInfo            Client for whom this message is intended
@param[in]   p_MsgInfo             Information regarding to the message
@param[out]  pe_QmiErr             if Failure, this will be populated to 
                                identify the reason for error

@retval    TRUE                   Message successfully handled
@retval    FALSE                  Message could not be handled successfully
*/ /* ------------------------------------------------------------------------*/
static boolean locQmiShimRouteMessages
(
   locQmiShimCltInfoStructT               *pz_CltInfo,
   const locQmiShimMsgInfoStructT         *pz_MsgInfo,
   qmi_error_type_v01                     *pe_QmiErr
   )
{
  boolean                     v_RetVal = TRUE;
  qmiLocGenRespMsgT_v02       z_QmiResp = { 0 };
  loc_client_info_s_type *pz_LocClientPtr = NULL;

  if (NULL == pz_CltInfo || NULL == pz_MsgInfo || NULL == pe_QmiErr)
  {
    LOC_MSG_ERROR("locQmiShimRouteMessages NULL parameter",
                  0, 0, 0);
    return FALSE;
  }

  pz_LocClientPtr = locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

  if (NULL == pz_LocClientPtr)
  {
    LOC_MSG_ERROR("locQmiShimRouteMessages NULL pz_LocClientPtr", 0, 0, 0);
    return FALSE;
  }

  /* Check for master client and cmd restriction */
  if (locClnt_IsCommandAllowed(pz_CltInfo->l_LocClientHandle, pz_MsgInfo->q_Id) == FALSE)
  {
    LOC_MW_CLIENT_MSG_HIGH(pz_LocClientPtr, "locQmiShimRouteMessages: client %d msg %lu not allowed",
                           pz_CltInfo->l_LocClientHandle, pz_MsgInfo->q_Id, 0);
    *pe_QmiErr = QMI_ERR_NO_PERMISSION_V01;
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Not doing check of inputs as this is called from within the module,
     and this will be called with proper parameters */

  switch (pz_MsgInfo->q_Id)
  {
    case QMI_LOC_SET_SPI_STATUS_REQ_V02:
      {
        (void)loc_qmiloc_set_spi_status_req(
           pz_LocClientPtr,
           (qmiLocSetSpiStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);

      }
      break;

    case QMI_LOC_INJECT_SENSOR_DATA_REQ_V02:
      {
        (void)loc_qmiloc_inject_sensor_data_req(
           pz_LocClientPtr,
           (qmiLocInjectSensorDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_TIME_SYNC_DATA_REQ_V02:
      {
        (void)loc_qmiloc_inject_time_sync_data_req(
           pz_LocClientPtr,
           (qmiLocInjectTimeSyncDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_CRADLE_MOUNT_CONFIG_REQ_V02:
      {
        (void)loc_qmiloc_set_cradle_mount_config_req(
           pz_LocClientPtr,
           (qmiLocSetCradleMountConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_CRADLE_MOUNT_CONFIG_REQ_V02:
      {
        (void)loc_qmiloc_get_cradle_mount_config_req(
           pz_LocClientPtr,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_EXTERNAL_POWER_CONFIG_REQ_V02:
      {
        cgps_BatteryChargerStatusEnumType e_ExtChargerStatus;
        qmiLocSetExternalPowerConfigReqMsgT_v02 *p_ConfigMsg;
        qmiLocExternalPowerConfigEnumT_v02 externalPowerState;

        (void)loc_qmiloc_set_external_power_config_req(
           pz_LocClientPtr,
           (qmiLocSetExternalPowerConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);

        /* Set the Charger status in CGPS Api */
        /* Translate the QMI Loc charger status to internal version */
        p_ConfigMsg =
           (qmiLocSetExternalPowerConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg;
        /* Send the configuration to CSM */
        locCsm_HandleSetExternalPowerConfigReq(p_ConfigMsg);
        externalPowerState = p_ConfigMsg->externalPowerState;
        switch (externalPowerState)
        {
          case eQMI_LOC_EXTERNAL_POWER_NOT_CONNECTED_V02:
            e_ExtChargerStatus = CGPS_BATTERY_CHARGER_NOT_CONNECTED;
            break;
          case eQMI_LOC_EXTERNAL_POWER_CONNECTED_V02:
            /* Assume it is connected and charged */
            e_ExtChargerStatus = CGPS_BATTERY_CHARGER_CONNECTED_CHARGED;
            break;

          case eQMI_LOC_EXTERNAL_POWER_UNKNOWN_V02:
          default:
            /* Default to charger state unknown */
            e_ExtChargerStatus = CGPS_BATTERY_CHARGER_STATE_UNKNOWN;
            break;
        }
        cgps_SetExternalChargerStatus(e_ExtChargerStatus);
      }
      break;

    case QMI_LOC_GET_EXTERNAL_POWER_CONFIG_REQ_V02:
      {
        (void)loc_qmiloc_get_external_power_config_req(
           pz_LocClientPtr,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_SENSOR_CONTROL_CONFIG_REQ_V02:
      {
        (void)loc_qmiloc_set_sensor_control_config_req(
           pz_LocClientPtr,
           (qmiLocSetSensorControlConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
        (void)loc_slim_set_sensor_control_config_req((qmiLocSetSensorControlConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg);
      }
      break;

    case QMI_LOC_GET_SENSOR_CONTROL_CONFIG_REQ_V02:
      {
        (void)loc_qmiloc_get_sensor_control_config_req(
           pz_LocClientPtr,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_SENSOR_PROPERTIES_REQ_V02:
      {
        (void)loc_qmiloc_set_sensor_properties_req(
           pz_LocClientPtr,
           (qmiLocSetSensorPropertiesReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_SENSOR_PROPERTIES_REQ_V02:
      {
        (void)loc_qmiloc_get_sensor_properties_req(
           pz_LocClientPtr,
           (qmiLocGetSensorPropertiesReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_REQ_V02:
      {
        (void)loc_qmiloc_set_sensor_performance_control_config_req(
           pz_LocClientPtr,
           (qmiLocSetSensorPerformanceControlConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_REQ_V02:
      {
        (void)loc_qmiloc_get_sensor_performance_control_config_req(
           pz_LocClientPtr,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_ADD_CIRCULAR_GEOFENCE_REQ_V02:
      {
        v_RetVal = locGeofenceAdd(
           pz_CltInfo->l_LocClientHandle,
           (qmiLocAddCircularGeofenceReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_DELETE_GEOFENCE_REQ_V02:
      {
        v_RetVal = locGeofenceDelete(
           pz_CltInfo->l_LocClientHandle,
           (qmiLocDeleteGeofenceReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_EDIT_GEOFENCE_REQ_V02:
      {
        v_RetVal = locGeofenceEdit(
           pz_CltInfo->l_LocClientHandle,
           (qmiLocEditGeofenceReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_QUERY_GEOFENCE_REQ_V02:
      {
        v_RetVal = locGeofenceQuery(
           pz_CltInfo->l_LocClientHandle,
           (qmiLocQueryGeofenceReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_SET_GEOFENCE_ENGINE_CONFIG_REQ_V02:
      {
        v_RetVal = locGeofenceSetEngineConfig(
           pz_CltInfo->l_LocClientHandle,
           (qmiLocSetGeofenceEngineConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_PEDOMETER_REPORT_REQ_V02:
      {
        v_RetVal =
           loc_slim_pedometer_report_req(
           pz_LocClientPtr,
           (qmiLocPedometerReportReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_MOTION_DATA_REQ_V02:
      {
        v_RetVal =
           loc_slim_inject_motion_data_req(
           pz_LocClientPtr,
           (qmiLocInjectMotionDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_VEHICLE_SENSOR_DATA_REQ_V02:
      {
        (void)loc_slim_inject_vehicle_sensor_data_req(pz_LocClientPtr,
                                                      (qmiLocInjectVehicleSensorDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                                      &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_WIFI_AP_DATA_REQ_V02:
      {
        v_RetVal =
           locPd_ProcessInjectWifiApDataReq(
           pz_LocClientPtr,
           (qmiLocInjectWifiApDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_GTP_CLIENT_DOWNLOADED_DATA_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        (void)loc_gtp_InjectGtpClientDownloadedDataReq(pz_LocClientPtr,
                                                       (qmiLocInjectGtpClientDownloadedDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                                       &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_UPLOAD_BEGIN_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gdt_UploadBeginStatusReq(pz_LocClientPtr,
                                     (qmiLocGdtUploadBeginStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                     &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_UPLOAD_END_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gdt_UploadEndStatusReq(pz_LocClientPtr,
                                   (qmiLocGdtUploadEndReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                   &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_TIME_ZONE_INFO_REQ_V02:
      {
        v_RetVal = locQmiShimHandleInjectTimeZoneInfoReq(
           pz_CltInfo,
           pz_MsgInfo,
           pe_QmiErr,
           &z_QmiResp);
      }
      break;


    case QMI_LOC_INJECT_APCACHE_DATA_REQ_V02:
      {
        loc_wle_InjectApCacheDataReq(pz_LocClientPtr,
                                     (qmiLocInjectApCacheDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                     &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_APDONOTCACHE_DATA_REQ_V02:
      {
        loc_wle_InjectNoApCacheDataReq(pz_LocClientPtr,
                                       (qmiLocInjectApDoNotCacheDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                       &z_QmiResp);
      }
      break;


    case QMI_LOC_GTP_AP_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gtp_ApStatusReq(pz_LocClientPtr, (qmiLocGtpApStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                            &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_DOWNLOAD_BEGIN_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gtp_DownloadBeginStatusReq(pz_LocClientPtr,
                                       (qmiLocGdtDownloadBeginStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                       &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_DOWNLOAD_READY_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gdt_DownloadReadyReq(pz_LocClientPtr,
                                 (const qmiLocGdtDownloadReadyStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                 &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_RECEIVE_DONE_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gdt_RcvDoneStatusReq(pz_LocClientPtr,
                                 (const qmiLocGdtReceiveDoneStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                 &z_QmiResp);
      }
      break;

    case QMI_LOC_GDT_DOWNLOAD_END_STATUS_REQ_V02:
      {
        pz_CltInfo->v_IsTdpClient = TRUE;
        loc_gdt_DownlodEndStatusReq(pz_LocClientPtr,
                                    (const qmiLocGdtDownloadEndStatusReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                    &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_SRN_AP_DATA_REQ_V02:
      {
        v_RetVal =
           locPd_ProcessInjectSrnApDataReq(
           pz_LocClientPtr,
           (const qmiLocInjectSrnApDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
           &z_QmiResp);
        break;
      }

    case QMI_LOC_CROWDSOURCE_MANAGER_CONTROL_REQ_V02:
      {
        v_RetVal = locCsm_ControlReq(pz_LocClientPtr,
                                     (const qmiLocCrowdSourceManagerControlReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                     &z_QmiResp);
      }
      break;

    case QMI_LOC_CROWDSOURCE_MANAGER_READ_DATA_REQ_V02:
      {
        v_RetVal = locCsm_ReadCsDataReq(pz_LocClientPtr,
                                        (const qmiLocCrowdSourceManagerReadDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                        &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_FDCL_BS_LIST_REQ_V02:
      {
        v_RetVal = loc_tle_GetFdclBsListReq(pz_LocClientPtr,
                                            (const qmiLocGetFdclBsListReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                            &z_QmiResp);
      }
      break;

    case QMI_LOC_INJECT_FDCL_DATA_REQ_V02:
      {
        v_RetVal = loc_tle_InjectFdclDataReq(pz_LocClientPtr,
                                             (const qmiLocInjectFdclDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                             &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_SUPPORTED_FEATURE_REQ_V02:
      {
        v_RetVal = locQmiShimHandleGetSupportedFeatureReq(pz_CltInfo,
                                                          pz_MsgInfo,
                                                          pe_QmiErr);
      }
      break;

    case QMI_LOC_START_LISTEN_POSITION_REQ_V02:
      {
        v_RetVal = locQmiShimHandleStartListenPositionReq(pz_LocClientPtr,
                                                          (qmiLocStartListenPositionReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                                          &z_QmiResp);
      }
      break;

    case QMI_LOC_STOP_LISTEN_POSITION_REQ_V02:
      {
        v_RetVal = locQmiShimHandleStopListenPositionReq(pz_LocClientPtr,
                                                         (qmiLocStopListenPositionReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                                         &z_QmiResp);
      }
      break;

    case QMI_LOC_GET_BS_OBS_DATA_REQ_V02:
    {
      v_RetVal = loc_tle_Fdcl2_GetBsObsDataReq(pz_LocClientPtr,
                                               (const qmiLocGetBsObsDataReqMsgT_v02 *)pz_MsgInfo->p_Msg,
                                               &z_QmiResp);
    }
    break; 
    
    default:
      {
        LOC_MSG_ERROR("This message:0x:%x has no known handler",
                      pz_MsgInfo->q_Id, 0, 0);
        v_RetVal = FALSE;
      }
      break;
  }

  (*pe_QmiErr) = z_QmiResp.resp.error;

  return v_RetVal;
}

/*============================================================================
 *                       Q C S I    C A L L B A C K S 
 * =========================================================================*/
/* -----------------------------------------------------------------------*//**
@brief
Callback function provided to QCSI, which notify the shim layer of a 
new client connecting. 

@param[in]   pz_ClientHandle   Handle of client that is connecting allocated by
                            QCSI
@param[in]   pz_ServiceCookie  Handle of service, as provided by SHIM layer to
                            QCSI during registration of service. 
@param[out]  pz_ConHandle      Connection handle allocated by shim given back
                            given to QCSI for reference. 

@retval    QMI_CSI_CB_NO_ERR        Client Connect was handled with success 
@retval    QMI_CSI_CB_CONN_REFUSED  If client connect cannot be accepted
@retval    QMI_CSI_CB_INTERNAL_ERR  For all other errors

@warning
A new client handle is accepted only if it is not present in the client 
queue.
*/ /* ------------------------------------------------------------------------*/
static qmi_csi_cb_error locQmiShimClientConnectCb
(
   void                     *p_ClientHandle,
   void                     *pz_ServiceCookie,
   void                    **pz_ConHandle
   )

{
  locQmiShimCltInfoStructT *pz_CltInfo;
  qmi_csi_cb_error e_RetVal = QMI_CSI_CB_INTERNAL_ERR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Check if the Client Handle is Non-Null, and is not a duplicate that 
     has already been used */
  if ((NULL != p_ClientHandle) &&
      (pz_ServiceCookie == &z_LocQmiShim) &&
      (NULL == q_linear_search(&z_LocQmiShim.z_ClientQ,
                               locQmiShimCompareQmiClientHandle,
                               p_ClientHandle)))
  {
    pz_CltInfo =
       (locQmiShimCltInfoStructT *)os_MemAlloc(
       sizeof(locQmiShimCltInfoStructT),
       OS_MEM_SCOPE_TASK);

    if (pz_CltInfo != NULL)
    {
      /* Memset to "0" for first use, it might have garbage and cause 
         trouble */
      memset(pz_CltInfo,
             0,
             sizeof(*pz_CltInfo));

      /* Initialize the link, return value is a pointer to the link, 
         not needed */
      (void)q_link(pz_CltInfo, &pz_CltInfo->z_Link);

      /* Store the client handle allocated by QCSI */
      pz_CltInfo->p_QmiClientHandle = p_ClientHandle;

      pz_CltInfo->v_IsTdpClient    = FALSE;

      /* Trigger Loc Open with a IOCTL Event Registration Mask, Event 
         Registration Mask is expected to be updated by client later on */
      pz_CltInfo->l_LocClientHandle = loc_open2(locQmiShimEventCb);

      if (pz_CltInfo->l_LocClientHandle == LOC_CLIENT_HANDLE_INVALID)
      {
        LOC_MSG_ERROR(" Could not accept a new client loc_open() ",
                      0, 0, 0);

        /* Free the memory in case of error */
        os_MemFree((void **)&pz_CltInfo);

        /* Connection is refused because of Loc-Api error */
        e_RetVal = QMI_CSI_CB_CONN_REFUSED;
      }
      else
      {
        /* Save the pointer to client object */
        pz_CltInfo->p_LocClient = locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

        /* Add it to the queue of clients */
        q_put(&z_LocQmiShim.z_ClientQ,
              &pz_CltInfo->z_Link);

        /* Give the Client Info as a handle to the QCSI Framework */
        (*pz_ConHandle) = pz_CltInfo;

        e_RetVal = QMI_CSI_CB_NO_ERR;
      }
    } /* if pz_CltInfo == NULL */
    else
    {
      LOC_MSG_ERROR(" Could not allocate memory for a new client",
                    0, 0, 0);
    }
  }
  else
  {
    LOC_MSG_ERROR(" Invalid input parameters rejecting the connect ",
                  0, 0, 0);
  }
  return e_RetVal;
}



/* -----------------------------------------------------------------------*//**
@brief
Callback function provided to QCSI, which notifies the shim layer of an  
existing client disconnecting. At this point loc-close is called to 
release the client handle with loc-api

@param[in]   pz_ConHandle     QMI-Handle of client that is Disconnecting. 
@param[in]   pz_ServiceCookie  Handle of service, as provided by SHIM layer to
                            QCSI during registration of service. 

*/ /* ------------------------------------------------------------------------*/
static void  locQmiShimClientDisconnectCb
(
   void                      *pz_ConHandle,
   void                      *pz_ServiceCookie
   )
{
  locQmiShimCltInfoStructT *pz_QmiShimCltInfo = NULL;
  LOC_MSG_HIGH("locQmiShimClientDisconnectCb : locQmiShimClnt 0x%x", pz_ConHandle, 0, 0);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL != pz_ConHandle)
  {
    /* Find the Connection Handle in the queue */
    pz_QmiShimCltInfo =
       (locQmiShimCltInfoStructT *)q_linear_search(&z_LocQmiShim.z_ClientQ,
                                                   locQmiShimCompareConnectionInfoHandle,
                                                   pz_ConHandle);
    if (NULL != pz_QmiShimCltInfo)
    {
      LOC_MSG_HIGH("locQmiShimClientDisconnectCb : ClientHandle = %d",
                   pz_QmiShimCltInfo->l_LocClientHandle, 0, 0);

      /* If the client registered for Geofence indication, purge
      geofences. ASSUMPTION: Only one client shall register for
      geofences and when this client deregisters or is killed, 
      all geofences in the system shall be deleted */
      if ((pz_QmiShimCltInfo->t_EventMask & QMI_LOC_EVENT_MASK_GEOFENCE_BREACH_NOTIFICATION_V02) ||
          (pz_QmiShimCltInfo->t_EventMask & QMI_LOC_EVENT_MASK_GEOFENCE_BATCH_BREACH_NOTIFICATION_V02))
      {
        /* no error checking done, errors are logged inside the
        function */
        locGeofencePurge();
      }

      if (LOC_API_SUCCESS != loc_close(pz_QmiShimCltInfo->l_LocClientHandle))
      {
        LOC_MSG_ERROR("loc_close returned error", 0, 0, 0);
      }

      /* Remove from Queue */
      if (TRUE != q_delete_ext(&z_LocQmiShim.z_ClientQ,
                               &pz_QmiShimCltInfo->z_Link))
      {
        LOC_MSG_ERROR("Could not delete the client info from queue ", 0, 0, 0);
      }

      /* Free the memory in case of error */
      os_MemFree((void **)&pz_QmiShimCltInfo);

    }
  }
}

/* -----------------------------------------------------------------------*//**
@brief
This callback is invoked when the infrastructure receives an incoming 
message. The infrastructure decodes the data and gives it to the services 

@param[in]  pz_ConHandle           Service handle used by the infrastructure
                                to communicate to each individual service.
@param[in]  z_ReqHandle            Opaque handle provided by the infrastructure
                                to specify a particular transaction and
                                message.

@param[in]  i_MsgId                Message Id pertaining to a particular
                                message. 
@param[in]  pz_Msg                 C struct with the decoded data.
@param[in]  i_MsgLen               Length of the c struct.
@param[in]  pz_ServiceCookie         Service specific data.Service cookie is 
                                registered with the infrastructure during 
                                service registration(qmi_csi_register).


@retval    QMI_CSI_NO_ERR          Success
@retval    QMI_CSI_CB_INTERNAL_ERR Error, Could not handle the message
*/ /* ------------------------------------------------------------------------*/
static qmi_csi_cb_error locQmiShimHandleRequestCb
(
   void                    *pz_ConHandle,
   qmi_req_handle           z_ReqHandle,
   uint32                   q_MsgId,
   void                    *pz_Msg,
   uint32                   q_MsgLen,
   void                    *pz_ServiceCookie
   )
{
  locQmiShimMsgInfoStructT        z_MsgInfo;
  qmiLocGenRespMsgT_v02           z_QmiLocResp;
  qmi_csi_cb_error                rc = QMI_CSI_CB_INTERNAL_ERR;
  boolean                         v_RetVal = FALSE;
  locQmiShimCltInfoStructT       *pz_CltInfo = NULL;
  qmi_error_type_v01              e_QmiErr = QMI_ERR_NONE_V01;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* NULL pz_ConHandle is handled */
  pz_CltInfo = q_linear_search(&z_LocQmiShim.z_ClientQ,
                               locQmiShimCompareConnectionInfoHandle,
                               pz_ConHandle);

  LOC_MSG_MED("Enter locQmiShimHandleRequestCb", 0, 0, 0);
  if ((NULL != pz_CltInfo) &&
      (NULL != z_ReqHandle) &&
      (((0 == q_MsgLen) && (NULL == pz_Msg)) ||
       ((0 < q_MsgLen) && (NULL != pz_Msg))) &&
      (pz_ServiceCookie == &z_LocQmiShim))
  {
    LOC_MW_CLIENT_MSG_HIGH(pz_CltInfo->p_LocClient,
                           "locQmiShimHandleRequestCb: >>> QMI_LOC Msg 0x%04x req, LocClientHandle %d z_ReqHandle 0x%x",
                           q_MsgId, pz_CltInfo->l_LocClientHandle, z_ReqHandle);
    LOC_MSG_MED("Received q_MsgLen:%d pz_Msg 0x%x pz_ServiceCookie 0x%x", q_MsgLen, pz_Msg, pz_ServiceCookie);
    z_MsgInfo.q_Id = q_MsgId;
    z_MsgInfo.p_Msg = pz_Msg;
    z_MsgInfo.q_MsgLen = q_MsgLen;

    /* Handle all Batching Requests here by sending immediate Indication after processing.
     Otherwise, handle all other Requests normally */
    switch (q_MsgId)
    {
      case QMI_LOC_GET_BATCH_SIZE_REQ_V02:
        {
          v_RetVal = locGetBatchSizeReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
      case QMI_LOC_READ_FROM_BATCH_REQ_V02:
        {
          v_RetVal = locReadFromBatchReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
      case QMI_LOC_RELEASE_BATCH_REQ_V02:
        {
          v_RetVal = locReleaseBatchReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
      case QMI_LOC_STOP_BATCHING_REQ_V02:
        {
          v_RetVal = locStopBatchingReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
        /* If it is the Start Batching Request, check the batching parameters inputed by client. */
      case QMI_LOC_START_BATCHING_REQ_V02:
        {
          v_RetVal = locStartBatchingReq(pz_CltInfo,
                                         z_ReqHandle,
                                         &z_MsgInfo);

          LOC_MSG_MED("locQmiShimHandleRequestCb: "
                      "locStartBatchingReq returned %d", v_RetVal, 0, 0);
          rc = (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
          return rc;
        }
      case QMI_LOC_START_DBT_REQ_V02:
        {
          v_RetVal = locStartDbtReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
      case QMI_LOC_STOP_DBT_REQ_V02:
        {
          v_RetVal = locStopDbtReq(pz_CltInfo, z_ReqHandle, &z_MsgInfo);
          return (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
        }
      case QMI_LOC_QUERY_AON_CONFIG_REQ_V02:
        {
          v_RetVal = locQueryAonConfigReq(pz_CltInfo,
                                          z_ReqHandle,
                                          &z_MsgInfo);

          rc = (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
          return rc;
        }
      case QMI_LOC_START_OUTDOOR_TRIP_BATCHING_REQ_V02:
        {
          v_RetVal = locStartOTBReq(pz_CltInfo,
                                    z_ReqHandle,
                                    &z_MsgInfo);
          rc = (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
          return rc;
        }

      case QMI_LOC_QUERY_OTB_ACCUMULATED_DISTANCE_REQ_V02:
        {
          v_RetVal = locOtb_QueryOTBAccumulatedDistance(pz_CltInfo->l_LocClientHandle);
          /* Do not return here, Resp sent back at the end of RequestCb() */
          break;
        }

        /* Multiple Case Statements handled by the same function */
      case QMI_LOC_SET_ENGINE_LOCK_REQ_V02:
      case QMI_LOC_GET_ENGINE_LOCK_REQ_V02:
      case QMI_LOC_SET_SBAS_CONFIG_REQ_V02:
      case QMI_LOC_GET_SBAS_CONFIG_REQ_V02:
      case QMI_LOC_SET_NMEA_TYPES_REQ_V02:
      case QMI_LOC_GET_NMEA_TYPES_REQ_V02:
      case QMI_LOC_SET_LOW_POWER_MODE_REQ_V02:
      case QMI_LOC_GET_LOW_POWER_MODE_REQ_V02:
      case QMI_LOC_SET_SERVER_REQ_V02:
      case QMI_LOC_GET_SERVER_REQ_V02:
      case QMI_LOC_DELETE_ASSIST_DATA_REQ_V02:
      case QMI_LOC_SET_XTRA_T_SESSION_CONTROL_REQ_V02:
      case QMI_LOC_GET_XTRA_T_SESSION_CONTROL_REQ_V02:
      case QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_REQ_V02:
      case QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_REQ_V02:
      case QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ_V02:
      case QMI_LOC_DELETE_SUPL_CERTIFICATE_REQ_V02:
      case QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02:
      case QMI_LOC_GET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02:
      case QMI_LOC_SET_PREMIUM_SERVICES_CONFIG_REQ_V02:
      case QMI_LOC_SET_XTRA_VERSION_CHECK_REQ_V02:
      case QMI_LOC_SET_GNSS_CONSTELL_REPORT_CONFIG_V02:
      case QMI_LOC_DELETE_GNSS_SERVICE_DATA_REQ_V02:
      case QMI_LOC_INJECT_XTRA_PCID_REQ_V02:
      case QMI_LOC_SET_BLACKLIST_SV_REQ_V02:
      case QMI_LOC_GET_BLACKLIST_SV_REQ_V02:
      case QMI_LOC_SET_CONSTELLATION_CONTROL_REQ_V02:
      case QMI_LOC_GET_CONSTELLATION_CONTROL_REQ_V02:
        {
          /* QMI_LOC request to Get/Set Server includes multiple server types processed in locPA,
             but one server type must be processed in locClient (Customer PDE Server Address) */
          if (((QMI_LOC_SET_SERVER_REQ_V02 == q_MsgId) ||
               (QMI_LOC_GET_SERVER_REQ_V02 == q_MsgId)) &&
              ((NULL != pz_Msg) &&
               ((eQMI_LOC_SERVER_TYPE_CUSTOM_PDE_V02 == ((qmiLocSetServerReqMsgT_v02 *)pz_Msg)->serverType) ||
                (eQMI_LOC_SERVER_TYPE_CUSTOM_SLP_V02 == ((qmiLocSetServerReqMsgT_v02 *)pz_Msg)->serverType)))
              )
          {
            v_RetVal = locClnt_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                                 q_MsgId, pz_Msg, q_MsgLen, &e_QmiErr);
          }
          else
          {
            /* Set up global PA module structure with QMI LOC specific info */
            v_RetVal = locPa_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                               q_MsgId, pz_Msg, q_MsgLen, &e_QmiErr);
            if ((TRUE == v_RetVal) && (NULL !=  pz_Msg))
            {
              /* Store the request to set/get PE Config for later access (setting failure mask for send indication) */
              if (QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02 == q_MsgId)
              {
                pz_CltInfo->z_PaReqUnion.z_SetPeCfg = *((qmiLocSetPositionEngineConfigParametersReqMsgT_v02 *)pz_Msg);
              }
              else if (QMI_LOC_GET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02 == q_MsgId)
              {
                pz_CltInfo->z_PaReqUnion.z_GetPeCfg = *((qmiLocGetPositionEngineConfigParametersReqMsgT_v02 *)pz_Msg);
              }
            }
          }
        }
        break;

      case QMI_LOC_REG_EVENTS_REQ_V02:
      case QMI_LOC_START_REQ_V02:
      case QMI_LOC_GET_FIX_CRITERIA_REQ_V02:
      case QMI_LOC_SET_OPERATION_MODE_REQ_V02:
      case QMI_LOC_GET_OPERATION_MODE_REQ_V02:
      case QMI_LOC_REGISTER_MASTER_CLIENT_REQ_V02:
        {
          /* Set up global PD module structure with QMI LOC specific info */
          v_RetVal = locClnt_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                               q_MsgId, pz_Msg, q_MsgLen, &e_QmiErr);
          if( (TRUE == v_RetVal) &&
              (NULL != pz_Msg) )
          {
            if (QMI_LOC_REG_EVENTS_REQ_V02 == q_MsgId)
            {
              /* Cache the EventMask for future use to filter events when generated from LocAPI */
              pz_CltInfo->t_EventMask = ((qmiLocRegEventsReqMsgT_v02 *)pz_Msg)->eventRegMask;
            }
            else if (QMI_LOC_START_REQ_V02 == q_MsgId)
            {
              qmiLocStartReqMsgT_v02 *p_StartReq = (qmiLocStartReqMsgT_v02 *)pz_Msg;

              /* Cache the session ID to be used when relaying the position Reports.
                 NOTE: When stop request is received, this session ID is not deleted,
                       as a new start request will overwrite the old session ID. */
              pz_CltInfo->u_SessionId = p_StartReq->sessionId;

              /* Reset the fix ID to zero whenever there is a start request */
              pz_CltInfo->q_FixId = 0;

              /* Check altitude assumed configuration from client */
              pz_CltInfo->e_CfgAltAssumed = eQMI_LOC_ALTITUDE_ASSUMED_IN_GNSS_SV_INFO_ENABLED_V02; /* Set default */
              if (p_StartReq->configAltitudeAssumed_valid)
              {
                pz_CltInfo->e_CfgAltAssumed = p_StartReq->configAltitudeAssumed;
              }
            }
          }
        }
        break;

      case QMI_LOC_GET_REGISTERED_EVENTS_REQ_V02:
        {
          v_RetVal = locQmiShimHandleGetRegisteredEventsReq(pz_CltInfo,
                                                            &z_MsgInfo,
                                                            &e_QmiErr);
          break;
        }

      case QMI_LOC_INJECT_POSITION_REQ_V02:
      case QMI_LOC_GET_BEST_AVAILABLE_POSITION_REQ_V02:
      case QMI_LOC_GET_AVAILABLE_WWAN_POSITION_REQ_V02:
      case QMI_LOC_INJECT_GSM_CELL_INFO_REQ_V02:
      case QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_REQ_V02:
      case QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_REQ_V02:
      case QMI_LOC_SECURE_GET_AVAILABLE_POSITION_REQ_V02:
#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
      case QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_REQ_V02:
      case QMI_LOC_INJECT_TEST_POSITION_REQ_V02:
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */
        {
          /* Set up global PD module structure with QMI LOC specific info */
          v_RetVal = locPd_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                             q_MsgId, pz_Msg, q_MsgLen, &e_QmiErr);
        }
        break;

      case QMI_LOC_GET_PREDICTED_ORBITS_DATA_SOURCE_REQ_V02:
      case QMI_LOC_GET_PREDICTED_ORBITS_DATA_VALIDITY_REQ_V02:
      case QMI_LOC_INJECT_UTC_TIME_REQ_V02:
      case QMI_LOC_INJECT_XTRA_DATA_REQ_V02:
      case QMI_LOC_QUERY_XTRA_INFO_REQ_V02:
        {
          /* Set up global XTRA module structure with QMI LOC specific info */
          v_RetVal = locXtra_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                               q_MsgId, pz_Msg, q_MsgLen);
          if (FALSE == v_RetVal)
          {
            e_QmiErr = QMI_ERR_MALFORMED_MSG_V01;
          }
        }
        break;

      case QMI_LOC_INJECT_WIFI_POSITION_REQ_V02:
      case QMI_LOC_NOTIFY_WIFI_STATUS_REQ_V02:
      case QMI_LOC_NOTIFY_WIFI_ATTACHMENT_STATUS_REQ_V02:
      case QMI_LOC_NOTIFY_WIFI_ENABLED_STATUS_REQ_V02:
        {
          /* Set up global WIFI module structure with QMI LOC specific info */
          v_RetVal = locWifi_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                               q_MsgId, pz_Msg, q_MsgLen);
          if (FALSE == v_RetVal)
          {
            e_QmiErr = QMI_ERR_MALFORMED_MSG_V01;
          }
        }
        break;

      case QMI_LOC_NI_USER_RESPONSE_REQ_V02:
        {
          v_RetVal = locNi_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                             q_MsgId, pz_Msg, q_MsgLen);
        }
        break;

      case QMI_LOC_INFORM_LOCATION_SERVER_CONN_STATUS_REQ_V02:
        {
          v_RetVal = locConn_ProcessQmiRequest(pz_CltInfo->l_LocClientHandle,
                                               q_MsgId, pz_Msg, q_MsgLen);
        }
        break;

      case QMI_LOC_STOP_REQ_V02:
        v_RetVal = locQmiShimStopReqHdlr(pz_CltInfo,
                                         &z_MsgInfo,
                                         &e_QmiErr);
        break;


      case QMI_LOC_INFORM_CLIENT_REVISION_REQ_V02:
        v_RetVal = locQmiShimHandleClientRevision(pz_CltInfo,
                                                  &z_MsgInfo,
                                                  &e_QmiErr);
        break;

      case QMI_LOC_GET_SERVICE_REVISION_REQ_V02:
        v_RetVal = locQmiShimHandleGetServRevisionReq_v02(pz_CltInfo,
                                                          &z_MsgInfo,
                                                          &e_QmiErr);
        break;

        /* Multiple Case Statements handled by the same function */
      case QMI_LOC_SET_SPI_STATUS_REQ_V02:
      case QMI_LOC_INJECT_SENSOR_DATA_REQ_V02:
      case QMI_LOC_INJECT_TIME_SYNC_DATA_REQ_V02:
      case QMI_LOC_SET_CRADLE_MOUNT_CONFIG_REQ_V02:
      case QMI_LOC_GET_CRADLE_MOUNT_CONFIG_REQ_V02:
      case QMI_LOC_SET_EXTERNAL_POWER_CONFIG_REQ_V02:
      case QMI_LOC_GET_EXTERNAL_POWER_CONFIG_REQ_V02:
      case QMI_LOC_SET_SENSOR_CONTROL_CONFIG_REQ_V02:
      case QMI_LOC_GET_SENSOR_CONTROL_CONFIG_REQ_V02:
      case QMI_LOC_SET_SENSOR_PROPERTIES_REQ_V02:
      case QMI_LOC_GET_SENSOR_PROPERTIES_REQ_V02:
      case QMI_LOC_SET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_REQ_V02:
      case QMI_LOC_GET_SENSOR_PERFORMANCE_CONTROL_CONFIGURATION_REQ_V02:
      case QMI_LOC_ADD_CIRCULAR_GEOFENCE_REQ_V02:
      case QMI_LOC_DELETE_GEOFENCE_REQ_V02:
      case QMI_LOC_EDIT_GEOFENCE_REQ_V02:
      case QMI_LOC_QUERY_GEOFENCE_REQ_V02:
      case QMI_LOC_PEDOMETER_REPORT_REQ_V02:
      case QMI_LOC_INJECT_MOTION_DATA_REQ_V02:
      case QMI_LOC_SET_GEOFENCE_ENGINE_CONFIG_REQ_V02:
      case QMI_LOC_INJECT_VEHICLE_SENSOR_DATA_REQ_V02:
      case QMI_LOC_INJECT_WIFI_AP_DATA_REQ_V02:
      case QMI_LOC_INJECT_GTP_CLIENT_DOWNLOADED_DATA_REQ_V02:
      case QMI_LOC_GDT_UPLOAD_BEGIN_STATUS_REQ_V02:
      case QMI_LOC_GDT_UPLOAD_END_REQ_V02:
      case QMI_LOC_INJECT_TIME_ZONE_INFO_REQ_V02:
      case QMI_LOC_INJECT_APCACHE_DATA_REQ_V02:
      case QMI_LOC_INJECT_APDONOTCACHE_DATA_REQ_V02:
      case QMI_LOC_GTP_AP_STATUS_REQ_V02:
      case QMI_LOC_GDT_DOWNLOAD_BEGIN_STATUS_REQ_V02:
      case QMI_LOC_GDT_DOWNLOAD_READY_STATUS_REQ_V02:
      case QMI_LOC_GDT_RECEIVE_DONE_STATUS_REQ_V02:
      case QMI_LOC_GDT_DOWNLOAD_END_STATUS_REQ_V02:
      case QMI_LOC_INJECT_SRN_AP_DATA_REQ_V02:
      case QMI_LOC_CROWDSOURCE_MANAGER_CONTROL_REQ_V02:
      case QMI_LOC_CROWDSOURCE_MANAGER_READ_DATA_REQ_V02:
      case QMI_LOC_GET_FDCL_BS_LIST_REQ_V02:
      case QMI_LOC_INJECT_FDCL_DATA_REQ_V02:
      case QMI_LOC_GET_SUPPORTED_FEATURE_REQ_V02:
      case QMI_LOC_START_LISTEN_POSITION_REQ_V02:
      case QMI_LOC_STOP_LISTEN_POSITION_REQ_V02:
      case QMI_LOC_GET_BS_OBS_DATA_REQ_V02:
        v_RetVal = locQmiShimRouteMessages(pz_CltInfo,
                                           &z_MsgInfo,
                                           &e_QmiErr);
        break;

      default:
        LOC_MSG_ERROR("locQmiShimHandleRequestCb: Not supported QMI_LOC Msg: 0x%04x",
                      q_MsgId, 0, 0);
        e_QmiErr = QMI_ERR_NOT_SUPPORTED_V01;
        break;
    }

    /* Fill and send the response back to the client */
    if (TRUE == v_RetVal)
    {
      z_QmiLocResp.resp.result = QMI_RESULT_SUCCESS_V01;
    }
    else
    {
      z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
      LOC_MW_CLIENT_MSG_ERROR(pz_CltInfo->p_LocClient,
                              "locQmiShimHandleRequestCb: Could not handle QMI_LOC request Msg 0x%04x Err 0x%x",
                              q_MsgId, e_QmiErr, 0);
    }
    z_QmiLocResp.resp.error  = e_QmiErr;

    /* Msg Id for Req and Resp are the same, hence sending response to the 
       incoming message id */
    if (QMI_CSI_NO_ERR != mqcsi_send_rsp(
          pz_CltInfo->p_QmiClientHandle,
          z_ReqHandle,
          q_MsgId,
          &z_QmiLocResp,
          sizeof(z_QmiLocResp)))
    {
      LOC_MW_CLIENT_MSG_ERROR(pz_CltInfo->p_LocClient,
                              "locQmiShimHandleRequestCb: <<< QMI_LOC Msg 0x%04x resp failed for locClient %d",
                              q_MsgId, pz_CltInfo->l_LocClientHandle, 0);
      v_RetVal = FALSE;
    }
    else
    {
      LOC_MW_CLIENT_MSG_HIGH(pz_CltInfo->p_LocClient,
                             "locQmiShimHandleRequestCb: <<< QMI_LOC Msg 0x%04x resp succeed,error:0x%04x,result:%d ",
                             q_MsgId, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
    }
  }
  else
  {
    LOC_MSG_ERROR("locQmiShimHandleRequestCb: Invalid Connection Info Handle from QCSI",
                  0, 0, 0);
  }

  rc = (v_RetVal == TRUE ? QMI_CSI_CB_NO_ERR : QMI_CSI_CB_INTERNAL_ERR);
  LOC_MSG_MED_0("Exit locQmiShimHandleRequestCb");
  return rc;
} /* handle_req */

/*****************************************************************************
 *****************************************************************************
 *
 *                   Externalized Function Definitions
 *
 *****************************************************************************
 * ***************************************************************************/

/* -----------------------------------------------------------------------*//**
@brief
This function needs to be called to initialize the QMI-Loc module. It
initializes the internal data structures of the module and registers with
SM to receive a callback letting QMI-Loc know when PDAPI is initialized. 

No other QMI related initialization, specifically no registration of QMI-Loc
service with the QMI framework is performed here.

@retval  TRUE  - Successful initialization of the module
@retval  FALSE - Module initialization was not successful

@warning
This function does not gaurd itself against being called multiple times. 
Once called it can be called again only if "cleanup" is called. 
*/ /* ------------------------------------------------------------------------*/
boolean locQmiShimInit(void)
{
  boolean             v_RetVal = TRUE;
  uint8               u_Cntr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Initialize the Service Context Information */
  memset(&z_LocQmiShim,
         0,
         sizeof(z_LocQmiShim));


  /* Initialize the queue which will hold all handles of clients and maintain 
     the mapping of QMI-Client-handles and the loc-api-client handles */
  (void)q_init(&z_LocQmiShim.z_ClientQ);

  if (FALSE == sm_RegisterApiNotifier(locQmiShimInitNotifierCb, THREAD_ID_LOC_MIDDLEWARE))
  {
    LOC_ERR_FATAL("Could not register Callback", 0, 0, 0);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
This function is called to reset and disconnect the shim layer from the 
QMI-Loc Service. After this function call, the loc-middleware task will need 
to call locQmiShimInit() to be able to communicate over the QMI-framework. 
*/ /* ------------------------------------------------------------------------*/
void locQmiShimCleanup
(
   void
   )
{
  locQmiShimCltInfoStructT *pz_CltInfo = NULL;
  qmi_csi_error             rc = QMI_CSI_INTERNAL_ERR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*  Continue Stopping only if we have a valid service handle */
  if (z_LocQmiShim.p_ServiceHandle != NULL)
  {
    /** Unregister with QSAP */
    if (z_LocQmiShim.z_QSAPUserHandle)
    {
      if (QMI_SAP_NO_ERR != qmi_sap_deregister(z_LocQmiShim.z_QSAPUserHandle))
      {
        LOC_MSG_ERROR("Could not deregister the service with QSAP",
                      0, 0, 0);
      }
    }

    /* Unregister with framework, so no more messages arrive */
    rc = mqcsi_unregister(z_LocQmiShim.p_ServiceHandle);
    if (rc != QMI_CSI_NO_ERR)
    {
      LOC_MSG_ERROR("Could not deregister the service successfully",
                    0, 0, 0);
    }

    /* Reset to initial values */
    memset(&z_LocQmiShim.p_ServiceHandle,
           0,
           sizeof(z_LocQmiShim.p_ServiceHandle));

    /* For each client call the disconnect callback. The same actions need to 
       be taken. Hence q_check() instead of q_get() */
    pz_CltInfo =
       (locQmiShimCltInfoStructT *)q_check(&z_LocQmiShim.z_ClientQ);
    while (NULL != pz_CltInfo)
    {

      /* Trigger the function that handles disconnect client callback, 
         as at this point the same functionality needs to be performed 
         NOTE: Memory is freed inside this call */
      locQmiShimClientDisconnectCb((void *)pz_CltInfo,
                                   &z_LocQmiShim);

      /* Get the next connection handle. 
         NOTE: it is q_check() not q_get() */
      pz_CltInfo =
         (locQmiShimCltInfoStructT *)q_check(&z_LocQmiShim.z_ClientQ);
    }

    if (0 != q_cnt(&z_LocQmiShim.z_ClientQ))
    {
      LOC_MSG_ERROR("Cleanup was not proper, still some clients dangling ",
                    0, 0, 0);
    }

    /* Destroy the queue. It is expected to call Init() again before using this 
       module */
    q_destroy(&z_LocQmiShim.z_ClientQ);

  }
}

/* -----------------------------------------------------------------------*//**
@brief
This function is called by the Service owner indicating that there is a 
QMI-Event that is queued up in QCSI that needs to be handled.
*/ /* ------------------------------------------------------------------------*/
void locQmiShimHandleEvent(void)
{
#ifndef __GRIFFON__
  qmi_csi_os_params   z_OsParams;
#endif /* __GRIFFON__ */
  qmi_csi_error       e_Err;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the TCB for the task in which initialization of shim was called 
     Assumption is that this function will be called in the service provider's
     context, and hence os_Self() should return the service provider's 
     context. If called in someone else's context, qmi_csi_handle_event() 
     should return an error */

#ifndef __GRIFFON__
  z_OsParams.tcb = os_Self();
  z_OsParams.sig = LOC_QMI_SHIM_SIGNAL;
#endif /* __GRIFFON__ */

  LOC_MSG_HIGH("Handle QMI Event", 0, 0, 0);
  e_Err = mqcsi_handle_event(z_LocQmiShim.p_ServiceHandle,
#ifdef __GRIFFON__
                             &g_z_OsParams
#else /* __GRIFFON__ */
                             & z_OsParams
#endif /* __GRIFFON__ */
                             );

  if (QMI_CSI_NO_ERR != e_Err)
  {
    LOC_MSG_ERROR("Could not handle the event properly Err:%d ",
                  e_Err, 0, 0);
  }
#ifdef __GRIFFON__

  //Unblock the "select" thread, ok to "select" the UDP socket now
  UPOS_ThreadActivate(&upos_thread);
#endif  /* __GRIFFON__ */
}

/* -----------------------------------------------------------------------*//**
@brief
A helper utility to read GNSS Lock control NV item.

@param[in]   void

@retval      void
*/ /* ------------------------------------------------------------------------*/
static void locQmiReadGnssLockCtrlNv(void)
{
  /* Represents OEM controls to config GNSS session request and notification to HLOS
     bit0 = CP_NILR_OEM_CTRL         = 0 = ACCEPT_ALWAYS
     bit1 = SUPL_NI_OEM_CTRL         = 0 = ACCEPT_ALWAYS
     bit2 = NFW_OEM_CTRL             = 0 = ACCEPT_ALWAYS
     bit3 = CP_NILR_NOTIFY_OEM_CTRL  = 0 = DONT_ALLOW
     bit4 = NFW_NOTIFY_OEM_CTRL      = 0 = DONT_ALLOW */
  uint32 q_GnssLockCtrlForOem = 0;
  pdsm_gps_lock_e_type gps_lock = PDSM_GPS_LOCK_MIN;

  /* Location Privacy NV 74235 reading */
  (void)gps_nv_efs_reg_item_read(
     GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM,
     sizeof(q_GnssLockCtrlForOem),
     &q_GnssLockCtrlForOem
     );
  loc_middleware_data.v_NvAllowNfwPosReq =\
     (q_GnssLockCtrlForOem & 1 << 2) == 0 ? TRUE : FALSE;
  loc_middleware_data.v_NvAllowNfwPosReqNotification =\
     (q_GnssLockCtrlForOem & 1 << 4) == 0 ? FALSE : TRUE;
  LOC_MSG_MED("locQmi_ProcessCmdRequest NV value = 0x%x, PosRequest=%d, Notification=%d",
              q_GnssLockCtrlForOem, loc_middleware_data.v_NvAllowNfwPosReq,
              loc_middleware_data.v_NvAllowNfwPosReqNotification);

  /* NV 452 gps1_lock reading */
  gps_lock = pdsm_get_gps1_lock();
  switch (gps_lock)
  {
    case PDSM_GPS_LOCK_NONE:
      loc_middleware_data.v_AfwAllowAfwPosReq = TRUE;
      loc_middleware_data.v_AfwAllowNfwPosReq = TRUE;
      break;
    case PDSM_GPS_LOCK_MI:
      loc_middleware_data.v_AfwAllowAfwPosReq = FALSE;
      loc_middleware_data.v_AfwAllowNfwPosReq = TRUE;
      break;
    case PDSM_GPS_LOCK_MT:
      loc_middleware_data.v_AfwAllowAfwPosReq = TRUE;
      loc_middleware_data.v_AfwAllowNfwPosReq = FALSE;
      break;
    case PDSM_GPS_LOCK_ALL:
      loc_middleware_data.v_AfwAllowAfwPosReq = FALSE;
      loc_middleware_data.v_AfwAllowNfwPosReq = FALSE;
      break;
    default:
      //both are set to TRUE in loc_middlware_module_init()
      break;
  }
}
/* -----------------------------------------------------------------------*//**
@brief
Function to process the locQmiShimInitNotifierCb. This function checks with SM
which service is enabled, QMI-LOC/QMI-PDS/etc. and if QMI-LOC is enabled, 
registers the QMI-LOC service with QCSI.
*/ /* ------------------------------------------------------------------------*/
void locQmi_ProcessCmdRequest
(
   const LocMW_LocCmdStructT *pz_Cmd
   )
{
#ifndef __GRIFFON__
  qmi_csi_os_params   z_OsParams;
#endif /* __GRIFFON__ */
  qmi_csi_error                rc = QMI_CSI_INTERNAL_ERR;
  qmi_idl_service_object_type  z_QmiLocServiceObject = NULL;
  uint32_t                     q_SvcId;
  pdsm_QMIServiceConfig        qmi_config;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Read in NV item before QMI init */
  if (FALSE == pdsm_read_qmi_services_config(&qmi_config))
  {
    LOC_ERR_FATAL("Could not read QMI Config!!", 0, 0, 0);
    return;
  }

  locQmiReadGnssLockCtrlNv();

  if (qmi_config.enableMask & PDSM_QMI_CONFIG_ENABLE_LOC)
  {
    loc_middleware_data.qmi_loc_enabled = TRUE;
  }
  else
  {
    /* Do not register with QMI framework, as QMI-LOC disabled in NV */
    LOC_MSG_HIGH("QMI-LOC Disabled!!", 0, 0, 0);
    return;
  }
#ifdef __GRIFFON__
  /* start the UDP transport */
  qmi_csi_xport_start(&udp_ops, NULL);

#else /* __GRIFFON__ */
  /* Get the TCB for the task in which initialization of shim was called */
  z_OsParams.tcb = os_Self();
  z_OsParams.sig = LOC_QMI_SHIM_SIGNAL;
#endif /* __GRIFFON__ */

  z_QmiLocServiceObject =  loc_get_service_object_v02();

  if (QMI_IDL_LIB_NO_ERR == qmi_idl_get_service_id(z_QmiLocServiceObject,
                                                   &q_SvcId))
  {

    /* Register with QMI SI for logging of supported interfaces */
    qmi_si_register_object(z_QmiLocServiceObject,
                           0,
                           loc_get_service_impl_v02());

#ifdef __GRIFFON__
    printf("QMI-LOC trying to register with QCSI\n");
#endif /* __GRIFFON__ */
    /* Register the service with QCSI Framework */
    rc = mqcsi_register(q_SvcId,
                        LOC_V02_IDL_MAJOR_VERS,
                        LOC_V02_IDL_MINOR_VERS,
                        z_QmiLocServiceObject,
                        locQmiShimClientConnectCb,
                        locQmiShimClientDisconnectCb,
                        locQmiShimHandleRequestCb,
                        (void *)&z_LocQmiShim,
#ifdef __GRIFFON__
                        &g_z_OsParams,
#else /* __GRIFFON__ */
                        &z_OsParams,
#endif /* __GRIFFON__ */
                        &(z_LocQmiShim.p_ServiceHandle));


    if (QMI_CSI_NO_ERR == rc)
    {
      qmi_sap_error sapErr;
      LOC_MSG_HIGH("QMI-LOC Registered Successfully with QCSI", 0, 0, 0);

      /** Register the service with QSAP*/
      sapErr = qmi_sap_register(z_QmiLocServiceObject,
                                NULL,
                                &(z_LocQmiShim.z_QSAPUserHandle));

      if (QMI_SAP_NO_ERR == sapErr)
      {
        LOC_MSG_HIGH("QMI-LOC Registered Successfully with QSAP", 0, 0, 0);
      }
      else
      {
        LOC_MSG_ERROR("Was not able to register successfully with QSAP Error:%d",
                      sapErr, 0, 0);
      }
#ifdef __GRIFFON__
      printf("QMI-LOC Registered Successfully with QCSI\n");

      // create a thread to read this UDP socket
      if (OK == UPOS_ThreadCreate(&upos_thread, UPOS_FindTask(NULL),
                                  read_udp_and_signal, 1, 1, 8192, "uposThread"))
      {
        printf("udp xport_open: created UPOS thread\n");
        //Activate the Thread
        UPOS_ThreadActivate(&upos_thread);
      }
      else
      {
        printf("udp xport_open: Could not create UPOS thread\n");
      }
#endif /* __GRIFFON__ */
    }
    else
    {
#ifdef __GRIFFON__
      printf("Was not able to register successfully with QCSI Error:%d\n", rc);
#endif /* __GRIFFON__ */
      LOC_MSG_ERROR("Was not able to register successfully with QCSI Error:%d",
                    rc, 0, 0);
    }
  }
  else
  {
#ifdef __GRIFFON__
    printf("Was not able to get Service ID: %d\n", rc);
#endif /* __GRIFFON__ */
    LOC_MSG_ERROR("Was not able to get Service ID",
                  0, 0, 0);
  }

}

/* -----------------------------------------------------------------------*//**
@brief
This function is to be called by the module that intends to send out an 
indication to the client of the QMI-LOC, Service. 

@return TRUE    The indication was successfully sent over QCSI
@return FALSE   The indication was not successfully sent over QCSI

@warning
This function assumes that it is called in the context of the service 
providers task. ( Loc-MW task )
*/ /* ------------------------------------------------------------------------*/
boolean locQmiShimSendInd
(
   loc_client_handle_type                  l_LocClientHandle,
   const locQmiShimIndInfoStructT         *pz_IndInfo
   )
{
  boolean v_RetVal = TRUE;
  locQmiShimCltInfoStructT   *pz_CltInfo = NULL;
  qmi_csi_error               e_QmiErr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  pz_CltInfo = q_linear_search(&z_LocQmiShim.z_ClientQ,
                               locQmiShimCompareLocApiHandle,
                               (void *)l_LocClientHandle);
  if ((NULL != pz_CltInfo) &&
      (NULL != pz_IndInfo))
  {
    e_QmiErr = mqcsi_send_ind(pz_CltInfo->p_QmiClientHandle,
                              pz_IndInfo->q_Id,
                              pz_IndInfo->p_Msg,
                              pz_IndInfo->q_MsgLen);

    if (QMI_CSI_NO_ERR != e_QmiErr)
    {
      /* Could not send an event out, some error in QCSI framework */
      LOC_MW_CLIENT_MSG_ERROR(pz_CltInfo->p_LocClient,
                              "locQmiShimSendInd: <<< QMI_LOC Msg 0x%04x ind failed for locClient %d QMI_CSI_NO_ERR=%d",
                              pz_IndInfo->q_Id, l_LocClientHandle, e_QmiErr);
      v_RetVal = FALSE;
    }
    else
    {
      LOC_MW_CLIENT_MSG_HIGH(pz_CltInfo->p_LocClient,
                             "locQmiShimSendInd: <<< QMI_LOC Msg 0x%04x ind succeed for LocClient %d",
                             pz_IndInfo->q_Id, l_LocClientHandle, 0);
    }
  }
  else
  {
    LOC_MSG_ERROR("locQmiShimSendInd: Could not find the QMI-client for LocClient %d",
                  l_LocClientHandle, 0, 0);
    v_RetVal = FALSE;
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
This function is to be called by the module that intends to broadcast an 
indication to multiple clients of the QMI-LOC, Service. 

@return TRUE    The indication was successfully sent over QCSI
@return FALSE   The indication was not successfully sent over QCSI

@warning
This function assumes that it is called in the context of the service 
providers task. ( Loc-MW task )
*/ /* ------------------------------------------------------------------------*/
boolean locQmiShimBroadcastInd
(
   uint64                           t_EventMask,
   const locQmiShimIndInfoStructT  *pz_IndInfo
   )
{
  boolean v_RetVal = TRUE;
  locQmiShimCltInfoStructT   *pz_CltInfo = NULL;
  uint32 error_code = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL != pz_IndInfo)
  {
    LOC_MSG_HIGH("locQmiShimBroadcastInd QMI_LOC_EVENT_MASK:0x%08x, QMI_LOC Msg 0x%04x, len:%u",
                 (uint32)(t_EventMask & 0x00000000FFFFFFFFULL), pz_IndInfo->q_Id, pz_IndInfo->q_MsgLen);
    for (pz_CltInfo = (locQmiShimCltInfoStructT *)q_check(&z_LocQmiShim.z_ClientQ);
         pz_CltInfo != NULL;
         pz_CltInfo = (locQmiShimCltInfoStructT *)q_next(&(z_LocQmiShim.z_ClientQ), &(pz_CltInfo->z_Link)))
    {
      if (0 != (t_EventMask & pz_CltInfo->t_EventMask))
      {
        /* this client has registered for something matching this event 
           (mask) */
        if (TRUE != locQmiShimSendInd(pz_CltInfo->l_LocClientHandle,
                                      pz_IndInfo))
        {
          error_code = 1;
        }
      }
    }
  }
  else
  {
    error_code = 2;
  }

  if (0 != error_code)
  {
    v_RetVal = FALSE;
    LOC_MSG_ERROR("locQmiShimBroadcastInd error: %u", error_code, 0, 0);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Untility function to send Response mesasge to Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the response message
@param[out]  pResp                 Pointer to send the response
@param[in]   resp_value            Response status being sent to control point
@param[in]   e_QmiErr              QMI Error type being sent to control point


@retval    TRUE           Sending Response to control point success
@retval    FALSE          Sending Response to control point fail
*/ /* ------------------------------------------------------------------------*/
static boolean locBatchingOperationSendResp(locQmiShimCltInfoStructT    *pz_CltInfo,
                                            qmi_req_handle              z_ReqHandle,
                                            locQmiShimMsgInfoStructT    *pz_MsgInfo,
                                            boolean                     resp_value,
                                            qmi_error_type_v01          e_QmiErr)
{
  boolean v_RetVal = TRUE;
  qmiLocGenRespMsgT_v02           z_QmiLocResp;

  memset(&z_QmiLocResp, 0, sizeof(z_QmiLocResp));

  /* Fill and send the response back to the client */
  if (TRUE == resp_value)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_SUCCESS_V01;
    z_QmiLocResp.resp.error = QMI_ERR_NONE_V01;
  }
  else
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error  = e_QmiErr;
  }

  /* Msg Id for Req and Resp are the same, hence sending response to the 
     incoming message id */
  if (QMI_CSI_NO_ERR != mqcsi_send_rsp(
        pz_CltInfo->p_QmiClientHandle,
        z_ReqHandle,
        pz_MsgInfo->q_Id,
        &z_QmiLocResp,
        sizeof(qmiLocGenRespMsgT_v02)))
  {
    LOC_MSG_ERROR("locBatchingOperationSendResp: <<< Send QMI_RESP 0X%04x failed for LocClient %d",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
    v_RetVal = FALSE;
  }
  else
  {
    LOC_MSG_MED("locBatchingOperationSendResp: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d ",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle GetBatchSize Request from Control point 

@param[in]   pz_QmiShimCltInfo     Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the get batch size request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locGetBatchSizeReq(locQmiShimCltInfoStructT    *pz_QmiShimCltInfo,
                                  qmi_req_handle              z_ReqHandle,
                                  locQmiShimMsgInfoStructT    *pz_MsgInfo)
{
  qmiLocGetBatchSizeIndMsgT_v02 z_GetBatchSizeInd = { 0 };
  boolean                       v_RetVal = TRUE;
  boolean                       v_RespResult = TRUE; /* default TRUE */
  qmi_error_type_v01            e_RespErr = QMI_ERR_NONE_V01; /* default NO Error */
  boolean                       v_Otb = FALSE; /* outdoor trip batching */
  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_QmiShimCltInfo->l_LocClientHandle);
  qmiLocGetBatchSizeReqMsgT_v02 *pz_GetBatchSizeReq = (qmiLocGetBatchSizeReqMsgT_v02 *)pz_MsgInfo->p_Msg;

  if ((NULL == pz_GetBatchSizeReq) || (NULL == pz_LocClient))
  {
    LOC_MSG_ERROR("locGetBatchSizeReq: NULL param(s)\n",
                  0, 0, 0);

    v_RespResult = FALSE;
    e_RespErr = QMI_ERR_INTERNAL_V01;
  }
  else if (pz_GetBatchSizeReq->batchSize <= 0)
  {
    /* check if batch size requested is valid */
    LOC_MSG_ERROR("locGetBatchSizeReq: invalid batching size %d",
                  pz_GetBatchSizeReq->batchSize, 0, 0);
    v_RespResult = FALSE;
    e_RespErr = QMI_ERR_INVALID_ARG_V01;
  }
  else if (TRUE == pz_GetBatchSizeReq->batchType_valid)
  {
    if (eQMI_LOC_LOCATION_BATCHING_V02 == pz_GetBatchSizeReq->batchType)
    {
      if (TRUE == pz_LocClient->batching_info.is_batching_client)
      {
        /* Memory is already allocated TBB client */
        LOC_MSG_ERROR("locGetBatchSizeReq: memory already allocated for TBB client",
                      0, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_DEVICE_IN_USE_V01;
      }
    }
    else if (eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02 == pz_GetBatchSizeReq->batchType)
    {
      v_Otb = TRUE;
      if (TRUE == pz_LocClient->z_OtbInfo.v_OtbClient)
      {
        /* Memory is already allocated for OTB client */
        LOC_MSG_ERROR("locGetBatchSizeReq: memory already allocated for OTB client",
                      0, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_DEVICE_IN_USE_V01;
      }
    }
    else
    {
      /* not supported batching type */
      LOC_MSG_ERROR("locGetBatchSizeReq: not supported batching type %d",
                    pz_GetBatchSizeReq->batchType, 0, 0);
      v_RespResult = FALSE;
      e_RespErr = QMI_ERR_INVALID_ARG_V01;
    }
  }
  /* if batchType_valid is not set, default is TBB */
  else
  {
    if (TRUE == pz_LocClient->batching_info.is_batching_client)
    {
      /* Memory is already allocated for TBB client */
      LOC_MSG_ERROR("locGetBatchSizeReq: memory already allocated for default TBB client",
                    0, 0, 0);
      v_RespResult = FALSE;
      e_RespErr = QMI_ERR_DEVICE_IN_USE_V01;
    }
  }

  /* send response first */
  v_RetVal = locBatchingOperationSendResp(pz_QmiShimCltInfo,
                                          z_ReqHandle,
                                          pz_MsgInfo,
                                          v_RespResult,
                                          e_RespErr);

  /* process the request and send indication only if RESP is SUCCESS */
  if ((TRUE == v_RespResult) && (TRUE == v_RetVal))
  {
    locQmiShimIndInfoStructT  shimIndInfo = { 0 };

    if (FALSE == v_Otb)
    {
      locGetBatchSizeHandleAllocate(pz_LocClient,
                                    &z_GetBatchSizeInd,
                                    pz_GetBatchSizeReq->batchSize,
                                    pz_GetBatchSizeReq->transactionId);
    }
    else
    {
      //allocate OTB buffer
      locOtb_HandleGetBatchSize(pz_LocClient,
                                &z_GetBatchSizeInd,
                                pz_GetBatchSizeReq->batchSize,
                                pz_GetBatchSizeReq->transactionId);
    }

    /* send indication */
    shimIndInfo.p_Msg = &z_GetBatchSizeInd;
    shimIndInfo.q_MsgLen = sizeof(z_GetBatchSizeInd);
    shimIndInfo.q_Id = QMI_LOC_GET_BATCH_SIZE_IND_V02;

    v_RetVal = locQmiShimSendInd(pz_LocClient->client_handle,
                                 &shimIndInfo);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle ReadFromBatch Request from Control point 

@param[in]   pz_QmiShimCltInfo     QmiShim Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                 to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the ReadFromBatch request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locReadFromBatchReq(locQmiShimCltInfoStructT     *pz_QmiShimCltInfo,
                                   qmi_req_handle               z_ReqHandle,
                                   locQmiShimMsgInfoStructT     *pz_MsgInfo)
{
  qmiLocReadFromBatchIndMsgT_v02   z_ReadFromBatchInd = { 0 };
  boolean                          v_RetVal = TRUE;
  boolean                          v_RespResult = TRUE; /* default TRUE */
  qmi_error_type_v01               e_RespErr = QMI_ERR_NONE_V01; /* default NO Error */
  boolean                          v_Otb = FALSE; /* outdoor trip batching */
  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_QmiShimCltInfo->l_LocClientHandle);
  qmiLocReadFromBatchReqMsgT_v02 *pz_ReadFromBatchReq =
     (qmiLocReadFromBatchReqMsgT_v02 *)(pz_MsgInfo->p_Msg);
  locQmiShimIndInfoStructT         z_ShimIndInfo = { 0 };

  if ((NULL == pz_ReadFromBatchReq) || (NULL == pz_LocClient))
  {
    LOC_MSG_ERROR("locReadFromBatchReq: NULL param(s)",
                  0, 0, 0);

    v_RespResult = FALSE;
    e_RespErr = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }
  else if (pz_ReadFromBatchReq->numberOfEntries > QMI_LOC_READ_FROM_BATCH_MAX_SIZE_V02)
  {
    LOC_MSG_ERROR("locReadFromBatchReq: invalid numberOfEntries %d",
                  pz_ReadFromBatchReq->numberOfEntries, 0, 0);
    v_RespResult = FALSE;
    e_RespErr = QMI_ERR_MALFORMED_MSG_V01;
  }
  else if (TRUE == pz_ReadFromBatchReq->batchType_valid)
  {
    if (eQMI_LOC_LOCATION_BATCHING_V02 == pz_ReadFromBatchReq->batchType)
    {
      /* nothing to do */
    }
    else if (eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02 == pz_ReadFromBatchReq->batchType)
    {
      v_Otb = TRUE;
    }
    else
    {
      LOC_MSG_ERROR("locReadFromBatchReq: invalid batchType %d",
                    pz_ReadFromBatchReq->batchType, 0, 0);
      v_RespResult = FALSE;
      e_RespErr = QMI_ERR_MALFORMED_MSG_V01;
    }
  }

  if (TRUE == v_RespResult)
  {
    if ((FALSE == v_Otb) &&
        (FALSE == pz_LocClient->batching_info.is_batching_client))
    {
      LOC_MSG_ERROR("locReadFromBatchReq: LocClient %d is not a batching client",
                    pz_LocClient->client_handle, 0, 0);
      v_RespResult = FALSE;
      e_RespErr = QMI_ERR_INVALID_HANDLE_V01;
    }
    else if ((TRUE == v_Otb) &&
             (FALSE == pz_LocClient->z_OtbInfo.v_OtbClient))
    {
      LOC_MSG_ERROR("locReadFromBatchReq: LocClient %d is not a outdoor trip batching client",
                    pz_LocClient->client_handle, 0, 0);
      v_RespResult = FALSE;
      e_RespErr = QMI_ERR_INVALID_HANDLE_V01;
    }
  }

  /* send response first */
  v_RetVal = locBatchingOperationSendResp(pz_QmiShimCltInfo,
                                          z_ReqHandle,
                                          pz_MsgInfo,
                                          v_RespResult,
                                          e_RespErr);

  /* process the request and send indication only if RESP is SUCCESS */
  if ((TRUE == v_RespResult) && (TRUE == v_RetVal))
  {
    if (FALSE == v_Otb)
    {
      v_RetVal = loc_handle_batching_read(pz_LocClient, pz_ReadFromBatchReq, &z_ReadFromBatchInd);
    }
    else
    {
      v_RetVal = locOtb_BtchingRead(pz_LocClient, pz_ReadFromBatchReq, &z_ReadFromBatchInd);
    }
  }

  /* send indication */
  z_ShimIndInfo.p_Msg = &z_ReadFromBatchInd;
  z_ShimIndInfo.q_MsgLen = sizeof(z_ReadFromBatchInd);
  z_ShimIndInfo.q_Id = QMI_LOC_READ_FROM_BATCH_IND_V02;

  v_RetVal = locQmiShimSendInd(pz_LocClient->client_handle,
                               &z_ShimIndInfo);

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Utility function to send release batch Indication to control point.

@param[in]   client_ptr            Client Info Handle 
@param[out]  p_releaseBatchInd     Release Batch Indication message to control point
@param[in]   request_status        Status from buffer free operation
@param[in]   reqTransactionId      Transaction Id in request message by control point 


@retval    TRUE           locQmiShimSendInd is success
@retval    FALSE          locQmiShimSendInd is failed
*/ /* ------------------------------------------------------------------------*/
static boolean loc_send_release_batch_ind
(
   loc_client_info_s_type            *pz_LocClient,
   qmiLocReleaseBatchIndMsgT_v02 *pz_ReleaseBatchInd,
   boolean                           v_RetVal,
   uint32                            q_TransactionId,
   qmiLocBatchingTypeEnumT_v02       e_BatchType
   )
{
  locQmiShimIndInfoStructT              shimIndInfo;

  /* invalid read size, return failure */
  if (v_RetVal == FALSE)
  {
    pz_ReleaseBatchInd->status = eQMI_LOC_GENERAL_FAILURE_V02;
  }
  else
  {
    pz_ReleaseBatchInd->status = eQMI_LOC_SUCCESS_V02;
  }

  /* update transaction id in indication - mandatory field */
  pz_ReleaseBatchInd->transactionId = q_TransactionId;

  shimIndInfo.p_Msg = pz_ReleaseBatchInd;
  shimIndInfo.q_MsgLen = sizeof(*pz_ReleaseBatchInd);
  shimIndInfo.q_Id = QMI_LOC_RELEASE_BATCH_IND_V02;

  /* send indication */
  return locQmiShimSendInd(pz_LocClient->client_handle,
                           &shimIndInfo);
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle ReleaseBatch Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the ReleaseBatch request

@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locReleaseBatchReq
(
   locQmiShimCltInfoStructT      *pz_QmiShimCltInfo,
   qmi_req_handle                z_ReqHandle,
   locQmiShimMsgInfoStructT      *pz_MsgInfo
   )
{
  qmiLocReleaseBatchIndMsgT_v02 z_ReleaseBatchInd = { 0 };
  boolean                       v_RetVal = FALSE;
  boolean                       v_RespResult = TRUE; /* default TRUE */
  qmi_error_type_v01            e_RespErr = QMI_ERR_NONE_V01; /* default NO Error */
  boolean                       v_Otb = FALSE; /* outdoor trip batching */
  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_QmiShimCltInfo->l_LocClientHandle);

  qmiLocReleaseBatchReqMsgT_v02 *pz_ReleaseBatchReq = (qmiLocReleaseBatchReqMsgT_v02 *)(pz_MsgInfo->p_Msg);

  if ((NULL == pz_ReleaseBatchReq) || (NULL == pz_LocClient))
  {
    LOC_MSG_ERROR("locReleaseBatchReq: NULL param(s)\n",
                  0, 0, 0);

    v_RespResult = FALSE;
    e_RespErr = QMI_ERR_INTERNAL_V01;
    return v_RetVal;
  }

  if ((TRUE == pz_ReleaseBatchReq->batchType_valid) &&
      (eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02 == pz_ReleaseBatchReq->batchType))
  {
    v_Otb = TRUE;
  }

  if (TRUE == v_RespResult)
  {
    if (FALSE == v_Otb)
    {
      /* check if memory is already allocated for location batching client */
      if (FALSE == pz_LocClient->batching_info.is_batching_client)
      {
        LOC_MSG_ERROR("locReleaseBatchReq: client handle %d is not a batching client",
                      pz_QmiShimCltInfo->l_LocClientHandle, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_INVALID_HANDLE_V01;
      }
      else if (TRUE == locAonCheckIfLocClientPresent(pz_QmiShimCltInfo->l_LocClientHandle, eLOC_AON_REQ_BATCH))
      {
        LOC_MSG_ERROR("locReleaseBatchReq: LB call QMI_LOC_STOP_BATCHING before RELEASE_BATCHING",
                      0, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_INVALID_OPERATION_V01;
      }
    }
    else
    {
      /* check if memory is already allocated for Outdoor Trip Batching client */
      if (FALSE == pz_LocClient->z_OtbInfo.v_OtbClient)
      {
        LOC_MSG_ERROR("locReleaseBatchReq: client handle %d is not a Otb client",
                      pz_QmiShimCltInfo->l_LocClientHandle, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_INVALID_HANDLE_V01;
      }
      else if (TRUE == locAonCheckIfLocClientPresent(pz_QmiShimCltInfo->l_LocClientHandle, eLOC_AON_REQ_OTB))
      {
        LOC_MSG_ERROR("locReleaseBatchReq: OTB call QMI_LOC_STOP_BATCHING before RELEASE_BATCHING",
                      0, 0, 0);
        v_RespResult = FALSE;
        e_RespErr = QMI_ERR_INVALID_OPERATION_V01;
      }
    }
  }

  /* send response first */
  v_RetVal = locBatchingOperationSendResp(pz_QmiShimCltInfo,
                                          z_ReqHandle,
                                          pz_MsgInfo,
                                          v_RespResult,
                                          e_RespErr);

  /* process the request and send indication only if RESP is SUCCESS */
  if ((TRUE == v_RespResult) && (TRUE == v_RetVal))
  {
    if (FALSE == v_Otb)
    {
      /* free the buffer and reset client info */
      v_RetVal = loc_batching_free_and_reset(pz_LocClient);

      /* Queue up the indication that needs to be sent */
      v_RetVal = loc_send_release_batch_ind(pz_LocClient, &z_ReleaseBatchInd, v_RetVal,
                                            pz_ReleaseBatchReq->transactionId, eQMI_LOC_LOCATION_BATCHING_V02);
    }
    else
    {
      if (FALSE == pz_LocClient->z_OtbInfo.v_Active)
      {
        /* free the OTB buffer and reset client info */
        v_RetVal = locOtb_FreeAndReset(pz_LocClient);
      }
      else
      {
        LOC_MSG_ERROR("locReleaseBatchReq: fail because session is active", 0, 0, 0);
        v_RetVal = FALSE;
      }

      /* Queue up the OTB indication that needs to be sent */
      v_RetVal = loc_send_release_batch_ind(pz_LocClient, &z_ReleaseBatchInd, v_RetVal,
                                            pz_ReleaseBatchReq->transactionId, eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02);
    }
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle StartBatching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   pStartBatchingReq     Request message from the control point


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartBatchingReq(locQmiShimCltInfoStructT     *pz_CltInfo,
                                   qmi_req_handle               z_ReqHandle,
                                   locQmiShimMsgInfoStructT     *pz_MsgInfo)
{
  boolean v_RetVal = FALSE;
  boolean v_SendErrorInd = FALSE;
  qmiLocStatusEnumT_v02  e_qmiLocStatus = eQMI_LOC_GENERAL_FAILURE_V02;
  qmiLocGenRespMsgT_v02  z_QmiLocResp;
  qmiLocStartBatchingReqMsgT_v02 *pStartBatchingReq = NULL;

  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

  if (NULL == pz_MsgInfo)
  {
    LOC_MSG_ERROR("locStartBatchingReq: invalid invalid msg info 0x%x",
                  pz_MsgInfo, 0, 0);
    return v_RetVal;
  }
  else if (NULL == pz_LocClient)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INTERNAL_V01;
    LOC_MSG_ERROR("locStartBatchingReq: invalid client info 0x%x",
                  pz_CltInfo, 0, 0);
  }
  else if (TRUE != pz_LocClient->batching_info.is_batching_client)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INVALID_OPERATION_V01;
    LOC_MSG_ERROR("locStartBatchingReq: Not a batching client",
                  0, 0, 0);
  }
  else if (NULL == pz_LocClient->batching_info.loc_batching_array)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INVALID_OPERATION_V01;
    LOC_MSG_ERROR("locStartBatchingReq: Call QMI_LOC_GET_BATCH_SIZE_REQ_V02 to allocate batching buffer",
                  0, 0, 0);
  }
  else if (FALSE == loc_middleware_data.v_AfwAllowAfwPosReq)
  {
    e_qmiLocStatus = eQMI_LOC_GNSS_DISABLED_V02;
    v_SendErrorInd = TRUE;
    LOC_MSG_ERROR("locStartBatchingReq: AFW locked",
                  0, 0, 0);
  }
  else
  {
    pStartBatchingReq = (qmiLocStartBatchingReqMsgT_v02 *)pz_MsgInfo->p_Msg;
    /* forward the request to AoN*/
    locAonBatchStartReq(pz_CltInfo->l_LocClientHandle,
                        pStartBatchingReq,
                        &z_QmiLocResp);
  }

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locStartBatchingReq: <<< Send QMI_RESP 0X%04x failed for locClient %d",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }
  else
  {
    LOC_MSG_MED("locStartBatchingReq: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d ",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
    if (v_SendErrorInd)
    {
      locAonSendStartBatchingInd(pz_CltInfo->l_LocClientHandle,
                                 pz_MsgInfo->q_Id, e_qmiLocStatus, eQMI_LOC_LOCATION_BATCHING_V02);
  }
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle StopBatching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopBatching request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStopBatchingReq
(
   locQmiShimCltInfoStructT    *pz_CltInfo,
   qmi_req_handle              z_ReqHandle,
   locQmiShimMsgInfoStructT    *pz_MsgInfo
   )
{
  boolean                 v_RetVal = FALSE;
  qmiLocGenRespMsgT_v02   z_QmiLocResp = { 0 };
  qmiLocStopBatchingReqMsgT_v02 *pz_StopBatchingReq = (qmiLocStopBatchingReqMsgT_v02 *)pz_MsgInfo->p_Msg;
  loc_client_info_s_type *pz_LocClient = locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);
  boolean                 v_Otb = FALSE; /* outdoor trip batching */
  boolean                 v_RespResult = FALSE;

  /* Input pointers have been checked in requestCb */
  if (TRUE == pz_StopBatchingReq->batchType_valid &&
      eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02 == pz_StopBatchingReq->batchType)
  {
    v_Otb = TRUE;
  }

  if (NULL == pz_LocClient)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error = QMI_ERR_INTERNAL_V01;
    LOC_MSG_ERROR("locStopBatchingReq: could not find client info",
                  0, 0, 0);
  }
  /* check if this client is a location batching client */
  else if ((FALSE == v_Otb) &&
           (FALSE == pz_LocClient->batching_info.is_batching_client))
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error = QMI_ERR_INVALID_OPERATION_V01;
    LOC_MSG_ERROR("locStopBatchingReq: NOT a location batching client", 0, 0, 0);
  }
  /* check if this client is a outdoor trip batching client */
  else if ((TRUE == v_Otb) &&
           (FALSE == pz_LocClient->z_OtbInfo.v_OtbClient))
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error = QMI_ERR_INVALID_OPERATION_V01;
    LOC_MSG_ERROR("locStopBatchingReq: NOT a outdoor trip batching client", 0, 0, 0);
  }
  else
  {
    v_RespResult = TRUE;
  }

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  /* Send a success indication if the response was a success
     if the the response was a failure no need to send an indication*/
  if ((TRUE == v_RetVal) &&
      (TRUE == v_RespResult))
  {
    locQmiShimIndInfoStructT      z_ShimIndInfo = { 0 };
    qmiLocStopBatchingIndMsgT_v02 z_StopBatchingInd = { 0 };

    /* forward the request to AoN*/
    if (FALSE == v_Otb)
    {
      locAonBatchStopReq(pz_CltInfo->l_LocClientHandle,
                         pz_StopBatchingReq,
                         &z_QmiLocResp);
    }
    else
    {
      locAonOtbStopReq(pz_CltInfo->l_LocClientHandle,
                       pz_StopBatchingReq,
                       &z_QmiLocResp);
    }

    z_StopBatchingInd.status = eQMI_LOC_SUCCESS_V02;
    z_StopBatchingInd.transactionId = pz_StopBatchingReq->transactionId;
    z_StopBatchingInd.requestId_valid = pz_StopBatchingReq->requestId_valid;
    z_StopBatchingInd.requestId = pz_StopBatchingReq->requestId;
    z_ShimIndInfo.p_Msg = &z_StopBatchingInd;
    z_ShimIndInfo.q_MsgLen = sizeof(z_StopBatchingInd);
    z_ShimIndInfo.q_Id = QMI_LOC_STOP_BATCHING_IND_V02;

    LOC_MSG_MED("locStopBatchingReq: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d ",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
    /* send indication */
    v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle,
                                 &z_ShimIndInfo);
  }
  else if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locStopBatchingReq: <<< Send QMI_RESP 0X%04x failed for LocClient",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle a Start DBT session from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StartDBT request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartDbtReq(locQmiShimCltInfoStructT     *pz_CltInfo,
                              qmi_req_handle               z_ReqHandle,
                              locQmiShimMsgInfoStructT     *pz_MsgInfo)
{
  boolean v_RetVal = FALSE;
  boolean v_SendErrorInd = FALSE;
  qmiLocStatusEnumT_v02  e_qmiLocStatus = eQMI_LOC_GENERAL_FAILURE_V02;
  qmiLocGenRespMsgT_v02  z_QmiLocResp;
  qmiLocStartDbtReqMsgT_v02 *pStartDbtReq = NULL;
  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

  if (NULL == pz_MsgInfo)
  {
    LOC_MSG_ERROR("locStartDbtReq: NULL QmiShimMsgInfo param", 0, 0, 0);
    return FALSE;
  }
  else if (NULL == pz_LocClient)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INTERNAL_V01;
    LOC_MSG_ERROR("locStartDbtReq: invalid client info 0x%x",
                  pz_CltInfo, 0, 0);
  }
  else if (FALSE == loc_middleware_data.v_AfwAllowAfwPosReq)
  {
    e_qmiLocStatus = eQMI_LOC_GNSS_DISABLED_V02;
    v_SendErrorInd = FALSE;
    LOC_MSG_ERROR("locStartDbtReq: AFW locked",
                  0, 0, 0);
  }
  else
  {
  pStartDbtReq = (qmiLocStartDbtReqMsgT_v02 *)pz_MsgInfo->p_Msg;
  /* forward the request to AoN*/
  locAonDbtStartReq(pz_CltInfo->l_LocClientHandle,
                    pStartDbtReq,
                    &z_QmiLocResp);
  }

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locStartDbtReq: <<< Send QMI_RESP 0X%04x failed for LocClient %d",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }
  else
  {
    LOC_MSG_MED("locStartDbtReq: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d ",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
    if (v_SendErrorInd)
    {
      locAonSendStartBatchingInd(pz_CltInfo->l_LocClientHandle,
                                 pz_MsgInfo->q_Id, e_qmiLocStatus, QMILOCBATCHINGTYPEENUMT_MAX_ENUM_VAL_V02);
  }
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle Stop a DBT session from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopDBT request

@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStopDbtReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                             qmi_req_handle              z_ReqHandle,
                             locQmiShimMsgInfoStructT    *pz_MsgInfo)
{
  boolean v_RetVal = FALSE;
  qmiLocGenRespMsgT_v02  z_QmiLocResp;
  qmiLocStopDbtReqMsgT_v02 *pStopDbtReq = NULL;

  if (NULL == pz_MsgInfo)
  {
    LOC_MSG_ERROR("locAonDbtStopReq: NULL QmiShimMsgInfo param", 0, 0, 0);
    return FALSE;
  }

  pStopDbtReq = (qmiLocStopDbtReqMsgT_v02 *)pz_MsgInfo->p_Msg;

  /* forward the request to AoN*/
  locAonDbtStopReq(pz_CltInfo->l_LocClientHandle,
                   pStopDbtReq,
                   &z_QmiLocResp);

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  /* Send a success indication if the response was a success
     if the the response was a failure no need to send an indication*/
  if ((TRUE == v_RetVal) &&
      (QMI_RESULT_SUCCESS_V01 == z_QmiLocResp.resp.result))
  {
    locQmiShimIndInfoStructT  shimIndInfo;
    qmiLocStopDbtIndMsgT_v02 stopDbtInd;
    stopDbtInd.status = eQMI_LOC_SUCCESS_V02;
    stopDbtInd.reqId_valid = TRUE;
    stopDbtInd.reqId = pStopDbtReq->reqId;
    shimIndInfo.p_Msg = &stopDbtInd;
    shimIndInfo.q_MsgLen = sizeof(stopDbtInd);
    shimIndInfo.q_Id = QMI_LOC_STOP_DBT_IND_V02;

    LOC_MSG_MED("locAonDbtStopReq: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);

    /* send indication */
    v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle,
                                 &shimIndInfo);
  }
  else if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locAonDbtStopReq: <<< Send QMI_RESP 0X%04x failed for LocClient %d",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle QueryAonConfig Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StopBatching request


@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locQueryAonConfigReq(locQmiShimCltInfoStructT    *pz_CltInfo,
                                    qmi_req_handle              z_ReqHandle,
                                    locQmiShimMsgInfoStructT    *pz_MsgInfo)
{
  boolean v_RetVal = FALSE;

  qmiLocGenRespMsgT_v02  z_QmiLocResp = { 0 };

  qmiLocQueryAonConfigReqMsgT_v02 *pQueryAonConfigReq =
     (qmiLocQueryAonConfigReqMsgT_v02 *)pz_MsgInfo->p_Msg;

  loc_client_info_s_type   *client_ptr =
     locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

  LOC_MSG_MED("locQueryAonConfigReq : got request to query Aon Config", 0, 0, 0);

  if (NULL == client_ptr)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error = QMI_ERR_INTERNAL_V01;
    LOC_MSG_ERROR("locQueryAonConfigReq: could not find client info",
                  0, 0, 0);
  }

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  /* Send a success indication if the response was a success
     if the the response was a failure no need to send an indication*/
  if ((TRUE == v_RetVal) &&
      (QMI_RESULT_SUCCESS_V01 == z_QmiLocResp.resp.result))
  {
    locQmiShimIndInfoStructT  shimIndInfo;
    qmiLocQueryAonConfigIndMsgT_v02 aonConfigInd;

    aonConfigInd.status = eQMI_LOC_SUCCESS_V02;
    aonConfigInd.transactionId_valid = TRUE;
    aonConfigInd.transactionId = pQueryAonConfigReq->transactionId;
    aonConfigInd.aonCapability_valid = TRUE;
    aonConfigInd.aonCapability = (
                                  QMI_LOC_MASK_AON_AUTO_BATCHING_SUPPORTED_V02 |
                                     QMI_LOC_MASK_AON_DISTANCE_BASED_BATCHING_SUPPORTED_V02 |
                                     QMI_LOC_MASK_AON_TIME_BASED_BATCHING_SUPPORTED_V02 |
                                     QMI_LOC_MASK_AON_DISTANCE_BASED_TRACKING_SUPPORTED_V02 |
                                     QMI_LOC_MASK_AON_UPDATE_TBF_SUPPORTED_V02 |
                                     QMI_LOC_MASK_AON_OUTDOOR_TRIP_BATCHING_SUPPORTED_V02);

    LOC_MSG_MED("locQueryAonConfigReq : returning Aon Config = %d, transaction id = %d",
                aonConfigInd.aonCapability, aonConfigInd.transactionId, 0);

    shimIndInfo.p_Msg = &aonConfigInd;
    shimIndInfo.q_MsgLen = sizeof(aonConfigInd);
    shimIndInfo.q_Id = QMI_LOC_QUERY_AON_CONFIG_IND_V02;
    /* send indication */
    v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle,
                                 &shimIndInfo);
  }
  else if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locQueryAonConfigReq: <<< Send QMI_RESP 0X%04x failed for LocClient %d",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }

  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
Function to handle Start Outdoor Trip Batching Request from Control point 

@param[in]   pz_CltInfo            Client Info Handle 
@param[in]   z_ReqHandle           Opaque handle provided by the infrastructure
                                 to specify a particular transaction and message.
@param[in]   pz_MsgInfo            Shim info struct containing the StartOTB request



@retval    TRUE           request command handle success
@retval    FALSE          request command handle failure
*/ /* ------------------------------------------------------------------------*/
static boolean locStartOTBReq
(
   locQmiShimCltInfoStructT     *pz_CltInfo,
   qmi_req_handle               z_ReqHandle,
   locQmiShimMsgInfoStructT     *pz_MsgInfo
   )
{
  boolean                 v_RetVal = FALSE;
  boolean                 v_SendErrorInd = FALSE;
  qmiLocStatusEnumT_v02   e_qmiLocStatus = eQMI_LOC_GENERAL_FAILURE_V02;
  qmiLocGenRespMsgT_v02   z_QmiLocResp;
  qmiLocStartOutdoorTripBatchingReqMsgT_v02 *pz_StartOTBgReq = NULL;
  loc_client_info_s_type *pz_LocClient =
     locClnt_FindHandle(pz_CltInfo->l_LocClientHandle);

  if (NULL == pz_MsgInfo)
  {
    LOC_MSG_ERROR("locStartOTBReq: invalid msg info 0x%x",
                  pz_MsgInfo, 0, 0);
    return v_RetVal;
  }
  else if (NULL == pz_LocClient)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INTERNAL_V01;
    LOC_MSG_ERROR("locStartOTBReq: invalid client info 0x%x",
                  pz_CltInfo, 0, 0);
  }
  else if (FALSE == pz_LocClient->z_OtbInfo.v_OtbClient)
  {
    z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
    z_QmiLocResp.resp.error =  QMI_ERR_INVALID_OPERATION_V01;
    LOC_MSG_ERROR("locStartOTBReq: Not a batching client", 0, 0, 0);
  }
  else if (FALSE == loc_middleware_data.v_AfwAllowAfwPosReq)
  {
    e_qmiLocStatus = eQMI_LOC_GNSS_DISABLED_V02;
    v_SendErrorInd = FALSE;
    LOC_MSG_ERROR("locStartOTBReq: AFW locked",
                  0, 0, 0);
  }
  else
  {
    pz_StartOTBgReq = (qmiLocStartOutdoorTripBatchingReqMsgT_v02 *)pz_MsgInfo->p_Msg;
    if (0 == pz_StartOTBgReq->batchDistance)
    {
      z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
      z_QmiLocResp.resp.error =  QMI_ERR_INVALID_ARG_V01;
      LOC_MSG_ERROR("locStartOTBReq: invalid batchdistance %d",
                    pz_StartOTBgReq->batchDistance, 0, 0);
    }
    else if (0 == pz_StartOTBgReq->minTimeInterval)
    {
      z_QmiLocResp.resp.result = QMI_RESULT_FAILURE_V01;
      z_QmiLocResp.resp.error =  QMI_ERR_INVALID_ARG_V01;
      LOC_MSG_ERROR("locStartOTBReq: invalid minTimeInterval %d",
                    pz_StartOTBgReq->minTimeInterval, 0, 0);
    }
    else
    {
      /* forward the request to AoN*/
      locAonOtbStartReq(pz_CltInfo->l_LocClientHandle,
                        pz_StartOTBgReq,
                        &z_QmiLocResp);
    }
  }

  /* send response */
  v_RetVal = (QMI_CSI_NO_ERR ==
                 mqcsi_send_rsp(pz_CltInfo->p_QmiClientHandle,
                                z_ReqHandle,
                                pz_MsgInfo->q_Id,
                                &z_QmiLocResp,
                                sizeof(qmiLocGenRespMsgT_v02)));

  if (TRUE != v_RetVal)
  {
    LOC_MSG_ERROR("locStartOTBReq: <<< Send QMI_RESP 0X%04x failed for LocClient %d ",
                  pz_MsgInfo->q_Id, pz_CltInfo->l_LocClientHandle, 0);
  }
  else
  {
    LOC_MSG_MED("locStartOTBReq: <<< Send QMI_RESP 0X%04x succeed,error:%d,result:%d ",
                pz_MsgInfo->q_Id, z_QmiLocResp.resp.error, z_QmiLocResp.resp.result);
    if (v_SendErrorInd)
    {
      locAonSendStartBatchingInd(pz_CltInfo->l_LocClientHandle,
                                 pz_MsgInfo->q_Id, e_qmiLocStatus, eQMI_LOC_OUTDOOR_TRIP_BATCHING_V02);
  }
  }
  return v_RetVal;
}

/* -----------------------------------------------------------------------*//**
@brief
A helper utility to send QMI_LOC_EVENT_ENGINE_LOCK_STATE_IND message.
*/ /* ------------------------------------------------------------------------*/
static void locSendEngineLockStateInd(void)
{
  locQmiShimIndInfoStructT  z_IndInfo = { 0 };
  qmiLocEventEngineLockStateIndMsgT_v02 z_EngLockStateInd = { 0 };
  z_IndInfo.q_Id          = QMI_LOC_EVENT_ENGINE_LOCK_STATE_IND_V02;
  z_IndInfo.q_MsgLen      = sizeof(z_EngLockStateInd);

  /* Fill up optional TLV locClient, engineLockState is deprecated*/
  z_EngLockStateInd.lockClient_valid = TRUE;
  if (FALSE == loc_middleware_data.v_AfwAllowAfwPosReq)
  {
    z_EngLockStateInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_AFW_V02;;
  }

  if (FALSE == loc_middleware_data.v_AfwAllowNfwPosReq &&
      FALSE == loc_middleware_data.v_NvAllowNfwPosReq)
  {
    z_EngLockStateInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_NFW_V02;;
  }

  if (FALSE == loc_middleware_data.v_AfwAllowPriPosReq)
  {
    z_EngLockStateInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_PRIVILEGED_V02;
  }

  //TODO, subType is not support yet

  z_IndInfo.p_Msg         = &z_EngLockStateInd;
  locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_ENGINE_LOCK_STATE_V02,
                         &z_IndInfo);
}

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC Indication for PA Set commands
---------------------------------------------------------------------------*/
boolean locQmi_ProcessPaSetInd
(
   loc_client_handle_type l_ClientHandle,
   pdsm_pa_e_type         e_ParamType,
   pdsm_pa_info_s_type   *p_PdsmPaInfo,
   qmiLocStatusEnumT_v02  e_IndStatus
   )
{
  locQmiShimIndInfoStructT   z_IndInfo  = { 0 };
  locQmiShimCltInfoStructT  *pz_CltInfo  = NULL;
  boolean                    v_RetVal = FALSE;

  LOC_MSG_HIGH("locQmi_ProcessPaSetInd: ParamType=%d, ClientHandle=%d, IndStatus=%d", e_ParamType, l_ClientHandle, e_IndStatus);

  do
  {
    pz_CltInfo = q_linear_search(&z_LocQmiShim.z_ClientQ, locQmiShimCompareLocApiHandle, (void *)l_ClientHandle);
    if (NULL == pz_CltInfo)
    {
      LOC_MSG_ERROR("locQmi_ProcessPaSetInd: Invalid Client Info for Handle=%d", l_ClientHandle, 0, 0);
      break;
    }

    /* Function parameters sanity check only applies to the following Param Type: PDSM_PA_PE_CONFIG_PARAM
       All other Param Types will ignore p_PdsmPaInfo, so no check required here. */

    /* Process all PDSM Set Parameter commands here */
    switch (e_ParamType)
    {
      case PDSM_PA_GPS_LOCK:
        {
          qmiLocSetEngineLockIndMsgT_v02 z_SetEngLockInd = { 0 };
          z_IndInfo.q_Id          = QMI_LOC_SET_ENGINE_LOCK_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(z_SetEngLockInd);
          z_SetEngLockInd.status  = e_IndStatus;
          z_IndInfo.p_Msg         = &z_SetEngLockInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
          if (eQMI_LOC_SUCCESS_V02 == e_IndStatus)
          {
            locSendEngineLockStateInd();
          }
        }
        break;

      case PDSM_PA_SBAS_USER_PREFERENCE:
        {
          qmiLocSetSbasConfigIndMsgT_v02 z_SetSbasCfgInd = { 0 };
          z_IndInfo.q_Id          = QMI_LOC_SET_SBAS_CONFIG_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(z_SetSbasCfgInd);
          z_SetSbasCfgInd.status  = e_IndStatus;
          z_IndInfo.p_Msg         = &z_SetSbasCfgInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_NMEA_SENTENCE_TYPE:
        {
          qmiLocSetNmeaTypesIndMsgT_v02 z_SetNmeaTypesInd = { 0 };
          z_IndInfo.q_Id            = QMI_LOC_SET_NMEA_TYPES_IND_V02;
          z_IndInfo.q_MsgLen        = sizeof(z_SetNmeaTypesInd);
          z_SetNmeaTypesInd.status  = e_IndStatus;
          z_IndInfo.p_Msg           = &z_SetNmeaTypesInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_ON_DEMAND_LOW_POWER_MODE:
        {
          qmiLocSetLowPowerModeIndMsgT_v02 z_SetLpmInd = { 0 };
          z_IndInfo.q_Id      = QMI_LOC_SET_LOW_POWER_MODE_IND_V02;
          z_IndInfo.q_MsgLen  = sizeof(z_SetLpmInd);
          z_SetLpmInd.status  = e_IndStatus;
          z_IndInfo.p_Msg     = &z_SetLpmInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case  PDSM_PA_1X_PDE_NETWORK_ADRS:
      case  PDSM_PA_1X_MPC_NETWORK_ADRS:
      case  PDSM_PA_UMTS_PDE_NETWORK_ADRS:
        {
          qmiLocSetServerIndMsgT_v02 z_SetServerInd = { 0 };
          z_IndInfo.q_Id         = QMI_LOC_SET_SERVER_IND_V02;
          z_IndInfo.q_MsgLen     = sizeof(z_SetServerInd);
          z_SetServerInd.status  = e_IndStatus;
          z_IndInfo.p_Msg        = &z_SetServerInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_DELETE_PARAMS:
        {
          qmiLocDeleteAssistDataIndMsgT_v02 z_DelAssistDataInd = { 0 };
          z_IndInfo.q_Id             = QMI_LOC_DELETE_ASSIST_DATA_IND_V02;
          z_IndInfo.q_MsgLen         = sizeof(z_DelAssistDataInd);
          z_DelAssistDataInd.status  = e_IndStatus;
          z_IndInfo.p_Msg            = &z_DelAssistDataInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_XTRAT_USER_SESSION_CONTROL:
        {
          qmiLocSetXtraTSessionControlIndMsgT_v02 z_SetXtraTInd = { 0 };
          z_IndInfo.q_Id        = QMI_LOC_SET_XTRA_T_SESSION_CONTROL_IND_V02;
          z_IndInfo.q_MsgLen    = sizeof(z_SetXtraTInd);
          z_SetXtraTInd.status  = e_IndStatus;
          z_IndInfo.p_Msg       = &z_SetXtraTInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_SUPL_SECURITY:
      case PDSM_PA_SUPL_VERSION:
      case PDSM_PA_LPP_CONFIG_INFO:
      case PDSM_PA_ASST_GLONASS_POS_PROTOCOL:
      case PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG:
      case PDSM_PA_SUPL_HASH_ALGORITHM:
      case PDSM_PA_SUPL_TLS_VERSION:
      case PDSM_PA_WIFI_SCAN_TIMEOUT_SEC:
      case PDSM_PA_LPPE_UP_CONFIG:
      case PDSM_PA_LPPE_CP_CONFIG:
      case PDSM_PA_EMERGENCY_EXTENSION_WINDOW:
        {
          qmiLocSetProtocolConfigParametersIndMsgT_v02    z_SetProtCfgInd = { 0 };
          z_IndInfo.q_Id          = QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(z_SetProtCfgInd);
          z_SetProtCfgInd.status  = e_IndStatus;
          z_IndInfo.p_Msg         = &z_SetProtCfgInd;
          v_RetVal = TRUE;

          if (eQMI_LOC_SUCCESS_V02 != e_IndStatus)
          {
            z_SetProtCfgInd.failedProtocolConfigParamMask_valid = TRUE;

            /* Since muxing is not supported there will ever only be one failed Protocol per indication.
               Setting the appropriate failed Protocol Mask here. */
            switch (e_ParamType)
            {
              case PDSM_PA_SUPL_SECURITY:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_SECURITY_V02;
                  break;
                }
              case PDSM_PA_SUPL_VERSION:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_VERSION_V02;
                  break;
                }
              case PDSM_PA_LPP_CONFIG_INFO:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPP_CONFIG_V02;
                  break;
                }
              case PDSM_PA_ASST_GLONASS_POS_PROTOCOL:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_ASSISTED_GLONASS_PROTOCOL_V02;
                  break;
                }
              case PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_EMERGENCY_PROTOCOL_V02;
                  break;
                }
              case PDSM_PA_SUPL_HASH_ALGORITHM:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_HASH_ALGO_V02;
                  break;
                }
              case PDSM_PA_SUPL_TLS_VERSION:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_TLS_VERSION_V02;
                  break;
                }
              case PDSM_PA_WIFI_SCAN_TIMEOUT_SEC:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_WIFI_SCAN_INJECT_TIMEOUT_V02;
                  break;
                }
              case PDSM_PA_LPPE_UP_CONFIG:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPPE_UP_V02;
                  break;
                }
              case PDSM_PA_LPPE_CP_CONFIG:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPPE_CP_V02;
                  break;
                }
              case PDSM_PA_EMERGENCY_EXTENSION_WINDOW:
                {
                  z_SetProtCfgInd.failedProtocolConfigParamMask = QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_EMERGENCY_CB_WINDOW_V02;
                  break;
                }
              default:
                {
                  LOC_MSG_ERROR("locQmi_ProcessPaSetInd: Invalid Config Parameter=%d", e_ParamType, 0, 0);
                  v_RetVal = FALSE;
                  break;
                }
            }
          }

          LOC_MSG_MED("locQmi_ProcessPaSetInd: Param=%d Status=%d", e_ParamType, e_IndStatus, 0);
          if (TRUE == v_RetVal)
          {
            v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
          }
        }
        break;

      case PDSM_PA_EFS_DATA:
        {
          LOC_MSG_MED("locQmi_ProcessPaGetInd: EFS Data File Operation=%d [0=INJ, 2=DEL, 4=PUR]",
                      loc_middleware_data.z_locPa_Data.e_EfsFileOperation, 0, 0);
          switch (loc_middleware_data.z_locPa_Data.e_EfsFileOperation)
          {
            case PDSM_FS_CREATE_WRITE_FILE:
              {
                qmiLocInjectSuplCertificateIndMsgT_v02    z_InjSuplCertInd = { 0 };
                z_IndInfo.q_Id     = QMI_LOC_INJECT_SUPL_CERTIFICATE_IND_V02;
                z_IndInfo.q_MsgLen = sizeof(z_InjSuplCertInd);
                z_InjSuplCertInd.status = e_IndStatus;
                z_IndInfo.p_Msg         = &z_InjSuplCertInd;
                v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
              }
              break;

            case PDSM_FS_DELETE_FILE:
            case PDSM_FS_PURGE_DIR:
              {
                qmiLocDeleteSuplCertificateIndMsgT_v02    z_DelSuplCertInd = { 0 };
                z_IndInfo.q_Id     = QMI_LOC_DELETE_SUPL_CERTIFICATE_IND_V02;
                z_IndInfo.q_MsgLen = sizeof(z_DelSuplCertInd);
                z_DelSuplCertInd.status = e_IndStatus;
                z_IndInfo.p_Msg         = &z_DelSuplCertInd;
                v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
              }
              break;

            default:
              {
                LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid EFS Data File Operation=%d",
                              p_PdsmPaInfo->pa_info.efs_data.efs_data_file_operation, 0, 0);
              }
          }
        }
        break;

      case PDSM_PA_PE_CONFIG_PARAM:
        {
          qmiLocPositionEngineConfigParamMaskT_v02 q_FailedPeCfgMask = 0;

          qmiLocSetPositionEngineConfigParametersIndMsgT_v02  z_SetPeCfgInd = { 0 };
          z_IndInfo.q_Id        = QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_IND_V02;
          z_IndInfo.q_MsgLen    = sizeof(z_SetPeCfgInd);
          z_SetPeCfgInd.status  = e_IndStatus;
          z_IndInfo.p_Msg       = &z_SetPeCfgInd;

          /* Function parameters sanity check */
          if (NULL == p_PdsmPaInfo)
          {
            LOC_MSG_ERROR("locQmi_ProcessPaSetInd: PDSM_PA_PE_CONFIG_PARAM NULL parameters", 0, 0, 0);
            v_RetVal = FALSE;
          }
          else
          {
            if (eQMI_LOC_SUCCESS_V02 != e_IndStatus)
            {
              /* Handle unsuccessful case from PA command callback */
              q_FailedPeCfgMask |= (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.injectedPositionControl_valid) ?
                 QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_INJECTED_POSITION_CONTROL_V02 : 0;
              q_FailedPeCfgMask |= (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.filterSvUsage_valid) ?
                 QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_FILTER_SV_USAGE_V02 : 0;
              q_FailedPeCfgMask |= (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.storeAssistData_valid) ?
                 QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA_V02 : 0;
              LOC_QMI_SHIM_SET_VALUE(z_SetPeCfgInd.failedPositionEngineConfigParamMask, q_FailedPeCfgMask);
            }
            else
            {
              /* If a bit mask is set in the PDAPI structure, then parameter was successfully set */
              if (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.injectedPositionControl_valid)
              {
                q_FailedPeCfgMask |= (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask &
                                         PDSM_PA_PE_CONFIG_PARAM_MASK_USE_INJECTED_POSITION) ?
                   0 : QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_INJECTED_POSITION_CONTROL_V02;
              }
              if (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.filterSvUsage_valid)
              {
                q_FailedPeCfgMask |= (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask &
                                         PDSM_PA_PE_CONFIG_PARAM_MASK_FILTER_SV_USED) ?
                   0 : QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_FILTER_SV_USAGE_V02;
              }
              if (pz_CltInfo->z_PaReqUnion.z_SetPeCfg.storeAssistData_valid)
              {
                q_FailedPeCfgMask |= (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask &
                                         PDSM_PA_PE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA) ?
                   0 : QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA_V02;
              }

              /* If there is at least one mask that failed, then the set did not succeed */
              if (q_FailedPeCfgMask != 0)
              {
                z_SetPeCfgInd.failedPositionEngineConfigParamMask_valid = TRUE;
                z_SetPeCfgInd.status = eQMI_LOC_GENERAL_FAILURE_V02;
              }
              else
              {
                z_SetPeCfgInd.status = eQMI_LOC_SUCCESS_V02;
              }
            }
            v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
          }
        }
        break;

      case PDSM_PA_PREMIUM_SERVICES_CONFIG:
        {
          qmiLocSetPremiumServicesCfgIndMsgT_v02    z_SetPremSvcsInd = { 0 };
          z_IndInfo.q_Id           = QMI_LOC_SET_PREMIUM_SERVICES_CONFIG_IND_V02;
          z_IndInfo.q_MsgLen       = sizeof(z_SetPremSvcsInd);
          z_SetPremSvcsInd.status  = e_IndStatus;
          z_IndInfo.p_Msg          = &z_SetPremSvcsInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_XTRA_VERSION_CHECK:
        {
          qmiLocSetXtraVersionCheckIndMsgT_v02   z_SetXtraVersInd = { 0 };
          z_IndInfo.q_Id           = QMI_LOC_SET_XTRA_VERSION_CHECK_IND_V02;
          z_IndInfo.q_MsgLen       = sizeof(z_SetXtraVersInd);
          z_SetXtraVersInd.status  = e_IndStatus;
          z_IndInfo.p_Msg          = &z_SetXtraVersInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_GNSS_CONSTELL_REPORT_CONFIG:
        {
          qmiLocSetGNSSConstRepConfigIndMsgT_v02 z_SetGnssCfgInd = { 0 };
          z_IndInfo.q_Id          = QMI_LOC_SET_GNSS_CONSTELL_REPORT_CONFIG_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(z_SetGnssCfgInd);
          z_SetGnssCfgInd.status  = e_IndStatus;
          z_IndInfo.p_Msg         = &z_SetGnssCfgInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_DELETE_PARAMS_EX1:
        {
          qmiLocDeleteGNSSServiceDataIndMsgT_v02 z_DeleteGNSSServiceDataInd = { 0 };
          z_IndInfo.q_Id             = QMI_LOC_DELETE_GNSS_SERVICE_DATA_IND_V02;
          z_IndInfo.q_MsgLen         = sizeof(z_DeleteGNSSServiceDataInd);
          z_DeleteGNSSServiceDataInd.status = e_IndStatus;
          z_IndInfo.p_Msg            = &z_DeleteGNSSServiceDataInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_PCID:
        {
          qmiLocInjectXtraPcidIndMsgT_v02  z_InjectXtraPcidInd = { 0 };
          z_IndInfo.q_Id             = QMI_LOC_INJECT_XTRA_PCID_IND_V02;
          z_IndInfo.q_MsgLen         = sizeof(z_InjectXtraPcidInd);
          z_InjectXtraPcidInd.status = e_IndStatus;
          z_IndInfo.p_Msg            = &z_InjectXtraPcidInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_PERSISTENT_SV_MASK:
        {
          qmiLocGenReqStatusIndMsgT_v02 z_QmiSetBlacklistSvReqStatusInd = { 0 };
          z_IndInfo.q_Id = QMI_LOC_SET_BLACKLIST_SV_IND_V02;
          z_IndInfo.q_MsgLen = sizeof(z_QmiSetBlacklistSvReqStatusInd);
          z_QmiSetBlacklistSvReqStatusInd.status = e_IndStatus;
          z_IndInfo.p_Msg = &z_QmiSetBlacklistSvReqStatusInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      case PDSM_PA_CONSTELLATION_CONTROL_CONFIG:
        {
          qmiLocGenReqStatusIndMsgT_v02 z_QmiConstelCtrlReqStatusInd = { 0 };
          z_IndInfo.q_Id = QMI_LOC_SET_CONSTELLATION_CONTROL_IND_V02;
          z_IndInfo.q_MsgLen = sizeof(z_QmiConstelCtrlReqStatusInd);
          z_QmiConstelCtrlReqStatusInd.status = e_IndStatus;
          z_IndInfo.p_Msg = &z_QmiConstelCtrlReqStatusInd;
          v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
        }
        break;

      default:
        {
          LOC_MSG_ERROR("locQmi_ProcessPaSetInd: Invalid Set Param=%d", e_ParamType, 0, 0);
          v_RetVal = FALSE;
        }
        break;
    }
  }while (0);

  return v_RetVal;
}

/*---------------------------------------------------------------------------
@brief
This function converts PDSM Constellation Status to QMI Type.

@param[in] pdsm_constellation_status_e_type

@retval    qmiLocConstellationStatusEnumT_v02
---------------------------------------------------------------------------*/
qmiLocConstellationStatusEnumT_v02 locQmiShimConvertConstellationStatusInd(
   pdsm_constellation_status_e_type e_PdsmStatus)
{
  switch (e_PdsmStatus)
  {
    case PDSM_CONSTELLATION_ENABLED_MANDATORY:
      return eQMI_LOC_CONSTELLATION_ENABLED_MANDATORY_V02;

    case PDSM_CONSTELLATION_ENABLED_INTERNALLY:
      return eQMI_LOC_CONSTELLATION_ENABLED_INTERNALLY_V02;

    case PDSM_CONSTELLATION_ENABLED_BY_CLIENT:
      return eQMI_LOC_CONSTELLATION_ENABLED_BY_CLIENT_V02;

    case PDSM_CONSTELLATION_DISABLED_NOT_SUPPORTED:
      return eQMI_LOC_CONSTELLATION_DISABLED_NOT_SUPPORTED_V02;

    case PDSM_CONSTELLATION_DISABLED_INTERNALLY:
      return eQMI_LOC_CONSTELLATION_DISABLED_INTERNALLY_V02;

    case PDSM_CONSTELLATION_DISABLED_BY_CLIENT:
      return eQMI_LOC_CONSTELLATION_DISABLED_BY_CLIENT_V02;

    case PDSM_CONSTELLATION_DISABLED_NO_MEMORY:
      return eQMI_LOC_CONSTELLATION_DISABLED_NO_MEMORY_V02;

    default:
      return eQMI_LOC_CONSTELLATION_DISABLED_NOT_SUPPORTED_V02;
  }
}

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC Indication for PA Get commands
---------------------------------------------------------------------------*/
boolean locQmi_ProcessPaGetInd
(
   loc_client_handle_type l_ClientHandle,
   pdsm_pa_e_type         e_ParamType,
   pdsm_pa_info_s_type   *p_PdsmPaInfo,
   qmiLocStatusEnumT_v02  e_IndStatus
   )
{
  locQmiShimIndInfoStructT  z_IndInfo  = { 0 };
  locQmiShimCltInfoStructT *pz_CltInfo  = NULL;
  boolean                   v_RetVal = FALSE;

  LOC_MSG_HIGH("locQmi_ProcessPaGetInd: ParamType=%d, ClientHandle=%d, IndStatus=%d", e_ParamType, l_ClientHandle, e_IndStatus);

  pz_CltInfo = q_linear_search(&z_LocQmiShim.z_ClientQ, locQmiShimCompareLocApiHandle, (void *)l_ClientHandle);
  if (NULL == pz_CltInfo)
  {
    LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid Client Info for Handle=%d", l_ClientHandle, 0, 0);
    return v_RetVal;
  }

  /* Function parameters sanity check */
  if ((NULL == p_PdsmPaInfo) && (PDSM_PA_PE_CONFIG_PARAM != e_ParamType))
  {
    LOC_MSG_ERROR("locQmi_ProcessPaGetInd: NULL parameters", 0, 0, 0);
    return v_RetVal;
  }

  /* Process all PDSM Get Parameter commands here */
  switch (e_ParamType)
  {
    case PDSM_PA_GPS_LOCK:
      {
        qmiLocGetEngineLockIndMsgT_v02   z_GetEngLockInd = { 0 };
        z_IndInfo.q_Id          = QMI_LOC_GET_ENGINE_LOCK_IND_V02;
        z_IndInfo.q_MsgLen      = sizeof(z_GetEngLockInd);
        z_GetEngLockInd.status  = e_IndStatus;
        z_IndInfo.p_Msg         = &z_GetEngLockInd;

        /* GPS lock from SM is no longer used to report Lock status,
           because locMW has the lock status after Location Privacy feature */
        LOC_MSG_LOW("locQmi_ProcessPaGetInd: GPS Lock Type=%d", p_PdsmPaInfo->pa_info.gps_lock, 0, 0);

        /* Fill up optional TLV locClient only */
        z_GetEngLockInd.lockClient_valid = TRUE;
        z_GetEngLockInd.lockClient = 0;
        if (FALSE == loc_middleware_data.v_AfwAllowAfwPosReq)
        {
          z_GetEngLockInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_AFW_V02;
        }

        if (FALSE == loc_middleware_data.v_AfwAllowNfwPosReq &&
            FALSE == loc_middleware_data.v_NvAllowNfwPosReq)
        {
          z_GetEngLockInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_NFW_V02;
        }

        if (FALSE == loc_middleware_data.v_AfwAllowPriPosReq)
        {
          z_GetEngLockInd.lockClient |= QMI_LOC_LOCK_CLIENT_MASK_PRIVILEGED_V02;
        }
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_SBAS_USER_PREFERENCE:
      {
        qmiLocGetSbasConfigIndMsgT_v02  z_GetSbasCfgInd = { 0 };
        z_IndInfo.q_Id          = QMI_LOC_GET_SBAS_CONFIG_IND_V02;
        z_IndInfo.q_MsgLen      = sizeof(z_GetSbasCfgInd);
        z_GetSbasCfgInd.status  = e_IndStatus;
        z_IndInfo.p_Msg         = &z_GetSbasCfgInd;

        if (eQMI_LOC_SUCCESS_V02 == e_IndStatus)
        {
          if (PDSM_PA_SBAS_STATUS_ENABLED == p_PdsmPaInfo->pa_info.sbas_user_preference)
          {
            LOC_QMI_SHIM_SET_VALUE(z_GetSbasCfgInd.sbasConfig, TRUE);
          }
          else
          {
            LOC_QMI_SHIM_SET_VALUE(z_GetSbasCfgInd.sbasConfig, FALSE);
          }
        }
        else
        {
          LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid SBAS Ind Status=%d", e_IndStatus, 0, 0);
        }

        LOC_MSG_MED("locQmi_ProcessPaGetInd: SBAS Status=%d", p_PdsmPaInfo->pa_info.sbas_user_preference, 0, 0);
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_NMEA_SENTENCE_TYPE:
      {
        qmiLocGetNmeaTypesIndMsgT_v02 z_GetNmeaTypesInd = { 0 };
        z_IndInfo.q_Id            = QMI_LOC_GET_NMEA_TYPES_IND_V02;
        z_IndInfo.q_MsgLen        = sizeof(z_GetNmeaTypesInd);
        z_GetNmeaTypesInd.status  = e_IndStatus;
        z_IndInfo.p_Msg           = &z_GetNmeaTypesInd;

        z_GetNmeaTypesInd.nmeaSentenceType_valid = TRUE;
        z_GetNmeaTypesInd.nmeaSentenceType       = 0;

        if (PDAPI_NMEA_GGA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GGA_V02;
        }
        if (PDAPI_NMEA_RMC & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_RMC_V02;
        }
        if (PDAPI_NMEA_GSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GSV_V02;
        }
        if (PDAPI_NMEA_GSA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GSA_V02;
        }
        if (PDAPI_NMEA_VTG & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_VTG_V02;
        }
        if (PDAPI_NMEA_PQXFI & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_PQXFI_V02;
        }
        if (PDAPI_NMEA_PSTIS & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_PSTIS_V02;
        }
        if (PDAPI_NMEA_GLGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GLGSV_V02;
        }
        if (PDAPI_NMEA_GNGSA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNGSA_V02;
        }
        if (PDAPI_NMEA_GNGNS & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNGNS_V02;
        }
        if (PDAPI_NMEA_GARMC & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GARMC_V02;
        }
        if (PDAPI_NMEA_GAGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GAGSV_V02;
        }
        if (PDAPI_NMEA_GAGSA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GAGSA_V02;
        }
        if (PDAPI_NMEA_GAVTG & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GAVTG_V02;
        }
        if (PDAPI_NMEA_GAGGA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GAGGA_V02;
        }
        if (PDAPI_NMEA_PQGSA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_PQGSA_V02;
        }
        if (PDAPI_NMEA_PQGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_PQGSV_V02;
        }
        if (PDAPI_NMEA_DEBUG & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_DEBUG_V02;
        }
        if (PDAPI_NMEA_GPDTM & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GPDTM_V02;
        }
        if (PDAPI_NMEA_GAGNS & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GAGNS_V02;
        }
        if (PDAPI_NMEA_GNDTM & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNDTM_V02;
        }
        if (PDAPI_NMEA_GBGGA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GBGGA_V02;
        }
        if (PDAPI_NMEA_GNGGA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNGGA_V02;
        }
        if (PDAPI_NMEA_GBGSA & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GBGSA_V02;
        }
        if (PDAPI_NMEA_GBGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GBGSV_V02;
        }
        if (PDAPI_NMEA_GQGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GQGSV_V02;
        }
        if (PDAPI_NMEA_GIGSV & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GIGSV_V02;
        }
        if (PDAPI_NMEA_GBRMC & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GBRMC_V02;
        }
        if (PDAPI_NMEA_GNRMC & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNRMC_V02;
        }
        if (PDAPI_NMEA_GBVTG & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GBVTG_V02;
        }
        if (PDAPI_NMEA_GNVTG & p_PdsmPaInfo->pa_info.nmea_sentence_type)
        {
          z_GetNmeaTypesInd.nmeaSentenceType |= QMI_LOC_NMEA_MASK_GNVTG_V02;
        }

        LOC_MSG_MED_2("locQmi_ProcessPaGetInd: NMEA Sentence Mask PDAPI=0x%X, QMI=0x%X",
                    p_PdsmPaInfo->pa_info.nmea_sentence_type, z_GetNmeaTypesInd.nmeaSentenceType);
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_1X_PDE_NETWORK_ADRS:
    case PDSM_PA_1X_MPC_NETWORK_ADRS:
    case PDSM_PA_UMTS_PDE_NETWORK_ADRS:
      {
        pdsm_server_address_s_type  z_ServerAddr;
        qmiLocGetServerIndMsgT_v02  z_GetServerInd = { 0 };
        z_IndInfo.q_Id         = QMI_LOC_GET_SERVER_IND_V02;
        z_IndInfo.q_MsgLen     = sizeof(z_GetServerInd);
        z_GetServerInd.status  = e_IndStatus;
        z_IndInfo.p_Msg        = &z_GetServerInd;

        z_ServerAddr = p_PdsmPaInfo->pa_info.server_address;

        if (PDSM_PA_1X_PDE_NETWORK_ADRS == p_PdsmPaInfo->pa_event)
        {
          z_GetServerInd.serverType = eQMI_LOC_SERVER_TYPE_CDMA_PDE_V02;
        }
        else if (PDSM_PA_1X_MPC_NETWORK_ADRS == p_PdsmPaInfo->pa_event)
        {
          z_GetServerInd.serverType = eQMI_LOC_SERVER_TYPE_CDMA_MPC_V02;
        }
        else if (PDSM_PA_UMTS_PDE_NETWORK_ADRS == p_PdsmPaInfo->pa_event)
        {
          z_GetServerInd.serverType = eQMI_LOC_SERVER_TYPE_UMTS_SLP_V02;
        }
        else
        {
          LOC_MSG_ERROR("locQmi_ProcessPaGetInd: GetServerInd, Invalid Network Address Type=%d", p_PdsmPaInfo->pa_event, 0, 0);
          return FALSE;
        }

        if (PDSM_SERVER_TYPE_IPV4_ADDRESS == z_ServerAddr.server_addr_type)
        {
          z_GetServerInd.ipv4Addr_valid = TRUE;
          z_GetServerInd.ipv4Addr.addr  = z_ServerAddr.server_adrs.server_adrs_v4.server_adrs;
          z_GetServerInd.ipv4Addr.port  = z_ServerAddr.server_adrs.server_adrs_v4.port_id;
        }
        else if (PDSM_SERVER_TYPE_URL_ADDRESS == z_ServerAddr.server_addr_type)
        {
          z_GetServerInd.urlAddr_valid = TRUE;

          /* Bounded Buffer Copy, will get truncated if buffer allocated is not enough */

          memscpy(&(z_GetServerInd.urlAddr),
                  sizeof(z_GetServerInd.urlAddr) - 1,
                  &(z_ServerAddr.server_adrs.server_adrs_url.url),
                  QMI_LOC_MAX_SERVER_ADDR_LENGTH_V02);

          /* Reserve the last byte in the array for null-terminating the string */
          z_GetServerInd.urlAddr[sizeof(z_GetServerInd.urlAddr) - 1] = '\0';
        }
        else
        {
          LOC_MSG_ERROR("locQmi_ProcessPaGetInd: GetServerInd, Invalid Server Type=%d", z_ServerAddr.server_addr_type, 0, 0);
          return FALSE;
        }

        LOC_MSG_MED("locQmi_ProcessPaGetInd: GetServerInd, Server type=%d",
                    z_ServerAddr.server_addr_type, 0, 0);
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_XTRAT_USER_SESSION_CONTROL:
      {
        qmiLocGetXtraTSessionControlIndMsgT_v02   z_GetXtraTInd = { 0 };
        z_IndInfo.q_Id        = QMI_LOC_GET_XTRA_T_SESSION_CONTROL_IND_V02;
        z_IndInfo.q_MsgLen    = sizeof(z_GetXtraTInd);
        z_GetXtraTInd.status  = e_IndStatus;
        z_IndInfo.p_Msg       = &z_GetXtraTInd;

        LOC_QMI_SHIM_SET_VALUE(z_GetXtraTInd.xtraTSessionControl, p_PdsmPaInfo->pa_info.session_control);

        LOC_MSG_MED("locQmi_ProcessPaGetInd: XTRA-T User Session Ctrl=%d", p_PdsmPaInfo->pa_info.session_control, 0, 0);
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_SUPL_VERSION:
    case PDSM_PA_SUPL_SECURITY:
    case PDSM_PA_LPP_CONFIG_INFO:
    case PDSM_PA_ASST_GLONASS_POS_PROTOCOL:
    case PDSM_PA_SUPL_TLS_VERSION:
    case PDSM_PA_SUPL_HASH_ALGORITHM:
    case PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG:
    case PDSM_PA_WIFI_SCAN_TIMEOUT_SEC:
    case PDSM_PA_LPPE_UP_CONFIG:
    case PDSM_PA_LPPE_CP_CONFIG:
    case PDSM_PA_EMERGENCY_EXTENSION_WINDOW:
      {
        qmiLocGetProtocolConfigParametersIndMsgT_v02 z_GetProtCfgInd = { 0 };
        z_IndInfo.q_Id          = QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_IND_V02;
        z_IndInfo.q_MsgLen      = sizeof(z_GetProtCfgInd);
        z_GetProtCfgInd.status  = e_IndStatus;
        z_IndInfo.p_Msg         = &z_GetProtCfgInd;

        if (eQMI_LOC_SUCCESS_V02 == e_IndStatus)
        {
          switch (p_PdsmPaInfo->pa_event)
          {
            case PDSM_PA_SUPL_SECURITY:
              {
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.suplSecurity,
                                       p_PdsmPaInfo->pa_info.supl_security);
              }
              break;

            case PDSM_PA_SUPL_VERSION:
              {
                qmiLocSuplVersionEnumT_v02 e_Version;

                switch (p_PdsmPaInfo->pa_info.supl_version)
                {
                  case PDSM_SUPL_VERSION_2_0_2:
                    e_Version = eQMI_LOC_SUPL_VERSION_2_0_2_V02;
                    break;
                  case PDSM_SUPL_VERSION_2_0:
                    e_Version = eQMI_LOC_SUPL_VERSION_2_0_V02;
                    break;
                  case PDSM_SUPL_VERSION_2_0_4:
                    e_Version = eQMI_LOC_SUPL_VERSION_2_0_4_V02;
                    break;
                  case PDSM_SUPL_VERSION_1_0:
                  default:
                    e_Version = eQMI_LOC_SUPL_VERSION_1_0_V02;
                    break;
                }
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.suplVersion, e_Version);
              }
              break;

            case PDSM_PA_LPP_CONFIG_INFO:
              {
                qmiLocLppConfigMaskT_v02 q_LppCfgMask = 0;
                if (p_PdsmPaInfo->pa_info.lpp_config_info & PDSM_PA_LPP_IN_LTE_UP_ENABLE)
                {
                  q_LppCfgMask |= QMI_LOC_LPP_CONFIG_ENABLE_USER_PLANE_V02;
                }
                if (p_PdsmPaInfo->pa_info.lpp_config_info & PDSM_PA_LPP_IN_LTE_CP_ENABLE)
                {
                  q_LppCfgMask |= QMI_LOC_LPP_CONFIG_ENABLE_CONTROL_PLANE_V02;
                }

                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.lppConfig, q_LppCfgMask);
              }
              break;

            case PDSM_PA_ASST_GLONASS_POS_PROTOCOL:
              {
                qmiLocAssistedGlonassProtocolMaskT_v02 q_AssistedGloMask = 0;
                if (p_PdsmPaInfo->pa_info.asst_glo_protocol & PDSM_PA_ASST_GLO_PROTOCOL_RRC_CP)
                {
                  q_AssistedGloMask |= QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_RRC_CP_V02;
                }
                if (p_PdsmPaInfo->pa_info.asst_glo_protocol & PDSM_PA_ASST_GLO_PROTOCOL_RRLP_UP)
                {
                  q_AssistedGloMask |= QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_RRLP_UP_V02;
                }
                if (p_PdsmPaInfo->pa_info.asst_glo_protocol & PDSM_PA_ASST_GLO_PROTOCOL_LPP_UP)
                {
                  q_AssistedGloMask |= QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_LPP_UP_V02;
                }
                if (p_PdsmPaInfo->pa_info.asst_glo_protocol & PDSM_PA_ASST_GLO_PROTOCOL_LPP_CP)
                {
                  q_AssistedGloMask |= QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_LPP_CP_V02;
                }

                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.assistedGlonassProtocolMask, q_AssistedGloMask);
              }
              break;

            case PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG:
              {
                qmiLocEmergencyProtocolEnumT_v02 e_Protocol = 0;
                switch (p_PdsmPaInfo->pa_info.gnss_emergency_protocol_select)
                {
                  case PDSM_PA_EMERGENCY_PROTOCOL_CONFIG_WCMDA_CP:
                    {
                      e_Protocol = eQMI_LOC_EMERGENCY_PROTOCOL_WCDMA_CP_V02;
                      break;
                    }
                  case PDSM_PA_EMERGENCY_PROTOCOL_CONFIG_WCMDA_UP:
                    {
                      e_Protocol = eQMI_LOC_EMERGENCY_PROTOCOL_WCDMA_UP_V02;
                      break;
                    }
                  default:
                    {
                      LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Unknown Emergency Protocol=%d", p_PdsmPaInfo->pa_info.gnss_emergency_protocol_select, 0, 0);
                    }
                }

                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.emergencyProtocol, e_Protocol);
              }
              break;

            case PDSM_PA_SUPL_HASH_ALGORITHM:
              {
                qmiLocSuplHashAlgoEnumT_v02 e_HashAlgo = 0;
                switch (p_PdsmPaInfo->pa_info.supl_hash_algorithm)
                {
                  case PDSM_PA_SUPL_HASH_ALGO_SHA1:
                    {
                      e_HashAlgo = eQMI_LOC_SUPL_HASH_ALGO_SHA1_V02;
                      break;
                    }
                  case PDSM_PA_SUPL_HASH_ALGO_SHA256:
                    {
                      e_HashAlgo = eQMI_LOC_SUPL_HASH_ALGO_SHA256_V02;
                      break;
                    }
                  default:
                    {
                      LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Unknown Hash Algo=%d", p_PdsmPaInfo->pa_info.supl_hash_algorithm, 0, 0);
                    }
                }

                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.suplHashAlgo, e_HashAlgo);
              }
              break;

            case PDSM_PA_SUPL_TLS_VERSION:
              {
                qmiLocSuplTlsVersionEnumT_v02 e_TlsVersion = 0;
                switch (p_PdsmPaInfo->pa_info.supl_tls_version)
                {
                  case PDSM_PA_SUPL_TLS_VERSION_1_0:
                    {
                      e_TlsVersion = eQMI_LOC_SUPL_TLS_VERSION_1_0_V02;
                      break;
                    }
                  case PDSM_PA_SUPL_TLS_VERSION_1_1:
                    {
                      e_TlsVersion = eQMI_LOC_SUPL_TLS_VERSION_1_1_V02;
                      break;
                    }
                  case PDSM_PA_SUPL_TLS_VERSION_1_2:
                    {
                      e_TlsVersion = eQMI_LOC_SUPL_TLS_VERSION_1_2_V02;
                      break;
                    }
                  default:
                    {
                      LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Unknown TLS version=%d", p_PdsmPaInfo->pa_info.supl_tls_version, 0, 0);
                    }
                }

                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.suplTlsVersion, e_TlsVersion);
              }
              break;

            case PDSM_PA_WIFI_SCAN_TIMEOUT_SEC:
              {
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.wifiScanInjectTimeout,
                                       p_PdsmPaInfo->pa_info.wifi_wait_timeout_sec);
              }
              break;

            case PDSM_PA_LPPE_UP_CONFIG:
              {
                uint64 t_LppeUpConfig = 0;
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info) &&
                    (PDSM_PA_LPPE_DBH_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info))
                {
                  t_LppeUpConfig |= QMI_LOC_LPPE_MASK_UP_DBH_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info) &&
                    (PDSM_PA_LPPE_WLAN_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info))
                {
                  t_LppeUpConfig |= QMI_LOC_LPPE_MASK_UP_AP_WIFI_MEASUREMENT_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info) &&
                    (PDSM_PA_LPPE_SRN_BTLE_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info))
                {
                  t_LppeUpConfig |= QMI_LOC_LPPE_MASK_UP_AP_SRN_BTLE_MEASUREMENT_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info) &&
                    (PDSM_PA_LPPE_UBP_ENABLE & p_PdsmPaInfo->pa_info.lppe_up_config_info))
                {
                  t_LppeUpConfig |= QMI_LOC_LPPE_MASK_UP_UBP_V02;
                }
                LOC_MSG_MED("locQmi_ProcessPaGetInd lppeUpConfig = 0x%x", p_PdsmPaInfo->pa_info.lppe_up_config_info, 0, 0);
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.lppeUpConfig, t_LppeUpConfig);
              }
              break;

            case PDSM_PA_LPPE_CP_CONFIG:
              {
                uint64 t_LppeCpConfig = 0;
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info) &&
                    (PDSM_PA_LPPE_DBH_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info))
                {
                  t_LppeCpConfig |= QMI_LOC_LPPE_MASK_CP_DBH_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info) &&
                    (PDSM_PA_LPPE_WLAN_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info))
                {
                  t_LppeCpConfig |= QMI_LOC_LPPE_MASK_CP_AP_WIFI_MEASUREMENT_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info) &&
                    (PDSM_PA_LPPE_SRN_BTLE_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info))
                {
                  t_LppeCpConfig |= QMI_LOC_LPPE_MASK_UP_AP_SRN_BTLE_MEASUREMENT_V02;
                }
                if ((PDSM_PA_LPPE_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info) &&
                    (PDSM_PA_LPPE_UBP_ENABLE & p_PdsmPaInfo->pa_info.lppe_cp_config_info))
                {
                  t_LppeCpConfig |= QMI_LOC_LPPE_MASK_UP_UBP_V02;
                }
                LOC_MSG_MED("locQmi_ProcessPaGetInd lppeCpConfig = 0x%x", p_PdsmPaInfo->pa_info.lppe_cp_config_info, 0, 0);
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.lppeCpConfig, t_LppeCpConfig);
              }
              break;

            case PDSM_PA_EMERGENCY_EXTENSION_WINDOW:
              {
                LOC_QMI_SHIM_SET_VALUE(z_GetProtCfgInd.emergencyCallbackWindow,
                                       p_PdsmPaInfo->pa_info.q_emergency_extension_window_config);
              }
              break;

            default:
              {
                LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid Get Protocol Config=%d", p_PdsmPaInfo->pa_event, 0, 0);
              }
              break;
          }
        }
        else
        {
          LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid Get Protocol Status=%d", e_IndStatus, 0, 0);
        }

        LOC_MSG_MED("locQmi_ProcessPaGetInd: Get Protocol Config=%d Status=%d", p_PdsmPaInfo->pa_event, e_IndStatus, 0);
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_PE_CONFIG_PARAM:
      {
        qmiLocGetPositionEngineConfigParametersIndMsgT_v02 z_GetPeCfgInd = { 0 };
        z_IndInfo.q_Id        = QMI_LOC_GET_POSITION_ENGINE_CONFIG_PARAMETERS_IND_V02;
        z_IndInfo.q_MsgLen    = sizeof(z_GetPeCfgInd);
        z_GetPeCfgInd.status  = e_IndStatus;
        z_IndInfo.p_Msg       = &z_GetPeCfgInd;

        if (eQMI_LOC_SUCCESS_V02 != e_IndStatus)
        {
          z_GetPeCfgInd.injectedPositionControl_valid = FALSE;
          z_GetPeCfgInd.filterSvUsage_valid           = FALSE;
          z_GetPeCfgInd.storeAssistData_valid         = FALSE;
        }
        else if (NULL != p_PdsmPaInfo)
        {
          qmiLocPositionEngineConfigParamMaskT_v02 q_GetPeCfgMask = 0;
          if (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask & PDSM_PA_PE_CONFIG_PARAM_MASK_USE_INJECTED_POSITION)
          {
            LOC_QMI_SHIM_SET_VALUE(z_GetPeCfgInd.injectedPositionControl, p_PdsmPaInfo->pa_info.pe_config_info.useInjectedPosition);
            q_GetPeCfgMask |= QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_INJECTED_POSITION_CONTROL_V02;
          }

          if (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask & PDSM_PA_PE_CONFIG_PARAM_MASK_FILTER_SV_USED)
          {
            LOC_QMI_SHIM_SET_VALUE(z_GetPeCfgInd.filterSvUsage, p_PdsmPaInfo->pa_info.pe_config_info.filterSvUsed);
            q_GetPeCfgMask |= QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_FILTER_SV_USAGE_V02;
          }

          if (p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask & PDSM_PA_PE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA)
          {
            LOC_QMI_SHIM_SET_VALUE(z_GetPeCfgInd.storeAssistData, p_PdsmPaInfo->pa_info.pe_config_info.storeAssistData);
            q_GetPeCfgMask |= QMI_LOC_POSITION_ENGINE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA_V02;
          }

          /* If the info received is not equal to what was requested, notify a failure */
          if (q_GetPeCfgMask != pz_CltInfo->z_PaReqUnion.z_GetPeCfg.getPositionEngineConfigParamMask)
          {
            LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid Get PE Config Mask=%d=%d, Status=%d",
                          q_GetPeCfgMask, pz_CltInfo->z_PaReqUnion.z_GetPeCfg.getPositionEngineConfigParamMask, e_IndStatus);
            z_GetPeCfgInd.status = eQMI_LOC_GENERAL_FAILURE_V02;
          }
          else
          {
            LOC_MSG_MED("locQmi_ProcessPaGetInd: Get PE Config Mask=%d, Status=%d",
                        p_PdsmPaInfo->pa_info.pe_config_info.peConfigMask, e_IndStatus, 0);
            z_GetPeCfgInd.status = eQMI_LOC_SUCCESS_V02;
          }
        }
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_PERSISTENT_SV_MASK:
      {
        qmiLocGetBlacklistSvIndMsgT_v02 z_QmiGetBlacklistSvReqStatusInd = { 0 };
        z_IndInfo.q_Id = QMI_LOC_GET_BLACKLIST_SV_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(z_QmiGetBlacklistSvReqStatusInd);
        z_QmiGetBlacklistSvReqStatusInd.status = e_IndStatus;
        if (NULL != p_PdsmPaInfo)
        {
          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.glo_persist_blacklist_sv,
                                 p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.q_GloSlotIdMask);

          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.bds_persist_blacklist_sv,
                                 p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.t_BdsPrnMask);

          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.qzss_persist_blacklist_sv,
                                 p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.u_QzssPrnMask);

          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.gal_persist_blacklist_sv,
                                 p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.t_GalPrnMask);

          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.sbas_persist_blacklist_sv,
            p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.t_SbasPrnMask);
          LOC_QMI_SHIM_SET_VALUE(z_QmiGetBlacklistSvReqStatusInd.navic_persist_blacklist_sv,
                                 p_PdsmPaInfo->pa_info.persistent_sv_mask.z_SetMask.w_NavicPrnMask);
        }
        z_IndInfo.p_Msg = &z_QmiGetBlacklistSvReqStatusInd;
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    case PDSM_PA_CONSTELLATION_CONTROL_CONFIG:
      {
        qmiLocGetConstellationConfigIndMsgT_v02 z_QmiGnssConfigInd = { 0 };
        z_IndInfo.q_Id = QMI_LOC_GET_CONSTELLATION_CONTROL_IND_V02;
        z_IndInfo.q_MsgLen = sizeof(qmiLocGetConstellationConfigIndMsgT_v02);
        z_QmiGnssConfigInd.status = e_IndStatus;

        if (NULL != p_PdsmPaInfo)
        {
          pdsm_constellation_status_s_type *pz_PaGetGnssStatus =
             &p_PdsmPaInfo->pa_info.z_constell_cfg.z_Get;

          z_QmiGnssConfigInd.gps_status_valid = TRUE;
          z_QmiGnssConfigInd.gps_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->gps_status);

          z_QmiGnssConfigInd.glonass_status_valid = TRUE;
          z_QmiGnssConfigInd.glonass_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->glo_status);

          z_QmiGnssConfigInd.bds_status_valid = TRUE;
          z_QmiGnssConfigInd.bds_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->bds_status);

          z_QmiGnssConfigInd.qzss_status_valid = TRUE;
          z_QmiGnssConfigInd.qzss_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->qzss_status);

          z_QmiGnssConfigInd.galileo_status_valid = TRUE;
          z_QmiGnssConfigInd.galileo_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->gal_status);

          z_QmiGnssConfigInd.navic_status_valid = TRUE;
          z_QmiGnssConfigInd.navic_status =
             locQmiShimConvertConstellationStatusInd(pz_PaGetGnssStatus->navic_status);
        }

        z_IndInfo.p_Msg = &z_QmiGnssConfigInd;
        v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
      }
      break;

    default:
      {
        LOC_MSG_ERROR("locQmi_ProcessPaGetInd: Invalid Get Param=%d", e_ParamType, 0, 0);
      }
  }

  return v_RetVal;
}

typedef struct {

  qmiLocStatusEnumT_v02 status;

}qmiLocGeneralIndMsgT_v02;  /* Message */

/* -----------------------------------------------------------------------*//**
@brief
After a postion has been sent out to a QMI_LOC client, loc_qmi_shim calls
this function to decide if a QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND message
shall be reported to the master client or not

@param[in]   l_ClientHandle   Location Client Info Handle
@param[in]   q_QmiIndMsgId    The QMI_LOC message ID which has been sent out
                           to the QMI_LOC client
@param[in]   e_QmiIndMsgStatus Is position message contain success data

@retval   void
*/ /* ------------------------------------------------------------------------*/
static void locQmi_ProcessNFWPosRequestNotificationInd
(
   loc_client_handle_type       l_ClientHandle,
   uint32                       q_QmiIndMsgId,
   qmiLocStatusEnumT_v02        e_QmiIndMsgStatus
   )
{
  qmiLocRequestResponseTypeEnumT_v02  e_ResType =
     eQMI_LOC_ACCEPTED_NO_LOCATION_PROVIDED_V02;
  loc_client_info_s_type *pz_Client = NULL;

  if (QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND_V02 == q_QmiIndMsgId ||
      QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_V02 == q_QmiIndMsgId)
  {
    pz_Client = locClnt_FindHandle(l_ClientHandle);
    if (!pz_Client)
    {
      LOC_MSG_ERROR("locQmi_ProcessNFWPosRequestNotificationInd: l_ClientHandle %d not found",
                    l_ClientHandle, 0, 0);
      return;
    }
    if (eQMI_LOC_SUCCESS_V02 == e_QmiIndMsgStatus)
      e_ResType = eQMI_LOC_ACCEPTED_LOCATION_PROVIDED_V02;
    else if (eQMI_LOC_GNSS_DISABLED_V02 == e_QmiIndMsgStatus)
      e_ResType = eQMI_LOC_REJECTED_V02;

    /* both are cached position */
    locQmiSendNFWPosRequestNotificationInd(pz_Client, e_ResType, TRUE);
  }
  return;
}

/*---------------------------------------------------------------------------
@brief
  Function to process QMI_LOC Indication for commands
---------------------------------------------------------------------------*/
boolean locQmi_ProcessInd
(
   loc_client_handle_type       l_ClientHandle,
   uint32                       q_QmiIndMsgId,
   void                        *p_QmiIndMsg,
   qmiLocStatusEnumT_v02        e_QmiIndMsgStatus
   )
{
  locQmiShimIndInfoStructT   z_IndInfo = { 0 };
  locQmiShimCltInfoStructT  *pz_CltInfo = NULL;
  boolean                    v_RetVal = FALSE;
  qmiLocGeneralIndMsgT_v02   z_QmiLocGeneralInd = { 0 };

  LOC_MSG_HIGH("locQmi_ProcessInd: QMI_LOC Msg=0x%04x, ClientHandle=%d, QmiIndMsgStatus=%d",
               q_QmiIndMsgId, l_ClientHandle, e_QmiIndMsgStatus);
  z_QmiLocGeneralInd.status = e_QmiIndMsgStatus;

  pz_CltInfo = (locQmiShimCltInfoStructT *)q_linear_search(&z_LocQmiShim.z_ClientQ, locQmiShimCompareLocApiHandle, (void *)l_ClientHandle);
  if (NULL == pz_CltInfo)
  {
    LOC_MSG_ERROR("locQmi_ProcessInd: Invalid Client Info for Handle=%d", l_ClientHandle, 0, 0);
    return v_RetVal;
  }

  v_RetVal = TRUE;

  switch (q_QmiIndMsgId)
  {
    /* Process of loc_pd messages start here */
    case QMI_LOC_INJECT_POSITION_IND_V02:
      {
        locPd_ModuleDataType *pz_LocPdData = &(loc_middleware_data.z_locPd_Data);

        LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);
        pz_LocPdData->z_InjPos.v_InProgress = FALSE;
        LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.pd_crit_sec);

        z_IndInfo.q_Id       = QMI_LOC_INJECT_POSITION_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          ((qmiLocGetBestAvailablePositionIndMsgT_v02 *)p_QmiIndMsg)->status = e_QmiIndMsgStatus;
          z_IndInfo.q_Id               = QMI_LOC_GET_BEST_AVAILABLE_POSITION_IND_V02;
          z_IndInfo.q_MsgLen           = sizeof(qmiLocGetBestAvailablePositionIndMsgT_v02);
          z_IndInfo.p_Msg              = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_GET_AVAILABLE_WWAN_POSITION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_AVAILABLE_WWAN_POSITION_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id               = QMI_LOC_GET_AVAILABLE_WWAN_POSITION_IND_V02;
          z_IndInfo.q_MsgLen           = sizeof(qmiLocGetAvailWwanPositionIndMsgT_v02);
          z_IndInfo.p_Msg              = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id               = QMI_LOC_SECURE_GET_AVAILABLE_POSITION_IND_V02;
          z_IndInfo.q_MsgLen           = sizeof(qmiLocSecureGetAvailablePositionIndMsgT_v02);
          z_IndInfo.p_Msg              = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_INJECT_GSM_CELL_INFO_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_INJECT_GSM_CELL_INFO_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_WWAN_OUT_OF_SERVICE_NOTIFICATION_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_IND_V02:
      {
        z_IndInfo.q_Id        = QMI_LOC_INJECT_NETWORK_INITIATED_MESSAGE_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

      /* Process of loc_wifi messages starts here */
    case QMI_LOC_INJECT_WIFI_POSITION_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_INJECT_WIFI_POSITION_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_NOTIFY_WIFI_STATUS_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_NOTIFY_WIFI_STATUS_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_NOTIFY_WIFI_ATTACHMENT_STATUS_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_NOTIFY_WIFI_ATTACHMENT_STATUS_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_NOTIFY_WIFI_ENABLED_STATUS_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_NOTIFY_WIFI_ENABLED_STATUS_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

      /* Process of loc_client messages starts here */
    case QMI_LOC_SET_SERVER_IND_V02:
      {
        z_IndInfo.q_Id         = QMI_LOC_SET_SERVER_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_GET_SERVER_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_SERVER_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id         = QMI_LOC_GET_SERVER_IND_V02;
          z_IndInfo.q_MsgLen     = sizeof(qmiLocGetServerIndMsgT_v02);
          z_IndInfo.p_Msg        = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_START_REQ_V02:
      {
        boolean b_IndFailure = FALSE;

        if (eQMI_LOC_SUCCESS_V02 == e_QmiIndMsgStatus)
        {
          if (LOC_API_SUCCESS != loc_start_fix(pz_CltInfo->l_LocClientHandle))
          {
            LOC_MSG_ERROR("locQmi_ProcessClntInd: loc_start_fix failed", 0, 0, 0);
            b_IndFailure = TRUE;
          }
        }
        else
        {
          LOC_MSG_ERROR("locQmi_ProcessClntInd: Set Fix Criteria failed, could not start fix", 0, 0, 0);
          b_IndFailure = TRUE;
        }

        if (b_IndFailure)
        {
          /* Manufacture a Position Report with Session Failure */
          qmiLocEventPositionReportIndMsgT_v02   z_PosRptInd = { 0 };

          z_IndInfo.q_Id             = QMI_LOC_EVENT_POSITION_REPORT_IND_V02;
          z_IndInfo.q_MsgLen         = sizeof(z_PosRptInd);
          z_IndInfo.p_Msg            = &z_PosRptInd;

          if (eQMI_LOC_GNSS_DISABLED_V02 == e_QmiIndMsgStatus)
          {
            z_PosRptInd.sessionStatus  = eQMI_LOC_SESS_STATUS_ENGINE_LOCKED_V02;
            locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
            locQmiSendNFWPosRequestNotificationInd(locClnt_FindHandle(l_ClientHandle),
                                                   eQMI_LOC_REJECTED_V02, FALSE);
          }
          else
          {
          z_PosRptInd.sessionStatus  = eQMI_LOC_SESS_STATUS_GENERAL_FAILURE_V02;
          locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
            locQmiSendNFWPosRequestNotificationInd(locClnt_FindHandle(l_ClientHandle),
                                                   eQMI_LOC_ACCEPTED_NO_LOCATION_PROVIDED_V02, FALSE);
        }
        }

        /* Nothing more to do here. There is no indication that needs to be sent. */
        v_RetVal = FALSE;
        break;
      }

    case QMI_LOC_GET_FIX_CRITERIA_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_FIX_CRITERIA_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id          = QMI_LOC_GET_FIX_CRITERIA_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(qmiLocGetFixCriteriaIndMsgT_v02);
          z_IndInfo.p_Msg         = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_GET_REGISTERED_EVENTS_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_REGISTERED_EVENTS_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id          = QMI_LOC_GET_REGISTERED_EVENTS_IND_V02;
          z_IndInfo.q_MsgLen      = sizeof(qmiLocGetRegisteredEventsIndMsgT_v02);
          z_IndInfo.p_Msg         = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_SET_OPERATION_MODE_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_SET_OPERATION_MODE_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_GET_OPERATION_MODE_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_OPERATION_MODE_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id         = QMI_LOC_GET_OPERATION_MODE_IND_V02;
          z_IndInfo.q_MsgLen     = sizeof(qmiLocGetOperationModeIndMsgT_v02);
          z_IndInfo.p_Msg        = p_QmiIndMsg;
        }
        break;
      }

      /* Process of XTRA messages starts here */
    case QMI_LOC_INJECT_XTRA_DATA_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_INJECT_XTRA_DATA_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_INJECT_XTRA_DATA_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocInjectXtraDataIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_GET_PREDICTED_ORBITS_DATA_SOURCE_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_SERVER_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_GET_PREDICTED_ORBITS_DATA_SOURCE_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocGetPredictedOrbitsDataSourceIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_GET_PREDICTED_ORBITS_DATA_VALIDITY_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_SERVER_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_GET_PREDICTED_ORBITS_DATA_VALIDITY_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocGetPredictedOrbitsDataValidityIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_INJECT_UTC_TIME_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_INJECT_UTC_TIME_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_QUERY_XTRA_INFO_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_QUERY_XTRA_INFO_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_QUERY_XTRA_INFO_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocQueryXtraInfoIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

      /* Process of loc_ni messages starts here */
    case QMI_LOC_NI_USER_RESPONSE_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_NI_USER_RESPONSE_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id         = QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_IND_V02;
          z_IndInfo.q_MsgLen     = sizeof(qmiLocGetProtocolConfigParametersIndMsgT_v02);
          z_IndInfo.p_Msg        = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id         = QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_IND_V02;
          z_IndInfo.q_MsgLen     = sizeof(qmiLocSetProtocolConfigParametersIndMsgT_v02);
          z_IndInfo.p_Msg        = p_QmiIndMsg;
        }
        break;
      }

      /* Process of loc_conn messages starts here */
    case QMI_LOC_INFORM_LOCATION_SERVER_CONN_STATUS_IND_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_INFORM_LOCATION_SERVER_CONN_STATUS_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

      /* Process of loc_qmi_shim messages starts here */
    case QMI_LOC_GET_SERVICE_REVISION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_SERVICE_REVISION_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_GET_SERVICE_REVISION_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocGetServiceRevisionIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_INJECT_TIME_ZONE_INFO_REQ_V02:
      {
        z_IndInfo.q_Id       = QMI_LOC_INJECT_TIME_ZONE_INFO_IND_V02;
        z_IndInfo.q_MsgLen   = sizeof(z_QmiLocGeneralInd);
        z_IndInfo.p_Msg      = &z_QmiLocGeneralInd;
        break;
      }

    case QMI_LOC_GET_SUPPORTED_FEATURE_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_GET_SUPPORTED_FEATURE_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_GET_SUPPORTED_FEATURE_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocGetSupportedFeatureIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_REGISTER_MASTER_CLIENT_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_REGISTER_MASTER_CLIENT_IND", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_REGISTER_MASTER_CLIENT_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocRegisterMasterClientIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_LOCATION_REQUEST_NOTIFICATION_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocLocationRequestNotificationIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
    case QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_IND_V02", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_ENABLE_POSITION_INJECTION_TEST_MODE_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocEnablePositionInjectionTestModeIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }

    case QMI_LOC_INJECT_TEST_POSITION_IND_V02:
      {
        if (NULL == p_QmiIndMsg)
        {
          LOC_MSG_ERROR("locQmi_ProcessInd: NULL p_QmiIndMsg for QMI_LOC_INJECT_TEST_POSITION_IND_V02", 0, 0, 0);
          v_RetVal = FALSE;
        }
        else
        {
          z_IndInfo.q_Id       = QMI_LOC_INJECT_TEST_POSITION_IND_V02;
          z_IndInfo.q_MsgLen   = sizeof(qmiLocInjectTestPositionIndMsgT_v02);
          z_IndInfo.p_Msg      = p_QmiIndMsg;
        }
        break;
      }
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

    default:
      {
        LOC_MSG_ERROR("locQmi_ProcessInd: Invalid QmiMsgId=0x%04x", q_QmiIndMsgId, 0, 0);
        v_RetVal = FALSE;
        break;
      }
  }

  if (TRUE == v_RetVal)
  {
    v_RetVal = locQmiShimSendInd(pz_CltInfo->l_LocClientHandle, &z_IndInfo);
    if (v_RetVal)
    {
      locQmi_ProcessNFWPosRequestNotificationInd(l_ClientHandle,
                                                 q_QmiIndMsgId, e_QmiIndMsgStatus);
    }
  }

  return v_RetVal;
}


/* -----------------------------------------------------------------------*//**
@brief
This function is to be called by the module that intends to broadcast an 
indication to TDP clients of the QMI-LOC, Service. 

@return TRUE    The indication was successfully sent over QCSI
@return FALSE   The indication was not successfully sent over QCSI

@warning
This function assumes that it is called in the context of the service 
providers task. ( Loc-MW task )
*/ /* ------------------------------------------------------------------------*/
boolean locQmiShimSendTdpInd(const locQmiShimIndInfoStructT *pz_IndInfo)
{
  boolean b_RetVal = FALSE;
  locQmiShimCltInfoStructT   *pz_CltInfo = NULL;
  uint32 error_code = 1;
  boolean b_TdpClientExist = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL != pz_IndInfo)
  {
    LOC_MSG_MED("locQmiShimSendTdpInd: Shim Send TDP Ind:0x%04x, msg_len:%d",
                pz_IndInfo->q_Id, pz_IndInfo->q_MsgLen, 0);
    for (pz_CltInfo = q_check(&z_LocQmiShim.z_ClientQ);
         pz_CltInfo != NULL;
         pz_CltInfo = q_next(&(z_LocQmiShim.z_ClientQ), &(pz_CltInfo->z_Link)))
    {
      if (pz_CltInfo->v_IsTdpClient)
      {
        /* this is a TDP client */
        b_TdpClientExist = TRUE;
        if (TRUE == locQmiShimSendInd(pz_CltInfo->l_LocClientHandle,
                                      pz_IndInfo))
        {
          LOC_MSG_MED("locQmiShimSendTdpInd: Send TDP Ind:Clt:%u, msg:0x%04x",
                      pz_CltInfo->p_QmiClientHandle, pz_IndInfo->q_Id, 0);
          error_code = 0;
        }
        else
        {
          LOC_MSG_MED("locQmiShimSendTdpInd: Could Not send TDP Ind:Clt:%u, msg:0x%04x",
                      pz_CltInfo->p_QmiClientHandle, pz_IndInfo->q_Id, 0);
          error_code = 1;
        }
      }
    }
  }
  else
  {
    LOC_MSG_HIGH("locQmiShimSendTdpInd: Could Not send Ind", 0, 0, 0);
    error_code = 2;
  }

  if (0 != error_code)
  {
    LOC_MSG_ERROR("locQmiShimSendTdpInd: error: %u", error_code, 0, 0);
  }
  else
  {
    b_RetVal = TRUE;
  }

  if (!b_TdpClientExist)
  {
    LOC_MSG_MED("locQmiShimSendTdpInd: There is no active TDP client", 0, 0, 0);
  }

  return b_RetVal;
}
