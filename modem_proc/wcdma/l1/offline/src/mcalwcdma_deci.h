#ifndef MCALWCDMA_DECI_H
#define MCALWCDMA_DECI_H

/*============================================================================*/
/** @file  
 * This module contains definition declaration for MCAL (modem core abstraction
 * layer) WCDMA decoder module. This header filer is only intended to be included
 * in MCAL WCDMA decoder related modules for internal use only.
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2015 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/mcalwcdma_deci.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/26/15    ms      DRMB fixes for TA.2.x
04/22/15    ac      Datapath W+W changes
01/26/15    ac      Initial checkin for X+W
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "comdef.h"
#include "mcalwcdma_dec.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* TrCh ref type bit field */
#define MCALWCDMA_DEC_TRCH_REFTYPE_SBCH_BF 0x01 /**< TrCh reference type S_BCH */
#define MCALWCDMA_DEC_TRCH_REFTYPE_NBCH_BF 0x02 /**< TrCh reference type N_BCH */
#define MCALWCDMA_DEC_TRCH_REFTYPE_DCH_BF  0x04 /**< TrCh reference type DCH */
#define MCALWCDMA_DEC_TRCH_REFTYPE_DSCH_BF 0x08 /**< TrCh reference type DSCH (Obsolete) */
#define MCALWCDMA_DEC_TRCH_REFTYPE_PCH_BF  0x10 /**< TrCh reference type PCH */
#define MCALWCDMA_DEC_TRCH_REFTYPE_FACH_BF 0x20 /**< TrCh reference type FACH */

/* Value for CRC length for each CRC enum type */
#define MCALWCDMA_DEC_CRC_LENGTH_0_VAL     0  /**< CRC length 0 size */
#define MCALWCDMA_DEC_CRC_LENGTH_8_VAL     8  /**< CRC length 8 size */
#define MCALWCDMA_DEC_CRC_LENGTH_12_VAL    12 /**< CRC length 12 size */
#define MCALWCDMA_DEC_CRC_LENGTH_16_VAL    16 /**< CRC length 16 size */
#define MCALWCDMA_DEC_CRC_LENGTH_24_VAL    24 /**< CRC length 24 size */


/** Macro to translate CRC enum to its absolute value */
#define MCALWCDMA_DEC_CRC_LENGTH_VAL(crc_enum) mcalwcdma_dec_crc_length_val_table[(crc_enum)]


/*    CCTrCh table info    */
/* ----------------------- */

/* Number of 32 bit word in mDSP CCTrCh table */
/*lint -emacro(506,DEC_MDSP_NUM_MAX_DWORD_CCTRCH_TABLE) Max number of W32 needed to 
  store a CCTrCh table in mDSP */ 
#define DEC_MDSP_NUM_MAX_DWORD_CCTRCH_TABLE \
 (DEC_MDSP_NUM_DWORD_CCTRCH_HDR + \
  DEC_MDSP_NUM_MAX_DWORD_TTI_ODRT + \
  DEC_MDSP_NUM_MAX_DWORD_TRCHT + \
  DEC_MDSP_NUM_MAX_DWORD_TFT + \
  DEC_MDSP_NUM_MAX_DWORD_TFCI_TABLE) 

/* Non PCCPCH R99 first and last CCTrCh table indices */
#define MCALWCDMA_DEC_FIRST_NON_PCCPCH_TABLE_IDX 0
#define MCALWCDMA_DEC_LAST_NON_PCCPCH_TABLE_IDX  (WFW_DBACK_NUM_CCTRCH_TABLES - 1)

/* Number of words in PCCPCH CCTrCh */
#define DEC_MDSP_NUM_W16_PCCPCH_CCTRCH_TABLE    1
#define MCALWCDMA_DEC_FIRST_PCCPCH_TABLE_IDX    WFW_DBACK_PCCPCH_CCTRCH_ID_FIRST
#define MCALWCDMA_DEC_LAST_PCCPCH_TABLE_IDX     WFW_DBACK_PCCPCH_CCTRCH_ID_LAST

