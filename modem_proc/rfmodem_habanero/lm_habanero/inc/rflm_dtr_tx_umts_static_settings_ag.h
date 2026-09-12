
#ifndef RFLM_DTR_TX_UMTS_STATIC_SETTINGS_AG_H
#define RFLM_DTR_TX_UMTS_STATIC_SETTINGS_AG_H


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
$Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_habanero/inc/rflm_dtr_tx_umts_static_settings_ag.h#1 $

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

#include "rflm_dtr_tx_struct_ag.h" 




extern rflm_dtr_tx_settings_type_ag rflm_dtr_tx_umts_sc_dac_230p4mhz_chain0_settings_ag;

extern rflm_dtr_tx_settings_type_ag rflm_dtr_tx_umts_sc_dac_230p4mhz_sapt_chain0_settings_ag;

extern rflm_dtr_tx_settings_type_ag rflm_dtr_tx_umts_dc_dac_230p4mhz_chain0_settings_ag;

extern rflm_dtr_tx_settings_type_ag rflm_dtr_tx_umts_dc_dac_230p4mhz_sapt_chain0_settings_ag;

#ifdef __cplusplus
}
#endif



#endif


