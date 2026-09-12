/*!
  @file
  rfwcdma_core_rxctl.c

  @brief
  Provides WCDMA Rx control functionality

  @details

*/

/*===========================================================================

Copyright (c) 2008, 2009, 2010, 2011,2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2013 - 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:08:47 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_rxctl.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/19/16   ag      Additional DC Loop BW gain changes
09/09/16   ag      DC Loop BW gain changes
08/29/16   ag      Fix KW warning
06/13/16   vs      Load HS switchpoints for multi carrier mode
05/04/16   sub     Separate DRx events into WTR LNA and eLNA writes; basically divide
                   DRx events into RFFE and GRFC events for proper execution of writes in CCS.
01/17/15   ak      Per LNA state RSB correction not working for SUB2
01/28/16   sd      Feature differentiation for WTRs TA2.3
12/23/15   ak      Changes to apply the LNA state correctly for DC HS case
11/26/15   ndb     Pass the frequency to tuner API's
11/16/15   dw      Change cm_init to int
11/10/15   asn     Fix error in DBDC notching
10/30/15   rmb     Use appropriate LNA swtich points (R99 or HS) depending on number of carriers.
09/29/15   sd      FR30081 AsDiv with eLNA gain compensation 
09/24/15   sd      FR30081 add support for AsDiv alt gain param in device APIs
09/28/15   vbh     Use nv container id for Rx V2 NVs
09/14/15   asn     Ensure dynamic notching happens only in DBDC
08/24/15   asn     Added API to enable asm grfc
08/07/15   ak       Per LNA state Rx RSB correction
07/29/15   asn     Clean up new RxAGC read API
07/27/15   rmb     Add antenna index in RxAGC enable parameters
                   and wrap it around a structure.
07/10/15   ag      Support for GRFC elna
06/18/15   vbh     Call the util routine to map carrier mask to carr_id for ant tuner 
06/10/15   rmb     Pass device type during Measurement scenarios in config band and chan
                   and use this information to get the Antenna Index for MDSP.
06/04/15   rmb     Remove unused routine.
06/08/15   vbh     Get band support on specified device
05/23/15   kr      Fix compilation error
05/14/15   ac      rxagc changes for w plus w04/30/15   ac      tq double buffer related changes
04/30/15   ac      tq double buffer related changes
05/12/15   vbh     Antenna tuner interface updates
04/24/15   vbh     [Rx/Tx split] Add device type parameter for rfwcdma_core_rxctl_config_band
04/14/15   ak      [Rx-Tx Split]Changes to Support Cal with the new Split Design
4/14/15    vbh     [Rx/TX split] Migration to new WCDMA data module
04/10/15   vbh     Provide additional parameters to driver interface apis
4/8/15     vbh     [Rx/Tx split]Increase the cmd action, rxlm_idx buffer size for max wcdma devices
04/07/15   ak      Migrating to new WCDMA state machine
04/07/15   ak      Rx/Tx Split Changes
03/20/15   rmb     Add Rx specific GRFC script based API and also mode config GRFC API.
03/03/14   rmb     Add Alternate path info to Config band and chan to choose correct NV container.
01/21/15   kai     Added support for Rx RSB calibration
12/12/14   rmb     Add Meas config and update the same in DM and also get
                   PLL settling time from WTR instead of from RFC.
11/17/14   rmb     Added Dynamic spur mitigation feature related changes.
10/28/14   vs      Get the static DVGA offset from RxLM SS for JO Modem 
10/14/14   kai     Fix overflow in Rx CGAGC params calculation
10/13/14   vbh     Add NV container support
09/26/14   kai     Added support for WCDMA Rx V2 NVs
09/19/14   rmb     Pass correct band info for ASM disable.
10/09/14   dw      Fix LNA frequency offset 2/3 not applied correctly
09/02/14   dw      Disable TxAGC during Tx bw change
08/21/14   rmb     Add alternate support in the WCDMA MSM layer.
08/11/14   rmb     Add alternate path support for W IRAT scenario. 
08/08/14   zhh     RF warmup profiling
07/23/14   dw      Added LNA dependent RSB programming support
08/04/14   rmb     Change in name in the usage of APIs.
06/23/14   zhh     implement ASM sleep during retune or any ifreq/irat 
06/06/14   dw      Added rfwcdma_core_rxctl_update_alt_path
06/05/14   dw      Added rfwcdma_core_rxctl_disable_sleep_asm()
06/06/14   ac      nbr support
04/18/14   rmb     Add IRAT meas enum info to config_band and config_chan APIs.
04/09/14   dw      Add Rx Cal mode flag in AGC command for NBEE
04/07/14   ac      jammer detect support
04/03/14   dw      Add dynamic RxLM update support
03/25/14   rmb     Change send RxAGC command API to use a new common API. 
03/24/14   rmb     Move the btf delay paramter udpate from Tx config to Rx config band.
03/13/14   rmb     Pass the IRAT flag to config chan and band API.
03/11/13   dw      Add Bolt DCHSUPA support
03/03/14   dw      Resetting LNA state in DRx disable (RF ON)
02/27/14   rmb     Add fix to tune to channel for multi-carrier scenario.
02/21/14   rmb     Add global NV pointers for each of band specific static NVs.
02/13/14   dw      [1]Change AGC command function to support multiple device
                   [2]Code cleanup
02/14/14    rmb    Computation for CGAGC Default LNA State change from G2 to G1.
02/05/14   dw      Increase generic script size, clear buffer after creation
01/31/14   rmb     Added new FED API to get RX information.
01/02/14   dw      [1]Bolt cleanup on legacy mdsp function call 
                   [2]Added warm time, PLL settling time and RF transaction time to DM interface
12/12/13   rmb     Fix to pick up NVs for the band asked.
10/28/13   dw      Add LNA state logging support
10/07/13   sbo     Fixed bug in rxctl_update_freq_offsets api to compare with freq.
9/25/13    vbh     Warning fixes 
09/24/13   dw      Added default lna_gain_state_oride setting
09/20/13   dw      Fix FGAGC LNA switchpoint query from new NV 
09/18/13   dw      Fix DRx script query
09/16/13   dw      Added support for FTM Cal mode and DRx scripts
09/10/13   dw      [1]Remove DBDC featurization and code clean up.
                   [2]Optimiza the code based on review comments
09/09/13   dw      Core changes for new Bolt MDSP module - new API to config Rx band/chan
08/30/13   vs      Modifying MDSP interface to make common across DI/BO/TR
08/29/13   sbo     Added support for 3 carriers in cgagc table
08/28/13   vs      Removal of featurization and moving to new NV structure
08/23/13   vs      Cleanup of dual carrier APIs
08/16/13   ndb     Clean-up the SPI based antenna tuner support
08/08/13   sma     KW warning fix
07/26/13   ac      compiler warning fix on dime 2.0
07/18/13   ac      DBDC bringup related changes
07/17/13   sma     Clear compiler warning and Fix RSB bug for non 1625/20 dev
07/08/13   sma     Modify support for rsb comp with temperature change
05/06/13   tks     Added support for rsb comp with temperature change
04/04/13   ka      WCDMA state machine updates
02/06/13   rmb     Added support for wakeup optimization.
01/30/13   ka      Updated interfaces to logical device
12/11/12   dw      Add support for DTx and DRx/LPM.
10/16/12   ac      rxagc update for rxd
10/06/12   ac      reverting the change, Wl1 submit once Wli dependency is added to correct branch
09/28/12   ac      device interface to pass band to rfc_wcdma_get_device
06/27/12   sbo     Corrected check for primary in rfwcdma_core_rxctl_set_cpc_drx
05/18/12   dw      Increase the AGC command timeout to 10ms. 
05/14/12   kai     Removed FW RXAGC cmd time out error fatal 
05/08/12   tks     Add support for CPC Drx.  
02/14/12   swb     Clear xoslamflag in FTM upon cmd response
11/04/11   gh      Add ant_tuner state check
10/25/11   ac/ka   Added support for FTM dual carrier freq comp feature 
10/21/11    tnt    Pass non-NULL ptr to rfcommon_ant_tuner_get_bins_and_idx()
10/17/11    gh     Add support for ant_tuner
10/13/11   swb     Added API for freezing Rx AGC accumulator
09/26/11   tks     Fixed KW warning.   
08/24/11    dw     Support polling based AGC cmd response handling
08/21/11    ac     BC19 related changes
06/29/11    dw     Rail cmLnaRise_3 to fix CGAGC hanging in high LNA state
06/23/11    dw     Add rfwcdma_core_rxctl_enable_dual_carrier
06/13/11    dw     Get zero db digital gain from MSM HAL
05/23/11    dw     Remove rfwcdmalib_msm.h 
02/22/11    dw     Include RxLM buffer index in AGC cmd
02/09/11    dw     Add support for WCDMA AGC cmd control
01/05/10    dw     Initial support for RxLM
09/10/10    dw     Add missing Chain1 CGAGC mdsp parameters for DC
08/26/10    av     Rail LNA rise 2. Fixes Ec/Io fluctuations. 
06/28/10    ac     lint fix
06/22/10    ka     KW warning fix
06/09/10    ac     compiler error fix
06/07/10    ac     freq comp for DC mode
05/20/10    tws    Add flag so CGAGC params are only initialised once.
05/12/10    ac     B11 bringup
3/17/10     kguo   Remove feature RF_HAS_WCDMA_RX_DUAL_CARRIER 
3/16/10     kguo   Merged from scmm_9k_final_merge branch to main\latest 
03/03/10    wen    Fixed compilation errors 
02/24/10    xw     Klocwork fix 
01/18/10   kguo    Fixed compiler errors for merged build 
01/10/10   kguo    Merged mdm9k dev branch to main\latest
08/14/09    ka     Added include file required for MDM9x00
08/13/09   jhe     freq comp change
07/29/09   ckl     Fixed type casting error of double data type
07/29/09   ckl     Removed compiler warnings
07/07/09   ckl     Changed cgagc parameters calculation
07/03/09   ckl     Delete mdsp_start_rx_agc function call in enable_rx_agc
03/19/09   ckl     Arrange order of header files
03/17/09   ckl     Added RF_HAS_LINEAR_INTERPOLATION compile option
03/15/09   ckl     Deleted rfnv.h
03/09/09   ckl     Code cleanup
11/06/08   av      Initial version.

============================================================================*/
#include "rfa_variation.h"
#include <math.h>
#include "comdef.h"
#include "rfcom.h"
#include "msg.h"
#include "rfumts.h"
#include "rfnv_wcdma.h"
#include "rfwcdma_mdsp.h"
#include "rfwcdma_msm.h"
#include "rfwcdma_core_rxctl.h"
#include "rfwcdma_core.h"
#include "rfwcdma_mc.h"
#include "wfw_sw_cmd_proc_intf.h"
#include "DALSys.h"
#include "rfnv_wcdma.h"
#include "rfdevice_wcdma_intf.h"
#include "rfcommon_msg.h"
#include "rfwcdma_core_temp_comp.h"
#include "rfwcdma_core_util.h"
#include "rfcommon_core.h"

#include "rfwcdma_mdsp_async.h"
#include "rf_hal_buffer.h"
#include "rfc_card_wcdma.h"
#include "rfwcdma_mdsp_sync.h"
#include "ftm_wcdma_ctl.h"
#include "ftm_wcdma_calv3.h"
#include "rfdevice_wcdma_asm_intf.h"
#include "rfdevice_wcdma_lna_intf.h"
#include "rfcommon_core_utils.h"
#include "rfwcdma_core_antenna_tuner.h"
#include "rfwcdma_data.h"

#include "rfc_wcdma_data.h"
#include "rfcommon_msm.h"
#include "rfwcdma_data.h"

#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_manager.h"
#endif

/*---------------------------------------------------------
                   Constant Definitions
---------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines Polling interval after AGC command in us 
*/
#define RFWCDMA_CORE_AGC_POLLING_INTERVAL 100

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines Polling timeout after AGC command in ust 
*/
#define RFWCDMA_CORE_AGC_POLLING_TIMEOUT 10000

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic SSBI script size(max) for a Rx event 
*/
#define RF_WDMA_RX_GEN_SSBI_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic RFFE script size(max) for a Rx event
 
*/
#define RF_WDMA_RX_GEN_RFFE_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic GRFC script size(max) for a Rx event
*/
#define RF_WDMA_RX_GEN_GRFC_SCRIPT_SIZE                                       10

/* LNA offset frequency compensation value */
int8 rfwcdma_core_rxctl_lna_offset_vs_freq_val[RFM_MAX_WAN_DEVICES][RF_WDMA_MAX_NUM_LNA_STATES] = { 0 };

/* DVGA gain offset frequency compensation value */
int8 rfwcdma_core_rxctl_vga_gain_offset_vs_freq_val[RFM_MAX_WAN_DEVICES] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/* DVGA gain offset temperature compensation value */
int16 rfwcdma_core_rxctl_vga_gain_offset_vs_temp_val = 0;

/* Rx Secondary chain DVGA gain offset temperature compensation value */
int16 rfwcdma_core_rxctl_c1_vga_gain_offset_vs_temp_val = 0;

/*! Dynamic notch callback data for a RFM device 
  Updated in callback from WTR device in MC Tune and Sleep */
static rfdevice_wcdma_spur_param_type rfwcdma_core_rxctl_dyn_notch_settings_cb[RFM_MAX_WAN_DEVICES]={0};

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Rx Frequency compensation variables
*/
typedef struct
{
  int8 i_vga_gain_offset_vs_freq;
  int8 i_lna_offset_vs_freq;
  int8 i_lna_2_offset_vs_freq;
  int8 i_lna_3_offset_vs_freq;
  int8 i_lna_4_offset_vs_freq;
} rfwcdma_core_rxctl_data_type;

extern const rf_hal_bus_resource_script_type* rfwcdma_core_get_resource_script_from_buffer
(
  rf_buffer_intf *buffer_ptr
);
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function enables Rx AGC operation.

  @detail
  This function manages the FW RxAGC cmd cfg parameters, and send command to FW to enable/disable
  RxAGC on the given chain/carriers with the provided RxLM buffer.
  It supports enable/disable RxAGC on multiple chains and carriers, but does not support multiple
  carrier on different devices (DBDC), i.e. following are supported.
 
  1. AGC cmd on Single band multi-carrier
  2. AGC cmd on PRx chain and DRx chain on the same CA path
 
  Following are not supported:
 
  AGC cmd of multi-carrier on different CA path.
 
  For AGC cmds on multiple chains/carrier, cmd action has to be same. Cmd actions on other device
  has to be set to invalid.

  @param device rfm_device 
  @param multi_carrier_idx: bit mask for carrier indices that maps to the AGC indices
  which needs to be enabled/disabled
  @param cmd Select AGC operation command: enable, disable, or noop
  @param rxlm buffer index
  @param Antenna index that RxAGC is to run on (either disable/enable) per RFM device
  @param rf_warmup_time RF warmup time used in RF wake up 
  @param rf_wakeup enable or disable rf wakeup optimization
 
  @return Return Api status
  boolean api_status
 */
boolean rfwcdma_core_rxctl_enable_rxagc
(
  rfwcdma_core_rxctl_enable_rxagc_params* rxagc_enable_params
  )
{
  /* Carrier ID*/
  uint8 agc_car_id = 0;
  /* RFM device */
  rfm_device_enum_type device = RFM_DEVICE_0;
  /* Antenna ID */
  rfm_wcdma_antenna_index ant_id = RFM_WCDMA_INVALID_ANTENNA;
  boolean api_status = FALSE;
  rfwcdma_mdsp_agc_action_type command_action = RFWCDMA_MDSP_AGC_CMD_NOOP;

  /* Initialize all agc command parameters to 0 */
  rfwcdma_mdsp_agc_cmd_param_type agc_cmd_param;
  memset(&agc_cmd_param, 0, sizeof(rfwcdma_mdsp_agc_cmd_param_type));

  if (rxagc_enable_params == NULL) 
  {
    RF_MSG(RF_FATAL, "rfwcdma_core_rxctl_enable_rxagc: RxAGC enable params is NULL");
    return api_status;
  }

  RF_MSG_6(RF_HIGH, "cmd action: %d, %d, %d, %d, %d, %d", 
           rxagc_enable_params->cmd_action[0],
           rxagc_enable_params->cmd_action[1],
           rxagc_enable_params->cmd_action[2],
           rxagc_enable_params->cmd_action[3],
           rxagc_enable_params->cmd_action[4],
           rxagc_enable_params->cmd_action[5]);

  /* Populate mdsp AGC cmd parameter structure*/
  for (device = RFM_DEVICE_0; device < RFM_WCDMA_MAX_DEVICE; device++)
  {
    command_action = rxagc_enable_params->cmd_action[device];

    if (command_action != RFWCDMA_MDSP_AGC_CMD_INVALID)
    {
      /* Get Antenna idx from the rxagc input params */
      ant_id = rxagc_enable_params->ant_idx[device];

      RF_MSG_3(RF_HIGH, "ant_id:%d, device:%d, cmd action: %d", ant_id, device, command_action);

      if (ant_id >= RFM_WCDMA_MAX_ANTENNA )
      {
        RF_MSG_2(RF_ERROR, "rfwcdma_core_rxctl_enable_rxagc: Invalid antenna=%d for device=%d", ant_id, device);
        continue;
      }

      /* populate agc cmd parameters. 
        command action and rxlm buffer index will be the same 
        for all AGCs in the config */
      for (agc_car_id = 0; agc_car_id < RF_WCDMA_MAX_NUM_RX_CARRIERS ; agc_car_id++)
      {
        if ((rxagc_enable_params->multi_carrier_idx) & (1 << agc_car_id))
        {
          agc_cmd_param.agc_action[ant_id][agc_car_id] = command_action;
          agc_cmd_param.rxlm_buf_idx[ant_id][agc_car_id] = rxagc_enable_params->rxlm_buf_idx[device];
        }
      }
    }/* (cmd_action[device]!=RFWCDMA_MDSP_AGC_CMD_INVALID) */
  }/* for (device = RFM_DEVICE_0; device < RFM_DEVICE_4; device++) */

  /* Populate warmup flag */
  agc_cmd_param.rf_warmup_en = rxagc_enable_params->rf_warmup_en;

  /* Populate nbr flag */
  agc_cmd_param.nbr_en_flag = (uint8)rxagc_enable_params->nbr_en_flag;
  agc_cmd_param.nbr_rxlm_buf_idx = rxagc_enable_params->nbr_rxlm_buf_idx;

  /* Populate Cal mode flag */
  if (ftm_wcdma_rxcal_opt_enabled() && IS_FTM_IN_WCAL_MODE())
  {
    agc_cmd_param.cal_mode = TRUE;
  }
  else
  {
    agc_cmd_param.cal_mode = FALSE;
  }
  agc_cmd_param.dc_loop_gain_burst_mode = FALSE;

  /* Send command to FW */
  (void)rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_RXAGC_CMD_ID, &agc_cmd_param );

  api_status = TRUE;
  return api_status;

}/* rfwcdma_core_rxctl_enable_rxagc */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function sends a command to FW to enable DC loop for some BPGs.

  @detail
  This function sends a command to FW to enable DC loop for some BPGs.  
 */
boolean rfwcdma_core_rxctl_enable_dc_gain
(
rfwcdma_core_rxctl_enable_rxagc_params* rxagc_enable_params
)
{

  /* Carrier ID*/
   uint8 agc_car_id = 0;
  /* RFM device */
   rfm_device_enum_type device = RFM_DEVICE_0;
  /* Antenna ID */
   rfm_wcdma_antenna_index ant_id = RFM_WCDMA_INVALID_ANTENNA;
   boolean api_status = FALSE;
   rfwcdma_mdsp_agc_action_type command_action = RFWCDMA_MDSP_AGC_CMD_NOOP;
 
  /* Initialize all agc command parameters to 0 */
  rfwcdma_mdsp_agc_cmd_param_type agc_cmd_param;
  memset(&agc_cmd_param, 0, sizeof(rfwcdma_mdsp_agc_cmd_param_type));

   if (rxagc_enable_params == NULL) 
   {
     RF_MSG(RF_FATAL, "rfwcdma_core_rxctl_enable_rxagc: RxAGC enable params is NULL");
     return api_status;
   }

    RF_MSG_6(RF_HIGH, "cmd action: %d, %d, %d, %d, %d, %d", 
             rxagc_enable_params->cmd_action[0],
             rxagc_enable_params->cmd_action[1],
             rxagc_enable_params->cmd_action[2],
             rxagc_enable_params->cmd_action[3],
             rxagc_enable_params->cmd_action[4],
             rxagc_enable_params->cmd_action[5]);


  /* Populate mdsp AGC cmd parameter structure*/
   for (device = RFM_DEVICE_0; device < RFM_WCDMA_MAX_DEVICE; device++)
   {
     command_action = rxagc_enable_params->cmd_action[device];
  
     if (command_action != RFWCDMA_MDSP_AGC_CMD_INVALID)
     {
       /* Get Antenna idx from the rxagc input params */
 	ant_id = rxagc_enable_params->ant_idx[device];
  
	RF_MSG_3(RF_HIGH, "ant_id:%d, device:%d, cmd action: %d", ant_id, device, command_action);
  
	if (ant_id >= RFM_WCDMA_MAX_ANTENNA )
	{
	  RF_MSG_2(RF_ERROR, "rfwcdma_core_rxctl_enable_rxagc: Invalid antenna=%d for device=%d", ant_id, device);
	  continue;
	}
  
	/* populate agc cmd parameters. 
	  command action and rxlm buffer index will be the same 
	  for all AGCs in the config */
         for (agc_car_id = 0; agc_car_id < RF_WCDMA_MAX_NUM_RX_CARRIERS ; agc_car_id++)
         {
	   if ((rxagc_enable_params->multi_carrier_idx) & (1 << agc_car_id))
	   {
             agc_cmd_param.agc_action[ant_id][agc_car_id] = command_action;
             agc_cmd_param.rxlm_buf_idx[ant_id][agc_car_id] = rxagc_enable_params->rxlm_buf_idx[device];
	   }
          }
	 }/* (cmd_action[device]!=RFWCDMA_MDSP_AGC_CMD_INVALID) */
	}/* for (device = RFM_DEVICE_0; device < RFM_DEVICE_4; device++) */


  agc_cmd_param.dc_loop_gain_burst_mode = TRUE;

  /* Send command to FW */
  (void)rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_RXAGC_CMD_ID, &agc_cmd_param );
  RF_MSG(RF_MED, "DEBUG_RF: DC loop gain enabled");
 
  api_status = TRUE;
  return api_status;
 
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function calculate CGAGC parameters from NV

  @detail

  @param rfm_device Logical Device in use
  @param rfcom_rf_band Operational WCDMA band
  @param carrier_id Carrier type to indicate Single or Dual Carrier
  @param cgagc_param CGAGC parameter slike DVGA and LNA offsets that need udpated
  @param alt_path Alternate path which is in use
 */
void rfwcdma_core_rxctl_cal_cgagc_params
(
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type band,
  rfcom_multi_carrier_hspa_id_type carr_id, 
  rflm_wcdma_cgagc_param_type *cgagc_param_ptr,
  uint32 alt_path
)
{
  rfnv_wcdma_nv_tbl_type        *nv_tbl_ptr;
  int16 vga_gain_offset, lna_range_offset_1, lna_range_offset_2;
  int16 lna_range_offset_3, lna_range_offset_4, lna_range_offset_5;
  int16 lna_range_rise_1, lna_range_rise_2, lna_range_rise_3, lna_range_rise_4;
  int16 lna_range_fall_1, lna_range_fall_2, lna_range_fall_3, lna_range_fall_4;
  int32 lna_trans1, lna_trans2, lna_trans3, lna_trans4;
  double power_value;
  uint32 zero_db_digital_gain = rfwcdma_msm_get_zero_db_digital_gain();
  uint32 nv_container =0;
  rfc_device_info_type *device_info_ptr;
  rfc_cfg_params_type cfg;

  /*FR30081 Arrays to hold AsDiv offset values*/
  int16 rx_gain_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES] = {0};
  int16 lna_rise_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1]= {0}; 
  int16 lna_fall_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1] = {0};

#ifdef FEATURE_RF_ASDIV  
  rfwcdma_nv_rx_asdiv_elna_gain_offset_type *rx_asdiv_offset_ptr = NULL;
  rfwcdma_nv_rx_asdiv_elna_switchpoint_type *rx_asdiv_switchpoint_ptr =NULL;
  uint32 asdiv_pos;
  uint8 lna_state;
#endif
  nv_tbl_ptr = rfnv_wcdma_get_nv_tbl_ptr(rfnv_wcdma_convert_band_rfcom_to_rfnv(band));
  if (nv_tbl_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "NV table pointer is NULL - Couldn't calculate AGC params from NV !", 0);
    return;
  }

  cfg.alternate_path = alt_path;
  cfg.band = band;
  cfg.logical_device = rfm_device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;


  if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
    RF_MSG_3( RF_HIGH, "rfwcdma_core_rxctl_cal_cgagc_params: For Device:%d, band: %d NV container is: %d",
              rfm_device, band, device_info_ptr->nv_container);
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_core_rxctl_cal_cgagc_params - Cannot get NV container for"
                         "device %d Div config, band %d",rfm_device,band);

  return;
  }
  
  if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
  {
    nv_container = device_info_ptr->nv_container; 
  }

  
#ifdef FEATURE_RF_ASDIV
  asdiv_pos = rfcommon_asdiv_get_current_position(rfm_device);

  /*Load AsDiv offset values*/
  if(asdiv_pos >= ASDIV_POSITION_0 && asdiv_pos < ASDIV_POSITION_MAX)
  {
    rx_asdiv_offset_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_offset_ptr[nv_container][asdiv_pos];
    rx_asdiv_switchpoint_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_switchpoint_ptr[nv_container][asdiv_pos];
  
    if(rx_asdiv_offset_ptr != NULL)
    {	 
      for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
      {
	    rx_gain_asdiv_offset[lna_state] = rx_asdiv_offset_ptr->rx_gain_offset[lna_state];
	  }
    } 		 
    if(rx_asdiv_switchpoint_ptr != NULL)
    {
      for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES-1; lna_state++)
	  {
	    lna_rise_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_rise;
        lna_fall_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_fall;
      }
    }
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_core_rxctl_cal_cgagc_params - Invalid AsDiv position: %d", asdiv_pos);
  }
#endif

  /* Calculate RxAgcVal_CmInit values */
  vga_gain_offset = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                          VGA_OFFSET,0,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[0]; 
  cgagc_param_ptr->cm_init[0] = (0-vga_gain_offset);

  lna_range_offset_1 = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                          LNA_OFFSET,0,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[1];  
  cgagc_param_ptr->cm_init[1] = (lna_range_offset_1 - vga_gain_offset);

  lna_range_offset_2 = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                          LNA_OFFSET,1,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[2];  
  cgagc_param_ptr->cm_init[2] = (lna_range_offset_2 - vga_gain_offset);

  lna_range_offset_3 = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                          LNA_OFFSET,2,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[3];
  
  cgagc_param_ptr->cm_init[3] = (lna_range_offset_3 - vga_gain_offset);

  lna_range_offset_4 = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                          LNA_OFFSET,3,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[4]; 
  cgagc_param_ptr->cm_init[4] = (lna_range_offset_4 - vga_gain_offset);

  lna_range_offset_5 = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type) nv_container,carr_id, 
                                                          LNA_OFFSET,4,&(nv_tbl_ptr->rx_dyn_data)) +
                                                          rx_gain_asdiv_offset[5]; 
  cgagc_param_ptr->cm_init[5] = (uint16)(lna_range_offset_5 - vga_gain_offset);

  /* Calculate Lna transition values */

  lna_range_rise_1 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type)nv_container, carr_id, 
                                                           LNA_RISE,0,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_rise_asdiv_offset[0]; 
  lna_range_rise_2 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_RISE,1,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_rise_asdiv_offset[1]; 
  lna_range_rise_3 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_RISE,2,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_rise_asdiv_offset[2]; 
  lna_range_rise_4 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_RISE,3,&(nv_tbl_ptr->rx_static_data)) + 
                                                           lna_rise_asdiv_offset[3]; 
  lna_range_fall_1 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_FALL,0,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_fall_asdiv_offset[0]; 
  lna_range_fall_2 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_FALL,1,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_fall_asdiv_offset[1]; 
  lna_range_fall_3 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_FALL,2,&(nv_tbl_ptr->rx_static_data)) +
                                                           lna_fall_asdiv_offset[2];
  lna_range_fall_4 = rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type) nv_container, carr_id, 
                                                           LNA_FALL,3,&(nv_tbl_ptr->rx_static_data)) + 
                                                           lna_fall_asdiv_offset[3];

  lna_trans1 = ((int32)lna_range_rise_1 + (int32)lna_range_fall_1) / 2;
  lna_trans2 = ((int32)lna_range_rise_2 + (int32)lna_range_fall_2) / 2;
  lna_trans3 = ((int32)lna_range_rise_3 + (int32)lna_range_fall_3) / 2;
  lna_trans4 = ((int32)lna_range_rise_4 + (int32)lna_range_fall_4) / 2;

  /* Calculate Z value for CmLnaFall_1, CmLnaFall2, and CmLnaRise1 */
  power_value = pow(10.0, ((double)lna_trans1 - (double)cgagc_param_ptr->cm_init[1]) / 100.0);
  cgagc_param_ptr->lna_fall_1 = (uint32)(power_value * zero_db_digital_gain);

  cgagc_param_ptr->lna_fall_2 = 0; //Set LNA Fall 2 to zero as the default LNA state in G1

  power_value = pow(10.0, ((double)lna_trans2 - (double)cgagc_param_ptr->cm_init[1]) / 100.0);
  cgagc_param_ptr->lna_rise_1 = (uint32)(power_value * zero_db_digital_gain);

  power_value = pow(10.0, ((double)lna_trans3 - (double)cgagc_param_ptr->cm_init[1]) / 100.0);
  cgagc_param_ptr->lna_rise_2 = (uint32) ( power_value * zero_db_digital_gain );

  power_value = pow(10.0, ((double)lna_trans4 - (double)cgagc_param_ptr->cm_init[1]) / 100.0);
  cgagc_param_ptr->lna_rise_3 = (uint32) ( power_value * zero_db_digital_gain );

  return;
}/* rfwcdma_core_rxctl_cal_cgagc_params */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function updates frequency offset data.

  @detail
  This function interpolates frequency compensation items from the channel
  number. And, applies the interpolation results to all addected frequency
  compensation items.

  @param rfm_device Logical device that is in use
  @param band Operational WCDMA band
  @param channel Operational channel
  @param alt_path Alternate path which is in use
 */
void rfwcdma_core_rxctl_update_freq_offsets
(
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type band,
  uint16 channel,
  uint32 alt_path
)
{
  uint8 rx_start_bin = 0;   /* starting RX cal channel for bin containing */
  int32 rx_bin_size  = 0;   /* number of channels in the bin (not constant) */
  int32 rx_delta_x   = 0;   /* scale of channel number in bin */
  int16 delta_y      = 0;   /* used to store delta_y between adjacent nv items */
  boolean rx_channel_under = 0; /* flag if lowest RX cal channel > tune value */
  uint32 rx_freq     = 0;
  uint32 bin_rx_freq = 0;
  uint16 bin_rx_chan = 0; 
  boolean out_of_range = FALSE;
  uint32 nv_container =0; 
  
  rfc_device_info_type *device_info_ptr;
  rfc_cfg_params_type cfg;

  /* Offsets Vs Freq are considered same across carriers */
  rfcom_multi_carrier_hspa_id_type carr_id  = RFCOM_SINGLE_CARRIER;

  rfwcdma_core_rxctl_data_type rfwcdma_rxfreq_comp;

  cfg.alternate_path = alt_path;
  cfg.band = band;
  cfg.logical_device = rfm_device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  memset(&rfwcdma_rxfreq_comp, 0, sizeof(rfwcdma_core_rxctl_data_type));

  if (band >= RFCOM_NUM_WCDMA_BANDS)
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_rxctl_update_freq_offsets: Not a valid band %d",
               band);
    return;
  }

    if (rfnv_wcdma_tbl_ptr[band] == NULL)
  {
    RF_MSG_1(RF_ERROR,
             "rfwcdma_core_rxctl_update_freq_offsets: NULL NV tbl ptr for band %d",
             band);
    return;
  }

  /* convert dis-joint cdma channel scheme to continuious.
     The cal channels are NV items and could be initilized
     to continuous,rather than calculating each time. */

  /* PATH 1 or 0 - RX ITEMS */
  rx_freq = rfwcdma_core_get_freq_from_uarfcn(band, channel, RFWCDMA_CORE_UARFCN_DL);

  if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
    RF_MSG_3( RF_HIGH, "rfwcdma_core_rxctl_update_freq_offsets: For Device:%d, band: %d NV container is: %d",
              rfm_device, band, device_info_ptr->nv_container);
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_core_rxctl_update_freq_offsets - Cannot get NV container for"
                         "device %d Div config, band %d",rfm_device,band);

  return;
  }
   if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
  {
    nv_container = device_info_ptr->nv_container; 
  } 

  /* do the same thing, checking against the 1st element of the
     RX cal channel list */
  bin_rx_chan = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                              carr_id,RX_CAL_CHAN_LIST,0,&(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ); 
  bin_rx_freq = rfwcdma_core_get_freq_from_uarfcn(band,bin_rx_chan,RFWCDMA_CORE_UARFCN_DL);

  if (rx_freq <= bin_rx_freq) 
  {
    rx_start_bin = 0;
    rx_channel_under++;    /* set flag if we are under the cal table
                              range flatline the value. */
  }

  /* search for frequency bin that contains the channel
     that is being compensated
     or if the bin contains zero we went off the table, use the last value */

  /* do the same thing, traversing the RX cal channel list */
  else
  {
    out_of_range = FALSE;
    while (rx_start_bin < (NV_FREQ_TABLE_SIZ - 1))
    {
      /* note: tables with less than 16 elements in them must be
         padded with 0's */
      bin_rx_chan = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,carr_id,
                             RX_CAL_CHAN_LIST,rx_start_bin+1,&(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );
      bin_rx_freq = rfwcdma_core_get_freq_from_uarfcn(band,bin_rx_chan,RFWCDMA_CORE_UARFCN_DL);
      if( bin_rx_freq == 0) 
      {
        out_of_range = TRUE;
        break;
      }

      if(rx_freq <= bin_rx_freq) 
      {
        break;            /* exit*/
      }
      else
      {
        rx_start_bin++;      /* check next bin*/
      }
    }
  }

  if (rx_start_bin == (NV_FREQ_TABLE_SIZ - 1) || out_of_range)
  {
    rx_bin_size = 0;
  }
  else
  {
     rx_bin_size = 
        rfwcdma_core_get_freq_from_uarfcn(band,rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
           carr_id,RX_CAL_CHAN_LIST,rx_start_bin+1,&(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ),RFWCDMA_CORE_UARFCN_DL) -
        rfwcdma_core_get_freq_from_uarfcn(band,rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
           carr_id,RX_CAL_CHAN_LIST,rx_start_bin,&(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ),RFWCDMA_CORE_UARFCN_DL);
  }

  /* These are the RX items */

  /* outside the array case */
  if ((rx_bin_size <= 0) || (rx_channel_under == 1))
  {
    /* update rx freq comp items */
    rfwcdma_rxfreq_comp.i_vga_gain_offset_vs_freq =
      (int8)rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container, carr_id,
                                                     VGA_OFFSET_VS_FREQ, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    rfwcdma_rxfreq_comp.i_lna_offset_vs_freq =
      (int8)rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container, carr_id,
                                                     LNA_OFFSET_VS_FREQ_1, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    rfwcdma_rxfreq_comp.i_lna_2_offset_vs_freq =
      (int8)rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container, carr_id,
                                                     LNA_OFFSET_VS_FREQ_2, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    rfwcdma_rxfreq_comp.i_lna_3_offset_vs_freq =
      (int8)rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container, carr_id,
                                                     LNA_OFFSET_VS_FREQ_3, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    rfwcdma_rxfreq_comp.i_lna_4_offset_vs_freq =
      (int8)rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container, carr_id,
                                                     LNA_OFFSET_VS_FREQ_4, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );
  }
  else  /* interpolate the item if it's delta_y is not zero */
  {
    /* ensures that dx/binsize <1 at bin boundary, when delta_y = 1. */
    /* delta_x is the distance from your local channel to the beginning of the
       bin, -1 */
    bin_rx_chan = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                         carr_id,RX_CAL_CHAN_LIST,rx_start_bin,&(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );
    
    bin_rx_freq = rfwcdma_core_get_freq_from_uarfcn(band,bin_rx_chan,RFWCDMA_CORE_UARFCN_DL);
      
    rx_delta_x = (int32)(rx_freq - bin_rx_freq - 1);

    /* DVGA GAIN OFFSET VS FREQ */
    /* delta_y is the difference between the lim_vs_freq values
       of the bin edges to the left and right of our rx frequency */
    /*lint -e{661} Suppress Lint */
    delta_y = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                                       carr_id, VGA_OFFSET_VS_FREQ, rx_start_bin + 1, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) -
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, VGA_OFFSET_VS_FREQ, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    (delta_y < 0) ? delta_y-- : delta_y++;

    /* take the ratio of the delta_x to the entire bin size, and apply
       it to the delta_y.  This is the value you need to add to the
       comp_vs_freq value of your left bin edge to get the comp_vs_freq
       value of your rx frequency */

    rfwcdma_rxfreq_comp.i_vga_gain_offset_vs_freq =
      (int8)(delta_y * rx_delta_x / rx_bin_size) +
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, VGA_OFFSET_VS_FREQ, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    /* LNA OFFSETS VS FREQ */
    /*lint -e{661} Suppress Lint */
    delta_y = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                                       carr_id, LNA_OFFSET_VS_FREQ_1, rx_start_bin + 1, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) -
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_1, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    (delta_y < 0) ? delta_y-- : delta_y++;

    rfwcdma_rxfreq_comp.i_lna_offset_vs_freq =
      (int8)(delta_y * rx_delta_x / rx_bin_size) +
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_1, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

    /*lint -e{661} Suppress Lint */
    delta_y = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                                       carr_id, LNA_OFFSET_VS_FREQ_2, rx_start_bin + 1, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) -
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_2, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );


    (delta_y < 0) ? delta_y-- : delta_y++;

    rfwcdma_rxfreq_comp.i_lna_2_offset_vs_freq =
      (int8)(delta_y * rx_delta_x / rx_bin_size) +
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_2, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );


    /*lint -e{661} Suppress Lint */
    delta_y = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                                       carr_id, LNA_OFFSET_VS_FREQ_3, rx_start_bin + 1, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) -
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_3, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );


    (delta_y < 0) ? delta_y-- : delta_y++;

    rfwcdma_rxfreq_comp.i_lna_3_offset_vs_freq =
      (int8)(delta_y * rx_delta_x / rx_bin_size) +
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_3, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );


    /*lint -e{661} Suppress Lint */
    delta_y = rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                                       carr_id, LNA_OFFSET_VS_FREQ_4, rx_start_bin + 1, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) -
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_4, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );


    (delta_y < 0) ? delta_y-- : delta_y++;

    rfwcdma_rxfreq_comp.i_lna_4_offset_vs_freq =
      (int8)(delta_y * rx_delta_x / rx_bin_size) +
      rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                               carr_id, LNA_OFFSET_VS_FREQ_4, rx_start_bin, &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) );

  }

  /* update lna offset frequency compensation value */

  /* LNA offset for G0 is 0*/
  rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][0] = 0;

  /* update lna offset 1 frequency compensation value */

  rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][1] =
    rfwcdma_rxfreq_comp.i_lna_offset_vs_freq;

  /* update lna offset 2 frequency compensation value */
  rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][2] =
    rfwcdma_rxfreq_comp.i_lna_2_offset_vs_freq;

  /* update lna offset 3 frequency compensation value */
  rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][3] =
    rfwcdma_rxfreq_comp.i_lna_3_offset_vs_freq;

  /* update lna offset 4 frequency compensation value */
   rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][4] =
    rfwcdma_rxfreq_comp.i_lna_4_offset_vs_freq;

  /* update DVGA gain offset frequency compensation value */
  rfwcdma_core_rxctl_vga_gain_offset_vs_freq_val[rfm_device] =
    rfwcdma_rxfreq_comp.i_vga_gain_offset_vs_freq;

  return;
}/* rfwcdma_core_rxctl_update_freq_offsets */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function updates vga gain offset data.

  @detail

 */
void rfwcdma_core_rxctl_temp_val_update
(
  int16 vga_gain_offset_vs_temp_val,
  int16 c1_vga_gain_offset_vs_temp_val
  )
{
  rfwcdma_core_rxctl_vga_gain_offset_vs_temp_val = vga_gain_offset_vs_temp_val;
  rfwcdma_core_rxctl_c1_vga_gain_offset_vs_temp_val = c1_vga_gain_offset_vs_temp_val;
}/* rfwcdma_core_rxctl_temp_val_update */

/*--------------------------------------------------------------------------*/
/*!
  @brief
 
  @details
  This function freezes/unfreezes the RX accumulator.
 
  @param path : transceiver to freeze/unfreeze
  @param freeze : flag to specify freeze/unfreeze
 */

void rfwcdma_core_rxctl_set_rx_freeze_accum(rfcom_device_enum_type device, boolean freeze)
{
  #ifndef FEATURE_WCDMA_DUAL_SYNC_INTERFACE
  rfwcdma_mdsp_set_rx_freeze_accum(device, freeze);
  #endif
}/* rfwcdma_core_rxctl_set_rx_freeze_accum */

/*----------------------------------------------------------------------------*/

/*!
  @brief
  This function will be called during online mode to do the temp comp calculation
  of rsb coefficient

  @details
  This function get the thermistor reading and calculate the rsb coefficient,
  write it to shared memory and send sync command to FW


  @param curr_temp
  This is the current temprature from thermistor
*/

boolean rfwcdma_core_rxctl_update_rsb_coeff(uint8 scaled_therm)
{
  uint8 valid_num_of_carrier                  = 0;
  int32 a_coeff                               = 0;
  int32 b_coeff                               = 0;
  rfm_device_enum_type rfm_device             = RFM_INVALID_DEVICE;
  rfcommon_msm_num_carr_bw bw = {0};
  rfwcdma_mdsp_set_data_param_type mdsp_data = { 0 };
  uint8 temp_lna_state = 0;
  rfcmn_status_type alt_path_status = RFCMN_PATH_SEL_SUCCESS;

  rxlm_chain_type rxlm_chain = RXLM_CHAIN_MAX;
  rfc_device_info_type *device_info_ptr = NULL;
  rfc_cfg_params_type cfg = { RF_PATH_MAX, RFM_INVALID_DEVICE, RFC_CONFIG_RXTX_INVALID, 0, RFC_REQ_DEFAULT_GET_DATA };
  uint8 alt_path = 0;

  rfdevice_wcdma_rx_rsb_cal_data_type nv_rsb_data = {0};
  rfdevice_wcdma_rx_rsb_cal_data_type *nv_rsb_data_ptr = NULL;
  rfwcdma_nv_rx_rsb_type *rsb_cal_data = NULL;
  uint32 nv_container_idx = 0;

  rfnv_wcdma_nv_tbl_type *nv_tbl_ptr = NULL;

  boolean api_status = TRUE;

  boolean asdiv_alt_gain = FALSE;

  /* Update the shared mem with new iqmc coefficients and ask fw 
     to perform dynamic rxlm update */
  for (rfm_device = RFM_DEVICE_0; rfm_device < RFM_WCDMA_MAX_DEVICE; rfm_device++)
  {
    valid_num_of_carrier = rfwcdma_data_get_num_carriers(rfm_device);

    if (rfwcdma_data_get_rf_state(rfm_device) == RFWCDMA_STATE_RX )
    {
      nv_tbl_ptr = rfnv_wcdma_get_nv_tbl_ptr(rfnv_wcdma_convert_band_rfcom_to_rfnv(rfwcdma_data_get_curr_band(rfm_device)));
      if (nv_tbl_ptr == NULL)
      {
        MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "NV table pointer is NULL! for device %d", rfm_device);
        return FALSE;
      }
      for (temp_lna_state = 0; temp_lna_state < RF_WDMA_MAX_NUM_LNA_STATES; temp_lna_state++)
      {
        bw.num_carr = rfwcdma_data_get_num_carriers(rfm_device);
        /* Initialize valid flag to TRUE */
        mdsp_data.data.iqmc_param.iqmc_valid[temp_lna_state] = TRUE;

        /* Get Alternate path info from RFC */
        alt_path_status = rfcmn_concurrency_mgr_update_rx_path( rfm_device,
                                                                RFCOM_WCDMA_MODE,
                                                                (int)rfwcdma_data_get_curr_band(rfm_device),
                                                                &alt_path);
        if (alt_path_status == RFCMN_PATH_SEL_FAILED) 
        {
           MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, 
               "rfwcdma_core_rxctl_update_rsb_coeff: Alternate path read failed");
        }

        /* Get RxLm chain and NV container from RFC */
        cfg.req = RFC_REQ_DEFAULT_GET_DATA;
        cfg.alternate_path = (uint32)alt_path;
        cfg.band = rfwcdma_data_get_curr_band(rfm_device);
        cfg.logical_device = rfm_device;
        cfg.rx_tx = RFC_CONFIG_RX;

        if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
        {
          rxlm_chain = device_info_ptr->modem_chain;
          nv_container_idx = device_info_ptr->nv_container;
          /* Get RSB Cal data from NV if exist */
          rsb_cal_data = &nv_tbl_ptr->rx_dyn_data.payload.rx_dyn_v2.rsb_cal_data[nv_container_idx];
          if (rsb_cal_data->valid == TRUE)
          {
            nv_rsb_data.nv_active = rsb_cal_data->rsb_data[temp_lna_state].nv_active;
            nv_rsb_data.sin_theta = rsb_cal_data->rsb_data[temp_lna_state].sin_theta;
            nv_rsb_data.cos_theta = rsb_cal_data->rsb_data[temp_lna_state].cos_theta;
            nv_rsb_data.gain_inv = rsb_cal_data->rsb_data[temp_lna_state].gain_inv;
            nv_rsb_data_ptr = &nv_rsb_data;
            MSG_5 ( MSG_SSID_RF, MSG_LEGACY_HIGH, 
                    "Get RSB Cal data: lna_state: %d nv_active: %d sin_theta: %d cos_theta %d, gain_inv: %d",
                    temp_lna_state, nv_rsb_data.nv_active, nv_rsb_data.sin_theta,
                    nv_rsb_data.cos_theta, nv_rsb_data.gain_inv );
          }/* if (rsb_cal_data->valid == TRUE) */
          else
          {
            MSG_3(MSG_SSID_RF, MSG_LEGACY_MED, 
                  "rfwcdma_core_rxctl_update_rsb_coeff: Rx Cal Data not present for NV cont idx: %d device %d, alt path: %d",
                  nv_container_idx, rfm_device, alt_path );
                  nv_rsb_data_ptr = NULL;

            //temp workaround ---@vbhide
            return TRUE;
          }
        }/* if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE) */
        else
        {
          MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
                "rfwcdma_core_rxctl_update_rsb_coeff: cannot get data from RFC for device %d, band %d",
                rfm_device, rfwcdma_data_get_curr_band(rfm_device));
          rxlm_chain = rxlm_get_chain(rfwcdma_data_get_rflm_buf_idx(rfm_device));
          nv_rsb_data_ptr = NULL;
        }

#ifdef FEATURE_RF_ASDIV
	    if(rfcommon_asdiv_get_current_position(rfm_device)==ASDIV_POSITION_1)
	    {
	      asdiv_alt_gain = TRUE;
	    }
	    else
	    {
              asdiv_alt_gain = FALSE;
	    }
#endif
        /* call rsb temp comp to get updated rsb coefficients A and B */
        rfdevice_wcdma_rx_get_rsb_coeff_online_temp_comp(rfm_device,
                                                         rfwcdma_data_get_curr_band(rfm_device),
                                                         bw.num_carr,
                                                         rfwcdma_core_temp_comp_get_raw_therm_read(scaled_therm),
                                                         temp_lna_state,
                                                         &a_coeff,
                                                         &b_coeff,
                                                         nv_rsb_data_ptr,
                                                         asdiv_alt_gain);
        
        /* Only update when there is valid B coefficiet*/
        if ((b_coeff != 0))
        {
          /* Query DTR to calcuate IQMC coefficient and update DM */
          mdsp_data.data.iqmc_param.iqmc_coeff[temp_lna_state] = 
          rfcommon_msm_get_iqmc_coeffs(rfwcdma_data_get_rflm_buf_idx(rfm_device),
                                       rxlm_chain,
                                       LM_UMTS,
                                       bw,
                                       a_coeff,
                                       b_coeff);

           MSG_7(MSG_SSID_RF, MSG_LEGACY_HIGH,
                 "RSB A:%d, RSB B:%d, lna_state:%d, rxlm_chain:%d, iqmc:%d, device:%d, rxlm_buf_idx:%d",
                 a_coeff, b_coeff, temp_lna_state, rxlm_chain, 
                 mdsp_data.data.iqmc_param.iqmc_coeff[temp_lna_state],
                 rfm_device,rfwcdma_data_get_rflm_buf_idx(rfm_device));

        }/* ((b_coeff != 0)) */
        else 
        {
          #ifndef FEATURE_TABASCO_MODEM /*This is needed since WTR2955 doesn't have rsb_coeff_temp_comp API defined*/
          /* If any LNA state has invalid data, invalidate all the IQMC data */
          mdsp_data.data.iqmc_param.iqmc_valid[temp_lna_state] = FALSE;
		  #endif
        }
      }/* for (temp_lna_state = 0; temp_lna_state < RF_WDMA_MAX_NUM_LNA_STATES; temp_lna_state++) */

      mdsp_data.rflm_handle = rfwcdma_data_get_rflm_buf_idx(rfm_device);
      mdsp_data.set_data_cmd = RFWCDMA_MDSP_SET_RX_IQMC;
      api_status &= rfwcdma_mdsp_async_set_data(&mdsp_data);
    }
  }/* for (rfm_device = RFM_DEVICE_0; rfm_device < RFM_DEVICE_4; rfm_device++) */

  return api_status;
}/* rfwcdma_core_rxctl_update_rsb_coeff */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function return the ant_mask

  @details
  depending on which device has been called in returns the ant_mask
  ant mask - 1 means enable Primary chain
  ant mask - 2 means enable Diversity chain
  ant mask - 3 means enable both chain ( This is not currently used)
  Caller who wants to enable both chains have to call this api twice
  and OR the masks

  @param rfm_device
  This is the current rfm device

 @return ant_mask
*/

void rfwcdma_core_rxctl_get_ant_mask(rfm_device_enum_type rfm_device, uint32 *ant_mask)
{
  rfm_wcdma_device_type dev_type = RFM_WCDMA_INVALID_DEVICE;

  dev_type = rfwcdma_data_get_device_type(rfm_device);

  if(dev_type == RFM_WCDMA_DIVERSITY_RX_DEVICE)
  {
    *ant_mask = 0x2;
  }

  else if(dev_type == RFM_WCDMA_PRIMARY_RX_DEVICE)
  {
    *ant_mask = 0x1;
  }

  else
  {
    RF_MSG_2(RF_ERROR,"rfwcdma_core_rxctl_get_ant_mask: Invalid Device type %d for device %d",
             dev_type, rfm_device);
    *ant_mask = 0x0;
  }
}/* rfwcdma_core_rxctl_get_ant_mask */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure Rx band parameters to MDSP module 

  @details
  This function will populate Rx band parameter data structure and pass down to
  RF-MDSP module.
 
  @param rxlm_handle
  RxLm handle
 
  @param rfm_device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param multi_carrier_idx
  Multi-Carrier index (bit mask) for the carrier/cell to be configured
 
  @param meas_scenario
  Enum to indicate IRAT scenario
 
  @param alt_path
  Alternate path which is in use
 
  @return
  Flag indicating the function execution success
*/
boolean rfwcdma_core_rxctl_config_band
(
  uint32 rxlm_handle,
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type band,
  uint32 multi_carrier_idx,
  rfwcdma_mdsp_rx_irat_meas_enum_type meas_scenario,
  uint32 alt_path,
  rfm_meas_rx_dev_handle_type meas_dev_type
)
{
  rfwcdma_mdsp_rx_cfg_bsp_type mdsp_cfg;
  rf_buffer_intf *lna_script_buffer[RF_WDMA_MAX_NUM_LNA_STATES][RFLM_WCDMA_LNA_MAX_TYPE] = { NULL };
  rf_buffer_intf *drx_script_buffer[RF_WCDMA_DRX_CTL_NUM][RFLM_WCDMA_LNA_MAX_TYPE] = { NULL };
  uint8 lna_state, drx_ctl;
  uint8 lna_type; 
  uint8 valid_ccs_tasks[RF_WDMA_MAX_NUM_LNA_STATES] = {0};
  rfdevice_wcdma_lna_param_type lna_param;
  boolean api_status = TRUE,status_set_index = TRUE;
  uint16 tx_chan, rx_chan = 0;
  uint64 tx_freq, rx_freq = 0;
  rfc_wcdma_core_config_type rfc_core_config = {0};
  rfcom_multi_carrier_hspa_id_type num_car;
  uint32 pll_settling_time = 200; // Do not change this number as currently device is not returning valid number

  uint32 nv_container =0;
  rfc_device_info_type *device_info_ptr;
  rfc_cfg_params_type cfg;
  rfm_device_enum_type paired_dev;

  /*FR30081 Arrays to hold offsets */  
  int16 lna_rise_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1]= {0}; 
  int16 lna_fall_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1] = {0};
  /*Flag to indicate if the AsDiv offsets are valid in NV*/
  boolean asdiv_alt_gain = FALSE;
  boolean is_consolidated = TRUE;
  rfnv_wcdma_nv_tbl_type *nv_tbl = rfnv_wcdma_get_nv_tbl_ptr(rfnv_wcdma_convert_band_rfcom_to_rfnv(band));

  if(nv_tbl == NULL)
  {
     RF_MSG_1(RF_ERROR, "rfwcdma_core_rxctl_config_band: NULL NV tbl ptr for band %d",
               band);
     return FALSE;
    
  }
  else
  {
    is_consolidated = nv_tbl->rx_static_data.payload.rx_stat_v2.is_consolidated;
  }


#ifdef FEATURE_RF_ASDIV  
  rfwcdma_nv_rx_asdiv_elna_switchpoint_type *rx_asdiv_switchpoint_ptr =NULL;
  uint32 asdiv_pos;
#endif
  /*In case of neighbor measurement use the info passed from meas call */

  if (meas_scenario == RFLM_WCDMA_NEIGHBOUR_MEAS ) 
  {
    if (meas_dev_type == DEV_HANDLE_PRX)
    {
      lna_param.rx_mode = RFDEVICE_PRX_OPER_MODE;
    }
    else if (meas_dev_type == DEV_HANDLE_DRX1)
    {
      lna_param.rx_mode = RFDEVICE_DRX_OPER_MODE;
    }
    else
    {
      lna_param.rx_mode = RFDEVICE_INVALID_OPER_MODE;
    }

    paired_dev = rfc_common_get_preferred_associated_rx_device(rfm_device);
  }
  else
  {
  if (rfwcdma_data_get_device_type(rfm_device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    lna_param.rx_mode = RFDEVICE_PRX_OPER_MODE;
  }
  else if (rfwcdma_data_get_device_type(rfm_device)== RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    lna_param.rx_mode = RFDEVICE_DRX_OPER_MODE;
  }
  else
  {
    lna_param.rx_mode = RFDEVICE_INVALID_OPER_MODE;
  }
    
  paired_dev = rfwcdma_data_get_associated_device_rx(rfm_device);
  }

  lna_param.paired_device = paired_dev;

  cfg.alternate_path = alt_path;
  cfg.band = band;
  cfg.logical_device = rfm_device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;


  /* NV table pointer needs to be intialized */
  if (band >= RFCOM_NUM_WCDMA_BANDS)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_rxctl_config_band: Not a valid band %d",
               band);
      return FALSE;
    }

  if (rfnv_wcdma_tbl_ptr[band] == NULL)
  {
     RF_MSG_1(RF_ERROR,
             "rfwcdma_core_rxctl_config_band: NULL NV tbl ptr for band %d",
               band);
      return FALSE;
  }

  memset(&mdsp_cfg, 0, sizeof(rfwcdma_mdsp_rx_cfg_bsp_type));

  /* Get LNA script RF buffer from device driver */
  num_car = rfwcdma_core_count_carriers(multi_carrier_idx);
  lna_param.device = rfm_device;
  lna_param.band = band;
  lna_param.carrier = num_car;

  if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
    //RF_MSG_3( RF_HIGH, "rfwcdma_core_rxctl_config_band: For Device:%d, band: %d NV container is: %d",
    //          rfm_device, band, device_info_ptr->nv_container);
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_core_rxctl_config_band: - Cannot get NV container for"
                         "device %d Div config, band %d",rfm_device,band);

  return FALSE;
  }

  if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
  {
    nv_container = device_info_ptr->nv_container; 
  }

#ifdef FEATURE_RF_ASDIV
  asdiv_pos = rfcommon_asdiv_get_current_position(rfm_device);

  if(asdiv_pos == ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  /*Load AsDiv offset values*/
  if(asdiv_pos >= ASDIV_POSITION_0 && asdiv_pos < ASDIV_POSITION_MAX)
  {
	rx_asdiv_switchpoint_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_switchpoint_ptr[nv_container][asdiv_pos];

    if(rx_asdiv_switchpoint_ptr != NULL)
    {
      for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES-1; lna_state++)
      {
	    lna_rise_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_rise;
	    lna_fall_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_fall;
      }

    }
  }  
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_core_rxctl_config_band: Invalid AsDiv position: %d",asdiv_pos);
  }
#endif

  for (lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
  {
    for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
    {
    /* Create local RF buffers for device script population */
      lna_script_buffer[lna_state][lna_type] = rf_buffer_create(RF_WDMA_RX_GEN_SSBI_SCRIPT_SIZE,
                                                    RF_WDMA_RX_GEN_RFFE_SCRIPT_SIZE,
                                                    RF_WDMA_RX_GEN_GRFC_SCRIPT_SIZE);

      if (lna_script_buffer[lna_state][lna_type] == NULL)
    {
      RF_MSG(RF_ERROR,
             "rfwcdma_core_rxctl_band_config: rf buffer allocation failed!");
      api_status = FALSE;
      break;
    }

      (void)rf_buffer_clear(lna_script_buffer[lna_state][lna_type]);
    }

    /* Set current LNA state */
    lna_param.current_state = lna_state;

    /* Retrieve LNA settings per gain state from rf device */
    rfdevice_wcdma_rx_lna_setup(rfm_device,
                                band,
                                &lna_param,
                                RFDEVICE_CREATE_SCRIPT,
                                  lna_script_buffer[lna_state][RFLM_WCDMA_LNA_WTR_TYPE]);


    status_set_index = rfdevice_wcdma_lna_set_gain(rfm_device,
                                                   band,
                                                   lna_script_buffer[lna_state][RFLM_WCDMA_LNA_EXT_TYPE],
                                                   RFDEVICE_CREATE_SCRIPT,
                                                   0,
                                                   lna_state,
                                                   asdiv_alt_gain); 


    /* Disable per-LNA state IQMC programming in RxAGC by default */                                                            
    mdsp_cfg.iqmc_param.iqmc_valid[lna_state] = FALSE;
  #ifdef FEATURE_TABASCO_MODEM 	
    rfwcdma_core_rxctl_update_rsb_coeff_per_lna(rfm_device, rxlm_handle, multi_carrier_idx, &mdsp_cfg, band, lna_state);
  #endif

    /* Update the number of CCS tasks for each LNA state*/
    for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
    {
      mdsp_cfg.device_cfg.lna[lna_state][lna_type].script = lna_script_buffer[lna_state][lna_type];
      mdsp_cfg.device_cfg.lna[lna_state][lna_type].delay = 0;
      if (rfwcdma_core_get_resource_script_from_buffer(lna_script_buffer[lna_state][lna_type])!=NULL)
        {
          
          valid_ccs_tasks[lna_state]++;
        }
    }
  	
	mdsp_cfg.device_cfg.num_ccs_tasks[lna_state] = valid_ccs_tasks[lna_state];
  }/* for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++) */

  /* Get dRx script RF buffer from device driver */
  for (drx_ctl = RF_WCDMA_DRX_ENABLE; drx_ctl < RF_WCDMA_DRX_CTL_NUM; drx_ctl++)
  {
    for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
    {
    /* Create local RF buffers for device script population */
      drx_script_buffer[drx_ctl][lna_type] = rf_buffer_create(RF_WDMA_RX_GEN_SSBI_SCRIPT_SIZE,
                                                  RF_WDMA_RX_GEN_RFFE_SCRIPT_SIZE,
                                                  RF_WDMA_RX_GEN_GRFC_SCRIPT_SIZE);

      if (drx_script_buffer[drx_ctl][lna_type] == NULL)
    {
      RF_MSG(RF_ERROR,
             "rfwcdma_core_rxctl_band_config: rf buffer allocation failed!");
      api_status = FALSE;
      break;
    }

      (void)rf_buffer_clear(drx_script_buffer[drx_ctl][lna_type]);
    } /*  for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)*/

    rfdevice_wcdma_rx_get_cpc_drx_val(rfm_device, band, (boolean)drx_ctl, RFDEVICE_CREATE_SCRIPT, drx_script_buffer[drx_ctl][RFLM_WCDMA_LNA_WTR_TYPE]);


    /* In DRx diable (Rx ON), need to reconfigure LNA state back to default before CGAGC gets restarted */
    if (drx_ctl == RF_WCDMA_DRX_DISABLE)
    {
      /* Populate LNA param to default LNA state before querying device driver and append script */
      lna_param.current_state = RF_WCDMA_DEFAULT_LNA_STATE;

      /* Set the gain state of WTR LNA */
      rfdevice_wcdma_rx_lna_setup(rfm_device,
                                  band,
                                  &lna_param,
                                  RFDEVICE_CREATE_SCRIPT,
                                  drx_script_buffer[drx_ctl][RFLM_WCDMA_LNA_WTR_TYPE]);

     /* Set the gain state of eLNA */
     status_set_index = rfdevice_wcdma_lna_set_gain(rfm_device,
                                                      band,
                                                      drx_script_buffer[drx_ctl][RFLM_WCDMA_LNA_EXT_TYPE],
                                                      RFDEVICE_CREATE_SCRIPT,
                                                      0,
                                                      lna_param.current_state,
                                                      asdiv_alt_gain);
    }

    /* Update the number of CCS tasks for each DRx control type*/
    for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
    {
      mdsp_cfg.device_cfg.drx[drx_ctl][lna_type].script = drx_script_buffer[drx_ctl][lna_type];
      mdsp_cfg.device_cfg.drx[drx_ctl][lna_type].delay = 0;
    } /* for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)*/

  }/* for (drx_ctl = RF_WCDMA_DRX_ENABLE; drx_ctl < RF_WCDMA_DRX_CTL_NUM; drx_ctl++) */

  //WTR3925 doesn't have an API to provide the PLL settling time
  
  mdsp_cfg.device_cfg.pll_settling_time = (uint32)WCDMA_CONV_US_TO_USTMR(pll_settling_time);

  /* Convert antenna index from RFM device */
  if ( (meas_scenario == RFLM_WCDMA_NEIGHBOUR_MEAS) && (meas_dev_type == DEV_HANDLE_PRX) ) 
  {
    mdsp_cfg.ant_idx = 0;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_config_band: Device passed: %d, Neigh PRx Antenna Idx: %d ", rfm_device, mdsp_cfg.ant_idx);
  }

  else if ( (meas_scenario == RFLM_WCDMA_NEIGHBOUR_MEAS) && (meas_dev_type == DEV_HANDLE_DRX1) ) 
  {
    mdsp_cfg.ant_idx = 1;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_config_band: Device passed: %d, Neigh DRx Antenna Idx: %d ", rfm_device, mdsp_cfg.ant_idx);
  }

  else if (meas_scenario == RFLM_WCDMA_HOME_MEAS)
  {
    mdsp_cfg.ant_idx = (uint8)rfwcdma_core_util_device_to_antenna(rfm_device);
  }

  else
  {
    RF_MSG_2(RF_ERROR, "rfwcdma_core_rxctl_config_band: Invalid scenario %d for converting device %d to Ant Index",
             meas_scenario, rfm_device);
    mdsp_cfg.ant_idx = 0;
  }

  /* Convert LNA index from RFM device */
  mdsp_cfg.lna_idx = (uint8)rfwcdma_core_util_device_to_lna_id(rfm_device, meas_scenario, multi_carrier_idx);

  /* extract the device pair for teh device passed in*/
  mdsp_cfg.device_pair = rfwcdma_core_get_device_pair(rfm_device);

  /* Populate Multi carrier index and RFLM handle */
  mdsp_cfg.multi_car_idx = multi_carrier_idx;
  mdsp_cfg.rflm_handle = rxlm_handle;

  /* Populate IRAT scenario */
  mdsp_cfg.meas_scenario = meas_scenario;

  /* Populate CGAGC parameters */
  rfwcdma_core_rxctl_cal_cgagc_params(rfm_device,
                                      band,
                                      num_car,
                                      &mdsp_cfg.cgagc_param, 
                                      alt_path);

  /* Populate BTF delay (Tx band paramter) in the Rx Config band() as FW reads BTF delay before Tx init */
  mdsp_cfg.btf_delay = rfnv_wcdma_tbl_ptr[band]->enc_btf_dly;

  /* Calculate Tx to Rx frequency Ratio of the mid band channel  */
  rx_chan = rfwcdma_core_get_middle_DL_chan_from_band(band);
  tx_chan = rfwcdma_core_map_rx_to_tx_chan(rx_chan, band);
  rx_freq = (uint64)rfwcdma_core_get_freq_from_uarfcn(band, rx_chan, RFWCDMA_CORE_UARFCN_DL);
  tx_freq = (uint64)rfwcdma_core_get_freq_from_uarfcn(band, tx_chan, RFWCDMA_CORE_UARFCN_UL);

  /* Q15 format*/
  mdsp_cfg.fgagc_band_param.tx_rx_freq_ratio = (uint16)((tx_freq << 15) / rx_freq);

  /* Get FGAGC LNA switchpoints */
  for (lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES - 1; lna_state++)
  {
   /*For WTR 2K this check will always get satisfied and we pick the non-HS RISE/FALL SP*/
   if((num_car == 0) || (is_consolidated == FALSE))
   {
    mdsp_cfg.fgagc_band_param.lnarise[lna_state] =
      rfnv_wcdma_get_info_from_rx_static_data((rfm_device_enum_type)nv_container,
                                              num_car,
                                              LNA_RISE,
                                              (lna_state),
                                              &(rfnv_wcdma_tbl_ptr[band]->rx_static_data) ) +
      lna_rise_asdiv_offset[lna_state];

    mdsp_cfg.fgagc_band_param.lnafall[lna_state] =
      rfnv_wcdma_get_info_from_rx_static_data((rfm_device_enum_type)nv_container,
                                              num_car,
                                              LNA_FALL,
                                              (lna_state),
                                              &(rfnv_wcdma_tbl_ptr[band]->rx_static_data) ) +
      lna_fall_asdiv_offset[lna_state];
   }
   else 
   {
    mdsp_cfg.fgagc_band_param.lnarise[lna_state] =
      rfnv_wcdma_get_info_from_rx_static_data((rfm_device_enum_type)nv_container,
                                              num_car,
                                              LNA_HS_RISE,
                                              (lna_state),
                                              &(rfnv_wcdma_tbl_ptr[band]->rx_static_data) ) +
      lna_rise_asdiv_offset[lna_state];

    mdsp_cfg.fgagc_band_param.lnafall[lna_state] =
      rfnv_wcdma_get_info_from_rx_static_data((rfm_device_enum_type)nv_container,
                                              num_car,
                                              LNA_HS_FALL,
                                              (lna_state),
                                              &(rfnv_wcdma_tbl_ptr[band]->rx_static_data) ) +
      lna_fall_asdiv_offset[lna_state];
   }
  }

  /* Populate the jam detect params from nv so it can be ported to FED api, for now using a temp unused nvs until the new ones are created*/
   mdsp_cfg.jam_det_param.jam_det_en = RFLM_WCDMA_JAM_DET_EN;
   mdsp_cfg.jam_det_param.rxagc_noise_floor_sq = RFLM_WCDMA_RXAGC_NOISE_FLOOR_SQUARED;
   mdsp_cfg.jam_det_param.rxagc_sat_det_threshold = RFLM_WCDMA_RXAGC_SAT_DET_THRESHOLD;

  /* Set LNA oride flag for FTM Cal mode */
  if (IS_FTM_CAL_MODE())
  {
    mdsp_cfg.fgagc_band_param.lna_oride_flag = TRUE;
    /* Set default oride state to 0 */
    mdsp_cfg.fgagc_band_param.lna_gain_state_oride = 0;
  }
  else
  {
    mdsp_cfg.fgagc_band_param.lna_oride_flag = FALSE;
  }

  /* Pass the data structure to MDSP async module */
  if (api_status == TRUE)
  {
    api_status = rfwcdma_mdsp_async_config_rx_band(&mdsp_cfg);
  }

  /* Destroy the local RF buffer to free up the memory */
  for (lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
  {
   for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
   	{
    api_status &= rf_buffer_destroy(lna_script_buffer[lna_state][lna_type]);
   	}
  }

  for (drx_ctl = RF_WCDMA_DRX_ENABLE; drx_ctl < RF_WCDMA_DRX_CTL_NUM; drx_ctl++)
  {
    for(lna_type = 0; lna_type < RFLM_WCDMA_LNA_MAX_TYPE; lna_type++)
    {
      api_status &= rf_buffer_destroy(drx_script_buffer[drx_ctl][lna_type]);
    }
  }

  return api_status;
}/* rfwcdma_core_rxctl_config_band */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure Rx band parameters to MDSP module 

  @details
  This function will populate Rx band parameter data structure and pass down to
  RF-MDSP module.
 
  @param rxlm_handle
  RxLm handle
 
  @param rfm_device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param multi_carrier_idx
  Multi-Carrier index (bit mask) for the carrier/cell to be configured
 
  @param meas_scenario
  Enum to indicate IRAT scenario 
 
  @param alt_path
  Alternate path which is in use
 
  @return
  Flag indicating the function execution success
 
  @note
  rfwcdma_core_rxctl_config_band needs to be called for band config first. Then can be called
  within the same band channel tuning without re-config band.
*/
boolean rfwcdma_core_rxctl_config_chan
(
  uint32 rxlm_handle,
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type band,
  uint16 channel[RFCOM_MAX_CARRIERS],
  uint32 multi_carrier_idx,
  rfwcdma_mdsp_rx_irat_meas_enum_type meas_scenario,
  uint32 alt_path,
  rfm_meas_rx_dev_handle_type meas_dev_type
)
{
  rfwcdma_mdsp_rx_cfg_chsp_type mdsp_cfg;
  rfc_device_info_type *device_info_ptr;
  rfc_cfg_params_type cfg;

  uint8 lna_state = 0;
  uint32 dl_freq, nv_container =0 ;
  boolean api_status;
  uint16 mid_chan = 0;
  rfcom_multi_carrier_hspa_id_type num_car = rfwcdma_core_count_carriers(multi_carrier_idx);

  /*FR30081 Array to hold offsets */
  int16 rx_gain_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES] = {0};   

#ifdef FEATURE_RF_ASDIV  
  rfwcdma_nv_rx_asdiv_elna_gain_offset_type *rx_asdiv_offset_ptr = NULL;
  uint32 asdiv_pos;
#endif
  cfg.alternate_path = alt_path;
  cfg.band = band;
  cfg.logical_device = rfm_device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_rxctl_config_chan: NULL NV tbl ptr for band %d",
               band);
      return FALSE;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
              "rfwcdma_core_rxctl_config_chan: Not a valid band %d",
              band);
    return FALSE;
  }

  memset(&mdsp_cfg, 0, sizeof(rfwcdma_mdsp_rx_cfg_chsp_type));

  /* Convert antenna index from RFM device */
  if ( (meas_scenario == RFLM_WCDMA_NEIGHBOUR_MEAS) && (meas_dev_type == DEV_HANDLE_PRX) ) 
  {
    mdsp_cfg.ant_idx = 0;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_config_chan: Device passed: %d, Neigh PRx Antenna Idx: %d ", rfm_device, mdsp_cfg.ant_idx);
  }

  else if ( (meas_scenario == RFLM_WCDMA_NEIGHBOUR_MEAS) && (meas_dev_type == DEV_HANDLE_DRX1) ) 
  {
    mdsp_cfg.ant_idx = 1;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_config_chan: Device passed: %d, Neigh DRx Antenna Idx: %d ", rfm_device, mdsp_cfg.ant_idx);
  }

  else if (meas_scenario == RFLM_WCDMA_HOME_MEAS)
  {
    mdsp_cfg.ant_idx = (uint8)rfwcdma_core_util_device_to_antenna(rfm_device);
  }

  else
  {
    RF_MSG_2(RF_ERROR, "rfwcdma_core_rxctl_config_chan: Invalid scenario %d for converting device %d to Ant Index",
             meas_scenario, rfm_device);
    mdsp_cfg.ant_idx = 0;
  }

  /* Convert LNA index from RFM device */
  mdsp_cfg.lna_idx = 
    (uint8)rfwcdma_core_util_device_to_lna_id(rfm_device, meas_scenario,multi_carrier_idx );

  /* Populate Multi carrier index and RFLM handle */
  mdsp_cfg.multi_car_idx = multi_carrier_idx;
  mdsp_cfg.rflm_handle = rxlm_handle;

  /* Populate IRAT Scenario */
  mdsp_cfg.meas_scenario = meas_scenario;

  if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
    RF_MSG_3( RF_HIGH, "rfwcdma_core_rxctl_config_chan: For Device:%d, band: %d NV container is: %d",
              rfm_device, band, device_info_ptr->nv_container);
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_core_rxctl_config_chan - Cannot get NV container for"
                         "device %d Div config, band %d",rfm_device,band);

  return FALSE;
  }

  if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
  {
    nv_container = device_info_ptr->nv_container; 
  }