#define DEC_MIN_RATE_MATCH_ATTRIB 1
#define DEC_MAX_RATE_MATCH_ATTRIB 256

/* DRM buffer storage locations are their parameters */

/* Number of DRM segments for BCH type CCTrCh */
#define MCALWCDMA_DEC_CCTRCH_BCH_TYPE_NUM_DRM_INFO 8

/* Number of segments for each sub */
#define MCALWCDMA_DEC_CCTRCH_R99_TYPE_NUM_DRM_INFO_PER_SUB 2

/* Number of DRM segments for R99 type CCTrCh */
#define MCALWCDMA_DEC_CCTRCH_R99_TYPE_NUM_DRM_INFO (WCDMA_NUM_SUBS * MCALWCDMA_DEC_CCTRCH_R99_TYPE_NUM_DRM_INFO_PER_SUB)

/* Number of areas in which DRM buffer is divided */
#define MCALWCDMA_DEC_NUM_DRM_BUF_AREA  \
  (MCALWCDMA_DEC_CCTRCH_R99_TYPE_NUM_DRM_INFO + \
   MCALWCDMA_DEC_CCTRCH_BCH_TYPE_NUM_DRM_INFO)

/* R99 CCTrCh uses DRM buffer parameter info starting
   at index 8 (may use index 9 if 8 is already allocated */
#define MCALWCDMA_DEC_CCTRCH_R99_TYPE_DRM_INFO_IDX 8

/* TURBO BETA SELECT types */

#define DEC_TURBO_BETA_SELECT_NO_LUT 0
#define DEC_TURBO_BETA_SELECT_3      1
#define DEC_TURBO_BETA_SELECT_6      2
#define DEC_TURBO_BETA_SELECT_9      3
#define DEC_TURBO_BETA_SELECT_12     4



/* CCTrCh TrCh TF Bit field masks, shifts etc. */
/* ------------------------------------------- */

/* CCTrCh table headers */
/* -------------------- */

/* Dirty Bit: Bit 31 */
#define DEC_CCTRCHT_DIRTY_BIT_BMSK 0x80000000
#define DEC_CCTRCHT_DIRTY_BIT_REG  0
#define DEC_CCTRCHT_DIRTY_BIT_SHFT 31

/* DRM buffer start addr: Bit 30-16 */
#define DEC_CCTRCHT_DBACK_ST_ADDR_BMSK 0x7FFF0000
#define DEC_CCTRCHT_DBACK_ST_ADDR_REG  0
#define DEC_CCTRCHT_DBACK_ST_ADDR_SHFT 16

/* TFCI bit : bit 15 */
#define DEC_CCTRCHT_TFCI_BMSK 0x00008000
#define DEC_CCTRCHT_TFCI_REG  0
#define DEC_CCTRCHT_TFCI_SHFT 15

/* DRM buffer storage dir: Bit 14 */
#define DEC_CCTRCHT_STORAGE_DIR_BMSK 0x00004000
#define DEC_CCTRCHT_STORAGE_DIR_REG  0
#define DEC_CCTRCHT_STORAGE_DIR_SHFT 14

/* TFCI table index : bit 13 */
#define DEC_CCTRCHT_TFCIT_IDX_BMSK 0x00002000
#define DEC_CCTRCHT_TFCIT_IDX_REG  0
#define DEC_CCTRCHT_TFCIT_IDX_SHFT 13

/* MBMS Num: Bit 8-5 */
#define DEC_CCTRCHT_MBMS_NUM_BMSK 0x000001E0
#define DEC_CCTRCHT_MBMS_NUM_REG  0
#define DEC_CCTRCHT_MBMS_NUM_SHFT 5

/* Number of TrCh : bit 4-0 */
#define DEC_CCTRCHT_NUM_TRCH_BMSK 0x0000001F
#define DEC_CCTRCHT_NUM_TRCH_REG  0
#define DEC_CCTRCHT_NUM_TRCH_SHFT 0

/* Transport channel table */
/* ---------------------- */


/* TrCh enable/disable : bit 30 */
#define DEC_TRCHT_ENABLE_BMSK 0x40000000
#define DEC_TRCHT_ENABLE_REG  0
#define DEC_TRCHT_ENABLE_SHFT 30

/* max TF size index in the TrCh : bit 28-23 */
#define DEC_TRCHT_MAX_TFI_BMSK 0x1F800000
#define DEC_TRCHT_MAX_TFI_REG  0
#define DEC_TRCHT_MAX_TFI_SHFT 23

/* TTI of the TrCh : bit 22-21 */
#define DEC_TRCHT_TTI_BMSK 0x00600000
#define DEC_TRCHT_TTI_REG  0
#define DEC_TRCHT_TTI_SHFT 21

/* CRC type of the TrCh : bit 20-18 */
#define DEC_TRCHT_CRC_BMSK 0x001C0000
#define DEC_TRCHT_CRC_REG  0
#define DEC_TRCHT_CRC_SHFT 18

/* Code type of the TrCh : bit 17-16 */
#define DEC_TRCHT_CODE_TYPE_BMSK 0x00030000
#define DEC_TRCHT_CODE_TYPE_REG  0
#define DEC_TRCHT_CODE_TYPE_SHFT 16

/* Transport format table base of the TrCh : bit 15-10 */
#define DEC_TRCHT_TFT_BASE_BMSK 0x0000FC00
#define DEC_TRCHT_TFT_BASE_REG  0
#define DEC_TRCHT_TFT_BASE_SHFT 10

/* Number of transport formats in the TrCh */
#define DEC_TRCHT_NUM_TF_BMSK 0x0000007F
#define DEC_TRCHT_NUM_TF_REG  0
#define DEC_TRCHT_NUM_TF_SHFT 0


/* Number of transport formats in the TrCh : bit 6-0 */
#define DEC_TRCHT_NUM_TF_BMSK 0x0000007F
#define DEC_TRCHT_NUM_TF_REG  0
#define DEC_TRCHT_NUM_TF_SHFT 0

/* Transport format Pool table */
/* ---------------------- */

/* Rate matched size per radio frame : bit 29-16 */
#define DEC_TFT_RM_SIZE_PER_RD_FR_BMSK 0x3FFF0000
#define DEC_TFT_RM_SIZE_PER_RD_FR_REG  0
#define DEC_TFT_RM_SIZE_PER_RD_FR_SHFT 16

/* Number of transport blocks */
#define DEC_TFT_NUM_TRBLK_BMSK 0x0000FC00
#define DEC_TFT_NUM_TRBLK_REG  0
#define DEC_TFT_NUM_TRBLK_SHFT 10

/* Number of code blocks */
#define DEC_TFT_NUM_CODE_BLK_BMSK 0x0000003F
#define DEC_TFT_NUM_CODE_BLK_REG  0
#define DEC_TFT_NUM_CODE_BLK_SHFT 0

/* Code block size */
#define DEC_TFT_CODE_BLK_SIZE_BMSK 0x1FFF0000
#define DEC_TFT_CODE_BLK_SIZE_REG  1
#define DEC_TFT_CODE_BLK_SIZE_SHFT 16

/* Transport block size */
#define DEC_TFT_TRBLK_SIZE_BMSK 0x00001FFF
#define DEC_TFT_TRBLK_SIZE_REG  1
#define DEC_TFT_TRBLK_SIZE_SHFT 0

/* DRM scale factor */
#define DEC_TF_DRM_SCALE_FACTOR_BMSK 0xE0000000 
#define DEC_TF_DRM_SCALE_FACTOR_REG  2
#define DEC_TF_DRM_SCALE_FACTOR_SHFT 29

/* RM parameter ePlusOverA */
#define DEC_TFT_EPLUSOVERA_BMSK 0x000FFFFF
#define DEC_TFT_EPLUSOVERA_REG  2
#define DEC_TFT_EPLUSOVERA_SHFT 0

/* delta N Odd */
#define DEC_TF_DELTA_N_ODD_BMSK 0x80000000
#define DEC_TF_DELTA_N_ODD_REG  3
#define DEC_TF_DELTA_N_ODD_SHFT 31 

