/*!
  @file
  ftm_device_manager.c

  @brief
  This module contains FTM common Control code for npt test.
 
  @addtogroup RF_COMMON_FTM_CONTROL
  @{
*/

/*==============================================================================

  Copyright (c) 2010 - 2015 Qualcomm Technologies, Inc. All Rights Reserved.

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

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_device_manager.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/18/16   ak      Fix Tune failure in FTM RF mode due to wrong device assigment
09/11/15   kg      Added a device capability check (Rx) to device allocation
08/12/15   pk      Featurized the cdma code for 1x compiled out build
08/11/15   zhw     Redundant F3 clean up
07/22/15   zhw     Added support for C2K C4/C5 FTM Cmd
07/20/15   zhw     Handle G extended/ber dispatch correctly
07/15/15   zhw     Enabled Bypass mode support
07/10/15   zhw     FTM device manager support for device reassign in SSIM mode
07/08/15   zhw     FTM device manager SSIM/MSIM support
06/15/15   zhw     Provide DTA Scenario awareness API for GSM Drx control
06/12/15   aro     Fixes to coex TH/AT for AT PreSi
06/11/15   zhw     Remapping algorithm update
06/11/15   aro     Fixes to coex TH/AT for AT PreSi
06/09/15   zhw     Update Remap solution to be compaitble with CDMA remapping
06/09/15   zhw     Added NPT test support for DTA scenarios
06/08/15   zhw     Update algorithm to handle sleep mode - fallback to existing mapping
06/05/15   zhw     Update Remap solution to handle sleep mode
06/05/15   zhw     Remap solution for FTM RF test
06/03/15   zhw     Added support for DRX query and GSM victim query
06/01/15   zhw     Initial Revision

==============================================================================*/

#include "comdef.h"
#include "ftm_common.h"
#include "rfcommon_core_utils.h"

#ifdef FEATURE_FACTORY_TESTMODE

#include "ftm_common_control.h"
#include "rfc_common.h"
#include "ftm_msg.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_device_manager.h"
#include "ftm_command_id.h"
#include "rfm.h"


#ifdef FEATURE_CDMA1X
#include "ftm_1x_control.h"
#include "ftm_cdma_data.h"
#endif /* FEATURE_CDMA1X */

#ifdef FEATURE_WCDMA
#ifdef ATLAS_RF_WCDMA_PRESI
#include "ftm_wcdma_interface.h"
#endif
#include "ftm_wcdma_ctl.h"
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_TDSCDMA
#include "ftm_tdscdma_ctl.h"
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_GSM
#include "ftm_gsm_rfctl.h"
#endif /* FEATURE_GSM */

static ftm_device_manager_data_type ftm_device_manager;

void
ftm_device_manager_init
(
)
{
  uint8 dev_idx;

  ftm_device_manager.operating_mode = FTM_DEVICE_MANAGER_SSIM_MODE;

  ftm_device_manager.npt_dta_scenario = FALSE;

  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device = RFM_INVALID_DEVICE;
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device = RFM_INVALID_DEVICE;
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band = SYS_BAND_CLASS_NONE;

  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].prx_device = RFM_INVALID_DEVICE;
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].drx_device = RFM_INVALID_DEVICE;
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].sys_band = SYS_BAND_CLASS_NONE;

  for ( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++ )
  {
    ftm_device_manager.device_mapping[dev_idx] = RFM_INVALID_DEVICE;
  }

}

void
ftm_device_manager_set_explicit_device_mapping
( 
   rfm_device_enum_type legacy_dev, 
   rfm_device_enum_type assigned_device
)
{ 
  if( legacy_dev < RFM_MAX_WAN_DEVICES )
  {
    ftm_device_manager.device_mapping[legacy_dev] = assigned_device; 

    FTM_MSG_2(FTM_MED,"ftm_device_manager_set_explicit_device_mapping: FTM Chain %d "
                       "assigned device updated to %d", 
                        legacy_dev, assigned_device );
  }
  else
  {
    FTM_MSG_1(FTM_ERROR,"ftm_device_manager_set_explicit_device_mapping: Invalid "
                      "FTM Chain %d passed in", legacy_dev );
  }

}

void
ftm_device_manager_reassign_device
( 
   rfm_device_enum_type legacy_dev, 
   rfm_device_enum_type assigned_device
)
{ 
  if( ftm_device_manager.device_mapping[legacy_dev] != RFM_INVALID_DEVICE &&
      assigned_device != RFM_INVALID_DEVICE &&
      ftm_device_manager.device_mapping[legacy_dev] != assigned_device )
  {
    FTM_MSG_4(FTM_HIGH,"ftm_device_manager_reassign_device: FTM Chain %d "
                       "assigned device changed from %d to %d, exiting current"
                       "rfmode on dev %d", 
                        legacy_dev, 
                        ftm_device_manager.device_mapping[legacy_dev],
                        assigned_device,
                        ftm_device_manager.device_mapping[legacy_dev] );

    ftm_rfmode_exit(ftm_device_manager.device_mapping[legacy_dev], FTM_STATE_PARK);
  }

  ftm_device_manager.device_mapping[legacy_dev] = assigned_device; 

}


