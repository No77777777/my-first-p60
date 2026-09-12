/*!
  @file
  qsh_legacy.h

  @brief
  Glue code to maintain backward compatability. This should be removed once
  all clients implement changes.
*/

/*==============================================================================

  Copyright (c) 2014 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/utils.mpss/5.4/api/qsh/backend/qsh_legacy.h#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
06/19/18   yx      CR 2263684: Ulog to QTrace migration in QSH_LOG
01/22/15   mm      Initial check-in             
==============================================================================*/

#ifndef QSH_LEGACY_H
#define QSH_LEGACY_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <qsh_types.h>
#include <qsh_util.h>
#include <qtrace.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

#define FEATURE_QSH_LEGACY


/*! @brief Expands to name of category index enum.
*/
#define QSH_CAT_IDX(cat) cat ## _IDX

/*!
  Macros to map QSH_LOG() category to qtrace tag.
*/
#define QSH_CAT_TPUT_TAG \
  "TPUT"
#define QSH_CAT_CFG_TAG \
  "CFG"
#define QSH_CAT_DSM_TAG \
  "DSM"
#define QSH_CAT_PERF_TAG \
  "PERF"
#define QSH_CAT_OTHER_TAG \
  "OTHER"
#define QSH_CAT_EVENT_TAG \
  "EVENT"

/*!
  Macros to map qtrace level to string.
*/
#define QSH_MSG_TYPE_HIGH_STR \
  "HIGH"
#define QSH_MSG_TYPE_MEDIUM_STR \
  "MED"
#define QSH_MSG_TYPE_INFO_STR \
  "INFO"
#define QSH_MSG_TYPE_DEBUG_STR \
  "DEBUG"

/*! @brief Provides a mapping between category enum and string representation.
*/
#define QSH_CAT_TPUT_STR    "TPUT"
#define QSH_CAT_CFG_STR     "CFG"
#define QSH_CAT_DSM_STR     "DSM"
#define QSH_CAT_PERF_STR    "PERF"
#define QSH_CAT_OTHER_STR   "OTHER"
#define QSH_CAT_EVENT_STR   "EVENT"

/*! @brief Category names padded to maximum width.
*/
#define QSH_CAT_TPUT_STR_PAD    QSH_CAT_TPUT_STR " "
#define QSH_CAT_CFG_STR_PAD     QSH_CAT_CFG_STR "  "
#define QSH_CAT_DSM_STR_PAD     QSH_CAT_DSM_STR "  "
#define QSH_CAT_PERF_STR_PAD    QSH_CAT_PERF_STR " "
#define QSH_CAT_OTHER_STR_PAD   QSH_CAT_OTHER_STR ""
#define QSH_CAT_EVENT_STR_PAD   QSH_CAT_EVENT_STR ""

/*!
  Macros to map QSH_LOG() msg_type to qtrace level.
*/
#define QSH_MSG_TYPE_HIGH_LEVEL \
  QTRACE_LEVEL_HIGH
#define QSH_MSG_TYPE_MEDIUM_LEVEL \
  QTRACE_LEVEL_MED
#define QSH_MSG_TYPE_INFO_LEVEL \
  QTRACE_LEVEL_DEBUG
#define QSH_MSG_TYPE_DEBUG_LEVEL \
  QTRACE_LEVEL_DEBUG

