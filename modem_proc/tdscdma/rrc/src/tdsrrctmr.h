#ifndef TDSRRCTMR_H
#define TDSRRCTMR_H
/*===========================================================================
              R R C T M R   H E A D E R  F I L E

DESCRIPTION
 The RRCTMR module consists of RRC timers implementation.
  
Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctmr.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/10/11   sks     Made changes to implement 5s guard timer for LTOW redirection.
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
09/22/10   kp      Added code for Qchat timer that delay HSPA status indication
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
09/13/10   as      Added code to support FEATURE_DUAL_SIM_WCDMA_TUNEAWAY

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "timer.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

#define TDSRRCTMR_RSSI_BER_REPORT_TIME 3000 /* Periodical RSSI&BER reporting time */
/* At present setting the value of infinity as 24 hrs */
#define TDSRRCTMR_INFINITY_VAL         86400000 
#define TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER_VAL 5000

/* The maximum length of timer name in char to print out */
#define TDSRRCTMR_TIMER_NAME_LEN_MAX  40

typedef void (*tdsrrc_non_def_timer_cb_ptr)( unsigned long param );

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


typedef enum
{
  /* 0 */
  TDSRRCTMR_T_300_TIMER,           /* T300 Timer from SIB 1 */
  /* 1 */
  TDSRRCTMR_T_308_TIMER,           /* T308 Timer from SIB 1 */
  /* 2 */
  TDSRRCTMR_WAIT_TIME_TIMER,       /* Wait Time Timer from RRC Connection Reject
                                   Message */
  /* 3 */
  TDSRRCTMR_UE_TO_CAMP_ON_TIMER,   /* Timer for UE to camp on, if not already */
  /* 4 */
  TDSRRCTMR_RRC_CON_DELAY_TIMER,   /* Timer for UE to delay RRC Conn Release */
  /* 5 */
  TDSRRCTMR_RCE_L2ACK_TIMER,       /* Timer to receive L2 ACK in acknowledged
                                   mode transmission for RCE procedure */
  /* 6 */
  TDSRRCTMR_RCR_L2ACK_TIMER,       /* Timer to receive L2 ACK in acknowledged
                                   mode transmission for RCR procedure */
  /* 7 */
  TDSRRCTMR_T_304_TIMER,           /* T304 Timer from SIB 1 */
  /* 8 */
  TDSRRCTMR_RSSI_BER_TIMER,        /* Timer to periodically report RSSI & BER 
                                   to CM */
  /* 9 */
  TDSRRCTMR_SIB_WAIT_TIMER,        /* Timer for maximum time to wait for SIBs 
                                   in a cell */
  /* 10 */
  TDSRRCTMR_T_302_TIMER,           /* T302 timer for CELL UPDATE retransmission */
  /* 11 */
  TDSRRCTMR_T_305_TIMER,           /* T305 timer for periodic CELL UPDATE */
  /* 12 */
  TDSRRCTMR_T_316_TIMER,           /* T316 Timer for Out of Service Area in
                                   CELL_PCH and URA_PCH state */
  /* 13 */
  TDSRRCTMR_T_317_TIMER,           /* T317 Timer for Out of service Area in
                                   CELL_FACH state */
  /* 14 */
  TDSRRCTMR_T_307_TIMER,           /* T307 Timer is max time for UE to get into
                                   In Service after T305 is expired */
  /* 15 */
  TDSRRCTMR_T_314_TIMER,           /* T314 Re-establish timer */
  /* 16 */
  TDSRRCTMR_T_315_TIMER,           /* T315 Re-establish timer */
  /* 17 */
  TDSRRCTMR_SIB7_EXP_TIMER,        /* Timer for SIB7 Expiration */
  /* 18 */
  TDSRRCTMR_IGNORE_BCCH_MOD_TIMER,  /* Timer to Ignore BCCH Modifcation Info IE
                                   if RRC is already processing one */
  /* 19 */
  TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER, /* Timer started during cell selection
                                           going DCH->FACH
                                         */ 
  /* 20 */
  TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER, /* Timer started during cell selection
                                           going FACH->FACH/PCH
                                         */ 
  /* 21 */
  TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER, /* Timer to detect L1 deadlocks */
  /* 22 */
  TDSRRCTMR_DELAY_ERR_FATAL_TIMER, /* Timer to delay ERR_FATAL */
  /* 23 */
  TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER,  /* Timer started during cell selection
                                          going DCH->CELL_PCH or DCH->URA_PCH*/
  /* 24 */
  TDSRRCTMR_T_3174_TIMER,               /* T3174 GTOTDS PCCO timer */
  /* 25 */
  TDSRRCTMR_FREQ_SCAN_TIMER,             /* Timer indicating if FREQ_SCAN should be
                                        initiated or not */
  /* 26 */
  TDSRRCTMR_T_OOS_TIMER,             /* Timer indicating if UE can not select a cell
                                        after OOS detection */
  /* 27 */
  TDSRRCTMR_PHY_CHAN_FAILURE_TIMER,        /* Phy Chan Failure timer used when initiating Cell Selec when  
                                       we get a phy_chan_failure after a rrc conn setup */
  /* 28 */
  TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER, /* Time duration for which no service should be 
                                        returned to NAS */
  /* 29 */
  TDSRRCTMR_BPLMN_SRCH_TIMER,         /* Timer for background PLMN search from 2G->3G */
  /* 30 */
  TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER,     /* Timer indicating if FULL FREQ SCAN should be
                                       initiated or not for a 2G->3G BPLMN srch request */
  /* 31 */
  TDSRRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER,     /* Timer indicating if FULL FREQ SCAN 
                                       should be done or not for a Manual PLMN search */
  /* 32 */
  TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER,   /* Timer for inter frequency redirection */
  /* 33 */
  TDSRRCTMR_CONN_MODE_OOS_TIMER,        /* Timer for OOS in Connected Mode */
  /* 34 */
  RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER,  /* Timer for 3G->3G BPLMN DRX tmr expiry*/
  /* 35 */
  TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER,   /* Max Timer to complete 3G->3G, 2G BPLMN search */


  /* 36 */
  TDSRRCTMR_INACTIVITY_TIMER,       /* Timer for inactivity handling */
  /* 37 */
  TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER,  /* Timer for six hr sib clean up*/


  /* 38 */
  TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER,
  /* 39 */
  TDSRRCTMR_CONN_MODE_OOS_DS_TIMER,

  /* 40 */
  TDSRRC_SIB_CHANGE_GUARD_TIMER,     /* Timer for sib change guard handling */
  
  /* 41 */
  TDSRRC_BCCH_MODIFICATION_TIMER,     /* Timer for BCCH Modification timer handling */


  /* 42 */
  RRCTMR_G2TD_REDIRECTION_WAIT_TIMER,
  /* 43 */
  RRCTMR_G2TD_REDIRECTION_LFS_TIMER,

  /* 44 */
  RRCTMR_TD2G_ABORT_GUARD_TIMER,

  /* 45 */
  TDSRRCTMR_T_319_TIMER,            /*T319 timer for DRX coefficient*/

  /* 46 */
  TDSRRCTMR_T_320_TIMER,

  /* 47 */
  TDSRRCTMR_COUEC_TIMER,

  /* #ifdef FEATURE_TDSCDMA_DSDS */
  /* 48 */
  TDSRRCTMR_WRM_LOCK_TIMER,
  /* 49 */
  TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER,
  /* 50 */
  TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER,   //When cannot get lock for T2T BPLMN before TL1 go to sleep, 
                                             //start this timer, after timeout, send RESUME_IND to CSP to start BPLMN
  /* 51 */
  TDSRRCTMR_WMR_UNLCOK_BY_TIMER,
  /* 52 */
  TDSRRCTMR_GET_TA_DURATION_TIMER,
  /* #endif // FEATURE_TDSCDMA_DSDS */

  /* 53 */
  TDSRRCTMR_T_323_TIMER,

  /* 54 */
  TDSRRCTMR_DED_PRI_VALIDITY_TIMER,

  /* 55 */
  TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER,

  /* 56 */
  TDSRRCTMR_RCE_WAIT_RESELCT_TIMER,

  /* 57 */
  // CR430034 RAU Reject workaround 
  TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER,

  /* 58 */
  TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER,
  
  /* 59 */
  TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER,
  
  /* 60 */
  TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER,

  /* 61 */
  TDSRRCTMR_SOFT_ABORT_GUARD_TIMER,

  /* 62 */
  TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER, /*M-PLMN Partial Report timer*/
  /* 63 */
  TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER,
  /* 64 */
  TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER,

  /* FR19720 */
  TDSRRCTMR_DA_BACKOFF_TIMER,

  /* 66 */
  TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER,
  

  /*67*/
  TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER,

  /* 68 */
  /* 9 sec guard timer for td2td manual bplmn search */
  TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER,
   /* 69 */
  TDSRRCTMR_SIB_STOP_PARTITION_TIMER,
  /* 70 */
  TDSRRCTMR_SIB_MASK_INVERT_TIMER,
  /* 71 */
  TDSRRCTMR_SIB_EMPTY_MASK_TIMER,
  /* 72*/
  TDSRRCTMR_RCE_ALLOW_TA_TIMER,
  /* 73*/
  TDSRRCTMR_QTA_GUARD_TIMER,
  /* 74 */
  TDSRRCTMR_HOLD_PS_TIMER,
  /* 75 */
  TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER,
   /* Keep as the last entry */
   TDSRRCTMR_MAX
}tdsrrctmr_timer_e_type;

