#ifndef TDSRRCSIGS_H
#define TDSRRCSIGS_H
/*===========================================================================
                      R R C    S I G N A L S 

DESCRIPTION

  This module defines Rex Signals used by RRC task. This module
	is internal to the RRC task and is not visible to modules
	outside the RRC task.


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsigs.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */



/*--------------------------------------------------------*/
/*                                                        */
/*                 RRC  SIGNALS                           */
/*                                                        */
/*  All REX SIGNALS for the RRC Task are defined here     */
/*--------------------------------------------------------*/

/* Note that 0x8000 and 0x0001 are reserved for task start*/
/* and task start acknowledgement, respectively           */

/*---------------------------------*/
/*                                 */
/*    Command Queue Signals        */
/*---------------------------------*/

/* Signal for internal RRC commands within the RRC task		*/
#define TDSRRC_INT_CMD_Q_SIG    0x0002

/* Signal for RRC commands sent by L1 task                */
#define TDSRRC_L1_CMD_Q_SIG     0x0004

/* Signal for RRC commands sent by MAC task           		*/
#define TDSRRC_MAC_CMD_Q_SIG    0x0008

/* Signal for RRC commands sent by RLC task               */
#define TDSRRC_RLC_CMD_Q_SIG    0x0010

/* Signal for RRC commands sent by MM task            		*/
#define TDSRRC_MM_CMD_Q_SIG     0x0020

/* Signal to send the watchdog report                           */
#define TDSRRC_RPT_TIMER_SIG    0x0040

/* Signal for RRC commands sent by TC task                      */
#define TDSRRC_TC_CMD_Q_SIG     0x0080

/* Signal for NV read/write completion                          */
#define TDSRRC_NV_SIG           0x0100

/* SIgnal for RRC commands sent by RR task                      */
#define TDSRRC_RR_CMD_Q_SIG     0x0200

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/* Signal for RRC commands sent by LCS Task */
#define TDSRRC_LSM_CMD_Q_SIG     0x0400
#endif


/* Signal for RRC commands sent by WCDMA RRC Task */
#define TDSRRC_RRC_CMD_Q_SIG   0x1000

#define TDSRRC_WRM_SIG   0x2000

/*Singal to indicate that Preemption is due*/
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#else
#define TDSRRC_WRM_PREEPTION_SIG 0x4000
#endif

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
#define TDSRRC_LTE_RRC_CMD_Q_SIG 0x8000
#endif
#define TDSRRC_MSGR_SIG 0x8000

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#define TDSRRC_L1_FF_CMD_Q_SIG   0x10000
#endif

//tag: FEATURE_TDSCDMA_DR_DSDS
#define TDSRRC_WRM_INFO_SIG   0x40000

/*---------------------------------*/
/*                                 */
/*    Other Signals                */
/*---------------------------------*/

/* Signal for RRC test tdsrrc_wait_ms() */
#define TDSRRC_TEST_WAIT_MS_SIG  0x04000000

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */



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

#endif /* TDSRRCSIGS_H */

