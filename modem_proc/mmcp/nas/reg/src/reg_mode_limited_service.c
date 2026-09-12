/*==============================================================================

                             COPYRIGHT INFORMATION

Copyright (c) 2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_mode_limited_service.c_v   1.2   06 Jun 2002 10:48:42   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/reg/src/reg_mode_limited_service.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/13/02   kwa     Initial release
05/31/02   kwa     Added update_equivalent_plmn_list to finished function.
06/27/02   kwa     Modified function that sends an mmr_reg_req due to
                   interface change.
10/01/02   kwa     Added reg_mode_limited_service_domain to store the requested
                   service domain for limited service independently from the
                   normal requested service domain.  This change was made to 
                   properly support limited service when the SIM/USIM is marked
                   as invalid by the network.
10/10/02   kwa     Killed timers upon processing request for limited service.
                   Fixed bug where REG did not send the cm_service_cnf message.
10/18/02   kwa     Changed behavior when the initial search for limited service
                   fails.  Instead of setting the PLMN Search Period Timer the
                   code now immediately reinitiates a search for service.
                   Added FEATURE_REG_DEEP_SLEEP.
04/04/03   kwa     Placed the code to repeatedly search for limited service
                   within FEATURE_REG_DEEP_SLEEP.
05/21/03   kwa     Restructured code to eliminate lint warnings.
06/04/03   kwa     Fixed one remaining lint warning.
09/08/03   kwa     Removed FEATURE_REG_DEEP_SLEEP.
09/16/03   kwa     Always set reg_mode_limited_service_domain to
                   SYS_SRV_DOMAIN_CS_ONLY when placing the phone in limited
                   service mode.
09/18/03   kwa     Added support for band preferences.
09/25/03   kwa     Revert to setting reg_mode_limited_service_domain to
                   the requested service domain when placing the phone in
                   limited service mode.
10/02/03   kwa     Added code to support the routine acquisition of normal
                   service when requesting limited service using
                   SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION.
10/22/03   kwa     Removed the parameters update_equivalent_plmn_list and
                   equivalent_plmn_list_p from the function finished.
11/19/03   kwa     Removed obsolete function for retrieving the mode name.
01/20/04   kwa     Added unexpected function handler
                   reg_mode_unexpected_start_plmn_selection_on_specified_plmn.
02/04/04   kwa     Added support for PS follow on request.
03/09/04   kwa     Added code in finished to set the HPLMN search period timer
                   if normal service was acquired during a routine search for
                   limited service.
05/04/04   kwa     Added code in start_limited_service to initiate the search
                   for limited service on the currently active rat.
05/16/04   kwa     Added support for CS follow on request.
06/08/04   kwa     Added code in start_limited_service to only initiate the
                   search for limited service on the currently active rat when
                   the active rat is consistent with the mode preference.
07/04/06   sn      Modified start_limited_service to fix incorrect brackets.
07/27/06   sn      Added Uniform OOS changes.
                   FEATURE_UMTS_UNIFORM_OOS_HANDLING
10/12/06   sn      WCDMA_1X mode preference implementation.
12/15/09   abhi  Removing UOOS code
==============================================================================*/

/*==============================================================================

                            INCLUDE FILES FOR MODULE

==============================================================================*/

#include "mmcp_variation.h"
#include "customer.h"
#include "reg_mode.h"
#include "reg_nv.h"
#include "reg_send.h"
#include "reg_state.h"
#include "reg_timers.h"

#include "msg.h"
#include "sys.h"
#include "policyman.h"

#include <stringl/stringl.h>
/*==============================================================================

                                TYPE DEFINITIONS

==============================================================================*/

/*==============================================================================

                            STATIC DATA DECLARATIONS

==============================================================================*/
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
static sys_srv_domain_e_type        reg_mode_limited_service_domain_sim[MAX_NAS_STACKS] = { SYS_SRV_DOMAIN_CS_ONLY, SYS_SRV_DOMAIN_CS_ONLY, SYS_SRV_DOMAIN_CS_ONLY};
#else
static sys_srv_domain_e_type        reg_mode_limited_service_domain_sim[MAX_NAS_STACKS] = { SYS_SRV_DOMAIN_CS_ONLY, SYS_SRV_DOMAIN_CS_ONLY};
#endif 
#define reg_mode_limited_service_domain reg_mode_limited_service_domain_sim[reg_as_id]

