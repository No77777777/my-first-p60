/*===========================================================================

              I P A   S T U B S . C

DESCRIPTION
  This file contains stubs to support MOB with IPA emulation builds

Copyright (c) 2017-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/18   nl      MOB and off-target IPA emulation memory issue
09/10/17   nl      Added MOB and off-target IPA emulation support
===========================================================================*/

#undef WIN32_LEAN_AND_MEAN

#include "time_svc.h"
#include <sio.h>
#include "VVDRV_deo_interrupt_controller.h"
#include "VVDRV_deo_time_os.c"
#include "VVDRV_deo_rpb.c"
#include "VVDRV_deo_os.c"
#include "VVDRV_deo_mutex_os.c"
#include "VVDRV_deo_interrupt_os.c"
#include "VVDRV_deo_interrupt_controller.c"

/*===========================================================================

                          FUNCTIONS

==========================================================================*/

/*!
The following functions do not compile in MOB.
*/

time_error_type time_register_cb_func
(
  time_cb_type cb_func,
  uint32 data
)
{
  return TIME_SUCCESS;
}

uint64 timetick_sclk_to_prec_us
(
  uint32 sclks
)
{
  return 1;
}


/*==========================================================================

FUNCTION sio_register_device_driver                             

@brief
Register SIO devices of driver with SIO
Implementation is the same as in DATAMODEM stubs file

@dependencies
None

@return
0 - Failed to register the SIO device with SIO service
1 - Success

@sideeffects
None

===========================================================================*/
uint32 sio_register_device_driver(sio_device_interafce * device_iface_ptr)
{
  return 1;
}


int32 VVDRV_deo_interrupt_controller_load_r3pc(DEO_INT_CTRL *funcTable)
{
	return 0;
}