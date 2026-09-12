#ifndef RLCDL_H
#define RLCDL_H
/*===========================================================================
                  D O W N L I N K  R L C  I N T E R N A L

                         H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  downlink RLC module.

Copyright (c) 2001 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/rlc/inc/rlcdl.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who      what, where, why
--------  ---      ----------------------------------------------------------
04/22/16  scv      Migrate RLC code to new requirements for DSM Priority Field
04/22/16  as       RLC to discard TB decode in Header Read when RLC is in STOP mode.
09/09/15  scv      RLC to process DL TM PDUs in reverse order only in ISR context
07/20/15  scv      Changes for DSM Multiple DeQ/EnQ
06/26/15  aa       Removing extern declarations from L2 code
04/29/15  scv      FR 25842: RLC RTT & Queuing Delay Computation
04/25/15  scv      W+W Changes to have messages support SubID
01/29/15  aa       L2 global reorg for W+W
01/08/15  kv       Replace __FILE__ with __FILENAME__ 
10/17/14  ts       Fix to discard DL PDUs received while in the Reset Pending State, to avoid potential ciphering mismatch right after the recovery
08/21/14  ts       FR22375: Fixed Medium compiler warnings in WCDMA on Bolt 2.0/2.1
01/09/14  scv      Code changes for UL Compression Feature
05/08/13  scv      Fix L2 Compilation Errors
05/03/13  as       Change to discard DL PDUs if PDU health passed by decoder is false
01/21/13  as       Added code check to enqueue DL data to Watermark queue only 
                   if its length is less than 1503 bytes.
01/17/13  as       Removed unwanted F3's.
01/04/13  kc       Prototypes for ZI reduction function
05/24/12  kc       Added changes for QCHAT HFN mismatch error recovery
07/13/12  grk      MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
07/24/12  geg      Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
04/24/12  kc       Moving WM deregistration prototypes to rlcrrcif.h
02/01/12  grk      Feature cleanup
12/06/10  grk      Added periodic PS<->RLC DL WM traces at RLC
10/11/11  grk      Fix to move PDU discarding(when WM is not registered) for TM
                   PDUs from ISR to task context.
08/17/11  grk      Fix to trigger UE initiated RESET if HFN value in RESET/RESET_ACK
                   received from NW is lower than the one currently configured.
03/02/11  kvk      Added support for PDU/SDU print and also Invalid SDU detection >1500
09/30/10  pj       moved enum definitions to rlci.h from rlcdl.h to avoid mob error 
09/20/10  pj       Added support to log erroneous conditions
07/07/10  pj       Added changes to use a global base timer for status processing
06/28/10  pj       Added changes for ZI memory reduction.
05/12/10  grk      Reorganized part of code under FEATURE_INACTIVITY_HANDLING.
05/06/10  pj       Added function prototypes 
04/13/10  grk      changes for ZI memory reduction.
12/07/09  grk      Changes to return valid deciphering parameters for PDUs 
                   received before receiving RESET_ACK (in case DL is in 
                   RESET_PENDING due to UE initiated RESET) and drop PDUs received 
                   after receiving RESET_ACK till DL comes out of RESET_PENDING.
10/21/09  grk      1.Added new DL state variable to keep track of SN till which
                     DL status array is updated dl_vr_h_status. 
                   2.Fix to use vr_r as last NAK SN in case dl_vr_h_status is less 
                     than vr_r.
08/13/09   grk     Changes for bitmap based status tracking.
04/13/09   pj      Added support for CS-over-HS
06/10/09   grk     Changes for ZI memory reduction on DL.
06/12/09   av      Added feature to genrate RLC event during DL config
05/29/09   ssg     Added support for SNOW_3G algo.
05/08/09   av      Enhancement to detect spurios reset pdu in srb 
05/11/09   ssg     Updated Copyright Information
04/29/09   av      Fixed compiler warnings in L2 
01/28/09   grk     Merged missed out fixes from taxis to mainline.
01/22/09   grk     Merged L2 mutex changes from taxis to mainline.
03/03/08   ssg     Added support to alternate e-bit interpretations under the
                   featue flag FEATURE_RLC_ALTERNATE_E_BIT.
12/19/08   ssg     Added prototype for rlc_enh_dl_data_reassembly_signal().
12/17/08   ksr     PDU boundary check while counting LI in PDU
12/05/08   grk     compilation fix for 8200 (rel7).
11/22/08   kvk     Now sharing Mutex with L1 for all L2 to avoid memory concurrency issues
11/21/08   kvk     rlci_get_li_count() will now take pdu_len to check PDU validity                  
11/13/08   pj      Moved prototype for rlci_chk_dl_reset_pending() to rlci.h
10/23/08   pj      Added prototype for rlci_chk_dl_reset_pending() function 
10/17/08   grk     Merged taxis changes to mainline.
09/02/08   cnp     Replaced intlock/free with wcdma_intlock/free.
           cnp     Replaced tasklock/free with wcdma_tasklock/free.
           cnp     Replaced intlock/free_sav with wcdma_intlock/free_sav.

05/20/08   kvk     Added dl_ctl_cnfg_local and am_common_config to store the
                   config locally
05/19/08   kvk     Moved RLCI_GET_CTRL_BLK_INDEX and RLCI_GET_BLK_PTR out of 
                   FEATURE_WCDMA_DL_DATA_PATH_OPT feature
05/16/08   ssg     Added prototype for crlc_dl_continue_handler().
04/25/08   ksr     app_field is already discounted while putting in PDU into SDU pointer. 
                   While discarding SDU, discard mechanism is updated to discard only 
                   when app_field is 0
03/24/08   pj      Added support for data path optimizations under 
                   FEATURE_WCDMA_UL_DATA_PATH_OPT & FEATURE_WCDMA_DL_DATA_PATH_OPT
           vg      1. Fixed issues during DL integration testing
           ar      2. a. Added macros for enabling/disabling UM user plane log packets
                         feature FEATURE_WCDMA_DL_DATA_PATH_OPT
                      b. Modified macro RLCI_GET_AM_LOGGING_CONFIG under feature
                         FEATURE_RLC_QXDM_LOGGING
02/01/08   pj      Changed FEATURE_MBMS flag to FEATURE_MODEM_MBMS.
11/30/07   ssg     Added mbms support under FEATURE_MBMS.
01/11/07   vg      added UM PDU logging structure in UM ctrl type
01/04/07   ssg     Added rlc_dl_log_cipher_pkt() function prototype.
11/10/06   vg      added prototype of WM clenup function
05/17/06   ssg     Added the variable cur_status_periodic in the struct
                   rlci_dl_am_state_type
08/15/05   mv      Added the variable report_vr_h in the struct
                   rlci_dl_am_state_type under the feature
                   FEATURE_RLC_REPORT_VR_H_ACK_SN.
05/31/05   mv      Defined RLC_DL_AM_USR_PLANE_START_LC_ID as the start for
                   user/data plane AM logical channels.
05/03/05   mv      Added the function definition for
                   crlc_dl_abort_cipher_config.
                   Added the parameter cipher_backup to the structure
                   rlci_dl_ctrl_blk_type.
5/01/05    mv      Fixed a bug in GET16 macro.
04/05/05   aw      Removed Log_on_demand feature.
03/07/05   mv/aw   Merged the HSDPA changes to mainline.
02/09/05   aw      Converted get16 function to GET16 macro.
1/27/05    mv      Removed the include file
06/08/04   mv      Increased the max receive window size from 768 to 2048.
06/02/04   mv      Fixed lint errors and warnings.
01/20/04   mv      Replaced the FEATURE_RLC_ENHANCED_CB with the
                   FEATURE_RLC_SDU_RCVD_ENHANCED_CB.
12/29/03   vsk     Replace FEATURE_MULTIPLE_PDP with FEATURE_RLC_ENHANCED_CB
12/06/03   ttl     Added data structure for PDU ciphering parameters logging.
10/14/03   mv      Added a variable rx_win to indicate the receiver's window
                   size on the downlink.
09/23/03   mv      Added a third parameter to the rlc_post_rx_func_ptr()
                   to support multiple PDP contexts under the feature
                   FEATURE_MULTIPLE_PDP.
09/04/03   ttl     Added fst_rx_pdu.
03/28/03   ttl     Changed UE_MAX_DL_RLC_ENTITY definition to be
                   UE_MAX_DL_LOGICAL_CHANNEL.
02/14/03   ttl     Removed duplicated declaration of rlc_dl_register_srvc_handler().
                   Removed rlc_dl_pdu_data_ind() definition due to the duplication
                   in macrlcif.h.
01/14/03   ttl     Added nak_list_q in rlci_dl_am_ctrl_type.
12/10/02   ttl     Added backup_timer_cnt for backup timer.
                   Added function declaration of rlc_dl_am_backup_time().
11/13/02   ttl     Added next_to_vr_r.
11/05/02   ttl     Added peer_reset_sn to rlci_dl_am_state_type.
10/25/02   ttl     Added FEATURE_DATA_MM to exclude the get16 if not defined.
                   and include dsbyte.h, otherwise.
09/16/02   ttl     Added last_sn_in_q in rlci_dl_am_state_type.
09/05/02   ttl     Added Ciphering feature.
07/31/02   ttl     Rewrote 10ms base timer, removed timer sig, using timer
                   callback function instead.
                   Removed rlci_dl_am_get_timer_sig(), rlci_dl_am_timer_sig_to_rlc_id(),
                   rlci_dl_am_release_timer_sig().
06/12/02   ttl     Restructure rlc_dl_chan_am_stat_type.
                   Added structure rlc_dl_chan_am_pdu_type.
05/13/02   ttl     Added DL_STOP to internal states.
05/08/02   ttl     Added 2nd parameter for rlc_post_rx_func_ptr().
04/24/02   ttl     Rewrote the statistic log code.
04/03/02   ttl     Defined a structure for Log_on_demand feature.
03/15/02   sk      Added structure rlc_dl_chan_am_stat_type for collecting the
                   statistics information for a single AM entity. Added the
                   field rlc_dl_chan_am_stat_type to rlci_dl_am_ctrl_type
                   structure.
02/13/02   ttl     Added service de-registration function.
11/09/01   sk      Added prev_vr_h ,status_periodic_cnt,status_prohibit_cnt,
                   status_periodic to rlci_dl_am_state_type structure.Added
                   timer_blk to rlci_dl_am_ctrl_blk_type.
11/04/01   ttl     Removed application/voice service callback function feature.
10/26/01   ttl     Added application/voice service callback function feature.
08/20/01   rc      Added support for QXDM logging.
06/18/01   rc      Added support for Processing Downlink TM PDUs in
                   both Task and Interrupt Context.
06/07/01   rc      Added prototype for crlc_dl_release_all_handler().
06/07/01   rc      Removed temporary fix for VST problem.
06/06/01   rc      Added fields discard_flag, nlen and change_wins to
                   rlci_dl_am_state_type.
                   Added a temporary fix for a VST problem.
05/21/01   rc      Added more comments.
05/02/01   ttl     Added definition for rlci_dl_proc_reset_msg().
03/27/01   ttl     Added rlc_register_am_sdu_discard_cb_req().
                   Changed the max of control block for RLC to
                   UE_MAX_DL_RLC_ENTITY.
02/28/01   ttl     Added support for Acknowledged Mode.
02/13/01   rc      Removed maccommdefs.h. Changed MAC_DL_MAX_RLC_PDUS to
                   UE_MAX_DL_RLC_PDUS.
01/08/01   rc      Modified Code to use data types defined in uecomdef.h
11/22/00   rc      Created file.

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/


#include "wcdma_variation.h"
#include "uecomdef.h"
#include "queue.h"
#include "memory.h"
#include "msg.h"
#include "err.h"
#include "assert.h"
#include "rex.h"
#include "rlcrrcif_v.h"
#include "rlcdsapif.h"
#include "macrlcif.h"
#include "rlci.h"
#include "l2dltask.h"
#include "l2ultask.h"
#include "rlclog.h"
#include "l1macif.h"
#include "stringl.h"
#include "modem_mem.h"
#include "rlcul.h" /* kchaitan */
#define MAX_RX_WINDOW_SIZE 2048

