#ifndef UIMDIAG_SIMLOCK_H
#define UIMDIAG_SIMLOCK_H

/*===========================================================================


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2020 by QUALCOMM Technologies, Inc (QTI).
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who   what, where, why
--------   ---   -----------------------------------------------------------
07/23/20   cj    Added new UIMDIAG command UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD
02/10/20   cj    Fixed compilation issue of declaration of SIMLOCK_MIN_GID_LEN
01/24/20   cj    Added check for min length of GID 
01/23/20   sg    Correct the UIMDIAG NS+SP lock category enum value as per spec
12/28/19   sg    Added support for NS+SP category
09/10/19   vdc   SimLock solution for activation at Point Of Sale
12/03/18   vgd   Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
07/13/17   nr    Remote SIM Unlock supported in common build
02/20/17   vdc   Added support to probe CK for correctness
12/02/16   vdc   Added support to check RSA public key for remote lock
11/03/16   bcho  Added support for ICCID simlock
10/06/16   ar    Added support for SP + EHPLMN simlock
08/03/16   vdc   Added support for SPN based SIMLOCK feature
03/11/16   ar    Remove UIMDIAG code using Stubs
07/10/15   stv   Deprecate UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_CMD
12/30/14   bcho  Support for more then 1 bytes of GID1/GID2
12/26/14   vv    Support for whitelist and blacklist codes in same category
06/04/14   vv    Remove FEATURE_SIMLOCK
02/24/14   tl    Initial Revision


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uimdiag.h"
#include "diagpkt.h"

#include "simlock_common.h"
#include "simlock_modem_lib.h"

/*=============================================================================

                   ENUMERATED DATA DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   ENUM:      UIMDAIG_SIMLOCK_CATEGORY_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the SIMLock categories
-------------------------------------------------------------------------------*/
typedef enum {
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_NW        = 0,  /* Network Personalization Feature              */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS        = 1,  /* Network Subset Personalization Feature       */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP        = 2,  /* Service Provider Personalization Feature     */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP        = 3,  /* Corporate Personalization Feature            */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SIM       = 4,  /* SIM/USIM Personalization Feature             */

  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE1 = 5,  /* Network1 Personalization Feature             */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_NW_TYPE2 = 6,  /* Network2 Personalization Feature             */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_HRPD     = 7,  /* HRPD Personalization Feature                 */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_SP       = 8,  /* Service Provider Personalization Feature     */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_CP       = 9,  /* Corporate Personalization Feature            */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP2_RUIM     = 10, /* RUIM Personalization Feature                 */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EXT    = 11, /* Service Provider Personalization Ext Feature */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_CP_EXT    = 12, /* Corporate Personalization Ext Feature        */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SPN       = 13, /* Service Provider Name Personalization Feature*/
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_SP_EHPLMN = 14, /* SP category along with EHPLMN data feature    */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_ICCID     = 15,  /* ICCID feature                                 */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_IMPI      = 16, /* IMPI feature                                 */
  UIMDIAG_SIMLOCK_CATEGORY_3GPP_NS_SP     = 17  /* NS category along with SP Feature            */
} uimdiag_simlock_category_enum_type;


/*=============================================================================

                       DEFINES DECLARATIONS

=============================================================================*/

