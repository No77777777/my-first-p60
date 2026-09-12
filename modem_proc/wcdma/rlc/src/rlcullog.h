#ifndef RLCULLOG_H
#define RLCULLOG_H

/*===========================================================================
               U P L I N K   R L C    L O G G I N G   H E A D E R    F I L E

DESCRIPTION
  This file provides the function prototypes for RLC uplink logging.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2001 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002-2006, 2008-2009 Qualcomm Technologies, Inc.  
All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcullog.h_v   1.5   12 Jun 2002 18:44:38   tliou  $
$Header: //components/rel/wcdma.mpss/8.4.0/rlc/src/rlcullog.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
06/05/15  scv     Fix Compilation Error
05/22/15  kv      PDU log packet enahncements in UL and DL (CR - 746106)
05/20/15  aa      W+W and new interface changes for CFCM
04/25/15  scv     W+W Changes to have messages support SubID
04/08/15  as      L2-RRC interdependent changes for W+W
03/04/15  aa      Replaced RLC UM/AM/TM config log packets with new packets
01/29/15  aa      L2 global reorg for W+W
08/23/12  mrg     MAC-iis: Special HE functionality with LI 7 configured and 
                  RLC PDU size greater than 126 octets
02/01/12  grk     Feature cleanup
05/11/09  ssg     Updated Copyright Information
07/25/08  ssg     Added parameter to rlc_ul_tx_am_nak_pdu_log_pkt().
11/23/06  vg      added support for UM DL/UL pdu logging under feature FEATURE_UM_PDU_LOGGING
12/09/05  aw      Obsoleted the RLC ul v1 am statistics log.
09/15/05  aw      Added prototype for funtion rlc_ul_tx_v2_am_statistic_pkt
                  and macros for RLC duplicate transmissions.
04/05/05  aw      Removed Log_on_demand feature.
08/19/04  mv      Separated the RLC PDU logging for the user plane and 
                  signaling plane logical channels.
12/06/03  ttl     Declared rlc_ul_tx_pdu_cipher_log_pkt().
11/05/02  ttl     Added logging for ciphering config.
06/02/02  ttl     Added rlc_ul_tx_am_nak_pdu_log_pkt().
04/24/02  ttl     Rewrote the statistic log code.
04/03/02  ttl     Added Log_on_demand feature.
03/15/02  sk      Added function definition rlc_ul_send_am_pdu_log_packet,
                  rlc_ul_send_am_stat_packet.
08/24/01  rc      Moved FEATURE definition to after the include definitions.
08/20/01  rc      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "wcdma_variation.h"
#include "rlclog.h"
#include "rlcrrcif_v.h"
#include "rlcul.h"

#ifdef FEATURE_RLC_QXDM_LOGGING

#ifndef TRANSMITTED_THREE_TIMES
#define TRANSMITTED_THREE_TIMES             3
#endif
#ifndef TRANSMITTED_FOUR_TIMES
#define TRANSMITTED_FOUR_TIMES              4
#endif
#ifndef TRANSMITTED_FIVE_TIMES
#define TRANSMITTED_FIVE_TIMES              5
#endif
#ifndef TRANSMITTED_SIX_TIMES
#define TRANSMITTED_SIX_TIMES               6
#endif
#ifndef TRANSMITTED_SEVEN_TIMES
#define TRANSMITTED_SEVEN_TIMES             7
#endif

typedef enum
{
  RLC_DEBUG_NONE,
  RLC_DEBUG_VAR,
  RLC_DEBUG_FLAG,
  RLC_DEBUG_LIMIT_NUM_SDU_PER_TTI,
  RLC_DEBUG_ENB_BUF_STATUS
 ,RLC_CHANGE_UL_COMPR_PARAMETERS = 5
 ,RLC_SEND_COMPRESSED_UNCOMPRESSED_PACKETS
 ,RLC_ENABLE_DISABLE_UL_COMPRESSION
 ,RLC_CHANGE_UL_COMP_BO_THRESHOLDS
 ,RLC_CHANGE_DACC_CONFIG_PARAMS
 ,RLC_ENABLE_UL_COMPR_RESOURCE_CRUNCH = 10
 ,RLC_UL_COMP_INDUCE_PTP_NAK
 ,RLC_INDUCE_BAD_RLC_NAK
 ,RLC_DEBUG_SDU_PDU_DUMP
 ,RLC_CHANGE_STMR_TICKS_PER_COMP_USAGE 
 ,RLC_UL_COMP_TEMP_2 = 15
 ,RLC_UL_COMP_TEMP_3
 ,RLC_UL_COMP_TEMP_4
 ,RLC_ULC_LOGGING_DEBUG
 ,RLC_CFCM_INTERNAL_SIMULATION
}wcdma_rlc_debug_var_type;

/*MACRO to reset RLC logging globals after log packet is populated*/
#define RLC_UL_RESET_LOGGING_GLOBALS(ptr)\
  do\
  { \
    ptr->num_pdu   = 0; \
    ptr->num_of_li = 0; \
    ptr->padded_li = 0; \
  }while(0)

/*===========================================================================

FUNCTION RLC_UL_SEND_TM_LOG_PACKET

DESCRIPTION
   This function prepares a log packet for configured Uplink RLC Transparent 
   Mode channels and sends the packet to log services.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_send_tm_log_packet
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_tm_config_type *tm_ptr  /* Ptr to Transparent Mode Config Info    */
);


