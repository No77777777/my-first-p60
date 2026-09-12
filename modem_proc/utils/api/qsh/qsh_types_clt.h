/*!
  @file
  qsh_types_clt.h

  @brief
  Exports client enum and dependent types.
  
  @note
  All definitions in this file will need to be modified when modifying 
  qsh_clt_e. This is done to minimize ulog footprint.
*/

/*==============================================================================

  Copyright (c) 2016 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/qsh/qsh_types_clt.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
05/31/16   ca      CR: 1022349 - Adding MPERF and WMS clients.
06/21/16   vt      CR: 1025948 - Changing onex client names.
10/25/15   ca      CR:922311: Enabling cmapi metric collection.
09/11/15   ca      CR:906440, qsh rule man phase 2
08/28/15   ca      CR 898746: QSH rule management feature.
02/13/15   mm      Changed TML1 -> TL1, split TL2 -> TRLC, TMAC
02/12/15   mm      Removed *_ALL clients
01/22/15   mm      Initial check-in
==============================================================================*/

#ifndef QSH_TYPES_CLT_H
#define QSH_TYPES_CLT_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*! @brief
    Enum used to identify the client. 
*/
enum qsh_clt_e
{
  QSH_CLT_NONE       = 0x0,  
  /* CORE */
  QSH_CLT_DSM        = 0x1,   /*!<  Data Services Memory */
  QSH_CLT_HEAP       = 0x2,  /*!<  Heap memory */
  QSH_CLT_CRASH      = 0x3, /*!<  Crash related info */
  QSH_CLT_QURT       = 0x4,  /*!<  QURT/Kernel  */
  QSH_CLT_IMAGE      = 0x5, /*!<  Image/Build/Product info */

  QSH_CLT_MPOWER     = 0x6,  /*!<  Modem Clock and Power Manager (MCPM) */
  
  QSH_CLT_A2         = 0x7,      /*!<  Advanced Accelerator */

  /* LTE */
  QSH_CLT_LFW        = 0x8,   /*!<  LTE Firmware */
  QSH_CLT_LML1       = 0x9,  /*!<  LTE ML1 */
  QSH_CLT_LMAC       = 0xA,  /*!<  LTE MAC */
  QSH_CLT_LRLC       = 0xB,  /*!<  LTE RLC */
  QSH_CLT_LPDCP      = 0xC, /*!<  LTE PDCP */
  QSH_CLT_LRRC       = 0xD,  /*!<  LTE RRC */

  /* WCDMA */
  QSH_CLT_WFW        = 0xE,   /*!<  WCDMA Firmware */
  QSH_CLT_WL1        = 0xF,   /*!<  WCDMA ML1 */
  QSH_CLT_WMAC       = 0x10,  /*!<  WCDMA MAC */
  QSH_CLT_WRLC       = 0x11,  /*!<  WCDMA RLC */
  QSH_CLT_WRRC       = 0x12,  /*!<  WCDMA RRC */

  /* TDS */
  QSH_CLT_TFW        = 0x13,   /*!<  TDS Firmware */
  QSH_CLT_TL1        = 0x14,   /*!<  TDS ML1 */
  QSH_CLT_TRLC       = 0x15,  /*!<  TDS RLC */
  QSH_CLT_TMAC       = 0x16,  /*!<  TDS MAC */
  QSH_CLT_TRRC       = 0x17,  /*!<  TDS RRC */

  /* GERAN */
  QSH_CLT_GFW        = 0x18,
  QSH_CLT_GSNDCP     = 0x19,
  QSH_CLT_GLLC       = 0x1A,
  QSH_CLT_GRLCUL     = 0x1B,
  QSH_CLT_GRLCDL     = 0x1C,
  QSH_CLT_GRR        = 0x1D,
  QSH_CLT_GMAC       = 0x1E,
  QSH_CLT_GL1        = 0x1F,

  /* 1X */
  QSH_CLT_ONEXL1     = 0x20,
  QSH_CLT_ONEXL2     = 0x21,
  QSH_CLT_ONEXL3     = 0x22,
  QSH_CLT_ONEXMSC    = 0x23,  
  QSH_CLT_ONEXFW     = 0x24,

