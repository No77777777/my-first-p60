#ifndef CMREGPRXI_H
#define CMREGPRXI_H
/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
                            R E G P R O X Y

                 I N T E R N A L    H E A D E R   F I L E

GENERAL DESCRIPTION
  This header file contains all the internal types necessary for implementing
  the RegProxy.

Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*============================================================= ==============

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmregprxi.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/31/14   jvo     Refactored message macros
01/24/14   rm      Adding debug buffers
07/21/13   skk     FR2921: Network initiated Authentication Procedure Support
                   during emergency call When SIM is perso Locked
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB
01/25/13   gm      FR 3059 - ONLINE optimization - skip duplicate subscription
04/19/12   xs      Add support of sending FPLMN list
10/31/11   rk      Use CMSS info instead of SD info for Domain selection algorithm
07/24/11   aj      SC SVLTE 1.0 merge to mainline
12/23/10   sv      Integrate DSDS feature
12/16/10   am      Replacing TG == T_PC with CM_DEBUG
11/30/10   sv      Replace TG==T_PC with CM_DEBUG
11/15/10   rm      Memory optimization changes
11/02/10   am      Replacing FEATURE_MOB_MMODE_CUTE
11/05/10   aj      DO->LTE reselection code
06/21/10   rk      Redesign of WRLF feature
01/08/10   ay      LTE Connected mode support
11/13/09   fj      Add prototype for cmregprx_map_prot_oprt_mode_to_sys() and
                   cmregprx_map_deact_to_stop_reason().
12/22/08   fj      Change API from GW to GWL.
09/01/09   sv      Moving from clk API to time API
03/01/09   sv      Lint cleanup.
03/13/08   sk      Fix to allow CDMA scans during dormant GW PS data calls
                   without tearing down the PS data call.
02/05/08   rn      Added support for new command CM_PH_STATUS_CHGD_REQ
11/30/06   rp      Adding suport for background service search
04/10/06   pk      Lint fixes
01/06/06   ic      Lint cleanup
11/14/05   ic      Removed #undef CMREGPRX_DEBUG so CUTE can define it in
                   order to enable assertions in cmregprx.c
                   Lint80 clean up
10/21/04   sj      Changed throttling mechanism for CM_SERVICE_CNF.
                   Removed handling of unused sanity timer mechanism.
07/30/04   sj      Added support for camped_ind.
02/06/04   ws      Initial jaguar/mainline merge.
11/03/03   ws      Fix LTK compile errors.
08/20/03   SJ      Added srv cnf hold period, if srv cnf comes back within
                   1 second.
03/18/03   sj      Added flag to indicate protocol is in warmup state.
01/21/03   ws      Updated copyright information for 2003
04/22/02   SJ      Initial release.

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */
#include "comdef.h"   /* Definition for basic types and macros */
#include "msg.h"      /* Message output services */
#include "err.h"      /* Error and error fatal services */
#include "cmd.h"      /* Command header for inter-task communication */
#include "time_svc.h" /* Interface for clock services */

#include "sd.h"       /* Interface for System Determination services */
#include "sd_v.h"
#include "prot.h"     /* Common header for mmoc <-> protocol communications*/
#include "prot_v.h"
#include "sys.h"

#include "task.h"     /* Interface for global task resources */
#include "dog.h"      /* Interface to watchdog procedures */
#include "cmwll.h"
#include "cmi.h"
#include "lte_rrc_ext_msg.h"
#include "cmdbg_qsh.h"



/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to correctly align function definitions (cosmetic).
*/
#ifndef EXTERN
#define EXTERN
#endif


/*===========================================================================
=============================================================================
=============================================================================
==================== RegProxy DEBUG RELATED STUFF ===========================
=============================================================================
=============================================================================
===========================================================================*/

/*---------------------------------------------------------------------------
      Compile RegProxy with debug information, asserts and messages.
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                  RegProxy_DEBUG/T_PC independent macros.
---------------------------------------------------------------------------*/
/* General message macros, some are for testing stubs */
#ifndef MSG

#ifdef CM_DEBUG
#error code not present
#else
#define MSG(   SSID, level, str )                 MSG_BASE(level,str, 0, 0, 0)
#define MSG_1( SSID, level, str, a )              MSG_BASE(level,str,a,0,0)
#define MSG_2( SSID, level, str, a, b )           MSG_BASE(level,str,a,b,0)
#define MSG_3( SSID, level, str, a, b, c )        MSG_BASE(level,str,a,b,c)
#endif /* CM_DEBUG */

#define MMODE_MSG_LEGACY_FATAL         MSG_LVL_FATAL
#define MMODE_MSG_LEGACY_ERROR         MSG_LVL_ERROR
#define MMODE_MSG_LEGACY_HIGH          MSG_LVL_HIGH
#define MMODE_MSG_LEGACY_MED           MSG_LVL_MED
#define MMODE_MSG_LEGACY_LOW           MSG_LVL_LOW
#endif /* ! MSG */

#ifndef MMODE_MSG_LEGACY_FATAL

#define MMODE_MSG_LEGACY_FATAL         MSG_LEGACY_FATAL
#define MMODE_MSG_LEGACY_ERROR         MSG_LEGACY_ERROR
#define MMODE_MSG_LEGACY_HIGH          MSG_LEGACY_HIGH
#define MMODE_MSG_LEGACY_MED           MSG_LEGACY_MED
#define MMODE_MSG_LEGACY_LOW           MSG_LEGACY_LOW

