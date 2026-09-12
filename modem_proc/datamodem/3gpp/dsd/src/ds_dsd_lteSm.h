#ifndef DS_DSD_LTESM_H
#define DS_DSD_LTESM_H
/*===========================================================================

                    D S _ D S D _ L T E S M . H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_lteSm.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/11/11    sk    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ds_dsd_common.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS_DSD_LTESM_TIMER_CB

DESCRIPTION   The is the function to process timer expiry events of this 
              state machine. 
 
DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None
===========================================================================*/
void ds_dsd_lteSm_timer_cb
(
  unsigned long               timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_LTESM_CREATE_SM_INSTANCE

DESCRIPTION   This function creates LTE SM instance corresponding to subs id

DEPENDENCIES  None.

RETURN VALUE  If sucess, statemachine pointer.
              Else, NULL.

SIDE EFFECTS  1. Creates SM instance
              2. Variables are initialized to default value.
===========================================================================*/
ds_dsd_sm_type* ds_dsd_lteSm_create_sm_instance
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_LTESM_INIT

DESCRIPTION   This function initializes LTE state machine

DEPENDENCIES  Event Dispatcher module is initialized already.

RETURN VALUE  None.

SIDE EFFECTS  Registered with event dispatcher module to get notification
===========================================================================*/
void ds_dsd_lteSm_init(void);

#endif /* DS_DSD_LTESM_H */
