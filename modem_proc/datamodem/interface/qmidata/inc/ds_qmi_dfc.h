#ifndef DS_QMI_DFC_H
#define DS_QMI_DFC_H
/*===========================================================================

                         D S _ Q M I _ D F C . H

DESCRIPTION

 The Data Services QMI DS FLOW CONTROL SERVICE  header file.

EXTERNALIZED FUNCTIONS

   qmi_dfc_init()
   
     Initialize the QMI-DS FLOW_CONTROL service

Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_dfc.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/29/18    mj     Created module
===========================================================================*/

#include "comdef.h"
#include "customer.h"

/*===========================================================================

                         MACRO DECLARATIONS

===========================================================================*/
#define DFC_QMI_INSTANCE_ID 0
#define DFC_QMI_SERVICE_COOKIE "DSDFC"

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_DATA_FLOW_MGMT


/*===========================================================================
  FUNCTION QMI_DFC_INIT()

  DESCRIPTION
    Initialize the QMI FLOW CONTROL service and register with QCSI

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_dfc_init
(
  void
);

/*===========================================================================
  FUNCTION QMI_DFC_UL_DATA_SENT_ACK_RCV()

  DESCRIPTION
    Process ack recived for end of data sent

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void qmi_dfc_ul_data_sent_ack_rcvd
(
  uint8                        conf_version, 
  uint8                        conf_type,
  uint16                       seq_num,
  uint8                        bearer_id
);

#endif/* FEATURE_DATA_FLOW_MGMT */
#endif /* DS_QMI_DFC_H */
