#ifndef TMS_SERVREG_H
#define TMS_SERVREG_H
/*
#============================================================================
#  Name:
#    tms_servreg.h 
#
#  Description:
#    Common header file for TMS Service Registry features
#
# Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdlib.h"
#include "servreg_common.h"
#include "tms_dll_api.h"

typedef enum
{
  SOC = 0x1,
  DOMAIN = 0x2,
  SUBDOMAIN = 0x4,
  PROVIDER = 0x8,
  SERVICE = 0x10
}servreg_string_type;

#if defined(__cplusplus)
extern "C"
{
#endif

/** =====================================================================
 * Function:
 *     servreg_get_pending_acks
 *
 * Description:
 *     Function gets the pending ack count for the service state
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : The service state for which the pending ack is requested
 * Returns:
 *     Ack count
 * =====================================================================  */
uint32_t servreg_get_pending_acks(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state);

/** =====================================================================
 * Function:
 *     servreg_get_service_name
 *
 * Description:
 *     Given the monitor handle, the function returns the service name 
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     service_name or SERVREG_NULL
 * =====================================================================  */
SERVREG_NAME servreg_get_service_name(SERVREG_MON_HANDLE sr_mon_handle);

/** =====================================================================
 * Function:
 *     servreg_get_pd_name
 *
 * Description:
 *     This function returns the pd name i.e format "soc/domain/subdomain".
 *     Memory is allocated for the string returned.
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     	pd name of the format "soc/domain/subdomain"
 * =====================================================================  */
SERVREG_NAME servreg_get_pd_name(SERVREG_MON_HANDLE sr_mon_handle);

/** =====================================================================
 * Function:
 *     servreg_loc_send_pfr
 *
 * Description:
 *     Send the PD failure reason
 *
 * Parameters:
 *     pd_name:     pointer to pd_name
 *     pd_name_len: pd name length
 *     pfr:         pointer to pd failure reason
 *     pfr_len:     pd failure reason string length
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_loc_send_pfr(char * pd_name, int pd_name_len, char * pfr, int pfr_len);

/** =====================================================================
 * Function:
 *     servreg_utils_get_string
 *
 * Description:
 *     It will return substring based on val passed.
 *
 * Parameters:
 *     str : String, from which , substring will be extracted
 *     val : combination of servreg_string_type enum values ,
 *           Contiguous bits should be populated
 *
 * Returns:
 *     SERVREG_NULL : error case OR not able to extract substring
 *                    otherwise required substring
 *
 * Note : Caller should free the memory which will be allocated in
 *        servreg_utils_return_string_between_delimiter() API
 * =====================================================================  */
SERVREG_NAME servreg_utils_get_string(SERVREG_NAME str, uint32_t val);

/** =====================================================================
 * Function:
 *     servreg_register_sync_notif_cb, servreg_deregister_sync_notif_cb
 *
 * Description:
 *     (De)Register with a callback function to get sync notification for that service state
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : The service state for which the client wants to get the sync notification
 *                     Should preferably be SERVREG_SERVICE_STATE_DOWN or SERVREG_SERVICE_STATE_UP
 *     callback      : Callback function registered
 *     cb_p1         : Callback function parameter. Can be set to sr_mon_handle
 *     cb_p2         : Callback function parameter 2. Can be set to the service state.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_register_sync_notif_cb(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state, SERVREG_FNSIG_CALLBACK const callback, SERVREG_CB_PARAM const cb_p1, SERVREG_CB_PARAM const cb_p2);

SERVREG_RESULT servreg_deregister_sync_notif_cb(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state, SERVREG_FNSIG_CALLBACK const callback, SERVREG_CB_PARAM const cb_p1, SERVREG_CB_PARAM const cb_p2);

#if defined(__cplusplus)
}
#endif

#endif
