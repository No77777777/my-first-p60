#ifndef TDSRLCLOG_H
#define TDSRLCLOG_H

/*===============================================================================================
                             R L C L O G . H

DESCRIPTION
This file contains RLC layer log packet structure definitions, 
prototypes for RLC logging functions, any constant definitions that are needed, and any external
variable declarations needed for RLC logging.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

================================================================================================*/


/*================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlclog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS

================================================================================================*/

#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "tdsuecomdef.h"
#include "log.h"
#include "tdsrlcrrcif.h"
#include "log_codes_tdscdma.h"
#ifdef T_WINNT
  #error code not present
#endif

#define TDSRLC_UL_NULL_STATE         0
#define TDSTM_UL_DATA_TRANSFER_READY 1
#define TDSUM_UL_DATA_TRANSFER_READY 2
#define TDSUM_UL_WAITING_TO_SUSPEND  3
#define TDSUM_UL_SUSPENDED           4
#define TDSAM_UL_DATA_TRANSFER_READY 5
#define TDSAM_UL_WAITING_TO_SUSPEND  6
#define TDSAM_UL_SUSPENDED           7
#define TDSAM_UL_RESET_PENDING       8
#define TDSAM_UL_CONFIG_PENDING      9

#define TDSRLC_DL_NULL_STATE         0
#define TDSTM_DL_DATA_TRANSFER_READY 1
#define TDSUM_DL_DATA_TRANSFER_READY 2
#define TDSAM_DL_DATA_TRANSFER_READY 3
#define TDSAM_DL_RESET_PENDING       4
       
#define TDSRLC_BCCH 0
#define TDSRLC_PCCH 1
#define TDSRLC_CCCH 2                      
#define TDSRLC_DCCH 3                     
#define TDSRLC_CTCH 4                     
#define TDSRLC_DTCH 5     
                
#define TDSRLC_DISCARD_MODE_TBE 0
#define TDSRLC_DISCARD_MODE_MAXDAT 1
#define TDSRLC_DISCARD_MODE_NO_DISCARD 2

  #define TDSMAX_UL_RLC_LOG_PDUS 16
  #define TDSMAX_DL_RLC_LOG_PDUS 16

#define TDSMAX_RLC_LOG_PDUS_SIZE 256

#define TDSMAX_CIPHER_PDU 24

#define TDSRLC_LOG_VERSION   1



/*! Data Structure for Unacknowledged and Acknowledged Mode Uplink Ciphering. */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8    rlc_ul_id;           /*!< \brief UL logical channel id(0..18)            */
  uint16   rb_id;               /*!< \brief Radio bearer id (0..31)                 */
  boolean  ciph_mode;           /*!< \brief Ciphering enabled - 0: NO, 1: Yes       */
  uint32   ciph_id;             /*!< \brief Ciphering key Id                        */
                                /*!< \brief 00 - 0, 01 - 1, 10 - 2, 11 - 3.         */
  uint32   hfn_am_um;           /*!< \brief Hyperframe Number (20 bits)             */
  uint16   ciph_act_sn;         /*!< \brief the SN to start the ciphering           */
}tdsrlc_log_ul_ciphering_type;

/*! Data Structure for Unacknowledged and Acknowledged Mode Downlink Ciphering. */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8    rlc_dl_id;           /*!< \brief DL logical channel id (0..18)           */
  uint16   rb_id;               /*!< \brief Radio bearer id (0..31)                 */
  boolean  ciph_mode;           /*!< \brief Ciphering enabled - 0: NO, 1: Yes       */
  uint32   ciph_id;             /*!< \brief Ciphering key Id                        */
                                /*!< \brief 00 - 0, 01 - 1, 10 - 2, 11 - 3.         */
  uint32   hfn_am_um;           /*!< \brief Hyperframe Number (20 bits)             */
  uint16   ciph_act_sn;         /*!< \brief the SN to start the ciphering           */
}tdsrlc_log_dl_ciphering_type;

/*! Data Structure for RLC UL states for Transparent, Unacknowledged and 
  Acknowledged Modes. */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  uint8    rlc_ul_id;           /*!< \brief UL logical channel id (0..18)           */
  uint8    rlc_state;           /*!< State of the RLC State Machine 
				 0000 - UL_NULL_STATE                    
                                 0001 - TDSTM_UL_DATA_TRANSFER_READY        
                                 0010 - TDSUM_UL_DATA_TRANSFER_READY        
                                 0011 - TDSUM_UL_WAITING_TO_SUSPEND         
                                 0100 - TDSUM_UL_SUSPENDED                  
                                 0101 - TDSAM_UL_DATA_TRANSFER_READY        
                                 0110 - TDSAM_UL_WAITING_TO_SUSPEND         
                                 0111 - TDSUM_UL_SUSPENDED                  
                                 1000 - TDSAM_UL_RESET_PENDING              */
} tdsrlc_log_ul_state_type;

