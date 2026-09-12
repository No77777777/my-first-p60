#ifndef RXCIDLE_H
#define RXCIDLE_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              CDMA2000 Receive Task Idle State Processing.

GENERAL DESCRIPTION
  Contains implementation of idle state operations that are not specifically
  tied to a physical layer channel.  Paging Channel, BCCH, and FCCCH all use
  this common functionality.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2002 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                              Edit History

$PVCSPath: O:/src/asw/COMMON/vcs/rxcidle.h_v   1.9   14 Aug 2002 13:22:24   sanjayr  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcidle.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
06/15/12   srk     Removed FEATURE_IS2000_REL_A_CC_SMMC
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
10/30/10   jtm     Modem 1x SU API changes.
03/26/09   jtm     Eliminated implicit includes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
03/05/03   fc      Consolidated f-csh fade timer implementation.
10/21/02   bn      Added support for NCC statistic counters
08/14/02   sr      Added #define MAX_SKIPPED_FRAMES
08/13/02   sr      Added Fast RAHO support
06/03/02   sr      Resolved compile errors for non-REL_A builds
05/31/02   sr      Added rxcidle_first_frame() to check if we are in the first
                   frame after wakeup
02/15/02   lad     Added rxcidle_chan_config().  Added rxcidle_pending_state()
                   for state transitions triggered in external context.
01/08/01   lad     Created file.

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"

//mux
#include "rxcmc_i.h"

//other
#include "rex.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Set the number of frames that will be skipped while waiting for the
   first frame after the slot boundary, in the case that the first frame
   is not being recognized. */
#define MAX_SKIPPED_FRAMES 6



/* -----------------------------------------------------
 * This represents the state of the mobile while it
 * it is monitoring the Paging channel for Fade reports.
 * ----------------------------------------------------- */
typedef enum {
  RXCIDLE_IDLE_FADE,
  RXCIDLE_ACCESS_FADE,
  RXCIDLE_CURR_FADE /* Used to specify the use of existing fade type. */
} rxcidle_fade_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXCIDLE_DEF_TIMER

DESCRIPTION
  This function defines f-csh fade timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void
rxcidle_def_timer (void);

/*===========================================================================

FUNCTION RXCIDLE_INIT

DESCRIPTION
  This function initializes the module.

DEPENDENCIES
  This must be called prior to using this module for the first time.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void
rxcidle_init (void);

/*===========================================================================

FUNCTION RXCIDLE_SET_FADE_TIMER

DESCRIPTION
  This function sets the appropriate fade timer for idle mode operation.

DEPENDENCIES
  rxcidle_init() must have been called.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void
rxcidle_set_fade_timer (rxcidle_fade_type fade_type);


/*===========================================================================

FUNCTION RXCIDLE_CLR_FADE_TIMER

DESCRIPTION
  This function clears the appropriate fade timer for idle mode operation.

DEPENDENCIES
  rxcidle_init() must have been called.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void
rxcidle_clr_fade_timer (void);

/*===========================================================================

FUNCTION RXCIDLE_WAIT_MASK

DESCRIPTION
  This function returns the REX signals that need to be monitored when
  rex_wait() is called during idle state operation.

DEPENDENCIES
  rxcidle_init() must have been called.

RETURN VALUE
  REX wait mask.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type
rxcidle_wait_mask (void);


/*===========================================================================

FUNCTION RXCIDLE_PROCESS_SIGS

DESCRIPTION
  This function checks signals for timer expiration and handles them.

DEPENDENCIES
  rxcidle_init() must have been called.

RETURN VALUE
  Boolean indicating if it is OK to continue normal operation.

SIDE EFFECTS
  None.

===========================================================================*/
boolean
rxcidle_process_sigs (rex_sigs_type sigs);


/*===========================================================================

FUNCTION RXCIDLE_FIRST_FRAME

DESCRIPTION
  This function determines whether it is the first frame after wake
  up

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is the first frame; FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxcidle_first_frame( void );
#endif /* RXCIDLE_H */