/* Number of filler bits */
#define DEC_TFT_NUM_FILLER_BITS_BMSK 0x7E000000
#define DEC_TFT_NUM_FILLER_BITS_REG  3
#define DEC_TFT_NUM_FILLER_BITS_SHFT 25

/* YAM threshold Id */
#define DEC_TFT_YAM_TH_ID_BMSK 0x01E00000
#define DEC_TFT_YAM_TH_ID_REG  3
#define DEC_TFT_YAM_TH_ID_SHFT 21

/* Rate matching type */
#define DEC_TFT_RM_TYPE_BMSK 0x00180000
#define DEC_TFT_RM_TYPE_REG  3
#define DEC_TFT_RM_TYPE_SHFT 19

/* RM paraneter eMinusOverA */
#define DEC_TFT_EMINUSOVERA_BMSK 0x0007FFFF
#define DEC_TFT_EMINUSOVERA_REG  3
#define DEC_TFT_EMINUSOVERA_SHFT 0

/* Turbo parameter: Last row bit exchange */
#define DEC_TFT_TP_LAST_ROW_BIT_EXG_BMSK 0x40000000
#define DEC_TFT_TP_LAST_ROW_BIT_EXG_REG  4
#define DEC_TFT_TP_LAST_ROW_BIT_EXG_SHFT 30

/* Turbo parameter: Num row index */
#define DEC_TFT_TP_NUM_ROW_IDX_BMSK 0x30000000
#define DEC_TFT_TP_NUM_ROW_IDX_REG  4
#define DEC_TFT_TP_NUM_ROW_IDX_SHFT 28

/* Turbo parameter: Num column */
#define DEC_TFT_TP_NUM_COL_BMSK 0x01FF0000
#define DEC_TFT_TP_NUM_COL_REG  4
#define DEC_TFT_TP_NUM_COL_SHFT 16

/* Turbo parameter: Prime number index */
#define DEC_TFT_TP_PRIME_NUM_IDX_BMSK 0x00003F00
#define DEC_TFT_TP_PRIME_NUM_IDX_REG  4
#define DEC_TFT_TP_PRIME_NUM_IDX_SHFT 8

/* Turbo parameter: MAX interation number */
#define DEC_TFT_TP_MAX_ITER_NUM_BMSK 0x0000000F
#define DEC_TFT_TP_MAX_ITER_NUM_REG  4
#define DEC_TFT_TP_MAX_ITER_NUM_SHFT 0

/* Turbo parameter: Turbo beta sel */
#define DEC_TFT_TP_TURBO_BETA_SEL_BMSK 0xF0000000
#define DEC_TFT_TP_TURBO_BETA_SEL_REG  5
#define DEC_TFT_TP_TURBO_BETA_SEL_SHFT 28

/* Turbo parameter: Prime number */
#define DEC_TFT_TP_PRIME_NUM_BMSK 0x01FF0000
#define DEC_TFT_TP_PRIME_NUM_REG  5
#define DEC_TFT_TP_PRIME_NUM_SHFT 16

/* Turbo parameter: Last row */
#define DEC_TFT_TP_LAST_ROW_BMSK 0x00001F00
#define DEC_TFT_TP_LAST_ROW_REG  5
#define DEC_TFT_TP_LAST_ROW_SHFT 8

/* Turbo parameter: Last column */
#define DEC_TFT_TP_LAST_COL_BMSK 0x000000FF
#define DEC_TFT_TP_LAST_COL_REG  5
#define DEC_TFT_TP_LAST_COL_SHFT 0

/* CTFC (or TFCI) table */
/* -------------------- */

/* TFCI entry valid bit */
#define DEC_TFCIT_VALID_BMSK 0x80000000
#define DEC_TFCIT_VALID_REG  0
#define DEC_TFCIT_VALID_SHFT 31

/* PDSCH spreading factor */
#define DEC_TFCIT_PDSCH_SF_BMSK 0x70000000
#define DEC_TFCIT_PDSCH_SF_REG  0
#define DEC_TFCIT_PDSCH_SF_SHFT 28

