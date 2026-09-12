/*!
   @file
   rfc_card_lte.cpp

   @brief
   This file contains the implementation of RFC LTE card interfaces

*/

/*===========================================================================

Copyright (c) 2008 - 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:27:23 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_lte.mpss/3.11/rf/rfc/src/rfc_card_lte.cpp#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
01/27/20   ssl     [CR2594829]Moving time profile f3's under debug flag
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
11/18/19   cv      TX alt path support
11/12/19   ssl     FR 56309: RF WU in parellel to MCPM config for HA modem
07/04/19   ssl     Skip vreg vote in Wake up from RFSW LTE side as MCPM Configures the VREG
03/02/18   ks      RFFE based XSW support
11/10/17   pv       LTE resource management in MMCP based on clients
03/23/16   sk      Jacala LTE FBRX tune-away support
02/23/16   sk      Pass FB chain to FBRX module from RFC
02/22/16   sk      Pass Rx band to get FB device
01/27/16   sk      Get LTE Feedback Device from RFC
01/06/16   vkm     F3 optimization - moving to new debug SSID "MSG_SSID_RF_DEBUG"
11/26/15   lhb     FR 31436: API to provide the HW band limitation to LTE RRC
11/17/15    jhap     condition to check tx sleep state to put the signals to LPM
11/04/15   sml     De-couple WTR LPM voting and VREG/WTR device voting and 
                   Add booking keeping of VREG/WTR device voting
11/03/15   vkm     Tracking correct CA mode to tune and meas along with correct altpath
11/02/15    vc     Rx/Tx split changes
10/24/15   stw     MC Level Band Decision maker added to handle split bands
09/28/15   stw     FR28012 :Alternate Path Used to find correct Split Band from RFC for Rx tuning
07/30/15   pkg     Remove Unwanted F3's.
07/25/15   JJ      Added FBRx coupler/switch re-program support
07/10/15   sb      Fix to not return error if TX device is not available
06/25/15   pkg     Changes to support Intra ULCA Tune on bands with splits.
06/16/15   sb      Prerequisite changes for TX c1 calibration 
06/16/15   sb      Update FTM valid device allocation logic
06/08/15   pkg     Fix KW errors related to RFM_MAX_WAN_DEVICES usage.
06/03/15   sb      Initialize local variables correctly while fetching from RFC
05/19/15   pkg     Backout changes to get Tx devices when Preffered Tx device is in use.
05/19/15   pkg     Enable Intra and Inter ULCA in FTM mode.
05/12/15   rcui    pass device to rflte_pm_info
04/17/15   mg      Changes to support Tx Rx Split Band design - 3
04/13/15   lhb     Ported : GRFC Alt Path Support
04/08/15   pkg     Changes to support Tx Rx Split Band design.
12/15/14   ars     Added fix to handle intra CA with split bands where PRx and DRx are on different splits.
12/05/14   dw      Changes to support split bands based on split specified in RFC.
10/20/14   ndb     Remove the Deprecated  API's 
10/09/14   rsr     Fixing compiler warning.
09/18/14   rsr     Changes to return TX LUT index when querying TX LUT info.
08/15/14   st      NS07 Port Update Support
07/31/14   ndb     Added support for extended EARFCN for LTE
07/10/14   jf      TM8 + SRS switching for TDD
07/08/14   gvn     Modify algorithm for split-intraband support
07/02/14   jf      AsDiv support
06/11/14   bar     Added rfc_lte_disable_fb_device() API to support IQ capture
05/15/14   gvn     Add Api for getting overlapping split bands
04/11/14   bsh     Split Band Support
01/09/14   svi     Split Band Support
10/21/13   nrk     Fix compiler warnings
05/21/13   pv      Removed the Temporary work around of disabling B28B split path to avoid crash
05/02/13   pv      Temporarily disabling B28B split path to avoid crash
04/25/13   gvn     Support for Split bands for LTE
04/10/13   kab     Added support for Tx Antenna Swap.
03/12/13   sn      XPT dual WTR fix for feedback path
10/31/12   vss     Change to get device API for returning a list of objects 
08/16/12   pl      Update RFC configuration strucutre to reflect new changes in RFC 
07/30/12   pl      Fix warning
07/27/12   sr      renamed rfdevice related enum types.
07/18/12   sr      added get_device api to return the rf device obj ptr
                   based on logical_path, rx_tx, band and device_type.
06/06/12   pl      Update RFC interface to take DEVICE instead of PATH
06/01/12   aca     Fix to clean up Tx signals when disable tx is called
04/24/12   cd      Fixed LTE exit mode to restore init settings for both Tx and Rx band GRFC/ANT_SELs
02/28/12   sr      made changes to put all the GPIOs into low power mode during sleep.
12/05/11   tnt     Add rfc_lte_mode_sleep() api
11/05/11   aak     Enable NPA calls from RFSW for power management on 9x15  
08/25/11   aak     Enable RF NPA voting only for MSM8960 
06/29/11   pl      Added path and band information in mode_exit()
06/23/11   pl      added mode_exit() interface
06/23/11   pl      Rename config_mode and get_config_mode_data
04/26/11   tnt     Porting to new vreg management interface
04/13/11   aak     Revert name to rfc_lte_manage_vregs()
04/13/11   aak     Change rfc_lte_manage_vregs() to rfc_lte_manage_power()
01/13/11   pl      Use rfcom_lte_band_type instead of rf_card_band_type
01/05/11   pl      Added band parameter to rfc_lte_get_rx/tx_band_config()
12/15/10   pl      Renamed to a c++ file
12/15/10   pl      Moves to call C++ rfc_lte class
07/08/09   av      Cleanup and added port configuration (RTR) data  in RFC
11/21/08   dyc     Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
extern "C" {
#include "rfc_card.h"
#include "rfc_card_lte.h"
#include "rfcommon_core_error.h"
#include "rfcommon_core.h"
#include "mdsp_intf.h"
#include "rflte_util.h"
#include "rfcommon_core_utils.h"
#include "rfc_common_data.h"
}
#include "rfc_lte.h"
#include "rflte_mc.h"
#include "rfc_lte_data.h"
#include "rflte_mc_rfc_interface.h"
#include "rfcommon_asdiv_manager.h"
#include "rflte_state.h"


#define IS_BIT_SET(mask, position)   ((((mask) >> (position)) & 0x1) == 1)
#define BAND_MASK_ARRAY_SIZE 3

/*structure for rflte to contain pm configuration info*/
/*This global should only be refered in this file*/
rfc_power_management_info rflte_pm_info;
/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function implements the card specific intializations for LTE mode

  @details


*/
void rfc_lte_config_mode_data( rfm_device_enum_type device )
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_config_mode_data:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->mode_config(device);
} /* rfc_lte_config_mode_data() */

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function implements the card specific intializations for LTE mode

  @details


*/
void* rfc_lte_get_mode_config(rfm_device_enum_type device, 
                              rfc_mode_config_data_enum_type config_data,
                              uint8 alt_path )
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_mode_config:RFC LTE object not initialized");
    return NULL;
  }

  return(rfc_lte_p->mode_config_get(device, config_data, alt_path));
}
/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function implements the card specific intializations for rx band
  changes.

  @details
  The band table which this RF card supports is not indexed in any way.
  Explicit logic will need to be provided to extract the band specific data
  structures.

*/
void rfc_lte_reset_grfc_band_config(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_reset_grfc_band_config:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->reset_grfc_band_config(device, band, alt_path);
}
/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function implements the card specific intializations for rx band
  changes.

  @details
  The band table which this RF card supports is not indexed in any way.
  Explicit logic will need to be provided to extract the band specific data
  structures.

*/
void rfc_lte_config_rx_band_data(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_config_rx_band_data:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->rx_band_config(device, band, alt_path);
} /* rfc_lte_config_rx_band_data() */

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function returns top level ptr to the current Rx band configuration.

  @details
  The function will return Rx configuration such as GPIO/GRFC settings along
  with any other parameter data structures for a particular band.  The void*
  allows for casting for usage under various band types.

  Dependency that rfc_set_band() be called before using this function.

  @parameter
  path request Rx path
  config_data type of data to obtain

  @Returns
  void * ptr of the current Rx band configuration ptr.

*/
void *rfc_lte_get_rx_band_config
(
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfc_band_config_data_enum_type config_data,
  uint8  alt_path
)
{
  void *data_config_ptr = NULL;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_rx_band_config:RFC LTE object not initialized");
    return data_config_ptr;
  }

  data_config_ptr = rfc_lte_p->rx_band_config_get(device, band, config_data, alt_path);

  return (data_config_ptr);
} /* rfc_lte_get_rx_band_config() */

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function implements the card specific intializations for tx band
  changes.

  Needed only for TDD B40 on RFC_8853_B
  Notneeded on RFC_8853_A

  @details
  The band table which this RF card supports is not indexed in any way.
  Explicit logic will need to be provided to extract the band specific data
  structures.

*/
void rfc_lte_config_tx_band_data(rfm_device_enum_type device, rfcom_lte_band_type band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_config_tx_band_data:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->tx_band_config(device, band);
} /* rfc_lte_config_tx_band_data() */

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function returns top level ptr to the current Tx band configuration.

  @details
  The function will return Tx configuration such as GPIO/GRFC settings along
  with any other parameter data structures for a particular band.  The void*
  allows for casting for usage under various band types.

  Dependency that rfc_set_band() be called before using this function.

  @parameter
  path request Rx path
  config_data type of data to obtain

  @Returns
  void * ptr of the current Tx band configuration ptr.

*/
void *rfc_lte_get_tx_band_config
(
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfc_band_config_data_enum_type config_data
)
{
  void *data_config_ptr = NULL;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_tx_band_config:RFC LTE object not initialized");
    return data_config_ptr;
  }

  data_config_ptr = rfc_lte_p->tx_band_config_get(device, band ,config_data);

  return (data_config_ptr);
} /* rfc_lte_get_tx_band_config() */

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
void *rfc_lte_get_meas_data
(
   rfm_device_enum_type device,
   rfcom_lte_band_type band,
   rfc_band_config_data_enum_type config_data
)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_config_mode_data:RFC LTE object not initialized");
    return NULL;
  }

  return ( rfc_lte_p->rx_band_config_get(device, band, config_data, RFLTE_MC_DEFAULT_ALTERNATE_PATH) );
} /* rfc_lte_get_meas_data() */

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
void rfc_lte_manage_vregs(uint8 sub_id,rfm_device_enum_type device, rfcom_lte_band_type band, rfm_path_state state)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  rf_path_enum_type path = rfcommon_core_lte_device_to_path(device);
  rf_time_tick_type begin;
  rf_time_type elapsed;

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_manage_vreg:RFC LTE object not initialized");
    return;
  }
  if(path > RF_PATH_MAX)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
          "rfc_lte_manage_vregs:RFC LTE Wrong device %d, Path %d", 
          device, path );
    return;
  }

  if(enable_mc_time_profile == TRUE)
  {
   begin = rf_time_get_tick ();
  }
  /* Protect the rfc sleep/wakeup functions with critical section */
  rf_common_enter_critical_section(rfc_get_lock_data());

  if ( (state == RF_PATH_SLEEP_STATE) || (state == RF_PATH_TX_SLEEP_STATE))
  {
    /* disable the rf signals, put the signals into low power mode */
    rfc_common_sleep(path, RFM_LTE_MODE);
  }
  else
  {
    /* enable the rf signals, if the signals are in low power mode */
    rfc_common_wakeup(path, RFM_LTE_MODE);
  }
  
  rf_common_leave_critical_section(rfc_get_lock_data());

  if(enable_mc_time_profile == TRUE)
  {
   elapsed = rf_time_get_elapsed ( begin, RF_USEC );

   MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
          " rfc_lte_manage_vregs:rfc_common_wakeup/sleep: path_idx %d, time elapsed = %d us",
          path,elapsed );
  }


//if(rflte_state_get_rx_wakeup_script_opt_flag_status(sub_id,RFLTE_MC_PCELL_IDX) == FALSE)
  {
  begin = rf_time_get_tick ();

  /*rflte_pm_info structure is filled out here so all cards version can reference */
  rflte_pm_info.mode = (sub_id)?(RFM_LTE_MODE_2):(RFM_LTE_MODE);
  rflte_pm_info.band = band;
  rflte_pm_info.path = path;
  rflte_pm_info.state = state;
  rflte_pm_info.rf_device = device;  /*need to change for sglte in future*/

  rfc_lte_p->manage_vregs(&rflte_pm_info);

  elapsed = rf_time_get_elapsed ( begin, RF_USEC );

  MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
         " rfc_lte_manage_vregs:rfc_lte_p->manage_vregs: path_idx %d, time elapsed = %d us",
           path,elapsed );
  }
} /* rfc_lte_manage_vreg() */

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
int32 rfc_lte_command_dispatch( rfm_device_enum_type device, int32 cmd, void *data)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_command_dispatch:RFC LTE object not initialized");
    return 0;
  }

  return ( rfc_lte_p->command_dispatch(device, cmd,data) );
} /* rfc_lte_command_dispatch() */

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
void rfc_lte_tx_disable(rfm_device_enum_type device, rfcom_lte_band_type band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_tx_disable:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->tx_disable(device, band);
}

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
void rfc_lte_mode_sleep(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_mode_sleep:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->mode_sleep(device, band, alt_path);
}

/* ----------------------------------------------------------------------- */
/*!
  @brief

  @details


*/
void rfc_lte_mode_exit(rfm_device_enum_type device, rfcom_lte_band_type rx_band, rfcom_lte_band_type tx_band, uint8 alt_path)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_mode_exit:RFC LTE object not initialized");
    return;
  }

  rfc_lte_p->mode_exit(device, rx_band, tx_band, alt_path);
}

void** rfc_lte_get_device(rfm_device_enum_type rfm_dev, rfc_rxtx_enum_type rx_tx,
                                   rfcom_lte_band_type band, rfdevice_type_enum_type dev_type)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
    return NULL;
  }

  return (rfc_lte_p->get_rf_device_obj(rfm_dev, rx_tx, band, dev_type ));
}

boolean rfc_lte_setup_fb_path (rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable)
{
  boolean api_status = TRUE ;
  rfc_lte_intf *rfc_obj = rfc_lte_intf::Get_Instance();
	
  api_status = rfc_obj->setup_fb_path(device_fb, band, et_enable);
	
  return(api_status);
	
}

/* ----------------------------------------------------------------------- */
/*!
  @brief
      This API is used to get the fb path scripts (switch and asm)
  @details
      It's simliar as rfc_lte_setup_fb_path, but that api get script and also
      execute the script immediately, this api only get script
*/

void* rfc_lte_get_fb_path_config (rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable)
{
  rfc_sig_cfg_type *signal_list_ptr = NULL;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return NULL ;
  }

  signal_list_ptr = 
    (rfc_sig_cfg_type *) rfc_lte_p->get_fb_path_config( device_fb, band, et_enable );

  return signal_list_ptr;

}

rfm_device_enum_type rfc_lte_get_fb_device(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band, rfcom_lte_band_type rx_band)
{
  rfm_device_enum_type fb_device = RFM_INVALID_DEVICE;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return RFM_INVALID_DEVICE ;
  }

  fb_device = rfc_lte_p->get_fb_device( tx_device, tx_band, rx_band);

  return fb_device;

}

uint32 rfc_lte_get_fb_chain(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band)
{
  uint32 fb_modem_chain = 3; // Default Chain on Jacala
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return RFM_INVALID_DEVICE ;
  }

  fb_modem_chain = rfc_lte_p->get_fb_chain( tx_device, tx_band);

  return fb_modem_chain;

}

/* ----------------------------------------------------------------------- */
/*!
  @brief
      This API is used to check if fb_device conflicts with the rx device passed
  @details
      FB device is obtained from RFC based on Tx Device and band and if 
      FB device is same as Rx device that is passed in then coflict flag is returned as TRUE
*/

#ifdef FEATURE_RF_HAS_WTR3925

boolean rfc_lte_is_fb_device_conflict(rfm_device_enum_type rx_device, rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band, rfcom_lte_band_type rx_band)
{
  rfm_device_enum_type fb_device = RFM_INVALID_DEVICE;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  boolean conflict_flag = FALSE;

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return RFM_INVALID_DEVICE ;
  }

  fb_device = rfc_lte_p->get_fb_device( tx_device, tx_band,rx_band);

  if (rx_device == fb_device)
  {
    conflict_flag = TRUE;
  }

  return conflict_flag;

}
#endif

boolean rfc_lte_setup_fb_device (rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_freq)
{
  boolean api_status = TRUE ;
  rfc_lte_intf *rfc_obj = rfc_lte_intf::Get_Instance();
	
  api_status = rfc_obj->setup_fb_device(device, device_fb, band, tx_freq);
	
  return(api_status);
	
}

boolean rfc_lte_disable_fb_device (rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_freq)
{
  boolean api_status = TRUE ;
  rfc_lte_intf *rfc_obj = rfc_lte_intf::Get_Instance();
	
  api_status = rfc_obj->disable_fb_device(device, device_fb, band, tx_freq);
	
  return(api_status);
	
}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.

   @param
   split_band: Pointer to boolean which will be set to true if split band is found
   rx_chan: earfcn passed
   rfc_band: pointer to lte band, which will be updated if split band

*/

void rfc_lte_get_rx_band(boolean *split_band, rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type *rfc_band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_rx_band(split_band, rx_chan, rfc_band);
  }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the updated tx device info.

   @param
   rfm_dev: tx device which needs to be re-configured
   curr_band: band for path override
   alt_path: alternate path to set

*/
void rfc_lte_update_tx_devices(rfm_device_enum_type rfm_dev, rfcom_lte_band_type curr_band, uint8 alt_path)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_updated_tx_port(rfm_dev, curr_band, alt_path);
	MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:alt_path %d", alt_path);
  }
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the tx band info. for the specified tx channel by querying RFC.

   @param
   split_band: Pointer to boolean which will be set to true if split band is found
   tx_chan: earfcn passed
   rfc_band: pointer to lte band, which will be updated if split band

*/
void rfc_lte_get_tx_band(boolean *split_band, rfcom_lte_earfcn_type tx_chan, rfcom_lte_band_type *rfc_band, boolean filtered_band)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_tx_band(split_band, tx_chan, rfc_band, filtered_band);
  }
}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the low and high freq for the split band
 
   @param
   split_band: band for which low and high channels are desired
   *low_earfcn: Pointer to low chan
   *high_earfcn: Pointer to high chan
 
   @return

*/
void rfc_lte_get_rx_split_band_earfcns(rfcom_lte_band_type split_band, rfcom_lte_earfcn_type *low_earfcn, rfcom_lte_earfcn_type *high_earfcn)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_rx_split_band_earfcns:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_rx_hi_lo_earfcn(split_band, low_earfcn, high_earfcn);
  }
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the low and high freq for the split band
 
   @param
   split_band: band for which low and high channels are desired
   *low_earfcn: Pointer to low chan
   *high_earfcn: Pointer to high chan
 
   @return

*/
void rfc_lte_get_tx_split_band_earfcns(rfcom_lte_band_type split_band, 
                                    rfcom_lte_earfcn_type *low_earfcn, 
                                    rfcom_lte_earfcn_type *high_earfcn)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, 
        MSG_LEGACY_ERROR, 
        "rfc_lte_get_tx_split_band_earfcns:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_tx_hi_lo_earfcn(split_band, low_earfcn, high_earfcn);
  }
}

rfm_device_enum_type rfc_lte_get_mapped_device_for_scell
(
 rfcom_lte_band_type pcell_band,
 rfcom_lte_band_type scell_band,
 rfm_device_enum_type scell_rfm_device
)
{
  rfm_device_enum_type result_rfm_device = RFM_INVALID_DEVICE;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_mapped_device_for_scell:RFC LTE object not initialized");
    return scell_rfm_device;
  }

  result_rfm_device = rfc_lte_p->get_mapped_device_for_scell(pcell_band,
                                                             scell_band,
                                                             scell_rfm_device);

  return result_rfm_device;
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
void rfc_lte_get_band_based_on_list(rflte_mc_band_decision_config_params_s_type* input_list, uint32 list_size)
{
   rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
   if(rfc_lte_p == NULL)
   {
      MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, 
          "rfc_lte_get_band_based_on_list:RFC LTE object not initialized");
   }
   else
   {
    rfc_lte_p->get_band_based_on_list(input_list,list_size);
   }
}


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
   rx_chan: tx channel
   @param 
   curr_bw: current system bw
   @param 
   path_index_override: Path override index to match the Tx path on the Band  
   @param 
   *rfc_band: updated rfc band.

*/

void rfc_lte_get_rx_band_v2(boolean *split_band, 
                            rfcom_device_enum_type device,
                            rfcom_lte_earfcn_type rx_chan,
                            rfc_lte_bw_type curr_bw,
                            uint8 path_index_override,
                            rfcom_lte_band_type *rfc_band,
                            uint8 alt_path)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, 
        "rfc_lte_get_rx_band_v2:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_rx_band_v2(split_band, device, rx_chan, curr_bw, path_index_override, rfc_band, alt_path);
  }
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
void rfc_lte_get_tx_band_v2(boolean *split_band, 
                            rfcom_lte_earfcn_type tx_chan,
                            rfc_lte_bw_type curr_bw,
                            uint8 path_index_override,
                            rfcom_lte_band_type *rfc_band,
                            boolean filtered_band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, 
        "rfc_lte_get_tx_band_v2:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_tx_band_v2(split_band, 
                              tx_chan, 
                              curr_bw, 
                              path_index_override, 
                              rfc_band, 
                              filtered_band);
  }

}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.

   @param
   split_band: Pointer to boolean which will be set to true if split band is found
   rx_chan: earfcn passed
   rfc_band: pointer to lte band, which will be updated if split band

*/

void rfc_lte_get_split_rx_bands(boolean *split_band, rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type *rfc_band1, rfcom_lte_band_type *rfc_band2)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_split_rx_bands((boolean *)split_band, rx_chan, (rfcom_lte_band_type *)rfc_band1, (rfcom_lte_band_type *)rfc_band2);
  }
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.

   @param
   split_band: Pointer to boolean which will be set to true if split band is found
   rx_chan: earfcn passed
   rfc_band: pointer to lte band, which will be updated if split band

*/

void rfc_lte_get_rx_split_band_channel_rasters(rfcom_lte_band_type split_band, 
                                               rfcom_lte_earfcn_type *low_earfcn,
                                               rfcom_lte_earfcn_type *high_earfcn,
                                               rfcom_lte_band_type *band_list,
                                               uint8 *num_of_splits)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_rx_split_band_channel_rasters(split_band, low_earfcn, high_earfcn, band_list, num_of_splits);
  }
}

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function generates the AsDiv GRFCs script for the requested device
  and band.

  @param position
  RF device for which to generate GRFC script
 
  @param cb_data
  Call back data to pass in additional info for extendability

  @param asdiv_script_ptr
  Output - The GRFC script
 
  @return
  True on success and FALSE on failure
*/

#ifdef FEATURE_RF_ASDIV  
boolean rfc_lte_get_asdiv_xsw_script
( 
  uint32 position,
  rf_buffer_intf *asdiv_script_ptr,
  void* cb_data
)
{
  boolean status;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();


  if ( asdiv_script_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "generate_asdiv_xsw_script : scrpit ptr is "
                      "null"); 
    return FALSE;
  }

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return FALSE ;
  }

  status = rfc_lte_p->generate_asdiv_xsw_script((int)position ,asdiv_script_ptr, cb_data);


  if( status == FALSE )
  {
    RF_MSG( RF_ERROR, "rfc_lte_get_asdiv_xsw_script : "
                      "script generation failed");
  }

  return status;
} /* rfc_lte_get_asdiv_xsw_script */

boolean rfc_lte_get_xsw_sleep_script
( 
  uint32 position,
  rf_buffer_intf *asdiv_script_ptr,
  void* cb_data
)
{
  boolean status;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();


  if ( asdiv_script_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_lte_get_xsw_sleep_script : scrpit ptr is "
                      "null"); 
    return FALSE;
  }

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return FALSE ;
  }

  status = rfc_lte_p->generate_xsw_sleep_script( (rfcommon_asdiv_position_type)position ,asdiv_script_ptr, cb_data);


  if( status == FALSE )
  {
    RF_MSG( RF_ERROR, "rfc_lte_get_xsw_sleep_script : "
                      "script generation failed");
  }

  return status;
} /* rfc_lte_get_asdiv_xsw_script */
#endif

void* rfc_lte_get_asd_xsw_config
(
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  uint32 position
)
{
  rfc_sig_cfg_type *rfc_asd_sig_info = NULL;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if ( rfc_lte_p == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"rfc_lte_p is NOT created. ",0);
    return rfc_asd_sig_info ;
  }

  rfc_asd_sig_info = 
    (rfc_sig_cfg_type *) rfc_lte_p->get_asd_xsw_config( device, band, (int)position );


  return rfc_asd_sig_info;

}
/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function generates the AsDiv GRFCs script for the requested device
   and band.
  
   @param position
   RF device for which to generate GRFC script
  
   @param cb_data
   Call back data to pass in additional info for extendability
  
   @param asdiv_script_ptr
   Output - The GRFC script
  
   @return
   True on success and FALSE on failure
*/
boolean rfc_lte_get_srs_xsw_script( uint32 position,
                                    rf_buffer_intf *srs_xsw_script_ptr,
                                    rfm_device_enum_type device,
                                    rfcom_lte_band_type band,
                                    boolean immediate)
{
  boolean status = TRUE;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  if ( srs_xsw_script_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "generate_srs_xsw_script : scrpit ptr is null"); 
    status = FALSE;
  }
  else
  {
     if ( rfc_lte_p == NULL )
     {
       MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"LTE RFC Obj is NOT created. ",0);
       status = FALSE;
     }
     else
     {
        status = rfc_lte_p->generate_srs_xsw_script((int)position,
                                                    srs_xsw_script_ptr,
                                                    device,
                                                    band,
                                                    immediate);
     }
  }

  if( status == FALSE )
  {
    RF_MSG( RF_ERROR, "rfc_lte_get_srs_xsw_script : "
                      "script generation failed");
  }

  return status;
} /* rfc_lte_get_srs_xsw_script */

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function updates device ports with NS07 specific settings

   @param
   

*/

void rfc_lte_update_ns07_ports(rfm_device_enum_type rfm_dev,rfcom_lte_band_type curr_band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    rfc_lte_p->get_ns07_ports(rfm_dev,curr_band);
  }
}

rfdevice_rxtx_common_class * rfc_lte_get_common_rxtx_device
(
  rfm_device_enum_type device, 
  rfcom_lte_band_type band,
  uint32 alt_path
)
{
  rfc_device_info_type *device_info_ptr = NULL;
  /*Device type used to extract tech specific object*/
  rfdevice_type_enum_type dev_type = RFDEVICE_TYPE_INVALID;
  /*Device instance used to extract tech specific object*/
  uint32 instance = 0;
  rfc_cfg_params_type cfg;

  cfg.alternate_path = alt_path;
  cfg.band = band;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_TX;
  cfg.path = rfcommon_core_device_to_path(device);

  /* Get RFC information */
  if (rfc_lte_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
     instance = device_info_ptr->rf_asic_info[0].instance;
     /*Retrieve device object*/
     return rfc_common_get_cmn_device_object((uint8)instance);
  }
  else
  {
     MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Could not get device info for LTE band  %d",band);
     return NULL;
  }
}

/* ----------------------------------------------------------------------- */
/*!
  @brief
  This function gets the split bands for a particular 
  band.

  @param full_band
  Band for which split band info is required
 
  @return
  Value with bits corresponding to present split bands set to 1. 
  0 if no split bands.
*/
uint64 rfc_lte_get_split_bands(rfcom_lte_band_type full_band)
{
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
  uint64 split_bands = 0;

  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_split_bands:RFC LTE object not initialized");
  }
  else
  {
    split_bands = rfc_lte_p->get_split_bands(full_band);
  }

  return split_bands;
  
}/* rfc_lte_get_split_bands(rfcom_lte_band_type full_band) */

/* ----------------------------------------------------------------------- */
/*!
   @brief
   This function returns the rx band info. for the specified rx channel by querying RFC.

   @param pcc_info
   PCell carrier info pointer.
 
   @param scc_info
   SCell carrier info pointer.
 
   @param intra_ca_split_band
   Pointer to lte band, which will be updated if split band is feasible.

*/

boolean rfc_lte_check_intra_ca_feasible(void *pcc_info, 
                                        void *scc_info,
                                        rfcom_lte_band_type *intra_ca_split_band,
                                        rfc_rxtx_enum_type config_type)
{
  boolean intra_band = FALSE;
  rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();

  MSG(MSG_SSID_RF_DEBUG, MSG_LEGACY_HIGH, "rfc_lte_check_intra_ca_feasible: check in");  
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_device:RFC LTE object not initialized");
  }
  else
  {
    intra_band = rfc_lte_p->check_intra_ca_feasible((rflte_mc_carrier_info_rfc_type*)pcc_info, 
                                                    (rflte_mc_carrier_info_rfc_type*)scc_info, 
                                                    intra_ca_split_band,
                                                    config_type);
  }
  return intra_band;
}


boolean rfc_lte_get_associated_tx_device(rfm_device_enum_type device, 
                                         rfcom_lte_band_type band, 
                                         rfm_device_enum_type* tx_device)
{


  rfc_common_data *rfc_data = rfc_common_data::get_instance();

  if (NULL == rfc_data)
  {
    RF_MSG( RF_ERROR, "rfc_lte_get_associated_tx_device - NULL RFC common data object." );
    return FALSE;
  }

  *tx_device = rfc_data->rfc_get_preferred_associated_device( device, RFC_CONFIG_TX );

  return TRUE;
}

/*Expectation is that PCELL is carrier index 0, Scell1 is carrier index 1, Scell2 is carrier index 2 
  curr_carr_mask indicates which carriers are active when API was called. The Bit which represents a carrier
  is located at the carrier index from LSB.
  */
boolean rfc_lte_get_valid_devices(rflte_mc_cell_idx_type carrier_index,
                                  rfcom_lte_band_type band,
                                  rflte_mc_device_info_type *ret_params,
                                  uint16 devices_used_mask)
{
  uint8 carr_count;
  uint8 carr_index =  (uint8)carrier_index;
  rfm_devices_configuration_type dev_cfg;
  uint8 band_mask_index = 0;
  uint8 relative_bit = 0;
  uint64 sys_band;
  rfm_device_enum_type temp_drx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type temp_tx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type pref_prx_device = RFM_INVALID_DEVICE;
  boolean tx_device_found = FALSE;
  uint8 dev_index;
  uint8 dev_loop_index;
  boolean status = TRUE;
  boolean valid_devices_found = FALSE;
  boolean temp_flag = TRUE;
  rfcom_band_type_u band_u;
  uint8 index = 0;
  uint8 dev_index_arr[RFM_MAX_WAN_DEVICES]; //Contains order of device indices in priority to be validated

  band_u.lte_band = band;
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_LTE_MODE, band_u);

  if(ret_params == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_valid_devices failed. NULL pointer passed");
    return FALSE;
  }
  if(!rfc_get_devices_configuration(&dev_cfg))
  {

    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_valid_devices failed ");
    return FALSE;
  }

  //find array index in band mask
  band_mask_index = sys_band / (sizeof(sys_band) * 8);
  relative_bit =  sys_band % (sizeof(sys_band) * 8);
  MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "Tx_Rx_Split:rfc_lte_get_valid_devices: band_mask_index = %d and relative bit = %d", band_mask_index, relative_bit, 0);


  /*Initialize array containing device indices in order which they will be validated */
  for(dev_index = 0; dev_index < RFM_MAX_WAN_DEVICES; dev_index++)
  {
    dev_index_arr[dev_index] = dev_index;
  }

  pref_prx_device = rfc_common_get_preferred_rx_device((sys_band_class_e_type)sys_band);

  /*Reorder device index array with preferred RX device as the first one in the array */
  if((uint8)pref_prx_device < RFM_MAX_WAN_DEVICES)
  {
    for(dev_index = (uint8)pref_prx_device; dev_index > 0; dev_index--)
    {
      dev_index_arr[dev_index] = dev_index_arr[dev_index - 1];
    }

    dev_index_arr[0] = (uint8)pref_prx_device;
  }


  MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "Tx_Rx_Split:rfc_lte_get_valid_devices: band_mask_index = %d and relative bit = %d, preferred prx dev:%d", band_mask_index, relative_bit, pref_prx_device);


  //Carrier loop
  for(carr_count = 0; carr_count < RFLTE_MC_MAX_CELL_IDX_SUPPORTED; carr_count++)
  {
    /*If carrier is the same as queried*/
    if(carr_count == carr_index)
    {

      //loop through the list of logical devices to determine valid set of devices for given band
      for(dev_loop_index = 0; dev_loop_index < RFM_MAX_WAN_DEVICES; dev_loop_index++)
      {

        dev_index = dev_index_arr[dev_loop_index];

        //Check band supported and RX supported and device not used already
        if(IS_BIT_SET(dev_cfg.device_support[dev_index].bands_supported.mask[band_mask_index], relative_bit) &&
           ((dev_cfg.device_support[dev_index].capability & RFM_DEVICE_RX_SUPPORTED) == RFM_DEVICE_RX_SUPPORTED) &&
           !IS_BIT_SET(devices_used_mask, dev_index))
        {
          // Associated RX device as DRX
          temp_drx_device = dev_cfg.device_support[dev_index].preferred_associated_rx_device;

          //If DRX device has been used, not a valid combo of PRX, DRX, TX
          valid_devices_found = IS_BIT_SET(devices_used_mask, (uint8)temp_drx_device) ? FALSE : TRUE;

          valid_devices_found &= (temp_drx_device < RFM_MAX_WAN_DEVICES) ? TRUE : FALSE;

          if(valid_devices_found)
          {
            //Check band is still supported on associated DRX
            valid_devices_found &= IS_BIT_SET(dev_cfg.device_support[temp_drx_device].bands_supported.mask[band_mask_index], relative_bit);
          }

          MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "Tx_Rx_Split:rfc_lte_get_valid_devices: PRX device:%d, Associatd DRX device = %d, is DRX device valid:%d", 
                dev_index, 
                temp_drx_device,
                valid_devices_found);

          //get Associated TX device if available
          if(rfc_lte_get_associated_tx_device((rfm_device_enum_type)dev_index,
                                              band,
                                              &temp_tx_device))
          {

            band_u.lte_band = band;			
            sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_LTE_MODE , band_u);
            temp_tx_device = rfc_common_get_preferred_tx_device( (sys_band_class_e_type) sys_band);

            // Check TX device is not used, if not invalid
            // It can be invalid if no associated TX device
            if(temp_tx_device < RFM_MAX_WAN_DEVICES)
            {
              temp_flag &= IS_BIT_SET(devices_used_mask, (uint8)temp_tx_device) ? FALSE : TRUE;

              MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH,
                    "Tx_Rx_Split:rfc_lte_get_valid_devices: dev used mask: %d, associated Tx device: %d, Is tx device unique? :%d",
                    devices_used_mask,
                    temp_tx_device,
                    temp_flag);

              //If TX device found is already used, try other TX device if available. If not, flag an error and move on
              if(!temp_flag)
              {
                //loop through logical devices tx band mask to fetch any other available unique TX device
                for(index = 0; index < RFM_MAX_WAN_DEVICES; index++)
                {
                  if(rflte_util_check_tx_dev_on_band((rfm_device_enum_type)index, band))
                  {
                    //if device is not same as already used preferred TX device and its not in the used devices mask,
                    //select device as new TX device
                    if((index != temp_tx_device) && !IS_BIT_SET(devices_used_mask, index))
                    {
                      //other TX device found for the band. Update TX device.
                      temp_tx_device = (rfm_device_enum_type)index;

                      tx_device_found = TRUE;
                      MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH,
                            "Tx_Rx_Split:rfc_lte_get_valid_devices: dev used mask: %d, new tx device found: %d",
                            devices_used_mask,
                            index);
                      break;
                    } else
                    {
                      MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH,
                            "Tx_Rx_Split:rfc_lte_get_valid_devices:  tx dev: %d has already been used in mask:%d",
                            index,
                            devices_used_mask
                            );
                    }
                  }
                }
              } 
              else
              {
                tx_device_found = IS_BIT_SET(dev_cfg.device_support[temp_tx_device].bands_supported.mask[band_mask_index], relative_bit);

                if(!tx_device_found)
                {
                  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfc_lte_get_valid_devices: associated TX device:%d is not supported on band:%d", temp_tx_device, band);
                }
              }
            }
          }


          //If valid devices found, update return params for the given carrier
          if(valid_devices_found)
          {
            ret_params->device[RF_LTE_PRX] =  (rfm_device_enum_type)dev_index;
            ret_params->device[RF_LTE_DRX] =  temp_drx_device;

            if(tx_device_found)
            {
              ret_params->device[RF_LTE_TX] =  temp_tx_device;
            }

            MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfc_lte_get_valid_devices: prx dev:%d, drx dev:%d, tx dev:%d", ret_params->device[RF_LTE_PRX], ret_params->device[RF_LTE_DRX], ret_params->device[RF_LTE_TX]);
            break;
          }
        }
      } // for(dev_loop_index = 0; dev_loop_index < RFM_MAX_WAN_DEVICES; dev_loop_index++)
    } // if(carr_count == carr_index)
  } //Carrier loop

  status = (TRUE == valid_devices_found) ? TRUE : FALSE;

  return status;
}

boolean rfc_lte_validate_devices( rfcom_lte_band_type band,
                                  rflte_mc_device_info_type* input_params,
                                  uint16 devices_used_mask)
{

  uint64 sys_band = rflte_util_get_band_mask(band);
  rfm_devices_configuration_type dev_cfg;
  uint8 dev_index;
  rfm_device_enum_type temp_dev;
  boolean status = TRUE;
  uint8 band_mask_index;
  uint8 relative_bit;
  rfcom_band_type_u band_u;
  band_u.lte_band = band; 

  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_LTE_MODE, band_u);
  if(input_params == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices failed. NULL pointer passed");
    return FALSE;
  }

  if(!rfc_get_devices_configuration( &dev_cfg ))
  {

    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices failed ");
    return FALSE;
  }

  //find array index
  band_mask_index = (sys_band / (sizeof(sys_band) * 8));
  relative_bit =  sys_band % (sizeof(sys_band) * 8);
  
  if (band_mask_index > BAND_MASK_ARRAY_SIZE - 1) {

     MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices, band_mask_index:%d, relative_bit:%d", band_mask_index, relative_bit); 
     return FALSE;
  }

  //Support for non associated devices

  for(dev_index = 0; dev_index < RF_LTE_MAX_DEVICES; dev_index++)
  {
    temp_dev = input_params->device[dev_index];
    if (temp_dev < RFM_MAX_WAN_DEVICES)
    {

       MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices, cond0:%d, cond1:%d", IS_BIT_SET(dev_cfg.device_support[temp_dev].bands_supported.mask[band_mask_index], relative_bit), (!IS_BIT_SET(devices_used_mask, temp_dev))); 

      if(IS_BIT_SET(dev_cfg.device_support[temp_dev].bands_supported.mask[band_mask_index], relative_bit) && 
         (!IS_BIT_SET (devices_used_mask, temp_dev)))
      {
        switch(dev_index)
        {
          case RF_LTE_TX:
             status &=  ((dev_cfg.device_support[temp_dev].capability & RFM_DEVICE_TX_SUPPORTED) == RFM_DEVICE_TX_SUPPORTED) ? TRUE : FALSE; 
          break;
          default:
          /*Dummy RX devices use alt path >= 100*/
          if (input_params->alt_path < RFC_PSEUDO_ALT_PATH_START_IDX)
          {
             status &= ((dev_cfg.device_support[temp_dev].capability & RFM_DEVICE_RX_SUPPORTED) == RFM_DEVICE_RX_SUPPORTED) ? TRUE : FALSE; 
          }
          break;
        }

        if(!status)
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices, device:%d not valid",temp_dev);
        }
      }
      else 
      {
         status &= FALSE;

         MSG_4(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices, device:%d not supported as dev type:%d on RF com band:%d, sys band:%d", temp_dev, dev_index, band, sys_band);

         MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_validate_devices, devices_used_mask:%d, band_mask_index:%d, bit location:%d", devices_used_mask, band_mask_index, relative_bit); 
         break; 
      }
    }
  }
  return status;

}

boolean rfc_lte_get_band_supported_device(rfcom_lte_band_type rf_band, rfm_device_enum_type* device, rfc_rxtx_enum_type rx_tx)
{
  uint8 index = 0;
  boolean status = FALSE;
  if(device == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_band_supported_device: Device pointer passed is NULL");
    return FALSE;
}
  *device = RFM_INVALID_DEVICE;

  if(rx_tx == RFC_CONFIG_RX)
  {
    for (index = 0; index < RFM_MAX_DEVICES; index++)
    {
      if(rflte_util_check_rx_dev_on_band((rfm_device_enum_type)index, rf_band))
      {
        *device = (rfm_device_enum_type)index;
        status = TRUE;
        break;
      }
    }
  }
  else if (rx_tx == RFC_CONFIG_TX)
  {
    for (index = 0; index < RFM_MAX_DEVICES; index++)
    {
      if(rflte_util_check_tx_dev_on_band((rfm_device_enum_type)index, rf_band))
      {
        *device = (rfm_device_enum_type)index;
        status = TRUE;
        break;
      }
    }
  }
  else
  {
    status = FALSE;
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_lte_get_band_supported_device: Invalid RxTx passed");
  }

  return status;
}


boolean rfc_lte_get_partial_band_info (rfc_lte_properties_type **ptr)	
{
    boolean status = TRUE;
    rfc_lte_intf *rfc_lte_p = rfc_lte_intf::Get_Instance();
	
  if(rfc_lte_p == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, 
        "rfc_lte_get_rx_band_v2:RFC LTE object not initialized");
     status = FALSE;
    }
 else
   {
     status = rfc_lte_p->rfc_get_lte_properties(ptr);  
    }

  return status;
}



