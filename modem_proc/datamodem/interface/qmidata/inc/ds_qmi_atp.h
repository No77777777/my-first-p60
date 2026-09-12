#ifndef _DS_QMI_ATP_H
#define _DS_QMI_ATP_H
/*===========================================================================

                         D S _ Q M I _ A T P . H

DESCRIPTION

 The Data Services QMI Application Traffic Pairing header file.

EXTERNALIZED FUNCTIONS

   qmi_atp_init()
     Initialize the QMI-ATP service and register with QCSI

Copyright (c) 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_atp.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/13/13    sj     Created module
===========================================================================*/

#include "comdef.h"
#include "customer.h"

/*===========================================================================
  FUNCTION QMI_ATP_INIT()

  DESCRIPTION
    Initialize the QMI-ATP service and register with QCSI

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_atp_init
(
  void
);

#endif /* _DS_QMI_ATP_H */
