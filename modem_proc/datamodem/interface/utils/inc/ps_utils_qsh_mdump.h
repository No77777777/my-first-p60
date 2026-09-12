/*!
  @file
  ps_utils_qsh_mdump.h

  @brief
  QSH Mini Dump definitions for Data Interfaces.
*/

/*==============================================================================

  Copyright (c) 2018 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.



when       who          what, where, why
--------   ---         ------------------------------------------------------------- 
06/14/18   ss          Initial release
==============================================================================*/

#ifndef PS_UTILS_QSH_MDUMP_H
#define PS_UTILS_QSH_MDUMP_H

#ifdef __cplusplus
extern "C"
{
#endif //_cplusplus
/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"


/*==============================================================================

                      EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*===========================================================================
FUNCTION PS_DPM_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for DPM global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void ps_dpm_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION PS_MEM_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void ps_mem_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION QMUX_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for QMUX global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void qmux_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION QMI_FRAMEWORK_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for QMI Framework global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void qmi_framework_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION QMI_QOS_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for QOS global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void qmi_qos_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION QMI_WDS_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for WDS global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void qmi_wds_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION REV_IP_TRANSPORT_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for REV IP global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void rev_ip_transport_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION RMNET_XPORT_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for RmNet xport global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void rmnet_xport_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION PS_SYS_CONF_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for PS SYS CONF global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void ps_sys_conf_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DS3G_SIOLIB_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for DS3G SIO LIB global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void ds3g_siolib_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DSATCMDP_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for AT CMD Processor global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void dsatcmdp_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DSATETSIPKT_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for AT CMD Processor global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void dsatetsipkt_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DSATCMIF_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for AT CM Interface global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void dsatcmif_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DSATSMS_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for ATCoP SMS global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void dsatsms_qsh_mdump_collect
(
  void
);

/*===========================================================================
FUNCTION DSATVOICE_QSH_MDUMP_COLLECT
  
DESCRIPTION
  This function collects the mdump for ATCoP Voice global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void dsatvoice_qsh_mdump_collect
(
  void
);

#endif /* FEATURE_QSH_MDUMP */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* PS_UTILS_QSH_MDUMP_H */