static sys_radio_access_tech_e_type reg_mode_limited_service_initial_rat_sim[MAX_NAS_STACKS] = { SYS_RAT_GSM_RADIO_ACCESS, SYS_RAT_GSM_RADIO_ACCESS};
#define reg_mode_limited_service_initial_rat   reg_mode_limited_service_initial_rat_sim[reg_as_id]

static reg_mode_3gpp_capability_s_type reg_mode_limited_service_acq_db_rat_order_sim[MAX_NAS_STACKS];
#define reg_mode_limited_service_acq_db_rat_order reg_mode_limited_service_acq_db_rat_order_sim[reg_as_id]

static reg_mode_3gpp_capability_s_type reg_mode_limited_service_rat_order_sim[MAX_NAS_STACKS];
#define reg_mode_limited_service_rat_order reg_mode_limited_service_rat_order_sim[reg_as_id]


#else

static sys_srv_domain_e_type        reg_mode_limited_service_domain = SYS_SRV_DOMAIN_CS_ONLY;
static reg_mode_3gpp_capability_s_type reg_mode_limited_service_acq_db_rat_order;
static sys_radio_access_tech_e_type reg_mode_limited_service_initial_rat = SYS_RAT_GSM_RADIO_ACCESS;

static reg_mode_3gpp_capability_s_type reg_mode_limited_service_rat_order;


#endif /* FEATURE_DUAL_SIM || FEATURE_SGLTE */
/*==============================================================================

                          STATIC FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================

FUNCTION NAME

  reg_mode_limited_service_get_acq_db_rat_list

==============================================================================*/
void reg_mode_limited_service_get_acq_db_rat_list
(
  reg_mode_3gpp_capability_s_type *rat_3gpp_capabilty,
  const sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  uint32 i;

  rat_3gpp_capabilty->num_items = 0;

  for (i = 0; (i < rat_pri_list_info->num_items) &&
              (rat_3gpp_capabilty->num_items < REG_MODE_MAX_3GPP_RAT); i++)
  {
    if (rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_GSM)         
    {
      rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_GSM_RADIO_ACCESS;       
      rat_3gpp_capabilty->num_items++;  
    }
    else if (rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_WCDMA)
    {
      rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_UMTS_RADIO_ACCESS;       
      rat_3gpp_capabilty->num_items++;  
    }
    else if (rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_TDS)
    {
      rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_TDS_RADIO_ACCESS;       
      rat_3gpp_capabilty->num_items++;  
    }
       
#ifdef FEATURE_LTE
    else if(rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
      rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_LTE_RADIO_ACCESS;       
      rat_3gpp_capabilty->num_items++;  
    }
#endif /* FEATURE_LTE */
  }
}

/*==============================================================================

FUNCTION NAME

  start_limited_service

==============================================================================*/

