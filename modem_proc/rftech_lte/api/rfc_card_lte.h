#ifndef RFC_CARD_LTE_H
#define RFC_CARD_LTE_H
/*!
  @file rfc_card_lte.h

  @brief
  This file contains LTE mode data definitions and prototypes.

  @details

*/

/*===========================================================================

Copyright (c) 2009 - 2020 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:27:23 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_lte.mpss/3.11/api/rfc_card_lte.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
11/18/19   cv      TX alt path support
03/02/18   ks      RFFE based XSW support
03/23/16   sk      Jacala LTE FBRX tune-away support
02/23/16   sk      Pass FB chain to FBRX module from RFC
02/22/16   sk      Pass Rx band to get FB device
01/27/16   sk      Get LTE Feedback Device from RFC
11/26/15   lhb     FR 31436: API to provide the HW band limitation to LTE RRC
11/03/15   vkm     Correct/Pass CA mode and ALT PATH in meas cases
10/24/15   stw     MC Level Band Decision maker added to handle split bands
09/28/15   stw     FR28012 :Alternate Path Used to find correct Split Band from RFC for Rx tuning
07/25/15    JJ     Added FBRx coupler/switch re-program support
06/25/15   pkg     Changes to support Intra ULCA Tune on bands with splits.
04/13/15   lhb     Ported : GRFC Alt Path Support
04/08/15   pkg     Changes to support Tx Rx Split Band design.
12/15/14    ars     Adding check_intra_ca_feasible() API to fix issues with intra CA on split bands.
12/05/14   dw      Changes to support split bands based on split specified in RFC.
10/20/14   ndb     Remove the Deprecated  API's 
10/09/14   rsr     Fixing compiler warning.
09/18/14   rsr     Changes to return TX LUT index when querying TX LUT info.
08/15/14   st      NS07 Port Update Support
08/01/14   ndb     Added support for extended EARFCN for LTE
07/10/14   jf      TM8 + SRS switching for TDD 
07/08/14   gvn     Modify algorithm for split-intraband support 
07/02/14   jf      AsDiv support 
06/11/14   bar     Added rfc_lte_disable_fb_device() API to support IQ capture
05/15/14   gvn     Add Api for getting overlapping split bands
04/11/14   bsh     Split Band Support
01/09/14   svi     Split Band Support
04/25/13   gvn     LTE Band split support
04/10/13   kab     Added support for Tx Antenna Swap. 
03/12/13   sn      XPT dual WTR fix for feedback path
10/31/12   vss     Change to get device API for returning a list of objects
07/30/12   pl      Fix Warning
07/27/12   sr      renamed rfdevice related enum types.
07/18/12   sr      added get_device api to return the rf device obj ptr
                   based on logical_path, rx_tx, band and device_type.
06/06/12   pl      Update RFC interface to take DEVICE instead of PATH
06/01/12   aca     Fix to clean up Tx signals when disable tx is called
04/24/12   cd      Fixed LTE exit mode to restore init settings for both Tx and Rx band GRFC/ANT_SELs
03/01/12   jf    Added LPM status support
12/05/11   tnt     Add rfc_lte_mode_sleep() api 
08/10/11   pl      Remove obsolete structure
06/29/11   pl      Added path and band information in mode_exit()
06/23/11   pl      Add support for exit mode
04/26/11   tnt     Porting to new vreg management interface
04/13/11   aak     Revert name to rfc_lte_manage_vregs()
04/13/11   aak     Change rfc_lte_manage_vregs() to rfc_lte_manage_power()
04/01/11   bmg     Added extern "C" for C++ safety.
02/28/11   tnt     Merge to MDM9K PLF110
01/13/11   pl      Use rfcom_lte_band_type instead of rf_card_band_type
01/05/11   pl      Adding band information into get_rx_band_config
12/21/10   can     Basic APT\GST support
07/20/10   tnt     remove obsolete field in rfc_lte_band_config_type
07/08/10   tnt     RFC re-architecture to support GRFC
06/16/09   qma     Initial check-in.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfc_card.h"
#include "rflte_ext_mc.h"
#include "rflte_mc_rfc_interface.h"

/*===========================================================================
                           Data Definitions
===========================================================================*/

