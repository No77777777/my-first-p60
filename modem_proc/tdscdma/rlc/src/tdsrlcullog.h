#ifndef TDSRLCULLOG_H
#define TDSRLCULLOG_H

/*===========================================================================
               U P L I N K   R L C    L O G G I N G   H E A D E R    F I L E

DESCRIPTION
  This file provides the function prototypes for RLC uplink logging.
  
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

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcullog.h_v   1.5   12 Jun 2002 18:44:38   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcullog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

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
#include "tdsrlcul.h"

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

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
void tdsrlc_ul_send_tm_log_packet
(
  tdsrlc_ul_tm_config_type *tm_ptr  /* Ptr to Transparent Mode Config Info    */
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
void tdsrlc_ul_send_um_log_packet
(
  tdsrlc_ul_um_config_type *um_ptr  /* Ptr to Unacknowledged Mode Config Info */
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
void tdsrlc_ul_send_am_log_packet
(
  tdsrlc_am_config_req_type *am_ptr /* Ptr to Acknowledged Mode Config Info */
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
void tdsrlc_ul_send_state_log_packet
(
  tdsrlc_ul_log_state_type *log_ptr       /* Pointer to State Information     */

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
void tdsrlc_ul_tx_am_statistic_pkt
(
  tdsrlci_uplink_table_type *ul_tbl_ptr     /* Pointer to am status Information   */
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
void tdsrlc_ul_tx_am_sig_plane_pdu_log_pkt
(
  tdsrlci_uplink_table_type *ul_tbl_ptr,  /* Pointer to  pdu  Information */
  uint16                  num_am_ch
  
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

void tdsrlc_ul_tx_am_usr_plane_pdu_log_pkt
(
  tdsrlci_uplink_table_type *ul_tbl_ptr,  /* Pointer to  pdu  Information */
  uint16                  num_am_ch
  
);

/*===========================================================================

FUNCTION RLC_UL_TX_AM_NAK_PDU_LOG_PKT

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
void tdsrlc_ul_tx_am_nak_pdu_log_pkt
(
  rlc_lc_id_type rlc_id,
  uint16         pdu_size,
  dsm_item_type  *pdu_ptr,
  uint16         byte_offset
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
void tdsrlc_ul_tx_cipher_log_packet
(
  tdsrlc_ul_cipher_config_type *cipher_ptr  /* Ptr to Downlink Cipher Config.   */
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
void tdsrlc_ul_tx_pdu_cipher_log_pkt
(
  tdsrlc_ul_pdu_cipher_info_type *tdsrlci_ul_pdu_cipher_log
);


#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

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
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
void tdsrlc_ul_tx_um_pdu_log_pkt
(
  tdsrlci_uplink_table_type  *ul_tbl_ptr,   /* Pointer to pdu log information */
  uint16                   num_um_ch
);

#endif
#endif /* TDSRLCULLOG_H */
