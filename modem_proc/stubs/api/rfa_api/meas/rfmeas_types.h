#ifndef RFMEAS_TYPES_H
#define RFMEAS_TYPES_H

/*
   @file
   rfm_meas.h

   @brief
   RF Driver's Inter Freq Meas external interface file.

   @details

*/

/*===========================================================================
Copyright (c) 2010 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfa_api/meas/rfmeas_types.h#2 $
$DateTime: 2019/12/12 19:52:25 $
$Author: prak $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/15   hk      Moving the longer rx_alpha to the end of the structure.
10/22/15   cs      Ensure longer rx_alpha available for DR contention
10/14/15   sb      Support for PRS measurement Gap to measure
                   cells with lower BW compared to current
09/10/15   cj      Added SRX QCTA support
07/27/15   asn     New Power ON OFF API
06/29/15   sw      Return power meas gamma to L1 (for IRAT X2G)
06/25/15   cj      L2L Interface change
05/13/15   sw      Device for TQP allocation returned to GL1
05/07/15   cj      Rx-TX split changes
05/06/15   nv      Adding logging support to log rfm meas APIs and their parameters
04/28/15   cj      Added changes for QDTA
04/24/15   rmb     Merge back some of the changes from mainline
04/14/15   nv      Added changes for Half scripts and NULL2X scripts
04/02/15   sw      Re-order fields in header - move deprecated fields to the end
03/27/15   zhw     Add support for scenario type, cell enum and Rx enum indexing
03/31/15   nv      First version of CRAT FTM GUI
03/27/15   ych     Add TDS tech meas param type
03/26/15   zhw     Convert tech params from pointer to unions to unions
03/26/15   sw      Define GSM  tech meas param type (part2)
03/16/15   nv      Updated structure for rfm_meas_generic_params_type
03/16/15   kab     Add LTE tech meas param type
03/13/15   sw      Define GSM tech meas param type
04/28/15   nv      Mainline the code under feature RF_HAS_MEAS_INTF_TH_2_0
04/17/15   nv      Added changes for Half scripts and NULL2X scripts
04/13/15   rmb     Remove new interfaces from #ifndef.
04/09/15   rmb     Bring back both old and new interfaces in #ifndef and #else
                   feature flag.
04/08/15   rmb     Put rfcom.h outside feature flag so that flag definition can be imported.
04/08/14   rmb     Merge Tips of Dev branch ont mainline.
04/07/15   nv      Correcting few changes of the previous checkin
04/07/15   nv      Merging the changes from CRAT DEV branch to mainline
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag
04/01/15   ych     Add TDS tech meas param type
03/11/15   zhw     Add 1x/HDR tech meas param type
03/11/15   rmb     Enum for NULL2X script and add union for tech related params for all tech.
                   Also, add a common params structure type common between Enter, Build Script and Exit param type.
02/04/15   mw      Interface additions for CRAT
01/05/15   gvn     Remove TH featurization to support 3DLCA
11/20/14   php     ML1-RF interface changes for IRAT to support HORxD/other fixes
10/21/14   sw      Introduce enums for DL paths
09/26/14   gvn     Support for 3DLCA gaps
07/31/14   ndb     Added support for extended EARFCN for LTE
05/13/14   rmb     Additional fields for WCDMA SBDC and SB3C support.
05/02/14   sc      Add subscription ID to rfm_meas_irat_info_type
12/11/13   rmb     Added pretune offset time for home and neighbour.
10/21/13   pl      Adding trigger type
10/15/13   pl      Adding preload type
10/15/13   pl      Update LTE PLL settling time
10/11/13   dw      Add pretune(preload) offset time in rfm_meas_irat_info_type
03/06/13   pl      Add struct tag to help with non-LTE builds
02/05/13   pl      Remove irat buffer from common header as per review comments
02/04/13   pl      Adding missed fields to IRAT common header structure
01/31/13   pl      IRAT common header restructure
01/22/13   pl      Implement code view comments
01/20/13   pl      FTM LTE-CA IRAT support
01/10/13   cj      Added source and target device id to rfm_meas_header_type structure
11/19/12   sc      RF-Only mDSP update shared variables cleanup
11/07/12   sc      Add common iRAT info params structure for types cleanup
10/30/12   sc      Cleanup common member ordering for casting
10/05/12   aa      Added wtow measurement parameter
04/04/12   av      Add is_measurement_gap_aborted flag to rfm_meas_exit_param_type to support Abort sequences.
01/17/12   av      Fix call flow for XtoG
10/21/11   zg      Added rfm_meas_ttog_irat_info_param_type
09/19/11   dw      Added rfm_meas_ltow_irat_info_param_type
07/26/11   ckl     Added W2W AGC and SSBI time in rfm_meas_wtow_irat_info_param_type
06/25/11   dw      Change W2W Pll settling time to uint16 type
05/27/11   dw      Add more IRAT info timing parameters support
04/08/11   sar     Removed condition #def's for CMI-4.
04/05/11   dw      GtoW/LtoW RxlM support
03/08/11   tnt     LM integration for IRAT
02/09/11   jyu     Added Level 1&2 RF FTM IRAT test
01/21/11   jfc     Include "rfc_card_gsmctl.h" to get defn of mdsp_burst_type
01/18/11   jfc     Featurize include of mdsp_intf.h under FEATURE_GSM
10/07/10   jyu     Added new type for rfm_meas_generic_params_type
09/29/10   av/jyu  Created new API rfm_meas_common_get_irat_info_param
05/20/10   tws     IRAT for MDM9K.
03/19/10   can     Initial common measurement APIs and support.
03/02/10   lcl     un-commented out startup and cleanup buffers
03/01/10   tws     Initial version
============================================================================*/


