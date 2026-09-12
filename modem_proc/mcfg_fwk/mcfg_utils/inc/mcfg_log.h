#ifndef MCFG_LOG_H
#define MCFG_LOG_H
/*==============================================================================

                M O D E M   C O N F I G   U T I L S
 
GENERAL DESCRIPTION
  Header MCFG Debug EFS logging support

Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_utils/inc/mcfg_log.h#1 $ 
$DateTime: 2021/04/08 00:04:19 $ 
$Author: pwbldsvc $ 
$Change: 30003610 $ 

when     who   what, where, why
-------- ---   ----------------------------------------------------------------
05/29/12 sbt   Initial creation

==============================================================================*/

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

/*------------------------------------------------------------------------------
  Log file
------------------------------------------------------------------------------*/
/* Log filenames. Log rotation is mcfg_debug.log -> mcfg_debug_old.log -> deleted */
#define MCFG_LOG_PATH       "/mcfg_debug.log"
#define MCFG_OLD_LOG_PATH   "/mcfg_debug_old.log"

/* How big the log can get before it gets rotated. */
#define MCFG_LOG_MAX_SIZE       (65536)
#define MCFG_MAX_LOG_LINE_LEN   (128)

/*==============================================================================
  Typedefs
==============================================================================*/

/*==============================================================================
  Variables
==============================================================================*/


/*============================================================================== 
 
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
 
==============================================================================*/
void mcfg_save_log_entry( const char *line );

#endif /* MCFG_LOG_H */

