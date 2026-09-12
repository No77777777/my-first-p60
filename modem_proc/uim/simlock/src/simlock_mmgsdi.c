/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   M M G S D I   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock mmgsdi interaction
  functions.


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_mmgsdi.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when          who     what, where, why
--------   ---     -----------------------------------------------------------
05/08/18    nr       Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock_mmgsdi.h"
#include "simlock_delay.h"
#include "simlock_modem.h"
#include "simlock_platform.h"
#include "mmgsdilib_common.h"
#include "mmgsdilib.h"
#include "queue.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/* Name of SIMLOCK client (to be notified to MMGSDI when doing a client id reg*/
#define SIMLOCK_CLIENT_NAME        "simlock"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

typedef enum
{
  SIMLOCK_MMGSDI_CARD_EVENT_CB   /* MMGSDI Card event callback */
} simlock_mmgsdi_cmd_e_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_TASK_MMGSDI_CMD_TYPE

   DESCRIPTION:
     The common command header for all commands received by SIMLOCK task
-------------------------------------------------------------------------------*/
typedef struct {
  q_link_type                 link;
  simlock_mmgsdi_cmd_e_type   cmd_type;
  mmgsdi_event_data_type      mmgsdi_event;
} simlock_task_mmgsdi_cmd_type;

/* MMGSDI event queue */
static q_type                   simlock_mmgsdi_evt_q;
static mmgsdi_client_id_type    simlock_mmgsdi_client_id = MMGSDI_CLIENT_ID_ZERO;

/*===========================================================================
FUNCTION SIMLOCK_MMGSDI_FLUSH_QUEUE

DESCRIPTION
  This function is called to clear the simlock_mmgsdi_evt_q.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void simlock_mmgsdi_flush_queue (
  void
)
{
  simlock_task_mmgsdi_cmd_type  *mmgsdi_cmd_type_ptr = NULL;

  while (q_cnt(&simlock_mmgsdi_evt_q)!= 0)
  {
    mmgsdi_cmd_type_ptr = (simlock_task_mmgsdi_cmd_type *)(q_get(&simlock_mmgsdi_evt_q));

    if (mmgsdi_cmd_type_ptr != NULL)
    {
      SIMLOCK_MEM_FREE(mmgsdi_cmd_type_ptr);
    }
  }
}/* simlock_mmgsdi_flush_queue */


/*===========================================================================
FUNCTION            SIMLOCK_MMGSDI_CARD_STATUS_CB

DESCRIPTION
  Callback function to return SIM card status.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_mmgsdi_card_status_cb
(
  const mmgsdi_event_data_type   *mmgsdi_event_ptr
)
{
  simlock_task_mmgsdi_cmd_type  *mmgsdi_cmd_type_ptr = NULL;

  if(mmgsdi_event_ptr == NULL ||
     mmgsdi_event_ptr->evt != MMGSDI_CARD_ERROR_EVT)
  {
    return;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(mmgsdi_cmd_type_ptr, sizeof(simlock_task_mmgsdi_cmd_type));
  if (mmgsdi_cmd_type_ptr == NULL)
  {
    return;
  }

  mmgsdi_cmd_type_ptr->cmd_type     = SIMLOCK_MMGSDI_CARD_EVENT_CB;
  mmgsdi_cmd_type_ptr->mmgsdi_event = *mmgsdi_event_ptr;

  (void)q_link(mmgsdi_cmd_type_ptr, &mmgsdi_cmd_type_ptr->link);

  /* Put the message on the queue */
  q_put(&simlock_mmgsdi_evt_q, &mmgsdi_cmd_type_ptr->link);

  /*  Items on the queue. Set the signal */
  (void)rex_set_sigs(UIM_SIMLOCK_TCB, SIMLOCK_MMGSDI_EVT_SIG);
}/* simlock_mmgsdi_card_status_cb */


