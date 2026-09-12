/*!
  @file
  msgr_utils.h

  @brief
  REQUIRED brief one-sentence description of this C header file.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*==============================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/utils.mpss/5.4/api/common/msgr_utils.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
06/11/19   am      CR2468421: FR55289 MDB collection
05/17/16   mg      CR1000126: Add support for the new task CAFI.
==============================================================================*/

#ifndef MSGR_UTILS_H
#define MSGR_UTILS_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <msgr_umid.h>  /* UMID definitions */


/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/


#define MSGR_UTILS_CFM  MSGR_TECH_MODULE( MSGR_TECH_UTILS, 0x01 ) /*0x1101*/
#define MSGR_UTILS_CAFI MSGR_TECH_MODULE( MSGR_TECH_UTILS, 0x02 ) /*0x1102*/
#define MSGR_UTILS_PLT  MSGR_TECH_MODULE( MSGR_TECH_UTILS, 0x03 ) /*0x1103*/
#define MSGR_UTILS_LB   MSGR_TECH_MODULE( MSGR_TECH_UTILS, 0x04 ) /*0x1104*/
#define MSGR_UTILS_QSH  MSGR_TECH_MODULE( MSGR_TECH_UTILS, 0x05 ) /*0x1105*/

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

#endif /* MSGR_UTILS_H */
