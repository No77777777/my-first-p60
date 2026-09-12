#ifndef MCALTDS_RM_H
#define MCALTDS_RM_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         L 1  R A T E  M A T C H I N G  

GENERAL DESCRIPTION
  This module contains the MCAL adaptable structure and variable statement
  of rate matching

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2011 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/21/10   cdf     file create.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1def.h"
#include "tdsl1const.h"
#ifdef FEATURE_MCAL_TDSCDMA
#include "mcaltds_deci.h"
#include "mcaltds_dec.h"
#else
/*yongh*/
//#include "tdsl1dlcctrchcfg.h"
#endif


/* Defines whether for uplink or downlink */
#define TDSRM_UPLINK      0
#define TDSRM_DOWNLINK    1
#define TDSRM_LINK_DRCT   2

typedef struct {
  /* The number of Ndata of the selected physical channels */
  uint32                                ndata_value;

  /* Number of used physical channels */
  uint8                                 phyNumUsed;

  /* Used SF of the last used physical channnel */
  tdsl1_sf_enum_type                    last_chan_sf;
} mcaltds_rm_ndata_set_struct_type;


typedef struct {
  /* Transport channel Id from RRC for tracing purpose. Value: 0~31 */
  uint8                       trch_id;

  /* rate matching type used */
  tdsl1_rm_type_enum_type     rm_type;

  /* Error variable eIni1, defined as array to support 80ms TTI,
   * which one to use dependents on radio frame index in a TTI */
  uint16                      eini1[8];

  /* Error variable eIni2, defined as array to support 80ms TTI,
   * which one to use dependents on radio frame index in a TTI */
  uint16                      eini2[8];

  /* Error variable eMinus1 */
  uint16                      eminus1;

  /* Error variable eMinus2 */
  uint16                      eminus2;

  /* eminus divided by a */
  uint16                      eminusOverA;

  /* Error variable ePlus1 */
  uint16                      eplus1;

  /* Error variable ePlus2 */
  uint16                      eplus2;

  /* eplus divided by a */
  uint16                      eplusOverA;

  /* Rate matched size in bit: deltaNij+Nij */            
  uint16                      rateMatchedSize; 

  /* DeltaNij is Odd number or not */
  boolean                     deltaNOdd;

  /* tf pool index, to indicate the index of TF pool for this TF
   * 0xFF for invalid value 
   */
  uint8                       tfi;
} mcaltds_rm_trch_rm_struct_type;

typedef struct {
  /* the TFC is valid or not */
  boolean                           valid;

  /* An element of Ndata set which is the min SET1.
   * See 25.222: 4.2.7.1 for the algorithm of determining SET1
   */
  mcaltds_rm_ndata_set_struct_type  ndata_element;

  /* Sum over all TrCHs of RMx * Nxj, used in Zij calculations 25.222/4.2.7 */
  uint32                            sum_RMx_Nxj;

  /* The rate matching parameters for each trch */
  mcaltds_rm_trch_rm_struct_type    rm[TDSL1_MAX_NUM_TRCH];
} mcaltds_rm_tfc_pool_struct_type;


/* Trubo Interleaving parameters */
typedef struct {
  /* C - 1, where C is the number of columns */
  uint8                       num_columns_minus1;

  /* last column in the last row */
  uint8                       last_column;

  /* the number of rows in the interleaver table */
  uint8                       num_row_index;

  /* last row bit exchange condition */
  boolean                     last_row_bit_exch;

  /* index of last row with valid data */
  uint8                       last_row;

  /* index of prime number p used in the index table */
  uint8                       prime_num_index;

  /* prime number p */
  uint16                      prime_number;
} mcaltds_rm_trch_turbo_ileav_parms_type;


typedef struct {
  /* Transport channel Id from RRC for tracing purpose. Value: 0~31 */
  uint8                       trch_id;

  /* Transport block size in bit */
  uint16                      trblk_size;

  /* Number of transport blocks per TTI */
  uint16                      num_trblk;

  /* CRC size, in bit */
  uint8                       crc_size;

  /* RMx * Nij, useful in RM calculations */
  uint32                      RMx_Nxj;

  /* Number of code segments */
  uint8                       num_code_segs;

  /* Number of bits per code segment */
  uint32                      num_bits_per_code_seg;

  /* Stores the number of coded bits per TTI */
  uint32                      num_coded_bits_per_tti;

  /* Rate matched size per radio frame, also known as Nij */
  uint32                      pre_rm_bits;

  /* Number of filler bits to be used in the first code segment. */
  uint32                      num_filler_bits;

  /* Bit stream length after transport block concatenation */
  uint32                      len_tb_concat;

  /* Turbo Interleaving parameters. Valid only if coding type is Trubo coding */
  mcaltds_rm_trch_turbo_ileav_parms_type   turbo_ileav_parms;
} mcaltds_rm_tf_pool_struct_type;


typedef struct {
  /* the number of available TFC in tdsrm_tfc_pool */
  uint16                      num_tfc;

  /* the number of available TF in tdsrm_tf_pool */
  uint8                       num_tf;

  /* the number of transport channels */
  uint8                       num_trch;
} mcaltds_rm_param_stats_struct_type;

/* the calculated rate matching parameters which are based on TFC */
/*extern mcaltds_rm_tfc_pool_struct_type  tdsrm_tfc_pool[TDSRM_LINK_DRCT][TDSUE_MAX_TFC];*/

/* the calculated rate matching parameters which are based on TF */
/*extern mcaltds_rm_tf_pool_struct_type  tdsrm_tf_pool[TDSRM_LINK_DRCT][TDSUE_MAX_TF];*/

extern mcaltds_rm_tfc_pool_struct_type  *tdsrm_tfc_pool[TDSRM_LINK_DRCT];
extern mcaltds_rm_tf_pool_struct_type  *tdsrm_tf_pool[TDSRM_LINK_DRCT];

/* To store some useful statistics for MCAL use */
extern mcaltds_rm_param_stats_struct_type tdsrm_param_stats;



/*===========================================================================
FUNCTION mcaltds_rm_get_dl_num_coded_bits_tti

DESCRIPTION
  This function calculates the number of coded bits per TTI for DL. Each code 
  block in the TTI is coded as per the coding type. The number of coded bits 
  is always 0 if the number of number of TB is 0. The code block size is not
  essentially 0 when TB block size is 0 (The Code Block size is min 40 for
  TC). This function assumes that number of TB is not 0. The formula to get
  the coded bits for a code block is:
    Uncoded concatenated bits with crc = K
    Coded bits = Y
    For No Coding :                Y = K
    For Conv 1/2 code Coded bits : Y = 2K + 16
    For Conv 1/3 code Coded bits : Y = 3K + 24
    For Turbo Coded bits (always 1/3) : Y = 3K + 12

DEPENDENCIES
  None

RETURN VALUE
  Number of coded bits in a TTI (or in all code blocks)

SIDE EFFECTS
  None
===========================================================================*/
extern uint32 mcaltds_rm_get_dl_num_coded_bits_tti
(
  /* The code block info */
  mcaltds_dec_code_blk_param_struct_type *code_blk_info,
  /* coding type of the TrCh */
  mcaltds_dec_coding_enum_type trch_coding_type
);

#endif
