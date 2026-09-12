#ifndef DIAG_CFG_H
#define DIAG_CFG_H
/*===========================================================================

                                  D I A G _ C F G . H

  GENERAL DESCRIPTION
  Contains configuration information for diag per platform

Copyright (c) 2009 - 2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                            EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/services/diag/platform/bmp/src/diag_cfg.h#1 $

when        who    what, where, why
--------    ---   -------------------------------------------------------------
03/12/17    sa    Update UserPD commit threshold in NRT mode
02/15/17    rs    Added APIs diagpkt_max_delayed_rsp_size() and diagpkt_total_delayed_bytes_queued() and exposed them public
01/25/17    rs    Removed DIAG_CTRL_RESEND_BUF_SIZE macro since we are not using heap anymore.
12/06/16    rs    removed smdl reference
11/03/16    sa    Implemented seperate queue for UserPD events and enableed buffering
06/30/16    nk    SMD References Removal
06/27/16    ph    New macro for event report packet length size in NRT.
12/22/15    is    Fix flow control race condition
07/14/15    sa    Added support for time sync feature.
05/15/15    sr    Changed the SMDL FIFO size to 8k
04/08/15    ps    Diag over sockets and dsm removal
12/04/14    vk    Added drain threshould #define
08/04/14    rh    Update for 16kb commands
12/04/14    vk    Added drain threshould #define
07/23/14    xy    Utilized optimized time get API
04/29/13    ph    Added a limit on active listeners.
01/16/13    rh    Changed diagbuf NRT drain parameters
01/03/13    sg    Command Req/Response Channel separation
05/10/12    rh    Added separate heap for Diag responses.
10/26/11    hm    Extended Event/Log Listener APIs added
02/28/11    sg    Power saving changes
02/28/11    vs    Increased event heap size
12/10/10    vs    Reverted 8 byte alignement for q6
11/18/10    vs    Moved definition of DIAGBUF_NEXT_ALIGNED_BYTE here
11/10/10    mad   Increased event heap size to 8K
10/27/10    SG    Moved DIAG_TX_SLEEP_THRESHOLD_DEFAULT,DIAG_TX_SLEEP_TIME_DEFAULT
                  to here from diagbuf_v.h
06/07/10    VG    Updated DIAGBUF_SIZE to be able to configure it in custdiag.h
03/10/10    JV    Reduced size of diag heap back to 8K. Use system heap for
                  stress test.
03/03/10    as    Increased Diag heap to accomadate Ondevice mask processing
02/16/10    vs    Changed diagbuf size to 128K for 9k q6
01/18/10    JV    Reduce the size of diagbuf to 32K
07/17/09    JV    Created for BMP
===========================================================================*/
#include "diag_v.h"
#include "time_svc.h" /* for time_get(); for Q6 apps and all other targets */

#define DIAGBUF_NEXT_ALIGNED_BYTE(p) (((p) + 3) & ~3)

#if defined(DIAG_SOCKET_SUPPORT)

#if defined(DIAG_IMAGE_MODEM_PROC)
#define DIAG_INST_ID_BASE 0  //DIAG socket channels for modem will have instance ID range from 0--63
#elif defined (DIAG_IMAGE_QDSP6_PROC)
#define DIAG_INST_ID_BASE 64  //ADSP sockets will range from 64-127
#endif

#define DIAG_SOCKET_INST_ID_CTRL (DIAG_INST_ID_BASE)
#define DIAG_SOCKET_INST_ID_CMD (DIAG_INST_ID_BASE+1)
#define DIAG_SOCKET_INST_ID_DATA (DIAG_INST_ID_BASE+2)
#define DIAG_SOCKET_INST_ID_DCI_CMD (DIAG_INST_ID_BASE+3)
#define DIAG_SOCKET_INST_ID_DCI_DATA (DIAG_INST_ID_BASE+4)

#define DIAG_MAX_NUM_SOCKETS 6
#endif

/* Not adding the DIAG_MAX_TX_PKT_SIZ and DIAG_MAX_RX_PKT_SIZ here because
they need to be exposed to clients. Currently defined in diagpkt.h */

/* DIAGBUF_SIZE is the size, in bytes, of diag's output buffer.  If this buffer
 * fills up, logs and messages will be dropped.  It is recommended to make this
 * number a power of 2 to allow the compiler to optimize the arithmetic using
 * this number to use shift operations instead of brute force math.
 *
 * Note: This must be larger than DIAG_MAX_TX_PKT_SIZ. */

#ifndef DIAGBUF_SIZE
#define DIAGBUF_SIZE (1024*32*4)
#endif

#ifndef DIAG_CTRL_BUF_SIZE
#define DIAG_CTRL_BUF_SIZE (8*1024)
#endif


#ifndef DIAG_TX_SLEEP_THRESHOLD_DEFAULT
#define DIAG_TX_SLEEP_THRESHOLD_DEFAULT (1024*16)
#endif

