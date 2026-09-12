#ifndef LOG_CODES_GSM_H
#define LOG_CODES_GSM_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 2010-2016 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                             Edit History

$Header: //components/rel/geran.mpss/7.4.0/api/public/log_codes_gsm.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
17/03/15   cs      CR804196 Use new passthrough versioned FCCH/SCH log packet
12/12/14   cs      CR754853 Add support for Parallel SCH detection handling
11/11/14   ws      CR702510 Add support for gprs message metrics E log packet
23/09/14   jj      CR 728792  Discrete Fourier transform (DFT) spur metric log packet
19-08-14   pjr     CR711506 Support for AMR Partial Decode and DTX detection FR19667
09-09-14   jk      CR697308:GL1 changes to support HLin LLin sawless feature
11-07-14   ws      CR690995 Hybrid AEQ log packet definition and implementation
10-07-14   jk      CR688587:GL1 changes to support LIF feature
21-07-14   jk      CR660801:Support for logging VAMOS log packet 0x5099
18-06-14   cos     CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
19-12-13   jk      CR503359 - GL1 changes to support Rx diversity
24-10-13   jj      CR 497390 LOG_GPRS_RECEIVE_BURST_METRICS_E_C_type
02-07-13   pjr     CR507542: Added LOG_GSM_COEX_PARAMS_C
10-09-08   tjw     Added LOG_GSM_AEQ_SCH_METRICS_C
09-02-26   tjw     Added LOG_GSM_NCELL_ASYNC_METRICS_C
09-02-11   tjw     Initial revision created by "splitting" modem/geran/gdiag/inc/log_codes_gsm.h

===========================================================================*/
/* APICOPY__LOG_GSM_BASE_C is defined in log_codes.h, define a copy here, explicitly
   and then check it where the header file can be included (i.e. NOT in an API file
   like this) */
#define APICOPY__LOG_GSM_BASE_C 0x5000
#define GSM_LOG_DEF( lOGvAL )   ( ( lOGvAL ) + APICOPY__LOG_GSM_BASE_C )

/* ========================   GSM LAYER1 PACKETS   ======================== */
#define LOG_GSM_POWER_SCAN_C                          GSM_LOG_DEF( 0x064 )
#define LOG_GSM_FCCH_DECODE_C                         GSM_LOG_DEF( 0x065 )
#define LOG_GSM_SCH_DECODE_C                          GSM_LOG_DEF( 0x066 )
#define LOG_GSM_FCCH_DECODE_V_C                       GSM_LOG_DEF( 0x413 )
#define LOG_GSM_SCH_DECODE_V_C                        GSM_LOG_DEF( 0x412 )
#define LOG_GSM_RECEIVE_BURST_METRICS_C               GSM_LOG_DEF( 0x06C )
#define LOG_GSM_PAGING_DATA_C                         GSM_LOG_DEF( 0x06F )
#define LOG_GSM_NCELL_ENGINE_MODE_C                   GSM_LOG_DEF( 0x070 )
#define LOG_GSM_NCELL_BA_LIST_C                       GSM_LOG_DEF( 0x071 )
#define LOG_GSM_SERVING_CELL_POWER_C                  GSM_LOG_DEF( 0x072 )

#define LOG_GSM_GSTMR_ADJ_C                           GSM_LOG_DEF( 0x073 )
#define LOG_GSM_NCELL_ACQ_C                           GSM_LOG_DEF( 0x075 )
#define LOG_GSM_TXLEV_TA_C                            GSM_LOG_DEF( 0x076 )

/* New Message Metrics w/ both hard and soft RxQual */
#define LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C        GSM_LOG_DEF( 0x079 )

/* Packets for auxiliary L1 measurement reports */
#define LOG_GSM_SERVING_AUX_MEAS_C                    GSM_LOG_DEF( 0x07A )
#define LOG_GSM_NCELL_AUX_MEAS_C                      GSM_LOG_DEF( 0x07B )

/* New AFC log with PDM and rotator values */
#define LOG_GSM_AFC_ADJUST_C                          GSM_LOG_DEF( 0x07C )

/* New NCELL async rx metrics log */
#define LOG_GSM_NCELL_ASYNC_METRICS_C                 GSM_LOG_DEF( 0x07D )

