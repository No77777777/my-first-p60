#ifndef TDSSRCHLOG_H
#define TDSSRCHLOG_H

/*===========================================================================
                           T D S S R C H L O G . H

DESCRIPTION
This file contains or includes files that contain Layer 1 searcher log packet
structure definitions, prototypes for Layer 1 seacher logging functions, any
constant definitions that are needed, and any external variable declarations
needed for Layer 1 searcher logging.


    Copyright (c) 1999-2011 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchlog.h#1 $ 
$DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------

04/15/2011  bw    initial cut                         


===========================================================================*/

#include "tdscdma_variation.h"
#include "log.h"
#include "log_codes.h"
#include "log_codes_tdscdma.h"

/*-----------------------------------------------------------------------
     CONSTANTS                                                         
-------------------------------------------------------------------------*/
/*! \brief Max results from midamble detection */
#define TDSSRCHLOG_MAX_MIDAMBLE_DET_RESULTS 8
/*! \brief Max number of cells for intra frequency measurement */
#define TDSSRCHLOG_MAX_INTRAF_MEAS_CELLS 40
/*! \brief Max number of ISCP measurement per subframe */ 
#define TDSSRCHLOG_NUM_ANTENNAS          2
/*! \brief Max iterative cell number per TS0*/
#define TDSSRCHLOG_MAX_TOP_RSCP_CELLS      8
/*! \brief For RSCP measurement there is 32 cells for measurement exclude the above 8 iterative cells*/
#define TDSSRCHLOG_MAX_REMAINED_RSCP_CELLS  32
/*! \brief Max RSCP measurement cells per TS0*/ 
#define TDSSRCHLOG_MAX_RSCP_CELLS                40
/*! \brief Max OTD measurement cells per TS0*/ 
#define TDSSRCHLOG_MAX_OTD_CELLS                 8
/*! \brief Max number of ISCP measurement per subframe */ 
#ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
#define TDSSRCHLOG_MAX_NUM_ISCP                  160
#else
#define TDSSRCHLOG_MAX_NUM_ISCP                  5
#endif
/*! \brief Max number of usable gaps for a measurement period*/
#define TDSSRCHLOG_MAX_MEAS_GAPS 4
/*! \brief Max number of GSM Cells for a measurement GAP*/
#define TDSSRCHLOG_MAX_GSM_CELL_PER_GAP 10
/*! \brief Max number of TD Cells*/
#define TDSSRCHLOG_MAX_TD_CELLS 96
/*! \brief Maximum Number of Cells Measured */
#define TDSSRCHLOG_MAX_CELL_MEAS           32
/*! \brief Maximum Number of GSM Cells logged */
#define TDSSRCHLOG_GSM_MAX_ACCUMULATE_CELL_NUM 8

#define TDSSRCHLOG_MAX_INTERNAL_EVENT       8        

#define TDSSRCHLOG_UL_MAX_NUM_TS     6

#define TDSSRCHLOG_MAX_TRANSPORT_CHANNELS   32 

 #define TDSSRCHLOG_MAX_CCTRCH         8

#define TDSSRCHLOG_DL_MAX_NUM_TS     6

#define TDSSRCHLOG_MAX_RAT_LIST_NUM  4

#define TDSSRCHLOG_TDS_MAX_CELL_NUM  64
#define TDSSRCHLOG_GSM_MAX_CELL_NUM  32
#define TDSSRCHLOG_L1_MAX_CCTRCH 8
#define TDSSRCHLOG_LTE_MAX_CELL_NUM  32
#define TDSSRCHLOG_MAX_NUM_MEAS_LTE_FREQ 4 
#define TDSSRCHLOG_MAX_LTE_CELLS_PER_FREQ 16
#define TDSSRCHLOG_MAX_INTERF_PRIORITY_LAYERS  8 
#define TDSSRCHLOG_MAX_GSM_PRIORITY_LAYERS     32
#define TDSSRCHLOG_MAX_EUTRA_PRIORITY_LAYERS   8
#define TDSSRCHLOG_MAX_NONTS0_TS_RSP_CELLS  8

/* Maximum number of cells to rank */
//#define TDSSRCH_CELL_RESEL_RANK_LOG_NUM_CELLS_MAX 32

#define TDSSRCHLOG_MAX_OTHER_RAT  15

#define TDSSRCHLOG_MAX_SCELL_TRACKING_RECORDS  40

#define TDSSRCHLOG_MAX_SUBFRAME_COUNT_FOR_ACCUMULATED_TS0_TABLE 80
#define TDSSRCHLOG_MAX_SUBFRAME_COUNT_FOR_ACCUMULATED_nonTS0_TABLE 40
#define TDSSRCHLOG_MAX_SUBFRAME_COUNT_FOR_ACCUMULATD_RSCP_MEAS 20


#define TDSSRCHLOG_MAX_ACCUMULATE_NUM_ISCP  160

#define TDSSRCHLOG_MAX_MEAS_EVENT 8

//#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) && defined(FEATURE_TDSCDMA_DMO_SUPPORT)

/*! \brief Maximum number of detected cells per frequncy */
#define TDSSRCHLOG_LTE_CPHY_IMEAS_MAX_DETECTED_CELLS 15

/*! \brief Maximum number of cells that can be measured in a single meas req */
#define TDSSRCHLOG_LTE_CPHY_IMEAS_MAX_MEAS_CELLS 15

/*! \brief RX antennas number*/
#define TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS 2

/*! \brief For EUTRA frequency up to 8 frequencies  */
#define TDSSRCHLOG_L1_MAX_EUTRA_FREQ 8

/*! \brief Max cells supported by EUTRA frequency */
#define TDSSRCHLOG_L1_MAX_EUTRA_CELLS_PER_FREQUENCY 16

/*! \brief max subframe count to out put the accumulated eutra meas result*/
#define TDSSRCHLOG_MAX_SUBFRAME_COUNT_FOR_ACCUMULATD_EUTRA_MEAS 80

/*! \brief CELL_DCH measurement occasion info LCR  */
#define TDSSRCHLOG_MAX_OCCASION_PATTERN    5

//#endif

/* LOG_TDSCDMA_BASE_C has been defined in log_codes.h */

/*-------------------------------------------------------------------------

			  LOG PACKET: TDSCDMA_SYNCDL_MID_DETECT_LOG_PACKET
				(LOG CODE: 0xD000)			

This structure is used to log syncDl and Midamble detection information. 
 
This Log is only generated during the power up system acquisition process. 
The worst case packet size for this log is 116 bytes.
--------------------------------------------------------------------------*/

/*! \brief TDSCDMA SyncDL Midamble detection cfg 
 *  
 * 384 bits total
 */
typedef struct
{
  /*! \brief TS0 start position (cx8) 
   *
   *If gap detection is successful, set based on SRCH_GAP_DETECT response 
   */
  uint32                    ts0StartPosition : 16;
  
   /*! \brief Number of sync-dl detection freq hypotheses */                                              
  uint32                    syncDlNumFreqBins : 3;

  /*! \brief Full 5-ms window sync-dl srch flag 
   0: FALSE, configured to 0 when gap detection is sucessful
   1: TRUE, configured to 1 when gap detection is failed/bypassed
   If set to 1, ts0StartPosition ignored
  */ 
  uint32                    syncDlFullWinSrch : 1;

  /*! \brief Rx diversity flag 
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored
  */
  uint32                    rxDiv : 1;
  /*! \brief Rx antenna config
   0: Antenna 0
   1: Antenna 1
  */ 
  uint32                    rxAntenna : 1;
  uint32                    :10;	/*!< \brief reserved */
  //32 bits

  /*! \brief Midamble mask used to indicate known barred cells
   Bit n, midambleMask[m]: for midamble m*32+n 
  */ 
  uint32                    midambleMask[4];
  //128 bits

  /*! \brief SYNC-DL mask used to indicate known barred cells
  @verbatim 
   The same info as midambleMask used for sync-dl detection
   Bit n: for midamble from 4*n to 4*(n+1)-1
  @endverbatim 
  */ 
  uint32                    syncDlMask;
  //32 bits

  /*! \brief SYNC-DL detection freq vectors
   Resoltuion: 1 Hz
   Up to 7 freqs
   Recommeded systems configuration for init acq: [-6670, 0, 6670] Hz
  */ 
  int16                     syncDlfreq[3];
  /*! \brief SYNC-DL detection pruning threshold
   Resolution: Q8 (linear)
   Recommended configuration: 16 (-12dB)
  */ 
  uint16                    syncDlPruningThresh;
  //64 bits

  /*! \brief SYNC-DL and midamble detection non-coherence integration length 
   Resoltution: 1 subframe, n means non-coherence length is n
   Recommeded systems configuration for init acq: 8 for one antenna
                                                  4 for two antennas
   Recommeded systems configuration for IRAT measurement: 1/3 for one antenna
  */ 
  uint16                    nonCohLength; 

  /*! \brief Midamble detection freq bin step size
   Resolution: 1 Hz 
   Recommeded systems configuration: 1000
  */ 
  uint16                    midambleFreqStepSize;
  //32 bits

  /*! \brief Midamble detection pruning timing threshold
   Resolution: 1 chip
   Recommended configuration: 20 
  */ 
  uint16                    midamblePruningTiming;
  /*! \brief Midamble detection pruning level 1 midamble threshold
   Resolution: Q8 (linear)
   Recommended configuration: 16 (-12dB)
  */ 
  uint16                    midamblePruningMid1;
  //32 bits

  /*! \brief Midamble detection pruning level 2 midamble threshold
   Resolution: Q8 (linear)
   Recommended configuration: 32 (-9dB)
  */ 
  uint16                    midamblePruningMid2;
  /*! \brief Midamble detection pruning level 2 sync-dl threshold
   Resolution: Q8 (linear)
   Recommended configuration: 32 (-9dB)
  */ 
  uint16                    midamblePruningSync2;
  //32 bits

  /*! \brief Midamble detection combining weight factor for non-zero shifts
   Resolution: Q14
   Recommeded systems configuration: 0x2000 (0.5)
  */ 
  uint16                    midambleNonZeroShiftWt;
  /*! \brief Number of midamble detection freq hypotheses 
   Total freq bins = 2*midambleNumTwoSidedFreqBins + 1
   Recommeded systems configuration for init acq: 4
  */ 
  uint8                     midambleNumTwoSidedFreqBins;
  /*! \brief LNA state
   Set based on the value returned in SRCH_GAP_DETECT response
  */
  uint8                     lnaState;
  //32 bits
} tdssrchlog_sync_mid_detect_cfg_type;

/*! \brief TDSCDMA SyncDL Midamble detection result
 * 64 bits total
 */
typedef struct
{
  /*! \brief DL-SYNC index (0~31)*/   
  uint32                    syncdlIdx : 8;
  /*! \brief Midamble index */   
  uint32                    midambleIdx : 8;
  /*! \brief TS0 start position (cx8) */   
  uint32                    ts0StartPosition : 16;
  //32 bits
  /*! \brief Quality metrics SNR */   
  uint16                    snr;
  /*! \brief Freq offset (Hz) */ 
  int16                     freqOffset;
  //32 bits
}  tdssrchlog_detected_cell_type;


/*! \brief       LOG PACKET: TDSCDMA_SYNCDL_MID_DETECT_LOG_PACKET
				(LOG CODE: 0xD000)			
   
This log packet is for internal use only!   
   
This structure is used to log syncDl and Midamble detection information. 
 
This Log is generated during the power up system acquisition process. 

MIN Size: 52 bytes, MAX Size: 116 bytes
The worst case packet size for this log is 116 bytes.
32+384+64*8=928 bits 
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  /*! \brief number of cells detected 
   */
  uint32  numCells:8;                     
  /*! \brief center frequency */
  uint32  center_freq:16;
  //32 bits

  /*! \brief TDSCDMA SyncDL Midamble detection cfg record */
  tdssrchlog_sync_mid_detect_cfg_type syncMidDetectCfg;
  //384 bits

  //Variable length members should be put at the end of the packet
  /*! \biref detected cell record, actual number determined by numCells */
  tdssrchlog_detected_cell_type  detectedCell[TDSSRCHLOG_MAX_MIDAMBLE_DET_RESULTS];
  //64 bits * numCells

} tdssrchlog_sync_mid_detect_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_SYNC_MID_DETECT_PKT_LEN(detected_cells)  ( \
    FPOS(tdssrchlog_sync_mid_detect_pkt_type, detectedCell)+  \
    ( detected_cells * sizeof(tdssrchlog_detected_cell_type)))

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_MEASUREMENT_RSCP_LOG_PACKET
 				 (LOG CODE: 0xD001)

This log packet outputs the filtered P-CCPCH RSCP measurement for all 
intra-frequency and inter-frequency cells in the sorted order from strong 
to weak. The worst case packet size for this log is 304 bytes. 
---------------------------------------------------------------------------*/
/*! \brief TDSCDMA MEASURE RSCP CELL subrecord 
 *  
 *  32 bits total
 */
typedef struct 
{
  /*! \brief Previous timing measurement result */
  uint16                    ts0StartPosition;
  /*! \brief Cells midamble index */
  uint8                     midambleIdx;
  /*! padding */
  uint8                     reserved;
} tdssrchlog_measure_rscp_cell_type;

//#ifndef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
/*! \brief TDSCDMA Inter Freq LNA subrecord
 *  
 * 	96 bits total
 */
typedef struct {
  /*! \brief power measurement value */
  uint32                     pwrMeasureValue;
  /*! \brief old power measurement value */
  uint32                     pwrMeasureOldValue;
  /*! \brief LNA state (0=G0, 1=G1, 2=G3, 3=G4) */
  uint8                      state;
  /*! \brief LNA hold timer */
  uint8                      holdTimer;
  /*! padding */
  uint16 reserved;

} tdssrchlog_inter_freq_lna_type;
//#endif

/*! \brief TDSCDMA RSCP Measurement Result Subrecord
 *  
 *  992 bits total
 */
typedef struct {
  /*! \brief RSSI, per antenna
  */
  int32                    rssi[TDSSRCHLOG_NUM_ANTENNAS];
  //64 bits
  /*! \brief measured frequency (Hz) 
  */
  uint32                    measuredFreq;
  //32 bits
  /*! \brief timing measurement results
  */ 
  int16                     ts0StartPosition[TDSSRCHLOG_MAX_OTD_CELLS];
  //16 * 8 bits
  /*! \brief Subframe nubmer for RSCP measurement */
  uint16                    subframeNum;
  uint16                    reserved;
  //32 bits
//  #ifdef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
//  tfw_rx_lna_t  lnaInfo;
//  #else
  /*! \brief LNA info for inter freq measurement 
  */
  tdssrchlog_inter_freq_lna_type      interLnaInfo;  //relocate for allignment
  //96 bits
//  #endif
  /*! \brief RSCP measurement results */ 
  int16                    rscp[TDSSRCHLOG_MAX_RSCP_CELLS];
  //16 * 40 bits
} tdssrchlog_measure_rscp_result_type;


/*! \brief		 LOG PACKET: TDSCDMA_MEASUREMENT_RSCP_LOG_PACKET
 				 (LOG CODE: 0xD001)
   
This log packet is for internal use only!   
   
This log packet outputs the filtered P-CCPCH RSCP measurement for all 
intra-frequency and inter-frequency cells in the sorted order from strong 
to weak. It is generated for every RSCP request sent to FW.

MIN Size: 144 bytes, MAX Size: 304 bytes
The worst case packet size for this log is 304 bytes. 
32+32+992+96+32*40=2432 bits 
*/
typedef struct 
{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  
  /*! \brief Number of cells for RSCP measurement */
  uint32                    numCells : 8;				//enlarge from 6 bits to 8 bits 
  /*! \brief Number of TSO JDS cells for the measured frequency */
  uint32                    numTs0JdcsCells : 4;			//enlarge from 3 bits to 4 bits
  /*! \brief Rx diversity flag 
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored
  */
  uint32                    rxDiv : 1;
  /*! \brief Rx antenna config
   0: Antenna 0
   1: Antenna 1
  */ 
  uint32                    rxAntenna : 1;
  /*! \brief Inter freq measurement flag */
  uint32                    interFreq : 1;
  /*! \brief Active cell primary freq flag */
  uint32                    activePrimaryFreq : 1; 		//relocate to meet byte boundary
  /*! \brief RF script buffer index for inter freq tuning */
  uint32                    interFreqRfBufIdx : 8;
  // 32 bits
  /*! \brief measured frequency (Hz) */
  uint32                    measuredFreq;
  // 32 bits
  /*! \brief LNA info for inter freq measurement */
//  #ifndef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  tdssrchlog_inter_freq_lna_type      interLnaInfo;
//   #else
//  tfw_rx_lna_t  lnaInfo;
  //96 bits  
//  #endif

  /*! \brief RSCP measurment result */
  tdssrchlog_measure_rscp_result_type rscpRsp;
  // 992 bits

  /*! \brief Cells config for RSCP measurement, atucal number of this field is denoted by numCells */
  tdssrchlog_measure_rscp_cell_type   cellCfg[TDSSRCHLOG_MAX_RSCP_CELLS]; //relocate to the end of the packet.
  // 32 bits * numCells
} tdssrchlog_measure_rscp_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_MEASURE_RSCP_PKT_LEN(num)  ( \
    FPOS(tdssrchlog_measure_rscp_pkt_type, cellCfg)+  \
    ( num * sizeof(tdssrchlog_measure_rscp_cell_type)))

/*--------------------------------------------------------------------------

				LOG PACKET: TDSCDMA_MEASUREMENT_ISCP_LOG_PACKET
 				 (LOG CODE: 0xD002)
                     
This packet outputs the ISCP measurement request and results. 
The worst case packet size for this log is 48 bytes. 
----------------------------------------------------------------------------*/

/*! \brief TDSCDMA MEASURE_ISCP SLOT CELL 
 *  
 * 	64 bits total
 */
typedef struct
{
  /*! \brief Slot Index	*/ 
  uint8                     slotIdx;
  /*! \brief Cell midamble index */
  uint8                     midambleIdx;
  //16 bits
  /*! \brief ISCP measurement results */
  int16                    iscp[TDSSRCHLOG_NUM_ANTENNAS];
  //32 bits
} tdssrchlog_measure_iscp_slot_cell_type;

/*! \brief		 LOG PACKET: TDSCDMA_MEASUREMENT_ISCP_LOG_PACKET
 				 (LOG CODE: 0xD002)
                     
This log packet is for internal use only!   
   
This packet outputs the ISCP measurement request and results. It is generated 
once every for every ISCP request sent to FW. 

MIN Size: 6 bytes, MAX Size: 36 bytes
32+32+64*5=384 bits (or 32+32+64*160=10304 bits ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT)
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  /*! \breif Resreved */
  uint32  reserved:8;
  /*! \brief number of ISCP measurement */
  uint32  numIscp:8;                     
  /*! \brief LSB for the subframe number */
  uint32  subframeLsb:8;
  //32 bits

  /*! \brief Subframe nubmer for ISCP measurement */
  uint16                    subframeNum;
  //32 bits
  /*! \brief ISCP slot & cell configuration and  measurement result, from 0 to numIscp, max 5 (or 160 ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT)*/
  tdssrchlog_measure_iscp_slot_cell_type slotCell[TDSSRCHLOG_MAX_NUM_ISCP];

  //64 bits * TDSSRCHLOG_MAX_NUM_ISCP

} tdssrchlog_measure_iscp_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_MEASURE_ISCP_PKT_LEN(num)  ( \
    FPOS(tdssrchlog_measure_iscp_pkt_type, slotCell)+  \
    ( num * sizeof(tdssrchlog_measure_iscp_slot_cell_type)))

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_GAP_DETECT_LOG_PACKET
 				 (LOG CODE: 0xD003)

This packet contains the step0 gap detection configuration parameters and result. 
The packet is generated when a gap detection response is received. 

-----------------------------------------------------------------------------*/

/*! \brief Gap Detect Result
   1. Only detected and ts0StartPosition in the response message are used to
      configure the next acq stage (sync-dl and midamble detection).
   2. For implementation efficiency, FW doesn't calculate ratios to find the
      power pattern. So FW returns power of high-power and low-power regions
      around the gap to SW.
   3. lnaState in the response message is used by FW for the following acq
      stages.
*/
typedef struct 
{
  /*! \brief Power of high-power region around the gap 
  */ 
  uint32                    highPwrRegionEng;
  //32 bits
  /*! \brief Power of high-power region around the gap 
  */ 
  uint32                    lowPwrRegionEng;
  //32 bits
  /*! \brief TS0 start position (cx8) 
   Invalid if detected flag is 0
  */   
  uint16                    ts0StartPosition;
  /*! \brief Gap detection flag
  @verbatim
   0: detection fails
   1: detected
  @endverbatim 
  */ 
  uint8                   detected;
  /*! \brief Gap detection LNA state
  @verbatim
   00: G0
   01: G1
   10: G2
   11: G3
   Used for the rest of acquisition stages
   Invalid if disableLNACtrl in SRCH_GAP_DETECT command is set as 1
  @endverbatim 
  */
  uint8                     lnaState;
  //32 bits
  /*! \brief Selected antenna
  @verbatim
   0: Antenna 0
   1: Antenna 1
  @endverbatim 
  */
  uint8         selectedAntenna;

  /*! \brief reserved */
  uint8 reserved[3];
  //32 bits

} tdssrchlog_gap_detect_result_t;

