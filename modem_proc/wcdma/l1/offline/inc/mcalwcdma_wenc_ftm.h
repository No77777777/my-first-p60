#ifndef __MCALWCDMA_WENC_FTM_H__
#define __MCALWCDMA_WENC_FTM_H__
/*============================================================================*/
/** @file
 * This module has declarations related to Factory test mode support for
 * Enhanced Uplink.
 */
/*============================================================================*/

/*============================================================================
                        M C A L W C D M A _ W E N C _ F T M. H
DESCRIPTION
  This module has definition and declaration related to MCAL (modem core
  abstraction layer) W Encoder APIs utilized by RF in Factory test mode.
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2013 QUALCOMM Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/mcalwcdma_wenc_ftm.h#3 $

when      who     what, where, why
--------  ---     --------------------------------------------------------------
03/11/15  vr      FTM support for EUL & DCHSUPA.
01/16/15  vr      Global reorg for W + W.
01/14/15  vr      Changing globals to CONST.
07/28/14  pkg     Code Check-in for 16QAM feature.
06/12/14  tsk     Added Jolokia featurization for FTM related changes
06/08/14  ar      File created 
*/ 
#if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
/*                         External Variable Declarations                  */
extern const uint8 eul_edpcch_pwr_offset_tab[];
extern const uint8 eul_a_ed_quan_hash_tab[1][169];
extern const uint8 eul_edpdch_pwr_offset_tab[1][30];
extern const uint8 eulenc_chan_code_to_hwsf_map_tab[];
extern const uint8 eulenc_chan_code_to_hwovsf_map_tab[];
extern const eul_n_e_data_tab_struct_type eul_n_e_data_tab_2ms;
extern const eul_n_e_data_tab_struct_type eul_n_e_data_tabs_10ms[];
extern const uint8 eulsg_post_fiq_firing_times_array[];

/*                External Functions Declarations                          */
extern uint32 enchs_get_AhsX15_for_delta
(
  uint8  delta_hs,          /* 0 ~ 8 */
  uint8  dpcch_slt_fmt_nm,  /* 0 ~ 3 */
  boolean compressed,       /* CM or normal */
  boolean cm_slot_format_a  /* CM A or B */
);

typedef struct {
  uint8 bc;
  uint8 bd;
  uint8 dhs;
  uint8 dec;
  uint8 ded;
  uint8 bed2;
  uint8 mu;
  uint8 num_dpdch;
  boolean hsdpcch_en;
  uint8 modulation_scheme;
  e_tti_enum_type e_tti;
  uint8 num_carriers;
  e_ul_chan_action_enum_type last_action;
  mdspasync_edpch_param_db_struct_type next_tti_fw_param_ptr[WL1_MAX_UL_CARR_IDX];
  l1_e_max_ch_codes_enum_type etfci_coding_sf[WL1_MAX_UL_CARR_IDX];
  post_cb_fn edch_sync_cmd_post_callback;
  eul_gen_tl_event_enum_type eul_event_type;
} mcalwcdma_wenc_ftm_eul_config_type;

typedef struct
{
  e_ul_mdsp_cfg_db_struct_type       e_ul_mdsp_cfg_db_info_for_ftm;
  mcalwcdma_wenc_ftm_eul_config_type mcalwcdma_wenc_ftm_eul_config_db;
} mcalwcdma_wenc_ftm_ext_cntrl_params_type;

extern mcalwcdma_wenc_ftm_ext_cntrl_params_type mcalwcdma_wenc_ftm_ext_cntrl_params;

typedef struct
{
  uint8 eram_bank_c2;
  uint8 eram_bank_c1;
} mcalwcdma_wenc_ftm_int_cntrl_params_type;

extern void eulenc_calc_turbo_interleaving_parms(uint32 num_bits_per_code_seg, wsub_id_e_type wsub_id);

/*                         Functions Declarations                          */
/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_eul_encoder_proc

DESCRIPTION   This function is the equivalent of the EUL FIQ in online mode 
              It populates encoder related parameters to micro-kernel and FW 
 
DEPENDENCIES  mcalwcdma_wenc_ftm_eul_config_db.next_tti_fw_param_ptr must be 
              populated before this ISR is enabled, ideally by a call to
              mcalwcdma_wenc_ftm_populate_next_tti_params

RETURN VALUE  None

SIDE EFFECTS  FW and micro-kernel will be populated with encoder parameters
===========================================================================*/
void mcalwcdma_wenc_ftm_eul_encoder_proc(void);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_mdsp_edch_pre_config

DESCRIPTION   This function is the called prior to sending the sync command
              for EUL to FW
 
DEPENDENCIES  None
 
RETURN VALUE  None

SIDE EFFECTS  e_ul_mdsp_cfg_db_info_for_ftm will be populated after this 
              function is called.
===========================================================================*/
void mcalwcdma_wenc_ftm_mdsp_edch_pre_config(e_ul_chan_action_enum_type action, e_tti_enum_type e_tti, uint8 num_carr, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_mdsp_edch_pre_callback

DESCRIPTION   This function is the pre-callback function for the EUL sync
              command in FTM mode. It fills the sync command with all the 
              parameters.
 
DEPENDENCIES  This function must be called as part of pre callback processing
              in the sync command interface
 
RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void mcalwcdma_wenc_ftm_mdsp_edch_pre_callback( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_mdsp_edch_post_callback_stop

DESCRIPTION   This function is the post-callback function for the EUL sync
              command in FTM mode when EUL stop is performed.  
 
DEPENDENCIES  This function must be called as part of post callback processing
              in the sync command interface
 
RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void mcalwcdma_wenc_ftm_mdsp_edch_post_callback_stop(uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_mdsp_edch_post_callback_start

DESCRIPTION   This function is the post-callback function for the EUL sync
              command in FTM mode when EUL start is performed.  
 
DEPENDENCIES  This function must be called as part of post callback processing
              in the sync command interface
 
RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void mcalwcdma_wenc_ftm_mdsp_edch_post_callback_start(uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_populate_next_tti_params

DESCRIPTION   This function sets up the modulator parameters for FTM mode 
              These are hard-coded parameters to allow RF Non signalling testing
 
DEPENDENCIES  mcalwcdma_wenc_ftm_eul_config_db must be populated before calling 
              this function 

RETURN VALUE  None

SIDE EFFECTS  FW - SW Interface will be updated with modulator paramteters
===========================================================================*/
void mcalwcdma_wenc_ftm_populate_next_tti_params(wl1_ul_carr_id_enum_type carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      mcalwcdma_wenc_ftm_eul_encoder_proc

DESCRIPTION   This function is the equivalent of the EUL FIQ in online mode 
              It populates encoder related parameters to micro-kernel and FW 
 
DEPENDENCIES  mcalwcdma_wenc_ftm_eul_config_db.next_tti_fw_param_ptr must be 
              populated before this ISR is enabled, ideally by a call to
              mcalwcdma_wenc_ftm_populate_next_tti_params

RETURN VALUE  None

SIDE EFFECTS  FW and micro-kernel will be populated with encoder parameters
===========================================================================*/
void mcalwcdma_wenc_ftm_eul_encoder_proc(void);
#endif /* FEATURE_WCDMA_BOLT_2_0 && !FEATURE_WCDMA_JOLOKIA_MODEM */
#endif //__MCALWCDMA_WENC_FTM_H__