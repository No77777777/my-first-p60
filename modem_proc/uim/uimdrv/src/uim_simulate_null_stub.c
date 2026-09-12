/*============================================================================
  FILE:         uim_simulate_null.c

  OVERVIEW:
   The file contains the simulate NULL related APIs.

  EXTERNALIZED FUNCTIONS:
    uim_switch_slot_mapping
    Api to switch logical to physical slot mapping.

    uim_get_logical_to_physical_mapping
    Api to query the current slot mapping

  DEPENDENCIES: N/A

              Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
              All Rights Reserved.
              QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_simulate_null_stub.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when          who        what, where, why
--------      ----       ---------------------------------------------------------
06/07/16      gm         Initial Revision
============================================================================*/
#include "uimglobals.h"
#include "uim_simulate_null.h"

/*============================================================================= 
  FUNCTION UIMGEN_ISO_HANDLE_SIMULATE_NULL_ST
=============================================================================*/
void uimgen_iso_handle_simulate_null_st
(
  uim_instance_global_type *uim_ptr
)
{
  (void) uim_ptr;
} /* uimgen_iso_handle_simulate_null_st */


/*============================================================================= 
  FUNCTION UIMGEN_ISO_HANDLE_SIMULATE_NULL_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_simulate_null_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  (void) rsp_ptr;
  (void) uim_ptr;
  (void) completed;
  return UIM_CMD_ERR;
} /* uimgen_iso_handle_simulate_null_st_rsp */


/*===========================================================================

FUNCTION UIM_SIMULATE_NULL_ADD_STATE

DESCRIPTION
  This function add extra state to uim command,

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_simulate_null_add_state
(
  uim_instance_global_type *uim_ptr
)
{
  (void) uim_ptr;
}


/*===========================================================================

FUNCTION uim_simulate_read_from_efs

DESCRIPTION
  This function read data from efs for simulate NULL

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_simulate_null_read_from_efs
(
  uim_instance_global_type *uim_ptr
)
{
  (void) uim_ptr;
}


/*===========================================================================

FUNCTION UIM_SIMULATE_NULL_TIMER_STATE_HANDLER

DESCRIPTION
  This function handles actions and change of state of uim_simulate_null_timer,
  when some event happens. This function is only for SIM BUSY Feature testing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_simulate_null_timer_state_handler
(
  uim_instance_global_type *uim_ptr,
  uim_busy_response_timer_event_type simulate_null_timer_event
)
{
  (void) uim_ptr;
  (void) simulate_null_timer_event;
}/* uim_simulate_null_timer_state_handler */

