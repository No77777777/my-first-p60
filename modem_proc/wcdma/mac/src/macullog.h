#ifndef MACULLOG_H
#define MACULLOG_H


/*===========================================================================
               U P L I N K   M A C    L O G G I N G   H E A D E R    F I L E

DESCRIPTION
  This file provides the function prototypes for MAC uplink logging.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2001 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002, 2008-2009 Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macullog.h_v   1.1   24 Aug 2001 15:30:52   rchar  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macullog.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
04/12/16  kv      FR 32607: Web Boosting to improve latency by initiating TVM so that NW can move UE to DCH
10/21/15   vr     Compilation error fixes for AT.
09/28/15   as     MAC to delay event 6D trigger by limiting the ETFCI selection in UL inorder to avoid 
                  UE hitting MTPL due to data traffic.
07/22/15   kv     FR 22377: RAVE-API
03/18/15   as     Added new NV item 73667 to control FR22545 changes.
03/03/15   as     SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
                  based upon L1 operating power and delay 6D trigger by limiting number of bits to be transmitted.
05/01/15  scv     W+W Changes to have log packets support SubID
04/25/15  scv     W+W Changes to have messages support SubID
04/10/15  kv      Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
04/08/15  as      L2-RRC interdependent changes for W+W
03/19/15  aa      Changes for new log packet 0x41CE
01/29/15  kc      L2 global reorg for W+W
08/07/13  kc      Log packet support for UL MAC cipher parameters
10/12/12  kc      HS_RACH MAC F3's moved under debug variable and x4314 log packet issue
01/02/12  grk     Feature cleanup
05/19/10  mrg     Header guard "MACULLOG_H" typo error corrected 
05/07/09  ssg     Updated Copyright Information
04/29/09  av      Fixed compiler warnings in L2 
04/23/09  mrg     Function mac_ul_event_amr_rate() is removed.
10/27/08  ssg     Added function mac_ul_event_amr_rate()
11/15/02  sk      Added function mac_ul_send_tvm_report_packet():
08/24/01  rc      Moved FEATURE definition to after the include definitions.
08/21/01  rc      Created file.

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "maclog.h"
#include "macrrcif_v.h"
#include "macinternal.h"

typedef enum
{
  MAC_DEBUG_NONE = 0,
  MAC_R99_ENABLE_DEBUG_F3S = 1,
  MAC_EUL_ENABLE_DEBUG_F3S = 2,
  MAC_DL_DEBUG_PRINT_INTERVAL = 3, 
  MAC_UL_DEBUG_PRINT_INTERVAL = 4,
  MAC_HS_LOGGING = 5, 
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
  MAC_EHS_LOGGING = 6, 
  MAC_EHS_P_TO_C_LOGGING = 7,
#endif
  MAC_TVM_LOGGING = 8,
  MAC_EUL_EVENT_6D_SUP_THRSHLD = 9,
  MAC_EUL_EVENT_6D_CONSEC_TTI  = 10,
  MAC_EUL_EVENT_6D_SKIP_COUNT    = 11,
  MAC_EUL_EVENT_6D_REINIT_COUNT  = 12,
 MAC_EUL_EVENT_6D_SUP_REDUCTION  = 13
#ifdef FEATURE_DATA_RAVE_SUPPORT 
  , MAC_EUL_ENABLE_RAVE_AT_W     = 14
  , MAC_EUL_SET_RAVE_PARAMS      = 15
#endif  
  , MAC_TVM_SET_E4B_PROHIBIT_CNT = 16
}wcdma_mac_debug_var_type;


/*===========================================================================

FUNCTION MAC_UL_SEND_CFG_LOG_PACKET

DESCRIPTION
   This function prepares a log packet for configured Uplink MAC Transport 
   Channels and sends the packet to log services.  

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void mac_ul_send_cfg_log_packet
(
  mac_ul_config_type **config_ptr,   /* Address of Ptr to Uplink MAC config */
  mac_ul_table_type *ul_table_ptr
);

/*===========================================================================

FUNCTION MAC_UL_SEND_RACH_PARAM_PACKET

DESCRIPTION
   This function prepares a log packet for RACH Transmit parameters and sends
   the packet to log services.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void mac_ul_send_rach_param_packet
(
  mac_ul_rach_txcntl_config_type *rach_tx_prmtrs, /* Ptr to RACH TX params  */
  mac_ul_table_type *ul_table_ptr
);

/*===========================================================================

FUNCTION MAC_UL_SEND_TFCS_CFG_PACKET

DESCRIPTION
   This function prepares a log packet for Uplink MAC TFCS configuration 
   and sends the packet to log services.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void mac_ul_send_tfcs_cfg_packet
(
  mac_ul_tfcs_type *tfcs_ptr,             /* Ptr to TFCS info              */
  uint8            *trch_id,              /* Transport Channel IDs         */
  mac_ul_table_type *ul_table_ptr
);
/*===========================================================================

FUNCTION mac_ul_send_tvm_report_packet

DESCRIPTION
   This function prepares a log packet for Uplink MAC Traffic volume report 
   information and sends the packet to log services.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void mac_ul_send_tvm_report_packet
(
  mac_ul_traffic_meas_ind_type *tvm_rpt_ptr,
  uint16    nreports_sent,
  mac_ul_table_type *ul_table_ptr
);

/*===========================================================================

FUNCTION MAC_UL_SEND_CFN_LOG_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void mac_ul_send_cfn_log_packet
(
  uint8 nsamples, mac_ul_table_type *ul_table_ptr
);

/*===========================================================================

FUNCTION MAC_UL_SEND_CIPHER_CONFIG_LOG_PACKET

DESCRIPTION
  Logs ciphering parameters for TM channels at MAC

===========================================================================*/
void mac_ul_send_cipher_config_log_packet(mac_cipher_config_type *cipher_ptr, mac_ul_table_type *ul_table_ptr);

/*===========================================================================

FUNCTION MAC_UL_CONVERT_TVM_BUF_PAYLOAD

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
uint32 mac_ul_convert_tvm_buf_payload
    (
    uint32    payload
    );

/*===========================================================================

FUNCTION wcdma_mac_set_dbg_var_through_qxdm

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
ex. commands
send_data 75 4 46 0 1 <val>   - for mac_enable_tfci_debug_dump
send_data 75 4 46 0 2 <val>   - for mac_enable_low_medium_f3s
send_data 75 4 46 0 3 <val>   - for mac_dl_debug_print_interval
send_data 75 4 46 0 4 <val>   - for mac_ul_debug_print_interval
send_data 75 4 46 0 5 <val>   - for mac_hs_logging
send_data 75 4 46 0 6 <val>   - for mac_ehs_logging
send_data 75 4 46 0 7 <val>   - for mac_ehs_p_to_c_logging
send_data 75 4 46 0 8 <val>   - for mac_hs_rach_f3_enable
send_data 75 4 46 0 9 <val>   - for mac_tvm logging

<val> - 32 bit value of debug variable
for providing hex value append val with 0x
===========================================================================*/
void wcdma_mac_set_dbg_var_through_qxdm
(
  sys_modem_as_id_e_type as_id,
  uint8 debug_var_type,
  uint32 debug_var_val
);

#endif /* MACULLOG_H */
