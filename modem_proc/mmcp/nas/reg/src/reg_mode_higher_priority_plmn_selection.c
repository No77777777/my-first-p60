/*==============================================================================

                             COPYRIGHT INFORMATION

Copyright (c) 2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_mode_higher_priority_plmn_selection.c_v   1.2   06 Jun 2002 10:48:28   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/reg/src/reg_mode_higher_priority_plmn_selection.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/13/02   kwa     Initial release
05/29/02   kwa     Changes to support sending a single RAT rather than a mode
                   preference in the mmr_reg_req message.
05/31/02   kwa     Added RPLMN in call to save equivalent PLMN list.
06/27/02   kwa     Modified function that sends an mmr_reg_req due to
                   interface change.
09/23/02   kwa     Added code to determine the allowed service domain.
10/18/02   kwa     Added FEATURE_REG_DEEP_SLEEP.
05/21/03   kwa     Restructured code to eliminate lint warnings.
06/04/03   kwa     Fixed one remaining lint warning.
09/07/03   kwa     Replaced call to reg_mode_start_plmn_selection with
                   reg_mode_reinitiate_plmn_selection to properly reset the
                   HPLMN search period timer.
09/08/03   kwa     Removed FEATURE_REG_DEEP_SLEEP.
09/18/03   kwa     Added support for band preferences.
10/22/03   kwa     Removed the parameters update_equivalent_plmn_list and
                   equivalent_plmn_list_p from the function finished.
11/19/03   kwa     Removed obsolete function for retrieving the mode name.
01/20/04   kwa     Added unexpected function handler
                   reg_mode_unexpected_start_plmn_selection_on_specified_plmn.
02/04/04   kwa     Added support for PS follow request.
03/03/04   kwa     Removed calls to clear the attempted and available PLMN
                   lists each time the start_higher_priority_plmn_selection
                   function is called.
04/28/04   kwa     Force the mode to SYS_NETWORK_SELECTION_MODE_AUTOMATIC in
                   finished to handle the case when normal service has been
                   acquired while looking for limited service.
05/16/04   kwa     Added support for CS follow request.
05/20/04   kwa     Removed calls to reg_mode_req_plmn_set since this function
                   is now called in reg_mode_update_attempted_plmn_list.  Added
                   code to resend an MMR_REG_REQ if an MMR_REG_CNF message was
                   received for the wrong RAT.
07/13/04   kwa     Added code to only resend an MMR_REG_REQ if an MMR_REG_CNF
                   message was received for the wrong RAT when the phone is
                   operating in single rather than dual mode.
02/17/05   kwa     Replaced calls to reg_mode_mode_pref_get with
                   reg_mode_req_mode_pref_get to properly handle the search
                   for a higher priority network after receiving an LAU or RAU
                   reject with cause #15.
12/21/04   up      Modified function finished to create a LOG PACKET to
                   indicate that higher priority plmn search is finished.
01/18/06   up      Calling of reg_log_umts_nas_hplmn_search_end is kept under
                   feature FEATURE_GTOW_BACKGROUND_PLMN_SEARCH.
10/12/06   sn      WCDMA_1x mode preference implementation.
==============================================================================*/

/*==============================================================================

                            INCLUDE FILES FOR MODULE

==============================================================================*/

#include "mmcp_variation.h"
#include "customer.h"
#include "reg_mode.h"
#include "reg_send.h"
#include "reg_sim_v.h"
#include "reg_timers.h"
#include "reg_log.h"
#include "reg_nv.h"
/*==============================================================================

                                TYPE DEFINITIONS

==============================================================================*/

/*==============================================================================

                            STATIC DATA DECLARATIONS

==============================================================================*/
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 

static boolean reg_mode_higher_priority_plmn_selection_first_attempt_sim[MAX_NAS_STACKS];
#define reg_mode_higher_priority_plmn_selection_first_attempt   reg_mode_higher_priority_plmn_selection_first_attempt_sim[reg_as_id]

static boolean reg_mode_higher_priority_plmn_selection_started_sim[MAX_NAS_STACKS];
#define reg_mode_higher_priority_plmn_selection_started   reg_mode_higher_priority_plmn_selection_started_sim[reg_as_id]

#else

static boolean reg_mode_higher_priority_plmn_selection_started;
static boolean reg_mode_higher_priority_plmn_selection_first_attempt;

#endif /* FEATURE_DUAL_SIM || FEATURE_SGLTE */


static reg_mode_automatic_mode_e_type reg_mode_automatic_type;
/*==============================================================================

                          STATIC FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================

FUNCTION NAME

  start_higher_priority_plmn_selection

==============================================================================*/

static boolean start_higher_priority_plmn_selection
(
  const sys_detailed_plmn_list_s_type*       available_plmn_list_p
)
{
  sys_radio_access_tech_e_type rat;
  sys_srv_domain_e_type        allowed_service_domain;
  sys_plmn_id_s_type           plmn;
  mmr_reg_interlace_scan_s_type    scan_info;
  boolean                      started = FALSE;
  mmr_reg_req_e_type sel_type = MMR_REG_REQ_NORMAL;
  boolean ehplmn_found = FALSE;
  
  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  reg_mode_clear_attempted_plmn_list();
  reg_mode_clear_available_plmn_list();
  reg_mode_automatic_type = REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH;
  reg_mode_update_available_plmn_list
  (
    REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH,

    reg_mode_rat_pri_list_get(),

    reg_mode_req_service_domain_get(),
    available_plmn_list_p
  );

#if defined(FEATURE_EQUIVALENT_HPLMN)
  ehplmn_found = reg_mode_keep_ehplmn_to_available_plmn_list();
#endif

  /*
  ** Get the next prioritized PLMN.
  */
  reg_mode_next_higher_priority_plmn
  (

    &plmn,
    &rat
  );

  /*
  ** Attempt to acquire service on the PLMN.
  */
  if (!sys_plmn_id_is_undefined(plmn))
  {
    reg_mode_update_attempted_plmn_list
    (
      plmn,
      rat
    );

    allowed_service_domain = reg_mode_allowed_service_domain
    (
      plmn,                                 /* plmn                   */
      reg_mode_req_service_domain_get()     /* req_service_domain     */
    );

    if (ehplmn_found)
    {
      /* Invalidate EPLMN list from MM during high prioirty PLMN 
      ** selection to restrict EPLMN camping as suitable cell */
      sel_type = MMR_REG_REQ_ACQ_HIGH_PRIOIRTY_SEL_W_O_EPLMN;
    }

    reg_send_mmr_reg_req
    (
      SYS_NETWORK_SELECTION_MODE_AUTOMATIC, /* network_selection_mode */
      plmn,                                 /* plmn                   */
      allowed_service_domain,               /* req_service_domain     */
      reg_mode_req_mode_pref_get(),         /* mode_pref              */
      FALSE,                                /* cs_for_request         */
      FALSE,                                /* ps_for_request         */
      rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
      ,reg_mode_band_pref_get()              /* band_pref              */
#endif
      ,reg_mode_rat_pri_list_get()                 /*rat_pri_list*/

      ,sel_type
#ifdef FEATURE_LTE
      ,reg_mode_sms_only_get(),
       0
#endif
#ifdef FEATURE_FEMTO_CSG
      ,SYS_CSG_ID_INVALID
#endif 
      ,scan_info
    );

    started = TRUE;

    reg_mode_higher_priority_plmn_selection_first_attempt = TRUE;
    reg_mode_higher_priority_plmn_selection_started = TRUE;
  }

  /*
  ** LOG a PACKET to indicate the list of available PLMNs
  ** after higher priority PLMN search is finished.
  */
  reg_log_umts_nas_hplmn_search_end(reg_mode_available_plmn_list_get());


  return started;
}

