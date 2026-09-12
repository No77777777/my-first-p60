#ifndef TDSRRCDISPATCHER_H
#define TDSRRCDISPATCHER_H
/*===========================================================================
                        R R C  D I S P A T C H E R  H E A D E R

DESCRIPTION

  This module is the header file for the RRC Dispatcher module.
	
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdispatcher.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
08/18/10   ss      Added SIB19 logging code.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "tdsrrcsibproci.h"
#include "tdsrrccmd_v.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#define TDSRRC_SRB5_SDU_LEN 1   /* For SRB, RLC size can utmost be 3/5/10 bits */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#define TDSMAX_SIZE_OF_SDU 512

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef struct
{
    uint8 rb_id;
    uint8 channel_type;
    uint8 message_type;
    uint16 sdu_length;
    uint32 ota_time;
    uint8 signalling_message[TDSMAX_SIZE_OF_SDU];

}tdsrrc_log_dl_last_sigalling_message_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */





/* =======================================================================
**                        Function Declarations
** ======================================================================= */


/*===========================================================================

FUNCTION  RRC_DISPATCHER

DESCRIPTION
 
  The RRC_DISPATCHER is the central wait-loop for the RRC task. This
  function constantly waits for events to be received. When events are
  received, they are dispatched to the appropriate function to be processed.
  
  This function is the only wait-loop in the RRC task. No other procedures
  can perform a local wait loop. 
 
  The set of events that the RRC task waits for is determined by RRC's
  event dispatch table. This table must be created before tdsrrc_dispatcher is
  called. Regardless of the current state of RRC, the set of events that
  the tdsrrc_dispatcher is waiting for is the same.
 
DEPENDENCIES

  Event dispatch table must be created before this function is called.  
   
RETURN VALUE

  None. Note that this function does not return unless the RRC task is being
  shut down. (*** This is TBD ***).
  
SIDE EFFECTS

  None.

===========================================================================*/

extern void tdsrrc_dispatcher ( void );

/*===========================================================================
FUNCTION RRC_EVENT_SIG

DESCRIPTION
  Wait on the specified signal.  Handle signals as they come in. Only time
  critical watchdog and other signals are acknowledged.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECT
  Some signals are ignored till we're done with the signal

===========================================================================*/
void tdsrrc_event_wait( rex_sigs_type sig);

/*===========================================================================

FUNCTION  RRC_DISPATCH_INTERNAL_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's internal
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_internal_commands( void );

/*===========================================================================

FUNCTION  RRC_DISPATCH_L1_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's l1
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_l1_commands( void );

/*===========================================================================

FUNCTION  RRC_DISPATCH_MAC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's mac
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_mac_commands( void );

/*===========================================================================

FUNCTION  RRC_DISPATCH_RLC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's rlc
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_rlc_commands( void );

/*===========================================================================

FUNCTION  RRC_DISPATCH_MM_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's mm
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_mm_commands( void );

/*===========================================================================

FUNCTION  RRC_DISPATCH_RR_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's RR
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcdispatch_rr_commands( void );

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
/*===========================================================================

FUNCTION    tdsrrc_log_dl_sig_message_for_ext_sibs

DESCRIPTION
    This function will log the downlink encoded OTA messages into QXDM for extension sibS.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_log_dl_sig_message_for_ext_sibs
(
  tdsrrc_SIB_type sib_type,
  tdssib_events_index_e_type index
);
#endif

/*===========================================================================

FUNCTION  RRC_DISPATCH_MSGR_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's Message Router
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_dispatch_msgr_commands( void );

#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
/*===========================================================================

FUNCTION    tdsrrcdispatch_get_dl_asn1_message_type

DESCRIPTION
  This function determines the type of the message received and returns this
  information as a byte suitable for reporting in a QXDM event.  This value
  is *not* the same as the ASN.1 based message type values returned by other
  similar functions.  See the rrclogging.h file for details.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns the message type if successful. Otherwise it returns 0xff.
  
SIDE EFFECTS
  None

===========================================================================*/
 uint8 tdsrrcdispatch_get_dl_asn1_message_type(
  tdsrrc_cmd_type              *cmd_ptr /* To hold received command */
);
#endif

#endif /* TDSRRCDISPATCHER_H */