#define RLC_WM_TRACE_CTR 50

/*---------------------------------------------------------------------------
  The user/data plane logical channel ids start from the value that is
  calculated by RRC as MAX DL TM channels + MAX DL UM channels +
  MAX AM Signalling channels.
---------------------------------------------------------------------------*/

#define RLC_DL_AM_USR_PLANE_START_LC_ID (UE_MAX_DL_TM_CHANNEL + \
                     UE_MAX_DL_UM_CHANNEL + UE_MAX_SRB_AM_EMTITIES_3)

#define RLC_DL_AM_START_LC_ID (UE_MAX_DL_TM_CHANNEL + \
                     UE_MAX_DL_UM_CHANNEL)

#define RLC_AM_HALF_MOD_MASK 0x07FF
/* Bitmask for enabling/disabling UM user plane log packets */
#define RLCI_UM_USR_PLANE_LOG_MASK 0x01

/* Bitmask for enabling/disabling AM user plane log packets */
#define RLCI_AM_USR_PLANE_LOG_MASK 0x02

/* Bitmask for enabling/disabling AM signalling plane log packets */
#define RLCI_AM_SIG_PLANE_LOG_MASK 0x04
 
#define RLCI_INVALID_SEQ_NUM 0xFFFF
#define RLCI_MAX_LI_IN_LIST 15

