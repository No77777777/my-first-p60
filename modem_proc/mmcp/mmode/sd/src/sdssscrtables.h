/*===========================================================================

              S Y S T E M   D E T E R M I N A T I O N

     S Y S T E M   S E L E C T I O N   S C R I P T   T A B L E S

DESCRIPTION
  This header file contains all of the script tables that once appeared in
  sdssscr.c. They have been tagged with macros from sdenumdbg.h in order to
  allow them to be read as either a script table or as a string map. The
  script table is the default behavior while string map generation occurs
  when this header file is included while FEATURE_SD_ENABLE_DEBUG_STRINGS
  has been enabled. In instances where this feature is not enabled,
  sdssscrtables.h must only be included in sdssscr.c.

Copyright (c) 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sdssscrtables.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/13   xs      Make VOLTE orig to SD backend
07/19/13   jvo     Remove all MDM/MSM Fusion features
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB
01/04/13   mg      Handle DO/1x to LTE reselection failure
10/10/12   qf      Make sure sd sends ok_to_orig for hdr data call if the
                   optimized ho LTE->ehrpd on going and not disturb the redir.
09/14/12   aj      allow UE to stay on redirected system after LTE to DO HO
09/26/12   vm      Unfeaturizing ssscr_hdr_user_ss_pref_redir script
09/17/12   xs      Fix non-SVDO LTE->DO redir after 1x call end
07/23/12   xs      Added support of location based BSR
04/15/12   aj      SD to continue in BSR timeline if hybr2_bsr flag is set
04/03/12   skk     Brought ssscr_int_resel_jcdma_idle &
                   ssscr_int_resel_jcdma_call_end scripts under dynamic
                   feature flag of JCDMA.
03/25/12   cl      Two BSR timer for LTE reselection requirements support
03/14/12   cl      GW/L limited reselection to LTE/DO enhancement
02/28/12   aj      SC SVLTE support to kick LTE when DO loss occurs while in
                   1x srv
02/12/12   aj      ensure MAP avoid systems are scanned during service loss
01/11/12   aj      update emerg call handling on hybr2
01/03/12   ab      Removal of script ssscr_misc_gw_net_list
11/28/11   ak      Change BSR behaviour when it involves 3GPP
11/15/11   ar      Added new script to handle pref change due to end of
                   call while another call is held
10/21/11   vk      Acquire LTE if SRV_LOST event is received in 1XCSFB call
                   conv state.
08/04/11   xs      Replace FEATURE_MMODE_NIKEL with FEATURE_MMODE_SC_SVLTE
07/24/11   aj      SC SVLTE 1.0 merge to mainline
07/14/11   rk      Adding missing ssscr_reserved_script and comments
06/06/11   rk      Revising fix for 1XCSFB call end handling
06/02/11   rk      Fix for BSR issue when 1XCSFB call is ended and LTE srv
                   is not available
05/17/11   rn      WCDMA channel locking feature
04/13/11   ak      Added support for scanning skipped GW systems on MSM.
04/07/11   ak      Fix issues in HDR-->LTE BSR on Fusion MDM.
03/27/11   anm     Added ssscr_user_ss_pref_opr_gw_emerg_call_end_ltd:
                   Send srv_req after 911 call end and NW sel mode is AUTO.
03/25/11   gm      LTE (Lmtd) to DO resel support
03/02/11   rk      Fixing issues for coming back to LTE after CSFB call end.
02/17/11   cl      Add support FEATURE_MMODE_REMOVE_1X for FUSION-MDM
01/31/11   gm      Extend acquisition while in ECBM & sys loss
02/07/11   xs      Dynamic control support of FEATURE_JCDMA
01/19/11   ak      Added support for periodic GWL scans in DO only coverage.
01/17/10   rk      Fix for reaquiring LTE service back after MT CSFB call end
01/15/10   rk      Making the CSFB script applicable for both 3GPP and 1XCSFB
12/23/10   sv      Integrate DSDS changes
12/10/10   rk      Adding e1xcsfb call initial support
11/24/10   rk      changes for 1XCSFB redirection based call
10/06/10   rm      Added ssscr_user_ss_pref_opr_gw_emerg_call_orig_ltd:
                   Send srv_req to NAS when 911 call is originated in ltd_srv
10/05/10   gm      sdssscr_non_condition_cons_print to simplify calling func
10/01/10   ak      Merged the Fusion code from Dev branch.
08/31/10   ak      Added scripts for EOOS optimization.
08/20/10   DQ      Initial release.

===========================================================================*/

#include "sdenumdbg.h" /* Contains all macros that describe the
                       ** enumerations.
                       */

