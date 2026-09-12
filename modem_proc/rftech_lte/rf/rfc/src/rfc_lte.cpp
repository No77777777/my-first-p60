/*!
   @file
   rfc_lte.cpp

   @brief
   This file contains implementation the RFC LTE class
   The RFC LTE card is an abstract class that acts as an interface to
   abstract different RF cards the system may support 

   The implementation aspects of this file contains only the following:
   1. Contructor
   2. Destructor
   3. Get_Instance for the singleton

*/

/*===========================================================================

  Copyright (c) 2010 - 2023 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2023/03/17 02:43:42 $ $Author: mplcsds1 $
$Header: //components/rel/rftech_lte.mpss/3.11/rf/rfc/src/rfc_lte.cpp#4 $

when       who     what, where, why
-------------------------------------------------------------------------------
03/16/23   rk      [CR3427105] Updated valid alt path
12/20/22   rk      [CR3361508]Fix No HDET issue due to GRFC control
06/28/22   sl      Tx Alt Path Fix
02/17/20   cv      Replacing SRS only config with ASDIV
11/18/19   cv      TX alt path support
03/02/18   ks      RFFE based XSW support
02/19/18   ks      F3 reduction
10/26/17   gk      Compilation Fixes for migration of 6.x LLVM to 8.x
07/29/16   rj      Remove flooding RF LTE F3 from rfc_lte.cpp
07/21/16   am      GRFC config change during tx band config
06/13/16   ani     Reinitialising RX GRFC during tx_config to eliminate inconsistency due to
                   shared GRFC
05/11/16   rj     Move F3 to debug SSID "MSG_SSID_RF_DEBUG"
05/10/16   rj     Move F3 to debug SSID "MSG_SSID_RF_DEBUG"
05/02/16   sbo     Updated the WTR->ADC mapping table for VCM cal.
03/10/16   cv      Modified Split Band decision algorithm
02/23/16   sk      Pass FB chain to FBRX module from RFC
02/22/16   sk      Pass Rx band to get FB device
01/27/16   sk      Get LTE Feedback Device from RFC
01/19/16    vc     Update the WTR ICI API to check intra + inter ICI.
01/06/16   vkm     F3 optimization - moving to new debug SSID "MSG_SSID_RF_DEBUG"
01/06/16   sml     Changes to enable XSW immediate writes
12/10/15   hm      FR 31436: API to provide the HW band limitation to LTE RRC
11/20/15   bir     Fixed KW errors
11/06/15   vkm     Enable usage of device mutex lock during accessing of
                   LTE device instances
11/03/15   sml    Access LTE device instances under a device mutex locks
11/03/15   vkm     Correct/Pass CA mode and ALT PATH in meas cases
11/02/15    vc     Rx/Tx split changes
10/28/15   dbc     Add configure_asm_xsw_port for antenna switching
10/27/15   vkm     Reset all device instances before it is updated
                   during ALT PATH update
10/24/15   stw     MC Level Band Decision maker added to handle split bands
10/21/15   mah     Ensure to account for alt-path when building band mask per device
10/21/15   mah     iRAT restriction band mask support
10/13/15   Saul    GNSS Blanking
09/28/15   stw     FR28012 :Alternate Path Used to find correct Split Band from RFC for Rx tuning
09/11/15   ska     Adding path override logic for determining IntraCA feasible
07/25/15   JJ      Added FBRx coupler/switch re-program support
07/24/15   dyc     Un-paired (dummy) tuner support.
07/21/15   Saul    Debug messages.
07/15/15   vkm     eLNA support
07/08/15   Saul    RF Device Max Bandwidth Interface
06/29/15   Saul    Use Tx device for fbrx-related RFC queries
10/24/14   lhb     Give band infromation for GRFC PA device
06/25/15   pkg     Changes to support Intra ULCA Tune on bands with splits.
06/23/15   cv      Reconfig Rx signal in case previous tx band share the same signal 
                   with current Rx band
06/12/15   sb      Handle request type other than get default data separately
06/12/15   kab     Revert - "Memset LTE Device pointer database before every update"
06/08/15   pkg     Fix KW errors related to RFM_MAX_WAN_DEVICES usage.
06/05/15   sb      Guard device specific band mask update under 
                   request type RFC_REQ_DEFAULT_GET_DATA
05/29/15   Saul    Call generate_rfcard_dynamic_properties at card creation.
05/25/15   sml     Update the Rx/Tx support on a device only on default get data 
                   request to RFC
05/19/15   pkg     Populate PA related information only for TX without Antenna Swap.
05/12/2015 rcui    pass device num to vreg params
04/21/15   ndb     Remove the Atuner CA PORT layer
04/13/15   lhb     Ported : GRFC Alt Path Support
03/30/15    rp     Code changes to initialize all Tx devices for ULCA.
03/23/15   dbc     Add support for ATUNER_CA_PORT_2 dummer tuner manager assignment
03/04/15   kab     Memset LTE Device pointer database before every update
03/05/15   vv      API support for rfc to capture modem chain to trx and VCO path mapping 
12/12/14    aa     Added changes to use FBRx GS3 during XPT cal
01/07/15   vv      API support for rfc to capture the modem chain to trx mapping  
01/06/15   kab     LTE B32, LTE-U 125,126,127 Support
12/15/14   ars     Adding check_intra_ca_feasible() API to fix issues with intra CA on split bands
12/11/14   dw/qzh  Adding additional check to ignore invalid bands while fetching
                   split band info from RFC.
12/05/14   dw      Changes to support split bands based on split specified in RFC.
11/18/14   dyc     Support updating rfc_asdiv_config_info table from RFC info
11/03/14   ska     Adding fix for correctly picking device pointer for B29
10/23/14   cri     Initialize band support and only scan valid bands
10/21/14   px      Added RFM Device support for PAPM
10/20/14   ndb     Remove the un-used ASD interface  + Add the GRFC scripts in 
                   in SRS ASD API
10/07/14   ndb     Added 3DL CA support(Phase 2)
09/16/14   sn      ASM set band RFM Device support
08/25/14   bar     Added support for Jolokia RUMI
08/15/14   st      NS07 Port Update Support
07/31/14   ndb     Added support for extended EARFCN for LTE
07/10/14   jf      TM8 + SRS switching for TDD
07/08/14   gvn     Modify algorithm for split-intraband support
07/02/14   jf      AsDiv support
06/16/14   tks     Added alternate path support for update port mapping
06/13/14   kab     Add 3xCA support
06/11/14   bar     Added disable_fb_device() API to support IQ capture 
06/06/14   gvn     Use band passed in, instead of global variable in reset_grfc
05/15/14   gvn     Add Api for getting overlapping split bands
05/14/14   jf      Remove F3 MSG
04/23/14   tks     Support for dyanmic path selection
04/11/14   npi     Clean up common device pointer usage
04/11/14   bsh     Split Band Support
04/09/14   yb      Added support for Coupler
04/02/14   Saul    RFC. Intermediate check-in for alternate path.
03/27/14   svi     [ASDIV] asdiv_tuner to support multiple RF configurations
03/25/14   yb      Added support for Tuner Linearizer
01/09/14   svi     Split Band Support
01/07/13   cri     fix for common class implementation of et path config
12/09/13   kab     Fixed compiler warnings
11/20/13   rp      Changes to support class based implementation for Common 
                   device driver.
10/31/13   shb     Converted TRX common device interface to C++
10/23/13   cri     Enable D3925 sample capture
10/07/13   gvn     Fix error checking in reset_grfc function
09/19/13   aks     Updated functions to support LTE device driver C++ Interface
08/12/13   bsh     Separate core and util functions
07/22/13   as      Added support for RFDEVICE_ASD_TUNER_MANAGER logical device init
07/09/13    pv     Added support for separating out TDD Tx, Rx ASM scripts.
06/06/13    pv     Added support for multple TDD ASM scripts.
06/06/13   php     Remove reset GRFC to Init as MC already resets
05/20/13   kai     Get the feedback path attenuation state from rfc
05/13/13   gvn     Removed unwanted F3s
05/06/13   vb      Bug fix for assiging the Tuner port object
05/06/13   vb      Support for Tuner manager factory
05/02/13   vb      KW Fix: Check for Tuner Manager validity
05/02/13   vb      Tuner CA changes 
04/10/13   kab     Added support for Tx Antenna Swap.
03/22/13   sr      changes to PAPM device interface.
03/20/13   kab     Decreased memory allocated to lte_rf_devices
03/12/13   sn      XPT dual WTR fix for feedback path
03/05/13   vrb     Replace RFC accessor param init_req boolean by a req enum
02/15/13   aca     Tuner manager programming update
02/12/13   aca     Tuner manager support
01/28/13   sbm     Enable LPM for HDET.
01/24/13   php     Initialize HDET device object
01/15/13   gvn     Remove FEATURE_RF_HAS_QTUNER
12/21/12   pv      Fixed the bug for wrong band port enum for more than 1 transceiver chip.
12/18/12   bsh     KW errors fixed
11/29/12   gvn     QTUNER API Integration
11/15/12   adk     Backed out Qtuner code
09/14/12   jf      Qtuner LTE support (initial)
10/31/12   vss     Change to get device API for returning a list of objects,
                    made changes to populate list of rfdevices into lte_rfdevices hash table
10/30/12   sr      changes create tech rfc object if the tech is supported by 
                   the specific rf-card.
10/15/12   sbm     Set tx band map for Q-PA device.
10/02/12   sbm     Null check for asm device type.
08/29/12   pl/sbm  Initialize global rx/tx band array to invalid at constructor and exit mode
08/28/12   vss     Add support for PA and QPoet (power tracking device) 
08/22/12   pl      Fix bug on RX band config
08/21/12   vss     Add support for RFDEVICE_PA and RFDEVICE_ASM. 
08/16/12   pl      Update RFC configuration strucutre to reflect new changes in RFC 
08/10/12   vrb     Use updated interfaces to push band port information to dev
08/08/12   vss     Update set_rx_port and set_tx_port to use device instance instead of RFM device 
08/03/12   sr      initialized the rf_devices[] ptrs .
07/30/12   pl      Fix warning
07/27/12   sr      renamed rfdevice related enum types.
07/18/12   sr      added get_device api to return the rf device obj ptr
                   based on logical_path, rx_tx, band and device_type.
07/12/12   sr      push band-port mapping info to devices 
07/09/12   sr      changes to use the rfc_data object instead of rfc_ag function ptr table.
06/01/12   aca     Fix to clean up Tx signals when disable tx is called
05/24/12   jf      Move the manage_vregs() to base-class from rf-card specific files 
05/18/12   sr      RFC-EFS code clean-up changes.
05/07/12   jf      Added support for rfc binary data 
04/24/12   cd      Fixed LTE exit mode to restore init settings for both Tx and Rx band GRFC/ANT_SELs
03/08/12   php     Added support for TXP2 signals
03/01/12   jf    Added LPM status support
12/05/11   tnt     Add mode_sleep() api
01/03/11   bmg     Removed delete from class destructor, see comments.
12/15/10   pl      Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
extern "C" {
#include "rfc_card.h"
#include "rfdevice_lte_interface.h"
#include "rflte_util.h"
#include "rflte_mc.h"
#include "rflte_state.h"
} // Extern "C"

#include "rfc_lte.h"
#include "rfc_common.h"
#include "rfc_card_lte.h"
#include "modem_mem.h"   
#include "rfc_vreg_mgr.h"
#include "rfc_lte_data.h"
#include "rfc_class.h"
#include "rfdevice_rxtx_common_class.h"
#include "rfdevice_asm.h"
#include "rfdevice_lna_cfg.h"
#include "rfdevice_lna.h"
#include "rfdevice_pa.h"
#include "rfdevice_papm.h"
#include "rfdevice_coupler.h"
#include "rfcommon_atuner_manager_factory.h"
#include "rfcommon_msg.h"
#include "rfcommon_fbrx_api.h"
#include "rfdevice_xsw.h"
#include "rfc_common_grfc_pa.h"
#include "rfcommon_core_utils.h"
#include "rflte_mc_rfc_interface.h"
#include "rfcommon_locks.h"


#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_tuner_manager.h"
#include "rfdevice_xsw_intf.h"
#endif
#include "rfcommon_atuner_manager_intf.h"

#include "rfcommon_core.h"

rfc_lte_intf *rfc_lte_intf::RFC_LTE_p = (rfc_lte_intf *)NULL;
static rfdevice_class *rfc_lte_common_grfc_pa_device = NULL;

rf_lock_data_type rflte_rf_devices_mutex;
// Singleton GetInstance method to instantiate Object
rfc_lte_intf * rfc_lte_intf::Get_Instance(void)
{
  return RFC_LTE_p;
}

// Craete the Singleton Object
rfc_lte_intf * rfc_lte_intf::create_instance(void)
{
  if (RFC_LTE_p == NULL )
  {
    RFC_LTE_p = (rfc_lte_intf *)new rfc_lte_intf();
    /* Device init is now moved to outside/after constructor to solve 
     issue where device info from this same rfc object is needed by rfdevice objects
     for rfc dynamic properties generation yet rfc object has not yet been constructed.
     The derived class must be initialized before init_rfdevices_for_all_bands() is called,
     which is why it's post-construction */
    if (RFC_LTE_p != NULL )
    {
      // initialize rfdevices with required rfc data
      RFC_LTE_p->init_rfdevices_for_all_bands();
    }
  }
  return RFC_LTE_p;
}

// Constructor 
rfc_lte_intf::rfc_lte_intf()
{
  int8 i = 0;
  uint8 log_dev = 0;
  uint8 rx_tx = 0;
  uint8 band = 0;
  uint8 dev_type = 0;
  uint8 dev_instance = 0;


  for(i = 0; i < RFCOM_NUM_LTE_BANDS; i++)
  {
   band_lpm_enable_status[i] = FALSE;
  }
  band_lpm_enable_status[RFCOM_BAND_LTE_B13] = TRUE;

  rf_common_init_critical_section(&rflte_rf_devices_mutex);

  // initialize the lte_rf_devices
  for (log_dev = 0; log_dev < RFM_MAX_WAN_DEVICES; log_dev++)
  {
    rx_band[log_dev] = RFCOM_BAND_LTE_INVALID;
    tx_band[log_dev] = RFCOM_BAND_LTE_INVALID;

    for(rx_tx = 0; rx_tx < RFC_CONFIG_RXTX_MAX; rx_tx++)
    {
      for (band = 0; band < RFCOM_NUM_LTE_BANDS; band++)
      {
        for(dev_type = 0; dev_type < RFDEVICE_TYPE_MAX_NUM; dev_type++)
        {
          for (dev_instance = 0; dev_instance <RFC_MAX_DEVICE_INSTANCES_PER_BAND; dev_instance++)
          {
          lte_rf_devices[log_dev][rx_tx][band][dev_type][dev_instance] = NULL;
          }
        }
      }
    }
  }
}

// Destructor
rfc_lte_intf::~rfc_lte_intf()
{

  RFC_LTE_p = NULL;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function performs one RFC initialization during power-up for LTE.

   @details

*/
void rfc_lte_intf::init(void)
{
 /* do nothing for now */
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function puts the RFC Tx signals in their default state.

   @details

*/

void rfc_lte_intf::tx_disable(rfm_device_enum_type device,rfcom_lte_band_type band)
{
  rfc_cfg_params_type cfg = { RF_PATH_0,  RFM_DEVICE_0, RFC_CONFIG_TX, RFCOM_NUM_LTE_BANDS, RFC_REQ_DEFAULT_GET_DATA};
  rfc_sig_cfg_type *rf_signals = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfm_device_capability_type capability = RFM_DEVICE_INVALID_SUPPORT;
  uint8 sub_id = rflte_mc_common_get_active_sub_id();
  uint32 carr_idx = rflte_state_get_carrier_idx_from_band(sub_id,band);
  uint8 cfg_alt_path=0;
  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }

  capability = rfc_common_get_device_capability( device );
  if( capability == RFM_DEVICE_TX_SUPPORTED )
  {
    /* set the previous band GRFC settings into default state */
    
   rflte_get_alt_path_from_current_device_table(sub_id,device, &cfg_alt_path);
   cfg.alternate_path = (uint32) cfg_alt_path;
   if ( cfg.alternate_path == RFLTE_MC_INVALID_ALTERNATE_PATH )
   {
     cfg.alternate_path = RFLTE_MC_DEFAULT_ALTERNATE_PATH;
   }
    cfg.logical_device = device;
    cfg.rx_tx = RFC_CONFIG_TX;
    cfg.band = band;
    cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    rfc_data->sig_cfg_data_get(&cfg, &rf_signals);
    if (rf_signals != NULL)
    {
      rfc_common_init_default(rf_signals);
    }
    else
    {
      RF_MSG_3( RF_ERROR, "RF LTE error. No signals returned. device %d band %d altp %d. Cannot disable Tx", device, band, cfg.alternate_path );
    }
  }
  else
  {
    RF_MSG_1( RF_ERROR,"RF LTE device %d is not a Tx device. Cannot disable Tx.", device);
  }

  //reset the tx bands 
  tx_band[device] = RFCOM_NUM_LTE_BANDS;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function performs the RFC sleep for LTE mode, where the RFC signals will be put their default
   state.

   @details

*/
void rfc_lte_intf::mode_sleep(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  rfcom_band_type_u band_u;
  uint64 sys_band;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }

  cfg.alternate_path = alt_path;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.band = band;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
  if (signal_list_ptr != NULL)
  {
    rfc_common_init_default(signal_list_ptr);
  }
  else
  {
    RF_MSG_3( RF_ERROR, "RF LTE error. No signals returned. device %d band %d altp %d. Cannot config Rx sleep", device, band, alt_path );
  }

  // Get corresponding Tx device
  band_u.lte_band = band;
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_LTE_MODE , band_u);
  tx_device = rfc_common_get_preferred_tx_device( (sys_band_class_e_type) sys_band);

  if (tx_device <= RFM_MAX_WAN_DEVICES)
  {
    cfg.logical_device = tx_device;
    cfg.rx_tx = RFC_CONFIG_TX;
    cfg.band = band;
    cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
    if (signal_list_ptr != NULL)
    {
      rfc_common_init_default(signal_list_ptr);
    }
    else
    {
      RF_MSG_3( RF_ERROR, "RF LTE error. No signals returned. device %d band %d altp %d. Cannot config Tx sleep", device, band, alt_path );
    }
  }

  //reset the rx and tx bands 
  rx_band[device] = RFCOM_NUM_LTE_BANDS;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function performs the RFC exit for LTE mode, where the RFC signals will be put their default
   state.

   @details

*/
void rfc_lte_intf::mode_exit(rfm_device_enum_type device, rfcom_lte_band_type rx_bnd, rfcom_lte_band_type tx_bnd, uint8 alt_path)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  rfcom_band_type_u band_u;
  uint64 sys_band;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }

  cfg.alternate_path = alt_path;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.band = rx_bnd;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
  if (signal_list_ptr != NULL)
  {
    rfc_common_init_default(signal_list_ptr);
  }
  else
  {
    RF_MSG_3( RF_ERROR, "RF LTE error. No signals returned. device %d band %d altp %d. Cannot config Rx exit", device, rx_bnd, alt_path );
  }

  //tx_device = rfc_common_get_preferred_associated_tx_device( device );

  band_u.lte_band = tx_bnd;
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_LTE_MODE , band_u);
  tx_device = rfc_common_get_preferred_tx_device( (sys_band_class_e_type) sys_band);
  
  if (tx_device <= RFM_MAX_WAN_DEVICES)
  {
    cfg.logical_device = tx_device;
    cfg.rx_tx = RFC_CONFIG_TX;
    cfg.band = tx_bnd;
    cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
    if (signal_list_ptr != NULL)
    {
      rfc_common_init_default(signal_list_ptr);
    }
    else
    {
      RF_MSG_3( RF_ERROR, "RF LTE error. No signals returned. device %d band %d altp %d. Cannot config Tx exit", device, tx_bnd, alt_path );
    }
  }

  rx_band[device] = RFCOM_BAND_LTE_INVALID;
  tx_band[device] = RFCOM_BAND_LTE_INVALID;
 
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function configure the RFC for LTE mode of operation.

   @details