#endif /* MMODE_MSG_LEGACY_FATAL */

/*-------------------------------------------------------------------------*/

/* MAIN MESSAGE MACROS */
/* Messages have numbers to designate the number of parameters, not counting
** the string parameter.  For example, if the format string is "x:%d, y:%y",
** use CMREGPRX_MSG_HIGH_2("x:%d, y:%y", x, y) because there are two parameters in
** the format string.
**
** CMREGPRX_MSG_LOW_x, CMREGPRX_MSG_MED_x, and CMREGPRX_MSG_HIGH_x simply print to the message log
**   with their designated message level.
**
** CMREGPRX_ERR_x quits during testing or uses MSG_MED on target
**
** CMREGPRX_ERR_FATAL_x prints to the message log and exits the software.
*/

#define CMREGPRX_ERR_FATAL_0(str)            ERR_FATAL( "=CMREGPRX= " str, 0, 0, 0 )
#define CMREGPRX_ERR_FATAL_1(str, a)         ERR_FATAL( "=CMREGPRX= " str, (a), 0, 0 )
#define CMREGPRX_ERR_FATAL_2(str, a, b)      ERR_FATAL( "=CMREGPRX= " str, (a), (b), 0 )
#define CMREGPRX_ERR_FATAL_3(str, a, b, c)   ERR_FATAL( "=CMREGPRX= " str, (a), (b), (c) )

#if defined(CM_DEBUG) || defined(CMREGPRX_DEBUG) || \
    defined(MMOC_DEBUG) || defined(SD_DEBUG)
#error code not present
#else
#define CMREGPRX_ERR_0(str)                MSG( MSG_SSID_CM,               \
                                            MMODE_MSG_LEGACY_MED,            \
                                            "=CMREGPRX= " str )
#define CMREGPRX_ERR_1(str, a)             MSG_1( MSG_SSID_CM,             \
                                            MMODE_MSG_LEGACY_MED,            \
                                            "=CMREGPRX= " str, (a) )
#define CMREGPRX_ERR_2(str, a, b)          MSG_2( MSG_SSID_CM,             \
                                            MMODE_MSG_LEGACY_MED,            \
                                            "=CMREGPRX= " str, (a), (b) )
#define CMREGPRX_ERR_3(str, a, b, c)       MSG_3( MSG_SSID_CM,             \
                                            MMODE_MSG_LEGACY_MED,            \
                                            "=CMREGPRX= " str, (a), (b), (c) )
#endif /* CM_DEBUG || CMREGPRX_DEBUG || MMOC_DEBUG || SD_DEBUG */

#define CMREGPRX_MSG_HIGH_0(str)             MSG( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str )
#define CMREGPRX_MSG_MED_0(str)              MSG( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str )
#define CMREGPRX_MSG_LOW_0(str)              MSG( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str )

/*-------------------------------------------------------------------------*/

#define CMREGPRX_MSG_HIGH_1(str, a)        MSG_1( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a) )
#define CMREGPRX_MSG_MED_1(str, a)         MSG_1( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a) )
#define CMREGPRX_MSG_LOW_1(str, a)         MSG_1( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str, (a) )
/*-------------------------------------------------------------------------*/

#define CMREGPRX_MSG_HIGH_2(str, a, b)     MSG_2( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b) )
#define CMREGPRX_MSG_MED_2(str, a, b)      MSG_2( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b) )
#define CMREGPRX_MSG_LOW_2(str, a, b)      MSG_2( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str, (a), (b) )

/*-------------------------------------------------------------------------*/

#define CMREGPRX_MSG_HIGH_3(str, a, b, c)  MSG_3( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c) )
#define CMREGPRX_MSG_MED_3(str, a, b, c)   MSG_3( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b), (c) )
#define CMREGPRX_MSG_LOW_3(str, a, b, c)   MSG_3( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str, (a), (b), (c) )
/*-------------------------------------------------------------------------*/

#define CMREGPRX_MSG_HIGH_4(str, a, b, c, d)  MSG_4( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d) )
#define CMREGPRX_MSG_MED_4(str, a, b, c, d)   MSG_4( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d) )
#define CMREGPRX_MSG_LOW_4(str, a, b, c, d)   MSG_4( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d) )

#define CMREGPRX_MSG_HIGH_5(str, a, b, c, d, e)  MSG_5( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e) )
#define CMREGPRX_MSG_MED_5(str, a, b, c, d, e)   MSG_5( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e) )
#define CMREGPRX_MSG_LOW_5(str, a, b, c, d, e)   MSG_5( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e) )

#define CMREGPRX_MSG_HIGH_6(str, a, b, c, d, e, f)  MSG_6( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )
#define CMREGPRX_MSG_MED_6(str, a, b, c, d, e, f)   MSG_6( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )
#define CMREGPRX_MSG_LOW_6(str, a, b, c, d, e, f)   MSG_6( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_LOW,             \
                                            "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )

#ifdef FEATURE_DISABLED_HABANERO

#define CMREGPRX_MSG_MED_6_EXT(str, a, b, c, d, e, f, sub_id)   MSG_6_EXT_V1( MSG_SSID_CM,               \
                                                                MMODE_MSG_LEGACY_MED, sub_id,            \
                                                                "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )

#define CMREGPRX_MSG_HIGH_0_EXT(str, sub_id)             MSG_EXT_V1( MSG_SSID_CM,               \
                                                         MMODE_MSG_LEGACY_HIGH, sub_id,           \
                                                         "=CMREGPRX= " str )
