/******************************************************************************
  @file    ds_profile_os_amss.c
  @brief   

  DESCRIPTION
  This file implements the modem (AMSS) specific routines

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsprofile/src/ds_profile_os_amss.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/30/09   mg      Created the module. First version of the file.
===========================================================================*/

#include "datamodem_variation.h"
#include "memory.h"
#include "err.h"
#include "ds_profile_os_amss.h"
#include "ds_profile_tech_common.h"

/*---------------------------------------------------------------------------
 					 PUBLIC FUNCTION IMPLEMENTATIONS
   Logging and locking functions for DS profile library
---------------------------------------------------------------------------*/

int ds_profile_log_init( 
  char *lib_instance
)
{
  /* Setup logging */
  return DSI_SUCCESS;
}

int ds_profile_lock_init( 
  plm_lock_type *lock 
)
{
  rex_init_crit_sect( lock );
  return DSI_SUCCESS;
}

int ds_profile_lock_acq( plm_lock_type *lock )
{
  rex_enter_crit_sect( lock );
  return DSI_SUCCESS;
}

int ds_profile_lock_rel( plm_lock_type *lock )
{
  rex_leave_crit_sect( lock );
  return DSI_SUCCESS;
}

int plm_lock_try( plm_lock_type *lock )
{
  rex_enter_crit_sect( lock );
  return DSI_SUCCESS;
}



