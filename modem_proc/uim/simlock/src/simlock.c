/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            G E N E R I C   S I M   L O C K   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2020, 2022 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock.c#8 $$ $DateTime: 2022/05/02 03:14:26 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/29/22   cj      Slot 1 configuration is considered by default for few slot policies
07/23/20   cj      Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD
01/31/20   ykr     Disabling slot as per SIMLOCK configuration
01/29/20   cj      Optimization of FR47581 changes
12/27/19   cj      Make device dual SIM to single SIM via RSU BLOB
09/13/19   vdc     SimLock solution for activation at Point Of Sale
03/12/19   kb      Inc retry count when wrong CK/HCK in ADDLOCK using CK/HCK
12/03/18   vgd     Move configuring pubkey from EFS to code for encrypting DEVICE_KEY
12/03/18   vgd     Added support for public keys for multiple RSU modes
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration 
07/24/17   nr      Reduce stack consumption 
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
02/21/17   vdc     Added F3 prints for HCK, Salt and iteration count
02/20/17   vdc     Added support to probe CK for correctness
12/01/16   vdc     Extend blob to support lock/unlock with HCK
11/28/16   vdc     Apply policies only if device is not in emergency only mode
11/03/16   bcho    Support for multi-app policy in SimLock for OR condition
10/28/16   nr      Support remote and local configurations together
10/06/16   ar      Added support for SP + EHPLMN simlock
09/02/16   vdc     Extend full and expired service policies on any slot
08/18/16   ar      Add support for simlock storage and emergency only mode
08/10/16   av      Reduce F3 logging in simlock
08/03/16   vdc     Added support for SPN based SIMLOCK feature
07/16/16   av      Postpone creation of simlock config files
05/20/16   vdc     Remove F3 messages for memory allocation failure
05/11/16   av      Skip checking simlock policies when no category is enabled
05/11/16   av      Assume default iteration cnt when not provided by caller
04/25/16   sp      Fix compiler warnings
03/21/16   av      Introduce support for SimLock fuse
02/19/16   vdc     Send control key presence in get config status
02/10/16   stv     Reject a lock request without ck and valid num of retires
01/25/16   ar      Change req_slot to Slot1, if slot policy is same for all
01/22/16   av      Support for autolock whitelist
01/07/16   stv     Remote simlock support
12/17/15   av      Introduced client reg and pre_policy valid IMSI check APIs
09/28/15   vv      Support for reuse code data
09/09/15   vv      Set appropriate status/feature, during ready to perso state
08/19/15   vv      Added support to retrieve retry attempts left for locking
08/17/15   vv      Re-locate the public key file
07/07/15   bcho    Redundant checks for SIMLOCK
06/30/15   stv     Unlock/Relock fail when locked with zero max retries
05/15/15   vv      Support for RSU set configuration in simlock
05/15/15   vv      Support for RSU in simlock
04/17/15   at      Update error code when max retries are reached
03/19/15   at      Introducing new relock feature in SIMLock
03/04/15   vv      Added support for new perso status
02/17/15   vv      Indicate the reason for invalid subscription
12/26/14   vv      Support for whitelist and blacklist codes in same category
12/17/14   vv      Relocate the simlock data files
11/10/14   vv      Add support for setting emergency mode for all the RATs
10/28/14   vv      Update the simlock init status in globals
10/03/14   vv      Do not allow temporary unlock if the lock is not set
09/29/14   vv      Added support for the remote SFS
09/29/14   vv      Added support for emergency only mode
09/18/14   tl      Change logic when determining if is card valid
09/17/14   tl      Add support for perso and deperso secure
08/29/14   vv      Added support for temporary unlock
08/27/14   tl      Introduce network related SIM Lock slot policies
08/25/14   tl      Introduce master slot 1 slot policy
06/20/14   vv      Added support for diag protection
04/15/14   vv      Added support for quad SIM
04/14/14   vv      Added support for identifying conflicting codes
03/25/14   tl      Secondary revision
02/14/14   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock.h"
#include "simlock_config.h"
#include "simlock_3gpp.h"
#include "simlock_3gpp2.h"
#include "simlock_crypto.h"
#include "simlock_file.h"
#include "simlock_util.h"
#include "simlock_category.h"
#include "simlock_category_conflicts.h"
#include "simlock_timer.h"
#include "simlock_cm.h"
#include "simlock_rsu_key.h"
#include "simlock_temp_unlock.h"
#include "simlock_time.h"
#include "simlock_validity.h"
#include "simlock_fuse.h"
#include "simlock_policy.h"
#include "simlock_mmgsdi.h"
#include "simlock_delay.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#else
#include "simlock_sfs.h"
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
#include "amssassert.h"
#include "simlock_config.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* If the user does not provide a num_iterations when enabling the lock
   with CK, we can use following as the default. Basically, any value
   10^r, where r ranges between 3 and 5 is a recommended value from
   security perspective... we chose r = 4. */
#define SIMLOCK_SHA256_DEFAULT_ITERATIONS                10000


/*===========================================================================
FUNCTION SIMLOCK_PRINT_LOCK_DATA

DESCRIPTION
  Prints the entire HCK, SALT data buffer and iteration count

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  VOID

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_print_lock_data
(
  const simlock_hck_type  *hck_data_ptr,
  const simlock_salt_type *salt_data_ptr,
  uint32                   iteration_cnt
)
{
  uint8  i = 0;

  if(hck_data_ptr == NULL || salt_data_ptr == NULL)
  {
    return;
  }

  for(i = 0; i < sizeof(simlock_hck_type) - 3; i += 4)
  {
    SIMLOCK_MSG_LOW_6("hck[%d-%d]:0x%x, 0x%x, 0x%x, 0x%x",
                      i,
                      i + 3,
                      hck_data_ptr[i],
                      hck_data_ptr[i + 1],
                      hck_data_ptr[i + 2],
                      hck_data_ptr[i + 3]);
  }

  for(i = 0; i < sizeof(simlock_salt_type) - 3; i += 4)
  {
    SIMLOCK_MSG_LOW_6("salt[%d-%d]:0x%x, 0x%x, 0x%x, 0x%x",
                      i,
                      i + 3,
                      salt_data_ptr[i],
                      salt_data_ptr[i + 1],
                      salt_data_ptr[i + 2],
                      salt_data_ptr[i + 3]);
  }

  SIMLOCK_MSG_LOW_1("Iteration count: 0x%x", iteration_cnt);
} /* simlock_print_lock_data */


/*===========================================================================
FUNCTION SIMLOCK_INIT

DESCRIPTION
  This is the init function expected to be called during the power-up. If
  config file is not already present, it will create the file.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_init
(
  void
)
{
  simlock_config_file_presence_enum_type config_file_presence
                                           = SIMLOCK_CONFIG_FILE_PRESENT;

  /* Check if the config file already exists. We allow the SimLock engine to
     proceed if the file is correct or if the file is missing and the fuse is
     not blown, which means that SimLock was never enabled before */
  config_file_presence = simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT);
  if(SIMLOCK_CONFIG_FILE_CORRUPTED == config_file_presence)
  {
    SIMLOCK_MSG_ERR_0("Config file or dir is corrupted");
    return SIMLOCK_GENERIC_ERROR;
  }
  else if(SIMLOCK_CONFIG_FILE_ABSENT == config_file_presence &&
          simlock_fuse_is_fuse_blown())
  {
    /* Config file is not present. At this point we should not
       continue further with the remaining SimLock init procedures
       if the SimLock fuse is blown.
       Note that the SimLock fuse is blown by SimLock once the lock
       is enabled successfully or if the SimLock config is moved to
       remote FS */
    SIMLOCK_MSG_ERR_0("Device is SimLock locked");
    return SIMLOCK_GENERIC_ERROR;
  }

#ifdef FEATURE_SIMLOCK_RSU
  (void)simlock_mmgsdi_init();

#ifdef FEATURE_SIMLOCK_RSU_TIME
  simlock_time_init();
  simlock_temp_unlock_init();
#endif /* FEATURE_SIMLOCK_RSU_TIME */
#endif /* FEATURE_SIMLOCK_RSU */

  return SIMLOCK_SUCCESS;
} /* simlock_init */


/* ==========================================================================
   FUNCTION:      SIMLOCK_UTIL_CONVERT_UIM_SLOT

   DESCRIPTION:
     This function converts the slot index into uim slot type

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:

==========================================================================*/
static void simlock_util_convert_uim_slot(
  uint8                     slot_index,
  uim_slot_type            *uim_slot_ptr)
{
  if(uim_slot_ptr == NULL)
  {
    return;
  }

  switch(slot_index)
  {
    case 0:
      *uim_slot_ptr = UIM_SLOT_1;
      break;
    case 1:
      *uim_slot_ptr = UIM_SLOT_2;
      break;
    case 2:
      *uim_slot_ptr = UIM_SLOT_3;
      break;
    default:
      *uim_slot_ptr = UIM_SLOT_NONE;
      break;
  }
}/* simlock_util_convert_uim_slot */


/* ==========================================================================
   FUNCTION:      SIMLOCK_UIM_REPORT

   DESCRIPTION:
     This dummy function is called when UIM report back command response.
     This is required since UIMDRIVERS need a callback function for external
     command

   DEPENDENCIES:
     None

   LIMITATIONS:


   RETURN VALUE:
     None

   SIDE EFFECTS:

==========================================================================*/
static void simlock_uim_report(
  uim_rpt_type *report_ptr
)
{
  (void)report_ptr;
}/* simlock_uim_report */


