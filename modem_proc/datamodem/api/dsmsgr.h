#ifndef DSMSGR_H
#define DSMSGR_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

               M S G R   I N T E R F A C E   F U N C T I O N S 

                            H E A D E R    F I L E

DESCRIPTION
  This file defines definitions for use by clients of dsmsgr_send and receive
  modules. This header file is intended to be used internal to the 3G Dsmgr 
  module only.


  Copyright (c) 2011 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/dsmsgr.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/09/11   ars     Removed featurization for SU API compliance
11/18/10   sa      Added function dsmsgrsnd_msg_send_ext().
08/09/09   vk      Moving DS to its own MSGR technology space
05/01/09   vk      Added support for test synchronization with DS task
12/22/08   vk      Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "msgr.h"

#include "pthread.h"

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*
** Receiver-side APIs
*/

pthread_t dsmsgrrecv_task (int priority);

#endif /* DSMSGR_H */
