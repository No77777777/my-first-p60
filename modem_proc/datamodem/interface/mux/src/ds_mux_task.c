/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     D S   M U X   T A S K

GENERAL DESCRIPTION
  This file contains all the functions, definitions and data types needed 
  for MUX Task.  Signal and command definitions for the MUX Task, and functions
  that should be used by other tasks to send commands to the MUX 
  Task are defined here.

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/src/ds_mux_task.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad     Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "queue.h"
#include "err.h"
#include "ps_utils.h"
#include "dog_hb_rex.h"
#include "ds_mux.h"
#include "ds_mux_task.h"
#include "ds_mux_taski.h"
#include "ds_mux_svc.h"
#include "rcinit_rex.h"
#include "ds_mux_platform.h"


rex_tcb_type    * rex_ds_mux_tcb;

/*--------------------------------------------------------------------------
  Defines for DOG. Assuming DOG_DYNAMIC_API feat is ON, since they (core bsp)
  dont allow anymore additions to dog files.
---------------------------------------------------------------------------*/
dog_report_type   ds_mux_dog_rpt_id           = 0;      /* Initial Safety */

/*===========================================================================

                     EXTERNAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================
FUNCTION DS_MUX_TASK()

DESCRIPTION
  This function is the entry point and main processing loop for the DS MUX TASK.

DEPENDENCIES
  Does not Return.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_mux_task
(
  uint32 ignored    /* Parameter received from Main Control task - ignored */
)
{
  rex_sigs_type     loop_sigs;      /* Signals to be processed within a task loop */
  rex_sigs_type     set_sigs = 0;   /* Currently set signals               */
  uint32            i;

  /* variables for the bit field conversion */
  uint32               sig_buffer[DS_MUX_MAX_SIGNALS];
  ds_mux_sig_enum_type curr_sig;
  uint32               num = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  
  rex_ds_mux_tcb = rex_self();

#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  rcinit_handshake_startup();

  ds_mux_dog_rpt_id = dog_hb_register_rex(
                (rex_sigs_type)1 << (rex_sigs_type)DS_MUX_DOG_HB_REPORT_SIGNAL);

  /*-------------------------------------------------------------------------
    Main task loop. Never exit!
  -------------------------------------------------------------------------*/
  for( ;;)
  {
    /*-----------------------------------------------------------------------
      Wait for work to do. No return from wait until one of the enabled
      signals is set. Even if we come out of wait, process only those
      signals which are currently enabled.
    -----------------------------------------------------------------------*/

    set_sigs |= rex_get_sigs( rex_ds_mux_tcb );

    set_sigs &= ds_mux_enabled_sig_mask;

    /*-----------------------------------------------------------------------
      Enter into the Wait only if no requested signal events have occurred
    -----------------------------------------------------------------------*/
    if( set_sigs == 0 )
    {
      set_sigs = rex_wait( ds_mux_enabled_sig_mask );
      set_sigs &= ds_mux_enabled_sig_mask;
    }

    (void)rex_clr_sigs( rex_ds_mux_tcb, set_sigs );

    loop_sigs = set_sigs & ~( (rex_sigs_type) 1
                             << (rex_sigs_type) DS_MUX_SIG_MASK_CHANGE_SIGNAL );
    set_sigs = 0;

#ifdef TEST_FRAMEWORK
      #error code not present
#endif /* TEST_FRAMEWORK */
    /*-----------------------------------------------------------------------
      Now handle the active signals one by one.
    -----------------------------------------------------------------------*/
    num = ds_mux_find_set_bits(&loop_sigs, 1, sig_buffer);
    for( i=0; i < num; i++)
    {
      curr_sig = (ds_mux_sig_enum_type)sig_buffer[i];
      loop_sigs &= ~( (rex_sigs_type) 1 <<
                      (rex_sigs_type) curr_sig );

      if ( ds_mux_sig_handler[curr_sig].sig_handler
            (
              curr_sig ,
              ds_mux_sig_handler[curr_sig].user_data_ptr
            ) == FALSE )
      {
        set_sigs |= ( (rex_sigs_type) 1 <<
                      (rex_sigs_type) curr_sig);
      }
    }

    ASSERT(0 == loop_sigs);
  } /* forever */

}/*ds_mux_task*/

