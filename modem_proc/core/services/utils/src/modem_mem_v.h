#ifndef MODEM_MEM_V_H
#define MODEM_MEM_V_H
/**
  @file modem_mem_v.h
  @brief 
    Contains the heap manager internal interface.

*/
/*===========================================================================

  Copyright (c) 2020 Qualcomm Technologies, Inc. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc.and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/utils/src/modem_mem_v.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/


/*=============================================================================

                           INCLUDE FILES

=============================================================================*/


/*=============================================================================*/

#ifdef FEATURE_HEAP_ALLOC_PROFILE

/* Generic profile struct which could be used 
   to profile any modem_mem* APIs */
typedef struct modem_mem_profile {
  /* Delta before and after the operation */
  uint32 time_delta;
  /* caller pointer */
  void * caller_ptr;
  /* Thread ID */
  unsigned long  task_id;
} modem_mem_profile_t;

/* We would use this Mask to check if time stamp exceeds 32 bits */
#define MSB_WORD              (0xFFFFFFFF)
#define MSB_WORD_BITS         (32)
/* The param has to be defined from outside of utils. If not we 
   default it to a specific value */
#ifndef MODEM_ALLOC_LOG_MAX
#define MODEM_ALLOC_LOG_MAX  (1024)
#endif
#endif

#endif /* MODEM_MEM_V_H */
