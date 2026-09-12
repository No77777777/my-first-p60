
/*!
  @file
  lte_ml1_log_ext.h

  @brief
  This file describes the external log interfaces to LTE ML1
*/


/*===========================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/api/lte_ml1_log_ext.h#5 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/06/16   ymu     Feature LTED Ml1 Check in
09/11/15   dsp     CR 901376: Addded AFC Logging support, log packet 0xB16D
08/19/15   avj     Logging changes for FR 23578: Multi-cluster PUSCH
04/27/15   avj     Update log packet 0xB16B to support UL CA
02/03/15   jw      Update log packet 0xB16C to support UL CA
12/10/14   as      Update log packet 0xB173 to support mod_type from FW
10/25/13   ap      Adding support for DLM log packets
10/16/13   awj     Initial version
===========================================================================*/

#ifndef LTE_ML1_LOG_EXT_H
#define LTE_ML1_LOG_EXT_H
#include "lte_as.h"
#include "lte_cphy_msg.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! Maximum number of neighbor cells per request */
#define LTE_ML1_LOG_MAX_NCELLS_PER_REQ  16

#define LTE_ML1_MAX_NUM_UE_RX_ANT 2

/*! Maximum number of HRPD band config structures */
#define LTE_ML1_SM_LOG_HRPD_BAND_CONFIG_MAX  32

/* Max number of CCs currently supported (including primary)*/
#define LTE_ML1_GM_LOG_MAX_CC                                             3

/* Max number of CCs UL supported */
#define LTE_ML1_GM_LOG_MAX_UL_CC                                          4

/*! Maximum number of HRPD pilots */
#define LTE_ML1_SM_LOG_HRPD_PILOT_INFO_MAX   32

#define LTE_ML1_GM_LOG_MAX_PDCCH_RESULTS_PER_TTI                         (8)

#define LTE_ML1_GM_LOG_DCI_INFO_MAX_CNT                                 (20)

#define LTE_ML1_LOG_GM_PUCCH_TX_N_1_PUCCH_NUM                            (4)
/*! Maximum number of HRPD pilots */
#define LTE_ML1_SM_LOG_HRPD_PILOT_INFO_MAX   32

/* Max no. of SCells supported by CA Metrics log packet */
#define LTE_ML1_DLM_CA_LOG_MAX_SCELLS      1

#define LTE_ML1_LOG_MAX_MBSFN_ALLOCATIONS                       (8)
#define LTE_ML1_DLM_LOG_PDSCH_STAT_MAX_CNT                      (25)

#define LTE_ML1_DLM_LOG_BLER_STATS_CARRIER_MAX_CNT  LTE_LL1_CARRIER_COUNT

#define LTE_ML1_DLM_LOG_MAX_CARRIERS LTE_LL1_CARRIER_COUNT
#define LTE_ML1_DLM_LOG_LINK_CAP_MAX_CNT                        (10)
#define LTE_ML1_DLM_LOG_RES_AVAIL_FRAC_MAX_CNT                  (20)

#define LTE_ML1_BOLT_LOG_PACKET_BASE_VERSION                    (16)

#define LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT                          (4)
#define LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT_CORRELATION              (10)
#define LTE_ML1_LOG_DLM_HO_RXD_ARD_MAX_CNT                      (10)

#define LTE_ML1_LOG_DLM_ARD_BEST2_SPFF_CNT                      (2)
#define LTE_ML1_LOG_DLM_ARD_4RX_SPFF_CNT                        (4)
#define LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT                          (4)
#define LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT_CORRELATION              (10)
#define LTE_ML1_LOG_DLM_HO_RXD_ARD_MAX_CNT                      (10)
#define LTE_ML1_LOG_DLM_ARD_NUM_ANT_CORRELATION                 (6)

#define   ARD_LOG_INVALID_CARRIER_OR_RANK                       (15)
#define   ARD_LOG_INVALID_SCHED_OR_SNR                          (255)
#define   LTE_ML1_MAX_UE_RX_ANT                                 (4)


typedef enum
{
  MOD_BPSK   = 0,
  MOD_QPSK   = 1,
  MOD_16QAM = 2,
  MOD_64QAM = 3,
  MOD_TYPE_MAX_WIDTH = 0xFFFFFFFF, ///< @internal set enum to 32bits
}lte_ml1_log_modulation_type_e;

typedef enum
{
  DCI_0 = 0,
  DCI_1,
  DCI_1A,
  DCI_1B,
  DCI_1C,
  DCI_1D,
  DCI_2,
  DCI_2A,
  DCI_3,
  DCI_3A,
  DCI_2B,
  DCI_2C,
  DCI_NUM_FORMATS,
  DCI_FORMAT_MAX_WIDTH = 0xFFFFFFFF, ///< @internal set enum to 32bits
}lte_ml1_log_dci_format_e;	

typedef enum
{
  C_RNTI = 0,
  SPS_RNTI,
  P_RNTI,
  RA_RNTI,
  TEMP_RNTI,
  SI_RNTI,
  TPC_PUSCH_RNTI,
  TPC_PUCCH_RNTI,
  MBMS_RNTI,
  MAX_NUM_RNTI_TYPES,
  RNTI_TYPE_MAX_WIDTH = 0xFFFFFFFF, ///< @internal set enum to 32bits
}lte_ml1_log_rnti_type_e;
typedef enum
{
  FDD = 0,                    ///<  FDD mode (multiplexing & bundling are the same)
  TDD_ACK_MUX,                ///<  Multiplexing mode for TDD with single carrier
  TDD_ACK_BUNDLING,           ///<  Bundling mode for TDD with single carrier
  TDD_ACK_PUCCH_1B_CH_SEL,    ///< PUCCH FMT 1B with ch selection for TDD single cell                                         
  TDD_CA_ACK_PUCCH_1B_CH_SEL, ///< PUCCH FMT 1B with ch selection for TDD+CA
  ACK_PUCCH_3,          ///< PUCCH FMT 3 for FDD+CA, TDD with single carrier and TDD+CA        
  ACK_TPYE_MAX_WIDTH = 0xFFFFFFFF, ///< @internal set enum to 32bits
}lte_ml1_log_ul_ack_reporting_mode_type_e; 

/*! @brief Main log structure for all serving cell information */
typedef PACK(struct)
{
  /*! Version */
  uint16  version:8;

    /*! Downlink bandwidth */
  uint16  dl_bandwidth:8;

  /*! System frame number (at ref time) */
  uint16   sfn;

  ///< serving cell earfcn
  uint32  earfcn; 

  ///< serving  cell_ID
  uint16  cell_ID:9;

  ///< decode result: 1 = pass, 0= fail;
  uint16  decode_result:1;

  /*! The phich duration configures lower limit on the size of the control
      region signaled by the PCFICH. */
  uint16 phich_duration:3;

   /*! Parameter for determining the number of PHICH groups */
  uint16 phich_resource:3;

  uint16  reserved0;
  
   ///< serving cell PSS
  uint32  pss_corr_result;

  ///< serving cell PSS
  uint32  sss_corr_result;

  /*------------------------*/
  ///< 10 ms boundary;
  uint32 ref_time[2];

   ///< PBCH payload
  uint32 mib_payload;

   ///< updated frequency offset estimate (in Hz);
  ///< invalid if freq_offset_invalid
  uint16  freq_offset;

  ///< number of antennas for cell: 1, 2, or 4;
  uint16  num_antennas:2;


  uint16  reserved1:14;

} lte_ml1_sm_log_serv_cell_info_pkt_s;


/*! @brief Serving Cell Measurement & Evaluation
*/
typedef struct
{
  /*! Version info */
  uint32  version:8;
  /*! standards version */
  uint32  standards_version:8;

  uint32  reserved0:16;

  /*! E-ARFCN */
  uint32  earfcn;

  /*! Physical cell ID, 0 - 504 */
  uint16  phy_cell_id:9;
  /*! Serving cell priority */
  uint16 serv_layer_prio:4;
  /*! reserved */
  uint32  reserved1:19;

  /*! measured RSRP, -140 ~ -40 dBm*/
  uint32  measured_rsrp:12;
  /*! True measured RSRP, -180 ~ -30 dBm*/
  uint32  true_measured_rsrp:12;
  /*! reserved */
  uint32  reserved2:8;

  /*! averge RSRP, -140 ~ -40 dBm*/
  uint32  avg_rsrp:12;
  /*! True averge RSRP, -180 ~ -30 dBm*/
  uint32  true_avg_rsrp:12;
  /*! reserved */
  uint32  reserved3:8;

  /*! measured RSRQ, -30 ~ 0*/
  uint32  measured_rsrq:10;
  /*! True measured RSRQ, -30 ~ +10 */
  uint32  true_measured_rsrq:10;
  /*! averge RSRQ, -30 ~ 0 */
  uint32  avg_rsrq:10;
  /*! reserved */
  uint32  reserved4:2;

  /*! True averge RSRQ, -30 ~ +10 */
  uint32  true_avg_rsrq:10;
  /*! measured RSSI, -110 ~ -10 */
  uint32  measured_rssi:11;
  /*! reserved */
  uint32  reserved5:11;

  /*! Qrxlevmin */
  uint32  q_rxlevmin:6;
  /*! P_max, unit of db, -30 to 33, 0x00=-30, 0x1=-29, ...0x3F=33,
      0x40=NP for not present */
  uint32  p_max:7;
  /*! max UE Tx Power */
  uint32  max_ue_tx_pwr:6;
  /*! Srxlev */
  uint32  s_rxlev:7;
  /*! num drxs S < 0 */
  uint32  num_drx_S_fail:6;

  /*! S_intra_search*/
  uint32  s_intra_search:6;
  /*! S_non_intra_search */
  uint32  s_non_intr_search:6;
  /*! meas rules updated or not */
  uint32  meas_rules_updated:1;
  /*! Measurement rules */
  uint32  meas_rules:4;
  /*! reserved */
  uint32  reserved6:15;

#ifdef FEATURE_LTE_REL9
  /*! Range -34 to -3 */
  uint32  q_qualmin:7;
  /*! Range -35 to +34 */
  uint32  s_qual:7;
  /*! Range 0 to +31 */
  uint32  s_intra_search_q:6;
  /*! Range 0 to +31 */
  uint32  s_non_intra_search_q:6;
  uint32  reserved7:6;
#endif
} lte_ml1_sm_log_idle_serv_meas_eval_s;


