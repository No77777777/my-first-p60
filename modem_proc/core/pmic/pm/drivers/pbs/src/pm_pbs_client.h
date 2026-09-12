#ifndef __PM_PBS_CLIENT_H__
#define __PM_PBS_CLIENT_H__

/** @file pm_pbs_client.h
 *
 *  \brief This header file contains API and type definitions for PBS Client driver.
*/
/*
 *  Copyright (c) 2018 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pbs/src/pm_pbs_client.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/13   kt      Created.
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "pm_err_flags.h"
#include "com_dtypes.h"
#include "pm_resources_and_types.h"
#include "pm_config_target.h"

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/**
 * @name pm_pbs_client_sw_trigger
 * 
 * @brief Force triggers the corresponding PBS client software 
 *        or hardware trigger sequence by writing to the
 *        TRIG_CTL register.
 * 
 * @param[in] pmic_chip. Primary PMIC: 0 Secondary PMIC: 1
 * @param[in] client: PBS Client type. Refer enum 
 *                    pm_pbs_client_type above.
 * @param[in] trigger: PBS Client hardware trigger number or 
 *                     software trigger. Refer enum
 *                     pm_pbs_client_trigger type above.
 * @param[in] trigger_argument: Argument to be passed to PBS 
 *                             trigger sequence. Enter NULL, if
 *                             no argument
 * param[in] trigger_type: Currently two trigger types are supported 
 *                         PM_PBS_CLIENT_FNF-Fire and forget
 *                         (All HW and some SW seq. and some SW seq are of 
 *                          FNF type)
 *                         PM_PBS_CLIENT_WAIT-Wait for sequence completion      
 * @param[out] trigger_return_value: value returned by PBS 
 *                                  trigger sequence. Enter
 *                                  NULL, if no return value is
 *                                  expected
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type 
pm_pbs_client_sw_trigger(uint8 pmic_chip, 
                         pm_pbs_client_type client, 
                         pm_pbs_client_trigger trigger, 
                         uint8 *trigger_argument, 
                         uint8 *trigger_return_value, 
                         pm_pbs_client_trigger_type trigger_type);


#endif /* __PM_PBS_CLIENT_H__ */
