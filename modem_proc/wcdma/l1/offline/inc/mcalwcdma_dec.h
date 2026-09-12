#ifndef MCALWCDMA_DEC_H
#define MCALWCDMA_DEC_H

/*============================================================================*/
/** @file  
 * This module has definition and declaration related to MCAL WCDMA decoder
 * module APIs
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2015 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/mcalwcdma_dec.h#5 $
$DateTime: 2021/02/10 03:14:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
11/19/19    pk      validation of MCM with CM activate and fixed pos rm method
04/19/18    br      F3 opts
05/18/16    ac      Don't update OLPC twice on LVA tracebacks > 1
10/16/15    ap      Initial check-in for FR 30856
10/17/15    ac      Initialize decoder log packet buffers for both subscriptions
08/27/15    ms      DRMB fixes for TA.2.x 
07/22/15    ac      Populate ctchIndicator in FW-SW interface during CCTrCh setup
07/22/15    ms      Featurization changes for TA W + W
06/26/15    hdk     Added ATLAS featurisation
06/05/15    ac      Allocate/deallocate TSN buffers at connenected mode init/deinit
06/03/15    svh     TA featurisation.
02/27/15    ap      Initial check-in for WL1 QSH framework 
04/22/15    ac      Datapath W+W changes
03/19/15    ac      W + W datapath init cleanup
02/28/12    rsr     W+W Phase 2 Checkin.
02/19/15    svh     Added check to catch HS decode done processing pending
                    for more than 2 TTIs.
02/05/15    ac      Replace non_first_traceback_status with lva_traceback_count
01/26/15    ac      Initial checkin for for X+W
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "comdef.h"
#include "uecomdef.h"
#include "dsm.h"
#include "mcalwcdma_common.h"

#include "rex.h"
#include "l1rrcif.h"
#include "log.h"

#ifndef FEATURE_CMI

/* -------------------------------------- */
/* PUBLIC API section of mcalwcdma_dec.h  */
/* -------------------------------------- */

/** Enum type definition for TTI enum type */
typedef enum {
  L1_TTI_10MS, /**< 10 ms TTI */
  L1_TTI_20MS, /**< 20 ms TTI */
  L1_TTI_40MS, /**< 40 ms TTI */
  L1_TTI_80MS, /**< 80 ms TTI */
  L1_NUM_TTI   /**< Total number oif TTI types */
} mcalwcdma_dec_tti_enum_type;

#else

/* Public portion of the API exists in wcdmamvsif.h */
#include "wcdmamvsif.h"

#endif /* FEATURE_CMI */

#include "l1msm.h"
#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
#include "wl1_qsh_ext.h"
#endif /* defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) */

/* ----------------------------------------------------------------------- */
/* Private Section.                                                        */
/* ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* PCCPCH TF info */
#define MCALWCDMA_DEC_BCH_NUM_TB          1
#define MCALWCDMA_DEC_BCH_TB_SIZE_BITS    246

/* Values for TTI in ms for each enum type */
#define L1_TTI_10MS_VAL 10 /**< TTI value for 10 ms */
#define L1_TTI_20MS_VAL 20 /**< TTI value for 20 ms */
#define L1_TTI_40MS_VAL 40 /**< TTI value for 40 ms */
#define L1_TTI_80MS_VAL 80 /**< TTI value for 80 ms */

/** Macro to translate TTI enum to its absolute value */
#define MCALWCDMA_DEC_TTI_VAL(tti_enum) mcalwcdma_dec_tti_val_table[(tti_enum)]

#define DEC_MDSP_SF_CFN_TTI_TYPE(cfn) \
  ((((cfn + 1) % 2) != 0) ? L1_TTI_10MS : \
   (((cfn + 1) % 4) != 0) ? L1_TTI_20MS : \
   (((cfn + 1) % 8) != 0) ? L1_TTI_40MS : L1_TTI_80MS)


/** Number of decoder related HSDPA information */
#define MCALWCDMA_DEC_HS_INFO_COUNT 2

/** Number of MAC-hs queue for a HSDPA information table */
#define MCALWCDMA_DEC_HS_MAC_HS_Q_MAX_COUNT 8

/* The final number for this depends on what kind of network configurations we end up 
 * seeing in practice. Being very conservative, I would not expect more than 1 Q for SRB and 
 * max 3 for data (1 I/B, 1 streaming and 1 conv bearer). Further, I would not expect all of 
 * these to be configured with max window - and finally chances that they all stall at the max 
 * window at the same time is very small. For that reason I would budget for 32 x 4 elements,
 * instead of 32 x 16, which I still consider very conservative.
 */
#define MCALWCDMA_DEC_MAX_AVG_HS_Q 4

/* TSN window size used at MAC */
#define MCALWCDMA_DEC_TSN_WIN_SIZE 32

/* 3GPP 25.321 Spec allows 70 MAC-d PDUs per MAC-hs transport block */
#define MCALWCDMA_DEC_MAX_DL_RLC_PDU 70

/* Maximum LC ID per Rel-7 Spec 25.321 */
#define MCALWCDMA_DEC_MAX_LC_ID 16

#ifdef FEATURE_MIMO
#define MCALWCDMA_DEC_MAX_HS_FR_PER_10MS_TTI 10
#else
#define MCALWCDMA_DEC_MAX_HS_FR_PER_10MS_TTI 5
#endif /* FEATURE_MIMO */

#ifdef FEATURE_HSDPA_MAC_EHS

#define MCALWCDMA_DEC_INVALID_Q_ID 8

#if defined FEATURE_WCDMA_3C_HSDPA
/* Max possible Reordering SDUs per HS TrBlk for 3C/4C */
#define MCALWCDMA_DEC_HS_REORDER_SDU_MAX_COUNT 44
#else
/* Max possible Reordering SDUs per HS TrBlk for SC/DC */
#define MCALWCDMA_DEC_HS_REORDER_SDU_MAX_COUNT 26
#endif


/* Macro for MAC to use to obtain DOB extension buffer size.
 * This defines the number of bytes available in DecOB for 
 * concatenating the partial PDUs to perform deciphering. 
 * Max possible RLC PDU size is 1504 bytes. For MIMO scenario,
 * there can be 10 * 1504 bytes every 10ms to concatenate, which
 * is equal to 15040 bytes. Set 16000 bytes to be on safer side.
 * Do not exceed 16000 bytes since copy engine has limitation of
 * 8K bytes per transfer and we are programming two copy
 * repeat tasks. */
#define MCALWCDMA_DEC_DOB_EXTN_BUF_SIZE 16000

/* The maximum number of reordering pdu in one transport block, currently 3 per 
 * 3GPP 25.321 Mac spec. */
#define MCALWCDMA_DEC_MAX_REORD_PDU_PER_HS_FRAME 3

/* The maximum number of partial pdu per reordering pdu, currently 2 per
 * 3GPP 25.321 Mac spec. */
#define MCALWCDMA_DEC_MAX_PARTIAL_PDU_PER_REORDER_PDU 2

/* We can have 2 complete pdus for each reordering queue in the previous TTI,
 * total = 3 * 2 * 5 for Non-MIMO
 * total = 3 * 2 * 10 for MIMO */
#define MCALWCDMA_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI \
            (MCALWCDMA_DEC_MAX_REORD_PDU_PER_HS_FRAME * \
             MCALWCDMA_DEC_MAX_PARTIAL_PDU_PER_REORDER_PDU * \
             MCALWCDMA_DEC_MAX_HS_FR_PER_10MS_TTI)

/** This macro indicates the total number of TSNs for which
 * complete PDUs were formed from previous TTIs partial PDUs. L1
 * retrieves a buffer from the Free Q, populates it and enqueues
 * into a Q to MAC. This macro is triple-buffered since
 * if MAC-hs task does not get a chance to run for three 10ms
 * frames, even then the information can be held in the
 * queue. */
#define MCALWCDMA_DEC_TOTAL_MAX_COMPLETE_PDU_FROM_PREV_TTI (MCALWCDMA_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI*3)


#if defined FEATURE_WCDMA_3C_HSDPA
#define MCALWCDMA_DEC_MAX_TSN_INFO_BUF 450
#elif defined FEATURE_WCDMA_DC_HSDPA
/* This count is to accommodate RLC not getting a chance to 
   run very frequently at peak data rates. */
#define MCALWCDMA_DEC_MAX_TSN_INFO_BUF 300
#else
#define MCALWCDMA_DEC_MAX_TSN_INFO_BUF ((MCALWCDMA_DEC_TSN_WIN_SIZE * MCALWCDMA_DEC_MAX_AVG_HS_Q) + \
                                         MCALWCDMA_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI)
#endif

#define MCALWCDMA_DEC_MAX_DMOV_SOURCE_DESCRIPTOR_LEN_MOVE_IN 100

#else /* FEATURE_HSDPA_MAC_EHS */

#define MCALWCDMA_DEC_MAX_TSN_INFO_BUF (MCALWCDMA_DEC_TSN_WIN_SIZE * MCALWCDMA_DEC_MAX_AVG_HS_Q)

#endif /* FEATURE_HSDPA_MAC_EHS */


/* Total number of CCTrCh table in mDSP */
#define MCALWCDMA_DEC_TOTAL_CCTRCH_TABLES (WFW_DBACK_NUM_PCCPCH_CCTRCH_TABLES + WFW_DBACK_NUM_CCTRCH_TABLES)
/* Number of non-BCH CCTrCh tables */
#define MCALWCDMA_DEC_TOTAL_CCTRCH_TABLES_NON_BCH (WFW_DBACK_NUM_CCTRCH_TABLES)

/** Indicate invalid MCAL WCDMA decoder CCTrCh Id value */
#define MCALWCDMA_DEC_INVALID_CCTRCH_ID_VAL 0xFF

/** Maximum number of DOB handles */
#define MCALWCDMA_DEC_MAX_DOB_HANDLES (WFW_DBACK_NON_HS_DECOB_STATUS_FIFO_SIZE+WFW_DBACK_HS_DECOB_STATUS_FIFO_SIZE)

/** Number of W32 to be read in transport block header information */
#define MCALWCDMA_DEC_TB_HDR_SIZE_W32 3
/** Number of word 32 data reads required for HS-DSCH MAC-d PDU */
#ifdef FEATURE_WCDMA_HS_FACH
/* Need two Word32 so that UE-ID is also read and given to MAC */
#define MCALWCDMA_DEC_HS_MAC_D_HDR_SIZE_W32 2
#else
#define MCALWCDMA_DEC_HS_MAC_D_HDR_SIZE_W32 1
#endif

/** This is the maximum number of transport block set for which the transport
 * block headers can be reported by L1 to L2 for decoding
 * Note that size defined is based on the maximum amout that can be
 * reported by mDSP. SW has to process that and should have that much capacity.
 * This is not as per UE capability specified in standard */
#define MCALWCDMA_DEC_MAX_TB_SET_HDR 32

/* Total number of TrCh table in mDSP */
#define MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH 12

/** Number of TPC reject logged per TPC index */
#define MCALWCDMA_DEC_LOG_MAX_TPC_INFO 6

/* Internal implementation of CEIL function be used in constants */
#define DEC_CEIL(x,y) (((x/y)*y)==x)?(x/y):((x/y)+1)

/*     CCTrCh header info       */    
/* ---------------------------- */

/* Number of 32 bit word in mDSP CCTrCh header entry */
#define DEC_MDSP_NUM_DWORD_CCTRCH_HDR     1

/* Transport channel table info */    
/* ---------------------------- */

/* Number of 32 bit word in mDSP transport channel table entry */
#define DEC_MDSP_NUM_DWORD_TRCHT_ENTRY    1
/* Number of 32 bit word in mDSP transport channel table */
#define DEC_MDSP_NUM_MAX_DWORD_TRCHT \
 (MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH * DEC_MDSP_NUM_DWORD_TRCHT_ENTRY)

/* TTI order table info */    
/* -------------------- */

/* Number of bits in a TTI order entry */
#define DEC_MDSP_NUM_BITS_TTI_ODR_ENTRY 8
/* number of word32 in TTI order entries */
#define DEC_MDSP_NUM_MAX_DWORD_TTI_ODRT \
 ((MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH * DEC_MDSP_NUM_BITS_TTI_ODR_ENTRY)/32)

/*      TF table info      */    
/* ----------------------- */

/* Total number of TF table in mDSP */
#define DEC_TOTAL_TFT_PER_CCTRCH 64
/* Maximum items in the TF pool */
#define MCALWCDMA_MAX_NUM_TF_POOL   WFW_DBACK_NUM_TF_POOL_TABLE_ENTRIES
/* Invalid TF pool index */
#define MCALWCDMA_INVALID_TF_POOL   0xFFFF

