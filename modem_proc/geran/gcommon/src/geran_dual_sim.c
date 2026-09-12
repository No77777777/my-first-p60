/*! \file geran_dual_sim.c

  Provides functions for use by GERAN entities supporting Dual SIM.

                Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/gcommon/src/geran_dual_sim.c#2 $ */
/* $DateTime: 2020/01/27 04:29:28 $$Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "geran_dual_sim_g.h"
#include "rex.h"
#include <stringl/stringl.h>
#include "geran_msgs.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif /* FEATURE_QSH_MDUMP */
/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 *--------------------------------------------------------------------------*/
 static char check_gas_id_task_name[REX_TASK_NAME_LEN+1];

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Externalised Function Definitions
 *---------------------------------------------------------------------------*/

/*!
 * \brief Range check gas_id to ensure no buffer overflow when it is used.
 *
 * \param gas_id_t
 *
 * \return gas_id_t
 */
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
gas_id_t check_gas_id(const gas_id_t gas_id)
{
  if (gas_id > GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rex_get_task_name(rex_self(), check_gas_id_task_name, ARR_SIZE(check_gas_id_task_name), NULL);
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRR, QSH_CLT_GRR)
#endif /* FEATURE_QSH_MDUMP */
    ERR_GERAN_FATAL_2("gas_id invalid %d. Calling task: %s", gas_id, check_gas_id_task_name);
    return GERAN_ACCESS_STRATUM_ID_1;
  }
  return gas_id;
}

/* EOF */