rfm_device_enum_type
ftm_device_manager_get_assigned_device
(
  rfm_device_enum_type legacy_dev
)
{
  return ftm_device_manager.device_mapping[legacy_dev];    
}
    
boolean
ftm_device_manager_remap_from_rfc
(
  ftm_mode_id_type mode,
  rfm_device_enum_type legacy_dev,
  rfm_device_enum_type* remapped_dev
)
{
  boolean remapper_lookup_status;
  
  sys_band_class_e_type sys_band;
  rfc_cal_device_remap_info_type source_device_info;
  rfc_cal_device_remap_info_type remapped_device_info;

  /* Update CDMA Band Mapping for Other CDMA controls that does not go
  through tech dispatch */
#ifdef FEATURE_CDMA1X
  if ( ftm_common_resolve_rf_mode( mode ) == RFM_1X_MODE || 
       ftm_common_resolve_rf_mode( mode ) == RFM_1XEVDO_MODE )
  {
    ftm_cdma_data_set_band_mapping( legacy_dev, 
                                    ftm_1x_resolve_bandclass( mode ) );
  }
#endif /*FEATURE_CDMA1X*/

  /*Initialize output to invalid device*/
  remapped_device_info.device = RFM_INVALID_DEVICE;

  source_device_info.device = legacy_dev;
  source_device_info.tech = ftm_common_resolve_rf_mode( mode );
  source_device_info.band = ftm_common_convert_ftm_mode_to_rfcom_band( mode );
  source_device_info.alt_path = 0;
  source_device_info.rxtx = RFC_CONFIG_RX;
  source_device_info.remapper_present = FALSE;

  rfc_common_get_remapped_device_info ( &source_device_info, &remapped_device_info);

  if ( remapped_device_info.remapper_present == TRUE )
  {
    *remapped_dev = remapped_device_info.device;   

    FTM_MSG_3(FTM_HIGH, "FTM_SET_MODE: RFC Remapper assigned "
                        "mode %d Legacy Chain %d "
                        "to Dev %d", mode, legacy_dev, 
                        *remapped_dev);

    remapper_lookup_status = TRUE;
  }
  else
  {
    remapper_lookup_status = FALSE;
  }

  return remapper_lookup_status;
}


