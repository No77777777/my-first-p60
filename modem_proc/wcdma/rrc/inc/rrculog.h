#ifndef RRCULOG_H
#define RRCULOG_H
#ifndef TEST_FRAMEWORK
/*===========================================================================

                    RRC ULOG Header

DESCRIPTION
  This file contains global declarations, MACROS and external references
  for the ULOG functionality to be used by WRRC

EXTERNALIZED FUNCTIONS

  wrrc_get_ulog_handle
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

$Header: //components/rel/wcdma.mpss/8.4.0/rrc/inc/rrculog.h#2 $
$DateTime: 2020/01/24 03:56:36 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/30/15   sp      Created this file. Declared variadic Macros for ULOG 
                   functionality
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
/*ULOG Header file*/
#include "ULogFront.h"
#include "sys.h"


/*Macro to count variadic arguments passed*/
#define GET_COUNT_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,COUNT,...) COUNT

/*Internal Macro used for calling ULOG APIs*/
#define WRITE_ULOG(...) ULogFront_RealTimePrintf(wrrc_get_ulog_handle(), GET_COUNT_MACRO(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0), __VA_ARGS__)

#define WRRC_ULOG_ERROR(msg, args...)   WRITE_ULOG("WRRC ERROR: "msg, ##args)

#ifdef FEATURE_DUAL_WCDMA
#define WRRC_ULOG_WARNING(msg, args...) WRITE_ULOG("WRRC WARNING: Sub-ID:%d "msg, (WAS_ID_M+1), ##args)
#define WRRC_ULOG_INFO(msg, args...)    WRITE_ULOG("Sub-ID:%d "msg, (WAS_ID_M+1), ##args)

#else
#define WRRC_ULOG_WARNING(msg, args...) WRITE_ULOG("WRRC WARNING: "msg, ##args)
#define WRRC_ULOG_INFO(msg, args...)    WRITE_ULOG(msg, ##args)
#endif

/*===========================================================================
FUNCTION     WRRC_ULOG_INIT

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
extern void wrrc_ulog_init(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================
FUNCTION     WRRC_GET_ULOG_HANDLE

DESCRIPTION
  This function is invoked for getting the wrm ulog handle. This is required for
  ULOG logging to the corresponding handle only.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  wrrc_ulog_handle - ULogHandle in use by WRRC code.

SIDE EFFECTS
  None.
===========================================================================*/
extern ULogHandle wrrc_get_ulog_handle(void);

#endif /*TEST_FRAMEWORK*/
#endif /*RRCULOG_H*/


