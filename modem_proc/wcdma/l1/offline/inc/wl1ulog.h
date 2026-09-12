#ifndef WL1ULOG_H
#define WL1ULOG_H
/*===========================================================================

                    L1 ULOG Header

DESCRIPTION
  This file contains global declarations, MACROS and external references
  for the ULOG functionality to be used by WL1

EXTERNALIZED FUNCTIONS

  wrm_get_ulog_handle
    This function returns the ulog handle required for ULOG tracing.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2015 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1ulog.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/15   sad     Added featurization support for Ulog
03/30/15   pg      Created this file. Declared variadic Macros for ULOG 
                   functionality
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
/*ULOG Header file*/
#include "ULogFront.h"

#include "wcdma_common_api.h"


/*Macro to count variadic arguments passed*/
#define GET_COUNT_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,COUNT,...) COUNT

#ifdef FEATURE_WCDMA_L1_ULOG_SUPPORT
/*Internal Macro used for calling ULOG APIs*/
#define WL1_WRITE_ULOG(wsub_id, ...) ULogFront_RealTimePrintf(wl1_wrm_get_ulog_handle(wsub_id), GET_COUNT_MACRO(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0), __VA_ARGS__)
#else
#define WL1_WRITE_ULOG(wsub_id, ...)
#endif /* FEATURE_WCDMA_L1_ULOG_SUPPORT */

#define WL1_WRM_ULOG_ERROR(wsub_id, msg, args...)   WL1_WRITE_ULOG(wsub_id, "WRM ERROR: "msg, ##args)
#define WL1_WRM_ULOG_WARNING(wsub_id, msg, args...) WL1_WRITE_ULOG(wsub_id, "WRM WARNING: "msg, ##args)
#define WL1_WRM_ULOG_INFO(wsub_id, msg, args...)    WL1_WRITE_ULOG(wsub_id, msg, ##args)

/*===========================================================================
FUNCTION     WL1_WRM_ULOG_INIT

DESCRIPTION
  This function is invoked once only to initialize the ulog handle for the tracing
  purpose.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_wrm_ulog_init(void);

/*===========================================================================
FUNCTION     WL1_WRM_GET_ULOG_HANDLE

DESCRIPTION
  This function is invoked for getting the wrm ulog handle. This is required for
  ULOG logging to the corresponding handle only.

DEPENDENCIES
  None.

PARAMETERS
  uint8 wsub_idx

RETURN VALUE
  wl1_wrm_ulog_handle - ULogHandle in use by WL1 WRM code.

SIDE EFFECTS
  None.
===========================================================================*/
extern ULogHandle wl1_wrm_get_ulog_handle(uint8);


#endif /*WL1ULOG_H*/