#define RLC_ENTRY "RLC DL ENTRY"
#define RLC_DUP "RLC DL DUP"
#define RLC_EXIT "RLC DL EXIT"

#ifdef FEATURE_QCHAT_HFN_MISMATCH_RECOVERY
/* Threshold value for counter that tracks continuous PDU error */
#define RLC_PDU_CONT_ERR_CNT_TRSHLD1 1
#define RLC_PDU_CONT_ERR_CNT_TRSHLD2 10
#endif /*FEATURE_QCHAT_HFN_MISMATCH_RECOVERY*/
 
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
/* Bitmask for enabling/disabling AM log packets for R7 */
#define RLCI_AM_R7_LOG_MASK 0x08
#endif /*((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))*/

#define RLCI_GET_RESEQ_Q_IDX(sn) ((sn) & RLC_AM_HALF_MOD_MASK)

#define RLCI_GET_PDU_DATA_PTR(pdu_ptr, offset) (pdu_ptr + offset)

#define RLCI_GET_DC_BIT(pdu_hdr)  (pdu_hdr & 0x8000)

#define RLCI_GET_AM_SN(pdu_hdr)   ((pdu_hdr & 0x7fff) >> 3)

#define RLCI_GET_POLL_BIT(pdu_hdr)  ((pdu_hdr & 0x0004) >> 2)

