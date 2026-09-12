/*!
  @file
  intf_sys.h

  @brief
  LL-ML interface message definitions for sys request queue

  @detail
  description of functions in this file
  
  to do:
  - change general config request
 
*/

/*===========================================================================

  Copyright (c) 2008-11 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/intf_sys.h#5 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/11   isalman Don't use q6 headers when compiling for UMPLT.
03/03/11   nbb     Added RF script + RXLM config message
07/15/09   anm     Added DL earfcn to sys serving cfg and category to ue cfg
===========================================================================*/
#ifndef INTF_SYS_H
#define INTF_SYS_H

#include "intf_common.h"
#include "intf_sys_msg.h"
#include "intf_config_app.h"
#include "intf_sleep_wakeup.h"
#include "intf_recovery.h"
#include "msgr.h"
#include "lte_LL1_log_srch.h"
#include "lte_LL1_log_ul.h"
#include "lte_LL1_log_demfront.h"
#include "lte_LL1_log_csf.h"
#include "lte_LL1_log_uers.h"
#include "lte_LL1_log_rxfe.h"
#include "lte_LL1_log_rlm.h"
#include "lte_LL1_log_prs.h"
#include "lte_LL1_log_lted.h"
#include "lte_LL1_ue_types.h"
#include "cxm_intf_types.h"

#ifndef SIM_PLATFORM
#include "intf_config_app.h"
#endif

/*===========================================================================

      Constants

===========================================================================*/
//for RX chain diversity feature.
#define LTE_LL1_RXLM_SCRIPT_REQ_INIT_NOTNEEDED 0x8
#define LTE_LL1_RXLM_SCRIPT_REQ_ACTION_DISABLE 0x4
#define LTE_LL1_RXLM_SCRIPT_REQ_RX1_APPLY      0x2
#define LTE_LL1_RXLM_SCRIPT_REQ_RX0_APPLY      0x1

//Threshold for PCC and SCC cell ref time difference
#define LTE_LL1_PCC_SCC_SYNC_THRESHOLD	1076	//~35us (32us as per the spec + 3us margin at time slam)
#define LTED_LL1_INVALID_DRID_ID			(-1)

/*===========================================================================

      Macros

===========================================================================*/



/*===========================================================================

      Typedefs

===========================================================================*/


// --------------------------------------------------------------------------
//
// Config serving cell
//
// --------------------------------------------------------------------------

/*! @brief  Configure serving cell request message structure.
            This request configures static serving cell parameters in LL1
            for use by downlink and uplink.  This request should not be 
            issued when DL and/or UL has already been enabled.
*/
typedef struct
{
  lte_LL1_req_header_struct   req_hdr;          ///< common request header
  
  uint16                      ul_dl_freq_ratio; ///< UL to DL carrier frequency 
                                                ///< ratio in Q15
  
  lte_num_rb_idx_enum         num_dl_rb_idx;    ///< DL number of RBs
  
  lte_num_rb_idx_enum         num_ul_rb_idx;    ///< DL number of RBs
  
  uint8                       num_dl_harq;      ///< # of DL HARQ processes 
                                                ///< (up to 8 for FDD, up to 15 for TDD)
  
  uint8                       num_ul_harq;      ///< # of UL HARQ processes (up to 8)
  
  lte_earfcn_t                dl_earfcn;        ///< DL EARFCN value
                                                
  lte_l1_frame_struct_e       frame_type;       ///< frame structure type
  
  lte_l1_cell_duplex_e        duplexing_mode;   ///< duplexing mode
  
  lte_l1_cp_e                 cp_mode;          ///< DL CP configuration

  lte_l1_cp_e                 ul_cp_mode;       ///< UL CP configuration
  
  lte_phy_cell_id_t           cell_id;          ///< cell ID (0 ~ 503)
  
  int8                        eNodeB_num_tx_ant;///< cell # of tx ant (1 ~ 4)

} lte_LL1_sys_config_serving_cell_req_struct;

LTE_LL1_INTF_MSG( sys_config_serving_cell_req );



/*! @brief  Configure serving cell confirmation message structure.
            This message is sent from LL1 in response to config_serving_cell_req
            message.  
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;            ///< common cnf header
  
} lte_LL1_sys_config_serving_cell_cnf_struct;

LTE_LL1_INTF_MSG( sys_config_serving_cell_cnf );

// ==========================================================================
// XXX the following are structures for updated serving cell config
//     messages that will support TDD & MBSFN awareness.
//     These messages are not supported yet.  
// ==========================================================================

// --------------------------------------------------------------------------
//
// scell static config
//
// --------------------------------------------------------------------------
/*! @brief  Serving cell static configuration request message structure.
            The scell static config request is used to configure 
            serving cell parameters obtained from initial acquisition. 
             
            This request must be issued before enabling DL.
            This request should not be issued once DL has been enabled.
            
            Semi-static parameters are not altered with this message.
            Semi-static config request should be issued to ensure
            validity of their parameters. 
            
*/
typedef struct
{
   boolean nlic_possibility;
   lte_LL1_carrier_type_enum rx_victim_carrier_id;
   lte_LL1_carrier_type_enum tx_aggr_carrier_id;
   lte_bandwidth_enum rx_victim_bw;
   lte_bandwidth_enum tx_aggr_bw;

}lte_LL1_sys_scell_stat_nlic_config_struct;

typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

  lte_phy_cell_id_t           cell_id;          ///< cell ID (0 ~ 503)
  
  uint8                       scell_idx;         ///< SCC id (0-7)	
  
  lte_earfcn_t                dl_earfcn;        ///< DL EARFCN value
  
  lte_l1_cp_e                 dl_cp_mode;       ///< DL CP configuration
  
  lte_l1_frame_struct_e       frame_type;       ///< frame structure type
  
  lte_l1_cell_duplex_e        duplexing_mode;   ///< duplexing mode
  
  lte_num_rb_idx_enum         num_dl_rb_idx;    ///< DL number of RBs index
  
  uint8                       num_eNb_tx_ant;   ///< number of eNb tx ant (1,2 or 4)
  boolean                     dual_demod_is_configured; ///< for demback use to skip config SCC if dual_demod_is_configured=TRUE
  boolean                     no_ic_reset;  ///< flag to indicated if FW should reset IC info when receiving scell_stat_cfg from ML1: if TRUE, IC info should not be reset

} lte_LL1_sys_scell_stat_config_req_carrier_payload_struct;

typedef struct
{
  lte_LL1_req_header_struct   req_hdr;          ///< common request header
  
  uint8 num_carriers;                           ///< Number of carriers to be configured.

  lte_LL1_sys_scell_stat_nlic_config_struct nlic_config; ///<NLIC info needed for image loading

  lte_LL1_sys_scell_stat_config_req_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.

} lte_LL1_sys_scell_stat_config_req_struct;

LTE_LL1_INTF_MSG( sys_scell_stat_config_req );


/*! @brief  scell static config confirmation message structure.
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
  uint8 num_carriers;                           ///< Number of carriers to be configured.

  lte_LL1_carrier_type_enum carrier_type[LTE_LL1_DL_NUM_CARRIERS ]; ///< Enum for dl carriers in  CA
  
} lte_LL1_sys_scell_stat_config_cnf_struct;

LTE_LL1_INTF_MSG( sys_scell_stat_config_cnf );

/*! @brief  scell stat deconfig req message structure.
*/

typedef struct
{

  lte_LL1_req_header_struct   req_hdr;          ///< common request header
  
  uint8 num_carriers;                           ///< Number of carriers to be deconfigured.

  lte_LL1_sys_scell_stat_nlic_config_struct nlic_config;

  lte_LL1_carrier_type_enum carrier_type[LTE_LL1_DL_NUM_CARRIERS ]; ///< Enum for dl carriers in  CA
  
} lte_LL1_sys_scell_stat_deconfig_req_struct;

LTE_LL1_INTF_MSG( sys_scell_stat_deconfig_req );

/*! @brief  scell stat deconfig confirmation message structure.
*/

typedef struct
{

  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
  uint8 num_carriers;                           ///< Number of carriers to be deconfigured.

  lte_LL1_carrier_type_enum carrier_type[LTE_LL1_DL_NUM_CARRIERS ]; ///< Enum for dl carriers in  CA
  
} lte_LL1_sys_scell_stat_deconfig_cnf_struct;

LTE_LL1_INTF_MSG( sys_scell_stat_deconfig_cnf );


typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in CA

  uint8  release_mask;  ///< Mask for resources to be released for this carrier, from lte_LL1_resource_e

  uint8  acquire_mask; ///< Mask for resources to be released for this carrier, from lte_LL1_resource_e

  lte_LL1_harq_state_s harq_state; // enum of HARQ states. 3 possibilities. No change in EMEM/HARQ state. Release EMEM/HARQ. Acquire EMEM/HARQ
} lte_LL1_sys_res_rel_acq_req_carrier_payload_struct;



/*! @brief  Release/Acquire resources request message structure.
            This request instructs FW to release/acquire the specified resources per carrier.
*/
typedef struct
{
  lte_LL1_req_header_struct   req_hdr;        ///< common request header

  uint8 num_carriers;                         ///< Number of carriers to be configured.

  lte_LL1_sys_res_rel_acq_req_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.
} lte_LL1_sys_res_rel_acq_req_struct;

LTE_LL1_INTF_MSG( sys_res_rel_acq_req );

/*! @brief  Release/Acquire resources request confirmation structure.
*/
typedef struct
{
  lte_LL1_cnf_header_struct   cnf_hdr;        ///< common cnf header
  uint8 num_carriers;

 lte_LL1_carrier_type_enum carrier_type[LTE_LL1_DL_NUM_CARRIERS ]; ///< Enum for dl carriers in  CA

} lte_LL1_sys_res_rel_acq_cnf_struct;

LTE_LL1_INTF_MSG( sys_res_rel_acq_cnf );

// --------------------------------------------------------------------------
//
// scell semi-static config
//
// --------------------------------------------------------------------------
/*! @brief  Serving cell semi-static configuration request message structure.
            The scell semi-static config request is used to configure 
            parameters from SIB.  This request can be issued once DL has
            been enabled.  
            
            If the value for a parameter is not known, ML can just specify
            a valid value for that parameter.  Some parameters will have
            an unknown enum for this situation.
            
            XXX add parameters for MBSFN
*/

#define LTE_LL1_MBSFN_ALLOCATION_INFO_SIZE      8

typedef enum
{
  LTE_LL1_MBSFN_CFG_UNKNOWN = 0,
  LTE_LL1_MBSFN_CFG_KNOWN
} lte_LL1_mbsfn_cfg_type_enum;

typedef struct
{
  lte_LL1_mbsfn_cfg_type_enum  cfg_type;  //Known -> MBSFN config from network; Unknown -> Dummy UE mbsfn config

  boolean is_enabled;           ///< flag to indicate if mbsfn cfg is enable or disable.
                                                                                        ///< Default is disabled if config is not present 
  
  uint8 num_allocations;        ///< Number of MBSFN frame allocations 
  
  lte_l1_mbsfn_allocation_s allocation_info[LTE_LL1_MBSFN_ALLOCATION_INFO_SIZE];
                                                                                                ///< MBSFN frame allocation info 
} lte_LL1_mbsfn_cfg_s;

typedef struct
{

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  uint8                       num_dl_harq;          ///< # of DL HARQ processes 
                                                    ///< (up to 8 for FDD, up to 
                                                    ///< 15 for TDD)
                                                    
  boolean                     ul_cell_enabled;      ///< Set to true for PSC, false for SSC.
  
  uint8                       num_ul_harq;          ///< # of UL HARQ processes 
                                                    ///< (up to 8)
                                                    ///< (not currently used)
  
  lte_num_rb_idx_enum         num_ul_rb_idx;        ///< UL number of RBs index
  
  lte_l1_cp_e                 ul_cp_mode;           ///< UL CP configuration

  lte_earfcn_t                ul_earfcn;            ///< UL EARFCN value
  
  uint16                      ul_dl_freq_ratio;     ///< UL to DL carrier frequency 
                                                    ///< ratio in Q15
                                               
  lte_l1_tdd_ul_dl_cfg_index_e  ul_dl_cfg_index;    ///< uplink-downlink config      
  
  lte_l1_tdd_special_subframe_pattern_e         spec_subf_cfg_index;  
                                                    ///< special subframe config index
                               
  lte_LL1_mbsfn_cfg_s           mbsfn_cfg;            ///< mbsfn configuration

  uint8                       mod_256QAM_enabled;   ///< 256QAM mode enabled

  boolean                      tx_ant_sel_configured;

  lte_l1_tdd_ul_dl_cfg_index_e  ul_ref_ul_dl_cfg_index;    ///< UL reference UL-DL config      
                                                    
  lte_l1_tdd_ul_dl_cfg_index_e  dl_ref_ul_dl_cfg_index;  
  uint8                         pucch_list_size;            ///< pucch list size for TPC pruning  ///< DL reference UL-DL config         
  boolean                       highSpeedFlag;           /// ML1 sends this flag based on HST flag in SIB2 message
  boolean                       alt_tbs_enabled; //alt TBS feature

} lte_LL1_sys_scell_semi_stat_config_req_carrier_payload_struct;


typedef struct
{
  lte_LL1_req_header_struct   req_hdr;          ///< common request header  
  
  uint8 num_carriers;                           ///< Number of carriers to be configured.

  lte_LL1_sys_scell_semi_stat_config_req_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.
    
} lte_LL1_sys_scell_semi_stat_config_req_struct;

LTE_LL1_INTF_MSG( sys_scell_semi_stat_config_req );


/*! @brief  scell static config confirmation message structure.
*/
typedef struct {                      
  
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in CA

  uint32 eib_base_addr; ///< Encoder Base address

} lte_LL1_sys_scell_semi_stat_config_cnf_carrier_payload_struct;

typedef struct {
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
  uint8 num_carriers; ///< Number of carriers to be configured.

  lte_LL1_sys_scell_semi_stat_config_cnf_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.
  
} lte_LL1_sys_scell_semi_stat_config_cnf_struct;

LTE_LL1_INTF_MSG( sys_scell_semi_stat_config_cnf );


// --------------------------------------------------------------------------
//
// Config UE
//
// --------------------------------------------------------------------------

/*! @brief  Configure UE request message structure.
            This request configures global UE parameters.  Configuration
            takes effect at specified action time.  
*/
typedef struct
{
  lte_LL1_req_header_struct   req_hdr;        ///< common request header  
  
  int8                        num_rx_ant;     ///< # of rx ant (valid values are 2 & 4, 
                                              ///<   Odyseey only supports 2)
  int8                        num_tx_ant;     ///< # of tx ant (should be 1)

  uint8                       category;       ///< UE category ( Range: 1- 5)

  uint8                       max_num_dl_carriers;     ///< Maximum # of DL carriers

  uint8                       max_num_ul_carriers;     ///< Maximum # of UL carriers
  
  uint8                       chip_id;       ///<  0 = Not valid, 1=8920, can add other Chip ID if required
  
} lte_LL1_sys_config_ue_req_struct;

LTE_LL1_INTF_MSG( sys_config_ue_req );


/*! @brief  Configure UE confirmation message structure.
            This message is sent from LL1 in response to config_ue_req message.
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
} lte_LL1_sys_config_ue_cnf_struct;

LTE_LL1_INTF_MSG( sys_config_ue_cnf );


// --------------------------------------------------------------------------
//
// Timing slam
//
// --------------------------------------------------------------------------

/*! @brief  Slam timing request message structure.
            This request changes LL1 system time to align with the specified
            reference time.  
            
            On Odyssey, this request can be issued while uplink and/or 
            downlink channels are enabled.  
                        
*/
typedef struct {  
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

  int64                     ref_time;         ///< reference time at start of 40ms frame
  
  int16                     ref_frame_num;    ///< 10ms frame number at ref_time
  
  boolean                   slam_frame_only;  ///< slam frame number only
                                              ///< (subframe # & timing remains
                                              ///< unchanged)
                                              
  int64                     scell_ref_time_delta; ///< The reference time difference between Scell and Pcell.
                                                  ///< Range -32 to 32 us

  boolean                   reset_ttl;            ///< Reset TTL when the slam request is received when TTL is enabled
} lte_LL1_sys_slam_timing_req_carrier_payload_struct;

typedef struct {  
  lte_LL1_req_header_struct req_hdr;            ///< common req header

  uint8 num_carriers;                           ///< Number of carriers to be configured.

  lte_LL1_sys_slam_timing_req_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.

} lte_LL1_sys_slam_timing_req_struct;

LTE_LL1_INTF_MSG( sys_slam_timing_req );


/*! @brief  Slam timing confirmation message structure.
            This confirmation is sent to indicate the requested timing slam 
            has completed.  Subframe indications sent after this confirmation 
            are based on the new slammed timing.
*/
typedef struct {                      
  
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  
  lte_LL1_sys_time_struct   next_sf;          ///< subframe # and 10ms
                                              ///< frame # of next subframe
                                              ///< after timing slam
  
} lte_LL1_sys_slam_timing_cnf_carrier_payload_struct;

typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;            ///< common cnf header
  
  uint8 num_carriers;                           ///< Number of carriers.

  lte_LL1_sys_slam_timing_cnf_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per CA.
  
} lte_LL1_sys_slam_timing_cnf_struct;

LTE_LL1_INTF_MSG( sys_slam_timing_cnf );


// --------------------------------------------------------------------------
//
// TA adjustment
//
// --------------------------------------------------------------------------

/*! @brief  TA adjustment request message structure.
            This requests changes the UL - DL subframe offset to the
            specified adjustment value.  If this request is processed
            for DL subframe x, UL subframe x+2 will have the newly
            specified offset.  
            
            Note: there may be some limiting in the rate of timing
            change on the uplink.
            
*/
typedef struct {                    

  lte_LL1_req_header_struct req_hdr;          ///< common req header
  
  boolean is_adjustment_abs;                    ///< flag indicating whether the TA value is absolute (11bit) or relative (6bit)
  boolean save_current_adjustment;
  boolean revert_to_saved_adjustment;
  int16 adjustment;                           ///< Timing advnace value sent by eNB in TA units 
  uint8 ul_carier_idx;
#ifdef TA_ALIGN_THOR_API_20
  boolean apply_forced_ta;
  uint8 reference_carrier_idx;
#endif

} lte_LL1_sys_ta_adj_req_struct;

LTE_LL1_INTF_MSG( sys_ta_adj_req );


/*! @brief  TA adjustment confirmation message structure.
            This confirmation is sent to indicate that the TA adjustment
            request has been processed.
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  uint8                     ul_carrier_idx;   ///< confirmation for SCELL
} lte_LL1_sys_ta_adj_cnf_struct;

LTE_LL1_INTF_MSG( sys_ta_adj_cnf );


// --------------------------------------------------------------------------
//
// Timing adjustment
//
// --------------------------------------------------------------------------

/*! @brief  Timing adjustment request message structure.
            This request adjusts the downlink subframe timing by a specified
            amount.  Adjustment to uplink will occur at a limited rate to 
            maintain specified TA offset between UL and DL subframes.
            
            Note: this request is not normally used.  Timing adjustment from
            TTL occurs without requiring ML SW to issue this message.
            
            This message is not supported on Odyssey
*/
typedef struct {                    

  lte_LL1_req_header_struct req_hdr;          ///< common req header
  
  int16 whole_sample_adjustment;              ///< timing adj in units of 1/30.72MHz 
                                              ///< (neg = advance)
                                              
  int16 fract_sample_adjustment;              ///< timing retard in units of 
                                              ///< 1/(3*30.72MHz) Valid values are 0~2
  
} lte_LL1_sys_adj_timing_req_struct;

LTE_LL1_INTF_MSG( sys_adj_timing_req );


/*! @brief  Timing adjustment confirmation message structure.
            This confirmation is sent to indicate that the timing adjustment
            request has been processed.
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
} lte_LL1_sys_adj_timing_cnf_struct;

LTE_LL1_INTF_MSG( sys_adj_timing_cnf );


// --------------------------------------------------------------------------
//
// subframe indication
//
// --------------------------------------------------------------------------

#define LTE_LL1_SYS_SUBFRAME_SNR_INVALID ((int16)-32767)

typedef struct
{
  uint32    rxfe_trig_rd_ostmr[2];         ///32 bit ustmr raw value, ML1 need to convert to ostmr
                                           /// API vstmr_lte_xo_to_ostmr can convert this to ostmr, but lost precision
  uint64    rxfe_vsrc_sctr_acc_ostmr_1k[2];///< [0]: Rx Ant0, [1]: Rx Ant1, Combination of VSRC SCTR 
                                           ///< and ACC in OSTMR_1K resolution (with Ts/1024 accuracy) at LTE subframe boundary
  boolean   rxfe_vsrc_sctr_acc_valid_data; ///<  validate RXFE_VSRC and RXFE_Cn_TRIG_RD_PHASE
                                           ///< TRUE: data are valid, FALSE: data not valid 
}lte_LL1_sys_gnss_timing_transfer_per_carrier_struct;

/*! @brief  Subframe indication message structure
            This indication is sent every subframe during LL1 DL subframe
            scheduling.  There is a relatively fixed timing relationship
            between when this message is sent and the start of the DL 
            subframe.  
*/
typedef struct {                      
  
  lte_LL1_sys_time_struct new_sys_time;       ///< new system time

  int16                   ind_send_offset;              ///< Offset from dl_subf_ref_time
                                                                                                                                                                                ///< to when ind msg is sent

  uint32                  dl_subf_cnt;       ///< DL subframe counter 
                                             ///< reset upon system time start

  int32					 ftl_il_disc;		   ///< il discrimnant 32S19	  
  
  int32					 tx_rx_time_diff;	   ///< (tx time - rx time)  in Ts
                                             
  uint32                  dl_subf_mstmr;      ///< MSTMR at dl subframe

  uint64                  dl_subf_ref_time[LTE_LL1_DL_NUM_CARRIERS];             ///< ref time at dl subframe

  int32                   total_timing_err_rx[LTE_LL1_MAX_NUM_UE_RX_ANT]; ///< Total timing error computed
                                                  ///< by TTL since DL is enabled
                                                  ///< in Ts units

  int32                   total_rot_freq_err; ///< Total frequency error at the
                                              ///< rotator in 32S12 Hz - for PCC

  int32                   total_rot_freq_err_scc[LTE_LL1_MAX_NUM_UE_RX_ANT*2];//[4];/// for secondary carriers. always 2 per scc

  int32                   total_vco_freq_err; ///< Total frequency error at the
                                              ///< VCO in 32S12 Hz - for PCC

  int32                   total_vco_freq_err_scc[LTE_LL1_MAX_NUM_UE_RX_ANT*2];//[4];/// for secondary carriers

  boolean                 freeze_ftl;         ///< True if FW detects large frequency FTL and freezes FTL loop
 

  #ifndef SIM_PLATFORM
  lte_LL1_rxftl_snr_state_s ftl_snr[LTE_LL1_DL_NUM_CARRIERS];        ///FTL SNR (linear in 32Q16)
  #else
  uint32 ftl_snr[2];
  #endif
  
  int16                   subf_snr;           ///< Subframe SNR in 16S8
  uint32                  send_timestamp;     ///< UNIV_STMR counter timestamp when message was sent

  lte_LL1_csf_rlm_snr_state_s rlm_snr_ind[LTE_LL1_DL_NUM_CARRIERS];         //rlm snr (db in 16Q8)

  boolean                 is_ul_stmr_cnt_valid; ///< whether the ul_subf_mstmr/ustmr is value
  uint32                  ul_subf_mstmr;        ///< MSTMR at ul subframe
  uint32                  ul_subf_ustmr;        ///< USTMR at ul subframe
  uint32                  dl_subf_ustmr;        ///< USTMR at dl subframe
  uint32                  monitor_rx_on_state;

  lte_LL1_sys_gnss_timing_transfer_per_carrier_struct  gnss_timing_transfer[LTE_LL1_CARRIER_COUNT];

  uint16  channel_delay[LTE_LL1_CARRIER_COUNT];   /* 16U13 */

  boolean is_mbsfn_sf[LTE_LL1_CARRIER_COUNT]; /* whether SFN subframe */
  
} lte_LL1_sys_subframe_ind_struct;

LTE_LL1_INTF_MSG( sys_subframe_ind );


typedef struct{
	uint64 last_doppler_update_time;                      //Last time Doppler winner computed across all 8pt/EDE/16pt
	uint16 last_doppler_update_value;                     //Final Doppler value selected across all 8pt/EDE/16pt
	lte_LL1_carrier_type_enum carrier_idx;
	lte_LL1_doppler_est_type_enum last_doppler_est_type;  //0:  short; 1: Long;  2: enhanced; 3: invalid final Doppler selected type
}lte_LL1_sys_doppler_ind_struct ;

LTE_LL1_INTF_MSG( sys_doppler_ind );

// --------------------------------------------------------------------------
//
// Stop system time
//
// --------------------------------------------------------------------------

/*! @brief  Stop system time request message structure.
            This request stops system time.  Once stopped, any timing 
            references from prior searches are invalid.  
            
            Confirmation message will be sent once the time has stopped and 
            LTE HW PE's are inactive.  
            
            Before restarting system time, reset request can be issued
            to reset LTE FW and HW blocks.  

            This message is not supported on Espresso

*/
typedef struct {                      

  lte_LL1_req_header_struct req_hdr;    ///< common req header
  
  uint8 symbol_num;                     ///< # of additional OFDM symbols 
                                        ///< to process (0 ~ 14) before
                                        ///< stopping
    
} lte_LL1_sys_stop_sys_time_req_struct;

LTE_LL1_INTF_MSG( sys_stop_sys_time_req );



/*! @brief  Stop system time confirmation message structure.
            This confirmation is sent to indicate that the stop
            system time request has been processed.

            Not supported on Espresso

*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
} lte_LL1_sys_stop_sys_time_cnf_struct;

LTE_LL1_INTF_MSG( sys_stop_sys_time_cnf );


// --------------------------------------------------------------------------
//
// run RF script
//
// --------------------------------------------------------------------------

/*! @brief  Message structure for RF script request message
            
            When received, RF script (at specified index) is executed to 
            take effect at serving cell subframe boundary - specified 
            start offset.
            
            Confirmation to the request is sent after completion of the 
            script.
            
            The RF script request should only be issued when RF has 
            been allocated for LTE.
            
*/
typedef struct {                      

  lte_LL1_req_header_struct req_hdr;
  
  uint8 rf_script_index;  ///< The index in the RF script array of the RF script 
                          ///< for the LTE FW to execute
                           
  uint16  start_offset;   ///< starting offset from DL subframe boundary
                          ///< This is in units of TS.
                          ///< Range = 0 .. 30720/2

  lte_earfcn_t         earfcn;   ///< The frequency we are tuning to
                            
  lte_bandwidth_idx_e  bw;       ///< The bandwidth we are tuning to

  ///< normally keep RFD TQ locked after RF script is completed (set to 0),
  ///< because SS_start/srch/meas cmd will follow;
  ///< but in case this tune to L is just to facilitate tune to other tech (a strange RF requirement),
  ///< will unlock RFD TQ immediately (set to 1) after RF script is complete because there is no SS_start/srch/meas to follow
  boolean unlock_rfd_tq;  
                         
} lte_LL1_sys_run_rf_script_req_struct;

LTE_LL1_INTF_MSG( sys_run_rf_script_req );

/*! @brief Run RF Script confirmation structure

   This confirmation will be sent when the RF script specified in run RF script
   has been completed.
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;
  
} lte_LL1_sys_run_rf_script_cnf_struct;

LTE_LL1_INTF_MSG( sys_run_rf_script_cnf );

#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_TX0_HANDLE          (0)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_TX1_HANDLE          (1)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_PRX_HANDLE          (2)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_DRX_HANDLE          (3)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_DRX2_HANDLE         (4)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_DRX3_HANDLE         (5)
#define LTE_LL1_EXECUTE_TUNE_SCRIPTS_MAX_HANDLE          (6)

// --------------------------------------------------------------------------
//
// run RXLM RF script
//
// --------------------------------------------------------------------------

/*! @brief  Message structure for RXLM and RF script request message
            
            When received,
            1) In IRAT mode RF script (at specified index) is executed to 
            take effect at serving cell subframe boundary - specified 
            start offset.
            2) In both online and IRAT modes the RXLM buffer indices are
            configured. In online mode it happens immediately, in IRAT
            mode it happens at the start time.
 
            In IDLE mode IRAT, if SW does not want to issue a RF script,
            ML1 can set the RF script index to INVALID_SCRIPT_INDEX
            
            Confirmation to the request is sent after completion of the 
            requested execution. Meaning for IRAT mode after execuition of
            scripts and in online mode after execution of RXLM buffers
            
            The RXLM/ RF script request should only be issued when RF has 
            been allocated for LTE.
            
*/
typedef struct {                      

 lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

  uint8 rf_script_index;  ///< The index in the RF script array of the RF script 
                          ///< for the LTE FW to execute
  uint8 rxlm_buf_id_ant0; ///< RXLM buffer index for antenna 0
  uint8 rxlm_buf_id_ant1; ///< RXLM buffer index for antenna 1
  boolean irat_mode;      ///< Set to TRUE for IRAT (connected and IDLE)
                          ///< Set to FALSE for online mode
  uint16  start_offset;   ///< starting offset from DL subframe boundary
                          ///< This is in units of TS.
                          ///< Range = 0 .. 30720/2
  lte_earfcn_t         earfcn;   ///< The frequency we are tuning to
                            
  lte_bandwidth_idx_e  bw;       ///< The bandwidth we are tuning to

  ///< normally keep RFD TQ locked after RF script is completed (set to 0),
  ///< because SS_start/srch/meas cmd will follow;
  ///< but in case this tune to L is just to facilitate tune to other tech (a strange RF requirement),
  ///< will unlock RFD TQ immediately (set to 1) after RF script is complete because there is no SS_start/srch/meas to follow
  boolean unlock_rfd_tq;  
//new field added to support RX diversity (disable/enable separately)
  // bit 2 for change or not change flag, bit 1 for RX1, bit 0 for RX0 
  // bit 2: 1 for change enforced, 0 for not  change. bit 1 and 0: value 0 for off, value 1 for on
  uint8 rx_toolchain_config;   // new
  
  boolean start_stop_flag; ///< True: Start RF specified in the message
                          ///< False:Stop RF specified in the message

  boolean is_rf_tune_needed;// Based on this flag , script based RF tuning is initiated to FED/ RF
                            // NOTE: 1.)This tuning does not support IRAT and should be used 
                            //      for tuning home LTE configuration only.
                            //       2.) Tuning is timed for the next subframe boundary 
                            //       after the message's OTA
  uint32 rf_tune_payload;   // This payload is sent to RF for tuning (sanity check for RF) and 
                            // used in FW to prepare the RF tune message to FED
                            // It is a bitmask corresponding to LTE_LL1_EXECUTE_TUNE_SCRIPTS_xxx
                            // which should be indicated per carrier.

  boolean skip_preload;     // For tune back scenerios the PRELOAD call for FED is not required.
                            // skipping this will allow ML1 to trigger subsequent messages without
                            // the need to wait for a confirmation.
							
  boolean isOfflineGap;     //For OfflineGap, ML1 does not send any communication to RF. 
                            //AT L2X Gap, entry FW toggles off SCC. And in this scenario, SCC remains OFF.
							//If TRUE for SCC, FW to toggle RX to ON
							//For PCC, should always be FALSE

}lte_LL1_sys_run_rxlm_rf_script_req_payload;

typedef struct {
  lte_LL1_req_header_struct req_hdr;

  uint8 num_carriers; ///< Number of carriers to be configured.
  
  lte_LL1_sys_run_rxlm_rf_script_req_payload  carrier[LTE_LL1_DL_NUM_CARRIERS]; ///< rxlm_rf_script_req parameters per CA.
}lte_LL1_sys_run_rxlm_rf_script_req_struct;

LTE_LL1_INTF_MSG( sys_run_rxlm_rf_script_req );

typedef struct {
  lte_LL1_req_header_struct req_hdr;

  boolean vu2_to_dtr_switch_en; ///< enable using VU2 slots for DTR

}lte_LL1_sys_dtr_brdg_cfg_chg_req_struct;

LTE_LL1_INTF_MSG( sys_dtr_brdg_cfg_chg_req );

//FW needs 2 ms to process NB switch
#define NB_SWICH_CNF_DELAY 2

typedef struct {
  lte_LL1_req_header_struct req_hdr;

  uint8 num_nbs;

  lte_LL1_sys_nb_mux_settings_t nb_switch_params[LTE_LL1_NB_ID_COUNT];

}lte_LL1_sys_nb_carrier_switch_req_struct;

LTE_LL1_INTF_MSG( sys_nb_carrier_switch_req );

typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for carriers to be supported for CA
 
  lte_LL1_nb_id_enum nb_id;  ///< Enum for NB id mapping
 
}lte_LL1_sys_nb_mux_settings_payload_struct;

/*!@brief CNF message for NB Switch b/n PSC/SSC
*/
typedef struct {
  lte_LL1_cnf_header_struct cnf_hdr;
  
  uint8 num_nbs;
  
  lte_LL1_sys_nb_mux_settings_payload_struct nb_mux_settings[LTE_LL1_NB_ID_COUNT];

}lte_LL1_sys_nb_carrier_switch_cnf_struct;

LTE_LL1_INTF_MSG( sys_nb_carrier_switch_cnf );

/*! @brief Run RXLM RF Script confirmation structure

   This confirmation will be sent when the RF script specified in run RF script
   has been completed (in IRAT mode) or when RXLM buffers are programmed ( in
   online mode)
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;

  uint8 carrier_idx_bitmask; //  (1 << LTE_LL1_CARRIER_PCC) | (1 << LTE_LL1_CARRIER_SCC_0)
    
} lte_LL1_sys_run_rxlm_rf_script_cnf_struct;

LTE_LL1_INTF_MSG( sys_run_rxlm_rf_script_cnf );

/*! RX tool chain config request for PCC/SCC
*/
typedef struct {
  lte_LL1_req_header_struct req_hdr;
  lte_LL1_carrier_type_enum carrier_type;  ///< Enum for dl carriers in  CA

  //new field added to support RX diversity (disable/enable separately)
  // bit 4 for specify sample server
  // bit 3 for change or not change flag
  // bit 2 for RX1
  // bit 1 for RX0
  uint8 rx_toolchain_config;

}lte_LL1_sys_rx_tool_chain_config_req_struct;

LTE_LL1_INTF_MSG( sys_rx_tool_chain_config_req );

typedef struct {
  lte_LL1_cnf_header_struct cnf_hdr;
  lte_LL1_carrier_type_enum carrier_type;  ///< Enum for dl carriers in  CA
}lte_LL1_sys_rx_tool_chain_config_cnf_struct;

LTE_LL1_INTF_MSG( sys_rx_tool_chain_config_cnf );

/*! @brief LTE logging mask update structure
*/
#define LTE_LL1_SYS_LOG_MASK_SZ_IN_WORD  3
typedef struct {

  lte_LL1_req_header_struct req_hdr;     ///< common req header

  uint32 mask[LTE_LL1_SYS_LOG_MASK_SZ_IN_WORD];  /*!< Compressed bitmask for LTE LL1 log IDs */

  uint16 csf_logging_period;  /* !< Configure logging period (in SF) of SE and whitened matrices */

  boolean tx_on; /*!<TRUE disable turning TX ON/OFF FALSE do not disable */

} lte_LL1_sys_log_mask_config_req_struct;

LTE_LL1_INTF_MSG( sys_log_mask_config_req );



// --------------------------------------------------------------------------
//
// parameter config (will change)
//
// --------------------------------------------------------------------------


/*! @brief lte LL sys configuration parameter id's
    These enums are also used to index into lte_sys_cfg[] array
*/
typedef enum
{
  LTE_LL1_SYS_PARAM_CONFIG__UD_OFFSET_MIN = 0,
  LTE_LL1_SYS_PARAM_CONFIG__UD_OFFSET_MAX,
  LTE_LL1_SYS_PARAM_CONFIG__UD_OFFSET_INIT,
  LTE_LL1_SYS_PARAM_CONFIG_COUNT 
} lte_LL1_sys_cmd_param_config_id_enum;


typedef struct {                      

  lte_LL1_req_header_struct req_hdr;
  
  lte_LL1_sys_cmd_param_config_id_enum param_id;  ///< parameter id number
  int16 index;                    ///< parameter array index (1 for non-array)
  int64 value;                    ///< parameter value
  
} lte_LL1_sys_param_config_req_struct;

LTE_LL1_INTF_MSG( sys_param_config_req );


/*! @brief param config CNF structure
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;
  
} lte_LL1_sys_param_config_cnf_struct;

LTE_LL1_INTF_MSG( sys_param_config_cnf );

// --------------------------------------------------------------------------
//
// GPS TimeTag
//
// --------------------------------------------------------------------------
/*! @brief  Message structure for GPS TimeTag request message
            
            Upon receiving this request, we obtain GPS RTC by directly
            reading the RC_FRAME_COUNT_STATUS register, and read the current
            LTE RTC value (i.e., unwrapped mstmr).
            
            Confirmation to the request is sent back with two RTC values
            obtained above.
            
*/
typedef struct {                      

  lte_LL1_req_header_struct req_hdr;                            
} lte_LL1_sys_gps_timetag_req_struct;

LTE_LL1_INTF_MSG( sys_gps_timetag_req );

/*! @brief GPS TimeTag confirmation structure

   This confirmation is sent back with GPS
   and LTE RTC values read within 5us of
   each other.
*/
typedef struct {                      
  lte_LL1_cnf_header_struct cnf_hdr;

  uint32 gps_rtc;   ///< 32-bit GPS RTC value from RC_FRAME_COUNT_STATUS register
  uint64 lte_rtc;   ///< 64-bit LTE RTC  
} lte_LL1_sys_gps_timetag_cnf_struct;

LTE_LL1_INTF_MSG( sys_gps_timetag_cnf );

// --------------------------------------------------------------------------
//
// SYNC_STMR_DUMP, for x2L timing transfer
//
// --------------------------------------------------------------------------

/*! @brief  Dump other tech's and LTE's STMR synchronously
            to maintain LTE timing across sleep cycles.
            Can turn LTE time off, then wakeup (restore) LTE timer using other tech's stmr
            
*/
typedef struct {                    

  lte_LL1_req_header_struct req_hdr;          ///< common req header
  
  lte_LL1_irat_time_e   other_tech; ///< this tech's stmr to correct LTE's stmr
  
} lte_LL1_sys_sync_stmr_dump_req_struct;

LTE_LL1_INTF_MSG( sys_sync_stmr_dump_req );


/*! @brief  return sync_stmr_dump info to SW,
            basically sync stmr values dump for both LTE and other tech,
            so SW can map other tech's timing window into LTE's timing
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;          ///< common cnf header
  
  lte_LL1_stmr_snapshot_s stmr_snapshot;

} lte_LL1_sys_sync_stmr_dump_cnf_struct;

LTE_LL1_INTF_MSG( sys_sync_stmr_dump_cnf );

/*! @brief  return sync_stmr_dump data for next TTRANS_WAKEUP
            This is moved from intf_async.h to here, because it refers to sync_stmr_dump
*/
typedef lte_LL1_sys_sync_stmr_dump_cnf_struct lte_LL1_async_ttrans_wakeup_cnf_struct;

LTE_LL1_INTF_MSG( async_ttrans_wakeup_cnf );


// --------------------------------------------------------------------------
//
// Sample recording done indication
//
// --------------------------------------------------------------------------

/*! @brief  Sample recording done indication message.
            This indication is sent only in the offline drx mode after
            all the samples required for offline processing are recorded.  
*/
typedef struct {
  msgr_hdr_struct_type              msg_hdr;      ///< message router header
  uint64  sample_rec_start_ref_time;              ///< 64-bit sample recording start time (for debuggin)
  uint64  sample_rec_end_ref_time;                ///< 64-bit sample recording end time (for debugging) 
  boolean bad_samples_due_to_rf_delay;            ///< indicate if RF-wakeup was delayed at the time of set_lna call
  /* No message payload */ 
} lte_LL1_sys_sample_rec_done_ind_msg_struct;

/*! @brief  
 *  The buffer XO manager (or another entity) needs details of the 
 *  mempool buffers and can be the narrowband searcher
 *  or the wideband demod buffer.        
 */
typedef enum {
  /*! This is the wideband demod td buffer */
  LTE_LL1_SYS_MEMPOOL_BUF_DEMOD,
  /*! This is the primary search td buffer */
  LTE_LL1_SYS_MEMPOOL_BUF_SRCH
}lte_LL1_sys_mp_buf_type_e;


/*! @brief  
 *  The buffer XO manager needs details  needs details of the 
 *  mempool buffers can be the narrowband searcher or the 
 *  wideband demod buffer. This message is expected only after
 *  LTE has been started and time/sample capture has started. 
 */
typedef struct{
  /*! Common request header struct. This message does not 
   * need to be timed */
  lte_LL1_req_header_struct      req_hdr;   

  /*! The buffer to send details for. */
  lte_LL1_sys_mp_buf_type_e   req_buffer;

}lte_LL1_sys_mempool_buf_details_req_struct;

LTE_LL1_INTF_MSG(sys_mempool_buf_details_req);

/*! @brief 
 *   The Mempool bridge configuration for AHB interface. This configuration
 *   (and parameters therein) are specific to the AHB interface only.
 */ 
typedef struct
{
  /*! The number of samples of q_size width */
  uint16   vbuf_len;

  /*! The min bank (0 ~15) */
  uint8    min_bank;

  /*! The max bank (0 ~15), must be >= min_bank */
  uint8    max_bank;

  /*! The init bank (0 ~ 15), must be >= min_bank 
   *  and <= max_bank */
  uint8    init_bank;

  /*! The sample bitwidth (hw code) refer SWI  */
  uint8    q_size   ;

  /*! The starting line address */
  uint16   start_vaddr;

}lte_LL1_mempool_buf_details_s;

/*! @brief 
 * The confirmation for the XO Mempool Details request message.
 */

typedef struct
{
  /*! common cnf header */
  lte_LL1_cnf_header_struct    cnf_hdr;

  /*! The result of the transaction */
  errno_enum_type              result;
  /*! The requested buffer echoed back to account
   * for any potential race conditions */
  lte_LL1_sys_mp_buf_type_e    req_buffer;

  /*! the details of the mempool requested.
   * We return one entry per RX antenna. Since
   * only the SRCH/DEMFRONT buffers are supported,
   * both entries in this array will be valid:
   * mp_buf_details[0] for RX ant0
   * mp_buf_details[1] for RX ant1 
   *  */
  lte_LL1_mempool_buf_details_s         mp_buf_details[LTE_LL1_MAX_NUM_UE_RX_ANT];
  

}lte_LL1_sys_mempool_buf_details_cnf_struct;

LTE_LL1_INTF_MSG(sys_mempool_buf_details_cnf);

/*! @brief This message is sent once by the CXMSW entity on the SW processor
 * to ML1 when the phone comes up. ML1 will send this message after
 * configuring LTE in FULL RES mode */
typedef struct
{
  lte_LL1_req_header_struct        req_hdr;
  
  /* The initial configuration */
  cxm_boot_config_v01_s            init_cfg;
} lte_LL1_sys_cxm_boot_params_req_struct;

LTE_LL1_INTF_MSG(sys_cxm_boot_params_req);

/*! @brief confirmation for the CXM config req */
typedef struct
{
  
  lte_LL1_cnf_header_struct    cnf_hdr;

  /* The result of the boot parameters configuration command */
  errno_enum_type              result;
} lte_LL1_sys_cxm_boot_params_cnf_struct;

LTE_LL1_INTF_MSG(sys_cxm_boot_params_cnf);

typedef struct
{

  /* This indicates the number of wcn requests processed in the
   * currently active abort window */
  lte_LL1_ind_header_struct                 ind_hdr;

  /* Whether the consecutive or average threshold was breached */
  cxm_wcn_txfrmdnl_stats_v01_s              wcn_error;

}lte_LL1_sys_cxm_wcn_threshold_exceeded_ind_struct;

LTE_LL1_INTF_MSG(sys_cxm_wcn_threshold_exceeded_ind);
 
typedef struct
{
  lte_LL1_req_header_struct    req_hdr;

  /* The dynamic coexistence policy */

  cxm_active_config_v02_s      cxm_policy;

} lte_LL1_sys_cxm_active_req_struct;

LTE_LL1_INTF_MSG(sys_cxm_active_req);

/*! @brief confirmation for the TX Active req struct */
typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr;

  /* The result of the boot parameters configuration command */
  errno_enum_type              result;
} lte_LL1_sys_cxm_active_cnf_struct;

LTE_LL1_INTF_MSG(sys_cxm_active_cnf);

/* @brief Enum for bitmask for controlling modules for light sleep 
 * @detail Only one of the bits can be set per module.
           E.g., for Demfront only MOD_DEMFRONT_CLK or MOD_DEMFRONT
           can be set in enable or disable bitmask
*/
typedef enum
{
  /* Demfront clock enable/disable (via clock gating) */
  LTE_LL1_UE_LIGHT_SLEEP_MOD_DEMFRONT_CLK,

  /* Demfront on/off */
  LTE_LL1_UE_LIGHT_SLEEP_MOD_DEMFRONT,

  /* Demback on/off */
  LTE_LL1_UE_LIGHT_SLEEP_MOD_DEMBACK,

  /* Rx ADC on/off */
  LTE_LL1_UE_LIGHT_SLEEP_MOD_RX_ADC,

  /* Tx DAC on/off */
  LTE_LL1_UE_LIGHT_SLEEP_MOD_TX_DAC,
  
  LTE_LL1_UE_LIGHT_SLEEP_MOD_NUM_MODULES
} lte_LL1_ue_light_sleep_mod_e;

/*! @brief Message to independently control modules for light sleep
 *  @note  This is expected to be a timed message
 *  @detail Only one of the enable or disable bitmasks can be non-zero
*/
typedef struct
{
  lte_LL1_req_header_struct req_hdr;

  /* Bitmask to enable modules at light sleep exit.
     Non-zero enable_bitmask means light sleep exit.
  */
  uint32 enable_bitmask;

  /* Bitmask to disable modules at light sleep entrance.
     Non-zero disable_bitmask means light sleep entrance.
  */
  uint32 disable_bitmask;
} lte_LL1_sys_light_sleep_mod_ctrl_req_struct;

LTE_LL1_INTF_MSG( sys_light_sleep_mod_ctrl_req );


/*! @brief Confirmation for message to control modules for light sleep
*/
typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  uint32 enabled_bitmask; ///< Bitmask of enabled modules after processing message
} lte_LL1_sys_light_sleep_mod_ctrl_cnf_struct;

LTE_LL1_INTF_MSG( sys_light_sleep_mod_ctrl_cnf );

/*! @brief Request message to support disabling updates to RF
 *  @detail : This sequence to be used to disable updates to HW (RF and RxFE )
 */


typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  uint16  freeze_update_mask;         ///<  lte_LL1_ue_freeze_update_mod_e values 
                                     ///< to indicate what entity(s) to be disabled
  uint16  unfreeze_update_mask;         ///<  lte_LL1_ue_freeze_update_mod_e values 
                                     ///< to indicate what entity(s) to be re-enabled 
} lte_LL1_sys_freeze_update_carrier_payload_t;

typedef struct
{
  lte_LL1_req_header_struct req_hdr;            
  uint8 num_carriers;              ///< Number of valid carrier information sent below.
  lte_LL1_sys_freeze_update_carrier_payload_t carrier[LTE_LL1_DL_NUM_CARRIERS]; ///< parameters per CA.
}lte_LL1_sys_freeze_update_req_struct;

LTE_LL1_INTF_MSG( sys_freeze_update_req );

/*! @brief Confirmation for message to disabling updates to RF
*/
typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  uint32 carrier_mask;                  ///< Carrier mask bitfield to confirm AGC frozen 
                                        ///  for the corresponding carrier, 
} lte_LL1_sys_freeze_update_cnf_struct;

LTE_LL1_INTF_MSG( sys_freeze_update_cnf );


// --------------------------------------------------------------------------
//
// FED based RF tune
//
// --------------------------------------------------------------------------

/*! @brief Request message to support FED based RF tuning
 *  @details: This sequence is used to trigger the scripts to tune RF by 
 *        calling FED API calls.
 *        Each request message generates a correspoding confirmation to ML1.
 *          
 */
//@TODO: Need to get this info from some RF API
#define LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_DL_HANDLES  (20)
#define LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_UL_HANDLES  (6)
#define LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_HANDLES  (LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_DL_HANDLES \
                                                          + LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_UL_HANDLES)

typedef struct
{
  uint32 handle                   :8; //< RFSW handle containing the tuning script, 0xFF invalid handles not allowed

#define  LTE_LL1_SYS_FED_BASED_RF_TUNE_HANDLE_IS_TX  (0) 
#define  LTE_LL1_SYS_FED_BASED_RF_TUNE_HANDLE_IS_RX  (1) 
  uint32 handle_type              :1; //< Handle is for Tx or Rx (used to determine which FED call to make)
  uint32 carrier_id               :3; //< Needed for relative timing anchoring. Same as lte_LL1_carrier_type_enum
  
  /* Note : The following script types are bit masks and FW and need to be OR'ed . FW will assert if no script present */
// Preload the script, always be immidiate (take current time + align_timing_offset)
#define  SYS_FED_BASED_RF_TUNE_SCRIPT_TYPE_PRELOAD          (0x1) 
// Trigger script, always be action timed
#define  SYS_FED_BASED_RF_TUNE_SCRIPT_TYPE_TRIGGER          (0x2) 
// non TRX script, always be action timed
#define  SYS_FED_BASED_RF_TUNE_SCRIPT_TYPE_NON_TRX          (0x4) 
  uint32 script_type_bitmask      :3;

#define  SYS_FED_BASED_RF_TUNE_ALIGN_ACTION_TIME_DL_SF_BNDRY (0) 
#define  SYS_FED_BASED_RF_TUNE_ALIGN_ACTION_TIME_UL_SF_BNDRY (1) 
  uint32 align_timing_uldl        :1;  //< Align timing to DL (if 1) or UL OTA subframe boundary of the carrier_id
  int32 align_timing_offset_usecs:16; //< Offset from the DL/UL subframe boundary (range [-32768, 32767] . 
                                   //  Ie it can be max 32.768 ms from the current time

}sys_fed_based_rf_tune_handle_combo_t;

typedef struct
{
  lte_LL1_req_header_struct req_hdr;            
  uint8 num_handle;               ///< Number of valid carrier information sent below.
  sys_fed_based_rf_tune_handle_combo_t handle_info[LTE_LL1_SYS_FED_BASED_RF_TUNING_MAX_NUM_HANDLES]; ///< parameters per handle.

}lte_LL1_sys_fed_based_rf_tune_req_struct;

LTE_LL1_INTF_MSG( sys_fed_based_rf_tune_req );

/*! @brief Confirmation for message to disabling updates to RF
*/
typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  uint8 num_handle;              ///< Number of valid carrier which are tuned
} lte_LL1_sys_fed_based_rf_tune_cnf_struct;

LTE_LL1_INTF_MSG( sys_fed_based_rf_tune_cnf );

// --------------------------------------------------------------------------
//
// MacroSleep Sleep Request
//
// --------------------------------------------------------------------------

/*! @brief Structure to consist of Macrosleep-sleep 
           related information for each carrier
 
  */
typedef struct
{

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  uint8                     num_pdcch_sf_to_monitor;   ///< number of PDCCH results to monitor (0,1,2 or 3)

}lte_LL1_carrier_specific_ms_sleep_info_t;

/*! @brief  Macrosleep Sleep request message structure. 
            This request is sent from ML1 to schedule a sleep on the immediate
            sleep in the following subframe, subject to meeting certain
            conditions. used mostly in, but not restricted to, CDRX
            short ON-duration scenarios.
 
            If request is honored, FW will schedule WB sample-record-stop,
            Rx_ON toggling HIGH -> LOW, Disable DL and VPE scheduling.
            
*/
typedef struct
{
  lte_LL1_req_header_struct req_hdr;      ///< Common request parameters
  lte_LL1_carrier_specific_ms_sleep_info_t ms_sleep_info[LTE_LL1_CARRIER_COUNT];
  uint8                 carrier_mask; ///< indicate which carrier(s) the request correspond to
} lte_LL1_sys_macrosleep_sleep_req_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_sleep_req );


// --------------------------------------------------------------------------
//
// MacroSleep Sleep Confirmation
//
// --------------------------------------------------------------------------

/*! @brief Structure to carry information on 
           Macrosleep-sleep status for requested carriers
 
  */
typedef struct
{

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  lte_LL1_macrosleep_status_enum sleep_status; ///< Macrosleep status

}lte_LL1_macrosleep_sleep_status_info_struct;


/*! @brief Confirmation for macrosleep-sleep request message
*/
typedef struct
{

  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  lte_LL1_macrosleep_sleep_status_info_struct  status_info[LTE_LL1_CARRIER_COUNT];
  uint8                 carrier_mask; ///< indicate which carrier(s) the cnf is for
} lte_LL1_sys_macrosleep_sleep_cnf_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_sleep_cnf );


// --------------------------------------------------------------------------
//
// MacroSleep Wakeup Request
//
// --------------------------------------------------------------------------



/*! @brief Specifies the Carrier_type to be used 
           in MS_wakeup request and confirmation messages
 
  */
typedef struct
{

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

}lte_LL1_macrosleep_carrier_type_info_struct;

/*! @brief  Macrosleep Wakeup request message structure. 
            This request is sent from ML1 to schedule a wakeup, corresponding
            to a previously sent macrosleep request.
 
            If wakeup request is honored, FW will schedule WB sample-record-start,
            Rx_ON toggling LOW -> HIGH, Enable DL and VPE scheduling.
            
*/
typedef struct
{
  lte_LL1_req_header_struct req_hdr;      ///< Common request parameters
  lte_LL1_macrosleep_carrier_type_info_struct   carrier_type_info[LTE_LL1_CARRIER_COUNT]; ///< Enum for dl carriers in  CA
  uint8                 carrier_mask; ///< indicate which carrier(s) the request correspond to
} lte_LL1_sys_macrosleep_wakeup_req_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_wakeup_req );

// --------------------------------------------------------------------------
//
// MacroSleep Wakeup Confirmation
//
// --------------------------------------------------------------------------
/*! @brief Confirmation for macrosleep-wakeup request message
*/
typedef struct
{

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA
  lte_LL1_macrosleep_wakeup_status_enum wakeup_status; ///< Macrosleep status
}lte_LL1_macrosleep_wakeup_status_info_struct;

typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  lte_LL1_macrosleep_wakeup_status_info_struct wakeup_info[LTE_LL1_CARRIER_COUNT]; ///< Enum for dl carriers in  CA
  uint8                 carrier_mask; ///< indicate which carrier(s) the cnf correspond to
} lte_LL1_sys_macrosleep_wakeup_cnf_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_wakeup_cnf );

// --------------------------------------------------------------------------
//
// MacroSleep Request (Sleep or wakeup) cancel
//
// --------------------------------------------------------------------------
typedef struct
{
  lte_LL1_req_header_struct req_hdr;      ///< Common request parameters
  lte_LL1_macrosleep_req_type_enum cancel_type;///< type of request to cancel
} lte_LL1_sys_macrosleep_cancel_req_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_cancel_req );

typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; ///< Common cnf header
  lte_LL1_macrosleep_cancel_status_enum cancel_info;///< status of cancel
} lte_LL1_sys_macrosleep_cancel_cnf_struct;

LTE_LL1_INTF_MSG( sys_macrosleep_cancel_cnf );

/*! @brief XXX - Temporary search PSS log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_pss_results_ind );

/*! @brief XXX - Temporary search SSS log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_sss_results_ind );

/*! @brief XXX - Temporary serving cell measurements log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_serving_cell_measurement_results_ind );

/*! @brief XXX - Temporary PUSCH log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_ul_pusch_tx_report_ind );

/*! @brief XXX - Temporary PUCCH log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_ul_pucch_tx_report_ind );

/*! @brief XXX - Temporary SRS log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_ul_srs_tx_report_ind );

/*! @brief XXX - Temporary RACH log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_ul_rach_tx_report_ind );

/*! @brief XXX - Temporary Demfront Serving cell CER log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_demfront_serving_cell_cer_ind );

/*! @brief XXX - Temporary Demfront Serving cell COM loop log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_serving_cell_com_loop_ind );

/*! @brief XXX - Temporary Demfront PDSCH Demapper configuration log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_demfront_pdsch_demapper_configuration_ind );

/*! @brief XXX - Temporary CSF Spectral Efficiency log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_spectral_efficiency_for_txmode_1_2_7_ind );

/*! @brief XXX - Temporary CSF Spectral Efficiency log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_spectral_efficiency_for_txmode_4_5_6_rank_1_ind );

/*! @brief XXX - Temporary CSF Spectral Efficiency log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_spectral_efficiency_for_txmode_4_rank_2_ind );

/*! @brief XXX - Temporary Whitened channel buffer log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_whitened_matrices_ind );

/*! @brief XXX - Temporary CSF PUCCH report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_pucch_report_ind );

/*! @brief XXX - Temporary CSF PUSCH report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_csf_pusch_report_ind );

/*! @brief XXX - Temporary UERS tone log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_uers_tones_ind );

/*! @brief XXX - Temporary UL Tx PUSCH report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_ul_agc_tx_report_ind );

/*! @brief XXX - Temporary DL Rx AGC report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_rxfe_agc_ind );

/*! @brief XXX - Temporary DL Rx AGC internal report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_rxfe_agc_int_ind );

/*! @brief XXX - Temporary Serving cell TTL report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_serving_cell_ftl_results_ind );

/*! @brief XXX - Temporary Serving cell FTL report log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_serving_cell_ttl_results_ind );

/*! @brief XXX - Temporary RLM logging results indication message
*/
LTE_LL1_INTF_MSG( log_rlm_results_ind );

/*! @brief XXX - Temporary Neighbor cell measurements, tracking report log message.
                 This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_srch_neighbor_cell_measurements_and_tracking_ind );

/*! @brief XXX - Temporary Antenna Correlation log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_demfront_antenna_correlation_results_ind );

/*! @brief XXX -  Temporary Demfront Serving cell RS log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_demfront_serving_cell_rs_ind );

/*! @brief XXX - Temporary Demfront Neighbor cell CER log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_demfront_neighbor_cell_cer_ind );

/*! @brief XXX - Temporary PRS log message.
  This will go away when we have logging through diag.
*/
LTE_LL1_INTF_MSG( log_prs_fw_ind );

// --------------------------------------------------------------------------
//
// IQMC update requests
// 
// --------------------------------------------------------------------------
/*! @brief IQMC update message request 
*/
typedef struct
{
   uint32 iqmc_ab; // IQMC AB: A - bit 0-12, B - bit 13-25

   uint32 iqmc_abdiv; // IQMC AB for diversity. Format the same as iqmcAb
} lte_LL1_sys_iqmc_update_iqmc_coef_t;

typedef struct
{
   uint32 carrier_en_bitmap;   // LSB maps to PCC, bit "1" maps to first SCC, etc

   lte_LL1_sys_iqmc_update_iqmc_coef_t iqmc_coeffts[LTE_LL1_DL_NUM_CARRIERS];

} lte_LL1_sys_iqmc_update_req_payload;

typedef struct 
{
   lte_LL1_req_header_struct req_hdr;
   lte_LL1_sys_iqmc_update_req_payload msg_payload;
} lte_LL1_sys_iqmc_update_req_struct;

LTE_LL1_INTF_MSG(sys_iqmc_update_req);

typedef struct
{
  /* Common request header */
  lte_LL1_req_header_struct req_hdr;
  /* AS ID */
  uint8 as_id;    
}lte_LL1_sys_stack_info_req_struct;

LTE_LL1_INTF_MSG(sys_stack_info_req);

#ifdef FEATURE_MODEM_ANTENNA_SWITCH_DIVERSITY
// --------------------------------------------------------------------------
//
// Antenna Diversity requests/confirmations
// 
// --------------------------------------------------------------------------
/*! @brief Antenna diversity message request 
*/
typedef struct
{
  lte_LL1_req_header_struct       req_hdr;        ///< Common request header
  lte_LL1_ant_setting_enum_t      ant_setting;    ///< ant setting 
  lte_LL1_ant_switch_type_enum_t  switch_type;    ///< ant switching type: type1 or 2
} lte_LL1_sys_ant_switch_req_struct;

LTE_LL1_INTF_MSG( sys_ant_switch_req );

/*! @brief Antenna diversity message confirmation
*/
typedef struct
{
  lte_LL1_cnf_header_struct  cnf_hdr;  ///< Confirmation message header
  lte_LL1_ant_setting_enum_t ant_setting; ///< applied ant_setting
} lte_LL1_sys_ant_switch_cnf_struct;

LTE_LL1_INTF_MSG( sys_ant_switch_cnf );
#endif
// --------------------------------------------------------------------------
//
// Get EIB address
//
// --------------------------------------------------------------------------

/*Interface */
/*! @brief EIB Address Request 
*/
typedef struct {                      

  lte_LL1_req_header_struct req_hdr;

  lte_LL1_carrier_type_enum       carrier_type;         ///< carrier_index, 0=PCC, 1=SCC

  lte_bandwidth_enum              bandwidth;

} lte_LL1_sys_eib_addr_req_struct;

LTE_LL1_INTF_MSG( sys_eib_addr_req );
/*Interface */
/*! @brief EIB Address Cnf 
*/
typedef struct {                      
  
  lte_LL1_cnf_header_struct cnf_hdr;

  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers in  CA

  //Ping/pong addreses
  uint32 eib_base_addr[2]; ///< Encoder Base addresses for ping and pong regions
  
} lte_LL1_sys_eib_addr_cnf_struct;

LTE_LL1_INTF_MSG( sys_eib_addr_cnf );

// --------------------------------------------------------------------------
//
// DRX_ON_DURATION requests/confirmations
// 
// --------------------------------------------------------------------------
/*! @brief DRX on-duration message request 
*/
typedef struct
{
  lte_LL1_req_header_struct   req_hdr;      ///< Common request header
  boolean drx_on_duration_state;
} lte_LL1_sys_drx_on_duration_req_struct;

LTE_LL1_INTF_MSG( sys_drx_on_duration_req );


/*! @brief QTA (Quick Tune away) message for DSDS feature
*   This request message will provide the configuration details to register
*   LTE tech for CXM module to check for coexistence conflicts 
*
*/

typedef struct
{
	uint8 start_subframe_num;
	uint16 start_frame_num;
	uint8 end_subframe_num;
	uint16 end_frame_num;
}lte_LL1_sys_fwqta_start_ind_struct;
LTE_LL1_INTF_MSG ( sys_fwqta_start_ind );
typedef struct
{
	uint8 subframe_num;
	uint16 frame_num;
}lte_LL1_sys_fwqta_end_ind_struct;
LTE_LL1_INTF_MSG ( sys_fwqta_end_ind );
typedef struct
{
  lte_LL1_req_header_struct req_hdr;                                        ///< Common request header
  uint32 priority;                                                       ///< LTE priority
#ifndef SIM_PLATFORM
  uint16 action_time_offset; // < starting offset from DL sub frame boundary in USTMR tick < Range = 0 .. 19200>
#endif
  uint8 num_dl_carriers;               ///< Number of DL carriers to be configured.
  uint8 num_ul_carriers;               ///< Number of UL carriers to be configured.
  lte_LL1_qta_dl_carrier_info_struct dl_carriers[LTE_LL1_DL_NUM_CARRIERS];  ///< Frequency ID per carrier
  lte_LL1_qta_ul_carrier_info_struct ul_carriers[LTE_LL1_UL_NUM_CARRIERS];                            ///< Frequency ID per carrier

  uint8 num_dl_carriers_to_tune;               ///< Number of DL carriers to be configured.
  lte_LL1_qta_dl_rf_tune_info_struct rf_tune_param[LTE_LL1_DL_NUM_CARRIERS];

}lte_LL1_sys_conflict_check_req_struct;

LTE_LL1_INTF_MSG ( sys_conflict_check_req );

/*! @brief QTA confirmation message if 1 CNF message if preferred 
*/

typedef struct 
{
  lte_LL1_cnf_header_struct  cnf_hdr;  ///< Confirmation message header
  uint32 frame_num;
  uint32 subframe_num;
}lte_LL1_sys_conflict_check_cnf_struct;

/*! @brief QTA confirmation message struct if two CNF messages is preferred by ML1


typedef struct 
{
  lte_LL1_cnf_header_struct  cnf_hdr;              ///< Confirmation message header
  uint64 ref_time;                                 ///< 64-bit reference time at begnining of conflict
  lte_LL1_qta_conflict_type_enum_t conflict_type;  ///< Conflict type (start or end)
}lte_LL1_sys_qta_cnf_struct;

*/
LTE_LL1_INTF_MSG( sys_conflict_check_cnf );
/*! @brief Conflict check IND message to be sent to ML1
           once L FW starts to yeilds to G in multi sim scenarios
*/
typedef enum
{
    LTE_LL1_TA_GAP_START,
    LTE_LL1_TA_GAP_END,
    LTE_LL1_STATUS_INVALID
} lte_LL1_conflict_check_status_enum;

typedef struct {
lte_LL1_ind_header_struct				  header;	
  lte_LL1_conflict_check_status_enum      status;
  /*! SF ref time at conflict check status */
  int64  sf_ref_time;	
  lte_LL1_sys_time_struct sys_time;
}lte_LL1_sys_conflict_check_ind_struct;

LTE_LL1_INTF_MSG( sys_conflict_check_ind );

/*! @brief Test message for DSDA interference pattern. This will be used for registering G activities 
*   in order to simluate interference pattern
*/

typedef struct
{
  lte_LL1_req_header_struct req_hdr;                                        ///< Common request header
  uint16 num_total_symbols;
  uint16 intf_start_sym;
  uint16 intf_end_sym;
}lte_LL1_sys_dsda_intf_test_req_struct;

LTE_LL1_INTF_MSG ( sys_dsda_intf_test_req );

typedef enum
{
  LTE_LL1_MULTI_RAT_CXM_REGISTRATION_TYPE_DEFAULT,
  LTE_LL1_MULTI_RAT_CXM_REGISTRATION_TYPE_ASDIV,
}lte_LL1_multi_rat_cxm_registration_type_e;

typedef struct
{
  lte_LL1_req_header_struct req_hdr;   

  uint32 duration_sf;
  lte_LL1_change_cxm_params_struct cxm_params;
  lte_LL1_multi_rat_cxm_registration_type_e registration_type;
}lte_LL1_sys_multi_rat_change_cxm_params_req_struct;

LTE_LL1_INTF_MSG ( sys_multi_rat_change_cxm_params_req );

/*! @brief Message to enable/disable diversity Rx during normal/simultaneous_LTE
*   This request message will provide the specific diversity antenna
*   that needs to be acquired/relinquished by LTE
*
*/
/* TODO: @viswa: This is struct which captures rxagc info for 1rx only */
/*! @brief  Parameters used when enabling AGC */
typedef struct
{
  /* Antenna logical index */
  lte_LL1_ue_rx_ant_enum rx_ant_idx;
  // RxAGC payload validity indicator 
  // - if TRUE during diversity Rx enable request then ML1 provided values will be used
  //   for RxAGC on the enabled Rx else existing FW values will continue to be used
  // - if TRUE during diversity Rx disable confirmation indicates valid FW RxAGC state
  //   at time of Rx antenna disable (will be FALSE if RxAGC is disabled)
  boolean is_valid;
  // RxAGC mode, should be the same as primary RxAGC mode (probably more suited for
  // confirmation payload than request)
  lte_LL1_rxagc_mode_e rxagc_start_mode;
  // Starting RxAGC param values (DVGA, LNA, RSSI etc)
  lte_LL1_rxagc_start_params_s rxagc_start_params;
  // DCC accum table validity indicator
  // - if TRUE for diversity Rx enable request FW will use ML1 provided values else
  //   existing DCC table will be used
  // - if TRUE for diversity Rx disable confirmation indicates valid FW RxAGC state
  //   at time of Rx antenna disable (will be FALSE if AGC is disabled)
  boolean accum_table_valid;
  // DCC accum value table
  lte_LL1_dcc_accum_cfg_table_s dcc_table;
}lte_LL1_sys_rxagc_info_struct;

typedef struct
{
  // Enum for dl carrier on which the Rx antenna mode needs to be changed
  lte_LL1_carrier_type_enum carrier;

#if 0
  // Rx antenna to be enabled or disabled - should be Rx0
  lte_LL1_ue_rx_ant_enum rx_ant;

  // Flag that indicates enable (TRUE) or disable (FALSE) command
  boolean enable;
  // RxAGC payload from ML1 to FW. FW wil ignore the payload if
  // RxAGC is disabled or during a Rx antenna disable request
  lte_LL1_sys_rxagc_info_struct rx_agc_info;
#endif
  // Num of RX to be enabled (max 2)
  uint8 num_rx_to_enable;

  // Mapping table for physical to logical_rx[2] ...
  // {1,0} means Rx1 is PRx(logical index 0), RX0 is DRx(logical index 1)
  // Array index is logical index, value is physical index.
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT];

  /*TODO:@viswa:
   * Info: rxagc info for 2rx.
   * - 'lte_LL1_sys_rxagc_info_struct' has AGC params for 1rx(different from HI), for 2rx
   *    two instances are added.
   * - 'rxagc_start_mode' will be part of 'lte_LL1_sys_rxagc_info_struct'
   * - 'num_rx_agc_params' is added which indicates how many instances of 'rx_agc_params'
   * - 'lte_LL1_sys_rxagc_params_struct' is not part of HA definitions to avoid confusion
   * - 'lte_LL1_sys_rxagc_info_struct' definition is different in HI & HA.
   * - */
  // RxAGC mode, should be the same as primary RxAGC mode (probably more suited for
  // confirmation payload than request)
  /*TODO: @viswa: not needed - cleanup */
  //lte_LL1_rxagc_mode_e rxagc_start_mode;

  /*Todo:@viswa: not needed - cleanup */
#if 0
  //boolean one_shot_pending;
