/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M   L O C K   R S U   C O N F I G   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock RSU configuration functions.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/20   cj      Remote SIM Unlock support for simlock rsu mode E & F
12/27/19   cj      Make device dual SIM to single SIM via RSU BLOB
12/27/19   sg      Added support for crypto API in MOB
12/06/19   tq      Added re-lock support for RSU MODE C
09/10/19   vdc     SimLock solution for activation at Point Of Sale
04/26/19   mm      Allowing ADD HCK CODE and SET HCK LOCK for mode B
04/25/19   mm      Provide msg response ptr for all lock hck request
12/03/18   vgd     Added support for public keys for multiple RSU modes
12/03/18   vgd     Return Blob too short error when signature is not provided
12/03/18   vgd     Increment offset by max GID length in blob
12/03/18   vgd     Return too short length error when blob length is short.
12/03/18   vgd     Validate input lock data of RSU before applying lock
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
11/02/17   av      Fix return value for set_configuration request
07/24/17   nr      Reduce stack consumption
07/13/17   nr      Remote SIM Unlock supported in common build
07/13/17   av      Enhanced simlock RSU solution
04/17/17   sk      Reject certain Simlock DIAG commands when RSU flag is defined
12/01/16   vdc     Extend blob to support lock/unlock with HCK
11/03/16   bcho    Added support for ICCID simlock
10/06/16   ar      Added support for SP + EHPLMN simlock
08/31/16   vdc     Use correct IMEI in OTA unlock and RSU blob validation
08/03/16   vdc     Added support for SPN based SIMLOCK feature
08/02/16   vdc     Fix compiler errors for FEATURE_SIMLOCK_QC_REMOTE_LOCK
01/20/16   stv     Fix to validate the RSA signature in the blob request
01/14/16   stv     Move variables to heap to reduce stack usage
01/07/16   stv     Remote simlock support
08/19/15   stv     Restrict native simlock locking when rsu is enabled
08/18/15   stv     Clear temp unlock data when temp unlock expires
08/12/15   stv     To send temporary unlock status indication
07/29/15   stv     Allow the rsu set configuration with/without protection in test environment
06/30/15   stv     Clear temp unlock data when a permanent unlock/reset simlock req
06/10/15   stv     Featurize integrity checks for rsu set configuration cmd
05/17/15   stv     Fix compiler warnings
05/15/15   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock.h"
#include "simlock_modem.h"
#include "simlock_crypto.h"
#include "simlock_rsu_config.h"
#include "simlock_temp_unlock.h"
#include "simlock_time.h"
#include "simlock_util.h"
#include "simlocklib.h"
#include "simlock_rsu_key.h"
#include "simlock_time_nitz.h"
#include "simlock_cm.h"
#include "simlock_time_gps.h"
#include "simlock_sfs.h"
#include "simlock_rsu.h"
#include "simlock_delay.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#else
#include "simlock_sfs.h"
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#define SIMLOCK_REMOTE_LOCK_TIME_FILE_VERSION                               1
#define SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_LEN_VERSION_1                    44
#define SIMLOCK_REMOTE_LOCK_RESPONSE_RSU_MODE_D_BLOB_LEN_VERSION_1         60
#define SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_MAJOR_VERSION_1                   1
#define SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_MINOR_VERSION_1                   0

 /* 2 bytes for versions + 1 byte for lock-operation + 1 byte for error-code +
    8 bytes for timestamp + 32 bytes for HMAC = 44 bytes */

/* Number of codes (NW + NS + SP) */
#define SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_1                     3

/* Number of codes (NW + NS + SP + SP-EHPLMN) */
#define SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_RSU_MODE_D_VERSION_1          4

/* (Chg flag(1) + Iter cnt(4) + Salt(16) + HCK(32) + Code data len(1)) * 3(NW + NS + SP) */
#define SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_2_OR_3              162

/* (Chg flag(1) + Iter cnt(4) + Salt(16) + HCK(32) + Code data len(1)) * 4(NW + NS + SP + IMPI) */
#define SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_4_OR_5              216

/* Chg flag(1) + Iter cnt(4) + Salt(16) + HCK(32) */
#define SIMLOCK_REMOTE_LOCK_LOCK_DATA_LEN_VERSION_2_OR_3                   53

#define SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH                                1

#define SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE                               6
#define SIMLOCK_REMOTE_LOCK_NS_CODE_DATA_SIZE                               8
#define SIMLOCK_REMOTE_LOCK_SP_CODE_DATA_SIZE                               7
#define SIMLOCK_REMOTE_LOCK_SP_EHPLMN_CODE_DATA_SIZE                       21

#define SIMLOCK_REMOTE_LOCK_CODE_DATA_CHANGE                                1
#define SIMLOCK_REMOTE_LOCK_CODE_DATA_NO_CHANGE                             0

/* Index of the position in the request RSU blob that contains the RSU
   operation */
#define SIMLOCK_LOCK_OPERATION_INDEX_IN_REQUEST_BLOB                        3

#define SIMLOCK_REMOTE_LOCK_LOCK_CODE_AND_SIGNATURE_SIZE                  257

#define SIMLOCK_RSU_CONFIG_HIGH_NIBBLE                                     0xF0
#define SIMLOCK_RSU_CONFIG_LOW_NIBBLE                                      0x0F

#define SIMLOCK_RSU_CONFIG_HIGH_BYTE                                       0xFF00
#define SIMLOCK_RSU_CONFIG_LOW_BYTE                                        0x00FF

#define SIMLOCK_RSU_CONFIG_GID_SIZE                                        2
#define SIMLOCK_RSU_CONFIG_RFU_SIZE                                        2
/* Max number of digits for Mode E and F */
#define SIMLOCK_MODE_E_F_IMEI_MAX_LEN                                      16

/* ----------------------------------------------------------------------------
   DATA TYPE:      simlock_rsu_config_non_ascii_mcc_mnc_type
------------------------------------------------------------------------------*/
typedef uint32 simlock_rsu_config_non_ascii_mcc_mnc_type;

/* ----------------------------------------------------------------------------
   DATA TYPE:      simlock_rsu_config_non_ascii_gid_type
------------------------------------------------------------------------------*/
typedef uint16 simlock_rsu_config_non_ascii_gid_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIGNON_NON_ASCII_MCC_MNC_LIST_TYPE

   DESCRIPTION:
     This structure contains the list of MCC/MNCs

     num_mcc_mnc_codes   : Number of MCC/MNCs in the list
     mcc_mnc_code_ptr    : Pointer to MCC/MNC data
-------------------------------------------------------------------------------*/
typedef struct
{
  uint16                                        num_mcc_mnc_codes;
  simlock_rsu_config_non_ascii_mcc_mnc_type   * mcc_mnc_code_ptr;
}simlock_rsu_config_non_ascii_mcc_mnc_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_NON_ASCII_GID_LIST_TYPE

   DESCRIPTION:
     This structure contains the list of GIDs

     num_gid_codes       : Number of GIDs in the list
     gid_code_ptr        : Pointer to GID data
-------------------------------------------------------------------------------*/
typedef struct
{
  uint16                                        num_gid_codes;
  simlock_rsu_config_non_ascii_gid_type       * gid_code_ptr;
}simlock_rsu_config_non_ascii_gid_list_type;

const simlock_rsu_config_non_ascii_mcc_mnc_type simlock_rsu_always_present_codes[] = {0x31016000, 0x31020000, 0x31021000, 0x31022000,
                                                                                      0x31023000, 0x31024000, 0x31025000, 0x31026000,
                                                                                      0x31027000, 0x31030000, 0x31031000, 0x31066000,
                                                                                      0x31080000, 0x00101000};

static const simlock_mcc_type simlock_rsu_test_mcc = {'0', '0', '1'};

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_FREE_CODE_DATA

DESCRIPTION
  This function frees the code data in the config data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
static void simlock_rsu_config_free_code_data
(
  simlock_rsu_config_data_type    * config_data_ptr
)
{
  if(config_data_ptr == NULL)
  {
    return;
  }

  if(config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr != NULL)
  {
    SIMLOCK_MEM_FREE(config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr);
  }
  if(config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr != NULL)
  {
    SIMLOCK_MEM_FREE(config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr);
  }
  if(config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr != NULL)
  {
    SIMLOCK_MEM_FREE(config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr);
  }
  if(config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr != NULL)
  {
    SIMLOCK_MEM_FREE(config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr);
  }
  if(config_data_ptr->auth_data.msg_data_ptr != NULL)
  {
    SIMLOCK_MEM_FREE(config_data_ptr->auth_data.msg_data_ptr);
  }

  SIMLOCK_MEM_FREE(config_data_ptr);
} /* simlock_rsu_config_free_code_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_GET_LOCK_STATUS

DESCRIPTION
  This function gets the status of the locks (enabled or disabled)

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_get_lock_status
(
  simlock_message_response_data_type  * lock_status_resp_ptr
)
{
  simlock_result_enum_type              simlock_status       = SIMLOCK_SUCCESS;
  simlock_message_request_data_type   * lock_status_req_ptr  = NULL;

  if(lock_status_resp_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(lock_status_req_ptr,
                               sizeof(simlock_message_request_data_type));
  if (lock_status_req_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  lock_status_req_ptr->msg_type  = SIMLOCK_GET_STATUS_MSG;

  /* call the engine with get status request */
  simlock_status = simlock_process_message(lock_status_req_ptr, lock_status_resp_ptr);

  SIMLOCK_MEM_FREE(lock_status_req_ptr);

  return simlock_status;
} /* simlock_rsu_config_get_lock_status */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_SEND_UNLOCK_SECURED

DESCRIPTION
  This function sends the unlock request to the simlock engine. This is
  a secured unlock request and so there is no CK

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_send_unlock_secured
(
  simlock_slot_enum_type        slot,
  simlock_category_enum_type    category
)
{
  simlock_result_enum_type              simlock_status  = SIMLOCK_SUCCESS;
  simlock_message_request_data_type   * unlock_req_ptr  = NULL;
  simlock_message_response_data_type  * unlock_resp_ptr = NULL;

  SIMLOCK_CHECK_AND_MEM_MALLOC(unlock_req_ptr,
                               sizeof(simlock_message_request_data_type));
  if (unlock_req_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(unlock_resp_ptr,
                               sizeof(simlock_message_response_data_type));
  if (unlock_resp_ptr == NULL)
  {
    SIMLOCK_MEM_FREE(unlock_req_ptr);
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  unlock_req_ptr->msg_type                                = SIMLOCK_UNLOCK_DEVICE_MSG;
  unlock_req_ptr->message.unlock_msg.slot                 = slot;
  unlock_req_ptr->message.unlock_msg.category             = category;
  unlock_req_ptr->message.unlock_msg.ck.simlock_ck_length = 0;
  unlock_req_ptr->message.unlock_msg.ignore_ck            = TRUE;

  memset(unlock_req_ptr->message.unlock_msg.ck.simlock_ck_data, 0x00,
         sizeof(unlock_req_ptr->message.unlock_msg.ck.simlock_ck_data));

  /* call the engine with unlock request */
  simlock_status = simlock_process_message(unlock_req_ptr, unlock_resp_ptr);

  SIMLOCK_MEM_FREE(unlock_req_ptr);
  SIMLOCK_MEM_FREE(unlock_resp_ptr);

  return simlock_status;
} /* simlock_rsu_config_send_unlock_secured */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PROCESS_PERMANENT_UNLOCK

DESCRIPTION
  This function processes the permanent unlock of the provided category

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_process_permanent_unlock
(
  simlock_category_enum_type category
)
{
  simlock_result_enum_type              simlock_status      = SIMLOCK_SUCCESS;
  uint8                                 category_index      = 0;
  boolean                               category_status     = FALSE;
  uint32                                duration            = 0;
  simlock_message_response_data_type  * get_status_resp_ptr = NULL;

  simlock_status = simlock_util_get_category_index(category, &category_index);
  if(simlock_status != SIMLOCK_SUCCESS ||
     category_index >= SIMLOCK_CATEGORY_COUNT_MAX)
  {
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(get_status_resp_ptr,
                               sizeof(simlock_message_response_data_type));
  if (get_status_resp_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* Get the lock status */
  simlock_status = simlock_rsu_config_get_lock_status(get_status_resp_ptr);

  category_status =
    get_status_resp_ptr->message.get_status_resp.lock_info[0].category_info[category_index].category_status;
  duration =
    get_status_resp_ptr->message.get_status_resp.remaining_time.duration[0][category_index];

  SIMLOCK_MEM_FREE(get_status_resp_ptr);

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* if the category is not ebabled, then no need to unlock.
     Check the status corresponding to slot1 (zero index) only as
     we lock categories on slot1 */
  if(!category_status)
  {
    return SIMLOCK_SUCCESS;
  }

  simlock_status = simlock_rsu_config_send_unlock_secured(SIMLOCK_SLOT_1, category);
  if (simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* Unlock is successful for this category.
     Check if this is temporarily unlocked previously and now successfully unlocked.
     If yes, send temporary event so that this would notify the end indication to clients */
  if(duration > 0)
  {
    simlock_send_temporary_unlock_event(SIMLOCK_SLOT_1, category);
  }

#ifdef FEATURE_SIMLOCK_RSU_TIME
  /* If permanent unlock is success, then all the categories are unlocked.
     Hence, clear the contents of temp unlock request from the sfs */
  simlock_temp_unlock_reset_config(SIMLOCK_TEMP_UNLOCK_RESET_CONFIG_IF_UNLOCKED);
#endif /* FEATURE_SIMLOCK_RSU_TIME */

  return simlock_status;
} /* simlock_rsu_config_process_permanent_unlock */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_SET_SLOT_OPERATION

DESCRIPTION
  This function sets the slot operation data received in the request.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_set_slot_operation
(
  simlock_disabled_slot_data_type  disabled_slots
)
{
  simlock_result_enum_type           simlock_status  = SIMLOCK_SUCCESS;
  simlock_message_request_data_type *req_ptr         = NULL;

  SIMLOCK_CHECK_AND_MEM_MALLOC(req_ptr,
                               sizeof(simlock_message_request_data_type));
  if(req_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  req_ptr->msg_type                                    = SIMLOCK_SET_SIMLOCK_POLICY_MSG;
  req_ptr->message.config_policy_msg.set_operation     = SIMLOCK_SET_SLOT_CONFIG_OPERATION;
  req_ptr->message.config_policy_msg.disabled_slots     = disabled_slots;

  simlock_status = simlock_process_message(req_ptr, NULL);

  SIMLOCK_MEM_FREE(req_ptr);

  return simlock_status;
} /* simlock_rsu_config_set_slot_operation */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_SEND_LOCK

DESCRIPTION
  This function sends the lock request to the engine

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_send_lock
(
  const simlock_rsu_config_lock_code_data_type *lock_code_data_ptr,
  simlock_category_enum_type                    category,
  simlock_slot_enum_type                        slot
)
{
  simlock_result_enum_type           simlock_status  = SIMLOCK_SUCCESS;
  simlock_message_request_data_type *req_ptr         = NULL;
  simlock_message_response_data_type *rsp_ptr        = NULL;
  uint8                              code_cnt        = 0;
  uint8                              curr_code_cnt   = 1;

  if(lock_code_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(req_ptr,
                               sizeof(simlock_message_request_data_type));
  if(req_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  switch(category)
  {
    case SIMLOCK_CATEGORY_3GPP_NW:
      if(lock_code_data_ptr->nw_lock.iteration_cnt == 0x00)
      {
        req_ptr->msg_type                                   = SIMLOCK_SET_LOCK_CK_MSG;
        req_ptr->message.lock_ck_msg.slot                   = slot;
        req_ptr->message.lock_ck_msg.category.category_type = category;

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->nw_lock.nw_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_ck_msg.category.code_data.nw_3gpp_code_list.nw_code_data[code_cnt] =
            lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt];
        }

        req_ptr->message.lock_ck_msg.category.code_data.nw_3gpp_code_list.num_nw_codes = code_cnt;

        /* Call the engine with lock request */
        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->nw_lock.nw_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                             = SIMLOCK_ADD_LOCK_CODES_CK_MSG;
        req_ptr->message.add_lock_codes_ck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_ck_msg.slot                   = slot;

        for(;code_cnt < lock_code_data_ptr->nw_lock.nw_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_ck_msg.category.code_data.nw_3gpp_code_list.nw_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->nw_lock.nw_codes - 1)
          {
            req_ptr->message.add_lock_codes_ck_msg.category.code_data.nw_3gpp_code_list.num_nw_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr);
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_ck_msg.category.code_data.nw_3gpp_code_list,
                   0x00,
                   sizeof(simlock_nw_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      else
      {
        /* Prepare set lock command for lock codes less than or equal to SIMLOCK_NUM_CODES_MAX(20) */
        req_ptr->msg_type                                    = SIMLOCK_SET_LOCK_HCK_MSG;
        req_ptr->message.lock_hck_msg.slot                   = slot;
        req_ptr->message.lock_hck_msg.category.category_type = category;
        req_ptr->message.lock_hck_msg.iteration_cnt          =
          lock_code_data_ptr->nw_lock.iteration_cnt;

        simlock_memscpy(req_ptr->message.lock_hck_msg.hck,
                        sizeof(req_ptr->message.lock_hck_msg.hck),
                        lock_code_data_ptr->nw_lock.hck,
                        sizeof(lock_code_data_ptr->nw_lock.hck));
        simlock_memscpy(req_ptr->message.lock_hck_msg.salt,
                        sizeof(req_ptr->message.lock_hck_msg.salt),
                        lock_code_data_ptr->nw_lock.salt,
                        sizeof(lock_code_data_ptr->nw_lock.salt));

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->nw_lock.nw_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_hck_msg.category.code_data.nw_3gpp_code_list.nw_code_data[code_cnt] =
            lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt];
        }

        req_ptr->message.lock_hck_msg.category.code_data.nw_3gpp_code_list.num_nw_codes = code_cnt;

        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
           break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->nw_lock.nw_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                              = SIMLOCK_ADD_LOCK_CODES_HCK_MSG;
        req_ptr->message.add_lock_codes_hck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_hck_msg.slot                   = slot;

        simlock_memscpy(req_ptr->message.add_lock_codes_hck_msg.hck,
                        sizeof(req_ptr->message.add_lock_codes_hck_msg.hck),
                        lock_code_data_ptr->nw_lock.hck,
                        sizeof(lock_code_data_ptr->nw_lock.hck));

        for(;code_cnt < lock_code_data_ptr->nw_lock.nw_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_hck_msg.category.code_data.nw_3gpp_code_list.nw_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->nw_lock.nw_codes - 1)
          {
            req_ptr->message.add_lock_codes_hck_msg.category.code_data.nw_3gpp_code_list.num_nw_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr)
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_hck_msg.category.code_data.nw_3gpp_code_list,
                   0x00,
                   sizeof(simlock_nw_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_NS:
      if(lock_code_data_ptr->ns_lock.iteration_cnt == 0x00)
      {
        req_ptr->msg_type                                   = SIMLOCK_SET_LOCK_CK_MSG;
        req_ptr->message.lock_ck_msg.slot                   = slot;
        req_ptr->message.lock_ck_msg.category.category_type = category;

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->ns_lock.ns_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_ck_msg.category.code_data.ns_3gpp_code_list.ns_code_data[code_cnt] =
            lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt];
        }

        req_ptr->message.lock_ck_msg.category.code_data.ns_3gpp_code_list.num_ns_codes = code_cnt;

        /* Call the engine with lock request */
        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->ns_lock.ns_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                             = SIMLOCK_ADD_LOCK_CODES_CK_MSG;
        req_ptr->message.add_lock_codes_ck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_ck_msg.slot                   = slot;

        for(;code_cnt < lock_code_data_ptr->ns_lock.ns_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_ck_msg.category.code_data.ns_3gpp_code_list.ns_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->ns_lock.ns_codes - 1)
          {
            req_ptr->message.add_lock_codes_ck_msg.category.code_data.ns_3gpp_code_list.num_ns_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr);
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_ck_msg.category.code_data.ns_3gpp_code_list,
                   0x00,
                   sizeof(simlock_ns_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      else
      {
        /* Prepare set lock command for lock codes less than or equal to SIMLOCK_NUM_CODES_MAX(20) */
        req_ptr->msg_type                                    = SIMLOCK_SET_LOCK_HCK_MSG;
        req_ptr->message.lock_hck_msg.slot                   = slot;
        req_ptr->message.lock_hck_msg.category.category_type = category;
        req_ptr->message.lock_hck_msg.iteration_cnt          =
          lock_code_data_ptr->ns_lock.iteration_cnt;

        simlock_memscpy(req_ptr->message.lock_hck_msg.hck,
                        sizeof(req_ptr->message.lock_hck_msg.hck),
                        lock_code_data_ptr->ns_lock.hck,
                        sizeof(lock_code_data_ptr->ns_lock.hck));
        simlock_memscpy(req_ptr->message.lock_hck_msg.salt,
                        sizeof(req_ptr->message.lock_hck_msg.salt),
                        lock_code_data_ptr->ns_lock.salt,
                        sizeof(lock_code_data_ptr->ns_lock.salt));

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->ns_lock.ns_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_hck_msg.category.code_data.ns_3gpp_code_list.ns_code_data[code_cnt] =
            lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt];
        }

        req_ptr->message.lock_hck_msg.category.code_data.ns_3gpp_code_list.num_ns_codes = code_cnt;

        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->ns_lock.ns_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                              = SIMLOCK_ADD_LOCK_CODES_HCK_MSG;
        req_ptr->message.add_lock_codes_hck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_hck_msg.slot                   = slot;

        simlock_memscpy(req_ptr->message.add_lock_codes_hck_msg.hck,
                        sizeof(req_ptr->message.add_lock_codes_hck_msg.hck),
                        lock_code_data_ptr->ns_lock.hck,
                        sizeof(lock_code_data_ptr->ns_lock.hck));

        for(;code_cnt < lock_code_data_ptr->ns_lock.ns_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_hck_msg.category.code_data.ns_3gpp_code_list.ns_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->ns_lock.ns_codes - 1)
          {
            req_ptr->message.add_lock_codes_hck_msg.category.code_data.ns_3gpp_code_list.num_ns_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr)
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_hck_msg.category.code_data.ns_3gpp_code_list,
                   0x00,
                   sizeof(simlock_ns_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      break;

    case SIMLOCK_CATEGORY_3GPP_SP:
      if(lock_code_data_ptr->sp_lock.iteration_cnt == 0x00)
      {
        req_ptr->msg_type                                   = SIMLOCK_SET_LOCK_CK_MSG;
        req_ptr->message.lock_ck_msg.slot                   = slot;
        req_ptr->message.lock_ck_msg.category.category_type = category;

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->sp_lock.sp_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_ck_msg.category.code_data.sp_3gpp_code_list.sp_code_data[code_cnt] =
            lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt];
        }

        req_ptr->message.lock_ck_msg.category.code_data.sp_3gpp_code_list.num_sp_codes = code_cnt;

        /* Call the engine with lock request */
        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->sp_lock.sp_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                             = SIMLOCK_ADD_LOCK_CODES_CK_MSG;
        req_ptr->message.add_lock_codes_ck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_ck_msg.slot                   = slot;

        for(;code_cnt < lock_code_data_ptr->sp_lock.sp_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_3gpp_code_list.sp_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->sp_lock.sp_codes - 1)
          {
            req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_3gpp_code_list.num_sp_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr);
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_3gpp_code_list,
                   0x00,
                   sizeof(simlock_sp_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      else
      {
        /* Prepare set lock command for lock codes less than or equal to SIMLOCK_NUM_CODES_MAX(20) */
        req_ptr->msg_type                                    = SIMLOCK_SET_LOCK_HCK_MSG;
        req_ptr->message.lock_hck_msg.slot                   = slot;
        req_ptr->message.lock_hck_msg.category.category_type = category;
        req_ptr->message.lock_hck_msg.iteration_cnt          =
          lock_code_data_ptr->sp_lock.iteration_cnt;

        simlock_memscpy(req_ptr->message.lock_hck_msg.hck,
                        sizeof(req_ptr->message.lock_hck_msg.hck),
                        lock_code_data_ptr->sp_lock.hck,
                        sizeof(lock_code_data_ptr->sp_lock.hck));
        simlock_memscpy(req_ptr->message.lock_hck_msg.salt,
                        sizeof(req_ptr->message.lock_hck_msg.salt),
                        lock_code_data_ptr->sp_lock.salt,
                        sizeof(lock_code_data_ptr->sp_lock.salt));

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->sp_lock.sp_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_hck_msg.category.code_data.sp_3gpp_code_list.sp_code_data[code_cnt] =
            lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt];
        }

        req_ptr->message.lock_hck_msg.category.code_data.sp_3gpp_code_list.num_sp_codes = code_cnt;

        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->sp_lock.sp_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                              = SIMLOCK_ADD_LOCK_CODES_HCK_MSG;
        req_ptr->message.add_lock_codes_hck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_hck_msg.slot                   = slot;

        simlock_memscpy(req_ptr->message.add_lock_codes_hck_msg.hck,
                        sizeof(req_ptr->message.add_lock_codes_hck_msg.hck),
                        lock_code_data_ptr->sp_lock.hck,
                        sizeof(lock_code_data_ptr->sp_lock.hck));

        for(;code_cnt < lock_code_data_ptr->sp_lock.sp_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_hck_msg.category.code_data.sp_3gpp_code_list.sp_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->sp_lock.sp_codes - 1)
          {
            req_ptr->message.add_lock_codes_hck_msg.category.code_data.sp_3gpp_code_list.num_sp_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr)
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_hck_msg.category.code_data.sp_3gpp_code_list,
                   0x00,
                   sizeof(simlock_sp_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      break;
    
    case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
      if(lock_code_data_ptr->sp_ehplmn_lock.iteration_cnt == 0x00)
      {
        req_ptr->msg_type                                   = SIMLOCK_SET_LOCK_CK_MSG;
        req_ptr->message.lock_ck_msg.slot                   = slot;
        req_ptr->message.lock_ck_msg.category.category_type = category;

        for(code_cnt = 0; code_cnt < lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes &&
                          code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++)
        {
          req_ptr->message.lock_ck_msg.category.code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[code_cnt] =
            lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt];
        }

        req_ptr->message.lock_ck_msg.category.code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes = code_cnt;

        /* Call the engine with lock request */
        simlock_status = simlock_process_message(req_ptr, NULL);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          break;
        }

        /* If lock codes are less than or equal to 20 then return success */
        if(code_cnt == lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes)
        {
          simlock_status = SIMLOCK_SUCCESS;
          break;
        }

        memset(req_ptr, 0x00, sizeof(simlock_message_request_data_type));

        /* Prepare add lock command if there are more than 20 lock codes */
        req_ptr->msg_type                                             = SIMLOCK_ADD_LOCK_CODES_CK_MSG;
        req_ptr->message.add_lock_codes_ck_msg.category.category_type = category;
        req_ptr->message.add_lock_codes_ck_msg.slot                   = slot;

        for(;code_cnt < lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes; code_cnt++, curr_code_cnt++)
        {
          req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_ehplmn_3gpp_code_list.sp_ehplmn_code_data[curr_code_cnt - 1] =
            lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt];

          if(curr_code_cnt == SIMLOCK_NUM_CODES_MAX ||
             code_cnt == lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes - 1)
          {
            req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_ehplmn_3gpp_code_list.num_sp_ehplmn_codes =
              curr_code_cnt;

            SIMLOCK_CHECK_AND_MEM_MALLOC(rsp_ptr,
                                         sizeof(simlock_message_response_data_type));
            if(rsp_ptr == NULL)
            {
              simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
              break;
            }

            simlock_status = simlock_process_message(req_ptr, rsp_ptr);

            SIMLOCK_MEM_FREE(rsp_ptr);
            if(simlock_status != SIMLOCK_SUCCESS)
            {
              break;
            }

            memset(&req_ptr->message.add_lock_codes_ck_msg.category.code_data.sp_ehplmn_3gpp_code_list,
                   0x00,
                   sizeof(simlock_sp_ehplmn_code_list_type));
            curr_code_cnt = 1;
          }
        }
      }
      break;

    default:
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
  }

  SIMLOCK_MEM_FREE(req_ptr);
  return simlock_status;
} /* simlock_rsu_config_send_lock */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PROCESS_PARTIAL_LOCK

DESCRIPTION
  This function processes the permanent lock of categories. In this case
  only NW lock is set

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_process_partial_lock
(
  const simlock_rsu_config_data_type    * config_data_ptr
)
{
  simlock_result_enum_type       simlock_status    = SIMLOCK_SUCCESS;
  simlock_category_data_type    *category_data_ptr = NULL;
  simlock_category_enum_type     category          = SIMLOCK_CATEGORY_3GPP_NW;

  if (config_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(category_data_ptr, sizeof(simlock_category_data_type));
  if(category_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* send unlock, in case there are any locks sets */
  for(category = SIMLOCK_CATEGORY_3GPP_NW;
      category <= SIMLOCK_CATEGORY_3GPP_SIM;
      category++)
  {
    simlock_status = simlock_rsu_config_process_permanent_unlock(category);
  }

  /* apply the NW lock based on the PLMNs received in the
     configuration data */
  if(config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes > 0)
  {
    simlock_status = simlock_rsu_config_send_lock(&config_data_ptr->code_data.lock_code_data,
                                                  SIMLOCK_CATEGORY_3GPP_NW,
                                                  SIMLOCK_SLOT_1);
  }

  SIMLOCK_MEM_FREE(category_data_ptr);
  return simlock_status;
} /* simlock_rsu_config_process_partial_lock */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_VALIDATE_INPUT_LOCK_DATA

DESCRIPTION
  This function validates the input lock data of the blob before unlocking
  and locking each category.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_validate_input_lock_data
(
  const simlock_rsu_config_lock_code_data_type    *lock_code_data_ptr
)
{
  uint8  code_cnt = 0;

  if (lock_code_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(lock_code_data_ptr->nw_lock.change_flag)
  {
    /* Check if the MCC/MNCs are valid */
    for(code_cnt = 0;
        code_cnt < lock_code_data_ptr->nw_lock.nw_codes &&
        code_cnt < SIMLOCK_NUM_CODES_MAX;
        code_cnt++)
    {
      if(!simlock_util_check_is_mcc_valid(lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt].mcc) ||
         !simlock_util_check_is_mnc_valid(&lock_code_data_ptr->nw_lock.nw_code_ptr[code_cnt].mnc))
      {
        return SIMLOCK_INVALID_LOCK_DATA;
      }
    }
  }

  if(lock_code_data_ptr->ns_lock.change_flag)
  {
    /* Check if the MCC/MNCs and d_6/d_7 are valid */
    for(code_cnt = 0;
        code_cnt < lock_code_data_ptr->ns_lock.ns_codes &&
        code_cnt < SIMLOCK_NUM_CODES_MAX;
        code_cnt++)
    {
      if(!simlock_util_check_is_mcc_valid(lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt].nw_code.mcc) ||
         !simlock_util_check_is_mnc_valid(&(lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt].nw_code.mnc)))
      {
        return SIMLOCK_INVALID_LOCK_DATA;
      }
      if(!SIMLOCK_IS_VALID_CODE_CHAR(lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt].imsi_digit6) ||
         !SIMLOCK_IS_VALID_CODE_CHAR(lock_code_data_ptr->ns_lock.ns_code_ptr[code_cnt].imsi_digit7))
      {
        return SIMLOCK_INVALID_LOCK_DATA;
      }
    }
  }

  if(lock_code_data_ptr->sp_lock.change_flag)
  {
    /* Check if the MCC/MNCs are valid */
    for(code_cnt = 0;
        code_cnt < lock_code_data_ptr->sp_lock.sp_codes &&
        code_cnt < SIMLOCK_NUM_CODES_MAX;
        code_cnt++)
    {
      if(!simlock_util_check_is_mcc_valid(lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt].nw_code.mcc) ||
         !simlock_util_check_is_mnc_valid(&(lock_code_data_ptr->sp_lock.sp_code_ptr[code_cnt].nw_code.mnc)))
      {
        return SIMLOCK_INVALID_LOCK_DATA;
      }
    }
  }

  if(lock_code_data_ptr->sp_ehplmn_lock.change_flag)
  {
    /* Check if the MCC/MNCs are valid */
    for(code_cnt = 0;
        code_cnt < lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes &&
        code_cnt < SIMLOCK_NUM_CODES_MAX;
        code_cnt++)
    {
      if(!simlock_util_check_is_mcc_valid(lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt].nw_code.mcc)      ||
         !simlock_util_check_is_mnc_valid(&(lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt].nw_code.mnc))        ||
         !simlock_util_check_is_mcc_valid(lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt].ehplmn_nw_code.mcc) ||
         !simlock_util_check_is_mnc_valid(&(lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_code_ptr[code_cnt].ehplmn_nw_code.mnc)))
      {
        return SIMLOCK_INVALID_LOCK_DATA;
      }
    }
  }

  return SIMLOCK_SUCCESS;
}/* simlock_rsu_config_validate_input_lock_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PROCESS_PERMANENT_LOCK

DESCRIPTION
  This function processes the permanent lock of categories. In this case
  both NW, NS and SP locks are set

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_process_permanent_lock
(
  const simlock_rsu_config_lock_code_data_type    *lock_code_data_ptr
)
{
  simlock_result_enum_type     simlock_status    = SIMLOCK_SUCCESS;

  if (lock_code_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* Validate that the lock data is valid and then proceed with applying of locks */
  if(simlock_rsu_config_validate_input_lock_data(lock_code_data_ptr) != SIMLOCK_SUCCESS)
  {
    return SIMLOCK_INVALID_LOCK_DATA;
  }

  /* If change flag is set to TRUE then unlock the category (NW, NS, SP, IMPI, SP-EHPLMN)
     to apply new configuration */
  if(lock_code_data_ptr->nw_lock.change_flag)
  {
    if(simlock_rsu_config_process_permanent_unlock(SIMLOCK_CATEGORY_3GPP_NW) != SIMLOCK_SUCCESS)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Apply NW lock before unlocking next categories. This makes sure that other
       categories are not unlocked if NW lock is not applied successfully*/
    if(lock_code_data_ptr->nw_lock.nw_codes > 0)
    {
      simlock_status = simlock_rsu_config_send_lock(lock_code_data_ptr,
                                                    SIMLOCK_CATEGORY_3GPP_NW,
                                                    SIMLOCK_SLOT_1);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        return SIMLOCK_GENERIC_ERROR;
      }
    }
  }

  if(lock_code_data_ptr->ns_lock.change_flag)
  {
    if(simlock_rsu_config_process_permanent_unlock(SIMLOCK_CATEGORY_3GPP_NS) != SIMLOCK_SUCCESS)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Apply NS lock before unlocking next categories. This makes sure that other
       categories are not unlocked if NS lock is not applied successfully*/
    if(lock_code_data_ptr->ns_lock.ns_codes > 0)
    {
      simlock_status = simlock_rsu_config_send_lock(lock_code_data_ptr,
                                                    SIMLOCK_CATEGORY_3GPP_NS,
                                                    SIMLOCK_SLOT_1);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        return SIMLOCK_GENERIC_ERROR;
      }
    }
  }

  if(lock_code_data_ptr->sp_lock.change_flag)
  {
    if(simlock_rsu_config_process_permanent_unlock(SIMLOCK_CATEGORY_3GPP_SP) != SIMLOCK_SUCCESS)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Apply SP lock for consistency .*/
    if(lock_code_data_ptr->sp_lock.sp_codes > 0)
    {
      simlock_status = simlock_rsu_config_send_lock(lock_code_data_ptr,
                                                    SIMLOCK_CATEGORY_3GPP_SP,
                                                    SIMLOCK_SLOT_1);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        return SIMLOCK_GENERIC_ERROR;
      }
    }
  }
  
  if(lock_code_data_ptr->sp_ehplmn_lock.change_flag)
  {
    if(simlock_rsu_config_process_permanent_unlock(SIMLOCK_CATEGORY_3GPP_SP_EHPLMN) != SIMLOCK_SUCCESS)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Apply SP-EHPLMN lock for consistency .*/
    if(lock_code_data_ptr->sp_ehplmn_lock.sp_ehplmn_codes > 0)
    {
      simlock_status = simlock_rsu_config_send_lock(lock_code_data_ptr,
                                                    SIMLOCK_CATEGORY_3GPP_SP_EHPLMN,
                                                    SIMLOCK_SLOT_1);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        return SIMLOCK_GENERIC_ERROR;
      }
    }
  }

  return simlock_status;
} /* simlock_rsu_config_process_permanent_lock */


#ifdef FEATURE_SIMLOCK_RSU_TIME
/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PROCESS_TEMPORARY_UNLOCK

DESCRIPTION
  This function processes the temporary unlock of categories locked

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_process_temporary_unlock
(
  const simlock_rsu_config_data_type    * config_data_ptr
)
{
  simlock_result_enum_type              simlock_status      = SIMLOCK_SUCCESS;
  simlock_category_enum_type            category            = SIMLOCK_CATEGORY_3GPP_NW;
  uint8                                 category_index      = 0;
  simlock_message_response_data_type  * get_status_resp_ptr = NULL;

  if (config_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* apply the configuration present in the temporary unlock
     request before starting the temporary unlock.
     This configuration will be active after the temporary
     unlock has expired */
  simlock_status = simlock_rsu_config_process_permanent_lock(&config_data_ptr->code_data.lock_code_data);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(get_status_resp_ptr,
                               sizeof(simlock_message_response_data_type));
  if (get_status_resp_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  /* get the lock status */
  simlock_status = simlock_rsu_config_get_lock_status(get_status_resp_ptr);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(get_status_resp_ptr);
    return simlock_status;
  }

  /* send the temporary unlock request.
     3GPP_SIM category is normally controlled by SafeSwitch module, while all
     other categories are under the control of operator. We don't want RSU to
     modify 3GPP_SIM. */
  for(category = SIMLOCK_CATEGORY_3GPP_NW;
      category < SIMLOCK_CATEGORY_3GPP_SIM;
      category++)
  {
    simlock_status = simlock_util_get_category_index(category, &category_index);
    if((simlock_status == SIMLOCK_SUCCESS) &&
       (category_index < SIMLOCK_CATEGORY_COUNT_MAX))
    {
      /* if the category is not enabled, then no need to temporary unlock.
         Check the status corresponding to slot1 (zero index) only as
         we lock categories on slot1 */
      if(!get_status_resp_ptr->message.get_status_resp.lock_info[0].category_info[category_index].category_status)
      {
        continue;
      }

      simlock_status = simlock_temp_unlock_process_request(SIMLOCK_SLOT_1,
                                                           category,
                                                           config_data_ptr->code_data.start_time,
                                                           config_data_ptr->code_data.end_time);
      if (simlock_status != SIMLOCK_SUCCESS)
      {
        break;
      }
    }
  } /* end loop categories */

  SIMLOCK_MEM_FREE(get_status_resp_ptr);
  return simlock_status;
} /* simlock_rsu_config_process_temporary_unlock */
#endif /* FEATURE_SIMLOCK_RSU_TIME */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_SEND_CONFIG_SWITCH

DESCRIPTION
  This function sends the config switch request to the simlock engine.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_send_config_switch
(
  simlock_slot_enum_type        slot,
  uint8                         lock_code
)
{
  simlock_result_enum_type              simlock_status        = SIMLOCK_SUCCESS;
  simlock_message_request_data_type   * change_config_req_ptr = NULL;

  SIMLOCK_CHECK_AND_MEM_MALLOC(change_config_req_ptr,
                               sizeof(simlock_message_request_data_type));
  if (change_config_req_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  change_config_req_ptr->msg_type                                = SIMLOCK_CHANGE_LIST_TYPE_MSG;
  change_config_req_ptr->message.change_list_type_msg.slot       = slot;
  change_config_req_ptr->message.change_list_type_msg.blacklist  = (lock_code == '0') ? FALSE : TRUE;

  /* call the engine with config switch request */
  simlock_status = simlock_process_message(change_config_req_ptr, NULL);

  SIMLOCK_MEM_FREE(change_config_req_ptr);

  return simlock_status;
} /* simlock_rsu_config_send_config_switch */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_APPLY_CONFIGURATION

DESCRIPTION
  This function applies configuration requested in the RSU data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_apply_configuration
(
  const simlock_rsu_config_data_type * config_data_ptr,
  simlock_rsu_mode_enum_type           rsu_mode
)
{
  simlock_result_enum_type    simlock_status  = SIMLOCK_SUCCESS;
  simlock_category_enum_type  category        = SIMLOCK_CATEGORY_3GPP_NW;

  if (config_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* disabled_slots_info_valid flag is set to TRUE.We need to set disabled
     slots info */
  if(config_data_ptr->code_data.disabled_slots_info_valid)
  {
    simlock_status = simlock_rsu_config_set_slot_operation(config_data_ptr->code_data.disabled_slots);

    if(simlock_status != SIMLOCK_SUCCESS)
    {
      return simlock_status;
    }
  }

  if(rsu_mode == SIMLOCK_RSU_MODE_C)
  {
    SIMLOCK_MSG_MED_1("lock_code 0x%x", config_data_ptr->header_data.data.lock_code);

    switch(config_data_ptr->header_data.data.lock_code)
    {
      case '0':
      case '1':
        /* Call SimLock function to switch */
        simlock_status = simlock_rsu_config_send_config_switch(SIMLOCK_SLOT_1,
                                                               config_data_ptr->header_data.data.lock_code);
        break;

      case '2':
        for(category = SIMLOCK_CATEGORY_3GPP_NW;
            category <= SIMLOCK_CATEGORY_3GPP_SIM;
            category++)
        {
          simlock_status = simlock_rsu_config_process_permanent_unlock(category);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            return simlock_status;
          }
        }
        break;

      default:
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
    }
  }
  else
  {
    switch(config_data_ptr->header_data.data.lock_state)
    {
      case SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK:
        simlock_status = simlock_rsu_config_process_permanent_lock(&config_data_ptr->code_data.lock_code_data);
        break;

      case SIMLOCK_RSU_CONFIG_LOCK_STATE_TEMPORARY_UNLOCK:
#ifdef FEATURE_SIMLOCK_RSU_TIME
        simlock_status = simlock_rsu_config_process_temporary_unlock(config_data_ptr);
#else
        simlock_status = SIMLOCK_UNSUPPORTED;
#endif /* FEATURE_SIMLOCK_RSU_TIME */
        break;

      case SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_UNLOCK:
        for(category = SIMLOCK_CATEGORY_3GPP_NW;
            category <= SIMLOCK_CATEGORY_3GPP_SIM;
            category++)
        {
          simlock_status = simlock_rsu_config_process_permanent_unlock(category);
          if(simlock_status != SIMLOCK_SUCCESS)
          {
            return simlock_status;
          }
        }
        break;

    case SIMLOCK_RSU_CONFIG_LOCK_STATE_PARTIAL_UNLOCK:
      if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        simlock_status = simlock_rsu_config_process_partial_lock(config_data_ptr);
      }
      else
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
      }
      break;

      default:
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
    }
  }

  return simlock_status;
} /* simlock_rsu_config_apply_configuration */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_CHECK_IMEI