rfm_device_enum_type 
ftm_device_manager_allocate_device
(
  ftm_subsys_id_type ftm_subsys_id,
  ftm_pkt_type* ftm_cmd_ptr
)
{

  boolean api_status = TRUE;
  rfm_device_enum_type assigned_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type legacy_device = RFM_INVALID_DEVICE;

  ftm_device_manager_mode_enum_type operating_mode = 
                                   ftm_device_manager_get_operating_mode();

  legacy_device = ftm_common_convert_subsys_id_to_legacy_device(ftm_subsys_id);

  if( legacy_device >= RFM_MAX_WAN_DEVICES  )
  {
    FTM_MSG_1(FTM_ERROR, "ftm_device_manager_allocate_device():"
                        "Invalid FTM Chain %d", 
                        legacy_device);
    return RFM_INVALID_DEVICE;
  }

  if( operating_mode == FTM_DEVICE_MANAGER_BYPASS_MODE )
  {
    assigned_device = legacy_device;
    return assigned_device;
  }

  if( ftm_cmd_ptr->rf_params.ftm_rf_cmd == FTM_SET_MODE )
  {
    switch( operating_mode ) 
    {
    case FTM_DEVICE_MANAGER_SSIM_MODE:
      /* Standalone Mode Device Assignment*/
      assigned_device = ftm_device_manager_allocate_ssim_device(ftm_cmd_ptr, legacy_device);
      break; 
    case FTM_DEVICE_MANAGER_MSIM_MODE:
      /* MSIM Mode Device Assignment*/
      assigned_device = ftm_device_manager_allocate_device_concurrent_mode(ftm_cmd_ptr, legacy_device);
      break;
    default:
      assigned_device = RFM_INVALID_DEVICE;
      break;
    }
  }
  else 
  {
    assigned_device = ftm_device_manager_get_assigned_device(legacy_device);   
  }/* if( ftm_cmd_ptr->rf_params.ftm_rf_cmd != FTM_SET_MODE )*/

  if( assigned_device == RFM_INVALID_DEVICE )
  {
    FTM_MSG_2(FTM_HIGH, "ftm_device_manager_allocate_device():"
                        "No valid remapping for Cmd %d Legacy Chain %d n, "
                        "returning same device", 
                        ftm_cmd_ptr->rf_params.ftm_rf_cmd,
                        legacy_device);
    assigned_device = legacy_device;
  }

  FTM_MSG_3(FTM_HIGH,"ftm_device_manager_allocate_device(): "
                     "CMD %d Chain %d mapped to Dev %d" , 
                      ftm_cmd_ptr->rf_params.ftm_rf_cmd, 
                      legacy_device, assigned_device );

  return assigned_device;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API maps legacy FTM Chain Numbers ( C0 - C3 ) to new RFC mapping based
  on a set of criteria. This Remapping is needed to ensure FTM RF test and
  FTM Char can still work with new RFC mapping.
 
  Below is done during FTM_SET_MODE
 
  1. If RFC remapper is present, always use remapped device
  2. If RFC Remapper is not present, and phone is not in Cal state, allocate preferred device
     While allocating for Prx, the associated device will also be located and stored
  3. If RFC Remapper is not present and phone is in Cal state, NO remapping.
 
  Below is done for any other command than FTM_SET_MODE
 
  Device is remapped based on the stored mapping during FTM_SET_MODE. If Invalid
  Device is found, there is no remapping.
 
  @param ftm_cmd_ptr
  FTM Command
 
  @param legacy_chain_dev
  Legacy Chain mapping
 
  @return device
  Remapped RFM Device based on new RFC mapping
*/
rfm_device_enum_type 
ftm_device_manager_allocate_ssim_device
(
  ftm_pkt_type* ftm_cmd_ptr,
  rfm_device_enum_type legacy_dev
)
{
  boolean api_status = TRUE;
  boolean rfc_remapped = TRUE;
  rfm_device_enum_type assigned_device = legacy_dev;
  rfm_device_enum_type rfc_remapped_device = RFM_INVALID_DEVICE;
  sys_band_class_e_type sys_band;

  ftm_mode_id_type mode = ftm_cmd_ptr->rf_params.ftm_rf_factory_data.mode; 

  rfc_remapped = ftm_device_manager_remap_from_rfc(mode, legacy_dev, &rfc_remapped_device);

  if( rfc_remapped == TRUE )
  {
    assigned_device = rfc_remapped_device;
  } 
  else 
  {
    if( rfm_get_calibration_state() == FALSE )    
    {
      /* Enter automatic preferred routine */
      sys_band = ftm_common_convert_ftm_mode_to_sys_band(mode);
      
      if( sys_band == SYS_BAND_CLASS_NONE )
      {
        FTM_MSG_2(FTM_HIGH,"ftm_common_convert_legacy_chain_to_device: Invalid"
                           "sys_band from mode %d. Use existing mapping for Dev %d", 
                           mode, legacy_dev);
        api_status = FALSE;
      
        assigned_device = ftm_device_manager.device_mapping[legacy_dev];
      }
    
      if( api_status == TRUE )
      {
        rfm_device_enum_type preferred_prx_device = 
                                      rfc_common_get_preferred_rx_device(sys_band); 
        rfm_device_enum_type preferred_drx_device = 
              rfc_common_get_preferred_associated_rx_device(preferred_prx_device); 
        
        switch( legacy_dev )
        {
        case RFM_DEVICE_0:
          assigned_device = preferred_prx_device; 
          break;
        case RFM_DEVICE_1:
          assigned_device = preferred_drx_device;
          break;
        case RFM_DEVICE_2:
        case RFM_DEVICE_3:
          {
            rfm_device_enum_type dev_idx;
      
            for( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++ )
            {          
              if( rfc_common_check_band_supported( sys_band, dev_idx ) )
              {
                /* If Band is supported on this device, check whether it's the same 
                device as preferred prx or drx device. If so, continue. If not,*/
                if( (dev_idx != preferred_prx_device) && 
                    (dev_idx != preferred_drx_device ) &&
                    (rfc_common_get_device_capability(dev_idx) == RFM_DEVICE_RX_SUPPORTED))
                {
                  if( legacy_dev == RFM_DEVICE_2 )
                  {
                    assigned_device = dev_idx; 
                  }
                  else if( legacy_dev == RFM_DEVICE_3 )
                  {
                    assigned_device = rfc_common_get_preferred_associated_rx_device( dev_idx );
                  }
                  break;
                }
                else
                {
                  continue;
                }
              }/* if( rfc_common_check_band_supported( sys_band, dev_idx ) ) */
            }/* for( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++ ) */
            break;      
          }
        default:
          FTM_MSG_1(FTM_HIGH, "Legacy Dev %d conversion not supported, returning"
                              " same device", legacy_dev);
          assigned_device = legacy_dev;
          break;        
        }
      } /* if( api_status == TRUE ) */
    
      FTM_MSG_3(FTM_HIGH, "FTM_SET_MODE: Preferred Device allocation "
                          "mode %d Legacy Chain %d "
                        "assigned Dev %d", mode, legacy_dev, 
                        assigned_device);
    }/* if( rfm_get_calibration_state() == FALSE ) */
    else 
    {
      assigned_device = legacy_dev;
    
      FTM_MSG(FTM_HIGH, "FTM_SET_MODE: Calibration mode on a card without"
                        "remapper. Bypass chain remapping.");
    }
  }

  /* Assign PRX Functionality Device, tear down existing mapped device if
  necessary */
  ftm_device_manager_reassign_device(legacy_dev, assigned_device);

  /* Assign DRx Functionality Device - Associated Rx device */
  switch( legacy_dev )
  {
  case RFM_DEVICE_0:
    ftm_device_manager.device_mapping[RFM_DEVICE_1] = rfc_common_get_preferred_associated_rx_device( assigned_device );
    break;
  case RFM_DEVICE_2:
    ftm_device_manager.device_mapping[RFM_DEVICE_3] = rfc_common_get_preferred_associated_rx_device( assigned_device );
    break;
  case RFM_DEVICE_1:
    ftm_device_manager.device_mapping[RFM_DEVICE_0] = rfc_common_get_preferred_associated_rx_device( assigned_device );
    break;
  case RFM_DEVICE_3:
    ftm_device_manager.device_mapping[RFM_DEVICE_2] = rfc_common_get_preferred_associated_rx_device( assigned_device );
    break;
  default:      
    assigned_device = RFM_INVALID_DEVICE;
    break;
  }

  return assigned_device;
    
}/* ftm_device_manager_allocate_ssim_device */


rfm_device_enum_type 
ftm_device_manager_allocate_device_concurrent_mode
(
  ftm_pkt_type* ftm_cmd_ptr,
  rfm_device_enum_type legacy_dev
)
{
  boolean api_status = TRUE;
  rfm_device_enum_type device = RFM_INVALID_DEVICE;
  sys_band_class_e_type sys_band;

  ftm_mode_id_type mode = ftm_cmd_ptr->rf_params.ftm_rf_factory_data.mode; 

  sys_band = ftm_common_convert_ftm_mode_to_sys_band(mode);
  
  if( sys_band == SYS_BAND_CLASS_NONE )
  {
    FTM_MSG_1(FTM_ERROR,"ftm_device_manager_allocate_msim_device: Invalid"
                      "sys_band from mode %d, use existing mapping", mode);

    device = ftm_device_manager.device_mapping[legacy_dev];

    api_status = FALSE;
  }

  if( api_status == TRUE )
  {
    FTM_MSG_2(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                       "mode %d, sys_band %d", mode, sys_band );

    if( legacy_dev == RFM_DEVICE_0 )
    {
      device = rfc_common_get_preferred_rx_device(sys_band); 

      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device = device;
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device = 
        rfc_common_get_preferred_associated_rx_device( device );
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band = sys_band;

      ftm_device_manager.device_mapping[RFM_DEVICE_0] = device;
      ftm_device_manager.device_mapping[RFM_DEVICE_1] = 
                          rfc_common_get_preferred_associated_rx_device( device );

      FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                         "Aggressor device assigned %d",
                device);
    }
    else if( legacy_dev == RFM_DEVICE_2 )
    {
      boolean victim_dev_found = FALSE;
      rfm_device_enum_type dev_idx;
      
      rfm_device_enum_type agressor_dev;
      rfm_device_enum_type agressor_drx_dev;
      sys_band_class_e_type agressor_sys_band;
      sys_band_class_e_type victim_sys_band;

      boolean victim_dev_found_but_conflicted = FALSE;

      agressor_dev = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device;
      agressor_drx_dev = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device;
      agressor_sys_band = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band;

      device = rfc_common_get_preferred_rx_device(sys_band); 

      if ( device == agressor_dev || device == agressor_drx_dev ||
           ftm_concurrency_manager_check_band_conflict(agressor_dev, agressor_sys_band, device, sys_band) || 
           ftm_concurrency_manager_check_band_conflict(agressor_drx_dev, agressor_sys_band, device, sys_band) )
      {
        FTM_MSG_3(FTM_HIGH,"ftm_device_manager_allocate_msim_device:"
                           "Sub 1 Preferred Device %d conflicted with Sub 0 device Prx %d Drx %d."
                           "Start searching through RFC for valid Sub 1 device",
                           device, agressor_dev, agressor_drx_dev);

        for ( dev_idx = RFM_DEVICE_0; dev_idx <= RFM_DEVICE_4; dev_idx += 2)
        {
          if( rfc_common_check_band_supported( sys_band, dev_idx ) == TRUE &&
              rfc_common_get_device_capability( dev_idx ) == RFM_DEVICE_RX_SUPPORTED && 
              dev_idx != agressor_dev && dev_idx != agressor_drx_dev )
          {
            victim_dev_found = TRUE;   
                     
            FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: Found valid Victim "
                               "device %d sys_band %d",
                      dev_idx, sys_band );

            if ( ftm_concurrency_manager_check_band_conflict(agressor_dev, agressor_sys_band, dev_idx, sys_band) || 
                 ftm_concurrency_manager_check_band_conflict(agressor_drx_dev, agressor_sys_band, dev_idx, sys_band) )
            {

              FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                "Victim device %d sys_band %d confliction detected",
                        dev_idx, sys_band );

              if ( victim_dev_found_but_conflicted == FALSE )
              {
                device = dev_idx;

                FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                  "First avaliable Victim device %d sys_band %d, stored ",
                                  dev_idx, sys_band );
              }

              victim_dev_found_but_conflicted = TRUE;                          
            }
            else
            {
              FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                "Victim device %d sys_band %d No conflict, use this one",
                        dev_idx, sys_band );
              victim_dev_found_but_conflicted = FALSE;
            }

            if ( victim_dev_found_but_conflicted == FALSE )
            {
              device = dev_idx;
              break; 
            }
          }
        }

        /* If DTA scenario, use Drx device of the first available device */
        if ( victim_dev_found_but_conflicted == TRUE )
        {
          device = rfc_common_get_preferred_associated_rx_device( device );

          FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                             "All Prx Victim dev conlicted. DTA scenario: "
                             "Drx of Victim Prx device used %d",
                              device);

          ftm_device_manager.npt_dta_scenario = TRUE;
        }
      }/* Conflicted */
      else
      {
        FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device:"
                           "Sub 1 Preferred Device %d no conflict. Assigned",
                  device);
      }
            
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].prx_device = device;      
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].drx_device = 
        rfc_common_get_preferred_associated_rx_device( device );

      ftm_device_manager.device_mapping[RFM_DEVICE_2] = device;
      ftm_device_manager.device_mapping[RFM_DEVICE_3] = 
                          rfc_common_get_preferred_associated_rx_device( device );

    }/* else if( legacy_dev == RFM_DEVICE_2 ) */
    else
    {
      FTM_MSG_1(FTM_ERROR,"ftm_device_manager does not support Set mode on FTM"
                          "Chain %d in MSIM mode", legacy_dev );
    }
    
  }/* if( api_status == TRUE )*/

  return device;
}

