#ifndef __WWAN_COEX_CHNL_H__
#define __WWAN_COEX_CHNL_H__
/*!
  @file wwan_coex_chnl.h

  @brief
   APIs exposed by the CXM for WWAN-WWAN COEX channel conflict

*/

/*=============================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated.
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

=============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcs.mpss/5.2/api/wwan_coex_chnl.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/01/15   sg      Increase QTA array size
04/13/15   sg      Initial Revision

=============================================================================*/

/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include "cxm.h"
#include "trm.h"

/*=============================================================================

                       COMMON TYPES AND DEFINES

=============================================================================*/
#define CXM_QTA_MAX_DEVICE_PAIR     8

/*=============================================================================

                       MESSAGE DATA TYPES/STRUCTURES

=============================================================================*/

typedef enum
{
  CXM_CARR_CA1,
  CXM_CARR_CA2
}cxm_carr_type;

/*! Structure to store QTA information for a pair of devices */
typedef struct
{
  rfm_device_enum_type       data_dev;
  rfm_device_enum_type       idle_dev;
  trm_client_type            data_client_type;
  trm_restriction_enum_type  rstr_type;
  boolean                    is_partial_conflict;
} cxm_qta_association_type;

/*! Structure for the QTA gap */
typedef struct
{
  trm_client_enum_t         data_clid;
  trm_client_enum_t         idle_clid;
  uint8                     num_entries;
  boolean                   is_qta_start;
  cxm_qta_association_type  qta_params[CXM_QTA_MAX_DEVICE_PAIR];
} cxm_qta_info_type;

/*=============================================================================

                        Function Declarations

=============================================================================*/



/*============================================================================

FUNCTION CXM_QTA_UPDATE_INFO

DESCRIPTION
  Function used by tech TRM to update QTA related parameters to COEX.
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean
    TRUE - if the parameters are valid
    FALSE - if the parameters are invalid

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_qta_update_info
(
  cxm_qta_info_type   *qta_info
);

/*=============================================================================

  FUNCTION:  wwan_coex_spur_list_init

=============================================================================*/
/*!
    @brief
    Initialize spur related entries
 
    @details
      Initialize spur related entries
      
    @return
     void

*/
/*===========================================================================*/
void wwan_coex_chnl_cflt_init(void);


#endif /* __WWAN_COEX_CHNL_H__ */