static void start_limited_service
(
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif

  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  sys_plmn_id_s_type           plmn;
  mmr_reg_interlace_scan_s_type    scan_info;
  sys_specialization_t  specialization = SYS_SPECIAL_NONE;
  policyman_status_t  status = POLICYMAN_STATUS_ERR;
  policyman_item_t    *pItem;
  mmr_reg_req_e_type  serv_req_type = MMR_REG_REQ_NORMAL;    

  sys_radio_access_tech_e_type initial_rat;
  uint8 i;
  uint8 j;

  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  {
    policyman_item_id_t ids[] = {POLICYMAN_ITEM_DEVICE_CONFIGURATION};
    status = policyman_get_items(ids, ARR_SIZE(ids), (policyman_item_t const **) &pItem);    
  }
  if (POLICYMAN_SUCCEEDED(status))
  {
     policyman_device_config_get_specialization(pItem, &specialization);
     policyman_item_release(pItem);
  }

  if(type == REG_CM_SERVICE_REQ_FULL_RAT_MODE)
  {
    serv_req_type = MMR_REG_REQ_FULL_RAT_MODE;
  }

  if(reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED)
  {
    reg_timers_stop_hplmn_search_period_timer();
    reg_timers_stop_hplmn_cfl_search_period_timer();
    reg_cfl_hplmn_search_counter = 0;
  }

  reg_mode_limited_service_domain = req_service_domain;

  sys_plmn_undefine_plmn_id(&plmn);

  /* Extract the RAT list into limited service RAT order list */
  reg_mode_get_3gpp_rat_capabilty(&reg_mode_limited_service_rat_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
      ,TRUE
#endif                                  
     );

  if(status == POLICYMAN_STATUS_SUCCESS)
  {
      if ((specialization != SYS_SPECIAL_SBM_RAT_SEARCH_ORDER) && 
         (type != REG_CM_SERVICE_REQ_FULL_RAT_MODE))
      {           
#ifdef FEATURE_LTE
          /* Move LTE to end if there is no emergency call pending */
#ifdef FEATURE_LTE_REL9
          if(reg_bit_mask_check(additional_info,
              (byte)SYS_ADDTNL_INFO_EMERG_CALL) == FALSE)
#endif
          {
              for (i= 0; i < reg_mode_limited_service_rat_order.num_items; i++)
              {
                  if(reg_mode_limited_service_rat_order.rat_list[i] == SYS_RAT_LTE_RADIO_ACCESS)
                  {
                      for(j = i+1; j < reg_mode_limited_service_rat_order.num_items; j++)
                      {
                          reg_mode_limited_service_rat_order.rat_list[j-1] = 
                              reg_mode_limited_service_rat_order.rat_list[j];
                      }
                      reg_mode_limited_service_rat_order.rat_list[j-1] = SYS_RAT_LTE_RADIO_ACCESS;
                      break;
                  }
              }
          }
#endif
      }
  }
#ifdef FEATURE_LTE
  if((reg_bit_mask_check(additional_info,
                         (byte)SYS_ADDTNL_INFO_EMERG_CALL) == TRUE) &&
     (reg_bit_mask_check(additional_info,
                         (byte)SYS_ADDTNL_INFO_RESTART_PLMN_SELECTION) == TRUE))
  {
    initial_rat = reg_mode_limited_service_rat_order.rat_list[0];
  }
  else
#endif
  if (((reg_state_service_status_get() == SYS_SRV_STATUS_SRV) ||
       (reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED) ||
       (reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED_REGIONAL)) &&
       (reg_mode_is_rat_present_in_rat_pri_list(
              reg_state_active_rat_get(), rat_pri_list_info, TRUE))
#ifdef FEATURE_LTE
       && ((reg_state_active_rat_get() != SYS_RAT_LTE_RADIO_ACCESS) 
#ifdef FEATURE_LTE_REL9
           ||
           (reg_bit_mask_check(additional_info,
                         (byte)SYS_ADDTNL_INFO_EMERG_CALL) == TRUE)
#endif
          )
#endif
     )
  {
    initial_rat = reg_state_active_rat_get();
  }
  else if((reg_state_service_status_get() == SYS_SRV_STATUS_NO_SRV) &&
          (reg_state_camped_ind_received_get() == TRUE) && 
#ifdef FEATURE_LTE_REL9
           (reg_bit_mask_check(additional_info,
                         (byte)SYS_ADDTNL_INFO_EMERG_CALL) == TRUE) &&
#endif
          (reg_mode_is_rat_present_in_rat_pri_list(reg_state_active_rat_get(), 
                                                   rat_pri_list_info, TRUE) == TRUE))
  {
    initial_rat = reg_state_active_rat_get();
  }
#if defined(FEATURE_LTE) && defined(FEATURE_LTE_REL9)
  else if((type == REG_CM_SERVICE_REQ_SUITABLE_SEARCH) &&
          (reg_state_rlf_rat_get() == SYS_RAT_LTE_RADIO_ACCESS) &&
          (reg_mode_is_rat_present_in_rat_pri_list(reg_state_rlf_rat_get(), 
                                                   rat_pri_list_info, TRUE) == TRUE))
  {
    initial_rat = reg_state_rlf_rat_get();
    plmn = reg_state_rlf_plmn_get();
  }
#endif
  else if (reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_UMTS_RADIO_ACCESS, rat_pri_list_info, TRUE) && (reg_nv_is_wcdma_freq_lock_enabled() == TRUE))
  {
    initial_rat = SYS_RAT_UMTS_RADIO_ACCESS;
    reg_mode_limited_service_rat_order.num_items = 0;
  }
  else
  {
      initial_rat = reg_mode_limited_service_rat_order.rat_list[0];
      if(status == POLICYMAN_STATUS_SUCCESS)
      {
          if (specialization != SYS_SPECIAL_SBM_RAT_SEARCH_ORDER)
          {
              if((reg_mode_service_available_rat_get() != initial_rat) &&
                  (reg_mode_service_available_rat_get() != SYS_RAT_NONE) && 
                  (reg_mode_is_rat_present_in_rat_pri_list(
                  reg_mode_service_available_rat_get(), rat_pri_list_info, TRUE))
#ifdef FEATURE_SGLTE
                  && !(REG_SGLTE_DUAL_STACK && reg_mode_service_available_rat_get() == SYS_RAT_GSM_RADIO_ACCESS)
#endif
#ifdef FEATURE_LTE
                 && (reg_mode_service_available_rat_get() != SYS_RAT_LTE_RADIO_ACCESS)
#endif
                  )
              {
                  initial_rat = reg_mode_service_available_rat_get();
              }
          }
      }
  } 
  //Remove initial rat from rat_list. So that it in not tried next time
  for (i= 0; i < reg_mode_limited_service_rat_order.num_items; i++)
  {
    if(reg_mode_limited_service_rat_order.rat_list[i] == initial_rat)
    {
      for(j = i+1; j < reg_mode_limited_service_rat_order.num_items; j++)
      {
        reg_mode_limited_service_rat_order.rat_list[j-1] = 
           reg_mode_limited_service_rat_order.rat_list[j];
      }
      break;
    }
  }
  if(reg_mode_limited_service_rat_order.num_items>0)
  {
    reg_mode_limited_service_rat_order.num_items--;  
  }

  //Keep track of the initial RAT chosen
  reg_mode_limited_service_initial_rat = initial_rat;

  if ( 
#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
       !(reg_mode_is_multimode_enabled(reg_mode_rat_pri_list_get())) &&
#endif
       (type == REG_CM_SERVICE_REQ_FULL_RAT_MODE) &&
       (reg_mode_scan_scope_get() == SYS_SCAN_SCOPE_FULL_BAND) &&
       (cs_for_request == FALSE) )
  {
     /* Create the acq db rat list and enable all the RATs (L/W/G/T) */
     reg_mode_limited_service_get_acq_db_rat_list(&reg_mode_limited_service_acq_db_rat_order, rat_pri_list_info);

     if (SYS_RAT_LTE_RADIO_ACCESS == initial_rat)
     {
       scan_info.use_timer = TRUE;
       scan_info.scan_time = (uint32)reg_nv_interlace_scan_timer_value_get();
     }
  }
  else
  {
     reg_mode_limited_service_acq_db_rat_order.num_items = 0;
  }


  reg_send_mmr_reg_req
  (
    (reg_mode_network_selection_mode_get() ==
    SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION ?
    SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION :
    SYS_NETWORK_SELECTION_MODE_LIMITED),  /* network_selection_mode */
    plmn,                                 /* plmn                   */
    reg_mode_limited_service_domain,      /* req_service_domain     */
    reg_mode_mode_pref_get(),             /* mode_pref              */
    cs_for_request,                       /* cs_for_request         */
    ps_for_request,                       /* ps_for_request         */

    initial_rat

#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref                             /* band_pref              */
#endif
    ,rat_pri_list_info

    ,serv_req_type
#ifdef FEATURE_LTE
    ,sms_only,
     additional_info
#endif
#ifdef FEATURE_FEMTO_CSG
    ,SYS_CSG_ID_INVALID
#endif 
    ,scan_info
  );
}

static void reg_mode_limited_service_interlace_acq_db_req
(
  void
)
{
  sys_radio_access_tech_e_type rat = SYS_RAT_GSM_RADIO_ACCESS;
  sys_plmn_id_s_type           plmn;
  mmr_reg_interlace_scan_s_type    scan_info;
  uint8                        index;
  int i;
  sys_rat_pri_list_info_s_type  *acq_db_rat_pri_list_info = NULL;
  mmr_reg_req_e_type  serv_req_type = MMR_REG_REQ_FULL_RAT_MODE;

  scan_info.new_scan  = TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

#ifndef FEATURE_MODEM_HEAP
  acq_db_rat_pri_list_info = (sys_rat_pri_list_info_s_type *)gs_alloc(sizeof(sys_rat_pri_list_info_s_type));
#else
  acq_db_rat_pri_list_info = (sys_rat_pri_list_info_s_type *)modem_mem_calloc(1, sizeof(sys_rat_pri_list_info_s_type), MODEM_MEM_CLIENT_NAS);
#endif
  reg_check_for_null_ptr((void*)acq_db_rat_pri_list_info);
                     
  /*Copy original RAT list into acq db RAT list */
  memscpy(acq_db_rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type), reg_mode_rat_pri_list_get(), sizeof(sys_rat_pri_list_info_s_type));

  if ( (reg_mode_limited_service_rat_order.num_items > 0) &&
#ifdef FEATURE_DUAL_SIM
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat, (sys_modem_as_id_e_type)reg_as_id) == TRUE) 
#else
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat) == TRUE)
#endif
     )
  {
    reg_mode_remove_rat_from_3gpp_rat_list(&reg_mode_limited_service_acq_db_rat_order,reg_mode_limited_service_rat_order.rat_list[0]);
  }

  rat = reg_mode_limited_service_acq_db_rat_order.rat_list[0];

  //remove rat from rat_search_order
  for (index = 1; index < reg_mode_limited_service_acq_db_rat_order.num_items; index++)
  {
     reg_mode_limited_service_acq_db_rat_order.rat_list[index-1] = 
          reg_mode_limited_service_acq_db_rat_order.rat_list[index];
  }
  reg_mode_limited_service_acq_db_rat_order.num_items--;

  /* update the acq db RAT list BST band cap before sending ACQ DB request. Enable all L/W/G/T RATs.*/
  for(i = 0; i < (int)acq_db_rat_pri_list_info->num_items; i++)
  {
    acq_db_rat_pri_list_info->priority_list_info[i].bst_rat_acq_required = TRUE;
    acq_db_rat_pri_list_info->priority_list_info[i].bst_band_cap = acq_db_rat_pri_list_info->priority_list_info[i].band_cap;
  }

  /* Set use timer flag as TRUE to indicate LRRC that it is interlace scan */
  if (
#ifdef FEATURE_DUAL_SIM
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat, (sys_modem_as_id_e_type)reg_as_id) == FALSE) 
#else
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat) == FALSE)
#endif
     )
  {
    scan_info.new_scan = FALSE;
    scan_info.use_timer = TRUE;
  }

  reg_mode_scan_scope_set(SYS_SCAN_SCOPE_ACQ_DB); 
  sys_plmn_undefine_plmn_id(&plmn);

  MSG_HIGH_DS_1(REG_SUB, "=REG= Sending interlace ACQ DB scan request for RAT %d", rat);

  reg_send_mmr_reg_req
  (
        (reg_mode_network_selection_mode_get() ==
        SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION ?
        SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION :
        SYS_NETWORK_SELECTION_MODE_LIMITED),  /* network_selection_mode */
        plmn,                                 /* plmn                   */
        reg_mode_limited_service_domain,      /* req_service_domain     */
        reg_mode_mode_pref_get(),             /* mode_pref              */
        reg_mode_cs_for_request_get(),        /* cs_for_request         */
        reg_mode_ps_for_request_get(),        /* ps_for_request         */
        rat                                  /* rat                    */
#ifdef FEATURE_GSM_BAND_PREF
        ,reg_mode_band_pref_get()              /* band_pref              */
#endif
#ifdef FEATURE_RAT_PRIORITY_LIST
        ,acq_db_rat_pri_list_info              /*ue_capabilty*/
#endif 
        ,serv_req_type
#ifdef FEATURE_LTE
       ,reg_mode_sms_only_get()
       ,reg_mode_additional_info_get()
#endif
#ifdef FEATURE_FEMTO_CSG
       ,SYS_CSG_ID_INVALID
#endif 
       ,scan_info
  );

  reg_mode_scan_scope_set(SYS_SCAN_SCOPE_FULL_BAND); 

