/******************************************************************************
  @file: loc_mgp_iface.c
  @brief:   This module handles LocMW - MGP Interface
  

  DESCRIPTION
   Qualcomm Location API MGP Module

  INITIALIZATION AND SEQUENCING REQUIREMENTS
   N/A

  -----------------------------------------------------------------------------
  Copyright (c) 2019 - 2020 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_mgp_iface.c#2 $
$DateTime: 2020/09/01 21:19:44 $
******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
08/31/18            Intial version
======================================================================*/
#include "loc_api_2.h"
#include "loc_api_internal.h"
#include "loc_utils.h"
#include "loc_qmi_shim.h"
#include "loc_mgp_iface.h"

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
void locMgp_ReportEphemerisIndProxy
(
  const gnss_EphemerisDataType* pz_GnssEphemerisData
)
{
  LOC_MSG_LOW_1("locMgp_ReportEphemerisIndProxy: Send IPC %u to LocMW", 
    LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT);
  
  if (NULL == pz_GnssEphemerisData)
  {
    LOC_MSG_ERROR_0("locMgp_ReportEphemerisIndProxy: NULL pointer");
    return;
  }

  /** Generate an IPC to loc MW*/
  if (FALSE == locMW_SendIpcMsg(
    LM_MIDDLEWARE_MSG_ID_GNSS_EPHEMERIS_REPORT,
    sizeof(*pz_GnssEphemerisData),
    (void *)pz_GnssEphemerisData))
  {
    LOC_MSG_ERROR_0("locMgp_ReportEphemerisIndProxy: could not send IPC to loc MW");
  }
}

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
//Utility function to fill updateAction for QMI indication
qmiLocEphUpdateActionEnumT_v02 populateEphUpdateAction(gnss_EphemUpdateActionType u_SrcAction)
{
  switch (u_SrcAction)
  {
    case  C_GNSS_EPHEM_UPDATE_SRC_SM:
      return eQMI_LOC_UPDATE_EPH_SRC_NETWORK_V02;

    case  C_GNSS_EPHEM_UPDATE_SRC_OTA:
      return eQMI_LOC_UPDATE_EPH_SRC_OTA_V02;

    case  C_GNSS_EPHEM_UPDATE_SRC_EFS:
      return eQMI_LOC_UPDATE_EPH_SRC_UNKNOWN_V02;

    case  C_GNSS_EPHEM_DELETE_SRC_SM:
      return eQMI_LOC_DELETE_EPH_SRC_NETWORK_V02;

    case  C_GNSS_EPHEM_DELETE_SRC_OTA:
      return eQMI_LOC_DELETE_EPH_SRC_OTA_V02;

    case  C_GNSS_EPHEM_DELETE_SRC_EFS:
      return eQMI_LOC_DELETE_EPH_SRC_UNKNOWN_V02;

    default:
      return eQMI_LOC_DELETE_EPH_SRC_UNKNOWN_V02;
  }
}

//Utility function to fill common ephemeris data for QMI indication
static void fillCommonEphData(qmiLocEphGnssDataStructT_v02* pz_Dest, gnss_CommonEpheType* pz_Src)
{
  pz_Dest->gnssSvId = pz_Src->w_SvId;
  pz_Dest->updateAction = populateEphUpdateAction(pz_Src->u_UpdateAction);
  pz_Dest->IODE = pz_Src->w_Iode;
  pz_Dest->aSqrt = pz_Src->d_aSqrt;
  pz_Dest->deltaN = pz_Src->d_DeltaN;
  pz_Dest->m0 = pz_Src->d_M0;
  pz_Dest->eccentricity = pz_Src->d_Eccentricity;
  pz_Dest->omega0 = pz_Src->d_Omega0;
  pz_Dest->i0 = pz_Src->d_I0;
  pz_Dest->omega = pz_Src->d_Omega;
  pz_Dest->omegaDot = pz_Src->d_OmegaDot;
  pz_Dest->iDot = pz_Src->d_iDot;
  pz_Dest->cUc = pz_Src->d_cUc;
  pz_Dest->cUs = pz_Src->d_cUs;
  pz_Dest->cRc = pz_Src->d_cRc;
  pz_Dest->cRs = pz_Src->d_cRs;
  pz_Dest->cIc = pz_Src->d_cIc;
  pz_Dest->cIs = pz_Src->d_cIs;
  pz_Dest->toe = pz_Src->q_ToeSec;
  pz_Dest->toc = pz_Src->q_Toc;
  pz_Dest->af0 = pz_Src->d_Af0;
  pz_Dest->af1 = pz_Src->d_Af1;
  pz_Dest->af2 = pz_Src->d_Af2;
}

