#ifndef TDS_RM_H
#define TDS_RM_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         L 1  R A T E  M A T C H I N G  

GENERAL DESCRIPTION
  This module contains the implementation of rate matching

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
11/18/10   cdf     file create.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "queue.h"
#include "tdsl1sapcommon.h"
#include "tdsulra.h"
#include "tdsl1ulcfg.h"
#include "tdsulutil.h"
#include "mcaltds_dl.h"
/*yongh*/
//#include "tdsl1dlcctrchcfg.h"
#include "tdsl1dlphychancfg.h"
#include "tfw_sw_intf_msg.h"
#include "mcaltds_rm.h"

/*===========================================================================

                             MACRO and CONST

===========================================================================*/
/* The maximum number of Ndata set for downlink
 *   Maximum number of physical channels per timeslot: 16
 *   Maximum number of timeslots                     : 6
 *   Possible number of SF of a physical channel     : 1
 *   So the number of Ndata set is 16 * 6 * 1 = 96
 * The maximum number of Ndata set for uplink
 *   Maximum number of physical channels per timeslot: 2
 *   Maximum number of timeslots                     : 5
 *   Possible number of SF of a physical channnel    : 4 
 *   So the number of Ndata set is 2 * 5 * 4 = 40
 * The larger one is 96
 */
#define TDSL1_MAX_NDATA_SET_NUM                 96


/* For RACH, puncturing is allowed if need, but PL is not specified by NW.
 * Since only CC_1_2 coding type is allowed for RACH, PL should not be 
 * less than 0.5.
 * Here set PL to 0.5 to make sure puncturing no less than 0.5 can go on 
 * smoothly.
 */
#define TDSL1_RACH_PUNCT_LIMIT                  50

/*===========================================================================

                                Structure

===========================================================================*/

typedef enum
{
  TDSRM_UL_DPCH = 0,
  TDSRM_PRACH = 1,
  TDSRM_DL_DPCH = 2,
  TDSRM_SCCPCH =3
} tdsrm_phychan_type_enum_type;

typedef struct {
  /* Whether the physical channel is valid */
  boolean                               valid;

  /* Uplink or downlink timeslot */
  uint8                                 link_drct;

  /* The number of TFCI bits */
  uint8                                 tfci_bits;

  /* The minimum spreading factor of the code channel 
   * signalled by higher layer 
   */
  tdsl1_sf_enum_type                    min_sf;

  /* The maximum spreading factor of the code channel.
   * It's the same as min_sf in case of staticSF, and it's the maximum
   * possible spreading factor in case of dynamicSF 
   */
  tdsl1_sf_enum_type                    max_sf;

  /* Channelisation code (1..16) */
  uint8                                 chan_code;

  /* Amount of SS and TPC bits sent in this timeslot */
  tdsl1_ss_tpc_symb_enum_type           ss_tpc_symbols;

  /* DPCH midamble shift and burst */
  tdsl1_midamble_ss_burst_struct_type   midamble_and_burst;
} tdsrm_phychan_info_struct_type;


typedef mcaltds_rm_ndata_set_struct_type tdsrm_ndata_set_struct_type;


typedef struct {
  /* The number of transport blocks. */
  uint16                                tb_block_num;

  /* Transport block size. */
  uint16                                tb_size;
} tdsrm_tf_info_struct_type;


typedef struct {
  /* Transport channel Id from RRC for tracing purpose. Value: 0~31 */
  uint8                                 trch_id;

  /* Transmission time interval */
  tdsl1_tti_enum_type                   tti;

  /* Channel coding type and rate */
  tdsl1_coding_enum_type                coding;

  /* Static Rate Matching attribute */
  uint16                                staticRM;

  /* CRC size, in bit */
  tdsl1_crc_length_enum_type            crc_size;

  /* tf pool base index */
  uint8                                 tf_base;

  /* The number of transport format */
  uint16                                num_tf;

  /* Each known Transport Format */
  tdsrm_tf_info_struct_type             tf_info[TDSUE_MAX_TF];
} tdsrm_trch_info_struct_type;


typedef struct {
  /* The number of known TFCs */
  uint16                                num_tfc;

  /* The number of TrCHs */
  uint8                                 num_trch;

  /* TFI info for each known TFC info. */
  uint8                                 tfi[TDSUE_MAX_TFC][TDSL1_MAX_NUM_TRCH]; 

  /* TFS info for each known TrCH. */
  tdsrm_trch_info_struct_type           trch_info[TDSL1_MAX_NUM_TRCH];
} tdsrm_tfc_and_trch_info_struct_type;


typedef mcaltds_rm_trch_turbo_ileav_parms_type tdsrm_trch_turbo_ileav_parms_type;

typedef mcaltds_rm_trch_rm_struct_type tdsrm_trch_rm_struct_type;

typedef mcaltds_rm_tfc_pool_struct_type tdsrm_tfc_pool_struct_type;

typedef mcaltds_rm_tf_pool_struct_type tdsrm_tf_pool_struct_type;