#define RLCI_GET_H_EXT_BIT(pdu_hdr)  (pdu_hdr & 0x0003)

#define RLCI_GET_LI_EXT_BIT(pdu_hdr) (pdu_hdr & 0x01)

#define RLCI_GET_CTRL_PDU_TYPE(pdu_hdr)  (pdu_hdr & 0x07)

#define RLCI_GET_ACCUM_LI(pdu_hdr)   (pdu_hdr >> 1)

#define RLCI_GET_INC_Q_IDX(idx)   ((idx+1) & RLC_AM_HALF_MOD_MASK)

#define RLCI_GET_RESET_SN(data)  ((data & 0x08) >> 3)

#define RLCI_GET_RESET_RESV(data)   (data & 0x07)

#define RLCI_GET_LI_VAL(li)    (li >> 1)

#define RLC_GET_ABS_LI(walk_item_ptr, offset, li_len, accum_li) \
 (li_len == RLC_LI_SHORT) ? (accum_li = walk_item_ptr->dsm_ptr->data_ptr[offset]) \
                          : (accum_li = (((uint16) walk_item_ptr->dsm_ptr->data_ptr[offset] << 8) | ((uint16)walk_item_ptr->dsm_ptr->data_ptr[offset + 1])))


#define RLCI_GET_UM_SN(pdu_hdr)   ((pdu_hdr & 0xFF) >> 1)

#define RLC_DISCARD_DL_AM_PDU  (1 << L2_PDU_DISCARD_HDR_MSB_POS)

#define rlc_discard_sdu_ptr(item_ptr) \
             rlci_discard_sdu_ptr(rlc_ul_table_ptr, item_ptr, __FILENAME__, __LINE__)

#ifdef FEATURE_RLC_QXDM_LOGGING
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
#define RLCI_GET_AM_LOGGING_CONFIG(rlc_dl_table_ptr, lc_id) \
      (((lc_id >= RLC_DL_AM_USR_PLANE_START_LC_ID) && \
        ((rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_USR_PLANE_LOG_MASK) || \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_R7_LOG_MASK))) || \
       ((lc_id < RLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_SIG_PLANE_LOG_MASK)))

#define RLCI_GET_AM_R7_LOG_ENABLED_THIS_LC(rlc_dl_table_ptr, lc_id) \
        ((lc_id >= RLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_R7_LOG_MASK))

