
/*! \file  pm_config_pam.c
 *  
 *  \brief  File Contains the PMIC NPA PAM Data
 *  \details  This file contains the PAM data for the PMIC NPA layer.
 *  
 *    Platform: Kamorta.XX.1.0
 *    Revision: KAMORTA.XX.1.0-MODEM-00003
 *    Template Version: 
 *    $Header: //components/rel/core.mpss/10.0/settings/pmic/pm/config/kamorta/pm_config_pam.c#7 $ 
 *    $DateTime: 2020/04/28 03:26:25 $  $Author: pwbldsvc $
 *  
 *  &copy; Copyright Feb. 12, 2020 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                        INCLUDE HEADER FILES

===========================================================================*/

#include "pm_npa_device.h"
#include "pm_npa_device_clk_buff.h"
#include "pm_npa_device_ldo.h"
#include "pm_npa_device_smps.h"
#include "pmapp_npa.h"
#include "npa_remote_resource.h"
#include "pm_npa_device_vs.h"

/*===========================================================================

                        MACRO DEFINITIONS

===========================================================================*/

#define PMIC_NPA_CLIENT_NODE_QFPROM "/node/pmic/client/qfprom"
#define PMIC_NPA_CLIENT_NODE_RF1_TECH_1X "/node/pmic/client/rf1_tech_1x"
#define PMIC_NPA_CLIENT_NODE_RF1_TECH_GPS "/node/pmic/client/rf1_tech_gps"
#define PMIC_NPA_CLIENT_NODE_RF1_TECH_GPS_ELNA "/node/pmic/client/rf1_tech_gps_elna"
#define PMIC_NPA_CLIENT_NODE_RF1_TECH_INIT "/node/pmic/client/rf1_tech_init"
#define PMIC_NPA_CLIENT_NODE_RF1_TECH_VREF_MSM "/node/pmic/client/rf1_tech_vref_msm"
#define PMIC_NPA_CLIENT_NODE_UIM1_ACTIVITY "/node/pmic/client/uim1_activity"
#define PMIC_NPA_CLIENT_NODE_UIM1_ENABLE "/node/pmic/client/uim1_enable"
#define PMIC_NPA_CLIENT_NODE_UIM2_ACTIVITY "/node/pmic/client/uim2_activity"
#define PMIC_NPA_CLIENT_NODE_UIM2_ENABLE "/node/pmic/client/uim2_enable"
#define PMIC_NPA_CLIENT_NODE_WCN_ACTIVITY "/node/pmic/client/wcn_activity"
#define PMIC_NPA_CLIENT_NODE_WCN_CORE "/node/pmic/client/wcn_core"
#define PMIC_NPA_CLIENT_NODE_WCN_PA "/node/pmic/client/wcn_pa"
#define PMIC_NPA_CLIENT_NODE_WCSS "/node/pmic/client/wcss"
/*===========================================================================

                        VARIABLES DEFINITION

===========================================================================*/

/*************************** QFPROM Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_qfprom_a_ldo12 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_qfprom_rails_info [] =
{
   {
      (void*)pm_pam_qfprom_a_ldo12,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_qfprom [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 12 ),
};

/*************************** RF1_TECH_1X Client ***************************/

