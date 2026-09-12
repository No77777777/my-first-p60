#ifndef RLCI_H
#define RLCI_H
/*===========================================================================
              R L C  I N T E R N A L  H E A D E R  F I L E

DESCRIPTION
  This file contains RLC internal data types and declarations that are shared 
  within RLC layer.
  
Copyright (c) 2001-04 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2005-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/



/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   
$Header: //components/rel/wcdma.mpss/8.4.0/rlc/inc/rlci.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------- 
01/16/18   sg      Made changes for to log interlayer commands for ML
05/11/16  as      RLC to clear pending partials correctly during RLC RESET.
04/06/16  scv     Reduce logging in DACC Datapath & Config
09/09/15  scv     RLC to process DL TM PDUs in reverse order only in ISR context
08/06/15  kv      W+W sub-ID support while printing Events
06/26/15  aa      Removing extern declarations from L2 code
06/02/15  scv     W+W: F3 changes to append incremented SubID
05/22/15  kv      PDU log packet enahncements in UL and DL (CR - 746106)
05/20/15  aa      W+W and new interface changes for CFCM
05/12/15  aa      Changes to inform NAS when UM PDU > 6 bytes is received on DL CTCH channels
05/01/15  scv     W+W Changes to have log packets support SubID
04/25/15  scv     W+W Changes to have messages support SubID
02/23/15  kv      Enable 0x4168/0x4169 logging when turned during a call and print ciphering params if wrong LI
01/29/15  aa      L2 global reorg for W+W
01/13/15  aa      Changes for new ULC log packet(0x41C5)
11/26/14  scv     Forgive BAD ACK/NAK for first 200ms after Establishment/Re-Establishment
10/16/14  as      Added code changes to move back to configured window size whenever there is change 
                  in RLC window size and FC is active by posting CFCM_OFF command internally.
07/01/14  aa      CFCM changes for RLC
06/11/14  scv     Fix for HFN desynchronization when HFN initialization and rollover happen together
04/02/14  scv     Code changes for UL Compression Feature
01/30/14  kv      0x421E/0x4220 Logging Fixes for 3C and WM debug prints
01/28/14  scv     Code changes for UL Compression Feature
01/09/14  scv     Code changes for UL Compression Feature
11/27/13  scv     Code changes for UL Compression feature
06/28/13  as      Added code to validate bitmap sufi status PDU correctly.
05/20/13  as      Added code to validate STATUS PDU before processing it.
01/21/13  as      Added code check to enqueue DL data to Watermark queue only 
                  if its length is less than 1503 bytes.
01/04/13   kc     Removing duplicate/redundant code
09/21/12   as     Cleaned up code with proper identation and added missing function headers. 
07/16/12  mrg     MAC_I & HS-RACH Compilation Warnings fixed
07/18/12  as      Fix to trigger RACH procedure whenever there is pending data in L2
                  watermark queue when L1 is in FACH state
07/13/12  grk     MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
08/13/12   mrg    HS-RACH & MAC-I Feature updated code check-in.
07/24/12  geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/18/12  mrg     L2 - MAC-I and HS-RACH code changes
04/26/12  grk     Added new API file exposing L2 command structures
04/26/12  grk     Mainlined code under FEATURE_INACTIVITY_HANDLING.
02/14/11  grk     Added logic to retransmit PDUs not acknowledged at poll timer
                  expiry instead retransmitting vt_s-1 always.
02/01/12  grk      Feature cleanup
08/19/11  grk     Change to send periodic status reports for SRBs when missing
                  PDUs are detected and timer-status-prohibit is configured.
11/02/11  grk     Fix to transmit correct HFN in RESET_ACK in response to a
                  new RESET received while in RESET_PENDING state.
08/17/11  grk     Fix to trigger UE initiated RESET if HFN value in RESET/RESET_ACK
                  received from NW is lower than the one currently configured.
08/11/11  grk     Reset status prohibit count after transmitting status PDU.
03/09/11  ssg     MAC-ehs support on Apollo.
03/09/11  kvk     Modified the debug code for detecting the Invalid SDU only if debug flag is enabled
03/02/11  kvk     Added support for PDU/SDU print and also Invalid SDU detection >1500
03/08/11  pj      Added debug code for detecting ul_ctl_blk_id_tbl corruption
03/02/11  pj      Added function definition to check if UL entity is in RESET_PENDING state
01/17/11  vp      CMI Phase-4 clean-up of header files
10/18/10  ssg     Added changes for new WCDMA-L2 interface.
10/28/10  pj      Removed FEATURE_WCDMA_RLC_LW_MUTEX for updating ReTx-Q semaphore
10/14/10  grk     Fix to correctly update ciphering parameters before sending
                  RESET_ACK for a NW initiated RESET.
09/30/10  pj      moved enum definitions to rlci.h from rlcdl.h to avoid mob error 
09/27/10  pj      Added logging mask for dumping RLC DL error conditions
07/07/10  pj      Added changes to use a global base timer for status processing
05/12/10  grk     Reorganized part of code under FEATURE_INACTIVITY_HANDLING.
05/10/10  pj      Added support for DL Datapath Log packet
04/13/10  grk     changes for ZI memory reduction.
03/30/10  grk     Modified prototype of DSM WM callback for CMI compliance.
03/22/10  grk     Added debug prints for TM.
12/07/09  grk     Changes to return valid deciphering parameters for PDUs 
                  received before receiving RESET_ACK (in case DL is in 
                  RESET_PENDING due to UE initiated RESET) and drop PDUs received 
                  after receiving RESET_ACK till DL comes out of RESET_PENDING.
12/7/09   kvk     Fixed lint warning
12/04/09  grk     Added protection for internal Q access, so that UL ISR and
                  DL task dont access the Q simultaneously.
11/24/09  pj      Added declarations for status prohibit timer functions
11/25/09  grk     Added fix to flush off retransmission Q as part of DL reset
                  procedure.
11/16/09  grk     Added debug code for retx_data_bytes corruption.
10/23/09   av     Temperature FC   
10/21/09  grk     1.Added new DL state variable to keep track of SN till which
                    DL status array is updated dl_vr_h_status. 
                  2.Fix to use vr_r as last NAK SN in case dl_vr_h_status is less 
                    than vr_r.
10/15/09  grk     Fix to send status for PDUs only after corresponding NAK 
                  status is updated.
10/07/09  av      Multi-client flow control algorithm 
08/13/09  grk     Changes for bitmap based status tracking.
08/05/09  pj      Added new Macro-definitions for RLC_DBG_UL_UM_PDU
07/30/09  grk     Added debug code.
06/25/09  grk     Added support for future ciphering activation, when SMC is
                  is received with activation sequence number less than vr_h.
06/12/09  av      Added feature to genrate RLC event during DL config
06/08/09  kvk     Fixed compile errors/warnings
06/02/09  kvk     Added debug masks for PDU and SDU debugging
05/29/09  ssg     Added support for SNOW_3G algo.
05/15/09  kvk     Added debug functionality to debug the TCP checksum error issues
05/12/09  pj      Added new Macro-definitions for 
                    1. SL-pair length {16 bits (SN,Length)}
                    2. LIST-header {8-bits (LIST-Type, #SL-pairs)}
05/11/09  ssg     Updated Copyright Information
04/29/09  av      Fixed compiler warnings in L2 
02/25/08  pj      Added requried changes to calculate report_vr_h value while 
                  copying NAKs to TX-NAK queue 
03/11/09  pj      Added code changes to have UL/DL-task & EUL-ISR concurrency 
                  protection under light-weight mutex
03/01/09  rm      Added new featurization for FEATURE_CMI.
01/22/09  grk     Merged L2 mutex changes from taxis to mainline.
16/01/08  grk     Cleanup for F3s under FEATURE_WCDMA_RLC_PROFILING flag.
13/01/08  grk     Moved F3s under debug variable, also allowing F3s to be 
                  printed only for SRBs
03/03/08  ssg     Added support to alternate e-bit interpretations under the
                  featue flag FEATURE_RLC_ALTERNATE_E_BIT.
12/17/08  grk     Moved PDU prints in rlc_ul_enh_build_um_pdus() under 
                  debug variable to avoid low throughput on UL.
12/08/08  ssg     Added debug value for UL-PP-HW. 
10/23/08  pj      Added prototype for rlci_chk_dl_reset_pending() function 
11/07/08  pj      Added new flag RLC_DBG_PDU_DUMP to enable debug code for 
                  complete PDU dumping
10/17/08  grk     Merged taxis changes to mainline.
01/10/08  grk     Compiler warning fixes for RLC_AM_INC_VT_DAT macro.
09/08/08  pj      Modified RLC_AM_INC_VT_DAT definition, so that it returns correct 
                  vt_dat value with MSB masked
09/15/08  ssg     Added status related debugging macro.
09/08/08  pj      Added prototype for rlci_get_first_dl_nak_sn()
08/26/08  pj      Added a flag for debugging ciphering issues using rlc_debug_var
08/08/08  ssg     Added fix to ignore NAKs above VT_MS, when tx_window shrinked.
08/08/08  ssg     Added fix to ignore duplicate Naks received.
07/21/08  ssg     Added debug code to DSM memory leaking under RLC_DBG_DSM_MEM_LEAK.
06/09/08  pj      Added fix for mis-alignment access issue. Added function 
                  rlc_copy_to_word()
05/20/08  kvk     Added new macro to debug RLC UM PDU
05/13/08  ssg     Added new macros RLCI_AM_SEQ_DEC, RLCI_UM_SEQ_DEC.
04/25/08  ar      Modified rlci_update_tx_win_size() declaration to include
                  argument "called_from_rlcfc"
04/18/08  ssg     Added appropriate macros for logging F3 msgs
04/18/08  ssg     Added global variable for debugging and appropriate macros.
03/28/08  pj      Added function prototype for rlci_set_duplicate_reset_rx()
03/24/08  pj      Added support for data path optimizations under 
                   FEATURE_WCDMA_UL_DATA_PATH_OPT & FEATURE_WCDMA_DL_DATA_PATH_OPT
          ar      1. Added macros for reserved 7 and 15 bit LI under feature flag
                     FEATURE_WCDMA_DL_DATA_PATH_OPT to support reserved special LIs
                     as per Rel-5
02/01/08  pj      Changed FEATURE_MBMS flag to FEATURE_MODEM_MBMS.
12/14/07  ssg     Added functon prototype for rlci_set_nw_only_init_reset().
12/06/07  vg      added support for multiple RRC operations handling at the
                  time of channel config req under feature flag 
                  FEATURE_MULTIPLE_RLC_OPERATIONS.
11/30/07  ssg     Added mbms support under FEATURE_MBMS.
10/29/07  ssg     Modified HFN related MACROS.
07/04/07  ssg     Added funtion prototype to set the type of ACK/MRW_ACK sufi
                  that is being processed.
05/19/07  ssg     Added macros for HFN increment & comparison.
10/06/06  ssg     Added function prototype for rlci_dl_change_state().
09/11/06  ttl     Add support to overwrite the HFN being using.
08/28/06  vg      Added function prototypes for rlci_update_tx_nak_q(), 
                  rlci_read_vr_mr() under FEATURE_REMOVE_OUT_OF_WINDOW_NAKS.
08/23/06  ssg     Added function protoytes rlci_empty_dl_control_q(),
                  rlci_set_ul_am_state() for one side re-establishment
                  under the feature FEATURE_RLC_ONE_SIDE_RE_ESTABLISHMENT.
07/28/06  ttl     Declare function rlci_get_dl_latest_pending_hfn().
07/06/06  ttl     Removed rlci_update_peer_tx_win_size().
03/03/06  ttl     Declare rlci_generate_dl_status().
02/22/06  ttl     Added declaration of rlci_check_mrw_ack_validity() to remove 
                  a compilation warning.
12/09/05  pdv     Code Optimization.
12/07/05  aw      Added the function rlc_set_activity_flag function call.
12/02/05  pdv     Handling corner cases of CR79364
11/07/05  aw      Added support for inactivity handling of user plane
                  UM and AM channels under the feature 
                  FEATURE_INACTIVITY_HANDLING.
08/25/05  aw      Removed the macro RLCI_Q_LAST_CHECK from feature definition.
07/20/05  aw      Added a macro RLCI_Q_LAST_CHECK() to get the most recently 
                  added item from the queue.
07/20/05  aw      Added function definitions to support timer based sdu 
                  discard.
07/04/05  mv      Added the function definition for rlci_read_vr_h_rx_state
                  to get the value of VR(H) from the downlink state
                  under FEATURE_RLC_REPORT_VR_H_ACK_SN.
11/04/04  mv      Moved the rlci_post_rlc_reset_cmd() function definition 
                  from rlculam.h
07/22/04  mv      Defined rlci_free_retx_buf_ptr().
06/08/04  mv      Increased the number of NAK buffers from 256 to 512.
06/02/04  mv      Fixed lint errors and warnings.
02/09/04  ttl     Added rlci_suspend_dl_am_timer() and rlci_resume_dl_am_time().
10/28/03  ttl     Added ciphering algorithm indication.
10/14/03  mv      Added the function rlci_change_peer_tx_win to change the
                  window size of the peer side's transmission window.
10/07/03  ttl     Modified ciphering code to handle two copies of new ciphering
                  config, one copy of current using and two copies of old
                  ciphering config.
08/20/03  ttl     Merged branch code into tip.
08/11/03  ttl     Added one more set of cipher parameters for two pending 
                  ciphering config.
03/28/03  ttl     Increasing NUM_NAK_BUF to 256.
03/09/03  ttl     Chnaged RLC_UL_TIMER_BASE to 20.
02/14/03  ttl     Define new_cipher_on.
01/20/03  ttl     Defined rlci_copy_naks_to_tx_nak_q().
01/14/03  ttl     Defined NUM_NAK_BUF. 
                  Naming convention change for rlci_get_ul_free_missing_list_q().
12/10/02  ttl     Defined RLC_BACKUP_TIMER_CNT.
                  Removed code under FEATURE_VST.
11/13/02  ttl     Added ignore_dl_reset_msg parameter to function 
                  rlci_request_tx_reset_ack().
11/05/02  ttl     Added rlci_request_tx_reset_ack().
10/30/02  ttl     Added Cipher restart feature.
10/06/02  ttl     Used & to replace % in RLCI_SEQ_INC() and RLCI_SEQ_DEC().
09/16/02  ttl     Added $Header definition.
09/05/02  ttl     Added Ciphering feature.
08/16/02  ttl     Defined RLC_AM_MOD_MASK.
                  Defined rlci_update_rx_win_size().
08/01/02  ttl     Added definition of rlci_get_dl_sdu_byte_cnt().
07/12/02  ttl     Declared update_nak_q().
05/13/02  ttl     Added function prototype of rlci_get_rx_win_size().
05/08/02  ttl     Added function definition of rlci_non_empty_func() and
                  is_ul_am_chan_in_rach().
03/15/02  sk      Added RLC_AM_LOG_TIMER_BASE. This is set to 3 seconds.Thats
                  the period of time we send the packets to qxdm for logging.
11/30/01  ttl     Declare rlci_set_post_rrc_reset_sig().
08/08/01  ttl     Gived functions' declaration for RESET procedure.
07/03/01  ttl     changed RLC_UL_TIMER_BASE to 100 if FEATURE_VST is defined.
06/29/01  ttl     Added INIT_TIMER_BLK.
05/29/01  ttl     Removed mrw_sufi_type_e_type for not using.
05/23/01  rc      Fixed warnings found while compiling with ARM.
05/21/01  rc      Added MACROS for queue processing and a MACRO for 
                  getting the absolute difference of two sequence numbers.
                  Prepended enums and MACROs with RLCI_*.
05/02/01  ttl     Added RESET, MRW, PDU ACK, NAK enqueuing, and RETX related 
                  topics.
03/26/01  ttl     Added typedef for rlci_pdu_e_type, rlci_sufi_e_type and
                  rlci_nak_e_type.
                  Modified typedef for rlci_missing_list_type.
                  Changed lc_id type from uint8 to rlc_lc_id_type.
02/28/01  ttl     Created file.

===========================================================================*/


#include "wcdma_variation.h"
#include "comdef.h"
#include "rlcrrcif_v.h"
#include "rlcrrcif.h"
#include "macrlcif.h"
#include "rlclog.h"
#include "mcalwcdma_dec.h"
#include "wl2api.h"
#include "logging_ext.h"

/* ----------------------------------------------------------------------- */
/* Private Section.                                                        */
/* ----------------------------------------------------------------------- */

#define MAX_DATA_LOGGING 500

#define RLC_UL_AM_WIN_SIZE_CHG_FLAG 0x01
#define RLC_DL_AM_WIN_SIZE_CHG_FLAG 0x02

/* Uplink Related debug */
#define   RLC_DBG_PDU_BLDG          0x00000001
#define   RLC_DBG_PDU_BLDG_1        0x00000002
#define   RLC_DBG_CTRL_PDU_BLDG     0x00000004
#define   RLC_DBG_STATUS            0x00000008
#define   RLC_DBG_UL_PP_HW          0x00000010
#define   RLC_DBG_MEM_LEAK          0x00000020
#define   RLC_DBG_UL_TM             0x00000040  
#define   RLC_DBG_ERR_LOG_SUBMIT    0x00000080

#define   RLC_UL_PDU_SIZE           42

#define   RLC_DBG_POLLING           0x00000100
#define   RLC_DBG_PDU_DUMP          0x00000200
#define   RLC_DBG_UL_CTL_BLK_ID_TBL 0x00000400
#define   RLC_UL_COMP_LOG           0x00000800
#define   RLC_DBG_UL_UM_PDU         0x02000000
#define   RLC_DBG_UL_AM             0x00020000

/* Downlink Related debug */
#define   RLC_DBG_DL_CTRL           0x00010000
#define   RLC_DBG_DL_WM_CNT         0x00002000       // Enables current WM cnt prints

#define   RLC_DBG_REASSEMBLY        0x00100000
#define   RLC_DBG_REASSEMBLY_1      0x00200000

#define   RLC_DBG_DL_UM_PDU         0x01000000
#define   RLC_DBG_DL_PROFILING      0x10000000
#define   RLC_DBG_DL_AM             0x08000000
#define   RLC_DBG_DL_TM             0x04000000  


/* Ciphering related */
#define   RLC_DBG_UL_CIPHERING      0x10000000
#define   RLC_DBG_DL_CIPHERING      0x20000000

#define   RLC_DBG_CORRUPTION      0x80000000
#define   RLC_DBG_RTX_CORRUPTION  0x40000000

#define   RLC_DBG_DL_PDU_DUMP       0x00001000

/*--------------------------------------------------------------------------
  Base LC ID for UM UL channels..
--------------------------------------------------------------------------*/
#define RLC_UL_UM_USR_PLANE_START_LC_ID 9

/*---------------------------------------------------------------------------
  AM data PDU
---------------------------------------------------------------------------*/
#define RLC_AM_SN_SIZE                 12
#define RLC_AM_DC_BIT_SIZE              1
#define RLC_AM_DC_BIT_DATA_PDU_MASK    0x8000
#define RLC_AM_POLL_BIT_SIZE            1
#define RLC_AM_POLL_BIT_MASK           0x04
#define RLC_AM_HE_SIZE                  2
#define RLC_AM_PDU_HDR_SIZE_BYTES       2
#define RLC_AM_PDU_HDR_SIZE_BITS        16

#define RLC_AM_SPECIAL_HE_MASK         0x02

#define RLC_MAX_SDU_SIZE               1503

/*---------------------------------------------------------------------------
  AM Status PDU
---------------------------------------------------------------------------*/
#define RLC_AM_STATUS_PDU_TYPE_SIZE     3
#define RLC_AM_PDU_TYPE_SIZE            3
#define RLC_AM_SUFI_TYPE_SIZE           4

/*---------------------------------------------------------------------------
  MRW SUFI
---------------------------------------------------------------------------*/
#define RLC_AM_MRW_LENGTH_SIZE          4
#define RLC_AM_MRW_N_SIZE               4
#define RLC_AM_MRW_N_LENGTH_SIZE        4

/*---------------------------------------------------------------------------
  LIST SUFI
---------------------------------------------------------------------------*/
#define RLC_AM_LIST_LENGTH_SIZE         4
#define RLC_AM_LIST_L_SIZE              4
#define RLC_AM_MAX_LENGTH_IN_LIST_SUFI  15
#define RLC_HDR_E_MASK                0x01

#define RLC_AM_BITMAP_BYTE             8
#define RLC_AM_RLIST_SN_SIZE           12
#define RLC_AM_RLIST_CW_SIZE           4

/*---------------------------------------------------------------------------
  AM RESET PDU
---------------------------------------------------------------------------*/
#define RLC_AM_RESET_PDU_SIZE          24
#define RLC_AM_RSN_SIZE                 1
#define RLC_AM_R1_SIZE                  3
#define RLC_AM_HFN_SIZE                20

/*---------------------------------------------------------------------------
  Un acknowledge Mode definitions
---------------------------------------------------------------------------*/
#define RLC_UM_PDU_HDR_SIZE_BYTES     1
#define RLC_UM_PDU_HDR_SIZE_BITS      8

/*---------------------------------------------------------------------------
  Length Indicator related definitions
---------------------------------------------------------------------------*/
#define RLC_LI_SHORT                  1
#define RLC_LI_LONG                   2

/* Reserved LI as per Rel-5. PDU shall be discarded */
#define RLC_LI_RESERVED_7_BIT         0x7D
#define RLC_LI_RESERVED_15_BIT        0x7FFD

#define RLC_LI_PIGGYBACK_7_BIT        0x7E
#define RLC_LI_PIGGYBACK_15_BIT       0x7FFE


#define RLC_LI_EXACT                  0x00
#define RLC_LI_OCTET_LESS             0x7FFB

#define RLC_LI_START_SDU_7_BIT        0x7C
#define RLC_LI_START_SDU_15_BIT       0x7FFC

#define RLC_LI_PADDING_7_BIT          0x7F
#define RLC_LI_PADDING_15_BIT         0x7FFF

