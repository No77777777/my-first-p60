#ifndef _DS_QMI_DSD_H
#define _DS_QMI_DSD_H
/*===========================================================================

                         D S _ Q M I _ D S D . H

DESCRIPTION

 The Data Services QMI Data Service Determination header file.

EXTERNALIZED FUNCTIONS

   qmi_dsd_init()
     Initialize the QMI-DSD service and register with QCSI

Copyright (c) 2012,2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_dsd.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/07/12    sj     Created module
===========================================================================*/

#include "comdef.h"
#include "customer.h"

/*===========================================================================
  FUNCTION QMI_DSD_INIT()

  DESCRIPTION
    Initialize the QMI-DSD service and register with QCSI

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_dsd_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#endif /* _DS_QMI_DSD_H */
