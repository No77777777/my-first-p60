#ifndef DIAGBUFFER_DEFS_H
#define DIAGBUFFER_DEFS_H

/*==============================================================================
@file diagbuffer_defs.h

Interface for DIAG ULog integration.

This file describes the common type-defines referenced in DIAG and ULOG.


Copyright (c) 2016,2018,2019 by Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*/

/*==============================================================================
                            Edit History

$Header: //components/rel/core.mpss/10.0/api/services/diagbuffer_defs.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/26/18   vg      Remove duplicate definition of DIAG_MAX_TX_PKT_SIZ macro
09/29/16   ph      New file created for DIAG ULOG Integration.

==============================================================================*/


#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  ULogHandle OR diagbuffer_handle can be used by external clients.
----------------------------------------------------------------------------*/
typedef  void *   ULogHandle;
typedef  ULogHandle diagbuffer_handle;

/*----------------------------------------------------------------------------
  ULogResult and diagbuffer_result can be used by external clients.
----------------------------------------------------------------------------*/
typedef  int32    ULogResult;
typedef ULogResult diagbuffer_result;

/*----------------------------------------------------------------------------
  If clients specify SYSDIAG_HANDLE while generating
  Messages/Logs/Events, they would go into system diag buffer.
----------------------------------------------------------------------------*/
#define SYSDIAG_HANDLE NULL

/*----------------------------------------------------------------------------
  Common definitions used in DIAG and DIAGBUFFER
----------------------------------------------------------------------------*/
#define DIAG_MAX_TX_PAD_SIZ (0x0100)   /* Arbitrary size to accommodate 
																									TX packet headers*/

#define DIAGBUFFER_MAX_MSG_SIZE ( DIAG_MAX_TX_PKT_SIZ + DIAG_MAX_TX_PAD_SIZ )
#define DIAGBUFFER_MAX_MSG_RSP_SIZE ULOG_MAX_MSG_SIZE

#ifdef __cplusplus
}
#endif

#endif // DIAGBUFFER_DEFS_H

