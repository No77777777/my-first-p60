#ifndef DS3GMSGR_H
#define DS3GMSGR_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

               M S G R   I N T E R F A C E   F U N C T I O N S 

                            H E A D E R    F I L E

DESCRIPTION
  This file defines definitions for use by clients of dsmsgr_register and send
  modules. This header file is intended to be used internal to the 3G Dsmgr 
  module only.


  Copyright (c) 2015-2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gmsgr.h#2 $
 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/16   zhangyan     Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "msgr.h"
#include "sys.h"
#include "dsmsgrrecv.h"

/*===========================================================================

                      EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define DS3G_MSGR_GLOBAL_MSG                        1             
#define DS3G_MSGR_LOCAL_MSG                         0

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Function pointer used by various DS3G clients to specify individual cback
  function to be invoked once message is received from MSGR
---------------------------------------------------------------------------*/
typedef boolean (* ds3g_msgr_client_func_ptr) 
(
  msgr_umid_type, 
  sys_modem_as_id_e_type,
  const msgr_hdr_struct_type *
);

/*---------------------------------------------------------------------------
  Payload used when posting a cmd to process MSG router cback
---------------------------------------------------------------------------*/
typedef struct
{
  dsmsgrrcv_msg_u       payload;
  void                 *user_data;
}ds3gmsgr_cmd_type;

/*===========================================================================
FUNCTION      DS3G_REG_MSGR_CB

DESCRIPTION   This function is called by all the clients of DS task that are 
              interested in listening a message 
  
PARAMETERS    msgr_type : message to be listened 
              func_ptr  : client cb
 
DEPENDENCIES  None.
  
RETURN VALUE  None.                                           
   
SIDE EFFECTS  None.
  
===========================================================================*/
void ds3gmsgr_reg_msgr_cb
(
  msgr_umid_type             msgrtype, 
  ds3g_msgr_client_func_ptr  func_ptr
);

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /* FEATURE_LAPP_UFS */

/*===========================================================================
FUNCTION      DS3GMSGR_MSG_SEND_EXT

DESCRIPTION   
  
PARAMETERS    msgr_type : message to be listened 
    
DEPENDENCIES  None.
  
RETURN VALUE  None.                                           
   
SIDE EFFECTS  None.
  
===========================================================================*/

boolean ds3gmsgr_msg_send_ext
(
  msgr_umid_type         msgtype,
  msgr_tech_module_type  tech_mod_type,
  msgr_hdr_struct_type   *msg,
  uint32                 msglen,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS3GMSGR_MSG_SEND

DESCRIPTION   

PARAMETERS    msgr_type : message to be listened

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.

===========================================================================*/
boolean ds3gmsgr_msg_send
(
  msgr_umid_type         msgtype,
  msgr_tech_module_type  tech_mod_type,
  msgr_hdr_struct_type   *msg,
  uint32                 msglen
);

#endif /* DS3GMSGR_H */