boolean reg_mode_get_higher_priority_plmn_selection_started(sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_DUAL_SIM
    return reg_mode_higher_priority_plmn_selection_started_sim[as_id];
#else
   return reg_mode_higher_priority_plmn_selection_started;
#endif
}
void reg_mode_set_higher_priority_plmn_selection_started(boolean status)
{
    reg_mode_higher_priority_plmn_selection_started = status;
}

/*==============================================================================

FUNCTION NAME

  start_higher_priority_irat_plmn_selection

==============================================================================*/
static boolean start_higher_priority_irat_plmn_selection
(
  const sys_detailed_plmn_list_s_type*       available_plmn_list_p
)
{
    sys_radio_access_tech_e_type rat;
    sys_srv_domain_e_type        allowed_service_domain;
    sys_plmn_id_s_type           plmn;
    boolean                      started = FALSE;
    boolean                      reg_start = TRUE;
    mmr_reg_req_e_type           type;
    mmr_reg_interlace_scan_s_type   scan_info;

    scan_info.new_scan  = TRUE;
    scan_info.use_timer = FALSE;
    scan_info.scan_time = 0;

    reg_mode_clear_attempted_plmn_list();
    reg_mode_clear_available_plmn_list();
    reg_mode_automatic_type = REG_MODE_AUTOMATIC_MODE_IRAT_HIGH_PRIORITY_SEARCH;

    /* Add the current RPLMN to the available reg*/
    reg_mode_update_available_plmn_list
    (
        REG_MODE_AUTOMATIC_MODE_IRAT_HIGH_PRIORITY_SEARCH,
        reg_mode_rat_pri_list_get(),
        reg_mode_req_service_domain_get(),
        available_plmn_list_p
    );


      reg_mode_update_lte_lists();
      reg_state_set_irat_hplmn_timer_search_ongoing(FALSE);
      reg_state_set_irat_lte_timer_search_ongoing(FALSE);
    
    /*
    ** Get the next prioritized PLMN.
    */
    reg_mode_next_available_plmn 
    (
       &plmn,
       &rat
    );

    /*
    ** Attempt to acquire service on the PLMN.
    */
    if (!sys_plmn_id_is_undefined(plmn))
    {
        reg_mode_update_attempted_plmn_list
        (
            plmn,
            rat
        );

        allowed_service_domain = reg_mode_allowed_service_domain
        (
            plmn,                                 /* plmn                   */
            reg_mode_req_service_domain_get()     /* req_service_domain     */
        );
        /* For SGLTE case, determine if split is needed. If so, send
           a split request with MMR_REG_REQ_SGLTE_SPLIT_REQUEST.
           The following conditions need to be met
           ds 1 active, ds2 inactive
           SGLTE mode

        */
        type = MMR_REG_REQ_NORMAL;
 #ifdef FEATURE_SGLTE
        if( IS_UE_IN_SGLTE_STATE_4 &&
            reg_state_get_active_rat_per_subs((sys_modem_as_id_e_type)reg_as_id) == SYS_RAT_GSM_RADIO_ACCESS &&
            reg_state_get_service_status_per_subs((sys_modem_as_id_e_type)reg_as_id)== SYS_SRV_STATUS_SRV &&
            (rat == SYS_RAT_LTE_RADIO_ACCESS ||
            rat == SYS_RAT_TDS_RADIO_ACCESS)
           )
         {
           if(IS_SGLTE_SPLIT_IN_PROGRESS)
            {
              reg_start = FALSE;
            }
            else
            {
              type = MMR_REG_REQ_SGLTE_PS_ONLY_ACQUISITION;
            }
         }
#endif

		if(reg_start)
        {
          reg_send_mmr_reg_req
          (
            SYS_NETWORK_SELECTION_MODE_AUTOMATIC, /* network_selection_mode */
            plmn,                                 /* plmn                   */
            allowed_service_domain,               /* req_service_domain     */
            reg_mode_req_mode_pref_get(),         /* mode_pref              */
            FALSE,                                /* cs_for_request         */
            FALSE,                                /* ps_for_request         */
            rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
            ,reg_mode_band_pref_get()              /* band_pref              */
#endif
            ,reg_mode_rat_pri_list_get()                 /*rat_pri_list*/
            ,type
#ifdef FEATURE_LTE
            ,reg_mode_sms_only_get(),
             0
#endif
#ifdef FEATURE_FEMTO_CSG
            ,SYS_CSG_ID_INVALID
#endif 
            ,scan_info
          );
          started = TRUE;
          reg_mode_higher_priority_plmn_selection_first_attempt = TRUE;
	   }
    }
    
    return started;
}