#define LI_SDU_FIT_7_BIT              0x7D
#define LI_SDU_FIT_15_BIT             0x7FFD



#define RLC_BITMAP_ARRAY_SIZE          128

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/
#define INIT_CTL_BLK_ID       0xFF

/* Initial value of rlc_id for 10ms Timer sig control block */
#define INIT_TIMER_BLK        0xFF

#define RLC_UM_MODULUS        128
#define RLC_UM_MOD_MASK       0x7F
#define RLC_UM_HALF_MOD       64
#define RLC_AM_MODULUS        4096
#define RLC_AM_HALF_MOD       2048
#define RLC_AM_MOD_MASK       0x0FFF

/* RESET, RESET ACK PDU size  */
#define RLC_RESET_PDU_SIZE    4

#define RLC_STATUS_PDU_HEADER_SIZE  4

/* MRW/MRW_ACK SUFI size in bits */
#define MRW_SUFI_SIZE         24
#define MRW_ACK_SUFI_SIZE     20

#define NUM_NAK_BUF 512

/* TX_WINDOW_SIZE SUFI  */
#define WINDOW_SUFI_SIZE      16
/* NAK SUFI size in a PDU in bits*/
#define NAK_LIST_SUFI_SIZE      24  /* 8-bits(LIST-Type, #SL-pairs) + 16-bits(SN,Length) */
#define NAK_LIST_SUFI_SL_SIZE   16 /* 16-bits(SN,Length) */
#define NAK_LIST_SUFI_HDR_SIZE  8  /* 8-bits(LIST-Type, #SL-pairs) */
#define NAK_LIST_SUFI_SL_NUM_LI_SIZE   20 /* 16-bits(SN,Length) */


#define NAK_BITMAP_SUFI_SIZE  28
#define NAK_BITMAP_SUFI_LEN_SN_SIZE  (4 + 12)
#define NAK_BITMAP_SUFI_SN_SIZE      (12)
#define NAK_BITMAP_SUFI_LEN_MASK     (0xF000)
#define NAK_BITMAP_SUFI_SN_MASK      (0xFFF)
#define NAK_BITMAP_SUFI_OCTET_BITS   (8)
#define NAK_RLIST_SUFI_SIZE   24
#define NAK_RLIST_LEN_SN_SIZE  (RLC_AM_LIST_LENGTH_SIZE + RLC_AM_RLIST_SN_SIZE)


#define ACK_SUFI_SIZE         16
#define NO_MORE_SUFI_SIZE     4

#define RLC_UL_TIMER_BASE     20
#define RLC_DL_TIMER_BASE RLC_UL_TIMER_BASE
#define RLC_UL_RE_EST_BAD_ST_FRGV (200/RLC_UL_TIMER_BASE)

#define RLC_AM_ACK_LSN_SIZE   12
#define RLC_AM_WINDOW_SN_SIZE 12

#define RLC_INVALID_RLC_ID    0xFF

#define RLC_INVALID_SN 0xFFFF
#define RLC_MAC_I_CTRL_PDU   0xFFFE
#define RLC_MAC_I_CCCH_PDU   0xFFFD

#define INVALID_LI                0xFFFF

#define LI_SDU_ONE_OCTECT_SHORT   0x7FFA

#define LI_SDU_MIDDLE_SEG_7_BIT   0x7E
#define LI_SDU_MIDDLE_SEG_15_BIT  0x7FFE

#define RLC_BACKUP_TIMER_CNT  5

#define RLC_MAX_SRB_ID 4

#define NUM_MAX_FC_ENTITY MAX(RLC_DL_FC_ENTITY_MAX, RLC_UL_FC_ENTITY_MAX)+1

#define RLC_RE_ASSEMBLY_Q_SIZE 2048

#ifdef FEATURE_MAC_I
#define POLL_SUFI_SIZE        16
#endif /*FEATURE_MAC_I*/

#ifdef FEATURE_RLC_QXDM_LOGGING
/* This timer base is used to send log packets to qxdm for logging of Am   */
/* statistics information. Currently set to 3 seconds                      */
#define RLC_AM_LOG_TIMER_BASE 3000

#define RLC_DEBUG_PDU_DUMP_WORDS 500
#define RLC_DEBUG_PDU_DUMP_BYTES 128
#define RLC_CIPHER_LOG_TIME 100              /* time in ms to compute the interval */
#define RLC_CHK_CIPHER_LOG_STATUS_INTVL (RLC_CIPHER_LOG_TIME/RLC_DL_TIMER_BASE)
#endif /*FEATURE_RLC_QXDM_LOGGING*/

#ifdef FEATURE_WCDMA_UL_COMPR
#define RLC_INVALID_ID 0xFF

#define MSG_PTP_FATAL                            WRLC_ERR_FATAL

/* 
#define MSG_PTP_ERROR                            MSG_ERROR
#define MSG_PTP_HIGH                             MSG_HIGH
#define MSG_PTP_MED                              MSG_MED
#define MSG_PTP_LOW                              MSG_MED
*/

#endif /*FEATURE_WCDMA_UL_COMPR*/

#ifndef FEATURE_WCDMA_RLC_ZI_REDUCTION_PHASE3
#define UE_MAX_DL_RLC_ENTITY    UE_MAX_DL_LOGICAL_CHANNEL
#else

/*---------------------------------------------------------------------------
SRB's - 2 UM (srb 0,1) + 3 AM (srb 2,3,4)
CTCH - 1 UM
BCCH - 1 TM
AMR - 3 TM
PS - 2 AM or 2 UM
Video call - 1 TM

UE_MAX_DL_RLC_ENTITY   18 (5TM, 5UM, 5AM + 3 buffer)
UE_MAX_DL_RLC_ENTITY   24 for MBMS with 2MCCH + 2MSCH + 2MTCH
---------------------------------------------------------------------------*/
#define UE_MAX_DL_RLC_ENTITY    18
#endif /*FEATURE_WCDMA_RLC_ZI_REDUCTION_PHASE3*/

#define RLC_DEBUG_CHANGE_ENDIANNESS(x)  (((x&0xFF000000) >> 24) | ((x&0x00FF0000) >> 8) | ((x&0x0000FF00) << 8) | ((x&0x000000FF) << 24))

#define RLCI_GET_DL_TBL_PTR(x) (&downlink_table[x])

#define RLC_SET_DATA_ACTIVITY_FLAG(rlc_ul_table_ptr, dl_data_id, user_plane_id) \
       ((dl_data_id >= user_plane_id) ? (rlc_set_activity_flag(rlc_ul_table_ptr)) : ((void)0))

/*---------------------------------------------------------------------------
    Bits/bytes/word related.
---------------------------------------------------------------------------*/
#define   CONVERT_BITS_TO_BYTES(x)      (((x) + 0x7) >> 3)
#define   CONVERT_BYTES_TO_BITS(x)      ((x) << 3)
#define   CONVERT_WORDS_TO_BYTES(x)     ((x) << 2)
#define   CONVERT_BITS_TO_WORDS(x)      (((x) + 0x1F) >> 5)
#define   CONVERT_BYTES_TO_WORDS(x)     (((x) + 0x3) >> 2)

/* retreives seven LSBs of x   */
#define RLC_AM_GET_VT_DAT(x)    ((x) & 0x7F)
/* Increments x by 1   */
#define RLC_AM_INC_VT_DAT(x)    (++(x))

#define RLC_AM_SET_PDU_NAK(x)  ((x) |= 0x80)
#define RLC_AM_PDU_NAKD(x)      ((x) & 0x80)
#define RLC_AM_CLEAR_PDU_NAK(x) ((x) &= 0x7F)

#define RLC_MIN_OF_THE_TWO(x, y) (((x) > (y)) ? (y) : (x))
#define RLC_MAX_OF_THE_TWO(x, y) (((x) > (y)) ? (x) : (y))


/* Sets the status for sequence number sn, in bitmap array nak_bitmap */
#define  RLCI_SET_AM_SN_STATUS_OPT_BITMAP( nak_bitmap, sn, modulus) \
      ( nak_bitmap[(sn & ( modulus - 1 ) ) >> 4]\
           = ( nak_bitmap[(sn & (modulus - 1) ) >> 4] | \
          ( 0x8000 >> ( sn & 0x000F ) ) ) )

/* resets the status for sequence number sn, in bitmap array nak_bitmap */
#define  RLCI_RESET_AM_SN_STATUS_OPT_BITMAP( nak_bitmap, sn, modulus ) \
         ( nak_bitmap[(sn & ( modulus - 1 ) ) >> 4]\
           = ( nak_bitmap[(sn & ( modulus - 1 ) ) >> 4] & \
           ( ~( 0x8000 >> ( ( sn & 0x000F ) ) ) ) ) )

/* Gets the status for sequence number sn, in bitmap array nak_bitmap */
#define  RLCI_GET_AM_STATUS_OPT_BITMAP( nak_bitmap, sn, modulus) \
         ( ( ( nak_bitmap[(sn & ( modulus - 1 ) ) >> 4] & \
           ( 0x8000 >> ( ( sn & 0x000F ) ) ) ) > 0 ) ? TRUE : FALSE )

/* Resets the bitmap array passed, nak_bitmap */
#define  RLCI_RESET_AM_NAK_STATUS( nak_bitmap ) \
         ( memset( nak_bitmap, 0, 128 * sizeof( uint16 ) ) )

/* Resets the status for sequence number from fsn to lsn, in bitmap array */
/* nak_bitmap, note that fsn and lsn should be at the same index in array */
#define  RLCI_RESET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, lsn, modulus) \
         ( nak_bitmap[(fsn & ( modulus - 1 ) ) >> 4]\
           = ( nak_bitmap[(fsn & ( modulus - 1 ) ) >> 4] & \
           ( ( 0xFFFF << ( 16 - ( fsn  & 0x000F ) ) ) | \
           ( 0xFFFF >> ( ( lsn & 0x000F ) + 1 ) ) ) ) )

/* Sets the status for sequence number from fsn to lsn, in bitmap array   */
/* nak_bitmap, note that fsn and lsn should be at the same index in array */
#define  RLCI_SET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, lsn, modulus) \
         ( nak_bitmap[( fsn & ( modulus - 1 ) ) >> 4]\
           = ( nak_bitmap[(fsn & ( modulus - 1 ) ) >> 4] | \
           ( ~( ( 0xFFFF << ( 16 - ( fsn & 0x000F ) ) ) | \
           ( 0xFFFF >> ( ( lsn & 0x000F ) + 1 ) ) ) ) ) )

/* Gets the status for sequence numbers from fsn to lsn as a 16 bit value */
/* note that fsn and lsn should be at the same index in array             */
#define  RLCI_GET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, lsn, modulus) \
         ( nak_bitmap[(fsn & ( modulus - 1 ) ) >> 4] & \
           ( ~( ( 0xFFFF << ( 16 - ( fsn & 0x000F ) ) ) | \
          ( 0xFFFF >> ( ( lsn & 0x000F ) + 1 ) ) ) ) )