DESCRIPTION
  This function checks if the IMEI received in the simlock configuration data
  matches the device IMEI and the IMEI stored in config (during lock procedure)

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_check_imei
(
  simlock_imei_type    imei_data
)
{
  simlock_slot_enum_type     slot                 = SIMLOCK_SLOT_1;
  uint8                      slot_index           = SIMLOCK_SLOT_COUNT_MAX;
  simlock_result_enum_type   simlock_status       = SIMLOCK_RSU_IMEI_VALIDATION_FAILED;
  simlock_config_data_type  *config_file_data_ptr = NULL;
  simlock_imei_type          device_imei;

  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Get the IMEI for each slot/context */
  for(slot = SIMLOCK_SLOT_1; slot < SIMLOCK_SLOT_NUM; slot++)
  {
    memset(&device_imei, 0x00, sizeof(simlock_imei_type));

    if(simlock_util_check_slot(slot) != SIMLOCK_SUCCESS ||
       simlock_util_get_imei(slot, &device_imei) != SIMLOCK_SUCCESS ||
       device_imei.imei_len == 0)
    {
      continue;
    }

    slot_index = SIMLOCK_SLOT_COUNT_MAX;
    if(SIMLOCK_SUCCESS != simlock_util_get_slot_index(slot, &slot_index) ||
       slot_index >= SIMLOCK_SLOT_COUNT_MAX)
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    if(config_file_data_ptr->imei[slot_index].imei_len == 0)
    {
      SIMLOCK_MSG_HIGH_1("IMEI not configured for simlock slot 0x%x", slot);
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* check if the IMEIs match */
    if(simlock_util_check_imei_matches(device_imei, imei_data) &&
       simlock_util_check_imei_matches(config_file_data_ptr->imei[slot_index], imei_data))
    {
      simlock_status = SIMLOCK_SUCCESS;
      break;
    }
  }

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_rsu_config_check_imei */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_VERIFY_DATA_INTEGRITY

DESCRIPTION
  This function verifies the integrity of simlock configuration data by
  comparing the macs

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_verify_data_integrity
(
  const simlock_rsu_config_data_type * config_data_ptr,
  simlock_rsu_mode_enum_type           rsu_mode
)
{
  simlock_result_enum_type        simlock_status    = SIMLOCK_SUCCESS;
  simlock_data_type               msg_digest_info   = {0, NULL};
  simlock_rsu_pubkey_data_type  * pubkey_ptr        = NULL;
  simlock_rsu_pubkey_enum_type    pubkey_type       = SIMLOCK_RSU_PUBKEY_BLOB_VALIDATION;
  secapi_pkx_padding_enum_type    padding_type      = SECAPI_PKX_RSA_PADDING_SIG_PSS;

  if (config_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

#ifdef FEATURE_QMI_UIM_SECURE_CHANNEL_TEST
  if (config_data_ptr->header_data.protection_type == SIMLOCK_RSU_CONFIG_PROTECTION_NONE ||
      rsu_mode == SIMLOCK_RSU_MODE_C)
  {
    /* In case of test build, allow binary blob without HMAC or RSA for testing purposes */
    SIMLOCK_MSG_MED_0("Allow the blob without protection");
    return SIMLOCK_SUCCESS;
  }
#endif /* FEATURE_QMI_UIM_SECURE_CHANNEL_TEST */

  if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
  {
    simlock_data_type               hmac = {0, NULL};

    if(config_data_ptr->header_data.protection_type != SIMLOCK_RSU_CONFIG_PROTECTION_SHA256)
    {
      return SIMLOCK_UNSUPPORTED;
    }

    simlock_status = simlock_rsu_process_generate_hash(
                       config_data_ptr->auth_data.msg_len,
                       config_data_ptr->auth_data.msg_data_ptr,
                       &hmac);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      if(memcmp(hmac.data_ptr, config_data_ptr->auth_data.data.hmac,
                SIMLOCK_HMAC_SHA256_LEN) != 0)
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
      }
    }

    SIMLOCK_MEM_FREE(hmac.data_ptr);

    return simlock_status;
  }
  /* If there is a protection to the blob, msg_len should not be zero
     And in production environment, no protection is not allowed */
  if((config_data_ptr->header_data.protection_type != SIMLOCK_RSU_CONFIG_PROTECTION_RSA) ||
     (config_data_ptr->auth_data.msg_len == 0) ||
     (config_data_ptr->auth_data.msg_data_ptr == NULL))
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  if(rsu_mode == SIMLOCK_RSU_MODE_C)
  {
    /* As per requirement we have to use same key for both encryption and blob validation for
       RSU_MODE_C */
    pubkey_type  = SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION;
    padding_type = SECAPI_PKX_RSA_PADDING_SIG_PKCS1_V15;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(pubkey_ptr, sizeof(simlock_rsu_pubkey_data_type));
  if(pubkey_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  simlock_status = simlock_rsu_key_read_pubkey_data(pubkey_type,
                                                    pubkey_ptr,
                                                    rsu_mode);

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(pubkey_ptr);
    return simlock_status;
  }

  simlock_status = simlock_crypto_generate_digest_sha256(config_data_ptr->auth_data.msg_data_ptr,
                                                         config_data_ptr->auth_data.msg_len,
                                                         &msg_digest_info);

  if(simlock_status == SIMLOCK_SUCCESS)
  {
    uint8 *rsa_ptr = NULL;

    SIMLOCK_CHECK_AND_MEM_MALLOC(rsa_ptr, sizeof(simlock_rsa_signature_type));
    if(rsa_ptr == NULL)
    {
      SIMLOCK_MEM_FREE(pubkey_ptr);
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    simlock_memscpy(rsa_ptr,
                    sizeof(simlock_rsa_signature_type),
                    config_data_ptr->auth_data.data.rsa,
                    sizeof(simlock_rsa_signature_type));

    /* Validating the signature is as follows
       Blob request would have a message(M) appended with signature(S).
       a. Generate the message hash(H) from the message(M) in blob request using SHA256
       b. Give H generated above and Signature (S) along with Public Key to secapi_pkx_authenticate()
       c. Success response for above API means signature (S) is generated for the message (M)
          and the data integrity of the blob is verified */
    simlock_status = simlock_crypto_rsa(SIMLOCK_CRYPTO_RSA_SIGNATURE_VALIDATE,
                                        rsa_ptr,
                                        SIMLOCK_RSA_LEN,
                                        pubkey_ptr->public_key.modulus,
                                        SIMLOCK_PUBKEY_MODULUS_LEN,
                                        pubkey_ptr->public_key.exponent,
                                        SIMLOCK_PUBKEY_EXPONENT_LEN,
                                        &msg_digest_info,
                                        padding_type);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      simlock_status = SIMLOCK_RSU_DATA_VALIDATION_FAILED;
    }

    SIMLOCK_MEM_FREE(rsa_ptr);
  }

  SIMLOCK_MEM_FREE(pubkey_ptr);
  SIMLOCK_MEM_FREE(msg_digest_info.data_ptr);
  return simlock_status;
} /* simlock_rsu_config_verify_data_integrity */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PARSE_LOCK_STATE

DESCRIPTION
  This function converts the lock state value in configuration data to
  the corresponding enum type

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_parse_lock_state
(
  uint8                                        parsed_value,
  simlock_rsu_config_lock_state_enum_type   *  lock_state_ptr,
  simlock_rsu_mode_enum_type                   rsu_mode
)
{
  if(lock_state_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  switch(parsed_value)
  {
    case 0:
      *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK;
      break;

    case 1:
      if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_TEMPORARY_UNLOCK;
      }
      else
      {
      *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_UNLOCK;
      }
      break;

    case 2:
      if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_UNLOCK;
      }
      else
      {
      *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_TEMPORARY_UNLOCK;
      }	
      break;

    case 3:
      if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        *lock_state_ptr = SIMLOCK_RSU_CONFIG_LOCK_STATE_PARTIAL_UNLOCK;
      }
      else
      {
        return SIMLOCK_GENERIC_ERROR;
      }
      break;

    default:
      return SIMLOCK_GENERIC_ERROR;
  }

  return SIMLOCK_SUCCESS;
}/* simlock_rsu_config_parse_lock_state */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PARSE_PROTECTION_TYPE

DESCRIPTION
  This function converts the protection type value in configuration data to
  the corresponding enum type

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_parse_protection_type
(
  uint8                                      parsed_value,
  simlock_rsu_mode_enum_type                 rsu_mode,
  simlock_rsu_config_protection_enum_type   *protection_type_ptr
)
{
  if(protection_type_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  switch(parsed_value)
  {
    case 0:
      *protection_type_ptr = SIMLOCK_RSU_CONFIG_PROTECTION_NONE;
      break;

    case 1:
      if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        *protection_type_ptr = SIMLOCK_RSU_CONFIG_PROTECTION_SHA256;
      }
      else
      {
      *protection_type_ptr = SIMLOCK_RSU_CONFIG_PROTECTION_RSA;
      }
       break;

    default:
      return SIMLOCK_GENERIC_ERROR;
  }

  return SIMLOCK_SUCCESS;
}/* simlock_rsu_config_parse_protection_type */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_IS_MCC_MNC_IN_ALWAYS_PRESENT_LIST

DESCRIPTION
  This function checks if the mcc-mnc is present in the list of mcc-mnc
  that are always added in the code data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static boolean simlock_rsu_config_is_mcc_mnc_in_always_present_list
(
  simlock_rsu_config_non_ascii_mcc_mnc_type      mcc_mnc
)
{
  uint8       list_cnt = 0;

  for (list_cnt = 0; list_cnt < (sizeof(simlock_rsu_always_present_codes)/sizeof(simlock_rsu_config_non_ascii_mcc_mnc_type));
       list_cnt++)
  {
    if(simlock_rsu_always_present_codes[list_cnt] == mcc_mnc)
    {
      return TRUE;
    }
  }

  return FALSE;
} /* simlock_rsu_config_is_mcc_mnc_in_always_present_list */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_CONVERT_NON_ASCII_MCC_MNC

