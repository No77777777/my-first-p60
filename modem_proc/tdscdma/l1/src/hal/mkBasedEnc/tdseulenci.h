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

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/mkBasedEnc/tdseulenci.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/15/13   alexyh   include encoder header file from common FW.
09/06/13   yuw      Supported Bolt
05/14/13   yuw      Initial check-in for dime
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

#ifdef FEATURE_THOR_MODEM
#include "enc_3g_cfg_api.h"
#include "enc_3g_rif.h"
#include "enc_api.h"
#include "enc_chintlv_rif.h"
#else /*FEATURE_BOLT_MODEM*/
#include "tdsenc_3g_cfg_api.h"
#include "tdsenc_3g_rif.h"
#include "tdsenc_api.h"
#include "tdsenc_chintlv_rif.h"
#endif /*FEATURE_THOR_MODEM*/

#ifdef FEATURE_TDSCDMA_HSUPA
#ifdef FEATURE_BOLT_MODEM
#include "tdsenc_common.h"
#include "tdsenc_commoni.h"
#endif /*FEATURE_BOLT_MODEM*/

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
#define TDSEULENC_PRE_RM_BITS_BMSK     0x01FFFF
//#define TDSEULENC_PRE_RM_BITS_BMSK     0/*HWIO_FMSK(TD_TX_EUL_CODING_PARAM_BANKc,NUM_PRE_RM_BITS)*/
//#define TDSEULENC_NUM_CODE_BLKS_BMSK   0x060000
//#define TDSEULENC_NUM_CODE_BLKS_BMSK   0/*HWIO_FMSK(TD_TX_EUL_CODING_PARAM_BANKc,NUM_CODE_SEGS)*/
#define TDSEULENC_NUM_CODE_BLKS_SHFT   17
//#define TDSEULENC_NUM_CODE_BLKS_SHFT   0/*HWIO_SHFT(TD_TX_EUL_CODING_PARAM_BANKc,NUM_CODE_SEGS) */

/* Nikel+TD HW interface TD_TX_EUL_CODE_SEG_SIZE_BANKc 
 * Bits     Name Description
 * 18:13    Num_filler_bits
 * 12:0     Num_bits_per_code_block 
  */
#define TDSEULENC_BITS_PER_BLK_BMSK    0x001FFF
#define TDSEULENC_BITS_PER_BLK_SHFT    13
#define TDSEULENC_NUM_FILLER_BITS_BMSK 0x3F

/*===========================================================================

                     ENC-LMEM REGISTERS Programming MICRO

===========================================================================*/
#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSEULENCI_WRITE_ENC_SS_EN(enable)\
{\
  uint32 val = (enable<<HWIO_ENC_SS_EN_ENC_SS_EN_SHFT)&HWIO_ENC_SS_EN_ENC_SS_EN_BMSK; \
  HWIO_ENC_SS_EN_OUT(val);\
  TDSL1_MSG1(HIGH, "ENC_SS_EN: val = %x", val);\
}
#else
#define TDSEULENCI_WRITE_ENC_SS_EN(enable)\
{\
  HWIO_ENC_SS_EN_OUT(enable);\
}
#endif /*FEATURE_TDSCDMA_PLT||TEST_FRAMEWORK*/

#if defined(FEATURE_TDSCDMA_PLT) || defined(TEST_FRAMEWORK)
#define TDSEULENCI_WRITE_IU_CLK_CTL(enable)\
{\
  uint32 val = (enable<<HWIO_IU_CLK_CTL_IU_CLK_EN_SHFT)&HWIO_IU_CLK_CTL_IU_CLK_EN_BMSK;\
  HWIO_IU_CLK_CTL_OUT(val);\
  TDSL1_MSG1(HIGH, "ENC_SS_EN: val = %x", val);\
}
#else
#define TDSEULENCI_WRITE_IU_CLK_CTL(enable)\
{\
  uint32 val = (enable<<HWIO_IU_CLK_CTL_IU_CLK_EN_SHFT)&HWIO_IU_CLK_CTL_IU_CLK_EN_BMSK;\
  HWIO_IU_CLK_CTL_OUT(val);\
}
#endif /*FEATURE_TDSCDMA_PLT||TEST_FRAMEWORK*/

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/*definitions for 2nd interlv/physical channel mapping caculations on Triton*/
typedef struct
{
    int16 row;
    int16 column;
}tdseulenc_2nd_intlv_matrix_position_type;

typedef struct
{
    tdseulenc_2nd_intlv_matrix_position_type start;
    tdseulenc_2nd_intlv_matrix_position_type last;
}tdseulenc_2nd_intlv_position_type;

typedef struct
{
  tdseulenc_2nd_intlv_matrix_position_type Last_bit_pos;
  tdseulenc_2nd_intlv_position_type slot[TDSL1_UL_MAX_NUM_TS];
}tdseulenc_2nd_intrlv_params_type;


#endif /* FEATURE_TDSCDMA_HSUPA */

#endif /* TDSEULENCI_H */

