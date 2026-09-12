#ifndef UIMDRV_HAL_REMOTE_H
#define UIMDRV_HAL_REMOTE_H
/*===========================================================================

                            U I M D R V  H A L  R E M O T E 

DESCRIPTION
  This file contains driver functions that are specific to UIM REMOTE

Copyright (c) 2017 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal_remote.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/13/17   ku      Common path for local and remote card
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimdrv.h"
#include "uimglobals.h"

/*============================================================================= 
  EXTERNALIZED FUNCTION PROTOTYPES
=============================================================================*/
/**
 *  UIMDRV power up function specific to remote UIM
 *  This function is a stub function in uimdrv.c
 *  For remote card power up operation, look at uim_remote.c
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimdrv_remote_power_up
(
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV power down function specific to remote UIM This
 *  function is a stub function in uimdrv.c
 *  For remote card power down operation, look at uim_remote.c
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimdrv_remote_power_down
(
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV reset function specific to remote UIM
 *  This function handles BTSAP reset command and takes
 *  appropriate action among card connect, card reset, and card
 *  power up.
 *  
 *  @param rsp_ptr[OUT] Pointer to command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimdrv_remote_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV update op param function specific to remote UIM This
 *  function is a stub function in uimdrv.c
 *  This function does nothing since remote UIM does not need
 *  this operation
 *  
 *  @param op_param[IN] operational parameters of the UIM
 *                 interface
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @return boolean Whether CLK stop is performed successfully.
 *          Since remote UIM doesn't have CLK, it always returns
 *          FALSE.
 */
boolean uimdrv_remote_update_op_param
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV send command function specific to remote UIM
 *  This function sends a command to the card via uim_remote
 *  interface
 *  
 *  @param uim_req[IN] Pointer to command request buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimdrv_remote_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
);


/**
 *  UIMDRV send APDU function specific to remote UIM
 *  This function sends a command to the card via uim_remote
 *  interface
 *  
 *  @param uim_req[IN] Pointer to command request buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimdrv_remote_send_apdu
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
);

#endif /* UIMDRV_HAL_REMOTE_H */