#ifndef FEATURE_MODEM_HEAP
  gs_free(acq_db_rat_pri_list_info);
#else
  modem_mem_free(acq_db_rat_pri_list_info, MODEM_MEM_CLIENT_NAS);
#endif
  acq_db_rat_pri_list_info = NULL; 
 
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
  sys_radio_access_tech_e_type rat = SYS_RAT_GSM_RADIO_ACCESS;
  mmr_reg_interlace_scan_s_type    scan_info;
  sys_plmn_id_s_type           plmn;
  boolean                      finished_plmn_selection = TRUE;
  uint8                        index;
  mmr_reg_req_e_type  serv_req_type = MMR_REG_REQ_NORMAL;  
  (void) available_plmn_list_p;

  scan_info.new_scan =  TRUE;
  scan_info.use_timer = FALSE;
  scan_info.scan_time = 0;

  if(reg_state_get_last_cm_service_req_type() == REG_CM_SERVICE_REQ_FULL_RAT_MODE)
  {
    serv_req_type = MMR_REG_REQ_FULL_RAT_MODE;
  } 
  

  /* filter out GSM from  main stack, on hybrid stack anyway after split, CM has to send another
  service req to activate hybrid stack, so no need to copy on hybrid.*/
#ifdef FEATURE_SGLTE
  if (REG_SGLTE_DUAL_STACK && (reg_as_id == reg_ps_stack_id))
  {
    reg_mode_remove_rat_from_3gpp_rat_list(&reg_mode_limited_service_rat_order,SYS_RAT_GSM_RADIO_ACCESS);
  }
