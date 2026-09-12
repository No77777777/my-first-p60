#ifndef SM_API_H
#define SM_API_H
/*------------------------------------------------------------------------------
  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.   
                  
      Export of this technology or software is regulated by the U.S. Government.
      Diversion contrary to U.S. law prohibited.
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
File comment
------------------------------------------------------------------------------*/

/**
  @file sm_api.h 
    
  SM interface exposed to other modules

*/

/*------------------------------------------------------------------------------
Mainpage
------------------------------------------------------------------------------*/
/**
  @mainpage SM Public API
  
*/

/**
     @defgroup SM Session Manager
     
     TODO: Add info about what session manager does here
*/

/*=============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/inc/sm_api.h#12 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/10/20   pa      Add support for PDR Mag Cal State
01/1/20    anksha  Added IPC for navic assist data download
12/1/19    jeev    IPC fix report upgrade
09/09/19   nath     Almanac and ephimeris flag reporting in NAVIC
02/19/19   skm     SUPL over WIFI phase 2 
07/24/19   nath    Added support for ME metrics
02/19/19   skm     SUPL over WIFI phase 2  
07/20/18   skm     Boeing CaaS Feature
03/29/18   sum     Added support for Unpropagated fix reporting
02/12/18   akas    Added Support for QZSS L1-s
05/29/17   sum     Added two DOP fields TDOP and GDOP
11/08/16   rn      Added Xtra Integrity support. 
08/10/16   mj      Handle Leap Second Info updates from ME
02/17/16   mj      Added a new bool u_CpiInconsistent into sm_GnssNavSolution to flag CPI inconsistencies 
02/02/16   mk      Add support for LPPM/PDR3.0 feature
09/23/15   hs      Unified fix reporting API for all fixes to be reported to SM
                   via this API
08/28/15   mj     Add support for INS only fixes for SAP changes
07/09/15   rh      Keep sm_UpdateCellIDInfo() API for SMART even with CM CID change  
07/03/15   jv      QZSS API Changes
06/04/15   jyw     Added CPE Timing Control
03/18/15   ss       Added function to report Wifi Pos fix response
01/08/15   gk      new pdapi events
11/18/14   ah      Added support for PrisonerSV persistent blacklist SV masks
09/15/14   jv      Galileo GNSS Support
08/08/14   jv      16-bit SVID support
07/24/14   jyw     Reserving IPC for CPE measurements 
07/18/14   jyw     Added API for CPE measurements
03/10/14   ah      Added support for Get Avail WWAN Pos
12/02/13   ah      Support GPS State Info requests for multiple clients simultaneously
10/28/13   hm      Added SM error event report for signal processing memory allocation failure
06/13/13   rh      Added sm_GnssNavSvInfo for SV info & sm_BdsAssistDataStruct
06/07/13   rh      Removed NO_FIX report and changed FIX report structure 
03/20/13  zhangwen Updated the IPCs in correspondence to payload changes in nofix/fix reports
01/28/13   mj      Updated sm_RegisterApiNotifier to include new param clientId
02/06/13   ah      Added support for BDS (BeiDou)
04/18/12   ssu     Adding sm_PositionReportStructType for GPS state information.
03/14/12   ss      Support of reset location serive.
02/27/12   rh      Added sm_SendEngineErrorRecoveryReport() API 
03/28/11   rh      Added heading uncertainty
04/28/10   kgc     Added bad health list to gnss assistance data. redfined ipc
02/12/10   vp      Added Sv polynomial function for OEM DRE enablement
06/10/09   gk      ODP 2.0 support
06/10/09   gk      Added Get UTC offset from NV
06/10/09   gk      ODP 1.1 support
01/12/09   atien   XTRA-T support
09/15/08   gk      Rename the on-demand struct to sm_ from cgps_04/27/08   gk      On_Demand Changes
04/17/08   jb      Allow changing IPC log with modified gps_NavSolutionExtra structure
08/01/07   rw      Add support for XTRA feature
03/27/07   mr      Removed positionflag #define s from this file
02/08/07   br      Added field to PPM report structure.
12/19/06   br      Clean-up.
12/01/06   jw      Modified sm_GpsMeasRptStructType to include gps_MeasBlkStructType
                   instead of redefining each field.
10/20/06   br      Exchanged include pdsmsrch.h with gps_1xl1_api.h.
07/03/06   mr      Changes MSG-IDs from macros to enums
04/04/06   sk      Added more definitions
03/20/06   ld      Initial version.

=============================================================================*/


/*=============================================================================

                                INCLUDE FILES

=============================================================================*/
#include "gps_variation.h"
#include "customer.h"  /* Customer configuration file */
#include "comdef.h"    /* Definition for basic types and macros */
#include "gps_common.h"
#include "gps_rrc_common.h"
#include "gnss_common.h"
#include "gnss_consts.h"
#include "aries_os_api.h"
#include "aries_os_globals.h"
#include "cgps_api.h"
#include "pdapi.h"
#include "gnss_cqsvc.h"  /* For Engine Error Recovery Report */
#include "gnss_lpp_ecid.h" /* LPP ECID definitions */
#include "wl_common.h"
#include "slim_client_types.h"
#include "mgp_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*=============================================================================

                       DEFINITIONS AND DECLARATIONS FOR MODULE

  =============================================================================*/

  /*=============================================================================

         Constants

  =============================================================================*/
#define MAX_PPM_RM_RPT_SAMPLES 25
#define NUM_SOFT_DECISIONS 20

#ifdef FEATURE_SM_DEBUG
#error code not present
#else
#define SM_ASSERT( xx_exp ) \
     if( !(xx_exp) ) \
     { \
        ERR( "SM: Assertion " #xx_exp " failed",0,0,0 ); \
     }
#endif /* FEATURE_SMLM_DEBUG */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                           IPC DEFINITIONS                               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/******************************************************************************
!!!!!!!!!!!!!! ATTENTION: Master copy of IPCs should be updated !!!!!!!!!!!!!!!

New IPCs created on any branch should do the following:
1) Use the IPC definitions in Master branch as baseline
Master Branch: //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

For TM IPCs, refer to:
Message ID                : m_tm_ipc_msg_id_type
Message ID to IPC mapping : tm_ipc_msg_id_type

For LM IPCs, refer to
Message ID                : m_lm_ipc_msg_id_type
Message ID to IPC mapping : lm_ipc_msg_id_type

After adding new IPCs to the CPL/mainline branch, update the Master copy of
sm_api.h at //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

******************************************************************************/

#define TM_CORE_MSG_ID_START 0x1027
#define SM_GEN8_IPC_ID_START_RANGE 0x10000

/* LM IPC Message ID Enum definitions */
typedef enum
{
  M_ARIES_MSG_ID_MGP_LM_FIRST_0 = C_USR_MSG_ID_FIRST,
  /*********************************************************************
   ** MSG ID's: MGP Info reports MGP => LM
   **********************************************************************/
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_0_0 = 11,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_0_0 = 12,
  M_ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_0_0 = 13,
  M_ARIES_MSG_ID_MGP_LM_REPORT_UTC_INFO_0 = 14,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_0_0 = 15,

  /***************************************************
   ** MSG ID's: MGP status reports MGP => LM
   ****************************************************/
  M_ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_0_0 = 16,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GENERAL_STATUS_0 = 17,
  M_ARIES_MSG_ID_MGP_LM_REPORT_EVENT_0 = 18,

  /***************************************************
   ** MSG ID's: PGI RF Info reports MGP => LM
   ****************************************************/
  M_ARIES_MSG_ID_MGP_LM_REPORT_RF_INFO_0 = 19,

  /***************************************************
   ** MSG ID's: CDMA reports AFLT => LM
   ****************************************************/
  M_ARIES_MSG_ID_AFLT_LM_REPORT_PPM_MEAS_0 = 20,
  M_ARIES_MSG_ID_AFLT_LM_REPORT_STATUS_0 = 21,
  M_ARIES_MSG_ID_AFLT_LM_REPORT_PPM_MEAS_IS801_0 = 22,

  /***************************************************
   ** MSG ID's: MGP data request MGP => LM
   ****************************************************/
  M_ARIES_MSG_ID_MGP_LM_REQ_SA_DATA_0 = 23,

  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_0_0 = 24,

  /***************************************************
   ** NOTE WELL:
   ** Revised/new messages - must go at end to avoid
   ** renumbering the messages above, s.t. tools like
   ** Apex and Playback can parse both Old and New
   ** files.
   ****************************************************/
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_1_0 = 25,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_1_0 = 26,

  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_2_0 = 27,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_2_0 = 28,

  /** MSG from MC with the Cno, Freq info from the IQ test **/
  M_ARIES_MSG_ID_MGP_LM_REPORT_WB_IQ_PARAMS_0 = 29,
  M_ARIES_MSG_ID_MGP_LM_END_0 = 30,

  M_ARIES_MSG_ID_TM_LM_FIRST_0 = M_ARIES_MSG_ID_MGP_LM_END_0,
  M_ARIES_MSG_ID_TM_LM_SARF_MODE_SWITCH_0 = 31,
  M_ARIES_MSG_ID_TM_LM_PRESC_DWELL_0 = 32,
  M_ARIES_MSG_ID_TM_LM_IQ_TEST_0 = 33,
  M_ARIES_MSG_ID_TM_LM_1SV_STANDALONE_0 = 34, /* Also serves single sv */
  M_ARIES_MSG_ID_TM_LM_MEM_BW_TEST_0 = 35,
  M_ARIES_MSG_ID_TM_LM_TSG_CONFIG_0 = 36,
  M_ARIES_MSG_ID_TM_LM_RF_CONFIG_0 = 37,
  M_ARIES_MSG_ID_TM_LM_RFDEV_TEST_0 = 38, /* Callisto RF Dev Multichan Track test command message */
  M_ARIES_MSG_ID_TM_LM_PRX_RF_CONFIG_0 = 39,
  M_ARIES_MSG_ID_TM_LM_RF_LINEARITY_CONFIG_0 = 40,
  M_ARIES_MSG_ID_TM_LM_RF_DEV_FEATURE_CAPABILITIES_0 = 41, /* Query for build's RF Dev feature capabilities*/
  M_ARIES_MSG_ID_TM_LM_GET_LATEST_WB_FFT_RESULTS_0 = 42,

  /* Following enums are reserved for OEMs usage */
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_1 = 43,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_2 = 44,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_3 = 45,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_4 = 46,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_5 = 47,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_6 = 48,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_7 = 49,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_8 = 50,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_9 = 51,
  M_ARIES_MSG_ID_TM_LM_OEM_RESERVED_10 = 52,
  M_ARIES_MSG_ID_TM_LM_SPECTRUM_ANALYZER_CMD_0 = 53,
  M_ARIES_MSG_ID_TM_LM_END_0 = 54,

  M_C_SM_LM_MSG_FIRST_0 = M_ARIES_MSG_ID_TM_LM_END_0,
  M_C_SM_LM_MSG_SESSION_REQUEST_0 = 55,    /* LM-TM session related request from TM to LM */
  M_C_SM_LM_MSG_REQUEST_0 = 56,            /* LM-TM session independent request from TM to LM */
  M_C_SM_LM_MSG_INFO_0 = 57,               /* LM-TM session independent info from TM to LM */
  M_C_SM_LM_DIAG_GSC_MSG_0 = 58,           /* GPS Session Control (GSC) DIAG packet from GPSDIAG to LM */
  M_C_SM_LM_SBAS_START_0 = 59,
  M_C_SM_LM_SBAS_STOP_0 = 60,
  M_C_SM_LM_SV_NO_EXIST_0 = 61,            /* Sv No Exist List updated from TM to LM */
  M_C_SM_LM_FORCE_DPO_FOR_POWER_MEAS_0 = 62, /* Force DPO for Power Meas Cmd from TM to LM */
  M_C_SM_LM_MSG_END_0 = 63,

  /****************************************************/
  /********** GEN8 MessageId Range Start **************/
  /****************************************************/
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_0 = SM_GEN8_IPC_ID_START_RANGE,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_3_0 = 65537,
  M_ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_1_0 = 65538,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_3_0 = 65539,
  M_ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_1_0 = 65540,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_0 = 65541,
  M_ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_0 = 65542,
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_0 = 65543,
  M_SM_MSG_ID_MGP_LM_DRSYNC_CONFIG_0 = 65544,
  M_SM_MSG_ID_MGP_LM_DRSYNC_PULSE_DESC_0 = 65545,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_4_0 = 65546,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_5_0 = 65547,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_4_0 = 65548,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_6_0 = 65549,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_5_0 = 65550,
  M_ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_2_0 = 65551,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_7_0 = 65552,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_6_0 = 65553,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_8_0 = 65554,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_0 = 65555,
  M_SM_MSG_ID_MGP_LM_DEBUG_NMEA_0 = 65556,
  M_SM_MSG_ID_MGP_LM_REPORT_XTRA_INJ_DONE_0 = 65557,
  M_SM_MSG_ID_LM_MGP_REPORT_XTRA_INJ_DONE_0 = 65558,
  M_SM_MSG_ID_MGP_LM_REPORT_NBIQ_PARAMS_0 = 65559,
  M_SM_MSG_ID_MGP_LM_REPORT_GNSS_RF_STATUS_INFO_0 = 65560,
  M_ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_3_0 = 65561,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_9_0 = 65562,
  M_SM_MSG_ID_MGP_LM_REPORT_GPS_EPHEMERIS_0 = 65563,
  M_SM_MSG_ID_MGP_LM_REPORT_GLO_EPHEMERIS_0 = 65564,
  M_SM_MSG_ID_MGP_LM_REPORT_GPS_SUBFRAME_0 = 65565,
  M_SM_MSG_ID_MGP_LM_REPORT_GLO_STRING_0 = 65566,
  M_SM_MSG_ID_MGP_RESET_LOCATION_SERVICE_DONE_0 = 65567,
  M_ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_0 = 65568,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_10_0 = 65569,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_0 = 65570,
  M_ARIES_MSG_ID_MGP_LM_REPORT_CPE_MEAS_0 = 65571,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_1 = 65572,
  M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_1 = 65573, /* Deprecated */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_1 = 65574,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_1 = 65575,
  M_SM_MSG_ID_MGP_LM_REPORT_BDS_EPHEMERIS_0 = 65576,
  M_SM_MSG_ID_MGP_LM_REPORT_GAL_EPHEMERIS_0 = 65577,
  M_ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_1 = 65578,
  M_SM_MSG_ID_MGP_LM_REPORT_GAL_PAGE_0 = 65579,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_1 = 65580,
  M_SM_MSG_ID_MGP_LM_REPORT_QZSS_EPHEMERIS_0 = 65581,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_2 = 65582,   /* Unified fix reporting API */
  M_ARIES_MSG_ID_MGP_LM_UPDATE_CPE_CONTROL_0 = 65583,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_3 = 65584,   /* Faulty CPI Reporting API */
  M_ARIES_MSG_ID_MGP_LM_XO_OFFSET_INFO = 65585,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_2 = 65586,
  M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_3 = 65587,
  M_SM_MSG_ID_LM_MGP_REPORT_XTRA_INTEGRITY_0 = 65588,
  M_SM_MSG_ID_MGP_LM_REPORT_XTRA_INTEGRITY_INJ_DONE_0 = 65589,
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_4 = 65590,   /* Multiband fix Reporting API */
  M_SM_MSG_ID_MGP_LM_HANDLE_DC_REPORT_0 = 65591,
  M_ARIES_MSG_ID_MGP_LM_REPORT_SVINFO_0 = 65592,
  M_ARIES_MSG_ID_MGP_LM_REPORT_SVMEASUSAGE_0 = 65593,
  M_SM_MSG_ID_MGP_LM_REPORT_UNPROPAGATED_FIX_1 = 65594,
  M_SM_MSG_ID_LM_LFM_CONFIG_0 = 65595,
  M_ARIES_MSG_ID_REPORT_ME_METRICS_0 = 65596,
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_0 = 65597, /* 1hz Meas - B2A, Navic, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_4 = 65598, /* 1hz Meas - B2A, Navic, Dynamic Memory */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_0 = 65599, /* Nhz Meas - B2A, Navic, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_0 = 65600, /* Nhz Meas - B2A, Navic, Dynamic Memory */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_2 = 65601, /* SV Poly with Multiband Information */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_1 = 65602, /* 1hz Meas - add systemTick, Logging Only */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_5 = 65603, /* 1hz Meas - add systemTick, Dynamic Memory */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_1 = 65604, /* Nhz Meas - add systemTick, Logging Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_1 = 65605, /* Nhz Meas - add systemTick, Dynamic Memory */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_5 = 65606,   /* DGNSS Fix report */  
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_6 = 65607, /* 1hz Meas + DGNSS Info - B2A, Navic, Dynamic Memory */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_6 = 65608,   /* IPC fix report for Navic */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_2 = 65609, /* 1hz Meas - B2A, Navic, new fields Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_2 = 65610, /* Nhz Meas - B2A, Navic, new fields Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_2 = 65611, /* Nhz Meas - add DGNSS */
  M_SM_MSG_ID_MGP_LM_REQ_XTRA_NAVIC_EPH_ASSIST_0 = 65612, /* Navic assist download request from MGP to LM */
  M_SM_MSG_ID_LM_MGP_REPORT_XTRA_NAVIC_ASSIST_DATA_0 = 65613, /* Navic assist ephemeris report to MGP  */
  M_SM_MSG_ID_LM_MGP_REPORT_XTRA_CONTROL_RECORD_DATA_0 = 65614, /* Xtra Control record  data to MGP */
  /* Reserved IDs 65615 - 65999 */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_66600 = 66600, /* Add B1C Support, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_66601 = 66601, /*  Add B1C Support NHz, Logging ID Only */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_66602 = 66602, /* SV Poly with B1C TGD/ISC Support and MB Health */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66603 = 66603,   /* Fix report with SystemTick and SystemTickUncMs */
  M_SM_MSG_ID_MGP_LM_LATENCY_REPORT_66604 = 66604,  /* Latency report */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_66605 = 66605, /* Add L2C/E5B Support, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_66606 = 66606, /*Add L2C/E5B Support NHz, Logging ID Only */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66607 = 66607, /* Added Integrity Risk, SystemTick and SystemTickUnc field*/
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66608 = 66608,  /* Added Protection AT/XT/Vertical */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66609 = 66609, /* Added DGNSS usage mask */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66610 = 66610, /* Added DCP mask */
  /* Added Integrity Risk, SystemTick and SystemTickUnc field*/
  M_MSG_ID_MGP_LM_BATCH_POLY_REPORT_66612 = 66612, /* Batched SV poly report */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG_66613 = 66613, /* Add B2B Support, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG_66614 = 66614, /*  Add B2B Support NHz, Logging ID Only */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_66615 = 66615, /* SV Poly with B2B TGD/ISC Support */
  M_MSG_ID_MGP_LM_BATCH_POLY_REPORT_66616 = 66616, /* Batched SV poly report - Add B2b TGD/ISC Support*/
  M_MSG_ID_MGP_LM_GOODNESS_IND_66617      = 66617, /* Goodness indication from NF to LM */
  M_SM_MSG_ID_MGP_LM_SELF_TEST_RESULTS_66618 = 66618, /* Self-Test results from MGP to LM */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_66619    = 66619, /* Higher Order SV Poly Support */
  M_MSG_ID_MGP_LM_BATCH_POLY_REPORT_66620 = 66620, /* Higher Order Batched SV Poly Support*/
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_LOG_66621 = 66621, /* Added ML Inference Pseudorange correction, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_LOG_66622 = 66622, /* Added ML Inference Pseudorange correction NHz, Logging ID Only */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_66623 = 66623, /* 1hz Meas ML Inference, Dynamic Memory */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_66624 = 66624, /* Bhz Meas - add Ml Inference */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66625 = 66625, /* Removed Conformity Index */
  M_MSG_ID_MGP_LM_REPORT_SV_POLY_66626    = 66626, /* Added Navic L1 structure fields */
  M_MSG_ID_MGP_LM_BATCH_POLY_REPORT_66627    = 66627, /* Added Navic L1 structure fields */
  M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66628    = 66628, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_LOG_66629    = 66629, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_LOG_66630    = 66630, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_66631    = 66631, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_66632    = 66632, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_UNPROPAGATED_FIX_66633    = 66633, /* Added Navic L1 structure fields */
  M_SM_MSG_ID_MGP_LM_REPORT_JAMMING_66634 = 66634, /* Jamming notification from MC */
  M_SM_MSG_ID_MGP_LM_TDM_EN_UPDATE_66635 = 66635, /* PE to TM TDM Enablement Update */
  /******************************************************************************
  When adding a new LM msg ID, you MUST insert any new items from the Master list,
  before your new item (and update the Master list). Follow the procedure below:

  1) Using the IPC definitions in Master branch as baseline,
     replace the enum m_lm_ipc_msg_id_type.
     Master Branch: //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

  2) In enum m_lm_ipc_msg_id_type, add a new message ID for the new IPC

  3) In enum lm_ipc_msg_id_type, create a mapping of the new IPC message
     with the message id from (2)

  In master IPC file sm_api.h at //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h
  Update enums m_lm_ipc_msg_id_type and lm_ipc_msg_id_type
  with the new IPC that you added.
  ******************************************************************************/
  M_C_SM_LM_IPC_MSG_END
} m_lm_ipc_msg_id_type;