#define CMREGPRX_MSG_HIGH_1_EXT(str, a, sub_id)        MSG_1_EXT_V1( MSG_SSID_CM,               \
                                                       MMODE_MSG_LEGACY_HIGH, sub_id,           \
                                                       "=CMREGPRX= " str, (a) )
#define CMREGPRX_MSG_HIGH_2_EXT(str, a, b, sub_id)     MSG_2_EXT_V1( MSG_SSID_CM,               \
                                                       MMODE_MSG_LEGACY_HIGH, sub_id,           \
                                                       "=CMREGPRX= " str, (a), (b) )
#define CMREGPRX_MSG_HIGH_3_EXT(str, a, b, c, sub_id)  MSG_3_EXT_V1( MSG_SSID_CM,               \
                                                       MMODE_MSG_LEGACY_HIGH, sub_id,     \
                                                       "=CMREGPRX= " str, (a), (b), (c) )
#define CMREGPRX_MSG_HIGH_4_EXT(str, a, b, c, d, sub_id)  MSG_4_EXT_V1( MSG_SSID_CM,               \
                                                          MMODE_MSG_LEGACY_HIGH, sub_id,           \
                                                          "=CMREGPRX= " str, (a), (b), (c), (d) )
#define CMREGPRX_MSG_HIGH_5_EXT(str, a, b, c, d, e, sub_id)  MSG_5_EXT_V1( MSG_SSID_CM,               \
                                                             MMODE_MSG_LEGACY_HIGH, sub_id,            \
                                                             "=CMREGPRX= " str, (a), (b), (c), (d), (e) )
#define CMREGPRX_MSG_HIGH_6_EXT(str, a, b, c, d, e, f, sub_id)  MSG_6_EXT_V1( MSG_SSID_CM,               \
                                                                MMODE_MSG_LEGACY_HIGH, sub_id,            \
                                                                "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )
#else

#define CMREGPRX_MSG_MED_6_EXT(str, a, b, c, d, e, f, sub_id)   MSG_6( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_MED,             \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )

#define CMREGPRX_MSG_HIGH_0_EXT(str, sub_id)             MSG( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str )
#define CMREGPRX_MSG_HIGH_1_EXT(str, a, sub_id)        MSG_1( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a) )
#define CMREGPRX_MSG_HIGH_2_EXT(str, a, b, sub_id)     MSG_2( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b) )
#define CMREGPRX_MSG_HIGH_3_EXT(str, a, b, c, sub_id)  MSG_3( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c) )
#define CMREGPRX_MSG_HIGH_4_EXT(str, a, b, c, d, sub_id)  MSG_4( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d) )
#define CMREGPRX_MSG_HIGH_5_EXT(str, a, b, c, d, e, sub_id)  MSG_5( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e) )
#define CMREGPRX_MSG_HIGH_6_EXT(str, a, b, c, d, e, f, sub_id)  MSG_6( MSG_SSID_CM,               \
                                           MMODE_MSG_LEGACY_HIGH,            \
                                           "=CMREGPRX= " str, (a), (b), (c), (d), (e), (f) )
#endif

/*---------------------------------------------------------------------------
                   CMREGPRX_DEBUG/T_PC dependent macros.
---------------------------------------------------------------------------*/

#define CMREGPRX_FUNC_NOT_IMP( func ) \
      CMREGPRX_MSG_HIGH_0( #func " NOT Implemented!" )

#define CMREGPRX_CASE_NOT_IMP( _case ) \
      CMREGPRX_ERR_0( #_case " case NOT Implemented" )

/*---------------------------------------------------------------------------
                  Asserts and verifications macros
---------------------------------------------------------------------------*/

#ifdef CMREGPRX_DEBUG
#error code not present
#else
#define CMREGPRX_ASSERT( exp )
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef CMREGPRX_DEBUG
#error code not present
#else
#define CMREGPRX_ASSERT_ENUM_IS_INRANGE( val, max )
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef CMREGPRX_DEBUG
#error code not present
#else
#define CMREGPRX_ASSERT_ENUM_IS_INRANGE_INC( val, max )
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef CMREGPRX_DEBUG
#error code not present
#else
#define CMREGPRX_ASSERT_ENUM_IS_INRANGE_OR( val, max, equal )
#endif



/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC COMMAND INTERFACE RELATED TYPES ===================
=============================================================================
=============================================================================
===========================================================================*/

typedef struct
{

  /* Phone status change type (NAM selection, Operating mode etc).
  */
  prot_ph_stat_chg_e_type              chg_type;

  /* Current GSM/WCDMA subscription availability status.
  */
  cm_subs_avail_e_type                 is_gwl_subs_avail;


  /* Current operating mode of the phone.
  */
  prot_oprt_mode_e_type                oprt_mode;

  /* Transaction Id that should be used for acknowledging this command.
  */
  prot_trans_type                      trans_id;

  /* Enumeration of protocols that have a change in
  ** subscription available status
  */
  prot_subsc_chg_e_type                prot_subsc_chg;

  mmgsdi_session_type_enum_type        session_type;

  sys_modem_as_id_e_type               as_id;

  sys_mode_pref_e_type                 mode_pref;

  uint16                               nv_context;

  sys_ue_mode_e_type  curr_ue_mode;

  boolean        is_ue_mode_substate_srlte;
} cmregprx_ph_stat_chgd_s_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{

  /* Sim state received from SIM_STATE_UPDATE_IND needs to be
  ** updated in CM and SD
  */
  boolean update_pending;

  /* Sim state received from SIM_STATE_UPDATE_IND */
  sys_sim_state_e_type sim_state;

} cmregprx_sim_state_update_s_type;

typedef struct
{

  /* Detach type
  */
  sys_ps_detach_e_type type;

  /* Transaction Id that should be used for acknowledging this command.
  */
  prot_trans_type                      trans_id;
} cmregprx_ps_detach_s_type;

typedef struct
{

  /* Transaction Id that should be used for acknowledging this command.
  */
  prot_trans_type                      trans_id;

  sys_ue_mode_e_type                   ue_mode;

  boolean    is_ue_mode_substate_srlte;

  boolean                              is_tau_required;

} cmregprx_ue_mode_switch_s_type;

typedef struct
{
  boolean                      is_stack_active;

  sys_sys_mode_e_type          active_rat;

} cmregprx_3gpp2_status_ind_s_type;


typedef struct
{

  boolean                         srv_req_proceed;

} cmregprx_srv_req_response_s_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Union of parameters that are associated with the different commands.
*/
typedef union cmregprx_cmd_msg_param_u
{

  /* Parameters for activate protocol command.
  */
  prot_act_s_type                       act_prot;

  /* Parameters for deactivate protocol command.
  */
  prot_deact_s_type                     deact_prot;

  /* Parameters for generic protocol command.
  */
  prot_gen_cmd_s_type                   gen_cmd;

  /* Parameters for phone status changed command.
  */
  cmregprx_ph_stat_chgd_s_type          gw_ph_stat_cmd;

  /* Parameters for Dual Standby pref chgd cmd
  */
  prot_ds_stat_chgd_s_type              ds_stat_chgd_cmd;

  cmregprx_ps_detach_s_type             ps_detach_cmd;

  cmregprx_ue_mode_switch_s_type        ue_mode_switch_cmd;

  cmregprx_srv_req_response_s_type      srv_req_proceed_cmd;

  prot_subs_cap_chgd_s_type            multimode_subs_chgd_cmd;

  cmregprx_3gpp2_status_ind_s_type     cm_3gpp2_status_chgd_cmd;

} cmregprx_cmd_msg_param_u_type;

/* A type for parameters that are associated with queuing a command to
** RegProxy module.
*/
typedef struct
{

  /* The header information that is associated with this command.
  */
  cmd_hdr_type                         hdr;

  /* Regproxy command name.
  */
  prot_cmd_e_type                      name;

  /* sub and stack id
  */
  mm_sub_stk_id_s_type                 mm_id;

  /* Union of parameters that are associated with the different commands.
  */
  cmregprx_cmd_msg_param_u_type       param;

} cmregprx_cmd_msg_s_type;



/*===========================================================================
=============================================================================
=============================================================================
==================== REGPROXY IMPLEMENTATION RELATED TYPES ==================
=============================================================================
=============================================================================
===========================================================================*/

#if defined (FEATURE_MMODE_TRIPLE_SIM) || defined (FEATURE_MMODE_SXLTE_G)
/* Number of command buffers for MMoC -> REG communication.
*/
#define  CMREGPRX_CMD_BUFS_NUM          (9)
#elif defined(FEATURE_MMODE_DUAL_SIM)
/* Number of command buffers for MMoC -> REG communication.
*/
#define  CMREGPRX_CMD_BUFS_NUM          (6)
#else
#define  CMREGPRX_CMD_BUFS_NUM          (3)
#endif

/* Max Timer in secs to proceed srv request if it was delayed.
** In srlte+x configuration to delay x sub srv request so that 
** dds lte acquisition can start first.
** This is safe guard to ensure non dds acquisition is not stuck in wait state
*/
#define CMREGPRX_MAX_TIMER_DELAY_SRV_REQ    (dword)(1)

#define CMREGPRX_UPTIME_NONE             (dword) (-1)

/* Reference period in secs for counting the number of CM_SERVICE_CNF.
*/
#define  CMREGPRX_SRV_REQ_CNT_PERIOD    (dword)(2)

/* Maximum count of CM_SERVICE_CNF during SRV_CNF_CNT_PERIOD before
** Regproxy will start throttling CM_SERVICE_CNF to SD.
*/
#define  CMREGPRX_MAX_SRV_CNF_REQ       (int)(6)

/* Starting uptime in Regproxy.
*/
#define  CMREGPRX_UPTIME_NULL           (dword)(0)

/* Typedef for REG transaction id.
*/
typedef  uint32                          cmregprx_trans_type;

/* default value for measurement id
*/
#define CMREGPRX_MEAS_ID_DEFAULT      (0);

/* default value for band group position
*/
#define CMREGPRX_POS_DEFAULT      (0);

#define CMREGPRX_NUM_MAX_STACKS     2
#define CMREGPRX_GPP_STACK_0     0
#define CMREGPRX_GPP_STACK_1     1

#define CMREGPRX_DS_STAT_CHGD_TRANS_ID 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of RegProxy states.
*/
typedef enum
{

  CMREGPRX_STATE_INACTIVE,             /* REG is in Inactive state.
                                       */

  CMREGPRX_STATE_ACTIVE,               /* REG is in active state.
                                       */

  CMREGPRX_STATE_DORMANT,              /* REG is in dormant state.
                                       */

  CMREGPRX_STATE_MAX                   /* For internal use. */

} cmregprx_state_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of RegProxy sub-states.
*/
typedef enum
{

  /* 0 */
  CMREGPRX_SUBSTATE_NULL,              /* Regproxy is in idle operating
                                       ** state.
                                       */

  CMREGPRX_SUBSTATE_WAIT_SRV_CNF,      /* RegProxy is in a state waiting for
                                       ** service request confirmation from
                                       ** REG.
                                       */

  CMREGPRX_SUBSTATE_WAIT_STOP_CNF,     /* RegProxy is in a state waiting for
                                       ** Stop request confirmation from REG.
                                       */

  CMREGPRX_SUBSTATE_WAIT_NET_LIST_CNF, /* RegProxy is in a state waiting for
                                       ** get network list request confirmation
                                       ** from REG.
                                       */

  /* 4 */
  CMREGPRX_SUBSTATE_WAIT_SIM_AVL_CNF,  /* RegProxy is in a state waiting for
                                       ** confirmation for the SIM availability
                                       ** changed request.
                                       */

  CMREGPRX_SUBSTATE_WAIT_PH_STATUS_CHGD_CNF,
  /* RegProxy is in a state waiting for
  ** confirmation for the Phone status
  ** changed request.
  */

  CMREGPRX_SUBSTATE_WAIT_DS_STAT_CHGD_CNF,
  /* Regproxy is in a state waiting for
  ** confirmation for Dual standby
  ** status change request from REG.
  */

  CMREGPRX_SUBSTATE_WAIT_SRV_IND,      /* RegProxy is in a state waiting for
                                       ** SRV IND. This is used in RESEL where
                                       ** NAS responds to SRV REQ with NO SRV
                                       ** SRV IND will have the LTE srv details
                                       */

  /* 8 */
  CMREGPRX_SUBSTATE_WAIT_PDP_ABORT_CNF,
  /* RegProxy is in a state waiting for
  ** confirmation for the pdp abort
  ** request
  */

  CMREGPRX_SUBSTATE_WAIT_PS_DETACH_CNF, /* Regproxy is in a state
                                       ** waiting for PS_DETACH_CNF from REG.
                                       */

  CMREGPRX_SUBSTATE_WAIT_GEN_CMD,  /* Regproxy is in a state
                                       ** waiting for followup GEN cmd from MMOC after
                                       ** sending PS_DETACH_CNF to MMOC.
                                       */

  CMREGPRX_SUBSTATE_SIM_REFRESH_WAIT_NOT_AVL_CNF,
  /* 11
  ** Regproxy is doing SIM refresh in LPM
  ** SIM not avl is sent, waiting for CNF
  */

  CMREGPRX_SUBSTATE_SIM_REFRESH_WAIT_AVL_CNF,
  /* 12
  **Regproxy is doing SIM refresh in LPM
  ** SIM avl is sent, waiting for CNF
  */

  CMREGPRX_SUBSTATE_WAIT_STOP_MODE_UE_CHG_CNF,

  CMREGPRX_SUBSTATE_STACK_DEACT_PROGRESS,
  /* 14
    **Regproxy processed CM_STACT_DEACT_IND
    ** Waiting for deact command from MMOC
    */

  /* 15 */
  CMREGPRX_SUBSTATE_WAIT_SRV_REQ_PROCEED,  /* Regproxy moves to
                                       ** WAIT_SRV_REQ_PROCEED substate after sending
                                       ** CM_SOA_REGPRX_SRV_REQ_PROCEED to CMSOA.
                                       ** This can ensure regprx does not receie any service_ind
                                       ** during this transaction.
                                       */

  CMREGPRX_SUBSTATE_MAX                /* For internal use. */

} cmregprx_substate_e_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of srv_req difference.
*/
typedef enum
{

  CMREGPRX_SRV_REQ_DIFF_NONE,          /* No difference in srv_req comparison.
                                       */

  CMREGPRX_SRV_REQ_DIFF_FOR_REQ_ONLY,  /* Difference is only in for_request
                                       */

  CMREGPRX_SRV_REQ_DIFF_OTHER_PARMS,   /* Difference in other parms too.
                                       */


  CMREGPRX_SRV_REQ_DIFF_MAX            /* For internal use. */

} cmregprx_srv_req_diff_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Structure to hold information pertaining to background service search
*/
typedef struct
{

  /*Measurement ID for the background search */
  word meas_id;

  /* Current position for the bandgroup. Used to query SD to get the next
  ** band group to search
  */
  uint8 pos;

  /* Scan scope for BPLMN search */
  sys_scan_scope_e_type   scan_scope;


#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH

  /* user requested or background search */
  reg_cm_network_list_req_e_type list_search_type;

#endif

  boolean ignore_manual_bst;
} cmregprx_list_search_info_s_type;


/* Type to hold info needed for SIM refresh. */
typedef struct
{
  prot_oprt_mode_e_type oprt_mode;
  /* Oprt mode, updated everytime oprt chgd is recvd from mmoc */

  boolean is_gwl_subs_avail;
  /* GWL sub, updated everytime sub chgd is recvd from mmoc */

  cmregprx_cmd_msg_s_type *pending_cmd;
  /* Pending cmd from mmoc to be processed while SIM refresh in progress */

  /* Info needed by NAS to process Dual SIM sub changes. Buferred to send
     during SIM refresh commands */
  uint8 nv_context;
  sys_mode_pref_e_type mode_pref;
  mmgsdi_session_type_enum_type session_type;

} cmregprx_sim_refresh_state_s_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  cmregprx_cmd_msg_s_type *pending_cmd;
  /* Pending cmd from mmoc to be processed while SIM refresh in progress */

} cmregprx_ue_mode_switch_pending_s_type;


/* A type for parameters that holds the state and other information of the
** RegProxy.
*/
typedef struct
{

  /* Current state of RegProxy.
  */
  cmregprx_state_e_type                state;

  /* Current sub-state of RegProxy.
  */
  cmregprx_substate_e_type             substate;

  /* Transaction id of the last request to REG.
  */
  cmregprx_trans_type                  reg_trans_id;

  /* Trans_id for the MMoC's request, can handle only one request
  ** at a time.
  */
  prot_trans_type                      mmoc_trans_id;

  /* Current GSM/WCDMA/LTE subscription availability status.
  */
  cm_subs_avail_e_type                 is_gwl_subs_avail;

  /* Flag to indicate if SD20 reselection is allowed or not.
  */
  boolean                              is_resel_allowed;

  /* Start time for counting number of CM_SERVICE_REQ -> to determine
  ** CM_SERVICE_REQ should be throttled.
  */
  dword                                start_thrtl_ref_uptime;

  /* Count of CM_SERVICE_REQ during the reference period.
  */
  int                                  cnt_cm_srv_req;

  /* Camped_ind buffer
  */

  cm_rpt_type                          *camped_ind_buf;

  /* Last srv_req action.
  */
  cm_service_req_s_type                last_srv_req;

  /* new_srv_req and sd_act are populated after validating
  ** the new service request
  */
  cm_service_req_s_type                new_srv_req;

  sd_ss_act_s_type                     sd_act;

  /* To hold background search information */
  cmregprx_list_search_info_s_type    list_search_info;

  /** The reason STOP_MODE_REQ was sent to REG. */
  sys_stop_mode_reason_e_type         stop_mode_reason;

  /* mm_id containing sub id and stack id
  */
  mm_sub_stk_id_s_type                mm_id;

  sys_modem_stack_id_e_type   nas_stack_id;

#ifdef FEATURE_CM_LTE
  /*
  ** This flag indicates if the UE is in LTE Connected mode
  */
  boolean                              lte_connected_mode;
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL

  /*
  ** This flag indicates if the UE is in WCDMA Connected mode
  */
  boolean                              wcdma_connected_mode;


#endif


  boolean                              is_rlf_reset_delayed;

  /* Req. ID.
  */
  uint16                                sys_sel_pref_req_id;

  /* This variable holding the pending UE_MODE which needs to be sent to NAS
  ** SGLTE/SRLTE: This is used to resolve the scenario of multiple back-2-back ue_mode change
  ** For example, UE_MODE (home) is processed and sys_sel_pref() command is queued
  ** on MMOC.
  ** MMOC starts ue_mode switch, and send STOP_MODE to NAS. NAS has not yet responded.
  ** Meanwhile, UE_MODE (roam) is also processed and queued to MMOC. So now, CM changed UE_mode to ROAM.
  ** NAS responded STOP_MODE_CNF, which triggers ue_mode (roam) to be sent out.
  ** This is wrong, because MMOC is still processing "HOME" preference, while NAS is moving ROAM.
  **
  ** The fix:  Having UE_MODE passing through MMOC per transaction.
  ** That's why we need this variable to hold the 'pending_ue_mode' that is passed back
  ** to CMREGPRX, so CMREGPRX uses this to send to NAS.
  */
  sys_ue_mode_e_type                    pending_ue_mode;

  boolean pending_substate_srlte;

  /* SIM refresh state, valid in LPM mode only */
  cmregprx_sim_refresh_state_s_type     sim_refresh;

  cmregprx_ue_mode_switch_pending_s_type  ue_mode_switch;

  /* UE mode sent to NAS */
  sys_ue_mode_e_type                   ue_mode;

  boolean is_ue_mode_substate_srlte;

} cmregprx_stack_info_s_type;

typedef struct
{
  sys_plmn_id_s_type           hplmn;
  /* HPLMN info from NAS */

  boolean                      is_waiting_for_plmn_block_resp;

  boolean                      is_send_unblock_on_next_flpmn;

  uint8                        stk_cnt;
  /* Num of actual cmregprx stacks per sim - max supported is 2 */

  uint32                       subs_capability;

  boolean                      trm_no_resource_sent;
  /* flag to indicate if acq_status NO_RESOURCE is sent to IMS 
  ** TRUE indicate lower layer loses srv due to no trm resource.
  */

  boolean                      delay_srv_req;

  /* The uptime that a sub is in wait for proceed state to send srv request.
  */
  dword                        delay_srv_req_uptime;

  cmregprx_stack_info_s_type  *stk_info[CMREGPRX_NUM_MAX_STACKS];
  /* array of stack info ptr */

  boolean is_tau_required ;

} cmregprx_sub_info_s_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* A type for parameters that holds the state and other information of the
** RegProxy.
  */
typedef struct
{

  /* Queue for commands to MMoC task.
  */
  q_type                               cmd_q;

  /* Free queue for commands to MMoC task.
  */

  /* REX signal to be set when a RegProxy command is queued.
  */
  rex_sigs_type                        cmd_q_sig;


  /* REX task context under which the RegProxy is running.
  */
  rex_tcb_type                         *rex_tcb_ptr;

  /* Dual standby preference */
  sys_modem_dual_standby_pref_e_type   ds_pref;

  /* active_subs reported to NAS */
  uint8                               active_subs;

  /* Device mode, ie. dsds, dsda, etc */
  sys_modem_device_mode_e_type         device_mode;

  cmregprx_sub_info_s_type            *sub_info[MAX_SIMS];

  /* Num of actual number of sub
  */
  uint8                                sub_cnt;

#ifdef CM_DEBUG
  #error code not present
#endif

  /* Store FPLMN lists from NAS */
  sys_forbidden_plmn_list_s_type       fplmn_list[SYS_FORBIDDEN_LIST_TYPE_MAX];

  cmregprx_trans_type                  ds_stat_chgd_trans_id;

  prot_trans_type                      mmoc_trans_id;

  cmregprx_trans_type                  subs_chgd_trans_id;

  /* Transaction id of the last request to REG.
  */
  cmregprx_trans_type                  unique_trans_id;

} cmregprx_info_s_type;


/*===========================================================================
=============================================================================
=============================================================================
==================== REGPROXY INTERNAL FUNCTIONS ============================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION cmregprx_get_info_ptr

DESCRIPTION
  This function will fetch the pointer to the RegProxy's info.

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_info_s_type* - Pointer to Regproxy's info.
  Note! Cannot be NULL

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_info_s_type           *cmregprx_get_info_ptr
(
  void

);

/*===========================================================================

FUNCTION cmregprx_get_stk_ptr

DESCRIPTION
  This function will fetch the pointer to the RegProxy's stack info for particular stack

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_info_s_type* - Pointer to Regproxy's info.
  Note! Cannot be NULL

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_stack_info_s_type  *cmregprx_get_stk_ptr
(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================

FUNCTION cmregprx_get_sim_state_update_info

DESCRIPTION

  Return cmregprx_sim_state_update state.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmregprx_sim_state_update_s_type *cmregprx_get_sim_state_update_info
(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmregprx_reset_sim_state_update_info

DESCRIPTION
  Reset the cmregprx_sim_state_update state info on GW voice call end flag
  after handling SIM_STATE_UPDATE_IND.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmregprx_reset_sim_state_update_info
(
  sys_modem_as_id_e_type asubs_id
);
/*===========================================================================

FUNCTION cmregprx_is_user_plmn_rat_selection_pending

DESCRIPTION
  Checks if cm is waiting on service cnf for user selected plmn plus RAT service request.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if waiting for service cnf
  FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmregprx_is_user_plmn_rat_selection_pending(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================
FUNCTION cmregprx_map_sys_rat_to_sd_mode

DESCRIPTION
  This function maps the RAT to sd_mode_e_type.

DEPENDENCIES
  None.

RETURN VALUE
  SD_MODE_GSM   - mode is GSM.
  SD_MODE_WCDMA - mode is WCDMA.
  SD_MODE_INACT - invalid mode.

SIDE EFFECTS
  None.
===========================================================================*/

sd_mode_e_type                  cmregprx_map_sys_rat_to_sd_mode
(

  sys_radio_access_tech_e_type    rat
  /* RAT type.
  */

);

/*===========================================================================

FUNCTION cmregprx_map_prot_oprt_mode_to_sys

DESCRIPTION
  This function maps the mmoc enum prot_oprt_mode_e_type to sys enum
  sys_oprt_mode_e_type

DEPENDENCIES
  None.

RETURN VALUE
  sys type operating mode status.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN sys_oprt_mode_e_type           cmregprx_map_prot_oprt_mode_to_sys
(

  prot_oprt_mode_e_type               prot_oprt_mode
  /* Operating mode - prot type.
  */
);


/*===========================================================================

FUNCTION cmregprx_map_deact_to_stop_reason

DESCRIPTION
  This function will map the MMoC's protocol deactivate reason to
  REG's stop mode reason.

DEPENDENCIES
  None.

RETURN VALUE
  REG's stop mode reason.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN sys_stop_mode_reason_e_type     cmregprx_map_deact_to_stop_reason
(
  prot_deact_e_type               deact_reason
  /* MMoC's deactivate reason.
  */

);


/*===========================================================================

FUNCTION cmregprx_misc_is_gw_resel_ok

DESCRIPTION
  This function should be called to check if  GSM/WCDMA
  protocol is busy and RegProxy should not perform any reselection.


DEPENDENCIES
  cmregprx_init() should have been called previously.

RETURN VALUE
  TRUE - if reselection is allowed
  FALSE - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean                           cmregprx_misc_is_gw_resel_ok
(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================

FUNCTION cmregprx_cmd_srv_req_proceed

DESCRIPTION
  This function is called by CMSOA to allow the service request

DEPENDENCIES
  cmregprx_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void                            cmregprx_cmd_srv_req_proceed
(

  boolean                  srv_req_proceed,
  /* Boolean indicates if we allow service request to proceed
  */

  mm_sub_stk_id_s_type     mm_id
);

/*===========================================================================

FUNCTION cmregprx_send_abort_cc_req

DESCRIPTION
  This function sends the ABORT requests to REG.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void                            cmregprx_send_abort_cc_req
(
  sys_stop_mode_reason_e_type     reason,
  /* Reason for deactivating REG.
  */

  sys_modem_as_id_e_type          as_id
);

/*===========================================================================

FUNCTION cmregprx_process_lte_srv_ind_and_send_avoid_list

DESCRIPTION
  Send the avoidance list based on the service indication from LTE RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void cmregprx_process_lte_srv_ind_and_send_avoid_list
(
  /* LTE srv indication pointer */
  const lte_rrc_service_ind_s  *rrc_srv_ind_ptr,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

@FUNCTION cm_update_manual_search_bst

@DESCRIPTION
  Update the plmn/rat/band in Manual Search BST

@DEPENDENCIES

None

@RETURNS
  TRUE if message is queued.
  FALSE otherwise.

@SIDE_EFFECT

None

===========================================================================*/
extern boolean  cmregprx_update_manual_search_bst (
  lte_rrc_csp_manual_bst_update_ind_s          *lte_manual_bst_update_ind

  /* Pointer to MSGR report */
);


/*===========================================================================

FUNCTION cmregprx_cmd_perform_plmn_blocking

DESCRIPTION
  This function send PLMN blocking request to NAS.


DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmregprx_cmd_perform_plmn_blocking (

  sys_block_plmn_e_type              plmn_block_pref,

  uint32                             blocking_interval,

  mm_sub_stk_id_s_type               mm_id

);

/*===========================================================================

FUNCTION cmregprx_send_ue_mode_ind

DESCRIPTION
  This function sends the UE mode Indication to REG

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmregprx_send_ue_mode_ind(
  sys_ue_mode_e_type   ue_mode,
  /* UE mode is the mode which UE is operating on. This is not Operation Mode
     ** (ONLINE, OFFLINE, LPM).
     */
  boolean              is_ue_mode_substate_srlte,
  /* valid only if UE mode is 1XSRLTE_CSFB_PREFERRED/1XSRLTE_ONLY.
          ** It indicates if UE is in CSFB mode or SRLTE mode of operation
       */
  sys_modem_as_id_e_type    as_id

);

#if (defined CM_GW_SUPPORTED || defined FEATURE_CM_LTE)

/*===========================================================================

FUNCTION cmregprx_send_data_roaming_ind

DESCRIPTION
  This function sends the data roaming indication to REG

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmregprx_send_data_roaming_ind(

  sys_data_roaming_e_type  roam_type,
  /* Data roaming setting to be sent to NAS */

  sys_modem_as_id_e_type  as_id
);

/*===========================================================================

FUNCTION cmregprx_send_roaming_req

DESCRIPTION
  This function sends the roaming request to NAS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmregprx_send_roaming_req(

  cm_set_roaming_pref_param_s_type *roaming_pref,
  /* Data roaming setting to be sent to NAS */

  sys_modem_as_id_e_type    as_id
);


/*===========================================================================

FUNCTION cmregprx_send_close_lb_ready_ind

DESCRIPTION
  This function sends the close loop back ready indication to REG

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmregprx_send_close_lb_ready_ind(

  boolean  close_loopback,
  /* Data roaming setting to be sent to NAS */

  sys_modem_as_id_e_type  as_id
);


#endif

/*===========================================================================

FUNCTION cmregprx_get_state

DESCRIPTION
  This function will return the cmregprx stack state.

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_state_e_type

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_state_e_type  cmregprx_get_state
(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================

FUNCTION cmregprx_get_substate

DESCRIPTION
  This function will return the cmregprx stack sub state.

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_substate_e_type

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_substate_e_type  cmregprx_get_substate
(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);




/*===========================================================================

FUNCTION cmregprx_get_conn_mode_status

DESCRIPTION
  This function will return the cmregprx stack lte connected mode.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
boolean  cmregprx_get_conn_mode_status
(
  mm_sub_stk_id_s_type  mm_id,
  /* sub and stack id */

  sys_sys_mode_e_type   sys_mode
);

/*===========================================================================

FUNCTION cmregprx_get_other_stk_ptr

DESCRIPTION
  This function will fetch the pointer of the other stack on the same sub.
  If mm_id.stk_id == MM_STACK_0, return pointer of mm_id.stk == MM_STACK_2.

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_info_s_type* - Pointer to Regproxy's info.

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_stack_info_s_type  *cmregprx_get_other_stk_ptr
(
  mm_sub_stk_id_s_type  mm_id
  /* sub and stack id */
);

/*===========================================================================

@FUNCTION cmregprx_map_to_mm_stk

@DESCRIPTION
  Map cmregprx stack to CM stack.
  cmregprx stack 0 -> MM_STACK_0
  cmregprx stack 1 -> MM_STACK_2

@DEPENDENCIES
  None

@RETURNS
  multimode_stack_e_type.

@SIDE_EFFECT
  None

===========================================================================*/
multimode_stack_e_type  cmregprx_map_to_mm_stk (
  uint8 cmreg_stk
);

/*===========================================================================

FUNCTION cmregprx_get_sub_ptr

DESCRIPTION
  This function will fetch the pointer to cmregprx sub info.

DEPENDENCIES
  None.

RETURN VALUE
  cmregprx_info_s_type* - Pointer to Regproxy's info.
  Note! Cannot be NULL

SIDE EFFECTS
  None.
===========================================================================*/
cmregprx_sub_info_s_type  *cmregprx_get_sub_ptr(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmregprx_get_sub_no_trm

DESCRIPTION
  This function will get no_trm information based on lower layer indication 
  for the specified sub.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean

SIDE EFFECTS
  None.
===========================================================================*/
boolean  cmregprx_get_sub_no_trm(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmregprx_get_active_stack_on_sub

DESCRIPTION
  This function maps sub to stack index

DEPENDENCIES
  None.

RETURN VALUE
  active stack

SIDE EFFECTS
  None.
===========================================================================*/
uint8 cmregprx_get_active_stack_on_sub
(
  sys_modem_as_id_e_type          as_id
);


#ifdef CM_DEBUG
#error code not present
#endif /* CM_DEBUG*/

#endif /* CMREGPRXI_H */
