#ifndef DS_MUX_TASKI_H
#define DS_MUX_TASKI_H
/*===========================================================================

                                 D S   M U X   T A S K I

                                 H E A D E R   F I L E

DESCRIPTION
  This is the internal header file for the MUX Task. This file
  contains all the functions, definitions and data types needed for internal
  to interface to the MUX Task.


  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/src/ds_mux_taski.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad      initial version
===========================================================================*/
    
/*===========================================================================

                      INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "rex.h"
#include "queue.h"
#include "ds_mux_defs.h"

extern rex_tcb_type  ds_mux_tcb;
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Command queues handled by DS_MUX task 
---------------------------------------------------------------------------*/
extern q_type        ds_mux_cmd_q;
extern q_type        ds_mux_cmd_free_q;

/*--------------------------------------------------------------------------
  The command buffer count. (Count is TBD)
---------------------------------------------------------------------------*/
#define DS_MUX_CMD_BUF_CNT  (200)

/*--------------------------------------------------------------------------
  Datatype to hold signal, command handlers 
---------------------------------------------------------------------------*/
extern ds_mux_sig_handler_info_type ds_mux_sig_handler[DS_MUX_MAX_SIGNALS];
extern ds_mux_cmd_handler_type      ds_mux_cmd_handler[DS_MUX_MAX_DEFINED_CMD_TYPES];
extern ds_mux_cmd_type              ds_mux_cmd_buf[ DS_MUX_CMD_BUF_CNT];

/*--------------------------------------------------------------------------
  Pool of signals being currently enabled for DS MUX main processing loop
---------------------------------------------------------------------------*/
extern rex_sigs_type ds_mux_enabled_sig_mask;

/*--------------------------------------------------------------------------
  DS_MUX's own critical section
---------------------------------------------------------------------------*/

extern ds_mux_crit_sect_type  global_ds_mux_crit_section;



/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION DS_MUX_DEFAULT_SIG_HANDLER()

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean 
ds_mux_default_sig_handler
(
  ds_mux_sig_enum_type sig,                   /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
);

/*===========================================================================

FUNCTION DS_MUX_DEFAULT_CMD_HANDLER()

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void 
ds_mux_default_cmd_handler
(
  ds_mux_cmd_enum_type cmd,         /* Actual command to be processed         */
  void *user_data_ptr            /* Command specific user parameters       */
);


/*===========================================================================
FUNCTION DS_MUX_POWERUP()

DESCRIPTION

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void 
ds_mux_powerup
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================

FUNCTION DS_MUX_PROCESS_CMD()

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/

boolean 
ds_mux_process_cmd
( 
  void
);

#endif /* DS_MUX_TASKI_H */