/*Return the last sequence number in the array element where sn is,      */
/* for eg. if sn is 0 the las sn which can be prsent in one 16 bit value */
/* (in an array of 16 bit values) is 15*/
#define  RLCI_GET_AM_COUNT_STATUS_OPT_BITMAP( sn ) \
         ( 0x000F - ( sn & 0x000F ) )

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif 

/*===========================================================================

MACRO RLC_ENH_DECODE_NUM_BITS

DESCRIPTION
  This macro is used a decode value  from specified memory based upon
  number of bits and offset passed.

PARAMETERS
  status_pdu_ptr     : Pointer from where bits should be extracted.
  pdu_end_bit_pos  : Maximum value of bit_offset.
  bit_offset             : Offset from where bits should be extracted.
  num_bits             : number of bits to be extracted
  decoded_val        : Final value that is decoded based upon above parameters.
  
DEPENDENCIES  
 num_bits should be <= 32.

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
#define RLC_ENH_DECODE_NUM_BITS(table_ptr, status_pdu_ptr, pdu_end_bit_pos, bit_offset, num_bits, decoded_val) \
do\
{\
  if (pdu_end_bit_pos >= (bit_offset + num_bits))\
  {\
    decoded_val = b_unpackd(status_pdu_ptr, bit_offset, num_bits);\
    bit_offset  += num_bits;\
  }\
  else\
  {\
    WL2_MSG_3(ERROR, table_ptr, "RLC_SUFI_ERR: Not enought bits to decode PDU end %d, num_bits %d, offset %d",\
              pdu_end_bit_pos, num_bits, bit_offset);\
    bit_offset = 0;\
  }\
}while(0);

/*===========================================================================

MACRO RLCI_SEQ_INC

DESCRIPTION
  This macro increases the passed in sequence number by 1 and modulo 
  RLC_UM_MODULUS/RLC_AM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode : UM or AM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_SEQ_INC(seq,mode) ((mode == UE_MODE_UNACKNOWLEDGED ) ?  \
                           (((seq)+1) & RLC_UM_MOD_MASK) :        \
                           (((seq)+1) & RLC_AM_MOD_MASK))

/*===========================================================================

MACRO RLCI_AM_SEQ_INC

DESCRIPTION
  This macro increases the passed in sequence number by 1 and modulo 
 RLC_AM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode :AM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_AM_SEQ_INC(seq) (((seq)+1) & RLC_AM_MOD_MASK)

/*===========================================================================

MACRO RLCI_AM_SEQ_INC_BY_NUM_SN

DESCRIPTION
  This macro increases the passed in sequence number by num_sn and modulo 
 RLC_AM_MODULUS according to the mode.

PARAMETERS
  seq        : Sequence numbers.
  num_sn  : Number of SN by which SN should be incremented

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_AM_SEQ_INC_BY_NUM_SN(seq,num_sn) (((seq)+num_sn) & RLC_AM_MOD_MASK)

/*===========================================================================

MACRO RLCI_UM_SEQ_INC

DESCRIPTION
  This macro increases the passed in sequence number by 1 and modulo 
 RLC_UM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode :AM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_UM_SEQ_INC(seq) (((seq)+1) & RLC_UM_MOD_MASK)

/*===========================================================================

MACRO RLCI_SEQ_DEC

DESCRIPTION
  This macro decreases the passed in sequence number by 1 and modulo 
  RLC_UM_MODULUS/RLC_AM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode : UM or AM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_SEQ_DEC(seq,mode) ((mode == UE_MODE_UNACKNOWLEDGED) ? \
                           (((seq)+127) & RLC_UM_MOD_MASK) :    \
                           (((seq)+4095) & RLC_AM_MOD_MASK)) 

/*===========================================================================

MACRO RLCI_AM_SEQ_DEC

DESCRIPTION
  This macro decreases the passed in sequence number by 1 and modulo 
  RLC_AM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode : AM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_AM_SEQ_DEC(seq) (((seq) + 4095) & RLC_AM_MOD_MASK)

/*===========================================================================

MACRO RLCI_UM_SEQ_DEC

DESCRIPTION
  This macro decreases the passed in sequence number by 1 and modulo 
  RLC_UM_MODULUS according to the mode.

PARAMETERS
  seq : Sequence numbers.
  mode : UM

DEPENDENCIES
  None

RETURN VALUE
  sequence number.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_UM_SEQ_DEC(seq) (((seq) + 127) & RLC_UM_MOD_MASK) 

/*===========================================================================

MACRO RLCI_SEQ_GE

DESCRIPTION
  This macro will compare the 2 passed sequence number parameters and 
  return TRUE if the 1st argument is Greater than or Equal to the 2nd 
  argument (modulo the seq_modulus/2), where seq_modulus is the 3rd argument

PARAMETERS
  seq1, seq2: Sequence numbers for comparison.
  seq_modulus: 128 for UM and 4096 for AM.
  All the passed arguments must be 32bit words.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if seq_1 >= seq_2 modulo seq_modulus, else FALSE

SIDE EFFECTS
  None
===========================================================================*/

#define RLCI_SEQ_GE( seq1, seq2, seq_modulus) \
  ((((seq1) - (seq2)) & (seq_modulus >> 1)) == 0)

/*===========================================================================

MACRO RLCI_SEQ_GT

DESCRIPTION
  This macro will compare the 2 passed sequence number parameters and 
  return TRUE if the 1st argument is Greater than the 2nd argument (modulo 
  input seq_modulus/2).

PARAMETERS
  seq1, seq2: Sequence numbers for comparison, and modulo to use.

DEPENDENCIES
  The passed arguments must be words

RETURN VALUE
  TRUE if seq_1 > seq_2 modulo seq_modulus, else FALSE

SIDE EFFECTS
  None
===========================================================================*/

#define RLCI_SEQ_GT( seq1, seq2, seq_modulus) \
  (((seq1) != (seq2)) && ((((seq1) - (seq2)) & (seq_modulus >> 1)) == 0))
  
/*===========================================================================

MACRO RLCI_CHECK_AM_SN_WITHIN_RLC_WIN

DESCRIPTION
  This macro will check whether the passed sequence number is with RLC AM window or not.
  i.e VT(A) < SN <= VT(S)

PARAMETERS
  VT_S - SN that is to be transmitted next time.
  VT_A - SN from which ACK/NAK is expected.
  SN    - SN that needs to be checked.  

DEPENDENCIES
  The passed arguments must be words

RETURN VALUE
  TRUE if within window else FALSE

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_CHECK_AM_SN_WITHIN_RLC_WIN(vt_a,vt_s,sn)(RLCI_SEQ_GT(vt_s, sn, RLC_AM_MODULUS) && \
                                                      RLCI_SEQ_GE(sn, vt_a, RLC_AM_MODULUS))

/*===========================================================================

MACRO RLCI_SEQ_DIFF

DESCRIPTION
  This macro finds the difference between two sequence numbers.

PARAMETERS
  seq1, seq2: Sequence numbers for comparison, 
  seq_modulus: Modulo to use.

DEPENDENCIES
  This macro assumes that seq1 is greater than seq2 (including sequence 
  modulus).
  
RETURN VALUE
  Absolute difference of the two sequence numbers.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_SEQ_DIFF(seq1, seq2, seq_modulus) \
((seq1 >= seq2) ?\
 (seq1 - seq2) :\
 ((seq_modulus - seq2) + seq1))

#define RLCI_HFN_INC(hfn) ((hfn+1) & 0xfffff)
#define RLCI_HFN_DEC(hfn) ((hfn+0xfffff) & 0xfffff)
#define RLCI_HFN_GT(hfn1, hfn2) \
  (((hfn1) != (hfn2)) && ((((hfn1) - (hfn2)) & 0x80000) == 0))
  
/*===========================================================================

MACRO RLCI_Q_GET_NEXT_ENTRY

DESCRIPTION
  Get the next entry from the Queue, given the current entry.

PARAMETERS
  q_ptr = Pointer to Queue.
  entry_ptr = Pointer to current entry.

DEPENDENCIES
  
RETURN VALUE
  Pointer to dsm item chain.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_Q_GET_NEXT_ENTRY(q_ptr, entry_ptr) \
    ((dsm_item_type *)q_next(q_ptr, &(entry_ptr->link)))

/*===========================================================================

MACRO RLCI_Q_GET

DESCRIPTION
  Remove an item from the head of the Queue.

PARAMETERS
  q_ptr : Pointer to the Queue.
  link : Link

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the dsm item.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_Q_GET(q_ptr) \
  (dsm_item_type *)(q_get(q_ptr))  
    

/*===========================================================================

MACRO RLCI_Q_CHECK

DESCRIPTION
   Get a pointer to the first item in the Queue

PARAMETERS
  reseq_q_ptr: Pointer to the Queue.

DEPENDENCIES
  This MACRO is used only for DSM items.

RETURN VALUE
  Pointer to the dsm item.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_Q_CHECK(q_ptr) \
  (dsm_item_type *)(q_check(q_ptr))

/*===========================================================================

MACRO RLCI_Q_LAST_CHECK

DESCRIPTION
   Get a pointer to the last item in the Queue

PARAMETERS
  q_ptr: Pointer to the Queue. The queue contains items of type q_type

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the q_type.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_Q_LAST_CHECK(q_ptr) \
  (q_last_check(q_ptr))

/*===========================================================================

MACRO RLCI_Q_PUT
DESCRIPTION
   Insert an item at the tail of a Queue

PARAMETERS
  reseq_q_ptr: Pointer to the Queue.
  item_ptr : Item to insert

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
#define RLCI_Q_PUT(q_ptr, new_link) \
   (q_put(q_ptr, new_link))
    
/*===========================================================================

MACRO RLCI_Q_INSERT

DESCRIPTION
  Insert an item into a Queue.

PARAMETERS
  reseq_q_ptr: Pointer to the Queue.
  new_link: link of the item to insert
  old_link: Link of item already in Queue

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_Q_NO_SELF_QPTR
#define RLCI_Q_INSERT(reseq_q_ptr, new_link, old_link) \
    q_insert(reseq_q_ptr, new_link, old_link);
#else
#define RLCI_Q_INSERT(reseq_q_ptr, new_link, old_link) \
    q_insert(new_link, old_link);
#endif /*FEATURE_Q_NO_SELF_QPTR*/
    

/*===========================================================================

MACRO RLCI_Q_DELETE

DESCRIPTION
  Delete an item from a Queue.

PARAMETERS
  reseq_q_ptr: Pointer to the Queue.
  item_ptr : Pointer to item to delete.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_Q_NO_SELF_QPTR
#define RLCI_Q_DELETE(q_ptr, item_ptr) \
{\
  item_ptr->dup_ptr = NULL;\
  q_delete(q_ptr, &(item_ptr->link));\
}
#else
#define RLCI_Q_DELETE(q_ptr, item_ptr) \
{\
  item_ptr->dup_ptr = NULL;\
  q_delete(&(item_ptr->link));\
}
#endif /*FEATURE_Q_NO_SELF_QPTR*/

/*===========================================================================
**                  TYPES DECLARATION
**=========================================================================*/

