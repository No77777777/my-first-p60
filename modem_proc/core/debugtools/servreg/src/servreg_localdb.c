/*
#============================================================================
#  Name:
#    servreg_localdb.c 
#
#  Description:
#    File containing API's to access Service Registry local database
#                                                                            
# Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.        
#============================================================================
*/
#include "stdarg.h"
#include "stdlib.h"
#include <stringl/stringl.h>
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "err.h"

#include "servreg_internal.h"
#include "servreg_localdb.h"

#include "tms_utils_msg.h"

/* From servreg_local_db_autogen.c file */
extern const servreg_entry_t servreg_local_services[];
extern const char *servreg_local_domain;
extern const char *servreg_soc_name;
extern const char *servreg_domain_name;
extern const char *servreg_subdomain_name;
extern const uint32_t servreg_service_count;
extern const uint32_t servreg_qmi_instance_id;

/** =====================================================================
 * Function:
 *     servreg_get_local_info
 *
 * Description:
 *     Function to get the local information
 *     Queries the local database
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     Requested information
 * =====================================================================  */
void* servreg_get_local_info(servreg_local_info_type info)
{
  switch(info)
  {
     case LOCAL_QMI_INSTANCE_ID:
        return (void*)(&servreg_qmi_instance_id);
     case LOCAL_SERVICE_COUNT:
        return (void*)(&servreg_service_count);
     case LOCAL_PROCESS:
        return (void*)servreg_local_domain;
     case LOCAL_SOC:
        return (void*)servreg_soc_name;
     case LOCAL_DOMAIN:
        return (void*)servreg_domain_name;
     case LOCAL_SUBDOMAIN:
        return (void*)servreg_subdomain_name;
     default:
        TMS_MSG_ERROR_1("Invalid param info %d",info);
        return SERVREG_NULL;
  }
}
