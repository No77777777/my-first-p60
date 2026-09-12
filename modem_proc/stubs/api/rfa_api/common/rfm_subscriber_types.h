#ifndef RFM_SUBSCRIBER_TYPES_H
#define RFM_SUBSCRIBER_TYPES_H

/*!
  @file
  rfm_subscriber_types.h
 
  @brief
  RF subscriber types
*/

/*======================================================================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document are confidential 
  and proprietary information of Qualcomm Technologies Incorporated and all rights therein are 
  expressly reserved. By accepting this material the recipient agrees that this material and the 
  information contained therein are held in confidence and in trust and will not be used, copied, 
  reproduced in whole or in part, nor its contents revealed in any manner to others without the 
  express written permission of Qualcomm Technologies Incorporated.

======================================================================================================================*/

/*======================================================================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
    
  $Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfa_api/common/rfm_subscriber_types.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
04/16/15   aro     Initial revision.

======================================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Identifies RF subscriber.
*/
typedef enum
{
  RFM_SUBSCRIBER_UNASSGINED,   /*!< Indicates unassigned subscriber */

  RFM_SUBSCRIBER_0,      /*!< Indicates subscriber 0  */

  RFM_SUBSCRIBER_1,     /*!< Indicates subscriber 1  */

  RFM_SUBSCRIBER_NUM,  /*!< Number of subcribers  */

  RFM_SUBSCRIBER_INVALID = RFM_SUBSCRIBER_NUM,     /*!< Indicates invalid  */

} rfm_subscriber_type;

#ifdef __cplusplus
}
#endif

#endif /* RFM_SUBSCRIBER_TYPES_H */