#endif

  // Num of Rx for which info is stored in rx_agc_params[]
  uint8 num_rx_agc_params;
  // Rx AGC parameters - max of (LTE_LL1_MAX_NUM_UE_RX_ANT)
  lte_LL1_sys_rxagc_info_struct rx_agc_params[LTE_LL1_MAX_NUM_UE_RX_ANT];

  // RxAGC payload from ML1 to FW. FW wil ignore the payload if
  // RxAGC is disabled or during a Rx antenna disable request
  /*TODO:@viswa - old instance - clean up */
  //lte_LL1_sys_rxagc_info_struct rx_agc_info;

  // Tau info per Rx
  lte_LL1_sys_rx_timeout_info_struct rx_timeout_info;

  // CSF info for this ARD request
  lte_LL1_sys_ard_csf_info_t csf_info;

}lte_LL1_sys_rx_ant_mode_chg_req_carrier_payload_t;

typedef struct
{
  // Common request header
  lte_LL1_req_header_struct req_hdr;
  /* for non_ard - disable diversity Rx
   * - default value set by ML1 is false
   * - for NON-ARD functionality set this to true
   * - NON-ARD use cases:
   *    - FBRX cases we disable diversity RX - RX chain, ADC disable, RXFE disable
   *    - DTA - diversity tune away, disable diversity rx - disable rx chain, ADC disable, RXFW disable
   * - ARD use case:
   *   - ARD will be similar to PDCCH 1rx
   *   - disable rx chain, ADC clock gating, no changes to RXFE
   *   -
   *   */
  boolean notARD;
  /* For FW internal usage.
   * for implicit switching case cnf is not required to be sent to ML1 */
  boolean cnfNotRequired;
  // Number of carriers to perform Rx antenna mode change
  uint8 num_carriers; 
  // Per carrier payload
  lte_LL1_sys_rx_ant_mode_chg_req_carrier_payload_t carrier[LTE_LL1_DL_NUM_CARRIERS];
}lte_LL1_sys_rx_ant_mode_chg_req_struct;

LTE_LL1_INTF_MSG ( sys_rx_ant_mode_chg_req );


/*! @brief Message to enable/disable 1 Rx
*   This request message will enable/disable 1Rx on a given carrier
*/
typedef struct
{
  /* Enum for dl carrier on which ONE Rx to be enabled/disabled */
  lte_LL1_carrier_type_enum      carrierType;

  /* TRUE:  Enable 1Rx (move from 2Rx to 1Rx)
     FALSE: Disable 1Rx (move from 1Rx to 2Rx) */
  boolean                        enable1Rx;

  /* Flag to indicate whether to touch only RF/RxFE:
     TRUE:  Effect is limited to RF/RxFE only. Demfront/Silver are not 
            touched.
     FALSE: All the modules take effect of 1Rx/2Rx transition */
  boolean                        toggleRfOnly;

  /* Mask to indicate whether the carrier is in intra-band with any
     other carrier.
     BIT 0 for carrier 0 and BIT 1 for carrier 1.
     E.g. 0x3 - means PCC and SCC are intra-band. */
  uint16                         intraBandCarrierMask;

  /* RxAGC payload from ML1 to FW. FW will ignore the payload 
     during a 1Rx disable request */
  lte_LL1_sys_rxagc_info_struct  rxAgcInfo;

} lte_LL1_sys_one_rx_req_carrier_payload_t;

typedef struct
{
  /* Common request header */
  lte_LL1_req_header_struct  reqHdr;

  /* Number of carriers to perform 1Rx enable/disable */
  uint8                      nCarriers; 

  /* Per carrier payload */
  lte_LL1_sys_one_rx_req_carrier_payload_t  carrier[LTE_LL1_DL_NUM_CARRIERS];

} lte_LL1_sys_one_rx_req_struct;

LTE_LL1_INTF_MSG(sys_one_rx_req);

/*! @brief Confirmation to diversity Rx enable/disable
*/

typedef struct
{
  // Enum for dl carrier on which the Rx antenna mode was requested
  lte_LL1_carrier_type_enum carrier;
  /*TODO:@viswa: Cleanup */
#if 0
  // Rx antenna that was enabled or disabled
  //lte_LL1_ue_rx_ant_enum rx_ant;
#endif
  // Num of RX enabled in FW after current CMD (max 4)
  uint8 num_rx_enabled;

  // Mapping table for physical to logical_rx[4] ...
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT];

  /*TODO:@viswa - expanded agc info struct - clean this as we dont need HI def here */
#if 0
  // Rx AGC payload at the time of Rx antenna disable request
  // Not populated during enable request (validity indicator is cleared)
  lte_LL1_sys_rxagc_info_struct rx_agc_info;
  //lte_LL1_sys_rxagc_info_struct rx_agc_info;
  lte_LL1_sys_rxagc_info_struct rx_agc_info;
#endif

  // Num of Rx for which info is stored in rx_agc_params[]
  uint8 num_rx_agc_params;
  // Rx AGC parameters - max of (LTE_LL1_MAX_NUM_UE_RX_ANT)
  lte_LL1_sys_rxagc_info_struct rx_agc_params[LTE_LL1_MAX_NUM_UE_RX_ANT];
}lte_LL1_sys_rx_ant_mode_chg_cnf_carrier_payload_t;

typedef struct 
{
  // Confirmation message header
  lte_LL1_cnf_header_struct  cnf_hdr;
  // Number of carriers on which RX_ANT_MODE_CHG was performed
  uint8 num_carriers; 
  // Debug field - PCC Frame number, SF number when the Rx antenna mode change was
  // executed (before CNF send). Invalid for ASYNC Rx antenna mode change request
  lte_LL1_sys_time_struct mode_chg_action_time;
  // Debug field - Universal STMR value after Rx antenna mode change
  // sequence is executed (i.e. before CNF send). Invalid for ASYNC Rx antenna
  // mode change request
  uint32 mode_chg_univ_stmr;
  // Per carrier confirmation payload
  lte_LL1_sys_rx_ant_mode_chg_cnf_carrier_payload_t carrier[LTE_LL1_DL_NUM_CARRIERS];
}lte_LL1_sys_rx_ant_mode_chg_cnf_struct;

LTE_LL1_INTF_MSG( sys_rx_ant_mode_chg_cnf );


/*! @brief Confirmation to REQ ONE_RX
*/

typedef struct
{
  /* Enum for dl carrier on which 1Rx enable/disable was requested */
  lte_LL1_carrier_type_enum      carrierType;

  /* Rx AGC payload at the time of 1Rx disable request
     Not populated during enable request (validity indicator is cleared) */
  lte_LL1_sys_rxagc_info_struct  rxAgcInfo;

} lte_LL1_sys_one_rx_cnf_carrier_payload_t;

typedef struct 
{
  /* Confirmation message header */
  lte_LL1_cnf_header_struct  cnfHdr;

  /* Number of carriers on which ONE_RX was performed */
  uint8                      nCarriers; 

  /* SF number when 1Rx was executed (before CNF send). */
  lte_LL1_sys_time_struct    modeChgActionTime;

  /* Per carrier confirmation payload */
  lte_LL1_sys_one_rx_cnf_carrier_payload_t  carrier[LTE_LL1_DL_NUM_CARRIERS];

} lte_LL1_sys_one_rx_cnf_struct;

LTE_LL1_INTF_MSG(sys_one_rx_cnf);

typedef struct
{
  // Common request header
  lte_LL1_req_header_struct req_hdr;
  // reverse mapping table, VPE index based
  //[0]: rx index for v0, [1]: v1, [2]: v2, [3]: v3
  //default mapping (rx0->v0): [0]=0,[1]=1,[2]=2,[3]=3
  lte_LL1_dual_demod_map_struct ant_map;
}lte_LL1_sys_dual_demod_map_chg_req_struct;

LTE_LL1_INTF_MSG ( sys_dual_demod_map_chg_req );

/*! @brief Confirmation to diversity Rx enable/disable
*/

typedef struct 
{
  // Confirmation message header
  lte_LL1_cnf_header_struct  cnf_hdr;
  //boolean map_succesful;
}lte_LL1_sys_dual_demod_map_chg_cnf_struct;

LTE_LL1_INTF_MSG( sys_dual_demod_map_chg_cnf );

/*! @brief Indication message to report ftl_freeze info */

typedef enum
{
  NO_HST,
  FTL_JUMP,
  TTL_JUMP,
  ONESHOT_TTL,
}lte_LL1_hst_trigger_type_enum;

typedef struct {
  lte_LL1_sys_time_struct       sys_time;       ///< new system time
  boolean                       freeze_ftl;     ///< True if FW detects large frequency FTL and freezes FTL loop
  lte_LL1_hst_trigger_type_enum hst_trigger_type;
} lte_LL1_sys_ftl_freeze_ind_struct;

LTE_LL1_INTF_MSG( sys_ftl_freeze_ind );


// --------------------------------------------------------------------------
  //     
// Sample recording done indication
  // 
// --------------------------------------------------------------------------
typedef struct
{
  lte_LL1_carrier_type_enum carrier_type; ///< Enum for dl carriers
  uint16 subframe_frame_ss_off;  ///SS Off SF/SFN: bits 0~3  = subframe number, bits 4~13 = frame number
} lte_LL1_sys_traffic_mode_sample_rec_done_ind_carrier_payload_struct;

/*! @brief  Sample recording done indication message.
            This indication is sent only after getting 
            config app to disable SCC and after demfront 
            disable is done, while still processing pipeline flush.  
*/
typedef struct { 

  uint8 num_carriers;                         ///< Number of carriers
  lte_LL1_sys_traffic_mode_sample_rec_done_ind_carrier_payload_struct carrier[LTE_LL1_DL_NUM_CARRIERS ]; ///< parameters per carrier.
} lte_LL1_sys_traffic_mode_sample_rec_done_ind_struct;

LTE_LL1_INTF_MSG( sys_traffic_mode_sample_rec_done_ind );

/* Req message (from ML1 to FW) to disable uSleep */
typedef struct
{
  /* Common request header */
  lte_LL1_req_header_struct req_hdr;

  /* ML1 sets this to TRUE when usleep task scheduling needs to be gated by FW */
  boolean disable_usleep_event_in_fw[LTE_LL1_DL_NUM_CARRIERS];   
} lte_LL1_sys_usleep_disable_req_struct;

LTE_LL1_INTF_MSG ( sys_usleep_disable_req );

/* Cnf message (from FW to ML1) after disabling uSleep */
typedef struct 
{
  lte_LL1_cnf_header_struct cnf_hdr;            ///< common cnf header

  uint8 carrier_bitmask; /* bitmask of carriers that disabled uSleep (either statically disabled in uK or uSleep event gated by FW)     */
                         /* bit X corresponds to carrier X                                                                              */
                         /* in case of static feature disable, FW sets this to 0x7 (to mean all 3 carriers -- PCC, SCC0 and SCC1)       */
} lte_LL1_sys_usleep_disable_cnf_struct;

LTE_LL1_INTF_MSG( sys_usleep_disable_cnf );

/*! Maximum number of LTE-D discovery Resource Pools */
#define LTED_MAX_NUM_RP  16

/*! LTE-D downlink schedule request message */
typedef struct {

  /*! Common request parameters */
  lte_LL1_req_header_struct req_hdr;
  
  /*! TRUE to reset the internal Firmware HARQ logic for LTE-D. 
      This is an indication for Firmarwe to take control of the HARQ buffer
      for LTE-D use, typically when the discovery period starts
  */
  boolean reset_harq; 

  /*! DRID index to be logged, referred to RV = 0 */
  uint8 drid_to_log;

  /*! Number of Resource Pools configured in this structure 
      (length of rp_config array)
  */
  int8 num_active_rp; 

  /*! Resource pool configuration, for the current subframe */
  struct {

    /*! Resource Pool ID */
    uint8 rp_id;        

    /*! Cyclic Prefix type */
    lte_l1_cp_e cp_type;     
    
    /*! Number of retransmissions */  
    uint8 num_retx;     

    /*! Start PRB index for the lower spectrum allocation */
    int8 start_prb1;

    /*! End PRB index for the lower spectrum allocation */
    int8 end_prb1;
        
    /*! Start PRB index for the higher spectrum allocation */
    int8 start_prb2;

    /*! End PRB index for the higher spectrum allocation */
    int8 end_prb2;

    /*! Redundancy Version index */
    uint8 rv_index;     

    /*! New Data Indicator for current Resource Pool. 
        If set to TRUE, the current subframe will not be recombined with
        previous subframes
    */
    uint8 ndi;

  } rp_config[LTED_MAX_NUM_RP]; 

  /*! TRUE to indicate start of discovery period for Firmware .
       This is an indication for Firmware to start using lted configuration to toggle
       RX ON/OFF (Moved to the end so that no need to alter sys/fw vectors)
   */
   boolean lted_enable;

   /*! TRUE to indicate end of discovery period for Firmware .
       This is an indication for Firmware to stop using lted configuration to toggle
       RX ON/OFF (Moved to the end so that no need to alter sys/fw vectors)
   */
   boolean lted_disable;

} lte_LL1_sys_lted_sched_req_struct;