/* Number of bits to store one TF soft link. There can be 64 TF per non-MBMS 
   CCTrCh and a total of 64 TF for all MBMS S-CCPCH carrying MTCH. thus the 
   toal number of TF in use in steady state can be 128.
   The TF pool is of 256 elements so it requires 8 bits to access any of the 256 
   elements  */ 
#define MCALWCDMA_DEC_BITS_PER_TF         8
/* Number of bits of TF pool info in one W32 */
#define MCALWCDMA_NUM_BITS_TF_PER_W32     32
/* Number of 32 bit word in mDSP transport format table for store 64 TF .
   One W32 can store 4 TF so a total of 16 W32 are needed for 64 TF */
#define DEC_MDSP_NUM_MAX_DWORD_TFT   \
  (DEC_TOTAL_TFT_PER_CCTRCH/(MCALWCDMA_NUM_BITS_TF_PER_W32/MCALWCDMA_DEC_BITS_PER_TF))

/*     TFCI table info     */    
/* ----------------------- */

/* Number of 32 bit word in mDSP TFCI table entry */
#define DEC_MDSP_NUM_DWORD_TFCIT_ENTRY 3
/* Number of TFCI entries in CCTrCh table */
#define DEC_TOTAL_TFCI_PER_CCTRCH       128
/* Maximum items in the CTFC pool */
#define MCALWCDMA_MAX_NUM_TFCI_POOL   WFW_DBACK_NUM_TFCI_POOL_TABLE_ENTRIES
#define MCALWCDMA_INVALID_TFCI_POOL   0xFFFF

/* Number of bits to store one TFC soft link. Total number of TFC per CCTrCh is
   128 and the TFC pool is made of 288 elements so it requires 9 bits to access 
   any of the 288 elements  */ 
#define MCALWCDMA_DEC_BITS_PER_TFC        9
/* Number of bits used in one W32 to store TFCI pool indices 
   2 TFCI entries will be placed in one W32 so a total of 18 bits are used per W32 */
#define MCALWCDMA_NUM_BITS_TFCI_PER_W32  ((32/MCALWCDMA_DEC_BITS_PER_TFC) * MCALWCDMA_DEC_BITS_PER_TFC)
/* Total number of W32 to accomodate 128 TF. One W32 can store 2 TFCI and 14 bits 
   are unused */ 
#define DEC_MDSP_NUM_MAX_DWORD_TFCI_TABLE   \
  (DEC_CEIL(DEC_TOTAL_TFCI_PER_CCTRCH, (MCALWCDMA_NUM_BITS_TFCI_PER_W32/MCALWCDMA_DEC_BITS_PER_TFC)))


/* Maximum TF possible per TrCh (From RRC spec 25.331 2000-12 V3.5.0 Page 442,443) */
#define DEC_MAX_TF_PER_TRCH 32
/* maximum number of TB in one TF. This is set to limit of maximum number of
   TB that can be received from all TrCh in CCTrCh at end of frame */
#define DEC_MAX_TB_PER_TF 32
/* the max TrBlk size in bits */
#define DEC_MAX_TB_BIT_SIZE 6400

/* Total DRM buffer size of 1888 words in the non-concurrent mode */
#define DEC_DRM_BUFFER_SIZE 1888
#define DEC_DRM_BUF_SYMBOLS_PER_WORD WFW_DEMOD_DRMB_NUM_BANKS

#define DEC_DRM_BUF_SEGMENT_SIZE(size)  CEIL((size * 1), DEC_DRM_BUF_SYMBOLS_PER_WORD)

#define DEC_DRM_BUF_UNUSED              0xFFFF

#define DEC_DRM_BUF_HARQ_MEM_ST_DIR DEC_DRM_BUF_ST_DIR_DEC

#ifndef TEST_FRAMEWORK
#define IMEM_L1_BASE  0x58004000
#else
#error code not present
#endif

/* DOB buffer size */
#define DEC_MAX_VD_DOB_SIZE WFW_DEMOD_NON_HS_VD_DOB_MEM_SIZE 
#define DEC_MAX_TD_DOB_SIZE WFW_DEMOD_TD_DOB_MEM_SIZE 

/* minumum TB energy matrix for TBset for Conv coded TrCh.
   If the egy matric value is lower than this CRC is forced to fail */
#define DEC_MIN_TB_EGY_MATRIC_THRESH 256

/* Number of MAC-d PDUs size per Q received in one HS sub frame */
#define DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT 8

/* Number of word 16 in a status FIFO entry */
#define DEC_NUM_WORD16_PER_FIFO_ENTRY 5

/* Number of word 32 in a TB status */
#define DEC_NUM_WORD32_PER_TB_SET_STATUS_CONV_CODING  4
#define DEC_NUM_WORD32_PER_TB_SET_STATUS_TURBO_CODING 1

/* Maximum decoder output buffer status FIFO entries */
#define DEC_MAX_NON_HS_STATUS_FIFO_COUNT  WFW_DBACK_NON_HS_DECOB_STATUS_FIFO_SIZE
#define DEC_MAX_HS_STATUS_FIFO_COUNT      WFW_DBACK_HS_DECOB_STATUS_FIFO_SIZE

#define DEC_MAX_STATUS_FIFO_COUNT DEC_MAX_NON_HS_STATUS_FIFO_COUNT + \
                                  DEC_MAX_HS_STATUS_FIFO_COUNT

/* Invalid index. Used as general purpose indicator */
#define MCALWCDMA_DEC_IDX_INVALID 0xFF

/* Number of bits required for BCH block
 * PCCPCH 246 bits + 16 bit CRC + 8 Tail bits = 270
 */
#define DEC_BCH_BLOCK_SIZE_IN_BITS 270

/* DRM buffer size required for BCH code block */
#define DEC_DRM_BUF_SIZE_BCH_BLOCK DEC_DRM_BUF_SEGMENT_SIZE(DEC_BCH_BLOCK_SIZE_IN_BITS)

/* DRM buffer strorage direction macro */
#define DEC_DRM_BUF_ST_DIR_INC 1
#define DEC_DRM_BUF_ST_DIR_DEC 0

#define MCALWCDMA_DEC_INVALID_INDEX     0xFF

/* MAC-HS header fields */
/* -------------------- */

/* CRC length for HS-DSCH block */
#define DEC_HS_DSCH_BLK_CRC_LEN 24

/* Number of bits in varioys fields of MAC-hs header */   
#define DEC_HS_MAC_HS_HDR_VF_NUM_BITS         1 /* Version flag */
#define DEC_HS_MAC_HS_HDR_QID_NUM_BITS        3 /* Queue Id */
#define DEC_HS_MAC_HS_HDR_TSN_NUM_BITS        6 /* Transmission seq number */
#define DEC_HS_MAC_HS_HDR_SID_NUM_BITS        3 /* MAC-d PDU size indicator */
#define DEC_HS_MAC_HS_HDR_N_MACD_PDU_NUM_BITS 7 /* Number of MAC-d s*/
#define DEC_HS_MAC_HS_HDR_F_FLAG_NUM_BITS     1 /* Final flag bit */

/* Maximum possible size of MAC-hs header */   
#define DEC_HS_MAC_HS_HDR_LEN_MAX \
  ((DEC_HS_MAC_HS_HDR_VF_NUM_BITS + \
    DEC_HS_MAC_HS_HDR_QID_NUM_BITS + \
    DEC_HS_MAC_HS_HDR_TSN_NUM_BITS) + \
   ((DEC_HS_MAC_HS_HDR_SID_NUM_BITS + \
     DEC_HS_MAC_HS_HDR_N_MACD_PDU_NUM_BITS + \
     DEC_HS_MAC_HS_HDR_F_FLAG_NUM_BITS) * \
    MCALWCDMA_DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT))

/* Number of word32 required to extract MAC-hs fields of maximum length */
/*lint -emacro(506,DEC_HS_DOB_W32_READ_FOR_MAC_HS_HEADER_DECODE) Max number of W32 needed to read
  MAC-HS header is dependent on various field lengths of the header */ 
#define DEC_HS_DOB_W32_READ_FOR_MAC_HS_HEADER_DECODE \
  ((DEC_HS_MAC_HS_HDR_LEN_MAX & 0x1F) ? \
   ((DEC_HS_MAC_HS_HDR_LEN_MAX >> 5) + 1) : \
   (DEC_HS_MAC_HS_HDR_LEN_MAX >> 5))
    
