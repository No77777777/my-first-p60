#ifndef TDSEULENC_H
#define TDSEULENC_H

/*==========================================================================
             TDSCDMA L1 HSUPA UL ENC driver related header file

DESCRIPTION
  This file contains definition, declaration required for HSUPA UL ENC driver
  
  Copyright (c) 2011 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/hal/jolokia/tdseulenc.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/06/13   yuw           Supported Bolt
02/08/11   weijunz      inital edit for TDSCDMA
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
#include "tdsenci.h"
#include "tdseulcfg.h"

#include "tdseulcfg.h"
#include "tdsl1rrcif.h"
#include "tdsl1macif.h"

/* ---------------------------- */
/* Check for necessary features */
/* ---------------------------- */


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSHSUPA_MAX_NUM_ETFCI_PER_TAB   64
#define TDSHSUPA_MAX_NUM_TS_FOR_UE_CAT1AND2   3
#define TDSHSUPA_MAX_NUM_TS_FOR_UE_CAT3TO6   5
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* parameters for HW, HW interface document for EUL coding */
typedef struct
{
  /* num_code_blocks at bits 17~18 and pre RM bits or Ne,j at bits 0~16.
     refer to Register TD_TX_EUL_CODING_PARAM_BANKc  or
              TX_UNIFIED_ENC_EUL_CODING_PARAM_BANKc   */

  uint32  blocks_pre_rm_bits;

  /* num_filler_bits at bits 13~18 and bits_per_block at bits 0~12
     refer to register TD_TX_EUL_CODE_SEG_SIZE_BANKc or
             TX_UNIFIED_ENC_EUL_CODE_SEG_SIZE_BANKc */
  uint32  filler_block_bits;

} tdseul_code_seg_param_struct_type;

/* EUL Turbo Interleaving parameters */
typedef struct {
  /**** TD_TX_EUL_TURBO_INTLV_PARAM0_BANKc  or
               TX_UNIFIED_ENC_EUL_TURBO_INTLV_PARAM0_BANKc
               parameters ****/
  /* C - 1, where C is the number of columns in the interleaver */
  uint8  num_columns_minus1;
  /* last column in the last row that contains valid data*/
  uint8  last_column;
  /* number  of rows in the interleaver table . 00: 20 rows PA. 01: 20 rows PB. 10: 10 rows PC. 11: 5 rows PD */
  uint8  num_row_index;
  /* one bit indicates  the condition of (K ==R*C) & (C == P+1)*/
  uint8  last_row_bit_exch;
  /* index of last row with valid data */
  uint8  last_row;

  /**** TD_TX_EUL_TURBO_INTLV_PARAM1_BANKc or
               TX_UNIFIED_ENC_EUL_TURBO_INTLV_PARAM1_BANKc
               parameters ****/
  /* index of prime number p used in the index table */
  uint8  prime_num_index;
  /* prime number p */
  uint16 prime_number;
} tdseulenc_turbo_intlv_parms_type;

/*EUL HARQ and Rate matching parameter type*/
typedef struct
{
    /*Set 1 for puncturing and 0 for repetition*/
    uint8 punc_or_rep ;
    /*SYS RM initial error*/
    uint16 sys_eini ;
    /*SYS RM eplu*/
    uint16 sys_eplus;
    /*SYS RM eminus*/
    uint16 sys_eminus;
    /*P1 RM initial error*/
    uint16 p1_eini;
    /*P1 RM eplus*/
    uint16 p1_eplus;
    /*P1 RM eminus*/
    uint16 p1_eminus;
    /*P2 RM initial error*/
    uint16 p2_eini;
    /*P2 RM eplus*/
    uint16 p2_eplus;
    /*P2 RM eminus*/
    uint16 p2_eminus;
    /*Number of system bits after RM*/
    uint16 nt_sys ;
    /*Number of Parity 1 bits after RM */
    uint16 nt_p1 ;
    /*Number of parity 2 bits after RM*/
    uint16 nt_p2 ;
    /*Number of bits post RM for E-PUCH channel . Ne,data,j*/
    uint16 n_edata_j ;

} tdseulenc_rm_parms_type ;

