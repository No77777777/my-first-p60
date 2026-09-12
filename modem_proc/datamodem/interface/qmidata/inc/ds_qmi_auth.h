#ifndef _DS_QMI_AUTH_H
#define _DS_QMI_AUTH_H
/*===========================================================================

                         D S _ Q M I _ A U T H . H

DESCRIPTION

 The Data Services QMI Device Management Service header file.

EXTERNALIZED FUNCTIONS

   qmi_dms_init()
     Register the DMS service with QMUX for all applicable QMI links

Copyright (c) 2005-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_auth.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/17/10    kk     Removing DCC command buffer dependency from QMI services.
02/19/09    am     DS Task De-coupling effort and introduction of DCC task.
09/15/08    ks     Created module
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_QMI_EAP
#include "ds_qmi_svc_ext.h"
#include "dcc_task_defs.h"


/*===========================================================================
  FUNCTION QMI_AUTH_INIT()

  DESCRIPTION
    Register the AUTH service with QMUX for all applicable QMI links

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_auth_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#endif /* FEATURE_DATA_QMI_EAP */
#endif /* _DS_QMI_AUTH_H */
