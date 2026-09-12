
/*!
   @file
   rfgsm_mc_rx.c

   @brief
   This file contains all type declarations and definitions for the GSM Main
   Control layer Rx.


   @details
   
*/

/*===========================================================================
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/mc/src/rfgsm_mc_rx.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/14/15   ec      Pass op_mode into rfgsm_core_rx_sleep so that Wakeup event will
                   only be cleared when PRx device is sleeping
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
08/07/15   sc      Update GL1 desense channel list on PRx wakeup
07/13/15   sc      Validate PRx and/or DRx devices on Rx Wakeup/Sleep/Burst
07/07/15   sc      Further Rx burst optimisations (~20% @ 144MHz)
06/19/15   sc      Update and enable optmised Rx burst and feature guard
05/27/15   sc      Calculate Rx burst data for optimised Rx burst API
05/21/15   sc      Add new Rx burst core API hooks
04/22/15   zbz     WTR interface change to support DRx
04/22/15   sc      Introduce new Rx API
04/16/15   sc      Manage RFM mode
04/10/15   zbz     Interface changes with device driver for split Rx/Tx
03/30/15   sc      Add Rx timing update functionality
03/26/15   sc      Add legacy functionality to new Rx/Tx Wakeup/Sleep call-flow
03/25/15   sc      Add core Rx Wakeup/Sleep APIs
03/12/15   sc      Initial version of GSM Main Control Rx

============================================================================*/

#include "msg.h"
#include "rfcom.h"
#include "rfgsm_mc_rx.h"
#include "rfgsm_core_rx.h"
#include "rfgsm_msg.h"



/*=============================================================================

                      STATIC AND EXTERN FUNCTION PROTOTYPES

=============================================================================*/
extern rfcom_mode_enum_type * rfgsm_mode;

extern int32 rfm_exit_mode( rfcom_device_enum_type device,
                            rfcom_mode_enum_type to_rfmode );

rfgsm_mc_status_type rfgsm_mc_rx_prepare( uint8 burst_index, 
                                          rfgsm_mc_rx_data_type * rx_data );

static boolean rfgsm_mc_rx_validate_devices( rfcom_device_enum_type prx_device,
                                             rfcom_device_enum_type drx_device,
                                             rfgsm_op_mode_type op_mode );



/*=============================================================================

                                DEFINITIONS

=============================================================================*/



/*=============================================================================

                                DECLARATIONS

=============================================================================*/
/*! @var rfgsm_mc_rx_crit_sect                                               */
/*! @brief variable to lock the Rx burst building                            */
/*! @details 2 GSM subscriptions can prepare Rx bursts at the same time for  */
/*!          the same device and therefore the operation needs to be mutexed */
static rf_lock_data_type rfgsm_mc_rx_crit_sect[RFM_MAX_WAN_DEVICES];



