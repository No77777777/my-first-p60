#ifndef _DS_QMI_OTT_H
#define _DS_QMI_OTT_H
/*===========================================================================

                         D S _ Q M I _ O T T . H

DESCRIPTION

 The Data Services QMI Application Traffic Pairing header file.

EXTERNALIZED FUNCTIONS

   qmi_ott_init()
     Initialize the QMI-OTT service and register with QCSI

Copyright (c) 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_ott.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/13/13    sj     Created module
===========================================================================*/

#include "comdef.h"
#include "customer.h"

/*===========================================================================
  FUNCTION QMI_OTT_INIT()

  DESCRIPTION
    Initialize the QMI-OTT service and register with QCSI

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_ott_init
(
  void
);

#endif /* _DS_QMI_OTT_H */