DESCRIPTION
  This function converts the non ascii MCC/MNC data in the configuration data to the
  simlock NW type

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_convert_non_ascii_mcc_mnc
(
  simlock_rsu_config_non_ascii_mcc_mnc_type      mcc_mnc,
  simlock_nw_code_data_type                   *  mcc_mnc_data_ptr
)
{
  uint8      temp_data = 0;

  if(mcc_mnc_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  temp_data = (uint8)((mcc_mnc & 0xFF000000) >> 24);

  mcc_mnc_data_ptr->mcc[0] = (((temp_data & SIMLOCK_RSU_CONFIG_HIGH_NIBBLE) >> 4) + '0');
  mcc_mnc_data_ptr->mcc[1] = ((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) + '0');

  temp_data = (uint8)((mcc_mnc & 0x00FF0000) >> 16);

  mcc_mnc_data_ptr->mcc[2] = (((temp_data & SIMLOCK_RSU_CONFIG_HIGH_NIBBLE) >> 4) + '0');

  if((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) == 0x0F)
  {
    mcc_mnc_data_ptr->mnc.mnc_len = 2;

    temp_data = (uint8)((mcc_mnc & 0x0000FF00) >> 8);
    mcc_mnc_data_ptr->mnc.mnc_data[0] = (((temp_data & SIMLOCK_RSU_CONFIG_HIGH_NIBBLE) >> 4) + '0');
    mcc_mnc_data_ptr->mnc.mnc_data[1] = ((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) + '0');
  }
  else
  {
    /* The correct coding for 2-digit MNC is 0x001F0100.
       But the test mcc-mnc is coded in blob as 0x00101000. */
    if((memcmp(simlock_rsu_test_mcc, mcc_mnc_data_ptr->mcc,
               SIMLOCK_MCC_LEN) == 0))
    {
      mcc_mnc_data_ptr->mnc.mnc_len = 2;

      mcc_mnc_data_ptr->mnc.mnc_data[0] = ((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) + '0');

      temp_data = (uint8)((mcc_mnc & 0x0000FF00) >> 8);
      mcc_mnc_data_ptr->mnc.mnc_data[1] = (((temp_data & SIMLOCK_RSU_CONFIG_HIGH_NIBBLE) >> 4) + '0');
    }
    else
    {
      mcc_mnc_data_ptr->mnc.mnc_len = 3;
      mcc_mnc_data_ptr->mnc.mnc_data[0] = ((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) + '0');

      temp_data = (uint8)((mcc_mnc & 0x0000FF00) >> 8);
      mcc_mnc_data_ptr->mnc.mnc_data[1] = (((temp_data & SIMLOCK_RSU_CONFIG_HIGH_NIBBLE) >> 4) + '0');
      mcc_mnc_data_ptr->mnc.mnc_data[2] = ((temp_data & SIMLOCK_RSU_CONFIG_LOW_NIBBLE) + '0');
    }
  }

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_convert_non_ascii_mcc_mnc */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_CONVERT_NON_ASCII_GID

DESCRIPTION
  This function converts the non ascii gid data in the configuration data to the
  simlock gid type

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_convert_non_ascii_gid
(
  simlock_rsu_config_non_ascii_gid_type    gid,
  simlock_gid_type                        *gid_data_ptr
)
{
  if(gid_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  gid_data_ptr->gid_len = SIMLOCK_RSU_CONFIG_GID_SIZE;

  gid_data_ptr->gid_data[0] = (uint8)((gid & SIMLOCK_RSU_CONFIG_HIGH_BYTE) >> 8);
  gid_data_ptr->gid_data[1] = (uint8)(gid & SIMLOCK_RSU_CONFIG_LOW_BYTE);

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_convert_non_ascii_gid */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_MODE_E_F_POPULATE_CODE_DATA

DESCRIPTION
  This function first converts the mcc, mnc and gid populates the lock code
  data by converting the lock/unlock data given in request.
  Freeing memory allocated in this function is caller's responsibility.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_mode_e_f_populate_code_data(
  simlock_rsu_config_non_ascii_mcc_mnc_list_type   mcc_mnc_info,
  simlock_rsu_config_non_ascii_gid_list_type       gid_info,
  simlock_rsu_mode_enum_type                       rsu_mode,
  simlock_rsu_config_data_type                    *config_data_ptr)
{ 
  simlock_result_enum_type   simlock_status          = SIMLOCK_SUCCESS;
  uint16                     code_cnt                = 0;
  uint16                     gid_code_cnt            = 0;
  uint16                     num_nw_lock_codes       = 0;
  uint16                     num_sp_lock_codes       = 0;
  uint8                      nw_code_cnt             = 0;
  uint8                      sp_code_cnt             = 0;
  simlock_nw_code_data_type  nw_code_data;
  uint8                      always_present_code_cnt = 0;

  if (config_data_ptr == NULL || 
      (rsu_mode != SIMLOCK_RSU_MODE_E && rsu_mode != SIMLOCK_RSU_MODE_F))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* We will add always present code only for mode E */
  if(rsu_mode == SIMLOCK_RSU_MODE_E)
  {
    always_present_code_cnt = sizeof(simlock_rsu_always_present_codes) / sizeof(simlock_rsu_config_non_ascii_mcc_mnc_type);
  }

  /* Populate NW Lock
     Number of possible network lock codes is the sum of
     a. Default MCC MNCs present in simlock_rsu_always_present_codes
     b. Number of MCC MNCs given in the request */
  num_nw_lock_codes = always_present_code_cnt + mcc_mnc_info.num_mcc_mnc_codes;

  if(num_nw_lock_codes > 0)
  {
    SIMLOCK_CHECK_AND_MEM_MALLOC(config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr,
                                 num_nw_lock_codes * sizeof(simlock_nw_code_data_type));
    if (config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr == NULL)
    {
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
  }

  /* Add the always present codes */
  for(code_cnt = 0; code_cnt < always_present_code_cnt &&
                    nw_code_cnt < SIMLOCK_NUM_CODES_MAX; code_cnt++, nw_code_cnt++)
  {
    memset(&nw_code_data, 0x00, sizeof(simlock_nw_code_data_type));

    simlock_status = simlock_rsu_config_convert_non_ascii_mcc_mnc(simlock_rsu_always_present_codes[code_cnt],
                                                                  &nw_code_data);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes++;
      config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr[nw_code_cnt] = nw_code_data;
    }
  }

  /* Add the codes given in the request */
  for(code_cnt = 0; (code_cnt < mcc_mnc_info.num_mcc_mnc_codes &&
                     nw_code_cnt < SIMLOCK_NUM_CODES_MAX &&
                     mcc_mnc_info.mcc_mnc_code_ptr != NULL); code_cnt++, nw_code_cnt++)
  {
    if(always_present_code_cnt > 0 &&
       simlock_rsu_config_is_mcc_mnc_in_always_present_list(mcc_mnc_info.mcc_mnc_code_ptr[code_cnt]))
    {
      continue;
    }

    memset(&nw_code_data, 0x00, sizeof(simlock_nw_code_data_type));

    simlock_status = simlock_rsu_config_convert_non_ascii_mcc_mnc(mcc_mnc_info.mcc_mnc_code_ptr[code_cnt],
                                                                  &nw_code_data);

    if(simlock_status == SIMLOCK_SUCCESS)
    {
      config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes++;
      config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr[nw_code_cnt] = nw_code_data;
    }
  }

  /* Set change flag to TRUE to consider the code data */
  config_data_ptr->code_data.lock_code_data.nw_lock.change_flag = TRUE;

  /* Populate SP Lock
     Number of possible SP lock codes is the product of
     a. num_nw_lock_codes (calculated above)
     b. Number of GIDs given in the request */
  num_sp_lock_codes = num_nw_lock_codes * gid_info.num_gid_codes;
  if(num_sp_lock_codes > 0)
  {
    SIMLOCK_CHECK_AND_MEM_MALLOC(config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr,
                                 num_sp_lock_codes * sizeof(simlock_sp_code_data_type));
    if (config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr == NULL)
    {
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
  }

  /* Add the SP lock codes with always present codes MCC MNCs with GIDs given in request */
  for(code_cnt = 0; code_cnt < always_present_code_cnt;
      code_cnt++)
  {
    memset(&nw_code_data, 0x00, sizeof(simlock_nw_code_data_type));

    simlock_status = simlock_rsu_config_convert_non_ascii_mcc_mnc(simlock_rsu_always_present_codes[code_cnt],
                                                                  &nw_code_data);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      for(gid_code_cnt = 0; (gid_code_cnt < gid_info.num_gid_codes &&
                             sp_code_cnt < SIMLOCK_NUM_CODES_MAX &&
                             gid_info.gid_code_ptr != NULL); gid_code_cnt++, sp_code_cnt++)
      {
        config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes++;
        config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[sp_code_cnt].nw_code = nw_code_data;
        simlock_status = simlock_rsu_config_convert_non_ascii_gid(gid_info.gid_code_ptr[gid_code_cnt],
                                                                  &config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[sp_code_cnt].gid1);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          return simlock_status;
        }
      } /* end loop for GIDs */
    }
  } /* end loop for MCC MNCs */

  /* Add the SP lock codes with MCC MNCs and GIDs given in request */
  for(code_cnt = 0; (code_cnt < mcc_mnc_info.num_mcc_mnc_codes &&
                     mcc_mnc_info.mcc_mnc_code_ptr != NULL); code_cnt++)
  {
    if(always_present_code_cnt > 0 &&
       simlock_rsu_config_is_mcc_mnc_in_always_present_list(mcc_mnc_info.mcc_mnc_code_ptr[code_cnt]))
    {
      continue;
    }

    memset(&nw_code_data, 0x00, sizeof(simlock_nw_code_data_type));

    simlock_status = simlock_rsu_config_convert_non_ascii_mcc_mnc(mcc_mnc_info.mcc_mnc_code_ptr[code_cnt],
                                                                  &nw_code_data);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      for(gid_code_cnt = 0; (gid_code_cnt < gid_info.num_gid_codes &&
                             sp_code_cnt < SIMLOCK_NUM_CODES_MAX &&
                             gid_info.gid_code_ptr != NULL) ; gid_code_cnt++, sp_code_cnt++)
      {
        config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes++;
        config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[sp_code_cnt].nw_code = nw_code_data;
        simlock_status = simlock_rsu_config_convert_non_ascii_gid(gid_info.gid_code_ptr[gid_code_cnt],
                                                                  &config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[sp_code_cnt].gid1);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          return simlock_status;
        }
      } /* end loop for GIDs */
    }
  } /* end loop for MCC MNCs */

  /* Set change flag to TRUE to consider the code data */
  config_data_ptr->code_data.lock_code_data.sp_lock.change_flag = TRUE;
    
  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_mode_e_f_populate_code_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PARSE_MODE_E_F_DATA

DESCRIPTION
  This function parses the simlock configuration data received in the request
  for mode E and F

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_parse_mode_e_f_data
(
  uint32                             in_data_len,
  const uint8                     *  in_data_ptr,
  simlock_rsu_config_data_type    *  out_config_data_ptr,
  simlock_rsu_mode_enum_type         rsu_mode
)
{
  simlock_result_enum_type                         simlock_status        = SIMLOCK_SUCCESS;
  uint16                                           offset                = 0;
  uint16                                           code_index            = 0;
  uint16                                           config_data_len       = 0;
  simlock_rsu_config_non_ascii_mcc_mnc_type        mcc_mnc               = 0;
  simlock_rsu_config_non_ascii_gid_type            gid                   = 0;
  uint16                                           num_of_codes          = 0;
  simlock_rsu_config_non_ascii_mcc_mnc_list_type   mcc_mnc_info          = {0, NULL};
  simlock_rsu_config_non_ascii_gid_list_type       gid_info              = {0, NULL};

  PACKED struct PACKED_POST
  {
    uint16   version_major;
    uint16   version_minor;
    uint8    protection_type;
    uint8    simlock_state;
    uint16   blob_len;
  } top_data;

  PACKED struct PACKED_POST
  {
    uint8    rfu[SIMLOCK_RSU_CONFIG_RFU_SIZE];
    uint64   start_time_ms;
    uint32   end_time_s;
    uint8    imei[SIMLOCK_MODE_E_F_IMEI_MAX_LEN];
  } bottom_data;

  if((in_data_ptr == NULL) ||
     (in_data_len == 0)    ||
     (out_config_data_ptr == NULL) ||
     (rsu_mode != SIMLOCK_RSU_MODE_E &&  rsu_mode != SIMLOCK_RSU_MODE_F))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(&top_data, 0x00, sizeof(top_data));
  memset(&bottom_data, 0x00, sizeof(bottom_data));

  /* if the data len is less than the top and bottome data size,
     then it is not valid */
  if(in_data_len < (sizeof(top_data) + sizeof(bottom_data)))
  {
    SIMLOCK_MSG_ERR_1("RSU config data length is less than minimum size, 0x%x",
                      in_data_len);
    return SIMLOCK_RSU_DATA_TOO_SHORT;
  }

  /* copy the config top data to packed struct */
  simlock_memscpy((uint8 *)&top_data,
                  sizeof(top_data),
                  in_data_ptr,
                  sizeof(top_data));

  out_config_data_ptr->header_data.version_major = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(top_data.version_major);
  out_config_data_ptr->header_data.version_minor = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(top_data.version_minor);
  
  if(((rsu_mode == SIMLOCK_RSU_MODE_E  &&
       out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MAJOR_VERSION_MAX &&
       out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MINOR_VERSION_MAX) || 
      (rsu_mode == SIMLOCK_RSU_MODE_F  &&
       out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MAJOR_VERSION_MAX &&
       out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MINOR_VERSION_MAX)) == FALSE)
  {
    SIMLOCK_MSG_ERR_1("Remote Lock blob version check failed for rsu_modde 0x%x", rsu_mode);
    return SIMLOCK_UNSUPPORTED;
  }

  simlock_status = simlock_rsu_config_parse_protection_type(top_data.protection_type,
                                                            rsu_mode,
                                                            &out_config_data_ptr->header_data.protection_type);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("failed to parse RSU protection type 0x%x",
                      top_data.protection_type);
    return simlock_status;
  }

  simlock_status = simlock_rsu_config_parse_lock_state(top_data.simlock_state,
                                                       &out_config_data_ptr->header_data.data.lock_state,
                                                       rsu_mode);
  /* Partial lock not suppported for mode F*/
  if(simlock_status == SIMLOCK_SUCCESS  &&
     rsu_mode  ==  SIMLOCK_RSU_MODE_F   &&
     top_data.simlock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PARTIAL_UNLOCK)
  {
    simlock_status = SIMLOCK_UNSUPPORTED;
  }

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_2("failed to parse RSU lock state 0x%x, status 0x%x",
                      top_data.simlock_state, simlock_status);
    return simlock_status;
  }

  config_data_len = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(top_data.blob_len);

  /* the config data len plus the top data len is
     less than than total data size, it is an error */
  if((config_data_len + sizeof(top_data)) < in_data_len)
  {
    SIMLOCK_MSG_ERR_2("RSU config data lengths inconsistent, Total len:0x%x, Data len: 0x%x",
                       in_data_len, config_data_len);
    return SIMLOCK_RSU_DATA_TOO_SHORT;
  }

  offset = sizeof(top_data);

  do
  {
    if (offset + sizeof(num_of_codes) > in_data_len)
    {
      simlock_status = SIMLOCK_RSU_DATA_TOO_SHORT;
      break;
    }

    /* get the number of mcc-mnc */
    simlock_memscpy((uint8 *)&num_of_codes,
                    sizeof(num_of_codes),
                    &in_data_ptr[offset],
                    sizeof(num_of_codes));

    mcc_mnc_info.num_mcc_mnc_codes = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(num_of_codes);

    SIMLOCK_MSG_MED_1("Num MCC-MNCs - 0x%x",
                       mcc_mnc_info.num_mcc_mnc_codes);

    if(mcc_mnc_info.num_mcc_mnc_codes > 0)
    {
      /* caller is responsible to FREE this memory */
      SIMLOCK_CHECK_AND_MEM_MALLOC(mcc_mnc_info.mcc_mnc_code_ptr,
                                   (mcc_mnc_info.num_mcc_mnc_codes * sizeof(simlock_rsu_config_non_ascii_mcc_mnc_type)));
      if(mcc_mnc_info.mcc_mnc_code_ptr == NULL)
      {
        simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
        break;
      }
    }

    offset += sizeof(num_of_codes);

    for(code_index = 0; code_index < mcc_mnc_info.num_mcc_mnc_codes; code_index++)
    {
      if(offset + sizeof(mcc_mnc) > in_data_len)
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }

      /* get the mcc-mnc */
      simlock_memscpy((uint8 *)&mcc_mnc,
                      sizeof(mcc_mnc),
                      &in_data_ptr[offset],
                      sizeof(mcc_mnc));

      mcc_mnc_info.mcc_mnc_code_ptr[code_index] = SIMLOCK_RSU_SWAP_BYTE_ORDER_32(mcc_mnc);
      offset += sizeof(mcc_mnc);
    }

    if (code_index != mcc_mnc_info.num_mcc_mnc_codes)
    {
      break;
    }

    if(offset + sizeof(num_of_codes) > in_data_len)
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    /* get the number of gids */
    simlock_memscpy((uint8 *)&num_of_codes,
                     sizeof(num_of_codes),
                     &in_data_ptr[offset],
                      sizeof(num_of_codes));

    gid_info.num_gid_codes = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(num_of_codes);

    SIMLOCK_MSG_MED_1("Num GIDs - 0x%x",
                      gid_info.num_gid_codes);

    if(gid_info.num_gid_codes > 0)
    {
      /* caller is responsible to FREE this memory */
      SIMLOCK_CHECK_AND_MEM_MALLOC(gid_info.gid_code_ptr,
                                   (gid_info.num_gid_codes * sizeof(simlock_rsu_config_non_ascii_gid_type)));
      if(gid_info.gid_code_ptr == NULL)
      {
        simlock_status = SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
    }

    offset += sizeof(num_of_codes);

    for(code_index = 0;
        code_index < gid_info.num_gid_codes;
        code_index++)
    {
      if(offset + sizeof(gid) > in_data_len)
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
        break;
      }

      /* get the gid */
      simlock_memscpy((uint8 *)&gid,
                      sizeof(gid),
                      &in_data_ptr[offset],
                      sizeof(gid));
      gid_info.gid_code_ptr[code_index] = SIMLOCK_RSU_SWAP_BYTE_ORDER_16(gid);
      offset += sizeof(gid);
    }

    if (code_index != gid_info.num_gid_codes)
    {
      break;
    }

    simlock_status = simlock_rsu_config_mode_e_f_populate_code_data(mcc_mnc_info, gid_info, rsu_mode, out_config_data_ptr);
  }while(0);

  SIMLOCK_MEM_FREE(mcc_mnc_info.mcc_mnc_code_ptr);
  SIMLOCK_MEM_FREE(gid_info.gid_code_ptr);

  if (simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  if(offset + sizeof(bottom_data) > in_data_len)
  {
    return SIMLOCK_RSU_DATA_TOO_SHORT;
  }

  /* copy the bottom config data to packed struct */
  simlock_memscpy((uint8 *)&bottom_data,
                  sizeof(bottom_data),
                  &in_data_ptr[offset],
                  sizeof(bottom_data));

  out_config_data_ptr->code_data.start_time = (SIMLOCK_RSU_SWAP_BYTE_ORDER_64(bottom_data.start_time_ms) / 1000);
  out_config_data_ptr->code_data.end_time   = SIMLOCK_RSU_SWAP_BYTE_ORDER_32(bottom_data.end_time_s);

  out_config_data_ptr->imei.imei_len = SIMLOCK_IMEI_MAX;

  /* imei in the config is 16-bytes and is zero padded
     in the end. We ignore the last byte */
  simlock_memscpy(out_config_data_ptr->imei.imei_data,
                  out_config_data_ptr->imei.imei_len,
                  bottom_data.imei,
                  out_config_data_ptr->imei.imei_len);

  offset += sizeof(bottom_data);

  if(out_config_data_ptr->header_data.protection_type == SIMLOCK_RSU_CONFIG_PROTECTION_SHA256)
  {
    if(offset + sizeof(out_config_data_ptr->auth_data.data.hmac) > in_data_len)
    {
      return SIMLOCK_RSU_DATA_TOO_SHORT;
    }

   /* copy the hmac data */
    simlock_memscpy(out_config_data_ptr->auth_data.data.hmac,
                    sizeof(out_config_data_ptr->auth_data.data.hmac),
                    &in_data_ptr[offset],
                    sizeof(out_config_data_ptr->auth_data.data.hmac));

    /* the msg to autheniticate is the entire data except the HMAC
       in the data */
    out_config_data_ptr->auth_data.msg_len = (in_data_len - SIMLOCK_HMAC_SHA256_LEN);

    /* caller is responsible to FREE this memory */
    SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->auth_data.msg_data_ptr,
                                 out_config_data_ptr->auth_data.msg_len);
    if(out_config_data_ptr->auth_data.msg_data_ptr == NULL)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* copy the data without the trailing mac This msg data
       will be required in the HMAC verification */
    simlock_memscpy(out_config_data_ptr->auth_data.msg_data_ptr,
                    out_config_data_ptr->auth_data.msg_len,
                    in_data_ptr,
                    out_config_data_ptr->auth_data.msg_len);
  }

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_parse_mode_e_f_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_POPULATE_LOCK_INFO