/* LM Message ID to IPC mapping */
typedef enum
{
  ARIES_MSG_ID_MGP_LM_FIRST = C_USR_MSG_ID_FIRST,
  /*********************************************************************
  ** MSG ID's: MGP Info reports MGP => LM
  **********************************************************************/
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_0,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_0,
  ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_0,
  ARIES_MSG_ID_MGP_LM_REPORT_UTC_INFO,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_0,

  /***************************************************
  ** MSG ID's: MGP status reports MGP => LM
  ****************************************************/
  ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_0,
  ARIES_MSG_ID_MGP_LM_REPORT_GENERAL_STATUS,
  ARIES_MSG_ID_MGP_LM_REPORT_EVENT,

  /***************************************************
  ** MSG ID's: PGI RF Info reports MGP => LM
  ****************************************************/
  ARIES_MSG_ID_MGP_LM_REPORT_RF_INFO,

  /***************************************************
  ** MSG ID's: CDMA reports AFLT => LM
  ****************************************************/
  ARIES_MSG_ID_AFLT_LM_REPORT_PPM_MEAS,
  ARIES_MSG_ID_AFLT_LM_REPORT_STATUS,
  ARIES_MSG_ID_AFLT_LM_REPORT_PPM_MEAS_IS801,

  /***************************************************
  ** MSG ID's: MGP data request MGP => LM
  ****************************************************/
  ARIES_MSG_ID_MGP_LM_REQ_SA_DATA,

  ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_0,

  /***************************************************
  ** NOTE WELL:
  ** Revised/new messages - must go at end to avoid
  ** renumbering the messages above, s.t. tools like
  ** Apex and Playback can parse both Old and New
  ** files.
  ****************************************************/
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_1,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_1,

  ARIES_MSG_ID_MGP_LM_REPORT_FIX_2,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_2,

  /** MSG from MC with the Cno, Freq info from the IQ test **/
  ARIES_MSG_ID_MGP_LM_REPORT_WB_IQ_PARAMS,

  ARIES_MSG_ID_MGP_LM_PRESC_DWELL_RESULTS,

  ARIES_MSG_ID_MGP_LM_NAV_CONFIG_CHANGE_RESP,

  ARIES_MSG_ID_MGP_LM_END,

  /****************************************************/
  /********** GEN8 MessageId Range Start **************/
  /****************************************************/
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_1 = SM_GEN8_IPC_ID_START_RANGE,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_3,
  ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_1,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_3,
  ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_1,
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_1,
  ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_2,
  MSG_ID_MGP_LM_REPORT_SV_POLY_0,
  SM_MSG_ID_MGP_LM_DRSYNC_CONFIG,
  SM_MSG_ID_MGP_LM_DRSYNC_PULSE_DESC,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_4,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_5,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_4,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_6,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_5,
  ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_2,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_7,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_6,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_8,
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_7,
  SM_MSG_ID_MGP_LM_DEBUG_NMEA,
  SM_MSG_ID_MGP_LM_REPORT_XTRA_INJ_DONE,
  SM_MSG_ID_LM_MGP_REPORT_XTRA_INJ_DONE,
  SM_MSG_ID_MGP_LM_REPORT_NBIQ_PARAMS,
  SM_MSG_ID_MGP_LM_REPORT_GNSS_RF_STATUS_INFO,
  ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX_3,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_9,
  SM_MSG_ID_MGP_LM_REPORT_GPS_EPHEMERIS,
  SM_MSG_ID_MGP_LM_REPORT_GLO_EPHEMERIS,
  SM_MSG_ID_MGP_LM_REPORT_GPS_SUBFRAME,
  SM_MSG_ID_MGP_LM_REPORT_GLO_STRING,
  SM_MSG_ID_MGP_RESET_LOCATION_SERVICE_DONE,
  ARIES_MSG_ID_MGP_LM_REPORT_NO_FIX,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_10,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_11,

  /***************************************************************************/
  /* All additions below this line must be done using master list            */
  /***************************************************************************/

  ARIES_MSG_ID_MGP_LM_REPORT_CPE_MEAS           = M_ARIES_MSG_ID_MGP_LM_REPORT_CPE_MEAS_0,
  /* DEPRECATED MSG_ID_MGP_LM_REPORT_SV_POLY    = M_MSG_ID_MGP_LM_REPORT_SV_POLY_1,*/
  ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE       = M_ARIES_MSG_ID_MGP_LM_REPORT_POS_ESTIMATE_1,  /* Deprecated */
  /* ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS = M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_3, DEPRECATED*/
  SM_MSG_ID_MGP_LM_REPORT_BDS_EPHEMERIS         = M_SM_MSG_ID_MGP_LM_REPORT_BDS_EPHEMERIS_0,
  SM_MSG_ID_MGP_LM_REPORT_GAL_EPHEMERIS         = M_SM_MSG_ID_MGP_LM_REPORT_GAL_EPHEMERIS_0,
  ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS  = M_ARIES_MSG_ID_MGP_LM_REPORT_ASSISTANCE_STATUS_1,
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME           = M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_TIME_1,
  SM_MSG_ID_MGP_LM_REPORT_GAL_PAGE              = M_SM_MSG_ID_MGP_LM_REPORT_GAL_PAGE_0,
  SM_MSG_ID_MGP_LM_REPORT_QZSS_EPHEMERIS        = M_SM_MSG_ID_MGP_LM_REPORT_QZSS_EPHEMERIS_0,
  ARIES_MSG_ID_MGP_LM_UPDATE_CPE_CONTROL        = M_ARIES_MSG_ID_MGP_LM_UPDATE_CPE_CONTROL_0,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX_13             = M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_3,   /* Faulty CPI Reporting API */
  ARIES_MSG_ID_MGP_LM_XO_OFFSET_INFO            = M_ARIES_MSG_ID_MGP_LM_XO_OFFSET_INFO,
#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_2         = M_ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS_2,
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */
  SM_MSG_ID_LM_MGP_REPORT_XTRA_INTEGRITY        = M_SM_MSG_ID_LM_MGP_REPORT_XTRA_INTEGRITY_0,
  SM_MSG_ID_MGP_LM_REPORT_XTRA_INTEGRITY_INJ_DONE = M_SM_MSG_ID_MGP_LM_REPORT_XTRA_INTEGRITY_INJ_DONE_0,
  SM_MSG_ID_MGP_LM_HANDLE_DC_REPORT             = M_SM_MSG_ID_MGP_LM_HANDLE_DC_REPORT_0,
  MSG_ID_MGP_LM_REPORT_SVINFO                   = M_ARIES_MSG_ID_MGP_LM_REPORT_SVINFO_0,
  MSG_ID_MGP_LM_REPORT_SVMEASUSAGE              = M_ARIES_MSG_ID_MGP_LM_REPORT_SVMEASUSAGE_0,
  SM_MSG_ID_MGP_LM_REPORT_UNPROPAGATED_FIX      = M_SM_MSG_ID_MGP_LM_REPORT_UNPROPAGATED_FIX_66633,
  SM_MSG_ID_LM_LFM_CONFIG                       = M_SM_MSG_ID_LM_LFM_CONFIG_0,
  SM_MSG_ID_MGP_LM_REPORT_ME_METRICS            = M_ARIES_MSG_ID_REPORT_ME_METRICS_0,
  SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS          = M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_66631,
  ARIES_MSG_ID_MGP_LM_REPORT_GPS_MEAS = SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS,
  SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS          = M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_66632,
  SM_MSG_ID_MGP_LM_REQ_XTRA_NAVIC_EPH_ASSIST       = M_SM_MSG_ID_MGP_LM_REQ_XTRA_NAVIC_EPH_ASSIST_0,
  SM_MSG_ID_LM_MGP_REPORT_XTRA_NAVIC_ASSIST_DATA   = M_SM_MSG_ID_LM_MGP_REPORT_XTRA_NAVIC_ASSIST_DATA_0,
  SM_MSG_ID_LM_MGP_REPORT_XTRA_CONTROL_RECORD_DATA = M_SM_MSG_ID_LM_MGP_REPORT_XTRA_CONTROL_RECORD_DATA_0,
  SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_MEAS_LOG = M_SM_MSG_ID_MGP_LM_REPORT_1HZ_GPS_ML_MEAS_LOG_66630,
  SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_MEAS_LOG = M_SM_MSG_ID_MGP_LM_REPORT_NHZ_GPS_ML_MEAS_LOG_66629,
  MSG_ID_MGP_LM_REPORT_SV_POLY = M_MSG_ID_MGP_LM_REPORT_SV_POLY_66626,
  SM_MSG_ID_MGP_LATENCY_REPORT   = M_SM_MSG_ID_MGP_LM_LATENCY_REPORT_66604,
  ARIES_MSG_ID_MGP_LM_REPORT_FIX = M_ARIES_MSG_ID_MGP_LM_REPORT_FIX_66628,
  MSG_ID_MGP_LM_BATCH_POLY_REPORT = M_MSG_ID_MGP_LM_BATCH_POLY_REPORT_66627,
  MSG_ID_MGP_LM_GOODNESS_IND = M_MSG_ID_MGP_LM_GOODNESS_IND_66617,
  SM_MSG_ID_MGP_LM_SELF_TEST_RESULTS = M_SM_MSG_ID_MGP_LM_SELF_TEST_RESULTS_66618,
  SM_MSG_ID_MGP_LM_REPORT_JAMMING = M_SM_MSG_ID_MGP_LM_REPORT_JAMMING_66634,
  SM_MSG_ID_MGP_LM_TDM_EN_UPDATE = M_SM_MSG_ID_MGP_LM_TDM_EN_UPDATE_66635,
  /******************************************************************************
  When adding a new LM msg ID, you MUST insert any new items from the Master list,
  before your new item (and update the Master list). Follow the procedure below:

  1) Using the IPC definitions in Master branch as baseline,
     replace the enum m_lm_ipc_msg_id_type.
     Master Branch: //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

  2) In enum m_lm_ipc_msg_id_type, add a new message ID for the new IPC

  3) In enum lm_ipc_msg_id_type, create a mapping of the new IPC message
     with the message id from (2)

  In master IPC file sm_api.h at //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h
  Update enums m_lm_ipc_msg_id_type and lm_ipc_msg_id_type
  with the new IPC that you added.
  ******************************************************************************/
  C_SM_LM_IPC_MSG_END = M_C_SM_LM_IPC_MSG_END
} lm_ipc_msg_id_type;

/* TM IPC Message ID Enum definitions */
typedef enum
{
  M_TM_CORE_MSG_ID_DIAG_0 = TM_CORE_MSG_ID_START, /* Forward to Diag Module */
  M_TM_CORE_MSG_ID_SESS_REQ_API_0 = 0x1028,         /* IPC MSG for TM-LM Session Request API */
  M_TM_CORE_MSG_ID_SESS_UPDATE_API_0_0 = 0x1029,      /* IPC MSG for TM-LM Session Update API */
  M_TM_CORE_MSG_ID_SESS_INFO_API_0_0 = 0x1030,      /* IPC MSG for TM-LM Session Info API */
  M_TM_CORE_MSG_ID_PDAPI_0 = 0x1031,            /* IPC MSG for PD API */
  M_TM_CORE_PDCOMM_MSG_ID_BASE_0 = 0x1032,        /* IPC ID base for PD COMM */
  M_TM_CORE_PDCOMM_MSG_ID_WRITE_ACK_0 = M_TM_CORE_PDCOMM_MSG_ID_BASE_0,
  M_TM_CORE_PDCOMM_MSG_ID_READ_DATA_0 = 0x1033,
  M_TM_CORE_PDCOMM_MSG_ID_NOTIFY_0 = 0x1034,
  M_TM_CORE_PDCOMM_MSG_ID_IOCTL_ACK_0 = 0x1035,
  M_TM_CORE_PDCOMM_MSG_ID_HASH_RESP_0 = 0x1036,
  M_TM_CORE_PDCOMM_MSG_ID_HANDLE_0 = 0x1037,
  M_TM_CORE_PDCOMM_MSG_ID_SMS_ACK_REPORT_0 = 0x1038,
  M_TM_CORE_ON_DEMAND_CHARGER_CONNECTED_0 = 0x1039,    /* IPC from on demand charger poll */
  M_TM_CORE_ON_DEMAND_CHARGER_REMOVED_0 = 0x1040,    /* IPC from on demand charger poll */
  M_TM_CORE_ON_DEMAND_BATTERY_FULLY_CHARGED_0 = 0x1041,/* IPC from on demand charger poll */
  TM_CORE_CMD_NTFY = 0x1051,
  M_TM_CORE_CM_MSG_ID_CMD_NTFY_0 = TM_CORE_CMD_NTFY,
  M_TM_CORE_CM_MSG_ID_EVENT_NTFY_0 = 0x1052,
  M_TM_CORE_NMEA_MSG_ID_SIO_OPEN_CB_0 = 0x1053,
  M_TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_0 = 0x1054,
  M_TM_CORE_MSG_ID_MM_0 = 0x1055,
  M_TM_CORE_MSG_ID_RRC_0 = 0x1056,
  M_TM_CORE_MSG_ID_MC_CELLDB_UPDATE_0 = 0x1057,      /* IPC from MC updating Cell DB data base */
  M_TM_CORE_MSG_ID_L1_0 = 0x1058,            /* IPC from L1 */
  M_TM_CORE_MSG_ID_GPS_STATE_INFO_0 = 0x1059,
  M_TM_CORE_ON_DEMAND_MSG_ID_CHARGER_CONNECTED_0 = 0x105A,
  M_TM_CORE_ON_DEMAND_MSG_ID_CHARGER_NOT_CONNECTED_0 = 0x105B,
  M_TM_CORE_ON_DEMAND_MSG_ID_BATTERY_FULLY_CHARGED_0 = 0x105C,
  /* This id is now available for use with new XTRA-T implementation :  TLE sends CellDB to XTM */
  M_TM_CORE_MSG_ID_REPORT_CELL_DATA_0 = 0x105D,      /* IPC from MC sending segmented Cell DB data */
  M_TM_CORE_MSG_ID_LTE_L1_0 = 0x105E,         /* IPC from LTE L1 */
  TM_CORE_RR_NTFY = 0x1060,
  M_TM_CORE_RR_MSG_ID_EVENT_NTFY_0 = TM_CORE_RR_NTFY,
  M_TM_CORE_MSG_ID_SIM_INIT_COMPLETED_0 = 0x1061,
  TM_CORE_ODP_SESS_START = 0x1066,
  M_TM_CORE_MSG_ID_ODP_SESS_START_0 = TM_CORE_ODP_SESS_START,
  M_TM_CORE_MSG_ID_SIM_CARD_INSERTED_0 = 0x1067,
  M_TM_CORE_MSG_ID_SIM_SUBSCRIPTION_READY_0 = 0x1068,
  M_TM_CORE_MSG_ID_SIM_SESSION_CHANGED_0 = 0x1069,
  M_TM_CORE_MSG_ID_SIM_SESSION_CLOSED_0 = 0x106A,
  M_TM_CORE_MSG_ID_MMGSDI_CLIENT_REG_0 = 0x106B,
  M_TM_CORE_MSG_ID_MMGSDI_LBS_SRV_AVAILABLE_0 = 0x106C,
  M_TM_CORE_MSG_ID_MMGSDI_CDMA_SVC_INITIALIZED_0 = 0x106D,
  M_TM_CORE_MSG_ID_MMGSDI_CMD_READ_CDMA_SVC_TBL_0 = 0x106E,
  M_TM_CORE_MSG_ID_LSMP_GET_PARAM_0 = 0x106F,              /* IPC MSG ID for GET PARAM request from LSMP API */
  M_TM_CORE_MSG_ID_LSMP_SET_PARAM_0 = 0x1070,              /* IPC MSG ID for SET PARAM request from LSMP API */
  M_TM_CORE_MSG_ID_LSMP_SEND_PREM_SVC_IND_STATUS_0 = 0x1071,
  TM_CORE_SEC_CLNT_ACTIVATED = 0x1080,
  M_TM_CORE_SEC_MSG_ID_CLNT_ACTIVATED_0 = TM_CORE_SEC_CLNT_ACTIVATED,
  TM_CORE_XSPI_INJECTION_REQUEST = 0x1090,
  M_TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST_0 = TM_CORE_XSPI_INJECTION_REQUEST,
  M_TM_CORE_MSG_ID_PKCS15_EVT_0 = 0x10A0,
  /*****************************************************/
  /*********** GEN8 Message ID Range Start *************/
  /*****************************************************/
  M_TM_CORE_MSG_ID_SESS_UPDATE_API_0 = SM_GEN8_IPC_ID_START_RANGE, /* IPC MSG for TM-LM Session Update API */
  M_TM_CORE_MSG_ID_SESS_INFO_API_0 = SM_GEN8_IPC_ID_START_RANGE + 1, /* IPC MSG for TM-LM Session Info API */
  M_TM_CORE_MSG_ID_XTM_CLIENT_TOKEN_0 = SM_GEN8_IPC_ID_START_RANGE + 2, /* IPC MSG for XTM (xtrat) - TM to report success of NV write*/
  M_TM_CORE_MSG_ID_XTM_SESSION_CONTROL_0 = SM_GEN8_IPC_ID_START_RANGE + 3,
  M_TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_2_0 = SM_GEN8_IPC_ID_START_RANGE + 4, /* SIO Close IPC */
  M_TM_CORE_NMEA_MSG_ID_DIAG_SIO_CLOSE_CB_2_0 = SM_GEN8_IPC_ID_START_RANGE + 5, /* SIO Diag Close IPC */
  /* TM-Core Geofence MSG */
  M_TM_CORE_GM_MSG_ID_BASE_0 = SM_GEN8_IPC_ID_START_RANGE + 6,
  M_TM_CORE_GM_MSG_ID_ADD_RESPONSE_0 = M_TM_CORE_GM_MSG_ID_BASE_0,
  M_TM_CORE_GM_MSG_ID_EDIT_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 7,
  M_TM_CORE_GM_MSG_ID_DELETE_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 8,
  M_TM_CORE_GM_MSG_ID_PURGE_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 9,
  M_TM_CORE_GM_MSG_ID_BREACH_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 10,
  M_TM_CORE_GM_MSG_ID_POS_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 11,
  M_TM_CORE_GM_MSG_ID_CLIENT_BLOB_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 12,
  M_TM_CORE_GM_MSG_ID_CLIENT_GEOFENCE_HANDELS_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 13,
  M_TM_CORE_GM_MSG_ID_GEOFENCE_HANDELS_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 14,
  M_TM_CORE_GM_MSG_ID_QUERY_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 15,
  M_TM_CORE_GM_MSG_ID_UNSOL_EVENT_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 16,
  M_TM_CORE_GM_MSG_ID_PRIO_POS_REQ_INDICATION_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 17,
  M_TM_CORE_GM_MSG_ID_SET_ENGINE_CONFIG_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 18,
  M_TM_CORE_GM_MSG_ID_ABORT_POS_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 19,
  M_TM_CORE_GM_MSG_ID_BATCH_BREACH_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 20,
  M_TM_CORE_GM_MSG_ID_PROXIMITY_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 21,

  M_TM_CORE_GM_MSG_ID_REPLACE_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 22,
  M_TM_CORE_GM_MSG_ID_BATCH_DWELL_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 23,
  M_TM_CORE_GM_MSG_ID_TZ_REQ_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 24,
  M_TM_CORE_GM_MSG_ID_SET_CLIENT_CONFIG_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 25,
  M_TM_CORE_GM_MSG_ID_WIFI_SIGNATURE_CHANGE_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 26,
  M_TM_CORE_GM_MSG_ID_CTXT_CHANGE_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 27,
  M_TM_CORE_GM_MSG_ID_LAST_0 = SM_GEN8_IPC_ID_START_RANGE + 50,
  M_TM_CORE_MGP_WBIQ_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 51,

  M_TM_CORE_SM_LOC_LTE_OTDOA_MEASUREMENTS_0 = SM_GEN8_IPC_ID_START_RANGE + 52,

  /*RRC cell info update*/
  M_TM_CORE_MSG_ID_RRC_CELL_INFO_DCH_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 53,

  /* Fast TCal Data Report Messages */
  M_TM_CORE_MSG_ID_FTCAL_TT_DATA_0 = SM_GEN8_IPC_ID_START_RANGE + 54,
  M_TM_CORE_MSG_ID_FTCAL_PPM_DATA_0 = SM_GEN8_IPC_ID_START_RANGE + 55,

  /* Fast TCal Enable/Disable Request */
  M_TM_CORE_MSG_ID_FTCAL_ENABLE_0 = SM_GEN8_IPC_ID_START_RANGE + 56,
  M_TM_CORE_MSG_ID_FTCAL_DISABLE_0 = SM_GEN8_IPC_ID_START_RANGE + 57,

  /* Engine Error Recovery Report */
  M_TM_CORE_MSG_ID_ENGINE_ERROR_RECOVERY_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 58,

  /* Griffon App Track Test Message (to be removed shortly) */
  M_TM_CORE_DUMMY_GRIFFON_MSG_0 = SM_GEN8_IPC_ID_START_RANGE + 59,

  /*Reset Location Service Done*/
  M_TM_CORE_MSG_ID_RESET_LOCATION_SERVICE_DONE_0 = SM_GEN8_IPC_ID_START_RANGE + 60,

  /* Best available position Report */
  M_TM_CORE_MSG_ID_BEST_AVAIL_POS_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 61,

  /* Engine DPO status Report */
  M_TM_CORE_MSG_ID_ENGINE_DPO_STATUS_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 62,

  /* LPP Control Plane Protocol Related */
  M_TM_CORE_MSG_ID_NAS_EVENT_BASE_0 = SM_GEN8_IPC_ID_START_RANGE + 63,

  M_TM_CORE_MSG_ID_NAS_FAILURE_EVENT_0 = SM_GEN8_IPC_ID_START_RANGE + 64,
  M_TM_CORE_MSG_ID_NAS_EVENT_LAST_0 = SM_GEN8_IPC_ID_START_RANGE + 69,

  M_TM_CORE_GNSS_LPP_ECID_MEASUREMENTS_0 = SM_GEN8_IPC_ID_START_RANGE + 70,

  /* Request to retrieve IMSI */
  M_TM_CORE_MSG_ID_RETRIEVE_IMSI_0 = SM_GEN8_IPC_ID_START_RANGE + 71,

  /* TM API registration MSG ID */
  M_TM_CORE_MSG_ID_API_REGISTRATION_0 = SM_GEN8_IPC_ID_START_RANGE + 72,

  /*Wifi Fix Response*/
  M_TM_CORE_MSG_ID_WIFI_FIX_RESP_0 = SM_GEN8_IPC_ID_START_RANGE + 73,

  /* TM API to receive LPPM Status Report  */
  M_TM_CORE_MSG_ID_LPPM_STATUS_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 75,

  /* TM IPC to re-read carrier-specific NV  */
  M_TM_CORE_MSG_ID_REFRESH_CARRIER_SPECIFIC_NV_0 = SM_GEN8_IPC_ID_START_RANGE + 76,

  M_TM_CORE_MSG_ID_LPPM_STATUS_0 = SM_GEN8_IPC_ID_START_RANGE + 77,
  M_TM_CORE_MSG_ID_PED_STATUS_0 = SM_GEN8_IPC_ID_START_RANGE + 78,
  M_TM_CORE_MSG_ID_GNSS_QUAL_IND_0 = SM_GEN8_IPC_ID_START_RANGE + 79,
  M_TM_CORE_MSG_ID_PED_DEV_CTX_IND_0 = SM_GEN8_IPC_ID_START_RANGE + 80,
  M_TM_CORE_MSG_ID_PED_ALIGN_AVAIL_IND_0 = SM_GEN8_IPC_ID_START_RANGE + 81,
  M_TM_CORE_MSG_ID_MGP_STATE_CHANGE_IND_TECHSEL_0 = SM_GEN8_IPC_ID_START_RANGE + 82,
  M_TM_CORE_MSG_ID_PED_SENSOR_ASSIST_AVAIL_IND_0 = SM_GEN8_IPC_ID_START_RANGE + 83,

  /* TM IPC to process GERA test CMD  */
  M_TM_CORE_MSG_ID_GERA_TEST_CMD_0 = SM_GEN8_IPC_ID_START_RANGE + 84,

  /* TM API to set AP-MP TTR Control settings */
  M_TM_CORE_MSG_ID_AP_MP_CONFIG_VALUE_0 = SM_GEN8_IPC_ID_START_RANGE + 85,

  /* TM API to set Leap Second Info */
  M_TM_CORE_MSG_ID_LEAP_SEC_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 86,

  /* TM API to receive the result of NAV CONFIG Change command */
  M_TM_CORE_MGP_NAV_CONFIG_CHANGE_RESP_0 = SM_GEN8_IPC_ID_START_RANGE + 87,

  /* TM API to issue NV operation status*/
  M_TM_CORE_MSG_ID_NV_OP_COMPLETE_0 = SM_GEN8_IPC_ID_START_RANGE + 88,

  /*TM API to handle sensor Baro pressure info */
  M_TM_CORE_MSG_ID_SENSOR_BARO_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 89,

  /* TM API to receive NMEA string, which will be sent to HLOS for debugging purpose */
  M_TM_CORE_MSG_ID_NMEA_DEBUG_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 90,

  /* TM API to handle XO offset info from MC */
  M_TM_CORE_MSG_ID_XO_OFFSET_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 91,

  /* TM API to send crowd source manager control request to Fusion CSM module */
  M_TM_CORE_MSG_ID_FUSION_CSM_CONTROL_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 92,

  /*Wifi Scan for Techsel*/
  M_TM_CORE_MSG_ID_WIFI_SCAN_TECHSEL_0 = SM_GEN8_IPC_ID_START_RANGE + 93,
  /*Lowi Error for Techsel*/
  M_TM_CORE_MSG_ID_LOWI_ERROR_TECHSEL_0 = SM_GEN8_IPC_ID_START_RANGE + 94,

  /* TM API for IQF2 GDT messages */
  M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_BEGIN_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 95,
  M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_OPEN_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 96,
  M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_CLOSE_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 97,
  M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_END_RESPONSE_0 = SM_GEN8_IPC_ID_START_RANGE + 98,
  M_TM_CORE_CSM_UL_MSG_ID_GDT_SEND_ACK_0 = SM_GEN8_IPC_ID_START_RANGE + 99,

  M_TM_CORE_CSM_SET_EXT_POWER_CONFIG_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 100,

  /* TM API to send crowd source manager request to read crowd sourced data */
  M_TM_CORE_MSG_ID_FUSION_CSM_READ_CS_DATA_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 101,

  /* TM API to receive varying NMEA debug string, which will be sent to HLOS for debugging purpose */
  M_TM_CORE_MSG_ID_NMEA_VAR_LEN_DEBUG_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 102,

  /*TM API to handle sensor info */
  M_TM_CORE_MSG_ID_SENSOR_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 103,

  /*Wifi Ranging Scan for Techsel*/
  M_TM_CORE_MSG_ID_WIFI_RANGING_SCAN_TECHSEL_0 = SM_GEN8_IPC_ID_START_RANGE + 104,

  /*GNSS LE Fix*/
  M_TM_CORE_MSG_ID_GNSS_LE_FIX_0 = SM_GEN8_IPC_ID_START_RANGE + 105,

  /*Sensor LE Status*/
  M_TM_CORE_MSG_ID_SENSOR_LE_STATUS_0 = SM_GEN8_IPC_ID_START_RANGE + 106,

  /*Sensor LE Relative Displacement*/
  M_TM_CORE_MSG_ID_SENSOR_LE_REL_DISP_0 = SM_GEN8_IPC_ID_START_RANGE + 107,

  /* TM API to receive AS bit from MC - DEPRECATED */
  /* TM_CORE_MSG_ID_SPOOFING_INCONSISTENCY shall be used to report AS Bit inconsistency */
  M_TM_CORE_MSG_ID_GPS_AS_BIT_DEPRECATED_0 = SM_GEN8_IPC_ID_START_RANGE + 108,

  /* TM API to receive Nav IQ Test Capabilities from MC */
  M_TM_CORE_MSG_ID_NAV_IQ_TEST_MASK_0 = SM_GEN8_IPC_ID_START_RANGE + 109,

  /* TM API to receive RF Dev Multichannel Track Test Capabilities from MC */
  M_TM_CORE_MSG_ID_RFDEV_TRACK_TEST_MASK_0 = SM_GEN8_IPC_ID_START_RANGE + 110,

  /* TM API to handle AMPM disable from MC */
  M_TM_CORE_MSG_ID_DISABLE_APMP_0 = SM_GEN8_IPC_ID_START_RANGE + 111,

  /* TM API to report GNSS Persistent SV Mask */
  M_TM_CORE_MSG_ID_PERSISTENT_SV_MASK_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 112,

  /* TM API to receive GNSS constellation config from MGP */
  M_TM_CORE_MSG_ID_CONSTELLATION_CONTROL_0 = SM_GEN8_IPC_ID_START_RANGE + 113,

  /* TM API to report GNSS Persistent SV Mask */
  M_TM_CORE_MSG_ID_PERSISTENT_SV_MASK_TO_FILTER_0 = SM_GEN8_IPC_ID_START_RANGE + 114,

  /* TM API to RC ON Threshold Info */
  M_TM_CORE_MSG_ID_RC_ON_THRESHOLD_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 115,

  M_TM_CORE_MSG_ID_SPOOFING_INCONSISTENCY_0 = SM_GEN8_IPC_ID_START_RANGE + 116,

  /* TM API to receive Prescribed Dwell Results for the PFA test */
  M_TM_CORE_MGP_PRESC_DWELL_RESULTS_0 = SM_GEN8_IPC_ID_START_RANGE + 117,

  /* TM API to receive NV/Cass config from Location Feature Manager*/
  M_TM_CORE_MSG_ID_LFM_CONF_INFO_0 = SM_GEN8_IPC_ID_START_RANGE + 118,

  /* TM API to receive Serving System notification from Data Services */
  M_TM_CORE_DS_MSG_ID_EVENT_NOTIFY_0 = SM_GEN8_IPC_ID_START_RANGE + 119,

  /* TM API to read new IMSI when subscription ready event is received */
  M_TM_CORE_READ_NEW_IMSI_0 = SM_GEN8_IPC_ID_START_RANGE + 120,

  /* TM API to send GNSS Time Conversion Parameters to client */
  TM_CORE_MSG_ID_CLOCK_CONV_PARAM_REPORT_0 = SM_GEN8_IPC_ID_START_RANGE + 121,

  /* TM API to start/stop the constrained TUNC logic */
  M_TM_CORE_MSG_ID_START_CONSTRAINED_TUNC = SM_GEN8_IPC_ID_START_RANGE + 122,
  M_TM_CORE_MSG_ID_STOP_CONSTRAINED_TUNC  = SM_GEN8_IPC_ID_START_RANGE + 123,

  /* TM API to send DGNSS corrections with the fix report and measurements. */
  M_TM_CORE_MSG_ID_SESS_UPDATE_API_1       = SM_GEN8_IPC_ID_START_RANGE + 124,
  
  M_TM_CORE_MSG_ID_DC_REPORT_INFO = SM_GEN8_IPC_ID_START_RANGE + 125,

  /* TM API to allow LTE to force GPS receiver off to reset the QLink */
  M_TM_CORE_MSG_ID_FORCE_RCVR_OFF_FROM_LTE_0  = SM_GEN8_IPC_ID_START_RANGE + 126,

  /* TM API to handle the navic assist ephemeris download request */
  M_TM_CORE_MSG_ID_XTRA_NAVIC_ASSIST_DOWNLOAD_REQ_0 = SM_GEN8_IPC_ID_START_RANGE + 127, 

  /* TM API to receive Mag Cal State from PE */
  M_TM_CORE_MSG_ID_PDR_MAG_CAL_STATE_0 = SM_GEN8_IPC_ID_START_RANGE + 132,

  /******************************************************************************
  When adding a new TM msg ID, you MUST insert any new items from the Master list,
  before your new item (and update the Master list). Follow the procedure below:

  1) Using the IPC definitions in Master branch as baseline,
  replace the enum m_tm_ipc_msg_id_type.
  Master Branch: //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

  2) In enum m_tm_ipc_msg_id_type, add a new message ID for the new IPC

  3) In enum tm_ipc_msg_id_type, create a mapping of the new IPC message
  with the message id from (2)

  In master IPC file sm_api.h at //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h
  Update enums m_tm_ipc_msg_id_type and tm_ipc_msg_id_type
  with the new IPC that you added.
  ******************************************************************************/
  M_TM_CORE_MSG_ID_LAST
}m_tm_ipc_msg_id_type;

/* TM Message ID to IPC mapping */
typedef enum
{
  TM_CORE_MSG_ID_DIAG = M_TM_CORE_MSG_ID_DIAG_0, /* Forward to Diag Module */
  TM_CORE_MSG_ID_SESS_REQ_API = M_TM_CORE_MSG_ID_SESS_REQ_API_0,  /* IPC MSG for TM-LM Session Request API */
  TM_CORE_MSG_ID_SESS_UPDATE_API_0 = M_TM_CORE_MSG_ID_SESS_UPDATE_API_0_0,
  TM_CORE_MSG_ID_SESS_INFO_API_0 = M_TM_CORE_MSG_ID_SESS_INFO_API_0_0,
  TM_CORE_MSG_ID_PDAPI = M_TM_CORE_MSG_ID_PDAPI_0,
  TM_CORE_PDCOMM_MSG_ID_BASE = M_TM_CORE_PDCOMM_MSG_ID_BASE_0,
  TM_CORE_PDCOMM_MSG_ID_WRITE_ACK = M_TM_CORE_PDCOMM_MSG_ID_WRITE_ACK_0,
  TM_CORE_PDCOMM_MSG_ID_READ_DATA = M_TM_CORE_PDCOMM_MSG_ID_READ_DATA_0,
  TM_CORE_PDCOMM_MSG_ID_NOTIFY = M_TM_CORE_PDCOMM_MSG_ID_NOTIFY_0,
  TM_CORE_PDCOMM_MSG_ID_IOCTL_ACK = M_TM_CORE_PDCOMM_MSG_ID_IOCTL_ACK_0,
  TM_CORE_PDCOMM_MSG_ID_HASH_RESP = M_TM_CORE_PDCOMM_MSG_ID_HASH_RESP_0,
  TM_CORE_PDCOMM_MSG_ID_HANDLE = M_TM_CORE_PDCOMM_MSG_ID_HANDLE_0,
  TM_CORE_PDCOMM_MSG_ID_SMS_ACK_REPORT = M_TM_CORE_PDCOMM_MSG_ID_SMS_ACK_REPORT_0,
  TM_CORE_ON_DEMAND_CHARGER_CONNECTED = M_TM_CORE_ON_DEMAND_CHARGER_CONNECTED_0,
  TM_CORE_ON_DEMAND_CHARGER_REMOVED = M_TM_CORE_ON_DEMAND_CHARGER_REMOVED_0,
  TM_CORE_ON_DEMAND_BATTERY_FULLY_CHARGED = M_TM_CORE_ON_DEMAND_BATTERY_FULLY_CHARGED_0,
  TM_CORE_CM_MSG_ID_CMD_NTFY = M_TM_CORE_CM_MSG_ID_CMD_NTFY_0,
  TM_CORE_CM_MSG_ID_EVENT_NTFY = M_TM_CORE_CM_MSG_ID_EVENT_NTFY_0,
  TM_CORE_NMEA_MSG_ID_SIO_OPEN_CB = M_TM_CORE_NMEA_MSG_ID_SIO_OPEN_CB_0,
  TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB = M_TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_0,
  TM_CORE_MSG_ID_MM = M_TM_CORE_MSG_ID_MM_0,
  TM_CORE_MSG_ID_RRC = M_TM_CORE_MSG_ID_RRC_0,
  TM_CORE_MSG_ID_MC_CELLDB_UPDATE = M_TM_CORE_MSG_ID_MC_CELLDB_UPDATE_0,
  TM_CORE_MSG_ID_L1 = M_TM_CORE_MSG_ID_L1_0,
  TM_CORE_MSG_ID_GPS_STATE_INFO = M_TM_CORE_MSG_ID_GPS_STATE_INFO_0,
  TM_CORE_ON_DEMAND_MSG_ID_CHARGER_CONNECTED = M_TM_CORE_ON_DEMAND_MSG_ID_CHARGER_CONNECTED_0,
  TM_CORE_ON_DEMAND_MSG_ID_CHARGER_NOT_CONNECTED = M_TM_CORE_ON_DEMAND_MSG_ID_CHARGER_NOT_CONNECTED_0,
  TM_CORE_ON_DEMAND_MSG_ID_BATTERY_FULLY_CHARGED = M_TM_CORE_ON_DEMAND_MSG_ID_BATTERY_FULLY_CHARGED_0,
  TM_CORE_MSG_ID_REPORT_CELL_DATA = M_TM_CORE_MSG_ID_REPORT_CELL_DATA_0,
  TM_CORE_MSG_ID_LTE_L1 = M_TM_CORE_MSG_ID_LTE_L1_0,
  TM_CORE_RR_MSG_ID_EVENT_NTFY = M_TM_CORE_RR_MSG_ID_EVENT_NTFY_0,
  TM_CORE_MSG_ID_SIM_INIT_COMPLETED = M_TM_CORE_MSG_ID_SIM_INIT_COMPLETED_0,
  TM_CORE_MSG_ID_ODP_SESS_START = M_TM_CORE_MSG_ID_ODP_SESS_START_0,
  TM_CORE_MSG_ID_SIM_CARD_INSERTED = M_TM_CORE_MSG_ID_SIM_CARD_INSERTED_0,
  TM_CORE_MSG_ID_SIM_SUBSCRIPTION_READY = M_TM_CORE_MSG_ID_SIM_SUBSCRIPTION_READY_0,
  TM_CORE_MSG_ID_SIM_SESSION_CHANGED = M_TM_CORE_MSG_ID_SIM_SESSION_CHANGED_0,
  TM_CORE_MSG_ID_SIM_SESSION_CLOSED = M_TM_CORE_MSG_ID_SIM_SESSION_CLOSED_0,
  TM_CORE_MSG_ID_MMGSDI_CLIENT_REG = M_TM_CORE_MSG_ID_MMGSDI_CLIENT_REG_0,
  TM_CORE_MSG_ID_MMGSDI_LBS_SRV_AVAILABLE = M_TM_CORE_MSG_ID_MMGSDI_LBS_SRV_AVAILABLE_0,
  TM_CORE_MSG_ID_MMGSDI_CDMA_SVC_INITIALIZED = M_TM_CORE_MSG_ID_MMGSDI_CDMA_SVC_INITIALIZED_0,
  TM_CORE_MSG_ID_MMGSDI_CMD_READ_CDMA_SVC_TBL = M_TM_CORE_MSG_ID_MMGSDI_CMD_READ_CDMA_SVC_TBL_0,
  TM_CORE_MSG_ID_LSMP_GET_PARAM = M_TM_CORE_MSG_ID_LSMP_GET_PARAM_0,  /* IPC MSG ID for GET PARAM request from LSMP API */
  TM_CORE_MSG_ID_LSMP_SET_PARAM = M_TM_CORE_MSG_ID_LSMP_SET_PARAM_0,  /* IPC MSG ID for SET PARAM request from LSMP API */
  TM_CORE_MSG_ID_LSMP_SEND_PREM_SVC_IND_STATUS = M_TM_CORE_MSG_ID_LSMP_SEND_PREM_SVC_IND_STATUS_0,
  TM_CORE_SEC_MSG_ID_CLNT_ACTIVATED = M_TM_CORE_SEC_MSG_ID_CLNT_ACTIVATED_0,
  TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST = M_TM_CORE_MSG_ID_XSPI_INJECTION_REQUEST_0,
  TM_CORE_MSG_ID_PKCS15_EVT = M_TM_CORE_MSG_ID_PKCS15_EVT_0,
  TM_CORE_MSG_ID_SESS_UPDATE_API = M_TM_CORE_MSG_ID_SESS_UPDATE_API_1, /* IPC MSG for TM-LM Session Update API */
  TM_CORE_MSG_ID_SESS_INFO_API = M_TM_CORE_MSG_ID_SESS_INFO_API_0,   /* IPC MSG for TM-LM Session Info API */
  TM_CORE_MSG_ID_XTM_CLIENT_TOKEN = M_TM_CORE_MSG_ID_XTM_CLIENT_TOKEN_0,/* IPC MSG for XTM (xtrat) - TM to report success of NV write*/
  TM_CORE_MSG_ID_XTM_SESSION_CONTROL = M_TM_CORE_MSG_ID_XTM_SESSION_CONTROL_0,
  TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_2 = M_TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_2_0, /* SIO Close IPC */
  TM_CORE_NMEA_MSG_ID_DIAG_SIO_CLOSE_CB_2 = M_TM_CORE_NMEA_MSG_ID_DIAG_SIO_CLOSE_CB_2_0, /* SIO Diag Close IPC */
  TM_CORE_GM_MSG_ID_BASE = M_TM_CORE_GM_MSG_ID_BASE_0,
  TM_CORE_GM_MSG_ID_ADD_RESPONSE = M_TM_CORE_GM_MSG_ID_ADD_RESPONSE_0,
  TM_CORE_GM_MSG_ID_EDIT_NOTIFY = M_TM_CORE_GM_MSG_ID_EDIT_NOTIFY_0,
  TM_CORE_GM_MSG_ID_DELETE_RESPONSE = M_TM_CORE_GM_MSG_ID_DELETE_RESPONSE_0,
  TM_CORE_GM_MSG_ID_PURGE_RESPONSE = M_TM_CORE_GM_MSG_ID_PURGE_RESPONSE_0,
  TM_CORE_GM_MSG_ID_BREACH_NOTIFY = M_TM_CORE_GM_MSG_ID_BREACH_NOTIFY_0,
  TM_CORE_GM_MSG_ID_POS_REQ = M_TM_CORE_GM_MSG_ID_POS_REQ_0,
  TM_CORE_GM_MSG_ID_CLIENT_BLOB_NOTIFY = M_TM_CORE_GM_MSG_ID_CLIENT_BLOB_NOTIFY_0,
  TM_CORE_GM_MSG_ID_CLIENT_GEOFENCE_HANDELS_NOTIFY = M_TM_CORE_GM_MSG_ID_CLIENT_GEOFENCE_HANDELS_NOTIFY_0,
  TM_CORE_GM_MSG_ID_GEOFENCE_HANDELS_NOTIFY = M_TM_CORE_GM_MSG_ID_GEOFENCE_HANDELS_NOTIFY_0,
  TM_CORE_GM_MSG_ID_QUERY_NOTIFY = M_TM_CORE_GM_MSG_ID_QUERY_NOTIFY_0,
  TM_CORE_GM_MSG_ID_UNSOL_EVENT_NOTIFY = M_TM_CORE_GM_MSG_ID_UNSOL_EVENT_NOTIFY_0,
  TM_CORE_GM_MSG_ID_PRIO_POS_REQ_INDICATION_NOTIFY = M_TM_CORE_GM_MSG_ID_PRIO_POS_REQ_INDICATION_NOTIFY_0,
  TM_CORE_GM_MSG_ID_SET_ENGINE_CONFIG_NOTIFY = M_TM_CORE_GM_MSG_ID_SET_ENGINE_CONFIG_NOTIFY_0,
  TM_CORE_GM_MSG_ID_ABORT_POS_REQ = M_TM_CORE_GM_MSG_ID_ABORT_POS_REQ_0,
  TM_CORE_GM_MSG_ID_BATCH_BREACH_NOTIFY = M_TM_CORE_GM_MSG_ID_BATCH_BREACH_NOTIFY_0,
  TM_CORE_GM_MSG_ID_PROXIMITY_NOTIFY = M_TM_CORE_GM_MSG_ID_PROXIMITY_NOTIFY_0,
  TM_CORE_GM_MSG_ID_REPLACE_NOTIFY = M_TM_CORE_GM_MSG_ID_REPLACE_NOTIFY_0,
  TM_CORE_GM_MSG_ID_BATCH_DWELL_NOTIFY = M_TM_CORE_GM_MSG_ID_BATCH_DWELL_NOTIFY_0,
  TM_CORE_GM_MSG_ID_TZ_REQ_NOTIFY = M_TM_CORE_GM_MSG_ID_TZ_REQ_NOTIFY_0,
  TM_CORE_GM_MSG_ID_SET_CLIENT_CONFIG_NOTIFY = M_TM_CORE_GM_MSG_ID_SET_CLIENT_CONFIG_NOTIFY_0,
  TM_CORE_GM_MSG_ID_WIFI_SIGNATURE_CHANGE_NOTIFY = M_TM_CORE_GM_MSG_ID_WIFI_SIGNATURE_CHANGE_NOTIFY_0,
  TM_CORE_GM_MSG_ID_CTXT_CHANGE_NOTIFY = M_TM_CORE_GM_MSG_ID_CTXT_CHANGE_NOTIFY_0,
  TM_CORE_GM_MSG_ID_LAST = M_TM_CORE_GM_MSG_ID_LAST_0,
  TM_CORE_MGP_WBIQ_INFO = M_TM_CORE_MGP_WBIQ_INFO_0,
  TM_CORE_SM_LOC_LTE_OTDOA_MEASUREMENTS = M_TM_CORE_SM_LOC_LTE_OTDOA_MEASUREMENTS_0,
  TM_CORE_MSG_ID_RRC_CELL_INFO_DCH_REPORT = M_TM_CORE_MSG_ID_RRC_CELL_INFO_DCH_REPORT_0, /*RRC cell info update*/
  TM_CORE_MSG_ID_FTCAL_TT_DATA = M_TM_CORE_MSG_ID_FTCAL_TT_DATA_0, /* Fast TCal Data Report Messages */
  TM_CORE_MSG_ID_FTCAL_PPM_DATA = M_TM_CORE_MSG_ID_FTCAL_PPM_DATA_0,
  TM_CORE_MSG_ID_FTCAL_ENABLE = M_TM_CORE_MSG_ID_FTCAL_ENABLE_0, /* Fast TCal Enable/Disable Request */
  TM_CORE_MSG_ID_FTCAL_DISABLE = M_TM_CORE_MSG_ID_FTCAL_DISABLE_0,
  TM_CORE_MSG_ID_ENGINE_ERROR_RECOVERY_REPORT = M_TM_CORE_MSG_ID_ENGINE_ERROR_RECOVERY_REPORT_0, /* Engine Error Recovery Report */
  TM_CORE_DUMMY_GRIFFON_MSG = M_TM_CORE_DUMMY_GRIFFON_MSG_0, /* Griffon App Track Test Message (to be removed shortly) */
  TM_CORE_MSG_ID_RESET_LOCATION_SERVICE_DONE = M_TM_CORE_MSG_ID_RESET_LOCATION_SERVICE_DONE_0, /*Reset Location Service Done*/
  TM_CORE_MSG_ID_BEST_AVAIL_POS_REPORT = M_TM_CORE_MSG_ID_BEST_AVAIL_POS_REPORT_0, /* Best available position Report */
  TM_CORE_MSG_ID_ENGINE_DPO_STATUS_REPORT = M_TM_CORE_MSG_ID_ENGINE_DPO_STATUS_REPORT_0, /* Engine DPO status Report */
  TM_CORE_MSG_ID_NAS_EVENT_BASE = M_TM_CORE_MSG_ID_NAS_EVENT_BASE_0, /* LPP Control Plane Protocol Related */
  TM_CORE_MSG_ID_NAS_DL_DATA_EVENT = TM_CORE_MSG_ID_NAS_EVENT_BASE,
  TM_CORE_MSG_ID_NAS_FAILURE_EVENT = M_TM_CORE_MSG_ID_NAS_FAILURE_EVENT_0,
  TM_CORE_MSG_ID_NAS_EVENT_LAST = M_TM_CORE_MSG_ID_NAS_EVENT_LAST_0,
  TM_CORE_GNSS_LPP_ECID_MEASUREMENTS = M_TM_CORE_GNSS_LPP_ECID_MEASUREMENTS_0,
  TM_CORE_MSG_ID_RETRIEVE_IMSI = M_TM_CORE_MSG_ID_RETRIEVE_IMSI_0, /* Request to retrieve IMSI */
  TM_CORE_MSG_ID_API_REGISTRATION = M_TM_CORE_MSG_ID_API_REGISTRATION_0, /* TM API registration MSG ID */
  TM_CORE_MSG_ID_WIFI_FIX_RESP = M_TM_CORE_MSG_ID_WIFI_FIX_RESP_0,
  /* TM API to receive LPPM Status Report  */
  TM_CORE_MSG_ID_LPPM_STATUS_REPORT = M_TM_CORE_MSG_ID_LPPM_STATUS_REPORT_0,


  /* TM IPC to re-read carrier-specific NV  */
  TM_CORE_MSG_ID_REFRESH_CARRIER_SPECIFIC_NV = M_TM_CORE_MSG_ID_REFRESH_CARRIER_SPECIFIC_NV_0,

  TM_CORE_MSG_ID_LPPM_STATUS = M_TM_CORE_MSG_ID_LPPM_STATUS_0,
  TM_CORE_MSG_ID_PED_STATUS = M_TM_CORE_MSG_ID_PED_STATUS_0,
  TM_CORE_MSG_ID_GNSS_QUAL_IND = M_TM_CORE_MSG_ID_GNSS_QUAL_IND_0,
  TM_CORE_MSG_ID_PED_DEV_CTX_IND = M_TM_CORE_MSG_ID_PED_DEV_CTX_IND_0,
  TM_CORE_MSG_ID_PED_ALIGN_AVAIL_IND = M_TM_CORE_MSG_ID_PED_ALIGN_AVAIL_IND_0,
  TM_CORE_MSG_ID_MGP_STATE_CHANGE_IND_TECHSEL = M_TM_CORE_MSG_ID_MGP_STATE_CHANGE_IND_TECHSEL_0,
  TM_CORE_MSG_ID_PED_SENSOR_ASSIST_AVAIL_IND = M_TM_CORE_MSG_ID_PED_SENSOR_ASSIST_AVAIL_IND_0,

  /* TM IPC to process GERA test CMD  */
  TM_CORE_MSG_ID_GERA_TEST_CMD = M_TM_CORE_MSG_ID_GERA_TEST_CMD_0,

  /* TM API to set AP-MP TTR Control settings */
  TM_CORE_MSG_ID_AP_MP_CONFIG_VALUE = M_TM_CORE_MSG_ID_AP_MP_CONFIG_VALUE_0,

  /* TM API to set Leap Second Info */
  TM_CORE_MSG_ID_LEAP_SEC_INFO = M_TM_CORE_MSG_ID_LEAP_SEC_INFO_0,

  /* TM API to receive the result of NAV CONFIG Change command */
  TM_CORE_MGP_NAV_CONFIG_CHANGE_RESP = M_TM_CORE_MGP_NAV_CONFIG_CHANGE_RESP_0,

  /* TM API to issue NV operation status*/
  TM_CORE_MSG_ID_NV_OP_COMPLETE = M_TM_CORE_MSG_ID_NV_OP_COMPLETE_0,

  /*TM API to handle sensor Baro pressure info */
  TM_CORE_MSG_ID_SENSOR_BARO_INFO = M_TM_CORE_MSG_ID_SENSOR_BARO_INFO_0,

  /* TM API to receive NMEA string, which will be sent to HLOS for debugging purpose */
  TM_CORE_MSG_ID_NMEA_DEBUG_INFO = M_TM_CORE_MSG_ID_NMEA_DEBUG_INFO_0,

  /* TM API to handle XO offset info from MC */
  TM_CORE_MSG_ID_XO_OFFSET_INFO = M_TM_CORE_MSG_ID_XO_OFFSET_INFO_0,

  /* TM API to send crowd source manager control request to Fusion CSM module */
  TM_CORE_MSG_ID_FUSION_CSM_CONTROL_REQ = M_TM_CORE_MSG_ID_FUSION_CSM_CONTROL_REQ_0,

  /*Wifi Scan for Techsel*/
  TM_CORE_MSG_ID_WIFI_SCAN_TECHSEL = M_TM_CORE_MSG_ID_WIFI_SCAN_TECHSEL_0,
  /*Lowi Error for Techsel*/
  TM_CORE_MSG_ID_LOWI_ERROR_TECHSEL = M_TM_CORE_MSG_ID_LOWI_ERROR_TECHSEL_0,

  /* TM API for IQF2 GDT messages */
  TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_BEGIN_RESPONSE = M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_BEGIN_RESPONSE_0,
  TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_OPEN_RESPONSE = M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_OPEN_RESPONSE_0,
  TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_CLOSE_RESPONSE = M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_CLOSE_RESPONSE_0,
  TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_END_RESPONSE = M_TM_CORE_CSM_UL_MSG_ID_GDT_UPLD_END_RESPONSE_0,
  TM_CORE_CSM_UL_MSG_ID_GDT_SEND_ACK = M_TM_CORE_CSM_UL_MSG_ID_GDT_SEND_ACK_0,

  TM_CORE_CSM_SET_EXT_POWER_CONFIG_REQ = M_TM_CORE_CSM_SET_EXT_POWER_CONFIG_REQ_0,

  /* TM API to send crowd source manager request to read crowd sourced data */
  TM_CORE_MSG_ID_FUSION_CSM_READ_CS_DATA_REQ = M_TM_CORE_MSG_ID_FUSION_CSM_READ_CS_DATA_REQ_0,

  /* TM API to receive varying NMEA debug string, which will be sent to HLOS for debugging purpose */
  TM_CORE_MSG_ID_NMEA_VAR_LEN_DEBUG_INFO = M_TM_CORE_MSG_ID_NMEA_VAR_LEN_DEBUG_INFO_0,

  /*TM API to handle sensor info */
  TM_CORE_MSG_ID_SENSOR_INFO = M_TM_CORE_MSG_ID_SENSOR_INFO_0,

  /*Wifi Ranging Scan for Techsel*/
  TM_CORE_MSG_ID_WIFI_RANGING_SCAN_TECHSEL = M_TM_CORE_MSG_ID_WIFI_RANGING_SCAN_TECHSEL_0,

  /*GNSS LE Fix*/
  TM_CORE_MSG_ID_GNSS_LE_FIX = M_TM_CORE_MSG_ID_GNSS_LE_FIX_0,

  /*Sensor LE Status*/
  TM_CORE_MSG_ID_SENSOR_LE_STATUS = M_TM_CORE_MSG_ID_SENSOR_LE_STATUS_0,

  /*Sensor LE Relative Displacement*/
  TM_CORE_MSG_ID_SENSOR_LE_REL_DISP = M_TM_CORE_MSG_ID_SENSOR_LE_REL_DISP_0,

  /* TM API to receive AS bit from MC - DEPRECATED */
  /* TM_CORE_MSG_ID_SPOOFING_INCONSISTENCY shall be used to report AS Bit inconsistency */
  TM_CORE_MSG_ID_GPS_AS_BIT_DEPRECATED = M_TM_CORE_MSG_ID_GPS_AS_BIT_DEPRECATED_0,

  /* TM API to receive Nav IQ Test Capabilities from MC */
  TM_CORE_MSG_ID_NAV_IQ_TEST_MASK = M_TM_CORE_MSG_ID_NAV_IQ_TEST_MASK_0,

  /* TM API to receive RF Dev Multichannel Track Test Capabilities from MC */
  TM_CORE_MSG_ID_RFDEV_TRACK_TEST_MASK = M_TM_CORE_MSG_ID_RFDEV_TRACK_TEST_MASK_0,

  /* TM API to handle AMPM disable from MC */
  TM_CORE_MSG_ID_DISABLE_APMP = M_TM_CORE_MSG_ID_DISABLE_APMP_0,

  /* TM API to report GNSS Persistent SV Mask */
  TM_CORE_MSG_ID_PERSISTENT_SV_MASK_REPORT = M_TM_CORE_MSG_ID_PERSISTENT_SV_MASK_REPORT_0,

  /* TM API to receive GNSS constellation config from MGP */
  TM_CORE_MSG_ID_CONSTELLATION_CONTROL = M_TM_CORE_MSG_ID_CONSTELLATION_CONTROL_0,

  /* TM API to report GNSS Persistent SV Mask */
  TM_CORE_MSG_ID_PERSISTENT_SV_MASK_TO_FILTER = M_TM_CORE_MSG_ID_PERSISTENT_SV_MASK_TO_FILTER_0,

  /* TM API to RC ON Threshold Info */
  TM_CORE_MSG_ID_RC_ON_THRESHOLD_INFO = M_TM_CORE_MSG_ID_RC_ON_THRESHOLD_INFO_0,

  TM_CORE_MSG_ID_SPOOFING_INCONSISTENCY = M_TM_CORE_MSG_ID_SPOOFING_INCONSISTENCY_0,

  /* TM API to receive Prescribed Dwell Results for the PFA test */
  TM_CORE_MGP_PRESC_DWELL_RESULTS = M_TM_CORE_MGP_PRESC_DWELL_RESULTS_0,

  /* TM API to receive NV/Cass config from Location Feature Manager*/
  TM_CORE_MSG_ID_LFM_CONF_INFO = M_TM_CORE_MSG_ID_LFM_CONF_INFO_0,

  /* TM API to receive Serving System notification from Data Services*/
  TM_CORE_DS_MSG_ID_EVENT_NTFY = M_TM_CORE_DS_MSG_ID_EVENT_NOTIFY_0,

  /* TM API to read new IMSI when subscription ready event is received */
  TM_CORE_READ_NEW_IMSI = M_TM_CORE_READ_NEW_IMSI_0,

  /* TM API to send GNSS Time Conversion Parameters to client */
  TM_CORE_MSG_ID_CLOCK_CONV_PARAM_REPORT = TM_CORE_MSG_ID_CLOCK_CONV_PARAM_REPORT_0,

  /* TM API to start/stop the constrained TUNC logic */
  TM_CORE_MSG_ID_START_CONSTRAINED_TUNC = M_TM_CORE_MSG_ID_START_CONSTRAINED_TUNC,
  TM_CORE_MSG_ID_STOP_CONSTRAINED_TUNC  = M_TM_CORE_MSG_ID_STOP_CONSTRAINED_TUNC,

  TM_CORE_MSG_ID_DC_REPORT_INFO = M_TM_CORE_MSG_ID_DC_REPORT_INFO,

  /* TM API to allow LTE to force GPS receiver off to reset the QLink */
  TM_CORE_MSG_ID_FORCE_RCVR_OFF_FROM_LTE = M_TM_CORE_MSG_ID_FORCE_RCVR_OFF_FROM_LTE_0,

  /* TM API to handle the navic assist ephemeris download request */
  TM_CORE_MSG_ID_XTRA_NAVIC_ASSIST_DOWNLOAD_REQ = M_TM_CORE_MSG_ID_XTRA_NAVIC_ASSIST_DOWNLOAD_REQ_0,
  /* TM API to receive Mag Cal State from PE */
  TM_CORE_MSG_ID_PDR_MAG_CAL_STATE = M_TM_CORE_MSG_ID_PDR_MAG_CAL_STATE_0,

  /******************************************************************************
  When adding a new TM msg ID, you MUST insert any new items from the Master list,
  before your new item (and update the Master list). Follow the procedure below:

  1) Using the IPC definitions in Master branch as baseline,
  replace the enum m_tm_ipc_msg_id_type.
  Master Branch: //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h

  2) In enum m_tm_ipc_msg_id_type, add a new message ID for the new IPC

  3) In enum tm_ipc_msg_id_type, create a mapping of the new IPC message
  with the message id from (2)

  In master IPC file sm_api.h at //source/qcom/qct/modem/gps/IPC_Definitions/sm_api.h
  Update enums m_tm_ipc_msg_id_type and tm_ipc_msg_id_type
  with the new IPC that you added.
  ******************************************************************************/

  TM_CORE_MSG_ID_LAST = M_TM_CORE_MSG_ID_LAST
} tm_ipc_msg_id_type;

