
#ifndef RFLM_DTR_TX_TYPEDEF_AG_H
#define RFLM_DTR_TX_TYPEDEF_AG_H


#ifdef __cplusplus
extern "C" {
#endif

/*
WARNING: This file is auto-generated.

Generated at:    Tue Jun 29 16:02:15 2021
Generated using: lm_autogen.pl v5.1.36
Generated from:  v7.4.5 of Jacala_TxFE_Register_Settings.xlsx
*/

/*=============================================================================

           T X    A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the modem register settings 
  configured by FW, provided by the rflm_dtr_tx.

Copyright (c) 2009, 2010, 2011, 2012, 2013, 2014, 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

$DateTime: 2021/06/30 04:51:17 $
$Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_divar/inc/rflm_dtr_tx_typedef_ag.h#2 $

=============================================================================*/

/*=============================================================================
                           REVISION HISTORY
Version    Author   Date   
         Comments   IR Number   Other POC (Comma Delimited)   Release Validation Notes   Dependencies   AG File Location   Changes for   
7.4.5   c_ygogad   6/29/2021   
         Updated DAC MSB cal & Mission mode settings, Ref HSR: https://projects.qualcomm.com/sites/IPCatalog/_layouts/15/WopiFrame.aspx?sourcedoc=/sites/IPCatalog/HSRs/Analog/Divar_TxDAC_Settings_HSR.xlsx&action=default      chandruk@qti.qualcommcom, vinuk@qti.qualcomm.com            TxDAC   
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

#include "comdef.h" 
#include "txlm_intf.h" 





typedef enum
{
  RFLM_DTR_TX_STATE_GSM_DAC_76P8MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_GSM_DAC_76P8MHZ_CHAIN0_IP2CAL,
  RFLM_DTR_TX_STATE_GSM_DAC_76P8MHZ_ANRITSU_CHAIN0,
  RFLM_DTR_TX_STATE_UMTS_SC_DAC_230P4MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_UMTS_SC_DAC_230P4MHZ_SAPT_CHAIN0,
  RFLM_DTR_TX_STATE_UMTS_DC_DAC_230P4MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_UMTS_DC_DAC_230P4MHZ_SAPT_CHAIN0,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW10MHZ_1KFFT,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW10MHZ_CW,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_INTRAULCA_NO15_NO20,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_INTRAULCA_15PLUS_20PLUS,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW20MHZ,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW15MHZ,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW5MHZ,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW3MHZ,
  RFLM_DTR_TX_STATE_LTE_DAC_230P4MHZ_CHAIN0_BW1P4MHZ,
  RFLM_DTR_TX_STATE_TDS_DAC_230P4MHZ_SAPT_CHAIN0,
  RFLM_DTR_TX_STATE_TDS_DAC_230P4MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_1X_DAC_230P4MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_1X_DAC_230P4MHZ_SAPT_CHAIN0,
  RFLM_DTR_TX_STATE_HDR_DAC_230P4MHZ_CHAIN0,
  RFLM_DTR_TX_STATE_NUM,
  RFLM_DTR_TX_STATE_INVALID
}rflm_dtr_tx_state_enum_type;



typedef enum
{
  RFLM_DTR_TX_BW_10MHZ, 
  RFLM_DTR_TX_BW_10MHZ_CW, 
  RFLM_DTR_TX_BW_20MHZ, 
  RFLM_DTR_TX_BW_40MHZ, 
  RFLM_DTR_TX_BW_15MHZ, 
  RFLM_DTR_TX_BW_5MHZ, 
  RFLM_DTR_TX_BW_3MHZ, 
  RFLM_DTR_TX_BW_1P4MHZ, 
  RFLM_DTR_TX_BW_NUM,
  RFLM_DTR_TX_BW_INVALID
}rflm_dtr_tx_enum_bw;



typedef enum
{
  RFLM_DTR_TX_CARRIER_CFG_SINGLE, 
  RFLM_DTR_TX_CARRIER_CFG_DUAL, 
  RFLM_DTR_TX_CARRIER_CFG_NUM,
  RFLM_DTR_TX_CARRIER_CFG_INVALID
}rflm_dtr_tx_enum_carrier_cfg;



typedef enum
{
  RFLM_DTR_TX_FFT_SIZE_1K, 
  RFLM_DTR_TX_FFT_SIZE_2K, 
  RFLM_DTR_TX_FFT_SIZE_NUM,
  RFLM_DTR_TX_FFT_SIZE_INVALID
}rflm_dtr_tx_enum_fft_size;



typedef enum
{
  RFLM_DTR_TX_INTRA_ULCA_NA, 
  RFLM_DTR_TX_INTRA_ULCA_NO_15_20, 
  RFLM_DTR_TX_INTRA_ULCA_WITH_15_20, 
  RFLM_DTR_TX_INTRA_ULCA_NUM,
  RFLM_DTR_TX_INTRA_ULCA_INVALID
}rflm_dtr_tx_enum_intra_ulca;



typedef struct
{
  txlm_chain_type chain;
  rflm_dtr_tx_enum_bw bw;
  rflm_dtr_tx_enum_carrier_cfg carrier_cfg;
  boolean sapt_on;
  rflm_dtr_tx_enum_fft_size fft_size;
  boolean ip2cal;
  boolean anritsu;
  rflm_dtr_tx_enum_intra_ulca intra_ulca;
}txlm_state_cfg_params;



typedef enum
{
  TXLM_GROUP_XO_VARS,	/* Use Struct: rflm_dtr_tx_xo_vars_group_struct */
  TXLM_GROUP_SW_XO_VARS,	/* Use Struct: rflm_dtr_tx_sw_xo_vars_group_struct */
  TXLM_GROUP_DIG_DELAY_VARS,	/* Use Struct: rflm_dtr_tx_dig_delay_vars_group_struct */
  TXLM_GROUP_ANALOG_DELAY_VARS,	/* Use Struct: rflm_dtr_tx_analog_delay_vars_group_struct */
  TXLM_GROUP_TXAGC_DM_VARS,	/* Use Struct: rflm_dtr_tx_txagc_dm_vars_group_struct */
  TXLM_GROUP_TXLM_VARS,	/* Use Struct: rflm_dtr_tx_txlm_vars_group_struct */
  TXLM_GROUP_TX_TOP,	/* Use Struct: rflm_dtr_tx_tx_top_group_struct */
  TXLM_GROUP_DAC_COMP,	/* Use Struct: rflm_dtr_tx_dac_comp_group_struct */
  TXLM_GROUP_TX_DAC_CFG,	/* Use Struct: rflm_dtr_tx_tx_dac_cfg_group_struct */
  TXLM_GROUP_IREF,	/* Use Struct: rflm_dtr_tx_iref_group_struct */
  TXLM_GROUP_LTE_CA,	/* Use Struct: rflm_dtr_tx_lte_ca_group_struct */
  TXLM_GROUP_TXC_DP_CFG,	/* Use Struct: rflm_dtr_tx_txc_dp_cfg_group_struct */
  TXLM_GROUP_IQMC,	/* Use Struct: rflm_dtr_tx_iqmc_group_struct */
  TXLM_GROUP_DC,	/* Use Struct: rflm_dtr_tx_dc_group_struct */
  TXLM_GROUP_PEQ,	/* Use Struct: rflm_dtr_tx_peq_group_struct */
  TXLM_GROUP_DM,	/* Use Struct: rflm_dtr_tx_dm_group_struct */
  TXLM_GROUP_EP_PEQ,	/* Use Struct: rflm_dtr_tx_ep_peq_group_struct */
  TXLM_GROUP_REF,	/* Use Struct: rflm_dtr_tx_ref_group_struct */
  TXLM_GROUP_TXFE_DELAYMATCH,	/* Use Struct: rflm_dtr_tx_txfe_delaymatch_group_struct */
  TXLM_GROUP_DPIQGAIN_TXC,	/* Use Struct: rflm_dtr_tx_dpiqgain_txc_group_struct */
  TXLM_GROUP_TIMESTAMP,	/* Use Struct: rflm_dtr_tx_timestamp_txc_group_struct */
  TXLM_GROUP_TXR_IQ_DAC,	/* Use Struct: rflm_dtr_tx_txr_iq_dac_group_struct */
  RFLM_DTR_TX_BLOCK_ENABLE_FLAGS,	/* Use Struct: rflm_dtr_tx_block_valid_flags_struct */
  RFLM_DTR_TX_INDICES,	/* Use Struct: rflm_dtr_tx_indices_struct */
  TXLM_GROUP_NUM,
  TXLM_GROUP_INVALID
}txlm_dyn_group_type;