LTE_LL1_INTF_MSG(sys_lted_sched_req);

/*! LTE-D downlink schedule confirmation message */
typedef struct
{
  /*! Common confirmation parameters */
  lte_LL1_cnf_header_struct cnf_hdr;      

} lte_LL1_sys_lted_sched_cnf_struct;

LTE_LL1_INTF_MSG(sys_lted_sched_cnf);

/*! @brief FBRX request message 
*/
typedef struct
{
  lte_LL1_req_header_struct req_hdr; // Common req header           
  uint32 frame_num;  /* FBRX gap OTA frame num*/
  uint32 subframe_num;  /* FBRX gap OTA subframe num*/
  boolean fbrx_tuneaway;  /* Need Tuneaway*/
  boolean pcc_drx_disruption;  /* FBRX needs to use PCC DRx*/
}lte_LL1_sys_fbrx_req_struct;

LTE_LL1_INTF_MSG( sys_fbrx_req );

typedef enum
{
  LTE_LL1_FBRX_CNF_INVALID = 0,
  LTE_LL1_FBRX_CNF_SUCCESS = 1,  /*Successful FBRX capture*/
  LTE_LL1_FBRX_CNF_SKIP_NBSWITCH_CONFLICT = 2,  /*FBRX capture skipped due to NBSwitch Conflict*/
  LTE_LL1_FBRX_CNF_SKIP_CFGAPP_CONFLICT = 3,  /*FBRX capture skipped due to CfgApp conflict*/
  LTE_LL1_FBRX_CNF_SKIP_UL_TIMELINE_MISS = 4,  /*FBRX capture skipped due to UL timeline miss before calling common FW*/
  LTE_LL1_FBRX_CNF_SKIP_DTXSF = 5,  /*FBRX capture skipped since FBRX is on UL DTX SF*/
  LTE_LL1_FBRX_CNF_SKIP_CDRX = 6,  /*FBRX capture skipped since FBRX is on 1st SF after CDRX wakeup*/
  LTE_LL1_FBRX_CNF_SKIP_GAPTIME_ERROR = 7,  /*FBRX capture skipped since gap timing is wrong*/
  LTE_LL1_FBRX_CNF_SKIP_MSGR_LATE = 8,  /*FBRX capture skipped since cmd arrived late at msgr*/
  LTE_LL1_FBRX_CNF_SKIP_OTHER = 9,  /*FBRX capture skipped*/
  LTE_LL1_FBRX_CNF_SKIP_CONFLICT_CHECK_ACTIVE = 10, /* FBRX capture skipped if conflict check active */
  LTE_LL1_FBRX_CNF_SKIP_SRCH_ACTIVE = 11, /* FBRX capture skipped if nb srch is active */
  LTE_LL1_FBRX_CNF_SKIP_SCC_MEAS_ACTIVE = 12, /* FBRX capture skipped if fbrx scheduled on scc and meas is active*/
  LTE_LL1_FBRX_CNF_SKIP_1RX_PDCCH_ACTIVE = 13, /* FBRX capture skipped if UE is in 1Rx mode*/
  LTE_LL1_FBRX_CNF_SKIP_MEAS_ACTIVE = 14, /* FBRX capture skipped if UE is doing Meas*/  
  LTE_LL1_FBRX_CNF_SKIP_PBCH_ACTIVE = 15, /* FBRX capture skipped if UE is doing PBCH*/  
  LTE_LL1_FBRX_CNF_SKIP_1RX_ARD_ACTIVE = 16 /* FBRX capture skipped if UE is in ARD 1Rx mode */
}lte_LL1_fbrx_cnf_enum;

/*! @brief FBRX Confirmation message
*/
typedef struct
{
  lte_LL1_cnf_header_struct    cnf_hdr; // Common cnf header
  lte_LL1_fbrx_cnf_enum status;  /*FBRX capture status*/
  boolean tuneaway;  /* If Tuneaway was performed*/
} lte_LL1_sys_fbrx_cnf_struct;

LTE_LL1_INTF_MSG( sys_fbrx_cnf );

typedef struct {
  lte_LL1_carrier_type_enum carrier_type; 
  lte_LL1_usleep_mode_e mode;
} lte_LL1_usleep_mode_t;

typedef struct
{
  lte_LL1_req_header_struct req_hdr;

  uint8 num_carriers;  ///< Number of carriers to be configured.

  /* usleep mode */
  lte_LL1_usleep_mode_t usleep_mode[LTE_LL1_DL_NUM_CARRIERS];
} lte_LL1_sys_usleep_mode_switch_req_struct;

LTE_LL1_INTF_MSG( sys_usleep_mode_switch_req );

typedef struct 
{
  lte_LL1_cnf_header_struct cnf_hdr;

  uint8 num_carriers; ///< Number of carriers to be configured.

  /* usleep mode */
  lte_LL1_usleep_mode_t usleep_mode[LTE_LL1_DL_NUM_CARRIERS];
} lte_LL1_sys_usleep_mode_switch_cnf_struct;

LTE_LL1_INTF_MSG( sys_usleep_mode_switch_cnf );

typedef enum
{
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_NONE = 0,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_UPDATE_COLLISION_WITH_P_OR_AP,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_UPDATE_NOT_SCHEDULED,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_COLLISION_WITH_P_OR_AP,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_NOT_SCHEDULED,
  LTE_LL1_SYS_ARD_CRTL_CSF_ERROR_SPEFF_EVAL_CSIRS_NOT_PRESENT_SUBFN,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_INVALID_SUBFN_0,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_INVALID_SUBFN_1,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_INVALID_SUBFN_2,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_RESULTS_TIMEOUT,
  LTE_LL1_SYS_ARD_CTRL_LAA_ERROR_CRS_NOT_DETECTED,
  LTE_LL1_SYS_ARD_CTRL_FTL_RESULTS_NOT_UPDATED,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_UPDATE_EXCEED_CAPACITY,
  LTE_LL1_SYS_ARD_CTRL_CSF_ERROR_SPEFF_EVAL_EXCEED_CAPACITY
} lte_LL1_sys_ard_ctrl_csf_error_enum;

typedef enum
{
  LTE_LL1_SYS_ARD_CTRL_CSF_NO_RES = 0,
  LTE_LL1_SYS_ARD_CTRL_CSF_SPEFF_UPDATE_RES,
  LTE_LL1_SYS_ARD_CTRL_CSF_SPEFF_EVAL_RES
} lte_LL1_sys_ard_ctrl_csf_results_type_enum;


/* ARD ctrl to CSF ARD payload */
typedef struct
{
  /* Time of the evaluation */
  uint16 frame;
  uint16 subframe;

  /* Carrier index */
  lte_LL1_carrier_type_enum carrier;

  /* Results type */
  lte_LL1_sys_ard_ctrl_csf_results_type_enum results_type;

  /* Error code mask indexed by "lte_LL1_sys_ard_ctrl_csf_error_enum" */
  uint16 error_code_mask;

  /* The best 2 Rx mask (physical) */
  //uint8 best_2_physical_rx_bmask;

  /* The best 2 Rx mask (logical) */
  //uint8 best_2_logical_rx_bmask;

  /* 4 Rx SPEFF - S32Q16*/
  //uint32 speff_4rx[LTE_MAX_NUM_UE_RX_ANT];

  /* Best 2 Rx SPEFF - S32Q16*/
  //uint32 speff_best_2rx[LTE_LL1_UE_RX_ANT_2];

  /* FTL SNR */
  uint32 ftl_snr[LTE_MAX_NUM_UE_RX_ANT];

  /* RSSI value */
  int32 inst_rssi_rx[LTE_MAX_NUM_UE_RX_ANT];

  /* RSRP value */
  int32 inst_rsrp_rx[LTE_MAX_NUM_UE_RX_ANT];

  /* RSRQ value */
  int32 inst_rsrq_rx[LTE_MAX_NUM_UE_RX_ANT];

  /* Cell timing */
  uint32 cell_timing[LTE_MAX_NUM_UE_RX_ANT];

  /* Num of RX enabled in FW after current CMD (max 4)
     Use this to know which Rx has RSRP, RSRQ, RSSI and
     FTL results */
  uint32 num_phy_rx_enabled;

  /* Mapping table for physical to logical_rx[4] ... */
  lte_LL1_ue_rx_ant_enum rx_ant_phy_to_logic_mapping[LTE_MAX_NUM_UE_RX_ANT];

  /* Rx corr results int32 Q15 format
     (indexed by "lte_LL1_sys_ard_valid_rx_group_enum") */
  //int32 rx_corr_metric[LTE_LL1_ARD_RX_GROUP_MAX];

  /*1 if rx23 is better, 0 if rx01 is better - logical ant indices*/
  //boolean is_rx23_preferred_pair;

  /* Flag to indicate if autorefersh after eval is aborted*/
  //boolean refresh_aborted;

} lte_LL1_sys_ard_csf_carrier_results_t;

/* CSF ARD to ARD ctrl payload for the CSF results */
typedef struct
{
  // Total num of carriers which have the results
  uint32 num_carriers;

  // CSF response
  lte_LL1_sys_ard_csf_carrier_results_t carrier_results[LTE_LL1_DL_NUM_CARRIERS];

} lte_LL1_sys_ard_csf_results_ind_struct;

LTE_LL1_INTF_MSG( sys_ard_csf_results_ind );

/*! @brief Message to change the ARD CSF state, also has
*   the structure for new CSF info to be latched with the new state
*   FW cannot support getting it on the same SF on same CC as
*   Ant Mode Chg Req
*/
typedef struct
{
  // Enum for dl carrier on which the Rx antenna mode needs to be changed
  lte_LL1_carrier_type_enum carrier;

  // Info required for evaluation of the best 2 and for
  // fallback csf speff update
  // Not looked at by fw for ard_purpose RX_MAP_UPDATE
  lte_LL1_sys_ard_csf_info_t csf_info;

}lte_LL1_sys_ard_csf_state_chg_req_carrier_payload_t;

typedef struct
{
  // Common request header
  lte_LL1_req_header_struct req_hdr;

  // Number of carriers to perform Rx antenna mode change
  uint8 num_carriers;

  // Per carrier payload
  lte_LL1_sys_ard_csf_state_chg_req_carrier_payload_t carrier[LTE_LL1_DL_NUM_CARRIERS];

}lte_LL1_sys_ard_csf_state_chg_req_struct;

LTE_LL1_INTF_MSG ( sys_ard_csf_state_chg_req );


/*! @brief If the flag send_1rx_fail_ind is set in lte_LL1_smem_t, we need to send this IND to ML1 whenever
  there is a state change in 1rx/2rx
*/
typedef struct {
  uint32 frame_num;
  uint32 subframe_num;
  uint8  ll_id_mask;     // Mask to combine ind for multiple ccs
  lte_LL1_ue_rx_ant_enum phy_index[LTE_LL1_DL_NUM_CARRIERS];//echo back the phy id to ML1
}lte_LL1_sys_ard_c1rx_ind_struct;

LTE_LL1_INTF_MSG( sys_ard_c1rx_ind );

#endif /* INTF_SYS_H */

