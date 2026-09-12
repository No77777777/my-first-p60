#ifndef USIM_QSH_EXT_H
#define USIM_QSH_EXT_H
/*===========================================================================


                U I M   Q S H   E X T   H E A D E R


DESCRIPTION
    This is the header file for events that UIM notifies to the QSH framework

   Copyright (c) 2016, 2019 by QUALCOMM Technologies, Inc. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/api/usim_qsh_ext.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/29/19   kb      Add support to send response when session handler is done
08/04/16   bcho    Support added for QSH generic failure event
07/21/16   bcho    Initial version


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "comdef.h"


/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* ----------------------------------------------------------------------------

   ENUM:      USIM_QSH_EVENT_E

   DESCRIPTION:
     This enum specifies the events on which QSH flushes the F3 log buffer
-------------------------------------------------------------------------------*/
typedef enum
{
  USIM_QSH_EVENT_CARD_ERROR              = 0x00, /* (RARE) Failure event: when card error event is sent to MMGSDI's clients */
  USIM_QSH_EVENT_CARD_INSERTED           = 0x01, /* (RARE) when card inserted event is sent to MMGSDI's clients */
  USIM_QSH_EVENT_RECOVERY_TRIGGERED      = 0x02, /* (RARE) Failure event: when recovery is triggered and notified to MMGSDI */
  USIM_QSH_EVENT_SESSION_ACTION_DONE     = 0x03, /* (RARE) Send when requested session management request is completed */
  USIM_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE = 0xFE, /* (RARE) Common event for logging of any failure event*/
  USIM_QSH_EVENT_MAX                             /* (NA)   For Internal bound checking only */
}usim_qsh_event_e;

#define USIM_QSH_MAJOR_VER        1
#define USIM_QSH_MINOR_VER        1

#endif /*USIM_QSH_EXT_H*/