/* ==========================================================================
   FUNCTION:      SIMLOCK_SLOT_OPERATION_PUP_PDOWN

   DESCRIPTION:
     This function is to perform card power up/power down operation.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     simlock_result_enum_type

   SIDE EFFECTS:

==========================================================================*/
static simlock_result_enum_type simlock_slot_operation_pup_pdown
(
  uint8                   slot_index,
  boolean                 slot_disabled
)
{
  uim_cmd_type           *uim_cmd_ptr = NULL;
  uim_return_type         uim_status  = UIM_SUCCESS;

  SIMLOCK_CHECK_AND_MEM_MALLOC(uim_cmd_ptr, sizeof(uim_cmd_type));

  if (uim_cmd_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Convert to UIM slot.*/
  simlock_util_convert_uim_slot(slot_index, &uim_cmd_ptr->hdr.slot);

  if(slot_disabled)
  {
    /* Send card power down command to UIMDRV */
    uim_cmd_ptr->hdr.command        = UIM_POWER_DOWN_F;
    uim_cmd_ptr->hdr.protocol       = UIM_NO_SUCH_MODE;
    uim_cmd_ptr->pdown.pdown_option = UIM_OPTION_NOTIFY_PDOWN;
  }
  else
  {
    /* Send card power up command to UIMDRV */
    uim_cmd_ptr->hdr.protocol     = UIM_NO_SUCH_MODE;
    uim_cmd_ptr->hdr.channel      = UIM_CHANNEL0;
    uim_cmd_ptr->hdr.command      = UIM_RESET_F;
    uim_cmd_ptr->reset.pup_option = UIM_PUP_OPTION_TELECOM_NOT_PUP_IF_ALREADY;
  }

  uim_cmd_ptr->hdr.user_data          = 0;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = simlock_uim_report;
  (void)q_link(uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link);

  uim_status = uim_cmd(uim_cmd_ptr);

  if (uim_status != UIM_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("UIM command queuing failed for slot: 0x%x",
                      uim_cmd_ptr->hdr.slot);
  }

  SIMLOCK_MEM_FREE(uim_cmd_ptr);
  return SIMLOCK_SUCCESS;
}/* simlock_slot_operation_pup_pdown */


/* ==========================================================================
   FUNCTION:      SIMLOCK_SLOT_OPERATION

   DESCRIPTION:
     This function is to perform card power up/power down operation.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     simlock_result_enum_type

   SIDE EFFECTS:

==========================================================================*/
static simlock_result_enum_type simlock_slot_operation
(
  const simlock_disabled_slot_data_type *  disabled_slots_ptr
)
{
  uint8                     slot_index     = SIMLOCK_SLOT_COUNT_MAX;
  simlock_slot_enum_type    slot           = SIMLOCK_SLOT_1;
  simlock_result_enum_type  simlock_status = SIMLOCK_SUCCESS;

  if(disabled_slots_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  for (slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    if(SIMLOCK_SUCCESS != simlock_util_get_slot_index(slot, &slot_index) ||
       slot_index >= SIMLOCK_SLOT_COUNT_MAX)
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    if (simlock_slot_operation_pup_pdown(slot_index,
          disabled_slots_ptr->slot[slot_index]) != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_LOW_1("pup/pdown operation failed for simlock slot: 0x%x",
                        slot);
    }
  }

  return simlock_status;
}/* simlock_slot_operation */


/*===========================================================================
FUNCTION SIMLOCK_SET_SIMLOCK_POLICY

DESCRIPTION
  This function sets the slot and subscription policy for the SIMLock. It
  checks if the SIMLock is disabled. If yes, it writes the policy to the
  config file located in SFS. If not, it'll return an error.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_simlock_policy
(
  const simlock_set_config_policy_msg_req_type   * policy_msg_ptr
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type      slot                 = SIMLOCK_SLOT_1;
  boolean                     lock_disabled        = TRUE;
  simlock_config_data_type   *config_file_data_ptr = NULL;

  if(policy_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_2("simlock_set_simlock_policy slot_policy: 0x%x, sub_policy: 0x%x",
                    policy_msg_ptr->slot_policy, policy_msg_ptr->sub_policy);

  /* Return if config id is present in request and we already have default config file*/
  if(policy_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT &&
     (SIMLOCK_CONFIG_FILE_ABSENT != simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT) ||
      simlock_max_inactive_config_created()))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(policy_msg_ptr->config_id);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Reject set policy request if config id in non-zero and SFS type is remote */
  if(config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE &&
     policy_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT)
  {
    SIMLOCK_MSG_ERR_0("Remote SFS is not supported for inactive config");
    simlock_config_free_config_data(config_file_data_ptr);
    return SIMLOCK_UNSUPPORTED;
  }

  /* check if simlock is disabled only while policy is also getting set. If not, return error as the
     configuration policies can be set only when the simlock is
     disabled. Trigger is UIMDIAG interface here. */
  if(policy_msg_ptr->set_operation == SIMLOCK_SET_POLICY_OPERATION ||
     policy_msg_ptr->set_operation == SIMLOCK_SET_POLICY_AND_SLOT_CONFIG_OPERATION)
  {
  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    if(simlock_util_all_categories_disabled(slot,
                                            config_file_data_ptr,
                                            NULL,
                                            NULL,
                                            TRUE))
    {
      continue;
    }

    lock_disabled = FALSE;
    break;
  }
  }

  /* if simlock is not disabled, then policies cannot be set */
  if(!lock_disabled)
  {
    simlock_status = SIMLOCK_INVALID_STATE;
  }
  else
  {
    if(policy_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT)
    {
      config_file_data_ptr->nickname_len = policy_msg_ptr->config_nickname.nickname_len;
      simlock_memscpy(config_file_data_ptr->nickname, sizeof(config_file_data_ptr->nickname),
                      policy_msg_ptr->config_nickname.nickname, policy_msg_ptr->config_nickname.nickname_len);
    }

    /* Don't update disabled slot info if only set policy operation is triggered. */
    if(policy_msg_ptr->set_operation == SIMLOCK_SET_POLICY_OPERATION ||
       policy_msg_ptr->set_operation == SIMLOCK_SET_POLICY_AND_SLOT_CONFIG_OPERATION)
    {
    simlock_status = simlock_config_update_and_write_policies(policy_msg_ptr,
                                                              config_file_data_ptr, 
                                                              policy_msg_ptr->config_id);

    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("set policy failed");
    }
  }
    if (simlock_status == SIMLOCK_SUCCESS &&
        (policy_msg_ptr->set_operation == SIMLOCK_SET_SLOT_CONFIG_OPERATION ||
         policy_msg_ptr->set_operation == SIMLOCK_SET_POLICY_AND_SLOT_CONFIG_OPERATION))
    {
      /* update and write the slot configuration to the config file */
      simlock_status = simlock_config_update_and_write_slot_config(&policy_msg_ptr->disabled_slots,
                                                                   config_file_data_ptr);

      if(simlock_status == SIMLOCK_SUCCESS)
      {
        simlock_status = simlock_slot_operation(&policy_msg_ptr->disabled_slots);
      }
      else
      {
        SIMLOCK_MSG_ERR_0("set slot configuration failed");
      }
    }

  simlock_config_free_config_data(config_file_data_ptr);
  }

  return simlock_status;
} /* simlock_set_simlock_policy */


/*===========================================================================
FUNCTION  SIMLOCK_SET_SIMLOCK_RSU_MODE

DESCRIPTION
  This function set the simlock rsu mode data in simlock config file.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_simlock_rsu_mode
(
  const simlock_set_rsu_mode_msg_req_type  *mode_config_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_result_enum_type       simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type         slot                 = SIMLOCK_SLOT_1;
  boolean                        lock_disabled        = TRUE;
  simlock_config_data_type      *config_file_data_ptr = NULL;

  if(mode_config_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Read config file */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* check if simlock is disabled. If not, return error as the
     mode configuration can be set only when the simlock is
     disabled */
  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    if(simlock_util_all_categories_disabled(slot,
                                            config_file_data_ptr,
                                            NULL,
                                            NULL,
                                            TRUE))
    {
      continue;
    }

    lock_disabled = FALSE;
    break;
  }

  SIMLOCK_MSG_HIGH_2("current mode config: 0x%x, new mode config: 0x%x",
                     config_file_data_ptr->rsu_mode, mode_config_ptr->rsu_mode);

  /* if simlock is not disabled, then switching mode config cannot be allowed */
  if(!lock_disabled)
  {
    simlock_status = SIMLOCK_INVALID_STATE;
  }
  else if ((config_file_data_ptr->rsu_mode != SIMLOCK_RSU_MODE_NO_RSU &&
            mode_config_ptr->rsu_mode != SIMLOCK_RSU_MODE_NO_RSU ) ||
           config_file_data_ptr->rsu_mode == mode_config_ptr->rsu_mode)
  {
    /* Not possible to switch from one RSU mode to another directly.
       It's also not possible to switch to the same RSU mode */
    simlock_status = SIMLOCK_UNSUPPORTED;
  }
  else
  {
    /* Delete earlier configuration when mode switch occur to avoid using stale data */
    (void)simlock_file_delete_all_rsu_files();

    config_file_data_ptr->rsu_mode = mode_config_ptr->rsu_mode;

    /* Update rsu_mode in simlock config file */
    simlock_status = simlock_config_write_data(config_file_data_ptr,
                                               SIMLOCK_CONFIG_ID_DEFAULT);
  }

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
#else
  (void)mode_config_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_set_simlock_rsu_mode */


/*===========================================================================
FUNCTION SIMLOCK_RUN_ALGORITHM

DESCRIPTION
  This is the core function for the SIMLock feature. It validates a SIM
  card present, by verifing the card data against the lock data for all the
  enabled categories. It then applies the configuration policy
  (slot and sub policy), to determine whether a subscription is valid on the
  card.
  If the SIMLock feature is not enabled, all the subcriptions on all the
  cards are considered as valid

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_run_algorithm
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_run_algorithm_msg_resp_type        * resp_msg_ptr
)
{
  simlock_result_enum_type           simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type             slot                 = SIMLOCK_SLOT_1;
  uint8                              sub_cnt              = 0;
  uint8                              slot_index           = 0;
  simlock_config_data_type          *config_file_data_ptr = NULL;

  if((req_msg_ptr == NULL)  ||
     (resp_msg_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* validate the subscriptions on the available slots */
  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    simlock_status = simlock_util_get_slot_index(slot, &slot_index);
    if((simlock_status == SIMLOCK_SUCCESS) &&
       (slot_index < SIMLOCK_SLOT_COUNT_MAX))
    {
      /* Check if slot is disabled in config or not. If yes, power down the card in slot. */
      if(config_file_data_ptr->disabled_slots.slot[slot_index] &&
         req_msg_ptr->card_data[slot_index].num_subscriptions > 0)
      {
        (void)simlock_slot_operation_pup_pdown(slot_index, TRUE);
      }

      for(sub_cnt = 0;
          (sub_cnt < req_msg_ptr->card_data[slot_index].num_subscriptions &&
           sub_cnt < SIMLOCK_SUBS_COUNT_MAX);
          sub_cnt++)
      {
        if (req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt].sub_type != SIMLOCK_SUBSCRIPTION_3GPP &&
            req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt].sub_type != SIMLOCK_SUBSCRIPTION_3GPP2)
        {
          continue;
        }

        /* Check if slot is disabled in config or not. If yes, set the subscription to invalid. */
        if(config_file_data_ptr->disabled_slots.slot[slot_index])
        {
          resp_msg_ptr->card_validity_data[slot_index].num_subscriptions++;
          resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt].is_subscription_valid = FALSE; 
          resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt].failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_SLOT_DISABLED;
          continue;
        }

        if(req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt].sub_type ==
                                                                  SIMLOCK_SUBSCRIPTION_3GPP)
        {
          resp_msg_ptr->card_validity_data[slot_index].num_subscriptions++;
          simlock_status = simlock_3gpp_run_algorithm(&req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt],
                                                      slot,
                                                      config_file_data_ptr,
                                                      &resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt]);
        }
        else if(req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt].sub_type ==
                                                                  SIMLOCK_SUBSCRIPTION_3GPP2)
        {
          resp_msg_ptr->card_validity_data[slot_index].num_subscriptions++;
          simlock_status = simlock_3gpp2_run_algorithm(&req_msg_ptr->card_data[slot_index].subscription_data[sub_cnt],
                                                       slot,
                                                       config_file_data_ptr,
                                                       &resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt]);
        }
      } /* end for loop, sub_cnt = 0 */
    }
  } /* end for loop, slot = SIMLOCK_SLOT_1 */

  /* Update invalid IMSI list that contains IMSIs that are invalid prior to
     application of config policies */
  simlock_validity_update_imsi_info(req_msg_ptr, resp_msg_ptr, TRUE);

  /* Apply the config policies if device is not in emergency only mode */
  if(simlock_status == SIMLOCK_SUCCESS &&
     simlock_util_is_device_in_emergency_mode(config_file_data_ptr) == FALSE)
  {
#ifdef FEATURE_SIMLOCK_RSU
    /* In case of RSU MODE D, there is a delay to apply SimLock. In that case,
       revert back all subscriptions to valid */
    if(simlock_util_get_number_of_slots() == 1 &&
       config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_D)
    {
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX))
        {
          for(sub_cnt = 0;
              (sub_cnt < req_msg_ptr->card_data[slot_index].num_subscriptions &&
               sub_cnt < SIMLOCK_SUBS_COUNT_MAX);
              sub_cnt++)
          {
            if(!resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt].is_subscription_valid &&
               simlock_delay_is_delay_timer_running())
            {
              resp_msg_ptr->card_validity_data[slot_index].subscription_data[sub_cnt].is_subscription_valid = TRUE; 
            }
          }
        }
      }
    }
