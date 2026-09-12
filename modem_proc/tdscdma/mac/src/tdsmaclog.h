#ifndef TDSMACLOG_H
#define TDSMACLOG_H

/*===============================================================================================
                             M A C L O G . H

DESCRIPTION
This file contains or includes files that contain MAC layer log packet structure definitions, 
prototypes for MAC logging functions, any constant definitions that are needed, and any external
variable declarations needed for Layer 1 logging.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

================================================================================================*/


/*================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maclog.h_v   1.8   20 Nov 2001 16:25:34   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaclog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS

================================================================================================*/

#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "tdsuecomdef.h"
#include "log.h"
#include "log_codes_tdscdma.h"

#ifdef T_WINNT
  #error code not present
#endif

/*!
 Data structure for Transport Format Combination information
 */

typedef PACK(struct) 
{

    /*!  \brief Transport Format Control Flag 
                  0 - TDSTFC_ALLOWED
                  1 - TDSTFC_NOT_ALLOWED
                  2 - TDSTFC_REMOVED
                  Note - The Control Flag should match mac_ul_tfc_control_e_type
                  defined in tdsmacrrcif.h */
    uint8   tfc_valid;

    /*!  \brief Transport Format Indicator range 0-31 */
    uint8   tfi[TDSUE_MAX_TRCH];


}tdsmac_log_tfc_type;

/*!
 Data structure for Access Service Class information
 */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{

    /*!  \brief Access Service Class Identifier,range 0-7 */
    uint8   identifier;

    /*!  \brief Persistence value
                0 to 10 (0 indicates 0.0 and 10 indicates 1.0) */
    uint8   persistence;

}tdsmac_log_asc_type;

/*!
 Data structure for Logical Channel configuration info
 */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{

    /*!  \brief RLC ID, range 0-18 */
    uint8   rlc_id;

    /*!  \brief Logical channel type
                 0 - BCCH, 1- PCCH, 2 - CCCH, 3 - DCCH, 4 - CTCH, 5 - DTCH
                  Note - The logical channel type should match the enum type
                  uecomdef_logchan_e_type defined in tdsuecomdef.h               */
    uint8   chan_type;

    /*!  \brief Logical channel mode,
                 0 - TM, 1 - UM, 2 - AM */
    uint8   chan_mode;

    /*!  \brief Radio Bearer ID,range 0-31 */
    uint8   rb_id;

    /*!  \brief MAC ID (applicable only in UL ) */
    uint8   mac_id;

    /*!  \brief MAC priority (applicable only in UL),range 1-8 */
    uint8   mac_priority;

}tdsmac_log_lc_cfg_type;

/*!
 Data structure for Transport channel configuration info
 */
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{

    /*!  \brief Transport channel ID; range 1 - 32 
                  Note: BCH and PCH do not have Transport Channel IDs,
                  it displays the CCTRCH ID for these two channels.    */
    uint8   trch_id;

    /*!  \brief Transport channel type
                 0 - TDSINVALID,
                 1 - BCH,
                 4 - PCH,
                 5- FACH,
                 6 - DSCH,
                 7 - RACH,
                 8 - DCH              */
    uint8   trch_type;

    /*!  \brief Number of logical channels mapped to this transport channel;
                 range 0-19 (4 AM Data + 4 AM Ctl + 8 UM + 3 TM = 19)    */
    uint8   num_logchan;

    /*!  \brief Logical channel information */
    tdsmac_log_lc_cfg_type     logical_chan_info[1];       

}tdsmac_log_trch_cfg_info_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{

  /*!  \brief Radio bearer identity 0-31 */
  uint32     rb_id;
  /*! Gives the raw payload in the channel when the report was triggered; values are 2^n */
  uint32    raw_payload;
  /*! Stores the result of the average of RLC payloads taken over the measurement interval;
              values are rounded to 2^n */
  uint32    avg_payload;

  /*! Stores the result of the variance of the RLC payloads taken over the measurement interval;
              values are rounded to 2^n  */
  uint32    var_payload;

}tdsmac_log_tvm_meas_info_type;


#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  /*! Uplink transport channel type
              0:DCH; 1:RACH;2:USCH (not used)
              RACH is the currently configured default in the uplink. 
              For 1.28 Mcps TDD, if E-DCH is configured to the UE, 
              and the Uplink transport channel type is USCH and UL
              Transport Channel identity is 32, it means E-DCH TVM is configured */

  uint8 trch_type;

  /*! Uplink transport channel identity range 1-32  */
  uint8 trch_id;
  

}tdsmac_log_meas_obj_type;


