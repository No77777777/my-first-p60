/******************************************************************************
  @file:  loc_pa.c
  @brief: Location Middleware PA module

  DESCRIPTION
This module configures/retrieves the NV setting of location
  engine on the modem, and processes EFS data.

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  -----------------------------------------------------------------------------
  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Copyright (c) 2013-2014 QUALCOMM Atheros, Inc.
  All Rights Reserved. 
  QUALCOMM Proprietary and Confidential.
  Copyright (c) 2015-2020 Qualcomm Technologies, Inc. 
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
06/18/20   sai      Added NAVIC delete clock info mask related changes
02/10/20   py       Navic SV Blacklisting Changes
01/17/20   jeev     Robust location implementation
02/19/19   skm      5G NR-NSA, LPP Rel-15 and SUPL 2.0.4 support 
02/14/19   sum      Issue with GAL sv data deletion
03/13/19   nath     Xtra integrity data deletion using QMI delete_all flag
02/14/19   sum      OOB check is missing for aiding data deletion
09/24/18   py       Added support to delete TGD data
10/25/18   nath     Added support for GAGNS
07/03/18   sj       Set flags to track calls to pdsm_set_parameters() before API is called.
09/14/17   sum      Added support for GPDTM
08/03/17   yh       locPa_ResetModule after failure case.
07/07/17   yh       Set v_PaCmdInProgress to TRUE for the success case only.
03/31/17   yh       Loc_pa clean up
02/17/17   skm      BLE and Barometer support for LPPE Phase 2 
11/15/16   yh       Remove big local varialbe in locPa_SendQmiIndicationProxy
06/08/16   yh       Lppe
05/25/16   mj       Add separate support for IONO BDS and GAL
05/19/15   yh       LocMW refactoring
04/25/16   yh       Fix SBAs not deleted bug
03/16/16   yh       XTRA PCID
10/06/15   yh       QZSS
08/28/15   ah       Added missing break when handling GET_SUPL_SECURITY
05/18/15   jv       Added deletion bits for SBAS eph & alm.
08/08/14   jv       Galileo Support added 
04/15/14   yh       XTRA version check added 
04/28/14   ah       Added handling of Set Premium Services indication status
03/14/14   ss       Handling EFS command response in loc_pa_event_cb_ext
03/10/14   ah       Added support for Set Premium Services Cfg
02/20/14   ah       Fixed issue with Get NMEA Type indication
01/24/14   ah       Added GLO support for Get/Set NMEA Type Requests in Loc MW
03/29/10   ns       Changed Loc MW to be an OEM type client
10/31/09   dx       EFS data injection
01/01/09   wc/etc   Initial version and development

  $Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/src/loc_pa.c#6 $
  $DateTime: 2023/12/13 06:41:36 $
  $Author: pwbldsvc $
======================================================================*/

#include "comdef.h"
#include "customer.h"
#include "target.h"

#include "msg.h"
#include "queue.h"

#include "aries_os_api.h"
#include "pdapi.h"
#include "pdsm_atl.h"
#include "loc_api_2.h"

#include "loc_pa.h"
#include "loc_api_internal.h"

#include "location_service_v02.h"
#include "loc_qmi_shim.h"
#include "loc_utils.h"
#include "sm_util.h"

#define LOC_PA_OPER_DEFAULT_TIMEOUT  (4000)

/* The Max Valid value of SUPL CertID that can be accepted
 * Valid Values of SUPL CertID is [0..9]
 */
#define LOC_PA_MAX_SUPL_CERT_ID      (9)

#define LOC_PA_ALLOC_PAINFO(pointer) \
   (pointer) = (pdsm_pa_info_type *)loc_calloc(sizeof(*(pointer))); \
   if(NULL == (pointer)) \
   { \
      LOC_MSG_ERROR("pz_PaInfo loc_call returns NULL",0,0,0); \
      return LOC_API_GENERAL_FAILURE; \
   }

#define LOC_PA_FREE_PAINFO(pz_PaInfo) \
   if(NULL != (pz_PaInfo)) \
   { \
      loc_free(pz_PaInfo); \
   }


/* All SUPL certificates will be injected in /SUPL/ folder, and will be named
 * Cert0, Cert1, .. Cert9. The common prefix for all Certificates is defined here
 *
 * Note: this prefix must be kept in sync with the definition in tm_common.h
 */
static char const * const locPa_CertDirPath   =  "/SUPL/";
static char const * const locPa_CertDirPrefix =  "/SUPL/Cert";

static char b_SulpCertFileName[PDSM_PD_FS_MAX_PATH_LEN_BYTES];

/* -----------------------------------------------------------------------*//**
@brief
   Mapping of masks from QMI-Loc Delete Masks to PDAPI Delete Masks. 
*//* ------------------------------------------------------------------------*/
typedef struct
{
  uint32        q_QmiMask;
  uint32        q_PdapiMask;
}locPa_MaskMapType;

static void locPa_EventCb
(
  void*                      p_UserData, /* user data pointer */
  pdsm_pa_event_type         q_PaEvent,  /* Indicate PA event */
  const pdsm_pa_info_s_type* pz_PaInfo    /* PARAM pz_PaInfo POINTER */
);

static void locPa_EventCbExt
(
  void*                      p_UserData,   /* user data pointer */
  pdsm_pa_e_type             e_ParamType,  /* Indicate param type (enum PDSM_PA_xxx) */
  const pdsm_pa_info_s_type* pz_PaInfo,     /* PARAM pz_PaInfo POINTER */
  pdsm_pa_cmd_err_e_type     e_PaCmdStatus /* Optional field to indicate success/failure status of PA operation */
);

static void locPa_CmdCb
(
  void*                  p_DataBlock,
  pdsm_pa_cmd_e_type     e_PaCmdType,
  pdsm_pa_cmd_err_e_type e_PaCmdStatus
);

void locPa_ResetModule(void);

static uint32 locPa_SetParam(loc_client_handle_type l_ClientHandle, pdsm_pa_e_type e_ParamType, pdsm_pa_info_type* pz_PaInfo);
static uint32 locPa_GetParam(loc_client_handle_type l_ClientHandle, pdsm_pa_e_type e_ParamType);
static uint32 locPa_SetEngineLock(loc_client_handle_type l_ClientHandle, const qmiLocSetEngineLockReqMsgT_v02* p_SetEngineLockReq);
static uint32 locPa_SetSbasConfig(loc_client_handle_type l_ClientHandle, uint8 u_EnableSbas);
static uint32 locPa_SetNmeaTypes (loc_client_handle_type l_ClientHandle, qmiLocNmeaSentenceMaskT_v02 q_NmeaTypes);
static uint32 locPa_SetServerAddr(loc_client_handle_type l_ClientHandle, const qmiLocSetServerReqMsgT_v02* p_SetServerReq);
static uint32 locPa_DeleteAssistData(loc_client_handle_type l_ClientHandle, const qmiLocDeleteAssistDataReqMsgT_v02* p_DelAssistData);
static uint32 locPa_SetPeConfig(loc_client_handle_type l_ClientHandle, const qmiLocSetPositionEngineConfigParametersReqMsgT_v02* p_SetPeCfg);
static uint32 locPa_SetLppConfig(loc_client_handle_type l_ClientHandle, qmiLocLppConfigMaskT_v02 q_LppConfigMask);
static uint32 locPa_SetSuplVersion(loc_client_handle_type l_ClientHandle, qmiLocSuplVersionEnumT_v02 e_SuplVersion);
static uint32 locPa_SetSuplSecurity(loc_client_handle_type l_ClientHandle, boolean b_SuplSecurity);
static uint32 locPa_SetSuplHashAlgo(loc_client_handle_type l_ClientHandle, qmiLocSuplHashAlgoEnumT_v02 e_SuplHashAlgo);
static uint32 locPa_SetPremiumSvcCfg(loc_client_handle_type l_ClientHandle, const qmiLocSetPremiumServicesCfgReqMsgT_v02* p_PremiumSvcCfg);
static uint32 locPa_SetSuplTlsVersion(loc_client_handle_type l_ClientHandle, qmiLocSuplTlsVersionEnumT_v02 e_SuplTlsVersion);
static uint32 locPa_SetAssistedGloConfig(loc_client_handle_type l_ClientHandle, qmiLocAssistedGlonassProtocolMaskT_v02 q_AssistedGloConfig);
static uint32 locPa_SetEmergencyProtocol(loc_client_handle_type l_ClientHandle, qmiLocEmergencyProtocolEnumT_v02 e_EmergencyProtocol);
static uint32 locPa_SetXtratSessionCtrl(loc_client_handle_type l_ClientHandle, uint8 v_Control);
static uint32 locPa_SetXtraVersionCheck(loc_client_handle_type l_ClientHandle, qmiLocXtraVersionCheckEnumT_v02 e_XtraVersion);
static uint32 locPa_SetWifiScanInjTimeout(loc_client_handle_type l_ClientHandle, uint8 u_WifiScanInjectTimeout);
static uint32 locPa_SetGnssConstellRptCfg(loc_client_handle_type l_ClientHandle, const qmiLocSetGNSSConstRepConfigReqMsgT_v02* p_GnssCfg);
static uint32 locPa_InjectSuplCert(loc_client_handle_type l_ClientHandle,uint8 u_SuplCertId,
                                 uint32 q_SuplCertDataLen,const uint8* pu_SuplCertData);
static uint32 locPa_DeleteEfsData(loc_client_handle_type l_ClientHandle, qmiLocDeleteSuplCertificateReqMsgT_v02* p_DeleteSuplCert);
static uint32 locPa_DeleteGnssServiceData(loc_client_handle_type l_ClientHandle, qmiLocDeleteGNSSServiceDataReqMsgT_v02* p_DeleteGnssServiceData);
static uint32 locPa_SetXtraPcid(loc_client_handle_type l_ClientHandle, const qmiLocInjectXtraPcidReqMsgT_v02* const pz_XtraPcid);
static uint32 locPa_SetLppeUpMode(loc_client_handle_type  l_ClientHandle, qmiLocLppeUpAuxTechMaskT_v02 t_LppeUpConfig);
static uint32 locPa_SetLppeCpMode(loc_client_handle_type  l_ClientHandle,qmiLocLppeCpAuxTechMaskT_v02 t_LppeCpConfig);
static uint32 locPa_SetBlacklistSv(loc_client_handle_type l_ClientHandle, const qmiLocSetBlacklistSvReqMsgT_v02* p_QmiLocMsgData);
static pdsm_constellation_mask locPa_ConvertQmiConstellationMask_ToPdsm(qmiLocConstellationMaskT_v02 t_QmiMask);
static uint32 locPa_SetConstellationControl(loc_client_handle_type l_ClientHandle, const qmiLocSetConstellationConfigReqMsgT_v02* p_QmiLocMsgData);
static uint32 locPa_SetEmergencyCbWindow(loc_client_handle_type  l_ClientHandle, uint32_t emergencyCallbackWindow);

static qmiLocStatusEnumT_v02 locPa_ConvertCmdError2Qmi(pdsm_pa_cmd_err_e_type pa_cmd_err);

static boolean locPa_SendQmiIndicationProxy
(
  loc_client_handle_type       l_ClientHandle,
  const pdsm_pa_info_s_type*   pz_ParamInfo,
  pdsm_pa_cmd_err_e_type       e_ParamStatus
);

/* Mapping of masks from QMI_LOC Assistance Data to PDAPI Assistance Data */
static const struct {
  qmiLocDeleteGnssDataMaskT_v02 t_QmiLocMask;
  uint64                        t_PdapiMask;
} z_QmiPdsmSatelliteDataMaskMap[] =
{
    { QMI_LOC_MASK_DELETE_GPS_SVDIR_V02,             PDSM_PA_DELETE_SVDIR                 },
    { QMI_LOC_MASK_DELETE_GPS_SVSTEER_V02,           PDSM_PA_DELETE_SVSTEER               },
    { QMI_LOC_MASK_DELETE_GPS_TIME_V02,              PDSM_PA_DELETE_TIME_GPS              },
    { QMI_LOC_MASK_DELETE_GPS_ALM_CORR_V02,          PDSM_PA_DELETE_ALM_CORR              },
    { QMI_LOC_MASK_DELETE_GLO_SVDIR_V02,             PDSM_PA_DELETE_SVDIR_GLO             },
    { QMI_LOC_MASK_DELETE_GLO_SVSTEER_V02,           PDSM_PA_DELETE_SVSTEER_GLO           },
    { QMI_LOC_MASK_DELETE_GLO_TIME_V02,              PDSM_PA_DELETE_TIME_GLO              },
    { QMI_LOC_MASK_DELETE_GLO_ALM_CORR_V02,          PDSM_PA_DELETE_ALM_CORR_GLO          },
    { QMI_LOC_MASK_DELETE_SBAS_SVDIR_V02,            PDSM_PA_DELETE_SVDIR_SBAS            },
    { QMI_LOC_MASK_DELETE_SBAS_SVSTEER_V02,          PDSM_PA_DELETE_SVSTEER_SBAS          },
    { QMI_LOC_MASK_DELETE_POSITION_V02,              PDSM_PA_DELETE_POS                   },
    { QMI_LOC_MASK_DELETE_TIME_V02,                  PDSM_PA_DELETE_TIME                  },
    { QMI_LOC_MASK_DELETE_IONO_V02,                  PDSM_PA_DELETE_IONO                  },
    { QMI_LOC_MASK_DELETE_UTC_V02,                   PDSM_PA_DELETE_UTC                   },
    { QMI_LOC_MASK_DELETE_HEALTH_V02,                PDSM_PA_DELETE_HEALTH                },
    { QMI_LOC_MASK_DELETE_SADATA_V02,                PDSM_PA_DELETE_SADATA                },
    { QMI_LOC_MASK_DELETE_RTI_V02,                   PDSM_PA_DELETE_RTI                   },
    { QMI_LOC_MASK_DELETE_SV_NO_EXIST_V02,           PDSM_PA_DELETE_SV_NO_EXIST           },
    { QMI_LOC_MASK_DELETE_FREQ_BIAS_EST_V02,         PDSM_PA_DELETE_FREQ_BIAS_EST         },
    { QMI_LOC_MASK_DELETE_BDS_SVDIR_V02,             PDSM_PA_DELETE_SVDIR_BDS             },
    { QMI_LOC_MASK_DELETE_BDS_SVSTEER_V02,           PDSM_PA_DELETE_SVSTEER_BDS           },
    { QMI_LOC_MASK_DELETE_BDS_TIME_V02,              PDSM_PA_DELETE_TIME_BDS              },
    { QMI_LOC_MASK_DELETE_BDS_ALM_CORR_V02,          PDSM_PA_DELETE_ALM_CORR_BDS          },
    { QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GPS_V02, PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GPS },
    { QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GLO_V02, PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GLO },
    { QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_BDS_V02, PDSM_PA_DELETE_GNSS_SV_BLACKLIST_BDS },
    { QMI_LOC_MASK_DELETE_GNSS_SV_BLACKLIST_GAL_V02, PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GAL },
    { QMI_LOC_MASK_DELETE_GAL_SVDIR_V02,             PDSM_PA_DELETE_SVDIR_GAL             },
    { QMI_LOC_MASK_DELETE_GAL_SVSTEER_V02,           PDSM_PA_DELETE_SVSTEER_GAL           },
    { QMI_LOC_MASK_DELETE_GAL_TIME_V02,              PDSM_PA_DELETE_TIME_GAL              },
    { QMI_LOC_MASK_DELETE_GAL_ALM_CORR_V02,          PDSM_PA_DELETE_ALM_CORR_GAL          }
};

