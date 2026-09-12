/*!
  @file
  qsh_mdump.h

  @brief
  Contains types common to client, invoke, and ext interfaces.
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

$Header: //components/rel/utils.mpss/5.4/api/qsh/qsh_mdump.h#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
05/11/20   av      CR2680695: A2 MDUMP Changes
06/10/19   am      CR2468188: Fixes to backup QSH/QTEACE data correctly
12/12/18   rn      CR2359853: Minidump changes for Hercules
08/09/18   am      CR2281003: FR51286 Minidump optimizations
01/12/18   am      CR2178624: bring qsh_mdump_smem_info under uncompressed region
10/06/17   mm      CR 2120679: Rebase UTILS 6.2.1
04/17/17   am      Initial draft
==============================================================================*/

#ifndef QSH_MDUMP_H
#define QSH_MDUMP_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <qsh_types_clt_autogen.h>
#include <sys.h>
#include <qurt_thread.h>


/*==============================================================================

                     EXTERNAL DEFINITION AND TYPES

==============================================================================*/

//#define FEATURE_QSH_MDUMP

#ifdef FEATURE_QSH_MDUMP
#define QSH_MDUMP_FN_ATTR  __attribute__((section(".qsh_mdump_fn"))) __attribute__((noinline))
#else
#define QSH_MDUMP_FN_ATTR
#endif

/*!
  @brief
  Client should use qsh_md_crash_cat_e to report crash category. 

  Enum to represent crash category.
*/

typedef enum {
  QSH_MDUMP_CRASH_CAT_UNKNOWN = 0, 
  QSH_MDUMP_CRASH_CAT_MMOC,
  QSH_MDUMP_CRASH_CAT_LRRC_DEADLOCK_TIMEOUT,
  QSH_MDUMP_CRASH_CAT_LML1_DEADLOCK_TIMEOUT,
} qsh_mdump_crash_cat_e;

/*!
  @brief
  Structure to contain MDUMP info for SMEM entry.

  To be used by Stability.

*/
typedef struct {
  void*        address;    /*!< Start address of MDUMP */
  size_t       size;       /*!< Size of MDUMP */
} qsh_mdump_smem_info_s;

#if 0
/*==============================================================================

  FUNCTION:  qsh_mdump_collect

==============================================================================*/
/*!
  @brief 
  API for dumping global variables by clients.
  
*/
/*============================================================================*/

QSH_MDUMP_FN_ATTR void qsh_mdump_collect
(
  /*! Address of the global to dump */
  void*                     addr, 
  /*! Size of the global */
  size_t                    size
);
#endif
/*==============================================================================

  FUNCTION:  qsh_mdump_prioritize_client

==============================================================================*/
/*!
  @brief 
  API to prioritize clients for MDUMP collection. This ensures that the MDUMP 
  for the prioritized clients are collected first which in turn makes the MDUMP
  more useful since the MDUMP is capped by memory available.
  
  @note
  Reporting client can prioritize multiple clients by calling this API 
  repeatedly once for each client.

  This API is NOP if client is already prioritized or MDUMP for that client is 
  already collected.
  
*/
/*============================================================================*/
QSH_MDUMP_FN_ATTR void qsh_mdump_prioritize_client
(
  /*! QSH client which wants to prioritize another QSH client */
  qsh_clt_e                  reporting_client,
  /*! QSH client which needs to be prioritized */
  qsh_clt_e                  priority_client
);

/*==============================================================================

  FUNCTION:  qsh_mdump_set_crash_client

==============================================================================*/
/*!
  @brief 
  QSH API to update crash client at error fatal.
  
*/
/*============================================================================*/
void qsh_mdump_set_crash_client
(
  /*! QSH client which wants to set crash client */
  qsh_clt_e                  reporting_client,
  /*! QSH crash client */
  qsh_clt_e                  client
);

/*==============================================================================

FUNCTION:  qsh_mdump_set_crash_subs_id

==============================================================================*/
/*!
@brief 
QSH API to update crash subscription id at error fatal.

*/
/*============================================================================*/
void qsh_mdump_set_crash_subs_id
(
  /*! QSH client which wants to set subscription ID of crashing client */
  qsh_clt_e                  reporting_client,
  /*! Subscription id of crashing client */
  sys_modem_as_id_e_type     subs_id
);

/*==============================================================================

FUNCTION:  qsh_mdump_set_crash_category

==============================================================================*/
/*!
@brief 
QSH API to update crash category at error fatal.

*/
/*============================================================================*/
void qsh_mdump_set_crash_category
(
  /*! QSH client setting the crash category */
  qsh_clt_e                  reporting_client,
  /*! Crash category */
  qsh_mdump_crash_cat_e      category
);

/*==============================================================================

FUNCTION:  qsh_mdump_set_crash_rat

==============================================================================*/
/*!
@brief 
QSH API to update crash RAT at error fatal.

*/
/*============================================================================*/
void qsh_mdump_set_crash_rat
(
  /*! QSH client providing the crashing RAT */
  qsh_clt_e                  reporting_client,

  /*! Active RAT in which crash occurred */
  sys_sys_mode_e_type        crash_rat
);

/*==============================================================================

FUNCTION:  qsh_mdump_map_thread_id

==============================================================================*/
/*!
@brief 
QSH API to map thread_id to client id

*/
/*============================================================================*/
void qsh_mdump_map_thread_id
(
  qsh_clt_e                  client, 
  qurt_thread_t              thread_id
);

/*==============================================================================

  FUNCTION:  qsh_mdump_collect_high

==============================================================================*/
/*!
  @brief 
  API to save high priority global variables by clients.
  
*/
/*============================================================================*/
QSH_MDUMP_FN_ATTR void qsh_mdumpi_collect_high
(
  /*! Address of the global to dump */
  const void* const          addr, 
  /*! Size of the global */
  size_t                     size,

  const char                *file,
  uint32                     line
);

#define qsh_mdump_collect_high(addr,size) \
    qsh_mdumpi_collect_high(addr,size,__FILENAME__,__LINE__);


/*==============================================================================

  FUNCTION:  qsh_mdump_collect_low

==============================================================================*/
/*!
  @brief 
  API to save low priority global variables by clients.
  
*/
/*============================================================================*/

QSH_MDUMP_FN_ATTR void qsh_mdumpi_collect_low
(
  /*! Address of the global to dump */
  const void* const          addr, 
  /*! Size of the global */
  size_t                     size,

  const char                *file,
  uint32                     line
);

#define qsh_mdump_collect_low(addr,size) \
    qsh_mdumpi_collect_low(addr,size,__FILENAME__,__LINE__);

/*==============================================================================

  FUNCTION:  qsh_mdump_smem_info

==============================================================================*/
/*!
  @brief 
  The API will return start addr and size of MDUMP to be used by stability for 
  SMEM entry.

  To be used by Stability.
  
*/
/*============================================================================*/
QSH_MDUMP_FN_ATTR void qsh_mdump_smem_info
(
  qsh_mdump_smem_info_s *smem_info
);

/*==============================================================================

  FUNCTION:  qsh_mdump_save_heap_data

==============================================================================*/
/*!
  @brief 
  Function to check if heap is collected by MDUMP.

  TRUE: Heap will be collected by MDUMP
  FALSE: Heap will not be collected by MDUMP and assumed complete heap is 
       backed up by minidump framework.
  
*/
/*============================================================================*/
QSH_MDUMP_FN_ATTR boolean qsh_mdump_save_heap_data
(
  void
);

#endif
