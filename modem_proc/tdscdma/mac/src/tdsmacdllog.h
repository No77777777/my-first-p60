#ifndef TDSMACDLLOG_H
#define TDSMACDLLOG_H


/*===========================================================================
         D O W N L I N K   M A C    L O G G I N G  H E A D E R   F I L E

DESCRIPTION
  This file provides the function prototypes for MAC downlink logging.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdllog.h_v   1.1   24 Aug 2001 15:32:50   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdllog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsmaclog.h"
#include "tdsmacrrcif.h"

#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
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
void tdsmac_dl_send_cfg_log_packet
(
  tdsmac_dl_config_type *cfgptr        /* Ptr to Downlink MAC config          */
);

#endif /* FEATURE_TDSCDMA_MAC_QXDM_LOGGING */
#endif /* TDSMACDLLOG_H */
