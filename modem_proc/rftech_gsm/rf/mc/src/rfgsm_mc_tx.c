

/*!
   @file
   rfgsm_mc_tx.c

   @brief
   This file contains all type declarations and definitions for the GSM Main
   Control layer Tx.


   @details
   
*/

/*===========================================================================
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/mc/src/rfgsm_mc_tx.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/28/15   sc      Enable thermistor and vbatt reads
05/19/15   sc      Check GL1 pass valid Tx device in Tx wakeup and sleep
03/31/15   sc      Add Tx wakeup event build and RFLM index APIs
03/30/15   sc      Add Tx timing update functionality
03/25/15   sc      Add core Rx Wakeup/Sleep APIs
03/12/15   sc      Initial version of GSM Main Control Tx

============================================================================*/

#include "msg.h"
#include "rfcom.h"
#include "rfgsm_mc_tx.h"
#include "rfgsm_core_tx.h"
#include "rfgsm_core_temp_comp.h"



/*=============================================================================

                      STATIC AND EXTERN FUNCTION PROTOTYPES

=============================================================================*/



/*=============================================================================

                                DEFINITIONS

=============================================================================*/




/*=============================================================================

                               FUNCTION DEFINITIONS

=============================================================================*/
/*! @fn rfgsm_mc_tx_wakeup                                                   */
/*! @brief This function initialises the device for GSM mode Tx capability   */
/*! @param  sub_id : subscription to wakeup for                              */
/*! @param  tx_device : Tx device waking up on                               */
/*! @param  txlm_buf_index : TxLM index allocated for modem settings         */
/*! @param  gfw_buff_ptr : pointer to GFW shared memory for interface index  */
/*! @param  timing_info : GL1 timing info pointer to populate with Tx timing */
/*! @param  therm_read_buff_ptr : GFW therm read pointer                     */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
/*! @TODO: Reduce number of arguments to this function */
rfgsm_mc_status_type rfgsm_mc_tx_wakeup( uint8 sub_id,
                                         rfcom_device_enum_type tx_device,
                                         uint32 txlm_buf_index,
                                         void * gfw_buff_ptr,
                                         void * therm_read_buff_ptr,
                                         rfgsm_timing_info_type * timing_info )
{
  /* Validate the Tx device */
  if( !rfgsm_core_get_device_tx_capability( tx_device ) )
  {
    ERR_FATAL("rfgsm_mc_tx_wakeup() GL1 passed RF invalid tx device ID %d for sub %d", tx_device, sub_id, 0 );
  }

  /* Update the therm read script and store the UMIDs for this device */
  if( !rfgsm_core_prepare_therm_read( sub_id, tx_device, therm_read_buff_ptr ) )
  {
    ERR_FATAL("rfgsm_mc_tx_wakeup() prepare therm read failed for sub %d dev %d", sub_id, tx_device, 0 );
  }

  /* Update timing information based on worst case for PRx and DRx */
  if( !rfgsm_core_tx_wakeup_timing_update( sub_id,
                                           tx_device,
                                           timing_info ) )
  {
    RF_MSG_2( MSG_LEGACY_ERROR, 
              "rfgsm_core_tx_wakeup_timing_update() failed for sub %d, dev %d", 
              sub_id, tx_device );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  /* Execute core Tx wakeup */
  if( !rfgsm_core_tx_wakeup(sub_id, tx_device, txlm_buf_index) )
  {
    RF_MSG_3( MSG_LEGACY_ERROR, 
              "rfgsm_mc_tx_wakeup() failed for sub %d, Tx dev %d, Tx RFLM handle %d", 
              sub_id, tx_device, txlm_buf_index );
    return RFGSM_MC_CORE_API_FAILURE;
  }
  
  /* Build scripts into HW events for RFLM processing */
  if( !rfgsm_core_tx_wakeup_build( sub_id, tx_device, gfw_buff_ptr ) )
  {
    RF_MSG_2( MSG_LEGACY_ERROR, 
              "rfgsm_core_tx_wakeup_build() failed for sub %d, dev %d", 
              sub_id, tx_device );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_tx_sleep                                                    */
/*! @brief This function puts the device to sleep from GSM Tx mode           */
/*! @param  sub_id : subscription to sleep for                               */
/*! @param  tx_device : Tx device sleeping on                                */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_tx_sleep( uint8 sub_id,
                                        rfcom_device_enum_type tx_device )
{
  /* Validate the Tx device */
  if( !rfgsm_core_get_device_tx_capability( tx_device ) )
  {
    ERR_FATAL("rfgsm_mc_tx_sleep() GL1 passed RF invalid tx device ID %d for sub %d", tx_device, sub_id, 0 );
  }

  /* Execute core Tx sleep */
  if (!rfgsm_core_tx_sleep(sub_id, tx_device))
  {
    RF_MSG_2( MSG_LEGACY_ERROR, 
              "rfgsm_mc_tx_sleep() failed for sub %d, Tx dev %d", 
              sub_id, tx_device );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  return RFGSM_MC_SUCCESS;
}




/*===========================================================================*/
/*! @fn rfgsm_mc_process_therm_read_rsp                                      */
/*! @brief This function processes the ther read reponse data                */
/*! @param  core_data : struct containing core data populated in cmd proc    */
/*! @param  tag : ID to tag the therm read sent with response                */
/*! @param  therm_read_ok : if therm read is valid                           */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_process_therm_read_rsp( rfgsm_core_data_type * core_data, 
                                                      uint8 tag, 
                                                      boolean therm_read_ok )
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  if( !rfgsm_core_process_therm_read_rsp(core_data, tag, therm_read_ok) )
  {
    RF_MSG( MSG_LEGACY_ERROR, "rfgsm_core_process_therm_read_rsp() Failed" );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  return RFGSM_MC_SUCCESS;
}



/*===========================================================================*/
/*! @fn rfgsm_mc_stop_therm_read                                             */
/*! @brief This function stops the timer triggering therm reads              */
/*! @param  tx_device : device to stop triggering therm read for             */
/*! @retval rfgsm_mc_status_type : success/failure status of the MC API      */
rfgsm_mc_status_type rfgsm_mc_stop_therm_read( rfm_device_enum_type tx_device )
{
  /* Stop therm read 2sec timer */
  rfgsm_core_temp_comp_stop( tx_device );

  return RFGSM_MC_SUCCESS;
}
