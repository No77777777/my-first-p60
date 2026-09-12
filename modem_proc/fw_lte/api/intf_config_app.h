/*!
  @file
  intf_config_app.h

  @brief
  This file contains data structures for config app 
  requests and confirmations.
 
*/

/*===========================================================================

  Copyright (c) 2009-10 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/intf_config_app.h#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef INTF_CONFIG_APP_H
#define INTF_CONFIG_APP_H

#include "intf_common.h"
#include "lte_l1_types.h"
#include "lte_LL1_ue_types.h"

/*===========================================================================

      Constants

===========================================================================*/


/*===========================================================================

      Macros

===========================================================================*/
/*! @brief
  TDD Pipeline GCB(gain circular buffer) related constants used in config
  app req and conf.
 */
#define LTE_LL1_SYS_PIPELINE_BUFFER_INITIAL_GAIN_STATE 2
#define LTE_LL1_SYS_PIPELINE_BUFFER_ENTRY_BITWIDTH 3
#define LTE_LL1_SYS_PIPELINE_BUFFER_NUM_ENTRIES 10
#define LTE_LL1_SYS_PIPELINE_INVALID_PIPELINE_TIMING_REF 0xFFFFFFFF      

/*! @brief
  TDD Pipeline X2L idle related constants used in config app req and conf.
 */
#define LTE_LL1_TDD_PIPELINE_SCHEDULING_OFFSET 30600 ///< offset between TDD pipeline 
                                      ///< interrupt and DL subframe in
                                      ///< Ts units
#define LTE_LL1_TDD_PIPELINE_START_OFFSET_X2L_IDLE_GAP (1*30720-LTE_LL1_TDD_PIPELINE_SCHEDULING_OFFSET)
/*===========================================================================

      Typedefs

===========================================================================*/

// --------------------------------------------------------------------------
//
// sys & async config app
//
// --------------------------------------------------------------------------

/*! @brief
  TDD Pipeline AGC state structure

  */
typedef struct
{
  /*! Pipeline timing reference in RTC ticks % 10ms 
   
    The reference time of entry 0 in the pipeline_buffer
   */
  uint32 pipeline_timing_ref;


  /*! Each index represents the pipeline buffer for it's respective
    antenna.

    Buffer is essentially a compact array of pipeline state entries.
    The buffer will begin at the Least significant bit.  

    Each entry is of width LTE_LL1_SYS_PIPELINE_BUFFER_ENTRY_BITWIDTH 
    and the buffer contains LTE_LL1_SYS_PIPELINE_BUFFER_NUM_ENTRIES entries

    32       24       16        8        0
    |---------|--------|--------|--------|
        |9 |8  |7 |6 |5  |4 |3 | 2 |1 |0 |


    */
  uint32 pipeline_buffer[LTE_LL1_MAX_NUM_UE_RX_ANT]; 

} lte_LL1_pipeline_agc_state_s;

/*! @brief
    Idle mode 1 rx related information passed by ML1
    ML1 will enable the best Rx available
*/
typedef struct
{
   /* bit 0 for LTE_LL1_UE_RX_ANT_0 and bit 1 for LTE_LL1_UE_RX_ANT_1 */
   uint8  ant_en_bitmask;  
} lte_LL1_idle_one_rx_info_struct;

/*! @brief
  ARD CSF info structure
  */
typedef enum
{
  LTE_LL1_ARD_RX_GROUP_0_1     = 0,	// Physical Rx[0,1]
  LTE_LL1_ARD_RX_GROUP_0_2     = 1,	// Physical Rx[0,2]
  LTE_LL1_ARD_RX_GROUP_0_3     = 2,	// Physical Rx[0,3]
  LTE_LL1_ARD_RX_GROUP_1_2     = 3,	// Physical Rx[1,2]
  LTE_LL1_ARD_RX_GROUP_1_3     = 4,	// Physical Rx[1,3]
  LTE_LL1_ARD_RX_GROUP_2_3     = 5,	// Physical Rx[2,3]
  LTE_LL1_ARD_RX_GROUP_0_1_2_3 = 6,	// FW will calculate this blindly w/o reading the bit
  LTE_LL1_ARD_RX_GROUP_MAX     = LTE_LL1_ARD_RX_GROUP_0_1_2_3
} lte_LL1_sys_ard_valid_rx_group_enum;