rfm_device_enum_type 
ftm_device_manager_allocate_msim_device
(
  ftm_pkt_type* ftm_cmd_ptr
)
{
  boolean api_status = TRUE;
  rfm_device_enum_type device = RFM_INVALID_DEVICE;
  sys_band_class_e_type sys_band;

  ftm_mode_id_type mode = ftm_cmd_ptr->rf_params.ftm_rf_factory_data.mode; 

  sys_band = ftm_common_convert_ftm_mode_to_sys_band(mode);
  
  if( sys_band == SYS_BAND_CLASS_NONE )
  {
    FTM_MSG_1(FTM_ERROR,"ftm_device_manager_allocate_msim_device: Invalid"
                      "sys_band from mode %d", mode);
    api_status = FALSE;
  }

  if( api_status == TRUE )
  {
    FTM_MSG_2(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                       "mode %d, sys_band %d", mode, sys_band );

    if( ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device == RFM_INVALID_DEVICE )
    {
      device = rfc_common_get_preferred_rx_device(sys_band); 

      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device = device;
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device = 
        rfc_common_get_preferred_associated_rx_device( device );
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band = sys_band;

      FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                         "Aggressor device assigned %d",
                device);
    }
    else if( ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].prx_device == RFM_INVALID_DEVICE  )
    {
      boolean victim_dev_found = FALSE;
      rfm_device_enum_type dev_idx;
      
      rfm_device_enum_type agressor_dev;
      rfm_device_enum_type agressor_drx_dev;
      sys_band_class_e_type agressor_sys_band;
      sys_band_class_e_type victim_sys_band;

      boolean victim_dev_found_but_conflicted = FALSE;

      agressor_dev = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device;      
      agressor_drx_dev = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device;
      agressor_sys_band = ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band;

      device = rfc_common_get_preferred_rx_device(sys_band); 

      if ( device == agressor_dev || device == agressor_drx_dev ||
           ftm_concurrency_manager_check_band_conflict(agressor_dev, agressor_sys_band, device, sys_band) || 
           ftm_concurrency_manager_check_band_conflict(agressor_drx_dev, agressor_sys_band, device, sys_band) )
      {
        FTM_MSG_3(FTM_HIGH,"ftm_device_manager_allocate_msim_device:"
                           "Sub 1 Preferred Device %d conflicted with Sub 0 device Prx %d Drx %d."
                           "Start searching through RFC for valid Sub 1 device",
                           device, agressor_dev, agressor_drx_dev);
        for ( dev_idx = RFM_DEVICE_0; dev_idx <= RFM_DEVICE_4; dev_idx += 2)
        {
          if( rfc_common_check_band_supported( sys_band, dev_idx ) == TRUE &&
              rfc_common_get_device_capability( dev_idx ) == RFM_DEVICE_RX_SUPPORTED &&
              dev_idx != ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device )
          {
            victim_dev_found = TRUE;   
                     
            FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: Found valid Victim "
                               "device %d sys_band %d",
                      dev_idx, sys_band );

            if ( ftm_concurrency_manager_check_band_conflict(agressor_dev, agressor_sys_band, dev_idx, sys_band) )
            {

              FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                "Victim device %d sys_band %d confliction detected",
                        dev_idx, sys_band );

              if ( victim_dev_found_but_conflicted == FALSE )
              {
                device = dev_idx;

                FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                  "First avaliable Victim device %d sys_band %d, stored ",
                                  dev_idx, sys_band );
              }

              victim_dev_found_but_conflicted = TRUE;                          
            }
            else
            {
              FTM_MSG_2(FTM_MED,"ftm_device_manager_allocate_msim_device: "
                                "Victim device %d sys_band %d No conflict, use this one",
                        dev_idx, sys_band );
              victim_dev_found_but_conflicted = FALSE;
            }

            if ( victim_dev_found_but_conflicted == FALSE )
            {
              device = dev_idx;
              break; 
            }
          }
        }
        
        if ( victim_dev_found_but_conflicted == TRUE )
        {
          device = rfc_common_get_preferred_associated_rx_device( device );

          FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device: "
                             "All Prx Victim dev conlicted. DTA scenario: "
                             "Drx of Victim Prx device used %d",
                              device);

          ftm_device_manager.npt_dta_scenario = TRUE;
        }
        
      }/* Conflicted */
      else
      {
        FTM_MSG_1(FTM_HIGH,"ftm_device_manager_allocate_msim_device:"
                           "Sub 1 Preferred Device %d no conflict. Assigned",
                  device);
      }

      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].prx_device = device;      
      ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].drx_device = 
        rfc_common_get_preferred_associated_rx_device( device );

    }/* else if( ftm_rf_npt_chain[FTM_NPT_TECH_VICTIM] == RFM_INVALID_DEVICE  ) */
    
  }/* if( api_status == TRUE )*/

  return device;
}