/*! \brief GapDetect SRCH_GAP_DETECT Configuration Command
*/
typedef struct 
{
  /*! \brief Narrow-band threshold when gap detection is successful 
  @verbatim 
   detectedNbThresh[n]: for G[n]
  @endverbatim 
  */
  uint32                    detectedNbThresh[4];
  //32 * 4 bits

  /*! \brief Wide-band threshold when gap detection is successful 
  @verbatim 
   detectedWbThresh[n]: for G[n]
   Resolution, recommended configuration: TBD 
  @endverbatim 
  */
  uint32                    detectedWbThresh[4];
  //32 * 4 bits

    /*! \brief Narrow-band threshold when gap detection fails
  @verbatim 
   failedNbThresh[n]: for G[n]
   Resolution, recommended configuration: TBD 
  @endverbatim 
  */
  uint32                    failedNbThresh[4];
  //32 * 4 bits
  /*! \brief Wide-band threshold when gap detection fails 
  @verbatim 
   failedWbThresh[n]: for G[n]
   Resolution, recommended configuration: TBD 
  @endverbatim 
  */
  uint32                    failedWbThresh[4];
  //32 * 4 bits

  /*! \brief Gap detection threshold 
  @verbatim 
   Resolution: unsigned 16Q8
   Recommended configuration: TBD 
  @endverbatim 
  */
  uint16                    gapDetectThresh;
  /*! \brief LNA states enable flag 
  @verbatim
   lnaStateEnable[n]: for G[n]
   Recommeded systems configuration: G[0]=0, G[1]=1, G[2]=1, G[3]=1
  @endverbatim 
  */ 
  uint8                     lnaStateEnable:4;
  /*! \brief Rx diversity flag 
  @verbatim
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored
  @endverbatim 
  */
  uint8                     rxDiv : 1;
  /*! \brief Rx antenna config
  @verbatim
   0: Antenna 0
   1: Antenna 1
  @endverbatim 
  */
  uint8                     rxAntenna : 1;
  /*! \brief Disable LNA ctrl (for test mode such as PBS baseband testing) 
  @verbatim
   0: LNA ctrl enable
   1: LNA ctrl disable
   If set to 1, lnaStateEnable[4] ignored
  @endverbatim 
  */ 
  uint8                   disableLNACtrl:1;
  uint8                   : 1;

  /*! \brief Number of subframes for gap detection 
  @verbatim
   Range: 0~255, placeholder for future change if needed
   Current valid configuration: 1
  @endverbatim 
  */ 
  uint8                     numSubFrame;
  //32 bits
} tdssrchlog_gap_detect_cfg_t;

/*! \brief       LOG PACKET: TDSCDMA_GAP_DETECT_LOG_PACKET
				(LOG CODE: 0xD003)			
   
This log packet is for internal use only!   
   
This structure is used to log gap detection information. 
 
This Log is generated during the power up system acquisition process. 
The purpose of Gap detection is to:
     1) Find high-low-high-low (HLHL) power pattern
     2) Determine whether gap detection is successful or failed
     3) Determine LNA state used for the rest of acquisition stages 

MIN Size: 84 bytes, MAX Size: 84 bytes
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  /*! \brief reserved */
  uint32  reserved:8;
  uint32  center_freq:16;
  //32 bits

  /*! \brief TDSCDMA Gap detection cfg record */
  tdssrchlog_gap_detect_cfg_t gapDetectCfg;

  /*! \biref detected cell record, actual number determined by numCells */
  tdssrchlog_gap_detect_result_t  gapDetectResult;

} tdssrchlog_gap_detect_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_GAP_DETECT_PKT_LEN()  ( \
    sizeof(tdssrchlog_gap_detect_pkt_type))

	

	

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_IRAT_RSCP_RSP_LOG_PACKET
 				 (LOG CODE: 0xD05F)
This packet contains iscp measurement results from FW and TL1 pruning results during the measurement stage. 
This packet is generated when a iscp measurement response is received from FW. 

-----------------------------------------------------------------------------*/


typedef struct
{
	uint16 cpid;
    uint16 mMetric;
	//32 bits
}cpid_mMetric_t;

typedef struct
{
    /*! \brief Rscp  */
    int16 rscp[128];     
    //16*128 bits
    /*! \brief tsRsp  */
    int16 tsRsp[128];
    //16*128 bits
    /*! \brief ts0StartPosition */
    uint16 ts0StartPosition;
    /*! \brief attribute 
     @verbatim
     0: TFW_RESULT_OK
     1: TFW_RESULT_NOK_TS0_CELL_CMD_NOT_RECEIVED
     2: TFW_RESULT_NOK_SERVING_CELL_CONFIG_ERROR
     3: TFW_RESULT_NOK_OTHER_REASON  (Same as TFW_RSCP_NOT_MEASURED )
     4: TFW_RESULT_NOK_CXM_CONFLICT_CHECK_LOST
     5: TFW_RESULT_NOK_REACQ_NOT_PERFORMED
     6: TFW_RESULT_NOK_REACQ_REACQ_RECEIVED_LATE
     @endverbatim  
    */
    uint16 errStatus;
    //32 bits
    /*! \brief mMetric  */
    
    cpid_mMetric_t cpid_mMetric[8];
    //32*8 bits
    /*! \brief rssi */
    int32 rssi;
    //32 bits
    /*! \brief interLnaInfo */
    tdssrchlog_inter_freq_lna_type  interLnaInfo;      
}tdssrchlog_irat_fw_rscp_rsp_t;



/*! \brief       LOG PACKET: TDSCDMA_IRAT_RSCP_RSP_LOG_PACKET
				(LOG CODE: 0xD05F)			
   
This log packet is for internal use only!   
   
This structure is used to log iscp measurement results from FW and TL1 pruning results during the measurement stage. 
 
The purpose of iscp measurement is to measure the power of adjacent cells in order that the serving cell can use the information
to make a decision on which cell to make a handover to. 
     
MIN Size: 588 bytes, MAX Size: 588 bytes

*/

typedef struct
{
/* Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
  /*! \brief total number of cells returned */
  uint8              num_of_cells; /* total number of cells returned */
  /*! \brief number of jds cells */
  uint8              num_of_jds_cells; 
  /*! \brief acquisition state 
   @verbatim
   0: TDSIRAT_ACQ_NONE
   1: TDSIRAT_ACQ_SUCCESS
   2: TDSIRAT_ACQ_FAILURE
   3: TDSIRAT_ACQ_STEP0_PASS
   4: TDSIRAT_ACQ_STEP0_FAIL
   5: TDSIRAT_MEAS_DONE
  @endverbatim   
  */
  uint8              acq_state;
  //32 bits 
  /*! \brief uarfce */
  uint32             uarfcn;
  //32 bits 
  /*! \brief cpid */
  uint16             cpid[8];
  //16*8 bits
  
  tdssrchlog_irat_fw_rscp_rsp_t fwRscpRsp;  

}tdssrchlog_irat_rscp_pkt_type;

#define TDSSRCHLOG_IRAT_RSCP_PKT_LEN()  ( \
    sizeof(tdssrchlog_irat_rscp_pkt_type))

	

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_MEASUREMENT_INTERNAL_CFG_LOG_PACKET
 				 (LOG CODE: 0xD005)

This packet contains the intra-frequency measurement configuration parameters. 
The packet is generated when the intra-frequency measurement is initiated or 
modified. 

The worst case packet size for this log is 44 bytes. 
32+32+32*8+32 = 352 bits
---------------------------------------------------------------------------*/
/*! \brief TDSCDMA INTERNAL EVENT CONFIG subrecord 
 *  
 *  32 bits total
 */

typedef struct
{
  /*! \brief  event id */
  uint8 EventId;
  /*! \brief Tx power threshold dbm units. ( -50 ... +33) valid for 6A/6B */
  int8 TxPowerThreshold;
  /*! \brief  Time difference ( 0...504). In event 6F/6G 10.3.7.80, real (0..63), step 0.125,
     RRC send L1 is (0, 504), L1 will translate to real value */
  uint8 Tadvthreshold;
  /*! \brief  Time To Trigger */
  uint8 TimeToTrigger;
  //32bit
} tdssrchlog_internal_event_config_type;

/*! \brief TDSCDMA INTERNAL PERIOD CONFIG subrecord 
 *  
 *  32 bits total
 */

typedef struct
{
  /*! \brief Amount of reporting (1,2,4,8,16,32,64, Infinity */
  uint16 ReportAmount;
  /*! \brief  Reporting Interval  (250,500,1000,2000, ... ) in ms */
  uint16 ReportInterval;
  //32bit
} tdssrchlog_internal_period_config_type;


/*! \brief	LOG PACKET: TDSCDMA_INTERNAL_MEAS_CONFIG_LOG_PACKET
 				 (LOG CODE: 0xD005)

This packet contains the intra-frequency measurement configuration parameters. 
The packet is generated when the intra-frequency measurement is initiated or 
modified. 

MIN Size: 12 bytes, MAX Size: 44 bytes
The worst case packet size for this log is 44 bytes. 
32+32+32*8+32 = 352 bits
*/

typedef struct
{

  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  /*! \brief Reserved */
  uint32  reserved:8;
  /*! \brief measurement id */
  uint32 MeasID:8;
  /*! \brief measurment command: 0 - Setup; 1-Modify; 2-Release */  
  uint32 MeasCMD:2;	
  /*! \brief  If Measure Quantity is yes, MeasQuan is Mandatory */
  uint32 MeasQuanIncl:1;
  /*! \brief Measure Quantity*/
  uint32 MeasQuan:3;
  uint32 :2;
  //32 bits
  /*! \brief if filter coff is included,if yes,FiltCoff is Mandatory*/
  uint32 FiltCoffIncl:1;
  /*! \brief filter coff*/
  uint32 FiltCoff:5;
  /*! \brief if report quantity is included,if yes,RptQuan is Mandatory*/
  uint32 RptQuanIncl:1;
  /*! \brief report quantity*/
  uint32 RptQuan:3;
  /*! \brief report trigger mode*/
  uint32 ReportTriggerMode:3;
  /*! \brief Num events,it decides the num of IntEventConfig[] */
  uint32 EventNum:7;
  uint32 :12;         
  //32 bits  
  /*! \brief  Periodical Measurement reporting criteria as in 10.3.7.53  
    Meaningful if "rpt_mode" is TDSL1_PERIODIC_RPT */
  tdssrchlog_internal_period_config_type IntPeriodConfig;
  //32bits 
  /*! \brief  Event Triggered Measurement reporting criteria as in 10.3.7.80  
    Meaningful if "rpt_mode" is TDSL1_EVENT_TRIGGER_RPT */
  tdssrchlog_internal_event_config_type IntEventConfig[TDSSRCHLOG_MAX_INTERNAL_EVENT];
  //32*8 bits  
} tdssrchlog_internal_meas_config_pkt_type;

/* Macro to determine the sizes of internal meas config log packet */
#define TDSSRCHLOG_MEASURE_INTERNAL_PKT_LEN(event_num)  ( \
    FPOS(tdssrchlog_internal_meas_config_pkt_type, IntEventConfig)+  \
    ( event_num * sizeof(tdssrchlog_internal_event_config_type)))

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_INTERNAL_RESULTS_LOG_PACKET
 				 (LOG CODE: 0xD007)

This log packet outputs the txpower and tadv results, It is generated once every 100ms.

The worst case packet size for this log is 68 bytes. 
32+32*2*6+32*4 = 544 bits
---------------------------------------------------------------------------*/

/*! \brief TDSCDMA INTERNAL TXPOWER subrecord 
 *  
 *  64 bits total
 */

typedef struct
{ 
  /*! \brief  Valid */      
  uint8   valid;

  /*! \brief  Valid [0,1]. 0: Total TX power for this slot has not hit Max */
  uint8   overMaxPwrInd;
  /*! \brief  Valid [0,1]. 0: Total TX power for this slot has not hit Min */
  uint8   underMinPwrInd;
  /* unused */
  uint8   :8;
  /*! \brief  Ajdusted max TX power w/o MPR in this slot w/ <TBD> 1/16 dB resolution, 
    ie. 4 fractional bits*/
  int16   pwrMaxAdjusted;
  /*! \brief   total TX power in this slot with <TBD> 1/16 dB resolution, i.e. 4 fractional points. */
  int16   TxPower;
  //32*2 bit
} tdssrchlog_internal_txpower_type;

/*! \brief TDSCDMA INTERNAL tADV subrecord 
 *  
 *  128 bits total
 */

typedef struct
{
  /*! \brief  Bit Mask to indicate which UL time slot will be transmitted and has
    corresponding valid timing info. Bit 0: UPPCH, Bit 1: TS1...Bit 5: TS 5 */
  uint16    txBitMask; 
  uint16:   16;
  /*! \brief  Timing relative to DL slots in 1/8 chip. Index 0 for UPPCH, Index [1..5]
    for UL Ts[1..5] */
  int16     timeVsDl[TDSSRCHLOG_UL_MAX_NUM_TS]; 
  //32*4 bit
} tdssrchlog_internal_Tadv_type;

/*! \brief	LOG PACKET: TDSCDMA_MEASUREMENT_INTERNAL_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD007)

This log packet outputs the txpower and tadv results, It is generated once every 100ms.

MIN Size: 4 bytes, MAX Size: 68 bytes
The worst case packet size for this log is 68 bytes. 
32+32*2*6+32*4 = 544 bits
*/

typedef struct
{
  /* Log header */
  log_hdr_type header;
  /*! \brief   Version  */
  uint32  version:8;
  /*! \brief  subframe number*/
  uint32 sfn:16;
  uint32 :8;
  //32 bit
  /*! \brief  txpower results*/
  tdssrchlog_internal_txpower_type TxPower[TDSSRCHLOG_UL_MAX_NUM_TS];
  //32*2*6 bit
  /*! \brief  tadv results*/
  tdssrchlog_internal_Tadv_type   Tadv;
  //32*4 bit
} tdssrchlog_internal_results_pkt_type;

/* Macro to determine the sizes of internal meas results log packet */
#define TDSSRCHLOG_INTERNAL_RESULT_PKT_LEN(num)  ( \
    FPOS(tdssrchlog_internal_results_pkt_type, TxPower)+  \
    ( num * sizeof(tdssrchlog_internal_txpower_type)))


/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_CELL_RESEL_RANK_LOG_PACKET
 				 (LOG CODE: 0xD009)

This log packet outputs the tdscdma cells and gsm cells's rscp and rank value.
It is generated once cells are evaluated

The worst case packet size for this log is 380 bytes. 
32+32*64+32*32 = 3040 bits
---------------------------------------------------------------------------*/

/*! \brief TDSCDMA CELL RANK subrecord 
 *  
 *  32 bits total
 */

typedef struct
{
  /*! \brief frequency of the tdscdma cell*/
  uint16    freq; 
  /*! \brief cell parameter id of the tdscdma cell*/
  uint16    cpi;
  /*! \brief rscp of the tdscdma cell dBm [-120,25] ,mapping to rang [-5,91]*/
  int16     rscp; 
  /*! \brief rank of the tdscdma cell (dBm)*/
  int16     rank;
  //32bits
} tdssrchlog_tds_cell_rank_info_type;

/*! \brief GSM CELL RANK subrecord 
 *  
 *  32 bits total
 */

typedef struct
{ 
  /*! \brief frequency of the gsm cell*/
  uint16    arfcn; 
  /*! \brief bsic of the gsm cell*/
  uint16    bsic;
  /*! \brief rssi of the gsm cell dBm [-110,-48] ,mapping to rang [0,63]*16*/
  int16     rssi; 
  /*! \brief rank of the gsm cell*/
  int16     rank;
  //32bits
} tdssrchlog_gsm_cell_rank_info_type;

/*! \brief    LOG PACKET: TDSCDMA_CELL_RESEL_RANK_LOG_PACKET
 				 (LOG CODE: 0xD009)

This log packet outputs the tdscdma cells and gsm cells's rscp and rank value.
It is generated once cells are evaluated

MIN Size: 4 bytes, MAX Size: 772 bytes
32+32*64+32*32 = 3040 bits
 */

typedef struct
{
  /* Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint32 version:8;
  /*! \brief number of tdscdma cell,it decides the number of TdsCellInfo[]*/
  uint32 TdsCellNum:8;
  /*! \brief number of gsm cell,it decides the number of GsmCellInfo[]*/
  uint32 GsmCellNum:8;
  uint32: 8;
  //32 bits
  /*! \brief TDS cell rank info */
  tdssrchlog_tds_cell_rank_info_type TdsCellInfo[TDSSRCHLOG_TDS_MAX_CELL_NUM];
  //32*64 bits
  /*! \brief GSM cell rank info */
  tdssrchlog_gsm_cell_rank_info_type GsmCellInfo[TDSSRCHLOG_GSM_MAX_CELL_NUM];
  //32*32 bits
} tdssrchlog_cell_resel_rank_pkt_type;

/* Macro to determine the sizes of cell resel rank log packet */
#define TDSSRCHLOG_CELL_RESEL_RANK_PKT_LEN(tdscellnum,gsmcellnum)  ( \
    FPOS(tdssrchlog_cell_resel_rank_pkt_type, TdsCellInfo)+  \
    ( tdscellnum * sizeof(tdssrchlog_tds_cell_rank_info_type))+  \
    ( gsmcellnum * sizeof(tdssrchlog_gsm_cell_rank_info_type)))

/*--------------------------------------------------------------------------

			 LOG PACKET: TDSCDMA_QUALITY_MEAS_CONFIG_LOG_PACKET
 				 (LOG CODE: 0xD006)

This log packet outputs the quality measurement configuration parameters.
It is generated once it is configured

 MIN Size: 56 bytes, MAX Size: 308 bytes
 32+32+32+32*8+8*8+64*32 = 2464 bits
---------------------------------------------------------------------------*/

/*! \brief QUALITY EVENT CONFIG subrecord 
 *  
 *  64 bits total
 */

typedef struct
{ /*! \brief transport channel id*/
  uint16 TrchId;
  /*! \brief Total No of CRC's before 5A could be generated 1..512*/
  uint16 TotalCRC;
  /*! \brief  BAD CRC's during the window, 1..512 r*/
  uint16 BadCRC;
  /*! \brief Pending time after trigger CRC's, 1..512*/
  uint16 PendAfterTrigger;
  //64bits
} tdssrchlog_quality_event_config_type;

/*! \brief QUALITY PERIOD CONFIG subrecord 
 *  
 *  64 bits total
 */

typedef struct
{
  /*! \brief  Amount of reporting (1,2,4,8,16,32,64, Infinity )*/
  uint16 ReportAmount;
  /*! \brief Reporting Interval  (250,500,1000,2000, ... ) in ms */
  uint16 ReportInterval;
  //32 BITS
} tdssrchlog_quality_period_config_type;

/*! \brief  LOG PACKET: TDSCDMA_MEASUREMENT_QUALITY_CFG_LOG_PACKET
 				 (LOG CODE: 0xD006)

This log packet outputs the quality measurement configuration parameters.
It is generated once it is configured

 MIN Size: 56 bytes, MAX Size: 308 bytes
 32+32+32+32*8+8*8+64*32 = 2464 bits
*/
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
    /*! Reserved */
  uint32 reserved:24;
  //32bits	
  /*! \brief measurement id*/
  uint32 MeasID:5;
  uint32 MeasCMD:2;	
  /*! \brief  Meas Quantity is always BLER. No need to indicate explicitly 
     ** Reporting Quantity to be included. */
  uint32 QuanRptIncl:1;
  /*! \brief BLER reporting is requested? If this is yes, next two fields are Mandatory */
  uint32 BlerRptIncl:1;
  /*! \brief True specifies that BLER to be reported for all DL transport channels. If set to TRUE
     ** Ignore the TrchNum and RptTrchId */
  uint32 RptAllChIncl:1;
  /*! \brief  Number of Transport Channels. 1..32 */
  uint32 TrchNum:6;
  /*! \brief number of Cctrch */
  uint32 CctrchNum:4; 
    /*! \brief Reporting Mode */
  uint32 ReportTriggerMode:4;
  /*! \brief transport channel number for event trigger,range[1 ..32 ],
     it decides the num of QuaEventConfig[]*/
  uint32 TrchNumForEvent:8;
  //32 bits
  /*! \brief Quality measurement period config */
  tdssrchlog_quality_period_config_type QuaPeriodConfig;
  //32 bits
  /*! \brief trchsport id corresponding to TrchNum*/
  uint8 RptTrchId[TDSSRCHLOG_MAX_TRANSPORT_CHANNELS];
  //32*8 bits 
  /*! \brief tfcs id corresponding to CctrchNum*/
  uint8 TfcsId[TDSSRCHLOG_L1_MAX_CCTRCH];
  //8*8 bits 
  /*! \brief Quality measurment event config */
  tdssrchlog_quality_event_config_type QuaEventConfig[TDSSRCHLOG_MAX_TRANSPORT_CHANNELS];
  //32*64 bits
} tdssrchlog_quality_meas_config_pkt_type;

/* Macro to determine the sizes of quality config log packet */
#define TDSSRCHLOG_MEASURE_QUALITY_PKT_LEN(trch_num,Cctrch_num,event_num)  ( \
    FPOS(tdssrchlog_quality_meas_config_pkt_type, RptTrchId)+  \
    ( trch_num * sizeof(uint8))+ \
    ( Cctrch_num * sizeof(uint8))+ \
    ( event_num * sizeof(tdssrchlog_quality_event_config_type)))


/*! \brief  LOG PACKET: TDSCDMA_MEASUREMENT_QUALITY_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD00C)

This packet records the transport channel quality. It is generated once every 80ms.

The worst case packet size for this log is 276 bytes. 
32+64*32+16*8 = 2208 bits
*/

typedef struct
{ /*! \brief Cctrch identity [1,32]*/
  uint8 CctrchId;
  /*! \brief timeslot number*/
  uint8 TsNum;
  /*! \brief SIR [0,63]*/
  uint8 SIR[TDSSRCHLOG_DL_MAX_NUM_TS];
  //8*8 = 32*2 bits
} tdssrchlog_sir_result_type;

typedef struct
{
  /*! \brief DL Transport channel identity [1,32]*/
  uint8 TrchId;
  /*! \brief DL Transport Channel BLER [0,63]*/
  uint8 TrchBLER;
  //16bits
} tdssrchlog_bler_result_type;

