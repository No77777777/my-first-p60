#ifndef _DS_QMI_VOICE_MSGR_IF_H
#define _DS_QMI_VOICE_MSGR_IF_H
/*===========================================================================

                         D S _ Q M I _ V O I C E _ MSGR _ I F . H

DESCRIPTION

 The Data Services Qualcomm Voice service Messenger interface header file.


EXTERNALIZED FUNCTIONS



Copyright (c) 2013 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/qmi/src/qmi_voice_msgr_if.h#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/23/13    vs     Initial revision.
===========================================================================*/

#include "cm.h"
#include  "log.h"
#include "cm_msgr_msg.h"
#include "qmi_mmode_task_cmd.h"


/*===========================================================================

                    RESPONSES

===========================================================================*/




/*===========================================================================

                    INDICATIONS

===========================================================================*/



/*===========================================================================

                   CALLBACKS

===========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                        EXTERNAL FUNCTION DEFINTIONS
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

void qmi_voice_msgr_handler( void *param );

void qmi_voice_msgr_register( msgr_client_t *msgr_client );

IxErrnoType qmi_voice_msgr_send_srvcc_config_req
(
  qmi_voice_cm_if_set_srvcc_call_context_s *const in_data_ptr,
  qmi_voice_cm_if_as_id_e_type as_id
);

void qmi_voice_msgr_ecall_status_ind
(
  mmode_qmi_voice_ecall_status_ind_msg_type *ecall_status_info
);

IxErrnoType qmi_voice_msgr_send_set_wwan_911_timer_req
(
  qmi_voice_cm_if_set_wwan_911_timer_s  *const in_data_ptr,
  qmi_voice_cm_if_as_id_e_type as_id
);

IxErrnoType qmi_voice_msgr_send_get_wwan_911_timer_req
(
  qmi_voice_cm_if_as_id_e_type           as_id
);

void qmi_voice_msgr_vice_dialog_info_ind
(
  mmode_qmi_voice_viceDialog_info_ind_msg_type *vice_dialog_info,
  sys_modem_as_id_e_type asubs_id
);

enum qmi_voice_subs_e
{
  QMI_VOICE_SUBS_NONE      = -1,
  QMI_VOICE_SUBS_PRIMARY   = 0,
#ifdef FEATURE_DUAL_SIM
  QMI_VOICE_SUBS_SECONDARY = 1,
#endif
#ifdef FEATURE_TRIPLE_SIM
  QMI_VOICE_SUBS_TERTIARY  = 2,
#endif
  QMI_VOICE_SUBS_MAX
};

#endif /* _DS_QMI_VOICE_MSGR_IF_H */


