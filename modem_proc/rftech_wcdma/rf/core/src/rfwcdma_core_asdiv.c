/*!
  @file
  rfwcdma_core_asdiv.c

  @brief
  This file contains all declarations and definitions necessary to use the RF
  WCDMA Antenna Switch Diversity 
*/

/*==============================================================================

  Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_asdiv.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
04/19/16   sub     RF F3 cleanup
10/13/15   Saul    GNSS Blanking
10/08/15   dw      Asdiv support for GRFC switch
09/10/15   rmb     Remove Error message if ASD signals are not present.
08/25/15   vs      Skip ASD event if input with same position
05/05/15   sd      [Rx/Tx split] Fix compile errrors on TA 2.0
07/04/15   ak      Migrating to new WCDMA state machine
07/04/15   ak      Rx/Tx Split Changes
12/16/14   ag      Aborting the pending AsDiv events
10/29/14   ag      Fixed Compiler Warning
10/27/14   kr      Fix compiler warning
10/04/14   ac      xsw script to be appended to tune, featurised for now
04/17/14   ac      asd bringup with rfc changes and device script
04/08/14   ac      asdiv ftm bringup changes
03/27/14   ac      porting dime asd changes to fix compile error 
03/05/14   ac      asdiv bringup support
08/07/13   aro     KW Fix
07/25/13   aro     Fixed the script_gen_only flag
07/19/13   aro     Featurized ASDIV
07/17/13   aro     Moved the L1 callback definition to rfm.c
07/15/13   aro/kr  Added initial support for WCDMA ASDiv
07/13/13   aro     Initial version

============================================================================*/

#include "comdef.h"
#include "rfcommon_msg.h"
#include "rfwcdma_core_asdiv.h"
#include "rfc_common.h"
#include "rfcom.h"
#include "rfm_device_types.h"
#include "rfc_card_wcdma.h"
#include "rfcommon_core.h"
#include "rfwcdma_mc.h"
#include "rf_buffer_intf.h"
#include "rfwcdma_data.h"
#ifdef FEATURE_RF_HAS_QTUNER
#include "rfwcdma_core_antenna_tuner.h"
#endif 
#include "rflm_api_wcdma.h"
#include "rflm_dm_api.h"
#include "rflm_cmn_intf.h"
#include "rflm_cmn_asd.h"
#ifdef FEATURE_RF_ASDIV


/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines asdiv SSBI script size(max) for a Rx event 
*/
#define RF_WDMA_RX_ASDIV_SSBI_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines asdiv RFFE script size(max) for a Rx event
 
*/
#define RF_WDMA_RX_ASDIV_RFFE_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines asdiv GRFC script size(max) for a Rx event
*/
#define RF_WDMA_RX_ASDIV_GRFC_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
extern void rfwcdma_mc_create_rf_buff(rf_buffer_intf** buffer_ptr);

/*----------------------------------------------------------------------------*/
extern void rfwcdma_mc_destroy_rf_buff(rf_buffer_intf* buffer_ptr);
/*----------------------------------------------------------------------------*/
extern boolean rfwcdma_mdsp_associate_event_id(uint16 event_id,rflm_dm_handle_id_t rflm_handle);

