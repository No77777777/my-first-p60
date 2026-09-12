/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M   L O C K   R S U   K E Y   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock RSU key functions.



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

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_rsu_key.c#4 $$ $DateTime: 2020/06/07 15:57:55 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/20   cj      Remote SIM Unlock support for simlock rsu mode E & F
12/27/19   sg      Added support for crypto API in MOB
09/10/19   vdc     SimLock solution for activation at Point Of Sale
12/03/18   vgd     Move configuring pubkey from EFS to code for encrypting DEVICE_KEY
12/03/18   vgd     Added support for public keys for multiple rsu modes
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
12/02/16   vdc     Added support to check RSA public key for remote lock
07/16/16   av      Postpone creation of simlock config files
01/07/16   stv     Remote simlock support
08/17/15   vv      Re-locate the public key file
06/22/15   stv     Prefer hardcoded key to EFS provisioing for pub key
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
#include "simlock_rsu.h"
#include "simlock_file.h"
#include "simlock_efs.h"
#include "simlock_crypto.h"
#include "simlock_util.h"
#include "simlock_rsu_key.h"
#include "simlock_config.h"
#include "uim_common_lib.h"
#include "uim_common_efs.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define SIMLOCK_RSU_PUBKEY_FILE_VERSION      1

#define SIMLOCK_RSU_INVALID_MOD_LEN          8

#define SIMLOCK_RSU_MAX_NUM_PUB_KEYS         (SIMLOCK_RSU_MODE_MAX - 1)

#define SIMLOCK_PUBKEY_EXPONENT_LEN          3

/* Additional 1 byte corresponds to: Version - 1 byte */
#define SIMLOCK_RSU_PUBKEY_FILE_SIZE_V1      (SIMLOCK_PUBKEY_MODULUS_LEN + SIMLOCK_PUBKEY_EXPONENT_LEN + 1)

/* Initialize the first 8-bytes to 0xFF and the rest to zero.
   If the first 8-bytes are 0xFF, then the modulus is considered
   as invalid. If the modulus is configured in the binary, this
   variable is expected to be updated during the integration */
