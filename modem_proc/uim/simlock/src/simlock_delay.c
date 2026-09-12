/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   D E L A Y   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock delay timer functions.


EXTERNALIZED FUNCTIONS


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

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_delay.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/26/18   nr      Intial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "uim_common_efs.h"

#if defined(FEATURE_SIMLOCK) && defined(FEATURE_SIMLOCK_RSU)

#include "simlock_rsu_config.h"
#include "simlock_modem.h"
#include "pm_rtc.h"
#include "rex.h"

/* delay timer value default and max in seconds */
#define   SIMLOCK_DELAY_TIMER_DEFAULT_VALUE      120
#define   SIMLOCK_DELAY_TIMER_MAX_VALUE          600

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_DELAY_TIMER_STATE_TYPE

   DESCRIPTION:
     Describes the SimLock delay timer states
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_DELAY_TIMER_NOT_CREATED = 0x00,
  SIMLOCK_DELAY_TIMER_RUNNING,
  SIMLOCK_DELAY_TIMER_EXPIRED
} simlock_delay_timer_state_type;

static simlock_delay_timer_state_type  simlock_delay_timer_state = SIMLOCK_DELAY_TIMER_NOT_CREATED;
static rex_timer_type                 *simlock_delay_timer_ptr   = NULL;

/*===========================================================================
FUNCTION SIMLOCK_DELAY_STOP_DELAY_TIMER

DESCRIPTION
  This function takes care of stopping delay timer.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_delay_stop_delay_timer
(
  void
)
{
  simlock_result_enum_type     simlock_status = SIMLOCK_SUCCESS;
  simlock_task_cmd_type      * task_cmd_ptr   = NULL;

  simlock_delay_timer_state = SIMLOCK_DELAY_TIMER_EXPIRED;

  SIMLOCK_CHECK_AND_MEM_MALLOC(task_cmd_ptr,
                               sizeof(simlock_task_cmd_type));
  if (task_cmd_ptr == NULL)
  {
    return;
  }

  memset(task_cmd_ptr, 0x00, sizeof(simlock_task_cmd_type));

  task_cmd_ptr->cmd_type = SIMLOCK_DELAY_TIMER_EXPIRY_CMD;

  /* Put on SIM Lock command queue */
  simlock_status = simlock_queue_cmd(task_cmd_ptr);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put
       onto the command queue */
    SIMLOCK_MEM_FREE(task_cmd_ptr);
  }

  SIMLOCK_MSG_MED_1("Queue of SIMLOCK_DELAY_TIMER_EXPIRY_CMD to SIMLOCK, status 0x%x", simlock_status);
} /* simlock_delay_stop_delay_timer */


/*===========================================================================
FUNCTION SIMLOCK_DELAY_TIMER_EXPIRY

DESCRIPTION
  This function handles the trigger of the delay timer expiry.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_delay_timer_expiry
(
  unsigned long param
)
{
  (void)param;

  simlock_delay_stop_delay_timer();
} /* simlock_delay_timer_expiry */


/*===========================================================================
FUNCTION SIMLOCK_DELAY_DELETE_DELAY_TIMER

DESCRIPTION
  This function is to delete delay timer. This gets called upon
  receiving card error event from MMGSDI or upon receiving blob
  from unlock server.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  NONE
===========================================================================*/
void simlock_delay_delete_delay_timer
(
  void
)
{
  SIMLOCK_MSG_MED_0("SimLock delay timer deleted");

  if(simlock_delay_timer_ptr != NULL)
  {
    (void)rex_clr_timer(simlock_delay_timer_ptr);

    rex_delete_timer_ex(simlock_delay_timer_ptr);

    simlock_delay_timer_ptr = NULL;
  }

  simlock_delay_timer_state = SIMLOCK_DELAY_TIMER_NOT_CREATED;
} /* simlock_delay_delete_delay_timer */


/*===========================================================================
FUNCTION SIMLOCK_DELAY_IS_DELAY_TIMER_RUNNING

DESCRIPTION
  Check if simlock delay timer is running or not.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_delay_is_delay_timer_running
(
  void
)
{
  if(simlock_delay_timer_state == SIMLOCK_DELAY_TIMER_RUNNING)
  {
    return TRUE;
  }

  return FALSE;
} /* simlock_delay_is_delay_timer_running */


/*===========================================================================
FUNCTION SIMLOCK_DELAY_INITIALIZE_DELAY_TIMER

DESCRIPTION
  Creates/starts the delay timer.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_delay_initialize_delay_timer
(
  uint16    *delay_timer_value_in_sec_ptr
)
{
  simlock_result_enum_type            simlock_status          = SIMLOCK_SUCCESS;
  uint32                              timer_value             = SIMLOCK_DELAY_TIMER_DEFAULT_VALUE;
  rex_timer_cnt_type                  delay_timer_value_in_ms = 0;
  simlock_remote_lock_timestamp_type  time_data;

  if(delay_timer_value_in_sec_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(&time_data, 0x00, sizeof(time_data));
  *delay_timer_value_in_sec_ptr = 0;

  /* Check if a SimLock blob was ever received in the past.
     if yes no need to start initial delay timer*/
  simlock_status = simlock_rsu_config_read_time_file(&time_data);
  if(simlock_status == SIMLOCK_SUCCESS)
  {
    return SIMLOCK_RSU_INCOMPATIBLE_STATE;
  }

  /* Do not start delay timer if timer is already expired */
  if(simlock_delay_timer_state == SIMLOCK_DELAY_TIMER_EXPIRED)
  {
    return SIMLOCK_RSU_INCOMPATIBLE_STATE;
  }

  /* Do not start delay timer if timer is running */
  if(simlock_delay_timer_state == SIMLOCK_DELAY_TIMER_RUNNING)
  {
    return SIMLOCK_INVALID_STATE;
  }

  if(simlock_delay_timer_ptr != NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  (void)uim_common_efs_read(UIM_COMMON_EFS_RSU_DELAY_TIMER_VALUE,
                            UIM_COMMON_EFS_ITEM_FILE_TYPE,
                            UIM_COMMON_EFS_DEVICE,
                            (uint8 *)&timer_value,
                            sizeof(timer_value));

  if(timer_value == 0)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  if(timer_value > SIMLOCK_DELAY_TIMER_MAX_VALUE)
  {
    timer_value = SIMLOCK_DELAY_TIMER_MAX_VALUE;
  }

  delay_timer_value_in_ms = timer_value * SIMLOCK_MILLISECONDS_IN_A_SECOND;

  /* Start delay timer */
  simlock_delay_timer_ptr =
                rex_create_timer_ex(simlock_delay_timer_expiry, 0);
  if(simlock_delay_timer_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  (void)rex_set_timer(simlock_delay_timer_ptr, delay_timer_value_in_ms);
  simlock_delay_timer_state = SIMLOCK_DELAY_TIMER_RUNNING;

  simlock_send_delay_timer_event();

  *delay_timer_value_in_sec_ptr = timer_value;

  return SIMLOCK_SUCCESS;
} /* simlock_delay_initialize_delay_timer */

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */
