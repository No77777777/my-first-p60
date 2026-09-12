/*!
  @file
  ps_utils_qtrace.c

  @brief
  QTrace implementation for Data Services.
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


when       who         what, where, why
--------   ---         ------------------------------------------------------------- 
03/19/18   skc         Initial check-in
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "ps_utils_qtrace.h"
#include "ds_Utils_DebugMsg.h"

/*==============================================================================

                         EXTERNAL VARIABLES

==============================================================================*/

/*!
  Table of handles per buffer.

  @note
  Initialized at start.

  @note
  QTRACE_CONTEXT_ID_COMMON maps to a valid context id  
*/
LOCAL qtrace_context_id_t ps_utils_qtrace_context_id_tbl[PS_UTILS_QTRACE_MAX] = 
{
  QTRACE_CONTEXT_ID_COMMON
};

/*==============================================================================

                                FUNCTIONS

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
)
{
  /* get context id for each buffer */

  ps_utils_qtrace_context_id_tbl[PS_UTILS_QTRACE_HF] = qtrace_buffer_init(
    /* owner client */
    QSH_CLT_DS,

    /* buffer id */
    PS_UTILS_QTRACE_HF,

    /* name */
    "DS_HF",
#ifdef FEATURE_DATA_QTRACE_BUFFER_2K
    /* size */
    QTRACE_SIZE_2K,
#else
    /* size */
    QTRACE_SIZE_256B,
#endif /* FEATURE_DATA_QTRACE_BUFFER_2K */

    /* mode_mask */
    QTRACE_BUFFER_MODE_STREAM,

    /* no restriction on mode change */
    QTRACE_BUFFER_TYPE_NONE,

    /* expecting high logging frequency */
    QTRACE_LOG_CAT_HIGH_FREQ
  );
}

/*==============================================================================

  FUNCTION: ps_utils_qtrace_deinit

==============================================================================*/
/*!
  @brief
  De-initializes buffers 

  @return
  None
*/
/*============================================================================*/
void ps_utils_qtrace_deinit
(
  void
)
{
  /* deinit buffers, setting back to common context id */
  qtrace_buffer_deinit(&ps_utils_qtrace_context_id_tbl[PS_UTILS_QTRACE_HF]);
}


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
)
{
  return ps_utils_qtrace_context_id_tbl[id_tbl];
}