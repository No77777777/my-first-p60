#ifndef TDSRLCUL_H
#define TDSRLCUL_H
/*===========================================================================
                  U P L I N K  R L C  I N T E R N A L
                      
                         H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  Uplink RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcul.h_v   1.29   09 Jul 2002 17:31:26   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlcul.h#2 $ $DateTime: 2021/02/04 23:39:42 $ $Author: pwbldsvc $


when      who     what, where, why
--------  ---     ---------------------------------------------------------- 
03/30/15  sarao    FR26213 for QSH support 
1/7/14    sarao  Creation and usage of MEDIUM SIZE DSM BUFFER
10/13/14  sarao  CFCM changes for RLC
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "queue.h"
#include "memory.h"
#include "msg.h"
#include "err.h"
#include "amssassert.h"
#include "rex.h"
#include "task.h"
#include "tdsrlcrrcif.h"
#include "tdsrlcdsapif.h"

#ifdef FEATURE_TDSCDMA_HSUPA
  #include "tdsl1macdata.h"
#endif //FEATURE_TDSCDMA_HSUPA

#include "tdsrlci.h"
#include "tdsl2dltask.h"
#include "tdsl2ultask.h"
#include "tdsrlclog.h"
#include "modem_mem.h"
#ifdef FEATURE_QXDM_DSM_TRACING
#include "fc_dsm_trace.h"
#endif

#ifdef FEATURE_TDSCDMA_AM_IGNORE_MAXRST
#include "tdsmacrrcif.h"
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsrlc_qsh_ext.h"
#endif /*FEATURE_QSH_EVENT_METRIC*/




/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

#ifdef FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER
#define TDSCDMA_DSMI_MEDIUM_ITEM_SIZ 628  /* The item size is set ot 5000bits as thats the maximum size of RLC PDU suppported */
#define TDSCDMA_DSMI_DS_MEDIUM_ITEM_CNT_STATIC TDSMAX_UL_PDUS_PER_TTI
#define TDSCDMA_DSM_DS_MEDIUM_ITEM_POOL ((dsm_mempool_id_type)(&tdscdma_dsm_ds_medium_item_pool))
extern dsm_pool_mgmt_table_type tdscdma_dsm_ds_medium_item_pool;
#endif //FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER


/* Avoid RLC reset on PS RB during MRAB call */
#ifdef FEATURE_TDSCDMA_AM_IGNORE_MAXRST
  #define TDS_RLC_UL_AM_IGNORE_MAXRST(ctrl_blk)((ctrl_blk->lc_type == UE_LOGCHAN_DTCH) && (tdsrrc_query_mac_rab_status() == TDSCS_PLUS_PS_RABS) )
#else
  #define TDS_RLC_UL_AM_IGNORE_MAXRST(ctrl_blk) (FALSE)
#endif


/*---------------------------------------------------------------------------
SRB's - 1 TM (srb 0) + 1 UM (srb 1) + 3 AM (srb 2,3,4)
AMR - 3 TM
PS - 2 AM or 2 UM
Video call - 1 TM

UE_MAX_UL_RLC_ENTITY   16  ( 5TM, 3UM, 5AM + 3 buffer)
---------------------------------------------------------------------------*/
#define TDSUE_MAX_UL_RLC_ENTITY  16

/*---------------------------------------------------------------------------
  The user/data plane logical channel ids start from the value that is 
  calculated by RRC as MAX UL TM channels + MAX UL UM channels + 
  MAX AM Signalling channels.
---------------------------------------------------------------------------*/

#define TDSRLC_UL_AM_USR_PLANE_START_LC_ID (UE_MAX_UL_TM_CHANNEL + \
                     TDSUE_MAX_UL_UM_CHANNEL + UE_MAX_SRB_AM_EMTITIES_3)

#define TDSRLC_UL_AM_START_LC_ID (UE_MAX_UL_TM_CHANNEL + \
                               TDSUE_MAX_UL_UM_CHANNEL)

  #define TDSUM_CIPHER_OFFSET 8
  #define TDSAM_CIPHER_OFFSET 16

extern tdsrlci_missing_list_type *tdsrlc_naklist_pool;

extern q_type tdsrlc_ul_free_nak_q;


#define TDSRLC_INVALID_COUNT_C            ~(0) /* Invalid value for count_c  */

/*---------------------------------------------------------------------------
  Length Indicator related definitions
---------------------------------------------------------------------------*/
#define TDSRLC_UL_LI_OCTET_LESS             0xFFF6  /* = (0x7FFB << 1)  */

#define TDSRLC_UL_LI_PADDING_7_BIT          0xFE    /* = (0x7F << 1)    */
#define TDSRLC_UL_LI_PADDING_15_BIT         0xFFFE  /* = (0x7FFFB << 1) */

#define TDSRLC_CHOOSE_LI(li_len,x,y)     ((li_len == TDSRLC_8_BIT_HDR) ? x : y)

/*---------------------------------------------------------------------------
  PDU padding related.
---------------------------------------------------------------------------*/
#define   TDSPDU_PADDING_MASK              0x20000000
#define   TDSSET_PDU_PADDING(x)            ((x) |= TDSPDU_PADDING_MASK)
#define   TDSSET_PDU_PADDING_LEN(x, y)     ((x) |= (((uint32)(y)) << 16))

#define   TDSIO_VEC_LEN_MASK               0x0000FFFF
#define   TDSGET_IO_VEC_LEN(x)             ((x) & TDSIO_VEC_LEN_MASK)

/*---------------------------------------------------------------------------
  Common definitions
---------------------------------------------------------------------------*/
#define   TDSMAX_IO_VEC_PER_PDU             20
#define   TDSMAX_HDR_BUFF_SIZE             404

/* The Max SDUs that are possible per TTI */
#define   TDSMAX_SDU_PER_TTI                 100
#define   TDSMAX_SDU_PER_TTI_EDCH            200
#define   TDSMAX_SDU_PER_TTI_NON_EDCH        100

/*---------------------------------------------------------------------------
  Maximum no. of DSM items for building control PDUs
  among all AM logical channels in a particular TTI.
---------------------------------------------------------------------------*/
#define   TDSMAX_CTRL_DSM_ITEMS              4

#define   TDSMAX_UL_PDUS_PER_TTI            26  /* Maximum no. of UL PDUs per TTI in R99 path */
#define   TDSRLC_SI_HDR_SIZE_BYTES           3
#define   TDSRLC_TSN_HDR_SIZE_BYTES          4

/*---------------------------------------------------------------------------
  7200A specific:
  io.length(31)   : TTI_LAST_IO_VEC  
  io.length(30)   : PDU_LAST_IO_VEC
  io.length(29)   : PDU_PADDING       --Valid only if io.length(30) = 1
  io.length(28)   : Reserved
  io.length(27-16): padding length    --Valid only if io.length(29) = 1
  io.length(15-0): data length
---------------------------------------------------------------------------*/
#define   TDSTTI_LAST_IO_VEC_MASK          0x80000000
#define   TDSSET_TTI_LAST_IO_VEC(x)        ((x) |= TDSTTI_LAST_IO_VEC_MASK)

#define   TDSPDU_LAST_IO_VEC_MASK          0x40000000
#define   TDSSET_PDU_LAST_IO_VEC(x)        ((x) |= TDSPDU_LAST_IO_VEC_MASK)


/* Enum for the RLC headers. includes UM, AM, 7/15 bit LI headers */
typedef enum
{
  TDSRLC_8_BIT_HDR  = 0x1,
  TDSRLC_16_BIT_HDR = 0x2
}tdsrlc_hdr_e_type;

typedef enum
{
  TDSRLC_AM_CONTROL_PDU   = 0x0,
  TDSRLC_AM_NEW_DATA_PDU  = 0x1,
  TDSRLC_AM_RETX_DATA_PDU = 0x2
}tdsrlc_am_pdu_e_type;

typedef enum
{
  TDSRLC_INVALID_CHNL    = 0x0,
  TDSRLC_NON_E_DCH_CHNL  = 0x1,
  TDSRLC_E_DCH_CHNL      = 0x2
}tdsrlc_ul_chnl_e_type;


typedef enum {
  TDSNO_SDU  = 0x00,
  TDSS_SDU   = 0x01,
  TDSE_SDU   = 0x02,
  TDSMID_SDU = 0x03,
  TDSENS_SDU = 0x04,
  TDSFIT_SDU = 0x05
} tdsrlci_sdu_pdu_seg_e_type;

