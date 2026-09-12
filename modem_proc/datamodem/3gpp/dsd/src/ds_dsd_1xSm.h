#ifndef DS_DSD_1XSM_H
#define DS_DSD_1XSM_H
/*===========================================================================

                      D S _ D S D _ 1 X S M . H
                   
DESCRIPTION
  

EXTERNALIZED FUNCTIONS

 Copyright (c) 2009 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_1xSm.h#2 $
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
FUNCTION      DS_DSD_1XSM_TIMER_CB

DESCRIPTION 
 
DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  
===========================================================================*/
void ds_dsd_1xSm_timer_cb
(
  unsigned long               timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_1XSM_CREATE_SM_INSTANCE

DESCRIPTION   This function creates 1x SM instance corresponding to subs id

DEPENDENCIES  None.

RETURN VALUE  If sucess, statemachine pointer.
              Else, NULL.

SIDE EFFECTS  1. Creates SM instance
              2. Variables are initialized to default value.
===========================================================================*/
ds_dsd_sm_type* ds_dsd_1xSm_create_sm_instance
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_1XSM_INIT

DESCRIPTION   This function initializes 1x state machine

DEPENDENCIES  Event Dispatcher module is initialized already.

RETURN VALUE  None.

SIDE EFFECTS  Registered with event dispatcher module to get notification
===========================================================================*/
void ds_dsd_1xSm_init(void);

#endif /* DS_DSD_1XSM_H */
