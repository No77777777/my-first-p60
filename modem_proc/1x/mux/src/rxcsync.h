#ifndef RXCSYNC_H
#define RXCSYNC_H
/*===========================================================================

                 C D M A    S Y N C    M O D U L E

DESCRIPTION
  This header file contains definitions for the sync module that
  are used by the rxc unit.  This header file is internal to the rxc unit.

  Copyright (c) 1997 - 2015 Qualcomm Technologies, Inc. 
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

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$PVCSPath: O:/src/asw/COMMON/vcs/rxcsync.h_v   1.1   20 Apr 2001 11:47:38   sanjayr  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcsync.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/12/15   jh      Changes for SW Demback
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
10/30/10   jtm     Modem 1x SU API changes.
08/05/09   jtm     Lint fixes.
03/26/09   jtm     Eliminated implicit includes.
04/20/04   jrp     Fixed lint warnings.
04/20/01   sr      Merged in from MSM5100 archive
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "comdef.h"
#include "customer.h"

//mux
#include "rxc_i.h"

//drivers
#include "dec5000_v.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* DEBUG CRC statistics */
extern dword rxc_sc_crc_good;
  /* number of Sync Channel messages with good CRCs */
extern dword rxc_sc_crc_bad;
  /* number of Sync Channel messages with bad CRCs */

extern dec_sc_status_type rxc_sc_status; /* Status(CRC + QUALITY), ser, energy */
extern rxc_sc_ser_type rxc_sc_ser_total;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION RXC_SC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Sync Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_sc_isr ( void );

/*===========================================================================

FUNCTION RXC_SC_INIT_ISR

DESCRIPTION
  This function is registers the interrupt handler.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_sc_init_isr( boolean use_sw_demback );

/*===========================================================================

FUNCTION RXC_SC_INIT

DESCRIPTION
  This funtion initializes the Sync channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_sc_init(rxc_state_type curr_state);

/*===========================================================================

FUNCTION RXC_SC

DESCRIPTION
  Sync channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern rxc_state_type rxc_sc ( void );

#endif /* RXCSYNC_H */
