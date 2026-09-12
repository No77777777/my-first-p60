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

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/13/17   ku      Common path for local and remote card
11/24/16   ks      Adding support for spcom interface
10/13/15   ks      Fixing KW issue
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimdrv_hal.h"
#include "uimdrv_hal_iso.h"
#include "uimdrv_hal_remote.h"
#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
#error code not present
#endif

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif

/*============================================================================= 
  GLOBALS
=============================================================================*/
/* Function pointer tables */
uim_gen_func_t             uim_pup_func_ptr[UIM_TECH_MAX];
uim_gen_func_t             uim_pdown_func_ptr[UIM_TECH_MAX];
uim_reset_func_t           uim_reset_func_ptr[UIM_TECH_MAX];
uim_update_op_param_func_t uim_update_op_param_func_ptr[UIM_TECH_MAX];
uim_send_cmd_func_t        uim_send_cmd_func_ptr[UIM_TECH_MAX];
uim_send_apdu_func_t       uim_send_apdu_func_ptr[UIM_TECH_MAX];

/*===========================================================================
 FUNCTION UIMDRV_HAL_INIT_FUNCTION_POINTER_TABLES
===========================================================================*/
void uimdrv_hal_init_function_ptr_tables(void)
{
  /* Populate power up function pointer table */
  uim_pup_func_ptr[UIM_ISO]    = uimdrv_iso_power_up;
  uim_pup_func_ptr[UIM_REMOTE] = uimdrv_remote_power_up;

  /* Populate power down function pointer table */
  uim_pdown_func_ptr[UIM_ISO]    = uimdrv_iso_power_down;
  uim_pdown_func_ptr[UIM_REMOTE] = uimdrv_remote_power_down;

  /* Populate reset function pointer table */
  uim_reset_func_ptr[UIM_ISO]    = uimdrv_iso_reset;
  uim_reset_func_ptr[UIM_REMOTE] = uimdrv_remote_reset;


  /* Populate update operational parameters function pointer table */
  uim_update_op_param_func_ptr[UIM_ISO]    = uimdrv_iso_update_op_param;
  uim_update_op_param_func_ptr[UIM_REMOTE] = uimdrv_remote_update_op_param;


  /* Populate send command function pointer table */
  uim_send_cmd_func_ptr[UIM_ISO]    = uimdrv_iso_send_command;
  uim_send_cmd_func_ptr[UIM_REMOTE] = uimdrv_remote_send_command;

  /* Populate send APDU function pointer table */
  uim_send_apdu_func_ptr[UIM_ISO]    = uimdrv_iso_send_apdu;
  uim_send_apdu_func_ptr[UIM_REMOTE] = uimdrv_remote_send_apdu;

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
  #error code not present
#endif

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif
} /* uim_init_function_ptr_tables */


/*===========================================================================
FUNCTION UIMDRV_HAL_POWER_UP
===========================================================================*/
void uimdrv_hal_power_up
(
  uim_instance_global_type *uim_ptr
) 
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    uim_pup_func_ptr[uim_ptr->setting.phy_layer](uim_ptr);
  }
}/* uimdrv_hal_power_up */


/*===========================================================================
FUNCTION UIMDRV_HAL_POWER_DOWN
===========================================================================*/
void uimdrv_hal_power_down
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    uim_pdown_func_ptr[uim_ptr->setting.phy_layer](uim_ptr);
  }
} /* uimdrv_hal_power_down */


/*===========================================================================
FUNCTION UIMDRV_HAL_RESET
===========================================================================*/
void uimdrv_hal_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    uim_reset_func_ptr[uim_ptr->setting.phy_layer](rsp_ptr, uim_ptr);
  }
} /* uimdrv_hal_update_op_params */


/*===========================================================================
FUNCTION UIMDRV_HAL_UPDATE_OP_PARAMS
===========================================================================*/
boolean uimdrv_hal_update_op_params
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    return uim_update_op_param_func_ptr[uim_ptr->setting.phy_layer](op_params, 
                                                                  uim_ptr);
  }
  return FALSE;
} /* uimdrv_hal_update_op_params */


/*===========================================================================
FUNCTION UIMDRV_HAL_SEND_COMMAND
===========================================================================*/
void uimdrv_hal_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    uim_send_cmd_func_ptr[uim_ptr->setting.phy_layer](uim_req, uim_ptr);
  }
} /* uimdrv_hal_send_command */


/*===========================================================================
FUNCTION UIMDRV_HAL_SEND_INTERMEDIATE_APDU
===========================================================================*/
void uimdrv_hal_send_intermediate_apdu
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer < UIM_TECH_MAX)
  {
    uim_send_apdu_func_ptr[uim_ptr->setting.phy_layer](uim_req, uim_ptr);
  }
}; /* uimdrv_hal_send_intermediate_apdu*/
