/*!
  @file
  rfgsm_core_intf.h

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

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_core_intf.h#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------ 
08/20/15   sc      Add framework for split Rx/Tx state machines
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
03/11/14   sw/sc   Initial version

=============================================================================*/

#ifndef RFGSM_CORE_INTF_H
#define RFGSM_CORE_INTF_H

/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include "comdef.h"
#include "rfgsm_core_state_machine.h"


#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/
/*                                                                           */
/*                              DEFINTIONS                                   */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                               FEATURES                                    */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                      EXTERNAL DEFINITIONS AND TYPES                       */
/*                                                                           */
/*===========================================================================*/



/*===========================================================================*/
/*                                                                           */
/*                                FUNCTIONS                                  */
/*                                                                           */
/*===========================================================================*/

boolean rfgsm_core_intf_sm_init_sub( void );

/*---------------------------------------------------------------------------*/

boolean rfgsm_core_intf_sm_destroy_sub( void );

/*---------------------------------------------------------------------------*/

boolean rfgsm_core_intf_sm_init_rx( rfm_device_enum_type rx_device );

/*---------------------------------------------------------------------------*/

boolean rfgsm_core_intf_sm_destroy_rx( rfm_device_enum_type rx_device );

/*---------------------------------------------------------------------------*/

boolean rfgsm_core_intf_sm_init_tx( rfm_device_enum_type tx_device );

/*---------------------------------------------------------------------------*/

boolean rfgsm_core_intf_sm_destroy_tx( rfm_device_enum_type rx_device );

/*---------------------------------------------------------------------------*/

void rfgsm_core_intf_sm_drive_sub( uint8 sub_id,
                                   RfgsmCoreSmSubInputType input );

/*---------------------------------------------------------------------------*/

void rfgsm_core_intf_sm_drive_rx( rfm_device_enum_type rx_device,
                                  RfgsmCoreSmRxInputType input );

/*---------------------------------------------------------------------------*/

void rfgsm_core_intf_sm_drive_tx( rfm_device_enum_type tx_device,
                                  RfgsmCoreSmTxInputType input );



#ifdef __cplusplus
}
#endif

#endif /* RFGSM_CORE_INTF_H */