#include "rfm_mode_types.h"
#include "rfcom.h"

#include "mdsp_intf.h"
#include "lm_types.h"

#include "rfm_hdr_agc_types.h"
#include "rflte_msg.h"
#include "rf_tdscdma_msg.h"

#include "rfm_wcdma_param_types.h"

// Initial values for startup_time and cleanup_time calculation
#define NUM_OF_MAX_SBI_BEFORE_WRITES 120
#define NUM_OF_MAX_SBI_AFTER_WRITES  60
#define SBI_WRITE_TIME_IN_NS 2200
// GENERIC_PLL_LOCK_TIME_IN_US used for GSM/WCDMA/1x/DO/LTE
#define GENERIC_PLL_LOCK_TIME_IN_US 200
#define LTE_PLL_LOCK_TIME_IN_US  GENERIC_PLL_LOCK_TIME_IN_US
// Values for Neighbour and Home Pretune offset time
#define NEIGHBOUR_PRETUNE_OFFSET_TIME_IN_US 1200
#define HOME_PRETUNE_OFFSET_TIME_IN_US 0
/* max number of devices per tx carrier */
#define MAX_TX_DEV_HANDLE_ITEM 1
/* max number of GSM rx devices (PRX + DRX) */
#define MAX_GSM_RX_DEV_HANDLE_ITEM 2
/* max number of Target rx devices (PRX + DRX) */
#define MAX_TARGET_RX_DEV_HANDLE_ITEM 2

/* Define DL paths for irat testing */
typedef enum
{
  RFM_MEAS_DL_PATH,
  RFM_MEAS_DL_PATH_LTE_CA_1,
#if !defined FEATURE_JOLOKIA_MODEM
  RFM_MEAS_DL_PATH_LTE_CA_2,
#endif
  RFM_MEAS_DL_PATH_MAX
} rfm_meas_src_params;

/* Define DL paths for irat testing */
typedef enum
{
  /*! Do not change this order and number assigned
    PRx always is zero */
  DEV_HANDLE_PRX = 0,
  DEV_HANDLE_DRX1 = 1,
  DEV_HANDLE_DRX2 = 2,
  DEV_HANDLE_DRX3 = 3,
  MAX_RX_DEV_HANDLE_ITEM
} rfm_meas_rx_dev_handle_type;

/* Define DL paths for irat testing */
typedef enum
{
 /*! Do not change this order and number assigned
    PCELL always is zero */
  RFM_MEAS_PCELL = 0,
  RFM_MEAS_SCELL_1 = 1,
  RFM_MEAS_SCELL_2 = 2,
  RFM_MEAS_CELL_NUM
} rfm_meas_carrier_type;


#define MAX_SOURCE_ITEM RFM_MEAS_CELL_NUM
#define MAX_NUMBER_CA_PATH RFM_MEAS_CELL_NUM

typedef enum
{
   RFM_MEAS_COMMON_FAILURE,
   RFM_MEAS_COMMON_SUCCESS,

} rfm_meas_result_type;

typedef enum
{
   RFM_MEAS_PROGRAM_STARTUP,
   RFM_MEAS_PROGRAM_BURSTS,
   RFM_MEAS_PROGRAM_CLEANUP,
   RFM_MEAS_PROGRAM_PRELOAD,
   RFM_MEAS_PROGRAM_TRIGGER,
   RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT,  /* for use with half scripts */
   RFM_MEAS_PROGRAM_NULL2X    /* For NULL2X script */
} rfm_meas_program_script_type;

typedef enum
{
   RFM_MEAS_SCENARIO_IRAT,
   RFM_MEAS_SCENARIO_QTA,
   RFM_MEAS_SCENARIO_QDTA,
   RFM_MEAS_SCENARIO_QCTA,
   RFM_MEAS_SCENARIO_SRX_TA,
   RFM_MEAS_SCENARIO_L2L_WIDEBAND,
   RFM_MEAS_SCENARIO_NUM
} rfm_meas_scenario_type;

typedef struct
{
    uint32 start_up_time;
    uint32 clean_up_time;
} rfm_meas_time_const_type;

typedef struct
{
   rfm_device_enum_type device;
   lm_handle_type  lm_handle;
} dev_handle_type;

/* Tech specific Structure for channel related paramters */

/* GSM */
typedef struct
{
  /*! @brief            rx devices (prx + drx) */
  dev_handle_type       rx_dev_handle[MAX_GSM_RX_DEV_HANDLE_ITEM];

  /*! @brief            target diversity enable/disable indicator  */
  uint16                diversity_en;

  /*! @brief            GSM band */
  rfcom_gsm_band_type   band;

  /*! @brief            source tec tx device*/
  dev_handle_type       tx_dev_handle[MAX_TX_DEV_HANDLE_ITEM];

  /*! @brief            subscription id for dual irat */
  uint8                 sub_id;

  /*! @brief            device to use for TQP allocation (X2G),
                        populated by RF and returned to GL1 */
  rfm_device_enum_type   tqp_dev;
} rfm_meas_gsm_params_type;

