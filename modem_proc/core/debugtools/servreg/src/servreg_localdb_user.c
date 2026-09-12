/*
#============================================================================
#  Name:
#    servreg_localdb_user.c 
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
#include "tms_utils_msg.h"

#include "servreg_internal.h"
#include "servreg_localdb.h"
#include "procinfo.h"
#include "procinfo_qdi.h"

/* From servreg_local_db_autogen.c file */
/*
extern servreg_entry_t servreg_local_services[] DLL_API_WEAK;
extern char *servreg_local_domain DLL_API_WEAK;
extern char *servreg_soc_name DLL_API_WEAK;
extern char *servreg_domain_name DLL_API_WEAK;
extern char *servreg_subdomain_name DLL_API_WEAK;
extern uint32_t servreg_service_count DLL_API_WEAK;
extern uint32_t servreg_qmi_instance_id DLL_API_WEAK;
*/

/* Default weak symbols to wlan pd for multi si */
const servreg_entry_t servreg_local_services[]={
   {"tms/servreg", 0 , 0},
};
const uint32_t servreg_service_count = 1;
char *servreg_local_domain = NULL;
char *servreg_soc_name = NULL;
char *servreg_domain_name = NULL;
char *servreg_subdomain_name = NULL;
const uint32_t servreg_qmi_instance_id = 0xFFFFFFFF;

void servreg_check_for_init(void)
{
  static boolean servreg_inited = FALSE;
  char * local_domain_copy;
  int len;

  if ( servreg_inited == TRUE )
    return;

  servreg_inited = TRUE;
  /* Buffer will not be freed */
  servreg_local_domain = calloc(1, PROCINFO_NAME_MAX_LEN);

  if ( servreg_local_domain == NULL )
   return;

  procinfo_qdi_invoke_op(PROCINFO_OP_GET_NAME, PROCINFO_DOMAIN_NAME_TYPE, servreg_local_domain, PROCINFO_NAME_MAX_LEN);

  if ( servreg_local_domain[0] == '\0' )
   return;

  len =  strlen(servreg_local_domain);

  /* Buffer will not be freed */
  local_domain_copy = calloc(1, len);

  if ( local_domain_copy == NULL )
   return;

  memscpy(local_domain_copy, len, servreg_local_domain, PROCINFO_NAME_MAX_LEN);

  servreg_soc_name = local_domain_copy;
  
  while ( *local_domain_copy != '\0' )
  {
    if ( *local_domain_copy == '/' )
    {   
      *local_domain_copy = '\0';
      break;
    }

    local_domain_copy++;
  }

  local_domain_copy++;

  if ( *local_domain_copy != '\0' )
    servreg_domain_name = local_domain_copy;
  else
    return;
  
  while ( *local_domain_copy != '\0' )
  {
    if ( *local_domain_copy == '/' )
    {   
      *local_domain_copy = '\0';
      break;
    }

    local_domain_copy++;
  }
 
  local_domain_copy++;

  if ( *local_domain_copy != '\0' )
    servreg_subdomain_name = local_domain_copy;
  else
    return;

}

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
  servreg_check_for_init();

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