#ifndef MACINTERNAL_H
#define MACINTERNAL_H

/*==================================================================

   MAC LAYER - INTERNAL DATA H E A D E R    F I L E

DESCRIPTION
  This file contains the definition of the data types that are
  internally used used by MAC subsystem software.

  This file is included only in MAC subsystem modules.

  Copyright (c) 2001 by Qualcomm Technologies, Inc.
  All Rights Reserved.
  Copyright (c) 2002-2005, 2007-2009 by Qualcomm Technologies, Inc.All Rights Reserved.

====================================================================*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macinternal.h_v   1.21   10 Jul 2002 12:50:28   skrishna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/inc/macinternal.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------
04/12/16   kv      FR 32607: Web Boosting to improve latency by initiating TVM so that NW can move UE to DCH
07/22/15   kv      FR 22377: RAVE-API
06/26/15   aa      Removing extern declarations from L2 code
06/01/15   scv     QSH: Changes to support SYNC_OPTIONAL for multiple metric config
06/01/15   scv     Code changes to support QSH
12/15/14   as      In FACH state MAC should send CMAC_STATUS_IND only for TM CCCH UL data.
05/18/15   as      Added new NV item 73667 to control FR22545 changes.
03/03/15   as      SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
                   based upon L1 operating power and delay 6D trigger by limiting number of bits to be transmitted.
04/25/15   scv     W+W Changes to have messages support SubID
04/10/15   kv      Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
04/08/15   as      W+W MVS L2 new interface changes.
03/19/15   aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
02/23/15   kv      Enable RxD after back-off timer expired and turn on RxD after persistency passed
02/20/15   aa      Mainlined FEATURE_MAC_QXDM_LOGGING
01/29/15   aa      L2 global reorg for W+W
05/28/13   kc      Change for MRM-ID extension for Rel-9
10/12/12   ts      Reordering of structure elements for memory optimization
07/13/12   grk     MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
08/13/12   mrg     HS-RACH TVM issues (Multiple TVMs sent , 4A/4B back to backcontinous reported)
                   1)Multiple unnecessary Events 4A-4B triggered are avoidede now
                   2)When Prohibit timer is running Timer_To_Trigger is not running – So it will delay TVM event triggering by PTT+TTT -fixed
                   3)During DCH<->RACH/FACH transition TRCH are added with our checking for TRCH type 
01/02/12   grk     Feature cleanup
12/15/10   vp      Fixed compiler warning
10/01/10   mrg     TVM event and periodic measurements handled with different timers
09/16/10   mrg     Moved auido(MVS) dependency under the feature flag FEATURE_MVS
10/07/09   kvk     Fixed featurization for WPLT MAC compilation
06/18/09   mrg     lowest_amr_rate_tfci variable is added to mac_amr_mode_info_type 
                   to store the TFCI index for which lowest possible AMR rate allowed 
05/29/09   ssg     Added support for SNOW_3G algo.
05/07/09   ssg     Updated Copyright Information
04/03/08   sk      Renamed FEATURE_MBMS to FEATURE_MODEM_MBMS
03/24/08   pj      Added mac_id to structure dlc_dch_config_info_type.
01/29/08   sk      Support for MBMS under FEATURE_MODEM_MBMS
09/05/07   sk      Added the pdu_size in the internal data structure under FEATURE_RLC_ENH_BUFFER_STATUS
07/19/05   sk      Added structure mac_amr_mode_info_type to store the lowest_amr_rate,the
                   highest amr rate that is available based on the TFCS table.Added
                   mac_amr_rate_e_type enum to indicate if it is a single rate, multi rate AMR call.
02/15/05   gsc     Added HSDPA support under FEATURE_HSDPA feature flag.
12/30/04   sk      Fixed lint errors
09/28/04   sk      Added marked to field.
09/19/04   sk      Added mac_ciphering_table which will maintain for each cfn the key, the
                   cipher_enable flag and the algorithm
07/24/04   sk      Added evt_not_first_meas to maci_ul_traffic_trch_event_param. This variable
                   will be associated with each particular transport channel.
07/16/04   sk      Added tfcs_control_ptr_in_use_e_type enum under FEATURE_TFCS_CONTROL_ENABLE
04/14/04   sk      Added a structure maci_ul_trch_tf_type.
12/26/03   sk      Added support for TFCS selection based on logical channel prioritites
                   under FEATURE_TFCS_LC_PRIORITIES
04/29/03   sk      Added uecomdef_logchan_e_type. This will save the channel type for DCH. Its under
                   FEATURE_TC_CONSTANT_DELAY.
02/13/03   sk      Changed num_events to num_events[UE_MAX_TRCH] in maci_ul_traffic_cfg_param.
12/31/02   sk      Added power_state, tblock to ul_tfcs_info_type under FEATURE_PWR_TFC_ELMN
12/24/02   sk      Added tfc_sig_bo and tfc_data_bo to ul_tfcs_info_type.
11/15/02   sk      Removed cfg_ind from maci_ul_traffic_cfg_param struct
09/26/02   sk      Added suspend_meas to maci_ul_traffic_cfg_index_type.
09/18/02   sk      Added RACH_WAITING_FOR_RIGHT_TTI to rach_state_e_type
09/04/02   sk      * ul_trch_info_type structure: Added boolean cs_or_ps_domain to indicate
                   if this transport channel belongs to CS or the PS domain.
                   * dl_dch_config_info_type structure : Added boolean cs_or_ps_domain to indicate
                   if this transport channel belongs to CS or the PS domain.
07/10/02   sk      Changed the size of report_threshold from uint16 to uint32
06/08/02   sk      Removed FEATURE_TRAFFIC_VOLUME_MEAS.
05/20/02   sk      Added enum traffic_meas_cfg_e_type for traffic volume measurements.
05/10/02   sk      Added data structures maci_ul_traffic_cfg_index_type,
                   maci_ul_traffic_cfg_param, maci_ul_traffic_trch_event_param
                   to support traffic volume measurements. The additions
                   are made under FEATURE_TRAFFIC_VOLUME_MEAS
11/03/01   ps      Added "highest_tti" field to "int_ul_tfcs_info" structure
09/19/01   ps      Added "ntb" field to "ul_trch_info_type"

=====================================================================*/

#include "wcdma_variation.h"
#include        "uecomdef.h"
#include "macrrcif.h"
#include "l1macdata.h"
#include "wmac_qsh_ext.h"

/*===================================================================

                     CONSTATNT DATA DEFINITIONS

====================================================================*/

/* Minimum value of DL CCTrCh ID */
#define     MAC_DL_MIN_CCTRCH_ID        0

/* Maximum value of DL CCTrCh ID */
#define     MAC_DL_MAX_CCTRCH_ID        (UE_MAX_CCTRCH + MAC_DL_MIN_CCTRCH_ID - 1)

/* Minimum value of DL TrCh ID */
#define     MAC_DL_MIN_TRCH_ID          1

/* Maximum value of DL TrCh ID */
#define     MAC_DL_MAX_TRCH_ID          32

/* Minimum value of UL TrCh ID */
#define     MAC_UL_MIN_TRCH_ID          1