boolean 
ftm_device_manager_is_device_npt_victim
(
   rfm_device_enum_type device  
)
{
  boolean ret_val = FALSE;

  if( ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_1].prx_device == device )
  {
    ret_val = TRUE;
  }

  return ret_val; 
}

boolean 
ftm_device_manager_is_device_in_dta_scenario
(
   rfm_device_enum_type device  
)
{
 
  return ftm_device_manager.npt_dta_scenario; 
}


rfm_device_enum_type 
ftm_device_manager_get_tech_prx_device
(
  ftm_rfstate_enum_type ftm_rf_tech
)
{
  rfm_device_enum_type dev_idx;
  boolean dev_found = FALSE;

  rfm_device_enum_type device = RFM_INVALID_DEVICE;

  for ( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++ )
  {
    if ( ftm_get_current_state(dev_idx) == ftm_rf_tech )
    {
      FTM_MSG_2(FTM_HIGH,"npt: First available device %d found for tech %d",
              dev_idx, ftm_rf_tech );
      dev_found = TRUE;
      device = dev_idx;                 
      break;
    }
  }

  if( dev_found == FALSE )
  {
    FTM_MSG_1(FTM_ERROR,"npt: ERROR: no active device found for tech %d",
            ftm_rf_tech );
  }

  return device;
}

rfm_device_enum_type 
ftm_device_manager_get_tech_drx_device
(
  ftm_rfstate_enum_type ftm_rf_tech
)
{
  rfm_device_enum_type prx_device;
  rfm_device_enum_type drx_device;

  prx_device = ftm_device_manager_get_tech_prx_device( ftm_rf_tech );

  drx_device = rfc_common_get_preferred_associated_rx_device( prx_device );

  FTM_MSG_2(FTM_HIGH,"npt: Drx device %d found for tech %d",
            drx_device, ftm_rf_tech );

  return drx_device;
}

void
ftm_device_manager_override_sub0_device
(
  rfm_device_enum_type device,
  sys_band_class_e_type sys_band
)
{
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].prx_device = device;
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].drx_device = 
        rfc_common_get_preferred_associated_rx_device( device );
  ftm_device_manager.tech_data[FTM_DEVICE_MANAGER_SUB_0].sys_band = sys_band;

  ftm_device_manager.device_mapping[RFM_DEVICE_0] = device;
  ftm_device_manager.device_mapping[RFM_DEVICE_1] = 
                      rfc_common_get_preferred_associated_rx_device( device );

  return;
}

void
ftm_device_manager_set_concurrent_mode
(
   boolean enable_flag
)
{
  ftm_device_manager.operating_mode = FTM_DEVICE_MANAGER_MSIM_MODE;
}

ftm_rsp_pkt_type
ftm_device_manager_set_bypass_mode
(
  ftm_common_pkt_type *ftm_req_data
)
{
  ftm_device_manager_mode_pkt_type* mode_data = (ftm_device_manager_mode_pkt_type*)(&(ftm_req_data->ftm_extended_params));
  ftm_rsp_pkt_type rsp = {FTM_RSP_DO_LEGACY, 0, ftm_req_data, FALSE};

  if( mode_data->enable_flag == 0 )
  {
    ftm_device_manager.operating_mode = FTM_DEVICE_MANAGER_BYPASS_MODE;    
  }
  else
  {
    ftm_device_manager.operating_mode = FTM_DEVICE_MANAGER_SSIM_MODE; 
  }
  
  return rsp;
}


ftm_device_manager_mode_enum_type
ftm_device_manager_get_operating_mode
(
   void
)
{
  return ftm_device_manager.operating_mode;
}

boolean
ftm_device_manager_is_subsys_id_supported
(
   ftm_subsys_id_type ftm_subsys_id
)
{
  boolean subsys_id_supported = FALSE;

  switch( ftm_subsys_id )
  {
  case FTM_1X_C0_C:
  case FTM_HDR_C0_C:
  case FTM_GSM_C0_C:
  case FTM_GSM_EXTENDED_C0_C:
  case FTM_WCDMA_C:
  case FTM_TDSCDMA_C:
  case FTM_1X_C1_C:
  case FTM_HDR_C1_C:
  case FTM_GSM_C1_C:
  case FTM_GSM_EXTENDED_C1_C:
  case FTM_WCDMA_RX_2_C:
  case FTM_TDSCDMA_RX_2_C:
  case FTM_1X_C2_C:
  case FTM_HDR_C2_C:
  case FTM_GSM_C2_C:
  case FTM_GSM_EXTENDED_C2_C:
  case FTM_WCDMA_RX_3_C:
  case FTM_TDSCDMA_C2_C:
  case FTM_1X_C3_C:
  case FTM_HDR_C3_C:
  case FTM_GSM_C3_C:
  case FTM_GSM_EXTENDED_C3_C:
  case FTM_WCDMA_RX_4_C:
  case FTM_TDSCDMA_C3_C:
    subsys_id_supported = TRUE;
    break;
  default:
    subsys_id_supported = FALSE;
    break;
  }

  return subsys_id_supported;
}

boolean
ftm_device_manager_is_subsys_id_extended_format
(
   ftm_subsys_id_type ftm_subsys_id
)
{
  boolean subsys_id_extended = FALSE;

  switch( ftm_subsys_id )
  {
  case FTM_GSM_BER_C0_C:
  case FTM_GSM_BER_C1_C:
  case FTM_GSM_BER_C2_C:
  case FTM_GSM_BER_C3_C:
    subsys_id_extended = TRUE;
    break;
  default:
    subsys_id_extended = FALSE;
    break;
  }

  return subsys_id_extended;
}


rfm_device_enum_type 
ftm_common_convert_subsys_id_to_legacy_device
(
  ftm_subsys_id_type ftm_subsys_id
)
{
  rfm_device_enum_type legacy_device;

  switch( ftm_subsys_id )
  {
  case FTM_1X_C0_C:
  case FTM_HDR_C0_C:
  case FTM_GSM_C0_C:
  case FTM_GSM_EXTENDED_C0_C:
  case FTM_GSM_BER_C0_C:
  case FTM_WCDMA_C:
  case FTM_TDSCDMA_C:
    legacy_device = RFM_DEVICE_0;
    break;
  case FTM_1X_C1_C:
  case FTM_HDR_C1_C:
  case FTM_GSM_C1_C:
  case FTM_GSM_EXTENDED_C1_C:
  case FTM_GSM_BER_C1_C:
  case FTM_WCDMA_RX_2_C:
  case FTM_TDSCDMA_RX_2_C:
    legacy_device = RFM_DEVICE_1;
    break;
  case FTM_1X_C2_C:
  case FTM_HDR_C2_C:
  case FTM_GSM_C2_C:
  case FTM_GSM_EXTENDED_C2_C:
  case FTM_GSM_BER_C2_C:
  case FTM_WCDMA_RX_3_C:
  case FTM_TDSCDMA_C2_C:
    legacy_device = RFM_DEVICE_2;
    break;
  case FTM_1X_C3_C:
  case FTM_HDR_C3_C:
  case FTM_GSM_C3_C:
  case FTM_GSM_EXTENDED_C3_C:
  case FTM_GSM_BER_C3_C:
  case FTM_WCDMA_RX_4_C:
  case FTM_TDSCDMA_C3_C:
    legacy_device = RFM_DEVICE_3;
    break;
  case FTM_1X_C4_C:
  case FTM_GSM_C4_C:
  case FTM_GSM_EXTENDED_C4_C:
  case FTM_GSM_BER_C4_C:
  case FTM_WCDMA_RX_5_C:
    legacy_device = RFM_DEVICE_4;
    break;
  case FTM_1X_C5_C:
  case FTM_GSM_C5_C:
  case FTM_GSM_EXTENDED_C5_C:
  case FTM_GSM_BER_C5_C:
  case FTM_WCDMA_RX_6_C:
    legacy_device = RFM_DEVICE_5;
    break;
  case FTM_WCDMA_RX_7_C:
    legacy_device = RFM_DEVICE_6;
    break;
  case FTM_WCDMA_RX_8_C:
    legacy_device = RFM_DEVICE_7;
    break;
  default:
    legacy_device = RFM_INVALID_DEVICE;
    break;
  }

  return legacy_device;
}