/*! \brief  LOG PACKET: TDSCDMA_MEASUREMENT_QUALITY_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD00C)

This packet records the transport channel quality. It is generated once every 80ms.

MIN Size: 4 bytes, MAX Size: 132 bytes
*/
typedef struct
{
  /*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint32  version:8;
  /*! \brief measurement id*/
  uint32 MeasId:5;
  /*! \brief indicate if it is a period report*/
  uint32 PeriodRptIncl:1;
  /*! \brief DL Transport channel number*/
  uint32 TrchNum:6;
  /*! \brief Cctrch number*/
  uint32 CctrchNum:4;
  uint32 :8;  
  //32bits
  /*! \brief BLER results*/
  tdssrchlog_bler_result_type BlerResult[TDSSRCHLOG_MAX_TRANSPORT_CHANNELS];
  //64*32 bits
  /*! \brief SIR results*/
  tdssrchlog_sir_result_type SirResult[TDSSRCHLOG_MAX_CCTRCH];
  //16*8 bits  
} tdssrchlog_quality_results_pkt_type;

/* Macro to determine the sizes of quality results log packet */
#define TDSSRCHLOG_QUALITY_RESULT_PKT_LEN(trchnum,cctrchnum)  ( \
    FPOS(tdssrchlog_quality_results_pkt_type, BlerResult)+  \
    ( trchnum * sizeof(tdssrchlog_bler_result_type))+  \
    ( cctrchnum * sizeof(tdssrchlog_sir_result_type)))


/*! \brief  LOG PACKET: TDSCDMA_CELL_RESELECT_CFG_LOG_PACKET
 				 (LOG CODE: 0xD004)

This packet contains the Cell Reselection configuration parameters. 
The packet is generated when the cell reselection measurements is initiated or modified.  

MIN Size: 34 bytes, MAX Size: 96 bytes
*/
 typedef struct
 {	
   /* ! \brief  HCS measurement threshold. Value (in dB) ranges (-105..91)  
     ** by step of 2. 0xFFFF indicates the parameter is not applicable. */	   
   int8    s_hcsrat;   
 
   /* ! \brief  Value (in dB) ranges (-105..91) by step of 2. 0xFFFF indicates 
     ** the parameter is not applicable.	 */
   int8	s_limit_srchrat;   
   
   /*! \brief  Inter-RAT (inter-system) measurement threshold. Value (in dB) 
     ** ranges (-105..91) by step of 2. 0xFFFF indicates the parameter
     ** is not applicable*/	   
   int8	s_interrat;	
   /*! \brief interrat type*/
   uint8    rat_type:3;  
   
   /*! \brief  Flag indicating whether s_interrat is absent */
   uint8	s_interrat_absent:1;  
	 
   /*! \brief  Flag indicating whether s_srchhcs is included */
   uint8	s_hcsrat_incl:1;	
   
   uint8	:3;	
   //32bit
 } tdssrchlog_interrat_resel_info_type;


typedef struct
{
  /*! \brief  Serving cell HCS priority. Value ranges (0..7). Default value is 0. */
  uint32   hcs_prio:8;

  /* ! \brief  Quality threshold level for HCS cell reselection. Value ranges (0..99). Default value is 0. */
  uint32   q_hcs:8;
  
  /*! \brief maximum number fo cell reselections. Value ranges (1..16).  Default value is 8. */  
  uint32   n_cr:8;
  
  /* ! \brief  Duration for evaluating alowed amount of cell reselections. 
    ** Default value is 0, which means the parameter is not used. When used, 
    ** the values are (0, 30, 60, 120, 180, 240), (in second). */  
  uint16   t_crmax;
  
  /* ! \brief  Additional time period before UE recert to low-mobility measurement. 
  ** Default value is 0, which means the parameter is not used. When used, 
  ** the value ranges (10..70) by step of 10, (in second). */  
  uint16   t_crmaxhyst;
  //32*2 bits
} tdssrchlog_serv_cell_hcs_info_struct_type;

/*! \brief  LOG PACKET: TDSCDMA_CELL_RESELECT_CFG_LOG_PACKET
 				 (LOG CODE: 0xD004)

This packet contains the Cell Reselection configuration parameters. 
The packet is generated when the cell reselection measurements is initiated or modified.  

MIN Size: 34 bytes, MAX Size: 96 bytes
*/

typedef struct
{

  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! Reserved */
  uint32 reserved:24;
  //32bits
  
  /*! \brief   whether s_intrasrch is included */
  uint8 SintraIncl:1;
  /*! \brief   whether s_intersrch is included */
  uint8 SinterIncl:1;
  /*! \brief   whether s_srchhcs is included */
  uint8 ShcsIncl:1;
  /*! \brief   Flag indicating whether info is included for any interrat neighbors  */
  uint8 SiratIncl:1;
  /*! \brief   DDH THIS NOT NEEDED num_rats should be and is expected to be zero if 
       there are no other rats.  Number of additional RATs included */
  uint8 NumIrat:4;
  /*! \brief   Intra-frequency measurement threshold. Value (in dB) ranges (-105..91) 
        by step of 2. 0xFFFF indicates the parameter is not applicable. */	
  int8 Sintra;
  /*! \brief   Inter-frequency measurement threshold. Value (in dB) ranges (-105..91) 
        by step of 2. 0xFFFF indicates the parameter is not applicable. */
  int8 Sinter;
  /*! \brief   HCS measurement threshold. Value (in dB) ranges (-105..91) 
        by step of 2. 0xFFFF indicates the parameter is not applicable. */
  int8 Shcs;
  //32bits
  
  /*! \brief   Hysteresis value of the serving cell for RSCP (?), It is used for 
       calculating cell-ranking R in HCS. Value ranges (0..40) by step of 2. */  
  uint16   qhyst1_s;

  /*! \brief   Cell reselection timer, integer (0..31000) in milli seconds */
  uint16   trselc_ms;
  //32bits
  
  /*! \brief   Maximum allowed UL TX power, UE_TXPWR_MAX_RACH (dBm) */
  int16   max_tx_pwr;
  
  /*! \brief   Minimum required received level for RSCP measurement (in dBm). 
       Value ranges (-115..-25) by step of 2. */
  int8   q_rxlevmin;

  /*! \brief   Offset to the signalled Qrxlevmin taken into account in the Srxlev 
  ** evaluation as a result of a periodic search for a higher priority  
  ** PLMN while camped normally in a VPLMN. SIB3 only in v770 Ext.      
  ** Value ranges (2..16) by step of 2. Default to 0 */  
  uint8   q_rxlevmin_offset:7;
  
  /*! \brief   Flag indicating if HCS info below is applicable */
  uint8 hcs_info_incl:1;
  //32bits

  /* ! \brief  Serving cell HCS information */
  tdssrchlog_serv_cell_hcs_info_struct_type  hcs_info;

  /* ! \brief  maximum number fo cell reselections. Value ranges (1..16).  Default value is 8. */  
  uint16   nhcs_n_cr;

  /*! \brief   Duration for evaluating alowed amount of cell reselections. 
  ** Default value is 0, which means the parameter is not used. When used,
  ** the values are (0, 30, 60, 120, 180, 240), (in second). */
  uint16   nhcs_t_crmax;
  //32 bits

  /*! \brief   Additional time period before UE recert to low-mobility measurement. 
  ** Default value is 0, which means the parameter is not used. When used, 
  ** the value ranges (10..70) by step of 10, (in second). */  
  uint8   nhcs_t_crmaxhyst;

  /* ! \brief  0 to 10;  coresponds  speed dependent scale factor/10 */
  uint8 tresel_scale;

  /*! \brief   4 to 19. corresponds to interf scale factor / 4*/
  uint8 tresel_interf_scale ;

  /* ! \brief  4 to 19. corresponds to inter-rat scalefactor/ 4*/
  uint8 tresel_irat_scale;   
  //32 bits
  /*! \brief interrat info,it's number is decided by NumIrat*/
  tdssrchlog_interrat_resel_info_type   interrat_info[TDSSRCHLOG_MAX_OTHER_RAT];
  //32*15 bits	
}tdssrchlog_resel_config_pkt_type;

/* Macro to determine the sizes of reselection configuration log packet */
#define TDSSRCHLOG_RESEL_CONFIG__PKT_LEN(iratnum)  ( \
    FPOS(tdssrchlog_resel_config_pkt_type, interrat_info)+  \
    ( iratnum * sizeof(tdssrchlog_interrat_resel_info_type)))

/*--------------------------------------------------------------------------
        LOG PACKET: Filtered RSCP MEASUREMENT RESULT
        (LOG CODE: 0xD00A)
                     
This packet outputs the filtered P-CCPCH RSCP measurement for all intra-frequency 
and inter-frequency cells. It is generated once every 100ms. 
In the case where multiple layer 3 filters are configured for 
the measurement, the filter selected for set management is used. 
 
This packet can be formed immediately, thus use the log_alloc() and log_commit. 
Do not need to init log header if it is dynamically allocated with log_alloc. 
 --------------------------------------------------------------------------*/
/*! \brief Cell List info
 *  42 bits
 */
typedef struct
{
  /*! UARFCH  */
  uint32 uarfcn:14;
  /*! cell parameter ID  */
  uint32 cellId:7;
  /*! Filtered P-CCPCH RSCP in the range of (-120 ...-25) dBm, 
    with 0.5dB resolution.  */
  uint32 filtRscp:8;
  /*! Indicate to which set the cell belongs:
    0:ASET;1:MSET;2:DSET*/
  uint32 status:2;
  /*! 0: not belonging to JDCS; 1:belonging to JDCS;  */
  uint32 inJds:1;
//32 bits

  /*! If the cell has the same working frequency as the serving cell's  */
  uint32 intra_secondary_frequency_indicator : 1;
  uint32  : 31;
  //uint16 reserved;
    /*! wall_time_offset(28:16) = subframe offset 
    *  wall_time_offset(15:0) = ts0StartPosition (in units of Cx8)
    */
  uint32 wall_time_offset;
      /*! \brief metric M, per OTD cell, for purpose of fake cell pruning 
  @verbatim
   M = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid_1stShift|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricM;
  /*! \brief metric M prime, per OTD cell, reserved 
  @verbatim   
   M' = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricMPrime;
//32 bits
#if 0 //def FEATURE_TDSCDMA_NFTL
  uint16 filt_slot_snr;
  uint16 reserved;  /*padding*/
//32 bits
  
  int32 filt_freq_error;
#endif
} tdssrchlog_meas_cell_list_type;

/*! \brief TDSCDMA_MEASUREMENT_ACCUMULATED_RSCP_LOG_PACKET 
 (LOG CODE: 0xD00A)
                     
This packet outputs the filtered P-CCPCH RSCP measurement for all intra-frequency 
and inter-frequency cells. It is generated once every 100ms. 
In the case where multiple layer 3 filters are configured for 
the measurement, the filter selected for set management is used. 
 
MIN Size: 8 bytes, MAX Size: 776 bytes
*/

typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! Reserved */
  uint32 reserved:13;
  /*! total frequency number  */
  uint32 freqNum:4;
  /*! total cell number  */
  uint32 cellNum:7;
  //32 bits

  /*! Measured RSCP threshold for adding a cell to the DSet; 
    In the range of (-120 ...-25) dBm, with 0.5dB resolution.  */
  int8 QdetectIn;
  /*! Filtered RSCP threshold for dropping a cell from DSet; 
    In the range of (-120 ...-25) dBm, with 0.5dB resolution.  */
  int8 QdetectOut;
  /*! Filtered absolute RSCP threshold for adding a cell to the JDCS; 
    In the range of (-120 ...-25) dBm, with 0.5dB resolution.  */
  int8 QjdcsAbs;
  /*! Filtered relative RSCP (to the serving cell) 
    threshold for adding a cell to the JDCS;  Range (-120..-25)dB  */
  int8 QjdcsRel;
  //32 bits
  
  /*! Serving cell Pathloss; Q8*/
  uint16 Pathloss;
  
  uint16 reserved2;
  /*! cell info  */
  tdssrchlog_meas_cell_list_type cellList[TDSSRCHLOG_MAX_TD_CELLS];

} tdssrchlog_meas_rscp_result_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_MEASURE_RSCP_RESULT_PKT_LEN(num)  ( \
    FPOS(tdssrchlog_meas_rscp_result_pkt_type, cellList)+  \
    ( num * sizeof(tdssrchlog_meas_cell_list_type)))

/*--------------------------------------------------------------------------
        LOG PACKET: Filtered ISCP MEASUREMENT RESULT
        (LOG CODE: 0xD00B)
                     
This packet holds the ISCP measurement result. It is generated once every 400ms. 
 
This packet can be formed immediately, thus use the log_alloc() and log_commit. 
Do not need to init log header if it is dynamically allocated with log_alloc. 
 --------------------------------------------------------------------------*/
/*! \brief TDSCDMA_MEASUREMENT_ACCUMULATED_ISCP_LOG_PACKET
  (LOG CODE: 0xD00B)
 
This packet holds the ISCP measurement result. It is generated once every 400ms. 
 
MIN Size: 4 bytes, MAX Size: 36 bytes.
 */

typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  /*! Reserved */
  uint32 reserved:16;
  /*! ISCP pair number  */
  uint32 iscp_pair:8;

  /*! \brief ISCP measurement slot & cell configuration	*/
  tdssrchlog_measure_iscp_slot_cell_type slotCell[TDSSRCHLOG_MAX_ACCUMULATE_NUM_ISCP];

} tdssrchlog_meas_iscp_result_pkt_type;

/* Macro to determine the sizes of the log packet */
#define TDSSRCHLOG_MEASURE_ISCP_RESULT_PKT_LEN(num)  ( \
    FPOS(tdssrchlog_meas_iscp_result_pkt_type, slotCell)+  \
    ( num * sizeof(tdssrchlog_measure_iscp_slot_cell_type)))

/*--------------------------------------------------------------------------
                 LOG PACKET: Measurement Scheduling Information(Internal)
                 (LOG CODE: 0xD008)
                     
This packet outputs the information for the measurement scheduling algorithm 
for FACH/DCH states. It is generated every time at the measurement scheduling 
point.(5 ms)
 
This packet will take some time to accumulate, must use the log_submit() API.
log_set code(), log_set_length(), and log_set_timestamp() must be used to 
populate the log header prior to submit. 
--------------------------------------------------------------------------*/
/*! \brief GSM cell info
 *  32 bits total
 */
typedef struct
{
  /*! \brief Integer (1..32), which is the cell index given in the inter-RAT Cell Info. List*/
  uint32 cellID:6;
  /*! \brief 0:DCS 1800 band used 1:PCS 1900 band used)*/
  uint32 bandIndicator:1;
  /*! \brief ARFCN*/
  uint32 arfcn:10;
  /*! \brief NCC*/
  uint32 ncc:3;
  /*! \brief BCC*/
  uint32 bcc:3; 
  uint32 :9; 
  //32 bits
} tdssrchlog_gsm_cell_info_type;

/*! \brief Measurement GAP info 
 */
typedef struct
{
  /*! \brief the subframe number of the GAP starting position*/
  uint32 startSubfnNum:16;
  /*! \brief the chip number of the GAP starting position*/
  uint32 startChipNum:16;
  //32 bits
  
  /*! \brief the GAP length in length*/
  uint32 gapLength:28;
  /*! \brief the GAP purpose for GSM measurement*/
  uint32 measPurpose:4;  
  //32 bits

  /*! \brief the number of GSM cells scheduled for the GAP*/
  uint32 gsmCellNum:4;
  /*! \brief the next index of GSM cells scheduled for the GAP*/
  uint32 nextRssiIdx:6;
  /*! \brief the RF cleanup buffer index for this measurement*/
  uint32 rfCleanupBufIdx:8;
  /*! \brief whether this gap is in use*/
  uint32 inUse:2;  
  uint32 :12;
  //32 bits

  /*! \brief actual gap start time for TFW reference*/
  uint32 tfwStartTime;
  //32 bits
  
  /*! \brief actual gap end time for TFW reference*/  
  uint32 tfwEndTime;  
  //32 bits

  /*! \brief command send time-sub frames*/  
  uint16 CmdSentSubfn;  
  /*! \brief command send time-1/8 chips*/    
  uint16 CmdSentChipx8;  
  //32 bits
  
  /*! \brief GSM cell info,	actual number determined by gsmCellNum*/
  tdssrchlog_gsm_cell_info_type gsmCell[TDSSRCHLOG_MAX_GSM_CELL_PER_GAP];
  //32*10 bits

} tdssrchlog_meas_gap_info_type;

/*! \brief TDSCDMA MEASURE_ISCP SLOT CELL FREQ
 *  32 bits total
 */
typedef struct
{
  /*! \brief Slot Index	*/ 
  uint8                     slotIdx;
  /*! \brief Cell midamble index */
  uint8                     midambleIdx;
  /*! \brief UARFCH for ISCP */
  uint16                    uarfcn;
  //32 bits
} tdssrchlog_meas_iscp_pair_type;

typedef struct
{
  /*! \brief GSM fn number  */
  uint32 gfn;
  /*! \brief GSM qs number  */
  uint32 qs;
} tdssrchlog_gsm_time_info_type;

typedef struct
{
  uint32 gfn_current;
  tdssrchlog_gsm_time_info_type gtime_latched;
  tdssrchlog_gsm_time_info_type gtime_gapstart[TDSSRCHLOG_MAX_MEAS_GAPS];
} tdssrchlog_gsm_latched_time_curr_info_type;

typedef struct
{
  uint32 gfn_current;
  tdssrchlog_gsm_time_info_type gtime_latched;
  tdssrchlog_gsm_time_info_type gtime_gapstart;
} tdssrchlog_gsm_latched_time_info_type;

/*! \brief TDSCDMA_MEASUREMENT_SCHEDULE_LOG_PACKET
  (LOG CODE: 0xD008)
 
This packet is for internal use only! 
 
This packet outputs the information for the measurement scheduling algorithm 
for FACH/DCH states. It is generated every time at the measurement scheduling 
point.(5 ms)
 
MIN Size: 16 bytes, MAX Size: 272 bytes. 
*/ 
typedef struct
{
  /* Log header */
  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  uint32  :24;
  //32 bits

  /*! \brief UE state, FACH: 0, DCH: 1  */
  uint32 ueState:1;
  /*! \brief the subframe number of the Schedule Point, range is 0 to 8191 */
  uint32 scheduleSubfnNum:13;
  /*! \brief the location of the Schedule Point in the subframe in chips, range is 0 to 6399 */
  uint32 scheduleChipNum:13;
  /*! \brief triggered by SCCH ISR?*/
  uint32 scchTrigger:1;
  uint32 :4;
  //32 bits

  /*! \brief Bitmask to indicate the slot usage of the first subframe. 
   - Two bit for each slot (MSB to LSB) for time slot 0,1,2,6,UpPTS:
        00 - idle; 01-uplink slot; 10-downlink slot; 11- unknown */
  uint32 slotUsage1st:16;
  /*! \brief Bitmask to indicate the slot usage of the first subframe. 
   - Two bit for each slot (MSB to LSB) for time slot 0,1,2,6,UpPTS:
        00 - idle; 01-uplink slot; 10-downlink slot; 11- unknown */
  uint32 slotUsage2nd:16;
  //32 bits

  /*! \brief is Profiling valid */
  uint16 profiling_valid;
  /*! \brief Profiling time Info */
  uint16 profiling_time_cx8;
  //32 bits

  /*! \brief the frequency number for RSCP measurement*/
  uint32 rscpUarfcn:16;
  /*! \brief the number of cells whose RSCP is scheduled at this schedule point*/
  uint32 rscpCellNum:8;
  /*! \brief the number of ISCP pairs scheduled at this schedule point*/
  uint32 iscpPairNum:4;
  /*! \brief the number of GAPs scheduled for GSM at this schedule point*/
  uint32 gsmgapNum:4;
  //32 bits

  /*! \brief Cell IDs for RSCP measurement, actual number determined by rscpCellNum*/
  uint8 rscpCellID[TDSSRCHLOG_MAX_INTRAF_MEAS_CELLS];

  /*! \brief ISCP measurement slot & cell configuration, actual number determined by iscpPairNum*/
  tdssrchlog_meas_iscp_pair_type slotCell[TDSSRCHLOG_MAX_NUM_ISCP]; 

  /*! \brief GSM latched time info, actual number determined by gsmgapNum*/
  tdssrchlog_gsm_latched_time_info_type gsmLatchTime[TDSSRCHLOG_MAX_MEAS_GAPS];

  /*! \brief GSM measurement GAP info, aactual number determined by gsmgapNum*/
  tdssrchlog_meas_gap_info_type gapInfo[TDSSRCHLOG_MAX_MEAS_GAPS];

} tdssrchlog_meas_schedule_pkt_type;

/* Macro to determine the sizes of meas schedule log packet */
#define TDSSRCHLOG_MEAS_SCHEDULE_PKT_LEN(rscp_no,iscp_no,latch_num,gap_num) ( \
    FPOS(tdssrchlog_meas_schedule_pkt_type, rscpCellID)+  \
    (rscp_no * sizeof(uint8)) + \
    (iscp_no * sizeof(tdssrchlog_meas_iscp_pair_type)) +\
    (gap_num * sizeof(tdssrchlog_gsm_latched_time_info_type)) + \
    (gap_num * sizeof(tdssrchlog_meas_gap_info_type)))

/*--------------------------------------------------------------------------
 				 LOG PACKET: TDSCDMA_MEASUREMENT_CFG_LOG_PACKET
 				 (LOG CODE: 0xD00D)

This packet contains the intra-frequency, inter-frequency and/or inter-RAT 
measurement configuration parameters. 

The packet is generated when the CPHY measurement reqeust is received. 
--------------------------------------------------------------------------*/
/*! \brief Cell Selection Re-Selection Info Structure Type
 */
typedef PACK(struct)
{
  /*! Offset used for CCPCH RSCP. Integer (-50..50), Default 0 */
  int16        qoffset_s_n;
  int16        reseved;
  //32 bits  

  /*! Maximum allowed ULTX power. UE_TXPWR_MAX_RACH dBm */
  int16        max_tx_pwr;

  /*! For FDD & TDD Cells: Q-rxlevmin for RSCP (dBm). 
      For GSM: Q-rxlevmin for GSM RSSI (dBm). Int (-115..-25 by step of 2).
      Default value is Q-rxlevmin for serving cell.   
  */
  int16        rxlev_min;
  //32 bits
} tdssrchlog_cell_sel_resel_info_struct_type;

/*! \brief HCS neighbor cell information
 */
typedef  PACK(struct)
{
  /*! Hierachical Cell Structrue Priority */
  uint16        hcs_prio;
  
  /*! Quality Threshold level for HCS cell reselection */    
  uint16        q_hcs;
  //32 bits

  /*! Penalty_time. 0 means not used. next two not allowed for 0 */    
  uint16        penalty_time;

  /*! Temporary_offset used for PCCPCH RSCP*/    
  uint16        tmp_offset;
  //32 bits
} tdssrchlog_nbr_cell_hcs_info_struct_type;

/*! \brief Cell Info Structure Type
 */
typedef  PACK(struct)
{
  /*! Cell Individual Offset, Real (-10..10 by step of 0.5) dB, default 0  */
  int32        cell_offset;
  //32 bits

  /* Cell Prarameter ID. (0..127) */ 
  uint8       cpi;

  /*! Flag indicating whether Ref time diff to the serving cell is included */
  uint8       ref_tm_diff_incl:1;
  /*! Flag indicating whether PCCPCH Tx Power is included */
  uint8       pccpch_txpwr_incl:1;
  /*! Neighbor Cell timing accuracy relative to the serving cell, 
      0 - 40 chips,
      1 - 256 chips,
      2 - 2560 chips */    
  uint8       time_acc:2;
  /*! Read SFN Indicator.TRUE means read of SFN requested for target cell*/  
  uint8       rd_sfn_ind:1;
  /*! Flag to indicate if cell selection and reselection info is specified for this cell or not. */  
  uint8       cell_sel_resel_info_incl:1;
  /*! Flag indicating whether Neighbor cell HCS information is included */  
  uint8       hcs_nbr_incl:1;
  uint8       :1;
  
  /*! Reference Time difference to Serving Cell, integer (0..38400) */ 
  uint16        ref_tm_diff;
  //32 bits
  
  
  /*! PCCPCH Tx power. Needed if calculating patchloss
      Units: Int  -10... +50 dBm 
  */
  int16         pccpch_txpwr;
  /*! the number of timeslots for ISCP measurement*/
  uint16        no_ts;
  //32 bits

  /*! timeslot list for ISCP measurement*/
  uint8         ts_list[6];
  
  /*! Cell selection and reselection info. Present only when cell_sel_resel_info_incl is TRUE.
   */
  tdssrchlog_cell_sel_resel_info_struct_type  cell_sel_info;
  

  /*! Neighbor cell HCS information. Present only when hcs_nbr_incl is TRUE.
  */  
  tdssrchlog_nbr_cell_hcs_info_struct_type  hcs_info;  
  
} tdssrchlog_cell_info_struct_type;

/*! \brief Intra-Frequency Cell Info List
 */
typedef  PACK(struct)
{
  /*! Number of intra-frequency cells removed, 0-32 */  
  uint32      num_cell_rmv:8;
  /*! Number of intra-frequency cells Added, 0-32 */
  uint32      num_cell_add:8;
  /*! Number of cells to measure, 0-32 */
  uint32      num_cell_meas:8;
  uint32      :8;
  //32 bits

  /*! Removed intra-freqency cells, actual number is determined by num_cell_rmv */
  uint8 rmv_cpid_list[TDSSRCHLOG_MAX_CELL_MEAS];

  /*! Only cells mentioned in this list should be measured. actual number is determined by num_cell_meas */
  uint8 meas_cpid_list[TDSSRCHLOG_MAX_CELL_MEAS ];
  
  /*! New intra-frequency cells, actual number is determined by num_cell_add */
  tdssrchlog_cell_info_struct_type  add_cell_info[ TDSSRCHLOG_MAX_CELL_MEAS ];
  
} tdssrchlog_intra_freq_cell_list_struct_type;

/*! \brief
    Intra-frequency measurement quantity
 */
typedef  PACK(struct)
{
  /*! Filter coefficient as in 10.3.7.9 */
  uint16    filter_coef;
  
  /*! Number of Measurement quantity. */
  uint8         meas_quan_list_no;

  /*! Measurement quantity. Following Measurement Quantity are supported for DCH Measurement report
  1 - TDSL1_ISCP,
  2 - TDSL1_RSCP,
  3 - TDSL1_PATHLOSS,
  */
  uint8   meas_quan0:2;
  uint8   meas_quan1:2;  
  uint8   meas_quan2:2;  
  uint8   meas_quan3:2;  
  //32 bits
} tdssrchlog_intra_freq_meas_quan_struct_type;

typedef PACK(struct)
{
	/* GSM_CARRIER_RSSI 
		0:TDSL1_GSM_CARRIER_RSSI
	*/	
  uint8  meas_quan:1;

  /* Filter Coefficient 
		TDSL1_GSM_FILTER_COEF_0 = 0,
	  TDSL1_GSM_FILTER_COEF_1 = 1,
	  TDSL1_GSM_FILTER_COEF_2 = 2,
	  TDSL1_GSM_FILTER_COEF_3 = 3,
	  TDSL1_GSM_FILTER_COEF_4 = 4,
	  TDSL1_GSM_FILTER_COEF_5 = 5,
	  TDSL1_GSM_FILTER_COEF_6 = 6,
	  TDSL1_GSM_FILTER_COEF_7 = 7,
	  TDSL1_GSM_FILTER_COEF_8 = 8,
	  TDSL1_GSM_FILTER_COEF_9 = 9,
	  TDSL1_GSM_FILTER_COEF_11 = 11,
	  TDSL1_GSM_FILTER_COEF_13 = 13,
	  TDSL1_GSM_FILTER_COEF_15 = 15,
	  TDSL1_GSM_FILTER_COEF_17 = 17,
	  TDSL1_GSM_FILTER_COEF_19 = 19,
	  TDSL1_GSM_FILTER_COEF_MAX,
	  TDSL1_GSM_FILTER_COEF_INVALID
	*/
  uint8  filter_coef:5;

  /* True indicates BSIC Verification Required */
  uint8  bsic_ver_req:1;

  uint8  padding:1;
}tdssrchlog_gsm_meas_quan_struct_type;

//#ifdef FEATURE_TDSCDMA_TO_LTE
typedef PACK(struct)
{
	/*! \brief EUTRA measurement quantity
	0:TDSL1_EUTRA_QUAN_RSRP,
  1:TDSL1_EUTRA_QUAN_RSRQ,
  2:TDSL1_EUTRA_QUAN_BOTH,
  3:TDSL1_EUTRA_QUAN_NONE
  */
  uint8  meas_quan:2;
  /*! \brief Filter Coefficient: DEFAULT fc0 
	TDSL1_EUTRA_FILTER_COEF_0 = 0,
  TDSL1_EUTRA_FILTER_COEF_1 = 1,
  TDSL1_EUTRA_FILTER_COEF_2 = 2,
  TDSL1_EUTRA_FILTER_COEF_3 = 3,
  TDSL1_EUTRA_FILTER_COEF_4 = 4,
  TDSL1_EUTRA_FILTER_COEF_5 = 5,
  TDSL1_EUTRA_FILTER_COEF_6 = 6,
  TDSL1_EUTRA_FILTER_COEF_7 = 7,
  TDSL1_EUTRA_FILTER_COEF_8 = 8,
  TDSL1_EUTRA_FILTER_COEF_9 = 9,
  TDSL1_EUTRA_FILTER_COEF_11 = 11,
  TDSL1_EUTRA_FILTER_COEF_13 = 13,
  TDSL1_EUTRA_FILTER_COEF_15 = 15,
  TDSL1_EUTRA_FILTER_COEF_17 = 17,
  TDSL1_EUTRA_FILTER_COEF_19 = 19,
  TDSL1_EUTRA_FILTER_COEF_MAX,
  TDSL1_EUTRA_FILTER_COEF_INVALID*/
  
  uint8 filter_coef:5;

  //padding
  uint8 reserved:1;
}tdssrchlog_eutra_meas_quan_struct_type;
//#endif

/*! \brief
	tdssrchlog_inter_sys_meas_quan_struct_type
	total 56bits
*/
typedef PACK(struct)
{
	/*! \brief indicating whether intra meas quantity is included*/
	uint8 intra_meas_quan_incl:1;
	
	/*! \brief
  0: TDSL1_GSM_SYSTEM,
  1: TDSL1_IS2000_SYSTEM,
  2: TDSL1_EUTRA_SYSTEM,
  3: TDSL1_SYSTEM_NONE\
  */
	uint8 system_choice:2;

	uint8 reserved:5;

	//8bits
	/*! \brief intra measurement quantity*/
	tdssrchlog_intra_freq_meas_quan_struct_type intra_meas_quan;
	//32bits

	/*! \brief inter rat measurement quantity*/
  union
  {
  	/*! \brief gsm measurement quantity*/
    tdssrchlog_gsm_meas_quan_struct_type      gsm_meas_quan;
		//8bits
		/*! \brief is2000 measurement quantity*/
    uint16   is2000_meas_quan;
		//16bits
//#ifdef FEATURE_TDSCDMA_TO_LTE
    /*! \brief eutra measurement quantity*/
    tdssrchlog_eutra_meas_quan_struct_type    eutra_meas_quan;
		//8bits
//#endif
  } u;
	//16bits
	
}tdssrchlog_inter_sys_meas_quan_struct_type;

/*! \brief
    Report quantities
 */
typedef  PACK(struct)
{
  /*! SFN-SFN observed time diff reporting indicator 
    0 - TDSL1_NO_REPORT,
    1 - TDSL1_REPORT_TYPE_1,
    2 - TDSL1_REPORT_TYPE_2
  */
  uint8 sfn_rpt:2;

  /*! Cell synchronization information reporting indicator */
  uint8   cfn_sfn_delta_rpt:1;

  /*! Cell Identity reporting indicator */
  uint8   cell_id_rpt:1;
  
  /*! PCCPCH ISCP reporting indicator */
  uint8   iscp_rpt:1;

  /*! PCCPCH RSCP reporting indicator */
  uint8   rscp_rpt:1;

  /*! Pathloss reporting indicator */
  uint8   pathloss_rpt:1;

  //8 bits
} tdssrchlog_intra_dch_rpt_quan_struct_type;

/*tdssrchlog_dch_gsm_rpt_quan_struct_type
	total 8bits
*/
typedef PACK(struct)
{
  /*Xiaoning note: this field should be removed*/
  /* Observed time difference to GSM cell Reporting indicator */
  uint8   time_diff:1;

  /* GSM Carrier RSSI Reporting indicator */
  uint8   rssi_rpt_ind:1;
} tdssrchlog_dch_gsm_rpt_quan_struct_type;

/*
	tdssrchlog_inter_sys_dch_rpt_quan_struct_type
	total:16bits
*/
typedef PACK(struct)
{
	/* Not used in this release should always be FALSE */
  uint8 utran_est_qual:1;

	/*0:TDSL1_GSM_SYSTEM,
	   1:TDSL1_IS2000_SYSTEM
	    2:TDSL1_EUTRA_SYSTEM
	   3:TDSL1_SYSTEM_NONE
	*/
  uint8 sys_choice:2;

	//padding:
  uint8 reserved:5;
	//8bits
//#ifdef FEATURE_TDSCDMA_TO_LTE
  union
  {
		 /*TDSL1_EUTRA_QUAN_RSRP = 0,
		  TDSL1_EUTRA_QUAN_RSRQ = 1,
		  TDSL1_EUTRA_QUAN_BOTH = 2,
		  TDSL1_EUTRA_QUAN_NONE = 3*/
		uint8 eutra_rpt_quan:2;
		tdssrchlog_dch_gsm_rpt_quan_struct_type   gsm_rpt_quan;
		/*Even without this padding, this union will take 8 bits in the log packet. 
		This uint8 padding field is to remind that this union will take 8 bits in the log packet, instead of 2 bits.*/
		uint8 padding; 
  }u;
  //padding
  //uint8 reserved2:6;   
//8bits
//#endif

}tdssrchlog_inter_sys_dch_rpt_quan_struct_type;

/*! \brief
    Reporting cell status
 */
typedef  PACK(struct)
{
  /*! Reporting cell status possible combonations. 10.3.7.61 of T25.331
   0 - Report cells within Active Set 
   1 - Report cells within Monitored Set on used Freq.   
   2 - Report cells within Aset and MSet on used Freq. 
   3 - Report cells within Detected Set 
   4 - Report cells within Detected Set on used Freq. 
   5 - Report All Aset + MSet on used Freq. 
   6 - Report All Aset + DSet on used Freq. 
   7 - Report All Aset + MSet and/or DSet on used Freq. 
   8 - Report cells within Virtual Active Set 
   9 - Report cells within Monitored Set on Non-used Freq. 
   10 - Report cells within MSet and/or Aset on Non-used Freq. 
   11 - Report cells within Virtual ASet + Mset on Non-used Freq. 
   12 - Report cells within Active Set + Virtual ASet  or of the Other RAT. 
   13 - Report cells within MSet and/or Aset on Used and Non-used Freq.
   */ 
  uint8 rpt_cell_stat;

  /*! Reporting cell type. 10.3.7.61 of T25.331 
  0 - TDSL1_CELL_REPORTING_TYPE1,    
  1 - TDSL1_CELL_REPORTING_TYPE2,     
  2 - TDSL1_CELL_REPORTING_TYPE3      
  */
  uint8 rpt_cell_type;  
  
  /*! Maximum number of reported cells */
  uint8 max_num_rpt_cells ;
  
  uint8 reserved;
  //32 bits     
} tdssrchlog_meas_rpt_cell_status_struct_type;

/*! \brief
    criteria per event as defined in 10.3.7.39 of TS25.331.
 */
typedef  PACK(struct)
{
  /*! Event ID 
  0 - TDSL1_INTRA_FREQ_EVENT_1G,
  1 - TDSL1_INTRA_FREQ_EVENT_1H,
  2 - TDSL1_INTRA_FREQ_EVENT_1I,
  */
  uint8  evt_id;

  /*! 0..29 by step of 0.5dB for event 1A and 1B. Set to 0xFF for all
      other events. 
  */
  uint8   rpt_range;

  /*! Number of cells foebidden to affect report range for event 1A and 1B. 
      set to 0xFF for all other events. 
  */   
  uint8  num_cell_forbidded;

  /*! Weight for event 1A and 1B. 0..20 by step of 0.1. set to 0xFF
       for all other events. 
  */
  uint8  w;
  //32 bits


  /*! Hysteresis. 0..7.5 by step of 0.5 dB, actual value is hyst*2. */
  uint16  hyst;

  /*! Threshold used for frequency for event 1E, 1F, 1H and 1I. -125..165. 
      Range used depend on measurement quantity: 
      PCCPCH ISCP -115~-25dbm;             
  */  
  int16  freq_thresh;
  //32 bits


  /*! Time duration for which the event triggering condition      
      needs to be fulfilled before the report can be sent, in ms. 
  */    
  uint16  time_to_trig;


  /*! Flag to indicate if Reporting cell status is included */
  uint16  rpt_cell_status_incl:1;
  /*! Measurement is "released" after the indicated amount (number) of   
      reporting(s) from the UE itself. 0 means "infinity". 
  */  
  uint16  num_rpt:15;
  //32 bits

  /*! Reporting cell status, present only when rpt_cell_status_include is TRUE */
  tdssrchlog_meas_rpt_cell_status_struct_type rpt_cell_status;
  //32 bits

  /*! forbidden cell id, actual number is determined by num_cell_forbidded */
  uint8  forbidden_cell [ 32 ];

} tdssrchlog_intra_freq_event_crit_struct_type;

/*! \brief
    Intra-frequency measurement reporting criteria
 */
typedef  PACK(struct)
{
  uint16 num_event;
  
  tdssrchlog_intra_freq_event_crit_struct_type evt[8];
} tdssrchlog_intra_freq_report_crit_struct_type;


/*! \brief
    criteria per event as defined in 10.3.7.39 of TS25.331.
    total 96bits
 */
typedef PACK(struct)
{
  /*! \brief Event ID 
	0: TDSL1_INTER_RAT_EVENT_3A,
  1: TDSL1_INTER_RAT_EVENT_3B,
  2: TDSL1_INTER_RAT_EVENT_3C,
  3: TDSL1_INTER_RAT_EVENT_3D 
  */
  uint32  evt_id:2;

 /*! \brief Used for event 3A, the actual value of W is a real number in the range
   (0.1..2.0) by 0.1 steps, value passed here is W*10
  */
  uint32   w:5;

	/*! \brief Hysteresis. 0..7.5 by .5 dB steps, value passed is Hysteresis*2 */
  uint32  hyst:4;
	
  /*! \brief Time duration for which the event triggering condition needs to
  be fulfilled before the report can be sent, in ms.
  */
  uint32  time_to_trig:16;
	
	/*! \brief indicating whether report cell status is included or not*/
  uint32   rpt_cell_status_incl:1;

  uint32   padding:4;
	//32bits

  /*! \brief Threshhold for own system, range (-115..0). Only applies to event 3A. */
  int16   own_thresh;
	//16bits
	
 /*! \brief Threshhold for other system, range (-115..0). Only applies to events
   3A, 3B and 3C.
 */
  int16   other_thresh;
	//16bits
		
  /*! \brief Reporting cell status */
  tdssrchlog_meas_rpt_cell_status_struct_type  rpt_cell_status;
	//32bits
}tdssrchlog_inter_sys_event_crit_struct_type;

/*! \brief
    Inter system measurement reporting criteria
    total 776bits
 */
typedef PACK(struct)
{
	/*! \brief event number*/
	uint8 num_event:8;
	//8bits

	/*! \brief event information*/
  tdssrchlog_inter_sys_event_crit_struct_type    evt[TDSSRCHLOG_MAX_MEAS_EVENT];
	//96*8=768bits
}tdssrchlog_inter_sys_report_crit_struct_type;

/*! \brief
    Periodical reporting criteria
 */
typedef  PACK(struct)
{
  /*! Amount of reporting (1,2,4,8,16,32,64, Infinity 
      Measurement is "released" after the indicated amount (number) of 
      report(s) from the UE itself. 0 means "infinity".   
  */
  uint16 rpt_amount;

  /*! Reporting Interval  (250,500,1000,2000, ... ) in ms, 0 is inavlid */
  uint16  rpt_interval;
} tdssrchlog_per_rpt_crit_struct_type;


/*! \brief Intra-frequency measurement system information
 */
typedef  PACK(struct)
{
  /*! Intra-frequency measurement identity number, default as 1 */
  uint16 meas_id;

  /*! Flag indicating whether the cell list is included  */
  uint16 cell_list_incl:1;

  /*! Flag indicating whether the measurement quantity is included  */    
  uint16 meas_quan_incl:1;

  /*! Flag indicating whether the RACH reporting quantity is included  */
  uint16 rach_rpt_quan_incl:1;

  /*! Flag indicating whether the max no of reported cells on RACH info is included */
  uint16 rach_rpt_max_incl:1;

  /*! Reporting info for CELL_DCH included  */
  uint16   intra_f_dch_rpt_info_incl:1;

  uint16 :11;
  //32 bits

  /*! Intra-Frequency Cell Info List as in 10.3.7.33 of TS25.331, exist only when cell_list_incl is true */
  tdssrchlog_intra_freq_cell_list_struct_type  cell_list;

  /*! Intra-frequency measurement quantity as in 10.3.7.38 of TS25.331, exist only when meas_quan_incl is true */  
  //tdsl1_intra_freq_meas_quan_struct_type  meas_quan;

  /*! Intra-frequency reporting quantity for RACH reporting as in 10.3.7.42, exis only when rach_rpt_quan_incl is true */
  //tdsl1_intra_freq_rach_rpt_quan_struct_type  rach_rpt_quan;

  /*! Maximum number of reported cells on RACH as in 10.3.7.43, exist only when rach_rpt_max_incl is true */
  //tdsl1_intra_freq_rach_rpt_max_enum_type    rach_rpt_max;

  /*! Reporting information in state CELL_DCH, as in 10.3.7.62,  exist only when intra_f_dch_rpt_info_incl is true*/
  //tdsl1_intra_freq_dch_rpt_info_struct_type  dch_rpt_info;
} tdssrchlog_intra_freq_sib_struct_type;

/*! \brief Measurement Parameters from SIB_11/12 */
typedef  PACK(struct)
{
  /*! Flag indicating if serving cell belongs to a HCS structure 
  */  
  uint32       hcs_used:1;

  /*! Flag indicating if intra-frequency measurement info is included */
  uint32      intra_freq_meas_info_incl:1;
  
  /*! Flag indicating if inter-system measurement info is included */ 
  uint32    inter_sys_meas_info_incl:1;
  uint32    :29;
  //32 bits

  /*! Intra-frequency measurement system information, exist only when intra_freq_meas_info_incl is true */
  tdssrchlog_intra_freq_sib_struct_type intra_f;

  /*! Inter-system cell information, exist only when inter_sys_meas_info_incl is true */    
  //tdssrchlog_inter_sys_cell_list_struct_type   inter_s;

} tdssrchlog_meas_sib_parm_struct_type;


/*! \brief
   Intra-frequency measurement control info 
 */
typedef  PACK(struct)
{
  /*! flag to indicate if cell info is present
      If true, cell_list is valid  
  */
  uint8 cell_list_incl:1;
  /*! flag to indicate if meas quantity is present
      If true, meas_quan is valid       
  */  
  uint8 meas_quan_incl:1 ;
  /*! flag indicating if the reporting quantities are given. 
      If true, aset_quan and mset_quan are valid 
      Intra-frequency reporting quantity as defined in 10.3.7.5 of TS25.331 
  */
  uint8 intra_f_rpt_quan_incl:1 ;

  /*! flag to indicate if reporting quantity for dset is given
      If true, dset_quan is valid  
  */
  uint8 dset_quan_incl:1;
  /*! flag to indicate if reporting cell status is given
      If true, rpt_cell_status is valid  
  */
  uint8 rpt_cell_status_incl:1;
  /*! Measurement reporting mode as in 10.3.7.49  
  TDSL1_PERIODIC_RPT,
  TDSL1_EVENT_TRIGGER_RPT,
  TDSL1_NO_RPT,
  TDSL1_NO_CHANGE,
  */
  uint8 rpt_mode:3;
  //8 bits

  /*! cell information 10.3.7.33, Present if cell_list_incl is true */
  tdssrchlog_intra_freq_cell_list_struct_type  cell_list;


  /*! Intra-frequency measurement quantity as in 10.3.7.38 of TS25.331. Present if meas_qual_incl is true */
  tdssrchlog_intra_freq_meas_quan_struct_type  meas_quan;

  /*! Rpt quantity for active set as defined in 10.3.7.41 of TS25.331. Present if intra_f_rpt_qual_incl is true */
  tdssrchlog_intra_dch_rpt_quan_struct_type    aset_quan;

  /*! Rpt quantity for monitored set as defined in 10.3.7.41 of TS25.331. Present if intra_f_rpt_qual_incl is true */
  tdssrchlog_intra_dch_rpt_quan_struct_type    mset_quan;


  /*! Rpt quantity for detected set as defined in 10.3.7.41 of TS25.331. Present if dset_qual_incl is true */
  tdssrchlog_intra_dch_rpt_quan_struct_type    dset_quan;
  
  /*! Event Triggered Measurement reporting criteria as in 10.3.7.39 of TS25.331 
      Present if "rpt_mode" is TDSL1_EVENT_TRIGGER_RPT 
  */
  tdssrchlog_intra_freq_report_crit_struct_type    rpt_crit;

  /*! Periodical Measurement reporting criteria as in 10.3.7.53 
      Present only if "rpt_mode" is TDSL1_PERIODIC_RPT 
  */  
  tdssrchlog_per_rpt_crit_struct_type periodic_crit;
  
  
  /*! Reporting cell status. Present only if rpt_cell_status_incl is true
  */
  tdssrchlog_meas_rpt_cell_status_struct_type  rpt_cell_status;
} tdssrchlog_intra_freq_meas_ctrl_struct_type;


/*! \brief
	tdssrchlog_gsm_meas_cell_info_type
	total 56 bits
*/
typedef PACK(struct)
{
  /*! \brief Cell Individual Offset value 
   Value used to offset the measured quantity in dB, 
  range (-50..50) */
  int16 cio;

	/*! \brief
		0: TDSL1_DCS_1800_BAND,    
    1: TDSL1_PCS_1900_BAND
  */
  uint8 band_indicator;

 /*! \brief BSIC 10.3.8.2  
    PLMN Colour Code low 3 bits are valid {b00000111}*/
  uint8 ncc;
  
 /*! \brief Base Station Colour Code low 3 bits are valid {b00000111} */    
  uint8 bcc;
	
  /*! \brief ARFCN  Integer (0..1023) */
  uint16  bcch_arfcn;

}tdssrchlog_gsm_meas_cell_info_type;


/*! \brief tdssrchlog_inter_sys_cell_list_struct_type
	total 3104bits
*/
typedef PACK(struct)
{
	/*! \brief Remove inter-RAT cells
  Number of inter-RAT cells removed 0=none, 1..TDSL1_MAX_CELL_MEAS
  TDSL1_REMOVE_ALL_CELLS(=0xFF)=remove all*/
  uint16  num_cell_rmv:5;

	/*! \brief Add inter-system cells 
  Number of inter-system cells Added*/
	uint16	num_cell_add:5;

	/*! \brief Reserved*/
	uint16:6;

	//16bits
	
  struct
  {
    /*! \brief Inter-RAT cell id, unique value in the range (0..<maxCellMeas-1>) */
    uint8    intersys_cell_id;
  } rmv_cell[ TDSSRCHLOG_MAX_CELL_MEAS ];

	//32*8 bits=256bits
	
  struct
  {
   /*! \brief Cell Id, Unique value in the range (0..<maxCellMeas-1>)
    Cell Id is an optional parameter for add cell, but L1 will require
    RRC to select a Cell Id for use, so the choice will be made on
    the UE in one place.
    */
    uint8  intersys_cell_id;

		 /*! \brief inidcating this cell is measured in idle pch fach state or not*/
		uint8 measure_in_idle_pch_fach;

		/*! \brief cell info*/
    tdssrchlog_gsm_meas_cell_info_type  gsm_cell;

  } add_cell[ TDSSRCHLOG_MAX_CELL_MEAS ];

	//72*32=2304 bits
	
  /*! \brief If cells_for_meas_incl is set to TRUE, then cells
  mentioned in this list should only be measured. For
  SIB's, its always set to FALSE.
  */
  uint16 cells_for_meas_incl:1;

  /*! \brief Number of cells to measure */
  uint16 num_cell_meas:8;
	
	/*! \brief Reserved*/
	uint16: 7;

	//16bits
	
 /*! \brief Only cells mentioned in this list should be measured.
  This list contains the inter-system cell id
  */
  uint16 meas_cells_list[ TDSSRCHLOG_MAX_CELL_MEAS ];

	//32*16=512bits
}tdssrchlog_inter_sys_cell_list_struct_type;

/*! \brief tdsl1_inter_sys_reporting_criteria_struct_type
   total 784bits
 */
typedef PACK(struct)
{
	/*! \brief
	0: TDSL1_PERIODIC_RPT,
  1: TDSL1_EVENT_TRIGGER_RPT,
  2: TDSL1_NO_RPT,
  3: TDSL1_NO_CHANGE,
  4: TDSL1_INVALID_RPT_MODE
  */
  uint8  rpt_mode:3;

  uint8 reserved:5;
	//8bits
  union
  {
    /*! \brief Event Triggered Measurement reporting criteria as in 10.3.7.30
       Meaningful if "rpt_mode" is TDSL1_EVENT_TRIGGER_RPT
    */
    tdssrchlog_inter_sys_report_crit_struct_type    evt_rpt_crit;
		//776bits

    /*! \brief Periodical Measurement reporting criteria as in 10.3.7.53
    Meaningful if "rpt_mode" is TDSL1_PERIODIC_RPT
    */
    tdssrchlog_per_rpt_crit_struct_type periodic_crit;
		//32bits
  } u;
	//776bits
}tdssrchlog_inter_sys_reporting_criteria_struct_type;

/*! \brief
   tdssrchlog_inter_sys_meas_ctrl_struct_type
   total: 3744bits=468byts
 */
typedef PACK(struct)
{
	/*! \brief flag to indicate if cell info is present
	  If true, cell_list is valid*/
	uint8 cell_list_incl:1;
	/*! \brief flag to indicate if meas quantity is present
	  If true, meas_quan is valid */ 
	uint8 meas_quan_incl:1;
  /*! \brief flag indicating if the reporting quantities are given. */
	uint8 rpt_quan_incl:1;
	/*! \brief flag to indicate if reporting cell status is given
      If true, rpt_cell_status is valid*/
	uint8 rpt_cell_status_incl:1;
	/*! \brief Reserve*/
	uint8 :4;

	//8bits
	
	/*! \brief cell information 10.3.7.23 */
  tdssrchlog_inter_sys_cell_list_struct_type  cell_list;
	//2848bits
	
  /*! \brief Intra-frequency measurement quantity as in 10.3.7.29 of TS25.331 */
  tdssrchlog_inter_sys_meas_quan_struct_type  meas_quan;
	//56bits
	
  /*! \brief Rpt quantity for Inter-RAT set as defined in 10.3.7.32 of TS25.331 */
  tdssrchlog_inter_sys_dch_rpt_quan_struct_type   inter_sys_quan;
	//16bits
	
	/*! \brief Reporting cell status as in 10.3.7.61
	 Optional field if reporting criteria is "Periodic reporting" or "No report"
	 otherwise not needed*/
	tdssrchlog_meas_rpt_cell_status_struct_type  rpt_cell_status;
	//32bits
	
	/*! \brief reporting criteria, periodic, event, and parameters */
	tdssrchlog_inter_sys_reporting_criteria_struct_type  rpt_criteria;
	//784bits
}tdssrchlog_inter_sys_meas_ctrl_struct_type;

//#ifdef FEATURE_TDSCDMA_TO_LTE
/*! \brief tdssrchlog_eutra_frequency_info_type
	total: 16+8+256=280bits
*/
typedef PACK(struct)
{
  /*! \brief EARFCN of the downlink carrier frequency
  ** Range: (0..65535)
  */
  uint16 EARFCN;  /*change from uint32 to uint16*/
	//16bits
	
  /*! \brief See tdsl1_eutra_meas_bandwidth_type comments
	0 TDSL1_MBW6,
  1 TDSL1_MBW15,
  2 TDSL1_MBW25,
  3 TDSL1_MBW50,
  4 TDSL1_MBW75,
  5 TDSL1_MBW100,
  6 TDSL1_MBW_INVALID
  */
  uint8 meas_band_width:3;
	
  /*! \brief Number of blacklisted cell for that LTE EARFCN */
  uint8 num_blacklisted_cells:4;
	
  /*! \brief reserved*/
  uint8 reserved:1;
	//8bits
	
  /*! \brief List of cell ids that are blacklised from LTE EARFCN */
  uint16 blacklisted_cells[TDSSRCHLOG_L1_MAX_EUTRA_CELLS_PER_FREQUENCY];
	//16*16=256bits
}tdssrchlog_eutra_frequency_info_type;

/*! \brief tdssrchlog_meas_eutra_frequency_list_type
	total:2248bits
*/
typedef PACK(struct)
{
	/*! \brief Number of LTE frequencies recieved from RRC */
  uint8 num_eutra_frequencies;
	//8bits
	
 	/*! \brief List of LTE frequencies, look at tdsl1_eutra_frequency_info_type for more details */
  tdssrchlog_eutra_frequency_info_type eutra_frequency[TDSSRCHLOG_L1_MAX_EUTRA_FREQ];
	//8*280bits
}tdssrchlog_meas_eutra_frequency_list_type;

/*! \brief tdssrchlog_inter_sys_eutra_freq_list_struct_type
	total: 8+128+2248=2384bits=298 bytes
*/
typedef PACK(struct)
{
	uint8       num_freq_rmv;
	//8bits
	
  /*! \brief Number of EUTRA freqs removed 0=none, 1..L1_MAX_EUTRA_FREQ
  L1_REMOVE_ALL_FREQ(=0xFF)=remove all */
  uint16  rmv_freq[ TDSSRCHLOG_L1_MAX_EUTRA_FREQ ];
	//16*8=128bits

	/*! \brief add frequency list*/
  tdssrchlog_meas_eutra_frequency_list_type add_freq_list;
	//2248bits
}tdssrchlog_inter_sys_eutra_freq_list_struct_type;

/*! \brief tdssrchlog_inter_sys_eutra_meas_ctrl_struct_type
	total:3280bits=410bytes
*/
typedef PACK(struct)
{
	/*! \brief  E-UTRA freq list info is included or not*/
	uint8 eutra_freq_list_incl:1;
	/*! \brief  E-UTRA measurement quantity is included or not*/
	uint8 eutra_meas_quantity_incl:1;
	/*! \brief E-UTRA measurement report quantity is included or not*/ 
	uint8 eutra_rpt_quantity_incl:1;
	/*! \brief report cell status is included or not*/
	uint8 rpt_cell_status_incl:1;
	//8bits
	/*! \brief reserved*/
	uint8 padding:4;

	/*! \brief  E-UTRA freq list info*/ 
  tdssrchlog_inter_sys_eutra_freq_list_struct_type eutra_freq_list;
	//2384bits
	
	/*! \brief  E-UTRA measurement quantity*/
  tdssrchlog_inter_sys_meas_quan_struct_type  meas_quan;
	//56bits

	/*! \brief E-UTRA measurement report quantity*/
  tdssrchlog_inter_sys_dch_rpt_quan_struct_type rpt_quan;
	//16bits

	/*! \brief report cell status*/
  tdssrchlog_meas_rpt_cell_status_struct_type  rpt_cell_status;
	//32bits

	/*! \brief report criteria*/
  tdssrchlog_inter_sys_reporting_criteria_struct_type  rpt_criteria;
	//784bits
}tdssrchlog_inter_sys_eutra_meas_ctrl_struct_type;


/*! \brief --------------------------------------------------------------------------
LOG PACKET: TDSCDMA_INTER_SYS_EUTRA_MEASUREMENT_CONTROL_LOG_PACKET 
(LOG_CODE 0xD066)

This packet contains the inter-RAT EUTRA measurement configuration parameters. 
The packet is generated when the inter-RAT EUTRA measurement is initiated, 
modified or released.  

tdssrchlog_inter_sys_eutra_meas_config_pkt_type


--------------------------------------------------------------------------*/
/*! \brief tdssrchlog_inter_sys_eutra_meas_config_pkt_type
*total:3856bits=482bytes
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint32  version:8;

	/*! \brief Measurement Command
  0 - TDSL1_MEAS_SETUP,
  1 - TDSL1_MEAS_MODIFY,
  2 - TDSL1_MEAS_RELEASE
  */

  uint32  meas_cmd:2;

	/*! \brief  A boolean to check if the following info is included   
       It is mandatory if the meas_cmd is "setup", optional 
       if it is "modify" and not needed if it is "release"  
  */
  uint32  meas_object_info_included:1;

	/*! \brief Number of additional measurements */
  uint32  num_add_meas:5;

	/*! \brief type of measurement, valid only when meas_object_info_included is TRUE 
  0 - TDSL1_MEAS_TYPE_INVALID,
  1 - TDSL1_INTRA_FREQ,
  2 - TDSL1_INTER_FREQ,
  3 - TDSL1_INTER_SYS,
  4 - TDSL1_LCS,
  5 - TDSL1_TRAF_VOL,
  6 - TDSL1_QUALITY,
  7 - TDSL1_UE_INTERNAL,
  8-  TDSL1_INTER_SYS_EUTRA,
  others--INVALID
  */
  uint32  meas_type:8;

	/*! \brief reserved*/
  uint32 reseved:8;

	//32bits
	
	/*! \brief If trans_id is TDSL1_NO_CNF_TRANSACTION_ID, RRC doesn't need a confirmation for the MCM sent */
  uint32  trans_id:16;
    
  /*! \brief Measurement Identity Number */
  uint32  meas_id:16;

	/*! \brief reserved*/
	//uint32 :0;
  //32 bits

  /*! \brief Measurement Identity for each addl measurement, actual number is determined by num_add_meas */
  uint16  add_meas[ 32 ];

	//32*16=512bits
	
  /*! \brief inter system eutra measurmenet control*/
	tdssrchlog_inter_sys_eutra_meas_ctrl_struct_type inter_s_eutra;
	//3280bits
}tdssrchlog_inter_sys_eutra_meas_config_pkt_type;

//#endif /*FEATURE_TDSCDMA_TO_LTE*/


//#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) && defined(FEATURE_TDSCDMA_DMO_SUPPORT)
/*! \brief tdssrchlog_occasion_pattern_seq_para_type
*	total:32bits
*/
typedef PACK(struct)
{
  /*! \brief CELL_DCH measurement occasion cycle length coefficient. 1~9 */
  uint8                              k;

  /*! \brief In frames. The measurement occasion position in the measurement period. 0~511 */
  uint16                             Offset;

  /*! \brief The measurement occasion length in frames starting from the Offset. 1~512 */
  uint16                             m_length;

  /*! \brief Bitmap indicating which of the timeslot(s) is/are allocated for measurement. Bit string (7)
  * Bit 0 is the first/leftmost bit of the bit string. */
  uint8                              timeslot_bitmap:7;

  uint8                              padding:1;
}tdssrchlog_occasion_pattern_seq_para_type;

/*! \brief
tdssrchlog_dmo_config_struct
16+8+32=56bits
*/
typedef PACK(struct)
{
  /*! \brief SFN when new configuration is received (to activate or to de-activate)*/
  uint16 config_sfn;
	//16bits
		
	/*! \brief patternIdentifier INTEGER(0..maxMeasOccasionPattern-1), 0~4 */
  uint16   pattern_sequence_identifier:3;
	
  /*! \brief 
   0 activate, 
   1 deactivate 
  */
	uint16 status_flag:1;

	/*! \brief
	 Flag to determine whether following items are included 
	*/
  uint16  measurement_purpose_incl:1;

  /*! \brief measurementPurpose BIT STRING (SIZE (5)) OPTIONAL
    Bit 0 is for Inter-frequency measurement.
    Bit 1 is for GSM carrier RSSI measurement.
    Bit 2 is for Initial BSIC identification.
    Bit 3 is for BSIC reconfirmation.
    Bit 4 is for E-UTRA measurement.
    The value 1 of a bit means that the measurement occasion pattern 
    sequence is applicable for the corresponding type of measurement.
    Bit 0 is the first/leftmost bit of the bit string. */
  uint16    measurement_purpose:5;
	
  /*! \brief measurementOccasionPatternParameter MeasurementOccasionPatternParameter OPTIONAL */
  uint16  occasion_pattern_seq_para_incl:1;

	/*! \brief reserved*/
     uint16 reserved:5;
	//16bits

	/*! \brief DMO parameters*/
  tdssrchlog_occasion_pattern_seq_para_type  occasion_pattern_seq_para;
	//32bits
}tdssrchlog_dmo_config_struct;


/*! \brief--------------------------------------------------------------------------
LOG PACKET:TDSCDMA_INTER_SYS_EUTRA_IDLE_INTERVAL_CFG_LOG_PACKET
(LOG_CODE 0xD067)

This log packet is generated whenever there is a new configuration related 
to idle-interval configuration 

total:8+8+32+16=64bits=8bytes

--------------------------------------------------------------------------*/
/*! \brief tdssrchlog_inter_sys_eutra_idle_interval_config_pkt_type
total:8+8+32+16=64bits=8bytes
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version:8;
	//8bits

	/*! \brief Whether Idle Interval is configured or not*/
	uint8 is_configed:1; 
	
	/*! \brief the new config is still pending or not*/
	uint8  config_pending:1;  
	//8bits
	
	/*! \brief reserved*/
	uint8 reserved:6;
	
	/*! \brief Associated measurement ID for this II*/
	uint16	meas_id;
	
	/*! \brief SFN when new config is received (to set up or release)
	*/
	uint16	config_sfn; 		
	//32bits
	
	/*! \brief the coefficient parameter to calculate the idle interval period.
	* Default value is 2. The actual idle interval period equal to 2^k radio frames. 
	*/
  uint8   k;
	/*! \brief The idle interval position in the period.Default value is 0. */
  uint8   Offset;
	//16bits
}tdssrchlog_inter_sys_eutra_idle_interval_config_pkt_type;

/*! \brief --------------------------------------------------------------------------
LOG PACKET:TDSCDMA_INTER_SYS_EUTRA_DMO_CFG_LOG_PACKET 
(LOG_CODE 0xD068)

This log packet is generated whenever there is a new configuration related 
to idle-interval configuration 

tdssrchlog_inter_sys_eutra_dmo_config_pkt_type

total:328bits
-----------------------------------------------------------------------------------*/
/*! \brief tdssrchlog_t2l_ota_gap_config
* total:328bits
*/
typedef PACK(struct)
{
	 /*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
	//8bits
	 /*! \brief dmo configuration information*/
	tdssrchlog_dmo_config_struct dmo_config[TDSSRCHLOG_MAX_OCCASION_PATTERN];
	//5*64=320bits
}tdssrchlog_inter_sys_eutra_dmo_config_pkt_type;

/*! \brief
	tdssrchlog_t2l_ota_gap_config
	total:56bits
*/
typedef PACK(struct)
{
	/*! \brief gap length*/
	uint8 gap_length;
  /*! \brief gap subframe start */
  uint16 gap_subfm_start;
  /*! \brief gap subframe end */
  uint16 gap_subfm_end;
  /*! \brief gap time slot start */
  uint8 gap_ts_start;
  /*! \brief gap time slot end */
  uint8 gap_ts_end;
}tdssrchlog_t2l_ota_gap_config;

/*! \brief tdssrchlog_t2l_wall_time_gap_config.
*	total:64bits
*/
typedef PACK(struct)
{
	/*! \brief tdscdma subframe start */
	uint16 start_frame;
	/*! \brief tdscdma subframe start offset: in chipX8*/
	uint16 start_offset;
	/*! \brief tdscdma subframe end */
	uint16 end_frame;
	/*! \brief tdscdma subframe end offset: in chipX8 */
	uint16 end_offset;
}tdssrchlog_t2l_wall_time_gap_config;


/*! \brief--------------------------------------------------------------------------
LOG PACKET:TDSCDMA_T2L_TFW_GAP_CFG_LOG_PACKET  
(LOG_CODE 0xD069)

This log packet describes the GAP configuration to TFW aimed to do the LTE SRCH 
and MEAS during this GAP. In this log packet, two parts is included. First part 
is OTA Gap described by use of OTA frame and Slots. The second part is Wall Time 
Gap described by use of wall time.

tdssrchlog_t2l_tfw_gap_config_pkt_type

total:136bits
--------------------------------------------------------------------------*/
/*! \brief tdssrchlog_t2l_tfw_gap_config_pkt_type
total:136bits
*/
typedef PACK(struct)
{
	/*! \brief  Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint16  version:8;
	/*! \brief 
	 0: Small-1, AGC
	 1: Small-2, AGC+Search
	 2: Small-3, AGC+Search in Vcell mode Gap
	 3: Small-4, AGC+Search+Meas
	 4: Small-5, AGC on F1 + Meas on F2
	 5: Medium-1, AGC+Search
	 6: Medium-2, AGC+Search+Meas
	 7: Large, AGC+Search+ Meas
	 8: Invalid
	 */
	uint16 srch_type:4;
	/*! \brief 
	0: SRCH_PERIODICITY, 
  1:MEAS_PERIODICITY 
  */
	uint16 sched_mode:1;
	/*! \brief Reserved*/
	uint16 reserved:3;
	//16bits

	/*! \brief OTA GAP configuration*/
	tdssrchlog_t2l_ota_gap_config ota_gap;
	//56bits

	/*! \brief wall time gap configuration*/
	tdssrchlog_t2l_wall_time_gap_config wall_time_gap;
	//64bits
}tdssrchlog_t2l_tfw_gap_config_pkt_type;