typedef enum {
  DL_NULL_STATE = 0,                    /* Logical channel is currently not */
                                        /* in use.                          */
  DL_DATA_TRANSFER_READY,               /* Logical Channel has been         */
                                        /* established, and data transfer   */
                                        /* is allowed.                      */
  DL_RESET_PENDING,                     /* RESET has been sent to the peer  */
                                        /* RLC entity,and no data transfer  */
                                        /* is allowed unitl a RESET ACK is  */
                                        /* received. Only applicable for    */
                                        /* AM channels.                     */
  DL_STOP = 0x04                        /* Stop to receive any transmission */
} rlci_dl_state_e_type;

typedef enum
{
  RLC_DL_FC_ENTITY_CFCM,
  RLC_DL_FC_ENTITY_PS_WM,
  RLC_DL_FC_ENTITY_MAX  
}rlc_dl_multi_client_fc_entity_type;

typedef enum
{
  RLC_UL_FC_ENTITY_CFCM,
  RLC_UL_FC_ENTITY_MAX  
}rlc_ul_multi_client_fc_entity_type;

typedef enum
{
  RLC_AM_TIMER_UNDEF_STATUS = 0, /* Timer doesnt exist */
  RLC_AM_TIMER_RUNNING,     /* Timer enabled, is running */
  RLC_AM_TIMER_SUSPEND,     /* Timer suspended, LC just re-config/ RESET */
  RLC_AM_TIMER_STOP         /* Timer stopped, by RRC*/
} rlc_tmr_status_type;

typedef enum {
  RLCI_CLOSE_RX_WIN = 0x00,
  RLCI_OPEN_RX_WIN = 0x01
} rlci_win_op_e_type;

/*---------------------------------------------------------------------------
  Enumerated type that defines the RLC Ciphering mode.
---------------------------------------------------------------------------*/
typedef enum {
  NO_CIPHER = 0,
  CIPHER_WAIT,
  CIPHER_MOD,
  CIPHER_ON
} cipher_mode_e_type;

/*--------------------------------------------------------------------------- 
  SUFI type -- Structure of a Super-Filed in status PDU.
---------------------------------------------------------------------------*/
typedef enum{
  RLCI_STATUS_PDU = 0x00,
  RLCI_RESET_PDU,
  RLCI_RESET_ACK_PDU,
  RLCI_RESERVED_PDU
#ifdef FEATURE_WCDMA_UL_COMPR
  , RLCI_PTP_PDU = 0x04
#endif
} rlci_pdu_e_type;

/*--------------------------------------------------------------------------- 
  SUFI type -- Structure of a Super-Filed in status PDU.
---------------------------------------------------------------------------*/
typedef enum{
  RLCI_SUFI_NO_MORE = 0x00,
  RLCI_SUFI_WINDOW,
  RLCI_SUFI_ACK,
  RLCI_SUFI_LIST,
  RLCI_SUFI_BITMAP,
  RLCI_SUFI_RLIST,
  RLCI_SUFI_MRW,
  RLCI_SUFI_MRW_ACK,
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
  RLCI_POLL_SUFI,
#endif
  RLCI_SUFI_RESERVED
} rlci_sufi_e_type;

/*--------------------------------------------------------------------------- 
  NAK type -- List, Bitmap and Relation List. 
---------------------------------------------------------------------------*/
typedef enum{
  RLCI_NAK_LIST_TYPE = 0,
  RLCI_NAK_BITMAP_TYPE,
  RLCI_NAK_RLIST_TYPE
} rlci_nak_e_type;

#ifdef FEATURE_RLC_QXDM_LOGGING
typedef enum{
  RLC_ERR_OUTSIDE_WINDOW = 0,    /* PDU received is putside rlc window */
  RLC_ERR_TIMER_DISCARD,          /* SDU-DISCARD-TIMER expired */
  RLC_ERR_INVALID_LI,             /* PDU with Invalid Length Indicator */
  RLC_ERR_DUPLICATE_PDU,          /* Duplicate PDU is received */
  RLC_ERR_BUFFER_FULL,            /* RLC buffer is full */
  RLC_ERR_INVALID_PDU_SIZE        /* PDU with invalid size is received */
} rlc_err_reason_type;

typedef enum  {
  DOWNLINK_FIXED_SIZE,
  DOWNLINK_FLEXIBLE_SIZE,
  UPLINK_FIXED_SIZE,
  UPLINK_FLEXIBLE_SIZE
}rlci_direction_and_rlc_e_type;
#endif /*FEATURE_RLC_QXDM_LOGGING*/

#ifdef FEATURE_WCDMA_UL_COMPR
typedef enum
{
  RLC_PTP_UL_COMP_HDR_DATA = 0x00,
  RLC_PTP_UL_COMP_HDR,
  RLC_PTP_UL_COMP_RESERVED
} rlc_ptp_ul_comp_scope_type;

typedef enum
{
  RLC_PTP_UL_COMP_ALGO_IPDC = 0x00,
  RLC_PTP_UL_COMP_ALGO_RESERVED
} rlc_ptp_ul_comp_algo_type;

typedef enum
{
  RLC_PTP_REAS_RSVD = 0x00,
  RLC_PTP_REAS_UE_INT_ISSUE,
  RLC_PTP_REAS_CFG_INFO_INSUFF,
  RLC_PTP_REAS_UNEXP_CALL_FLOW,
  RLC_PTP_REAS_NOT_SUPP,
  RLC_PTP_REAS_RSVD_MAX
}rlci_ptp_sufi_rsp_reason_type;

typedef enum{
  RLCI_PTP_ULCM_RSP_RESERVED = 0x00,
  RLCI_PTP_ULCM_RSP_REASON,
  RLCI_PTP_ULCM_RSP_RESERVED_MAX
} rlci_ptp_rsp_param_tag_type;

typedef enum{
  RLCI_PTP_MSG_RESP_RESERVED = 0x00,
  RLCI_PTP_MSG_RESP_UL_COMPR,
  RLCI_PTP_MSG_RESP_RESERVED_MAX
} rlci_ptp_msg_response_type;

typedef enum{
  RLCI_PTP_ULCM_RESERVED = 0x00,
  RLCI_PTP_ULCM_BASIC_CONFIG,
  RLCI_PTP_ULCM_THRESHOLD,
  RLCI_PTP_ULCM_PKT_HDR_LEN,
  RLCI_PTP_ULCM_RESERVED_MAX
} rlci_ptp_param_tag_type;

typedef enum{
  RLCI_PTP_UL_RESERVED = 0x00,
  RLCI_PTP_UL_COMP_ENABLE,
  RLCI_PTP_UL_COMP_DISABLE,
  RLCI_PTP_RESERVED_MAX
} rlci_ptp_tag_type;

typedef enum{
  RLCI_PTP_FUNC_MGMT = 0x00,
  RLCI_PTP_RESERVED
} rlci_ptp_msg_type;

/* Enum to represent the PTP SUFI Type */
typedef enum{
  RLCI_PTP_MSG_RESERVED = 0x00,
  RLCI_PTP_MSG,
  RLCI_PTP_MSG_ACK,
  RLCI_PTP_MSG_NACK,
  RLCI_PTP_MSG_NOMORE,
  RLCI_PTP_MSG_ERROR,
  RLCI_PTP_MSG_RESERVED_MAX
} rlci_ptp_sufi_type;
#endif /*FEATURE_WCDMA_UL_COMPR*/

typedef uint16 (*state_to_entity_flow_function_type)(sys_modem_as_id_e_type, uint16, uint16);
typedef void (*output_flow_to_output_trigger_function_type) (sys_modem_as_id_e_type, uint16, void*);

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
  
  rlc_rrc_tm_rab_type rlc_dl_tm_rab;
  
} rlci_dl_tm_ctrl_type;


/*--------------------------------------------------------------------------- 
  Structure to hold nak/rtx status in bitmap array.
  This structure type is used both in UL and DL.
---------------------------------------------------------------------------*/
typedef struct
{

  uint16     status_bitmap[RLC_BITMAP_ARRAY_SIZE]; /* array holding nak/rtx*/
                                                   /* status.             */
  uint16     next_sn;                              /* sn from where to start*/
                                                   /* adding naks or from  */
                                                   /* where to start rtx   */
                                                   /* PDUs in case we left */
                                                   /* the process in b/w   */
  boolean    next_sn_valid;                        /*valid flag for next_sn*/

}rlc_opt_status_type;

/*--------------------------------------------------------------------------- 
  Structure to hold a 16-bit value as individual bits.
---------------------------------------------------------------------------*/
typedef struct
{
  uint8 bit0  : 1;
  uint8 bit1  : 1;
  uint8 bit2  : 1;
  uint8 bit3  : 1;
  uint8 bit4  : 1;
  uint8 bit5  : 1;
  uint8 bit6  : 1;
  uint8 bit7  : 1;
  uint8 bit8  : 1;
  uint8 bit9  : 1;
  uint8 bit10 : 1;
  uint8 bit11 : 1;
  uint8 bit12 : 1;
  uint8 bit13 : 1; 
  uint8 bit14 : 1;
  uint8 bit15 : 1;

}rlc_16bit_map_type;

/*--------------------------------------------------------------------------- 
  Union to hold a 16-bit value as a 16-bit uint as well as individual 16bits.
  This is used in calculating number of 1's/0's in the 16-bit value in this
  union.
---------------------------------------------------------------------------*/
typedef union
{
  rlc_16bit_map_type   bit_map;
  uint16               var;
}rlc_bitmap_type;

typedef struct {
  sys_modem_as_id_e_type as_id;
  uint16 state[NUM_MAX_FC_ENTITY];
  uint16 entity_flow[NUM_MAX_FC_ENTITY];
  state_to_entity_flow_function_type state_to_entity_function_array[NUM_MAX_FC_ENTITY];
  uint16 num_fc_entity;
  output_flow_to_output_trigger_function_type output_flow_to_output_trigger_function;
  uint16 configured_max_flow;
  uint16 output_flow;
  uint16 prev_output_flow;
  void *fc_object;
  rex_crit_sect_type *fc_semaphore;
}multiclient_fc_arbitarator_type;