static const pm_npa_clk_buff_kvps
pm_pam_rf1_tech_1x_a_clk_buff2 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
};
static const pm_npa_clk_buff_kvps
pm_pam_rf1_tech_1x_a_clk_buff3 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo1 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo9 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo12 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo13 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo18 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_1x_a_ldo21 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP: LV_WAN=OFF; HV_WAN=OFF; BBRx_LDO=OFF, RF_2P7_LDO=OFF; RFCLK1=OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2704000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_CLK1 on, L16/LDO18 off (use for GPS only scenarios)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2704000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_CLK1 on, L16/LDO18 on (use if non GPS tech active)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2704000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_rf1_tech_1x_rails_info [] =
{
   {
      (void*)pm_pam_rf1_tech_1x_a_clk_buff2,
      PM_NPA_CLK_BUFF
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_clk_buff3,
      PM_NPA_CLK_BUFF
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo1,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo9,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo12,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo13,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo18,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_1x_a_ldo21,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_rf1_tech_1x [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  clk, 2 ),
      PMIC_NPA_NODE_DEP_LIST( A,  clk, 3 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 1 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 9 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 12 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 13 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 18 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 21 ),
};

/*************************** RF1_TECH_GPS Client ***************************/

static const pm_npa_clk_buff_kvps
pm_pam_rf1_tech_gps_a_clk_buff3 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_a_ldo1 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_a_ldo9 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_a_ldo12 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_a_ldo13 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_a_ldo18 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_rf1_tech_gps_rails_info [] =
{
   {
      (void*)pm_pam_rf1_tech_gps_a_clk_buff3,
      PM_NPA_CLK_BUFF
   },
   {
      (void*)pm_pam_rf1_tech_gps_a_ldo1,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_a_ldo9,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_a_ldo12,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_a_ldo13,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_a_ldo18,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_rf1_tech_gps [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  clk, 3 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 1 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 9 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 12 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 13 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 18 ),
};

/*************************** RF1_TECH_GPS_ELNA Client ***************************/

static const pm_npa_clk_buff_kvps
pm_pam_rf1_tech_gps_elna_a_clk_buff3 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
   },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo1 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo9 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo12 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo13 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo17 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1304000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1304000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_rf1_tech_gps_elna_a_ldo18 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: RF_MODE1: RFCLK1=ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1200000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_rf1_tech_gps_elna_rails_info [] =
{
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_clk_buff3,
      PM_NPA_CLK_BUFF
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo1,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo9,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo12,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo13,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo17,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_rf1_tech_gps_elna_a_ldo18,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_rf1_tech_gps_elna [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  clk, 3 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 1 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 9 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 12 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 13 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 17 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 18 ),
};

/*************************** RF1_TECH_INIT Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_rf1_tech_init_a_ldo1 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: RF_SLEEP;
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: WTR v2.0, Nominal part
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: WTR v2.0, SS part
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1048000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_rf1_tech_init_rails_info [] =
{
   {
      (void*)pm_pam_rf1_tech_init_a_ldo1,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_rf1_tech_init [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 1 ),
};

/*************************** RF1_TECH_VREF_MSM Client ***************************/

static const pm_npa_vs_kvps
pm_pam_rf1_tech_vref_msm_a_vs1 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
   },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
   },
};

static const pm_npa_pam_client_cfg_type
pm_pam_rf1_tech_vref_msm_rails_info [] =
{
   {
      (void*)pm_pam_rf1_tech_vref_msm_a_vs1,
      PM_NPA_VREG_VS
   },
};

static npa_node_dependency
pm_pam_deps_rf1_tech_vref_msm [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  vs, 1 ),
};

/*************************** UIM1_ACTIVITY Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_uim1_activity_a_ldo19 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: UIM_STANDBY
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: UIM_ACTIVE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_uim1_activity_rails_info [] =
{
   {
      (void*)pm_pam_uim1_activity_a_ldo19,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_uim1_activity [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 19 ),
};

/*************************** UIM1_ENABLE Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_uim1_enable_a_ldo7 [] =
{
   // Mode: PMIC_NPA_MODE_ID_UIM_STANDBY
   // Comments: UIM_NO_VOTE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_LOW
   // Comments: UIM_CLASS_C_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C
   // Comments: UIM_CLASS_C
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_HIGH
   // Comments: UIM_CLASS_C_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_LOW
   // Comments: UIM_CLASS_B_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B
   // Comments: UIM_CLASS_B
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH
   // Comments: UIM_CLASS_B_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_uim1_enable_a_ldo19 [] =
{
   // Mode: PMIC_NPA_MODE_ID_UIM_STANDBY
   // Comments: UIM_NO_VOTE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_LOW
   // Comments: UIM_CLASS_C_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1712000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C
   // Comments: UIM_CLASS_C
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_HIGH
   // Comments: UIM_CLASS_C_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1888000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_LOW
   // Comments: UIM_CLASS_B_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2848000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B
   // Comments: UIM_CLASS_B
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2928000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH
   // Comments: UIM_CLASS_B_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2928000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_uim1_enable_rails_info [] =
{
   {
      (void*)pm_pam_uim1_enable_a_ldo7,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_uim1_enable_a_ldo19,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_uim1_enable [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 7 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 19 ),
};

/*************************** UIM2_ACTIVITY Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_uim2_activity_a_ldo20 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: UIM_STANDBY
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: UIM_ACTIVE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_uim2_activity_rails_info [] =
{
   {
      (void*)pm_pam_uim2_activity_a_ldo20,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_uim2_activity [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 20 ),
};

/*************************** UIM2_ENABLE Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_uim2_enable_a_ldo7 [] =
{
   // Mode: PMIC_NPA_MODE_ID_UIM_STANDBY
   // Comments: UIM_NO_VOTE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_LOW
   // Comments: CLASS_C_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C
   // Comments: UIM_CLASS_C
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_HIGH
   // Comments: UIM_CLASS_C_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_LOW
   // Comments: UIM_CLASS_B_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B
   // Comments: UIM_CLASS_B
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH
   // Comments: UIM_CLASS_B_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1256000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_uim2_enable_a_ldo20 [] =
{
   // Mode: PMIC_NPA_MODE_ID_UIM_STANDBY
   // Comments: UIM_NO_VOTE
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_LOW
   // Comments: CLASS_C_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1712000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C
   // Comments: UIM_CLASS_C
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_C_HIGH
   // Comments: UIM_CLASS_C_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1888000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_LOW
   // Comments: UIM_CLASS_B_LOW
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2848000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B
   // Comments: UIM_CLASS_B
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2928000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH
   // Comments: UIM_CLASS_B_HIGH
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 2928000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_uim2_enable_rails_info [] =
{
   {
      (void*)pm_pam_uim2_enable_a_ldo7,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_uim2_enable_a_ldo20,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_uim2_enable [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 7 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 20 ),
};

/*************************** WCN_ACTIVITY Client ***************************/

static const pm_npa_smps_kvps
pm_pam_wcn_activity_a_smps6 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: Disable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_AUTO,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: Enable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: Enable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
};

static const pm_npa_smps_kvps
pm_pam_wcn_activity_a_smps7 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: Disable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_AUTO,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: Enable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: Enable PWM Mode
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_SMPS_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_NPM,  /**< [Retention, LPM, AUTO(default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0,  /**< [X uV] -> max aggregation. */
    },
};

static const pm_npa_pam_client_cfg_type
pm_pam_wcn_activity_rails_info [] =
{
   {
      (void*)pm_pam_wcn_activity_a_smps6,
      PM_NPA_VREG_SMPS
   },
   {
      (void*)pm_pam_wcn_activity_a_smps7,
      PM_NPA_VREG_SMPS
   },
};

static npa_node_dependency
pm_pam_deps_wcn_activity [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  smps, 6 ),
      PMIC_NPA_NODE_DEP_LIST( A,  smps, 7 ),
};

/*************************** WCN_CORE Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_wcn_core_a_ldo16 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: OFF(Disable, 0V)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: disable, V, LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: Enable, V, LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1800000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_ldo_kvps
pm_pam_wcn_core_a_ldo17 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: OFF(Disable, 0V)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: disable, V, LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1304000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: Enable, V, LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 1304000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_wcn_core_rails_info [] =
{
   {
      (void*)pm_pam_wcn_core_a_ldo16,
      PM_NPA_VREG_LDO
   },
   {
      (void*)pm_pam_wcn_core_a_ldo17,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_wcn_core [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 16 ),
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 17 ),
};

/*************************** WCN_PA Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_wcn_pa_a_ldo23 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: OFF
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: disable, 3.0V, LPM)
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 3000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_LV
   // Comments: Enable, 3.0V, LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 3000000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_MV
   // Comments: Enable, 3.152V,LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 3152000, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_HV
   // Comments: Enable, 3.312V,LPM
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 3312000, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_wcn_pa_rails_info [] =
{
   {
      (void*)pm_pam_wcn_pa_a_ldo23,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_wcn_pa [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 23 ),
};

/*************************** WCSS Client ***************************/

static const pm_npa_ldo_kvps
pm_pam_wcss_a_ldo8 [] =
{
   // Mode: PMIC_NPA_MODE_ID_GENERIC_OFF
   // Comments: off
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_DISABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_STANDBY
   // Comments: ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
   // Mode: PMIC_NPA_MODE_ID_GENERIC_ACTIVE
   // Comments: ON
   {
      PM_NPA_KEY_SOFTWARE_ENABLE, 4, PM_NPA_GENERIC_ENABLE,  /**< [Disable (default), Enable] -> max aggregation (left to right). */
      PM_NPA_KEY_LDO_SOFTWARE_MODE, 4, PM_NPA_VREG_MODE_LPM,  /**< [Retention, LPM (default), NPM] -> max aggregation (left to right). */
      PM_NPA_KEY_MICRO_VOLT, 4, 0, /**< [X uV] -> max aggregation. */
    },
};
static const pm_npa_pam_client_cfg_type
pm_pam_wcss_rails_info [] =
{
   {
      (void*)pm_pam_wcss_a_ldo8,
      PM_NPA_VREG_LDO
   },
};

static npa_node_dependency
pm_pam_deps_wcss [] =
{
      PMIC_NPA_NODE_DEP_LIST( A,  ldo, 8 ),
};

/*************************** NPA NODE RESOURCE ***************************/

static pm_npa_node_resource_info 
pm_npa_pam_node_rsrcs [] =
{
   {
      PMIC_NPA_GROUP_ID_QFPROM, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_qfprom_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_QFPROM, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_qfprom, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_qfprom),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_RF1_TECH_1X, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_DEFAULT, // Resource Attribute
      (void*) pm_pam_rf1_tech_1x_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_RF1_TECH_1X, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_rf1_tech_1x, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_rf1_tech_1x),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_RF1_TECH_GPS, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_rf1_tech_gps_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_RF1_TECH_GPS, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_rf1_tech_gps, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_rf1_tech_gps),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_RF1_TECH_GPS_ELNA, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_rf1_tech_gps_elna_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_RF1_TECH_GPS_ELNA, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_rf1_tech_gps_elna, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_rf1_tech_gps_elna),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_RF1_TECH_INIT, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_DEFAULT, // Resource Attribute
      (void*) pm_pam_rf1_tech_init_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_RF1_TECH_INIT, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_rf1_tech_init, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_rf1_tech_init),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_RF1_TECH_VREF_MSM, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_DEFAULT, // Resource Attribute
      (void*) pm_pam_rf1_tech_vref_msm_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_RF1_TECH_VREF_MSM, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_rf1_tech_vref_msm, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_rf1_tech_vref_msm),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_UIM1_ACTIVITY, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_uim1_activity_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_UIM1_ACTIVITY, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_uim1_activity, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_uim1_activity),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_UIM1_ENABLE, // Resource Name
      PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_uim1_enable_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_UIM1_ENABLE, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_uim1_enable, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_uim1_enable),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_UIM2_ACTIVITY, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_uim2_activity_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_UIM2_ACTIVITY, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_uim2_activity, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_uim2_activity),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_UIM2_ENABLE, // Resource Name
      PMIC_NPA_MODE_ID_UIM_ACTIVE_CLASS_B_HIGH,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_uim2_enable_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_UIM2_ENABLE, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_uim2_enable, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_uim2_enable),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_WCN_ACTIVITY, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_wcn_activity_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_WCN_ACTIVITY, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_wcn_activity, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_wcn_activity),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_WCN_CORE, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_wcn_core_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_WCN_CORE, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_wcn_core, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_wcn_core),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_WCN_PA, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_HV,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_wcn_pa_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_WCN_PA, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_wcn_pa, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_wcn_pa),  // Deps Count
   },
   {
      PMIC_NPA_GROUP_ID_WCSS, // Resource Name
      PMIC_NPA_MODE_ID_GENERIC_ACTIVE,   // Maximum Value
      NPA_RESOURCE_SINGLE_CLIENT | NPA_RESOURCE_DRIVER_UNCONDITIONAL, // Resource Attribute
      (void*) pm_pam_wcss_rails_info, // Resource User Data
      PMIC_NPA_CLIENT_NODE_WCSS, // Node Name
      NPA_NODE_DEFAULT, // Node Attributes
      NULL,
      pm_pam_deps_wcss, // Node Dependency
      NPA_ARRAY_SIZE(pm_pam_deps_wcss),  // Deps Count
   },
};

