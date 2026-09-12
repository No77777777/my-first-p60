#ifndef TDSEULENCI_H
#define TDSEULENCI_H

/*==========================================================================
             TDSCDMA L1 HSUPA UL ENC driver related header file

DESCRIPTION
  This file contains definition, declaration required for HSUPA UL ENC driver
  
  Copyright (c) 2013 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/dime/tdseulenci.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/14/13   yuw  Initial check-in for dime
===========================================================================*/


/* ---------------------------- */
/* Define/unde local features   */
/* ---------------------------- */

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_TDSCDMA_HSUPA

#include "tdseulcfg.h"

#include "tdseulcfg.h"
#include "tdsl1rrcif.h"
#include "tdsl1macif.h"

#ifdef FEATURE_TDSCDMA_QTF_A2_SIM
//ERAM buffer for Off-target A2 Simulator
uint8 off_target_eram_buffer[ 8 ][0x1000];
#define TDSEULENC_DEST_ERAM_BANK_REL_OFST_RD(curr_eram_bank)  (&off_target_eram_buffer[ curr_eram_bank ])

uint8 test_a2_output_buf[8][0x1000];
#else
// Macro to point to the relative offset of the ERAM bank. 
#define TDSEULENC_DEST_ERAM_BANK_REL_OFST_RD(curr_eram_bank)  \
                ((curr_eram_bank   << TDSEULENC_ERAM_BANK_ADDR_OFST))

#endif /* FEATURE_TDSCDMA_QTF_A2_SIM */


/* Nikel+TD HW interface TD_TX_EUL_CODING_PARAM_BANKc 
* Bits     Name Description
 * 18:17    Num_code_segs
 * 16:0     Num_pre_rm_bits  Ne,j 
  */
//#define TDSEULENC_PRE_RM_BITS_BMSK     0x01FFFF
#define TDSEULENC_PRE_RM_BITS_BMSK     HWIO_FMSK(TD_TX_EUL_CODING_PARAM_BANKc,NUM_PRE_RM_BITS)
//#define TDSEULENC_NUM_CODE_BLKS_BMSK   0x060000
#define TDSEULENC_NUM_CODE_BLKS_BMSK   HWIO_FMSK(TD_TX_EUL_CODING_PARAM_BANKc,NUM_CODE_SEGS)
//#define TDSEULENC_NUM_CODE_BLKS_SHFT   17
#define TDSEULENC_NUM_CODE_BLKS_SHFT   HWIO_SHFT(TD_TX_EUL_CODING_PARAM_BANKc,NUM_CODE_SEGS) 

/* Nikel+TD HW interface TD_TX_EUL_CODE_SEG_SIZE_BANKc 
 * Bits     Name Description
 * 18:13    Num_filler_bits
 * 12:0     Num_bits_per_code_block 
  */
//#define TDSEULENC_BITS_PER_BLK_BMSK    0x001FFF
#define TDSEULENC_BITS_PER_BLK_BMSK    HWIO_FMSK(TD_TX_EUL_CODE_SEG_SIZE_BANKc,NUM_BITS_CODE_SEG)
//#define TDSEULENC_NUM_FILL_BITS_BMSK   0x07E000
#define TDSEULENC_NUM_FILL_BITS_BMSK   HWIO_FMSK(TD_TX_EUL_CODE_SEG_SIZE_BANKc,NUM_FILLER_BITS)
//#define TDSEULENC_NUM_FILL_BITS_SHFT   13
#define TDSEULENC_NUM_FILL_BITS_SHFT   HWIO_SHFT(TD_TX_EUL_CODE_SEG_SIZE_BANKc,NUM_FILLER_BITS)


/* Nikel+TD HW interface TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc
 * Bits     Name Description
 * 23:16    NUM_COLUMNS_MINUS1
 * 15:8     LAST_COLUMN
 * 7:6      NUM_ROW_INDEX
 * 5        LAST_ROW_BIT_EXCH
 * 4:0      LAST_ROW
 */