typedef struct
{
  // Best 2 slection reference carrier. Used in case of SRx/SLNA group
  // where all the CCs within the group will have one reference
  // carrier. All the CCs will put a bias in best 2 decicion towards
  // reference carrier's best 2. Idea is to have same best 2 pair for
  // all CCs within SRx group
  /*TOdo: @viswa: cleanup */
  //lte_LL1_carrier_type_enum best2_selection_ref_carrier;

  // Valid Rx groups that need to be considered for best 2 eval
  // Its a bit mask where each bit corrosponds to a group given in
  // "lte_LL1_sys_ard_valid_rx_group_enum"
  // ***** FW will assert if this is set to 0 in EVAL MODE *****
  /*TOdo: @viswa: cleanup */
  //uint8 valid_rx_groups;

  // Last steady state physical map, used in
  // 1) EVAL mode during P/AP collision
  // 2) SPEFF Refresh
  // ***** This needs to be valid in every req *****
  uint8 csf_num_rx;
  lte_LL1_ue_rx_ant_enum csf_rx_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT];

  // A flag to indicate the start of best 2 EVAL
  boolean start_eval;

  /*Todo:@viswa: cleanup */

  // A flag to indicate if SPEFF based best 2 eval needs to be skipped
  // i.e. only send FTL SNR in the indication
  // ***** Checked only if start_best2_eval == TRUE *****
  boolean skip_speff_based_eval;
  /*TODO:@viswa - cleanup */
#if 0
  // A flag to indicate the CSF Speff refresh
  boolean speff_refresh;
#endif
} lte_LL1_sys_ard_csf_info_t;

/*TODO:@viswa - 'lte_LL1_sys_rxagc_params_struct' not needed for HA,
 * 'lte_LL1_sys_rxagc_info_struct' is modified accordingly. - cleanup */
#if 0
/*! @brief  Parameters used when enabling AGC */
typedef struct
{
  /* Rx Index for which the info is present */
  lte_LL1_ue_rx_ant_enum rx_ant_idx;

  /* RxAGC payload validity indicator
   - if TRUE during Rx enable request then ML1 provided values will be used
     for RxAGC on the enabled Rx else existing FW values will continue to be used
   - if TRUE during Rx disable confirmation indicates valid FW RxAGC state
     at time of Rx antenna disable (will be FALSE if RxAGC is disabled)*/

  /* NOTE: Currently this flag is not checked in config app so FW will blindly copy
           the AGC from the payload. This flag is checked only for ANT_MODE_CHNG */
  boolean is_valid;

  // DCC accum table validity indicator
  // - if TRUE for Rx enable request FW will use ML1 provided values else
  //   existing DCC table will be used
  // - if TRUE for Rx disable confirmation indicates valid FW RxAGC state
  //   at time of Rx antenna disable (will be FALSE if AGC is disabled)

  /* NOTE: This flag is checked for both config APP and ANT_MODE_CHNG
           if this flag is false, FW does not copy from PRx but works
           on what it has */
  boolean accum_table_valid;

  /* RX AGC start params */
  lte_LL1_rxagc_start_params_s rxagc_start_params;

  /* DCC accum table */
  lte_LL1_dcc_accum_cfg_table_s dcc_table;

} lte_LL1_sys_rxagc_params_struct;
#endif

/*TODO: @viswa: This struct definition captures agc params of 2rx.
 * - For HA expanded one is used for antenna mode change request - clean up */