/*Enum type for E-DCH modulation type*/
typedef enum
{
    TDSEUL_MODU_QPSK,
    TDSEUL_MODU_16QAM
} tdseul_modu_enum_type ;

/*param list for EUL HW config*/
typedef struct
{
    /*boolean new transmission or retransmission*/
    boolean new_trans ;
    /*EUL_ERAM bank index. 0...7*/
    uint8  eram_bank_idx;
    /*UE category. Set to TRUE when use UE CAT 1and2 E-TFCI table,FLASE when use UE Cat 3 to 6 table */
    boolean ue_cat_1and2 ;
    /*selected E-TFCI*/
    uint8  etfci;
    /*number of E-PUCH timeslot */
    uint8  num_ts;
    /*number of bits for E-PUCH channel */
    uint16 n_edata_j;
    /*RSN */
    uint8  rsn;
    /*selected E-PUCH modulation type*/
    tdseul_modu_enum_type modu_type ;
    /* brief HARQ Process ID. Valid [0..3] */
    uint8 harq_id;
    /* E-UCCH instance number, 1 ~ 8 */
    uint8 num_eucch;
    /* E-PUCH SF index, 0:SF1; 1:SF2; 2:SF4; 3:SF8; 4:SF16  */
    uint8 epuch_sf_idx;
    /* Timeslot number,  value starts from 1 */
    uint8 time_slot[TDSL1_UL_MAX_NUM_TS];
    /* symbols per timeslot */
    uint32 symbol_slot[TDSL1_UL_MAX_NUM_TS];
} tdseulenc_hw_cfg_parms_type ;





/* ==========================================================================
** Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern uint32 tdsedpch_l1_p2_ul_ctrl_wrd[8*TDSEULENC_MAX_NUM_PDU_BLD ];
extern uint32 tdsedpch_l1_p2_ul_ctrl_wrd_idx[8] ;

extern uint32 tdseulenc_debug_run_eul_pp_emulator;

extern uint32 hsupa_tx_reg[50] ;

#ifdef FEATURE_TDSCDMA_L1M_PROFILING_LOG_PACKET
extern uint32 tdseulenc_a2_done_cb_univ_stmr;
extern uint16 tdseulenc_a2_done_cb_sub_frame;
extern boolean tdseulenc_a2_done_cb_flag;
#endif

#define L1_CIPH_BUFFER_IDX_INIT_VAL  0xFFFFFFFF
#define L1_COPY_ENG_BUFFER_IDX_INIT_VAL  0xFFFFFFFF


/* =======================================================================
**                        Function Declarations
** ======================================================================= */


/*===========================================================================
FUNCTION  TDSEULENC_GET_CODE_SEG_PARAM_STRUCT_FOR_ETFCI

DESCRIPTION
  This function returns a pointer to pre RM params struct for an ETFCI.

DEPENDENCIES
  Input parameters must be validated already.

RETURN VALUE
  const tdseul_pre_rm_param_struct_type*

SIDE EFFECTS
  None
===========================================================================*/
extern const tdseul_code_seg_param_struct_type* tdseulenc_get_code_seg_param_struct_for_etfci
(
  boolean ue_cat1and2,
  uint8 num_ts, /* up to 3 for UE cat 1and 2 . up to 5 for UE cat 3 to 6*/
  uint8 etfci  /* 0 ~ 63 */
);

void tdseulenc_calc_turbo_interleaving_parms(uint32 num_bits_per_code_seg) ;
void tdseulenc_calc_rm_parms(uint16 n_e_j,uint16 n_edata_j,uint8 rsn,tdseul_modu_enum_type modu_type);



/*===========================================================================
FUNCTION  TDSEULENC_SET_EUL_HW

DESCRIPTION
  This function calculate parameter for EUL TX register and set register value
  for next TTI E-DCH.

DEPENDENCIES
  HW interafce

RETURN VALUE
  None 

SIDE EFFECTS
  HW ready for next TTI EUL data
===========================================================================*/
void tdseulenc_set_eul_hw(tdseulenc_hw_cfg_parms_type *hw_cfg_ptr);

