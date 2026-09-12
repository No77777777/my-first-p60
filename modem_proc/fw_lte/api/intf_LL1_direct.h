/*!
  @file
  intf_LL1_direct.h

  @brief
  This file contains interfaces for functions and variables that
  are directly called or call ML1 or MCPM related functionality
  
  @detail
  description of functions in this file
 
*/

/*===========================================================================

  Copyright (c) 2008-09 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/intf_LL1_direct.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef INTF_LL1_DIRECT_H
#define INTF_LL1_DIRECT_H

#include "intf_common.h"
#include "intf_async_msg.h"
#include "intf_sys.h"
#include "msgr.h"
#include "hw_mem_pool_buf.h"
#include "hw_decob_buf.h"
#include "lte_LL1_ue_types.h"
#include "intf_config_app.h"

/*===========================================================================

      Constants

===========================================================================*/


/*===========================================================================

      Macros

===========================================================================*/


/*===========================================================================

      Typedefs

===========================================================================*/


/*===========================================================================

      Variables

===========================================================================*/


/*===========================================================================

      Function Prototypes

===========================================================================*/

// Direct interface call shared with MCPM.. Returns the current
// TDEC clock in KHZ format
unsigned int  lte_mcpm_curr_tdec_clock_inKHz(void );


#endif /* INTF_ASYNC_H */