#endif /* FEATURE_SIMLOCK_RSU */

    simlock_status = simlock_apply_config_policy(req_msg_ptr,
                                                 config_file_data_ptr, resp_msg_ptr);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("apply config policy failed");
    }
  }

  /* Update valid IMSI list */
  simlock_validity_update_imsi_info(req_msg_ptr, resp_msg_ptr, FALSE);

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_run_algorithm */


/*===========================================================================
FUNCTION SIMLOCK_SWITCH_CONFIG

DESCRIPTION
  This function switches the configuration of the code data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_switch_config
(
  const simlock_change_list_type_msg_req_type * req_msg_ptr
)
{
  simlock_result_enum_type           simlock_status       = SIMLOCK_SUCCESS;
  simlock_category_enum_type         category             = SIMLOCK_CATEGORY_3GPP_NW;
  simlock_category_file_data_type   *category_data_ptr    = NULL;
  simlock_config_data_type          *config_file_data_ptr = NULL;
  boolean                            category_status_updated  = FALSE;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("Change configuration to: 0x%x", req_msg_ptr->blacklist);

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                               sizeof(simlock_category_file_data_type));
  if(category_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  simlock_status = SIMLOCK_GENERIC_ERROR;

  /* Since the call to change the configuration is called from RSU, categories
     are only checked for slot1.
     --If all categories are enabled then we need to update only code type.
     --If all categories are disabled but file present then we need to update
       code type and enable the categories as well.
  */
  for(category = SIMLOCK_CATEGORY_3GPP_NW; category <= SIMLOCK_CATEGORY_3GPP2_RUIM;
      category++)
  {
    /* If the category is NOT enabled and file is NOT present then nothing to do. */
    if(!simlock_util_check_category_enabled(config_file_data_ptr,
                                            category,
                                            req_msg_ptr->slot)    && 
       !simlock_util_check_category_disabled_file_present(config_file_data_ptr,
                                                          category,
                                            req_msg_ptr->slot))
    {
      continue;
    }

    memset(category_data_ptr, 0x00, sizeof(simlock_category_file_data_type));

    simlock_status = simlock_category_read_lock_data(category,
                                                     req_msg_ptr->slot,
                                                     category_data_ptr,
                                                     config_file_data_ptr->sfs_type,
                                                     SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    /* For each available category, change the following settings:
      1) Config #1 to Config #2:
          A) General code type to Blacklist.
      2) Config #2 to Config #1:
          A) General code type to Whitelist.
    */
    if(req_msg_ptr->blacklist)
    {
      category_data_ptr->category_header.code_type = SIMLOCK_CODE_BLACKLIST;
    }
    else
    {
      category_data_ptr->category_header.code_type = SIMLOCK_CODE_WHITELIST;
    }

    /* Write the category data */
    simlock_status = simlock_category_write_lock_data(req_msg_ptr->slot,
                                                      category_data_ptr,
                                                      config_file_data_ptr->sfs_type,
                                                      SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    if(simlock_util_check_category_disabled_file_present(config_file_data_ptr,
                                                         category,
                                                         req_msg_ptr->slot))
    {
      uint8  category_index = 0;
      uint8  slot_index     = 0;

      simlock_status = simlock_util_get_slot_index(req_msg_ptr->slot, &slot_index);
      if((simlock_status != SIMLOCK_SUCCESS) ||
         (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }
  
      simlock_status = simlock_util_get_category_index(category,
                                                       &category_index);
      if((simlock_status != SIMLOCK_SUCCESS) ||
         (category_index >= SIMLOCK_CATEGORY_COUNT_MAX))
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }

      /* enable the category in config file */
      simlock_status = simlock_config_update_and_write_category_status(
                                         category_index,
                                         slot_index,
                                         SIMLOCK_ENABLED,
                                         SIMLOCK_CK_NOT_PRESENT,
                                         config_file_data_ptr,
                                         config_file_data_ptr->sfs_type,
                                         SIMLOCK_CONFIG_ID_DEFAULT);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        break;
      }
      category_status_updated = TRUE;
    }
    simlock_category_free_code_data(category_data_ptr);
  }

  if(category_status_updated)
  {
    /* SimLock configuration has been successfully written. Blow the 
       SimLock fuse for enhanced security. The function below blows 
       the fuse only if it has not been blown yet. So, its ok to call 
       it even if fuse is already blown */
    if(SIMLOCK_SUCCESS != simlock_fuse_blow_fuse())
    {
      ASSERT(0);
    }
  }
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_switch_config */


/*===========================================================================
FUNCTION SIMLOCK_SET_LOCK_CK

DESCRIPTION
  This function sets and enables the SIMLock for a category. It takes the ck
  and the iteration count as the inputs and generates HCK needed for the
  lock. It also generates salt needed for generating the HCK. It then writes
  the lock data to a category file. Also, it updates the config file
  indicating that the lock is enabled for the category, upon successfully
  updating the lock data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_lock_ck
(
  const simlock_set_lock_ck_msg_req_type  * req_msg_ptr,
  simlock_token_msg_resp_type             * resp_msg_ptr
)
{
  simlock_result_enum_type               simlock_status       = SIMLOCK_SUCCESS;
  uint8                                  slot_index           = 0;
  simlock_slot_enum_type                 slot_id              = SIMLOCK_SLOT_1;
  uint8                                  category_index       = 0;
  simlock_category_file_data_type      * category_data_ptr    = NULL;
  simlock_salt_type                      salt_data_buf;
  simlock_hck_type                       hck_data_buf;
  simlock_config_data_type             * config_file_data_ptr = NULL;
  uint32                                 iteration_cnt        = SIMLOCK_SHA256_DEFAULT_ITERATIONS;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_set_lock_ck for category: 0x%x",
                    req_msg_ptr->category.category_type);

  memset(salt_data_buf, 0x00, SIMLOCK_SALT_LEN);
  memset(hck_data_buf, 0x00, SIMLOCK_HCK_LEN);

  if(req_msg_ptr->iteration_cnt > 0)
  {
    iteration_cnt = req_msg_ptr->iteration_cnt;
  }

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(
                          req_msg_ptr->category.category_type);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* if the auto_lock is enabled, then the lock codes are set from the
     SIM card. So in this case, codes cannot be blacklist */
  if((req_msg_ptr->auto_lock) &&
     (req_msg_ptr->blacklist))
  {
    SIMLOCK_MSG_ERR_1("both autolock and blacklist are enabled, for category, 0x%x",
                      req_msg_ptr->category.category_type);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* if the auto_lock is enabled, then the lock codes are set from the
     SIM card. So in this case, codes cannot be reused */
  if((req_msg_ptr->auto_lock) &&
     (req_msg_ptr->reuse_previous_code_list))
  {
    SIMLOCK_MSG_ERR_1("both autolock and reuse code are enabled, for category, 0x%x",
                      req_msg_ptr->category.category_type);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Return error if auto-lock is enabled for SP EHPLMN category */
  if (req_msg_ptr->auto_lock &&
      req_msg_ptr->category.category_type == SIMLOCK_CATEGORY_3GPP_SP_EHPLMN)
  {
    SIMLOCK_MSG_ERR_0("Auto-lock is not supported for SP EHPLMN category");
    return SIMLOCK_INVALID_LOCK_DATA;
  }

  if(req_msg_ptr->ck.simlock_ck_length == 0 &&
     req_msg_ptr->num_retries_max != 0)
  {
    SIMLOCK_MSG_ERR_1("Retries should be zero for lock without ck 0x%x", req_msg_ptr->num_retries_max);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Return if config id is present in request and we already have default config file*/
  if(req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT &&
     (SIMLOCK_CONFIG_FILE_ABSENT != simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT) ||
      simlock_max_inactive_config_created()))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(req_msg_ptr->config_id);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    /* Reject lock request if config id in non-zero and SFS type is remote */
    if(config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE &&
       req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT)
    {
      SIMLOCK_MSG_ERR_0("Remote SFS is not supported for inactive config");
      simlock_status = SIMLOCK_UNSUPPORTED;
      break;
    }

    /* Reject lock request if remote SFS is disabled and lock request from TZ.
       This never happens just a defensive check */
    if(config_file_data_ptr->sfs_type == SIMLOCK_SFS_LOCAL &&
       req_msg_ptr->sfs_type == SIMLOCK_SFS_REMOTE)
    {
      SIMLOCK_MSG_ERR_0("Remote SFS is not enabled");
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* if the slot policy is same on all the available slots, then
       it is expected to have one category file per each enabled category
       i.e. all the codes for that category are in one file */
    if(simlock_policy_uses_slot1_config_for_all_slots(config_file_data_ptr->slot_policy)
        != FALSE)
    {
      slot_id = SIMLOCK_SLOT_1;
    }
    else
    {
      slot_id = req_msg_ptr->slot;
    }

    simlock_status = simlock_util_get_slot_index(slot_id, &slot_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_status = simlock_util_get_category_index(req_msg_ptr->category.category_type,
                                                     &category_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (category_index >= SIMLOCK_CATEGORY_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* check if the category is already enabled */
    if(simlock_util_check_category_enabled(config_file_data_ptr,
                                           req_msg_ptr->category.category_type,
                                           slot_id))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x already enabled",
                        req_msg_ptr->category.category_type);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* Codes need to be validated in both of the following cases:
       - autolock is not enabled
       - autolock is enabled with a non-zero list of codes (in this case, the
         code data is the whitelist of codes such that once the simcard is
         inserted, if the simcard data matches any of the entry in the whitelist,
         the device is autolocked to that simcard */
    if(req_msg_ptr->reuse_previous_code_list == FALSE)
    {
      /* check if the code data chars are valid. If auto_lock is TRUE, code data
         may be empty - that is not an error condition */
      simlock_status = simlock_util_check_code_data(&req_msg_ptr->category,
                                                    req_msg_ptr->auto_lock);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("invalid code data for the category, 0x%x",
                          req_msg_ptr->category.category_type);
        break;
      }

      /* check if the codes are conflicting */
      simlock_status = simlock_category_verify_codes_for_conflicts(&req_msg_ptr->category,
                                                                   config_file_data_ptr,
                                                                   slot_id,
                                                                   req_msg_ptr->config_id);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("conflicting code data for the category, 0x%x",
                          req_msg_ptr->category.category_type);
        break;
      }
    }

    if(req_msg_ptr->ck.simlock_ck_length > 0)
    {
      /* generate the random salt */
      simlock_status = simlock_crypto_generate_salt(salt_data_buf);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("salt generation failed, status: 0x%x", simlock_status);
        break;
      }

      /* generate the HCK by running the PBKDF2 algo */
      simlock_status = simlock_crypto_run_pbkdf2_algorithm((const uint8 *)req_msg_ptr->ck.simlock_ck_data,
                                                           (uint16)req_msg_ptr->ck.simlock_ck_length,
                                                           salt_data_buf,
                                                           SIMLOCK_SALT_LEN,
                                                           iteration_cnt,
                                                           hck_data_buf,
                                                           SIMLOCK_HCK_LEN);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("hck generation failed, status: 0x%x", simlock_status);
        break;
      }

      SIMLOCK_MSG_LOW_0("**HCK generated**");

      simlock_print_lock_data(&hck_data_buf, &salt_data_buf, iteration_cnt);
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    if(!req_msg_ptr->reuse_previous_code_list)
    {
      simlock_status = simlock_category_build_lock_data(req_msg_ptr->auto_lock,
                                                        req_msg_ptr->blacklist,
                                                        req_msg_ptr->num_retries_max,
                                                        iteration_cnt,
                                                        &req_msg_ptr->category,
                                                        salt_data_buf,
                                                        hck_data_buf,
                                                        category_data_ptr);
    }
    else
    {
      /* Read the lock code header and data */
      simlock_status = simlock_category_read_lock_data(req_msg_ptr->category.category_type,
                                                       slot_id,
                                                       category_data_ptr,
                                                       req_msg_ptr->sfs_type,
                                                       SIMLOCK_CONFIG_ID_DEFAULT);
      if(simlock_status == SIMLOCK_SUCCESS)
      {
        simlock_status = simlock_category_build_header_data(req_msg_ptr->category.category_type,
                                                            req_msg_ptr->auto_lock,
                                                            req_msg_ptr->blacklist,
                                                            req_msg_ptr->num_retries_max,
                                                            iteration_cnt,
                                                            salt_data_buf,
                                                            hck_data_buf,
                                                            category_data_ptr);
      }
    }

    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("build/read lock data failed, status: 0x%x", simlock_status);
      break;
    }

    /* Update header with token and config with config_nickname */
    if(req_msg_ptr->config_id && resp_msg_ptr != NULL)
    {
      category_data_ptr->category_header.token.token_present = TRUE;

      simlock_status = simlock_crypto_get_random_data(
                         category_data_ptr->category_header.token.token_data,
                         sizeof(category_data_ptr->category_header.token.token_data));

      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("Token creation filed, status: 0x%x", simlock_status);
        break;
      }

      resp_msg_ptr->token.token_present = TRUE;
      simlock_memscpy(resp_msg_ptr->token.token_data, sizeof(resp_msg_ptr->token.token_data),
                      category_data_ptr->category_header.token.token_data, SIMLOCK_TOKEN_LEN);

      config_file_data_ptr->nickname_len = req_msg_ptr->config_nickname.nickname_len;
      simlock_memscpy(config_file_data_ptr->nickname, sizeof(config_file_data_ptr->nickname),
                      req_msg_ptr->config_nickname.nickname, req_msg_ptr->config_nickname.nickname_len);
    }

    /* write the category data */
    simlock_status = simlock_category_write_lock_data(slot_id,
                                                      category_data_ptr,
                                                      req_msg_ptr->sfs_type,
                                                      req_msg_ptr->config_id);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      simlock_ck_present_type ck_present = SIMLOCK_CK_NOT_PRESENT;
  
      if(req_msg_ptr->ck.simlock_ck_length > 0)
      {
        ck_present = SIMLOCK_CK_PRESENT;
      }
  
      /* enable the category in config file */
      simlock_status = simlock_config_update_and_write_category_status(
                                         category_index,
                                         slot_index,
                                         SIMLOCK_ENABLED,
                                         ck_present,
                                         config_file_data_ptr,
                                         req_msg_ptr->sfs_type,
                                         req_msg_ptr->config_id);
      if(simlock_status == SIMLOCK_SUCCESS && 
         req_msg_ptr->config_id == SIMLOCK_CONFIG_ID_DEFAULT)
      {
        /* SimLock configuration has been successfully written. Blow the SimLock
           fuse for enhanced security. The function below blows the fuse only
           if it has not been blown yet. So, its ok to call it everytime the set
           lock ck API is called */
        if(SIMLOCK_SUCCESS != simlock_fuse_blow_fuse())
        {
          ASSERT(0);
        }
      }
      else
      {
        SIMLOCK_MSG_ERR_1("update category status failed, status: 0x%x",
                          simlock_status);
      }
    }
  }while(0);

  /* this frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_set_lock_ck */