/*---------------------------------------------------------------------------
                           User Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with user
** events.
*/
/*lint -esym(551, dsssscr_user_event_tbl_string_map) */
SD_BEGIN_SCRIPT (dsssscr_user_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_up_emerg),
                                      /* User commands phone to power-up),
                                      ** in emergency mode.
                                      ** be done.
                                      */
                                      /* 1 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_up),
                                      /* User commands phone to power-up */

                                      /* 2 */
  SD_ENUM_ELEMENT(ssscr_user_offline_cdma),
                                      /* User commands phone to enter offline
                                      ** CDMA mode of operation */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#include "mmcp_variation.h"
  #if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
                                      /* 3 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw_manual_wakeup_acq),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target and the
                                      ** orig mode is manual mode. The phone
                                      ** is currently doing powersave wake up
                                      ** acquisitions.
                                      */
                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw_manual),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target and the
                                      ** orig mode is manual mode.
                                      */
                                      /* 5 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw_pwrup_wakeup_acq),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target and no
                                      ** system has been acquired since power
                                      ** up. The phone is currently doing
                                      ** powersave wake up acquisitions.
                                      */
                                      /* 6 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw_pwrup),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target and no
                                      ** system has been acquired since power
                                      ** up. */

                                      /* 7 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw_wakeup_acq),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target. The phone
                                      ** is currently doing powersave wake up
                                      ** acquisitions.
                                      */
                                      /* 8 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit_gw),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems in GW only target. */

  #else

  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),

  #endif /* (FEATURE_GSM) || (FEATURE_WCDMA)  */

                                        /* 9 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_list_full),
                                      /* User changes the SS-Preference and
                                      ** origination mode is not emergency and
                                      ** emergency list is full */

                                      /* 10 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_ue_cap_change),
                                      /* User changes theUE capability while
                                      ** in operation state 
                                      */
                                      /* 11 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_1xcsfb_ecbm),
                                      /* User changes the SS-Preference while
                                      ** in operation state and the serving
                                      ** system is a CDMA system that does
                                      ** not conflict with the new
                                      ** SS-Preference */

                                      /* 12 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_save_exit),
                                      /* User commands phone to exit power-
                                      ** save mode and to start searching for
                                      ** systems */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      /* 13 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_3gpp_srv_lost_c2k_scan_opt),
                                        /* UE changes SS-Preferences to 
                                        ** enter full rat mode */
  

  #if defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)

                                      /* 14 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sv_opr_switch_csfb),
                                        /* UE changes SS-Preferences to switch
                                        ** to CSFB operation from SVLTE */

                                      /* 15 */
  SD_ENUM_ELEMENT(ssscr_hybr2_user_ss_pref_sv_opr_switch_svlte),
                                        /* UE changes SS-preferences to switch
                                        ** to SVLTE operation from CSFB */

                                      /* 16 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sv_opr_switch_svlte),
                                      /* UE changes SS-preferences to switch
                                      ** to SVLTE operation from CSFB */
  #else

  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),

  #endif

                                      /* 17 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_1x_dereg_end),
                                      /* User changes ss-preference 1x
                                         PPP deregistration clean up */

                                      /* 18 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_gps_end),
                                      /* User changes SS-Preference to end
                                      ** GPS mode */

                                      /* 19 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_3gpp_ps_call_connect_gw_opr),

                                      /* 20 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_avoid_sys),
                                      /* User commands phone to avoid the
                                      ** system
                                      */

                                      /* 21 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_hdr_cdma_ho),
                                      /* User changes the SS-Preference for
                                      ** handing off the call to 1X */

                                      /* 22 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_not_norm),
                                      /* User changes the SS-Preference and
                                      ** origination mode change is not
                                      ** normal (i.e. other than none to
                                      ** normal or normal to none) */

                                      /* 23 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_mode_pref_none),
                                      /* User changes the SS-Preference and
                                      ** new mode pref is NONE */

                                      /* 24 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_orig_after_hard_failure_ran),
                                      /* Emergency call preferences
                                      ** being forced on Main stack after HardfailureRAN
                                      ** for the first time */

                                      /* 25 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_list_empty),
                                      /* User changes the SS-Preference in
                                      ** order to originate an emergency
                                      ** call and emergency acquisition list
                                      ** is empty */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Optimized silent redial Scripts.
  */

                                      /* 26 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_start),
                                      /* User changes the SS-Preference in
                                      ** order to start an origination and
                                      ** redial list is empty */

                                      /* 27 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_hdr_opti_redial_new),
                                         /* User changes the SS-Preference in
                                      ** order to start optimized silent
                                      ** redial when SD is not in service and the enhanced 
                                      ** SR feature is enabled*/

                                      /* 28 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial_start_hdr_only),
                                      /* User changes the SS-Preference in
                                      ** order to start optimized silent
                                      ** redial in HDR only mode
                                      ** & redial list is empty */

                                      /* 29 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial_start_prl),
                                      /* User changes the SS-Preference in
                                      ** order to start optimized silent
                                      ** redial and idle system is from
                                      ** PRL */

                                      /* 30 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial_start),
                                      /* User changes the SS-Preference in
                                      ** order to start optimized silent
                                      ** redial and idle system is not from
                                      ** PRL */

                                      /* 31 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial_hdr_only),
                                      /* User changes the SS-Preference in
                                      ** order to start optimized silent
                                      ** redial in HDR only mode */

                                      /* 32 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial_prl),
                                      /* User changes the SS-Preference in
                                      ** order to do optimized silent redial
                                      ** and idle system is from PRL */

                                      /* 33 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redial),
                                      /* User changes the SS-Preference in
                                      ** order to do optimized silent redial
                                      ** and idle system is not from PRL */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 34 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_otasp_sys_ok),
                                      /* User changes the SS-Preference to
                                      ** originate an OTASP call and idle
                                      ** system is from OTASP activation
                                      ** block */

                                      /* 35 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_otasp),
                                      /* User changes the SS-Preference to
                                      ** originate an OTASP call */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
                                      /* 36 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_colloc),
                                      /* User changes the SS-Preference to
                                      ** originate on a collocated system */
     
                                      /* 37 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_csg_change_acq),
                                           /* User selects csg cell while
                                           ** in acquisition state 
                                           */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 38 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_conf),
                                      /* User changes the SS-Preference while
                                      ** in operation state and serving
                                      ** system conflicts with the new
                                      ** SS-Preference */

                                      /* 39 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_lock),
                                      /* User changes the SS-Preference while
                                      ** in operation state and MS is CDMA
                                      ** locked until power-cycle */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  #if defined(FEATURE_LTE_TO_1X)
                                      /* 40 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_lte_csfb_orig_end),
                                      /* 1XCSFB call got ended
                                      ** So reaquire LTE system again
                                      */

                                      /* 41 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_csfb_orig_end),
                                      /* 1XCSFB call got ended
                                      ** So reaquire LTE system again
                                      */

                                      /* 42 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_csfb_orig_end),
                                      /* 1XCSFB call got ended
                                      ** So reaquire LTE system again
                                      */
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif

                                      /* 43 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_map_call_end),
                                      /* MO call ends while in operation
                                      ** state and the acquistion system is a
                                      ** CDMA MAP system that does not
                                      ** conflict with the new SS-Preference
                                      */

                                      /* 44 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_orig_end),
                                      /* User changes the SS-Preference while
                                      ** in operation state and the serving
                                      ** system is a CDMA system that does
                                      ** not conflict with the new
                                      ** SS-Preference */

                                      /* 45 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma),
                                      /* User changes the SS-Preference while
                                      ** in operation state and the serving
                                      ** system is a CDMA system that does
                                      ** not conflict with the new
                                      ** SS-Preference */

                                      /* 46 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_hdr),
                                      /* User changes the SS-Preference while
                                      ** in operation state and the serving
                                      ** system is an HDR system that does
                                      ** not conflict with the new
                                      ** SS-Preference */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #ifdef FEATURE_MMODE_SC_SVLTE
                                      /* 47 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_stop_hybr2_bsr),
                                      /* stop BSR timeline on HYBR2 */

  SD_ENUM_ELEMENT(ssscr_user_ss_pref_stop_hybr2_bsr_sync_up),
                                      /* stop BSR timeline on HYBR2 due to syncup*/


                                      /* 49 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_more_pref_do_scan_reacq),
                                      /* Reacq srv sys on HYBR2 after LTE to DO
                                      ** BSR failed */

                                      /* 50 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_sync_up_wakeup_acq),
                                      /* Stack sync up on HYBR2
                                      ** while it is doing wakeup acquisition */

                                      /* 51 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_sync_up_hybr_bsr_resume_gwl),
                                      /* Stack sync up on HYBR2 during HYBR_BSR
                                      ** state to resume full-srv acquisition */

                                      /* 52 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_sync_up_pwr_save),
                                      /* Stack sync up on HYBR2
                                      ** while it is in power save */

                                      /* 53 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_sync_up_no_srv),
                                      /* Stack sync up on HYBR2
                                      ** while it is in no srv */

                                      /* 54 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_sync_up_opr),
                                      /* User changes the SS-Preference while
                                      ** in operation state and the serving
                                      ** system is GW system.
                                      */

                                      /* 55 */
  //SD_ENUM_ELEMENT(ssscr_user_ss_pref_lbsr_lte_not_avail),
                                        /* Start Long BSR on HYBR2
                                        ** when 1x is in LTENotAvail */

                                      /* 56 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_ss_pref_activate_silent),
                                        /* Start BSR on HYBR2
                                        ** without any immediate BSR  */

                                      /* 57 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_subsc_change_stack_sync_up_bsr),
                                      /* Start BSR on HYBR2
                                      ** without any immediate BSR  */

                                      /* 58 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_stack_sync_up_bsr),
                                      /* Start BSR on HYBR2
                                      ** due to 1x or DO srv  */
  #else
                                      /* 47 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_bsr_start),
                                      /* Start BSR on MAIN
                                      ** due to  DO srv on HYBR1 */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script), /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script), /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */

  #endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  #if defined(SD_GSM_SUPPORTED) || defined(SD_WCDMA_SUPPORTED) || defined(FEATURE_SD_LTE)

                                        /* 58 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_resel_lmtd), 
                                        /* User changes the SS-Preference while
                                         ** MS is doing reselection after
                                         ** acquired the limited system */

                                        /* 59 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_lte_emerg_not_supp),
                                        /* User changes the SS-Preference while
                                        ** in operation in LTE Limited/full srv,
                                        ** to make emergency call while LTE
                                        ** does not support emergency
                                        */

                                        /* 60 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_lte_emerg_supp),
                                        /* User changes the SS-Preference while
                                        ** in operation in LTE full srv,
                                        ** to make emergency call while LTE
                                        ** supports emergency
                                        */

                                        /* 61 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_lte_emerg_cb),
                                        /* User changes the SS-Preference while
                                        ** in operation in LTE Limited/full srv,
                                        ** to enter ECBM.
                                        */
    
                                        /* 62 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_lte_emerg_cb_end),
                                        /* User changes the SS-Preference while
                                        ** in LTE full srv ECBM in order to exit,
                                        ** ECBM.
                                        */

                                        /* 63 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg_call_orig_w_lock),
                                        /* User changes the SS-Preference while
                                        ** in operation in TGW Limited/full srv,
                                        ** to make emergency call while WCDMA
                                        ** freq lock is enabled.
                                        */

                                        /* 64 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_gw_emerg_call_orig_full_srv_req_man),
                                      /* User changes the SS-Preference while
                                      ** in TGW OOS recovery.
                                      */

                                        /* 65 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg_call_orig_ltd),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW Limited srv, to
                                      ** make emergency call.
                                      */

                                        /* 66 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg_call_end_ltd),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW Limited srv, to
                                      ** end emergency call.
                                      */

                                        /* 67 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_orig_end_ltd_reg),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW Limited regional srv, to
                                      ** end emergency call.
                                      */

                                        /* 68 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg_end_srv),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW to end the call and in fULL SRV
                                      */

                                        /* 69 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW to make emergency
                                      ** call.
                                      */

                                        /* 70 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_emerg_vrfy_state),
                                      /* User changes the SS-Preference while
                                      ** in Verify state in TGW to make emergency
                                      ** call.
                                      */

                                        /* 71 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_end_lte_only),
                                      /* User changes the SS-Preference while
                                      ** in operation in TGW to make emergency
                                      ** call.
                                      */

                                        /* 72 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_cs_in_ecall),
                                        /* User starts CS call when SD has GW
                                        ** service and no mode conflicts.
                                        ** Even if domain conflict), SD must
                                        ** continue operation and not issue
                                        ** acquistion request for CS service
                                        */

                                        /* 73 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_lte_ltd_reg),

                                        /* 74 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_call_orig_ltd),
                                      /* User starts call origination while in
                                      ** GW limited service in auto/manual mode.
                                      */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved */
                                      /* 78 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_start_csps),
                                        /* User starts ps_call when SD doesn't have
                                        ** both services.
                                        */

  #if defined(FEATURE_LTE_TO_1X) || defined (FEATURE_3GPP_CSFB)

                                      /* 79 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_csfb_orig_start),
                                        /* User starts CSFB call when SD has LTE
                                        ** service and no mode conflicts.
                                        ** Even if domain conflict), SD must
                                        ** continue operation and not issue
                                        ** acquistion request for CS service
                                        */

                                      /* 80 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_cdma_csfb_orig_start),
                                        /* SD gets 1XCSFB call pref when SD has CDMA
                                        ** continue operation and not issue
                                        ** acquistion request
                                        */

  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif

                                      /* 81 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_volte_orig),
                                        /* User starts VOLTE call when SD has LTE
                                        ** full service. SD won't issue service request
                                        */

                                      /* 82 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_start_cs),
                                        /* User starts CS call when SD has GW
                                        ** service but not CS domain. SD issues
                                        ** acquistion request for CS service
                                        */

                                      /* 83 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_cs_no_conf),
                                        /* User starts CS call when SD has GW
                                        ** service and no mode or domain
                                        ** conflicts. SD must continue
                                        ** operation and not issue
                                        ** acquistion request for CS service
                                        */

                                      /* 84 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_ps_no_conf),
                                        /* User starts PS call when SD has GW
                                        ** service and no mode or domain
                                        ** conflicts. SD must continue
                                        ** operation and not issue
                                        ** acquistion request for PS service
                                        */

                                      /* 85 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_ps_resume_full_srv),
                                        /* User trying to resume data for
                                        ** dormant PS call when SD has PS
                                        ** service and SD is in operation state.
                                        */

                                      /* 86 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_ps_resume_no_ps_srv),
                                        /* User trying to resume data for
                                        ** dormant PS call when SD is not
                                        ** having PS service and SD is in
                                        ** operation state.
                                        */

                                      /* 87 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_ps_resume_ltd_srv),
                                        /* User trying to resume data for
                                        ** dormant PS call when SD is having
                                        ** limited service and SD is in
                                        ** operation state
                                        */

                                      /* 88 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_ps_resume_no_srv_any),
                                        /* User trying to resume data for
                                        ** dormant PS call when SD is in
                                        ** no service or power save.
                                        */
                                      /* 89 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_redial_ps),
                                        /* User changes the SS-Preference while
                                        ** in operation state and the serving
                                        ** system is GW system while CS orig fails
                                        ** and needs to redial over PS
                                        */

                                      /* 90 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw),
                                        /* User changes the SS-Preference while
                                        ** in operation state and the serving
                                        ** system is GW system.
                                        */

                                      /* 91 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_end),
                                        /* User ends the call  while
                                        ** in operation state and the serving
                                        ** system is GWL system.
                                        */

                                      /* 92 */ 
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig_end_lmtd_regional), 
                                        /* User ends the call  while 
                                        ** in operation state and the serving 
                                        ** system is GWL system and the serving 
                                        ** status is limited regional. 
                                        */ 

  #else

  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),

  #endif /* (SD_GSM_SUPPORTED) || (SD_WCDMA_SUPPORTED) || (FEATURE_SD_LTE) */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      /* 93 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_domain),
                                      /* User changes the Domain Preference while
                                      ** in operation state and the serving
                                      ** system is GW system in Manual Mode..
                                      */

                                      /* 94 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr_gw_orig),
                                              /* User changes the SS-Preference while
                                      ** in full service state.
                                      */

                                      /* 95 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_csg_change_opr),
                                           /* User selects csg cell while
                                           ** in operation state 
                                           */                                      
                                      /* 96 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_opr),
                                      /* User changes the SS-Preference while
                                      ** in operation state.
                                      */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 97 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_pseudo_online),
                                    /* User changes the SS-Preference to put
                                    ** the phone to pseudo online state
                                    */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 98 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redir_emerg),
                                      /* User changes the SS-Preference to
                                      ** originate an emergency call while
                                      ** working on a redirection */

                                      /* 99 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_redir),
                                      /* User changes the SS-Preference while
                                      ** working on a redirection */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 100 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_resel_lmtd_reg),
                                      /* User changes the SS-Preference while
                                      ** MS is doing reselection after
                                      ** acquired the limited regional system */

                                      /* 101 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_resel_lmtd),
                                      /* User changes the SS-Preference while
                                      ** MS is doing reselection after
                                      ** acquired the limited system */

                                      /* 102 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_resel),
                                      /* User changes the SS-Preference while
                                      ** MS is doing reselection */

  SD_ENUM_ELEMENT(ssscr_user_ss_pref_emerg_orig_3gpp),
                                      /* Emerg call orig for 3GPP only devices */
  
                                      /* 103 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_db_ps_orig),    
                                      /* PS orig during acq db scan at NAS */

                                      /* 104 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_vrfy_orig),
                                      /* User changes the SS-Preference while
                                      ** in verification state in order to
                                      ** originate call other than OTASP */

                                      /* 105 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_orig_normal),
                                      /* User changes the SS-Preference while
                                      ** in acquisition state in order to
                                      ** originate a normal call */

                                      /* 106 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_acq_orig_emerg),
                                      /* User changes the SS-Preference while
                                      ** in acquisition state in order to
                                      ** originate an emergency call */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_end_w_l_connected_mode),    /* Reserved script */

                                      /* 109 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_end_ltd_reg),
                                   /* call origination end when in limited regional*/

                                      /* 110 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_end_vrfy),
                                      /* Call origination ended when in verify */

                                      /* 111 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_end),
                                      /* Call origination ended */

  #ifdef FEATURE_HICPS_STACK_SYNC_UP


  #if (defined FEATURE_MMODE_SC_SVLTE)

                                      /* 112 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sync_up_hybr_bsr_resume_gwl),
                                     /* Resume Lmtd->Full after Hybr BSR */

                                      /* 113 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sync_up_hybr_bsr_continue),
                                     /* Resume Lmtd->Full after Hybr BSR */
  #else

  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  
  #endif /* FEATURE_MMODE_SC_SVLTE */

                                      /* 114 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_orig_resume),
                                      /* pref change), ps orig ersume
                                      */

                                      /* 115 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_mode_chg_in_sync_up),
                                      /* pref change), while in sync-up
                                      */

                                      /* 116 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sync_up_mode_chg_gwl_hybr1_srv),
                                      /* restoring GWL after HDR acquired. */

  #ifdef FEATURE_MMODE_SC_SVLTE

                                      /* 117 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sync_up_mode_chg_gwl_hybr2_srv),
                                      /* restoring GW after LTE acquired. */
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  
  #endif /* FEATURE_MMODE_SC_SVLTE */

                                      /* 117 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_sync_up_mode_chg),
                                      /* pref change for sync-up
                                      */
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  
  #endif /* FEATURE_HICPS_STACK_SYNC_UP */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      
  SD_ENUM_ELEMENT(ssscr_reserved_script),     /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref_ue_mode_chg),     
                                      /* User change event due to UE mode change */
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 121 */
  SD_ENUM_ELEMENT(ssscr_user_ss_pref),
                                      /* User changes the SS-Preference */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)

                                      /* 122 */
  SD_ENUM_ELEMENT(ssscr_user_nam_sel_gw_opr),
                                      /* User changes the NAM selection in
                                      ** operation mode), and the target
                                      ** capability is GW only */

                                      /* 123 */
  SD_ENUM_ELEMENT(ssscr_user_nam_sel_gw),
                                      /* User changes the NAM selection and
                                      ** the target capability is GW only */

  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif /* (FEATURE_GSM) || (FEATURE_WCDMA)  */

                                      /* 124 */
  SD_ENUM_ELEMENT(ssscr_user_nam_sel_manual),
                                      /* User changes the NAM selection), and
                                      ** orig_mode is manual. */

                                      /* 125 */
  SD_ENUM_ELEMENT(ssscr_user_nam_sel),
                                      /* User changes the NAM selection */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 126 */
  SD_ENUM_ELEMENT(ssscr_user_pwr_down),
                                      /* User commands phone to phone to
                                      ** power-off), perform a reset or enter
                                      ** LPM */

  #if defined(SD_GSM_SUPPORTED) || defined(SD_WCDMA_SUPPORTED)
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 127 */
  SD_ENUM_ELEMENT(ssscr_user_get_net_auto_mode_acq_state),
                                      /* User commands to get the available
                                      ** networks in auto mode
                                      */

                                      /* 128*/
  SD_ENUM_ELEMENT(ssscr_user_get_net_auto_mode_pwr_save_state),
                                      /* User commands to get the available
                                      ** networks in auto mode
                                      */
                                      
                                      /* 129 */
  SD_ENUM_ELEMENT(ssscr_user_get_net_auto_mode_opr_state),
                                      /* User commands to get the available
                                      ** networks in auto mode
                                      */                                 
                                      /* 130 */     
  SD_ENUM_ELEMENT(ssscr_user_get_net_auto_mode_opr_state_csg),
                                      /* CSG get network search
                                                                  ** when camped on 3gpp2 system
                                                                   */

                                      /* 131 */
  SD_ENUM_ELEMENT(ssscr_user_get_net),
                                      /* User commands to get the available
                                      ** networks
                                      */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 132 */
  SD_ENUM_ELEMENT(ssscr_hybr_2_user_term_get_net),

                                      /* 133 */
  SD_ENUM_ELEMENT(ssscr_user_term_get_net),
                                      /* User commands to terminate the get
                                      ** available networks command.
                                      */
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif /* (SD_GSM_SUPPORTED) || (SD_WCDMA_SUPPORTED)  */
                  
                                      /* 134 */                    
  SD_ENUM_ELEMENT(ssscr_hybr_1_user_hybr_start),
                                      /* User commands to start CDMA+HDR
                                      ** hybrid mode of operation.
                                      */

                                      /* 135 */
  SD_ENUM_ELEMENT(ssscr_hybr_1_user_hybr_end),
                                      /* User commands to end CDMA+HDR
                                      ** hybrid mode of operation.
                                      */

                                      /* 136 */
  SD_ENUM_ELEMENT(ssscr_user_cdma_redir_lte_invalid),
                                      /* MS invalid LTE to 1x redirection
                                      ** message
                                      */

                                      /* 137 */
  SD_ENUM_ELEMENT(ssscr_user_cdma_redir_lte),
                                      /* MS redirection message LTE to 1x6                                      */
  #if defined(FEATURE_LTE_TO_1X)

                                      /* 138 */
  SD_ENUM_ELEMENT(ssscr_user_cdma_ho_lte),
                                      /* MS ho message LTE to 1x
                                      */
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif

                                      /* 139 */
  SD_ENUM_ELEMENT(ssscr_user_prot_deactivate)

};

