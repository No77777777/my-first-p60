#ifndef SERVREG_LOCATOR_INTERNAL_H
#define SERVREG_LOCATOR_INTERNAL_H
/*
#============================================================================
#  Name:
#    servreg_locator_internal.h 
#
#  Description:
#     Common header file for Service Registry feature
#
# Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdlib.h"
#include "servreg_common.h"
#include "servreg_internal.h"
#include "servreg_utils.h"
#include "DALSys.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/* Domain list entry structure */
struct servreg_dl_entry_s
{
   SERVREG_NAME domain_name;
   uint32_t instance_id;
   uint8_t service_data_valid;
   uint32_t service_data;
   struct servreg_dl_entry_s * next;
};
typedef struct servreg_dl_entry_s servreg_dl_entry_t, * servreg_dl_entry_p;

/* Domain list table structure */
struct servreg_dl_table_s
{
   uint32_t domain_list_signature;
   SERVREG_NAME service_name;
   SERVREG_RESULT domain_list_status;
   uint16_t total_domains;
   uint16_t db_rev_count;
   struct servreg_dl_entry_s * domain_list_head;
   struct servreg_dl_table_s * next;
};
typedef struct servreg_dl_table_s servreg_dl_table_t, * servreg_dl_table_p;

/* Type casts as accessor functions */
#define sr_dl_table2sr_dl_handle(x)    ((SERVREG_DL_HANDLE)x)
#define sr_dl_handle2sr_dl_table(x)    ((servreg_dl_table_p)x)

struct servreg_pd_list_s
{
   SERVREG_NAME pd_name;
   struct servreg_pd_list_s *next;
};

typedef struct servreg_pd_list_s servreg_pd_list_t, *servreg_pd_list_p;

typedef enum
{
   SERVREG_LOC_INDICATION_REGISTER_REQ      = QMI_SERVREG_LOC_INDICATION_REGISTER_REQ_V01,
   SERVREG_LOC_GET_DOMAIN_LIST_REQ          = QMI_SERVREG_LOC_GET_DOMAIN_LIST_REQ_V01,
   SERVREG_LOC_REGISTER_SERVICE_LIST_REQ    = QMI_SERVREG_LOC_REGISTER_SERVICE_LIST_REQ_V01,
   SERVREG_LOC_DATABASE_UPDATED_IND         = QMI_SERVREG_LOC_DATABASE_UPDATED_IND_V01,
   SERVREG_LOC_PFR_REQ                      = QMI_SERVREG_LOC_PFR_RESP_V01,   
}servreg_locator_msg_req_type;

#define SERVREG_LOC_REQ_TIMEOUT_MS           3000

/** =====================================================================
 * Function:
 *     servreg_dl_table_alloc
 *
 * Description:
 *     Allocates the servreg domainlist handle
 *
 * Parameters:
 *     None
 *
 * Returns:
 *    servreg_dl_table_p : Dynamically allocated servreg domainlist handle
 * =====================================================================  */
static servreg_dl_table_p servreg_dl_table_alloc(void);

/** =====================================================================
 * Function:
 *     servreg_record_pd_entry
 *
 * Description:
 *     Record the pd entry in the pd-list
 *
 * Parameters:
 *    sr_pd_name     : Actual sr pd name associated to the pd monitor handle.
 *
 * Returns:
 *    None
 * =====================================================================  */
void servreg_record_pd_entry(SERVREG_NAME sr_pd_name);

/** =====================================================================
 * Function:
 *     servreg_delete_pd_entry
 *
 * Description:
 *     Delete the pd entry in the pd-list
 *
 * Parameters:
 *    sr_pd_name     : Actual sr pd name associated to the pd monitor handle.
 *
 * Returns:
 *    None
 * =====================================================================  */
void servreg_delete_pd_entry(SERVREG_NAME sr_pd_name);

/** =====================================================================
 * Function:
 *     servreg_loc_send_service_list
 *
 * Description:
 *     Send the service list to TN apps
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_loc_send_service_list(void);

/** =====================================================================
 * Function:
 *     servreg_get_pdlist
 *
 * Description:
 *     Get the list of all pd's ("soc/domain/subdomain" strings) attached 
 *     to the SR framework in root-pd.
 *     If API used in user-pd, populates nothing.
 *
 * Parameters:
 *    dlhandle     : Allocated domain list handle.
 *
 * Returns:
 *    Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_get_pdlist(SERVREG_DL_HANDLE dlhandle);

#if defined(__cplusplus)
}
#endif

#endif