/*! @brief  This structure will be used for L<->1x  */
typedef struct
{
  sys_channel_type      band_chan;
  /*!<  Band and channel on which 1x will be tuned,
        regardless of the source or target tech type  */
} rfm_meas_1x_params_type;

/*! @brief  This structure will be used for L<->DO and G<->DO  */
typedef struct
{
  sys_channel_type      band_chan;
  /*!<  Band and channel on which HDR will be tuned,
        regardless of the source or target tech type  */

  rfm_hdr_agc_mode_type agc_mode;
  /*!< Specify HDR RxAGC Mode */

  uint32                agc_rtc_offset;
  /*!< Specify HDR RxAGC RTC Offset */

  int16                 prev_agc;
  /*!< Specify HDR RxAGC previous AGC value */

} rfm_meas_hdr_params_type;

/* WCDMA */
typedef struct
{
  uint16           channels[MAX_NUMBER_CA_PATH][RFCOM_MAX_CARRIERS]; /*!< DL Channel on which WCDMA will be tuned */

  rfcom_multi_carrier_hspa_id_type carrier_types[MAX_NUMBER_CA_PATH]; /*!< Specify SC or DC mode */

  /*This tells us whether WCDMA is on Sub 0 or 1*/
  rfm_wcdma_subscription_type sub_type;

} rfm_meas_wcdma_params_type;


/* LTE tx params for meas*/

/*---------------------------------------------------------------*/

/*!>
  @brief
  This module contains structures and enums that define the
  payloads of the different MSGR commands to be received
  and dispatched back by the LTE SUBSYSTEM

  @{
*/

/*-----------------------------------------------------------------------*/
typedef enum
{
  RFM_L2L_TUNE_AWAY_TUNE_BACK_SCRIPT,
  RFM_L2L_TUNE_AWAY_SCRIPT,
  RFM_L2L_ABORT_SCRIPT,
  RFM_L2L_INVALID_TUNE_SCRIPT
} rfm_l2l_setup_and_build_scripts_type;


typedef struct
{
   rfcom_lte_earfcn_type  tx_chan[MAX_NUMBER_CA_PATH];
   /*!<  Channel on which LTE will be tuned, regardless of source or target tech  */

   rfcom_lte_bw_type     tx_bw[MAX_NUMBER_CA_PATH];
   /*!<  Channel bandwidth on which LTE will use, regardless of source or target tech  */

   /*!<  Carrier index mask: which carriers is the request for on source */
} rfm_meas_lte_tx_params_type;


/* LTE meas params*/
typedef struct
{
   rfcom_lte_earfcn_type  rx_chan[MAX_NUMBER_CA_PATH];
   /*!<  Channel on which LTE will be tuned, regardless of source or target tech  */

   rfcom_lte_bw_type     rx_bw[MAX_NUMBER_CA_PATH];
   /*!<  Channel bandwidth on which LTE will use, regardless of source or target tech  */

   boolean               skip_drx_pre_config;
   /*!<  skip pre_config to be performed on the same transceiver */

   uint8 src_chain_mask[MAX_NUMBER_CA_PATH];
   /*!<  Chain mask indicating active Rx chains*/

   uint16 single_chain_bitmask[MAX_NUMBER_CA_PATH];
   /*!<  Rx_SC_Bit_Mask [8 bits] | Tx_SC_Bit_Mask [8 bits] */

   uint32                active_carrier_idx_mask;
   /*!<  Carrier index mask: which carriers is the request for on source */

   rfm_meas_lte_tx_params_type tx_params;
   /*!<  Tx params to enable support for Rx-TX split and ULCA*/

} rfm_meas_lte_params_type;

typedef struct
{
   rfcom_device_enum_type device;
   /*!<  Device on which LTE will be tuned  */

   rfcom_device_enum_type div_device;
   /*!<  Diversity Device on which LTE will be tuned  */

   uint32                band;
   /*!< Specify technology band*/

   rfcom_lte_earfcn_type                rx_channel;
   /*!<  Specify technology DL channel */

   rfcom_lte_bw_type     bw;
   /*!<  Specify technology channel bandwidth,
         populate only for LTE  */

   lm_handle_type        rxlm_buf_idx_ant0;
   /*!<  LM buffer index for prx,
         populate only for target tech */

   lm_handle_type        rxlm_buf_idx_ant1;
   /*!<  LM buffer index for drx,
         populate only for target tech */

   uint16                div_en;
   /*!<  source diversity enable/disable indicator  */
} rfm_meas_lte_rx_chan_param_type;