typedef enum {
  TDSNONE_CLOSE = 0x00,
  TDSAPP_CLOSE = 0x01,
  TDSRX_TX_CLOSE = 0x02,
  TDSHALF_CLOSE_WIN = 0x80
} tdsrlci_close_rx_win_e_type;

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
typedef enum {
  TDSPEER_TX_DONT_CARE = 0x00,
  TDSPEER_TX_ONE_WIN,
  TDSPEER_TX_HALF_WIN, 
  TDSPEER_TX_FULL_WIN 
} tdsrlci_peer_tx_win_size_e_type;
#else
typedef enum {
  TDSDONT_CARE = 0x00,
  TDSFULL_WIN = 0x01,
  TDSHALF_WIN = 0x02
} tdsrlci_open_rx_win_e_type;
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

/*---------------------------------------------------------------------------
  Enumerated type that defines the RLC Uplink Logical Channel states.
---------------------------------------------------------------------------*/
typedef enum {
  TDSUL_NULL_STATE = 0,                    /* Logical channel is currently not*/
                                        /* in use.                         */
  TDSUL_DATA_TRANSFER_READY,               /* Logical channel has been        */
                                        /* established, and data transfer  */
                                        /* is allowed.                     */
  TDSUL_WAITING_TO_SUSPEND,                /* RRC has sent a suspend request  */
                                        /* with N value greater than 0, and*/
                                        /* RLC has not yet transmitted     */
                                        /* SN + N PDUs. Only applicable to */
                                        /* UM and AM channels.             */
  TDSUL_SUSPENDED = 0x04,                  /* Data transfer on the logical    */
                                        /* channel has been temporarily    */
                                        /* suspended. Only applicable to UM*/
                                        /* and AM channels.                */
  TDSUL_RESET_PENDING = 0x08,              /* RESET has been sent to the peer */
                                        /* entity, and no data transfer is */
                                        /* allowed until a RESET_ACK is    */
                                        /* received. Only applicable to AM */
                                        /* channels.                       */
  TDSUL_CONFIG_PENDING = 0x10,             /* Waiting for the DL side to      */
                                        /* complete its config             */
  TDSUL_STOP = 0x20                        /* Stop all of the transmission    */ 
} tdsrlci_ul_state_e_type;

/*--------------------------------------------------------------------------- 
  Typedef for storing parameters specific to transparent mode.
---------------------------------------------------------------------------*/
typedef struct {
  boolean seg_ind;                      /* Segmentation Indication - TRUE  */ 
                                        /* if segmentation is on, FALSE    */
                                        /* otherwise.                      */
  boolean act_incl;                     /* Activation Time - True if       */
                                        /* included, False Otherwise       */
  uint8   act_time;                     /* Activation Time.                */
  rlc_lc_id_type rlc_id;                /* RLC logical channel ID.         */
  uint16  timer_discard;                /* Timer Discard value in ms.      */
#ifdef FEATURE_TM_LB
  tdsrlc_loop_back_mode_e_type lpm;
#endif /* FEATURE_TM_LB */
} tdsrlci_ul_tm_ctrl_type;

/*--------------------------------------------------------------------------- 
  Typedef for storing info of UM PDU logging
---------------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
typedef struct{
  uint16 num_pdu;                 /* Number of PDUs to be built every TTI  */
  uint16 pdu_size;                /* PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_UL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
} tdsrlc_ul_chan_um_pdu_type;
#endif

/*--------------------------------------------------------------------------- 
  Typedef for storing parameters specific to unacknowledged mode.
---------------------------------------------------------------------------*/
typedef struct {
  boolean act_incl;                     /* Activation Time - True if       */
                                        /* included, False Otherwise       */
  uint8   act_time;                     /* Activation Time.                */
  uint16  timer_discard;                /* Timer Discard value in ms       */
  //uint8   rlc_sn;                       /* RLC Sequence Number             */
  uint8   li_length;                    /* Length of the length indicator  */
                                        /* - 7 or 15 bits                  */
  uint8   vt_us;                        /* Sequence Number of the next in- */
                                        /* Sequence Pdu to be transmitted. */
  uint8   n_to_suspend;                 /* Number of PDUs can be sent in   */
                                        /* waiting to suspend state.       */
  boolean sn_wrap;                      /* SN wraparound?                  */
  rlc_lc_id_type rlc_id;                /* RLC logical channel ID.         */

  boolean alternate_e_bit;              /* TRUE - Alternate E-bit interpretation */
                                        /* FALSE- Normal E-bit interpretation    */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
  tdsrlc_ul_chan_um_pdu_type  ul_chan_um_pdu;   /* Stores the UM PDU information */
#endif
#endif
} tdsrlci_ul_um_ctrl_type;

#define TDSRLC_TXQ_SIZE        2048      /* Max. PDUs can be tx'd w/o ACK   */
#define TDSRLC_TXQ_SIZE_MASK   0x7FF 

#define TDSMAX_MUI   256          /* Message Unit ID                 */

#define TDSMAX_RLC_PDU_SIZE   1024          /* As the current DSM item for R4 channel is 128 bytes * 8 = 1024bits  */

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
typedef struct {

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  uint16  rlc_fc_ws;
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
} tdsrlc_fc_ws_type;
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

#ifdef FEATURE_QSH_EVENT_METRIC
typedef struct{
uint8* start_address;
qsh_metric_id_t rlc_qsh_metric_id;
qsh_client_metric_context_id_t rlc_qsh_metric_cntxt_id;
} tdsrlc_qsh_metric_address_type;

typedef enum{
	TDSRLC_QSH_METRICS_DISABLED = 0,
	TDSRLC_QSH_METRICS_ENABLED = 1,	
}tdsrlc_qsh_status_e_type;
#endif