/*! Data Structure for RLC DL states for Transparent, Unacknowledged and 
  Acknowledged Modes */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  uint8    rlc_dl_id;           /*!< \brief DL logical channel id (0..18)           */
  uint8    rlc_state;           /*!< \brief State of the RLC State Machine          
                                0000 - DL_NULL_STATE                    
                                0001 - TDSTM_DL_DATA_TRANSFER_READY        
                                0010 - TDSUM_DL_DATA_TRANSFER_READY        
                                0011 - TDSAM_DL_DATA_TRANSFER_READY        
                                0100 - TDSAM_DL_RESET_PENDING              */
} tdsrlc_log_dl_state_type;

/*! Data structure for RLC Uplink Transparent Mode */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8 log_chan_type;          /*!< \brief Type of TM UL Logical Channel
                                 TDSRLC_CCCH   
                                 TDSRLC_DCCH
                                 TDSRLC_DTCH 
								*/
  uint8 log_chan_ul_rlc_id;     /*!< \brief UL Logical Channel RLC ID(0..18)        */
  uint8 time_disc;              /*!< \brief Elapsed time in msec before an SDU is   
                                discarded at the transmitter.Values:    
                                10, 20, 30, 40, 50, 60, 70, 80, 90, 100 
								*/
  boolean segm;                 /*!< \brief Segmentation is allowed or not.
                                0 - FALSE
                                1 - TRUE 
								*/
}tdsrlc_log_ul_tm_type;


/*! Data structure for RLC Downlink Transparent Mode */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8 log_chan_type;          /*!< \brief Type of TM DL Logical Channel           
                                TDSRLC_BCCH
                                TDSRLC_PCCH
                                TDSRLC_DCCH
                                TDSRLC_DTCH
								*/
  uint8 log_chan_dl_rlc_id;     /*!< \brief DL Logical Channel RLC ID(0..18) */
  boolean segm;                 /*!< \brief Segmentation is allowed or not.         
                                0 - FALSE
                                1 - TRUE 
								*/
}tdsrlc_log_dl_tm_type;


/*! Data structure for RLC Uplink Unacknowledged Mode */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_chan_type;         /*!< \brief Type of UM UL Logical Channel
                                TDSRLC_DCCH
                                TDSRLC_DTCH
								*/
  uint8  log_chan_ul_rlc_id;    /*!< \brief UL Logical Channel RLC ID(0..18)        */
  uint8  time_disc;             /*!< \brief Elapsed time in msec before an SDU
                                is discarded at the transmitter. Values:
                                10, 20, 30, 40, 50, 60, 70, 80, 90, 100 */
  uint8  li_length;             /*!< \brief Size of the length indicator in UMD
                                PDU - 7 or 15 bits. 
								*/
  uint32 hfn;                   /*!< \brief Hyperframe Number - 20 bits             */
}tdsrlc_log_ul_um_type;


/*!  Data structure for RLC Downlink Unacknowledged Mode */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_chan_type;         /*!< \brief Type of UM DL Logical Channel
                                TDSRLC_CCCH
                                TDSRLC_DCCH
                                TDSRLC_CTCH
                                TDSRLC_DTCH
								*/
  uint8  log_chan_dl_rlc_id;    /*!< \brief DL Logical Channel RLC ID(0..18)        */
  uint8  li_length;             /*!< \brief Size of the length indicator in UMD
                                PDU - 7 or 15 bits.
								*/
  uint32 hfn;                   /*!< \brief Hyperframe Number - 20 bits             */
}tdsrlc_log_dl_um_type;

/*!  Data structure for RLC Uplink Acknowledged Mode */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_data_ul_rlc_id;    /*!< \brief Data Logical Channel UL RLC ID (0..18)  */
  uint8  log_cont_ul_rlc_id;    /*!< \brief Control Logical Channel UL RLC ID(0..18)*/
  uint16 rb_id;                 /*!< \brief Radio Bearer ID                         */
  uint8  log_chan_type;         /*!< \brief Logical Channel Type
                                TDSRLC_DCCH
                                TDSRLC_DTCH
								*/
  uint8  li_length;             /*!< \brief Size of the length indicator field
                                in the AMD PDU, 7 bits or 15 bits.      */
  uint16 trans_wind_size;       /*!< \brief Max number of RLC PUs permitted to
                                be sent without receiving an ACK,
                                Max. value 4095 , but only 15 values.   */
  uint16 tmr_rst;               /*!< \brief Timer for detecting loss of RESET
                                ACK PDU. Max. value 1000, but only 16 values.
								*/
  uint32 hfn;                   /*!< \brief Hyperframe Number - 20 bits.            */
  uint8  max_rst;               /*!< \brief Maximum number of Retransmissions of
                                RESET PDU. Values: 1, 4, 6, 8, 12, 16, 24, 32 */
  tdsrlc_discard_mode_e_type 
        discard_mode;           /*!< \brief SDU_DISCARD: TDSRLC_DISCARD_MODE_TBE
                                TDSRLC_DISCARD_MODE_MAXDAT
                                TDSRLC_DISCARD_MODE_NO_DISCARD*/
  uint32 tmr_discard;           /*!< \brief Elapsed time in ms before an SDU is
                                discarded. Max value 7500, but only 16 values. */
  uint16 tmr_mrw;               /*!< \brief Used to trigger the retransmission of a
                                status PDU containing an MRW SUFI field.
                                Max value 1000, but only 16 values.     */
  uint8 max_dat;                /*!< \brief Number of retx. of a PU before the SDU 
                                to which it belongs is discarded.Max
                                value is 40, but only 16 values.        */
  uint8 max_mrw;                /*!< \brief Max number of retx. of the MRW command,
                                Values - 1, 4, 6, 8, 12, 24, 32         */
  uint16 tmr_poll_proh;         /*!< \brief Minimum time between polls in ms(0 disabled).*/
  uint16 tmr_poll;              /*!< \brief Poll retx timeout in ms.                */
                                /*!< \brief Max value 1000, but only 17 values      */
  uint8 poll_pdu;                /*!< \brief Interval between polls in units of PUs  
                                (0= POLL_PU disabled)                   
                                Values - 0, 1, 2, 4, 8, 16, 32, 64, 128 */
  uint8 poll_sdu;               /*!< \brief POLL_SDU - Interval between polls       
                                in units of SDUs                        
                                0 - POll_SDU is disabled, 1, 4, 16, 64  */
  boolean last_tx_poll;         /*!< \brief Last_tx_poll [3]- Indicates if a poll   
                                should be sent when the last PU in the  
                                tx buffer is transmitted.               
                                    0 - Off                             
                                    1 - On                               */
  boolean last_retx_poll;       /*!< \brief Last_retx_poll [4]- Indicates if a poll 
                                  !< \brief should be sent when the last PU in the  
                                retx buffer is transmitted.             
                                 0 - Off                              
                                 1 - On                               */                    
  uint8 poll_win;               /*!< \brief poll_win : When a poll_win % of the     
                                Tx window is reached, a poll should be  
                                transmitted. Values: 0, 50, 60, 70, 80, 85, 90, 95, 100*/
  uint16 timer_poll_periodic;   /*!< \brief Interval between polls  in ms for       
                                periodic polling. Values: 0, 100, 200,  
                                300, 400, 500, 750, 1000, 2000          */
} tdsrlc_log_ul_am_type;


/*! Data structure for RLC Downlink Acknowledged Mode.*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_data_dl_rlc_id;    /*!< \brief Data Logical Channel DL RLC ID(0..18)   */
  uint8  log_cont_dl_rlc_id;    /*!< \brief Control Logical Channel DL RLC ID(0..18)*/
  uint16 rb_id;                 /*!< \brief Radio Bearer ID                         */
  uint8  log_chan_type;         /*!< \brief Logical Channel Type                    
									TDSRLC_DCCH                                
									TDSRLC_DTCH                                */
  uint8  li_length;             /*!< \brief Size of the length indicator field      
                                in the AMD PDU, 7 bits or 7 bits        */
  uint16 rcv_wind_size;         /*!< \brief Max number of RLC PUs allowed to        
                                 be received without receiving an ACK,   
                                 integer max 4095 - 14 values            */
  uint16 tmr_epc;               /*!< \brief Timer to wait in ms before the estimated
                                PDU counter starts to decrement.        
                                Max value - 1000, 17 values             */
  uint32 hfn;                   /*!< \brief Hyperframe Number - 20 bits             */
  uint16 tmr_period;            /*!< \brief Time between STATUS reports in ms for   
                                periodic status reporting.              
                                Max - 1200, 8 values.                   */
  boolean inseq_delivery;       /*!< \brief Indicates if RLC should                 
                                preserve the order of higher layer PDUs.
                                 0 - False                             
                                 1 - True                              */
  uint16 status_prohibit;       /*!< \brief Minimum time between STATUS reports
                                 in ms. Values: 0, 160, 320, 640, 1280.  */
  boolean missing_pdu_ind;      /*!< \brief Indicates if the UE should send a       
                                STATUS report for each missing PU that is detected. 
                                   0 - False                             
                                   1 - true                              */

} tdsrlc_log_dl_am_type;

/*!  Data structure for logging RLC Uplink Acknowledged Mode Statistics */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_data_ul_rlc_id;      /*!< \brief Data Logic Channel UL RLC ID(0..18)   */
  uint8  log_ctl_ul_rlc_id;       /*!< \brief Control Logic Channel UL RLC ID(0..18)*/
  uint8  log_chan_type;           /*!< \brief Logical Channel Type (-1..5)          */
  uint16 tot_num_rlc_resets;      /*!< \brief Total number of rlc resets            */
  uint32 tot_num_pdu_byte_txd;    /*!< \brief Total number of PDU bytes Txd         */
  uint32 tot_num_sdu_bytes_txd;   /*!< \brief Total number of SDU bytes Rxd from upper layer */
  uint16 tot_num_data_pdu_txd;    /*!< \brief total number of AMD PDUs transmitted  */
  uint16 tot_num_data_pdu_retxd;  /*!< \brief Total number of AMD PDUs retransmitted*/
  uint16 tot_num_pdu_nak;         /*!< \brief Total number of AMD PDUs nakd to NW   */
  uint16 tot_num_ctl_pdu_txd;     /*!< \brief Total number of control PDUs Txd      */
  uint16 sn_last_pdu_txd;         /*!< \brief Sequence number of last PDU Txd. VT_S 0..4096 */
  uint16 sn_last_inseq_pdu_ackd;  /*!< \brief Sequence number of the last insequence
                                       pdu acked by the peer entity.  VT_A 0..4096 */
  uint16 tx_win_size;             /*!< \brief Transmission window size. VT_MS 1..4095 */
  uint16 ul_am_buf_status;        /*!< \brief Snap shot of the AM buffer occupancy  */
  uint16 tot_num_pdus_with_two_retrans; /*!< \brief Total number of retrans for     */
  uint16 tot_num_pdus_with_three_retrans; /*!< \brief pdus which have more than 2   */
  uint16 tot_num_pdus_with_four_retrans;  /*!< \brief dat counts. Number of PDUs    */
  uint16 tot_num_pdus_with_five_retrans; /*!< \brief with 2, 3, 4, or >=6 dat counts*/
  uint16 tot_num_pdus_with_six_or_more_retrans; 
} tdsrlc_log_ul_am_stat_type;

