#ifndef TDSRLCDL_H
#define TDSRLCDL_H
/*===========================================================================
                  D O W N L I N K  R L C  I N T E R N A L

                         H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  downlink RLC module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlcdl.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

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
#include "assert.h"
#include "rex.h"
#include "tdsrlcrrcif.h"
#include "tdsrlcdsapif.h"
#include "tdsmacrlcif.h"
#include "tdsrlci.h"
#include "tdsl2dltask.h"
#include "tdsl2ultask.h"
#include "tdsrlclog.h"
#include "tdsl1macif.h"

#include "modem_mem.h"
#define TDSMAX_RX_WINDOW_SIZE 2048

#define TDSRLC_WM_TRACE_CTR 50
/*---------------------------------------------------------------------------
  The user/data plane logical channel ids start from the value that is
  calculated by RRC as MAX DL TM channels + MAX DL UM channels +
  MAX AM Signalling channels.
---------------------------------------------------------------------------*/

#define TDSRLC_DL_AM_USR_PLANE_START_LC_ID (UE_MAX_DL_TM_CHANNEL + \
                     TDSUE_MAX_DL_UM_CHANNEL + UE_MAX_SRB_AM_EMTITIES_3)

#define TDSRLC_DL_AM_START_LC_ID (UE_MAX_DL_TM_CHANNEL + \
                     TDSUE_MAX_DL_UM_CHANNEL)


#define TDSRLC_RE_ASSEMBLY_Q_SIZE 2048

#define TDSRLC_AM_HALF_MOD_MASK 0x07FF
/* Bitmask for enabling/disabling UM user plane log packets */
#define TDSRLCI_UM_USR_PLANE_LOG_MASK 0x01

/* Bitmask for enabling/disabling AM user plane log packets */
#define TDSRLCI_AM_USR_PLANE_LOG_MASK 0x02

/* Bitmask for enabling/disabling AM signalling plane log packets */
#define TDSRLCI_AM_SIG_PLANE_LOG_MASK 0x04
 
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/* Bitmask for enabling/disabling AM log packets for R7 */
#define TDSRLCI_AM_R7_LOG_MASK 0x08
#endif

#define TDSRLCI_GET_RESEQ_Q_IDX(sn) ((sn) & TDSRLC_AM_HALF_MOD_MASK)

#define TDSRLCI_GET_PDU_DATA_PTR(pdu_ptr, offset) (pdu_ptr + offset)

#define TDSRLCI_GET_DC_BIT(pdu_hdr)  (pdu_hdr & 0x8000)

#define TDSRLCI_GET_AM_SN(pdu_hdr)   ((pdu_hdr & 0x7fff) >> 3)

#define TDSRLCI_GET_POLL_BIT(pdu_hdr)  ((pdu_hdr & 0x0004) >> 2)

#define TDSRLCI_GET_H_EXT_BIT(pdu_hdr)  (pdu_hdr & 0x0003)

#define TDSRLCI_GET_LI_EXT_BIT(pdu_hdr) (pdu_hdr & 0x01)

#define TDSRLCI_GET_CTRL_PDU_TYPE(pdu_hdr)  (pdu_hdr & 0x07)

#define TDSRLCI_GET_ACCUM_LI(pdu_hdr)   (pdu_hdr >> 1)

#define TDSRLCI_GET_INC_Q_IDX(idx)   ((idx+1) & TDSRLC_AM_HALF_MOD_MASK)

#define TDSRLCI_GET_RESET_SN(data)  ((data & 0x08) >> 3)

#define TDSRLCI_GET_RESET_RESV(data)   (data & 0x07)

#define TDSRLCI_GET_LI_VAL(li)    (li >> 1)

#define TDSRLCI_INVALID_SEQ_NUM 0xFFFF
#define TDSRLCI_MAX_LI_IN_LIST 15

#define TDSRLC_GET_ABS_LI(walk_item_ptr, offset, li_len, accum_li) \
 (li_len == TDSRLC_LI_SHORT) ? (accum_li = walk_item_ptr->dsm_ptr->data_ptr[offset]) \
                          : (accum_li = (((uint16) walk_item_ptr->dsm_ptr->data_ptr[offset] << 8) | ((uint16)walk_item_ptr->dsm_ptr->data_ptr[offset + 1])))


#define TDSRLCI_GET_UM_SN(pdu_hdr)   ((pdu_hdr & 0xFF) >> 1)