/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION tdsrrctmr_timers_callback

DESCRIPTION
  This function will be called when ever RRC timers expire.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrctmr_timers_callback
(
   unsigned long param                   /* Parameter to Identify timer */                               
)
;

/*===========================================================================

FUNCTION tdsrrctmr_create_timers

DESCRIPTION
  This function will create all RRC related timers. This function will be 
  called at RRC Task startup.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrctmr_create_timers
(
  void
);

/*===========================================================================

FUNCTION tdsrrctmr_start_timer

DESCRIPTION
  This function will set the timer value and starts the timer.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrctmr_start_timer
(
  tdsrrctmr_timer_e_type timer,                /* Timer to start */
  unsigned long timer_value                 /* Value for timer */                      
);

/*===========================================================================

FUNCTION tdsrrctmr_stop_timer

DESCRIPTION
  This function will clear the timer value and stops the timer.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrctmr_stop_timer
(
  tdsrrctmr_timer_e_type timer                 /* Timer to stop */
);

/*===========================================================================

FUNCTION tdsrrctmr_get_remaining_time

DESCRIPTION
  This function will return the remaining tics for the Timer passed in.
  
DEPENDENCIES
  None

RETURN VALUE
  Unsigned Long.  Returns 0 if times is not recognized.
  
SIDE EFFECTS
  None
===========================================================================*/
uint64 tdsrrctmr_get_remaining_time
(
  tdsrrctmr_timer_e_type timer                 /* Timer to stop */
);

/*===========================================================================

FUNCTION tdsrrc_def_non_defer_timer

DESCRIPTION
  Define non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_def_non_defer_timer(timer_type *p_timer, tdsrrc_non_def_timer_cb_ptr timer_cb_ptr, unsigned long param);
/*===========================================================================

FUNCTION tdsrrc_set_non_defer_timer

DESCRIPTION
  Set non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/

void tdsrrc_set_non_defer_timer
(
   timer_type     *p_timer,  /* pointer to timer to set  */
   timetick_type  cnt       /* value to set the timer   */
);
/*===========================================================================

FUNCTION tdsrrc_clr_non_defer_timer

DESCRIPTION
  Clear non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_clr_non_defer_timer(timer_type *p_timer);
/*===========================================================================

FUNCTION tdsrrc_clr_non_defer_timer

DESCRIPTION
  Get remaining time of non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
timetick_type tdsrrc_get_non_defer_timer
(
  timer_type *p_timer       /* pointer to the timer to get */
);
#endif /* TDSRRCTMR_H */

