#ifndef __PMAPP_COMMON_H__
#define __PMAPP_COMMON_H__
/*===========================================================================


              PMAPP COMMON HEADER FILE

\details
    This file contains functions prototypes and variable/type/constant 
  declarations to support conversions between voltage values and PMIC
  register values
  
Copyright (c) 2013            by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/pmic/pm/pmapp_common.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/14/17   rl      File created
===========================================================================*/

#include "pm_err_flags.h"
#include "pm_resources_and_types.h"
#include "com_dtypes.h"

/**
Fg ESR Quiet Mode Enable/Disable
 @param in entry_exit  Specifies wheather entry sequence needs 
                        to be triggered or the exit sequence
                        TRUE for Entry Sequence
                        FALSE for Exit Sequence

 @note Usage Example:
 err_flag = pmapp_cmn_esr_trig(PMAPP_CMN_ESR_ENTRY_SEQ);
 
 @return 
 PM_ERR_FLAG__SUCCESS if successful. See #pm_err_flag_type 
*/
pm_err_flag_type 
pmapp_fg_esr_qmode(boolean entry_exit);                                           

#endif /* __PMAPP_COMMON_H__ */