/*! @brief Subpacket struct for serv cell measurement result*/
typedef struct
{
  /*! Physical cell ID, 0 ~ 504*/
  uint16 phy_cell_id:9;
  /*! Serving cell index: 0..7 */
  uint16 serv_cell_index:3;
  uint16 is_serv_cell:1;
  uint16 reserved_0:3;
  uint16 rserved_00;

  /*! System frame time when measurements are made, 0 ~ 1023*/
  uint32 current_sfn:10;
  /*! System subframe time when measurements are made, 0 ~ 9*/
  uint32 current_subfn:4;
  uint32 reserved_1:18;

  /*!Measurement done on restricted subframe , 0 or 1 */
  uint32 is_subfm_restricted:1;
  /*! cell timing for rx antenna 0, 0 ~ 307199*/
  uint32 cell_timing_0:19;
  uint32 reserved_2:12;

  /*! cell timing for rx antenna 1, 0 ~ 307199*/
  uint32 cell_timing_1:19;
  /*! SFN for rx antenna 0 corresponding to cell timing, 0 ~ 1023*/
  uint32 cell_timing_sfn_0:10;
  uint32 reserved_3:3;

  /*! SFN for rx antenna 1 corresponding to cell timing, 0 ~ 1023*/
  uint32 cell_timing_sfn_1:10;
  /*! Inst RSRP value for antenna 0, -140 ~ -40 dBm*/
  uint32 inst_rsrp_rx_0:12;
  uint32 reserved_4:10;

  /*! pathloss value for antenna 0, -180 ~ -30 dBm*/
  uint32 pathloss_rsrp_rx_0:12;
  /*! Inst RSRP value for antenna 1, -140 ~ -40 dBm*/
  uint32 inst_rsrp_rx_1:12;
  uint32 reserved_5:8;

  /*! Pathloss value for antenna 1, -180 ~ -30 dBm*/
  uint32 pathloss_rsrp_rx_1:12;
  /*! Inst RSRP value combined across tx-rx pairs, -140 ~ -40 dBm*/
  uint32 inst_rsrp:12;
  uint32 reserved_6:8;

  /*! Filtered RSRP */
  uint32 filtered_rsrp:12;
  /*! Inst RSRQ value for antenna 0, -30 to 0 dBm*/
  uint32 inst_rsrq_rx_0:10;
  /*! True inst RSRQ value for antenna 0, -30 to +10 dBm*/
  uint32 reserved_7:10;

  /*! Inst RSRQ value for antenna 0, -30 to 0 dBm*/
  uint32 inst_rsrq_rx_1:10;
  /*! Inst RSRQ value combined across tx-rx pairs, -30 ~ 0 dBm*/
  uint32 inst_rsrq:10;
  uint32 reserved_8:12;

  /*! Filtered RSRQ */
  uint32 filtered_rsrq:12;
  /*! Inst RSSI value for antenna 0, -110 ~ -10 dBm*/
  uint32 inst_rssi_rx_0:11;
  uint32 reserved_9:9;
  

  /*! Inst RSSI value for antenna 1, -110 ~ -10 dBm*/
  uint32 inst_rssi_rx_1:11;
  /*! Inst RSRQ value combined across tx-rx pairs, -110 ~ -10 dBm*/
  uint32 inst_rssi:11;
  uint32 reserved_10:10;

  /*! DVGA gain in log domain for antenna 0 and 1*/
  int16  dvga_rx_log_0;
  int16  dvga_rx_log_1;
  /*! LNA gain in log domain for antenna 0 and 1*/
  int16  lna_gain_rx_log_0;
  int16  lna_gain_rx_log_1;

  /*! Wideband CQI */
  uint16 cqi_web;
  /*! Residual frequency error */
  int16  res_freq_error;

  /*! FTL-SNR in linear scale for antenna 0 and 1*/
  uint32 ftl_snr_linear_rx_0;
  uint32 ftl_snr_linear_rx_1;
  /*! FTL-SNR value for antenna 0, -20 to 30 dBm*/
  uint32 ftl_snr_rx_0:9;
  /*! FTL-SNR value for antenna 1, -20 to 30 dBm*/
  uint32 ftl_snr_rx_1:9;
  uint32 reserved_11:14;

  /*! RS-SNR in linear scale for antenna 0 and 1*/
  uint32 rs_snr_linear_rx_0;
  uint32 rs_snr_linear_rx_1;
  /*! RS-SNR value for antenna 0, -20 to 30 dBm*/
  uint32 rs_snr_rx_0:9;
  /*! RS-SNR value for antenna 1, -20 to 30 dBm*/
  uint32 rs_snr_rx_1:9;
  uint32 reserved_12:14;

  /*! Projected SIR in Q_format 4 */
  int32 projected_sir;
}lte_ml1_sm_log_meas_result_per_cell_s;

typedef struct
{
  /*! E-ARFCN, 0 ~ 39649 */
  uint32 earfcn; 

  uint8 num_cells;
  uint8 valid_rx;///< 0  NA
                  ///< 1  RX0
                  ///< 2  RX1
                  ///< 3  RX0_RX1
				  
  /* Valid values for logical_to_phy_rx_map are 0,1 and invalid or default is 0xFF or 255.*/
  uint8                                   logical_to_phy_rx_map[LTE_ML1_MAX_NUM_UE_RX_ANT];
  
  lte_ml1_sm_log_meas_result_per_cell_s   meas_result[1 + LTE_MAX_NUM_CRS_IC_CELLS];

} lte_ml1_sm_log_serv_cell_meas_response_subpkt_s;


/*! @brief Idle cell measurement info struct */
typedef struct
{
  /*! physical cell id, 0 ~ 503*/
  uint32 cell_id:9;
  /*! Cumulative frequency offset, 0 ~ 0xffff*/
  uint32 ftl_cumu_freq_offset:16;
  uint32 reserved_0:7;

  /*! Inst RSRP value for rx antenna 0, -140 ~ -40 dBm*/
  uint32 inst_rsrp_rx_0:12;
  /*! True Inst RSRP value for rx antenna 0, -180 ~ -30 dBm*/
  uint32 true_inst_rsrp_rx_0:12;
  uint32 reserved_1:8;

  /*! Inst RSRP value for rx antenna 1, -140 ~ -40 dBm*/
  uint32 inst_rsrp_rx_1:12;
  /*! True Inst RSRP value for rx antenna 1, -180 ~ -30 dBm*/
  uint32 true_inst_rsrp_rx_1:12;
  uint32 reserved_2:8;

  /*! Inst RSRP value combined across tx-rx pairs, -140 ~ -40 dBm*/
  uint32 inst_rsrp:12;
  /*! True Inst RSRP value combined across tx-rx pairs, -180 ~ -30 dBm*/
  uint32 true_inst_rsrp:12;
  uint32 reserved_3:8;

  /*! Inst RSRQ value for rx antenna 0, -30 ~ 0 dBm*/
  uint32 inst_rsrq_rx_0:10;
  /*! True Inst RSRQ value for rx antenna 0, -30 ~ +10 dBm*/
  uint32 true_inst_rsrq_rx_0:10;
  /*! Inst RSRQ value for rx antenna 1, -30 ~ 0 dBm*/
  uint32 inst_rsrq_rx_1:10;
  uint32 reserved_4:2;

  /*! True Inst RSRQ value for rx antenna 1, -30 ~ +10 dBm*/
  uint32 true_inst_rsrq_rx_1:10;
  /*! Inst RSRQ value combined across tx-rx pairs, -30 ~ 0 dBm*/
  uint32 inst_rsrq:10;
  /*! True Inst RSRQ value combined across tx-rx pairs, -30 ~ 0 dBm*/
  uint32 true_inst_rsrq:10;
  uint32 reserved_5:2;

  /*! Inst RSRQ value for rx antenna 0, -110 ~ -10 dBm*/
  uint32 inst_rssi_rx_0:11;
  uint32 reserved_6:21;

  /*! Inst RSSI value for rx antenna 0, -110 ~ -10 dBm*/
  uint32 inst_rssi_rx_1:11;
  /*! Inst RSSI value combined acros tx-rx pairs, -110 ~ -10 dBm*/
  uint32 inst_rssi:11;
  uint32 reserved_7:10;

  /*! DVGA gain in log domain for antenna 0 and 1*/
  int32  dvga_rx_log_0:16;
  int32  dvga_rx_log_1:16;
  /*! LNA gain in log domain for antenna 0 and 1*/
  int32  lna_gain_rx_log_0:16;
  int32  lna_gain_rx_log_1:16;

  /*! Frame bounrary ref time 0, 0 ~ 0xffff_ffff*/
  uint32 frame_bdry_ref_time_0;
  /*! Frame bounrary ref time 1, 0 ~ 0xffff_ffff*/
  uint32 frame_bdry_ref_time_1;

  /*! Total timing adj to CIR 0, 0 ~ 0xffff*/
  uint32 total_timing_adj_cir_0:16;
  /*! Total timing adj to CIR 1, 0 ~ 0xffff*/
  uint32 total_timing_adj_cir_1:16;

} lte_ml1_sm_log_idle_ncell_meas_response_info_s;


/*! @brief Subpacket struct for idle cell measurement response*/
typedef struct
{
  /*! Cell frequency measured*/
  uint32 earfcn:32;

  /*! Number of cell measurements, 0 ~ 16 */
  uint32 num_cells:5;
  /*! Current count of SWRP in MP, 1 ~ 4*/
  uint32 cnt_swrp:2;
  uint32 duplexing_mode:2;
  /*! Serving cell index: 0..7 */
  uint32 serv_cell_index:3;
  uint32 reserved_0:20;

  /*! Cell measuerment info list */
  lte_ml1_sm_log_idle_ncell_meas_response_info_s
        meas_resp_info_list[LTE_ML1_LOG_MAX_NCELLS_PER_REQ];

} lte_ml1_sm_log_ncell_meas_response_subpkt_s;


/*! @brief Connected neighbor cell meas paramters*/
typedef struct
{
  /*! SW reporting period in ms, 40, 80 or 160*/
  uint16 swrp:8;
  /*! Scheduling period in ms, 40, 80 or 160*/
  uint16 sp:8;

  /*! ncell occasion enum*/
  uint16 occasion:2;
  /*! Number of scheduling periods per measurement period, 1, 2 or 4*/
  uint16 num_sp_per_mp:3;
  /*! Duration of measurement gap in ms, cannot be more than 15 */
  uint16 gap_duration:4;
  /*! One bit per scheduling period.
    0: no gap
    1:Measurement gap in the end of scheduling period */
  uint16 gap_mask:1;
  uint16 reserved:6;

} lte_ml1_sm_log_ncell_conn_s;


/*! @brief Idle neighbor cell measurement request paramters*/
typedef struct
{
  /*! Physical cell id, 0 ~ 503 */
  uint16 cell_id:9;

  /*! cell cyclic prefix type
      0x00 = LTE_L1_CP_MODE_NORMAL
      0x01 = LTE_L1_CP_MODE_EXTENDED
      0x02 = LTE_L1_CP_MODE_EXTENDED_MBSFN
      0x03 = LTE_L1_MAX_NUM_CP_MODES*/
  uint16 cp_type:2;

  /*! Number of tx antenna for the eNb, 1 or 2*/
  uint16 eNb_tx_ant:2;
  uint16 ttl_enable:1;
  uint16 ftl_enable:1;
  uint16 reserved_0:1;

  /*! valid if ftl_enable is true*/
  uint16 ftl_cumu_freq_offset;

  /*! valid if ttl_enable is true*/
  uint32 frame_bdry_ref_time_0;
  uint32 frame_bdry_ref_time_1;

  uint16 total_timing_adj_cir_0;
  uint16 total_timing_adj_cir_1;

} lte_ml1_sm_log_ncell_meas_req_param_s;


/*! @brief Subpacket struct for connected ncell meas request*/
typedef struct
{
  /*! Cell earfcn, 0 ~ 65535 */
  uint32 earfcn:32;

  /*! Number of neighbor cells to measure, 1 ~ 16 */
  uint32 num_ncells:4;
  /*! Number of TX antennas, 1 ~ 2 */
  uint32 num_rx_ant:2;
  /*! Serving cell index: 0..7 */
  uint32 serv_cell_index:3;
  uint32 reserved_0:23;

  lte_ml1_sm_log_ncell_conn_s conn_params;

  lte_ml1_sm_log_ncell_meas_req_param_s
          cell_list[LTE_ML1_LOG_MAX_NCELLS_PER_REQ];

} lte_ml1_sm_log_conn_ncell_meas_req_subpkt_s;


/*! @brief DRX Request
*/
typedef struct
{
  /*! Version info */
  uint32  version:8;
  uint32  reserved0:24;

  /*! E-ARFCN Range: 0-262143*/
  uint32   earfcn;  /* 32 bits */

  /*! Physical cell ID, 0 - 504 */
  uint16  phy_cell_id;

  /*! paging cycle index 0x0 = 320 ms
	0x1 = 640 ms
	0x2 = 1280 ms
	0x3 = 2560 ms
	*/
  uint8   paging_cycle_index;

	/*! Nb 0x0 = fourT 0x1 = twoT 0x2 = oneT 0x3 =  halfT 0x4 = quarterT
	0x5 = oneEightT 0x6 = onSixteenthT 0x7 = oneThirtySecondT*/
  uint8   nb;

  /*! UE ID IMSI mod 1024. Range 0 to 1023 */
  uint16 ueid;

	/*! Paging SFN offset Range 0 to 1023 */
  uint16  paging_occ_sfn_offset;

	/*! Paging sub-FN offset Range 0 to 9*/
  uint8  paging_occ_subfn_offset;

  /*! reserved */
  uint32  reserved1:24;

} lte_ml1_common_log_paging_drx_info_s;