  /* HDR */
  QSH_CLT_HMC        = 0x25,        /*!<  HDR MC */
  QSH_CLT_HDEC       = 0x26,       /*!<  HDR DEC */
  QSH_CLT_HRX        = 0x27,        /*!<  HDR HRX */
  QSH_CLT_HTX        = 0x28,        /*!<  HDR TX */
  QSH_CLT_HSRCH      = 0x29,      /*!<  HDR SRCH */

  /* MMCP */
  QSH_CLT_CM         = 0x2A,         /*!<  MMCP Call Manager */
  QSH_CLT_SD         = 0x2B,         /*!<  MMCP SD */
  QSH_CLT_MMOC       = 0x2C,       /*!<  MMCP MMOC */
  QSH_CLT_PM         = 0x2D,         /*!<  MMCP Policy Manager */
  QSH_CLT_REG        = 0x2E,        /*!<  MMCP REG */
  QSH_CLT_MM         = 0x2F,         /*!<  MMCP MM */
  QSH_CLT_CNM        = 0x30,        /*!<  MMCP CNM */
  QSH_CLT_MN         = 0x31,         /*!<  MMCP MN */
  QSH_CLT_SM         = 0x32,         /*!<  MMCP SM */

  /* DATA */
  QSH_CLT_DS3GPP     = 0x33,
  QSH_CLT_DS3GPP2    = 0x34,
  QSH_CLT_IPA        = 0x35,
  
  /* MCS */
  QSH_CLT_TRM        = 0x36,
  QSH_CLT_TCXOMGR    = 0x37,    /*<! MCS TCXOMGR */
  QSH_CLT_SLPC       = 0x38,
  QSH_CLT_VSTMR      = 0x39,
  QSH_CLT_FWS        = 0x3A,        /*<! MCS FW Services */
  QSH_CLT_MSGR       = 0x3B,
  QSH_CLT_HWIO       = 0x3C,      /*<! MCS HWIO */
  QSH_CLT_CFCM       = 0x3D,

  /* IMS */
  QSH_CLT_SMS        = 0x3E,
  QSH_CLT_MEDIA      = 0x3F,

  /* UIM */
  QSH_CLT_USIM       = 0x40,
  QSH_CLT_UTLKIT     = 0x41,     /*!<  UIM toolkit */
  QSH_CLT_UDRVRS     = 0x42,     /*!<  UIM Drivers */
  QSH_CLT_UPBM       = 0x43,
  QSH_CLT_UQMI       = 0x44,

  QSH_CLT_AP         = 0x45,    /*!<  Application processor */  
  QSH_CLT_HOST       = 0x46,  /*!<  Host PC */ 
  QSH_CLT_EMBMS      = 0x47, /*!<  EMBMS*/
  QSH_CLT_VOLTE      = 0x48, /*!<  VOLTE */
  QSH_CLT_RF         = 0x49,    /*!<  RF */
  QSH_CLT_OTHER      = 0x4A, /*!<  Other */
  QSH_CLT_ALL        = 0x4B,   /*!<  All Client. DO NOT USE for logging */
  QSH_CLT_QSH        = 0x4C,

  QSH_CLT_QRARB      = 0x4D,  /*!< Qlink Recovery Arbitrator */
  QSH_CLT_CAFI       = 0x4E,  
  QSH_CLT_MPERF      = 0x4F,  /*!< performance client */
  QSH_CLT_WMS        = 0x50,  /*!< wireless message services */

  /* NR5G */
  QSH_CLT_NR5GRF     = 0x51,
  QSH_CLT_NR5GFW     = 0x52,
  QSH_CLT_NR5GML1    = 0x53,
  QSH_CLT_NR5GMAC    = 0x54,
  QSH_CLT_NR5GRLC    = 0x55,
  QSH_CLT_NR5GPDCP   = 0x56,
  QSH_CLT_NR5GRRC    = 0x57,
  QSH_CLT_PLT        = 0x58,
  QSH_CLT_IMS        = 0x59,
  QSH_CLT_LOC_SM     = 0x5A,
  QSH_CLT_LOC_ME     = 0x5B,
  QSH_CLT_LOC_PE     = 0x5C,
  QSH_CLT_LOC_AON    = 0x5D,

