#ifndef SIMLOCK_RSU_CONFIG_H
#define SIMLOCK_RSU_CONFIG_H
/*===========================================================================


            S I M   L O C K   R S U   C O N F I G   H E A D E R


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

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_rsu_config.h#4 $$ $DateTime: 2020/06/07 15:57:55 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/20   cj      Remote SIM Unlock support for simlock rsu mode E & F
12/27/19   cj      Make device dual SIM to single SIM via RSU BLOB
04/25/19   mm      Updated major and minor version for rsu mode A
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
07/13/17   nr      Remote SIM Unlock supported in common build
07/13/17   av      Enhanced simlock RSU solution
12/01/16   vdc     Extend blob to support lock/unlock with HCK
01/07/16   stv     Remote simlock support
08/19/15   stv     Restrict native simlock locking when rsu is enabled
05/15/15   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"

#if defined(FEATURE_SIMLOCK) && defined(FEATURE_SIMLOCK_RSU)
#include "simlock.h"
#include "simlock_crypto.h"
#include "simlock_util.h"
#include "simlock_time.h"

/*=============================================================================

                   DATA DECLARATIONS

=============================================================================*/
#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1                      1
#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_2                      2
#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_3                      3
#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_4                      4
#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_5                      5

#define SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION                        0
#define SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION_5                      5

#define SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_MAX                    SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_5
#define SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION_MAX                    SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION

#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_A_MAJOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_3
#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_A_MINOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION

#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_D_MAJOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1
#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_D_MINOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION

#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MAJOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1
#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_E_MINOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION_5

#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MAJOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MAJOR_VERSION_1
#define SIMLOCK_REMOTE_LOCK_CONFIG_RSU_MODE_F_MINOR_VERSION_MAX         SIMLOCK_REMOTE_LOCK_CONFIG_MINOR_VERSION_5

#define SIMLOCK_RSU_SWAP_BYTE_ORDER_16(x)      \
     (((x >> 8) & 0xFF) + ((x & 0xFF) << 8))

#define SIMLOCK_RSU_SWAP_BYTE_ORDER_32(x)      \
     (((x & 0xFF000000) >> 24) + ((x & 0x00FF0000) >> 8) + ((x & 0x0000FF00) << 8) + ((x & 0x000000FF) << 24))

#define SIMLOCK_RSU_SWAP_BYTE_ORDER_64(x)      \
     (((x & 0xFF00000000000000) >> 56) + ((x & 0x00FF000000000000) >> 40) + ((x & 0x0000FF0000000000) >> 24) + ((x & 0x000000FF00000000) >> 8) +    \
      ((x & 0x00000000FF000000) << 8) + ((x & 0x0000000000FF0000) << 24) + ((x & 0x000000000000FF00) << 40) + ((x & 0x00000000000000FF) << 56))

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_RFU_CONFIG_PROTECTION_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the protection scheme of the config data
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_RSU_CONFIG_PROTECTION_NONE    = 0,
  SIMLOCK_RSU_CONFIG_PROTECTION_SHA256,
  SIMLOCK_RSU_CONFIG_PROTECTION_RSA
}simlock_rsu_config_protection_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      SIMLOCK_RFU_CONFIG_LOCK_STATE_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the current lock state in the config data
-------------------------------------------------------------------------------*/
typedef enum {
  SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_LOCK  = 0,
  SIMLOCK_RSU_CONFIG_LOCK_STATE_TEMPORARY_UNLOCK,
  SIMLOCK_RSU_CONFIG_LOCK_STATE_PERMANENT_UNLOCK,
  SIMLOCK_RSU_CONFIG_LOCK_STATE_PARTIAL_UNLOCK
}simlock_rsu_config_lock_state_enum_type;

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_HEADER_DATA_TYPE

   DESCRIPTION:
     This structure contains the header data of the RSU configuration

     version_major       : Major version of config data
     version_minor       : Minor version of config data
     protection_type     : Type of protection (None, MAC or RSA Sig)
     lock_state          : State of the simlock
     time_stamp          : Time stamp at which the blob is created
-------------------------------------------------------------------------------*/
typedef struct
{
  uint16                                     version_major;
  uint16                                     version_minor;
  simlock_rsu_config_protection_enum_type    protection_type;
  union {
    simlock_rsu_config_lock_state_enum_type  lock_state;
    uint8                                    lock_code;
  }data;
  uint64                                     time_stamp;
}simlock_rsu_config_header_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_NW_CATEGORY_INFO_TYPE

   DESCRIPTION:
     This structure contains the list of MCC/MNCs

     change_flag      : Indicates code data to be used for set lock
     iteration_cnt    : Number of iterations for PBKDF2 algorithm
     salt             : Salt of PBKDF2 algorithm
     hck              : Hashed control key
     nw_codes         : Number of nw_codes in the list
     nw_code_ptr      : Pointer to nw code data
-------------------------------------------------------------------------------*/
typedef struct
{
  boolean                       change_flag;
  uint32                        iteration_cnt;
  simlock_salt_type             salt;
  simlock_hck_type              hck;
  uint16                        nw_codes;
  simlock_nw_code_data_type    *nw_code_ptr;
}simlock_rsu_nw_category_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_NS_CATEGORY_INFO_TYPE

   DESCRIPTION:
     This structure contains the NS lock info

     change_flag      : Indicates code data to be used for set lock
     iteration_cnt    : Number of iterations for PBKDF2 algorithm
     salt             : Salt of PBKDF2 algorithm
     hck              : Hashed control key
     ns_codes         : Number of ns codes in the list
     ns_code_ptr      : Pointer to NS code data
