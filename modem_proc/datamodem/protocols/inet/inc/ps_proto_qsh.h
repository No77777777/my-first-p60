/*!
  @file
  ps_proto_qsh.h

  @brief
  QSH definitions for Data Protocols.
*/

/*==============================================================================

  Copyright (c) 2019 QUALCOMM Technologies Incorporated. All Rights Reserved

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
01/09/19   rj          Initial release
==============================================================================*/

#ifndef PS_PROTO_QSH_H
#define PS_PROTO_QSH_H

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

FUNCTION ps_utils_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
 
QSH_MDUMP_FN_ATTR void  ps_proto_qsh_cb
(
  void
);

/*===========================================================================
FUNCTION ps_utils_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for Data Interface module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void ps_proto_qsh_mdump_collect
(
  void
);
#endif /* FEATURE_QSH_MDUMP */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* PS_UTILS_QSH_H */