typedef struct
{
  /*! Results pertaining to each cell */
  /* PN position In units of chipx8 Range 0...307200 (38400*8) */
  uint32                                          pn_pos;

  /*!Scrambling code for this peak  Range 0..511*/
  uint16                                          psc;

  /*********************************************************************
  ** Units of the three items below (RSSI, RSCP, Ec/Io) are two times **
  ** the actual dBm value, giving a resolution for each of 0.5 dBm.   **
  *********************************************************************/

  /*!The received wide band power, including thermal noise and
  noise generated in the receiver,within the bandwidth defined
  by the receiver pulse shaping filter Ref 3gpp 25.215*/
  int16                                           cpich_rssi;

  /*!Received Signal Code Power, the received power on one code
  measured on the Primary CPICH. Ref 3gpp 25.215*/
  int16                                           cpich_rscp;

  /*!The received energy per chip divided by the power density
  in the band. The CPICH Ec/No is identical to CPICH RSCP/UTRA
  Carrier RSSI. Measurement shall be performed on the Primary
  CPICH. Ref : 3gpp spec 25.215*/
  int16                                           cpich_ecno;

  /*!Filtered RSCP*/
  int16                                           cpich_rscp_filtered;

  /*!Filtered ECNO*/
  int16                                           cpich_ecno_filtered;

  /*!If cell is newly detected or configured */
  uint32										  cell_mew_detected;
}lte_ml1_sm_log_conn_meas_wcdma_cell_results_s;


/*! CDMA band class configuration structure */
typedef struct
{
  /*! Band class (Range 0...31) */
  uint8 band;
  /*! Cell reselection priority (Range 0...7) */
  uint8 priority;
  /*! Threshold, high-prio layer (Range 0...63) */
  uint8 threshX_high;
  /*! Threshold, low-prio layer (Range 0...63) */
  uint8 threshX_low;

} lte_ml1_sm_log_idle_cdma_meas_config_band_config_s;


/*! CDMA neighbor pilot information structure */
typedef struct
{
  /*! Band class (Range 0...31) */
  uint8 band;
  /*! Reserved */
  uint8  reserved1;
  uint16 reserved2;
  /*! Channel (Range 0...2047) */
  uint16 channel;
  /*! Pilot id (Range 0...511) */
  uint16 pilot;

} lte_ml1_sm_log_idle_cdma_meas_config_pilot_info_s;


/*! CDMA reselection parameters structure */
typedef struct
{
  /*! Treselection in seconds (Range 0...7) */
  uint8 Tresel;
  /*! Medium mobility scaling factor
     0x01 = 0.25, 0x2 = 0.50, 0x3 = 0.75, 0x4 = 1.00 */
  uint8 Tresel_medium;
  /*! High mobility scaling factor
     0x01 = 0.25, 0x2 = 0.50, 0x3 = 0.75, 0x4 = 1.00 */
  uint8 Tresel_high;
  /*! Number of band class configurations */
  uint8 num_bands;
  /*! Number of neighbor pilots */
  uint8 num_pilots;
  /*! Reserved */
  uint8  reserved1;
  uint16 reserved2;
  /*! Band class configuration list */
  lte_ml1_sm_log_idle_cdma_meas_config_band_config_s
    band_list[LTE_ML1_SM_LOG_HRPD_BAND_CONFIG_MAX];
  /*! Neighbor pilot list */
  lte_ml1_sm_log_idle_cdma_meas_config_pilot_info_s
    pilot_list[LTE_ML1_SM_LOG_HRPD_PILOT_INFO_MAX];

} lte_ml1_sm_log_idle_cdma_meas_config_resel_params_s;


/*! LTE ML1 CDMA Idle Meas Config Subpacket */
typedef struct
{
  /*! CDMA window size  (Range 0...15) */
  uint8 window_size;
  /*! Reserved */
  uint8  reserved1;
  uint16 reserved2;
  /*! HRPD reselection parameters */
  lte_ml1_sm_log_idle_cdma_meas_config_resel_params_s hrpd;
  /*! 1x reselection parameters */
  lte_ml1_sm_log_idle_cdma_meas_config_resel_params_s onex;
  lte_mem_instance_type inst_num;
} lte_ml1_sm_log_idle_cdma_meas_config_subpkt_s;

/*! @brief WCDMA Idle Measurement Configuration subpackets*/
typedef struct
{
  uint16  frequency:       16;
  uint16  cell_resel_prio:  8;
  uint16  thresh_high:      8;

  uint16  thresh_low:       8;
  uint16  Qrxlevmin_act:    8;
  uint16  P_max:            8;
  uint16  Q_qual_min:       8;

} lte_ml1_sm_log_idle_wcdma_meas_config_freq_info_s;


typedef struct
{
  uint8   tresel:         8;
  uint8   sf_med:         8;
  uint8   sf_high:        8;
  uint8   num_wcdma_freq: 8;

  lte_ml1_sm_log_idle_wcdma_meas_config_freq_info_s
      frequency [LTE_CPHY_MEAS_MAX_WCDMA_FREQ];
} lte_ml1_sm_log_idle_wcdma_meas_config_subpkt_s;


typedef struct
{
  uint16 freq;
  uint16 padding;
} lte_ml1_sm_log_idle_gsm_meas_config_freq_s;


typedef struct
{
  uint8  resel_prio:    8;
  uint8  thresh_x_high: 8;
  uint8  thresh_x_low:  8;
  int8   qrxlev_min:    8;

  uint8  P_max:         8;
  uint8  NCC_permitted: 8;
  uint16 num_freqs:    16;

} lte_ml1_sm_log_idle_gsm_meas_config_freq_group_help_s;


typedef struct
{
  lte_ml1_sm_log_idle_gsm_meas_config_freq_group_help_s info;
  lte_ml1_sm_log_idle_gsm_meas_config_freq_s freqs[LTE_CPHY_MEAS_MAX_GERAN_FREQS_IN_GROUP];
} lte_ml1_sm_log_idle_gsm_meas_config_freq_group_s;


typedef struct
{

  uint8 t_resel: 8;
  uint8 sf_med:  8;
  uint8 sf_high: 8;

  uint8 num_freq_groups: 8;
}lte_ml1_sm_log_idle_gsm_meas_config_subpkt_help_s;


typedef struct
{
  lte_ml1_sm_log_idle_gsm_meas_config_subpkt_help_s info;

  lte_ml1_sm_log_idle_gsm_meas_config_freq_group_s freq_groups[LTE_CPHY_MEAS_MAX_GERAN_FREQ_GROUPS];/*16*/
  lte_mem_instance_type inst_num;
 }lte_ml1_sm_log_idle_gsm_meas_config_subpkt_s;


/*==========================================================================*/
/* LTE ML1 DCI infomation report (Log code ID: 0xB16C)
 *---------------------------------------------------------------------------*/

#define LTE_ML1_LOG_GM_DCI_INFO_VERSION 28

/*! @brief GM DCI Info Log Record definition
*/
typedef struct
{
  /* First Section = 1 Word*/
  uint32  pdcch_timing_sfn:10;///< System frame number of PDCCH
  uint32  pdcch_timing_sub_fn:4;///< Subframe number of PDCCH
  uint32  ul_grant_num:3;///< Number of UL Grants; Range: 0 to 4
  uint32  reserved1:15;

  /* Second Section = 3 * LTE_ML1_GM_LOG_MAX_UL_CC = 12 Words */  
  struct
  {
    /* First */
    uint32  ul_grant_present:1;///< UL Grant Present
                               ///< 0x0 No
                               ///< 0x1 Yes
    uint32  ul_grant_cell_idx:3;///< Cell Index; Range is 0 to 7
    uint32 ul_grant_alloc_type:1;///< Resource Allocation Type - 0 or 1                           
    uint32  ul_grant_riv_width:4;///< Bit width of RIV/'r' based on UL frequency configuration
                                 ///< Range is 5 to 14 
    uint32  ul_grant_riv:14;///< RIV Value: Resource indicator value if type 0 allocation,
                            ///  if resource allocation type 1, represents 'r' value 
                            ///< Range is 32 to 16383
    uint32  ul_grant_hopping_flag:1;///< 0x0 disabled
                                    ///< 0x1 enabled
    uint32  ul_grant_mcs_index:5;///< MCS index; Range is 0 to 31
    uint32  ul_grant_ndi:1;///< New Data Indicator
    uint32  ul_grant_tpc:2;///< TPC command
	
    /* Second */
    uint32  ul_grant_cyclic_shift_dmrs:3;///< Cyclic shift DMRS	
    uint32  ul_grant_dci_0_k:3;///< K value for DCI 0
                               ///< Range: 4, 6, 7 
    uint32  ul_grant_ul_index_dai:3;///< UL index for TDD UL/DL 0 or
                                    ///< UL Downlink Assignment Index for TDD UL/DL 1-6. 
                                    ///< This field is displayed only when Duplex Mode is TDD.
                                    ///< Range: 1..4
    uint32  ul_grant_cqi_req:2;///< CQI request 
    uint32  ul_grant_srs_req:1;///< This field means if aperiodic srs config is 
							   ///< present in ul grant or not.
                               ///< 0- No
                               ///< 1- Yes
    uint32  ul_grant_rb_start:7;///< Starting Resource Block for the received grant :
                                ///< Range is 1 to 110
                                ///< For type 1 allocation, this is for the first cluster
    uint32  ul_grant_num_rbs:7;///< Total number of RBs allocated in the grant.
                               ///< Range is 1 to 110
                               ///< For type 1 allocation, this is for the first cluster
    uint32  ul_grant_tbs_index:5;///< Transport Block Size Index. 
                                 ///< Transport Block Size can be determined using TBS Index 
                                 ///< and Number of Resource Blocks
                                 ///< Range is 0 to 31
    uint32  reserved2_1:1;
	
	
    /* Third */
    lte_ml1_log_modulation_type_e  ul_grant_mod_type:2;///< Modulation Type                                 
    uint32  ul_grant_rb_start_2:7;///< Starting Resource Block for the received grant :
                                  ///< Range is 1 to 110
                                  ///< For type 1 allocation, this is for the second cluster
                                  ///< if type 0 allocation, this field is not filled
    uint32  ul_grant_num_rbs_2:7; ///< Total number of RBs allocated in the grant.
                                  ///< Range is 1 to 110
                                  ///< For type 1 allocation, this is for the second cluster
                                  ///< if type 0 allocation, this field is not filled
    uint32  rbg_size:3;///< if type 1 allocation, represents size of RBG for UL Bandwidth
                       ///< if type 0 allocation, this field is not filled
                       ///< Range is 1 to 4
    uint32  ul_grant_rv_index:2;///< Redundancy Version Index
                                ///< Possible Value - 0, 1, 2, 3
    uint32  harq_id:3;///< HARQ ID; Range is 0 to 7
    lte_ml1_log_rnti_type_e  rnti_type:4;
    uint32  aggregation_lvl:4;///< Aggregation Level 

    /* Fourth */
    uint32  search_space:1;///< Decoded Search Space    
    uint32  tx_ant_sel:3;///< TX antenna selection
    uint32  reserved2_3:28;
  }ul_grant_info[LTE_ML1_GM_LOG_MAX_UL_CC];
  
  /* Third Section = 1 Word */
  uint32  dl_grant_num:3; ///< Number of DL Grants; Range: 0 to 3
  uint32  reserved3:29;

  /* Fourth Section = 2 * LTE_ML1_GM_LOG_MAX_CC = 6 Words */
  struct
  {
    /* First */
    uint32  dl_grant_present:1;///< 0x0 - No
                             ///< 0x1 - Yes
    uint32  dl_grant_cc_id:3;
    lte_ml1_log_dci_format_e  dl_grant_format_type:4;///< PDCCH format type of DL grant

    uint32  dl_grant_num_ack_nak_bits:2;///< Number of ACK/NAK bits for DL grant
                                      ///< Range is 0 to 2
    uint32  dl_grant_tpc_cmd:3;///< TPC command in DL grant
    uint32  dl_grant_dai:3;///< DL Downlink Assignment Index for TDD UL/DL 1-6.
	                     ///< This filed is displayed only when Duplex Mode is 1.
                         ///< Range: 1 to 4
    uint32  dl_grant_srs_req:1;					 
    uint32  dl_grant_n_cce:10;///< start CEE
    lte_ml1_log_rnti_type_e  rnti_type:4;
    uint32  reserved4_1:1;
    /* Second */
 
    uint32  aggregation_lvl:4;///< Aggregation Level
    uint32  search_space:1;///< Decoded Search Space      					 
    uint32  reserved4_2:27;

  }dl_grant_info[LTE_ML1_GM_LOG_MAX_CC];

  /* Fifth Section = 1 Word */
  uint32  tpc_dci_present:1;///< TPC DCI Present
                            ///< 0x0 - No
                            ///< 0x1 - Yes
  uint32  tpc_dci_format_type:1;///< 0x0 - Format 3
                                ///< 0x1 - Format 3a
  lte_ml1_log_rnti_type_e  tpc_dci_rnti_type:4;
  uint32  tpc_dci_tpc_cmd:3;///< TPC command in the TPC DCI
  uint32  pdcch_order_present:1;///< 0x0 - No
                                ///< 0x1 - Yes
  uint32  reserved5:22;
}lte_ml1_gm_log_dci_info_record_s;

