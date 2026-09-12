#ifndef LPA_H
#define LPA_H
/*===========================================================================


                  L P A   D E F I N I T I O N S

                      A N D   F U N C T I O N S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
03/29/18   bcho    LPA phase2.1 changes
12/07/16   av      LPA phase2 initial changes
10/06/16   av      Introduce DS http post retry timer logic
05/16/16   av      Route HTTP request to QMI HTTP
04/14/16   ks      Task init changes for QMI UIM HTTP
04/11/16   av      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "lpa_platform.h"
#include "comdef.h"
#include "rex.h"
#include "queue.h"
#include "rcinit.h"
#include "lpalib.h"
#include "qmi_uim_http.h"

#include "DDIChipInfo.h"
#include "DDIPlatformInfo.h"
#include "DALDeviceId.h"

/*=============================================================================

                       GLOBAL DECLARATIONS

=============================================================================*/
extern boolean                           lpa_task_stopped;
extern DalChipInfoIdType                 lpa_chip_id;

/*=============================================================================

                       DEFINE DECLARATIONS

=============================================================================*/

extern rex_tcb_type                          *lpa_tcb_ptr;
#define UIM_LPA_TCB                           lpa_tcb_ptr

/*--------------------------------------------------------------------------
                             Signal Masks
--------------------------------------------------------------------------*/
#define LPA_CMD_SIG                            0x00000001
#define LPA_LPD_CMD_SIG                        0x00000002
#define LPA_MMGSDI_CMD_SIG                     0x00000004
#define LPA_HTTP_RSP_SIG                       0x00000008
#define LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1     0x00000010
#define LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_2     0x00000020
#define LPA_TASK_STOP_SIG                      0x00000040
#define LPA_QMI_MODEM_TASK_READY_SIG           0x00000080
#define LPA_DOG_RPT_TIMER_SIG                  0x00000100

/* Signals used by QMI UIM HTTP */
#define QMI_UIM_HTTP_CMD_Q_SIG                 0x00000200
#define QMI_UIM_HTTP_QCSI_SIG                  0x00000400

#define LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_1 0x00000800
#define LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_2 0x00001000

#define LPA_NOTIFICATION_CMD_SIG               0x00002000

#define LPA_SVN_LEN                            0x03
#define LPA_SVN_BYTE1_DEFAULT                  0x01
#define LPA_SVN_BYTE2_DEFAULT                  0x00
#define LPA_EUICC_CHALLENGE_LEN                0x10

/* ----------------------------------------------------------------------------
   ENUM:      LPA_EUICC_PHASE_ENUM_TYPE

   DESCRIPTION:
     Indicates the eUICC phase
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_PHASE1_EUICC,    /* Conforming up to published phase1 of SGP spec */
  LPA_PHASE1_1_EUICC,  /* Conforming up to phase1.1 of SGP spec */
  LPA_PHASE2_EUICC,    /* Conforming up to phase2 of SGP spec */
  LPA_PHASE2_1_EUICC   /* Conforming up to phase2.1 of SGP spec */
} lpa_euicc_phase_enum_type;

/* ----------------------------------------------------------------------------

   ENUM:      LPA_TASKS_TYPE

   DESCRIPTION:
     The enum for various modem tasks that LPA interacts with
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_QMI_MODEM_TYPE  = 0x00,      /* QMI Modem task */
  LPA_TASK_TYPE_MAX   = 0xFF
} lpa_tasks_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_CLIENT_DATA_TYPE

   DESCRIPTION:
     Stores information received from client as part of an LPA request

-----------------------------------------------------------------------------*/
typedef struct {
  lpa_callback_type                             callback;
  lpa_event_callback_type                       event_callback;
  void                                         *user_data_ptr;
  lpa_message_enum_type                         cmd;
} lpa_client_data_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_SEND_RESPONSE

DESCRIPTION
  This function sends a response to the client by invoking the client
  callback and clears all data corresponding to the current command.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_send_response (
  lpa_result_enum_type                 status,
  lpa_response_data_type              *response_ptr,
  lpa_slot_id_enum_type                slot_id,
  boolean                              reset_all
);

/*===========================================================================
FUNCTION LPA_SEND_EVENT

DESCRIPTION
  This function sends an event to the client by invoking the client event
  callback.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_send_event (
  lpa_result_enum_type                 status,
  lpa_event_data_type                 *event_ptr,
  lpa_slot_id_enum_type                slot_id,
  boolean                              reset_all
);

/*===========================================================================
FUNCTION LPA_SAVE_CLIENT_DATA

DESCRIPTION
  Function saves the client data that needs to be saved throughout the
  duration of the command.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_save_client_data (
  lpa_callback_type                  callback,
  lpa_event_callback_type            event_callback,
  void                              *user_data_ptr,
  lpa_slot_id_enum_type              slot_id,
  lpa_message_enum_type              cmd
);

/*===========================================================================
FUNCTION LPA_CLEAR_CLIENT_DATA

DESCRIPTION
  Function clears the saved client data.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_clear_client_data (
  lpa_slot_id_enum_type              slot_id
);

/* ==========================================================================
FUNCTION LPA_RESET

DESCRIPTION:
  Performs a reset of all modules.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
==========================================================================*/
void lpa_reset (
  lpa_slot_id_enum_type                  slot_id
);

/*===========================================================================
FUNCTION LPA_RETRIEVE_EUICC_PHASE

DESCRIPTION
  This function can be called to retrieve the phase conformance of the eUICC

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_retrieve_euicc_phase (
  lpa_slot_id_enum_type             slot_id,
  lpa_euicc_phase_enum_type        *phase_ptr
);

/*===========================================================================
FUNCTION LPA_SET_SVN

DESCRIPTION
  This function can be called to set the svn (eUICC version number) global
 
DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_set_svn (
  lpa_slot_id_enum_type             slot_id,
  uint8                             svn_len,
  uint8                            *svn_ptr
);

/*===========================================================================
FUNCTION LPA_GET_SVN

DESCRIPTION
  This function can be called to get the svn (eUICC version number) global
 
DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_get_svn (
  lpa_slot_id_enum_type             slot_id,
  uint8                             svn_ptr_len,
  uint8                            *svn_ptr
);

/*===========================================================================
FUNCTION LPA_GET_CURRENT_CMD

DESCRIPTION
  This function can be called to find out which LPA command is in progress
 
DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_get_current_cmd (
  lpa_slot_id_enum_type             slot_id,
  lpa_message_enum_type            *cmd_ptr
);

#endif /* LPA_H */