#ifdef __cplusplus
#include "rfdevice_class.h"
extern "C" {
#endif

#define RFC_APT_SMPS_CHAR_TBL_NV_SIZE   64

/* Structure for LPM_status support*/
  typedef struct
  {
     rfcom_lte_band_type band;
     boolean lpm_band_status;
  } rfc_lte_band_lpm_type;
/*===========================================================================
                           Prototypes
===========================================================================*/
/* ----------------------------------------------------------------------- */
void rfc_lte_config_mode_data( rfm_device_enum_type device );
/* ----------------------------------------------------------------------- */
void* rfc_lte_get_mode_config(rfm_device_enum_type device, 
                              rfc_mode_config_data_enum_type config_data,
                              uint8 alt_path);
/* ----------------------------------------------------------------------- */
void rfc_lte_config_rx_band_data(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path);
/* ----------------------------------------------------------------------- */
void *rfc_lte_get_rx_band_config(rfm_device_enum_type device, rfcom_lte_band_type band,
                                 rfc_band_config_data_enum_type config_data, uint8 alt_path);
/* ----------------------------------------------------------------------- */
void rfc_lte_config_tx_band_data (rfm_device_enum_type device, rfcom_lte_band_type band);
/* ----------------------------------------------------------------------- */
void *rfc_lte_get_tx_band_config (rfm_device_enum_type device, rfcom_lte_band_type band,
                                  rfc_band_config_data_enum_type config_data);
/* ----------------------------------------------------------------------- */
void *rfc_lte_get_meas_data (rfm_device_enum_type device, rfcom_lte_band_type band,
                             rfc_band_config_data_enum_type config_data);
/* ----------------------------------------------------------------------- */
void rfc_lte_manage_vregs(uint8 sub_id,rfm_device_enum_type device, rfcom_lte_band_type band, rfm_path_state state);
/* ----------------------------------------------------------------------- */
int32 rfc_lte_command_dispatch( rfm_device_enum_type device, int32 cmd, void *data);
/* ----------------------------------------------------------------------- */
void rfc_lte_mode_exit(rfm_device_enum_type device, rfcom_lte_band_type rx_band, rfcom_lte_band_type tx_band, uint8 alt_path);
/* ----------------------------------------------------------------------- */
void rfc_lte_mode_sleep(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path);
/* ----------------------------------------------------------------------- */
void rfc_lte_tx_disable(rfm_device_enum_type device, rfcom_lte_band_type band);
/* ----------------------------------------------------------------------- */
void** rfc_lte_get_device(rfm_device_enum_type rfm_dev, rfc_rxtx_enum_type rx_tx,
                                   rfcom_lte_band_type band, rfdevice_type_enum_type dev_type);
								   
boolean rfc_lte_setup_fb_path (rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable);

void* rfc_lte_get_fb_path_config (rfm_device_enum_type device_fb, rfcom_lte_band_type band, boolean et_enable);

rfm_device_enum_type rfc_lte_get_fb_device(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band, rfcom_lte_band_type rx_band);

uint32 rfc_lte_get_fb_chain(rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band);

#ifdef FEATURE_RF_HAS_WTR3925
boolean rfc_lte_is_fb_device_conflict(rfm_device_enum_type rx_device, rfm_device_enum_type tx_device, rfcom_lte_band_type tx_band, rfcom_lte_band_type rx_band);
#endif

boolean rfc_lte_setup_fb_device (rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_chan);

boolean rfc_lte_disable_fb_device (rfm_device_enum_type device, rfm_device_enum_type device_fb,rfcom_lte_band_type band, uint32 tx_chan);
/* ----------------------------------------------------------------------- */
void rfc_lte_get_rx_band(boolean *split_band, rfcom_lte_earfcn_type rx_chan,
                         rfcom_lte_band_type *rfc_band);
/* ----------------------------------------------------------------------- */
void rfc_lte_get_tx_band(boolean *split_band, rfcom_lte_earfcn_type tx_chan, 
                         rfcom_lte_band_type *rfc_band, boolean filtered_band);

/* ----------------------------------------------------------------------- */
void rfc_lte_update_tx_devices(rfm_device_enum_type rfm_dev, 
                               rfcom_lte_band_type curr_band, 
                               uint8 alt_path);

/* ----------------------------------------------------------------------- */
void rfc_lte_get_rx_split_band_earfcns(rfcom_lte_band_type split_band, 
                                       rfcom_lte_earfcn_type *low_earfcn,
                                       rfcom_lte_earfcn_type *high_earfcn);

/* ----------------------------------------------------------------------- */
rfm_device_enum_type rfc_lte_get_mapped_device_for_scell
(
 rfcom_lte_band_type pcell_band,
 rfcom_lte_band_type scell_band,
 rfm_device_enum_type ml1_rfm_device
);
/* ----------------------------------------------------------------------- */
void rfc_lte_reset_grfc_band_config(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 alt_path);

/*This API is meant to group list of channel,bw,device and fit them into same split*/
void rfc_lte_get_band_based_on_list(rflte_mc_band_decision_config_params_s_type* input_list, uint32 list_size);

/* These new APIs are for querying band based on additional params */
void rfc_lte_get_rx_band_v2(boolean *split_band, rfcom_device_enum_type device, rfcom_lte_earfcn_type rx_chan, rfc_lte_bw_type curr_bw,
                            uint8 path_index_override, rfcom_lte_band_type *rfc_band, uint8 alt_path);

void rfc_lte_get_tx_band_v2(boolean *split_band, rfcom_lte_earfcn_type tx_chan, rfc_lte_bw_type curr_bw,
                            uint8 path_index_override, rfcom_lte_band_type *rfc_band, boolean filtered_band);
/* ----------------------------------------------------------------------- */
void rfc_lte_get_tx_split_band_earfcns(rfcom_lte_band_type split_band, rfcom_lte_earfcn_type *low_earfcn, rfcom_lte_earfcn_type *high_earfcn);
								   
/* ----------------------------------------------------------------------- */
void rfc_lte_get_split_rx_bands(boolean *split_band, rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type *rfc_band1, rfcom_lte_band_type *rfc_band2);
								   
/* ----------------------------------------------------------------------- */
void rfc_lte_get_rx_split_band_channel_rasters(rfcom_lte_band_type split_band, 
                                               rfcom_lte_earfcn_type *low_earfcn,
                                               rfcom_lte_earfcn_type *high_earfcn,
                                               rfcom_lte_band_type *band_list,
                                               uint8 *num_of_splits);

/* ----------------------------------------------------------------------- */
boolean rfc_lte_check_intra_ca_feasible(void *pcc_info, 
                                        void *scc_info,
                                        rfcom_lte_band_type *intra_ca_split_band,
                                        rfc_rxtx_enum_type config_type);

/* ----------------------------------------------------------------------- */
void rfc_lte_update_ns07_ports(rfm_device_enum_type rfm_dev,rfcom_lte_band_type curr_band);
/* ----------------------------------------------------------------------- */
uint64 rfc_lte_get_split_bands(rfcom_lte_band_type full_band);
/* ----------------------------------------------------------------------- */

#ifdef FEATURE_RF_ASDIV
boolean rfc_lte_get_asdiv_xsw_script(uint32 position, rf_buffer_intf *asdiv_script_ptr, void* cb_data);

/* ----------------------------------------------------------------------- */

void* rfc_lte_get_asd_xsw_config(rfm_device_enum_type device, rfcom_lte_band_type band, uint32 position);
/* ----------------------------------------------------------------------- */		

boolean rfc_lte_get_xsw_sleep_script (  uint32 position, rf_buffer_intf *asdiv_script_ptr, void* cb_data );

#endif	
/* ----------------------------------------------------------------------- */				   
boolean rfc_lte_get_srs_xsw_script( uint32 position,
                                    rf_buffer_intf *srs_xsw_script_ptr,  
                                    rfm_device_enum_type device,
                                    rfcom_lte_band_type band,
                                    boolean immediate);								   
/* ----------------------------------------------------------------------- */				   
rfdevice_rxtx_common_class * rfc_lte_get_common_rxtx_device
(
  rfm_device_enum_type rfm_device, 
  rfcom_lte_band_type band,
  uint32 alt_path
); 

boolean rfc_lte_get_associated_tx_device(rfm_device_enum_type device, 
                                         rfcom_lte_band_type band, 
                                         rfm_device_enum_type* tx_device);


boolean rfc_lte_get_valid_devices(rflte_mc_cell_idx_type carrier_index,
                                  rfcom_lte_band_type band,
                                  rflte_mc_device_info_type* ret_params,
                                  uint16 devices_used_mask);

boolean rfc_lte_validate_devices( rfcom_lte_band_type band,
                                  rflte_mc_device_info_type* input_params,
                                  uint16 devices_used_mask);


boolean rfc_lte_get_band_supported_device(rfcom_lte_band_type rf_band, rfm_device_enum_type* device, rfc_rxtx_enum_type rx_tx);

boolean rfc_lte_get_partial_band_info (rfc_lte_properties_type **ptr);

#ifdef __cplusplus
}
#endif

#endif /* RFC_CARD_LTE_H */