#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  /*! Event ID 
                0:4a; 1:4b  */
  uint8 event_id;

  /*! Reporting Threshold
              Integer(0..25) to enumerate (8,16,32,64,128,256,512,1024,2K,3K,4K,
              6K,8K,12K,16K,24K,32K,48K,64K,96K,128K,192K,256K,384K,512K,768K); 
              Threshold in bytes and N Kbytes = N*1024 bytes */

  uint8 report_threshold;
  
  /*! Time to trigger
            Integer(0..15) to enumerate (0,10, 20, 40,60, 80, 100,120, 160,200, 
            240,320, 640,1280, 2560,5000) ms */
  uint8 time_to_trigger;

  /*! Pending time after trigger 
                Integer(0..6) to enumerate (250,500,1000,2000, 4000,8000,16000)ms */
  uint8 pending_time;

  /*! Tx interruption after trigger
              Integer(0..6) to enumerate (250,500,1000,2000, 4000,8000,16000) ms */
  uint8 tx_intrupt;
  

}tdsmac_log_meas_trch_event_info_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  /*! Uplink transport channel type
              0:DCH; 1:RACH;2:USCH (not used)
              RACH is the currently configured default in the uplink. 
              For 1.28 Mcps TDD, if E-DCH is configured to the UE, 
              and the Uplink transport channel type is USCH and UL
              Transport Channel identity is 32, it means E-DCH TVM is configured */

  uint8 trch_type;
  /*! Uplink transport channel identity 
                range 1-32  */
  uint8 trch_id;
  /*! number of event range1-2 */
  uint8 num_event;
  tdsmac_log_meas_trch_event_info_type  trch_event_info;// expand the struct here
  
}tdsmac_log_meas_event_info_type;
#ifdef __GNUC__
 typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
  /*! Amount of reporting
              Integer (0..7) to enumerate (1, 2, 4, 8, 16, 32, 64, Infinity); 
              The default value is infinity */
  uint8 reporting_amt;

  /*! Reporting interval
              Integer (0..15) to enumerate (0,250,500,1000,2000,3000,4000,6000,8000,12000,
              16000,20000,24000,28000,32000,64000); 
              ‘0’ means no periodical reporting     */
  uint8 reporting_interval;
}tdsmac_log_meas_periodic_info_type;
 

/*! TDSCDMA_UL_MAC_CFG_LOG_PACKET */

typedef PACK(struct)
{
    /*!  \brief Log header */
    log_hdr_type   hdr;
    /*!  \brief Version number */
    uint8 version_num;
    /*!  \brief Number of UL Transport channels ,range 1-8 */
    uint8           num_trch;  

    /*!  \brief Ciphering enabled
                  True  – Enabled
                  False – Disabled  */
    boolean         ciphering_flag;

    /*!  \brief Ciphering Key ID, values 0, 1, 2, 3  */
    uint8           ciphering_key_id;

    /*!  \brief Hyperframe Number, 20 bits  */
    uint32          hfn;

    /*!  \brief Transport Channel Information */
    tdsmac_log_trch_cfg_info_type       trch_info[TDSUE_MAX_TRCH];

}TDSCDMA_UL_MAC_CFG_LOG_PACKET_type;


/*! TDSCDMA_DL_MAC_CFG_LOG_PACKET */

typedef PACK(struct)
{
    /*!  \brief Log header */
    log_hdr_type   hdr;
    /*!  \brief Version number */
    uint8 version_num;
    /*!  \brief Number of UL Transport channels, range 1-8 */
    uint8           num_trch;  

    /*!  \brief  Ciphering enabled
               0 - no
               1 - yes */
    boolean         ciphering_flag;

    /*!  \brief Ciphering Key ID - 0,1,2,3 */
    uint8           ciphering_key_id;

    /*!  \brief  Hyperframe Number, 20 bits */
    uint32          hfn;

    /*!  \brief Transport Channel Information */
    tdsmac_log_trch_cfg_info_type       trch_info[TDSUE_MAX_TRCH];

}TDSCDMA_DL_MAC_CFG_LOG_PACKET_type;

/*! TDSCDMA_MAC_RACH_CFG_LOG_PACKET */


typedef PACK(struct)
{
    /*!  \brief Log header */
    log_hdr_type   hdr;
    /*!  \brief Version number */
    uint8 version_num;

    /*!  \brief Maximum Number of RACH Preamble Cycles, range 1-32 */
    uint8               max_num_preamble_cycles;  

    /*!  \brief Minimum amount of Backoff time in TTIs, range 0-50 */
    uint8               min_backoff;

    /*!  \brief Maximum amount of Backoff time in TTIs, range 0-50 */
    uint8               max_backoff;

    /*!  \brief Number of Access Service Classes, max8 */
    uint8               num_asc;

    /*!  \brief Access Service Class information */
    tdsmac_log_asc_type    asc_info[MAX_ASC];

}TDSCDMA_MAC_RACH_CFG_LOG_PACKET_type;