/*--------------------------------------------------------------------------- 
  Used to store the necessary up-link control state variables 
  for an RLC AM entity.
---------------------------------------------------------------------------*/
typedef struct {
  uint16  vt_s;                         /* VT(S) - Send State              */
  uint16  vt_a;                         /* VT(A) - Acked State             */
  uint16  vt_ms;                        /* VT(MS) - VT(A) + Tx_Window_size */
  uint16  vt_wins;                      /* Tx_Window_size                  */
  uint16  peer_wins;                    /* Tx_Window_Size to peer side     */
  boolean tx_wins;                      /* Send Tx_Window_Size SUFI?       */
  uint8   vt_pu;                        /* VT(PU) - Poll_PU                */
  uint8   vt_sdu;                       /* VT(SDU) - Poll_SDU              */
  uint8   vt_rst;                       /* VT(RST) - # of RESET sent       */
  uint8   reset_sn;                     /* Sequence number for RESET PDU.  */
                                        /* Initial to 0                    */
  uint8   reset_ack_sn;                 /* Sequence number for RESET_ACK   */
                                        /* PDU Initial to 0                */
  uint16  poll_proh_cnt;
  uint16  poll_peri_cnt;
  boolean poll_prohibit;                /* timer_poll_prohibit on or off   */
  boolean poll_next;                    /* Set polling bit in the next     */
                                        /* outgoing frame                  */
  uint16  poll_window_size;             /* poll window size used for       */
                                        /* tx poll window % calculation    */
  uint8   timer_poll_dat;               /* Max number of polling rerty     */
  boolean timer_poll;                   /* Truned on/off timer polling     */
  uint16  timer_poll_sn;                /* SN for timer polling            */
  uint16  timer_poll_cnt;               /* Counter for timer polling       */
  boolean owe_reset;                    /* Need to send a Reset PDU        */
  boolean need_reset_ack;               /* In need for RESET ACK?          */
  uint16  reset_timer_cnt;              /* Timer counter for RESET PDU retx*/
  boolean owe_reset_ack;                /* Need to send a Reset_ACK PDU    */
  boolean status_rpt;                   /* Being allowed to report status  */
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA   
  boolean status_opt_enalbed;           /* If the optimization of the delay of status report after TA is enalbed or not */
  uint16  prev_ack_sn;                  /* prev_ack_sn - Store the previous ack sn value */
  uint32  status_rpt_time;              /* The time of trigger point of status report */
  uint32  status_prohibit_time_reduce;  /* The status prohibit time after reduce */
#endif  
  boolean ack_pdu;                      /* ACK received PDU in downlink.   */
  boolean tx_mrw;                       /* If TRUE, send the first MRW in  */
                                        /* mrw_q                           */
  uint16  mrw_timer_cnt;
  uint16  q_mrw_sn;                     /* Latest queued MRW's SN          */
  uint32  retx_size;                    /* number of bytes to be reTX.     */
  uint32  retx_size_reduced;            /* used when there is a TX window reduction
                                           indicates, number of bytes (within Tx window) 
                                           to be reTX */
  tdsrlci_close_rx_win_e_type close_rx_win;/* APP_CLOSE_RX_WIN,               */
                                        /* RLC_CLOSE_RX_WIN                */
  uint8   li_length;                    /* Length of the length indicator  */
                                        /* - 7 or 15 bits */
  uint8   n_to_suspend;                 /* Number of PDUs can be sent in   */
                                        /* waiting to suspend state.       */
  boolean need_mrw_ack;                 /* In need for MRW ACK?            */
  boolean nw_only_init_reset;           /* TRUE - UE rxd RESET PDU.        */
                                        /* FALSE- When duplicate Reset PDU */
                                        /*        is received.             */
                                        /*      - Reset PDU is rxd while   */
                                        /*        onging UE initiated Reset*/
                                        /*        procedure.               */
  boolean duplicate_reset_pdu_rx;       /* TRUE - UE rxd duplicate RESET   */
  uint32  txd_reset_ack_hfn;            /* To store previous hfni value, used
                                           to handle duplicate resets      */
                                        
  uint16  retx_win_cnt;                 /* Timer count for retx win SUFI   */
  boolean retx_win_size;
  byte    n_retx_win;
  boolean retx_q_sema;                  /* retx q access semaphore         */
  boolean mrw_q_sema;                   /* mrw q access semaphore         */
  boolean internal_q_sema;              /* mrw q access semaphore         */
  boolean mrw_ack_q_sema;               /* mrw ack q access semaphore      */  
  uint8   backup_timer_cnt;             /* backup timer                    */
  boolean poll_delay;                   /* delay the polling to next PDU   */
  boolean fst_tx_pdu;                   /* TRUE, if the next going out PDU */
                                        /* is the 1st tx PDU.              */
  boolean reset_fail;                   /* indicates reset failure on a 
                                           logical channel                 */
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  tdsrlc_fc_ws_type rlc_fc_ws;             /* Peer window size requests from
                                           all RLC flow controls.          */
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
  uint8   dl_cpu_base_fc_f;                /* Flag of indication of performing*/
                                        /* CPU base flow control.          */
  uint8   ul_cpu_base_fc_f;                /* Flag of indication of performing*/
                                        /* CPU base flow control.          */
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
} tdsrlci_ul_am_state_type;

/*--------------------------------------------------------------------------- 
  Used to store the mapping information between the SDUs and PDUs. When the 
  ACK for a PDU is received ( and the SDUs being put in this PDU are required 
  to report to the upper layer), the RLC uses this table to obtain the MUIs 
  for these SDUs and the information about other segments of the same SDU in 
  another PDUs gotten ACKed as well. The individual SDU information needed to 
  be reported to the upper layer is stored in another table. 
  The validity of the SDU confirmation callback function is checked every time 
  to determine whether to look at this mapping information when an ACK for a 
  PDU is received from the peer entity.
---------------------------------------------------------------------------*/
typedef struct {
  uint8 mui;                     /* starting SDU sequence number           */
  uint8 distance;                /* number of consecutive SDUs in this PDU */
  tdsrlci_sdu_pdu_seg_e_type sdu_seg;
                                 /* SDU segmentation infor in this PDU.    */
                                 /* The value of this variable stands for: */
                                 /* 0: NO SDU. or only padding exist       */
                                 /* 1: Contains only start of an SDU.      */
                                 /* 2: Contains only end of an SDU.        */
                                 /* 3: Contains only the middle of an SDU. */
                                 /* 4: Contains the end of an SDU and the  */
                                 /*    start of another SDU.               */
                                 /* 5: All SDUs in this PDU are fit in.    */
  
  uint8 num_ack;/* number of the ACKs received.                            */
                /* 0 : not receiving Ack yet.                              */
                /* 1 : either the ACK being received for this PDU or the   */
                /*     previous PDU containing the part of the same SDU    */
                /*     in this PDU got ACKed.                              */
                /* 2 : both this PDU and previous PDU got acked.           */
                /* When the ACK for this PDU is received, the num_ack is   */
                /* incrementing by one. For an SDU being segmented into    */
                /* several segments, and each segment being fit into one   */
                /* PDU, except the first segment, each segment needs two   */
                /* acks to show this PDU/SDU segment and the previous      */
                /* PDU/SDU segment being ACKed. When the num_ack reaches   */
                /* the required value for this PDU (1 for the first        */
                /* segment and 2 for the reset), it continues to look at   */
                /* the next PDU to update the num_ack. (i.e. Adds one to   */
                /* it. When the last segment of the SDU reaches, and the   */
                /* num_ack is 2, the callback function to report this      */
                /* SDU to the upper layer is called, if necessary.         */
  boolean exact_fit;              /* One byte less or exactly fit w/o LI   */
  uint8   rpt_cnt;    /* number of SDUs being reported to the upper layer */
} tdsrlci_am_sdu_pdu_map_type;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*---------------------------------------------------------------------------
  Data Structure for logging RLC UL statistics for acknowledged mode.
---------------------------------------------------------------------------*/

typedef struct
{
  uint32 tot_num_pdu_byte_txd;    /* Total number of bytes transmitted     */
                                  /* Includes teh ctrl & data pdus         */
  uint32 tot_num_sdu_byte_txd;    /* Total number of sdu bytes rxd from    */
                                  /* upper layer                           */
  uint16 tot_num_new_data_pdu_txd;/* total number of data PDUs transmitted */
  uint16 tot_num_data_pdu_retxd;  /* Total number of PDUs retransmitted    */
  uint16 tot_num_pdu_nacked;      /* Total number of pdus naked            */
  uint16 tot_num_ctrl_pdu_txd;    /* Total number of control PDUs          */
  uint16 tot_num_rlc_resets;      /* Total number of rlc resets            */
  uint16 ul_am_buf_status;        /* Stores the uplink buffer status value */
  uint16 tot_num_pdus_with_two_or_more_retrans; /* Total number of dat   */
  uint16 tot_num_pdus_with_three_or_more_retrans; /* count for pdus which  */
  uint16 tot_num_pdus_with_four_or_more_retrans;  /* have more than 2 dat  */
  uint16 tot_num_pdus_with_five_or_more_retrans;  /* counts. Number of PDUs*/
  uint16 tot_num_pdus_with_six_or_more_retrans;   /* with >2, >3, >4, >5   */
} tdsrlc_ul_chan_am_stat_type;                       /* or >=6 dat counts     */

/*--------------------------------------------------------------------------
Data structures for logging the first 4 bytes of the PDU by QCAT.
---------------------------------------------------------------------------*/

typedef struct{
  uint16 num_pdu;                 /* Number of PDUs to be built every TTI  */
  uint16 pdu_size;                /* PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_UL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
} tdsrlc_ul_chan_am_pdu_type;

/*--------------------------------------------------------------------------
Data structures for logging the UL AM&UM PDU ciphering parameters
---------------------------------------------------------------------------*/

typedef struct{
  uint8  log_ul_rlc_id;           /* Data Logical Channel UL RLC ID(0..18) */
  uint32 cipher_key_idx;
  uecomdef_umts_cipher_algo_e_type  cipher_algo;
  uint32 count_c;                 /* PDU size in octets of each PDU        */
                                  /* Dump of the status PDU                */
} tdsrlc_ul_cipher_log_data_type;

/*--------------------------------------------------------------------------
Data structures for logging the UL AM&UM PDUs ciphering parameters
---------------------------------------------------------------------------*/
typedef struct{
  uint16 num_pdu;                  /* Number of PDUs to be built every TTI  */
  boolean ul_cipher_logging ; /* TRUE if cipher logging in progress in TaskC */
  tdsrlc_ul_cipher_log_data_type cipher_log_data[TDSMAX_CIPHER_PDU];
                                  /* The first four bytes of the pdu       */
} tdsrlc_ul_pdu_cipher_info_type;


#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*--------------------------------------------------------------------------- 
  Used as an entry in the transmit queue to store the transmitted RLC frame 
  for the later retransmit requirement by the peer side. The DSM item 
  contains the completely formatted RLC frame, excluded the piggybacked 
  status PDUs.
---------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------- 
  Contains all the headers & LI's for a particular PDU.
---------------------------------------------------------------------------*/
typedef struct
{
  uint8   *buff;
  uint16   num_bytes;
}tdsrlc_buff_type;

/*--------------------------------------------------------------------------- 
  Contains IO vectors corresponding to a particular PDU.
---------------------------------------------------------------------------*/
typedef struct
{
  uint8* ptr;
  uint32 len;
}tdsrlc_io_vec_type;

typedef struct
{
  tdsrlc_io_vec_type io_vec[TDSMAX_IO_VEC_PER_PDU];
  uint8           num_io_vecs;
}tdsrlc_io_vec_list_type;

/*--------------------------------------------------------------------------- 
  The headers i.e. MAC-e, MAC-es, RLC, LIs are stored in the Header Buffer
  while building the PDUs. All these headers are copied into the ERAM bank
  header section by L1. Each header is considered as a separate data unit.
  MAC-e, MAC-es data units are byte aligned. RLC headers, LIs need be word
  aligned.
---------------------------------------------------------------------------*/
typedef struct
{
  uint32 num_valid_words;         /* No. of words taken for headers.       */
  
  uint32 buff[TDSMAX_HDR_BUFF_SIZE]; /* Store MAC-e, MAC-es, RLC headers, LIs */
}tdsrlc_ul_hdr_buff_type;

typedef dsm_item_type * tdsfree_sdu_buffer_type;

typedef struct
{

  tdsfree_sdu_buffer_type *sdu_buffer_ptr;    /* Pointer to the SDU that needs*/
                                           /* to be freed.                 */
  uint8 num_sdu;                           /* No. of SDUs to be freed.     */

} tdsrlc_um_sdu_q_type;

typedef struct
{
  dsm_item_type *sdu_ptr;     /* Contains pointer to the 1st SDU whose     */
                              /* data goes as payload with this PDU        */
                              
  uint16 offset;              /* Offset with in the 1st SDU                */
                              
  uint16 payload_length;      /* Length of PDU payload excluding padding   */
                              /* and RLC Header - 2bytes AM                */
  
  uint16 ctrl_info;           /* bit15 - bit11 --> RESERVED                */

                              /* bit10 --> 1 - HE is set in this PDU       */
                              
                              /* bit9 --> 1 - LI=7ffb present in next PDU. */
                              /*            - This is used while SDU ackcnf*/
  
                              /* bit8 --> 1 - Last octet of SDU is last the*/
                              /*              octet of PDU and LI fits in  */
                              /*              this PDU itself              */
                              
                              /* bit7 --> 1 - LI=0 present in next PDU.This*/
                              /*            - is used during SDU ack cnf.  */
                              
                              /* bit6 --> 1 - LI=0 present in this PDU     */
                              
                              /* bit5 --> 1 - LI=0x7ffb present in this PDU*/

                              /* bit4 --> 1 - PDU contains padding         */

                              /* bit3-bit0--> No. of SDUs ended in this PDU*/
                              /* Note: This information will be used when  */
                              /* L2-Ack is received from NW.This will tell */
                              /* how many SDUs need to be freed. If LI=0/  */
                              /* 0x7ffb needs to be sent in next PDU then  */
                              /* num_sdus need to be incremented in the    */
                              /* current PDU information.                  */
}tdsrlc_am_pdu_info_type;

typedef struct 
{
  tdsrlc_am_pdu_info_type pdu_info; /* New Book Keeping Information           */
  uint8  vt_dat_cur;                      /*bit(6..0)-No.of times a PDU txd*/
                                          /* bit(7) = 1 - PDU NAK'd        */
                                          /* bit(7) = 0 - Not NAK'd        */

}tdsrlci_txq_element_type;


/*--------------------------------------------------------------------------
  Used to store the AM transmission queue and a boolean to indicate if 
  the AM transmission queue is in use for a particular AM channel
--------------------------------------------------------------------------*/

typedef struct
{
  boolean in_use;                         /* indicates if the transmission
                                             queue for a particular AM 
                                             channel is in use             */
  tdsrlci_txq_element_type *am_txq;
                                          /* AM transmission queue has RLC
                                             TXQ_SIZE as 2047 right now, 
                                             Memory allocation for this Q is 
                                             done during LC establishment.  */
} tdsrlci_am_txq_type;



/*--------------------------------------------------------------------------- 
 Definition for R99 PDU dsm pool.
---------------------------------------------------------------------------*/
typedef struct
{
  /* DSM item used to build data PDU in this TTI.  */
  dsm_item_type *item_ptr[TDSMAX_UL_PDUS_PER_TTI];

  /* No. of DSM items used. */
  uint8         num_item;
}tdsrlc_pdu_dsm_type;

/*--------------------------------------------------------------------------- 
  Definition for ctrl pdu dsm pool
---------------------------------------------------------------------------*/
typedef struct
{
  dsm_item_type *item_ptr;                /* Statically allocated DSM item */
                                          /* which is used for building the*/
                                          /* contrl PDUs in this TTI       */
  uint16        num_ctrl_bits;            /* No. of ctrl_pdu bits built    */
}tdsrlc_ctrl_pdu_type;

typedef struct
{
  tdsrlc_ctrl_pdu_type ctrl_pdu[TDSMAX_CTRL_DSM_ITEMS];
  
  /* No. of DSM items used. */
  uint8         num_item;
}tdsrlc_am_ctrl_dsm_type;


/*--------------------------------------------------------------------------- 
  Used to store the necessary up-link control information for an RLC AM 
  entity.
---------------------------------------------------------------------------*/
typedef struct {
  tdsrlc_am_common_config_type  am_common_local;   /* Local copy common config variables.      */ 
  tdsrlc_ul_am_config_type  ul_cnfg_var_local;              /* config information from RRC.  */
  tdsrlc_am_common_config_type *am_common;   /* ptr to common config variables.      */ 
  tdsrlc_ul_am_config_type *ul_cnfg_var;     /* pointer to config information from RRC.  */
  tdsrlci_ul_am_state_type ul_state;         /* Control state variables.      */

  tdsrlc_tmr_status_type ul_tmr_sts;         /* holds status of RLC AM timer */

  rex_timer_type tmr_discard_handle;      /* RLC timer for timer based SDU */
                                          /* discard                       */
  uint16 tmr_discard_sequence_num;        /* Sequence number for which     */
                                          /* discard timer is running      */

  rex_timer_type* tmr_cellpch_ptr;        /* Pointer Rex timer Structure   */
  tdsrlc_opt_status_type    ul_status_opt_bitmap; /*UL rtx status bitmap array   */
  uint16                 ul_num_sn_li;    /*number of SN-LI pairs to be formed*/
                                          /*on UL, basically indicates status BO*/
                                          /*on UL in terms of number of SN-LI pairs*/
  q_type mrw_q;                           /* MRW queue                     */
  q_type mrw_ack_q;                       /* MRW ACK queue                 */
                                               
  q_type internal_q;                      /* Contains SDUs that are tx and */
                                          /* their corresponding PDUs are  */
                                          /* yet to be acknowledged.       */
  boolean  sdu_cnf_pending;               /* TRUE - if sdu cnf is delayed  */
                                          /* because LI_exact/LI_octet_less*/
                                          /* present in next pdu, but ack'd*/
                                          /* till this pdu. FALSE-otherwise*/
#ifndef FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE
  uint8    sdu_mui;                       /* MUI if sdu cnf delayed        */
#endif  

  void (*sdu_cnf_cb) (rlc_lc_id_type ul_data_lc_id, 
                      uecomdef_status_e_type status,
                      uint8 mui,
                      rlc_sdu_tx_fail_reason_e_type failure_reason
                      );                  /* SDU confirm callback          */
                                          /* function.                     */

  tdsrlci_am_txq_type *am_txq_ptr;           /* AM transmission queue pointer */

boolean sdu_start_seg_ack;  /* 1 - pdu containing start segment of this SDU*/
                            /* is ack'd, but remaining pdus containing this*/
                            /* SDU segments are yet to be transmitted.     */
                            /* 0 - other wise                              */
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
          
  tdsrlc_ul_chan_am_stat_type ul_chan_am_stat; /* Stores the Am statistics    */
                                            /* info.                       */  
  tdsrlc_ul_chan_am_pdu_type  ul_chan_am_pdu;   /* Stores the PDU information */
                                             /* for a single AM entity     */
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

  tdsmulticlient_fc_arbitarator_type multiclient_dl_fc_arbitarator;
  tdsmulticlient_fc_arbitarator_type multiclient_ul_fc_arbitarator;


} tdsrlci_ul_am_ctrl_type;



/*--------------------------------------------------------------------------- 
  Typedef for storing parameters specific to transparent mode.
---------------------------------------------------------------------------*/
typedef union {
  tdsrlci_ul_tm_ctrl_type tm_ctrl;        /* Transparent Mode parameters.     */
  tdsrlci_ul_um_ctrl_type um_ctrl;        /* Unacknowledged Mode parameters.  */
  tdsrlci_ul_am_ctrl_type am_ctrl;        /* Acknowledged Mode parameters.    */
} tdsrlci_ul_ctrl_u_type;


/*---------------------------------------------------------------------------
  Used to store information about a uplink logical channel. 
---------------------------------------------------------------------------*/
typedef  struct {
  uecomdef_logchan_e_type lc_type;      /* Uplink logical channel type     */
  uecomdef_logch_mode_e_type lc_mode;   /* RLC Mode                        */
  tdsrlci_ul_state_e_type lc_state;        /* Uplink Logical channel state    */
  tdsrlci_ciphering_type cipher;           /* Ciphering Parameters            */

  tdsrlci_ciphering_type cipher_backup;    /* Backup for ciphering parameters
                                           while applying the new one      */

  tdsrlci_ul_ctrl_u_type ctrl;             /* Union of TM, UM and AM specific */
                                        /* parameters.                     */
  rex_timer_type* tmr_discard_ptr;      /* Pointer Rex timer Structure     */
                                        /* for Timer discard using.        */
  dsm_watermark_type *ul_wm_ptr;        /* Pointer to the associated RLC   */
                                        /* uplink data queue               */ 
  dsm_item_type *cur_sdu_ptr;           /* Point to the starting of data   */
                                        /* on current SDU DSM chain        */ 
  uint16  cur_sdu_len;                  /* Size of the current SDU         */
  uint16  cur_sdu_tx_len;               /* Bytes txd from the current SDU  */

  boolean wm_in_rach;                   /* TRUE: the channel is in RACH.   */
                                        /* Register non_empty_func ptr when*/
                                        /* the WM is registered.           */
  boolean special_ind;                  /* Boolean to indicate if          */
                                        /* the first length indicator value*/
                                        /* of the next PDU should be a     */
                                        /* special indicator (value 0x00). */
  boolean octet_less;                   /* Boolean to indicate if the last */
                                        /* PDU was one octet short of      */
                                        /* filling in the LI.              */
  boolean padding;                      /* Boolean to indicate if the rest */
                                        /* of the PDU(s) in the TTI should */
                                        /* have padding bits.              */
  boolean purge_wm;                     /*Boolean to indicate of WM is being
                                          purged, if TRUE, zero BO is 
                                          reported for that logical channel*/
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  boolean ul_pdu_logging;     /* boolean to indicate if set to TRUE  then 
                                                    dont fill the logging structure in ctr block*/
#endif

} tdsrlci_ul_ctrl_blk_type;

 
/*---------------------------------------------------------------------------
  Database that is used to store information about all the uplink 
  logical channels. This database should be initialized when the uplink 
  RLC task is started. The maximum number of entries in the table is 21.
---------------------------------------------------------------------------*/

typedef struct {
  uint8 active_ch;                      /* Number of active uplink logical */
                                        /* channels.                       */
  tdsrlci_ul_ctrl_blk_type ctrl_blk[TDSUE_MAX_UL_RLC_ENTITY];
                                        /* Data structure used to store all*/ 
                                        /* the information for the uplink  */
                                        /* logical channel.                */
} tdsrlci_uplink_table_type;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*---------------------------------------------------------------------------
  The following two structures are used only for QXDM logging.
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  Data Structure for RLC UL states for Transparent, Unacknowledged and 
  Acknowledged Modes.
---------------------------------------------------------------------------*/
typedef struct
{
  uint8    rlc_id;              /* UL logical channel id (0..18)           */
  uint8    lc_state;            /* State of the RLC State Machine          */
                                /*   0 - RLC_UL_NULL_STATE                 */
                                /*   1 - TM_UL_DATA_TRANSFER_READY         */
                                /*   2 - UM_UL_DATA_TRANSFER_READY         */
                                /*   3 - UM_UL_WAITING_TO_SUSPEND          */
                                /*   4 - UM_UL_SUSPENDED                   */
                                /*   5 - AM_UL_DATA_TRANSFER_READY         */
                                /*   6 - AM_UL_WAITING_TO_SUSPEND          */
                                /*   7 - AM_UL_SUSPENDED                   */
                                /*   8 - AM_UL_RESET_PENDING               */
                                /*   9 - AM_UL_CONFIG_PENDING              */
} tdsrlc_ul_chan_state_type;

typedef struct
{
  uint8 nchan;                  /* Number of Uplink logical channels.      */
  tdsrlc_ul_chan_state_type ul_state[TDSUE_MAX_UL_LOGICAL_CHANNEL];                                                 
                                /* State Information for each Uplink       */
                                /* logical channel.                        */

}tdsrlc_ul_log_state_type;

#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

/*---------------------------------------------------------------------------
  Table to store Uplink SDU tx conformation.
---------------------------------------------------------------------------*/
typedef struct 
{
  rlc_lc_id_type lc_id;         /* AM UL logical channel id                */
  uint8 mui;                    /* SDU discarded whose failure             */
                                /* confirmation is pending                 */
}tdsrlci_ul_am_sdu_tx_cnf_type ;


/*-------------------------------------------------------------------------
  These queues hold the UM SDUs transmitted in this TTI for non-EDCH and 
  EDCH channels respectively. SDUs from these queues are freed during next 
  TTI build request for the corresponding channel.
-------------------------------------------------------------------------*/   
extern tdsrlc_um_sdu_q_type tdsrlc_free_sdu_q_non_edch;
extern tdsrlc_um_sdu_q_type tdsrlc_free_sdu_q_edch;

extern tdsfree_sdu_buffer_type tdsfree_sdu_buffer_edch [TDSMAX_SDU_PER_TTI_EDCH];
extern tdsfree_sdu_buffer_type tdsfree_sdu_buffer_non_edch [TDSMAX_SDU_PER_TTI_NON_EDCH];
extern tdsrlc_am_ctrl_dsm_type tdsctrl_pdu_dsm_pool;
extern tdsrlc_pdu_dsm_type 	 tdsrlc_pdu_dsm_pool; 


/* rlc_id to ul control block index table */
extern uint8 tdsul_ctl_blk_id_tbl[TDSUE_MAX_UL_LOGICAL_CHANNEL];

/*===========================================================================

FUNCTION RLCI_UL_AM_BASE_TIMER_HANDLER

DESCRIPTION
  Handle 10ms Base Timer timed out.
  In Uplink, there is only one base timer. All timers related in the Uplink
  are done in the form of counters. Every time the 10ms base timer expired,
  this function looks into which timer counters are needed to be increased
  by one and then compares w/ the real configured timer related to the
  individual counters for timer expiration. If the real timer expired, a
  related flag will be raised.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_am_base_timer_handler (void);

typedef enum
{
  TDSRLC_FC_CMD_NULL_ENTITY,
  TDSRLC_FC_CMD_LARGE_DSM_ENTITY,
  TDSRLC_FC_CMD_SMALL_DSM_ENTITY,
  TDSRLC_FC_CMD_DUP_ENTITY
} tdsrlc_fc_cmd_entity_type;

typedef struct 
{
  q_link_type               link;
  tdsrlc_fc_cmd_entity_type    entity;
  uint16                    cmd;
}tdsrlc_fc_cmd_type;

#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*===========================================================================

FUNCTION RLC_UL_CRIT_SECT_INIT

DESCRIPTION
  Initializes the RLC Uplink Critcal Sections.
  
DEPENDENCIES
  This function should be called at task init.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_ul_crit_sect_init
(
  void
);
#endif

/*===========================================================================

FUNCTION RLC_UL_INIT

DESCRIPTION
  Initializes the RLC Uplink Logical channel table.
  
DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_ul_init
(
  void
);

/*===========================================================================

FUNCTION CRLC_UL_CIPHER_CONFIG_HANDLER

DESCRIPTION
  Configures ciphering on one or more Unacknowledged or Acknowledged 
  Mode Channels.  This function configures the ciphering parameters that are 
  recevied from RRC. The ciphering parameters are updated only if the the 
  channel is in TDSUL_DATA_TRANSFER_READY STATE.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_cipher_config_handler
( 
  tdsrlc_ul_cipher_config_type *cipher_ptr  /* Pointer to ciphering config.   */
);