//Utility function to fill GPS system time of ephemeris report
static boolean fillEphReportingTime(qmiLocGnssTimeStructT_v02* pz_QmiEphTimeOut, gps_TimeStructType* pz_MgpEphTimeIn)
{
  if ((NULL == pz_QmiEphTimeOut) || (NULL == pz_MgpEphTimeIn) ||
      (C_TIME_SRC_INVALID == pz_MgpEphTimeIn->e_SourceOfTime))
  {
    LOC_MSG_ERROR_2("fillEphReportingTime: NULL Input Ptr, QMI=%d GNSS=%d, Or Invalid Time", 
                    (NULL==pz_QmiEphTimeOut), (NULL==pz_MgpEphTimeIn));
    return FALSE;
  }
  pz_QmiEphTimeOut->system = eQMI_LOC_SV_SYSTEM_GPS_V02;
  pz_QmiEphTimeOut->systemClkTimeBias = pz_MgpEphTimeIn->f_ClkTimeBias;
  pz_QmiEphTimeOut->systemClkTimeUncMs = pz_MgpEphTimeIn->f_ClkTimeUncMs;
  pz_QmiEphTimeOut->systemMsec = pz_MgpEphTimeIn->q_GpsMsec;
  pz_QmiEphTimeOut->systemWeek = pz_MgpEphTimeIn->w_GpsWeek;
  return TRUE;
}

//Utility function to fill GPS ephemeris report and send indication to client
static void sendGpsEphemerisInd(gnss_EphemerisDataType* pz_GnssEphemerisData)
{
  qmiLocGpsEphemerisReportIndMsgT_v02* pz_QmiGpsEph = NULL;
  gnss_GpsEpheDataType* pz_MgpGpsQzssEph = NULL;
  uint8 i = 0;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_GPS_EPHEMERIS_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiGpsEph)};

  LOC_MSG_HIGH_0("sendGpsEphemerisInd");

  if ((NULL == pz_GnssEphemerisData) || (MGP_GNSS_TYPE_GPS != pz_GnssEphemerisData->u_GnssSys))
  {
    LOC_MSG_ERROR_2("SanityCheckFail NULL=%d System=%d", (NULL==pz_GnssEphemerisData),
                    (NULL!=pz_GnssEphemerisData) ? pz_GnssEphemerisData->u_GnssSys : -1);
    return;
  }

  pz_QmiGpsEph = (qmiLocGpsEphemerisReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocGpsEphemerisReportIndMsgT_v02), OS_MEM_SCOPE_TASK);
  if (NULL == pz_QmiGpsEph)
  {
    LOC_MSG_ERROR_0("sendGpsEphemerisInd memAlloc failed");
    return;
  }
  ind_info.p_Msg = (void *)pz_QmiGpsEph;
  pz_MgpGpsQzssEph = &pz_GnssEphemerisData->y_EphemData.z_GpsData[0];
  pz_QmiGpsEph->gpsEphemerisList_len = pz_GnssEphemerisData->u_ValidSvCount;
  for (i = 0; i < pz_QmiGpsEph->gpsEphemerisList_len; i++)
  {
    fillCommonEphData(&pz_QmiGpsEph->gpsEphemerisList[i].commonEphemerisData,
                      &pz_MgpGpsQzssEph[i].z_EpheMain);

    pz_QmiGpsEph->gpsEphemerisList[i].signalHealth =
      pz_MgpGpsQzssEph[i].z_EpheAux.u_SvHealth;

    pz_QmiGpsEph->gpsEphemerisList[i].URAI =
      pz_MgpGpsQzssEph[i].z_EpheAux.u_Accuracy;

    pz_QmiGpsEph->gpsEphemerisList[i].codeL2 =
      pz_MgpGpsQzssEph[i].z_EpheAux.u_CodeL2;

    pz_QmiGpsEph->gpsEphemerisList[i].dataFlagL2P =
      pz_MgpGpsQzssEph[i].z_EpheAux.u_PcodeFlag;

    pz_QmiGpsEph->gpsEphemerisList[i].tgd =
      pz_MgpGpsQzssEph[i].z_EpheAux.d_Tgd;

    pz_QmiGpsEph->gpsEphemerisList[i].fitInterval =
      pz_MgpGpsQzssEph[i].z_EpheAux.u_FitInterval;

    pz_QmiGpsEph->gpsEphemerisList[i].IODC =
      pz_MgpGpsQzssEph[i].z_EpheAux.w_Iodc;
  }

  /* Populate optional TLVs */
  pz_QmiGpsEph->gpsSystemTime_valid = fillEphReportingTime(&pz_QmiGpsEph->gpsSystemTime, &pz_GnssEphemerisData->z_GpsSystemTime);

  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_EPHEMERIS_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("sendGpsEphemerisInd: IND could not be sent");
  }
  os_MemFree((void**)&pz_QmiGpsEph);
}