/*===========================================================================
FUNCTION SIMLOCK_UPDATE_RETRIES_AFTER_FAILURE_AND_WRITE

DESCRIPTION
  This function updates the current retries for the given slot and category
  after a wrong attempt. It also writes the updated information to the SFS.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type
    - SIMLOCK_INCORRECT_KEY:       If the number of remaining attempts is
                                   greater than 0.
    - SIMLOCK_MAX_RETRIES_REACHED: If the number of remaining attempts is 0.

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_update_retries_after_failure_and_write
(
  simlock_category_file_data_type  *category_data_ptr,
  simlock_slot_enum_type            slot_id,
  simlock_sfs_enum_type             sfs_type,
  uint32                           *curr_retries_ptr,
  uint32                           *num_retries_max_ptr,
  uint8                             config_id
)
{
  simlock_result_enum_type simlock_status = SIMLOCK_INCORRECT_KEY;

  SIMLOCK_MSG_ERR_0("Mismatch in key data, update retries & write to SFS");

  if(category_data_ptr   == NULL ||
     curr_retries_ptr    == NULL ||
     num_retries_max_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  if(category_data_ptr->category_header.num_retries_max != 0)
  {
    category_data_ptr->category_header.curr_retries++;

    if (SIMLOCK_SUCCESS == simlock_category_write_lock_data(slot_id,
                                                            category_data_ptr,
                                                            sfs_type,
                                                            config_id))
    {
      *num_retries_max_ptr = category_data_ptr->category_header.num_retries_max;
      *curr_retries_ptr    = category_data_ptr->category_header.curr_retries;

      /* Update error code if max retries reached for this attempt */
      if(category_data_ptr->category_header.curr_retries >=
           category_data_ptr->category_header.num_retries_max)
      {
        simlock_status = SIMLOCK_MAX_RETRIES_REACHED;
      }
    }
  }

  SIMLOCK_MSG_MED_3("max_retries: 0x%x, curr_retries: 0x%x, simlock_status: 0x%x",
                     *num_retries_max_ptr,
                     *curr_retries_ptr,
                     simlock_status);

  return simlock_status;
} /* simlock_update_retries_after_failure_and_write */


/*===========================================================================
FUNCTION SIMLOCK_RELOCK_DEVICE

DESCRIPTION
  This function reenables the SIMLock for the passed category with the input
  CK. As a first step it makes sure that the category is disabled & its
  data - like salt, HCK is present in the file. Then HCK is generated using
  the input CK, stored salt & iteration count. Relock is allowed only when
  the generated & stored HCK matches. Finally, data related to retries & the
  category is updated in the respective files.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_relock_device
(
  const simlock_relock_device_msg_req_type  * req_msg_ptr,
  simlock_relock_device_msg_resp_type       * resp_msg_ptr
)
{
  simlock_result_enum_type               simlock_status       = SIMLOCK_SUCCESS;
  uint8                                  slot_index           = 0;
  uint8                                  category_index       = 0;
  simlock_category_file_data_type      * category_data_ptr    = NULL;
  simlock_hck_type                       hck_data_buf;
  simlock_config_data_type             * config_file_data_ptr = NULL;

  if((req_msg_ptr == NULL) || (resp_msg_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* Cannot proceed if there in no CK passed */
  if(req_msg_ptr->ck.simlock_ck_length == 0)
  {
    SIMLOCK_MSG_ERR_0("input parameters error, 0 simlock_ck_length");
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_relock_device for category: 0x%x",
                    req_msg_ptr->category_type);

  memset(hck_data_buf, 0x00, SIMLOCK_HCK_LEN);

  /* Read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    SIMLOCK_MSG_ERR_0("read config file failed");
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    simlock_status = simlock_util_get_slot_index(req_msg_ptr->slot, &slot_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_status = simlock_util_get_category_index(req_msg_ptr->category_type,
                                                     &category_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (category_index >= SIMLOCK_CATEGORY_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* If the category is still enabled, cannot relock */
    if(simlock_util_check_category_enabled(config_file_data_ptr,
                                           req_msg_ptr->category_type,
                                           req_msg_ptr->slot))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x already enabled",
                        req_msg_ptr->category_type);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* If the category doesn't have stored CK data irrespective of how the lock
       was enabled/disabled, we cannot proceed */
    if(config_file_data_ptr->category_status[slot_index][category_index].ck_present
         == SIMLOCK_CK_NOT_PRESENT)
    {
      SIMLOCK_MSG_ERR_1("category has no stored CK: 0x%x", req_msg_ptr->category_type);
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* Now get the category's data */
    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    /* Read the lock code header and data */
    simlock_status = simlock_category_read_lock_data(req_msg_ptr->category_type,
                                                     req_msg_ptr->slot,
                                                     category_data_ptr,
                                                     req_msg_ptr->sfs_type,
                                                     SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    /* If the current unlock attempts count is equal to maximum
       retries allowed then return error */
    if(category_data_ptr->category_header.num_retries_max != 0)
    {
      if(category_data_ptr->category_header.curr_retries ==
           category_data_ptr->category_header.num_retries_max)
      {
        SIMLOCK_MSG_ERR_0("relock attempts expired");
        resp_msg_ptr->num_retries_max = category_data_ptr->category_header.num_retries_max;
        resp_msg_ptr->curr_retries = category_data_ptr->category_header.curr_retries;

        simlock_status = SIMLOCK_MAX_RETRIES_REACHED;
        break;
      }
    }

    SIMLOCK_MSG_LOW_0("**HCK from file**");

    simlock_print_lock_data(&category_data_ptr->category_header.hck,
                            &category_data_ptr->category_header.salt,
                            category_data_ptr->category_header.iteration_cnt);

    /* generate the HCK by running the PBKDF2 algo */
    simlock_status = simlock_crypto_run_pbkdf2_algorithm((uint8 *)req_msg_ptr->ck.simlock_ck_data,
                                                         (uint16)req_msg_ptr->ck.simlock_ck_length,
                                                         category_data_ptr->category_header.salt,
                                                         SIMLOCK_SALT_LEN,
                                                         category_data_ptr->category_header.iteration_cnt,
                                                         hck_data_buf,
                                                         SIMLOCK_HCK_LEN);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("hck generation failed, status: 0x%x", simlock_status);
      break;
    }

    SIMLOCK_MSG_LOW_0("**HCK generated**");

    simlock_print_lock_data(&hck_data_buf,
                            &category_data_ptr->category_header.salt,
                            category_data_ptr->category_header.iteration_cnt);

    /* If the stored HCK matches the generated HCK, proceed to lock */
    if(memcmp(hck_data_buf, category_data_ptr->category_header.hck, SIMLOCK_HCK_LEN) == 0)
    {
      SIMLOCK_MSG_ERR_0("HCK matches");

      /* Update the retries count */
      category_data_ptr->category_header.curr_retries = 0;

      /* Write the category data */
      simlock_status = simlock_category_write_lock_data(req_msg_ptr->slot,
                                                        category_data_ptr,
                                                        req_msg_ptr->sfs_type,
                                                        SIMLOCK_CONFIG_ID_DEFAULT);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("write lock data failed, status: 0x%x", simlock_status);
        simlock_status = SIMLOCK_GENERIC_ERROR;
      }
      else
      {
        /* Enable the category in config file */
        simlock_status = simlock_config_update_and_write_category_status(
                                         category_index,
                                         slot_index,
                                         SIMLOCK_ENABLED,
                                         SIMLOCK_CK_PRESENT,
                                         config_file_data_ptr,
                                         req_msg_ptr->sfs_type,
                                         SIMLOCK_CONFIG_ID_DEFAULT);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          SIMLOCK_MSG_ERR_1("Update category status failed, status: 0x%x",
                            simlock_status);
          simlock_status = SIMLOCK_GENERIC_ERROR;
        }
      }

      if(simlock_status == SIMLOCK_SUCCESS)
      {
        /* Send retries back in case of successful relocking */
        resp_msg_ptr->num_retries_max =
          category_data_ptr->category_header.num_retries_max;
        resp_msg_ptr->curr_retries =
          category_data_ptr->category_header.curr_retries;

        SIMLOCK_MSG_MED_2("max_retries: 0x%x, curr_retries: 0x%x",
                           resp_msg_ptr->num_retries_max,
                           resp_msg_ptr->curr_retries);
      }
    }
    else
    {
      simlock_status = simlock_update_retries_after_failure_and_write(
                         category_data_ptr,
                         req_msg_ptr->slot,
                         req_msg_ptr->sfs_type,
                         &resp_msg_ptr->curr_retries,
                         &resp_msg_ptr->num_retries_max,
                         SIMLOCK_CONFIG_ID_DEFAULT);
    }
  }while(0);

  /* This frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_relock_device */