/*===========================================================================

FUNCTION CRLC_UL_RESUME_HANDLER

DESCRIPTION
  Resume transmission on one or more uplink unacknowledged or acknowledged 
  mode channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_resume_handler
( 
  tdsrlc_ul_resume_type *resume_ptr         /* Pointer to Resume Information  */
);



/*===========================================================================

FUNCTION CLRLC_UL_REG_FOR_DATA_IND_HANDLER

DESCRIPTION
  Register Data Indication callback function to registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsclrlc_ul_reg_for_data_ind_handler
(
  tdsrlc_ul_register_for_data_ind_type *ul_reg_data_ind_ptr
);

/*===========================================================================

FUNCTION CLRLC_UL_UNREG_FOR_DATA_IND_HANDLER

DESCRIPTION
  De-Register Data Indication callback function from registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsclrlc_ul_unreg_for_data_ind_handler
(
  tdsrlc_ul_unregister_for_data_ind_type *ul_unreg_data_ind_ptr
);

/*===========================================================================

FUNCTION RLC_UL_REGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_register_srvc_handler 
(
  tdsrlc_ul_register_srvc_type *reg_ptr      /* Ptr to WM registration info   */
);

/*===========================================================================

FUNCTION RLC_UL_DEREGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_deregister_srvc_handler 
(
  tdsrlc_ul_deregister_srvc_type *de_reg_ptr /* Ptr to WM registration info   */
);

