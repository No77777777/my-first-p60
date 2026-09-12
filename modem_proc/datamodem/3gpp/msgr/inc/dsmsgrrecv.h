#ifndef DSMSGRRECV_H
#define DSMSGRRECV_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

               M S G R   I N T E R F A C E   F U N C T I O N S 

                            H E A D E R    F I L E

DESCRIPTION
  This file defines definitions for use by clients of dsmsgr_send and receive
  modules. This header file is intended to be used internal to the 3G Dsmgr 
  module only.


  Copyright (c) 2015 - 2019 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/msgr/inc/dsmsgrrecv.h#3 $
 
  $DateTime: 2021/06/01 08:54:28 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/30/15   pvb     Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "msgr.h"
#include "emm_irat_if_msg.h"
#include "dstask_v.h"

#ifdef FEATURE_DATA_LTE
#include "lte_pdcp_msg.h"
#include "lte_tlb_msg.h"
#include "lte_tlb.h"
#include "ds_eps_msg.h"
#include "lte_rlc_msg.h"
#include "lte_mac_msg.h"
#include "lte_mac_ind.h"
#include "lte_mac.h"
#include "emm_ext_msg.h"
#include "lte_cphy_msg.h"
#include "lte_rrc_ext_msg.h"
#include "lte_cphy_rssi_msg.h"
#endif

#include "dsmsgrrecv_msg.h"
#include "dsmsgrrecv.h"
#include "ds_3gpp_msg.h"
#include "ds3gpp_ext_msg.h"
#include "data_msg.h"

#include "rex.h"
#include "timer.h"
#include "rcinit.h"

#ifdef FEATURE_TDSCDMA
#include "tds_rrc_ext_if.h"
#endif  /* FEATURE_TDSCDMA */

#include "ds_qmi_dms.h"
#include "policyman_msg.h"

#include "ds3gflowmgr.h"
#include "cm_msgr_msg.h"
#include "qmi_mmode_msgr_msg.h"
#include "wcn_coex_mgr.h"

#include "ds_lowlatency_ext_msg.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  common cback function pointer invoked when DS MSGR receives message info
---------------------------------------------------------------------------*/
typedef void (* dsmsgrrecv_cback_f_ptr)
(
  const msgr_hdr_struct_type *,
  void *
); 

/*---------------------------------------------------------------------------
  Union of all the messages that DS can receive
---------------------------------------------------------------------------*/
typedef union {
  /* This will be the only info in case the message is a SYNC MSG*/
  msgr_hdr_s                         hdr;

#ifdef FEATURE_DATA_LTE

  /* These messages will be received from EMM */
  emm_tau_complete_ind_type             emm_tau_complete_ind;
  cm_3gpp_service_status_s_type         cm_service_status_ind;

  /* These messages will be received from PDCP */
  lte_pdcpul_rab_register_cnf_msg_s     ds_lte_pdcpul_rab_register_cnf_msg;
  lte_pdcpdl_rab_register_cnf_msg_s     ds_lte_pdcpdl_rab_register_cnf_msg;
  lte_pdcp_rab_deregister_cnf_msg_s     ds_lte_pdcp_rab_deregister_cnf_msg;
  lte_tlb_lb_close_tl_ind_s             ds_lte_tlb_lb_close_tl_ind_s;
  emm_t3402_changed_ind_s_type          ds_lte_emm_t3402_changed_ind;
  emm_reset_apn_switch_ind_type         ds_lte_reset_attach_apn_ind;
  lte_cphy_ca_event_ind_s               ds_lte_cphy_ca_event_ind;
  lte_cphy_ca_event_ext_ind_s           ds_lte_cphy_ca_event_ext_ind;
#ifdef FEATURE_DATA_4_5G
  lte_cphy_mimo_mod_event_ind_s         ds_lte_cphy_mimo_mod_event_ind;
#endif
  lte_cphy_tx_power_report_info_cnf_s   ds_lte_cphy_tx_power_report_info_cnf;
  lte_cphy_achievable_tput_status_ind_s ds_lte_cphy_achievable_tput_status_ind;
  lte_pdcpul_pkt_discard_msg_s	        ds_lte_pdcpul_pkt_discard_msg;
#endif /* FEATURE_DATA_LTE */

#ifdef FEATURE_DATA_EMBMS
  /* These messages will be received from LTE RRC*/
  lte_rrc_embms_status_change_ind_s   ds_lte_rrc_embms_status_change_ind;
  lte_rrc_embms_act_tmgi_list_ind_s   ds_lte_rrc_embms_act_tmgi_list_ind;
  lte_rrc_embms_avail_tmgi_list_ind_s ds_lte_rrc_embms_avail_tmgi_list_ind;
  lte_rrc_embms_deact_tmgi_ind_s      ds_lte_rrc_embms_deact_tmgi_ind;
  lte_rrc_embms_deact_tmgi_cnf_s      ds_lte_rrc_embms_deact_tmgi_cnf;
  lte_rrc_embms_act_tmgi_cnf_s        ds_lte_rrc_embms_act_tmgi_cnf;
  lte_rrc_embms_oos_warn_ind_s        ds_lte_rrc_embms_warn_tmgi_list_ind;
  lte_rlcdl_embms_wm_register_cnf_msg_s    ds_lte_rlcdl_embms_wm_register_cnf_msg_s;
  lte_rlcdl_embms_wm_deregister_cnf_msg_s  ds_lte_rlcdl_embms_wm_deregister_cnf_msg_s;
  ds_3gpp_embms_content_desc_control_msg_s ds_3gpp_embms_content_desc_control_msg;
  lte_rrc_embms_service_interest_ind_s    ds_lte_rrc_embms_service_interest_ind;
#endif /* FEATURE_DATA_EMBMS */
  /* These messages will be received from 3gpp mode handler */
  ds_3gpp_phys_link_flow_disable_msg_s ds_3gpp_phys_link_flow_disable_msg;
  ds_3gpp_phys_link_flow_enable_msg_s  ds_3gpp_phys_link_flow_enable_msg;
  cxm_coex_desense_ind_s               ds_coex_desense_ind;
  qmi_dms_policyman_cfg_update_ind_type_msg_s  policyman_cfg_update_ind_msg;
  cm_plmn_list_chngd_ind_s_type        plmn_list_change_ind;

#if defined(FEATURE_ELS_WIFIMEAS_LTE_U_PH1) || defined(FEATURE_ELS_LWA)
  lte_rrc_wifi_meas_cfg_ind_s            rrc_wifi_meas_cfg_ind;
#endif 

#ifdef FEATURE_DATA_RAVE_SUPPORT
   lte_mac_ul_tput_info_start_cnf_msg_s  ds_lte_mac_ul_tput_info_start_cnf_msg;
   lte_mac_ul_tput_info_ind_s            ds_lte_mac_ul_tput_info_ind;
#endif
  /* This message will be received from CM */ 
  mmode_qmi_voice_conf_participants_info_cmd_msg_type
                                       ds_mmode_qmi_voice_conf_participants_info_cmd_msg;

  cxm_coex_lteu_rrc_info_s               ds_lteu_rrc_info_msg;
  cm_mid_srvcc_ho_comp_list_s_type       cm_mid_srvcc_ho_comp_list;

  ds_llm_get_ard_llm_req_s               ds_llm_get_ard_llm_req_msg;
} dsmsgrrcv_msg_u;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION       DSMSGRRCV_REG_HDLR

DESCRIPTION    API invoked by DS clients to register a callback for a UMID 
 
PARAMETERS     MSGRTYPE: UMID for which cback is to be registered 
               Func_ptr: Cback to be invoked once message is sent for that UMID 
 
DEPENDENCIES   NONE

RETURN VALUE   NONE 

SIDE EFFECTS   NONE
===========================================================================*/
void dsmsgrrecv_reg_cb 
(
  msgr_umid_type                 msgrtype, 
  dsmsgrrecv_cback_f_ptr         cback_func_ptr,
  boolean                        global_msg,
  void                          *data_ptr
);

/*===========================================================================
FUNCTION       DSMSGRRCV_DEREG_HDLR

DESCRIPTION    API invoked by DS clients to deregister a callback for a UMID 
 
PARAMETERS     MSGRTYPE: UMID for which cback is to be deregistered 
 
DEPENDENCIES   NONE

RETURN VALUE   NONE 

SIDE EFFECTS   NONE
===========================================================================*/
void dsmsgrrecv_dereg_cb 
(
   msgr_umid_type msgrtype
);

/*===========================================================================
FUNCTION       DSMSGRRCV_INIT

DESCRIPTION    API invoked by pthread function during MSGR task initialization 

PARAMETERS     None

DEPENDENCIES   NONE

RETURN VALUE   NONE 

SIDE EFFECTS   NONE
===========================================================================*/
void dsmsgrrcv_init 
(
  void
);

#endif /* DSMSGRRECV_H */