/*!
                   TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET
 */

typedef PACK(struct)
{
  /*!  \brief Log header */
  log_hdr_type   hdr;
  /*!  \brief Version number */
    uint8 version_num;
    /*!  \brief Number of Transport Format Combinations range 0-127 */
    uint16  num_tfc;

    /*!  \brief Number of Transport Channels range 1-8 */
    uint8   num_trch;

    /*!  \brief Transport channel IDs range 1-32 */
    uint8   trch_ids[TDSUE_MAX_TRCH];

    /*!  \brief Transport Channel TTI info
                 0 - 10 msec;
                 1 - 20 msec;
                 2 - 40 msec;
                 3 - 80 msec    */
    uint8   tti_types[TDSUE_MAX_TRCH];

    /*!  \brief Transport Format Combination info */
    tdsmac_log_tfc_type    tfc_info[TDSUE_MAX_TFC];

}TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type;


/*! TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET */

typedef PACK(struct)
{
  /*!  \brief Log header */
  log_hdr_type   hdr;
  /*!  \brief Log packet version number */
  uint8 version_num;

  /*!  \brief  Measurement identity range 1-16  */
  uint8     meas_id;

  /*!  \brief 0-Periodic, 1-Event Triggered */
  uint8     report_mode;

  /*!  \brief 0- Event_4A, 1-Event_4B */
  uint8     event_id;

  /*!  \brief 0-RACH, 1-DCH */
  uint8     trch_type;

  /*!  \brief trch id range 1-32   */
  uint8     trch_id;

  /*!  \brief  In periodic mode, represents the number of reports sent */
  uint16    num_reports_sent;

  /*!  \brief  Number of radio bearers range 1-32 */
  uint8    num_rbs;
  
  /*!  \brief  Measurement results per radio bearer */
  tdsmac_log_tvm_meas_info_type    meas_result_per_rb[TDS_MAX_RB];

}TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type;


/*!
TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET
*/
typedef PACK(struct)
{
  /*!  \brief Log header */
  log_hdr_type   hdr;
  /*!  \brief  Log packet version number */
  uint8 version_num;

  /*!  \brief  Measurement identity range 1-16 */
  uint8     meas_id;

  /*! Measurement action
              0-Start doing traffic volume measurements     
              1-Modify an existing measurement              
              2-Delete all measurements associated with the measurement identity                        
              3-Deletes all the exisiting measurement ids   
              4-Save the measurement info 
              5-Resume measurement    */

  uint8     meas_action;

  /*! Measurement quantity type
              0-raw buffer payload 
              1-Average buffer payload 
              2-Variance of rlc buffer payload 
              0xFF-Not indicated   */
  uint8 meas_quant_type;

  /*! Measure time interval
              Integer (20, 40, ..260) ms by steps of 20ms 
              0xFFF -not indicated  */
  uint8 time_interval;

  /*! Measurement Report Quantity type
              Bitmask Bits 
              0 (LSB) –If set  Indicates raw rlc payload measurement information in the report
              1 – If set indicates average payload measurement information in the report
              2- If set inidictaes variance payload measurement information in the report
              Rest of the bits are reserved for future use */
  uint8 meas_report_quan_type;

  /*! Transfer Mode
                0-Ack Mode
                1-Unack Mode  */
  uint8 trans_mode;

  /*! report mode
                0-No Report
                1-Peridic
                2-Event Mode  */
 uint8 report_mode;

 /*! number of trCH 
       range 1-32
       0XFF indicates applies to all UL trch */
 uint8 num_trch;
 
 /*! NO. of trch event range 1-2 */
 uint8 num_trch_event;
 tdsmac_log_meas_periodic_info_type periodic_info;
 tdsmac_log_meas_obj_type meas_obj[TDSUE_MAX_TRCH];


 
 tdsmac_log_meas_event_info_type   event_info[TDSUE_MAX_TRCH];


}TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type;

      
/*!  \brief Uplink MAC config */
#define TDSMAC_UL_CFG_PACKET_LEN(num_trch, num_logchan) ( \
  FPOS(TDSCDMA_UL_MAC_CFG_LOG_PACKET_type, trch_info)+ \
      (num_trch * (sizeof(uint8) + sizeof(uint8) + \
       sizeof(uint8)))+ \
      ( num_logchan * sizeof(tdsmac_log_lc_cfg_type))  \
  )

