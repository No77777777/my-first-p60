/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   R S U   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock RSU functions.


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

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_rsu.c#3 $$ $DateTime: 2020/06/07 15:57:55 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
05/06/20   cj      Remote SIM Unlock support for simlock rsu mode E & F
09/10/19   vdc     SimLock solution for activation at Point Of Sale
04/25/19   mm      Updated major and minor version for rsu mode A
12/03/18   vgd     Added support for public keys for multiple RSU modes
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
07/24/17   nr      Reduce stack consumption
07/13/17   nr      Remote SIM Unlock supported in common build
07/13/17   av      Enhanced simlock RSU solution
12/02/16   vdc     Added support to check RSA public key for remote lock
01/07/16   stv     Remote simlock support
06/16/15   stv     Fix to perform efs sync operation for critical file updates
05/15/15   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock.h"
#include "simlock_file.h"
#include "simlock_efs.h"
#include "simlock_rsu.h"
#include "simlock_rsu_key.h"
#include "simlock_rsu_config.h"
#include "simlock_modem.h"
#include "simlock_delay.h"
#include "amssassert.h"

/*=============================================================================

                   DATA DECLARATIONS

=============================================================================*/
#define SIMLOCK_RSU_MODE_D_BLOB_REQUEST_MAJOR_VERSION           1
#define SIMLOCK_RSU_MODE_D_BLOB_REQUEST_MINOR_VERSION           0
#define SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1   60

typedef  uint64  simlock_rsu_session_id_type;

/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_GENERATE_HASH

DESCRIPTION
  This function generates hmac on the data that is passed in the request

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_process_generate_hash
(
  uint16                 raw_data_len,
  const uint8           *raw_data_ptr,
  simlock_data_type     *hmac_ptr
)
{
  simlock_rsu_device_key_type     device_key_data_buf;
  simlock_hmac_signature_type     hmac_data_buf;
  simlock_result_enum_type        simlock_status   = SIMLOCK_SUCCESS;

  if(raw_data_len == 0 ||
     raw_data_ptr == NULL ||
     hmac_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(hmac_ptr, 0x00, sizeof(simlock_data_type));
  memset(device_key_data_buf, 0x00, SIMLOCK_RSU_DEVICE_KEY_LEN);
  memset(hmac_data_buf, 0x00, SIMLOCK_HMAC_SHA256_LEN);

  /* read the device key from file */
  simlock_status = simlock_rsu_key_read_device_key_file(device_key_data_buf);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  /* generate the hmac */
  simlock_status = simlock_crypto_generate_hmac_sha256((uint8 *)raw_data_ptr,
                                                       raw_data_len,
                                                       device_key_data_buf,
                                                       SIMLOCK_RSU_DEVICE_KEY_LEN,
                                                       hmac_data_buf);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(hmac_ptr->data_ptr, sizeof(hmac_data_buf));
  if(hmac_ptr->data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }
  hmac_ptr->data_len = sizeof(hmac_data_buf);

  simlock_memscpy(hmac_ptr->data_ptr,
                  hmac_ptr->data_len,
                  hmac_data_buf,
                  sizeof(hmac_data_buf));

  return simlock_status;
} /* simlock_rsu_process_generate_hash */


/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_RETRIEVE_VERSION

DESCRIPTION
  This function retrieves the highest version of blob supported by RSU engine

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_process_retrieve_version
(
  simlock_rsu_version_type     *version_ptr,
  simlock_rsu_mode_enum_type    rsu_mode
)
{
  if(version_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  switch(rsu_mode)
  {
    case SIMLOCK_RSU_MODE_A:
      version_ptr->major = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_A_MAJOR_VERSION_MAX;
      version_ptr->minor = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_A_MINOR_VERSION_MAX;
      break;

    case SIMLOCK_RSU_MODE_B:
    version_ptr->major = SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_MAX;
    version_ptr->minor = SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION_MAX;
      break;

    case SIMLOCK_RSU_MODE_C:
    case SIMLOCK_RSU_MODE_D:
    version_ptr->major = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_D_MAJOR_VERSION_MAX;
    version_ptr->minor = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_D_MINOR_VERSION_MAX;
      break;

    case SIMLOCK_RSU_MODE_E:
      version_ptr->major = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MAJOR_VERSION_MAX;
      version_ptr->minor = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MINOR_VERSION_MAX;
      break;

    case SIMLOCK_RSU_MODE_F:
      version_ptr->major = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MAJOR_VERSION_MAX;
      version_ptr->minor = SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MINOR_VERSION_MAX;
      break;

    default:
      return SIMLOCK_UNSUPPORTED;
  }

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_process_retrieve_version */


/*===========================================================================
FUNCTION SIMLOCK_RSU_GENERATE_SESSION_ID

DESCRIPTION
  This function generates session ID (Monotonically increasing value).

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_rsu_session_id_type

  Returns 0 in case of error.

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_rsu_session_id_type simlock_rsu_generate_session_id
(
  void
)
{
  simlock_result_enum_type     simlock_status     = SIMLOCK_GENERIC_ERROR;
  uint32                       rsu_session_id_len = 0;
  simlock_rsu_session_id_type  rsu_session_id     = 0;

  /* Check for file presence. If file is not found it means that it is the first time */
  simlock_status = simlock_file_check(SIMLOCK_FILE_RSU_SESSION_ID, &rsu_session_id_len, SIMLOCK_CONFIG_ID_DEFAULT);
  if(simlock_status == SIMLOCK_SUCCESS)
  {
    uint8 *rsu_session_id_ptr = NULL;

    if(rsu_session_id_len != sizeof(simlock_rsu_session_id_type))
    {
      /* File got corrupted */
      return 0;
    }

    /* read the session_id data  This read should be always successful */
    simlock_status = simlock_file_read(&rsu_session_id_ptr,
                                       &rsu_session_id_len,
                                       SIMLOCK_FILE_RSU_SESSION_ID,
                                       SIMLOCK_SFS_LOCAL,
                                       SIMLOCK_CONFIG_ID_DEFAULT);
    if(simlock_status != SIMLOCK_SUCCESS ||
       rsu_session_id_len != sizeof(simlock_rsu_session_id_type))
    {
      SIMLOCK_MSG_ERR_1("read rsu_session_id file failed, status: 0x%x",
                        simlock_status);
      return 0;
    }

    simlock_memscpy((void *)&rsu_session_id,
                    sizeof(simlock_rsu_session_id_type),
                    rsu_session_id_ptr,
                    rsu_session_id_len);

    SIMLOCK_MEM_FREE(rsu_session_id_ptr);
  }

  rsu_session_id++;
  if(rsu_session_id == 0)
  {
    /* Returning error incase if it is round off to zero as the session_id
       is expected to be monotonically increasing value. */
    return 0;
  }

  /* write the session_id file data */
  simlock_status = simlock_file_write((uint8 *)&rsu_session_id,
                                      sizeof(simlock_rsu_session_id_type),
                                      SIMLOCK_FILE_RSU_SESSION_ID,
                                      SIMLOCK_SFS_LOCAL,
                                      SIMLOCK_CONFIG_ID_DEFAULT);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("write rsu_session_id file failed, status: 0x%x",
                      simlock_status);
    return 0;
  }

  return rsu_session_id;
} /* simlock_rsu_generate_session_id */


/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_BLOB_REQUEST

DESCRIPTION
  This function generates blob request requested by simlock agent/unlock server

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_process_blob_request
(
  simlock_data_type     *blob_request_ptr
)
{
  uint8                        offset          = 0;
  simlock_result_enum_type     simlock_status  = SIMLOCK_SUCCESS;
  simlock_data_type            hmac_data       = {0, NULL};
  simlock_imei_type            device_imei     = {0};
  simlock_rsu_session_id_type  rsu_session_id  = 0;
  simlock_rsu_version_type     rsu_version     = {0};

  if(blob_request_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* IMEI */
  if(simlock_util_get_imei(SIMLOCK_SLOT_1, &device_imei) != SIMLOCK_SUCCESS ||
     device_imei.imei_len == 0 ||
     device_imei.imei_len > SIMLOCK_IMEI_MAX)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  rsu_session_id = simlock_rsu_generate_session_id();
  if(rsu_session_id == 0)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  simlock_status = simlock_rsu_process_retrieve_version(&rsu_version, SIMLOCK_RSU_MODE_D);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(blob_request_ptr->data_ptr,
                               SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  if(blob_request_ptr->data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  blob_request_ptr->data_len = SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1;

  /* Message type */
  ASSERT(offset + 1 <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  blob_request_ptr->data_ptr[offset++] = 1;

  ASSERT(offset + SIMLOCK_IMEI_MAX <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  simlock_memscpy(&blob_request_ptr->data_ptr[offset],
                  SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1 - offset,
                  device_imei.imei_data,
                  device_imei.imei_len);
  offset = offset + SIMLOCK_IMEI_MAX ;

  /* blob request major and minor version */
  ASSERT(offset + 1 <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  blob_request_ptr->data_ptr[offset++] = SIMLOCK_RSU_MODE_D_BLOB_REQUEST_MAJOR_VERSION ;

  ASSERT(offset + 1 <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  blob_request_ptr->data_ptr[offset++] = SIMLOCK_RSU_MODE_D_BLOB_REQUEST_MINOR_VERSION;

  /* Session ID */
  ASSERT(offset + sizeof(rsu_session_id) <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  rsu_session_id = simlock_util_htonll(rsu_session_id);
  simlock_memscpy(&(blob_request_ptr->data_ptr[offset]),
                  SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1 - offset,
                  &rsu_session_id,
                  sizeof(rsu_session_id));
  offset = offset + sizeof(rsu_session_id);

  /* Supported Highest Major and Minor blob version */
  ASSERT(offset + 1 <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  blob_request_ptr->data_ptr[offset++] = rsu_version.major;

  ASSERT(offset + 1 <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  blob_request_ptr->data_ptr[offset++] = rsu_version.minor;

  /* hmac on the above data */
  if(SIMLOCK_SUCCESS != simlock_rsu_process_generate_hash(offset,
                                                          blob_request_ptr->data_ptr,
                                                          &hmac_data) ||
     hmac_data.data_ptr == NULL)
  {
    /* could not generate HMAC on the response blob. Server will most likely
       reject the response as it won't be able to validate the HMAC */
    SIMLOCK_MSG_ERR_0("Failed to generate HMAC on the response blob data");
    SIMLOCK_MEM_FREE(blob_request_ptr->data_ptr);
    return SIMLOCK_GENERIC_ERROR;
  }

  ASSERT(offset + SIMLOCK_HMAC_SHA256_LEN <= SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1);
  simlock_memscpy(&(blob_request_ptr->data_ptr[offset]),
                  SIMLOCK_REMOTE_LOCK_RSU_MODE_D_BLOB_REQ_LEN_VERSION_1 - offset,
                  hmac_data.data_ptr,
                  hmac_data.data_len);

  SIMLOCK_MEM_FREE(hmac_data.data_ptr);

  return SIMLOCK_SUCCESS;
} /* simlock_rsu_process_blob_request */


/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_START_DELAY_TIMER_REQUEST

DESCRIPTION
  This function process the start delay timer request received from client.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_process_start_delay_timer_request
(
  uint16    *delay_timer_value_in_sec_ptr
)
{
  return simlock_delay_initialize_delay_timer(delay_timer_value_in_sec_ptr);
} /* simlock_rsu_process_start_delay_timer_request */


/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_STOP_DELAY_TIMER_REQUEST

DESCRIPTION
  This function process the stop delay timer request received from client.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_process_stop_delay_timer_request
(
  void
)
{
  if(simlock_delay_is_delay_timer_running())
  {
    simlock_delay_stop_delay_timer();
    return SIMLOCK_SUCCESS;
  }

  return SIMLOCK_INVALID_STATE;
} /* simlock_rsu_process_stop_delay_timer_request */


/*===========================================================================
FUNCTION SIMLOCK_RSU_PROCESS_MESSAGE

DESCRIPTION
  This is the function which processes the RSU requests in the SIMLock.
  It is the entry point to the various requests in RSU feature.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_process_message
(
  const simlock_rsu_msg_req_type          * msg_req_ptr,
  simlock_rsu_message_response_data_type  * msg_resp_ptr
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_UNSUPPORTED;
  simlock_config_data_type   *config_file_data_ptr = NULL;

  if(msg_req_ptr == NULL ||
     msg_resp_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  /* Read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    /* Config read failed means no RSU is enabled. so returning here
       without processing blob */
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Process RSU related functions only if a valid RSU mode is configured */
  if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_NO_RSU ||
     config_file_data_ptr->rsu_mode >= SIMLOCK_RSU_MODE_MAX)
  {
    SIMLOCK_MSG_MED_1("simlock process RSU message, invalid RSU mode: 0x%x",
                      config_file_data_ptr->rsu_mode);
    simlock_config_free_config_data(config_file_data_ptr);
    return SIMLOCK_UNSUPPORTED;
  }

  msg_resp_ptr->msg_type = msg_req_ptr->msg_type;

  switch(msg_req_ptr->msg_type)
  {
    case SIMLOCK_RSU_GET_KEY_MSG:
      /* Get key message only exposed to RSU MODE A, E and F */
      if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_A || 
         config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_E ||
         config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_F)
      {
        simlock_status = simlock_rsu_device_key_generate_and_encrypt(&(msg_resp_ptr->message.get_key_resp_data),
                                                                     config_file_data_ptr->rsu_mode);
      }
      else if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_C)
      {
        simlock_status = simlock_rsu_nonce_generate_and_encrypt(&(msg_resp_ptr->message.get_key_resp_data),
                                                                config_file_data_ptr->rsu_mode);
      }
      else
      {
        simlock_status = SIMLOCK_UNSUPPORTED;
      }
      break;

    case SIMLOCK_RSU_SET_CONFIGURATION_MSG:
      simlock_status = simlock_rsu_config_set_data(msg_req_ptr->message.rsu_data.data_len,
                                                   msg_req_ptr->message.rsu_data.data_ptr,
                                                   &(msg_resp_ptr->message.set_config_resp_data),
                                                   config_file_data_ptr->rsu_mode);
      break;

    case SIMLOCK_RSU_CHECK_PUBLIC_KEY_MSG:
      simlock_status = simlock_remote_lock_compare_public_key(&msg_req_ptr->message.public_key,
                                                              config_file_data_ptr->rsu_mode);
      break;

    case SIMLOCK_RSU_GENERATE_HASH_MSG:
      if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_A)
      {
    simlock_status = simlock_rsu_process_generate_hash((uint16) msg_req_ptr->message.rsu_data.data_len,
                                                         msg_req_ptr->message.rsu_data.data_ptr,
                                                         &(msg_resp_ptr->message.generate_hash_resp_data));
      }
      else
      {
        simlock_status = SIMLOCK_UNSUPPORTED;
      }
      break;

    case SIMLOCK_RSU_RETRIEVE_VERSION_MSG:
      simlock_status = simlock_rsu_process_retrieve_version(&(msg_resp_ptr->message.version),
                                                            config_file_data_ptr->rsu_mode);
      break;

    case SIMLOCK_RSU_BLOB_REQUEST_MSG:
      if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_D)
      {
        simlock_status = simlock_rsu_process_blob_request(&(msg_resp_ptr->message.generate_simlock_blob_resp_data));
      }
      else
      {
        simlock_status = SIMLOCK_UNSUPPORTED;
      }
      break;

    case SIMLOCK_RSU_DELAY_TIMER_START_REQUEST_MSG:
      if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_D)
      {
        simlock_status = simlock_rsu_process_start_delay_timer_request(&(msg_resp_ptr->message.delay_timer_value));
      }
      else
      {
        simlock_status = SIMLOCK_UNSUPPORTED;
      }
      break;

    case SIMLOCK_RSU_DELAY_TIMER_STOP_REQUEST_MSG:
      if(config_file_data_ptr->rsu_mode == SIMLOCK_RSU_MODE_D)
      {
        simlock_status = simlock_rsu_process_stop_delay_timer_request();
      }
      else
      {
        simlock_status = SIMLOCK_UNSUPPORTED;
      }
      break;

    default:
      simlock_status = SIMLOCK_UNSUPPORTED;
      break;
  }

  simlock_config_free_config_data(config_file_data_ptr);

  SIMLOCK_MSG_MED_1("simlock process RSU message, status: 0x%x", simlock_status);
  return simlock_status;
} /* simlock_rsu_process_message */

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */
