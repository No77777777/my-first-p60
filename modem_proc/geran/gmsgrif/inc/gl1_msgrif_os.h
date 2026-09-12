#ifndef GL1_MSGRIF_OS_H
#define GL1_MSGRIF_OS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  GL1 MSGR Interface
                  OS Header File


DESCRIPTION
   GL1 Msgr IF Rex Signal definitions

EXTERNALIZED FUNCTIONS

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header:

when       who      what, where, why
--------   -------- ---------------------------------------------
29/10/17   km      CR2133917 GL1 support for L+L
21/01/15   jj      CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
17/10/14   jj      CR741205 moving  G2X CNF from l1 task to MSGR 
14/10/13   pa      CR536820: Added GL1 Msgr If Task. Initial version.

===========================================================================*/


/*===========================================================================

                DEFINITIONS AND DECLARATIONS

===========================================================================*/



/*===========================================================================

                         REX SIGNALS

===========================================================================*/
#define GL1_MSGRIF_RF_TASK_SIG                         0x0001
#define GL1_MSGRIF_GFW_ASYNC_INTF_ROUTER_SIG           0x0002

#define GL1_MSGRIF_G2X_INTF_ROUTER_SIG                 0x0004
#define GL1_MSGRIF_CFCM_CPU_MONITOR_SIG                0x0008

#define GL1_MGSRIF_VFRMAP_ROUTER_SIG                   0x0010

/*===========================================================================

                EXTERN DEFINITIONS AND DECLARATIONS

===========================================================================*/

#endif /* GL1_MSGRIF_OS_H */