//#define TDSEULENC_TURBO_INTLV_LAST_ROW_BMSK           0x1F
#define TDSEULENC_TURBO_INTLV_LAST_ROW_BMSK          HWIO_FMSK(TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc,LAST_ROW)
//#define TDSEULENC_TURBO_INTLV_LAST_ROW_BIT_EXCH_SHFT  5
#define TDSEULENC_TURBO_INTLV_LAST_ROW_BIT_EXCH_SHFT HWIO_SHFT(TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc,LAST_ROW_BIT_EXCH)
//#define TDSEULENC_TURBO_INTLV_NUM_ROW_INDEX_SHFT      6
#define TDSEULENC_TURBO_INTLV_NUM_ROW_INDEX_SHFT     HWIO_SHFT(TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc,NUM_ROW_INDEX)
//#define TDSEULENC_TURBO_INTLV_LAST_COLUMN_SHFT        8
#define TDSEULENC_TURBO_INTLV_LAST_COLUMN_SHFT       HWIO_SHFT(TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc,LAST_COLUMN)
//#define TDSEULENC_TURBO_INTLV_NUM_COLUMNS_MINUS1_SHFT 16
#define TDSEULENC_TURBO_INTLV_NUM_COLUMNS_MINUS1_SHFT HWIO_SHFT(TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc,NUM_COLUMNS_MINUS1)

/* Nikel+TD HW interface TD_TX_EUL_TURBO_INTLV_PARAM1_BANKc
 * 14:9     PRIME_NUM_INDEX
 * 8:0      PRIME_NUMBER
 */
//#define TDSEULENC_TURBO_INTLV_PRIME_NUMBER_BMSK     0x1FF
#define TDSEULENC_TURBO_INTLV_PRIME_NUMBER_BMSK     HWIO_FMSK(TD_TX_EUL_TURBO_INTLV_PARAM1_BANKc,PRIME_NUMBER)
//#define TDSEULENC_TURBO_INTLV_PRIME_NUM_INDEX_SHFT  9
#define TDSEULENC_TURBO_INTLV_PRIME_NUM_INDEX_SHFT  HWIO_SHFT(TD_TX_EUL_TURBO_INTLV_PARAM1_BANKc,PRIME_NUM_INDEX)

/* Nikel+TD HW interface TD_TX_EUL_RM_SYS_EINI
 * 16       EUL_PUNC_EN_SYS
 * 15:0     Eini for system bits
 */
//#define TDSEULENC_RM_SYS_EINI_BMASK    0xFFFF
#define TDSEULENC_RM_SYS_EINI_BMASK       HWIO_FMSK(TD_TX_EUL_RM_SYS_EINI,INITIAL_ERROR_S)
//#define TDSEULENC_RM_PUNCT_EN_SYS_SHFT    16
#define TDSEULENC_RM_PUNCT_EN_SYS_SHFT    HWIO_SHFT(TD_TX_EUL_RM_SYS_EINI,EUL_PUNCT_EN_SYS)  

/* Nikel+TD HW interface TD_TX_EUL_RM_SYS_EPLUS_EMINUS
 * 31:16    EPLUS for system bits
 * 15:0     EMINUS for system bits
 */
//#define TDSEULENC_RM_SYS_EMINUS_BMASK   0xFFFF
#define TDSEULENC_RM_SYS_EMINUS_BMASK     HWIO_FMSK(TD_TX_EUL_RM_SYS_EPLUS_EMINUS,EMINUS_S)
//#define TDSEULENC_RM_SYS_EPLUS_SHFT    16
#define TDSEULENC_RM_SYS_EPLUS_SHFT       HWIO_SHFT(TD_TX_EUL_RM_SYS_EPLUS_EMINUS,EPLUS_S)

/* Nikel+TD HW interface TD_TX_EUL_RM_P1_EINI
 * 16       EUL_PUNC_EN_PARITY
 * 15:0     Eini for parity 1 bits
 */
//#define TDSEULENC_RM_P1_EINI_BMASK    0xFFFF
#define TDSEULENC_RM_P1_EINI_BMASK        HWIO_FMSK(TD_TX_EUL_RM_P1_EINI,INITIAL_ERROR_P1)
//#define TDSEULENC_RM_PUNCT_EN_P1_SHFT    16
#define TDSEULENC_RM_PUNCT_EN_P1_SHFT     HWIO_SHFT(TD_TX_EUL_RM_P1_EINI,EUL_PUNCT_EN_PARITY)

