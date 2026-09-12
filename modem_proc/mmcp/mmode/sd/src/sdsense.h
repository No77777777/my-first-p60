#ifndef SDSENSE_H
#define SDSENSE_H
/*===========================================================================

               S Y S T E M   D E T E R M I N A T I O N

               I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  This header file contains definitions used internally by the System
  Determination.

  This header file should NOT be included by any source file outside the
  System Determination module.

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sdsense.h#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/20/16   aj       Initial version.

===========================================================================*/

#include "mmcp_variation.h"

#include "customer.h" /* Customer configuration file */
#include "comdef.h"   /* Definition for basic types and macros */
#include "sys.h"



/*===========================================================================
================================= MACRO =====================================
===========================================================================*/



/*===========================================================================
=============================== ENUMERATION =================================
===========================================================================*/

/**
** Enumeration of scan types
*/

typedef enum
{
  SDSENSE_SCAN_NORMAL = 0,
  /* Battery normal, Not stationary */
  SDSENSE_SCAN_STATIONARY,
  /* Battery normal,  Stationary */
  SDSENSE_SCAN_NORMAL_BATT_LOW,
  /* Battery low, Not stationary */
  SDSENSE_SCAN_STATIONARY_BATT_LOW
  /* Battery low,  stationary */

} sdsense_scan_type_e_type;



/**
** Enumeration of registration states
*/

typedef enum
{
  SDSENSE_REG_STT_DEREGISTERED,
  /* Sdsense is not registed with cafi. initial state
     and state when deregistration cnf is recieved
  */
  SDSENSE_REG_STT_REG_PENDING,
  /* registration request has been sent . waiting for cnf
  */
  SDSENSE_REG_STT_REGISTERED,
  /* Cafi registration cnf has been recieved
  */
  SDSENSE_REG_STT_DEREG_PENDING,
  /* DEregistration request has been sent . waiting for cnf
  */

} sdsense_reg_stt_e_type;



/*===========================================================================
=============================================================================
=============================================================================
============================= @FUNCTION DECLARATION ==========================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION sdsense_init

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  Initializes the sdsense component

SIDE EFFECTS
  None.

===========================================================================*/
extern void                   sdsense_init( void );


/*===========================================================================

FUNCTION sdsense_deregister

DESCRIPTION
It deregisters a sub from knowing the Cafi input.
SDsense will update the registered_subs information.
if no subs is registered , SDsense will deregister from Cafi

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void                   sdsense_deregister(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

);
/*===========================================================================

FUNCTION sdsense_register

DESCRIPTION
It registers a sub for knowing the Cafi input.
SDsense will update the registered_subs information.
if no sub was registered to begin with , SDsense will register with Cafi

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void                   sdsense_register(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

);
/*===========================================================================
FUNCTION sdsense_get_curr_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
extern dword               sdsense_get_curr_oos_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
  */
);

/*===========================================================================

FUNCTION sdsense_get_curr_lmtd_bsr_timer

DESCRIPTION
  Returns the pwr save timer that should be used for BSR

DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
extern dword               sdsense_get_curr_lmtd_bsr_timer(


  uint8               curr_count
  /* Number of BSR cycles completed on the stack
  */
);
/*===========================================================================

FUNCTION  sdsense_msgr_handle

DESCRIPTION
 Handles MSGR indications in sdsense module. Calls each individual handler.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void           sdsense_msgr_handle(

  void         *param
  /* sd_msgr_type payload */
);


/*===========================================================================

FUNCTION sdsense_get_scan_state

DESCRIPTION
  Returns the current value of scan_state

DEPENDENCIES
 sdsense_init() should have been called

RETURN VALUE
  scan state

SIDE EFFECTS
  None.

===========================================================================*/
extern sdsense_scan_type_e_type       sdsense_get_scan_state( void );


#endif /* SDSENSE_H */
