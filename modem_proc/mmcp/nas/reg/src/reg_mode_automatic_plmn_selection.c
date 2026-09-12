/*==============================================================================

                             COPYRIGHT INFORMATION

Copyright (c) 2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_mode_automatic_plmn_selection.c_v   1.2   06 Jun 2002 10:48:10   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/reg/src/reg_mode_automatic_plmn_selection.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

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
10/28/02   kwa     Added call to reg_send_mmr_stop_mode_req with reason
                   SYS_STOP_MODE_REASON_DEEP_SLEEP.
11/02/02   kwa     Added code to send cm_deep_sleep_ind only after the plmn
                   search period timer has reached its maximum value.
05/21/03   kwa     Restructured code to eliminate lint warnings.
06/04/03   kwa     Fixed one remaining lint warning.
09/08/03   kwa     Removed FEATURE_REG_DEEP_SLEEP.
09/18/03   kwa     Added sys_band_mask_type to start_automatic_plmn_selection.
10/22/03   kwa     Removed the parameters update_equivalent_plmn_list and
                   equivalent_plmn_list_p from the function finished.
11/19/03   kwa     Removed obsolete function for retrieving the mode name.
01/20/04   kwa     Added unexpected function handler
                   reg_mode_unexpected_start_plmn_selection_on_specified_plmn.
02/04/04   kwa     Added support for PS follow on request.
05/16/04   kwa     Added support for CS follow on request.
05/20/04   kwa     Removed calls to reg_mode_req_plmn_set since this function
                   is now called in reg_mode_update_attempted_plmn_list.  Added
                   code to resend an MMR_REG_REQ if an MMR_REG_CNF message was
                   received for the wrong RAT.
07/13/04   kwa     Added code to only resend an MMR_REG_REQ if an MMR_REG_CNF
                   message was received for the wrong RAT when the phone is
                   operating in single rather than dual mode.
01/05/06   sn      Removed stopping of HPLMN Timer in start_automatic_plmn_
                   selection.
02/16/06   sn      EHPLMN Feature. 
10/12/06   sn      WCDMA_1X mode preference implementation.
11/15/11  abhi   For EFRPLMNSI Selecting RLPMN if HPLMN is not found
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
#include "reg_state.h"
#include "msg.h"
#include "reg_nv.h"
#include "cm_v.h"

/*==============================================================================

                                TYPE DEFINITIONS

==============================================================================*/

/*==============================================================================

                            STATIC DATA DECLARATIONS

==============================================================================*/
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 


static reg_mode_automatic_mode_e_type reg_mode_automatic_plmn_selection_mode_sim[MAX_NAS_STACKS];
#define reg_mode_automatic_plmn_selection_mode   reg_mode_automatic_plmn_selection_mode_sim[reg_as_id]

static boolean                        reg_mode_automatic_plmn_selection_first_attempt_sim[MAX_NAS_STACKS];
#define reg_mode_automatic_plmn_selection_first_attempt   reg_mode_automatic_plmn_selection_first_attempt_sim[reg_as_id]


static reg_mode_3gpp_capability_s_type   reg_mode_automatic_plmn_selection_foreground_rat_list_sim[MAX_NAS_STACKS];
#define reg_mode_automatic_plmn_selection_foreground_rat_list reg_mode_automatic_plmn_selection_foreground_rat_list_sim[reg_as_id]

static boolean reg_mode_is_pwr_up_bsr_sim[MAX_NAS_STACKS] ={FALSE,FALSE};
#define reg_mode_is_pwr_up_bsr reg_mode_is_pwr_up_bsr_sim[reg_as_id]

#else

static reg_mode_automatic_mode_e_type reg_mode_automatic_plmn_selection_mode;

static boolean                        reg_mode_automatic_plmn_selection_first_attempt;


static reg_mode_3gpp_capability_s_type reg_mode_automatic_plmn_selection_foreground_rat_list;

static boolean reg_mode_is_pwr_up_bsr  = FALSE;

#endif /* FEATURE_DUAL_SIM || FEATURE_SGLTE */



/*==============================================================================

                          STATIC FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================

FUNCTION NAME

  start_automatic_plmn_selection

==============================================================================*/