#ifdef FEATURE_RF_ASDIV
  asdiv_pos = rfcommon_asdiv_get_current_position(rfm_device);

  /*Load AsDiv offset values*/
  if(asdiv_pos >= ASDIV_POSITION_0 && asdiv_pos < ASDIV_POSITION_MAX)
  {
    rx_asdiv_offset_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_offset_ptr[nv_container][asdiv_pos];  

	if(rx_asdiv_offset_ptr != NULL)
    {      
      for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
      {
	    rx_gain_asdiv_offset[lna_state] = rx_asdiv_offset_ptr->rx_gain_offset[lna_state];
      }
    }
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_core_rxctl_config_chan - Invalid AsDiv position: %d",asdiv_pos);
  }
  
#endif  
  
  /* Update Rx freq comp data */
  rfwcdma_core_rxctl_update_freq_offsets(rfm_device, band, channel[0], alt_path);

  /* ================== Populate LNA and DVGA offsets ======================= */
  for (lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
  {
    if (lna_state == 0)
    {
      /* LNA offset 0 is always 0 */
      mdsp_cfg.fgagc_chan_param.lna_offset[0] = 0;
    }
    else
    {
      if (!IS_FTM_CAL_MODE())
      {
        /* Get other LNA state offset from NV module */
        mdsp_cfg.fgagc_chan_param.lna_offset[lna_state] =
          rfwcdma_core_rxctl_lna_offset_vs_freq_val[rfm_device][lna_state] +
          rfnv_wcdma_get_info_from_rx_dynamic_data((rfm_device_enum_type)nv_container,
                                                   num_car,
                                                   LNA_OFFSET,
                                                   (lna_state - 1),
                                                   &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) ) +
          rx_gain_asdiv_offset[lna_state];
      }
      else
      {
        /* Zero out LNA offset for FTM Cal mode */
        mdsp_cfg.fgagc_chan_param.lna_offset[lna_state] = 0;
      }
    }
    /* RxAGC min value = LNA offset - 512 */
    mdsp_cfg.fgagc_chan_param.rxagcmin[lna_state] = 
      mdsp_cfg.fgagc_chan_param.lna_offset[lna_state] - 512;

    /* Set RxAGC max value to 512 for each lna state */
    mdsp_cfg.fgagc_chan_param.rxagcmax[lna_state] = 512;
  }

  /* ======= Populate XO paramters - DL freq and Inverse DL frequency ======= */
   if (num_car == RFCOM_SINGLE_CARRIER)
   {
     mid_chan = channel[0];
   }

   else if (num_car == RFCOM_DUAL_CARRIER)
   {
     mid_chan = (channel[0] + channel[1]) / 2;
   }

   else if (num_car == RFCOM_3_CARRIER)
   {
     mid_chan = (channel[0] + channel[1] + channel[2]) / 3;
   }
   else
   {
     MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "Invalid multi carrier index - %d and num of carriers - %d", 
           multi_carrier_idx, num_car);
     return 0;
   }

   dl_freq = rfwcdma_core_get_freq_from_uarfcn( band,
                                                (uint32)mid_chan,
                                                RFWCDMA_CORE_UARFCN_DL );

   // DL frequency(Hz) in Q6 format
   mdsp_cfg.xo_chan_param.dl_freq = ((uint64)dl_freq * 1000) << 6 ;

   /* Check for division by Zero */
   if (mdsp_cfg.xo_chan_param.dl_freq != 0)
   {
     // Inverse DL frequency(Hz) = (2^33) / (DL freq in Hz) in Q19 format; 52 = 33 + 19
     mdsp_cfg.xo_chan_param.inv_dl_freq = (uint32)( ((uint64)1 << 52) / (dl_freq * 1000) );
   }

   else
   {
     MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "Invalid band %d, channel %d and DL frequency %d", band, mid_chan, dl_freq);
     return 0;
   }

  if (!IS_FTM_CAL_MODE())
  {
    /* Get VGA gain offset */
    mdsp_cfg.fgagc_chan_param.dvga_gain_offset =
    rfwcdma_core_rxctl_vga_gain_offset_vs_freq_val[rfm_device] +
    rfwcdma_core_rxctl_vga_gain_offset_vs_temp_val +
    rfnv_wcdma_get_info_from_rx_dynamic_data((rfnv_wcdma_container_enum_type)nv_container,
                                             num_car,
                                             VGA_OFFSET,
                                             0,
                                             &(rfnv_wcdma_tbl_ptr[band]->rx_dyn_data) )+
                                             rx_gain_asdiv_offset[0];

    /* Get Static VGA gain offset from RXLM*/
    rfwcdma_msm_get_static_dvga_offset(mdsp_cfg.rflm_handle, (int32*)mdsp_cfg.fgagc_chan_param.static_dvga_offset);
  
  }
  else
  {
    /* Zero out DVGA gain offset for FTM Cal mode */
    mdsp_cfg.fgagc_chan_param.dvga_gain_offset = 0;
  }


  /* Pass the data structure to MDSP async module */
  api_status = rfwcdma_mdsp_async_config_rx_chan(&mdsp_cfg);

  return api_status;

}/* rfwcdma_core_rxctl_config_chan */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function disable and sleep ASM module for Rx

  @details
  It will call ASM disable Rx and ASM sleep for the given device. If it is
  primary device, it will loop through all supported WCDMA bands and sleep all
  associcated ASMs
 
  @param device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param script
  RF buffer script pointer
 
  @param execution_type
  Execution type of the script (immeidate or creat script)

  @param script_timing
  Execution timing of the script 

  @param disable_ind
  Indicate the disable operation type
 
  @return
  Flag indicating the function execution success
 
*/
boolean rfwcdma_core_rxctl_disable_sleep_asm
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing,
  rfwcdma_core_rxctl_dis_first_ind_type disable_ind
)
{
  rfcom_wcdma_band_type temp_band = RFCOM_BAND_IMT;
  boolean status = TRUE;

  if(RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND == disable_ind)
  {
    if ((rfwcdma_core_is_band_supported_on_dev(band,device)) &&
         rfwcdma_mc_get_asm_status_bit(device, band))
    {
      /*disable ASM RX before sleep*/
      status &= rfdevice_wcdma_asm_disable_rx(device,
                                              band,
                                              script,
                                              execution_type,
                                              script_timing
                                              );
      /* Put the ASM to sleep */
      status &= rfdevice_wcdma_asm_sleep_rx(device,
                                            band,
                                            script,
                                            execution_type,
                                            script_timing
                                            );
      status &= rfwcdma_mc_config_asm_status_bit_map(device,
                                                     band,
                                                     FALSE);
    }
  }/* (RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND == disable_ind) */
  else if(RFWCDMA_CORE_ASM_TUNER_DIS_ALL_BANDS == disable_ind)
  {
    for (temp_band = RFCOM_BAND_IMT; temp_band<RFCOM_NUM_WCDMA_BANDS; temp_band++)
    {
      if ((rfwcdma_core_is_band_supported_on_dev(temp_band,device)) &&
           rfwcdma_mc_get_asm_status_bit(device, temp_band))
      {
        /*disable ASM RX before sleep*/
          status &= rfdevice_wcdma_asm_disable_rx(device,
                                                  temp_band,
                                                  script,
                                                  execution_type,
                                                  script_timing
                                                  );

         /* Put the ASM to sleep */
         status &= rfdevice_wcdma_asm_sleep_rx(device,
                                               temp_band,
                                               script,
                                               execution_type,
                                               script_timing
                                               );
         status &= rfwcdma_mc_config_asm_status_bit_map(device,
                                                        temp_band,
                                                        FALSE);
      }
    }
  }/* if(RFWCDMA_CORE_ASM_TUNER_DIS_ALL_BANDS == disable_ind) */
  else
  {
    ;
  }

  return status;
}/* rfwcdma_core_rxctl_disable_sleep_asm */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update alternate path info to RFC for concurrency management

  @details
  This function will query concurrency manager for alternat path info and
  update port mapping in RFC
 
  @param device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @return
  Flag indicating the function execution success
 
*/
boolean rfwcdma_core_rxctl_update_alt_path
(  
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  boolean is_irat_neighbour,
  uint32 *alt_path_ptr
)
{
  rf_card_band_type rfc_band = RF_BCI_BAND;
  rfcmn_status_type alt_path_status = RFCMN_PATH_SEL_SUCCESS;
  uint8 alt_path = 0;

  if (alt_path_ptr == NULL)
  {
    ERR_FATAL("NULL alt_path poitner",0,0,0);
    return FALSE;
  }

  /* --------------------- Update concurrency manager for alt path info ---------------------*/

  rfc_band = rfwcdma_core_convert_band_rfcom_to_rf_card(band);

  if (is_irat_neighbour == FALSE) 
  {
    alt_path_status =  rfcmn_concurrency_mgr_update_rx_path( device,
                                                            RFCOM_WCDMA_MODE,
                                                            (int)band,
                                                            &alt_path);
  }

  else
  {
     alt_path_status =  rfcmn_concurrency_mgr_update_irat_rx_path( device,
                                                                   RFCOM_WCDMA_MODE,
                                                                   (int)band,
                                                                   &alt_path);
  }
  
  if (alt_path_status == RFCMN_PATH_SEL_FAILED)
  {
    RF_MSG_3(RF_ERROR, "rfwcdma_mc_rx_prep_tune: failed to query alt path "
                       "from concurrecy mgr on device:%d, band:%d, status:%d",
             device, rfc_band, alt_path_status);
    return FALSE;
  }
  else 
  {
    if (FALSE == rfc_update_alt_port_mapping( device, 
                                              RFCOM_WCDMA_MODE, 
                                              rfc_band, 
                                              alt_path))
    {
      RF_MSG_3(RF_ERROR, "rfwcdma_mc_rx_prep_tune: failed to update alt path "
                         "to concurrecy mgr on device:%d, band:%d, alt path:%d",
               device, rfc_band, alt_path);
      return FALSE;
    }
  }
  *alt_path_ptr = (uint32)(alt_path & 0xFF);
  return TRUE;
  /* ------------------End of Update Alt. Path ------------------*/
}/* rfwcdma_core_rxctl_update_alt_path */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function enable and wakeup ASM module for Rx

  @details
  It will call ASM enable Rx and ASM wakeup for the given device.
 
  @param device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param script
  RF buffer script pointer
 
  @param execution_type
  Execution type of the script (immeidate or creat script)
 
  @return
  Flag indicating the function execution success
 
*/
boolean rfwcdma_core_rxctl_enable_asm
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing
)
{
  boolean api_status = TRUE;

  if((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    /* Wake up ASM device */
    rfdevice_wcdma_asm_wakeup_rx(device,
                                 band,
                                 script,
                                 execution_type,
                                 script_timing);
    /* Configure ASM Rx */
    api_status &= rfdevice_wcdma_asm_enable_rx(device,
                                 band,
                                 script,
                                 execution_type,
                                 script_timing);
    api_status &= rfwcdma_mc_config_asm_status_bit_map(device,
                                 band,
                                 TRUE);
  }
  else
  {
    api_status = FALSE;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_enable_asm: Invalid param - device %d band %d",
                      device, band);
  }

  return api_status;
}/* rfwcdma_core_rxctl_enable_asm */

boolean rfwcdma_core_rxctl_enable_asm_grfc
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_buffer_intf *script,
  rf_buffer_intf *grfc_script,
  rf_device_execution_type execution_type,
  int16 script_timing
)
{
  boolean api_status = TRUE;

  if((device >= RFM_DEVICE_0) && (device < RFM_MAX_WAN_DEVICES) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    /* Wake up ASM device */
    rfdevice_wcdma_asm_wakeup_rx(device,
                                 band,
                                 script,
                                 execution_type,
                                 script_timing);
    /* Configure ASM Rx */
    api_status &= rfdevice_wcdma_asm_enable_rx_grfc(device,
                                 band,
                                 script,
                                 grfc_script,
                                 execution_type,
                                 script_timing);

    api_status &= rfwcdma_mc_config_asm_status_bit_map(device,
                                 band,
                                 TRUE);
  }
  else
  {
    api_status = FALSE;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_enable_asm: Invalid param - device %d band %d",
                      device, band);
  }

  RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_enable_asm: param - device %d band %d",
                      device, band);
  return api_status;
}/* rfwcdma_core_rxctl_enable_asm */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function enable and wakeup TUNER module for Rx

  @details
  It will call TUNER enable Rx.
 
  @param device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param script
  RF buffer script pointer
 
  @param execution_type
  Execution type of the script (immeidate or creat script)

  @param script_timing
  Execution timing of the script 
 
  @return
  Variable indicating the status of API execution.
 
*/
boolean rfwcdma_core_rxctl_enable_tuner
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rfcom_multi_carrier_hspa_id_type carr_id,
  uint16 chan_num,
  rf_device_execution_type execution_type,
  rf_buffer_intf *script
)
{
  boolean api_status = TRUE;
  rfcommon_atuner_config_req_type atuner_cfg_req;
  rfcom_band_type_u band_u;

  memset(&atuner_cfg_req, 0, sizeof(rfcommon_atuner_config_req_type)); 

  band_u.wcdma_band = band;
  atuner_cfg_req.rfm_device = device;
  atuner_cfg_req.band = band_u;
  atuner_cfg_req.mode = RFCOM_WCDMA_MODE;
  atuner_cfg_req.chan_num = chan_num;
  atuner_cfg_req.freq = rfwcdma_core_get_freq_from_uarfcn_no_limit(band,
                                                                   chan_num,
                                                                   RFWCDMA_CORE_UARFCN_DL);

  /* device type mapping */
  if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_PRX_DEVICE;
  }
  else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE)
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_DRX_DEVICE;
  }
  else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_TX_DEVICE) 
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_TX_DEVICE;
  }
  else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_INVALID_DEVICE) 
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_INVALID_DEVICE;
  }

  atuner_cfg_req.carrier_id = rfwcdma_core_map_carr_mask_to_atuner_id(rfwcdma_data_get_carrier_mask(device));

  if((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    api_status &= rfwcdma_core_antenna_tuner_init(&atuner_cfg_req,
                                                  script,
                                                  execution_type);

    if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE || rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE ) 
    {
      api_status &= rfwcdma_core_antenna_tuner_program_rx_mode (&atuner_cfg_req,
                                                              execution_type,
                                                              script);
    }

    /* For Tx mode program Tx mode will be called from Tx enable*/
    
    api_status &= rfwcdma_mc_config_tuner_status_bit_map(device,
                                                         band,
                                                         TRUE);
    
  }
  else
  {
    api_status = FALSE;
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_enable_tuner: Invalid param - device %d band %d",
                      device, band);
  }

  return api_status;
}/* rfwcdma_core_rxctl_enable_tuner */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function disable TUNER module for Rx

  @details
  It will call TUNER disable Rx for the given device. 
 
  @param device
  RFM device enum ID
 
  @param band
  WCDMA operation band
 
  @param script
  RF buffer script pointer
 
  @param execution_type
  Execution type of the script (immeidate or creat script)

  @param script_timing
  Execution timing of the script 

  @param disable_ind
  Indicate the disable operation type
 
  @return
  Variable indicating the status of API execution.
 
*/
boolean rfwcdma_core_rxctl_disable_tuner
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_device_execution_type execution_type,
  rf_buffer_intf *script,
  int16 script_timing,
  rfwcdma_core_rxctl_dis_first_ind_type disable_ind
)
{
  boolean api_status = TRUE;
  rfcom_wcdma_band_type temp_band = RFCOM_BAND_IMT;
  rfcommon_atuner_config_req_type atuner_cfg_req;
  rfcom_band_type_u band_u;
  rfcom_multi_carrier_hspa_id_type carr_id = rfwcdma_data_get_num_carriers(device);

  memset(&atuner_cfg_req, 0, sizeof(rfcommon_atuner_config_req_type)); 
  band_u.wcdma_band = band;

  atuner_cfg_req.rfm_device = device;
  atuner_cfg_req.band = band_u;
  atuner_cfg_req.mode = RFCOM_WCDMA_MODE;

  /*RF device type mapping */
  if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_PRX_DEVICE;
  }
  else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_DRX_DEVICE;
  }
  else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_INVALID_DEVICE) 
  {
    atuner_cfg_req.rx_tx = RFCMN_ATUNER_INVALID_DEVICE;
    RF_MSG_1(RF_ERROR, "rfwcdma_core_rxctl_disable_tuner called on invalid device%d.. returning", device);
    return FALSE;
  }

  atuner_cfg_req.carrier_id = rfwcdma_core_map_carr_mask_to_atuner_id(rfwcdma_data_get_carrier_mask(device));

  if(RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND == disable_ind)
  {
    if ((rfwcdma_core_is_band_supported_on_dev(band,device)) &&
         rfwcdma_mc_get_tuner_status_bit(device, band))
    {
      api_status &= rfwcdma_core_antenna_tuner_disable(&atuner_cfg_req,
                                         execution_type,
                                         script,
                                         script_timing);

      api_status &= rfwcdma_mc_config_tuner_status_bit_map(device,
                                                           band,
                                                           FALSE);
    }
  }
  else if(RFWCDMA_CORE_ASM_TUNER_DIS_ALL_BANDS == disable_ind)
  {
    for (temp_band = RFCOM_BAND_IMT; temp_band<RFCOM_NUM_WCDMA_BANDS; temp_band++)
    {
      if ((rfwcdma_core_is_band_supported_on_dev(temp_band,device)) &&
           rfwcdma_mc_get_tuner_status_bit(device, temp_band))
      {
        band_u.wcdma_band = temp_band;
        atuner_cfg_req.band = band_u;

        api_status &= rfwcdma_core_antenna_tuner_disable(&atuner_cfg_req,
                                           execution_type,
                                           script,
                                           script_timing);

        api_status &= rfwcdma_mc_config_tuner_status_bit_map(device,
                                                             temp_band,
                                                             FALSE);
      }
    }
  }

  return api_status;
}/* rfwcdma_core_rxctl_disable_tuner */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This callback is called by Device (WTR) to update dynamic spurs

  @details
  This will update Dynamic spurs to DTR and also send dynamic update
  command to FW for programming RxFE
 
  @param dyn_notch_filter_settings
  Dynamic spur settings passed from Device to be updated to MSM

  @return
  None
 
*/
void rfwcdma_core_rxctl_update_dynamic_notch_cb
(
 rfdevice_wcdma_spur_param_type* dyn_notch_filter_settings
)
{
  rfcom_device_enum_type rfm_device = RFM_INVALID_DEVICE;
  boolean status = FALSE;
  uint32 carrier = 0;
  uint16 channel[RFCOM_MAX_CARRIERS] = {0};
  rfwcdma_msm_dyn_notch_cmd_param_type notch_cmd_params = {0};
  rfcom_multi_carrier_hspa_id_type num_carr = RFCOM_MAX_CARRIERS;

  if ( dyn_notch_filter_settings == NULL ) 
  {
     RF_MSG(RF_ERROR,
            "rfwcdma_core_rxctl_update_dynamic_notch_cb: Notch Callback data in NULL" );
     return;
  }

  /* Proceed with notching only if we are in DBDC */
  for (rfm_device = RFM_DEVICE_0; rfm_device < RFM_MAX_WAN_DEVICES; rfm_device++)
  {
    if (2 == rfwcdma_data_get_carrier_mask(rfm_device))
    {
      break;
    }
    else
    {
       if ( rfm_device == (RFM_MAX_WAN_DEVICES - 1) )
       {
         RF_MSG(RF_ERROR, "rfwcdma_core_rxctl_update_dynamic_notch_cb: Skipping dynamic notch update as we are not in DBDC");
         return;
       }
       continue;
    }
  }
  rfm_device = dyn_notch_filter_settings->curr_logical_dev;

  if ( (rfm_device < RFM_DEVICE_0) || (rfm_device >= RFM_WCDMA_MAX_DEVICE) ) 
  {
     RF_MSG_1(RF_ERROR,
             "rfwcdma_core_rxctl_update_dynamic_notch_cb: Invalid device %d",
              rfm_device);
     return;
  }

  RF_MSG_1(RF_HIGH, "rfwcdma_core_rxctl_update_dynamic_notch_cb: Call back called for device %d", rfm_device);

  /* We apply dynamic notches immediately for devices which are already tuned
     and also clear notches during sleep*/
  if ( ( (dyn_notch_filter_settings->apply_notch_immediate == TRUE) && (dyn_notch_filter_settings->current_cell_spur_count > 0) ) ||
        (dyn_notch_filter_settings->clear_notch == TRUE)
      )
  { 
     /* Make sure we are tuned already from MC state machine */
     num_carr = rfwcdma_data_get_num_carriers(rfm_device);

     /* For RF state information, all the carriers should have same state. So here querying first carrier*/
     if ( (num_carr < RFCOM_MAX_CARRIERS ) && (rfwcdma_data_get_rf_state(rfm_device)== RFWCDMA_STATE_RX) )
     {     
       /* Extract channel information for all carriers so that it can be sent to MSM api in the form of array*/
       for (carrier = 0; carrier < num_carr+1; carrier++)
       {
         channel[carrier] = rfwcdma_data_get_curr_chan(rfm_device,carrier);
         /* RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_update_dynamic_notch_cb: Channel %d carrier %d", channel[carrier],carrier ); */
       }

       /* Update dynamic notch settings for the device callback called for. This will also send Dynamic udpate command to FW */
       if ( (dyn_notch_filter_settings->apply_notch_immediate == TRUE) &&
            (dyn_notch_filter_settings->current_cell_spur_count > 0)
          )
       {
          /* RF_MSG(RF_HIGH, "rfwcdma_core_rxctl_update_dynamic_notch_cb: Dynamic tune"); */

          /* Update Notch settings including static and Dynamic spurs */
          status = rfwcdma_msm_update_dynamic_notch(rfm_device,
                                                    channel,
                                                    rfwcdma_data_get_num_carriers(rfm_device),
                                                    rfwcdma_data_get_rflm_buf_idx(rfm_device),
                                                    dyn_notch_filter_settings,
                                                    RFWCDMA_MSM_DYNAMIC_NOTCH_TUNE,
                                                    &notch_cmd_params);

          /* Send Dynamic RxLM Notch update cmd to FW */
          status &= rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd( rfm_device,
                                                                    rfwcdma_data_get_rflm_buf_idx(rfm_device),
                                                                    &notch_cmd_params );
                                                                   
          if (status == FALSE) 
          {
            RF_MSG(RF_ERROR,"rfwcdma_core_rxctl_update_dynamic_notch_cb: Dynamic tune failed");
            return;
          }

       }

       else if(dyn_notch_filter_settings->clear_notch == TRUE)
       {
          /* Update Notch settings which includes only static spurs */
          status = rfwcdma_msm_update_dynamic_notch(rfm_device,
                                                    channel,
                                                    rfwcdma_data_get_num_carriers(rfm_device),
                                                    rfwcdma_data_get_rflm_buf_idx(rfm_device),
                                                    dyn_notch_filter_settings,
                                                    RFWCDMA_MSM_DYNAMIC_NOTCH_SLEEP,
                                                    &notch_cmd_params);

          /* Send Dynamic RxLM Notch update cmd to FW */
          status &= rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd( rfm_device,
                                                                    rfwcdma_data_get_rflm_buf_idx(rfm_device),
                                                                    &notch_cmd_params );

          if (status == FALSE) 
          {
            RF_MSG(RF_ERROR,"rfwcdma_core_rxctl_update_dynamic_notch_cb: Dynamic sleep failed");
            return;
          }
       }
     } /* if ( (num_carr < RFCOM_MAX_CARRIERS ) && (rfwcdma_data_get_rf_state(rfm_device)== RFWCDMA_STATE_RX) ) */  
  } /*     if ( ( (dyn_notch_filter_settings->apply_notch_immediate == TRUE) && (dyn_notch_filter_settings->current_cell_spur_count > 0) ) ||
        (dyn_notch_filter_settings->clear_notch == TRUE) ) */

  return;
}/* rfwcdma_core_rxctl_update_dynamic_notch_cb */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function sends Dynamic Notch update RxLM command to FW

  @details
  This is usually called during Dynamic tune (SCELL Tune)
  and Dynamic spur removal (SCELL sleep)
 
  @param device
  Device for which dynamic notch rxlm command needs to be sent
 
  @param rxlm_handle
  RxLM handle that needs dynamic spur update
 
  @param notch_params
  Notch parameters that is needed for Dynamic update

  @return
  API status of dynamic notch RxLM command
 
