#ifndef PS_CERT_UTILSI_H
#define PS_CERT_UTILSI_H
/*===========================================================================
                           PS_CERT_UTILI_H
===========================================================================*/

/*!
  @file
  ps_cert_utilsi.h

  @brief
  This module includes the initialization and deinitialization

  @detail  
*/

/*===========================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/inc/ps_cert_utilsi.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/10/15   fn      Initiali version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================
                         EXTERNAL FUNCTION DEFINTIONS
===========================================================================*/
/*===========================================================================
FUNCTION      PS_CERT_HTTP_INIT

DESCRIPTION   Initializes resource for ps certificate utility during task 
              startup 

DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  None 
===========================================================================*/
void ps_cert_http_init(void);

/*===========================================================================
FUNCTION      PS_CERT_HTTP_DEINIT

DESCRIPTION   Deinitialization of PS CERT utility

DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  None 
===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#ifdef __cplusplus
}
#endif

#endif /* PS_CERT_UTILSI_H */
