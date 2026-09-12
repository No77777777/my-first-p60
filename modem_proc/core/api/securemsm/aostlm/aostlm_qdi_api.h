#ifndef AOSTLM_QDI_API_H
#define AOSTLM_QDI_API_H
/*===========================================================================

               A S W   S E C U R I T Y   S E R V I C E S

                    A O S T L M   S U B S Y S T E M

GENERAL DESCRIPTION
API Provided to Dynamically Loaded AOST features for licensing.
Uses a QDI Driver to dynamically discover the AOST Client.


EXTERNALIZED FUNCTIONS
aostlm_qdi_audiomm_request_access
aostlm_qdi_request_access 
aostlm_qdi_status
aostlm_qdi_report_activation
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                                USAGE
 
          
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/api/securemsm/aostlm/aostlm_qdi_api.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/25/2014  wt     initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "aostlm_audiomm_wrapper.h"

/*=====================================================================
                              Typedefs
======================================================================*/ 

/*=====================================================================
                              Functions
======================================================================*/

/**
 * @brief Allows AOST Features to request access.
 * 
 * @details If the AOSTLM client is not yet connected this call will block
 *          until the server is connected AND the client has fetched license
 *          information from the server. This operation will also block if
 *          another process is requesting access.
 * 
 * @param [in] feature_id: The identifier of the feature requesting access
 *         
 * @return aostlm_access_t
 *          AOSTLM_GRANT_COMM if a valid commercial license is present
 *          AOSTLM_GRANT_EVAL if a valid evaluation license is present and
 *             uses remain
 *          AOSTLM_DENY if there is no valid license for this feature
 *
 *
 */
aostlm_access_t aostlm_qdi_req_access(aostlm_feature_id_t feature_id);

/**
 * @brief Allows client to query for aostlm status
 *
 *  
 * @return status structure with information regarding aostlm 
 *         connectivity
 *
 */
aostlm_client_state_t aostlm_qdi_status(void);

/**
 * @brief allows a module to request access to operate.  
 *
 * @param [in] license_id: The identifier of the license the 
 *      module is managed by
 * 
 * @param [in] module_id:  Identifier of the module requesting 
 *        access
 *  
 * @return  AOSTLM_GRANT_COMM if the module is granted 
 *          commercial access
 *  
 *          AOSTLM_GRANT_EVAL if the module is granted
 *          evaluation license 
 *  
 *          AOSTLM_DENY if access to the module is denied
 * 
 */
aostlm_access_t aostlm_qdi_audiomm_request_access(const aostlm_license_id_t license_id,
                                              const aostlm_module_id_t  module_id);

/**
 * @brief Allows client to indicate that it has been activated
 *  
 * @param [in] feature_id: the identifier of the feature 
 *        reporting use
 *  
 * @return None
 *
 */
void aostlm_qdi_report_activation(aostlm_feature_id_t feature_id);


#endif /* AOSTLM_QDI_API_H */