/*--------------------------------------------------------------------------- 
  Typedef for storing the ciphering parameters for a logical channel.
---------------------------------------------------------------------------*/
typedef struct {
  cipher_mode_e_type cipher_mode;      /* Ciphering mode                   */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo;  /* Ciphering algorithn.(UEA0, UEA1) */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo_new;  /* Ciphering algorithn.         */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo_new1; /* Ciphering algorithn.         */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo_old;  /* Ciphering algorithn.         */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo_old1; /* Ciphering algorithn.         */
  uint32  cipher_key_id;               /* Ciphering Key Id                 */
  uint32  cipher_key_id_new;           /* Ciphering Key Id                 */
  uint32  cipher_key_id_new1;          /* Ciphering Key Id for 2nd pending */
                                       /* session                          */
  uint32  cipher_key_id_old;           /* Ciphering Key Id                 */
  uint32  cipher_key_id_old1;          /* Ciphering Key Id                 */
  uint32  hfn;                         /* Hyperframe number                */
  uint32  hfn_new;                     /* Hyperframe number                */
  uint32  hfn_new1;                    /* Hyperframe number for 2nd        */
                                       /* pending session.                 */
  uint32  hfn_old;                     /* Hyperframe number                */
  uint32  hfn_old1;                    /* Hyperframe number                */
  uint32  count;                       /* Count - Ciphering HFN + RLC      */
                                       /* sequence number                  */
  uint32  count_old;                   /* Count - Ciphering HFN + RLC      */
                                       /* sequence number                  */
  uint32  count_old1;                   /* Count - Ciphering HFN + RLC      */
                                       /* sequence number                  */
  uint16  rb_id;                       /* Radio Bearer Id                  */
  uint16  act_sn;                      /* SN to start ciphering            */
  uint16  act_sn_new;                  /* act_sn for 2nd pending session   */
  uint16  act_sn_new1;                 /* act_sn for 2nd pending session   */
  uint16  act_sn_old;                  /* old act_sn                       */
  uint16  act_sn_old1;                  /* old act_sn                      */
  boolean cipher_all;                  /* TRUE - cipher all PDU tx'd.      */
  boolean act_old_cfg;                 /* TRUE - old config activated.     */
  boolean act_old_cfg1;                /* TRUE - old1 config activated.    */
  boolean new_cipher_on;               /* TRUE - Just transfer to CIPHER_ON*/
                                       /* w/o receiving any data yet.      */ 
  boolean two_pending;                 /* Two ciphering pending sessions   */
  boolean avoid_rollover_check;        /* TRUE - new config was applied when act_sn >= 0
                                          & vr_h < 4096. so do not check for roll-over 
                                          until vr_h rolls over as well     */
} rlci_ciphering_type;

/*--------------------------------------------------------------------------- 
  Move Receiving Window Queue type
---------------------------------------------------------------------------*/
typedef struct 
{
  q_link_type link;
  uint16 mrw_sn;                          /* MRW sequence #           */
  uint8  mrw_n;                           /* n_length for MRW         */
  uint8  vt_mrw;                          /* VT(MRW) - # of MRW sent */
} rlci_mrwq_type;

typedef struct {
  dsm_item_type*   dsm_ptr;
  uint16           offset;
  uint16           length;
}rlc_am_sdu_info_type;

typedef struct{
  /* Q of RLC ENH DATA structures */
  /* MAC will deliver Non-HS data to RLC using this data structure
     * MAC will enqueue them into rlc_enh_data_q and post a signal to RLC
     * to act in TASK context. After handling, the entry will be returned
     * to rlc_enh_free_data_q */
  rlc_enh_data_ind_type *rlc_enh_data;
  
  q_type rlc_enh_data_q;
  q_type rlc_enh_free_data_q;

  /* Q of TSN info structures */
  /* MAC delivers the tsn structure to RLC in interrupt context 
     * This TSN will be processed in RLC DL task context and 
     * releases (puts) them back in Free Q (call provided by MAC-HS)*/
  q_type rlc_tsn_info_q;
}rlc_dl_data_q_type;

typedef struct {
  uint16 pdu_cnt;

  /* starting address of big dsm poiner which may have many PDU payload */
  dsm_item_type   *dsm_ptr;

  /* offset from where this PDU payload starts */
  uint16 offset;
  
  /* length of PDU*/
  uint16 length;
}rlci_dl_temp_pdu_info_type;

typedef struct {
  dsm_item_type*   dsm_ptr;
  uint16           offset;
  uint16           length;
  /* AM - last 15 bits - SN(12bits) + P(1 bit) + HE(2 bits) */
  /* UM - last 8 bits - SN(7 bits) + E(1 bit) */
  uint16           ctrl_info; 
}rlc_pdu_info_type;
  
#ifdef FEATURE_RLC_QXDM_LOGGING
/*------------------------------------------------------------------- 
  typedef for storing a structure, containing data when rlc config
  event is generated. rlc config event is used in automation, and is
  generated for DTCH UM and AM channels, whenever the channels are
  established, reconfiged and modified. Right now the event is generated
  only for donwlink only.
  -------------------------------------------------------------------*/

typedef struct  {
  uint8 rlc_id;                      /*rlc id forwhich the event is generated*/
  uint8 direction_and_rlc_type;      /*This denotes the UL or DL rlc, with 
                                       fixed or flexible rlc pdu size*/
  uint16 size;                       /*for fixed rlc pdu size, the size var
                                       denotes the pdu size. for flexible pdu
                                       siize this denotes the length of LI*/
}rlci_event_wcdma_rlc_config_type;

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
                                /* 0 - RLC_DL_NULL_STATE                   */
                                /* 1 - TM_DL_DATA_TRANSFER_READY           */
                                /* 2 - UM_DL_DATA_TRANSFER_READY           */
                                /* 3 - AM_DL_DATA_TRANSFER_READY           */
                                /* 4 - AM_DL_RESET_PENDING                 */

} rlc_dl_chan_state_type;

typedef struct
{
  uint8 nchan;                  /* Number of Downlink logical channels.    */
  rlc_dl_chan_state_type dl_state[UE_MAX_DL_LOGICAL_CHANNEL];
                                /* State Information for each Downlink     */
                                /* logical channel.                        */
}rlc_dl_log_state_type;

/*--------------------------------------------------------------------------
Data structures for logging the DL AM&UM PDU ciphering parameters
---------------------------------------------------------------------------*/
  
typedef struct{
  uint8  log_dl_rlc_id;           /* Data Logical Channel DL RLC ID(0..18) */
  uint32 cipher_key_idx;
  uecomdef_wcdma_cipher_algo_e_type cipher_algo;
  uint32 b4_cipher;               /* The 1st 4 bytes of the PDU before     */
                                  /* decipher.                             */
  uint32 count_c;                 /* PDU size in octets of each PDU        */
                                  /* Dump of the status PDU                */
} rlc_dl_cipher_log_data_type;

/*--------------------------------------------------------------------------
Data structures for logging the DL AM&UM PDUs ciphering parameters
---------------------------------------------------------------------------*/
typedef struct{
  boolean cipher_pdu_logging;     /* TRUE - cipher PDUs to be logged in    */
                                  /* Task Context, FALSE - otherwise       */
  uint8 num_pdu;                  /* Number of PDUs to be built every TTI  */
  rlc_dl_cipher_log_data_type cipher_log_data[MAX_CIPHER_PDU];
                                  /* The first four bytes of the pdu       */
} rlc_dl_pdu_cipher_info_type;
  
typedef struct{
  rlc_dl_log_state_type dl_am_log_state;
  uint8  rlc_dl_cipher_log_status_check;   /*To check if Cipher log is on perodically */
  uint32 rlc_debug_pdu_dump_array[RLC_DEBUG_PDU_DUMP_WORDS];
  uint32 rlc_debug_pdu_size_bits;
  rex_timer_type dl_tmr_log_blk;    /* Rex timer used for qxdm loggin  */
  rlc_dl_tm_config_type dl_tm_log_ptr;
  rlc_dl_um_config_type dl_um_log_ptr;
  rlc_am_config_req_type dl_am_log_ptr;
  rlc_dl_log_state_type dl_log_state;
  boolean rlci_dl_pdu_cipher_log_on;
  rlc_dl_pdu_cipher_info_type rlci_dl_pdu_cipher_log;
}rlc_dl_logging_var_type;

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
}rlc_dl_chan_am_stat_type;

/*--------------------------------------------------------------------------
Data structures for logging the first 4 bytes of the PDU by QCAT.
---------------------------------------------------------------------------*/

typedef struct{
  uint16 num_pdu;                 /* Number of PDUs to be built every TTI  */
  uint32 log_data[MAX_DL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
  uint16 pdu_size[MAX_DL_RLC_LOG_PDUS];
#else
  uint16 pdu_size;                /* PDU size in octets of each PDU        */
#endif /*((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))*/  
} rlc_dl_chan_am_pdu_type;
#endif /*FEATURE_RLC_QXDM_LOGGING*/

#ifdef FEATURE_WCDMA_UL_COMPR
/* Performance related metrics and traces */
typedef struct
{
  uint32 num_miss_comp;
  uint32 num_valid_comp_run;
  uint32 num_dacc_calls;
  uint32 num_ticks_taken;
  uint32 num_uncomp_bytes;
  uint32 num_comp_bytes;
  uint32 num_dsm_processed;
  uint32 num_comp_pkts;
  uint32 num_uncomp_pkts;
} ul_comp_perf_metrics_type;
  
/* ul_comp_mem_size - In KBytes 
 *   value 0 - 2 KB
 *   value 1 - 4 KB
 *   value 2 - 8 KB
 *   value 3 - 16 KB 
 */

typedef struct
{
  boolean                      enabled;
  rlc_lc_id_type               rlc_id;
  uint16                       ul_comp_mem_size; 
  uint16                       ul_comp_algo_type;
  uint16                       ul_comp_algo_ver;
  uint16                       ul_comp_threshold;
  rlc_ptp_ul_comp_scope_type   ul_comp_scope;
  uint16                       ul_comp_hdr_len;
}rlc_ptp_ul_comp_config_type;
#endif /*FEATURE_WCDMA_UL_COMPR*/

/*---------------------------------------------------------------------------
  Typedef for storing UM PDU logging parameters
---------------------------------------------------------------------------*/
#ifdef FEATURE_UM_PDU_LOGGING
typedef struct{
  uint16 num_pdu;                              /* Number of PDUs to be built every TTI  */
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))  
  uint16 pdu_size[MAX_DL_RLC_LOG_PDUS];        /* PDU size in octets of each PDU        */
#else
  uint16 pdu_size;
#endif
  uint32 log_data[MAX_DL_RLC_LOG_PDUS];
                                  /* The first four bytes of the pdu       */
} rlc_dl_chan_um_pdu_type;
#endif
  
typedef struct {
  boolean in_use;
  rlc_lc_id_type dl_data_id;
  rlc_pdu_info_type *pdu_info;
  dsm_item_type* partial_sdu_ptr;
  dsm_item_type* last_dsm_ptr;
  uint32 sdu_size;
}rlc_dl_am_reseq_q_type;

typedef struct{
  mcalwcdma_dec_iovec_type    *piggy_back_complete_pdu_ptr;
  #ifdef FEATURE_WCDMA_UL_COMPR
  mcalwcdma_dec_iovec_type    *ptp_debug_iovec_ptr;
  #endif

  /* Holds the AM SDU Chain during reassembly */
  rlci_dl_temp_pdu_info_type dl_am_pdu_info;

  /* Resequence queue for each AM Channel */
  rlc_dl_am_reseq_q_type dl_reseq_q_pool[UE_MAX_DL_AM_CHANNEL];

}rlc_dl_reseq_var_type;

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
  
  boolean alternate_e_bit;	        /* TRUE - Alternate E-bit interpretation */
				        /* FALSE - Normal E-bit interpretation   */
#ifdef FEATURE_UMTS_BMC_MSIM
  boolean checked_for_cmas;             /* TRUE indicates indication to NAS about
                                           CMAS msg has already been sent  */
#endif
  dsm_item_type* partial_sdu_ptr;
  dsm_item_type* last_dsm_ptr;