#define UIMDIAG_SIMLOCK_DATA_BLOCK_LEN         20

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_category_info_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               category;
  uint8                               category_status;
  uint8                               blacklist;
  uint32                              num_retries_max;
  uint32                              curr_retries;
} uimdiag_simlock_category_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_lock_info_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uimdiag_simlock_category_info_type  category_info[SIMLOCK_CATEGORY_COUNT_MAX];
} uimdiag_simlock_lock_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_get_status_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  simlock_slot_policy_enum_type            slot_policy;
  simlock_subscription_policy_enum_type    subscription_policy;
  uimdiag_simlock_lock_info_type           lock_info[SIMLOCK_SLOT_COUNT_MAX];
} uimdiag_simlock_get_status_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_unlock_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint32    num_retries_max;
  uint32    curr_retries;
} uimdiag_simlock_unlock_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_token_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8     simlock_token[SIMLOCK_TOKEN_LEN];
} uimdiag_simlock_token_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_get_rsu_mode_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  simlock_rsu_mode_enum_type get_rsu_mode;
} uimdiag_simlock_get_rsu_mode_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_nw_code_data_type

   DESCRIPTION:
     This structure consists of MCC and MNC in ASCII.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8    mcc[SIMLOCK_MCC_LEN];
  uint8    mnc_length;
  uint8    mnc[SIMLOCK_MNC_MAX];
} uimdiag_simlock_nw_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_ns_code_data_type

   DESCRIPTION:
     This structure consists of network subset code data
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uimdiag_simlock_nw_code_data_type   nw_code;
  uint8                               imsi_digit6;
  uint8                               imsi_digit7;
} uimdiag_simlock_ns_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_sp_code_data_type

   DESCRIPTION:
     This structure consists of service provider code data
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uimdiag_simlock_nw_code_data_type   nw_code;
  uint8                               gid1;
} uimdiag_simlock_sp_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_cp_code_data_type

   DESCRIPTION:
     This structure consists of corporate provider code data
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uimdiag_simlock_nw_code_data_type   nw_code;
  uint8                               gid1;
  uint8                               gid2;
} uimdiag_simlock_cp_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_sim_code_data_type

   DESCRIPTION:
     This structure consists of sim code data
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uimdiag_simlock_nw_code_data_type  nw_code;
  uint8                              imsi_msin_len;
  uint8                              imsi_msin[SIMLOCK_MSIN_MAX];
} uimdiag_simlock_sim_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_nw_type2_code_data_type

   DESCRIPTION:
     This structure consists of irm code data in ASCII
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                              irm[SIMLOCK_IRM_LEN];
} uimdiag_simlock_nw_type2_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_nw_code_list_type

   DESCRIPTION:
     This structure consists of list of MCCs and MNCs
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               num_nw_codes;
  uimdiag_simlock_nw_code_data_type   nw_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_nw_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_ns_code_list_type

   DESCRIPTION:
     This structure consists of list of network subset codes.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               num_ns_codes;
  uimdiag_simlock_ns_code_data_type   ns_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_ns_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_sp_code_list_type

   DESCRIPTION:
     This structure consists of list of service provider codes.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               num_sp_codes;
  uimdiag_simlock_sp_code_data_type   sp_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_sp_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_cp_code_list_type

   DESCRIPTION:
     This structure consists of list of corporate provider codes.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               num_cp_codes;
  uimdiag_simlock_cp_code_data_type   cp_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_cp_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_sim_code_list_type

   DESCRIPTION:
     This structure consists of list of sim codes.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                                num_sim_codes;
  uimdiag_simlock_sim_code_data_type   sim_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_sim_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      uimdiag_simlock_nw_type2_code_list_type

   DESCRIPTION:
     This structure consists of list of network type2 codes.
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                                     num_nw_type2_codes;
  uimdiag_simlock_nw_type2_code_data_type   nw_type2_code_data[SIMLOCK_NUM_CODES_MAX];
} uimdiag_simlock_nw_type2_code_list_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_get_category_data_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                                      category_type;
  PACKED union PACKED_POST{
    /* 3GPP */
    uimdiag_simlock_nw_code_list_type        nw_3gpp_code_list;
    uimdiag_simlock_ns_code_list_type        ns_3gpp_code_list;
    uimdiag_simlock_sp_code_list_type        sp_3gpp_code_list;
    uimdiag_simlock_cp_code_list_type        cp_3gpp_code_list;
    uimdiag_simlock_sim_code_list_type       sim_3gpp_code_list;

    /* 3GPP2 */
    uimdiag_simlock_nw_code_list_type        nw_type1_3gpp2_code_list;
    uimdiag_simlock_nw_type2_code_list_type  nw_type2_3gpp2_code_list;
    uimdiag_simlock_sim_code_list_type       ruim_3gpp2_code_list;
  } code_data;
} uimdiag_simlock_get_category_data_rsp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_code_data_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8            code[UIMDIAG_SIMLOCK_DATA_BLOCK_LEN];
} uimdiag_simlock_code_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:              uimdiag_simlock_get_category_data_ext_rsp_type

   DESCRIPTION:
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8                               category_type;
  uint8                               num_of_codes;
  uimdiag_simlock_code_data_type      code_data;
} uimdiag_simlock_get_category_data_ext_rsp_type;

