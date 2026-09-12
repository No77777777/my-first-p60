#ifndef __PMAPP_NPA_H__
#define __PMAPP_NPA_H__

/*! \file  pmapp_npa.h
*  
 *  \brief  File Contains the PMIC NPA CMI Code
 *  \details  This file contains the needed definition and enum for PMIC NPA layer.
*
 *    PMIC code generation Version: 1.0.0.0
 *    PMIC code generation Locked Version: MDM9655_NPA_09102015 - LOCKED

 *    This file contains code for Target specific settings and modes.
*
 *  &copy; Copyright 2016-2020 Qualcomm Technologies, All Rights Reserved
*/

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pmapp_npa.h#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/

/*===========================================================================

                        DEFINITION

===========================================================================*/


/*===========================================================================

                        DEFINITIONS

===========================================================================*/
/** @addtogroup pmapp_npa
@{ */

/*
Generic GROUP IDs/Mode IDs that can be used by clients to configure the resources in
ACTIVE/STANDBY states.
 */

#define PMIC_NPA_GROUP_ID_QFPROM               "/pmic/client/qfprom"
#define PMIC_NPA_GROUP_ID_RF_TECH_ANT_SWITCH   "/pmic/client/rf_tech_ant_switch"
#define PMIC_NPA_GROUP_ID_RF_TECH_GPS          "/pmic/client/rf_tech_gps"
#define PMIC_NPA_GROUP_ID_RF1_TECH_GPS_ELNA    "/pmic/client/rf1_tech_gps_elna"
#define PMIC_NPA_GROUP_ID_RF_TECH_INIT         "/pmic/client/rf_tech_init"
#define PMIC_NPA_GROUP_ID_RF_TECH_DLS          "/pmic/client/rf_tech_dls"
#define PMIC_NPA_GROUP_ID_RF_TECH_WAN_QLINK    "/pmic/client/rf_tech_wan_qlink"
#define PMIC_NPA_GROUP_ID_RF_TECH_COMMON       "/pmic/client/rf_tech_common"
#define PMIC_NPA_GROUP_ID_RF_TECH_MMWAVE       "/pmic/client/rf_tech_mmwave"
#define PMIC_NPA_GROUP_ID_RF_TECH_SUB6         "/pmic/client/rf_tech_sub6"
#define PMIC_NPA_GROUP_ID_UIM1_ACTIVITY        "/pmic/client/uim1_activity"
#define PMIC_NPA_GROUP_ID_UIM1_ENABLE          "/pmic/client/uim1_enable"
#define PMIC_NPA_GROUP_ID_UIM2_ACTIVITY        "/pmic/client/uim2_activity"
#define PMIC_NPA_GROUP_ID_UIM2_ENABLE          "/pmic/client/uim2_enable"
#define PMIC_NPA_GROUP_ID_UIM3_ACTIVITY        "/pmic/client/uim3_activity"
#define PMIC_NPA_GROUP_ID_UIM3_ENABLE          "/pmic/client/uim3_enable"
#define PMIC_NPA_GROUP_ID_WCN_ANT_SWITCH       "/pmic/client/wcn_ant_switch"
#define PMIC_NPA_GROUP_ID_WCN_ANT_SWITCH_2     "/pmic/client/wcn_ant_switch_2"
#define PMIC_NPA_GROUP_ID_WCN_CORE             "/pmic/client/wcn_core"
#define PMIC_NPA_GROUP_ID_WCN_PA               "/pmic/client/wcn_pa"
#define PMIC_NPA_GROUP_ID_WCN_PA_2             "/pmic/client/wcn_pa_2"
#define PMIC_NPA_GROUP_ID_WCN_ACTIVITY         "/pmic/client/wcn_activity"
#define PMIC_NPA_GROUP_ID_WCSS                 "/pmic/client/wcss"
#define PMIC_NPA_GROUP_ID_XO                   "/pmic/client/xo"
#define PMIC_NPA_GROUP_ID_PM_INIT              "/pmic/client/pm_init"
#define PMIC_NPA_GROUP_ID_VDD_MSS              "/pmic/client/vdd_mss"
#define PMIC_NPA_GROUP_ID_RF1_TECH_VREF_MSM    "/pmic/client/rf1_tech_vref_msm"
#define PMIC_NPA_GROUP_ID_WCN_RF_CLK           "/pmic/client/wcn_rf_clk"



#define PMIC_NPA_GROUP_ID_RF1_TECH_1X          "/pmic/client/rf1_tech_1x"
#define PMIC_NPA_GROUP_ID_RF1_TECH_INIT        "/pmic/client/rf1_tech_init"
#define PMIC_NPA_GROUP_ID_RF1_TECH_GPS         "/pmic/client/rf1_tech_gps"
#define PMIC_NPA_GROUP_ID_WCN_CORE_2           "/pmic/client/wcn_core_2"
#define PMIC_NPA_GROUP_ID_MEM_UVOL             "/pmic/client/vdd_mem_uvol"