#define RLCI_GET_AM_R7_LOG_ENABLED(rlc_dl_table_ptr) \
        (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_R7_LOG_MASK)
#else
#define RLCI_GET_AM_LOGGING_CONFIG(rlc_dl_table_ptr, lc_id) \
      (((lc_id >= RLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_USR_PLANE_LOG_MASK)) || \
        ((lc_id < RLC_DL_AM_USR_PLANE_START_LC_ID) && \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_AM_SIG_PLANE_LOG_MASK)))
#endif  /*((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))*/

#define RLCI_GET_UM_LOGGING_CONFIG(rlc_dl_table_ptr) \
         (rlc_dl_table_ptr->rlci_log_mask & RLCI_UM_USR_PLANE_LOG_MASK)

#define RLC_AM_STAT_INC_ERR_PDU(macro_var)  macro_var++

#define RLC_AM_STAT_INC_CTRL_PDU(macro_var) macro_var++

#define RLC_AM_STAT_INC_DATA_PDU(macro_var) macro_var++

#define RLC_AM_STAT_UPD_NUM_PDU_BYTE_RCVD(macro_var, length) \
            macro_var += length

#define RLC_AM_STAT_UPD_NUM_SDU_BYTE_RCVD(macro_var, length) \
            macro_var += length
#endif /*FEATURE_RLC_QXDM_LOGGING*/

/*===========================================================================

MACRO GET16

DESCRIPTION
  This macro return the word value from the 2 byte value

DEPENDENCIES
  None

RETURN VALUE
  A word representing the 2 bytes pointed at by the passed parameter

SIDE EFFECTS
  None
===========================================================================*/

#define GET16(cp) (                             \
                      ((( cp[0]) << 8) | cp[1])   \
                  )

/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/
typedef struct {
  uint8 rlc_id;
  uint8 npdus;
  uint16 pdu_size;
  dsm_item_type *dsm_ptr[UE_MAX_DL_RLC_PDUS];
}rlci_dl_chan_data_type;

typedef struct {
  uint8 nchan;
  rlci_dl_chan_data_type chan_info[UE_MAX_DL_LOGICAL_CHANNEL];
} rlci_dl_frame_data_type;

/*===========================================================================

FUNCTION RLCI_GET_DL_CTL_BLK_PTR_BY_INDEX

DESCRIPTION
  Returns Ctrl Blk Ptr from downlink table for specified index 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlci_dl_ctrl_blk_type *rlci_get_dl_ctl_blk_ptr_by_index 
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  uint8 index
);

/*===========================================================================

FUNCTION RLCI_GET_DL_CTL_BLK_INDEX_FOR_RLC_ID

DESCRIPTION
  Returns index of ctrl blk in downlink table for specified RLC ID 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 rlci_get_dl_ctl_blk_index_for_rlc_id 
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type rlc_id
);

/*===========================================================================

FUNCTION RLCI_GET_DL_CTL_BLK_PTR_BY_RLC_ID

DESCRIPTION
  Returns Ctrl Blk Ptr from downlink table for specified RLC ID 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlci_dl_ctrl_blk_type *rlci_get_dl_ctl_blk_ptr_by_rlc_id 
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type rlc_id
);

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
void rlci_dl_am_base_timer_handler (rlci_uplink_table_type *rlc_ul_table_ptr, rlci_downlink_table_type *rlc_dl_table_ptr, mac_ul_table_type *ul_table_ptr );
/*===========================================================================

FUNCTION RLC_DL_INIT

DESCRIPTION
  Initializes the RLC Downlink Logical channel table.

DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_dl_init
(
  sys_modem_as_id_e_type as_id
);

/*===========================================================================

FUNCTION CRLC_DL_CIPHER_CONFIG_HANDLER

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
void crlc_dl_cipher_config_handler
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_dl_cipher_config_type *cipher_ptr   /* Ptr to Downlink Cipher Config */
);
/*===========================================================================

FUNCTION CRLC_DL_HFN_HANDLER

DESCRIPTION
 This function handles the request for the Hyperframe number by RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void crlc_dl_hfn_handler
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_dl_hfn_type *hfn_ptr                 /* Ptr to HFN info              */
);