/*!  Data structure for logging RLC Downlink Acknowledged Mode Statistic */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_data_dl_rlc_id;      /*!< \brief Data Logic Channel DL RLC ID(0..18)   */
  uint8  log_ctl_dl_rlc_id;       /*!< \brief Control Logic Channel DL RLC ID (0-18)*/
  uint8  log_chan_type;           /*!< \brief Logical Channel Type (-1..5)          */
  uint32 tot_num_pdu_byte_rxd;    /*!< \brief Total number of PDU bytes received    */
  uint32 tot_num_sdu_byte_rxd;    /*!< \brief Total number of SDU bytes sends to the upper layer */
  uint16 tot_num_error_pdu_rxd;   /*!< \brief Erroneous PDUs Rx'd                   */
  uint16 tot_num_data_pdu_rxd;    /*!< \brief Total number of AMD PDUs received     */
  uint16 tot_num_ctl_pdu_rxd;     /*!< \brief Total number of Control PDUs rx'd     */
  uint16 tot_num_pdu_nak;         /*!< \brief Total number of AMD PDUs NAK by the peer entity */
  uint16 sn_last_inseq_pdu_rxd;   /*!< \brief Sequence number of last in sequence PDU received. VR(R) 0..4096 */
  uint16 highest_sn_pdu_rxd;      /*!< \brief Highest seq number PDU Rxd VR_H       */
                                  /*!< \brief 0..4096                               */
} tdsrlc_log_dl_am_stat_type;

/*! Data structure for logging UL RLC PDU information */

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_data_ul_rlc_id;      /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint16 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_UL_RLC_LOG_PDUS];
                                  /*!< \brief The first four bytes of the pdu       */
} tdsrlc_log_ul_am_pdu_data_type;

/*! Data structure for logging DL RLC PDU information */

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_data_dl_rlc_id;      /*!< \brief Data Logical Channel DL RLC ID(0..18) */
  uint16 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_DL_RLC_LOG_PDUS];
                                  /*!< \brief The first four bytes of the pdu       */
} tdsrlc_log_dl_am_pdu_data_type;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*! Data structure for logging Flexible DL RLC PDU logging and Size */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint32 log_data;                /*!< \brief The first four bytes of the pdu       */
} tdsrlc_log_flex_dl_am_pdu_info_type;

/*! Data structure for logging Flexible DL RLC PDU information */

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_data_dl_rlc_id;      /*!< \brief Data Logical Channel DL RLC ID        */
  uint8  num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint8 li_length;                /*!< \brief Length of the length indicator        */
                                  /*!< \brief 1 - 7 bits, 2 - 15 bits               */
} tdsrlc_log_flex_dl_am_pdu_data_type;
#endif /* ((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)) */

/*! Data structure for logging UL RLC AM NAK PDU information */

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_ctl_ul_rlc_id;       /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint8  log_data[TDSMAX_RLC_LOG_PDUS_SIZE]; /*!< \brief Dump of the status PDU                */
} tdsrlc_log_ul_am_nak_pdu_type;

/*! Data structure for logging UL RLC AM NAK PDU information */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_ctl_dl_rlc_id;       /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint8  log_data[TDSMAX_RLC_LOG_PDUS_SIZE]; /*!< \brief Dump of the status PDU                */
} tdsrlc_log_dl_am_nak_pdu_type;

/*! Data structure for logging UL RLC PDU Ciphering information */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_ul_rlc_id;           /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint32 cipher_key_idx;
  tdsrlc_cipher_algo_e_type cipher_algo;
  uint32 count_c;                 /*!< \brief PDU size in octets of each PDU        
                                  Dump of the status PDU                */
} tdsrlc_log_ul_pdu_cipher_info_type;

/*! Data structure for logging DL RLC PDU Ciphering information */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_dl_rlc_id;           /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint32 cipher_key_idx;
  tdsrlc_cipher_algo_e_type cipher_algo;
  uint32 b4_cipher;               /*!< \brief The 1st 4 bytes of the PDU before decipher */
  uint32 count_c;                 /*!< \brief PDU size in octets of each PDU        
                                  Dump of the status PDU                */
} tdsrlc_log_dl_pdu_cipher_info_type;

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
/*!  Data structure for logging DL RLC UM PDU  */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  log_data_dl_rlc_id;      /*!< \brief Data Logical Channel DL RLC ID(0..18) */
  uint8 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_DL_RLC_LOG_PDUS]; /*!< \brief The first four bytes of the pdu       */
} tdsrlc_log_dl_um_pdu_data_type;

