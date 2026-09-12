/*!
  @file IPA_test_mutils_zuc.h

  @brief
  The commmon security header file for MODEM

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*==============================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/12/12   kp      Initial version  .
==============================================================================*/

#ifndef ZUC_H
#define ZUC_H
#include <comdef.h>

#ifdef __cplusplus
extern "C" 
{
#include <dsm.h>


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
void zuc_dsm_EEA3 (uint8* , uint32 , uint32 , uint32 , dsm_item_type*);
}
#endif /*C++*/
#endif /* ZUC_H*/