  QSH_CLT_MAX        = 0x5E,   /*!<  DO NOT USE. Add new enums before this entry*/
};

/* -----------------------------------------------------------------------------

  Note: Modify all following groups when changing qsh_clt_e above.
  
----------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*! @brief
    Provides a mapping between client enum and string representation.
*/
/* -------------------------------------------------------------------------- */
#define QSH_CLT_NONE_STR        "NONE"
#define QSH_CLT_DSM_STR         "DSM"
#define QSH_CLT_HEAP_STR        "HEAP"
#define QSH_CLT_CRASH_STR       "CRASH"
#define QSH_CLT_QURT_STR        "QURT"
#define QSH_CLT_IMAGE_STR       "IMAGE"

#define QSH_CLT_MPOWER_STR      "MPOWER"
  
#define QSH_CLT_A2_STR          "A2"

#define QSH_CLT_LFW_STR         "LFW"
#define QSH_CLT_LML1_STR        "LML1"
#define QSH_CLT_LMAC_STR        "LMAC"
#define QSH_CLT_LRLC_STR        "LRLC"
#define QSH_CLT_LPDCP_STR       "LPDCP"
#define QSH_CLT_LRRC_STR        "LRRC"

#define QSH_CLT_WFW_STR         "WFW"
#define QSH_CLT_WL1_STR         "WL1"
#define QSH_CLT_WMAC_STR        "WMAC"
#define QSH_CLT_WRLC_STR        "WRLC"
#define QSH_CLT_WRRC_STR        "WRRC"

#define QSH_CLT_TFW_STR         "TFW"
#define QSH_CLT_TL1_STR         "TL1"
#define QSH_CLT_TRLC_STR        "TRLC"
#define QSH_CLT_TMAC_STR        "TMAC"
#define QSH_CLT_TRRC_STR        "TRRC"

#define QSH_CLT_GFW_STR         "GFW"
#define QSH_CLT_GSNDCP_STR      "GSNDCP"
#define QSH_CLT_GLLC_STR        "GLLC"
#define QSH_CLT_GRLCUL_STR      "GRLCUL"
#define QSH_CLT_GRLCDL_STR      "GRLCDL"
#define QSH_CLT_GRR_STR         "GRR"
#define QSH_CLT_GMAC_STR        "GMAC"
#define QSH_CLT_GL1_STR         "GL1"

#define QSH_CLT_ONEXL1_STR      "ONEXL1"
#define QSH_CLT_ONEXL2_STR      "ONEXL2"
#define QSH_CLT_ONEXL3_STR      "ONEXL3"
#define QSH_CLT_ONEXMSC_STR     "ONEXMSC"
#define QSH_CLT_ONEXFW_STR      "ONEXFW"

#define QSH_CLT_HMC_STR         "HMC"
#define QSH_CLT_HDEC_STR        "HDEC"
#define QSH_CLT_HRX_STR         "HRX"
#define QSH_CLT_HTX_STR         "HTX"
#define QSH_CLT_HSRCH_STR       "HSRCH"

#define QSH_CLT_CM_STR          "CM"
#define QSH_CLT_SD_STR          "SD"
#define QSH_CLT_MMOC_STR        "MMOC"
#define QSH_CLT_PM_STR          "PM"
#define QSH_CLT_REG_STR         "REG"
#define QSH_CLT_MM_STR          "MM"
#define QSH_CLT_CNM_STR         "CNM"
#define QSH_CLT_MN_STR          "MN"
#define QSH_CLT_SM_STR          "SM"

#define QSH_CLT_DS3GPP_STR      "DS3GPP"
#define QSH_CLT_DS3GPP2_STR     "DS3GPP2"
#define QSH_CLT_IPA_STR         "IPA"

#define QSH_CLT_TRM_STR         "TRM"
#define QSH_CLT_TCXOMGR_STR     "TCXOMGR"
#define QSH_CLT_SLPC_STR        "SLPC"
#define QSH_CLT_VSTMR_STR       "VSTMR"
#define QSH_CLT_FWS_STR         "FWS"
#define QSH_CLT_MSGR_STR        "MSGR"
#define QSH_CLT_HWIO_STR        "HWIO"
#define QSH_CLT_CFCM_STR        "CFCM"