#define tdsrlc_discard_sdu_ptr(item_ptr) \
             tdsrlci_discard_sdu_ptr(item_ptr, __FILENAME__, __LINE__)

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
#define TDSRLCI_GET_AM_LOGGING_CONFIG(lc_id) \
      (((lc_id >= TDSRLC_DL_AM_USR_PLANE_START_LC_ID) && \
        ((tdsrlci_log_mask & TDSRLCI_AM_USR_PLANE_LOG_MASK) || \
         (tdsrlci_log_mask & TDSRLCI_AM_R7_LOG_MASK))) || \
       ((lc_id < TDSRLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (tdsrlci_log_mask & TDSRLCI_AM_SIG_PLANE_LOG_MASK)))

#define TDSRLCI_GET_AM_R7_LOG_ENABLED_THIS_LC(lc_id) \
        ((lc_id >= TDSRLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (tdsrlci_log_mask & TDSRLCI_AM_R7_LOG_MASK))

#define TDSRLCI_GET_AM_R7_LOG_ENABLED() \
        (tdsrlci_log_mask & TDSRLCI_AM_R7_LOG_MASK)
#else
#define TDSRLCI_GET_AM_LOGGING_CONFIG(lc_id) \
      (((lc_id >= TDSRLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (tdsrlci_log_mask & TDSRLCI_AM_USR_PLANE_LOG_MASK)) || \
        ((lc_id < TDSRLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (tdsrlci_log_mask & TDSRLCI_AM_SIG_PLANE_LOG_MASK)))
#endif

#define TDSRLCI_GET_UM_LOGGING_CONFIG \
         (tdsrlci_log_mask & TDSRLCI_UM_USR_PLANE_LOG_MASK)

//DL_INTG
/*         
#define TDSRLC_AM_STAT_INC_ERR_PDU(dl_chan_am_stat_ptr) \
            dl_chan_am_stat_ptr->tot_num_error_pdu_rxd++

#define TDSRLC_AM_STAT_INC_CTRL_PDU(dl_chan_am_stat_ptr) \
            dl_chan_am_stat_ptr->tot_num_ctl_pdu_rxd++

#define TDSRLC_AM_STAT_INC_DATA_PDU(dl_chan_am_stat_ptr) \ 
            dl_chan_am_stat_ptr->tot_num_data_pdu_rxd++

#define TDSRLC_AM_STAT_UPD_NUM_PDU_BYTE_RCVD(dl_chan_am_stat_ptr, length) \
            dl_chan_am_stat_ptr->tot_num_pdu_byte_rxd += length

#define TDSRLC_AM_STAT_UPD_NUM_SDU_BYTE_RCVD(dl_chan_am_stat_ptr, length) \
            dl_chan_am_stat_ptr->tot_num_sdu_byte_rxd += length
*/

#define TDSRLC_AM_STAT_INC_ERR_PDU(macro_var)  macro_var++

#define TDSRLC_AM_STAT_INC_CTRL_PDU(macro_var) macro_var++

#define TDSRLC_AM_STAT_INC_DATA_PDU(macro_var) macro_var++

#define TDSRLC_AM_STAT_UPD_NUM_PDU_BYTE_RCVD(macro_var, length) \
            macro_var += length

#define TDSRLC_AM_STAT_UPD_NUM_SDU_BYTE_RCVD(macro_var, length) \
            macro_var += length

#endif


#define TDSRLCI_GET_CTRL_INDEX(lc_id)   tdsdl_ctl_blk_id_tbl[lc_id]
#define TDSRLCI_GET_CTRL_BLK_PTR(ctrl_index)    &(tdsdownlink_table.ctrl_blk[ctrl_index])
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
#define TDSRLC_WIN_OPT_THRESHOLDE (60) 
#endif
/*===========================================================================

MACRO TDSGET16

DESCRIPTION
  This macro return the word value from the 2 byte value

DEPENDENCIES
  None

RETURN VALUE
  A word representing the 2 bytes pointed at by the passed parameter

SIDE EFFECTS
  None
===========================================================================*/

#define TDSGET16(cp) (                             \
                      ((( cp[0]) << 8) | cp[1])   \
                  )

/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

/*---------------------------------------------------------------------------
SRB's - 2 UM (srb 0,1) + 3 AM (srb 2,3,4)
CTCH - 1 UM
BCCH - 1 TM
AMR - 3 TM
PS - 2 AM or 2 UM
Video call - 1 TM

TDSUE_MAX_DL_RLC_ENTITY   18 (5TM, 5UM, 5AM + 3 buffer)
TDSUE_MAX_DL_RLC_ENTITY   24 for MBMS with 2MCCH + 2MSCH + 2MTCH
---------------------------------------------------------------------------*/
#define TDSUE_MAX_DL_RLC_ENTITY    18


extern tdsrlci_missing_list_type *tdsrlc_dl_nak_pool;
extern q_type tdsrlc_dl_free_nak_q;

typedef enum {
  TDSDL_NULL_STATE = 0,                    /* Logical channel is currently not */
                                        /* in use.                          */
  TDSDL_DATA_TRANSFER_READY,               /* Logical Channel has been         */
                                        /* established, and data transfer   */
                                        /* is allowed.                      */
  TDSDL_RESET_PENDING,                     /* RESET has been sent to the peer  */
                                        /* RLC entity,and no data transfer  */
                                        /* is allowed unitl a RESET ACK is  */
                                        /* received. Only applicable for    */
                                        /* AM channels.                     */
  TDSDL_STOP = 0x04                        /* Stop to receive any transmission */
} tdsrlci_dl_state_e_type;

/*---------------------------------------------------------------------------
  Typedef for storing parameters specific to downlink transparent mode.
---------------------------------------------------------------------------*/
typedef struct {
  boolean seg_ind;                      /* Segmentation Indication- TRUE if*/
                                        /* segmentation is on, FALSE       */
                                        /* otherwise.                      */
  boolean act_incl;                     /* Activation Time - True if       */
                                        /* included, False Otherwise       */
  uint8 act_time;                       /* Activation Time.                */
} tdsrlci_dl_tm_ctrl_type;

/*---------------------------------------------------------------------------
  Typedef for storing UM PDU logging parameters
---------------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
typedef struct{
  uint16 num_pdu;                 /* Number of PDUs to be built every TTI  */
  uint16 pdu_size;                /* PDU size in octets of each PDU        */
  uint32 log_data[TDSMAX_DL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
} tdsrlc_dl_chan_um_pdu_type;
#endif



/*---------------------------------------------------------------------------
  Typedef for storing parameters specific to downlink unacknowledged mode.
---------------------------------------------------------------------------*/
typedef struct {
  boolean act_incl;                     /* Activation Time - True if       */
                                        /* included, False Otherwise       */
  uint8 act_time;                       /* Activation Time.                */
  uint8 li_length;                      /* Length of the length indicator  */
                                        /* - 7 or 15 bits                  */
  uint8 vr_ur;                          /* Sequence Number of the next Pdu */
                                        /* to be received. This value is   */
                                        /* set to RLC_SN + 1 upon reception*/
                                        /* of a PDU                        */
  dsm_item_type *pdu_ptr;               /* Pointer to chain of pdus being  */
                                        /* reassembled                     */
  boolean discard_flag;                 /* Indicates if the PDUs belonging */
                                        /* to a SDU should be discarded    */
  rlc_lc_id_type rlc_id;                /* Logic channel ID                */

  dsm_item_type* partial_sdu_ptr;
  dsm_item_type* last_dsm_ptr;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
  tdsrlc_dl_chan_um_pdu_type  dl_chan_um_pdu;  /* UM PDU logging structure*/
#endif
#endif


  boolean alternate_e_bit;          /* TRUE - Alternate E-bit interpretation */
                                    /* FALSE - Normal E-bit interpretation   */

#ifdef FEATURE_TDSCDMA_UM_CCCH_OPT
boolean need_fisrt_Li_flag;
#endif
} tdsrlci_dl_um_ctrl_type;

/*---------------------------------------------------------------------------
  Used to store the necessary down-link control state information for an
  RLC AM entity.
---------------------------------------------------------------------------*/
typedef struct {
  uint16 old_vr_r;
  uint16 dl_vr_h_status;                /*DL vr_h state indicating the SN */
                                        /*till which nak array has been   */
                                        /*updated. Alternatively this indicates*/
                                        /*the highest SN till which UL should*/
                                        /*form status PDUs.               */
  uint16 vr_r;                          /* VR(R) -- Rx state               */
  uint16 vr_h;                          /* VR(H) -- Expected Rx state      */
  uint16 prev_vr_h;                     /* PREV_VR_H - Store the old value */
                                        /* of VR(H)                        */
  uint16  report_vr_h;

  uint16 rx_win;                        /* Rx window size                  */
  uint16 vr_mr;                         /* VR(MR) -- V(R) + Rx_Window_size */
  uint16 first_err;                     /* First Error Sequence Number     */
                                        /* received in the status report.  */
                                        /* Valid only if nak_rvcd = TRUE   */
  boolean nak_rcvd;                     /* Received a NAK in the Status    */
                                        /* Report.                         */
  boolean rcvd_poll;                    /* Received a poll from the peer   */
                                        /* side in this TTI.               */
  boolean status_prohibit;              /* status report prohibit or not   */
  boolean missing_pdu_ind;              /* Missing PDU indicator           */
  uint8 li_length;                      /* Length of the length indicator  */
                                        /* value 1 means 7 bits            */
                                        /* Value 2 means 15 bits           */
  boolean discard_flag;                 /* Flag indicating whether the     */
                                        /* first N LIs and SDUs of the     */
                                        /* first PDU in the reseq Queue    */
                                        /* need to be discarded.           */
  byte nlen;                            /* Number of LIs to discard. Valid */
                                        /* only if discard_flag == TRUE    */
  boolean change_wins;                  /* Indicates if window size changed*/
  uint16  status_periodic_cnt;
  uint16  status_prohibit_cnt;
  uint16  last_sn_in_q;                 /* Last SN in re-assemble queue.   */
  boolean status_periodic;              /* Status periodic                 */
  uint8   peer_reset_sn;
  uint8   backup_timer_cnt;             /* backup timer                    */
  boolean fst_rx_pdu;                   /* TRUE, if the next rx PDU is the */
                                        /* 1st rx'd PDU.                   */

  uint16  cur_status_periodic;          /* Current Timer_Status_Periodic   */
                                        /* value                           */
  boolean lower_hfn_rcvd;               /*flag to track if HFN value received */
                                        /*in RESET/RESET_ACK is lower than the one configured*/
  uint16 last_sn_processed;
} tdsrlci_dl_am_state_type;


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*---------------------------------------------------------------------------
  Data Structure for RLC DL states for Acknowledged Mode statistics logging
---------------------------------------------------------------------------*/
typedef struct{
  uint32 tot_num_pdu_byte_rxd;    /* Total number of bytes received        */
  uint32 tot_num_sdu_byte_rxd;    /* Total number of sdu bytes to be sent  */
                                  /* to upper layer                        */
  uint16 tot_num_error_pdu_rxd;
  uint16 tot_num_data_pdu_rxd;    /* Total number of AMD PDUs received     */
  uint16 tot_num_ctl_pdu_rxd;     /* Total number of Control PDUs received */
  uint16 tot_num_pdu_nacked;      /* Total number of pdus nacked by peer   */
}tdsrlc_dl_chan_am_stat_type;

/*--------------------------------------------------------------------------
Data structures for logging the first 4 bytes of the PDU by QCAT.
---------------------------------------------------------------------------*/

typedef struct{
  uint16 num_pdu;                 /* Number of PDUs to be built every TTI  */
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  uint16 pdu_size[TDSMAX_DL_RLC_LOG_PDUS];
#else
  uint16 pdu_size;                /* PDU size in octets of each PDU        */
#endif
  uint32 log_data[TDSMAX_DL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
} tdsrlc_dl_chan_am_pdu_type;

#endif


typedef struct {
  dsm_item_type*   dsm_ptr;
  uint16           offset;
  uint16           length;
  /* AM - last 15 bits - SN(12bits) + P(1 bit) + HE(2 bits) */
  /* UM - last 8 bits - SN(7 bits) + E(1 bit) */
  uint16           ctrl_info; 
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
  uint32           rec_time;  /* The system receive time for this PDU*/ //xlong SDU time
#endif
}tdsrlc_pdu_info_type;

typedef struct {
  dsm_item_type*   dsm_ptr;
  uint16           offset;
  uint16           length;
}tdsrlc_am_sdu_info_type;

typedef struct {
  boolean in_use;
  rlc_lc_id_type dl_data_id;
  tdsrlc_pdu_info_type *pdu_info;
  dsm_item_type* partial_sdu_ptr;
  dsm_item_type* last_dsm_ptr;
  uint32 sdu_size;
}tdsrlc_dl_am_reseq_q_type;  


typedef struct {
  uint16 pdu_cnt;

  /* starting address of big dsm poiner which may have many PDU payload */
  dsm_item_type   *dsm_ptr;

  /* offset from where this PDU payload starts */
  uint16 offset;

  /* length of PDU*/
  uint16 length;
}tdsrlci_dl_temp_pdu_info_type;

/*---------------------------------------------------------------------------
  Used to store the necessary down-link control information for
  an RLC AM entity.
---------------------------------------------------------------------------*/
typedef struct {
  tdsrlc_am_common_config_type am_common_local; /* Local copy of common config variables.        */
  tdsrlc_dl_am_config_type  dl_ctl_cnfg_local;   /* Local copy dl control config variables.    */
  tdsrlc_am_common_config_type *am_common; /* pointer to common config variables.        */
  tdsrlc_dl_am_config_type *dl_ctl_cnfg;   /* pointer to dl control config variables.    */
  tdsrlci_dl_am_state_type  dl_state_var;  /* dl control state variables.     */
  tdsrlc_opt_status_type  dl_status_opt_bitmap; /* bitmap array for DL status */
  uint16               num_sn_li; /* Number of SN_LI pairs for LIST SUFI*/
  tdsrlc_dl_am_reseq_q_type *dl_reseq_q;   /* DL Resequence Queue holding 
                                           the received DL PDUs */
  void (*sdu_discard_cb) (rlc_lc_id_type data_lc_id, byte num_sdu);
                                        /* SDU discard callback function   */
  tdsrlc_tmr_status_type dl_tmr_sts;       /* Holds status of DL AM timer     */
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  tdsrlc_dl_chan_am_stat_type dl_chan_am_stat; /* Stores the Am statistics    */
                                            /* information for one channel */
  tdsrlc_dl_chan_am_pdu_type  dl_chan_am_pdu;   /* Stores the PDU information */
                                             /* for a single AM entity     */
#endif
  boolean  reset_ack_rcvd;    /*Flag indicating if valid RESET_ACK is received on DL */
} tdsrlci_dl_am_ctrl_type;

/*---------------------------------------------------------------------------
  Union for storing parameters specific to a mode.
---------------------------------------------------------------------------*/
typedef union {
  tdsrlci_dl_tm_ctrl_type tm_ctrl;         /* Transparent Mode parameters.    */
  tdsrlci_dl_um_ctrl_type um_ctrl;         /* Unacknowledged Mode parameters. */
  tdsrlci_dl_am_ctrl_type am_ctrl;         /* Acknowledged Mode parameters.   */
} tdsrlci_dl_ctrl_u_type;

/*---------------------------------------------------------------------------
  Used to store information about a downlink logical channel.
---------------------------------------------------------------------------*/
typedef  struct {
  uecomdef_logchan_e_type lc_type;      /* Downlink logical channel type   */
  uecomdef_logch_mode_e_type lc_mode;   /* RLC Mode                        */
  tdsrlci_dl_state_e_type lc_state;        /* Downlink Logical channel state  */
  tdsrlci_ciphering_type cipher;           /* Ciphering Parameters            */

  tdsrlci_ciphering_type cipher_backup;    /* Backup for ciphering parameters
                                           while applying the new one      */

  tdsrlci_dl_ctrl_u_type ctrl;             /* Union of TM, UM and AM specific */
                                        /* parameters.                     */
  boolean context;                      /* Task(FALSE) or Interrupt (TRUE) */

  void *rlc_post_rx_func_ptr_para;      /* 3rd parameter for               */
                                        /* rlc_post_rx_func_ptr()          */
  uint16 n_sdu;                      
  void  (*rlc_post_rx_func_ptr)(uint8, uint8, void *);
                                        /* Callback function called when an*/
                                        /* SDU is placed on the downlink   */
                                        /* data queue                      */

  dsm_watermark_type *dl_wm_ptr;        /* Pointer to the associated RLC   */
                                        /* downlink data queue             */

} tdsrlci_dl_ctrl_blk_type;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*---------------------------------------------------------------------------
  The following two structures are used only for QXDM logging.
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  Data Structure for RLC DL states for Transparent, Unacknowledged and
  Acknowledged Modes
---------------------------------------------------------------------------*/
typedef struct
{
  uint8    rlc_id;              /* DL logical channel id (0..18)           */
  uint8    lc_state;            /* State of the RLC State Machine          */
                                /* 0 - TDSRLC_DL_NULL_STATE                   */
                                /* 1 - TDSTM_DL_DATA_TRANSFER_READY           */
                                /* 2 - TDSUM_DL_DATA_TRANSFER_READY           */
                                /* 3 - TDSAM_DL_DATA_TRANSFER_READY           */
                                /* 4 - TDSAM_DL_RESET_PENDING                 */

} tdsrlc_dl_chan_state_type;

typedef struct
{
  uint8 nchan;                  /* Number of Downlink logical channels.    */
  tdsrlc_dl_chan_state_type dl_state[TDSUE_MAX_DL_LOGICAL_CHANNEL];
                                /* State Information for each Downlink     */
                                /* logical channel.                        */
}tdsrlc_dl_log_state_type;

/*--------------------------------------------------------------------------
Data structures for logging the DL AM&UM PDU ciphering parameters
---------------------------------------------------------------------------*/

typedef struct{
  uint8  log_dl_rlc_id;           /* Data Logical Channel DL RLC ID(0..18) */
  uint32 cipher_key_idx;
  uecomdef_umts_cipher_algo_e_type cipher_algo;
  uint32 b4_cipher;               /* The 1st 4 bytes of the PDU before     */
                                  /* decipher.                             */
  uint32 count_c;                 /* PDU size in octets of each PDU        */
                                  /* Dump of the status PDU                */
} tdsrlc_dl_cipher_log_data_type;

/*--------------------------------------------------------------------------
Data structures for logging the DL AM&UM PDUs ciphering parameters
---------------------------------------------------------------------------*/
typedef struct{
  boolean cipher_pdu_logging;     /* TRUE - cipher PDUs to be logged in    */
                                  /* Task Context, FALSE - otherwise       */
  uint8 num_pdu;                  /* Number of PDUs to be built every TTI  */
  tdsrlc_dl_cipher_log_data_type cipher_log_data[TDSMAX_CIPHER_PDU];
                                  /* The first four bytes of the pdu       */
}  tdsrlc_dl_pdu_cipher_info_type;


#endif


/*---------------------------------------------------------------------------
  Database that is used to store information about all the downlink logical
  channels. Maximum number of downlink logical channels is 21.
---------------------------------------------------------------------------*/

typedef struct {
  uint8 active_ch;                      /* Num of active logical channels. */
  tdsrlci_dl_ctrl_blk_type ctrl_blk[TDSUE_MAX_DL_RLC_ENTITY];
                                        /* Data structure used to store all*/
                                        /* the information for the         */
                                        /* downlink logical channel.       */
} tdsrlci_downlink_table_type;

typedef struct {
  uint8 rlc_id;
  uint8 npdus;
  uint16 pdu_size;
  dsm_item_type *dsm_ptr[TDSUE_MAX_DL_RLC_PDUS];
}tdsrlci_dl_chan_data_type;

typedef struct {
  uint8 nchan;
  tdsrlci_dl_chan_data_type chan_info[TDSUE_MAX_DL_LOGICAL_CHANNEL];
} tdsrlci_dl_frame_data_type;


extern uint8 tdsdl_ctl_blk_id_tbl[TDSUE_MAX_DL_LOGICAL_CHANNEL];

/*===========================================================================

FUNCTION RLCI_DL_AM_HANDLE_10MS_TIMER_CB

DESCRIPTION
  Handle 10ms Base Timer timed out.
  In downlink, there is only one base timer. All timers related in the downlink
  are done in the form of counters. Every time the 10ms base timer expired,
  this function looks into the timer counters that needed to be increased
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
#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*===========================================================================

FUNCTION TDSRLC_DL_CRIT_SECT_INIT

DESCRIPTION
  Initializes the RLC DL critical sections

DEPENDENCIES
  This function should be called at task init.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_crit_sect_init(void);
#endif
void tdsrlci_dl_am_base_timer_handler ( void );
/*===========================================================================

FUNCTION TDSRLC_DL_INIT

DESCRIPTION
  Initializes the RLC Downlink Logical channel table.

DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_init
(
  void
);

/*===========================================================================

FUNCTION TDSCRLC_DL_CIPHER_CONFIG_HANDLER

DESCRIPTION
  Configures ciphering on one or more Unacknowledged or Acknowledged
  Mode Channels.This function configures the ciphering parameters that are
  recevied from RRC. The ciphering parameters are updated only if the the
  channel is in DL_DATA_TRANSFER_READY STATE.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_cipher_config_handler
(
  tdsrlc_dl_cipher_config_type *cipher_ptr   /* Ptr to Downlink Cipher Config */
);

/*===========================================================================

FUNCTION RLC_DL_REGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark and the callback
  function for the logical channels on the downlink.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_register_srvc_handler
(
  tdsrlc_dl_register_srvc_type *reg_ptr      /* Ptr to WM registration Info   */
);

/*===========================================================================

FUNCTION TDSRLC_DL_DEREGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the de-registration of the watermark and the callback
  function for the logical channels on the downlink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_deregister_srvc_handler
(
  tdsrlc_dl_deregister_srvc_type *de_reg_ptr      /* Ptr to WM registration Info   */
);

/*===========================================================================

FUNCTION TDSRLC_DL_PDU_DATA_HANDLER

DESCRIPTION
 This function processes PDUs received on unacknowledged mode and
 acknowledged mode channels. If the channel for which the PDU is intended is
 in DL_DATA_TRANSFER_READY State, a function is called to process the PDU.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_pdu_data_handler(void);


/*===========================================================================

FUNCTION TDSRLC_REGISTER_AM_SDU_DISCARD_CB_REQ

DESCRIPTION
  Register AM SDU discard callback function.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_register_am_sdu_discard_cb_req
(
  tdsrlc_register_am_sdu_discard_callback_type *dl_sdu_discard_cb
);

/*===========================================================================

FUNCTION TDSCRLC_DL_TM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Transparent Mode Logical
  channels. This function is called when RRC sends a TDSCRLC_DL_TM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to DL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the
  DL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  DL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_tm_config_handler
(
  tdsrlc_dl_tm_config_type *tm_ptr /* Pointer to TM config info               */
);