/*============================================================================*/
/*!
  @name Antenna Switch Diversity

  @brief
  Functions related to antenna switch diversity
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Create GRFC Switch script
 
  @details
  Based on the immediate flag, this function will create the GRFC(only) switch script
  or will perform the immediate GRFC config.
 
  @param ant_sw_pos
  New Antenna Switch Position
 
  @param rf_buffer
  Pointer to buffer to store script
 
  @param cb_data
  Pointer to callback data

  @return
  Status indicating the success of the function execution  
*/
boolean rfwcdma_asdiv_create_grfc_script
(
  rfcommon_asdiv_position_type ant_sw_pos, /*!< New Antenna Position */
  rf_buffer_intf *rf_buffer, /*!< pointer to buffer to store Script */
  void *cb_data /*!< Callback data to be passed to the function */
)
{
  rfwcdma_asdiv_asm_script_data *asm_script_data;
  rfc_sig_cfg_type *rfc_asd_signals_ptr = NULL;
  boolean script_result = TRUE;
  rfm_device_enum_type device;
  rfcom_wcdma_band_type band;
  boolean immediate;

  /* Get the ASM script data */
  asm_script_data = (rfwcdma_asdiv_asm_script_data*)cb_data;

  /* NULL pointer check */
  if ( asm_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_asdiv_create_asm_script: NULL callback data" );
    return FALSE;
  } /* if ( asm_script_data == NULL ) */

  /* Populate params */
  device = asm_script_data->device;
  band = asm_script_data->band;
  immediate = asm_script_data->immediate;

  /* Get Signal List */
  rfc_asd_signals_ptr = 
    (rfc_sig_cfg_type *)rfc_wcdma_get_asd_xsw_config(device,band,ant_sw_pos);
  
  /* NULL Pointer check */
  if (rfc_asd_signals_ptr != NULL  )
  {
    /* Create Script */
    if ( immediate == FALSE )
    {
      script_result &= rfc_common_generate_grfc_script( rfc_asd_signals_ptr,
                                                        rf_buffer,
                                                        0 );  
      if ( script_result == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rfwcdma_asdiv_create_grfc_script: Device %d "
                  "SwitchPos %d - Script Gen failed", 
                  device, ant_sw_pos);

      }/* if ( script_result == FALSE ) */
      else
      {
        RF_MSG_2( RF_HIGH, "rfwcdma_asdiv_create_grfc_script: Device %d "
                  "SwitchPos %d - Script Gen SUCCESS", 
                  device, ant_sw_pos);
      }
    } /* if ( immediate == FALSE ) */
    else
    {
      /* Perform Immediate execution */
      script_result &= rfc_common_config_signal(rfc_asd_signals_ptr, TRUE /* gnss_blanking_enabled */);
      if ( script_result == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rfwcdma_asdiv_create_GRFC_script: Device %d "
                            "SwitchPos %d - IMM failed", device, ant_sw_pos);
      }/* if ( script_result == FALSE ) */
    } /* if ! ( immediate == FALSE ) */
  } /* if (rfc_asd_signals_ptr != NULL  ) */

  return script_result;
}

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Create Asdiv Switch script
 
  @details
  Based on the immediate flag, this function will create the Asdiv switch script
  or will perform the immediate Asdiv config. Script will include
  1.       RFFE ASDiv Scripts via RFFE XSW device
  2.       GRFC ASDiv Scripts via GRFC XSW device
  3.       GRFC ASDiv Scripts via RFC Signal list
 
  @param ant_sw_pos
  New Antenna Switch Position
 
  @param rf_buffer
  Pointer to buffer to store script
 
  @param cb_data
  Pointer to callback data

  @return
  Status indicating the success of the function execution  
*/
boolean 
rfwcdma_asdiv_create_asm_script
(

  rfcommon_asdiv_position_type ant_sw_pos, /*!< New Antenna Position */
  rf_buffer_intf *rf_buffer, /*!< pointer to buffer to store Script */
  void *cb_data /*!< Callback data to be passed to the function */)
{
  rfwcdma_asdiv_asm_script_data *asm_script_data;
  rfc_sig_cfg_type *rfc_asd_signals_ptr = NULL;
  boolean script_result = TRUE;
  rfm_device_enum_type device;
  rfcom_wcdma_band_type band;
  boolean immediate;

  /* Get the ASM script data */
  asm_script_data = (rfwcdma_asdiv_asm_script_data*)cb_data;

  /* NULL pointer check */
  if ( asm_script_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_asdiv_create_asm_script: NULL callback data" );
    return FALSE;
  } /* if ( asm_script_data == NULL ) */

  /* Populate params */
  device = asm_script_data->device;
  band = asm_script_data->band;
  immediate = asm_script_data->immediate;

  /* Get Signal List */
  rfc_asd_signals_ptr = 
    (rfc_sig_cfg_type *)rfc_wcdma_get_asd_xsw_config(device,band,ant_sw_pos);
  
  /* NULL Pointer check */
  if (rfc_asd_signals_ptr != NULL  )
  {
    /* Create Script */
    if ( immediate == FALSE )
    {
      script_result &= rfc_common_generate_grfc_script( rfc_asd_signals_ptr,
                                                        rf_buffer,
                                                        0 );  
      if ( script_result == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rfwcdma_asdiv_create_asm_script: Device %d "
                  "SwitchPos %d - Script Gen failed", 
                  device, ant_sw_pos);

      }/* if ( script_result == FALSE ) */
    } /* if ( immediate == FALSE ) */
    else
    {
      /* Perform Immediate execution */
      script_result &= rfc_common_config_signal(rfc_asd_signals_ptr, TRUE /* gnss_blanking_enabled */ );
      if ( script_result == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rfwcdma_asdiv_create_asm_script: Device %d "
                            "SwitchPos %d - IMM failed", device, ant_sw_pos);
      }/* if ( script_result == FALSE ) */
    } /* if ! ( immediate == FALSE ) */
  } /* if (rfc_asd_signals_ptr != NULL  ) */

  #ifdef FEATURE_RF_ASDIV  
  script_result &=rfc_wcdma_get_asdiv_xsw_script(ant_sw_pos,rf_buffer,cb_data);
  #endif

  RF_MSG_4( RF_HIGH, "rfwcdma_asdiv_create_asm_script: Device %d Band %d "
            "SwitchPos %d IMM %d - NULL signal List", 
            device, band, ant_sw_pos, immediate );

  return script_result;

} /* rfwcdma_update_asd_grfc_script */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Initiate Antenna Switching
 
  @details
  This function will be called via common RFM to perform antenna switching
  when WCDMA is the primary controlling tech. Based on the action flag, this
  function will either create switch script for the given device with the
  given antenna switch position; or perform immdiate hardware writes to
  perform antenna switch. Immediate hardware write would be primarily done
  for FTM mode testing.

  @param device
  RFM Device on which antenna position is set
 
  @param ant_sw_pos
  New Antenna Switch Position
 
  @param cb_func
  Callback function pointer passed by L1 to be called when switch is completed
 
  @param cb_data
  Callback function data
 
  @param immediate
  Flag indicating if a script is to be created or immediate action is to
  be done

  @return
  Status indicating the success of the function execution
*/
boolean 
rfwcdma_asdiv_handle_set_antenna
(
  rfm_device_enum_type device,
  rfcommon_asdiv_position_type ant_sw_pos,
  rfm_l1_cb_type cb_func,
  void *cb_data,
  boolean immediate
)
{

  rf_buffer_intf* ant_sw_script = NULL;
  rf_path_enum_type path = rfcommon_core_device_to_path(device);
  boolean ret_val = TRUE;
  rfcommon_asdiv_asm_conifg_type get_asm_script_cb;
  rfcommon_asdiv_send_script_type send_script_cb;
  rfcommon_asdiv_switch_start_notify_type switch_start_cb;
  rfwcdma_asdiv_asm_script_data asm_script_data;
  rfcommon_asdiv_update_tuner_type update_tuner_cb;

  /* AsDiv Rxlm handle ID*/
  uint32 asd_rxlm_handle;

  asd_rxlm_handle = rfwcdma_data_get_rflm_buf_idx(device);

  if ( rfcommon_asdiv_get_current_position(device) ==  ant_sw_pos )
  {
   RF_MSG_2( RF_HIGH, "rfwcdma_asdiv_handle_set_antenna: "
                      "Current position %d is same as the position requested %d", 
                       rfcommon_asdiv_get_current_position(device) , 
                       ant_sw_pos );
   return ret_val;
  }
    /*! allocate asd memory*/
  rflm_cmn_asd_allocate_memory(asd_rxlm_handle);

  /* Continue only if ret_val is TRUE */
  if ( ret_val == TRUE )
  {
    /* Create rf buffer and clear */
    rfwcdma_mc_create_rf_buff(&ant_sw_script);
    (void)rf_buffer_clear(ant_sw_script);  

    asm_script_data.device = device;
    asm_script_data.band = rfwcdma_data_get_curr_band(device);
    asm_script_data.immediate = immediate;

    /* Populate the script callback function data */
    get_asm_script_cb.cb_func = rfwcdma_asdiv_create_asm_script;
    get_asm_script_cb.cb_data = &asm_script_data;
    if ( immediate == TRUE )
    {
      send_script_cb.cb_func = NULL;
      update_tuner_cb.cb_func = NULL;
      update_tuner_cb.cb_data = FALSE;
    }
    else
    {
      send_script_cb.cb_func = rfcommon_asdiv_manager_send_script;
      #ifdef FEATURE_RF_HAS_QTUNER
      update_tuner_cb.cb_func = rfwcdma_core_antenna_tuner_incremental_update_imm;
      #else
      update_tuner_cb.cb_func = NULL;
      #endif
      
      update_tuner_cb.cb_data = TRUE;
    }
    send_script_cb.cb_data = NULL;
    switch_start_cb.cb_func = NULL;
    switch_start_cb.cb_data = NULL;

    /* Initiate Switch */
    ret_val &= rfcommon_asdiv_manager_config_switch( device,
                                                     RFM_IMT_MODE,
                                                     ant_sw_pos, 
                                                     ant_sw_script, 
                                                     ant_sw_script,
                                                     get_asm_script_cb,
                                                     send_script_cb,
                                                     switch_start_cb,
                                                     update_tuner_cb,
                                                     cb_func, 
                                                     cb_data, 
                                                      FALSE, 
                                                     immediate, 0, 
                                                     asd_rxlm_handle);
    
    /* Destroy the Buffer */
    rfwcdma_mc_destroy_rf_buff(ant_sw_script);

    RF_MSG_4( RF_HIGH, "rfwcdma_asdiv_handle_set_antenna: [Status %d] "
              "Device %d SwitchPos %d, IMM %d Done", ret_val, device,
              ant_sw_pos, 
              immediate );

  } /* if ( ret_val == TRUE ) */
  else
  {
    RF_MSG_4( RF_ERROR, "rfwcdma_asdiv_handle_set_antenna: [Status %d] "
              "Device %d SwitchPos %d, IMM %d", ret_val, device, ant_sw_pos, 
              immediate );
  }

  return ret_val;

} /* rfwcdma_mc_handle_set_antenna */


