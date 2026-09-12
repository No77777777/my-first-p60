#ifndef TDSCDMA_MAC_QSH_H
#define TDSCDMA_MAC_QSH_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            TDSMAC_QSH_EXT.H                 

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
2/13/15    sarao    Changes made after MAV feedback
2/06/15    sarao     Initial version for tdsmac_qsh_ext.h


===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <string.h>
#include <qsh.h>




#define TDSMAC_QSH_MAJOR_VER 0
#define TDSMAC_QSH_MINOR_VER 1

typedef enum
{
  TDSMAC_QSH_METRIC_R99_STAT,
  TDSMAC_QSH_METRIC_EUL_STAT 
}tdsmac_qsh_metric_mac_e;

typedef struct
{
  qsh_metric_hdr_s hdr; 					/* This header has the timestamp (uint16) */
  
  uint32 mac_r99ul_bo_bytes;					/* UMTS BO in bytes (R99 and R6) History Buffer in last sec */
}tdsmac_qsh_metric_r99_stat_s;

typedef struct
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  
  uint8  num_si_sent_pri;
  uint8  num_si_sent_sec;				/*TBD */
  uint32 mac_eul_bo_bytes;
}tdsmac_qsh_metric_eul_stat_s;


#endif
