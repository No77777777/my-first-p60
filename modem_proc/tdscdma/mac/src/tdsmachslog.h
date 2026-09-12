#ifndef TDSMACHSLOG_H__
#define TDSMACHSLOG_H__

/*===========================================================================
                             M A C H S L O G . H

DESCRIPTION
   This file contains MAC HS layer log packet structure definitions, 
   prototypes for MAC HS logging functions, any constant definitions that 
   are needed, and any external variable declarations needed for MAC HS logging.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


============================================================================*/


/*==========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmachslog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS


============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif
#include "log_codes_tdscdma.h"

#if  defined (FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING)

#include "tdsuecomdef.h"
#include "log.h"

#ifdef T_WINNT
  #error code not present
#endif

#define TDSMAC_HS_NUM_REPORTS_IN_STATUS_LOG_PKT                           100



/*!===========================================================================

                      DATA STRUCTURES FOR LOG PACKETS
                      
===========================================================================*/                      

/*!
 Data structure for Logical Channel configuration info
 */

typedef PACK(struct)
{
   /*! RLC ID range 0-17 */
   uint8   rlc_id;
   
   /*! Logical channel type 
                0 – BCCH
                1 – PCCH
                2 – CCCH
                3 – DCCH
                4 – CTCH
                5 – DTCH  */
   uint8   chan_type;
   
   /*! Logical channel mode  
                0 – TM
                1 – UM
                2 or 3 – AM   */
   uint8   chan_mode;
   
   uint8 reserved; /*!< reserved for alignment*/
}tdsmac_hs_log_dlc_cfg_type;


/*!
 Data structure for HSDPA PDU size configuration
 */
typedef PACK(struct)
{
   /*! PDU Size  in MAC HS Header range 1-5000 */
   uint16 mac_pdu_size;
   
   /*! PDU size index in MAC HS header range 0-7 */
   uint8 mac_pdu_index;
   
   uint8 reserved; /*!< reserved for alignment*/
}tdsmac_hs_log_pdu_size_info_type;

/*!
 Data structure for HSDPA Reordering Queue configuration 
 */
typedef PACK(struct)
{
   /*! Queue Id range 0-7 */
   uint8 queue_id;
   
   /*! Receiver Window Size */
   uint8 window_size;

   /*! Reordering release timer in msecs
                Valid values:
                  10,20,30,40,50,60,70,80,90,100,120,140,160,200, 300, 400 */
   uint16 release_timer;
	  
   /*! No of PDU sizes range 1-8 */
   uint8 no_of_pdu_sizes;

   uint8 reserved; /*!< reserved for alignment*/
   uint16 reserved1; /*!< reserved for alignment*/
   /*! MAC PDU size type information for this queue */
   tdsmac_hs_log_pdu_size_info_type pdu_info[UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE];

} tdsmac_hs_log_queue_info_type;

/*!
 Data structure for HSDPA MAC D FLOW configuration Information 
 */ 
typedef PACK(struct)
{
  /*! CCTrCh ID range 0-7 */
  cctrch_id_type cctrch_id;

  /*! MAC-d Flow Id range 1-8 */
  uint8 mac_dflow_id; 

  /*! No of priority queues range 1-8 */
  uint8 no_of_queues;

  /*! Number of dedicated RLC logical channels mapped
              to the HSDSCH transport channel range 1-32  */
  uint8 ndlchan;

  /*! Queue Information */
  tdsmac_hs_log_queue_info_type queue_info[UE_MAX_HS_QUEUE];

  /*! Info of dedicated logical channels mapped */
  tdsmac_hs_log_dlc_cfg_type dlchan_info[UE_MAX_LOGCHAN_PER_DFLOW];

} tdsmac_hs_log_mac_dflow_info_type;

/*!
 Data structure for HSDPA Status log packet
 */    
typedef PACK(struct)
{
  /*! Queue Id to which this TSN belongs (Range 0-7) */
  uint8 queue_id;

  /*! Received TSN (Range 0-63) */
  uint8 rcvd_tsn;

  /*! Next expected TSN (Range 0-63) */
  uint8 next_expected_tsn;

  /*! Window Upper Edge TSN to which timer is associated (Range 0-63) */
  uint8 window_upper_edge;

  /*! TSN to which the timer is associated (Range 0-63)
            -- If T1 timer is disabled, the value 255 shall be returned,
               otherwise the T1_TSN shall be specified.  */ 
  uint8 t1_tsn;
  
  uint8 reserved; /*!< reserved for alignment*/
  uint16 reserved1; /*!< reserved for alignment*/

  /*! current SFN (Range 0-4095) */
  uint16 cur_sfn;

  /*! current sub frame number (Range 0-8190) */
  uint16 cur_sub_frame_num;
}tdsmac_hs_log_status_info_type;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

/*!
 Data structure for Enh HSDPA Reordering Queue log packet
 */    
typedef PACK(struct)
{
   /*! Queue Id range 0-7 */
   uint8 queue_id;
     
   /*! Treset timer value - used in CellFACH and CellPCH states */
   uint8 t_reset;

   
   /*! Reordering release timer in msecs
                Valid values: 10,20,30,40,50,60,70,80,90,100, 120,140,160,200, 300, 400 */
   uint16 release_timer;

   /*! Receiver Window Size */
   uint8 window_size;

   
  uint8 reserved; /*!< reserved for alignment*/
  uint16 reserved1; /*!< reserved for alignment*/  
   
} tdsmac_ehs_log_queue_info_type;

/*!
 Data structure for Enh HSDPA PDU log information
 */    
