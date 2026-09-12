/*!
  @file
  rfgsm_core_intf.cpp

  @brief
  RF GSM core shim to interface with CPP files within core
 
  @details
  This file is written in "C" style as it is the shim to CPP state machine
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_intf.cpp#1 $
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
08/20/15   sc      Add framework for split Rx/Tx state machines
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
10/31/14   sw/sc   Initial version

===========================================================================*/
#include "rfcom.h"
#include "msg.h"

#include "rfgsm_core_intf.h"
#include "rfgsm_core_state_machine.h"
#include "rfm_device_types.h"



/*===========================================================================*/
/*                                                                           */
/*                                MACROS                                     */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                              DEFINTIONS                                   */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                             DECLARATIONS                                  */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                          FUNCTION PROTOTYPES                              */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                         FUNCTION DEFINITIONS                              */
/*                                                                           */
/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_init_sub                                          */
/*! @brief initialise the state machine for all subscriptions                */
/*! @details This API should be called once in rfgsm_mc_init                 */
/*! @param void                                                              */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_init_sub( void )
{
  /* Create and initialise all the subscription state machines */
  for (uint32 sub_id = 0; sub_id < RFGSM_SM_MAX_SUBS; sub_id++ )
  {
    RfgsmCoreSmInitSub( sub_id );
  }

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_destroy_sub                                       */
/*! @brief destory the state machines for all subscriptions                  */
/*! @details This API should be called once in rfgsm_mc_deinit               */
/*! @param void                                                              */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_destroy_sub( void )
{
  /* Destroy all the subscription state machines */
  for (uint32 sub_id = 0; sub_id < RFGSM_SM_MAX_SUBS; sub_id++ )
  {
    RfgsmCoreSmDeinitSub( sub_id );
  }

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_init_rx                                           */
/*! @brief initialise the state machine for a specific Rx device             */
/*! @details This API should be called once in rfgsm_mc_init for each valid  */
/*!          Rx capable device                                               */
/*! @param rx_device - Rx device to create a state machine for               */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_init_rx( rfm_device_enum_type rx_device )
{
  /* Validate the device ID */
  if( rx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_init_rx() invalid Rx device %d", rx_device, 0, 0);
    return FALSE;
  }

  /* Create and initialise Rx state machine */
  RfgsmCoreSmInitRx( rx_device );

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_destroy_rx                                        */
/*! @brief destory the state machines for the specified rx device            */
/*! @details This API should be called once in rfgsm_mc_deinit               */
/*! @param rx_device - Rx device state machine to destroy                    */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_destroy_rx( rfm_device_enum_type rx_device )
{
  /* Validate the device ID */
  if( rx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_destroy_rx() invalid Rx device %d", rx_device, 0, 0);
    return FALSE;
  }

  /* Destroy Rx state machine */
  RfgsmCoreSmDeinitRx( rx_device );

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_init_tx                                           */
/*! @brief initialise the state machine for a specific Tx device             */
/*! @details This API should be called once in rfgsm_mc_init for each valid  */
/*!          Tx capable device                                               */
/*! @param void                                                              */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_init_tx( rfm_device_enum_type tx_device )
{
  /* Validate the device ID */
  if( tx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_init_tx() invalid Tx device %d", tx_device, 0, 0);
    return FALSE;
  }

  /* Create and initialise Tx state machine */
  RfgsmCoreSmInitTx( tx_device );

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_destroy_tx                                        */
/*! @brief destory the state machines for the specified tx device            */
/*! @details This API should be called once in rfgsm_mc_deinit               */
/*! @param tx_device - Tx device state machine to destroy                    */
/*! @retval boolean - success flag                                           */
boolean rfgsm_core_intf_sm_destroy_tx( rfm_device_enum_type tx_device )
{
  /* Validate the device ID */
  if( tx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_destroy_tx() invalid Tx device %d", tx_device, 0, 0);
    return FALSE;
  }

  /* Destroy Tx state machine */
  RfgsmCoreSmDeinitTx( tx_device );

  return TRUE;
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_drive_sub                                         */
/*! @brief Drive the state machine to manage states and transitions          */
/*! @details The API gets the subscription-specific state machine object and */
/*!          tries to drive the state machine with an input.                 */
/*!          All checks and errors are handled in the state machine module.  */
/*! @param sub_id - subscription of the state machine to drive               */
/*! @param input - the input to drive the state machine                      */
/*! @retval void                                                             */
void rfgsm_core_intf_sm_drive_sub( uint8 sub_id,
                                   RfgsmCoreSmSubInputType input )
{
  /* Validate the Sub ID */
  if( sub_id >= RFGSM_SM_MAX_SUBS )
  {
    ERR_FATAL("rfgsm_core_intf_sm_drive_sub() invalid sub %d", sub_id, 0, 0);
    return;
  }

  RfgsmCoreSmDriveSub( sub_id, input );
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_drive_rx                                          */
/*! @brief Drive the state machine to manage states and transitions          */
/*! @details The API gets the device-specific state machine object and       */
/*!          tries to drive the state machine with an input.                 */
/*!          All checks and errors are handled in the state machine module.  */
/*! @param rx_device - Rx device ID to determine which state machine to drive*/
/*! @param input - the input to drive the state machine                      */
/*! @retval void                                                             */
void rfgsm_core_intf_sm_drive_rx( rfm_device_enum_type rx_device,
                                  RfgsmCoreSmRxInputType input )
{
  /* Validate the device ID */
  if( rx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_drive_rx() invalid Rx device %d", rx_device, 0, 0);
    return;
  }

  RfgsmCoreSmDriveRx( rx_device, input );
}



/*===========================================================================*/
/*! @fn rfgsm_core_intf_sm_drive_tx                                          */
/*! @brief Drive the state machine to manage states and transitions          */
/*! @details The API gets the device-specific state machine object and       */
/*!          tries to drive the state machine with an input.                 */
/*!          All checks and errors are handled in the state machine module.  */
/*! @param tx_device - Tx device ID to determine which state machine to drive*/
/*! @param input - the input to drive the state machine                      */
/*! @retval void                                                             */
void rfgsm_core_intf_sm_drive_tx( rfm_device_enum_type tx_device,
                                  RfgsmCoreSmTxInputType input )
{
  /* Validate the device ID */
  if( tx_device >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("rfgsm_core_intf_sm_drive_tx() invalid Tx device %d", tx_device, 0, 0);
    return;
  }

  RfgsmCoreSmDriveTx( tx_device, input );
}

