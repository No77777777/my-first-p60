#ifndef RLC_QSH_INT_H
#define RLC_QSH_INT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WRLC QSH INT . H                 

GENERAL DESCRIPTION
   Contains declarations for function which are QUALCOMM proprietary 
   and may or may not be shipped as source code.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/28/15   aa     Initial version for wrlc_qsh_int.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wrlc_qsh_ext.h"
#include "rlcul.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#ifdef FEATURE_QSH_MDUMP

QSH_MDUMP_FN_ATTR
void wrlc_qsh_mdump_collect(sys_modem_as_id_e_type as_id, qsh_client_cb_params_s* qsh_cb_params_ptr);

#endif

#endif /*RLC_QSH_INT_H*/

