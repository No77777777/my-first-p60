#ifndef SERVREG_UTILS_H
#define SERVREG_UTILS_H
/*
#============================================================================
#  Name:
#    servreg_utils.h 
#
#  Description:
#     Utility API header file for Service Registry feature
#
# Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdlib.h"
#include "servreg_common.h"
#include "servreg_internal.h"
#include "DALSys.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define SERVREG_NAME_DELIMITER  "/"
#define SERVREG_NAME_MAX_LENGTH  64
#define SERVREG_ROOT_PD_NAME  "root_pd"

#define SERVREG_RCESN_NAME_PREFIX          "sr:"
#define SERVREG_RCESN_NAME_PREFIX_LEN       3

#define SERVREG_SYNC_NAME_UP_PREFIX         "sr_up:"
#define SERVREG_SYNC_NAME_DOWN_PREFIX       "sr_dn:"
#define SERVREG_SYNC_NAME_PREFIX_LEN        6

#define SERVREG_SSR_NAME_PREFIX             "ssr:"
#define SERVREG_SSR_NAME_PREFIX_LEN         4
#define SERVREG_SSR_NAME_DOWN_SUFFIX        ":before_shutdown"
#define SERVREG_SSR_NAME_DOWN_SUFFIX_LEN    16
#define SERVREG_SSR_NAME_UP_SUFFIX          ":after_powerup"
#define SERVREG_SSR_NAME_UP_SUFFIX_LEN      14

/** =====================================================================
 * Function:
 *     servreg_utils_return_string_between_delimiter
 *
 * Description:
 *     It will return substring between delimiter for a given string.
 *
 * Parameters:
 *     str : String, from which , substring will be extracted
 *     delim_start : position of first delimiter
 *     delim_end : position of second delimiter
 *     delim : delimiter, which is used to divide string
 *
 * Returns:
 *     SERVREG_NULL : error case OR not able to extract substring
 *                    otherwise required substring
 *
 * Note : Caller should free the memory which will be allocated in this API
 * =====================================================================  */
SERVREG_NAME servreg_utils_return_string_between_delimiter(SERVREG_NAME str, uint32_t delim_start, uint32_t delim_end, SERVREG_NAME delim);

/** =====================================================================
 * Function:
 *     servreg_calculate_relationship
 *
 * Description:
 *     It will return relationship between service_name and local process.
 *     Please refer servreg_relationship_type enum to understand relationship types.
 *     Process indicates "soc/domain/subdomain". We will extract "Process"
 *     from servreg_name and caluclate its relationship with Local "Process".
 *     Local indicates "where this code is executing".
 *     Example : if we are executing this code in Audio PD then local process
 *               will be "msm/adsp/audio_pd" and if we are executing in modem 
 *               Root PD then local process will be "msm/modem/root_pd".
 *
 * Parameters:
 *     service_name : Service for which we need to calculate its relationship
 *                    with Local Process
 *     relation  :    Calculated Relationship will be saved in it.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_calculate_relationship(SERVREG_NAME service_name, servreg_relationship_type *relation);

/** =====================================================================
 * Function:
 *     servreg_name_check
 *
 * Description:
 *     This function checks if the given name is a valid service name or not.
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     SERVREG_SUCCESS       : If name is valid
 *     SERVREG_INVALID_PARAM : If name is invalid
 * =====================================================================  */
SERVREG_RESULT servreg_name_check(SERVREG_NAME domain, SERVREG_NAME service);

/** =====================================================================
 * Function:
 *     servreg_create_rcesn_name
 *
 * Description:
 *     This function takes in the service name and returns the rcesn sync name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     rcecb sync name : "sr:soc/domain/subdomain/provider/service" or just "sr:soc/domain/subdomain" 
 * =====================================================================  */
SERVREG_NAME servreg_create_rcesn_name(SERVREG_NAME name);

/** =====================================================================
 * Function:
 *     servreg_create_sync_name
 *
 * Description:
 *     This function takes in the service name and returns the rcecb sync name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     rcecb sync name : "sr_xx:soc/domain/subdomain/provider/service" or just "sr_xx:soc/domain/subdomain" 
 *                        sr_xx : sr_up or sr_down supported for now
 * =====================================================================  */
SERVREG_NAME servreg_create_sync_name(SERVREG_NAME name, SERVREG_SERVICE_STATE state);

/** =====================================================================
 * Function:
 *     servreg_create_ssr_name
 *
 * Description:
 *     This function takes in the service name and returns the before shutdown ssr message name
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *     state : state of the service. UP or DOWN
 *
 * Returns:
 *     ssr name : "ssr:####:before_shutdown" or "ssr:####:after_powerup" #### will be the name extracted from the input param domain
 * =====================================================================  */
SERVREG_NAME servreg_create_ssr_name(SERVREG_NAME domain, SERVREG_SERVICE_STATE state);

/** =====================================================================
 * Function:
 *     servreg_concat
 *
 * Description:
 *     This functions concatenates "soc/domain/subdomain" and "provider/service"
 *     string
 *
 * Parameters:
 *     domain : "soc/domain/subdomain" info
 *     service : "provider/service" info
 *
 * Returns:
 *     concatenated name : "soc/domain/subdomain/provider/service"
 * =====================================================================  */
SERVREG_NAME servreg_concat(SERVREG_NAME domain, SERVREG_NAME service);

/** =====================================================================
 * Function:
 *     servreg_nmelen
 *
 * Description:
 *     Function to find out the string length
 *
 * Parameters:
 *     name : string
 *
 * Returns:
 *     int : string length
 * =====================================================================  */
int servreg_nmelen(SERVREG_NAME const name);

/** =====================================================================
 * Function:
 *     servreg_nmecmp
 *
 * Description:
 *     Compares two strings
 *
 * Parameters:
 *     name_1 : string 1
 *     name_2 : string 2
 *     len    : length of the string
 *
 * Returns:
 *     -1, 0 or 1
 * =====================================================================  */
int servreg_nmecmp(SERVREG_NAME const name_1, SERVREG_NAME const name_2, int len);

/** =====================================================================
 * Function:
 *     servreg_nmehash
 *
 * Description:
 *     Function calculated the hash value given a name
 *
 * Parameters:
 *     name : name given to calculate the hash value
 *
 * Returns:
 *     hash : hash value calculated for the given name
 * =====================================================================  */
servreg_hash_t servreg_nmehash(SERVREG_NAME const name, uint32_t size);

/** =====================================================================
 * Function:
 *     servreg_is_valid_state_change
 *
 * Description:
 *     This functions check if state change is valid or not
 *
 * Parameters:
 *     sr_mon_handle
 *     new state
 *     is_valid
 *
 * Returns:
 *     SERVREG_SUCCESS : if state change is valid 
 *     else Refer to the enum SERVREG_RESULT for list of possible results 
 * =====================================================================  */
SERVREG_RESULT servreg_is_valid_state_change(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE new_state, SERVREG_BOOL *is_valid);

#if defined(__cplusplus)
}
#endif

#endif
