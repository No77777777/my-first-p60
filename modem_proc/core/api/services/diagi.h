#ifndef DIAGI_H
#define DIAGI_H

/*! 
 @ingroup diag_misc 
  @file diagi.h 
  @brief 
    Diagnostic services miscellaneous header File.
    Provides Diagnostic services miscellaneous APIs  
*/

/*
===========================================================================

    Copyright (c) 1990-2012, 2015-2016, 2018 QUALCOMM Technologies, Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary

===========================================================================

                            Edit History

$Header: //components/rel/core.mpss/10.0/api/services/diagi.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/27/18   gn      Featurize SIO and DSM code under DIAG_SIO_SUPPORT
07/08/16   gn      Unused Public API removal
07/09/15   is      Removed some un-used Modem feature flags
11/16/12   ph      Klocwork warning fixes
10/14/11   hm      Diag consumer API - modem side changes
05/21/10   SUG     Doxygenation Changes
09/30/09   JV      Removed FEATURE_DIAG_NV from this public header file. Removed
                   redundant prototype for diag_nv_read().
07/31/09   JV      Added FEATURE_DIAG_NV back (no NV functionality on the Q6)
07/31/09   JV      Merged Q6 diag code back to mainline
07/21/09   mad     Changed prototype of diag_wait to remove osal.h inclusion
07/15/09   mad     Removed un-necessary includes:diag.h,diagbuf.h,log.h
07/14/09   JV      Introduced osal.
07/10/09   as      Mainlined code under FEATURE_DIAG_NV
05/07/09   vk      removed dependencies from diagi_v.h 
12/12/08   vg      CMI creating diagi.h diagi_v.h
10/03/08   vg      Updated code to use PACK() vs. PACKED
11/21/06   as      Moved DIAG internal features from diagi.h to diag.h
08/28/06   as      Added win mobile featurization support.
03/28/06   pc      Changed DIAG_TX_APP_SIG from 0x00800000 to 0x00080000.
10/19/05   as      Modified diag_searchlist_type to make it 4-byte aligned.
07/05/05   sl      Added support for SPC_TIMEOUT to double the timeout value
                   on consecutive incorrect SPCs.
06/16/05   as      New signal for DM to communicate completion.
05/17/05   as      Added new signal for Application processor data.
10/24/01   jal     New signal for SIO to indicate the UART is flushed.
                   New signal for SIO to indicate port closure is complete.
08/20/01   jal     Supported more Diag packets.  Added NV cache typedef,
                   prototypes for diag_set_sp_state(), downloader status.
06/27/01   lad     Various non-functional changes to facilitate update of
                   log services.
04/06/01   lad     Added definitions of DIAG_TASK_xxx sigs to decouple from 
                   task.h.
                   ASYNC con<F2st definitions moved to diagcomm.h?
                   Externalized msg_nice[] and log_nice[] for streaming config 
                   support.
                   Moved prototype of diag_kick_watchdog() to diagtarget.h.
                   Added prototype of diagpkt_process_request() (moved from 
                   diagpkt.h).
                   Updated prototype for diag_do_escaping().
                   Removed prototype for diag_do_unescaping().
                   Removed references to diagpkt_refresh_cache().
02/23/01   lad     Rearchitected the diagnostics subsystem to be a service 
                   rather than a module.  All coupling to targets has been
                   moved to target-specific implementations.  The only coupling
                   that remains are common services and REX.
                   Old history comments have been removed since most of them
                   are no longer applicable.

===========================================================================*/


#include "comdef.h"
#include <string.h>

#if defined(DIAG_SIO_SUPPORT)
  #include "dsm.h"
#endif

/*! 
   @name diag Signal Codes    
   @ingroup diag_misc   
*/ 

/*=========================================================================*/
/*   START TEMP For Bringup (Virtio)                                         */
/*=========================================================================*/

/* Diag config values */
#define DIAG_TX_SLEEP_THRESHOLD_DEFAULT (1024*16)
#define DIAG_TX_SLEEP_TIME_DEFAULT 10

/* On Device feature */
//#define FEATURE_ONDEVICE_DIAG
#define ONDEVICE_DIAG_MOUNT   "/hdev/sdc4h0p1"
#define ONDEVICE_DIAG_DIR     "/hdev/sdc4h0p1/diag"
#define ONDEVICE_DIAG_CFG     "/hdev/sdc4h0p1/config/Diag.cfg"  


  /* ---------------------------------------------------------------------
   *                    Core Image Feature Flags
   * --------------------------------------------------------------------- */
#ifdef FEATURE_COREIMG

  /* COREIMG Feature flags can be added here */

#else /* #ifdef FEATURE_COREIMG */

  /* ---------------------------------------------------------------------
   *                 Non-Core Image Feature Flags
   * --------------------------------------------------------------------- */

#ifdef FEATURE_IS2000
  #define DIAG_IS2000
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_ADSP_DIAG
  #define FEATURE_DIAG_ADSP_CALLBACK
#endif /* FEATURE_ADSP_DIAG */
   
#if defined (IMAGE_MODEM_PROC)
  //#define DIAG_CM_AVAIL
#endif 

#if defined(IMAGE_MODEM_PROC) && defined(FEATURE_WCDMA)
  #define FEATURE_DIAG_WCDMA_CFN_SFN_TIMESTAMP
#endif


#if defined(FEATURE_WCDMA)
  #define DIAG_WCDMA
#endif

#if defined(FEATURE_GSM)
  #define DIAG_GSM
#endif


#endif /* #ifndef FEATURE_COREIMG */

#endif /* DIAGI_H */