#define QSH_MSG_TYPE_HIGH_LOG_n(n, client, category, summary, ...) \
  QSH_LOG_HIGH_n(n, client, category, summary, ##__VA_ARGS__)
  
#define QSH_MSG_TYPE_MEDIUM_LOG_n(n, client, category, summary, ...) \
  QSH_LOG_MEDIUM_n(n, client, category, summary, ##__VA_ARGS__)
  
#define QSH_MSG_TYPE_INFO_LOG_n(n, client, category, summary, ...) \
  QSH_LOG_INFO_n(n, client, category, summary, ##__VA_ARGS__)
  
#define QSH_MSG_TYPE_DEBUG_LOG_n(n, client, category, summary, ...) \
  QSH_LOG_DEBUG_n(n, client, category, summary, ##__VA_ARGS__)

#if !defined(QSH_DISABLE_HIGH) && defined(FEATURE_QSH_ON_TARGET)
  #define QSH_LOG_HIGH_n(n, client, category, summary, ...) \
    do \
    { \
       QTRACE_S( \
        QTRACE_CONTEXT_ID_ANALYSIS, \
        client, \
        QSH_MSG_TYPE_HIGH_LEVEL, \
        SYS_MODEM_AS_ID_NONE, \
        category ## _TAG, \
        client ## _STR_PAD " | " summary " | " \
        category ## _TAG , \
        ##__VA_ARGS__); \
    } while(0)
#else
  #define QSH_LOG_HIGH_n(n, client, category, summary, ...)
#endif

#if !defined(QSH_DISABLE_MEDIUM) && defined(FEATURE_QSH_ON_TARGET)
  #define QSH_LOG_MEDIUM_n(n, client, category, summary, ...) \
    do \
    { \
       QTRACE_S( \
        QTRACE_CONTEXT_ID_ANALYSIS, \
        client, \
        QSH_MSG_TYPE_MEDIUM_LEVEL, \
        SYS_MODEM_AS_ID_NONE, \
        category ## _TAG, \
        client ## _STR_PAD " | " summary " | " \
        category ## _TAG , \
        ##__VA_ARGS__); \
    } while(0)
#else
  #define QSH_LOG_MEDIUM_n(n, client, category, summary, ...)
#endif

#if !defined(QSH_DISABLE_INFO) && defined(FEATURE_QSH_ON_TARGET)
  #define QSH_LOG_INFO_n(n, client, category, summary, ...) \
    do \
    { \
       QTRACE_S( \
        QTRACE_CONTEXT_ID_ANALYSIS, \
        client, \
        QSH_MSG_TYPE_INFO_LEVEL, \
        SYS_MODEM_AS_ID_NONE, \
        category ## _TAG, \
        client ## _STR_PAD " | " summary  " | " \
        category ## _TAG , \
        ##__VA_ARGS__); \
    } while(0)
#else
  #define QSH_LOG_INFO_n(n, client, category, summary, ...)
#endif

#if !defined(QSH_DISABLE_DEBUG) && defined(FEATURE_QSH_ON_TARGET)
  #define QSH_LOG_DEBUG_n(n, client, category, summary, ...) \
    do \
    { \
       QTRACE_S( \
        QTRACE_CONTEXT_ID_ANALYSIS, \
        client, \
        QSH_MSG_TYPE_DEBUG_LEVEL, \
        SYS_MODEM_AS_ID_NONE, \
        category ## _TAG, \
        client ## _STR_PAD " | " summary  " | " \
        category ## _TAG , \
        ##__VA_ARGS__); \
    } while(0)
#else
  #define QSH_LOG_DEBUG_n(n, client, category, summary, ...)
#endif

/*! Legacy log macros maintained here.
*/
#define QSH_LOG_HIGH_0(client, category, summary) \
  QSH_LOG_HIGH_n(0, client, category, summary)
#define QSH_LOG_HIGH_1(client, category, summary, a1) \
  QSH_LOG_HIGH_n(1, client, category, summary, a1)
#define QSH_LOG_HIGH_2(client, category, summary, a1, a2) \
  QSH_LOG_HIGH_n(2, client, category, summary, a1, a2)
#define QSH_LOG_HIGH_3(client, category, summary, a1, a2, a3) \
  QSH_LOG_HIGH_n(3, client, category, summary, a1, a2, a3)
#define QSH_LOG_HIGH_4(client, category, summary, a1, a2, a3, a4) \
  QSH_LOG_HIGH_n(4, client, category, summary, a1, a2, a3, a4)
#define QSH_LOG_HIGH_5(client, category, summary, a1, a2, a3, a4, a5) \
  QSH_LOG_HIGH_n(5, client, category, summary, a1, a2, a3, a4, a5)
#define QSH_LOG_HIGH_6(client, category, summary, a1, a2, a3, a4, a5, a6) \
  QSH_LOG_HIGH_n(6, client, category, summary, a1, a2, a3, a4, a5, a6)
#define QSH_LOG_HIGH_7(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7) \
  QSH_LOG_HIGH_n(7, client, category, summary, a1, a2, a3, a4, a5, a6, a7)
#define QSH_LOG_HIGH_8(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8) \
  QSH_LOG_HIGH_n(8, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8)
#define QSH_LOG_HIGH_9(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9) \
  QSH_LOG_HIGH_n(9, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9)
#define QSH_LOG_HIGH_10(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9, a10) \
  QSH_LOG_HIGH_n(10, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10)
    
#define QSH_LOG_MEDIUM_0(client, category, summary) \
  QSH_LOG_MEDIUM_n(0, client, category, summary)
#define QSH_LOG_MEDIUM_1(client, category, summary, a1) \
  QSH_LOG_MEDIUM_n(1, client, category, summary, a1)
#define QSH_LOG_MEDIUM_2(client, category, summary, a1, a2) \
  QSH_LOG_MEDIUM_n(2, client, category, summary, a1, a2)
#define QSH_LOG_MEDIUM_3(client, category, summary, a1, a2, a3) \
  QSH_LOG_MEDIUM_n(3, client, category, summary, a1, a2, a3)
#define QSH_LOG_MEDIUM_4(client, category, summary, a1, a2, a3, a4) \
  QSH_LOG_MEDIUM_n(4, client, category, summary, a1, a2, a3, a4)
#define QSH_LOG_MEDIUM_5(client, category, summary, a1, a2, a3, a4, a5) \
  QSH_LOG_MEDIUM_n(5, client, category, summary, a1, a2, a3, a4, a5)
#define QSH_LOG_MEDIUM_6(client, category, summary, a1, a2, a3, a4, a5, a6) \
  QSH_LOG_MEDIUM_n(6, client, category, summary, a1, a2, a3, a4, a5, a6)
#define QSH_LOG_MEDIUM_7(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7) \
  QSH_LOG_MEDIUM_n(7, client, category, summary, a1, a2, a3, a4, a5, a6, a7)
#define QSH_LOG_MEDIUM_8(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8) \
  QSH_LOG_MEDIUM_n(8, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8)
#define QSH_LOG_MEDIUM_9(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9) \
  QSH_LOG_MEDIUM_n(9, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9)
#define QSH_LOG_MEDIUM_10(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9, a10) \
  QSH_LOG_MEDIUM_n(10, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10)

#define QSH_LOG_INFO_0(client, category, summary) \
  QSH_LOG_INFO_n(0, client, category, summary)
#define QSH_LOG_INFO_1(client, category, summary, a1) \
  QSH_LOG_INFO_n(1, client, category, summary, a1)
#define QSH_LOG_INFO_2(client, category, summary, a1, a2) \
  QSH_LOG_INFO_n(2, client, category, summary, a1, a2)
#define QSH_LOG_INFO_3(client, category, summary, a1, a2, a3) \
  QSH_LOG_INFO_n(3, client, category, summary, a1, a2, a3)
#define QSH_LOG_INFO_4(client, category, summary, a1, a2, a3, a4) \
  QSH_LOG_INFO_n(4, client, category, summary, a1, a2, a3, a4)
#define QSH_LOG_INFO_5(client, category, summary, a1, a2, a3, a4, a5) \
  QSH_LOG_INFO_n(5, client, category, summary, a1, a2, a3, a4, a5)
#define QSH_LOG_INFO_6(client, category, summary, a1, a2, a3, a4, a5, a6) \
  QSH_LOG_INFO_n(6, client, category, summary, a1, a2, a3, a4, a5, a6)
#define QSH_LOG_INFO_7(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7) \
  QSH_LOG_INFO_n(7, client, category, summary, a1, a2, a3, a4, a5, a6, a7)
#define QSH_LOG_INFO_8(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8) \
  QSH_LOG_INFO_n(8, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8)
#define QSH_LOG_INFO_9(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9) \
  QSH_LOG_INFO_n(9, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9)
#define QSH_LOG_INFO_10(client, category, summary, a1, a2, a3, a4, a5, a6, \
  a7, a8, a9, a10) \
  QSH_LOG_INFO_n(10, client, category, summary, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10)

/*! since RESET was removed */
//#define QSH_ACTION_RESET  QSH_ACTION_NONE

/*! @brief Parameters passed into the call back functions 
    registered with QSH using the function qsh_reg_client_with_cb(...) . 
*/
typedef struct
{
  /*! whether to run analysis, reset stats ... */
  qsh_action_e action;   
  /*! category mask. Use enum qsh_cat_e values to define mask e.g. 
      QSH_CAT_CFG|QSH_CAT_PERF (since the values are bit mask simple
      "or" will work) */
  uint32 category_mask;  
  /*! whether the registered CB can run in sync/async mode.
      When called with async_allowed = false the clients MUST RUN 
      the registered CB in the same task context which invoked their call back  
      (e.g. error fatal). When called with async_allowed = true
      the clients have an option to run their registered CB in async mode. 
      The client should call qsh_analysis_async_done() when async analysis
      has completed. */
  boolean async_allowed; 
} qsh_cb_params_s;

/*! @brief Analysis functions for clients registered with 
    qsh_reg_client_with_cb(...). 
 
    Argument is qsh_cb_params_s. Check the definition of the struct above. 
    Returns true for CB running in sync mode and false for async mode.
*/
typedef boolean (*qsh_analysis_cb_type) (qsh_cb_params_s *);

/*! @brief Parameters for invoking QSH. 
*/
typedef struct
{ 
  /*! client */
  qsh_clt_e client;   
  /*! whether to run analysis, reset stats ... */   
  qsh_action_e action;   
  /*! category mask. Use enum qsh_cat_e values to define mask e.g. 
      QSH_CAT_CFG|QSH_CAT_PERF (since the values are bit mask simple
      "or" will work) */
  uint32 category_mask;  
  /*! whether the registered CB can run in sync/async mode.
      When called with async_allowed = false the clients MUST RUN 
      the registered CB in the same task context which invoked their call back  
      (e.g. error fatal). When called with async_allowed = true
      the clients have an option to run their registered CB in async mode. */
  boolean async_allowed; 
} qsh_invoke_params_s;

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  qsh_reg_client_with_cb

==============================================================================*/
/*!
  @brief
  Function for the external modules to register their call back which
  can be called when the analysis is performed by QSH.
 
  @note
  Clients are supposed to update arguments major_ver and minor_ver when changes
  are made in their callback function

*/
/*============================================================================*/
void qsh_reg_client_with_cb
(
  qsh_clt_e client, /*!< client registering for the call back*/
  qsh_analysis_cb_type analysis_cb_ptr, /*!< call back for analysis */
  uint16 major_ver, /*!< major version for the CB */
  uint16 minor_ver /*!< minor version for the CB */
);

/*==============================================================================

  FUNCTION:  qsh_invoke_cb

==============================================================================*/
/*!
  @brief
  To invoke the call backs registered with QSH. 
 
  @note The CB registered through qsh_reg_client_with_cb(...) SHOULD NOT CALL
  this function else there will be an assertion.
 
  @return
  None.
*/
/*============================================================================*/
void qsh_invoke_cb
(
  qsh_invoke_params_s *params /*!< invoke parameters */
);

#endif /* QSH_LEGACY_H */
