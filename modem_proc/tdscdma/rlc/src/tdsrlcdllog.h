#ifndef TDSRLCDLLOG_H
#define TDSRLCDLLOG_H

/*===========================================================================
         D O W N L I N K   R L C    L O G G I N G  H E A D E R   F I L E

DESCRIPTION
  This file provides the function prototypes for RLC downlink logging.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcdllog.h_v   1.5   12 Jun 2002 18:44:00   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcdllog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsrlclog.h"
#include "tdsrlcrrcif.h"
#include "tdsrlcdl.h"

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/
 extern uint32 tdsrlci_log_mask;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_DL_SEND_TM_LOG_PACKET

DESCRIPTION
  This function prepares a log packet for configured Downlink RLC Transparent 
  Mode channels and sends the packet to log services.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_send_tm_log_packet
(
  tdsrlc_dl_tm_config_type *tm_ptr  /* Ptr to Transparent Mode Config Info    */
);


/*===========================================================================

FUNCTION RLC_DL_SEND_UM_LOG_PACKET

DESCRIPTION
  This function prepares a log packet for configured Downlink RLC 
  Unacknowledged Mode channels and sends the packet to log services.    

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_send_um_log_packet
(
  tdsrlc_dl_um_config_type *um_ptr  /* Ptr to Unacknowledged Mode Config Info */
);


/*===========================================================================

FUNCTION RLC_DL_SEND_AM_LOG_PACKET

DESCRIPTION
  This function prepares a log packet for configured Downlink RLC 
  Acknowledged Mode channels and sends the packet to log services.    

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_send_am_log_packet
(
  tdsrlc_am_config_req_type *am_ptr /* Ptr to Acknowledged Mode Config Info   */
);


/*===========================================================================

FUNCTION RLC_DL_SEND_STATE_LOG_PACKET

DESCRIPTION
  This function prepares a log packet with state information for Downlink RLC 
  logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_send_state_log_packet
(
  tdsrlc_dl_log_state_type *log_ptr      /* Pointer to State Information      */
);

/*===========================================================================

FUNCTION RLC_DL_SEND_AM_STAT_PACKET

DESCRIPTION
  This function prepares a log packet with statistics information for Downlink RLC 
  logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_tx_am_statistic_pkt
(
  tdsrlci_downlink_table_type *log_ptr      /* Pointer to State Information   */
);
/*===========================================================================

FUNCTION RLC_DL_TX_AM_NAK_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with NAK control PDU information for 
  Downlink RLC logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_tx_am_nak_pdu_log_pkt
(
  rlc_lc_id_type rlc_id,
  uint16         pdu_size,
  dsm_item_type  *pdu_ptr
  , uint16          offset

);

/*===========================================================================

FUNCTION  RLC_DL_TX_CIPHER_LOG_PACKET

DESCRIPTION
  Send DL Ciphering config LOG packet to DIAG.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_tx_cipher_log_packet
(
  tdsrlc_dl_cipher_config_type *cipher_ptr  /* Ptr to Downlink Cipher Config.   */
);

/*===========================================================================

FUNCTION  RLC_DL_TX_CIPHER_PDU_LOG_PKT

DESCRIPTION
  Send ciphering PDU parameters log to DIAG.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_tx_pdu_cipher_log_pkt
(
   tdsrlc_dl_pdu_cipher_info_type *rlci_dl_pdu_cipher_log
);
#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*===========================================================================

FUNCTION RLC_DL_R7_FLEX_AM_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with pdu information for 
  Downlink RLC logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_enh_r7_am_pdu_log_pkt(tdsrlci_downlink_table_type  *dl_tbl_ptr);

#endif

/*===========================================================================

FUNCTION RLC_DL_ENH_TX_AM_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with pdu information for 
  Downlink RLC logical channels and sends the packet to log services.      

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_enh_tx_am_pdu_log_pkt(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, uint8 lc_id);

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
/*===========================================================================

FUNCTION RLC_DL_ENH_TX_UM_USR_PLANE_PDU_LOG_PKT

DESCRIPTION
  This function prepares a log packet with pdu information for
  Downlink UM logical channels and sends the packet to
  log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_enh_tx_um_pdu_log_pkt(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr);
#endif

/*===========================================================================

FUNCTION rlci_enh_dl_log_info_handler

DESCRIPTION
This function checks the status of AM and UM log packets and appropriately 
sets the log_mask bits. 
Following bit configuration is used: 
RLCI_UM_USR_PLANE_LOG_MASK : 0x01
RLCI_AM_USR_PLANE_LOG_MASK : 0x02
RLCI_AM_SIG_PLANE_LOG_MASK : 0x04

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_enh_dl_log_info_handler
(
  void
);

#endif /* TDSRLCDLLOG_H */
