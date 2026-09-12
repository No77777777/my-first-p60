#ifndef WL1_X2W_SRHC_LOG_H
#define WL1_X2W_SRHC_LOG_H

/*===========================================================================
   FILE: wl1x2wsrchlog.h

GENERAL DESCRIPTION
   This file contains all of the definitions necessary for G2W srch log 
   packets.
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2002 - 2013 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/13/19   hs     Added uarfcn TO 0x41D7, 0x41D8, 0x41D9 log packet
10/02/15   gn     Added 0x41D7, 0x41D8, 0x41D9 log packet for X2W driver code
06/24/14   cc     Moving G2W driver to WL1 folder
03/15/12   mm     CR 344531 G2W drivers new design
26/03/08   ws     Added PACKED_POST for QDSP6 targets
03/04/07   ws     Corrected Featurisation for GSM only builds
09/16/05   am     Created file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "log.h"
#include "log_codes_wcdma_int.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define WL1_X2W_SRCH_LOG_MAX_PSC_PEAKS_TO_KEEP 12
#define WL1_X2W_SRCH_LOG_MAX_NASTT_PEAK_COUNT 6
#define WL1_X2W_SRCH_LOG_LIST_PEAKS_PER_TASK 4
#define WL1_X2W_MDSP_LOG_LIST_SRCH_MAX_TASKS 41

/* Header definition since the header definition is defined in log.c */
typedef PACKED struct PACKED_POST
{
  uint16 len;  /* Specifies the length, in bytes of 
                 the entry, including this header. */

  uint16 code; /* Specifies the log code for the 
                  entry as enumerated above.       
                  Note: This is specified as word 
                  to guarantee size.               */

  qword ts;    /* The system timestamp for the log entry. The upper 48 bits
                  represent elapsed time since 6 Jan 1980 00:00:00 
                  in 1.25 ms units. The low order 16 bits represent elapsed 
                  time since the last 1.25 ms tick in 1/32 chip units 
                  (this 16 bit counter wraps at the value 49152).          */
} wl1_x2w_log_hdr_struct_type;


/*--------------------------------------------------------------------------
                 LOG PACKET: WL1_X2W_SRCH_STEP1_LOG_PACKET
                     
This structure is used to log search step 1 information. These packets occur
upon every id search in idle mode.          
--------------------------------------------------------------------------*/

#ifndef WL1_X2W_SRCH_STEP1_LOG_PACKET
#define WL1_X2W_SRCH_STEP1_LOG_PACKET L1_WCDMA_X2W_STEP1_0x41D7_LOG_PKT
#else
#error redefinition of LOG command code: WL1_X2W_SRCH_STEP1_LOG_PACKET
#endif

/* information for each peak returned */
typedef PACKED struct PACKED_POST 
{
   /* Pos value */
   uint32 pos ;

   /* energy level for the peak */
   uint16 eng ;

} wl1_x2w_srch_log_psc_peak_struct_type;
 
  
LOG_RECORD_DEFINE(WL1_X2W_SRCH_STEP1_LOG_PACKET)

  /* Pkt Version */
  uint8 pkt_ver;
  /* EcIo conversion type */
  uint8 ecio_cov_type;
  /*RSSI value in dBm units*/
  int16 rssi;
  /* Number of peaks obtained */
  uint8 step1_valid_peak_cnt;
  /* Num of peaks requested */
  uint8 num_peaks_req;
  /* Threshold peak value for Step-1 results */
  uint16 peak_threshold;
  /* Threshold value to trigger NASTT */
  uint16 nastt_retrigger_threshold;
  /* Non-coherent integration length*/
  uint8 nn;  
  /* UARFCN Value */
  uint16 uarfcn;
  /* the actual peak log records */
  wl1_x2w_srch_log_psc_peak_struct_type  wl1_x2w_srch_log_psc_peak_info[WL1_X2W_SRCH_LOG_MAX_PSC_PEAKS_TO_KEEP];

LOG_RECORD_END


/*--------------------------------------------------------------------------
                 LOG PACKET: WL1_X2W_SRCH_NASTT_LOG_PACKET
                     
This structure is used to log NASTT search information. These packets occur
upon every id search in idle mode.         
--------------------------------------------------------------------------*/

/* the log code for the seach step 2 packet */
#ifndef WL1_X2W_SRCH_NASTT_LOG_PACKET
#define WL1_X2W_SRCH_NASTT_LOG_PACKET L1_WCDMA_X2W_NASTT_0x41D8_LOG_PKT
#else
#error redefinition of LOG command code: WL1_X2W_SRCH_NASTT_LOG_PACKET
#endif

/* information for each code group tested  */
typedef PACKED struct PACKED_POST 
{
  /* Primary Scrambling code PSC position in chipx8 resolution.  Range :  0....(8x2560 - 1) */
  uint16 psc ;

  /* energy level for the peak */
  uint16 eng ;

  /* position value */
  uint32 pos ;
   
} wl1_x2w_srch_log_nastt_struct_type ;


LOG_RECORD_DEFINE(WL1_X2W_SRCH_NASTT_LOG_PACKET)
  
  /* Pkt Version */
  uint8 pkt_ver;
  /* EcIo conversion type */
  uint8 ecio_cov_type;
  /*RSSI value */
  int16 rssi;
  /* Number of valid NASTT peaks */
  uint8 nastt_valid_peak_cnt;
  /* Number of peaks retrieved */
  uint16 num_peaks;
  /* Threshold value for NAS-TT results */
  uint16 nastt_threshold;
  /* UARFCN Value */
  uint16 uarfcn;
  wl1_x2w_srch_log_nastt_struct_type  wl1_x2w_srch_log_nastt_info[WL1_X2W_SRCH_LOG_MAX_NASTT_PEAK_COUNT] ;