/*===========================================================================
FUNCTION  TDSEULENC_START_EUL_ENCODING

DESCRIPTION
  This function selects EUL ERAM bank for encoding and triggers EUL HW encoding .

DEPENDENCIES
  HW interafce

RETURN VALUE
  None 

SIDE EFFECTS
  HW ready for next TTI EUL data
===========================================================================*/
void tdseulenc_start_eul_encoding(uint8 eram_bank_index);


//#ifdef FEATURE_WCDMA_UL_DATA_PATH_OPT
/*=========================================================
FUNCTION      L1_L2_BUILD_CIPHER_ENGINE_CTRL_INFO

DESCRIPTION   This function is called by RLC to create the 
              ciphering control words from the parameters

DEPENDENCIES  Pointer to buffer and word to write should be 
              initialized

RETURN VALUE  VOID

SIDE EFFECTS  Ciphering Control word will be written to 
              __ciphering_ctrl_buf
============================================================*/
void tdsl1_l2_build_cipher_engine_ctrl_info
(
  uint32 data_unit_len,                       /* bits 0~14 data_unit_len */
  uint32 radio_bearer_id,                     /* bits 15~19 radio_bearer_id */
  uecomdef_umts_cipher_algo_e_type cipher_algo,/* bit 20 algo Snow3G/Kasumi */
  uint32 cipher_key_index,                    /* bits 21~23 cipher_key_index */
  uint32 cipher_offset,                       /* bits 24~29 ciphering_offset */
  boolean last_unit_of_tti,                   /* bit 31 indicates last_unit  */
  boolean cipher_enabled,                     /* bit 30 indicates ciph enabled */
  uint32 cipher_count_c                       /* CountC present if ciph enabled,else -1) */
);

/*=========================================================
FUNCTION      EULENC_RESET_BUILD_FRAME_IFC_CONTENTS

DESCRIPTION   Reset all the interface contents between L1 and L2
              to initial values

DEPENDENCIES  NONE

RETURN VALUE  VOID

SIDE EFFECTS  NONE
============================================================*/
void tdseulenc_reset_build_frame_ifc_contents(uint8 harq_id);

/*=========================================================
FUNCTION      EULENC_VALIDATE_BUILD_FRAME_PARAMETERS

DESCRIPTION   Validate all the interface contents passed from 
              L2 to build the TBS under consideration

DEPENDENCIES  NONE

RETURN VALUE  VOID

SIDE EFFECTS  NONE
============================================================*/
void tdseulenc_validate_build_frame_parameters(void);


/*=========================================================
FUNCTION      EULENC_A2_UL_INIT

DESCRIPTION   

DEPENDENCIES  VOID

RETURN VALUE  VOID

SIDE EFFECTS  VOID
============================================================*/
void tdseulenc_a2_ul_init(void);

void tdseulenc_a2_ul_pwr_down(void) ;

/*=========================================================
FUNCTION      EULENC_BUILD_A2_TASKS

DESCRIPTION   This function is called after data is formed by L2 and 
              given to L1, in build frame indication.It has CTRL Info
              and CIPH words

DEPENDENCIES  RLC should have defined and prepared the data and header info
              blocks, as well as PDU-info.

RETURN VALUE  VOID

SIDE EFFECTS  NONE
============================================================*/
void tdseulenc_build_a2_tasks(uint8 harq_id);

/*=========================================================================

FUNCTION tdseulenc_cmd_done_handler

DESCRIPTION
  This function is a handler of the cmd done interrupt of uK

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

=========================================================================*/
void tdseulenc_cmd_done_handler(void);

//#endif /* FEATURE_WCDMA_UL_DATA_PATH_OPT */

#endif /* FEATURE_TDSCDMA_HSUPA */

#endif /* TDSEULENC_H */

