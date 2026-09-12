#ifndef TRM_DEPRECATED_APIS_H
#define TRM_DEPRECATED_APIS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*==

           T R A N S C E I V E R   R E S O U R C E   M A N A G E R

              Transceiver Resource Manager Deprecated APIs Header File

GENERAL DESCRIPTION

  This file provides some common definitions for trm.cpp & trmlog.cpp


EXTERNALIZED FUNCTIONS

  None


REGIONAL FUNCTIONS

  None


INITIALIZATION AND SEQUENCING REQUIREMENTS

  None


  Copyright (c) 2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*==



===============================================================================

                           EDIT HISTORY FOR MODULE

$Header: //components/rel/mcs.mpss/5.2/api/trm_deprecated_apis.h#1 $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ----------------------------------------------------------
05/28/2015   sr      Removal of deprecated APIs
01/22/2015   jm      Remove deprecated APIs not referenced in CRM build (2nd round)
12/17/2014   jm      Adding deprecated attribute to all APIs
12/10/2014   jm      Remove deprecated APIs not referenced in CRM build
07/23/2014   sr      GERAN using deprecated priority causing L2G resel/redir/CCO 
                     failure in DR-DSDS mode (700576)
06/24/2014   mn      Adding support for num pending callbacks in req and not,
                      req and not enhanced and unlock callbacks. (CR: 684638).
05/15/2014   mn      Initial version.

=============================================================================*/

/*=============================================================================

                           INCLUDE FILES FOR MODULE

=============================================================================*/

/*=============================================================================

                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module. */

rfm_device_enum_type trm_get_rf_device
(
/* The client who's RF chain ownership is desired */
trm_client_enum_t               client
);


/*============================================================================

FUNCTION TRM_FREQ_RESERVE_AT

DESCRIPTION
  Specifies the given client needs the given transceiver resource at the given
  time, for the given duration, for the supplied reason. This is different to
  old api since it takes band as an argument. This is used to check if the
  band is valid and also if components mapped to the band don't have any
  concurrency issues.

  This would be used with the "trm_request()" function, below.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.

============================================================================*/
void trm_freq_reserve_at
(
/* The client which needs the RF resource */
trm_client_enum_t               client,

/* The RF resource which is being requested */
trm_resource_enum_t             resource,

/* When the resource will be needed (sclks timestamp) */
trm_time_t                      when,

/* How long the resource will be needed for (in sclks) */
trm_duration_t                  duration,

/* Why the resource is needed (used for priority decisions) */
trm_reason_enum_t               reason,

trm_frequency_type_t            freq_info
);


#endif /* TRM_DEPRECATED_APIS_H */