#ifndef TDSL1M_FTM_H
#define TDSL1M_FTM_H

/*============================================================================
          N O N   S I G N A L L I N G   F T M   H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for supporting
non-signalling ftm code.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2013 Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/19/15    sk      Fixed DSM leak on TX disbled case
07/17/14    sk      Fixed compile warnings
08/30/13    lub     file created.
                    
===========================================================================*/

#ifdef FEATURE_TDSCDMA_FTM_NS_NEW
/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "tdsl1_ftm_msg.h"
#include "tdsl1rrcif.h"
#include "tdsrrccmd.h"
#include "tdsrrccmd_v.h"
#include "tdsl1std.h"
#include "tdsl1msgr.h"
#include "tdseulsg.h"
#include "tdsl1rrcif.h"
#include "tds_l1_api.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define TDSL1_FTM_RMC_TYPE_12_2_TB_SIZE    244

#define TDSL1_FTM_BER_UL_MAC_MAX_HDR_SIZE  6
#define TDSL1_FTM_BER_UL_MAX_NUM_TB        12
#define TDSL1_FTM_BER_UL_MAX_TRCH          4

typedef struct
{
  tdsl1_ftm_ns_rmc_setup_type rmc_setup;
} tdsl1_ftm_ns_global_type;

typedef struct
{  
  tdsl1dlcctrchcfg_cctrch_info_struct_type    *dl_cctrch_info;
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type    *dl_ctfc_info;
  tdsl1_dl_phycfg_phychan_db_struct_type      *dl_phychan_db;   
    
  uint16                           current_rf_chan;
  boolean                          current_rf_chan_active;
  boolean                          isInitialized;
} tdsl1_ftm_ns_dl_db_type;


typedef struct
{
  tdsl1_ul_cctrch_cfg_struct_type  *ul_cctrch_info;
  tdsl1_ul_phych_cfg_struct_type   *ul_phychan_db;

  tdsmac_ul_tfcs_type              *tfcs_ptr;
  boolean                           isInitialized;
} tdsl1_ftm_ns_ul_db_type;

typedef struct {
  /* pointer to transport block data */
  dsm_item_type *tb_data;
  /* MAC header data */
  uint8          mac_hdr[TDSL1_FTM_BER_UL_MAC_MAX_HDR_SIZE];

  /* size of MAC header in bits (0..39)*/
  uint8          mac_hdr_size;
    /* ciphering flag for this block */
  boolean        ciphering_on;
  /* TM ciphering state */
  tdsl1_tb_tm_cipher_state_enum_type tm_cipher_state;
    /* radio bearer id */
  uint8          radio_bearer_id;
    /* ciphering key */
  uint8          ciphering_key_id;
    /* combination of hyper frame num and RLC seq num */
  uint32         count;
    /* offset in transport block from where ciphering should start */
  uint8          offset;
 /* Ciphering Algorithm: Kasumi or Snow3G*/
  uint32 cipher_algo;

} tdsl1_ftm_ns_ul_tb_data_type;

/* transport channel data */
typedef struct {
  tdsl1_tti_enum_type tti;
    /* number of transport blocks - also indicates no TrCH if zero */
  uint8              num_tb;
    /* transport block size in bits (0..5120) */
  uint16             tb_size;
    /* transport channel data */
  tdsl1_ftm_ns_ul_tb_data_type trch_data[TDSL1_FTM_BER_UL_MAX_NUM_TB];
} tdsl1_ftm_ns_ul_trch_data_type;

/* Tx data (for a 10ms frame) + control information */
typedef struct {
    /* number of transport channels */
  uint8                num_trch;
    /* flag to indicate if MAC is busy writing this buffer */
  boolean              mac_busy;
    /* TFCI used for this 10ms frame of Tx data */
  uint16               TFCI;
    /* data of all transport channels in the 10ms frame */
  tdsl1_ftm_ns_ul_trch_data_type tx_data[TDSL1_FTM_BER_UL_MAX_TRCH];
    /* ASC index valid for PRACH only */
  uint8                asc_num;
} tdsl1_ftm_ns_ul_tx_data_type;


typedef enum
{
    TDSL1_FTM_HSUPA_CHAN_STATE_INACTIVE,
    TDSL1_FTM_HSUPA_CHAN_STATE_ACTIVE
} tdsl1_ftm_hsupa_chan_state_type;