typedef enum
{
  RFLM_DTR_TX_CHIPSET_ID_CHIPSET_JACALA,
  RFLM_DTR_TX_CHIPSET_ID_NUM,
  RFLM_DTR_TX_CHIPSET_ID_INVALID
}rflm_dtr_tx_enum_chipset_id;

typedef enum
{
  RFLM_DTR_TX_LTE_LTE_BW10MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_BW20MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_BW15MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_BW5MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_BW3MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_BW1p4MHZ_WTR_SAW,
  RFLM_DTR_TX_LTE_LTE_INTRA_ULCA_SAW,
  RFLM_DTR_TX_LTE_UMTS_SC_SAW_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_SC_SAPT_SAW_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_DC_SAW_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_DC_SAPT_SAW_WTR4905,
  RFLM_DTR_TX_LTE_LTE_BW10MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_BW20MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_BW15MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_BW5MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_BW3MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_BW1p4MHZ_SAWLESS,
  RFLM_DTR_TX_LTE_LTE_INTRA_ULCA_SAWLESS,
  RFLM_DTR_TX_LTE_UMTS_SC_SAWLESS_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_SC_SAPT_SAWLESS_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_DC_SAWLESS_WTR4905,
  RFLM_DTR_TX_LTE_UMTS_DC_SAPT_SAWLESS_WTR4905,
  RFLM_DTR_TX_DP_PEQ_SW_DYNAMIC_SETTINGS_NUM,
  RFLM_DTR_TX_DP_PEQ_SW_DYNAMIC_SETTINGS_INVALID
}rflm_dtr_tx_dp_peq_sw_dynamic_settings_enum_type;

