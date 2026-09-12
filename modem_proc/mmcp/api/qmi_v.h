#ifndef QMI_V_H
#define QMI_V_H
/*==============================================================================
@FILE_DOC

              QMI VOICE AND NAS   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary for
  clients to interface with the QMI voice and NAS module

Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 $Header: //components/rel/mmcp.mpss/7.9.0/api/qmi_v.h#1 $

when             who          what, where, why
--------       ---          -------------------------------------------------------------
10/16/2018   rupamm     Added API for NAS to get default bearere capability info for setup res
==============================================================================*/


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "sys.h"
#include "cm_gw.h"

/*---------------------------------------------------------------------------
** Functions
**---------------------------------------------------------------------------
*/

/*===========================================================================

FUNCTION: qmi_voice_util_setup_ind_param_per_sub

DESCRIPTION
  This function sets the bearer capability to default value indicating Voice call.

DEPENDENCIES
  None.

RETURN VALUE
  Default bearer capability.

SIDE EFFECTS
  None

===========================================================================*/
extern void qmi_voice_util_setup_ind_param_per_sub(

  cm_bearer_capability_s_type    *bearer_capability_1,

  sys_modem_as_id_e_type          asubs_id
);

/*=========================================================================
  FUNCTION:  qmi_voice_cm_sups_get_ss_ref

===========================================================================*/
/*!
    @brief
    Assigning reference to supplimentary services for identifying it uniquely

    @return
    retuns SS reference value
*/
/*=========================================================================*/

extern uint8 qmi_voice_cm_sups_get_ss_ref(void);

#endif /* QMI_V_H */