/*===========================================================================

FUNCTION TDSCRLC_DL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical
  channels. This function is called when RRC sends a TDSCRLC_DL_UM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to DL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the
  DL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  DL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_um_config_handler
(
  tdsrlc_dl_um_config_type *um_ptr /* Pointer to UM config info               */
);

/*===========================================================================

FUNCTION TDSCRLC_DL_AM_CONFIG_HANDLER
  This function handles the configuration of an Acknowledged Mode entity.
  it is called when RRC sends a CRLC_DL_AM_CONFIG_REQ to RLC. If a new
  entity is to be established, the function checks if the channels exists.
  If it already exists, an ERR message is dispalyed. If the entity does not
  exist, an uplink table entry for this entity is updated and the state is
  set to DATA_TRANSFER_READY.
  If an entity is to be released,the function checks if the channel is in the
  DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  DATA_TRANSFER_READY state. If it is, the downlink table entry for the channel
  is updated with the configuration received from RRC.

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdscrlc_dl_am_config_handler
(
  tdsrlci_dl_am_config_req_type *dl_am_config_req_ptr
);

/*===========================================================================

FUNCTION TDSGET_AM_CTL_RLK

DESCRIPTION
  This function return the pointer of control block for an AM.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the AM control block.

SIDE EFFECTS
  None
===========================================================================*/