/*=============================================================================

                               FUNCTION DEFINITIONS

=============================================================================*/
/*! @fn rfgsm_mc_rx_wakeup                                                   */
/*! @brief This function initialises the device for GSM mode Rx capability   */
/*! @param  sub_id : subscription to wakeup for                              */
/*! @param  prx_device : primary Rx device waking up on                      */
/*! @param  drx_device : diversity Rx device waking up on                    */
/*! @param  op_mode : PRx, DRx, or PRx+DRx wakeup mode                       */
/*! @param  rxlm_buf_index : RxLM index allocated for modem settings         */
/*! @param  gfw_buff_ptr : pointer to GFW shared memory for interface index  */
/*! @param  timing_info : GL1 timing info pointer to populate with Rx timing */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
/*! @TODO: Reduce number of arguments to this function */
rfgsm_mc_status_type rfgsm_mc_rx_wakeup( uint8 sub_id,
                                         rfcom_device_enum_type prx_device,
                                         rfcom_device_enum_type drx_device,
                                         rfgsm_op_mode_type op_mode,
                                         rfgsm_rxlm_buf_index_type * rxlm_buf_index,
                                         void * gfw_buff_ptr,
                                         rfgsm_timing_info_type * timing_info )
{
  /* Validate devices */
  if( !rfgsm_mc_rx_validate_devices( prx_device, drx_device, op_mode ) )
  {
    /* Error fatal as cannot continue with non Rx-capable device IDs */
    ERR_FATAL( "rfgsm_mc_rx_wakeup() GL1 passed RF invalid device PRx dev %d, DRx dev %d, op mode %d",
               prx_device, drx_device, op_mode );
  }

  /* Update timing information based on worst case for PRx and DRx */
  if( !rfgsm_core_rx_wakeup_timing_update( sub_id,
                                           prx_device,
                                           drx_device,
                                           op_mode,
                                           timing_info ) )
  {
    RF_MSG_4( MSG_LEGACY_ERROR, 
              "rfgsm_core_rx_wakeup_timing_update() failed for sub %d, PRx dev %d, DRx dev %d, op_mode %d", 
              sub_id, prx_device, drx_device, op_mode );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  /* Execute Rx Wakeup for primary if PRx mode or PRx+DRx mode */
  if( op_mode == RFGSM_PRX_OP_MODE ||
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* If the last mode was not GSM ensure to exit that mode first */
    if(rfgsm_mode[prx_device] != RFCOM_GSM_MODE)
    {
      (void)rfm_exit_mode(prx_device, RFCOM_GSM_MODE);
    }

    /* Execute core Rx wakeup for primary device */
    if( !rfgsm_core_rx_wakeup(sub_id, prx_device, rxlm_buf_index->prx_idx, RFDEVICE_PRX_OPER_MODE) )
    {
      RF_MSG_4( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_wakeup() failed for sub %d, PRx dev %d, PRx RFLM handle %d, op_mode %d", 
                sub_id, prx_device, rxlm_buf_index->prx_idx, op_mode );
      return RFGSM_MC_CORE_API_FAILURE;
    }

    /* Update desense channel list for PRx wakeup */
    if( !rfgsm_core_rx_update_desense_chan_list( sub_id, prx_device ) )
    {
      RF_MSG_2( MSG_LEGACY_ERROR, 
                "rfgsm_mc_rx_wakeup() update desense chan list failed for sub %d, PRx dev %d", 
                sub_id, prx_device );
      return RFGSM_MC_CORE_API_FAILURE;
    }
  }

  /* Execute Rx Wakeup for diversity if DRx mode or PRx+DRx mode */
  if( op_mode == RFGSM_DRX_OP_MODE ||
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* If the last mode was not GSM ensure to exit that mode first */
    if(rfgsm_mode[drx_device] != RFCOM_GSM_MODE)
    {
      (void)rfm_exit_mode(drx_device, RFCOM_GSM_MODE);
    }

    /* Execute core Rx wakeup for primary device */
    if( !rfgsm_core_rx_wakeup(sub_id, drx_device, rxlm_buf_index->drx_idx, RFDEVICE_DRX_OPER_MODE) )
    {
      RF_MSG_4( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_wakeup() failed for sub %d, DRx dev %d, DRx RFLM handle %d, op_mode %d", 
                sub_id, drx_device, rxlm_buf_index->drx_idx, op_mode );
      return RFGSM_MC_CORE_API_FAILURE;
    }
  }

  /* Build scripts into HW events for RFLM processing */
  if( !rfgsm_core_rx_wakeup_build( sub_id,
                                   prx_device,
                                   drx_device,
                                   op_mode,
                                   gfw_buff_ptr ) )
  {
    RF_MSG_4( MSG_LEGACY_ERROR, 
              "rfgsm_core_rx_wakeup_build() failed for sub %d, PRx dev %d, DRx dev %d, op_mode %d", 
              sub_id, prx_device, drx_device, op_mode );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  /* Set the rfm mode */
  if( op_mode == RFGSM_PRX_OP_MODE || 
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rfgsm_mode[prx_device] = RFCOM_GSM_MODE;
    rfgsm_mode[drx_device] = RFCOM_GSM_MODE;
  }
  else if( op_mode == RFGSM_DRX_OP_MODE )
  {
    rfgsm_mode[drx_device] = RFCOM_GSM_MODE;
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx_sleep                                                    */
/*! @brief This function puts the device to sleep from GSM Rx mode           */
/*! @param  sub_id : subscription to sleep for                               */
/*! @param  prx_device : primary Rx device sleeping on                       */
/*! @param  drx_device : diversity Rx device sleeping on                     */
/*! @param  op_mode : PRx, DRx, or PRx+DRx sleep mode                        */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_rx_sleep( uint8 sub_id,
                                        rfcom_device_enum_type prx_device,
                                        rfcom_device_enum_type drx_device,
                                        rfgsm_op_mode_type op_mode )
{
  /* Validate devices */
  if( !rfgsm_mc_rx_validate_devices( prx_device, drx_device, op_mode ) )
  {
    /* Error fatal as cannot continue with non Rx-capable device IDs */
    ERR_FATAL( "rfgsm_mc_rx_sleep() GL1 passed RF invalid device PRx dev %d, DRx dev %d, op mode %d",
               prx_device, drx_device, op_mode );
  }

  if( op_mode == RFGSM_DRX_OP_MODE ||
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* Check RFM mode for DRx device has not been changed during GSM activity */
    if( rfgsm_mode[drx_device] != RFCOM_GSM_MODE )
    {
      RF_MSG_4( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_sleep() drx dev %d not in GSM mode for sub %d, op_mode %d, mode is %d", 
                drx_device, sub_id, op_mode, rfgsm_mode[drx_device] );
      return RFGSM_MC_CORE_API_FAILURE;
    }

    /* Execute core Rx sleep for diversity device */
    if( !rfgsm_core_rx_sleep(sub_id, drx_device, op_mode) )
    {
      RF_MSG_3( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_sleep() failed for sub %d, drx dev %d, op_mode %d", 
                sub_id, drx_device, op_mode );
      return RFGSM_MC_CORE_API_FAILURE;
    }
  }

  if( op_mode == RFGSM_PRX_OP_MODE ||
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* Check RFM mode for PRx device has not been changed during GSM activity */
    if( rfgsm_mode[prx_device] != RFCOM_GSM_MODE )
    {
      RF_MSG_4( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_sleep() prx dev %d not in GSM mode for sub %d, op_mode %d, mode is %d", 
                prx_device, sub_id, op_mode, rfgsm_mode[prx_device] );
      return RFGSM_MC_CORE_API_FAILURE;
    }

    /* Execute core Rx sleep for primary device */
    if( !rfgsm_core_rx_sleep(sub_id, prx_device, op_mode) )
    {
      RF_MSG_3( MSG_LEGACY_ERROR, 
                "rfgsm_core_rx_sleep() failed for sub %d, prx dev %d, op_mode %d", 
                sub_id, prx_device, op_mode );
      return RFGSM_MC_CORE_API_FAILURE;
    }
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx                                                          */
/*! @brief This function builds Rx scripts for processing                    */
/*! @details Based on parameters passed from GL1, RF will build upto 12 Rx's */
/*!          in a loop 1 at a time.                                          */
/*! @param  sub_id : subscription to sleep for                               */
/*! @param  prx_device : primary Rx device sleeping on                       */
/*! @param  drx_device : diversity Rx device sleeping on                     */
/*! @param  op_mode : PRx, DRx, or PRx+DRx sleep mode                        */
/*! @param  rx_burst_params : Rx parameters from GL1 per burst               */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_rx( rfgsm_mc_rx_data_type * rx_data )
{
  /* Initialise variables */
  uint32 index = 0;
  rfgsm_mc_status_type status = RFGSM_MC_CORE_API_FAILURE;
  uint8 sub_id = rx_data->sub_id;
  rfcom_device_enum_type prx_device = rx_data->prx_device;
  rfcom_device_enum_type drx_device = rx_data->drx_device;
  rfgsm_op_mode_type op_mode = rx_data->op_mode;
  uint8 buffer_id = rx_data->buffer_id;
  rfa_rf_gsm_rx_burst_type * rx_burst_params = rx_data->rx_burst_params;

  /* Validate devices */
  if( !rfgsm_mc_rx_validate_devices( prx_device, drx_device, op_mode ) )
  {
    /* Error fatal as cannot continue with non Rx-capable device IDs */
    ERR_FATAL( "rfgsm_mc_rx() GL1 passed RF invalid device PRx dev %d, DRx dev %d, op mode %d",
               prx_device, drx_device, op_mode );
  }

  /* Check for valid num bursts */
  if( rx_burst_params->num_rx_bursts == 0 || 
      rx_burst_params->num_rx_bursts > RFA_RF_GSM_MAX_RX_ACTIVITIES )
  {
    RF_MSG_4( MSG_LEGACY_ERROR, 
              "rfgsm_mc_rx() invalid num bursts for sub %d, PRx dev %d, DRx dev %d, op_mode %d", 
              sub_id, prx_device, drx_device, op_mode );
    return status;
  }

  /* Checks if GL1 have not assigned the GFW shared memory ptr */
  for( index = 0; index < rx_burst_params->num_rx_bursts; index++ )
  {
    /* Check GL1 pass a valid GFW shared mem pointer */
    if( rx_burst_params->gfw_rf_burst_event[index] == NULL )
    {
      RF_MSG_5( MSG_LEGACY_ERROR, 
                "rfgsm_mc_rx() GL1 pass GRF NULL GFW ptr for Rx %d, sub %d, PRx dev %d, DRx dev %d, op_mode %d", 
                index, sub_id, prx_device, drx_device, op_mode );
      return status;
    }
  }

  /* Get device-specific lock/s for Rx burst building */
  rf_common_enter_critical_section(&rfgsm_mc_rx_crit_sect[prx_device]);
  if( op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rf_common_enter_critical_section(&rfgsm_mc_rx_crit_sect[drx_device]);
  }

  RFGSM_MC_PROC_LOG(RFGSM_MC_RX_PREPARE_START, sub_id, 0, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Main Rx creation loop */
  for( index = 0; index < rx_burst_params->num_rx_bursts; index++ )
  {
    /* Only if execution fails does status become failure and break */
    status = RFGSM_MC_SUCCESS;

    /* If this Rx has been blanked go to next */
    /* @TODO commented out until GL1 use this flag
    if( rx_burst_params->rx_chan_params[index].blanked == TRUE )
    {
      RF_MSG_5( MSG_LEGACY_HIGH,
                "rfgsm_mc_rx() GL1 blanked Rx %d for sub %d, PRx dev %d, DRx dev %d, op_mode %d", 
                index, sub_id, prx_device, drx_device, op_mode );
      continue;
    }
    */

    /* Execute the calculations and script building of the Rx burst */
    if( !rfgsm_mc_rx_prepare( index, rx_data ) )
    {
      /* Core execution failed so break with failure status */
      status = RFGSM_MC_CORE_API_FAILURE;
      break;
    }
  } //for( index = 0; index < rx_burst_params->num_rx_bursts; index++ )

  RFGSM_MC_PROC_LOG(RFGSM_MC_RX_PREPARE_STOP, sub_id, 0, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Release device-specific lock/s for Rx burst building */
  if( op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rf_common_leave_critical_section(&rfgsm_mc_rx_crit_sect[drx_device]);
  }
  rf_common_leave_critical_section(&rfgsm_mc_rx_crit_sect[prx_device]);

  return status;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx_prepare                                                  */
/*! @brief Function for calculating and building Rx scripts                  */
/*! @details This function is called per Rx burst                            */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_rx_prepare( uint8 burst_index, 
                                          rfgsm_mc_rx_data_type * rx_data )
{
  /* Initialise variables */
  boolean core_rx_status = FALSE;
  rfgsm_mc_status_type mc_rx_prepare_status = RFGSM_MC_ERROR;
  uint8 sub_id = rx_data->sub_id;
  rfcom_device_enum_type prx_device = rx_data->prx_device;
  rfcom_device_enum_type drx_device = rx_data->drx_device;
  rfgsm_op_mode_type op_mode = rx_data->op_mode;
  uint8 buffer_id = rx_data->buffer_id;
  rfa_rf_gsm_rx_burst_type * rx_burst_params = rx_data->rx_burst_params;

  /* Calculate and prepare PRx burst */
  if( rfgsm_core_rx_prepare( sub_id, 
                             prx_device, 
                             rx_burst_params, 
                             buffer_id, 
                             burst_index ) )
  {
    RFGSM_MC_PROC_LOG(RFGSM_MC_RX_PRX_COMPLETE, sub_id, burst_index, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

    /* Execute Rx for diversity if PRx+DRx mode */
    if( op_mode == RFGSM_PRX_DRX_OP_MODE )
    {
      /* Calculate and prepare DRx burst */
      if( rfgsm_core_rx_diversity_prepare( sub_id, 
                                           drx_device, 
                                           rx_burst_params, 
                                           buffer_id, 
                                           burst_index ) )
      {
        RFGSM_MC_PROC_LOG(RFGSM_MC_RX_DRX_COMPLETE, sub_id, burst_index, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

        /* PRx + DRx were both successful */
        core_rx_status = TRUE;
      }
      else
      {
        /* DRx failed therefore print error and keep core status FALSE */
        RF_MSG_4( MSG_LEGACY_ERROR, 
                  "rfgsm_mc_rx() rfgsm_core_rx_diversity failed for op mode %d, sub %d, PRx dev %d, DRx dev %d", 
                  op_mode, sub_id, prx_device, drx_device );
      }
    }
    else
    {
      /* PRx-only was successful */
      core_rx_status = TRUE;
    }

    /* If core PRx or core PRx + DRx pass, build scripts and events */
    if( core_rx_status == TRUE )
    {
      if( rfgsm_core_rx_build_scripts( sub_id, 
                                       prx_device, 
                                       drx_device, 
                                       rx_burst_params,
                                       (op_mode == RFGSM_PRX_DRX_OP_MODE),
                                       buffer_id,
                                       burst_index ) )
      {
        RFGSM_MC_PROC_LOG(RFGSM_MC_RX_BUILDING_COMPLETE, sub_id, burst_index, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

        /* Successful execution */
        mc_rx_prepare_status = RFGSM_MC_SUCCESS;
      }
      else
      {
        RF_MSG_4( MSG_LEGACY_ERROR, 
                  "rfgsm_mc_rx() rfgsm_core_rx_build failed for op mode %d, sub %d, PRx dev %d, DRx dev %d", 
                  op_mode, sub_id, prx_device, drx_device );
      }
    }
  }
  else
  {
    RF_MSG_4( MSG_LEGACY_ERROR, 
              "rfgsm_mc_rx() rfgsm_core_rx failed for op mode %d, sub %d, PRx dev %d, DRx dev %d", 
              op_mode, sub_id, prx_device, drx_device );
  }

  return mc_rx_prepare_status;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx_init_once                                                */
/*! @brief Function for initialising any Rx-specific data one time only      */
/*! @details This function is called once at mc_init and is used to          */
/*!          initialise any data that is Rx specific.                        */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_rx_init_once( void )
{
  /* Initialise variables */
  rfcom_device_enum_type device;

  /* Create an Rx mutex lock per RFM logical device */
  for( device = RFM_DEVICE_0; device < RFM_MAX_WAN_DEVICES; device++ )
  {
    rf_common_init_critical_section(&rfgsm_mc_rx_crit_sect[device]);
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx_destroy                                                  */
/*! @brief Function for destroying any Rx-specific data one time only        */
/*! @details This function is called once at mc_deinit and is used to        */
/*!          destroy any data that is Rx specific.                           */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_rx_destroy( void )
{
  /* Initialise variables */
  rfcom_device_enum_type device;

  /* Destroy the Rx mutex lock for each RFM logical device */
  for( device = RFM_DEVICE_0; device < RFM_MAX_WAN_DEVICES; device++ )
  {
    rf_common_deinit_critical_section(&rfgsm_mc_rx_crit_sect[device]);
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_rx_validate_devices                                         */
/*! @brief Function verifies if the device/s are valid                       */
/*! @details This function checks if the devices from GL1 are valid          */
/*!          If PRx or PRx+DRx mode the PRx is checked, if DRx or PRx+DRx    */
/*!          mode the DRx is checked. There is a bad return if core handle is*/
/*!          null or the device is not Rx capable.                           */
/*! @retval boolean : success/failure status of the MC API                   */
boolean rfgsm_mc_rx_validate_devices( rfcom_device_enum_type prx_device,
                                      rfcom_device_enum_type drx_device,
                                      rfgsm_op_mode_type op_mode )
{
  /* Initialise status variable */
  boolean status = TRUE;

  /* Validate the PRx device if PRx or PRx+DRx modes */
  if( (op_mode == RFGSM_PRX_OP_MODE) ||
      (op_mode == RFGSM_PRX_DRX_OP_MODE) )
  {
    /* Validate the Rx device */
    if( !rfgsm_core_get_device_rx_capability( prx_device ) )
    {
      RF_MSG_1( MSG_LEGACY_ERROR, 
                "rfgsm_mc_rx_validate_devices() PRx dev %d not Rx capable", 
                prx_device );
      status &= FALSE;
    }
  }

  /* Validate the DRx device if DRx or PRx+DRx modes */
  if( op_mode == RFGSM_DRX_OP_MODE ||
      op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* Validate the Rx device */
    if( !rfgsm_core_get_device_rx_capability( drx_device ) )
    {
      RF_MSG_1( MSG_LEGACY_ERROR, 
                "rfgsm_mc_rx_validate_devices() DRx dev %d not Rx capable", 
                drx_device );
      status &= FALSE;
    }
  }

  return status;
}