*/
boolean rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd
(
 rfm_device_enum_type device,
 uint32 rxlm_handle,
 const rfwcdma_msm_dyn_notch_cmd_param_type* notch_params
)
{
  rfwcdma_mdsp_dyn_rxlm_cmd_param_type dyn_rxlm_update_cmd_param = {{0},0,0};
  boolean dyn_cmd_status = FALSE;
  rfm_wcdma_device_type dev_type = RFM_WCDMA_INVALID_DEVICE;

  if (notch_params == NULL) 
  {
    RF_MSG_2(RF_ERROR,
            "rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd: Notch params is NULL for device %d with RxLM index %d", device,
             rxlm_handle);
    return FALSE;
  }

  dev_type = rfwcdma_data_get_device_type(device);

  /* RxLM index for the device */
  if(dev_type == RFM_WCDMA_DIVERSITY_RX_DEVICE)
  {
    dyn_rxlm_update_cmd_param.rxlm_buf_idx[1] = rxlm_handle; // RxD goes to chain 1
  }

  else if(dev_type == RFM_WCDMA_PRIMARY_RX_DEVICE)
  {
    dyn_rxlm_update_cmd_param.rxlm_buf_idx[0] = rxlm_handle; // Pri goes to chain 0
  }
  
  else
  {
    RF_MSG_2(RF_ERROR,"rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd: Invalid Device type %d for device %d",
             dev_type, device);
    return FALSE;
  } 

  /* Get DTR group mask for Notch Filter update */ 
  dyn_rxlm_update_cmd_param.dtr_group_mask = 0;    

  /* Common RxFE 01*/
  if ( notch_params->num_notch_cmn_grp01_regs > 0 )
  {
    dyn_rxlm_update_cmd_param.dtr_group_mask = rfwcdma_msm_get_dtr_group_mask(device, RFWCDMA_MSM_DTR_COMMON_NOTCH_01_GROUP);
  }

  /* Common RxFE 23*/
  if ( notch_params->num_notch_cmn_grp23_regs > 0 )
  {
    dyn_rxlm_update_cmd_param.dtr_group_mask |= rfwcdma_msm_get_dtr_group_mask(device, RFWCDMA_MSM_DTR_COMMON_NOTCH_23_GROUP);
  }

  /* Udpate RxLM for dedicated RxFE unconditionally */
  dyn_rxlm_update_cmd_param.dtr_group_mask |= rfwcdma_msm_get_dtr_group_mask(device, RFWCDMA_MSM_DTR_DEDICATED_NOTCH_GROUP);

  /* Rx chain antenna mask is specific to each rfm device */
  rfwcdma_core_rxctl_get_ant_mask(device, &dyn_rxlm_update_cmd_param.chain_mask);

  /* Send dynamic rxlm update cmd to fw */
  dyn_cmd_status = rfwcdma_mdsp_sync_send_cmd(RFWCDMA_MDSP_RXLM_DYN_UPDATE_CMD_ID, (void *)&dyn_rxlm_update_cmd_param);

  if (dyn_cmd_status == FALSE)
  {
     RF_MSG_2(RF_ERROR,
            "RxLM Dynamic udpate command failed for device %d with RxLM index %d", device,
            rxlm_handle);
  }

  return dyn_cmd_status;
}/* rfwcdma_core_rxctl_send_dynamic_notch_rxlm_cmd*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This is to get access to dynamic notch callback data
 
  @param device
  Device for which callback data needs to be accessed

  @return
  Dynamic notch callback data for the device
 
*/
rfdevice_wcdma_spur_param_type* rfwcdma_core_rxctl_get_dynamic_notch_cb_data
(
 rfm_device_enum_type device
)
{
   return &(rfwcdma_core_rxctl_dyn_notch_settings_cb[device]);
}/* rfwcdma_core_rxctl_get_dynamic_notch_cb_data */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This resets dynamic notch callback data for all devices
 
  @param
  None
 
  @return
  None
 
*/
void rfwcdma_core_rxctl_reset_dynamic_notch_cb_data()
{
  uint8 i = 0;

  for (i=0;i<RFM_MAX_WAN_DEVICES;i++) 
  {
    rfwcdma_core_rxctl_dyn_notch_settings_cb[i].apply_notch_immediate = FALSE;
    rfwcdma_core_rxctl_dyn_notch_settings_cb[i].clear_notch = FALSE;
    rfwcdma_core_rxctl_dyn_notch_settings_cb[i].curr_logical_dev = RFM_INVALID_DEVICE;
    memset(rfwcdma_core_rxctl_dyn_notch_settings_cb[i].current_cell_notch_depth_list, 0, RFDEVICE_WCDMA_MAX_SUPPORTED_NOTCHES * sizeof(uint16));
    memset(rfwcdma_core_rxctl_dyn_notch_settings_cb[i].current_cell_notch_freq_list, 0, RFDEVICE_WCDMA_MAX_SUPPORTED_NOTCHES * sizeof(int32));
    rfwcdma_core_rxctl_dyn_notch_settings_cb[i].current_cell_spur_count= 0;
  }
}/* rfwcdma_core_rxctl_reset_dynamic_notch_cb_data */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will generate GRFC script for all signals in this Rx Device
 
  @param device
  Rx RFM Device for which GRFC script needs to generated
 
  @param band
  RFCOM WCDMA Band for which GRGC script needs to be generated
 
  @param grfc_buf_ptr
  GRFC SCript Buffer Pointer has the list of GRFC writes
 
  @return status
  Status of the script generation api
*/
boolean rfwcdma_core_rxctl_generate_grfc_script
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_buffer_intf *grfc_buf_ptr
)
{
  boolean api_status = FALSE;
  rfc_sig_cfg_type *rfc_rx_grfc_tbl_ptr = NULL;

  if( (device >= RFM_MAX_DEVICES) || (band >= RFCOM_NUM_WCDMA_BANDS) )
  {
    RF_MSG_2(RF_ERROR, "rfwcdma_core_rxctl_generate_grfc_script: Invalid device %d or band %d", 
             device, band);
    return api_status;
  }

  rfc_rx_grfc_tbl_ptr =  
    (rfc_sig_cfg_type *)rfc_wcdma_get_rx_band_config( device,
                                                      band, 
                                                      RFC_GRFC_DATA );
  
   /*==========================================================================
    For each valid pointer, Look up GRFC Engine Number 
           and populate the IRAT GRFC buffer with correct logic.
   ==========================================================================*/

  
  if( rfc_rx_grfc_tbl_ptr != NULL )
  { 
    api_status = rfc_common_generate_grfc_script( rfc_rx_grfc_tbl_ptr, grfc_buf_ptr ,
                                                  0 /* delay */ );
  }

  else
  {
    RF_MSG_2(RF_HIGH, "rfwcdma_core_rxctl_generate_grfc_script: No GRFCs for device %d, band %d", 
             device, band);
    api_status = TRUE;
  }

  return api_status;
  
} /*rfwcdma_core_rxctl_generate_grfc_script*/
/*----------------------------------------------------------------------------*/
/*! This function is init API common between Rx and Tx. Need to be moved to rfwcdma_core.h*/
/*!
  @brief
  This function will generate GRFC script for mode config.
  This is called once during before generating Rx or Tx specific GRFC scripts

  @param grfc_buf_ptr
  GRFC SCript Buffer Pointer has the list of GRFC writes
 
  @return status
  Status of the script generation api
*/
boolean rfwcdma_core_generate_mode_config_grfc_script
(
  rf_buffer_intf *grfc_buf_ptr
)
{
  boolean api_status = FALSE;
  rfc_sig_cfg_type *rfc_init_grfc_tbl_ptr = NULL;

  rfc_init_grfc_tbl_ptr =  
    (rfc_sig_cfg_type *)rfc_wcdma_get_mode_config_data(RFC_MODE_GRFC_DATA);
  
   /*==========================================================================
    For each valid pointer, Look up GRFC Engine Number 
           and populate the IRAT GRFC buffer with correct logic.
   ==========================================================================*/

  
  if( rfc_init_grfc_tbl_ptr != NULL )
  { 
    api_status = rfc_common_generate_grfc_script( rfc_init_grfc_tbl_ptr, grfc_buf_ptr ,
                                                  0 /* delay */ );
  }

  else
  {
    RF_MSG(RF_HIGH, "rfwcdma_core_generate_mode_config_grfc_script: No GRFCs in Mode config");
    api_status = TRUE;
  }

  return api_status;
  
} /*rfwcdma_core_generate_mode_config_grfc_script*/



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Configure the Rx RSB paramters per LNA state 

  @details
  This function will populate Rx RSB values per LNA and pass down to
  RF-MDSP module.
 
  @param rfm_device
  RFM device enum ID

  @param rxlm_handle
  RxLm handle 
 
  @param multi_carrier_idx
  Multi-Carrier index (bit mask) for the carrier/cell to be configured
 
  @param mdsp_cfg
  Data structure to push the RSB value

  @param band
  WCDMA operation band
 
  @param lna_state
  LNA state
 
  @return
  Flag indicating the function execution success
*/

void rfwcdma_core_rxctl_update_rsb_coeff_per_lna
(
rfm_device_enum_type rfm_device,
uint32 rxlm_handle,
uint32 multi_carrier_idx,
rfwcdma_mdsp_rx_cfg_bsp_type* mdsp_cfg,
rfcom_wcdma_band_type band,
uint8 lna_state
)
{
  rfdevice_wcdma_rx_rsb_type rsb_data = {0};
  rxlm_chain_type rxlm_chain = RXLM_CHAIN_MAX;
  uint32 rsb_coeff_val = 0;
  rfcommon_msm_num_carr_bw wcdma_bw = {0};
  boolean asdiv_alt_gain = FALSE;
  rfcom_multi_carrier_hspa_id_type num_chan = rfwcdma_core_count_carriers(multi_carrier_idx);
  
#ifdef FEATURE_RF_ASDIV
  if(rfcommon_asdiv_get_current_position(rfm_device)==ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  else
  {
    asdiv_alt_gain = FALSE;
  }
#endif  
  
  /*Update the RSB params to query the WTR drivers*/
  rsb_data.band =  band;
  rsb_data.lna_gain_state = lna_state;
  if(num_chan == RFCOM_SINGLE_CARRIER)
  {
    rsb_data.num_chan = RFCOM_SINGLE_CARRIER;
    wcdma_bw.num_carr = RFCOM_SINGLE_CARRIER;
  }
  else if(num_chan == RFCOM_DUAL_CARRIER)
  {
    rsb_data.num_chan = RFCOM_DUAL_CARRIER;
    wcdma_bw.num_carr = RFCOM_DUAL_CARRIER;	
  }
  else
  {  
    RF_MSG_1(RF_ERROR, "rfwcdma_core_rxctl_update_rsb_coeff_per_lna: Invalid number of channels %d ",
	num_chan);
  }
  
  rxlm_chain = rxlm_get_chain(rxlm_handle);
  
  /* Get RSB coefficients from device driver. WTR2955 doesn't have the Temp based RSB API defined */
  rfdevice_wcdma_rx_compute_rsb_coeff(rfm_device, band, &rsb_data, 	RFDEVICE_EXECUTE_IMMEDIATE, NULL, NULL, asdiv_alt_gain); 

  /*Calculate the IQMC coeff*/
  rsb_coeff_val = rfcommon_msm_get_iqmc_coeffs(rxlm_handle, rxlm_chain, LM_UMTS, wcdma_bw, rsb_data.rsb_a, rsb_data.rsb_b);

  /*Push the IQMC value to the DM to be programmed during LNA update*/
  mdsp_cfg->iqmc_param.iqmc_coeff[lna_state] = rsb_coeff_val;
 
  mdsp_cfg->iqmc_param.iqmc_valid[lna_state] = FALSE; 
 
  RF_MSG_3(RF_LOW, "IQMC RxLM RSB A %d B %d RSB Coeff %d", rsb_data.rsb_a, rsb_data.rsb_b, rsb_coeff_val); 
}