/*===========================================================================
FUNCTION            SIMLOCK_MMGSDI_REG_STATUS_CB

DESCRIPTION
  This callback function handles registration confirmation from MMGSDI.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_mmgsdi_reg_status_cb
(
  mmgsdi_return_enum_type      status,
  mmgsdi_cnf_enum_type         cnf,
  const mmgsdi_cnf_type       *cnf_ptr
)
{
  if(cnf_ptr == NULL)
  {
    return;
  }

  switch(cnf)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
      simlock_mmgsdi_client_id =
          cnf_ptr->client_id_and_evt_reg_cnf.response_header.client_id;
      break;

    default:
      break;
  }
}/* simlock_mmgsdi_reg_status_cb */


/*===========================================================================
FUNCTION           SIMLOCK_MMGSDI_INIT

DESCRIPTION
  This function handles the registration of simlock with MMGSDI.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_mmgsdi_init
(
  void
)
{
  mmgsdi_return_enum_type    mmgsdi_status = MMGSDI_SUCCESS;

  (void)q_init(&simlock_mmgsdi_evt_q);

  mmgsdi_status = mmgsdi_client_id_and_evt_reg_ext (simlock_mmgsdi_card_status_cb,
                                                    simlock_mmgsdi_reg_status_cb,
                                                    0,
                                                    SIMLOCK_CLIENT_NAME,
                                                    strlen(SIMLOCK_CLIENT_NAME));
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("mmgsdi_client_id_and_evt_reg failed 0x%x", mmgsdi_status);
    return SIMLOCK_GENERIC_ERROR;
  }

  return SIMLOCK_SUCCESS;
}/* simlock_mmgsdi_init */


/*===========================================================================
FUNCTION          SIMLOCK_CARD_EVENT_CB

DESCRIPTION
  This function handles MMGSDI events.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_card_event_cb
(
  const mmgsdi_event_data_type *mmgsdi_event_ptr
)
{
  if(mmgsdi_event_ptr == NULL)
  {
    return;
  }

  switch (mmgsdi_event_ptr->evt)
  {
    case MMGSDI_CARD_ERROR_EVT:
      /* delete simlock delay timer */
      simlock_delay_delete_delay_timer();
      break;

    default:
      break;
  }
}/* simlock_card_event_cb */


/*===========================================================================
FUNCTION                SIMLOCK_HANDLE_MMGSDI_EVT

DESCRIPTION
  This function read the MMGSDI events from the queue and handle it.
  Typically we do not have to keep the mmgsdi event data around, so it
  is freed immediately after processing.

DEPENDENCIES
  None

RETURN VALUE
  Void
  
SIDE EFFECTS
  None
===========================================================================*/
void simlock_handle_mmgsdi_evt
(
  void
)
{
  simlock_task_mmgsdi_cmd_type  *simlock_mmgsdi_cmd_ptr = NULL;

  simlock_mmgsdi_cmd_ptr = (simlock_task_mmgsdi_cmd_type*)(q_get(&simlock_mmgsdi_evt_q));

  if(simlock_mmgsdi_cmd_ptr == NULL)
  {
    return;
  }

  switch (simlock_mmgsdi_cmd_ptr->cmd_type)
  {
    case SIMLOCK_MMGSDI_CARD_EVENT_CB:
      /* MMGSDI Card event callback */
      simlock_card_event_cb(&simlock_mmgsdi_cmd_ptr->mmgsdi_event);
      break;

    default:
      /* Unknown command */
      break;
  }

  (void) rex_clr_sigs(rex_self(), SIMLOCK_MMGSDI_EVT_SIG);

  if (q_cnt(&simlock_mmgsdi_evt_q) > 0)
  {
    /*  Items on the queue. Set the signal */
    (void) rex_set_sigs(rex_self(), SIMLOCK_MMGSDI_EVT_SIG);
  }

  SIMLOCK_MEM_FREE(simlock_mmgsdi_cmd_ptr);
} /* simlock_handle_mmgsdi_evt */

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */

