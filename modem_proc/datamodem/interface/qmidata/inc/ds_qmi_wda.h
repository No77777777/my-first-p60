#ifndef _DS_QMI_WDA_H
#define _DS_QMI_WDA_H
/*===========================================================================

                         D S _ Q M I _ W D A . H

DESCRIPTION

 The Data Services QMI Wireless Data Administrative Service header file.

EXTERNALIZED FUNCTIONS

   qmi_wda_init()
     Register the WDA service with QMUX for all applicable QMI links

Copyright (c) 2011,2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/inc/ds_qmi_wda.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/02/11    sy     Created module
===========================================================================*/

#include "comdef.h"
#include "customer.h"

/*---------------------------------------------------------------------------
  This is callback type to notify powersave mode change.
----------------------------------------------------------------------------*/
typedef void (* qmi_wda_powersave_mode_cb_type)
(
  uint32  ep_id,
  uint32  mask
);

/*---------------------------------------------------------------------------
  Enum type mask for powersave mode
----------------------------------------------------------------------------*/
typedef enum
{
  QMI_WDA_POWERSAVE_CONFIG_DL_MARKER        = 0x01,
  /**< Control downlink marker notification sent by HW \n */
  QMI_WDA_POWERSAVE_CONFIG_FLOW_CTL         = 0x02,
  /**< Control Flow enable/disable indication \n */
  QMI_WDA_POWERSAVE_CONFIG_ALL_MASK         = 0x7FFFFFFF
  /**< Control ALL indication listed above \n */
} qmi_wda_powersave_config_mask_e_type;

/*---------------------------------------------------------------------------
  Structure type storing clients information
----------------------------------------------------------------------------*/
typedef struct
{
  qmi_wda_powersave_config_mask_e_type  conf_mask;
  qmi_wda_powersave_mode_cb_type        callback;
} qmi_wda_powersave_client_info_type;

/*===========================================================================
  FUNCTION QMI_WDA_INIT()

  DESCRIPTION
    Register the WDA service with QMUX for all applicable QMI links

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern void  qmi_wda_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================
  FUNCTION QMI_WDA_REG_POWERSAVE_MODE()

  DESCRIPTION
    Register powersave mode with the WDA service

  PARAMETERS
    conf_mask  : mask to be configured to set
    callback   : callback function pointer to notify powersave mode
                 enabled/disabled

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_wda_reg_powersave_mode
(
  qmi_wda_powersave_config_mask_e_type  conf_mask,
  qmi_wda_powersave_mode_cb_type        callback
);

#endif /* _DS_QMI_WDA_H */
