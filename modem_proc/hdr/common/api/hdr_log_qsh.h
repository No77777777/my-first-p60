#ifndef HDR_QSH_LOG_H
#define HDR_QSH_LOG_H


/*===========================================================================

      H D R   Q S H  L O G 

DESCRIPTION
  This file contains the implementation of the 1x-EVDO QSH logging functionality

  Copyright (c) 2007 - 2016 Qualcomm Technologies Incorporated. All Rights Reserved

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

 when       who       what, where, why
 --------   --------  ----------------------------------------------------------
 05/27/16   vko     QSH framework for events capturing

 ===========================================================================*/
                     
/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "hsrch_qsh_ext.h"
#include "sys.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define hdras_id_t sys_modem_as_id_e_type 


/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

/*===========================================================================

FUNCTION  hdr_qsh_register

DESCRIPTION
  hdr Registration with QSH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdr_qsh_register(qsh_clt_e hdr_qsh_clt);  

/*===========================================================================
   FUNCTION     hdr_qsh_event_notify
  
   DESCRIPTION
   Notifies an event to QSH if configured
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    hdras_id, event_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void hdr_qsh_event_notify(
   hdras_id_t hdr_as_id, 
   qsh_clt_e hdr_qsh_clt,
   hsrch_qsh_event_e event_id
   );

#endif /* FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

#endif /* HDR_QSH_LOG_H */
