#ifndef DS3GCMIFEX_H
#define DS3GCMIFEX_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

           C A L L   M G R   I N T E R F A C E   F U N C T I O N S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains functions for interfacing to Call Manager, including
  Data Services client initialization and processing of call events. This
  header file is intended to be used internal to the 3G Dsmgr module only.


  Copyright (c) 2001 - 2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds3gcmif.h_v   1.1   28 Feb 2003 18:53:34   rchar  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gcmif_ex.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/15/17   rs     Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msgr.h"
#include "sys.h"
#include "dsmsgrrecv.h"
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define DS3GEX_CM_NUM_CBS         2 

typedef void (*ds3gex_cm_detach_info_type)
(
  sys_sys_mode_e_type         rat,
  sys_modem_as_id_e_type      subs_id
);

extern ds3gex_cm_detach_info_type 
  ds3gex_cm_detach_info_client_tbl[DS3GEX_CM_NUM_CBS];

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS3GEX_CM_INIT

DESCRIPTION 
  This is the initializer function for all DS<-->CM related functionality

DEPENDENCIES 
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
extern void ds3gex_cm_init
(
  void 
);

/*===========================================================================
FUNCTION      DS3GEX_CM_REGISTER_CLIENT_FOR_DETACH_INFO

DESCRIPTION 
  This function is used by clients to register for the detach information
  when CM sends service domain change indication to DS.
 
PARAMETERS
  client_func_ptr   - Call back function to be called

DEPENDENCIES 
  Note - DS3GEX_CM_NUM_CBS needs to be increased when a new
         client registers for the detach information

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
extern void ds3gex_cm_register_client_for_detach_info
(
  ds3gex_cm_detach_info_type  client_func_ptr
);

/*===========================================================================
FUNCTION   DS3GEX_CM_SRV_IND_HDLR

DESCRIPTION
  This is the handler function called as part of CM service indicaton from
  CM. This function determines whether a detach was triggered and uses
  this information to clear network override throttling.
 
PARAMETERS
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - if the message header is correct and we can handle the message
  FALSE - otherwise

  Note: We will not return FALSE for errors in the message payload

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds3gex_cm_srv_ind_hdlr
(
  msgr_umid_type              msgrtype,
  sys_modem_as_id_e_type      subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION   DS3GEX_CM_STUB_SEND_DETACH_IND

DESCRIPTION
  This is a test utility function to call all the DS 3GPP registered clients
  when detach is triggered
 
PARAMETERS
  sys_mode   -- System mode on which detach is triggered
  subs_id    -- Subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
===========================================================================*/
void ds3gex_cm_stub_send_detach_ind
(
  sys_sys_mode_e_type         sys_mode,
  sys_modem_as_id_e_type      subs_id
);

#endif /* DS3GCMIFEX_H */