/* Nikel+TD HW interface TD_TX_EUL_RM_P1_EPLUS
  * 15:0     Eplus for parity 1 bits
 */
//#define TDSEULENC_RM_P1_EPLUS_BMASK    0xFFFF
#define TDSEULENC_RM_P1_EPLUS_BMASK      HWIO_RMSK(TD_TX_EUL_RM_P1_EPLUS)

/* Nikel+TD HW interface TD_TX_EUL_RM_P1_EPMINUS
  * 15:0     Eminus for parity 1 bits
 */
//#define TDSEULENC_RM_P1_EMINUS_BMASK    0xFFFF
#define TDSEULENC_RM_P1_EMINUS_BMASK      HWIO_RMSK(TD_TX_EUL_RM_P1_EMINUS)  

/* Nikel+TD HW interface TD_TX_EUL_RM_P2_EINI
  * 15:0     Eini for parity 2 bits
 */
//#define TDSEULENC_RM_P2_EINI_BMASK    0xFFFF
#define TDSEULENC_RM_P2_EINI_BMASK      HWIO_RMSK(TD_TX_EUL_RM_P2_EINI)

/* Nikel+TD HW interface TD_TX_EUL_RM_P2_EPLUS_EMINUS
 * 31:16    EPLUS for parity 2 bits
 * 15:0     EMINUS for parity 2 bits
 */
//#define TDSEULENC_RM_P2_EMINUS_BMASK   0xFFFF
#define TDSEULENC_RM_P2_EMINUS_BMASK     HWIO_FMSK(TD_TX_EUL_RM_P2_EPLUS_EMINUS,EMINUS_P2)
//#define TDSEULENC_RM_P2_EPLUS_SHFT    16
#define TDSEULENC_RM_P2_EPLUS_SHFT       HWIO_SHFT(TD_TX_EUL_RM_P2_EPLUS_EMINUS,EPLUS_P2)

/* Nikel+TD HW interface TD_TX_EUL_RM_SYS_NUMBITS
  * 15:0     NT_SYS_NUMBITS
 */
//#define TDSEULENC_RM_NT_SYS_NUMBITS_BMASK    0xFFFF
#define TDSEULENC_RM_NT_SYS_NUMBITS_BMASK      HWIO_RMSK(TD_TX_EUL_RM_SYS_NUMBITS)
/* Nikel+TD HW interface TD_TX_EUL_RM_P1P2_EMINUS
 * 31:16    NT_P2_NUMBITS
 * 15:0     NT_P1_NUMBITS
 */
//#define TDSEULENC_RM_NT_P1_NUMBITS_BMASK   0xFFFF
#define TDSEULENC_RM_NT_P1_NUMBITS_BMASK   HWIO_FMSK(TD_TX_EUL_RM_P1P2_NUMBITS,NT_P1_NUMBITS) 
//#define TDSEULENC_RM_NT_P2_NUMBITS_SHFT    16
#define TDSEULENC_RM_NT_P2_NUMBITS_SHFT    HWIO_SHFT(TD_TX_EUL_RM_P1P2_NUMBITS,NT_P2_NUMBITS)

#define TDSEULENC_RM_BANK_SEL_BMASK    HWIO_RMSK(TD_TX_EUL_RM_BANK_SEL)

/* Nikel+TD HW interface TD_TX_EUL_MOD_CTL
 * 3     MOD_MODE
 * 1:0     NT_P1_NUMBITS
 */
//#define TDSEULENC_CONSTE_REARRANG_BMASK   0x3
#define TDSEULENC_CONSTE_REARRANG_BMASK     HWIO_FMSK(TD_TX_EUL_MOD_CTL,CONSTELLATION_REARRANGE)
//#define TDSEULENC_MOD_MODE_SHFT           3
#define TDSEULENC_MOD_MODE_SHFT             HWIO_SHFT(TD_TX_EUL_MOD_CTL,MOD_MODE)   

#endif /* FEATURE_TDSCDMA_HSUPA */

#endif /* TDSEULENCI_H */

