#ifndef AOSTLM_API_H
#define AOSTLM_API_H
/*===========================================================================

               A S W   S E C U R I T Y   S E R V I C E S

                    A O S T L M   S U B S Y S T E M

GENERAL DESCRIPTION
API Provided to AOST features for licensing


EXTERNALIZED FUNCTIONS
aostlm_request_access 
aostlm_report_activation
aostlm_status 
aostlm_audit_count
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                                USAGE
 
 The AOSTLM API allows features to request permission to run
          
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/api/securemsm/aostlm/aostlm_api.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/28/2014  djc     initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*=====================================================================
                              Typedefs
======================================================================*/

/* Feature Id Type */
typedef uint32 aostlm_feature_id_t;

/* request result type*/
typedef enum 
{
  AOSTLM_GRANT_COMM   = 0,     /* Access Granted for Valid Commercial License */
  AOSTLM_GRANT_EVAL,           /* Access Granted for Valid Evaluation License */
  AOSTLM_DENY,                 /* Access Denied */
  AOSTLM_INVALID_RSP   = 0xF   /* Reserved - Not a Valid Access Response Type */
} aostlm_access_t;

/* aostlm client state*/
typedef enum
{
    AOSTLM_STOPPED = 0,     /* Cannot use AOSTLM, it needs to be initialized */
    AOSTLM_PARSED,          /* Cannot use AOSTLM, it has been initialized, but has not
                               yet finished parsing data */
    AOSTLM_READY            /* AOSTLM is live and may be used.*/
} aostlm_client_state_t;

/*=====================================================================
                              Functions
======================================================================*/

/**
 * @brief Allows AOST Features to request access.
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
aostlm_access_t aostlm_req_access(aostlm_feature_id_t feature_id);

/**
 * @brief Allows client indicate feature has been activated
 *
 * @param [in] feature_id:  The identifier of the feature being 
 *        activated
 *  
 * @return none
 *
 */
void aostlm_report_activation(aostlm_feature_id_t feature_id);

/**
 * @brief Fetches the audit count for a feature 
 *  
 * @param feature_id the feature ID we are querying 
 * 
 * @return times this feature has been activated
 */
uint32 aostlm_audit_count(aostlm_feature_id_t feature_id);

/**
 * @brief Allows client to query for aostlm status
 *  
 *  
 * @return status structure with information regarding aostlm 
 *         client status
 *
 */
aostlm_client_state_t aostlm_status(void);
#endif /* AOSTLM_API_H */