DESCRIPTION
  This function populates lock related information - Change flag,
  Iteration count, Salt and Hashed CK

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_populate_lock_info(
  const uint8                        *in_data_ptr,
  uint32                              in_data_len,
  simlock_category_enum_type          simlock_category,
  uint16                              offset,
  simlock_rsu_config_data_type       *out_config_data_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint8               change_flag;
    uint32              iteration_cnt;
    simlock_salt_type   salt;
    simlock_hck_type    hck;
  } blob_lock_data;

  if(in_data_ptr              == NULL ||
     out_config_data_ptr      == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  if(offset + sizeof(blob_lock_data) >= in_data_len)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  memset(&blob_lock_data, 0x00, sizeof(blob_lock_data));

  simlock_memscpy((uint8 *)&blob_lock_data,
                  sizeof(blob_lock_data),
                  in_data_ptr + offset,
                  sizeof(blob_lock_data));

  if(blob_lock_data.change_flag != SIMLOCK_REMOTE_LOCK_CODE_DATA_NO_CHANGE &&
     blob_lock_data.change_flag != SIMLOCK_REMOTE_LOCK_CODE_DATA_CHANGE)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Parse lock data only if change flag is set to 1 */
  if(blob_lock_data.change_flag == SIMLOCK_REMOTE_LOCK_CODE_DATA_CHANGE)
  {
    switch(simlock_category)
    {
      case SIMLOCK_CATEGORY_3GPP_NW:
        out_config_data_ptr->code_data.lock_code_data.nw_lock.change_flag = TRUE;
        out_config_data_ptr->code_data.lock_code_data.nw_lock.iteration_cnt =
          SIMLOCK_RSU_SWAP_BYTE_ORDER_32(blob_lock_data.iteration_cnt);
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.nw_lock.salt,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.nw_lock.salt),
                        blob_lock_data.salt,
                        SIMLOCK_SALT_LEN);
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.nw_lock.hck,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.nw_lock.hck),
                        blob_lock_data.hck,
                        SIMLOCK_HCK_LEN);
        break;

      case SIMLOCK_CATEGORY_3GPP_NS:
        out_config_data_ptr->code_data.lock_code_data.ns_lock.change_flag = TRUE;
        out_config_data_ptr->code_data.lock_code_data.ns_lock.iteration_cnt =
          SIMLOCK_RSU_SWAP_BYTE_ORDER_32(blob_lock_data.iteration_cnt);
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.ns_lock.salt,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.ns_lock.salt),
                        blob_lock_data.salt,
                        SIMLOCK_SALT_LEN);
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.ns_lock.hck,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.ns_lock.hck),
                        blob_lock_data.hck,
                        SIMLOCK_HCK_LEN);
        break;

      case SIMLOCK_CATEGORY_3GPP_SP:
        out_config_data_ptr->code_data.lock_code_data.sp_lock.change_flag = TRUE;
        out_config_data_ptr->code_data.lock_code_data.sp_lock.iteration_cnt =
          SIMLOCK_RSU_SWAP_BYTE_ORDER_32(blob_lock_data.iteration_cnt);
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.sp_lock.salt,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.sp_lock.salt),
                        blob_lock_data.salt,
                        sizeof(blob_lock_data.salt));
        simlock_memscpy(out_config_data_ptr->code_data.lock_code_data.sp_lock.hck,
                        sizeof(out_config_data_ptr->code_data.lock_code_data.sp_lock.hck),
                        blob_lock_data.hck,
                        sizeof(blob_lock_data.hck));
        break;

      default:
        return SIMLOCK_GENERIC_ERROR;
    }
  }

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_populate_lock_info */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_CONVERT_MCC_MNC