/*===========================================================================
FUNCTION SIMLOCK_UNLOCK_DEVICE

DESCRIPTION
  This function disables the SIMLock for a category. It takes the ck
  as the input. It reads the category file to retrieve the salt,
  iteration count and the HCK. It generates the HCK using the input ck and
  retrieved salt and iteration count. If the generated and retrieved HCK
  matches, it unlocks the device for the category. Also, it updates the
  config file indicating that the lock is disabled for the category,
  upon successfully unlocking.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_unlock_device
(
  const simlock_unlock_device_msg_req_type   * req_msg_ptr,
  simlock_unlock_device_msg_resp_type        * resp_msg_ptr
)
{
  simlock_result_enum_type            simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type              slot_id              = SIMLOCK_SLOT_1;
  uint8                               slot_index           = 0;
  uint8                               category_index       = 0;
  simlock_category_file_data_type  *  category_data_ptr    = NULL;
  simlock_sfs_enum_type               sfs_type             = SIMLOCK_SFS_LOCAL;
  simlock_hck_type                    hck_data_buf;
  simlock_config_data_type          * config_file_data_ptr = NULL;

  if(req_msg_ptr == NULL ||
     resp_msg_ptr == NULL ||
     (req_msg_ptr->ignore_ck && req_msg_ptr->probe_ck))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_unlock_device for category: 0x%x",
                    req_msg_ptr->category);

  memset(hck_data_buf, 0x00, SIMLOCK_HCK_LEN);

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    /* if the slot policy is same on all the available slots, then
       it is expected to have one category file per each enabled category
       i.e. all the codes for that category are in one file */
    if(simlock_policy_uses_slot1_config_for_all_slots(config_file_data_ptr->slot_policy) 
        != FALSE)
    {
      slot_id = SIMLOCK_SLOT_1;
    }
    else
    {
      slot_id = req_msg_ptr->slot;
    }

    simlock_status = simlock_util_get_slot_index(slot_id, &slot_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_status = simlock_util_get_category_index(req_msg_ptr->category,
                                                     &category_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (category_index >= SIMLOCK_CATEGORY_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* check if the category is enabled. If not error */
    if(!simlock_util_check_category_enabled(config_file_data_ptr,
                                            req_msg_ptr->category,
                                            slot_id))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x not enabled",
                        req_msg_ptr->category);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* when a category is locked without CK we should not allow it to be unlocked with CK */
    if(config_file_data_ptr->category_status[slot_index][category_index].ck_present == SIMLOCK_CK_NOT_PRESENT &&
       req_msg_ptr->ck.simlock_ck_length > 0)
    {
      SIMLOCK_MSG_ERR_1("category was securely locked without CK: 0x%x",
                        req_msg_ptr->category);
      /* Decreasing the number of retries is not required when category is locked without CK as it
         provides no real value. */
      simlock_status = SIMLOCK_INCORRECT_KEY;
      break;
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                     req_msg_ptr->category,
                                                     slot_id);
    /* read the lock code header and data */
    simlock_status = simlock_category_read_lock_data(req_msg_ptr->category,
                                                     slot_id,
                                                     category_data_ptr,
                                                     sfs_type,
                                                     SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    /* if the current unlock attempts count is equal to maximum
       retries allowed then return error */
    if(category_data_ptr->category_header.num_retries_max != 0)
    {
      if(category_data_ptr->category_header.curr_retries ==
         category_data_ptr->category_header.num_retries_max)
      {
        SIMLOCK_MSG_ERR_0("unlock attempts expired");
        resp_msg_ptr->num_retries_max =
           category_data_ptr->category_header.num_retries_max;
        resp_msg_ptr->curr_retries =
           category_data_ptr->category_header.curr_retries;

        simlock_status = SIMLOCK_MAX_RETRIES_REACHED;
        break;
      }
    }

    SIMLOCK_MSG_LOW_0("**HCK from file**");

    simlock_print_lock_data(&category_data_ptr->category_header.hck,
                            &category_data_ptr->category_header.salt,
                            category_data_ptr->category_header.iteration_cnt);

    SIMLOCK_MSG_MED_1("num_codes: 0x%x",
                      category_data_ptr->category_header.num_of_codes);

    if(req_msg_ptr->ck.simlock_ck_length > 0)
    {
      if(category_data_ptr->category_header.iteration_cnt == 0)
      {
        SIMLOCK_MSG_ERR_0("invalid lock data");
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }

      /* generate the HCK by running the PBKDF2 algo */
      simlock_status = simlock_crypto_run_pbkdf2_algorithm((const uint8 *)req_msg_ptr->ck.simlock_ck_data,
                                                           (uint16)req_msg_ptr->ck.simlock_ck_length,
                                                           category_data_ptr->category_header.salt,
                                                           SIMLOCK_SALT_LEN,
                                                           category_data_ptr->category_header.iteration_cnt,
                                                           hck_data_buf,
                                                           SIMLOCK_HCK_LEN);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("hck generation failed, status: 0x%x", simlock_status);
        break;
      }

      SIMLOCK_MSG_LOW_0("**HCK generated**");

      simlock_print_lock_data(&hck_data_buf,
                              &category_data_ptr->category_header.salt,
                              category_data_ptr->category_header.iteration_cnt);
    }

    /* Allow unlock in following conditions:
       1) If CK needs to be ignored (Remote simlock version 2).
       2) If CK length is zero and CK is not present.
       3) If CK length is non-zero and HCK comparison is successful. */
    if(req_msg_ptr->ignore_ck ||
       (req_msg_ptr->ck.simlock_ck_length == 0 &&
        config_file_data_ptr->category_status[slot_index][category_index].ck_present == SIMLOCK_CK_NOT_PRESENT)||
       (memcmp(hck_data_buf, category_data_ptr->category_header.hck, SIMLOCK_HCK_LEN) == 0))
    {
      SIMLOCK_MSG_MED_0("key data matched");

      /* Update category status to disabled only if the request is to unlock */
      if(req_msg_ptr->probe_ck == FALSE)
      {
        /* update the config file with category status as disabled */
        simlock_status = simlock_config_update_and_write_category_status(category_index,
                                                                         slot_index,
                                                                         SIMLOCK_DISABLED_FILE_PRESENT,
                                                                         config_file_data_ptr->category_status[slot_index][category_index].ck_present,
                                                                         config_file_data_ptr,
                                                                         sfs_type,
                                                                         SIMLOCK_CONFIG_ID_DEFAULT);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          SIMLOCK_MSG_ERR_1("update config file failed, status: 0x%x", simlock_status);
        }
        else
        {
          /* in case the category is successfully unlocked while
             the temporary unlock is currently active, stop the timer */
          (void)simlock_timer_stop(slot_id, req_msg_ptr->category);

          /* Also, reset the retries count & write the data back */
          category_data_ptr->category_header.curr_retries = 0;

          /* Write the lock data */
          simlock_status = simlock_category_write_lock_data(slot_id,
                                                            category_data_ptr,
                                                            sfs_type,
                                                            SIMLOCK_CONFIG_ID_DEFAULT);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            SIMLOCK_MSG_ERR_0("write lock data failed");
          }
        }
      }
    }
    else
    {
      simlock_status = simlock_update_retries_after_failure_and_write(
                         category_data_ptr,
                         slot_id,
                         sfs_type,
                         &resp_msg_ptr->curr_retries,
                         &resp_msg_ptr->num_retries_max,
                         SIMLOCK_CONFIG_ID_DEFAULT);
    }
  }while(0);

  /* this frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_unlock_device */


/*===========================================================================
FUNCTION SIMLOCK_ACTIVATE_CONFIG_REQUEST

DESCRIPTION
  This function activates inactive config based on config id
  supported

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_activate_config_request
(
  const simlock_activate_config_msg_req_type * req_msg_ptr
)
{
  simlock_result_enum_type  simlock_status  = SIMLOCK_SUCCESS;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_activate_config_request for id: 0x%x",
                    req_msg_ptr->config_id);

  simlock_status = simlock_activate_inactive_config(req_msg_ptr->config_id);

  if(simlock_status == SIMLOCK_SUCCESS)
  {
    if(SIMLOCK_SUCCESS != simlock_delete_inactive_config())
    {
      SIMLOCK_MSG_MED_0("simlock_delete_inactive_config failed");
    }
  }
  else
  {
    SIMLOCK_MSG_ERR_2("simlock_activate_config_request failed for id: 0x%x status: 0x%x",
                      req_msg_ptr->config_id,
                      simlock_status);
  }

  return simlock_status;
} /* simlock_activate_config_request */


/*===========================================================================
FUNCTION SIMLOCK_GET_TOKEN_REQUEST

DESCRIPTION
  This function retrieves the token of all the categories on all the slots
  supported

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_get_token_request
(
  simlock_get_token_msg_resp_type * resp_msg_ptr
)
{
  simlock_result_enum_type          simlock_status          = SIMLOCK_SUCCESS;
  simlock_slot_enum_type            slot                    = SIMLOCK_SLOT_1;
  simlock_category_enum_type        category                = SIMLOCK_CATEGORY_3GPP_NW;
  uint8                             slot_index              = 0;
  uint8                             category_index          = 0;
  simlock_sfs_enum_type             sfs_type                = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type         *config_file_data_ptr    = NULL;
  simlock_category_file_data_type   category_file_data;

  if(resp_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(&category_file_data, 0x00,
            sizeof(simlock_category_file_data_type));

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    simlock_status = simlock_util_get_slot_index(slot, &slot_index);
    if((simlock_status == SIMLOCK_SUCCESS) &&
       (slot_index < SIMLOCK_SLOT_COUNT_MAX))
    {
      for(category = SIMLOCK_CATEGORY_3GPP_NW; category <= SIMLOCK_CATEGORY_3GPP2_RUIM;
          category++)
      {
        simlock_status = simlock_util_get_category_index(category, &category_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (category_index < SIMLOCK_CATEGORY_COUNT_MAX))
        {
          memset(&category_file_data, 0x00,
                 sizeof(simlock_category_file_data_type));

          /* check if a category is disabled. If yes, continue */
          if(!simlock_util_check_category_enabled(config_file_data_ptr, category, slot))
          {
            resp_msg_ptr->slot_token_data[slot_index].category_token_data[category_index].token_present = FALSE;

            memset(resp_msg_ptr->slot_token_data[slot_index].category_token_data[category_index].token_data,
                   0x00,
                   SIMLOCK_TOKEN_LEN);
            continue;
          }

          sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                           category,
                                                           slot);
          /* read the lock data */
          simlock_status = simlock_category_read_lock_data(category, slot,
                                                           &category_file_data,
                                                           sfs_type,
                                                           SIMLOCK_CONFIG_ID_DEFAULT);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            SIMLOCK_MSG_ERR_2("read lock data failed, category: 0x%x,"
                              " slot: 0x%x", category, slot);

            /* this frees the code data in the category */
            simlock_category_free_code_data(&category_file_data);
            continue;
          }

          resp_msg_ptr->slot_token_data[slot_index].category_token_data[category_index].token_present = 
            category_file_data.category_header.token.token_present;

          simlock_memscpy(resp_msg_ptr->slot_token_data[slot_index].category_token_data[category_index].token_data,
                          SIMLOCK_TOKEN_LEN,
                          category_file_data.category_header.token.token_data,
                          SIMLOCK_TOKEN_LEN);

          simlock_category_free_code_data(&category_file_data);
        }
      } /* end loop categories */
    }
  } /* end loop for slots */

  simlock_config_free_config_data(config_file_data_ptr);

  return SIMLOCK_SUCCESS;
} /* simlock_get_token_request */


