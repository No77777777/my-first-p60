/*!
  @file
  ftm_1x_rf_test.c

  @brief
  This module contains FTM CDMA RF test code.

  @addtogroup RF_FTM_COMMON_CONTROL
  @{
*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies, Inc.. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc..

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_cdma.mpss/3.11/1x/ftm/src/ftm_1x_rf_test.c#1 $
  $DateTime: 2021/03/09 05:07:58 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/14/16   rs      KW error fix
12/09/15   pk      Added support for CDMA rx\tx split
08/11/15   zhw     Added support for Tx Control Command
08/03/15   zhw     Initial Revision

===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfa_variation.h"
#include "customer.h"
#include "comdef.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"

#ifdef FEATURE_CDMA1X

#include "string.h"
#include "ftm_msg.h"
#include "rfm_1x.h"
#include "ftm_1x_control.h"
#include "rfm_1x_ftm.h"
#include "rfm_internal.h"
#include "ftm_hdr_control.h"
#include "rf_cdma_data.h"
#include "ftm_cdma_data.h"
#include "rf_cdma_utils_freq.h"
#include "ftm_cdma_state_types.h"
#include "rfcommon_math.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_1x_rf_test.h"
#include "rf_1x_mc.h"
#include "ftm_1x_msm.h"
#include "rf_cdma_utils.h"

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Helper API for locate device for a given RF State

  @return
  Error code indicating the status of the API
*/
static rfm_device_enum_type
radio_config_locate_curr_device
(
   rf_cdma_state_type rf_state
)
{
  rfm_device_enum_type device = RFM_INVALID_DEVICE;
  rfm_device_enum_type dev_idx = RFM_INVALID_DEVICE;
  
  for(dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++)
  {
    if( rf_cdma_data_get_1x_rf_state(dev_idx) == rf_state )
    {
      device = dev_idx;
      break;
    }
  }

  return device;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Handles the radio_config command - Add device

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
static ftm_rf_test_field_error_code_mask_type
radio_config_add_device
(
   ftm_rf_test_radio_config_unpack_data_t* unpacked_data
)
{
  boolean api_status = TRUE;
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_INVALID_DEVICE;

  /* Go through add list and store data */
  if( unpacked_data == NULL )
  {
    api_status = FALSE;    
  }

  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type drx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  sys_band_class_e_type band = SYS_BAND_CLASS_NONE ;
  sys_channel_num_type chan = 0;

  if( api_status == TRUE )
  {
    /* Quick hack: SUB ID is defined as 1,2 in FTM whereas 0,1 in RFLM */
    rfm_1x_set_subscriber_id(unpacked_data->subscriber-1);

    device_mask = unpacked_data->add_list.device_mask;
    while( device_mask != 0 )
    {
      device = (rfm_device_enum_type)
               rf_extract_first_one_bit_lsb_location(device_mask);

      device_mask &= (~rf_extract_first_one_bit_mask(device_mask));

      FTM_MSG_1(FTM_MED,"[FTM.RFTEST][1X][ADD], device %d extracted", device);

      if( device >= RFM_DEVICE_0 && device < RFM_MAX_WAN_DEVICES )
      {
        if( unpacked_data->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX )
        {
          tx_device = device;
        }
        else if( unpacked_data->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX )
        {
          if( unpacked_data->add_list.cmd_params[device].is_primary == TRUE )
          {
            prx_device = device;
          }
          else
          {
            drx_device = device;
          }

          band = (sys_band_class_e_type)unpacked_data->add_list.cmd_params[device].band;
          chan = (sys_channel_num_type)unpacked_data->add_list.cmd_params[device].per_carrier[0].channel;

        }
      }          
      else
      {
        FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][ADD]: Invalid device found when parsing devices, "
                          "device %d, mask 0x%08x", device, device_mask );
        api_status = FALSE;
      }
    }

    FTM_MSG_5(FTM_MED,"[FTM.RFTEST][1X][ADD]: Finished parsing devices, "
                      "prx_device %d, drx_device %d, tx_device %d, band %d, chan %d",
                      prx_device, drx_device, tx_device, band, chan );
  }

  /* Set up RX Device - Enter mode, Set Chan */
  if( api_status == TRUE && prx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;

    ftm_cdma_data_status_type dev_state;
    ftm_1x_rfm_callback_data_type cb_payload;
    sys_channel_type band_chan; /* holds band_chan of master device */

    cb_payload.device = prx_device; /* Populate the CB Payload */

    rfm_ret &= ( ftm_rfmode_enter( prx_device, FTM_STATE_1X ) == TRUE );

    ftm_cdma_data_get_device_status( prx_device, &dev_state );

    rfm_ret &= ( rfm_enter_mode( prx_device, RFM_1X_MODE, NULL, NULL, dev_state.rxlm_handle) == 0 );

    band_chan.band = band;
    band_chan.chan_num = chan;

    rfm_ret &= ftm_cdma_control_enable_tech( RFM_1X_MODE, 
                                             prx_device, 
                                             band_chan,
                                             dev_state.rxlm_handle, 
                                             &ftm_1x_rfm_callback, 
                                             &cb_payload );

    rfm_ret &= ( rfm_1x_prep_wakeup_rx( prx_device, dev_state.rxlm_handle, band_chan, &ftm_1x_rfm_callback, &cb_payload ) >= 0 );

    rfm_ret &= ( rfm_1x_exec_wakeup_rx( prx_device, dev_state.rxlm_handle, &ftm_1x_rfm_callback, &cb_payload ) >= 0 );

    if( rfm_ret != TRUE)
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][ADD]: PRX Setup failed ");
      api_status = FALSE;
    }
  } /* PRX Setup */

  /* Setup Tx device */
  if( api_status == TRUE && tx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;
    ftm_cdma_data_status_type dev_state;
    ftm_1x_rfm_callback_data_type cb_payload;

    cb_payload.device = tx_device; /* Populate the CB Payload */

    /* Step 1: Set frequency adjustment parameters to default for FTM Tx wakeup. Freq error is initialized to zero */
    (void)rfm_1x_ftm_set_tx_freq_adjust( 0 );

    /* Locate Prx Device based on RF STM, if not provided by command */
    if( prx_device == RFM_INVALID_DEVICE )
    {
      prx_device = radio_config_locate_curr_device(RF_CDMA_STATE_RX);

      if( prx_device == RFM_INVALID_DEVICE )
      {
        FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][ADD]: failed to locate Prx Device");
        rfm_ret = FALSE;       
      }
    }

    /* Step 2: configure and enable Tx - use 0 for callback and userdata */
    if( rfm_ret == TRUE )
    {
      ftm_cdma_data_status_type dev_status;

      ftm_cdma_data_get_device_status( tx_device, &dev_status );
            
      rfm_ret &= ( ftm_1x_wakeup_tx_v2( tx_device, prx_device, 
                                         
                                        &ftm_1x_rfm_callback, &cb_payload ) >= 0);

      /* Step 2.5: force 1x into traffic mode */
      rfm_1x_set_pa_access_probe_config( tx_device, TRUE );
    }

    /* Step 3: configure and enable modulator for 1x */
    if( rfm_ret == TRUE )
    {
      void* smem_addr = ftm_cdma_data_1x_get_fw_smem_addr();
      if (smem_addr != NULL) 
      {
        ftm_1x_enable_modulator( tx_device, smem_addr );
      }
      else
      {
        rfm_ret = FALSE;       
        FTM_MSG( FTM_ERROR, "[FTM.RFTEST][1X][ADD]: TX fw shared memory ptr is NULL" );
      }
    }

    if( rfm_ret != TRUE)
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][ADD]:Tx Setup failed ");
      api_status = FALSE;
    }
  }/* Setup Tx device */

  /* Enable Diversity */
  if( api_status == TRUE && drx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;
    ftm_cdma_data_status_type dev_state;
    
    rfm_ret &= ftm_rfmode_enter( drx_device, FTM_STATE_1X );

    (void)ftm_cdma_data_get_device_status( drx_device, &dev_state );

    if ( rfm_ret == TRUE )
    {
      /* Enter 1x Mode */
      rfm_ret &= ( rfm_enter_mode( drx_device, RFM_1X_MODE, NULL, NULL,
                                   dev_state.rxlm_handle) == 0 );
    }

    if( rfm_ret == TRUE )
    {
      /* Check Tune allowed */
      rfm_ret &= ftm_cdma_control_is_tune_allowed( RFM_1X_MODE, drx_device, band );
    }

    /* Locate Prx Device based on RF STM, if not provided by command */
    if( prx_device == RFM_INVALID_DEVICE )
    {
      prx_device = radio_config_locate_curr_device(RF_CDMA_STATE_RX);

      if( prx_device == RFM_INVALID_DEVICE )
      {
        FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][ADD]: failed to locate Prx Device");
        rfm_ret = FALSE;       
      }
    }

    if( rfm_ret == TRUE ) 
    {
      ftm_1x_rfm_callback_data_type cb_payload;
      /* Call RFM Function to Enable Diversity */
      cb_payload.device = drx_device; /* Populate the CB Payload */
      rfm_ret &= ( rfm_1x_enable_diversity( prx_device, rf_cdma_data_get_rxlm_handle(prx_device), 
                                            drx_device, dev_state.rxlm_handle, 
                                            &ftm_1x_rfm_callback, &cb_payload ) >= 0 );
    }

    if( rfm_ret != TRUE )
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][ADD]: DRx Setup failed ");
      api_status = FALSE;
    }
  }

  if( api_status == TRUE )
  {
    status = 0;
  }
  else
  {
    status = 1; 
  }

  return status; 
}/* ftm_1x_rf_test_add_device */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Handles the radio_config command - Drop device

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
static ftm_rf_test_field_error_code_mask_type
radio_config_drop_device
(
   ftm_rf_test_radio_config_unpack_data_t* unpacked_data
)
{
  boolean api_status = TRUE;
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_INVALID_DEVICE;

  /* Go through add list and store data */
  if( unpacked_data == NULL )
  {
    api_status = FALSE;    
  }

  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type drx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  sys_band_class_e_type band = SYS_BAND_CLASS_NONE ;
  sys_channel_num_type chan = 0;

  if( api_status == TRUE )
  {
    device_mask = unpacked_data->drop_list.device_mask;
    while( device_mask != 0 )
    {
      device = (rfm_device_enum_type)
               rf_extract_first_one_bit_lsb_location(device_mask);

      device_mask &= (~rf_extract_first_one_bit_mask(device_mask));

      FTM_MSG_1(FTM_MED,"[FTM.RFTEST][1X][DROP], device %d extracted", device);

      if( device < RFM_MAX_WAN_DEVICES )
      {
        rf_cdma_state_type rf_state = rf_cdma_data_get_1x_rf_state( device ) ;

        switch( rf_state )
        {
          case RF_CDMA_STATE_RX:
            prx_device = device;
            break;
          case RF_CDMA_STATE_RXDIV:
            drx_device = device;
            break;
          case RF_CDMA_STATE_TX:
            tx_device = device;
            break;
          default:
            FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][DROP]: dev %d asked to drop from an invalid state %d",
                      device, rf_state );
            break;
        }
      }          
      else
      {
        FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][DROP]: Invalid device found when parsing devices, "
                          "device %d, mask 0x%08x", device, device_mask );
        api_status = FALSE;
      }
    }

    FTM_MSG_3(FTM_MED,"[FTM.RFTEST][1X][DROP]: Finished parsing devices, "
                      "prx_device %d, drx_device %d, tx_device %d",
                      prx_device, drx_device, tx_device);
  }

  /* Drop Drx device and exit mode */
  if( api_status == TRUE && drx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;
    
    rfm_ret &= rf_1x_mc_tear_down( drx_device );

    rfm_ret &= ftm_rfmode_enter( drx_device, FTM_STATE_PARK );

    if( rfm_ret != TRUE )
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][DROP]:DRx Drop failed ");
      api_status = FALSE;
    }
  }/* Drop Drx device and exit mode */

  /* Drop Tx device and exit mode */
  if( api_status == TRUE && tx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;
    ftm_cdma_data_status_type dev_state;
    ftm_1x_rfm_callback_data_type cb_payload;

    cb_payload.device = tx_device; /* Populate the CB Payload */

    /* Step 1: disable modulator for 1x */
    (void) ftm_1x_disable_modulator( tx_device );

    /* Step 2: Tear down Tx device */
    rfm_ret &= rf_1x_mc_tear_down( tx_device );

    if( rfm_ret != TRUE)
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][DROP]:Tx Setup failed ");
      api_status = FALSE;
    }
  }/* Drop Tx device */

  /* Drop Prx Device and exit mode */
  if( api_status == TRUE && prx_device != RFM_INVALID_DEVICE )
  {
    boolean rfm_ret = TRUE;
    rfm_device_enum_type active_tx_device = RFM_INVALID_DEVICE;
    rfm_device_enum_type active_drx_device = RFM_INVALID_DEVICE;

    active_tx_device = radio_config_locate_curr_device( RF_CDMA_STATE_TX );
    active_drx_device = radio_config_locate_curr_device( RF_CDMA_STATE_RXDIV );

    /* Preventive check for Active Tx/Drx Device */
    if( active_tx_device != RFM_INVALID_DEVICE || active_drx_device != RFM_INVALID_DEVICE )
    {
      FTM_MSG_3(FTM_ERROR,"[FTM.RFTEST][1X][DROP]:INVALID SEQUENCE. PRX %d shutdown before DRX %d/TX %d disable",
                prx_device, active_drx_device, active_tx_device);

      rfm_ret = FALSE;
    }

    if( rfm_ret == TRUE )
    {
      rfm_ret &= rf_1x_mc_tear_down(prx_device); 

      rfm_ret &= ftm_rfmode_enter( prx_device, FTM_STATE_PARK );
    }

    if( rfm_ret != TRUE)
    {
      FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][DROP]:PRX Drop failed ");
      api_status = FALSE;
    }
  }/* Drop Prx Device and exit mode */

  if( api_status == TRUE )
  {
    status = 0;
  }
  else
  {
    status = 1; 
  }

  return status; 
}/* ftm_1x_rf_test_drop_device */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Handles the radio_config command - Reconfigure Device

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
static ftm_rf_test_field_error_code_mask_type
radio_config_reconfig_device
(
   ftm_rf_test_radio_config_unpack_data_t* unpacked_data
)
{
  boolean api_status = TRUE;
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_INVALID_DEVICE;

  /* Go through add list and store data */
  if( unpacked_data == NULL )
  {
    api_status = FALSE;    
  }

  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type drx_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;

  boolean prx_retune_rejected = FALSE;
  boolean drx_retune_rejected = FALSE;
  boolean tx_retune_rejected = FALSE;

  sys_channel_type band_chan; /* holds band_chan of master device */
  sys_band_class_e_type band = SYS_BAND_CLASS_NONE ;
  sys_channel_num_type chan = 0;

  if( api_status == TRUE )
  {
    device_mask = unpacked_data->reconfig_list.device_mask;
    while( device_mask != 0 )
    {
      device = (rfm_device_enum_type)
               rf_extract_first_one_bit_lsb_location(device_mask);

      device_mask &= (~rf_extract_first_one_bit_mask(device_mask));

      FTM_MSG_1(FTM_MED,"[FTM.RFTEST][1X][RECONFIG], device %d extracted", device);

      if( device >= RFM_DEVICE_0 && device < RFM_MAX_WAN_DEVICES )
      {
        if( unpacked_data->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX )
        {
          tx_device = device;
        }
        else if( unpacked_data->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX )
        {
          if( unpacked_data->reconfig_list.cmd_params[device].is_primary == TRUE )
          {
            prx_device = device;
          }
          else
          {
            drx_device = device;
          }

          band = (sys_band_class_e_type)unpacked_data->reconfig_list.cmd_params[device].band;
          chan = (sys_channel_num_type)unpacked_data->reconfig_list.cmd_params[device].per_carrier[0].channel;

        }
      }          
      else
      {
        FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Invalid device found when parsing devices, "
                          "device %d, mask 0x%08x", device, device_mask );
        api_status = FALSE;
      }
    }

    FTM_MSG_5(FTM_MED,"[FTM.RFTEST][1X][RECONFIG]: Finished parsing devices, "
                      "prx_device %d, drx_device %d, tx_device %d, band %d, chan %d",
                      prx_device, drx_device, tx_device, band, chan );
  }

  band_chan.band = band;
  band_chan.chan_num = chan;

  if( prx_device == RFM_INVALID_DEVICE )
  {
    FTM_MSG(FTM_MED,"[FTM.RFTEST][1X][RECONFIG]: Invalid PRx Device. No need to reconfigure" );
    
  }
  else /* if( prx_device == RFM_INVALID_DEVICE )*/
  {
    if( api_status == TRUE )
    {
      if( rf_cdma_data_get_1x_rf_state( prx_device ) != RF_CDMA_STATE_RX )
      {
        FTM_MSG_1(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Attempt to retune without PRx device or bad state PRx %d. Invalid Sequence",
                  prx_device );
        prx_retune_rejected = TRUE;
      }

      if( prx_device != RFM_INVALID_DEVICE &&  
          ( ftm_cdma_control_is_tune_allowed(RFM_1X_MODE, prx_device, band ) == FALSE ) )
      {
        FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Band %d not supported on PRx Device %d. Invalid Sequence",
                  band, prx_device );
        prx_retune_rejected = TRUE;
      }

      if( drx_device != RFM_INVALID_DEVICE && 
          rf_cdma_data_get_1x_rf_state( drx_device ) != RF_CDMA_STATE_RXDIV )
      {
        FTM_MSG_1(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Attempt to retune inactive DRx Device %d. Invalid Sequence",
                  drx_device );
        drx_retune_rejected = TRUE;
      }

      if( drx_device != RFM_INVALID_DEVICE && 
          ( ftm_cdma_control_is_tune_allowed( RFM_1X_MODE, drx_device , band ) == FALSE ) )
      {
        FTM_MSG_2(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Band %d not supported on DRx Device %d. Invalid Sequence",
                  band, drx_device );
        drx_retune_rejected = TRUE;
      }

      if( tx_device != RFM_INVALID_DEVICE &&
          rf_cdma_data_get_1x_rf_state( tx_device ) != RF_CDMA_STATE_TX )
      {
        FTM_MSG_1(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Attempt to retune inactive Tx Device %d. Invalid Sequence",
                  tx_device );
        tx_retune_rejected = TRUE;
      }
    }

    if( prx_retune_rejected || drx_retune_rejected || tx_retune_rejected )
    {
      FTM_MSG(FTM_HIGH,"[FTM.RFTEST][1X][RECONFIG]: Invalid sequence detected. Reject RECONFIG request" );
      api_status = FALSE;
    }

    /* Retune PRx, Drx & Tx Device */
    if( api_status == TRUE )
    {
      boolean rfm_ret = TRUE;
      ftm_cdma_data_status_type dev_state;
      ftm_1x_rfm_callback_data_type cb_payload;

      cb_payload.device = prx_device;

      /* Before Retune call L1 API to set Frame valid to False, if in RxTx state */
      if ( tx_device != RFM_INVALID_DEVICE )
      {
        ftm_1x_enable_baseband_tx(FALSE);
      }

      if( drx_device != RFM_INVALID_DEVICE )
      {
        /* clean up current device from concurrency manager before retune */ 
        ftm_concurrency_manager_cleanup_device_allocation(drx_device, RFM_1X_MODE); 
      }

      /* Query the current RF State */
      (void)ftm_cdma_data_get_device_status( prx_device, &dev_state );

      rfm_ret &= (rfm_1x_retune_v2(prx_device, tx_device, dev_state.rxlm_handle, 
                                   band_chan, &ftm_1x_rfm_callback, &cb_payload ) >= 0 ); 

      /* After Retune call L1 API to set Frame valid to TRUE , this will 
         trigger FW to run RF on/off scripts, if in RxTx state*/ 
      if ( tx_device != RFM_INVALID_DEVICE )
      {
        ftm_1x_enable_baseband_tx(TRUE);
      }

      if( rfm_ret != TRUE)
      {
        FTM_MSG(FTM_ERROR,"[FTM.RFTEST][1X][RECONFIG]: Retune failed ");
        api_status = FALSE;
      }
    }/* Retune PRx, Drx & Tx Device */
  }/* if( prx_device != RFM_INVALID_DEVICE ) */

  if( api_status == TRUE )
  {
    status = 0;
  }
  else
  {
    status = 1; 
  }

  return status; 
}/* radio_config_reconfig_device*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Handles the cdma RF test radio_config command

  @details
  This function is a callback function used to handle the cdma RF test
  radio_config command sent from the common framework
  Pre-requisite : The API should be registered as a callback.

  @param command_unpack_data
  Poitner to the command_unpack_data containing all the params required for
  the radio setup.

  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type
ftm_1x_rf_test_radio_config
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;

  FTM_MSG( FTM_HIGH, "ftm_1x_rf_test_radio_config: Received radio_config" );

  /*Follow this order of Drop, Add and Reconfig*/
  /*Step 1: Process the Drop list*/
  status |= radio_config_drop_device( command_unpack_data );

  /*Step 2: Process the Add list*/
  status |= radio_config_add_device( command_unpack_data );

  /*Step 3: Process the Reconfig list*/
  status |= radio_config_reconfig_device( command_unpack_data );

  if(0 != status)
  {
    FTM_MSG_1( FTM_ERROR, "ftm_1x_rf_test_radio_config: status 0x%x", status );
  }

  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Call back to handle Rx Measure command

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
ftm_rf_test_field_error_code_mask_type
ftm_1x_rf_test_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  boolean api_status = TRUE;
  uint8 meas_index;
  rfm_cdma_ftm_rx_agc_data_type rx_agc_data;
  rfm_device_enum_type device;

  for( meas_index = 0; meas_index < num_of_measurements;
       meas_index ++ )
  {
    
    device = query_data[meas_index].input.device;
   
    api_status &= rfm_1x_ftm_get_rx_agc_data( device, &rx_agc_data, RFM_CDMA_AGC_FORMAT__DBM10);

    if( api_status == TRUE )
    {
      query_data[meas_index].output.rxagc = rx_agc_data.rx_agc[0];
      query_data[meas_index].output.lna_gain_state = rx_agc_data.gain_state;
    }
    else
    {
      FTM_MSG_1(FTM_ERROR,"ftm_1x_rf_test_rx_measure: error when reading AGC on device %d", device );
      error_code = 1;
    }
  }

  return error_code;
}/* ftm_1x_rf_test_rx_measure */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Call back to handle Tx Control command

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type 
ftm_1x_rf_test_tx_control
( 
  ftm_rf_test_tx_control_unpacked_data_t *query_data,
  uint32 num_of_actions
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  boolean api_status = TRUE;
  uint8 action_index;
  rfm_device_enum_type device;
  int16 tx_power;
  int16 tx_pwr_mdsp_units;
  boolean enable_tx;

  for( action_index = 0; action_index < num_of_actions; action_index ++ )
  { 
    const rf_cdma_data_status_type *dev_status_r;/* Dev Status Read Ptr*/
       
    device = query_data[action_index].input.device;
    enable_tx = (boolean)query_data[action_index].input.tx_action;
    tx_power = (int16)query_data[action_index].input.tx_power;

    FTM_MSG_3( FTM_LOW, "ftm_1x_rf_test_tx_control: Device %d, Tx ON %d, TxPwr(dBm10) %d",
                         device, enable_tx, tx_power );

    
    dev_status_r = rf_cdma_get_device_status( device );

    if ( dev_status_r == NULL  )
    {
      RF_MSG_1( RF_ERROR, "ftm_1x_rf_test_tx_control(): NULL dev status for device %d", device );
      api_status = FALSE;
    }

    if( api_status == TRUE )
    {    
      tx_pwr_mdsp_units = rf_cdma_mdsp_dbm10_to_txagc( tx_power, 
                                                       10*rf_cdma_get_turnaround_constant(dev_status_r->curr_band) );

      /* Call RFM Function to set Tx power */
      api_status = rfm_1x_ftm_set_tx_pwr ( device, RFM_CDMA_TX_PWR_OPEN_LOOP, tx_pwr_mdsp_units ) ;
    }

    if( api_status == FALSE )
    {
      FTM_MSG_1(FTM_ERROR,"ftm_1x_rf_test_tx_control: error when setting Tx power on device %d", device );

      error_code = 1;

      break;
    }
  }/* for( action_index = 0; action_index < num_of_actions; action_index ++ ) */

  return error_code;
}/* ftm_1x_rf_test_tx_control */

#endif /* FEATURE_CDMA1X */
#endif /* FEATURE_FACTORY_TESTMODE */