typedef struct
{
   rfcom_device_enum_type device;
   /*!<  Source Device on which LTE will be tuned  */

   rfcom_device_enum_type div_device;
   /*!<  Source Diversity Device on which LTE will be tuned  */

   rfcom_device_enum_type div2_device;
   /*!<  Source Diversity2 Device on which LTE will be tuned  */

   rfcom_device_enum_type div3_device;
   /*!<  Source Diversity3 Device on which LTE will be tuned  */

   rfcom_mode_enum_type  source_tech;
   /*!<  Specify source technology for IRAT */

   uint32                source_band;
   /*!< Specify source technology band*/

   rfcom_lte_earfcn_type source_rx_channel;
   /*!<  Specify source technology DL channel */

   /*! @brief  source diversity enable/disable indicator  */
   uint16                source_div_en;

   rfcom_lte_bw_type     source_bw;
   /*!<  Specify source technology channel bandwidth  */

   uint32                source_buffer_index;
   /*!<  Buffer index for clean-up scripts.*/

   lm_handle_type        source_rxlm_buf_idx_ant0;
   /*!<  LM buffer index for prx*/

   lm_handle_type        source_rxlm_buf_idx_ant1;
   /*!<  LM buffer index for drx*/

   lm_handle_type        source_rxlm_buf_idx_ant2;
   /*!<  LM buffer index for drx2*/

   lm_handle_type        source_rxlm_buf_idx_ant3;
   /*!<  LM buffer index for drx3*/

   rfcom_device_enum_type target_device;
   /*!<  Target Device on which LTE will be tuned  */

   rfcom_device_enum_type target_div_device;
   /*!<  Target Diversity Device on which LTE will be tuned  */

   rfcom_mode_enum_type  target_tech;
   /*!<  Specify target technology for IRAT */

   uint32                target_band;
   /*!<  Specify target technology band */

   rfcom_lte_earfcn_type target_rx_channel;
   /*!<  Specify source technology DL channel */

   /*! @brief  target diversity enable/disable indicator  */
   uint16                target_div_en;

   rfcom_lte_bw_type     target_bw;
   /*!<  Specify source technology channel bandwidth  */

   uint32                target_buffer_index;
   /*!<  Buffer index for clean-up scripts.*/

   lm_handle_type        target_rxlm_buf_idx_ant0;
   /*!<  LM buffer index for prx*/

   lm_handle_type        target_rxlm_buf_idx_ant1;
   /*!<  LM buffer index for prx*/

   rfm_l2l_setup_and_build_scripts_type tune_script_type;
   /*!<   Type of IRAT script requested*/

   uint32                num_src_scell;
   /*!<   Number of SCELL configured at source tech */

   rfm_meas_lte_rx_chan_param_type src_scell_param[(MAX_NUMBER_CA_PATH-1)];
   /*!<   SCELL configuration parameters */

   uint8 src_chain_mask[MAX_NUMBER_CA_PATH];
   /*!<  Chain mask indicating active Rx chains*/

   uint16 single_chain_bitmask[MAX_NUMBER_CA_PATH];
   /*!<  Rx_SC_Bit_Mask [8 bits] | Tx_SC_Bit_Mask [8 bits] */

   uint32                active_carrier_idx_mask;
   /*!<  Carrier index mask: which carriers is the request for on source.*/

} rfm_l2l_setup_and_build_scripts_param_type;

/*-----------------------------------------------------------------------*/


/* TDSCDMA*/
typedef struct
{
  uint16           channels;
  rfa_tdscdma_rxlm_mode_t irat_rxlm_mode;
} rfm_meas_tdscdma_params_type;

/* Union having channel related paramteres for all tech*/
typedef union
{
  rfm_meas_gsm_params_type gsm_params;
  rfm_meas_1x_params_type onex_params;
  rfm_meas_hdr_params_type hdr_params;
  rfm_meas_wcdma_params_type wcdma_params;
  rfm_meas_lte_params_type lte_params;
  rfm_meas_tdscdma_params_type tdscdma_params;

} rfm_meas_tech_setup_params_type;


typedef struct
{
   /* An array of rx main and div devices and lm_hnadles
    * per CA. All the carrier freqs are the same.
    * Unused array elements device=RFM_INVALID_DEVICE
    * Each element in the array is for a different dev/handle pair
    * Use rfm_meas_rx_dev_handle_type to index each dev/handle pair */
   dev_handle_type  rx_dev_handle[MAX_RX_DEV_HANDLE_ITEM];
   /* An array of tx devices lm_hnadles. One max at present */
   dev_handle_type  tx_dev_handle[MAX_TX_DEV_HANDLE_ITEM];
   uint32           band;
} rfm_meas_param_v2_type;

typedef struct
{
  rfm_device_enum_type device;
  rfm_device_enum_type div_device;
  rfm_device_enum_type div2_device;
  rfm_device_enum_type div3_device;
  boolean div_en;
  uint32 band;
} rfm_meas_param_type;

typedef struct rfm_meas_header_struct
{
   rfcom_mode_enum_type   source_tech;
   rfcom_mode_enum_type   target_tech;
   uint8                  num_source_item;
   uint8                  num_target_item;

   /* Use of new device and handle struct.
    * Each element in the array is for a different carrier
    * Use rfm_meas_carrier_type to index source_param/target param*/
   rfm_meas_param_v2_type   source_param_v2[MAX_NUMBER_CA_PATH];
   rfm_meas_param_v2_type   target_param_v2[MAX_NUMBER_CA_PATH];

   /* To be deprecated */
   rfm_meas_param_type    source_param[MAX_SOURCE_ITEM];
   rfm_meas_param_type    target_param;
} rfm_meas_header_type;

/* Structure that combines fields that is common between Enter Param, Setup Param and Exit Param*/
typedef struct
{
  rfm_meas_header_type header;
  rfm_meas_tech_setup_params_type source_param;
  rfm_meas_tech_setup_params_type target_param;
  rfm_meas_program_script_type script_type;
  /*  Script type for L1 to indicate regular IRAT (tune away/tune back), Half scripts or NULL2X */
  rfm_meas_scenario_type scenario_type;
  /*  Scenario type for L1 to indicate regular IRAT, QTA, QDTA, QCTA */
} rfm_meas_common_param_type;