/*! Data structure for logging UL UM RLC PDU */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8  log_data_ul_rlc_id;      /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint8 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_UL_RLC_LOG_PDUS]; /*!< \brief The first four bytes of the pdu       */
} tdsrlc_log_ul_um_pdu_data_type;

/*! Adding this structure for dynamic UM PDU logging. we will give the size of this tructure to the 
  allocate memeory on DIAG side.*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */ 
{
  uint8 num_um_entity;
  uint8  log_data_rlc_id;      /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint8 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
}tdsrlc_um_log_header_type ;
#endif

/*!  Adding this structure for dynamic AM PDU logging. we will give the size of this tructure to the 
  allocate memeory on DIAG side.*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  uint8  num_am_entity;
  uint8  log_data_rlc_id;      /*!< \brief Data Logical Channel UL RLC ID(0..18) */
  uint16 num_pdu;                 /*!< \brief Number of PDUs logged                 */
  uint16 pdu_size;                /*!< \brief PDU size in octets of each PDU        */
}rlc_am_log_header_type ;

/*! Logging parameters for RLC PDU logging. RLCLOG_SIG_PDU indicates
  indicates the signaling plane PDUs, RLCLOG_USR _PDU indicates the
  user plane PDUs and RLCLOG_MAX indicates the size of array for storing
  the logging parameters. */
typedef enum
{
    TDSRLCLOG_SIG_PDU = 0,
    TDSRLCLOG_USR_PDU = 1,
    TDSRLCLOG_MAX
}tdsrlc_log_pdu_enum_type;


#define TDSRLCLOG_TYPE_IDX 0
#define TDSRLCLOG_CH_IDX   1


/*! TDSCDMA_UL_RLC_CIPH_CFG_LOG_PACKET */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*! Number of UM and AM uplink logical channels to configure for ciphering. */
  uint8 num_ciphlog_chan_ul;

  /*! Ciphering Information for each UM and AM logical channel. */
  tdsrlc_log_ul_ciphering_type ul_ciphering[TDSUE_MAX_UL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY];
}TDSCDMA_UL_RLC_CIPH_CFG_LOG_PACKET_type;

/*! TDSCDMA_UL_RLC_STATE_LOG_PACKET */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*! Number of Uplink logical channels */
  uint8 num_log_chan_ul;

  /*! State Information for each Uplink logical channel */
  tdsrlc_log_ul_state_type ul_state[TDSUE_MAX_UL_LOGICAL_CHANNEL];                                                 

}TDSCDMA_UL_RLC_STATE_LOG_PACKET_type;

/*! TDSCDMA_UL_RLC_TM_LOG_PACKET */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*! Number of Uplink Transparent Mode logical channels(1..8).  */
  uint8 num_log_chan_tm_ul;

  /*!   Information for each Uplink Transparent Mode logical channel.   */
  tdsrlc_log_ul_tm_type rlc_ul_tm[UE_MAX_UL_TM_CHANNEL];                                                

}TDSCDMA_UL_RLC_TM_LOG_PACKET_type;

/*! 
                   TDSCDMA_UL_RLC_UM_LOG_PACKET
*/

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Uplink Unacknowledged Mode logical channels(1..3).
  */
  uint8 num_log_chan_um_ul;

  /*!
    Information for each Uplink Unacknowledged Mode logical channel. 
  */
  tdsrlc_log_ul_um_type rlc_ul_um[TDSUE_MAX_UL_UM_CHANNEL];                                                

}TDSCDMA_UL_RLC_UM_LOG_PACKET_type;

/*! 
                   TDSCDMA_UL_RLC_AM_LOG_PACKET
*/

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
	/*! Version number */
    uint8 version;
  /*!
    Number of Uplink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_ul;

  /*!
    For each RLC AM Uplink entity 
  */
  tdsrlc_log_ul_am_type rlc_ul_am[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_UL_RLC_AM_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_CIPH_CFG_LOG_PACKET
*/


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of UM and AM downlink logical channels to configure for
    ciphering. 
  */
  uint8 num_ciphlog_chan_dl;

  /*!
    Ciphering Information for each UM and AM downlink logical channel. 
  */
  tdsrlc_log_dl_ciphering_type dl_ciphering[TDSUE_MAX_DL_UM_CHANNEL + TDSUE_MAX_AM_ENTITY];

}TDSCDMA_DL_RLC_CIPH_CFG_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_STATE_LOG_PACKET
 */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Downlink logical channels. 
  */
  uint8 num_log_chan_dl;

  /*!
    State Information for each Downlink logical channel. 
  */
  tdsrlc_log_dl_state_type dl_state[TDSUE_MAX_DL_LOGICAL_CHANNEL];                                                 

}TDSCDMA_DL_RLC_STATE_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_TM_LOG_PACKET
*/