/*===========================================================================

FUNCTION CRLC_DL_HFN_CNF

DESCRIPTION
 This function sends the hyperframe number of the requested channels to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void crlc_dl_hfn_cnf
(
  sys_modem_as_id_e_type as_id,
  rlc_dl_hfn_cnf_type *hfn_ptr             /* Ptr to HFN CNF info.         */
);


/*===========================================================================

FUNCTION RLC_DL_PDU_DATA_HANDLER

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
void rlc_dl_pdu_data_handler(void);

/*===========================================================================

FUNCTION RLC_REGISTER_AM_SDU_DISCARD_CB_REQ

DESCRIPTION
  Register AM SDU discard callback function.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_register_am_sdu_discard_cb_req
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_register_am_sdu_discard_callback_type *dl_sdu_discard_cb
);

/*===========================================================================

FUNCTION CRLC_DL_TM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Transparent Mode Logical
  channels. This function is called when RRC sends a CRLC_DL_TM_CONFIG_REQ
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
void crlc_dl_tm_config_handler
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_dl_tm_config_type *tm_ptr /* Pointer to TM config info               */
);


/*===========================================================================

FUNCTION CRLC_DL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical
  channels. This function is called when RRC sends a CRLC_DL_UM_CONFIG_REQ
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
void crlc_dl_um_config_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_dl_um_config_type *um_ptr /* Pointer to UM config info               */
);

/*===========================================================================

FUNCTION CRLC_DL_AM_CONFIG_HANDLER
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

void crlc_dl_am_config_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlci_dl_am_config_req_type *dl_am_config_req_ptr
);

/*===========================================================================

FUNCTION RLC_DL_REGISTER_SRVC_HANDLER_SUB

DESCRIPTION
  This function handles the registration of the watermark and the callback
  function for the logical channels on the downlink.
===========================================================================*/
void rlc_dl_register_srvc_handler_per_sub
(
  sys_modem_as_id_e_type as_id,
  rlc_dl_register_srvc_type *reg_ptr      /* Ptr to WM registration Info   */
);

/*===========================================================================

FUNCTION RLC_DL_DEREGISTER_SRVC_HANDLER_SUB

DESCRIPTION
  This function handles the de-registration of the watermark and the callback
  function for the logical channels on the downlink.
===========================================================================*/
void rlc_dl_deregister_srvc_handler_per_sub
(
  sys_modem_as_id_e_type as_id,
  rlc_dl_deregister_srvc_type *de_reg_ptr      /* Ptr to WM registration Info   */
);

/*===========================================================================

FUNCTION RLCI_DL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_UL_RESET_DONE and RLCI_DL_RESET_REQ messages from
  UL AM RLC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_dl_proc_reset_msg
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  wcdma_l2_dl_cmd_enum_type cmd_id,
  rlc_lc_id_type dl_rlc_data_id
);

/*===========================================================================

FUNCTION CRLC_DL_RELEASE_ALL_HANDLER

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
void crlc_dl_release_all_handler(rlci_uplink_table_type *rlc_ul_table_ptr, rlci_downlink_table_type *rlc_dl_table_ptr);

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
void rlci_dl_am_handle_10ms_timer_cb(uint32 timer_sig);

/*===========================================================================

FUNCTION  RLC_DL_AM_BACKUP_TIMER

DESCRIPTION
  This function check the backup timer count. If it is greater than the
  hard-coded value (5). Start the 10ms timer for the AM channels.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_dl_am_backup_timer(rlci_downlink_table_type *rlc_dl_table_ptr);

/*==========================================================================

FUNCTION RLC_PROCESS_MAC_HS_RESET_REQ

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
void rlc_process_mac_hs_reset_req
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  mac_hs_dl_reset_req_type  *mac_hs_reset_req
);


/*===========================================================================

FUNCTION CRLC_DL_ABORT_CIPHER_CONFIG

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

void crlc_dl_abort_cipher_config
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_abort_cipher_config_type *abort_cipher_info
);

/*===========================================================================

FUNCTION CRLC_DL_WM_CLEANUP_CMD_HANDLER

DESCRIPTION
   this function cleanup the PDCP-RLC WM by a call back function.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

void crlc_dl_wm_clean_up_cmd_handler
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlci_cleanup_wm_cmd_type *cleanup_wm_cmd
);

/*===========================================================================

FUNCTION CRLC_DL_CONTINUE_HANDLER

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
void crlc_dl_continue_handler
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_dl_continue_type *continue_ptr      /* Ptr to continue Info                */
);

