/*!
  @file
  ps_utils_qtrace.h

  @brief
  QTrace definitions common to Data Services.
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
03/19/18   skc          QTRACE related changes
==============================================================================*/

#ifndef PS_UTILS_QTRACE_H
#define PS_UTILS_QTRACE_H

#ifdef __cplusplus
extern "C"
{
#endif //_cplusplus
/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "qtrace.h"

/*==============================================================================

                              MACROS

==============================================================================*/

#define PS_TAG_ATCOP               "PS_ATCoP"
#define PS_TAG_QMI_WDS             "PS_QMI_WDS"
#define PS_TAG_QMI_DMS             "PS_QMI_DMS"
#define PS_TAG_QMI_FW              "PS_QMI FRAMEWORK"
#define PS_TAG_NETIFACE            "PS_NETIFACE"
#define PS_TAG_SYSAPI              "PS_SYSAPI"
#define PS_TAG_DSPROFILEDB         "PS_DSPROFILEDB"
#define PS_TAG_IFACECTLS           "PS_IFACECTLS"
#define PS_TAG_PSSOCK              "PS_PSSOCK"
#define PS_TAG_DSNET               "PS_DSNET"
#define PS_TAG_DSPROFILE           "PS_DSPROFILE"
#define PS_TAG_QMICORE             "PS_QMICORE"
#define PS_TAG_DSS                 "PS_DSS"
#define PS_TAG_DSSOCK              "PS_DSSOCK"
#define PS_TAG_UTILS               "PS_UTILS"

/*!
  Enum of buffer ids.
*/
typedef enum
{
  /* For high-frequency */
  PS_UTILS_QTRACE_HF,

  /* Add new buffers before this. */
  PS_UTILS_QTRACE_MAX
} ps_utils_qtrace_enum_type;


/*==============================================================================

                      EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  ps_utils_qtrace_init

==============================================================================*/
/*!
  @brief
  Initializes buffers.

  @return
  None
*/
/*============================================================================*/
void ps_utils_qtrace_init
(
  void
);

/*==============================================================================

  FUNCTION:  ps_utils_qtrace_deinit

==============================================================================*/
/*!
  @brief
  De-initializes buffers.

  @return
  None
*/
/*============================================================================*/
void ps_utils_qtrace_deinit
(
  void
);

/*==============================================================================

  FUNCTION:  ps_utils_qtrace_get_context_id_tbl

==============================================================================*/
/*!
  @brief
  Function to get CONTEXT ID TABLE.

  @return
  ps_utils_qtrace_context_id_tbl
*/
/*============================================================================*/
qtrace_context_id_t ps_utils_qtrace_get_context_id_tbl
(
  ps_utils_qtrace_enum_type id_tbl
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* PS_UTILS_QTRACE_H */
