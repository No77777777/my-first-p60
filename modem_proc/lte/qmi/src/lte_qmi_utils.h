/*==============================================================================

                             LTE_QMI_UTILS.H


GENERAL DESCRIPTION
  This file deals with the utility related activities for the LTE QMI service. T

 

Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
==============================================================================*/

/*==============================================================================

                            EDIT HISTORY FOR FILE

  $Header: 
  $DateTime: 
  $Author: 

when        who    what, where, why
--------    ---    -------------------------------------------------------------
11/01/15    psv    created file
==============================================================================*/


#ifndef LTE_QMI_UTILS_H
#define LTE_QMI_UTILS_H

/*==============================================================================

                                INCLUDE FILES

==============================================================================*/

#include <stringl.h>
#include <dsm_pool.h>

/*==============================================================================

                             INTERNAL MACROS

==============================================================================*/


     /*! This macro is a wrapper for the memscpy function */

#define LTE_QMI_MEMCPY(dst_ptr, dst_size, src_ptr, src_size) \
{ \
  size_t ret_size = 0; \
  if (dst_size < src_size) \
  { \
    LTE_RRC_MSG_2_FATAL ( "UTILS: memscpy, dst_size %d bytes < src_size %d, " \
                         "bytes", dst_size, src_size ); \
  } \
  if (src_size > (ret_size = memscpy(dst_ptr, dst_size, src_ptr, src_size))) \
  { \
    LTE_RRC_MSG_2_FATAL ( "UTILS: Truncation in memscpy, bytes copied to dest %d, " \
                         "src size %d bytes", ret_size, src_size ); \
  } \
} 


/*! This macro is a wrapper for the memsmove function */

#define LTE_QMI_MEMMOVE(dst_ptr, dst_size, src_ptr, src_size) \
{ \
  size_t ret_size = 0; \
  if (dst_size < src_size) \
  { \
    LTE_RRC_MSG_2_FATAL ( "UTILS: memsmove, dst_size %d bytes < src_size %d, " \
                         "bytes", dst_size, src_size ); \
  } \
  if (src_size > (ret_size = memsmove(dst_ptr, dst_size, src_ptr, src_size))) \
  { \
    LTE_RRC_MSG_2_FATAL ( "UTILS: Truncation in memsmove, bytes copied to dest %d, " \
                         "src size %d bytes", ret_size, src_size ); \
  } \
} 


/*! @brief Macros for remaining free memory in DSM small item pool
*/
#define LTE_QMI_REM_SMALL_ITEM_POOL_SIZE_IN_BYTES (DSM_POOL_FREE_CNT(DSM_DS_SMALL_ITEM_POOL) \
                                                   * DSM_POOL_ITEM_SIZE(DSM_DS_SMALL_ITEM_POOL))

/*! @brief DSM Pool to use
*/
#define LTE_QMI_DSM_POOL(size) ((size <= LTE_QMI_REM_SMALL_ITEM_POOL_SIZE_IN_BYTES) ? \
                                (DSM_DS_SMALL_ITEM_POOL) : (DSM_DS_LARGE_ITEM_POOL))



#endif /* LTE_QMI_UTILS_H */