static void start_automatic_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  reg_mode_automatic_mode_e_type       automatic_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  sys_radio_access_tech_e_type initial_rat;
  mmr_reg_interlace_scan_s_type   scan_info;
  reg_mode_3gpp_capability_s_type  hplmn_rat_search_order;

#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type     ehplmn_list;
  sys_plmn_id_s_type           rplmn, plmn_initial;
#else
  sys_plmn_id_s_type           hplmn;
#endif

  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  reg_mode_automatic_plmn_selection_first_attempt = TRUE;

  if(automatic_mode != REG_MODE_AUTOMATIC_MODE_IRAT_TO_LTE)
  {
  reg_mode_automatic_plmn_selection_mode = automatic_mode;
  }

  reg_mode_automatic_plmn_selection_foreground_rat_list.num_items = 0;

  reg_mode_clear_attempted_plmn_list();

#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = reg_sim_ehplmn_list_get();
#else
  hplmn = reg_sim_read_hplmn();
#endif

 #ifdef FEATURE_EQUIVALENT_HPLMN
   plmn_initial = ehplmn_list.plmn[0];
 /* If RPLMN is one of the EHPLNM then start PLMN selection from RPLMN 
  ** else use EHPLMN[0]
  */
  if ((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_MMSS_HPLMN) || 
    (reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION))
  {
	if(reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_MMSS_HPLMN)
    {
	    reg_mode_determine_first_plmn(&plmn_initial, &hplmn_rat_search_order, rat_pri_list_info, FALSE);
    }
    rplmn = reg_sim_read_rplmn(reg_mode_req_service_domain_get());
    if (reg_sim_is_ehplmn(rplmn))
    {
        plmn_initial = rplmn;
    }
  }
#endif

  reg_mode_get_3gpp_rat_capabilty(&hplmn_rat_search_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
      ,TRUE
#endif                                  
     );
  
  reg_sim_read_hplmn_rat_search_order(&hplmn_rat_search_order);
   if(sys_plmn_match(
#ifdef FEATURE_EQUIVALENT_HPLMN
          (plmn_initial)
#else
            hplmn
#endif
           , reg_sim_read_rplmn(reg_mode_req_service_domain_get()))) // hplmn is rplmn check
    {
      if(reg_state_get_mcs_pri()) // efs check
      {
        //re-arrange the rplmn rat search order
        reg_mode_prioritize_mcs_rat_order(&hplmn_rat_search_order,reg_state_get_mcs_low_priority_rat(),reg_state_get_mcs_high_priority_rat());
      }
        
    }
#ifdef FEATURE_LTE
  reg_mode_adjust_rat_order_plmn_backoff_forbidden
  (
#ifdef FEATURE_EQUIVALENT_HPLMN
    &(plmn_initial),
#else
    &hplmn,
#endif
    &hplmn_rat_search_order
  );
#endif
  
  initial_rat = hplmn_rat_search_order.rat_list[0];        

  if(reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION &&
   	  !cm_per_subs_is_plmn_rat_allowed(plmn_initial,initial_rat,(sys_modem_as_id_e_type)reg_sub_id,reg_set_cm_stack_id()))
   	{
   	  reg_mode_is_pwr_up_bsr = TRUE;
	  reg_mode_start_foreground_plmn_search();
	  reg_mode_is_pwr_up_bsr = FALSE;
	  return;
   	}
#ifdef FEATURE_HDR_TO_LTE
  if(automatic_mode == REG_MODE_AUTOMATIC_MODE_IRAT_TO_LTE)
  {
    reg_mode_clear_available_plmn_list();
    initial_rat = SYS_RAT_LTE_RADIO_ACCESS;
  }
  else