static const locPa_MaskMapType z_QmiPdsmClockInfoMaskMap[] =
{
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_TIME_EST_V02,          PDSM_PA_DELETE_CLOCK_INFO_TIME_EST },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_FREQ_EST_V02,          PDSM_PA_DELETE_CLOCK_INFO_FREQ_EST },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_WEEK_NUMBER_V02,       PDSM_PA_DELETE_CLOCK_INFO_WEEK_NUMBER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_RTC_TIME_V02,          PDSM_PA_DELETE_CLOCK_INFO_RTC_TIME },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_TIME_TRANSFER_V02,     PDSM_PA_DELETE_CLOCK_INFO_TIME_TRANSFER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GPSTIME_EST_V02,       PDSM_PA_DELETE_CLOCK_INFO_GPSTIME_EST },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GLOTIME_EST_V02,       PDSM_PA_DELETE_CLOCK_INFO_GLOTIME_EST },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GLODAY_NUMBER_V02,     PDSM_PA_DELETE_CLOCK_INFO_GLODAY_NUMBER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GLO4YEAR_NUMBER_V02,   PDSM_PA_DELETE_CLOCK_INFO_GLO4YEAR_NUMBER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GLO_RF_GRP_DELAY_V02,  PDSM_PA_DELETE_CLOCK_INFO_GLO_RF_GRP_DELAY },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_DISABLE_TT_V02,        PDSM_PA_DELETE_CLOCK_INFO_DISABLE_TT },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GG_LEAPSEC_V02,        PDSM_PA_DELETE_CLOCK_INFO_GG_LEAPSEC },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GG_GGTB_V02,           PDSM_PA_DELETE_CLOCK_INFO_GG_GGTB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_BDSTIME_EST_V02,       PDSM_PA_DELETE_CLOCK_INFO_BDSTIME_EST },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GB_GBTB_V02,           PDSM_PA_DELETE_CLOCK_INFO_GB_GBTB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_BG_BGTB_V02,           PDSM_PA_DELETE_CLOCK_INFO_BG_BGTB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_BDSWEEK_NUMBER_V02,    PDSM_PA_DELETE_CLOCK_INFO_BDSWEEK_NUMBER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_BDS_RF_GRP_DELAY_V02,  PDSM_PA_DELETE_CLOCK_INFO_BDS_RF_GRP_DELAY },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTIME_EST_V02,       PDSM_PA_DELETE_CLOCK_INFO_GALTIME_EST }, 
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOGPS_TB_V02,       PDSM_PA_DELETE_CLOCK_INFO_GPS_GAL_TB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOGLO_TB_V02,       PDSM_PA_DELETE_CLOCK_INFO_GLO_GAL_TB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GALTOBDS_TB_V02,       PDSM_PA_DELETE_CLOCK_INFO_GAL_BDS_TB },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GALWEEK_NUMBER_V02,    PDSM_PA_DELETE_CLOCK_INFO_GALWEEK_NUMBER },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_GAL_RF_GRP_DELAY_V02,  PDSM_PA_DELETE_CLOCK_INFO_GAL_RF_GRP_DELAY },
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TIME_EST_V02,    PDSM_PA_DELETE_CLOCK_INFO_NAVICTIME_EST},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_WEEK_NUMBER_V02, PDSM_PA_DELETE_CLOCK_INFO_NAVICWEEK_NUMBER},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_RF_GRP_DELAY_V02,PDSM_PA_DELETE_CLOCK_INFO_NAVIC_RF_GRP_DELAY},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GPS_TB_V02,   PDSM_PA_DELETE_CLOCK_INFO_GPS_NAVIC_TB},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GLO_TB_V02,   PDSM_PA_DELETE_CLOCK_INFO_GLO_NAVIC_TB},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_BDS_TB_V02,   PDSM_PA_DELETE_CLOCK_INFO_BDS_NAVIC_TB},
    { QMI_LOC_MASK_DELETE_CLOCK_INFO_NAVIC_TO_GAL_TB_V02,   PDSM_PA_DELETE_CLOCK_INFO_GAL_NAVIC_TB},
};

static const locPa_MaskMapType z_QmiPdsmCellDBMaskMap[] =
{
    { QMI_LOC_MASK_DELETE_CELLDB_POS_V02,                PDSM_PA_DELETE_CELLDB_MASK_POS_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_LATEST_GPS_POS_V02,     PDSM_PA_DELETE_CELLDB_MASK_LATEST_GPS_POS_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_OTA_POS_V02,            PDSM_PA_DELETE_CELLDB_MASK_OTA_POS_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_EXT_REF_POS_V02,        PDSM_PA_DELETE_CELLDB_MASK_EXT_REF_POS_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_TIMETAG_V02,            PDSM_PA_DELETE_CELLDB_MASK_TIMETAG_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_CELLID_V02,             PDSM_PA_DELETE_CELLDB_MASK_CELLID_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_CACHED_CELLID_V02,      PDSM_PA_DELETE_CELLDB_MASK_CACHED_CELLID_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_LAST_SRV_CELL_V02,      PDSM_PA_DELETE_CELLDB_MASK_LAST_SRV_CELL_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_CUR_SRV_CELL_V02,       PDSM_PA_DELETE_CELLDB_MASK_CUR_SRV_CELL_INFO },
    { QMI_LOC_MASK_DELETE_CELLDB_NEIGHBOR_INFO_V02,      PDSM_PA_DELETE_CELLDB_MASK_NEIGHBOR_INFO_INFO },
};

static const locPa_MaskMapType z_QmiPdsmCommonMaskMap[] =
{
    { QMI_LOC_DELETE_COMMON_MASK_POS_V02,             PDSM_PA_DELETE_POS },
    { QMI_LOC_DELETE_COMMON_MASK_TIME_V02,            PDSM_PA_DELETE_TIME },
    { QMI_LOC_DELETE_COMMON_MASK_UTC_V02,             PDSM_PA_DELETE_UTC },
    { QMI_LOC_DELETE_COMMON_MASK_RTI_V02,             PDSM_PA_DELETE_RTI },
    { QMI_LOC_DELETE_COMMON_MASK_FREQ_BIAS_EST_V02,   PDSM_PA_DELETE_FREQ_BIAS_EST },
};

// A general helper function to translate QMI mask to PDSM mask
/* -----------------------------------------------------------------------*//**
@brief
  A helper utility to translate QMI mask to PDSM mask
  
@param[in]   pz_QmiPdsmMaskMap    Predefined QMI and PDSM mask map
             q_Mask               QMI mask
             u_MaskMapSize        Mask map size

@retval      PSDM mask
*//* ------------------------------------------------------------------------*/
static uint32 locPa_XlateQmiPdsmMask(
   const locPa_MaskMapType *pz_QmiPdsmMaskMap,
   uint32                  q_Mask,
   uint8                   u_MaskMapSize)
{
   int    l_Cntr;
   uint32 q_PdapiMask = 0;

   if(NULL == pz_QmiPdsmMaskMap)
   {
      LOC_MSG_ERROR("locPa_XlateQmiPdsmMask, NULL pointer",0,0,0);
      return q_PdapiMask;
   }

   for (  l_Cntr = 0; l_Cntr < u_MaskMapSize; l_Cntr++ )
   {
      if(0 != (pz_QmiPdsmMaskMap[l_Cntr].q_QmiMask & q_Mask))
      {
         q_PdapiMask |= pz_QmiPdsmMaskMap[l_Cntr].q_PdapiMask;
      }
   }

   return q_PdapiMask;
}

/*---------------------------------------------------------------------------
@brief
  Function to initialize location PA module

@retval    TRUE   Called successfully
@retval    FALSE  Called unsuccessfully
---------------------------------------------------------------------------*/
boolean locPa_Init()
{
  locPa_ModuleDataType*      pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
  pdsm_client_status_e_type  pdsm_ret_val;
  boolean                    ret_val;

  pz_LocPaData->e_PaMoMethod = MO_UP;
  do
  {
    pz_LocPaData->pz_ProcessTimer = os_TimerCreate(LOC_MIDDLEWARE_TIMER_ID_PA, THREAD_ID_LOC_MIDDLEWARE);

    if (loc_middleware_data.pdapi_client_id == -1)
    {
      loc_middleware_data.pdapi_client_id = pdsm_client_init(PDSM_CLIENT_TYPE_PDA);
    }

    if (loc_middleware_data.pdapi_client_id == -1)
    {
      LOC_MSG_ERROR ( "pdsm_client_init failed", 0, 0, 0);
      ret_val = FALSE;
      break;
    }

    pdsm_ret_val = pdsm_client_pa_reg(loc_middleware_data.pdapi_client_id,
                                      NULL,
                                      locPa_EventCb,
                                      PDSM_CLIENT_EVENT_REG,
                                      PDSM_PA_EVENT_MASK,
                                      NULL);
    if (pdsm_ret_val != PDSM_CLIENT_OK)
    {
      ret_val = FALSE;
      break;
    }

    /* register with the pdsm pa extension */
    pdsm_ret_val = pdsm_client_pa_reg_ex(loc_middleware_data.pdapi_client_id,
                                         NULL,
                                         locPa_EventCbExt,
                                         PDSM_CLIENT_EVENT_REG,
                                         NULL);
    if (pdsm_ret_val != PDSM_CLIENT_OK)
    {
      ret_val = FALSE;
      break;
    }

    pdsm_ret_val = pdsm_client_act(loc_middleware_data.pdapi_client_id);
    if (pdsm_ret_val != PDSM_CLIENT_OK)
    {
      ret_val = FALSE;
      break;
    }
    else
    {
      ret_val = TRUE;
    }

    // Issue a command to get mo method
//    locPa_GetParam(LOC_CLIENT_HANDLE_INVALID, PDSM_PA_MO_METHOD); /* AHSIAU: Is this still required */

   } while (0);

   return ret_val;
}/* locPa_Init */

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
   const void*                  p_QmiLocMsgData,
   uint32                 q_QmiLocMsgLen,
   qmi_error_type_v01     *pe_QmiErr
)
{
   locPa_ModuleDataType*  pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
   uint8*                 pu_DestBuffer = NULL;
   uint32                 q_RetVal = LOC_API_GENERAL_FAILURE;
   boolean                v_RetVal = FALSE;

   if (NULL == pe_QmiErr)
   {
      LOC_MSG_ERROR("locPa_ProcessQmiRequest: NULL pe_QmiErr", 0,0,0);
      return v_RetVal;
   }

   /* Function arguments sanity check */
   if (LOC_CLIENT_HANDLE_INVALID == l_ClientHandle)
   {
      LOC_MSG_ERROR("locPa_ProcessQmiRequest: invalid clientHandle", 0,0,0);
      *pe_QmiErr = QMI_ERR_INTERNAL_V01;
      return v_RetVal;
   }

   /* Check for master client and cmd restriction */
   if (locClnt_IsCommandAllowed(l_ClientHandle, q_QmiLocMsgId) == FALSE)
   {
      LOC_MSG_HIGH("locPa_ProcessQmiRequest: client %d msg %lu not allowed", l_ClientHandle, q_QmiLocMsgId, 0);
      *pe_QmiErr = QMI_ERR_NO_PERMISSION_V01;
      return v_RetVal;
   }

   LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);

   /* Allow only one PA cmd to be processed at any time */
   if (pz_LocPaData->v_PaCmdInProgress)
   {
      LOC_MSG_ERROR("locPa_ProcessQmiRequest rejected, ParamType=%d still in progress", pz_LocPaData->e_ParamType, 0, 0);

      LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
      *pe_QmiErr = QMI_ERR_SESSION_OWNERSHIP_V01;
      return v_RetVal;
   }
   LOC_MSG_MED("locPa_ProcessQmiRequest start timer, set InProgress for QMI_LOC 0x%04x", q_QmiLocMsgId,0,0);

   /* We need to set these flags here because we can get the cmd/event callbacks 
    * before our call to pdsm_set_parameters() is over!! */
   pz_LocPaData->v_PaCmdInProgress = TRUE;
   pz_LocPaData->q_PaCmdCounterIn++;
   pz_LocPaData->l_ClientHandle    = l_ClientHandle;
   pz_LocPaData->q_QmiLocMsgId     = q_QmiLocMsgId;
   
   LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

   switch (q_QmiLocMsgId)
   {
      case QMI_LOC_SET_ENGINE_LOCK_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetEngineLock(l_ClientHandle, (qmiLocSetEngineLockReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_ENGINE_LOCK_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_ENGINE_LOCK_REQ_V02:
      {
         q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_GPS_LOCK);
         break;
      }

      case QMI_LOC_SET_SBAS_CONFIG_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetSbasConfig(l_ClientHandle, ((qmiLocSetSbasConfigReqMsgT_v02*)p_QmiLocMsgData)->sbasConfig);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_SBAS_CONFIG_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_SBAS_CONFIG_REQ_V02:
      {
         q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_SBAS_USER_PREFERENCE);
         break;
      }

      case QMI_LOC_SET_NMEA_TYPES_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetNmeaTypes(l_ClientHandle, ((qmiLocSetNmeaTypesReqMsgT_v02*)p_QmiLocMsgData)->nmeaSentenceType);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_NMEA_TYPES_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_NMEA_TYPES_REQ_V02:
      {
         q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_NMEA_SENTENCE_TYPE);
         break;
      }

      case QMI_LOC_SET_SERVER_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetServerAddr(l_ClientHandle, (qmiLocSetServerReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_SERVER_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_SERVER_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            switch (((qmiLocGetServerReqMsgT_v02*)p_QmiLocMsgData)->serverType)
            {
               case eQMI_LOC_SERVER_TYPE_CDMA_PDE_V02:
                  q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_1X_PDE_NETWORK_ADRS);
                  break;

               case eQMI_LOC_SERVER_TYPE_CDMA_MPC_V02:
                  q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_1X_MPC_NETWORK_ADRS);
                  break;

               case eQMI_LOC_SERVER_TYPE_UMTS_SLP_V02:
                  q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_UMTS_PDE_NETWORK_ADRS);
                  break;

               default:
                  LOC_MSG_ERROR("locPa_ProcessQmiRequest: Server Type not found", 0, 0, 0);
                  break;
            }
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_GET_SERVER_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_DELETE_ASSIST_DATA_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_DeleteAssistData(l_ClientHandle,
                                          (qmiLocDeleteAssistDataReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_DELETE_ASSIST_DATA_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_SET_XTRA_T_SESSION_CONTROL_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetXtratSessionCtrl(l_ClientHandle, ((qmiLocSetXtraTSessionControlReqMsgT_v02*)p_QmiLocMsgData)->xtraTSessionControl);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_XTRA_T_SESSION_CONTROL_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_XTRA_T_SESSION_CONTROL_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_XTRAT_USER_SESSION_CONTROL);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_GET_XTRA_T_SESSION_CONTROL_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_SET_PROTOCOL_CONFIG_PARAMETERS_REQ_V02:
      {
         uint8 u_CntTlv = 0;
         qmiLocSetProtocolConfigParametersReqMsgT_v02* p_QmiLocSetProtReq = 
         (qmiLocSetProtocolConfigParametersReqMsgT_v02*) p_QmiLocMsgData;
         if (NULL != p_QmiLocSetProtReq)
         {
           u_CntTlv += ((p_QmiLocSetProtReq->vxVersion_valid)                  ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->suplSecurity_valid)               ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->suplVersion_valid)                ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->lppConfig_valid)                  ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->assistedGlonassProtocolMask_valid)?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->emergencyProtocol_valid)          ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->suplHashAlgo_valid)               ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->suplTlsVersion_valid)             ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->wifiScanInjectTimeout_valid)      ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->lppeCpConfig_valid)               ?1:0);
           u_CntTlv += ((p_QmiLocSetProtReq->lppeUpConfig_valid)               ?1:0);
          u_CntTlv += ((p_QmiLocSetProtReq->emergencyCallbackWindow_valid) ? 1 : 0);
         }
         q_RetVal = LOC_API_UNSUPPORTED;
         
         if (u_CntTlv > 1)
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest: Multiple cfgs in SetProtocolConfigParamMask not supported", 0, 0, 0);
         }
         else if (u_CntTlv == 0)
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest: Zero cfgs in SetProtocolConfigParamMask not supported", 0, 0, 0);
         }
         else if (NULL != p_QmiLocSetProtReq)
         {
            /* Only one TLV is specified */
            if (p_QmiLocSetProtReq->vxVersion_valid)
            {
               /* Special option, handled by NI module */
               if(TRUE == locNi_ProcessQmiRequest
                          (l_ClientHandle, q_QmiLocMsgId, p_QmiLocMsgData, q_QmiLocMsgLen ))
               {
                  q_RetVal = LOC_API_SUCCESS;
               }
               else
               {
                  q_RetVal = LOC_API_GENERAL_FAILURE;
               }
            }
            else if (p_QmiLocSetProtReq->suplSecurity_valid)
            {
               q_RetVal = locPa_SetSuplSecurity(l_ClientHandle, (p_QmiLocSetProtReq->suplSecurity ? TRUE : FALSE));
            }
            else if (p_QmiLocSetProtReq->suplVersion_valid)
            {
               q_RetVal = locPa_SetSuplVersion(l_ClientHandle, p_QmiLocSetProtReq->suplVersion);
            }
            else if (p_QmiLocSetProtReq->lppConfig_valid)
            {
               q_RetVal = locPa_SetLppConfig(l_ClientHandle, p_QmiLocSetProtReq->lppConfig);
            }
            else if (p_QmiLocSetProtReq->assistedGlonassProtocolMask_valid)
            {
               q_RetVal = locPa_SetAssistedGloConfig(l_ClientHandle, p_QmiLocSetProtReq->assistedGlonassProtocolMask);
            }
            else if (p_QmiLocSetProtReq->emergencyProtocol_valid)
            {
               q_RetVal = locPa_SetEmergencyProtocol(l_ClientHandle, p_QmiLocSetProtReq->emergencyProtocol);
            }
            else if (p_QmiLocSetProtReq->suplHashAlgo_valid)
            {
               q_RetVal = locPa_SetSuplHashAlgo(l_ClientHandle, p_QmiLocSetProtReq->suplHashAlgo);
            }
            else if (p_QmiLocSetProtReq->suplTlsVersion_valid)
            {
               q_RetVal = locPa_SetSuplTlsVersion(l_ClientHandle, p_QmiLocSetProtReq->suplTlsVersion);
            }
            else if (p_QmiLocSetProtReq->wifiScanInjectTimeout_valid)
            {
               q_RetVal = locPa_SetWifiScanInjTimeout(l_ClientHandle, p_QmiLocSetProtReq->wifiScanInjectTimeout);
            }
            else if ( p_QmiLocSetProtReq->lppeUpConfig_valid )
            {
               q_RetVal = locPa_SetLppeUpMode(l_ClientHandle, p_QmiLocSetProtReq->lppeUpConfig);
            }
            else if ( p_QmiLocSetProtReq->lppeCpConfig_valid )
            {
               q_RetVal = locPa_SetLppeCpMode(l_ClientHandle, p_QmiLocSetProtReq->lppeCpConfig);
            }
          else if (p_QmiLocSetProtReq->emergencyCallbackWindow_valid)
          {
            q_RetVal = locPa_SetEmergencyCbWindow(l_ClientHandle, p_QmiLocSetProtReq->emergencyCallbackWindow);
          }
            else
            {
               LOC_MSG_ERROR("locPa_ProcessQmiRequest: SetProtocolConfigParamMask not found", 0, 0, 0);
            }
         }
         break;
      }

      case QMI_LOC_GET_PROTOCOL_CONFIG_PARAMETERS_REQ_V02:
      {
         if (NULL == p_QmiLocMsgData)
         {
           LOC_MSG_ERROR("p_QmiLocMsgData is NULL", 0, 0, 0);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_VX_VERSION_V02)
         {
            /* Special option, handled by NI module */
            if(TRUE == locNi_ProcessQmiRequest 
                       (l_ClientHandle, q_QmiLocMsgId, p_QmiLocMsgData, q_QmiLocMsgLen ))
            {
               q_RetVal = LOC_API_SUCCESS;
            }
            else
            {
               q_RetVal = LOC_API_GENERAL_FAILURE;
            }
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_SECURITY_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_SUPL_SECURITY);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_VERSION_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_SUPL_VERSION);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPP_CONFIG_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_LPP_CONFIG_INFO);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_ASSISTED_GLONASS_PROTOCOL_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_ASST_GLONASS_POS_PROTOCOL);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_EMERGENCY_PROTOCOL_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_HASH_ALGO_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_SUPL_HASH_ALGORITHM);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_SUPL_TLS_VERSION_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_SUPL_TLS_VERSION);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_WIFI_SCAN_INJECT_TIMEOUT_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_WIFI_SCAN_TIMEOUT_SEC);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPPE_UP_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_LPPE_UP_CONFIG);
         }
         else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02*)p_QmiLocMsgData)->getProtocolConfigParamMask &
                                                               QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_LPPE_CP_V02)
         {
            q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_LPPE_CP_CONFIG);
         }
        else if (((qmiLocGetProtocolConfigParametersReqMsgT_v02 *)p_QmiLocMsgData)->getProtocolConfigParamMask &
                    QMI_LOC_PROTOCOL_CONFIG_PARAM_MASK_EMERGENCY_CB_WINDOW_V02)
        {
          q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_EMERGENCY_EXTENSION_WINDOW);
        }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest: GetProtocolConfigParamMask not found", 0, 0, 0);
         }
         break;
      }

      case QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetPeConfig(l_ClientHandle,
                                       (qmiLocSetPositionEngineConfigParametersReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_GET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ_V02:
      {
         q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_PE_CONFIG_PARAM);
         break;
      }

      case QMI_LOC_SET_PREMIUM_SERVICES_CONFIG_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetPremiumSvcCfg(l_ClientHandle,
                                          (qmiLocSetPremiumServicesCfgReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_GET_POSITION_ENGINE_CONFIG_PARAMETERS_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_SET_XTRA_VERSION_CHECK_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetXtraVersionCheck(l_ClientHandle,
                                             ((qmiLocSetXtraVersionCheckReqMsgT_v02*)p_QmiLocMsgData)->xtraVersionCheckMode);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_XTRA_VERSION_CHECK_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_SET_GNSS_CONSTELL_REPORT_CONFIG_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetGnssConstellRptCfg(l_ClientHandle,
                                                (qmiLocSetGNSSConstRepConfigReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_SET_GNSS_CONSTELL_REPORT_CONFIG NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            qmiLocInjectSuplCertificateReqMsgT_v02*   pz_SuplCertificateReqMsg = (qmiLocInjectSuplCertificateReqMsgT_v02*)p_QmiLocMsgData;

            if (QMI_LOC_MAX_SUPL_CERT_LENGTH_V02 < pz_SuplCertificateReqMsg->suplCertData_len)
            {
               LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ Certificate Length=%d greater than what the Interface accepts", 
                              pz_SuplCertificateReqMsg->suplCertData_len, 0, 0);
               q_RetVal = LOC_API_INVALID_PARAMETER;
            }
            else if ( LOC_PA_MAX_SUPL_CERT_ID < pz_SuplCertificateReqMsg->suplCertId )
            {
               LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ suplCertId=%d exceeds MAX", 
                              pz_SuplCertificateReqMsg->suplCertData_len, 0, 0);
               q_RetVal = LOC_API_INVALID_PARAMETER;
            }
            else
            {
               q_RetVal = locPa_InjectSuplCert(l_ClientHandle,
                                          pz_SuplCertificateReqMsg->suplCertId,
                                          pz_SuplCertificateReqMsg->suplCertData_len,
                                          pz_SuplCertificateReqMsg->suplCertData);
            }
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_INJECT_SUPL_CERTIFICATE_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_DELETE_SUPL_CERTIFICATE_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_DeleteEfsData(l_ClientHandle,
                                       (qmiLocDeleteSuplCertificateReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_DELETE_SUPL_CERTIFICATE_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_DELETE_GNSS_SERVICE_DATA_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_DeleteGnssServiceData(l_ClientHandle,
                                       (qmiLocDeleteGNSSServiceDataReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_DELETE_GNSS_SERVICE_DATA_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_INJECT_XTRA_PCID_REQ_V02:
      {
         if(NULL != p_QmiLocMsgData)
         {
            q_RetVal = locPa_SetXtraPcid(l_ClientHandle,
                                       (qmiLocInjectXtraPcidReqMsgT_v02*)p_QmiLocMsgData);
         }
         else
         {
            LOC_MSG_ERROR("locPa_ProcessQmiRequest QMI_LOC_INJECT_XTRA_PCID_REQ NULL p_QmiLocMsgData",0,0,0);
         }
         break;
      }

      case QMI_LOC_SET_BLACKLIST_SV_REQ_V02:
      {
        q_RetVal = locPa_SetBlacklistSv(l_ClientHandle, (qmiLocSetBlacklistSvReqMsgT_v02*)p_QmiLocMsgData);
        break;
      }

      case QMI_LOC_GET_BLACKLIST_SV_REQ_V02:
      {
        q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_PERSISTENT_SV_MASK);
        break;
      }

      case QMI_LOC_SET_CONSTELLATION_CONTROL_REQ_V02:
      {
        q_RetVal = locPa_SetConstellationControl(l_ClientHandle, (qmiLocSetConstellationConfigReqMsgT_v02*)p_QmiLocMsgData);
        break;
      }

      case QMI_LOC_GET_CONSTELLATION_CONTROL_REQ_V02:
      {
        q_RetVal = locPa_GetParam(l_ClientHandle, PDSM_PA_CONSTELLATION_CONTROL_CONFIG);
        break;
      }

      default:
      {
        LOC_MSG_ERROR("locPa_ProcessQmiRequest: unsupported QMI_LOC 0x00%x", q_QmiLocMsgId, 0, 0);
        q_RetVal = LOC_API_GENERAL_FAILURE;
        break;
      }
   }

   if( LOC_API_SUCCESS == q_RetVal)
   {
      os_TimerStart(pz_LocPaData->pz_ProcessTimer, LOC_PA_OPER_DEFAULT_TIMEOUT, 0);
     *pe_QmiErr = QMI_ERR_NONE_V01;
      v_RetVal = TRUE;
   }
   else
   {
      locPa_ResetModule();
      *pe_QmiErr = locMW_ConvertToQmiErr(q_RetVal);
      LOC_MSG_ERROR("locPa_ProcessQmiRequest: QMI_LOC 0x%04x returns error=%d", q_QmiLocMsgId, q_RetVal, 0);
   }

   return v_RetVal;
}/* locPa_ProcessQmiRequest */

static void locPa_EventCb
(
  void*                      p_UserData, /* user data pointer */
  pdsm_pa_event_type         q_PaEvent,  /* Indicate PA event */
  const pdsm_pa_info_s_type* pz_PaInfo    /* PARAM pz_PaInfo POINTER */
)
{
  locPa_ModuleDataType* pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
  boolean               v_PaCmdInProgress;
  pdsm_pa_e_type        e_ParamType;
  pdsm_pa_cmd_e_type    e_PaCmdType;

  if (NULL == pz_PaInfo)
  {
    LOC_MSG_ERROR("locPa_EventCb: pz_PaInfo NULL parameter", 0, 0, 0);
    return;
  }

  /* Extract read copies of LocPa module data */
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  v_PaCmdInProgress = pz_LocPaData->v_PaCmdInProgress;
  e_ParamType       = pz_LocPaData->e_ParamType;
  e_PaCmdType       = pz_LocPaData->e_PaCmdType;

  if (v_PaCmdInProgress)
  {
    LOC_MSG_HIGH("locPa_EventCb: PaCmdType=%d, ParamType=%d, PaEvent=0x%x", e_PaCmdType, e_ParamType, q_PaEvent);

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    locPa_SendQmiIndicationProxy((loc_client_handle_type)p_UserData, pz_PaInfo, PDSM_PA_CMD_ERR_NOERR);
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
    return;
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

  if (q_PaEvent == PDSM_PA_EVENT_MO_METHOD) /* AHSIAU: Is this still required */
  {
    pz_LocPaData->e_PaMoMethod = pz_PaInfo->pa_info.mo_method;
    LOC_MSG_MED("locPa_EventCb: MO method=%d", q_PaEvent, 0, 0);
  }

  /* Event received when nobody is waiting for the event */
  LOC_MSG_ERROR("locPa_EventCb: ParamType=%d expected ParamType=%d v_PaCmdInProgress=%u",
                 pz_PaInfo->pa_event, e_ParamType, v_PaCmdInProgress);
}/* locPa_EventCb */

static void locPa_EventCbExt
(
  void*                      p_UserData,   /* user data pointer */
  pdsm_pa_e_type             e_ParamType,  /* Indicate PDSM param type, PDSM_PA_xxx enum */
  const pdsm_pa_info_s_type* pz_PaInfo,     /* A pointer to a PA state information struct */
  pdsm_pa_cmd_err_e_type     e_PaCmdStatus /* Optional field to indicate success/failure status of PA operation */
)
{
  locPa_ModuleDataType* pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
  boolean               v_PaCmdInProgress;
  pdsm_pa_cmd_e_type    e_PaCmdType;

  if (NULL == pz_PaInfo)
  {
    LOC_MSG_ERROR("locPa_EventCbExt: pz_PaInfo NULL parameter", 0, 0, 0);
    return;
  }

  if ((e_ParamType != pz_PaInfo->pa_event) || (e_ParamType != pz_LocPaData->e_ParamType))
  {
    LOC_MSG_ERROR("locPa_EventCbExt: ParamType=%d pa_event=%d e_ParamType=%d mismatch",
                   e_ParamType, pz_PaInfo->pa_event, pz_LocPaData->e_ParamType);
  }

  /* Extract read copies of LocPa module data */
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  v_PaCmdInProgress = pz_LocPaData->v_PaCmdInProgress;
  e_PaCmdType       = pz_LocPaData->e_PaCmdType;

  if (v_PaCmdInProgress)
  {
    LOC_MSG_HIGH("locPa_EventCbExt: PaCmdType=%d, ParamType=%d, PaCmdStatus=%d",
                  e_PaCmdType, e_ParamType, e_PaCmdStatus);

    /* Send IPC message to LocMW task to handle QMI_LOC indication */
    locPa_SendQmiIndicationProxy((loc_client_handle_type)p_UserData, pz_PaInfo, e_PaCmdStatus);
    LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
    return;
  }

  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

  /* No PA command in progress, might be already returned by locPa_EventCb*/
  LOC_MSG_HIGH("locPa_EventCbExt: PaCmdType=%d, ParamType=%d, v_PaCmdInProgress=%u",
                 e_PaCmdType, pz_PaInfo->pa_event, v_PaCmdInProgress);
}/* locPa_EventCbExt */

static void locPa_CmdCb
(
  void*                  p_DataBlock,
  pdsm_pa_cmd_e_type     e_PaCmdType,
  pdsm_pa_cmd_err_e_type e_PaCmdStatus
)
{
  locPa_ModuleDataType* pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
  boolean               v_PaCmdInProgress = FALSE;
  pdsm_pa_e_type        e_ParamType = PDSM_PA_NONE;
  pdsm_pa_info_s_type*  pz_ParamInfo = NULL;
  loc_client_handle_type l_ClientHandle;

  /* Extract read copies of LocPa module data */
  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  v_PaCmdInProgress = pz_LocPaData->v_PaCmdInProgress;
  e_ParamType       = pz_LocPaData->e_ParamType;
  l_ClientHandle = pz_LocPaData->l_ClientHandle;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

  LOC_MSG_HIGH("locPa_CmdCb: PaCmdType=%d, ParamType=%d, PaCmdStatus=%d", e_PaCmdType, e_ParamType, e_PaCmdStatus);

  if (!v_PaCmdInProgress || (pz_LocPaData->e_PaCmdType != e_PaCmdType))
  {
    LOC_MSG_ERROR("locPa_CmdCb: v_PaCmdInProgress=%u or PaCmdType mismatch %d==%d",
                   v_PaCmdInProgress, e_PaCmdType, pz_LocPaData->e_PaCmdType);
  }
  else if (PDSM_PA_CMD_ERR_NOERR != e_PaCmdStatus)
  {
    pz_ParamInfo = (pdsm_pa_info_s_type*)loc_calloc(sizeof(*pz_ParamInfo));
    if(NULL == pz_ParamInfo)
    {
       LOC_MSG_ERROR("locPa_CmdCb: loc_calloc returned NULL",0,0,0);
    }
    else
    {
      pz_ParamInfo->client_id = l_ClientHandle;
      pz_ParamInfo->pa_event  = e_ParamType;
      LOC_MSG_ERROR("locPa_CmdCb: error, send indication", 0, 0, 0);
      locPa_SendQmiIndicationProxy(l_ClientHandle, pz_ParamInfo, e_PaCmdStatus);
      loc_free(pz_ParamInfo);
    }
  }

}/* locPa_CmdCb */

/*---------------------------------------------------------------------------
@brief
  Function to process Timer callback.

  When the timer fires, the location engine has not received the PA callback.
  Notify client of the failed status and reset the state.
---------------------------------------------------------------------------*/
void locPa_ProcessTimerCb(void)
{
  locPa_ModuleDataType* pz_LocPaData = &(loc_middleware_data.z_locPa_Data);
  pdsm_pa_info_s_type*  pz_ParamInfo = NULL;

  LOC_MSG_HIGH("locPa_ProcessTimerCb: fired", 0, 0, 0);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  if (pz_LocPaData->v_PaCmdInProgress)
  {
    pz_ParamInfo = (pdsm_pa_info_s_type*)loc_calloc(sizeof(*pz_ParamInfo));
    if(NULL != pz_ParamInfo)
    {
      /* Notify client about the timeout by sending IPC message to LocMW task to handle QMI_LOC indication */
      pz_ParamInfo->client_id = pz_LocPaData->l_ClientHandle;
      pz_ParamInfo->pa_event  = pz_LocPaData->e_ParamType;
      locPa_SendQmiIndicationProxy(pz_LocPaData->l_ClientHandle, pz_ParamInfo, PDSM_PA_CMD_ERR_OTHER);
      loc_free(pz_ParamInfo);
    }
    else
    {
       LOC_MSG_ERROR("locPa_ProcessTimerCb: loc_calloc returned NULL",0,0,0);
    }

    /* Reset the PA module */
    pz_LocPaData->v_PaCmdInProgress = FALSE;
    pz_LocPaData->q_PaCmdCounterOut++;
    pz_LocPaData->l_ClientHandle    = LOC_CLIENT_HANDLE_INVALID;
  }
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
}

/*---------------------------------------------------------------------------
@brief
  Function to reset the PA module. This is called when in progress pa cmd is
  done. This is called when PA cmd error fails, or PA event arrives.

  NOTE: This function is always called within LocMW critical section.
---------------------------------------------------------------------------*/
void locPa_ResetModule(void)
{
  locPa_ModuleDataType* pz_LocPaData = &(loc_middleware_data.z_locPa_Data);

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  /* Reset the PA module */
  pz_LocPaData->v_PaCmdInProgress = FALSE;
  pz_LocPaData->q_PaCmdCounterOut++;
  pz_LocPaData->l_ClientHandle    = LOC_CLIENT_HANDLE_INVALID;
  os_TimerStop(pz_LocPaData->pz_ProcessTimer);
  LOC_MSG_MED("locPa_ResetModule PaCmdType=%d, ParamType=%d, QMI_LOC 0x%04x",
     pz_LocPaData->e_PaCmdType, pz_LocPaData->e_ParamType, pz_LocPaData->q_QmiLocMsgId);
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);
}

/*---------------------------------------------------------------------------
@brief
  Function to process the PDSM Set Parameters LocMW request

@param[in] l_ClientHandle : Loc API client handle
@param[in] e_ParamType    : PDSM PA parameter to set

@retval    LOC_API_SUCCESS  Called successfully
@retval    <Anything Else>  Called unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetParam
(
  loc_client_handle_type l_ClientHandle,
  pdsm_pa_e_type         e_ParamType,
  pdsm_pa_info_type*     pz_PaInfo
)
{
  uint32 q_RetVal = LOC_API_GENERAL_FAILURE;

  /* Function arguments sanity check */
  if (NULL == pz_PaInfo)
  {
    LOC_MSG_ERROR("locPa_SetParam: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  loc_middleware_data.z_locPa_Data.e_ParamType = e_ParamType;
  loc_middleware_data.z_locPa_Data.e_PaCmdType = PDSM_PA_CMD_SET_PARAM;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

  if (pdsm_set_parameters(locPa_CmdCb,
                          (void*) l_ClientHandle,
                          e_ParamType,
                          pz_PaInfo,
                          loc_middleware_data.pdapi_client_id))
  {
    q_RetVal = LOC_API_SUCCESS;
  }
  else
  {
    q_RetVal = LOC_API_GENERAL_FAILURE;
  }

  LOC_MSG_LOW("locPa_SetParam: RetVal=%d, ParamType=%d", q_RetVal, e_ParamType, 0);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the PDSM Get Parameters LocMW request

@param[in] l_ClientHandle : Loc API client handle
@param[in] e_ParamType    : PDSM PA parameter to get

@retval    LOC_API_SUCCESS  Called successfully
@retval    <Anything Else>  Called unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_GetParam
(
  loc_client_handle_type l_ClientHandle,
  pdsm_pa_e_type         e_ParamType
)
{
  uint32 q_RetVal = LOC_API_GENERAL_FAILURE;

  LOC_MW_ENTER_CRIT_SECTION(&loc_middleware_data.crit_sec);
  loc_middleware_data.z_locPa_Data.e_ParamType = e_ParamType;
  loc_middleware_data.z_locPa_Data.e_PaCmdType = PDSM_PA_CMD_GET_PARAM;
  LOC_MW_LEAVE_CRIT_SECTION(&loc_middleware_data.crit_sec);

  if (pdsm_get_parameters(locPa_CmdCb,
                          (void*) l_ClientHandle,
                          e_ParamType,
                          loc_middleware_data.pdapi_client_id))
  {
    q_RetVal = LOC_API_SUCCESS;
  }
  else
  {
    q_RetVal = LOC_API_GENERAL_FAILURE;
  }

  LOC_MSG_LOW("locPa_GetParam: RetVal=%d, ParamType=%d", q_RetVal, e_ParamType, 0);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Engine Lock QMI_LOC request

@param[in] l_ClientHandle      : Loc API client handle
@param[in] p_SetEngineLockReq  : Parameters related to set engine lock

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetEngineLock
(
  loc_client_handle_type    l_ClientHandle,
   const qmiLocSetEngineLockReqMsgT_v02* p_SetEngineLockReq
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type* pz_PaInfo = NULL;
   qmiLocLockEnumT_v02 e_EngineLock;

   /* Function arguments sanity check */
   if (NULL == p_SetEngineLockReq)
   {
      LOC_MSG_ERROR("locPa_SetEngineLock: NULL parameters", 0, 0, 0);
      return LOC_API_INVALID_PARAMETER;
   }

   e_EngineLock = p_SetEngineLockReq->lockType;

   LOC_MSG_LOW("locPa_SetEngineLock: e_QmiLock=%d, SUB_VALID=%d, SUB_TYPE=%d",
               e_EngineLock, p_SetEngineLockReq->subType_valid, p_SetEngineLockReq->subType);
  LOC_MSG_LOW("locPa_SetEngineLock: lockClient_valid=%d, lockClient=%d",
              p_SetEngineLockReq->lockClient_valid, p_SetEngineLockReq->lockClient, 0);

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)

  /* Honor optional TLV locClient first, then the manadatory EngineLock TLV*/
  if (p_SetEngineLockReq->lockClient_valid)
  {
    if (QMI_LOC_LOCK_CLIENT_MASK_AFW_V02 & p_SetEngineLockReq->lockClient)
      loc_middleware_data.v_AfwAllowAfwPosReq = FALSE;
    if (QMI_LOC_LOCK_CLIENT_MASK_NFW_V02 & p_SetEngineLockReq->lockClient)
      loc_middleware_data.v_AfwAllowNfwPosReq = FALSE;
    if (QMI_LOC_LOCK_CLIENT_MASK_PRIVILEGED_V02 & p_SetEngineLockReq->lockClient)
      loc_middleware_data.v_AfwAllowPriPosReq = FALSE;

    if (0 == p_SetEngineLockReq->lockClient)
    {
      pz_PaInfo->gps_lock = PDSM_GPS_LOCK_NONE;
    }
    else if (QMI_LOC_LOCK_CLIENT_MASK_AFW_V02 & p_SetEngineLockReq->lockClient)
    {
      pz_PaInfo->gps_lock = PDSM_GPS_LOCK_MI;
      if ((QMI_LOC_LOCK_CLIENT_MASK_NFW_V02 & p_SetEngineLockReq->lockClient) &&
          (QMI_LOC_LOCK_CLIENT_MASK_PRIVILEGED_V02 & p_SetEngineLockReq->lockClient))
      {
        pz_PaInfo->gps_lock = PDSM_GPS_LOCK_ALL;
      }
    }
    else if (QMI_LOC_LOCK_CLIENT_MASK_NFW_V02 & p_SetEngineLockReq->lockClient)
    {
      pz_PaInfo->gps_lock = PDSM_GPS_LOCK_MT;
    }
    else
    {
      LOC_MSG_ERROR("locPa_SetEngineLock: Invalid lockClient=%d", p_SetEngineLockReq->lockClient, 0, 0);
      q_RetVal = LOC_API_INVALID_PARAMETER;
    }
  }
  else
  {
  switch (e_EngineLock)
  {
    case eQMI_LOC_LOCK_NONE_V02:
         pz_PaInfo->gps_lock = PDSM_GPS_LOCK_NONE;
        loc_middleware_data.v_AfwAllowAfwPosReq = TRUE;
        loc_middleware_data.v_AfwAllowNfwPosReq = TRUE;
      break;

    case eQMI_LOC_LOCK_MI_V02:
         pz_PaInfo->gps_lock = PDSM_GPS_LOCK_MI;
        loc_middleware_data.v_AfwAllowAfwPosReq = FALSE;
        loc_middleware_data.v_AfwAllowNfwPosReq = TRUE;
      break;

    case eQMI_LOC_LOCK_MT_V02:
         pz_PaInfo->gps_lock = PDSM_GPS_LOCK_MT;
        loc_middleware_data.v_AfwAllowAfwPosReq = TRUE;
        loc_middleware_data.v_AfwAllowNfwPosReq = FALSE;
      break;

    case eQMI_LOC_LOCK_ALL_V02:
         pz_PaInfo->gps_lock = PDSM_GPS_LOCK_ALL;
        loc_middleware_data.v_AfwAllowAfwPosReq = FALSE;
        loc_middleware_data.v_AfwAllowNfwPosReq = FALSE;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetEngineLock: Invalid Lock=%d", e_EngineLock, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }
  }


   // ---
   // AAR TODO: Once PDSM defines a new sub data structure, under pz_PaInfo to convey
   // the SUB info, need to do the following
   // - need to assign p_SetEngineLockReq->subType
   // - if p_SetEngineLockReq->subType_valid, otherwise set it to eQMI_LOC_LOCK_DD_SUB_V02
   // ---

   /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_GPS_LOCK, pz_PaInfo);
   }

  if (PDSM_GPS_LOCK_ALL != pz_PaInfo->gps_lock)
  {
    /* In case a lock is unblocked, restart the apptracking session */
    LOC_MSG_LOW("locPa_SetEngineLock: pd_schedule", 0, 0, 0);
    (void)locMW_SendLocCmdProxy(LOC_HANDLE_REGISTERED_CLIENTS,
                                LOC_CMD_TYPE_PD_SCHEDULE, 0, NULL);
  }
   LOC_PA_FREE_PAINFO(pz_PaInfo)
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set SBAS Config QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] b_EnableSbas   : Enable/Disable SBAS received from client

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetSbasConfig
(
  loc_client_handle_type l_ClientHandle,
  uint8                  u_EnableSbas
)
{
  uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type *pz_PaInfo = NULL;

  LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  pz_PaInfo->sbas_user_preference = u_EnableSbas;

  /* Set up global PA module structure with PDAPI specific info */
  q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_SBAS_USER_PREFERENCE, pz_PaInfo);
  LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_LOW("locPa_SetSbasConfig: RetVal=%d, u_EnableSbas=%d", q_RetVal, u_EnableSbas, 0);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set NMEA Types QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] q_NmeaTypes    : NMEA Sentence Type received from client

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetNmeaTypes
(
  loc_client_handle_type            l_ClientHandle,
  qmiLocNmeaSentenceMaskT_v02       q_NmeaTypes
)
{
  uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type *pz_PaInfo = NULL;

  LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  pz_PaInfo->nmea_sentence_type = 0;

  if (QMI_LOC_NMEA_MASK_GGA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GGA;
  }
  if (QMI_LOC_NMEA_MASK_RMC_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_RMC;
  }
  if (QMI_LOC_NMEA_MASK_GSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GSV;
  }
  if (QMI_LOC_NMEA_MASK_GSA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GSA;
  }
  if (QMI_LOC_NMEA_MASK_VTG_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_VTG;
  }
  if (QMI_LOC_NMEA_MASK_PQXFI_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_PQXFI;
  }
  if (QMI_LOC_NMEA_MASK_PSTIS_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_PSTIS;
  }
  if (QMI_LOC_NMEA_MASK_GLGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GLGSV;
  }
  if (QMI_LOC_NMEA_MASK_GNGSA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNGSA;
  }
  if (QMI_LOC_NMEA_MASK_GNGNS_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNGNS;
  }
  if (QMI_LOC_NMEA_MASK_GARMC_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GARMC;
  }
  if (QMI_LOC_NMEA_MASK_GAGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GAGSV;
  }
  if (QMI_LOC_NMEA_MASK_GAGSA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GAGSA;
  }
  if (QMI_LOC_NMEA_MASK_GAVTG_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GAVTG;
  }
  if (QMI_LOC_NMEA_MASK_GAGGA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GAGGA;
  }
  if (QMI_LOC_NMEA_MASK_PQGSA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_PQGSA;
  }
  if (QMI_LOC_NMEA_MASK_PQGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_PQGSV;
  }
  if (QMI_LOC_NMEA_MASK_DEBUG_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_DEBUG;
  }
  if (QMI_LOC_NMEA_MASK_GPDTM_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GPDTM;
  }
  if (QMI_LOC_NMEA_MASK_GAGNS_V02 & q_NmeaTypes)
  {
    /* GAGNS is deprecated, so always force disable */
    pz_PaInfo->nmea_sentence_type &= ~PDAPI_NMEA_GAGNS;
  }
  if (QMI_LOC_NMEA_MASK_GBGGA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GBGGA;
  }
  if (QMI_LOC_NMEA_MASK_GBGSA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GBGSA;
  }
  if (QMI_LOC_NMEA_MASK_GBGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GBGSV;
  }
  if (QMI_LOC_NMEA_MASK_GBRMC_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GBRMC;
  }
  if (QMI_LOC_NMEA_MASK_GBVTG_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GBVTG;
  }
  if (QMI_LOC_NMEA_MASK_GQGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GQGSV;
  }
  if (QMI_LOC_NMEA_MASK_GIGSV_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GIGSV;
  }
  if (QMI_LOC_NMEA_MASK_GNGGA_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNGGA;
  }
  if (QMI_LOC_NMEA_MASK_GNRMC_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNRMC;
  }
  if (QMI_LOC_NMEA_MASK_GNDTM_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNDTM;
  }
  if (QMI_LOC_NMEA_MASK_GNVTG_V02 & q_NmeaTypes)
  {
    pz_PaInfo->nmea_sentence_type |= PDAPI_NMEA_GNVTG;
  }

  if( 0 == pz_PaInfo->nmea_sentence_type )
  {
    q_RetVal = LOC_API_INVALID_PARAMETER;
    LOC_MSG_ERROR("locPa_SetNmeaTypes: RetVal=%d, qmiLocNmeaSentenceMaskT=0x%x, pdsm_pa_nmea_type=0x%x",
                q_RetVal, q_NmeaTypes, pz_PaInfo->nmea_sentence_type);
  }
  else
  {
  /* Set up global PA module structure with PDAPI specific info */
     q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_NMEA_SENTENCE_TYPE, pz_PaInfo);
     LOC_MSG_MED("locPa_SetNmeaTypes: RetVal=%d, q_QmiNmeaMask=0x%x, w_PdapiNmeaMask=%d",
                q_RetVal, q_NmeaTypes, pz_PaInfo->nmea_sentence_type);
  }
  LOC_PA_FREE_PAINFO(pz_PaInfo)


  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Server Address QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] p_SetServerReq : Set Server Request received from client

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetServerAddr
(
  loc_client_handle_type            l_ClientHandle,
  const qmiLocSetServerReqMsgT_v02* p_SetServerReq
)
{
   uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_e_type              e_ParamType = PDSM_PA_NONE;
   pdsm_pa_info_type           *pz_PaInfo = NULL;
   pdsm_server_address_s_type* p_ServerAddr = NULL;

  /* Function arguments sanity check */
  if (NULL == p_SetServerReq)
  {
    LOC_MSG_ERROR("locPa_SetServerAddr: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   p_ServerAddr = &(pz_PaInfo->server_address);

  switch (p_SetServerReq->serverType)
  {
    case eQMI_LOC_SERVER_TYPE_CDMA_PDE_V02:
    {
      e_ParamType = PDSM_PA_1X_PDE_NETWORK_ADRS;
      break;
    }
    case eQMI_LOC_SERVER_TYPE_CDMA_MPC_V02:
    {
      e_ParamType = PDSM_PA_1X_MPC_NETWORK_ADRS;
      break;
    }
    case eQMI_LOC_SERVER_TYPE_UMTS_SLP_V02:
    {
      e_ParamType = PDSM_PA_UMTS_PDE_NETWORK_ADRS;
      break;
    }
    default:
    {
      LOC_MSG_ERROR("locPa_SetServerAddr: Invalid Server Type=%d",
                      p_SetServerReq->serverType, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
    }
  }

  /* Convert Server Address from QMI LOC to PDAPI */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locMW_ConvertAddrToPdapi(p_SetServerReq, p_ServerAddr);
   }

   if (LOC_API_SUCCESS != q_RetVal)
  {
    LOC_MSG_ERROR("locPa_SetServerAddr: URL nor IP address provided", 0, 0, 0);
   }
   else
   {
      /* Set up global PA module structure with PDAPI specific info */
      q_RetVal = locPa_SetParam(l_ClientHandle, e_ParamType, pz_PaInfo);
  }

   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_LOW("locPa_SetServerAddr: RetVal=%d, ServerType=%d",
               q_RetVal, e_ParamType, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Delete Assistance Data QMI_LOC request

@param[in] l_ClientHandle  : Loc API client handle
@param[in] p_DelAssistData : Delete Assistance Data

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_DeleteAssistData
(
  loc_client_handle_type                   l_ClientHandle,
  const qmiLocDeleteAssistDataReqMsgT_v02* p_DelAssistData
)
{
  uint32             q_RetVal = LOC_API_SUCCESS;
  pdsm_pa_info_type *pz_PaInfo = NULL;
  uint8             u_Cntr;
  uint8             u_MaxLen;

  /* Function arguments sanity check */
  if (NULL == p_DelAssistData)
  {
    LOC_MSG_ERROR("locPa_DeleteAssistData: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  if (LOC_MW_IS_VALID_BOOLEAN(p_DelAssistData->deleteAllFlag))
  {
    if (p_DelAssistData->deleteAllFlag)
    {
      pz_PaInfo->delete_params.pdsm_delete_parms_flags = PDSM_PA_DELETE_ALL; //PDSM_PA_DELETE_ALL;
      pz_PaInfo->delete_params.pdsm_delete_parms_flags_ext = PDSM_PA_DELETE_ALL_EXT; //PDSM_PA_DELETE_ALL_EXT;
      pz_PaInfo->delete_params.delete_celldb_mask      = CGPS_MAX_UINT32;
      pz_PaInfo->delete_params.delete_clock_info_mask  = PDSM_PA_DELETE_ALL_CLOCK_INFO;
    }
    else
    {
      if (p_DelAssistData->deleteSvInfoList_valid)
      {
        const qmiLocDeleteSvInfoStructT_v02* p_SvInfo;

        if (p_DelAssistData->deleteSvInfoList_len > QMI_LOC_DELETE_MAX_SV_INFO_LENGTH_V02)
        {
          LOC_MSG_ERROR("deleteSvInfoList_len is greater than the size of the array", 0, 0, 0);
          return LOC_API_INVALID_PARAMETER;
        }

        /* Loop through each global navigation satellite system (GNSS) and
            (a) Ensure validity
            (b) Glean Assistance information */
        for (u_Cntr = 0; u_Cntr < p_DelAssistData->deleteSvInfoList_len; u_Cntr++)
        {
          p_SvInfo = &(p_DelAssistData->deleteSvInfoList[u_Cntr]);

          if ((p_SvInfo->gnssSvId >= PDSM_PD_SV_ID_GPS_MIN) &&
              (p_SvInfo->gnssSvId <= PDSM_PD_SV_ID_GPS_MAX))
          {
            /* Validate that the Sv System is set correctly */
            if (eQMI_LOC_SV_SYSTEM_GPS_V02 != p_SvInfo->system)
            {
              LOC_MSG_ERROR("locPa_DeleteAssistData: GPS Sv System mismatch", 0, 0, 0);
              q_RetVal = LOC_API_GENERAL_FAILURE;
              break;
            }
            else
            {
              /* Glean GPS-specific information */
              if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH;
              }
              if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM;
              }
            }
          }
          else if ((p_SvInfo->gnssSvId >= PDSM_PD_SV_ID_SBAS_MIN) &&
                   (p_SvInfo->gnssSvId <= PDSM_PD_SV_ID_SBAS_MAX))
          {
            /* Validate that the Sv System is set correctly */
            if (eQMI_LOC_SV_SYSTEM_SBAS_V02 != p_SvInfo->system)
            {
              LOC_MSG_ERROR("locPa_DeleteAssistData: SBAS Sv System mismatch", 0, 0, 0);
              q_RetVal = LOC_API_GENERAL_FAILURE;
              break;
            }
            else
            {
              /* Glean SBAS-specific information */
              if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_SBAS;
              }
              if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_SBAS;
              }
            }
          }
          else if ((p_SvInfo->gnssSvId >= PDSM_PD_SV_ID_GLO_MIN) &&
                   (p_SvInfo->gnssSvId <= PDSM_PD_SV_ID_GLO_MAX))
          {
            /* Validate that the Sv System is set correctly */
            if (eQMI_LOC_SV_SYSTEM_GLONASS_V02 != p_SvInfo->system)
            {
              LOC_MSG_ERROR("locPa_DeleteAssistData: GLO Sv System mismatch", 0, 0, 0);
              q_RetVal = LOC_API_GENERAL_FAILURE;
              break;
            }
            else
            {
              /* Glean GLO-specific information */
              if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_GLO;
              }
              if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_GLO;
              }
            }
          }
          else if ((p_SvInfo->gnssSvId >= PDSM_PD_SV_ID_GAL_MIN) &&
                   (p_SvInfo->gnssSvId <= PDSM_PD_SV_ID_GAL_MAX))
          {
            /* Validate that the Sv System is set correctly */
            if (eQMI_LOC_SV_SYSTEM_GALILEO_V02 != p_SvInfo->system)
            {
              LOC_MSG_ERROR("locPa_DeleteAssistData: GAL Sv System mismatch", 0, 0, 0);
              q_RetVal = LOC_API_GENERAL_FAILURE;
              break;
            }
            else
            {
              /* Glean GAL-specific information */
              if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_GAL;
              }
              if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_SvInfo->deleteSvInfoMask)
              {
                pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_GAL;
              }
            }
          }
          else
          {
            LOC_MSG_ERROR("locPa_DeleteAssistData: Invalid Sv System parameter", 0, 0, 0);
            q_RetVal = LOC_API_GENERAL_FAILURE;
            break;
          }
        }
      }

      /* Process BDS specific delete SV info */
      if (p_DelAssistData->deleteBdsSvInfoList_valid)
      {
        const qmiLocDeleteBDSSvInfoStructT_v02* p_BdsSvInfo;

        if (p_DelAssistData->deleteBdsSvInfoList_len > QMI_LOC_DELETE_MAX_BDS_SV_INFO_LENGTH_V02)
        {
          LOC_MSG_ERROR("deleteBdsSvInfoList_len is greater than the size of the array", 0, 0, 0);
          return LOC_API_INVALID_PARAMETER;
        }

        for (u_Cntr = 0; u_Cntr < p_DelAssistData->deleteBdsSvInfoList_len; u_Cntr++)
        {
          p_BdsSvInfo = &(p_DelAssistData->deleteBdsSvInfoList[u_Cntr]);

          /* Check the range of BDS satellites */
          if ((p_BdsSvInfo->gnssSvId < PDSM_PD_SV_ID_BDS_MIN) || 
              (p_BdsSvInfo->gnssSvId > PDSM_PD_SV_ID_BDS_MAX))
          {
            q_RetVal = LOC_API_GENERAL_FAILURE;
            break;
          }
          /* Glean BDS-specific information */
          if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_BdsSvInfo->deleteSvInfoMask)
          {
            pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_BDS;
          }
          if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_BdsSvInfo->deleteSvInfoMask)
          {
            pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_BDS;
          }
        }
      }

      /* Process GAL specific delete SV info */
      if (p_DelAssistData->deleteGalSvInfoList_valid)
      {
        const qmiLocDeleteGALSvInfoStructT_v02* p_GalSvInfo;

        if (p_DelAssistData->deleteGalSvInfoList_len > QMI_LOC_DELETE_MAX_GAL_SV_INFO_LENGTH_V02)
        {
          LOC_MSG_ERROR("deleteGalSvInfoList_len is greater than the size of the array", 0, 0, 0);
          return LOC_API_INVALID_PARAMETER;
        }

        for (u_Cntr = 0; u_Cntr < p_DelAssistData->deleteGalSvInfoList_len; u_Cntr++)
        {
          p_GalSvInfo = &(p_DelAssistData->deleteGalSvInfoList[u_Cntr]);

          /* Check the range of GAL satellites */
          if ((p_GalSvInfo->gnssSvId < PDSM_PD_SV_ID_GAL_MIN) ||
              (p_GalSvInfo->gnssSvId > PDSM_PD_SV_ID_GAL_MAX))
          {
            q_RetVal = LOC_API_GENERAL_FAILURE;
            break;
          }
          /* Glean GAL-specific information */
          if (QMI_LOC_MASK_DELETE_EPHEMERIS_V02 & p_GalSvInfo->deleteSvInfoMask)
          {
            pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_GAL;
          }
          if (QMI_LOC_MASK_DELETE_ALMANAC_V02 & p_GalSvInfo->deleteSvInfoMask)
          {
            pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_GAL;
          }
        }
      }

      /* Process GNSS Delete Assistance Data Mask */
      if (p_DelAssistData->deleteGnssDataMask_valid)
      {
        u_MaxLen = (sizeof(z_QmiPdsmSatelliteDataMaskMap)) / (sizeof(z_QmiPdsmSatelliteDataMaskMap[0]));
        for (u_Cntr = 0; u_Cntr < u_MaxLen; u_Cntr++)
        {
           if (z_QmiPdsmSatelliteDataMaskMap[u_Cntr].t_QmiLocMask & p_DelAssistData->deleteGnssDataMask)
          {
             pz_PaInfo->delete_params.pdsm_delete_parms_flags |= z_QmiPdsmSatelliteDataMaskMap[u_Cntr].t_PdapiMask;
          }
        }
      }

      /* Process GNSS Cell DB Delete Assistance Data Mask */
      if (p_DelAssistData->deleteCellDbDataMask_valid)
      {
        pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_CELLDB_INFO;

        u_MaxLen = (sizeof(z_QmiPdsmCellDBMaskMap)) / (sizeof(z_QmiPdsmCellDBMaskMap[0]));
        for (u_Cntr = 0; u_Cntr < u_MaxLen; u_Cntr++)
        {
           if (z_QmiPdsmCellDBMaskMap[u_Cntr].q_QmiMask & p_DelAssistData->deleteCellDbDataMask)
          {
            pz_PaInfo->delete_params.delete_celldb_mask |= z_QmiPdsmCellDBMaskMap[u_Cntr].q_PdapiMask;
          }
        }
      }

      /* Process GNSS Clock Info Delete Assistance Data Mask */
      if (p_DelAssistData->deleteClockInfoMask_valid)
      {
        pz_PaInfo->delete_params.pdsm_delete_parms_flags |= PDSM_PA_DELETE_CLOCK_INFO;

        u_MaxLen = (sizeof(z_QmiPdsmClockInfoMaskMap)) / (sizeof(z_QmiPdsmClockInfoMaskMap[0]));
        for (u_Cntr = 0; u_Cntr < u_MaxLen; u_Cntr++)
        {
           if (z_QmiPdsmClockInfoMaskMap[u_Cntr].q_QmiMask & p_DelAssistData->deleteClockInfoMask)
          {
            pz_PaInfo->delete_params.delete_clock_info_mask |= z_QmiPdsmClockInfoMaskMap[u_Cntr].q_PdapiMask;
          }
        }
      }
    }
  }
  else
  {
    LOC_MSG_ERROR("locPa_DeleteAssistData: Invalid boolean deleteAllFlag=%d", p_DelAssistData->deleteAllFlag, 0, 0);
    q_RetVal = LOC_API_GENERAL_FAILURE;
  }
 
  /* Set up global PA module structure with PDAPI specific info */
  if( LOC_API_SUCCESS == q_RetVal)
  {
    q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_DELETE_PARAMS, pz_PaInfo);
  }
  LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_LOW("locPa_DeleteAssistData: GnssData QmiLocMask=0x%llx, PdapiMask=0x%llx, RetVal=%d",
               p_DelAssistData->deleteGnssDataMask,
               pz_PaInfo->delete_params.pdsm_delete_parms_flags, q_RetVal);
  LOC_MSG_LOW("locPa_DeleteAssistData: CellDbMask = 0x%x, ClockInfoMask=0x%x",
               pz_PaInfo->delete_params.delete_celldb_mask,
               pz_PaInfo->delete_params.delete_clock_info_mask, 0);
  return q_RetVal;
}


