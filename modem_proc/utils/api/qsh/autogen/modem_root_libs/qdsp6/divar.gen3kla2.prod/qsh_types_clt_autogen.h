#ifndef QSH_TYPES_CLT_AUTOGEN_H
#define QSH_TYPES_CLT_AUTOGEN_H

/* Max width of client name */
#define QSH_CLT_WIDTH_MAX \
  8

/* Enum used to identify the client */
typedef enum qsh_clt_e {
  QSH_CLT_NONE = 0,
  QSH_CLT_DSM = 1,
  QSH_CLT_HEAP = 2,
  QSH_CLT_CRASH = 3,
  QSH_CLT_QURT = 4,
  QSH_CLT_IMAGE = 5,
  QSH_CLT_MPOWER = 6,
  QSH_CLT_A2 = 7,
  QSH_CLT_LFW = 8,
  QSH_CLT_LML1 = 9,
  QSH_CLT_LMAC = 10,
  QSH_CLT_LRLC = 11,
  QSH_CLT_LPDCP = 12,
  QSH_CLT_LRRC = 13,
  QSH_CLT_WFW = 14,
  QSH_CLT_WL1 = 15,
  QSH_CLT_WMAC = 16,
  QSH_CLT_WRLC = 17,
  QSH_CLT_WRRC = 18,
  QSH_CLT_TFW = 19,
  QSH_CLT_TL1 = 20,
  QSH_CLT_TRLC = 21,
  QSH_CLT_TMAC = 22,
  QSH_CLT_TRRC = 23,
  QSH_CLT_GFW = 24,
  QSH_CLT_GSNDCP = 25,
  QSH_CLT_GLLC = 26,
  QSH_CLT_GRLCUL = 27,
  QSH_CLT_GRLCDL = 28,
  QSH_CLT_GRR = 29,
  QSH_CLT_GMAC = 30,
  QSH_CLT_GL1 = 31,
  QSH_CLT_ONEXL1 = 32,
  QSH_CLT_ONEXL2 = 33,
  QSH_CLT_ONEXL3 = 34,
  QSH_CLT_ONEXMSC = 35,
  QSH_CLT_ONEXFW = 36,
  QSH_CLT_HMC = 37,
  QSH_CLT_HDEC = 38,
  QSH_CLT_HRX = 39,
  QSH_CLT_HTX = 40,
  QSH_CLT_HSRCH = 41,
  QSH_CLT_CM = 42,
  QSH_CLT_SD = 43,
  QSH_CLT_MMOC = 44,
  QSH_CLT_PM = 45,
  QSH_CLT_REG = 46,
  QSH_CLT_MM = 47,
  QSH_CLT_CNM = 48,
  QSH_CLT_MN = 49,
  QSH_CLT_SM = 50,
  QSH_CLT_DS3GPP = 51,
  QSH_CLT_DS = 52,
  QSH_CLT_IPA = 53,
  QSH_CLT_TRM = 54,
  QSH_CLT_TCXOMGR = 55,
  QSH_CLT_SLPC = 56,
  QSH_CLT_VSTMR = 57,
  QSH_CLT_FWS = 58,
  QSH_CLT_MSGR = 59,
  QSH_CLT_HWIO = 60,
  QSH_CLT_CFCM = 61,
  QSH_CLT_SMS = 62,
  QSH_CLT_MEDIA = 63,
  QSH_CLT_USIM = 64,
  QSH_CLT_UTLKIT = 65,
  QSH_CLT_UDRVRS = 66,
  QSH_CLT_UPBM = 67,
  QSH_CLT_UQMI = 68,
  QSH_CLT_AP = 69,
  QSH_CLT_HOST = 70,
  QSH_CLT_EMBMS = 71,
  QSH_CLT_VOLTE = 72,
  QSH_CLT_RF = 73,
  QSH_CLT_OTHER = 74,
  QSH_CLT_ALL = 75,
  QSH_CLT_QSH = 76,
  QSH_CLT_QRARB = 77,
  QSH_CLT_CAFI = 78,
  QSH_CLT_MPERF = 79,
  QSH_CLT_WMS = 80,
  QSH_CLT_NR5GRF = 81,
  QSH_CLT_NR5GFW = 82,
  QSH_CLT_NR5GML1 = 83,
  QSH_CLT_NR5GMAC = 84,
  QSH_CLT_NR5GL2UL = 85,
  QSH_CLT_NR5GL2DL = 86,
  QSH_CLT_NR5GRRC = 87,
  QSH_CLT_PLT = 88,
  QSH_CLT_IMS = 89,
  QSH_CLT_LOC_SM = 90,
  QSH_CLT_LOC_ME = 91,
  QSH_CLT_LOC_PE = 92,
  QSH_CLT_LOC_AON = 93,
  QSH_CLT_BSI = 94,
  QSH_CLT_LSM = 95,
  QSH_CLT_L2LB = 96,
  QSH_CLT_RTP = 97,
  QSH_CLT_ENL2DL = 98,
  QSH_CLT_ENL2UL = 99,
  QSH_CLT_CMAPI = 100,
  QSH_CLT_LMTSMGR = 101,
  QSH_CLT_MCS = 102,
  QSH_CLT_MCFG = 103,
  QSH_CLT_V2XL1 = 104,
  QSH_CLT_MSF = 105,
  QSH_CLT_MACE = 106,
  QSH_CLT_THERMAL = 107,
  QSH_CLT_ENL1 = 108,
  QSH_CLT_SSGCCS = 109,
  QSH_CLT_SSGSCA = 110,
  QSH_CLT_V2XRRC = 111,
  QSH_CLT_V2XL2 = 112,
  QSH_CLT_MAX = 114
} qsh_clt_e;