//Utility function to fill GLO ephemeris report and send indication to client
static void sendGloEphemerisInd(gnss_EphemerisDataType* pz_GnssEphemerisData)
{
  qmiLocGloEphemerisReportIndMsgT_v02* pz_QmiGloEph = NULL;
  gnss_GloEpheDataType* pz_MgpGloEph = NULL;
  uint8 i = 0;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_GLONASS_EPHEMERIS_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiGloEph)};

  LOC_MSG_HIGH_0("sendGloEphemerisInd");

  if ((NULL == pz_GnssEphemerisData) || (MGP_GNSS_TYPE_GLO != pz_GnssEphemerisData->u_GnssSys))
  {
    LOC_MSG_ERROR_2("SanityCheckFail NULL=%d System=%d", (NULL == pz_GnssEphemerisData),
      (NULL != pz_GnssEphemerisData) ? pz_GnssEphemerisData->u_GnssSys : -1);
    return;
  }

  pz_QmiGloEph = (qmiLocGloEphemerisReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocGloEphemerisReportIndMsgT_v02), OS_MEM_SCOPE_TASK);
  if (NULL == pz_QmiGloEph)
  {
    LOC_MSG_ERROR_0("sendGloEphemerisInd memAlloc failed");
    return;
  }
  ind_info.p_Msg = (void *)pz_QmiGloEph;
  pz_MgpGloEph = &pz_GnssEphemerisData->y_EphemData.z_GloData[0];
  pz_QmiGloEph->gloEphemerisList_len = pz_GnssEphemerisData->u_ValidSvCount;

  for (i = 0; i < pz_QmiGloEph->gloEphemerisList_len; i++)
  {
    pz_QmiGloEph->gloEphemerisList[i].updateAction = populateEphUpdateAction(pz_MgpGloEph[i].u_UpdateAction);
    pz_QmiGloEph->gloEphemerisList[i].gnssSvId =
      pz_MgpGloEph[i].w_SvId;

    pz_QmiGloEph->gloEphemerisList[i].bnHealth =
      pz_MgpGloEph[i].u_BnHealth;

    pz_QmiGloEph->gloEphemerisList[i].lnHealth =
      pz_MgpGloEph[i].u_LnHealth;

    pz_QmiGloEph->gloEphemerisList[i].tb =
      pz_MgpGloEph[i].q_Tb;

    pz_QmiGloEph->gloEphemerisList[i].ft =
      pz_MgpGloEph[i].u_Accuracy;

    pz_QmiGloEph->gloEphemerisList[i].gloM =
      pz_MgpGloEph[i].u_GloM;

    pz_QmiGloEph->gloEphemerisList[i].enAge =
      pz_MgpGloEph[i].u_EnAge;

    pz_QmiGloEph->gloEphemerisList[i].gloFrequency =
      pz_MgpGloEph[i].b_FreqNum + 8;

    pz_QmiGloEph->gloEphemerisList[i].p1 =
      pz_MgpGloEph[i].u_P1;

    pz_QmiGloEph->gloEphemerisList[i].p2 =
      pz_MgpGloEph[i].u_P2;

    pz_QmiGloEph->gloEphemerisList[i].deltaTau =
      pz_MgpGloEph[i].f_DeltaL1L2;

    memscpy(&pz_QmiGloEph->gloEphemerisList[i].position, sizeof(pz_QmiGloEph->gloEphemerisList[i].position),
            &pz_MgpGloEph[i].d_Pos, sizeof(pz_MgpGloEph[i].d_Pos));

    memscpy(&pz_QmiGloEph->gloEphemerisList[i].velocity, sizeof(pz_QmiGloEph->gloEphemerisList[i].velocity),
            &pz_MgpGloEph[i].d_Vel, sizeof(pz_MgpGloEph[i].d_Vel));

    memscpy(&pz_QmiGloEph->gloEphemerisList[i].acceleration, sizeof(pz_QmiGloEph->gloEphemerisList[i].acceleration),
            &pz_MgpGloEph[i].d_Acc, sizeof(pz_MgpGloEph[i].d_Acc));


    pz_QmiGloEph->gloEphemerisList[i].tauN =
      pz_MgpGloEph[i].f_TauN;

    pz_QmiGloEph->gloEphemerisList[i].gamma =
      pz_MgpGloEph[i].f_GammaFreqOff;

    pz_QmiGloEph->gloEphemerisList[i].toe =
      pz_MgpGloEph[i].d_Toe;

    pz_QmiGloEph->gloEphemerisList[i].nt =
      pz_MgpGloEph[i].w_Nt;
  }

  /* Populate optional TLVs */
  pz_QmiGloEph->gpsSystemTime_valid = fillEphReportingTime(&pz_QmiGloEph->gpsSystemTime, &pz_GnssEphemerisData->z_GpsSystemTime);
  
  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_EPHEMERIS_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("sendGloEphemerisInd: IND could not be sent");
  }

  os_MemFree((void**)&pz_QmiGloEph);
}