rfm_mode_enum_type 
ftm_common_convert_subsys_id_to_tech
(
  ftm_subsys_id_type ftm_subsys_id
)
{
  rfm_mode_enum_type tech = RFM_INVALID_MODE;

  switch( ftm_subsys_id )
  {
  case FTM_1X_C0_C:
  case FTM_1X_C1_C:
  case FTM_1X_C2_C:
  case FTM_1X_C3_C:
    tech = RFM_1X_MODE;
    break;
  case FTM_HDR_C0_C:
  case FTM_HDR_C1_C:
  case FTM_HDR_C2_C:
  case FTM_HDR_C3_C:
    tech = RFM_1XEVDO_MODE;
    break;
  case FTM_GSM_C0_C:
  case FTM_GSM_EXTENDED_C0_C:
  case FTM_GSM_BER_C0_C:
  case FTM_GSM_C1_C:
  case FTM_GSM_EXTENDED_C1_C:
  case FTM_GSM_BER_C1_C:
  case FTM_GSM_C2_C:
  case FTM_GSM_EXTENDED_C2_C:
  case FTM_GSM_BER_C2_C:
  case FTM_GSM_C3_C:
  case FTM_GSM_EXTENDED_C3_C:
  case FTM_GSM_BER_C3_C:
    tech = RFM_EGSM_MODE;
    break;
  case FTM_WCDMA_C:
  case FTM_WCDMA_RX_2_C:
  case FTM_WCDMA_RX_3_C:
  case FTM_WCDMA_RX_4_C:
    tech = RFM_IMT_MODE;
    break;
  case FTM_TDSCDMA_C:
  case FTM_TDSCDMA_RX_2_C:
  case FTM_TDSCDMA_C2_C:
  case FTM_TDSCDMA_C3_C:
    tech = RFM_TDSCDMA_MODE;
    break;
  default:

    FTM_MSG_1(FTM_ERROR,"ftm_common_convert_subsys_id_to_tech:"
                       "ftm_subsys_id %d unsupported, returning invalid value",
                       ftm_subsys_id);

    tech = RFM_INVALID_MODE;
    break;
  }

  return tech;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API Converts FTM MODE to sys band. Can be used to check for RFC Match
  . Currently supports 1x/W/T/G.
 
  @param mode 
  FTM Mode enum
 
  @return sys_band
  Converted sys band
*/
sys_band_class_e_type
ftm_common_convert_ftm_mode_to_sys_band
(
   ftm_mode_id_type mode
)
{
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_NONE;
  rfm_mode_enum_type tech = ftm_common_resolve_rf_mode( mode ); 

  switch( tech )
  {
#ifdef FEATURE_CDMA1X  
  case RFM_1X_MODE:
    sys_band = ftm_1x_resolve_bandclass( mode ); 
    break;
#endif /*FEATURE_CDMA1X*/

  #ifdef FEATURE_WCDMA
  case RFM_IMT_MODE:     
    {
      rfcom_wcdma_band_type wcdma_band;
      rfcom_band_type_u band;
      #ifdef ATLAS_RF_WCDMA_PRESI
      wcdma_band = (rfcom_wcdma_band_type)ftm_wcdma_utils_mode_to_wcdma_band( mode );
      #else
      wcdma_band = ftm_wcdma_convert_phone_mode_to_rfcom_band_type( mode );
      #endif
      band.wcdma_band = wcdma_band;
      sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, band);
      break;
    }
  #endif
  #ifdef FTM_HAS_TDSCDMA
  case RFM_TDSCDMA_MODE:
    {
      rfcom_tdscdma_band_type tds_band;
      rfcom_band_type_u band;
      tds_band = ftm_tdscdma_util_convert_band_ftm_to_rfcom( mode );
      band.tdscdma_band = tds_band;
      sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_TDSCDMA_MODE, band);
      break;
    }
  #endif
  #ifdef FEATURE_GSM
  case RFM_EGSM_MODE:
    {
      rfcom_gsm_band_type gsm_band;
      rfcom_band_type_u band;
      gsm_band = ftm_gsm_convert_ftm_mode_id_type_to_rfcom( mode );
      band.gsm_band = gsm_band;
      sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_EGSM_MODE, band);
      break;
    }
  #endif
  default:
    FTM_MSG_2(FTM_HIGH,"ftm_common_convert_ftm_mode_to_sys_band "
                       "invalid tech %d from mode %d",
                       tech, mode );    
    break;
  }

  return sys_band;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API Converts FTM MODE to RFCOM band. It returns uint32 which is best
  matched to RFC use. Currently supports 1x/W/T/G
 
  @param mode 
  FTM Mode enum
 
  @return band
  Converted RF COM Band
*/
uint32
ftm_common_convert_ftm_mode_to_rfcom_band
(
   ftm_mode_id_type mode
)
{
  uint32 band = 0;

  rfm_mode_enum_type tech = ftm_common_resolve_rf_mode( mode ); 

  switch( tech )
  {
#ifdef FEATURE_CDMA1X  
  case RFM_1X_MODE:
    band = (uint32)ftm_1x_resolve_bandclass( mode ); 
    break;
  #endif /*FEATURE_CDMA1X*/
  #ifdef FEATURE_WCDMA
  case RFM_IMT_MODE:     
    #ifdef ATLAS_RF_WCDMA_PRESI
    band = (uint32)ftm_wcdma_utils_mode_to_wcdma_band( mode );
    #else
    band = (uint32)ftm_wcdma_convert_phone_mode_to_rfcom_band_type( mode );
    #endif
    break;
  #endif
  #ifdef FTM_HAS_TDSCDMA
  case RFM_TDSCDMA_MODE:
    band = (uint32)ftm_tdscdma_util_convert_band_ftm_to_rfcom( mode );
    break;    
  #endif
  #ifdef FEATURE_GSM
  case RFM_EGSM_MODE:
    band = (uint32)ftm_gsm_convert_ftm_mode_id_type_to_rfcom( mode );
    break;
  #endif
  default:
    FTM_MSG_2(FTM_HIGH,"ftm_common_convert_ftm_mode_to_rfcom_band_u "
                       "invalid tech %d from mode %d",
                       tech, mode );    
    break;
  }

  return band;
}

/*! \} */

#endif /*FEATURE_FACTORY_TESTMODE */

/*! @} */
