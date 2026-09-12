#ifndef MCALTDS_DECLOG_H
#define MCALTDS_DECLOG_H

/*============================================================================*/
/** @file  
 * This module has definition and declaration related to MCAL WCDMA decoder
 * log packet definition
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2006 - 2008 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_declog.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------

===========================================================================*/

#include "tdscdma_variation.h"
#ifdef FEATURE_MCAL_TDSCDMA
/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "customer.h"
#include "qw.h"
#include "log.h"
#include "mcaltds_deci.h"
#include "mcaltds_dec.h"
//#include "log_codes_tds_int.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations 
** ----------------------------------------------------------------------- */

/* DL TBset debug log packet related definitions */
/* ----------------------------------------------*/

#define TDSDL_MAX_TBSET_DUMP_SIZE_W32      400
#define TDSDL_MAX_NUM_TBSIZE_DUMP          3

LOG_RECORD_DEFINE(TDSDL_DEBUG_TBSET_PKT)
  uint8 cctrch_id; /* CCTrCh Id of this CCTrCh */
  uint8 trch_id;   /* TrCh ID */
  uint8 num_tb;    /* Number of TB in this TBset */
  uint16 tb_size;  /* TB size of the TBs */
  uint8 crc_length;   /* Number of CRC bits */
  boolean post_header; /* Header/Data read event */
  uint32 dob_data[TDSDL_MAX_TBSET_DUMP_SIZE_W32];
LOG_RECORD_END

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

#endif /* FEATURE_MCAL_TDSCDMA */
#endif /* MCALWCDMA_DECLOG_H */
