/*===========================================================================

                          D S 7 0 7 _ R M S M _ P R O X Y . C

DESCRIPTION

  The Data Services RmSm Proxy'S main operations are to handle 1X Dial String
  Callbacks, set up the port bridge between the MDM and MSM, handle modem 
  control signals such as Data Terminal Ready(DTR), Carrier Detect(CD)
  signals and handle abort commands.

Copyright (c) 2001 - 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707_rmsm.h_v   1.6   23 Jan 2003 16:34:28   randrew  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/src/ds707_rmsm_proxy.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
02/08/12    sk      Feature cleanup.
05/26/11    dvk     Global Variables Cleanup
01/07/11    vs     Changes to make dstask free-floating.
10/13/10    op     Migrated to MSG 2.0 macros
10/12/10    vs     Remove q_init for the watermark queue as it is already 
                   initialized inside dsm_queue_init.
05/21/10    vs     Initial File

===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