*/
void rfc_lte_intf::mode_config(rfm_device_enum_type device)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }

  //MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"Select LTE mode device %d", device);

  /* initialize the RFC rx and tx band information */
  rx_band[device] = RFCOM_BAND_LTE_INVALID;
  tx_band[device] = RFCOM_BAND_LTE_INVALID;
  
  // Setup Rx GRFCs
  cfg.alternate_path = 0;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_INIT;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr );
  if (signal_list_ptr != NULL)
  {
    rfc_common_config_signal(signal_list_ptr, TRUE /* gnss_blanking_enabled */ );
  }
  else
  {
    RF_MSG_1( RF_ERROR, "RF LTE error. No signals returned. device %d. Cannot config Rx mode", device );
  }

  // Setup Tx GRFCs
  // Get corresponding Tx device
  // we dont have band info to use rfc_common_get_preferred_tx_device, it is also not mandatory at mode config
  tx_device = rfc_common_get_preferred_associated_tx_device( device );
  if (tx_device <= RFM_MAX_WAN_DEVICES)
  {
    cfg.logical_device = tx_device;
    cfg.rx_tx = RFC_CONFIG_TX;
    cfg.req = RFC_REQ_INIT;
    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr );
    if (signal_list_ptr != NULL)
    {
      rfc_common_config_signal(signal_list_ptr, TRUE /* gnss_blanking_enabled */ );
    }
    else
    {
      RF_MSG_1( RF_ERROR, "RF LTE error. No signals returned. device %d. Cannot config Tx mode", device );
    }
  }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function return the LTE mode RFC configuration info. It enables us to make the 
   accessor function pointer point to cases where we have either TX or RX config and init
   flag set to true

   @details

*/
void *rfc_lte_intf::mode_config_get (rfm_device_enum_type device, rfc_mode_config_data_enum_type config_data, uint8 alt_path)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  void                      *ret_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return ret_ptr;
  }

  if( (device < RFM_DEVICE_0) || (device > RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return ret_ptr;
  }

  if (alt_path >= RFLTE_MC_INVALID_ALTERNATE_PATH)
  {
     alt_path = RFLTE_MC_DEFAULT_ALTERNATE_PATH;
  }  
  
  cfg.alternate_path = alt_path;

  switch (config_data)
  {
  case RFC_MODE_TX_GRFC_ENGINE_DATA:

    // Setup configuration parameters
    cfg.logical_device = device;
    cfg.rx_tx = RFC_CONFIG_TX;
    cfg.req = RFC_REQ_INIT;

    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);          
    ret_ptr = (void *)(signal_list_ptr);
    break;

  case RFC_MODE_RX_GRFC_ENGINE_DATA:

    // Setup configuration parameters
    cfg.logical_device = device;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.req = RFC_REQ_INIT;

    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);          
    ret_ptr = (void *)(signal_list_ptr);
    break;


  default:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Unsupported data type %d", config_data);          
    break;
  }

  return ret_ptr;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function handles RFC related generic commands.

   @details

*/
int32 rfc_lte_intf::command_dispatch( rfm_device_enum_type device, int32 cmd, void *data )
{
  rfcom_lte_band_type band = RFCOM_BAND_LTE_INVALID;

  if(data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," data is NULL for device (%d)", device);
    return 0;
  }

  switch(cmd)
  {
    case RFC_LTE_LPM_ENABLE_STATUS:      
      band = ((rfc_lte_band_lpm_type*) data)->band;
      if ((device == RFM_DEVICE_0 || device == RFM_DEVICE_2) && band < RFCOM_NUM_LTE_BANDS)
      {
        ((rfc_lte_band_lpm_type*) data)->lpm_band_status = band_lpm_enable_status[band];
      }
      else
      {
        MSG_3(MSG_SSID_RF_DEBUG, MSG_LEGACY_ERROR," invalid cmd (%d) for device (%d) and band(%d)", cmd, device, band);
      }
      break;

    default:
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
            "invalid cmd (%d) for device (%d)", cmd, device);
      break;
  }
  return 0;
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function resets the GRFC list for the LTE Rx for the specified band. 

   @details

*/
void rfc_lte_intf::reset_grfc_band_config( rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if( (device < RFM_DEVICE_0) || (device > RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"reset_grfc_band_config: Invalid device %d", device);
    return;
  }

  // Setup common configuration parameters
  cfg.alternate_path = alt_path;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  cfg.band = (int)band;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
//  MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"reset_grfc_band_config: Setting to defaults,RX band enum %d on device %d", band, device);
  if (signal_list_ptr != NULL)
  {
    rfc_common_init_default(signal_list_ptr);
  }
  else
  {
    RF_MSG_3( RF_ERROR, "RF LTE error. No signals available. device %d band %d altp %d. Cannot reset band config", device, band, alt_path );
  }
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function configures the RFC for the LTE Rx for the specified band. 

   @details

*/
void rfc_lte_intf::rx_band_config( rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path )
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }

  // Setup common configuration parameters
  cfg.alternate_path = alt_path;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  /*Cleanup for previous band done, now start configuration for the new band*/
  signal_list_ptr = NULL;
  cfg.band = (int)band;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
  if (signal_list_ptr != NULL)
  {
    rfc_common_config_signal(signal_list_ptr, rfcommon_get_gnss_blanking_is_enabled( RFCOM_LTE_MODE, band ) );
  }
  else
  {
    RF_MSG_3( RF_ERROR, "RF LTE error. No signals available. device %d band %d altp %d. Cannot config Rx band", device, band, alt_path );
  }

  /*Update the curr band*/
  rx_band[device] = band;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the  rx band RFC configuration ino. for the specified rx band.

   @details

*/
void *rfc_lte_intf::rx_band_config_get (rfm_device_enum_type device, rfcom_lte_band_type band,
                                        rfc_band_config_data_enum_type config_data, uint8 alt_path)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type          *signal_list_ptr = NULL;
  void                      *ret_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return ret_ptr;
  }

  if( (device < RFM_DEVICE_0) || (device > RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return ret_ptr;
  }


  // Setup configuration parameters
  cfg.alternate_path = alt_path;
  cfg.logical_device = device;
  cfg.band = band;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);

  /* Retrieve requested data if exists */
  switch(config_data)
  {
    case RFC_GPIO_DATA:
    case RFC_GRFC_DATA:
    case RFC_RF_CTRL_DATA:
      ret_ptr = (void *)(signal_list_ptr);
      break;
    case RFC_CORE_DATA:
      // Dime RFC Changes : Should query the device instead of RFC to get the port info.
      // ret_ptr = (void *)(&(signal_list_ptr->rf_asic_info.rf_asic_port_id));
      break;
    default :
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Unsupported data type %d", config_data);          
      break;
  }

  return (ret_ptr);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function configures the RFC for the LTE Tx for the specified band. 

   @details

*/
void rfc_lte_intf::tx_band_config( rfm_device_enum_type device, rfcom_lte_band_type band )
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  uint8 active_rx_dev = 0;
  boolean gnss_blanking_enabled;
  uint8 sub_id = rflte_mc_common_get_active_sub_id();
  uint32 carr_idx = rflte_state_get_carrier_idx_from_band(sub_id,band);
  uint8 cfg_alt_path=0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if( (device < RFM_DEVICE_0) || (device >= RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return;
  }


  MSG_2(MSG_SSID_RF_DEBUG, MSG_LEGACY_MED,"Select LTE TX band enum %d on device %d", band, device);

  gnss_blanking_enabled = rfcommon_get_gnss_blanking_is_enabled( RFCOM_LTE_MODE, band );

  // Setup configuration parameters
  rflte_get_alt_path_from_current_device_table(sub_id,device, &cfg_alt_path);

  cfg.alternate_path = (uint32) cfg_alt_path;
 
  if ( cfg.alternate_path == RFLTE_MC_INVALID_ALTERNATE_PATH )
  {
    cfg.alternate_path = RFLTE_MC_DEFAULT_ALTERNATE_PATH;
  }
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  /*Before configuring the new band(parameter "band"), need to move back the GRFC/GPIOs configured for previous band
  (tx_band[device])to their default state*/
  if((tx_band[device] != band) && (tx_band[device] != RFCOM_BAND_LTE_INVALID) && (tx_band[device] != RFCOM_NUM_LTE_BANDS))
  {
    cfg.band = (int)tx_band[device];
    rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
    MSG_2(MSG_SSID_RF_DEBUG, MSG_LEGACY_MED,"tx_band_config: Setting to defaults, TX band enum %d on device %d", band, device);
    if (signal_list_ptr != NULL)
    {
      rfc_common_init_default(signal_list_ptr);
    }
    else
    {
      MSG_2( MSG_SSID_RF_DEBUG, MSG_LEGACY_ERROR, "RF LTE error. No signals available. device %d band %d altp %d. Cannot config Tx band", device, cfg.band);
    }

    /* Need to reconfig Rx signal as the previous tx band might have shared 
       the same signal with current Rx band*/
     /*Reconfiguring Rx path in all the active devices  */
	for( active_rx_dev = 0 ; active_rx_dev < RFM_MAX_WAN_DEVICES ; active_rx_dev++)
	{
	   if((rx_band[active_rx_dev] == band) && (rx_band[active_rx_dev] != RFCOM_BAND_LTE_INVALID) && 
	   	  (rx_band[active_rx_dev] != RFCOM_NUM_LTE_BANDS))
	   {
              cfg.rx_tx = RFC_CONFIG_RX;
              signal_list_ptr = NULL;
	      cfg.logical_device = (rfm_device_enum_type)active_rx_dev;
              cfg.band = rx_band[active_rx_dev];
              rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
              rfc_common_config_signal(signal_list_ptr, gnss_blanking_enabled );	
	   }
	}
    cfg.rx_tx = RFC_CONFIG_TX;
  }

  signal_list_ptr = NULL;
  cfg.band = band;
  cfg.logical_device = device;
  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);
  rfc_common_config_signal(signal_list_ptr, gnss_blanking_enabled );

  /*Update the curr band*/
  tx_band[device] = band;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the  tx band RFC configuration ino. for the specified tx band.

   @details

*/
void *rfc_lte_intf::tx_band_config_get (rfm_device_enum_type device, rfcom_lte_band_type band,
                                        rfc_band_config_data_enum_type config_data)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  void                      *ret_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  uint8 sub_id = rflte_mc_common_get_active_sub_id();
  uint32 carr_idx = rflte_state_get_carrier_idx_from_band(sub_id,band);
  uint8 cfg_alt_path=0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return ret_ptr;
  }

  if( (device < RFM_DEVICE_0) || (device > RFM_MAX_DEVICES) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE device not valid, device %d ", device);
    return ret_ptr;
  }

  // Setup configuration parameters
  rflte_get_alt_path_from_current_device_table(sub_id,device, &cfg_alt_path);

  cfg.alternate_path = (uint32) cfg_alt_path;
 
  if ( cfg.alternate_path == RFLTE_MC_INVALID_ALTERNATE_PATH )
  {
    cfg.alternate_path = RFLTE_MC_DEFAULT_ALTERNATE_PATH;
  }
  cfg.logical_device = device;
  cfg.band = band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_data->sig_cfg_data_get(&cfg, &signal_list_ptr);

  /* Retrieve requested data if exists */
  switch(config_data)
  {
    case RFC_GPIO_DATA:
    case RFC_GRFC_DATA:
    case RFC_RF_CTRL_DATA:
      ret_ptr = (void *)(signal_list_ptr);
      break;
    case RFC_CORE_DATA:
      // Dime RFC changes : should query the device to get the port info.
      // ret_ptr = (void *)(&(signal_list_ptr->rf_asic_info.rf_asic_port_id));
      break;
    default :
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Unsupported data type %d", config_data);          
      break;
  }

  return (ret_ptr);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function configures the vregs for LTE mode

   @details

