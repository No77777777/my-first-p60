
#ifndef RFLM_DTR_TX_STRUCT_AG_H
#define RFLM_DTR_TX_STRUCT_AG_H


#ifdef __cplusplus
extern "C" {
#endif

/*
WARNING: This file is auto-generated.

Generated at:    Fri Dec 20 03:39:48 2019
Generated using: lm_autogen.pl v5.1.36
Generated from:  v7.4.4 of Jacala_TXLM_register_settings.xlsx
*/

/*=============================================================================

           T X    A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the modem register settings 
  configured by FW, provided by the rflm_dtr_tx.

Copyright (c) 2009, 2010, 2011, 2012, 2013, 2014, 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

$DateTime: 2021/03/09 05:29:45 $
$Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_habanero/inc/rflm_dtr_tx_struct_ag.h#1 $

=============================================================================*/

/*=============================================================================
                           REVISION HISTORY
Version    Author   Date   
         Comments   IR Number   Other POC (Comma Delimited)   Release Validation Notes   Dependencies   AG File Location   Changes for   
7.4.4   yitankar   11/4/2019   
         Update DAC calibration config for Habanero. Reference document - https://projects.qualcomm.com/sites/IPCatalog/HSRs/Analog/Kamorta_TxDAC_Settings_HSR.xlsx      dsarma@qti.qualcomm.com            DAC   
7.4.3   nichunw   12/18/2015   
         1. Fixed REF_I_GAIN/REF_Q_GAIN to 1 for HDR.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            C2K   
7.4.2   nichunw   11/30/2015   
         1. Update SYSBDY_duration_NANOSECS for 1x from 192 cx1 to 384 cx1.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            C2K   
7.4.1   nichunw   11/4/2015   
         1. Added Jacala IREF LUT.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.4.0   nichunw   10/17/2015   
         1. Added tech system boundary duration in TxLM var,SYSBDY_duration_NANOSECS, for autopin feature.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            Autopin   
7.3.8   nichunw   10/15/2015   
         1. Disable SORU for LTE ULCA with aggregated BW below or equal to 20MHz.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.3.7   nichunw   10/1/2015   
         1. Added SORU coefficients for LTE intra-band ULCA. New enums added in DP_PEQ_SW_Dynamic tab.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.3.6   nichunw   9/18/2015   
         1. Adding modem specific tabs for Jacala support.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.3.5   nichunw   9/14/2015   
         [Back out the Jacala change] 1. Updated Eldarion foundry-specific DAC configurations and IREF table.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.3.4   nichunw   7/17/2015   
         1. Fixed the n_index of ULCA chain1 states.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.3.3   lmiao/nichunw   6/24/2015   
         1. Updated DAC_cal_LM_settings to distinguish between Sahi and Eldarion.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.3.2   nichunw   6/10/2015   
         1. Update COMBODAC_CFG0 for Eldarion.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.3.1   nichunw   6/8/2015   
         1. Due to increase of foundry ID bits and the chip-dependent feature, the pass in value in the     overflow_1(6)    avg_error_1(5)    overflow_0(4)    avg_error_0(3)    foundry_ID(2:0)]. The mask values are also changed correspondingly. 2. Chip_id=0 for Sahi    1 for Eldarion and 2 for Jacala. 3. Updated Sahi's fab-dependent calibration and mission mode DAC config and IREF LUT. 3. Added Eldarion DAC config."   
7.3.0   nichunw   5/28/2015   
         Rename version number to 7.3.0 for FW dependency change in autogen.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            All   
7.2.2   nichunw   5/27/2015   
         1. Add remaining bits of TX_MODE_COMMON on LTE_interband_CA_SW_Dynamic tab.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.2.1   nichunw   5/12/2015   
         1. New definition of intra_ULCA state parameter.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.2.0   nichunw   4/21/2015   
         1. Separate TXR_IQ_DAC_An from TXR block to a new TXR_IQ_DAC block (runtime dynamic). C2K needs to dynamically updating this register on hand-off between low-band and mid/high-band in WTR2955.       nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            All   
7.1.2   nichunw   4/13/2015   
         1. Enabled TWO_SAMPLES_CYCLE for HDR.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DO   
7.0.3   nichunw   4/10/2015   
         1. Revert back to 7.0 to exclude LTE CA mixer changes for bring-up. 2. Includes all the change in 7.1.1.  3. Updated COMBODAC_CFG1_d to enable the IREF read-out      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            All techs   
7.1.1   nichunw   4/9/2015   
         1. Change REF_DWN_FACTOR to 1 for TDS states to lower down the DPD capture rate by half. 2. Fixed naming on DAC_cal_LM_settings tab. 3. Disable PEQ/SORU for all tech.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            All techs   
7.1.0   nichunw   3/3/2015   
         1. Restore LTE_DAC_230p4MHz_chain0_BW20MHz state. 2. Update version number to 7.1.0 for the new addition of  a runtime dynamic block in v7.0.2.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE   
7.0.2   nichunw   2/5/2015   
         1. Added GSM_DAC_76p8MHz_Anritsu_chain1 state. 2. Added LTE 40MHz states for intra-ULCA. 3. Updated COMBODAC_CFG1_d to enable the IREF read-out. 4. Added LTE_interband_CA_SW_Dynamic tab for LTE interband CA configurations. 5. Added LTE_CA_MIXER block for intra-band CA. 6. Make TX_TOP subgroup SW dynamic.      nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            LTE, DAC, GSM   
7.0.1   nichunw   10/28/2014   
         1. Updated the d index in COMBODAC_CAL_STATUS on the DAC cal control register tab.       nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            DAC   
7.0.0   nichunw   10/2/2014   
         Iinitial version. Changes from Jolokia: 1. Two complete TXC/TXR chains and all technologies are supported on each chain. All TXC registers are in pairs  2.TXC/TXR chain 1 is identical to chain 0, while in Jolokia chain 1 is simplified to GSM only. 3. Support DC-HSUPA. TX_UNIFIED_ENC_EUL_CARR_ENS is added to specified SC or DC UMTS.  4. Two DACs/WTRs to support full DSDA. Added chain 1 ComboDAC registers).       nichunw@qti.qualcomm.com,sunilk@qti.qualcomm.com            All   
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              
                              

=============================================================================*/
/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rflm_dtr_tx_fw_intf_ag.h" 
#include "rflm_dtr_tx_typedef_ag.h" 



/* This struct captures flags that control whether certain groups of registers are programmed for a given state. */
typedef struct
{
  boolean mtc_common_flag;/* This flag enables programming of the TXC registers to program both the MTC_TRIG blocks*/
  boolean mdm_clk1_flag;/* This flag enables programming of the MTC registers to program the CLK_DAC for Tx chain 1*/
  boolean mdm_clk0_flag;/* This flag enables programming of the MTC registers to program the CLK_DAC for Tx chain 1*/
  boolean tx_common_flag;/* This flag enables programming of the TXC registers to program both the Tx chains*/
  boolean iref_update_flag;/* This flag enables programming of the DAC IREF registers to program both the IREF gain value for both chains*/
  boolean lte_ca_mixer_flag;/* This flag enables programming of the DAC IREF registers to program both the IREF gain value for both chains*/
  boolean txc_flag;/* This flag enables programming of the TXC registers to program both chains*/
  boolean txr_flag;/* This flag enables programming of the TXR registers to program the Tx chain n*/
  boolean txr_iq_dac_flag;/* This flag enables programming of the TXR_IQ_DAC registers to program the Tx chain n*/
}rflm_dtr_tx_block_valid_flags_struct;



 /* Group MTC_TRIG: */ 
/* This block consist of the TXC registers to program both the MTC_TRIG blocks*/
typedef struct
{
  uint32 tx_cn_ctrl_sel;
  uint32 tx_cn_trig_cmd;
  uint32 tx_cn_trig_val;
}rflm_dtr_tx_mtc_trig_group_struct;



 /* Group DACCOMP_MUX: */ 
/* This block consist of the TXC registers to program both the MTC_TRIG blocks*/
typedef struct
{
  uint32 daccomp_mux_chainn_strt_val;
  uint32 daccomp_mux_chainn_strt_trig_cmd;
  uint32 daccomp_mux_chainn_stop_val;
  uint32 daccomp_mux_chainn_stop_trig_cmd;
}rflm_dtr_tx_daccomp_mux_group_struct;



 /* Group DAC_CLK: */ 
/* This block consist of the TXC registers to program both the MTC_TRIG blocks*/
typedef struct
{
  uint32 mss_cc_dac_clk_cfg;
}rflm_dtr_tx_dac_clk_group_struct;



 /* Group TX_EN: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 tx_enable;
}rflm_dtr_tx_tx_en_group_struct;



 /* Group TX_TOP: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 tx_enable_gsm_c;
  uint32 tx_mode_common;
  uint32 tx_mode_common_mask;
  uint32 tx_tds_ctrl;
  uint32 tx_tds_ctrl_mask;
  uint32 tx_mode_ofdma;
  uint32 tx_mode_ofdma_mask;
  uint32 tx_mode_ofdma_ca;
  uint32 tx_mode_ofdma_ca_mask;
  uint32 tx_unified_enc_eul_carr_ens;
  uint32 tx_unified_enc_eul_carr_ens_mask;
  uint32 tx_mem_access_cfg;
  uint32 tx_stmr_trig_frac_cnt_c;
  uint32 tx_mode_c;
}rflm_dtr_tx_tx_top_group_struct;



 /* Group DAC_COMP: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 mss_cc_clk_xo_gate_cfg;
  uint32 dac_calib_cfg_c;
}rflm_dtr_tx_dac_comp_group_struct;



 /* Group TX_DAC_CFG: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 txdac_config0_c;
  uint32 txdac_config1_c;
}rflm_dtr_tx_tx_dac_cfg_group_struct;



 /* Group AXIW: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 tx_axi_wr_intf_event;
  uint32 tx_axi_wr_intf_event_mask;
}rflm_dtr_tx_axiw_group_struct;



 /* Group AXIR: */ 
/* This block consist of the TXC registers to program both the Tx chains*/
typedef struct
{
  uint32 tx_axi_rd0_intf_ctl;
  uint32 tx_axi_rd0_intf_ctl_mask;
  uint32 tx_axi_rd0_intf_event;
  uint32 tx_axi_rd0_intf_event_mask;
}rflm_dtr_tx_axir_group_struct;



 /* Group IREF: */ 
/* This block consist of the DAC IREF registers to program both the IREF gain value for both chains*/
typedef struct
{
  uint32 txdac_config2_c;
  uint32 txdac_config3_c;
  uint32 tx_dacn_update_strt_val;
  uint32 tx_dacn_update_strt_trig_cmd;
  uint32 tx_dacn_update_stop_val;
  uint32 tx_dacn_update_stop_trig_cmd;
}rflm_dtr_tx_iref_group_struct;



 /* Group LTE_CA: */ 
/* This block consist of the DAC IREF registers to program both the IREF gain value for both chains*/
typedef struct
{
  uint32 tx_lte_ca_mixer_ctl;
  uint32 tx_lte_ca_mixer_ctl_mask;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_c0;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_c0_mask;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_c1;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_c1_mask;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_offset_c0;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_offset_c0_mask;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_offset_c1;
  uint32 tx_lte_ca_mixer_rotator_ctrl_wd_offset_c1_mask;
  uint32 tx_lte_ca_iq_gain_c0;
  uint32 tx_lte_ca_iq_gain_c0_mask;
  uint32 tx_lte_ca_iq_gain_c1;
  uint32 tx_lte_ca_iq_gain_c1_mask;
  uint32 tx_lte_ca_iq_gain_set2_c0;
  uint32 tx_lte_ca_iq_gain_set2_c0_mask;
  uint32 tx_lte_ca_iq_gain_set2_c1;
  uint32 tx_lte_ca_iq_gain_set2_c1_mask;
  uint32 tx_lte_ca_iq_gain_set3_c0;
  uint32 tx_lte_ca_iq_gain_set3_c0_mask;
  uint32 tx_lte_ca_iq_gain_set3_c1;
  uint32 tx_lte_ca_iq_gain_set3_c1_mask;
  uint32 tx_lte_ca_timestamp_t0;
  uint32 tx_lte_ca_timestamp_t0_mask;
  uint32 tx_lte_ca_timestamp_t1;
  uint32 tx_lte_ca_timestamp_t1_mask;
  uint32 tx_lte_ca_timestamp_t2;
  uint32 tx_lte_ca_timestamp_t2_mask;
  uint32 tx_lte_ca_up2_dm_bp_sel_c0;
  uint32 tx_lte_ca_up2_dm_bp_sel_c0_mask;
  uint32 tx_lte_ca_up2_dm_bp_sel_c1;
  uint32 tx_lte_ca_up2_dm_bp_sel_c1_mask;
  uint32 tx_lte_ca_timestamp_reuse;
  uint32 tx_lte_ca_timestamp_reuse_mask;
  uint32 tx_lte_ca_dm_delay;
  uint32 tx_lte_ca_dm_delay_mask;
}rflm_dtr_tx_lte_ca_group_struct;



 /* Group TIMESTAMP_TXC: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_timestamp_t1_an;
  uint32 txc_timestamp_t2_an;
}rflm_dtr_tx_timestamp_txc_group_struct;



 /* Group TXC_DP_CFG: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_tech_sel_an;
  uint32 txc_dp_cfg_2_an;
  uint32 txc_dp_cfg_an;
}rflm_dtr_tx_txc_dp_cfg_group_struct;



 /* Group TXC: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dp_env_scale_val_an;
  uint32 txc_dp_env_scale_val_set2_an;
  uint32 txc_dp_phase_ovr_val_an;
  uint32 txc_pp_phase_ovr_val_an;
  uint32 txc_dp_env_ovr_val_an;
  uint32 txc_ep_env_ovr_val_an;
  uint32 txc_ep_env_ovr_val_set2_an;
  uint32 txc_ep_gain_an;
  uint32 txc_ep_gain_set2_an;
  uint32 txc_pp_gain_an;
  uint32 txc_pp_spill_an;
  uint32 txc_timestamp_reuse_an;
  uint32 txc_sample_count_ctl_an;
  uint32 txc_amam_pending_an;
  uint32 txc_ampm_pending_an;
  uint32 txc_amam_pending_set2_an;
  uint32 txc_ampm_pending_set2_an;
}rflm_dtr_tx_txc_group_struct;



 /* Group RAMP: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dp_ramp_arm_an;
  uint32 txc_dp_ramp_start_time_an;
  uint32 txc_dp_ramp_slot_a_cfg_an;
  uint32 txc_dp_ramp_slot_b_cfg_an;
  uint32 txc_dp_ramp_gap_an;
}rflm_dtr_tx_ramp_group_struct;



 /* Group ROT: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dp_rot_phase_inc_an;
  uint32 txc_dp_rot_phase_inc_set2_an;
  uint32 txc_dp_rot_phase_init_an;
  uint32 txc_dp_rot_phase_init_set2_an;
  uint32 txc_dp_rot_phase_shift_an;
  uint32 txc_dp_rot_phase_shift_set2_an;
}rflm_dtr_tx_rot_group_struct;



 /* Group IQMC: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dp_iqmc_an;
  uint32 txc_dp_iqmc_set2_an;
}rflm_dtr_tx_iqmc_group_struct;



 /* Group DC: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_ep_dcoc_an;
  uint32 txc_ep_dcoc_set2_an;
  uint32 txc_dp_dcoc_an;
  uint32 txc_dp_dcoc_set2_an;
}rflm_dtr_tx_dc_group_struct;



 /* Group PEQ: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dp_peq1_an;
  uint32 txc_dp_peq1_set2_an;
  uint32 txc_dp_peq1_q_an;
  uint32 txc_dp_peq1_q_set2_an;
  uint32 txc_dp_peq2_an;
  uint32 txc_dp_peq2_set2_an;
}rflm_dtr_tx_peq_group_struct;



 /* Group DM: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_dm1_cfg_an;
}rflm_dtr_tx_dm_group_struct;



 /* Group EP_PEQ: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_ep_peq1_an;
  uint32 txc_ep_peq1_set2_an;
  uint32 txc_ep_peq2_a0;
}rflm_dtr_tx_ep_peq_group_struct;



 /* Group REF: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_ref_arm_an;
  uint32 txc_ref_dwn_factor_an;
  uint32 txc_ref_num_samples_an;
  uint32 txc_ref_start_time_an;
  uint32 txc_ref_mode_an;
  uint32 txc_ref_i_gain_val_an;
  uint32 txc_ref_q_gain_val_an;
  uint32 txc_ref_powercalc_or_log_an;
}rflm_dtr_tx_ref_group_struct;



 /* Group DBG: */ 
/* This block consist of the TXC registers to program both chains*/
typedef struct
{
  uint32 txc_testbus_select_an;
}rflm_dtr_tx_dbg_group_struct;



 /* Group TXFE_DELAYMATCH: */ 
/* This block consist of the delay matching registers to program the Tx chain n*/
typedef struct
{
  uint32 txc_ep_dm2_fine_tau_an;
  uint32 txc_ep_dm3_frac_tau_an;
}rflm_dtr_tx_txfe_delaymatch_group_struct;



 /* Group DPIQGAIN_TXC: */ 
/* This block consist of the DP_IQ_GAIN registers to program the Tx chain 0*/
typedef struct
{
  uint32 txc_dp_iq_gain_an;
  uint32 txc_dp_iq_gain_an_mask;
  uint32 txc_dp_iq_gain_set2_an;
  uint32 txc_dp_iq_gain_set2_an_mask;
}rflm_dtr_tx_dpiqgain_txc_group_struct;



 /* Group TXR: */ 
/* This block consist of the TXR registers to program the Tx chain n*/
typedef struct
{
  uint32 txr_str_ctrl_an;
  uint32 txr_pda_ctrl_an;
  uint32 txr_pda_wd_update_ctrl_an;
  uint32 txr_pda_ctrl_wd_offset_an;
  uint32 txr_pda_offset_cmd_an;
  uint32 txr_fifo_cmd_an;
  uint32 txr_fifo_ctrl_an;
}rflm_dtr_tx_txr_group_struct;



 /* Group PPDSM: */ 
/* This block consist of the TXR registers to program the Tx chain n*/
typedef struct
{
  uint32 txr_ppdsm_ctl_an;
  uint32 txr_ppdsm_override_val_an;
  uint32 txr_ppdsm_override_rep_an;
}rflm_dtr_tx_ppdsm_group_struct;



 /* Group TXR_STREAM: */ 
/* This block consist of the TXR registers to program the Tx chain n*/
typedef struct
{
  uint32 txr_stream_if_ctrl_an;
  uint32 txr_fmt_conv_ctl_an;
}rflm_dtr_tx_txr_stream_group_struct;



 /* Group TXR_IQ_DAC: */ 
/* This block consist of the TXR_IQ_DAC registers to program the Tx chain n*/
typedef struct
{
  uint32 txr_iq_dac_an;
}rflm_dtr_tx_txr_iq_dac_group_struct;



// SW FW Interface Buffer

typedef struct ALIGN(32)
{
  rflm_dtr_tx_xo_vars_group_struct xo_vars_params;
  rflm_dtr_tx_sw_xo_vars_group_struct sw_xo_vars_params;
  rflm_dtr_tx_dig_delay_vars_group_struct dig_delay_vars_params;
  rflm_dtr_tx_analog_delay_vars_group_struct analog_delay_vars_params;
  rflm_dtr_tx_txagc_dm_vars_group_struct txagc_dm_vars_params;
  rflm_dtr_tx_txlm_vars_group_struct txlm_vars_params;
  rflm_dtr_tx_mtc_trig_group_struct mtc_trig_params;
  rflm_dtr_tx_daccomp_mux_group_struct daccomp_mux_params;
  rflm_dtr_tx_dac_clk_group_struct dac_clk_params;
  rflm_dtr_tx_dac_clk_group_struct mtc0_params;
  rflm_dtr_tx_tx_en_group_struct tx_en_params;
  rflm_dtr_tx_tx_top_group_struct tx_top_params;
  rflm_dtr_tx_dac_comp_group_struct dac_comp_params;
  rflm_dtr_tx_tx_dac_cfg_group_struct tx_dac_cfg_params;
  rflm_dtr_tx_axiw_group_struct axiw_params;
  rflm_dtr_tx_axir_group_struct axir_params;
  rflm_dtr_tx_iref_group_struct iref_params;
  rflm_dtr_tx_lte_ca_group_struct lte_ca_params;
  rflm_dtr_tx_timestamp_txc_group_struct timestamp_txc_params;
  rflm_dtr_tx_txc_dp_cfg_group_struct txc_dp_cfg_params;
  rflm_dtr_tx_txc_group_struct txc_params;
  rflm_dtr_tx_ramp_group_struct ramp_params;
  rflm_dtr_tx_rot_group_struct rot_params;
  rflm_dtr_tx_iqmc_group_struct iqmc_params;
  rflm_dtr_tx_dc_group_struct dc_params;
  rflm_dtr_tx_peq_group_struct peq_params;
  rflm_dtr_tx_dm_group_struct dm_params;
  rflm_dtr_tx_ep_peq_group_struct ep_peq_params;
  rflm_dtr_tx_ref_group_struct ref_params;
  rflm_dtr_tx_dbg_group_struct dbg_params;
  rflm_dtr_tx_txfe_delaymatch_group_struct txfe_delaymatch_params;
  rflm_dtr_tx_dpiqgain_txc_group_struct dpiqgain_txc_params;
  rflm_dtr_tx_timestamp_txc_group_struct timestamp_params;
  rflm_dtr_tx_txr_group_struct txr_params;
  rflm_dtr_tx_ppdsm_group_struct ppdsm_params;
  rflm_dtr_tx_txr_stream_group_struct txr_stream_params;
  rflm_dtr_tx_txr_iq_dac_group_struct txr_iq_dac_params;
  rflm_dtr_tx_indices_struct rflm_dtr_tx_reg_indices;
  rflm_dtr_tx_block_valid_flags_struct rflm_dtr_tx_block_valid;
  rflm_dtr_tx_header_struct tx_header;
}rflm_dtr_tx_settings_type_ag;




#ifdef __cplusplus
}
#endif



#endif