/*===========================================================================
FUNCTION SIMLOCK_GET_LOCK_STATUS

DESCRIPTION
  This function retrieves the status of all the categories on all the slots
  supported

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_get_lock_status
(
  const simlock_get_status_msg_req_type *req_msg_ptr,
  simlock_get_status_msg_resp_type      *resp_msg_ptr
)
{
  simlock_result_enum_type          simlock_status          = SIMLOCK_SUCCESS;
  simlock_slot_enum_type            slot                    = SIMLOCK_SLOT_1;
  simlock_slot_enum_type            locked_slot             = SIMLOCK_SLOT_1;
  simlock_category_enum_type        category                = SIMLOCK_CATEGORY_3GPP_NW;
  uint8                             slot_index              = 0;
  uint8                             category_index          = 0;
  simlock_sfs_enum_type             sfs_type                = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type         *config_file_data_ptr    = NULL;
  simlock_category_file_data_type   category_file_data;

  if(req_msg_ptr == NULL || resp_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(&category_file_data, 0x00,
            sizeof(simlock_category_file_data_type));

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* update the policies */
  resp_msg_ptr->slot_policy = config_file_data_ptr->slot_policy;
  resp_msg_ptr->subscription_policy = config_file_data_ptr->sub_policy;

  /* Update the emergency only mode from EFS */
  if (config_file_data_ptr->device_mode == SIMLOCK_DEVICE_EMERGENCY_ONLY_MODE)
  {
    resp_msg_ptr->emergency_only_mode = TRUE;
  }
  else
  {
    resp_msg_ptr->emergency_only_mode = FALSE;
  }

  /* Update the simlock config file storage location */
  if (config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE)
  {
    resp_msg_ptr->remote_simlock_storage = TRUE;
  }
  else
  {
    resp_msg_ptr->remote_simlock_storage = FALSE;
  }

  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM;
      slot++)
  {
    simlock_status = simlock_util_get_slot_index(slot, &slot_index);
    if((simlock_status == SIMLOCK_SUCCESS) &&
       (slot_index < SIMLOCK_SLOT_COUNT_MAX))
    {
      for(category = SIMLOCK_CATEGORY_3GPP_NW; category <= SIMLOCK_CATEGORY_3GPP2_RUIM;
          category++)
      {
        simlock_status = simlock_util_get_category_index(category, &category_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (category_index < SIMLOCK_CATEGORY_COUNT_MAX))
        {
          memset(&category_file_data, 0x00,
                 sizeof(simlock_category_file_data_type));

          resp_msg_ptr->lock_info[slot_index].category_info[category_index].category = category;

          /* check if a category is disabled. If yes, continue */
          if(!simlock_util_check_category_enabled(config_file_data_ptr, category, slot) &&
             !simlock_util_check_category_disabled_file_present(config_file_data_ptr, category, slot))
          {
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_status = FALSE;
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_ok_to_reactivate = FALSE;
            continue;
          }

          /* in this case, codes are present in one file */
          if(config_file_data_ptr->slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS)
          {
            locked_slot = SIMLOCK_SLOT_1;
          }
          else
          {
            locked_slot = slot;
          }

          sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                           category,
                                                           locked_slot);
          /* read the lock data */
          simlock_status = simlock_category_read_lock_data(category, locked_slot,
                                                           &category_file_data,
                                                           sfs_type,
                                                           SIMLOCK_CONFIG_ID_DEFAULT);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            SIMLOCK_MSG_ERR_2("read lock data failed, category: 0x%x,"
                              " slot: 0x%x", category, locked_slot);

            /* this frees the code data in the category */
            simlock_category_free_code_data(&category_file_data);
            simlock_config_free_config_data(config_file_data_ptr);
            return simlock_status;
          }

          if(simlock_util_check_category_enabled(config_file_data_ptr, category, slot))
          {
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_status           = TRUE;
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_ok_to_reactivate = FALSE;
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].ck_present                =
              config_file_data_ptr->category_status[slot_index][category_index].ck_present;
          }
          else
          {
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_status           = FALSE;
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].category_ok_to_reactivate = TRUE;
          }

          if(category_file_data.category_header.code_type == SIMLOCK_CODE_BLACKLIST)
          {
            resp_msg_ptr->lock_info[slot_index].category_info[category_index].blacklist = TRUE;
          }
          resp_msg_ptr->lock_info[slot_index].category_info[category_index].num_retries_max =
            category_file_data.category_header.num_retries_max;
          resp_msg_ptr->lock_info[slot_index].category_info[category_index].curr_retries =
            category_file_data.category_header.curr_retries;

          simlock_category_free_code_data(&category_file_data);
        }
      } /* end loop categories */

      resp_msg_ptr->disabled_slots = config_file_data_ptr->disabled_slots;
    }
  } /* end loop for slots */

  /* get the temporary unlock remaining time */
  (void)simlock_temp_unlock_get_remaining_time(&resp_msg_ptr->remaining_time);

  simlock_config_free_config_data(config_file_data_ptr);

  /* Read inactive configurations only when it is requested.
     This is to ensure to avoid unnecessary reads for clients who are NOT
     interested in inactive configurations */
  if(req_msg_ptr->inactive_config &&
     SIMLOCK_CONFIG_FILE_ABSENT == simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT))
  {
    simlock_status = simlock_get_inactive_config(&resp_msg_ptr->inactive_config_data);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("simlock_get_inactive_config returns ERROR");
    }
  }

  return simlock_status;
} /* simlock_get_lock_status */


/*===========================================================================
FUNCTION SIMLOCK_GET_LOCK_CATEGORY_DATA

DESCRIPTION
  This function retrieves the code data of the requested category on
  a slot

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_get_lock_category_data
(
  const simlock_get_category_data_msg_req_type * req_msg_ptr,
  simlock_get_category_data_msg_resp_type      * resp_msg_ptr
)
{
  simlock_result_enum_type        simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type          slot_id              = SIMLOCK_SLOT_1;
  simlock_slot_policy_enum_type   slot_policy          = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;
  simlock_sfs_enum_type           sfs_type             = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type       *config_file_data_ptr = NULL;

  if((req_msg_ptr == NULL) ||
     (resp_msg_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(simlock_util_check_slot(req_msg_ptr->slot) != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_get_category_data for category: 0x%x",
                    req_msg_ptr->category);

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(
                                     req_msg_ptr->category);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* check the slot policy */
  slot_policy = config_file_data_ptr->slot_policy;

  /* if the slot policy is same on all the available slots, then
     it is expected to have one category file per each enabled category
     i.e. all the codes for that category are in one file */
  if(slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS)
  {
    slot_id = SIMLOCK_SLOT_1;
  }
  else
  {
    slot_id = req_msg_ptr->slot;
  }

  /* check if the category is enabled. If not error */
  if(!simlock_util_check_category_enabled(config_file_data_ptr,
                                          req_msg_ptr->category,
                                          slot_id))
  {
    SIMLOCK_MSG_ERR_1("category 0x%x not enabled",
                      req_msg_ptr->category);
    simlock_status = SIMLOCK_INVALID_STATE;
  }
  else
  {
    sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                     req_msg_ptr->category,
                                                     slot_id);

    simlock_status = simlock_category_get_code_data(slot_id, req_msg_ptr->category,
                                                    resp_msg_ptr,
                                                    sfs_type);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("get code data failed");
    }
  }

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_get_lock_category_data */


/*===========================================================================
FUNCTION SIMLOCK_GET_SIMLOCK_RSU_MODE

DESCRIPTION
  This function retrieves the current enabled RSU mode

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_get_simlock_rsu_mode
(
  simlock_get_rsu_mode_msg_resp_type      * resp_msg_ptr
)
{
#ifdef FEATURE_SIMLOCK_RSU
  simlock_result_enum_type        simlock_status       = SIMLOCK_SUCCESS;
  simlock_config_data_type       *config_file_data_ptr = NULL;

  if(resp_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }
  
  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }
  
  /* update the RSU mode */
  resp_msg_ptr->rsu_mode = config_file_data_ptr->rsu_mode;
  SIMLOCK_MSG_HIGH_1("simlock_get_simlock_rsu_mode: 0x%x", resp_msg_ptr->rsu_mode);

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
#else
  (void)resp_msg_ptr;
  return SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU */
} /* simlock_get_simlock_rsu_mode */