#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)


/* <EJECT> */

/*---------------------------------------------------------------------------
                    CDMA Acquisition Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with CDMA
** acquisition events.
*/
/*lint -esym(551, dsssscr_cdma_acq_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_cdma_acq_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_cdma_acq_failed_pilot_reacq),
                                      /* Exception script for when
                                      ** acquisition fails during the pilot
                                      ** channel acquisition sub-state while
                                      ** attempting to reacquire a system and
                                      ** this system is not already on a
                                      ** reacquisition schedule */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_acq_failed_sync_gsrdm_source_channel),
                                      /* Exception SS-Script for when
                                      ** acquisition fails during the sync
                                      ** channel acquisition sub-state while
                                      ** attempting to reacquire a source system 
                                      ** of GSRDM indication and all the channels in the 
                                      ** destination list of GSRDM indication were permanent mapped channels*/

  SD_ENUM_ELEMENT(ssscr_cdma_acq_failed_sync_reacq),
                                      /* Exception SS-Script for when
                                      ** acquisition fails during the sync
                                      ** channel acquisition sub-state while
                                      ** attempting to reacquire a system and
                                      ** this system is not already on a
                                      ** reacquisition schedule */

  SD_ENUM_ELEMENT(ssscr_cdma_acq_failed_sync),
                                      /* Exception SS-Script for when
                                      ** acquisition fails during the sync
                                      ** channel acquisition sub-state while
                                      ** attempting to acquire a system and
                                      ** this system is not already on a
                                      ** reacquisition schedule */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_cdma_acq_prot_mis),
                                      /* CDMA acquisition fails due to a
                                      ** protocol revision mismatch (i.e.),
                                      ** MOB_P_REV < MIN_P_REV or MS does not
                                      ** support the Paging Channel data
                                      ** rate */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_acq_ho_fail)
                                      /* CDMA handoff fails
                                      */

};
#endif /* defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900) */

#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)