#define QSH_CLT_SMS_STR         "SMS"
#define QSH_CLT_MEDIA_STR       "MEDIA"

#define QSH_CLT_USIM_STR        "USIM"
#define QSH_CLT_UTLKIT_STR      "UTLKIT"
#define QSH_CLT_UDRVRS_STR      "UDRVRS"
#define QSH_CLT_UPBM_STR        "UPBM"
#define QSH_CLT_UQMI_STR        "UQMI"

#define QSH_CLT_AP_STR          "AP"
#define QSH_CLT_HOST_STR        "HOST"
#define QSH_CLT_EMBMS_STR       "EMBMS"
#define QSH_CLT_VOLTE_STR       "VOLTE"
#define QSH_CLT_RF_STR          "RF"
#define QSH_CLT_OTHER_STR       "OTHER"
#define QSH_CLT_ALL_STR         "ALL"
#define QSH_CLT_QSH_STR         "QSH"

#define QSH_CLT_QRARB_STR       "QRARB"
#define QSH_CLT_CAFI_STR        "CAFI"
#define QSH_CLT_MPERF_STR       "MPERF"
#define QSH_CLT_WMS_STR         "WMS"

#define QSH_CLT_NR5GRF_STR      "NR5GRF"
#define QSH_CLT_NR5GFW_STR      "NR5GFW"
#define QSH_CLT_NR5GML1_STR     "NR5GML1"
#define QSH_CLT_NR5GMAC_STR     "NR5GMAC"
#define QSH_CLT_NR5GRLC_STR     "NR5GRLC"
#define QSH_CLT_NR5GPDCP_STR    "NR5GPDCP"
#define QSH_CLT_NR5GRRC_STR     "NR5GRRC"
#define QSH_CLT_PLT_STR         "PLT"
#define QSH_CLT_IMS_STR         "IMS"
#define QSH_CLT_LOC_SM_STR      "LOC_SM"
#define QSH_CLT_LOC_ME_STR      "LOC_ME"         
#define QSH_CLT_LOC_PE_STR      "LOC_PE"
#define QSH_CLT_LOC_AON_STR     "LOC_AON"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*! @brief
    Client names padded to maximum width (7 chars).

    @note
    "ALL" clients are not required to be padded to maximum width, since
    max-width padding is only required for log prints.
*/
/* -------------------------------------------------------------------------- */
#define QSH_CLT_NONE_STR_PAD        QSH_CLT_NONE_STR "    "
#define QSH_CLT_DSM_STR_PAD         QSH_CLT_DSM_STR "     "
#define QSH_CLT_HEAP_STR_PAD        QSH_CLT_HEAP_STR "    "
#define QSH_CLT_CRASH_STR_PAD       QSH_CLT_CRASH_STR "   "
#define QSH_CLT_QURT_STR_PAD        QSH_CLT_QURT_STR "    "
#define QSH_CLT_IMAGE_STR_PAD       QSH_CLT_IMAGE_STR "   "

#define QSH_CLT_MPOWER_STR_PAD      QSH_CLT_MPOWER_STR "  "
  
#define QSH_CLT_A2_STR_PAD          QSH_CLT_A2_STR "      "

#define QSH_CLT_LTE_ALL_STR_PAD     ""
#define QSH_CLT_LFW_STR_PAD         QSH_CLT_LFW_STR "     "
#define QSH_CLT_LML1_STR_PAD        QSH_CLT_LML1_STR "    "
#define QSH_CLT_LMAC_STR_PAD        QSH_CLT_LMAC_STR "    "
#define QSH_CLT_LRLC_STR_PAD        QSH_CLT_LRLC_STR "    "
#define QSH_CLT_LPDCP_STR_PAD       QSH_CLT_LPDCP_STR "   "
#define QSH_CLT_LRRC_STR_PAD        QSH_CLT_LRRC_STR "    "

