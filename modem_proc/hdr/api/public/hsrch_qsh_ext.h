#ifndef HSRCH_QSH_LOG_H
#define HSRCH_QSH_LOG_H


/*===========================================================================

      H D R   Q S H  L O G 

DESCRIPTION
  This file contains the implementation of the 1x-EVDO QSH logging functionality

  Copyright (c) 2016 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

 when       who       what, where, why
 --------   --------  ----------------------------------------------------------
 07/25/16   vko     Add generic QSH event
 05/27/16   vko     QSH framework for events capturing

 ===========================================================================*/
                     
/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "qsh.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define HDR_QSH_MAJOR_VER 1
#define HDR_QSH_MINOR_VER 1

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

typedef enum
{
  HSRCH_QSH_EVENT_ANY_FAILURE = 0x00,
  HSRCH_QSH_EVENT_RLF,
  HSRCH_QSH_EVENT_OOS,
  HSRCH_QSH_EVENT_RESEL,
  HSRCH_QSH_EVENT_ACCESS_FAILURE,
  HSRCH_QSH_EVENT_OFS_FAILURE,
  HSRCH_QSH_EVENT_L2D_REDIR_FAILURE,
  HSRCH_QSH_EVENT_L2D_RESEL_FAILURE,
  HSRCH_QSH_EVENT_D2L_RESEL_FAILURE,
  HSRCH_QSH_EVENT_D2L_REDIR_FAILURE,
  HSRCH_QSH_EVENT_PAGE_MISS,
  HSRCH_QSH_EVENT_EMPTY_QTA_GAP,
  HSRCH_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT,
  HSRCH_QSH_EVENT_SYNC_TIMING_ERROR,
  HSRCH_QSH_EVENT_RF_TUNE_FREQ_ERROR,
  HSRCH_QSH_EVENT_HIGH_RXLEV_DIFF_PRI_DIV,
  HSRCH_QSH_EVENT_CALL_ORIG_FAILURE,
  HSRCH_QSH_EVENT_INVALID  = 0xff
}hsrch_qsh_event_e;

#endif /* HSRCH_QSH_LOG_H */