#define LOG_GSM_MON_BURST_C                           GSM_LOG_DEF( 0x082 )
#define LOG_GSM_SCH_BURST_METRICS_C                   GSM_LOG_DEF( 0x083 )
#define LOG_GSM_BCCH_BURST_METRICS_C                  GSM_LOG_DEF( 0x085 )
#define LOG_GSM_BCCH_MSG_METRICS_C                    GSM_LOG_DEF( 0x086 )
#define LOG_GSM_SCH_MSG_METRICS_VER2_C                GSM_LOG_DEF( 0x087 )

/* Transmit burst metrics */
#define LOG_GSM_TRANSMIT_BURST_METRICS_C              GSM_LOG_DEF( 0x088 )

/* MDSP command log */
#define LOG_GSM_MDSP_CMD_C                            GSM_LOG_DEF( 0x08C )
#define LOG_GSM_GL1_HW_CMD_C                          GSM_LOG_DEF( 0x08D )
#define LOG_GSM_MDSP_DEBUG_BUFFER_C                   GSM_LOG_DEF( 0x08F )
#define LOG_GSM_SAIC_METRICS_C                        GSM_LOG_DEF( 0x091 )
#define LOG_GPRS_ESAIC_METRICS_C                      GSM_LOG_DEF( 0x092 )
#define LOG_GSM_AEQ_SCH_METRICS_C                     GSM_LOG_DEF( 0x093 )
#define LOG_GSM_COEX_PARAMS_C                         GSM_LOG_DEF( 0x094 )
#define LOG_GSM_DFT_SPUR_METRICS_C                    GSM_LOG_DEF( 0x095 )
#define LOG_GSM_RxD_METRICS_C                         GSM_LOG_DEF( 0x096 )
#define LOG_GSM_EPD_METRICS_C                         GSM_LOG_DEF( 0x097 )
#define LOG_GSM_PARALLEL_SCH_DATA_C                   GSM_LOG_DEF( 0x098 )
#define LOG_GSM_VAMOS_METRICS_C                       GSM_LOG_DEF( 0x099 )
#define LOG_GSM_METRICS_HLLL_STATE_C                  GSM_LOG_DEF( 0x09A )
#define LOG_GSM_SAIC_METRICS_V2_C                     GSM_LOG_DEF( 0x09B )
#define LOG_GPRS_ESAIC_METRICS_V2_C                   GSM_LOG_DEF( 0x09C )
#define LOG_GSM_AEQ_SCH_METRICS_V2_C                  GSM_LOG_DEF( 0x09D )
#define LOG_GSM_AEQ4_METRICS_C                        GSM_LOG_DEF( 0x09E )
#define LOG_GSM_LIF_METRICS_C                         GSM_LOG_DEF( 0x09F )
#define LOG_GSM_AMR_PD_DTX_BURST_METRICS_C            GSM_LOG_DEF( 0x0A2 )
#define LOG_GSM_AMR_PD_DTX_MESSAGE_METRICS_C          GSM_LOG_DEF( 0x0A3 )

/* ========================   GSM LAYER2 PACKETS   ======================== */
#define LOG_GSM_L2_STATE_C                            GSM_LOG_DEF( 0x0C8 )
#define LOG_GSM_L2_TRANSMISSION_STATUS_C              GSM_LOG_DEF( 0x0C9 )
#define LOG_GSM_L2_OUTSTANDING_FRAME_C                GSM_LOG_DEF( 0x0CA )

/* ========================   GSM LAYER3 PACKETS   ======================== */
#define LOG_GSM_RR_STATE_C                            GSM_LOG_DEF( 0x12C )
#define LOG_GSM_RR_PROTOCOL_ERROR_C                   GSM_LOG_DEF( 0x12E )
#define LOG_GSM_RR_SIGNALING_MESSAGE_C                GSM_LOG_DEF( 0x12F )
#define LOG_GSM_RR_CELL_RESELECTION_PARAMS_C          GSM_LOG_DEF( 0x130 )
#define LOG_GSM_RR_RACH_CONTROL_PARAMS_C              GSM_LOG_DEF( 0x131 )
#define LOG_GSM_RR_CONTROL_CHANNEL_PARAMS_C           GSM_LOG_DEF( 0x132 )
#define LOG_GSM_RR_CELL_OPTIONS_C                     GSM_LOG_DEF( 0x133 )
#define LOG_GSM_RR_CELL_INFO_C                        GSM_LOG_DEF( 0x134 )
#define LOG_GSM_RR_CHANNEL_CONFIGURATION_C            GSM_LOG_DEF( 0x135 )
#define LOG_GSM_RR_CIPHERING_MODE_C                   GSM_LOG_DEF( 0x136 )
#define LOG_GSM_RR_CELL_RESELECTION_MEASMNTS_C        GSM_LOG_DEF( 0x137 )
#define LOG_GSM_RR_DSC_COUNTER_C                      GSM_LOG_DEF( 0x138 )
#define LOG_GSM_RR_S_COUNTER_C                        GSM_LOG_DEF( 0x139 )
#define LOG_GSM_RR_SACCH_RPT_C                        GSM_LOG_DEF( 0x13A )
#define LOG_GSM_RR_AVAILABLE_PLMN_LIST_C              GSM_LOG_DEF( 0x13B )
#define LOG_GSM_RR_BA_LIST_C                          GSM_LOG_DEF( 0x13C )
#define LOG_GSM_RR_FAKE_CELL_DATA_C                   GSM_LOG_DEF( 0x144 )
#define LOG_GSM_RR_DED_MEAS_PARAMETERS_C              GSM_LOG_DEF( 0x145 )

#define LOG_GSM_RR_ACQ_DB_ARFCN_LIST                  GSM_LOG_DEF( 0x146 ) 

/* ===========================   GPRS PACKETS  ============================ */
#define LOG_GPRS_INDICATORS_C                         GSM_LOG_DEF( 0x1F4 )
#define LOG_GPRS_GENERAL_PARAMS_C                     GSM_LOG_DEF( 0x1F5 )
#define LOG_GPRS_CELL_OPTIONS_C                       GSM_LOG_DEF( 0x1F6 )
#define LOG_GPRS_POWER_CONTROL_PARAMS_C               GSM_LOG_DEF( 0x1F7 )
#define LOG_GPRS_MOBILE_ALLOCATION_C                  GSM_LOG_DEF( 0x1F8 )
#define LOG_GPRS_PBCCH_DESCRIPTION_C                  GSM_LOG_DEF( 0x1F9 )
#define LOG_GPRS_GRR_STATE_C                          GSM_LOG_DEF( 0x1FA )
#define LOG_GPRS_RR_CELL_RESELECTION_PARAMS_C         GSM_LOG_DEF( 0x1FB )
#define LOG_GPRS_RR_CELL_RESELECTION_MEASMNTS_C       GSM_LOG_DEF( 0x1FC )
#define LOG_GPRS_RR_PSI1_C                            GSM_LOG_DEF( 0x1FD )
#define LOG_GPRS_RR_PSI2_C                            GSM_LOG_DEF( 0x1FE )
#define LOG_GPRS_RR_PSI3_C                            GSM_LOG_DEF( 0x1FF )

#define LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C           GSM_LOG_DEF( 0x200 )
#define LOG_GPRS_RLC_UL_EVENT_COUNTS_C                GSM_LOG_DEF( 0x201 )
#define LOG_GPRS_RLC_UL_STATS_C                       GSM_LOG_DEF( 0x202 )

#define LOG_GPRS_RLC_UL_CONFIG_C                      GSM_LOG_DEF( 0x203 )
#define LOG_GPRS_RLC_DL_CONFIG_C                      GSM_LOG_DEF( 0x204 )

/* ========================  New items for EGPRS ========================= */
#define LOG_EGPRS_RLC_UL_HEADER_C                     GSM_LOG_DEF( 0x205 )
#define LOG_EGPRS_RLC_DL_HEADER_C                     GSM_LOG_DEF( 0x206 )
/* =====================  End of New items for EGPRS ====================== */

#define LOG_EGPRS_RLC_EPUAN_C                         GSM_LOG_DEF( 0x209 )

#define LOG_GPRS_RLC_DL_STATS_C                       GSM_LOG_DEF( 0x20A )
#define LOG_GPRS_RLC_UL_RELEASE_IND_C                 GSM_LOG_DEF( 0x20D )
#define LOG_GPRS_RLC_DL_RELEASE_IND_C                 GSM_LOG_DEF( 0x20E )
#define LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C         GSM_LOG_DEF( 0x20F )
#define LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C         GSM_LOG_DEF( 0x210 )

#define LOG_EGPRS_RLC_EPDAN_C                         GSM_LOG_DEF( 0x211 )

#define LOG_GPRS_LLC_ME_INFO_C                        GSM_LOG_DEF( 0x212 )
#define LOG_GPRS_LLC_SAPI_STATES_C                    GSM_LOG_DEF( 0x213 )
#define LOG_GPRS_LLC_XID_INFO_C                       GSM_LOG_DEF( 0x214 )
#define LOG_GPRS_LLC_PDU_STATS_C                      GSM_LOG_DEF( 0x215 )
#define LOG_GPRS_LLC_PERIODIC_STATS_C                 GSM_LOG_DEF( 0x216 )
#define LOG_GPRS_LLC_READY_TIMER_STATUS_C             GSM_LOG_DEF( 0x217 )
#define LOG_GPRS_LLC_SEND_UI_FRAME_C                  GSM_LOG_DEF( 0x218 )
#define LOG_GPRS_LLC_RECV_UI_FRAME_C                  GSM_LOG_DEF( 0x219 )
#define LOG_GPRS_LLC_READY_TIMER_STATUS_VER2_C        GSM_LOG_DEF( 0x21A )
#define LOG_GPRS_LLC_CIPHER_INFO_C                    GSM_LOG_DEF( 0x21B )

#define LOG_GPRS_SNDCP_ACTIVE_PDP_CONTEXTS_C          GSM_LOG_DEF( 0x21C )
#define LOG_GPRS_SNDCP_PDP_CONTEXT_INFO_C             GSM_LOG_DEF( 0x21D )
#define LOG_GPRS_SNDCP_STATES_C                       GSM_LOG_DEF( 0x21E )
#define LOG_GPRS_SNDCP_PDU_STATS_C                    GSM_LOG_DEF( 0x21F )
#define LOG_GPRS_SNDCP_UL_TCP_HDR_C                   GSM_LOG_DEF( 0x220 )
#define LOG_GPRS_SNDCP_DL_TCP_HDR_C                   GSM_LOG_DEF( 0x221 )

#define LOG_GPRS_RLC_UL_STATISTICS_C                  GSM_LOG_DEF( 0x222 )
#define LOG_GPRS_RLC_DL_STATISTICS_C                  GSM_LOG_DEF( 0x223 )

#define LOG_GPRS_MAC_SIGNALLING_MESSAGE_C             GSM_LOG_DEF( 0x226 )
#define LOG_GPRS_MAC_STATE_C                          GSM_LOG_DEF( 0x227 )
#define LOG_GPRS_MAC_UL_TBF_ESTABLISH_C               GSM_LOG_DEF( 0x228 )
#define LOG_GPRS_MAC_UL_TBF_RELEASE_C                 GSM_LOG_DEF( 0x229 )
#define LOG_GPRS_MAC_DL_TBF_ESTABLISH_C               GSM_LOG_DEF( 0x22A )
#define LOG_GPRS_MAC_DL_TBF_RELEASE_C                 GSM_LOG_DEF( 0x22B )
#define LOG_EGPRS_MAC_UL_ACKNACK_C                    GSM_LOG_DEF( 0x22C )
#define LOG_EGPRS_MAC_DL_ACKNACK_C                    GSM_LOG_DEF( 0x22D )
#define LOG_EGPRS_SRB_HARD_DECISIONS_C                GSM_LOG_DEF( 0x22E )


#define LOG_GPRS_SM_GMM_OTA_MESSAGE_C                 GSM_LOG_DEF( 0x230 )
#define LOG_GPRS_AIR_INTERFACE_SUMMARY_C              GSM_LOG_DEF( 0x231 )

#define LOG_GPRS_AGC_C                                GSM_LOG_DEF( 0x232 )
#define LOG_GPRS_TIMING_ADVANCE_C                     GSM_LOG_DEF( 0x233 )
#define LOG_GPRS_POWER_CONTROL_C                      GSM_LOG_DEF( 0x234 )
#define LOG_GPRS_TRANSFER_SUMMARY_C                   GSM_LOG_DEF( 0x235 )
//#define LOG_HOPPING_FREQUENCY_ARFCN_C                 GSM_LOG_DEF( 0x236 )
#define LOG_GPRS_TX_SCHEDULED_RESULTS_C               GSM_LOG_DEF( 0x237 )

/* This is used in both GPRS and EGPRS */
#define LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C             GSM_LOG_DEF( 0x23F )

#define LOG_GPRS_RECEIVE_BURST_METRICS_A_C            GSM_LOG_DEF( 0x245 )
#define LOG_GPRS_RECEIVE_BURST_METRICS_B_C            GSM_LOG_DEF( 0x246 )
#define LOG_GPRS_RECEIVE_BURST_METRICS_C_C            GSM_LOG_DEF( 0x247 )
#define LOG_GPRS_RECEIVE_BURST_METRICS_D_C            GSM_LOG_DEF( 0x248 )
#define LOG_GPRS_RECEIVE_BURST_METRICS_E_C            GSM_LOG_DEF( 0x249 )
#define LOG_GPRS_RECEIVE_MSG_METRICS_B_C              GSM_LOG_DEF( 0x24A )
#define LOG_GPRS_RECEIVE_MSG_METRICS_C_C              GSM_LOG_DEF( 0x24B )
#define LOG_GPRS_RECEIVE_MSG_METRICS_D_C              GSM_LOG_DEF( 0x24C )
#define LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C         GSM_LOG_DEF( 0x24D )
#define LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C         GSM_LOG_DEF( 0x24E )
#define LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C         GSM_LOG_DEF( 0x24F )
#define LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C         GSM_LOG_DEF( 0x250 )
#define LOG_GPRS_RECEIVE_MSG_METRICS_E_VER2_C         GSM_LOG_DEF( 0x253 )

/* ==============   GSM MODE WCDMA MEASUREMENT PACKETS   ================== */
#define LOG_GSM_WCDMA_KNOWN_LIST_C                    GSM_LOG_DEF( 0x258 )
#define LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C               GSM_LOG_DEF( 0x259 )
#define LOG_GSM_WCDMA_DED_CELL_LIST_C                 GSM_LOG_DEF( 0x25A )

/* ==============   GSM MODE WCDMA MEASUREMEMTS REPORT FROM RR ============ */
#define LOG_GSM_DED_3G_MEASUREMENTS_C                 GSM_LOG_DEF( 0x25B )

/* ============== GSM to WCDMA Reselection Rejected at WCDMA ============= */
#define LOG_GSM_IDLE_MODE_3G_RESELECT_REJ_C           GSM_LOG_DEF( 0x25C )

/* ==============   GSM/GPRS INTERRAT PACKETS            ================== */
#define LOG_GPRS_RR_3G_MEASUREMENT_PARAMS_C           GSM_LOG_DEF( 0x262 )
#define LOG_GPRS_RR_3G_RESELECTION_MEASUREMENTS_C     GSM_LOG_DEF( 0x263 )

/* =====================   GERAN COMMON LOG PACKETS   ===================== */
#define LOG_GERAN_F3_MESSAGES_C                       GSM_LOG_DEF( 0x264 ) /* Not Implemented */

/* ======================================================================== */
/* User defined codes (starting at bit 1024, or 0x5400, incrementing by 0x10) */
#define LOG_GSM_FN_ADJ_C                              GSM_LOG_DEF( 0x400 )
#define LOG_GSM_NEXT_USR_CODE_C                       GSM_LOG_DEF( 0x420 )

/* ========================       WMS PACKETS      ======================== */
#define LOG_WMS_RECEIVE_CB                            GSM_LOG_DEF( 0x384 )
#define LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C             GSM_LOG_DEF( 0x385 )
#define LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C           GSM_LOG_DEF( 0x386 )

/* ======================================================================== */
/* Last GSM code
   - Must be beyond the GPRS range or GPRS logging will not work */
#define LOG_GSM_LAST_C                                (LOG_GSM_NEXT_USR_CODE_C + 0x0A00)

#endif /* LOG_CODES_GSM_H */