*/
void rfc_lte_intf::manage_vregs(rfc_power_management_info* pm_info)
{
  rfc_vreg_param vreg_param(pm_info->path, pm_info->mode);
  vreg_param.set_band(pm_info->band);
  vreg_param.set_device(pm_info->rf_device);
  vreg_param.set_rf_state(pm_info->state);

  rfc_vreg_mgr *vreg_mgr_p = rfc_vreg_mgr::get_instance();
  if (vreg_mgr_p != NULL)
  {
    vreg_mgr_p->lte_manage_power(vreg_param);
  }
  else
  {
    ERR_FATAL("Cannot Create VREG_MGR for  N6600 NA card",0,0,0);
  }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the device object ptr for the specified logical_path,
  rx or tx, band and device type.

  @details
  This function returns the device object ptr for the specified logical_path,
  rx or tx, band and device type.

*/
void** rfc_lte_intf::get_rf_device_obj(rfm_device_enum_type rfm_dev, rfc_rxtx_enum_type rx_tx,
                                      rfcom_lte_band_type band, rfdevice_type_enum_type dev_type)
{
  void **dev_obj = NULL;
  uint8 antenna = 0;
  rfdevice_class *asd_tuner_mgr = NULL;

  if (rfm_dev >= RFM_MAX_WAN_DEVICES || rx_tx >= RFC_CONFIG_RXTX_MAX 
      || band >= RFCOM_NUM_LTE_BANDS || dev_type >= RFDEVICE_TYPE_MAX_NUM)
  {
    MSG_4(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_lte_intf::get_device_obj(%d, %d, %d, %d): Incorrect Parameters",
          rfm_dev, rx_tx, band, dev_type);
    return dev_obj;
  }

  dev_obj = lte_rf_devices[rfm_dev][rx_tx][band][dev_type];

#ifdef FEATURE_RF_ASDIV
  /** If Asdiv Tuner Manager is present get the correct list of device objects 
      based on the ASDiv Switch position for specific Antenna */
  
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  
  asd_tuner_mgr = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASD_TUNER_MANAGER, 0); 

  if ((dev_type == RFDEVICE_TUNER_MANAGER) && (asd_tuner_mgr!= NULL))
  {
    antenna = ant_params[rfm_dev][rx_tx][band].antenna_num ;

    dev_obj = (void **)(((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->get_asdiv_device_list(antenna, dev_type, (rfdevice_class **)dev_obj));
  }
#endif /* FEATURE_RF_ASDIV*/

  return dev_obj;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function initializes the rfdevices with the rfc dependent data.

  @details
  This function initializes the rfdevices with the rfc dependent data.

*/
void rfc_lte_intf::init_rf_devices(rfc_cfg_params_type *cfg, rfc_device_info_type *device_info_ptr)
{
  rfdevice_class *cmn_dev_obj = NULL;
  rfdevice_type_enum_type dev_type = RFDEVICE_TYPE_INVALID;
  uint32 instance = 0;
  uint8 i = 0;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfcom_band_type_u band_u;
  uint8 trx_dev_instance = 0;
  uint8 pa_dev_instance = 0;
  uint8 asm_dev_instance = 0;
  uint8 papm_dev_instance = 0;
  uint8 tuner_dev_instance = 0;
  uint8 hdet_dev_instance = 0;
  uint8 coupler_dev_instance = 0;
  uint8 xsw_dev_instance = 0;
  uint8 lna_dev_instance = 0;
  uint8 temp_port_state = RFC_PORT_STATE_DEFAULT;
  uint8 default_port_state = RFC_PORT_STATE_A;
  rfdevice_trx_lte_rx *rx_dev_obj = NULL;
  rfdevice_trx_lte_tx *tx_dev_obj = NULL;
  rfdevice_trx_phy_path_enum_type phy_path= RFDEVICE_TRX_MAX_PATHS;
  uint8 ant_num = 0;
  /* eLNA related */
   rfdevice_lna_cfg lna_cfg_c;
  boolean tmp_status = FALSE;
  rfc_dynamic_properties_params_type rfc_dynamic_params;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfdevice_lte_get_rx_intra_ca_info_type ca_info;
  rfdevice_lte_get_rx_path_info_type path_info;
  int num_trx_rx_device = 0;

#ifdef FEATURE_RF_ASDIV
  rfdevice_class *asd_tuner_mgr = NULL;
#endif

  rfc_sig_cfg_type* mode_rf_signals = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (NULL == rfc_cmn)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC CMN Object is NOT Created", 0);
    return;
  }

  if (NULL == cfg || NULL == device_info_ptr)
  {
    /* band is not supported */
    // MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"init_rf_devices(0x%x, 0x%x) : Bad parameters!", cfg, device_info_ptr);
    return;
  }
  if (cfg->logical_device >= RFM_MAX_WAN_DEVICES || cfg->rx_tx >= RFC_CONFIG_RXTX_MAX 
      || cfg->band >= RFCOM_NUM_LTE_BANDS)
  {
    MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_lte_intf::init_rf_devices: logical_dev, rx_tx, band (%d, %d, %d ): Incorrect Parameters",
          cfg->logical_device, cfg->rx_tx, cfg->band);
    return;
  }

  ant_num = device_info_ptr->ant_num;
  band_u.lte_band = (rfcom_lte_band_type)cfg->band;
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band( RFM_LTE_MODE, band_u );

  memset( &rfc_dynamic_params, 0x0, sizeof(rfc_dynamic_params) );

  if (cfg->port_state == RFC_PORT_STATE_A)
  {
    temp_port_state = RFC_PORT_STATE_A;

  while (i < device_info_ptr->num_devices)
  {
    dev_type = device_info_ptr->rf_asic_info[i].device_type;
    instance = device_info_ptr->rf_asic_info[i].instance;
    cmn_dev_obj = rfc_cmn->get_cmn_rf_device_object(dev_type, (uint8)instance);
    if (cmn_dev_obj == NULL)
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"cmd dev obj is NULL %d %d", dev_type,instance);
    i++;
      continue;
    }

    switch(dev_type)
    {
      case RFDEVICE_TRANSCEIVER:

          /* Push the band-port information to device */
        if (RFC_CONFIG_RX == cfg->rx_tx) /* rx device data */
        {
          if (trx_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
          {
            /* Read phy path from RFC */
            phy_path= 
              (rfdevice_trx_phy_path_enum_type)
              device_info_ptr->rf_asic_info[trx_dev_instance].phy_path_num;

            /* Get the RX device ptr */
            rx_dev_obj = 
              (rfdevice_trx_lte_rx *)
              ((rfdevice_rxtx_common_class*)cmn_dev_obj)\
                    -> get_tech_instance( RFM_LTE_MODE,
                                          RFDEVICE_RX, 
                                          phy_path);

            if( NULL == rx_dev_obj)
            {
              RF_MSG(RF_ERROR, "RFC LTE: Unable to get Rx device ptr!!");
            }

            if(cfg->logical_device > RFM_MAX_WAN_DEVICES)
            {
              RF_MSG(RF_ERROR, "RFC LTE: cfg->logical_device out of bound");
            }

            /* Store Rx Device ptr */
            lte_rf_devices\
              [cfg->logical_device][cfg->rx_tx]\
              [cfg->band][dev_type][trx_dev_instance] = rx_dev_obj;

            /* Call Rx set band data */
            rfdevice_lte_rx_set_band_data
            (
              rx_dev_obj,
              (rfcom_lte_band_type)cfg->band, 
              device_info_ptr->rf_asic_info[trx_dev_instance].data,
              RFC_ASIC_INFO_DATA_SIZE ,
              cfg->logical_device
            );

            trx_dev_instance++;
          } /*end of  if (trx_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND) */
          else
          {
            RF_MSG_1 ( RF_ERROR,"Trx device Prx instance overflow trx_dev_instance: %d", 
                       trx_dev_instance );
            return;
          }/* end of else if (trx_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND) */
        }/* end of if ( RFC_CONFIG_RX == cfg->rx_tx ) */

        if (RFC_CONFIG_TX == cfg->rx_tx) /* tx device data */
        {
          if (trx_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
          {
              tx_dev_obj =
              (rfdevice_trx_lte_tx *)
              ((rfdevice_rxtx_common_class*)cmn_dev_obj)\
                 -> get_tech_instance( RFM_LTE_MODE,
                                       RFDEVICE_TX, 
                                       RFDEVICE_TRX_PATH_0 );

          /* Temporarilty push Device index 0 data to the transceiver.
          This assumes index 0 in the list of devices is the transceiver device.
          This needs to be updated to find the transceiver device using RFDEVICE_TRANSCEIVER id*/
              lte_rf_devices\
                 [cfg->logical_device][cfg->rx_tx]\
                 [cfg->band][dev_type][trx_dev_instance] = tx_dev_obj;

              rfdevice_lte_tx_set_band_data
              (
                tx_dev_obj,
                                        (rfcom_lte_band_type)cfg->band, 
                                        device_info_ptr->rf_asic_info[trx_dev_instance].data,
                                        RFC_ASIC_INFO_DATA_SIZE,
                cfg->logical_device
              );
        }
          else
          {
              RF_MSG_1
              (
                 RF_ERROR,
                 "TRX Tx device instance overflow "
                 "trx_dev_instance: %d", 
                 trx_dev_instance
              );
        }
        }/* end of if (RFC_CONFIG_TX == cfg->rx_tx */

      if (cfg->rx_tx == RFC_CONFIG_RX)
      {
        num_trx_rx_device++;
         /*call the below api to capture the transciever->phy path->modem chain(ADC) mapping to perform adc dc cal.*/
        if(num_trx_rx_device == 1)
        {
          if(!rfc_cmn->set_trx_phy_path_adc_mapping(
	                             device_info_ptr->modem_chain,                    /*modem chain to map the ADC*/
	                             device_info_ptr->rf_asic_info[i].phy_path_num,   /*physical path*/
	                             device_info_ptr->rf_asic_info[i].instance,       /*Transciever device type instance*/
	                             RFM_LTE_MODE,                                    /*tech*/
	                             band_u,                                          /*band*/
	                             (rfdevice_class*)rx_dev_obj,                     /*rx device pointer*/
	                             device_info_ptr->rf_asic_info[i].data,           /*port info*/
	                             RFC_ASIC_INFO_DATA_SIZE                          /*data size*/
	                         )
              )
              {
	         RF_MSG_3( RF_ERROR, "rfc_cmn->set_trx_phy_path_adc_mapping() failed. check rfc"
	                              "modem_chain=%d,phy_path_num=%d, trx logical instance=%d  ",
	                              device_info_ptr->modem_chain, 
	                              device_info_ptr->rf_asic_info[i].phy_path_num,
	                              device_info_ptr->rf_asic_info[i].instance
	                             );
	       }
  	  }
        }


        /////////////////// START RFC DYNAMIC PROPERTIES SECTION ///////////////////

        // Get tranceiver indication of restriction to single tech
        tmp_status = TRUE;
        if( !rfdevice_cmn_get_rxtx_simultaneous_rat_support( (rfdevice_rxtx_common_class*) cmn_dev_obj, 
                                                             &rfc_dynamic_params.supports_multi_rat ) )
        {
          tmp_status = FALSE;
          RF_MSG( RF_ERROR, "RFC ERROR. CMN RF device rat support API returned false. Cannot generate RFC dynamic properties." );
        }

        // Get TRX capability
        rfc_dynamic_params.trx_capability = rfc_cmn->rfc_get_device_capability( cfg->logical_device );
        // Get max bandwidth
        switch ( rfc_dynamic_params.trx_capability )
        {
        case RFM_DEVICE_RX_SUPPORTED:
          if( rfdevice_lte_rx_get_rx_intra_ca_support( cfg->logical_device, band_u.lte_band, &ca_info ) )
          {
            // Get Rx max bandwidth
            rfc_dynamic_params.max_bw_khz = rflte_util_get_bw_in_khz( ca_info.max_bw_supported );
          }
          else
          {
            tmp_status = FALSE;
            rfc_dynamic_params.max_bw_khz = 0;
            RF_MSG_2( RF_ERROR, "RFC ERROR. LTE RX device API returned false for dev %d band %d. Cannot generate RFC dynamic properties.",
                      cfg->logical_device, band_u.lte_band );
          }

          /* Check to see if device supports iRAT for band, if device is non-preferred device:
              - if the band is supported over ICI (for interband CA) on the device, then iRAT
                cannot be supported on the device
           */
          if ( rfdevice_lte_rx_get_ici_path_info( cfg->logical_device, band_u.lte_band, &path_info ) )
          {
            rfc_dynamic_params.restrict_irat_on_non_preferred_dev = path_info.is_ici_path;            
          }
          else
          {
            rfc_dynamic_params.restrict_irat_on_non_preferred_dev = FALSE;
          }
          break;

        case RFM_DEVICE_TX_SUPPORTED:
          // Get Tx max bandwidth
          if( !rfdevice_lte_tx_get_max_bw_supported( cfg->logical_device, band_u.lte_band, &rfc_dynamic_params.max_bw_khz ) )
          {
            tmp_status = FALSE;
            rfc_dynamic_params.max_bw_khz = 0;
            RF_MSG_2( RF_ERROR, "RFC ERROR. LTE TX device API returned false for dev %d band %d. Cannot generate RFC dynamic properties.",
                      cfg->logical_device, band_u.lte_band );
          }
          break;

        default:
          tmp_status = FALSE;
          RF_MSG_2( RF_ERROR, "RFC ERROR. Device %d TRX capability %d not supported. Cannot generate RFC dynamic properties.",
                    cfg->logical_device, rfc_dynamic_params.trx_capability );
          break;
        }
        
        if (tmp_status == TRUE)
        {
          // Generate RFC dynamic properties
          rfc_dynamic_params.device = cfg->logical_device;
          rfc_dynamic_params.dev_instance = (uint8) instance;
          rfc_dynamic_params.sys_band = sys_band;
          rfc_dynamic_params.antenna = ant_num;
          (void) rfc_cmn->generate_rfcard_dynamic_properties( &rfc_dynamic_params );         
        }
        else
        {
          RF_MSG( RF_ERROR, "RFC detected ERROR. See preceeding errors. Cannot generate RFC dynamic properties." );
        }

        /////////////////// END RFC DYNAMIC PROPERTIES SECTION ///////////////////

      break; /* end case RFDEVICE_TRANSCEIVER */

      case RFDEVICE_ASM:
        if (asm_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
        {
        lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][asm_dev_instance] =\
          cmn_dev_obj;
        asm_dev_instance++;
        band_u.lte_band = (rfcom_lte_band_type)cfg->band;

        if(cfg->rx_tx == RFC_CONFIG_RX)
        {
            /* Update the Rx ASM Port state with temp_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_rx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               temp_port_state);

          ((rfdevice_asm *)cmn_dev_obj)->set_rx_band_map
                                         (
                                           cfg->logical_device,
                                           RFCOM_LTE_MODE, 
                                           band_u, 
                                           device_info_ptr->rf_asic_info[i].data, 
                                           RFC_ASIC_INFO_DATA_SIZE
                                         );

            /* Update the Tx ASM Port state with default_port_state and Call the ASM API below */
           ((rfdevice_asm *)cmn_dev_obj)->set_rx_switch_state(RFCOM_LTE_MODE,
                                                              band_u,
                                                              default_port_state);
        }
        else if (cfg->rx_tx == RFC_CONFIG_TX)
        {
            /* Update the Rx ASM Port state with temp_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_tx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               temp_port_state);
          ((rfdevice_asm *)cmn_dev_obj)->set_tx_band_map
                                         (
                                           cfg->logical_device,
                                           RFCOM_LTE_MODE, 
                                           band_u, 
                                           device_info_ptr->rf_asic_info[i].data, 
                                           RFC_ASIC_INFO_DATA_SIZE
                                         );

             /* Update the Tx ASM Port state with default_port_state and Call the ASM API below */
           ((rfdevice_asm *)cmn_dev_obj)->set_tx_switch_state(RFCOM_LTE_MODE,
                                                              band_u,
                                                              default_port_state);
        }
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"ASM device instance overflow asm_dev_instance:"
                     " %d", asm_dev_instance);
        }
        break;
        

       case RFDEVICE_PA:
        /*set band map is only for QPA. for HCPA and sattelite PA this should not affect.
        The set_band_map() is empty for rffe based PA.*/
        if(cfg->rx_tx == RFC_CONFIG_TX)
        {
        if (pa_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
        {
          lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][pa_dev_instance] =\
            cmn_dev_obj;
          pa_dev_instance++;
          band_u.lte_band = (rfcom_lte_band_type)cfg->band;
          ((rfdevice_pa *)cmn_dev_obj)->set_band_map
                                         (
                                           RFCOM_LTE_MODE, 
                                           band_u, 
                                           device_info_ptr->rf_asic_info[i].data, 
                                           RFC_ASIC_INFO_DATA_SIZE
                                         );
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"PA device instance overflow pa_dev_instance:"
                     " %d", pa_dev_instance);
        }
        }
        break;
      case RFDEVICE_PAPM:
        if(cfg->rx_tx == RFC_CONFIG_TX)
        {
          if (papm_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
          {
          lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][papm_dev_instance] =\
            cmn_dev_obj;  
          band_u.lte_band = (rfcom_lte_band_type)cfg->band;
          if (cmn_dev_obj != NULL)
          {
            ((rfdevice_papm *)cmn_dev_obj)->set_band_map( cfg->logical_device,
                                                          RFCOM_LTE_MODE, 
                                                          band_u, 
                                                          device_info_ptr->rf_asic_info[i].data, 
                                                          RFC_ASIC_INFO_DATA_SIZE);
          }
                                        
          papm_dev_instance++;
          }
          else
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"PAPM device instance overflow papm_dev_instance:"
                       " %d", papm_dev_instance);
          }
        }
        break;

      case RFDEVICE_XSW:
          /* Confirm we were actually able to obtain our cmn_dev_obj */
        if (cmn_dev_obj == NULL)
        {
           RF_MSG_1(RF_FATAL, "Unable to get CMN RF Device Object!", 0); 
        }
        else
        {
          lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][xsw_dev_instance] = cmn_dev_obj; 
          band_u.lte_band = (rfcom_lte_band_type)cfg->band;

          if (cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1)
          {

            ((rfdevice_xsw *)cmn_dev_obj)->set_band_map_swap(RFCOM_LTE_MODE, band_u, 
                                                             device_info_ptr->rf_asic_info[i].data, 
                                                             RFC_ASIC_INFO_DATA_SIZE);
            ((rfdevice_xsw *)cmn_dev_obj)->set_srs_band_map_swap(RFCOM_LTE_MODE, band_u, 
                                                                 device_info_ptr->rf_asic_info[i].data, 
                                                                 RFC_ASIC_INFO_DATA_SIZE);
          }/* !RFC_CONFIG_RX */
          else if (cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1)
          {

             ((rfdevice_xsw *)cmn_dev_obj)->set_band_map_default(RFCOM_LTE_MODE, band_u, 
                                                                 device_info_ptr->rf_asic_info[i].data, 
                                                                 RFC_ASIC_INFO_DATA_SIZE);
             ((rfdevice_xsw *)cmn_dev_obj)->set_srs_band_map_default(RFCOM_LTE_MODE, band_u, 
                                                                     device_info_ptr->rf_asic_info[i].data, 
                                                                     RFC_ASIC_INFO_DATA_SIZE);
          }/* !RFC_CONFIG_TX */ 
          else
          {
            RF_MSG_1(RF_ERROR, "Invalid configuration provided in cfg->req", 0);
          }
        } /* end IF cmn_dev_obj==NULL */         
        break; /* end case RFDEVICE_XSW */

      case RFDEVICE_TUNER:
        if ( (cfg->rx_tx == RFC_CONFIG_TX) || (cfg->rx_tx == RFC_CONFIG_RX) )
        {
          if (tuner_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
          {
            /*** Get Tuner Device ***/
            lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][tuner_dev_instance] = cmn_dev_obj; 
            band_u.lte_band = (rfcom_lte_band_type)cfg->band;
            if ((cmn_dev_obj != NULL) && (cfg->rx_tx == RFC_CONFIG_TX))
            {
              /*** Set distortion configuration ***/
              ((rfdevice_antenna_tuner *)cmn_dev_obj)->set_band_distortion_config_map(RFCOM_LTE_MODE, band_u, 
                 device_info_ptr->rf_asic_info[i].data);
            }

            /*** Get Tuner manager ***/
            /*Grab the associated Tuner manager instance, for now there is one to one mapping between
               a tuner device and tuner manager*/
            cmn_dev_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TUNER_MANAGER, (uint8)instance);

            /* Make sure there is a valid Antenna tuner manager */
            if (cmn_dev_obj != NULL)
            {
              lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][RFDEVICE_TUNER_MANAGER][tuner_dev_instance] = cmn_dev_obj;              }
            tuner_dev_instance++;
          }
          else
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Tuner device instance overflow tuner_dev_instance:"
                       " %d", tuner_dev_instance);
          }
        }
        break;

       case RFDEVICE_HDET:
          if (cfg->rx_tx == RFC_CONFIG_TX)
          {
             if (hdet_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
             {
                lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][hdet_dev_instance] =\
                   cmn_dev_obj;
                hdet_dev_instance++;
             }
             else
             {
                MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "HDET device instance overflow hdet_dev_instance:"
                      " %d", hdet_dev_instance);
             }
          }

          break; 
 
      case RFDEVICE_COUPLER:
          if (cfg->rx_tx == RFC_CONFIG_TX)
          {
             if (coupler_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
             {
               lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][coupler_dev_instance] =\
                  cmn_dev_obj;
               band_u.lte_band = (rfcom_lte_band_type)cfg->band;
               if ((cmn_dev_obj != NULL) && (cfg->rx_tx == RFC_CONFIG_TX))
               {
                 /*** Set distortion configuration ***/
                 ((rfdevice_coupler *)cmn_dev_obj)->set_band_config_map(RFCOM_LTE_MODE, band_u, 
                  device_info_ptr->rf_asic_info[i].data);
               }
               coupler_dev_instance++;
             }
             else
             {
                MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "COUPLER device instance overflow coupler_dev_instance:"
                      " %d", coupler_dev_instance);
             }
          }

          break; 

    case RFDEVICE_LNA:
      if (lna_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
      {
      lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][lna_dev_instance] =\
        cmn_dev_obj;
      lna_dev_instance++;
      band_u.lte_band = (rfcom_lte_band_type)cfg->band;
          /* Update the Rx ASM Port state with temp_port_state and Call the ASM API below */
      lna_cfg_c.band = band_u;
      lna_cfg_c.mode = RFCOM_LTE_MODE;
      lna_cfg_c.rfm_device = cfg->logical_device;
      lna_cfg_c.trigger_type = RFDEVICE_LNA_TRIGGER_NONE;
        ((rfdevice_lna *)cmn_dev_obj)->set_band_map
                                       (
                                        &lna_cfg_c,
                                        device_info_ptr->rf_asic_info[i].data,
                                        RFC_ASIC_INFO_DATA_SIZE
                                       );
      }
      break;

      default:
        break;
    }

    i++;
  }

  /* Final Check: If there are no PAs specified in RFC (as is the current
     case for GRFC PA) then append a generic GRFC PA
  */
#ifndef T_RUMI_EMULATION
  if( (cfg->rx_tx == RFC_CONFIG_TX)&& 
      (pa_dev_instance == 0)&& 
      (rfc_data != NULL)&& 
      (cfg->req == RFC_REQ_DEFAULT_GET_DATA)) 
  {
    /* Create grfc_pa instance if it has not been created */
    if(rfc_lte_common_grfc_pa_device == NULL)
    {
      rfc_lte_common_grfc_pa_device = new grfc_pa();
    }

    if(rfc_lte_common_grfc_pa_device != NULL)
    {
      /* If grfc_pa device is valid then program sig info (band_map) */      
      /* get band sig data */
      rfc_data->sig_cfg_data_get(cfg, &mode_rf_signals);
      
      band_u.lte_band = (rfcom_lte_band_type)cfg->band;
           
      /* program band map */
      ((rfdevice_pa*)rfc_lte_common_grfc_pa_device)->set_band_map( RFM_LTE_MODE, 
                                                                   band_u, 
                                                                   (int32*) mode_rf_signals, 
                                                                   0 /* Don't care */ );

      /* Assign grfc_pa instance to table */
      lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][RFDEVICE_PA][pa_dev_instance] =\
            (void*)rfc_lte_common_grfc_pa_device;
          pa_dev_instance++;
    }
    else
    {
      /* Return FALSE if we failed to create the PA instance. */
      RF_MSG_1(RF_FATAL,"Unable to create GRFC PA instance!", 0);
    }
  } /* end if for adding GRFC PA device */
#endif /* T_RUMI_EMULATION */


  ant_params[cfg->logical_device][cfg->rx_tx][cfg->band].antenna_num = ant_num ;

  /* If there is a Tuner device in the rf-card, we need make sure that there is at-least 
        one Tuner Instance for each Antenna Path. This is required by the Tuner design and also by the ASDiv design.
        Based on the state(Rx/Tx/Sleep) of the antenna path, other tuner devices need to be configured.
        So, We need to create a Dummy Tuner Device Object. 
    */
         
  /* If the tuner device is NOT present for this band */
  if (tuner_dev_instance == 0 )
  {
    /* get the dummy Tuner object if needed. If there are no tuners in the rf-card, Dummy Tuner Mgr will not be created.*/
    rfcommon_atuner_manager_intf *tuner_mgr_obj = (rfcommon_atuner_manager_intf *)(rfc_cmn->get_dummy_tuner_mgr(ant_num)); 
    
    if (tuner_mgr_obj != NULL)
    {
      lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][RFDEVICE_TUNER_MANAGER][tuner_dev_instance] = tuner_mgr_obj;

#ifdef FEATURE_RF_ASDIV
      /* update the rfc_asdiv_config info table with RFC attributes */
      asd_tuner_mgr = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASD_TUNER_MANAGER, 0);

      if ( asd_tuner_mgr != NULL )
      {
        /*Update ASDIV only if dummy tuner needed for the given antenna */
        if (((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->is_dummy_tuner_needed(ant_num))
        {
         (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->update_rfc_asdiv_config_info_dtuner(
                                                                                        ant_num,   \
                                                                                        RFDEVICE_TUNER,  \
                                                                                        tuner_dev_instance);
         RF_MSG_2(RF_HIGH, "ASDIV: Dummy tuner for ant=%d, 0x%x", ant_num, tuner_mgr_obj );
      }
      }
      else
      {
         RF_MSG(RF_ERROR, "ASDIV: asd_tuner_mgr returns NULL ptr");
      }  
#endif/*FEATURE_RF_ASDIV*/
    }
  }


  }
  else if (cfg->port_state == RFC_PORT_STATE_B)
  {
    temp_port_state = RFC_PORT_STATE_B;

    while (i < device_info_ptr->num_devices)
    {
      dev_type = device_info_ptr->rf_asic_info[i].device_type;
      instance = device_info_ptr->rf_asic_info[i].instance;
      cmn_dev_obj = rfc_cmn->get_cmn_rf_device_object(dev_type, (uint8)instance);
      if (cmn_dev_obj == NULL)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"cmd dev obj is NULL %d %d", dev_type,instance);
        i++;
        continue;
      }

      switch(dev_type)
      {
      case RFDEVICE_ASM:
        if (asm_dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND)
        {
          lte_rf_devices[cfg->logical_device][cfg->rx_tx][cfg->band][dev_type][asm_dev_instance] =\
            cmn_dev_obj;
          asm_dev_instance++;
          band_u.lte_band = (rfcom_lte_band_type)cfg->band;

          if(cfg->rx_tx == RFC_CONFIG_RX)
          {
            /* Update the Rx ASM Port state with temp_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_rx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               temp_port_state);
            ((rfdevice_asm *)cmn_dev_obj)->set_rx_band_map
                                           (
                                             cfg->logical_device,
                                             RFCOM_LTE_MODE, 
                                             band_u, 
                                             device_info_ptr->rf_asic_info[i].data, 
                                             RFC_ASIC_INFO_DATA_SIZE
                                           );

            /* Update the Rx ASM Port state with default_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_rx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               default_port_state);
          }
          else if (cfg->rx_tx == RFC_CONFIG_TX)
          {
            /* Update the Tx ASM Port state with temp_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_tx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               temp_port_state);
            ((rfdevice_asm *)cmn_dev_obj)->set_tx_band_map
                                           (
                                             cfg->logical_device,
                                             RFCOM_LTE_MODE, 
                                             band_u, 
                                             device_info_ptr->rf_asic_info[i].data, 
                                             RFC_ASIC_INFO_DATA_SIZE
                                           );

            /* Update the Tx ASM Port state with default_port_state and Call the ASM API below */
            ((rfdevice_asm *)cmn_dev_obj)->set_tx_switch_state(RFCOM_LTE_MODE,
                                                               band_u,
                                                               default_port_state);
          }
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"ASM device instance overflow asm_dev_instance:"
                     " %d", asm_dev_instance);
        }
        break;

      default:
        break;
      }
      i++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function initializes the rfdevices with the rfc dependent data.

  @details
  This function initializes the rfdevices with the rfc dependent data.

*/
/*Featurize the code for now on TA*/
#ifdef FEATURE_TABASCO_MODEM
#define REQ_ARRAY_SIZE 1
void rfc_lte_intf::init_rfdevices_for_all_bands(void)
{
  uint8 band;
  uint8 rfm_device=0;
  uint8 device = RFM_INVALID_DEVICE;
  rfc_cfg_params_type       cfg;
  rfc_device_info_type *device_info_ptr =  NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean skip_flag = FALSE;
  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

#ifdef FEATURE_RF_ASDIV
  rfdevice_class *cmn_dev_obj = NULL;
  rfdevice_class *asd_tuner_mgr = NULL;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  /* ASDiv requires to scan each band to determine the antenna pair configuration and what types of
     devices are on each antenna.  The antenna pairing needs to be known "prior" to the devices
     object assignment (per band) so that it can be determined if dummy device manager object is
     needed to be assigned or not.
   */
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH, "Processing asdiv_config info table for LTE...");

  asd_tuner_mgr = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASD_TUNER_MANAGER, 0);
  if ( asd_tuner_mgr != NULL )
  {
    cfg.alternate_path = 0;
    cfg.logical_device = RFM_DEVICE_0;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ )
    {
      cfg.band = band;
      cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      if (device_info_ptr != NULL)
      {
         (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->update_rfc_asdiv_config_info(&cfg, \
                                                                                          device_info_ptr);
      }

      cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      if (device_info_ptr != NULL)
      {
         (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->update_rfc_asdiv_config_info(&cfg, \
                                                                                          device_info_ptr);
      }
    }
  }
  else
  {
    RF_MSG(RF_ERROR, "ASDIV: asd_tuner_mgr returns NULL ptr");
  }
#endif /* FEATURE_RF_ASDIV */

  uint8 temp_log_device;
  uint32 temp_alt_path;
  uint8 temp_rx_tx;
  uint8 temp_req_data_arr[REQ_ARRAY_SIZE] = {RFC_REQ_DEFAULT_GET_DATA};
  uint8 temp_req_data;
  uint8 temp_port_state;

  for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++)
  {
    for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ )
    {
      for (temp_rx_tx = RFC_CONFIG_RX; temp_rx_tx < RFC_CONFIG_RXTX_MAX; temp_rx_tx++)
      {
        for (temp_req_data = 0; temp_req_data < REQ_ARRAY_SIZE; temp_req_data++)
        {
          skip_flag = FALSE;
          //Check TX supported on given band to reduce underlying loop iterations
           if ((temp_rx_tx == RFC_CONFIG_TX) && 
               (!rflte_nv_get_tx_band_support((rfcom_lte_band_type)band)) &&
               (temp_req_data != 0)) 
          {
            skip_flag = TRUE;
          }

           if (skip_flag == FALSE)
           {
            for (temp_port_state = RFC_PORT_STATE_A; temp_port_state < RFC_PORT_STATE_B; temp_port_state++)
            {
              cfg.logical_device = (rfm_device_enum_type)temp_log_device;
              cfg.band = band;
              cfg.rx_tx = (rfc_rxtx_enum_type)temp_rx_tx;
              cfg.req = (rfc_req_type)temp_req_data_arr[temp_req_data];
              cfg.port_state = (rfc_port_state_type)temp_port_state;

               /* RF_MSG_5(RF_HIGH, 
                          "init_rfdevices_for_all_bands: initializing devices for dev:%d, rx_tx:%d, band:%d, req:%d, port_state:%d ",
                          cfg.logical_device,
                          cfg.rx_tx,
                          cfg.band,
                          cfg.req,
                          cfg.port_state);*/

              for (temp_alt_path = 0; temp_alt_path < RFC_MAX_SUPPORTED_NUM_ALTERNATE_PATH; temp_alt_path++)
              {
                cfg.alternate_path = temp_alt_path;

              if(TRUE == rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr))
              {
                if (RFC_CONFIG_TX == temp_rx_tx) 
                {
                   MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, 
                         "TX_RX_Split: init_rfdevices_for_all_bands: Setting Tx dev bandmask for band %d", band);
                  rflte_util_set_tx_dev_band_mask((rfm_device_enum_type)temp_log_device, (rfcom_lte_band_type)band);
                }
                else if (RFC_CONFIG_RX == temp_rx_tx)
                {
                   MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, 
                         "TX_RX_Split: init_rfdevices_for_all_bands: Setting Rx dev bandmask for band %d", band);
                   rflte_util_set_rx_dev_band_mask((rfm_device_enum_type)temp_log_device, (rfcom_lte_band_type)band);
                }
                init_rf_devices(&cfg, device_info_ptr);
              } /* if (TRUE == rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr)) */
             } /*for (temp_alt_path = 0; temp_alt_path < RFC_MAX_SUPPORTED_NUM_ALTERNATE_PATH; temp_alt_path++) */
            } /* for (temp_port_state = RFC_PORT_STATE_A; temp_port_state < RFC_PORT_STATE_B; temp_port_state++) */
          } /* if (skip_flag == FALSE) */
        } /* for (temp_req_data = 0; temp_req_data < REQ_ARRAY_SIZE; temp_req_data++) */
      } /* for (temp_rx_tx = RFC_CONFIG_RX; temp_rx_tx < RFC_CONFIG_RXTX_MAX; temp_rx_tx++) */
    } /* for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ ) */
  } /* for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++) */

  for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++)
  {
    cfg.alternate_path = 0;
    cfg.logical_device = (rfm_device_enum_type)temp_log_device;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for (band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++)
    {
      cfg.band = band;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      init_rf_devices(&cfg, device_info_ptr);
    }

    cfg.alternate_path = 0;
    cfg.logical_device = (rfm_device_enum_type)temp_log_device;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for (band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++)
    {
      cfg.band = band;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      init_rf_devices(&cfg, device_info_ptr);
    }
  }
}
#else  /* FEATURE_TABASCO_MODEM */

#define REQ_ARRAY_SIZE 1
void rfc_lte_intf::init_rfdevices_for_all_bands(void)
{
  uint8 band;
  uint8 device = RFM_INVALID_DEVICE;
  rfc_cfg_params_type       cfg;
  rfc_device_info_type *device_info_ptr =  NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean skip_flag = FALSE;
  uint8 rfm_device=0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
    }

#ifdef FEATURE_RF_ASDIV
  rfdevice_class *cmn_dev_obj = NULL;
  rfdevice_class *asd_tuner_mgr = NULL;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  /* ASDiv requires to scan each band to determine the antenna pair configuration and what types of
     devices are on each antenna.  The antenna pairing needs to be known "prior" to the devices
     object assignment (per band) so that it can be determined if dummy device manager object is
     needed to be assigned or not.
   */
  MSG(MSG_SSID_RF, MSG_LEGACY_HIGH, "Processing asdiv_config info table for LTE...");

  asd_tuner_mgr = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASD_TUNER_MANAGER, 0);
  if ( asd_tuner_mgr != NULL )
  {
    cfg.alternate_path = 0;
    cfg.logical_device = RFM_DEVICE_0;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ )
    {
      cfg.band = band;
      cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      if (device_info_ptr != NULL)
      {
         (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->update_rfc_asdiv_config_info(&cfg, \
                                                                                          device_info_ptr);
      }

      cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      if (device_info_ptr != NULL)
      {
         (void)((rfcommon_asdiv_tuner_manager*)asd_tuner_mgr)->update_rfc_asdiv_config_info(&cfg, \
                                                                                          device_info_ptr);
      }
    }
  }
  else
  {
    RF_MSG(RF_ERROR, "ASDIV: asd_tuner_mgr returns NULL ptr");
  }
#endif /* FEATURE_RF_ASDIV */


  uint8 temp_log_device;
  uint32 temp_alt_path;
  uint8 temp_rx_tx;
  uint8 temp_req_data_arr[REQ_ARRAY_SIZE] = {RFC_REQ_DEFAULT_GET_DATA};
  uint8 temp_req_data;
  uint8 temp_port_state;

  for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++)
  {
    for (band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++)
    {
      for (temp_rx_tx = 0; temp_rx_tx < RFC_CONFIG_RXTX_MAX; temp_rx_tx++)
      {
        for (temp_req_data = 0; temp_req_data < REQ_ARRAY_SIZE; temp_req_data++)
        {
          skip_flag = FALSE;

          /* Check TX supported on given band 
           * If TX is not supported, skip subsequent loop iterations for TX
           */
          if ((temp_rx_tx == RFC_CONFIG_TX) &&
              (!rflte_nv_get_tx_band_support((rfcom_lte_band_type)band)) &&
              (temp_req_data != 0))
          {
            skip_flag = TRUE;
          }

          if (skip_flag == FALSE)
          {
            for (temp_port_state = RFC_PORT_STATE_A; temp_port_state < RFC_PORT_STATE_B; temp_port_state++)
            {
              cfg.alternate_path = 0;
              cfg.logical_device = (rfm_device_enum_type)temp_log_device;
              cfg.band = band;
              cfg.rx_tx = (rfc_rxtx_enum_type)temp_rx_tx;
              cfg.req = (rfc_req_type)temp_req_data_arr[temp_req_data];
              cfg.port_state = (rfc_port_state_type)temp_port_state;

              MSG_5(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH,
                       "init_rfdevices_for_all_bands: initializing devices for dev:%d, rx_tx:%d, band:%d, req:%d, port_state:%d ",
                       cfg.logical_device,
                       cfg.rx_tx,
                       cfg.band,
                       cfg.req,
                       cfg.port_state);

              if (TRUE == rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr))
              {
                /* 
                 * Update device specific band mask only for request type = RFC_REQ_DEFAULT_GET_DATA
                 * so that for other request types, the bit masks for TX and RX are not set redundantly 
                 */

                if (temp_req_data_arr[temp_req_data] == RFC_REQ_DEFAULT_GET_DATA)
                {

                  if (RFC_CONFIG_TX == temp_rx_tx)
                  {
                    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
                          "TX_RX_Split: init_rfdevices_for_all_bands: Setting Tx dev bandmask for band %d", band);
                    rflte_util_set_tx_dev_band_mask((rfm_device_enum_type)temp_log_device, (rfcom_lte_band_type)band);
                  } else if (RFC_CONFIG_RX == temp_rx_tx)
                  {
                    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
                          "TX_RX_Split: init_rfdevices_for_all_bands: Setting Rx dev bandmask for band %d", band);
                    rflte_util_set_rx_dev_band_mask((rfm_device_enum_type)temp_log_device, (rfcom_lte_band_type)band);
                  }
                } /* if (temp_req_data_arr[temp_req_data] == RFC_REQ_DEFAULT_GET_DATA) */

                init_rf_devices(&cfg, device_info_ptr);
              } /* if (TRUE == rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr)) */
            } /* for (temp_port_state = RFC_PORT_STATE_A; temp_port_state < RFC_PORT_STATE_B; temp_port_state++) */
          } /* if (skip_flag == FALSE) */
        } /* for (temp_req_data = 0; temp_req_data < REQ_ARRAY_SIZE; temp_req_data++) */
      } /* for (temp_rx_tx = RFC_CONFIG_RX; temp_rx_tx < RFC_CONFIG_RXTX_MAX; temp_rx_tx++) */
    } /* for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ ) */
  } /* for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++) */

  /* 
   * handle all other request types separately to avoid overwriting 
   * of device object pointers for unintentinted devices, bands, rx/tx 
   */
  for (temp_log_device = 0; temp_log_device < RFM_MAX_WAN_DEVICES; temp_log_device++)
  {
    cfg.alternate_path = 0;
    cfg.logical_device = (rfm_device_enum_type)temp_log_device;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for (band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++)
    {
      cfg.band = band;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      init_rf_devices(&cfg, device_info_ptr);
    }

#if (defined (FEATURE_THOR_MODEM))
/*!FBRx loop*/
    for(rfm_device=0; rfm_device < RFM_MAX_DEVICES; rfm_device++)
    {
      cfg.logical_device = (rfm_device_enum_type)rfm_device;
      cfg.rx_tx = RFC_CONFIG_TX;
      cfg.req = RFC_REQ_ENABLE_FBRX;
  
      for ( band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++ )
      {
         cfg.band = band;
         if(rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr)== TRUE)
         {
           if(!rfc_set_fbrx_mapping(       
                                      &cfg, 
                                      device_info_ptr
                                   )
             )
           {
                RF_MSG_3( RF_ERROR, " rfc_set_fbrx_mapping() failed for rfm device %d. "
                                    "May not be supported for this tech %d band %d. check rfc",
                                    rfm_device,RFCOM_LTE_MODE,band);
           } 
         }
      }
    }
#endif
    cfg.alternate_path = 0;
    cfg.logical_device = (rfm_device_enum_type)temp_log_device;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    cfg.port_state = RFC_PORT_STATE_A; // By default we should configure Port A settings.

    for (band = RFCOM_BAND_LTE_B1; band < RFCOM_NUM_LTE_BANDS; band++)
    {
      cfg.band = band;
      rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);
      init_rf_devices(&cfg, device_info_ptr);
    }
  }
}
#endif /* FEATURE_TABASCO_MODEM */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function obtains GRFC signal information and configures them

  @details
  This function obtains GRFC signal information of switches and configures them to setup
  feedback path for XPT calibration

*/
boolean rfc_lte_intf::setup_fb_path(rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable)
{
  rfc_cfg_params_type cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_lte_data_ptr = NULL;

  cfg.alternate_path = 0;    
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device_fb;
  cfg.req = et_enable?RFC_REQ_ENABLE_XPT_CAPTURE:RFC_REQ_DISABLE_XPT_CAPTURE;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();
  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return(FALSE);
  }
  rfc_lte_data_ptr->sig_cfg_data_get(&cfg, &signal_list_ptr);

  if (signal_list_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE signal_list_ptr is NULL", 0);
    return(FALSE);
  }

  rfc_common_config_signal(signal_list_ptr, rfcommon_get_gnss_blanking_is_enabled( RFCOM_LTE_MODE, band ) );
 
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function obtains GRFC signal information (no immediate configure)

  @details
  This function obtains GRFC signal information of switches to setup
  feedback path for FBRx or XPT calibration

*/
void *rfc_lte_intf::get_fb_path_config(rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable)
{
  rfc_cfg_params_type cfg;
  rfc_sig_cfg_type       *signal_list_ptr = NULL;
  rfc_lte_data *rfc_lte_data_ptr = NULL;

  cfg.alternate_path = 0;    
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device_fb;
  cfg.req = et_enable?RFC_REQ_ENABLE_FBRX:RFC_REQ_DISABLE_FBRX;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();
  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return NULL;
  }
  rfc_lte_data_ptr->sig_cfg_data_get(&cfg, &signal_list_ptr);

  if (signal_list_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE signal_list_ptr is NULL", 0);
    return NULL;
  }

  return( (void*)signal_list_ptr);

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function obtains The Rx Device to be used as Feedback Device based on Modem Chain (no immediate configure)

  @details
  This function obtains The Rx Device to be used as Feedback Device based on Modem Chain.
  Feedback Device for FBRx or XPT calibration

*/
rfm_device_enum_type rfc_lte_intf::get_fb_device(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band, rfcom_lte_band_type rx_band)
{
  rfc_cfg_params_type cfg;
  rfc_lte_data *rfc_lte_data_ptr = NULL;
  rfc_device_info_type *device_info_ptr = NULL;
  uint32 modem_chain = 4; //Default chain for FBRX on THOR
  rfm_device_enum_type fb_device;
  boolean fb_device_found = FALSE;

  cfg.alternate_path = 0;    
  cfg.band = (int)tx_band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = tx_device;
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();
  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return RFM_INVALID_DEVICE;
  }

  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

  if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_fb_device: RFC LTE device_info_ptr for Tx is NULL", 0);
    return RFM_INVALID_DEVICE;
  }

  modem_chain = device_info_ptr->modem_chain;

  for (fb_device = RFM_DEVICE_0 ; fb_device < RFM_MAX_WAN_DEVICES ; fb_device = (rfm_device_enum_type)((int)fb_device + 1))
  {
    cfg.alternate_path = 0;    
    cfg.band = (int)rx_band;
    cfg.rx_tx = RFC_CONFIG_RX;
    cfg.logical_device = fb_device;
    cfg.req = RFC_REQ_DEFAULT_GET_DATA;

	rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

	if (device_info_ptr != NULL)
    {
      if (modem_chain == device_info_ptr->modem_chain)
      {
        fb_device_found = TRUE;
		break;
      }
    }
  }

  if (fb_device_found == TRUE)
  {
    return fb_device;
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
		  "get_fb_device: No match found in RFC. Returning FB Device as Tx Device : %d", tx_device);
	
    return tx_device;
  }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function obtains the FB Modem Chain from RFC for a given Tx device

  @details
  This function obtains the Feedback Modem Chain based on RFC.
  Feedback Chain for FBRx or XPT calibration

*/
uint32 rfc_lte_intf::get_fb_chain(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band)
{
  rfc_cfg_params_type cfg;
  rfc_lte_data *rfc_lte_data_ptr = NULL;
  rfc_device_info_type *device_info_ptr = NULL;
  uint32 modem_chain = 3; //Default chain for FBRX on Jacala

  cfg.alternate_path = 0;    
  cfg.band = (int)tx_band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = tx_device;
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();
  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return RFM_INVALID_DEVICE;
  }

  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

  if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_fb_chain: RFC LTE device_info_ptr for Tx is NULL", 0);
    return RFM_INVALID_DEVICE;
  }

  modem_chain = device_info_ptr->modem_chain;
	
  return modem_chain;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function obtains device information from RFC and configures them

  @details
  This function obtains feedback device information from RFC and configures for XPT calibration

*/
boolean rfc_lte_intf::setup_fb_device(rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_freq)
{
  rfc_cfg_params_type cfg;
  rfc_intf *rfc_cmn = NULL;
  rfdevice_class *trx_device = NULL;
  rfdevice_rxtx_common_class* trx_cmn_device = NULL;  
  rfc_device_info_type *device_info_ptr = NULL;
  rfc_lte_data *rfc_lte_data_ptr = NULL;
  rfdevice_trx_lte_tx *tx_device = NULL;  
  rfdevice_lte_script_data_type script_data = {TRUE,NULL};  
  uint32 temp_instance;
  boolean set_path = TRUE;
  rfc_cmn_properties_type *rfc_cmn_properties_ptr = NULL;
  rfdevice_rxtx_common_class* device_ptr = NULL;
  
  cfg.alternate_path = 0;
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();

  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE Data Object is NOT Created", 0);
    return(FALSE);
  }
  
  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

    if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE device_info_ptr for Tx is NULL", 0);
    return(FALSE);
  }
    
  temp_instance =device_info_ptr->rf_asic_info[0].instance;

  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device; //Use Tx Device
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

    if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE device_info_ptr for Rx is NULL", 0);
    return(FALSE);
  }
      
  rfc_cmn = rfc_intf::get_instance();  
  trx_device= rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER, device_info_ptr->rf_asic_info[0].instance);
  if( trx_device != NULL )
  {
    /* extract the lte device object from the cmn_device object and store it */ 
    trx_cmn_device = (rfdevice_rxtx_common_class *)trx_device;
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: trx_cmn_device not created", 0);
    return(FALSE);
  }
      
      
  if (temp_instance !=device_info_ptr->rf_asic_info[0].instance)
  {  
    tx_device = (rfdevice_trx_lte_tx*)(((rfdevice_rxtx_common_class *)trx_device)\
                                       ->get_tech_instance
                                       (
                                                                    RFM_LTE_MODE, 
                                         RFDEVICE_TX,
                                         RFDEVICE_TRX_PATH_0
                                        )
                                       );
  
    if (NULL != tx_device)
    {
      rfdevice_lte_tx_tune_to_default_port( tx_device,
                                            band, 
                                            (uint32)tx_freq,
                                             0, 
                                            RFDEVICE_DPD_DISABLED,
                                            &script_data, 
                                            TRUE, 
                                            (rfm_device_enum_type)((uint16)device_fb-1));
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: tx device pointer is NULL!", 0 ); 
      return(FALSE);
    }
  }

  /* get the feedback path attenuation state from rfc   */
  set_path &= rfc_get_cmn_device_properties(&rfc_cmn_properties_ptr);

  if(rfc_cmn_properties_ptr != NULL)
  {
    /* Turn on feedback path */
    rfdevice_cmn_set_et_path(trx_cmn_device,
                             RFM_LTE_MODE, 
                             (rfm_fb_attenuation_state_type)rfc_cmn_properties_ptr->rfc_attenuation_state,
                             TRUE);
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"setup_fb_device: Set et path to attenuation state %d", rfc_cmn_properties_ptr->rfc_attenuation_state);        
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: rfc_cmn_properties pointer is NULL", 0 ); 
    return(FALSE);
  }

  /* Get common device object for new device architecture */
  device_ptr = rfdevice_lte_tx_get_common_dev_ptr(device, band);

  if (NULL != device_ptr)
  {
    /* Set feedback path attenuation to gain state 2 and enable */
    set_path &= rfcommon_fbrx_mc_enable_rf_dev(device_ptr, RFLM_FBRX_GAIN_STATE_3);
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: common device pointer is NULL", 0 ); 
    return(FALSE);
  }
  
  return (set_path);
}  


boolean rfc_lte_intf::disable_fb_device(rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_freq)
{
  rfc_cfg_params_type cfg;
  rfc_intf *rfc_cmn = NULL;
  rfdevice_class *trx_device = NULL;
  rfdevice_rxtx_common_class* trx_cmn_device = NULL;  
  rfc_device_info_type *device_info_ptr = NULL;
  rfc_lte_data *rfc_lte_data_ptr = NULL;
  rfdevice_trx_lte_tx *tx_device = NULL;  
  rfdevice_lte_script_data_type script_data = {TRUE,NULL};  
  uint32 temp_instance;
  boolean set_path = TRUE;
  rfc_cmn_properties_type *rfc_cmn_properties_ptr = NULL;
  rfdevice_rxtx_common_class* device_ptr = NULL;
  
  cfg.alternate_path = 0;
  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;

  rfc_lte_data_ptr = rfc_lte_data::get_instance();

  if (rfc_lte_data_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE Data Object is NOT Created", 0);
    return(FALSE);
  }
  
  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

  if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE device_info_ptr for Tx is NULL", 0);
    return(FALSE);
  }
    
  temp_instance =device_info_ptr->rf_asic_info[0].instance;

  cfg.band = (int)band;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.logical_device = device; //Use Tx Device
  cfg.req = RFC_REQ_ENABLE_XPT_CAPTURE;

  rfc_lte_data_ptr->devices_cfg_data_get(&cfg, &device_info_ptr);

  if (device_info_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: RFC LTE device_info_ptr for Rx is NULL", 0);
    return(FALSE);
  }
      
  rfc_cmn = rfc_intf::get_instance();  
  trx_device= rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER, device_info_ptr->rf_asic_info[0].instance);
  if( trx_device != NULL )
  {
    /* extract the lte device object from the cmn_device object and store it */ 
    trx_cmn_device = (rfdevice_rxtx_common_class *)trx_device;
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: trx_cmn_device not created", 0);
    return(FALSE);
  }
      
      
  if (temp_instance !=device_info_ptr->rf_asic_info[0].instance)
  {  
    tx_device = (rfdevice_trx_lte_tx*)(((rfdevice_rxtx_common_class *)trx_device)\
                                       ->get_tech_instance
                                       (
                                                                    RFM_LTE_MODE, 
                                         RFDEVICE_TX,
                                         RFDEVICE_TRX_PATH_0
                                        )
                                       );
  
    if (NULL != tx_device)
    {
      rfdevice_lte_tx_tune_to_default_port( tx_device,
                                            band, 
                                            (uint32)tx_freq,
                                             0, 
                                            RFDEVICE_DPD_DISABLED,
                                            &script_data, 
                                            TRUE, 
                                            (rfm_device_enum_type)((uint16)device_fb-1));
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: tx device pointer is NULL!", 0 ); 
      return(FALSE);
    }
  }

  /* Get common device object for new device architecture */
  device_ptr = rfdevice_lte_tx_get_common_dev_ptr(device, band);

  if (NULL != device_ptr)
  {
    /* Set feedback path attenuation to gain state 2 and enable */
    set_path &= rfcommon_fbrx_mc_disable_rf_dev(device_ptr);
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"setup_fb_device: common device pointer is NULL", 0 ); 
    return(FALSE);
  }
  
  return (set_path);
}  

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.
 
   @param
   *split_band: pointer to boolean indicating whether it was a split band
   rx_chan: DL channel number
   *rfc_band: band for which data is desired

   @details

*/
void rfc_lte_intf::get_rx_band(boolean *split_band, rfcom_lte_earfcn_type rx_chan, 
                               rfcom_lte_band_type *rfc_band)
{
  
  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  int i = 0;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;
  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(*rfc_band, &temp_logical_device, RFC_CONFIG_RX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_rx_band, failed to get supported device for band %d ", *rfc_band);
    return;
  }
  // Setup configuration parameters
  cfg.alternate_path = 0;
  cfg.logical_device = temp_logical_device;
  cfg.band = (int)*rfc_band;
  cfg.rx_tx = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
  if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
  {
    *split_band = TRUE;
    /* Loop through channels to find true band */
      while ((unsigned int)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
      {
        if (((uint32)rx_chan >= signal_list_ptr[0].band_info[i].start_chan) && ((uint32)rx_chan <= signal_list_ptr[0].band_info[i].stop_chan))
        {
          *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
          break;
        }
        i++;
      }    
  } 
  else
  {
    /* Do Nothing, probably not a split band */
  }
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the tx band info. for the specified tx channel by querying RFC.
 
   @param
   *split_band: pointer to boolean indicating whether it was a split band
   tx_chan: UL channel number
   *rfc_band: band for which data is desired

   @details

*/
void rfc_lte_intf::get_tx_band(boolean *split_band, rfcom_lte_earfcn_type tx_chan, 
                               rfcom_lte_band_type *rfc_band,boolean filtered_band)
{
  
  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  int i = 0;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(*rfc_band, &temp_logical_device, RFC_CONFIG_TX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_tx_band, failed to get supported device for band %d ", *rfc_band);
    return;
  }

  // Setup configuration parameters
  cfg.alternate_path = 0;
  cfg.logical_device = temp_logical_device;
  cfg.band = (int)*rfc_band;
  cfg.rx_tx = RFC_CONFIG_TX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
 if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
 {
   *split_band = TRUE;
   /* Loop through channels to find true band */
     while ((unsigned int)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
     {
       if (filtered_band)
       {
          if (signal_list_ptr[0].band_info[i].status == RFC_FILTERED_PATH_TRUE)
          {
             *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
             break;
          }
       }
       else if (((uint32)tx_chan >= signal_list_ptr[0].band_info[i].start_chan) && ((uint32)tx_chan <= signal_list_ptr[0].band_info[i].stop_chan))
       {
         *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
         break;
       }
       i++;
     }
   
 } 
}

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  API to update the init_rf_devices depending on cfg parameters passed. 
 
  @details
  it will update the init_rf_devices for tx device with different path overide.
 
  @param rfm_dev
  current logical device which needs port reconfiguration
 
  @param curr_band
  current rf band involved in path swapping or port selection 
 
  @param alt_path
  preferred alternate path to use for a specific logical device & band combo
 
  @return status
  TRUE if a successful match is found in RFC AG and rf port info update is
  complete, else return FALSE 
*/
boolean rfc_lte_intf::get_updated_tx_port
(
  rfm_device_enum_type rfm_dev, 
  rfcom_lte_band_type curr_band, 
  uint8 alt_path
)
{
  
  rfc_cfg_params_type  cfg;
  rfc_device_info_type *device_info_ptr;
  boolean status = TRUE;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"get_updated_tx_port:RFC LTE Data Object is NOT Created", 0);
    status = FALSE;
  }

  if(status == TRUE)
  {    
    /* Initialize the config params structure */
    cfg.logical_device = rfm_dev;
    cfg.band           = (uint8)curr_band; 
    cfg.rx_tx          = RFC_CONFIG_TX;
    cfg.req            = RFC_REQ_DEFAULT_GET_DATA;
    cfg.port_state        = RFC_PORT_STATE_A;
    cfg.alternate_path = alt_path; 
  
    /* Get the column specific to a band, logical device & alt path */
    status = rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr); 
  } 

  if(status == TRUE)
  {
    /* Update the band port enumeration for all devices in that logical path */
    init_rf_devices(&cfg, device_info_ptr); 
  }

  return status; 
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the tx band info. for the specified tx channel by querying RFC.
 
   @param
   split_band: band for which low and high channels are desired
   *low_earfcn: Pointer to low chan
   *high_earfcn: Pointer to high chan

   @details

*/
void rfc_lte_intf::get_tx_hi_lo_earfcn(rfcom_lte_band_type split_band, 
                                     rfcom_lte_earfcn_type *low_earfcn,
                                     rfcom_lte_earfcn_type *high_earfcn)
{

  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  int i = 0;
  /* These params help to detect the largest range for a given split band */
  boolean largest_range_detected = FALSE;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;


  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }


  if(!rfc_lte_get_band_supported_device(split_band, &temp_logical_device, RFC_CONFIG_TX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_tx_hi_lo_earfcn, failed to get supported device for band %d ", split_band);
    return;
  }

  // Setup configuration parameters
  cfg.alternate_path = 0;
  cfg.logical_device = temp_logical_device;
  cfg.band = split_band;
  cfg.rx_tx = RFC_CONFIG_TX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
 
  if ((TRUE == band_split_ret_val) && (NULL != signal_list_ptr))
  {
   /* Loop through channels to find true band and the largest channel range */
    while ((uint32)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
    {
      if (signal_list_ptr[0].band_info[i].band == split_band)
      {
          if (FALSE == largest_range_detected)
          {
              *low_earfcn = signal_list_ptr[0].band_info[i].start_chan;
              *high_earfcn = signal_list_ptr[0].band_info[i].stop_chan;
              largest_range_detected = TRUE;
          }
          else
          {
              if (*low_earfcn > signal_list_ptr[0].band_info[i].start_chan)
              {
                  *low_earfcn = signal_list_ptr[0].band_info[i].start_chan;
              }
              if (*high_earfcn < signal_list_ptr[0].band_info[i].stop_chan)
              {
                  *high_earfcn = signal_list_ptr[0].band_info[i].stop_chan;
              }
          }
      }
      i++;
    }
  }
}

rfm_device_enum_type rfc_lte_intf::get_mapped_device_for_scell 
(
 rfcom_lte_band_type pcell_band,
 rfcom_lte_band_type scell_band,
 rfm_device_enum_type scell_rfm_device
)
{
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfc_band_pair_list_type *band_pair_lists = NULL;

  rfc_data->ca_scell_log_path_swap_data_get(&band_pair_lists);
  uint8 band_pair_list_idx = 0;

  if (band_pair_lists != NULL)
  {
    while (band_pair_lists->band_pair_list[band_pair_list_idx].band1 != RFCOM_NUM_LTE_BANDS)
    {
      if (band_pair_lists->band_pair_list[band_pair_list_idx].band1 == pcell_band &&
          band_pair_lists->band_pair_list[band_pair_list_idx].band2 == scell_band)
      {
        MSG_3(MSG_SSID_RF, MSG_LEGACY_LOW,"rfc_lte_intf::get_mapped_device_for_scell() swapping "
              " devices pcell band: %d scell band: %d scell_rfm_device:%d", pcell_band, scell_band, scell_rfm_device);
        /* Dynamic SCELL Swap not supported for SCC2 right now. Need to be designed; if necessary. */
        if (scell_rfm_device == RFM_DEVICE_2) 
        {
          return (RFM_DEVICE_0);
        } 
        /* Dynamic SCELL Swap not supported for SCC2 right now. Need to be designed; if necessary. */
        else if (scell_rfm_device == RFM_DEVICE_3)
        {
          return (RFM_DEVICE_1);
        }
        else
        {
         MSG_1(MSG_SSID_RF, MSG_LEGACY_LOW,"rfc_lte_intf::get_mapped_device_for_scell() "
                                      " no swapping done scell device passed: %d", scell_rfm_device);
         return (scell_rfm_device);
        }
        break;
      }
      band_pair_list_idx++;
    }
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_LOW,"rfc_lte_intf::get_mapped_device_for_scell() table"
                                      " not available pcell band: %d scell band: d", pcell_band, scell_band);
  }
  return (scell_rfm_device);
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   API to insert candidate_band in band_list.
   If candidate_band is not in band_list => inserts it in band_list along with associated_list_index
   If candidate_band is already present in band list => Adds associated_list_index

   @details
   API to insert candidate_band in band_list.
   If candidate_band is not in band_list => inserts it in band_list along with associated_list_index
   If candidate_band is already present in band list => Adds associated_list_index

   @parameter
   @param
   band_list: List of Bands, Associated LIST Indices
   @param 
   band_list_size: Current band_list_size
   @param 
   candidate_band: Candidate Band
   @param
   associated_list_index : Associated LIST index
   @return
   TRUE if successfully able to insert candidate_band or associated_list_index. FALSE if ran out of space RFA_RF_LTE_MAX_CELL_SUPPORTED
*/
boolean rfc_lte_intf::candidate_band_insert(rflte_mc_band_index_list_s_type* band_list,uint32* band_list_size,rfcom_lte_band_type candidate_band,uint32 associated_list_index)
{
   uint32 band_loop;
   boolean unique_band = TRUE;

   //Find if already present band or new band
   for (band_loop = 0; band_loop < (*band_list_size); band_loop++)
   {
      if (band_list[band_loop].full_band == candidate_band)
      {
         unique_band = FALSE;
         //Insert associated_list_index
         if (band_list[band_loop].size_list_indices != RFA_RF_LTE_MAX_CELL_SUPPORTED)
         {
            band_list[band_loop].associated_list_indices[band_list[band_loop].size_list_indices] = associated_list_index;
            band_list[band_loop].size_list_indices++;
         }
         else
         {
            return FALSE;
         }

         break;
      }
   }

   //handle unique band case
   if (unique_band == TRUE)
   {
      //Insert new Band
      if ((*band_list_size) != RFA_RF_LTE_MAX_CELL_SUPPORTED)
      {
         band_list[(*band_list_size)].full_band = candidate_band;
         band_list[(*band_list_size)].associated_list_indices[band_list[(*band_list_size)].size_list_indices] = associated_list_index;
         band_list[(*band_list_size)].size_list_indices++;
         (*band_list_size)++;
      }
      else
      {
         return FALSE;
      }
   }

   return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This API is meant to operate on list of channel,bw,device and fit them into same split band. Indicates RFCOM_BAND_LTE_INVALID if not able to fit within same split band
   Constraint: Input List Must Have Full Bands in it

   @details
   This API is meant to operate on list of channel,bw,device and fit them into same split band. Indicates RFCOM_BAND_LTE_INVALID if not able to fit within same split band
   Constraint: Input List Must Have Full Bands in it

   @parameter
   @param
   input_list: List of Channels, BWs, Devices 
   @param 
   list_size: Size of input_list
*/
void rfc_lte_intf::get_band_based_on_list(rflte_mc_band_decision_config_params_s_type* input_list, uint32 list_size)
{
   uint32 list_index = 0;
   uint32 unique_band_list_size = 0;
   uint32 unique_band_loop_index = 0;
   rflte_mc_band_index_list_s_type unique_band_index_list[RFA_RF_LTE_MAX_CELL_SUPPORTED];
   rfc_cfg_params_type  cfg;
   rfc_band_split_info_type  *signal_list_ptr = NULL;
   rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
   boolean band_split_ret_val = FALSE;
   uint32 table_index = 0;
   rfcom_lte_band_type table_match_list[RFA_RF_LTE_MAX_CELL_SUPPORTED][RFLTE_MC_MAX_NUM_SPLITS];
   uint32 associated_list_loop = 0;
   uint32 table_column_row0 = 0;
   uint32 table_column = 0;
   uint32 num_split_match_found[RFA_RF_LTE_MAX_CELL_SUPPORTED];
   uint32 local_index = 0;
   rfcom_lte_band_type band_match = RFCOM_BAND_LTE_INVALID;
   boolean match_found = FALSE;
   uint32 debug_print_loop = 0;
   boolean debug_rfc_split_band_decision = FALSE;

   if (rfc_data == NULL)
   {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
      return;
   }
   
   //Initialise
   memset(&cfg, 0x0, sizeof(rfc_cfg_params_type));

   for (unique_band_loop_index = 0; unique_band_loop_index <  RFA_RF_LTE_MAX_CELL_SUPPORTED; unique_band_loop_index++)
   {
      unique_band_index_list[unique_band_loop_index].size_list_indices = 0;
   }

   //Find Number of Unique Bands and Create List of Unique Bands with Associated input_list indices
   for (list_index = 0; list_index < list_size ; list_index++)
   {
      if (candidate_band_insert(unique_band_index_list,&unique_band_list_size,input_list[list_index].band,list_index) == FALSE)
      {
         ERR_FATAL("band list full : Cannot Insert Unique Band in list",0,0,0);
      }
   }

   //MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "get_band_based_on_list : Unique Band List Size %d", unique_band_list_size);

   //Operate on Each Unique band and when size_list_indices > 1 , channels, BWs must fit within same Band
   for (unique_band_loop_index = 0; unique_band_loop_index < unique_band_list_size; unique_band_loop_index++)
   {
      if (debug_rfc_split_band_decision == TRUE)
      {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "get_band_based_on_list : Unique Band %d Associated List Size %d",
               unique_band_index_list[unique_band_loop_index].full_band,
               unique_band_index_list[unique_band_loop_index].size_list_indices);
      }

      //Go over associated List Indices and for each find all possible matches in Split Config Table
      for (associated_list_loop = 0; associated_list_loop < unique_band_index_list[unique_band_loop_index].size_list_indices ; associated_list_loop++)
      {
         num_split_match_found[associated_list_loop] = 0;
         local_index = unique_band_index_list[unique_band_loop_index].associated_list_indices[associated_list_loop];

         /*Data for Local List*/
         if (debug_rfc_split_band_decision == TRUE)
         {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "get_band_based_on_list : Local_index %d",local_index);
            MSG_6(MSG_SSID_RF, MSG_LEGACY_HIGH, "get_band_based_on_list : alt %d bnd %d bw %d chan %d device %d path_ovr %d",
                  input_list[local_index].alternate_path,
                  input_list[local_index].band,
                  input_list[local_index].bw,
                  input_list[local_index].channel,
                  input_list[local_index].device,
                  input_list[local_index].path_override_index);
         }

         // Setup configuration parameters
         cfg.logical_device = input_list[local_index].device;
         cfg.band = (int)(input_list[local_index].band);
         cfg.rx_tx = RFC_CONFIG_RX;
         cfg.alternate_path = input_list[local_index].alternate_path;

         band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);

         if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
         {
            table_index = 0;
            /* We need to first validate the path index override param and retreive the band */
            if (input_list[local_index].path_override_index < RFC_PATH_MAX)
            {
              /* Loop through path index override to find true band */
              while ((unsigned int)signal_list_ptr[0].band_info[table_index].path_index != RFC_INVALID_PARAM)
              {
                if ((unsigned int)signal_list_ptr[0].band_info[table_index].path_index == input_list[local_index].path_override_index)
                {
                  table_match_list[associated_list_loop][num_split_match_found[associated_list_loop]] = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[table_index].band);
                  num_split_match_found[associated_list_loop]++;
                  break;
                }
                table_index++;
              }
            }
            else
            {
              /* Loop through channels to find true band based on BW */
              while ((unsigned int)signal_list_ptr[0].band_info[table_index].band != RFC_INVALID_PARAM)
              {
                /* Match the System BW and Alternate Path before checking the channels */
                if (((signal_list_ptr[0].band_info[table_index].sys_bw == input_list[local_index].bw)|| (signal_list_ptr[0].band_info[table_index].sys_bw == RFC_BW_ALL)) && 
                    (signal_list_ptr[0].band_info[table_index].alt_path == input_list[local_index].alternate_path))
                {
                  /* Once the BW and Alternate Path is matched then scan for valid chan */
                  if (((uint32)input_list[local_index].channel >= signal_list_ptr[0].band_info[table_index].start_chan) && 
                      ((uint32)input_list[local_index].channel <= signal_list_ptr[0].band_info[table_index].stop_chan))
                  {
                    table_match_list[associated_list_loop][num_split_match_found[associated_list_loop]] = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[table_index].band);
                    num_split_match_found[associated_list_loop]++;
                  }
                }
                table_index++;
              }
            }
         } 
         else
         {
            /* Push Table Match Into table_match_list. In this case it is same as input*/
            table_match_list[associated_list_loop][num_split_match_found[associated_list_loop]] = input_list[local_index].band;
            num_split_match_found[associated_list_loop]++;
         }

         if(debug_rfc_split_band_decision == TRUE)
         {
            MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "Associated index %d Split match found for channel = %d Number Matches %d",
                  associated_list_loop,
                  input_list[local_index].channel,
                  num_split_match_found[associated_list_loop]);

            for (debug_print_loop = 0; debug_print_loop <  num_split_match_found[associated_list_loop] ; debug_print_loop++)
            {
               MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "Band %d", table_match_list[associated_list_loop][debug_print_loop]);
            }
         }
      }

      /*Find Intersection between Associated Indices using N choose 2 with Running Intersection List. Exit if Intersection List becomes NULL. 
      Note we allow comparison between same entry here so that when There is only one Associated List Index, we populate Intersection List Correctly*/

      /**************************************************Table Matching***************************************************************************
      Take ROW0 of Table and find match between its entries with the rest of the rows including itself. When Match Found among all ROWS => Found Intersection 
      If ran out of first row entries and no match found => Intersection Not Possible. Pictogram Below 

      TABLE CASE I
       
      X Y Z            ------->        ROW 0
      Y W
      Z Y W 
       
      for this example , Intersection is Y and shall be found while processing 2nd entry in ROW 0 
       
      TABLE CASE II 
       
      X              --------->        ROW 0 
       
      for this example , Intersection is X with ROW 0 and shall be found while processing 1st entry in ROW 0
      
      ******************************************************************************************************************************************/

      //Initialize Band
      band_match = RFCOM_BAND_LTE_INVALID;
      match_found = FALSE;
      /*Process entries in ROW 0*/
      for (table_column_row0 = 0; (table_column_row0   < num_split_match_found[0]) && (match_found == FALSE) ; table_column_row0++)
            {
         /*Initialize*/
         match_found = TRUE;
         /*Go over all ROWs*/
         for (associated_list_loop = 0; (associated_list_loop < unique_band_index_list[unique_band_loop_index].size_list_indices) && (match_found == TRUE); associated_list_loop++)
               {
            match_found = FALSE;
            /*Go over all COLs*/
            for (table_column = 0; (table_column  <  num_split_match_found[associated_list_loop]) && (match_found == FALSE) ; table_column++)
                  {
               if (table_match_list[0][table_column_row0] == table_match_list[associated_list_loop][table_column] )
                     {
                  match_found = TRUE;
                  }
               }
               }

         /*Intersection Update*/
         if (match_found == TRUE)
               {
            band_match = table_match_list[0][table_column_row0];
         }
      }


      //Mark Final Band
         for (associated_list_loop = 0; associated_list_loop < unique_band_index_list[unique_band_loop_index].size_list_indices ; associated_list_loop++)
         {
            local_index = unique_band_index_list[unique_band_loop_index].associated_list_indices[associated_list_loop];
         input_list[local_index].band = band_match;
         if(debug_rfc_split_band_decision == TRUE)
         {
            MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "List Index %d Band %d",local_index,input_list[local_index].band);
         }
      }
      
   }

}


  /* These new APIs are for querying band based on additional params */
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band based on certain selection params.

   @details
   The function will return rx band part of the rfc_band pointer. 

   @parameter
   @param
   split_band: Flag specifying if the path was split or not. 
   @param 
   rx_chan: Rx channel
   @param 
   curr_bw: current system bw
   @param 
   path_index_override: Path override index to match the Tx path on the Band  
   @param 
   *rfc_band: updated rfc band.

*/
void rfc_lte_intf::get_rx_band_v2(boolean *split_band, 
                                  rfcom_device_enum_type device,
                                  rfcom_lte_earfcn_type rx_chan,
                                  rfc_lte_bw_type curr_bw,
                                  uint8 path_index_override,
                                  rfcom_lte_band_type *rfc_band,
                                  uint8 alt_path)
{
  
  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  int i = 0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }
  // Setup configuration parameters
  cfg.logical_device = device;
  cfg.band = (int)*rfc_band;
  cfg.rx_tx = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);

  if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
  {
    *split_band = TRUE;

    /* We need to first validate the path index override param and retreive the band */
    if (path_index_override < RFC_PATH_MAX)
    {
      /* Loop through path index override to find true band */
      while ((unsigned int)signal_list_ptr[0].band_info[i].path_index != RFC_INVALID_PARAM)
      {
        if ((unsigned int)signal_list_ptr[0].band_info[i].path_index == path_index_override)
        {
          *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
          break;
        }
        i++;
      }
    }
    else
    {
      /* Loop through channels to find true band based on BW */
      while ((unsigned int)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
      {
        /* Match the System BW and Alternate Path before checking the channels */
        if (((signal_list_ptr[0].band_info[i].sys_bw == curr_bw)|| (signal_list_ptr[0].band_info[i].sys_bw == RFC_BW_ALL)) &&
            (signal_list_ptr[0].band_info[i].alt_path == alt_path))
        {
          /* Once the BW and Alternate Path is matched then scan for valid chan */
          if (((uint32)rx_chan >= signal_list_ptr[0].band_info[i].start_chan) && 
              ((uint32)rx_chan <= signal_list_ptr[0].band_info[i].stop_chan))
          {
            *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
            break;
          }
        }
        i++;
      }
    }
  } 
  else
  {
    /* Do Nothing, probably not a split band */
  }

  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "Split Band, final band returned = %d alt_path = %d", *rfc_band, alt_path);
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the tx band based on certain selection params.

   @details
   The function will return tx band part of the rfc_band pointer. 

   @parameter
   @param
   split_band: Flag specifying if the path was split or not. 
   @param 
   rx_chan: tx channel
   @param 
   curr_bw: current system bw
   @param 
   path_index_override: Path override index to match the Tx path on the Band  
   @param 
   *rfc_band: updated rfc band.

*/