#ifdef FEATURE_QCHAT_HFN_MISMATCH_RECOVERY
  uint32 pdu_err_cnt; /* Counter to track continuous error */
#endif /*FEATURE_QCHAT_HFN_MISMATCH_RECOVERY*/
  
#ifdef FEATURE_RLC_QXDM_LOGGING
#ifdef FEATURE_UM_PDU_LOGGING
  rlc_dl_chan_um_pdu_type  dl_chan_um_pdu;  /* UM PDU logging structure*/
#endif /*FEATURE_UM_PDU_LOGGING*/
#endif /*FEATURE_RLC_QXDM_LOGGING*/
} rlci_dl_um_ctrl_type;

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
} rlci_dl_am_state_type;

/*---------------------------------------------------------------------------
  Used to store the necessary down-link control information for
  an RLC AM entity.
---------------------------------------------------------------------------*/
typedef struct {
  rlc_am_common_config_type am_common_local; /* Local copy of common config variables.        */
  rlc_dl_am_config_type  dl_ctl_cnfg_local;   /* Local copy dl control config variables.    */
  rlc_am_common_config_type *am_common; /* pointer to common config variables.        */
  rlc_dl_am_config_type *dl_ctl_cnfg;   /* pointer to dl control config variables.    */
  rlci_dl_am_state_type  dl_state_var;  /* dl control state variables.     */
  rlc_opt_status_type  dl_status_opt_bitmap; /* bitmap array for DL status */
  uint16               num_sn_li; /* Number of SN_LI pairs for LIST SUFI*/
  rlc_dl_am_reseq_q_type *dl_reseq_q;   /* DL Resequence Queue holding 
                                           the received DL PDUs */
  void (*sdu_discard_cb) (rlc_lc_id_type data_lc_id, byte num_sdu);
                                        /* SDU discard callback function   */
  rlc_tmr_status_type dl_tmr_sts;       /* Holds status of DL AM timer     */  
  boolean  reset_ack_rcvd;    /*Flag indicating if valid RESET_ACK is received on DL */
#ifdef FEATURE_RLC_QXDM_LOGGING
  rlc_dl_chan_am_stat_type dl_chan_am_stat; /* Stores the Am statistics    */
                                            /* information for one channel */
  rlc_dl_chan_am_pdu_type  dl_chan_am_pdu;   /* Stores the PDU information */
                                             /* for a single AM entity     */
#endif /*FEATURE_RLC_QXDM_LOGGING*/
} rlci_dl_am_ctrl_type;

/*---------------------------------------------------------------------------
  Union for storing parameters specific to a mode.
---------------------------------------------------------------------------*/
typedef union {
  rlci_dl_tm_ctrl_type tm_ctrl;         /* Transparent Mode parameters.    */
  rlci_dl_um_ctrl_type um_ctrl;         /* Unacknowledged Mode parameters. */
  rlci_dl_am_ctrl_type am_ctrl;         /* Acknowledged Mode parameters.   */
} rlci_dl_ctrl_u_type;

/*---------------------------------------------------------------------------
  Used to store information about a downlink logical channel.
---------------------------------------------------------------------------*/
typedef  struct {
  uecomdef_logchan_e_type lc_type;      /* Downlink logical channel type   */
  uecomdef_logch_mode_e_type lc_mode;   /* RLC Mode                        */
  rlci_dl_state_e_type lc_state;        /* Downlink Logical channel state  */
  rlci_ciphering_type cipher;           /* Ciphering Parameters            */
  rlci_ciphering_type cipher_backup;    /* Backup for ciphering parameters
                                                                           while applying the new one      */
  rlci_dl_ctrl_u_type ctrl;             /* Union of TM, UM and AM specific */
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
} rlci_dl_ctrl_blk_type;

/*---------------------------------------------------------------------------
  Database that is used to store information about all the downlink logical
  channels. Maximum number of downlink logical channels is 21.
---------------------------------------------------------------------------*/

typedef struct {
  sys_modem_as_id_e_type as_id;
  uint8 active_ch;                      /* Num of active logical channels. */
  rlci_dl_ctrl_blk_type ctrl_blk[UE_MAX_DL_RLC_ENTITY];
                                        /* Data structure used to store all*/
                                        /* the information for the         */
                                        /* downlink logical channel.       */
  /* Variable to store log status information
     *  RLCI_UM_USR_PLANE_LOG_MASK : 0x01
     *  RLCI_AM_USR_PLANE_LOG_MASK : 0x02
     *  RLCI_AM_SIG_PLANE_LOG_MASK : 0x04 */
  uint32  rlci_log_mask;
  boolean dl_pdu_decode_result;
  rex_timer_type        rlc_dl_am_base_tmr; /* This is base 10ms timer for RLC AM channels */
                                            /* at expiry, periodic status/ control related */
                                            /* procedures are triggered*/
  boolean               rlc_dl_am_base_tmr_start;  

  /* Holds temp SDU info while reassembly from PDUs */
  rlci_dl_temp_pdu_info_type dl_um_pdu_info;
  uint8 wm_trace_timer[UE_MAX_DL_LOGICAL_CHANNEL];
  
  rlc_dl_reseq_var_type rlc_dl_reseq_var;
  rlc_dl_data_q_type rlc_dl_data_q;
  rlc_dl_hfn_cnf_type dl_hfn_cnf;
  uint8 dl_ctl_blk_id_tbl[UE_MAX_DL_LOGICAL_CHANNEL];

  #ifdef FEATURE_BMC_RLC_MULTIPLE_PDU_SUPPORT
  boolean ctch_first_pdu;
  #endif /*FEATURE_BMC_RLC_MULTIPLE_PDU_SUPPORT*/
  
  #ifdef FEATURE_RLC_QXDM_LOGGING
  rlc_dl_logging_var_type rlc_dl_logging;
  #endif /*FEATURE_RLC_QXDM_LOGGING*/
  
  #ifdef FEATURE_WCDMA_DL_DATAPATH_LOG_PKT
  uint16 rlc_global_sub_fn;       /* holds SUB_FN value in received TSN*/
  #endif /*FEATURE_WCDMA_DL_DATAPATH_LOG_PKT*/
} rlci_downlink_table_type;

extern rlci_downlink_table_type downlink_table[];

/*===========================================================================

FUNCTION rlc_update_nak_status

DESCRIPTION
    - This function sets/resets the nak status for a range of pdus with sn 
      starting with (fsn) and ending with sn (lsn) (fsn and lsn inclusive).   
  
DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is to be updated.
  lsn        - last sequence number for which status is to be updated.
  update     - pdu status 
               FALSE - ACK
               TRUE  - NAK
  mod        - modulus to use..

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_update_nak_status
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  boolean               update,
  uint16                mod
);
/*===========================================================================

FUNCTION rlc_get_num_set

DESCRIPTION
    - This function gets the nak status indicating whether we have any naks
      pending or not.
  
DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number from where to count.
  lsn        - last sequence number till which to count.
  mod        - modulus to use..

RETURN VALUE
  number of 1's in bitmap array
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_get_num_set
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  uint16                mod
);

/*===========================================================================

FUNCTION rlc_ul_get_rtx_status

DESCRIPTION
    - This function gets the nak status from sn fsn to lsn indicating whether
      we have any retx pending or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is needed.
  lsn        - last sequence number for which status is needed.
  mod        - modulus to use..

RETURN VALUE
  rtx status - TRUE - rtx pdus are present.
               FALSE - no rtx pdus.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_ul_get_rtx_status
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
);

/*===========================================================================

FUNCTION RLCI_READ_RX_STATE

DESCRIPTION
  Read the RX state(VR_R) from the Down-link side.
  
DEPENDENCIES
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlci_read_rx_state
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id       /* Logical channel ID for data channel  */
);


/*===========================================================================

FUNCTION RLCI_READ_VR_H_RX_STATE

DESCRIPTION
  Read the RX state(VR_H) from the Down-link side.
  
DEPENDENCIES
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_read_vr_h_rx_state
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);
/*===========================================================================

FUNCTION RLCI_GET_VR_H_RX_STATE

DESCRIPTION
  Read the RX state(VR_H) from the Down-link side.

DEPENDENCIES

INPUT ARGUMENTS
  lc_id      - logical channel id.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_get_vr_h_rx_state
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);

/*===========================================================================

FUNCTION RLCI_GET_PREV_DL_VR_H_STATUS

DESCRIPTION
  Read the RX state(dl_vr_h_status) from the Down-link side.
  
DEPENDENCIES
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

uint16 rlci_get_dl_vr_h_status
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);
/*===========================================================================

FUNCTION RLCI_READ_VR_H_RX_STATE

DESCRIPTION
  Update report_vr_h, which is the last sn to be used for ACK SUFI.
  
DEPENDENCIES
NONE

INPUT ARGUMENTS
  lc_id      - logical channel id.
  vr_h       - report_vr_h is updated with this value.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_update_vr_h_rx_state
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte    lc_id,
  uint16  vr_h
);

/*===========================================================================

FUNCTION RLCI_RESET_STATUS_PROHIBIT_CNT

DESCRIPTION
 Resets status prohibit count
  
DEPENDENCIES
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_reset_status_prohibit_cnt
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);

/*===========================================================================

FUNCTION RLCI_UL_AM_BASE_TIMER_HANDLER

DESCRIPTION
  Handle 10ms Base Timer timed out.
  In Uplink/ Downlink, there is only one base timer. 
  This call back will restart the base timer(s) & set needed signals for 
  handling timer expiry in L2_UL & L2_DL tasks.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_am_base_timer_cb (uint32 cb_param);

/*===========================================================================

FUNCTION RLCI_RCVD_MRW_ACK

DESCRIPTION
  Look at the MRW_Q to compare the first MRW item in the Q to the received 
  MRW ACK_SN and N. If they are matched or the MRW ACK_SN is acknowledging
  advance, recycle the first MRW item in the MRW Q. 
  If the MRW ACK_SN is acknowledging advance, process the acknowledged SN
  to the TXQ and VT(A).
  
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_rcvd_mrw_ack
(
  rlc_lc_id_type rlc_id,
  uint16  mrw_sn,
  uint8   mrw_n
);

/*===========================================================================

FUNCTION RLCI_GET_RX_WIN_SIZE

DESCRIPTION
  Obtain the rx_win_size.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  rx_win_size.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlci_get_rx_win_size (rlci_downlink_table_type *rlc_dl_table_ptr, rlc_lc_id_type rlc_id);

/*===========================================================================

FUNCTION RLCI_GET_DL_SDU_BYTE_CNT

DESCRIPTION

  Obtains the number of data bytes received from the the network.

DEPENDENCIES
  None.

RETURN VALUE
  The byte count of received SDUs.
  
SIDE EFFECTS
  None.
===========================================================================*/
uint32 rlci_get_dl_sdu_byte_cnt (rlci_downlink_table_type *rlc_dl_table_ptr, rlc_lc_id_type rlc_id);
 
