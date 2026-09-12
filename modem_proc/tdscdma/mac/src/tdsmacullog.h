#ifndef TDSMACULLOG_H
#define TDSMACULLOG_H


/*===========================================================================
               U P L I N K   M A C    L O G G I N G   H E A D E R    F I L E

DESCRIPTION
  This file provides the function prototypes for MAC uplink logging.
  
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

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macullog.h_v   1.1   24 Aug 2001 15:30:52   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacullog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsmaclog.h"
#include "tdsmacrrcif.h"
#include "tdsmacinternal.h"


#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING


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
void tdsmac_ul_send_cfg_log_packet
(
  tdsmac_ul_config_type **config_ptr   /* Address of Ptr to Uplink MAC config */
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
void tdsmac_ul_send_rach_param_packet
(
  tdsmac_ul_rach_txcntl_config_type *rach_tx_prmtrs /* Ptr to RACH TX params  */
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
void tdsmac_ul_send_tfcs_cfg_packet
(
  tdsmac_ul_tfcs_type *tfcs_ptr,             /* Ptr to TFCS info              */
  uint8            *tdstrch_id               /* Transport Channel IDs         */
);
/*===========================================================================

FUNCTION tdsmac_ul_send_tvm_report_packet

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
void tdsmac_ul_send_tvm_report_packet
(
  tdsmac_ul_traffic_meas_ind_type *tvm_rpt_ptr,
  uint16    nreports_sent
);

/*===========================================================================

FUNCTION TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void tdsmac_ul_tvm_cfg_log_packet
(
 tdsmac_ul_traffic_meas_config_type const *tvm_cfg_ptr
 );


#endif /* FEATURE_TDSCDMA_MAC_QXDM_LOGGING */

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
uint32 tdsmac_ul_convert_tvm_buf_payload
    (
    uint32    payload
    );

#endif /* TDSMACULLOG_H */
