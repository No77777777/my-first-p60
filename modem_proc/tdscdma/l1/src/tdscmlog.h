#ifndef L1CMLOG_H
#define L1CMLOG_H

/*===========================================================================
                     L 1 C M L O G . H

DESCRIPTION
  This header file describes the CcTrCh log packet definitions

Copyright (c) 2000 - 2008 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdscmlog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
09/22/09    sup     Removed log code assignments and placed them in
                    log_codes_tds_int.h
06/10/08    ms      Added PACKED_POST attribute for structures to make it
                    compatible for GNU compiler.
05/19/05    bbd     Changed datatype of fcch_tone_qs_offset to resolve lint error.
03/31/04    gs      Scaled back the threshold for swapping double buffer
                    for compressed mode GSM logging. It is in macro
                    TDSSRCHGSM_LOG_SUBMIT_SIZE_LIMIT
11/06/03    gs      Deleted dulicate #defines copied from other modules.
10/04/02    gs      Corrected the type of RSSI in periodic RSSI log info.
                    Renamed cell_state to cell_meas_state to give more
                    logical name.
09/27/02    gs      Added GSM CM meas log entity Id
                    TDSSRCHGSM_PERIODIC_RSSI_LOG_INFO_ID.
                    Added cell state to GSM periodic measurement log info.
08/09/02    gs      Created this file
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "qw.h"
#include "log.h"
//#include "log_codes_tds_int.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Header definition since the header definition is defined in log.c */
typedef PACKED struct PACKED_POST
{
  uint16 len;  /* Specifies the length, in bytes of 
                 the entry, including this header. */

  uint16 code; /* Specifies the log code for the 
                  entry as enumerated above.       
                  Note: This is specified as word 
                  to guarantee size.               */

  qword ts;    /* The system timestamp for the log entry. The upper 48 bits
                  represent elapsed time since 6 Jan 1980 00:00:00 
                  in 1.25 ms units. The low order 16 bits represent elapsed 
                  time since the last 1.25 ms tick in 1/32 chip units 
                  (this 16 bit counter wraps at the value 49152).          */
} tdscm_log_hdr_struct_type;


typedef PACKED struct PACKED_POST
{
  /* number of GSM carrier scanned */
  uint8 num_carriers;
  /* RSSI samples per carrier */
  uint8 num_samples;
  /* Time taken for collection of these samples */
  uint8 num_w_frames_used;
} tdscm_rssi_report_prd_log_info_struct_type;

typedef PACKED struct PACKED_POST
{
  /* Carrier RF channel number */
  uint16 carrier_arfcn;
  /* RSSI dBmx16 */
  int16 rssi_dbmx16;
  /* This indicate cell state and is translation of
     tdssrchgsm_srch_meas_state in to uint8 */
  uint8 cell_meas_state;
} tdscm_gsm_carrier_rssi_report_log_info_struct_type;

#define TDSCM_GSM_RSSI_RPT_PRD_LOG_INFO_SZ (sizeof(tdscm_rssi_report_prd_log_info_struct_type))
#define TDSCM_GSM_CAR_RSSI_RPT_LOG_INFO_SZ (sizeof(tdscm_gsm_carrier_rssi_report_log_info_struct_type))

/* L1 CM log packet definition */
/* --------------------------- */

#endif /* L1CMLOG_H */

