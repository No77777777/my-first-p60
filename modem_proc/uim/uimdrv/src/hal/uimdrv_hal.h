#ifndef UIMDRV_HAL_H
#define UIMDRV_HAL_H
/*===========================================================================

                            U I M D R V  H A L

DESCRIPTION
  This file contains driver functions for UIMDRV HAL interface. UIMDRV HAL
  abstracts the physical interface to card. It maintains a function pointer
  table for each physical interface to execute appropriate operations.

Copyright (c) 2017 by QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/13/17   ku      Common path for local and remote card
10/30/15   hyo     Cleaning up comment 
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimdrv.h"
#include "uimglobals.h"

/*============================================================================= 
  TYPE DEFINITION
=============================================================================*/
/* uimdrv function pointer types */
typedef void (*uim_gen_func_t) (uim_instance_global_type*);
typedef void (*uim_reset_func_t) (uim_rsp_buf_type*,
                                  uim_instance_global_type*);
typedef boolean (*uim_update_op_param_func_t) (uim_op_params_type const*, 
                                               uim_instance_global_type*);
typedef void (*uim_send_cmd_func_t) (uim_req_buf_type*,
                                     uim_instance_global_type*);

typedef void (*uim_send_apdu_func_t) (uim_req_buf_type*,
                                      uim_instance_global_type*);


/*============================================================================= 
  EXTERNALIZED FUNCTION PROTOTYPES
=============================================================================*/
/**
 *  Initializes the function pointer tables for UIMDRV operation
 */
void uimdrv_hal_init_function_ptr_tables(void);

/**
 *  This function looks up the uimdrv hal power up function
 *  pointer table and calls power up function according to the
 *  physical layer
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_hal_power_up
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function looks up the uimdrv hal power down function
 *  pointer table and calls power down function according to the
 *  physical layer
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_hal_power_down
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function looks up the uimdrv hal reset function
 *  pointer table and calls reset function according to the
 *  physical layer 
 *  
 *  @param rsp_ptr[OUT] Pointer to command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_hal_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
);

/**
 *  This function looks up the uimdrv hal update op param
 *  function pointer table and calls update op param function
 *  according to the physical layer
 *  
 *  @param op_param[IN] operational parameters of the UIM
 *                      interface
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 *  @return boolean Whether CLK stop is performed successfully.
 */
boolean uimdrv_hal_update_op_params
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
);


/**
 *  This function looks up the uimdrv hal send command
 *  function pointer table and calls send command function
 *  according to the physical layer
 *  
 *  Side effect: If the command was successful, reponse buffer
 *  in UIM global is updated
 *  
 *  @param uim_req[IN] Pointer to command request buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the
 *                     UIM global structure
 */
void uimdrv_hal_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
);

void uimdrv_hal_send_intermediate_apdu
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
);

#endif /* UIMDRV_HAL_H */