/*===========================================================================

FUNCTION CRLC_UL_TM_CONFIG_HANDLER

DESCRIPTION

  This function handles the configuration of Transparent Mode Logical 
  channels. This function is called when RRC sends a CRLC_TM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the downlink table entry for the 
  channel is updated with the configuration received from RRC.

  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdscrlc_ul_tm_config_handler 
(
  tdsrlc_ul_tm_config_type *tm_ptr         /* Pointer to TM config Info.      */
);

/*===========================================================================

FUNCTION CRLC_UL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical 
  channels. This function is called when RRC sends a CRLC_UM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the downlink table entry for the 
  channel is updated with the configuration received from RRC.


DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdscrlc_ul_um_config_handler 
(
  tdsrlc_ul_um_config_type *um_ptr           /* Pointer to UM config Info     */
);

/*===========================================================================

FUNCTION CRLC_UL_AM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of an Acknowledged Mode entity.
  it is called when RRC sends a CRLC_UL_AM_CONFIG_REQ to RLC. If a new 
  entity is to be established, the function checks if the channels exists. 
  If it already exists, an ERR message is dispalyed. If the entity does not 
  exist, an uplink table entry for this entity is updated and the state is
  set to DATA_TRANSFER_READY. 
  If an entity is to be released,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  DATA_TRANSFER_READY state. If it is, the downlink table entry for the 
  channel is updated with the configuration received from RRC.


DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_am_config_handler 
(
  tdsrlc_am_config_req_type *am_ptr
);

/*===========================================================================

FUNCTION RLC_UL_REGISTER_AM_SDU_CNF_CB_REQ

DESCRIPTION
  Register the SDU confirm callback function as request by RRC.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_register_am_sdu_cnf_cb_req
(
  tdsrlc_register_am_sdu_confirm_callback_type *sdu_cnf_cb_req
);

/*===========================================================================

FUNCTION RLCI_GET_CTL_BLK_PTR

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_ul_ctrl_blk_type *tdsrlci_get_ctl_blk_ptr 
(
  byte ch_id
);
/*===========================================================================
FUNCTION RLCI_SET_UL_NUM_SN_LI

DESCRIPTION
  Updates ul_num_sn_li, which indicates the number of SN-Li
  pairs which will formed on UL. This API is used to copy number
  SN-LI pairs calculated in DL to UL. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_set_ul_num_sn_li 
(
  uint8   lc_id,
  uint16  num_sn_li
);

/*===========================================================================

FUNCTION RLCI_DL_CONFIG_CNF_HANDLER

DESCRIPTION
  Handle the config confirmation from the Downlink side. This is for AM.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_config_cnf_handler
(
  tdsrlci_dl_config_cnf_type *cnf_ptr
);

/*===========================================================================

FUNCTION CRLC_UL_AM_RESET_HANDLER

DESCRIPTION
  Reset all AM entities that are configured.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_am_reset_handler(void);

/*===========================================================================

FUNCTION RLCI_UL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_DL_RESET_DONE and RLCI_UL_RESET_REQ messages from 
  DL AM RLC. 
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_proc_reset_msg
(
  tdsl2_ul_cmd_enum_type cmd_id, 
  rlc_lc_id_type ul_rlc_data_id
);

/*===========================================================================

FUNCTION RLCI_AM_HANDLE_10MS_TIMER_CB

DESCRIPTION
  Handle 10ms Base Timer timed out. 
  In Uplink, there is only one base timer. All timers related in the Uplink
  are done in the form of counters. Every time the 10ms base timer expired,
  this function looks into which timer counters are needed to be increased
  by one and then compares w/ the real configured timer related to the 
  individual counters for timer expiration. If the real timer expired, a
  related flag will be raised.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_am_handle_10ms_timer_cb(uint32 cb_param);

/*===========================================================================

FUNCTION  RLCI_TM_HANDLE_DISCARD_TIMER_CB

DESCRIPTION
  This callback function is called when the SDU discard timer expired.
  This function scan the WM to see whether there are SDUs needed to be 
  removed since they stay in the WM for too long. Also, it put the SDU
  discard time stamp on each new incoming SDU.
  
  The SDU discard time stamp is put in the 18th bit of the app_field, 
  since the lower 16 bits are used as the bit count of the DSM packet 
  chain.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_tm_handle_discard_timer_cb (uint32 cb_param);


/*===========================================================================

FUNCTION  RLCI_UM_HANDLE_DISCARD_TIMER_CB

DESCRIPTION
  This callback function is called when the SDU discard timer expired.
  This function scan the WM to see whether there are SDUs needed to be 
  removed since they stay in the WM for too long. Also, it put the SDU
  discard time stamp on each new incoming SDU.
  
  The SDU discard time stamp is put in the 3rd bit of the app_field, 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_um_handle_discard_timer_cb(uint32 cb_param);

/*===========================================================================

FUNCTION RLCI_AM_HANDLE_CELLPCH_TIMER_CB

DESCRIPTION
  Process CELLPCH_TIMER.
  
  This timer is set because of finding some retx PDUs remaining when RRC 
  registers data_ind CB to go to CELL_PCH. Since the RRC has to finish 
  its procedure, we set the timer and come back later to move UE out of
  CELL_PCH to send those remaining retx PDUs.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_am_handle_cellpch_timer_cb(uint32 cb_param);

/*===========================================================================

FUNCTION CRLC_RELEASE_ALL_HANDLER

DESCRIPTION
  Releases all Uplink Channels and sends a command to the Downlink to 
  release all downlink channels.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_release_all_handler(
  void
  );

/*===========================================================================

FUNCTION tdsrlci_ul_proc_release_all_cnf_msg

DESCRIPTION
  Releases all Uplink Channels after receive dl release cnf done.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_proc_release_all_cnf_msg(
  void
 );

/*===========================================================================

FUNCTION RLCI_POST_DL_RESET_MSG

DESCRIPTION
  Post RESET messages to the DL AM RLC. The RESET messages could be 
  RLCI_DL_RESET_REQ or RLCI_UL_RESET_DONE.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_dl_reset_msg
(
  void
);

/*===========================================================================

FUNCTION RLCI_POST_RESET_TO_RRC

DESCRIPTION
  Post the RESET result message to RRC
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_reset_to_rrc
(
  void
);

#ifndef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================

FUNCTION RLC_OPEN_PEER_TX_WIN

DESCRIPTION
  Set tx_wins flag for the next TTI to send status PDU to open the peer
  side's TX_WIN_SIZE.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_open_peer_tx_win
(
  rlc_lc_id_type rlc_id,
  tdsrlci_close_rx_win_e_type close_rx_win,
  tdsrlci_open_rx_win_e_type open_rx_win
);

/*===========================================================================

FUNCTION RLC_CLOSE_PEER_TX_WIN

DESCRIPTION
  Set tx_wins flag for the next TTI to send status PDU to close the peer
  side's TX_WIN_SIZE.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_close_peer_tx_win
(
  rlc_lc_id_type rlc_id,
  tdsrlci_close_rx_win_e_type close_rx_win
);
#endif //!FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL


/*===========================================================================

FUNCTION RLCI_POST_RLC_UL_RESET_DONE_SIG

DESCRIPTION
  This function post TDSRLC_AM_UL_RESET_DONE_SIG to L2 UL task. When L2 UL task 
  sees this message in the task context, it posts RLCI_UL_RESET_DONE to RLC
  DL.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_rlc_ul_reset_done_sig
(
  rlc_lc_id_type  rlc_id
);

/*===========================================================================

FUNCTION RLCI_POST_RLC_UL_RESET_DONE_CMD

DESCRIPTION
  Posts RLCI_UL_RESET_DONE to RLC DL. This function is excuted in the task
  context.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_rlc_ul_reset_done_cmd
(
  void
);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLCI_UL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION
  Collect the AM statistics data and send the statistics log packet out.
    
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_ul_am_handle_qxdm_log_timer(uint32 timer_sig);

/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  This function will be call when each PDU is building. This function is to 
  avoid to check the logging bit from the DIAG every time when a PDU is 
  building, variable tdsrlci_ul_pdu_cipher_log_on will be updated at the 
  beginning of building PDUs each TTI. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the PDU ciphering parameters logging is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_get_ul_pdu_cipher_log_on (void);

/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  Provide the access pointer of tdsrlci_ul_pdu_cipher_log data structure.  

DEPENDENCIES
  None.

RETURN VALUE
  pointer of tdsrlci_ul_pdu_cipher_log.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_ul_pdu_cipher_info_type *tdsrlci_get_ul_pdu_cipher_log_ptr(void);



#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

/*===========================================================================

FUNCTION RLC_UL_PURGE_UPLINK_WM

DESCRIPTION
  Purges the SDUs from the uplink WM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_purge_uplink_wm
(
  tdsrlc_ul_purge_wm_ind_type *ul_purge_wm_ind
);

/*===========================================================================

FUNCTION CRLC_UL_ABORT_CIPHER_CONFIG

DESCRIPTION
  This function reverts back to the old ciphering configuration if an abort
  is received from RRC while applying the new configuration due to triggering
  of cell update in RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_abort_cipher_config
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info
);


/*===========================================================================

FUNCTION  RLCI_AM_HANDLE_DISCARD_TIMER_CB

DESCRIPTION
  This callback function is called when the SDU discard timer expires.
  This function posts a command for RLC to process the expired timer. 
    
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_am_handle_discard_timer_cb(uint32 cb_param);


#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================

FUNCTION RLC_UL_APP_NEW_WINSUFI

DESCRIPTION
  Assigns new window size required by TDSAPP_CLOSE and transmits new window sufi.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_app_new_winsufi
(
  rlc_lc_id_type rlc_id,
  tdsrlci_peer_tx_win_size_e_type peer_win
);

/*===========================================================================

FUNCTION RLCI_UL_NUM_ACTI_NON_SRB_AM_CH

DESCRIPTION
  Returns number of non SRB AM channels established.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlci_ul_num_acti_non_srb_am_ch(void);

#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

/*===========================================================================

FUNCTION RLCI_POST_SDU_TX_CNF

DESCRIPTION
  Calls the sdu_cnf_cb function.The SDU tx confirmation delay information
  is stored in tdsam_ul_sdu_tx_cnf.This function is called in L2-UL context.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_sdu_tx_cnf(void);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_START_UL_LOGGING

DESCRIPTION
   This function will start logging AM/UM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_start_ul_logging (void);
#endif

/*===========================================================================

FUNCTION CRLC_UL_CONTINUE_HANDLER

DESCRIPTION
  Continue transmission on one or more uplink unacknowledged or acknowledged
  mode channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_continue_handler
(
  tdsrlc_ul_continue_type *continue_ptr      /* Ptr to continue Info                */
);


