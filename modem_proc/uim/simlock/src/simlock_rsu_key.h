#ifndef SIMLOCK_RSU_KEY_H
#define SIMLOCK_RSU_KEY_H
/*===========================================================================


            S I M   L O C K   R S U   K E Y   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_rsu_key.h#3 $$ $DateTime: 2020/06/07 15:57:55 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/20   cj      Remote SIM Unlock support for simlock rsu mode E & F
12/03/18   vgd     Move configuring pubkey from EFS to code for encrypting DEVICE_KEY
12/03/18   vgd     Added support for public keys for multiple RSU modes
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
07/13/17   nr      Remote SIM Unlock supported in common build 
07/13/17   av      Enhanced simlock RSU solution
12/02/16   vdc     Added support to check RSA public key for remote lock
07/16/16   av      Postpone creation of simlock config files 
01/07/16   stv     Remote simlock support
05/17/15   stv     Fix compiler errors
05/15/15   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock.h"

/*=============================================================================

                   DATA DECLARATIONS

=============================================================================*/
#define SIMLOCK_RSU_NONCE_LEN                  95
#define SIMLOCK_RSU_B64_STRING_LEN             80
#define SIMLOCK_RSU_RANDOM_NUM_LEN             60

typedef uint8  simlock_rsu_nonce_type[SIMLOCK_RSU_NONCE_LEN];

/*=============================================================================

                   ENUMERATED DATA DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_RSU_PUBKEY_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the type of public key (based on its purpose)
-----------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_RSU_PUBKEY_BLOB_VALIDATION                              = 0,
  SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION                               = 1,
  SIMLOCK_RSU_PUBKEY_KEY_ENCRYPTION_AND_BLOB_VALIDATION           = 2
} simlock_rsu_pubkey_enum_type;

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_PUBKEY_DATA_TYPE

   DESCRIPTION:
     This structure contains the public key data

     version:    File version
     public_key: Public Key (Modulus and Exponent)
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint8                               version;
  simlock_remote_lock_public_key_type public_key;
} simlock_rsu_pubkey_data_type;


/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/
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
);

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
  simlock_rsu_device_key_type     device_key_data
);

/*===========================================================================
FUNCTION SIMLOCK_RSU_DEVICE_KEY_GENERATE_AND_ENCRYPT

DESCRIPTION
  This function generates the modem key for the RSU. It encrypts the key,
  with the public key provisioned in the device.

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
);

/*===========================================================================
FUNCTION SIMLOCK_RSU_KEY_READ_NONCE_FILE

DESCRIPTION
  This function reads the modem nonce file and provides the nonce data.

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
);

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
);

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
);

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
);

#endif /* FEATURE_SIMLOCK  && FEATURE_SIMLOCK_RSU */

#endif /* SIMLOCK_RSU_KEY_H */