/*! \brief
	tdssrchlog_t2l_eutra_detec_result
	total:224bits
*/
typedef PACK(struct)
{
	/*! \brief cell physical id*/
	uint16 cell_id;
	//8bits
	
	/*! \brief
  0:LTE_L1_CP_MODE_NORMAL = 0,

  1:LTE_L1_CP_MODE_EXTENDED,

  2: LTE_L1_CP_MODE_EXTENDED_MBSFN,
  */
	uint8 cp_type;
	//8bits

	/*! \brief timing offset*/
	uint64 timing_offset[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//64*2=128bits
	
	/*! \brief timing adjustment cir*/
	uint16 timing_adjustment_cir[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//16*2=32bits
	
	/*! \brief frequency offset estimate*/
	int16 freq_offset_est;
	//16bits
	
	/*! \brief SSS correlation*/
	uint32 sss_corr;
	//32bits
}tdssrchlog_t2l_eutra_detec_result;

/*! \brief
	tdssrchlog_t2l_eutra_meas_result
	total:472bits
*/
typedef PACK(struct)
{
	/*! \brief cell physical id*/
	uint16 cell_id;
	//8bits
	
	/*! \brief
  0:LTE_L1_CP_MODE_NORMAL = 0,

  1:LTE_L1_CP_MODE_EXTENDED,

  2: LTE_L1_CP_MODE_EXTENDED_MBSFN,
  */
	uint8 cp_type;
	//8bits

	/*! \brief cer_snr*/
	int32 cer_snr[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//32*2=64bits

	/*! \brief rsrp of each antennas*/
	int32 rsrp[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//32*2=64bits

	/*! \brief rsrq of each antennas*/
	int32 rsrq[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//32*2=64bits

	/*! \brief rssi of each antennas*/
	int32 rssi[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//32*2=64bits

	/*! \brief timing offset of each antennas*/
	uint64 timing_offset[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//64*2=128bits

	/*! \brief timing adjustment of each antennas*/
	uint16 timing_adjustment_cir[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//16*2=32bits

	/*! \brief frequency offset estimate*/
	int16 freq_offset_est;
	//16bits

	/*! \brief SSS correlation*/
	uint32 sss_corr;
	//32bits
}tdssrchlog_t2l_eutra_meas_result;


/*! \brief--------------------------------------------------------------------------
LOG PACKET:TDSCDMA_T2L_EUTRA_MEASUREMENT_RESULT_LOG_PACKET   
(LOG_CODE 0xD06B)

This log packet record the LTE SRCH/MEAS result from LTE ML1. 
This measurement result is corresponding to each TIME_SRCH_MREAS_REQ 
configuration

tdssrchlog_t2l_eutra_measurement_result_pkt_type

total:17560bits=2195bytes
--------------------------------------------------------------------------*/
/*! \brief tdssrchlog_t2l_eutra_measurement_result_pkt_type
*total:17560bits=2195bytes
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
	//8bits
	/*! \brief earfcn for AGC or cell detection in this gap. */
	uint16 earfcn;  /*change from uint32 to uint16*/

	/*!brief EARFCN which is measured in this gap*/
	uint16 measured_earfcn; /*change from uint32 to uint16*/
	//16bits
	/*! \brief detected cells number*/
	uint8 num_detect_cells;
	//8bits
	/*! \brief measured cells number*/
	uint8 num_meas_cells;
	//8bits
	
	/*! \brief detected cell result*/
	tdssrchlog_t2l_eutra_detec_result detect_cells[TDSSRCHLOG_LTE_CPHY_IMEAS_MAX_DETECTED_CELLS];
	//224*15=3360bits
	
	/*! \brief measured cell result*/
	tdssrchlog_t2l_eutra_meas_result meas_cells[TDSSRCHLOG_LTE_CPHY_IMEAS_MAX_MEAS_CELLS];
	//472bits*30=14160bits
}tdssrchlog_t2l_eutra_measurement_result_pkt_type;


/*! \brief--------------------------------------------------------------------------
LOG PACKET:TDSCDMA_T2L_TIME_MEAS_SRCH_REQ_LOG_PACKET   
(LOG_CODE 0xD06A)

This log packet is generated when TL1 send TIMED_SRCH_MEAS_REQ to LTE ML1 
aimed to record the context of the parameters.

tdssrchlog_t2l_time_meas_srch_req_pkt_type

total:520 bits
--------------------------------------------------------------------------*/
/*! \brief tdssrchlog_t2l_time_meas_srch_req_pkt_type
total:520 bits
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
	//8bits
	
	/*! \brief measurement earfcn*/
	uint16 earfcn;  /*change from uint32 to uint16*/
	//16bits
	
	/*! \brief
	0: dl_banwidth field is invalid
	1: dl_banwidth field is valid
	*/
	uint8 dl_bandwith_present:1;
	/*! \brief
  6: LTE_BW_NRB_6, 
  15: LTE_BW_NRB_15,
  25: LTE_BW_NRB_25,
  50: LTE_BW_NRB_50,
  75: LTE_BW_NRB_75,
  100: LTE_BW_NRB_100,
  */
	uint8 dl_bandwidth:7;
	
	/*! \brief perform search or not*/
	uint8 perform_search:1;
	
	/*! \brief whether measure newly detected cells*/
	uint8 measure_newly_detected_cells:1;
	
	/*! \brief virtual cell information present or not*/
	uint8 vcell_info_present:1;
	
	/*! \brief reserved*/
	uint8 reserved:5;
	
	//8bits
	
	/*!brief EARFCN which is measured in this gap. 
	Depending on the gap type, this EARFCN could be different from the EARFCN for AGC. */
	uint16 measured_earfcn; /*change from uint32 to uint16*/
	
	/*! \brief cells number*/
	uint8 num_cells;
	//8bits

	/*! \brief measurement information for each cell*/
	uint16 cells[TDSSRCHLOG_LTE_CPHY_IMEAS_MAX_MEAS_CELLS];
	//16*30=480
}tdssrchlog_t2l_time_meas_srch_req_pkt_type;

/*! \brief
	tdssrchlog_t2l_eutra_cell_accumulate_meas_result
	total:136 bits
*/
typedef PACK(struct)
{
	/*! \brief lte earfcn*/
	uint16 earfcn;  /*change from uint32 to uint16*/
	
	/*! \brief cell pysical id*/
	uint16 cell_id;
	//32bits
	
	/*! \brief
  0: TDSSRCHLTEDRV_CP_MODE_NORMAL,
  1: TDSSRCHLTEDRV_CP_MODE_EXTENDED,
  2: TDSSRCHLTEDRV_CP_MODE_EXTENDED_MBSFN,
  3: TDSSRCHLTEDRV_MAX_NUM_CP_MODES
  */
	uint8 cp_type:2;
	
	/*! \brief
	0: meas_cell;
	1: detected_cell;
	*/
	uint8 cell_type:1;

	/*! \brief current detected or not*/
	uint8 current_detect:1;
	
	/*! \brief
	0: non_bl_cell;
	1: bl_cell;
	*/
	uint8 bl_status:1;
	
	/*! \brief reserved*/
	uint8 reserved:3;
	
	//8bits
	
		/*! \brief spec range : -140 dBm to -44 dBm with 1 dB resolution*/
 	int16 filt_rsrp;
	//16bits
	
	/*! \brief spec range : -19.5 dB to -3 with 0.5 dB resolution */
 	int16 filt_rsrq;
	//16bits
	
	/*! \brief filtered cer snr*/
 	uint32 filt_cer_snr[TDSSRCHLOG_LTE_CPHY_IMEAS_NUM_RX_ANTENNAS];
	//32*2=64bits
}tdssrchlog_t2l_eutra_cell_accumulate_meas_result;

/*! \brief --------------------------------------------------------------------------
LOG PACKET:TDSCDMA_T2L_EUTRA_MEASUREMENT_ACCUMULATE_RESULT_LOG_PACKET    
(LOG_CODE 0xD06C)

The log packet is generated per 480ms in order to give us the full scene 
of all LTE cells with filtered measurement result.

tdssrchlog_t2l_eutra_measurement_accumulate_result_pkt_type

total:17424bits=2178bytes
------------------------------------------------------------------------------------*/
/*! \brief tdssrchlog_t2l_eutra_measurement_accumulate_result_pkt_type
total: 17424bits=2178bytes
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
	//8bits

	/*! \brief cell number*/
	uint8 num_cells;
	//8bits

	/*! \brief acummulated resulte for each cell*/
	tdssrchlog_t2l_eutra_cell_accumulate_meas_result cells[TDSSRCHLOG_L1_MAX_EUTRA_FREQ*TDSSRCHLOG_L1_MAX_EUTRA_CELLS_PER_FREQUENCY];
	//136bits*8*16=17408bits
}tdssrchlog_t2l_eutra_measurement_accumulate_result_pkt_type;

//#endif /*FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT && FEATURE_TDSCDMA_DMO_SUPPORT*/

/*! \brief
    Measurement Control Message
 */
typedef  PACK(struct)
{
  /*! If trans_id is TDSL1_NO_CNF_TRANSACTION_ID, RRC doesn't need a confirmation for the MCM sent */
  uint32  trans_id;
  //32 bits
    
  /*! Measurement Identity Number */
  uint16  meas_id;
    
  /*! Measurement Command
  0 - TDSL1_MEAS_SETUP,
  1 - TDSL1_MEAS_MODIFY,
  2 - TDSL1_MEAS_RELEASE
  */
  uint16  meas_cmd:2;
  /*!  A boolean to check if the following info is included   
       It is mandatory if the meas_cmd is "setup", optional 
       if it is "modify" and not needed if it is "release"  
  */
  uint16  meas_object_info_included:1;
  /*! Number of additional measurements */
  uint16  num_add_meas:5;
  /*! type of measurement, valid only when meas_object_info_included is TRUE 
  0 - TDSL1_MEAS_TYPE_INVALID,
  1 - TDSL1_INTRA_FREQ,
  2 - TDSL1_INTER_FREQ,
  3 - TDSL1_INTER_SYS,
  4 - TDSL1_LCS,
  5 - TDSL1_TRAF_VOL,
  6 - TDSL1_QUALITY,
  7 - TDSL1_UE_INTERNAL,
  */
  uint16  meas_type:8;
  //32 bits

  /*! Measurement Identity for each addl measurement, actual number is determined by num_add_meas */
  uint16  add_meas[ 32 ];

  /*! Measurement object, present when meas_object_info_included is TRUE, union type determined by meas_type  */  
  union
  {
    tdssrchlog_intra_freq_meas_ctrl_struct_type  intra_f;
    //tdsl1_inter_freq_meas_ctrl_struct_type  inter_f;
    //tdsl1_inter_sys_meas_ctrl_struct_type   inter_s;
  } meas_object;


} tdssrchlog_meas_ctrl_parm_struct_type;

/*! \brief TDSCDMA_MEASUREMENT_CFG_LOG_PACKET   
  (LOG CODE: 0xD00D)

This packet contains the intra-frequency measurement configuration parameters. 
The packet is generated when the intra-frequency measurement is initiated or 
modified.   
   
The worst case packet size for this log is 1704 bytes.
*/

typedef  PACK(struct)
{

  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint8  version:8;
  /*! \brief Reserved */
  uint8  reserved:8;
  /*! \brief Searcher State:
  0 - TDSSRCH_IDLE_STATE,   
  1 - TDSSRCH_FS_STATE,     
  2 - TDSSRCH_ACQ_STATE,    
  3 - TDSSRCH_BCH_STATE,    
  4 - TDSSRCH_FACH_STATE, 
  5 - TDSSRCH_DCH_STATE, 
  6 - TDSSRCH_DRX_STATE, 
  7 - TDSSRCH_DZ_STATE,  
  8 - TDSSRCH_BPLMN_STATE,
	9 - TDSSRCH_SYNC_STATE,	*/
  uint8 srchState;
  /*! \brief Measurement Info Type:
      0 - SIB; 
      1 - MCM; 
      2 - STATE CHANGE; 
      3 - INTERF SIB; 
      4 - PRIORITY CELL LIST*/
  uint8 measParmType;
  //32 bits

  /*! \brief meas configuration parameter, type is determined by measParmType */
  union
  {
    tdssrchlog_meas_sib_parm_struct_type sib;
  
    tdssrchlog_meas_ctrl_parm_struct_type ctrl;
  
    /* This cmd comes to L1 only when RRC transitions from FACH<-->DCH */
    //tdssrchlog_meas_trans_type trans_meas;
  
    //tdssrchlog_inter_freq_cell_list_struct_type inter_freq_sib;
  
  } u;

}tdssrchlog_meas_config_pkt_type;
   
/*
	tdssrchlog_inter_sys_meas_config_pkt_type
	total: 4320bits=540byts
*/
typedef PACK(struct)
{
	/*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint32  version:8;

	/*! \brief Measurement Command
  0 - TDSL1_MEAS_SETUP,
  1 - TDSL1_MEAS_MODIFY,
  2 - TDSL1_MEAS_RELEASE
  */

  uint32  meas_cmd:2;

	/*! \brief  A boolean to check if the following info is included   
       It is mandatory if the meas_cmd is "setup", optional 
       if it is "modify" and not needed if it is "release"  
  */
  uint32  meas_object_info_included:1;

	/*! Number of additional measurements */
  uint32  num_add_meas:5;

	/*! \brief type of measurement, valid only when meas_object_info_included is TRUE 
  0 - TDSL1_MEAS_TYPE_INVALID,
  1 - TDSL1_INTRA_FREQ,
  2 - TDSL1_INTER_FREQ,
  3 - TDSL1_INTER_SYS,
  4 - TDSL1_LCS,
  5 - TDSL1_TRAF_VOL,
  6 - TDSL1_QUALITY,
  7 - TDSL1_UE_INTERNAL,
  */
  uint32  meas_type:8;

	/*reserved*/
	uint32 :8;

	//32bits
	
	/*! \brief If trans_id is TDSL1_NO_CNF_TRANSACTION_ID, RRC doesn't need a confirmation for the MCM sent */
  uint32  trans_id:16;
    
  /*! \brief Measurement Identity Number */
  uint32  meas_id:16;

  //32 bits

  /*! \brief Measurement Identity for each addl measurement, actual number is determined by num_add_meas */
  uint16  add_meas[ 32 ];

	//32*16=512bits

	/*! \brief inter system measurement control*/
	tdssrchlog_inter_sys_meas_ctrl_struct_type inter_s;
	//3744bits
}tdssrchlog_inter_sys_meas_config_pkt_type;

/*! \brief Serving cell frequency and priority information in SIB19. */
typedef PACK(struct)
{
  /*! \brief Range 0 to 15. Values 0 to 7 is for NW assigned priority, 
    values 8 to 14 invalid, value 15 when serving cell frequency has no priority. */
  int8 priority;          

  /*! \brief Serving cell UARFCN */
  uint16 UARFCN;  

  /*! \brief priority search threshold. Range: 2*(0 to 31) */
  uint8 s_prioritysearch1; 

   /*! \brief Range: (0 to 31)*/
  uint8 thresh_servinglow; 
  //40 bits
} tdssrchlog_serving_cell_priority_info_type;


/*! \brief Struct to store the inter-f priority info received from SIB19/3rd MEAS_REQ */
typedef PACK(struct)
{

/*! \brief TDS-CDMA Inter Frequency UARFCN */
  uint16 UARFCN;

  /*! \brief Range: (0 to 7) */
  int8 priority;     

  /*! \brief Range: 2*(0 to 31)*/
  uint8 threshx_high;

  /*! \brief Range: 2*(0 to 31)*/
  uint8 threshx_low; 

  /*! \brief Range: 2*(-60 to -13)+1, Default : -119 */
  int32 q_rxlevmin_tdd;
  //72 bits
} tdssrchlog_inter_freq_priority_info_type;

/*! \brief The GSM priority info received from SIB19/3rd MEAS_REQ */
typedef PACK(struct)
{
    /*! \brief GSM ARFCN */
  uint16 ARFCN;
   /*! \brief Band indicator 
     0 -- TDSL1_DCS_1800_BAND
     1 -- TDSL1_PCS_1900_BAND
     */
  uint8 band;

  /*! \brief Range: (0 to 7) */
  uint8 priority;       

  /*! \brief Range: 2*(-58 to -13)+1 */
  int16 q_rxlevmin_gsm; 

  /*! \brief Range: 2*(0 to 31)*/
  uint8 threshx_high;

  /*! \brief Range: 2*(0 to 31)*/
  uint8 threshx_low;  

} tdssrchlog_inter_rat_gsm_priority_info_type;

/*! \brief LTE priority info per EARFCN received from SIB19/3rd MEAS_REQ */
typedef PACK(struct)
{
  /*! \brief EARFCN of the downlink carrier frequency Range: (0..65535)*/
  uint16 EARFCN;  /*change from uint32 to uint16*/
  
  /*! \brief Priority of the LTE freq, Range: (0..7) */
  int8 priority;
  
  /*!\brief Threshold to be used when calculating suitability criteria for that LTE EARFCN 
             Range: 2*(-70 to -22) 
  */
  int16 q_rxlevmin_eutra; 
  
  /*! \brief Max threshold value RSRP, dB. Range: 2*(0 to 31) */
  uint8 threshx_high; 
  
  /*! \brief Min threshold value RSRP, Range: 2*(0 to 31) dB */
  uint8 threshx_low;  
  
  /*! \brief Threshold to be used when calculating suitability criteria for that LTE EARFCN.  
   Per spec RSRQ, dB default value is negative infinity, L1 should treat 0xFF as default */
  int16 q_qualmin_eutra; 
  /*! \brief whether threhold_high2 is included or not. 
    0 -- not included
    1 -- included
    */
  uint8 high2_incl;    
  /*! \brief threhold_high2 */
  uint8 threshx_high2;   
  /*! \brief whether threhold_low2 is included or not. 
    0 -- not included
    1 -- included
    */
  uint8 low2_incl;
  /*! \brief threhold_low2 */
  uint8 threshx_low2;  
} tdssrchlog_inter_rat_eutra_priority_info_type;

/*! \brief TDSCDMA_SIB19_MEASUREMENT_CFG_LOG_PACKET   
  (LOG CODE: 0xD059)

This packet contains the SIB19 measurement configuration parameters. 
The packet is generated when SIB19 is configured or modified.   
   
The packet size for this log is 446 bytes.
*/

typedef  PACK(struct)
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint8  version:8;  
  /*! \brief Searcher State:
  0 - TDSSRCH_IDLE_STATE,   
  1 - TDSSRCH_FS_STATE,     
  2 - TDSSRCH_ACQ_STATE,    
  3 - TDSSRCH_BCH_STATE,    
  4 - TDSSRCH_FACH_STATE, 
  5 - TDSSRCH_DCH_STATE, 
  6 - TDSSRCH_DRX_STATE, 
  7 - TDSSRCH_DZ_STATE,  
  8 - TDSSRCH_BPLMN_STATE,
	9 - TDSSRCH_SYNC_STATE,	*/
  uint8 srchState;
  /*! \brief Reserved */
  uint16  reserved:16;  
  //32 bits

  /*! \brief Number of TD-SCDMA Inter Frequencies  */
  uint8 num_inter_freq_priorities;
  /*! \brief Number of GSM cells  */
  uint8 num_gsm_priorities;
  /*! \brief Number of EUTRA frequencies */
  uint8 num_eutra_priorities;
  /*! \brief Padding for word alignments */
  uint8 reserved2;
  //32 bits

  /*! \brief Serving cell frequency and priority information  */
  tdssrchlog_serving_cell_priority_info_type serving_cell_info;
  
  /*! \brief Priority Information for each Inter frequency  */
  tdssrchlog_inter_freq_priority_info_type inter_freq_priority[TDSSRCHLOG_MAX_INTERF_PRIORITY_LAYERS];
  
  /*! \brief Priority information of GSM cells  */
  tdssrchlog_inter_rat_gsm_priority_info_type gsm_priority[TDSSRCHLOG_MAX_GSM_PRIORITY_LAYERS];

  /*! \brief Priority information of EUTRA frequencies */
  tdssrchlog_inter_rat_eutra_priority_info_type eutra_priority[TDSSRCHLOG_MAX_EUTRA_PRIORITY_LAYERS];

}tdssrchlog_sib19_meas_config_pkt_type;

/*! \brief Measurement result of one EUTRA cell */
typedef PACK(struct) 
{   
  /*! \brief The physical cell id of the detected cell */
  uint16 cell_id;  

  /*! \brief RSRP comb.value combined across Tx-Rx pairs. In linear scale. */
  int16 rsrp;
  /*! \brief Instant. RSRQ value combined across Tx-Rx pairs. In linear scale. */
  int16 rsrq;
  
  /*! \brief Cell selection values evaluated by SRCH and to be reported to RRC */  
  int16 s_rxlev;

  /*! \brief Cell selection values evaluated by SRCH and to be reported to RRC */  
  int16 s_qual;

} tdssrchlog_eutra_cell_meas_result_type;

typedef PACK(struct)
{
  /*! \brief EARFCN of the downlink carrier frequency Range: (0..65535)*/
  uint16 EARFCN;  /*change from uint32 to uint16*/
  
  /*! \brief Priority of the LTE freq, Range: (0..7) */
  int8 priority;

  /*! \brief Number of measured cells, Range: (0..7) */
  uint8 num_cells;
  //32 bits

  tdssrchlog_eutra_cell_meas_result_type eutra_cells[TDSSRCHLOG_MAX_LTE_CELLS_PER_FREQ];

}tdssrchlog_eutra_freq_meas_info_type;

/*! \brief TDSCDMA_MEASUREMENT_EUTRA_CELLS_LOG_PACKET   
  (LOG CODE: 0xD05A)

This packet contains priority scheduling mode, and, if any, 
the EUTRA cell measurement results organized by frequency. 
 
For idle mode, this packet is for each DRX cycle. 
 
The best case packet size for this log is 5 bytes (no EUTRA freq is measured);    
The worst case packet size for this log is 661 bytes (4 EUTRA freq are measured).
*/

typedef  PACK(struct)
{
  /* Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint8  version:8;  
  /*! \brief System frame number  */
  uint16 sfn:12; 
  /*! \brief sub frame number within 1 frame (Range: 0, 1)  */
  uint16 subframe:1;
  /*! \brief Time slot number within 1 subframe (Range: 0 to 6) 
    Default set to 0 in idle mode */
  uint16 timeslot:3;
  /*! \brief Prirority scheduling mode 
    0 -- High Priority Mode
    1 -- All Priority Mode */
  uint8  scheduling_mode;
  //32 bits

  /*! \brief Number of EUTRA frequencies which are measured (Range: 0 to 4) */
  uint8 number_eutra_freq;
  /*! \brief Measurement results of each EUTRA frequency */
  tdssrchlog_eutra_freq_meas_info_type measured_eutra_freq[TDSSRCHLOG_MAX_NUM_MEAS_LTE_FREQ];

}tdssrchlog_eutra_cell_measurement_pkt_type;

typedef PACK(struct)
{
  /*! \brief frequency of the tdscdma cell*/
  uint16    freq; 
  /*! \brief cell parameter id of the tdscdma cell*/
  uint16    cpi;
  /*! \brief Priority of the tdscdma cell (-1: unassigned)*/
  int16    priority;
  /*! \brief rank of the tdscdma cell (dBm)*/
  int16     rank;
  /*! \brief rscp of the tdscdma cell dBm [-120,25] ,mapping to rang [-5,91]*/
  int16     rscp; 
  
  /*! \brief s_rxlev of this cell*/
  int16     s_rxlev;
  /*! \brief Tresel count in unit of subframes (5ms)*/
  int16     t_resel_count; 

  /*! \brief Padding for word alignment*/
  uint16    reserved; 
  //32bits
} tdssrchlog_abs_prior_tds_cell_rank_info_type;

/*! \brief GSM CELL RANK subrecord 
 *  
 *  32 bits total
 */

typedef PACK(struct)
{ 
    /*! \brief ID of the gsm cell*/
  uint16 cell_id; 
  /*! \brief frequency of the gsm cell*/
  uint16    arfcn; 
  
  /*! \brief Priority of the gsm cell (-1: unassigned)*/
  int16    priority;

  /*! \brief bsic of the gsm cell*/
  uint16    bsic;
  /*! \brief rssi of the gsm cell dBm [-110,-48] ,mapping to rang [0,63]*16*/
  int16     rssi; 
  /*! \brief rank of the gsm cell*/
  int16     rank;
  /*! \brief f s_rxlev of the gsm cell*/
  int16 s_rxlev; 

  /*! \brief Tresel count in unit of subframes (5ms)*/
  int16     t_resel_count; 
  //32bits
} tdssrchlog_abs_prior_gsm_cell_rank_info_type;

typedef PACK(struct)
{
  /*! \brief frequency of the EUTRA cell*/
  uint16    earfcn;   /*change from uint32 to uint16*/
  /*! \brief cell  id of the EUTRA cell*/
  uint16    cell_id;
  /*! \brief Priority of the EUTRA cell */
  int16    priority;

  /*! \brief RSRP comb.value combined across Tx-Rx pairs. In linear scale. */
  int16 rsrp;
  /*! \brief Instant. RSRQ value combined across Tx-Rx pairs. In linear scale. */
  int16 rsrq;
  
  /*! \brief Cell selection values evaluated by SRCH and to be reported to RRC */  
  int16 s_rxlev;

  /*! \brief Cell selection values evaluated by SRCH and to be reported to RRC */  
  int16 s_qual;
  /*! \brief Tresel count in unit of subframes (5ms)*/
  int16     t_resel_count; 
  //32bits
} tdssrchlog_abs_prior_eutra_cell_rank_info_type;

/*! \brief    LOG PACKET: TDSCDMA_ABSOLUTE_PRIORITY_CELL_RANK_LOG_PACKET
 				 (LOG CODE: 0xD05B)

This log packet outputs the priority, rscp and rank value of 
the tdscdma cells, gsm cells and EUTRA cells.
It is generated once cells are evaluated. 

MIN Size: 16 bytes, MAX Size: 2078 bytes
*/

typedef PACK(struct)
{
  /* Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version:8;  
  /*! \brief System frame number  */
  uint16 sfn:12; 
  /*! \brief sub frame number within 1 frame (Range: 0, 1)  */
  uint16 subframe:1;
  /*! \brief Time slot number within 1 subframe (Range: 0 to 6) 
    Default set to 0 in idle mode */
  uint16 timeslot:3;
  /*! \brief UE state mode 
    0 -- IDLE DRX
    1 -- URA PCH DRX
    2 -- CELL PCH DRX
  */
  uint8  ue_state;
  //32bits
  /*! \brief Tresel threshold in use for TDS (Range 0 to 6200 subframes) */
  uint16 tresel_tds;

  /*! \brief Tresel threshold in use for GSM (Range 0 to 6200 subframes) */
  uint16 tresel_gsm;

  /*! \brief Tresel threshold in use for EUTRA (Range 0 to 6200 subframes) */
  uint16 tresel_eutra;

  /*! \brief Number of TDS cells evaluated with Tresel running (Range: 0 to 64) */
  uint16 num_tds_cell;

  /*! \brief Number of GSM cells evaluated with Tresel running (Range: 0 to 32) */
  uint16 num_gsm_cell;

  /*! \brief Number of EUTRA cells evaluated with Tresel running (Range: 0 to 32) */
  uint16 num_eutra_cell;

  /*! \brief TDS cell rank info */
  tdssrchlog_abs_prior_tds_cell_rank_info_type TdsCells[TDSSRCHLOG_TDS_MAX_CELL_NUM];
  //32*64 bits
  /*! \brief GSM cell rank info */
  tdssrchlog_abs_prior_gsm_cell_rank_info_type GsmCells[TDSSRCHLOG_GSM_MAX_CELL_NUM];
  //32*32 bits
  /*! \brief EUTRA cell rank info */
  tdssrchlog_abs_prior_eutra_cell_rank_info_type EutraCells[TDSSRCHLOG_LTE_MAX_CELL_NUM];
} tdssrchlog_absolute_priority_cell_rank_pkt_type;
   
/* Macro to determine the sizes of absolute priority cell resel rank log packet */
#define TDSSRCHLOG_ABS_PRIO_CELL_RESEL_RANK_PKT_LEN(tdscellnum,gsmcellnum, eutracellnum)  ( \
    FPOS(tdssrchlog_absolute_priority_cell_rank_pkt_type, TdsCells)+  \
    ( tdscellnum * sizeof(tdssrchlog_abs_prior_tds_cell_rank_info_type))+  \
    ( gsmcellnum * sizeof(tdssrchlog_abs_prior_gsm_cell_rank_info_type))+ \
    ( eutracellnum * sizeof(tdssrchlog_abs_prior_eutra_cell_rank_info_type)))
/*--------------------------------------------------------------------------
 	LOG PACKET: TDSCDMA_MEASUREMENT_GSM_TIMING_AND_INTERMEDIATE_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD00F)

This packet records detailed information on the GSM measurement timing and results.
It is generated after each GSM RSSI measurement, FCCH tone detection or BSIC verification.

The worst case packet size for this log is 1004 bytes. 
32+32*250 = 8032 bits
--------------------------------------------------------------------------*/


#define TDSSRCHGSM_RSSI_MEAS_LOG_INFO_ID     0
#define TDSSRCHGSM_FCCH_DETEC_LOG_INFO_ID    1
#define TDSSRCHGSM_SCH_DECODE_LOG_INFO_ID    2

typedef struct 
{
  /*! \brief  Carrier RF channel number */
  uint16 carrier_arfcn;
  
  /*! \brief  Burst start GSM QS offset */
  uint16 burst_start_qs_offset;

  /* ! \brief LNA gain range choosen */
  uint8 gsm_lna_state;

  /* ! \brief cell index number */
  uint8 cell_index;
  
  /* ! \brief RSSI dBmx16 */
  int16 rssi_dbmx16;

  /*! \brief  RSSI sample head and tail */
  uint16 sample_head;

  /* ! \brief sample timestamp */
  uint16 sample_timestamp;
} tdssrchgsm_rssi_burst_log_info_struct_type;

typedef struct
{
  /*! \brief  number of GSM RSSI scans */
  uint32 num_bursts:8;
  
  /* ! \brief is MEAS_DONE_SIG */
  uint32 meas_done_sig:1;
  
  /* ! \brief  indicate last arfcn or not */
  uint32 hit_last_arfcn:1;
  
  /* ! \brief valid arfcn number */
  uint32 num_valid_arfcn:6;

  uint32 :16;

  /*! \brief  burst infomation*/
  tdssrchgsm_rssi_burst_log_info_struct_type burst_info[TDSSRCHLOG_GSM_MAX_CELL_NUM];

} tdssrchgsm_rssi_log_info_struct_type;


typedef struct 
{
  /* ! \brief Carrier RF channel number */
  uint16 carrier_arfcn;
  
  /* ! \brief ACQ start GSM QS offset */
  uint16 acq_start_qs_offset;

  /*! \brief  is  tone  detected*/
  uint32  tone_detected:1;
  
  /*! \brief  is  sch crc pass*/
  uint32  sch_crc_pass:1;
  
  /*! \brief  is detected tone forbiddened */
  uint32  got_forbidden_tone:1;

  /*! \brief  NCC number */
  uint32  bsic_ncc:3;

  /*! \brief  BCC number */
  uint32  bsic_bcc:3;

  /*! \brief  cell measure state */
  uint32  meas_state:4;

  /*! \brief  cell state such as unused, used, deleted*/
  uint32  cell_state:3;

  /*! \brief  ACQ window start offset */
  uint32 result_start_offset :16;

  /*! \brief  GSM muti frame number if got forbidden tone */
  uint8 forbidden_tone_gfn;

  /*! \brief  frame number in muti GSM frame after adjusting */
  uint8 frame_offset;

  /*! \brief  offset in a GSM frame if got forbidden tone */
  uint16 forbidden_tone_goffset;
  
  /*! \brief  qs offset after adjusting */
  uint32 qs_offset;
  
  /*! \brief  Tone QS offset from ACQ start offset */
  int16 fcch_tone_qs_offset;
  
  /*! \brief  freq offset */
  int16 freq_offset;
  
  /*! \brief  SNR estimate of the tone */
  uint16 snr_estimate;
  
  /*! \brief  GSM QS offset to the center of the burst at which SCH is decoded */
  uint16 sch_qs_offset;
  
  /*! \brief  Decoded SCH data */
  uint32 sch_decoded_data;
  
  /*! \brief  frame and qs if got forbidden tone */
  uint32 forbidden_frame_qs[3];
} tdssrchgsm_fcch_log_info_struct_type;

#define TDSSRCHGSM_SCH_DEC_LOG_INFO_BSIC_VER_APPLICABLE_BIT    0
#define TDSSRCHGSM_SCH_DEC_LOG_INFO_BSIC_VER_BSIC_MISMATCH_BIT 1
#define TDSSRCHGSM_SCH_DEC_LOG_INFO_BSIC_VER_RFN_MISMATCH_BIT  2
#define TDSSRCHGSM_SCH_DEC_LOG_INFO_CRC_PASSED_BIT             3


typedef struct 
{
  /*! \brief  Carrier RF channel number */
  uint16 carrier_arfcn;
  
  /*! \brief  GSM frame number at which SCH decoded */
  uint16 sch_decode_gfn;
  
  /*! \brief  GSM QS offset to the center of the burst at which SCH is decoded */
  uint16 sch_burst_qs_offset;
  
  /*! \brief  SCH frame nunber 1, 11, 12, 21, 22, 31, 32, 41, 42 */
  uint8 sch_decode_frame_number;
  
  /*! \brief  indicate the BSIC verification status and CRC pass status */
  uint8 sch_dec_crc_bsic_verify_bf;

  /* ! \brief Tone QS offset from ACQ start offset */
  int16 sch_burst_time_offset_qs;

  /*! \brief  punctured UL slot */
  int8  ul_slot_punctured;
  
  /*! \brief  punctured UL slot */
  int8  crc_pass:4;

  /*! \brief  cell state such as unused, used, deleted */
  int8  cell_state:4;

  /*! \brief  NCC number */
  uint16 bsic_ncc:3;

  /*! \brief  BCC number */
  uint16 bsic_bcc:3;
  
  /*! \brief  indicate this result was meant for bsic Id */
  uint16 purpose_bsic_id:1;
  
  /*! \brief  will send signal done to L1M to call bsic verify update function */
  uint16 meas_signal_done:1;
  
  /*! \brief  cell measure state before update */
  uint16 meas_state_before:3;
  
  /*! \brief  cell measure state after update */
  uint16 meas_state_now:3;
  
  /*! \brief  is bsic match after reconf decode */
  uint16 bsic_match:2;

  /*! \brief  offset number in muti frame */
  uint16 sch_frame_offset;

  /*! \brief  Decoded SCH data */
  uint32 sch_decoded_data;

  /*! \brief  GSM  frame number */
  uint32 hyper_frame_number;

  /*! \brief  offset number  in  hyper frame */
  uint32 hyper_frame_offset;

  /*! \brief  offset in a GSM frame */
  uint32 qs_offset;
  
} tdssrchgsm_sch_dec_log_info_struct_type;


/* log packet info size */
#define TDSSRCHGSM_RSSI_MEAS_LOG_INFO_SZ  (sizeof(tdssrchgsm_rssi_log_info_struct_type))
#define TDSSRCHGSM_RSSI_BURST_LOG_INFO_SZ (sizeof(tdssrchgsm_rssi_burst_log_info_struct_type))
#define TDSSRCHGSM_FCCH_ACQ_LOG_INFO_SZ   (sizeof(tdssrchgsm_fcch_log_info_struct_type))
#define TDSSRCHGSM_SCH_DEC_LOG_INFO_SZ    (sizeof(tdssrchgsm_sch_dec_log_info_struct_type))

#define TDSSRCHGSM_MEAS_LOG_BUF_NUM (8)
#define TDSSRCHGSM_LOG_MAX_BUF_SIZE 1000
#define TDSSRCHGSM_LOG_SUBMIT_SIZE_LIMIT 800
#define TDSSRCHGSM_LOG_SUBMIT_MAX_TDS_SUBFRAME_CNT 100

typedef union
{
  tdssrchgsm_rssi_log_info_struct_type      rssi_log_info;
  tdssrchgsm_fcch_log_info_struct_type      fcch_log_info;
  tdssrchgsm_sch_dec_log_info_struct_type   sch_log_info;
  
}tdssrchlog_gsm_info_strcut_type;

typedef struct 
{

  /* ! \brief Log header */
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  
  /* ! \brief Log Information ID. */
  uint32 entity_id:8;

  /* ! \brief message ID. */
  uint32  message_id:16;
  
  /* ! \brief TDSCDMA Subframe # when GAP starts */
  uint16 gap_start_sfn;
  
  /* ! \brief TDSCDMA chipx8 # when GAP starts */
  uint16 gap_start_cx8_offset;
  
  /* ! \brief TDSCDMA Subframe # when GAP ends */
  uint16 gap_end_sfn;
  
  /* ! \brief TDSCDMA chipx8 # when GAP ends */
  uint16 gap_end_cx8_offset;
  
  /* ! \brief GSM frame number when GAP starts */
  uint16 gap_gfn;
  
  /* ! \brief GSM QS # when GAP starts */
  uint16 gap_start_qs_offset;
  
  /* ! \brief GSM gap duration in QS */
  uint32 gap_length_qs;

  /* ! \brief startup rf buffer index */
  uint32 startup_rf_buffer_index:2;
  
  /* ! \brief cleanup rf buffer index */
  uint32 cleanup_rf_buffer_index:2;
  
  /* ! \brief is GSM rsp has a valid result */
  uint32 result_valid_flag:1;
  
  /* ! \brief is searcher state */
  uint32 srch_state:11;
  
  /* ! \brief is TDSCDMA Subframe # when result received*/
  uint32 result_rcvd_sfn:16;
  
  /* ! \brief is chip offset when result received*/
  uint32 result_rcvd_offset;
  
  tdssrchlog_gsm_info_strcut_type  gsm_log_info;
}tdssrchlog_meas_gsm_results_struct_type;


#define TDSSRCHLOG_GSM_MEAS_RESULTS_PKT_LEN() (sizeof(tdssrchlog_meas_gsm_results_struct_type))

    
/*--------------------------------------------------------------------------
 	LOG PACKET: TDSCDMA_MEASUREMENT_GSM_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD00E)

This packet outputs the GSM cell measurement results. It is generated once every 100ms.  

The worst case packet size for this log is bytes. 

--------------------------------------------------------------------------*/
/*! \brief Intra-Frequency Measurement configurations 
 * 32 bits total
 */
typedef struct
{
  uint32 cellID:14;
  uint32 bandIndicator:1;
  uint32 bsicValid:1;
  uint8 ncc;
  uint8 bcc;
  //32 bits

  uint16 arfcn;
  int16 rssi;
  //32 bits
}tdssrchlog_gsm_cell_meas_result_type;

/*! \brief		 LOG PACKET: TDSCDMA_MEASUREMENT_GSM_RESULT_LOG_PACKET
 				 (LOG CODE: 0xD00E)

This packet outputs the GSM cell measurement results. It is generated once every 100ms.  

The worst case packet size for this log is bytes.
*/
typedef struct 
{

  log_hdr_type header;

  /*! Version  */
  uint32  version:8;
  uint32  reserved:16;
  uint32  numCells:8;                     
  //32 bits

  //Variable length members should be put at the end of the packet
  tdssrchlog_gsm_cell_meas_result_type gsmCellInfo[TDSSRCHLOG_GSM_MAX_ACCUMULATE_CELL_NUM];

}tdssrchlog_gsm_accumulate_meas_result_pkt_type;

/* Macro to determine the sizes of IntraF meas config log packet */
#define TDSSRCHLOG_GSM_ACCUMULATE_RESULT_PKT_LEN(config_cells) ( \
    FPOS(tdssrchlog_gsm_accumulate_meas_result_pkt_type, gsmCellInfo)+  \
    ( config_cells * sizeof(tdssrchlog_gsm_cell_meas_result_type)))      


    
/*--------------------------------------------------------------------------
 LOG PACKET: TDSCDMA_SCELL_TRACKING_STATUS_LOG_PACKET
    (LOG CODE: 0xD051)
--------------------------------------------------------------------------*/

/*! \brief TDSCDMA serving cell tracking status subrecord
 *  
 *   160 bits total. 
 */

typedef struct
{
  /*! \brief the system time in seconds  */
  uint32 sys_time;
  /*! \brief tdscdma sub-frame number */
  uint16 sub_fn;
  /*! \brief tdsl1m state */
  uint16 l1_state;

  /*! \brief xo temperature in 1/1024 centigrade degree  */
  int32  xo_temp;

  /*! \brief freq offset in 1/1024 ppm */
  int32  freq_offset;

  /*! \brief rx-time offset regarding to wall-time */
  uint16 ts0startpositon;
  
  /*! \brief reserve */
  uint16 reserve;
}tdssrchlog_scell_tracking_status_type;


/*! \brief  LOG PACKET: TDSCDMA_SCELL_TRACKING_STATUS_LOG_PACKET
    (LOG CODE: 0xD051)

This packet outputs the serving cell frequency and timing tracking status, 

It is generated once after getting 40 records (about 200ms) 

The packet size for this log is 804  bytes.
40 * 20 + 4
*/
typedef struct 
{

  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32  reserved:16;
  /*! \brief the number of tracking status record  */
  uint32  numItems:8;                     
  //32 bits

  /*! \brief content of each tracking status*/
  tdssrchlog_scell_tracking_status_type trackingstatus[TDSSRCHLOG_MAX_SCELL_TRACKING_RECORDS];
  //40 * 160 bits

}tdssrchlog_scell_tracking_status_pkt_type;

/* Macro to determine the sizes of scell tracking status log packet */
#define TDSSRCHLOG_SCELL_TRACKING_STATUS_PKT_LEN  \
        (sizeof(tdssrchlog_scell_tracking_status_pkt_type))



/*--------------------------------------------------------------------------
 LOG PACKET: TDSCDMA_TS0_CELL_UPDATE_LOG_PACKET
    (LOG CODE: 0xD052)
--------------------------------------------------------------------------*/
/*! \brief RSCP CELL INFO
 *  
 *   64 bits total. 
 */
typedef struct {
  /*! \brief Previous timing measurement result
  @verbatim 
   1) Absolute wall time offset of the subframe boundary (cx8) 
   2) If available, configured as ts0StartPosition from previous RSCP 
      response msg.
   3) If not available, configured as ts0StartPosition from the serving 
      cell.
  @endverbatim 
  */
  uint32                    wallTimeOffset;
  /*! \brief Cells midamble index */
  uint32                     midambleIdx:8;
  /*! reserved */
  uint32                     : 24;
    /*! \brief TS-RSP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int16  tsRsp;
  /*! \brief RSCP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int16  rscp;
    /*! \brief metric M, per OTD cell, for purpose of fake cell pruning 
  @verbatim
   M = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid_1stShift|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricM;
  /*! \brief metric M prime, per OTD cell, reserved 
  @verbatim   
   M' = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricMPrime;
    /*! \brief noise variance computed from DWPTS_GP in dBm (format 16Q8)*/   
  //32 bits
#if defined(FEATURE_TDSCDMA_NFTL) || defined(FEATURE_TDSCDMA_EAMM_OPT)
    uint16 slot_snr;
    uint16 filt_slot_snr;  
  //32 bits
    
    int32 freq_error;
    int32 filt_freq_error;  

    uint16 meas_gap;  
    uint16 reserved;
  //32 bits
    int32 dopplerEst;
#endif
} tdssrchlog_top_eight_rscp_cell_info_t;
/*! \brief RSCP CELL INFO
 *  
 *   64 bits total. 
 */
typedef struct {
  /*! \brief Previous timing measurement result
  @verbatim 
   1) Absolute wall time offset of the subframe boundary (cx8) 
   2) If available, configured as ts0StartPosition from previous RSCP 
      response msg.
   3) If not available, configured as ts0StartPosition from the serving 
      cell.
  @endverbatim 
  */
  uint32                     midambleIdx:8;
  /*! reserved */
  uint32                     :0;
    /*! \brief TS-RSP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int16  tsRsp;
  /*! \brief RSCP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int16  rscp;
  //32 bits
} tdssrchlog_remained_rscp_cell_info_t;

 

/*! \brief CELL ISCP REPORT
 *  
 */
typedef struct
{
  /*! \brief Cells midamble index */
  int8  midambleIdx;
  int8  reserved;
  /*! \brief ISCP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int16  iscp;
  //32 bits
}tdssrchlog_ts0_cell_iscp_report_t;



/*! \brief  LOG PACKET: TDSSRCHLOG_TS0_CELL_UPDATE_LOG_SHORTEN_PACKET
    (LOG CODE: 0xD058)

This packet contains the TS0_CELL measurment request and results. 

It is generated when ts0_cell measurment result is received. 

The packet size for this log is 588  bytes.
*/
typedef struct 
{

  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32  reserved:24;
  //32 bits
   /*! \brief Rx Subframe nubmer for unscheduled measurement */
    uint16                    subframeNum;
  /*! \brief measured frequency (Hz)
  @verbatim 
   Used by FW for Frequency Tracking Loop (FTL)
  @endverbatim 
  */
  uint16                    measuredFreq;
    //32 bits

  /*! \brief total number of cells for TS0 operation */
    uint32                    numCells : 6;      
  /*! \brief total number of cells for for iterative cancellation in TS0 
  @verbatim 
   1-4: if it is for demodualtion
   1-8: if it is for measurement 
  @endverbatim 
  */
    uint32                    numItrvCells : 4;
  /*! \brief total number of cells for for ISCP measurement in TS0
  */
    uint32                    numIscpCells: 3;

  /*! \brief Rx diversity flag 
  @verbatim
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored
  @endverbatim 
  */
  uint32                    rxDiv : 1;
  /*! \brief Rx antenna config
  @verbatim   
   0: Antenna 0
   1: Antenna 1
  @endverbatim 
  */ 
  uint32                    rxAntenna : 1;
  /*! \brief Inter freq measurement flag
  @verbatim 
   0: Intra freq
   1: Inter freq
  @endverbatim 
  */
  uint32                    interFreq : 1;
  /*! \brief Active cell primary freq flag 
  @verbatim 
   0: measured freq is not the same as primary freq of the serving cell
   1: measured freq is the same as the primary freq of the serving cell
  @endverbatim 
  */
  uint32                      activePrimaryFreq : 1;

  /*! \brief RF script buffer index for inter freq tuning
  @verbatim 
   If interFreq is 0, ignored
  @endverbatim 
  */
  uint32                    rfRxFreqBufIndex : 8;
  /*! \brief Index to RXLM static settings
  @verbatim 
   If interFreq is 0, ignored
  @endverbatim 
  */
  uint32                    rxlmStaticBufIndex :7;
  //32 bits

  //32 bits
    /*! \brief error status of current report 
  @verbatim 
   0: no error detected
   1: no  ts0_cell_cmd received
   2: serving cell config error 
   others: reserved
  @endverbatim 
  */
    uint16                    errStatus   : 3;
  //reserved
    uint16                   :   13;
    int16                     noiseVar;
	 /*! \brief RSSI on two antennas */ 
    int16                     rssi[TDSSRCHLOG_NUM_ANTENNAS];
    //32 bits
     int16                    midRssi[TDSSRCHLOG_NUM_ANTENNAS];
  /*! \brief LNA info for inter freq measurement 
  @verbatim 
   1) Used for inter freq measurement (interFreq set to 1)
   2) SW helps to remember LNA state for each inter measurement frequency
   3) From previous MEASURE_RSCP response msg
   4) Init values are 0
  @endverbatim 
  */
//  #ifndef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  tdssrchlog_inter_freq_lna_type   interLnaInfo;
// #else
//  tfw_rx_lna_t  lnaInfo;
  //96 bits  
//  #endif
  /*! \brief Cells config for RSCP measurement 

  @verbatim 
   1) If numCells>8, cellCfg[0~7] has timing measurement.
   2) configure wallTimeOffset if it is available for this particulr cell, 
      otherwise 0.
   3) If (measured freq==serving cell primary freq), 
      a. The first cell (cellCfg[0]) must be the serving cell
      b. The next are the rest of TS0 interative cells for that freq
      c. The next are the rest cells for timing measurement
      c. The last cells are ISCP cells 
  @endverbatim 
  */
  tdssrchlog_top_eight_rscp_cell_info_t   top8cellrpt[TDSSRCHLOG_MAX_TOP_RSCP_CELLS];
}tdssrchlog_ts0_cell_update_shorten_pkt_type;
/* Macro to determine the sizes of ts0 cell update log packet */
#define TDSSRCHLOG_TS0_CELL_UPDATE_SHORTEN_PKT_LEN  \
        (sizeof(tdssrchlog_ts0_cell_update_shorten_pkt_type))






/*! \brief  LOG PACKET: TDSSRCHLOG_TS0_CELL_UPDATE_LOG_PACKET
    (LOG CODE: 0xD052)

This packet contains the TS0_CELL measurment request and results. 

It is generated when ts0_cell measurment result is received. 

The packet size for this log is 588  bytes.
*/
typedef struct 
{

  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32  reserved:24;
  //32 bits
   /*! \brief Rx Subframe nubmer for unscheduled measurement */
    uint16                    subframeNum;
  /*! \brief measured frequency (Hz)
  @verbatim 
   Used by FW for Frequency Tracking Loop (FTL)
  @endverbatim 
  */
  uint16                    measuredFreq;
    //32 bits

  /*! \brief total number of cells for TS0 operation */
    uint32                    numCells : 6;      
  /*! \brief total number of cells for for iterative cancellation in TS0 
  @verbatim 
   1-4: if it is for demodualtion
   1-8: if it is for measurement 
  @endverbatim 
  */
    uint32                    numItrvCells : 4;
  /*! \brief total number of cells for for ISCP measurement in TS0
  */
    uint32                    numIscpCells: 3;

  /*! \brief Rx diversity flag 
  @verbatim
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored
  @endverbatim 
  */
  uint32                    rxDiv : 1;
  /*! \brief Rx antenna config
  @verbatim   
   0: Antenna 0
   1: Antenna 1
  @endverbatim 
  */ 
  uint32                    rxAntenna : 1;
  /*! \brief Inter freq measurement flag
  @verbatim 
   0: Intra freq
   1: Inter freq
  @endverbatim 
  */
  uint32                    interFreq : 1;
  /*! \brief Active cell primary freq flag 
  @verbatim 
   0: measured freq is not the same as primary freq of the serving cell
   1: measured freq is the same as the primary freq of the serving cell
  @endverbatim 
  */
  uint32                      activePrimaryFreq : 1;

  /*! \brief RF script buffer index for inter freq tuning
  @verbatim 
   If interFreq is 0, ignored
  @endverbatim 
  */
  uint32                    rfRxFreqBufIndex : 8;
  /*! \brief Index to RXLM static settings
  @verbatim 
   If interFreq is 0, ignored
  @endverbatim 
  */
  uint32                    rxlmStaticBufIndex :7;
  //32 bits


  //32 bits
    /*! \brief error status of current report 
  @verbatim 
   0: no error detected
   1: no  ts0_cell_cmd received
   2: serving cell config error 
   others: reserved
  @endverbatim 
  */
    uint16                    errStatus   : 3;
  //reserved
    uint16                    :  13;
    int16                     noiseVar;
	 /*! \brief RSSI on two antennas */ 
    int16                     rssi[TDSSRCHLOG_NUM_ANTENNAS];
    //32 bits
     int16                    midRssi[TDSSRCHLOG_NUM_ANTENNAS];
  /*! \brief LNA info for inter freq measurement 
  @verbatim 
   1) Used for inter freq measurement (interFreq set to 1)
   2) SW helps to remember LNA state for each inter measurement frequency
   3) From previous MEASURE_RSCP response msg
   4) Init values are 0
  @endverbatim 
  */
//  #ifndef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  tdssrchlog_inter_freq_lna_type   interLnaInfo;
//    #else
//  tfw_rx_lna_t  lnaInfo;
  //96 bits  
//  #endif
  /*! \brief Cells config for RSCP measurement 

  @verbatim 
   1) If numCells>8, cellCfg[0~7] has timing measurement.
   2) configure wallTimeOffset if it is available for this particulr cell, 
      otherwise 0.
   3) If (measured freq==serving cell primary freq), 
      a. The first cell (cellCfg[0]) must be the serving cell
      b. The next are the rest of TS0 interative cells for that freq
      c. The next are the rest cells for timing measurement
      c. The last cells are ISCP cells 
  @endverbatim 
  */
  tdssrchlog_top_eight_rscp_cell_info_t   top8cellrpt[TDSSRCHLOG_MAX_TOP_RSCP_CELLS];
  tdssrchlog_remained_rscp_cell_info_t   other32cellrpt[TDSSRCHLOG_MAX_REMAINED_RSCP_CELLS];
  tdssrchlog_ts0_cell_iscp_report_t  last5cellIscprpt[TDSSRCHLOG_MAX_NUM_ISCP];
  
}tdssrchlog_ts0_cell_update_pkt_type;

/* Macro to determine the sizes of ts0 cell update log packet */
#define TDSSRCHLOG_TS0_CELL_UPDATE_PKT_LEN  \
        (sizeof(tdssrchlog_ts0_cell_update_pkt_type))

/* Macro to determine the sizes of meas schedule log packet */
//  #ifndef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
#define TDSSRCHLOG_TS0_CELLUPDATE_PKT_LEN(toprscp_no,rscp_no,iscp_no) ( \
    FPOS(tdssrchlog_ts0_cell_update_pkt_type, rssi)+  \
    (4*sizeof(int16)) + \
    (1 * sizeof(tdssrchlog_inter_freq_lna_type)) + \
    (toprscp_no * sizeof(tdssrchlog_top_eight_rscp_cell_info_t)) + \
    ((rscp_no-toprscp_no) * sizeof(tdssrchlog_remained_rscp_cell_info_t)) + \
    (iscp_no * sizeof(tdssrchlog_ts0_cell_iscp_report_t)))
//    #else
	
/* #define TDSSRCHLOG_TS0_CELLUPDATE_PKT_LEN(toprscp_no,rscp_no,iscp_no) ( \
    FPOS(tdssrchlog_ts0_cell_update_pkt_type, rssi)+  \
    (4*sizeof(int16)) + \
    (1 * sizeof(tfw_rx_lna_t)) + \
    (toprscp_no * sizeof(tdssrchlog_top_eight_rscp_cell_info_t)) + \
    ((rscp_no-toprscp_no) * sizeof(tdssrchlog_remained_rscp_cell_info_t)) + \
    (iscp_no * sizeof(tdssrchlog_meas_iscp_pair_type)))
    #endif
*/

/*--------------------------------------------------------------------------
 LOG PACKET: TDSCDMA_TS0_CELL_TABLE_LOG_PACKET
    (LOG CODE: 0xD053)
 LOG PACKET: TDSCDMA_ACCUMULATE_TS0_CELL_TABLE_LOG_PACKET  
    (LOG CODE: 0xD056)
--------------------------------------------------------------------------*/
typedef struct
{
  uint8 iterative_cell_flag;
  int8  ts_rsp;
  uint16 ts_rsp_time_stamp;
  //32 bits
  uint16 wall_time_offset_stamp;
  int16 rscp;
  //32 bits
  /*! wall_time_offset(28:16) = subframe offset 
    * wall_time_offset(15:0) = ts0StartPosition (in units of Cx8)
    */
  uint32 wall_time_offset;  
   /*! \brief metric M, per OTD cell, for purpose of fake cell pruning 
  @verbatim
   M = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid_1stShift|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricM;
  /*! \brief metric M prime, per OTD cell, reserved 
  @verbatim   
   M' = |CIR_mid_1stShift*conj(CIR_dwpts)|^2/(|CIR_mid|^2*|CIR_dwpts|^2)
   Format 16Q16
  @endverbatim 
  */
    uint16                     metricMPrime;
    /*! \brief noise variance computed from DWPTS_GP in dBm (format 16Q8)*/
  //32 bits
}
tdssrchlog_ts0_cell_info_type;

/*! \brief  LOG PACKET: TDSSRCHLOG_TS0_CELL_TABLE_LOG_PACKET
    (LOG CODE: 0xD053)

This packet outputs the entire ts0 table maintained by L1 SW.

It is generated when ts0_cell measurment result is received. 

The packet size for this log is 1536  bytes.
*/

typedef struct 
{

  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32 reserved:8;
  /*! \brief the frequency measured*/
  uint32  measuredfreq:16;

  //32 bits

  /*! \brief ts0 table content */
  tdssrchlog_ts0_cell_info_type ts0_table[128];
}tdssrchlog_ts0_cell_table_pkt_type;

/* Macro to determine the sizes of ts0 cell table log packet */
#define TDSSRCHLOG_TS0_CELL_TABLE_PKT_LEN  \
        (sizeof(tdssrchlog_ts0_cell_table_pkt_type))
        
        
/*--------------------------------------------------------------------------
 LOG PACKET: TDSCDMA_NONTS0_CELL_UPDATE_LOG_PACKET
    (LOG CODE: 0xD054)
--------------------------------------------------------------------------*/
/*! \brief RSCP CELL INFO
 *  
 *   64 bits total. 
 */
typedef struct {
/*! \brief Previous timing measurement result
  @verbatim 
   1) Absolute wall time offset of the subframe boundary (cx8) 
   2) If available, configured as ts0StartPosition from previous RSCP 
      response msg.
   3) If not available, configured as ts0StartPosition from the serving 
      cell.
  @endverbatim 
  */
  uint32                    wallTimeOffset;
  /*! \brief Cells midamble index */
  uint32                     midambleIdx:8;
     /*! \brief TS-RSP value of a particulr cell
  @verbatim
   Format 16Q8
  @endverbatim 
  */
  int32                     tsRsp:16;
   /*! reserved */
   int32                    :8;
  
  //32 bits
} tdssrchlog_measure_workfreq_rscp_cell_info_t;

typedef struct
{
  /*! \brief Slot Index,valid slot index for ISCP measurement: 2, 3, 4, 5, 6, 0 */ 
  uint8                     slotIdx;
  /*! \brief Cell midamble index */
  uint8                     midambleIdx;
  int16                     filtTsRsp;
  int16                     iscp;
  int16                     reserved;
}tdssrchlog_workfreq_cell_iscp_measurementrpt_t;


/*! \brief  LOG PACKET: TDSSRCHLOG_NONTS0_CELL_UPDATE_LOG_PACKET
    (LOG CODE: 0xD054)
 * This packet outputs non-ts0 cell update cmd and response, 
 * It is generated once after SW receives FW non-TS0 cell response.
 * The packet size for this log is 448 bytes.
 */
typedef struct 
{
  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32  reserved:24;
  //32 bits
   /*! \brief Rx Subframe nubmer for unscheduled measurement */
  uint16                    subframeNum;
  /*! \brief message sequence number */ 
  uint16                    msgId;
  /*! \brief total number of cells for non-TS0 operation, up to 8 */
  uint16                    numCells : 6;      
  /*! \brief total number of cells for for iterative cancellation in non-TS0 
   * 1-4: if it is for demodualtion
   * 1-8: if it is for measurement (ISCP)
   */
  uint16                    numItrvCells : 4;
  /*! \brief total number of cells for for ISCP measurement in non-TS0 */
  uint16                    numIscpCells: 3;
  uint16                    errStatus       : 3;
  uint16                    reserved1;
   /*! \brief wallTimeOfs of current serving cell
   * wallTimeOfs(28:16) = subframe offset 
   * wallTimeOfs(15:0) = ts0StartPosition (in units of Cx8)
   */
  uint32                    currTimeOffset;
  /*! \brief current Freq offset (unit: Q8 Hz) */
  int32                     currFreqOffset;
    /*! \brief noise variance for each slot in dBm (format 16Q8)
     index 0-> slot 2, 1 -> 3, 2->4, 3->5, 4->6*/             
  int16                     noiseVar[5];
  /*! \brief midamble RSSI for each slot in dBm (format 16Q8)
     index 0-> slot 2, 1 -> 3, 2->4, 3->5, 4->6*/             
  int16                     rssi[5];
  /*! \brief Cell midamble index for demodulation, the first one must be serving cell */

   /*! \brief Cell midamble index for demodulation, the first one must be serving cell */
  tdssrchlog_measure_workfreq_rscp_cell_info_t     demodCellrpt[TDSSRCHLOG_MAX_NONTS0_TS_RSP_CELLS];

  /*! \brief ISCP measurement for nonTs0 slots & their respective midamble ID 
   * slotCellCfg[n] has the lower/same slotIdx than slotCellCfg[n+1] 
   */
  tdssrchlog_workfreq_cell_iscp_measurementrpt_t iscpCellrpt[TDSSRCHLOG_MAX_NUM_ISCP];
}tdssrchlog_nonts0_cell_update_pkt_type;

/* Macro to determine the sizes of scell tracking status log packet */
#define TDSSRCHLOG_NONTS0_CELL_UPDATE_PKT_LEN  \
        (sizeof(tdssrchlog_nonts0_cell_update_pkt_type))

/*converting the following F3 msg to log packet info in 0xD06D
   TDSL1_MSG4(HIGH,"RXDPM update  HS_SCCH Activity every sub frame : CRC_pass=%d, HCSN=%d,SCCH_SIR_TARGET=%d,SCCH_SCHEDULE_RATE=%d",scch_update_latest.scch_crc_pass,scch_update_latest.scch_hcsn,scch_update_latest.scch_sir_target,tdsrxdiv_scch_schedule_rate) ;
   */
/*!
 *RXDPM update. 
 *Logged every 10ms
 *Size: 6 bytes
 */
typedef PACK(struct)
{
	/*!CRC passed or not*/
    uint8 scch_crc_pass ;
	/*!HCSN*/
    uint8 scch_hcsn ;
	/*!SCCH SIR target*/
    int16 scch_sir_target ;
	/*!SCCH schedule rate*/
	uint16 tdsrxdiv_scch_schedule_rate ;
}rxdpm_update_hsscch_type;

/* converting the following F3 msg to log packet info in 0xD06D.
    MSG_HIGH("HSDPA UPDATE HS-SCCH DECODE DONE : hcsn=%d, Scch_SIR_Target=%d,sub_fn=%d",scch_update->scch_hcsn,scch_update->scch_sir_target,tdsseq_get_sub_frame_num());
	*/
/*!
 *info when HSDPA HS-SCCH decoding is done.
 *logged every 5ms
 *size: 5 bytes 
 */
typedef PACK(struct)
{
	/*!HCSN*/
    uint8	scch_hcsn ;
	/*!SCCH SIR target*/
    int16	scch_sir_target ;
	/*!subframe number*/
	uint16	sub_fn;
}hsdpa_update_hsscch_decode_type;

/*!
1. LOG PACKET: TDSCDMA_RXD_LOG_PACKET (LOG CODE: 0xD06D).

2. In DCH state, every 10ms the packet is submitted at the end of function "tdsrxdiv_state_update()"; 
	In DRX state, updated every DRX state.	

3. Packet size: 27 bytes
 */
typedef PACK(struct) 
{
  /*! \brief Log header */
  log_hdr_type header;
  /*! \brief Version  */
  uint8  version;
  /*!subframe number*/
	uint32							sfn;
	/*!RxD state*/
	uint8							rxd_state; 
	/*!RDDS state*/
	uint8							rdds_state;	
	/*!RxDPM state*/
	uint8							rxdpm_state;	
	/*!ARD state*/
	uint8 							ard_state;
	/*!condition bit mask*/
	uint16 							tdsrxdiv_current_cond;
	/*!rxdpm update*/
	rxdpm_update_hsscch_type		rxdpm_update_hsscch;
	/*!hsdpa update*/
	hsdpa_update_hsscch_decode_type hsdpa_update_hsscch_decode[2];
}tdssrchlog_RxD_pkt_type; 

/* Macro to determine the sizes of RxD log packet */
#define TDSSRCHLOG_RXD_PKT_LEN  (sizeof(tdssrchlog_RxD_pkt_type))

/*--------------------------------------------------------------------------
 LOG PACKET: TDSCDMA_NONTS0_CELL_TABLE_LOG_PACKET
    (LOG CODE: 0xD055)
 LOG PACKET: TDSCDMA_ACCUMULATE_NON_TS0_CELL_TABLE_LOG_PACKET 
    (LOG CODE: 0xD057)
   --------------------------------------------------------------------------*/
typedef struct 
{
  uint8 iterative_cell_flag;
  int8  ts_rsp;
  uint16 ts_rsp_time_stamp;
/*! Filtered F_metric value at SW L1 */
#ifdef FEATURE_TDSCDMA_NEW_NON_TS0_PRUNING	
  uint16 cell_F_metric;
#endif 	
}tdssrchlog_nonts0_cell_info_type;

/*! \brief  LOG PACKET: TDSSRCHLOG_NONTS0_CELL_TABLE_LOG_PACKET
    (LOG CODE: 0xD055)

This packet outputs the entire non-TS0 table maintained by L1 SW.

It is generated when non-TS0 demod result is received. 

The packet size for this log is 520  bytes.
*/

typedef struct 
{

  log_hdr_type header;

  /*! \brief Version  */
  uint32  version:8;
  uint32  reserved:24;
  //32 bits

  /*! \brief non-TS0 table content, 512bytes */
  tdssrchlog_nonts0_cell_info_type nonts0_table[128];
}tdssrchlog_nonts0_cell_table_pkt_type;

/* Macro to determine the sizes of ts0 cell table log packet */
#define TDSSRCHLOG_NONTS0_CELL_TABLE_PKT_LEN  \
        (sizeof(tdssrchlog_nonts0_cell_table_pkt_type))


#endif /* TDSSRCHLOG_H */