//Utility function to fill BDS ephemeris report and send indication to client
static void sendBdsEphemerisInd(gnss_EphemerisDataType* pz_GnssEphemerisData)
{
  qmiLocBdsEphemerisReportIndMsgT_v02* pz_QmiBdsEph = NULL;
  gnss_BdsEpheDataType* pz_MgpBdsEph = NULL;
  uint8 i = 0;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_BDS_EPHEMERIS_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiBdsEph)};

  LOC_MSG_HIGH_0("sendBdsEphemerisInd");

  if ((NULL == pz_GnssEphemerisData) || (MGP_GNSS_TYPE_BDS != pz_GnssEphemerisData->u_GnssSys))
  {
    LOC_MSG_ERROR_2("SanityCheckFail NULL=%d System=%d", (NULL == pz_GnssEphemerisData),
      (NULL != pz_GnssEphemerisData) ? pz_GnssEphemerisData->u_GnssSys : -1);
    return;
  }

  pz_QmiBdsEph = (qmiLocBdsEphemerisReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocBdsEphemerisReportIndMsgT_v02), OS_MEM_SCOPE_TASK);
  if (NULL == pz_QmiBdsEph)
  {
    LOC_MSG_ERROR_0("sendBdsEphemerisInd memAlloc failed");
    return;
  }
  ind_info.p_Msg = (void *)pz_QmiBdsEph;
  pz_MgpBdsEph = &pz_GnssEphemerisData->y_EphemData.z_BdsData[0];
  pz_QmiBdsEph->bdsEphemerisList_len = pz_GnssEphemerisData->u_ValidSvCount;
  for (i = 0; i < pz_QmiBdsEph->bdsEphemerisList_len; i++)
  {
    fillCommonEphData(&pz_QmiBdsEph->bdsEphemerisList[i].commonEphemerisData,
                      &pz_MgpBdsEph[i].z_EpheMain);

    pz_QmiBdsEph->bdsEphemerisList[i].svHealth =
      pz_MgpBdsEph[i].z_EpheAux.u_Health;

    pz_QmiBdsEph->bdsEphemerisList[i].AODC =
      pz_MgpBdsEph[i].z_EpheAux.u_Aodc;

    pz_QmiBdsEph->bdsEphemerisList[i].tgd1 =
      pz_MgpBdsEph[i].z_EpheAux.d_Tgd1;

    pz_QmiBdsEph->bdsEphemerisList[i].tgd2 =
      pz_MgpBdsEph[i].z_EpheAux.d_Tgd2;

    pz_QmiBdsEph->bdsEphemerisList[i].URAI =
      pz_MgpBdsEph[i].z_EpheAux.u_Accuracy;
  }

  /* Populate optional TLVs */
  pz_QmiBdsEph->gpsSystemTime_valid = fillEphReportingTime(&pz_QmiBdsEph->gpsSystemTime, &pz_GnssEphemerisData->z_GpsSystemTime);

  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_EPHEMERIS_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("sendBdsEphemerisInd: IND could not be sent");
  }

  os_MemFree((void**)&pz_QmiBdsEph);
}

