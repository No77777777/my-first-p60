/*!
  @file
  lte_LL1_log_timing.h

  @brief
  timing log packet data structures

  @detail
  !!! Please do not modify !!!
  This file was auto-generated on Fri Feb 12 16:25:11 2010.
  Command line used: perl w2h.pl log_timing timing 1
   Note: The params, [1], correspond to version numbers for
         each log message in the listed order in the ICD

*/

/*===========================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_log_timing.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef LTE_LL1_LOG_TIMING_H
#define LTE_LL1_LOG_TIMING_H


/*==========================================================================*/
/*! @brief
 * 8 LTE LL1 Serving Cell Frame Timing (Log code ID: 0xB114)
 *
 *  This log packet provides information about downlink and uplink timing. When 
 *  enabled, the log record is generated at least once every 20 subframes.  If 
 *  change in frame timing is outside of the timing adjustment range,  a new 
 *  log is generated. 
---------------------------------------------------------------------------*/
#if 0
#define LTE_LL1_LOG_TIMING_SERVING_CELL_FRAME_TIMING_VERSION 1

/*! @brief serving_cell_frame_timing main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 number_of_records                : 5;  ///< Range: 1-20.  Each record correspond to a
                                                         ///< subframe.
  uint32 starting_sub_frame_number        : 4;  ///< Sub-frame number (Range 0..9)
  uint32 starting_system_frame_number     : 10;  ///< System frame number (range 0..1023)
  uint32 reserved0                        : 5; 
  uint32 starting_dl_frame_timing_offset  : 19;  ///< Downlink frame timing offset (in Ts units)
                                                         ///< Range 0-307200
  uint32 reserved1                        : 13; 
  uint32 starting_ul_frame_timing_offset  : 19;  ///< Uplink frame timing offset (in Ts units)
                                                         ///< Range 0-307200
  uint32 starting_ul_timing_advance       : 11;  ///< Starting NTA value (in units of 16 Ts)
  uint32 reserved2                        : 2; 
   uint8 timing_adjustment[60]                        ;  ///< (20x24) bits
                                                         ///< Packed array of 24-bit adjustment records.
                                                         ///< Format of each record is:
                                                         ///< Bits 10:0 = change in DL frame timing (in Ts
                                                         ///< units), Range = -512 .. 511; bit10 is sign bit.
                                                         ///< Bits 15:11 = change in UL frame timing (in Ts
                                                         ///< units), Range = -16..15; bit15 is sign bit.
                                                         ///< Bits 23:16 = change in timing advance (in 16 Ts
                                                         ///< units) Range = -128..127; bit23 is sign bit.
}lte_LL1_log_timing_serving_cell_frame_timing_ind_struct;
#else


//version number for Atlas: 121-140
//version number for Hercules: 141-160
#define LTE_LL1_LOG_TIMING_SERVING_CELL_FRAME_TIMING_VERSION 141 
#define MAX_SCELL_FRAME_RECORDS 20
#define LTE_LL1_MAX_NUM_RX_ANT  4

typedef struct
{
  uint8 timing_adjustment[3];           ///< (20x24) bits
                                        ///< Packed array of 24-bit adjustment records.
                                        ///< Format of each record is:
                                        ///< Bits 10:0 = change in DL frame timing (in Ts
                                        ///< units), Range = -512 .. 511; bit10 is sign bit.
                                        ///< Bits 15:11 = change in UL frame timing (in Ts
                                        ///< units), Range = -16..15; bit15 is sign bit.
                                        ///< Bits 23:16 = change in timing advance (in 16 Ts
                                        ///< units) Range = -128..127; bit23 is sign bit.
  uint8  rx_ant_bmask;                                 ///< enabled antenna bitmask
  uint32 dl_sf_bndry_ustmr;             ///< SF boundary USTMR
  uint64 dl_sf_bndry_vsrc;              ///< SF boundary VSRCx16
  int32  total_freq_error;             ///< Total Freq Error including SRx offset in Hz
  int32  rotator_corrn;                ///< Rotator correction in HW units 29S28
  uint32  vsrc_t1byt2_corrn;             ///< VSRC t1byt2 correction
  uint32  vstmr_fcw;                    ///< FCW                                 
  int8   vsrc_sctr_error[LTE_LL1_MAX_NUM_RX_ANT];      ///< Q4 VSRC drift vs bookkeeped value computed for this dump
                                                       ///< calculate in linear value as x/16.0
  uint32 reserved0;                                    ///< 4 byte padding needed for 8 byte alignment 
                                                       ///due to the usage of uint64 in this structure
  int16  phase_cont_error[LTE_LL1_MAX_NUM_RX_ANT];     ///< NB Phase Tracking Error, 14s13 
                                                       ///< calculate in degrees as (x/2^13)*180.0 
                                                       ///range is [-180, +180) degrees
} lte_LL1_log_scell_frame_records_debug_struct;

/*! @brief serving_cell_frame_timing main struct
 */
typedef struct
{
  uint32 version                          : 8;   ///< Log packet version. Range: 0...255.
                                                 ///< Version = 1 for the log packet structure described below
  uint32 number_of_records                : 5;   ///< Range: 1-20.  Each record correspond to a subframe
  uint32 starting_sub_frame_number        : 4;   ///< Sub-frame number (Range 0..9)
  uint32 starting_system_frame_number     : 10;  ///< System frame number (range 0..1023)
  uint32 carrier_index                    : 4;   ///< 0-PCC, 1-SCC
  // ATLAS_CS1_CXN_ID_TODO: Add CXN ID field or repurpose one of the reserved fields. 
  // It needs to be 2 bit wide atleast as we may need 3 cxns
  uint32 reserved0                        : 1; 
  uint32 starting_dl_frame_timing_offset  : 19;  ///< Downlink frame timing offset (in Ts units)
                                                 ///< Range 0-307200
  uint32 reserved1                        : 13; 
  uint32 starting_ul_frame_timing_offset  : 19;  ///< Uplink frame timing offset (in Ts units)
                                                 ///< Range 0-307200
  uint32 starting_ul_timing_advance       : 11;  ///< Starting NTA value (in units of 16 Ts)
  uint32 en_b114_debug_mode               : 1;  ///< debug mode flag
  uint32 reserved2                        : 1; 
  uint32 dl_sf_mstmr_time;                       ///< MSTMR count read from HW on current DL subframe  
  lte_LL1_log_scell_frame_records_debug_struct scell_frame_debug_record[MAX_SCELL_FRAME_RECORDS];
}lte_LL1_log_timing_serving_cell_frame_timing_ind_struct;

#endif

#endif //LTE_LL1_LOG_TIMING_H