/*=============================================================================

       Macros

=============================================================================*/

/*=============================================================================

       Typedefs

=============================================================================*/

/*=============================================================================
   GNSS Measurement Report structure
=============================================================================*/

typedef struct
{
  gnss_MeasBlkStructType z_MeasBlk;
  /* z_MeasBlk is dynamic size. DO NOT add anything here. */

} sm_GpsMeasRptStructType;

typedef struct
{
  gnss_SvPoly z_SvPolyReport;

} sm_SvPolyReportType;


/**
* Structure containing Bias information for an individual sensor
**/
typedef struct
{
  uint16  w_SensorValidMask; /* Indicated which of the axes have valid data
                                Use SENSOR_BIAS_VALID_xxx macro to find which
                                of the axes have valid data*/
  FLT f_XaxisValue; /*  X-axis Bias
                      - Units gyroscope:(radians)/(seconds)
                      - Units accelerometer:(meters)/(seconds^2)*/
  FLT f_YaxisValue; /*  Y-axis Bias
                      - Units gyroscope:(radians)/(seconds)
                      - Units accelerometer:(meters)/(seconds^2)*/
  FLT f_ZaxisValue; /*  Z-axis Bias
                      - Units gyroscope:(radians)/(seconds)
                      - Units accelerometer:(meters)/(seconds^2)*/
} sm_SensorBiasValueStructType;

/**
 * On-device Sensor Bias data from PE
 */
typedef struct
{
  uint16                       w_SeqenceNumber; /* Incremental packet serial number starting from 0, to identify missing packets  */
  sm_SensorBiasValueStructType z_GyroBias;      /* Contains axis validity mask + x,y,z values for Gyro bias */
  sm_SensorBiasValueStructType z_AccelBias;     /* Contains axis validity mask + x,y,z values for accel bias */
} sm_SensorParamRptStructType;

/* SM-specific navigation position flags. Each field is binary. The TRUE/FALSE state is described below. */
typedef struct
{
  /* GPS position report validity flags */
  uint64 b_IsValid             : 1;  /* Solution is/is not valid */
  uint64 b_IsItarViolated      : 1;
  uint64 b_IsGpsTimeValid      : 1;
  uint64 b_IsPuncValid         : 1;
  uint64 b_IsTuncValid         : 1;
  uint64 b_IsAltValid          : 1;

  /* Technology used in computing the position fix flags */
  uint64 b_IsSatellite         : 1;   /* Satellite is used for computing position fix */
  uint64 b_IsCellId            : 1;   /* Cell Id is used for computing position fix */
  uint64 b_IsWifi              : 1;   /* Wifi is used for computing postion fix */
  uint64 b_IsSensors           : 1;   /* Sensors is used for computing position fix */
  uint64 b_IsRefLoc            : 1;   /* Reference location is used for computing position fix */
  uint64 b_IsCoarsePos         : 1;   /* Coarse position is used for computing position fix */

  uint64 b_IsPosBestAvail      : 1;   /* Pos is best available position */
  uint64 b_IsSensorPropogatedGNSSFix : 1;   /* Solution used Only INS ONLY fixes */
  uint64 b_IsTdp               : 1;   /* TDP meas are used for computing position fix */
  uint64 b_IsPosInConsistent:1; /* Indication to SM is any inconsistency detected amongst
                               position in ALE database during a emergency session */
  uint64 b_IsPosFallBackGnss:1; /* Position is a fall back GNSS position and not necessarily best */
} sm_NavPosFlags;

/**   struct sm_NavPositionStructType describes the current position,
 *    velocity in the position engine. DO NOT change this
 *    structure without versioning the IPC log which use this
 *    struct.  Due to Unified Fix Reporting, only one IPC needs updating.
 *     - ARIES_MSG_ID_MGP_LM_REPORT_FIX
 * 
 *    This struct is derived from gps_NavSolution and differs
 *    only in that new fields have been added. this was done to
 *    help limit the changes cascading through the rest of SM.
 * 
 */
typedef struct
{
  /* Bit flags copied verbatim from gnss_NavSolutionStructType */
  gnss_NavSlnFlagsStructType z_PosFlags;

  /* Additional bit flags specific to SM */
  sm_NavPosFlags             z_SmPosFlags;

  /* GNSS Fix Time Information */
  uint16 w_GpsWeek;             /* GPS week number of fix */
  uint32 q_GpsTimeMs;           /* GPS time in week (milliseconds) of fix */
  uint8  u_GloNumFourYear;      /* Glonass four-year cycle number */
  uint16 w_GloNumDaysInFourYear; /* Glonass calendar day within the current four-year cycle */
  uint32 q_GloTimeMs;           /* Glonass time in day (milliseconds) */
  uint32 q_UtcTimeMs;           /* UTC time (milliseconds) adjusted for leap seconds */
  uint32 q_RefFCount;           /* Receiver's ms counter value */
  uint8  u_InitGpsTimeSrc;      /* Time source information (pdsm_extended_time_src_e_type) */

  /* GNSS Fix Position Information */
  uint32 q_PosCounter;          /* Position structure counter */
  DBL    d_PosEcef[3];          /* Earth Centered, Earth Fixed position fix (m) */
  DBL    d_PosLla[3];           /* Lat, Long, Alt */
  DBL    d_VelEcef[3];          /* ECEF velocity fix (m/s) */
  FLT    f_VelEnu[3];           /* East, North, Up velocity (m/s) */
  FLT    f_HeadingRad;          /* Heading angle, [0,2*pi] (rad). Referenced to North (0 rad) */
  FLT    f_HeadingUncRad;       /* Heading uncertainty (rad) */
  FLT    f_GnssHeadingRad;      /* Gnss Only heading */
  FLT    f_GnssHeadingUncRad;   /* Gnss Only heading uncertainty (rad) */

  FLT    f_Punc3dMeters;        /* Position standard deviation estimate (m) */
  FLT    f_PuncLatLonMeters[2]; /* Position (Lat, Long) uncertainty (m) */
  FLT    f_PuncVertMeters;      /* Vertical position uncertainty (m) */
  FLT    f_ErrorEllipse[3];     /* loc_uncrtnty_ang (degree), loc_uncrtnty_a (m), loc_uncrtnty_p (m) */
  FLT    f_Vunc3dMps;           /* Velocity uncertainty (m/s) */
  FLT    f_VuncEastNorthMps[2]; /* East, North velocity uncertainty (m/s) */
  FLT    f_VuncVertMps;         /* Vertical velocity uncertainty (m/s) */
  FLT    f_ClockBias;           /* Receiver's clock bias (m) */
  FLT    f_ClockBiasUncMs;      /* Receiver's clock uncertainty (m) */
  FLT    f_ClockDriftRate;      /* Receiver's clock drift (m/s) */
  FLT    f_ClockDriftRateUncMps; /* Receiver's clock frequency bias uncertainty (m/s) */
  gnss_PeTimeBiasStructType z_InterGnssTb[GNSS_TB_TOTNUM]; /* Inter-constellation Time Biases */
  FLT    f_FilteredAlt;         /* Filtered altitude (m) */
  FLT    f_FilteredAltUnc;      /* Filtered altitude uncertainty, one-sided (m) */
  FLT    f_UnfilteredAlt;       /* Unfiltered altitude (m) */
  FLT    f_UnfilteredAltUnc;    /* Unfiltered altitude uncertainty, one-sided (m) */
  FLT    f_MagDeviation;        /* Magnetic deviation from true north (degrees) */

  FLT    f_PDOP;                /* Position Dilution of Precision (unitless) */
  FLT    f_HDOP;                /* Horizontal DOP (unitless) */
  FLT    f_VDOP;                /* Vertial DOP (unitless) */
  FLT    f_TDOP;                /* Time DOP (unitless) */
  FLT    f_GDOP;                /* Geometrical DOP (unitless) */

  uint8  u_FirstFix;            /* Set TRUE when we have are first fix */
  uint8  u_DriftEstimated;      /* Set TRUE when the clock drift rate is estimated from
                                 * a difference of two clock biases */

  uint8  u_MagDevGood;          /* TRUE if magnetic devation is good */
  uint8  u_IonoCorrected;       /* FALSE indicates that not all SV measurements were iono corrected */
  uint8  u_TropoCorrected;      /* FALSE indicates that not all SV measurements were tropo corrected */

  pdsm_pd_reliability_e_type e_HoriRelIndicator; /* horizontal reliability */
  pdsm_pd_reliability_e_type e_VertRelIndicator; /* vertical reliability */
  pdsm_pd_altitude_source_e_type e_AltitudeSource; /* Altitude Source*/

  uint16 w_SensorDataUsageMask;    /* A bit set to 1 indicates that data from the corresponding 
                                      sensor was used in order to compute the content (position,
                                      velocity, heading) in this packet.
                                      0x0001 - Accelerometer
                                      0x0002 - Gyro
                                      0xFFFC - Reserved
                                      Future versions of the GPS service may use reserved bits 
                                      to represent new sensor types. A control point should 
                                      ignore any bits that it considers to be reserved. */
  uint16 w_SensorAidingIndicator;  /* A bit set to to 1 indicates that certain fields (position, 
                                      velocity, heading, etc) in parsed position data TLV were 
                                      aided with sensor data.
                                      0x0001 - Heading aided with sensor data
                                      0x0002 - Speed aided with sensor data
                                      0x0004 - Position aided with sensor data
                                      0x0008 - Velocity aided with sensor data
                                      0FFF0  - Reserved
                                      Future versions of the GPS service may use reserved bits 
                                      to represent new methods of sensor aiding (e.g. tightly 
                                      coupled measurement combinations). A control point should 
                                      ignore any bits that it considers to be reserved. */
  boolean b_CpiInconsistent;       /* TRUE if a faulty CPI fix was previously sent to SM. 
                                      A faulty CPI is detected by PE using Reference Location sent by network */

  FLT f_TechContributions[TECHNOLOGIES_MAX]; /* Technology contributions.  */

  int32 l_AgcMetric[GNSS_GMB_IDX_TOTAL]; /* AGC Metric in 0.01 db per band */

  int32 l_BpJammerMetric[GNSS_GMB_IDX_TOTAL]; /* BpJammerMetric in 0.01 db per band */

  gnss_IonoFreeSvIdMaskStructType z_IonoFreeSvIdMask; /* Iono-Free SV ID mask */
  /* This structure is part of an IPC that needs to be updated before adding anything to it */
} sm_NavPositionStructType;

/**
     struct containing fix info from PE

     This structure is used by PE to notify SM of periodic fixes
   
     @see sm_ReportFix
*/

/* for backward/forward compatibility, in case some day we may need to make 
   SM_API & PDAPI's NavInfo structs different */
#define SM_GNSS_SV_INFO_MEAS_VALID       PDSM_GNSS_SV_INFO_MEAS_VALID
#define SM_GNSS_SV_INFO_USABLE           PDSM_GNSS_SV_INFO_USABLE
#define SM_GNSS_SV_INFO_USED_IN_POS_FIX  PDSM_GNSS_SV_INFO_USED_IN_POS_FIX
#define SM_GNSS_SV_INFO_USED_IN_VEL_FIX  PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX
#define SM_GNSS_SV_INFO_SBAS_IONO_CORR   PDSM_GNSS_SV_INFO_SBAS_IONO_CORR
#define SM_GNSS_SV_INFO_SBAS_FAST_CORR   PDSM_GNSS_SV_INFO_SBAS_FAST_CORR
#define SM_GNSS_SV_INFO_SBAS_LONG_CORR   PDSM_GNSS_SV_INFO_SBAS_LONG_CORR
#define SM_GNSS_SV_INFO_SBAS_RTI_CORR    PDSM_GNSS_SV_INFO_SBAS_RTI_CORR

