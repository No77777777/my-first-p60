/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   P O L I C Y  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock policy functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2020, 2024 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_policy.c#5 $$ $DateTime: 2024/04/22 06:22:47 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/18/24   ghk     Set all subs to valid instead of sub with index 0 only
01/29/20   cj      Optimization of FR47581 changes
11/13/19   vdc     Added support for new simlock policy
09/10/19   vdc     SimLock solution for activation at Point Of Sale
07/13/17   nr      Remote SIM Unlock supported in common build
06/09/17   bcho    Read GID1/GID2 from card if required in SIMLOCK algo
05/16/17   bcho    Use slot1 data for applying sub policy for few slot policies
04/16/17   bcho    Set perso status properly if sub is locked due to other app
03/19/17   bcho    Only apply sub policy if category is enabled
02/23/17   sk      Enhanced logging in apply sub and config policy fcns
01/26/17   bcho    Support for slot policy valid card must on all slots
01/20/17   vdc     Store only 1 IMSI of the sub that has acquired full srv
01/05/17   vdc     Limit policy wait for full srv or imsi matched to 3GPP
12/20/16   vdc     Derive correct IMSI for comparing IMSI when sub is OOS
12/20/16   vdc     Support for simlock policy wait for full srv or imsi matched
11/09/16   bcho    Check 3GPP2 subscription validity for 3GPP_AND_3GPP2 policy
11/03/16   bcho    Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_policy.h"
#include "simlock_util.h"
#include "simlock_modem.h"

/*===========================================================================

                   ENUMERATED DATA DECLARATIONS

===========================================================================*/

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_CARD_INVALID_REASON_TYPE

   DESCRIPTION:
     This enum specifies the reason for considering a card as invalid
-----------------------------------------------------------------------------*/
typedef enum
{
  SIMLOCK_CARD_INVALID_REASON_NONE,
  SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION,
  SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION,
} simlock_card_invalid_reason_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_IS_CARD_VALID_EXT

DESCRIPTION
  This function checks if a card on a slot is valid.If card is not present on
  slot, it will be considered as valid. All subscriptions on card
  must be valid for the card to be considered valid.
  If USIM/SIM is present, but not ready (for example, PIN1 is enabled and not
  verified, or USIM/SIM is not activated), SIMLOCK will consider this as invalid
  subscription if any 3GPP SIMLOCK category is enabled on that slot.
  If CSIM/RUIM is present, but not ready (for example, PIN1 is enabled and not
  verified, or CSIM/RUIM is not activated), SIMLOCK will consider this as invalid
  subscription if any 3GPP2 SIMLOCK category is enabled on that slot.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE   : If the card is valid
  FALSE  : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