/*===========================================================================

FUNCTION RLC_UL_FORM_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_form_io_vec_per_sdu
(
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len,       /* Amount of data for the PDU     */
  tdsl1_l2_data_type    *l1_ul_tx_buf_ptr  
);

/*===========================================================================

FUNCTION RLC_UL_FORM_NON_EDCH_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_form_non_edch_io_vec_per_sdu
(
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len        /* Amount of data for the PDU     */
);

/*===========================================================================

FUNCTION RLC_UL_UPDATE_HDR

DESCRIPTION

  Stores the RLC PDU(AM/UM) header, LIs into the local buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_update_hdr
(
  uint8         hdr_len,
  uint16        hdr_val,
  tdsrlc_buff_type *hdr_list 
);

/*===========================================================================

FUNCTION RLC_UL_FILL_IO_VEC

DESCRIPTION

  Stores the IO vector into the L1 tx buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_fill_io_vec
( 
  uint8             *data_ptr,        /* Source location                */
  uint32            len,              /* No. of bytes                   */
  tdsl1_l2_data_type  *l1_ul_tx_buf_ptr, /* Place this IO vector is stored */
  boolean           last_io_vec  
);

/*===========================================================================

FUNCTION RLC_UL_GET_UL_HDR

DESCRIPTION
  
  Returns the pointer to the gloabal header buffer ul_hdr.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_ul_hdr_buff_type* tdsrlc_get_ul_hdr(void);

/*===========================================================================

FUNCTION RLC_UL_GET_PRE_ALLOC_PDU_DSM

DESCRIPTION

  Provides the next available DSM item for the pre allocated pool for
  building the R99 PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the available DSM item form the pool other wise NULL pointer.

SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type* tdsrlc_ul_get_pre_alloc_pdu_dsm(void);

/*===========================================================================

FUNCTION RLC_UL_INIT_IO_VEC_LIST

DESCRIPTION

  Intialising global io_vec_list before storing the set IO vector for 
  while building R99 AM new/re-tx, UM PDUs.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init_io_vec_list(void);

/*===========================================================================

FUNCTION RLC_UL_BUILD_NON_EDCH_PDU

DESCRIPTION

  Builds the R99 PDU using new io vector method of building PDUs

DEPENDENCIES
  None.

RETURN VALUE
  boolean: TRUE  - R99 pdu successfully build
           FASLE - R99 pdu building failed

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_build_non_edch_pdu
(
  tdsrlc_buff_type             *hdr_list,   /* Pointer to the headers         */
  tdsrlc_ul_pdu_data_type      *pdu_info,   /* valid if chnl_type = NON_E_DCH */
  uint16                    padding_len,  /* PDU padding length             */
  uint32                   key_index,   /* Ciphering key_id               */
  boolean                   enabled,     /* TRUE - cipheing applicable     */
                                         /* FALSE - otherwise              */
  uecomdef_umts_cipher_algo_e_type cipher_algo,
  uint32                    cnt_c        /* COUNT_C parameter              */
);

/*===========================================================================

FUNCTION RLC_UL_INIT_CTRL_DSM_POOL

DESCRIPTION

  Initializes the control PDU DSM pool.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init_ctrl_dsm_pool(void);

/*===========================================================================

FUNCTION RLC_DSM_PACK_DATA

DESCRIPTION

  This will pack the amount of bits from the specified byte & bit position
  in the data_ptr of the PDu

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dsm_pack_data
(
  dsm_item_type *pdu_ptr,   /* Pointer to the PDU dsm item    */
  uint32        pack_data,  /* Data to pack                   */
  uint16        bit_pos,    /* Bit position from the 1st byte */
  uint8         data_len    /* No. of bits to pack            */
);