#define QSH_CLT_WFW_STR_PAD         QSH_CLT_WFW_STR "     "
#define QSH_CLT_WL1_STR_PAD         QSH_CLT_WL1_STR "     "
#define QSH_CLT_WMAC_STR_PAD        QSH_CLT_WMAC_STR "    "
#define QSH_CLT_WRLC_STR_PAD        QSH_CLT_WRLC_STR "    "
#define QSH_CLT_WRRC_STR_PAD        QSH_CLT_WRRC_STR "    "

#define QSH_CLT_TFW_STR_PAD         QSH_CLT_TFW_STR "     "
#define QSH_CLT_TL1_STR_PAD         QSH_CLT_TL1_STR "     "
#define QSH_CLT_TRLC_STR_PAD        QSH_CLT_TRLC_STR "    "
#define QSH_CLT_TMAC_STR_PAD        QSH_CLT_TMAC_STR "    "
#define QSH_CLT_TRRC_STR_PAD        QSH_CLT_TRRC_STR "    "

#define QSH_CLT_GFW_STR_PAD         QSH_CLT_GFW_STR "     "
#define QSH_CLT_GSNDCP_STR_PAD      QSH_CLT_GSNDCP_STR "  "
#define QSH_CLT_GLLC_STR_PAD        QSH_CLT_GLLC_STR "    "
#define QSH_CLT_GRLCUL_STR_PAD      QSH_CLT_GRLCUL_STR "  "
#define QSH_CLT_GRLCDL_STR_PAD      QSH_CLT_GRLCDL_STR "  "
#define QSH_CLT_GRR_STR_PAD         QSH_CLT_GRR_STR "     "
#define QSH_CLT_GMAC_STR_PAD        QSH_CLT_GMAC_STR "    "
#define QSH_CLT_GL1_STR_PAD         QSH_CLT_GL1_STR "     "

#define QSH_CLT_ONEXL1_STR_PAD      QSH_CLT_ONEXL1_STR "  "
#define QSH_CLT_ONEXL2_STR_PAD      QSH_CLT_ONEXL2_STR "  "
#define QSH_CLT_ONEXL3_STR_PAD      QSH_CLT_ONEXL3_STR "  "
#define QSH_CLT_ONEXMSC_STR_PAD     QSH_CLT_ONEXMSC_STR " "
#define QSH_CLT_ONEXFW_STR_PAD      QSH_CLT_ONEXFW_STR "  "

#define QSH_CLT_HMC_STR_PAD         QSH_CLT_HMC_STR "     "
#define QSH_CLT_HDEC_STR_PAD        QSH_CLT_HDEC_STR "    "
#define QSH_CLT_HRX_STR_PAD         QSH_CLT_HRX_STR "     "
#define QSH_CLT_HTX_STR_PAD         QSH_CLT_HTX_STR "     "
#define QSH_CLT_HSRCH_STR_PAD       QSH_CLT_HSRCH_STR "   "

#define QSH_CLT_CM_STR_PAD          QSH_CLT_CM_STR "      "
#define QSH_CLT_SD_STR_PAD          QSH_CLT_SD_STR "      "
#define QSH_CLT_MMOC_STR_PAD        QSH_CLT_MMOC_STR "    "
#define QSH_CLT_PM_STR_PAD          QSH_CLT_PM_STR "      "
#define QSH_CLT_REG_STR_PAD         QSH_CLT_REG_STR "     "
#define QSH_CLT_MM_STR_PAD          QSH_CLT_MM_STR "      "
#define QSH_CLT_CNM_STR_PAD         QSH_CLT_CNM_STR "     "
#define QSH_CLT_MN_STR_PAD          QSH_CLT_MN_STR "      "
#define QSH_CLT_SM_STR_PAD          QSH_CLT_SM_STR "      "

#define QSH_CLT_DS3GPP_STR_PAD      QSH_CLT_DS3GPP_STR "  "
#define QSH_CLT_DS3GPP2_STR_PAD     QSH_CLT_DS3GPP2_STR " "
#define QSH_CLT_IPA_STR_PAD         QSH_CLT_IPA_STR "     "