/*===========================================================================
FUNCTION SIMLOCK_SET_LOCK_HCK

DESCRIPTION
  This function sets and enables the SIMLock for a category. It takes the ck
  and the iteration count as the inputs and generates HCK needed for the
  lock. It then writes the lock data to a category file. Also, it updates
  the config file indicating that the lock is enabled for the category,
  upon successfully updating the lock data.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_lock_hck
(
  const simlock_set_lock_hck_msg_req_type  * req_msg_ptr,
  simlock_token_msg_resp_type              * resp_msg_ptr
)
{
  simlock_result_enum_type               simlock_status       = SIMLOCK_SUCCESS;
  simlock_slot_enum_type                 slot_id              = SIMLOCK_SLOT_1;
  uint8                                  slot_index           = 0;
  uint8                                  category_index       = 0;
  simlock_category_file_data_type      * category_data_ptr    = NULL;
  simlock_set_lock_ck_msg_req_type     * lock_ck_msg_ptr      = NULL;
  simlock_config_data_type             * config_file_data_ptr = NULL;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_set_lock_hck for category: 0x%x",
                    req_msg_ptr->category.category_type);

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(
                          req_msg_ptr->category.category_type);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* if the auto_lock is enabled, then the lock codes are set from the
     SIM card. So in this case, codes cannot be blacklist */
  if((req_msg_ptr->auto_lock) &&
     (req_msg_ptr->blacklist))
  {
    SIMLOCK_MSG_ERR_1("both autolock and blacklist are enabled, for category, 0x%x",
                      req_msg_ptr->category.category_type);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* if the auto_lock is enabled, then the lock codes are set from the
     SIM card. So in this case, codes cannot be reused */
  if((req_msg_ptr->auto_lock) &&
     (req_msg_ptr->reuse_previous_code_list))
  {
    SIMLOCK_MSG_ERR_1("both autolock and reuse code are enabled, for category, 0x%x",
                      req_msg_ptr->category.category_type);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Return error if auto-lock is enabled for SP EHPLMN category */
  if (req_msg_ptr->auto_lock &&
      req_msg_ptr->category.category_type == SIMLOCK_CATEGORY_3GPP_SP_EHPLMN)
  {
    SIMLOCK_MSG_ERR_0("Auto-lock is not supported for SP EHPLMN category");
    return SIMLOCK_INVALID_LOCK_DATA;
  }

  /* Return if config id is present in request and we already have default config file*/
  if(req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT &&
     (SIMLOCK_CONFIG_FILE_ABSENT != simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT) ||
       simlock_max_inactive_config_created()))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(req_msg_ptr->config_id);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    /* Reject lock request if config id in non-zero and SFS type is remote */
    if(config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE &&
       req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT)
    {
      SIMLOCK_MSG_ERR_0("Remote SFS is not supported for inactive config");
      simlock_status = SIMLOCK_UNSUPPORTED;
      break;
    }

    /* if the slot policy is same on all the available slots, then
       it is expected to have one category file per each enabled category
       i.e. all the codes for that category are in one file */
    if(config_file_data_ptr->slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS)
    {
      slot_id = SIMLOCK_SLOT_1;
    }
    else
    {
      slot_id = req_msg_ptr->slot;
    }

    simlock_status = simlock_util_get_slot_index(slot_id, &slot_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_status = simlock_util_get_category_index(req_msg_ptr->category.category_type,
                                                     &category_index);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (category_index >= SIMLOCK_CATEGORY_COUNT_MAX))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* check if the category is already enabled */
    if(simlock_util_check_category_enabled(config_file_data_ptr,
                                           req_msg_ptr->category.category_type,
                                           slot_id))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x already enabled",
                        req_msg_ptr->category.category_type);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* Codes need to be validated in both of the following cases:
       - autolock is not enabled
       - autolock is enabled with a non-zero list of codes (in this case, the
         code data is the whitelist of codes such that once the simcard is
         inserted, if the simcard data matches any of the entry in the whitelist,
         the device is autolocked to that simcard */
    if(req_msg_ptr->reuse_previous_code_list == FALSE)
    {
      /* check if the code data chars are valid. If auto_lock is TRUE, code data
         may be empty - that is not an error condition */
      simlock_status = simlock_util_check_code_data(&req_msg_ptr->category,
                                                    req_msg_ptr->auto_lock);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("no valid code data for the category, 0x%x",
                           req_msg_ptr->category.category_type);
        break;
      }

      /* check if the codes are conflicting */
      simlock_status = simlock_category_verify_codes_for_conflicts(&req_msg_ptr->category,
                                                                   config_file_data_ptr,
                                                                   slot_id,
                                                                   req_msg_ptr->config_id);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("conflicting code data for the category, 0x%x",
                          req_msg_ptr->category.category_type);
        break;
      }
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(lock_ck_msg_ptr,
                                 sizeof(simlock_set_lock_ck_msg_req_type));
    if(lock_ck_msg_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    lock_ck_msg_ptr->slot = slot_id;
    simlock_memscpy(&lock_ck_msg_ptr->category,
                    sizeof(simlock_category_data_type),
                    &req_msg_ptr->category,
                    sizeof(simlock_category_data_type));
    lock_ck_msg_ptr->auto_lock = req_msg_ptr->auto_lock;
    lock_ck_msg_ptr->blacklist = req_msg_ptr->blacklist;
    lock_ck_msg_ptr->num_retries_max = req_msg_ptr->num_retries_max;
    lock_ck_msg_ptr->iteration_cnt = req_msg_ptr->iteration_cnt;
  
    if(!req_msg_ptr->reuse_previous_code_list)
    {
      simlock_status = simlock_category_build_lock_data(req_msg_ptr->auto_lock,
                                                        req_msg_ptr->blacklist,
                                                        req_msg_ptr->num_retries_max,
                                                        req_msg_ptr->iteration_cnt,
                                                        &req_msg_ptr->category,
                                                        req_msg_ptr->salt,
                                                        req_msg_ptr->hck,
                                                        category_data_ptr);
    }
    else
    {
      /* Read the lock code header and data */
      simlock_status = simlock_category_read_lock_data(req_msg_ptr->category.category_type,
                                                       slot_id,
                                                       category_data_ptr,
                                                       req_msg_ptr->sfs_type,
                                                       SIMLOCK_CONFIG_ID_DEFAULT);
      if(simlock_status == SIMLOCK_SUCCESS)
      {
        simlock_status = simlock_category_build_header_data(req_msg_ptr->category.category_type,
                                                            req_msg_ptr->auto_lock,
                                                            req_msg_ptr->blacklist,
                                                            req_msg_ptr->num_retries_max,
                                                            req_msg_ptr->iteration_cnt,
                                                            req_msg_ptr->salt,
                                                            req_msg_ptr->hck,
                                                            category_data_ptr);
      }
    }
  
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("build lock data failed, status: 0x%x", simlock_status);
      break;
    }

    /* Update header with token config with config_nickname */
    if(req_msg_ptr->config_id && resp_msg_ptr != NULL)
    {
      simlock_status = simlock_crypto_get_random_data(
                         category_data_ptr->category_header.token.token_data,
                         sizeof(category_data_ptr->category_header.token.token_data));

      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("Token creation filed, status: 0x%x", simlock_status);
        break;
      }

      category_data_ptr->category_header.token.token_present = TRUE;

      resp_msg_ptr->token.token_present = TRUE;
      simlock_memscpy(resp_msg_ptr->token.token_data, sizeof(resp_msg_ptr->token.token_data),
                      category_data_ptr->category_header.token.token_data, SIMLOCK_TOKEN_LEN);

      config_file_data_ptr->nickname_len = req_msg_ptr->config_nickname.nickname_len;
      simlock_memscpy(config_file_data_ptr->nickname, sizeof(config_file_data_ptr->nickname),
                      req_msg_ptr->config_nickname.nickname, req_msg_ptr->config_nickname.nickname_len);
    }

    /* write the category data */
    simlock_status = simlock_category_write_lock_data(slot_id,
                                                      category_data_ptr,
                                                      req_msg_ptr->sfs_type,
                                                      req_msg_ptr->config_id);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      /* enable the category in config file */
      simlock_status = simlock_config_update_and_write_category_status(
                                         category_index,
                                         slot_index,
                                         SIMLOCK_ENABLED,
                                         SIMLOCK_CK_PRESENT,
                                         config_file_data_ptr,
                                         req_msg_ptr->sfs_type,
                                         req_msg_ptr->config_id);
      if(simlock_status == SIMLOCK_SUCCESS && 
         req_msg_ptr->config_id == SIMLOCK_CONFIG_ID_DEFAULT)
      {
        /* SimLock configuration has been successfully written. Blow the SimLock
           fuse for enhanced security. The function below blows the fuse only
           if it has not been blown yet. So, its ok to call it everytime the set
           lock hck API is called */
        if(SIMLOCK_SUCCESS != simlock_fuse_blow_fuse())
        {
          ASSERT(0);
        }
      }
      else
      {
        SIMLOCK_MSG_ERR_1("update category status failed, status: 0x%x",
                          simlock_status);
      }
    }
  }while(0);

  /* this frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  SIMLOCK_MEM_FREE(lock_ck_msg_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_set_lock_hck */


/*===========================================================================
FUNCTION SIMLOCK_TEMPORARY_UNLOCK_REQUEST

DESCRIPTION
  This function sets the temporary unlock for a category or unsets a
  previous temporary unlock.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_temporary_unlock_request
(
  const simlock_temporary_unlock_msg_req_type  * req_msg_ptr
)
{
  simlock_result_enum_type   simlock_status       = SIMLOCK_SUCCESS;
  simlock_config_data_type  *config_file_data_ptr = NULL;
  boolean                    category_enabled     = FALSE;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_2("simlock_temporary_unlock_request, req_type: 0x%x, category: 0x%x",
                    req_msg_ptr->temporary_unlock_type,req_msg_ptr->category);

  /* check if slot is correct */
  if(simlock_util_check_slot(req_msg_ptr->slot) != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(req_msg_ptr->category);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("category 0x%x not supported", req_msg_ptr->category);
    return simlock_status;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    SIMLOCK_MSG_ERR_0("read config file failed");
    return SIMLOCK_GENERIC_ERROR;
  }

  /* check if the category is enabled. */
  category_enabled = simlock_util_check_category_enabled(config_file_data_ptr,
                                                         req_msg_ptr->category,
                                                         req_msg_ptr->slot);

  simlock_config_free_config_data(config_file_data_ptr);
  
  if(!category_enabled)
  {
    SIMLOCK_MSG_ERR_1("category 0x%x not enabled",
                      req_msg_ptr->category);
    return SIMLOCK_INVALID_STATE;
  }

  switch(req_msg_ptr->temporary_unlock_type)
  {
    case SIMLOCK_TEMPORARY_UNLOCK_SET:
      return simlock_timer_start(req_msg_ptr->slot,
                                 req_msg_ptr->category,
                                 req_msg_ptr->duration);

    case SIMLOCK_TEMPORARY_UNLOCK_UNSET:
      return simlock_timer_stop(req_msg_ptr->slot,
                                req_msg_ptr->category);

    default:
      break;
  }

  return SIMLOCK_GENERIC_ERROR;
} /* simlock_temporary_unlock_request */


/*===========================================================================
FUNCTION SIMLOCK_SET_DEVICE_MODE_REQUEST

DESCRIPTION
  This function sets the device mode to emergency only or normal mode

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_device_mode_request
(
  const simlock_set_device_mode_msg_req_type  * req_msg_ptr
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_SUCCESS;
  simlock_config_data_type   *config_file_data_ptr = NULL;

  if(req_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_set_device_mode_request, req: 0x%x",
                    req_msg_ptr->mode);

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    SIMLOCK_MSG_ERR_0("read config file failed");
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    if(config_file_data_ptr->device_mode == req_msg_ptr->mode)
    {
      SIMLOCK_MSG_ERR_1("device is already in mode, 0x%x",
                         req_msg_ptr->mode);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    simlock_status = simlock_config_update_and_write_device_mode(req_msg_ptr->mode,
                                                                 config_file_data_ptr);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      /* SimLock configuration has been successfully updated. Blow the SimLock
         fuse for enhanced security. The function below blows the fuse only
         if it has not been blown yet. So, its ok to call it everytime the
         set device mode API is called */
      if(SIMLOCK_SUCCESS != simlock_fuse_blow_fuse())
      {
        ASSERT(0);
      }
    }
  }while(0);

  simlock_config_free_config_data(config_file_data_ptr);

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* this sets the device in the requested mode by calling the
     CM API. This function performs a sync wait until the response
     is received or the timer expires */
  return simlock_set_device_mode_sync(req_msg_ptr->mode);
} /* simlock_set_device_mode_request */


