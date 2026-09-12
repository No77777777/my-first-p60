/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                          PHONEBOOK MANAGER EFS STUB FUNCTIONS

GENERAL DESCRIPTION
  This file contains the functions used to manage phonebook entries within the
  EFS.

  Copyright (c) 2015 - 2016 by QUALCOMM Technologies, Inc(QTI).
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/pbm/src/pbmefs_stub.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/12/16   kv      Fix compilation warnings - void unused variables
08/24/15   nr      Initial Revision
===========================================================================*/
#include "pbm.h"
#include "pbmefs.h"

/*===========================================================================
    Declare static globals for EFS
===========================================================================*/
boolean pbm_efs_disable = TRUE;
/* Will be TRUE if EFS support is to be disabled by default here */

/*===========================================================================
FUNCTION PBM_EFS_INIT

DESCRIPTION
  This stub is for pbm_efs_init ()

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_efs_init(
  void
)
{
  return PBM_SUCCESS;
} /* pbm_efs_init */


/*===========================================================================
FUNCTION PBM_EFS_GENERATE_UNIQUE_ID

DESCRIPTION
This stub is for pbm_efs_generate_unique_id ().

DEPENDENCIES
  None

SIDE EFFECTS
===========================================================================*/
uint16 pbm_efs_generate_unique_id(
  void
)
{
  return 0;
} /* pbm_efs_generate_unique_id */


/*===========================================================================
FUNCTION PBM_EFS_GARBAGE_RECOLLECT

DESCRIPTION
  This stub is for pbm_efs_garbage_recollect ()

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void pbm_efs_garbage_recollect(
  void
)
{
  return;
} /* pbm_efs_garbage_recollect */


/*===========================================================================
FUNCTION PBM_RECORD_WRITE_TO_EFS

DESCRIPTION
  This stub is for pbm_record_write_to_efs ()

DEPENDENCIES
  The caller should free write cmd structure in case of an error.
  If successful, write cmd is freed by this function (synchronous operation) or
  within the write callbacks (asynchronous operation).

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_record_write_to_efs(
  pbm_write_record_s_type *write
)
{
  (void)write;
  return PBM_ERROR_NOT_SUPPORTED;
} /* pbm_record_write_to_efs */


/*===========================================================================
FUNCTION PBM_GET_NEXT_PHONE_UNIQUE_ID

DESCRIPTION
  This stub is for pbm_get_next_phone_unique_id ().

DEPENDENCIES
  Must be called from within pbm_crit_sect critical section.

SIDE EFFECTS
===========================================================================*/
pbm_return_type pbm_get_next_phone_unique_id(
  uint64 *ph_unique_id
)
{
  (void)ph_unique_id;
  return PBM_ERROR_NOT_SUPPORTED;
} /* pbm_get_next_phone_unique_id */


/*===========================================================================
FUNCTION PBM_GET_PHONE_UNIQUE_ID_FROM_RECORD

DESCRIPTION
  This stub is for pbm_get_phone_unique_id_from_record ()

DEPENDENCIES

SIDE EFFECTS

COMMENTS

SEE ALSO
===========================================================================*/
pbm_return_type pbm_get_phone_unique_id_from_record(
  const pbm_addr_cache_s_type    *record,
  uint64                         *ph_unique_id
)
{
  (void)record;
  (void)ph_unique_id;
  return PBM_ERROR_NOT_SUPPORTED;
} /* pbm_get_phone_unique_id_from_record */