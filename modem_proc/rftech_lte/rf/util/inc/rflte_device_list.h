/*!
  @file
  rflte_device_list.h

  @brief
  Vector of RFM devices abstraction.
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

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

===========================================================================*/


/*===========================================================================


                EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/rf/util/inc/rflte_device_list.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/18/15   bmg     Initial version

===========================================================================*/

/*===========================================================================

                  INCLUDE FILES

===========================================================================*/

#ifndef RFLTE_DEVICE_LIST_H
#define RFLTE_DEVICE_LIST_H

#include "rfm_device_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
typedef struct rflte_device_list_s
{
  int len;
  int max_len;
  rfm_device_enum_type* list;
} rflte_device_list_type;

/*-----------------------------------------------------------------------*/
void
rflte_device_list_init(
  rflte_device_list_type* const dev_list
);

/*-----------------------------------------------------------------------*/
void
rflte_device_list_deinit(
  rflte_device_list_type* const dev_list
);

/*-----------------------------------------------------------------------*/
void
rflte_device_list_append(
  rflte_device_list_type* const dev_list,
  const rfm_device_enum_type device
);

/*-----------------------------------------------------------------------*/
void
rflte_device_list_remove_first(
  rflte_device_list_type* const dev_list,
  const rfm_device_enum_type device
);

/*-----------------------------------------------------------------------*/
void
rflte_device_list_clear(
  rflte_device_list_type* const dev_list
);

/*-----------------------------------------------------------------------*/
void
rflte_device_list_set(
  rflte_device_list_type* const dev_list,
  const int num_devices,
  ...
);

/*-----------------------------------------------------------------------*/
int
rflte_device_list_compare(
  const rflte_device_list_type* const lhs,
  const rflte_device_list_type* const rhs
);

/*-----------------------------------------------------------------------*/
boolean
rflte_device_list_dev_in_list(
  const rflte_device_list_type* const dev_list,
  const rfm_device_enum_type device
);

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_DEVICE_LIST_H */