//Utility function to fill GAL ephemeris report and send indication to client
static void sendGalEphemerisInd(gnss_EphemerisDataType* pz_GnssEphemerisData)
{
  qmiLocGalEphemerisReportIndMsgT_v02* pz_QmiGalEph = NULL;
  gnss_GalEpheDataType* pz_MgpGalEph = NULL;
  uint8 i = 0;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_GALILEO_EPHEMERIS_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiGalEph)};

  LOC_MSG_HIGH_0("sendGalEphemerisInd");

  if ((NULL == pz_GnssEphemerisData) || (MGP_GNSS_TYPE_GAL != pz_GnssEphemerisData->u_GnssSys))
  {
    LOC_MSG_ERROR_2("SanityCheckFail NULL=%d System=%d", (NULL == pz_GnssEphemerisData),
      (NULL != pz_GnssEphemerisData) ? pz_GnssEphemerisData->u_GnssSys : -1);
    return;
  }

  pz_QmiGalEph = (qmiLocGalEphemerisReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocGalEphemerisReportIndMsgT_v02), OS_MEM_SCOPE_TASK);
  if (NULL == pz_QmiGalEph)
  {
    LOC_MSG_ERROR_0("sendGalEphemerisInd memAlloc failed");
    return;
  }
  ind_info.p_Msg = (void *)pz_QmiGalEph;
  pz_MgpGalEph = &pz_GnssEphemerisData->y_EphemData.z_GalData[0];
  pz_QmiGalEph->galEphemerisList_len = pz_GnssEphemerisData->u_ValidSvCount;
  for (i = 0; i < pz_QmiGalEph->galEphemerisList_len; i++)
  {
    fillCommonEphData(&pz_QmiGalEph->galEphemerisList[i].commonEphemerisData,
                      &pz_MgpGalEph[i].z_EpheMain);

    pz_QmiGalEph->galEphemerisList[i].dataSourceSignal =
      pz_MgpGalEph[i].z_EpheAux.u_EphSignalSrc;

    pz_QmiGalEph->galEphemerisList[i].sisIndex =
      pz_MgpGalEph[i].z_EpheAux.u_SisaIdx;

    pz_QmiGalEph->galEphemerisList[i].bgdE1E5a =
      pz_MgpGalEph[i].z_EpheAux.d_BgdE1E5a;

    pz_QmiGalEph->galEphemerisList[i].bgdE1E5b =
      pz_MgpGalEph[i].z_EpheAux.d_BgdE1E5b;

    pz_QmiGalEph->galEphemerisList[i].svHealth =
      pz_MgpGalEph[i].z_EpheAux.u_Health;
  }

  /* Populate optional TLVs */
  pz_QmiGalEph->gpsSystemTime_valid = fillEphReportingTime(&pz_QmiGalEph->gpsSystemTime, &pz_GnssEphemerisData->z_GpsSystemTime);

  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_EPHEMERIS_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("sendGalEphemerisInd: IND could not be sent");
  }

  os_MemFree((void**)&pz_QmiGalEph);
}