/*---------------------------------------------------------------------------
@brief
  Function to process the Set Position Engine Config QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] p_SetPeCfg     : Position Engine Config received from client

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetPeConfig
(
  loc_client_handle_type                                    l_ClientHandle, 
  const qmiLocSetPositionEngineConfigParametersReqMsgT_v02* p_SetPeCfg
)
{
   uint32                     q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type         *pz_PaInfo = NULL;
   pdsm_pa_pe_config_s_type*  p_PeCfg  = NULL;

  /* Function arguments sanity check */
  if (NULL == p_SetPeCfg)
  {
    LOC_MSG_ERROR("locPa_SetPeConfig: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   p_PeCfg  = &(pz_PaInfo->pe_config_info);
  p_PeCfg->peConfigMask = 0;

  if (p_SetPeCfg->injectedPositionControl_valid &&
      LOC_MW_IS_VALID_BOOLEAN(p_SetPeCfg->injectedPositionControl))
  {
    p_PeCfg->peConfigMask       |= PDSM_PA_PE_CONFIG_PARAM_MASK_USE_INJECTED_POSITION;
    p_PeCfg->useInjectedPosition = p_SetPeCfg->injectedPositionControl;
  }
  else
  {
      q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  if (p_SetPeCfg->filterSvUsage_valid &&
      LOC_MW_IS_VALID_BOOLEAN(p_SetPeCfg->filterSvUsage))
  {
    p_PeCfg->peConfigMask |= PDSM_PA_PE_CONFIG_PARAM_MASK_FILTER_SV_USED;
    p_PeCfg->filterSvUsed  = p_SetPeCfg->filterSvUsage;
  }
  else
  {
      q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  if (p_SetPeCfg->storeAssistData_valid &&
      LOC_MW_IS_VALID_BOOLEAN(p_SetPeCfg->storeAssistData))
  {
    p_PeCfg->peConfigMask   |= PDSM_PA_PE_CONFIG_PARAM_MASK_STORE_ASSIST_DATA;
    p_PeCfg->storeAssistData = p_SetPeCfg->storeAssistData;
  }
  else
  {
      q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  /* If at least one of the parameters is invalid, reject the entire message */
   if (LOC_API_INVALID_PARAMETER != q_RetVal)
  {
    /* Set up global PA module structure with PDAPI specific info */
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_PE_CONFIG_PARAM, pz_PaInfo);
  }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_MED("locPa_SetPeConfig: RetVal=%d", q_RetVal, 0, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set XTRA-T Session Control QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] v_Control      : XTRA session control flag

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetXtratSessionCtrl
(
  loc_client_handle_type l_ClientHandle,
  uint8                  v_Control
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->session_control = v_Control;

  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_XTRAT_USER_SESSION_CONTROL, pz_PaInfo);

   LOC_MSG_MED("locPa_SetXtratSessionCtrl: RetVal=%d, SessionCtrl=%c",
            q_RetVal, pz_PaInfo->session_control, 0);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set SUPL Version QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] q_SuplVersion  : SUPL Version

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetSuplVersion
(
  loc_client_handle_type l_ClientHandle,
   qmiLocSuplVersionEnumT_v02  e_SuplVersion
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   switch(e_SuplVersion)
   {
   case eQMI_LOC_SUPL_VERSION_1_0_V02:
         pz_PaInfo->supl_version = PDSM_SUPL_VERSION_1_0;
      break;
   case eQMI_LOC_SUPL_VERSION_2_0_V02:
         pz_PaInfo->supl_version = PDSM_SUPL_VERSION_2_0;
      break;
   case eQMI_LOC_SUPL_VERSION_2_0_2_V02:
         pz_PaInfo->supl_version = PDSM_SUPL_VERSION_2_0_2;
      break;
   case eQMI_LOC_SUPL_VERSION_2_0_4_V02:
         pz_PaInfo->supl_version = PDSM_SUPL_VERSION_2_0_4 ;
      break;
   default:
      LOC_MSG_ERROR("locQmiShimHandleSetProtocolConfig: Invalid suplVersion %d",
                     e_SuplVersion,0,0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
      break;
   }

   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      /* Set up global PA module structure with PDAPI specific info */
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_SUPL_VERSION, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetSuplVersion: RetVal=%d, SuplVersion=%d",
                q_RetVal, pz_PaInfo->supl_version, 0);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set SUPL Security QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] b_SuplSecurity : SUPL Security (TRUE:SUPL, FALSE:UPL)

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetSuplSecurity
(
  loc_client_handle_type l_ClientHandle,
  boolean                b_SuplSecurity
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->supl_security = b_SuplSecurity;

  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_SUPL_SECURITY, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetSuplSecurity: RetVal=%d, SuplSecurity=%d",
               q_RetVal, pz_PaInfo->supl_security, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set LPP Configuration QMI_LOC request

@param[in] l_ClientHandle      : Loc API client handle
@param[in] q_AssistedGloConfig : LPP configuration

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetLppConfig
(
  loc_client_handle_type         l_ClientHandle,
  qmiLocLppConfigMaskT_v02       q_LppConfigMask
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->lpp_config_info = 0;

  if (q_LppConfigMask & QMI_LOC_LPP_CONFIG_ENABLE_USER_PLANE_V02)
  {
      pz_PaInfo->lpp_config_info |= PDSM_PA_LPP_IN_LTE_UP_ENABLE;
  }
  if (q_LppConfigMask & QMI_LOC_LPP_CONFIG_ENABLE_CONTROL_PLANE_V02)
  {
      pz_PaInfo->lpp_config_info |= PDSM_PA_LPP_IN_LTE_CP_ENABLE;
  }

  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_LPP_CONFIG_INFO, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)
   
  LOC_MSG_MED("locPa_SetLppConfig: RetVal=%d, q_QmiLppCfgMask=%d, q_PdapiLppCfgMask=%d",
               q_RetVal, q_LppConfigMask, pz_PaInfo->lpp_config_info);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Assisted Glonass Configuration QMI_LOC request

@param[in] l_ClientHandle      : Loc API client handle
@param[in] q_AssistedGloConfig : Assisted Glonass configuration mask

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetAssistedGloConfig
(
  loc_client_handle_type                       l_ClientHandle,
  qmiLocAssistedGlonassProtocolMaskT_v02       q_AssistedGloConfig
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->asst_glo_protocol = 0;

  if (q_AssistedGloConfig & QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_RRC_CP_V02)
  {
      pz_PaInfo->asst_glo_protocol |= PDSM_PA_ASST_GLO_PROTOCOL_RRC_CP;
  }
  if (q_AssistedGloConfig & QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_RRLP_UP_V02)
  {
      pz_PaInfo->asst_glo_protocol |= PDSM_PA_ASST_GLO_PROTOCOL_RRLP_UP;
  }
  if (q_AssistedGloConfig & QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_LPP_UP_V02)
  {
      pz_PaInfo->asst_glo_protocol |= PDSM_PA_ASST_GLO_PROTOCOL_LPP_UP;
  }
  if (q_AssistedGloConfig & QMI_LOC_ASSISTED_GLONASS_PROTOCOL_MASK_LPP_CP_V02)
  {
      pz_PaInfo->asst_glo_protocol |= PDSM_PA_ASST_GLO_PROTOCOL_LPP_CP;
  }

  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_ASST_GLONASS_POS_PROTOCOL, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetAssistedGloConfig: RetVal=%d, Assisted GLO config=%u",
               q_RetVal, pz_PaInfo->asst_glo_protocol, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Emergency Protocol QMI_LOC request

@param[in] l_ClientHandle      : Loc API client handle
@param[in] e_EmergencyProtocol : Emergency Protocol

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetEmergencyProtocol
(
  loc_client_handle_type                 l_ClientHandle,
  qmiLocEmergencyProtocolEnumT_v02       e_EmergencyProtocol
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  switch (e_EmergencyProtocol)
  {
    case eQMI_LOC_EMERGENCY_PROTOCOL_WCDMA_CP_V02:
         pz_PaInfo->gnss_emergency_protocol_select = PDSM_PA_EMERGENCY_PROTOCOL_CONFIG_WCMDA_CP;
      break;

    case eQMI_LOC_EMERGENCY_PROTOCOL_WCDMA_UP_V02:
         pz_PaInfo->gnss_emergency_protocol_select = PDSM_PA_EMERGENCY_PROTOCOL_CONFIG_WCMDA_UP;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetEmergencyProtocol: Invalid protocol=%d", e_EmergencyProtocol, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_GNSS_EMERGENCY_PROTOCOL_CONFIG, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetEmergencyProtocol: RetVal=%d, emergency protocol=%u",
               q_RetVal, pz_PaInfo->gnss_emergency_protocol_select, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set SUPL Hash Algorithm QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] e_SuplHashAlgo : SUPL Hash Algorithm

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetSuplHashAlgo
(
  loc_client_handle_type            l_ClientHandle,
   qmiLocSuplHashAlgoEnumT_v02       e_SuplHashAlgo
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  switch (e_SuplHashAlgo)
  {
    case eQMI_LOC_SUPL_HASH_ALGO_SHA1_V02:
         pz_PaInfo->supl_hash_algorithm = PDSM_PA_SUPL_HASH_ALGO_SHA1;
      break;

    case eQMI_LOC_SUPL_HASH_ALGO_SHA256_V02:
         pz_PaInfo->supl_hash_algorithm = PDSM_PA_SUPL_HASH_ALGO_SHA256;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetSuplHashAlgo: Invalid hash value=%d", e_SuplHashAlgo, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_SUPL_HASH_ALGORITHM, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetSuplHashAlgo: RetVal=%d, Hash Algo=%u",
               q_RetVal, pz_PaInfo->supl_hash_algorithm, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set SUPL TLS version QMI_LOC request

@param[in] l_ClientHandle   : Loc API client handle
@param[in] e_SuplTlsVersion : SUPL TLS version

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetSuplTlsVersion
(
  loc_client_handle_type              l_ClientHandle,
   qmiLocSuplTlsVersionEnumT_v02       e_SuplTlsVersion
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  switch (e_SuplTlsVersion)
  {
    case eQMI_LOC_SUPL_TLS_VERSION_1_0_V02:
      pz_PaInfo->supl_tls_version = PDSM_PA_SUPL_TLS_VERSION_1_0;
      break;

    case eQMI_LOC_SUPL_TLS_VERSION_1_1_V02:
      pz_PaInfo->supl_tls_version = PDSM_PA_SUPL_TLS_VERSION_1_1;
      break;

    case eQMI_LOC_SUPL_TLS_VERSION_1_2_V02:
      pz_PaInfo->supl_tls_version = PDSM_PA_SUPL_TLS_VERSION_1_2;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetSuplTlsVersion: Invalid TLS version=%d", e_SuplTlsVersion, 0, 0);
      q_RetVal = LOC_API_INVALID_PARAMETER;
  }
  
  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_SUPL_TLS_VERSION, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetSuplTlsVersion: RetVal=%d, TLS version=%u",
               q_RetVal, pz_PaInfo->supl_tls_version, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Wifi Scan Inject Timeout QMI_LOC request

@param[in] l_ClientHandle          : Loc API client handle
@param[in] u_WifiScanInjectTimeout : Wifi Scan Inject Timeout duration

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetWifiScanInjTimeout
(
  loc_client_handle_type l_ClientHandle,
  uint8                  u_WifiScanInjectTimeout
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->wifi_wait_timeout_sec = u_WifiScanInjectTimeout; /* uint8 time in seconds */
  
  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_WIFI_SCAN_TIMEOUT_SEC, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetWifiScanInjTimeout: RetVal=%d, WifiScanInjectTimeout=%u",
               q_RetVal, pz_PaInfo->wifi_wait_timeout_sec, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set Premium Services Configuration QMI_LOC request

@param[in] l_ClientHandle  : Loc API client handle
@param[in] p_PremiumSvcCfg : Premium Service Type & Configuration

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetPremiumSvcCfg
(
  loc_client_handle_type                        l_ClientHandle,
  const qmiLocSetPremiumServicesCfgReqMsgT_v02* p_PremiumSvcCfg
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

  /* Function arguments sanity check */
  if (NULL == p_PremiumSvcCfg)
  {
    LOC_MSG_ERROR("locPa_SetPremiumSvcCfg: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  switch (p_PremiumSvcCfg->premiumServiceType)
  {
    case eQMI_LOC_PREMIUM_SERVICE_GTP_CELL_V02:
    {
         pz_PaInfo->premium_svc_cfg.service = PDSM_PA_PREMIUM_SVC_GTP_CELL;
      break;
    }

    case eQMI_LOC_PREMIUM_SERVICE_SAP_V02:
    {
         pz_PaInfo->premium_svc_cfg.service = PDSM_PA_PREMIUM_SVC_SAP;
      break;
    }

    case eQMI_LOC_PREMIUM_SERVICE_GTP_ENH_CELL_V02:
    {
         pz_PaInfo->premium_svc_cfg.service = PDSM_PA_PREMIUM_SVC_GTP_ENH_CELL;
      break;
    }

    case eQMI_LOC_PREMIUM_SERVICE_GTP_WIFI_V02:
    {
         pz_PaInfo->premium_svc_cfg.service = PDSM_PA_PREMIUM_SVC_GTP_WIFI;
       break;
    }

    default:
      LOC_MSG_ERROR("locPa_SetPremiumSvcCfg: Invalid service type passed %d",
                     p_PremiumSvcCfg->premiumServiceType, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  switch (p_PremiumSvcCfg->premiumServiceCfg)
  {
    case eQMI_LOC_PREMIUM_SERVICE_DISABLED_V02:
         pz_PaInfo->premium_svc_cfg.config = PDSM_PA_PREMIUM_SVC_CFG_DISABLED;
      break;

    case eQMI_LOC_PREMIUM_SERVICE_ENABLED_BASIC_V02:
         pz_PaInfo->premium_svc_cfg.config = PDSM_PA_PREMIUM_SVC_CFG_ENABLED_BASIC;
      break;

    case eQMI_LOC_PREMIUM_SERVICE_ENABLED_PREMIUM_V02:
         pz_PaInfo->premium_svc_cfg.config = PDSM_PA_PREMIUM_SVC_CFG_ENABLED_PREMIUM;
      break;

  case eQMI_LOC_PREMIUM_SERVICE_ENABLED_THIRDPARTY_V02:
         pz_PaInfo->premium_svc_cfg.config = PDSM_PA_PREMIUM_SVC_CFG_ENABLED_THIRDPARTY;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetPremiumSvcCfg: Invalid service config type passed %d",
                     p_PremiumSvcCfg->premiumServiceCfg, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_PREMIUM_SERVICES_CONFIG, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetPremiumSvcCfg: RetVal=%d, Service=%d, Config=%d",
               q_RetVal, pz_PaInfo->premium_svc_cfg.service, pz_PaInfo->premium_svc_cfg.config);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set XTRA Version Check QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] e_XtraVersion  : XTRA Version Check Configuration

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetXtraVersionCheck
(
  loc_client_handle_type                l_ClientHandle,
  qmiLocXtraVersionCheckEnumT_v02       e_XtraVersion
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  switch (e_XtraVersion)
  {
    case eQMI_LOC_XTRA_VERSION_CHECK_DISABLE_V02:
         pz_PaInfo->xtra_version_check = PDSM_PA_XTRA_VERSION_CHECK_DISABLED;
      break;

    case eQMI_LOC_XTRA_VERSION_CHECK_AUTO_V02:
         pz_PaInfo->xtra_version_check = PDSM_PA_XTRA_VERSION_CHECK_AUTO;
      break;

    case eQMI_LOC_XTRA_VERSION_CHECK_XTRA2_V02:
         pz_PaInfo->xtra_version_check = PDSM_PA_XTRA_VERSION_CHECK_XTRA2;
      break;

    case eQMI_LOC_XTRA_VERSION_CHECK_XTRA3_V02:
         pz_PaInfo->xtra_version_check = PDSM_PA_XTRA_VERSION_CHECK_XTRA3;
      break;

    default:
      LOC_MSG_ERROR("locPa_SetXtraVersionCheck: Invalid XTRA version=%d",
                     e_XtraVersion, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
  }

  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
   {
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_XTRA_VERSION_CHECK, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetXtraVersionCheck: RetVal=%d, XTRA version=%d",
               q_RetVal, e_XtraVersion, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Set GNSS Constellation Report Config QMI_LOC request

@param[in] l_ClientHandle : Loc API client handle
@param[in] p_GnssCfg      : GNSS Constellation Report Configuration

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetGnssConstellRptCfg
(
  loc_client_handle_type                        l_ClientHandle,
  const qmiLocSetGNSSConstRepConfigReqMsgT_v02* p_GnssCfg
)
{
  uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type* pz_PaInfo = NULL;

  /* Function arguments sanity check */
  if (NULL == p_GnssCfg)
  {
    LOC_MSG_ERROR("locPa_SetGnssConstellRptCfg: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  /* Sanity check that at least one valid config was received */
  if (!p_GnssCfg->measReportConfig_valid && !p_GnssCfg->svPolyReportConfig_valid &&
      !p_GnssCfg->reportFullEphemerisDb_valid && !p_GnssCfg->reportFullSvPolyDb_valid)
  {
    LOC_MSG_ERROR("locPa_SetGnssConstellRptCfg: Invalid configurations received", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  if (p_GnssCfg->measReportConfig_valid)
  {
    pz_PaInfo->z_set_gnss_config.meas_report_config_valid = p_GnssCfg->measReportConfig_valid;
    pz_PaInfo->z_set_gnss_config.e_meas_report_config     = p_GnssCfg->measReportConfig;
  }
  if (p_GnssCfg->svPolyReportConfig_valid)
  {
    pz_PaInfo->z_set_gnss_config.sv_poly_report_config_valid = p_GnssCfg->svPolyReportConfig_valid;
    pz_PaInfo->z_set_gnss_config.e_sv_poly_report_config     = p_GnssCfg->svPolyReportConfig;
  }
  if ((TRUE == p_GnssCfg->reportFullEphemerisDb_valid) &&
      (TRUE == p_GnssCfg->reportFullEphemerisDb))
  {
    pz_PaInfo->z_set_gnss_config.reqEphDbFromMgp = TRUE;
  }
  if ((TRUE == p_GnssCfg->reportFullSvPolyDb_valid) &&
      (TRUE == p_GnssCfg->reportFullSvPolyDb))
  {
    pz_PaInfo->z_set_gnss_config.reqSvPolyDbFromMgp = TRUE;
  }
  /* Set up global PA module structure with PDAPI specific info */
  q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_GNSS_CONSTELL_REPORT_CONFIG, pz_PaInfo);
  LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_HIGH_5("locPa_SetGnssConstellRptCfg: RetVal=%d, MeasRptCfg=%d, SvPolyCfg=%d RptEph=%d RptPoly=%d",
                 q_RetVal, p_GnssCfg->measReportConfig, p_GnssCfg->svPolyReportConfig,
                 p_GnssCfg->reportFullEphemerisDb, p_GnssCfg->reportFullSvPolyDb);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to convert Pa command error from PDAPI to QMI LOC format

@param[in] e_CmdStatus  : Command Status sent from PDSM

@retval    corresponding qmiLocStatusEnumT_v02 value
---------------------------------------------------------------------------*/
static qmiLocStatusEnumT_v02 locPa_ConvertCmdError2Qmi(pdsm_pa_cmd_err_e_type e_CmdStatus)
{
  switch (e_CmdStatus)
  {
    case PDSM_PA_CMD_ERR_NOERR:
      return eQMI_LOC_SUCCESS_V02;

    case PDSM_PA_CMD_ERR_OFFLINE_S:
      return eQMI_LOC_PHONE_OFFLINE_V02;

    case PDSM_PA_CMD_ERR_PA_ACTIVE_S:
    case PDSM_PA_CMD_ERR_NOT_ALLOWED:
      return eQMI_LOC_ENGINE_BUSY_V02;

      case PDSM_PA_CMD_ERR_OTHER:
         return eQMI_LOC_TIMEOUT_V02;

    default:
         LOC_MSG_HIGH("pdsm_pa_cmd_err=%d convert to eQMI_LOC_GENERAL_FAILURE", e_CmdStatus, 0, 0);
      return eQMI_LOC_GENERAL_FAILURE_V02;
  }
}

/*---------------------------------------------------------------------------
@brief
  Function to send IPC to LocMW task for servicing Indication to previous
  QMI_LOC request.
  
  NOTE: This function is always called within LocMW critical section.

@param[in] l_ClientHandle : Loc API client handle
@param[in] e_ParamType    : Parameter Type sent from PDSM
@param[in] p_ParamInfo    : Parameter Information Response from PDSM
@param[in] e_ParamStatus  : Parameter Status sent from PDSM

@retval    TRUE    if IPC posted to LocMW successfully
@retval    FALSE   if IPC posted to LocMW unsuccessfully
---------------------------------------------------------------------------*/
static boolean locPa_SendQmiIndicationProxy
(
  loc_client_handle_type l_ClientHandle,
  const pdsm_pa_info_s_type*   pz_ParamInfo,
  pdsm_pa_cmd_err_e_type       e_ParamStatus
)
{
  os_IpcMsgType*       p_IpcMsg = NULL;
  locPa_PdapiIndMsg*   pz_IndMsg = NULL;

  /* Function parameters sanity check */
  if (NULL == pz_ParamInfo)
  {
    LOC_MSG_ERROR("locPa_SendQmiIndicationProxy: NULL parameters", 0, 0, 0);
    return FALSE;
  }

  /* Generate IPC message to LocMW */
  p_IpcMsg = os_IpcCreate(sizeof(*pz_IndMsg), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_LOC_MIDDLEWARE);
  if(NULL == p_IpcMsg)
  {
    LOC_MSG_ERROR("locPa_SendQmiIndicationProxy: os_IpcCreate failed", 0, 0, 0);
    return FALSE;
  }

  LOC_MSG_MED("locPa_SendQmiIndicationProxy: ClientHandle=%d, ParamType=%d, ParamStatus=%d",
               l_ClientHandle, pz_ParamInfo->pa_event, e_ParamStatus);

  p_IpcMsg->q_MsgId = LM_MIDDLEWARE_MSG_ID_QMI_LOC_PA_IND;
  pz_IndMsg = (locPa_PdapiIndMsg*)(p_IpcMsg->p_Data);
  pz_IndMsg->l_ClientHandle = l_ClientHandle;
  pz_IndMsg->e_PaCmdType    = loc_middleware_data.z_locPa_Data.e_PaCmdType; /* GET or SET Parameter */
  pz_IndMsg->z_ParamInfo    = *pz_ParamInfo;
  pz_IndMsg->e_ParamStatus  = e_ParamStatus;

  /* Generate IPC message to LocMW */
  if(FALSE == os_IpcSend(p_IpcMsg, THREAD_ID_LOC_MIDDLEWARE))
  {
    LOC_MSG_ERROR("locPa_SendQmiIndicationProxy: Sending IPC to LocMW failed", 0, 0, 0);
    os_IpcDelete(p_IpcMsg);
    return FALSE;
  }

  /* Client has been notified of the PA processing status, reset the PA module */
  locPa_ResetModule();

  return TRUE;
}

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
)
{
  qmiLocStatusEnumT_v02 e_IndStatus;

  if (NULL == p_IpcMsg)
  {
    LOC_MSG_ERROR("locPa_SendQmiIndication: Invalid IPC message", 0, 0, 0);
    return FALSE;
  }
  else
  {
    locPa_PdapiIndMsg* p_PdapiIndMsg = (locPa_PdapiIndMsg*)(p_IpcMsg->p_Data);

    /* Translate Indication Status from PDAPI to QMI_LOC type */
    e_IndStatus = locPa_ConvertCmdError2Qmi(p_PdapiIndMsg->e_ParamStatus);

    /* Determine if Indication is for GET or SET parameter */
    if (PDSM_PA_CMD_GET_PARAM == p_PdapiIndMsg->e_PaCmdType)
    {
      return locQmi_ProcessPaGetInd(p_PdapiIndMsg->l_ClientHandle,
                                    p_PdapiIndMsg->z_ParamInfo.pa_event,
                                   &(p_PdapiIndMsg->z_ParamInfo), e_IndStatus);
    }
    else if (PDSM_PA_CMD_SET_PARAM == p_PdapiIndMsg->e_PaCmdType)
    {
      return locQmi_ProcessPaSetInd(p_PdapiIndMsg->l_ClientHandle,
                                    p_PdapiIndMsg->z_ParamInfo.pa_event,
                                   &(p_PdapiIndMsg->z_ParamInfo), e_IndStatus);
    }
    else
    {
      LOC_MSG_ERROR("locPa_SendQmiIndication: Invalid PaCmdType", 0, 0, 0);
      return FALSE;
    }
  }
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Inject Supl Certificate Data QMI_LOC request

@param[in] l_ClientHandle   : Loc API client handle
@param[in] u_SuplCertId       : Certificate ID of the SUPL certificate
@param[in] q_SuplCertDataLen  : SUPL certificate length
@param[in] pu_SuplCertData    : SUPL certificate contents

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_InjectSuplCert
(
  loc_client_handle_type                  l_ClientHandle,
   uint8                      u_SuplCertId,
   uint32                     q_SuplCertDataLen,
   const uint8*               pu_SuplCertData
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;
  uint32            q_CharsWritten = 0;

   /* Called within loc_pa.c skip input parameter sanity check */
   if(NULL == pu_SuplCertData)
  {
      LOC_MSG_ERROR("locPa_InjectSuplCert: NULL pointer", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  /* Write the prefix to the file name */
  q_CharsWritten = GNSS_STRLCPY( b_SulpCertFileName,
                                 locPa_CertDirPrefix,
                                 sizeof(b_SulpCertFileName));

  if (q_CharsWritten > 0 &&
      q_CharsWritten < (PDSM_PD_FS_MAX_PATH_LEN_BYTES - 1))
  {
    /* Add the numerical suffix */
      b_SulpCertFileName[q_CharsWritten]     = '0' + u_SuplCertId;
    b_SulpCertFileName[q_CharsWritten + 1] = 0; 
      pz_PaInfo->efs_data.efs_data_filename_len = q_CharsWritten + 1;
      pz_PaInfo->efs_data.efs_data_filename = b_SulpCertFileName;
  }
  else
  {
      LOC_MSG_ERROR("locPa_InjectSuplCert: Filename too long: %d > %d",
                   q_CharsWritten, PDSM_PD_FS_MAX_PATH_LEN_BYTES, 0);
  }

   pz_PaInfo->efs_data.efs_data_file_operation = PDSM_FS_CREATE_WRITE_FILE;
   loc_middleware_data.z_locPa_Data.e_EfsFileOperation = PDSM_FS_CREATE_WRITE_FILE;
   pz_PaInfo->efs_data.efs_data_part_number    = 1;
   pz_PaInfo->efs_data.efs_data_total_parts    = 1;
   pz_PaInfo->efs_data.efs_data_reserved       = 0;

   pz_PaInfo->efs_data.efs_data_ptr = (unsigned char *)pu_SuplCertData;
   pz_PaInfo->efs_data.efs_data_len = q_SuplCertDataLen;

  /* Set up global PA module structure with PDAPI specific info */
   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_EFS_DATA, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_MED("locPa_InjectSuplCert: RetVal=%d", q_RetVal, 0, 0);
   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Delete EFS Data QMI_LOC request

@param[in] l_ClientHandle   : Loc API client handle
@param[in] p_DeleteSuplCert : Delete EFS Data Request received from client

@retval    LOC_API_SUCCESS  Converted successfully
@retval    <Anything Else>  Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_DeleteEfsData
(
  loc_client_handle_type                  l_ClientHandle,
  qmiLocDeleteSuplCertificateReqMsgT_v02* p_DeleteSuplCert
)
{
   uint32             q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type *pz_PaInfo = NULL;
  uint32            q_CharsWritten = 0;

  /* Function arguments sanity check */
  if (NULL == p_DeleteSuplCert)
  {
    LOC_MSG_ERROR("locPa_DeleteEfsData: NULL parameters", 0, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  if (p_DeleteSuplCert->suplCertId_valid)
  {
    if (LOC_PA_MAX_SUPL_CERT_ID < p_DeleteSuplCert->suplCertId)
    {
      LOC_MSG_ERROR("locPa_DeleteEfsData: interface allows only 0-9, Id=%d",
                     p_DeleteSuplCert->suplCertId, 0, 0);
         q_RetVal = LOC_API_INVALID_PARAMETER;
    }
      else
      {
    /* Write the prefix to the file name */
    q_CharsWritten = GNSS_STRLCPY( b_SulpCertFileName,
                                   locPa_CertDirPrefix,
                                       sizeof(b_SulpCertFileName));

    if (q_CharsWritten > 0 &&
        q_CharsWritten < (PDSM_PD_FS_MAX_PATH_LEN_BYTES - 1))
    {
      /* Add the numerical suffix */
      b_SulpCertFileName[q_CharsWritten]     = '0' + p_DeleteSuplCert->suplCertId;
      b_SulpCertFileName[q_CharsWritten + 1] = 0; 
            pz_PaInfo->efs_data.efs_data_filename_len   = q_CharsWritten + 1;
            pz_PaInfo->efs_data.efs_data_file_operation = PDSM_FS_DELETE_FILE;
            pz_PaInfo->efs_data.efs_data_filename = b_SulpCertFileName;
      loc_middleware_data.z_locPa_Data.e_EfsFileOperation = PDSM_FS_DELETE_FILE;
    }
    else
    {
      LOC_MSG_ERROR("locPa_DeleteEfsData: Filename too long: %d > %d",
                     q_CharsWritten, PDSM_PD_FS_MAX_PATH_LEN_BYTES, 0);
            q_RetVal = LOC_API_INVALID_PARAMETER;
         }
    }
  }
  else
  {
    /* If suplCertId is not valid, delete all certificates */
      pz_PaInfo->efs_data.efs_data_file_operation = PDSM_FS_PURGE_DIR;
    loc_middleware_data.z_locPa_Data.e_EfsFileOperation = PDSM_FS_PURGE_DIR;

    /* Provide the dir from which to delete all certificates */
    (void) GNSS_STRLCPY( b_SulpCertFileName, 
                         locPa_CertDirPath,
                           sizeof(b_SulpCertFileName));
  }

   pz_PaInfo->efs_data.efs_data_len            = 0;
   pz_PaInfo->efs_data.efs_data_ptr            = NULL;
   pz_PaInfo->efs_data.efs_data_part_number    = 0;
   pz_PaInfo->efs_data.efs_data_total_parts    = 0;
   pz_PaInfo->efs_data.efs_data_reserved       = 0;
   pz_PaInfo->efs_data.efs_data_filename = b_SulpCertFileName;

  /* Set up global PA module structure with PDAPI specific info */
   if(LOC_API_INVALID_PARAMETER != q_RetVal)
{
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_EFS_DATA, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_MED("locPa_DeleteEfsData: RetVal=%d, File Operation=%d [2=DEL, 4=PUR]",
               q_RetVal, pz_PaInfo->efs_data.efs_data_file_operation, 0);
   return q_RetVal;
   }

// Translate GNSS service data system & deleteSatelliteDataMask
static void locPa_XlateDeleteSatelliteData(
  const qmiLocDeleteSatelliteDataStructT_v02 *pz_DeleteSatelliteData,
  pdsm_delete_parms_type* pz_Param)
{
   if(NULL == pz_Param || NULL == pz_DeleteSatelliteData)
   {
      LOC_MSG_ERROR("locPa_XlateDeleteSatelliteData, 0x%x, 0x%x",
                     pz_Param, pz_DeleteSatelliteData, 0);
      return;
   }

   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_GPS_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, GPS, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH;
         pz_Param->eph_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM;
         pz_Param->alm_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_HEALTH;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVDIR;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVSTEER;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_CORR;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GPS;
         pz_Param->gps_gnss_sv_blacklist_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SA_DATA_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SADATA;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SV_NO_EXIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SV_NO_EXIST;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_IONO_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_IONO;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TIME_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_TIME_GPS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_MB_DATA_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_MB_DATA;
      }
    if (0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TGD_DATA_V02))
    {
      pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TGD_DATA_GPS;
    }
   }
   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_GLO_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, GLO, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_GLO;
         pz_Param->glo_eph_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_GLO;
         pz_Param->glo_alm_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_HEALTH_GLO;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVDIR_GLO;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVSTEER_GLO;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_CORR_GLO;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GLO;
         pz_Param->glo_gnss_sv_blacklist_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TIME_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_TIME_GLO;
      }
    if (0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TGD_DATA_V02))
    {
      pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TGD_DATA_GLO;
    }
   }
   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_BDS_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, BDS, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_BDS;
         pz_Param->bds_eph_sv_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_BDS;
         pz_Param->bds_alm_sv_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_HEALTH_BDS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVDIR_BDS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVSTEER_BDS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_CORR_BDS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_GNSS_SV_BLACKLIST_BDS;
         pz_Param->bds_gnss_sv_blacklist_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TIME_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_TIME_BDS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_IONO_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_IONO_BDS;
      }
    if (0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TGD_DATA_V02))
    {
      pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TGD_DATA_BDS;
    }
   }
   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_GAL_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, GAL, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_GAL;
         pz_Param->gal_eph_sv_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_GAL;
         pz_Param->gal_alm_sv_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVDIR_GAL;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVSTEER_GAL;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_CORR_GAL;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GAL;
         pz_Param->gal_gnss_sv_blacklist_mask = CGPS_MAX_UINT64;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TIME_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_TIME_GAL;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_IONO_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_IONO_GAL;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_HEALTH_GAL;
      }
    if (0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TGD_DATA_V02))
    {
      pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TGD_DATA_GAL;
    }
   }
   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_NAVIC_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, NavIC, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_EPH_NAVIC;
         pz_Param->navic_eph_sv_mask = CGPS_MAX_UINT16;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_ALM_NAVIC;
         pz_Param->navic_alm_sv_mask = CGPS_MAX_UINT16;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_SVDIR_NAVIC;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_SVSTEER_NAVIC;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_ALM_CORR_NAVIC;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_GNSS_SV_BLACKLIST_NAVIC;
         pz_Param->navic_gnss_sv_blacklist_mask = CGPS_MAX_UINT16;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TIME_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TIME_NAVIC;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_IONO_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_IONO_NAVIC;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_HEALTH_NAVIC;
      }
   }
 
   if(0 != (pz_DeleteSatelliteData->system & QMI_LOC_SYSTEM_QZSS_V02))
   {
      MSG_1(MSG_SSID_GNSS_LOCMW, MSG_LEGACY_MED, "locPa_XlateDeleteSatelliteData, QZSS, deleteSatelliteDataMask=0x%x",
                     pz_DeleteSatelliteData->deleteSatelliteDataMask);
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_EPHEMERIS_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_QZSS;
         pz_Param->qzss_eph_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALMANAC_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_QZSS;
         pz_Param->qzss_alm_sv_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVHEALTH_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_HEALTH_QZSS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVDIR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVDIR_QZSS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_SVSTEER_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_SVSTEER_QZSS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_ALM_CORR_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_CORR_QZSS;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_BLACKLIST_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_GNSS_SV_BLACKLIST_QZSS;
         pz_Param->qzss_gnss_sv_blacklist_mask = CGPS_MAX_UINT32;
      }
      if(0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_IONO_V02))
      {
         pz_Param->pdsm_delete_parms_flags |= PDSM_PA_DELETE_IONO_QZSS;
      }
    if (0 != (pz_DeleteSatelliteData->deleteSatelliteDataMask & QMI_LOC_DELETE_DATA_MASK_TGD_DATA_V02))
    {
      pz_Param->pdsm_delete_parms_flags_ext |= PDSM_PA_EXT_DELETE_TGD_DATA_QZSS;
    }
   }
}

//GNSS delete service data handler function
static uint32 locPa_XlateDeleteGnssServiceData
(
   const qmiLocDeleteGNSSServiceDataReqMsgT_v02  *pz_DeleteGnssServiceData,
   pdsm_delete_parms_type                        *pz_PdapiDeleteDataMask
)
{
   uint32    q_RetVal = LOC_API_SUCCESS;

   if(NULL == pz_DeleteGnssServiceData || NULL == pz_PdapiDeleteDataMask)
   {
      LOC_MSG_ERROR("locPa_XlateDeleteGnssServiceData, 0x%x, 0x%x",
                    pz_DeleteGnssServiceData,pz_PdapiDeleteDataMask,0);
      return LOC_API_INVALID_PARAMETER;
   }

   memset(pz_PdapiDeleteDataMask, 0, sizeof(*pz_PdapiDeleteDataMask));

   if( FALSE == LOC_MW_IS_VALID_BOOLEAN(pz_DeleteGnssServiceData->deleteAllFlag))
   {
      LOC_MSG_ERROR("locPa_XlateDeleteGnssServiceData, invalid deleteAllFlag %d",
                     pz_DeleteGnssServiceData->deleteAllFlag,0,0);
      return LOC_API_INVALID_PARAMETER;
   }

   if(TRUE == pz_DeleteGnssServiceData->deleteAllFlag )
   {
      qmiLocDeleteSatelliteDataStructT_v02  z_DeleteSatellitData = {0};
      z_DeleteSatellitData.system = CGPS_MAX_UINT32;
      z_DeleteSatellitData.deleteSatelliteDataMask = CGPS_MAX_UINT32;

      // clock Info
      pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_CLOCK_INFO;
      pz_PdapiDeleteDataMask->delete_clock_info_mask = PDSM_PA_DELETE_ALL_CLOCK_INFO;

    /*XTRA integrity data*/
    pz_PdapiDeleteDataMask->q_delete_xtra_int_mask = PDSM_PA_DELETE_XTRA_INTEGRITY_ALL;

      // cell db
      pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_CELLDB_INFO;
      pz_PdapiDeleteDataMask->delete_celldb_mask = CGPS_MAX_UINT32;

      // common data
      pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |=
         locPa_XlateQmiPdsmMask(z_QmiPdsmCommonMaskMap,
                                CGPS_MAX_UINT32,
                                sizeof(z_QmiPdsmCommonMaskMap)/sizeof(z_QmiPdsmCommonMaskMap[0]));

      // satellite data
      locPa_XlateDeleteSatelliteData(&z_DeleteSatellitData,
                                      pz_PdapiDeleteDataMask);

      // miscellaneous SBAS data
      pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_EPH_SBAS;
      pz_PdapiDeleteDataMask->sbas_eph_sv_mask = CGPS_MAX_UINT32;
      pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_ALM_SBAS;
      pz_PdapiDeleteDataMask->sbas_alm_sv_mask = CGPS_MAX_UINT32;
   }
   else
   {
       // TLV=0x10 - clock Info
      if(TRUE == pz_DeleteGnssServiceData->deleteClockInfoMask_valid)
      {
         pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_CLOCK_INFO;
         pz_PdapiDeleteDataMask->delete_clock_info_mask =
         locPa_XlateQmiPdsmMask(z_QmiPdsmClockInfoMaskMap,
                                pz_DeleteGnssServiceData->deleteClockInfoMask,
                                sizeof(z_QmiPdsmClockInfoMaskMap)/sizeof(z_QmiPdsmClockInfoMaskMap[0]));
      }

      // TLV=0x11 - cell db
      if(TRUE == pz_DeleteGnssServiceData->deleteCellDbDataMask_valid)
      {
         pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_CELLDB_INFO;
         pz_PdapiDeleteDataMask->delete_celldb_mask =
         locPa_XlateQmiPdsmMask(z_QmiPdsmCellDBMaskMap,
                                pz_DeleteGnssServiceData->deleteCellDbDataMask,
                                sizeof(z_QmiPdsmCellDBMaskMap)/sizeof(z_QmiPdsmCellDBMaskMap[0]));
      }

      // TLV=0x12 - common data
      if(TRUE == pz_DeleteGnssServiceData->deleteCommonDataMask_valid)
      {
         pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |=
         locPa_XlateQmiPdsmMask(z_QmiPdsmCommonMaskMap,
                                    pz_DeleteGnssServiceData->deleteCommonDataMask,
                                    sizeof(z_QmiPdsmCommonMaskMap)/sizeof(z_QmiPdsmCommonMaskMap[0]));
      }

      // TLV=0x13 - satellite data
      if(TRUE == pz_DeleteGnssServiceData->deleteSatelliteData_valid)
      {
         locPa_XlateDeleteSatelliteData(&pz_DeleteGnssServiceData->deleteSatelliteData,
                                             pz_PdapiDeleteDataMask);
      }

	   // TLV=0x14 - Extended clock Info
      if(TRUE == pz_DeleteGnssServiceData->deleteExtClockInfoMask_valid)
      {
         pz_PdapiDeleteDataMask->pdsm_delete_parms_flags |= PDSM_PA_DELETE_CLOCK_INFO;
         pz_PdapiDeleteDataMask->delete_clock_info_mask |=
         locPa_XlateQmiPdsmMask(z_QmiPdsmClockInfoMaskMap,
                                pz_DeleteGnssServiceData->deleteExtClockInfoMask,
                                sizeof(z_QmiPdsmClockInfoMaskMap)/sizeof(z_QmiPdsmClockInfoMaskMap[0]));
      }
   }

   return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
  Function to process the Delete GNSS Service Data QMI_LOC request

@param[in] l_ClientHandle           : Loc API client handle
@param[in] pz_DeleteGnssServiceData : Delete GNSS Service Data

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_DeleteGnssServiceData
(
   loc_client_handle_type l_ClientHandle,
   qmiLocDeleteGNSSServiceDataReqMsgT_v02* pz_DeleteGnssServiceData
)
{
   uint32                       q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type           *pz_PaInfo = NULL;

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   /* Skp input argument NULL check because it is done in locPa_XlateDeleteGnssServiceData */
   q_RetVal = locPa_XlateDeleteGnssServiceData (pz_DeleteGnssServiceData, &(pz_PaInfo->delete_params));

   if (q_RetVal == LOC_API_SUCCESS)
   {
      /* Set up global PA module structure with PDAPI specific info */
      q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_DELETE_PARAMS_EX1, pz_PaInfo);
   }
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_LOW("locPa_DeleteGnssServiceData: RetVal=%d", q_RetVal, 0,0);
   return q_RetVal;
}

/** loc_pa_set_xtra_pcid
 *  Handle the request to set XTRA PCID
 *  @param[in] client_handle: Loc API client handle
 *  @param[in] t_XtraPcid: XTRA PCID
 *   */
/*---------------------------------------------------------------------------
@brief
  Function to process the Inject XTRA PCID QMI_LOC request

@param[in] l_ClientHandle  : Loc API client handle
@param[in] pz_XtraPcid     : XTRA PCID

@retval    LOC_API_SUCCESS   Converted successfully
@retval    <Anything Else>   Converted unsuccessfully
---------------------------------------------------------------------------*/
static uint32 locPa_SetXtraPcid
(
   loc_client_handle_type l_ClientHandle,
   const qmiLocInjectXtraPcidReqMsgT_v02* const pz_XtraPcid
)
{
   uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type          *pz_PaInfo = NULL;

   if (l_ClientHandle < 0 || NULL == pz_XtraPcid) 
   {
      LOC_MSG_ERROR( "locPa_SetXtraPcid, client_handle = %d, pz_XtraPcid = %p",
                     l_ClientHandle,pz_XtraPcid,0);
      return LOC_API_INVALID_PARAMETER;
   }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   pz_PaInfo->pcid_info = pz_XtraPcid->xtraPcid;

   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_PCID, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_LOW ("locPa_SetXtraPcid, q_RetVal = %d",q_RetVal, 0,0);

   return q_RetVal;
}

/** locPa_SetLppeUpMode
 *  Handle the request to set LPPE UP mode
 *  @param[in] client_handle: Loc API client handle
 *  @param[in] t_LppeUpConfig: LPPE UP Mode mask
 *   */
static uint32 locPa_SetLppeUpMode
(
   loc_client_handle_type        l_ClientHandle,
   qmiLocLppeUpAuxTechMaskT_v02  t_LppeUpConfig
)
{
   uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type          *pz_PaInfo = NULL;

   if (l_ClientHandle < 0 ) 
   {
      LOC_MSG_ERROR( "locPa_SetLppeUpMode, l_ClientHandle = %d",l_ClientHandle,0, 0);
      return LOC_API_INVALID_PARAMETER;
   }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   if( QMI_LOC_LPPE_MASK_UP_DBH_V02 & t_LppeUpConfig)
   {
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_DBH_ENABLE;
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_ENABLE;
   }
   if( QMI_LOC_LPPE_MASK_UP_AP_WIFI_MEASUREMENT_V02 & t_LppeUpConfig)
   {
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_WLAN_ENABLE;
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_ENABLE;
   }
   if ( QMI_LOC_LPPE_MASK_UP_AP_SRN_BTLE_MEASUREMENT_V02 & t_LppeUpConfig ) 
   {
      /*TODO SRN BLE scans are disabled. Need to enable it once Public 
        Mac Address fix becomes available from HLOS*/
      //pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_SRN_BTLE_ENABLE;
      //pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_ENABLE;
   }
   if (QMI_LOC_LPPE_MASK_UP_UBP_V02 & t_LppeUpConfig )
   {
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_UBP_ENABLE;
      pz_PaInfo->lppe_up_config_info |= PDSM_PA_LPPE_ENABLE;
   }

   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_LPPE_UP_CONFIG, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_MED ("locPa_SetLppeUpMode, t_LppeUpConfig = 0x%8x q_RetVal = %d",
                  t_LppeUpConfig, q_RetVal, 0);
   return q_RetVal;
}

/** locPa_SetLppeCpMode
 *  Handle the request to set LPPE CP mode
 *  @param[in] client_handle: Loc API client handle
 *  @param[in] t_LppeCpConfig:  LPPE CP Mode mask
 *   */
static uint32 locPa_SetLppeCpMode
(
   loc_client_handle_type           l_ClientHandle,
   qmiLocLppeCpAuxTechMaskT_v02     t_LppeCpConfig
)
{
   uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;
   pdsm_pa_info_type          *pz_PaInfo = NULL;

   if (l_ClientHandle < 0 ) 
   {
      LOC_MSG_ERROR( "locPa_SetLppeCpMode, l_ClientHandle = %d",l_ClientHandle,0, 0);
      return LOC_API_INVALID_PARAMETER;
   }

   LOC_PA_ALLOC_PAINFO(pz_PaInfo)
   if( QMI_LOC_LPPE_MASK_CP_DBH_V02 & t_LppeCpConfig)
   {
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_DBH_ENABLE;
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_ENABLE;
   }
   if( QMI_LOC_LPPE_MASK_CP_AP_WIFI_MEASUREMENT_V02 & t_LppeCpConfig)
   {
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_WLAN_ENABLE;
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_ENABLE;
   }

   if ( QMI_LOC_LPPE_MASK_CP_AP_SRN_BTLE_MEASUREMENT_V02 & t_LppeCpConfig ) 
   {
      /*TODO SRN BLE scans are disabled. Need to enable it once Public 
        Mac Address fix becomes available from HLOS*/
      //pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_SRN_BTLE_ENABLE;
      //pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_ENABLE;
   }
   if (QMI_LOC_LPPE_MASK_CP_UBP_V02 & t_LppeCpConfig )
   {
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_UBP_ENABLE;
      pz_PaInfo->lppe_cp_config_info |= PDSM_PA_LPPE_ENABLE;
   }

   q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_LPPE_CP_CONFIG, pz_PaInfo);
   LOC_PA_FREE_PAINFO(pz_PaInfo)

   LOC_MSG_MED ("locPa_SetLppeCpMode, t_LppeCpConfig = 0x%8x q_RetVal = %d",
                  t_LppeCpConfig, q_RetVal, 0);
   return q_RetVal;
}

/** locPa_SetBlacklistSv
*  Handle the Blacklist SV requests
*  @param[in] client_handle: Loc API client handle
*  @param[in] pz_ReqMsg:  Pointer to the QMI Message payload
**/
static uint32 locPa_SetBlacklistSv(
  loc_client_handle_type l_ClientHandle, const qmiLocSetBlacklistSvReqMsgT_v02* pz_ReqMsg)
{
  uint32            q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type z_PaInfo = { 0 };

  if (l_ClientHandle < 0)
  {
    LOC_MSG_ERROR_1("locPa_SetBlacklistSv, l_ClientHandle = %d", l_ClientHandle);
    return LOC_API_INVALID_HANDLE;
  }

  if (NULL == pz_ReqMsg)
  {
    LOC_MSG_ERROR_0("locPa_SetBlacklistSv: NULL Pointer received for Set");
    return LOC_API_INVALID_PARAMETER;
  }

  /* Check if all TLVs are invalid */
  if (!pz_ReqMsg->glo_persist_blacklist_sv_valid &&
      !pz_ReqMsg->glo_clear_persist_blacklist_sv_valid &&
      !pz_ReqMsg->bds_persist_blacklist_sv_valid &&
      !pz_ReqMsg->bds_clear_persist_blacklist_sv_valid &&
      !pz_ReqMsg->qzss_persist_blacklist_sv_valid &&
      !pz_ReqMsg->qzss_clear_persist_blacklist_sv_valid &&
      !pz_ReqMsg->gal_persist_blacklist_sv_valid &&
      !pz_ReqMsg->gal_clear_persist_blacklist_sv_valid &&
      !pz_ReqMsg->sbas_persist_blacklist_sv_valid &&
      !pz_ReqMsg->sbas_clear_persist_blacklist_sv_valid)
  {
    LOC_MSG_ERROR_0("No Valid TLV in SET REQUEST");
    return LOC_API_INVALID_PARAMETER;
  }

  //Sanity check is done. Populate z_PaInfo and send to SM for processing.
  //GLONASS
  if (0 != pz_ReqMsg->glo_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.q_GloSlotIdMask = (uint32)pz_ReqMsg->glo_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: GLO SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.q_GloSlotIdMask);
  }
  if (0 != pz_ReqMsg->glo_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.q_GloSlotIdMask = (uint32)pz_ReqMsg->glo_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: GLO ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.q_GloSlotIdMask);
  }
  //BDS
  if (0 != pz_ReqMsg->bds_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.t_BdsPrnMask = pz_ReqMsg->bds_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: BDS SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.t_BdsPrnMask);
  }
  if (0 != pz_ReqMsg->bds_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.t_BdsPrnMask = pz_ReqMsg->bds_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: BDS ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.t_BdsPrnMask);
  }
  //QZSS
  if (0 != pz_ReqMsg->qzss_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.u_QzssPrnMask = (uint32)pz_ReqMsg->qzss_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: QZSS SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.u_QzssPrnMask);
  }
  if (0 != pz_ReqMsg->qzss_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.u_QzssPrnMask = (uint32)pz_ReqMsg->qzss_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: QZSS ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.u_QzssPrnMask);
  }
  //GALILEO
  if (0 != pz_ReqMsg->gal_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.t_GalPrnMask = pz_ReqMsg->gal_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: GAL SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.t_GalPrnMask);
  }
  if (0 != pz_ReqMsg->gal_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.t_GalPrnMask = pz_ReqMsg->gal_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: GAL ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.t_GalPrnMask);
  }

  //SBAS
  if (0 != pz_ReqMsg->sbas_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.t_SbasPrnMask = pz_ReqMsg->sbas_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: SBAS SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.t_SbasPrnMask);
  }
  if (0 != pz_ReqMsg->sbas_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.t_SbasPrnMask = pz_ReqMsg->sbas_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: SBAS ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.t_SbasPrnMask);
  }

  //NAVIC
  if (0 != pz_ReqMsg->navic_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_SetMask.w_NavicPrnMask = pz_ReqMsg->navic_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: NAVIC SetMask=0x%X", z_PaInfo.persistent_sv_mask.z_SetMask.w_NavicPrnMask);
  }
  if (0 != pz_ReqMsg->navic_clear_persist_blacklist_sv_valid)
  {
    z_PaInfo.persistent_sv_mask.z_ClrMask.w_NavicPrnMask = pz_ReqMsg->navic_clear_persist_blacklist_sv;
    LOC_MSG_HIGH_1("locPa_SetBlacklistSv: NAVIC ClrMask=0x%X", z_PaInfo.persistent_sv_mask.z_ClrMask.w_NavicPrnMask);
  }
  
  q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_PERSISTENT_SV_MASK, &z_PaInfo);

  LOC_MSG_MED_1("locPa_SetBlacklistSv: q_RetVal = %u", q_RetVal);
  return q_RetVal;
}

