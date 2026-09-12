#ifndef PS_MIP6_RSM_H
#define PS_MIP6_RSM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          P S _ M I P 6 _ R S M . H


GENERAL DESCRIPTION
  The implementation of the Mobile IPv6 Registration State Machine (RSM).

EXTERNALIZED FUNCTIONS

  ps_mip6_rsm_powerup_init()
    Performs the power up initialization for RSM.

  ps_mip6_rsm_alloc()
    Allocates an instance of the Registration State Machine and returns
    a handle to it.

  ps_mip6_rsm_config()
    Configures the RSM instance with the provided parameters.

  ps_mip6_rsm_free()
    Frees the allocated memory for RSM

  ps_mip6_rsm_post_event()
    Posts an event to the RSM state machine.

  ps_mip6_rsm_ind_cback_reg()
    Registers to receive indications from state machine

  ps_mip6_rsm_ind_cback_dereg()
    De-Registers the indication callback registered via reg function

  ps_mip6_input()
    Processes inbound packets for MIPv6 RSM

  ps_mip6_notify()
     Handles inbound ICMPv6 error packets for MIPv6

INITIALIZATION AND SEQUENCING REQUIREMENTS
  TBD
  
Copyright (c) 2006-2009 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/mip/inc/ps_mip6_rsm.h#2 $
  $Author: pwbldsvc $
  $DateTime: 2019/10/22 02:53:17 $
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#endif /* PS_MIP6_RSM_H */