/* Maximum value of UL TrCh ID */
#define     MAC_UL_MAX_TRCH_ID          32

/* Minimum value of DL RLC ID */
#define     MAC_DL_MIN_RLC_ID           0

/* Maximum value of DL RLC ID */
#define     MAC_DL_MAX_RLC_ID           (UE_MAX_DL_LOGICAL_CHANNEL + MAC_DL_MIN_RLC_ID - 1)

/* Minimum value of UL RLC ID */
#define     MAC_UL_MIN_RLC_ID           0

/* Maximum value of UL RLC ID */
#define     MAC_UL_MAX_RLC_ID           (UE_MAX_UL_LOGICAL_CHANNEL + MAC_UL_MIN_RLC_ID - 1)

/* Minimum value of DL Radio Bearer ID */
#define     MAC_DL_MIN_RB_ID            0

/* Maximum value of DL Radio Bearer ID */
#define     MAC_DL_MAX_RB_ID            (MAX_RB + MAC_DL_MIN_RB_ID - 1)

/* Minimum value of UL Radio Bearer ID */
#define     MAC_UL_MIN_RB_ID            0

/* Maximum value of UL Radio Bearer ID */
#define     MAC_UL_MAX_RB_ID            (MAX_RB + MAC_UL_MIN_RB_ID - 1)

/* Minimum value of MAC ID of DCCH/DTCH */
#define         MAC_MIN_MAC_ID          1

/* Maximum value of MAC ID of DCCH/DTCH */
#define         MAC_MAX_MAC_ID          15

/* Minimum value of UL Access Service Class ID */
#define         MAC_UL_MIN_ASC_ID       0

/* MAximum value of UL Access Service Class ID */
#define         MAC_UL_MAX_ASC_ID       7

/* Minimum value of UL logical channel priotity */
#define         MAC_UL_MIN_MLP_ID       1

/* MAximum value of UL logical channel priority */
#define         MAC_UL_MAX_MLP_ID       8

/* Maximum number of MAC logical channel priorities */
#define         MAC_UL_MAX_MLPS         (MAC_UL_MAX_MLP_ID - MAC_UL_MIN_MLP_ID + 1)

/* Maximum size of uplink MAC PDU header */
#define         MAC_UL_MAX_HDR_SIZE     5

/* Minimum value of DL transport block ID */
#define         MAC_DL_MIN_TB_ID        0

/* Maximum value of DL transport block ID */
#define         MAC_DL_MAX_TB_ID        (UE_MAX_DL_RLC_PDUS + MAC_DL_MIN_TB_ID - 1)

/* Maximum number of traffic volume measurements */
#define     MAC_UL_MAX_MEAS_ID          32

/* Minimum measurement identity that a traffic volume measurement can have */
#define     MAC_UL_MIN_MEAS_ID          1

#define     MAC_UL_TRAFFIC_VOLUME_TMR_BASE     10

/* Number of traffic volume measurements that MAC supports */
#define     MAC_UL_NUM_TRAFFIC_MEAS     6

/* Number of traffic volume measurements that can be present at a time */
#define     UE_NUM_TRAFFIC_MEAS                (MAC_UL_MAX_MEAS_ID-MAC_UL_MIN_MEAS_ID+1 )

#define     TRAFFIC_MAX_SAMPLES     26

/*The periodic timer reset value can be set using this multiplication factor */
#ifdef FEATURE_WCDMA_TVM_PERIODIC_TIMER_ENH
#error code not present
#else
#define     MAC_UL_PERIODIC_TVM_TIMER_SCALE        1
#endif

#ifdef FEATURE_WCDMA_PWR_TFC_ELMN
#define TFC_PWR_STATE_BITMAP_SIZE (L1_UL_TFC_MAX/32) 
#endif

#define RLC_NUM_PROFILE_SAMPLES 30
#define MAC_NUM_PROFILE_SAMPLES 60

#ifdef FEATURE_TC_CONSTANT_DELAY
#define MAX_NUM_CFN 10
#endif

/* Log Cipher Parameter packet every 40 samples */
#define MAC_UL_MAX_CIPHER_LOG_CNT 40

/*============================================================================================

                     ENUMERATED TYPE DATA DEFINITIONS

=============================================================================================*/

/*--------------------------------------------------------------------------------------------
 RACH State
--------------------------------------------------------------------------------------------*/
typedef enum
{
  RACH_INVALID_STATE= 0,
  RACH_IN_IDLE,
  RACH_WAITING_FOR_STATUS,
  RACH_RCVD_NOACK,
  RACH_IN_BACKOFF,
  RACH_PERSISTENCE_FAILED,
  RACH_WAITING_FOR_RIGHT_TTI
} rach_state_e_type;


typedef enum
{
  CFG_INVALID=0,
  CFG_COMPLETE,
  CFG_INCOMPLETE
} traffic_meas_cfg_e_type;

#ifdef FEATURE_WCDMA_PWR_TFC_ELMN

typedef enum
{
  SUPPORTED_STATE=0,
  EXCESS_PWR_STATE,
  BLOCKED_STATE
} tfc_power_state_e_type;

#endif


typedef enum
{
  FULL_TFCS_TABLE=0,
  DEFAULT_TFCC_TABLE,
  CURRENT_TFCC_TABLE
}tfcs_control_ptr_in_use_e_type;


#ifdef FEATURE_WCDMA_MULTIRATE_AMR

typedef enum
{
  SINGLE_RATE_AMR=0,
  MULTI_RATE_AMR,
  AMR_RATE_UNDEFINED
}mac_amr_rate_e_type;

#endif

#if defined( FEATURE_WCDMA_DC_HSUPA ) && defined(FEATURE_WCDMA_EVT6D_ENH)
typedef enum
{
  EVT6D_OPT_DISABLE = 0,
  EVT6D_OPT_ETFCI_BASED_LOGIC = 1,
  EVT6D_OPT_POWER_BASED_LOGIC = 3,
  EVT6D_OPT_INVALID
} mac_evt_6d_opt_e_type;
#endif
/*============================================================================================

                     STRUCTURE TYPE DATA DEFINITIONS

============================================================================================*/
/*---------------------------------------------------------------------
 DL Transport Block Information

 During pass 1 (header decoding), MAC determnines and saves the RLC ID
 of each transport block in this table, Uses the table information
 during pass 2 (data indication)
----------------------------------------------------------------------*/
typedef struct
{
  /* TRUE indicates the TB as valid */
  /* FALSE otherwise */
  boolean                 valid;

  /* DL RLC logical channel ID */
  rlc_lc_id_type  rlc_id;

}dl_tb_info_type;

/*---------------------------------------------------------------------
 Transport Channel ID to Transport Channel Index Table
 ----------------------------------------------------------------------*/
typedef struct
{
  /* TRUE     indicates that this transport channel is valid */
  boolean                     valid;

  /* Transport Channel Index */
  uint8                       idx;

}trch_id_to_idx_map_type;