/* use PDAPI SVINFO structure diretly since PDAPI is now internal API */
typedef pdsm_GnssNavSvInfo      sm_GnssNavSvInfo;
typedef pdsm_GnssSvInfoType     sm_GnssSvInfoType;

/*=============================================================================
   GPS Assistance data status structure
=============================================================================*/
/* Bit-mask definitions for the Wish list */
#define C_GNSS_ASSIST_REFTIME  (1U<<0)      /// Reference Time (GPS or GLONASS)
#define C_GNSS_ASSIST_REFLOC   (1U<<1)      /// Reference Location
#define C_GNSS_ASSIST_ACQ      (1U<<2)      /// Acquisition Assistance contains SV direction & steering info
#define C_GNSS_ASSIST_SA        (1U<<3)     /// Sensitvity Assistance
#define C_GNSS_ASSIST_NAVMODL  (1U<<4)      /// Ephemeris a.k.a. NavigationModel
#define C_GNSS_ASSIST_ALM      (1U<<5)      /// Almanac
#define C_GNSS_ASSIST_IONO     (1U<<6)      /// IonosphericModel (Propogated signal delay)
#define C_GNSS_ASSIST_RTI      (1U<<7)      /// RealTimeIntegrity a.k.a satellite health info
#define C_GNSS_ASSIST_UTC      (1U<<8)      /// UTC Time info (used to relate with GPS or GLONASS system)
#define C_GNSS_ASSIST_TIME_MODELS (1U<<9)   /// Time model (GNSS-GNSS system time offset)
#define C_GNSS_ASSIST_AUXI_INFO   (1U<<10)  /// Auxiliary Info (Used in conjunction with NavModel)

/* the following are retained for backwards compatibility
   but their use is deprecated. Use C_GNSS_ASSIST_xxx flags
   instead 
*/
#define C_GPS_ASSIS_REFTIME      C_GNSS_ASSIST_REFTIME
#define C_GPS_ASSIS_REFLOC       C_GNSS_ASSIST_REFLOC
#define C_GPS_ASSIS_ACQ          C_GNSS_ASSIST_ACQ
#define C_GPS_ASSIS_SA           C_GNSS_ASSIST_SA
#define C_GPS_ASSIS_NAVMODL      C_GNSS_ASSIST_NAVMODL
#define C_GPS_ASSIS_ALM          C_GNSS_ASSIST_ALM
#define C_GPS_ASSIS_IONO         C_GNSS_ASSIST_IONO
#define C_GPS_ASSIS_RTI          C_GNSS_ASSIST_RTI
#define C_GPS_ASSIS_UTC          C_GNSS_ASSIST_UTC

/* The data size required for single SV information in PQWP7 is
- DataType  : 1 digit
- DataSource: 1 digit
- Data age  : 11 digits (+/-int32_max sec range)
- 3 commas  : 3 bytes
This should multiplied by total number of SVs and adding 100 to accomodate for
header, time and sentence termination*/
#define C_DBUG_NMEA_PQWP7_BUFER_SIZE (16*(N_ALL_GNSS_SV-N_BDS_SV+N_BDS_SV_TOTAL)+100)

typedef struct
{
  uint16 w_Sv;                     /* SV PRN */
  uint16 w_Iode;                   /* Issue of data Ephemeris tag */

} sm_SvEphIodeStructType;

typedef struct
{
  FLT f_PosUnc;
  boolean u_Valid;
} sm_PosUncStructType;

typedef struct
{
  FLT f_TimeUnc;
  boolean u_Valid;
} sm_TimeUncStructType;

typedef struct
{
  uint16                 w_Week;     /* GPS Week of Ephemeris validity */
  uint16                 w_Toe;      /* Reference time of the latest Ephemeris */
  uint16                 w_ToeLimit; /* Ephemeris Age tolerance (hours) */

} sm_GpsEphTime;

typedef struct
{
  uint16                 w_N4;        /* GLONASS 4-Year count (N4)*/
  uint16                 w_Na;        /* GLONASS Day in 4-year cycle (Na) */
  uint16                 w_Tb;        /* Reference time of the latest Ephemeris: uinit: 15 minutes, range 0 to 95 */
  uint16                 w_ToeLimit;  /* Ephemeris Age tolerance, double sided. 
                                         1 = 30 min, 2 = 45 min, 3 = 60 min; 
                                         0 = reserved; other = invalid */
} sm_GloEphTime;

typedef struct
{
  uint16                 w_Week;     /* BDS Week of Ephemeris validity */
  uint16                 w_ToeLimit; /* Ephemeris Age tolerance (hours) */
  uint32                 q_Toe;      /* Reference time of the latest Ephemeris */
} sm_BdsEphTime;

typedef struct
{
  uint16                 w_Week;     /* GAL Week of Ephemeris validity */
  uint16                 w_ToeLimit; /* Ephemeris Age tolerance (hours) */
  uint32                 q_Toe;      /* Reference time of the latest Ephemeris */
} sm_GalEphTime;

typedef struct
{
  uint16                 w_Week;     /* NavIC Week of Ephemeris validity */
  uint16                 w_ToeLimit; /* Ephemeris Age tolerance (hours) */
  uint32                 q_Toe;      /* Reference time of the latest Ephemeris */
} sm_NavicEphTime;

typedef union
{
  sm_GpsEphTime z_GpsEphTime;
  sm_GloEphTime z_GloEphTime;
  sm_BdsEphTime z_BdsEphTime;
  sm_GalEphTime z_GalEphTime;
  sm_NavicEphTime z_NavicEphTime;
  /* add other constellation times here */
} sm_GnssEphTime;

typedef struct
{
  boolean                u_EphInfoIncl;     /**< TRUE if EPH information is included */
  uint8                  u_NumSvs;          /**< Number of SVs for which IODE info is provided */
  uint32                 q_WishListMask;    /**< Bit-mask indicating the type of data needed */
  uint32                 q_EphHaveList;     /**< Bit-mask of SVs for which Ephemeris is available */
  uint32                 q_EphNeedList;     /**< Bit-mask of SVs for which Ephemeris is needed */
  uint32                 q_AlmHaveList;     /**< Bit-mask of SVs for which Almanac is available */
  uint32                 q_AlmNeedList;     /**< Bit-mask of SVs for which Almanac is needed */
  uint32                 q_VisibleSVList;   /**< Bit-mask of all visible SVs*/
  uint32                 q_SvNoExistMask;   /**< Bit-mask of SVs which are non-existent */
  sm_GnssEphTime         z_AssistDataTime;  /**< time associated with navigation data */
  sm_TimeUncStructType   z_TimeUnc;         /**< Time uncertainty information */
  sm_PosUncStructType    z_PosUnc;          /**< Horizontal position uncertainty */
  uint32                 q_HealthHaveList;  /**< Good list */
  uint32                 q_HealthNeedList;  /**< Unknown */
  uint32                 q_HealthBadList;   /**< Bad SVs */
  sm_SvEphIodeStructType z_SvEphIode[N_SV];
} sm_GnssAssistDataStruct;

typedef struct
{
  boolean                u_EphInfoIncl;     /**< TRUE if EPH information is included */
  uint8                  u_NumSvs;          /**< Number of SVs for which IODE info is provided */
  uint32                 q_WishListMask;    /**< Bit-mask indicating the type of data needed */
  uint64                 t_EphHaveList;     /**< Bit-mask of SVs for which Ephemeris is available */
  uint64                 t_EphNeedList;     /**< Bit-mask of SVs for which Ephemeris is needed */
  uint64                 t_AlmHaveList;     /**< Bit-mask of SVs for which Almanac is available */
  uint64                 t_AlmNeedList;     /**< Bit-mask of SVs for which Almanac is needed */
  uint64                 t_VisibleSVList;   /**< Bit-mask of all visible SVs*/
  uint64                 t_SvNoExistMask;   /**< Bit-mask of SVs which are non-existent */
  sm_GnssEphTime         z_AssistDataTime;  /**< time associated with navigation data */
  sm_TimeUncStructType   z_TimeUnc;         /**< Time uncertainty information */
  sm_PosUncStructType    z_PosUnc;          /**< Horizontal position uncertainty */
  uint64                 t_HealthHaveList;  /**< Good list */
  uint64                 t_HealthNeedList;  /**< Unknown */
  uint64                 t_HealthBadList;   /**< Bad SVs */
  sm_SvEphIodeStructType z_SvEphIode[N_BDS_SV];
} sm_BdsAssistDataStruct;

typedef struct
{
  boolean                u_EphInfoIncl;     /**< TRUE if EPH information is included */
  uint8                  u_NumSvs;          /**< Number of SVs for which IODE info is provided */
  uint32                 q_WishListMask;    /**< Bit-mask indicating the type of data needed */
  uint64                 t_EphHaveList;     /**< Bit-mask of SVs for which Ephemeris is available */
  uint64                 t_EphNeedList;     /**< Bit-mask of SVs for which Ephemeris is needed */
  uint64                 t_AlmHaveList;     /**< Bit-mask of SVs for which Almanac is available */
  uint64                 t_AlmNeedList;     /**< Bit-mask of SVs for which Almanac is needed */
  uint64                 t_VisibleSVList;   /**< Bit-mask of all visible SVs*/
  uint64                 t_SvNoExistMask;   /**< Bit-mask of SVs which are non-existent */
  sm_GnssEphTime         z_AssistDataTime;  /**< time associated with navigation data */
  sm_TimeUncStructType   z_TimeUnc;         /**< Time uncertainty information */
  sm_PosUncStructType    z_PosUnc;          /**< Horizontal position uncertainty */
  uint64                 t_HealthHaveList;  /**< Good list */
  uint64                 t_HealthNeedList;  /**< Unknown */
  uint64                 t_HealthBadList;   /**< Bad SVs */
  sm_SvEphIodeStructType z_SvEphIode[N_GAL_SV];
} sm_GalAssistDataStruct;

typedef struct
{
  boolean                 u_EphInfoIncl;     /**< TRUE if EPH information is included */
  uint8                   u_NumSvs;          /**< Number of SVs for which IODE info is provided */
  uint32                  q_WishListMask;    /**< Bit-mask indicating the type of data needed */
  uint32                  q_EphHaveList;     /**< Bit-mask of SVs for which Ephemeris is available */
  uint32                  q_EphNeedList;     /**< Bit-mask of SVs for which Ephemeris is needed */
  uint32                  q_AlmHaveList;     /**< Bit-mask of SVs for which Almanac is available */
  uint32                  q_AlmNeedList;     /**< Bit-mask of SVs for which Almanac is needed */
  uint32                  q_VisibleSVList;   /**< Bit-mask of all visible SVs*/
  uint32                  q_SvNoExistMask;   /**< Bit-mask of SVs which are non-existent */
  sm_GnssEphTime          z_AssistDataTime;  /**< time associated with navigation data */
  sm_TimeUncStructType    z_TimeUnc;         /**< Time uncertainty information */
  sm_PosUncStructType     z_PosUnc;          /**< Horizontal position uncertainty */
  uint32                  q_HealthHaveList;  /**< Good list */
  uint32                  q_HealthNeedList;  /**< Unknown */
  uint32                  q_HealthBadList;   /**< Bad SVs */
  sm_SvEphIodeStructType  z_SvEphIode[N_QZSS_SV];
} sm_QzssAssistDataStruct;

typedef struct
{
  boolean                u_EphInfoIncl;     /**< TRUE if EPH information is included */
  uint8                  u_NumSvs;          /**< Number of SVs for which IODE info is provided */
  uint32                 q_WishListMask;    /**< Bit-mask indicating the type of data needed */
  uint64                 t_EphHaveList;     /**< Bit-mask of SVs for which Ephemeris is available */
  uint64                 t_EphNeedList;     /**< Bit-mask of SVs for which Ephemeris is needed */
  uint64                 t_AlmHaveList;     /**< Bit-mask of SVs for which Almanac is available */
  uint64                 t_AlmNeedList;     /**< Bit-mask of SVs for which Almanac is needed */
  uint64                 t_VisibleSVList;   /**< Bit-mask of all visible SVs*/
  uint64                 t_SvNoExistMask;   /**< Bit-mask of SVs which are non-existent */
  sm_GnssEphTime         z_AssistDataTime;  /**< time associated with navigation data */
  sm_TimeUncStructType   z_TimeUnc;         /**< Time uncertainty information */
  sm_PosUncStructType    z_PosUnc;          /**< Horizontal position uncertainty */
  uint64                 t_HealthHaveList;  /**< Good list */
  uint64                 t_HealthNeedList;  /**< Unknown */
  uint64                 t_HealthBadList;   /**< Bad SVs */
  sm_SvEphIodeStructType z_SvEphIode[N_NAVIC_SV];
} sm_NavicAssistDataStruct;

typedef struct
{
  uint32                   q_WishListMask;   /**< Bit-mask indicating the type of data needed */
  uint32                   q_EphHaveList;    /**< Bit-mask of SVs for which Ephemeris is available */
  uint32                   q_EphNeedList;    /**< Bit-mask of SVs for which Ephemeris is needed */
  uint32                   q_AlmHaveList;    /**< Bit-mask of SVs for which Almanac is available */
  uint32                   q_AlmNeedList;    /**< Bit-mask of SVs for which Almanac is needed */
  uint32                   q_VisibleSVList;  /**< Bit-mask of all visible SVs*/
  uint32                   q_SvNoExistMask;  /**< Bit-mask of SVs which are non-existent */
  boolean                  u_EphInfoIncl;    /**< TRUE if EPH information is included */
  uint16                   w_GpsWeek;        /**< GPS Week of Ephemeris validity */
  uint16                   w_Toe;            /**< Reference time of the latest Ephemeris */
  uint16                   w_ToeLimit;    /**< Ephemeris Age tolerance (hours) */
  uint8                    u_NumSvs;      /**< Number of SVs for which IODE info is provided */
  sm_TimeUncStructType     z_TimeUnc;       /**< Time uncertainty information */
  sm_PosUncStructType      z_PosUnc;        /**< Horizontal position uncertainty */
  sm_SvEphIodeStructType   z_SvEphIode[N_SV];
  uint32                   q_HealthHaveList;
  uint32                   q_HealthNeedList;
  uint32                   q_HealthNoExList;
  uint32                   q_HealthBadList;

  sm_GnssAssistDataStruct  z_GloAssistData; /**< glonass assist data status structure */
  sm_BdsAssistDataStruct   z_BdsAssistData; /**< BDS assist data status structure */
  sm_GalAssistDataStruct   z_GalAssistData; /**< GAL assist data status structure */
  sm_QzssAssistDataStruct  z_QzssAssistData; /**< QZSS assist data status structure */
  sm_NavicAssistDataStruct z_NavicAssistData;/**< Navic assist data status structure */
  uint8                    u_Valid; /**< which assistance data is valid. 
                                                  Bit 0 for gps,
                                                  bit 1 for Glonass
                                                  bit 2 for BDS
                                                  bit 3 for Galileo
                                                  bit 4 for QZSS
                                                  bit set=valid, not-set=invalid.
                                                  Use SM_ASSIST_DATA_VALID_xxx */
} sm_InternalAssistDataStatusStructType;

#define SM_ASSIST_DATA_VALID_MASK_GPS          ((uint16)(0x01<<0))
#define SM_ASSIST_DATA_VALID_MASK_GLONASS      ((uint16)(0x01<<1))
#define SM_ASSIST_DATA_VALID_MASK_BDS          ((uint16)(0x01<<2))
#define SM_ASSIST_DATA_VALID_MASK_GAL          ((uint16)(0x01<<3))
#define SM_ASSIST_DATA_VALID_MASK_QZSS         ((uint16)(0x01<<4))
#define SM_ASSIST_DATA_VALID_MASK_NAVIC        ((uint16)(0x01<<5))

typedef struct
{
  uint16                   w_Valid; /**< which assistance data is valid. 
                                                  Bit 0 for gps,
                                                  bit 1 for Glonass
                                                  bit 2 for BDS
                                                  bit 3 for (reserved)
                                                  bit 3 for GAL
                                                  bit 4 for QZSS
                                           bit set=valid, not-set=invalid.
                                           Use SM_ASSIST_DATA_VALID_xxx */

  sm_GnssAssistDataStruct    z_GpsAssistData;   /**< gps assistance data */
  sm_GnssAssistDataStruct    z_GloAssistData;   /**< glonass assistance data */
  sm_BdsAssistDataStruct     z_BdsAssistData;   /**< BDS assistance data */
  sm_GalAssistDataStruct     z_GalAssistData;   /**< GAL assistance data */
  sm_QzssAssistDataStruct    z_QzssAssistData;  /**< QZSS assistance data */
  sm_NavicAssistDataStruct   z_NavicAssistData; /**< Navic assisstance data */
} sm_GnssAssistDataStatusStructType;


/*=============================================================================
   Pilot Phase Measurement Report structure
=============================================================================*/
typedef struct
{
  int32 l_PilotPnPhase;
  uint8 u_PilotRmse;
  uint8 u_PilotEcio;

} sm_PilotInfoStructType;

/** UTC information
 * 
 * UTC data decoded by PE off the air to SM. Since both GLONASS
 * and GPS times are consistent, leap second conversion is
 * unnecessary and the same API may be used. Refer MGP TO SM API
 * documentation for details
 * 
 */
typedef struct
{
  uint32 q_A0;      /* UTC: convert from GPS time to UTC time. */
  uint32 q_A1;
  uint8  u_DeltaTls;
  uint8  u_Tot;
  uint8  u_WNt;
  uint8  u_WNlsf;
  uint8  u_DN;
  uint8  u_DeltaTlsf;

} sm_UtcInfoRptStructType;

/*=============================================================================
   Receiver State Change event
=============================================================================*/
/**
     Receiver State Change event

     ME reports receiver state change and associated gpsRtc
*/
typedef struct
{
  uint32                   q_GpsRtcMs;
  gps_RcvrStateEnumType e_CurrentRcvrState;
  gnss_RcvrSubStateStructType z_currentRcvrSubState;
  uint8 g_RcvrStateErrorInd;
} sm_RcvrStateChangeStructType;

/**
     SM Event Report type

     ME and PE report back to SM when one of these events
     happens
*/
typedef enum
{
  /**< Event type */                 /**< When generated */
  SM_EVENT_MIN = 0,
  SM_EVENT_ME_TIME_TICK,          /**< Periodically in Receiver IDLE/ON modes */
  SM_EVENT_ME_NEW_MEAS,           /**< Periodically, if new measurements are obtained */
  SM_EVENT_ME_ASSIGN_CHANGE,      /**< Processing mode changes in ME */
  SM_EVENT_ME_CELL_CHANGE,        /**< When serving cell changes */
  SM_EVENT_ME_TIME_TRANSFER,      /**< New time-transfer took place. Cell changed or periodic update on current cell */
  SM_EVENT_ME_TIME_UPDATE,        /**< Clock update in ME, better (or forced) estimate received */
  SM_EVENT_ME_TIME_DISCONTINUITY, /**< ME detected a time discontinuity */
  SM_EVENT_ME_TIME_ERR_DETECT,    /**< ME detected an error in its clock estimate */
  SM_EVENT_ME_MEAS_SUSPENDED,     /**< Measurements are suspended (resources pre-empted) */
  SM_EVENT_ME_MEAS_ABORTED,       /**< Measurements are aborted (resources lost completely for the session) */
  SM_EVENT_ME_SA_DATA_NEEDED,     /**< ME needs SA data to improve the sensitivity */
  SM_EVENT_PE_NEW_NAVDATA,        /**< PE received new Ephemeris */
  SM_EVENT_PE_NEW_ALMANAC,        /**< PE received new Almanac */
  SM_EVENT_PE_NEW_HEALTH,         /**< PE received new SV Health info */
  SM_EVENT_PE_NEW_IONO_UTC,       /**< PE received new Iono/UTC info */
  SM_EVENT_PE_DEL_NAVDATA,        /**< Ephemeris data is deleted */
  SM_EVENT_PE_DEL_ALMANAC,        /**< Almanac data is deleted */
  SM_EVENT_PE_STITCH_FIRST_BP,    /**< PE has started Stitch back propogation */
  SM_EVENT_PE_STITCH_INCOMPLETE_BP, /**< PE has suspended Stitch back propogation */
  SM_EVENT_PE_STITCH_LAST_BP,     /**< PE has completed Stitch back propogation */
  SM_EVENT_PE_KF_RESET_OR_INIT,   /**< KF has been reset or initialized */
  SM_EVENT_PE_LOG_KF,             /**< KF data has been logged */
  SM_EVENT_ME_IQ_DONE,            /**< IQ Collect/FFT done */
  SM_EVENT_PE_NO_NEW_FIX,         /**< PE no new fix generated */
  SM_EVENT_PE_LOG_ALM_CORR,       /**< XTRA alm corr have been logged */
  SM_EVENT_ME_SIG_PROC_MEM_ALLOC_ERROR, /** Error event to indicate Session mode allocation failure for Signal Processing memory*/
  SM_EVENT_ME_CONSTELLATION_CFG_BAND1, /**< ME Updates Active Qualified constellation info for Band1 */
  SM_EVENT_ME_CONSTELLATION_CFG_BAND5, /**< ME Updates Active Qualified constellation info for Band5 */
  SM_EVENT_ME_BOOTUP_CAL_STATUS,
  SM_EVENT_MAX
  
} sm_EventEnumType;

/*=============================================================================
   Structure for reporting events to SM
=============================================================================*/
typedef struct
{
  sm_EventEnumType e_EventType;
  uint32           q_EventData;
  uint32           q_EventTime;
} sm_ReportEventStructType;

typedef union
{
  cgps_1xRfInfoType   CDMARfInfo;
  cgps_EVDORfInfoType EVDORfInfo;
} sm_ReportRfInfoUnionType;

typedef struct
{
  gps_DataSourceType       RfInfoSource;
  sm_ReportRfInfoUnionType RfInfoData;
} sm_ReportRfInfoStructType;

/**
 * Time report from MGP/PE
 * 
 * Delivers estimate of current time to SM. PE calls this API in
 * response to a request from SM via mgp_ReqGnssTime
 */
typedef struct
{
  gps_TimeChkSourceType source;
  gnss_ClockStructType  gnssClock;
} sm_ReportGnssTimeStructType;


/**
 * SM_GEN7_COMPATIBILITY(sm_ReportGpsTimeStructType)
 * sm_ReportGpsTimeStructType is being retained for backwards
 * compatibility. Gen7 MGP APIs call this to report time.
 * this however, may be removed at a later date since the
 * data actually comes from PE.
 * See sm_ReportGnssTime in MGP to SM api documentation
 */
typedef struct
{
  gps_TimeChkSourceType    source;
  gps_ClockStructType      GpsClock;
} sm_ReportGpsTimeStructType;

/**
     Latest position and associated uncertainties

     This structure is used by ME to indicate the position and
     its associated uncertainties.
   
     @see sm_UpdatePosPuncTunc
*/
typedef struct cgps_OnDemandPosPuncTunc
{
  //  sm_GpsFixRptStructType pos_info; /* null if no valid position is available */
  boolean b_Valid; /**< specifies if the pos punc/tunc are valid */
  FLT     f_Tunc; /**< in us */
  FLT     f_Punc; /**< in meters */
  uint32  eph_svmask; /**< SV Mask for the ephemeris */
  uint32  alm_svmask; /**< SV Mask for the Almanac */
  uint32  health_svmask; /**< SV Mask for the health */
  uint32  health_bad_svmask; /**< SV Mask for the health */
  uint32  health_noex_svmask; /**< SV Mask for the health */
  uint32  health_unk_svmask; /**< SV Mask for the health */
  uint32  visible_svmask; /**< SV mask for visible SVs*/
} sm_OnDemandPosPuncTuncStruct;

typedef struct
{
  /* GPS L1 C/N0 results */
  FLT f_GpsL1CNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_GpsL1FreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* GLO G1 C/N0 results */
  FLT f_GloG1CNoDBHz;  /* Signal strength calculated in 
                           dBHz as part of WBIQ test*/
  FLT f_GloG1FreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* BDS B1 C/N0 results */
  FLT f_BdsB1CNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_BdsB1FreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* GAL E1 C/N0 results */
  FLT f_GalE1CNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_GalE1FreqHz; /* Freq in Hz calculated as part of WBIQ test */

  uint32 q_GnssConfig;    /* Flags containing configured constellations.
                         C/N0 results are valid for the configured constellations.  
                         Bit 0 - GPS
                         Bit 1 - Glonass
                         Bit 4 - Beidou
                         Bit 5 - Galileo */

  /* ADC mean/amp in units of 0.1 mV */
  int32 l_AdcMeanIp1mV; /* Mean Estimate (DC) of I component in 
                       ADC processor in 0.1 mV */
  int32 l_AdcMeanQp1mV; /* Mean Estimate (DC) of Q component in 
                       ADC processor in 0.1 mV */
  uint32 q_AdcAmpIp1mV; /* Amplitude estimate of I component in 
                       ADC processor in 0.1 mV */
  uint32 q_AdcAmpQp1mV; /* Amplitude estimate of Q component in 
                       ADC processor in 0.1 mV */

  uint8  u_AdcIqSatPerStatus;  /* Indicates validity/status of data.
                                  0 - ADC IQ saturation percentage is not valid
                                  3 - Number of samples in ADC IQ test was zero
                                  4 - ADC IQ saturation percentage is valid */
  uint32 q_ISatPer; /* Percentage of I samples saturated, in units of 0.001 */
  uint32 q_QSatPer; /* Percentage of Q samples saturated, in units of 0.001 */
  int32  l_AdcIqWbJammerPwrEstDb; /* Estimated wideband jammer power in units of 0.01 dB.
                                     Only valid if wideband jammer power estimation was enabled in the 
                                     ADC IQ test request.*/
  int32  l_GpsL1NoiseFloorEstDb; /* GPS Noise floor estimate over 2 MHz in units of 0.01 dB */
  int32  l_GloG1NoiseFloorEstDb; /* GLO Noise floor estimate over 10 MHz in units of 0.01 dB */
  int32  l_BdsB1NoiseFloorEstDb; /* BDS Noise floor estimate over 4 MHz in units of 0.01 dB */
  int32  l_GalE1NoiseFloorEstDb; /* GAL Noise floor estimate over 4 MHz in units of 0.01 dB */
  int32  l_GloG1FreqNoiseFloorEstDb[N_GLO_FREQ]; /* GLO Noise floor estimate 
                                                  for each 1 MHz in units of 0.01 dB */

  uint32 q_MultiBandConfig; /* Flags containing configured constellations.
                               C/N0 results are valid for the configured constellations.  
                               Bit 9 - GPS L5
                               Bit 10 - GAL E5a
                               Bit 11 - BDS B2a
                               Bit 12 - GAL E5b
                               Bit 13 - NavIC 
                               Bit 14 - GPS L2 */

  /* GPS L5 / GAL E5a C/N0 results */
  FLT f_GpsL5GalE5aCNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_GpsL5GalE5aFreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* GAL E5b C/N0 results */
  FLT f_GalE5bCNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_GalE5bFreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* NavIC C/N0 results */
  FLT f_NavicCNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_NavicFreqHz; /* Freq in Hz calculated as part of WBIQ test */

  /* GPS L2 C/N0 results */
  FLT f_GpsL2CNoDBHz;  /* Signal strength calculated in 
                          dBHz as part of WBIQ test*/
  FLT f_GpsL2FreqHz; /* Freq in Hz calculated as part of WBIQ test */

} sm_ReportWBIQInfoStructType;

typedef struct {
  int32   l_RefOffsetPpb;        /* XO offset in PPB */
  int32   l_ClkFreqBiasPpb;      /* Clock frequency bias in PPB */
  int32   l_ClkFreqBiasUncPpb;   /* Clock frequency uncertainty in PPB */
  boolean v_IsDataValid;         /* Data validity */
}sm_ReportXOOffsetInfoStructType;

/* Structure used by MC Jobmanager to send Prescribed Dwell results
   back to TM (via LM) */
typedef struct
{
  mgp_GnssType e_GnssType; /* GPS/GLO/BDS/GAL */
  uint8     u_Sv;         /* GNSS Sv Id */
  uint8     u_ChanNum;    /* GNSS Channel Number */
  boolean   v_ValidPeak;  /* Indicates a valid peak report when TRUE */
  uint32    q_CN0p1dB;    /* units of 0.1 dB */

  /* Receiver config info */
  uint32 q_GnssConfig;    /* Flags containing configured 
                            constellations */
  uint8 u_NumGloChannels; /* Number of GLO channels
                             enabled in this config */
} sm_PfaTestPresDwellResultsType;

/* This structure is used to pass on NBIQ data pointer
   and size of NBIQ data to SM from MGP */
typedef struct
{
  void *p_NBIQDataPtr;
  uint32 q_NBIQDataSize;
} sm_ReportNBIQInfoStructType;


/* This structure is used to pass on BP Amp and 
   PGA gain information from MGP to SM. This can be extended
   later on if OEM wants other GNSS RF related data */
typedef struct
{
  int32   l_PGAGain;

  /* BP1 params */
  uint32  q_Bp1LbwAmplI;
  uint32  q_Bp1LbwAmplQ;

  /* BP3 GLO processor status */
  uint32  q_Bp3GloAmplI;
  uint32  q_Bp3GloAmplQ;
} sm_ReportGNSSRfStatusInfoStructType;

#define SM_GPS_STATE_POS_VALID 0x0001
#define SM_GPS_STATE_TIME_VALID 0x0002
#define SM_GPS_STATE_PUNC_VALID 0x0004
#define SM_GPS_STATE_TUNC_VALID 0x0008
#define SM_GPS_STATE_FUNC_VALID 0x0010
#define SM_GPS_STATE_FCLKDRT_VALID 0x0020
#define SM_GPS_STATE_CLKBIAS_VALID 0x0040

typedef enum
{
  /*Get GPS State Info Req */
  SM_GPS_STATE_INFO_REQ_TYPE_NONE                      = 0,
  SM_GPS_STATE_INFO_REQ_TYPE_MIN                       = 1,
  SM_GPS_STATE_INFO_REQ_TYPE_ON_DEMAND                 = SM_GPS_STATE_INFO_REQ_TYPE_MIN, /* deprecated */
  SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE               = 2,
  SM_GPS_STATE_INFO_REQ_TYPE_DIAG_POS                  = 3, /* deprecated */
  SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL = 4,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI    = 5,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T = 6,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI     = 7,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_GNSS_AVAIL_POS_QMI     = 8,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_REFLOC_AVAIL_POS_QMI     = 9,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_COARSE_AVAIL_POS_QMI     = 10,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_WIFI_AVAIL_POS_QMI     = 11,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_GILE_AVAIL_POS_QMI     = 12,
  SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH                   = 13,
  /****ADD ALL GPS STATE INFO REQUESTORS ABOVE THIS.
           AND ANY OTHER KIND OF REQUESTS AT THE END OF THE NUM and update MAX
   ****/
  SM_GPS_STATE_INFO_REQ_TYPE_MAX                        = SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH,

  /*Wifi fix request*/
  SM_REQ_TYPE_WIFI_POS_FOR_MIN                          = SM_GPS_STATE_INFO_REQ_TYPE_MAX + 1,
  SM_REQ_TYPE_WIFI_POS_FOR_BLE                          = SM_REQ_TYPE_WIFI_POS_FOR_MIN,
  SM_REQ_TYPE_WIFI_POS_FOR_MAX                          = SM_REQ_TYPE_WIFI_POS_FOR_BLE,

  /* Position Estimation request */
  SM_REQ_TYPE_GET_POS_EST                               = SM_REQ_TYPE_WIFI_POS_FOR_MAX + 1,
  SM_REQ_TYPE_MAX                                       = SM_REQ_TYPE_GET_POS_EST
} sm_ReqEnumType;

typedef enum
{
  SM_LOC_EST_SRC_GPS  = 1,
  SM_LOC_EST_SRC_WIFI  = 2
}sm_LocEstimateSrcEnumType;

typedef struct sm_GpsStateInfo
{
  sm_ReqEnumType  b_ReqType;
  uint32                      q_Flags;      /* flags to indicate which fields are valid */
  uint8                       e_PosInfoSrc; /* Source of the Position Estimate */
  DBL                         d_PosLLa[3];
  FLT                         f_AltUnc;     /* Altitude Uncertainty in meters */
  FLT                         f_Tunc;       /* in us */
  FLT                         f_Punc;       /* in meters */

  uint16                      w_GpsWeek;    /* GPS Week time reference for the Position */
  uint32                      q_GpsMsec;    /* GPS Msec time reference for the Position */
  FLT                         f_Func;       /* 1 sided max freq bias uncertainty [m/s] */
  FLT                         f_ClkDrift;   /* Clock Drift in meters/sec */
  FLT                         f_ClkBias;    /* Clock frequency bias in meters/sec */
} sm_GpsStateInfoStruct;

/* Fix Auxiliary into type enum that indicates what element of the union 
sm_FixAuxInfoStructType is reported to SM */
typedef enum
{
  SM_AUXINFO_TYPE_NONE = 0,
  SM_AUXINFO_TYPE_WIFI,
  SM_AUXINFO_TYPE_TDP,
} sm_FixAuxInfoTypeEnumType;

/* LP GTP-Wifi Auxiliary info structure */
typedef struct
{
  wl_ResultFlagsType  z_WifiResult;
} sm_WifiAuxInfoStructType;

/* Unified fix reporting Auxliary information */
typedef union
{
  sm_WifiAuxInfoStructType z_WifiAuxInfo;
} sm_FixAuxInfoStructType;

typedef struct
{
  sm_NavPositionStructType    z_NavPos;     /* position-related info */

  sm_GnssNavSvInfo            z_NavSvInfo;  /* SV-related info */

  sm_SensorParamRptStructType z_OnDeviceSensorParam; /* sensor-related info */
} sm_FixInfoStructType;

//DELME
typedef sm_FixInfoStructType   sm_GpsFixRptStructType;


/* Unified fix reporting structure. ALE SHALL always report all fixes to any SM component
via a common API with this payload */
typedef struct
{
  sm_ReqEnumType b_ReqType;      /* Source of the request for best available position */
  sm_FixInfoStructType       z_FixInfo;     /* position-related info */
  sm_FixAuxInfoTypeEnumType  z_FixAuxInfoType; /* Type that indicates what element of the below union is
                                            going to be reported */
  sm_FixAuxInfoStructType    z_FixAuxInfo; /* Auxiliary info for the reported fix is available */
} sm_FixReportStructType;

#ifdef FEATURE_CGPS_XTRA_T
/* Structure for transferring cell data from MC to SM */
typedef struct
{
  uint8  u_PartNumber; /* current part number of this structure */
  uint8  u_TotalParts; /* total number of parts in overall transfer */
  uint32 q_DataSize;   /* number of bytes of data pointed to by p_DataPtr */
  void *p_DataPtr;    /* pointer to byte buffer containing cell data */
} sm_ReportCellDataStructType;
#endif /* FEATURE_CGPS_XTRA_T */

typedef pdsm_pd_ext_var_len_debug_nmea_s_type sm_DebugNMEAVarLen;

typedef struct
{
  uint8    u_Buffer[MAX_NMEA_SENTENCE_LEN]; /* debug NMEA payload */
  uint32   dwLength;    /* length of payload including the null termination */
  uint32   dwReserved;  /* reserved for future use */
} sm_DebugNMEA;

typedef struct
{
  uint16            w_Prn;
  gps_EphStructType z_GpsEphemeris;
} sm_GpsEphemeris;

typedef struct
{
  uint16            w_SlotId;
  glo_EphStructType z_GloEphemeris;
} sm_GloEphemeris;

typedef struct
{
  uint16            w_Prn;
  bds_EphStructType z_BdsEphemeris;
} sm_BdsEphemeris;

typedef struct
{
  uint16            w_Prn;
  gal_EphStructType z_GalEphemeris;
} sm_GalEphemeris;

typedef enum
{
  SM_LOC_LTE_OTDOA_PDAPI_EVENT_OTDOA_START, /* Start event when OTDOA is started */
  SM_LOC_LTE_OTDOA_PDAPI_EVENT_OTDOA_STOP   /* Stop event when OTDOA is stopped */
} sm_loc_lte_otdoa_pdapi_event_type;

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/* Type def PDSM definitions to SM API type for GNSS SV INFO structure for PE's use*/
typedef pdsm_SvPosSrcEnumType sm_SvPosSrcEnumType;
typedef pdsm_SvHealthEnumType sm_SvHealthEnumType;
typedef pdsm_SvInfoStructType sm_SvInfoStructType;
typedef pdsm_constellation_e_type sm_GnssSystemEnumType;
typedef pdsm_GnssSvInfoStructType sm_GnssSvInfoStructType;

/* Type def PDSM definition for Clock Process Noisde structure for PE's use*/
typedef pdsm_GnssClkModelParamType sm_GnssClkModelStructType;
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

typedef enum
{
  SM_LFM_FEATURE_MIN      =0,
  SM_LFM_FEATURE_SV_POLY  =1,         /*SV POLY reporting enable/disabled*/
  SM_LFM_FEATURE_OEMDRE   =2,         /*OEM DRE enable/disable*/
  SM_LFM_FEATURE_MAX,
}sm_LFMFeatureEnumType;

typedef struct
{
  sm_LFMFeatureEnumType e_featureType;
  boolean v_status;
}sm_LFMConfigUpdateStructType;

/*  Struct for PDR Magnetometer Calibrated State */
typedef struct
{
  boolean v_MagCalState;
} sm_MagCalStateStructType;

/*---------------------------------------------------------------------------
                      GPS Presc Dwell LOG TYPE
---------------------------------------------------------------------------*/
/* -------------------------------------------------------------------
** NAME : LOG_GPS_PRESCRIBED_DWELL_RESULT
**
** DESCRIPTION : It is logged everytime there's presc dwell result avaialable.
**
** PERIODICITY : Async. Depends on dwell duration.
** ------------------------------------------------------------------- */

typedef PACKED struct PACKED_POST
{
  uint8  u_Version;
  uint8  u_RespType; /* 0 Dwell Results
                      * 1 Incoh Sums
                      * 2 Coh Sums
                      * 3-255 Reserved
                      */

  uint32  q_TotalPackets;
  uint32  q_CurrPacketNum;
} sm_PrescDwellCommon;

typedef PACKED struct PACKED_POST
{
  uint16  w_Sv;
  uint32  q_GpsRtc;
  uint8   u_Flags;
  uint8   u_SrchModeIndex;
  uint16  w_PostDetN;
  uint16  w_CnoDb;
  uint32  f_SubMs;
  uint32  f_DopplerFreq;
  uint32  f_MultipathEst;
  uint32  q_Reserved0;
} sm_PrescDwellResult;

/* Final struct defining the DM log packet */
LOG_RECORD_DEFINE(LOG_GPS_PRESCRIBED_DWELL_RESULT_C)

sm_PrescDwellCommon z_PrescDwellCommon;
sm_PrescDwellResult z_PrescDwellResult;
LOG_RECORD_END

   typedef LOG_GPS_PRESCRIBED_DWELL_RESULT_C_type sm_PrescDwellLogType;



/*---------------------------------------------------------------------------
                      GPS DEMOD SOFT DECISIONS LOG TYPE
---------------------------------------------------------------------------*/

/* -------------------------------------------------------------------
** NAME : log_gps_demod_soft_decisions_type
**
** DESCRIPTION : It is logged everytime GPS searcher accumulates
** 20 soft DPSK decisions from firmware.
**
** PERIODICITY : every 20 bits which is approximately 400ms for each
**   active channel.
** ------------------------------------------------------------------- */

LOG_RECORD_DEFINE(LOG_GPS_DEMOD_SOFT_DECISIONS_C)

uint8 u_VersionId;        /* The version of this log type */

uint16 w_Sv;               /* SV PRN number */

uint16 w_DemodId;         /* This ID will be used to correlate across demod-
                          ** related logs that come from the same channel. It
                          ** is incremented everytime any channel is newly started.
                          ** It has a different purpose than SV PRN number which
                          ** one might think can be used, but if let's say a
                          ** channel is aborted due to tuneback, we want to
                          ** distinguish between data coming from channel before
                          ** tuneback or after tuneback (since they have the same
                          ** SV PRN number) */

uint32 q_BitId;           /* This ID will be used to enumerate this log, it
                          ** serves two purposes:
                          ** - to detect dropped packet of this log
                          ** - in addition to task id, to further correlate
                          **   this log with other demod related logs
                          ** It represents how many GPS bits have passed since
                          ** the start of the current demod session */

/* Soft DPSK decisions straight from firmware, untruncated */
int16 x_SoftDecisions[NUM_SOFT_DECISIONS];

uint8 u_NumBits;          /* Number of bits in the array */

LOG_RECORD_END

   typedef LOG_GPS_DEMOD_SOFT_DECISIONS_C_type log_gps_demod_soft_decisions_type;

/* for engine error recovery report, QMI header file gnss_cqsvc.h uses it
   original struct defined in position_determination_service_v01.h  */
typedef  cqsvc_engine_err_recovery_rpt_ind_msg gnss_engine_err_recovery_rpt;

/* -----------------------------------------------------------------------*//**
@brief
This callback function is called by TM once PDAPI is intialized. This is 
used to notify of potential clients that PDAPI clients can now initialize. 
*/ /* ------------------------------------------------------------------------*/
typedef void (*sm_ApiInitNotifierCb)
   (
   void
   );

typedef pdsm_pd_ext_dpo_status_rpt sm_DpoStatusReportStructType;

/*---------------------------------------------------------------------------
                      LPPM Status Report
---------------------------------------------------------------------------*/
#define SM_LPPM_GNSS_TYPE_MAX (MGP_GNSS_TYPE_GAL + 1)

typedef struct {
  mgp_GnssType e_ConstellationType;
  /**<   Constellation type.
      Valid Values :
      - MGP_GNSS_TYPE_GPS (0) --  Constellation type is GPS.
      - MGP_GNSS_TYPE_GLO (1) --  Constellation type is GLONASS.
      - MGP_GNSS_TYPE_BDS (2) --  Constellation type is Beidou.
      - MGP_GNSS_TYPE_GAL (3) --  Constellation type is Galileo.    */

  uint32 q_RequiredSvOrbitalModelBitMask;
  /**<   List of required SVs' orbital model (bit mask), one bit per-SV.
       Valid values for each of 32 bits:
         0 -- SV's orbital model not required.
         1 -- SV's orbital model required.
  */

  uint32 q_AvailableSvOrbitalModelBitMask;
  /**<   List of available SVs' orbital model (bit mask), one bit per-SV.
       Valid values for each of 32 bits:
         0 -- SV's orbital model not available.
         1 -- SV's orbital model available.
  */
} sm_LppmConstellationDataStructType;

/** LPPM status structure */
typedef struct {

  /* Flag to indicate if LPPM can engage.
     When already engaged, this flag will always return TRUE. */
  boolean v_EngagePossible;

  /* Current LPPM State */
  gnss_RcvrLppmStateEnumType e_State;
  /**<   LPPM state.
  Valid Values :
      - GNSS_RCVR_STATE_LPPM_NONE (0) -- disabled.
      - GNSS_RCVR_STATE_LPPM_WAITING_CLOCK (1) -- enabled but waiting for clock.
      - GNSS_RCVR_STATE_LPPM_DISENGAGED (2) -- enabled and disengaged.
      - GNSS_RCVR_STATE_LPPM_INIT (3) -- enabled and engaged in initialization mode.
      - GNSS_RCVR_STATE_LPPM_HIGH_UNC (4) -- enabled and engaged with high uncertainty.
      - GNSS_RCVR_STATE_LPPM_LOW_UNC (5) -- enabled and engaged with low uncertainty. */

  /*  Time Uncertainty */
  float f_TimeUncMs;
  /**<   Time uncertainty.
       - Type: Floating point
       - Units: Milliseconds */

  /*  Constellation Data For LPPM */
  uint32 q_ConstellationDataLen;  /**< Must be set to # of elements in constellationData */
  sm_LppmConstellationDataStructType z_ConstellationData[SM_LPPM_GNSS_TYPE_MAX];

  /* MGP LPPM configuration */
  boolean v_IsQueryResponse;
  mgp_LppmConfigStructType z_LppmConfig;

} sm_LppmStatusStructType;

/*************************************************************************
  GNSS LE Reporting Data-Structures
*************************************************************************/
/** GNSS LE Reporting Enum */
typedef enum
{
  /*GNSS LE reporting Off*/
  SM_GNSS_LE_RPT_OFF,
  /*GNSS LE reporting On*/
  SM_GNSS_LE_RPT_ON,
  SM_GNSS_LE_RPT_MAX
} sm_GnssLEReportingEnumType;

/** GNSS LE Reporting Error Code Enums */
typedef enum
{
  /* No error */
  SM_GNSS_LE_RPT_STATUS_ERROR_NONE,
  /* MGP session is inactive */
  SM_GNSS_LE_RPT_STATUS_ERROR_SESSION_INACTIVE,
  /* GNSS LE reporting already On */
  SM_GNSS_LE_RPT_STATUS_ERROR_ALREADY_ON,
  SM_GNSS_LE_RPT_STATUS_ERROR_MAX
}sm_GnssLERptStatusErrorEnum;

/**GNSS LE Reporting Status Structure*/
typedef struct {
  sm_GnssLEReportingEnumType e_State;
  sm_GnssLERptStatusErrorEnum e_Error;
} sm_GnssLERptStatusStructType;

/*************************************************************************
  Sensor LE Data-Structures
*************************************************************************/

/** Sensor Relative Displacement Status */
typedef enum
{
  /* Relative Displacement is Valid*/
  SM_SENSOR_REL_DISP_STATUS_SUCCESS,
  /*Relative Displacement is not valid due to an Error*/
  SM_SENSOR_REL_DISP_STAUS_ERROR,
  SM_SENSOR_REL_DISP_STATUS_MAX = CGPS_MAX_UINT32
}sm_SensorLERelDispStatusEnum;