/*==============================================================================

FUNCTION NAME

  finished

==============================================================================*/

static boolean finished
(
        sys_plmn_service_state_s_type  service_state,
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{
  sys_radio_access_tech_e_type rat;
  sys_srv_domain_e_type        allowed_service_domain;
  mmr_reg_interlace_scan_s_type    scan_info;
  sys_plmn_id_s_type           plmn;
  boolean                      success = FALSE;
  boolean                      finished_higher_priority_plmn_selection = FALSE;

  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  success = reg_mode_successful_registration
  (
    service_state.plmn,
    service_state.service_status,
    service_state.active_rat,
    service_state.plmn_reg_type
  );

  /*
  ** The MS is registered.
  */
  if (success)
  {
    if (service_state.roaming_ind == SYS_ROAM_STATUS_ON 
#if defined(FEATURE_EQUIVALENT_HPLMN)
        || !reg_mode_is_camped_plmn_is_higher_priority_ehplmn(reg_state_service_plmn_get())
#endif
      )
    {
      reg_timers_start_hplmn_search_period_timer();
    }
    else
    {
        MSG_HIGH_DS_0(REG_SUB, "=REG= Starting reg_mode_rat_search_start");
        reg_mode_rat_search_start();
    }

    finished_higher_priority_plmn_selection = TRUE;
  }
  /*
  ** The MS is not registered.
  */
  else
  {
    /*
    ** If the MS is not on the requested RAT then retry one more time.
    */

    if((reg_mode_higher_priority_plmn_selection_first_attempt == TRUE) &&
        (service_state.active_rat != reg_mode_req_rat_get()) &&
       !(reg_mode_is_rat_present_in_rat_pri_list(service_state.active_rat,
                                            reg_mode_rat_pri_list_get(),TRUE)))
    {
      reg_send_last_mmr_reg_req();
      reg_mode_higher_priority_plmn_selection_first_attempt = FALSE;
    }
    else

    {
      reg_mode_update_available_plmn_list
      (
        reg_mode_automatic_type,

        reg_mode_rat_pri_list_get(),

        reg_mode_req_service_domain_get(),
        available_plmn_list_p
      );

     if (reg_mode_automatic_type == REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH)
     {
    /*
    ** Get the next prioritized PLMN.
    */
      reg_mode_next_higher_priority_plmn
      (

        &plmn,
        &rat
      );
     }
#ifdef FEATURE_TDSCDMA
     else 
     {
      reg_mode_next_available_plmn      
      (
        &plmn,
        &rat
      );
     }
#endif
      /*
      ** Attempt to acquire service on the PLMN.
      */
      if (!sys_plmn_id_is_undefined(plmn))
      {
        reg_mode_update_attempted_plmn_list
        (
          plmn,
          rat
        );
  
        allowed_service_domain = reg_mode_allowed_service_domain
        (
          plmn,                                 /* plmn                   */
          reg_mode_req_service_domain_get()     /* req_service_domain     */
        );
  
        reg_send_mmr_reg_req
        (
          SYS_NETWORK_SELECTION_MODE_AUTOMATIC, /* network_selection_mode */
          plmn,                                 /* plmn                   */
          allowed_service_domain,               /* req_service_domain     */
          reg_mode_req_mode_pref_get(),         /* mode_pref              */
          FALSE,                                /* cs_for_request         */
          FALSE,                                /* ps_for_request         */
          rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
          ,reg_mode_band_pref_get()              /* band_pref              */
#endif
          ,reg_mode_rat_pri_list_get()                    /*ue_capabilty*/
 
          ,MMR_REG_REQ_NORMAL
#ifdef FEATURE_LTE
          ,reg_mode_sms_only_get(),
           0
#endif
#ifdef FEATURE_FEMTO_CSG
          ,SYS_CSG_ID_INVALID
#endif 
          ,scan_info
        );

        reg_mode_higher_priority_plmn_selection_first_attempt = TRUE;
        if (reg_mode_automatic_type == REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH)
        {
          reg_mode_higher_priority_plmn_selection_started = TRUE;
        }

      }
      /*
      ** All attempts to register on a higher priority PLMN have failed.
      */
      else
      {
        reg_mode_reinitiate_plmn_selection
        (
          SYS_NETWORK_SELECTION_MODE_AUTOMATIC,
          reg_mode_req_mode_pref_get(),
          FALSE,
          FALSE,
#ifdef FEATURE_GSM_BAND_PREF
          reg_mode_req_service_domain_get(),
          reg_mode_band_pref_get()
#else
          reg_mode_req_service_domain_get()
#endif
          ,reg_mode_rat_pri_list_get()

#ifdef FEATURE_LTE
          ,reg_mode_sms_only_get(),
           0
#endif 
        );
        reg_mode_higher_priority_plmn_selection_first_attempt = FALSE;
      }
    }
  }

  return finished_higher_priority_plmn_selection;
}


/*==============================================================================

FUNCTION NAME

  number

==============================================================================*/

static reg_mode_e_type number ( void )
{
  return REG_MODE_HIGHER_PRIORITY_PLMN_SELECTION;
}


/*==============================================================================

                               DATA DECLARATIONS

==============================================================================*/

reg_mode_s_type reg_mode_higher_priority_plmn_selection = 
{
  reg_mode_unexpected_start_automatic_plmn_selection,
  start_higher_priority_plmn_selection,
  reg_mode_unexpected_start_limited_service,
  reg_mode_unexpected_start_manual_plmn_selection,
  reg_mode_unexpected_start_plmn_selection,
  reg_mode_unexpected_start_plmn_selection_roaming_plmn,
  reg_mode_unexpected_start_plmn_selection_on_specified_plmn,
  reg_mode_unexpected_start_foreground_plmn_search,
  reg_mode_unexpected_start_preferred_plmn_search,
#if defined(FEATURE_LTE) || defined(FEATURE_WRLF_SYSTEM_SEL)
  reg_mode_unexpected_start_suitable_cell_selection,
#endif
  start_higher_priority_irat_plmn_selection,
#ifdef FEATURE_FEMTO_CSG
  reg_mode_unexpected_start_automatic_csg_selection,
  reg_mode_unexpected_start_automatic_csg_selection_app,
  reg_mode_unexpected_start_manual_csg_selection,
  reg_mode_unexpected_start_manual_csg_selection_app,
#endif
  reg_mode_unexpected_update_plmn_selection_mode,
  finished,
  number
};

/*==============================================================================

                              FUNCTION DEFINITIONS

==============================================================================*/
#ifdef FEATURE_QSH_DUMP
void get_dump_reg_mode_higher_priority_plmn_selection_vars(reg_debug_data_s_type* ptr,
                                                           reg_as_id_e_type as_id)
{
    ASSERT (ptr != NULL);
    ptr->reg_mode_automatic_type_debug = 
        (reg_mode_automatic_mode_debug_e_type)reg_mode_automatic_type;
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE    
    ptr ->reg_mode_higher_priority_plmn_selection_first_attempt_debug = 
       reg_mode_higher_priority_plmn_selection_first_attempt_sim[as_id];
#else   
    ptr ->reg_mode_higher_priority_plmn_selection_first_attempt_debug = 
       reg_mode_higher_priority_plmn_selection_first_attempt;
#endif
}
#endif

