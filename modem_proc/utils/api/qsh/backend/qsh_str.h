/*!
  @file
  qsh_str.h

  @brief
  Contains string lookup table declarations.
*/

/*==============================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/utils.mpss/5.4/api/qsh/backend/qsh_str.h#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
12/07/18   mm      CR 2363373: Enable stubs on linux MOB
08/22/18   mm      CR 2300075: Logging cleanup
05/22/18   mm      CR 2223829: Hexsim support
04/02/18   mm      CR 2206706: QTrace adjustment for QTF on MOB
08/18/17   mm      CR 2095406: Added qshrink strings
08/01/16   ca      CR: 1048777 Rule Equation.             
12/29/15   ca      CR:935478: cmapi report, mini dump state collet,
                   Re-arch of rule management validation. 
08/28/15   ca      CR 898746: QSH rule management feature.
06/20/14   ar      Moved qsh_crit_sect_enter() and qsh_crit_sect_leave() to 
                   qsh.c file
05/23/14   mm      Added standardized analysis summary
03/12/14   vd      CR 630063: initial version                   
==============================================================================*/

#ifndef QSH_STR_H
#define QSH_STR_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <qsh_types.h>
#include <qsh_util.h>

/*==============================================================================

                         EXTERNAL VARIABLES

==============================================================================*/

/*! Client names as strings
*/
extern const char * qsh_client_str[QSH_CLT_MAX];

/*! Client names as strings in QShrink section
  @note
  For MOB, map QSR lookup to normal string table due to macro arg limitation.
*/
#if defined(FEATURE_QSH_STUBS) && !defined(FEATURE_HEXSIM)
  #define qsh_client_str_qsr \
    qsh_client_str
  #define qsh_client_str_qsr_pad \
    qsh_client_str_pad
#else
extern const char * qsh_client_str_qsr[QSH_CLT_MAX];
extern const char * qsh_client_str_qsr_pad[QSH_CLT_MAX];
#endif /* FEATURE_QSH_STUBS */

/*! Client names padded to maximum length (crashscope won't pad with %-*s)
*/
extern const char * qsh_client_str_pad[QSH_CLT_MAX];

/*! Category names padded to maximum length
*/
extern const char * qsh_cat_str_pad[QSH_CAT_MAX_IDX];

/*! Messages types padded to maximum length
*/
extern const char * qsh_msg_type_str_pad[QSH_MSG_TYPE_MAX];

/*! Action type as strings */
extern const char * qsh_action_str[QSH_ACTION_MAX_IDX];

/*! Callback mode as strings 
*/
extern const char * qsh_action_mode_str[QSH_ACTION_MODE_MAX];

/*! Callback mode done as strings 
*/
extern const char * qsh_action_mode_done_str[QSH_ACTION_MODE_DONE_MAX];

/*! Event actions as strings
*/
extern const char * qsh_event_action_str[QSH_EVENT_ACTION_MAX];

#endif /* QSH_STR_H */