//Utility function to fill QZSS ephemeris report and send indication to client
static void sendQzssEphemerisInd(gnss_EphemerisDataType* pz_GnssEphemerisData)
{
  qmiLocQzssEphemerisReportIndMsgT_v02* pz_QmiQzssEph = NULL;
  gnss_QzssEpheDataType* pz_MgpQzssQzssEph = NULL;
  uint8 i = 0;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_QZSS_EPHEMERIS_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiQzssEph)};

  LOC_MSG_HIGH_0("sendQzssEphemerisInd");

  if ((NULL == pz_GnssEphemerisData) || (MGP_GNSS_TYPE_QZSS != pz_GnssEphemerisData->u_GnssSys))
  {
    LOC_MSG_ERROR_2("SanityCheckFail NULL=%d System=%d", (NULL == pz_GnssEphemerisData),
      (NULL != pz_GnssEphemerisData) ? pz_GnssEphemerisData->u_GnssSys : -1);
    return;
  }

  pz_QmiQzssEph = (qmiLocQzssEphemerisReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocQzssEphemerisReportIndMsgT_v02), OS_MEM_SCOPE_TASK); 
  if (NULL == pz_QmiQzssEph)
  {
    LOC_MSG_ERROR_0("sendQzssEphemerisInd memAlloc failed");
    return;
  }
  ind_info.p_Msg = (void *)pz_QmiQzssEph;
  pz_MgpQzssQzssEph = &pz_GnssEphemerisData->y_EphemData.z_QzssData[0];
  pz_QmiQzssEph->qzssEphemerisList_len = pz_GnssEphemerisData->u_ValidSvCount;
  for (i = 0; i < pz_QmiQzssEph->qzssEphemerisList_len; i++)
  {
    fillCommonEphData(&pz_QmiQzssEph->qzssEphemerisList[i].commonEphemerisData,
                      &pz_MgpQzssQzssEph[i].z_EpheMain);

    pz_QmiQzssEph->qzssEphemerisList[i].signalHealth =
      pz_MgpQzssQzssEph[i].z_EpheAux.u_SvHealth;

    pz_QmiQzssEph->qzssEphemerisList[i].URAI =
      pz_MgpQzssQzssEph[i].z_EpheAux.u_Accuracy;

    pz_QmiQzssEph->qzssEphemerisList[i].codeL2 =
      pz_MgpQzssQzssEph[i].z_EpheAux.u_CodeL2;

    pz_QmiQzssEph->qzssEphemerisList[i].dataFlagL2P =
      pz_MgpQzssQzssEph[i].z_EpheAux.u_PcodeFlag;

    pz_QmiQzssEph->qzssEphemerisList[i].tgd =
      pz_MgpQzssQzssEph[i].z_EpheAux.d_Tgd;

    pz_QmiQzssEph->qzssEphemerisList[i].fitInterval =
      pz_MgpQzssQzssEph[i].z_EpheAux.u_FitInterval;

    pz_QmiQzssEph->qzssEphemerisList[i].IODC =
      pz_MgpQzssQzssEph[i].z_EpheAux.w_Iodc;
  }

  /* Populate optional TLVs */
  pz_QmiQzssEph->gpsSystemTime_valid = fillEphReportingTime(&pz_QmiQzssEph->gpsSystemTime, &pz_GnssEphemerisData->z_GpsSystemTime);

  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_EPHEMERIS_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("sendQzssEphemerisInd: IND could not be sent");
  }
  os_MemFree((void**)&pz_QmiQzssEph);
}