#if 0
typedef struct
{
  // RxAGC mode, should be the same as primary RxAGC mode (probably more suited for
  // confirmation payload than request)
  lte_LL1_rxagc_mode_e rxagc_start_mode;

  /*Todo:@viswa: cleanup */
#if 0
  //boolean one_shot_pending;
#endif

  // Num of Rx for which info is stored in rx_agc_params[]
  uint8 num_rx;

  // Rx AGC parameters - max of (LTE_LL1_MAX_NUM_UE_RX_ANT)
  lte_LL1_sys_rxagc_params_struct rx_agc_params[LTE_LL1_MAX_NUM_UE_RX_ANT];

}lte_LL1_sys_rxagc_info_struct;
#endif

/*! @brief  Per Rx/rank action needed */
typedef enum
{
  // No action
  LTE_LL1_ACTION_NONE = 0,

  // Resume (timeout value is needed if its a resume)
  LTE_LL1_ACTION_RESUME,

  // Reset (Complete hard reset)
  LTE_LL1_ACTION_RESET,

  // Freeze (used by CSF to freeze rank SPEFF)
  LTE_LL1_ACTION_FREEZE

}lte_LL1_sys_reset_resume_action_e;

typedef struct
{
  // Action required per ***Logical*** Rx
  // Index 0 for PRx, 1 for DRx0 and so on.
  lte_LL1_sys_reset_resume_action_e action[LTE_LL1_MAX_NUM_UE_RX_ANT];

  // Timeout value per ***Logical*** Rx
  // Only used if action is RESUME
  uint32 timeout_value[LTE_LL1_MAX_NUM_UE_RX_ANT];

  // used to force DF HARD reset even though there is no rx map change
  // this is to switch to best 4rx out of 8Rx  ( FR60826)
  boolean force_ant_reset; /*TODO:@viswa check if this is needed ???  */


}lte_LL1_sys_rx_timeout_info_struct;

#if 0
typedef struct
{
  // Action required per Rank
  // Index 0: rank 1, Index 1: Rank 2 and so on
  lte_LL1_sys_reset_resume_action_e action[MAX_CSF_RANK];

  // Timeout value per Rank
  // Only used if action is RESUME
  uint32 timeout_value[MAX_CSF_RANK];

}lte_LL1_sys_rank_timeout_info_struct;
#endif

/*! @brief  config app request message structure.
            This request is issued to configure LTE app, which consists of
            the following controls:
            - module enabling/disabling/resuming
            - resource mode configuration
            - processing mode configuration             
            
            Enable restrictions:
            - sys time must be enabled in order to enable any other entity
            - MSTMR/WB sync cannot be enabled without sample recording enabled
            - TTL/FTL requires channel estimation to be enabled
            
            Mode change restrictions:
            - on-line/off-line transitions can only occur with stopped sample
              recording.
            - Only system time can be enabled in LTE_LL1_UE_RES_MODE_LTE_IDLE mode.
            - sample recording, AGC, TTL, FTL, channel est must be stopped upon
              entering LTE_LL1_UE_RES_MODE_LTE_IRAT mode
              
            Initial settings:
            Initial mode proc mode is on-line.
            Initial res mode is LTE_LL1_UE_RES_MODE_NO_LTE
            
            Sampling rate selection:
             for 20MHz BW, use sampling rate of 30.72MHz
              "  15MHz  "   "     "       "   " 30.72MHz
              "  10MHz  "   "     "       "   " 15.36MHz
              "  5MHz   "   "     "       "   "  7.68MHz
              
*/

/*! @brief
    IC cell freq erro info struct
    ML1 will use this field to provide IC cell frequency error information to FW
*/
typedef struct
{
   uint16   cell_id;        /* cell_id for this IC cell */
   int32    freq_err;       /* IC cell frequncy error : format Q6 */
} lte_LL1_ic_cell_freq_err_info_struct;