/*===========================================================================

FUNCTION RLC_UL_SEND_UM_LOG_PACKET

DESCRIPTION
  This function prepares a log packet for configured Uplink RLC 
  Unacknowledged Mode channels and sends the packet to log services.  

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_send_um_log_packet
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_um_config_type *um_ptr,  /* Ptr to Unacknowledged Mode Config Info */
  boolean *est_or_reest
);


/*===========================================================================

FUNCTION RLC_UL_SEND_AM_LOG_PACKET

DESCRIPTION
  This function prepares a log packet for configured Uplink RLC 
  Acknowledged Mode channels and sends the packet to log services.    

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_send_am_log_packet
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_am_config_req_type *am_ptr, /* Ptr to Acknowledged Mode Config Info */
  boolean *est_or_reest
);


/*===========================================================================

FUNCTION RLC_UL_SEND_STATE_LOG_PACKET

DESCRIPTION
  This function prepares a log packet with state information for Uplink RLC 
  logical channels and sends the packet to log services.    
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_send_state_log_packet
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_log_state_type *log_ptr       /* Pointer to State Information     */

);

/*===========================================================================

FUNCTION RLC_UL_TX_AM_STATISTIC_PKT

DESCRIPTION
  This function prepares a log packet with statistics information for Uplink RLC 
  logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_tx_am_statistic_pkt
(
  rlci_uplink_table_type *ul_tbl_ptr     /* Pointer to am status Information   */
);

/*===========================================================================

FUNCTION RLC_UL_TX_AM_SIG_PLANE_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with PDU size, data information for 
  Uplink RLC signaling plane logical channels and sends the packet to 
  log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_tx_am_sig_plane_pdu_log_pkt
(
  rlci_uplink_table_type *ul_tbl_ptr,  /* Pointer to  pdu  Information */
  uint16                  num_am_ch,  
  boolean 		  is_eul  
);

/*===========================================================================

FUNCTION RLC_UL_TX_AM_USR_PLANE_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with PDU size, data information for 
  Uplink RLC user plane logical channels and sends the packet to 
  log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void rlc_ul_tx_am_usr_plane_pdu_log_pkt
(
  rlci_uplink_table_type *ul_tbl_ptr,  /* Pointer to  pdu  Information */
  uint16                  num_am_ch,  
  boolean 		  is_eul  
);

/*===========================================================================

FUNCTION RLC_UL_TX_AM_STATUS_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with NAK control PDU information for 
  Uplink RLC logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_tx_am_status_pdu_log_pkt
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_ul_am_ctrl_type   *am_ctrl_ptr,
  uint16                  pdu_size,      
  dsm_item_type          *pdu_ptr,
  uint16                  byte_offset   
);

/*===========================================================================

FUNCTION  RLC_UL_TX_CIPHER_LOG_PACKET

DESCRIPTION
  Send UL Ciphering config LOG packet to DIAG.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_tx_cipher_log_packet
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_cipher_config_type *cipher_ptr  /* Ptr to Downlink Cipher Config.   */
);

/*===========================================================================

FUNCTION  RLC_UL_TX_CIPHER_PDU_LOG_PKT

DESCRIPTION
  Send ciphering PDU parameters log to DIAG.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_tx_pdu_cipher_log_pkt
(
  rlci_uplink_table_type  *rlc_ul_table_ptr,
  rlc_ul_pdu_cipher_info_type *rlci_ul_pdu_cipher_log
);


#endif /* FEATURE_RLC_QXDM_LOGGING */


/*===========================================================================

FUNCTION RLC_UL_TX_UM_USR_PLANE_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with pdu  information for
  Uplink RLC UM logical channels and sends the packet to
  log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_UM_PDU_LOGGING
void rlc_ul_tx_um_pdu_log_pkt
(
  rlci_uplink_table_type  *ul_tbl_ptr,   /* Pointer to pdu log information */  
  boolean 		   is_eul
);

#endif

#ifdef FEATURE_MAC_I
void    rlc_enh_ul_flex_pdu_log_pkt(rlci_uplink_table_type *rlc_ul_table_ptr, uint16 num_am_ch);

void rlc_enh_ul_um_flex_pdu_log_pkt_info
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint8                          num_pdu,
  rlci_ul_ctrl_blk_type         *ctrl_blk_ptr,
  rlc_log_flex_ul_pdu_data_type *ul_pdu_log_ptr
);

void rlc_enh_ul_um_flex_pdu_info
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint8                  num_pdu,
  rlci_ul_ctrl_blk_type  *ctrl_blk_ptr,
  rlc_log_flex_ul_pdu_info_type *ul_pdu_ptr
);
#endif

/*===========================================================================

FUNCTION wcdma_rlc_set_dbg_var_through_qxdm

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
ex. commands
send_data 75 4 45 0 1 <val>   - for rlc_debug_var
send_data 75 4 45 0 5 <val>   - for RLC_CHANGE_UL_COMPR_PARAMETERS
send_data 75 4 45 0 6 <val>   - for RLC_SEND_COMPRESSED_UNCOMPRESSED_PACKETS
send_data 75 4 45 0 7 <val>   - for RLC_ENABLE_DISABLE_UL_COMPRESSION
send_data 75 4 45 0 8 <val>   - for RLC_CHANGE_UL_COMP_BO_THRESHOLDS
<val> - 32 bit value of debug variable
for providing hex value append val with 0x

===========================================================================*/
void wcdma_rlc_set_dbg_var_through_qxdm
(
  sys_modem_as_id_e_type as_id,
  uint8 debug_var_type,
  uint32 debug_var_val
);

#endif /* RLCULLOG_H */