void rfc_lte_intf::get_tx_band_v2(boolean *split_band, 
                                  rfcom_lte_earfcn_type tx_chan,
                                  rfc_lte_bw_type curr_bw,
                                  uint8 path_index_override,
                                  rfcom_lte_band_type *rfc_band,
                                  boolean filtered_band)
{
  
  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  int i = 0;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;


  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(*rfc_band, &temp_logical_device, RFC_CONFIG_TX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_tx_band_v2, failed to get supported device for band %d ", *rfc_band);
    return;
  }

  // Setup configuration parameters
  cfg.logical_device = temp_logical_device;
  cfg.band = (int)*rfc_band;
  cfg.rx_tx = RFC_CONFIG_TX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);

  if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
  {
    *split_band = TRUE;

    /* We need to first validate the path index override param and retreive the band */
    if (path_index_override < RFC_PATH_MAX)
    {
      /* Loop through path index override to find true band */
      while ((unsigned int)signal_list_ptr[0].band_info[i].path_index != RFC_INVALID_PARAM)
      {
        if ((unsigned int)signal_list_ptr[0].band_info[i].path_index == path_index_override)
        {
          *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
          break;
        }
        i++;
      }
    }
    else
    {
      /* Loop through channels to find true band based on BW */
      while ((unsigned int)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
      {
        /* filtered_band will only take precedence if Path override index is not valid */
        if (filtered_band)
        {
          if (signal_list_ptr[0].band_info[i].status == RFC_FILTERED_PATH_TRUE)
          {
             *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
             break;
          }
        }
        /* Match the System BW before checking the channels */
        else if (((signal_list_ptr[0].band_info[i].sys_bw == curr_bw)||(signal_list_ptr[0].band_info[i].sys_bw == RFC_BW_ALL)) &&
                 ((signal_list_ptr[0].band_info[i].alt_path == RFLTE_MC_DEFAULT_ALTERNATE_PATH)))

        {
          /* Once the BW is matched then scan for valid chan */
          if (((uint32)tx_chan >= signal_list_ptr[0].band_info[i].start_chan) && 
              ((uint32)tx_chan <= signal_list_ptr[0].band_info[i].stop_chan))
          {
            *rfc_band = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
            break;
          }
        }
        i++;
      }
    }
  } 
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.
 
   @param
   split_band: band for which low and high channels are desired
   *low_earfcn: Pointer to low chan
   *high_earfcn: Pointer to high chan

   @details

*/
void rfc_lte_intf::get_rx_hi_lo_earfcn(rfcom_lte_band_type split_band, 
                                       rfcom_lte_earfcn_type *low_earfcn,
                                       rfcom_lte_earfcn_type *high_earfcn)
{

  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;

  /* These params help to detect the largest range for a given split band */
  boolean largest_range_detected = FALSE;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;

  int i = 0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(split_band, &temp_logical_device, RFC_CONFIG_RX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_rx_hi_lo_earfcn, failed to get supported device for band %d ", split_band);
    return;
  }


  // Setup configuration parameters
  cfg.logical_device = temp_logical_device;
  cfg.band = split_band;
  cfg.rx_tx = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
 
  if ((band_split_ret_val == TRUE) && (signal_list_ptr != NULL))
  {
   /* Loop through channels to find true band and the largest channel range */
    while ((uint32)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
    {
      if (signal_list_ptr[0].band_info[i].band == split_band)
      {
        if (largest_range_detected == FALSE)
        {
          *low_earfcn = signal_list_ptr[0].band_info[i].start_chan;
          *high_earfcn = signal_list_ptr[0].band_info[i].stop_chan;
          largest_range_detected = TRUE;
        }
        else
        {
          if (*low_earfcn > signal_list_ptr[0].band_info[i].start_chan)
          {
            *low_earfcn = signal_list_ptr[0].band_info[i].start_chan;
          }
          if (*high_earfcn < signal_list_ptr[0].band_info[i].stop_chan)
          {
              *high_earfcn = signal_list_ptr[0].band_info[i].stop_chan;
          }
        }
      }
      i++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  API to update the rf port enumeration for various devices 
 
  @details
  Function is used as part of path swapping procedure. It is called when a
  suitable alternate path is selected for a particular CA or DSDS/DSDA scenario.
  The alternate path info along with the current band and logical device is used
  to determine which specific device info structure for that band should be
  used. Based on the info, rf port enumeration for various rf devices are
  updated. 
 
  @param rfm_dev
  current logical device which needs port reconfiguration
 
  @param curr_band
  current rf band involved in path swapping or port selection 
 
  @param alt_path
  preferred alternate path to use for a specific logical device & band combo
 
  @return status
  TRUE if a successful match is found in RFC AG and rf port info update is
  complete, else return FALSE 
*/
boolean rfc_lte_intf::get_updated_rf_port
(
  rfm_device_enum_type rfm_dev, 
  rf_card_band_type curr_band, 
  uint8 alt_path
)
{
  rfcom_lte_band_type  rf_band = RFCOM_BAND_LTE_INVALID; 
  rfc_cfg_params_type  cfg;
  rfc_device_info_type *device_info_ptr;
  boolean status = TRUE;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  uint8 dev_type = 0;
  uint8 dev_instance = 0;

  if (rfc_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC LTE Data Object is NOT Created", 0);
    status = FALSE;
  }

  if(status == TRUE)
  {    
  /* Convert from rf card band type to rf com band type */
  rf_band = rflte_util_convert_rfc_band_to_rfcom_band(curr_band); 

  /* Initialize the config params structure */
  cfg.logical_device = rfm_dev;
  cfg.band           = (uint8)rf_band; 
  cfg.rx_tx          = RFC_CONFIG_RX;
  cfg.req            = RFC_REQ_DEFAULT_GET_DATA;
  cfg.port_state     = RFC_PORT_STATE_A;

  /* Enable when AG support is available */
  cfg.alternate_path = alt_path; 

  /* Get the column specific to a band, logical device & alt path */
    status = rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr); 
  } 

  if(status == TRUE)
  {

    rfc_lte_device_enter_critical_section();
	
	for(dev_type = 0; dev_type < RFDEVICE_TYPE_MAX_NUM; dev_type++)
    {
      for (dev_instance = 0; dev_instance <RFC_MAX_DEVICE_INSTANCES_PER_BAND; dev_instance++)
      {
        if(rf_band < RFCOM_NUM_LTE_BANDS)
        {
        lte_rf_devices[rfm_dev][cfg.rx_tx][rf_band][dev_type][dev_instance] = NULL;
      }
    }
    }
    /* Update the band port enumeration for all devices in that logical path */
    init_rf_devices(&cfg, device_info_ptr); 
    
    rfc_lte_device_leave_critical_section();

  }

  return status; 
}

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  API to detect the delta devices between two alternate paths for the same
  logical device
 
  @details
  Function retrieves the devices list for a particular logical device and its
  two alternate paths. These device lists are compared and their delta devices
  are stored and return to the calling function. The information is used by
  lte tech main control to determine the reconfiguration requirements in the
  case of a Pcell/Scell swap
 
  @param rfm_dev
  current logical device for which a path update is requested
 
  @param curr_band
  current band in use
 
  @param prev_alt_path
  alternate path which is currently used by this logical device for
  the specific band
 
  @param next_alt_path
  alternate path which will be replacing the current alt path for this
  particular logical device and band combo
 
  @retval status
  return's TRUE if its able to successfully detect the delta devices between
  two alternate paths, else returns FALSE. 

*/
boolean rfc_lte_intf::get_delta_dev
(
  rfm_device_enum_type rfm_dev, 
  rf_card_band_type curr_band, 
  uint8 prev_alt_path, 
  uint8 next_alt_path 
)
{
  uint8 loop_idx  = 0; 
  uint8 delta_idx = 0; 
  rfcom_lte_band_type  rf_band = RFCOM_BAND_LTE_INVALID; 
  rfc_cfg_params_type  prev_cfg;
  rfc_cfg_params_type  next_cfg; 
  rfc_device_info_type *prev_dev_info_ptr;
  rfc_device_info_type *next_dev_info_ptr; 
  rfdevice_type_enum_type delta_dev[RFDEVICE_TYPE_MAX_NUM]; 
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return FALSE;
  }

  /* Convert from rf card band type to rf com band type */
  rf_band = rflte_util_convert_rfc_band_to_rfcom_band(curr_band); 

  /* Initialize the config params structure */
  prev_cfg.logical_device = rfm_dev;
  prev_cfg.band           = (uint8)rf_band; 
  prev_cfg.rx_tx          = RFC_CONFIG_RX;
  prev_cfg.req            = RFC_REQ_DEFAULT_GET_DATA;
  prev_cfg.port_state     = RFC_PORT_STATE_A;
  /* // Wait for alternate path support
  prev_cfg.alt_path       = prev_alt_path; */ 

  next_cfg.logical_device = rfm_dev;
  next_cfg.band           = (uint8)rf_band; 
  next_cfg.rx_tx          = RFC_CONFIG_RX;
  next_cfg.req            = RFC_REQ_DEFAULT_GET_DATA;
  next_cfg.port_state     = RFC_PORT_STATE_A;
  /* // Wait for alternate path support
  next_cfg.alt_path       = next_alt_path; */ 

  /* Obtain reference to the column in RFC ss matching the current band,
     logical device and alternate path */
  rfc_data->devices_cfg_data_get(&prev_cfg, &prev_dev_info_ptr); 
  rfc_data->devices_cfg_data_get(&next_cfg, &next_dev_info_ptr); 

  if (prev_dev_info_ptr == NULL || next_dev_info_ptr == NULL)
  {
    RF_MSG(RF_ERROR, "Unable to find a matching column in "
                     "RFC SS for specified alternate path"); 
    return FALSE; 
  }

  if (prev_dev_info_ptr->num_devices != next_dev_info_ptr->num_devices)
  {
    RF_MSG(RF_ERROR, "Check if the RFC columns are matching in terms of "
                     "number of devices to reconfigure"); 
    return FALSE; 
  }

  for (loop_idx = 0; loop_idx < prev_dev_info_ptr->num_devices; loop_idx++)
  {
    if ( prev_dev_info_ptr->rf_asic_info[loop_idx].device_type == 
         next_dev_info_ptr->rf_asic_info[loop_idx].device_type )
    {
      if ( prev_dev_info_ptr->rf_asic_info[loop_idx].data[1] != 
           next_dev_info_ptr->rf_asic_info[loop_idx].data[1] )
      {
        delta_dev[delta_idx++] = next_dev_info_ptr->rf_asic_info[loop_idx].device_type; 
      } 
    }
  }

  return TRUE; 
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.
 
   @param
   *split_band: pointer to boolean indicating whether it was a split band
   rx_chan: DL channel number
   *rfc_band: band for which data is desired

   @details

*/
void rfc_lte_intf::get_split_rx_bands(boolean *split_band, rfcom_lte_earfcn_type rx_chan, 
                               rfcom_lte_band_type *rfc_band1, rfcom_lte_band_type *rfc_band2)
{
  
  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  /* split_count keeps track of number of split band matches for a given earfcn.
     This will be 2 in case a earfcn falls in the overlap region between two split bands*/
  int i = 0, split_count = 0;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;


  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(*rfc_band1, &temp_logical_device, RFC_CONFIG_RX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_split_rx_bands, failed to get supported device for band %d ", *rfc_band1);
    return;
  }

  /* Setup configuration parameters */
  cfg.alternate_path = 0;
  cfg.logical_device = temp_logical_device;
  cfg.band = (int)*rfc_band1;
  cfg.rx_tx = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
  if ( (band_split_ret_val == TRUE) && (signal_list_ptr != NULL) )
  {
    *split_band = TRUE;
    /* Loop through channels to find true band */
      while ((unsigned int)signal_list_ptr[0].band_info[i].band != RFC_INVALID_PARAM)
      {
        if (((uint32)rx_chan >= signal_list_ptr[0].band_info[i].start_chan) && ((uint32)rx_chan <= signal_list_ptr[0].band_info[i].stop_chan))
        {
          split_count++;
          /* split_count will be 1 for the 1st match, set both bands the same in case there is no
           * overlap
           */
          if (split_count == 1)
          {
            *rfc_band1 = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
            *rfc_band2 = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
          }
          /* split_count will be 2 for the 2nd match, in this case, there is an overlap*/
          else if (split_count == 2)
          {
            *rfc_band2 = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[i].band);
            break;
          }
        }
        i++;
      }
  } 
  else
  {
    /* Do Nothing, not a split band */
  }
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.
 
   @param
   Input parameter:
   split_band: band for which low and high channels are desired
   Output Parameters:
   *low_earfcn: will store list of low earfcns
   *high_earfcn: will store list of high earfcns
   *band_list: Will store list of split bands 

   @details
   Limitation: only works on RFC which is bandwidth independent. To support RFC list which lists earfcns
   by split band and bandwidth, this api may need modification

*/
void rfc_lte_intf::get_rx_split_band_channel_rasters(rfcom_lte_band_type split_band, 
                                                     rfcom_lte_earfcn_type *low_earfcn,
                                                     rfcom_lte_earfcn_type *high_earfcn,
                                                     rfcom_lte_band_type *band_list,
                                                     uint8 *num_of_splits)
{

  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;
  int split_count = 0;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return;
  }

  if(!rfc_lte_get_band_supported_device(split_band, &temp_logical_device, RFC_CONFIG_RX))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"get_rx_split_band_channel_rasters, failed to get supported device for band %d ", split_band);
    return;
  }

  // Setup configuration parameters
  cfg.logical_device = temp_logical_device;
  cfg.band = split_band;
  cfg.rx_tx = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
 
  if ((band_split_ret_val == TRUE) && (signal_list_ptr != NULL))
  {
   /* Loop through RFC data to get earfcn range and list of split bands */
    while ((uint32)signal_list_ptr[0].band_info[split_count].band != RFC_INVALID_PARAM)
    {
      if (RFLTE_MC_MAX_RFC_SPLITS == split_count)
      {
        MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC has more split band combos than can be handled by RFSW!! Please check RFC SS.");
        break;
      }
      if (NULL != low_earfcn)
      {
        *low_earfcn = (rfcom_lte_earfcn_type)signal_list_ptr[0].band_info[split_count].start_chan;
        low_earfcn++;
      }
      if (NULL != high_earfcn)
      {
        *high_earfcn = (rfcom_lte_earfcn_type)signal_list_ptr[0].band_info[split_count].stop_chan;
        high_earfcn++;
      }
      if (NULL != band_list)
      {
         *band_list = (rfcom_lte_band_type)(signal_list_ptr[0].band_info[split_count].band);
         band_list++;
      }
      split_count++;
    }
    if (NULL != num_of_splits)
    {
      *num_of_splits = (uint8)split_count;
    }
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the AsDiv script for the requested device
  and band.

  @param position
  RF device for which to generate script
 
  @param cb_data
  Call back data to pass in additional info for extendability

  @param asdiv_script_ptr
  Output - The asdiv script
 
  @return
  True on success and FALSE on failure
*/

#ifdef FEATURE_RF_ASDIV  
boolean rfc_lte_intf::generate_asdiv_xsw_script
( 
  uint32 position,
  rf_buffer_intf *asdiv_script_ptr,
  void *cb_data
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rflte_mc_asdiv_get_script_struct_type *xsw_script_data;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  uint8 i= 0;
  rfdevice_xsw_config_type xsw_config;
  rf_device_execution_type script_exec_type;
  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_lte_intf::generate_xsw_sleep_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get the ASM script data */
  xsw_script_data = (rflte_mc_asdiv_get_script_struct_type*)cb_data;

  /* NULL pointer check */
  if ( xsw_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "generate_xsw_sleep_script: NULL callback data" );
    return FALSE;
  } /* if ( asm_script_data == NULL ) */

  /* Populate params */
  script_exec_type = (xsw_script_data->immediate == TRUE) ? RFDEVICE_EXECUTE_IMMEDIATE : RFDEVICE_CREATE_SCRIPT;

  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = xsw_script_data->device; /* Not required*/
  band_path_cfg.band = xsw_script_data->band;
  band_path_cfg.rx_tx=RFC_CONFIG_RX;
  band_path_cfg.path = RF_PATH_0;
  band_path_cfg.port_state = RFC_PORT_STATE_A;

  xsw_config.band.lte_band = xsw_script_data->band;
  xsw_config.mode = RFM_LTE_MODE;

  if ((rfcommon_asdiv_position_type) position == 1) 
  {
    band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
    band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

   /* update XSW port to use if needed */
   status = configure_asm_xsw_port( xsw_script_data->device, xsw_script_data->band);

   /* Get device info pointer */
   rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

   if ( device_info_ptr != NULL )
   {      
     /* loop through the device info ptr to find the index of each device object*/    
     for (i=0;i < device_info_ptr->num_devices;i++)
     {
       /*Get device type and device instance to retrieve device object*/
       cmn_dev_type = device_info_ptr->rf_asic_info[i].device_type;
       switch (cmn_dev_type) 
       {           
         case RFDEVICE_XSW:
           instance = device_info_ptr->rf_asic_info[i].instance;
           device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_XSW,(uint8)instance);

           if( device_obj != NULL )
           {
                xsw_config.trigger = RFDEVICE_XSW_TRIGGER_WAKEUP; 
                status &=  rfdevice_xsw_wakeup ( &xsw_config,(void *)device_obj, asdiv_script_ptr,script_exec_type, 0);
                xsw_config.trigger = RFDEVICE_XSW_TRIGGER_NONE; 
                status &= rfdevice_xsw_set_cross_switch_config ( &xsw_config,(void *)device_obj, asdiv_script_ptr, script_exec_type, 0 );
                xsw_config.trigger =  RFDEVICE_XSW_TRIGGER_ALL;
                status &= rfdevice_xsw_trigger ( &xsw_config, (void *)device_obj, asdiv_script_ptr, script_exec_type,0 );                         
           }
           else
           {
             status = FALSE ;
           }
          break;

          default:
            break;    
         } 
     }/*for (i < device_info_ptr->num_devices)*/

   } 
   else
   {
     status = FALSE ; 
     MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"rfc_lte_intf::generate_xsw_sleep_script "
                     "device_info_ptr is NULL ");
   }

  return status;

} /* xsw device script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function updates the XSW port in the ASM device containing the XSW based on
  device and band.

  @param device
  RF device for which to update XSW port

  @param band
  Band to update XSW port for

  @return
  True on success and FALSE on failure
*/
boolean rfc_lte_intf::configure_asm_xsw_port
( 
  rfm_device_enum_type device,
  rfcom_lte_band_type band
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  uint8 i= 0;
  rfcom_band_type_u band_u;

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_lte_intf::configure_asm_xsw_port: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Populate params */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = device;
  band_path_cfg.band           = band;
  band_path_cfg.rx_tx          = RFC_CONFIG_RX;
  band_path_cfg.path           = RF_PATH_0;
  band_path_cfg.port_state     = RFC_PORT_STATE_A;
  band_path_cfg.req            = RFC_REQ_DISABLE_ANT_SWAP_1; /* functionality is not position dependent */

  band_u.lte_band = band;

  /* Get device info pointer */
  rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

  if ( device_info_ptr != NULL )
  {        
   /* loop through the device info ptr to find the index of each device object*/
   for (i=0;i < device_info_ptr->num_devices;i++)
   {
     /*Get device type and device instance to retrieve device object*/
     cmn_dev_type = device_info_ptr->rf_asic_info[i].device_type;

     switch (cmn_dev_type) 
     {           
       case RFDEVICE_ASM:
         instance = device_info_ptr->rf_asic_info[i].instance;
         device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_ASM,(uint8)instance);

         if( device_obj != NULL )
         {
           status = ((rfdevice_asm *)device_obj)->set_asm_xsw_port_config( device, RFM_LTE_MODE, band_u );
         }
         else
         {
           status = FALSE;
         }

         break;

        default:
         break;    
       } 
   }/*for (i < device_info_ptr->num_devices)*/
  } 

  return status;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the AsDiv script for the requested device
  and band.

  @param position
  RF device for which to generate script
 
  @param cb_data
  Call back data to pass in additional info for extendability

  @param asdiv_script_ptr
  Output - The asdiv script
 
  @return
  True on success and FALSE on failure
*/

boolean rfc_lte_intf::generate_xsw_sleep_script
( 
  rfcommon_asdiv_position_type position,
  rf_buffer_intf *asdiv_script_ptr,
  void *cb_data
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rflte_mc_asdiv_get_script_struct_type *xsw_script_data;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  uint8 i= 0;
  rfdevice_xsw_config_type xsw_config;
  rf_device_execution_type script_exec_type;
  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_lte_intf::generate_asdiv_xsw_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Get the ASM script data */
  xsw_script_data = (rflte_mc_asdiv_get_script_struct_type*)cb_data;

  /* NULL pointer check */
  if ( xsw_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "generate_asdiv_xsw_script: NULL callback data" );
    return FALSE;
  } /* if ( asm_script_data == NULL ) */

  /* Populate params */
  script_exec_type = (xsw_script_data->immediate == TRUE) ? RFDEVICE_EXECUTE_IMMEDIATE : RFDEVICE_CREATE_SCRIPT;

  band_path_cfg.alternate_path = 0;
  band_path_cfg.logical_device = xsw_script_data->device; /* Not required*/
  band_path_cfg.band = xsw_script_data->band;
  band_path_cfg.rx_tx=RFC_CONFIG_RX;
  band_path_cfg.path = RF_PATH_0;
  band_path_cfg.port_state = RFC_PORT_STATE_A;

  xsw_config.band.lte_band = xsw_script_data->band;
  xsw_config.mode = RFM_LTE_MODE;

  if ((rfcommon_asdiv_position_type) position == 1) 
  {
    band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
    band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

   /* update XSW port to use if needed */
   status = configure_asm_xsw_port( xsw_script_data->device, xsw_script_data->band);

   /* Get device info pointer */
   rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

   if ( device_info_ptr != NULL )
   {      
     /* loop through the device info ptr to find the index of each device object*/    
     for (i=0;i < device_info_ptr->num_devices;i++)
     {
       /*Get device type and device instance to retrieve device object*/
       cmn_dev_type = device_info_ptr->rf_asic_info[i].device_type;
       switch (cmn_dev_type) 
       {           
         case RFDEVICE_XSW:
           instance = device_info_ptr->rf_asic_info[i].instance;
           device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_XSW,(uint8)instance);

           if( device_obj != NULL )
           {
                xsw_config.trigger = RFDEVICE_XSW_TRIGGER_SLEEP;
                status =  rfdevice_xsw_sleep ( &xsw_config,(void *)device_obj, asdiv_script_ptr,script_exec_type, 0);
           }
           else
           {
             status = FALSE ;
           }
          break;

          default:
            break;    
         } 
     }/*for (i < device_info_ptr->num_devices)*/

   } 
   else
   {
     status = FALSE ; 
     MSG(MSG_SSID_RF, MSG_LEGACY_HIGH,"rfc_lte_intf::generate_asdiv_xsw_script "
                     "device_info_ptr is NULL ");
   }

  return status;

} /* xsw device script */

#endif /*#define FEATURE_RF_ASDIV */

