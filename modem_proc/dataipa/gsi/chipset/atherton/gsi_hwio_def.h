#ifndef __GSI_HWIO_DEF_H__
#define __GSI_HWIO_DEF_H__
/*
===========================================================================
*/
/**
  @file gsi_hwio.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    IPA_0_GSI_TOP_.*

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2021 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/dataipa.mpss/3.2/gsi/chipset/atherton/gsi_hwio_def.h#1 $
  $DateTime: 2021/06/01 07:09:14 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: GSI
 *--------------------------------------------------------------------------*/

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_CFG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_enable : 1;
  uint32 mcs_enable : 1;
  uint32 double_mcs_clk_freq : 1;
  uint32 uc_is_mcs : 1;
  uint32 gsi_pwr_clps : 1;
  uint32 bp_mtrix_disable : 1;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_cfg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_cfg_s def;
  uint32 value;
} gsi_hwio_def_gsi_cfg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MANAGER_MCS_CODE_VER 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ver : 32;
} gsi_hwio_def_gsi_manager_mcs_code_ver_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_manager_mcs_code_ver_s def;
  uint32 value;
} gsi_hwio_def_gsi_manager_mcs_code_ver_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_ZEROS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 zeros : 32;
} gsi_hwio_def_gsi_zeros_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_zeros_s def;
  uint32 value;
} gsi_hwio_def_gsi_zeros_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_PERIPH_BASE_ADDR_LSB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 base_addr : 32;
} gsi_hwio_def_gsi_periph_base_addr_lsb_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_periph_base_addr_lsb_s def;
  uint32 value;
} gsi_hwio_def_gsi_periph_base_addr_lsb_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_PERIPH_BASE_ADDR_MSB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 base_addr : 32;
} gsi_hwio_def_gsi_periph_base_addr_msb_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_periph_base_addr_msb_s def;
  uint32 value;
} gsi_hwio_def_gsi_periph_base_addr_msb_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_PERIPH_PENDING 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid_bit_map : 32;
} gsi_hwio_def_gsi_periph_pending_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_periph_pending_s def;
  uint32 value;
} gsi_hwio_def_gsi_periph_pending_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MOQA_CFG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 client_req_prio : 8;
  uint32 client_oord : 8;
  uint32 client_oowr : 8;
  uint32 reserved0 : 8;
} gsi_hwio_def_gsi_moqa_cfg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_moqa_cfg_s def;
  uint32 value;
} gsi_hwio_def_gsi_moqa_cfg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_REE_CFG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 move_to_esc_clr_mode_trsh : 1;
  uint32 reserved0 : 7;
  uint32 max_burst_size : 8;
  uint32 reserved1 : 16;
} gsi_hwio_def_gsi_ree_cfg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_ree_cfg_s def;
  uint32 value;
} gsi_hwio_def_gsi_ree_cfg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_WR_WRR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 client0_wr_weight : 4;
  uint32 client1_wr_weight : 4;
  uint32 client2_wr_weight : 4;
  uint32 client3_wr_weight : 4;
  uint32 client4_wr_weight : 4;
  uint32 client5_wr_weight : 4;
  uint32 client6_wr_weight : 4;
  uint32 client7_wr_weight : 4;
} gsi_hwio_def_gsi_shram_wr_wrr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_wr_wrr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_wr_wrr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_WR_WRR_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 client8_wr_weight : 4;
  uint32 client9_wr_weight : 4;
  uint32 reserved0 : 24;
} gsi_hwio_def_gsi_shram_wr_wrr_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_wr_wrr_1_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_wr_wrr_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_RD_WRR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_eng_shram_rd_weight : 4;
  uint32 mcs_ld_shram_rd_weight : 4;
  uint32 re_cntxt_shram_rd_weight : 4;
  uint32 csr_shram_rd_weight : 4;
  uint32 ie_shram_rd_weight : 4;
  uint32 ach_shram_rd_weight : 4;
  uint32 moqa_sdma_shram_rd_weight : 4;
  uint32 ch_cmdq_shram_rd_weight : 4;
} gsi_hwio_def_gsi_shram_rd_wrr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_rd_wrr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_rd_wrr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_RD_WRR_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ee_cmdq_shram_rd_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_gsi_shram_rd_wrr_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_rd_wrr_1_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_rd_wrr_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_CGC_CTRL 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 region_1_hw_cgc_en : 1;
  uint32 region_2_hw_cgc_en : 1;
  uint32 region_3_hw_cgc_en : 1;
  uint32 region_4_hw_cgc_en : 1;
  uint32 region_5_hw_cgc_en : 1;
  uint32 region_6_hw_cgc_en : 1;
  uint32 region_7_hw_cgc_en : 1;
  uint32 region_8_hw_cgc_en : 1;
  uint32 region_9_hw_cgc_en : 1;
  uint32 region_10_hw_cgc_en : 1;
  uint32 region_11_hw_cgc_en : 1;
  uint32 region_12_hw_cgc_en : 1;
  uint32 region_13_hw_cgc_en : 1;
  uint32 reserved0 : 19;
} gsi_hwio_def_gsi_cgc_ctrl_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_cgc_ctrl_s def;
  uint32 value;
} gsi_hwio_def_gsi_cgc_ctrl_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MSI_CACHEATTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ashared : 1;
  uint32 ainnershared : 1;
  uint32 anoallocate : 1;
  uint32 atransient : 1;
  uint32 areqpriority : 2;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_msi_cacheattr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_msi_cacheattr_s def;
  uint32 value;
} gsi_hwio_def_gsi_msi_cacheattr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_EVENT_CACHEATTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ashared : 1;
  uint32 ainnershared : 1;
  uint32 anoallocate : 1;
  uint32 atransient : 1;
  uint32 areqpriority : 2;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_event_cacheattr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_event_cacheattr_s def;
  uint32 value;
} gsi_hwio_def_gsi_event_cacheattr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DATA_CACHEATTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ashared : 1;
  uint32 ainnershared : 1;
  uint32 anoallocate : 1;
  uint32 atransient : 1;
  uint32 areqpriority : 2;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_data_cacheattr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_data_cacheattr_s def;
  uint32 value;
} gsi_hwio_def_gsi_data_cacheattr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_TRE_CACHEATTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ashared : 1;
  uint32 ainnershared : 1;
  uint32 anoallocate : 1;
  uint32 atransient : 1;
  uint32 areqpriority : 2;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_tre_cacheattr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_tre_cacheattr_s def;
  uint32 value;
} gsi_hwio_def_gsi_tre_cacheattr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_REE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 stop_ch_comp_int_weight : 4;
  uint32 new_re_int_weight : 4;
  uint32 ch_empty_int_weight : 4;
  uint32 reserved0 : 20;
} gsi_hwio_def_ic_int_weight_ree_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_ree_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_ree_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_EVT_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 evnt_eng_int_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_ic_int_weight_evt_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_evt_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_evt_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_INT_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 int_eng_int_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_ic_int_weight_int_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_int_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_int_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_CSR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ch_cmd_int_weight : 4;
  uint32 ee_generic_int_weight : 4;
  uint32 reserved0 : 24;
} gsi_hwio_def_ic_int_weight_csr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_csr_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_csr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_TLV_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 tlv_0_int_weight : 4;
  uint32 tlv_1_int_weight : 4;
  uint32 tlv_2_int_weight : 4;
  uint32 reserved0 : 20;
} gsi_hwio_def_ic_int_weight_tlv_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_tlv_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_tlv_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_TIMER_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 timer_int_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_ic_int_weight_timer_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_timer_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_timer_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_DB_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 new_db_int_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_ic_int_weight_db_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_db_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_db_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_RD_WR_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_int_weight : 4;
  uint32 write_int_weight : 4;
  uint32 reserved0 : 24;
} gsi_hwio_def_ic_int_weight_rd_wr_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_rd_wr_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_rd_wr_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IC_INT_WEIGHT_UCONTROLLER_ENG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ucontroller_gp_int_weight : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_ic_int_weight_ucontroller_eng_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ic_int_weight_ucontroller_eng_s def;
  uint32 value;
} gsi_hwio_def_ic_int_weight_ucontroller_eng_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MANAGER_EE_QOS_n 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ee_prio : 2;
  uint32 reserved0 : 6;
  uint32 max_ch_alloc : 5;
  uint32 reserved1 : 3;
  uint32 max_ev_alloc : 5;
  uint32 reserved2 : 11;
} gsi_hwio_def_gsi_manager_ee_qos_n_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_manager_ee_qos_n_s def;
  uint32 value;
} gsi_hwio_def_gsi_manager_ee_qos_n_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ch_cntxt_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ch_cntxt_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ch_cntxt_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ev_cntxt_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ev_cntxt_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ev_cntxt_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_re_storage_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_re_storage_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_re_storage_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_re_esc_buf_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_re_esc_buf_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_re_esc_buf_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ee_scrach_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ee_scrach_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ee_scrach_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_func_stack_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_func_stack_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_func_stack_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_mcs_scratch_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_MCS_SCRATCH1_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch1_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_mcs_scratch1_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch1_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_MCS_SCRATCH2_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch2_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_mcs_scratch2_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_mcs_scratch2_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_CH_VP_TRANS_TABLE_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ch_vp_trans_table_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ch_vp_trans_table_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ch_vp_trans_table_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_EV_VP_TRANS_TABLE_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ev_vp_trans_table_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ev_vp_trans_table_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ev_vp_trans_table_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_USER_INFO_DATA_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_user_info_data_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_user_info_data_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_user_info_data_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_EE_CMD_FIFO_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ee_cmd_fifo_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ee_cmd_fifo_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ee_cmd_fifo_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_PTR_CH_CMD_FIFO_BASE_ADDR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_shram_ptr_ch_cmd_fifo_base_addr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_ptr_ch_cmd_fifo_base_addr_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_ptr_ch_cmd_fifo_base_addr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_CH_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ch_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ch_cmd_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ch_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_EE_GENERIC_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ee_generic_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ee_generic_cmd_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ee_generic_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_CH_DB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ch_db_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ch_db_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ch_db_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_EV_DB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ev_db_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ev_db_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ev_db_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_NEW_RE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_new_re_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_new_re_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_new_re_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_CH_DIS_COMP 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ch_dis_comp_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ch_dis_comp_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ch_dis_comp_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_CH_EMPTY 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_ch_empty_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_ch_empty_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_ch_empty_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_EVENT_GEN_COMP 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_event_gen_comp_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_event_gen_comp_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_event_gen_comp_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_0_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_2_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_1_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_periph_if_tlv_in_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_TIMER_EXPIRED 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_timer_expired_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_timer_expired_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_timer_expired_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_WRITE_ENG_COMP 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_write_eng_comp_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_write_eng_comp_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_write_eng_comp_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_READ_ENG_COMP 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_read_eng_comp_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_read_eng_comp_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_read_eng_comp_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_UC_GP_INT 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_uc_gp_int_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_uc_gp_int_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_uc_gp_int_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_IRAM_PTR_INT_MOD_STOPED 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_iram_ptr_int_mod_stoped_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_iram_ptr_int_mod_stoped_s def;
  uint32 value;
} gsi_hwio_def_gsi_iram_ptr_int_mod_stoped_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_INST_RAM_n 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 inst_byte_0 : 8;
  uint32 inst_byte_1 : 8;
  uint32 inst_byte_2 : 8;
  uint32 inst_byte_3 : 8;
} gsi_hwio_def_gsi_inst_ram_n_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_inst_ram_n_s def;
  uint32 value;
} gsi_hwio_def_gsi_inst_ram_n_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SHRAM_n 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 shram : 32;
} gsi_hwio_def_gsi_shram_n_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_shram_n_s def;
  uint32 value;
} gsi_hwio_def_gsi_shram_n_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_TEST_BUS_SEL 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_testbus_sel : 8;
  uint32 reserved0 : 8;
  uint32 gsi_hw_events_sel : 4;
  uint32 reserved1 : 12;
} gsi_hwio_def_gsi_test_bus_sel_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_test_bus_sel_s def;
  uint32 value;
} gsi_hwio_def_gsi_test_bus_sel_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_TEST_BUS_REG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_testbus_reg : 32;
} gsi_hwio_def_gsi_test_bus_reg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_test_bus_reg_s def;
  uint32 value;
} gsi_hwio_def_gsi_test_bus_reg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_BUSY_REG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 csr_busy : 1;
  uint32 ree_busy : 1;
  uint32 mcs_busy : 1;
  uint32 timer_busy : 1;
  uint32 rd_wr_busy : 1;
  uint32 ev_eng_busy : 1;
  uint32 int_eng_busy : 1;
  uint32 ree_pwr_clps_busy : 1;
  uint32 db_eng_busy : 1;
  uint32 dbg_cnt_busy : 1;
  uint32 uc_busy : 1;
  uint32 ic_busy : 1;
  uint32 sdma_busy : 1;
  uint32 reserved0 : 19;
} gsi_hwio_def_gsi_debug_busy_reg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_busy_reg_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_busy_reg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_EVENT_PENDING 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid_bit_map : 32;
} gsi_hwio_def_gsi_debug_event_pending_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_event_pending_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_event_pending_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_TIMER_PENDING 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid_bit_map : 32;
} gsi_hwio_def_gsi_debug_timer_pending_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_timer_pending_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_timer_pending_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_RD_WR_PENDING 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid_bit_map : 32;
} gsi_hwio_def_gsi_debug_rd_wr_pending_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_rd_wr_pending_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_rd_wr_pending_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SPARE_REG_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 fix_ieob_wrong_msk_disable : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_gsi_spare_reg_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_spare_reg_1_s def;
  uint32 value;
} gsi_hwio_def_gsi_spare_reg_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_COUNTER_CFGn 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 enable : 1;
  uint32 stop_at_wrap_arnd : 1;
  uint32 clr_at_read : 1;
  uint32 evnt_type : 5;
  uint32 ee : 4;
  uint32 virtual_chnl : 8;
  uint32 chain : 1;
  uint32 reserved0 : 11;
} gsi_hwio_def_gsi_debug_counter_cfgn_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_counter_cfgn_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_counter_cfgn_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_COUNTERn 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 counter_value : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_debug_countern_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_countern_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_countern_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_PC_FROM_SW 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_debug_pc_from_sw_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_pc_from_sw_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_pc_from_sw_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_SW_STALL 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mcs_stall : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_gsi_debug_sw_stall_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_sw_stall_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_sw_stall_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_PC_FOR_DEBUG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 iram_ptr : 12;
  uint32 reserved0 : 20;
} gsi_hwio_def_gsi_debug_pc_for_debug_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_pc_for_debug_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_pc_for_debug_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_SEL 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 sel_write : 1;
  uint32 reserved0 : 7;
  uint32 sel_tid : 8;
  uint32 sel_mid : 8;
  uint32 reserved1 : 8;
} gsi_hwio_def_gsi_debug_qsb_log_sel_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_sel_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_sel_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 log_clr : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_gsi_debug_qsb_log_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_clr_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_ERR_TRNS_ID 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 err_write : 1;
  uint32 reserved0 : 7;
  uint32 err_tid : 8;
  uint32 err_mid : 8;
  uint32 err_saved : 1;
  uint32 reserved1 : 7;
} gsi_hwio_def_gsi_debug_qsb_log_err_trns_id_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_err_trns_id_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_err_trns_id_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_31_0 : 32;
} gsi_hwio_def_gsi_debug_qsb_log_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_0_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_43_32 : 12;
  uint32 ainnershared : 1;
  uint32 anoallocate : 1;
  uint32 ashared : 1;
  uint32 acacheable : 1;
  uint32 atransient : 1;
  uint32 aooord : 1;
  uint32 aooowr : 1;
  uint32 reserved0 : 1;
  uint32 alen : 4;
  uint32 asize : 4;
  uint32 areqpriority : 4;
} gsi_hwio_def_gsi_debug_qsb_log_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_1_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ammusid : 12;
  uint32 amemtype : 4;
  uint32 reserved0 : 16;
} gsi_hwio_def_gsi_debug_qsb_log_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_2_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_QSB_LOG_LAST_MISC_IDn 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_20_0 : 21;
  uint32 write : 1;
  uint32 tid : 5;
  uint32 mid : 5;
} gsi_hwio_def_gsi_debug_qsb_log_last_misc_idn_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_qsb_log_last_misc_idn_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_qsb_log_last_misc_idn_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_SW_RF_n_WRITE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 data_in : 32;
} gsi_hwio_def_gsi_debug_sw_rf_n_write_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_sw_rf_n_write_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_sw_rf_n_write_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_SW_RF_n_READ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rf_reg : 32;
} gsi_hwio_def_gsi_debug_sw_rf_n_read_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_sw_rf_n_read_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_sw_rf_n_read_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_EE_n_CH_k_VP_TABLE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 phy_ch : 5;
  uint32 valid : 1;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_debug_ee_n_ch_k_vp_table_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_ee_n_ch_k_vp_table_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_ee_n_ch_k_vp_table_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_DEBUG_EE_n_EV_k_VP_TABLE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 phy_ev_ch : 5;
  uint32 valid : 1;
  uint32 reserved0 : 26;
} gsi_hwio_def_gsi_debug_ee_n_ev_k_vp_table_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_debug_ee_n_ev_k_vp_table_s def;
  uint32 value;
} gsi_hwio_def_gsi_debug_ee_n_ev_k_vp_table_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chtype_protocol : 3;
  uint32 chtype_dir : 1;
  uint32 ee : 4;
  uint32 chid : 5;
  uint32 reserved0 : 1;
  uint32 erindex : 5;
  uint32 reserved1 : 1;
  uint32 chstate : 4;
  uint32 element_size : 8;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_length : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_base_addr_lsbs : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_2_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_base_addr_msbs : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_3_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_4 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_ptr_lsb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_4_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_4_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_4_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_5 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_ptr_msb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_5_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_5_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_5_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_6 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_lsb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_6_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_6_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_6_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_CNTXT_7 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_msb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_7_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_cntxt_7_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_cntxt_7_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_RE_FETCH_READ_PTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_ptr : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_read_ptr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_read_ptr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_read_ptr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 re_intr_db : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_write_ptr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_write_ptr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_re_fetch_write_ptr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_QOS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 wrr_weight : 4;
  uint32 reserved0 : 4;
  uint32 max_prefetch : 1;
  uint32 use_db_eng : 1;
  uint32 use_escape_buf_only : 1;
  uint32 reserved1 : 21;
} gsi_hwio_def_ee_n_gsi_ch_k_qos_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_qos_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_qos_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_SCRATCH_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_scratch_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_SCRATCH_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_scratch_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_SCRATCH_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_scratch_2_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_SCRATCH_3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_scratch_3_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_scratch_3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_DB_ENG_WRITE_PTR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 last_db_2_mcs : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_ee_n_gsi_ch_k_db_eng_write_ptr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_db_eng_write_ptr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_db_eng_write_ptr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chtype : 4;
  uint32 ee : 4;
  uint32 evchid : 8;
  uint32 intype : 1;
  uint32 reserved0 : 3;
  uint32 chstate : 4;
  uint32 element_size : 8;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_length : 16;
  uint32 reserved0 : 16;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_base_addr_lsbs : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_2_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 r_base_addr_msbs : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_3_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_4 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_ptr_lsb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_4_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_4_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_4_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_5 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 read_ptr_msb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_5_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_5_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_5_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_6 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_lsb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_6_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_6_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_6_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_7 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_msb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_7_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_7_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_7_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_8 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 int_modt : 16;
  uint32 int_modc : 8;
  uint32 int_mod_cnt : 8;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_8_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_8_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_8_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_9 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 intvec : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_9_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_9_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_9_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_10 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 msi_addr_lsb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_10_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_10_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_10_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_11 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 msi_addr_msb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_11_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_11_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_11_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_12 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rp_update_addr_lsb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_12_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_12_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_12_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_CNTXT_13 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rp_update_addr_msb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_13_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_cntxt_13_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_cntxt_13_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_SCRATCH_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_ev_ch_k_scratch_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_scratch_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_scratch_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_SCRATCH_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_ev_ch_k_scratch_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_scratch_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_scratch_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_DOORBELL_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_lsb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_doorbell_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_doorbell_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_doorbell_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_k_DOORBELL_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_msb : 32;
} gsi_hwio_def_ee_n_gsi_ch_k_doorbell_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_k_doorbell_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_k_doorbell_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_DOORBELL_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_lsb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_doorbell_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_doorbell_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_doorbell_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_k_DOORBELL_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 write_ptr_msb : 32;
} gsi_hwio_def_ee_n_ev_ch_k_doorbell_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_k_doorbell_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_k_doorbell_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_STATUS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 enabled : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ee_n_gsi_status_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_status_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_status_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_CH_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid : 8;
  uint32 reserved0 : 16;
  uint32 opcode : 8;
} gsi_hwio_def_ee_n_gsi_ch_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ch_cmd_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ch_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_EV_CH_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid : 8;
  uint32 reserved0 : 16;
  uint32 opcode : 8;
} gsi_hwio_def_ee_n_ev_ch_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_ev_ch_cmd_s def;
  uint32 value;
} gsi_hwio_def_ee_n_ev_ch_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_EE_GENERIC_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
	uint32 opcode : 5;
	uint32 virt_ch_index: 5;
	uint32 ee: 4;
	uint32 reserved: 18;
} gsi_hwio_def_ee_n_gsi_ee_generic_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_ee_generic_cmd_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_ee_generic_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_HW_PARAM_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ev_ch_num : 8;
  uint32 gsi_ch_num : 8;
  uint32 num_ees : 5;
  uint32 periph_conf_addr_bus_w : 5;
  uint32 periph_sec_grp : 5;
  uint32 use_axi_m : 1;
} gsi_hwio_def_ee_n_gsi_hw_param_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_hw_param_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_hw_param_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_HW_PARAM_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_m_data_bus_w : 8;
  uint32 gsi_num_qad : 4;
  uint32 gsi_nonsec_en : 4;
  uint32 gsi_sec_en : 1;
  uint32 gsi_vmidacr_en : 1;
  uint32 gsi_qrib_en : 1;
  uint32 gsi_use_xpu : 1;
  uint32 gsi_num_timers : 5;
  uint32 gsi_use_bp_mtrix : 1;
  uint32 gsi_use_db_eng : 1;
  uint32 gsi_use_uc_if : 1;
  uint32 gsi_escape_buf_only : 1;
  uint32 gsi_simple_rd_wr : 1;
  uint32 gsi_blk_int_access_region_1_en : 1;
  uint32 gsi_blk_int_access_region_2_en : 1;
} gsi_hwio_def_ee_n_gsi_hw_param_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_hw_param_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_hw_param_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_HW_PARAM_2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_iram_size : 3;
  uint32 gsi_num_ch_per_ee : 5;
  uint32 gsi_num_ev_per_ee : 5;
  uint32 gsi_ch_pend_translate : 1;
  uint32 gsi_ch_full_logic : 1;
  uint32 gsi_use_sdma : 1;
  uint32 gsi_sdma_n_int : 3;
  uint32 gsi_sdma_max_burst : 8;
  uint32 gsi_sdma_n_iovec : 3;
  uint32 gsi_use_rd_wr_eng : 1;
  uint32 gsi_use_inter_ee : 1;
} gsi_hwio_def_ee_n_gsi_hw_param_2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_hw_param_2_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_hw_param_2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_SW_VERSION 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 step : 16;
  uint32 minor : 12;
  uint32 major : 4;
} gsi_hwio_def_ee_n_gsi_sw_version_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_sw_version_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_sw_version_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_MCS_CODE_VER 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ver : 32;
} gsi_hwio_def_ee_n_gsi_mcs_code_ver_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_mcs_code_ver_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_mcs_code_ver_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_GSI_HW_PARAM_3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_sdma_max_os_rd : 4;
  uint32 gsi_sdma_max_os_wr : 4;
  uint32 gsi_num_prefetch_bufs : 4;
  uint32 gsi_m_addr_bus_w : 8;
  uint32 gsi_ree_max_burst_len : 5;
  uint32 gsi_use_irom : 1;
  uint32 reserved0 : 6;
} gsi_hwio_def_ee_n_gsi_hw_param_3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_gsi_hw_param_3_s def;
  uint32 value;
} gsi_hwio_def_ee_n_gsi_hw_param_3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_TYPE_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ch_ctrl : 1;
  uint32 ev_ctrl : 1;
  uint32 glob_ee : 1;
  uint32 ieob : 1;
  uint32 inter_ee_ch_ctrl : 1;
  uint32 inter_ee_ev_ctrl : 1;
  uint32 general : 1;
  uint32 reserved0 : 25;
} gsi_hwio_def_ee_n_cntxt_type_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_type_irq_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_type_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_TYPE_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ch_ctrl : 1;
  uint32 ev_ctrl : 1;
  uint32 glob_ee : 1;
  uint32 ieob : 1;
  uint32 inter_ee_ch_ctrl : 1;
  uint32 inter_ee_ev_ctrl : 1;
  uint32 general : 1;
  uint32 reserved0 : 25;
} gsi_hwio_def_ee_n_cntxt_type_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_type_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_type_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_GSI_CH_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_EV_CH_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map_msk : 11;
  uint32 reserved0 : 21;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_EV_CH_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map_msk : 11;
  uint32 reserved0 : 21;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_gsi_ch_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_EV_CH_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ev_ch_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_IEOB_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ieob_irq_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_IEOB_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map_msk : 11;
  uint32 reserved0 : 21;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ieob_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SRC_IEOB_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_src_ieob_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_src_ieob_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GLOB_IRQ_STTS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 error_int : 1;
  uint32 gp_int1 : 1;
  uint32 gp_int2 : 1;
  uint32 gp_int3 : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_glob_irq_stts_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_glob_irq_stts_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_glob_irq_stts_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GLOB_IRQ_EN 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 error_int : 1;
  uint32 gp_int1 : 1;
  uint32 gp_int2 : 1;
  uint32 gp_int3 : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_glob_irq_en_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_glob_irq_en_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_glob_irq_en_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GLOB_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 error_int : 1;
  uint32 gp_int1 : 1;
  uint32 gp_int2 : 1;
  uint32 gp_int3 : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_glob_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_glob_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_glob_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GSI_IRQ_STTS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_break_point : 1;
  uint32 gsi_bus_error : 1;
  uint32 gsi_cmd_fifo_ovrflow : 1;
  uint32 gsi_mcs_stack_ovrflow : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_stts_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_gsi_irq_stts_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_stts_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GSI_IRQ_EN 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_break_point : 1;
  uint32 gsi_bus_error : 1;
  uint32 gsi_cmd_fifo_ovrflow : 1;
  uint32 gsi_mcs_stack_ovrflow : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_en_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_gsi_irq_en_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_en_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_GSI_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_break_point : 1;
  uint32 gsi_bus_error : 1;
  uint32 gsi_cmd_fifo_ovrflow : 1;
  uint32 gsi_mcs_stack_ovrflow : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_gsi_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_gsi_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_INTSET 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 intype : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ee_n_cntxt_intset_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_intset_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_intset_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_MSI_BASE_LSB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 msi_addr_lsb : 32;
} gsi_hwio_def_ee_n_cntxt_msi_base_lsb_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_msi_base_lsb_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_msi_base_lsb_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_MSI_BASE_MSB 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 msi_addr_msb : 32;
} gsi_hwio_def_ee_n_cntxt_msi_base_msb_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_msi_base_msb_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_msi_base_msb_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_INT_VEC 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 int_vec : 32;
} gsi_hwio_def_ee_n_cntxt_int_vec_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_int_vec_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_int_vec_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_ERROR_LOG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  /*err_desc*/
  uint32 err_desc4 : 4;
  uint32 err_desc3 : 4;
  uint32 err_desc2 : 4;
  uint32 err_desc1 : 4;
  uint32 rsvd : 3;
  uint32 ch_ev_virtual_idx : 5;
  uint32 err_type : 4;
  uint32 ee : 4;
} gsi_hwio_def_ee_n_error_log_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_error_log_s def;
  uint32 value;
} gsi_hwio_def_ee_n_error_log_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_ERROR_LOG_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 error_log_clr : 32;
} gsi_hwio_def_ee_n_error_log_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_error_log_clr_s def;
  uint32 value;
} gsi_hwio_def_ee_n_error_log_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SCRATCH_0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_cntxt_scratch_0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_scratch_0_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_scratch_0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: EE_n_CNTXT_SCRATCH_1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scratch : 32;
} gsi_hwio_def_ee_n_cntxt_scratch_1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ee_n_cntxt_scratch_1_s def;
  uint32 value;
} gsi_hwio_def_ee_n_cntxt_scratch_1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MCS_CFG 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mcs_enable : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_gsi_mcs_cfg_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_mcs_cfg_s def;
  uint32 value;
} gsi_hwio_def_gsi_mcs_cfg_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_TZ_FW_AUTH_LOCK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 dis_iram_write : 1;
  uint32 dis_debug_shram_write : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_gsi_tz_fw_auth_lock_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_tz_fw_auth_lock_s def;
  uint32 value;
} gsi_hwio_def_gsi_tz_fw_auth_lock_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_MSA_FW_AUTH_LOCK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 dis_iram_write : 1;
  uint32 dis_debug_shram_write : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_gsi_msa_fw_auth_lock_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_msa_fw_auth_lock_s def;
  uint32 value;
} gsi_hwio_def_gsi_msa_fw_auth_lock_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: GSI_SP_FW_AUTH_LOCK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 dis_iram_write : 1;
  uint32 dis_debug_shram_write : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_gsi_sp_fw_auth_lock_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_gsi_sp_fw_auth_lock_s def;
  uint32 value;
} gsi_hwio_def_gsi_sp_fw_auth_lock_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_ORIGINATOR_EE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ee_number : 4;
  uint32 reserved0 : 28;
} gsi_hwio_def_inter_ee_n_originator_ee_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_originator_ee_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_originator_ee_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_GSI_CH_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid : 8;
  uint32 reserved0 : 16;
  uint32 opcode : 8;
} gsi_hwio_def_inter_ee_n_gsi_ch_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_gsi_ch_cmd_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_gsi_ch_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_EV_CH_CMD 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 chid : 8;
  uint32 reserved0 : 16;
  uint32 opcode : 8;
} gsi_hwio_def_inter_ee_n_ev_ch_cmd_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_ev_ch_cmd_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_ev_ch_cmd_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_GSI_CH_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map : 32;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_EV_CH_IRQ 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_ev_ch_irq_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_GSI_CH_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map_msk : 11;
  uint32 reserved0 : 21;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_EV_CH_IRQ_MSK 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map_msk : 11;
  uint32 reserved0 : 21;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_msk_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_ev_ch_irq_msk_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_msk_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_GSI_CH_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gsi_ch_bit_map : 32;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_gsi_ch_irq_clr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: INTER_EE_n_SRC_EV_CH_IRQ_CLR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 ev_ch_bit_map : 32;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_clr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_inter_ee_n_src_ev_ch_irq_clr_s def;
  uint32 value;
} gsi_hwio_def_inter_ee_n_src_ev_ch_irq_clr_u;

/*----------------------------------------------------------------------------
 * MODULE: IPA_0_GSI_TOP_XPU3
 *--------------------------------------------------------------------------*/

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_GCR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 aaden : 1;
  uint32 aalog_mode_dis : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_gcr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_gcr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_gcr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SCR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 scfgere : 1;
  uint32 sclere : 1;
  uint32 scfgeie : 1;
  uint32 scleie : 1;
  uint32 reserved0 : 4;
  uint32 dynamic_clk_en : 1;
  uint32 reserved1 : 23;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_scr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_scr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_scr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_CR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfgere : 1;
  uint32 clere : 1;
  uint32 cfgeie : 1;
  uint32 cleie : 1;
  uint32 reserved0 : 3;
  uint32 vmiden : 1;
  uint32 dynamic_clk_en : 1;
  uint32 reserved1 : 23;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_cr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_cr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_cr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RPU_ACR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 suvmid : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rpu_acr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rpu_acr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rpu_acr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_GCR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 qad0den : 1;
  uint32 qad0log_mode_dis : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_gcr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_gcr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_gcr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_CR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfgere : 1;
  uint32 clere : 1;
  uint32 cfgeie : 1;
  uint32 cleie : 1;
  uint32 reserved0 : 4;
  uint32 dynamic_clk_en : 1;
  uint32 reserved1 : 23;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_cr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_cr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_cr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_GCR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 qad1den : 1;
  uint32 qad1log_mode_dis : 1;
  uint32 reserved0 : 30;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_gcr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_gcr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_gcr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_CR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfgere : 1;
  uint32 clere : 1;
  uint32 cfgeie : 1;
  uint32 cleie : 1;
  uint32 reserved0 : 4;
  uint32 dynamic_clk_en : 1;
  uint32 reserved1 : 23;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_cr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_cr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_cr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_IDR3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 nvmid : 8;
  uint32 mv : 1;
  uint32 pt : 1;
  uint32 reserved0 : 22;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_idr3_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_IDR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 num_qad : 4;
  uint32 reserved0 : 4;
  uint32 vmidacr_en : 8;
  uint32 sec_en : 8;
  uint32 nonsec_en : 8;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_idr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_IDR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 reserved0 : 16;
  uint32 config_addr_width : 6;
  uint32 reserved1 : 2;
  uint32 client_addr_width : 6;
  uint32 reserved2 : 2;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_idr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_IDR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 xputype : 2;
  uint32 reserved0 : 3;
  uint32 clientreq_halt_ack_hw_en : 1;
  uint32 reserved1 : 10;
  uint32 nrg : 10;
  uint32 reserved2 : 6;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_idr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_idr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_REV 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 step : 16;
  uint32 minor : 12;
  uint32 major : 4;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rev_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rev_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rev_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_LOG_MODE_DIS 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 log_mode_dis : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_log_mode_dis_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_log_mode_dis_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_log_mode_dis_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGN_FREESTATUSr 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgfreestatus : 21;
  uint32 reserved0 : 11;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_freestatusr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_freestatusr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_freestatusr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SEAR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_31_0 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sear0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sear0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sear0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SESR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sesr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SESRRESTORE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesrrestore_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sesrrestore_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesrrestore_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SESYNR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 xprotns : 1;
  uint32 awrite : 1;
  uint32 xinst : 1;
  uint32 xpriv : 1;
  uint32 reserved0 : 4;
  uint32 qad : 8;
  uint32 alen : 8;
  uint32 asize : 3;
  uint32 reserved1 : 2;
  uint32 burstlen : 1;
  uint32 ac : 1;
  uint32 reserved2 : 1;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SESYNR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mid : 8;
  uint32 pid : 5;
  uint32 bid : 3;
  uint32 vmid : 8;
  uint32 tid : 8;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SESYNR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 memtype : 3;
  uint32 reserved0 : 4;
  uint32 transient : 1;
  uint32 noallocate : 1;
  uint32 ooowr : 1;
  uint32 ooord : 1;
  uint32 orderedwr : 1;
  uint32 orderedrd : 1;
  uint32 portmrel : 1;
  uint32 innerwritethrough : 1;
  uint32 innertransient : 1;
  uint32 innershared : 1;
  uint32 innercacheable : 1;
  uint32 innernoallocate : 1;
  uint32 writethrough : 1;
  uint32 shared : 1;
  uint32 full : 1;
  uint32 exclusive : 1;
  uint32 error : 1;
  uint32 earlywrresp : 1;
  uint32 device_type : 2;
  uint32 device : 1;
  uint32 cacheable : 1;
  uint32 burst : 1;
  uint32 bar : 2;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sesynr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_SEAR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_63_32 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sear1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_sear1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_sear1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_EAR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_31_0 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_ear0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_ear0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_ear0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_ESR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_esr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_ESRRESTORE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esrrestore_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_esrrestore_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esrrestore_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_ESYNR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 xprotns : 1;
  uint32 awrite : 1;
  uint32 xinst : 1;
  uint32 xpriv : 1;
  uint32 reserved0 : 4;
  uint32 qad : 8;
  uint32 alen : 8;
  uint32 asize : 3;
  uint32 reserved1 : 2;
  uint32 burstlen : 1;
  uint32 ac : 1;
  uint32 reserved2 : 1;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_ESYNR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mid : 8;
  uint32 pid : 5;
  uint32 bid : 3;
  uint32 vmid : 8;
  uint32 tid : 8;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_ESYNR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 memtype : 3;
  uint32 reserved0 : 4;
  uint32 transient : 1;
  uint32 noallocate : 1;
  uint32 ooowr : 1;
  uint32 ooord : 1;
  uint32 orderedwr : 1;
  uint32 orderedrd : 1;
  uint32 portmrel : 1;
  uint32 innerwritethrough : 1;
  uint32 innertransient : 1;
  uint32 innershared : 1;
  uint32 innercacheable : 1;
  uint32 innernoallocate : 1;
  uint32 writethrough : 1;
  uint32 shared : 1;
  uint32 full : 1;
  uint32 exclusive : 1;
  uint32 error : 1;
  uint32 earlywrresp : 1;
  uint32 device_type : 2;
  uint32 device : 1;
  uint32 cacheable : 1;
  uint32 burst : 1;
  uint32 bar : 2;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_esynr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_EAR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_63_32 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_ear1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_ear1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_ear1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_EAR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_31_0 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_ESR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_ESRRESTORE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esrrestore_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esrrestore_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esrrestore_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_ESYNR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 xprotns : 1;
  uint32 awrite : 1;
  uint32 xinst : 1;
  uint32 xpriv : 1;
  uint32 reserved0 : 4;
  uint32 qad : 8;
  uint32 alen : 8;
  uint32 asize : 3;
  uint32 reserved1 : 2;
  uint32 burstlen : 1;
  uint32 ac : 1;
  uint32 reserved2 : 1;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_ESYNR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mid : 8;
  uint32 pid : 5;
  uint32 bid : 3;
  uint32 vmid : 8;
  uint32 tid : 8;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_ESYNR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 memtype : 3;
  uint32 reserved0 : 4;
  uint32 transient : 1;
  uint32 noallocate : 1;
  uint32 ooowr : 1;
  uint32 ooord : 1;
  uint32 orderedwr : 1;
  uint32 orderedrd : 1;
  uint32 portmrel : 1;
  uint32 innerwritethrough : 1;
  uint32 innertransient : 1;
  uint32 innershared : 1;
  uint32 innercacheable : 1;
  uint32 innernoallocate : 1;
  uint32 writethrough : 1;
  uint32 shared : 1;
  uint32 full : 1;
  uint32 exclusive : 1;
  uint32 error : 1;
  uint32 earlywrresp : 1;
  uint32 device_type : 2;
  uint32 device : 1;
  uint32 cacheable : 1;
  uint32 burst : 1;
  uint32 bar : 2;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_esynr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD0_EAR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_63_32 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad0_ear1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_EAR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_31_0 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_ESR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_ESRRESTORE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 cfg : 1;
  uint32 client : 1;
  uint32 cfgmulti : 1;
  uint32 clmulti : 1;
  uint32 reserved0 : 28;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esrrestore_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esrrestore_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esrrestore_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_ESYNR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 xprotns : 1;
  uint32 awrite : 1;
  uint32 xinst : 1;
  uint32 xpriv : 1;
  uint32 reserved0 : 4;
  uint32 qad : 8;
  uint32 alen : 8;
  uint32 asize : 3;
  uint32 reserved1 : 2;
  uint32 burstlen : 1;
  uint32 ac : 1;
  uint32 reserved2 : 1;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_ESYNR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 mid : 8;
  uint32 pid : 5;
  uint32 bid : 3;
  uint32 vmid : 8;
  uint32 tid : 8;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_ESYNR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 memtype : 3;
  uint32 reserved0 : 4;
  uint32 transient : 1;
  uint32 noallocate : 1;
  uint32 ooowr : 1;
  uint32 ooord : 1;
  uint32 orderedwr : 1;
  uint32 orderedrd : 1;
  uint32 portmrel : 1;
  uint32 innerwritethrough : 1;
  uint32 innertransient : 1;
  uint32 innershared : 1;
  uint32 innercacheable : 1;
  uint32 innernoallocate : 1;
  uint32 writethrough : 1;
  uint32 shared : 1;
  uint32 full : 1;
  uint32 exclusive : 1;
  uint32 error : 1;
  uint32 earlywrresp : 1;
  uint32 device_type : 2;
  uint32 device : 1;
  uint32 cacheable : 1;
  uint32 burst : 1;
  uint32 bar : 2;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_esynr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_QAD1_EAR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 addr_63_32 : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_qad1_ear1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGN_OWNERSTATUSr 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgownerstatus : 21;
  uint32 reserved0 : 11;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_ownerstatusr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_ownerstatusr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_ownerstatusr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_GCR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rg_owner : 3;
  uint32 reserved0 : 5;
  uint32 rg_sec_apps : 1;
  uint32 reserved1 : 23;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_GCR3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 secure_access_lock : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr3_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_gcr3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_CR0 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgsclrden_apps : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr0_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr0_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr0_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_CR1 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgclrden : 3;
  uint32 reserved0 : 29;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr1_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr1_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr1_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_CR2 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgsclwren_apps : 1;
  uint32 reserved0 : 31;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr2_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr2_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr2_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_CR3 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rgclwren : 3;
  uint32 reserved0 : 29;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr3_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr3_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_cr3_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_RACR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 re : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_racr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_racr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_racr_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_0_GSI_TOP_XPU3_RGn_WACR 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 we : 32;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_wacr_s;

/* Union definition of register */
typedef union 
{
  gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_wacr_s def;
  uint32 value;
} gsi_hwio_def_ipa_0_gsi_top_xpu3_rgn_wacr_u;


#endif /* __GSI_HWIO_DEF_H__ */