/* PDSCH OVSF code */
#define DEC_TFCIT_PDSCH_OVSF_CODE_BMSK 0xFF000000
#define DEC_TFCIT_PDSCH_OVSF_CODE_REG  1
#define DEC_TFCIT_PDSCH_OVSF_CODE_SHFT 24

/* TFI index 0 */
#define DEC_TFCIT_TFI0_BMSK 0x0000003F
#define DEC_TFCIT_TFI0_REG  0
#define DEC_TFCIT_TFI0_SHFT 0

/* TFI index 1 */
#define DEC_TFCIT_TFI1_BMSK 0x00000FC0
#define DEC_TFCIT_TFI1_REG  0
#define DEC_TFCIT_TFI1_SHFT 6

/* TFI index 2 */
#define DEC_TFCIT_TFI2_BMSK 0x0003F000
#define DEC_TFCIT_TFI2_REG  0
#define DEC_TFCIT_TFI2_SHFT 12

/* TFI index 3 */
#define DEC_TFCIT_TFI3_BMSK 0x00FC0000
#define DEC_TFCIT_TFI3_REG  0
#define DEC_TFCIT_TFI3_SHFT 18

/* TFI index 4 */
#define DEC_TFCIT_TFI4_BMSK 0x0000003F
#define DEC_TFCIT_TFI4_REG  1
#define DEC_TFCIT_TFI4_SHFT 0

/* TFI index 5 */
#define DEC_TFCIT_TFI5_BMSK 0x00000FC0
#define DEC_TFCIT_TFI5_REG  1
#define DEC_TFCIT_TFI5_SHFT 6

/* TFI index 6 */
#define DEC_TFCIT_TFI6_BMSK 0x0003F000
#define DEC_TFCIT_TFI6_REG  1
#define DEC_TFCIT_TFI6_SHFT 12

/* TFI index 7 */
#define DEC_TFCIT_TFI7_BMSK 0x00FC0000
#define DEC_TFCIT_TFI7_REG  1
#define DEC_TFCIT_TFI7_SHFT 18

/* TFI index 8 */
#define DEC_TFCIT_TFI8_BMSK 0x0000003F
#define DEC_TFCIT_TFI8_REG  2
#define DEC_TFCIT_TFI8_SHFT 0

/* TFI index 9 */
#define DEC_TFCIT_TFI9_BMSK 0x00000FC0
#define DEC_TFCIT_TFI9_REG  2
#define DEC_TFCIT_TFI9_SHFT 6

/* TFI index 10 */
#define DEC_TFCIT_TFI10_BMSK 0x0003F000
#define DEC_TFCIT_TFI10_REG  2
#define DEC_TFCIT_TFI10_SHFT 12

/* TFI index 11 */
#define DEC_TFCIT_TFI11_BMSK 0x00FC0000
#define DEC_TFCIT_TFI11_REG  2
#define DEC_TFCIT_TFI11_SHFT 18


/* PCCPCH CCTrCh bitmasks */
/* ---------------------- */

#define DEC_PCCPCH_CCTRCH_EVEN_ENABLE_BMSK 0x0001
#define DEC_PCCPCH_CCTRCH_EVEN_ENABLE_SHFT 0

#define DEC_PCCPCH_CCTRCH_EVEN_DRMB_IDX_BMSK 0x06
#define DEC_PCCPCH_CCTRCH_EVEN_DRMB_IDX_SHFT 1

#define DEC_PCCPCH_CCTRCH_ODD_ENABLE_BMSK 0x0008
#define DEC_PCCPCH_CCTRCH_ODD_ENABLE_SHFT 3

#define DEC_PCCPCH_CCTRCH_ODD_DRMB_IDX_BMSK 0x30
#define DEC_PCCPCH_CCTRCH_ODD_DRMB_IDX_SHFT 4



/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

/* =======================================================================
**                        Function Declarations
** ======================================================================= */


#endif /* MCALWCDMA_DECI_H */