#endif

  /* Add the RAT back to top of the list if full band scan is not completed (currently only applicable to LTE)*/
  if ( 
#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
       !(reg_mode_is_multimode_enabled(reg_mode_rat_pri_list_get())) &&
#endif
       (reg_state_get_last_cm_service_req_type() == REG_CM_SERVICE_REQ_FULL_RAT_MODE) &&
       (reg_send_last_reg_req_scan_scope_get() == SYS_SCAN_SCOPE_FULL_BAND) &&
#ifdef FEATURE_DUAL_SIM
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat, (sys_modem_as_id_e_type)reg_as_id) == FALSE) 
#else
       (reg_mode_get_rat_search_done_flag(reg_mode_limited_service_initial_rat) == FALSE)
#endif
     )
  {
     if (reg_mode_limited_service_rat_order.num_items < REG_MODE_MAX_3GPP_RAT)
     {
        for (index = reg_mode_limited_service_rat_order.num_items; index > 0; index--)
        {
          reg_mode_limited_service_rat_order.rat_list[index] = reg_mode_limited_service_rat_order.rat_list[index - 1];
        }

        reg_mode_limited_service_rat_order.rat_list[0] = reg_mode_limited_service_initial_rat;
        reg_mode_limited_service_rat_order.num_items++;
     }
  }

  /*
  ** The MS found service on the initial RAT. 
  */
  
  if (service_state.service_status != SYS_SRV_STATUS_NO_SRV
#ifdef FEATURE_LTE
       && //if UE camps on LTE cell without emc support and e911 call is pending then find service on next rat
       ((reg_bit_mask_check(reg_mode_additional_info_get(),
                           (byte)SYS_ADDTNL_INFO_EMERG_CALL) == FALSE) ||
         service_state.active_rat != SYS_RAT_LTE_RADIO_ACCESS ||
         service_state.eps_nw_feature_support.emc_bs_supported == TRUE)
#endif
      )
  {
    if ( (service_state.service_status == SYS_SRV_STATUS_SRV) &&
         (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION) &&
         (service_state.roaming_ind == SYS_ROAM_STATUS_ON 
#if defined(FEATURE_EQUIVALENT_HPLMN)
         || !reg_mode_is_camped_plmn_is_higher_priority_ehplmn(reg_state_service_plmn_get())
#endif
         ) )
    {
      reg_timers_start_hplmn_search_period_timer();
    }
    reg_mode_limited_service_rat_order.num_items = 0;
    reg_mode_limited_service_acq_db_rat_order.num_items = 0;
  }
  else if (reg_mode_limited_service_acq_db_rat_order.num_items > 0)
  {
     reg_mode_limited_service_interlace_acq_db_req();
     finished_plmn_selection = FALSE;
  }
  /*
  ** The MS did not find service on the initial RAT.
  */  
  else if (reg_mode_limited_service_rat_order.num_items > 0)
  {
    rat = reg_mode_limited_service_rat_order.rat_list[0];

    //remove rat from rat_search_order
    for (index = 1; index < reg_mode_limited_service_rat_order.num_items; index++)
    {
          reg_mode_limited_service_rat_order.rat_list[index-1] = 
                 reg_mode_limited_service_rat_order.rat_list[index];
    }
    reg_mode_limited_service_rat_order.num_items--;

     if ( 
#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
         !(reg_mode_is_multimode_enabled(reg_mode_rat_pri_list_get())) &&
#endif
          (reg_state_get_last_cm_service_req_type() == REG_CM_SERVICE_REQ_FULL_RAT_MODE) &&
          (reg_mode_scan_scope_get() == SYS_SCAN_SCOPE_FULL_BAND) &&
          (reg_mode_cs_for_request_get() == FALSE) )
    {
      //Reset the ACQ DB RAT priority list
      reg_mode_limited_service_get_acq_db_rat_list(&reg_mode_limited_service_acq_db_rat_order, reg_mode_rat_pri_list_get());

      if (rat == SYS_RAT_LTE_RADIO_ACCESS)
      {
        scan_info.use_timer = TRUE;
        scan_info.scan_time = (uint32)reg_nv_interlace_scan_timer_value_get();
        
        if (rat == reg_mode_limited_service_initial_rat)
        {
           scan_info.new_scan = FALSE;
        }
      }
    }

    reg_mode_limited_service_initial_rat = rat;

    sys_plmn_undefine_plmn_id(&plmn);

    reg_send_mmr_reg_req
    (
           (reg_mode_network_selection_mode_get() ==
           SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION ?
           SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION :
           SYS_NETWORK_SELECTION_MODE_LIMITED),  /* network_selection_mode */
           plmn,                                 /* plmn                   */
           reg_mode_limited_service_domain,      /* req_service_domain     */
           reg_mode_mode_pref_get(),             /* mode_pref              */
           reg_mode_cs_for_request_get(),        /* cs_for_request         */
           reg_mode_ps_for_request_get(),        /* ps_for_request         */
           rat                                  /* rat                    */
   #ifdef FEATURE_GSM_BAND_PREF
           ,reg_mode_band_pref_get()              /* band_pref              */
   #endif
           ,reg_mode_rat_pri_list_get()      /*ue_capabilty*/
           ,serv_req_type
   #ifdef FEATURE_LTE
          ,reg_mode_sms_only_get()
          ,reg_mode_additional_info_get()
   #endif
   #ifdef FEATURE_FEMTO_CSG
          ,SYS_CSG_ID_INVALID
   #endif 
          ,scan_info
    );
    
    finished_plmn_selection = FALSE;    
  }

  return finished_plmn_selection;
}




/*==============================================================================

FUNCTION NAME

  number

==============================================================================*/

static reg_mode_e_type number ( void )
{
  return REG_MODE_LIMITED_SERVICE;
}


/*==============================================================================

                               DATA DECLARATIONS

==============================================================================*/

reg_mode_s_type reg_mode_limited_service = 
{
  reg_mode_unexpected_start_automatic_plmn_selection,
  reg_mode_unexpected_start_higher_priority_plmn_selection,
  start_limited_service,
  reg_mode_unexpected_start_manual_plmn_selection,
  reg_mode_unexpected_start_plmn_selection,
  reg_mode_unexpected_start_plmn_selection_roaming_plmn,
  reg_mode_unexpected_start_plmn_selection_on_specified_plmn,
  reg_mode_unexpected_start_foreground_plmn_search,
  reg_mode_unexpected_start_preferred_plmn_search,
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
void get_dump_reg_mode_limited_service_vars (reg_debug_data_s_type* ptr,
                                             reg_as_id_e_type as_id)
{
    ASSERT (ptr != NULL);
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
    ptr->reg_mode_limited_service_domain_debug = 
        reg_mode_limited_service_domain_sim[as_id];
    ptr->reg_mode_limited_service_rat_order_debug = 
        reg_mode_limited_service_rat_order_sim[as_id];
#else
    ptr->reg_mode_limited_service_domain_debug = 
        reg_mode_limited_service_domain;
    ptr->reg_mode_limited_service_rat_order_debug = 
        reg_mode_limited_service_rat_order;
#endif
}
#endif