/*---------------------------------------------------------------------------
@brief
This function converts QMI Constellation to PDSM Type.

@param[in] qmiLocConstellationMaskT_v02

@retval    pdsm_constellation_mask
---------------------------------------------------------------------------*/
static pdsm_constellation_mask locPa_ConvertQmiConstellationMask_ToPdsm(
  qmiLocConstellationMaskT_v02 t_QmiMask)
{
  pdsm_constellation_mask t_PdsmOutMask = 0ULL;

  if (0 != (t_QmiMask & QMI_LOC_CONSTELLATION_GLO_V02))
  {
    t_PdsmOutMask |= PDSM_LOC_CONSTELLATION_GLO;
  }
  if (0 != (t_QmiMask & QMI_LOC_CONSTELLATION_BDS_V02))
  {
    t_PdsmOutMask |= PDSM_LOC_CONSTELLATION_BDS;
  }
  if (0 != (t_QmiMask & QMI_LOC_CONSTELLATION_QZSS_V02))
  {
    t_PdsmOutMask |= PDSM_LOC_CONSTELLATION_QZSS;
  }
  if (0 != (t_QmiMask & QMI_LOC_CONSTELLATION_GAL_V02))
  {
    t_PdsmOutMask |= PDSM_LOC_CONSTELLATION_GAL;
  }
  if (0 != (t_QmiMask & QMI_LOC_CONSTELLATION_NAVIC_V02))
  {
    t_PdsmOutMask |= PDSM_LOC_CONSTELLATION_NAVIC;
  }
  return t_PdsmOutMask;
}

/** locPa_SetConstellationControl
*  Handle the constellation config requests
*  @param[in] client_handle: Loc API client handle
*  @param[in] pz_ReqMsg:  Pointer to the QMI Message payload
**/
static uint32 locPa_SetConstellationControl(
  loc_client_handle_type l_ClientHandle, const qmiLocSetConstellationConfigReqMsgT_v02* pz_ReqMsg)
{
  uint32            q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type z_PaInfo = { 0 };

  if (l_ClientHandle < 0)
  {
    LOC_MSG_ERROR_1("locPa_SetConstellationControl, l_ClientHandle = %d", l_ClientHandle);
    return LOC_API_INVALID_HANDLE;
  }

  if (NULL == pz_ReqMsg)
  {
    LOC_MSG_ERROR_0("locPa_SetConstellationControl: NULL Pointer received for Set");
    return LOC_API_INVALID_PARAMETER;
  }

  /* If Reset is FALSE and Disable is valid, it will be ignored */
  if (!pz_ReqMsg->resetConstellations  && 
      pz_ReqMsg->disableMask_valid)
  {
    LOC_MSG_ERROR_0("locPa_SetConstellationControl: DisableTlv valid but not supported. Ignoring");
  }

  /* If Reset is FALSE and Enable is not valid, there is nothing to do */
  if (!pz_ReqMsg->resetConstellations &&
      !pz_ReqMsg->enableMask_valid)
  {
    LOC_MSG_ERROR_0("locPa_SetConstellationControl: EnableTlv invalid. Nothing to do");
    return LOC_API_INVALID_PARAMETER;
  }

  /* If Reset is FALSE && EnableMask is valid but 0.
     Nothing to enable */
  if (!pz_ReqMsg->resetConstellations &&
      pz_ReqMsg->enableMask_valid &&
      !pz_ReqMsg->enableMask)
  {
    LOC_MSG_ERROR_0("locPa_SetConstellationControl: Nothing to enable");
    return LOC_API_INVALID_PARAMETER;
  }
  //Sanity check is done. Populate z_PaInfo and send to SM for processing.
  if (TRUE == pz_ReqMsg->resetConstellations)
  {
    z_PaInfo.z_constell_cfg.z_Set.v_resetToNvDefault = TRUE;
    z_PaInfo.z_constell_cfg.z_Set.t_enableMask = 0ULL;
    LOC_MSG_HIGH_0("locPa_SetConstellationControl: Reset To NV Default");
  }
  else
  {
    z_PaInfo.z_constell_cfg.z_Set.v_resetToNvDefault = FALSE;
    z_PaInfo.z_constell_cfg.z_Set.t_enableMask = locPa_ConvertQmiConstellationMask_ToPdsm(pz_ReqMsg->enableMask);
    LOC_MSG_HIGH_1("locPa_SetConstellationControl: SET enableMask=0x%X",
      z_PaInfo.z_constell_cfg.z_Set.t_enableMask);
  }
  q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_CONSTELLATION_CONTROL_CONFIG, &z_PaInfo);

  LOC_MSG_MED_1("locPa_SetConstellationControl: q_RetVal = %u", q_RetVal);
  return q_RetVal;
}

/** locPa_SetEmergencyCbWindow
*  Handle the request to set emergency callback window
*  @param[in] client_handle: Loc API client handle
*  @param[in] emergencyCallbackWindow:  Emergency callback window (in seconds)
**/
static uint32 locPa_SetEmergencyCbWindow
(
   loc_client_handle_type  l_ClientHandle,
   uint32_t emergencyCallbackWindow
   )
{
  uint32                      q_RetVal = LOC_API_GENERAL_FAILURE;
  pdsm_pa_info_type          *pz_PaInfo = NULL;

  if (l_ClientHandle < 0)
  {
    LOC_MSG_ERROR("locPa_SetEmergencyCbWindow, l_ClientHandle = %d", l_ClientHandle, 0, 0);
    return LOC_API_INVALID_PARAMETER;
  }

  LOC_PA_ALLOC_PAINFO(pz_PaInfo)
  pz_PaInfo->q_emergency_extension_window_config = emergencyCallbackWindow;

  q_RetVal = locPa_SetParam(l_ClientHandle, PDSM_PA_EMERGENCY_EXTENSION_WINDOW, pz_PaInfo);
  LOC_PA_FREE_PAINFO(pz_PaInfo)

  LOC_MSG_MED("locPa_SetEmergencyCbWindow, emergencyCallbackWindow = %d q_RetVal = %d",
              emergencyCallbackWindow, q_RetVal, 0);
  return q_RetVal;
}