/*---------------------------------------------------------------------
 BCH CONFIG TABLE

 The CCTrCh config info received from RRC is validated and stored in
 this structure. The information is used during header decoding
----------------------------------------------------------------------*/
typedef struct
{
  /* TRUE             if transport channel is enabled */
  /* FALSE    otherwise */
  boolean                     bch_enable;

  /* RLC ID of the logical channel to which this TrCh is mapped */
  rlc_lc_id_type      bcch_id;

}bch_config_info_type;

/*---------------------------------------------------------------------
 PCH CONFIG TABLE

 The CCTrCh config info received from RRC is validated and stored in
 this structure. This information is used during header decoding.
----------------------------------------------------------------------*/
typedef struct
{
  /* TRUE if the transport channel is enabled */
  /* FALSE otherwise */
  boolean                     pch_enable;

  /* RLC ID of the logical channel to which this TrCh is mapped */
  rlc_lc_id_type      pcch_id;

} pch_config_info_type;

/*---------------------------------------------------------------------
 Dedicated Logical Channel mapped to DSCH

 The info of DSCH-Dedicated Logical Channels received from RRC is
 validated and stored in this structure.
----------------------------------------------------------------------*/
typedef struct
{
  /* DSCH Enable Flag */
  boolean                     enable;

  /* RLC ID */
  rlc_lc_id_type      rlc_id;

  /* Radio Bearer ID */
  rb_id_type          rb_id;

  /* RLC Mode */
  uecomdef_logch_mode_e_type          rlc_mode;

}dlc_dsch_config_info_type;

/*---------------------------------------------------------------------
 DSCH CONFIG TABLE

 The Transport Channel config info received from RRC is validated and
 stored in this structure.
----------------------------------------------------------------------*/
typedef struct
{
  /* CCTrCh ID */
  cctrch_id_type       cctrch_id;

  /* Transport channel ID */
  tr_ch_id_type   trch_id;

  /* Number of dedicated logical channels mapped */
  uint8                        num_dlc;

  /* Dedicated logical channel configuration */
  dlc_dsch_config_info_type    dlcinfo[MAX_DED_LOGCHAN_PER_TRCH];

}dsch_config_info_type;

/*---------------------------------------------------------------------
 Config info of dedicated logical channel mapped to DL DCH
----------------------------------------------------------------------*/

typedef struct
{
  /* TRUE         if the DLC is enabled */
  /* FALSE        otherwise */
  boolean    enable;

  /* logical channel id received from RRC/NW <1..15>
   - valid for HS only */
  uint8      mac_id;

  /* RLC ID */
  rlc_lc_id_type      rlc_id;

  /* RAdio bearer ID */
  rb_id_type          rb_id;

  /* RLC mode */
  uecomdef_logch_mode_e_type  rlc_mode;

//#ifdef FEATURE_TC_CONSTANT_DELAY
  /* DCCH or DTCH */
  uecomdef_logchan_e_type chan_type;
//#endif

}dlc_dch_config_info_type;

/*---------------------------------------------------------------------
 DCH CONFIG TABLE

 The Transport Channel config info received from RRC is validated and
 stored in this structure.
----------------------------------------------------------------------*/
typedef struct
{
  /* CCTrCh ID */
  cctrch_id_type       cctrch_id;

  /* Transport channel ID */
  tr_ch_id_type   trch_id;

  uint8   mac_d_flow_id;

  /* Number of DCCH/DTCHs */
  uint8                        num_dlc;

  /* Dedicated logical channel configuration */
  dlc_dch_config_info_type    dlcinfo[MAX_DED_LOGCHAN_PER_TRCH];

  /* 0(False) indicates it is CS domain, 1(TRUE) indicates it is PS domain */
  boolean    cs_or_ps_domain;

}dl_dch_config_info_type;
/*---------------------------------------------------------------------
 Config info of dedicated logical channel mapped to FACH
----------------------------------------------------------------------*/
typedef struct
{
  /* TRUE if dedicated logical channel is enabled */
  /* FALSE otherwise */
  boolean              enable;

  /* Dedicated logical channel ID */
  rlc_lc_id_type               rlc_id;

}dlc_fach_config_info_type;

/*---------------------------------------------------------------------
 FACH CONFIG TABLE

 The Transport channel config info received from RRC is validated and
 stored in this structure.
----------------------------------------------------------------------*/
typedef struct
{
  /* ID of the CCTrCH to which this transport channel mapped */
  cctrch_id_type       cctrch_id;

  /* Transport channel ID */
  tr_ch_id_type   trch_id;

  /* TRUE if BCCH is enabled */
  /* FALSE otherwise */
  boolean                      bcch_enable;

  /* BCCH ID */
  rlc_lc_id_type       bcch_id;

  /* TRUE if CCCH is enabled */
  /* FALSE otherwise */
  boolean                      ccch_enable;

  /* CCCH ID */
  rlc_lc_id_type       ccch_id;

  /* TRUE if CTCH is enabled */
  /* FALSE otherwise */
  boolean                      ctch_enable;

  /* CTCH ID */
  rlc_lc_id_type       ctch_id;

  /* TRUE if there is at least one DCCH/DTCH */
  /* FALSE otherwise */
  boolean                      dlc_enable;

  /* Number of DCCH/DTCHs */
  uint8                        num_dlc;

  /* Dedicated logical channel info */
  dlc_fach_config_info_type     dlcinfo[MAX_DED_LOGCHAN_PER_TRCH];

}fach_config_info_type;

/*----------------------------------------------------------------------------------------*/
/* UL TFCS information used internally by MAC                                                                                     */
/*----------------------------------------------------------------------------------------*/
typedef struct
{
  

  /* TFC Buffer Occupancy */
  uint32                    tfc_bo;

  /* TFC buffer occupancy contributed by data on CCCH & DCCH */
  uint32                    tfc_sig_bo;

  /* TFC buffer occupancy contributed by data on DTCH */
  uint32                    tfc_data_bo;

  /* This TFC was */
  boolean         marked[UE_MAX_TRCH];

  /* TFC bo at this priority level */
  uint32          tfc_priority_bo[MAC_UL_MAX_MLPS];

  /* This denotes the number of bits avaialble for this TFC */
  uint16          tfc_priority_ntb[MAC_UL_MAX_MLPS];

  /* This denotes the still available space for each transport channel
  in this TFC */
  uint16          trch_available_ntb[UE_MAX_TRCH];

#ifdef FEATURE_WCDMA_PWR_TFC_ELMN
  /*Timer in MAC will be based on the build frame indication that MAC gets from L1.
  This timer will keep count of the time this TFC is in Excess power state and will
  be used to transition to Blocked state if needed. */
  uint16                          tblock;
   /*Indicates if the TFC is in Supported, Excess or Blocked state.*/
  tfc_power_state_e_type          power_state;

#endif
/* TFC Control Flag -- TFC_ALLOWED or TFC_NOT_ALLOWED */
  mac_ul_tfc_control_e_type control_flag;
  /* Highest TTI in this TFC */
  l1_tti_enum_type          highest_tti;

}ul_tfcs_info_type;

/*--------------------------------------------------------------------------------
For each transpport channel, this structure would indicate the allowed tf's for this
channel. and the number of pdus that can be built. This will later be used to
eliminate tfc's when npdus is les than the tb_size for a particular transport
channel adn a particular transport format.
---------------------------------------------------------------------------------*/
typedef struct
{
  uint16  ntblks;
  uint16  tb_size;
  uint16  npdus;
  boolean allowed;

}maci_ul_trch_tf_type;

/*----------------------------------------------------------------------------------------*/
/* UL Transport Channel information                                                                                                               */
/*----------------------------------------------------------------------------------------*/
typedef struct
{
  /* Number of logical channels */
  /* This value is set on receiving CMAC_UL_CONFIG_REQ */
  uint8                           nlchan;

  /* Transport channel ID */
  tr_ch_id_type           trch_id;

  /* Logical Channel IDs */
  /* This value is set on receiving CMAC_UL_CONFIG_REQ */
  rlc_lc_id_type          rlc_id[UE_MAX_UL_LOGICAL_CHANNEL];

  /* TFI of the previous TTI boundary */
  /* This value is cleared on receiving CMAC_UL_CONFIG_REQ */
  uint8                           prev_tfi;

  /* Flag indicating whether data was transmitted on previous TTI boundary or not */
  boolean                         prev_tx_flag;

  /* Number of actual Transport Blocks in TTI */
  uint8               ntb;

  /* Indicates if the domain is CS or PS */
  boolean   cs_or_ps_domain;

}ul_trch_info_type;

/*----------------------------------------------------------------------------------------*/
/* UL Coded Composite Transport Channel information                                                                               */
/*----------------------------------------------------------------------------------------*/
typedef struct
{
  /* CCTrCh Type */
  mac_ul_cctrch_e_type    cctrch_type;

  /* Number of Transport Channels */
  /* This value =1 if the CCTrCh type = RACH */
  /* This value can be any value between 1 and Maximum Number of Transport Channels */
  uint8   ntrch;

  /* Selected TFCI */
  uint16  selected_tfci;

}ul_cctrch_info_type;

/*------------------------------------------------------------------*/
/* UL Logical channel information                                   */
/*------------------------------------------------------------------*/
typedef struct
{

  /* TRUE indicates logical channel is enabled */
  boolean                                         enable;

  /* Logical channel type */
  uecomdef_logchan_e_type         lc_type;

  /* MAC Header size */
  uint8                                           mac_hdr_size;

  /* MAC header value */
  uint8                                           mac_hdr_value[MAC_UL_MAX_HDR_SIZE];

  /* RLC mode */
  uecomdef_logch_mode_e_type      rlc_mode;

  /* Radio bearer identity */
  rb_id_type                                      rb_id;

  /* Transport Channel Index */
  uint8                                           trch_idx;

  /* Access Service Class (in case of CCCH) or */
  /* MAC Logical Channel Priority (in case of DCCH/DTCH) */
  uint8                                           asc_or_mlp;

  /* RLC Header Length in bits */
  uint8                                           rlc_hdr_length;

  /* Lists the rlc sizes that are valid for this logical channel */
  mac_ul_rlc_size_info_type                      rlc_size_info;

  uint32                                         pdu_size;

}ul_lc_info_type;

typedef struct
{
  /* the event could be either event_4a or event_4b */
  mac_ul_traffic_event_id_e_type  event_id;

  boolean   evt_not_first_meas;
  tr_ch_id_type   first_event_trch_id;

  /* The previous value of buffer occupancy */
  uint32    prev_bo;

  /* threshold in bytes (8,16,32,64,...) */
  uint32    report_threshold;

  /* Indicates the period of time in ms during which the event has to be satisfied for*/
  /* MAC to trigger a report */
  uint16    time_to_trigger;

  /* Indicates the period of time during which it is forbidden to send any new report */
  /* with the same traffic volume event identity even if the triggering condition is  */
  /* fulfilled */
  uint16    trigger_prohibit_tmr;

  /* Indicates how long the UE shall block DTCH transmissions on the RACH after a   */
  /* measurement report is triggered */
  uint16    tx_interrupt_tmr;

  /* Counter to keep track of Time to trigger*/
  uint16    trigger_tmr_cnt;

  /*Counter to keep track of Trigger prohibit timer */
  uint16    prohibit_tmr_cnt;

}maci_ul_traffic_trch_event_param;

/*-------------------------------------------------------------------------------*/
/* MAC stores the config information corresponding to each measurement id        */
/*-------------------------------------------------------------------------------*/

typedef struct
{
  /* False indicates apply measurement for all uplink transport channels */
  /* True indicates that apply measurement only for those specified in   */
  /* the object list */
  boolean                                       meas_obj_list_incl;

  /* Number of transport channels */
  uint16                                        num_trch;

  /* Transport channel id and type information for each of these transport channels */
  mac_ul_traffic_trch_info_type                 trch_info[UE_MAX_TRCH];

  /* If set to true include raw rlc payload measurement information in the report*/
  boolean                                       rlc_payload;

  /* If set to true include average payload measurement information in the report*/
  boolean                                       avg_payload;

  /* If set to true include variance payload measurement information in the report*/
  boolean                                       var_payload;

  /* If the reporting quantity is avergae or variance then the interval to take the average
  or variance is given by meas_interval */
  uint16                                        nsamples;

  /* Contains the report mode-indicates if it is periodic/event_triggered*/
  mac_ul_traffic_report_mode_e_type             report_mode;

  /* Contains the report transfer mode*/
  mac_ul_traffic_report_transfer_mode_e_type    transfer_mode;

  /* first meas is set to true initially. TRue indicates this is the
  first time a measurement report is going to be posted for this meas id */
  boolean                                       first_meas;

  /*-----------------------------------------------------------------------------------
      The following parameters are used when the mode is periodic
  -----------------------------------------------------------------------------------*/
  /* True indicates there is no limit on the number of reports that we send to RRC    */
  /* False indicates,send only the number of reports that are specified in num_reports*/
  boolean                                       report_infinity;

  /* The number of reports that can be sent */
  uint16                                        num_reports;

  /* Indicates in milliseconds the interval of periodic report */
  uint16                                        report_interval;


  /* In the case of periodic if there is a limit on number of reports that we   */
  /* need to send then num_reports_sent will tell us how many reports have been */
  /* sent so far                                                                */
  uint16                                        nreports_sent;

  /* This counter will keep track if the timer has expired */
  uint32                                        periodic_tmr_cnt;

  /*-----------------------------------------------------------------------------------
      The following parameters are used when the mode is event_triggered.
  -----------------------------------------------------------------------------------*/
  boolean                                       event_trch_incl;

  uint16                                        event_num_trch;

  /* Transport channel id and type information for each of these transport channels */
  mac_ul_traffic_trch_info_type                 event_trch_info[UE_MAX_TRCH];

  /* Number of events for each transport channel that is specified in event_trch_info*/
  uint16                                        num_events[UE_MAX_TRCH];

  /* Event paramaeters for each of these transport channels */
  maci_ul_traffic_trch_event_param              trch_event_param[UE_MAX_TRCH][2];

  /* Stores the event_id and the transport channel on which the event occurred */
  mac_ul_traffic_event_result                   event_result;

  /* Stores the index from which samples have to be taken for computing the raw, avg, variance*/
  uint16                                        current_idx;

}maci_ul_traffic_cfg_param;

