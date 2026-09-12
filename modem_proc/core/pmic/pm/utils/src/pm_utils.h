#ifndef __PM_UTILS_H__
#define __PM_UTILS_H__

/*! \file
 *  
 *  \brief  pm_utils.c ----This file contains the implementation of utils functions.
 *  \details This file contains the implementation of utils functions.
 *  
 *    &copy; Copyright 2018 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.

$Header: //components/rel/core.mpss/10.0/pmic/pm/utils/src/pm_utils.h#1 $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/12/13   umr      Created

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "com_dtypes.h"
#include "pm_err_flags.h"
/*===========================================================================

                        GENERIC FUNCTION PROTOTYPES

===========================================================================*/
extern uint32
pm_utils_get_upper_idx(uint32 data_value, uint32 *data_list, uint32 list_count);

pm_err_flag_type pm_busy_wait(uint32 req_wait_usec);

#endif // __PM_UTILS_H__