/** Sensor LE Relative Displacement Struct */
typedef struct
{
  /*Modem timestamp of the begining of the diplacement interval*/
  uint64 t_ModemTsFromMsec;
  /*Modem timestamp of the end of the diplacement interval*/
  uint64 t_ModemTsToMsec;
  /*Displacement Status Enum*/
  sm_SensorLERelDispStatusEnum e_Status;
  /*North, East and Vertical Displacement in meters
    [0]: North Displacement
    [1]: East Displacement
    [2]: Vertical Displacement*/
  FLT f_EnuDisp[3];
  /*Displacement Error
    [0]: Displacement error ellipse semi-major axis. Units: meters
    [1]: Displacement error ellipse semi-minor axis. Units: meters
    [2]: Displacement error ellipse angle. Units: Radians*/
  FLT f_DispError[3];
  /*Displacement Vertical Uncertainity*/
  FLT f_DispVertUnc;
}sm_SensorLERelDispStructType;

/** Sensor LE State Enum */
typedef enum
{
  /*Sensor LE State Off*/
  SM_SENSOR_LE_STATE_OFF,
  /*Sensor LE State On*/
  SM_SENSOR_LE_STATE_ON,
  SM_SENSOR_LE_STATE_MAX
} sm_SensorLEStateEnumType;

/** Sensor LE Status Error Code Enums */
typedef enum
{
  /* No error */
  SM_SENSOR_LE_STATUS_ERROR_NONE,
  /* MGP session is inactive */
  SM_SENSOR_LE_STATUS_ERROR_SESSION_INACTIVE,
  /* Sensor LE already On */
  SM_SENSOR_LE_STATUS_ERROR_ALREADY_ON,
  /* Sensor LE insufficient memory */
  SM_SENSOR_LE_STATUS_ERROR_NO_MEMORY,
  SM_SENSOR_LE_STATUS_ERROR_MAX
}sm_SensorLEStatusErrorEnum;

/**Sensor LE State Structure*/
typedef struct {
  sm_SensorLEStateEnumType e_State;
  sm_SensorLEStatusErrorEnum e_Error;
} sm_SensorLEStatusStructType;

typedef enum
{
  INVALID_INCONSISTENCY = 0,

  /* SM Reported Inconsistencies */
  SM_TIME_IN_PAST_WRT_XTRA_INCONSISTENCY,
  SM_TIME_IN_PAST_WRT_ROM_ALMANAC_INCONSISTENCY,
  SM_TIME_IN_PAST_WRT_BUILD_INCONSISTENCY,
  SM_INCONSISTENCY_LAST = SM_TIME_IN_PAST_WRT_BUILD_INCONSISTENCY,
  SM_INCONSISTENCY_MAX = 0xFF, 

  /* PE Reported Inconsistencies */
  PE_INJECTED_COARSE_POSITION_INCONSISTENCY,
  PE_INJECTED_TDP_POSITION_INCONSISTENCY,
  PE_INJECTED_GTP_WIFI_POSITION_INCONSISTENCY,
  PE_INJECTED_REFLOC_WITH_GNSS_INCONSISTENCY,
  PE_INJECTED_REFLOC_WITH_CPI_INCONSISTENCY,
  PE_EPHEMERIS_INCONSISTENCY,
  PE_ALMANAC_INCONSISTENCY,
  PE_XTRA_INCONSISTENCY,
  PE_NAV_DATA_OUT_OF_BOUNDS_INCONSISTENCY,
  PE_PARTIAL_ERROR_RECOVERY_INCONSISTENCY,
  PE_INCONSISTENCY_LAST = PE_PARTIAL_ERROR_RECOVERY_INCONSISTENCY,
  PE_INCONSISTENCY_MAX = 0xFFF,

  /* ME Reported Inconsistencies */
  ME_INJECTED_TIME_INCONSISTENCY,
  ME_SUBFRAME_MISALIGNMENT_INCONSISTENCY,
  ME_DECODED_TIME_INCONSISTENCY,
  ME_BACKGROUND_SCAN_ERROR_INCONSISTENCY,
  ME_LEAP_SECOND_INCONSISTENCY,
  ME_WEEK_NUMBER_INCONSISTENCY,
  ME_UTC_DATA_INCONSISTENCY,
  ME_A_S_BIT_INCONSISTENCY,
  ME_INCONSISTENCY_LAST = ME_A_S_BIT_INCONSISTENCY,
  ME_INCONSISTENCY_MAX = 0xFFFF,
}sm_InconsistencyEnumT;

/**************************************************/
/**************************************************
PE Inconsistencies 
**************************************************/
typedef struct
{
  double d_BaselineHepe2;
  double d_InjectedHepe2;
  double d_DistBtwPos2;
  uint8 u_InjectedPosIdx;
  uint8 u_BaselinePosIdx;
  uint8 u_DeletedPosIdx;
  uint8 u_BaselinePosRel;
  uint8 u_InputPosRel;
}sm_PosInconsistencyRepT;

typedef struct
{
  uint8 u_InputNavDataType;
  uint16 w_SvId;
  uint8 u_ExistingNavDataType;
  double d_ClockDiff;
  double d_ClockDiffThresh;
  double d_PosDiff;
  double d_PosDiffThresh;
  boolean v_IsInputDeleted;
  uint32 q_ExistingNavDelMask;
}sm_AlmEphXtraRepT;

/* Nav data Bound Check Failure Reporting structure */
typedef struct
{
  uint16 w_SvId;
  gnss_NavDataParamEnumType e_ParamType;
  DBL d_ParamValue;
  DBL d_LowerBound;
  DBL d_UpperBound;
}sm_OutOfBoundsNavDataRepT;

typedef struct
{
  uint8 u_ErrorRecoveryType;
  gnss_MeasIdMaskStructType z_MsecErrSvMask;
  double *pd_MsecErrPrRes; //pd_MsecErrPrRes[]
  uint8 u_LenMsecErrPrRes;
  uint32 q_WlsErrorSvMask;
  double d_DistBtwWlsInitFixPos;
  double d_WlsInitPosUnc;
  double d_WlsFixPosUnc;
}sm_PartialErrRecoveryRepT;

/**************************************************
ME Inconsistencies
**************************************************/
typedef struct
{
  uint8 u_InjectedTimeSrc;      /* C_TIME_SRC_* defined in gps_common.h */
  uint16 w_InjectedGpsWeek;
  uint32 q_InjectedGpsMsec;
  FLT f_InjectedTuncMsec;

  uint8 u_BaseTimeSrc;
  uint16 w_BaseGpsWeek;
  uint32 q_BaseGpsMsec;
  FLT f_BaseTuncMsec;

  uint32 q_MsecOffset;
  uint8 u_ActionTaken;
}sm_InjectedTimeIncRepT;

typedef struct
{
  uint8 u_GnssSrc;
  uint16 w_SvId;
  uint8 u_BaseTimeSrc;
  uint16 w_BaseGpsWeek;
  uint32 q_BaseGpsMsec;
  FLT f_BaseTuncMsec;
  uint8 u_ActionTaken;
}sm_SubFramMisAlignIncRepT;

typedef struct
{
  uint8 u_GnssType; /* GNSS type */
  uint16 w_SvId; /* For GLO, this shall be slot ID 
                     255 if slot ID is unknown */
  int8   b_GloFreqId; /* Freq # for GLO SV and not valid of non-GLO */
  uint32 q_DecodedMsec;
  FLT f_DecodedTuncMsec;
  uint8 u_BaseTimeSrc;
  uint32 q_BaseMsec;
  FLT f_BaseTuncMsec;
  uint32 q_MsecOffset; /* Abolsute diff btw decoded & baseline */
  uint64 t_ListSvsDetected;
  uint8 u_ActionTaken;
}sm_DecodedTimeIncRepT;

typedef struct
{
  uint8 u_GnssType;
  uint16 w_SvId; /* For GLO, this shall be slot ID 
                     255 if slot ID is unknown */
  int8 b_GloFreqId;
  uint16 w_Cn0Diff; /* in 0.1 of dBhz */
  uint16 w_AbsCodePhaseDiff; /* in usec */
  int16 x_SpeedDiff; /* in m/sec */
  uint8 u_PriorPeakSteerSrc; /* Source of prior peak which compared
                                against background scan peak
                                FALSE - measurement
                                TRUE - steering */
  uint8 u_ActionTaken;
}sm_BGScanErrorIncRepT;

typedef struct
{
  uint16 w_SvId; /* For GLO, this shall be slot ID 
                     255 if slot ID is unknown */
  int8 b_GloFreqId;
  uint8 u_BaseLs;
  uint8 u_BaseLsSrc;
  uint8 u_BaseTimeSrc;
  uint16 w_BaseGpsWeek;
  uint32 q_BaseGpsMsec;
  FLT f_BaseTuncMsec;
  uint8 u_CalculatedLs;
  uint8 u_ActionTaken;
}sm_LeapSecIncRepT;

typedef struct
{
  uint8  u_FlaggedGpsWeekSrc;
  uint16 w_FlaggedGpsWeek;
  uint8 u_BaseTimeSrc;
  uint16 w_BaseGpsWeek;
  uint32 q_BaseGpsMsec;
  uint32 f_BaseTuncMsec;
  uint8 u_ActionTaken;
}sm_WeekNumberIncRepT;

typedef struct
{
  uint8 u_InjectedUtcSrc;
  gps_UtcInfoRptStructType z_InjectedUtc;
  uint32 q_CauseOfInconsistencies;
  uint8 u_ActionTaken;
}sm_UtcDataIncRepT;

typedef struct
{
  uint32 q_SvBitmask;
}sm_AntiSpoofBitIncRepT;

/**************************************************/
typedef union
{
  /* PE Inconsistencies */
  sm_PosInconsistencyRepT z_PosIncR;
  sm_AlmEphXtraRepT z_AlmEphXtraIncR;
  sm_OutOfBoundsNavDataRepT z_OOBNavDataIncR;
  sm_PartialErrRecoveryRepT z_PartialErrRecoveryIncR;

  /* ME Inconsistencies */
  sm_InjectedTimeIncRepT z_InjectedTimeIncR;
  sm_SubFramMisAlignIncRepT z_SFMisalignIncR;
  sm_DecodedTimeIncRepT z_DecodedTimeIncR;
  sm_BGScanErrorIncRepT z_BGScanErrorIncR;
  sm_LeapSecIncRepT z_LeapSecIncR;
  sm_WeekNumberIncRepT z_GpsWeekNumIncR; /* SM also uses this */
  sm_UtcDataIncRepT z_UtcDataIncR;
  sm_AntiSpoofBitIncRepT z_ASBitIncR;
} sm_ReportablesUnionT;

typedef struct
{
  sm_InconsistencyEnumT e_Type;
  uint64 t_TimetickMsec;
  sm_ReportablesUnionT y_Reportables;
} sm_InconsistencyDataT;

/*Structure to request IPC events to LM*/
typedef enum
{
 SM_REQ_TYPE_GET_NAVIC_EPH_ASSIST,
 SM_REQ_TYPE_CANCEL_NAVIC_EPH_ASSIST,
 SM_REQ_TYPE_NAVIC_ASSIST_MAX
} sm_XtraEnumType;

typedef struct
{
  sm_XtraEnumType e_XtraType;
} sm_ReqXtraStructType;

/*=============================================================================

       Functions

=============================================================================*/

/**
 * This function sends the GNSS Measurements obtained by PE to
 * SM. PS Measurements are sent out periodically for NMEA
 * sentence generation and for diagnostic reasons (periodicity
 * is set by SM). They are also sent when measurement processing
 * is complete in 1x MS-Assisted mode of operation. The
 * measurement status report indicates the reason for sending
 * the measurements.
 * @param meas_ptr
 */
void sm_ReportGnssMeasInfo(const gnss_MeasBlkStructType *meas_ptr);

/**
* This function sends the GNSS NHz Measurements from PE to SM
* @param meas_ptr
*/
void sm_ReportGnssMeasInfoNHz(const gnss_MeasBlkStructType *meas_ptr);

/**
  @brief   This function sends the GPS Measurements obtained by ME to SM.

  This function sends the Receiver State change event report to
  SM
  
  @return      
  @sideeffects None
  @see         
*/
void sm_ReportRcvrStateChange(
   uint32 q_GpsRtcMs,
   /**< GNSS RTC msec at which this report was generated.  */
   gps_RcvrStateEnumType e_CurRcvrState,
   /**< new receiver state */
   const gnss_RcvrSubStateStructType *pz_CurRcvrSubState,
   /**< new receiver sub-state */
   uint8 g_RcvrStateErrorInd
   /**< ME status error indicator */

   );


/**
  @brief  This function sends the GNSS SV poly report calculated by
          PE to SM. GNSS SV poly reports are sent periodically
          once MGP is turned ON

  @return
  @sideeffects None
  @see

*/
void sm_ReportSVPolynomialGNSS(const gnss_SvPoly *p_GnssSvPolyRpt);

/**
  @brief  This function sends the GPS Fix report calculated by
          PE to SM. GPS FIX/NO-FIX reports are sent periodically
          once MGP is turned ON. 
          This function is also used to send BestAvail Pos, Wifi fix,
          Pos estimatin, etc to SM.
  
  @return      
  @sideeffects None
  @see 

*/
void sm_ReportFix(const sm_FixReportStructType *p_FixRpt
                  /**< Pointer to the structure containing the GPS Fix data */);

/*=============================================================================

FUNCTION
  sm_ReportUtcInfo

DESCRIPTION
  This function sends the UTC data decoded by PE off the air to SM.

DEPENDENCIES
  None.

PARAMETERS
  p_UtcInfo - Pointer to the structure containing the UTC info

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportUtcInfo
(
   const sm_UtcInfoRptStructType *p_UtcInfo
   );

/**
* @brief  PE indicates to SM the status of current assistance
*         data and wish-list using this API
  
  @return    
  @sideeffects None
  @see 

*/
void sm_ReportGnssAssistDataStatus(const sm_GnssAssistDataStatusStructType *p_AssistDataInfo);

/**
  @brief  ME and PE typically inform SM about events of interest by calling this function.
          SM may call other APIs in response to an event of interest
  
  @return      
  @sideeffects None
  @see         
*/
void sm_ReportEvent(const sm_ReportEventStructType *p_ReportEvent
                    /**< payload denoting event of interest */);

/**
  @brief  This function sends RF Info data from PGI to SM
  
  @return      
  @sideeffects None
  @see
  @todo add more meaningful parameter info
*/
void sm_ReportRfInfo(const sm_ReportRfInfoStructType *p_ReportRfInfo
                     /**< Pointer to the structure containing RF Info data. */);


/**
* @brief  ME calls this function to request SA (assistance) data
*         from the network
  
* @return      void
  @sideeffects None
  @see


*/
void sm_RequestSAData(void);

/*=============================================================================

FUNCTION
  sm_StoreFtcalTTData

DESCRIPTION
  This function sends TT data from MC to SM.

DEPENDENCIES
  None.

PARAMETERS
  cgps_FtcalTTStruct 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_StoreFtcalTTData(cgps_FtcalTTStruct *p_TTData);

/*=============================================================================

FUNCTION
  sm_ReportWBIQInfo

DESCRIPTION
  This function sends Wide Band IQ Info data from MC to SM.

DEPENDENCIES
  None.

PARAMETERS
  p_ReportWBIQInfo - Pointer to the structure containing WBIQ params

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportWBIQInfo(const sm_ReportWBIQInfoStructType *p_ReportWBIQInfo);

/*=============================================================================

FUNCTION
  sm_ReportNBIQInfo

DESCRIPTION
  This function sends Narrow Band IQ Info data from MC to SM.

DEPENDENCIES
  None.

PARAMETERS
  p_ReportNBIQInfo - Pointer to the structure containing WBIQ params

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportNBIQInfo(const sm_ReportNBIQInfoStructType *p_ReportNBIQInfo);


/*=============================================================================

FUNCTION
  sm_ReportGNSSRfStatusInfo

DESCRIPTION
  This function sends GNSS RF status Info data from MC to SM.

DEPENDENCIES
  None.

PARAMETERS
  p_ReportGNSSRfInfo - Pointer to the structure containing GNSS RF status
  information 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGNSSRfStatusInfo(const sm_ReportGNSSRfStatusInfoStructType *p_ReportGNSSRfInfo);

#ifdef FEATURE_CGPS_UMTS_CELLDB
/*=============================================================================

FUNCTION
  sm_UpdateCellIDInfo

DESCRIPTION
  This function sends the latest Cell ID information to SM. This information
  is maintained in MGP and is updated to SM whenever there is a cell change
  or when new Cell ID is received from RR/RRC.
 
  Latest Cell ID changes will come from Call Manager instead of MGP, but this
  API can be used to inject Cell ID change to SM (like in SMART)

DEPENDENCIES
  None.

PARAMETERS
  e_TriggerRAT - RAT that triggered the cell db update
  p_CellInfoCached - Pointer to the Cached Cell ID information.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_UpdateCellIDInfo(const cgps_ActiveRATEnumType e_TriggerRAT, const cgps_CellInfoCachedStruct *const p_CellInfoCached);
#endif /* FEATURE_CGPS_UMTS_CELLDB */

/**
  @brief  Delivers estimate of current time to SM. PE calls this
          API in response to a request from SM via
          mgp_ReqGnssTime (to be defined by MGP).
  
  @return  
  @sideeffects None
  @see 

*/
void sm_ReportGnssTime(const sm_ReportGnssTimeStructType *p_ReportGnssTime/**< gnss time struct */);


/*===========================================================================
FUNCTION sm_send_message

DESCRIPTION
  Function to send an IPC message to SM threads

RETURN VALUE
 ipc_msg_ptr = If IPC message was send successfully

DEPENDENCIES
  None

=============================================================================*/

os_IpcMsgType* sm_send_message(uint32 msg_id, uint32 size, uint32 thread, const void *data);

#ifdef FEATURE_CGPS_XTRA_T
/*=============================================================================

FUNCTION
  sm_ReportCellData

DESCRIPTION
  This function sends cell data from MGP to SM.

DEPENDENCIES
  None.

PARAMETERS
  sm_ReportCellDataStructType - Pointer to the structure containing cell data

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportCellData(const sm_ReportCellDataStructType *const p_ReportCellData);
#endif /* FEATURE_CGPS_XTRA_T */

/*=============================================================================
FUNCTION
  sm_ReportPrescDwellResult

DESCRIPTION
  This function gets called when a presc dwell log is available.

DEPENDENCIES
  None.

PARAMETERS
  p_PrescDwellResult - Pointer to the structure containing presc dwell result

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportPrescDwellResult(const sm_PrescDwellLogType *p_PrescDwellResult);

/*=============================================================================

FUNCTION
  sm_ReportDemodSoftDecisions

DESCRIPTION
  This function gets called when demod soft-decision log is available.

DEPENDENCIES
  None.

PARAMETERS
  p_DemodResult - Pointer to the structure containing demod soft decision data

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportDemodSoftDecisions(const log_gps_demod_soft_decisions_type *p_DemodResult);

/*=============================================================================

FUNCTION
  sm_GetGpsUtcOffset

DESCRIPTION
  This function gets UTC offset configured in the NV.

DEPENDENCIES
  None.

PARAMETERS
  p_UtcOffset - Pointer to the UTC offset value

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void  sm_GetGpsUtcOffset(uint8 *p_UtcOffset);

/*=============================================================================

FUNCTION
  sm_EnableSensorLPM

DESCRIPTION
  This function turns on sensor low power mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void  sm_EnableSensorLPM(void);

/*=============================================================================

FUNCTION
  sm_DisableSensorLPM

DESCRIPTION
  This function turns off sensor low power mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void  sm_DisableSensorLPM(void);

/*=============================================================================

FUNCTION
  sm_EnableSensorFPM

DESCRIPTION
  This function turns on sensor low power mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void  sm_EnableSensorFPM(void);

/*=============================================================================

FUNCTION
  sm_DisableSensorFPM

DESCRIPTION
  This function turns off sensor low power mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void  sm_DisableSensorFPM(void);

/**
* @brief  Indicate to SM that it should request external SPI information
  
  @return    
  @sideeffects None
  @see 

*/
void sm_XspiStartInjection(void);

/**
* @brief  Indicate to SM that it should close the 
          SPI information injection stream
  
  @return    
  @sideeffects None
  @see 

*/
void sm_XspiStopInjection(void);

/*=============================================================================

FUNCTION
  sm_ReportDrSyncPulseDescriptor

DESCRIPTION
  

DEPENDENCIES
  None.

PARAMETERS
  

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportDrSyncPulseDescriptor(gnss_DRSyncPulseDescType *pDrSyncPulseDesc);

/*=============================================================================

FUNCTION
  sm_ReportDrSyncConfig

DESCRIPTION


DEPENDENCIES
  None.

PARAMETERS


RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportDrSyncConfig(gnss_DRSyncConfigType *pDrSyncConfig);

/*=============================================================================

FUNCTION
  sm_ReportDebugNMEA

DESCRIPTION
  Accepts formatted proprietary NMEA string for debug purpose.
  NMEA string is sent as IPC to the LM task by this API

DEPENDENCIES
  None.

PARAMETERS
  sm_DebugNMEA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportDebugNMEA(sm_DebugNMEA *pDebugNMEA);

/*=============================================================================

FUNCTION
  sm_RegisterApiNotifierCb

DESCRIPTION
  Called by potential clients to register their callbacks. The callbacks are 
  used to notify the clients that the API/Engine is now ready to accept clients

  If the API/Engine is ready to accept clients, the callback will be called 
  inline.  The caller should be ready to handle this and queue an IPC to itself. 

DEPENDENCIES
  None.

PARAMETERS
  pNotifierCb - The callback of the client to be registered

RETURN VALUE
  TRUE:  registration has succeeded, or if Engine is ready the callback is 
         called immediately;
  FALSE:  registration failed

SIDE EFFECTS
  None.

=============================================================================*/
boolean sm_RegisterApiNotifier(sm_ApiInitNotifierCb pNotifierCb, os_ThreadIdEnumType clientThreadId);

