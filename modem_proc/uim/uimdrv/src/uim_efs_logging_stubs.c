/*==============================================================================
  FILE:         uim_logging.c

  OVERVIEW:     File contains the functions for APDU and EFS logging.

  DEPENDENCIES: N/A

                Copyright (c) 2014-2016 QUALCOMM Technologies, Inc.(QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/

/*=============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_efs_logging_stubs.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       --------------------------------------------------------- 
04/27/16    ks         Fix KW issue
09/07/15    na         Draft created for EFS logging function
==============================================================================*/
#include "uim_p.h"
#include "uimi.h"
#include "uimglobals.h"
#include "uim_logging.h"

/**
 * DEFINITIONS OF EXTERNAL FUNTIONS
 *
 */

/**
 * This function will simply retunrs the FALSE
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean uim_efslog_init( uim_instance_global_type *uim_ptr )
{
  (void)uim_ptr;
  /* Just returning FALSE without doing anything */
  return FALSE;
} /* uim_efslog_init */


/**
 * This Function would now become dummy and simply returns the control
 *
 * @param attribute Attribute of logging data
 * @param character Data to be logged.
 * @param uim_ptr Pointer to the global data common.
 */
void uim_efslog_apdu_to_buffer(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  /* do nothing */
  return;

} /* uim_efslog_apdu_to_buffer */


/**
 * This Function is being stub out and just return the conrtol.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_efslog_save_buffer_to_efs( uim_instance_global_type *uim_ptr )
{
  (void)uim_ptr;
  /* Do nothing and just return the control*/
  return;
} /* uim_efslog_save_buffer_to_efs */


/**
 * Cleans up EFSLOG when complete
 *
 * @param uim_ptr
 */
void uim_efslog_clean_up( uim_instance_global_type *uim_ptr )
{
  (void)uim_ptr;
  /* Do nothing and just return the control */
  return;
} /* uim_efslog_clean_up */


/**
 * DEFINITIONS OF INTERNAL FUNTIONS
 *
 */

/**
 * This file handle the processing of purge signal.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return none.
 */
void uim_efslog_process_purge_sig(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return;
} /* uim_efslog_process_purge_sig */