#define QSH_CLT_NONE_STR \
  "NONE"

#define QSH_CLT_DSM_STR \
  "DSM"

#define QSH_CLT_HEAP_STR \
  "HEAP"

#define QSH_CLT_CRASH_STR \
  "CRASH"

#define QSH_CLT_QURT_STR \
  "QURT"

#define QSH_CLT_IMAGE_STR \
  "IMAGE"

#define QSH_CLT_MPOWER_STR \
  "MPOWER"

#define QSH_CLT_A2_STR \
  "A2"

#define QSH_CLT_LFW_STR \
  "LFW"

#define QSH_CLT_LML1_STR \
  "LML1"

#define QSH_CLT_LMAC_STR \
  "LMAC"

#define QSH_CLT_LRLC_STR \
  "LRLC"

#define QSH_CLT_LPDCP_STR \
  "LPDCP"

#define QSH_CLT_LRRC_STR \
  "LRRC"

#define QSH_CLT_WFW_STR \
  "WFW"

#define QSH_CLT_WL1_STR \
  "WL1"

#define QSH_CLT_WMAC_STR \
  "WMAC"

#define QSH_CLT_WRLC_STR \
  "WRLC"

#define QSH_CLT_WRRC_STR \
  "WRRC"

#define QSH_CLT_TFW_STR \
  "TFW"

#define QSH_CLT_TL1_STR \
  "TL1"

#define QSH_CLT_TRLC_STR \
  "TRLC"

#define QSH_CLT_TMAC_STR \
  "TMAC"

#define QSH_CLT_TRRC_STR \
  "TRRC"

#define QSH_CLT_GFW_STR \
  "GFW"

#define QSH_CLT_GSNDCP_STR \
  "GSNDCP"

#define QSH_CLT_GLLC_STR \
  "GLLC"

#define QSH_CLT_GRLCUL_STR \
  "GRLCUL"

#define QSH_CLT_GRLCDL_STR \
  "GRLCDL"

#define QSH_CLT_GRR_STR \
  "GRR"

#define QSH_CLT_GMAC_STR \
  "GMAC"

#define QSH_CLT_GL1_STR \
  "GL1"

#define QSH_CLT_ONEXL1_STR \
  "ONEXL1"

#define QSH_CLT_ONEXL2_STR \
  "ONEXL2"

#define QSH_CLT_ONEXL3_STR \
  "ONEXL3"

#define QSH_CLT_ONEXMSC_STR \
  "ONEXMSC"

#define QSH_CLT_ONEXFW_STR \
  "ONEXFW"

#define QSH_CLT_HMC_STR \
  "HMC"

#define QSH_CLT_HDEC_STR \
  "HDEC"

#define QSH_CLT_HRX_STR \
  "HRX"

#define QSH_CLT_HTX_STR \
  "HTX"

#define QSH_CLT_HSRCH_STR \
  "HSRCH"

#define QSH_CLT_CM_STR \
  "CM"

#define QSH_CLT_SD_STR \
  "SD"

#define QSH_CLT_MMOC_STR \
  "MMOC"

#define QSH_CLT_PM_STR \
  "PM"

#define QSH_CLT_REG_STR \
  "REG"

#define QSH_CLT_MM_STR \
  "MM"

#define QSH_CLT_CNM_STR \
  "CNM"

#define QSH_CLT_MN_STR \
  "MN"

#define QSH_CLT_SM_STR \
  "SM"

#define QSH_CLT_DS3GPP_STR \
  "DS3GPP"

#define QSH_CLT_DS_STR \
  "DS"

#define QSH_CLT_IPA_STR \
  "IPA"

#define QSH_CLT_TRM_STR \
  "TRM"

#define QSH_CLT_TCXOMGR_STR \
  "TCXOMGR"

#define QSH_CLT_SLPC_STR \
  "SLPC"

#define QSH_CLT_VSTMR_STR \
  "VSTMR"

#define QSH_CLT_FWS_STR \
  "FWS"

#define QSH_CLT_MSGR_STR \
  "MSGR"

#define QSH_CLT_HWIO_STR \
  "HWIO"

#define QSH_CLT_CFCM_STR \
  "CFCM"

#define QSH_CLT_SMS_STR \
  "SMS"

#define QSH_CLT_MEDIA_STR \
  "MEDIA"

#define QSH_CLT_USIM_STR \
  "USIM"

#define QSH_CLT_UTLKIT_STR \
  "UTLKIT"

#define QSH_CLT_UDRVRS_STR \
  "UDRVRS"

#define QSH_CLT_UPBM_STR \
  "UPBM"

#define QSH_CLT_UQMI_STR \
  "UQMI"

#define QSH_CLT_AP_STR \
  "AP"

#define QSH_CLT_HOST_STR \
  "HOST"

#define QSH_CLT_EMBMS_STR \
  "EMBMS"

#define QSH_CLT_VOLTE_STR \
  "VOLTE"

#define QSH_CLT_RF_STR \
  "RF"

#define QSH_CLT_OTHER_STR \
  "OTHER"

#define QSH_CLT_ALL_STR \
  "ALL"

#define QSH_CLT_QSH_STR \
  "QSH"

#define QSH_CLT_QRARB_STR \
  "QRARB"

#define QSH_CLT_CAFI_STR \
  "CAFI"

#define QSH_CLT_MPERF_STR \
  "MPERF"

#define QSH_CLT_WMS_STR \
  "WMS"

#define QSH_CLT_NR5GRF_STR \
  "NR5GRF"

#define QSH_CLT_NR5GFW_STR \
  "NR5GFW"

#define QSH_CLT_NR5GML1_STR \
  "NR5GML1"

#define QSH_CLT_NR5GMAC_STR \
  "NR5GMAC"

#define QSH_CLT_NR5GL2UL_STR \
  "NR5GL2UL"

#define QSH_CLT_NR5GL2DL_STR \
  "NR5GL2DL"

#define QSH_CLT_NR5GRRC_STR \
  "NR5GRRC"

#define QSH_CLT_PLT_STR \
  "PLT"

#define QSH_CLT_IMS_STR \
  "IMS"

#define QSH_CLT_LOC_SM_STR \
  "LOC_SM"

#define QSH_CLT_LOC_ME_STR \
  "LOC_ME"

#define QSH_CLT_LOC_PE_STR \
  "LOC_PE"

#define QSH_CLT_LOC_AON_STR \
  "LOC_AON"

#define QSH_CLT_BSI_STR \
  "BSI"

#define QSH_CLT_LSM_STR \
  "LSM"

#define QSH_CLT_L2LB_STR \
  "L2LB"

#define QSH_CLT_RTP_STR \
  "RTP"

#define QSH_CLT_ENL2DL_STR \
  "ENL2DL"

#define QSH_CLT_ENL2UL_STR \
  "ENL2UL"

#define QSH_CLT_CMAPI_STR \
  "CMAPI"

#define QSH_CLT_LMTSMGR_STR \
  "LMTSMGR"

#define QSH_CLT_MCS_STR \
  "MCS"

#define QSH_CLT_MCFG_STR \
  "MCFG"

#define QSH_CLT_V2XL1_STR \
  "V2XL1"

#define QSH_CLT_MSF_STR \
  "MSF"

#define QSH_CLT_MACE_STR \
  "MACE"

#define QSH_CLT_THERMAL_STR \
  "THERMAL"

#define QSH_CLT_ENL1_STR \
  "ENL1"

#define QSH_CLT_SSGCCS_STR \
  "SSGCCS"

#define QSH_CLT_SSGSCA_STR \
  "SSGSCA"

#define QSH_CLT_V2XRRC_STR \
  "V2XRRC"

#define QSH_CLT_V2XL2_STR \
  "V2XL2"

#define QSH_CLT_NONE_STR_PAD \
  "NONE    "

#define QSH_CLT_DSM_STR_PAD \
  "DSM     "