-------------------------------------------------------------------------------*/
typedef struct
{
  boolean                       change_flag;
  uint32                        iteration_cnt;
  simlock_salt_type             salt;
  simlock_hck_type              hck;
  uint16                        ns_codes;
  simlock_ns_code_data_type    *ns_code_ptr;
}simlock_rsu_ns_category_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_SP_CATEGORY_INFO_TYPE

   DESCRIPTION:
     This structure contains the SP lock info

     change_flag      : Indicates code data to be used for set lock
     iteration_cnt    : Number of iterations for PBKDF2 algorithm
     salt             : Salt of PBKDF2 algorithm
     hck              : Hashed control key
     sp_codes         : Number of sp codes in the list
     sp_code_ptr      : Pointer to SP code data
-------------------------------------------------------------------------------*/
typedef struct
{
  boolean                       change_flag;
  uint32                        iteration_cnt;
  simlock_salt_type             salt;
  simlock_hck_type              hck;
  uint16                        sp_codes;
  simlock_sp_code_data_type    *sp_code_ptr;
}simlock_rsu_sp_category_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_SP_EHPLMN_CATEGORY_INFO_TYPE

   DESCRIPTION:
     This structure contains SP-EHPLMN lock info

     change_flag             : Indicates code data to be used for set lock
     iteration_cnt           : Number of iterations for PBKDF2 algorithm
     salt                    : Salt of PBKDF2 algorithm
     hck                     : Hashed control key
     sp_ehplmn_codes         : Number of sp_ehplmn_codes in the list
     sp_ehplmn_code_ptr      : Pointer to sp_ehplmn code data
-------------------------------------------------------------------------------*/
typedef struct
{
  boolean                            change_flag;
  uint32                             iteration_cnt;
  simlock_salt_type                  salt;
  simlock_hck_type                   hck;
  uint16                             sp_ehplmn_codes;
  simlock_sp_ehplmn_code_data_type  *sp_ehplmn_code_ptr;
}simlock_rsu_sp_ehplmn_category_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_CODE_DATA_TYPE

   DESCRIPTION:
     This structure contains the configuration data

     nw_lock           : nw lock information
     ns_lock           : ns lock information
     sp_lock           : sp lock information
     sp_ehplmn_lock    : sp_ehplmn lock information
-------------------------------------------------------------------------------*/
typedef struct
{
  simlock_rsu_nw_category_info_type         nw_lock;
  simlock_rsu_ns_category_info_type         ns_lock;
  simlock_rsu_sp_category_info_type         sp_lock;
  simlock_rsu_sp_ehplmn_category_info_type  sp_ehplmn_lock;
}simlock_rsu_config_lock_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_CODE_DATA_TYPE

   DESCRIPTION:
     This structure contains the configuration data of the RSU

     lock_code_data   : Contains the lock code information
     start_time       : Temporary unlock start time (in seconds)
     end_time         : Temporary unlock end time (in seconds)
     slot_operation   : Provides info on enabling/disabling of slots
-------------------------------------------------------------------------------*/
typedef struct
{
  simlock_rsu_config_lock_code_data_type     lock_code_data;
  simlock_time_type                          start_time;
  simlock_time_type                          end_time;
  boolean                                    disabled_slots_info_valid;
  simlock_disabled_slot_data_type            disabled_slots;
}simlock_rsu_config_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_AUTH_DATA_TYPE

   DESCRIPTION:
     This structure contains the authentication parameters for the configuration
     data

     msg_len          : Length of the message
     msg_data_ptr     : Pointer to the message to be authenticated
     hmac             : The HMAC associated with the blob
-------------------------------------------------------------------------------*/
typedef struct
{
  uint16                                  msg_len;
  uint8                                  *msg_data_ptr;
  union {
    simlock_hmac_signature_type           hmac;
    simlock_rsa_signature_type            rsa;
  } data;
}simlock_rsu_config_auth_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_RSU_CONFIG_DATA_TYPE

   DESCRIPTION:
     This structure contains the configuration data

     category_header  : header data
     code_data        : code data
-------------------------------------------------------------------------------*/
typedef struct
{
  simlock_rsu_config_header_data_type    header_data;
  simlock_rsu_config_code_data_type      code_data;
  simlock_imei_type                      imei;
  simlock_rsu_config_auth_data_type      auth_data;
} simlock_rsu_config_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SIMLOCK_REMOTE_LOCK_TIMESTAMP_TYPE

   DESCRIPTION:
     This structure contains the time data related to the last processed
     request. Time in milliseconds.

     version             : File version
     timestamp           : Time stamp when blob is created
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint8     version;
  uint64    timestamp;
} simlock_remote_lock_timestamp_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

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
);

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
boolean simlock_rsu_config_is_lock_allowed(
  const simlock_message_request_data_type *req_data_ptr
);

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
);

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */
#endif /* SIMLOCK_RSU_CONFIG_H */