#endif
  {
    reg_mode_reset_available_plmn_list
    (
#ifdef FEATURE_EQUIVALENT_HPLMN
      plmn_initial,
#else
      hplmn,
#endif
      &hplmn_rat_search_order
    );
  }


  reg_mode_update_attempted_plmn_list
  (
#ifdef FEATURE_EQUIVALENT_HPLMN
    plmn_initial,
#else
    hplmn,
#endif
    initial_rat
  );

  reg_send_mmr_reg_req
  (
    network_selection_mode,              /* network_selection_mode */
#ifdef FEATURE_EQUIVALENT_HPLMN
    plmn_initial,
#else
    hplmn,                                /* plmn                   */
#endif
    req_service_domain,                   /* req_service_domain     */
    mode_pref,                            /* mode_pref              */
    cs_for_request,                       /* cs_for_request         */
    ps_for_request,                       /* ps_for_request         */
    initial_rat                            /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref                             /* band_pref              */
#endif
    ,rat_pri_list_info

    , ((automatic_mode == REG_MODE_AUTOMATIC_MODE_IRAT_TO_LTE)?
          MMR_REG_REQ_IRAT_TO_LTE:
          ((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION)?
          MMR_REG_REQ_BETTER_SYSTEM_RESELECTION : 
         ((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_EF_RAT_REFRESH)? 
                 MMR_REG_REQ_EF_RAT_REFRESH : MMR_REG_REQ_NORMAL)))
#ifdef FEATURE_LTE
   , sms_only,
     additional_info
#endif 
#ifdef FEATURE_FEMTO_CSG
  ,SYS_CSG_ID_INVALID
#endif 
  ,scan_info
  );

  reg_mode_automatic_plmn_selection_first_attempt = TRUE;
}
static boolean start_preferred_plmn_search
(

  sys_network_selection_mode_e_type    network_selection_mode,  
  sys_mode_pref_e_type                 mode_pref,               
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  , sys_band_mask_type                   band_pref
#endif  
  , const sys_rat_pri_list_info_s_type  *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  sys_plmn_id_s_type plmn;
  sys_radio_access_tech_e_type initial_rat;
  mmr_reg_interlace_scan_s_type    scan_info;
   reg_mode_3gpp_capability_s_type rplmn_rat_search_order;

   reg_mode_automatic_mode_e_type plmn_sel_mode;

   scan_info.new_scan  = TRUE;
   scan_info.use_timer = FALSE;
   scan_info.scan_time = 0;

   reg_mode_automatic_plmn_selection_foreground_rat_list.num_items = 0;


   reg_mode_get_3gpp_rat_capabilty(&rplmn_rat_search_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
		,TRUE
#endif                                  
	   );

	plmn_sel_mode = reg_mode_get_plmn_selection_mode();
	
	if(plmn_sel_mode == REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN)
	{
	  reg_mode_automatic_plmn_selection_mode = REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN;
	}
	else
	{
	  reg_mode_automatic_plmn_selection_mode = REG_MODE_AUTOMATIC_MODE_USER_RESELECTION;
	}
  
	plmn = reg_state_service_plmn_get();
    reg_mode_determine_first_plmn(&plmn, &rplmn_rat_search_order, rat_pri_list_info, TRUE);
	  
#ifdef FEATURE_LTE
	reg_mode_adjust_rat_order_plmn_backoff_forbidden(&plmn,&rplmn_rat_search_order);
#endif
	initial_rat = rplmn_rat_search_order.rat_list[0]; 	   
	  
    if(!(reg_mode_get_available_plmn_list_populated() == TRUE))  
    {
      reg_mode_reset_available_plmn_list
      (
        plmn,
        &rplmn_rat_search_order
      );
    }  
	/* If plmn is defined (either plmn from available list or rplmn, and allowed service domain is not no_service then 
	** send mmr_reg_req on this plmn.
	*/
	if(!sys_plmn_match(plmn, reg_state_service_plmn_get()))
	{
	  reg_mode_update_attempted_plmn_list
	  (
		plmn,
		initial_rat
	  );
          reg_mode_managed_roaming_plmn_reset();
          reg_mode_foreground_search_status = REG_MODE_FOREGROUND_SEARCH_STARTED;
	  reg_send_mmr_reg_req
	  (
		network_selection_mode,  /* network_selection_mode */
		plmn,					/* plmn 				  */
		req_service_domain,  /* req_service_domain	   */
		mode_pref,				 /* mode_pref			   */
		0, 		 /* cs_for_request		   */
		0, 		 /* ps_for_request		   */
		initial_rat 			/* rat					  */
#ifdef FEATURE_GSM_BAND_PREF
		,band_pref				  /* band_pref				*/
#endif
		,rat_pri_list_info

		,MMR_REG_REQ_NORMAL
#ifdef FEATURE_LTE
		,sms_only,
		 additional_info
#endif
#ifdef FEATURE_FEMTO_CSG
		,SYS_CSG_ID_INVALID
#endif 
                ,scan_info
	  );
	  reg_mode_automatic_plmn_selection_first_attempt = TRUE;
	  reg_mode_req_rat_set(initial_rat);
	  reg_mode_req_plmn_set(plmn);
	  return TRUE;
	}
	else
	{ 
	 return FALSE;
	}
}
static void start_foreground_plmn_search
(
  sys_network_selection_mode_e_type    network_selection_mode,  
  sys_mode_pref_e_type                 mode_pref,               
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  , sys_band_mask_type                   band_pref
#endif  
  , const sys_rat_pri_list_info_s_type  *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  sys_plmn_id_s_type plmn;
  mmr_reg_interlace_scan_s_type scan_info;
  sys_radio_access_tech_e_type initial_rat;
  reg_mode_automatic_mode_e_type plmn_sel_mode;

  uint8 i,j;

  initial_rat = reg_state_active_rat_get();

  reg_mode_get_3gpp_rat_capabilty(&reg_mode_automatic_plmn_selection_foreground_rat_list,
                                   rat_pri_list_info
#ifdef FEATURE_SGLTE
      ,TRUE
#endif                                  
     );  
  if(!reg_mode_is_rat_present_in_rat_pri_list(initial_rat, rat_pri_list_info, TRUE))
  {
     initial_rat = reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[0];
  }


  plmn_sel_mode = reg_mode_get_plmn_selection_mode();
  
  if(plmn_sel_mode == REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN)
  {
    reg_mode_automatic_plmn_selection_mode = REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN;
  }
  else if(reg_mode_is_pwr_up_bsr == FALSE) 
  {
    reg_mode_automatic_plmn_selection_mode = REG_MODE_AUTOMATIC_MODE_USER_RESELECTION;
  }

  reg_mode_foreground_search_status = REG_MODE_FOREGROUND_SEARCH_STARTED;
  
  sys_plmn_undefine_plmn_id(&plmn);

  reg_mode_clear_available_plmn_list();

  reg_mode_managed_roaming_plmn_reset();

  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  reg_send_mmr_reg_req
    (
      network_selection_mode,                        /* network_selection_mode */
      plmn,                                          /* plmn                   */
      req_service_domain,                            /* req_service_domain     */
      mode_pref,                                     /* mode_pref              */
      0,                                             /* cs_for_request         */
      0,                                             /* ps_for_request         */
      initial_rat                                    /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
      ,band_pref                                      /* band_pref              */
#endif
     ,rat_pri_list_info
    
      ,MMR_REG_REQ_NORMAL
#ifdef FEATURE_LTE
      ,sms_only,
       additional_info
#endif
#ifdef FEATURE_FEMTO_CSG
      ,SYS_CSG_ID_INVALID
#endif
      ,scan_info
    );
  reg_mode_automatic_plmn_selection_first_attempt = TRUE;
  reg_mode_req_rat_set(initial_rat);
  reg_mode_req_plmn_set(plmn);

  //Remove initial rat from the rat_list. 
  for (i= 0; i < reg_mode_automatic_plmn_selection_foreground_rat_list.num_items; i++)
  {
    if(reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[i] == initial_rat)
    {
      for(j = i+1; j < reg_mode_automatic_plmn_selection_foreground_rat_list.num_items; j++)
      {
        reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[j-1] = 
            reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[j];
      }
      break;
    }
  }
  reg_mode_automatic_plmn_selection_foreground_rat_list.num_items--;  

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
  sys_radio_access_tech_e_type  rat;
  sys_detailed_plmn_list_s_type *available = NULL;
  sys_srv_domain_e_type         allowed_service_domain;
  mmr_reg_interlace_scan_s_type scan_info;
  sys_plmn_id_s_type            plmn;
  boolean                       success = FALSE;
  boolean                       finished_automatic_plmn_selection = FALSE;
  int32 file_size = 0;
  uint8 index;

  sys_detailed_plmn_list_info_s_type managed_roaming_plmn;

  sys_radio_access_tech_e_type   rplmn_rat;
  sys_plmn_id_s_type           rplmn;
  file_size = sizeof(uint32)+(available_plmn_list_p->length*sizeof(sys_detailed_plmn_list_info_s_type));
  available = (sys_detailed_plmn_list_s_type *)modem_mem_calloc(1, file_size, MODEM_MEM_CLIENT_NAS);
  mm_check_for_null_ptr((void*)available);
  memset((void *)available,0,file_size);
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
    else if (reg_nv_w_bplmn_pseudo_enabled()
            ||reg_nv_is_forced_irat_enabled()
            )
    {    
        /* Start the timer if roaming is off and PLMN meets criteria
        */                   
       if ((service_state.roaming_ind == SYS_ROAM_STATUS_OFF)&&
            ((service_state.active_rat == SYS_RAT_TDS_RADIO_ACCESS) ||
            (service_state.active_rat == SYS_RAT_GSM_RADIO_ACCESS)||
               (service_state.active_rat == SYS_RAT_UMTS_RADIO_ACCESS)) &&
             (reg_mode_is_usim_compliant() || reg_nv_w_bplmn_pseudo_enabled()) &&
            reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, reg_mode_rat_pri_list_get(), FALSE)
          )
        {
          reg_mode_start_appropriate_irat_timers();
        }
    }

    finished_automatic_plmn_selection = TRUE;
    reg_mode_automatic_plmn_selection_first_attempt = FALSE;
   }
  /*
  ** The MS is not registered.
  */
  else
  {

    if((reg_mode_automatic_plmn_selection_first_attempt == TRUE) &&
       (service_state.active_rat != reg_mode_req_rat_get()) &&
       !(reg_mode_is_rat_present_in_rat_pri_list(service_state.active_rat,
                                            reg_mode_rat_pri_list_get(),TRUE)))
    {
      reg_send_last_mmr_reg_req();
      reg_mode_automatic_plmn_selection_first_attempt = FALSE;
    }
    else

    {
      	if(reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION) 
		{ 
			memscpy((void *)available, file_size, (void *)available_plmn_list_p, file_size) ;
			reg_mode_filter_bsr_plmn(available);
			reg_mode_update_available_plmn_list
			(
			reg_mode_automatic_plmn_selection_mode,													

			reg_mode_rat_pri_list_get(),

			reg_mode_req_service_domain_get(),
			available
			);
		}
		else
		{
			reg_mode_update_available_plmn_list
			(
			reg_mode_automatic_plmn_selection_mode,													

			reg_mode_rat_pri_list_get(),

			reg_mode_req_service_domain_get(),
			available_plmn_list_p
			);
		}
				

     
#ifdef FEATURE_LTE
      /*remove LTE from foreground RAT list if SIM is invalid for PS domain */
      if((reg_state_sim_status_get() == SYS_SIM_STATE_PS_INVALID) &&
          (reg_mode_automatic_plmn_selection_foreground_rat_list.num_items > 0))
      {
        reg_mode_remove_rat_from_3gpp_rat_list(&reg_mode_automatic_plmn_selection_foreground_rat_list, 
                                                                               SYS_RAT_LTE_RADIO_ACCESS);
      }
#endif 
      if(reg_mode_automatic_plmn_selection_foreground_rat_list.num_items > 0)
      {
        rat = reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[0];

      
        sys_plmn_undefine_plmn_id(&plmn);

        reg_send_mmr_reg_req
        (
          reg_mode_network_selection_mode_get(),         /* network_selection_mode */
          plmn,                                          /* plmn                   */
          reg_mode_req_service_domain_get(),             /* req_service_domain     */
          reg_mode_mode_pref_get(),                           /* mode_pref              */
          0,                                             /* cs_for_request         */
          0,                                             /* ps_for_request         */
          rat                                           /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
          ,reg_mode_band_pref_get()                       /* band_pref              */
#endif

          ,reg_mode_rat_pri_list_get()                
          
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

        reg_mode_req_rat_set(rat);
        reg_mode_req_plmn_set(plmn);
 
        //remove rat from rat_list
        for (index = 1; index < reg_mode_automatic_plmn_selection_foreground_rat_list.num_items; index++)
        {
          reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[index-1] = 
            reg_mode_automatic_plmn_selection_foreground_rat_list.rat_list[index];
        }
        reg_mode_automatic_plmn_selection_foreground_rat_list.num_items--;  

      }
      else
      {
        /*
        ** Get the next prioritized PLMN.
        */
        reg_mode_next_available_plmn
        (
          &plmn,
          &rat
        );

        if(reg_nv_efrplmnsi_select_rplmn() == TRUE &&
           (reg_mode_get_hplmn_to_selected(
 #if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
                                (sys_modem_as_id_e_type)reg_as_id
 #endif
                                           ) == TRUE))
        {         
 #ifdef FEATURE_EQUIVALENT_HPLMN  
            if(!reg_sim_is_ehplmn( plmn))
 #else
            if(!sys_plmn_match(reg_sim_read_hplmn(), plmn) )
 #endif  
           {
             reg_mode_next_equivalent_rplmn(&rplmn, &rplmn_rat);
             if(!sys_plmn_id_is_undefined(rplmn))
             {
               plmn = rplmn;
               rat = rplmn_rat;
               MSG_HIGH_DS_0(REG_SUB,"=REG= RPLMN to be selected after HPLMN");
             }
          }
        }      
        /*
        ** Attempt to acquire service on the PLMN.
        */
        if (!sys_plmn_id_is_undefined(plmn))
        {
            	if (!(sys_plmn_id_is_undefined(plmn))
					&&
					   //change
#ifdef FEATURE_EQUIVALENT_HPLMN  
						(reg_sim_is_ehplmn( plmn))
#else
						(sys_plmn_match(reg_sim_read_hplmn(), plmn) ) 
#endif
			   &&(reg_mode_automatic_plmn_selection_mode ==  REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION)
#ifdef FEATURE_DUAL_SIM
				&&
													   !cm_per_subs_is_plmn_rat_allowed(plmn,rat,(sys_modem_as_id_e_type)reg_sub_id,reg_set_cm_stack_id()))
#else                                  
								&&						!cm_is_plmn_rat_allowed(plmn,rat,(sys_modem_as_id_e_type)reg_sub_id))
#endif
			{
				 reg_mode_remove_from_attempted_plmn_list(plmn, rat); 
				 reg_mode_remove_from_available_plmn_list(plmn, rat); 
				 sys_plmn_undefine_plmn_id(&plmn);
			}
            if((((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_MMSS_HPLMN)&&
#ifdef FEATURE_EQUIVALENT_HPLMN  
            (!reg_sim_is_ehplmn( plmn))
#else
            (!sys_plmn_match(reg_sim_read_hplmn(), plmn) ) 
#endif
             )
			 || (reg_mode_automatic_plmn_selection_mode == 
								 REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION))

             &&(!cm_per_subs_is_plmn_rat_allowed(plmn,rat,(sys_modem_as_id_e_type)reg_sub_id,reg_set_cm_stack_id()))
			 &&(!sys_plmn_id_is_undefined(plmn)))
          {
            MSG_HIGH_DS_0(REG_SUB,"=REG= CM API for PLMN/RAT allowed query returned FALSE, send no service to CM");
            finished_automatic_plmn_selection = TRUE;
          }
          else
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
              reg_mode_network_selection_mode_get(), /* network_selection_mode */
              plmn,                                 /* plmn                   */
              allowed_service_domain,               /* req_service_domain     */
              reg_mode_mode_pref_get(),             /* mode_pref              */
              reg_mode_cs_for_request_get(),        /* cs_for_request         */
              reg_mode_ps_for_request_get(),        /* ps_for_request         */
              rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
             ,reg_mode_band_pref_get()              /* band_pref              */
#endif
             ,reg_mode_rat_pri_list_get()                 /*rat_pri_list*/
              
             , ((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_BETTER_SYSTEM_RESELECTION)?
                   MMR_REG_REQ_BETTER_SYSTEM_RESELECTION : 
                   ((reg_mode_automatic_plmn_selection_mode == REG_MODE_AUTOMATIC_MODE_EF_RAT_REFRESH)? 
                      MMR_REG_REQ_EF_RAT_REFRESH : MMR_REG_REQ_NORMAL))
#ifdef FEATURE_LTE
             ,reg_mode_sms_only_get(),
              0
#endif
#ifdef FEATURE_FEMTO_CSG
             ,SYS_CSG_ID_INVALID
#endif 
             ,scan_info
            );
  
            reg_mode_automatic_plmn_selection_first_attempt = TRUE;
          }
        }
        else
        {
          managed_roaming_plmn = reg_mode_managed_roaming_plmn_get();
         if(mm_managed_roaming_enabled && !sys_plmn_id_is_undefined(managed_roaming_plmn.plmn))
         {
           allowed_service_domain = reg_mode_allowed_service_domain
           (
             managed_roaming_plmn.plmn,                                 /* plmn                   */
             reg_mode_req_service_domain_get()     /* req_service_domain     */
           );
           
           reg_send_mmr_reg_req
           (
             reg_mode_network_selection_mode_get(), /* network_selection_mode */
             managed_roaming_plmn.plmn,                                 /* plmn                   */
             allowed_service_domain,               /* req_service_domain     */
             reg_mode_mode_pref_get(),             /* mode_pref              */
             reg_mode_cs_for_request_get(),        /* cs_for_request         */
             reg_mode_ps_for_request_get(),        /* ps_for_request         */
             managed_roaming_plmn.rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
             ,reg_mode_band_pref_get()              /* band_pref              */
#endif    
             ,reg_mode_rat_pri_list_get()                 /*rat_pri_list*/
  
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

            reg_mode_req_rat_set(managed_roaming_plmn.rat);
            reg_mode_req_plmn_set(managed_roaming_plmn.plmn);
            reg_mode_managed_roaming_plmn_reset();

            reg_mode_automatic_plmn_selection_first_attempt = TRUE; 
         } 
         else
         {
            finished_automatic_plmn_selection = TRUE;

            reg_mode_automatic_plmn_selection_first_attempt = FALSE;
         }
        }
      }
    }
  }
  modem_mem_free(available, MODEM_MEM_CLIENT_NAS);
  available = NULL;
  return finished_automatic_plmn_selection;
}


