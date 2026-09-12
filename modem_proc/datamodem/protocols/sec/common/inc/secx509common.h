#ifndef SECX509COMMON_H
#define SECX509COMMON_H


/*===========================================================================

                     EFS storage and purge utility for dynamic certificate
							 
FILE:  secx509common.h

DESCRIPTION:
	Utility function for use by clients like TLS/IPsec. It manages dynamic certificate
	EFS storage management including store, retrieve and purge      
===========================================================================*/

/*=========================================================================

                      EDIT HISTORY FOR FILE
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/19/18   ggrewal Initial API's for EFS storage and purge utility for dynamic certificates (FR-46987)
============================================================================*/

#include <stdlib.h>
#include "stdio.h"
#include "secdiag.h"

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "task.h"
#include "assert.h"
#include "msg.h"
#include "err.h"

#include "secerrno.h"
#include "secssl.h"
#include "secsslca.h"
#include "secasn1.h"
#include "secx509.h"
#include "secapi.h" 

#include "fs_public.h" 
#include "fs_errno.h"
#include "str_util.h"
#include "UxCommon.h"

#define SECX509_DYNAMIC_MAX_CERTS           		5 /**< Maximum number of dynamic CA certificates in a directory */

/**
 Clients use this API to store the dynamic certificate to the EFS. Clients are expected to invoke this API only once for 
 the newly fetched dynamic certificate with which server chain has been successfully validated.
*/
secerrno_enum_type secx509_common_write_dynamic_cert_to_efs(
   const char* base_directory_path,
   uint8* cert_buffer,
   uint16 cert_size,
   uint8 max_dynamic_certs_supported, 
   uint32 last_successfully_used_timestamp,
   secasn1_data_type* dn
);

secerrno_enum_type secx509_common_read_dynamic_cert_from_efs(
   const char* base_directory_path, 
   const char* file_name,
   uint8** cert_buffer,
   uint16* cert_size   
);

/**
 This API is invoked by the clients any time the root CA certificate is used for successful validation
*/
secerrno_enum_type secx509_common_update_dynamic_cert_usage_stats(
   const char* base_directory_path, 
   char* file_name, 
   uint32 last_successfully_used_timestamp
);

/**
 Clients should invoke this API to write in-memory usage stats/info of dynamic certificates to the EFS storage
*/
secerrno_enum_type secx509_common_write_dynamic_certs_info_to_efs(
	const char* base_directory_path
);

/**
 API should be invoked by client when they want to purge the certificate from EFS for situations like expired certificate
*/
secerrno_enum_type secx509_common_purge_dynamic_cert_from_efs(
   const char* base_directory_path, 
   const char* file_name
);

/**
  API should be invoked by client when the task shuts down 
  This should be the last API which clients invoke before shutdown/teardown
  Implementation will clear the memory allocation done for in memory map storing certificate stats
*/
secerrno_enum_type secx509_common_free_dynamic_certs_info_memory_buffer(
   const char* base_directory_path
);

typedef struct
{
  char*  file_name; /* Certificate Authority file names from EFS */
  secasn1_data_type* dn;
} secx509_dynamic_rootca_file_type;

typedef struct
{
  uint16  fs_num_ca;    /* number of CA certs under EFS directory */
  secx509_dynamic_rootca_file_type*  cert_files[SECX509_DYNAMIC_MAX_CERTS];
} secx509_dynamic_rootca_folder_type;

/**
  API to get access to contents of certsinfo (i.e. fileName and DN details)
  Client passes base_directory_path (e.g. /cert/)
  Client is responsible to free up the memory at the end by calling secx509_common_free_dynamic_certs_info_memory_buffer
  root_folder_data returned by this API is simply pointing to the objects in this utility, no seperate allocation done for clients
*/
secerrno_enum_type secx509_common_get_dynamic_certs_info (
   const char*  base_directory_path,
   secx509_dynamic_rootca_folder_type** root_folder_data
);

#endif /* SECX509COMMON_H */