typedef struct
{
  rfm_meas_common_param_type* setup_params;

  /* To be deprecated, use one in the common structure */
  rfm_meas_header_type header;
  void* source_param;
  void* target_param;
  rfm_meas_program_script_type script_type;
} rfm_meas_setup_param_type;

typedef struct
{
  rfm_meas_common_param_type* enter_params;

  /* To be deprecated, use one in the common structure */
  rfm_meas_header_type header;
  void* source_param;
  void* target_param;
} rfm_meas_enter_param_type;

typedef struct
{
  rfm_meas_common_param_type* exit_params;
  boolean is_measurement_gap_aborted;

  /* To be deprecated, use one in the common structure */
  rfm_meas_header_type header;
  void* source_param;
  void* target_param;
} rfm_meas_exit_param_type;



/*! @brief  This structure will be common to all technology  */
typedef struct
{
   /*! @brief  Serving RAT  */
   rfcom_mode_enum_type  serving_rat;

   /*! @brief  neighbor RAT  */
   rfcom_mode_enum_type  neighbor_rat;

} rfm_meas_cell_infor_type;


/*! @brief  This structure will be used for L<->G and W<->G  */
typedef struct
{

   rfcom_device_enum_type device;
   /*!<  Device on which WCDMA will be tuned  */

   /*! @brief  Serving band  */
   rf_card_band_type     serving_band;

   /*! @brief  Serving channel  */
   uint16                serving_chan;

   /*! @brief  neighbor band  */
   rf_card_band_type     neighbor_band;

   /*! @brief  neighbor channel  */
   uint16                neighbor_chan;

   /*! @brief  startup buffer indicator for L<->G and W<->G */
   mdsp_burst_type       startup_buffer;

   /*! @brief cleanup buffer indicator for L<->G and W<->G */
   mdsp_burst_type       cleanup_buffer;

   /*! @brief  The rest of the element only needed for L2G and W2G.  Not applicable for G2L and G2W. */
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
   /*! @brief  RSSI to set the LNA  */
   dBx16_T               rx_lvl_dbm;

   /*! @brief  Information to be reported back to GL1  */
   rf_gain_T             *rf_gain;

#endif /* FEATURE_GNSS_ONLY_NO_WWAN */
   /*! @brief  Needed for RF driver optimization */
   rf_burst_type         rf_burst;

   /*! @brief power measurement buffer indicator for L->G and W->G */
   mdsp_burst_type       meas_buffer;

   /*! @brief  Serving cell RxLM buffer index  */
   lm_handle_type        serving_rxlm_buf_idx;

   /*! @brief  Neighbor cell RxLM buffer index  */
   lm_handle_type        neighbor_rxlm_buf_idx;

} rfm_meas_params_channel_type;


/*! @brief  This structure will be passed to rfm_meas_enter() and rfm_meas_setup() */
typedef struct
{
   /*! @brief  The number of neighbor cells to be measured--Mainly for L2G and W2G  */
   uint16                   num_neighbor;

   /* Pointer to neighbour cell list */
   rfm_meas_params_channel_type * channel_info;

} rfm_meas_tech_params_type;

typedef struct
{
    rfm_meas_header_type header;
    uint32 startup_time;
    uint32 cleanup_time;

   /*! @brief Specifies the neighbor pre-tune offset time in us units  */
    uint32 nbr_pretune_offset_time;

   /*! @brief Specifies the home pre-tune offset time in us units  */
    uint32 home_pretune_offset_time;

    void* irat_info_param;
    void* tech_specific_info_param;
    uint8 sub_id;
    rfm_meas_program_script_type script_type; /*  Script type for L1 to indicate regular IRAT, Half scripts or NULL2X */
} rfm_meas_irat_info_type;


typedef struct
{
   /*! @brief Specifies the start duration for XtoY in Target Tech Units */
   uint16 start_duration;

   /*! @brief Power measurement setup time for XtoY in Target Tech Units */
   uint16 power_meas_setup_duration;

   /*! @brief Specifies the cleanup duration for XtoY in Serving Tech Units */
   uint16 cleanup_duration;

   /*! @brief Delta between RX STOP trigger and CCS release for XtoY in Target Tech Units*/
   uint16 power_meas_gamma;

   /*! @brief XtoY pll settling time only */
   uint16 pll_settling_time;

   /*! @brief XtoY SBI write time */
   uint16 sbi_write_time;

   /*! @brief XtoY AGC settling time */
   uint16 agc_settling_time;

   /*! @brief Power measurement setup DR-DSDS time for XtoY in Target Tech Units */
   uint16 power_meas_alpha_rffe;

} rfm_meas_xtoy_irat_info_param_type;


typedef struct
{
   /*! @brief  Specifies the start duration for WtoG=num_sbis*2.4Qs  */
   uint16 start_duration;

   /*! @brief  GSM power measurement setup time: num_sbis*2.4+tunetime (QS)  */
   uint16 power_meas_setup_duration;

   /*! @brief  WtoG cleanup duration = num_sbi*2.4+pll settling+agc settling */
   uint16 cleanup_duration;

   /*! @brief  WtoG tune back WCDMA pll settling time only (us) */
   uint16 w_pll_settling_time;

} rfm_meas_wtog_irat_info_param_type;


