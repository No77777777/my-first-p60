#ifndef MACDLLOG_H
#define MACDLLOG_H


/*===========================================================================
         D O W N L I N K   M A C    L O G G I N G  H E A D E R   F I L E

DESCRIPTION
  This file provides the function prototypes for MAC downlink logging.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2001 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2008-2009 Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdllog.h_v   1.1   24 Aug 2001 15:32:50   rchar  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macdllog.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
04/25/15  scv     W+W Changes to have messages support SubID
01/29/15  kc      L2 global reorg for W+W
01/02/12  grk     Feature cleanup
05/07/09  ssg     Updated Copyright Information
08/24/01  rc      Moved FEATURE definition to after the include definitions.
08/21/01  rc      Created file.

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "maclog.h"
#include "macrrcif_v.h"

/*===========================================================================

FUNCTION MAC_DL_SEND_CFG_LOG_PACKET

DESCRIPTION
   This function prepares a log packet for configured Downlink MAC Transport 
   Channels and sends the packet to log services.  

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void mac_dl_send_cfg_log_packet
(
  mac_dl_config_type *cfgptr,        /* Ptr to Downlink MAC config          */
  mac_dl_table_type *dl_table_ptr
);
#endif /* MACDLLOG_H */