#define QSH_CLT_TRM_STR_PAD         QSH_CLT_TRM_STR "     "
#define QSH_CLT_TCXOMGR_STR_PAD     QSH_CLT_TCXOMGR_STR " "
#define QSH_CLT_SLPC_STR_PAD        QSH_CLT_SLPC_STR "    "
#define QSH_CLT_VSTMR_STR_PAD       QSH_CLT_VSTMR_STR "   "
#define QSH_CLT_FWS_STR_PAD         QSH_CLT_FWS_STR "     "
#define QSH_CLT_MSGR_STR_PAD        QSH_CLT_MSGR_STR "    "
#define QSH_CLT_HWIO_STR_PAD        QSH_CLT_HWIO_STR "    "
#define QSH_CLT_CFCM_STR_PAD        QSH_CLT_CFCM_STR "    "

#define QSH_CLT_SMS_STR_PAD         QSH_CLT_SMS_STR "     "
#define QSH_CLT_MEDIA_STR_PAD       QSH_CLT_MEDIA_STR "   "

#define QSH_CLT_USIM_STR_PAD        QSH_CLT_USIM_STR "    "
#define QSH_CLT_UTLKIT_STR_PAD      QSH_CLT_UTLKIT_STR "  "
#define QSH_CLT_UDRVRS_STR_PAD      QSH_CLT_UDRVRS_STR "  "
#define QSH_CLT_UPBM_STR_PAD        QSH_CLT_UPBM_STR "    "
#define QSH_CLT_UQMI_STR_PAD        QSH_CLT_UQMI_STR "    "

#define QSH_CLT_AP_STR_PAD          QSH_CLT_AP_STR "      "
#define QSH_CLT_HOST_STR_PAD        QSH_CLT_HOST_STR "    "
#define QSH_CLT_EMBMS_STR_PAD       QSH_CLT_EMBMS_STR "   "
#define QSH_CLT_VOLTE_STR_PAD       QSH_CLT_VOLTE_STR "   "
#define QSH_CLT_RF_STR_PAD          QSH_CLT_RF_STR "      "
#define QSH_CLT_OTHER_STR_PAD       QSH_CLT_OTHER_STR "   "
#define QSH_CLT_ALL_STR_PAD         ""
#define QSH_CLT_QSH_STR_PAD         QSH_CLT_QSH_STR "     "

#define QSH_CLT_QRARB_STR_PAD       QSH_CLT_QRARB_STR "   "
#define QSH_CLT_CAFI_STR_PAD        QSH_CLT_CAFI_STR "    "
#define QSH_CLT_MPERF_STR_PAD       QSH_CLT_MPERF_STR "   "
#define QSH_CLT_WMS_STR_PAD         QSH_CLT_WMS_STR "     "

#define QSH_CLT_NR5GRF_STR_PAD      QSH_CLT_NR5GRF_STR "  "
#define QSH_CLT_NR5GFW_STR_PAD      QSH_CLT_NR5GFW_STR "  "
#define QSH_CLT_NR5GML1_STR_PAD     QSH_CLT_NR5GML1_STR " "
#define QSH_CLT_NR5GMAC_STR_PAD     QSH_CLT_NR5GMAC_STR " "
#define QSH_CLT_NR5GRLC_STR_PAD     QSH_CLT_NR5GRLC_STR " "
#define QSH_CLT_NR5GPDCP_STR_PAD    QSH_CLT_NR5GPDCP_STR ""
#define QSH_CLT_NR5GRRC_STR_PAD     QSH_CLT_NR5GRRC_STR " "
#define QSH_CLT_PLT_STR_PAD         QSH_CLT_PLT_STR "     "
#define QSH_CLT_IMS_STR_PAD         QSH_CLT_IMS_STR "     "

#define QSH_CLT_LOC_SM_STR_PAD      QSH_CLT_LOC_SM_STR "  "
#define QSH_CLT_LOC_ME_STR_PAD      QSH_CLT_LOC_ME_STR "  "
#define QSH_CLT_LOC_PE_STR_PAD      QSH_CLT_LOC_PE_STR "  "
#define QSH_CLT_LOC_AON_STR_PAD     QSH_CLT_LOC_AON_STR " "

/* -------------------------------------------------------------------------- */

#endif /* QSH_TYPES_CLT_H */