static uint32 num_of_pam_rsrcs = sizeof(pm_npa_pam_node_rsrcs) / sizeof(pm_npa_node_resource_info);


/*************************** Device Layer Resources ***************************/

static pm_npa_remote_name_type
pmic_npa_clk_remote_resources [] =
{
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, clk, 2), "clka\x02\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, clk, 3), "clka\x03\x00\x00\x00" },
};

static pm_npa_remote_resource_type
pmic_npa_remote_clk = 
{
      pmic_npa_clk_remote_resources,
      sizeof(pmic_npa_clk_remote_resources) / sizeof(pm_npa_remote_name_type)
};

static pm_npa_remote_name_type
pmic_npa_ldo_remote_resources [] =
{
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 1), "ldoa\x01\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 7), "ldoa\x07\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 8), "ldoa\x08\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 9), "ldoa\x09\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 12), "ldoa\x0C\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 13), "ldoa\x0D\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 16), "ldoa\x10\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 17), "ldoa\x11\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 18), "ldoa\x12\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 19), "ldoa\x13\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 20), "ldoa\x14\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 21), "ldoa\x15\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, ldo, 23), "ldoa\x17\x00\x00\x00" },
};

static pm_npa_remote_resource_type
pmic_npa_remote_ldo = 
{
      pmic_npa_ldo_remote_resources,
      sizeof(pmic_npa_ldo_remote_resources) / sizeof(pm_npa_remote_name_type)
};

static pm_npa_remote_name_type
pmic_npa_smps_remote_resources [] =
{
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, smps, 6), "smpa\x06\x00\x00\x00" },
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, smps, 7), "smpa\x07\x00\x00\x00" },
};

static pm_npa_remote_resource_type
pmic_npa_remote_smps = 
{
      pmic_npa_smps_remote_resources,
      sizeof(pmic_npa_smps_remote_resources) / sizeof(pm_npa_remote_name_type)
};

static pm_npa_remote_name_type
pmic_npa_vs_remote_resources [] =
{
   { PMIC_DEV_RSRC_NAME_VEC_IN( A, vs, 1), "vsa\x00\x01\x00\x00\x00" },
};

static pm_npa_remote_resource_type
pmic_npa_remote_vs = 
{
      pmic_npa_vs_remote_resources,
      sizeof(pmic_npa_vs_remote_resources) / sizeof(pm_npa_remote_name_type)
};

/*************************** Published resources ***************************/
static char *pmic_publish_resources[] =
{
    PMIC_NPA_GROUP_ID_WCN_CORE, 
    PMIC_NPA_GROUP_ID_WCN_PA, 
    PMIC_NPA_GROUP_ID_WCSS
};

static pm_npa_resource_publish_type pm_npa_pub_pam_rsrcs = 
{
   NPA_ALL_PDS,
   pmic_publish_resources,
   sizeof(pmic_publish_resources)/sizeof(char *)
};

/****************************** PAM data ************************************/
pm_npa_pam_data_type 
pm_pam_data =
{
  .pm_npa_pam_rsrcs            = pm_npa_pam_node_rsrcs,
  .num_of_pam_rsrcs            = &num_of_pam_rsrcs,
  .pmic_npa_remote_clk         = &pmic_npa_remote_clk,
  .pmic_npa_remote_ldo         = &pmic_npa_remote_ldo,
  .pmic_npa_remote_smps        = &pmic_npa_remote_smps,
  .pmic_npa_remote_vs          = &pmic_npa_remote_vs,
  .pm_npa_pub_pam_rsrcs        = &pm_npa_pub_pam_rsrcs
};

