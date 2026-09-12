#ifndef SIMLOCK_UTIL_H
#define SIMLOCK_UTIL_H
/*===========================================================================


            S I M   L O C K   U T I L   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_util.h#3 $$ $DateTime: 2019/12/23 02:54:10 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/23/19   tq      Check wild digit only for lock data 
12/03/18   vgd     Validate input lock data of RSU before applying lock
12/03/18   vgd     Remote SIM Unlock support for SIMLOCK_RSU_MODE_D
05/07/18   dd      Remote SIM Unlock support to switch configuration
08/11/17   av      Pass structure by reference to avoid compiler issues
07/13/17   nr      Remote SIM Unlock supported in common build
07/13/17   av      Enhanced simlock RSU solution
04/16/17   bcho    Set perso status properly if sub is locked due to other app
01/26/17   bcho    Support for slot policy valid card must on all slots
01/19/17   ar      Modify wild digit support for GID in SP-EHPLMN category
11/03/16   bcho    Added support for ICCID simlock
10/06/16   ar      Added support for SP + EHPLMN simlock
08/31/16   vdc     Use correct IMEI in OTA unlock and RSU blob validation
08/03/16   vdc     Added support for SPN based SIMLOCK feature
05/11/16   av      Skip checking simlock policies when no category is enabled
01/22/16   av      Support for autolock whitelist
08/19/15   vv      Added support to retrieve retry attempts left for locking
05/15/15   vv      Support for RSU set configuration in simlock
02/17/15   vv      Indicate the reason for invalid subscription
01/06/15   vv      Support for identifying test SIMs
12/30/14   bcho    Support for more then 1 bytes of GID1/GID2
12/17/14   vv      Relocate the simlock data files
11/10/14   vv      Add support for setting emergency mode for all the RATs
09/29/14   vv      Added support for the remote SFS
09/29/14   vv      Added support for emergency only mode
05/12/14   vv      Fix compiler errors
04/14/14   vv      Added support for identifying conflicting codes
01/07/14   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_SIMLOCK

#include "simlock.h"
#include "simlock_config.h"
#include "simlock_file.h"
#include "simlock_modem_p.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define simlock_util_htonll(x)                                            \
  (((((uint64)(x) & 0x00000000000000FFULL) << 56)|                       \
  (((uint64)(x) & 0x000000000000FF00ULL) << 40)   |                       \
  (((uint64)(x) & 0x0000000000FF0000ULL) << 24)   |                       \
  (((uint64)(x) & 0x00000000FF000000ULL) << 8)    |                       \
  (((uint64)(x) & 0x000000FF00000000ULL) >> 8)    |                       \
  (((uint64)(x) & 0x0000FF0000000000ULL) >> 24)   |                       \
  (((uint64)(x) & 0x00FF000000000000ULL) >> 40)   |                       \
  (((uint64)(x) & 0xFF00000000000000ULL) >> 56)))

#define SIMLOCK_IS_VALID_CODE_CHAR(c)              \
  ((((c) >= '0' && (c) <= '9') || ((c) == SIMLOCK_LOCK_CODE_WILD_CHAR)) ? TRUE : FALSE)

/* ----------------------------------------------------------------------------
   STRUCTURE:      simlock_category_details_type

   DESCRIPTION:
     This structure contains category and status
------------------------------------------------------------------------------*/
typedef struct {
  boolean                      status;
  simlock_category_enum_type   category;
} simlock_category_details_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_UTIL_COMPOSE_CATEGORY_FILE_ENUM