typedef struct
{
   /*! @brief  Specifies the start duration for LtoG=num_sbis*2.4Qs  */
   uint16 start_duration;

   /*! @brief  GSM power measurement setup time: num_sbis*2.4+tunetime (QS)  */
   uint16 power_meas_setup_duration;

   /*! @brief  LtoG cleanup duration = num_sbi*2.4+pll settling*/
   uint16 cleanup_duration;


} rfm_meas_ltog_irat_info_param_type;


typedef struct
{
   /*! @brief  WCDMA pll settling time only (us) */
   uint16 w_pll_settling_time;

} rfm_meas_gtow_irat_info_param_type;

/*! @brief  This structure will be used for L->W, W->L, DO->L, L->DO, L->1x, L->L, 1x->L  */
typedef struct
{
   /*! @brief  source tech  */
   rfm_mode_enum_type    src_tech;

   /*! @brief  source CA PATH 0 device*/
   rfm_device_enum_type  src_ca_path0_device;

   /*! @brief  source CA PATH 0 band  */
   uint16                src_ca_path0_band;

   /*! @brief  source CA PATH 0 Carrier TYPE  */
   uint16                src_ca_path0_car_type;

   /*! @brief  source channel  Car 0 for CA0*/
   uint32  src_ca_path0_chan_car0;

   /*! @brief  source channel  Car 1 for CA0*/
   uint32  src_ca_path0_chan_car1;

   /*! @brief  source channel  Car 2 for CA0*/
   uint32  src_ca_path0_chan_car2;

   /*! @brief  source bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     src_ca_path0_bw;

   /*! @brief  source diversity enable/disable indicator  */
   uint16                src_ca_path0_div_en;

   /*! @brief  source tech  DIV device CA Path 0 Device id 1*/
   rfm_device_enum_type  src_ca_path0_div_device_id1;

   /*! @brief  source tech  DIV device CA Path 0 Device id 2*/
   rfm_device_enum_type  src_ca_path0_div_device_id2;

    /*! @brief  source tech DIV device CA Path 0 Device id 3*/
   rfm_device_enum_type  src_ca_path0_div_device_id3;

   /*! @brief  source tech  Tx En for  CA Path 0 */
   rfm_device_enum_type  src_ca_path0_tx_en;

   /*! @brief  source tech  Tx device  CA Path 0 */
   rfm_device_enum_type  src_ca_path0_tx_device;

   /*!<  LM buffer index for SRC CA Path 0 Tx*/
   lm_handle_type        src_ca_path0_txlm_buf_idx;

   /*!<  LM buffer index for prx*/
   lm_handle_type        src_ca_path0_prx_rxlm_buf_idx;

   /*!<  LM buffer index for drx DIV 1*/
   lm_handle_type        src_ca_path0_drx_device_1_rxlm_buf_idx;

   /*!<  LM buffer index for drx DIV 1*/
   lm_handle_type        src_ca_path0_drx_device_2_rxlm_buf_idx;

   /*!<  LM buffer index for drx DIV 1*/
   lm_handle_type        src_ca_path0_drx_device_3_rxlm_buf_idx;

   /*! @brief  target tech  */
   rfm_mode_enum_type    tgt_tech;

   /*! @brief  source tech  device*/
   rfm_device_enum_type  tgt_device;

   /*! @brief  target band  */
   uint16                tgt_band;

   /*! @brief  target channel  */
   uint32 tgt_chan;

   /*! @brief  target bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     tgt_bw;

   /*! @brief  target diversity enable/disable indicator  */
   uint16                tgt_div_en;

   /*! @brief  source tech  DIV device*/
   rfm_device_enum_type  tgt_div_device;

   /*!<  LM buffer index for TGT prx*/
   lm_handle_type        tgt_prx_rxlm_buf_idx;

   /*!<  LM buffer index for TGT drx*/
   lm_handle_type        tgt_drx_rxlm_buf_idx;

   /*! @brief  test level  */
   uint16                test_level;

   /*! @brief falg to check if source tech has CA Path 1 enabled */
   uint16                src_ca_path1_flag_enable;

   /*! @brief  source CA Path 1 device*/
   rfm_device_enum_type  src_ca_path1_device;

   /*! @brief  source CA Path 1 band  */
   uint16                src_ca_path1_band;

   /*! @brief  source CA Path 1 Carrier type  */
   uint16                src_ca_path1_car_type;

   /*! @brief  source channel  Car 0 for CA1*/
   uint32  src_ca_path1_chan_car0;

   /*! @brief  source channel  Car 1 for CA1*/
   uint32  src_ca_path1_chan_car1;

   /*! @brief  source channel  Car 2 for CA1*/
   uint32  src_ca_path1_chan_car2;

   /*! @brief  source bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     src_ca_path1_bw;

   /*! @brief  source diversity enable/disable indicator  */
   uint16                src_ca_path1_div_en;

   /*! @brief  source tech  DIV device CA Path 1 Device id 1*/
   rfm_device_enum_type  src_ca_path1_div_device_id1;

   /*! @brief  source tech  DIV device CA Path 1 Device id 2*/
   rfm_device_enum_type  src_ca_path1_div_device_id2;

    /*! @brief  source tech DIV device CA Path 1 Device id 3*/
   rfm_device_enum_type  src_ca_path1_div_device_id3;

   /*! @brief  source tech  Tx En for  CA Path 1 */
   rfm_device_enum_type  src_ca_path1_tx_en;

   /*! @brief  source tech  Tx device  CA Path 1 */
   rfm_device_enum_type  src_ca_path1_tx_device;

   /*!<  LM buffer index for SRC CA Path 1 Tx*/
   lm_handle_type        src_ca_path1_txlm_buf_idx;

   /*!<  LM buffer index for SRC CA path 1 prx*/
   lm_handle_type        src_ca_path1_prx_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA path 1 drx Div 1*/
   lm_handle_type        src_ca_path1_drx_device_1_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA path 1 drx Div 2*/
   lm_handle_type        src_ca_path1_drx_device_2_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA path 1 drx Div 3*/
   lm_handle_type        src_ca_path1_drx_device_3_rxlm_buf_idx;

   /*! @brief falg to check if source tech has CA Path 2 enabled */
   uint16                src_ca_path2_flag_enable;

   /*! @brief  source CA Path 2 device*/
   rfm_device_enum_type  src_ca_path2_device;

   /*! @brief  source CA Path 2 band  */
   uint16                src_ca_path2_band;

   /*! @brief  source CA Path 2 Carrier type  */
   uint16                src_ca_path2_car_type;

   /*! @brief  source channel  Car 0 for CA2*/
   uint32  src_ca_path2_chan_car0;

   /*! @brief  source channel  Car 1 for CA2*/
   uint32  src_ca_path2_chan_car1;

   /*! @brief  source channel  Car 2 for CA2*/
   uint32  src_ca_path2_chan_car2;

   /*! @brief  source bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     src_ca_path2_bw;

   /*! @brief  source diversity enable/disable indicator  */
   uint16                src_ca_path2_div_en;

   /*! @brief  source tech  DIV device CA Path 2 Device id 1*/
   rfm_device_enum_type  src_ca_path2_div_device_id1;

   /*! @brief  source tech  DIV device CA Path 2 Device id 2*/
   rfm_device_enum_type  src_ca_path2_div_device_id2;

    /*! @brief  source tech DIV device CA Path 2 Device id 3*/
   rfm_device_enum_type  src_ca_path2_div_device_id3;

    /*! @brief  source tech Tx En for  CA Path 2 */
   rfm_device_enum_type  src_ca_path2_tx_en;

   /*! @brief  source tech  Tx device  CA Path 2 */
   rfm_device_enum_type  src_ca_path2_tx_device;

   /*!<  LM buffer index for SRC CA Path 2 Tx*/
   lm_handle_type        src_ca_path2_txlm_buf_idx;

   /*!<  LM buffer index for SRC CA Path 2 prx*/
   lm_handle_type        src_ca_path2_prx_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA Path 2 drx Div 1*/
   lm_handle_type        src_ca_path2_drx_device_1_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA Path 2 drx Div 2*/
   lm_handle_type        src_ca_path2_drx_device_2_rxlm_buf_idx;

   /*!<  LM buffer index for SRC CA Path 2 drx Div 3*/
   lm_handle_type        src_ca_path2_drx_device_3_rxlm_buf_idx;


} rfm_meas_generic_params_type;