LOG_RECORD_END


/*--------------------------------------------------------------------------
                 LOG PACKET: WL1_X2W_SRCH_LIST_LOG_PACKET
                     
This structure is used to log list search information.
--------------------------------------------------------------------------*/

/* the log code for the list seach packet */
#ifndef WL1_X2W_SRCH_LIST_LOG_PACKET
#define WL1_X2W_SRCH_LIST_LOG_PACKET L1_WCDMA_X2W_LIST_0x41D9_LOG_PKT 
#else
#error redefinition of LOG command code: WL1_X2W_SRCH_LIST_LOG_PACKET
#endif

typedef PACKED struct PACKED_POST 
{
  /* pn position */
  uint32 pos[WL1_X2W_SRCH_LOG_LIST_PEAKS_PER_TASK];

  /* searcher energy */
  uint16 eng[WL1_X2W_SRCH_LOG_LIST_PEAKS_PER_TASK];
    
} wl1_x2w_srch_log_list_search_task_type ;
 
/* information for each task specified */
typedef PACKED struct PACKED_POST 
{
  /* coherent integration duration */
  uint16 nc ;
  /* non-coherent integration */
  uint8 nn ;
  /* window start pn position  specified in offsets from reference
     counter in chipx8 resolution */
  uint32 pn_pos ;
    
  /* scrambling code index. 13 bits for scr_code and 3 bits for set */
  uint16 scr_code;

  /* alignment mode. 000 - no alignment.  
                     001 - align coherent integration time (CIT) to the
                           time when pn count is a multiple of 256 in 
                           chipx1 resolution 
                     010 - align CIT to the time when pn count is a 
                           multiple of 512 in chipx1 resolution
                     011 - align CIT to the time when pn count is a 
                           multiple of 1024 in chipx1 resolution
                     100 - align CIT to the time when pn count is a 
                           multiple of 2560 in chipx1 resolution
                     111 - reserved */
  uint8 align_mode ;

  /* peak detector  mode 
       0x00 - Normal mode - the peak detection is enabled and only the
              energy larger than the threshold and also greater than 
              energies half-chip away from it in either direction will
              be considered a peak
    
       0x01 - bypass mode -the peak detection is disabled and all energies
              above the threshold will be reported as peaks
        
       0x02 - Shoulder mode- used for searching associated with PICH in
              sleep mode. The searcher h/w returns 5 sets of peaks per 
              search window(each peak consists of 3 adjacent chipx2 reso-
              lution peaks) DSP interpolates these chipx2 resolution peaks
              to chipx8 resolution and reports the 4 best peaks to the 
              host processor
                       */
  uint8 pd_mode;
    
  /* window size 1...38400 in unit of chips*/
  uint16 win_siz ;

  /*  Output info   */  
  wl1_x2w_srch_log_list_search_task_type  wl1_x2w_srch_log_task_info;

} wl1_x2w_srch_log_list_input_task_type ;


LOG_RECORD_DEFINE(WL1_X2W_SRCH_LIST_LOG_PACKET)
   
  /* Pkt Version */
  uint8 pkt_ver;
  /* EcIo conversion type */
  uint8 ecio_cov_type;
  /* Rssi at this point in dbm units */
  int16 rssi;

 /*(4) idle reconfirmation/
    (5) stage1/
    (6) stage2/
    (7) dedicated reconfirmation */
 uint8 srch_type;

  /* Threshold energy for PN search results  */
  uint16 threshold_energy;
 
  /* number of tasks specified  */
  uint8 num_task_spec ;
  /* UARFCN Value */
  uint16 uarfcn;
  wl1_x2w_srch_log_list_input_task_type wl1_x2w_srch_log_input_task_info[WL1_X2W_MDSP_LOG_LIST_SRCH_MAX_TASKS] ;


LOG_RECORD_END

/* step1 log length */
#define WL1_X2W_SRCH_STEP1_PACKET_LEN(step1_peaks)  ( \
    FPOS(WL1_X2W_SRCH_STEP1_LOG_PACKET_type, wl1_x2w_srch_log_psc_peak_info)+  \
	 ( step1_peaks * sizeof(wl1_x2w_srch_log_psc_peak_struct_type))  )
     
/* NASTT log length */
#define WL1_X2W_SRCH_NASTT_PACKET_LEN(nastt_peaks)  ( \
    FPOS(WL1_X2W_SRCH_NASTT_LOG_PACKET_type, wl1_x2w_srch_log_nastt_info)+  \
	 ( nastt_peaks * sizeof(wl1_x2w_srch_log_nastt_struct_type))  )
    
/* List search log length */
#define WL1_X2W_SRCH_LIST_SRCH_PACKET_LEN(input_tasks)  ( \
	FPOS(WL1_X2W_SRCH_LIST_LOG_PACKET_type, wl1_x2w_srch_log_input_task_info ) + \
	(input_tasks*sizeof(wl1_x2w_srch_log_list_input_task_type)) + \
	(input_tasks*sizeof(wl1_x2w_srch_log_list_search_task_type))  )

#endif /* WL1_X2W_SRHC_LOG_H */