typedef PACK(struct)
{

  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Downlink Transparent Mode logical channels(1..8).
  */
  uint8 num_log_chan_tm_dl;

  /*!
    Information for each Downlink Transparent Mode logical channel. 
  */
  tdsrlc_log_dl_tm_type rlc_dl_tm[UE_MAX_DL_TM_CHANNEL];                                                

}TDSCDMA_DL_RLC_TM_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_UM_LOG_PACKET
 */
typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
	/*! Version number */
  uint8 version;
  /*!
    Number of Downlink Unacknowledged Mode logical channels(1..3).
  */
  uint8 num_log_chan_um_dl;

  /*!
    Information for each Downlink Unacknowledged Mode logical channel. 
  */
  tdsrlc_log_dl_um_type rlc_dl_um[TDSUE_MAX_DL_UM_CHANNEL];                                                

}TDSCDMA_DL_RLC_UM_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_AM_LOG_PACKET
 */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Downlink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_dl;

  /*!
    For each RLC AM Downlink entity. 
  */
  tdsrlc_log_dl_am_type rlc_dl_am[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_DL_RLC_AM_LOG_PACKET_type;

/*! 
                   TDSCDMA_UL_RLC_AM_STAT_LOG_PACKET
 */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Uplink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_ul;

  /*!
    For each RLC AM Uplink entity. 
  */
  tdsrlc_log_ul_am_stat_type     rlc_ul_am_stat[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_UL_RLC_AM_STAT_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_AM_STAT_LOG_PACKET
 */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Downlink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_dl;

  /*!
    For each RLC AM Downlink entity. 
  */
  tdsrlc_log_dl_am_stat_type rlc_dl_am_stat[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_DL_RLC_AM_STAT_LOG_PACKET_type;

/*! 
  Log Packets 0x4144 and 0x414B used for logging RLC PDUs are now obsolete.
  Two new packets have replaced these log packets 
  RLC_AM_USR_PLANE_PDU_LOG_PACKET for logging user plane PDUs
  and RLC_AM_SIG_PLAN_PDU_LOG_PACKET for logging signalling plane RLC
  PDUs.
*/

/*! 
                   TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET
 */


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
    uint8 version;
  /*!
    Number of Uplink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_ul;

  /*!
    For each RLC AM Uplink entity. 
  */
  tdsrlc_log_ul_am_pdu_data_type ul_am_pdu_data[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET_type;

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version;
  /*!
    Number of Uplink Acknowledged Mode entities (1..4).
  */
  uint8 num_um_entity_ul;

  /*!
    For each RLC AM Uplink entity.
  */
  tdsrlc_log_ul_um_pdu_data_type ul_um_pdu_data[TDSUE_MAX_UL_UM_CHANNEL];

}TDSCDMA_UL_RLC_UM_USR_PLANE_PDU_LOG_PACKET_type;
#endif

/*! 
             TDSCDMA_DL_RLC_AM_USR_PLANE_PDU_LOG_PACKET
 */



typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
   uint8 version;
  /*!
    Number of Downlink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_dl;

  /*!
    For each RLC AM Uplink entity. 
  */
  tdsrlc_log_dl_am_pdu_data_type dl_am_pdu_data[TDSUE_MAX_AM_ENTITY];

}TDSCDMA_DL_RLC_AM_USR_PLANE_PDU_LOG_PACKET_type;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;

  /*!
    Number of Downlink Acknowledged Mode entities (1..4).
  */
  uint8 num_am_entity_dl;

  /*!
    For each RLC AM Uplink entity. 
  */
  tdsrlc_log_flex_dl_am_pdu_data_type dl_am_flex_pdu_data[TDSUE_MAX_AM_ENTITY];

  /*!
    For each PDU received - SIZE and DATA information. 
  */
  tdsrlc_log_flex_dl_am_pdu_info_type  pdu_info[TDSMAX_DL_RLC_LOG_PDUS];

}TDSCDMA_DL_FLEXIBLE_RLC_AM_PDU_LOG_PACKET_type;

#endif /* ((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS))  */


#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;
  /*!
    Number of Downlink Acknowledged Mode entities (1..4).
  */
  uint8 num_um_entity_dl;

  /*!
    For each RLC AM Uplink entity.
  */
  tdsrlc_log_dl_um_pdu_data_type dl_um_pdu_data[TDSUE_MAX_DL_UM_CHANNEL];

}TDSCDMA_DL_RLC_UM_USR_PLANE_PDU_LOG_PACKET_type;

#endif
/*! 
                   TDSCDMA_UL_RLC_AM_NAK_PDU_LOG_PACKET
 */


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
   /*! Version number */
  uint8 version_num;
  /*!
    NAK PDU information.
  */
  tdsrlc_log_ul_am_nak_pdu_type ul_am_nak_pdu;

}TDSCDMA_UL_RLC_AM_NAK_PDU_LOG_PACKET_type;

/*! 
                   TDSCDMA_DL_RLC_AM_NAK_PDU_LOG_PACKET
 */


typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;
  /*!
    NAK PDU information.
  */
  tdsrlc_log_dl_am_nak_pdu_type dl_am_nak_pdu;
  
}TDSCDMA_DL_RLC_AM_NAK_PDU_LOG_PACKET_type;

/*! 
              TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET
 */


typedef PACK(struct)
{

  /*!
                   Signalling PDU information
  */
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;

  uint8 num_am_entity_ul;

  tdsrlc_log_ul_am_pdu_data_type ul_am_pdu_data[UE_MAX_SRB_AM_EMTITIES_3];

}TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET_type;

  /*!
               TDSCDMA_DL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET
  */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;
  /*! Signalling PDU information */

  uint8 num_am_entity_dl;
  
  tdsrlc_log_dl_am_pdu_data_type dl_am_pdu_data[UE_MAX_SRB_AM_EMTITIES_3];


 }TDSCDMA_DL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET_type;


/*! TDSCDMA_UL_RLC_PDU_CIPHER_LOG_PACKET */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
      /*! Version number */
  uint8 version_num;
  /*! Number of Uplink Channels.*/
  uint16 num_pdu;

  /*! For each RLC AM Uplink entity. */
  tdsrlc_log_ul_pdu_cipher_info_type  ul_pdu_cipher_info[TDSMAX_CIPHER_PDU];

}TDSCDMA_UL_RLC_PDU_CIPHER_LOG_PACKET_type;

/*! TDSCDMA_DL_RLC_PDU_CIPHER_LOG_PACKET */

typedef PACK(struct)
{
  /*! Log header */
  log_hdr_type   hdr;
  /*! Version number */
  uint8 version_num;
  /*! Number of Downlink Channels. */
  uint16 num_pdu;

  /*! For each RLC AM Uplink entity. */
  tdsrlc_log_dl_pdu_cipher_info_type  dl_pdu_cipher_info[TDSMAX_CIPHER_PDU];

}TDSCDMA_DL_RLC_PDU_CIPHER_LOG_PACKET_type;

/*! 
  Length of Uplink Transparent Mode Configuration Log Packet.
*/
#define TDSRLC_UL_TM_PACKET_LEN(num_tm) ( \
  FPOS(TDSCDMA_UL_RLC_TM_LOG_PACKET_type, rlc_ul_tm)+ \
      (num_tm * sizeof(tdsrlc_log_ul_tm_type)) )
      
/*! 
  Length of Uplink Unacknowledged Mode Configuration Log Packet.
*/
#define TDSRLCLOG_UL_UM_PACKET_LEN(num_um) ( \
  FPOS(TDSCDMA_UL_RLC_UM_LOG_PACKET_type, rlc_ul_um)+ \
      (num_um * sizeof(tdsrlc_log_ul_um_type)) )
      
/*! 
  Length of Uplink Acknowledged Mode Configuration Log Packet.
*/
#define TDSRLCLOG_UL_AM_PACKET_LEN(num_am) ( \
  FPOS(TDSCDMA_UL_RLC_AM_LOG_PACKET_type, rlc_ul_am)+ \
      (num_am * sizeof(tdsrlc_log_ul_am_type)) )

/*! 
  Length of Downlink Transparent Mode Configuration Log Packet.
*/
#define TDSRLCLOG_DL_TM_PACKET_LEN(num_tm) ( \
  FPOS(TDSCDMA_DL_RLC_TM_LOG_PACKET_type, rlc_dl_tm)+ \
      (num_tm * sizeof(tdsrlc_log_dl_tm_type)) )
      
/*! 
  Length of Downlink Unacknowledged Mode Configuration Log Packet.
*/
#define TDSRLCLOG_DL_UM_PACKET_LEN(num_um) ( \
  FPOS(TDSCDMA_DL_RLC_UM_LOG_PACKET_type, rlc_dl_um)+ \
      (num_um * sizeof(tdsrlc_log_dl_um_type)) )
      
/*! 
  Length of Downlink Acknowledged Mode Configuration Log Packet.
*/
#define TDSRLCLOG_DL_AM_PACKET_LEN(num_am) ( \
  FPOS(TDSCDMA_DL_RLC_AM_LOG_PACKET_type, rlc_dl_am)+ \
      (num_am * sizeof(tdsrlc_log_dl_am_type)) )

/*! 
  Length of Uplink State Log Packet.
*/
#define TDSRLC_UL_STATE_PACKET_LEN(num_chan) ( \
  FPOS(TDSCDMA_UL_RLC_STATE_LOG_PACKET_type, ul_state)+ \
      (num_chan * sizeof(tdsrlc_log_ul_state_type)) )

/*! 
  Length of Downlink State Log Packet.
*/
#define TDSRLCLOG_DL_STATE_PACKET_LEN(num_chan) ( \
  FPOS(TDSCDMA_DL_RLC_STATE_LOG_PACKET_type, dl_state)+ \
      (num_chan * sizeof(tdsrlc_log_dl_state_type)) )
      
/*! 
  Length of Downlink AM Statistics packet.
*/
#define TDSRLCLOG_DL_AM_STAT_PACKET_LEN(num_chan) ( \
  FPOS(TDSCDMA_DL_RLC_AM_STAT_LOG_PACKET_type, rlc_dl_am_stat)+ \
      (num_chan * sizeof(tdsrlc_log_dl_am_stat_type)) )

/*! 
  Length of Uplink AM Statistics packet V2.
*/
#define TDSRLCLOG_UL_V2_AM_STAT_PACKET_LEN ( \
  FPOS(TDSCDMA_UL_RLC_AM_STAT_LOG_PACKET_type, rlc_ul_am_stat)+ \
   (TDSUE_MAX_AM_ENTITY * sizeof(tdsrlc_log_ul_am_stat_type)) );
   
/*! 
  Length of UL AM User Plane PDU log packet.
*/
#define TDSRLC_UL_AM_USR_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET_type, ul_am_pdu_data)+ \
      (sizeof(rlc_am_log_header_type) + (num_of_pdu * 4)) )

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
#define TDSRLC_UL_UM_USR_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_UL_RLC_UM_USR_PLANE_PDU_LOG_PACKET_type, ul_um_pdu_data)+ \
      (sizeof(tdsrlc_um_log_header_type) + (num_of_pdu * 4)) )
#endif

/*! 
  Length of UL AM Signaling Plane PDU log packet.
*/
      
#define TDSRLC_UL_AM_SIG_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET_type, ul_am_pdu_data)+ \
      (sizeof(rlc_am_log_header_type) + (num_of_pdu * 4)) )
                                                
/*! 
  Length of DL AM User Plane PDU log packet.
*/
#define TDSRLC_DL_AM_USR_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_DL_RLC_AM_USR_PLANE_PDU_LOG_PACKET_type, dl_am_pdu_data)+ \
      (sizeof(rlc_am_log_header_type) + (num_of_pdu * 4)) )
    
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
#define TDSRLC_DL_UM_USR_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_DL_RLC_UM_USR_PLANE_PDU_LOG_PACKET_type, dl_um_pdu_data)+ \
      (sizeof(tdsrlc_um_log_header_type) + (num_of_pdu * 4)) )
#endif

/*! 
  Length of DL AM Signaling Plane PDU log packet.
*/
#define TDSRLC_DL_AM_SIG_PLANE_PDU_LOG_PACKET_LEN(num_of_pdu) ( \
  FPOS(TDSCDMA_DL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET_type, dl_am_pdu_data)+ \
      (sizeof(rlc_am_log_header_type) + (num_of_pdu * 4)) )
                                                
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*! 
  Length of DL AM User Plane PDU log packet.
*/
#define TDSRLC_DL_AM_FLEXIBLE_PDU_LOG_PACKET_LEN(num_entity, num_of_pdu) ( \
  FPOS(TDSCDMA_DL_FLEXIBLE_RLC_AM_PDU_LOG_PACKET_type, dl_am_flex_pdu_data)+ \
      (sizeof(tdsrlc_log_flex_dl_am_pdu_data_type) * num_entity) +  \
      (sizeof(tdsrlc_log_flex_dl_am_pdu_info_type) * num_of_pdu))
#endif

/*! 
  Length of UL AM NAK PDU log packet.
*/
#define TDSRLC_UL_AM_NAK_PDU_LOG_PACKET_LEN() ( \
  FPOS(TDSCDMA_UL_RLC_AM_NAK_PDU_LOG_PACKET_type, ul_am_nak_pdu)+ \
      sizeof(tdsrlc_log_ul_am_nak_pdu_type) )
                                                
/*! 
  Length of DL AM NAK PDU log packet.
*/
#define TDSRLC_DL_AM_NAK_PDU_LOG_PACKET_LEN() ( \
  FPOS(TDSCDMA_DL_RLC_AM_NAK_PDU_LOG_PACKET_type, dl_am_nak_pdu)+ \
      sizeof(tdsrlc_log_dl_am_nak_pdu_type) )
                                                
/*! 
  Length of Uplink Ciphering Configuration Log Packet.
*/
#define TDSRLC_UL_CIPH_PACKET_LEN(num_chan) ( \
  FPOS(TDSCDMA_UL_RLC_CIPH_CFG_LOG_PACKET_type, ul_ciphering)+ \
      (num_chan * sizeof(tdsrlc_log_ul_ciphering_type)) )
      
/*! 
  Length of Downlink Ciphering Configuration Log Packet.
*/
#define TDSRLC_DL_CIPH_PACKET_LEN(num_chan) ( \
  FPOS(TDSCDMA_DL_RLC_CIPH_CFG_LOG_PACKET_type, dl_ciphering)+ \
      (num_chan * sizeof(tdsrlc_log_dl_ciphering_type)) )
      
#define TDSRLC_UL_PDU_CIPH_INFO_PACKET_LEN( num_pdu) ( \
  FPOS(TDSCDMA_UL_RLC_PDU_CIPHER_LOG_PACKET_type, ul_pdu_cipher_info)+ \
      (num_pdu * sizeof(tdsrlc_log_ul_pdu_cipher_info_type)) )

#define TDSRLC_DL_PDU_CIPH_INFO_PACKET_LEN( num_pdu) ( \
  FPOS(TDSCDMA_DL_RLC_PDU_CIPHER_LOG_PACKET_type, dl_pdu_cipher_info)+ \
      (num_pdu * sizeof(tdsrlc_log_dl_pdu_cipher_info_type)) )

#ifdef T_WINNT
  #error code not present
#endif

#endif