#define QSH_CLT_HEAP_STR_PAD \
  "HEAP    "

#define QSH_CLT_CRASH_STR_PAD \
  "CRASH   "

#define QSH_CLT_QURT_STR_PAD \
  "QURT    "

#define QSH_CLT_IMAGE_STR_PAD \
  "IMAGE   "

#define QSH_CLT_MPOWER_STR_PAD \
  "MPOWER  "

#define QSH_CLT_A2_STR_PAD \
  "A2      "

#define QSH_CLT_LFW_STR_PAD \
  "LFW     "

#define QSH_CLT_LML1_STR_PAD \
  "LML1    "

#define QSH_CLT_LMAC_STR_PAD \
  "LMAC    "

#define QSH_CLT_LRLC_STR_PAD \
  "LRLC    "

#define QSH_CLT_LPDCP_STR_PAD \
  "LPDCP   "

#define QSH_CLT_LRRC_STR_PAD \
  "LRRC    "

#define QSH_CLT_WFW_STR_PAD \
  "WFW     "

#define QSH_CLT_WL1_STR_PAD \
  "WL1     "

#define QSH_CLT_WMAC_STR_PAD \
  "WMAC    "

#define QSH_CLT_WRLC_STR_PAD \
  "WRLC    "

#define QSH_CLT_WRRC_STR_PAD \
  "WRRC    "

#define QSH_CLT_TFW_STR_PAD \
  "TFW     "

#define QSH_CLT_TL1_STR_PAD \
  "TL1     "

#define QSH_CLT_TRLC_STR_PAD \
  "TRLC    "

#define QSH_CLT_TMAC_STR_PAD \
  "TMAC    "

#define QSH_CLT_TRRC_STR_PAD \
  "TRRC    "

#define QSH_CLT_GFW_STR_PAD \
  "GFW     "

#define QSH_CLT_GSNDCP_STR_PAD \
  "GSNDCP  "

#define QSH_CLT_GLLC_STR_PAD \
  "GLLC    "

#define QSH_CLT_GRLCUL_STR_PAD \
  "GRLCUL  "

#define QSH_CLT_GRLCDL_STR_PAD \
  "GRLCDL  "

#define QSH_CLT_GRR_STR_PAD \
  "GRR     "

#define QSH_CLT_GMAC_STR_PAD \
  "GMAC    "

#define QSH_CLT_GL1_STR_PAD \
  "GL1     "

#define QSH_CLT_ONEXL1_STR_PAD \
  "ONEXL1  "

#define QSH_CLT_ONEXL2_STR_PAD \
  "ONEXL2  "

#define QSH_CLT_ONEXL3_STR_PAD \
  "ONEXL3  "

#define QSH_CLT_ONEXMSC_STR_PAD \
  "ONEXMSC "

#define QSH_CLT_ONEXFW_STR_PAD \
  "ONEXFW  "

#define QSH_CLT_HMC_STR_PAD \
  "HMC     "

#define QSH_CLT_HDEC_STR_PAD \
  "HDEC    "

#define QSH_CLT_HRX_STR_PAD \
  "HRX     "

#define QSH_CLT_HTX_STR_PAD \
  "HTX     "

#define QSH_CLT_HSRCH_STR_PAD \
  "HSRCH   "

#define QSH_CLT_CM_STR_PAD \
  "CM      "

#define QSH_CLT_SD_STR_PAD \
  "SD      "

#define QSH_CLT_MMOC_STR_PAD \
  "MMOC    "

#define QSH_CLT_PM_STR_PAD \
  "PM      "

#define QSH_CLT_REG_STR_PAD \
  "REG     "

#define QSH_CLT_MM_STR_PAD \
  "MM      "

#define QSH_CLT_CNM_STR_PAD \
  "CNM     "

#define QSH_CLT_MN_STR_PAD \
  "MN      "

#define QSH_CLT_SM_STR_PAD \
  "SM      "

#define QSH_CLT_DS3GPP_STR_PAD \
  "DS3GPP  "

#define QSH_CLT_DS_STR_PAD \
  "DS      "

#define QSH_CLT_IPA_STR_PAD \
  "IPA     "

#define QSH_CLT_TRM_STR_PAD \
  "TRM     "

#define QSH_CLT_TCXOMGR_STR_PAD \
  "TCXOMGR "

#define QSH_CLT_SLPC_STR_PAD \
  "SLPC    "

#define QSH_CLT_VSTMR_STR_PAD \
  "VSTMR   "

#define QSH_CLT_FWS_STR_PAD \
  "FWS     "