//Query id to query PAM client resources
//usage: query_status = npa_query_by_name(PMIC_NPA_GROUP_ID_QFPROM, PM_PRM_QUERY_PAM_CLIENT, &query_result ); 
//return: query_result.data.value, TRUE : all resources set correctly
#define PM_PRM_QUERY_PAM_CLIENT  0x5000
/*===========================================================================

                        ENUMERATION

===========================================================================*/
/**
Vote                                  PMIC XO  PMIC XO Buffer  SOC CXO PAD
PMIC_NPA_MODE_ID_CLK_CXO_XO_OFF       OFF      OFF             OFF
PMIC_NPA_MODE_ID_CLK_CXO_BUFFER_OFF    ON      OFF             OFF
PMIC_NPA_MODE_ID_CLK_SOC_CXO_PAD_OFF   ON       ON             OFF
PMIC_NPA_MODE_ID_CLK_CXO_XO_ON         ON       ON              ON
*/
enum
{
   PMIC_NPA_MODE_ID_CLK_CXO_XO_OFF         = 0,
   PMIC_NPA_MODE_ID_CLK_CXO_BUFFER_OFF     = 1,
   PMIC_NPA_MODE_ID_CLK_SOC_CXO_PAD_OFF    = 2,
   PMIC_NPA_MODE_ID_CLK_CXO_XO_ON          = 3,
   PMIC_NPA_MODE_ID_XO_MAX                 = 4,
};

enum
{
    PMIC_NPA_MODE_ID_GENERIC_OFF = 0,
    PMIC_NPA_MODE_ID_GENERIC_STANDBY = 1,
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE = 2,
    PMIC_NPA_MODE_ID_GENERIC_LV = PMIC_NPA_MODE_ID_GENERIC_ACTIVE,
    PMIC_NPA_MODE_ID_GENERIC_MV = 3,
    PMIC_NPA_MODE_ID_GENERIC_HV = 4,
    PMIC_NPA_MODE_ID_GENERIC_LOW_LV = 5,
    PMIC_NPA_MODE_ID_GENERIC_LOW_MV = 6,
    PMIC_NPA_MODE_ID_GENERIC_LOW_HV = 7,
    PMIC_NPA_MODE_ID_GENERIC_MAX,
};

enum
{
   PMIC_NPA_MODE_ID_UIM_STANDBY = 0,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_LOW = 1,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C = 2,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_HIGH = 3,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_LOW = 4,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B = 5,
   PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH = 6,
   PMIC_NPA_MODE_ID_UIM_MAX = 7,
};


//temporary modes till clients move to newer modes for all targets

/** Generic modes used by all RF1_TECH GROUP IDs. */
enum
{
   PMIC_NPA_MODE_ID_RF_MODE_0   = 0, /**< SLEEP */
   PMIC_NPA_MODE_ID_RF_MODE_1   = 1, /**< DVS_LOW */
   PMIC_NPA_MODE_ID_RF_MODE_2   = 2, /**< DVS_HIGH */
   PMIC_NPA_MODE_ID_RF_MODE_3   = 3, /**< optional DVS_LOW for 2nd RF card*/
   PMIC_NPA_MODE_ID_RF_MODE_4   = 4, /**< optional DVS_HIGH for 2nd RF card*/
   PMIC_NPA_MODE_ID_RF_MODE_5   = 5, /**< Not used */
   PMIC_NPA_MODE_ID_RF_MODE_6   = 6, /**< Not used */
   PMIC_NPA_MODE_ID_RF_MODE_MAX = 7, /**< Max */
};

/** Generic modes used by UIM GROUP IDs. */
enum
{
   PMIC_NPA_MODE_ID_UIM_MODE_1 = 0, /**< STANDBY/NO_VOTE */
   PMIC_NPA_MODE_ID_UIM_MODE_2 = 1, /**< ACTIVE/UIM_CLASS_C_LOW */
   PMIC_NPA_MODE_ID_UIM_MODE_3 = 2, /**< UIM_CLASS_C */
   PMIC_NPA_MODE_ID_UIM_MODE_4 = 3, /**< ACTIVE/UIM_CLASS_C_HIGH */
   PMIC_NPA_MODE_ID_UIM_MODE_5 = 4, /**< UIM_CLASS_B_LOW */
   PMIC_NPA_MODE_ID_UIM_MODE_6 = 5, /**< UIM_CLASS_B */
   PMIC_NPA_MODE_ID_UIM_MODE_7 = 6, /**< UIM_CLASS_B_HIGH */
   PMIC_NPA_MODE_ID_UIM_MODE_8 = 7, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_9 = 8, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_10 = 9, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_11 = 10, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_12 = 11, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_13 = 12, /**< Not used */
   PMIC_NPA_MODE_ID_UIM_MODE_MAX = 13, /**< Max */
};

enum
{
   PMIC_NPA_MODE_ID_WCN_MODE_0,
   PMIC_NPA_MODE_ID_WCN_MODE_1,
   PMIC_NPA_MODE_ID_WCN_MODE_2,
   PMIC_NPA_MODE_ID_WCN_MODE_3,
   PMIC_NPA_MODE_ID_WCN_MODE_4,
   PMIC_NPA_MODE_ID_WCN_MODE_5,
   PMIC_NPA_MODE_ID_WCN_MODE_MAX,
};


/** @} */ /* end_addtogroup pmapp_npa */
#endif // __PMAPP_NPA_H__