DESCRIPTION
  This function converts the mcc mnc to the required format.
  data_ptr should be a buffer of minimum 6 bytes and this is to be ensured by caller.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type  simlock_rsu_config_convert_mcc_mnc(
  const uint8                 *data_ptr,
  uint16                       data_len,
  simlock_nw_code_data_type   *mcc_mnc_ptr
)
{
  uint8 offset = 0;
  uint8 i      = 0;

  if(data_ptr == NULL ||
     data_len < SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE ||
     mcc_mnc_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  for(i = 0; (i < SIMLOCK_MCC_LEN && offset < data_len); i++)
  {
    mcc_mnc_ptr->mcc[i] = data_ptr[offset++];
  }

  mcc_mnc_ptr->mnc.mnc_len = 3;

  for(i = 0; (i < SIMLOCK_MNC_MAX && offset < data_len); i++)
  {
    mcc_mnc_ptr->mnc.mnc_data[i] = data_ptr[offset++];
  }

  if(mcc_mnc_ptr->mnc.mnc_data[i - 1] == 'F')
  {
    mcc_mnc_ptr->mnc.mnc_len = 2;
    mcc_mnc_ptr->mnc.mnc_data[i - 1] = '\0';
  }

  return SIMLOCK_SUCCESS;
}/* simlock_rsu_config_convert_mcc_mnc */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_PARSE_DATA

DESCRIPTION
  This function parse the simlock configuration data received in the request

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_parse_data
(
  uint32                             in_data_len,
  const uint8                     *  in_data_ptr,
  simlock_rsu_config_data_type    *  out_config_data_ptr,
  simlock_rsu_mode_enum_type         rsu_mode
)
{
  simlock_result_enum_type         simlock_status  = SIMLOCK_SUCCESS;
  uint16                           offset          = 0;
  uint16                           code_index      = 0;
  uint16                           blob_len        = 0;
  simlock_nw_code_data_type        mcc_mnc_data;

  PACKED struct PACKED_POST
  {
    uint8   version_major;
    uint8   version_minor;
    uint8   protection_type;
    uint8   simlock_state;
    uint8   imei[SIMLOCK_IMEI_MAX];
    uint64  time_stamp;
  } blob_header_data;

  PACKED struct PACKED_POST
  {
    uint64  start_time_ms;
    uint32  end_time_s;
  } blob_additional_header_data;

  if((in_data_ptr == NULL) ||
     (in_data_len == 0)    ||
     (out_config_data_ptr == NULL))
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  if(rsu_mode == SIMLOCK_RSU_MODE_C)
  {
    uint8                              lock_code   = 0;
    simlock_rsu_nonce_type             nonce_data  = {0};

    if(in_data_len != SIMLOCK_REMOTE_LOCK_LOCK_CODE_AND_SIGNATURE_SIZE ||
       in_data_ptr == NULL)
    {
      return SIMLOCK_INCORRECT_PARAMS;
    }

    simlock_status = simlock_rsu_key_read_nonce_file(nonce_data);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      return simlock_status;
    }

    lock_code = in_data_ptr[offset++];
    out_config_data_ptr->header_data.data.lock_code = lock_code;
    out_config_data_ptr->header_data.protection_type = SIMLOCK_RSU_CONFIG_PROTECTION_RSA;

    /* copy the rsa data */
     simlock_memscpy(out_config_data_ptr->auth_data.data.rsa,
                     sizeof(out_config_data_ptr->auth_data.data.rsa),
                     &in_data_ptr[offset],
                     sizeof(out_config_data_ptr->auth_data.data.rsa));

    /* the msg to autheniticate is the entire data except the rsa
       in the data */
    out_config_data_ptr->auth_data.msg_len = (in_data_len - SIMLOCK_RSA_LEN) + sizeof(nonce_data);

    /* caller is responsible to FREE this memory */
    SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->auth_data.msg_data_ptr,
                                 out_config_data_ptr->auth_data.msg_len);
    if(out_config_data_ptr->auth_data.msg_data_ptr == NULL)
    {
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    /* Copy the data without the trailing mac This msg data
        will be required in the RSA verification  Payload shall be
        [LOCK_CODE][NONCE] to generate digest */
    out_config_data_ptr->auth_data.msg_data_ptr[0] = lock_code;
    simlock_memscpy(out_config_data_ptr->auth_data.msg_data_ptr + 1,
                    out_config_data_ptr->auth_data.msg_len - 1,
                    nonce_data,
                    sizeof(nonce_data));
  }
  else if(rsu_mode == SIMLOCK_RSU_MODE_E || rsu_mode == SIMLOCK_RSU_MODE_F )
  {
    return simlock_rsu_config_parse_mode_e_f_data(in_data_len,
                                                  in_data_ptr,
                                                  out_config_data_ptr,
                                                  rsu_mode);
  }
  else
  {
    memset(&blob_header_data, 0x00, sizeof(blob_header_data));
    memset(&blob_additional_header_data, 0x00, sizeof(blob_additional_header_data));

    if(in_data_len > SIMLOCK_LOCK_OPERATION_INDEX_IN_REQUEST_BLOB)
    {
      /* Lets copy atleast the request lock_state as we need to fill in that info
         in the response blob even if the rest of the data parsing fails */
      simlock_status = simlock_rsu_config_parse_lock_state(in_data_ptr[SIMLOCK_LOCK_OPERATION_INDEX_IN_REQUEST_BLOB],
                                                           &out_config_data_ptr->header_data.data.lock_state,
                                                           rsu_mode);
      if(simlock_status != SIMLOCK_SUCCESS)
      {
        SIMLOCK_MSG_ERR_1("Failed to parse lock state 0x%x",
                          in_data_ptr[SIMLOCK_LOCK_OPERATION_INDEX_IN_REQUEST_BLOB]);
        return simlock_status;
      }
    }

    if(in_data_len < sizeof(blob_header_data))
    {
      SIMLOCK_MSG_ERR_1("Remote Lock config data length is less than blob header data, 0x%x",
                        in_data_len);
      return SIMLOCK_RSU_DATA_TOO_SHORT;
    }

    /* Copy minimum required header data from incoming data */
    simlock_memscpy((uint8 *)&blob_header_data,
                    sizeof(blob_header_data),
                                                      in_data_ptr,
                    sizeof(blob_header_data));

    out_config_data_ptr->header_data.version_major = blob_header_data.version_major;
    out_config_data_ptr->header_data.version_minor = blob_header_data.version_minor;

    simlock_status = simlock_rsu_config_parse_protection_type(blob_header_data.protection_type,
                                                              rsu_mode,
                                                              &out_config_data_ptr->header_data.protection_type);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("Failed to parse Remote Lock protection type 0x%x",
                         blob_header_data.protection_type);
      return simlock_status;
    }

    /* check the versions */
    if(rsu_mode == SIMLOCK_RSU_MODE_D)
    {
      if((out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1 &&
          out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION) == FALSE)
      {
        SIMLOCK_MSG_ERR_0("Remote Lock blob version check failed for SIMLOCK_RSU_MODE_D");
        return SIMLOCK_UNSUPPORTED;
      }

      /* Check minimum length of the blob for each major version */
      if(in_data_len < sizeof(blob_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_RSU_MODE_D_VERSION_1)
      {
        SIMLOCK_MSG_ERR_1("Remote Lock config data length is less than minimum size for SIMLOCK_RSU_MODE_D, 0x%x",
                          in_data_len);
        return SIMLOCK_RSU_DATA_TOO_SHORT;
      }
    }
    else
    {
    /* check the versions */
    if(((out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1 &&
         out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION) ||
        (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2 &&
         out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION) ||
        (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_3 &&
         out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION) ||
        (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_4 &&
         out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION) ||
        (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_5 &&
         out_config_data_ptr->header_data.version_minor == SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION)) == FALSE)
    {
      SIMLOCK_MSG_ERR_0("Remote Lock blob version check failed");
      return SIMLOCK_UNSUPPORTED;
    }

    /* Check minimum length of the blob for each major version */
    if((out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1 &&
        in_data_len < sizeof(blob_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_1) ||
       (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2 &&
        in_data_len < sizeof(blob_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_2_OR_3) ||
       (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_3 &&
      in_data_len < sizeof(blob_header_data) + sizeof(blob_additional_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_2_OR_3) ||
     (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_4 &&
      in_data_len < sizeof(blob_header_data) + sizeof(blob_additional_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_4_OR_5) ||
     (out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_5 &&
      in_data_len < sizeof(blob_header_data) + sizeof(blob_additional_header_data) + sizeof(blob_len) + sizeof(uint8) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_VERSION_4_OR_5))
    {
      SIMLOCK_MSG_ERR_1("Remote Lock config data length is less than minimum size, 0x%x",
                        in_data_len);
      return SIMLOCK_RSU_DATA_TOO_SHORT;
    }
    }

    offset = sizeof(blob_header_data);

    if(rsu_mode != SIMLOCK_RSU_MODE_D &&
       out_config_data_ptr->header_data.version_major >= SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_3)
    {
      /* Copy temporary unlock related start and end times from incoming data */
      simlock_memscpy((uint8 *)&blob_additional_header_data,
                      sizeof(blob_additional_header_data),
                      in_data_ptr + offset,
                      sizeof(blob_additional_header_data));

      offset += sizeof(blob_additional_header_data);

      out_config_data_ptr->code_data.start_time = (SIMLOCK_RSU_SWAP_BYTE_ORDER_64(blob_additional_header_data.start_time_ms) / 1000);
      out_config_data_ptr->code_data.end_time   = SIMLOCK_RSU_SWAP_BYTE_ORDER_32(blob_additional_header_data.end_time_s);
    }
    
	if(out_config_data_ptr->header_data.version_major >= SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_5)
    {
      /* slot_operation provides info on powering up/powering down of slots.
         First 5 bits is set as mentioned as below
         B1  0: disable slot1
             1: enable slot1 (if the slot exists)
         B2  0: disable slot2
             1: enable slot2 (if the slot exists)
         B3  0: disable slot3
             1: enable slot3 (if the slot exists)
         B4  0: disable slot4
             1: enable slot4 (if the slot exists)
         B5  0: configuration of active slots does not change and value in b1 to b4 is ignored.
             1: configuration of active slots indicated in b1 to b4 is valid. */
      if(in_data_ptr[offset] & 0x10)
      {
        uint8 slot_index = 0;
    
        out_config_data_ptr->code_data.disabled_slots_info_valid = TRUE;
    
        for (slot_index = 0; slot_index < SIMLOCK_SLOT_COUNT_MAX; slot_index++)
        {
          if(!(in_data_ptr[offset] & (0x01 << slot_index)))
          {
            out_config_data_ptr->code_data.disabled_slots.slot[slot_index] = TRUE;
          }
        }
      }
      else
      {
        out_config_data_ptr->code_data.disabled_slots_info_valid = FALSE;
      }
    
      offset += sizeof(uint8);
    }
    /* Copy blob length from incoming data (after swapping the bytes) */
    blob_len = (in_data_ptr[offset + 1] << 8) + in_data_ptr[offset];

    offset += sizeof(blob_len);

    simlock_status = simlock_rsu_config_parse_protection_type(blob_header_data.protection_type,
                                                              rsu_mode,
                                                                      &out_config_data_ptr->header_data.protection_type);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("Failed to parse Remote Lock protection type 0x%x",
                         blob_header_data.protection_type);
      return simlock_status;
    }

    /* Check minimum length of the blob when protection type is RSA */
    if(rsu_mode == SIMLOCK_RSU_MODE_D &&
       out_config_data_ptr->header_data.protection_type == SIMLOCK_RSU_CONFIG_PROTECTION_RSA &&
       in_data_len < sizeof(blob_header_data) + sizeof(blob_len) + SIMLOCK_REMOTE_LOCK_MIN_CODE_DATA_LEN_RSU_MODE_D_VERSION_1 + SIMLOCK_RSA_LEN)
    {
      SIMLOCK_MSG_ERR_1("Remote Lock config data length is less than minimum size for Mode D when protection type is RSA, 0x%x",
                        in_data_len);
      return SIMLOCK_RSU_DATA_TOO_SHORT;
    }

    out_config_data_ptr->imei.imei_len = SIMLOCK_IMEI_MAX;
    simlock_memscpy(out_config_data_ptr->imei.imei_data,
                    out_config_data_ptr->imei.imei_len,
                    blob_header_data.imei,
                    out_config_data_ptr->imei.imei_len);

    out_config_data_ptr->header_data.time_stamp = SIMLOCK_RSU_SWAP_BYTE_ORDER_64(blob_header_data.time_stamp);

    if((blob_len + offset) < in_data_len)
    {
      SIMLOCK_MSG_ERR_2("Remote Lock config data lengths inconsistent, Total len:0x%x, Data len: 0x%x",
                         in_data_len, blob_len);
      return SIMLOCK_RSU_DATA_TOO_SHORT;
    }

    /* Parse NW lock data */
    if(rsu_mode != SIMLOCK_RSU_MODE_D &&
       out_config_data_ptr->header_data.version_major >= SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2)
    {
      if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK)
      {
        simlock_status = simlock_rsu_config_populate_lock_info(in_data_ptr,
                                                               in_data_len,
                                                               SIMLOCK_CATEGORY_3GPP_NW,
                                                               offset,
                                                               out_config_data_ptr);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          return simlock_status;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_LOCK_DATA_LEN_VERSION_2_OR_3;
    }
    else if(out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1)
    {
      out_config_data_ptr->code_data.lock_code_data.nw_lock.change_flag = TRUE;
    }

    if(offset + SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH > in_data_len)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Consider below data when lock state is permanent lock and change flag is TRUE */
    if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK &&
       out_config_data_ptr->code_data.lock_code_data.nw_lock.change_flag)
    {
      out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes = in_data_ptr[offset];

      if(out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes > 0)
      {
        /* caller is responsible to FREE this memory */
        SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr,
                                     (out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes * sizeof(simlock_nw_code_data_type)));
        if(out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr == NULL)
        {
          return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      for(code_index = 0;
          code_index < out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_codes;
          code_index++)
      {
        if(offset + SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE > in_data_len)
        {
          return SIMLOCK_GENERIC_ERROR;
        }

        memset(&mcc_mnc_data, 0x00, sizeof(mcc_mnc_data));
        if(SIMLOCK_SUCCESS == simlock_rsu_config_convert_mcc_mnc(&in_data_ptr[offset],
                                                                 (in_data_len - offset),
                                                                 &mcc_mnc_data))
        {
          simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr[code_index],
                          sizeof(out_config_data_ptr->code_data.lock_code_data.nw_lock.nw_code_ptr[code_index]),
                          &mcc_mnc_data,
                          sizeof(mcc_mnc_data));
        }
        offset += SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE;
      }
    }
    else
    {
      uint8 nw_codes = 0x00;

      /* When the lock state is permanent unlock or change flag is set to 0 then
         move the offset to next category as there is no need to parse the data. */
      nw_codes = in_data_ptr[offset];
      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      if(offset + (nw_codes * SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE) > in_data_len)
      {
        return SIMLOCK_GENERIC_ERROR;
      }

      offset += (nw_codes * SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE);
    }

    /* Parse NS lock data */
    if(rsu_mode != SIMLOCK_RSU_MODE_D &&
       out_config_data_ptr->header_data.version_major >= SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2)
    {
      if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK)
      {
        simlock_status = simlock_rsu_config_populate_lock_info(in_data_ptr,
                                                               in_data_len,
                                                               SIMLOCK_CATEGORY_3GPP_NS,
                                                               offset,
                                                               out_config_data_ptr);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          return simlock_status;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_LOCK_DATA_LEN_VERSION_2_OR_3;
    }
    else if(out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1)
    {
      out_config_data_ptr->code_data.lock_code_data.ns_lock.change_flag = TRUE;
    }

    if(offset + SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH > in_data_len)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Consider below data when lock state is permanent lock and change flag is TRUE */
    if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK &&
       out_config_data_ptr->code_data.lock_code_data.ns_lock.change_flag)
    {
      out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_codes = in_data_ptr[offset];

      if(out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_codes > 0)
      {
        /* caller is responsible to FREE this memory */
        SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr,
                                     (out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_codes * sizeof(simlock_ns_code_data_type)));
        if(out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr == NULL)
        {
          return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      for(code_index = 0;
          code_index < out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_codes;
          code_index++)
      {
        if(offset + SIMLOCK_REMOTE_LOCK_NS_CODE_DATA_SIZE > in_data_len)
        {
          return SIMLOCK_GENERIC_ERROR;
        }

        memset(&mcc_mnc_data, 0x00, sizeof(mcc_mnc_data));
        if(SIMLOCK_SUCCESS == simlock_rsu_config_convert_mcc_mnc(&in_data_ptr[offset],
                                                                 (in_data_len - offset),
                                                                 &mcc_mnc_data))
        {
          simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr[code_index].nw_code,
                          sizeof(out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr[code_index].nw_code),
                          &mcc_mnc_data,
                          sizeof(mcc_mnc_data));
        }

        offset += SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE;

        /* Copy the IMSI 6th and 7th byte values */
        out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr[code_index].imsi_digit6 = in_data_ptr[offset++];
        out_config_data_ptr->code_data.lock_code_data.ns_lock.ns_code_ptr[code_index].imsi_digit7 = in_data_ptr[offset++];
      }
    }
    else
    {
      uint8 ns_codes = 0x00;

      /* When the lock state is permanent unlock or change flag is set to 0 then
         move the offset to next category as there is no need to parse the data. */
      ns_codes = in_data_ptr[offset];
      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      if(offset + (ns_codes * SIMLOCK_REMOTE_LOCK_NS_CODE_DATA_SIZE) > in_data_len)
      {
        return SIMLOCK_GENERIC_ERROR;
      }

      offset += (ns_codes * SIMLOCK_REMOTE_LOCK_NS_CODE_DATA_SIZE);
    }

    /* Parse SP lock data */
    if(rsu_mode != SIMLOCK_RSU_MODE_D &&
       out_config_data_ptr->header_data.version_major >= SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2)
    {
      if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK)
      {
        simlock_status = simlock_rsu_config_populate_lock_info(in_data_ptr,
                                                               in_data_len,
                                                               SIMLOCK_CATEGORY_3GPP_SP,
                                                               offset,
                                                               out_config_data_ptr);
        if(simlock_status != SIMLOCK_SUCCESS)
        {
          return simlock_status;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_LOCK_DATA_LEN_VERSION_2_OR_3;
    }
    else if(out_config_data_ptr->header_data.version_major == SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1)
    {
      out_config_data_ptr->code_data.lock_code_data.sp_lock.change_flag = TRUE;
    }

    if (offset + SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH > in_data_len)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    /* Consider below data when lock state is permanent lock and change flag is TRUE */
    if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK &&
       out_config_data_ptr->code_data.lock_code_data.sp_lock.change_flag)
    {
      out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes = in_data_ptr[offset];

      if(out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes > 0)
      {
        /* caller is responsible to FREE this memory */
        SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr,
                                     (out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes * sizeof(simlock_sp_code_data_type)));
        if(out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr == NULL)
        {
          return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
        }
      }

      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      for(code_index = 0;
          code_index < out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_codes;
          code_index++)
      {
        if(offset + SIMLOCK_REMOTE_LOCK_SP_CODE_DATA_SIZE > in_data_len)
        {
          return SIMLOCK_GENERIC_ERROR;
        }

        memset(&mcc_mnc_data, 0x00, sizeof(mcc_mnc_data));
        if(SIMLOCK_SUCCESS == simlock_rsu_config_convert_mcc_mnc(&in_data_ptr[offset],
                                                                 (in_data_len - offset),
                                                                 &mcc_mnc_data))
        {
          simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[code_index].nw_code,
                          sizeof(out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[code_index].nw_code),
                          &mcc_mnc_data,
                          sizeof(mcc_mnc_data));
        }

        offset += SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE;

        /* Copy the GID value */
        out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[code_index].gid1.gid_len = 1;
        out_config_data_ptr->code_data.lock_code_data.sp_lock.sp_code_ptr[code_index].gid1.gid_data[0] = in_data_ptr[offset++];
      }
    }
    else
    {
      uint8 sp_codes = 0x00;

      /* When the lock state is permanent unlock or change flag is set to 0 then
         move the offset to next category as there is no need to parse the data. */
      sp_codes = in_data_ptr[offset];
      offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

      if(offset + (sp_codes * SIMLOCK_REMOTE_LOCK_SP_CODE_DATA_SIZE) > in_data_len)
      {
        return SIMLOCK_GENERIC_ERROR;
      }

      offset += (sp_codes * SIMLOCK_REMOTE_LOCK_SP_CODE_DATA_SIZE);
    }
    
    if(rsu_mode == SIMLOCK_RSU_MODE_D)
    {
      out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.change_flag = TRUE;

      /* Parse SP-EHPLMN lock data */
      if (offset + SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH > in_data_len)
      {
        return SIMLOCK_GENERIC_ERROR;
      }

      /* Consider below data when lock state is permanent lock and change flag is TRUE */
      if(out_config_data_ptr->header_data.data.lock_state == SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK)
      {
        out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_codes = in_data_ptr[offset];

        if(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_codes > 0)
        {
          /* caller is responsible to FREE this memory */
          SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr,
                                       (out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_codes * sizeof(simlock_sp_ehplmn_code_data_type)));
          if(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr == NULL)
          {
            return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
          }
        }

        offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

        for(code_index = 0;
            code_index < out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_codes;
            code_index++)
        {
          if(offset + SIMLOCK_REMOTE_LOCK_SP_EHPLMN_CODE_DATA_SIZE > in_data_len)
          {
            return SIMLOCK_GENERIC_ERROR;
          }

          memset(&mcc_mnc_data, 0x00, sizeof(mcc_mnc_data));
          if(SIMLOCK_SUCCESS == simlock_rsu_config_convert_mcc_mnc(&in_data_ptr[offset],
                                                                   (in_data_len - offset),
                                                                   &mcc_mnc_data))
          {
            simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].nw_code,
                            sizeof(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].nw_code),
                            &mcc_mnc_data,
                            sizeof(mcc_mnc_data));
          }

          offset += SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE;

          /* Copy the GID value */
          out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].gid1.gid_len = in_data_ptr[offset++];
          simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].gid1.gid_data[0],
                          sizeof(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].gid1.gid_data),
                          &in_data_ptr[offset],
                          out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].gid1.gid_len);

          offset += SIMLOCK_MAX_GID_LEN;

          memset(&mcc_mnc_data, 0x00, sizeof(mcc_mnc_data));
          if(SIMLOCK_SUCCESS == simlock_rsu_config_convert_mcc_mnc(&in_data_ptr[offset],
                                                                   (in_data_len - offset),
                                                                   &mcc_mnc_data))
          {
            simlock_memscpy(&out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].ehplmn_nw_code,
                            sizeof(out_config_data_ptr->code_data.lock_code_data.sp_ehplmn_lock.sp_ehplmn_code_ptr[code_index].ehplmn_nw_code),
                            &mcc_mnc_data,
                            sizeof(mcc_mnc_data));
          }

          offset += SIMLOCK_REMOTE_LOCK_NW_CODE_DATA_SIZE;
        }
      }
      else
      {
        uint8 sp_ehplmn_codes = 0x00;

        /* When the lock state is permanent unlock or change flag is set to 0 then
           move the offset to next category as there is no need to parse the data. */
        sp_ehplmn_codes = in_data_ptr[offset];
        offset += SIMLOCK_REMOTE_LOCK_NUM_CODES_LENGTH;

        if(offset + (sp_ehplmn_codes * SIMLOCK_REMOTE_LOCK_SP_EHPLMN_CODE_DATA_SIZE) > in_data_len)
        {
          return SIMLOCK_GENERIC_ERROR;
        }

        offset += (sp_ehplmn_codes * SIMLOCK_REMOTE_LOCK_SP_EHPLMN_CODE_DATA_SIZE);
      }
    }

    if(out_config_data_ptr->header_data.protection_type == SIMLOCK_RSU_CONFIG_PROTECTION_RSA)
    {
      if(offset + sizeof(out_config_data_ptr->auth_data.data.rsa) > in_data_len)
      {
        return SIMLOCK_GENERIC_ERROR;
      }

     /* copy the rsa data */
      simlock_memscpy(out_config_data_ptr->auth_data.data.rsa,
                      sizeof(out_config_data_ptr->auth_data.data.rsa),
                      &in_data_ptr[offset],
                      sizeof(out_config_data_ptr->auth_data.data.rsa));

      /* the msg to autheniticate is the entire data except the rsa
         in the data */
      out_config_data_ptr->auth_data.msg_len = (in_data_len - SIMLOCK_RSA_LEN);

      /* caller is responsible to FREE this memory */
      SIMLOCK_CHECK_AND_MEM_MALLOC(out_config_data_ptr->auth_data.msg_data_ptr,
                                   out_config_data_ptr->auth_data.msg_len);
      if(out_config_data_ptr->auth_data.msg_data_ptr == NULL)
      {
        return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
      }

      /* copy the data without the trailing mac This msg data
         will be required in the RSA verification */
      simlock_memscpy(out_config_data_ptr->auth_data.msg_data_ptr,
                      out_config_data_ptr->auth_data.msg_len,
                      in_data_ptr,
                      out_config_data_ptr->auth_data.msg_len);
    }
  }
  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_parse_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_READ_TIME_FILE

DESCRIPTION
  This function reads the remote lock time file and provides the
  last times data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_config_read_time_file
(
  simlock_remote_lock_timestamp_type    * time_data_ptr
)
{
  simlock_result_enum_type    simlock_status     = SIMLOCK_SUCCESS;
  uint32                      time_file_size     = 0;
  uint8                     * time_file_data_ptr = NULL;

  if(time_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  do
  {
    simlock_status = simlock_file_read(&time_file_data_ptr,
                                       &time_file_size,
                                       SIMLOCK_FILE_REMOTE_BLOB_TIME,
                                       SIMLOCK_SFS_LOCAL,
                                       SIMLOCK_CONFIG_ID_DEFAULT);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (time_file_data_ptr == NULL))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

     /* if the file size is not of the expected size,
       then it is not valid */
    if(time_file_size != sizeof(simlock_remote_lock_timestamp_type))
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_memscpy((uint8 *)time_data_ptr,
                    sizeof(simlock_remote_lock_timestamp_type),
                    time_file_data_ptr,
                    time_file_size);

    /* File version check */
    if(time_data_ptr->version != SIMLOCK_REMOTE_LOCK_TIME_FILE_VERSION)
    {
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }
  }while (0);

  SIMLOCK_MEM_FREE(time_file_data_ptr);
  return simlock_status;
} /* simlock_rsu_config_read_time_file */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_VALIDATE_AND_STORE_TIMESTAMP

DESCRIPTION
  This function validates the timestamp contained in the blob request
  If it has a valid time stamp, it writes to the sfs

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_validate_and_store_timestamp(
  uint64 timestamp
)
{
  simlock_remote_lock_timestamp_type  time_data;
  simlock_result_enum_type            simlock_status = SIMLOCK_SUCCESS;

  memset(&time_data, 0x00, sizeof(time_data));

  simlock_status = simlock_rsu_config_read_time_file(&time_data);

  /* If the read fails, it would be the first request case */
  if((simlock_status != SIMLOCK_SUCCESS) ||
     (time_data.timestamp < timestamp))
  {
    /* Create necessary directories before writing file */
    simlock_config_create_simlock_dir_if_needed();

    time_data.version = SIMLOCK_REMOTE_LOCK_TIME_FILE_VERSION;
    time_data.timestamp = timestamp;

    simlock_status =  simlock_file_write((uint8 *)&time_data,
                                         sizeof(simlock_remote_lock_timestamp_type),
                                         SIMLOCK_FILE_REMOTE_BLOB_TIME,
                                         SIMLOCK_SFS_LOCAL,
                                         SIMLOCK_CONFIG_ID_DEFAULT);
  }
  else
  {
    SIMLOCK_MSG_MED_2("Last time stamp is 0x%x 0x%x",
                       time_data.timestamp >> 32 & 0xFFFFFFFF,
                       time_data.timestamp & 0xFFFFFFFF);
    simlock_status = SIMLOCK_RSU_STALE_DATA;
  }

  return simlock_status;
}/* simlock_rsu_config_validate_and_store_timestamp */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_VALIDATE_BLOB

DESCRIPTION
  This function validates the simlock blob received

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type  simlock_rsu_config_validate_blob(
  simlock_rsu_config_data_type  *config_data_ptr,
  simlock_rsu_mode_enum_type     rsu_mode
)
{
  simlock_result_enum_type   simlock_status = SIMLOCK_GENERIC_ERROR;

  if (config_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* For commercial deployment, only the protection type SHA256 is supported.
     This can be supported also in test environment */
  /* verify the integrity of the data */
  simlock_status = simlock_rsu_config_verify_data_integrity(config_data_ptr,
                                                            rsu_mode);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_0("data integrity check failed");
    if(rsu_mode == SIMLOCK_RSU_MODE_C)
    {
      simlock_filename_type        file_name;

      memset(file_name, 0x00, sizeof(file_name));

      /* RSA Signature is not valid. Delete the stored NONCE */
      (void)simlock_util_get_file_path_from_enum(SIMLOCK_FILE_RSU_NONCE,
                                                 file_name);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#else
      (void)simlock_sfs_delete_file(file_name);
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
    }

    return simlock_status;
  }

  if(rsu_mode != SIMLOCK_RSU_MODE_C)
  {
    /* check the IMEI */
    simlock_status = simlock_rsu_config_check_imei(config_data_ptr->imei);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("imei check failed");
      return simlock_status;
    }

    if(rsu_mode != SIMLOCK_RSU_MODE_E && rsu_mode != SIMLOCK_RSU_MODE_F)
    {
    /* Validate and store the time stamp */
      simlock_status = simlock_rsu_config_validate_and_store_timestamp(config_data_ptr->header_data.time_stamp);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      return simlock_status;
    }
  }
  }

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_validate_blob */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_CREATE_RESPOSE_BLOB

DESCRIPTION
  This function creates a rseponse blob containing info needed by the server
  that initiated the RSU blob

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_config_create_response_blob
(
  simlock_result_enum_type                  status,
  simlock_rsu_config_lock_state_enum_type   lock_state,
  uint64                                    time_stamp,
  simlock_data_type                       * resp_data_ptr,
  simlock_rsu_mode_enum_type                rsu_mode
)
{
  uint8                                  offset     = 0;
  simlock_data_type                      hmac_data;

  if(resp_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(&hmac_data, 0x00, sizeof(hmac_data));

  if(rsu_mode == SIMLOCK_RSU_MODE_D)
  {
    simlock_imei_type       device_imei;

    memset(&device_imei, 0x00, sizeof(simlock_imei_type));

    if(simlock_util_get_imei(SIMLOCK_SLOT_1, &device_imei) != SIMLOCK_SUCCESS ||
       device_imei.imei_len == 0 || device_imei.imei_len > SIMLOCK_IMEI_MAX)
    {
      return SIMLOCK_GENERIC_ERROR;
    }

    SIMLOCK_CHECK_AND_MEM_MALLOC(resp_data_ptr->data_ptr,
                                 SIMLOCK_REMOTE_LOCK_RESPONSE_RSU_MODE_D_BLOB_LEN_VERSION_1);
    if(resp_data_ptr->data_ptr == NULL)
    {
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }

    resp_data_ptr->data_len = SIMLOCK_REMOTE_LOCK_RESPONSE_RSU_MODE_D_BLOB_LEN_VERSION_1;

    /* Type of message: 2 - SimLock blob response  */
    resp_data_ptr->data_ptr[offset++] = 2;

    /* IMEI */
    simlock_memscpy(&(resp_data_ptr->data_ptr[offset]),
                    SIMLOCK_REMOTE_LOCK_RESPONSE_RSU_MODE_D_BLOB_LEN_VERSION_1 - offset,
                    device_imei.imei_data,
                    device_imei.imei_len);
    offset = offset + SIMLOCK_IMEI_MAX;
  }
  else
  {
    SIMLOCK_CHECK_AND_MEM_MALLOC(resp_data_ptr->data_ptr,
                                 SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_LEN_VERSION_1);
    if(resp_data_ptr->data_ptr == NULL)
    {
      return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
    }
    resp_data_ptr->data_len = SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_LEN_VERSION_1;
  }

  resp_data_ptr->data_ptr[offset++] = SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_MAJOR_VERSION_1;
  resp_data_ptr->data_ptr[offset++] = SIMLOCK_REMOTE_LOCK_RESPONSE_BLOB_MINOR_VERSION_1;

  /* lock operation */
  switch(lock_state)
  {
    case SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK:
      resp_data_ptr->data_ptr[offset++] = 0x00;
      break;

    case SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_UNLOCK:
      resp_data_ptr->data_ptr[offset++] = 0x01;
      break;

    case SIMLOCK_RSU_CONFIG_LOCK_STATE_TEMPORARY_UNLOCK:
      resp_data_ptr->data_ptr[offset++] = 0x02;
      break;

    default:
      /* If the status passed into this function is SIMLOCK_SUCCESS, overwrite
         it with ERROR in order to indicate to the server that there was some
         problem */
      SIMLOCK_MSG_ERR_1("Invalid lock state : 0x%x", lock_state);
      if(SIMLOCK_SUCCESS == status)
      {
        status = SIMLOCK_GENERIC_ERROR;
      }
      break;
  }

  /* rsu operation error/status code */
  switch(status)
  {
    case SIMLOCK_SUCCESS:
      resp_data_ptr->data_ptr[offset++] = 0x00;
      break;

    case SIMLOCK_RSU_DATA_TOO_SHORT:
      resp_data_ptr->data_ptr[offset++] = 0x02;
      break;

    case SIMLOCK_RSU_DATA_VALIDATION_FAILED:
      resp_data_ptr->data_ptr[offset++] = 0x03;
      break;

    case SIMLOCK_RSU_GET_TIME_FAILED:
      if(rsu_mode == SIMLOCK_RSU_MODE_D)
      {
        resp_data_ptr->data_ptr[offset++] = 0x01;
      }
      else
      {
        resp_data_ptr->data_ptr[offset++] = 0x04;
      }
      break;

    case SIMLOCK_RSU_IMEI_VALIDATION_FAILED:
      resp_data_ptr->data_ptr[offset++] = 0x05;
      break;

    case SIMLOCK_RSU_STALE_DATA:
      resp_data_ptr->data_ptr[offset++] = 0x06;
      break;

    case SIMLOCK_GENERIC_ERROR:
    default:
      resp_data_ptr->data_ptr[offset++] = 0x01;
      break;
  }

  /* request timestamp */
  time_stamp = simlock_util_htonll(time_stamp);
  simlock_memscpy(&(resp_data_ptr->data_ptr[offset]),
                  resp_data_ptr->data_len - offset,
                  &time_stamp,
                  sizeof(time_stamp));
  offset = offset + sizeof(time_stamp);

 #ifndef FEATURE_UIM_TEST_FRAMEWORK
  /* hmac on the above data */
  if(SIMLOCK_SUCCESS != simlock_rsu_process_generate_hash(offset,
                                                          resp_data_ptr->data_ptr,
                                                          &hmac_data) ||
     hmac_data.data_ptr == NULL)
  {
    /* could not generate HMAC on the response blob. Server will most likely
       reject the response as it won't be able to validate the HMAC */
    SIMLOCK_MSG_ERR_0("Failed to generate HMAC on the response blob data");
    SIMLOCK_MEM_FREE(resp_data_ptr->data_ptr);
    return SIMLOCK_GENERIC_ERROR;
  }
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  simlock_memscpy(&(resp_data_ptr->data_ptr[offset]),
                  resp_data_ptr->data_len - offset,
            hmac_data.data_ptr,
            hmac_data.data_len);

  SIMLOCK_MEM_FREE(hmac_data.data_ptr);

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_config_create_response_blob */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_SET_DATA

DESCRIPTION
  This function sets the simlock configuration data received in the request

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_config_set_data
(
  uint32                       raw_data_len,
  const uint8                * raw_data_ptr,
  simlock_data_type          * resp_data_ptr,
  simlock_rsu_mode_enum_type   rsu_mode
)
{
  simlock_result_enum_type             simlock_status              = SIMLOCK_SUCCESS;
  simlock_rsu_config_data_type       * decoded_rsu_config_data_ptr = NULL;

  if(raw_data_len == 0 ||
     raw_data_ptr == NULL ||
     resp_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(decoded_rsu_config_data_ptr,
                               sizeof(simlock_rsu_config_data_type));
  if(decoded_rsu_config_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  do
  {
    /* parse the data */
    simlock_status = simlock_rsu_config_parse_data(raw_data_len,
                                                   raw_data_ptr,
                                                   decoded_rsu_config_data_ptr,
                                                   rsu_mode);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("parse config data failed");
      break;
    }

    /* validate the blob request */
    simlock_status = simlock_rsu_config_validate_blob(decoded_rsu_config_data_ptr,
                                                      rsu_mode);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      break;
    }

    /* delete the simlock delay timer as SimLock engine
       received blob from unlock server */
    simlock_delay_delete_delay_timer();

    /* apply the configuration */
    simlock_status = simlock_rsu_config_apply_configuration(decoded_rsu_config_data_ptr,
                                                            rsu_mode);
  } while(0);

  if(rsu_mode == SIMLOCK_RSU_MODE_A ||
     rsu_mode == SIMLOCK_RSU_MODE_D)
  {
    /* Construct the response blob for RSU mode RSU_A & RSU_D.
       In case of RSU_A & RSU_D, the user application expects to receive an entire
       response blob even in case of erroneous processing of request blob.
       Incase of an error in the processing/validation of request blob above,
       if we return the error simlock_status to QMIUIM (and hence to the RSU
       service) along with the response blob, the RSU service ignores the
       response blob. So, in order to ensure that the RSU service is always
       able to take the response blob into account and pass it on to the user
       application, we must ensure to send a SIMLOCK_SUCCESS for the
       set_configuration request. Any error during the processing/validation
       of request blob is anyways captured in a field of the response blob. */
    simlock_status = simlock_rsu_config_create_response_blob(
                       simlock_status,
                       decoded_rsu_config_data_ptr->header_data.data.lock_state,
                       decoded_rsu_config_data_ptr->header_data.time_stamp,
                       resp_data_ptr,
                       rsu_mode);
  }

  simlock_rsu_config_free_code_data(decoded_rsu_config_data_ptr);

  return simlock_status;
} /* simlock_rsu_config_set_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_CONFIG_IS_LOCK_ALLOWED

DESCRIPTION
  This function checks if the lock request is to be allowed or not.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_rsu_config_is_lock_allowed
(
  const simlock_message_request_data_type *req_data_ptr
)
{
  boolean                     is_allowed           = FALSE;
  simlock_config_data_type   *config_file_data_ptr = NULL;
  simlock_rsu_mode_enum_type  rsu_mode             = SIMLOCK_RSU_MODE_NO_RSU;

  if(req_data_ptr == NULL)
  {
    return FALSE;
  }

  /* Read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    /* Config read failed means no RSU enabled. so returning TRUE to allow native
       SimLock commands */
    return TRUE;
  }

  /* Retrieve RSU mode */
  rsu_mode = config_file_data_ptr->rsu_mode;

  /* Free configuration */
  simlock_config_free_config_data(config_file_data_ptr);
  config_file_data_ptr = NULL;

  if (rsu_mode == SIMLOCK_RSU_MODE_NO_RSU)
  {
    return TRUE;
  }

  switch(req_data_ptr->msg_type)
  {
    case SIMLOCK_SET_LOCK_CK_MSG:
      /* Reject the request if following conditions are satisfied :
      1) if request have blacklist
      2) if request have auto lock enabled
      3) We allow Ck len > 0 only in SIMLOCK_RSU_MODE_B.*/
      if(req_data_ptr->message.lock_ck_msg.blacklist ||
         req_data_ptr->message.lock_ck_msg.auto_lock ||
         (rsu_mode != SIMLOCK_RSU_MODE_B &&
          req_data_ptr->message.lock_ck_msg.ck.simlock_ck_length > 0))
      {
        return FALSE;
      }

      switch(req_data_ptr->message.lock_ck_msg.category.category_type)
      {
        case SIMLOCK_CATEGORY_3GPP_NW:
        case SIMLOCK_CATEGORY_3GPP_NS:
        case SIMLOCK_CATEGORY_3GPP_SP:
        case SIMLOCK_CATEGORY_3GPP_SP_EHPLMN:
          is_allowed = TRUE;
          break;

        default:
          is_allowed = FALSE;
          break;
      }
      break;

    case SIMLOCK_ADD_LOCK_CODES_CK_MSG:
      if(rsu_mode == SIMLOCK_RSU_MODE_C)
      {
        if(req_data_ptr->message.lock_ck_msg.ck.simlock_ck_length > 0 ||
           req_data_ptr->message.lock_ck_msg.auto_lock)
        {
          is_allowed = FALSE;
        }
        else
        {
          is_allowed = TRUE;
        }
      }
      break;

    /* For HCK Lock and HCK Add Lock, Lock request is allowed only for mode B */
    case SIMLOCK_SET_LOCK_HCK_MSG:
    case SIMLOCK_ADD_LOCK_CODES_HCK_MSG:
      if(rsu_mode == SIMLOCK_RSU_MODE_B)
      {
        is_allowed = TRUE;
      }
      else
      {
        is_allowed = FALSE;
      }
      break;
    /* For Relock messages Lock request is not allowed */
    case SIMLOCK_RELOCK_DEVICE_MSG:
      is_allowed = FALSE;
      break;

    /* For any other messages, the request should be allowed e.g. Get Status msg, Run algo etc. */
    default:
      is_allowed = TRUE;
      break;
  }

  SIMLOCK_MSG_MED_2("simlock_rsu_config_is_lock_allowed is_allowed 0x%x msg 0x%x", is_allowed, req_data_ptr->msg_type);
  return is_allowed;
} /* simlock_rsu_config_is_lock_allowed */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */
