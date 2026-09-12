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

  $Header: //components/rel/core.mpss/10.0/securemsm/cryptodrivers/prng/environment/env/amss/inc/PrngEL_Env.h#1 $
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
#include "PrngCL.h"
#include <cache_mmu.h>
#include "err.h"

PrngEL_Result_Type PRNGEL_ENABLE_CLK_CHK(void);
PrngEL_Result_Type PRNGEL_DISABLE_CLK_CHK(void);
void PRNGEL_MUTEX_INIT_CHK(void);
void PRNGEL_MUTEX_ENTER_CHK(void);
void PRNGEL_MUTEX_EXIT_CHK(void);

#define PRNGEL_MUTEX_TYPE qurt_mutex_t

#define PRNGEL_MUTEX_INIT() PRNGEL_MUTEX_INIT_CHK()

#define PRNGEL_MUTEX_ENTER() PRNGEL_MUTEX_ENTER_CHK()

#define PRNGEL_MUTEX_EXIT() PRNGEL_MUTEX_EXIT_CHK()

#define PRNGCL_ENABLE_CLK() PRNGEL_ENABLE_CLK_CHK()

#define PRNGCL_DISABLE_CLK() PRNGEL_DISABLE_CLK_CHK()

#define PRNGCL_STATUS_CLK( PTRCLKFLAG ) PrngCL_DAL_Clock_Status( PTRCLKFLAG )

#define PRNGEL_MEMORY_BARRIER() memory_barrier()

#define PRNGEL_GETDATA(random_ptr, random_len) PrngCL_getdata(random_ptr, random_len) 

#define PRNGEL_INIT() PrngCL_init() 

#define PRNGEL_DEINIT() PrngCL_deinit()

#define PRNGEL_LITE_INIT() PrngCL_lite_init()

#define PRNGEL_LITE_DEINIT() PrngCL_lite_deinit()

#endif /*__PRNGEL_ENV_H__ */