typedef struct
{
  tdseulsg_mod_type_enum_type epuch_mod_type;
  int16 epuch_tx_pwr;
  uint8 num_e_ucch;
  uint8 eagch_crri;
  uint8 eagch_trri;
  uint8 eagch_ecsn;
} tdsl1_ftm_hsupa_config_struct_type;

typedef struct
{
    tdsl1_ftm_hsupa_chan_state_type    current_state;
    tdsl1_ftm_hsupa_config_struct_type eagch_epuch_cfg;
    tdsl1_e_info_struct_type           hsupa_edch_cfg;
} tdsl1_ftm_ns_hsupa_db_type;


extern tdsl1_ftm_ns_ul_tx_data_type   tdsl1_ftm_ns_ul_tx_data;
extern tdsl1_ftm_ns_hsupa_db_type     tdsl1_ftm_hsupa_db;
/*===========================================================================

                   FUNTION DEFINITIONS

===========================================================================*/
void tdsl1_ftm_ns_send_cphy_setup_req(tdsl1_ext_cmd_type *ext_cmd_ptr, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_send_hsupa_cphy_setup_req(tdsl1_ext_cmd_type *ext_cmd_ptr, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_init_cphy_setup(tdsl1_setup_cmd_type *cmd, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_set_dl_cphy_setup(tdsl1_setup_cmd_type *cmd, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_set_ul_cphy_setup(tdsl1_setup_cmd_type *cmd, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_set_dl_cphy_setup_8820C_UPA(tdsl1_setup_cmd_type *cmd, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_ns_set_ul_cphy_setup_8820C_UPA(tdsl1_setup_cmd_type *cmd, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);

void tdsl1_ftm_set_dl_cctrch_db_information(tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_dl_phychan_db_information(tdsl1_dl_phychan_enum_type phych_type, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_ul_cctrch_db_information(tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_ul_phychan_db_information(tdsl1_ul_phychan_enum_type phych_type, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);

void tdsl1_ftm_set_dl_cctrch_db_information_8820C_UPA(tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_dl_phychan_db_information_8820C_UPA(tdsl1_dl_phychan_enum_type phych_type, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_ul_cctrch_db_information_8820C_UPA(tdsl1_api_ftm_ns_cphy_setup_req_t *msg);
void tdsl1_ftm_set_ul_phychan_db_information_8820C_UPA(tdsl1_ul_phychan_enum_type phych_type, tdsl1_api_ftm_ns_cphy_setup_req_t *msg);

void tdsl1_ftm_ns_allocate_dl_db(void);
void tdsl1_ftm_ns_allocate_ul_db(void);

extern void tdsl1_ftm_ns_trblk_data_ind (tdsl1_dl_tb_data_struct_type *data_ptr,uint32 crc_status [][8]);
void tdsl1_ftm_ns_set_ul_tx_data (tdsl1_dl_tb_data_struct_type *dl_data_ptr);
extern void tdsl1_ftm_ns_ul_build_frame_ind(tdsl1_tti_enum_type tti, boolean ul_tx_disabled, tdsl1_ul_tx_data_type *tx_buf_ptr);
uint8 tdsl1_ftm_ns_get_max_tti_frm_num (tdsl1_ftm_ns_ul_tx_data_type *mac_data_ptr);

extern tdsl1_eul_edch_ctrl_info_struct_type * tdsl1_ftm_hsupa_tdsmac_eul_etfci_selection(tdsl1_eul_sg_upd_info_struct_type * ftm_sg_info);
extern void tdsl1_ftm_hsupa_eul_phy_edch_build_frame_ind(tdseul_l1_build_frame_struct_type * ftm_build_frame_info);
extern void tdsl1_ftm_hsupa_sim_eagch(tdseulsg_agch_results_struct_type *ftm_eagch_info_ptr, uint16 ftm_curr_sub_fn);
extern void tdsl1_ftm_hsupa_sim_schld_ehich(tdseulsg_schld_ehich_result_struct_type *ftm_schld_ehich_ptr, uint16 ftm_curr_sub_fn);
int16 tdsl1_ftm_hsupa_get_epuch_tx_pwr(void);
void tdsl1_ftm_ns_init_ul_tx_data (void);
void tdsl1_ftm_ns_ul_clean_tx_buf(void);

#endif  /*FEATURE_TDSCDMA_FTM_NS_NEW*/
#endif  /*TDSL1M_FTM_H*/
