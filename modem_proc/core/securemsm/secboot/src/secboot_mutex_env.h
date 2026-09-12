#ifndef _SECBOOT_MUTEX_ENV_H_
#define _SECBOOT_MUTEX_ENV_H_
/*===========================================================================

  Define image sw type

  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All rights reserved.
  Qualcomm Technologies, Inc. Confidential and Proprietary.

===========================================================================*/

/*=========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/securemsm/secboot/src/secboot_mutex_env.h#1 $
$DateTime: 2019/07/15 05:43:51 $
$Author: pwbldsvc $

when       who                  what, where, why
--------   ----                 ------------------------------------------- 
02/08/19    mk                  initial version

==========================		*/										 
#include "rex.h"
																												 
#define SECBOOT_MUTEX_TYPE rex_crit_sect_type

#define SECBOOT_MUTEX_ENTER()\
        { if(secboot_mutex_init == 0) \
          { \
            rex_init_crit_sect(&secboot_mutex); \
            secboot_mutex_init = 1; \
           } \
          rex_enter_crit_sect(&secboot_mutex);\
        }

#define SECBOOT_MUTEX_EXIT() rex_leave_crit_sect(&secboot_mutex)

 
/**
 * @brief 
 *        
 *
 * @return None
 *
 * @see 
 *
 */
secboot_error_type secboot_util_mutex_enter (void);

/**
 * @brief 
 *        
 *
 * @return None
 *
 * @see 
 *
 */
secboot_error_type secboot_util_mutex_exit(void);
 
#endif