/*===========================================================================
FUNCTION SIMLOCK_ADD_LOCK_CODES_CK_REQUEST

DESCRIPTION
  This function processes the request to add lock codes to a category, which
  is already enabled. It reads the lock data of the requested category and
  checks if the CK matches. If it matches, the new lock codes are added to
  the existing codes and lock data is written to the category file.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_add_lock_codes_ck_request
(
  const simlock_add_lock_codes_ck_msg_req_type  * req_msg_ptr,
  simlock_add_lock_device_msg_resp_type         * resp_msg_ptr
)
{
  simlock_result_enum_type               simlock_status    = SIMLOCK_SUCCESS;
  simlock_category_file_data_type      * category_data_ptr = NULL;
  simlock_ck_present_type                ck_status         = SIMLOCK_CK_NOT_PRESENT;
  simlock_slot_enum_type                 slot_id           = SIMLOCK_SLOT_1;
  simlock_sfs_enum_type                  sfs_type          = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type             * config_file_data_ptr = NULL;
  simlock_hck_type                       hck_data_buf;

  if(req_msg_ptr == NULL || resp_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_add_lock_codes_ck_request for category: 0x%x",
                    req_msg_ptr->category.category_type);

  memset(hck_data_buf, 0x00, SIMLOCK_HCK_LEN);

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(
                                   req_msg_ptr->category.category_type);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* Return if config id is present in request and we already have default config file*/
  if(req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT &&
     (SIMLOCK_CONFIG_FILE_ABSENT != simlock_config_check_config_file(SIMLOCK_CONFIG_ID_DEFAULT) ||
      simlock_max_inactive_config_created()))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(req_msg_ptr->config_id);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    /* Reject add lock request if config id in non-zero and SFS type is remote */
    if(config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE &&
       req_msg_ptr->config_id != SIMLOCK_CONFIG_ID_DEFAULT)
    {
      SIMLOCK_MSG_ERR_0("Remote SFS is not supported for inactive config");
      simlock_status = SIMLOCK_UNSUPPORTED;
      break;
    }

    /* if the slot policy is same on all the available slots, then
       it is expected to have one category file per each enabled category
       i.e. all the codes for that category are in one file */
    if(config_file_data_ptr->slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS)
    {
      slot_id = SIMLOCK_SLOT_1;
    }
    else
    {
      slot_id = req_msg_ptr->slot;
    }

    /* check if the category is enabled */
    if(!simlock_util_check_category_enabled(config_file_data_ptr,
                                            req_msg_ptr->category.category_type, slot_id))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x not enabled", req_msg_ptr->category.category_type);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                     req_msg_ptr->category.category_type,
                                                     slot_id);

    simlock_status = simlock_util_check_ck_status(config_file_data_ptr,
                                                  req_msg_ptr->category.category_type,
                                                  slot_id,
                                                  &ck_status);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    /* if the category is locked without CK, do not
       let additional codes with CK */
    if((ck_status == SIMLOCK_CK_NOT_PRESENT) &&
       (req_msg_ptr->ck.simlock_ck_length > 0))
    {
      SIMLOCK_MSG_ERR_1("category was securely locked without CK: 0x%x",
                        req_msg_ptr->category.category_type);
      simlock_status = SIMLOCK_INCORRECT_KEY;
      break;
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    /* read the lock code header and data */
    simlock_status = simlock_category_read_lock_data(req_msg_ptr->category.category_type,
                                                     slot_id,
                                                     category_data_ptr,
                                                     sfs_type,
                                                     req_msg_ptr->config_id);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    if(category_data_ptr->category_header.auto_lock &&
       req_msg_ptr->blacklist)
    {
      SIMLOCK_MSG_ERR_0("For autolocking, the lock codes cannot be blacklist codes");
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* if the number of retries is equal to maximum
       retries allowed then return error */
    if(category_data_ptr->category_header.num_retries_max != 0)
    {
      if(category_data_ptr->category_header.curr_retries >=
           category_data_ptr->category_header.num_retries_max)
      {
        SIMLOCK_MSG_ERR_0("number of retries expired");
        resp_msg_ptr->num_retries_max =
           category_data_ptr->category_header.num_retries_max;
        resp_msg_ptr->curr_retries =
           category_data_ptr->category_header.curr_retries;

        /* this frees the code data in the category */
        simlock_category_free_code_data(category_data_ptr);
        SIMLOCK_MEM_FREE(category_data_ptr);
        return SIMLOCK_MAX_RETRIES_REACHED;
    }
  }


    if(req_msg_ptr->ck.simlock_ck_length > 0)
    {
      if(category_data_ptr->category_header.iteration_cnt == 0)
      {
        SIMLOCK_MSG_ERR_0("invalid lock data");
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }

      /* generate the HCK by running the PBKDF2 algo */
      simlock_status = simlock_crypto_run_pbkdf2_algorithm((const uint8 *)req_msg_ptr->ck.simlock_ck_data,
                                                           (uint16)req_msg_ptr->ck.simlock_ck_length,
                                                           category_data_ptr->category_header.salt,
                                                           SIMLOCK_SALT_LEN,
                                                           category_data_ptr->category_header.iteration_cnt,
                                                           hck_data_buf,
                                                           SIMLOCK_HCK_LEN);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("hck generation failed, status: 0x%x", simlock_status);
        break;
      }
    }

    /* compare the HCK generated with that in the category file */
    if((ck_status == SIMLOCK_CK_NOT_PRESENT) ||
       (memcmp(hck_data_buf, category_data_ptr->category_header.hck,
              SIMLOCK_HCK_LEN) == 0))
    {
      simlock_status = simlock_category_append_lock_data(&req_msg_ptr->category,
                                                         req_msg_ptr->blacklist,
                                                         category_data_ptr);
      if(simlock_status == SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_MED_0("key data matched, adding new codes...");

        /* write the category data */
        simlock_status = simlock_category_write_lock_data(slot_id,
                                                          category_data_ptr,
                                                          sfs_type,
                                                          req_msg_ptr->config_id);
        resp_msg_ptr->token.token_present = category_data_ptr->category_header.token.token_present;

        simlock_memscpy(resp_msg_ptr->token.token_data, sizeof(resp_msg_ptr->token.token_data),
                        category_data_ptr->category_header.token.token_data, SIMLOCK_TOKEN_LEN);
      }
    }
    else
    {
      simlock_status = simlock_update_retries_after_failure_and_write(
                         category_data_ptr,
                         slot_id,
                         sfs_type,
                         &resp_msg_ptr->curr_retries,
                         &resp_msg_ptr->num_retries_max,
                         req_msg_ptr->config_id);
    }
  }while(0);

  /* this frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_add_lock_codes_ck_request */


/*===========================================================================
FUNCTION SIMLOCK_ADD_LOCK_CODES_HCK_REQUEST

DESCRIPTION
  This function processes the request to add lock codes to a category, which
  is already enabled. It reads the lock data of the requested category and
  checks if the HCK matches. If it matches, the new lock codes are added to
  the existing codes and lock data is written to the category file.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_add_lock_codes_hck_request
(
  const simlock_add_lock_codes_hck_msg_req_type  * req_msg_ptr,
  simlock_add_lock_device_msg_resp_type          * resp_msg_ptr
)
{
  simlock_result_enum_type               simlock_status       = SIMLOCK_SUCCESS;
  simlock_category_file_data_type      * category_data_ptr    = NULL;
  simlock_slot_enum_type                 slot_id              = SIMLOCK_SLOT_1;
  simlock_ck_present_type                ck_status            = SIMLOCK_CK_NOT_PRESENT;
  simlock_sfs_enum_type                  sfs_type             = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type             * config_file_data_ptr = NULL;

  if(req_msg_ptr == NULL || resp_msg_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_add_lock_codes_hck_request for category: 0x%x",
                    req_msg_ptr->category.category_type);

  /* check if category is supported */
  simlock_status = simlock_util_check_category_supported(
                                   req_msg_ptr->category.category_type);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  do
  {
    /* if the slot policy is same on all the available slots, then
       it is expected to have one category file per each enabled category
       i.e. all the codes for that category are in one file */
    if(config_file_data_ptr->slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS)
    {
      slot_id = SIMLOCK_SLOT_1;
    }
    else
    {
      slot_id = req_msg_ptr->slot;
    }

    /* check if the category is enabled */
    if(!simlock_util_check_category_enabled(config_file_data_ptr,
                                            req_msg_ptr->category.category_type, slot_id))
    {
      SIMLOCK_MSG_ERR_1("category 0x%x not enabled", req_msg_ptr->category.category_type);
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    sfs_type = simlock_config_read_category_sfs_type(config_file_data_ptr,
                                                     req_msg_ptr->category.category_type,
                                                     slot_id);

    /* if the category is locked without CK, do not
       let additional codes with CK */
    simlock_status = simlock_util_check_ck_status(config_file_data_ptr,
                                                  req_msg_ptr->category.category_type,
                                                  slot_id,
                                                  &ck_status);
    if(simlock_status != SIMLOCK_SUCCESS || ck_status == SIMLOCK_CK_NOT_PRESENT)
    {
      if (simlock_status == SIMLOCK_SUCCESS)
      {
        simlock_status = SIMLOCK_INCORRECT_KEY;
      }
      break;
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr,
                                 sizeof(simlock_category_file_data_type));
    if(category_data_ptr == NULL)
    {
      simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

    /* read the lock code header and data */
    simlock_status = simlock_category_read_lock_data(req_msg_ptr->category.category_type,
                                                     slot_id,
                                                     category_data_ptr,
                                                     sfs_type,
                                                     SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    if(category_data_ptr->category_header.auto_lock &&
       req_msg_ptr->blacklist)
    {
      SIMLOCK_MSG_ERR_0("For autolocking, the lock codes cannot be blacklist codes");
      simlock_status = SIMLOCK_INVALID_STATE;
      break;
    }

    /* if the number of retries is equal to maximum
     retries allowed then return error */
    if(category_data_ptr->category_header.num_retries_max != 0)
    {
      if(category_data_ptr->category_header.curr_retries >=
         category_data_ptr->category_header.num_retries_max)
      {
        SIMLOCK_MSG_ERR_0("number of retries expired");
        resp_msg_ptr->num_retries_max =
           category_data_ptr->category_header.num_retries_max;
        resp_msg_ptr->curr_retries =
           category_data_ptr->category_header.curr_retries;

        /* this frees the code data in the category */
        simlock_category_free_code_data(category_data_ptr);
        SIMLOCK_MEM_FREE(category_data_ptr);
        return SIMLOCK_MAX_RETRIES_REACHED;
      }
    }

    /* compare the HCK in the request with that in the category file */
    if(memcmp(req_msg_ptr->hck, category_data_ptr->category_header.hck,
              SIMLOCK_HCK_LEN) == 0)
    {
      simlock_status = simlock_category_append_lock_data(&req_msg_ptr->category,
                                                         req_msg_ptr->blacklist,
                                                         category_data_ptr);
      if(simlock_status == SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_MED_0("hash key data matched, adding new codes...");

        /* write the category data */
        simlock_status = simlock_category_write_lock_data(slot_id,
                                                          category_data_ptr,
                                                          sfs_type,
                                                          SIMLOCK_CONFIG_ID_DEFAULT);
      }
    }
    else
    {
      simlock_status = simlock_update_retries_after_failure_and_write(
                         category_data_ptr,
                         slot_id,
                         sfs_type,
                         &resp_msg_ptr->curr_retries,
                         &resp_msg_ptr->num_retries_max,
                         SIMLOCK_CONFIG_ID_DEFAULT);
    }
  }while(0);

  /* this frees the code data in the category */
  simlock_category_free_code_data(category_data_ptr);
  SIMLOCK_MEM_FREE(category_data_ptr);
  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_add_lock_codes_hck_request */

#endif /* FEATURE_SIMLOCK */