/*=============================================================================

FUNCTION
  sm_ReportInjDone

DESCRIPTION
  Indication from CD that xtra or xtra integrity injection was completed 
  successfully. 

DEPENDENCIES
  None.

PARAMETERS
gnss_XtraInjMaskType: The mask to indicate the updated constellations.
uint8               : 0 for XTRA or 1 for XTRA Integrity.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportInjDone( gnss_XtraInjMaskType q_AcceptedMask, 
                       uint8 u_XtraInjectType );

/*=============================================================================

FUNCTION
  sm_ReportResetLocationServiceDone

DESCRIPTION
  Indication from MGP that location service reset is complete.
  MGP may have succesfully reset or partially reset.
  A status is passed to this function by MGP to let SM know the status of the
  reset location service.

DEPENDENCIES
  None.

PARAMETERS
 status: Status of reset location service.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportResetLocationServiceDone(pdsm_pd_ext_reset_location_service_status_e_type status);
/*=============================================================================

FUNCTION

  sm_SendEngineErrorRecoveryReport

DESCRIPTION
  ME will call this API to send engine error recovery report to SM so it'll be
  forwarded to QMI by calling gnss_CQmiSvcSendEngineErrorRecoveryReport()
  No sanity check will be performed for the error recovery report from ME

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_SendEngineErrorRecoveryReport(gnss_engine_err_recovery_rpt *pErrReport);

/*=============================================================================

FUNCTION
  sm_DpoStatusReport

DESCRIPTION
  Function to report periodic DPO status report.
  When DPO status reporting is enabled by the application, this function
  will be called at a nominal rate of 1 Hz. 

DEPENDENCIES
  None.

PARAMETERS
  Information about DPO disengagement from MGP to SM . 

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_DpoStatusReport(sm_DpoStatusReportStructType *p_DpoStatusReport);

/*=============================================================================

FUNCTION
  sm_ReportGpsEphemeris

DESCRIPTION
  This function sends a GPS ephemeride from PE to SM.

DEPENDENCIES
  None.

PARAMETERSa
  w_Prn    - PRN of ephemeride being returned
  pz_GpsEph - Pointer to the ephemeris information

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGpsEphemeris(const uint16 w_Prn, const gps_EphStructType *pz_GpsEph);

/*=============================================================================

FUNCTION
  sm_ReportQzssEphemeris

DESCRIPTION
  This function sends a QZSS ephemeris from PE to SM.

DEPENDENCIES
  None.

PARAMETERSa
  u_Prn    - PRN of ephemeride being returned
  pz_GpsEph - Pointer to the ephemeris information

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportQzssEphemeris(const uint8 u_Prn, const gps_EphStructType *pz_GpsEph);


/*=============================================================================

FUNCTION
  sm_ReportGloEphemeris

DESCRIPTION
  This function sends a GLO ephemeride from PE to SM.

DEPENDENCIES
  None.

PARAMETERSa
  w_SlotId  - Slot ID of ephemeride being returned
  pz_GloEph - Pointer to the ephemeris information

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGloEphemeris(const uint16 w_SlotId, const glo_EphStructType *pz_GloEph);

/*
******************************************************************************
* sm_SvSframePut
*
* Function description:
*
*    This function is called by PE to provide a decoded subframe to PE.
*
* Parameters:
*
*  p_SvSframe -  Pointer to SV subframe structure
*
* Return value:
*
*  void
*
* ******************************************************************************
*/
void sm_SvSubframePut(const gnss_SvSubframeStruct *p_SvSubframe);

/*
******************************************************************************
* sm_SvGalPagePut
*
* Function description:
*
*    This function is called by PE to provide a decoded Galileo Page to PE.
*
* Parameters:
*
*  p_SvSframe -  Pointer to SV GAL Page structure
*
* Return value:
*
*  void
*
* ******************************************************************************
*/
void sm_SvGalPagePut(const gnss_GalPageStruct *p_SvGalPage);

/*
******************************************************************************
* sm_GloStringPut
*
* Function description:
*
*  This function is called by PE to provide a decoded GLONASS string to PE.
*
* Parameters:
*
*  pz_Str -  input, Pointer to a GLONASS string
*
* Return value:
*
*  void
*
* ******************************************************************************
*/
void sm_GloStringPut(const gnss_GloStringStruct *pz_Str);


/*===========================================================================
  sm_loc_lte_ecid_recv_measurements

  Description:

     This function is used by LOC_LTE_ECID module to send the ECID measurements
 Parameters:
    Reason: stop reason

  Return value: 
    boolean: SUCCESS or FAIL
=============================================================================*/
boolean sm_loc_lte_ecid_recv_measurements(
   gnss_lpp_ecid_meas_info_type *p_ecid_measurement_info);

/*=============================================================================

FUNCTION
  sm_ReportBdsEphemeris

DESCRIPTION
  This function sends a BDS ephemeride from PE to SM.

DEPENDENCIES
  None.

PARAMETERSa
  w_Sv      - SV ID of ephemeride being returned
  pz_BdsEph - Pointer to the ephemeris information

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportBdsEphemeris(const uint16 w_Sv, const bds_EphStructType *pz_BdsEph);


/*=============================================================================

FUNCTION
  sm_ReportGalEphemeris

DESCRIPTION
  This function sends a GAL ephemeride from PE to SM.

DEPENDENCIES
  None.

PARAMETERSa
  w_Sv      - SV ID of ephemeride being returned
  pz_GalEph - Pointer to the ephemeris information

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGalEphemeris(const uint16 w_Sv, const gal_EphStructType *pz_GalEph);


/*===========================================================================
  sm_InitGnssNavPosition

  Description:

     Initalizes a sm_NavPositionStructType safely

 Parameters:

   pz_Pos:  pointer to the struct to initialize

  Return value: 
    boolean: true if successfully initialized

=============================================================================*/
boolean sm_InitGnssNavPosition(sm_NavPositionStructType *pz_Pos);


/*=============================================================================

FUNCTION
  sm_ReportCPEMeas

DESCRIPTION
  Report CPE meas from ME 

DEPENDENCIES
  None.

PARAMETERS
 p_CPEMeas - Pointer to CPE Measurement Block

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportCPEMeas(gnss_CPEMeasBlockStructType *p_CPEMeas);

/*=============================================================================

FUNCTION
  sm_UpdateCPETimingControl

DESCRIPTION
  This updates SM about the CPE timing control that is read by NV

DEPENDENCIES
  None.

PARAMETERS
  q_CPEControl - CPE Control Value

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_UpdateCPETimingControl(uint32 q_CPEControl);

/*===========================================================================
  sm_ReportMgpLppmStatus

  Description:

     This function is used by MGP to report LPPM status to SM.

 Parameters:

   pz_LppmStatus:  pointer to the LPPM status struct

  Return value: 
    void

=============================================================================*/
void sm_ReportMgpLppmStatus(const sm_LppmStatusStructType *pz_LppmStatus);

/*===========================================================================
  sm_ReportMgpPedStatus

  Description:

     This function is used by MGP to report Pedestrian status to SM.

 Parameters:

   e_PedStatus:  Pedestrian status enum

  Return value: 
    void

=============================================================================*/
void sm_ReportMgpPedStatus(gnss_MgpPedStatusEnumType e_PedStatus);

/*===========================================================================
  sm_ReportGnssQualInd

  Description:

     This function is used by MGP to report GNSS Quality indications to SM.

 Parameters:

   e_GnssQualityInd:  GNSS Quality indication enum

  Return value: 
    void

=============================================================================*/
void sm_ReportGnssQualInd(const gnss_QualIndEnumType e_GnssQualityInd);

/*===========================================================================
  sm_ReportPedDevContext

  Description:

     This function is used by MGP to report Pedestrian Device Context to SM.

 Parameters:

   e_PedDevCtx:  Pedestrian Device Context enum

  Return value: 
    void

=============================================================================*/
void sm_ReportPedDevContext(const slimPedDevCxtTypeEnumT e_PedDevCtx);
/*===========================================================================
  sm_ReportPedAlignAvailInd

  Description:

     This function is used by MGP to report Pedestrian Alignement availibility to SM.

 Parameters:

   v_PedAlignSensorAvailable: whether Pedestrian alignement is available or not

  Return value: 
    void

=============================================================================*/
void sm_ReportPedAlignAvailInd(boolean v_PedAlignSensorAvailable);

/*===========================================================================
  sm_ReportPedSensorAssistAvailInd

  Description:

     This function is used by MGP to report Pedestrian Sensor Assistance
     availibility to SM.

 Parameters:

   v_PedSensorAssistAvailable: whether Pedestrian Sensor Assistance is available or not

  Return value:
    void

=============================================================================*/
void sm_ReportPedSensorAssistAvailInd(boolean v_PedSensorAssistAvailable);

/*===========================================================================
  sm_XlatePedDevCtxFromSlimToGnss

  Description:

     This function is used by translate Pedsestrian Device Context enum from Slim enum to Gnss Enum
 Parameters:

   slimPedDevCxtTypeEnumT: Pedsestrian Device Context Slim enum

  Return value: 
    Pedsestrian Device Context Gmss enum

=============================================================================*/
gnss_PedDevCxtEnumType sm_XlatePedDevCtxFromSlimToGnss(slimPedDevCxtTypeEnumT e_PedDevCtx);

/*=============================================================================

FUNCTION
  sm_ReportLeapSecondInfo

DESCRIPTION
  This function sends a Leap Second Info from ME to SM.

DEPENDENCIES
  None.

PARAMETERS
  mgp_LeapSecondInfoStructType - Pointer to leap second info

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportLeapSecondInfo(const mgp_LeapSecondInfoStructType *pz_LeapSecInfo);

/*=============================================================================

FUNCTION
  sm_NavConfigChangeResp

DESCRIPTION
  This function sends MGP's response to the Nav config change diag command.

DEPENDENCIES
  None.

PARAMETERS
  u_CurrGnssConfig - Current GNSS configuration.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_NavConfigChangeResp(const uint32 q_CurrGnssConfig);

/*=============================================================================

FUNCTION
  sm_ReportNMEAForDebug

DESCRIPTION
  Accepts formatted proprietary NMEA string for debug purpose.
  NMEA string is sent as IPC to the TM task by this API, for forwarding to HLOS
  for debugging purpose.

DEPENDENCIES
  None.

PARAMETERS
  sm_DebugNMEA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportNMEAForDebug(sm_DebugNMEA *pDebugNMEA);

/*===========================================================================
FUNCTION
  sm_ReportUnpropagatedGnssFixInfo

  Description:  This function sends the unpropagated Fix report.
  
Parameters :  Pointer to the structure containing the GPS Fix data

  Return value: None
=============================================================================*/
void sm_ReportUnpropagatedGnssFixInfo(const sm_FixReportStructType *p_GnssFixRpt);

#ifdef FEATURE_LOCTECH_NHZ
/*===========================================================================
  sm_NhzConfigPut

  Description:

     Get the NHz config from SM

 Parameters:

   pq_NhzConfig:  pointer to the config

  Return value: 
    None

=============================================================================*/
void sm_NhzConfigPut(uint32 *pq_NhzConfig);

/*===========================================================================
  sm_NhzDefaultConfigPut

  Description:  Get the NHz default config from SM
  
  Parameters :  None

  Return value: Default config

=============================================================================*/
uint32 sm_NhzDefaultConfigPut();
#endif /* FEATURE_LOCTECH_NHZ */

/*=============================================================================

FUNCTION
  sm_ReportXOOffsetInfo

DESCRIPTION
  This function sends XO offset data to SM.

DEPENDENCIES
  None.

PARAMETERS

  l_RefOffsetPpb - Ref osc offset in PPB
  l_ClkFreqBiasPpb - Clock freq bias in PPB
  l_ClkFreqBiasUncPpb - Clock freq bias uncertainty in PPB

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportXOOffsetInfo(int32 l_RefOffsetPpb,
                           int32 l_ClkFreqBiasPpb,
                           int32 l_ClkFreqBiasUncPpb);

/*=============================================================================

FUNCTION
  sm_ReportVarLenNMEAForDebug

DESCRIPTION
  Accepts formatted proprietary NMEA string for debug purpose.
  NMEA string is sent as IPC to the TM task by this API, for forwarding to HLOS
  for debugging purpose.

DEPENDENCIES
  None.

PARAMETERS
  sm_DebugNMEA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportVarLenNMEAForDebug(sm_DebugNMEAVarLen const *const cpz_DebugNMEA);

/*=============================================================================

FUNCTION
  sm_ReportGnssLEStatus

DESCRIPTION
  This function is to send GNSS LE fix reporting status to SM

DEPENDENCIES
  None.

PARAMETERS
  p_GnssLERptStatus -Pointer to GNSS LE Reporting Status struct

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGnssLEStatus(sm_GnssLERptStatusStructType *p_GnssLERptStatus);

/*=============================================================================

FUNCTION
  sm_ReportSensorLERelDisplacement

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
void sm_ReportSensorLERelDisplacement(const sm_SensorLERelDispStructType *const p_RelDisp);

/*=============================================================================

FUNCTION
  sm_ReportSensorLEStatus

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
void sm_ReportSensorLEStatus(const sm_SensorLEStatusStructType *const p_SensorLEStatus);

/*=============================================================================

FUNCTION
  sm_ReportIqTestCapabilities

DESCRIPTION
  This function is used to send Nav IQ Test capabilities to TM Diag, such
  that unsupported IQ test requests can be rejected.

DEPENDENCIES
  None.

PARAMETERS
  q_IqTestMask - Bit mask of the supported IQ tests, bit shifts defined in
  e_CollectModeTypeVal

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportIqTestCapabilities(uint32 q_IqTestMask);

/*===========================================================================
FUNCTION sm_loc_lte_send_pdapi_event

DESCRIPTION
  This function sends a PDAPI event when OTDOA session starts/stops
  emergency positioning

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void sm_loc_lte_send_pdapi_event(sm_loc_lte_otdoa_pdapi_event_type sess_event_type);

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/* 
 ******************************************************************************
 * 
 *  sm_ConfigExtTimeTransferMode
 *
 * Description:
 *  This function send message to ME module to configures the GNSS state 
 *  for external time transfer.  
 *
 *
 * Parameters:
 *      none  
 *
 * Return value: 
 *     TRUE:  The configuration request was succesfully sent
 *     FALSE: The configuration request was not sent
 ******************************************************************************
*/
boolean sm_ConfigExtTimeTransferMode(uint8 u_EnableFlag);

/*=============================================================================

FUNCTION
  sm_ReportGnssDpoControl

DESCRIPTION
  This function sends a DPO contol value from ME to SM.

DEPENDENCIES
  None.

PARAMETERS
  pu_DpoControl - Pointer to DPO control value

RETURN VALUE
  TRUE  : DPO control value reported to SM successfully.
  FALSE : DPO control value reported to SM unsuccessfully.

SIDE EFFECTS
  None.

=============================================================================*/
boolean sm_ReportGnssDpoControl(const uint8 *pu_DpoControl);

/*=============================================================================

FUNCTION
  sm_ReportAssistanceDataState

DESCRIPTION
  This function reports the assistance data state to LBS

DEPENDENCIES
  None.

PARAMETERS
  const gnss_EmerAssistReportQmi - Pointer to the structure containing the assistance data state

RETURN VALUE
  Boolean: TRUE:  Successfully sent the assistance data state report to LBS
           FALSE: Failed to send the assistance data state report to LBS

SIDE EFFECTS
  None.

=============================================================================*/
boolean sm_ReportAssistanceDataState(const gnss_EmerAssistReportQmi *const pz_AssistanceDataReport);

/*=============================================================================

FUNCTION

  sm_ReportTimeTransferStatusInd

DESCRIPTION
  This function passes on APMP status indication from MC to QMI. 
  And also sets the APMP to disabled in GERA when this indication is received

DEPENDENCIES
  None.

PARAMETERS
  u_ReadyFlag: Indicate if the GNSS engine is ready for time transfer
  pz_Ind:          The time transfer info in LocEng format
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportTimeTransferStatusInd(boolean u_ReadyFlag, const locEngTimeTransferGpsTimeIndMsgT_v01 *pz_Ind);

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*=============================================================================

FUNCTION

  sm_ReportTimeTransferStatusInd

DESCRIPTION
  This function passes on APMP status indication from MC to QMI. 
  And also sets the APMP to disabled in GERA when this indication is received

DEPENDENCIES
  None.

PARAMETERS
  u_ReadyFlag: Indicate if the GNSS engine is ready for time transfer
  pz_Ind:          The time transfer info in LocEng format
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportTimeTransferStatusInd(boolean u_ReadyFlag, const locEngTimeTransferGpsTimeIndMsgT_v01 *pz_Ind);
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*=============================================================================

FUNCTION
  sm_ReportGnssTimeConversionInfo

DESCRIPTION
  This function sends the GNSS Tiem conversion parameters generated by ME to SM.
  ME will generate 1 report which will include clock information for all
  constellations it measures.

DEPENDENCIES
  None.

PARAMETERS
  pz_GnssTimeConversionInfo - Pointer to the structure containing the
                              gnss_TimeConversionParam data

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportGnssTimeConversionInfo(const gnss_TimeConversionParamType *pz_GnssTimeConversionInfo);

/*=============================================================================

FUNCTION
  sm_ReportClkModelInfo

DESCRIPTION
  This function is used by PE to send GNSS Clock Model parameters
  to SM.

DEPENDENCIES
  None.

PARAMETERS
  pz_GnssClkModelInfo - Pointer to the structure containing the
                        sm_GnssClkModelStructType data

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportClkModelInfo(const sm_GnssClkModelStructType *pz_GnssClkModelInfo);
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*=============================================================================

FUNCTION
  sm_ReportGnssPersistentSvMask

DESCRIPTION
  This function sends a blacklisted SV mask from PE to SM.

DEPENDENCIES
  None.

PARAMETERS
  p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
  TRUE  : Persistent SV Mask reported to SM successfully.
  FALSE : Persistent SV Mask reported to SM unsuccessfully.

SIDE EFFECTS
  None.

=============================================================================*/
boolean sm_ReportGnssPersistentSvMask(const gnss_SvIdMaskStructType *p_SvMask);

/*=============================================================================

FUNCTION
sm_ReportConstellationCtrlCfg

DESCRIPTION
This function is used to send GNSS Constellation Control Configuration

DEPENDENCIES
None.

PARAMETERS
pz_MgpConfig - GNSS Constellation Control Configuration in MGP

RETURN VALUE
None.

SIDE EFFECTS
None.

=============================================================================*/
void sm_ReportConstellationCtrlCfg(const gnss_ConstellationConfigT *pz_MgpConfig);

/*=============================================================================

FUNCTION
sm_ReportGnssPersistentSvMaskToFilter

DESCRIPTION
This function sends a blacklisted SV mask from PE to SM.
SM will use this mask to filter out the outgoing measurements to network/AP

DEPENDENCIES
None.

PARAMETERS
p_SvMask - Pointer to the persistent blacklist SV mask

RETURN VALUE
TRUE  : Persistent SV Mask reported to SM successfully.
FALSE : Persistent SV Mask reported to SM unsuccessfully.

SIDE EFFECTS
None.

=============================================================================*/
boolean sm_ReportGnssPersistentSvMaskToFilter(const gnss_SvIdMaskStructType *p_SvMask);

/*===========================================================================
sm_ReportInconsistency

Description:

This function provides an interface for external modules to report spoofing
inconsistencies to Anti-Spoof Module (runs in SM task context)

Parameters:
Pointer to Inconsistency Data

Return value:
void
=============================================================================*/
void sm_ReportInconsistency(sm_InconsistencyDataT *pz_InconsistencyData);

/*=============================================================================
FUNCTION
  sm_SendLFMConfigUpdate

DESCRIPTION
  This function is called by LFM and updates SM with configurations read by
  Location Feature manager either by NV or CaaS (if CaaS feature is enabled).
  LM and TM updates are sent accordingly by calling respective api's.

DEPENDENCIES
  None.

PARAMETERS
  e_NvItem   - NV Item Enum Value being configured
  v_Status   - Feature Status

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_SendLFMConfigUpdate(sm_LFMConfigUpdateStructType *p_LFMfeatureStatus);

/*=============================================================================

FUNCTION
  sm_PfaTestPrescDwellResults

DESCRIPTION
  This function sends prescribed dwell results from MC to TM (via LM).

DEPENDENCIES
  None.

PARAMETERS
  p_PrescDwellResults - Pointer to the presc dwell results

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_PfaTestPrescDwellResults(const sm_PfaTestPresDwellResultsType *p_PrescDwellResults);

/*=============================================================================

FUNCTION
sm_ReportMEMetrics


DESCRIPTION
This function is called by ME to send the band measurement metrics

DEPENDENCIES
None.

PARAMETERS
pz_MEMetrics - Band Measurement Metrics

RETURN VALUE
None.

SIDE EFFECTS
None.

=============================================================================*/
void sm_ReportMEMetrics(  const gnss_MEMetricsStructType *pz_MEMetrics);

/*=============================================================================

FUNCTION
  sm_SendLFMConfigUpdate

DESCRIPTION
  This function is called by LFM and updates SM with configurations read by
  Location Feature manager either by NV or CaaS (if CaaS feature is enabled).
  LM and TM updates are sent accordingly by calling respective api's.

DEPENDENCIES
  None.

PARAMETERS
  e_NvItem   - NV Item Enum Value being configured
  v_Status   - Feature Status

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_SendLFMConfigUpdate(sm_LFMConfigUpdateStructType *p_LFMfeatureStatus);

/*=============================================================================

FUNCTION
  sm_ReportCPEMeas

DESCRIPTION
  Report CPE meas from ME 

DEPENDENCIES
  None.

PARAMETERS
 p_CPEMeas - Pointer to CPE Measurement Block

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_ReportCPEMeas(gnss_CPEMeasBlockStructType *p_CPEMeas);

/*=============================================================================

FUNCTION
  sm_UpdateCPETimingControl

DESCRIPTION
  This updates SM about the CPE timing control that is read by NV

DEPENDENCIES
  None.

PARAMETERS
  q_CPEControl - CPE Control Value

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_UpdateCPETimingControl(uint32 q_CPEControl);

/*=============================================================================
FUNCTION
sm_ReportEphemeris

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
void sm_ReportEphemeris(const gnss_EphemerisDataType *pz_GnssEphemerisData);

/*=============================================================================
FUNCTION
sm_ReportQmiEvent

DESCRIPTION
This function sends an event notification to LocMw.

DEPENDENCIES
None.

PARAMETERS
Pointer to Event Data Payload

RETURN VALUE
None.

SIDE EFFECTS
None.
=============================================================================*/
void sm_ReportQmiEvent(gnss_QmiEventReportT* pz_EventData);

/*
 ******************************************************************************
 * sm_mode_switch
 *
 * Function description:
 *
 * This function handles the MGP ON in "SARF" operation mode.  
 * "SARF" operation mode is turned off by turning MGP OFF
 *
 * Parameters:
 * e_sarf_state - Desired MGP mode.
 *
 *
 * Return value:
 *
 *
 ******************************************************************************
*/
void sm_mode_switch(cgps_sarf_mode_switch_enum_type e_sarf_state);

/*=============================================================================

FUNCTION
  sm_RequestXtraData

DESCRIPTION
  Request from CD for XTRA data. 

DEPENDENCIES
  None.

PARAMETERS
   sm_XtraEnumType:          .

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void sm_RequestXtraData(sm_XtraEnumType e_XtraType );

/*=============================================================================
  
FUNCTION
  sm_PeMagCalState
  
DESCRIPTION
  This function is called by PE to send the PDR Magnetometer Calibration state
  
DEPENDENCIES
  None.
  
PARAMETERS
  pz_PELocEngineStatsReport - Pointer to the structure,
  pz_MagCalState containing Magnetometer's Calibrated State
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===============================================================================*/
void sm_PeMagCalState(const sm_MagCalStateStructType *pz_MagCalState);

/*=============================================================================
  
FUNCTION
  sm_EnhancedXtraConfigPut
  
DESCRIPTION
  This function is called by PE to send the TDM Enablement update to TM
  
DEPENDENCIES
  None.
  
PARAMETERS
  b_isTdmEnabled - Enablement boolean
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===============================================================================*/
void sm_EnhancedXtraConfigPut(boolean b_isTdmEnabled);

#ifdef __cplusplus
}
#endif
#endif /* SM_API_H */