/*---------------------------------------------------------------------------
                     CDMA Operation Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with CDMA
** operation events.
*/
/*lint -esym(551, dsssscr_cdma_opr_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_cdma_opr_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_new_redir),
                                      /* A new CDMA system is fully acquired
                                      ** and the SID/NID is new and
                                      ** redirection indication is on */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_conf_vrfy),
                                      /* A new CDMA system is fully acquired
                                      ** and this system conflicts with the
                                      ** current SS-Preference and the
                                      ** SS-State is verify */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_frbd_vrfy),
                                      /* A new CDMA system is fully acquired
                                      ** and this system is forbidden and
                                      ** origination mode is other than
                                      ** emergency and the SS-State is
                                      ** verify */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_neg_vrfy_redir),
                                      /* A new CDMA system is fully acquired
                                      ** and this system is negative),
                                      ** redirection is ON), origination mode
                                      ** is other than emergency and the
                                      ** SS-State is verify */

                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_vrfy_absr),
                                      /* A new CDMA system is fully acquired
                                      ** during ABSR and the registration
                                      ** status of this system is current.
                                      */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_vrfy_aoos),
                                      /* A new CDMA system is fully acquired
                                      ** during AOOS and the registration
                                      ** status of this system is not current.
                                      */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_conf_mode),
                                      /* A new CDMA system is fully acquired
                                      ** and this system conflicts with the
                                      ** current mode preference */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_conf),
                                      /* A new CDMA system is fully acquired
                                      ** and this system conflicts with the
                                      ** current SS-Preference */

                                      /* 8 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_frbd),
                                      /* A new CDMA system is fully acquired
                                      ** and this system is forbidden and
                                      ** origination mode is other than
                                      ** emergency */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_pref_new),
                                      /* A new CDMA serving system is fully
                                      ** acquired), but there are more
                                      ** preferred systems in the serving
                                      ** system's GEO. The SID/NID of the new
                                      ** serving system is different from the
                                      ** previous SID/NID */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_pref),
                                      /* A new CDMA serving system is fully
                                      ** acquired), but there are more
                                      ** preferred systems in the serving
                                      ** system's GEO */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_emerg_redir),
                                      /* A new CDMA serving system is fully
                                      ** acquired and origination mode is
                                      ** emergency and redirection is ON */

                                      /* 12 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_emerg),
                                      /* A new CDMA serving system is fully
                                      ** acquired and origination mode is
                                      ** emergency */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_handoff),
                                      /* A new CDMA serving system is fully
                                      ** acquired and handoff indication is
                                      ** ON */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_gsrdm_srdm_ndss),
                                      /* A new CDMA serving system is fully
                                      ** acquired), but MS got to acquired
                                      ** system through a CDMA GSRM received
                                      ** on not most preferred system */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid_pref_resel),
                                      /* A new CDMA serving system is fully
                                      ** acquired), but there are more
                                      ** preferred systems in the serving
                                      ** system's GEO */

                                      /* 16 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_sid),
                                      /* A CDMA system with a new SID/NID is
                                      ** fully acquired */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_oh_chan),
                                      /* A CDMA system with a new channel has
                                      ** been fully acquired. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_rescan),
                                      /* MS receives a SPM with the RESCAN
                                      ** field set to 1 */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_reg_rej_emerg),
                                      /* CDMA registration is rejected while
                                      ** in emergency (origination or
                                      ** callback) mode */

                                      /* 20 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_reg_rej_redir_rif),
                                      /* CDMA registration is rejected while
                                      ** redirection indication is ON and
                                      ** RETURN_IF_FAILED = 1 */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_reg_rej_redir),
                                      /* CDMA registration is rejected while
                                      ** redirection indication is ON */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_reg_rej),
                                      /* Registration is rejected */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_lte_irat_fail),
                                      /* CDMA to LTE reselection failed but
                                      ** CDMA serving system was not lost
                                      */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_int_sys_lost_no_resource),
								/* CDMA Serving system is lost	- treated as internal serving system lost */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_map_prl),
                                      /* CDMA Serving sysetm is lost when
                                      ** the acq system is in the avoided
                                      ** list due to MAP. And IDL_SYS is in
                                      ** the PRL.*/

                                      /* 26 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_map),
                                      /* CDMA Serving sysetm is lost when
                                      ** the acq system is in the avoided
                                      ** list due to MAP. And IDL_SYS is not
                                      ** in the PRL.*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_vrfy_sched),
                                      /* CDMA Serving system is lost during
                                      ** the VERIFY SS-State and acquired
                                      ** system is to be scheduled for
                                      ** reacquisition attempts */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_vrfy),
                                      /* CDMA Serving system is lost during
                                      ** the VERIFY SS-State */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_otasp_emerg),
                                      /* CDMA Serving system is lost while in
                                      ** origination mode is OTASP or
                                      ** emergency */

                                      /* 30*/
  #ifdef FEATURE_JCDMA
  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_over_reach),
                                      /* CDMA Serving system is lost and MS
                                      ** is in over-reach (can not reach
                                      ** the BS) */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  #endif

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_csfb_orig),/* CDMA Serving system is lost while
                                      ** user trying or originate a 1XCSFB call */

                                      /* 32 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost_orig),
                                      /* CDMA Serving system is lost while
                                      ** user trying or originate a call */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_lost),
                                      /* CDMA Serving system is lost */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_invalid_gsrm_rif),
                                      /* MS receives invalid GSRM and
                                      ** RETURN_IF_FAILED = 1 */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_invalid_gsrm),
                                      /* MS receives invalid GSRM */

                                      /* 36 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_invalid),
                                      /* MS receives invalid redirection
                                      ** message */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_emerg),
                                      /* MS redirected while in emergency
                                      ** origination mode */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_gsrdm_all_perm_map_rif), 
                                     /* MS received GSRDM indication, with all the systems 
                                     in destination list are permanent avoided systems and RIF is TRUE */
  
  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_gsrdm_all_perm_map),
                                    /* MS received GSRDM indication, with all the systems 
                                     in destination list are permanent avoided systems and RIF is FALSE */

                                      /* 40 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_gsrdm_rif),
                                    /* MS received GSRDM indication, and RIF is TRUE */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_gsrdm),
                                    /* MS received GSRDM indication, and RIF is FALSE */


  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir_rif),
                                      /* MS receives redirection message and
                                      ** RETURN_IF_FAILED = 1 */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_redir),
                                      /* MS receives redirection message */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 44 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_ndss_off),
                                      /* MS receives NDSS off indication */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_new_sys),
                                      /* MS idle-handoff to a BS with unknown
                                      ** configuration */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_resel_emerg_cb),
                                      /* MS detects that pilot power is below
                                      ** a BS specified threshold and
                                      ** origination mode is emergency
                                      ** callback */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_resel_prl),
                                      /* MS detects that pilot power is below
                                      ** a BS specified threshold and idle
                                      ** system is a preferred PRL system */

                                      /* 48 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_sys_resel),
                                      /* MS detects that pilot power is below
                                      ** a BS specified threshold */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_lock),
                                      /* MS receives a lock order */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_acc_success_acq),
                                      /* MS has a successful access and
                                      ** acq_sys is MAP system
                                      */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_acc_success_srv),
                                      /* MS has a successful access and
                                      ** srv_sys is MAP system
                                      */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_acc_fail_pwr_reg),
                                      /* MS has a successful access and
                                      ** srv_sys is MAP system
                                      */

  #if defined(FEATURE_LTE_TO_1X)

                                      /* 53 */
  SD_ENUM_ELEMENT (ssscr_cdma_opr_acc_term_csfb),
                                       /* Redirection based CSFB
                                       ** call is released
                                       */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  #endif

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_traffic),
                                      /* MS transitions from TC to idle
                                      ** without calling on release
                                      ** indication */


  #ifdef FEATURE_JCDMA

                                      /* 55 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_overreach_l),
                                      /* MS is in over-reach condition (i.e.
                                      ** can not reach the BS) for long
                                      ** time */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_overreach),
                                      /* MS is in over-reach condition (i.e.
                                      ** can not reach the BS) */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  #endif

                                      /* 57 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_emerg_cb),
                                      /* Access attempt terminates and
                                      ** emergency callback mode is in
                                      ** effect */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_acc_den),
                                      /* Access attempt terminates due to
                                      ** access denied */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_timeout_emerg),
                                      /* Access attempt terminates due to
                                      ** timeout and origination mode is
                                      ** emergency */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_emerg),
                                      /* Access attempt terminates due to
                                      ** max access probes and origination
                                      ** mode is emergency */

                                      /* 61 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_otasp),
                                      /* Access attempt terminates due to
                                      ** max access probes and origination
                                      ** mode is OTASP */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_prl_srda),
                                     /* Access attempt terminates due to
                                     ** max access probes and idle system is
                                     ** a listed PRL system),
                                     ** with SRDA ENABLED */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_prl2),
                                      /* Access attempt terminates due to
                                      ** max access probes), idle system is
                                      ** a listed PRL system and in the avoid
                                      ** state. */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_prl),
                                      /* Access attempt terminates due to
                                      ** max access probes and idle system is
                                      ** a listed PRL system */

                                      /* 65 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map_srda),
                                      /* Access attempt terminates due to
                                      ** max access probes), and SRDA
                                      ** is ENABLED */


  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map2),
                                      /* Access attempt terminates due to
                                      ** max access probes and in the avoid
                                      ** state. */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_map),
                                      /* Access attempt terminates due to
                                      ** max access probes */

  #ifndef FEATURE_JCDMA

                                      /* 68 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_acc_term_acc_fail),
                                      /* Access attempt terminates due to
                                      ** access fail */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  #endif
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 69 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_cam_err_emerg_cb),
                                      /* MS receives a channel assignment
                                      ** that is not supported by the MS
                                      ** while in emergency callback mode */

  SD_ENUM_ELEMENT(ssscr_cdma_opr_cam_err),
                                      /* MS receives a channel assignment
                                      ** that is not supported by the MS */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  SD_ENUM_ELEMENT(ssscr_cdma_opr_cam),
                                      /* MS receives a channel assignment
                                      ** that is supported by the MS */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_hdm),
                                      /* MS receives a handoff direction
                                      ** message */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      /* 73 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_itspm),
                                      /* MS receives an in traffic system
                                      ** parameters message with new SID/NID
                                      ** information */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  #if defined(FEATURE_LTE_TO_1X)
  SD_ENUM_ELEMENT (ssscr_cdma_opr_release_csfb_call),
                                       /* 1XCSFB call is released */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  #endif
                                       /*75*/
  SD_ENUM_ELEMENT(ssscr_cdma_opr_release_redir),
                                      /* Call is released and redirection is
                                      ** on */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_release),
                                      /* Call is released */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_cdma_opr_prot_mis),
                                      /* CDMA protocol mismatch */

                                      /* 78 */
  SD_ENUM_ELEMENT(ssscr_cdma_opr_operator_capacity_changed)
                        /* CDMA operator capacity changed */
};
#endif /* defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900) */



#ifdef FEATURE_HDR

/*---------------------------------------------------------------------------
                        HDR User Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with HDR
** acquisition events.
*/
/*lint -esym(551, dsssscr_hdr_user_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_hdr_user_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_opr_orig_end),
                                      /* User changes the SS-Preference while
                                      ** in operation state */


  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_opti_redial_new),
				      /* optimized redial starts while HDR
				      ** is not in service */

		
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_redial_start),
                                      /* optimized redial starts and redial
                                      ** list is empty */

                                      /* 3 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_redial),
                                      /* optimized redial ongoing and redial
                                      ** list is not empty */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_opr_redir_mode_conf),
                                     /* User changed preferences while in operation
                                                              state but there is mode pref conflict */
                                      
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_opr),
                                      /* User changes the SS-Preference while
                                      ** in operation state */


  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_acq_orig),
                                      /* User changes the SS-Preference while
                                      ** in acquisition to originate a normal
                                      ** call */

                                      /* 7 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_opt_ho_orig),
                        /* Script for when user changes the SS-Preference while
                                      ** optimized HO LTE->ehrpd is in progress to originate a hdr
                                      ** data call
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_colloc_pwr_save_wakeup_acq_micro),
                                      /* User changes the SS-Preference while
                                      ** in collocated power save and power
                                      ** save timeline is currently doing
                                      ** wakeup acquisition), and main is acquired
                                      ** for less than 10 seconds
                                      */



  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_colloc_pwr_save_wakeup_acq_rotate),
                                      /* User changes the SS-Preference while
                                      ** in collocated power save and power
                                      ** save timeline is currently doing
                                      ** wakeup acquisition.and main is acquired
                                      ** for more than 10 seconds
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_colloc_pwr_save_micro),
                                      /* User changes the SS-Preference while
                                      ** in collocated power save and power
                                      ** save timeline is currently in
                                      ** sleep period), and main is acquired
                                      ** for less than 10 seconds
                                      */

                                        /* 11 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_colloc_pwr_save_rotate),
                                      /* User changes the SS-Preference while
                                      ** in collocated power save and power
                                      ** save timeline is currently in
                                      ** sleep period), and main is acquired
                                      ** for more than 10 seconds
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_colloc_pwr_save_call_end),
                                      /* User changes the SS-Preference while
                                      ** in collocated power save and power
                                      ** save timeline is currently in
                                      ** sleep period), and main is acquired
                                      ** for more than 10 seconds
                                      */

  #ifdef FEATURE_HICPS_STACK_SYNC_UP
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_sync_up_pwr_save_wakeup_acq),
                                      /* User changes the SS-Preference), for),
                                      ** stack sync-up), while in collocated
                                      ** power save and power save timeline
                                      ** is currently in wakeup acquisitions.
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_sync_up_pwr_save),
                                      /* User changes the SS-Preference), for),
                                      ** stack sync-up), while in collocated
                                      ** power save and power save timeline
                                      ** is currently in sleep period.
                                      */

                                      /* 15 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_sync_up_acq),
                                      /* User changes the SS-Preference), for),
                                      ** stack sync-up), while in collocated
                                      ** acquisitions.
                                      */
  #else
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved script */
  #endif /* FEATURE_HICPS_STACK_SYNC_UP */

  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),              /* Reserved script */


  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_hdr_vrfy_redir),
                                      /* User changes ss pref during redir while
                                      ** HDR is in verify state
                                      */
                                      /* 19 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_redir),       /* User pref change during redir */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_stack_syncup_bsr),

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref_ue_mode_chgd),
                                      /* User changes the SS-Preference */

  SD_ENUM_ELEMENT(ssscr_hdr_user_ss_pref),
                                      /* User changes the SS-Preference */

                                      /* 23 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_hybr_start_ue_mode_chgd),
                                      /* User starts hybrid operation */

  SD_ENUM_ELEMENT(ssscr_hdr_user_hybr_start),
                                      /* User starts hybrid operation */

                                      /* 25 */
  SD_ENUM_ELEMENT(ssscr_hdr_user_hybr_end),
                                      /* User ends hybrid operation */

  SD_ENUM_ELEMENT(ssscr_hdr_user_redir_lte)
                                      /* MS receives an LTE-HDR redirection
                                      ** message
                                      */
};

/*---------------------------------------------------------------------------
                     HDR Acquisition Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with HDR
** acquisition events.
*/
/*lint -esym(551, dsssscr_hdr_acq_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_hdr_acq_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_hdr_acq_failed_pilot_reacq),
                                      /* HDR acquisition fails due to failure
                                      ** to acquire the pilot channel while
                                      ** attempting to reacquire the
                                      ** candidate system */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  SD_ENUM_ELEMENT(ssscr_hdr_acq_failed_sync),
                                      /* HDR acquisition fails due to failure
                                      ** to receive the sync channel message
                                      ** and the system that is being
                                      ** attempted acquisition is not already
                                      ** on a reacquisition schedule */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_acq_prot_mis),
                                      /* HDR acquisition fails due to
                                      ** protocol mismatch (i.e., the access
                                      ** terminal's revision number is not in
                                      ** the range defined by the
                                      ** MinimumRevision and MaximumRevision
                                      ** (inclusive) fields of the sync
                                      ** message */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      /* 3 */
  SD_ENUM_ELEMENT(ssscr_hdr_acq_bad_sys),
                                      /* HDR system is reported to be bad.
                                      */
};

/*---------------------------------------------------------------------------
                      HDR Operation Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with HDR
** operation events.
*/
/*lint -esym(551, dsssscr_hdr_opr_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_hdr_opr_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_new_redir),
                                      /* A new CDMA system is fully acquired
                                      ** and the SID/NID is new and
                                      ** redirection indication is on */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_conf_vrfy),
                                      /* A new HDR system is fully acquired
                                      ** and this system conflicts with the
                                      ** current SS-Preference and the
                                      ** SS-State is verify */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_frbd_vrfy),
                                      /* A new HDR system is fully acquired
                                      ** and this system is forbidden and
                                      ** origination mode is other than
                                      ** emergency and the SS-State is
                                      ** verify */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_not_colloc_vrfy),
                                      /* A new HDR system is fully acquired
                                      ** and this system is not collocated
                                      ** with the IDL_SYS of the main SD
                                      ** instance and the SS-State is
                                      ** verify */

                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_conf),
                                      /* A new HDR system is fully acquired
                                      ** and this system conflicts with the
                                      ** current SS-Preference */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_frbd),
                                      /* A new HDR system is fully acquired
                                      ** and this system is forbidden and
                                      ** origination mode is other than
                                      ** emergency */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_not_colloc),
                                      /* A new HDR system is fully acquired
                                      ** and this system is not collocated
                                      ** with the IDL_SYS of the main SD
                                      ** instance */
//  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_more_pref_colloc_redir),
//                                      /* A new redirected HDR serving system
//                                      ** is fully acquired), but there are
//                                      ** more preferred systems in the
//                                      ** serving system's GEO and the orig
//                                      ** mode is collocated system. */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_more_pref_colloc),
                                      /* A new HDR serving system is fully
                                      ** acquired), but there are more
                                      ** preferred systems in the serving
                                      ** system's GEO and the orig mode is
                                      ** collocated system. */

                                      /* 8 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid_more_pref),
                                      /* A new HDR serving system is fully
                                      ** acquired), but there are more
                                      ** preferred systems in the serving
                                      ** system's GEO */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_oh_sid),
                                      /* An HDR system is a new SID is fully
                                      ** acquired */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_redir),
                                      /* MS receives an HDR redirection
                                      ** message */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_vrfy_sched),
                                      /* HDR serving system is lost during
                                      ** verification and the lost system is
                                      ** not already on a reacquisition
                                      ** schedule */

                                      /* 12 */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_vrfy),
                                      /* HDR serving system is lost during
                                      ** the VERIFY SS-State */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_lte_irat_fail),
                                      /* HDR to LTE reselection failed but
                                      ** HDR serving system was not lost
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_skip_colloc),
                                      /* HDR serving system is lost and the
                                      ** origination mode is collocated
                                      ** system */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_colloc),
                                      /* HDR serving system is lost and the
                                      ** origination mode is collocated
                                      ** system */

                                      /* 16 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost_orig),
                                      /* HDR serving system is lost while
                                      ** user try to originate a call */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_sys_lost),
                                      /* HDR serving system is lost */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_acc_fail_colloc),
                                      /* Script for when HDR system access
                                      ** attempt fails for at least once and
                                      ** is collocated */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_acc_den),
                                      /* Script for when HDR system access
                                      ** is denied and origination mode
                                      ** is collocated */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_con_den_general),
                                      /* Script for when HDR system access
                                      ** is denied  reason : general
                                      */

                                      /* 21 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_con_den_net_busy),
                                      /* Script for when HDR system access
                                      ** is denied  reason : network busy
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_con_den_auth),
                                      /* Script for when HDR system access
                                      ** is denied  reason: auth failure
                                      */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_acc_timeout),
                                      /* Script for when HDR system access
                                      ** attempt timeout i.e. no TCA or NO
                                      ** RTCACK */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_acc_fail_prl),
                                      /* Script for when HDR system access
                                      ** attempt fails and idle system is
                                      ** from the PRL */

                                      /* 25 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_acc_fail),
                                      /* Script for when HDR system access
                                      ** attempt fails */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_abort),
                                      /* MS does not receive an OpenResponse
                                      ** message within T-SBPANResponse
                                      ** seconds after sending the
                                      ** OpenRequest message */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_bad_sys),
                                      /* HDR system is reported to be bad.
                                      */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_release),
                                      /* HDR exit the traffic channel */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 29 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_nego_timeout_7),
                                      /* Session negotiation timed out at
                                      ** least 7 times. */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_nego_timeout_4),
                                      /* Session negotiation timed out at
                                      ** least 4 times. */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_nego_timeout_3),
                                      /* Session negotiation timed out at
                                      ** least 3 times. */

  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_nego_timeout_2),
                                      /* Session negotiation timed out at
                                      ** least 2 times. */

                                      /* 36 */
  SD_ENUM_ELEMENT(ssscr_hdr_opr_ses_nego_timeout_1),
                                      /* Session negotiation timed out at
                                      ** least 1 times. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_end_bcmcs),
                                      /* HDR BCMCS flow monitoring is ended
                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_hdr_opr_prot_mis),
                                      /* MOB_P_REV < MIN_P_REV or MS does
                                      ** not support the Paging Channel*/
};

#endif /* FEATURE_HDR */

#if defined(FEATURE_GSM) || defined(FEATURE_WCDMA) || defined(FEATURE_SD_LTE)

/*---------------------------------------------------------------------------
                      GSM/WCDMA Operation Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with HDR
** operation events.
*/
/*lint -esym(551, dsssscr_gw_opr_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_gw_opr_event_tbl) {

                                     /* 0 */
  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_emerg_attached),
                                     /* Received srv info from NAS 
                                     ** when it is in emerg attached*/

  
  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_frbd_vrfy),
                                     /* Change in GSM/WCDMA service system),
                                     ** the acquired system is forbidden
                                     ** plmn system and system selection
                                     ** state is in verify state. */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_frbd),
                                     /* Change in GSM/WCDMA service system),
                                     ** the acquired system is forbidden
                                     ** plmn system and system selection
                                     ** state is in operation. */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_pref_conf),
                                      /* Change in GSM/WCDMA serving system
                                      ** and new system conflicts with the
                                      ** current SS-Preference */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_srv_conf_gw_no_1x),
                                      /* For GW only target),
                                      ** Change in GSM/WCDMA serving system
                                      ** and new system conflicts with the
                                      ** current SS-Preference */

                                        /* 5 */
  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_srv_conf),
                                      /* Change in GSM/WCDMA serving system
                                      ** and new system conflicts with
                                      ** requested service */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_lmtd_invalid_camped),
                                      /* We have limited service because we
                                      ** are in CAMPED_ONLY mode and
                                      ** SIM is invalid for all selected domains */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_exit_lmtd_camp),
                                      /* If UE is not emergency attached state
                                      ** and NAS is in pure limited mode, exit pure limited mode
                                      ** state upon getting limited service notification */


  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_lmtd_camped),
                                      /* We have limited service because we
                                      ** are in CAMPED_ONLY mode and there are
                                      ** more preferred systems available. */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_lmtd_invalid),
                                      /* We have limited service because we
                                      ** were not able to acquire full service and
                                      ** SIM is invalid for all selected domains */

                                      /* 10 */
  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_lmtd),
                                      /* We have limited service because we
                                      ** were not able to acquire full
                                      ** service */

  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_more_preferred_hdr_scan),


  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info_more_preferred),
                                      /* We have full service and
                                      ** there are more preferred systems
                                      ** in PRL */

  SD_ENUM_ELEMENT(ssscr_hybr_2_gw_opr_srv_info_pref_pending),
                                      /* Change in GSM/WCDMA serving system
                                                       ** when pref change are pending to force 
                                      */
                                      
  SD_ENUM_ELEMENT(ssscr_gw_opr_srv_info),
                                      /* Change in GSM/WCDMA serving system.
                                      */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 15 */
  SD_ENUM_ELEMENT(ssscr_gw_opr_acc_done),
                                      /* GSM/WCDMA call was over.
                                      */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//  SD_ENUM_ELEMENT(ssscr_gw_opr_sys_lost_orig_gw),
//                                     /* Serving system was lost and orig
//                                     ** mode is not manual nor none for
//                                     ** GW only target.
//                                     */
//
//  SD_ENUM_ELEMENT(ssscr_gw_opr_sys_lost_gw),
//                                     /* Serving system was lost for GW only
//                                     ** target.
//                                     */
//
  SD_ENUM_ELEMENT(ssscr_gw_opr_sys_lost),
                                      /* Serving system was lost
                                      */

  SD_ENUM_ELEMENT(ssscr_gw_opr_operator_capability_changed)
                  /* Serving System Operator Capability changed*/       
};

#endif /* defined(FEATURE_GSM) || defined(FEATURE_WCDMA) */

/*---------------------------------------------------------------------------
                      Miscellaneous Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with
** miscellaneous events.
*/
/*lint -esym(551, dsssscr_misc_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_misc_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_misc_orig_success),
                                  /* Origination is successful */

  SD_ENUM_ELEMENT(ssscr_misc_irat_to_gwl),
                                  /* reselection to 3GPP from 3GPP2*/

  SD_ENUM_ELEMENT(ssscr_misc_local_deactivate),
                                  /* locally deactivate a particular stack */

  SD_ENUM_ELEMENT(ssscr_misc_local_activate),
                                  /* locally activate a particular stack */

  SD_ENUM_ELEMENT(ssscr_misc_gw_sim_state_update_pwr_save_gw),
                                  /* SIM_STATE_UPDATE_IND received from REG
                                  ** during power save*/

  SD_ENUM_ELEMENT(ssscr_misc_gw_sim_state_update),
                                 /* SIM_STATE_UPDATE_IND received from REG */								  
  SD_ENUM_ELEMENT(ssscr_misc_lte_to_hdr_bsr)
                                 /* Immediate BSR to HDR start */

};

/*---------------------------------------------------------------------------
                       Internal Events Script Table
---------------------------------------------------------------------------*/

/* This table enables associating unique System Selection Scripts with
** internally generated events.
*/
/*lint -esym(551, dsssscr_int_event_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_int_event_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_rlf_during_emerg),

                                      /* 1 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_lte_conn_mode_manual),

                                      /* 2 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_lte_conn_mode),
  
  #if defined ( FEATURE_WRLF_SYSTEM_SEL)
                                      /* 3 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_wcdma_conn_mode_manual),

                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_wcdma_conn_mode),
  
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  
  #endif /* FEATURE_WRLF_SYSTEM_SEL */

                                     /* 5 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_manual_emerge_call_orig),
                                      /* service is lost and orig mode is
                                      ** manual and during the call.
                                      */

                                    /* 6 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_manual_call_orig),
                                      /* service is lost and orig mode is
                                      ** manual and during the call.
                                      */

                                    /* 7 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_manual),
                                      /* Service is declared lost and orig
                                      ** mode is manual
                                      */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                    /* 8 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_mode_pref_none),
                                      /* Service is declared lost and 
                                      ** mode pref is NONE
                                      */
                                      
                                    /* 9 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_call_orig_gw_pwrup),
                                      /* Service is declared lost and orig
                                      ** mode is emerg and AT is in a call orig
                                      */

                                     /* 10 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_orig_gw_fullrat),
                                          /* Internally generated SS-Event),
                                          ** indicating that GW service is
                                          ** declared lost and PM has forced fullrat
                                          ** idle system is not
                                          ** empty and this is GW only target.*/

                                    /* 11 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_orig_gw_pwrup),
                                      /* Service is declared lost and orig
                                      ** mode is emerg
                                      */
                                      
                                    /* 12 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_hybr2_bsr),
                                        /* Service is declared lost and
                                        ** BSR should happen on hybr2 stack
                                        */

                                    /* 13 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_orig_gw_pwrup),
                                      /* Service is declared lost while
                                      ** trying to originate a call and idle
                                      ** system is empty.
                                      ** And the target capability is GW only
                                      ** system. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                    /* 14 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_call_orig_gw),
                                      /* Service is declared lost while
                                      ** trying to originate a E911 call and idle
                                      ** system is empty.
                                      ** And the target capability is GW only
                                      ** system. */



                                    /* 15 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_orig_gw),
                                      /* Service is declared lost while
                                      ** mode is emerg and idle
                                      ** system is empty.
                                      ** And the target capability is GW only
                                      ** system. */

                                    /* 16 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_orig_gw),
                                      /* Service is declared lost while
                                      ** trying to originate a call and idle
                                      ** system is not empty.
                                      ** And the target capability is GW only
                                      ** system. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
                                    /* 17 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_hdr_cdma_ho),
                                      /* Service is declared lost while
                                      ** handing off the call to 1x */
                                      
                                    /* 18 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_normal_orig),
                                      /* Service is declared lost while
                                      ** trying to originate a normal call */
                                      
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


                                    /* 19 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_hdr_redial_start),
                                      /* service is lost during optimized 
                                      ** silent redial and redial
                                      ** list is empty */

                                    /* 20 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_hdr_redial),
                                      /* service is lost during optimized 
                                      ** silent redial and redial
                                      ** list is updated */
  
                                    /* 21 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_redial_start_hdr_only),
                                      /* In HDR only mode, service is lost  
                                      ** during optimized silent redial and 
                                      ** redial list is empty */

                                    /* 22 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_redial_hdr_only),
                                      /* In HDR only mode, service is lost  
                                      ** during optimized silent redial and 
                                      ** redial list is updated */
  
                                    /* 23 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_colloc_call_orig),
                                      /* Service is declared lost while
                                      ** origination mode is collocated
                                      ** system and during a call */

  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  SD_ENUM_ELEMENT(ssscr_reserved_script),
  

                                      /* 28 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_colloc_orig),
                                      /* Service is declared lost while
                                      ** origination mode is collocated
                                      ** system acquisition */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 29 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_otasp_orig),
                                      /* Service is declared lost while
                                      ** trying to originate an OTASP call */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
                                      /* 30 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_orig_colloc),
                                      /* Service is declared lost while
                                      ** trying to originate an emergency
                                      ** call on hybrid hdr*/

                                      /* 31 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_orig),
                                      /* Service is declared lost while
                                      ** trying to originate an emergency
                                      ** call on main stack*/


                                      /* 32 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_cdma_emerg_cb),
                                      /* Service is declared lost while in
                                      ** emergency callback mode on a 1x system */
 
                                      /* 33 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_emerg_cb),
                                      /* Service is declared lost while in
                                      ** emergency callback mode */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 34 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_gw_pwrup),
                                      /* Internally generated SS-Event),
                                      ** indicating that GW service is
                                      ** declared lost and idle system is
                                      ** empty and this is GW only target.*/

                                      /* 35 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost_gw),
                                      /* Internally generated SS-Event),
                                      ** indicating that GW service is
                                      ** declared lost and idle system is not
                                      ** empty and this is GW only target.*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  
                                      /* 36 */
  SD_ENUM_ELEMENT(ssscr_int_srv_lost),
                                      /* Internally generated SS-Event),
                                      ** indicating that service is declared
                                      ** lost */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
