#ifndef SRCH_PCH_H
#define SRCH_PCH_H
/*===========================================================================
   FILE: srch_parm.h

GENERAL DESCRIPTION:
      This header file contains parameter definitions used by search module.
      
      
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c)2000-2012 by Qualcomm Technologies, Inc. All Rights Reserved.
      
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchparm.h_v   1.34   11 Jul 2002 21:30:46   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/srchpch.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
1/14/15   psr     Created file

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "l1def.h"
#include "wl1m.h"

/*===========================================================================

                        GLOBAL DECLARATIONS

===========================================================================*/


typedef struct
{
  /* Flag to indicate that PCH cleanup is pending */
  boolean srch_pch_cleanup_pending;
}srchpch_ext_cntrl_params_type;

extern srchpch_ext_cntrl_params_type srchpch_ext_cntrl_params[WCDMA_NUM_SUBS];

void srchpch_ext_cntrl_params_init(void);

void srchpch_int_cntrl_params_init(void);


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/



#endif /* SRCH_PCH_H */