typedef struct
{
  /* True indicates if this entry has valid parameters in it */
  boolean                     valid;

  /* TRUE indicates that the measurements are temporarily suspended.
  FALSE indicates there is no such restraint on this measurement id */
  boolean                     suspend_meas;

  /* Pointer to the config information */
  maci_ul_traffic_cfg_param   *traffic_cfg_ptr;

}maci_ul_traffic_cfg_index_type;


typedef struct
{

  traffic_meas_cfg_e_type  cfg_e_type;

  /* Number of transport channels */
  uint16                            ntrch;

  /* Transport channel type and id for each of these transport channels*/
  mac_ul_traffic_trch_info_type     trch_info[UE_MAX_TRCH];

}maci_ul_traffic_obj_list_cfg_type;

/*-----------------------------------------------------------------------------
For a particular transport channel with a particular priority, this
structure gives the list of the logical channels mapped to this transport
channel, the rlc size buffer occupancy and the total buffer occupancy
information is stored.
-----------------------------------------------------------------------------*/

typedef struct
{
  uint8           num_lc;
  rlc_lc_id_type  lc_id[UE_MAX_UL_LOGICAL_CHANNEL];
  uint32          rlc_size_bo[UE_MAX_TF];
  uint32          total_bo;
  /* This flag indicates if pdus were picked up from this logical
  channel */
  uint8           start_idx;

}maci_ul_trch_priority_info_type;

typedef struct
{
  /* The ciphering algorithm can be either UE_0 or UE_1 */
  uecomdef_wcdma_cipher_algo_e_type     cipher_algo;

  /* Cipher Enable flag */
  /* TRUE if ciphering is enabled */
  /* FALSE otherwise */
  boolean     cipher_enable;

  /* Ciphering key index */
  uint8       key_idx;

}mac_ciphering_table;

#ifdef FEATURE_WCDMA_MULTIRATE_AMR

typedef struct
{
  /* This is the rate that MVS will be configured with */
  uint32  amr_rate;

  /* Among the tfci's that are given this is the lowest possible rate. In case
  of multirate amr call this rate will be put in the minimum set */
  uint32  lowest_amr_rate;

  /* TFCI index for which lowest possible AMR rate allowed */
  uint16    lowest_amr_rate_tfci;
  
  /* Enum value indicates if it is SINGLE_RATE, MULTI_RATE or UNDEFINED. */
  mac_amr_rate_e_type   amr_rate_type;

}mac_amr_mode_info_type;

#endif

#ifdef FEATURE_TC_CONSTANT_DELAY
typedef struct{
 boolean first_tb; /* True Indicates if this is the first transport block that is received */
 uint16   dl_cfn[MAX_NUM_CFN]; /* Downlink CFN */
 uint8   delay;     /* The constant delay which is applied to every transport block.*/
 uint8   cur_idx;   /* Index where the downlink cfn is stored */
 uint8   start_idx; /* Index where the Uplink task starts reading */
}lb_delay_struct_type;
#endif

typedef struct
{
  sys_modem_as_id_e_type as_id;
   /*-----------------------------------------------------------------
   Mark the cfn at which some data on a DCH transport channel is
   received which needs to be looped back.So if there are transport blocks
   received on multiple tranport channels in the same tti then note
   the cfn only once. This flag will ensure that the cfn is marked only
   once.
   -----------------------------------------------------------------*/
#ifdef FEATURE_TC_CONSTANT_DELAY
   boolean     cfn_marked;
   lb_delay_struct_type  lb_delay_type;
#endif

  /* TRUE implies MAC received a new configuration parameters from RRC and waiting    */
  /* L1 signal to actually configure. FALSE implies there are no pending configuration*/
  /* requests now */
  boolean dl_configuration_pending;

  /* TRUE indicates that DL MAC CRNTI is valid, FALSE otherwise */
  boolean     dl_mac_crnti_valid;
  
  /* TRUE indicates that DL MAC URNTI is valid, FALSE otherwise */
  boolean     dl_mac_urnti_valid;
  
  /* Number of active FACHs */
  uint8 num_active_fachs;

  /* Number of active DSCHs */
  uint8 num_active_dschs;

  /* Number of active DL DCHs */
  uint8    num_active_dl_dchs;
  
  /* DL MAC CRNTI value */
  uint16      dl_mac_crnti;
  
  /* DL MAC URNTI value */
  uint32      dl_mac_urnti;  

  /* Dowlink transport block information table */
  dl_tb_info_type       dl_tb_info[UE_MAX_DL_RLC_PDUS];
  
  /* Transport Channel ID to Index Mapping Table */
  trch_id_to_idx_map_type   dl_dch_id_to_idx[MAX_TRCH];
  trch_id_to_idx_map_type   dsch_id_to_idx[MAX_TRCH];
  trch_id_to_idx_map_type   fach_id_to_idx[2*MAX_TRCH];

  /* BCH Internal Configuration Database */
  bch_config_info_type    bch_cfg_info[UE_MAX_CCTRCH];
  pch_config_info_type    pch_cfg_info[UE_MAX_CCTRCH];
  dl_dch_config_info_type   dl_dch_cfg_info[UE_MAX_TRCH];
  dsch_config_info_type   dsch_cfg_info[UE_MAX_TRCH];
  fach_config_info_type   fach_cfg_info[UE_MAX_TRCH];
  
  /* MAC dl config information */
  mac_dl_config_type  dl_cfginfo;
  
  /* Ordered ciphering config data */
  mac_cipher_config_type    cur_dl_cipher_info[MAX_CN_DOMAIN];
  mac_cipher_config_type    new_dl_cipher_info[MAX_CN_DOMAIN];

  /* Poking this to TRUE will provide messages to see whereciphering went wrong */  
  boolean ciphering_garbled;  

  /* Once the ciphering_grabled message is set to TRUE the debug print messages
  would be printed every 500 ms. */
  uint8 mac_dl_debug_print_interval;

  uint8 prev_print_cfn;

}mac_dl_table_type;

/* MAC_UL_BUILD_FRAME_IND command */
typedef struct
{
    /* current tti */
    l1_tti_enum_type    tti;

    /* current CFN */
    uint8               cfn;

    boolean             srb_omit;

    /* pointer to Tx data buffer */
    l1_ul_tx_data_type  *tx_buf_ptr;

}mac_ul_build_frame_ind_type;

/*-----------------------------------------------------------------------
  This struct is added to implement Rel-5 Spec CR 2272. While in
  the middle of ciphering configuration and prior to reaching the activation
  time, if MAC receives a new ciphering config indicated to MAC by RRC
  as SMC_INITIATED_NON_HO_CIPHER_CFG, MAC will save the ciphering config
  and apply the config when the pending config's activation time has reached.
-----------------------------------------------------------------------*/
typedef struct {
  mac_cipher_config_type  cipher_info;
  boolean                 cipher_info_saved;
}mac_saved_cipher_info_type;

extern mac_dl_table_type mac_dl_table[];
#define     GET_MAC_DL_TABLE_PTR(as_id)    (&mac_dl_table[as_id])

typedef struct {

  boolean                   mac_increment_hfn_on_rollover[MAX_CN_DOMAIN];

  /* TRUE indicates that there is a ciphering configuration that is pending*/
  boolean                   ul_cipher_activation_pending[MAX_CN_DOMAIN]; 

  mac_ciphering_table       cfn_ciphering_table[256];
  
  mac_cipher_config_type    cur_ul_cipher_info[MAX_CN_DOMAIN];   /* Ordered ciphering config data */
  
  /* Before activation time, ciphering config received from RRC is stored in this structure */
  mac_cipher_config_type    new_ul_cipher_info[MAX_CN_DOMAIN];

  uint16                    prev_cfn; /* Save the prev cfn value sent by physical layer every TTI */

  uint32                    count_c[MAX_CN_DOMAIN];   /* Count C value for each of the domain */

  uint32                    cfn_hfn_table[MAX_CN_DOMAIN][256];   /* Look up table for finding the hfn associated with the cfn */
  
  /* Indicates MAC that it needs to get a new value of HFN from RRC on reaching the activation time for the first time */
  boolean                   get_start_value_to_transmit[MAX_CN_DOMAIN];
  
  boolean                   prev_mac_increment_hfn_on_rollover[MAX_CN_DOMAIN];
  
  mac_ciphering_table       prev_cfn_ciphering_table[256];
  
  mac_cipher_config_type    prev_cur_ul_cipher_info[MAX_CN_DOMAIN];   /* Ordered ciphering config data */
  
  mac_cipher_config_type    prev_new_ul_cipher_info[MAX_CN_DOMAIN];   /* Before activation time, cipher config received from RRC is stored here */
  
  boolean                   prev_ul_cipher_activation_pending[MAX_CN_DOMAIN];  /* TRUE - ciphering configuration is pending*/
  
  uint16                    old_prev_cfn;   /* Save the prev cfn value sent by physical layer every TTI */
  
  uint32                    prev_count_c[MAX_CN_DOMAIN];   /* Count C value for each of the domain */
  
  uint32                    prev_cfn_hfn_table[MAX_CN_DOMAIN][256];   /* Table that keeps track of the HFN associated with the cfn */
  
  boolean                   prev_get_start_value_to_transmit[MAX_CN_DOMAIN];
  
  mac_saved_cipher_info_type saved_new_ul_cipher_info[MAX_CN_DOMAIN];  /*  Implementation of Rel-5 spec CR 2272. */
}mac_ul_cipher_table;

typedef struct
{
  boolean   mac_ul_rach_abrt_rcvd;
  boolean   data_present_on_srb;

  uint8     npreamble_cycles; /* Number of RACH Preamble Cycles */
  
  uint16    backoff_val; /* Backoff value */
  uint16    num_tx_tbs; /* Number of Transport Blocks transmitted on RACH */ 

 #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  uint32    num_tti_consec_without_data;
 #endif   

  rach_state_e_type rach_state; /* RACH State */
  rach_state_e_type hs_rach_state; /* HS-RACH State */  
  boolean mac_rach_ccch_data_presence;
  
  mac_ul_rach_cctrch_config_type    *rach_cfg_ptr; /* Pointer to ordered config data of RACH */

  mac_ul_rach_cctrch_config_type    rach_cfg_info;
  
  l1_ul_tx_data_type    *l1_tx_buf_ptr; /* Layer 1 Transmit Data Buffer Pointer */
  
  /* Holds contents of L1 Tx buffer used by RLC to pend status if CTRL PDUs are trashed during RACH_ABORT */
  l1_ul_tx_data_type mac_rach_tx_buf; 
  
  mac_ul_rach_txcntl_config_type    rach_cntl_info; /* Ordered RACH transmit control parameters */  
}mac_rach_table_type;

typedef struct
{
 boolean mac_priority_empty[MAC_UL_MAX_MLPS];
#ifdef FEATURE_WCDMA_MRAB_TVM_MAC_OPT
 #error code not present
#endif
 
 /* This indicates the minimum transport block size for each transport channel. THis will be used for computing
 the exact buffer status for UM for traffic volume measurements. In case of UM, the PDU sizes can vary. We
 will pick up the minimum size corresponding to the transport channel to which the LC is mapped to and use the
 minimum TB size for computation of headers. For AM PDU size is fixed and is configured by RRC to RLC 
 Hence this field will NOT be used for AM. TVM is not done for TM channels. */
 uint32    trch_min_tb_size[UE_MAX_TRCH];
 uint32    trch_rlc_size_sig_bo[UE_MAX_TRCH][UE_MAX_TF]; /* TrCh buffer occupancies based on rlc sizes */
 uint32    trch_rlc_size_data_bo[UE_MAX_TRCH][UE_MAX_TF]; /* TrCh buffer occupancies based on rlc sizes */
 
 l1_tti_enum_type         current_tti; /* Current TTI */
 mac_rab_status_e_type    rab_status;
 mac_ul_tfc_control_e_type        mac_ul_full_tfcs_control_table[UE_MAX_TFC];
 mac_ul_tfc_control_e_type        mac_ul_default_tfcs_control_table[UE_MAX_TFC];
 mac_ul_tfc_control_e_type        mac_ul_current_tfcs_control_table[UE_MAX_TFC];
 
 mac_ul_tfcs_type    *oc_ul_tfcs_ptr; /* Pointer to ordered TFCS data */
 
#ifdef FEATURE_TC_CONSTANT_DELAY
   l1_tti_enum_type    highest_trch_tti;
#endif
 
#ifdef FEATURE_TFCS_CM_BY_HLS
   l1_tti_enum_type    min_trch_tti;
#endif
 
#ifndef FEATURE_WCDMA_MAC_UL_ZI_REDUCTION
   ul_tfcs_info_type   int_ul_tfcs_info[UE_MAX_TFC]; /* UL TFCS Info */
#else
   #error code not present
#endif /* FEATURE_WCDMA_MAC_UL_ZI_REDUCTION */
  
   uint32 mac_mvs_codec_info;
 
#ifdef FEATURE_WCDMA_PWR_TFC_ELMN
   uint16 tnotify, tmodify, tadapt, tl1proc;
 /*Bitmap to store TFC Power State info
   1 - TFC is in blocked state
   0 - TFC is in supported state */
   uint32 tfc_pwr_state_bitmap[TFC_PWR_STATE_BITMAP_SIZE];
#endif
 
 mac_ul_tfcs_type                 mac_ul_tfcs_table;
}mac_ul_tfcs_info_type;

typedef struct {
  uint32 countc;
  uint8  cfn;
  uint8 key_idx;
}mac_ul_cipher_log_pkt_type;

typedef struct
{
  int32 frame_qty_start;
  int32 cx8_qty_start;
  int32 frame_qty_end;
  int32 cx8_qty_end;
  uint32 add_on1;
  uint32 add_on2;
  uint16 add_on3;
}l2_rlc_profile_data_param_type;