/* Macro to extract MAC-hs fields */  
#define DEC_HS_MAC_HDR_GET_FIELD(buf,offset_ptr,field) \
  EXTRACT_BITS_FROM_BUF(buf,offset_ptr, DEC_HS_MAC_HS_HDR_##field##_NUM_BITS)

/* Spec 25.321 Max possible Reordering PDUs per HS TrBlk */
#define MCALWCDMA_DEC_HS_REORDER_PDU_MAX_COUNT 3

/*debug flag definitions */
#define MCALWCDMA_DEC_DEBUG_F3_DECODE_DONE_ISR 0x01
#define MCALWCDMA_DEC_DEBUG_F3_HEADER_READ_ISR 0x02
#define MCALWCDMA_DEC_DEBUG_F3_A2_CURRENT_TTI 0x04
#define MCALWCDMA_DEC_DEBUG_F3_A2_P_2_C 0x08
#define MCALWCDMA_DEC_DEBUG_F3_HS_PDU_BEFORE_DECIPHERING 0x10
#define MCALWCDMA_DEC_DEBUG_F3_R99_RELATED_DECIPHERING 0x20
#define MCALWCDMA_DEC_DEBUG_F3_A2_DONE_ISR 0x40
#define MCALWCDMA_DEC_DEBUG_F3_CRASH_UPON_DSM_EXHAUSTION 0x80
#define MCALWCDMA_DEC_DEBUG_F3_DSM_ALLOCATION 0x100
#define MCALWCDMA_DEC_DEBUG_F3_GENERAL_DEBUG_RELATED 0x200
#define MCALWCDMA_DEC_DEBUG_F3_LOGGING_RELATED 0x400
#define MCALWCDMA_DEC_DEBUG_F3_QICE_PN_DRIFT_RELATED 0x800
#define MCALWCDMA_DEC_DEBUG_F3_QSH_LOGGING 0x1000

#ifdef FEATURE_WCDMA_JOLOKIA_MODEM
#define MCALWCDMA_DEC_TDEC_DOB_START_ADDR (HWIO_DECOB_MEM_BASE_MIN_ADDR )
#elif defined (FEATURE_THOR_MODEM) || defined(FEATURE_WCDMA_ATLAS_MODEM )
/* TODO : Correct macro name needs to be obtained from HW/Core team */
#define MCALWCDMA_DEC_TDEC_DOB_START_ADDR (0x0)
#else
/* DOB start address for TDEC */
#define MCALWCDMA_DEC_TDEC_DOB_START_ADDR (HWIO_MEM_POOL_AXI_START_ADDR + 22784)
#endif

/* DOB status item bit mask shifts */
/* ------------------------------- */

/* for viterbi decoder */
#define DEC_DOB_ST_VD_TB_CRC_BITS_BMSK 0xFFFFFFFF
#define DEC_DOB_ST_VD_TB_CRC_BITS_REG  0
#define DEC_DOB_ST_VD_TB_CRC_BITS_SHFT 0

#define DEC_DOB_ST_VD_TRCH_EGY_MAT_BMSK 0x0000FFFF
#define DEC_DOB_ST_VD_TRCH_EGY_MAT_REG  1
#define DEC_DOB_ST_VD_TRCH_EGY_MAT_SHFT 0

#define DEC_DOB_ST_VD_TRCH_SE_CNT_BMSK 0xFFFF0000
#define DEC_DOB_ST_VD_TRCH_SE_CNT_REG  2
#define DEC_DOB_ST_VD_TRCH_SE_CNT_SHFT 16

#define DEC_DOB_ST_VD_CB_Q_BITS_BMSK 0x0000FFFF
#define DEC_DOB_ST_VD_CB_Q_BITS_REG  2
#define DEC_DOB_ST_VD_CB_Q_BITS_SHFT 0

/* Last Best Metric */
#define DEC_DOB_ST_VD_TRCH_LBM_BMSK  0x00007F00
#define DEC_DOB_ST_VD_TRCH_LBM_REG   3
#define DEC_DOB_ST_VD_TRCH_LBM_SHFT  8

/* Last State Metric */
#define DEC_DOB_ST_VD_TRCH_LSM_BMSK  0x0000007F
#define DEC_DOB_ST_VD_TRCH_LSM_REG   3
#define DEC_DOB_ST_VD_TRCH_LSM_SHFT  0

/* VD energy metric 32 bit word */
#define DEC_DOB_ST_VD_TRCH_ZSB_BMSK  0xFFFFFFFF
#define DEC_DOB_ST_VD_TRCH_ZSB_REG   3
#define DEC_DOB_ST_VD_TRCH_ZSB_SHFT  0

/* for turbo decoder */
#define DEC_DOB_ST_TD_TB_CRC_BITS_BMSK 0xFFFFFFFF
#define DEC_DOB_ST_TD_TB_CRC_BITS_REG  0
#define DEC_DOB_ST_TD_TB_CRC_BITS_SHFT 0

/* for uncoded TrChs */
#define DEC_DOB_ST_UNC_TB_CRC_BITS_BMSK 0xFFFFFFFF
#define DEC_DOB_ST_UNC_TB_CRC_BITS_REG  0
#define DEC_DOB_ST_UNC_TB_CRC_BITS_SHFT 0

#define DEC_DOB_ST_UNC_TRCH_EGY_MAT_BMSK 0x0000FFFF
#define DEC_DOB_ST_UNC_TRCH_EGY_MAT_REG  1
#define DEC_DOB_ST_UNC_TRCH_EGY_MAT_SHFT 0

#define DOB_TB_SET_STATUS_ITEM(buffer, item, coding_type) \
  (((buffer)[DEC_DOB_ST_##coding_type##_##item##_REG] & \
    DEC_DOB_ST_##coding_type##_##item##_BMSK) >> \
   DEC_DOB_ST_##coding_type##_##item##_SHFT)

/* Invalid HS DOB transfer address*/
#define MCALWCDMA_DEC_INVALID_DOB_XFER_ADDR 0xFFFF
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/** MCAL WCDMA decoder CCTrCh id type that is returned for CCrCh setup request */
typedef uint8 mcalwcdma_dec_cctrch_id;

/** MCAL WCDMA decoder DOB status handle type definition */
typedef uint32 mcalwcdma_dec_dob_status_handle_type;

/* Call back function type declaration */
/* ----------------------------------- */

/* Enum type declaration */
/* --------------------- */

#ifdef FEATURE_HSDPA_MAC_EHS
/* L1 will interpret the segmentation index type and then assign a segmentation type
to each MAC-ehs PDU within the reordering pdu.

OTA_Reordering_Segmentation_Type  MAC-ehs_segmentation_type

01                            The first mac-ehs sdu will be of type partial end and the remaining
                              mac-ehs pdus will be having complete pdu as segmentation type
10                            The last mac-ehs sdu will be of type partial start and the remaining
                              mac-ehs pdus will be having complete pdu as segmentation type
11                            If there is only 1 mac-ehs sdu in the reordering pdu then it will be of
                              type partial middle, start or end. If there is 2 or more then
                              the first one will be partial end and the last one will be partial start
00                            All mac-ehs sdus are complete pdus

*/
typedef enum
{
  COMPLETE_PDU,
  PARTIAL_FIRST,
  PARTIAL_MIDDLE,
  PARTIAL_LAST
} mcalwcdma_dec_rlc_pdu_segmentation_type;
#endif /* FEATURE_HSDPA_MAC_EHS */

#ifdef FEATURE_WCDMA_HS_FACH
/* This enum defines the type of H-RNTI used
   to decode the HS TrBlk */
typedef enum
{
  L1_DL_BCCH_H_RNTI_TYPE,
  L1_DL_COMMON_H_RNTI_TYPE,
  L1_DL_DEDICATED_H_RNTI_TYPE,
  L1_DL_BLIND_DECODING_TYPE
} mcalwcdma_dec_e_fach_h_rnti_type;
#endif /* FEATURE_WCDMA_HS_FACH */


/*----------------------------------------------------------------------------
   Enum indicated by L1 to MAC along with HS data inidcation in order to make 
   decision for processing the current, prev or both TTIs
 -----------------------------------------------------------------------------*/
/* HSDPA Action Variable */
typedef enum
{
  MACDL_HS_CURR_TTI,
  MACDL_HS_PREV_TTI,
  MACDL_HS_CURR_PREV_TTI,
  MACDL_HS_NONE
} mcalwcdma_dec_mac_hs_tti_type;





/** Transparant mode deciphering states for various transport block */
typedef enum
{
  L1_TB_TM_CIPHER_NOT_APPLICABLE, /**< Block not ciphering in TM mode */
  L1_TB_TM_CIPHER_START,          /**< First block iciphered in TM mode */
  L1_TB_TM_CIPHER_CONT,           /**< Continue ciphering key stream generation */
  L1_TB_TM_CIPHER_STOP,           /**< Last block iciphered in TM mode */
  L1_TB_TM_CIPHER_SINGLE          /**< Single block in TM mode so don't */
} mcalwcdma_dec_tb_tm_cipher_state_enum_type;

typedef enum 
{
  DL_EVEN_CFN_PCCPCH,
  DL_ODD_CFN_PCCPCH,
  DL_BOTH_CFN_PCCPCH
} dl_cctrch_cfn_type ;


/** Strcuture for DOB status information */ 
typedef struct
{
  uint32 crc_bits;

  uint16 energy_metric; /**< energy metric of the TB set */
  uint16 sym_err_count; /**< symbol error count for TB set */
  /** CRC error bits of each TB. MSB is last block received in this TBset */

  uint16 quality_bits;  /**< quality bits for TB set */
  /** Valid bit mask for various information below. */
  uint8 valid_bit_mask;

  /** Coding type of transport chanel */
  mcalwcdma_dec_coding_enum_type coding_type;

  boolean zsb;          /**< zero state bit */
  uint32 zsb_word;
} mcalwcdma_dec_dob_status_type;


/** This structure defines information for HSDPA MAC-d PDU size */ 
typedef struct
{
  /** indicate this Size index Id is valid */
  boolean valid;
  /** MAC-d PDU size for this SID */
  uint16 size;
} mcalwcdma_dec_hs_mac_d_pdu_sz_info_struct_type;

/** This structure defines information for HSDPA Q information for its MAC-d PDI size */ 
typedef struct
{
  /** indicate this Q Id is valid */
  boolean valid;

  /** indicate Size info for each SID (Size identifier)
   * SID goes with index of this array */
  mcalwcdma_dec_hs_mac_d_pdu_sz_info_struct_type
    mac_d_pdu_sz_info[MCALWCDMA_DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT];

} mcalwcdma_dec_hs_mac_d_pdu_sz_q_info_struct_type;

/** This structure defines the information for HSDPA information table for
 * getting MAC-d PDU out of MAC-HS block */
typedef struct
{
  /** MAC-d queue information for this HS table */
  mcalwcdma_dec_hs_mac_d_pdu_sz_q_info_struct_type
    mac_d_pdu_sz_q_info[MCALWCDMA_DEC_HS_MAC_HS_Q_MAX_COUNT];
} mcalwcdma_dec_hs_info_struct_type;


/* Structures for Header indication to L2 */

/** This structure defines the ciphering parameters for a transport block
 * header information of a transport block */
typedef struct
{
  /** Ciphering enabled or not for this 
   * transport block. */
  boolean ciphering_control;
  /** Ciphering algorithm specified by L2 for this
   * transport block. */
  mcalwcdma_dec_ciphering_algo_enum_type ciphering_algo;
  
  /** Ciphering bit offset returned by L2 of each transport block. */
  uint8 ciphering_bit_offset;
  
  /** Ciphering key id returned by L2 of this transport block. */
  uint8 ciphering_key_id;
  
  /** Ciphering count (CFN/SFN) returned by L2 of this transport block. */
  uint32 ciphering_count;
  
  /** Ciphering radio bearer id returned by L2 for this transport block. */
  uint8 ciphering_radio_bearerid;
} mcalwcdma_dec_ciphering_param_struct_type;

/** This structure defines the transport block header information for a
 * transport block */
typedef struct
{
  /** TB Id. This is set and maintained by MAC. Its passed back to MAC in
   * data indication */
  uint16  tb_id;
  
  /** Valid flag for this TB. If MAC finds any problem with this TB
   * it will set this TB valid flag to FALSE otherwise it will be TRUE */
  boolean valid;

  /* Flag to discard the DL PDUs received while the RESET ACK was being processed */
  boolean discard_l2_pdu;
  
  /** TB index of this TB */
  uint8 tb_idx; 
  
  /** Logical channel ID for MAC to fill up in Rel5-Rel6 case,
   *  irrespective of C/T mux. */
  uint8 lc_id;

  /** RLC Id corresponding to lc_id - to be filled by MAC 
   * This is needed to pass to RLC because RLC doesnt have notion of
   * lc_id. It only understands rlc_id */
  uint8 rlc_id;

  /** Bit offset from the starting transport block. The offset specified in
   * in the number of bits. The L2 function that decode the header, reports
   * back the offset to read the transport block data in this strcuture member */
  uint8 num_bit_offset_trblk;
  
  /** ciphering parameters for this transport block */
  mcalwcdma_dec_ciphering_param_struct_type ciphering_param;
  
  /** Pointer to the transport block header */
  uint8 tb_header[MCALWCDMA_DEC_TB_HDR_SIZE_W32 * 4];
} mcalwcdma_dec_tb_hdr_struct_type;


/* IOVEC definiton:
 * An IOVEC is a data structure that contains pointer to a DSM item,
 * offset within the DSM item in bytes and the length of the PDU
 * in bytes.
 */
typedef struct
{
  dsm_item_type *ptr; /* Pointer to DSM item */
  uint16 offset; /* Offset in bytes from the beginning of data 
                    section of the DSM item */
  uint16 length; /* Length in bytes of the PDU */
} mcalwcdma_dec_iovec_type;

/* Each complete pdu will reside in one dsm item. The dsm item is large enough to hold
multiple complete pdus. rlc_hdr_data is the demulitplexed header portion from the rlc pdu
these are separate so that the data can be contiguous and can be shipped to higher layers
avoiding a dsm copy and a new dsm alloc */

typedef struct
{
  mcalwcdma_dec_iovec_type iovec; /* IOVEC for this complete PDU */

  uint32 rlc_hdr_data; /* RLC Header contents */

  /* For rel-7 LC_ID is known to L1 while doing the header read event. For rel-6 the MAC will
  pass the LC_ID information at the end of the header read event. DSM items are allocated
  per lc_id basis hence L1 needs to be aware of the lc_id for rel6 too.
  This is NW configured Logical Channel ID <1..15> and not internal RLC ID */
  uint8 rlc_id; /* Logical Channel ID */
} complete_pdu_type;

#ifdef FEATURE_HSDPA_MAC_EHS
typedef struct
{
  mcalwcdma_dec_iovec_type iovec; /* IOVEC for this partial PDU */

  /* L1 uses this variable to indicate to MAC whether
     RLC PDU header is received completely or not. */
  boolean complete_rlc_hdr_rcvd;
  
  uint32 rlc_hdr_data; /* RLC Header contents */
  
  /* This indicates if the demultiplexed PDU is a partial first, middle or last*/
  mcalwcdma_dec_rlc_pdu_segmentation_type segmentation_info; /* Segmentation indication */

  uint8 lc_id; /* Logical Channel ID to which this pdu belongs*/
} partial_pdu_type;
#endif

/** Enum type definition for TSN trace enum type */
typedef enum {
  DL_TSN_FREE_Q = 1, /**< TSN is in Dec TSN info free Q */
  DL_TSN_L1_INT_Q,   /**< TSN is in L1 Internal Q */
  DL_TSN_INFO_Q,     /**< TSN is in Dec TSN info Q */
  DL_TSN_MAC_INT,    /**< TSN is in MAC internal structures */
  DL_TSN_RLC_Q,      /**< TSN is in RLC TSN info Q */
  DL_TSN_RLC         /**< TSN is in RLC */
} mcalwcdma_dec_tsn_trace_enum_type;

/** If a TSN structure passed to MAC has both complete
 *  and partial pdu info, then RLC should not release the
 *  structure. MAC will have to free this structure when the
 *  partial PDUs are taken care of. */
typedef struct
{
  q_link_type link; /**< Link to manage this item on the queue */
  uint8 index;      /**< index of this element in array */

  /* Indicates which module is holding the TSN items */
  mcalwcdma_dec_tsn_trace_enum_type tracer;

  /** UE can receive Complete PDU, Complete PDU and Partial Start
   *  of another PDU in that order. But this structure
   *  organization destroys the original order. Seems ok from MAC
   *  point of view as of now, the order is not important. */

  /* TSN decoded as part of the Header Read event 
     14 bits for >= 3C and 6 bits for < 3C */
  uint16 tsn;

  /* Queue ID decoded as part of the Header Read event */
  uint8 queue_id;

  /** TB Id. This is set by MAC during Header read event. Its
   * passed back to MAC in data indication */
  uint16 tb_id;

  /** Flag for MAC usage */
  boolean received;

 #ifdef FEATURE_WCDMA_HS_FACH
  /* Enum to indicate H-RNTI used to decode this block */
  mcalwcdma_dec_e_fach_h_rnti_type h_rnti_type;
 #endif

  /* While freeing this structure (either MAC/RLC), set this field to 0.
   * L1 will set them to zero also, during Q init. */
  uint8 num_complete_pdus;
  
 #ifdef FEATURE_HSDPA_MAC_EHS
  /* While freeing this structure (either MAC/RLC), set this field to 0.
   * L1 will set them to zero also, during Q init. */
  uint8 num_partial_pdus;
 #endif

  complete_pdu_type complete_pdu[MCALWCDMA_DEC_MAX_DL_RLC_PDU];

 #ifdef FEATURE_HSDPA_MAC_EHS
  /*UE_MAX_PARTIAL_PDU_PER_REORDER_PDU=2 since in a reordering pdu either the
  first mac-ehs sdu could be a partial one and/or the last mac-ehs sdu in the
  reordering pdu could be a partial one*/
  partial_pdu_type partial_pdu[MCALWCDMA_DEC_MAX_PARTIAL_PDU_PER_REORDER_PDU];
 #endif
} mcalwcdma_dec_tsn_info_type;

/** This structure defines the DSM information per logical
 *  channel. */
typedef struct
{
  /** DSM item being populated */
  dsm_item_type *current_dsm_ptr;

  /** Offset in the DSM ptr where the next TB data can be
   *  populated (units of bytes). */
  uint16 offset;
} mcalwcdma_dec_dsm_info_type;

#ifdef FEATURE_HSDPA_MAC_EHS

/* This structure identifies the number of partial PDU segements which forms
a complete PDU. These segments will belong to different TSN's and resides in the
reassembly Q in MAC. Once MAC finds that a complete PDU can be formed, this
is populated and given as part of the header read event along with
deciphering information for complete PDUs already residing in the DOB*/

/** This structure provides the location of various segments of
 *   this RLC PDU and also contains deciphering information
 *   from RLC, if necessary. */
typedef struct
{
  /* For rel-7 LC_ID is known to L1 while doing the header read event. For rel-6 the MAC will
  pass the LC_ID information at the end of the header read event. DSM items are allocated
  per lc_id basis hence L1 needs to be aware of the lc_id for rel6 too*/
  uint8 lc_id;

  /** RLC Id corresponding to lc_id - to be filled by MAC 
   * This is needed to pass to RLC because RLC doesnt have notion of
   * lc_id. It only understands rlc_id */
  uint8 rlc_id;
  
  /* Number of Partial PDUs that complete this RLC PDU */
  uint16 num_partial_pdus;
  /* Length in bytes of the complete RLC PDU. Populated
   * by MAC. This is required by L1 in order to prepare 
   * the move-out task. If this field is not present, 
   * L1 has to again loop through all the source descriptor 
   * list entries and compute the length of each complete PDU */
  uint16 length;
  /* TSN info structure corresponding to first segment of the 
   * RLC PDU. */
  mcalwcdma_dec_tsn_info_type *tsn_info_buf;

  /** ciphering parameters for this RLC PDU */
  mcalwcdma_dec_ciphering_param_struct_type ciphering_param;
} mcalwcdma_dec_partial_rlc_pdu_hdr_struct_type;

/** This structure defines the information for all RLC PDUs that can
 *  be constructed out of multiple partial PDUs. This structure
 *  also contains the information required to get the data
 *  from the hardware. */
typedef struct
{
  /** Number of complete RLC PDUs that can be formed */
  uint16 num_complete_pdus;

  /* for each complete RLC PDU that is being formed */
  mcalwcdma_dec_partial_rlc_pdu_hdr_struct_type rlc_pdu_hdr_info[MCALWCDMA_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI];
} mcalwcdma_dec_rlc_pdu_concatenate_info_struct_type;

typedef struct
{
  /* For rel-7 LC_ID is known to L1 while doing the header read event. For rel-6 the MAC will
  pass the LC_ID information at the end of the header read event. DSM items are allocated
  per lc_id basis hence L1 needs to be awar of the lc_id for rel6 too*/
  uint8 lc_id;        /* Logical Channel ID */
  uint16 tsn;          /* Transmission seq number 14 bits for >= 3C and 6 bits for < 3C */
  uint16 length;      /* Length of MAC e-HS SDU in bytes*/
  mcalwcdma_dec_rlc_pdu_segmentation_type segmentation_info; /* Complete or Partial PDU */
} mcalwcdma_dec_mac_ehs_sdu_hdr_type;

/** Every PDU formed out of partial PDUs from previous TTIs
 *  will have a corresponding TSN provided by MAC. The list
 *  of TSNs corresponding to each complete PDU will be queued
 *  to MAC as part of a queue separate from the queue for
 *  passing current TTIs TSN info structures. */
typedef struct
{
  q_link_type link; /**< Link to manage this item on the queue */
  uint8 index;      /**< index of this element in array */

  /** TSN corresponding to first segment of a complete PDU */
  mcalwcdma_dec_tsn_info_type *tsn_info_buf;
} mcalwcdma_dec_concatenated_pdu_tsn_type;

extern mcalwcdma_dec_dsm_info_type partial_pdu_dsm_info[MCALWCDMA_DEC_MAX_LC_ID];

/** This array stores the TSNs for which complete PDUs were
 * formed from previous TTIs partial PDUs. L1 retrieves a buffer
 * from the Free Q, populates it and enqueues into a Q to MAC.
 * This data structure is triple-buffered since if MAC-hs
 * task does not get a chance to run for three 10ms frames, even
 * then the information can be held in the queue. */


/* Q of TSN value structures */
/* L1 pulls structures out of Free Q, fills it and enqueues them in Q */
/* MAC uses the structures in Q and puts them back in Free Q */


#endif /* FEATURE_HSDPA_MAC_EHS */




/** This structure defines the transport block header information for a
 * transport block set (or a transport channel) */
typedef struct
{
  /** Number of transport blocks in this transport block set. */
  uint8 num_tb;
  
  /** CCTrCh Ids for the transport block set. This is set by WHL1 */
  uint8 cctrch_id;
  
  /** TrCh reference type for the transport block set */
  mcalwcdma_dec_trch_ref_type_enum_type trch_ref_type;
  

  /** HS and non-HS channel information */
  union
  {
    /** R99 non-HS channel information */
    struct
    {
      uint8 cfn; /**< CFN for this transport block set */
      uint8 trch_id; /**< TrCh Ids for the transport block set. Set by WHL1 */
      uint8 tfi; /**< TF index for this TBset or TrCh */
      uint8 num_total_tb; /**< Total number of TB in the TF of this TBset */
      uint16 tb_num_bits; /* Number of bits in this TB */
    } non_hs;

    /** HS channel information */
    struct
    {
      uint16 sub_frame_num; /**< Starting Sub frame number when TTI is received */
      uint8 version_flag; /**< version flag 1 bit */
      uint8 queue_id; /**< Queue Id 3 bits */
      uint8 tsn; /**< Transmission seq number 6 bits */
      uint8 num_sid_in_hdr; /**< k of SIDk,Nk pair in MAC HS header up to 8 */
      /** Nk value for each k */
      uint8 num_blk[MCALWCDMA_DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT];
      /** SID value for each k */
      uint8 sid_val[MCALWCDMA_DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT];
    } hs;

   #ifdef FEATURE_HSDPA_MAC_EHS
    /** E-HS channel information */
    struct
    {
      uint16 sub_frame_num; /* Starting Sub frame number when TTI is received */
      uint8 num_reorder_sdu; /* Number of Reordering SDUs in this HS TTI */
     #ifdef FEATURE_WCDMA_HS_FACH
      /* Enum to indicate H-RNTI used to decode this block */
      mcalwcdma_dec_e_fach_h_rnti_type h_rnti_type;
     #endif
      /* LC-ID, Length, TSN and Segmentation Type of each MAC-eHS SDU in this HS TTI */
      mcalwcdma_dec_mac_ehs_sdu_hdr_type mac_ehs_sdu_hdr[MCALWCDMA_DEC_HS_REORDER_SDU_MAX_COUNT];
    } ehs;
   #endif /* FEATURE_HSDPA_MAC_EHS */
  } info;


  /** indicate that TBs in this TB set are in transparent mode */
  boolean tb_in_tm_mode;
  /** Pointer to the structure array of information of all
    transport block header in this TB set */
  mcalwcdma_dec_tb_hdr_struct_type *tb_hdr_info;
} mcalwcdma_dec_tb_set_hdr_struct_type;

/** This structure defines the information of all transport block headers
 * information for all transport block sets for which the headers need to be
 * decoded and information is required to get the data from the hardware. */
typedef struct
{
  /** Number of transport block set for which the TrBlk header is to be
   * decoded for all transport blocks in them */
  uint8 num_tb_set;
  
  /** transport block set information for all the transport block headers
   * in each transport block set */
  mcalwcdma_dec_tb_set_hdr_struct_type tb_set_hdr[MCALWCDMA_DEC_MAX_TB_SET_HDR];

 #ifdef FEATURE_HSDPA_MAC_EHS
  /** Information for all RLC PDUs that can be constructed out of
   *  multiple partial PDUs. */
  mcalwcdma_dec_rlc_pdu_concatenate_info_struct_type rlc_pdu_concatenate_info;
 #endif
} mcalwcdma_dec_tb_hdr_decode_struct_type;

/* Structures for Data indication to L2 */

/** This structure defines the data element for a transport block in a 
 * transport block set */
typedef struct
{
  /** TB Id. This is set and maintained by MAC */
  uint16  tb_id;
  
  /** RLC PDU size of the data in this TB in bits. In case of loopback mode 2
   * for the DCH TrCh the data size will include the CRC length also */
  uint16 data_size;
  
  /** data buffer size is bytes used to accomodate the RLC PDU bits */
  uint16 buffer_size_bytes;
  
  /** DSM item to store payload */
  dsm_item_type *tb_data_buffer;
} mcalwcdma_dec_tb_data_in_tbset_struct_type;

/** This structure defines the data and status element for a transport block set */
typedef struct
{
  /** TrCh reference type for the transport block set */
  mcalwcdma_dec_trch_ref_type_enum_type trch_ref_type;

  /** Number of transport blocks in this transport block set. */
  uint8 num_tb;
  
  /** Confidence in the Decode result - True: Good, False: Bad */
  boolean decode_result;
  
  /** transport block size for the transport block of this trblk set */
  uint16 trblk_size;
  
  /** points to the array of the TB info structure */
  mcalwcdma_dec_tb_data_in_tbset_struct_type *tb_data_info;
} mcalwcdma_dec_tb_set_data_struct_type;

/** This structure defines the transport block information that is used to get
 * the transport block headers decoded and update the transport block
 * information to the L2 buffers */
typedef struct
{
  /** Number of transport block set for which the data and status is to be
     transferred to L2. 
     If the feature FEATURE_WCDMA_DL_DATA_PATH_OPT is defined, field is valid
     only for R99 data. If only HS data is present, it will be set to zero.*/
  uint8 num_tb_set;
  
  /** Data and status for all transport blocks sets */
  mcalwcdma_dec_tb_set_data_struct_type tb_set_data[MCALWCDMA_DEC_MAX_TB_SET_HDR];
} mcalwcdma_dec_tb_data_struct_type;


#ifdef FEATURE_DL_PATH_PROFILING
typedef struct
{
  uint32 cfn;
  uint32 cx1;
  uint32 rd_idx;
  uint32 wr_idx;
  uint32 start_addr;
  uint32 hs_trch_type;
  uint32 fw_nonhs_counter;
  uint32 sw_nonhs_counter;
  uint32 fw_hs_counter;
  uint32 sw_hs_counter;
  uint32 timetick;
} dec_decode_done_isr_type;

typedef struct
{
  uint32 decode_start;
  uint32 decode_step2;
  uint32 decode_mid;
  uint32 decode_end;
} dec_decode_done_isr_start_end_type;

typedef struct
{
  uint32 trigger_ts;
  uint32 cb_ts;
  uint32 idx;
  uint8 num_ptoc;
} dec_gather_and_decipher_struct_type;

typedef struct
{
  uint8  num_non_hs_dob_handles;
  uint8  num_hs_dob_handles;
  uint8  num_partial;
  uint8  num_complete;
  uint32 hdr_rd_start;
  uint32 hdr_rd_step2;
  uint32 hdr_rd_end;
  uint32 l2_start;
  uint32 l2_end;
  uint32 p2_start;
  uint32 p2_end;
  uint32 cur_tbset_proc_start;
  uint32 prev_tbset_proc_start;
  uint32 dmov_trig_start;
} dec_hdr_rd_isr_struct_type;

typedef struct
{
  uint32 cfn;
  uint32 cx1;
  uint32 timetick;
} dec_data_rd_struct_type;

typedef struct
{
  uint32 dma_done;
  uint32 tsn_enqueue_done;
  uint32 l2_ind_done;
} dec_dma_from_dob_struct_type;
#endif

/* CCTrCh reference type enum type */
typedef enum
{
  DEC_CCTRCH_REF_TYPE_BCH,      /* CCTrCh with BCH type TrCh */
  DEC_CCTRCH_REF_TYPE_PCH_FACH, /* CCTrCh with PCH and/or FACH type TrCh */
  DEC_CCTRCH_REF_TYPE_DCH,      /* CCTrCh with DCH type TrCh */
  DEC_CCTRCH_REF_TYPE_INVALID   /* Used when can't generate CCTrCh ref type */
} dec_cctrch_ref_type_enum_type;

/* This enum defines the rate matching type for mDSP rate matching
   parameters */
 typedef enum
{
  DEC_NO_RM,     /* No rate matching to be used */
  DEC_RM_REPEAT, /* Repetition used for rate matching */
  DEC_RM_PUNCT   /* Puncturing used for rate matching */
} dec_rm_type_enum_type;

/* This enum defines the endian-ness types of the DOB */
typedef enum
{
  /* litle endian mode used to read the word32 status data */
  DEC_DOB_LITTLE_ENDIAN,
  /* big endian mode used for access the data octets as word32 reads */
  DEC_DOB_BIG_ENDIAN
} dec_dob_endian_type_enum_type;

/* Type of data transfer mode */
typedef enum
{
  DEC_DATA_XFER_MODE_BG_READ,
  DEC_DATA_XFER_MODE_READ_W_IMME_TM_CIPHER
} dec_data_xfer_mode_eum_type;

/* Structure type declaration */
/* -------------------------- */

/* DRM Buffer information structure */
typedef struct
{
  /* Availability status */
  boolean available;

  /* DRM buffer start address */
  uint16 drm_buffer_start_addr;

  /* DRM buffer size */
  uint16 drm_buf_size;

  /* DRM buffer size in bits */
  uint16 drm_buf_size_bits;

  /* DRMB ID to CCTrCh mapping */
  uint8 cctrch_idx;

  /* CcTrCh reference type that is using DRM buffer area */
  dec_cctrch_ref_type_enum_type cctrch_ref;
} mcalwcdma_dec_drm_buffer_info_struct_type;

/* TF pool structure */
typedef struct
{
  /* PLEASE DO NOT CHANGE THE ORDER OF ELEMENTS
     IN THIS STRUCTURE. 
     The members are placed strategically for memory optimization */
  uint8  next;    /**< Link to the next node */
  uint8  num_tb;  /**< Number of transport block in TBS. */
  uint16 tb_size; /**< Transport block size. */
}mcalwcdma_dec_tf_pool_info_struct_type;

typedef struct
{
  uint8  num_tb;   /**< Number of transport block in TBS. */
  uint16 tb_size;  /**< Transport block size. */
} dec_cctrch_table_tf_info_struct_type;

typedef struct
{
  /* Transport channel ID. Used only for logging */
  uint8 trch_id;

  /* Transport channel reference type */
  mcalwcdma_dec_trch_ref_type_enum_type trch_ref_type;

  /* FACH TrCh type 
     0x00   R99 FACH TrCh 
     0x01   MCCH FACH TrCh
     0x02   MTCH FACH TrCh
     0x04   MSCH FACH TrCh
     0xFF   Not a FACH transport channel */
  uint8 fach_trch_type;
  
  /* Coding type of transport chanel */
  mcalwcdma_dec_coding_enum_type coding_type;

  /* CRC length in bits */
  uint8 crc_length;

  /* TTI duration of transport channel */
  mcalwcdma_dec_tti_enum_type tti_value;

  /* DRMB size of TrCh in bits */
  uint16 drm_buffer_size;

  /* Loop back mode for this TrCh. Only Applicable if TrCh ref type is DCH */
  mcalwcdma_dec_loop_back_mode_type_enum_type loopback_mode;

  /* Rate matching attribute. USed only for logging */
  uint16 rate_match_attrib;

  /* Number of TF in this TrCh */
  uint8 num_tf;

  /* TFT base in TF table */
  uint16 tft_base;

  /* TFT index for this transport channel max sized TF */
  uint8 max_size_tft_idx;


  /* Indicates whether this TrCh is of type DCCH */
  boolean is_dcch;

  /* Indicates whether this TrCh carries voice */
  boolean is_voice;
} dec_cctrch_table_trch_info_struct_type;

/* CCTrCh table information */
typedef struct
{
  /* CCTrCh table in use flag */
  boolean in_use;

  /* CCTrCh ID for which this index is being used */
  uint8 cctrch_id;
  
  /* CCTrCh Table index corresponding to the information contained here */
  uint8 table_index;

  /* indicate number of TFC in CTFC table */
  uint8 num_tfc;

  /* CCTrCh reference type, indicate type of TrCh in that CCTrCh */
  dec_cctrch_ref_type_enum_type ref_type;

  /* DRM buffer segment index */
  uint8 drm_buf_area_id;

  /* Second DRM buffer segment index - used for PCCPCH and MTCH soft combining */
  uint16 sec_drm_buf_area_id;

  /* The field is used only for PCCPCH CCTrCh */
  /* Even or odd PCCPCH CFN enabled */
  dl_cctrch_cfn_type even_odd_cfn;

  /* Numner of TrCh in this CCTrCh */
  uint8 num_trch;

  /* TrCh info structure */
  dec_cctrch_table_trch_info_struct_type trch_info[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* TF info structure */

  /* Total number of TF in this CCTrCh */
  uint8 num_tf;

  /* Index of the TF pool for each TF in CCTrCh */
  uint8 tf_pool_index[DEC_TOTAL_TFT_PER_CCTRCH];

  /* Head for the TFCI pool. The TFCI pool can be traversed from this index till index+num_tfc */
  uint16 tfci_pool_head;

  /* CTCH presence */
  boolean ctch_present;

  mcalwcdma_dec_rm_method_enum_type rm_method;

} dec_cctrch_table_info_struct_type;

/* This structure defines the CCTrCh table structure for mDSP. This includes the
   CCTrCh header that has CTFC table index, and number of TrCh. This is followed
   by transport channel (TrCh) table and transport format (TF) table */
typedef struct
{
  uint32 cctrch_header; /* CCTrCh table header. */

  /* Transport chanel index sorted by their TTI length */
  uint32 trch_tti_order_table[DEC_MDSP_NUM_MAX_DWORD_TTI_ODRT];
  
  /* Transport channel table */
  uint32 trch_table[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH][DEC_MDSP_NUM_DWORD_TRCHT_ENTRY];
  
  /* Transport format table */
  uint32 tf_table[DEC_MDSP_NUM_MAX_DWORD_TFT];

  /* TFCI table */
  uint32 tfci_table[DEC_MDSP_NUM_MAX_DWORD_TFCI_TABLE];

  /* The number of W32 needed for storing TF and TFCI info in this CCTrCh */
  uint8 dec_max_tf_w32_in_cctrch;
  uint8 dec_max_tfc_w32_in_cctrch;
} dec_cctrch_table_struct_type;

typedef struct
{
  /* Store TrCh table value */
  uint32 trch_table_value[DEC_MDSP_NUM_DWORD_TRCHT_ENTRY];
}mcalwcdma_dec_trch_table_info_struct_type;

/* This structure stores the CCTrCh header and the corresponding address */
typedef struct
{
  uint32 cctrch_header[DEC_MDSP_NUM_DWORD_CCTRCH_HDR];
  mcalwcdma_dec_trch_table_info_struct_type trch_info[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH];
}mcalwcdma_dec_header_info_struct_type;

/* This structure defines code block parameters for a transport format. */
typedef struct
{
  /* Code block size after code block segmentation. */
  uint16 code_blk_size;
  
  /* Number of code blocks after the code block segmentation */
  uint16 num_code_blks;
  
  /* Number filler bits or padded bits after the code block segmentation. 
    These bits are always 0 and are added at the begining of code block
    or first segment. */
  uint16 num_filler_bits;
} mcalwcdma_dec_code_blk_param_struct_type;

/* This structure defines turbo parameters that are required by
   turbo decoder. */
typedef struct
{
  /* Interleaver length. This is derived directly from the code block size. */
  uint16 interleaver_length;
  
  /* Number of maximum iteration. The possible values are 6 to 8 depending
    on the code rate. */
  uint8 max_iterations;
  
  /* Row index for the interleaving pattern. The possible values are B00 to
    B11 depending number of rows. This value indicates the pattern index
    used in inter-row permutations */
  uint8 row_index;
  
  /* Number of columns. The possible values are 5 to 257 depending number of
    rows. The value in this variable is one less than the actual number
    of columns. */
  uint16 num_columns;
  
  /* Row and column product. The product is between the actual number of
    column and rows and not as in variable row_index and num_columns */
  uint16 row_column_product;
  
  /* Prime number that is used to decide the number of column value. The
    possible values of the prime number  are 7 to 257. */
  uint16 prime_num;
  
  /* Prime number index corresponding to the prime number. The prime number
    index starts from 0 for prime number 7, 1 for 11 and so on. */
  uint8 prime_num_index;
  
  /* Last column used by last code block data in turbo interleaver table. */
  uint16 last_column;
  
  /* Last row used by last code block data in turbo interleaver table. */
  uint8 last_row;
  
  /* Last row bit exchange condition for the intra and inter row
    permutations. This is set to true if the code block size is equal to
    product of number of rows and column and number of column is one less
    than prime number. */
  boolean last_row_bits_exchange;
} mcalwcdma_dec_turbo_param_struct_type;

/* This structure defines rate matching parameters that are required for
   de-rate marching. */
typedef struct
{
  /* Rate matched size per radio frame */
  uint16 rate_matched_size_per_radio_frame;
  
  /* De rate matching scale factor */
  uint16 drm_scale_factor;
  
  /* Optimized eplus/a value for rate matching parameters */
  uint32 eplus_over_a;
  
  /* Optimized eminus/a value for rate matching parameters */
  uint32 eminus_over_a;
  
  /* Indicates it the delta N per TTI is ODD or not */
  boolean delta_n_tti_odd;
  
  /* rate matching type used */
  dec_rm_type_enum_type rm_type;
} dec_rm_param_struct_type;

/* This enum defines the TrCh types that the TBSet info represents */
typedef enum
{
  DEC_TBSET_TRCH_TYPE_R99,
  DEC_TBSET_TRCH_TYPE_HS,
  DEC_TBSET_TRCH_TYPE_EHS
} mcalwcdma_dec_tbset_trch_type;

/** This structure defines the decoder output buffer status FIFO entry structure.
 * It has all the information that mDSP status FIFO has. In addition to that it
 * has got information about the transport block that is collected from the TF
 * information using the mDSP TF Id */
typedef struct
{
  q_link_type link; /**< Link to manage this item on the queue */
  uint16 index;      /**< index of this element in array */
  
  boolean valid;               /**< information is valid or not              */
  uint16 trch_start_addr;      /**< Transport channel start address in DOB   */
  boolean hs_trch_type; /**< Indicate if TBset is of HS type or not */

  /** This is HS or non -HS info based on hs_trch_type */
  union
  {
    /** non-HS (R99) TBset information */
    struct
    {
      boolean tfci_decode_pass; /**< TFCI decode pass indicator */
      boolean zsb;              /**< zero state bit */
      uint8 mdsp_cctrch_id;     /**< mDSP CCTrCh Id (mDSP CCTrCh table index) */
      uint8 mdsp_trch_id;       /**< mDSP TrCh Id (mDSP TrCh table index) */
      uint8 mdsp_tf_id;         /**< mDSP TF Id (mDSP TF table index) */
      uint8 cfn_value;          /**< CFN value reported in status FIFO */
      uint8  num_tb;            /**< Number of transport block */ 
      uint16 tb_size_bits;      /**< Transport block size in number of bits */ 
      /** Word 32 align size of TrBlk. Hardware stores the data for TrBlks
       * start at uint32 align addr */
      uint16 tb_32bit_align_size_word32;
      /** Indicate don't check CRC pass/fail and get data for block of this TrCh */
      boolean override_crc;
      mcalwcdma_dec_dob_status_type dob_status; /**< DOB status information */
     #ifdef FEATURE_UMTS_BMC
      uint16 ctch_sfn;             /* SFN pointing to start of CTCH block TTI  */
     #endif
      WfwDemodDobTypeEnum dob_type;
      boolean dcch_early_detect;    /* Indicates whether DCCH was detected by FW */
      boolean decode_result;    /**< Indicates if the decode result is good */
      boolean skipped_decode;      /**< This decode will not affect PC and BLER updates */
#if defined(FEATURE_WCDMA_TABASCO_MODEM) && !defined(FEATURE_DUAL_WCDMA)
      boolean non_first_traceback; /**< Indicate subsequent traceback needed for CRC pass */
#else
      uint8 lva_traceback_count; /**< Indicate number of tracebacks needed for CRC pass */
#endif
      boolean olpc_updated;          /** OLPC has already been updated for this decode */
    } non_hs;

    /** HS TBset information */
    struct
    {
      uint8 hs_table_id;        /**< HSDPA table index Id */
      uint8 harq_id;            /**< HARQ process Id */
      boolean new_data;         /**< indicate new data bit state */
      boolean crc_pass;         /**< indicate PDSCH blocks CRC pass/fail status */
      uint16 global_sub_fn;     /**< HSDPA global sub frame number */
      uint16 hs_tb_size;        /**< HS transport block size */
      uint16 hs_hdr_size;       /**< MAC-hs header size */
      uint8  num_total_tb;      /**< Total block of all sizes */
      uint16 mac_d_pdu_sz[DEC_HS_MACD_PDU_SZ_PER_Q_MAX_COUNT]; /**< PDU size info */
      uint8 dob_fifo_idx;
     #ifdef FEATURE_WCDMA_HS_FACH
      uint8 hrnti_type;   /**< indicate type of H-RNTI or blind decoding */
     #endif
    } hs;

  } tbset_info;

  /** This points to the array of CRC data bits */
  uint32 *crc_data_bits_ptr;
} dec_dob_status_fifo_struct_type;

/** @name Macros for DOB status FIFO member access
 *  Now HS and non HS type depending on HSDPA feature defined.
 *  Following macros help them in easy access */
/**@{*/

#ifdef __GNUC__

 #define DEC_DOB_STATUS_FIFO(dob_status_fifo,member) \
  dob_status_fifo.tbset_info.non_hs.member
 #define DEC_DOB_STATUS_FIFO_PTR(dob_status_fifo,member) \
  dob_status_fifo->tbset_info.non_hs.member

#else /* __GNUC__ */

 #define DEC_DOB_STATUS_FIFO(dob_status_fifo,member) \
  dob_status_fifo.tbset_info.non_hs.##member
 #define DEC_DOB_STATUS_FIFO_PTR(dob_status_fifo,member) \
  dob_status_fifo->tbset_info.non_hs.##member

#endif /* __GNUC__ */

/**@}*/

/* The data structure for storing decoded TFCI info as mirrored from mDSP */
typedef struct
{
  /* Decoded CFN */
  uint16 decoded_cfn;

  /* Max energies 1 and 2 */
  uint16 max_energy1;
  uint16 max_energy2;

  /* The decoded TFCI */
  uint16 decoded_tfci;
}mcalwcdma_dec_decoded_tfci_info_struct_type;

/* L1 internal CCTrCh setup flags */
typedef struct
{
  boolean ctch_present; /* CTCH present on this CCTrCh*/

} l1_internal_cctrch_setup_struct_type;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

typedef struct
{
  /* CCTrCh table information structures */
  dec_cctrch_table_info_struct_type mcalwcdma_dec_cctrch_info[MCALWCDMA_DEC_TOTAL_CCTRCH_TABLES];
}mcalwcdma_dec_ext_cntrl_params_mdmp_type;

typedef struct
{
  /* Indicate if decoder related HSDPA information is valid or not */
  boolean mcalwcdma_dec_hs_info_valid[MCALWCDMA_DEC_HS_INFO_COUNT];

  /* This array stores the code block parameters of all TF of all TrCh in the
   CCTrCh under setup*/
  mcalwcdma_dec_code_blk_param_struct_type *mcalwcdma_dec_code_blk_param[MCALWCDMA_DEC_TOTAL_TRCHT_PER_CCTRCH];

  /* TF pool related variables */
  mcalwcdma_dec_tf_pool_info_struct_type mcalwcdma_dec_tf_pool[MCALWCDMA_MAX_NUM_TF_POOL];

  /* Decoder related HSDPA information */
  mcalwcdma_dec_hs_info_struct_type mcalwcdma_dec_hs_info[MCALWCDMA_DEC_HS_INFO_COUNT];

  /* Structure to hold minidump variables. */
  mcalwcdma_dec_ext_cntrl_params_mdmp_type mdmp;
}mcalwcdma_dec_ext_cntrl_params_type;

typedef struct
{
  /* Decode-done ISR profiling */
  uint8 decode_done_isr_start_bpg;
}mcalwcdma_decdata_ext_cntrl_params_mdmp_type;

typedef struct
{
  /* Debug var for DL datapath F3s
   Bit 0 => Decode Done ISR
   Bit 1 => Header Read ISR
   Bit 2 => A2 programming - Current TTI path
   Bit 3 => A2 programming - P to C path
   Bit 4 => HS PDUs before deciphering
   Bit 5 => R99 PDUs before and after deciphering and deciph params
   Bit 6 => A2 Done ISR - data read from DSM and DecoB
   Bit 7 => Crash upon DSM exhaustion
   Bit 8 => DSM Allocation
   Bit 9 => General Debug related
   Bit 10 => Logging related
   Bit 11 => QICE PN drift related
 */
  uint16 datapath_f3_debug[WCDMA_NUM_SUBS];

  /* Critical section mutex for Dec HS HARQ statistics */
  rex_crit_sect_type dec_hs_harq_stats_crit_sect;

  /* Start and end DOB indices for current TTIs header read */
  uint16 mcalwcdma_dec_hs_start_process_fifo_idx;
  uint16 mcalwcdma_dec_hs_end_process_fifo_idx;
  
  /* Structure to hold minidump variables. */
  mcalwcdma_decdata_ext_cntrl_params_mdmp_type mdmp;
}mcalwcdma_decdata_ext_cntrl_params_type;

extern mcalwcdma_decdata_ext_cntrl_params_type mcalwcdma_decdata_ext_cntrl_params;
extern mcalwcdma_dec_ext_cntrl_params_type mcalwcdma_dec_ext_cntrl_params;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* This is the CFN adjustment from the CFN of the last frame
   of the TTI to reach at first CFN of the TTI */
extern const uint8 mcalwcdma_dec_cfn_adjust_for_tti[L1_NUM_TTI];

/* CRC length value corresponding to CRC enum type */
extern const uint8 mcalwcdma_dec_crc_length_val_table[L1_NUM_CRC_LENGTH];

/* TTI value corresponding to TTI enum type */
extern const uint16 mcalwcdma_dec_tti_val_table[L1_NUM_TTI];

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*============================================================================*/
/**
 * This function initializes the variables related to decoder module
 */
/*============================================================================*/

extern void mcalwcdma_dec_init( wsub_id_e_type wsub_id);
extern void mcalwcdma_dec_init_per_sub( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_decdata_init

DESCRIPTION
  This function initializes the variables related to decoder module

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_decdata_init_stack( wsub_id_e_type wsub_id);
extern void mcalwcdma_decdata_init_connected( wsub_id_e_type wsub_id);
extern void mcalwcdma_decdata_deinit_connected( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION mcalwcdma_dec_get_turbo_coding_configured

DESCRIPTION
   This function returns TRUE if only convolutional coding is used on all the
   TrCh across all active CCTrCh. 

DEPENDENCIES
   None.

RETURN VALUE
   TRUE is only convolutional coding used for all R99 TrCh
   FALSE otherwise

SIDE EFFECTS
   None.

===========================================================================*/

extern boolean mcalwcdma_dec_get_turbo_coding_configured (void);

/*===========================================================================

FUNCTION mcalwcdma_dec_get_conv_coding_configured

DESCRIPTION
   This function returns TRUE if convolutional coding is used on atleast one
   TrCh across all active CCTrCh.

DEPENDENCIES
   None.

RETURN VALUE
   TRUE is convolutional coding used for at least one R99 TrCh
   FALSE otherwise

SIDE EFFECTS
   None.

===========================================================================*/

extern boolean mcalwcdma_dec_get_conv_coding_configured (void);

/*===========================================================================

FUNCTION mcalwcdma_dec_get_turbo_coding_enabled

DESCRIPTION
   This function returns TRUE if there is at least one TURBO coding TrCh
   in enable state.

DEPENDENCIES
   None.

RETURN VALUE
   TRUE if at least one turbo coding TrCh is configured and in enable state
   FALSE otherwise

SIDE EFFECTS
   None.

===========================================================================*/

extern boolean mcalwcdma_dec_get_turbo_coding_enabled ( wsub_id_e_type wsub_id);



/*============================================================================*/
/**
 * This function validate CCTrCh setup parameters for their valiidity. It do
 * all range checks other things that bound mDSP and ARM interface.
 *
 * @return TRUE or FALSE based on validation success.
 * indicator
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_validate_cctrch_setup(
  /** CCTrCh info structure pointer */
  mcalwcdma_dec_cctrch_info_struct_type *cctrch_info,
  /** TFC information for this CCTrCh */
  mcalwcdma_dec_ctfc_info_struct_type *ctfc_info, wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function setup CCTrCh table in mDSP. It computes rate maching parameters,
 * coding parameters, selects CCTrCh table index in mDSP to be setup with these
 * parameters and uploads the contents to mDSP.
 * If there is associated TFC table with CcTrCh. It finds free TFC table index
 * and upload TFC information in that table.
 *
 * @return MCAL WCDMA decoder CCTrCh Id for the CcTrCh setup. If there is failure
 * to setup CCTrCh, it returns MCALWCDMA_DEC_INVALID_CCTRCH_ID_VAL as error
 * indicator
 *
 * @see MCALWCDMA_DEC_INVALID_CCTRCH_ID_VAL
 */
/*============================================================================*/

extern void mcalwcdma_dec_setup_pccpch_table_mdsp(
  /** MCAL WCDMA decoder CCTrCh Id to be setup for PCCPCH */
  mcalwcdma_dec_cctrch_id cctrch_idx,
  /* Even, odd or both CFN to be enabled/disabled */
  dl_cctrch_cfn_type cfn_type,
  /* Enable/disable state */
  boolean state, wsub_id_e_type wsub_id);

extern mcalwcdma_dec_cctrch_id mcalwcdma_dec_cctrch_setup(
  /** number of bits that can be in the physical channel(s) that are
   * associated with this CCTrCh */
  uint32 physical_channel_bits,
  /** CCTrCh info structure pointer */
  mcalwcdma_dec_cctrch_info_struct_type *cctrch_info,
  /** TFC information for this CCTrCh */
  mcalwcdma_dec_ctfc_info_struct_type *ctfc_info,
  /** L1 internal setup info  */
  l1_internal_cctrch_setup_struct_type internal_info,
  /** Subscription ID */
  wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function frees CCTrCh table Id and mark correspondig CCTrCh table index
 * and DRM buffer area available to use.
 */
/*============================================================================*/

extern void mcalwcdma_dec_cctrch_free(
  /** MCAL WCDMA decoder CCTrCh Id to be freed */
  mcalwcdma_dec_cctrch_id cctrch_id, wsub_id_e_type wsub_id);

extern boolean mcalwcdma_dec_enable_disable_trch(
  /* CCTrCh table index */
  uint8 cctrch_idx,
  /* TrCh index in this CCTrCh */
  uint8 trch_idx,
  /* New TrCh state, 0: disable, 1: enable */
  boolean state, wsub_id_e_type wsub_id);


/*============================================================================*/
/**
 * This function saves HSDPA channel decode related information. Information
 * is copied into internal structures and is marked valid.
 */
/*============================================================================*/

extern void mcalwcdma_dec_hs_update_hs_info(
  /** HSDPA info Id to update information */
  uint8 dec_hs_info_id,
  /** HADPA decode related information structure pointer */
  mcalwcdma_dec_hs_info_struct_type *dec_hs_info, wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function frees HSDPA table decode info and mark correspondig is invalid
 */
/*============================================================================*/

extern void mcalwcdma_dec_hs_free_hs_info(
  /** MCAL WCDMA decoder HS info Id to be freed */
  uint8 dec_hs_info_id, wsub_id_e_type wsub_id);

/*============================================================================*/
/** @name Accessor function for DOB handles contents
 * Following function provide access to various information in DOB handle */
/**@{*/ 
/*============================================================================*/

/*============================================================================*/
/**
 * Query if DOB handle if of HS type or not
 *
 * @return TRUE or FALSE. TRUE indicate DOB handle is of HS type
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_query_handle_type_hs(
  /** DOB status handle to query for HS type */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get HS channel info Id for this DOB status handle. DOB status handle must be
 * checked for it to be HS type. No validation is done in this function.
 *
 * @return HS channel info Id for this channel that was returned at the time of
 * HS channel setup.
 */
/*============================================================================*/

extern uint8 mcalwcdma_dec_get_hs_info_id(
  /** DOB status handle for which to get HS info Id */
  mcalwcdma_dec_dob_status_handle_type handle);

extern uint16 mcalwcdma_dec_get_hs_subframe_num(
  mcalwcdma_dec_dob_status_handle_type handle);


/*============================================================================*/
/**
 * Get R99 channel info Id for this DOB status handle. DOB status handle must be
 * checked for it to be R99 channel type and not HS type. No validation is done
 * in this function.
 *
 * @return R99 channel CCTrCh info Id for this channel that was returned at the
 * time of CCTrCh setup.
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_trch_idx_exists(uint8 cctrch_idx, uint8 trch_idx, wsub_id_e_type wsub_id);

extern uint8 mcalwcdma_dec_get_cctrch_info_id(
  /** DOB status handle for which to get CCTrCh info Id */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get transport channel information index in CCTrCh. 
 *
 * @return Transport channel information index in CCTrCh. This is same index
 * for the transport channel that was in CCTrCh setup info at the time of setup.
 */
/*============================================================================*/

extern uint8 mcalwcdma_dec_get_trch_index(
  /** DOB status handle for which to transport channel index */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get transport format information index of transport channel in CCTrCh. 
 *
 * @return Transport format information index of transport channel in CCTrCh.
 * This is same index that was in CCTrCh setup info at the time of setup.
 */
/*============================================================================*/

extern uint8 mcalwcdma_dec_get_tf_index(
  /** DOB status handle for which to transport format index */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Query TFCI decode status.
 *
 * @return TRUE or FALSE. TRUE indicate that TFCI decode passed.
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_get_tfci_decode_status(
  /** DOB status handle for which to TFCI decode status is queried */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the starting CFN of decoded TTI
 */
/*============================================================================*/

extern uint32 mcalwcdma_dec_get_crc_result_bits(
  /** DOB status handle for which to get CRC pass/fail result bits */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the ZSB bit value
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_get_zsb(
  /** DOB status handle for which to get ZSB bit value */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the ZSB word value
 */
/*============================================================================*/

extern uint32 mcalwcdma_dec_get_zsb_word(
                             mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the starting CFN of decoded TTI
 */
/*============================================================================*/
extern uint8 mcalwcdma_dec_get_decode_cfn(
  /** DOB status handle for which to get decoded CFN */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the value of FW DCCH detection result **
 */
/*============================================================================*/
extern uint8 mcalwcdma_dec_get_dcch_early_detection(
  /** DOB status handle for which to get decoded CFN */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get the value of FW LVA skipped decode status **
 */
/*============================================================================*/
extern uint8 mcalwcdma_dec_get_skipped_decode_status(
  /** DOB status handle for which to get skip decode flag */
  mcalwcdma_dec_dob_status_handle_type handle);

#if defined(FEATURE_WCDMA_TABASCO_MODEM) && !defined(FEATURE_DUAL_WCDMA)
/*============================================================================*/
/**
 * Get the value of FW non first traceback status **
 */
/*============================================================================*/
extern uint8 mcalwcdma_dec_get_non_first_traceback_status(
  /** DOB status handle for which to get LVA non-first traceback flag */
  mcalwcdma_dec_dob_status_handle_type handle);
#else
/*============================================================================*/
/**
 * Get the value of FW LVA traceback count **
 */
/*============================================================================*/
extern uint8 mcalwcdma_dec_get_lva_traceback_count(
  /** DOB status handle for which to get LVA traceback count */
  mcalwcdma_dec_dob_status_handle_type handle);
#endif

/*============================================================================*/
/**
 * Set flag indicating OLPC has been updated **
 */
/*============================================================================*/
extern void mcalwcdma_dec_set_olpc_updated_status(
                                       /* DOB status handle */
                                       mcalwcdma_dec_dob_status_handle_type handle,
                                       /* OLPC updated status value */
                                       boolean status);

/*============================================================================*/
/**
 * Get flag indicating OLPC has been updated **
 */
/*============================================================================*/
extern boolean mcalwcdma_dec_get_olpc_updated_status(
                                       /* DOB status handle */
                                       mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Set the decode status value **
 */
/*============================================================================*/
extern void mcalwcdma_dec_set_decode_status(
                                            /* DOB status handle for which to TFCI decode status is queried */
                                            mcalwcdma_dec_dob_status_handle_type handle,
                                            /* Decode status value */
                                            boolean status);

/*============================================================================*/
/**
 * Set CRC override status for DOB handle. Used in case of loopback mode need
 * to pass CRC errored block
 */
/*============================================================================*/

extern void mcalwcdma_dec_set_crc_oride_status(
  /** DOB status handle for which to TFCI decode status is queried */
  mcalwcdma_dec_dob_status_handle_type handle,
  /** CRC override status value */
  boolean status);

/*============================================================================*/
/**
 * Get CRC override status for DOB handle. Used in case of loopback mode need
 * to pass CRC errored block
 *
 * @return CRC overriode status TRUE/FALSE
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_query_crc_oride_status(
  /* DOB status handle for which to TFCI decode status is queried */
  mcalwcdma_dec_dob_status_handle_type handle);

/*============================================================================*/
/**
 * Get Energy Metric value for DOB handle.
 *
 * @return Energy Metric value
 */
/*============================================================================*/

extern uint16 mcalwcdma_dec_query_energy_metric(
  /* DOB status handle for which Energy Metric is queried */
  mcalwcdma_dec_dob_status_handle_type handle);

/*===========================================================================
FUNCTION mcalwcdma_dec_overwrite_tfci_fail

DESCRIPTION
  This function is called when UE is configured in loopback mode and encounters
  a TFCI decode error. In this case, the TFCI decode error is overwritten
  and treated as CRC failure with TF as the largest TF. This will help in
  looping back junk data to L2. 
  If TFCI decode error was treated as such, no data is passed to L2.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  DOB status FIFO values changed
===========================================================================*/

extern void mcalwcdma_dec_overwrite_tfci_fail(
  /* DOB handle */
  mcalwcdma_dec_dob_status_handle_type handle,
  /* TF index to be set for this TBset */
  uint8 tf_index);

/*============================================================================*/
/**
 * This function returns pointer to DOB status information structure for the
 * DOB status handle passed in function argument. Since there is no DOB status
 * for HS channel, it will return NULL for that. A pointer returned remains
 * valid untill DOB status handle is freed. There is no check done for validity
 * of DOB status handle while returning information pointer
 *
 * @return DOB status information pointer in case of R9 channel. NULL otherwise
 */
/*============================================================================*/

extern mcalwcdma_dec_dob_status_type* mcalwcdma_dec_get_dob_status(
  /** Status FIFO index for which the DOB status is to be read */
  mcalwcdma_dec_dob_status_handle_type handle);

/**@}*/

/*============================================================================*/
/**
 * This function frees DOB handle in MCAL decoder database. After that access to
 * DOB handle information accessor function should not be done.
 */
/*============================================================================*/
 
extern void mcalwcdma_dec_free_dob_handle(
  /** DOB hadle to be freed */
  mcalwcdma_dec_dob_status_handle_type handle, wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function reads transport block header information. This function is
 * called in the interrupt context by the transport block header read heart
 * beat handler. This function gets the transport header information for all
 * transport blocks that have passed CRC or are in loopback mode that needs
 * CRC errored blocks to be passed up. Rest of the blocks are discarded.
 * This function is passed the pointer to the array of the DOB status handles
 * and number if handles in that array to be processed.
 *
 * Out of the function is to update TBset header info structure. Its pointer is
 * one of the argument. This structure is for higher layers that WHL1.
 */
/*============================================================================*/

extern void mcalwcdma_dec_get_decode_trblk_header(
  /** Number of DOB handles for which header need to be read (input) */
  uint8 num_dob_handles,
  /** DOB handle array pointer for which to read the data (input) */
  mcalwcdma_dec_dob_status_handle_type *dob_handles_in,
  /** DOB handle array pointer corresponding to TBset header structure elements (output)
   * This aray pointer must have at least num_dob_handles elements */
  mcalwcdma_dec_dob_status_handle_type *dob_handles_out,
  /** pointer to the TB header decode info strcuture (output)
   * This aray pointer must have at least num_dob_handles elements */
  mcalwcdma_dec_tb_hdr_decode_struct_type *tb_hdr_decode_info, wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function reads data from DOB for all blocks in current TBset header
 * information. It implements a priority based tasks for data transfer along
 * with deciphering. Order or priority is as following
 * 
 * - Trigger deciphering if deciphering is not in progress and deciphering
 *   task are already prepared and ready
 * - Poll for deciphering done status if it is active
 * - Prepare and keep read next non TM mode ciphered block deciphering tasks
 *   if they are not yet ready and ciphered blocks are pending
 * - Transfer data in background when following conditions are met.
 *   - If ciphering is in progress it will not transfer data more than what
 *     is being deciphered.
 *   - If it hit block that is ciphered but not yet deciphered, data transfer
 *     aborts
 * 
 * After non TM or unciphered data has been transferred then TM mode ciphered
 * data is deciphered and transferred from DOB block by block.
 */
/*============================================================================*/

extern void mcalwcdma_dec_get_trblk_data(
  /** pointer to the TB header decode info strcuture */
  mcalwcdma_dec_tb_hdr_decode_struct_type *tb_hdr_decode_info,
  /** Ptr to array of the DOB handles for these TB sets Hdr info */
  mcalwcdma_dec_dob_status_handle_type *dob_handles,
  /* Ptr to structure in which to store data */
  mcalwcdma_dec_tb_data_struct_type *dl_tb_data_info, wsub_id_e_type wsub_id);
  

/*============================================================================*/
/**
 * This function updates the ciphering key for downlink deciphering operation
 * to the ciphering key index locations requested.
 */
/*============================================================================*/

extern void mcalwcdma_dec_set_ciphering_key(
  /** ciphering key index (one of the 6 keys) to which the key is set */
  uint8 index,
  /** ciphering key info. Pointer to the array of 16 bytes. May not be W32
   * aligned */
  uint8 *ciphering_key_data, wsub_id_e_type wsub_id);

/*============================================================================*/
/** @name External APIs for MCAL WCDMA decoder
 * Following APIs are expected to be imlpemented by user of MCAL WCDMA decoder
 * module and are called by MCAL WCDMA encoder at various instances as described
 * in each function prototype below */
/**@{*/
/*============================================================================*/

/*============================================================================*/
/**
 * This function in called in response to a TBset decoded. For every TBset
 * decoded, MCAL WCDMA decoder reads mDSP DOB status FIFO and set information
 * contents for this handle and calls this function.
 *
 * @return TRUE or FALSE. WHL1 may not be able to accept this handle and return
 * boolean. If it is FALSE, handle is immediately freed by MCAL WCDMA decoder.
 */
/*============================================================================*/

extern boolean mcalwcdma_dec_tbset_decode_done_cb(
  /* DOB status handle for this TB decode done */
  mcalwcdma_dec_dob_status_handle_type handle, wsub_id_e_type wsub_id);


/*============================================================================*/
/**
 * This function reads the BCH data out of the DOB for the handle
 * passed in by WHL1. 

 * @return Raw BCH data
 */
/*============================================================================*/

extern uint32 mcalwcdma_dec_read_bch_sfn( 
   mcalwcdma_dec_dob_status_handle_type handle);

#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))

/*============================================================================*/
/**
 * This function returns one free TSN entry from Free Queue.
 */
/*============================================================================*/
extern mcalwcdma_dec_tsn_info_type *dec_get_tsn_buffer(void);  
#endif

/**@}*/
/*============================================================================*/
/**
 * This function dumps HARQ statistics as observed from DOB status FIFO. This 
 * function is called every time the log packet accumulating HS DL data is 
 * flushed out. 
 */
/*============================================================================*/

extern void mcalwcdma_dec_hs_dump_stats_from_dob_status_fifo( wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function resets HARQ statistics as observed from DOB status FIFO. Once
 * the DOB data statistics has been dumped, it resets the counters so that the 
 * next set of statistics can be accumulated.

 * @sideeffect All counters are reset to 0.
 */
/*============================================================================*/

extern void mcalwcdma_dec_hs_reset_stats(void);

/*============================================================================*/
/**
 * This function accumulates MAC-hs header log samples and submits to DIAG once
 * the packet is full. IT then creates a new packet for next accumulation.
 */
/*============================================================================*/

extern void mcalwcdma_dec_hs_update_mac_hs_header_logging(
  /* DOB status handle */
  mcalwcdma_dec_dob_status_handle_type handle,
  /* buffer for MAC-HS header data */
  uint32 *mac_hs_header_buf,
  uint16 offset, wsub_id_e_type wsub_id);

#ifdef FEATURE_HSDPA_MAC_EHS
/*===========================================================================
FUNCTION mcalwcdma_dec_hs_update_mac_ehs_header_logging

DESCRIPTION
  This function accumulates MAC-ehs header log samples and submits to DIAG once
  the packet is full. It then creates a new packet for next accumulation.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_dec_hs_update_mac_ehs_header_logging(
  /* DOB status handle */
  mcalwcdma_dec_dob_status_handle_type handle,
  /* TB set header information structure pointer */
  mcalwcdma_dec_tb_set_hdr_struct_type *tbset_hdr_info,
  uint32 num_reorder_sdu_in_reord_pdu[],
  uint8 si_info[],
  uint16 tsn_info[],
  uint8 q_info[]
  , wsub_id_e_type wsub_id);
#endif /* FEATURE_HSDPA_MAC_EHS */


/*============================================================================*/
/**
 * This function is interrupt service routine for TBset decode done function.
 * It gets the head of the status FIFO in mDSP and get the status FIFO info
 * from that index. It repeat till it has got all the status FIFO. It may be
 * noted that one interrupt corresponds to one status FIFO update. However
 * there might be some more TBset decoded while the current index is being
 * read from status FIFO. This function tries to get the maximum possible
 * status FIFO entries. It then calls the WHL1 decoder callback function and
 * passes a DOB handle for WHL1 to operate on.
 */
/*============================================================================*/

extern void mcalwcdma_dec_decode_done_isr_handler_cb( wsub_id_e_type wsub_id);

/*============================================================================*/
/**
 * This function posts a request to submit any partial accumulated
 * MCAL decoder log packets.
 */
/*============================================================================*/

extern void mcalwcdma_dec_submit_pending_logpkts( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  mcalwcdma_dec_update_tfci_stat_logpkt

DESCRIPTION
  This funtion accumulates TFCI statistics log packet samples.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_dec_update_tfci_stat_logpkt( wsub_id_e_type wsub_id);

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION  mcalwcdma_dec_process_cctrch_log_on_demand

DESCRIPTION
  This function collects all the items required for DL CCTrCH
  logging and submits the log packet to QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mcalwcdma_dec_process_cctrch_log_on_demand( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION  mcalwcdma_dec_process_tfc_log_on_demand

DESCRIPTION
  This funtion submits TFC log information to QXDM

DEPENDENCIES
  CCTrCH database

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mcalwcdma_dec_process_tfc_log_on_demand( wsub_id_e_type wsub_id);
#endif


extern boolean mcalwcdma_dec_get_cctrch_enable_state(
  /* CCTrCh index for which enable/disable state is queried */
  uint8 cctrch_index, wsub_id_e_type wsub_id);

extern void mcalwcdma_dec_free_drm_buffer(
  /* CCTrCh index for which DRM buffer has to be freed */
  uint8 cctrch_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_update_loopback_info

DESCRIPTION
  This function updates the loopback mode of a Trch in MCAL.
  
DEPENDENCIES
  None

RETURN VALUE
  None   

SIDE EFFECTS
  None
===========================================================================*/
extern void mcalwcdma_dec_update_loopback_info(
  /* CCTrCh index */
  uint8 cctrch_index, 
  /* TrCh index */
  uint8 trch_index,
  /* loopback mode */
  mcalwcdma_dec_loop_back_mode_type_enum_type loopback_mode, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_write_trigger_dm_task

DESCRIPTION
  This function writes the parameters required for Trigger DM task to the 
  UMTS DL HW Task Q.

DEPENDENCIES
  None

RETURN VALUE
  Updated write pointer if SUCCESS
  0xFFFFFFFF if FAILURE

SIDE EFFECTS
  None
==========================================================================*/

extern void mcalwcdma_dec_write_trigger_dm_task(void);

/*===========================================================================
FUNCTION mcalwcdma_dec_free_status_fifo

DESCRIPTION
  This function frees the DOB status FIFO in MDSP.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_dec_free_status_fifo(
                                   /* status FIFO index in mDSP to be freed */
                                   uint16 fifo_index, wsub_id_e_type wsub_id);

extern uint32 mcalwcdma_dec_get_hs_dob_fifo_idx(mcalwcdma_dec_dob_status_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_get_trch_enable_state

DESCRIPTION
  This function returns the TrCh enable state. It need the CCTrCh table index
  and TrCh table index.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE based on trch state

SIDE EFFECTS
  
===========================================================================*/

extern boolean dl_get_trch_enable_state(
  /* CCTrCh table index for which the enable state is to be set */
  uint8 cctrch_index,
  /* TrCh index */
  uint8 trch_index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_do_hs_data_transfer

DESCRIPTION
  This function programs Data Mover and P2 hardware to perform data read
  activity for HS blocks residing in DOB and also to perform partial
  pdu concatenation.

  - Checks to see if previous data transfer has completed. If it
    is still pending, then do not program a new transfer and wait
    until the previous one completes.
  - Checks to see if P2 UMTS TaskQ h/w read and write pointers
    are in sync, if not, err_fatal.
  - Calls mcalwcdma_dec_process_current_tti_hs_blocks routine
  - If there is a need to do partial pdu concatenation, calls
    mcalwcdma_dec_process_prev_tti_hs_blocks routine
  - If there is a need for any data transfer, calls
    mcalwcdma_dec_trigger_p2_processing and 
    mcalwcdma_dec_dsm_maintenance routines
  - If there is no data transfer activity needed, it immediately
    updates hsDobStatFifoReadIndex in the FW-SW interface.
  
ASSUMPTIONS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_dec_do_hs_data_transfer(
                                      /* pointer to the TB header decode info strcuture */
                                      mcalwcdma_dec_tb_hdr_decode_struct_type *tb_hdr_decode_info,
                                      /* Ptr to array of the DOB handles for these TB sets Hdr info */
                                      mcalwcdma_dec_dob_status_handle_type *dob_handles, wsub_id_e_type wsub_id);

extern uint16 mcalwcdma_dec_get_hs_harq_id(
                                   mcalwcdma_dec_dob_status_handle_type handle);

extern void mcalwcdma_dec_submit_tbset_stat_logpkt_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_a2_umts_dl_init

DESCRIPTION
  This function initializes the A2 DL PHY module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void mcalwcdma_dec_a2_umts_dl_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_get_a2_dl_phy_status

DESCRIPTION
  This function returns the status of A2 DL PHY module whether it is
  active or not.

DEPENDENCIES
  None

RETURN VALUE
  boolean - A2 DL PHY status

SIDE EFFECTS
  None
===========================================================================*/
extern boolean mcalwcdma_dec_get_a2_dl_phy_status(void);


/*===========================================================================
FUNCTION mcalwcdma_dec_get_num_coded_bits_per_tti

DESCRIPTION
  This function returns the total number of coded bits per TTI for a 
  set of (CCTrCh, TrCh, TF)

DEPENDENCIES
  None

RETURN VALUE
  Total number of coded bits per TTI

SIDE EFFECTS
  None
===========================================================================*/

extern uint32 mcalwcdma_dec_get_num_coded_bits_per_tti(
  /* CCTrCh index */
  uint8 cctrch_idx,
  /* TrCh index */
  uint8 trch_idx,
  /* TF index */
  uint8 tf_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wcdma_datapath_set_datapath_f3_dbg_var_through_qxdm

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
ex. commands
send_data 75 4 49 0 1 <val>   - for datapath_f3_debug

<val> - 32 bit value of debug variable
for providing hex value append val with 0x

===========================================================================*/

extern void wcdma_datapath_set_datapath_f3_dbg_var_through_qxdm
(
  uint8 debug_var_type,
  uint16 debug_var_val
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_cctrch_dirty_bit_update_pre_cb

DESCRIPTION
  This function updates the Demback TrCh table (FW-SW intf) based on the
  TrCh state information available in SW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void mcalwcdma_dec_cctrch_dirty_bit_update_pre_cb ( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION mcalwcdma_dec_do_data_transfer

DESCRIPTION
  This function does the data transfer of all blocks in header indication and
  read the data from DOB for them.
  
  It supports 2 modes of data transfer.
  1. Background data transfer: For this block must be unciphered or if ciphered
     deciphering must be done already. TM modeciphered blocks are not handled
     in this mode
  2. TM mode ciphered blocks are handled in tis mode. Blocks are deciphered
     immediately block by block before reading data out of DOB.

DEPENDENCIES
  Use following variable set outside the function
  dec_num_tbset_done_for_bg_data_xfer: Starts checking data transfer from this
    value.
  dec_num_tb_done_for_bg_data_xfer: Start checking for data transfer for this
    block in TBset
  dec_num_bits_being_deciphered: If deciphering is not in progress then
    it transfers atleast  this much amount before giving up control for
    deciphering done status check.

RETURN VALUE
  None

SIDE EFFECTS
  Set flag dec_data_xfer_waiting_for_decipher_done if encouters block with
  ciphering but not yet deciphered. Background data transfer stops with this
  condition
  Updates variables dec_num_tbset_done_for_bg_data_xfer and
  dec_num_tb_done_for_bg_data_xfer
==========================================================================*/

extern LOCAL void mcalwcdma_dec_do_data_transfer(
  /* pointer to the TB header decode info strcuture */
  mcalwcdma_dec_tb_hdr_decode_struct_type *tb_hdr_decode_info,
  /* Ptr to array of the DOB handles for these TB sets Hdr info */
  mcalwcdma_dec_dob_status_handle_type *dob_handles,
  /* Data transfer mode type */
  dec_data_xfer_mode_eum_type data_xfer_mode,
  /* Ptr to structure in which to store data */
  mcalwcdma_dec_tb_data_struct_type *dl_tb_data_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dec_hs_init_hs_block_bg_xfer

DESCRIPTION
  This function initialize the output used for extracting DL HS MAC-d SDU
  from HS-DSCH block
  The parameter that are inited are
  - Current Size indicator information index (order is as they are in MAC-hs
    header) that is being transferred from DOB. Value set to 0
  - Number of blocks scanned in that SID information. Value set to 0
  - Current MAC-d PDU block offset from start of HS-DSCH block. Value set to
    MAC-hs header size.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  All above mentioned variables in descriptions are initialized
===========================================================================*/

extern void dec_hs_init_hs_block_bg_xfer(
  /* DOB status FIFO information structure pointer */
  dec_dob_status_fifo_struct_type *mdsp_status_fifo);

/*===========================================================================
FUNCTION mcalwcdma_dec_proc_log_submit

DESCRIPTION
  This function is called in response to the log service request by 
  the decoder module. It checks for the state of the log packets and if
  waiting to be submitted, it calls the appropriate routines to submit.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

extern void mcalwcdma_dec_proc_log_submit( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION mcalwcdma_dec_write_nop_task_to_mem_and_align_wr_ptr

DESCRIPTION
  This function writes NOOP task words to the UMTS DL HW Task Q and aligns
  the write pointer to 32-byte address.

DEPENDENCIES
  None

RETURN VALUE
  Updated write pointer if SUCCESS
  0xFFFFFFFF if FAILURE

SIDE EFFECTS
  None
==========================================================================*/

extern void mcalwcdma_dec_write_nop_task_to_mem_and_align_wr_ptr(void);


#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================

FUNCTION wl1_decdata_debug_dump

DESCRIPTION 
    Dumps local variables into wl1_datapath_mini_debug_info_struct_type  

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
void wl1_decdata_debug_dump(wl1_qsh_datapath_info_struct_type *dump_ptr);
#endif /* FEATURE_WCDMA_QSH_DUMP*/

/*===========================================================================
FUNCTION  mcalwcdma_dec_get_trch_is_voice

DESCRIPTION
  Returns whether given TrCh ID carries voice

DEPENDENCIES
  None

RETURN VALUE
  TRUE is TrCh ID carries voice, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean mcalwcdma_dec_get_trch_is_voice(uint8 trch_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  mcalwcdma_dec_get_rm_method_for_cm

DESCRIPTION
  This function returns the rm_method in DCH when CPHy set up is not in progress.
  If CPHY is in progress, L1_DL_NO_RM is returned

DEPENDENCIES
  None

RETURN VALUE
  valid rm_method - if cphy set up is not in progress in DCH state
  L1_DL_NO_RM - otherwise

SIDE EFFECTS
  None
===========================================================================*/
mcalwcdma_dec_rm_method_enum_type mcalwcdma_dec_get_rm_method_for_cm(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  wl1_qsh_mdump_collect_mcaldecdata

DESCRIPTION
  Collect mcaldecdata variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_mcaldecdata(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_qsh_mdump_collect_mcaldec

DESCRIPTION
  Collect mcaldec variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_mcaldec(wsub_id_e_type wsub_id);

#endif
#endif /* MCALWCDMA_DEC_H */