/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Abort AsD
 
  @details
  call rflm api to abort pending events in AsDiv
  
  @param handle
  Rxlm buffer index used to handle the pending events   
  
  @return
  
*/
void rfwcdma_core_asdiv_abort (uint32 handle)
{
  rflm_cmn_asd_abort_pending_event((uint32)handle);
  RF_MSG_1( RF_HIGH, "rfwcdma_asdiv_abort: [handle %d] ", handle);

} /* rfwcdma_asdiv_abort */

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Util function get the Asdiv device script type
 
  @details
  Util function get the Asdiv device script type
  
  @param handle
  Asdiv data  
  
  @return
  Script bus type
  
*/
rf_hal_bus_resource_type rfwcdma_core_asdiv_get_script_type(rfwcdma_asdiv_asm_script_data asdiv_data)
{
  rf_buffer_intf *temp_script_buffer =  NULL;
  const rf_hal_bus_resource_script_type* script_ptr = NULL;
  rf_hal_bus_resource_type bus_resource = 0;

  temp_script_buffer = rf_buffer_create(RF_WDMA_RX_ASDIV_SSBI_SCRIPT_SIZE,
                                        RF_WDMA_RX_ASDIV_RFFE_SCRIPT_SIZE,
                                        RF_WDMA_RX_ASDIV_GRFC_SCRIPT_SIZE);

  if ( temp_script_buffer == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_asdiv_get_script_type, fail to create buffer ");
    return RF_HAL_BUS_RESOURCE_RFFE;
  }
  
  rf_buffer_clear(temp_script_buffer);

  rfc_wcdma_get_asdiv_xsw_script(0,temp_script_buffer,(void*)&asdiv_data);

  for (bus_resource = RF_HAL_BUS_RESOURCE_SSBI; bus_resource < RF_HAL_BUS_RESOURCE_MAX; bus_resource++)
  {
    script_ptr = rf_buffer_get_script_ptr(temp_script_buffer, bus_resource);
    if (script_ptr != NULL && script_ptr->num_trans > 0)
    {
      rf_buffer_destroy(temp_script_buffer);
      /* valid RF script found*/
      return bus_resource;
    }
  }

  rf_buffer_destroy(temp_script_buffer);

  RF_MSG( RF_HIGH, "No xsw device Asdiv script ");

  return RF_HAL_BUS_RESOURCE_RFFE;

}

/*! @} */

#endif