typedef struct
{
  uint32 dump_idx;
  l2_rlc_profile_data_param_type profile_array[RLC_NUM_PROFILE_SAMPLES];
}l2_rlc_profile_data_struct_type;

typedef struct
{
int32 frame_qty_start;
int32 cx8_qty_start;
int32 frame_qty_end;
int32 cx8_qty_end;
}l2_profile_data_struct_type;

#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
typedef struct
{
  boolean lb_configured_flag;
  uint16  loop_back_constant_delay;
  uint32  ul_cfn;
  uint16  lb_trch_tti_info;
}rlc_ul_lb_lc_info_type;
#endif

#ifdef FEATURE_WCDMA_DS_WEB_BOOST
typedef struct
{
  boolean  frc_trig_tvm;             /* TRUE indicates that force Trigger of TVM is initated by DS */
  
  boolean  is_e4a_cnf;               /* Indicates if e4a is configured by NW                       */

  uint16   max_prohibit_e4b;         /* Configured value to prohibit e4b after e4a is force trig   */

  uint16   prohibit_e4b_cnt;         /* Instantaneous time until which e4b is prohibited           */

  uint32   max_cnf_thrshld;          /* Max threshold stored for e4a                               */
}mac_frc_trig_tvm_type;
#endif

typedef struct
{
  boolean             mac_enable_amr_debug_dump;
  uint8               mac_ul_debug_print_interval; /* The debug messages in UL will be printed every 500 ms. */
  mac_ul_cipher_log_pkt_type cipher_log[MAC_UL_MAX_CIPHER_LOG_CNT];
  uint16                     cipher_log_idx;
  l2_rlc_profile_data_struct_type mac_build_frame_profile_dump;
  l2_rlc_profile_data_struct_type phy_build_frame_profile_dump;
  l2_rlc_profile_data_struct_type rlc_tfc_profile_dump;

  /* Profiling samples variables to capture the snapshot of the times stamps */
  l2_profile_data_struct_type l2_profile_dump[MAC_NUM_PROFILE_SAMPLES];
  boolean l2_print_profile_data;
  uint32 l2_profile_idx;
  int32 frame_qty, cx8_qty;
  uint8 mac_r99_enable_debug_f3s;
#ifdef FEATURE_WCDMA_MRAB_TVM_MAC_OPT  
  #error code not present
#endif
}mac_ul_debug_ino_type;

/* Dummy Signaling information data type */
typedef struct dummy_signaling_info
{
  /* Dummy signaling flag */
  boolean dummy_sig_active;
    
  /* DCH Transport channel ID that is mapped to a DCCH */
  uint16 dummy_dch_idx;

} dummy_signaling_info_type;

typedef struct
{
 /*The number of traffic volume measurements so far */
 uint16  num_event_measurements;
 uint16  num_periodic_measurements;
 
 uint32              mac_multirab_opt_bitmask;
 
 boolean mac_tvm_event_timer_started;
 boolean mac_tvm_periodic_timer_started;
 
 
 /*This is the index at which the raw_rlc_payload information for the rb's */
 /* in the current timeout would be placed */
 uint16  cur_index;
 
 /* Traffic volume measurement Config table */
 maci_ul_traffic_cfg_index_type   ul_traffic_info_table[UE_NUM_TRAFFIC_MEAS];
 
 /* Traffic volume timer*/
 rex_timer_type                   mac_ul_tvm_event_tmr_blk;
 rex_timer_type                   mac_ul_tvm_periodic_tmr_blk;
 
 uint16                           tvm_debug;
 
 /* This array contains the raw payload information for all the radio bearers*/
 uint32                           rb_raw_payload[MAX_RB][TRAFFIC_MAX_SAMPLES];
 
 /* TRUE indicates that the Tx interruption timer is active currently. FALSE*/
 /* indicates that the DTCH transmissions on RACH are not blocked now.*/
 boolean                          block_transmission;
 
#ifdef FEATURE_WCDMA_DS_WEB_BOOST
 mac_frc_trig_tvm_type            trig_tvm_params;
#endif
 
 /* Measurement id which caused the blocking of DTCH transmissions on RACH*/
 uint16                           tx_interrupt_tmr;
 
 /* Counter to keep track of the Tx interruption timer */
 uint16                           tx_interrupt_cnt;
 
 /* This will indicate if the circular buffer has rolled over */
 boolean                          rolled_over;
 
 /* This structure would contain the results of theresults for a particular
 measurement identity at a time */
 mac_ul_traffic_meas_ind_type    meas_ind;
 
 boolean prepare_addtl_meas_incl;

}mac_traffic_info_type;

/* RAVE params for R-99 */
#ifdef FEATURE_DATA_RAVE_SUPPORT 
typedef struct
{ 
  uint32   total_capacity;         /* Max capacity possible based on tfci allowed */
}mac_ul_r99_rave_param_type;
#endif

typedef struct
{
  sys_modem_as_id_e_type as_id;
  uint8   mac_tm_bearers_count;
  
  ul_cctrch_info_type       int_ul_cctrch_info;   /* Uplink CCTrCh info internally used by MAC */

  /* this array holds indeices where transport channel info for that transport channel id is stored */
  trch_id_to_idx_map_type ul_trch_id_to_idx_table[MAX_TRCH];

  /* Uplink Logical Channel info table internally used by MAC */
  ul_lc_info_type         int_ul_lc_info[UE_MAX_UL_LOGICAL_CHANNEL];
  
  /* UL Transport Channel Info table internally used by MAC */
  ul_trch_info_type       int_ul_trch_info[UE_MAX_TRCH];  

  /* Holds info of TrChs and LogChs mapped to it at various priority levels */
#ifndef FEATURE_WCDMA_MAC_UL_ZI_REDUCTION
  maci_ul_trch_priority_info_type int_ul_trch_priority_info[UE_MAX_TRCH][MAC_UL_MAX_MLPS];
#else
  #error code not present
#endif /* FEATURE_WCDMA_MAC_UL_ZI_REDUCTION */
  
  /* Holds info as to the number of pdus that can be built for a particular TrCh with a particular transport format */
  maci_ul_trch_tf_type int_ul_trch_tf_info[UE_MAX_TRCH][UE_MAX_TF];

  l1_prach_status_enum_type   rach_tx_status[2];   /* Double buffer - PRACH transmit status */
  
#ifdef FEATURE_WCDMA_HS_RACH
  l1_hsrach_status_enum_type   hs_rach_tx_status[2];   /* Double buffer - HS-RACH transmit status */
#endif

  mac_ul_build_frame_ind_type   bld_frame_ind_info;   /* UL Frame Build Info */

  mac_ul_config_type    ul_cfginfo; /* The configuration parameters are stored in here */

#ifdef FEATURE_TC_CONSTANT_DELAY  
  mac_loop_back_mode_e_type loopback_test_mode; /* Indicates if the loopback test mode is 1, 2 or invalid*/
  uint8  cur_ul_cfn; /* Notes the uplink cfn */
  rlc_ul_lb_lc_info_type   mac_loop_back_lc_info[UE_MAX_UL_TM_CHANNEL];
#endif

  boolean     ul_mac_crnti_valid; /* TRUE if the C-RNTI is valid. FALSE otherwise */
  
  uint16      ul_mac_crnti;   /* C-RNTI value */

  /* Flag to denote that MAC is still building previous request */
  boolean mac_build_pending;

  boolean mac_ul_config_in_progress;
  
  /* TRUE  - MAC received a new configuration parameters from RRC and waiting L1 signal to actually configure. 
     FALSE - There are no pending configuration requests now */
  boolean               ul_configuration_pending;

#ifdef FEATURE_WCDMA_PWR_TFC_ELMN
  boolean       tfc_excess_pwr_state[UE_MAX_TFC];  
#endif

  boolean       tfc_rlc_size_for_dch_defined;  

#ifdef FEATURE_TFCS_CM_BY_HLS
    L1_UL_QUERY_TFCS_RESTRICT_CB_FUNC_TYPE *ptr_to_l1_fn;
#endif

  mac_ul_tfc_control_e_type               mac_ul_pending_tfcs_control_table[UE_MAX_TFC];
  boolean                                 mac_ul_tfcc_config_pending;
  boolean                                 mac_ul_tfcc_config_in_process;
  uint16                                  mac_ul_tfcc_activation_cfn;
  uint16                                  mac_ul_tfcs_pending_control_duration;
  rex_timer_type                          ul_tfc_ctrl_tmr_blk;

  dummy_signaling_info_type   int_dummy_sig_info;  

  uint8             data_ptr[80];   /* Pointer to the TB data */

  boolean mac_rach_rxd_enable;

  mac_ul_cipher_table mac_cipher_info;

  mac_rach_table_type rach_info;

  mac_ul_tfcs_info_type tfcs_info;

  mac_traffic_info_type traffic_info;

  mac_ul_debug_ino_type debug_info;

  wcdma_icommon_event_callback_fn_t amr_event_cb;

#ifdef FEATURE_DATA_RAVE_SUPPORT 
  mac_ul_r99_rave_param_type   mac_ul_r99_rave_info;
#endif
}mac_ul_table_type;

