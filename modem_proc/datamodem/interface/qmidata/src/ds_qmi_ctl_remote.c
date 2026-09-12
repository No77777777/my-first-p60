/*==========================================================================*/
/*!
  @file
  ds_qmi_ctl_remote.c

  @brief
  This file provides implementation for remoting certain QMI CTL
  messages to a different modem. 

  @details
  The following external methods are supported.

  qmi_ctl_remote_init()
    Initialize the QMI control remoting service
  
  qmi_ctl_remote_set_data_format()
    Sets the data format on the remote port.

  qmi_ctl_remote_release()
    Release the QMI control remoting services

  Copyright (c) 2011 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_ctl_remote.c#2 $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/06/11    hm     Created module
===========================================================================*/

/*===========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "ds_qmi_ctl_remote.h"
#include "ds_Utils_DebugMsg.h"


/*---------------------------------------------------------------------------
  Providing stubs if the FEATURE_DATA_QMI_CTL_REMOTE is not defined.
---------------------------------------------------------------------------*/
qmi_instance_e_type 
qmi_ctl_remote_init
(
  qmi_instance_e_type  qmi_inst
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_0 ("QMI CTL REMOTE not supported");

  (void) qmi_inst;
  return QMI_INSTANCE_MAX;
}

void qmi_ctl_remote_set_data_format
(
  qmi_instance_e_type                qmi_inst,
  uint16                             link_prot,
  uint8                              qos_hdr_enabled,
  boolean                            ul_tlp
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_0 ("QMI CTL REMOTE not supported");

  (void) qmi_inst;
  (void) link_prot;
  (void) qos_hdr_enabled;
  (void) ul_tlp;
  return;
}

void qmi_ctl_remote_release
(
  void 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_0 ("QMI CTL REMOTE not supported");
  return;
}