void *rfc_lte_intf::get_asd_xsw_config(rfm_device_enum_type device, rfcom_lte_band_type band, int position)
{
  rfc_cfg_params_type       cfg;
  rfc_sig_cfg_type          *rf_asd_signals = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfdevice_xsw_config_type xsw_config;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return rf_asd_signals;
  }

  // Setup configuration parameters
  cfg.alternate_path = 0;
  cfg.logical_device = device;
  cfg.band = band;

  xsw_config.band.lte_band=band;
  xsw_config.mode= RFM_LTE_MODE;


   if ( position == 1 ) 
  {
     cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
     xsw_config.xsmode=RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
      cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
      xsw_config.xsmode=RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

  rfc_data->sig_cfg_data_get(&cfg, &rf_asd_signals);

  return( (void*)rf_asd_signals);

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function generates the AsDiv script for the requested device
  and band.

  @param position
  RF device for which to generate script
 
  @param xsw_script_data
  Call back data to pass in additional info for extendability

  @param srs_xsw_script_ptr
  Output - The asdiv script
 
  @return
  True on success and FALSE on failure
*/
boolean rfc_lte_intf::generate_srs_xsw_script
( 
  uint32 position,
  rf_buffer_intf *srs_xsw_script_ptr,
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  boolean immediate
)
{
  boolean status = TRUE;
  rfc_cfg_params_type band_path_cfg;
  rfc_device_info_type* device_info_ptr = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type cmn_dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  /* Pointer to device class to hold common object */
  rfdevice_class *device_obj = NULL;
  uint8 i= 0;
  rfdevice_xsw_config_type xsw_config;
  rfc_sig_cfg_type *rfc_grfc_tbl_ptr = NULL;
  int32 number_of_grfc = 0;
  rf_device_execution_type script_exec_type;

  if ( rfc_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_lte_intf::generate_srs_xsw_script: "
                      "rfc_data ptr is null !!");
    return FALSE;
  }

  /* Populate params */
  band_path_cfg.alternate_path = 0;
  band_path_cfg.path = RF_PATH_0;
  band_path_cfg.logical_device = device; /* Not required*/
  band_path_cfg.band = band;
  band_path_cfg.rx_tx=RFC_CONFIG_RX;
  band_path_cfg.port_state = RFC_PORT_STATE_A;

  xsw_config.band.lte_band = band;
  xsw_config.mode = RFM_LTE_MODE;

  
  script_exec_type = (immediate == TRUE) ? RFDEVICE_EXECUTE_IMMEDIATE : RFDEVICE_CREATE_SCRIPT;

  if (position == 1) 
  {
    band_path_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_SWAP;
  }
  else
  {
    band_path_cfg.req = RFC_REQ_DISABLE_ANT_SWAP_1;
    xsw_config.xsmode = RFDEVICE_XSW_PRX_DRX_PATH_DEFAULT;
  }

   /* Get the list of devices */
   rfc_data->devices_cfg_data_get(&band_path_cfg, &device_info_ptr );

   /*Configure the list of devices*/
   if ( device_info_ptr != NULL )
   {      
     /* loop through the device info ptr to find the index of each device object*/    
     for (i=0;i < device_info_ptr->num_devices;i++)
     {
       /*Get device type and device instance to retrieve device object*/
       cmn_dev_type = device_info_ptr->rf_asic_info[i].device_type;
       switch (cmn_dev_type) 
       {           
         case RFDEVICE_XSW:
           instance = device_info_ptr->rf_asic_info[i].instance;
           device_obj = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_XSW,(uint8)instance);

           if( device_obj != NULL )
           {
             
	        //status & =  rfdevice_xsw_wakeup ( &xsw_config,(void *)device_obj, srs_xsw_script_ptr,script_exec_type, 0);
                xsw_config.trigger = RFDEVICE_XSW_TRIGGER_NONE; 
                status &=  rfdevice_xsw_set_cross_switch_config ( &xsw_config,(void *)device_obj, srs_xsw_script_ptr, script_exec_type, 0 );
                xsw_config.trigger =  RFDEVICE_XSW_TRIGGER_ALL;
                status &= rfdevice_xsw_trigger ( &xsw_config, (void *)device_obj, srs_xsw_script_ptr, script_exec_type,0 );         
              

           }
           else
           {
             status = FALSE ;
           }
          break;

          default:
            break;    
         } 
     }/*for (i < device_info_ptr->num_devices)*/
   } 
   else
   {
     MSG(MSG_SSID_RF_DEBUG, MSG_LEGACY_ERROR,"rfc_lte_data::generate_srs_xsw_script "
                     "device_info_ptr is NULL ");
   }

  /*Get the GRFC's Info */  

  rfc_grfc_tbl_ptr = (rfc_sig_cfg_type*)get_asd_xsw_config(device,band,position);

  if (rfc_grfc_tbl_ptr != NULL)
  {
     /*Get ANT_SEL grfc's in script buffer*/
    number_of_grfc = rfc_common_get_grfc_buffer(rfc_grfc_tbl_ptr, RFC_ANT_SEL, srs_xsw_script_ptr);
  }
  
  MSG_4(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH,"rfc_lte_data::generate_srs_xsw_script"
                                     "Number of GRFC extracted for SRS switch = %d "
                                     "RFM_Device:%d, band:%d, Position:%d",
                                     device,band,position,number_of_grfc);

  return status;
}

boolean rfc_lte_intf::get_ns07_ports
(
  rfm_device_enum_type rfm_dev, 
  rfcom_lte_band_type rf_band  
)
{
  rfc_cfg_params_type  cfg;
  rfc_device_info_type *device_info_ptr;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();

  if (rfc_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return FALSE;
  }
  
  /* Initialize the config params structure */
  cfg.logical_device = rfm_dev;
  cfg.band           = (uint8)rf_band; 
  cfg.rx_tx          = RFC_CONFIG_RX;
  cfg.req            = RFC_REQ_DEFAULT_GET_DATA;
  cfg.port_state     = RFC_PORT_STATE_NS07;

  /* Enable when AG support is available */
  // cfg.alternate_path = alt_path; 

  /* Get the column specific to a band, logical device & alt path */
  rfc_data->devices_cfg_data_get(&cfg, &device_info_ptr);   

  /* Update the band port enumeration for all devices in that logical path */
  if (device_info_ptr != NULL)
  {
    init_rf_devices(&cfg, device_info_ptr); 
  }
  else
  {
    RF_MSG_3( RF_ERROR, "RF LTE error. No signals available. device %d band %d altp %d. Cannot get ports", rfm_dev, rf_band, cfg.alternate_path );
  }

  return TRUE; 
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function gives the split band mask for the requested full band.

  @param band
  Full Band for which the split band mask is to be generated.
 
  @return
  uint64 with bit positions corresponding to split bands set to 1. 
  If no split bands, then 0 is returned.
*/
uint64 rfc_lte_intf::get_split_bands(rfcom_lte_band_type band)
{
  rfc_cfg_params_type   cfg;
  rfc_band_split_info_type  *signal_list_ptr = NULL;
  rfc_lte_data *rfc_data     = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  uint64 split_band          = 0;   
  int counter                = 0;
  rfm_device_enum_type temp_logical_device = RFM_INVALID_DEVICE;

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC LTE Data Object is NOT Created", 0);
    return 0;
  }

  if(!rfc_lte_get_band_supported_device(band, &temp_logical_device, RFC_CONFIG_RX))
  {
    MSG_1(MSG_SSID_RF_DEBUG, MSG_LEGACY_ERROR,"get_split_bands, failed to get supported device for band %d ", band);
    return 0;
  }

  // Setup configuration parameters
  cfg.logical_device = temp_logical_device;
  cfg.band           = band;
  cfg.rx_tx          = RFC_CONFIG_RX;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr);
  
  if ( ( TRUE == band_split_ret_val ) && ( NULL != signal_list_ptr ) )
  {
    /* Loop through channels to find split bands */
    while ((uint32)signal_list_ptr[0].band_info[counter].band != RFC_INVALID_PARAM)
    {
      /*  Check to ignore the invald split bands in RFC.
              The invalid bands are added in RFC in case the main band is empty. */
      if( ( 99999 != signal_list_ptr[0].band_info[counter].start_chan ) || 
               ( 99999 != signal_list_ptr[0].band_info[counter].stop_chan ) )
          {
            split_band = split_band | ( ( ( uint64 ) 0x1 ) << signal_list_ptr[0].band_info[counter].band );
      }
      counter++;
        }
  }

  return split_band;
  
}/* get_split_bands(rfcom_lte_band_type band) */

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function checks whether intra-band is possible and returns the band in case intra ca is
   possible
 
   @param pcc_info
   Pointer to PCell carrier information
 
   @param scc_info
   Pointer to SCell carrier information
 
   @param intra_ca_band   
   Pointer to be populated with the intra_ca_split_band if intra is true

   @details
   This function checks whether intra-band is possible and returns the band in case intra ca is
   possible.
   The possible scenarios are:
   1. inter-band CA: should return FALSE
   2. intra CA (no split): return TRUE, populate intra_ca_band with pcc\scc band
   3. intra CA (scc in same split as pcc): return TRUE, populate intra_ca_band with pcc\scc band
   4. intra CA (scc in different split, pcc retune needed): return TRUE, populate intra_ca_band with new split
   5. split CA: (scc and pcc cannot fit in a split): return FALSE: This should be disallowed at higher layers
   6. split CA: (pcc and scc belong to different split bands eg: B40 and B41): return FALSE
*/

boolean rfc_lte_intf::check_intra_ca_feasible(rflte_mc_carrier_info_rfc_type *pcc_info, 
                                              rflte_mc_carrier_info_rfc_type *scc_info,
                                              rfcom_lte_band_type *intra_ca_band,
                                              rfc_rxtx_enum_type config_type)
{

  rfc_cfg_params_type       cfg;
  rfc_band_split_info_type          *signal_list_ptr_pcc = NULL;
  rfc_band_split_info_type          *signal_list_ptr_scc = NULL;
  rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  boolean band_split_ret_val = FALSE;
  boolean debug_flag = FALSE;
  uint8 i = 0;
  uint8 j = 0;
  boolean intra_feasible = FALSE;

  int split_count = 0;

  /* Validate input parameters */
  if (NULL == pcc_info)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"check_intra_ca_feasible: pcc_info is NULL", 0);
    return FALSE;
  }

  if (NULL == scc_info)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"check_intra_ca_feasible: scc_info is NULL", 0);
    return FALSE;
  }

  if (NULL == intra_ca_band)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"check_intra_ca_feasible: intra_ca_band is NULL", 0);
    return FALSE;
  }
  /* End of Validate input parameters */

  if (rfc_data == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"check_intra_ca_feasible: RFC LTE Data Object is NOT Created", 0);
    return FALSE;
  }

  if ((TRUE == debug_flag))
  {
    MSG_5(MSG_SSID_RF, MSG_LEGACY_HIGH, "Input params: PCC band %d, bw %d, Chan %d, device %d, override %d", 
          pcc_info->band, pcc_info->rflte_sys_bw, pcc_info->rflte_chan, pcc_info->device, pcc_info->path_override_index);
    MSG_5(MSG_SSID_RF, MSG_LEGACY_HIGH, "Input params: SCC band %d, bw %d, Chan %d, device %d, override %d", 
          scc_info->band, scc_info->rflte_sys_bw, scc_info->rflte_chan, scc_info->device, scc_info->path_override_index);
  }

  /* Get PCC RFC info */
  cfg.logical_device = pcc_info->device;
  cfg.band = pcc_info->band;
  cfg.rx_tx = config_type;

  band_split_ret_val = rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr_pcc);
  
  /* Get SCC RFC info */

  cfg.logical_device = scc_info->device;
  cfg.band = scc_info->band;
  cfg.rx_tx = config_type;

  band_split_ret_val &= rfc_data->band_split_cfg_data_get(&cfg, &signal_list_ptr_scc);
  

  if ((TRUE == debug_flag))
  {
  /* Debug only */
  if (signal_list_ptr_pcc != NULL)
  {
    MSG_4(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH, "check_intra_ca_feasible: Signal ptr 1st row: PCC band %d, bw %d, start Chan %d, stop_chan %d ", 
          signal_list_ptr_pcc[0].band_info[0].band, 
          signal_list_ptr_pcc[0].band_info[0].sys_bw, 
          signal_list_ptr_pcc[0].band_info[0].start_chan, 
          signal_list_ptr_pcc[0].band_info[0].stop_chan);
  }
  
  if (signal_list_ptr_scc != NULL)
  {
    MSG_4(MSG_SSID_RF, MSG_LEGACY_HIGH, "check_intra_ca_feasible: Signal ptr 1st row: SCC band %d, bw %d, start Chan %d, stop_chan %d ", 
          signal_list_ptr_scc[0].band_info[0].band, 
          signal_list_ptr_scc[0].band_info[0].sys_bw, 
          signal_list_ptr_scc[0].band_info[0].start_chan, 
          signal_list_ptr_scc[0].band_info[0].stop_chan);
  }
  }

  MSG_1(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH, "check_intra_ca_feasible: band_split_ret_val %d ", band_split_ret_val);

  if(band_split_ret_val == TRUE)
  {
    if ((signal_list_ptr_pcc != NULL) && (signal_list_ptr_scc != NULL))
    {
      /* We need to first validate the path index override param and retreive the band */
      if ( (pcc_info->path_override_index < RFC_PATH_MAX) && (scc_info->path_override_index < RFC_PATH_MAX) )
      {
        rfcom_lte_band_type pcc_band_override = RFCOM_BAND_LTE_INVALID;
        rfcom_lte_band_type scc_band_override = RFCOM_BAND_LTE_INVALID;

        /* Loop through path index override to find true band - PCC */
        while ((unsigned int)signal_list_ptr_pcc[0].band_info[i].path_index != RFC_INVALID_PARAM)
        {
          if ((unsigned int)signal_list_ptr_pcc[0].band_info[i].path_index == pcc_info->path_override_index)
          {
            pcc_band_override = (rfcom_lte_band_type)(signal_list_ptr_pcc[0].band_info[i].band);
            break;
          }
          i++;
        }

        /* Loop through path index override to find true band - SCC */
        while ((unsigned int)signal_list_ptr_scc[0].band_info[j].path_index != RFC_INVALID_PARAM)
        {
          if ((unsigned int)signal_list_ptr_scc[0].band_info[j].path_index == scc_info->path_override_index)
          {
            scc_band_override = (rfcom_lte_band_type)(signal_list_ptr_scc[0].band_info[j].band);
            break;
          }
          j++;
        }

        if ( (pcc_band_override == scc_band_override) && (pcc_band_override != RFCOM_BAND_LTE_INVALID) )
        {
          intra_feasible = TRUE;
          *intra_ca_band = pcc_band_override;
          MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "check_intra_ca_feasible: PCC and PCC split bands match: intra_ca_band %d, i %d, j %d", 
                    *intra_ca_band, i , j);
        }
      }
      else
      {  
      /*if pcc bw != ALL_BW, loop on rfc_pcc_bw till row found for the 1st split */
      while (intra_feasible == FALSE)
      {
        /* Loop over PCC rfc data to find 1st split row which matches its bw */
        /* Match the System BW before checking the channels */
          while((signal_list_ptr_pcc[0].band_info[i].sys_bw != pcc_info->rflte_sys_bw) &&
                (signal_list_ptr_pcc[0].band_info[i].sys_bw != RFC_BW_ALL) )
          {
            /* Check to ensure there is no infinite loop */
            if (signal_list_ptr_pcc[0].band_info[i].sys_bw == RFC_INVALID_PARAM)
            {
              MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "check_intra_ca_feasible: Sys bw passed doesn't match the RFC rflte_sys_bw %d ", 
                    pcc_info->rflte_sys_bw);
              break;
            }
            i++;
          }

          /* Break out of Top level While loop if reached end of RFC struct */
          if (signal_list_ptr_pcc[0].band_info[i].sys_bw == RFC_INVALID_PARAM)
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "check_intra_ca_feasible: Reached end of PCC RFC info without a match i %d ", i);
            break;
          }

          /* Loop over SCC rfc data to find 1st split row which matches its bw */

          while( (signal_list_ptr_scc[0].band_info[j].sys_bw != scc_info->rflte_sys_bw) &&
                 (signal_list_ptr_pcc[0].band_info[i].sys_bw != RFC_BW_ALL) )
          {
            /* Check to ensure there is no infinite loop */
            if (signal_list_ptr_scc[0].band_info[j].sys_bw == RFC_INVALID_PARAM)
            {
              MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "check_intra_ca_feasible: Sys bw passed doesn't match the RFC rflte_sys_bw %d ", 
                    scc_info->rflte_sys_bw);
              break;
            }
            j++;
          }

          /* Break out of Top level While loop if reached end of RFC struct */
          if (signal_list_ptr_scc[0].band_info[j].sys_bw == RFC_INVALID_PARAM)
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "check_intra_ca_feasible: Reached end of SCC RFC info without a match j %d ", j);
            break;
          }

          if ((TRUE == debug_flag))
          {
            MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH,"check_intra_ca_feasible:loop variables i: %d, j: %d", i, j);
          }

          /* If pcc rx_chan fits within this split and scc rx_chan fits within this split, intra is possible*/
          if( (((uint32)pcc_info->rflte_chan >= signal_list_ptr_pcc[0].band_info[i].start_chan) && 
                ((uint32)pcc_info->rflte_chan <= signal_list_ptr_pcc[0].band_info[i].stop_chan))
                                                 &&
            (((uint32)scc_info->rflte_chan >= signal_list_ptr_scc[0].band_info[j].start_chan) && 
                ((uint32)scc_info->rflte_chan <= signal_list_ptr_scc[0].band_info[j].stop_chan)) )
          {
            intra_feasible = TRUE;
            /* Additional check to make sure both PCC and SCC matched the same split */
            if (signal_list_ptr_pcc[0].band_info[i].band == signal_list_ptr_scc[0].band_info[j].band)
            {
              *intra_ca_band = (rfcom_lte_band_type) (signal_list_ptr_pcc[0].band_info[i].band);
                MSG_5(MSG_SSID_RF, MSG_LEGACY_HIGH, "check_intra_ca_feasible: PCC and PCC split bands match: pcc_band %d, scc_band %d, intra_ca_band %d, i %d, j %d", 
                    signal_list_ptr_pcc[0].band_info[i].band,
                    signal_list_ptr_scc[0].band_info[j].band,
                      *intra_ca_band, i, j );
            }
            else
            {
              /* Break out of top level while: error condition */
              intra_feasible = FALSE;
              MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "check_intra_ca_feasible: PCC and SCC split bands didn't match: pcc_band %d, scc_band %d ", 
                    signal_list_ptr_pcc[0].band_info[i].band,
                    signal_list_ptr_scc[0].band_info[j].band);
              break;
            }
          }
          else
          {
            i++;
            j++;
            continue;
          }
      } /* while (intra_feasible == FALSE) */
      }
    } /* NULL Check */
    else
    {
      MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "check_intra_ca_feasible: Error retrieving data from rfc: NULL signal ptrs returned!!");
    }
  }
  else /* Non-split bands */
  {
    if (pcc_info->band == scc_info->band)
    {
      intra_feasible = TRUE;
      *intra_ca_band = pcc_info->band;
    }
  }

  MSG_2(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH, "check_intra_ca_feasible: intra_feasible %d, intra_ca_band %d ", intra_feasible, *intra_ca_band);

  return intra_feasible;
}

/*!
  @brief
  This function get partial band range information from RFC.

  @param ptr
  point to LTE parital band range information in RFC
 
  @return
   TRUE if get partial band infomation successfully, else false 
*/

boolean rfc_lte_intf::rfc_get_lte_properties (rfc_lte_properties_type **ptr)
{
   boolean status = FALSE;
   rfc_lte_data *rfc_data = rfc_lte_data::get_instance();
  
  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, " RFC Common data object is NOT created : rfc_common_data == NULL!" );
    return status;
  }

  status = rfc_data->get_lte_properties(ptr);
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enter Critical Section

  @details
  This function will acquire lock to assign critical section for the successding 
  code. The code will be set to critical section  until the lock is released.
  Thus there should not be an attempt to re-acquire the same lock without 
  releasing it. This will result in Deadlock scenario.
 
  @param crit_section
  Variable storing the critical section lock
*/
void rfc_lte_device_enter_critical_section()
{      
   rf_common_enter_critical_section(&rflte_rf_devices_mutex);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable Critical Section

  @details
  This function will release lock which was acquired for the preceecing code.
 
  @param crit_section
  Variable storing the critical section lock
*/
void rfc_lte_device_leave_critical_section()
{      
   rf_common_leave_critical_section(&rflte_rf_devices_mutex);
}