//  SD_ENUM_ELEMENT(ssscr_int_resel_colloc_redir),
//                                      /* Script for starting the reselection
//                                      ** on a collocated system after
//                                      ** receiving the redirection message.*/
//
//  SD_ENUM_ELEMENT(ssscr_int_resel_colloc_release),
//                                      /* Script for starting the reselection
//                                      ** on a collocated system after
//                                      ** receiving the release message. */
//
//  SD_ENUM_ELEMENT(ssscr_int_resel_colloc_new_sid),
//                                      /* Script for starting the reselection
//                                      ** on a collocated system when the
//                                      ** subnet ID of the new serving
//                                      ** system is different from the
//                                      ** previous subnet ID. */
//
//
//  SD_ENUM_ELEMENT(ssscr_int_resel_colloc_ss_pref),
//                                      /* Script for starting the reselection
//                                      ** on a collocated system when there is
//                                      ** a SS PREF change. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 37 */
  SD_ENUM_ELEMENT(ssscr_int_resel_colloc),
                                      /* Script for starting the reselection
                                      ** on a collocated system */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 38 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_none),
                                      /* Script for starting the full service
                                      ** reselection process.
                                      */

                                      /* 39 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_invalid),
                                      /* Script for handling reselection timer 
                                      ** expiry when sim domain is invalid for
				      ** the stack.
                                      */

                                      /* 40 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_srch_manual),
                                      /* Script for doing the full service
                                      ** reselection when in manual plmn
                                      ** selection.
                                      */

                                      /* 41 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_srch_gw_no_1x),
                                      /* GW only target.
                                      ** Script for doing the full service
                                      ** reselection when not in manual plmn
                                      ** selection.
                                      */

                                      /* 42 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_srch),
                                      /* Script for doing the full service
                                      ** reselection when not in manual plmn
                                      ** selection.
                                      */

                                      /* 43 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_release),
                                      /* Script for doing reselection just
                                      ** after releasing the GSM/WCDMA call.
                                      */

                                      /* 44 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_ss_pref),
                                      /* Script for doing reselection just after
                                      ** ss-pref change.
                                      */

                                      /* 45 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_reacq),
                                      /* Script for reacquiring the limited
                                      ** service after reselection for full
                                      ** service has failed.
                                      */

                                      /* 46 */
  SD_ENUM_ELEMENT(ssscr_int_resel_lmtd_wait),
                                      /* Script for sleep interval during the
                                      ** full service reselection process 
                                      */

                                      /* 47 */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */

                                      /* 48 */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
 
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */

                                      /* 50 */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */

  #if (defined FEATURE_MMODE_SC_SVLTE)
                                      /* 51 */
  SD_ENUM_ELEMENT(ssscr_int_resel_hybr_bsr_to_none),
                                      /* Script for reselecting Main service
                                      ** after Hybr (DO) service is acquired
                                      ** and there are more pref systems on
                                      ** Main (GWL)
                                      */
                                      /* 52 */
  SD_ENUM_ELEMENT(ssscr_int_resel_hybr_bsr_to_full),
                                      /* Script for reselecting full service
                                      ** after resel timer at a given stage
                                      ** expires and an attempt to acquire
                                      ** in HYBR stack is made
                                      */
                                      /* 53 */
  SD_ENUM_ELEMENT(ssscr_int_resel_hybr_bsr_to_hdr_end),
                                        /* Script for LTE to HDR BSR end */

                                      /* 54 */
  SD_ENUM_ELEMENT(ssscr_int_resel_more_preferred_hdr_scan),
                                          /* Script for starting the reselection
                                          ** to more preferred hdr systems */
                                      
  #else
  
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),    /* Reserved script */
  
  #endif /* FEATURE_MMODE_SC_SVLTE */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                      /* 55 */
  SD_ENUM_ELEMENT(ssscr_int_resel_jcdma_idle),
                                      /* JCDMA reselection when idle */

                                      /* 56 */
  SD_ENUM_ELEMENT(ssscr_int_resel_jcdma_call_end),
                                      /* JCDMA reselection after
                                      ** SS-Preference change or call release
                                      */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
                                      /* 57 */
  SD_ENUM_ELEMENT(ssscr_int_resel_none),
                                      /* Starting reselection */

                                      /* 58 */
  SD_ENUM_ELEMENT(ssscr_int_resel_is_absr),
                                      /* Starting reselection with ABSR */

                                      /* 59 */
  SD_ENUM_ELEMENT(ssscr_int_resel_absr),
                                      /* Reselection for a avoided system. */

                                      /* 60 */
  SD_ENUM_ELEMENT(ssscr_int_resel_wait_absr),
                                      /* Wait for ABSR timer expired */

                                      /* 61 */
  SD_ENUM_ELEMENT(ssscr_int_resel_srch),
                                      /* Reselection after a new system is
                                      ** acquired */

                                      /* 62 */
  SD_ENUM_ELEMENT(ssscr_int_resel_reacq_sync_up_bsr),
                                      /* When we are doing BSR Only dont reacquire
                                      ** just continue with the BSR algorithm
                                      */

                                      /* 63 */
  SD_ENUM_ELEMENT(ssscr_int_resel_reacq),
                                      /* Reacquiring the serving system after
                                      ** reselection period is over */

                                      /* 64 */
  SD_ENUM_ELEMENT(ssscr_int_resel_init_to_norm),
                                      /* Detecting that initial reselection
                                      ** mode is over and switching to normal
                                      ** reselection mode */

                                      /* 65 */
  SD_ENUM_ELEMENT(ssscr_int_resel_init_ss_pref),
                                      /* Initial reselection after
                                      ** SS-Preference change */

                                      /* 66 */
  SD_ENUM_ELEMENT(ssscr_int_resel_init_release),
                                      /* Initial reselection after call
                                      ** release */

                                      /* 67 */
  SD_ENUM_ELEMENT(ssscr_int_resel_norm_ss_pref),
                                      /* Normal reselection after
                                      ** SS-Preference change */

                                      /* 68 */
  SD_ENUM_ELEMENT(ssscr_int_resel_norm_release),
                                      /* Normal reselection after call
                                      ** release */

                                      /* 69 */
  SD_ENUM_ELEMENT(ssscr_int_resel_stage1_end),
                                      /* Normal reslection iteration 1
                                      ** completed. Switching to
                                      ** iteration 2
                                      */

                                      /* 70 */
  SD_ENUM_ELEMENT(ssscr_int_resel_stage2_end),
                                      /* Normal reslection iteration 2
                                      ** completed. Switching to
                                      ** iteration 3
                                      */

                                      /* 71 */
  SD_ENUM_ELEMENT(ssscr_int_resel_stage3_end),
                                      /* Normal reslection iteration 3
                                      ** completed. Stay in this mode
                                      ** till reselection end.
                                      */

                                      /* 72 */
  SD_ENUM_ELEMENT(ssscr_int_resel_stage_any_pause),
                                      /* Normal reselection iteration
                                      ** incomplete. SRCH_LST not
                                      ** exhausted
                                      */

                                      /* 73 */
  SD_ENUM_ELEMENT(ssscr_int_resel_norm_absr),  /* Normal reselection */

                                      /* 74 */
  SD_ENUM_ELEMENT(ssscr_int_resel_norm),       /* Normal reselection start */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_reserved_script),       /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),       /* Reserved script */
  SD_ENUM_ELEMENT(ssscr_reserved_script),       /* Reserved script */
};

/* <EJECT> */
/*lint +e708 */ /* Info 708 union initialization */
/*---------------------------------------------------------------------------
                            Called Scripts Table
---------------------------------------------------------------------------*/