/*! This log packet gives important parameters about UL Grant received in 
 *  DCI Format 0. The log record is generated once every 50 subframes or 
 *  every 1 sec, whichever occurs first.
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;///< @internal

  /* First Word */
  uint32  version:8;///< Log packet version; range is 0 to 255
                    ///< Version - 24 for the log packet structure 
                    ///< described below
  uint32  reserved0:15;
  uint32  duplex_mode:1;///< Duplex mode:
                        ///< 0x0: FDD
                        ///< 0x1: TDD
  uint32  num_records:8;///< Number of records;Range is 1 to 20 
  lte_ml1_gm_log_dci_info_record_s
    dci_info_record [LTE_ML1_GM_LOG_DCI_INFO_MAX_CNT];
} lte_ml1_gm_log_dci_info_s;

/*! @brief GM Tx N_1_pucch record
*/
typedef struct
{
  uint16  n_1_pucch_i:12;
  uint16  reserved:4;
} lte_ml1_gm_log_n_1_pucch_record_s;

#ifdef FEATURE_LTE_DISCOVERY
/*! @brief GM LTED Tx Report Record definition */
typedef struct
{
  /* First word */
  uint32 sfn:10;///< PUSCH OTA system frame number
                ///< Range is 0 to 1023
  uint32 sub_fn:4;///< PUSCH OTA subframe number
                  ///< Range is 0 to 9
  uint32 retx_idx:2;///< Retransmission time for LTED
                    ///< Range is 0 to 3   
  uint32 rv:2;///< RV indexfor LTED
              ///< Range is 0 to 3
  uint32 num_rb:2;///< number of resource block
                  ///< Range is 0 to 3
  uint32 starting_rb:8;///< Start resource block
                          ///< Range is 0 to 255
  uint32 reserved1:4;

  /* Second word */
  uint32 beta_pusch:16;///< Gains for LTED channel
                       ///< Range is 0 to 4095                  
  uint32 total_tx_power:16;///< Tx power for LTED
                           ///< Range is -128 to 127
                       
  /* Third word */ 
  uint32 transp_block_size:8;///< Size of transport block
                               ///< Range is 0 to 255 
              
  uint32 eib_idx:1;///< eib index 
                   ///< Range is 0 to 1
  uint32 n_rnti:16;///< LTED RNTI
                   ///< Range is 0 to 4095
  uint32 antenna_num:2;///< Antenna number used
                       ///< Range is 0 to 3
  uint32 mod_type:2;///< Modulation type
                         ///< 0: LTE_LL1_UL_MOD_TYPE_DATA_BPSK
                         ///< 1: LTE_LL1_UL_MOD_TYPE_DATA_QPSK
                         ///< 2: LTE_LL1_UL_MOD_TYPE_DATA_16QAM
                         ///< 3: LTE_LL1_UL_MOD_TYPE_DATA_64QAM
  uint32 reserved2:3;
}
lte_ml1_gm_log_lted_tx_record_s;

/*! @brief GM LTED Rx Report Record definition */
typedef struct
{
  /* First word */
  uint32 sfn:10;///< PUSCH OTA system frame number
                ///< Range is 0 to 1023
  uint32 sub_fn:4;///< PUSCH OTA subframe number
                  ///< Range is 0 to 9
  uint32 rp_seq:4;///< Rx pool sequence
                    ///Range is 0 to 16
  uint32 num_rp:4;///< Rx pool sequence
                    ///Range is 0 to 16                    
  uint32 rp_id:8;///< Resource Pool ID 
                 ///< Range is 0 to 255   
  uint32 cp_type:1;///< Cyclic Prefix type
                   ///< 0 : Normal
                   ///< 1 : Extend
  uint32 reserved1:1;

  /* Second word */
  uint32 start_prb1:8;///< Start PRB index for the lower spectrum allocation
                      ///< Range is 0 to 255
  uint32 end_prb1:8;///< End PRB index for the lower spectrum allocation
                    ///< Range is 0 to 255
  uint32 start_prb2:8;///< Start PRB index for the higher spectrum allocation
                      ///< Range is 0 to 255   
  uint32 end_prb2:8;///< End PRB index for the higher spectrum allocation
                    ///< Range is 0 to 255  

  /* Third word */                 
  uint32 retx_idx:2;///< Retransmission time for LTED
                    ///< Range is 0 to 3   
  uint32 rv:2;///< RV indexfor LTED
              ///< Range is 0 to 3
  uint32 ndi:1;///< New Data Indicator
              ///< Range is 0 and 1
  uint32 reserved2:27;
}
lte_ml1_gm_log_lted_rx_record_s;
#endif /*FEATURE_LTE_DISCOVERY*/

/*! @brief GM PUSCH Tx Report Record definition */
typedef struct
{
  /* First word */
  uint32 chan_type:1;///< 0 - PUCCH
                     ///< 1 - PUSCH
  uint32 cell_idx:3;///< Cell Index; Range 0 to 7
  uint32 sfn:10;///< PUSCH OTA system frame number
                ///< Range is 0 to 1023
  uint32 sub_fn:4;///< PUSCH OTA subframe number
                  ///< Range is 0 to 9
  uint32 trblk_size:14;///< PUSCH transport block size in bytes

  /* Second word */
  uint32 csf_present_flag:1;///< Whether CSF is being scheduled 
  uint32 ack_nak_present_flag:2;///< Whether UL ACK/NAK is present
                                ///< 0: ACK/NAK not present
                                ///< 1: ACK/NAK present
                                ///< 2: Forced NAK present
  uint32 ca_mode_enabled:1;///< Whether at least a Scell is configured
                           ///< 0: CA mode disabled
                           ///< 1: CA mode enabled
  lte_ml1_log_ul_ack_reporting_mode_type_e ack_reporting_mode:3;///< Ack/Nak reporting mode

  uint32 ack_nak_to_send_mask:9;///< The mask to indicate which DL subframe needs to ACK or NAK.
                                ///< This array is printed only when 
                                ///< "UL ACK/NAK Present Flag" is 1 or 2, 
                                ///< and "ACK/NAK Reporting Mode" is 1-6.
  uint32 ack_nak_to_send_mask_scell_1:9;///< This field is printed only when 
                                        ///< "CA Mode Enabled" is 1
  uint32 reserved2:7;

  /* Third word */
  uint32 ack_nak_to_send_mask_scell_2:9;///< This field is printed only when 
                                        ///< "CA Mode Enabled" is 1
  uint32 dci_0_present:1;///< Whether this PUSCH is based on DCI 0
                         ///< 0: DCI 0 grant not present
                         ///< 1: DCI 0 grant present
  uint32 w_ul_dai:4;///< This field is printed only when "CA Mode Enabled" is 1
  uint32 n_bundled:4;///< The total number of subframes in current bundling. 
                     ///< This field is displayed only when 
                     ///< ACK/NAK Reporting Mode is 0x1 or 0x2.
                     ///< Range: 1 to 9
  uint32 end_of_bundling:4;///< The very last subframe number of current bundling. 
                           ///< This field is displayed only 
                           ///< when ACK/NAK Reporting Mode is 0x1 or 0x2.
                           ///< Range: 0 to 9
  uint32 ack_nak_len:3;///< ACK/NAK length 
                       ///< This field is printed only when it is greater than 0
  uint32 reserved3:7;

  /* Fourth word */
  uint32 beta_pusch:16;///< Range is 0 to 65535
  uint32 total_tx_power:8;///< Range is -112 dBm to 23 dBm, with 1 dB stepsize
  uint32 cyclic_shift_dmrs:3;///< Cyclic Shift DMRS
  uint32 reserved4:5;

  /* Fifth word */
  uint32 rb_start:7;///< Starting resource block number (1-110)
  uint32 rv:2;///< Redundancy Version
  lte_ml1_log_modulation_type_e mod_type:2;
  uint32 num_rbs:7;///< Number of contiguous resource blocks (1-110)
  uint32 harq_id:3;///< Harq index
  uint32 retx_index:5;///< Retransmission index
  uint32 hopping_flag:1;///< Freq Hopping Flag
                        ///< 0x0 - Disabled
                        ///< 0x1 - Enabled
  uint32 harq_ack_offset:4;///< I_harq_ack_offset
  uint32 reserved5:1;
  
  /* Sixth word */  
  uint32 cqi_offset:4;///< I_cqi_offset
  uint32 ri_offset:4;///< I_ri_offset
  uint32 hopping_payload:2;///< Used to determine VRB to PRB mapping
                           ///< Range is 0 to 3
  uint32 srs_present_flag:1;///< Whether SRS is being scheduled
  uint32 srs_ue_or_cell_specific:1;///< Cell or UE specific SRS occasion
  uint32 n_dmrs:3;///< N_dmrs
  uint32 antenna_num:2;///< Number of antenna
  uint32 tti_bundl_index:3;///< TTI Bundle Index
                           ///< Range: 0 to 3
                           ///< 0x7 - Invalid 
  uint32 eib_index:1;///< EIB Index; Range: 0 to 1
  uint32 mcs_index:5;///< The index for mcs
  uint32 padding:6;///< To pad 6 bits to align PUCCH and PUSCH tx records. Not displayed. 

  /* Sevent word */
  uint32 alloc_type:1;///< Resource allocation type - 0 or 1
  uint32 rb_start_2:7;///< Starting resource block number for second cluster(1-110)
  uint32 num_rbs_2:7;///< Number of contiguous resource blocks for second cluster (1-110)
  uint32 reserved6:17;

  /* Eighth word */
  int32 afc_rx_freq_error;///< AFC Rx Freq Error [Hz] 
}lte_ml1_gm_log_pusch_tx_report_record_s;

