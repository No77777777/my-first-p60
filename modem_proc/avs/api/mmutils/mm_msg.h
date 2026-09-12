/*========================================================================
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
======================================================================== */
/**
@file mm_msg.h

@brief This file contains a utility for generating diagnostic messages.
 This file defines macros for printing debug messages on the target or
 in simulation.
*/

/*========================================================================
Edit History

$Header: //components/dev/avs.gecko/1.0/sranumul.avs.gecko.1.0.posal_memorymap_cleanup/posal/inc/mm_msg.h

when       who     what, where, why
--------   ---     -------------------------------------------------------
05/03/18   rv      Created file.
11/12/18   akr     Cosim Implementation Integration
========================================================================== */

#ifndef _MM_MSG_H
#define _MM_MSG_H

#ifdef AVS_BUILD_SOS
#ifndef SIM
/** MSG to FARF converter & Diag utilities needed only for .so files.*/
#include "HAP_diag.h"
#else
#define MM_MSG_BASE_NEEDED
#include "mm_msg_base.h"
#endif
#else // !AVS_BUILD_SOS
/** these includes don't need to be backward compatible as
 * these are not exposed to shared libraries.*/
#define MM_MSG_BASE_NEEDED
#include "mm_msg_base.h"
#endif // AVS_BUILD_SOS


#endif // #ifndef _MM_MSG_H