typedef PACK(struct)
{
  /*! Received TSN (Range 0-63) */
  uint8 rcvd_tsn;
  
  /*! Next Expected TSN (Range 0-63) */
  uint8 next_expected_tsn;
  
  /*! Upper Edge TSN (Range 0-63) */
  uint8 upper_edge;
  
  /*! T1 TSN (Range 0-63) */
  uint8 t1_tsn;
  
  /*! Queue Id and LC Id
              Bit 2..0 - Queue Id
              Bit 6..3 - LC Id
              Bit 7 reserved
   */
  uint8 queue_lc_id;
  
  uint8 reserved1; /*!< reserved for alignment*/  
  /*! Bit 1..0 - Reassembly PDU Type (0 - Complete, 1 - Partial Start, 
   *                                 2 - Partial Middle, 3 - Partial End)
   * Bit 12..2 - Segment Size in Bytes
   * Bit 15..13 - Reserved
   */
  uint16 pdu_seg_info_size;
  
} tdsmac_ehs_log_pdu_info;
#endif

/*===========================================================================

                      DATA STRUCTURES FOR EVENTS
                      
===========================================================================*/                      

/*!
 Data structure for Timer Expiry Event
 */    
typedef PACK(struct)
{
   /*! Queue for which the timer expired */
   uint8 queue_id;

   /*! Associated TSN */
   uint8 t1_tsn;
   
   uint16 reserved1; /*!< reserved for alignment*/  
}tdsmac_hs_log_timer_expiry_event_type;

/*===========================================================================

                      LOG PACKET DEFINITIONS
                      
===========================================================================*/                      

/*! TDSCDMA_MAC_HS_CONFIG_LOG_PACKET */
typedef PACK(struct)
{
  /*! brief Log header */
  log_hdr_type   hdr;
  /*! Version Number */
  uint8 version_num;
  
  /*! HSDPA Action 
                0 – HS_START
                1 – HS_STOP
                2 – HS_RECFG */
  uint8 mac_hsdpa_action;

  /*! Number of MAC-d Flows (Range 1-8). */
  uint8 num_dflow;  
  
  /*! Indicator whether reset is requested or not */
  boolean mac_hsdpa_reset_indicator;
  
  /*! Indicator whether MAC-ehs is configured or not */
  boolean mac_ehs_enabled;
  
  /*! Number of MAC-ehs reordering Queues (Ranger 1-8) */
  uint8 num_mac_ehs_reord_queue;  
  
  uint16 reserved1; /*!< reserved for alignment*/  
  /*! HSDPA MAC-d Flow Structure */
  tdsmac_hs_log_mac_dflow_info_type hsdpa_info[UE_MAX_MAC_D_FLOW];
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  /*! Enh HSDPA Reordering Queue structure */
  tdsmac_ehs_log_queue_info_type enh_hs_queue_info[UE_MAX_HS_QUEUE];
#endif

}TDSCDMA_MAC_HS_CONFIG_LOG_PACKET_type;
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

/*! TDSCDMA_MAC_HS_REASSEMBLY_LOG_PACKET */
typedef PACK(struct)
{
  /*! brief Log header */
  log_hdr_type   hdr;
  /*! Log packet version number */
  uint8 version_num;
  /*! Version number, Active Mac
          Bit 3:0 – Version number; 
     	            version number of this log packet; 
                    this can be displayed in the packet name,
                    e.g., Version 2
          Bit 4   – Active Mac; MAC-hs version
 	                0 – MAC-hs
 	                1 – MAC-ehs
          Bit 7:5 – Reserved */
  uint8 hs_type_and_other_info;
  
  /*! Num PDUs 
              Makes log packet submit every 10 ms in worst case;
              range 1 to 255 */
  uint8 num_pdu;
  
  uint16 reserved1; /*!< reserved for alignment*/  
  tdsmac_ehs_log_pdu_info pdu_info[15*TDSL1_DEC_HS_REORDER_SDU_MAX_COUNT];
}TDSCDMA_MAC_HS_REASSEMBLY_LOG_PACKET_type;

#endif /* #if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS) */




/*! TDSCDMA_MAC_HS_STATUS_REPORT_LOG_PACKET */

typedef PACK(struct)
{
  /*! brief Log header */
  log_hdr_type   hdr;
  /*! Log packet version number */
  uint8 version_num;
  
  /*! Number of accumulated Status log packets (Range 0-100) */
  uint8 num_pkts;

  uint8 reserved; /*!< reserved for alignment*/
  uint16 reserved1; /*!< reserved for alignment*/  
  /*! List of Status packets */
  tdsmac_hs_log_status_info_type pkt_list[TDSMAC_HS_NUM_REPORTS_IN_STATUS_LOG_PKT];
}TDSCDMA_MAC_HS_STATUS_REPORT_LOG_PACKET_type;


/*! TDSCDMA_MAC_HS_RESET_LOG_PACKET */
typedef PACK(struct)
{
  /*! brief Log header */
  log_hdr_type   hdr;
  /*! Log packet version number */ 
  uint8 version_num;

  /*! Number of logical channel currently mapped to different Mac-d flows (Range 1-32) */
  uint8 num_dlc;

   /*! List of logical channel Ids. (Id range 1-32) */
  uint8 dlc_list[TDSUE_MAX_DL_LOGICAL_CHANNEL];


}TDSCDMA_MAC_HS_RESET_LOG_PACKET_type;



#ifdef T_WINNT
  #error code not present
#endif

#endif /* FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING */
#endif /* TDSMACHSLOG_H__ */