/*===========================================================================

FUNCTION rlci_enh_dl_check_pdu_validity

DESCRIPTION
   Validate the PDU Contents.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

boolean rlci_enh_dl_check_pdu_validity
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_dl_ctrl_blk_type    *ctrl_blk,
  complete_pdu_type        *complete_pdu_ptr,
  boolean                  *rxd_piggy_back, 
  mcalwcdma_dec_iovec_type *piggy_back_pdu
  , boolean alt_e_bit
);

/*===========================================================================

FUNCTION rlci_get_li_value

DESCRIPTION
   Get the LI Value.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_get_li_value(complete_pdu_type *complete_pdu_ptr, uint16 offset,
                         uint16 li_len);

/*===========================================================================

FUNCTION rlci_get_li_count

DESCRIPTION
   Get number of LI present.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_get_li_count(dsm_item_type* dsm_ptr, uint16 offset, uint16 li_len, 
                         uint16 pdu_len, sys_modem_as_id_e_type as_id);


/*===========================================================================

FUNCTION rlc_enh_dl_pdu_data_handler

DESCRIPTION
    - Process the R99 data, if received
       - Get the RLC Info from the rlc_enh_data_queue. Protect this operation with 
         WCDMA_INTLOCK, as it this queue is accessed by MAC-HS also
       - After processing all the entries(PDUs) in the received rlc_enh_data_ptr,
         rlc_enh_data_ptr will be added to free queue
    - Process the HS data, if received
       - Get the TSN from the rlc_tsn_info_q. Protect this operation with WCDMA_INTLOCK
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
void rlc_enh_dl_pdu_data_handler(rlci_uplink_table_type *rlc_ul_table_ptr, rlci_downlink_table_type *rlc_dl_table_ptr, mac_ul_table_type *ul_table_ptr );

/*===========================================================================

FUNCTION rlci_discard_sdu_ptr

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
void rlci_discard_sdu_ptr
(
 rlci_uplink_table_type *rlc_ul_table_ptr,
 dsm_item_type **dsm_ptr,
 const char * file,
 uint32 line
);

/*===========================================================================

FUNCTION rlc_discard_pdu

DESCRIPTION
    Discard the PDU if not NULL

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rlc_discard_pdu
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  complete_pdu_type       *complete_pdu_ptr
);

/*===========================================================================

FUNCTION rlci_enh_update_nak_list

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
void rlci_enh_update_nak_list(rlci_uplink_table_type *rlc_ul_table_ptr, rlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION rlci_enh_status_generation

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

void rlci_enh_status_generation(rlci_uplink_table_type *rlc_ul_table_ptr,
                                    rlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION rlci_enh_dl_reassembly_post_pdu

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
void rlci_enh_dl_reassembly_post_pdu(rlci_uplink_table_type *rlc_ul_table_ptr,
                                            rlci_downlink_table_type *rlc_dl_table_ptr,
                                            rlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION rlc_enh_rcvd_data_post_processing

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

void rlc_enh_rcvd_data_post_processing(rlci_uplink_table_type *rlc_ul_table_ptr,
                                       rlci_downlink_table_type *rlc_dl_table_ptr,
                                       rlci_dl_ctrl_blk_type *ctrl_blk_ptr,
                                       uint8 prev_rlc_id, 
                                       boolean hs_path, mac_ul_table_type *ul_table_ptr );

/*===========================================================================

FUNCTION rlc_enh_dl_pdu_data_handler_hs

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

void rlc_enh_dl_pdu_data_handler_hs(rlci_uplink_table_type *rlc_ul_table_ptr,
                                    rlci_downlink_table_type *rlc_dl_table_ptr,
                                    l1_dec_hs_tsn_info_type *tsn_info_buf_ptr, mac_ul_table_type *ul_table_ptr );

/*===========================================================================

FUNCTION rlc_enh_handle_tm_data

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
void rlc_enh_handle_tm_data
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_enh_data_ind_type *rlc_enh_data_ptr
);

/*===========================================================================

FUNCTION rlc_enh_dl_pdu_data_handler_non_hs

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

void rlc_enh_dl_pdu_data_handler_non_hs(rlci_uplink_table_type *rlc_ul_table_ptr, rlci_downlink_table_type *rlc_dl_table_ptr,
                                        rlc_enh_data_ind_type  *rlc_enh_data_ptr, mac_ul_table_type *ul_table_ptr );

/*===========================================================================

FUNCTION DL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Downlink control block for an RLC entity.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 dl_assign_ctl_blk(rlci_downlink_table_type *rlc_dl_table_ptr);

/*===========================================================================

FUNCTION RLC_UL_ENQ_DSM_ITEM_TO_WM

DESCRIPTION
  API to check for SDU length boundary check and enqueue to DL WM queue.

DEPENDENCIES
  None.

RETURN VALUE
  Returns SDU length that is enqueued to WM
  Returns 0 if SDU is not within boundary.

SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlc_ul_enq_dsm_item_to_wm 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type         rlc_id,
  rlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
  dsm_item_type         *sdu_ptr
);

/*===========================================================================

FUNCTION RLC_UL_AM_CHAIN_DSM_ITEM

DESCRIPTION
  Function to check for SDU length boundary check and chain the DSM item to 
  chain_tail_ptr if SDU length is valid.
  
DEPENDENCIES
  None.

RETURN VALUE
  Returns SDU Length if valid, else returns 0.
  
SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_ul_am_chain_dsm_item
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_dl_ctrl_blk_type  *ctrl_blk_ptr,
  dsm_item_type          *sdu_ptr,
  dsm_item_type         **chain_head_ptr,
  dsm_item_type         **chain_tail_ptr
);

/*===========================================================================

FUNCTION RLCI_DL_EMPTY_SDUS_IN_WM

DESCRIPTION
  This function removes all the pending SDUs from the DL WM. 
===========================================================================*/
void rlci_dl_empty_sdus_in_wm