/*! @brief GM PUCCH Tx Report Record definition */
typedef struct
{
  /* First word */
  uint32 chan_type:1;///< 0 - PUCCH
                     ///< 1 - PUSCH
  uint32 sfn:10;///< PUCCH OTA system frame number
                ///< Range is 0 to 1023
  uint32 sub_fn:4;///< PUCCH OTA subframe number
                  ///< Range is 0 to 9
  uint32 total_tx_power:8;///< Range is -112 dBm to 23 dBm, with 1 dB stepsize
  uint32 ack_nak_present_flag:2;///<0: ACK/NAK not present
                                ///<1: ACK/NAK present
                                ///<2: Forced NAK present
  uint32 ca_mode_enabled:1;///< Whether at least a Scell is configured
                           ///< 0: CA mode disabled
                           ///< 1: CA mode enabled
  lte_ml1_log_ul_ack_reporting_mode_type_e ack_reporting_mode:3;///< Ack/Nak reporting mode

  uint32 ack_nak_len:2;///< ACK/NAK length
  uint32 reserved0:1;

  /* Second word */
  uint32 ack_nak_to_send_mask:9;
  uint32 ack_nak_to_send_mask_scell_1:9;///< This field is printed only when 
                                        ///< "CA Mode Enabled" is 1
  uint32 ack_nak_to_send_mask_scell_2:9;///< This field is printed only when 
                                        ///< "CA Mode Enabled" is 1
  uint32 reserved1:5;

	/* Third word */
  uint32 beta_pucch:16;///< Range is 0 to 65535
  uint32 reserved2:1;
  uint32 n_1_pucch:12;///< PUCCH resource for format 1/1a/1b
                      ///< Range is 0 to 2083
  uint32 n_1_pucch_num:3; ///< Number of n_1_pucch sent t in TDD multiplexing mode 
                          ///< or Carrier Aggregation mode. This field is displayed
						  ///< only when its value is non-zero.
                          ///< Range is 0 to 4
  /* Fourth and Fifth words */
  lte_ml1_gm_log_n_1_pucch_record_s
    n_1_pucch_records[LTE_ML1_LOG_GM_PUCCH_TX_N_1_PUCCH_NUM];///< n_1_pucch_array[4]
	                                                         ///< This field is displayed only when "Number of 
															 ///< n_1_pucch" is non-zero.
															 ///< The number of n_1_pucch_array [i] displayed is 
															 ///< "Number of n_1_pucch". Each one is 2 byte.

  /* Sixth word */
  uint32 n_2_pucch:10;///< PUCCH resource for format 2/2a/2b
  uint32 n_3_pucch:10;///< PUCCH resource for format 3
  uint32 sr_present_flag:1;///< Scheduling Request Present
                           ///< 0x0 - Not present
                           ///< 0x1 - Present
  uint32 trni_ack_flag:1;///< Temp RNTI ACK Flag
                         ///< 0x0 - Not present
                         ///< 0x1 - Present
  uint32 srs_present_flag:1;///< Whether SRS is being scheduled
  uint32 srs_ue_or_cell_specific:1;///< Cell or UE specific SRS occasion
  uint32 csf_present_flag:1;///< 0x0 - Not present
                            ///< 0x1 - Present
  uint32 padding:7;///< Reserved

  /* Seventh word */
  int32 afc_rx_freq_error;///< AFC Rx Freq Error [Hz] 
}lte_ml1_gm_log_pucch_tx_report_record_s;

/*! @brief GM Tx Report Log packet union */
typedef union
{
  lte_ml1_gm_log_pucch_tx_report_record_s pucch_tx_report;
  lte_ml1_gm_log_pusch_tx_report_record_s pusch_tx_report;
} lte_ml1_gm_log_tx_report_u;

/*! @brief GM PDCCH PHICH Info Log Record definition
*/
typedef struct
{
  uint32  num_pdcch_results:3;///< Range is 0 to 7
  uint32  num_phich_results:3;///< Range is 0 to 7
  uint32  pdcch_timing_sfn:10;///< System frame number of PDCCH
  uint32  pdcch_timing_sub_fn:4;///< Sub frame number of PDCCH
  uint32  reserved0:12;///< Reserved 

  struct 
  {
    uint32  carr_id:3;///< Cell Index; Range 0 to 7
    uint32  phich_present:1;///< PHICH present
    uint32  phich_1_present:1;///< PHICH1 present
    uint32  phich_val:1;///< PHICH value
                        ///< 0x0 - NAK
                        ///< 0x1 - ACK

    uint32  phich_1_val:1;///< PHICH1 value
                        ///< 0x0 - NAK
                        ///< 0x1 - ACK
    uint32  reserved1:25;///< Reserved
  }phich_info[LTE_ML1_GM_LOG_MAX_CC];

  struct
  {
    uint32 carr_id:3;///< Cell Index; Range 0 to 7
    uint32 rnti_type:4;///< RNTI type; Range 0 to 9
    uint32 payload_size:7;///< Payload size
    uint32 aggregation_lvl:2;///< Aggregation Level; Range 0 to 3
    uint32 srch_space:1;///< Search Space
                        ///< 0x0 - Common
                        ///< 0x1 - UE-specific

    uint32 sps_grant_type:3;///< SPS grant type; Range 0 to 4
    uint32 new_dl_tx:1;///< Grant for New DL
                       ///< 0x0 - DL grant not present
                       ///< 0x1 - DL grant present
    uint32 num_dl_trblks:2;///< Number of transport blocks
    uint32 reserved2:9;///< Reserved

    uint32 s_index_0:5; ///< s0 index for resource allocation type 1
                        ///< Range - 0 to 25
    uint32 s_index_1:5;///< s1 index for resource allocation type 1
                        ///< Range - 0 to 25
    uint32 s_index_2:5;///< s2 index for resource allocation type 1
                        ///< Range - 0 to 25
    uint32 s_index_3:5;///< s3 index for resource allocation type 1
                        ///< Range - 0 to 25
    uint32 reserved3:12;///< Reserved
  }pdcch_info[LTE_ML1_GM_LOG_MAX_PDCCH_RESULTS_PER_TTI];
}lte_ml1_gm_log_pdcch_phich_info_record_s;

/*! @brief GM Tx Report Log packet struct */
typedef struct
{
  lte_ml1_gm_log_tx_report_u tx_report;
} lte_ml1_gm_log_tx_report_record_s;

/*! @brief ULM Common config log struct
*/
typedef struct
{
  /* Version */
  uint32 version:8;
  uint32 reserved:24;                                  //32 bits

  /* PUSCH Config */
  uint32 pusch_cfg_present:1;
  uint32 pusch_n_sb:2;
  uint32 pusch_hopping_mode:1;
  uint32 pusch_hopping_offset:7;
  uint32 pusch_enable_64_qam:1;
  uint32 pusch_group_hopping_enabled:1;
  uint32 pusch_group_assignment_pusch:5;
  uint32 pusch_seq_hopping_enabled:1;
  uint32 pusch_cyclic_shift:3;

  /* PUCCH Config */
  uint32 pucch_cfg_present:1;
  uint32 pucch_delta_shift:2;
  uint32 pucch_n_rb_cqi:7;                             //32 bits
  uint32 pucch_n_cs_an:3;
  uint32 pucch_n1_pucch_an:11;

  /* SRS Config */
  uint32 srs_cfg_present:1;
  uint32 srs_enable:1;
  uint32 srs_bw_cfg:3;
  uint32 srs_subframe_config:4;
  uint32 srs_sim_ack_nak_srs:1;
  uint32 srs_max_up_pts:1;
  uint32 reserved1:7;                                  //32 bits

} lte_ml1_ulm_log_common_cfg_s;

/*! @brief ULM Dedicated config log struct
*/
typedef struct
{
  /* Version */
  uint32 version:8;
  uint32 std_ver:8;
  uint32 reserved:16;                                   //32 bits

  /* SRS Config */
  uint32 srs_cfg_present:1;
  uint32 srs_enable:1;
  uint32 srs_bw:2;
  uint32 srs_hopping_bw:2;
  uint32 srs_freq_domain_position:5;
  uint32 srs_duration:1;
  uint32 srs_cfg_index:10;
  uint32 srs_tx_comb:1;
  uint32 srs_cyclic_shift:3;

  /* CQI Config */
  uint32 cqi_cfg_present:1;
  uint32 cqi_enable:1;
  uint32 cqi_reporting_aperiodic_present:1;
  uint32 cqi_reporting_mode_aperiodic:3;                  // 32 bits
  uint32 cqi_reporting_periodic_present:1;
  uint32 cqi_format_indicator:1;
  uint32 cqi_nom_pdsch_rs_epre_offset:4;
  uint32 cqi_pucch_resource_index:11;
  uint32 cqi_ri_cfg_enabled:1;
  uint32 cqi_ri_cfg_index:10;
  uint32 cqi_subband_cqi_k:3;
  uint32 cqi_sim_ack_nak_cqi:1;                           //32 bits
  uint32 cqi_pmi_cfg_index:10;
  uint32 cqi_mask:1;

  /* Antenna Config */
  uint32 antenna_cfg_present:1;
  uint32 antenna_transmission_mode:3;
  uint32 antenna_tx_antenna_sel_enable:1;
  uint32 antenna_tx_antenna_sel_ctrl:1;
  uint32 reserved1:15;                                    //32 bits
  uint32 antenna_codebook_subset_rest[2];                 //64 bits

} lte_ml1_ulm_log_dedicated_cfg_s;

/* CA metrics log packet event types */
typedef enum
{
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_CONFIG = 0,
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_DECONFIG,
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_ACTIVATE,
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_DEACTIVATE,
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_VRLF_ACTIVATE,
  LTE_ML1_CA_METRIC_LOG_EVENT_SCC_VRLF_DEACTIVATE,
  LTE_ML1_CA_METRIC_LOG_EVENT_CONNECTION_RELEASE,
  LTE_ML1_CA_METRIC_LOG_EVENT_HANDOVER,
  LTE_ML1_CA_METRIC_LOG_EVENT_MAX
} lte_ml1_dlm_ca_log_metrics_events_e;

/* CA metrics log packet SCell state types */
typedef enum
{
  LTE_ML1_CA_METRIC_LOG_SCELL_STATE_DEACTIVATED = 0,
  LTE_ML1_CA_METRIC_LOG_SCELL_STATE_ACTIVATED,
  LTE_ML1_CA_METRIC_LOG_SCELL_STATE_MAX
} lte_ml1_dlm_ca_log_metrics_scell_state_e;

/* CA metrics log packet SCell state types */
typedef enum
{
  LTE_ML1_CA_METRIC_LOG_SCELL_UPLINK_STATE_NOT_CONFIGURED = 0,
  LTE_ML1_CA_METRIC_LOG_SCELL_UPLINK_STATE_CONFIGURED
} lte_ml1_dlm_ca_log_metrics_scell_uplink_state_e;

/* CA metrics cell metrics information */
typedef struct
{
  uint32 dl_num_sf_scheduled;  /* Number of SF the cell was scheduled */
  uint32 ul_num_sf_scheduled;
  uint64 dl_tb_size;           /* Total TB size count for cell */
  uint64 ul_tb_size;
} lte_ml1_dlm_ca_log_cell_metrics_s;

/* CA metrics information */
typedef struct
{
  lte_ml1_dlm_ca_log_cell_metrics_s pcell_only;          /* PCell only metrics */
  lte_ml1_dlm_ca_log_cell_metrics_s scell_any;           /* Any SCell metrics */
  lte_ml1_dlm_ca_log_cell_metrics_s pcell_and_scell_any; /* PCell and any SCell metrics */

  lte_ml1_dlm_ca_log_cell_metrics_s pcell;               /* PCell metrics */
  lte_ml1_dlm_ca_log_cell_metrics_s scells[LTE_ML1_DLM_CA_LOG_MAX_SCELLS]; /* SCell metrics */

  uint64 elapsed_time;      /* Time elapsed in ms */
} lte_ml1_dlm_ca_log_ca_metrics_s;

