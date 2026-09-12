#ifndef RRCCMDI_H
#define RRCCMDI_H
/*===========================================================================
             R R C   C O M M A N D S   I N T E R N A L   H E A D E R

DESCRIPTION

  This module contains the internal header information for the RRC
  Commands module. This file is internal to RRC and should not be included
  by any module outside RRC.


Copyright (c) 2000, 2002-2004, 2007, 2008 Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrccmdi.h#2 $   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/26/15   gv      Made changes to process the alternate/free floating sub command queue till 
                   it reaches NULL before exiting the loop
07/15/15   sn      Changes for FR 28274:NAS_GET_NEGOTIATED_DRX_REQ_MSG
05/28/15   bc      Changes to share SIBDB and ACQDB among two SUBs in a W+W solution
05/07/14   vi      Made changes to put the rrc_l1_ff_cmd_q usage under critical section to avoid race condition in FF cmds. 
09/10/12   gv      Added support for new interface between WRRC and TDS RRC
12/12/11   gv      Added code to support free floating of tasks
12/05/11   pm      Added code to support cell id request by CM in DCH state"
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
06/29/10   rm      Added code to support LTE<->WCDMA Inter RAT
05/08/09   ss      Updated Copyright Information
03/06/08   da      Changed FEATURE_MBMS to FEATURE_MODEM_MBMS
12/21/07   vr      Added MBMS Service Manager(MSM) command queue
12/17/04   vr      Added CM queue for WCDMA -> 1x handover
07/30/02   xfg     Added extern rrc_rr_cmd_q for supporting inter-rat handover.
02/04/02   xfg     Added extern rrc_tc_cmd_q for loopback test
12/08/00   ram     Moved CMD_BASE_MASK definiton from rrccmd.c
12/06/00   ram     Removed rrc_cmd_e_type definition since it's no longer
                   used. Moved rrc_free_cmd_buf back to rrccmd.h since the
                   function now uses rrc_cmd_type as a parameter.
11/30/00   rj      removed rrccmd.h from include files
11/29/00   ram     Added rrc_cmd_e_type to be used within RRC. Added externalized
                   function rrc_free_cmd_buf that is used by other RRC modules
                   to free commands.
11/17/00   ram     Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "comdef.h"
#include "queue.h"
#include "rrcdata_v.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define CMD_BASE_MASK 0xFF000000  /* Mask used to determine base numbers of
                                  RRC command ids */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
typedef struct
{
  q_type rrc_int_cmd_q;

/* Queue for RRC commands sent by L1 task                 */
  q_type rrc_l1_cmd_q;

/* Queue for RRC commands sent by MAC task                */
  q_type rrc_mac_cmd_q;

/* Queue for RRC commands sent by RLC task                */
  q_type rrc_rlc_cmd_q;

/* Queue for RRC commands sent by MM task                 */
  q_type rrc_mm_cmd_q;

/* Queue for RRC commands sent by TC task                 */
  q_type rrc_tc_cmd_q;

  /* Queue for RRC commands sent by RR task  */
  q_type rrc_rr_cmd_q;
  
#if defined (FEATURE_CGPS_UMTS_CP_WCDMA)
  /* Queue for RRC commands sent by LSM task   */
  q_type rrc_lsm_cmd_q;
#endif

/* Queue for RRC commands sent by CM task             */
  q_type rrc_cm_cmd_q;

#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
/*Queue type for WCDMA RRC cmds from LTE RRC*/
  q_type rrc_lte_cmd_q;
  q_type rrc_lte_free_cmd_q;
#endif

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
/*Queue type for WL1 sending free floating cmds to RRC*/
  q_type rrc_l1_ff_cmd_q;
#ifdef FEATURE_DUAL_WCDMA 
  q_type rrc_alt_sub_ff_cmd_q;
#endif
#endif

#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
  q_type rrc_tdsrrc_cmd_q;
#endif

#if defined(FEATURE_THOR_MODEM) || defined(FEATURE_WCDMA_ATLAS_MODEM) || defined(FEATURE_TABASCO_MODEM)
/*Queue type for WCDMA RRC cmds from QMI*/
  q_type rrc_qmi_cmd_q;
  q_type rrc_qmi_free_cmd_q;
#endif

}rrc_cmd_q_type;

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
/*  All Command Queues for the RRC Task are externed here  */
/*--------------------------------------------------------*/
#ifdef FEATURE_DUAL_WCDMA 
extern rrc_cmd_q_type rrc_cmd_q_sim[MAX_RRC_STACKS];
#define rrc_cmd_q rrc_cmd_q_sim[WAS_ID_M]
#else
extern rrc_cmd_q_type rrc_cmd_q;
#endif

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
void rrc_init_command_queues(sys_modem_as_id_e_type wrrc_as_id);


#endif /* RRCCMDI_H */