#ifndef DIAG_TX_SLEEP_TIME_DEFAULT
#define DIAG_TX_SLEEP_TIME_DEFAULT 10
#endif

#define DIAGBUF_COMMIT_THRESHOLD (1024*8)                  /* # of bytes committed to Diag buffer before draining. */

#define DIAG_MAX_DATA_BLOCK_SIZE 0x4000 /* size of block that can be sent on the channel */

#define DIAG_MAX_SOCKETS_DATA_BLOCK_SIZE 0x4000 /* size of block that can be sent on the sockets channel */
/*  If the value of DIAG_MAX_SOCKETS_DATA_BLOCK_SIZE is increased it needs to be coordinated with apps, 
    and then #error check needs to be updated to match with the APPS updated value.
	Ex: On APPS Diag side if it is updated to take a bigger packet of 32KB,
	    then we nee to upadte below check to be checked for greater than 0x8000 value .*/
#if ((DIAG_MAX_SOCKETS_DATA_BLOCK_SIZE) > 0x4000)
#error "To increase DIAG_MAX_SOCKETS_DATA_BLOCK_SIZE beyond 16kb we need changes on APPS Diag to support it because apps has limit of 16kb."
#endif 

#define DIAG_CTRL_READ_BUF_SIZE 0x2000 //8k
//The number of bytes to drain from a buffer before moving onto the next one.
#define DIAG_CORE_PD_DRAIN_THRESHOLD 0x2000

/* Following #defines are for MultiPD configuration */
#ifndef DIAG_MULTIPD_BUF_SIZE
#define DIAG_MULTIPD_BUF_SIZE (1024*32)
#endif

#define DIAG_MPD_EVENT_HEAP_SIZE (1024*2)

/*
   Following variables are need to be updated with % value one wants for one's PD
   DIAG_MULTIPD_BUF_DRAIN_THRESHOLD_PER controls how much should be drained from
   the PD's diag buffer before moving to next PD's diag buffer. If one sets it too
   high one will impact all other PDs including Core PD.

   DIAG_MULTIPD_BUF_COMMIT_THRESHOLD_PER - is used to signal Diag task to drain when
   these many bytes of traffic has been generated. This is % of PD's buffer
   DIAG_MULTIPD_BUF_SIZE.
*/
#ifndef DIAG_MULTIPD_BUF_DRAIN_THRESHOLD_PER
#define DIAG_MULTIPD_BUF_DRAIN_THRESHOLD_PER 7
#endif

#ifndef DIAG_MULTIPD_BUF_COMMIT_THRESHOLD_PER
#define DIAG_MULTIPD_BUF_COMMIT_THRESHOLD_PER 7
#endif

#ifndef DIAG_MULTIPD_BUF_COMMIT_THRESHOLD_NRT_PER
#define DIAG_MULTIPD_BUF_COMMIT_THRESHOLD_NRT_PER 60
#endif


/* The size of the DIAG heap.  Currently, it is only used for response
   packets and allocating tasks for stress testing.  DIAGBUF_SIZE only
   impacts MSG and LOG packets. */
#ifndef DIAG_HEAP_SIZE
#define DIAG_HEAP_SIZE (1024 * 8)
#endif
#define DIAG_RSP_HEAP_SIZE (1024 * 32)
/* These are the non real time values that are used when diag is in power savings mode*/
#define DIAGBUF_COMMIT_THRESHOLD_NRT (1024*64)

#define DIAG_TX_SLEEP_THRESHOLD_NRT  (1024*32)

#define DIAG_TX_SLEEP_TIME_NRT       5

#define EVENT_RPT_PKT_SIZE_NRT (1000)
#define EVENT_RPT_PKT_LEN_SIZE_NRT (3000)

#define DIAG_EVENT_HEAP_SIZE (1024 * 16)
#define DIAG_EVENT_SEC_HEAP_SIZE (1024 * 4)
#define DIAG_LISTENERS_HEAP_SIZE (1024 * 8)

#define DIAG_MAX_ACTIVE_LISTENERS ( 18 )                    /*Arbitrary limit on active listeners to avoid exhaustion of system heap */

#define DIAG_HDLC_PAD_LEN (150)                            /* # of bytes padding to account for hdlc/crc bytes when filling DSM item */

/* Configurations for Diag Fwd task */
#ifndef DIAG_FWD_TX_SLEEP_THRESHOLD_DEFAULT
#define DIAG_FWD_TX_SLEEP_THRESHOLD_DEFAULT (10)         /* X number of DSMs to drain before suspending */
#endif

#ifndef DIAG_FWD_TX_SLEEP_TIME_DEFAULT
#define DIAG_FWD_TX_SLEEP_TIME_DEFAULT (10)              /* # of ms to sleep after draining X number of DSMs */
#endif

#endif /* DIAG_CFG_H */


