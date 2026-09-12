#ifndef TDSRMLOG_H
#define TDSRMLOG_H


/*===========================================================================
                     T D S R M L O G . H

DESCRIPTION
  This header file describes the Rate Matching log packet definitions.

Copyright (c) 2011-2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsrmlog.h#1 $ 
$DateTime: 2019/08/19 10:51:23 $ 
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
10/12/11    cdf     Initial created
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

/* + defangc@2011-10-10 */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/*! \brief Max number of elements in TF pool */
#define TDSRMLOG_MAX_TRCH                  8

/*! \brief Max number of elements in TF pool */
#define TDSRMLOG_MAX_TF                    64

typedef struct
{
  /* Word 0 */
  /*! \brief Transport channel ID, Integer number 0 to 31 */
  uint32                      trch_id:8;            

  /*! \brief Number of code segments */
  uint32                      num_code_segs:8;

  uint32                      :16;

  /* Word 1 */
  /*! \brief Number of bits per code segment */
  uint32                      num_bits_per_code_seg;

  /* Word 2 */
  /*! \brief Rate matched size per radio frame, also known as Nij */
  uint32                      pre_rm_bits;

  /* Word 3 */
  /*! \brief Number of filler bits to be used in the first code segment. */
  uint32                      num_filler_bits;

  /* Word 4 */
  /*! \brief Bit stream length after transport block concatenation */
  uint32                      len_tb_concat;

  /* Word 5 */
  /*! \brief C - 1, where C is the number of columns */
  uint8                       num_columns_minus1;

  /*! \brief last column in the last row */
  uint8                       last_column;

  /*! \brief the number of rows in the interleaver table */
  uint8                       num_row_index;

  /*! \brief last row bit exchange condition */
  uint8                       last_row_bit_exch;

  /* Word 6 */
  /*! \brief index of last row with valid data */
  uint8                       last_row;

  /*! \brief index of prime number p used in the index table */
  uint8                       prime_num_index;

  /*! \brief prime number p */
  uint16                      prime_number;
} tdsrmlog_param_per_tf_type;

/*! \brief       LOG PACKET: TDSCDMA_L1_TF_POOL_PARAM_LOG_PACKET
                 (LOG CODE: 0xD02D)

This log packet is for internal use only! 

This log packet holds the description of the UL/DL TF based rate matching 
parameters for encoding/decoding.

This record is generated when (DL/UL)DPCH/SCCPCH/PRACH channel setup/reconfig.

MIN Size: 8 bytes, MAX Size: 1800 bytes
32 +32 + 64*7*32 = 14400 bits 
*/
typedef struct
{
  /*! \brief Log header */
  log_hdr_type header;

  /* Word 0 */
  /*! \brief Log packet version; range is 0 to 255 */
  uint32  version:8;  

  /*! \brief CCTrCH ID */
  uint32  cctrch_id:1;

  /*! \brief Cell Parameter ID, Integer(0..127) */
  uint32  cpi:7;

  /*! \brief UTRA Absolute Radio Frequency Number */
  uint32  uarfcn:16;               

  /* Word 1 */
  /*! \brief Current sub-frame number (0..8191) */
  uint32  cur_subfrm:16;

  /*! \brief Current CFN (0..255)  */
  uint32  cur_cfn:8;

  /*! \brief total number of TF */
  uint32  num_tf:8;

  /*! \brief Rate Matching parameters based on TF, actual number determined by num_tf */
  tdsrmlog_param_per_tf_type param_per_tf[TDSRMLOG_MAX_TF];
} tdsrmlog_tf_pool_param_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSRMLOG_TF_POOL_PARAM_PKT_LEN(num_tf) ( \
    FPOS(tdsrmlog_tf_pool_param_pkt_type, param_per_tf)+ \
    ((num_tf) * sizeof(tdsrmlog_param_per_tf_type)))

typedef struct
{
  /* Word 0 */
  /*! \brief Transport channel Id from RRC for tracing purpose. Value: 0~31 */
  uint32  trch_id:5;

  /*! \brief rate matching type: 
   *  0 - no rm 
   *  1 - repetition
   *  2 - puncturing
  */
  uint32  rm_type:2;

  /*! \brief DeltaNij is Odd number or not */
  uint32  deltaNOdd:1;

  /*! \brief tf pool index, to indicate the index of TF pool for this TF
   *  0xFF for invalid value 
   */
  uint32  tfi:8;

  /*! \brief Rate matched size in bit: deltaNij+Nij */            
  uint32  rateMatchedSize:16; 

  /* Word 1~4 */
  /*! \brief Error variable eIni1, defined as array to support 80ms TTI,
   *  which one to use dependents on radio frame index in a TTI 
   */
  uint16  eini1[8];

  /* Word 5~8 */
  /*! \brief Error variable eIni2, defined as array to support 80ms TTI,
   *  which one to use dependents on radio frame index in a TTI 
   */
  uint16  eini2[8];

  /* Word 9 */
  /*! \brief Error variable eMinus1 */
  uint16  eminus1;

  /*! \brief Error variable eMinus2 */
  uint16  eminus2;

  /* Word 10 */
  /*! \brief Error variable ePlus1 */
  uint16  eplus1;

  /*! \brief Error variable ePlus2 */
  uint16  eplus2;
} tdsrmlog_trch_param_per_tfc_type;


/*! \brief       LOG PACKET: TDSCDMA_L1_TFC_POOL_PARAM_LOG_PACKET
                 (LOG CODE: 0xD02F)

This log packet is for internal use only! 

This log packet holds the description of the UL/DL TFC based rate matching 
parameters for encoding/decoding, one packet for one TFC.

This record is generated when (DL/UL)DPCH/SCCPCH/PRACH channel setup/reconfig.

MIN Size: 8 bytes, MAX Size: 360 bytes
32 + 32 + 32*11*8 = 2880 bits 
*/
typedef struct
{
  /*! \brief Log header */
  log_hdr_type header;

  /* Word 0 */
  /*! \brief Log packet version; range is 0 to 255 */
  uint32  version:8;  

  /*! \brief CCTrCH ID */
  uint32  cctrch_id:8;

  /*! \brief TFC id starting from 0 (actual TFCI = TFC id+1) */
  uint32  tfci:8;

  /*! \brief Number of transport channel */
  uint32  num_trch:8;

  /* Word 1 */
  /*! \brief Number of used physical channels */
  uint32  phy_num_used:8;

  /*! \brief The type of physical channel: 
   *  0 - UL DPCH 
   *  1 - PRACH
   *  2 - DL DPCH
   *  3 - SCCPCH 
   */
  uint32  chan_type:8;

  /*! \brief The number of Ndata of the selected physical channels */
  uint32  ndata_value:16;

  /*! \brief Rate Matching parameters per Trch based on TFC, actual number determined by num_trch*/
  tdsrmlog_trch_param_per_tfc_type  trch_param[TDSRMLOG_MAX_TRCH];
} tdsrmlog_tfc_pool_param_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSRMLOG_TFC_POOL_PARAM_PKT_LEN(num_trch) ( \
    FPOS(tdsrmlog_tfc_pool_param_pkt_type, trch_param)+ \
    ((num_trch) * sizeof(tdsrmlog_trch_param_per_tfc_type)))

/* - defangc@2011-10-10 */

#endif /* TDSRMLOG_H */