/* CA metrics log packet */
typedef struct
{
  uint8  version;    /* Version of the log packet */

  uint8  event;      /* Event that triggered the log packet */
  uint8  num_scells; /* Number of SCells */
  uint8  subfn;      /* Subframe number of the event trigger */
  uint16 sfn;        /* System frame number of the event trigger */
  uint16 reserved;

  /* PCell information */
  struct
  {
    uint32 earfcn;  /* E-ARFCN of PCell */
    uint16 pci;     /* Physical cell ID of PCell */
    uint8  band;    /* Band of PCell */
    uint8  dl_bw;   /* Bandwidth of Cell */
    uint8  reserved;
  } pcell;

  /* SCell information */
  struct
  {
    uint32 earfcn;  /* E-ARFCN of PCell */
    uint16 pci;     /* Physical cell ID of PCell */
    uint8  band;    /* Band of PCell */
    uint8  dl_bw;   /* Bandwidht for the Scell */
    uint8  dl_state;   /* State of the SCell */
    uint8  ul_state;
  } scells[LTE_ML1_DLM_CA_LOG_MAX_SCELLS];

  lte_ml1_dlm_ca_log_ca_metrics_s total_metrics;         /* Total connection metrics */
  lte_ml1_dlm_ca_log_ca_metrics_s ca_configured_metrics; /* CA configured metrics */
  lte_ml1_dlm_ca_log_ca_metrics_s ca_activated_metrics;  /* CA activated metrics */

} lte_ml1_dlm_ca_log_metrics_s;

/*! @brief DLM common cfg log structure
    size = 4 + 4 + 1 + [1+1] + 1 + [(4*(0 to 8))] = 44bytes (worst case)
	Packet ID: LOG_LTE_ML1_DLM_COMN_CFG_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;                                      //32bit word1 start

  /*!***************** TDD cfg *****************/
  /*! tdd cfg: cfg valid if present */
  uint32  tdd_comn_cfg_present:1;
  uint32  tdd_cfg_subfn_assign:4;
  uint32  tdd_cfg_special_subfn_patterns:4;

  /*!***************** UL CP cfg *****************/
  /*! UL CP cfg: cfg valid if present */
  uint32  ul_cp_comn_cfg_present:1;
  uint32  ul_cp_cfg_cp:1;
  uint32  reserved0:13;                                   //32bit word1 end

  /*!***************** UL freq cfg *****************/
  /*! UL freq cfg: cfg valid if present */
  uint32  ul_freq_comn_cfg_present:1;                     //32bit word2 start
  uint32  ul_freq_present:1;
  uint32  ul_freq:16;
  uint32  ul_bw:7;
  uint32  spec_emisson_present:1;
  uint32  spec_emission:6;                                //32bit word2 end

  /*!***************** MIB info *****************/
  /*! MIB info: cfg valid if present */                  //32bit word3 start
  uint8 mib_info_present:1;
  uint8 num_enodeb_tx_ant:3;
  uint8 phich_duration:1;
  uint8 phich_resource:2;

  /*!***************** PDSCH common cfg *****************/
  /*! PDSCH common cfg: cfg valid if present */
  uint8 pdsch_comn_cfg_present:1;
  int8  ref_signal_pwr;
  uint8 pb;

  /*!***************** MBSFN cfg *****************/
  /*! MBSFN cfg: cfg valid if present */
  uint8                 mbsfn_comn_cfg_present:1;
  uint8                 num_mbms_allocations:4;
  uint8                 reserved1:3;                     //32bit word3 end

  /*! MBSFN cfg: VARIABLE SIZE ALLOCATION based on num_mbms allocations */
  struct
  {
    uint32  allocation_offset:3;                         //32bit word4 start
    uint32  allocation_period:3;
    uint32  allocation_type:1;
    uint32  reserved2:1;
    uint32  allocation_mask:24;                          //32bit word4 end
  }mbsfn_cfg[LTE_ML1_LOG_MAX_MBSFN_ALLOCATIONS];

} lte_ml1_log_dlm_comn_cfg_s;

/*! @brief DLM dedicated cfg log structure = 4 + 4 + 4 + 4 + 1 + 1 + 2 = 20bytes
    Packet ID: LOG_LTE_ML1_DLM_DED_CFG_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  uint32  version:8;                                        //32bit word1 start

  /*!***************** PUCCH TPC cfg *****************/
  /*! PUCCH TPC cfg: cfg valid if present */
  uint32  pucch_tpc_ded_cfg_present:1;
  uint32  pucch_enabled:1;
  uint32  pucch_tpc_rnti_val:16;
  uint32  pucch_dci_format:1;
  uint32  pucch_tpc_index:5;                                //32bit word1 end

  /*!***************** PUSCH TPC cfg *****************/
  /*! PUSCH TPC cfg: cfg valid if present */
  uint32  pusch_tpc_ded_cfg_present:1;                      //32bit word2 start
  uint32  pusch_enabled:1;
  uint32  pusch_tpc_rnti_val:16;
  uint32  pusch_dci_format:1;
  uint32  pusch_tpc_index:5;
  uint32  reserved0:8;                                      //32bit word2 end

  /*!***************** Antenna dedicated cfg *****************/
  uint32 codebook_subset_rest_lsw;                          //32bit word3
  uint32 codebook_subset_rest_msw;                          //32bit word4

  /*! cfg valid if present */
  uint8   antenna_ded_cfg_present:1;                        //32bit word5 start
  uint8   transmission_mode:4;
  uint8   tx_antenna_sel_enable:1;
  uint8   tx_antenna_sel_ctrl:1;
  uint8   reserved1:1;

  /*!***************** PDSCH dedicated cfg *****************/
  /*! PDSCH dedicated cfg: cfg valid if present */
  uint8 pdsch_ded_cfg_present:1;
  uint8 pa:4;
  uint8 reserved2:2;

  /*!***************** UE related cfg *****************/
  /*! UE related cfg: cfg valid if present */
  uint8 ue_related_ded_cfg_present:1;
  uint16 c_rnti;                                            //32bit word5 end

} lte_ml1_log_dlm_ded_cfg_s;


/*! @brief DLM pdsch stat record log structure  = 2 + 2 + 2 + 3* [6] + 2 = 26 bytes
*/
typedef struct
{
  /*! ********** PDSCH stat per record **************/

  /* OTA Tx time stamp */
  uint16    subframe_num:4;                   ///< 0 ~9     //32bit word1 start
  uint16    frame_num:12;                     ///< radio frame number 0-1023

  uint16 num_rbs:8;                           ///< number or rbs allocated
  uint16 num_layers:8;                        ///< number of layers
                                              //32bit word1 end

  /*! Num of transport blocks present or valid */
  uint16    num_trbs_present:8;
  uint16    serving_cell_index:3;
  /*! HSIC enabled/disabled flag */
  uint16    hsic_enabled:1;
  uint16    reserved0:4;

  /*! FIXED SIZE ALLOCATION based on "LTE_LL1_MAX_PDSCH_TRANMISSIONS_PER_TTI" */
  struct                                                //32bit word 2 - 5.5  start
  {
    uint8     harq_id:4;                       ///< (up to 8 for FDD, up to 15 for TDD)
    uint8     rv:2;                            ///< (0, 1, 2, 3)
    uint8     ndi:1;                           ///< (0 or 1)
    uint8     crc_result:1;

    uint8     rnti_type:4;
    uint8     tb_index:1;                      ///< (0 or  1 (MIMO))
    uint8     pdsch_result_discard_retx:1;
    uint8     did_recomb:1;
    uint8     ack_nack:1;                      ///0->nack, 1->ack

    uint16    trblk_size;                     /// Size in bytes

    uint16    mcs:8;                          ///< ( MCS index 0 - 31)
    uint16    num_rbs:8;                      ///< number of rbs allocated to this tb

    #ifdef FEATURE_XMOD
    uint8     mod_type:8;                     ///< modulation type (QPSK, 64QAM, etc.)
    uint8     reserved3;
    #endif /* FEATURE_XMOD */ 
  }tb[2];

  uint16      pmch_id:8;     ///< PMCH id: only applicable to pmch decodes
  uint16      area_id:8;     ///< Area id used: only applicable to pmch decodes  //32bit word 2 - 6  end
} lte_ml1_log_dlm_pdsch_stat_record_s;

/*! @brief DLM pdsch stat log records structure = 4 + (1 to 25)*26 = 654 bytes (worst case)
    Packet ID: LOG_LTE_ML1_DLM_PDSCH_STAT_IND_LOG_C
*/
#ifdef FEATURE_XMOD
#define LTE_ML1_LOG_DLM_PDSCH_STAT_RECORD_VERSION 25
#else
#define LTE_ML1_LOG_DLM_PDSCH_STAT_RECORD_VERSION 5
#endif /* FEATURE_XMOD */

typedef struct
{
  /*!***************** Version *****************/
  uint32     version:8;                                     //32bit word1 start

  /*!***************** Number of PDSCH records available *****************/
  uint32     num_records:8;
  uint32     reserved0:16;                                  //32bit word1 end

  /*!***************** PDSCH records *****************/
  /*! VARIABLE SIZE ALLOACTION based on num of records field */
  lte_ml1_log_dlm_pdsch_stat_record_s    record[LTE_ML1_DLM_LOG_PDSCH_STAT_MAX_CNT];
} lte_ml1_log_dlm_pdsch_stat_records_s;

/*! @brief DLM BLER stats log records structure = 4 + (1 to 2)*12 = 28 bytes (worst case) 
    Packet ID: LOG_LTE_ML1_DLM_BLER_STAT_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  uint32     version:8;                                     //32bit word1 start

  /*!***************** Number of carriers available *****************/
  uint32     num_carriers:8;
  uint32     reserved0:16;                                  //32bit word1 end

  /*!***************** BLER stats *****************/
  /*! VARIABLE SIZE ALLOACTION based on num of carriers field */
  struct
  {
    uint32     total_trblk_rcvd;
    uint32     total_trblk_crc_pass;
    uint32     dl_throughput;
  }carrier[LTE_ML1_DLM_LOG_BLER_STATS_CARRIER_MAX_CNT];
} lte_ml1_log_dlm_bler_stats_s;

typedef enum
{
  ZERO = 0,
  LOW  = 1,
  HIGH = 2,
  INVALID = 0xFF
} confidence_type_e;

/*! @brief DLM throughput estimation Link capacity record log structure  = 
    2 + 4 + 2 + (1 to 3)*4 = 20 bytes
*/
typedef struct
{
  uint16     frame_num:12;                     ///< radio frame number 0-1023
  uint16     subframe_num:4;                   ///< Subframe number 0 ~9

  /*! @brief Instantaneous link capacity in units of Mbps (x16), 
      range >= 0, 0x0000 = 0 Mbps, 0x0001 = 0.0625 Mbps, ...
  */
  uint16     link_cap;
  
  uint32     t_accumulate_expired:8;           ///< Flag indicates if T_accumulate expired or not
  confidence_type_e     confidence:8;          ///< Confidence of link capacity
  uint32     num_carriers:8;                   ///< Number of carriers
  uint32     reserved0:8;                      ///< Reserved

  /*! @brief For carrier[i] where 0 <= i < num_carriers. Size each = 4 bytes */
  struct
  {
    uint16   cqi;                             ///< Wideband CQI
    uint16   efficiency;                      ///< Efficiency from CQI in Mbps
  }carrier[LTE_ML1_DLM_LOG_MAX_CARRIERS];

} lte_ml1_log_dlm_link_cap_record_s;

/*! @brief DLM Throughput estimation link capacity stats log records structure = 
    4 + 20*10 = 204 bytes (worst case) 
    Packet ID: LOG_LTE_ML1_DLM_LINK_CAP_STAT_LOG_C
*/
typedef struct
{
  uint32     version:8;                       ///< Log packet version 0-255
  uint32     num_records:8;                   ///< Number of records max 10
  uint32     reserved0:16;                    ///< Reserved

  /*! @brief For record[i] where 0 <= i < num_records */
  lte_ml1_log_dlm_link_cap_record_s    record[LTE_ML1_DLM_LOG_LINK_CAP_MAX_CNT];
} lte_ml1_log_dlm_link_cap_records_s;

/*! @brief DLM throughput estimation resource availability fraction record log structure  = 
    2 + 2 + 4 + 4 + 2 = 14 bytes
*/
typedef struct
{
  uint16     frame_num:12;                     ///< radio frame number 0-1023
  uint16     subframe_num:4;                   ///< Subframe number 0 ~9

  uint16     t_accumulate_expired:8;           ///< Flag indicates if T_accumulate expired or not
  uint16     dl_frac:8;                        ///< DL fraction 0 <= Range <= 1 (x64), 0x00 = 0, 0x01 = 0.015625, ..., 0x40 = 1  

  /*! @brief Instantaneous resource availability fraction , 0 <= range <= 1 (x64) 
      0x00 = 0, 0x01 = 0.015625, ..., 0x40 = 1 
  */
  uint32 res_avail_frac:8;
  /*! @brief RSRP value for rx antenna 0 in units of dB (x16), 
      range -140 to -40 dB, 0x00 = -140 dB, 0x1 = -139.9375 dB, ...
  */
  uint32 rsrp_rx_0:12;
  /*! @brief RSRP value for rx antenna 1 in units of dB (x16), 
      range -140 to -40 dB, 0x00 = -140 dB, 0x1 = -139.9375 dB, ...
  */
  uint32 rsrp_rx_1:12;

  /*! @brief RSSI value for rx antenna 0 in units of dB (x16), 
      range -110 to -10 dB, 0x00 = -110 dB, 0x1 = 109.9375 dB, ... , 0x640 = -10 dB
  */
  uint32 rssi_rx_0:12;
  /*! @brief RSSI value for rx antenna 0 in units of dB (x16), 
      range -110 to -10 dB, 0x00 = -110 dB, 0x1 = 109.9375 dB, ... , 0x640 = -10 dB
  */
  uint32 rssi_rx_1:12;
  uint32 reserved1:8;                          ///< Reserved

} lte_ml1_log_dlm_res_avail_frac_record_s;

/*! @brief DLM Throughput estimation resource availability fraction stats log records structure = 
    4 + 14*20 = 284 bytes (worst case) 
    Packet ID: LOG_LTE_ML1_DLM_RES_AVAIL_FRAC_STAT_LOG_C
*/
typedef struct
{
  uint32     version:8;                       ///< Log packet version 0-255
  uint32     num_records:8;                   ///< Number of records, max 20
  uint32     reserved0:16;                    ///< Reserved

  /*! @brief For record[i] where 0 <= i < num_records */
  lte_ml1_log_dlm_res_avail_frac_record_s    record[LTE_ML1_DLM_LOG_RES_AVAIL_FRAC_MAX_CNT];
} lte_ml1_log_dlm_res_avail_frac_records_s;


/*! @brief DLM Rx id enum */
typedef enum
{
  LTE_ML1_LOG_DLM_RX0,
  LTE_ML1_LOG_DLM_RX1,
  LTE_ML1_LOG_DLM_RX2,
  LTE_ML1_LOG_DLM_RX3,
  LTE_ML1_LOG_DLM_RX_MAX_NUM
} lte_ml1_log_dlm_rx_idx_e;

/*! @brief DLM HO Rxd Status log structure
    size = 4 + 4 + 4 = 12 bytes
    Packet ID: LOG_LTE_ML1_DLM_HO_RXD_STATUS_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;                                      

  uint8   reserved0;             

  uint16  frame_num:12;                                 ///< radio frame number 0-1023   
  uint16  subframe_num:4;                               ///< Subframe number 0 ~9  

  uint8   ho_rxd_enabled;                               
  lte_ml1_log_dlm_rx_idx_e ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT];
  uint8   num_antennas;
  uint16  reserved1;

} lte_ml1_log_dlm_ho_rxd_status_s;


/*! @brief DLM ARD Decision enum */
typedef enum
{
  LTE_ML1_LOG_DLM_ARD_DEFAULT,
  LTE_ML1_LOG_DLM_ARD_ON,
  LTE_ML1_LOG_DLM_ARD_OFF_SCHED_RATE,
  LTE_ML1_LOG_DLM_ARD_OFF_CORRELATION,
  LTE_ML1_LOG_DLM_ARD_OFF_SPEFF,
  LTE_ML1_LOG_DLM_ARD_OFF_CINR,
  LTE_ML1_LOG_DLM_ARD_MAX_NUM,

} lte_ml1_log_dlm_ard_decision_reason_e;

/*! @brief DLM HO Rxd ARD Statistics log record structure
    size = 4 bytes
*/
typedef struct {
    uint16  frame_num:12;                                 ///< radio frame number 0-1023   
    uint16  subframe_num:4;                               ///< Subframe number 0 ~9  
    lte_ml1_log_dlm_ard_decision_reason_e ard_decision;   
    uint8  sched_rate;                                   
} lte_ml1_log_dlm_ho_rxd_ard_stats_record_s;

/*! @brief DLM HO Rxd ARD Statistics log structure
    size = 4 + 10*4 = 44 bytes
    Packet ID: LOG_LTE_ML1_DLM_HO_RXD_ARD_STATS_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;                                      //32bit word1 start

  uint8  num_records;                                         ///< Number of records max 10
  uint16 reserved0;                                       //32bit word1 end

  lte_ml1_log_dlm_ho_rxd_ard_stats_record_s record[LTE_ML1_LOG_DLM_HO_RXD_ARD_MAX_CNT];

} lte_ml1_log_dlm_ho_rxd_ard_stats_s;

/*! @brief DLM Measurements log structure
    size = 4 + 4*2 + 6*4 + 6*4 = 60 bytes
    Packet ID: LOG_LTE_ML1_DLM_MEAS_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;                                      //32bit word1 start

  uint32  reserved0:8;

  uint16  frame_num:12;                                 ///< radio frame number 0-1023   
  uint16  subframe_num:4;                               ///< Subframe number 0 ~9  

  /*! @brief Final IIR filtered cinr per rx
   */
  struct {
    uint16 cinr_val:12;
    uint8  reserved1:4;
  } cinr_rx[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT];

  /*! @brief Final IIR filtered signal coorelation per rx
   */
  uint32 rx_signal_corr[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT_CORRELATION];

  /*! @brief Final IIR filtered inerference coorelation per rx
   */
  uint32 rx_interference_corr[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT_CORRELATION];

} lte_ml1_log_dlm_meas_s;

/*! @brief DLM HO Rxd SPEF Parameters log structure
    size = 4 + 4 + 4 + 4 = 16 bytes
    Packet ID: LOG_LTE_ML1_DLM_HO_RXD_SPEF_PARAMS_LOG_C
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;                                    //32bit word1 start

  uint8  reserved0;             

  uint16  frame_num:12;                                 ///< radio frame number 0-1023   
  uint16  subframe_num:4;                               ///< Subframe number 0 ~9  

  /*!***************** SPEF cfg *****************/
  uint16 wb_spef_rank1_horxd;                           //32bit word2 start
  uint16 wb_spef_rank2_horxd;                           //32bit word2 end
  uint16 rank_index_horxd;                              //32bit word3 start
  uint16 wb_spef_rank1_vpe1;                            //32bit word3 end
  uint16 wb_spef_rank2_vpe1;                            //32bit word4 start
  uint16 rank_index_vpe1;                               //32bit word4 end

} lte_ml1_log_dlm_ho_rxd_spef_params_s;


/*! @brief DLM ARD transition reasons */
typedef enum
{
  ARD_LOG_TXN_REASON_START =             0x1,
  ARD_LOG_TXN_REASON_STOP =              0x2,
  ARD_LOG_TXN_REASON_RANK_FB =           0x4,
  ARD_LOG_TXN_REASON_EVAL_RESULT =       0x8,
  ARD_LOG_TXN_REASON_WAKEUP =            0x10,
  ARD_LOG_TXN_REASON_TRAFFIC =           0x20,
  ARD_LOG_TXN_REASON_1RX_ALLOW =         0x40,
  ARD_LOG_TXN_REASON_1RX_DISALLOW =      0x80,
  ARD_LOG_TXN_REASON_SUSPEND =           0x100,
  ARD_LOG_TXN_REASON_RESUME =            0x200,
  ARD_LOG_TXN_REASON_RXCAP_DOWNGRADE =   0x400,
  ARD_LOG_TXN_REASON_RXCAP_UPGRADE =     0x800,
  ARD_LOG_TXN_REASON_SRX =               0x1000,
  ARD_LOG_TXN_REASON_FLOW_CONTROL =      0x2000,
  ARD_LOG_TXN_REASON_RACH =              0x4000,
  ARD_LOG_TXN_REASON_FORCED_MAX_RX =     0x8000,
  ARD_LOG_TXN_REASON_PRS_START =         0x10000,
  ARD_LOG_TXN_REASON_VOLTE =             0x20000,
  ARD_LOG_TXN_REASON_EFS_FORCE_4RX =     0x40000,
  ARD_LOG_TXN_REASON_EFS_FORCE_2RX =     0x80000,
  ARD_LOG_TXN_REASON_RLM_SNR       =     0x100000,
  ARD_LOG_TXN_REASON_ASDIV_BUMP       =  0x200000,
  
  ARD_LOG_TXN_REASON_EFS_FORCE_1RX    =  0x800000,

  /* always keep this in the end, have a relatively high value so bitfield 
     need not be changed frequently */
  ARD_LOG_TXN_REASON_DEFAULT =            0x2000000,
} ard_log_txn_reason_e;

/*! @brief DLM ARD transition failure reasons */
typedef enum
{
  ARD_LOG_FAIL_REASON_NA                   =     0x1,
  ARD_LOG_FAIL_REASON_ARD_INTERNAL         =     0x2,
  ARD_LOG_FAIL_REASON_CONFLICT             =     0x4,
  ARD_LOG_FAIL_REASON_CONFLICT_PBCH        =     0x8,
  ARD_LOG_FAIL_REASON_CONFLICT_DOPPLER     =     0x10,
  ARD_LOG_FAIL_REASON_CONFLICT_ASDIV       =     0x20,
  ARD_LOG_FAIL_REASON_CONFLICT_PRS         =     0x40,
  ARD_LOG_FAIL_REASON_CONFLICT_DEACT_SRCH  =     0x80,
  ARD_LOG_FAIL_REASON_CONFLICT_RACH        =     0x100,
  ARD_LOG_FAIL_REASON_MISS_ACT_TIME        =     0x200,
  ARD_LOG_FAIL_REASON_MISS_MEAS_PREP       =     0x400,
  ARD_LOG_FAIL_REASON_SCC_NOT_ACTIVATED    =     0x800,
  ARD_LOG_FAIL_REASON_SCC_RX23_NOT_READY   =     0x1000,
  ARD_LOG_FAIL_REASON_SCC_NOT_FOUND        =     0x2000,
  ARD_LOG_FAIL_REASON_ULCA_TUNE            =     0x4000,
}ard_log_fail_reason_e;