/*! @brief  This structure will be used for half scritps*/
typedef struct
{
   /*! @brief  source tech  */
   rfm_mode_enum_type    src_tech;

   /*! @brief  source tech  device*/
   rfm_device_enum_type  src_device;

   /*! @brief  source band  */
   uint16                src_band;

   /*! @brief  source channel  */
   uint32 src_chan;

   /*! @brief  source bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     src_bw;

   /*! @brief  source diversity enable/disable indicator  */
   uint16                src_div_en;

   /*! @brief  source tech  DIV device*/
   rfm_device_enum_type  src_div_device;

   /*!<  LM buffer index for src prx*/
   lm_handle_type        src_prx_rxlm_buf_idx;

   /*!<  LM buffer index for src drx*/
   lm_handle_type        src_drx_rxlm_buf_idx;

   /*! @brief  target tech  */
   rfm_mode_enum_type    tgt_tech;

   /*! @brief  source tech  device*/
   rfm_device_enum_type  tgt_device;

   /*! @brief  target band  */
   uint16                tgt_band;

   /*! @brief  target channel  */
   uint32 tgt_chan;

   /*! @brief  target bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     tgt_bw;

   /*! @brief  target diversity enable/disable indicator  */
   uint16                tgt_div_en;

   /*! @brief  source tech  DIV device*/
   rfm_device_enum_type  tgt_div_device;

   /*!<  LM buffer index for TGT prx*/
   lm_handle_type        tgt_prx_rxlm_buf_idx;

   /*!<  LM buffer index for TGT drx*/
   lm_handle_type        tgt_drx_rxlm_buf_idx;

   /*! @brief  test level  */
   uint16                test_level;
} rfm_meas_half_scripts_params_type;


/*! @brief  This structure will be used for null2x scritps*/
typedef struct
{
   /*! @brief  target tech  */
   rfm_mode_enum_type    tgt_tech;

   /*! @brief  source tech  device*/
   rfm_device_enum_type  tgt_device;

   /*! @brief  target band  */
   uint16                tgt_band;

   /*! @brief  target channel  */
   uint32 tgt_chan;

   /*! @brief  target bandwidth  */
   /*! For technologies except LTE, _bw will be set by default as RFCOM_BW_LTE_1P4MHz*/
   rfcom_lte_bw_type     tgt_bw;

   /*! @brief  target diversity enable/disable indicator  */
   uint16                tgt_div_en;

   /*! @brief  source tech  DIV device*/
   rfm_device_enum_type  tgt_div_device;

   /*!<  LM buffer index for TGT prx*/
   lm_handle_type        tgt_prx_rxlm_buf_idx;

   /*!<  LM buffer index for TGT drx*/
   lm_handle_type        tgt_drx_rxlm_buf_idx;

   /*! @brief  test level  */
   uint16                test_level;

} rfm_meas_null2x_params_type;


typedef enum
{
 RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_X=0,
 RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_Y,

 RFM_HALF_SCRIPT_NBR_X_2_NBR_Y,
 RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ,

 RFM_HALF_SCRIPT_NBR_Y_2_NBR_X,
 RFM_HALF_SCRIPT_NBR_Y_2_HOMEFREQ
}rfm_meas_half_script_type;


typedef struct
{
  uint16 pll_settling_time;
  uint16 agc_settling_time;  /* AGC settling Time in usec */
  uint16 ssbi_write_time;    /* ssbi_write_time in usec */
} rfm_meas_wtow_irat_info_param_type;

typedef struct
{
  uint16 pll_settling_time;
  uint16 agc_settling_time;  /* AGC settling Time in usec */
  uint16 ssbi_write_time;    /* ssbi_write_time in usec */
} rfm_meas_ltow_irat_info_param_type;

typedef struct
{
   /*! @brief  Specifies the start duration for TtoG=num_sbis*2.4Qs  */
   uint16 start_duration;
   /*! @brief  GSM power measurement setup time: num_sbis*2.4+tunetime (QS)  */
   uint16 power_meas_setup_duration;
   /*! @brief  TtoG cleanup duration = num_sbi*2.4+pll settling*/
   uint16 cleanup_duration;
} rfm_meas_ttog_irat_info_param_type;

typedef struct
{

     rfcom_device_enum_type device;
     /*!<  Device on which WCDMA will be tuned  */

     uint16 home_arfcn[RFCOM_MAX_CARRIERS];
     /*!<  DL Channel on which WCDMA will be tuned  */

     uint16 nbr_arfcn;
     /*!<  Neighbor Channel on which WCDMA will be tuned  */

     rfcom_multi_carrier_hspa_id_type carrier_type;
     /*!<  Specify SC or DC mode  */

     uint16 home_buffer_index;
     /*!<  Home RFLM handle for startup/cleanup scripts.*/

     uint16 nbr_buffer_index;
     /*!<  Neighbor RFLM handle for startup/cleanup scripts.*/


} rfm_meas_w2w_setup_and_build_scripts_param_type;

/*Name of APIs which we want to log in rfm_meas.c file*/
/*If we want to log new API, we need to add it to this enum*/
typedef enum{
 RFM_MEAS_COMMON_GET_IRAT_INFO_PARAM,
 RFM_MEAS_COMMON_SCRIPT_ENTER,
 RFM_MEAS_COMMON_SCRIPT_BUILD_SCRIPT,
 RFM_MEAS_COMMON_SCRIPT_EXIT,
 RFM_MEAS_COMMON_SCRIPT_MAX
}rfm_meas_common_api_type;


/*Union of all the possible input data structures to the APIs*/
/*We will select the appropriate structure according to the API*/
/*which is logging*/
typedef union{
 rfm_meas_irat_info_type   rfm_meas_log_irat_info;
 rfm_meas_enter_param_type rfm_meas_log_enter_param;
 rfm_meas_setup_param_type rfm_meas_log_setup_param;
 rfm_meas_exit_param_type  rfm_meas_log_exit_param;
}rfm_meas_param_log_type;


typedef struct{
 /*Name of the API which we are logging*/
 rfm_meas_common_api_type api_type;
 /*Parameters of the corresponding API*/
 rfm_meas_param_log_type rfm_meas_param_log;
 /*Call back handler of the corresponding API*/
 rfm_cb_handler_type cb_handler;
}rfm_meas_log_type;

/*The number of API calls we want to log*/
#define RFM_MEAS_LOG_BUFFER_SIZE 15

typedef struct{
 /*logging index*/
 uint8 buffer_index;
 /*Circular array storing log information*/
 rfm_meas_log_type rfm_meas_log[RFM_MEAS_LOG_BUFFER_SIZE];
 /*Parity bits to check memory corruption issues. The value of below*/
 /*variable should be zero, unless some memory corruption happens*/
 uint16 parity_bits;
}rfm_meas_log_buffer_type;

typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfm_l2l_setup_and_build_scripts_param_type l2l_build_script_request_params;
  rfm_meas_setup_param_type * l2l_build_script_params;
} rfa_rf_lte_l2l_build_scripts_req_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  /*echo back the input*/
  rfm_l2l_setup_and_build_scripts_param_type l2l_build_script_request_params;
  // result of the request
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_l2l_build_scripts_cnf_s;

#endif /* RFMEAS_TYPES_H */
