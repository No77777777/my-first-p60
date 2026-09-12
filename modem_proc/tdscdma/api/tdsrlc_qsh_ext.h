#ifndef TDSCDMA_RLC_QSH_EXT_H
#define TDSCDMA_RLC_QSH_EXT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            TDSRLC_QSH_EXT.H                 

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
2/06/15    sarao     Initial version for tdsrlc_qsh_ext.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include <string.h>
#include <qsh.h>




#define TDSRLC_QSH_MAJOR_VER 0
#define TDSRLC_QSH_MINOR_VER 1
#define TDS_RLC_QSH_MAX_BAD_STATUS_PDUS_LOGGING 5




typedef enum
{
  TDSRLC_QSH_METRIC_RLC_STAT
}tdsrlc_qsh_metric_rlc_e;

typedef struct
{
	uint8 rb_id;
	uint16 vt_s;
	uint16 vt_a;
	uint16 sn;
}tdsrlc_qsh_bad_status_pdu_stats_s;

typedef struct
{
  qsh_metric_hdr_s hdr; 									/* This header has the timestamp (uint16) */

  uint16 num_rlc_reset_dl;									/* Number of RLC Resets on DL  */
  uint16 num_rlc_reset_ul;									/* Number of RLC Resets on UL  */
  uint16 num_rlc_urecov_err;								/* Number of RLC Unrecoverable Errors  */
  uint16 rlc_fc_win_size;									/* Latest UL FC window size in last 1 sec */
  uint32 num_pdu_bytes_tx;									/* Number of RLC Total PDU Bytes sent out last sec */
  uint32 num_pdu_bytes_retx;								/* Number of RLC Retransmission PDU Bytes sent out last sec */
  uint32 rlc_dl_tput;										/* Currently providing bits/sec of PDUs rx on DL assuming QSH calls us every 1 sec */
  uint32 rlc_ul_tput;										/* Currently providing bits/sec of PDUs Tx on UL assuming QSH calls us every 1 sec */
  tdsrlc_qsh_bad_status_pdu_stats_s rlc_bad_status_pdu_stats[TDS_RLC_QSH_MAX_BAD_STATUS_PDUS_LOGGING];  /*Logging last 5 Bad status PDUs details in last sec */
  
}tdsrlc_qsh_metric_data_stats_s;


#endif