static boolean simlock_is_card_valid_ext
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr,
  simlock_slot_enum_type                       slot,
  const simlock_config_data_type             * config_data_ptr,
  simlock_card_invalid_reason_type           * invalid_reason_ptr
)
{
  uint8                          num_sub                = 0;
  uint8                          slot_index             = 0;
  simlock_category_details_type  category_3gpp_details  = {FALSE};
  simlock_category_details_type  category_3gpp2_details = {FALSE};

  if((req_msg_ptr == NULL) ||
     (result_msg_ptr == NULL) ||
     (invalid_reason_ptr == NULL) ||
     (config_data_ptr == NULL))
  {
    return FALSE;
  }

  if((simlock_util_get_slot_index(slot, &slot_index) !=
        SIMLOCK_SUCCESS) ||
     (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
  {
    return FALSE;
  }

  /* Cards with invalid number of subscriptions should not be considered valid */
  if(req_msg_ptr->card_data[slot_index].num_subscriptions > SIMLOCK_SUBS_COUNT_MAX)
  {
    *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION;
    return FALSE;
  }

  /* If card is not present, slot is valid */
  if(req_msg_ptr->card_data[slot_index].is_card_present == FALSE)
  {
    return TRUE;
  }

  /* Check if slot is disabled in config or not. If yes, card is not valid. */
  if(config_data_ptr->disabled_slots.slot[slot_index])
  {
    *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION;
    return FALSE;
  }

  /* check if 3gpp and 3gpp2 categories are disabled. If yes, no need to check
     whether subscription data is present in request when respective app is
     available on card. */
  (void)simlock_util_all_categories_disabled(SIMLOCK_SLOT_1,
                                             config_data_ptr,
                                             &category_3gpp_details,
                                             &category_3gpp2_details,
                                             FALSE);

  /* check if subscription data is present in request when 3gpp simlock
  category is enabled and USIM/SIM app is availabe on card. */
  if (req_msg_ptr->card_data[slot_index].is_gw_app_available &&
      category_3gpp_details.status)
  {
    boolean subscription_data_present = FALSE;

    for(num_sub = 0; num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions; num_sub++)
    {
      if(req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type == SIMLOCK_SUBSCRIPTION_3GPP)
      {
        subscription_data_present = TRUE;
        break;
      }
    }

    /* Subscription data not present means either app is stuck at PIN1 or not activated at all */
    if(subscription_data_present == FALSE)
    {
      *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION;
      return FALSE;
    }
  }

  /* check if subscription data is present in request when 3gpp2 simlock
  category is enabled and CSIM/RUIM app is availabe on card. */
  if (req_msg_ptr->card_data[slot_index].is_1x_app_available &&
      category_3gpp2_details.status)
  {
    boolean subscription_data_present = FALSE;

    for(num_sub = 0; num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions; num_sub++)
    {
      if(req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type == SIMLOCK_SUBSCRIPTION_3GPP2)
      {
        subscription_data_present = TRUE;
        break;
      }
    }

    /* Subscription data not present means either app is stuck at PIN1 or not activated at all */
    if(subscription_data_present == FALSE)
    {
      *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION;
      return FALSE;
    }
  }

  /* All subscriptions on card must be valid for the card to be considered valid */
  for(num_sub = 0; num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions; num_sub++)
  {
    if(!result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid)
    {
      *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION;
      return FALSE;
    }
  }

  return TRUE;
} /* simlock_is_card_valid_ext */


/*===========================================================================
FUNCTION SIMLOCK_IS_CARD_VALID

DESCRIPTION
  This function checks if a card on a slot is valid. All subscriptions on card
  must be valid for the card to be considered valid

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE   : If the card is valid
  FALSE  : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
static boolean simlock_is_card_valid
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr,
  simlock_slot_enum_type                       slot,
  simlock_card_invalid_reason_type           * invalid_reason_ptr
)
{
  uint8       num_sub     = 0;
  uint8       slot_index  = 0;

  if((req_msg_ptr == NULL) ||
     (result_msg_ptr == NULL) ||
     (invalid_reason_ptr == NULL))
  {
    return FALSE;
  }

  if((simlock_util_get_slot_index(slot, &slot_index) !=
        SIMLOCK_SUCCESS) ||
     (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
  {
    return FALSE;
  }

  /* Cards with no valid subscription should not be considered valid */
  if(req_msg_ptr->card_data[slot_index].num_subscriptions == 0 ||
     req_msg_ptr->card_data[slot_index].num_subscriptions > SIMLOCK_SUBS_COUNT_MAX)
  {
    *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION;
    return FALSE;
  }

  /* All subscriptions on card must be valid for the card to be considered valid */
  for(num_sub = 0; num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions; num_sub++)
  {
    if(!result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid)
    {
      *invalid_reason_ptr = SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION;
      return FALSE;
    }
  }

  return TRUE;
} /* simlock_is_card_valid */


/*===========================================================================
FUNCTION SIMLOCK_APPLY_VALID_3GPP_OR_3GPP2_POLICY

DESCRIPTION
  This function updates the validity of subscriptions on a card, based on
 validity of either 3GPP or 3GPP2 subscription.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_apply_valid_3gpp_or_3gpp2_policy
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr,
  simlock_slot_enum_type                       slot
)
{
  uint8                                       num_sub              = 0;
  uint8                                       slot_index           = 0;
  boolean                                     is_card_valid        = FALSE;

  if((req_msg_ptr == NULL) ||
     (result_msg_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if((simlock_util_get_slot_index(slot, &slot_index) !=
        SIMLOCK_SUCCESS) ||
     (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* If any 3GPP or 3GPP2 subscription is valid, means card is valid */
  for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                    num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
  {
    if(req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
         SIMLOCK_SUBSCRIPTION_3GPP ||
       req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
         SIMLOCK_SUBSCRIPTION_3GPP2)
    {
      if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid)
      {
        is_card_valid = TRUE;
        break;
      }
    }
  }

  /* If no valid subscription was found, then nothing to do. */
  if(!is_card_valid)
  {
    return SIMLOCK_SUCCESS;
  }

  /* Update the validity of the subscriptions */
  for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                    num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
  {
    result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
      TRUE;
  }

  return SIMLOCK_SUCCESS;
} /* simlock_apply_valid_3gpp_or_3gpp2_policy */


/*===========================================================================
FUNCTION SIMLOCK_APPLY_SUB_POLICY

DESCRIPTION
  This function updates the validity of a subscription on a card, based on
  the subscription policy set in the SIMLock feature.

  If the sub policy is set to one of the following, then the card is
  considered as valid only if the respective subscription(s) is/are present
  and valid on the card.
  1. Valid 3GPP required
  2. Valid 3GPP2 required
  3. Valid 3GPP and 3GPP2 required
  4. Valid 3GPP or 3GPP2 required

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_apply_sub_policy
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr,
  simlock_subscription_policy_enum_type        sub_policy,
  simlock_slot_enum_type                       slot,
  simlock_config_data_type                   * config_data_ptr
)
{
  uint8                                       num_sub                = 0;
  uint8                                       slot_index             = 0;
  boolean                                     is_card_valid          = TRUE;
  boolean                                     is_3gpp_app_active     = FALSE;
  boolean                                     is_3gpp2_app_active    = FALSE;
  simlock_subscription_invalid_reason_type    failure_reason         = SIMLOCK_SUBSCRIPTION_INVALID_REASON_CONFIG_POLICY;
  simlock_category_enum_type                  failed_category        = SIMLOCK_CATEGORY_3GPP_NW;
  uint32                                      retries_max            = 0;
  uint32                                      retries_current        = 0;
  simlock_category_details_type               category_3gpp_details  = {FALSE};
  simlock_category_details_type               category_3gpp2_details = {FALSE};
  simlock_slot_enum_type                      policy_slot             = SIMLOCK_SLOT_1;

  if (sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_INDEPENDENT)
  {
    return SIMLOCK_SUCCESS;
  }

  if((req_msg_ptr == NULL)     ||
     (config_data_ptr == NULL) ||
     (result_msg_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if((simlock_util_get_slot_index(slot, &slot_index) !=
        SIMLOCK_SUCCESS) ||
     (slot_index >= SIMLOCK_SLOT_COUNT_MAX))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  if(!simlock_policy_uses_slot1_config_for_all_slots(config_data_ptr->slot_policy))
  {
    policy_slot = slot;
  }

  /* Don't apply sub policy if all categories are disabled */
  if(simlock_util_all_categories_disabled(policy_slot,
                                          config_data_ptr,
                                          &category_3gpp_details,
                                          &category_3gpp2_details,
                                          FALSE))
  {
    return SIMLOCK_SUCCESS;
  }

  /* This is the only policy that makes valid an invalid session, so we need
     special handling. All other policies restrict valid sessions and are
     handled below */
  if(sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_OR_3GPP2_REQUIRED)
  {
    return simlock_apply_valid_3gpp_or_3gpp2_policy(req_msg_ptr, result_msg_ptr, slot);
  }

  /* apply the subscription policy */
  for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                    num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
  {
    if((sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_REQUIRED) ||
       (sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_AND_3GPP2_REQUIRED))
    {
      if(req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
           SIMLOCK_SUBSCRIPTION_3GPP)
      {
        is_3gpp_app_active = TRUE;
        if(!result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid)
        {
          failure_reason  = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_LOCKED;
          failed_category = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failed_category;
          retries_max     = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].num_retries_max;
          retries_current = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].curr_retries;
          is_card_valid   = FALSE;
          break;
        }
      }
    }

    if ((sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP2_REQUIRED) ||
        (sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_AND_3GPP2_REQUIRED))
    {
      if(req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
           SIMLOCK_SUBSCRIPTION_3GPP2)
      {
        is_3gpp2_app_active = TRUE;
        if(!result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid)
        {
          failure_reason  = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_LOCKED;
          failed_category = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failed_category;
          retries_max     = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].num_retries_max;
          retries_current = result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].curr_retries;
          is_card_valid   = FALSE;
          break;
        }
      }
    }
  }

  if(is_card_valid)
  {
    switch(sub_policy)
    {
      case SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_REQUIRED:
        /* if the 3gpp app is present but not activated,
           mark card as not valid */
        if(req_msg_ptr->card_data[slot_index].is_gw_app_available && !is_3gpp_app_active)
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_ACTIVE;
          is_card_valid = FALSE;
        }
        /* if the 3gpp app is not present on card,
           mark card as not valid */
        else if(!req_msg_ptr->card_data[slot_index].is_gw_app_available)
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_PRESENT;
          is_card_valid = FALSE;
        }

        /* In both scenarios, either dependency app is present but not active or
           dependency app is not present on card, we need to populate failed
           category, current retries and max retries */
        if(is_card_valid)
        {
          break;
        }

        if(category_3gpp_details.status)
        {
          failed_category = category_3gpp_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        else
        {
          failed_category = category_3gpp2_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp2_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        break;

      case SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP2_REQUIRED:
        /* if the 3gpp2 app is present but not activated,
           mark card as not valid */
        if(req_msg_ptr->card_data[slot_index].is_1x_app_available && !is_3gpp2_app_active)
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_ACTIVE;
          is_card_valid = FALSE;
        }
        /* if the 3gpp2 app is not present on card,
           mark card as not valid */
        else if(!req_msg_ptr->card_data[slot_index].is_1x_app_available)
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_PRESENT;
          is_card_valid = FALSE;
        }

        if(is_card_valid)
        {
          break;
        }

        /* In both scenarios, either dependency app is present but not active or
           dependency app is not present on card, we need to populate failed
           category, current retries and max retries */
        if(category_3gpp2_details.status)
        {
          failed_category = category_3gpp2_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp2_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        else
        {
          failed_category = category_3gpp_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        break;

      case SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_AND_3GPP2_REQUIRED:
        /* if the 3GPP/3gpp2 app is present but not activated,
           mark card as not valid */
        if((req_msg_ptr->card_data[slot_index].is_gw_app_available && !is_3gpp_app_active) ||
           (req_msg_ptr->card_data[slot_index].is_1x_app_available && !is_3gpp2_app_active))
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_ACTIVE;
          is_card_valid = FALSE;
        }
        /* if the 3gpp/3gpp2 app is not present on card,
           mark card as not valid */
        else if(!req_msg_ptr->card_data[slot_index].is_gw_app_available ||
                !req_msg_ptr->card_data[slot_index].is_1x_app_available)
        {
          failure_reason = SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_REQUIRED_BUT_NOT_PRESENT;
          is_card_valid = FALSE;
        }

        if(is_card_valid)
        {
          break;
        }

        /* In both scenarios, either dependency app is present but not active or
           dependency app is not present on card, we need to populate failed
           category, current retries and max retries */
        if(category_3gpp_details.status)
        {
          failed_category = category_3gpp_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        else
        {
          failed_category = category_3gpp2_details.category;

          (void)simlock_util_get_retry_count(config_data_ptr,
                                             category_3gpp2_details.category,
                                             policy_slot,
                                             &retries_current,
                                             &retries_max);
        }
        break;

      default:
         break;
    }
  }

  /* if the card is valid, it means the policy check
     is OK. So need to update the card validity status */
  if(is_card_valid)
  {
    return SIMLOCK_SUCCESS;
  }

  /* Update the validity of the subscriptions on a slot based on the
     subscription policy */
  for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                    num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
  {
    if(((sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_REQUIRED) &&
        (req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
            SIMLOCK_SUBSCRIPTION_3GPP2))
                            ||
       ((sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP2_REQUIRED) &&
        (req_msg_ptr->card_data[slot_index].subscription_data[num_sub].sub_type ==
            SIMLOCK_SUBSCRIPTION_3GPP))
                            ||
       (sub_policy == SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_AND_3GPP2_REQUIRED))
    {
      result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
          FALSE;
      if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
           SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
      {
        result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
          failure_reason;
        result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failed_category =
          failed_category;
        result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].num_retries_max =
          retries_max;
        result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].curr_retries =
          retries_current;
      }
    }
  }

  return SIMLOCK_SUCCESS;
} /* simlock_apply_sub_policy */


/*===========================================================================
FUNCTION SIMLOCK_APPLY_CONFIG_POLICY

DESCRIPTION
  This function updates the validity of a subscription on a card, based on
  the slot and the subscription policy set in the SIMLock feature.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_apply_config_policy
(
  const simlock_run_algorithm_msg_req_type   * req_msg_ptr,
  simlock_config_data_type                   * config_data_ptr,
  simlock_run_algorithm_msg_resp_type        * result_msg_ptr
)
{
  simlock_result_enum_type               simlock_status = SIMLOCK_SUCCESS;
  simlock_slot_policy_enum_type          slot_policy    = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;
  simlock_subscription_policy_enum_type  sub_policy     = SIMLOCK_SUBSCRIPTION_POLICY_INDEPENDENT;
  simlock_slot_enum_type                 slot           = SIMLOCK_SLOT_1;
  simlock_card_invalid_reason_type       invalid_reason = SIMLOCK_CARD_INVALID_REASON_NONE;
  uint8                                  slot_index     = 0;
  uint8                                  sub_index      = 0;
  uint8                                  num_sub        = 0;
  boolean                                is_card_valid  = TRUE;

  if((req_msg_ptr == NULL)     ||
     (config_data_ptr == NULL) ||
     (result_msg_ptr == NULL))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  slot_policy = config_data_ptr->slot_policy;
  sub_policy  = config_data_ptr->sub_policy;

  SIMLOCK_MSG_MED_2("simlock_apply_config_policy: slot_policy=0x%x, sub_policy=0x%x",
                    slot_policy, sub_policy);

  /* first apply the sub policy to subcriptions on all supported slots */
  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    if(simlock_apply_sub_policy(req_msg_ptr,
                                result_msg_ptr,
                                sub_policy,
                                slot,
                                config_data_ptr) !=
         SIMLOCK_SUCCESS)
    {
      return SIMLOCK_GENERIC_ERROR;
    }
  }

  /* now apply the slot policy */
  switch(slot_policy)
  {
    case SIMLOCK_SLOT_POLICY_LOCK_ONLY_SLOT_1:
      for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              TRUE;
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_SLOT_1:
      /* Unless all the categories on slot-1 are disabled,
         check if there is a valid card on slot-1 */
      if(simlock_util_all_categories_disabled(SIMLOCK_SLOT_1,
                                              config_data_ptr,
                                              NULL,
                                              NULL,
                                              FALSE))
      {
        is_card_valid = TRUE;
      }
      else
      {
        is_card_valid = simlock_is_card_valid(req_msg_ptr, result_msg_ptr, SIMLOCK_SLOT_1, &invalid_reason);
      }

      for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;
            if(!is_card_valid)
            {
              if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
              {
                if(invalid_reason == SIMLOCK_CARD_INVALID_REASON_NO_SUBSCRIPTION)
                {
                  result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                      SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_ON_SLOT1_REQUIRED_BUT_NOT_PRESENT;
                }
                else if(invalid_reason == SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION)
                {
                  result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                      SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_ON_SLOT1_REQUIRED_BUT_LOCKED;
                }
              }
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ANY_SLOT:
      /* check if there is a valid card on any slot */
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        is_card_valid = simlock_is_card_valid(req_msg_ptr, result_msg_ptr, slot, &invalid_reason);
        if(is_card_valid)
        {
          break;
        }
      }

      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;
            if(!is_card_valid)
            {
              if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
              {
                if(invalid_reason == SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION)
                {
                  result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                      SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_ON_ANY_SLOT_REQUIRED_BUT_LOCKED;
                }
              }
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_MASTER_SLOT_1:
      if(simlock_util_all_categories_disabled(SIMLOCK_SLOT_1,
                                              config_data_ptr,
                                              NULL,
                                              NULL,
                                              FALSE))
      {
        is_card_valid = TRUE;
      }
      else
      {
        is_card_valid = simlock_is_card_valid(req_msg_ptr, result_msg_ptr, SIMLOCK_SLOT_1, &invalid_reason);
      }
      if(is_card_valid)
      {
        for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
        {
          simlock_status = simlock_util_get_slot_index(slot, &slot_index);
          if((simlock_status == SIMLOCK_SUCCESS) &&
             (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
             (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
          {
            for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                              num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
            {
              result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
                TRUE;
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_BLOCK_ALL_SLOTS_IF_SLOT_1_EXPIRED:
      is_card_valid = TRUE;
      for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                         sub_index < req_msg_ptr->card_data[0].num_subscriptions; sub_index++)
      {
        if(req_msg_ptr->card_data[0].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_FULL)
        {
          is_card_valid = TRUE;
          break;
        }
        else if(req_msg_ptr->card_data[0].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_EXPIRED)
        {
          is_card_valid = FALSE;
        }
      }

      for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;
            if(!is_card_valid)
            {
              if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
              {
                result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_SUB_ON_SLOT1_EXPIRED;
              }
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_SLOT_1:
      is_card_valid = FALSE;
      for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                         sub_index < req_msg_ptr->card_data[0].num_subscriptions; sub_index++)
      {
        if(req_msg_ptr->card_data[0].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_FULL)
        {
          is_card_valid = TRUE;
          break;
        }
      }

      for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;
            if(!is_card_valid)
            {
              if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
              {
                result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_WAIT_FOR_FULL_SRV_ON_SLOT1;
              }
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_BLOCK_ALL_SLOTS_IF_ALL_VALID_SLOTS_EXPIRED:
      is_card_valid = FALSE;
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        if(simlock_is_card_valid(req_msg_ptr, result_msg_ptr, slot, &invalid_reason) &&
           simlock_util_get_slot_index(slot, &slot_index) == SIMLOCK_SUCCESS &&
           slot_index < SIMLOCK_SLOT_COUNT_MAX)
        {
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            if(req_msg_ptr->card_data[slot_index].subscription_data[sub_index].service_status != SIMLOCK_SRV_STATUS_EXPIRED)
            {
              is_card_valid = TRUE;
              break;
            }
          }

          if(is_card_valid)
          {
            break;
          }
        }
      }

      /* If all valid slots are expired then there is nothing to update
         for other subscriptions */
      if(!is_card_valid)
      {
        break;
      }

      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_ANY_VALID_SLOT:
      is_card_valid   = FALSE;
      /* Check if there is a valid card on any slot */
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        if(simlock_is_card_valid(req_msg_ptr, result_msg_ptr, slot, &invalid_reason) &&
           simlock_util_get_slot_index(slot, &slot_index) == SIMLOCK_SUCCESS         &&
           slot_index < SIMLOCK_SLOT_COUNT_MAX)
        {
          /* Check if there a subscription which has full service that satisfies enabled categories */
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            if(req_msg_ptr->card_data[slot_index].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_FULL)
            {
              is_card_valid = TRUE;
              break;
            }
          }

          if(is_card_valid)
          {
            break;
          }
        }
      }

      /* Not able to find any subscription with full service. So, nothing to
         update for other subscriptions, but allow only the cards which are
         valid to possibly get full service on it */
      if(!is_card_valid)
      {
        break;
      }

      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[sub_index].is_subscription_valid =
              is_card_valid;
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_ANY_VALID_SLOT_OR_IMSI_MATCH:
      is_card_valid   = FALSE;
      /* Check if there is a valid card on any slot */
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        if(simlock_is_card_valid(req_msg_ptr, result_msg_ptr, slot, &invalid_reason) &&
           simlock_util_get_slot_index(slot, &slot_index) == SIMLOCK_SUCCESS         &&
           slot_index < SIMLOCK_SLOT_COUNT_MAX)
        {
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            /* Mark the subscriptions as valid if there is a 3GPP subscription
               which has full service that satisfies enabled categories
               (store the IMSI into SFS) */
            if(req_msg_ptr->card_data[slot_index].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_FULL)
            {
              if(req_msg_ptr->card_data[slot_index].subscription_data[sub_index].sub_type == SIMLOCK_SUBSCRIPTION_3GPP)
              {
                (void)simlock_file_write(
                        (uint8 *)req_msg_ptr->card_data[slot_index].subscription_data[sub_index].imsi.imsi,
                        req_msg_ptr->card_data[slot_index].subscription_data[sub_index].imsi.imsi_len,
                        SIMLOCK_FILE_IMSI_DATA,
                        SIMLOCK_SFS_LOCAL,
                        SIMLOCK_CONFIG_ID_DEFAULT);
              }

              is_card_valid = TRUE;
              break;
            }
          }

          if(is_card_valid)
          {
            break;
          }
        }
      }

      /* When full service is not available then match for IMSI */
      if(!is_card_valid)
      {
        uint32   imsi_len = 0;
        uint8   *imsi_ptr = NULL;

        for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
        {
          if(simlock_is_card_valid(req_msg_ptr, result_msg_ptr, slot, &invalid_reason) &&
             simlock_util_get_slot_index(slot, &slot_index) == SIMLOCK_SUCCESS         &&
             slot_index < SIMLOCK_SLOT_COUNT_MAX)
          {
            for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                               sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
            {
              if(simlock_file_read(&imsi_ptr, &imsi_len, SIMLOCK_FILE_IMSI_DATA,
                                   SIMLOCK_SFS_LOCAL, SIMLOCK_CONFIG_ID_DEFAULT) == SIMLOCK_SUCCESS &&
                 imsi_len != 0 &&
                 imsi_ptr != NULL &&
                 req_msg_ptr->card_data[slot_index].subscription_data[sub_index].imsi.imsi_len == imsi_len &&
                 memcmp(req_msg_ptr->card_data[slot_index].subscription_data[sub_index].imsi.imsi,
                        imsi_ptr,
                        imsi_len) == 0)
              {
                is_card_valid = TRUE;
              }

              SIMLOCK_MEM_FREE(imsi_ptr);
              if(is_card_valid)
              {
                break;
              }
            }

            if(is_card_valid)
            {
              break;
            }
          }
        }
      }

       /* Not able to find any subscription with full service or no IMSI has
          matched. So, nothing to update for other subscriptions, but allow
          only the cards which are valid to possibly get full service on it */
      if(!is_card_valid)
      {
        break;
      }

      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[sub_index].is_subscription_valid =
              is_card_valid;
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ALL_SLOTS:
      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        if(FALSE == simlock_is_card_valid_ext(req_msg_ptr, result_msg_ptr,
                                              slot, config_data_ptr,
                                              &invalid_reason))
        {
          is_card_valid = FALSE;
          break;
        }
      }

      if(is_card_valid)
      {
        break;
      }

      for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX))
        {
          for(num_sub = 0; (num_sub < req_msg_ptr->card_data[slot_index].num_subscriptions &&
                            num_sub < SIMLOCK_SUBS_COUNT_MAX); num_sub++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].is_subscription_valid =
              is_card_valid;

            if(result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason ==
                  SIMLOCK_SUBSCRIPTION_INVALID_REASON_NONE)
            {
              if(invalid_reason == SIMLOCK_CARD_INVALID_REASON_INVALID_SUBSCRIPTION)
              {
                result_msg_ptr->card_validity_data[slot_index].subscription_data[num_sub].failure_reason =
                    SIMLOCK_SUBSCRIPTION_INVALID_REASON_APP_ON_ANY_SLOT_REQUIRED_BUT_LOCKED;
              }
            }
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_SLOT1_OR_IMSI_MATCH:
       is_card_valid = FALSE;

      /* Check if there is a valid card on slot1 */
      for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                         sub_index < req_msg_ptr->card_data[0].num_subscriptions; sub_index++)
      {
        /* Mark the subscriptions as valid if there is a 3GPP subscription
           which has full service that satisfies enabled categories
           (store the IMSI into SFS) */
        if(req_msg_ptr->card_data[0].subscription_data[sub_index].sub_type == SIMLOCK_SUBSCRIPTION_3GPP &&
           req_msg_ptr->card_data[0].subscription_data[sub_index].service_status == SIMLOCK_SRV_STATUS_FULL)
        {
          (void)simlock_file_write(
                  (uint8 *)req_msg_ptr->card_data[0].subscription_data[sub_index].imsi.imsi,
                  req_msg_ptr->card_data[0].subscription_data[sub_index].imsi.imsi_len,
                  SIMLOCK_FILE_IMSI_DATA,
                  SIMLOCK_SFS_LOCAL,
                  SIMLOCK_CONFIG_ID_DEFAULT);

          is_card_valid = TRUE;
          break;
        }
      }

      /* When full service is not available then match for IMSI */
      if(!is_card_valid)
      {
        uint32   imsi_len = 0;
        uint8   *imsi_ptr = NULL;

        for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                           sub_index < req_msg_ptr->card_data[0].num_subscriptions; sub_index++)
        {
          if(simlock_file_read(&imsi_ptr, &imsi_len, SIMLOCK_FILE_IMSI_DATA,
                               SIMLOCK_SFS_LOCAL, SIMLOCK_CONFIG_ID_DEFAULT) == SIMLOCK_SUCCESS &&
             imsi_len != 0 &&
             imsi_ptr != NULL &&
             req_msg_ptr->card_data[0].subscription_data[sub_index].imsi.imsi_len == imsi_len &&
             memcmp(req_msg_ptr->card_data[0].subscription_data[sub_index].imsi.imsi,
                    imsi_ptr,
                    imsi_len) == 0)
          {
            is_card_valid = TRUE;
          }

          SIMLOCK_MEM_FREE(imsi_ptr);
          if(is_card_valid)
          {
            break;
          }
        }
      }

      /* Not able to find any subscription on slot1 with full service or no
         IMSI has matched. So, nothing to update for other subscriptions */
      if(!is_card_valid)
      {
        break;
      }

      /* Update card1's card validity status to other subscriptions present in other slots */
      for(slot = SIMLOCK_SLOT_2; slot < SIMLOCK_SLOT_NUM; slot++)
      {
        simlock_status = simlock_util_get_slot_index(slot, &slot_index);
        if((simlock_status == SIMLOCK_SUCCESS) &&
           (slot_index < SIMLOCK_SLOT_COUNT_MAX) &&
           (config_data_ptr->disabled_slots.slot[slot_index] == FALSE))
        {
          for(sub_index = 0; sub_index < SIMLOCK_SUBS_COUNT_MAX &&
                             sub_index < req_msg_ptr->card_data[slot_index].num_subscriptions; sub_index++)
          {
            result_msg_ptr->card_validity_data[slot_index].subscription_data[sub_index].is_subscription_valid =
              is_card_valid;
          }
        }
      }
      break;

    case SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS:
    case SIMLOCK_SLOT_POLICY_UNIQUE_FOR_EACH_SLOT:
      /* Nothing to do */
      break;

    default:
      return SIMLOCK_GENERIC_ERROR;
  }

  return simlock_status;
} /* simlock_apply_config_policy */


/*===========================================================================
FUNCTION SIMLOCK_POLICY_USES_SLOT1_CONFIG_FOR_ALL_SLOTS

DESCRIPTION
  This function checks whether slot policy uses slot1 configuration also for
  all additional slots.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE   : If policy uses slot1 config
  FALSE  : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_policy_uses_slot1_config_for_all_slots
(
  simlock_slot_policy_enum_type slot_policy
)
{
  if(slot_policy == SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS ||
     slot_policy == SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ANY_SLOT ||
     slot_policy == SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_ANY_VALID_SLOT ||
     slot_policy == SIMLOCK_SLOT_POLICY_BLOCK_ALL_SLOTS_IF_ALL_VALID_SLOTS_EXPIRED ||
     slot_policy == SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_ANY_VALID_SLOT_OR_IMSI_MATCH ||
     slot_policy == SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_ALL_SLOTS ||
     slot_policy == SIMLOCK_SLOT_POLICY_WAIT_FOR_FULL_SERVICE_ON_SLOT1_OR_IMSI_MATCH)
  {
    return TRUE;
  }
  return FALSE;
}/* simlock_policy_uses_slot1_config_for_all_slots */

#endif /* FEATURE_SIMLOCK */

