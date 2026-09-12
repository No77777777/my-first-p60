#ifndef DS_WLAN_PROXY_EXT_H
#define DS_WLAN_PROXY_EXT_H
/*===========================================================================

                      DS_WLAN_PROXY_EXT.H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

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
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/wlan/wlan_proxy/inc/ds_wlan_proxy_ext.h#1 $
  $Author: pwbldsvc $ 

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/08/19    my     Added support for QSH MDUMP collection 
01/08/19    my     Created Module
===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_wlan_proxy_mode_hdlr_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for dds_wlan_proxy_instance_ptrs static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_wlan_proxy_mode_hdlr_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_wlan_proxy_dsd_if_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for ds_wlan_proxy_dsd_info static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_wlan_proxy_dsd_if_qsh_mdump_collect();

#endif /* FEATURE_QSH_MDUMP */

#endif /*DS_WLAN_PROXY_EXT_H*/
