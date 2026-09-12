#ifndef _DS_QMI_CSVT_H
#define _DS_QMI_CSVT_H
/*===========================================================================

                         D S _ Q M I _ C S V T . H

DESCRIPTION

 The Qualcomm MSM Interface for Circuit-switched Video Telephony header file.

EXTERNALIZED FUNCTIONS

Copyright (c) 2011,2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_csvt.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
01/05/12    sa     New CSVT service.
08/22/11    kk     Initial version.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "dsucsdappif.h"

#if (defined(FEATURE_DATA_GCSD) || defined(FEATURE_DATA_WCDMA_CS))
/*===========================================================================

                            VARIABLE DEFINITIONS

===========================================================================*/
#define MAX_DIAL_STRING_LEN                     80
/*===========================================================================

                            EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_CSVT_INIT()

  DESCRIPTION
    Initializes the QMI CSVT service on the given instance

  PARAMETERS
    inst - the QMI instance on which the service initialization is sought

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void qmi_csvt_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#endif /* (defined(FEATURE_DATA_GCSD) || defined(FEATURE_DATA_WCDMA_CS)) */
#endif /* _DS_QMI_CSVT_H */