extern mac_ul_table_type mac_ul_table[];
#define     GET_MAC_UL_TABLE_PTR(as_id)    (&mac_ul_table[as_id])

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
typedef enum {
  WMAC_QSH_START_METRIC,
  WMAC_QSH_STOP_METRIC,
  WMAC_QSH_LOG_METRIC,
  WMAC_QSH_NO_OP
}wmac_qsh_metric_action_enum_type;

typedef struct 
{
  uint32 mac_r99ul_bo_bytes;
  uint32 num_r99_bo_qery;
 }wmac_qsh_metric_r99_statictics;

typedef struct 
{
  uint8  num_happy_bit_ind_pri;
  uint8  num_happy_bit_ind_sec;
  uint8  num_si_sent_pri;
  uint8  num_si_sent_sec;
  uint32 mac_eul_bo_bytes;
  uint32 num_eul_bo_query;
}wmac_qsh_metric_eul_statictics;

typedef boolean wmac_metric_cb_type(sys_modem_as_id_e_type as_id,wmac_qsh_metric_e metric_id, void* metric_addr,  wmac_qsh_metric_action_enum_type action);

typedef struct
{
  /*address of the metric*/
  void* metric_addr;
  void* metric_context_id;
}wmac_metric_addr_info;

typedef struct
{
  /*corresponding cb to call*/
  wmac_metric_cb_type* metric_cb_ptr;
  wmac_metric_addr_info metric_addr_info[WCDMA_NUM_SUBS];
}wmac_qsh_metric_db_struct_type;

extern wmac_qsh_metric_eul_statictics wmac_qsh_metric_eul_stat;
extern wmac_qsh_metric_r99_statictics wmac_qsh_metric_r99_stat[];
#endif

#ifdef FEATURE_WCDMA_QSH_DUMP 
typedef void wmac_dump_cb_type(uint8 as_id,void* dump_ptr);
typedef struct
{
  /*address of the dump*/
  uint32 dump_struct_size;
  /*corresponding cb to call*/
  wmac_dump_cb_type* dump_cb_ptr;
}wmac_qsh_dump_db_struct_type;

/*===========================================================================
FUNCTION WMAC_QSH_DUMP_R99_DL_INFO

DESCRIPTION Function collects MAC R99 DL config to the address specified 
by QSH.
===========================================================================*/
void wmac_qsh_dump_r99_dl_info(uint8 as_id,void *write_dump_addr);

/*===========================================================================
FUNCTION WMAC_QSH_DUMP_R99_UL_INFO

DESCRIPTION Function collects MAC R99 UL config to the address specified by QSH.
===========================================================================*/
void wmac_qsh_dump_r99_ul_info(uint8 as_id,void *write_dump_addr);

/*===========================================================================
FUNCTION WMAC_QSH_DUMP_DL_HS_INFO

DESCRIPTION Function collects MAC DL HS config and pending TSN info  
to the address specified by QSH.
===========================================================================*/
void wmac_qsh_dump_dl_hs_info(uint8 as_id,void *write_dump_addr);

/*===========================================================================
FUNCTION WMAC_QSH_DUMP_EUL_INFO

DESCRIPTION Function collects MAC UL HS config and MAC FLOW-ID info 
to the address specified by QSH.
===========================================================================*/
void wmac_qsh_dump_eul_info(uint8 as_id,void *write_dump_addr);
#endif

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/*===========================================================================
FUNCTION WMAC_QSH_EUL_STAT_METRIC

DESCRIPTION Function triggers metric start/stop or updates the MAC metric 
collection for R99 metric details to the address specified by QSH.
===========================================================================*/
boolean wmac_qsh_eul_stat_metric(sys_modem_as_id_e_type as_id,wmac_qsh_metric_e metric_id, void* metric_addr,  wmac_qsh_metric_action_enum_type action);

/*===========================================================================
FUNCTION WMAC_QSH_R99_STAT_METRIC

DESCRIPTION Function triggers metric start/stop or updates the MAC metric 
collection for R99 metric details to the address specified by QSH.
===========================================================================*/
boolean wmac_qsh_r99_stat_metric(sys_modem_as_id_e_type as_id,wmac_qsh_metric_e metric_id, void* metric_addr,  wmac_qsh_metric_action_enum_type action);

/*===========================================================================
FUNCTION WMAC_QSH_PROC_METRIC_COMMAND

DESCRIPTION Function triggers metric start/stop or updates the MAC metric 
collection for the requested METRIC ID('s) to the address specified by QSH.
===========================================================================*/
void wmac_qsh_proc_metric_command(qsh_client_cb_params_s* wmac_qsh_metric_cfg,sys_modem_as_id_e_type as_id);
#endif

#ifdef FEATURE_WCDMA_HS_RACH
void  mac_hsrach_stop_tebs0_timer_dl_data_recevied(sys_modem_as_id_e_type as_id);
#endif /* FEATURE_WCDMA_HS_RACH */

#endif // macinternal_h