typedef struct
{                            
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
                            
  uint16  enable_mask;      ///< OR of lte_LL1_ue_rx_mod_e values 
                            ///< to indicate what entity(s) to be enabled

  uint16  disable_mask;      ///< OR of lte_LL1_ue_rx_mod_e values 
                             ///< to indicate what entity(s) to be disabled
                                                        
  uint16  resume_mask;      ///< OR of lte_LL1_ue_rx_resumable_mod_e values 
                            ///< to indicate what entity(s) to resume.  

  /*! @brief  Parameters used when enabling sample recording */
  lte_LL1_ue_wb_samp_rate_e wb_samp_rec_rate_sel;     ///< WB sampling rate select

  /*! @brief  Parameters used when recording into the secondary LTE sample server */
  lte_LL1_samp_serv_s       samp_serv;

  /*! @brief  Parameters used when enabling AGC */
  //lte_LL1_rxagc_mode_e    rxagc_start_mode;           ///< agc mode

  uint32                  two_pwr_33_over_dl_freq;    ///< 2^33/dl_freq
                                                      ///< Q-factor: Q19;
                                                      ///< It is needed becaue dl_carrier_freq
                                                      ///< does not have enough accuracy
  int32                   start_rot_freq_corr;        ///< Starting FTL frequency 
                                                      ///< correction at the rotator
                                                      ///< Q-factor: In 32S16
  int32                   start_vco_freq_corr;        ///< Starting VCO frequency 
                                                      ///< correction at the VCO
                                                      ///< Q-factor: In 32S16
  uint8   num_ic_cell;                                ///< number of IC cells for this serving cell
                                          ///< to be used to store information provided on
                                          ///<lte_LL1_ic_cell_freq_err_info_struct
  lte_LL1_ic_cell_freq_err_info_struct  ic_cell_freq_err[LTE_MAX_NUM_CRS_IC_CELLS];

  /*! @brief Parameters used for antenna correlation */
  uint16                  reporting_period;           ///< reporting period in ms units

  uint8                   slam_rot;                   ///< Slam the rotator 

/* Changes for online DRX aGc */
  uint8   agc_drx_step1_cnt;                         // Subframe count with FASt time constant
  uint8   agc_drx_step2_cnt;                         // Subframe count with Slow time constant
  uint8                   rxagc_start_lna_state[LTE_LL1_MAX_NUM_UE_RX_ANT];      ///< starting AGC LNA state
  uint8   num_rx_agc_params;
  //lte_LL1_rxagc_start_params_s  rxagc_start_params[LTE_LL1_MAX_NUM_UE_RX_ANT];  //< starting AGC param values

  uint8   reset_dcc_start_stop_time_flag; ///< Reset the DC accum START/STOP times (current
    // use case is DCC START/STOP time reset after RF SW api used for retune)

  boolean one_shot_dvga;                              ///< Flag to enable one-shot DVGA

  lte_l1_cell_duplex_e        duplexing_mode;          //< FDD/TDD indication before the Scell is configured 
                                                       // (should be same as Scell after camped on it)
  uint32 tdd_x2l_vcell_frame_boundary;                 // location of the VCell frame_boundary %307200 for TDD X2L idle AGC
  boolean                     rf_wakeup_mode_enabled;  ///< RF chip (RTR/WTR) wakeup mode is enabled
  lte_LL1_rf_wakeup_rx_on_e   rf_wakeup_rx_on;         ///< 1: turn on Rx, 0: turn off Rx via GRFC; 
                                                       ///< Note: only applicable if rf_wakeup_mode_enabled is 1.
  lte_LL1_pipeline_agc_state_s pipeline_agc_state;   ///< pipeline agc ref time and GCB buffer 
  
  /* DCC accum table; Only when (accum_table_valid is set AND the rxagc is enabled),
    the table is taken by the LL1 */
  //boolean accum_table_valid;
  /*! @brief DCC accum value table; one for each rx chain*/
  //lte_LL1_dcc_accum_cfg_table_s dcc_table[LTE_LL1_MAX_NUM_UE_RX_ANT];

  boolean                       is_intra_band_ca;		   ///< flag indicating whether this is intra ca mode.
  lte_LL1_carrier_type_enum     rxagc_share_carrier_lna;   ///< used to signal to a carrier to share the LNA from another carrier 

  lte_LL1_ue_wb_df_mode_e wb_df_mode;     ///< WB DF receiver operation mode
  uint32                        shared_rf_chain_bitmap; ///< bit-field indicating if the RF chain is shared with another carrier/cell
                                                /// based on lte_LL1_carrier_type_enum
                                                /// At Minimum , the bit mask for the current carrier should be set.
                                                /// If more than 1 bit is set, FW will process the carriers corresponding to  
                                                /// the set bit as single Rx with the corresponding carriers.
                                                /// valid bitwidth = LTE_LL1_CARRIER_COUNT
                                               
  uint16 init_doppler_hz;

  lte_LL1_usleep_mode_e mode;  

  lte_LL1_idle_one_rx_info_struct idle_1rx_data;

  /* ARD API */
  uint8 num_rx_to_enable; /// Num of RX to be enabled (max 2)
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT]; /// Mapping table for physical to logical_rx[4] ...
                                                                       /// {1,3,0,2} means Rx1 is PRx, RX3 is DRx0 and Rx2 is DRx2
                                                                       /// if with above mapping, num_rx_to_enable is 2, FW will enable Rx1 and 3
                                                                       /// Rx1 being the PRx and Rx3 being the DRx0

  /*TODO: We already have provisioning for AGC info for both antennas as part of below.
   * - lte_LL1_rxagc_mode_e    rxagc_start_mode;
   * - lte_LL1_rxagc_start_params_s  rxagc_start_params[LTE_LL1_MAX_NUM_UE_RX_ANT];
   * - boolean accum_table_valid;
   * - lte_LL1_dcc_accum_cfg_table_s dcc_table[LTE_LL1_MAX_NUM_UE_RX_ANT]; */
  //lte_LL1_sys_rxagc_info_struct rx_agc_info;  /// AGC info passed by ML1, has per Rx LNA/DVGA?DC etc.
  
  /* unwrapped 'lte_LL1_sys_rxagc_info_struct' - to make it compatable with existing code 
   *  - All missing params of 'lte_LL1_sys_rxagc_info_struct' which are missing in config_app req are added here.
   *  - All agc params, existing & new ones are are co located at one place.
   *  - TODO: this is done to avoid making too many changes. Clean up when time permits.
   *  - This is also done to have uniformity with agc params between config app & antenna mode change request.
   * */
  /* Antenna logical index */
  lte_LL1_ue_rx_ant_enum rx_ant_idx[LTE_LL1_MAX_NUM_UE_RX_ANT];
  boolean is_valid[LTE_LL1_MAX_NUM_UE_RX_ANT];
  /*! @brief  Parameters used when enabling AGC */
  //lte_LL1_rxagc_mode_e rxagc_start_mode[LTE_LL1_MAX_NUM_UE_RX_ANT];
  lte_LL1_rxagc_mode_e    rxagc_start_mode; /* same for both antenna */          ///< agc mode
  // Starting RxAGC param values (DVGA, LNA, RSSI etc)
  lte_LL1_rxagc_start_params_s  rxagc_start_params[LTE_LL1_MAX_NUM_UE_RX_ANT];  //< starting AGC param values

  /* DCC accum table; Only when (accum_table_valid is set AND the rxagc is enabled),
    the table is taken by the LL1 */
  boolean accum_table_valid; /* Not used by FW. */
  /*! @brief DCC accum value table; one for each rx chain*/
  lte_LL1_dcc_accum_cfg_table_s dcc_table[LTE_LL1_MAX_NUM_UE_RX_ANT];


  lte_LL1_sys_rx_timeout_info_struct rx_timeout_info; /// Timeout info per Rx
  //lte_LL1_sys_rank_timeout_info_struct rank_timeout_info;  /// Timeout info per Rank
  lte_LL1_sys_ard_csf_info_t csf_info; /// CSF info for the ARD
} lte_LL1_sys_config_app_carrier_payload_t;

typedef struct {

  boolean valid;
  uint16 frame_num;
  uint16 subframe_num;
  int16 rlm_snr;         ///< RLM SNR (log domain in 16Q8)

} lte_LL1_csf_rlm_snr_state_s;

typedef struct
{
  lte_LL1_nb_id_enum nb_id;  ///< Enum for NB id mapping
  
  lte_LL1_nb_operation_type operation_type; ///< Enum for operation type

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for carriers to be supported for CA

}lte_LL1_sys_nb_mux_settings_t;


typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for carriers to be supported for CA

  uint8  hw_res_release_mask;  ///< Mask for resources to be released for this carrier, from lte_LL1_resource_e

  uint8  hw_res_acquire_mask; ///< Mask for resources to be released for this carrier, from lte_LL1_resource_e

  lte_LL1_harq_state_s harq_state;	// enum of HARQ states. 3 possibilities. No change in EMEM/HARQ state. Release EMEM/HARQ. Acquire EMEM/HARQ

} lte_LL1_sys_hw_res_req_carrier_t;

typedef struct
{
  uint8 num_carriers; ///< Number of carriers to be configured.

  lte_LL1_sys_hw_res_req_carrier_t hw_res_req[LTE_LL1_DL_NUM_CARRIERS]; ///< HW Resource rel/acq info per carrier.

} lte_LL1_sys_hw_res_req_payload_t;

/*Enum to indicate if fw should release or acquire 
  rxfe chains -- > right now used ONLY for IRAT*/
typedef enum
{
   LTE_RXFE_DEFAULT =  0,  ///< Default value
   LTE_RXFE_ACQUIRE = 26,  ///< Indicates fw to release chains during l2l/l2x gap enter
   LTE_RXFE_RELEASE = 52   ///< Indicates fw to acquire chains during l2l/l2x gap exit
} lte_LL1_rxfe_acquire_release_e;

/* ONLY for IRAT(l2x,l2l,x2l) 
 Used for releasing/acquiring rxfe only for carriers that
 would be impacted in the gap*/
typedef struct
{
  lte_LL1_rxfe_acquire_release_e mode;
  uint8 num_handles_impacted;
  uint8 handle[LTE_LL1_MAX_NUM_UE_RX_ANT];
} lte_LL1_rxfe_acquire_release_t;

typedef struct
{
  lte_LL1_ue_res_mode_e   resource_mode;              ///< resource allocation mode

  lte_LL1_ue_proc_mode_e  proc_mode;                  ///< processing mode
                                                      ///< (on-line or off-line)
                   
  lte_LL1_ue_mode_sys_e sys_mode;              ///< start, resume, stop (used to be LTE_LL1_UE_MOD_SYS_TIME) 

  lte_LL1_ue_df_enable_mode_e enable_mode;            ///< demfront enabling mode

  /*! @brief  Parameters used when enabling/resuming system time */
  lte_l1_cell_systime_s  sys_time_start_cell_time;    ///< starting cell time
  
  uint32                 sys_time_resume_elapsed_time;///< elapse time in Ts units for
                                                      ///< use with sys time resume
													  
  boolean                 not_ftm_mode_flag; ///< FTM mode indication (for RFCal)
  // NOTE: ML populates this with NOT_FTM=1 ALWAYS. RFSW implicitly sets FTM=1 by memsetting
  //CFG_APP payload to 0 

  lte_LL1_gap_type_e      gap_type;

  boolean dyn_odrx_optimizations_enable; ///<1/0: Ena/Dis for Dyn ODRX optimizations 
  
  uint8 num_nbs;
  lte_LL1_sys_nb_mux_settings_t  nb_switch_params[LTE_LL1_NB_ID_COUNT];

  boolean                       dual_demod_is_active; //enable in SCC_0 to enter into dual demod mode
  lte_LL1_dual_demod_map_struct dual_demod_mapping; //mapping setting when dual demod is enabled

  lte_LL1_sys_hw_res_req_payload_t hw_res_req_params;

  lte_LL1_rxfe_acquire_release_t rxfe_acquire_release_params; ///< Indicate if fw should release or acquire 
                                                              ///< rxfe chains -- > right now used ONLY for IRAT

  boolean                 retain_harq_mem; ///< If TRUE, do not reset HARQ

  boolean                 do_rfwakeup_status_check; ///<ML1 tells FW whether delayed RF wakeup status check and due recovery should be done or not>
  /* ARD related flag 
   * Notes: 
   * problem statement: In ARD SRX case, during SCC activation if PCC is in 1rx state & as per ARD algo SCC should start with 2rx.
   * Since this is SRX case, if SCC has to be activated in 2rx then PCC has to be brought back to 2rx. 
   * ML1 has timeline limitation moving PCC to 2rx before sending SCC activation in 2rx.
   * In SRX case, both carriers should have same number of rx enabled.   
   * Fix: To address this it is agreed upon between ML1, FW & systems to have following work around 
     - ML1 will send sys config app request for SCC only during SCC activation. 
     - FW has to implicitly move PCC to 2rx state & do SCC activation there after. 
     - To move PCC to 2rx 'implicit2Rx' flag is set and PCC related carrier params are shared with fw
     - PCC params which are shared with fw. 
       lte_LL1_carrier_type_enum carrier_type;
       uint8   rxagc_start_lna_state[LTE_LL1_MAX_NUM_UE_RX_ANT];      ///< starting AGC LNA state
       uint8   num_rx_agc_params;
       uint8 num_rx_to_enable; /// Num of RX to be enabled (max 2)
       lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT]; /// Mapping table for physical to logical_rx[4] ...
       lte_LL1_ue_rx_ant_enum rx_ant_idx[LTE_LL1_MAX_NUM_UE_RX_ANT];
       boolean is_valid[LTE_LL1_MAX_NUM_UE_RX_ANT];
       lte_LL1_rxagc_mode_e    rxagc_start_mode;        
       lte_LL1_rxagc_start_params_s  rxagc_start_params[LTE_LL1_MAX_NUM_UE_RX_ANT];  //< starting AGC param values
       boolean accum_table_valid;
       lte_LL1_dcc_accum_cfg_table_s dcc_table[LTE_LL1_MAX_NUM_UE_RX_ANT];
       lte_LL1_sys_rx_timeout_info_struct rx_timeout_info; /// Timeout info per Rx
   
     Ex: config_req.carrier[0] => Has SCC related params to activate SCC.
         config_req.carrier[1] => Has PCC related params to implicity move PCC to 2rx state. 
  */
  boolean                 implicit2Rx;
}lte_LL1_sys_config_app_common_payload_t;

/*! @brief Config APP struct for CA

*/
typedef struct
{
  lte_LL1_req_header_struct req_hdr;            
  uint8 num_carriers; ///< Number of carriers to be configured.
  lte_LL1_sys_config_app_common_payload_t common;   ///<  Common request parameters
  lte_LL1_sys_config_app_carrier_payload_t carrier[LTE_LL1_DL_NUM_CARRIERS]; ///< parameters per CA.
#ifdef FEATURE_MODEM_ANTENNA_SWITCH_DIVERSITY
  lte_LL1_ant_setting_enum_t  ant_setting;             ///< ant setting
#endif
}
lte_LL1_sys_config_app_req_struct;

LTE_LL1_INTF_MSG( sys_config_app_req );

#ifndef SIM_PLATFORM
/*! @brief  FTL SNR structure.
            
*/
typedef struct {                      
  
  boolean ftl_enabled;  /* TRUE: FTL enabled; FALSE: FTL disabled */

  boolean valid; /// TRUE if FTL SNR info is valid

  uint32 ftl_snr[LTE_LL1_MAX_NUM_UE_RX_ANT];         ///< FTL SNR (linear in 32Q16)

  uint8 num_rx_enabled; //
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT]; //

} lte_LL1_rxftl_snr_state_s;
#endif

/*! @brief  Configure app confirmation message structure.
            This message is sent after LL1 starts the specified entities
            in the configure rx start request message.
*/
typedef struct {                      
  
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

  uint16  ref_sfn;                        ///< SFN of the last radio frame 
                                          ///< corresponding to passed frame ref time

  uint64  ref_time;                       ///< 64 bit RTC time of last radio frame
                                          ///<  start corresponding to ref_sfn above 
                                          ///<  i.e. pair [ref time, SFN]

  int32   rot_freq_correction;            ///< Most recent frequency correction at the rotator
                                          ///< in Q12. Divide by 2^12 to get value in Hz.

  int32   vco_freq_correction;            ///< Most recent VCO frequency correction in Q12.
                                          ///< Divide by 2^12 to get value in Hz.
  
  uint8   lna_state;                      ///< most recent LNA state  
  lte_LL1_rxagc_start_params_s  rxagc_curr_params[LTE_LL1_MAX_NUM_UE_RX_ANT];  ///< current AGC param values
  lte_LL1_pipeline_agc_state_s pipeline_agc_state; ///< pipeline agc ref time and GCB buffer 

   /*! @brief DCC accumulation report; one for each rx chain*/
   lte_LL1_dcc_accum_cfg_table_s dcc_table[LTE_LL1_MAX_NUM_UE_RX_ANT];

  uint64  last_doppler_updated_rtc;       ///< RTC count when Doppler was last updated in FW
  uint16  last_doppler_updated_hz;
  boolean last_doppler_rtc_valid;
  uint16  subframe_frame_ss_on;           ///SS On SF/SFN: bits 0~3  = subframe number, bits 4~13 = frame number


  #ifndef SIM_PLATFORM
  lte_LL1_rxftl_snr_state_s ftl_snr;      ///FTL SNR
  #endif
  uint8 num_rx_enabled; //
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT];

}lte_LL1_sys_config_app_cnf_carrier_payload_struct;

typedef struct {
  lte_LL1_cnf_header_struct cnf_hdr;      ///< common cnf header
  uint8 num_carriers; ///< Number of carriers to be configured.
  lte_LL1_sys_config_app_cnf_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS]; ///< Cfg_app parameters per CA.
  int16  subframe_num;                   ///< subframe_num when sending config_app_cnf
}lte_LL1_sys_config_app_cnf_struct;

LTE_LL1_INTF_MSG( sys_config_app_cnf );


// --------------------------------------------------------------------------
//
// Async config app structures (for use in async message queue)
//
// --------------------------------------------------------------------------
typedef lte_LL1_sys_config_app_req_struct  lte_LL1_async_config_app_req_struct;

LTE_LL1_INTF_MSG( async_config_app_req );

typedef lte_LL1_sys_config_app_cnf_struct  lte_LL1_async_config_app_cnf_struct;

LTE_LL1_INTF_MSG( async_config_app_cnf );



// XXX alias to older name
typedef lte_LL1_sys_config_app_req_struct  lte_LL1_sys_config_rx_req_struct;

LTE_LL1_INTF_MSG( sys_config_rx_req );

typedef lte_LL1_sys_config_app_cnf_struct  lte_LL1_sys_config_rx_cnf_struct;

LTE_LL1_INTF_MSG( sys_config_rx_cnf );


#endif

