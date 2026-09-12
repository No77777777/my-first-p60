#ifndef DS707_CCM_LOG_H
#define DS707_CCM_LOG_H

/*===========================================================================

                         D S 7 0 7 _ C C M _ L O G
GENERAL DESCRIPTION
  This file contains the congestion control manager functionality. 
  Currently it runs in FC task context.

EXTERNALIZED FUNCTIONS
 DS707_CCM_MEMORY_LOG
   Logs the current memory usage info

Copyright (c) 2011 - 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707_rmsm.h_v   1.6   23 Jan 2003 16:34:28   randrew  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/inc/ds707_ccm_log.h#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
07/22/11   vpk         First version of file.
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*===========================================================================

FUNCTION DS707_CCM_LOG_MEMORY

DESCRIPTION
  Logs current usage info of DSM items and Watermarks

  Takes outcommand as parameter and captures in the log.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds707_ccm_log_memory
( 
  uint8 out_cmd
);

#endif
