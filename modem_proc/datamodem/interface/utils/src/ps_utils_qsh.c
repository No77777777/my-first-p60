/*!
  @file
  ps_utils_qsh.c

  @brief
  QSH implementation for Data Interfaces.
*/

/*==============================================================================

  Copyright (c) 2020 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who         what, where, why
--------   ---         ------------------------------------------------------------- 
06/14/18   ss         Initial release
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#if defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_MDUMP)
#include "ps_utils_qsh.h"
#include "ps_utils_qsh_mdump.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_qmi_if.h"
#include "ds_qmi_wds.h"
#include "ds_qmi_qos.h"

#include "ps_sys_conf.h"
#include "queue.h"


#include "ds3gsiolib.h"
#include "dsati.h"
#include "dsatetsicall.h"
#include "dsatetsipkt.h"
#include "dsatapi.h"
#include "sys.h"




/*==============================================================================

                         DEFINITIONS

==============================================================================*/


/*==============================================================================

                         EXTERNAL Variables

==============================================================================*/
#ifdef FEATURE_QSH_MDUMP

extern boolean dsatcmdp_processing_async_cmd;
extern uint8 cops_no_mode_change[SYS_MODEM_AS_ID_MAX-1];
extern boolean waiting_for_an_ack;
extern dsati_preproc_mode_enum_type preprocessor_mode;
extern dsat_num_item_type dsat_gprs_attach_status[DSAT_SYS_MODEM_AS_ID_MAX];
extern q_type  ps_sys_eventi_q;
extern boolean dsatme_pending_mmgsdi_command[];

#endif /* FEATURE_QSH_MDUMP */

/*==============================================================================

                                FUNCTIONS

==============================================================================*/

/*===========================================================================

FUNCTION PS_UTILS_QSH_CB

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
void  ps_utils_qsh_cb(qsh_client_cb_params_s *cb_params_ptr)
{

  if (cb_params_ptr == NULL)
  {
    return;
  }

#ifdef FEATURE_QSH_MDUMP
  if(cb_params_ptr->action & QSH_ACTION_MDUMP)
  {
    ps_utils_qsh_mdump_collect();
  }
#endif /* FEATURE_QSH_MDUMP */
  
}

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION PS_UTILS_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for Data Interface module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void ps_utils_qsh_mdump_collect()
{
  /* Capturing global variable through functions */
  
  /* QMI */
  qmi_wds_qsh_mdump_collect();
  qmi_qos_qsh_mdump_collect();

  /* Sysapi */
  ps_sys_conf_qsh_mdump_collect();

  /*DPM */
  ps_dpm_qsh_mdump_collect();

  /* QMUX */
  qmux_qsh_mdump_collect();
  qmi_framework_qsh_mdump_collect();

  /* RmNet */
  rev_ip_transport_qsh_mdump_collect();
  rmnet_xport_qsh_mdump_collect();

  /* ATCOP */
  ds3g_siolib_qsh_mdump_collect();
  dsatcmdp_qsh_mdump_collect();
  dsatetsipkt_qsh_mdump_collect();
  dsatcmif_qsh_mdump_collect();
  dsatsms_qsh_mdump_collect();
  dsatvoice_qsh_mdump_collect();
  
  /* Mem Pool */
  ps_mem_qsh_mdump_collect();
  
  /* Capturing Global variable directly */
  qsh_mdump_collect_high(&qmi_if_info, sizeof(qmi_if_info_type));
  qsh_mdump_collect_high(&ps_sys_eventi_q, sizeof(q_type));
  qsh_mdump_collect_high(&dsatcmdp_processing_async_cmd, sizeof(boolean));
  qsh_mdump_collect_high(&cops_no_mode_change,(SYS_MODEM_AS_ID_MAX-1)*sizeof(uint8));
  qsh_mdump_collect_high(&waiting_for_an_ack, sizeof(boolean));
  qsh_mdump_collect_high(&preprocessor_mode, sizeof(dsati_preproc_mode_enum_type));
  qsh_mdump_collect_high(&ds3g_siolib_port_allocated, DS3G_SIOLIB_PORTID_MAX * sizeof(ds3g_siolib_port_e_type));
  qsh_mdump_collect_high(&ds3g_siolib_last_port_allocated, DS3G_SIOLIB_PORTID_MAX * sizeof(ds3g_siolib_port_e_type));
  qsh_mdump_collect_high(&dsatcmdp_restricted_commands, (DSAT_APPS_ID_MAX - 1) * sizeof(boolean));
  qsh_mdump_collect_high(&dsatme_pending_mmgsdi_command, (DSAT_APPS_ID_MAX - 1) * sizeof(boolean));
  qsh_mdump_collect_high(&etsicall_cmd_pending, DSAT_SYS_MODEM_AS_ID_MAX * sizeof(etsicall_cmd_type));
  qsh_mdump_collect_high(&dsatcmdp_dds_qcsimapp_val, sizeof(dsat_num_item_type));
  qsh_mdump_collect_high(&ds3g_siolib_answer_info, sizeof(ds3g_siolib_answer_info_type));
  qsh_mdump_collect_high(&dsat_pdp_state, sizeof(dsat_pdp_connect_s_type));
  qsh_mdump_collect_high(&ds3g_siolib_port_signal_status, (DS3G_SIOLIB_PORTID_MAX + 1) * sizeof(ds3g_siolib_port_signal_status_type));
  #ifdef FEATURE_ETSI_ATTACH
  qsh_mdump_collect_high(&dsat_gprs_attach_status, DSAT_SYS_MODEM_AS_ID_MAX * sizeof(dsat_num_item_type));
  #endif /* FEATURE_ETSI_ATTACH */
  qsh_mdump_collect_high(&dsat_qcsimapp_table, DSAT_SLOT_IDX_MAX * sizeof(dsat_qcsimapp_table_s_type));
  qsh_mdump_collect_high(&dsat_pending_state, sizeof(dsat_global_state));
  qsh_mdump_collect_high(&dsat_curr_cmd_var, sizeof(dsat_cmd_hdlr_state_s_type));
  qsh_mdump_collect_high(&dsat_pdp_cid, DSAT_MAX_ALLOWED_PROFILES * sizeof(byte));
  qsh_mdump_collect_high(&dsat_qcsimapp_info, DSAT_APPS_ID_MAX * sizeof(dsat_qcsimapp_info_s_type));
  
}
#endif /* FEATURE_QSH_MDUMP */
#endif /* defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_MDUMP) */

