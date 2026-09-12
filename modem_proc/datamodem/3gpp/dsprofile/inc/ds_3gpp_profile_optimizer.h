
 /******************************************************************************
 @file    ds_3gpp_profile_optimizer.h
 @brief

  DESCRIPTION
  Tech specific implementation of 3GPP Profile Management

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009 - 2016 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
****************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when       who     what, where, why
--------   ---     ----------------------------------------------------------
9/9/16     vb      Optimize profiles (persistent and non persistent) 
=============================================================================*/

#ifndef DS_3GPP_PROFILE_OPTIMIZER_H
#define DS_3GPP_PROFILE_OPTIMIZER_H

#include "ds_profile_db.h"
#include "dsumtspdpreg.h"
#include "ds_profile_3gppi.h"
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ds_3gppi_utils.h"
#include <string.h>
#include "amssassert.h"
#include "msg.h"
#include "err.h"
#include "dsumtspdpregint.h"
#include "dsati.h"
#include "ps_sys.h"
#include "ds_sys.h"

/*=========================================================================
FUNCTION DS_3GPP_PROFILE_RUN_PERSISTENT_OPTIMIZER

DESCRIPTION
 
PARAMETERS:
  None
 
DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_profile_persistent_optimizer_cmd_hdlr
(
  
  const ds_cmd_type      *cmd_ptr
);


/*=========================================================================
FUNCTION DS_3GPP_PROFILE_RUN_PERSISTENT_OPTIMIZER

DESCRIPTION
  Post a cmd to run persistent profile optimizer
 
PARAMETERS:
  None
 
DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/

void ds_3gpp_profile_run_persistent_optimizer
(
  sys_modem_as_id_e_type   subs_id
);

#endif /*DS_3GPP_PROFILE_OPTIMIZER_H*/