typedef enum
{
  RFLM_DTR_TX_LTE_INTER_CA,
  RFLM_DTR_TX_LTE_INTERBAND_CA_SW_DYNAMIC_SETTINGS_NUM,
  RFLM_DTR_TX_LTE_INTERBAND_CA_SW_DYNAMIC_SETTINGS_INVALID
}rflm_dtr_tx_lte_interband_ca_sw_dynamic_settings_enum_type;

typedef enum
{
  RFLM_DTR_TX_DP_PEQ,
  RFLM_DTR_TX_LTE_INTERBAND_CA,
  RFLM_DTR_TX_DYN_FUNCTIONALITY_NUM,
  RFLM_DTR_TX_DYN_FUNCTIONALITY_INVALID
}rflm_dtr_tx_dyn_functionality_type;

typedef struct
{
	txlm_chain_type chain;
	boolean et_slave;
	rflm_dtr_tx_enum_chipset_id chipset_id;
}txlm_dac_cal_config_type;

typedef enum
{
   /* Write enums */
   TXLM_DAC_CAL_CMD_WRITE_DAC_CALIB_CMDS_C_0_1_MSBCAL_MEM_READ, /* Trigger to read DACc internal memory. */
   TXLM_DAC_CAL_CMD_WRITE_DAC_CALIB_CMDS_C_0_1_MSBCAL_MEM_WRITE, /* Trigger to write DACc internal memory. */
   TXLM_DAC_CAL_CMD_WRITE_DAC_CALIB_CMDS_C_0_1_TX_CFG_START, /* Trigger MSB Calibration on DACc */

   /* Read enums */
   TXLM_DAC_CAL_CMD_READ_DAC_CALIB_STATUS_C_0_1_MSBCAL_MEM_RD_DONE, /* Indicate DACc internal memory read done */
   TXLM_DAC_CAL_CMD_READ_DAC_CALIB_STATUS_C_0_1_MSBCAL_MEM_WR_DONE, /* Indicate DACc internal memory write done */
   TXLM_DAC_CAL_CMD_READ_DAC_CALIB_STATUS_C_0_1_TX_ALL_DONE, /* Indicates all calibrations is done. */

   TXLM_DAC_CAL_CMD_NUM,
   TXLM_DAC_CAL_CMD_INVALID
}txlm_dac_cal_cmd_enum_type;


#ifdef __cplusplus
}
#endif



#endif