/* This table enables SS-Script to activate other scripts.
**
** Note that this table is implemented as an array that is being indexed by
** sdssscr_e_type enum values. In other words, the
** dsssscr_called_tbl[ SDSSSCR_XXX ] entry reference the SS-Script that is
** associated with the SDSSSCR_XXX enum value.
*/
/*lint -esym(551, dsssscr_called_tbl_string_map) */
SD_BEGIN_SCRIPT(dsssscr_called_tbl) {

                                      /* 0 */
  SD_ENUM_ELEMENT(ssscr_called_clr_acq_lsts),          /* SDSSSCR_CLR_ACQ_LSTS */
  SD_ENUM_ELEMENT(ssscr_called_clr_acq_sched_lsts),    /* SDSSSCR_CLR_ACQ_SCHED_LSTS */
  SD_ENUM_ELEMENT(ssscr_called_clr_avoid_lsts),        /* SDSSSCR_CLR_AVOID_LSTS */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_called_srv_lost_norm),         /* SDSSSCR_SRV_LOST_NORM */

                                      /* 4 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_norm_slnt),    /* SDSSSCR_SRV_LOST_NORM_SLNT */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_norm_srv_off_until_oh),

                                      /* 6 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_skip),         /* SDSSSCR_SRV_LOST_SKIP */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_skip_silent),  /* SDSSSCR_SRV_LOST_SKIP_SLNT */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_skip_acq),     /* SDSSSCR_SRV_LOST_SKIP_ACQ */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_skip_idl),     /* SDSSSCR_SRV_LOST_SKIP_IDL */

                                      /* 10 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_skip_srv_off_until_oh),
                                /* SDSSSCR_SRV_LOST_SKIP_SRV_OFF_UNTIL_OH */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_called_sys_acc_fail_prl),      /* SDSSSCR_SYS_ACC_FAIL_PRL */
  SD_ENUM_ELEMENT(ssscr_called_sys_acc_fail_avail),    /* SDSSSCR_SYS_ACC_FAIL_AVAIL */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SD_ENUM_ELEMENT(ssscr_called_resel_jcdma),           /* SDSSSCR_RESEL_JCDMA */
  SD_ENUM_ELEMENT(ssscr_called_resel_dm),              /* SDSSSCR_RESEL_DM */

  SD_ENUM_ELEMENT(ssscr_called_resel_none),            /* SDSSSCR_RESEL_NONE */
                                      /* 16 */
  SD_ENUM_ELEMENT(ssscr_called_resel_absr),            /* SDSSSCR_RESEL_ABSR */
  SD_ENUM_ELEMENT(ssscr_called_resel_init),            /* SDSSSCR_RESEL_INIT */
  SD_ENUM_ELEMENT(ssscr_called_resel_init_to_norm),    /* SDSSCR_RESEL_INIT_TO_NORM */

  SD_ENUM_ELEMENT(ssscr_called_resel_norm),            /* SDSSSCR_RESEL_NORM */
                                      /* 20 */
  SD_ENUM_ELEMENT(ssscr_called_resel_full_srv),        /* SDSSSCR_RESEL_FULL_SRV */
  SD_ENUM_ELEMENT(ssscr_called_resel_hybr),            /* SDSSSCR_RESEL_HYBR */
  SD_ENUM_ELEMENT(ssscr_called_resel_gw_full_srv),     /* SDSSSCR_RESEL_GW_FULL_SRV */

  SD_ENUM_ELEMENT(ssscr_called_resel_full_srv_manual), /* SDSSSCR_RESEL_FULL_SRV_MANUAL */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                      /* 24 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save),              /* SDSSSCR_PWR_SAVE */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_jcdma),        /* SDSSSCR_PWR_SAVE_JCDMA */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_colloc_1sys),  /* SDSSSCR_PWR_SAVE_COLLOC_1SYS */
                                     /* 27 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_colloc),       /* SDSSSCR_PWR_SAVE_COLLOC */
                                     /* 28 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_colloc_deep),  /* SDSSSCR_PWR_SAVE_COLLOC_DEEP */

  SD_ENUM_ELEMENT(ssscr_called_pwr_save_colloc_shallow),/*SDSSSCR_PWR_SAVE_COLLOC_SHALLOW */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_rx_gt_99),     /* SDSSSCR_PWR_SAVE_RX_GT_99 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_jcdma_1_2_b),  /* SDSSSCR_PWR_SAVE_JCDMA_1_2_B */
                                    /* 32 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_jcdma_1_3_b),  /* SDSSSCR_PWR_SAVE_JCDMA_1_3_B */

  SD_ENUM_ELEMENT(ssscr_called_pwr_save_overreach_2_2),/* SDSSSCR_PWR_SAVE_OVERREACH_2_2 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_overreach_2_3),/* SDSSSCR_PWR_SAVE_OVERREACH_2_3 */

  SD_ENUM_ELEMENT(ssscr_called_pwr_save_gw_pwrup),     /* SDSSSCR_PWR_SAVE_GW_PWRUP */
                                    /* 36 */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_gw),           /* SDSSSCR_PWR_SAVE_GW */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_emerg_call_orig_pwrup),
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_orig_pwrup), /* SDSSSCR_SRV_LOST_GW_ORIG_PWRUP */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_emerg_call_orig),
                                      /* SDSSSCR_SRV_LOST_EMERG_CALL_GW_ORIG */
                                    /* 40 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_orig),      /* SDSSSCR_SRV_LOST_GW_ORIG */

  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_pwrup),     /* SDSSSCR_SRV_LOST_GW_PWRUP */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw),           /* SDSSSCR_SRV_LOST_GW */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  SD_ENUM_ELEMENT(ssscr_called_hdr_acq_rotate),        /* SDSSSCR_HDR_ACQ_ROTATE */
  SD_ENUM_ELEMENT(ssscr_called_hdr_acq_rotate_srv_lost),
                                      /* SDSSSCR_HDR_ACQ_ROTATE_SRV_LOST */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                   /* 45*/
  SD_ENUM_ELEMENT(ssscr_called_cdma_acq_micro),        /* SDSSSCR_CDMA_ACQ_MICRO */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  SD_ENUM_ELEMENT(ssscr_called_reserved),
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                    /* 47 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_hdr_cdma_ho),  /* SDSSSCR_SRV_LOST_HDR_CDMA_HO */

  SD_ENUM_ELEMENT(ssscr_called_chk_and_clr_acq_sys),   /* SDSSSCR_CHK_AND_CLR_ACQ_SYS */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                  /* 49 */
  SD_ENUM_ELEMENT(ssscr_called_reserved),
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
                                 /* 53 */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */

  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
                                      /* 66 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_lte_conn),     /* SDSSSCR_SRV_LOST_LTE_CONN */
  #ifdef FEATURE_WRLF_SYSTEM_SEL
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_wcdma_conn_stage1), /* SDSSSCR_SRV_LOST_WCDMA_CONN_STAGE1 */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_wcdma_conn_stage2), /* SDSSSCR_SRV_LOST_WCDMA_CONN_STAGE2 */
  #else
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  #endif /* FEATURE_WRLF_SYSTEM_SEL */

                                      /* 69 */
  SD_ENUM_ELEMENT(ssscr_called_acq_gw_ltd),            /* SDSSSCR_ACQ_GW_LTD */

  SD_ENUM_ELEMENT(ssscr_called_srv_lost_emerg_mode),   /* SDSSSCR_SRV_LOST_EMERG_MODE */

  SD_ENUM_ELEMENT(ssscr_called_resel_lmtd_to_hybr_bsr),/* SDSSSCR_RESEL_LMTD_TO_HYBR_BSR */

  SD_ENUM_ELEMENT(ssscr_called_resel_lmtd_to_full),    /* SDSSSCR_RESEL_LMTD_TO_FULL */

                                      /* 73 */
  SD_ENUM_ELEMENT(ssscr_called_resel_norm_scan),       /* SDSSSCR_RESEL_NORM_SCAN */
  SD_ENUM_ELEMENT(ssscr_called_scan_map_systems),      /* SDSSSCR_SCAN_MAP_SYSTEMS*/
  SD_ENUM_ELEMENT(ssscr_called_bsr_start),             /* SSSCR_CALLED_BSR_START */
  SD_ENUM_ELEMENT(ssscr_called_loc_bsr),               /* SDSSSCR_LOC_BSR */
  SD_ENUM_ELEMENT(ssscr_called_pwr_save_bsr),          /* SDSSSCR_CALLED_PWR_SAVE_BSR */

                                      /* 78 */
  SD_ENUM_ELEMENT(ssscr_called_get_net_bplmn),         /* SDSSSCR_CALLED_GET_NET */
  SD_ENUM_ELEMENT(ssscr_called_user_get_net),                /* SDSSSCR_USER_GET_NET */


  SD_ENUM_ELEMENT(ssscr_called_srv_lost_gw_orig_no_pref_chgd),              
                                                 /* SDSSSCR_CALLED_SRV_LOST_GW_ORIG_NO_PREF_CHGD */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */

                                      /* 83 */
  SD_ENUM_ELEMENT(ssscr_called_gwl_opr_sys_lost_dsds), /* SDSSSCR_CALLED_GWL_OPR_SYS_LOST_DSDS */

  #ifdef FEATURE_LIMITED_SCAN_OPT
  SD_ENUM_ELEMENT(ssscr_called_gwl_no_srv_lmtd_sys_scan), /* SDSSSCR_GWL_NO_SRV_LMTD_SYS_SCAN */

  SD_ENUM_ELEMENT(ssscr_called_gwl_no_srv_lmtd_scan), /* SDSSSCR_GWL_NO_SRV_LMTD_SCAN */

  SD_ENUM_ELEMENT(ssscr_called_gwl_no_srv_lmtd_true_scan), /* SDSSSCR_GWL_NO_SRV_LMTD_TRUE_SCAN */
  #else
  SD_ENUM_ELEMENT(ssscr_called_reserved),
  SD_ENUM_ELEMENT(ssscr_called_reserved),
  SD_ENUM_ELEMENT(ssscr_called_reserved),
  #endif

                                      /* 87 */
  SD_ENUM_ELEMENT(ssscr_called_operator_capability_changed),       /* SDSSSCR_OPERATOR_CAPABILITY_CHANGED */

  SD_ENUM_ELEMENT(ssscr_called_acq_db_scan_during_sleep_time),     /* SDSSSCR_ACQ_DB_SCAN_DURING_SLEEP_TIME */
  SD_ENUM_ELEMENT(ssscr_called_reserved),  
  SD_ENUM_ELEMENT(ssscr_called_reserved),        
                                      /* 91 */
  SD_ENUM_ELEMENT(ssscr_called_reserved),   
  SD_ENUM_ELEMENT(ssscr_called_more_preferred_do_scan),         /* SDSSSCR_MORE_PREFERRED_DO_SCAN*/
  SD_ENUM_ELEMENT(ssscr_called_non_sv_acq_db_scan_mru),         /* SDSSSCR_NON_SV_ACQ_DB_SCAN_MRU*/
  SD_ENUM_ELEMENT(ssscr_called_csg_app_service_req),      /* SDSSSCR_CSG_APP_SERVICE_REQ */
                                      /* 95 */
  SD_ENUM_ELEMENT(ssscr_called_hdr_opti_redial_start),     /* SDSSSCR_HDR_OPTI_REDIAL_START */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_norm_slnt_non_multi_sim),     /* SDSSSCR_SRV_LOST_NORM_SLNT_NON_MULTI_SIM */
  #ifdef FEATURE_LIMITED_SCAN_OPT
  SD_ENUM_ELEMENT(ssscr_called_gwl_no_srv_plmn_found_lmtd_sys_scan), /* SDSSSCR_GWL_NO_SRV_PLMN_FOUND_LMTD_SYS_SCAN */
  #else
  SD_ENUM_ELEMENT(ssscr_called_reserved),              /* RESERVED */
  #endif 
                                      /* 98 */
  SD_ENUM_ELEMENT(ssscr_called_acq_db_scan_pwr_up),    /* SDSSSCR_ACQ_DB_SCAN_PWR_UP */
  SD_ENUM_ELEMENT(ssscr_called_acq_db_scan_pwr_up_3gpp),     /* SDSSSCR_ACQ_DB_SCAN_PWR_UP_3GPP */
  SD_ENUM_ELEMENT(ssscr_called_stack_sync_up_bsr_acq_all),     /* SDSSSCR_SS_PREF_STACK_SYNC_UP_BSR_ACQ_ALL */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_norm_acq_mode),         /* SDSSSCR_SRV_LOST_NORM_ACQ_MODE */
                                      /* 102 */
  SD_ENUM_ELEMENT(ssscr_called_srlte_no_trm_scope_acqdb),     /* SDSSSCR_SRLTE_NO_TRM_SCOPE_ACQDB */
  SD_ENUM_ELEMENT(ssscr_called_srlte_scope_full),         /* SDSSSCR_SRLTE_SCOPE_FULL */
  SD_ENUM_ELEMENT(ssscr_called_start_1xsib8_scan),        /* SDSSSCR_1XSIB8_SCAN_START */
  SD_ENUM_ELEMENT(ssscr_called_3gpp_srv_lost_c2k_scan_opt), /* SDSSSCR_3GPP_SRV_LOST_C2K_SCAN_OPT */
                                      /* 106 */
  SD_ENUM_ELEMENT(ssscr_called_start_bsr_timer),                 /* SDSSSCR_START_BSR_TIMER */
  SD_ENUM_ELEMENT(ssscr_called_gwl_opr_srv_info_sys_lost), /* SDSSSCR_GWL_OPR_SRV_INFO_SYS_LOST */
  SD_ENUM_ELEMENT(ssscr_called_opr_gw_emerg_call_orig_ltd),   /* SDSSSCR_OPR_GW_EMERG_CALL_ORIG_LTD */
  SD_ENUM_ELEMENT(ssscr_called_no_subs_emerg_mode),         /* SDSSSCR_NO_SUBS_EMERG_MODE */
                                      /* 110 */
  SD_ENUM_ELEMENT(ssscr_called_camped_full_mode_scan),      /* SDSSSCR_CAMP_FULL_MODE_SCAN */
  SD_ENUM_ELEMENT(ssscr_called_camped_mini_mode_scan),       /* SDSSSCR_CAMP_MINI_MODE_SCAN */
  SD_ENUM_ELEMENT(ssscr_called_create_emerg_list),        /* SDSSSCR_CREATE_EMERG_LIST */
  SD_ENUM_ELEMENT(ssscr_called_acq_db_emerg_call_orig),  /* SDSSSCR_ACQ_DB_EMERG_CALL_ORIG */
  SD_ENUM_ELEMENT(ssscr_called_orig_end_acq_db),          /* SDSSSCR_ORIG_END_ACQ_DB */
  SD_ENUM_ELEMENT(ssscr_called_srv_lost_china_emerg_mode)   /* SDSSSCR_SRV_LOST_CHINA_EMERG_MODE */
  };