void locMgp_HandleEphemerisReport(const os_IpcMsgType *cpz_IpcMsg)
{
  gnss_EphemerisDataType *pz_GnssEphemerisData = NULL; 

  LOC_MSG_MED_0("locMgp_HandleEphemerisReport"); 

  if (NULL == cpz_IpcMsg)
  {
    LOC_MSG_ERROR_0("locMgp_HandleEphemerisReport: NULL cpz_IpcMsg");
    return;
  }

  pz_GnssEphemerisData = (gnss_EphemerisDataType*)cpz_IpcMsg->p_Data;

  switch (pz_GnssEphemerisData->u_GnssSys)
  {
    case MGP_GNSS_TYPE_GPS:
      sendGpsEphemerisInd(pz_GnssEphemerisData);
      break;

    case MGP_GNSS_TYPE_GLO:
      sendGloEphemerisInd(pz_GnssEphemerisData);
      break;

    case MGP_GNSS_TYPE_BDS:
      sendBdsEphemerisInd(pz_GnssEphemerisData);
      break;

    case MGP_GNSS_TYPE_GAL:
      sendGalEphemerisInd(pz_GnssEphemerisData);
      break;
    
    case MGP_GNSS_TYPE_QZSS:
      sendQzssEphemerisInd(pz_GnssEphemerisData);
      break;

    default:
      LOC_MSG_ERROR_1("locMgp_HandleEphemerisReport: Invalid GNSS Type %d", pz_GnssEphemerisData->u_GnssSys);
      break;
  }
}

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
void locMw_ReportEvent(gnss_QmiEventReportT* pz_EventData)
{
  /** Generate an IPC to loc MW*/
  if (FALSE == locMW_SendIpcMsg(
    LM_MIDDLEWARE_MSG_ID_GNSS_EVENT_REPORT,
    sizeof(*pz_EventData),
    (void *)pz_EventData))
  {
    LOC_MSG_ERROR_0("locMw_ReportEvent: could not send IPC to loc MW");
  }
}

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
void loc_HandleEventReport(const os_IpcMsgType *cpz_IpcMsg)
{
  gnss_QmiEventReportT* pz_EventData = NULL;
  qmiLocEventReportIndMsgT_v02* pz_QmiEventReport = NULL;
  locQmiShimIndInfoStructT ind_info = {QMI_LOC_EVENT_REPORT_IND_V02, NULL, (int32)sizeof(*pz_QmiEventReport)};

  if (NULL == cpz_IpcMsg)
  {
    LOC_MSG_ERROR_0("loc_HandleEventReport: NULL cpz_IpcMsg");
    return;
  }
  pz_EventData = (gnss_QmiEventReportT*)cpz_IpcMsg->p_Data;

  pz_QmiEventReport = (qmiLocEventReportIndMsgT_v02 *)os_MemAlloc(sizeof(qmiLocEventReportIndMsgT_v02), OS_MEM_SCOPE_TASK);
  if (NULL == pz_QmiEventReport)
  {
    LOC_MSG_ERROR_0("loc_HandleEventReport: Malloc Failed for IND");
    return;
  }
  memset(pz_QmiEventReport, 0, sizeof(*pz_QmiEventReport));

  //TLV 0x01 Mandatory Event Report Mask
//  pz_QmiEventReport->eventReport = pz_EventData->t_EventReportMask;
  if ( C_GNSS_EP_DELETE_GPS_EPHEMERIS_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GPS_EPHEMERIS_ALL_V02;
  }
  if ( C_GNSS_EP_DELETE_GPS_SV_POLY_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GPS_SV_POLY_ALL_V02;
  }

  if ( C_GNSS_EP_DELETE_GLO_EPHEMERIS_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GLO_EPHEMERIS_ALL_V02;
  }
  if ( C_GNSS_EP_DELETE_GLO_SV_POLY_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GLO_SV_POLY_ALL_V02;
  }

  if ( C_GNSS_EP_DELETE_BDS_EPHEMERIS_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_BDS_EPHEMERIS_ALL_V02;
  }
  if ( C_GNSS_EP_DELETE_BDS_SV_POLY_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_BDS_SV_POLY_ALL_V02;
  }

  if ( C_GNSS_EP_DELETE_GAL_EPHEMERIS_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GAL_EPHEMERIS_ALL_V02;
  }
  if ( C_GNSS_EP_DELETE_GAL_SV_POLY_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_GAL_SV_POLY_ALL_V02;
  }

  if ( C_GNSS_EP_DELETE_QZSS_EPHEMERIS_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_QZSS_EPHEMERIS_ALL_V02;
  }
  if ( C_GNSS_EP_DELETE_QZSS_SV_POLY_ALL & pz_EventData->t_EventReportMask )
  {
    pz_QmiEventReport->eventReport |= QMI_LOC_DELETE_QZSS_SV_POLY_ALL_V02;
  }

  if ( C_GNSS_EP_DELETE_GPS_IONO & pz_EventData->t_EventReportMask )
  {
    LOC_MSG_HIGH_0("loc_HandleEventReport: GPS IONO Delete not supported");
  }
  if ( C_GNSS_EP_DELETE_BDS_IONO & pz_EventData->t_EventReportMask )
  {
    LOC_MSG_HIGH_0("loc_HandleEventReport: BDS IONO Delete not supported");
  }
  if ( C_GNSS_EP_DELETE_QZSS_IONO & pz_EventData->t_EventReportMask )
  {
    LOC_MSG_HIGH_0("loc_HandleEventReport: QZSS IONO Delete not supported");
  }

  //TLV 0x10 - GPS System Time
  if (pz_EventData->t_ValidityMask & GNSS_QMI_EVENT_GPS_TIME_VALID)
  {
    pz_QmiEventReport->gpsSystemTime_valid = TRUE;
    pz_QmiEventReport->gpsSystemTime.system = eQMI_LOC_SV_SYSTEM_GPS_V02;
    pz_QmiEventReport->gpsSystemTime.systemClkTimeBias = pz_EventData->z_GpsSystemTime.f_ClkTimeBias;
    pz_QmiEventReport->gpsSystemTime.systemClkTimeUncMs = pz_EventData->z_GpsSystemTime.f_ClkTimeUncMs;
    pz_QmiEventReport->gpsSystemTime.systemMsec = pz_EventData->z_GpsSystemTime.q_GpsMsec;
    pz_QmiEventReport->gpsSystemTime.systemWeek = pz_EventData->z_GpsSystemTime.w_GpsWeek;
  }

  //TLV 0x11 - Klobuchar Iono Model
  if (pz_EventData->t_ValidityMask & GNSS_QMI_EVENT_KLOBUCHAR_MODEL_VALID)
  {
    pz_QmiEventReport->klobucharIonoModel_valid = TRUE;
    pz_QmiEventReport->klobucharIonoModel.alpha0 = pz_EventData->z_KlobucharModel.alpha0;
    pz_QmiEventReport->klobucharIonoModel.alpha1 = pz_EventData->z_KlobucharModel.alpha1;
    pz_QmiEventReport->klobucharIonoModel.alpha2 = pz_EventData->z_KlobucharModel.alpha2;
    pz_QmiEventReport->klobucharIonoModel.alpha3 = pz_EventData->z_KlobucharModel.alpha3;
    pz_QmiEventReport->klobucharIonoModel.beta0 = pz_EventData->z_KlobucharModel.beta0;
    pz_QmiEventReport->klobucharIonoModel.beta1 = pz_EventData->z_KlobucharModel.beta1;
    pz_QmiEventReport->klobucharIonoModel.beta2 = pz_EventData->z_KlobucharModel.beta2;
    pz_QmiEventReport->klobucharIonoModel.beta3 = pz_EventData->z_KlobucharModel.beta3;
    pz_QmiEventReport->klobucharIonoModel.dataSource = pz_EventData->z_KlobucharModel.u_GnssSys;
  }
  
  //TLV 0x12 - GLONASS Time Scale Correction (tauC)
  if (pz_EventData->t_ValidityMask & GNSS_QMI_EVENT_GLONASS_TAU_C_VALID)
  {
    pz_QmiEventReport->tauC_valid = TRUE;
    pz_QmiEventReport->tauC = pz_EventData->d_GloTauC;
  }

  //TLV 0x13 - Leap Seconds
  if (pz_EventData->t_ValidityMask & GNSS_QMI_EVENT_LEAP_SECONDS_VALID)
  {
    pz_QmiEventReport->leapSec_valid = TRUE;
    pz_QmiEventReport->leapSec = pz_EventData->b_LeapSeconds;
  }

  LOC_MSG_MED_1("loc_HandleEventReport: 0x%X", pz_QmiEventReport->eventReport);

  ind_info.p_Msg = (void *)pz_QmiEventReport;
  // send indication through QCSI
  if (FALSE == locQmiShimBroadcastInd(QMI_LOC_EVENT_MASK_GNSS_EVENT_REPORT_V02, &ind_info))
  {
    LOC_MSG_ERROR_0("loc_HandleEventReport: IND could not be sent");
  }

  os_MemFree((void**)&pz_QmiEventReport);
}