/*===========================================================================

FUNCTION RLCI_UPDATE_RX_WIN_SIZE

DESCRIPTION
  Update the Receiving Window. 
  
  RLCI_CLOSE_RX_WIN : close the Receiving Window to VR_H.
  RLCI_OPEN_RX_WIN : Open the Receiving Window to 
                    (VR_R + config RX_WINDOW) % 4096
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_update_rx_win_size
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type      rlc_id, 
  rlci_win_op_e_type  win_op
);

/*===========================================================================

FUNCTION RLCI_GET_DL_HFN

DESCRIPTION
   This function is called by RLC UL to get latest HFN's for all established
   RLC UM and AM radio-bearers.  RLC populates the * to struct 
   rlc_hfn_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If HFN's for all UM and AM rb's are populated.
  FALSE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlci_get_dl_hfn
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_hfn_req_type* hfn_req             /*ptr to hfn request type          */
);

/*===========================================================================

FUNCTION RLCI_GET_DL_COUNT_C

DESCRIPTION
   This function is called by RLC UL to get latest COUNT_C's for all established
   RLC UM and AM radio-bearers.  RLC populates the * to struct 
   rlc_count_c_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If COUNT_C's for all UM and AM rb's are populated.
  FALSE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlci_get_dl_count_c
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_count_c_req_type* count_c_req             /*ptr to hfn request type          */
);

/*===========================================================================

FUNCTION  RLCI_SUSPEND_DL_AM_TIMER

DESCRIPTION
  It suspends the DL AM 20ms timer. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_suspend_dl_am_timer
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte lc_id
);

/*===========================================================================

FUNCTION  RLCI_RESUME_DL_AM_TIMER

DESCRIPTION
  It resumes the DL AM 20ms timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_resume_dl_am_timer
(
 rlci_downlink_table_type *rlc_dl_table_ptr,
 byte lc_id
);

void  rlc_dsm_touch_exit(dsm_item_type *pkt_head_ptr);

/*===========================================================================

FUNCTION RLCI_RESET_POLL_IND

DESCRIPTION
  Calling this function with the logical channel ID would reset reset
  "rcvd_poll" indication.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_reset_poll_ind(rlci_downlink_table_type *rlc_dl_table_ptr, rlc_lc_id_type rlc_id);

/*===========================================================================

FUNCTION RLCI_GET_DL_LATEST_PENDING_HFN

DESCRIPTION
   This function is called by RRC to get latest pending HFN for indicated 
   logical channel ID via RLC UL.  
   RLC DL populates the info to the struct rlc_pending_hfn_req_type.
   If there is a pending configuration for UL alone then 
   dl_pending_hfn_present flag will be set to FALSE and vice-versa.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  If either dl_pending_hfn_present or ul_pending_hfn_present
                    flag is set to TRUE then function will return SUCCESS.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_get_dl_latest_pending_hfn
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  rlc_pending_hfn_req_type *pending_hfn
);

/*===========================================================================

FUNCTION RLCI_DL_OVERWRITE_HFN

DESCRIPTION
   This function is called by RRC to overwrite the HFN of RLC currently using
   thru UL.

   This function will be called after RRC re-establish RLC to overwrite the 
   new HFN that RRC just comes up before the traffic can be resumed.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  Done successful / no such LC or no ciphering at all for
                    this LC.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlci_dl_overwrite_hfn
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  uint32 hfn
);

/*===========================================================================

FUNCTION RLCI_DL_CHANGE_STATE

DESCRIPTION
   This function will change the downlink RLC from DL_RESET_PENDING state to
   its previous state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_dl_change_state
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);

/*===========================================================================

FUNCTION RLCI_READ_VR_MR

DESCRIPTION
  Read the RX state(VR_MR) from the Down-link side.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlci_read_vr_mr
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type lc_id
);

/*===========================================================================

FUNCTION rlci_get_config_action

DESCRIPTION
  this function checks all the bits of the bitmask and return the
  corresponding action set by RRC to RLC at the tme of channel config req
    
DEPENDENCIES
  None.
  
RETURN VALUE
  action type
  
SIDE EFFECTS
  None.

===========================================================================*/

rlc_channel_action_cnf_e_type rlci_get_config_action(rlci_downlink_table_type *rlc_dl_table_ptr, uint16 *cfg_mask);

/*===========================================================================

FUNCTION RLC_COPY_TO_WORD

DESCRIPTION
  This function copies data from uint8 pointer into a word byte-by-byte
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 rlc_copy_to_word(uint8 *src_ptr);

/*===========================================================================

FUNCTION rlc_get_nak_bitmap

DESCRIPTION
    - This function gets the nak bitmap array maintained on DL.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.

RETURN VALUE
  nak bitmap - pointer to nak bitmap array.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_opt_status_type  *rlc_get_nak_bitmap
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte                  lc_id  
);

/*===========================================================================

FUNCTION rlc_get_nak_status

DESCRIPTION
    - This function gets the nak status indicating whether we have any naks
      pending to be transmited or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is needed.
  lsn        - last sequence number for which status is needed.
  mod        - modulus to use..

RETURN VALUE
  nak status - TRUE - nak is present.
               FALSE - no naks are there.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_get_nak_status
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte                  lc_id,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
);
/*===========================================================================

FUNCTION rlc_get_num_nak

DESCRIPTION
    - This function returns the number number of sn_li pairs in the 
      DL nak status array.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.


RETURN VALUE
  num_sn_li - number of sn_li pairs in the DL nak status array.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_get_num_nak
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte                  lc_id  
);

/*===========================================================================

FUNCTION rlc_get_num_nak_sn_li

DESCRIPTION
    - This function calculates and returns the number number of sn_li pairs 
      in the nak status array..

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.
  fsn        - fist sequence number from which sn_li is counted.
  lsn        - last sequence number till which sn_li is counted.
  mod        - modulus to use..


RETURN VALUE
  num_sn_li - number number of sn_li pairs in the nak status array.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_get_num_nak_sn_li
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte                  lc_id,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
);

/*===========================================================================

FUNCTION rlc_set_num_nak_sn_li

DESCRIPTION
    - This function sets the value of number of sn_li pairs, num_sn_li.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.
  num_sn_li  - value for num_sn_li

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_set_num_nak_sn_li
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  byte                  lc_id,
  uint16                num_sn_li
);

/*===========================================================================

FUNCTION RLCi_CHK_DL_RESET_PENDING

DESCRIPTION
  This function checks if the DL is in RESET pending state.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating, RESET pending or not.

SIDE EFFECTS
  None
===========================================================================*/

boolean rlci_chk_dl_reset_pending
(
  rlci_downlink_table_type *rlc_dl_table_ptr,
  uint8 lc_id
);

#ifdef FEATURE_RLC_QXDM_LOGGING
rlc_cipher_algo_e_type rlci_map_cipher_algo
(
  uecomdef_wcdma_cipher_algo_e_type algo
);

/*===========================================================================

FUNCTION RLC_ERROR_LOG_SUBMIT

DESCRIPTION
  Function used to log the following RLC error conditions described in 
  section 11.3.4 of 25.322

PARAMETERS  
1. dl_rlc_data_id: Indicates DL rlc logical channel ID; Value starts from > 16 for AM channels
2. rlc_sn: Indicates the erroneous PDU just ignored by UE RLC; Range "0 - 4095" 
           and value of "0xFFFF" indicates that the SN is not-applicable for this
           error condition
3. rlc_err_reason: The below table gives the mapping of the error number 
to error description:
    0. PDU outside window
    1. Timer Discard timeout
    2. Invalid length
    3. PDU duplicates
    4. Buffer full
    5. Invalid size

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void rlc_error_log_submit(rlc_lc_id_type dl_rlc_data_id, uint16 rlc_sn, 
                          rlc_err_reason_type rlc_err_reason);
#endif /*FEATURE_RLC_QXDM_LOGGING*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif
void wrlc_qsh_mdump_set_rlc(sys_modem_as_id_e_type as_id);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif
void wmac_qsh_mdump_set_mac(sys_modem_as_id_e_type as_id);

#define WRLC_ERR_FATAL(as_id, FMT, A, B, C) \
do {\
wrlc_qsh_mdump_set_rlc(as_id);\
ERR_FATAL(FMT, A, B, C);\
}while(0)


#define WMAC_ERR_FATAL(as_id, FMT, A, B, C) \
do {\
wmac_qsh_mdump_set_mac(as_id);\
ERR_FATAL(FMT, A, B, C);\
}while(0)
/*===========================================================================
  WL2_MSG_X - Macros only take non-string parameters (like %d, not %s)
  
  PARAMETERS
  1. LEVEL        Options include LOW, MED, HIGH, ERROR, FATAL
  2. table_ptr    Table Pointer that has a field "as_id" indicating current SubID 
  
  USAGE           WL2_MSG_2(ERROR, rlc_ul_table_ptr, "LC ID %d is mapped to RB ID %d", lc_id, rb_id );
===========================================================================*/
//uint32 replace_sub_id;

#ifdef FEATURE_DUAL_WCDMA

#define WL2_MSG_0(LEVEL, table_ptr, string) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string); \
}

#define WL2_MSG_1(LEVEL, table_ptr, string, ARG1) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_1_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1 ); \
}

#define WL2_MSG_2(LEVEL, table_ptr, string, ARG1, ARG2) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_2_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2 ); \
}

#define WL2_MSG_3(LEVEL, table_ptr, string, ARG1, ARG2, ARG3) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_3_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3 ); \
}

#define WL2_MSG_4(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_4_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4 ); \
}

#define WL2_MSG_5(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_5_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4, ARG5 ); \
}

#define WL2_MSG_6(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_6_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6 ); \
}

#define WL2_MSG_7(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_7_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7 ); \
}

#define WL2_MSG_8(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_8_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8 ); \
}

#define WL2_MSG_9(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1); \
MSG_9_EXT(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, 1, &diag_as_id, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9 ); \
}

#define WL2_EVENT_REPORT_PAYLOAD(table_ptr, event_id, length, payload) \
do\
{\
uint32 diag_as_id = (uint32)(table_ptr->as_id + 1);\
event_report_payload_ext(event_id, length, payload, 1, &diag_as_id);\
}while(0);

#else

#define WL2_MSG_0(LEVEL, table_ptr, string)\
MSG(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string)

#define WL2_MSG_1(LEVEL, table_ptr, string, ARG1) \
MSG_1(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1 )

#define WL2_MSG_2(LEVEL, table_ptr, string, ARG1, ARG2) \
MSG_2(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2 )

#define WL2_MSG_3(LEVEL, table_ptr, string, ARG1, ARG2, ARG3) \
MSG_3(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3 )

#define WL2_MSG_4(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4) \
MSG_4(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4 )

#define WL2_MSG_5(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5) \
MSG_5(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4, ARG5 )

#define WL2_MSG_6(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
MSG_6(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6 )

#define WL2_MSG_7(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
MSG_7(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7 )

#define WL2_MSG_8(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
MSG_8(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8 )

#define WL2_MSG_9(LEVEL, table_ptr, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
MSG_9(MSG_SSID_WCDMA_L2, MSG_LEGACY_ ## LEVEL, string, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9 )

#define WL2_EVENT_REPORT_PAYLOAD(table_ptr, event_id, length, payload) \
event_report_payload(event_id, length, payload);
#endif

#endif /*RLCI_H*/