/*=============================================================================

                       FUNCTION DECLARATIONS

=============================================================================*/

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_SET_POLICY_CMD

DESCRIPTION
  This function sets the slot and subscription policy in the SIM Lock feature
  and stores the information in the SFS.

DEPENDENCIES
  This can only be processed when all SIM Lock categories are disabled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_set_policy_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_HCK_LOCK_CMD

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature
  will then be locked to the device to an HCK which is generated by
  the carrier using a randomly generated SALT key and the original
  control key stored by the carrier.
 
  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_hck_lock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_CK_LOCK_CMD

DESCRIPTION
  This function allows the client to provision one SIM Lock feature
  with up to 20 sets of that feature data. The SIM Lock feature will then be locked
  to the device with the CK passed to the client by the carrier.
 
  In addition, an autolock indicator, blacklist indicator, maximum
  unlock attempt indicator and key generation iteration count indicator
  will be stored along with the security data in the SFS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_ck_lock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_UNLOCK_CMD

DESCRIPTION
  This function allows the user to unlock and disable one SIMLOCK feature on an
  identified slot.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_unlock_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_STATUS_CMD

DESCRIPTION
  This function retrieves all relavent SIMLOCK information and configurations
  across all slots and subscription applications including the slot and
  subscription policies.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_get_status_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_RSU_MODE_CMD

DESCRIPTION
  This function retrieves the RSU mode

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_get_rsu_mode_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);


/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD

DESCRIPTION
  This function adds the lock codes for one SIM Lock feature if the CK
  matches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_add_lock_codes_ck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_ADD_LOCK_CODES_HCK_CMD

DESCRIPTION
  This function adds the lock codes for one SIM Lock feature if the HCK
  matches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_add_lock_codes_hck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================

FUNCTION UIMDIAG_SIMLOCK_GET_CATEGORY_DATA_EXT_CMD

DESCRIPTION
  This function retrieves the SIM Lock category data for one SIM Lock feature.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
PACKED void * uimdiag_simlock_get_category_data_ext_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_REMOTE_LOCK_CHECK_PUBLIC_KEY_CMD

DESCRIPTION
  This function checks if the public key is matched against the public key which
  is configured in code or EFS (key in code takes priority)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_remote_lock_check_public_key_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_PROBE_CK_CMD

DESCRIPTION
  This function checks if the input CK matches with the locked CK for a category

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_probe_ck_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_MODE_CONFIG_CMD

DESCRIPTION
  This function configures build config according to the input received from
  diag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_mode_config_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_DEVICE_KEY_CONFIG_CMD

DESCRIPTION
  This function provision device key config according to the input received from
  diag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_device_key_config_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

/*===========================================================================
FUNCTION UIMDIAG_SIMLOCK_SET_CONFIGURATION_CMD

DESCRIPTION
  This function sets config id and config nickname for subsequent UIMDIAG
  SIMLOCK commands. Config id and config nickname corresponds to operator for
  which inactive configuration and category files will be created for below
  UIMDIAG SIMLOCK commands.

  UIMDIAG_SIMLOCK_HCK_LOCK_CMD
  UIMDIAG_SIMLOCK_HCK_LOCK_EXT_CMD
  UIMDIAG_SIMLOCK_CK_LOCK_CMD
  UIMDIAG_SIMLOCK_CK_LOCK_EXT_CMD 
  UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_CMD
  UIMDIAG_SIMLOCK_ADD_LOCK_CODES_CK_EXT_CMD
  UIMDIAG_SIMLOCK_SET_POLICY_CMD

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_simlock_set_configuration_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
);

#endif /* UIMDIAG_SIMLOCK_H */
