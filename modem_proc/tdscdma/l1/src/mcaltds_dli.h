#ifndef MCALTDS_DLI_H
#define MCALTDS_DLI_H

/*============================================================================*/
/** @file
 * This module has definition and declaration related to MCAL UMTS downlink
 * channel setup module APIs
 */
/*============================================================================*/

/*============================================================================
Copyright (c) 2007 - 2008 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_dli.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
7/5/11      YH       Cleanup code for TD production

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/
#include "tdscdma_variation.h"
#ifdef FEATURE_MCAL_TDSCDMA
/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "comdef.h"
#include "mcaltds_dl.h"
#include "tfw_sw_intf_msg.h"

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
#define MCALTDS_DL_NOTIFY_ASAP 0
#define MCALTDS_DL_NOTIFY_NORMAL 1
#define MCALTDS_DL_NOTIFY_DELAY 2
#define MCALTDS_DL_NOTIFY_PROTECT 3

#define MCALTDS_DL_DELAY_NOTIFY_OFFSET 1
#define MCALTDS_DL_DELAY_NOTIFY_FACH_DROP_OFFSET 2 
#define MCALTDS_DL_DELAY_NOTIFY_PROTECT 4


/*Yong add deep srch const to pass them to FW.*/
#define MCALTDS_DL_DEEP_SRCH_FREQ_STEP_SIZE 400 
#define MCALTDS_DL_DEEP_SRCH_NUM_SIDED_FREQ_BINS 5 

/*Yongh define this datastructure to dump all of configuration command*/
typedef struct{
mcaltds_dl_phychan_enum_type phychan_type;
tfw_dl_midamble_config_table_cmd_t mcaltds_midamble_cfg_msg;
tfw_dl_pccpch_config_cmd_t mcaltds_pccpch_cfg_msg;
tfw_dl_pich_config_cmd_t mcaltds_pich_cfg_msg;
tfw_dl_sccpch_config_cmd_t mcaltds_sccpch_cfg_msg;
tfw_dl_dpch_config_cmd_t mcaltds_dpch_cfg_msg;
tfw_rx_time_sync_cmd_t mcaltds_rx_sync_cfg_msg;
}mcaltds_dl_cfg_cmd_struct;

/** This enumerates the various possible states of a downlink physical channel */
typedef enum
{
  /** The free state of channel, when it ready for any kind of addition */
  MCALTDS_DL_PHYCHAN_INACTIVE,
  /** This is when the channel is active in software and MDSP sense */
  MCALTDS_DL_PHYCHAN_ACTIVE,
  /** This is when channel is under configuration for addition */
  MCALTDS_DL_PHYCHAN_CFG_TO_ACTIVE,
  /** This is when channel is under re-configuration*/
  MCALTDS_DL_PHYCHAN_RECFG_TO_ACTIVE,
  /** This is when channel is under configuration for drop */
  MCALTDS_DL_PHYCHAN_CFG_TO_INACTIVE,
  /** Number of physical channel types */
  MCALTDS_NUM_DL_PHYCHAN_STATES
} mcaltds_dl_phychan_state_enum_type;

/* Structure type declaration */
/* -------------------------- */

/* This structure is used to contain the parameters for the downlink
  * physical channel config command to the mDSP.  Also included are
  * masks for the valid bits for each field 
  */
typedef struct
{
  /** Current state for the channel */
  mcaltds_dl_phychan_state_enum_type dl_phychan_state;

  /** Channel status */
  boolean in_use;

  /* subframe number for action time */
  uint16 subfn;

} mcaltds_dl_phychan_db_entry_struct_type;



/*===========================================================================
FUNCTION         mcaltds_dl_phychan_notify

DESCRIPTION      
  Notify highlayer result of physical channel operation.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
LOCAL void mcaltds_dl_phychan_notify(uint8 config_id);

/*===========================================================================
FUNCTION         mcaltds_dl_phychan_delay_notify

DESCRIPTION      
  If channel operation is not asap, Notify highlayer result of physical channel operation,
  when register frame boundary event is triggerred.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
LOCAL void mcaltds_dl_phychan_delay_notify(uint16 subfn, uint8 config_id);

/*===========================================================================
FUNCTION         mcaltds_dl_phychan_notify_delay_cb

DESCRIPTION      
  When register frame boundary event is triggerred, this function will be called to 
  notify result of physical channel operation.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
LOCAL void mcaltds_dl_phychan_notify_delay_cb(uint16 subfn, uint32 config_id);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_midamble_cfg_cmd_msg

DESCRIPTION      
  Send downlink midamble configuration to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_midamble_cfg_cmd_msg(tfw_dl_midamble_config_table_cmd_t *midamble_cfg_msg);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_pich_cfg_cmd_msg

DESCRIPTION      
  Send PICH physical channel configuration parameter to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_pich_cfg_cmd_msg(tfw_dl_pich_config_cmd_t *pich_cfg_msg);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_pccpch_cfg_cmd_msg

DESCRIPTION      
  Send PCCPCH physical channel configuration parameter to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_pccpch_cfg_cmd_msg(tfw_dl_pccpch_config_cmd_t *pccpch_cfg_msg);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_sccpch_cfg_cmd_msg

DESCRIPTION      
  Send SCCPCH physical channel configuration parameter to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_sccpch_cfg_cmd_msg(tfw_dl_sccpch_config_cmd_t *sccpch_cfg_msg);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_dpch_cfg_cmd_msg

DESCRIPTION      
  Send DPCH physical channel configuration parameter to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_dpch_cfg_cmd_msg(tfw_dl_dpch_config_cmd_t *dpch_cfg_msg);


/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_rx_time_sync_cmd_msg

DESCRIPTION      
  Send this difference of RX counter and wall time counter to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_send_fw_rx_time_sync_cmd_msg(tfw_rx_time_sync_cmd_t *rx_time_sync_msg);

/*===========================================================================
FUNCTION         mcaltds_dl_send_fw_sync_status_cmd_msg

DESCRIPTION      
  Send dpch sync detection status  to FW.

DEPENDENCIES
  None
  
RETURN VALUE 
  None

SIDE EFFECTS
  None.
  
===========================================================================*/

void mcaltds_dl_send_fw_sync_status_cmd_msg(tfw_inSync_config_cmd_t *sync_status_msg);

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_midamble_cfg_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_midamble_cfg_cmd(tfw_dl_midamble_config_table_cmd_t *midamble_cmd_ptr);
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_pich_cfg_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_pich_cfg_cmd(tfw_dl_pich_config_cmd_t *pich_cmd_ptr);
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_pccpch_cfg_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_pccpch_cfg_cmd(tfw_dl_pccpch_config_cmd_t *pccpch_cmd_ptr);
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_sccpch_cfg_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_sccpch_cfg_cmd(tfw_dl_sccpch_config_cmd_t *sccpch_cmd_ptr);
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_dpch_cfg_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_dpch_cfg_cmd(tfw_dl_dpch_config_cmd_t *dpch_cmd_ptr);
/*===========================================================================
FUNCTION         mcaltds_dl_log_fw_rx_time_sync_cmd

DESCRIPTION      
  Callback function that mcal manager calls when it is signaled that there is
  a packet to be logged.

DEPENDENCIES
  None
  
RETURN VALUE 
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void mcaltds_dl_log_fw_rx_time_sync_cmd(tfw_rx_time_sync_cmd_t *rx_time_sync_cmd_ptr);

#endif
#endif /* FEATURE_MCAL_TDSCDMA */
#endif /* MCALWCDMA_DLI_H */