/*!  \brief Downlink MAC config */
#define TDSMAC_DL_CFG_PACKET_LEN(num_trch, num_logchan) ( \
  FPOS(TDSCDMA_DL_MAC_CFG_LOG_PACKET_type, trch_info)+ \
      (num_trch * (sizeof(uint8) + sizeof(uint8) + \
       sizeof(uint8)))+ \
      ( num_logchan * sizeof(tdsmac_log_lc_cfg_type))  \
  )
      
/*!  \brief RACH config */      
#define TDSMAC_RACH_CFG_PACKET_LEN(num_asc) ( \
  FPOS(TDSCDMA_MAC_RACH_CFG_LOG_PACKET_type, asc_info)+ \
      (num_asc * sizeof(tdsmac_log_asc_type)) \
  )

/*!  \brief TFCS config */
#define TDSMAC_UL_TFCS_CFG_PACKET_LEN(num_tfc, num_trch)(\
  FPOS(TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type, trch_ids) +\
      (num_trch * sizeof(uint8)) + \
      (num_trch * sizeof(uint8)) + \
      (num_tfc * (sizeof(uint8) + (num_trch * sizeof(uint8)))) \
  )

/*!  \brief Traffic volume report packet */
#define TDSMAC_UL_TVM_REPORT_PACKET_LEN(num_rb)(\
  FPOS(TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type, meas_result_per_rb) +\
  (num_rb*(sizeof(uint32)+sizeof(uint32)+sizeof(uint32)+sizeof(uint32)))) 



/*!  \brief TVM Perdic Meas, Amount of reporting, infinity */
#define TDSMAC_UL_TVM_PACKET_PERDIC_RPT_TIMES_INFINITY   7 

/*!  \brief TVM Perdic Meas, kind number  of reporting times : 1, 2, 4, 8, 16, 32, 64 */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_TIMES    7

/*!  \brief TVM Perdic Meas, kind number  of Reporting interval : 
0,250,500,1000,2000,3000,4000,6000,8000,12000,
16000,20000,24000,28000,32000,64000 ms */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_INTERVAL 16

/*!  \brief TVM Meas, kind number  of Reporting Threshold : 
8,16,32,64,128,256,512,1024,2K,3K,4K,6K,8K,12K,16K,24K,32K,48K,64K,9
6K,128K,192K,256K,384K,512K,768K  */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_REPORT_THRESHOLD    26

/*!  \brief TVM Meas, kind number  of length of trigger timer : 
0,10, 20, 40,60, 80, 100,120, 160,200, 240,320, 640,1280, 2560,5000 ms  */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_TIMER_TO_TRIGGER    16

/*!  \brief TVM Meas, kind number  of length of pending timer : 
250,500,1000,2000, 4000,8000,16000 ms  */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_PROHIBIT_TMR        7

/*!  \brief TVM Meas, kind number  of length of interruption timer : 
250,500,1000,2000, 4000,8000,16000 ms  */
#define TDSMAC_UL_TVM_PACKET_MAX_NUM_TX_INTERRUPT_TMR    7

/*!  \brief Uplink transport channel type
0:DCH; 1:RACH;2:USCH (not used)
RACH is the currently configured default in the uplink. 
For 1.28 Mcps TDD, if E-DCH is configured to the UE, and the Uplink transport channel type
is USCH and UL Transport Channel identity is 32, it means E-DCH TVM is configured.
*/

#define TDSMAC_UL_TVM_PACKET_TRCH_TYPE_DCH  			0
#define TDSMAC_UL_TVM_PACKET_TRCH_TYPE_RACH 			1
#define TDSMAC_UL_TVM_PACKET_TRCH_TYPE_USCH 			3

/*!  \brief ID of EDCH  */
#define TDSMAC_UL_TVM_PACKET_EDCH_ID        			32

/*!  \brief EVENT ID 0:4a; 1:4b */
#define TDSMAC_UL_TVM_PACKET_EVENT_4A        			0
#define TDSMAC_UL_TVM_PACKET_EVENT_4B        			1

/*!  \brief  Meas Quantity type 
0-	raw buffer payload 
1-	Average buffer payload 
2-	Variance of rlc buffer payload 
0xFF-Not indicated 
*/
#define TDSMAC_UL_TVM_PACKET_MASK_PAYLOAD       		0
#define TDSMAC_UL_TVM_PACKET_MASK_AVG_PAYLOAD   		1
#define TDSMAC_UL_TVM_PACKET_MASK_VAR_PAYLOAD   		2
#define TDSMAC_UL_TVM_CFG_LOG_NO_INDICATE  		0xFF
/*!  \brief
num_trch : 0xFF
indicates applies to all UL trch
*/
#define TDSMAC_UL_TVM_CFG_LOG_ALL_TRCH     		0xFF



#define TDSMAC_LOG_VERSION                              1


#ifdef T_WINNT
  #error code not present
#endif



#endif