/*===========================================================================

FUNCTION RLCI_GET_ACTIVE_UL_UM_CHNLS

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrlci_get_active_ul_um_chnls (void);

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*=========================================================================

FUNCTION RLCI_CHANGE_PEER_TX_WIN_FC

DESCRIPTION

    This function changes the peer side's transmission window size in case of
    RESET procedure is finished and RLC  wants to change the peer window size to
    the last FC window size

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

uint16 tdsrlci_change_peer_tx_win_fc
(
   tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr
); 
#endif

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

/*===========================================================================

FUNCTION RLCI_UL_DSM_FC_UPDATE_PEER_RX_WIN

DESCRIPTION
  Assigns new window size required by TDSRX_TX_CLOSE and transmits new window sufi.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_dsm_fc_update_peer_rx_win
(
  rlc_lc_id_type rlc_id,
  tdsrlci_peer_tx_win_size_e_type peer_win
);
#endif
/*===========================================================================

FUNCTION UL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Uplink control block for an RLC entity.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 tdsul_assign_ctl_blk(void);


/*===========================================================================

FUNCTION  RLCI_SET_SDU_DISCARD_TIMER 

DESCRIPTION
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_set_sdu_discard_timer
(
  uint8 index, 
  rlc_lc_id_type rlc_id
);

/*===========================================================================

FUNCTION RLC_UL_REMOVE_SDUS_FROM_WM

DESCRIPTION
  This function removes the SDUs from the UL WM. The number of SDUs that 
  are purged is the minimum of number specified by higher layers and the
  number of SDUs present in the WM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_remove_sdus_from_wm
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,
  uint8 num_sdus
);

/*===========================================================================

FUNCTION RLC_UL_COPY_IO_VEC_DATA

DESCRIPTION

  Copies the data from the set of IO vectors into the pre allocated DSM item
  while building R99 pdus.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_copy_io_vec_data
(
  dsm_item_type *item_ptr,  /* Pointer to the pre allocated DSM item       */
  uint16         offset      /* Offset with in the DSM item, from where the */
                            /* data copy should begin                      */
);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_UL_INIT_AM_LOG_PARAMS

DESCRIPTION

 Initalizes the tdslogging_params for the AM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init_am_log_params(void);

/*===========================================================================

FUNCTION RLC_UL_CHECK_AM_LOG_STATUS

DESCRIPTION

  Checks if the user plane/signalling plane PDU logging is turned on or not.

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE  if PDU needs to be logged
            FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_check_am_log_status
(
  tdsrlci_ul_am_ctrl_type    *am_ctrl_ptr, /* Pointer to ul_chan_am_pdu   */
  rlc_lc_id_type          rlc_id,       /* RLC ID                      */
  uint16                  pdu_size      /* RLC PDU SIZE                */
);

#endif



/*===========================================================================

FUNCTION RLCI_UPDATE_TX_WIN_SIZE

DESCRIPTION
  Received Tx_Window_Size from the peer side. Update VT(MS) according the
  newly received tx_window_size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_update_tx_win_size
(
  tdsrlci_ul_am_ctrl_type *ctrl_blk,
  uint16          tx_win_size           /* tx_window_size                 */
);
#ifdef FEATURE_QXDM_DSM_TRACING
/*===========================================================================

FUNCTION RLC_FC_GET_DSM_TRACE_CB

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_get_dsm_trace_cb
(
    uint32 cb_data, 
    fc_dsm_trace_info_type *wm_stat_ptr
);
#endif

#if 0

/*===========================================================================

FUNCTION RLC_POST_FC_INTERNAL_CMD

DESCRIPTION
  This function post internal cmd to L@ UL task,
  so the fc cmd is processed in UL L2 task.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_post_fc_internal_cmd (
  tdsrlc_fc_cmd_entity_type entity,
  uint16 cmd);

/*===========================================================================

FUNCTION RLC_FC_INTERNAL_CMD_HDLR

DESCRIPTION
  processing function of L2 UL task.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_internal_cmd_hdlr (void);

/*===========================================================================

FUNCTION  RLCI_FC_DSM_CB

DESCRIPTION
  The DSM item pool goes to the DON'T-EXCEED level. RESET RLC channels 
  for user plain to release DSM items on hold.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_fc_dsm_cb
(
  dsm_mempool_id_enum_type  dsm_pool_id,
  dsm_mem_level_enum_type   dsm_mem_level, 
  dsm_mem_op_enum_type      dsm_mem_op
);

#endif
/*===========================================================================

FUNCTION TDSRLC_DUMP_UL_CTL_BLK_ID_TBL

DESCRIPTION
  dumps uplink_table index values of LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dump_ul_ctl_blk_id_tbl(void);
/*===========================================================================

FUNCTION TDSRLC_SANITIZE_UL_CTL_BLK_ID_TBL

DESCRIPTION
  Checks for any duplicate uplink_table index values of two different LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_sanitize_ul_ctl_blk_id_tbl(void);

#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION tdsrlc_send_qsh_cmd

DESCRIPTION
  Send local cmd TDSL2_QSH_CMD to L2 . it's called by the tds qsh timer registered callback
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrlc_send_qsh_cmd( qsh_client_cb_params_s *param );

/*===========================================================================
FUNCTION tdsrlc_qsh_debug_callback

DESCRIPTION
This is the callback function for QSH to collect data from TDSRLC. The callback function will 
send a local command to L2-RLC to trigger the metric collection.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrlc_qsh_debug_callback(qsh_client_cb_params_s *param);

/*===========================================================================

FUNCTION TDSRLC_QSH_TC_METRICS_INIT

DESCRIPTION
  Initializing the metrics for test support
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_qsh_tc_metrics_init(void);

/*===========================================================================

FUNCTION TDSRLC_QSH_REGISTER

DESCRIPTION
  Registering TDS L2 for QSH metrics collection with QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_register(void);


/*===========================================================================

FUNCTION tdsrlc_update_metric_address

DESCRIPTION
  This will update the local copy of next address and metric context recieved from QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_update_metric_address(qsh_metric_id_t metric_id, qsh_client_metric_context_id_t qsh_metric_context_id, sys_modem_as_id_e_type subs_id, uint8* qsh_address );

/*===========================================================================

FUNCTION TDSRLC_QSH_COLLECT_REPORT_METRICS

DESCRIPTION
  This function collects the metrics on timer expiry callback. Writes the data on
  to the address provided and then notifies QSH of the data written.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_collect_report_metrics(qsh_client_cb_params_s* param);


/*===========================================================================

FUNCTION tdsrlc_qsh_process_cfg

DESCRIPTION
  This will process the Metric cfg command from QSH. It also saves the start address
  and does a check on alloted size.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_process_cfg(qsh_client_cb_params_s* param);


/*===========================================================================

FUNCTION TDSRLC_QSH_PROCESS_CMD

DESCRIPTION
  This function is a command handler for QSH commands CFG and TIMER EXPIRY
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_qsh_process_cmd(tdsrlc_qsh_command_type* rlc_qsh_cmd);


void tdsrlc_qsh_update_bad_status_pdu(uint8 rb_id, uint16 sn, uint16 vt_a, uint16 vt_s);

#endif /*FEATURE_QSH_EVENT_METRIC */
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
/*===========================================================================

FUNCTION TDSRLC_PROCESS_TA_IND

DESCRIPTION
  Processes the LTA Start/Stop received from RRC

===========================================================================*/
void tdsrlci_process_ta_ind(tdsrlc_ta_info_type *ta_info_ptr);

/*===========================================================================

FUNCTION TDSRLC_PROC_LTA_START_IND

DESCRIPTION
  Processes LTA Start Indication received from RRC

===========================================================================*/
void tdsrlc_proc_lta_start_ind(tdsrlc_ta_success_e_type ta_succ_ind);

/*===========================================================================

FUNCTION TDSRLC_PROC_LTA_STOP_IND

DESCRIPTION
  Processes LTA Stop Indication received from RRC

===========================================================================*/
void tdsrlc_proc_lta_stop_ind(tdsrlc_ta_success_e_type ta_succ_ind);
/*===========================================================================

FUNCTION TDSRLC_GET_SYSTEM_TIME

DESCRIPTION
  Get system time

===========================================================================*/
uint32 tdsrlc_get_system_time(void);
#endif/*#FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA */
#endif