#if defined(FEATURE_TDSCDMA_JOLOKIA)|| defined(FEATURE_BOLT_MODEM)
/*UL Timeslot confg type for the input parameters of 2nd interlv on Triton*/
typedef struct
{
    /*number of phy channels in a slot, max value is 2*/
    uint8 num_phy_chan;
    /*number of bits in ch0 within a subframe, ch0 is with the lower physical sequence number in the slot*/
    uint32 num_bits_in_ch0;         
    /*number of bits in ch1 within a subframe, ch1 is with  the higher physical sequence number in the slot*/
    uint32 num_bits_in_ch1;
    uint32 sf0;
    uint32 sf1;
    #ifdef FEATURE_BOLT_MODEM
    tdsl1_sf_enum_type sf0_enum;
    tdsl1_sf_enum_type sf1_enum;
    #endif /*FEATURE_BOLT_MODEM*/
    /*nata of the slot in subframe (num_bits_in_ch0+num_bits_in_ch1)*/
    uint32 ndata_subframe;  
    /*nata of the slot in frame. For 5ms TTI, it is same as ndata_subframe, For other TTI, it is ndata_subframe*2*/
    uint32 ndata_frame;
}tdsrm_ul_ts_cfg_type;

void    tdsrm_get_ul_ts_cfg(uint16 tfci, tdsrm_ul_ts_cfg_type* ul_ts_cfg_ptr);
#endif /*FEATURE_TDSCDMA_JOLOKIA||FEATURE_BOLT_MODEM*/

#ifdef FEATURE_BOLT_MODEM
void tdsrm_get_symnum_data1_data2_per_slot(
  uint8 sf_val, 
  uint8 num_tfci, 
  tdsl1_ss_tpc_symb_enum_type ss_tpc_symb,
  uint16* symnum_data1,
  uint16* symnum_data2);
#endif


/*===========================================================================

                        Functions and Variables Statement

===========================================================================*/

/* Prime number list. See 25.222 V3.5.0 2000-12 Sec 4.2.3.2.3.1. */
extern const uint16 tdsrm_prime_num_list[TDSL1_TRUBO_INTLV_PRIME_LIST_SIZE];

extern boolean tdsrm_ul_dpch_rm_process
(
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg_ptr,
  tdsl1_ul_cctrch_cfg_struct_type *cctrch_ptr,
  tdsmac_ul_tfcs_type *tfcs_ptr
);

extern boolean tdsrm_ul_prach_rm_process
(
  tdsl1_ul_cctrch_cfg_struct_type *cctrch_ptr,
  tdsmac_ul_tfcs_type *tfcs_ptr,
  tdsulra_ctrl_struct_type *ra_ctrl_ptr
);

extern boolean tdsrm_dl_rm_process
(
  tdsl1_dl_phycfg_phychan_db_struct_type *phch_cfg_parms_ptr,
  tdsl1_dl_phychan_enum_type dl_phychan_type,
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type *cctrch_info_ptr,
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type *ctfc_info_ptr
);

extern void tdsrm_determine_ul_sb_lowest_non_zero_rate_tfci( void );

extern uint16 tdsrm_get_ul_sb_non_zero_rate_tfci( void );

extern boolean tdsrm_get_ul_dpch_phy_config
(
  tfw_ul_dpch_config_t *dpch_config_ptr,
  uint16 tfci,
  boolean is_sb,
  uint16 *sb_tfci/*output para valid only if is_sb is TRUE */
);

extern boolean tdsrm_get_ul_init_dpch_phy_config
(
   tfw_dpch_tx_pwr_timing_config_t *dpch_init_cfg_ptr
);

extern void tdsrm_get_ul_rm_params
(
  uint16 tfci,
  uint8 nrf /* next radio frame number */
);

extern boolean tdsrm_get_ul_ts_bit_num
(
  uint16 tfci,
  uint16 *ts_bit_num_ptr
);

extern uint32 tdsrm_get_L_value(uint16 tfci);

/*=========================================================================
FUNCTION tdsrm_current_tfc_if_exist_crc

DESCRIPTION
  This function is called to calculate if current TFC have CRC or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE: one of trches crc length is not "0"
  FALSE:all trch of this TFC the crc length is "0"

SIDE EFFECTS
=========================================================================*/
boolean tdsrm_current_tfc_if_exist_crc(uint8 tfc_idx);

/*#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_DSDS_QTA)*/
/*===========================================================================
FUNCTION        tdsrm_get_slot_num_per_tfc

DESCRIPTION     This function is used to 
                1. calculate the slot occupation for TFC according to TFCI
                after RM.

DEPENDENCIES    Depends on RM 

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
uint8  tdsrm_get_slot_occupation_num_per_tfc(uint8 tfci);
/*#endif*/ /*FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT || FEATURE_TDSCDMA_DMO_SUPPORT||FEATURE_TDSCDMA_DSDS_QTA*/

#endif /* TDS_RM_H */