tdsrlci_dl_ctrl_blk_type *tdsget_am_ctl_blk
(
  byte lc_id
);

/*===========================================================================

FUNCTION TDSRLCI_DL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_UL_RESET_DONE and RLCI_DL_RESET_REQ messages from
  UL AM RLC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_proc_reset_msg
(
  tdsl2_dl_cmd_enum_type cmd_id,
  rlc_lc_id_type dl_rlc_data_id
);

/*===========================================================================

FUNCTION TDSCRLC_DL_RELEASE_ALL_HANDLER

DESCRIPTION
  Releases all downlink Channels and sends a confirmation to the uplink
  after release.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_release_all_handler(
  void
  );


/*===========================================================================

FUNCTION TDSRLCI_DL_GET_CTL_BLK_PTR

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_dl_ctrl_blk_type *tdsrlci_dl_get_ctl_blk_ptr(uint8 ch_id) ;


/*===========================================================================

FUNCTION RLCI_DL_AM_HANDLE_10MS_TIMER_CB

DESCRIPTION
  Handle 10ms Base Timer timed out.
  In downlink, there is only one base timer. All timers related in the downlink
  are done in the form of counters. Every time the 10ms base timer expired,
  this function looks into the timer counters that needed to be increased
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
void tdsrlci_dl_am_handle_10ms_timer_cb(uint32 timer_sig);


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

/*===========================================================================

FUNCTION TDSRLCI_DL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION

This function is called when the qxdm log timer expires. This functionthen
prepares to send the packet to qxdm for logging purposes. TDSRLC_AM_LOG_TIMER_BASE
is currently set to 3seconds.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrlci_dl_am_handle_qxdm_log_timer(uint32 dl_log_timer_sig);

#endif  /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

#ifdef FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER
/*===========================================================================

FUNCTION  TDSRLC_DL_AM_BACKUP_TIMER

DESCRIPTION
  This function check the backup timer count. If it is greater than the
  hard-coded value (5). Start the 10ms timer for the AM channels.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_am_backup_timer(void);
#endif

/*==========================================================================

FUNCTION TDSRLC_PROCESS_MAC_HS_RESET_REQ

DESCRIPTION
  This function handles the MAC HS Reset command from MAC. RLC sends
  a status report on receiving this command even if any status prohibit
  timer is on.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

========================================================================*/
void tdsrlc_process_mac_hs_reset_req
(
  tdsmac_hs_dl_reset_req_type  *mac_hs_reset_req
);


/*===========================================================================

FUNCTION TDSCRLC_DL_ABORT_CIPHER_CONFIG

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

void tdscrlc_dl_abort_cipher_config
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info
);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION TDSRLC_DL_LOG_CIPHER_PKT

DESCRIPTION
This function logs the dowlink PDU ciphering parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_log_cipher_pkt
(
  void
);
#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */


/*===========================================================================

FUNCTION TDSCRLC_DL_WM_CLEANUP_CMD_HANDLER

DESCRIPTION
   this function cleanup the PDCP-RLC WM by a call back function.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

void tdscrlc_dl_wm_clean_up_cmd_handler
(
  tdsrlci_cleanup_wm_cmd_type *cleanup_wm_cmd
);

/*===========================================================================

FUNCTION TDSCRLC_DL_CONTINUE_HANDLER

DESCRIPTION
  Continue transmission on one or more downlink unacknowledged or acknowledged
  mode channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_continue_handler
(
  tdsrlc_dl_continue_type *continue_ptr      /* Ptr to continue Info                */
);




/*===========================================================================

FUNCTION tdsrlci_enh_dl_check_pdu_validity

DESCRIPTION
   Validate the PDU Contents.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrlci_enh_dl_check_pdu_validity
(
  tdsrlci_dl_ctrl_blk_type    *ctrl_blk,
  tds_complete_pdu_type        *complete_pdu_ptr,
  boolean                  *rxd_piggy_back, 
  tdsl1_dec_iovec_type *piggy_back_pdu
  , boolean alt_e_bit
);

/*===========================================================================

FUNCTION tdsrlci_get_li_value

DESCRIPTION
   Get the LI Value.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

uint16 tdsrlci_get_li_value(tds_complete_pdu_type *complete_pdu_ptr, uint16 offset,
                         uint16 li_len);

/*===========================================================================

FUNCTION tdsrlci_get_li_count

DESCRIPTION
   Get number of LI present.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

uint16 tdsrlci_get_li_count(dsm_item_type* dsm_ptr, uint16 offset, uint16 li_len, 
                         uint16 pdu_len);


/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler

DESCRIPTION
    - Process the R99 data, if received
       - Get the RLC Info from the tdsrlc_enh_data_queue. Protect this operation with 
         TDS_INTLOCK, as it this queue is accessed by MAC-HS also
       - After processing all the entries(PDUs) in the received rlc_enh_data_ptr,
         rlc_enh_data_ptr will be added to free queue
    - Process the HS data, if received
       - Get the TSN from the tdsrlc_tsn_info_q. Protect this operation with TDS_INTLOCK
         as it this queue is accessed by MAC-HS also
       - After processing all the entries(PDUs) in the received tsn_info_buf_ptr,
         tsn_info_buf_ptr will be added to free queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_dl_pdu_data_handler(void);

#if 0
//#ifdef FEATURE_Q6_MT
/*===========================================================================

FUNCTION rlc_enh_dl_data_reassembly_signal

DESCRIPTION  
  Go through all the AM Channels and if there is any old_vr_r which is different
  from vr_r, then there is some data left over and RLC can reassemble them.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Need to optimize the function such that, only AM channels will be used 
  in FOR loop, than all the logical channels
  Data in the first AM LC will be completed before moving to next AM LC,
  as Signal doesnt know about the LC which triggered this action

===========================================================================*/
void rlc_enh_dl_data_reassembly_signal(void);
#endif

/*===========================================================================

FUNCTION tdsrlci_discard_sdu_ptr

DESCRIPTION
    For every DSM Buffer/item in the packet chain, discard the buffer/item as 
    per conditions below
    If the DSM Buffer/item app_field is zero, 
       - DISCARD the DSM Buffer/item by calling dsmi_free_buffer
    Otherwise
       - Nothing

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_discard_sdu_ptr
(
 dsm_item_type **dsm_ptr,
 const char * file,
 uint32 line
);
/*===========================================================================

FUNCTION tdsrlc_discard_pdu

DESCRIPTION
    Discard the PDU if not NULL

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_discard_pdu
(
  tds_complete_pdu_type       *complete_pdu_ptr
);
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION tdsrlc_enh_logging_data_handler

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     
      If first_pdu_on_this_lch is TRUE, initialise the tdsnum_pdu to zero

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_logging_data_handler(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                  tds_complete_pdu_type *complete_pdu_ptr,
                                  boolean first_pdu_on_this_lch);

/*===========================================================================

FUNCTION tdsrlc_enh_logging_post_data_handler

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_logging_post_data_handler(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                       uint8 lc_id, boolean hs_path);

#endif

/*===========================================================================

FUNCTION tdsrlci_enh_update_nak_list

DESCRIPTION
    - As part of Post-Processing, Status message will be prepared as below
      - Prepare the LIST SUFI in which, 
        SN is the sequence number in which first hole is existing and 
        L is the number of consecutive holes present      

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_update_nak_list(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION tdsrlci_enh_status_generation

DESCRIPTION
    - As part of Post-Processing, Status message will be prepared
      - Copy NAK item from NAK_LIST_Q to TX_NAK_Q
      - Let the UL know that it has to send a status report to the peer entity
      - Clear the rcvd poll bit if set
      - Turn the status prohibit flag to TRUE to prohibit the sending further status

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_enh_status_generation(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION tdsrlci_enh_dl_reassembly_post_pdu

DESCRIPTION
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality which will work on the received PDUs in this call
       - If needed NAK list will be prepared
       - If needed, reassembly functionality is called to prepare SDUs out of
         received in-seq PDUs.
       - If needed and allowed, Status message will be prepared

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_dl_reassembly_post_pdu(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION tdsrlc_enh_rcvd_data_post_processing

DESCRIPTION
    - As part of the post_processing functionality
      - 1. If configured, Post the logging data
      - 2. For all the previously received PDUs on prev_lc_id, 
             For AM - Call the Post procesing function
             For UM - Reassembly is taken care during the PDU handling itself
                      If there is anything left over in dl_pdu_info_ptr 
                      for this LgCh,
                        - Update to partial_sdu_ptr
      - 3. If any SDU is enqueued into Water mark and Call back is registerd, 
           Call the callback function to post SDU data to Upper Layers

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_rcvd_data_post_processing(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
                                       uint8 prev_rlc_id, 
                                       boolean hs_path);


/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler_hs

DESCRIPTION
    - For every log Channel
       - Process each PDU one after another
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality (Reassembly, Status building, Status preperation)
      which will work on the received PDUs in this call
    - For UM and TM, during the PDU processing itself Post processing is completed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_dl_pdu_data_handler_hs(tdsl1_dec_hs_tsn_info_type *tsn_info_buf_ptr);


/*===========================================================================

FUNCTION tdsrlc_enh_handle_tm_data

DESCRIPTION
    - Process TM data, if present and configured to process in interrupt context
    - If some data present in TM with interrupt context TRUE, 
        - Process the TM PDUs
    - If some more PDUs are left in the rcvd, copy them into local buffer
      and back into rlc_enh_data_ptr
    - If nothing, return the entry to free queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_handle_tm_data(tdsrlc_enh_data_ind_type *rlc_enh_data_ptr);

/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler_non_hs

DESCRIPTION
    - For every log Channel
       - Process each PDU one after another
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality (Reassembly, Status building, Status preperation)
      which will work on the received PDUs in this call
    - For UM and TM, during the PDU processing itself Post processing is completed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_dl_pdu_data_handler_non_hs(tdsrlc_enh_data_ind_type  *rlc_enh_data_ptr);

/*===========================================================================

FUNCTION TDSRLCI_DEBUG_TRACE_PRINT_DATA

DESCRIPTION
  To print the contents of the data in the item_ptr.
  P.S: Needed trace length needs to be passed in number of words
  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_debug_trace_print_data(dsm_item_type *walk_item_ptr, 
                                 uint16         needed_trace_length);

/*===========================================================================

FUNCTION TDSRLCI_DBG_DUMP_DATA

DESCRIPTION
  To print the contents of the data in the item_ptr.
  P.S: Needed trace (offset,length) needs to be passed in number of bytes
  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dbg_dump_pdu(dsm_item_type *dsm_ptr, uint16 offset, uint16 len);

/*===========================================================================

FUNCTION TDSDL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Downlink control block for an RLC entity.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 tdsdl_assign_ctl_blk(void);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION TDSRLCI_GENERATE_DL_CONFIG_EVENT

DESCRIPTION
  To generate an event whenever a DTCH logical channel is established, modified
  or restablished, to aid in automation. The argument rlc_size is used only
  in the case the UM RLC entity to pass the size to this function, as this
  information is not available in the control block.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_generate_dl_config_event(tdsrlci_dl_ctrl_blk_type *ctrl_blk,
                                   uint32 rlc_size);
#endif /*FEATURE_QXDM_LOGGING*/

#endif