/*==============================================================================

FUNCTION NAME

  number

==============================================================================*/

static reg_mode_e_type number ( void )
{
  return REG_MODE_AUTOMATIC_PLMN_SELECTION;
}


/*==============================================================================

                               DATA DECLARATIONS

==============================================================================*/

reg_mode_s_type reg_mode_automatic_plmn_selection = 
{
  start_automatic_plmn_selection,
  reg_mode_unexpected_start_higher_priority_plmn_selection,
  reg_mode_unexpected_start_limited_service,
  reg_mode_unexpected_start_manual_plmn_selection,
  reg_mode_unexpected_start_plmn_selection,
  reg_mode_unexpected_start_plmn_selection_roaming_plmn,
  reg_mode_unexpected_start_plmn_selection_on_specified_plmn,
  start_foreground_plmn_search,
  start_preferred_plmn_search,
#if defined(FEATURE_LTE) || defined(FEATURE_WRLF_SYSTEM_SEL)
  reg_mode_unexpected_start_suitable_cell_selection,
#endif
  reg_mode_unexpected_start_higher_priority_irat_plmn_selection,
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
/*=============================================================================
     QSH related APIs
==============================================================================*/
void get_dump_reg_mode_automatic_plmn_selection_vars (reg_debug_data_s_type* ptr, 
                                                      reg_as_id_e_type as_id)
{
    ASSERT (ptr != NULL);
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
    ptr->reg_mode_automatic_plmn_selection_first_attempt_debug = 
        reg_mode_automatic_plmn_selection_first_attempt_sim[as_id];
    ptr->reg_mode_automatic_plmn_selection_foreground_rat_list_debug = 
        reg_mode_automatic_plmn_selection_foreground_rat_list_sim[as_id]; 
    ptr->reg_mode_automatic_plmn_selection_mode_debug = 
        (reg_mode_automatic_mode_debug_e_type)reg_mode_automatic_plmn_selection_mode_sim[as_id];
#else
    ptr->reg_mode_automatic_plmn_selection_first_attempt_debug = 
        reg_mode_automatic_plmn_selection_first_attempt;
    ptr->reg_mode_automatic_plmn_selection_foreground_rat_list_debug = 
        reg_mode_automatic_plmn_selection_foreground_rat_list; 
    ptr->reg_mode_automatic_plmn_selection_mode_debug = 
        (reg_mode_automatic_mode_debug_e_type)reg_mode_automatic_plmn_selection_mode;
#endif
};
#endif

reg_mode_3gpp_capability_s_type* reg_mode_get_foreground_search_rat_list(void)
{
    return &reg_mode_automatic_plmn_selection_foreground_rat_list;
}


