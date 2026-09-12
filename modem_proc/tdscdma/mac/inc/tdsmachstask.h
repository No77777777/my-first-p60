#ifndef TDSMAC_HS_DL_TASK_H
#define TDSMAC_HS_DL_TASK_H
/*============================================================================================

            M A C   H S D P A   D L   T A S K    H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface with downlink mac hs task.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

============================================================================================*/

/*============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/inc/tdsmachstask.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   ----------------------------------------------------------------------------
10/24/10   guo   Initial release Upmerged WCDMA to TDS

============================================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


/* Public portion of the API exists in tdstmcif.h */
#include "tdstmcif.h"


/* ----------------------------------------------------------------------- */
/* Private Section.                                                        */
/* ----------------------------------------------------------------------- */

#include  "queue.h"

/*============================================================================================
                        DOWNLINK LAYER 2 SIGNAL DEFINITIONS
============================================================================================*/

/* This signal is set when an item is placed on mac_hs_cmd_q */
#define     TDSMAC_HS_DL_DATA_IND_SIG         0x0001
#define     TDSMAC_HS_DL_DOG_RPT_TIMER_SIG    0x0002
#define     TDSMAC_HS_QUEUE_1_TIMER_SIG       0x0004
#define     TDSMAC_HS_QUEUE_2_TIMER_SIG       0x0008
#define     TDSMAC_HS_QUEUE_3_TIMER_SIG       0x0010
#define     TDSMAC_HS_QUEUE_4_TIMER_SIG       0x0020
#define     TDSMAC_HS_QUEUE_5_TIMER_SIG       0x0040
#define     TDSMAC_HS_QUEUE_6_TIMER_SIG       0x0080
#define     TDSMAC_HS_QUEUE_7_TIMER_SIG       0x0100
#define     TDSMAC_HS_QUEUE_8_TIMER_SIG       0x0200
#define     TDSMAC_HS_DL_CONFIG_REQ_SIG       0x0400

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING 
#define     TDSMAC_HS_DL_STATUS_LOG_TIMER_SIG 0x0800
#endif

#ifdef FEATURE_Q6_MT
#define     TDSMAC_HS_P_TO_C_DATA_IND_SIG     0x1000
#endif
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
#define     TDSMAC_HS_T1_OPT_IND_SIG          0x2000
#endif
/*=============================================================================================

FUNCTION  tds_mac_hs_dl_wait

DESCRIPTION
	This function waits for any signal in a specified signal mask to be set.
	If this procedure detects that any signal in the specified signal mask is
	set, it will return the entire mask of signals set in the task's tcb.

DEPENDENCIES
	None.

RETURN VALUE
	Mask of signals returned by the rex_wait call.

SIDE EFFECTS
	None.

=============================================================================================*/
uint32  tds_mac_hs_dl_wait
	 (
	 /* signals to wait on */
	 uint32  wait_sigs
	 );


#endif /* TDSMAC_HS_DL_TASK_H */