DESCRIPTION
  This is a utility function that composes the category file enum for a
  category on a slot

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_data_file_enum_type simlock_util_compose_category_file_enum
(
  simlock_category_enum_type category,
  simlock_slot_enum_type     slot
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_FILE_PATH_FROM_ENUM

DESCRIPTION
  This is a utility function that return the category file corresponding to
  the category file enum

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_get_file_path_from_enum
(
  simlock_data_file_enum_type  file_enum,
  simlock_filename_type        file_name
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_CODE_DATA

DESCRIPTION
  This is a utility function that checks if the lock codes are present

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_check_code_data
(
  const simlock_category_data_type * category_data_ptr,
  boolean                            auto_lock
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_CATEGORY_SUPPORTED

DESCRIPTION
  This is a utility function that checks if a category is supported

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_check_category_supported
(
  simlock_category_enum_type  category_type
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_SLOT

DESCRIPTION
  This is a utility function that checks if a slot is supported

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_check_slot
(
  simlock_slot_enum_type  slot
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_MCC_MATCHES

DESCRIPTION
  This is a utility function that checks if the MCCs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MCCs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_mcc_matches
(
  const simlock_mcc_type  lock_mcc,
  const simlock_mcc_type  sim_mcc,
  boolean                 is_lock_data
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_MNC_MATCHES

DESCRIPTION
  This is a utility function that checks if the MNCs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MCCs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_mnc_matches
(
  const simlock_mnc_type  *lock_mnc_ptr,
  const simlock_mnc_type  *sim_mnc_ptr,
  boolean                  is_lock_data
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_MSIN_MATCHES

DESCRIPTION
  This is a utility function that checks if the MSINs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MSINs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_msin_matches
(
  const simlock_msin_type   *lock_msin_ptr,
  const simlock_msin_type   *sim_msin_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_IRM_MATCHES

DESCRIPTION
  This is a utility function that checks if the IRMs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the IRMs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_irm_matches
(
  const simlock_msin_type   *lock_msin_ptr,
  const simlock_msin_type   *sim_msin_ptr,
  boolean                    is_lock_data
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_GID_MATCHES

DESCRIPTION
  This is a utility function that checks if the GIDs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the GIDs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_gid_matches
(
  const simlock_gid_type              *lock_gid_ptr,
  const simlock_gid_type              *sim_gid_ptr,
  boolean                             wild_digit_support,
  const boolean                       is_lock_data
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_IMSI_DIGIT_MATCHES

DESCRIPTION
  This is a utility function that checks if the IMSI digits (d6 or d7)
  match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the GIDs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_imsi_digit_matches
(
  char        lock_digit,
  char        sim_digit,
  boolean     is_lock_data
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_SPN_MATCHES

DESCRIPTION
  This is a utility function that checks if the SPNs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the SPNs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_spn_matches
(
  const simlock_spn_type *lock_spn_ptr,
  const simlock_spn_type *sim_spn_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_ICCID_MATCHES

DESCRIPTION
  This is a utility function that checks if the ICCIDs match

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the ICCIDs match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_iccid_matches
(
  const simlock_iccid_type *lock_iccid_ptr,
  const simlock_iccid_type *sim_iccid_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_SLOT_INDEX

DESCRIPTION
  This is a utility function that gets the slot index corresponding to a
  slot id

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_get_slot_index
(
  simlock_slot_enum_type    slot,
  uint8                   * slot_index
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_CATEGORY_INDEX

DESCRIPTION
  This is a utility function that gets the category index corresponding to a
  category

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type  simlock_util_get_category_index
(
  simlock_category_enum_type     category,
  uint8                       *  category_index
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_CATEGORY_ENABLED

DESCRIPTION
  This is a utility function that checks if a category is enabled

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE  : If the category for the slot is enabled
  FALSE : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_category_enabled
(
  const simlock_config_data_type   * config_data_ptr,
  simlock_category_enum_type         category,
  simlock_slot_enum_type             slot
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_MCC_VALID

DESCRIPTION
  This is a utility function that checks if the MCC is valid
  The acceptable MCC digits are 0 - 9 and '*' ASCII characters

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MCC is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_mcc_valid
(
  const simlock_mcc_type  lock_mcc
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_MNC_VALID

DESCRIPTION
  This is a utility function that checks if the MNC is valid
  The acceptable MNC digits are 0 - 9 and '*' ASCII characters

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MNC is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_mnc_valid
(
  const simlock_mnc_type  *lock_mnc_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_MSIN_VALID

DESCRIPTION
  This is a utility function that checks if the MSIN is valid
  The acceptable MSIN digits are 0 - 9 and '*' ASCII characters

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the MSIN is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_msin_valid
(
  const simlock_msin_type  *lock_msin_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_SPN_VALID

DESCRIPTION
  This is a utility function that checks if the SPN is valid

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the SPN is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_spn_valid
(
  const simlock_spn_type *spn_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_IRM_VALID

DESCRIPTION
  This is a utility function that checks if the IRM is valid
  The acceptable IRM digits are 0 - 9 and '*' ASCII characters

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the IRM is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_irm_valid
(
  const simlock_irm_type  lock_irm
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_ICCID_VALID

DESCRIPTION
  This is a utility function that checks if the ICCID is valid
  The acceptable ICCID digits are 0 - 9 and '*' ASCII characters

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the ICCID is valid
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_is_iccid_valid
(
  const simlock_iccid_type  *lock_iccid_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IF_CODES_MATCH

DESCRIPTION
  This function checks if the code from a category that needs to be enabled
  matches with the code in the already enabled category.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If the codes match
  FALSE: Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_if_codes_match
(
  const simlock_category_code_data_type   * enabled_code_data_ptr,
  const simlock_category_code_data_type   * lock_code_data_ptr,
  simlock_category_enum_type                enabled_category,
  simlock_category_enum_type                lock_category
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_IS_DEVICE_IN_EMERGENCY_MODE

DESCRIPTION
  This is a utility function that checks if the device is in the emergency
  only mode

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE  : If the device is in the emergency mode
  FALSE : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_is_device_in_emergency_mode
(
  const simlock_config_data_type   * config_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_FILE_ENUM_TO_FILE_TYPE

DESCRIPTION
  This is a utility function that converts a simlock data file to file type
  (config or category file), slot and category

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_file_enum_to_file_type
(
  simlock_data_file_enum_type    data_file_enum,
  simlock_file_type            * file_type_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_DEEP_COPY

DESCRIPTION
  The function allocates memory and does a deep copy. The caller of this
  function is responsible to de-allocate the memory

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void simlock_util_deep_copy
(
  void       ** dest_pptr,
  const void  * src_ptr,
  uint32        len
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GENERATE_REQUEST_TOKEN

DESCRIPTION
  This function generates a token to be used in a request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  simlock_token_id_type

SIDE EFFECTS
  None
===========================================================================*/
simlock_token_id_type simlock_util_generate_request_token
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IS_TEST_MCC_MNC

DESCRIPTION
  This function checks if the MCC/MNC in the code data matches with the
  test MCC/MNCs

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  TRUE   : if the MCC/MNC in the code data matches one of the test MCC/MNCs
  FALSE  : if otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean simlock_util_check_is_test_mcc_mnc
(
  simlock_mcc_type               sim_mcc,
  const simlock_mnc_type         *sim_mnc_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_CK_STATUS

DESCRIPTION
  This is a utility function that checks if a category is enabled with the
  CK or not

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_check_ck_status
(
  const simlock_config_data_type   * config_data_ptr,
  simlock_category_enum_type         category,
  simlock_slot_enum_type             slot,
  simlock_ck_present_type         *  ck_status_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_IMEI_MATCHES

DESCRIPTION
  This function checks if the IMEIs provided matches

DEPENDENCIES

LIMITATIONS
  NONE

RETURN VALUE
  TRUE : If both the IMEIs match
  FALSE: If otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_imei_matches(
  const simlock_imei_type    device_imei,
  const simlock_imei_type    req_imei
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_IMEI

DESCRIPTION
  This function reads the context specific IMEI from the NV and provides
  it in a ASCII format

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_get_imei(
  simlock_slot_enum_type    slot,
  simlock_imei_type       * imei_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_MEID

DESCRIPTION
  This function reads the device MEID from the NV and provides it in a ASCII
  format

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_util_get_meid(
  simlock_slot_enum_type    slot,
  simlock_meid_type       * meid_data_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_CHECK_CATEGORY_DISABLED_FILE_PRESENT

DESCRIPTION
  This is a utility function that checks if a category is disabled but
  the corresponding file is present

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  TRUE  : If the category for the slot is disabled but the file is present
  FALSE : Otherwise

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_check_category_disabled_file_present
(
  const simlock_config_data_type   * config_data_ptr,
  simlock_category_enum_type         category,
  simlock_slot_enum_type             slot
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_ALL_CATEGORIES_DISABLED

DESCRIPTION
  This function returns TRUE in if all the categories are either disabled or
  are temporarily unlocked. FALSE otherwise.

  ignore_temporary_unlock indicates if the function should consider the
  temporary unlock or not. When temporary unlock is considered, locked
  categories with temporary unlock will be considered as unlocked.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_all_categories_disabled
(
  simlock_slot_enum_type                    slot,
  const simlock_config_data_type          * config_data_ptr,
  simlock_category_details_type           * locked_3gpp_category_details_ptr,
  simlock_category_details_type           * locked_3gpp2_category_details_ptr,
  boolean                                   ignore_temporary_unlock
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_RETRY_COUNT

DESCRIPTION
  This function returns current retries and max retries for requested category
  if it is enabled.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  boolean

SIDE EFFECTS
  NONE
===========================================================================*/
boolean simlock_util_get_retry_count
(
  const simlock_config_data_type          * config_data_ptr,
  simlock_category_enum_type                category,
  simlock_slot_enum_type                    slot,
  uint32                                  * curr_retries_ptr,
  uint32                                  * num_retries_max_ptr
);

/*===========================================================================
FUNCTION SIMLOCK_UTIL_GET_NUMBER_OF_SLOTS

DESCRIPTION
  This function returns number of activer HW slots supported.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  uint8

SIDE EFFECTS
  NONE
===========================================================================*/
uint8 simlock_util_get_number_of_slots
(
  void
);

#endif /* FEATURE_SIMLOCK */

#endif /* SIMLOCK_UTIL_H */