/*! @brief DLM ARD conflict reasons */
typedef enum
{
  ARD_LOG_CONFLICT_NONE                 =     0x1,
  ARD_LOG_CONFLICT_BEST2_DISALLOW       =     0x2,
  ARD_LOG_CONFLICT_ASDIV                =     0x4,
  ARD_LOG_CONFLICT_PBCH                 =     0x8,
  ARD_LOG_CONFLICT_PRS                  =     0x10,
  ARD_LOG_CONFLICT_RACH                 =     0x20,
  ARD_LOG_CONFLICT_DOPPLER              =     0x40,
  ARD_LOG_CONFLICT_SRX                  =     0x80,
  ARD_LOG_CONFLICT_GAP                  =     0x100,
  ARD_LOG_CONFLICT_RANK                 =     0x200,
  ARD_LOG_CONFLICT_AUTOGAP              =     0x400
}ard_log_conflict_type_e;


/*! @brief DLM ARD conflict outcome types */
typedef enum
{
  ARD_LOG_CONFLICT_OUTCOME_NONE,        
  ARD_LOG_CONFLICT_OUTCOME_RESOLVED,    
  ARD_LOG_CONFLICT_OUTCOME_OVERRIDDEN,  
  ARD_LOG_CONFLICT_OUTCOME_BLOCKED,  
}ard_log_conflict_outcome_e;

/*! mirror FW result type for log packet */
typedef enum
{
  ARD_EVAL_LOG_RESULT_TYPE_NO_RES = 0,
  ARD_EVAL_LOG_RESULT_TYPE_SPEFF_UPDATE,
  ARD_EVAL_LOG_RESULT_TYPE_SPEFF_EVAL
} lte_ml1_log_dlm_ard_eval_results_type_e;

/*! mirror FW error codes for log packet */
typedef enum
{
  ARD_EVAL_LOG_ERROR_NONE = 0,
  ARD_EVAL_LOG_ERROR_SPEFF_UPDATE_COLLISION_WITH_P_OR_AP,
  ARD_EVAL_LOG_ERROR_SPEFF_UPDATE_NOT_SCHEDULED, /*5 Carriers max for CSF- this error code may kick in for >5CA cases*/
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_COLLISION_WITH_P_OR_AP,
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_NOT_SCHEDULED, /*5 Carriers max for CSF- this error code may kick in for >5CA cases*/
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_CSIRS_NOT_PRESENT_SUBFN,
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_INVALID_SUBFN_0,
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_INVALID_SUBFN_1,
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_INVALID_SUBFN_2,
  ARD_EVAL_LOG_ERROR_SPEFF_EVAL_RESULTS_TIMEOUT,
  ARD_EVAL_LOG_ERROR_CRS_NOT_DETECTED,
  ARD_EVAL_LOG_ERROR_FTL_RESULTS_NOT_UPDATED,
} lte_ml1_log_dlm_ard_eval_error_code_e;


/*! mirror FW result type for log packet */
typedef enum
{
  ARD_EVAL_LOG_SKIP_CAUSE_NA = 0,
  ARD_EVAL_LOG_SKIP_CAUSE_FW_ERROR,
  ARD_EVAL_LOG_SKIP_CAUSE_CDRX_OFF,
  ARD_EVAL_LOG_SKIP_CAUSE_HO_START,
  ARD_EVAL_LOG_SKIP_CAUSE_STOP,
  ARD_EVAL_LOG_SKIP_CAUSE_RECFG,
  ARD_EVAL_LOG_SKIP_CAUSE_SUSPEND,
  ARD_EVAL_LOG_SKIP_CAUSE_GAP,
  ARD_EVAL_LOG_SKIP_CAUSE_TOGGLE,
  ARD_EVAL_LOG_SKIP_CAUSE_PEND,
  ARD_EVAL_LOG_SKIP_CAUSE_PEND_SRX,
  ARD_EVAL_LOG_SKIP_CAUSE_CONFLICT,
  ARD_EVAL_LOG_SKIP_CAUSE_DISALLOW,
} ard_eval_log_skip_reason_e;

#define LTE_ML1_LOG_DLM_ARD_EVAL_RESULTS_VERSION 48

/*! @brief DLM ARD Eval Results Struct
    size = 4 + 23*4*7 = 648 bytes
    Packet ID: LOG_LTE_ML1_ARD_EVAL_RESULTS_LOG_C
 
    This is an external log packet that captures Eval results across carriers
    sent to ARD controller.
*/
typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32                         version:8;
  uint32                         num_curr_carriers:8; ///< Number of carriers in decision Info
  uint32                         seq_num:16; ///< Log packet sequence number

  /*! This is an array of carriers since multiple carriers can have Eval Results 
      at the same time */
  struct
  {
    /*! 1st word */
    uint32                        carrier_id:4; ///< Carrier 0-4
    uint32                        eval_time:16; ///< Eval time - 0 to 10239
    uint32                        pre_num_rx:4; ///< Pre-eval Number of Rx antennas 1-4
    uint32                        reserved0:8; ///< Reserved

    /*! 2nd word */
    uint8                         pre_rx_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Pre-eval Antenna<->Rx map

    /*! 3rd word */
    uint32                        num_rx:4; ///< Number of Rx antennas 1-4
    uint32                        reserved1:28; ///< Reserved

    /*! 4th word */
    uint8                         rx_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map

    /*! 5th word */
    uint32                        skipped:1;///< Yes/No
    uint32                        skip_reason:8; ///< Skip reason
    uint32                        result_type:4; ///< Result type
    uint32                        error_code:6; ///< error code
    uint32                        best2rx_physical_rxmask:4; ///< Bitmask of best2rx physical Rx
                                                                  ///< 0  NA
                                                                  ///< 3  RX0_RX1
                                                                  ///< 5  RX0_RX2
                                                                  ///< 9  RX0_RX3
                                                                  ///< 6  RX1_RX2
                                                                  ///< 10 RX1_RX3
                                                                  ///< 12 RX2_RX3
    uint32                        best2rx_logical_rxmask:4; ///< Bitmask of best2rx logical Rx
    uint32                        rx23_preferred:1; ///< Rx23 preferred - TRUE or FALSE
    uint32                        reserved2:4; ///< Reserved

    /*! 6th, 6th, 8th, 9th word */
    struct
    {
      uint32                      speff_res[LTE_ML1_LOG_DLM_ARD_4RX_SPFF_CNT]; ///< R1/R2/R3/R4 4Rx SPEFF results
    }speff_4rx_result;

    /*! 10th, 11th word */
    struct
    {
      uint32                      speff_res[LTE_ML1_LOG_DLM_ARD_BEST2_SPFF_CNT]; ///< R1/R2 Best2Rx SPEFF results
    }speff_best2rx_result;

    /*! 12th, 13th, 14th, 15th, 16th, 17th */
    uint32                        rx_ant_corr[LTE_ML1_LOG_DLM_ARD_NUM_ANT_CORRELATION]; ///< Rx antenna correlation for each pair

    /*! 18th word */
    int8                          ftl_snr[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< FTL SNR: -20 to 40 dB

    /*! 19th, 20th word */
    int16                         inst_rsrp[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Inst RSRP: -180 to -30 dBm

    /*! 21st, 22nd word */
    int16                         inst_rssi[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Inst RSSI: -110 to -10 dBm
    /*! 23rd word */
    int8                          inst_rsrq[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Inst RSRQ: -30 to +10 dB
    
  }carrier_info[LTE_ML1_DLM_LOG_MAX_CARRIERS];
} lte_ml1_log_dlm_ard_eval_results_s;


/*! @brief DLM QC ARD SRX group ids */
typedef enum
{
  ARD_LOG_SRX_GROUP_NONE,
  ARD_LOG_SRX_GROUP_1,
  ARD_LOG_SRX_GROUP_2,
  ARD_LOG_SRX_GROUP_3,
}ard_srx_group_id_e;


#define LTE_ML1_LOG_DLM_ARD_STATUS_VERSION 50

/*! @brief DLM QC ARD Status log structure
	if volte_enabled
		size = 392 bytes
	else
		size = 308 bytes
    Packet ID: LOG_LTE_ML1_QC_ARD_STATUS_LOG_C
 
    ARD Status Log packet captures Rxmap and CSF state change
    for mutliple carriers. It also logs various ARD sub-modules information
    and system/per-CC settings at the time of the transition.
*/
typedef struct
{
  /*! 12th word */
  uint32                        old_volte_state:4; ///< Volte state 0-4
  uint32                        old_volte_num_antennas:4; ///< Number of Rx antennas 1-4
  uint32                        new_volte_state:4; ///< state 0-4
  uint32                        new_volte_num_antennas:4; ///< Number of Rx antennas 1-4	
  uint32                        reserved2:16; ///< Reserved

  /*! 13th word */
  uint8                         old_volte_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map

  /*! 14th word */
  uint8                         new_volte_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map
} lte_ml1_log_dlm_qc_ard_volte_status_s;

typedef struct
{
  /*!***************** Version *****************/
  /*! Log packet version */
  uint32  version:8;
  uint32  num_curr_carriers:6; ///< Number of carriers in decision Info
  uint32  volte_enabled:1; ///< Volte enabled : Yes or No
  uint32  forced_max_rx:1; ///< Forced Max Rx : Yes or No
  uint32  seq_num:16; ///< Log packet sequence number

  /*! this is an array of carriers since multiple carriers can have state
     transitions at the same time */
  struct
  {
    /*! 1st word */
    uint32                        carrier_id:4; ///< Carrier 0-4
    uint32                        srx_group_id:4; ///< SRX group id 0 to 3, 0 = NONE
    uint32                        failure_reason:24; ///< Failure reason - NA if transition status is Pass

    /*! 2nd word */
    uint32                        event_time:16; ///< Event time - 0 to 10239
    uint32                        transition_time:16; ///< Transition time - 0 to 10239

    /*! 3rd word */
    uint32                        txn_reason; ///< bitmask of ARD transition reasons

    /*! 4th word */
    uint32                        conflict_reason_mask:12; ///< Failure reason - NA if transition status is Pass
    uint32                        conflict_outcome:4; ///< Failure reason - NA if transition status is Pass
    uint32                        old_num_antennas:4; ///< Number of Rx antennas 1-4
    uint32                        new_num_antennas:4; ///< Number of Rx antennas 1-4
    uint32                        reserved0:8; ///< Reserved

    /*! 5th word */
    uint8                         old_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map

    /*! 6th word */
    uint8                         new_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map
	
    /*! 7th word */
    uint32                        csf_steady_state:4; ///< csf steady state
    uint32                        traffic_macro_state:4;
    uint32                        traffic_sub_state:4;
    uint32                        traffic_reason:4; ///< Traffic subreason	
    uint32                        old_csf_state:4; ///< CSF state 0-4
    uint32                        old_csf_num_antennas:4; ///< Number of Rx antennas 1-4
    uint32                        new_csf_state:4; ///< CSF state 0-4
    uint32                        new_csf_num_antennas:4; ///< Number of Rx antennas 1-4

    /*! 8th word */
    uint8                         old_csf_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map
	
    /*! 9th word */
    uint8                         new_csf_ant_mapping[LTE_ML1_LOG_DLM_HO_RXD_NUM_ANT]; ///< Antenna<->Rx map

    /*! 10th word */
    uint32                        last_rank:3; ///< Last Rank - 1 to 4
    uint32                        last_rank_time:16; ///< Last Rank time - 0 to 10239
    uint32                        has_4rxcap:1; ///< has_4rxcap TRUE or FALSE
    uint32                        best2rx_allowed:1; ///< Best2Rx allowed TRUE or FALSE
    uint32                        allow_1rx:1; ///< 1Rx allowed TRUE or FALSE
    uint32                        flow_control:1; ///< Flow Control TRUE or FALSE
    uint32                        reserved1:9; ///< Reserved
	
    /*! 11th word */
    lte_ml1_log_dlm_qc_ard_volte_status_s volte_status; ///< Volte state info, if volte_enabled TRUE

  }carrier_state_info[LTE_ML1_DLM_LOG_MAX_CARRIERS];
} lte_ml1_log_dlm_qc_ard_status_s;

#endif /* LTE_ML1_LOG_EXT_H */
