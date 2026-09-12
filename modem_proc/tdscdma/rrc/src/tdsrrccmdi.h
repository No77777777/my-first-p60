#ifndef TDSRRCCMDI_H
#define TDSRRCCMDI_H
/*===========================================================================
             R R C   C O M M A N D S   I N T E R N A L   H E A D E R

DESCRIPTION

  This module contains the internal header information for the RRC
  Commands module. This file is internal to RRC and should not be included
  by any module outside RRC.


Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccmdi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/16/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   ttl    Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "queue.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSCMD_BASE_MASK 0xFF000000  /* Mask used to determine base numbers of
                                  RRC command ids */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/*      EXTERNALIZED RRC COMMAND QUEUES                   */
/*                                                        */
/*  All Command Queues for the RRC Task are defined here  */
/*--------------------------------------------------------*/

/* Queue for internal RRC commands within the RRC task    */
extern q_type tdsrrc_int_cmd_q;

/* Queue for RRC commands sent by L1 task                 */
extern q_type tdsrrc_l1_cmd_q;

/* Queue for RRC commands sent by MAC task                */
extern q_type tdsrrc_mac_cmd_q;

/* Queue for RRC commands sent by RLC task                */
extern q_type tdsrrc_rlc_cmd_q;

/* Queue for RRC commands sent by MM task                 */
extern q_type tdsrrc_mm_cmd_q;

/* Queue for RRC commands sent by TC task                 */
extern q_type tdsrrc_tc_cmd_q;

/* Queue for RRC commands sent by GSM RR task             */
extern q_type tdsrrc_rr_cmd_q;

/* Queue for RRC commands sent by WCDMA RRC task          */
extern q_type tdsrrc_rrc_cmd_q;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*Queue type for WL1 sendign free flaoting cmds to RRC*/
extern q_type tdsrrc_l1_ff_cmd_q;
#endif


/*Queue type for TDSCDMA RRC cmds from message router*/
extern q_type tdsrrc_msgr_cmd_q;
extern q_type tdsrrc_msgr_free_cmd_q;

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */




/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION RRC_INIT_COMMAND_QUEUES

DESCRIPTION

  This function initializes the RRC command queues. This function needs
  to be called once at the time of the RRC task start-up.
 
DEPENDENCIES

  This function assumes that all REX signals for RRC task have been cleared 
  before this function is called.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.
  
===========================================================================*/
void tdsrrc_init_command_queues( void );


#endif /* TDSRRCCMDI_H */

