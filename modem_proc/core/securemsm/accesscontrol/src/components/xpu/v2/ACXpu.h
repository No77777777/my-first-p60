#ifndef AC_XPU_H
#define AC_XPU_H

/*===========================================================================
Copyright (c) 2010-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/accesscontrol/src/components/xpu/v2/ACXpu.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/01/12   sg      Add APIs to add/remove VMIDs permissions from an xPU
09/14/10   tk      First version.
============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "AccessControlMba.h"


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/
 /**
 * Applies static XPU configuration. The static configuration is always assumed
 * to be present and not dynamically changed (e.g. by PIL).
 */
int tzbsp_xpu_static_config(const tzbsp_xpu_cfg_t * xpu_cfg,
                            const uint32 xpu_cfg_count);


#endif /* AC_XPU_H */
