#ifndef UIMGEN_HAL_H
#define UIMGEN_HAL_H
/*===========================================================================

                            U I M G E N  H A L

DESCRIPTION
  This file contains functions for UIMGEN HAL interface. UIMGEN HAL maintains
  function pointer tables for generic state machines. It keeps generic
  command process and response process neat. Also, it constructs a framework
  that can be extended if different state machine bahevior is needed.

Copyright (c) 2016 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimgen_hal.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/02/15  hyo     Initial Revision
===========================================================================*/
#include "uimgen.h"
#include "uimglobals.h"

/*============================================================================= 
  TYPE DEFINITION
=============================================================================*/
typedef uim_cmd_status_type (*uim_gen_rsp_func_t) (uim_rsp_buf_type         *rsp_ptr,
                                                   uim_instance_global_type *uim_ptr,
                                                   uim_cmd_status_type *completed);
typedef void (*uim_gen_cmd_func_t) (uim_instance_global_type*);

/*============================================================================= 
  EXTERNALIZED FUNCTION PROTOTYPES
=============================================================================*/
/**
 *  This function initializes the function pointer tables for UIM generic
 *  command state machine
 * 
 */
void uimgen_hal_init_cmd_func_ptr_table(void);


/**
 *  This function initializes the function pointer tables for UIM generic
 *  response state machine
 * 
 */
void uimgen_hal_init_rsp_func_ptr_table(void);


/**
 *  This function handles generic command state
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_hal_handle_generic_cmd_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function handles generic command state response
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_hal_handle_generic_cmd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);
#endif /* UIMGEN_HAL_H */