static const uint8 simlock_rsu_pubkey_modulus_a[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Configure Key for RSU mode B like configured for mode A */
static const uint8 simlock_rsu_pubkey_modulus_b[] = {0x00};

/* Configure Key for RSU mode C like configured for mode A */
static const uint8 simlock_rsu_pubkey_modulus_c[] = {0x00};

/* Configure Key for RSU mode D like configured for mode A */
static const uint8 simlock_rsu_pubkey_modulus_d[] = {0x00};

/* Configure Key for RSU mode E like configured for mode A */
static const uint8 simlock_rsu_pubkey_modulus_e[] = {0x00};

/* Configure Key for RSU mode F like configured for mode A */
static const uint8 simlock_rsu_pubkey_modulus_f[] = {0x00};
/* For each RSU mode enabled in the device, its corresponding public key needs to be configured.
   Thus, the corresponding modulus should be updated here so that it is present in the 
   corresponding public key.*/
static const uint8 *simlock_rsu_pubkey_modulus[SIMLOCK_RSU_MAX_NUM_PUB_KEYS] =
  {
    simlock_rsu_pubkey_modulus_a, /* Mode A */
    simlock_rsu_pubkey_modulus_b, /* Mode B */
    simlock_rsu_pubkey_modulus_c, /* Mode C */
    simlock_rsu_pubkey_modulus_d,  /* Mode D */
    simlock_rsu_pubkey_modulus_e,  /* Mode E */
    simlock_rsu_pubkey_modulus_f  /* Mode F */
  };

/* If the exponent is configured in the binary, this variable
  is expected to be updated during the integration */
static const uint8 simlock_rsu_pubkey_exponent[SIMLOCK_RSU_MAX_NUM_PUB_KEYS][SIMLOCK_PUBKEY_EXPONENT_LEN] = 
{
 {0x00, 0x00, 0x00}, /* Mode A */
 {0x00, 0x00, 0x00}, /* Mode B */
 {0x00, 0x00, 0x00}, /* Mode C */
 {0x00, 0x00, 0x00}, /* Mode D */
 {0x00, 0x00, 0x00}, /* Mode E */
 {0x00, 0x00, 0x00}  /* Mode F */
};

/* Initialize the first 8-bytes to 0xFF and the rest to zero.
   If the first 8-bytes are 0xFF, then the modulus is considered
   as invalid. If the modulus is configured in the binary, this
   variable is expected to be updated during the integration */
static const uint8 simlock_rsu_devicekey_pubkey_modulus_a[SIMLOCK_PUBKEY_MODULUS_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     							         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
																						 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
																						 
/* If the exponent is configured in the binary, this variable
  is expected to be updated during the integration */
static const uint8 simlock_rsu_devicekey_pubkey_exponent_a[SIMLOCK_PUBKEY_EXPONENT_LEN] = 
 {0x00, 0x00, 0x00}; /* Mode A Device Key Exponent */

/*=============================================================================

                   ENUMERATED DATA DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_RSU_DEVICE_ID_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the rsu device id being used to concatenate with
     B64(Random) to generate NONCE.
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_RSU_DEVICE_ID_IMEI = 0,
  SIMLOCK_RSU_DEVICE_ID_MEID
} simlock_rsu_device_id_enum_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_RSU_GET_PUB_KEY_INDEX

DESCRIPTION
  This function takes the rsu mode as input and returns the corresponding
  index of the simlock_rsu_pubkey_modulus and simlock_rsu_pubkey_exponent
  array.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  uint8

SIDE EFFECTS
  NONE
===========================================================================*/
static uint8 simlock_rsu_get_pub_key_index
(
  simlock_rsu_mode_enum_type rsu_mode
)
{
  uint8 index = 0;
  
  switch(rsu_mode)
  {
    case SIMLOCK_RSU_MODE_A:
      index = 0;
      break;

    case SIMLOCK_RSU_MODE_B:
      index = 1;
      break;

    case SIMLOCK_RSU_MODE_C:
      index = 2;
      break;

    case SIMLOCK_RSU_MODE_D:
      index = 3;
      break;

    case SIMLOCK_RSU_MODE_E:
      index = 4;
      break;

    case SIMLOCK_RSU_MODE_F:
      index = 5;
      break;

    default:
      index = SIMLOCK_RSU_MAX_NUM_PUB_KEYS;
      break;
  }
  
  return index;
} /* simlock_rsu_get_pub_key_index */


/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_IS_PUBKEY_CONFIGURED

DESCRIPTION
  This function checks if the public key (based on the type) is configured
  or not.
  Note that key used for blob validation is configured in the source code
  whereas the key used for encrypting keymod is configured in the SFS

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE  : If the public key is configured
  FALSE : If otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
static boolean simlock_rsu_key_is_pubkey_configured
(
  simlock_rsu_pubkey_enum_type      pubkey_type,
  simlock_rsu_mode_enum_type        rsu_mode
)
{
  boolean                  is_pubkey_configured = FALSE;
  uint8                    invalid_modulus_data[SIMLOCK_RSU_INVALID_MOD_LEN] =
                                                  {0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0xFF};
  uint8                    invalid_exponent_data[SIMLOCK_PUBKEY_EXPONENT_LEN] =
                                                  {0x00, 0x00, 0x00};
  uint8                    index                = 0;

#ifndef FEATURE_UIM_TEST_FRAMEWORK
  switch(pubkey_type)
  {
    case SIMLOCK_RSU_PUBKEY_BLOB_VALIDATION:
    case SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION:
      /* Pubkey for blob validation is expected to be configured in the binary
         for all RSU solutions */
      index = simlock_rsu_get_pub_key_index(rsu_mode);
      switch(rsu_mode)
      {
        case SIMLOCK_RSU_MODE_A:
          if(sizeof(simlock_rsu_pubkey_modulus_a) == SIMLOCK_PUBKEY_MODULUS_LEN &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        case SIMLOCK_RSU_MODE_B:
          if(sizeof(simlock_rsu_pubkey_modulus_b) == SIMLOCK_PUBKEY_MODULUS_LEN &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        case SIMLOCK_RSU_MODE_C:
          if(sizeof(simlock_rsu_pubkey_modulus_c) == SIMLOCK_PUBKEY_MODULUS_LEN &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        case SIMLOCK_RSU_MODE_D:
          if(sizeof(simlock_rsu_pubkey_modulus_d) == SIMLOCK_PUBKEY_MODULUS_LEN &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        case SIMLOCK_RSU_MODE_E:
          if(sizeof(simlock_rsu_pubkey_modulus_e) == SIMLOCK_PUBKEY_MODULUS_LEN                                &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN                         &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        case SIMLOCK_RSU_MODE_F:
          if(sizeof(simlock_rsu_pubkey_modulus_f) == SIMLOCK_PUBKEY_MODULUS_LEN                                &&
             sizeof(simlock_rsu_pubkey_exponent[index]) == SIMLOCK_PUBKEY_EXPONENT_LEN                         &&
             memcmp(simlock_rsu_pubkey_modulus[index], invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
             memcmp(simlock_rsu_pubkey_exponent[index], invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
          {
            is_pubkey_configured = TRUE;
          }
          break;

        default:
          SIMLOCK_MSG_ERR_0("Invalid RSU mode");
          break;
      }

      if(!is_pubkey_configured)
      {
        SIMLOCK_MSG_ERR_0("pubkey not configured or is configured improperly");
      }    
      break;

    case SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION:
      /* Since public key encryption is only done for mode A, so proceed only if rsu_mode is mode A */
      if (rsu_mode == SIMLOCK_RSU_MODE_A)
      {
        if(sizeof(simlock_rsu_devicekey_pubkey_modulus_a) == SIMLOCK_PUBKEY_MODULUS_LEN &&
           sizeof(simlock_rsu_devicekey_pubkey_exponent_a) == SIMLOCK_PUBKEY_EXPONENT_LEN &&
           memcmp(simlock_rsu_devicekey_pubkey_modulus_a, invalid_modulus_data, SIMLOCK_RSU_INVALID_MOD_LEN) != 0 &&
           memcmp(simlock_rsu_devicekey_pubkey_exponent_a, invalid_exponent_data, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
        {
          is_pubkey_configured = TRUE;
        }
      }          
      break;

    default:
      break;
  }
#else
  #error code not present
#endif /* !FEATURE_UIM_TEST_FRAMEWORK */

  return is_pubkey_configured;
} /* simlock_rsu_key_is_pubkey_configured */


/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_READ_PUBKEY_DATA

DESCRIPTION
  This function reads the public key (based of the type) used by RSU engine

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_key_read_pubkey_data
(
  simlock_rsu_pubkey_enum_type      pubkey_type,
  simlock_rsu_pubkey_data_type    * pubkey_data_ptr,
  simlock_rsu_mode_enum_type        rsu_mode
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_GENERIC_ERROR;
  uint8                       index                = 0;

  if(pubkey_data_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  switch(pubkey_type)
  {
    case SIMLOCK_RSU_PUBKEY_BLOB_VALIDATION:
    case SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION:
      if(simlock_rsu_key_is_pubkey_configured(pubkey_type, rsu_mode))
      {
        index = simlock_rsu_get_pub_key_index(rsu_mode);
        if(index >= SIMLOCK_RSU_MAX_NUM_PUB_KEYS || 
           simlock_rsu_pubkey_modulus[index] == NULL ||
           simlock_rsu_pubkey_exponent[index] == NULL)
        {
          return SIMLOCK_GENERIC_ERROR;
        }

        simlock_memscpy(pubkey_data_ptr->public_key.modulus,
                        SIMLOCK_PUBKEY_MODULUS_LEN,
                        simlock_rsu_pubkey_modulus[index],
                        SIMLOCK_PUBKEY_MODULUS_LEN);

        simlock_memscpy(pubkey_data_ptr->public_key.exponent,
                        SIMLOCK_PUBKEY_EXPONENT_LEN,
                        simlock_rsu_pubkey_exponent[index],
                        SIMLOCK_PUBKEY_EXPONENT_LEN);

        pubkey_data_ptr->version = SIMLOCK_RSU_PUBKEY_FILE_VERSION;
        simlock_status = SIMLOCK_SUCCESS;
      }
      break;

    case SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION:
      /* Since public key encryption is only done for mode A, so validate if the incoming rsu mode
         is mode A or not */
      if (rsu_mode != SIMLOCK_RSU_MODE_A)
      {
        return SIMLOCK_GENERIC_ERROR;
      }
	  
      if(simlock_rsu_key_is_pubkey_configured(pubkey_type, rsu_mode))
      {
        simlock_memscpy(pubkey_data_ptr->public_key.modulus,
                        sizeof(pubkey_data_ptr->public_key.modulus),
                        simlock_rsu_devicekey_pubkey_modulus_a,
                        SIMLOCK_PUBKEY_MODULUS_LEN);

        simlock_memscpy(pubkey_data_ptr->public_key.exponent,
                        sizeof(pubkey_data_ptr->public_key.exponent),
                        simlock_rsu_devicekey_pubkey_exponent_a,
                        SIMLOCK_PUBKEY_EXPONENT_LEN);

        pubkey_data_ptr->version = SIMLOCK_RSU_PUBKEY_FILE_VERSION;
        simlock_status = SIMLOCK_SUCCESS;
      }
      break;

    default:
      break;
  }

  return simlock_status;
} /* simlock_rsu_key_read_pubkey_data */


/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_GET_RANDOM_KEY

DESCRIPTION
  This function gets the random device key. If the random device key file exists in the SFS
  location, it is read for the key. If not, it generates the random device key.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
static simlock_result_enum_type simlock_rsu_key_get_random_device_key
(
  simlock_rsu_device_key_type            device_key
)
{
  simlock_result_enum_type    simlock_status  = SIMLOCK_SUCCESS;

  /* read the device key from file */
  simlock_status = simlock_rsu_key_read_device_key_file(device_key);
  if(simlock_status == SIMLOCK_SUCCESS)
  {
    return SIMLOCK_SUCCESS;
  }

  /* file doesn't exist, generate the random device key */
  simlock_status = simlock_crypto_get_random_data(device_key, sizeof(simlock_rsu_device_key_type));
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("Device key generation failed, status: 0x%x", simlock_status);
    return simlock_status;
  }

  /* write the random device key to the file */
  simlock_status = simlock_file_write(device_key, sizeof(simlock_rsu_device_key_type),
                                      SIMLOCK_FILE_RSU_DEVICE_KEY, SIMLOCK_SFS_LOCAL,
                                      SIMLOCK_CONFIG_ID_DEFAULT);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("Device write key failed, status: 0x%x", simlock_status);
  }

  return simlock_status;
} /* simlock_rsu_key_get_random_device_key */


/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_READ_DEVICE_KEY_FILE

DESCRIPTION
  This function reads the modem key file and provides the device key data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_key_read_device_key_file
(
  simlock_rsu_device_key_type        device_key_data
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_SUCCESS;
  uint32                      device_key_file_size     = 0;
  uint8                     * device_key_file_data_ptr = NULL;

  do
  {
    /* read the entire file data. device_key_file_data_ptr holds the
       raw key data */
    simlock_status = simlock_file_read(&device_key_file_data_ptr,
                                       &device_key_file_size,
                                       SIMLOCK_FILE_RSU_DEVICE_KEY,
                                       SIMLOCK_SFS_LOCAL,
                                       SIMLOCK_CONFIG_ID_DEFAULT);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (device_key_file_data_ptr == NULL))
    {
      SIMLOCK_MSG_ERR_1("device key file read failed, status: 0x%x",
                        simlock_status);
      break;
    }

     /* if the file size is not of the expected size,
       then it is not valid */
    if(device_key_file_size != SIMLOCK_RSU_DEVICE_KEY_LEN)
    {
      SIMLOCK_MSG_ERR_1("device key file size is not as expected, 0x%x",
                        device_key_file_size);
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_memscpy(device_key_data,
                    SIMLOCK_RSU_DEVICE_KEY_LEN,
                    device_key_file_data_ptr,
                    device_key_file_size);
  }while (0);

  SIMLOCK_MEM_FREE(device_key_file_data_ptr);
  return simlock_status;
} /* simlock_rsu_key_read_device_key_file */


/*===========================================================================
FUNCTION SIMLOCK_RSU_DEVICE_KEY_GENERATE_AND_ENCRYPT

DESCRIPTION
  This function generates the modem key for the RSU. It encrypts the key,
  with the public key provisioned in the device

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_device_key_generate_and_encrypt
(
  simlock_data_type          * encrypted_key_ptr,
  simlock_rsu_mode_enum_type   rsu_mode
)
{
  simlock_result_enum_type        simlock_status  = SIMLOCK_SUCCESS;
  simlock_rsu_pubkey_data_type  * pubkey_data_ptr = NULL;
  simlock_rsu_device_key_type     device_key_data_buf;
  simlock_rsu_pubkey_enum_type    public_key_type = SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION;
  secapi_pkx_padding_enum_type    padding_type = SECAPI_PKX_RSA_PADDING_SIG_PKCS1_V15;

  memset(device_key_data_buf, 0x00, SIMLOCK_RSU_DEVICE_KEY_LEN);

  SIMLOCK_MSG_MED_0("simlock_rsu_device_key_generate");

  if(encrypted_key_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(encrypted_key_ptr, 0x00, sizeof(simlock_data_type));

  switch(rsu_mode)
  {
    case SIMLOCK_RSU_MODE_A:
    case SIMLOCK_RSU_MODE_E:
      padding_type = SECAPI_PKX_RSA_PADDING_SIG_PKCS1_V15;
      break;

    case SIMLOCK_RSU_MODE_F:
      padding_type = SECAPI_PKX_RSA_PADDING_ENC_PKCS1_V15;
      break;

    default:
      return SIMLOCK_INCORRECT_PARAMS;
  }

  /* get the random device key */
  simlock_status = simlock_rsu_key_get_random_device_key(device_key_data_buf);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("get random key failed, status: 0x%x", simlock_status);
    return simlock_status;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(pubkey_data_ptr, sizeof(simlock_rsu_pubkey_data_type));
  if(pubkey_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  if(rsu_mode ==  SIMLOCK_RSU_MODE_A)
  {
  /* read from SFS the public key parameters - modulus and exponent */
    public_key_type = SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION;
  }

  /* read from hard coded public key parameters for mode E and F 
     if it is not mode A then we are passing 
     SIMLOCK_RSU_PUBKEY_FOR_BOTH_KEY_ENCRYPTION_AND_BLOB_VALIDATION 
     because for mode E and F we will use public key configured in code */
  simlock_status = simlock_rsu_key_read_pubkey_data(public_key_type,
                                                    pubkey_data_ptr,
                                                    rsu_mode);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_0("unable to get the pubkey");
    SIMLOCK_MEM_FREE(pubkey_data_ptr);
    return simlock_status;
  }

  /* check the version */
  if(pubkey_data_ptr->version != SIMLOCK_RSU_PUBKEY_FILE_VERSION)
  {
    SIMLOCK_MSG_ERR_1("pubkey file version, 0x%x not supported",
                      pubkey_data_ptr->version);
    SIMLOCK_MEM_FREE(pubkey_data_ptr);
    return simlock_status;
  }

  /* perform the key encryption using RSA */
  simlock_status = simlock_crypto_rsa(SIMLOCK_CRYPTO_RSA_ENCRYPT,
                                      device_key_data_buf,
                                      SIMLOCK_RSU_DEVICE_KEY_LEN,
                                      pubkey_data_ptr->public_key.modulus,
                                      SIMLOCK_PUBKEY_MODULUS_LEN,
                                      pubkey_data_ptr->public_key.exponent,
                                      SIMLOCK_PUBKEY_EXPONENT_LEN,
                                      encrypted_key_ptr,
                                      padding_type);

  SIMLOCK_MEM_FREE(pubkey_data_ptr);

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("key encryption failed, status: 0x%x", simlock_status);
    return simlock_status;
  }
  return SIMLOCK_SUCCESS;
} /* simlock_rsu_device_key_generate_and_encrypt */


/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_READ_NONCE_FILE

DESCRIPTION
  This function reads the modem nonce file and provides the nonce data

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_key_read_nonce_file
(
  simlock_rsu_nonce_type        nonce_data
)
{
  simlock_result_enum_type    simlock_status       = SIMLOCK_SUCCESS;
  uint32                      nonce_file_size      = 0;
  uint8                     * nonce_file_data_ptr  = NULL;

  do
  {
    /* read the entire file data. keymod_file_data_ptr holds the
       raw key data */
    simlock_status = simlock_file_read(&nonce_file_data_ptr,
                                       &nonce_file_size,
                                       SIMLOCK_FILE_RSU_NONCE,
                                       SIMLOCK_SFS_LOCAL,
                                       SIMLOCK_CONFIG_ID_DEFAULT);
    if((simlock_status != SIMLOCK_SUCCESS) ||
       (nonce_file_data_ptr == NULL))
    {
      SIMLOCK_MSG_ERR_1("read nonce file failed, status: 0x%x",
                        simlock_status);
      break;
    }

     /* if the file size is not of the expected size,
       then it is not valid */
    if(nonce_file_size != SIMLOCK_RSU_NONCE_LEN)
    {
      SIMLOCK_MSG_ERR_1("nonce file size is not as expected, 0x%x",
                        nonce_file_size);
      simlock_status = SIMLOCK_GENERIC_ERROR;
      break;
    }

    simlock_memscpy(nonce_data,
                    SIMLOCK_RSU_NONCE_LEN,
                    nonce_file_data_ptr,
                    nonce_file_size);
  }while (0);

  SIMLOCK_MEM_FREE(nonce_file_data_ptr);

  return simlock_status;
} /* simlock_rsu_key_read_nonce_file */


/*===========================================================================
FUNCTION SIMLOCK_RSU_NONCE_GENERATE_AND_ENCRYPT

DESCRIPTION
  This function generates the modem nonce for the RSU. It encrypts the nonce,
  with the public key provisioned in the device

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_rsu_nonce_generate_and_encrypt
(
  simlock_data_type          * encrypted_nonce_ptr,
  simlock_rsu_mode_enum_type   rsu_mode
)
{
  simlock_result_enum_type         simlock_status          = SIMLOCK_SUCCESS;
  simlock_rsu_pubkey_data_type    *pubkey_data_ptr         = NULL;
  char                            *b64_nonce_ptr           = NULL;
  uint8                           *nonce_buf_ptr           = NULL;
  simlock_rsu_device_id_enum_type  rsu_device_id           = SIMLOCK_RSU_DEVICE_ID_IMEI;
  uint8                            b64_nonce_buf_len       = 0;
  uint8                            random_key_data_buf[SIMLOCK_RSU_RANDOM_NUM_LEN] = {0};

  SIMLOCK_MSG_MED_0("simlock_rsu_nonce_generate");

  if(encrypted_nonce_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  memset(encrypted_nonce_ptr, 0x00, sizeof(simlock_data_type));

  /* generate the random key */
  simlock_status = simlock_crypto_get_random_data(random_key_data_buf, sizeof(random_key_data_buf));
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("key generation failed, status: 0x%x", simlock_status);
    return simlock_status;
  }

  /* NONCE is calculated as one of this:
     a. B64(random) + IMEI
     b. B64(random) + MEID + 'Z' */
  b64_nonce_ptr = uim_common_util_alloc_bin_to_base64string(random_key_data_buf, sizeof(random_key_data_buf));
  if(b64_nonce_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  b64_nonce_buf_len = strlen(b64_nonce_ptr);
  if(b64_nonce_buf_len != SIMLOCK_RSU_B64_STRING_LEN)
  {
    SIMLOCK_MEM_FREE(b64_nonce_ptr);
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Allocate nonce buffer with size SIMLOCK_RSU_NONCE_LEN bytes */
  SIMLOCK_CHECK_AND_MEM_MALLOC(nonce_buf_ptr, SIMLOCK_RSU_NONCE_LEN);
  if(nonce_buf_ptr == NULL)
  {
    SIMLOCK_MEM_FREE(b64_nonce_ptr);
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  simlock_memscpy(nonce_buf_ptr, SIMLOCK_RSU_NONCE_LEN, b64_nonce_ptr, b64_nonce_buf_len);

  SIMLOCK_MEM_FREE(b64_nonce_ptr);

  (void)uim_common_efs_read(UIM_COMMON_EFS_RSU_DEVICE_ID,
                            UIM_COMMON_EFS_ITEM_FILE_TYPE,
                            UIM_COMMON_EFS_DEVICE,
                            &rsu_device_id,
                            sizeof(rsu_device_id));

  /* concatenate the device id based on NV */
  if(rsu_device_id == SIMLOCK_RSU_DEVICE_ID_IMEI)
  {
    simlock_imei_type          device_imei;

    memset(&device_imei, 0x00, sizeof(simlock_imei_type));

    simlock_status = simlock_util_get_imei(SIMLOCK_SLOT_1, &device_imei);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      simlock_memscpy(nonce_buf_ptr + b64_nonce_buf_len,
                      SIMLOCK_RSU_NONCE_LEN - b64_nonce_buf_len,
                      device_imei.imei_data,
                      device_imei.imei_len);
    }
  }
  else if(rsu_device_id == SIMLOCK_RSU_DEVICE_ID_MEID)
  {
    simlock_meid_type  device_meid = {0};

    /* read MEID and append to B64 encoded random number */
    simlock_status = simlock_util_get_meid(SIMLOCK_SLOT_1, &device_meid);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      simlock_memscpy(nonce_buf_ptr + b64_nonce_buf_len,
                      SIMLOCK_RSU_NONCE_LEN - b64_nonce_buf_len,
                      device_meid.meid_data,
                      device_meid.meid_len);

      if((b64_nonce_buf_len + device_meid.meid_len) < SIMLOCK_RSU_NONCE_LEN)
      {
        /* append 'Z' to the nonce */
        nonce_buf_ptr[b64_nonce_buf_len + device_meid.meid_len] = 'Z';
      }
      else
      {
        simlock_status = SIMLOCK_GENERIC_ERROR;
      }
    }
  }
  else
  {
    simlock_status = SIMLOCK_GENERIC_ERROR;
  }

  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MEM_FREE(nonce_buf_ptr);
    return simlock_status;
  }

  /* write the random nonce to the file */
  simlock_status = simlock_file_write(nonce_buf_ptr,
                                      SIMLOCK_RSU_NONCE_LEN,
                                      SIMLOCK_FILE_RSU_NONCE,
                                      SIMLOCK_SFS_LOCAL,
                                      SIMLOCK_CONFIG_ID_DEFAULT);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("write nonce failed, status: 0x%x", simlock_status);
    SIMLOCK_MEM_FREE(nonce_buf_ptr);
    return simlock_status;
  }

  /* we have finished the first part of the function where NONCE is generated
     and we need to do encryption here. */
  do
  {
    SIMLOCK_CHECK_AND_MEM_MALLOC(pubkey_data_ptr, sizeof(simlock_rsu_pubkey_data_type));
    if(pubkey_data_ptr == NULL)
    {
      break;
    }

    simlock_status = simlock_rsu_key_read_pubkey_data(SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION,
                                                      pubkey_data_ptr,
                                                      rsu_mode);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_0("unable to get the pubkey");
      break;
    }

    /* check the version */
    if(pubkey_data_ptr->version != SIMLOCK_RSU_PUBKEY_FILE_VERSION)
    {
      SIMLOCK_MSG_ERR_1("pubkey file version, 0x%x not supported",
                        pubkey_data_ptr->version);
      break;
    }

    /* perform the key encryption using RSA */
    simlock_status = simlock_crypto_rsa(SIMLOCK_CRYPTO_RSA_ENCRYPT,
                                        nonce_buf_ptr,
                                        SIMLOCK_RSU_NONCE_LEN,
                                        pubkey_data_ptr->public_key.modulus,
                                        SIMLOCK_PUBKEY_MODULUS_LEN,
                                        pubkey_data_ptr->public_key.exponent,
                                        SIMLOCK_PUBKEY_EXPONENT_LEN,
                                        encrypted_nonce_ptr,
                                        SECAPI_PKX_RSA_PADDING_ENC_PKCS1_V15);
    if(simlock_status != SIMLOCK_SUCCESS)
    {
      SIMLOCK_MSG_ERR_1("key encryption failed, status: 0x%x", simlock_status);
    }
  }while(0);

  SIMLOCK_MEM_FREE(pubkey_data_ptr);
  SIMLOCK_MEM_FREE(nonce_buf_ptr);

  return simlock_status;
} /* simlock_rsu_nonce_generate_and_encrypt */


/*===========================================================================
FUNCTION SIMLOCK_REMOTE_LOCK_COMPARE_PUBLIC_KEY

DESCRIPTION
  This function compares the input public key (modulus, exponent) with the
  public key that is configured for blob validation purposes

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_remote_lock_compare_public_key
(
  const simlock_remote_lock_public_key_type  *remote_lock_public_key_ptr,
  simlock_rsu_mode_enum_type                  rsu_mode
)
{
  simlock_rsu_pubkey_data_type  * pubkey_data_ptr = NULL;
  simlock_result_enum_type        simlock_status  = SIMLOCK_SUCCESS;

  if(remote_lock_public_key_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  SIMLOCK_CHECK_AND_MEM_MALLOC(pubkey_data_ptr, sizeof(simlock_rsu_pubkey_data_type));
  if(pubkey_data_ptr == NULL)
  {
    return SIMLOCK_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  simlock_status = simlock_rsu_key_read_pubkey_data(SIMLOCK_RSU_PUBKEY_BLOB_VALIDATION,
                                                    pubkey_data_ptr,
                                                    rsu_mode);
  if(simlock_status != SIMLOCK_SUCCESS ||
     pubkey_data_ptr->version != SIMLOCK_RSU_PUBKEY_FILE_VERSION ||
     memcmp(pubkey_data_ptr->public_key.modulus, remote_lock_public_key_ptr->modulus, SIMLOCK_PUBKEY_MODULUS_LEN) != 0 ||
     memcmp(pubkey_data_ptr->public_key.exponent, remote_lock_public_key_ptr->exponent, SIMLOCK_PUBKEY_EXPONENT_LEN) != 0)
  {
    simlock_status = SIMLOCK_GENERIC_ERROR;
  }

  SIMLOCK_MEM_FREE(pubkey_data_ptr);
  return simlock_status;
} /* simlock_remote_lock_compare_public_key */


/*===========================================================================
FUNCTION  SIMLOCK_SET_SIMLOCK_RSU_DEVICE_KEY

DESCRIPTION
  This function set the simlock rsu device key in simlock file SFS.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_simlock_rsu_device_key
(
  const simlock_rsu_device_key_type      *rsu_device_key_ptr
)
{
  simlock_result_enum_type       simlock_status       = SIMLOCK_SUCCESS;
  simlock_config_data_type      *config_file_data_ptr = NULL;
  simlock_rsu_device_key_type    device_key_data_buf;
  simlock_rsu_mode_enum_type     rsu_mode             = SIMLOCK_RSU_MODE_NO_RSU;

  if(rsu_device_key_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  /* Read config file */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    return SIMLOCK_GENERIC_ERROR;
  }

  rsu_mode = config_file_data_ptr->rsu_mode;
  simlock_config_free_config_data(config_file_data_ptr);

  if(rsu_mode != SIMLOCK_RSU_MODE_D)
  {
    SIMLOCK_MSG_ERR_1("simlock_set_simlock_rsu_device_key not supported for mode: 0x%x", rsu_mode);
    return SIMLOCK_UNSUPPORTED;
  }

  memset(device_key_data_buf, 0x00, SIMLOCK_RSU_DEVICE_KEY_LEN);

  /* read the device key from file */
  simlock_status = simlock_rsu_key_read_device_key_file(device_key_data_buf);
  if(simlock_status == SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_0("Device Key is already provisioned");
    /* If the DEVICE_KEY is already present in the modem,
       the terminal returns an error */
    return SIMLOCK_GENERIC_ERROR;
  }

  /* write the device key to the file */
  simlock_status = simlock_file_write((uint8 *)rsu_device_key_ptr,
                                      sizeof(simlock_rsu_device_key_type),
                                      SIMLOCK_FILE_RSU_DEVICE_KEY,
                                      SIMLOCK_SFS_LOCAL,
                                      SIMLOCK_CONFIG_ID_DEFAULT);
  if(simlock_status != SIMLOCK_SUCCESS)
  {
    SIMLOCK_MSG_ERR_1("write device key failed, status: 0x%x", simlock_status);
  }

  return simlock_status;
} /* simlock_set_simlock_rsu_device_key */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */
