#ifndef SEQSTATES_H
#define SEQSTATES_H
/*===========================================================================

       L 1    S E Q U E N C E R    S T A T E    R O U T I N E
                   D E C L A R A T I O N S

DESCRIPTION
  This file contains the declarations for the state initialization
  and cleanup routines for the Layer 1 Sequencer Module.

EXTERNALIZED FUNCTIONS

  Sequencer Idle Initialiazation and Cleanup Routines.
  seq_idle_init
  seq_idle_cleanup

  Sequencer ACQ state Initialization and Cleanup Routines.
  seq_acq_init
  seq_acq_cleanup

  Sequencer BCH state Initialization and Cleanup Routines.
  seq_bch_init
  seq_bch_cleanup

  Sequencer FACH state Initialization and Cleanup Routines.
  seq_fach_init
  seq_fach_cleanup

  Sequencer PCH state Initialization and Cleanup Routines.
  seq_pch_init
  seq_pch_cleanup

  Sequencer DCH state Initialization and Cleanup Routines.
  seq_dch_init
  seq_dch_cleanup

EXTERNALIZED MACROS
  None

INTERNALIZED FUNCTIONS
  None

INTERNALIZED MACROS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  These functions are called during state transitions.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/msm5200/l1/vcs/tdsseqstates.h_v   1.0   15 Jun 2001 12:21:40   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsseqstates.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/08/09  sup     Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to tdsdec.h, tdsenci.h, tdsl1m.h, tdssrch.h, tdssrchi.h, tdssrchlog.h
09/26/07   nd      Added extern for the seq init and cleanup for FS state 
07/15/01   sh      Created File.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1m.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*===========================================================================
         Sequencer Idle State Init and Cleanup Procedures
===========================================================================*/
/*===========================================================================

FUNCTION TDSSEQ_IDLE_INIT

DESCRIPTION
  This function is called when L1 enters the TDSL1M_IDLE state. This will
  install the General Timeline, enable the SFN/CFN Event on the 
  Timeline, and initialize the SFN and CFN counts to zero. The Timeline
  is then slaved to the Frame Reference Counter.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  A previously installed General Timeline is destroyed.

===========================================================================*/
extern void tdsseq_idle_init( void );

extern boolean tdsseq_idle_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer Acq State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_acq_init( void );

extern boolean tdsseq_acq_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer SYNC State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_sync_init( void );

extern boolean tdsseq_sync_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer BCH State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_bch_init( void );

extern boolean tdsseq_bch_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer PCH State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_pch_init( void );

extern boolean tdsseq_pch_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer FACH State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_fach_init( void );

extern boolean tdsseq_fach_cleanup( tdsl1m_state_enum_type next_state );

/*===========================================================================
         Sequencer DCH State Init and Cleanup Procedures
===========================================================================*/
extern void tdsseq_dch_init( void );

extern boolean tdsseq_dch_cleanup( tdsl1m_state_enum_type next_state );
#endif /* SEQSTATES_H */