#define QSH_CLT_MSGR_STR_PAD \
  "MSGR    "

#define QSH_CLT_HWIO_STR_PAD \
  "HWIO    "

#define QSH_CLT_CFCM_STR_PAD \
  "CFCM    "

#define QSH_CLT_SMS_STR_PAD \
  "SMS     "

#define QSH_CLT_MEDIA_STR_PAD \
  "MEDIA   "

#define QSH_CLT_USIM_STR_PAD \
  "USIM    "

#define QSH_CLT_UTLKIT_STR_PAD \
  "UTLKIT  "

#define QSH_CLT_UDRVRS_STR_PAD \
  "UDRVRS  "

#define QSH_CLT_UPBM_STR_PAD \
  "UPBM    "

#define QSH_CLT_UQMI_STR_PAD \
  "UQMI    "

#define QSH_CLT_AP_STR_PAD \
  "AP      "

#define QSH_CLT_HOST_STR_PAD \
  "HOST    "

#define QSH_CLT_EMBMS_STR_PAD \
  "EMBMS   "

#define QSH_CLT_VOLTE_STR_PAD \
  "VOLTE   "

#define QSH_CLT_RF_STR_PAD \
  "RF      "

#define QSH_CLT_OTHER_STR_PAD \
  "OTHER   "

#define QSH_CLT_ALL_STR_PAD \
  "ALL     "

#define QSH_CLT_QSH_STR_PAD \
  "QSH     "

#define QSH_CLT_QRARB_STR_PAD \
  "QRARB   "

#define QSH_CLT_CAFI_STR_PAD \
  "CAFI    "

#define QSH_CLT_MPERF_STR_PAD \
  "MPERF   "

#define QSH_CLT_WMS_STR_PAD \
  "WMS     "

#define QSH_CLT_NR5GRF_STR_PAD \
  "NR5GRF  "

#define QSH_CLT_NR5GFW_STR_PAD \
  "NR5GFW  "

#define QSH_CLT_NR5GML1_STR_PAD \
  "NR5GML1 "

#define QSH_CLT_NR5GMAC_STR_PAD \
  "NR5GMAC "

#define QSH_CLT_NR5GL2UL_STR_PAD \
  "NR5GL2UL"

#define QSH_CLT_NR5GL2DL_STR_PAD \
  "NR5GL2DL"

#define QSH_CLT_NR5GRRC_STR_PAD \
  "NR5GRRC "

#define QSH_CLT_PLT_STR_PAD \
  "PLT     "

#define QSH_CLT_IMS_STR_PAD \
  "IMS     "

#define QSH_CLT_LOC_SM_STR_PAD \
  "LOC_SM  "

#define QSH_CLT_LOC_ME_STR_PAD \
  "LOC_ME  "

#define QSH_CLT_LOC_PE_STR_PAD \
  "LOC_PE  "

#define QSH_CLT_LOC_AON_STR_PAD \
  "LOC_AON "

#define QSH_CLT_BSI_STR_PAD \
  "BSI     "

#define QSH_CLT_LSM_STR_PAD \
  "LSM     "

#define QSH_CLT_L2LB_STR_PAD \
  "L2LB    "

#define QSH_CLT_RTP_STR_PAD \
  "RTP     "

#define QSH_CLT_ENL2DL_STR_PAD \
  "ENL2DL  "

#define QSH_CLT_ENL2UL_STR_PAD \
  "ENL2UL  "

#define QSH_CLT_CMAPI_STR_PAD \
  "CMAPI   "

#define QSH_CLT_LMTSMGR_STR_PAD \
  "LMTSMGR "

#define QSH_CLT_MCS_STR_PAD \
  "MCS     "

#define QSH_CLT_MCFG_STR_PAD \
  "MCFG    "

#define QSH_CLT_V2XL1_STR_PAD \
  "V2XL1   "

#define QSH_CLT_MSF_STR_PAD \
  "MSF     "

#define QSH_CLT_MACE_STR_PAD \
  "MACE    "

#define QSH_CLT_THERMAL_STR_PAD \
  "THERMAL "

#define QSH_CLT_ENL1_STR_PAD \
  "ENL1    "

#define QSH_CLT_SSGCCS_STR_PAD \
  "SSGCCS  "

#define QSH_CLT_SSGSCA_STR_PAD \
  "SSGSCA  "

#define QSH_CLT_V2XRRC_STR_PAD \
  "V2XRRC  "

#define QSH_CLT_V2XL2_STR_PAD \
  "V2XL2   "

#endif /* QSH_TYPES_CLT_AUTOGEN_H */
