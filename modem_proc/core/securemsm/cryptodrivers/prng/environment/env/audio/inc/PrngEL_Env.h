#ifndef __PRNGEL_ENV_H__
#define __PRNGEL_ENV_H__

/*===========================================================================

                       P R N G E n g i n e D r i v e r

                       H e a d e r  F i l e (e x t e r n a l)

DESCRIPTION
  This header file contains HW Crypto specific declarations.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright (c) 2010 - 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/securemsm/cryptodrivers/prng/environment/env/audio/inc/PrngEL_Env.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/09/11   nk      Added the Mutex Enter and Exit routines
7/25/10    yk     Initial version
============================================================================*/

#include "comdef.h"
#include "qurt.h"
#include "PrngCL_DALIntf.h"
#include <cache_mmu.h>
#include "err.h"
#include "PrngEL.h"

#define PRNGEL_MUTEX_TYPE qurt_mutex_t

#define PRNGEL_MUTEX_INIT()\
        { if(prngel_mutex_init == 0) \
          { qurt_pimutex_init(&prngel_mutex); \
            prngel_mutex_init = 1; \
          }\
        }

#define PRNGEL_MUTEX_ENTER()\
        { if(prngel_mutex_init == 0) \
            ERR_FATAL("Prng mutex not initialized by RC init",0,0,0);\
          qurt_pimutex_lock(&prngel_mutex);\
        }

#define PRNGEL_MUTEX_EXIT() qurt_pimutex_unlock(&prngel_mutex)

#define PRNGCL_ENABLE_CLK() 0

#define PRNGCL_DISABLE_CLK() 0

#define PRNGCL_STATUS_CLK( PTRCLKFLAG ) 

#define PRNGEL_GETDATA(random_ptr, random_len) PrngEL_getdata_QDI(random_ptr, random_len) 
//#define PRNGEL_MEMORY_BARRIER() memory_barrier()
#define PRNGEL_MEMORY_BARRIER()  

#define PRNGEL_INIT() 0

#define PRNGEL_DEINIT() 0

#define PRNGEL_LITE_INIT() 0

#define PRNGEL_LITE_DEINIT() 0 

#endif /*__PRNGEL_ENV_H__ */