(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_dl_ctrl_blk_type *ctrl_blk_ptr,
  uint8 rlc_id
);

/*===========================================================================
FUNCTION: RLC_DL_ALLOCATE_MEM_ZI_OPT

DESCRIPTION:
  This function dynamically allocates memory to RLC-DL data structures
===========================================================================*/
void rlc_dl_allocate_mem_ZI_opt(rlci_downlink_table_type *rlc_dl_table_ptr);

/*===========================================================================
FUNCTION: RLC_DL_DEALLOCATE_MEM_ZI_OPT

DESCRIPTION:
  This function frees the RLC-DL dynamic allocated data structures
===========================================================================*/
void rlc_dl_deallocate_mem_ZI_opt(rlci_downlink_table_type *rlc_dl_table_ptr);

#ifdef FEATURE_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLCI_DL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION

This function is called when the qxdm log timer expires. This functionthen
prepares to send the packet to qxdm for logging purposes. RLC_AM_LOG_TIMER_BASE
is currently set to 3seconds.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void rlci_dl_am_handle_qxdm_log_timer(rlci_downlink_table_type *rlc_dl_table_ptr, uint32 dl_log_timer_sig);

/*===========================================================================

FUNCTION RLC_DL_LOG_CIPHER_PKT

DESCRIPTION
This function logs the dowlink PDU ciphering parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_dl_log_cipher_pkt
(
  rlci_downlink_table_type *rlc_dl_table_ptr
);

/*===========================================================================

FUNCTION rlc_enh_logging_data_handler

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     
      If first_pdu_on_this_lch is TRUE, initialise the num_pdu to zero

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_enh_logging_data_handler(rlci_uplink_table_type *rlc_ul_table_ptr,
                                  rlci_downlink_table_type *rlc_dl_table_ptr,
                                  rlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                  complete_pdu_type *complete_pdu_ptr,
                                  boolean first_pdu_on_this_lch);

/*===========================================================================

FUNCTION rlc_enh_logging_post_data_handler

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_enh_logging_post_data_handler(rlci_downlink_table_type *rlc_dl_table_ptr,
                                rlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                       uint8 lc_id, boolean hs_path);

/*===========================================================================

FUNCTION RLCI_GENERATE_DL_CONFIG_EVENT

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

void rlci_generate_dl_config_event(rlci_dl_ctrl_blk_type *ctrl_blk,
                                   uint32 rlc_size,
                                   rlci_downlink_table_type *rlc_dl_table_ptr);

#endif  /* FEATURE_RLC_QXDM_LOGGING */

#endif /*RLCDL_H*/

