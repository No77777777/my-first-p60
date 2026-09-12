#ifndef TDSDRXLOG_H
#define TDSDRXLOG_H

/*===========================================================================
                             T D S D R X L O G . H

DESCRIPTION
  This header file describes the DRX log packet definitions

Copyright (c) 1999-2011 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when        who     what, where, why
--------    ---     --------------------------------------------------------
30/08/11    weis     Created this file

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "log.h"
#include "log_codes.h"
#include "log_codes_tdscdma.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSL1DRXLOG_NUM_TIMEPOINT 33

/*       LOG PACKET: TDSCDMA_DRX_CYCLE_LOG_PACKET
        (LOG CODE: 0xD050)
*/

/*! \brief TDSCDMA sleep timeline subrecord
 *  
 *   1,344 bits total
 */

typedef struct {
  /*! \brief sleep clock counter at each time point  */
  uint32 sc[TDSL1DRXLOG_NUM_TIMEPOINT];

  /*! \brief tdscdma system sub-frame number at SDT*/
  uint16 sdt_rsc_sub_fn;
  /*! \brief tdscdma system chipx8 at SDT*/
  uint16 sdt_rsc_cx8;
  /*! \brief wall time sub-frame number at SDT*/
  uint16 sdt_frc_sub_fn;
  /*! \brief wall time chipx8 at SDT*/
  uint16 sdt_frc_cx8;

  /*! \brief restored tdscdma system sub-frame number at OLS*/
  uint16 slammed_ols_rsc_sub_fn;
  /*! \brief restored tdscdma system chipx8 at OLS*/
  uint16 slammed_ols_rsc_cx8;
  /*! \brief restored wall time sub-frame number at OLS*/
  uint16 slammed_ols_frc_sub_fn;
  /*! \brief restored wall time chipx8 at OLS*/
  uint16 slammed_ols_frc_cx8;

  /*! \brief read tdscdma system sub-frame number at OLS isr after slamming*/
  uint16 read_ols_rsc_sub_fn;
  /*! \brief read tdscdma system chipx8 at OLS isr after slamming*/
  uint16 read_ols_rsc_cx8;
  /*! \brief read wall time sub-frame number at OLS isr after slamming*/
  uint16 read_ols_frc_sub_fn;
  /*! \brief read wall time chipx8 at OLS isr after slamming*/
  uint16 read_ols_frc_cx8;

  /*! \brief time inverval from GTS to OLS in unit of chipx8*/
  uint32 sleep_duration_Cx8;

  /*! \brief timing error in unit of chipx8 with univeral stmr as reference*/
  int32  sleep_duration_error; 

  /*! \brief pich sub-frame number */
  uint16 pich_rsc_sub_fn; 
  /*! \brief reserve */
  uint16 reserve;
} tdsdrxlog_timeline_profile ;

/*! \brief       LOG PACKET: TDSCDMA_DRX_CYCLE_LOG_PACKET
        (LOG CODE: 0xD050)

This structure is used to log sleep timeline and xo frequency drift information in each drx cycle. 
 
This Log is only generated in drx mode. 
The packet size is 172 bytes .
32+1,344 = 1,376 bits
*/

typedef struct
{
  /*! Log header  */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  /*! \brief reserved */
  uint32  reserved:8;
  /*! \brief drx cycle lengh in unit of frames  */
  uint32  drx_cycle_len:16;                     
  //32 bits

  /*! \brief TDSCDMA sleep timeline information */
  tdsdrxlog_timeline_profile sleep_timeline;
  //1,344 bits 
} tdsdrxlog_drx_cycle_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSDRXLOG_DRX_CYCLE_PKT_LEN  (sizeof(tdsdrxlog_drx_cycle_pkt_type))


/*------------------PCH PATTERN DETECTING------------------------------------*/


#define TDSDRXLOG_PCH_DET_OFF 0
#define TDSDRXLOG_PCH_DET_DETECTING 1
#define TDSDRXLOG_PCH_DET_DETECTED 2


#define TDSDRXLOG_PCH_DET_NOSKIP 0
#define TDSDRXLOG_PCH_DET_SKIP_PI 1
#define TDSDRXLOG_PCH_DET_SKIP_PCH 2



/*! \brief       LOG PACKET: TDSCDMA_L1_DL_DRX_PCH_PATTERN_LOG_PACKET
        (LOG CODE: 0xD075)

DL DRX PCH PATTERN DETECTION logging. 
*/
typedef struct
{
  /* Word 0 */
  /*! \brief Log header */
  log_hdr_type header;
  /*! \brief Log packet version; range is 0 to 255 */
  uint32  version:8;
  /*! \brief OFF DETECTING DETECTED.*/
  uint32  state:2;
  /*! \brief pattern_value.*/
  uint32  pattern_val:3;
  /*! \brief NO_SKIP, SKIP PI, skip PCH.*/
  uint32  skip_type:2;
  /*! \brief subframe number */
  uint32  skip_subSfn:13;
  /*! \brief PCCPCH SNR LOWER than threshold. */
  uint32  pccpch_snr_low:1;
  /*! \brief reserved.*/
  uint32  reserved1:3; 
  /*! \brief NV enabled.*/
  uint32 nv_enable:1;
  /*! \brief PCCPCH SNR threshhold.*/
  uint32 pccpch_snr_thresh_q8:16;
  /*! \brief PCCPCH SNR threshhold.*/
  uint32 pccpch_iir_filter:7;
  /*! \brief paging rate threshhold.*/
  uint32 paging_rate_thresh:7;
  /*! \brief reserved.*/
  uint32  reserved2:1; 
} tdsdrxlog_pch_det_patt_cmd_pkt_type;

#define TDSDRXLOG_DRX_PCH_DET_PATT_PKT_LEN() (sizeof(tdsdrxlog_pch_det_patt_cmd_pkt_type))


#endif //TDSDRXLOG_H

