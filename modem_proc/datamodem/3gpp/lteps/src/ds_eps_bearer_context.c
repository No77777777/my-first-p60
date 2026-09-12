
/*!
  @file
  ds_eps_bearer_context.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps/src/ds_eps_bearer_context.c#11 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/30/14   ss      New event added: IFACE_EXTENDED_IP_CONFIG_EX_EV
6/24/14    vs      DS decrement TMR priority to low for VOLTE with QCI=1
11/05/13   ss      Delegating RAT specific handling during call_end.
08/08/12   ss      Optimization to trigger Dynamic BP only when threshold
                   num of acks are received.
07/04/12   ss      Freeing the BP timer handle in deact_cnf and avoiding
                   crash due to using stale timer handle.
03/22/12   ss      Addition of RAB_FAILURE and ESM local cause codes.
01/02/12   msh     Coan: Feature cleanup
12/21/11   ss      Added support for watermark tuning.
11/17/11   sa      Added support for V6 Loopback Mode B.
09/09/11   rr      Added support for fallback from Dual IP to single IP when nw rejects 
                   with specific cause codes in UMTS mode
03/16/11   ttv     Fixed compilation warnings.
03/10/11   ttv     Added support for LTE watermark tracing.
02/10/11   ss      Added support for configurable RAB_REESTAB RETRY.
01/07/11   ss      Changed watermark levels.
09/07/10   vk      Allow TLB mode A to be closed without intervening OPEN cmd
07/10/10   vk      Support for data path bridging for sdio ports
05/28/10   vrk     Setting A2 bridge when resuming on LTE
05/17/10   hs      Added APN disable and PDN inactivity feature support.
04/23/10   vk      Use of MSGR for UL flow control event processing in LTE
04/21/10   sa      Added support for Loopback Mode B.
04/03/10   vk      Added support for MSG2.0 based logging
04/02/10   vrk     UMTS interRAT support
02/04/10   vd      Using the changed flow functions.
01/27/10   vd      Using changed flow manager function name.
01/23/10   vk      Added helper macros for virtual function dispatch
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include <stringl/stringl.h>

#ifdef FEATURE_DATA_LTE

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "ds_eps_bearer_context.h"
#include "ds_eps_bearer_context_ex.h"
#include "ds_eps_throughput_hdlr.h"
#include "ds_3gpp_nv_manager.h"
#include "ds_3gppi_utils.h"
#include "ds_3gpp_loopback_hdlr.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_pdn_context_ex.h"
#include "ds_flow_control.h"
#include "ds_3gpp_flow_manager.h"
#include "ds_eps_bearer_flow_manager.h"
#include "ds3gmgr.h"
#include "ds3gsubsmgr.h"
#include "ds3gflowmgr.h"
#include "ds3gmmgsdiif.h"
#include "ds3gprofile_validator.h"
#include "ds3gtputmgr.h"
#include "ds3gmgr.h"
#include "ds3gutil.h"
#include "ds3gmshif.h"
#include "dsutil.h"
#include "ds_3gpp_hdlr.h"
#include "lte_pdcp_msg.h"
#include "dsmsgrrecv_msg.h"
#include "ran.h"
#include "ds_eps_tlb.h"
#include "sio.h"
#include "ds_3gpp_a2.h"
#include "ds_eps_msg.h"
#include "lte_pdcp_ext_api.h"
#include "lte.h"
#include "lte_misc.h"
#include "ds_eps_pdn_context.h"
#include "ds_3gpp_apn_switch_mgr.h"
#include "dsm_queue.h"
#include "pstimer.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds3gpp_ext_msg.h"
#include "ds_umts_bearer_context.h"
#include "ds_3gpp_apn_table.h"
#include "lte_cphy_msg.h"
#include "lte_rrc_ext_msg.h"
#include "lte_rlc_msg.h"
#include "lte_rlc_ext_api.h"
#include "ps_stat_iface.h"
#include "ds_3gpp_pdn_context.h"

#include "ds3gdsdif.h"

#include "ds_3gpp_pdn_limit_hdlr.h"

#include "emm_irat_if_msg.h"
#include "ds_dsd_apm_ext_i.h"
#include "ds_3gpp_low_prio_sig_hdlr.h"
#include "ds_3gpp_pdn_throt_spec.h"
#include "ds_3gpp_srv_req_throt_sm.h"
#include "ds_3gpp_network_override_hdlr.h"
#include "ds_3gpp_rab_reestab_sm.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
#include "ds_dsd_ext_i.h"
#include "ds_3gpp_link_switch_mgr.h"
#include "ds_3gpp_remote_switch_mgr.h"
#include "ds_3gpp_tput_hdlr.h"
#include "ds_3gpp_pco_epco_hdlr.h"
#include "ps_pkt_info_utils.h"
#include "ps_phys_link.h"
/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define MBPS5   5000000
#define MBPS10  10000000
#define MBPS25  25000000
#define MBPS50  50000000
#define MBPS75  75000000
#define MBPS100 100000000
#define MBPS150 150000000
#define MBPS300 300000000
#define MBPS450 450000000
#ifdef  FEATURE_XMOD
  #define MBPS110   110000000
  #define MBPS220   220000000
  #define MBPS230   230000000
  #define MBPS310   310000000
  #define MBPS320   320000000
  #define MBPS400   400000000
  #define MBPS600   600000000
  #define MBPS810   810000000
  #define MBPS1100  1100000000
  #define MBPS1250  1250000000
  #define MBPS1420  1420000000
  #define MBPS1700  1700000000
  #define MBPS2100  2100000000
  #define MBPS2200  2200000000
  #define MBPS4000  4000000000
  #define MBPS9600  9600000000
  #define MBPS14000 14000000000
  #define MBPS26000 26000000000
#endif

ps_iface_ioctl_data_bearer_rate ds_eps_bearer_rate[LTE_UE_CLASSES_MAX] =
{ 
  /*CAT 0*/
  {-1,     -1,      -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_BPS, -1,   -1,  -1, -1, -1, -1}},
  /*CAT 1*/
  {MBPS5,  MBPS10,   -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 5,   10,   -1, -1, -1, -1}},
  /*CAT 2*/
  {MBPS25, MBPS50,  -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 25,  50,  -1, -1, -1, -1}},
  /*CAT 3*/
  {MBPS50, MBPS100, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 50,  100, -1, -1, -1, -1}},
  /*CAT 4*/
  {MBPS50, MBPS150, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 50,  150, -1, -1, -1, -1}},
  /*CAT 5*/
  {MBPS75, MBPS300, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 75,  300, -1, -1, -1, -1}},
  /*CAT 6*/
  {MBPS50, MBPS300, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 50,  300, -1, -1, -1, -1}},
  /*CAT 7*/
  {MBPS100,MBPS300, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 100, 300, -1, -1, -1, -1}},
  /*CAT 8*/
  {-1,     -1,      -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_BPS,  -1,  -1,  -1, -1, -1, -1}},
  /*CAT 9*/
  {MBPS50, MBPS450, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 50,  450, -1, -1, -1, -1}},
  /*CAT 10*/
  {MBPS100,MBPS450, -1, -1, -1, -1, {PS_IFACE_DATA_RATE_TYPE_MBPS, 100, 450, -1, -1, -1, -1}},
#ifdef FEATURE_XMOD
  /*CAT 11*/
  {MBPS50,    MBPS600,   -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 50,    600,   -1, -1, -1, -1}},
  /*CAT 12*/
  {MBPS100,   MBPS600,   -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 100,   600,   -1, -1, -1, -1}},
  /*CAT 13*/
  {MBPS150,   MBPS400,   -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 150,   400,   -1, -1, -1, -1}},
  /*CAT 14*/
  {INT32_MAX, INT32_MAX, -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 9600,  4000,  -1, -1, -1, -1}},
  /*CAT 15*/
  {MBPS230,   MBPS810,   -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 230,   810,   -1, -1, -1, -1}},
  /*CAT 16*/
  {MBPS110,   MBPS1100,  -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 110,   1100,  -1, -1, -1, -1}},
  /*CAT 17*/
  {INT32_MAX, INT32_MAX, -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 2200,  26000, -1, -1, -1, -1}},
  /*CAT 18*/
  {MBPS220,   MBPS1250,  -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 220,   1250,  -1, -1, -1, -1}},
  /*CAT 19*/
  {INT32_MAX, MBPS1700,  -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 14000, 1700,  -1, -1, -1, -1}},
  /*CAT 20*/
  {MBPS320,   MBPS2100,  -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 320,   2100,  -1, -1, -1, -1}},
  /*CAT 21*/
  {MBPS310,   MBPS1420,  -1, -1, -1, -1,  {PS_IFACE_DATA_RATE_TYPE_MBPS, 310,   1420,  -1, -1, -1, -1}},
#endif
};

/*---------------------------------------------------------------------------
  Below table is used to determine the LTE UL watermark levels based on
  ue category. LO = 160000, HI = 265000, DNE = 345000, DNE QCNT = 690
  are used as default values (50 Mbps rates). New increases are made on top
  of these levels.
---------------------------------------------------------------------------*/
uint32 ds_eps_bearer_ul_wm_level_tbl[LTE_UE_CLASSES_MAX][UL_WM_ARRAY_SIZE] =
{
  /* LO, HI, DNE, DNE QCNT */
  { 160000, 265000, 345000, 690 }, /* CAT 0 */
  { 160000, 265000, 345000, 690 }, /* CAT 1 */
  { 160000, 265000, 345000, 690 }, /* CAT 2 */
  { 160000, 265000, 345000, 690 }, /* CAT 3 */
  { 160000, 265000, 345000, 690 }, /* CAT 4 */
  { 160000, 265000, 345000, 690 }, /* CAT 5 */
  { 160000, 265000, 345000, 690 }, /* CAT 6 */
  { 220000, 325000, 405000, 810 }, /* CAT 7 */
  { 160000, 265000, 345000, 690 }, /* CAT 8 */
  { 160000, 265000, 345000, 690 }, /* CAT 9 */
  { 220000, 325000, 405000, 810 }, /* CAT 10 */
#ifdef FEATURE_XMOD
  { 160000, 265000, 345000, 690 }, /* CAT 11 */
  { 250000, 355000, 435000, 870 }, /* CAT 12 */
  { 250000, 355000, 435000, 870 }, /* CAT 13 */
#endif /* FEATURE_XMOD */
};

/*global variable to hold LTE ue category index, per subs id*/
static uint8 ds_eps_bearer_cntxt_ue_cat_id[DS3GSUBSMGR_SUBS_ID_MAX] = {0};

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/


/*! @brief EPS bearer context table */
ds_eps_bearer_context_s eps_bearer_context_tbl[DS_3GPP_MAX_BEARER_CONTEXT] = {{NULL}};

extern ds_pdn_context_s ds_pdn_context_tbl[DS_3GPP_MAX_PDN_CONTEXT];

/* debug flag, 0 = tx F3, 1=rx F3 */
byte ds_eps_dbg_flag = 0;

/* global to hold lte div duplex */
static sys_div_duplex_e_type ds_3gpp_div_duplex[DS3GSUBSMGR_SUBS_ID_COUNT];

#define DS_EPS_TX_DBG_MSG 0x1
#define DS_EPS_RX_DBG_MSG 0x2
/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/
static uint8 ds_eps_bearer_cntxt_get_seq_num
(
  sys_modem_as_id_e_type   subs_id
);

LOCAL void ds_eps_bearer_cntxt_performResumeValidation
(
  sys_sys_mode_e_type       new_mode,
  sys_sys_mode_e_type       old_mode,
  sys_modem_as_id_e_type    subs_id
);
LOCAL void ds_eps_bearer_cntxt_performResume
(
  ds_bearer_context_s     *bearer_context_p
);
LOCAL lte_qci_enum_type ds_eps_bearer_cntxt_get_qci_info
(
  ds_bearer_context_s  *bearer_context_p
);

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REGISTER_DL_DATAPATH

DESCRIPTION
  This function is used to establish the complete Downlink Data path at single shot
  Setting up of watermarks, registering with IPA and registering with DL PDCP

PARAMETERS
 bearer_context_p: Bearer for which data path is to be setup

DEPENDENCIES
  None.

RETURN VALUE
None
  
SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_register_dl_datapath
(
  ds_bearer_context_s *bearer_context_p
);

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PDCPUL_IMPLICIT_DEREG_REQ_MSG_HDLR

DESCRIPTION
  This is the message handler function that will be called after L2 moves data
  back into watermark and deregisters the watermark. As part of this message
  handler DS will make a note of all the EPS IDs for which WMs are deregistered
  and sends a confirmation to L2.
  
PARAMETERS 
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE   -- If handling is successful
  FALSE  -- Otherwise
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_pdcpul_implicit_dereg_req_msg_hdlr
(  
  msgr_umid_type                                 msgrtype,
  sys_modem_as_id_e_type                         subs_id,
  const msgr_hdr_struct_type                    *dsmsg_ptr
);
#endif /* FEATURE_LAPP */

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_eps_bearer_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR void ds_eps_bearer_cntxt_qsh_mdump_collect()
{
  qsh_mdump_collect_high(&ds_eps_bearer_cntxt_ue_cat_id, sizeof(uint8));
}
#endif /* FEATURE_QSH_MDUMP */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PDP_IP_RX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received on Um interface to ps_iface for
  pdp-ip call.
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
LOCAL boolean ds_eps_bearer_cntxt_pdp_ip_rx_um_data_cb
(
  ps_sig_enum_type sig,
  void             *user_data_p
)
{
  dsm_item_type          *item_ptr = NULL;          /* Pointer to DSM item */
  ds_bearer_context_wm_s *rx_watermark_ptr = NULL;
  int32                  ps_handle = DS_3GPP_BEARER_CTXT_INVALID_HANDLE;
  ds_bearer_context_s    *bearer_context_p = NULL; /* Pointer to bearer context */
  boolean                result = TRUE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Get the bearer context from the ps handle
  -------------------------------------------------------------------------*/
  ps_handle = (int32)user_data_p;
  bearer_context_p 
    = ds_3gpp_bearer_cntxt_get_bearer_from_ps_handle(ps_handle);

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return result;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  if(bearer_context_p->ds_bearer_context_dyn_p->ps_handle != ps_handle)
  {
    DS_LTE_MSG2_HIGH_EX(subs_id,"Bearer ps handle is not valid: "
                     "original handle %d, current handle %d. "
                     "No attempt made to dequeue Um data to PS",
                     ps_handle, 
                     bearer_context_p->ds_bearer_context_dyn_p->ps_handle);
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return result;
  }

    /*-------------------------------------------------------------------------
    RM_FLOW_ENABLED flag is set to TRUE always for Embedded calls.
    For Laptop calls, this flag is set based on the flow events happening on
    the Rm Phys link.
    -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->rm_flow_enabled == TRUE)
  {
    /*-----------------------------------------------------------------------
      Get PDCP state information for the call.
    -----------------------------------------------------------------------*/
    rx_watermark_ptr = &(bearer_context_p->lte_rx_wm);

    /*-----------------------------------------------------------------------
      The PDCP wm item has IP packets queued up. De-queue and send it on the
      correct physlink.
    -----------------------------------------------------------------------*/
    item_ptr = (dsm_item_type *)dsm_dequeue(&(rx_watermark_ptr->wm_item));      
    if (item_ptr != NULL)
    {
      if ((ds_eps_dbg_flag & DS_EPS_RX_DBG_MSG) != 0)
      {
        DS_LTE_MSG1_HIGH_EX(subs_id,"ip rx um data cb. Bearer Instance:%d ",\
                         (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
      }
      /*---------------------------------------------------------------------
        Data ready for PPP processing. Invoke function to process data
        stream.  
      ---------------------------------------------------------------------*/
      /*---------------------------------------------------------------------
        Call ps_phys_link_input with the dsm chain. This routes the packet to
        the correct iface (v4 or v6) and bridges it to the RM iface if need be 
      ---------------------------------------------------------------------*/
      (void)ps_phys_link_input(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                               &item_ptr,NULL);
      result = FALSE;
    }    
  }
  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  return result;

} /* ds_eps_bearer_cntxt_pdp_ip_rx_um_data_cb() */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PDP_IP_TX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received to the lower layers for
  pdp-ip call.
  This function is registered with ps_iface for a PDP-IP call
  
PARAMETERS  
  call_id:     CM call id
  mode_info_p: mode specific info pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_pdp_ip_tx_um_data_cb
(
  ps_phys_link_type *phys_link_ptr,
  dsm_item_type     **item_ptr,
  ps_meta_info_type *meta_info_ptr,
  void              *tx_info_ptr
)
{
  ds_bearer_context_s                       *bearer_context_p;
  ds_bearer_context_wm_s                    *tx_watermark_ptr;
  sys_modem_as_id_e_type                    subs_id = SYS_MODEM_AS_ID_NONE; 
  ds_bearer_cntxt_tput_opt_struct_type      tput_opt_params;
  errno_enum_type                           ps_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  bearer_context_p = (ds_bearer_context_s*)tx_info_ptr;

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_ul_crit_sect);

 if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    PS_META_INFO_FREE(&meta_info_ptr);
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
    return;
  }

  subs_id = bearer_context_p->ds_bearer_context_dyn_p->subs_id ;

  /*-------------------------------------------------------------------------
    Get PDCP state information for call.
  -------------------------------------------------------------------------*/
  tx_watermark_ptr = &(bearer_context_p->tx_wm);

  /*-------------------------------------------------------------------------
    Enqueue this item to PDCP
  -------------------------------------------------------------------------*/
  if ((ds_eps_dbg_flag & DS_EPS_TX_DBG_MSG) != 0)
  {
    DS_LTE_MSG1_HIGH_EX(subs_id,"ip tx um data cb. Bearer Instance:%d ",\
                     (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  }

  tput_opt_params = ds_3gpp_nv_get_back_pressure_params(subs_id);  

  /*-------------------------------------------------------------------------
    Check for uplink Acks and increment the ack counter
  -------------------------------------------------------------------------*/
  if(TRUE == tput_opt_params.back_pressure_removal &&
     DSM_QUEUE_PRIORITY_HIGH == dsm_item_get_priority(*item_ptr) &&
     TCP_ACK_REG == ps_pkt_info_check_tcp_ack(*item_ptr, &ps_errno))
  {
    bearer_context_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter++;

    /*-------------------------------------------------------------------------
      Start the BP timer on getting the first ACK, but do not disable BP. Once
      the timer expires, based on the current ack cnt, BP would be disabled.
    -------------------------------------------------------------------------*/
    if (bearer_context_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter == 1)
    {
      ds_bearer_cntxt_start_bp_timer(bearer_context_p,subs_id);
    }
  }
  

  (*item_ptr)->link.next_ptr = NULL;
  lte_pdcpul_tag_each_pkt(*item_ptr, (uint8)subs_id);
  dsm_enqueue(&tx_watermark_ptr->wm_item, item_ptr);

#ifdef FEATURE_DATA_FLOW_MGMT
  ds3g_burst_flow_compute_avg_latency(&bearer_context_p->flow_control_info);
#endif /* FEATURE_DATA_FLOW_MGMT */

  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);

  PS_META_INFO_FREE(&meta_info_ptr);

} /* ds_eps_bearer_cntxt_pdp_ip_tx_um_data_cb() */

/*===========================================================================

FUNCTION:  ds_eps_bearer_reenqueue_pkts_pktcount

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr).It will also increase the dne
  count if required based on the num_pkt_bytes

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued
    num_pkt_byes: Number of bytes to be enqueued
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.  
===========================================================================*/
void ds_eps_bearer_reenqueue_pkts_pktcount
(
  dsm_watermark_type* wm_ptr,     /*WM to which pkts need to be re-enqueued*/
  uint32 num_pkt_bytes,               /*Total number of bytes to be re-enqueued*/
  dsm_item_type* head_dsm_ptr,    /*Head of DSM chain*/
  uint32 num_pkts                 /*number of packets*/
)
{
  dsm_item_type* iter_ptr = NULL;
  dsm_item_type* nx_ptr = NULL;
  uint32 new_dne_cnt =0;
  uint32                                    buffer = 0;
  uint32                                    new_dne_q_cnt =0;
  uint32                                    pkt_buffer_cnt = 0;

  if(head_dsm_ptr == NULL || wm_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_3("Dsm Pointer or Wm pointer is Null",0,0,0); 
    DS3GPP_ASSERT(0);
    return;
  }
  
  iter_ptr = (dsm_item_type *)head_dsm_ptr->link.next_ptr;
    
  /* Flow control is disabled /Back pressure is off/disabled*/
  if(wm_ptr->hiwater_func_ptr == NULL)
  {
    /* empty the wm, give priority to status bytes */
    if((wm_ptr->current_cnt + num_pkt_bytes + buffer) > 
       wm_ptr->dont_exceed_cnt)
    {
      DS_3GPP_MSG0_HIGH("Back pressure is disabled,"
                        "emptying watermark ");
      dsm_empty_queue(wm_ptr);
    }
  }
  else
  {
    if((wm_ptr->current_cnt + num_pkt_bytes + buffer) > wm_ptr->dont_exceed_cnt)
    {
      new_dne_cnt = wm_ptr->dont_exceed_cnt + num_pkt_bytes + buffer;
      DS_3GPP_MSG1_HIGH("Wm pointer Dne Updated %d",new_dne_cnt); 
      dsm_set_dne(wm_ptr,new_dne_cnt);
    }
  }

  if((wm_ptr->q_ptr->cnt + num_pkts + pkt_buffer_cnt) > wm_ptr->dne_q_cnt)
  {
    new_dne_q_cnt = wm_ptr->q_ptr->cnt + num_pkts + pkt_buffer_cnt;
    dsm_set_dne_q_cnt(wm_ptr,new_dne_q_cnt);
    DS_3GPP_MSG1_HIGH("Wm pointer new dne queue count %d",
                      new_dne_q_cnt); 
  }

  head_dsm_ptr->link.next_ptr = NULL;
  dsm_enqueue(wm_ptr,&head_dsm_ptr);

  while(iter_ptr != NULL)
  {
    nx_ptr = (dsm_item_type *) iter_ptr->link.next_ptr;
    iter_ptr->link.next_ptr = NULL;
    dsm_enqueue(wm_ptr,&iter_ptr);
    iter_ptr = nx_ptr;
  }
  DS_3GPP_MSG1_HIGH("Wm pointer Current count %d",wm_ptr->current_cnt); 

} 

/*===========================================================================

FUNCTION:  DS_EPS_BEARER_REENQUEUE_PKTS_PKTCOUNT_SPECIAL

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr). This is a special API which will be called
  when control pkts are to be queued during handover. The pkts in this case will
  not be dropped.

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued    
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  Num pkts enqueued

SIDE EFFECTS
  None.  
===========================================================================*/
uint32 ds_eps_bearer_reenqueue_pkts_pktcount_special
(
  dsm_watermark_type* wm_ptr,     /*WM to which pkts need to be re-enqueued*/  
  dsm_item_type* head_dsm_ptr     /*Head of DSM chain*/  
)
{
  dsm_item_type* iter_ptr = NULL;
  dsm_item_type* nx_ptr = NULL;
  uint32 num_pkts_enqueued = 1;

  if(head_dsm_ptr == NULL || wm_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_3("Dsm Pointer or Wm pointer is Null",0,0,0); 
    DS3GPP_ASSERT(0);
    return 0;
  }
  
  iter_ptr = (dsm_item_type *)head_dsm_ptr->link.next_ptr;

  head_dsm_ptr->link.next_ptr = NULL;

  dsm_enqueue_special(wm_ptr,
                      &head_dsm_ptr,
                      DSM_WM_ENQ_RULE_IGNORE_DNE);

  while(iter_ptr != NULL)
  {
    nx_ptr = (dsm_item_type *) iter_ptr->link.next_ptr;
    iter_ptr->link.next_ptr = NULL;
    dsm_enqueue_special(wm_ptr,
                        &iter_ptr,
                        DSM_WM_ENQ_RULE_IGNORE_DNE);
    iter_ptr = nx_ptr;
    ++num_pkts_enqueued;
  }
  
  DS_3GPP_MSG1_HIGH("Num pkts re-enqueued: %d", num_pkts_enqueued); 
  return num_pkts_enqueued;
}

/*===========================================================================

FUNCTION:  ds_eps_bearer_reenqueue_pkts

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr).It will also increase the dne
  count if required based on the num_pkt_bytes

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued
    num_pkt_byes: Number of bytes to be enqueued
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.  
===========================================================================*/
void ds_eps_bearer_reenqueue_pkts
(
  dsm_watermark_type* wm_ptr,     /*WM to which pkts need to be re-enqueued*/
  uint32 num_pkt_bytes,               /*Total number of bytes to be re-enqueued*/
  dsm_item_type* head_dsm_ptr,    /*Head of DSM chain*/
  uint8 as_id
)
{
  dsm_item_type* iter_ptr = NULL;
  dsm_item_type* nx_ptr = NULL;
  uint32 new_dne_cnt =0;
  uint32 buffer = 0;

  if(head_dsm_ptr == NULL || wm_ptr == NULL)
  {
    DS_3GPP_ERROR_FATAL("Dsm Pointer or Wm pointer is Null");
    return;
  }
  
  iter_ptr = (dsm_item_type *)head_dsm_ptr->link.next_ptr;
    
  /* Flow control is disabled /Back pressure is off/disabled*/
  if(wm_ptr->hiwater_func_ptr == NULL)
  {
    DS_3GPP_MSG0_HIGH("Back pressure is disabled ");
    return;
  }
  else
  {
    if((wm_ptr->current_cnt + num_pkt_bytes + buffer) > wm_ptr->dont_exceed_cnt)
    {
      new_dne_cnt = wm_ptr->dont_exceed_cnt + num_pkt_bytes + buffer;
      DS_3GPP_MSG1_HIGH("Wm pointer Dne Updated %d",new_dne_cnt); 
      dsm_set_dne(wm_ptr,new_dne_cnt);
    }
  }

  head_dsm_ptr->link.next_ptr = NULL;
  dsm_enqueue(wm_ptr,&head_dsm_ptr);

  while(iter_ptr != NULL)
  {
    nx_ptr = (dsm_item_type *) iter_ptr->link.next_ptr;
    iter_ptr->link.next_ptr = NULL;
    dsm_enqueue(wm_ptr,&iter_ptr);
    iter_ptr = nx_ptr;
  }


} /* ds_eps_bearer_cntxt_pdp_ip_tx_um_data_cb() */
/*===========================================================================
FUNCTION:  ds_eps_bearer_flush_bytes

DESCRIPTION
  This API will be used to flush the eps bearer and ensure that the wm's
  number of bytes to be remaining at the end is less than or equal to the
  bytes specified
  

PARAMETERS
  eps_id : eps id
  num_pkt_byes: Number of bytes to be remaining

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.  
===========================================================================*/
void ds_eps_bearer_flush_bytes
(
  uint8   eps_id,           /*eps bearer id which needs to be flused*/
  uint32  num_pkt_bytes,    /*Total number of bytes to be remaining 
                             at end of procedure*/
  uint8   as_id
)
{
  dsm_item_type                   *item_ptr = NULL;
  int                             bearer_cntx_inst;
  ds_bearer_context_s             *bearer_context_p = NULL;
  ds_eps_bearer_context_s         *eps_bearer_context = NULL;
  dsm_watermark_type              *wm_ptr = NULL;
  uint32                           initial_count = 0;   
 
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(bearer_cntx_inst = 0;
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_inst++) 
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      continue;
    }
	if(!ds_bearer_cntxt_validate_bearer_subscription(bearer_context_p,(sys_modem_as_id_e_type)as_id))
	{
	  continue;
	}
    eps_bearer_context = (ds_eps_bearer_context_s *)
        DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);

    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context))
    {
      continue;
    }
	  
    if(eps_bearer_context->ds_eps_bearer_context_dyn_p->eps_bearer_id == eps_id)
    {
      DS_LTE_MSG2_HIGH_EX(as_id,"Found the bearer context with subs_id %d eps bearer id %d",
                         as_id,eps_id);

      break;
    }	
  }
  if(bearer_cntx_inst >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_MSG1_LOW("Cannot find the requested bearer context with "
                       "eps bearer id: %d",eps_id);
    bearer_context_p = NULL;
  }
  

  if(bearer_context_p != NULL)
  {
    wm_ptr = &(bearer_context_p->lte_rx_wm.wm_item);
    initial_count = wm_ptr->current_cnt;
    if(num_pkt_bytes == 0)
    {
      if(wm_ptr != NULL)
      {
        dsm_empty_queue(wm_ptr);  
      }
    }
    else
    {
      while((wm_ptr != NULL) &&
            ((item_ptr = dsm_dequeue(wm_ptr)) != NULL))
      {
        (void)dsm_free_packet(&item_ptr);
        if(wm_ptr != NULL  &&
           wm_ptr->current_cnt <= num_pkt_bytes)
        {
          break;
        }
      }
    } 
    DS_LTE_MSG2_HIGH_EX(as_id,"Inital count %d Current count %d packets "
                     ,initial_count,wm_ptr->current_cnt);
  }

  return;
} 
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTX_SETUP_PDP_PS_TO_PDCP_WM

DESCRIPTION
  This function sets up PS to PDCP watermark.
  
PARAMETERS  
  call_id:     CM call id
  mode_info_p: mode specific info pointer
    
DEPENDENCIES 
  None.

RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_setup_pdp_ps_to_pdcp_wm
(
  ds_bearer_context_s       *bearer_context_ptr
)
{
  dsm_watermark_type                          *tx_wm_ptr = NULL;
  ds_pdn_context_s                            *pdn_cntx_p = NULL;
  sys_modem_as_id_e_type                       subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;
  ds_eps_bearer_cntxt_wm_levels_type           eps_ul_wm_levels;
  uint32                                       ul_data_rate=0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
  {
    return;
  }

  pdn_cntx_p = bearer_context_ptr->ds_bearer_context_dyn_p->pdn_context_ptr;

  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }

  tx_wm_ptr = &(bearer_context_ptr->tx_wm.wm_item);

  if (tx_wm_ptr == NULL)
  {
    DS_3GPP_MSG1_LOW("tx wm ptr is NULL for bearer instance %d",
                bearer_context_ptr->ds_bearer_context_dyn_p->client_data_ptr);
    return;
  }

  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);  
  ul_data_rate = ds_eps_get_configured_ul_data_rate(subs_id);

  /* Set wm level based on NV if it is set */
  if (um_wm_threshold.lte_wm_levels_valid == TRUE)
  {
    DS_3GPP_MSG3_MED("Setting LTE UL wm levels based on NV settings,"
                     "low_wm %d, hi_wm %d, dne_wm %d",
                     um_wm_threshold.lte_ul_lo,
                     um_wm_threshold.lte_ul_hi,
                     um_wm_threshold.lte_ul_dne);
    dsm_set_low_wm(tx_wm_ptr,
                   um_wm_threshold.lte_ul_lo);
    dsm_set_hi_wm(tx_wm_ptr,
                  um_wm_threshold.lte_ul_hi);
    dsm_set_dne(tx_wm_ptr,
                um_wm_threshold.lte_ul_dne);
    dsm_set_dne_q_cnt(tx_wm_ptr,
                      DSLTEPS_UL_WM_DEFAULT_DNE_QCNT);
  }
  else
  {
    ds_eps_bearer_cntxt_get_ul_wm_levels_per_ul_rate(ul_data_rate,
                                                     &eps_ul_wm_levels);
    dsm_set_low_wm(tx_wm_ptr,
                   eps_ul_wm_levels.lo_wm_level);
    dsm_set_hi_wm(tx_wm_ptr,
                  eps_ul_wm_levels.hi_wm_level);
    dsm_set_dne(tx_wm_ptr,
                eps_ul_wm_levels.dne_q_cnt);
    dsm_set_dne_q_cnt(tx_wm_ptr,
                      eps_ul_wm_levels.dne_q_cnt);
  }

  /*-----------------------------------------------------------------------
    Pass the Bearer Cntx instance in watermarks registered so that we
    can identify the call when the callback is called
  -----------------------------------------------------------------------*/

  tx_wm_ptr->lowater_func_ptr = 
                          (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_enable_cb;
  tx_wm_ptr->lowater_func_data = 
          (void*)bearer_context_ptr->ds_bearer_context_dyn_p->client_data_ptr;
  tx_wm_ptr->hiwater_func_ptr = 
                         (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_disable_cb;
  tx_wm_ptr->hiwater_func_data = 
          (void*)bearer_context_ptr->ds_bearer_context_dyn_p->client_data_ptr;

#ifdef FEATURE_DATA_FLOW_MGMT
  tx_wm_ptr->dne_func_ptr = 
                         (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_dne_disable_cb;


  tx_wm_ptr->gone_empty_func_ptr = 
          (wm_cb_type)ds_bearer_cntxt_ps_to_L2_gone_empty_cb;

  tx_wm_ptr->gone_empty_func_data = 
          (void*)bearer_context_ptr;
#endif /*FEATURE_DATA_FLOW_MGMT */

  /* Invoke l2_to_ps_lo_wm_cb here such that flow can be enabled if WM 
     current_cnt is less than lo_level */

  if (tx_wm_ptr->current_cnt < tx_wm_ptr->hi_watermark) 
  {
    ds_bearer_cntxt_ps_to_L2_flow_enable_cb(tx_wm_ptr,
      bearer_context_ptr->ds_bearer_context_dyn_p->client_data_ptr);
  }

  return;
} /* ds_eps_bearer_cntxt_setup_pdp_ps_to_pdcp_wm() */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REGISTER_UL_WITH_PDCP

DESCRIPTION
  This function sends UL registration message to PDCP.
  
PARAMETERS  
  bearer_context_p:     bearer_context_pointer
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_register_ul_with_pdcp
(
  ds_bearer_context_s       *bearer_context_ptr
)
{
  ds_eps_bearer_context_s            *eps_bearer_context_ptr = NULL;
  lte_pdcpul_rab_register_req_msg_s   pdcp_ul_rab_reg_req_msg;
  boolean                             rtval = TRUE;
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;
  /*-----------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id))
  {
    return FALSE;
  }
 
  eps_bearer_context_ptr = 
    (ds_eps_bearer_context_s *) (bearer_context_ptr->ds_bearer_context_dyn_p->
                                   vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_ptr))
  {
    return FALSE;
  }
  
  pdcp_ul_rab_reg_req_msg.eps_id = eps_bearer_context_ptr->
                              ds_eps_bearer_context_dyn_p->eps_bearer_id;

  pdcp_ul_rab_reg_req_msg.ps_pdcp_wm_ptr = 
                             (void *) &(bearer_context_ptr->tx_wm.wm_item);

  pdcp_ul_rab_reg_req_msg.allow_llm_discard_timer = 
     ds_3gpp_pdn_cntxt_is_bearer_allowed_to_use_discard_timer(
      pdcp_ul_rab_reg_req_msg.eps_id, subs_id);

  /*-----------------------------------------------------------------------
     In regular loopback mode, register with loopback module
     In L2 loopback mode and mission mode - register with PDCP. 
  -----------------------------------------------------------------------*/
  if(ds_3gpp_loopback_is_enabled() && 
     !ds_3gpp_l2_loopback_is_enabled())
  {
    rtval = ds_3gpp_loopback_lte_pdcpul_rab_register_req(
              eps_bearer_context_ptr->ds_eps_bearer_context_dyn_p->eps_bearer_id,
              &(bearer_context_ptr->tx_wm.wm_item),
              subs_id);
  }
  else
  {
    rtval = ds3gmsgr_msg_send_ext
            (
              LTE_PDCPUL_RAB_REGISTER_REQ,  
              MSGR_DS_MSGRRECV,
              (msgr_hdr_struct_type*)(&pdcp_ul_rab_reg_req_msg),
              sizeof(lte_pdcpul_rab_register_req_msg_s), 
              subs_id
            );
  }

  if(rtval == FALSE) 
  {
    DS_LTE_MSG0_LOW(" Unable to send message to DSMSGR");
  }

  return(rtval);                  
} /* dsEpsBearerContxt_register_ul_with_PDCP */
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_DL_LOW_WM_CB

DESCRIPTION
  This function sets up the downlink dne watermarks as part of downlink low
  watermark callback; if wm levels were changed due to confifured throughput 
  indication
  
PARAMETERS  
  bearer_context_p   - bearer context pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_dl_low_wm_cb
(
 dsm_watermark_type *wm,
 void*              callback_data
)
{
  uint32                                           bearer_instance = 0;
  ds_bearer_context_s                             *bearer_context_p = NULL;
  sys_modem_as_id_e_type                           subs_id
                                                     = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type      um_wm_threshold;
  ds_bearer_context_wm_s                          *rx_wm_p = NULL;
  uint32                                           wm_dne_level = 0;
  uint32                                           wm_dne_q_level = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  bearer_instance = (uint32) callback_data;
  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  subs_id =  bearer_context_p->ds_bearer_context_dyn_p->subs_id;
  um_wm_threshold =  ds_3gpp_nv_get_um_wm_threshold(subs_id);
  rx_wm_p = &(bearer_context_p->lte_rx_wm);
  if( bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE )
  {
      wm_dne_level = ds_bearer_cntx_get_lte_dl_wm_level(bearer_context_p,
                                                        &um_wm_threshold,
                                                        DL_WM_DNE_INDEX,
                                                        subs_id);

      wm_dne_q_level = ds_bearer_cntx_get_lte_dl_wm_level(bearer_context_p,
                                                        &um_wm_threshold,
                                                        DL_WM_DNE_QCNT_INDEX,
                                                        subs_id);

      if(rx_wm_p->wm_item.dont_exceed_cnt != wm_dne_level)
      {
        dsm_set_dne(&rx_wm_p->wm_item, wm_dne_level);

        DS_3GPP_MSG3_HIGH("Downlink WM levels: DNE: %d HI: %d LO: %d",
                     rx_wm_p->wm_item.dont_exceed_cnt,
                     rx_wm_p->wm_item.hi_watermark,
                     rx_wm_p->wm_item.lo_watermark);

      }

      if(rx_wm_p->wm_item.dne_q_cnt != wm_dne_q_level)
      {
        dsm_set_dne_q_cnt(&rx_wm_p->wm_item, wm_dne_q_level);
      }

      DS_3GPP_MSG2_HIGH("Downlink WM levels: DNE: %d DNE queue count: %d",
                     rx_wm_p->wm_item.dont_exceed_cnt,
                     rx_wm_p->wm_item.dne_q_cnt);
  }

   
} /* ds_eps_bearer_cntxt_dl_low_wm_cb() */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_SETUP_PDPD_DL_WM

DESCRIPTION
  This function sets up the downlink watermarks
  
PARAMETERS  
  bearer_context_p:     bearer_context_pointer
      
DEPENDENCIES 
  None.

RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/

void ds_eps_bearer_cntxt_setup_pdp_dl_wm
(
  ds_bearer_context_s       *bearer_context_p
)
{
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;
  uint32                                       dl_data_rate=0;
  ds_eps_bearer_cntxt_wm_levels_type           eps_dl_wm_levels;
  dsm_watermark_type                           *rx_wm_ptr = NULL;

  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }


  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  rx_wm_ptr = &(bearer_context_p->lte_rx_wm.wm_item);
  if (rx_wm_ptr == NULL)
  {
    return;
  }

  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);

  dl_data_rate = ds_eps_get_configured_dl_data_rate(subs_id);
  if (um_wm_threshold.lte_wm_levels_valid == TRUE)
  {
    dsm_set_low_wm(rx_wm_ptr,
                 um_wm_threshold.lte_dl_lo);
    dsm_set_hi_wm(rx_wm_ptr,
                um_wm_threshold.lte_dl_hi);
    dsm_set_dne(rx_wm_ptr,
              um_wm_threshold.lte_dl_dne);
    dsm_set_dne_q_cnt(rx_wm_ptr,
                    DSLTEPS_DL_WM_DEFAULT_DNE_QCNT);
}
  else
  {
    ds_eps_bearer_cntxt_get_dl_wm_levels_per_dl_rate(dl_data_rate,
                                                     &eps_dl_wm_levels);
    dsm_set_low_wm(rx_wm_ptr,
                    eps_dl_wm_levels.lo_wm_level);
    dsm_set_hi_wm(rx_wm_ptr,
                    eps_dl_wm_levels.hi_wm_level);
    dsm_set_dne(rx_wm_ptr,
                    eps_dl_wm_levels.dne_wm_level);
    dsm_set_dne_q_cnt(rx_wm_ptr,
                    eps_dl_wm_levels.dne_q_cnt);
  }

  rx_wm_ptr->lowater_func_ptr = 
                          (wm_cb_type)ds_eps_bearer_cntxt_dl_low_wm_cb;
  rx_wm_ptr->lowater_func_data = 
          (void*)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr;



}
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REGISTER_DL_WITH_PDCP

DESCRIPTION
  This function registers downlink with PDCP
  
PARAMETERS  
  bearer_context_p  - bearer_context_pointer
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_register_dl_with_pdcp
(
  ds_bearer_context_s       *bearer_context_p
)
{
  boolean                             rtval = TRUE;
  ds_eps_bearer_context_s            *eps_bearer_context_p = NULL;
  lte_pdcpdl_rab_register_req_msg_s   pdcp_dl_rab_reg_req_msg;
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;
  /*-----------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return FALSE;
  }

  pdcp_dl_rab_reg_req_msg.eps_id = eps_bearer_context_p->
    ds_eps_bearer_context_dyn_p->eps_bearer_id;

#ifndef FEATURE_DATA_JO_PRESIL
  /* Send the downlink watermark pointer to Pdcp */
  pdcp_dl_rab_reg_req_msg.pdcp_wm_ptr = &(bearer_context_p->lte_rx_wm.wm_item);
#endif

  /*-----------------------------------------------------------------------
     In regular loopback mode, register with loopback module
     In L2 loopback mode and mission mode - register with PDCP. 
  -----------------------------------------------------------------------*/
  if(ds_3gpp_loopback_is_enabled() && 
     !ds_3gpp_l2_loopback_is_enabled()) 
  {
     rtval = ds_3gpp_loopback_lte_pdcpdl_rab_register_req(
               eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id,
               &(bearer_context_p->lte_rx_wm.wm_item),
               subs_id);
  }
  else
  { 
    rtval = ds3gmsgr_msg_send_ext
            (
              LTE_PDCPDL_RAB_REGISTER_REQ, 
              MSGR_DS_MSGRRECV,
              (msgr_hdr_struct_type*)(&pdcp_dl_rab_reg_req_msg),
              sizeof(lte_pdcpdl_rab_register_req_msg_s),
              subs_id
          );
  }

  if(rtval == FALSE) 
  {
    DS_LTE_MSG0_LOW(" Unable to send message to DSMSGR");
  }

  return(rtval);                  
} /* ds_eps_bearer_cntxt_register_dl_with_pdcp */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_DEREGISTER_DL_WITH_PDCP

DESCRIPTION
  This function deregisters downlink with PDCP
  
PARAMETERS  
  bearer_context_p  - bearer_context_pointer
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/

boolean ds_eps_bearer_cntxt_deregister_dl_with_pdcp
(
  ds_bearer_context_s       *bearer_context_p
)
{
  boolean                            rtval = TRUE;
  ds_eps_bearer_context_s           *eps_bearer_context_p = NULL;
  lte_pdcp_rab_deregister_req_msg_s  pdcp_dl_rab_dereg_req_msg;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;
  /*-----------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return FALSE;
  }

  pdcp_dl_rab_dereg_req_msg.eps_id = eps_bearer_context_p->
    ds_eps_bearer_context_dyn_p->eps_bearer_id;

  
  rtval = ds3gmsgr_msg_send_ext
          (
            LTE_PDCPDL_RAB_DEREGISTER_REQ, 
            MSGR_DS_MSGRRECV,
            (msgr_hdr_struct_type*)(&pdcp_dl_rab_dereg_req_msg),
            sizeof(lte_pdcp_rab_deregister_req_msg_s),
            subs_id
          );

  if(rtval == FALSE)
  {
    DS_LTE_MSG0_LOW(" Unable to send message to DSMSGR");
  }

  return(rtval);                  
} /* ds_eps_bearer_cntxt_deregister_dl_with_pdcp */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_DEREGISTER_UL_WITH_PDCP

DESCRIPTION
  This function deregisters upnlink with PDCP
  
PARAMETERS  
  bearer_context_p  - bearer_context_pointer
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_deregister_ul_with_pdcp
(
  ds_bearer_context_s       *bearer_context_p
)
{
  boolean                            rtval = TRUE;
  ds_eps_bearer_context_s           *eps_bearer_context_p = NULL;
  lte_pdcp_rab_deregister_req_msg_s  pdcp_ul_rab_dereg_req_msg;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;
  /*----------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return FALSE;
  }

  pdcp_ul_rab_dereg_req_msg.eps_id = eps_bearer_context_p->
    ds_eps_bearer_context_dyn_p->eps_bearer_id;


  /*-----------------------------------------------------------------------
     In regular loopback mode, de-register with loopback module
     In L2 loopback mode and mission mode - de-register with PDCP. 
  -----------------------------------------------------------------------*/
  if(ds_3gpp_loopback_is_enabled() && 
     !ds_3gpp_l2_loopback_is_enabled())  
  {
    rtval = ds_3gpp_loopback_lte_pdcpul_rab_deregister_req(
              eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id,
              subs_id);
  }
  else
  {
    rtval = ds3gmsgr_msg_send_ext
            ( 
            LTE_PDCPUL_RAB_DEREGISTER_REQ, 
            MSGR_DS_MSGRRECV,
            (msgr_hdr_struct_type*)(&pdcp_ul_rab_dereg_req_msg),
            sizeof(lte_pdcp_rab_deregister_req_msg_s),
            subs_id
          );
  }
    
  if(rtval == FALSE) 
  {
    DS_LTE_MSG0_LOW(" Unable to send message to DSMSGR");
  }

  return(rtval);                  
} /* ds_eps_bearer_cntxt_deregister_ul_with_pdcp */



void ds_eps_bearer_cntxt_cleanup_pdcp_watermarks
(
  ds_bearer_context_s       *bearer_context_p
)
{

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Emtpy TX and RX WM
  -------------------------------------------------------------------------*/
  dsm_empty_queue ( &(bearer_context_p->tx_wm.wm_item) );
  
  dsm_empty_queue ( &(bearer_context_p->lte_rx_wm.wm_item));
  
  /*-----------------------------------------------------------------------
    Set the total_rcvd_cnt in the PS->PDCP and PDCP->PS watermark to 0.
    The total_rcvd_cnt should not be modified outside DSM code            .
    Legacy code.
  -----------------------------------------------------------------------*/
  bearer_context_p->tx_wm.wm_item.total_rcvd_cnt    
       = 0;
  bearer_context_p->lte_rx_wm.wm_item.total_rcvd_cnt       
      = 0;

} /* dsltepsd_cleanup_pdcp_watermarks() */


static void ds_eps_bearer_cntxt_dereg_pdcp
(
  ds_bearer_context_s     *bearer_context_p
)
{
  boolean                          retval = FALSE;
  ds_eps_bearer_context_flow_dir_e bearer_flow_dir;
  ds_eps_bearer_context_s         *eps_bearer_context_p;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;  

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p) == FALSE)
  {
    DS3GPP_ASSERT(0);
    return;
  }
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  eps_bearer_context_p = (ds_eps_bearer_context_s*)
                   (bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return;
  }

  if(eps_bearer_context_p->ds_eps_bearer_context_dyn_p->is_registered_pdcp)
  {
    /*----------------------------------------------------------------------
      Get the flow direction for this bearer
    --------------------------------------------------------------------*/
    bearer_flow_dir = ds_eps_bearer_cntxt_getFlowDirection(bearer_context_p);
    if ( bearer_flow_dir == DS_EPS_FLOW_DIR_BIDIRECTIONAL)
    {
      if (!ds_eps_bearer_cntxt_deregister_ul_with_pdcp(bearer_context_p) ||
          !ds_eps_bearer_cntxt_deregister_dl_with_pdcp(bearer_context_p)) 
      {
        retval = FALSE;
      }
    }
    else if(bearer_flow_dir == DS_EPS_FLOW_DIR_UPLINK )
    {
      retval = ds_eps_bearer_cntxt_deregister_ul_with_pdcp(bearer_context_p);
    }
    else if(bearer_flow_dir == DS_EPS_FLOW_DIR_DOWNLINK )
    {
      retval = ds_eps_bearer_cntxt_deregister_dl_with_pdcp(bearer_context_p);
    }

    if(retval == FALSE)
    {
      DS_LTE_MSG2_ERROR_EX(subs_id, "Dereg with ul/dl Failure eps_id d bearer_flow_dir d",
                         eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id,
                         bearer_flow_dir);
    }

    eps_bearer_context_p->ds_eps_bearer_context_dyn_p->is_registered_pdcp = FALSE;
  }

}


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PHYS_LINK_CLEANUP

DESCRIPTION
  This function clears up the phys link when the bearer is torn down.
  
PARAMETERS  
  bearer_context_p   - bearer context pointer
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_phys_link_cleanup
(
  ds_bearer_context_s       *bearer_context_p
)
{
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Cleanup TX and RX watermarks
  -------------------------------------------------------------------------*/
   ds_eps_bearer_cntxt_cleanup_pdcp_watermarks(bearer_context_p);
} /* ds_eps_bearer_cntxt_phys_link_cleanup */

/*===========================================================================
FUNCTION DS_EPS_BEARER_SEND_TRM_PRIORITY_IND

DESCRIPTION  
  This function is used to send TRM Priority Indication to RRC on
  RAB Resstablishment / Release.
      
PARAMETERS 
  1. bearer_context_p 
  2. proc_type
 
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_send_trm_priority_ind
(
  ds_bearer_context_s*  bearer_context_p,
  sys_proc_type_e_type  proc_type
)
{
  lte_dsds_trm_priority_req_s  trm_priority_req = {{0}};
  ds_3gpp_lte_qos_params_type  lte_netw_alloc_qos = {0};
  ds_pdn_context_s             *pdn_context_p = NULL;
  sys_modem_as_id_e_type       subs_id;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_3gpp_nv_manager_get_enable_send_trm_priority() == TRUE)
  {
    if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
    {
      return;
    }

    ds_eps_bearer_flow_manager_get_lte_netw_alloc_qos(bearer_context_p,
                                                      &lte_netw_alloc_qos);

    pdn_context_p = (ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->
                                         pdn_context_ptr);

    if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_p))
    {
      return;
    }

    if(((pdn_context_p->ds_pdn_context_dyn_p->rab_reestab_call_type[DS_3GPP_PDN_CONTEXT_IPV6_IFACE] == 
                                   PS_IFACE_3GPP_LTE_REESTAB_CALLTYPE_VOLTE)||
        (pdn_context_p->ds_pdn_context_dyn_p->rab_reestab_call_type[DS_3GPP_PDN_CONTEXT_IPV4_IFACE] == 
                                   PS_IFACE_3GPP_LTE_REESTAB_CALLTYPE_VOLTE))&&
       (lte_netw_alloc_qos.qci == DS_3GPP_LTE_QCI_1))
    {
      // Post TRM_PRIORITY_REQ to RRC

      trm_priority_req.proc_type = proc_type;
      trm_priority_req.client_id = LTE_TRM_PRIORITY_REQ_FROM_DS;


        /* Inform TLB that DS is ready for incoming packets */
      if( FALSE == ds3gmsgr_msg_send_ext
             ( LTE_RRC_DSDS_TRM_PRIORITY_REQ,
               MSGR_DS_LTE, 
               (msgr_hdr_struct_type*)(&trm_priority_req),
               sizeof(trm_priority_req),
               subs_id
             ) )
      {
        DS_LTE_MSG0_ERROR_EX(subs_id, "Posting of TRM_PRIORITY_REQ failed");
      }
      else
      {
        DS_LTE_MSG0_LOW("Posted TRM_PRIORITY_REQ ");
      }
    }
  }
  else
  {
    DS_LTE_MSG0_LOW("NV: DS_3GPP_ENABLE_SEND_TRM_PRIORITY is FALSE. "
                     "Not notifying TRM Priority to LTE RRC");
  }

}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_DIV_DUPLEX

DESCRIPTION
  This function returns the LTE div duplex reported by CM.

PARAMETERS
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  Last div duplex reported by CM.

SIDE EFFECTS
  None.
===========================================================================*/
sys_div_duplex_e_type ds_eps_bearer_cntxt_get_div_duplex
(
  sys_modem_as_id_e_type  subs_id
)
{
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return SYS_DIV_DUPLEX_NONE;
  }

  return ds_3gpp_div_duplex[ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id)];
} /* ds_eps_bearer_cntxt_get_div_duplex */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_SET_DIV_DUPLEX

DESCRIPTION
  This function returns the previous lte div duplex reported by CM.

PARAMETERS
  new_div_duplex: Div Duplex 
  subs_id:        Subscription id

DEPENDENCIES
  None.

RETURN VALUE
  Last lte div duplex reported by CM.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_eps_bearer_cntxt_set_div_duplex
(
  sys_div_duplex_e_type   new_div_duplex,
  sys_modem_as_id_e_type  subs_id
)
{
  if (ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    ds_3gpp_div_duplex[ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id)] = new_div_duplex;
    DS_LTE_MSG2_MED_EX(subs_id,"New Div Duplex: %d, Subs id(CM): %d",new_div_duplex, subs_id);
  }
} /* ds_eps_bearer_cntxt_set_div_duplex() */

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_SO_MASK

DESCRIPTION
  This function returns the SO MASK corresponding to the LTE div duplex
  and carrier aggregation.

PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  lte so mask 

SIDE EFFECTS
  None.
===========================================================================*/
uint64 ds_eps_bearer_cntxt_get_so_mask
(
  sys_modem_as_id_e_type  subs_id
)
{
  uint64                        so_mask = DS_SYS_SO_EX_UNSPECIFIED;
  sys_sys_mode_e_type           mode = SYS_SYS_MODE_NONE;
  sys_div_duplex_e_type         div_duplex = SYS_DIV_DUPLEX_NONE;
  ds3gsubsmgr_subs_id_e_type    ds3g_subs_id = DS3GSUBSMGR_SUBS_ID_INVALID;
#ifdef FEATURE_LAPP
  boolean                       nr5g_pref_sys = FALSE;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds3g_subs_id = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  if (!ds3gsubsmgr_is_ds3g_subs_index_valid(ds3g_subs_id))
  {
    return so_mask;
  }

  mode = ds3gpp_get_current_network_mode(subs_id);

  div_duplex = ds_eps_bearer_cntxt_get_div_duplex(subs_id);

#ifdef FEATURE_LAPP
  nr5g_pref_sys = ds_3gpp_link_switch_is_nr5g_pref_sys(subs_id);

  if (nr5g_pref_sys)
  {
    so_mask = DS_SYS_SO_EX_3GPP_5G_TDD;
  }
  else
#endif /* FEATURE_LAPP */
  {
    so_mask = ds3g_get_so_mask_from_div_duplex(div_duplex, mode);
  }
  
  /*---------------------------------------------------------------------  
    Get SO mask only if DSD preferred system is LTE.
    If Preferred system is 5G then SO mask will be set to
    DS_SYS_SO_EX_3GPP_5G_FDD
  ---------------------------------------------------------------------*/
  if ( mode == SYS_SYS_MODE_LTE
#ifdef FEATURE_LAPP
      && (!nr5g_pref_sys)
#endif
      )
  {
    /* Update the so mask if carrier aggregation is on */
    if (ds_eps_pdn_cntxt_get_num_of_active_scells(subs_id) > 0)
    {
      if (ds_eps_pdn_cntxt_get_num_of_ul_active_scells(subs_id) > 0)
      {
        so_mask |= (DS_SYS_SO_EX_3GPP_LTE_CA_DL | DS_SYS_SO_EX_3GPP_LTE_CA_UL);
      }
      else
      {
        so_mask |= DS_SYS_SO_EX_3GPP_LTE_CA_DL; 
      }
    }
  }
  so_mask |= ds_eps_bearer_cntxt_ex_get_so_mask(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));

  return so_mask;
} /* ds_eps_bearer_cntxt_get_so_mask_for_div_duplex */
#endif /* FEATURE_DATA_WLAN_MAPCON */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_QCI_INFO

DESCRIPTION
  This function gets QCI info from EPS bearer flow manager
  
PARAMETERS  
  bearer_context_p
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  QCI value.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
LOCAL lte_qci_enum_type ds_eps_bearer_cntxt_get_qci_info
(
  ds_bearer_context_s  *bearer_context_p
)
{
  ds_eps_bearer_flow_manager_s *eps_bflow_manager_p = NULL;
  ds_bearer_flow_manager_s     *bflow_manager_p = NULL;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE; 
  /*-------------------------------------------------------------------------------------------*/
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
 
  bflow_manager_p = ds_bearer_flow_manager_find_flow_manager(bearer_context_p);
  if(!ds_bearer_flow_manager_validate_flow_manager(bflow_manager_p))
  {
   return LTE_QCI_0;
  }
  
  
  eps_bflow_manager_p = 
  (ds_eps_bearer_flow_manager_s *)(bflow_manager_p->ds_bearer_flow_manager_dyn_p\
                                   ->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);
  
  if(!ds_eps_bearer_flow_manager_validate_eps_bearer_flow_manager(eps_bflow_manager_p))
  {
   DS_LTE_MSG0_LOW("eps_bearer_context_p is NULL");
   return LTE_QCI_0;
  }
  else
  {
    DS_LTE_MSG1_HIGH_EX(subs_id,"eps_bearer_context_p QCI value = %d",
                        (int)eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci);
    return (lte_qci_enum_type)eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci;
  }
} /*ds_eps_bearer_cntxt_get_qci_info*/

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_CALL_RAB_RE_ESTAB_IND_HDLR

DESCRIPTION
  This function does mode specific operations that have to be performed on
  RAB re-establishment.

  Currently all that this does is A2 specific.
  
PARAMETERS  
  bearer_context_p
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_call_rab_re_estab_ind_hdlr
(
  ds_bearer_context_s              *bearer_context_p,
  cm_call_mode_info_s_type         *mode_info_ptr
  
)
{
  boolean                            retval = FALSE;
  ds_eps_bearer_context_flow_dir_e   bearer_flow_dir;
  ds_eps_bearer_context_s           *eps_bearer_context_p;
  ds_bearer_flow_manager_s          *bflow_manager_p = NULL;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;
#ifdef FEATURE_LAPP_UFS  
  #error code not present
#endif
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p) == FALSE)
  {
    DS3GPP_ASSERT(0);
    return FALSE;
  }

  eps_bearer_context_p = (ds_eps_bearer_context_s*)
                   (bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return DS3G_FAILURE;
  }

  /*---------------------------------------------------------------------  
    Update flow if necessary
  ---------------------------------------------------------------------*/
  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(
    (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,&subs_id))
  {
    return DS3G_FAILURE;
  }

#ifdef FEATURE_LAPP_UFS  
  #error code not present
#endif /* FEATURE_LAPP_UFS  */  

  eps_bearer_context_p->ds_eps_bearer_context_dyn_p->rb_id = 
     mode_info_ptr->info.lte_call.rb_id;

/* ToDo - Move to eps_bearer_context_ex*/  
#ifdef FEATURE_LAPP_UFS  
  #error code not present
#endif /* FEATURE_LAPP_UFS  */    

  if((ds_eps_attach_sm_get_state(subs_id) == DS_EPS_PROCESSED_CNTXT_TXFR_IND) &&
     (ds_eps_bearer_cntxt_get_qci_info(bearer_context_p) == LTE_QCI_0))
  {
    bflow_manager_p = ds_bearer_flow_manager_find_flow_manager(bearer_context_p);
    if (ds_eps_bearer_flow_manager_update_flow_on_resume(bflow_manager_p) == FALSE)
    {
      DS_LTE_MSG1_LOW("Flow update error on bearer instance 0x%d, skip",
                     (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
    }
  }
  /*----------------------------------------------------------------------
    Get the flow direction for this bearer
  --------------------------------------------------------------------*/
  bearer_flow_dir = ds_eps_bearer_cntxt_getFlowDirection(bearer_context_p);
  if ((bearer_flow_dir == DS_EPS_FLOW_DIR_BIDIRECTIONAL) ||
       bearer_flow_dir == DS_EPS_FLOW_DIR_UPLINK)
  {
    /*----------------------------------------------------------------------
      Configure the PS-PDCP WM before registring with PDCP
    --------------------------------------------------------------------*/
    ds_eps_bearer_cntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p);
    retval = ds_eps_bearer_cntxt_register_ul_with_pdcp(bearer_context_p);
    if(retval == FALSE)
    {
      
      DS_LTE_MSG0_ERROR_EX(subs_id, "UL Registering with lower layer failed");
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP_DORMANT);
      /*---------------------------------------------------------------------  
        Register each enqueue function ptr.      
      ---------------------------------------------------------------------*/
      (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =\
        (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
      (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data =\
        (void*) bearer_context_p;
    }
  }
  /*----------------------------------------------------------------------
    for dedicated bearer, we need to check the flow spec to see if it is
    uni or bi-directional
  --------------------------------------------------------------------*/
  else if (bearer_flow_dir == DS_EPS_FLOW_DIR_DOWNLINK)
  {
    ds_eps_bearer_cntxt_register_dl_datapath(bearer_context_p);
    /* Disallow UL Flow as flow direction is only DL  */
    ds_flow_manager_update_uplink_flow_capability(bearer_context_p, FALSE);
  }
  else
  {
    // invalid case
    DS_LTE_MSG1_LOW("Invalid flow dir for bearer_context:%x",
                      bearer_context_p);
    return DS3G_FAILURE;        
  }

  //Send the QCI of this bearer to interested parties.
  ds_eps_bearer_cntxt_send_QOS_indication(bearer_context_p);

  ds_eps_bearer_cntxt_send_trm_priority_ind(bearer_context_p,
                                            SYS_PROC_TYPE_HIGH_PRIORITY_PS_CALL);

  return ( DS3G_SUCCESS );
} /* ds_eps_bearer_cntxt_call_rab_re_estab_ind_hdlr */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_CALL_RAB_REL_IND_HDLR

DESCRIPTION
  This function does mode specific operations that have to be performed on
  RAB re-establishment.

  Currently all that this does is A2 specific.
  
PARAMETERS  
  bearer_context_p

DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_call_rab_rel_ind_hdlr
(
  ds_bearer_context_s               *bearer_context_p,
  lte_nas_local_cause_T             *lte_nas_local_cause
)
{
  sys_modem_as_id_e_type                 subs_id = SYS_MODEM_AS_ID_NONE;
  ds_eps_bearer_context_s               *eps_bearer_context_p = NULL;
  /*----------------------------------------------------------------------*/

  ds_eps_bearer_cntxt_send_trm_priority_ind(bearer_context_p,
                                            SYS_PROC_TYPE_NONE);

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return ( DS3G_FAILURE );
  }

  eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return ( DS3G_FAILURE );
  }
  /*trigger service req if cm indicate rel cause PDCP_OUT_OF_SYNC and 
    currently there is no data in wm*/
  if((bearer_context_p->tx_wm.wm_item.current_cnt == 0)&&
  	 (lte_nas_local_cause->valid)&&
  	 (lte_nas_local_cause->local_cause_value == EMM_CON_REL_PDCP_OUT_OF_SYNC))
  {
    DS_LTE_MSG0_HIGH_EX(subs_id,"trigger service req due to PDCP_OUT_OF_SYNC");  
    ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL); 
  }

  /* ToDo - Move to eps_bearer_context_ex*/  
#ifdef FEATURE_LAPP_UFS  
  #error code not present
#endif /* FEATURE_LAPP_UFS  */

  ds_eps_bearer_cntxt_reset_dl_byte_count(bearer_context_p);
  ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);
  return ( DS3G_SUCCESS );
} /* ds_eps_bearer_cntxt_call_rab_re_estab_ind_hdlr */

/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_PDCPDL_RAB_REG_CNF_MSG_HDLR

DESCRIPTION
  This function handles the confirmation of the DL watermark registration 
  with PDCP
  
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
 Boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_pdcpdl_rab_reg_cnf_msg_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{
  ds_bearer_context_s        *bearer_context_p = NULL;
  eps_bearer_id_T             eps_bearer_id = 0;
  ds_eps_bearer_context_s    *eps_bearer_context_ptr = NULL;
  /*-----------------------------------------------------------------------*/

  if (dsmsg == NULL)
  {
    DS_LTE_MSG0_LOW("dsmsg is NULL");
    return DS3G_FAILURE;
  }
  eps_bearer_id = ((lte_pdcpdl_rab_register_cnf_msg_s*)dsmsg)->eps_id;

  bearer_context_p =
    ds_bearer_cntxt_get_bearer_from_eps_id(eps_bearer_id, subs_id);

  DS_LTE_MSG1_HIGH_EX(subs_id,"RAB_REG_DL_CNF for eps bearer id :%d",eps_bearer_id);
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /*----------------------------------------------------------------------
      It is possible that we are receiving this message after the call end
      due to disconnect was processed. In such a case, just ignore and ret
      DS3G_SUCCESS. Note that success is returned to the component that 
      handles incoming message. A success means only that the message was
      handled. It does not specify anything about the lower layers being
      set up right
    ----------------------------------------------------------------------*/
    return DS3G_SUCCESS;
  }

  eps_bearer_context_ptr = (ds_eps_bearer_context_s *)\
                     (bearer_context_p->ds_bearer_context_dyn_p->
                      vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_ptr))
  {
    return FALSE;
  }


  if(bearer_context_p->ds_bearer_context_dyn_p->state != 
     DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB &&
     bearer_context_p->ds_bearer_context_dyn_p->state != 
     DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
  {
    /*----------------------------------------------------------------------
      Ignore the lower layer set up since it is not expected in the states
      other than the ones mentioned above
    ----------------------------------------------------------------------*/
    DS_LTE_MSG2_HIGH_EX(subs_id,"Recd wm set up conf - bearer state: %d Bearer inst: %d. Ignore",
             bearer_context_p->ds_bearer_context_dyn_p->state,
                     (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));
    return DS3G_SUCCESS;
  }

  if(msgrtype != LTE_PDCPDL_RAB_REGISTER_CNF)
  {
    DS_LTE_MSG0_LOW("unexpected MSG type");
    return FALSE;
  }

  if( ((lte_pdcpdl_rab_register_cnf_msg_s*)dsmsg)->status != E_SUCCESS)
  {
    DS_LTE_MSG1_LOW("unexpected MSG Status: %d",
                      ((lte_pdcpdl_rab_register_cnf_msg_s*)dsmsg)->status);

      /*-------------------------------------------------------------------------  
       Register each enqueue function ptr.      
      -------------------------------------------------------------------------*/
    (bearer_context_p->tx_wm.wm_item).
      each_enqueue_func_ptr =\
         (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (bearer_context_p->tx_wm.wm_item).
      each_enqueue_func_data =\
                                (void*) bearer_context_p;

    /* if the bearer is coming out of dormancy, put it back to dormancy */
    if (bearer_context_p->ds_bearer_context_dyn_p->state 
         == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)
    {
      ds_bearer_cntxt_set_bearer_state( bearer_context_p,
                                        DS_BEARER_CONTEXT_STATE_UP_DORMANT);

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif  
    }
    /* if the bearer is being setup, put it to dormant and notify pdn that bearer
       setup is complete */
    else
    {

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP_DORMANT);
      ps_phys_link_down_ind(&bearer_context_p->ds_bearer_context_dyn_p->phys_link);
      
      /*----------------------------------------------------------------------
         Notify PDN that the bearer setup is complete
      ----------------------------------------------------------------------*/
      DS_LTE_MSG1_HIGH_EX(subs_id,"BEARER UP IND CB for eps bearer id :%d",eps_bearer_id);
      if (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
          ->bearer_up_ind_cb != NULL) 
      {
        if(bearer_context_p->ds_bearer_context_dyn_p->is_default != TRUE)
        {
          if (ds_bearer_flow_manager_activate_flow(bearer_context_p) == FALSE)
          {
            DS_LTE_MSG0_LOW("Error calling activate flow");
          }
        }
        bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
          ->bearer_up_ind_cb(\
          bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
                               bearer_context_p, NULL);
      }
    }
    return FALSE;
  }
  
  /*-----------------------------------------------------------------------
    Update the Phys Link status
  -----------------------------------------------------------------------*/
  ps_phys_link_up_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));

  /*-----------------------------------------------------------------------
    Enable flow on the primary phys link
  -----------------------------------------------------------------------*/
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                   DS_FLOW_UMTS_MASK, FALSE);

  /*-----------------------------------------------------------------------
    Send PS phys link disable for the bearer if subs is coex flow disabled
  -----------------------------------------------------------------------*/
  if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
  {
    ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p,
                                  DS_FLOW_COEX_MASK, FALSE);
  }

  /*-----------------------------------------------------------------------
    If the bearer is coming out of dormancy, there is no need to call the 
    bearer_up_ind call back.
    If this is a new bearer (bearer state is COMING_UP), we need to call
    the bearer up ind call back
  -----------------------------------------------------------------------*/
  if (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)     
  {
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP);

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif  
  }
  else if (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
  {
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP);
#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif
   
    /*----------------------------------------------------------------------
       Notify PDN that the bearer setup is complete
    ----------------------------------------------------------------------*/
    DS_LTE_MSG1_HIGH_EX(subs_id,"BEARER UP IND CB for eps bearer id :%d",eps_bearer_id);
    if (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr->bearer_up_ind_cb != NULL) 
    {
      if(bearer_context_p->ds_bearer_context_dyn_p->is_default != TRUE)
      {
          if(ds_bearer_flow_manager_activate_flow(bearer_context_p) == FALSE)
          {
            DS_LTE_MSG0_LOW("Error calling activate flow");
          }
      }
      bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr->bearer_up_ind_cb(\
                             bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
                             bearer_context_p, NULL);
    }

  }
  ds_eps_bearer_cntxt_reset_dl_byte_count(bearer_context_p);
  ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);
  ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT 
  ds3gtputmgr_update_uplink_throughput_configure(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                             FALSE);
#endif


  eps_bearer_context_ptr->ds_eps_bearer_context_dyn_p->is_registered_pdcp = TRUE;

  return DS3G_SUCCESS; 
} /* ds_eps_bearer_cntxt_pdcpdl_rab_reg_cnf_msg_hdlr */

/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_PDCPDL_RAB_REG_CNF_MSG_HDLR

DESCRIPTION
  This function handles the confirmation of the DL watermark deregistration 
  with PDCP
  
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_eps_bearer_cntxt_pdcpdl_rab_dereg_cnf_msg_hdlr 
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{
  ds_bearer_context_s     *bearer_context_p = NULL;
  eps_bearer_id_T          eps_bearer_id = 0;
  /*------------------------------------------------------------------------*/

  if (dsmsg == NULL)
  {
    DS_LTE_MSG0_LOW("dsmsg is NULL");
    return DS3G_FAILURE;
  }

  eps_bearer_id = ((lte_pdcp_rab_deregister_cnf_msg_s*)dsmsg)->eps_id;

  bearer_context_p = 
    ds_bearer_cntxt_get_bearer_from_eps_id(eps_bearer_id, subs_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /*----------------------------------------------------------------------
      It is possible that we are receiving this message after the call end
      due to disconnect was processed. In such a case, just ignore and ret
      DS3G_SUCCESS. Note that success is returned to the component that 
      handles incoming message. A success means only that the message was
      handled. It does not specify anything about the lower layers being
      set up right
    ----------------------------------------------------------------------*/
    return DS3G_SUCCESS;
  }


  if(msgrtype != LTE_PDCPDL_RAB_DEREGISTER_CNF ||
    ((lte_pdcpdl_rab_register_cnf_msg_s*)dsmsg)->status != E_SUCCESS)
  {
    DS_LTE_MSG1_LOW("unexpected MSG type or MSG Status: %d",
                      ((lte_pdcpdl_rab_register_cnf_msg_s*)dsmsg)->status);
    return FALSE;
  }
  
  DS_LTE_MSG1_HIGH_EX(subs_id,"RAB_REG_DL_CNF for eps bearer id :%d",eps_bearer_id);

  return DS3G_SUCCESS; 
} /* ds_eps_bearer_cntxt_pdcpdl_rab_reg_cnf_msg_hdlr */



/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_PDCPUL_RAB_REG_CNF_MSG_HDLR

DESCRIPTION
  This function handles the confirmation of the UL watermark registration 
  with PDCP
  
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_pdcpul_rab_reg_cnf_msg_hdlr 
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{
  ds_bearer_context_s               *bearer_context_p = NULL;
  eps_bearer_id_T                    eps_bearer_id = 0;
  ds_eps_bearer_context_flow_dir_e   bearer_flow_dir;
  ds_eps_bearer_context_s             *eps_bearer_context_ptr = NULL;
  /*------------------------------------------------------------------------*/

  if (dsmsg == NULL)
  {
    DS_LTE_MSG0_LOW("dsmsg is NULL");
    return DS3G_FAILURE;
  }

  eps_bearer_id = ((lte_pdcpul_rab_register_cnf_msg_s*)dsmsg)->eps_id;

  bearer_context_p =
    ds_bearer_cntxt_get_bearer_from_eps_id(eps_bearer_id, subs_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /*----------------------------------------------------------------------
      It is possible that we are receiving this message after the call end
      due to disconnect was processed. In such a case, just ignore and ret
      DS3G_SUCCESS. Note that success is returned to the component that 
      handles incoming message. A success means only that the message was
      handled. It does not specify anything about the lower layers being
      set up right
    ----------------------------------------------------------------------*/
    return DS3G_SUCCESS;
  }

  /*----------------------------------------------------------------------
    When switching the bearer back to LTE, bearer state will be UP.
    In that case, don't ignore the confirmation.
  ----------------------------------------------------------------------*/
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif
   
  if( bearer_context_p->ds_bearer_context_dyn_p->state != 
      DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB &&
      bearer_context_p->ds_bearer_context_dyn_p->state != 
      DS_BEARER_CONTEXT_STATE_PEND_RAB_REG )
  {
    /*----------------------------------------------------------------------
      Ignore the lower layer set up since it is not expected in the states
      other than the ones mentioned above
    ----------------------------------------------------------------------*/
    DS_LTE_MSG2_HIGH_EX(subs_id,
             "Recd wm set up conf - bearer state: %d Bearer inst: %d. Ignore",
             bearer_context_p->ds_bearer_context_dyn_p->state,
             (uint32)(bearer_context_p->
             ds_bearer_context_dyn_p->client_data_ptr));
    return DS3G_SUCCESS;
  }


  if(msgrtype != LTE_PDCPUL_RAB_REGISTER_CNF)
  {
    DS_LTE_MSG0_LOW("unexpected MSG type");
    return FALSE;
  }


  eps_bearer_context_ptr = (ds_eps_bearer_context_s *)\
                     (bearer_context_p->ds_bearer_context_dyn_p->
                      vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_ptr))
  {
    return FALSE;
  }

  if( ((lte_pdcpul_rab_register_cnf_msg_s*)dsmsg)->status != E_SUCCESS)
  {  
    /* registration with pdcp UL failed, move bearer to dormant and notify 
       pdn context that bearer setup is complete*/
    DS_LTE_MSG2_HIGH_EX(subs_id,"unexpected MSG Status: %d, "
                     "RAB_REG_UL_CNF FAILURE for eps bearer id :%d, "
                     "setting bearer to UP dormant",
                     ((lte_pdcpul_rab_register_cnf_msg_s*)dsmsg)->status,
                     eps_bearer_id);
    /* if the bearer is coming out of dormancy, put it back to dormancy */
    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)
    {

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif 

      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP_DORMANT);
      /*-------------------------------------------------------------------------  
        Register each enqueue function ptr.
      -------------------------------------------------------------------------*/
      (bearer_context_p->tx_wm.wm_item).
        each_enqueue_func_ptr =\
             (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
      (bearer_context_p->tx_wm.wm_item).
        each_enqueue_func_data =\
             (void*) bearer_context_p;
    }
    /* if the bearer is being setup, put it to dormant and notify pdn that bearer
       setup is complete */
    else
    {
      
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP_DORMANT);

      /*-------------------------------------------------------------------------  
        Register each enqueue function ptr.
      -------------------------------------------------------------------------*/
      (bearer_context_p->tx_wm.wm_item).
        each_enqueue_func_ptr =\
           (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
      (bearer_context_p->tx_wm.wm_item).
        each_enqueue_func_data =\
           (void*) bearer_context_p;

      ps_phys_link_down_ind(&bearer_context_p->ds_bearer_context_dyn_p->phys_link);

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif

      /*----------------------------------------------------------------------
         Notify PDN that the bearer setup is complete
      ----------------------------------------------------------------------*/
      DS_LTE_MSG1_HIGH_EX(subs_id,"BEARER UP IND CB for eps bearer id :%d",eps_bearer_id);
      if (bearer_context_p->ds_bearer_context_dyn_p->
          pdn_context_cb_tbl_ptr->bearer_up_ind_cb != NULL) 
      {
        if(bearer_context_p->ds_bearer_context_dyn_p->is_default != TRUE)
        {
          if(ds_bearer_flow_manager_activate_flow(bearer_context_p) == FALSE)
          {
            DS_LTE_MSG0_LOW("Error calling activate flow");
          }
        }
        bearer_context_p->ds_bearer_context_dyn_p->
          pdn_context_cb_tbl_ptr->bearer_up_ind_cb(\
          bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
                               bearer_context_p, NULL);
      }
    }
    
    return FALSE;

  }
  else
  {
    bearer_context_p->ds_bearer_context_dyn_p->
      ul_registered_with_ltepdcp = TRUE;
  }


  DS_LTE_MSG1_HIGH_EX(subs_id,"RAB_REG_ULCNF for EPS bearer id:%d",eps_bearer_id);

  /*----------------------------------------------------------------------
    Get the flow direction for this bearer
  --------------------------------------------------------------------*/
  bearer_flow_dir = ds_eps_bearer_cntxt_getFlowDirection(bearer_context_p);
  if (bearer_flow_dir == DS_EPS_FLOW_DIR_BIDIRECTIONAL)
  {
    ds_eps_bearer_cntxt_register_dl_datapath(bearer_context_p);
  }
  else if (bearer_flow_dir == DS_EPS_FLOW_DIR_UPLINK)
  {
    /*-----------------------------------------------------------------------
      Lower layer setup is complete
    -----------------------------------------------------------------------*/
    ps_phys_link_up_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));

    /*----------------------------------------------------------------------
      Enable flow on the primary phys link
    ----------------------------------------------------------------------*/
    ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                     DS_FLOW_UMTS_MASK, FALSE);

    /*-----------------------------------------------------------------------
      Send PS phys link disable for the bearer if subs is coex flow disabled
    -----------------------------------------------------------------------*/
    if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
    {
      ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p,
                                   DS_FLOW_COEX_MASK, FALSE);
    }

    /*-----------------------------------------------------------------------
      If the bearer is coming out of dormancy, there is no need to call the 
      bearer_up_ind call back.
      If this is a new bearer (bearer state is COMING_UP), we need to call
      the bearer up ind call back
    -----------------------------------------------------------------------*/
    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)     
    {
  
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP);

#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif  

    }
    else if (bearer_context_p->ds_bearer_context_dyn_p->state == 
             DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
    {
#ifdef FEATURE_LAPP_UFS 
      #error code not present
#endif
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP);
     
      /*----------------------------------------------------------------------
         Notify PDN that the bearer setup is complete
      ----------------------------------------------------------------------*/
      DS_LTE_MSG1_HIGH_EX(subs_id,"BEARER UP IND CB for eps bearer id :%d",eps_bearer_id);
      if (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
          ->bearer_up_ind_cb != NULL) 
      {
        if(bearer_context_p->ds_bearer_context_dyn_p->is_default != TRUE)
        {
          if (ds_bearer_flow_manager_activate_flow(bearer_context_p) == FALSE)
          {
            DS_LTE_MSG0_LOW("Error calling activate flow");
          }
        }
        bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
          ->bearer_up_ind_cb(\
                               bearer_context_p->
                             ds_bearer_context_dyn_p->pdn_context_ptr,
                               bearer_context_p, NULL);
      }
    }
    
    ds_eps_bearer_cntxt_reset_dl_byte_count(bearer_context_p);
    ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);


    eps_bearer_context_ptr->ds_eps_bearer_context_dyn_p->is_registered_pdcp = TRUE;

  }
  else
  {
    // invalid case
    DS_LTE_MSG1_ERROR_EX(subs_id, "Invalid flow dir for bearer_context:x",
                      bearer_context_p);
    return FALSE;
  }
 
  DS_LTE_MSG1_LOW("Sent RAB_REG_DL_REQ for EPS bearer ID:%d",eps_bearer_id); 
     
  return TRUE;   

} /* ds_eps_bearer_cntxt_pdcpul_rab_reg_cnf_msg_hdlr */

/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_PDCPUL_RAB_DEREG_CNF_MSG_HDLR

DESCRIPTION
  This function handles the confirmation of the UL watermark deregistration 
  with PDCP
  
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_pdcpul_rab_dereg_cnf_msg_hdlr 
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{
  ds_bearer_context_s          *bearer_context_p = NULL;
  eps_bearer_id_T               eps_bearer_id = 0;
  /*------------------------------------------------------------------------*/

  if (dsmsg == NULL)
  {
    DS_LTE_MSG0_LOW("dsmsg is NULL");
    return DS3G_FAILURE;
  }

  eps_bearer_id = ((lte_pdcp_rab_deregister_cnf_msg_s*)dsmsg)->eps_id;

  bearer_context_p =
    ds_bearer_cntxt_get_bearer_from_eps_id(eps_bearer_id, subs_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /*----------------------------------------------------------------------
      It is possible that we are receiving this message after the call end
      due to disconnect was processed. In such a case, just ignore and ret
      DS3G_SUCCESS. Note that success is returned to the component that 
      handles incoming message. A success means only that the message was
      handled. It does not specify anything about the lower layers being
      set up right
    ----------------------------------------------------------------------*/
    return DS3G_SUCCESS;
  }

  
  if(msgrtype != LTE_PDCPUL_RAB_DEREGISTER_CNF)
  {
    DS_LTE_MSG0_LOW("unexpected MSG type");
    return FALSE;
  }

  /* Setting ul_reg_with ltepdcp to FALSE as WM is now deregistered */
  bearer_context_p->ds_bearer_context_dyn_p->ul_registered_with_ltepdcp = FALSE;  

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif

  if( ((lte_pdcp_rab_deregister_cnf_msg_s*)dsmsg)->status != E_SUCCESS)
  {    
    /* deregistration with pdcp UL failed, move bearer to dormant and notify 
       pdn context that bearer setup is complete*/
    DS_LTE_MSG2_HIGH_EX(subs_id,"unexpected MSG Status: %d, "
                     "PDCP_DEREG_UL_CNF FAILURE for eps bearer id :%d, "
                     "setting bearer to UP dormant",
                     ((lte_pdcpul_rab_register_cnf_msg_s*)dsmsg)->status,
                      eps_bearer_id);
    
    return FALSE;
  }

  return TRUE;

} /* ds_eps_bearer_cntxt_pdcpul_rab_dereg_cnf_msg_hdlr */

/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_PDCPUL_PKT_DISCARD_IND_MSG_HDLR

DESCRIPTION
  This is a function to send packet discard indication to IMS so they can
  relinquish TRM.
 
PARAMETERS
  msgrtype  -- Message UMID 
  subs_id   -- Subscription ID
  dsmsg     -- Pointer to payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_eps_bearer_cntxt_pdcpul_pkt_discard_ind_msg_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{
  eps_bearer_id_T                  eps_id = 0;
  ds_bearer_context_s              *bearer_cntxt_ptr = NULL;
  ds_pdn_context_s                 *pdn_cntxt_ptr = NULL;
  ps_phys_link_type                *phys_link_ptr = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (dsmsg == NULL)
  {
    return DS3G_FAILURE;
  }

  eps_id = ((lte_pdcpul_pkt_discard_msg_s*)dsmsg)->eps_id;
  DS_LTE_MSG1_HIGH_EX(subs_id,"LTE PDCP UL PKT DISCARD MSG eps_id: %d",eps_id);

  bearer_cntxt_ptr = ds_bearer_cntxt_get_bearer_from_eps_id(eps_id, subs_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntxt_ptr))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto exit_func;
  }

  if(!(bearer_cntxt_ptr->ds_bearer_context_dyn_p->is_default))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto exit_func;
  }

  pdn_cntxt_ptr = (ds_pdn_context_s*)ds_bearer_cntxt_get_pdn_cntxt(bearer_cntxt_ptr);

  if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntxt_ptr))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    goto exit_func;
  }

  if(!ds3gpp_pdn_context_is_ims_pdn(pdn_cntxt_ptr,subs_id))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto exit_func;
  }
  
  if(ds_eps_bearer_cntxt_get_qci_info(bearer_cntxt_ptr) != LTE_QCI_5)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
    goto exit_func;
  }

  phys_link_ptr = &(bearer_cntxt_ptr->ds_bearer_context_dyn_p->phys_link);

  if(!PS_PHYS_LINK_IS_VALID(phys_link_ptr))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_5;
    goto exit_func;
  }

  ps_phys_link_packet_discard_ind(phys_link_ptr);  //Genrate event to Interfaces

exit_func:
  DS_LTE_MSG1_HIGH_EX(subs_id,"In func ds_eps_bearer_cntxt_pdcpul_pkt_discard_ind_msg_hdlr: local_err = %d",
                      local_err);
	
  return DS3G_SUCCESS;

} /* ds_eps_bearer_cntxt_pdcpul_pkt_discard_ind_msg_hdlr */


/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_UE_CAP_INFO_IND_MSG_HDLR

DESCRIPTION
  This function is used to handle the message from RRC. This message indicates
  LTE capability info.
  
PARAMETERS
  msgrtype  - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_ue_cap_info_ind_msg_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
)
{
  ds3gsubsmgr_subs_id_e_type                   subs_indx = 
                                                  DS3GSUBSMGR_SUBS_ID_INVALID;
  lte_ue_cap_info_s                           *msg_ptr;
  /*------------------------------------------------------------------------*/

  if(dsmsg_ptr == NULL)
  {
    DS_LTE_MSG0_ERROR_EX(subs_id, "dsmsg_ptr is NULL");
    return FALSE;
  }

  if(msgrtype != LTE_RRC_UE_CAP_INFO_IND)
  {
    DS_LTE_MSG0_LOW("unexpected MSG type");
    return FALSE;
  }

  msg_ptr = (lte_ue_cap_info_s*)dsmsg_ptr;

  subs_indx = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);
  if(ds3gsubsmgr_is_ds3g_subs_index_valid(subs_indx) == FALSE)
  {
    return FALSE;
  }

  if ((msg_ptr)->valid == FALSE)
  {
    DS_LTE_MSG0_ERROR_EX(subs_id, "LTE RRC UE CAP INFO is not valid");
    return FALSE;
  }

  /* Update ue cat cache if it changes */
  if (ds_eps_bearer_cntxt_ue_cat_id[subs_indx] != (msg_ptr)->ue_cat)
  {
    DS_LTE_MSG3_HIGH_EX(subs_id,"LTE category for Subs index %d changed. Old: %d, New: %d",
                     subs_indx, 
                     ds_eps_bearer_cntxt_ue_cat_id[subs_indx],
                     (msg_ptr)->ue_cat);

    ds_eps_bearer_cntxt_ue_cat_id[subs_indx] = (msg_ptr)->ue_cat;
  }

  return TRUE;
} /* ds_eps_bearer_cntxt_ue_cap_info_ind_msg_hdlr */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_BEARER_ACTIVATE_IND

DESCRIPTION
  This function sets up the bearer context when bearer activation indication
  is processed.
  
PARAMETERS  
  call_id:     CM call id
  mode_info_p: mode specific info pointer
    
DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
static boolean ds_eps_bearer_cntxt_bearer_activate_ind
(
  cm_call_id_type                 call_id,
  const cm_call_mode_info_s_type  *mode_info_p
)
{
  ps_phys_link_link_protocol_handle_type protocol_handle;
  ds_bearer_context_s                    *bearer_context_p;
  uint32                                 bearer_context_inst;
  boolean                                result = DS3G_FAILURE;
  ds_pdn_context_s                      *pdn_context_p = NULL;
  cm_lte_call_info_s_type               *lte_call_info = NULL;
#ifdef FEATURE_DATA_REL10
  byte                                  *apn = NULL;
  byte                                   decoded_apn[DS_UMTS_MAX_APN_STRING_LEN];
#endif /*FEATURE_DATA_REL10*/
  sys_modem_as_id_e_type                 subs_id = SYS_MODEM_AS_ID_NONE;
  ds_eps_tlb_per_subs_info_type         *hndl = NULL;
  boolean                                tlb_hndl_is_valid = FALSE;
  uint32                                 rat_mask = DS_3GPP_THROT_RAT_MASK_LTE;
  ds_eps_bearer_context_dyn_s           *eps_bearer_cntxt_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Get the bearer context from CM_Call_Id
    If PDN context Activate bearer Ind returned success, we should not have
    any problem finding the bearer context corresponding to the CM call Id
    So assert if we cannot find bearer from CM call id.
  -------------------------------------------------------------------------*/
  bearer_context_p = ds_bearer_cntxt_get_bearer_from_call_id(call_id);

  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p) == FALSE)
  {
    DS3GPP_ASSERT(0);
    return FALSE;
  }

  memset(&protocol_handle, 0x0, sizeof(ps_phys_link_link_protocol_handle_type));

  pdn_context_p = (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

  if (!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_context_p, &subs_id))
  {
    return FALSE;
  }

  bearer_context_inst = (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr;

  lte_call_info = (cm_lte_call_info_s_type*) &(mode_info_p->info.lte_call) ;
  /*------------------------------------------------------------------------- 
    If NW provided TI for GW, the bearer can be transfered to GW RAT.
  -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW =
                     mode_info_p->info.lte_call.gsm_umts_connection_id.valid;

  /*-------------------------------------------------------------------------
    Associate the mode specific blob that is going to be populated to the 
    bearer context pointer.
  -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ] =\
    (void*)&eps_bearer_context_tbl[bearer_context_inst];

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt
     (&eps_bearer_context_tbl[bearer_context_inst]))
  {
    return FALSE;
  }

  eps_bearer_cntxt_ptr = 
     (eps_bearer_context_tbl[bearer_context_inst].ds_eps_bearer_context_dyn_p);

  /*------------------------------------------------------------------------- 
    Associate this EPS bearer context with bearer context
  -------------------------------------------------------------------------*/
  eps_bearer_cntxt_ptr->bearer_context_p = bearer_context_p;

  /*------------------------------------------------------------------------- 
    Update the EPS bearer id for this bearer
  -------------------------------------------------------------------------*/
  eps_bearer_cntxt_ptr->eps_bearer_id = lte_call_info->eps_bearer_id;  

  /*------------------------------------------------------------------------- 
    Store the EPS bearer id for this bearer int the Phys Link data structure
 -------------------------------------------------------------------------*/
  PS_PHYS_LINK_SET_BEARER_ID(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                             eps_bearer_cntxt_ptr->eps_bearer_id);

  /*------------------------------------------------------------------------- 
    Update the EPS linked bearer id for this bearer
  -------------------------------------------------------------------------*/
  eps_bearer_cntxt_ptr->lbi = lte_call_info->lbi;                                      

  hndl = ds_eps_tlb_get_per_subs_info_hndl(subs_id);

  if (ds_eps_tlb_validate_per_subs_info_hndl(hndl))
  {
    tlb_hndl_is_valid = TRUE;
  }

  /*------------------------------------------------------------------------- 
    Put the bearer into TLB state if TLB is active
  -------------------------------------------------------------------------*/
  if (tlb_hndl_is_valid &&
      ((hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_ACTIVE) ||
      (hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_A_CLOSED)))
  {
    eps_bearer_cntxt_ptr->is_in_tlb = TRUE;
  }

  /*-------------------------------------------------------------------------
    Set the Tx function 
  -------------------------------------------------------------------------*/
  if (ds_3gpp_nv_manager_get_data_shark() == TRUE)
  {
    DS_LTE_MSG0_HIGH_EX(subs_id,"DS_3GPP_DATA_SHARK_ENABLE NV enabled. "
                     "Registering loopback tx cb");
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                 ds_3gpp_loopback_pdp_data_ctrl_setup_cb,
                                 bearer_context_p);
  }
  else
  {
    if (tlb_hndl_is_valid &&
        ((hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_ACTIVE) ||
         (hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_A_CLOSED)) &&
         (hndl->tlb_state.mode == LTE_TLB_MODE_A))
    {
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->
                                   phys_link),
                                   dsEpsTlb_pdp_ip_tx_um_data_cb,
                                   bearer_context_p);
    } 
    else 
    {
      ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                   ds_eps_bearer_cntxt_pdp_ip_tx_um_data_cb,
                                   bearer_context_p);
    }
  }
  
  /*---------------------------------------------------------------------------
   * We are filling the protocol handle structure with the iface pointers
  ---------------------------------------------------------------------------*/

  if (((bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr) == NULL) || 
      (!ds_3gpp_pdn_cntx_validate_pdn_context((bearer_context_p->
                                      ds_bearer_context_dyn_p->pdn_context_ptr))))
  {
    return FALSE;
  }

  if(pdn_context_p->ds_pdn_context_dyn_p->
     ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE] != NULL)
  {
    protocol_handle.none_handle.handle.ip_proto_handle.v4_iface_ptr = \
                 &(pdn_context_p->
                ds_pdn_context_dyn_p->
              ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]->ps_iface);
  }

  if(pdn_context_p->ds_pdn_context_dyn_p->
     ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE] != NULL)
  {
    protocol_handle.none_handle.handle.ip_proto_handle.v6_iface_ptr = \
                 &(pdn_context_p->
                   ds_pdn_context_dyn_p->
                   ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]->ps_iface);
  }

  protocol_handle.none_handle.high_protocol =
                              PS_PHYS_LINK_HIGHER_LAYER_PROTOCOL_IP;

  /*---------------------------------------------------------------------------
    Set the link layer protocol to NONE and pass the protocol handle into the
    function that will do this. The protocol handle has just been populated
    with the higher layer protocol and the pointers to the ifaces that might
    communicate over this physlink.
  ---------------------------------------------------------------------------*/
  (void) ps_phys_link_set_link_protocol(&(bearer_context_p->ds_bearer_context_dyn_p->
                                          phys_link),
                                        PS_PHYS_LINK_LINK_PROTOCOL_NONE,
                                        protocol_handle);
  
  /*-------------------------------------------------------------------------
    Set the handler for the downlink signal for the call.
  -------------------------------------------------------------------------*/
  if (tlb_hndl_is_valid &&
      ((hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_ACTIVE) ||
       (hndl->tlb_state.ds_eps_bearer_tlb_state == DS_EPS_BEARER_TLB_A_CLOSED)) &&
       (hndl->tlb_state.mode == LTE_TLB_MODE_A))
  {
    /*-------------------------------------------------------------------------
      Set the handler for the downlink signal for the call.
      PS handles are assigned when the bearer is allocated, and invalidated
      when the bearer is deallocated. The original handle is passed to the 
      callback to make sure that only bearers with valid handles are processed.
    -------------------------------------------------------------------------*/
    (void)ps_set_sig_handler(
                    bearer_context_p->ds_bearer_context_dyn_p->rx_sig,
                    dsEpsTlb_pdp_ip_rx_um_drop_data_cb, 
                  (void *)bearer_context_p->ds_bearer_context_dyn_p->ps_handle);
  } else {
    (void)ps_set_sig_handler(
                  bearer_context_p->ds_bearer_context_dyn_p->rx_sig,
                    ds_eps_bearer_cntxt_pdp_ip_rx_um_data_cb, 
                  (void *)bearer_context_p->ds_bearer_context_dyn_p->ps_handle);
  }

  /*-------------------------------------------------------------------------  
    Enable it.
  -------------------------------------------------------------------------*/
  ps_enable_sig(bearer_context_p->ds_bearer_context_dyn_p->rx_sig);

      /*---------------------------------------------------------------------
        Allocate timer handle for dynamic back pressure disable
      ---------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure
        = ps_timer_alloc(ds_bearer_cntxt_tmr_dyn_back_pressure_exp_hdlr,
                         (void *)bearer_context_p);

      if (PS_TIMER_INVALID_HANDLE == 
          bearer_context_p->ds_bearer_context_dyn_p->
          tmr_hndl_dyn_back_pressure)
      {
        DS_LTE_MSG1_HIGH_EX(subs_id,"Invalid PS timer handle for bearer inst: %d",
                           (uint32)bearer_context_p->
                          ds_bearer_context_dyn_p->client_data_ptr);
      }

      bearer_context_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter 
        = bearer_context_p->ds_bearer_context_dyn_p->last_tcp_dl_ack_count = 0;

  /*-------------------------------------------------------------------------  
    Enable rm_flow_enabled
  -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->rm_flow_enabled = TRUE;

  /*-------------------------------------------------------------------------  
    For Dedicated Bearer bind the phys_link to Flow. This is called whenever 
    the bearer is newly setup, and can happen for NW-initiated and 
    UE-initiated. 
  -------------------------------------------------------------------------*/
  result = ds_bearer_flow_manager_bind_flow(bearer_context_p);
  if(result == FALSE)
  {
    DS_LTE_MSG0_LOW("Bind Flow failed for bearer");
  }


  // mlushin TODO:
  // Save RBID once NAS adds support

  /*-------------------------------------------------------------------------
    Move the state of the bearer to DS_BEARER_CONTEXT_STATE_PEND_RAB_REG
    This indicates the fact that we have responded to the Activate bearer
    indication from CM with DS3G_SUCCESS.

    Should there be a tear down requested from the APP, we will send a PDN
    disconnect and not a PDN Abort.
  -------------------------------------------------------------------------*/

  ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                   DS_BEARER_CONTEXT_STATE_PEND_RAB_REG);

#ifdef FEATURE_DATA_REL10
  /*---------------------------------------------------------------------
    EPS Bearer context activate ind got successfull processed by UE.
    Clear APN throttling state machine if it was globally blocked.
  ----------------------------------------------------------------------*/
  if(ds_3gpp_nv_manager_get_nv_lte_rel_10(subs_id))
  {

    if (lte_call_info->apn_name.valid)
    {
      apn = lte_call_info->apn_name.address;
      memset(decoded_apn, 0, DS_UMTS_MAX_APN_STRING_LEN);
      if((apn!=NULL)&&
         (lte_call_info->apn_name.apn_addr_len <= DS_UMTS_MAX_APN_STRING_LEN) &&
         (ds3g_decode_apn_name(lte_call_info->apn_name.address,
            (uint32)lte_call_info->apn_name.apn_addr_len,
            decoded_apn) == TRUE))
      {
        apn = decoded_apn;
      }
      else
      {
        DS_3GPP_ERROR_LOG_0_EX(subs_id, "unable to decode the APN");
        apn = pdn_context_p->ds_pdn_context_dyn_p->pdp_profile.context.apn;
      }
    }
    else
    {
      apn = pdn_context_p->ds_pdn_context_dyn_p->pdp_profile.context.apn;
    }
    DATA_3GPP_MSG_SPRINTF_1(MSG_LEGACY_HIGH,
                            "Trying to unblock potentially blocked APN: %s",
                            (char*)apn);

    /*---------------------------------------------------------------------
      Unblock the APN on Global RAT and LTE RAT if throttling per RAT is
      enabled
    ----------------------------------------------------------------------*/

    if (ds_3gpp_nv_manager_get_enable_pdn_throt_per_rat(subs_id)||
        ds_3gppi_is_rel_12_or_above_supported(subs_id))
    {
      rat_mask |= DS_3GPP_THROT_RAT_MASK_LTE;
    }

    ds_3gpp_pdn_throt_handle_nw_inited_unblock_apn(apn,subs_id, rat_mask);
  }
#endif /*FEATURE_DATA_REL10*/

  return DS3G_SUCCESS;
} /* ds_eps_bearer_cntxt_bearer_activate_ind */

/*===========================================================================
STATIC FUNCTION DS_EPS_BEARER_CONTEXT_MAP_CAUSE_CODES

DESCRIPTION
  

PARAMETERS
  map_rule     - Rule for mapping cause-codes
  *output_ptr  - ptr to location holding converted cause-code
  *input_ptr   - ptr to location with cause-codes to be converted

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_map_cause_codes
(
    ds3gpp_inter_layer_map_e map_rule,
    void                    *output_ptr,
    const void              *input_ptr,
    sys_modem_as_id_e_type   subs_id 
)
{
  ps_iface_net_down_reason_type    *ps_net_down_output_ptr = NULL;  
                                                  /*PS IFACE NET DOWN REASON */
  const cm_call_mode_info_s_type   *mode_info_p;
  esm_cause_enum_T                 esm_cause;
  lte_nas_local_cause_enum_T       esm_local_cause;
  emm_failure_type                 emm_cause;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mode_info_p = (const cm_call_mode_info_s_type *)input_ptr;

  /*------------------------------------------------------------------------- 
    We look at the call end cause codes in the following order:
    1. EMM Cause codes
    2. ESM Cause codes.
    3. ESM Local Cause codes.
   
    IMPORTANT:
    Whenever adding a new PS Net Down reason, the convention is as follows:
   
    1. If a PS Net Down reason is for ESM Cause codes, add to
       Net down reason category(type): 6
       The value field should be the same as the ESM Cause code value.
       For example: PS_NET_DOWN_REASON_IP_V4_ONLY_ALLOWED - Type: 6, Value: 50
   
    2. If a PS Net Down reason is for EMM/ESM Local Cause codes, add to
       Net down reason category(type): 3
    -------------------------------------------------------------------------*/

  if (map_rule == CALL_END_EMM_CAUSE_TO_PS_IFACE_CAUSE_CODES)
  {
    DS_LTE_MSG2_MED_EX(subs_id,
        "mapping CALL_END_EMM_CAUSE_TO_PS_IFACE_CAUSE_CODES,"
        "Cause type: %d, Detailed cause: %d",
        mode_info_p->info.lte_call.emm_failure_cause.cause_type,
        mode_info_p->info.lte_call.emm_failure_cause.detailed_cause);

    ps_net_down_output_ptr = (ps_iface_net_down_reason_type *) output_ptr;

    switch(mode_info_p->info.lte_call.emm_failure_cause.cause_type)
    {
	  case LTE_NAS_IRAT_ATTACH_REJECT:
	  {
		switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
        {
		  case LTE_NAS_NW_FAILURE:
            *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NETWORK_FAILURE;
            break;  
			 
          case LTE_NAS_CONGESTION:
            *ps_net_down_output_ptr = PS_NET_DOWN_REASON_CONGESTION;
            break;
			 
		  default:
                         DS_LTE_MSG1_ERROR_EX(subs_id,
                "Unable to map att rej EMM Cause d to PS Net Down Reason",
                 mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
            *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
            break;
		}
		break;
	  }
      case LTE_NAS_IRAT_CONN_EST_FAILURE:
      {
        switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
        {
          case LTE_RRC_CONN_EST_FAILURE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE;
             break;
           case LTE_RRC_CONN_EST_FAILURE_ABORTED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_ABORTED;
             break;
           case LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_ACCESS_BARRED;
             break;  
           case LTE_RRC_CONN_EST_FAILURE_CELL_RESEL:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CELL_RESEL;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_CONFIG_FAILURE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_TIMER_EXPIRED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_LINK_FAILURE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_LINK_FAILURE;
             break;  
           case LTE_RRC_CONN_EST_FAILURE_NOT_CAMPED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_NOT_CAMPED;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_SI_FAILURE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_SI_FAILURE;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_CONN_REJECT:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONN_REJECT;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_REQ_ERROR:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_REQ_ERROR;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_TAI_CHANGE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_TAI_CHANGE;
             break;    
           case LTE_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE;
             break;    
           default:
             DS_LTE_MSG1_ERROR_EX(subs_id,
                "Unable to map EMM Cause d to PS Net Down Reason",
                 mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
             break;
        } /* switch(detailed_cause) */
        break;
      } /* case LTE_NAS_IRAT_CONN_EST_FAILURE*/

       case LTE_NAS_IRAT_CONN_REL:
       {
         switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
         {
           case LTE_RRC_CONN_REL_NORMAL:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_NORMAL;
             break;
           case LTE_RRC_CONN_REL_RLF:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_RLF;
             break;
           case LTE_RRC_CONN_REL_CRE_FAILURE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_REL_CRE_FAILURE;
             break;
           case LTE_RRC_CONN_REL_OOS_DURING_CRE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_REL_OOS_DURING_CRE;
             break;
           case LTE_RRC_CONN_REL_ABORTED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_ABORTED;
             break;
           case LTE_RRC_CONN_REL_SIB_READ_ERROR:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_CONN_REL_SIB_READ_ERROR;
             break;
           case LTE_RRC_CONN_REL_ABORTED_IRAT_SUCCESS:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_REL_ABORTED_IRAT_SUCCESS;
             break;
           case LTE_RRC_CONN_REL_RLF_SEC_NOT_ACTIVE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_REL_RLF_SEC_NOT_ACTIVE;
             break;
           case LTE_RRC_CONN_REL_IRAT_TO_LTE_ABORTED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_TO_LTE_ABORTED;
             break;
           case LTE_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_SUCCESS:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_SUCCESS;
             break;
           case LTE_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_ABORTED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_RRC_CONN_REL_IRAT_FROM_LTE_TO_G_CCO_ABORTED;
             break;
           default:
              DS_LTE_MSG1_ERROR_EX(subs_id,
                 "Unable to map EMM Cause d to PS Net Down Reason!",
                  mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
              *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
              break;
         } /* switch(detailed_cause) */
         break;
       } /* case LTE_NAS_IRAT_CONN_REL*/

       case LTE_NAS_IRAT_UL_MSG_CNF_FAILURE:
       {
         switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
         {          
           case LTE_RRC_UL_DATA_CNF_FAILURE_TXN:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_TXN;
             break;
           case LTE_RRC_UL_DATA_CNF_FAILURE_HO:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_HO;
             break;
           case LTE_RRC_UL_DATA_CNF_FAILURE_CONN_REL:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CONN_REL;
             break;
           case LTE_RRC_UL_DATA_CNF_FAILURE_RLF:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_RLF;
             break;
           case LTE_RRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN;
             break;
           default:
              DS_LTE_MSG1_ERROR_EX(subs_id,
                 "Unable to map EMM Cause d to PS Net Down Reason",
                  mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
              *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
              break;
         } /* switch(detailed_cause) */

         break;
       } /* case LTE_NAS_IRAT_UL_MSG_CNF_FAILURE*/

       case LTE_NAS_IRAT_SR_REJECT:
       {
         switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
         {    
           case LTE_NAS_IMSI_UNKNOWN_IN_HSS:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IMSI_UNKNOWN_IN_HSS;
             break;
           case LTE_NAS_ILLEGAL_UE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ILLEGAL_MS;
             break;
           case LTE_NAS_IMEI_NOT_ACCEPTED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IMEI_NOT_ACCEPTED;
             break;
           case LTE_NAS_ILLEGAL_ME:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ILLEGAL_ME;
             break;
           case LTE_NAS_EPS_SERVICES_NOT_ALLOWED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_EPS_SERVICES_NOT_ALLOWED;
             break;
           case LTE_NAS_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED;
             break;
           case LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK;
             break;
           case LTE_NAS_IMPLICITLY_DETACHED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IMPLICITLY_DETACHED;
             break;
           case LTE_NAS_PLMN_NOT_ALLOWED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PLMN_NOT_ALLOWED;
             break;
           case LTE_NAS_TRACKING_AREA_NOT_ALLOWED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_TRACKING_AREA_NOT_ALLOWED;
             break;
           case LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA;
             break;
           case LTE_NAS_EPS_SERVICES_NOT_ALLOWED_IN_PLMN:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_EPS_SERVICES_NOT_ALLOWED_IN_PLMN;
             break;
           case LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
             break;
           case LTE_NAS_MSC_TEMPORARILY_NOT_REACHABLE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MSC_TEMPORARILY_NOT_REACHABLE;
             break;
           case LTE_NAS_NW_FAILURE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NETWORK_FAILURE;
             break;
           case LTE_NAS_CS_DOMAIN_NOT_AVAILABLE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_CS_DOMAIN_NOT_AVAILABLE;
             break;
           case LTE_NAS_ESM_FAILURE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_FAILURE;
             break;
           case LTE_NAS_MAC_FAILURE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MAC_FAILURE;
             break;
           case LTE_NAS_SYNCH_FAILURE:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_SYNCH_FAILURE;
             break;
           case LTE_NAS_CONGESTION:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_CONGESTION;
             break;
           case LTE_NAS_UE_SECURITY_CAPABILITIES_MISMATCH:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_UE_SECURITY_CAPABILITIES_MISMATCH;
             break;
           case LTE_NAS_SECURITY_MODE_REJ_UNSPECIFIED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_SECURITY_MODE_REJ_UNSPECIFIED;
             break;
           case LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_NOT_AUTHORIZED_FOR_THIS_CSG;
             break;
           case LTE_NON_EPS_AUTH_UNACCEPTABLE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_NON_EPS_AUTH_UNACCEPTABLE;
             break;
           case LTE_NAS_CS_FALLBACK_CALL_EST_NOT_ALLOWED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_CS_FALLBACK_CALL_EST_NOT_ALLOWED;
             break;
           case LTE_NAS_CS_DOMAIN_TEMP_NOT_AVAILABLE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_CS_DOMAIN_NOT_AVAILABLE;
             break;
           case LTE_NAS_NO_EPS_BEARER_CONTEXT_ACTIVATED:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_NO_EPS_BEARER_CONTEXT_ACTIVATED;
             break;
           case LTE_NAS_SEMANTICALLY_INCORRECT_MSG:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MESSAGE_INCORRECT_SEMANTIC;
             break;
           case LTE_NAS_INVALID_MANDATORY_INFO:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_INVALID_MANDATORY_INFO;
             break;
           case LTE_NAS_NON_EXISTENT_MSG_TYPE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MESSAGE_TYPE_UNSUPPORTED;
             break;
           case LTE_NAS_MSG_TYPE_INCOMPATIBLE_WITH_PROTOCOL_STATE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MSG_TYPE_NONCOMPATIBLE_STATE;
             break;
           case LTE_NAS_NON_EXISTANT_INFO:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_UNKNOWN_INFO_ELEMENT;
             break;
           case LTE_NAS_CONDITIONAL_IE_ERROR:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_CONDITIONAL_IE_ERROR;
             break;
           case LTE_NAS_MSG_INCOMPATIBLE_WITH_PROTOCOL_STATE:
             *ps_net_down_output_ptr = 
                PS_NET_DOWN_REASON_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE;
             break;
           case LTE_NAS_PROTOCOL_ERROR_UNSPECIFIED:
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PROTOCOL_ERROR;
             break;

           default:
             DS_LTE_MSG1_ERROR_EX(subs_id,
                "Unable to map EMM Cause d to PS Net Down Reason!",
                 mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
             *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
             break;
           } /* switch(detailed_cause) */

           break;
         } /* case LTE_NAS_IRAT_SR_REJECT */

         case LTE_NAS_IRAT_INVALID_STATE:
         {
          switch(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause)
          {    
            case EMM_INVALID_STATE_IMEI_NOT_ACCEPTED_BY_NW:
              *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IMEI_NOT_ACCEPTED;
             break;
		 
	    case EMM_INVALID_STATE_NO_TRM_RESOURCES:
              *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NO_TRM;
             break;
			
	    default: 
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_INVALID_STATE;
           break;
           }
	   break;
          } /* case LTE_NAS_IRAT_INVALID_STATE */
 
         case LTE_NAS_IRAT_ACCESS_BARRING:
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_ACCESS_BARRED;
            break;

         case LTE_NAS_IRAT_T3417_EXPIRY:
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_T3417_EXPIRED;
           break;
 
         case LTE_NAS_IRAT_LOWER_LAYER_FAILURE:
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NAS_LAYER_FAILURE;
           break;

         case LTE_NAS_IRAT_T3410_EXPIRY:
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_PROC_TIME_OUT;
            break;

         default:
         {
           DS_LTE_MSG1_ERROR_EX(subs_id,
              "Unable to map EMM Cause d to PS Net Down Reason",
               mode_info_p->info.lte_call.emm_failure_cause.detailed_cause );
           *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
           break;
         }
       } /* switch(mode_info_p->info.lte_call.emm_failure_cause.cause_type) */ 

  } /* EMM Cause codes*/
    
  else if (mode_info_p->info.lte_call.esm_cause.valid)
  {
  switch(map_rule)
  {
    case CALL_END_ESM_CAUSE_TO_PS_IFACE_CAUSE_CODES:
      ps_net_down_output_ptr = (ps_iface_net_down_reason_type *) output_ptr;
        esm_cause = mode_info_p->info.lte_call.esm_cause.esm_cause;
  
      DS_LTE_MSG1_MED_EX(subs_id,"mapping CALL_END_ESM_CAUSE_TO_PS_IFACE_CAUSE_CODES, input_cause_code:%d",
                      (uint32)(esm_cause));

        ds_eps_bearer_cntxt_map_esm_to_ps_down_reason(ps_net_down_output_ptr, 
                                                      esm_cause);      
      break;

      default:
        DS_LTE_MSG0_ERROR_EX(subs_id, "EVENT_CAUSE_CODES: Mapping Rule not implemented!");
        return FALSE;
    }
  } /* ESM Cause codes*/
  else if (mode_info_p->info.lte_call.esm_local_cause.valid)
  {
    switch(map_rule)
    {
      case CALL_END_ESM_CAUSE_TO_PS_IFACE_CAUSE_CODES:
        ps_net_down_output_ptr = (ps_iface_net_down_reason_type *) output_ptr;
        esm_local_cause = mode_info_p->info.lte_call.esm_local_cause.local_cause;
    
        DS_LTE_MSG1_MED_EX(subs_id,
          "mapping CALL_END_LOCAL_ESM_CAUSE_TO_PS_IFACE_CAUSE_CODES, input_cause_code:%d",
          (uint32)(esm_local_cause));

        ds_eps_bearer_cntxt_map_esm_local_to_ps_down_reason(
           ps_net_down_output_ptr,esm_local_cause);

        break;

        default:
          DS_LTE_MSG0_ERROR_EX(subs_id, "EVENT_CAUSE_CODES: Mapping Rule not implemented!");
    }
  } /* ESM local cause codes*/
  /* Check EMM cause code. On access barring, update ESM cause.
     EMM cause exists for EPS procedures only.
     Compiler cannot convert cm_emm_failure_type to emm_failure_type. Conversion
     is done on a per field basis */
  //emm_cause = (emm_failure_type) mode_info_p->info.lte_call.emm_failure_cause;
  
  if(map_rule == CALL_END_EMM_CAUSE_TO_PS_IFACE_CAUSE_CODES)
  {
    ps_net_down_output_ptr = (ps_iface_net_down_reason_type *) output_ptr;

    emm_cause.access_barring_rem_time = 
                  mode_info_p->info.lte_call.emm_failure_cause.access_barring_rem_time;
    emm_cause.cause_type = (emm_irat_failure_cause_type) 
                  mode_info_p->info.lte_call.emm_failure_cause.cause_type;
     
    memscpy((void *)&emm_cause.cause, sizeof(uint16),
            (void *)&(mode_info_p->info.lte_call.emm_failure_cause.detailed_cause),
            sizeof(uint16));
    
    DS_3GPP_MSG3_HIGH("EMM_CAUSE:%d:EST_STATUS:%d:ACC_BARR_TIME:%d",emm_cause.cause_type,
                      emm_cause.cause.est_status,emm_cause.access_barring_rem_time);

    if((emm_cause.cause_type == LTE_NAS_IRAT_ACCESS_BARRING || 
       ((emm_cause.cause_type == LTE_NAS_IRAT_CONN_EST_FAILURE) &&  
        (emm_cause.cause.est_status == LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED)))  &&
       (ps_net_down_output_ptr != NULL))

    {

      DS_3GPP_MSG0_LOW("EMM check pass");

      if(emm_cause.access_barring_rem_time == ACCESS_BARRING_INFINITE_RETRY_TIME)
      {
        DS_LTE_MSG2_HIGH_EX(subs_id,"ds_eps_bearer_cntxt_map_cause_codes: EMM failure, "
                         "access barring infinite retry. Updating %d to %d", 
                         *ps_net_down_output_ptr, 
                          PS_NET_DOWN_REASON_EMM_ACCESS_BARRED_INFINITE_RETRY);

        *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_ACCESS_BARRED_INFINITE_RETRY;

      }
    }
    else if(ps_net_down_output_ptr == NULL)
    {
      DS_3GPP_ERROR_LOG_0_EX(subs_id, "ps_net_down_output_ptr is NULL");
          return FALSE;
    }

  }
  return TRUE;
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GO_NULL_HDLR

DESCRIPTION
  This function is used clean up a bearer 
  This function will be invoked by the 3gpp pdn redial module
  
PARAMETERS 
  bearer_context_p  - pointer to the bearer context that is to be used
                      to bring up the call  
 down_reason        - reason why the bearer went null
 call_was_connected - shows if the call was connected or not
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
 None
   
SIDE EFFECTS 
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_go_null_hdlr
(
  ds_bearer_context_s      *bearer_context_p,
  ds_3gpp_down_reason_t    down_reason,
  boolean                  call_was_connected  
)
{
  ds_pdn_context_s                         *pdn_context_p = NULL;
  sys_modem_as_id_e_type      subs_id = SYS_MODEM_AS_ID_NONE;
/*-------------------------------------------------------------------------*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  /*-------------------------------------------------------------------------
    DeRegister with PDCP.
  -------------------------------------------------------------------------*/
  ds_eps_bearer_cntxt_dereg_pdcp(bearer_context_p);

  /*-------------------------------------------------------------------------
    DeRegister with DPM
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
  {
    ds_bearer_cntx_deregister_dpm(bearer_context_p); 
  }

  bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm = FALSE;

  pdn_context_p = (ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->
                                       pdn_context_ptr);

  if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_p))
  {
    return;
  }

  if( bearer_context_p->ds_bearer_context_dyn_p->state != 
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT)
  {
    DS_LTE_MSG0_HIGH_EX(subs_id,"RAB not released, sending SYS_PROC_TYPE_NONE to Lower Layers");
    ds_eps_bearer_cntxt_send_trm_priority_ind(bearer_context_p,
                                            SYS_PROC_TYPE_NONE);
  }
  ds_eps_bearer_cntxt_reset_dl_byte_count(bearer_context_p);
  ds_3gpp_tput_hdlr_downlink(pdn_context_p->ds_pdn_context_dyn_p->subs_id, FALSE);
  /*---------------------------------------------------------------------------
    Tear Down flows and Also Add unbind flow to phys link
  ----------------------------------------------------------------------------*/
  if (ds_bearer_flow_manager_go_null_flow(bearer_context_p) == FALSE)
  {
    DS_LTE_MSG0_ERROR_EX(subs_id, "Error calling goNullFlow");
  }

 /*--------------------------------------------------------------------------- 
   Release the memory for the flow context and clear the array entries for this
   bearer do this from a flow context context
  ----------------------------------------------------------------------------*/
  if(ds_bearer_flow_manager_delete_all_flow_context(
                              pdn_context_p,bearer_context_p)==FALSE)
  {
    DS_LTE_MSG0_ERROR_EX(subs_id, "Error when deleting flow contexts");
  }

  if(bearer_context_p->ds_bearer_context_dyn_p->is_default)
  {
    /*---------------------------------------------------------------------
     Unbind the phys_link from the default flow 
    ---------------------------------------------------------------------*/
    ds_pdn_cntxt_unbind_default_flows((void*)pdn_context_p);
  }/* bearer_context_p->is_default */  

  ds_eps_bearer_cntxt_phys_link_cleanup(bearer_context_p);

  /*--------------------------------------------------------------------------
    Free the bearer instance. Note that this function only resets the fields
    of the bearer. The in_use is not changed until the PDN context frees up
    the bearer
  --------------------------------------------------------------------------*/
  ds_bearer_cntxt_free_bearer_inst(bearer_context_p);

  /*---------------------------------------------------------------------
    Each enqueue func_ptr needs to be reset when bearer inst is freed.
    This needs to be done to avoid race conditions when phys_link_gone is
    posted and flow gets enabled on the phys_link leading to each_enq_f
    called when the bearer is getting freed.
  ---------------------------------------------------------------------*/
  ps_phys_link_gone_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
  DS_LTE_MSG0_HIGH_EX(subs_id,"Phys link gone");

  if (bearer_context_p->ds_bearer_context_dyn_p->
      pdn_context_cb_tbl_ptr->bearer_down_ind_cb != NULL) 
  {
    bearer_context_p->ds_bearer_context_dyn_p->
      pdn_context_cb_tbl_ptr->bearer_down_ind_cb(
      bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
      bearer_context_p, 
      down_reason,
      call_was_connected);
  }
  ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT 
  ds3gtputmgr_update_uplink_throughput_configure(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                             FALSE);
#endif

} /*ds_eps_bearer_cntxt_go_null_hdlr() */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_UPDATE_DOWN_REASON

DESCRIPTION
  This function check error code in CM call cmd, if valid, update it 
  to the eps bearer context down reason.
  
PARAMETERS  
  bearer_context_p   - bearer context obj ptr
  call_info_p        - DSMGR input which also contains CM mode info 
    
DEPENDENCIES 
  None.

RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_update_down_reason
(
  ds_bearer_context_s            *bearer_context_p,
  const ds3g_call_info_cmd_type  *call_info_p
)
{
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }
  
  if(call_info_p == NULL)
  {
    return;
  }
  
  if(call_info_p->call_cmd_err == CM_CALL_CMD_ERR_DUAL_SWITCH)
  {
    bearer_context_p->ds_bearer_context_dyn_p->down_reason = 
	                           PS_NET_DOWN_REASON_DUAL_SWITCH;
  }
  else if(call_info_p->call_cmd_err == CM_CALL_CMD_ERR_MPLMN_IN_PROGRESS )
  {
    bearer_context_p->ds_bearer_context_dyn_p->down_reason = 
	                           PS_NET_DOWN_REASON_MPLMN_IN_PROGRESS;
  }
  else if(bearer_context_p->ds_bearer_context_dyn_p->down_reason == 
             PS_NET_DOWN_REASON_NOT_SPECIFIED)
  {
    bearer_context_p->ds_bearer_context_dyn_p->down_reason = 
                             PS_NET_DOWN_REASON_REJECTED_BY_CALL_MANAGER;    
  }
  else
  {
    /*if down_reason is filled with other value, don't overwrite it*/
  }
  
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_DEACTIVATE_CNF

DESCRIPTION
  This function deactivates the eps bearer.
  
PARAMETERS  
  call_id:     CM call id
  mode_info_p: mode specific info pointer
  end_type:    0:regular,1:abort,2:reject
    
DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
static void ds_eps_bearer_cntxt_deactivate_cnf
(
  ds_bearer_context_s            *bearer_context_p,
  const ds3g_call_info_cmd_type  *call_info_p,
  uint8                           end_type,
  boolean                         call_was_connected
)
{

  ds_pdn_context_s                         *pdn_context_p = NULL;
  ps_iface_net_down_reason_type            down_reason = 
    PS_NET_DOWN_REASON_NOT_SPECIFIED;    
  ds_3gpp_down_reason_t                    net_down_reason;
  /* Bit vector representing PCO decode info*/
  uint32                                   decode_mask = 0;  
  ps_iface_type                            *ps_v4_iface_p = NULL;
  ps_iface_type                            *ps_v6_iface_p = NULL;
  ds_3gpp_iface_s                          *ds3gpp_iface_v4_p = NULL; 
#ifdef FEATURE_DATA_PS_IPV6               
  ds_3gpp_iface_s                          *ds3gpp_iface_v6_p = NULL; 
#endif                                    
  cm_lte_call_info_s_type                  *lte_call_info = NULL;
#ifdef FEATURE_DATA_REL10                 
  byte                                     *apn = NULL;
#endif /*FEATURE_DATA_REL10*/             
  uint8                                     index = 0;
  sys_modem_as_id_e_type                    subs_id  = SYS_MODEM_AS_ID_NONE;
  ds3gsubsmgr_subs_id_e_type                subs_index = DS3GSUBSMGR_SUBS_ID_MIN;
  ds_bearer_cntxt_per_subs_info_type       *per_subs_info_hndl = NULL;
  uint32                                    rat_mask = 
                                              DS_3GPP_THROT_RAT_MASK_GLOBAL;
  boolean                                   rel12_plus = FALSE;
  ds_eps_bearer_context_s                   *eps_bearer_context_p = NULL;
  ds_3gpp_protocol_cfg_options_s            ds_proto_config_options;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Make sure the call instance can be ended
  -------------------------------------------------------------------------*/
  if (bearer_context_p->ds_bearer_context_dyn_p->state == 
      DS_BEARER_CONTEXT_STATE_DOWN ||
      bearer_context_p->ds_bearer_context_dyn_p->state == 
      DS_BEARER_CONTEXT_STATE_PEND_REDIAL) 
  {
    DS_LTE_MSG1_ERROR_EX(subs_id, 
       "Trapped, trying to end already ended call. Bearer instance:d, return",
       (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));
    return;
  }

  pdn_context_p = (ds_pdn_context_s*)(bearer_context_p->
                                      ds_bearer_context_dyn_p->pdn_context_ptr);

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_context_p,&subs_id))
  {
    return;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    return;
  }

  eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return;
  }
  
  /* ToDo - Move to eps_bearer_context_ex*/
#ifdef FEATURE_LAPP_UFS 
    #error code not present
#endif /* FEATURE_LAPP_UFS  */
  

  if (ds_3gppi_is_rel_12_or_above_supported(subs_id))
  {
    rel12_plus = TRUE;
  }

  apn = pdn_context_p->ds_pdn_context_dyn_p->pdp_profile.context.apn;

  if(*apn == '\0')
  {
    (void)ds_pdn_cntxt_get_resolved_apn_name(pdn_context_p, &apn);
  }

  /*----------------------------------------------------------------------
    Start the PDN Wait Timer as the default bearer is being deactivated
    This needs to be done only for non-attach PDNs.
  -----------------------------------------------------------------------*/
  if(ds_3gpp_pdn_limit_is_enabled(subs_id) && 
     pdn_context_p->ds_pdn_context_dyn_p->ue_initiated_tear_down && 
     bearer_context_p->ds_bearer_context_dyn_p->is_default)
  {
    if((index = ds_3gpp_pdn_limit_tbl_find_entry(apn, subs_id)) 
         != DS_3GPP_PDN_LIMIT_TBL_INVALID_ENTRY )
    {
      /*------------------------------------------------------------------------
      We need not perform throttling for emergency PDN nor do we increase
      the throttle timer/count
      ------------------------------------------------------------------------*/  
      if (!DS_3GPP_PDN_SUPPORTS_EMERGENCY_SERVICES(pdn_context_p))
      {
        ds_3gpp_pdn_limit_start_pdn_wait_tmr(index, subs_id);
      }
    }
  }
 

  lte_call_info = (cm_lte_call_info_s_type*) 
                  &(call_info_p->mode_info.info.lte_call);
  ds_eps_bearer_cntxt_update_down_reason(bearer_context_p, call_info_p);
  /*------------------------------------------------------------------------
    Check if we have attached.
    If not, reset the target rat field if it had been set to LTE
    Don't reset the target field if we have a cached_seq_num, Sys mode
    must always be LTE if we have a cached sequence number.
  ------------------------------------------------------------------------*/
  if((ds3gpp_get_is_attach_complete_flag(subs_id) == FALSE) && 
     (ds3g_is_pref_pkt_mode_ex((SYS_SYS_MODE_LTE),subs_id) == TRUE) &&
     (!ds_eps_pdn_cntx_is_pdn_conn_req_pending(subs_id)))
  {
    ds_eps_pdn_cntxt_reset_target_rat(pdn_context_p);
  }

  if(pdn_context_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
        [DS_3GPP_PDN_CONTEXT_IPV4_IFACE]!=NULL)
  {
    ds3gpp_iface_v4_p = (pdn_context_p->ds_pdn_context_dyn_p->
            ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]);
    ps_v4_iface_p  = &(ds3gpp_iface_v4_p->ps_iface);
  }
#ifdef FEATURE_DATA_PS_IPV6
  if(pdn_context_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
           [DS_3GPP_PDN_CONTEXT_IPV6_IFACE]!=NULL)
  {
    ds3gpp_iface_v6_p = (pdn_context_p->ds_pdn_context_dyn_p->
          ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]);
    ps_v6_iface_p =  &(ds3gpp_iface_v6_p->ps_iface);
  }
#endif

  /*----------------------------------------------------------------------
    Retrieve the protocol config options from CM struct mode_info_p.
    Check if NW provided PCO or EPCO and convert accordingly.
  ----------------------------------------------------------------------*/

  if( TRUE == lte_call_info->esm_cause.valid )
  {
    memset(&ds_proto_config_options,0,sizeof(ds_3gpp_protocol_cfg_options_s));
    ds_3gpp_pco_epco_hdlr_get_config_from_lte
      (lte_call_info,
       &ds_proto_config_options,
       &(pdn_context_p->ds_pdn_context_dyn_p->epco_enabled));
  /*-------------------------------------------------------------------------
    Get any other protocol specific options that are received and store it
    in the call specific structure. Additon of decode Protocol Params in call end
  -------------------------------------------------------------------------*/
    decode_mask = ds_pdn_cntxt_decodeProtocolParams(
         ds3gpp_iface_v4_p,
         ds3gpp_iface_v6_p,
         &ds_proto_config_options);
  }
  /* clear the rab re-establish timer for this bearer */
  DS_BEARER_CNTXT_UNSET_BITMASK(per_subs_info_hndl, bearer_context_p);

  ds_bearer_cntxt_reset_access_barring_retry();
  
  /*-----------------------------------------------------------------------
    Reset the rab_reestab_retry counter for this bearer.
  -----------------------------------------------------------------------*/
  DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);
  
  if(per_subs_info_hndl->uplink_data_bitmask == 0)
  {
    if(per_subs_info_hndl->timer_running == TRUE)
    {
      DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl);
      ds_3gpp_rab_reestab_reset_state(subs_id);
    }
  }

  /*------------------------------------------------------------------------
   * If this was the bearer context that was used to send a reorig request,
   * and we get a call_end for this bearer, reset the 
   * service_request_pending_flag and the service_response_expected flags
   * to FALSE   
  ------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp == TRUE)
  {
    per_subs_info_hndl->service_request_pending_flag = FALSE;
    bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp = FALSE;
  }

  /*-----------------------------------------------------------------
    We need to check for APN Switching action if this is last standing
    PDN and whether any switching action has been already decided on
    attach reject
   
    If a switching action has been already decided on attach reject,
    then we need not perform additional check for Switch
  ------------------------------------------------------------------*/
  if(FALSE == dsPdnContext_is_any_other_normal_pdn_up(pdn_context_p, subs_id) &&
     (ds_bearer_cntxt_is_default(bearer_context_p) == TRUE))
  {
    ds_3gpp_apn_switch_mgr_populate_switching_payload_per_rat(
       bearer_context_p->ds_bearer_context_dyn_p->call_mode,
       DS_DSD_APM_DETACH_NETWORK_CAUSE,
       (uint16)pdn_context_p->ds_pdn_context_dyn_p->pdp_profile_num,
       pdn_context_p->ds_pdn_context_dyn_p->state,
       (cm_call_mode_info_u_type *)&(call_info_p->mode_info.info),
       subs_id);
  }

  /*----------------------------------------------------------------------------------- 
    Reset conditions
    REL12 + enabled and global throttling not set and SRV_REQ_THROT_BLOCK_PLMN not set
  ------------------------------------------------------------------------------------*/
  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g( subs_id );
  if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_index))
  {
    return;
  }

  if((ds_3gpp_nv_manager_get_nw_override_cleanup(subs_id)) &&
     (ds_3gpp_pdn_cntxt_is_bearer_deactivated_by_network(pdn_context_p,
                                                        (void *)bearer_context_p)))
  {
     ds_3gpp_network_override_hdlr_set_overridden_ip
                      (pdn_context_p->ds_pdn_context_dyn_p->pdp_profile.context.apn,
                       DS_3GPP_PDN_THROT_PDP_UNDEFINED,
                       DS_3GPP_PDN_THROT_NTW_O_RIDE_CAUSE_MAX,
                       DS_3GPP_THROT_RAT_MASK_LTE,
                       call_info_p->mode_info.info.lte_call.reattempt_indicator,
                       FALSE,
                       TRUE,
                       subs_id);
  }

  /* Check the NW cause code to see if more details can be provided as 
     Net down reason */

  if(lte_call_info->emm_failure_cause.cause_type != LTE_NAS_IRAT_NONE)
  {
    DS_LTE_MSG1_HIGH_EX(subs_id,"Received valid EMM cause in deact_cnf,cause type : %d",
                      lte_call_info->emm_failure_cause.cause_type);

    ds_eps_bearer_cntxt_map_cause_codes
      (CALL_END_EMM_CAUSE_TO_PS_IFACE_CAUSE_CODES, &down_reason,
        (void *)&(call_info_p->mode_info),
        subs_id);
  }
  else if (lte_call_info->esm_cause.valid ||
      lte_call_info->esm_local_cause.valid)
  {
    DS_LTE_MSG2_HIGH_EX(subs_id,
                        "Received valid cause in deact_cnf : ESM cause valid: %d "
                        "ESM local cause valid: %d", 
                        lte_call_info->esm_cause.valid,
                        lte_call_info->esm_local_cause.valid);
    ds_eps_bearer_cntxt_map_cause_codes(CALL_END_ESM_CAUSE_TO_PS_IFACE_CAUSE_CODES,
                                        &down_reason,
                                        (void *)&(call_info_p->mode_info),
                                        subs_id);

    if( ds_3gppi_is_rel_11_or_above_supported(subs_id) )
    {
      /* ----------------------------------------------------------------
      New function added as part of Release 11. This functions handles
      the NAS pdp reject with t3396 ie. 
      ------------------------------------------------------------------*/
      if(lte_call_info->esm_cause.esm_cause == NAS_ESM_INSUFFICIENT_RESOURCES 
        &&  pdn_context_p->ds_pdn_context_dyn_p->state == DS_PDN_CONTEXT_STATE_UP)
      {
        if (ds_3gpp_hdlr_is_ue_in_ac11to15(subs_id) && 
            ds_3gppi_is_rel_11_or_above_supported(subs_id))
        {
          DS_LTE_MSG0_LOW("UE is AC: 11-15, not blocking APN for attach");
        }
        else
        {
          ds_pdn_throt_spec_lte_handle_nas_reject_with_t3396_ie(lte_call_info,
                                                                pdn_context_p,
                                                                down_reason);
          
          ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                           DS_BEARER_CONTEXT_STATE_GOING_DOWN);
          DS_LTE_MSG1_LOW("UE is not AC: 11-15, blocking APN for attach"
                           " bearer state: %d", 
                            bearer_context_p->ds_bearer_context_dyn_p->state);
          ds_3gpp_pdn_throt_advertise_blocked_apns_current_plmn(subs_id);
#ifdef FEATURE_DATA_LTE
          ds_3gpp_pdn_cntxt_disable_lte_if_attach_profile_unavailable(subs_id);
#endif /* FEATURE_DATA_LTE */
        }
      }
    }
  }
  else if(bearer_context_p->ds_bearer_context_dyn_p->down_reason != 
          PS_NET_DOWN_REASON_NOT_SPECIFIED)
  {
    down_reason = bearer_context_p->ds_bearer_context_dyn_p->down_reason;
    DS_LTE_MSG1_HIGH_EX(subs_id,"APN invalid param change.Down_reason:0x%x",
                     down_reason);
  }
  else
  {
    down_reason = PS_NET_DOWN_REASON_NOT_SPECIFIED;
  }

  /*----------------------------------------------------------------------
     Notify PDN that the bearer setup is complete
  ---------------------------------------------------------------------*/
  DS_LTE_MSG1_HIGH_EX(subs_id,
                      "BEARER DOWN IND CB for bearer :%x", 
                      bearer_context_p);

  net_down_reason.reason_T = DS_3GPP_PS_IFACE_NET_DOWN_REASON_T;
  net_down_reason.reason.ps_iface_down_reason = down_reason;

#ifdef FEATURE_DATA_REL10
  if(ds_3gpp_nv_manager_get_nv_lte_rel_10(subs_id))
  {
    if(down_reason == PS_NET_DOWN_REASON_UMTS_REATTACH_REQ)
    {
      /*---------------------------------------------------------------------
        EPS bearer context deactivation request with cause #39 �reactivation                                  
        got successfull processed by UE. Clear APN throttling state machine                                     
        if it was globally blocked i.e. unthrottle the APN
      ----------------------------------------------------------------------*/
      DATA_3GPP_MSG_SPRINTF_1(MSG_LEGACY_HIGH,
                              "Trying to unblock potentially blocked APN: %s",
                              (char*)apn);
      /*---------------------------------------------------------------------
        Unblock the APN on Global RAT and LTE RAT if throttling per RAT is
        enabled
      ----------------------------------------------------------------------*/
      if (ds_3gpp_nv_manager_get_enable_pdn_throt_per_rat(subs_id)||
          ds_3gppi_is_rel_12_or_above_supported(subs_id))
      {
        rat_mask |= DS_3GPP_THROT_RAT_MASK_LTE;
      }

      ds_3gpp_pdn_throt_handle_nw_inited_unblock_apn(apn,subs_id, rat_mask);
    }
  }
#endif /*FEATURE_DATA_REL10*/

  /*-------------------------------------------------------------------------
    Free the PS timer allocated for dynamic BP disable
  -------------------------------------------------------------------------*/
  if (PS_TIMER_SUCCESS != 
      ps_timer_free(bearer_context_p->
                    ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure))
  {
    DS_LTE_MSG1_ERROR_EX(subs_id, 
                        "Failed to deallocate timer for bearer inst: d",
                        (uint32)(bearer_context_p->
                         ds_bearer_context_dyn_p->client_data_ptr));
  }

  /*--------------------------------------------------------------------------
    Clear the timer handle as stale handle could cause issues during inter-RAT
  --------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
                       tmr_hndl_dyn_back_pressure = PS_TIMER_INVALID_HANDLE;

  if(bearer_context_p->ds_bearer_context_dyn_p->is_default)
  {
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_PEND_REDIAL);
    DS_LTE_MSG1_HIGH_EX(subs_id,
                      "ds_eps_bearer_cntxt_deactivateCnf: Changing bearer %d "
                      "state to DS_BEARER_CONTEXT_STATE_PEND_REDIAL",
                      bearer_context_p);
    if (bearer_context_p->ds_bearer_context_dyn_p->
        pdn_context_cb_tbl_ptr->bearer_down_ind_cb != NULL) 
    {
      bearer_context_p->ds_bearer_context_dyn_p->
        pdn_context_cb_tbl_ptr->bearer_down_ind_cb(\
          (void*)pdn_context_p, bearer_context_p, 
           net_down_reason, call_was_connected);
    }
  }
  else
  {
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_DOWN);
    DS_LTE_MSG1_HIGH_EX(subs_id,
                        "ds_eps_bearer_cntxt_deactivateCnf: Changing bearer %d "
                        "state to DS_BEARER_CONTEXT_STATE_DOWN",
                        bearer_context_p);
    ds_eps_bearer_cntxt_go_null_hdlr(bearer_context_p,  
                                     net_down_reason, call_was_connected);
  }

} /* ds_eps_bearer_cntxt_deactivateCnf */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_TEAR_DOWN_PDN_CONNECTION

DESCRIPTION
  This function is used to tear down a PDN connection
  This function will decide if it has to send out a PDN Abort request or a 
  PDN Disconnect request 

PARAMETERS
  bearer_context_p  - pointer to the bearer context that is to be used
                      to end the call
  call_info_p       - call info structure
  info_ptr          - void pointer

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, DS3G_FAILURE otherwise. 
  When this function returns failure, the bearer and the PDN contexts are
  cleaned up from the calling function. 
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_eps_bearer_cntxt_tear_down_pdn_connection
(
  ds_bearer_context_s        *bearer_context_p,
  ds_3gpp_pdn_call_info_type *call_info_p,
  void                       *info_ptr
)
{
  ds_eps_bearer_context_s* eps_bearer_context_p;
  boolean                  ret_val = FALSE;
  boolean                  is_lte_avail = TRUE;
  ds_bearer_context_s*     temp_bearer_context_p = NULL;
  ds_eps_bearer_context_s* temp_eps_bearer_context_p = NULL;
  boolean                  local_disconnect = FALSE;
  ds_pdn_context_s*        pdn_context_ptr = NULL; 
  sys_modem_as_id_e_type   subs_id  = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  pdn_context_ptr = (ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr);

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_context_ptr,&subs_id))
  {
    return FALSE;
  }
  /*-------------------------------------------------------------------------
    Based on the state of the bearer, invoke functions into DSMGR

    Logic:

    DS_BEARER_CONTEXT_STATE_COMING_UP    - Send PDN Abort
    DS_BEARER_CONTEXT_STATE_PEND_RAB_REG - Received Activate Bearer Ind 
                                           already and responded to it with
                                           SUCCESS. Send PDN disconnect
    DS_BEARER_CONTEXT_STATE_GOING_DOWN   - Do nothing (This means that the 
                                           Activate Bearer Indication was
                                           rejected
  -------------------------------------------------------------------------*/
  switch(bearer_context_p->ds_bearer_context_dyn_p->state)
  {
    case DS_BEARER_CONTEXT_STATE_COMING_UP:
      ret_val = ds3g_msh_abort_pdn_connection_ex( call_info_p, 
                          bearer_context_p->ds_bearer_context_dyn_p->sdf_id,
                                                  subs_id);
      break;

    case DS_BEARER_CONTEXT_STATE_PEND_RAB_REG:
    case DS_BEARER_CONTEXT_STATE_UP:
    case DS_BEARER_CONTEXT_STATE_UP_DORMANT:
    case DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB:
      is_lte_avail = ds3g_is_sys_available_ex(SYS_SYS_MODE_LTE,subs_id);

      /*----------------------------------------------------------------------- 
        On PLs which have FEATURE_DATA_WLAN_MAPCON defined, we always look at 
        the info provided by EPC to determine whether IRAT cause code should be 
        set while sending PDN Disconnect. 
        In case PS_IFACE_TEAR_DOWN_REASON_HANDOFF is not provided by EPC, 
        we would not include IRAT cause code as part of PDN Disconnect.

      -----------------------------------------------------------------------*/
      if (info_ptr != NULL && 
            (info_ptr == (void *)TRUE) )
      {
        DS_LTE_MSG0_HIGH_EX(subs_id,"Send local disconnect");
        local_disconnect = TRUE;
      }
      /*----------------------------------------------------------------------- 
        is_lte_avail will be TRUE, if:
        1. LTE radio is in a state other than Out of Service or
        2. LTE Attach is in progress or
        3. CM SS event indicated that LTE is target RAT or
        4. BSR is in progress

        If is_lte_avail is not TRUE, check whether EHRPD is the 
        preferred system. If yes, include IRAT cause as part of PDN Disconnect. 
        Oherwise do not include IRAT cause as part of PDN Disconnect.
      -----------------------------------------------------------------------*/
      if (!local_disconnect &&
          !is_lte_avail     && 
          ds3gdsdif_ehprd_is_pref_system(subs_id))
      {
        DS_LTE_MSG0_HIGH_EX(subs_id,"is_lte_avail is FALSE and EHRPD is preferred. Send local disconnect");
        local_disconnect = TRUE;
      }

      if(is_lte_avail == FALSE)
      {
        /*----------------------------------------------------------------------
          For the current PDN, set the flag to indicate that we are sending 
          local disconnect. This is needed to prevent this PDN from showing up
          in the list of PDNs that we will be cleaning up in this section of 
          code.
        ----------------------------------------------------------------------*/
        ds_3gpp_pdn_cntx_set_local_discon_sent_flag((ds_pdn_context_s*)
                                           (bearer_context_p->
                                       ds_bearer_context_dyn_p->pdn_context_ptr));

        /*----------------------------------------------------------------------
          Phone has not lost service and it not in LTE either.
          It has acquired another mode. At this point, we need to find out if 
          there are PDNs that are in GOING DOWN state.

          If there are and if local disconnect has not been sent on them, we
          have to send it.
        ----------------------------------------------------------------------*/

        while((temp_bearer_context_p = (ds_bearer_context_s*)
               ds_3gpp_pdn_cntx_get_bearer_for_pdn_in_going_down(subs_id)) !=
                 NULL)
        {
          /*--------------------------------------------------------------------
            We call the utility function to find out if there is any PDN for
            which a local disconnect has not been sent and the PDN is in going
            down state. If yes, the utility function returns the pointer to the
            bearer on which we have to send local disconnect.

            We repeat this until we get NULL as the bearer context.

            Set the flag in the PDN context that we have sent local disconnect.
          --------------------------------------------------------------------*/

          ds_3gpp_pdn_cntx_set_local_discon_sent_flag((ds_pdn_context_s*)
                                      (temp_bearer_context_p->
                                       ds_bearer_context_dyn_p->pdn_context_ptr));

          /*--------------------------------------------------------------------
            Get the EPS bearer context (to get the EPS bearer Id)
          --------------------------------------------------------------------*/
          temp_eps_bearer_context_p = (ds_eps_bearer_context_s*)\
                     temp_bearer_context_p->ds_bearer_context_dyn_p
            ->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];



          if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(temp_eps_bearer_context_p))
          {
            DS_LTE_MSG1_ERROR_EX(subs_id, 
                              "EPS bearer pointer is NULL. PDN ptr: 0xx Cont.",
                              temp_bearer_context_p->
                              ds_bearer_context_dyn_p->pdn_context_ptr);
            continue;
          }

          DS_LTE_MSG1_HIGH_EX(subs_id,
                           "Send local disc on Going Down PDN. EPS Br ID: %d",
                           temp_eps_bearer_context_p->ds_eps_bearer_context_dyn_p->
                           eps_bearer_id);

          /*---------------------------------------------------------------------
            Update the state so we do not send multiple disconnect req to CM. 
          ---------------------------------------------------------------------*/

          ds_bearer_cntxt_set_bearer_state(temp_bearer_context_p,
                                            DS_BEARER_CONTEXT_STATE_GOING_DOWN);

          if(ds3g_msh_pdn_disconnect_req_ex((void*)call_info_p,
             temp_bearer_context_p->ds_bearer_context_dyn_p->sdf_id,
             temp_bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                                       temp_eps_bearer_context_p->
                                         ds_eps_bearer_context_dyn_p->eps_bearer_id,
                                         local_disconnect,
                                            subs_id) != TRUE)
          {
            DS_LTE_MSG0_LOW("Could not send local disconnect");
          }

        }//Call the PDN context helper till it declares no more to clean up
        
        DS_LTE_MSG0_LOW("Done clean up of cntxt in Going Down St before IRAT");
      } //moved out of LTE and not in fade

      eps_bearer_context_p = (ds_eps_bearer_context_s*)\
                        bearer_context_p->ds_bearer_context_dyn_p->
        vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

      if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
      {
        return FALSE;
      }
      /*---------------------------------------------------------------------
        Update the state so we do not send multiple disconnect req to CM. 
      ---------------------------------------------------------------------*/
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_GOING_DOWN);
      if((ret_val = ds3g_msh_pdn_disconnect_req_ex((void*)call_info_p,
      bearer_context_p->ds_bearer_context_dyn_p->sdf_id,
      bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                                         eps_bearer_context_p->ds_eps_bearer_context_dyn_p
                                               ->eps_bearer_id,
                                            local_disconnect,
                                                subs_id)) != TRUE )
      {
        DS_LTE_MSG0_LOW("Could not send PDN disconnect req");
      }
      break;

    default:
      DS_LTE_MSG1_HIGH_EX(subs_id,"Ignoring tear down Pdn Connection. Bearer State: %d",
                       bearer_context_p->ds_bearer_context_dyn_p->state);
      ret_val = DS3G_SUCCESS;
      break;

  }
  return ret_val;
} /* dsEpsBearerCntx_tear_down_pdn_connection */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_BRING_UP_PDN_CONNECTION

DESCRIPTION
  This function is used to initiate a new PDN connectivity request.
  This function will be invoked both during Attach as well as during addnl
  PDN connection requests. 

PARAMETERS
  bearer_context_p  - pointer to the bearer context that is to be used
                      to bring up the call
  info_p            - pointer to the info passed from higher layers
  call_info_p       - structure passed from the calling function. Used to
                      identify a call until CM Call Id gets assigned
  useDefaultDialStr - Boolean indicating the dial string to use.

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, DS3G_FAILURE otherwise. 
  When this function returns failure, the bearer and the PDN contexts are
  cleaned up from the calling function. 
  
SIDE EFFECTS
  None.

===========================================================================*/
static boolean ds_eps_bearer_cntxt_bring_up_pdn_connection
(
  void                       *info_p,
  ds_3gpp_pdn_call_info_type *call_info_p,
  boolean                    useDefaultDialStr
)
{
  ds_bearer_context_s   *bearer_context_p;
  uint8                  seq_num = 0;
  ds_pdn_context_s*      pdn_cntx_p = NULL;
  uint8                  index = 0;
  sys_modem_as_id_e_type subs_id  = SYS_MODEM_AS_ID_NONE;
  ds3geventmgr_attach_prof_info_type       profile_info;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  if(call_info_p == NULL)
  {
    DS_3GPP_ERROR_FATAL("call_info_p is NULL");
    return FALSE;
  }

  if(call_info_p->call_instance >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_ERROR_FATAL("Call Instance is invalid");
    return FALSE;
  }

  /*---------------------------------------------------------------------------
    Get the bearer context pointer and set the LTE to LTE if required
  ---------------------------------------------------------------------------*/
  bearer_context_p = 
    ds_bearer_cntxt_get_instance_ptr((uint8)call_info_p->call_instance);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  if(bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE)
  {
    DS_LTE_MSG2_HIGH_EX(subs_id,"Updating bearer: call_mode from %d to %d",
                      bearer_context_p->ds_bearer_context_dyn_p
                     ->call_mode,SYS_SYS_MODE_LTE);
    bearer_context_p->ds_bearer_context_dyn_p->call_mode = SYS_SYS_MODE_LTE;
  }

  pdn_cntx_p = (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  
  /*---------------------------------------------------------------------------
    Ensure that this is not an Initial / Handover Attach procedure.
    PDN Limit handling applies only to Non-Attach scenarios.
  ---------------------------------------------------------------------------*/

  if(ds_3gpp_pdn_limit_is_enabled(subs_id) && 
     ds_pdn_cntxt_is_ue_attached(subs_id)  &&
     pdn_cntx_p->ds_pdn_context_dyn_p->handover_attach_flag == FALSE)
  {
    if((index = ds_3gpp_pdn_limit_tbl_find_entry
                  (pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.context.apn,subs_id))
                != DS_3GPP_PDN_LIMIT_TBL_INVALID_ENTRY)
    {
      /*---------------------------------------------------------------------------
        Start the PDN Connection Timer if it has not already started.
      ---------------------------------------------------------------------------*/

      if(ds_3gpp_pdn_limit_get_current_pdn_conn_tmr_val(index,subs_id) == 0)
      {
        ds_3gpp_pdn_limit_start_pdn_conn_tmr(index,subs_id);
      }
    }
  }

  /*-------------------------------------------------------------------------- 
    Populate the sequence number and low priority signaling bit
   -------------------------------------------------------------------------*/

  seq_num = ds_eps_bearer_cntxt_get_seq_num(subs_id);

  call_info_p->low_priority_signaling =
    ds_3gpp_get_low_priority_signaling_bit(pdn_cntx_p);

  /*----------------------------------------------------------------------- 
    If UE is not yet attached and this is an emergency call, set target RAT to
    LTE, so that DSD can update Serving Status to LTE Limited Service.
   -------------------------------------------------------------------------*/
  if (!ds_eps_attach_sm_is_ue_attached(subs_id) &&
      DS_3GPP_PDN_SUPPORTS_EMERGENCY_SERVICES(pdn_cntx_p))
  {
    DS_LTE_MSG0_HIGH_EX(subs_id,"Emergency Attach Initiation. Setting Target_Rat to LTE");

    /*----------------------------------------------------------------------- 
    There will be no PDN_CONN_IND to clear the PLMN cache in case of 
    emergency attach procedure, thus cleaning the cache here 
    ----------------------------------------------------------------------- */
    ds3g_clear_current_plmn_id(subs_id);

    memset(&profile_info, 0, sizeof(ds3geventmgr_attach_prof_info_type));
    strlcpy(profile_info.apn_name, 
            (const char*)pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.
            context.apn,
            DS_SYS_MAX_APN_LEN);

    profile_info.emergency_calls_supported = 
      pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.emergency_calls_are_supported;

    ds3g_set_target_rat_ex( SYS_SYS_MODE_LTE,
                            &profile_info,
                            subs_id);
  }

  /*--------------------------------------------------------------------------- 
    Make a decision if the PDN connectivity request has to be initiated as
    Low Priority Signaling
  --------------------------------------------------------------------------*/
  return ds3g_msh_initiate_pdn_connection_ex(call_info_p,seq_num,subs_id);
} /* dsEpsBearerCntx_bring_up_pdn_connection */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REDIAL_HDLR

DESCRIPTION
  This function is used to redial  PDN connectivity request.
  This function will be invoked both during Attach as well as during addnl
  PDN connection requests. 

PARAMETERS
  bearer_context_p  - pointer to the bearer context that is to be used
                      to bring up the call  
  call_info_p       - structure passed from the calling function. Used to
                      identify a call until CM Call Id gets assigned  

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, DS3G_FAILURE otherwise. 
  When this function returns failure, the bearer and the PDN contexts are
  cleaned up from the calling function. 
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_redial_hdlr
(  
  ds_bearer_context_s                   *bearer_context_p
)
{
  ds_3gpp_pdn_call_info_type       call_info;
  uint8                            seq_num = 0;
  ds_pdn_context_s                 *pdn_cntx_ptr = NULL;
  sys_modem_as_id_e_type           subs_id  = SYS_MODEM_AS_ID_NONE;
  boolean                          ret_val = DS3G_FAILURE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }
  pdn_cntx_ptr = (ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr);
  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_ptr,&subs_id))
  {
    return FALSE;
  }

  if(bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE)
  {
    DS_LTE_MSG3_HIGH_EX(subs_id,"Updating bearer: call_mode from %d to %d",
                      (uint32)(bearer_context_p->
                               ds_bearer_context_dyn_p->client_data_ptr),
                      bearer_context_p->ds_bearer_context_dyn_p->
                     call_mode,SYS_SYS_MODE_LTE);
    bearer_context_p->ds_bearer_context_dyn_p->call_mode = SYS_SYS_MODE_LTE;
  }
  /*-------------------------------------------------------------------------
    Save the call instance to pass to ds3g_msh_initiate_call(). This will
    be used in the dsumtsps_orig_call_hdlr() to determine which call
    was being originated.
  -------------------------------------------------------------------------*/
  call_info.call_instance    = (uint32)(bearer_context_p->
                                ds_bearer_context_dyn_p->client_data_ptr);
  call_info.info_ptr         = NULL; //Not Used
  call_info.mt_flag          = FALSE;

  /*------------------------------------------------------------------------
    Populate the sequence number
  -------------------------------------------------------------------------*/
  seq_num = ds_eps_bearer_cntxt_get_seq_num(subs_id);

  call_info.low_priority_signaling =
    ds_3gpp_get_low_priority_signaling_bit(pdn_cntx_ptr);

  ret_val = ds3g_msh_initiate_pdn_connection_ex(&call_info,seq_num,subs_id);

  /* moving the bearer state to COMING_UP after sending PDN Request to CM  */
  ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                   DS_BEARER_CONTEXT_STATE_COMING_UP);
  return ret_val;
} /* dsEpsBearerCntx_redial_hdlr */


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_ORIG_PARAMS

DESCRIPTION
  This function is used to fill up the orinigation parameters.
  For now, this does nothing. 

PARAMETERS
  bearer_context_p  - pointer to the bearer context that is to be used
                      to bring up the call
  mm_orig_params_p  - pointer to the orig params structure
  ds3gi_call_info   - pointer to the call info structure

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, 
  DS3G_FAILURE otherwise. 
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_get_orig_params
(
  ds_bearer_context_s              *bearer_context_p,
  ds_3gpp_orig_params_s            *mm_orig_params_p,                          
  const ds_3gpp_pdn_call_info_type *ds3gi_call_info
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*--------------------------------------------------------------------------
    Cache the SDF Id into the bearer context
  --------------------------------------------------------------------------*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  bearer_context_p->ds_bearer_context_dyn_p->sdf_id = 
    mm_orig_params_p->orig_params_u.lte_ps_orig_params_ptr->sdf_id;

  return DS3G_SUCCESS;
} /* ds_eps_bearer_cntxt_get_orig_params */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_DATA_TRFR_ACTIVITY

DESCRIPTION
  This function is used to get the

PARAMETERS
  bearer_tech_p     - pointer to the bearer tech structure
  ps_errno          - pointer to the error code field

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, 
  DS3G_FAILURE otherwise. 
  
SIDE EFFECTS
  DS3G_FAILURE could be returned due to either Data inactivity or other error
  Thus when DS3G_FAILURE is returned, the inactivity timer is not reset.

===========================================================================*/
static boolean ds_eps_bearer_cntxt_get_data_trfr_activity
(
  ds_bearer_context_s  *bearer_context_p
)
{
  boolean                     ret_val = FALSE;
  uint64                      tx_bytes_current = 0;
  uint64                      tx_bytes_temp = 0;
  uint64                      rx_bytes_current = 0;
  ds_eps_bearer_context_s     *eps_bearer_context_p = NULL;
  ds_pdn_context_s            *pdn_cntxt_p = NULL;
  ps_iface_type               *v4_iface_p = NULL;
  ps_iface_type               *v6_iface_p = NULL;  
  lte_pdcpdl_activity_stats_s pdcp_dl_stats;
  sys_modem_as_id_e_type      subs_id =  SYS_MODEM_AS_ID_NONE;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  pdn_cntxt_p = (ds_pdn_context_s *)
                  (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr);

  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntxt_p))
  {
    return FALSE;
  }

  v4_iface_p = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntxt_p);
  v6_iface_p = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntxt_p);
  
  /*--------------------------------------------------------------------------
    Get the EPS bearer context
  --------------------------------------------------------------------------*/
  eps_bearer_context_p = (ds_eps_bearer_context_s*)
                        bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

  if(ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p ))
  {
    /* Get V4 iface tx stats */
    if (PS_IFACE_IS_VALID(v4_iface_p))
    {
      ps_stat_get_iface(PS_STAT_IFACE_BYTES_TX,
                        (void *)v4_iface_p,
                        (void *)&tx_bytes_temp,
                        sizeof(tx_bytes_temp)
                        );
    }

    tx_bytes_current += tx_bytes_temp;

    DS_LTE_MSG2_HIGH_EX(subs_id,"Queried V4 iface stats tx_bytes_v4: 0x%x%x", 
                        QWORD_HIGH(tx_bytes_temp),
                        QWORD_LOW(tx_bytes_temp));

    tx_bytes_temp = 0;

    /* Get V6 iface stats */
    if (PS_IFACE_IS_VALID(v6_iface_p))
    {
      ps_stat_get_iface(PS_STAT_IFACE_BYTES_TX,
                        (void *)v6_iface_p,
                        (void *)&tx_bytes_temp,
                        sizeof(tx_bytes_temp)
                        );
    }

    tx_bytes_current += tx_bytes_temp;

    DS_LTE_MSG2_HIGH_EX(subs_id,"Queried V6 iface stats tx_bytes_v6: 0x%x%x", 
                        QWORD_HIGH(tx_bytes_temp),
                        QWORD_LOW(tx_bytes_temp));

    /*------------------------------------------------------------------------
      For DL stats, query PDCP only if the bearer is in UP state
    ------------------------------------------------------------------------*/
    if(bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP)
    {
      if(lte_pdcpdl_get_activity_statistics(eps_bearer_context_p->ds_eps_bearer_context_dyn_p
                                            ->eps_bearer_id,
                                            &pdcp_dl_stats,
                                            (uint8)subs_id 
                                            )== DS3G_SUCCESS)
      {
        DS_LTE_MSG1_HIGH_EX(subs_id,"DL bytes: %d",pdcp_dl_stats.num_bytes);
        rx_bytes_current = (uint64)pdcp_dl_stats.num_bytes;

        /*--------------------------------------------------------------------
          If the current stats are the same as the one that has been cached in
          the bearer context, there has been no data transfer.
          Set the ret_val to FALSE indicating data inactivity
        ----------------------------------------------------------------------*/
        if( ((tx_bytes_current != 0) && (tx_bytes_current != bearer_context_p->ds_bearer_context_dyn_p->tx_bytes)) ||
            ((rx_bytes_current != 0) && (rx_bytes_current != bearer_context_p->ds_bearer_context_dyn_p->rx_bytes)) )
        {
          ret_val = TRUE;
        }
      } //PDCP api returned sucess
      else
      {
        DS_LTE_MSG0_LOW("API to get PDCP DL stats returned FAILURE");
        return DS3G_FAILURE;
      }
    }//bearer context is UP
    else
    {
      DS_LTE_MSG0_HIGH_EX(subs_id,"Bearer is dormant. Checking activity on UL only");
      /*----------------------------------------------------------------------
        Set the Rx bytes count to zero. This is needed as PDCP would have lost
        its counters. Hence we are synchronizing with PDCP so that when the
        bearer comes up again, DS and PDCP start from 0
      ----------------------------------------------------------------------*/
      rx_bytes_current = 0;

      /*--------------------------------------------------------------------
        Check data activity on uplink only
      ----------------------------------------------------------------------*/
      if((tx_bytes_current != 0) && 
         (tx_bytes_current != bearer_context_p->ds_bearer_context_dyn_p->tx_bytes))
      {
        /*----------------------------------------------------------------------
          There has been data activity on UL. Although it cannot be established
          if the data activity was before or after the bearer went dormant, we
          take this as data activity
        ----------------------------------------------------------------------*/
        ret_val = TRUE;
      }
    }

    /*----------------------------------------------------------------------
      Update the stats in the bearer context with the current stats.
      This can be done irrespective of whether data transfer happened or not
    ----------------------------------------------------------------------*/
    bearer_context_p->ds_bearer_context_dyn_p->tx_bytes = tx_bytes_current;
    bearer_context_p->ds_bearer_context_dyn_p->rx_bytes = rx_bytes_current;
  }
  else
  {
    DS_LTE_MSG0_LOW("Could not get EPS bearer context from Bearer Context");
  }

  DS_LTE_MSG1_HIGH_EX(subs_id,"Detected Data Activity %d", ret_val);
  return ret_val;
} /* ds_eps_bearer_cntxt_get_data_trfr_activity */

/*==========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_BEARER_RATE

DESCRIPTION This function returns the tx and rx rates of current call's
            channel.
    
PARAMETERS
   call_inst        - index into the call table
  *argval_ptr       - Bearer rate info will be put in here
  *ps_errno         - Ptr to error code (if any)
   subs_id          - Subscription id
  
  
DEPENDENCIES
  None.
                  
RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS 
  None
===========================================================================*/
int ds_eps_bearer_cntxt_get_bearer_rate
(
  uint32                   call_inst,
  void                    *argval_ptr,
  sint15                  *ps_errno,
  sys_modem_as_id_e_type   subs_id
)
{
  uint8 ue_cat; 
  ds3gsubsmgr_subs_id_e_type  subs_indx = DS3GSUBSMGR_SUBS_ID_INVALID;

  ps_iface_ioctl_data_bearer_rate *rate_info = 
           (ps_iface_ioctl_data_bearer_rate *)argval_ptr;
 
  subs_indx = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);
  if(ds3gsubsmgr_is_ds3g_subs_index_valid(subs_indx) == FALSE)
  {
    if(ps_errno != NULL)
    {
      *ps_errno = DS_EFAULT;
    }
    DS_LTE_MSG1_ERROR_EX(subs_id, "invalid subs index:d return error", subs_indx);
    return -1;
  }

  ue_cat = ds_eps_bearer_cntxt_ue_cat_id[subs_indx];

  if(ue_cat >= LTE_UE_CLASSES_MAX)
  {
    DS_LTE_MSG1_ERROR_EX(subs_id, "LTE category d not valid", ue_cat);
    *ps_errno = DS_EINVAL;
    return -1;
  }

  *rate_info = ds_eps_bearer_rate[ue_cat];
  return 0;
}

/*========================================================================== 
FUNCTION DS_EPS_BEARER_CNTXT_EXAMINE_IF_EMM_CAUSE_THROT_NEEDED

DESCRIPTION 
  This function checks whether EMM cause throttling is needed or not.
    
PARAMETERS
  LTE Call Info Ptr
  rab_throt_info
  
DEPENDENCIES
  None.
                  
RETURN VALUE
  TRUE, if throttling needed
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
void ds_eps_bearer_cntxt_examine_if_emm_cause_throt_needed
(
  cm_lte_call_info_s_type             *lte_call_info_ptr,
  ds_3gpp_throt_rab_info_s            *rab_throt_info,
  sys_modem_as_id_e_type               subs_id
)
{
  boolean throt_is_needed = FALSE;

  do
  {
    if(rab_throt_info == NULL)
    {
      DS_LTE_MSG0_LOW("Input Params NULL. ");
      break;
    }

    rab_throt_info->lower_layer_failure = FALSE;
    rab_throt_info->throttle_needed = FALSE;

    if(lte_call_info_ptr == NULL)
    {
      DS_LTE_MSG0_LOW("Input Params NULL. ");
      break;
    }

    DS_LTE_MSG2_HIGH_EX(subs_id,
                    "EMM Failure Cause Type: %d, "
                    "EMM Failure Detailed Reason: %d", 
                    lte_call_info_ptr->emm_failure_cause.cause_type,
                    lte_call_info_ptr->emm_failure_cause.detailed_cause);

    /*-------------------------------------------------------------------
       Check the EMM Cause type and EMM Detailed Cause if necessary
       DS to interpret all the below EMM failure cause types
       as Lower layer failures
       Cause types are 
       LTE_NAS_IRAT_CONN_EST_FAILURE --> and cause code is not
                                    LTE_RRC_CONN_EST_FAILURE_CONN_REJECT
       LTE_NAS_IRAT_UL_MSG_CNF_FAILURE
       LTE_NAS_IRAT_LOWER_LAYER_FAILURE 
    ---------------------------------------------------------------------*/
    switch (lte_call_info_ptr->emm_failure_cause.cause_type)
    {
      case LTE_NAS_IRAT_CONN_EST_FAILURE:
      {
        rab_throt_info->lower_layer_failure = TRUE;
        switch (lte_call_info_ptr->emm_failure_cause.detailed_cause)
        {
          case LTE_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE:
          {
            throt_is_needed = FALSE;
            break;
          }

          case LTE_RRC_CONN_EST_FAILURE_CONN_REJECT:
          {
            rab_throt_info->lower_layer_failure = FALSE;
            if((!ds_3gpp_nv_manager_get_reg_pdn_throttling(subs_id)) || 
               (ds_3gpp_srv_req_throt_t3402_is_enabled(subs_id)))
            {
              throt_is_needed = FALSE;
            }
            else
            {
              throt_is_needed = TRUE;
            }
            break;
          } 

          case LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED:
          {
            rab_throt_info->lower_layer_failure = FALSE;
            throt_is_needed = FALSE;
            break;
          }

          case LTE_RRC_CONN_EST_FAILURE_TIMER_EXPIRED:
          {
           if (ds_3gpp_srv_req_throt_t3402_is_enabled(subs_id)) 
           {
             throt_is_needed = FALSE; 
           }
           else  
           {
             throt_is_needed = TRUE;
           }
           break;
          }
           
          case LTE_RRC_CONN_EST_FAILURE_TAI_CHANGE:
          {
            throt_is_needed = FALSE;
            break;
          }
		  
		  case LTE_RRC_CONN_EST_FAILURE_NOT_CAMPED:
          {
            throt_is_needed = FALSE;
            break;
          }
		  
          default:
          {
            throt_is_needed = TRUE;
            break;
          }
        } /* switch case for detailed causes corresponding to LTE_NAS_IRAT_CONN_EST_FAILURE*/
        break;
      } /* Cause Type: LTE_NAS_IRAT_CONN_EST_FAILURE */

      case LTE_NAS_IRAT_CONN_REL:
        throt_is_needed = TRUE;
        break;

      case LTE_NAS_IRAT_LOWER_LAYER_FAILURE:
        rab_throt_info->lower_layer_failure = TRUE;
        throt_is_needed = TRUE;
        break;

     /*----------------------------------------------------------------------- 
       VzW requirement:
       For REL12+, no throttling is needed when T3417 timer expiry is received
       NAS would perform the desired 1min throttling
      -----------------------------------------------------------------------*/
    case LTE_NAS_IRAT_T3417_EXPIRY:
        rab_throt_info->lower_layer_failure = FALSE;
        if(ds_3gppi_is_rel_12_or_above_supported(subs_id) && 
           ds_3gpp_data_retry_is_enabled(subs_id))
        {

          throt_is_needed = FALSE;
        }
        else
        {
          throt_is_needed = TRUE;
        }
        break;
		
      case LTE_NAS_IRAT_UL_MSG_CNF_FAILURE:
        rab_throt_info->lower_layer_failure = TRUE;
        throt_is_needed = TRUE;
        break;

      case LTE_NAS_IRAT_SR_REJECT:
      {
        switch(lte_call_info_ptr->emm_failure_cause.detailed_cause)
        {
          case LTE_NAS_ILLEGAL_UE:
          case LTE_NAS_ILLEGAL_ME:
          case LTE_NAS_EPS_SERVICES_NOT_ALLOWED:
          case LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW:
          case LTE_NAS_IMPLICITLY_DETACHED:
          case LTE_NAS_PLMN_NOT_ALLOWED:
          case LTE_NAS_TRACKING_AREA_NOT_ALLOWED:
          case LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA:
          case LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA:
          case LTE_NAS_CS_DOMAIN_NOT_AVAILABLE:
          case LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG:
          case LTE_NAS_CS_DOMAIN_TEMP_NOT_AVAILABLE:
            throt_is_needed = FALSE;
            break;

          case LTE_NAS_CONGESTION:
          case LTE_NAS_NO_EPS_BEARER_CONTEXT_ACTIVATED:
          case LTE_NAS_SEVERE_NW_FAILURE:
            if(ds_3gpp_nv_manager_get_global_throttling(subs_id) == TRUE)
            {
               throt_is_needed = FALSE;
               break;
            }  
            else
            {
               throt_is_needed = TRUE;
               break;
            }

          default:
            throt_is_needed = TRUE;
            break;
        } /* switch case for detailed causes corresponding to LTE_NAS_IRAT_SR_REJECT  */
       
        break;
      } /* Cause Type: LTE_NAS_IRAT_SR_REJECT */
      default:
      {
        throt_is_needed = FALSE;
        break;
      }
    } /* switch (lte_call_info_ptr->emm_failure_cause.cause_type) */

    rab_throt_info->throttle_needed = throt_is_needed;
    
  } while(0); 

  DS_LTE_MSG1_HIGH_EX(subs_id,"EMM Cause Throt needed is %d.", throt_is_needed);

  return;
}/*ds_eps_bearer_cntxt_examine_if_emm_cause_throt_needed*/

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_EXAMINE_IF_THROT_NEEDED

DESCRIPTION
  This function is used to examine the cause code of rejection and determine
  if we need to perform throttling.

PARAMETERS
  call_info_ptr  - Pointer to the call info to find out rejection cause
  rab throt info

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None.

===========================================================================*/
static void ds_eps_bearer_cntxt_examine_if_throt_needed
(
  dsgwl_ps_call_info_u_type           *call_info_ptr,
  ds_3gpp_throt_rab_info_s            *rab_throt_info_ptr,
  sys_modem_as_id_e_type               subs_id
)
{
  cm_lte_call_info_s_type  *lte_call_info_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {

    if((call_info_ptr == NULL) || (rab_throt_info_ptr == NULL))
    {
      break;
    }

    lte_call_info_ptr = &(call_info_ptr->lte_call);

    if(lte_call_info_ptr == NULL)
    {
      break;
    }

    rab_throt_info_ptr->lower_layer_failure = FALSE;
    /*
      1. We first check if EMM Failure Cause Type is other than None. If yes,    
         we check EMM Cause codes to determine whether Throttling is allowed.
     
      2. Next we check the ESM Local is valid flag.If the flag is set, we check                                                                                                                                               .
         the ESM Local cause codes to determine whether Throttling is allowed.
    */

    if(lte_call_info_ptr->emm_failure_cause.cause_type != LTE_NAS_IRAT_NONE)
    { 
        ds_eps_bearer_cntxt_examine_if_emm_cause_throt_needed
           (lte_call_info_ptr,rab_throt_info_ptr,subs_id);
    }
    else if(lte_call_info_ptr->esm_local_cause.valid == TRUE)
    {
      /*------------------------------------------------------------------------
        Now determine if any throttling has to be done. Examine emm cause codes
        that comes as esm_local_cause
      ------------------------------------------------------------------------*/
      switch(lte_call_info_ptr->esm_local_cause.local_cause)
      {
        case LTE_NAS_ILLEGAL_UE_LOCAL: // lint-fallthrough
        case LTE_NAS_ILLEGAL_ME_LOCAL:
        case LTE_NAS_EPS_SERVICES_NOT_ALLOWED_LOCAL:
        case LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW_LOCAL:
        case LTE_NAS_IMPLICITLY_DETACHED_LOCAL:
        case LTE_NAS_PLMN_NOT_ALLOWED_LOCAL:
        case LTE_NAS_TRACKING_AREA_NOT_ALLOWED_LOCAL:
        case LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA_LOCAL:
        case LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA_LOCAL:
        case LTE_NAS_CS_DOMAIN_NOT_AVAILABLE_LOCAL:
        case LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG_LOCAL:
        case LTE_NAS_CS_DOMAIN_TEMP_NOT_AVAILABLE_LOCAL:
        case LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE:
        case ESM_LOWER_LAYER_FAILURE:
        case LTE_NAS_SERVICE_REQ_FAILED_DS_DISALLOW:
        case LTE_NAS_LRRC_CONN_EST_FAILURE_RF_UNAVAILABLE:
          DS_LTE_MSG1_HIGH_EX(subs_id,"Rej cause: %d. No back off needed for listed cause",
                            lte_call_info_ptr->esm_local_cause.local_cause);
          rab_throt_info_ptr->throttle_needed = FALSE;
          break;
        default:
          DS_LTE_MSG1_HIGH_EX(subs_id,"Rej cause: %d. Back off needed for unlisted cause",
                            lte_call_info_ptr->esm_local_cause.local_cause);
          /*--------------------------------------------------------------------
            Set ret val to true so that we know we have to 
          --------------------------------------------------------------------*/
          rab_throt_info_ptr->throttle_needed = TRUE;
          break;
      }
      } /* switch on EMM cause code returned */
  }while(0); 

  return;
} /* ds_eps_bearer_cntxt_examine_if_throt_needed */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_BEARER_TECH_TYPE

DESCRIPTION
  This function is used to fill up the bearer tech structure with the LTE
  specific attributes

PARAMETERS
  bearer_tech_p     - pointer to the bearer tech structure
  ps_errno          - pointer to the error code field
  subs_id           - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, 
  DS3G_FAILURE otherwise. 
  
SIDE EFFECTS
  None.

===========================================================================*/
static int ds_eps_bearer_cntxt_get_bearer_tech_type
(
  ps_iface_bearer_technology_type  *bearer_tech_p,
  sint15                           *ps_errno,
  sys_modem_as_id_e_type            subs_id
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*--------------------------------------------------------------------------
    Configure the RAT mask to reflect LTE as the air-interface.
  --------------------------------------------------------------------------*/
  bearer_tech_p->data.umts_type.rat_mask = PS_IFACE_3GPP_LTE;

  /*--------------------------------------------------------------------------
    This is needed so that LINT does not complain about an unused argument.
    This argument is not needed in this function but will be needed in the
    bearer_tech handlers for other modes.
  --------------------------------------------------------------------------*/
  if(ps_errno != NULL)
  {
    *ps_errno = DS_ENOERR;
  }

  return 0;
} /* ds_eps_bearer_cntxt_get_bearer_tech_type */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REGISTER_DL_DATAPATH

DESCRIPTION
  This function is used to establish the complete Downlink Data path at single shot
  Setting up of watermarks, registering with IPA and registering with DL PDCP

PARAMETERS
 bearer_context_p: Bearer for which data path is to be setup

DEPENDENCIES
  None.

RETURN VALUE
None
  
SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_register_dl_datapath
(
  ds_bearer_context_s *bearer_context_p
)
{
  boolean                            retval = FALSE;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;
  /*-------------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  
  /*-------------------------------------------------------------------------
     Configure the DL WM
   -------------------------------------------------------------------------*/
  ds_eps_bearer_cntxt_setup_pdp_dl_wm(bearer_context_p);

  /*----------------------------------------------------------------------
    Register DL with PDCP
    --------------------------------------------------------------------*/
  retval = ds_eps_bearer_cntxt_register_dl_with_pdcp(bearer_context_p);
  if(retval == FALSE)
  {
    DS_LTE_MSG0_LOW("Registering with lower layer failed");

    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT);

    /*-------------------------------------------------------------------------  
        Register each enqueue function ptr.      
       ------------------------------------------------------------------------*/
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =\
                             (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data =\
                              (void*) bearer_context_p;
  }
}/* ds_eps_bearer_cntxt_register_dl_datapath*/

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_RE_REGISTER_WM_WITH_PDCP

DESCRIPTION
  This function is used to reestablish the Data path when Flow direction has got modified due to Bearer Modify indication
  from NW

PARAMETERS
  flow_dir: previous firection
  modified_flow_dir :new direction after modify
  call_id: Callid for which modify occured

DEPENDENCIES
  None.

RETURN VALUE
  TRUE is all is successful, 
  FALSE otherwise. 
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_re_register_wm_with_pdcp
(
  ds_eps_bearer_context_flow_dir_e         bearer_flow_dir,
  ds_eps_bearer_context_flow_dir_e         modified_bearer_flow_dir,
  cm_call_id_type                          call_id,
  sys_modem_as_id_e_type                   subs_id  
)
{
   ds_bearer_context_s *bearer_context_p = NULL;
   ds_eps_bearer_context_s *eps_bearer_context_p = NULL;
   boolean                  retval = FALSE;
   boolean                  ul_reg = FALSE;
   boolean                  ul_dereg = FALSE;
   boolean                  dl_reg = FALSE;
   boolean                  dl_dereg = FALSE;
  /*-----------------------------------------------------------------------*/ 
  bearer_context_p = 
                ds_bearer_cntxt_get_bearer_from_call_id(call_id);
  if(bearer_context_p == NULL)
  {
    return FALSE;;
  }

  eps_bearer_context_p = (ds_eps_bearer_context_s*)(bearer_context_p->
                ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);
  
  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return FALSE;
  }
  
  switch(bearer_flow_dir)
  {
    case DS_EPS_FLOW_DIR_DOWNLINK:
      if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_UPLINK)
      {
        dl_dereg = TRUE;
        ul_reg = TRUE;
      }
      else if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_BIDIRECTIONAL)
      {
        ul_reg = TRUE;
      }
      break;
    case DS_EPS_FLOW_DIR_UPLINK:
      if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_DOWNLINK)
      {
        ul_dereg = TRUE;
        dl_reg = TRUE;
      }
      else if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_BIDIRECTIONAL)
      {
        dl_reg = TRUE;
      }
      break;
    case DS_EPS_FLOW_DIR_BIDIRECTIONAL:
      if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_DOWNLINK)
      {
        ul_dereg = TRUE;
      }
      else if(modified_bearer_flow_dir == DS_EPS_FLOW_DIR_UPLINK)
      {
        dl_dereg = TRUE;
      }
      break;
    default:
      break;
  }/*switch*/

  if(dl_dereg)
  {    
    DS_LTE_MSG1_HIGH_EX(subs_id,"Dereg with DL_PDCP eps_id %d",
      eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id);
   /*-------------------------------------------------------------------------
    DeRegister with Pdcp.
    -------------------------------------------------------------------------*/
    retval = ds_eps_bearer_cntxt_deregister_dl_with_pdcp(bearer_context_p);
    if(retval == FALSE)
    {
      DS_LTE_MSG1_ERROR_EX(subs_id, "Dereg with dl Failure eps_id d",
        eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id);
    }
     dsm_empty_queue ( &(bearer_context_p->lte_rx_wm.wm_item) );
  }
  else if(ul_dereg)
  {
    retval = ds_eps_bearer_cntxt_deregister_ul_with_pdcp(bearer_context_p);
    if(retval == FALSE)
    {
       DS_LTE_MSG1_ERROR_EX(subs_id, "Dereg with ul Failure eps_id d",
        eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id);
    }

    dsm_empty_queue ( &(bearer_context_p->tx_wm.wm_item) );
    ds_flow_manager_update_uplink_flow_capability(bearer_context_p, FALSE);
  }
  
  if(ul_reg)
  {
     /* Updating the Bearer state to RAB Registering */
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_PEND_RAB_REG);

    DS_LTE_MSG1_HIGH_EX(subs_id,"Registering UL with PDCP, eps_id %d",eps_bearer_context_p
                                 ->ds_eps_bearer_context_dyn_p->eps_bearer_id);

    /*---------------------------------------------------------------------  
      Since previous direction was DL only, UL WM may have some stale data
      present, we need to flush it before establishing the path with PDCP
    ---------------------------------------------------------------------*/
    dsm_empty_queue(&(bearer_context_p->tx_wm.wm_item));
  
    ds_eps_bearer_cntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p);
    retval = ds_eps_bearer_cntxt_register_ul_with_pdcp(bearer_context_p);
    if(retval == FALSE)
    {    
      DS_LTE_MSG0_ERROR_EX(subs_id, "UL Registering with lower layer failed");
      
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_UP_DORMANT);
      /*---------------------------------------------------------------------  
          Register each enqueue function ptr.      
      ---------------------------------------------------------------------*/
      (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =\
                              (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
      (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data =\
                                                     (void*) bearer_context_p;      
    }   
    /* Allow UL Flow as flow direction is UL  */   
    ds_flow_manager_update_uplink_flow_capability(bearer_context_p, TRUE);
  }
  else if(dl_reg)
  {
     /* registering DL path */ 
     /* Updating the Bearer state to RAB Regestering */
     ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                      DS_BEARER_CONTEXT_STATE_PEND_RAB_REG);

     ds_eps_bearer_cntxt_register_dl_datapath(bearer_context_p);
  }
  return TRUE;
}

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_BEARER_TECH_TYPE_EX

DESCRIPTION
  This function is used to fill up the bearer tech structure with the LTE
  specific attributes

PARAMETERS
  bearer_tech_p     - pointer to the bearer tech structure
  subs_id           - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS is all is successful, 
  DS3G_FAILURE otherwise. 
  
SIDE EFFECTS
  None.

===========================================================================*/
static int ds_eps_bearer_cntxt_get_bearer_tech_type_ex
(
  ds_sys_system_status_info_type  *bearer_tech_ex_p,
  sys_modem_as_id_e_type           subs_id
)
{
  ds3gsubsmgr_subs_id_e_type    ds3g_subs_id = DS3GSUBSMGR_SUBS_ID_INVALID;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_LTE_MSG0_LOW("Mode passed in is LTE");

  if(bearer_tech_ex_p == NULL)
  {
    DS3GPP_ASSERT(0);
    return -1;
  }

  ds3g_subs_id = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  if (!ds3gsubsmgr_is_ds3g_subs_index_valid(ds3g_subs_id))
  {
    return -1;
  }

  bearer_tech_ex_p->technology = DS_SYS_NETWORK_3GPP;
  bearer_tech_ex_p->rat_value = DS_SYS_RAT_EX_3GPP_LTE;

#ifdef FEATURE_LAPP
  if (ds_3gpp_link_switch_is_nr5g_pref_sys(subs_id))
  {
    bearer_tech_ex_p->rat_value = DS_SYS_RAT_EX_3GPP_5G;
  }
#endif

  bearer_tech_ex_p->so_mask = ds_3gpp_bearer_cntxt_get_so_mask(subs_id);

  return 0;
} /* ds_eps_bearer_cntxt_get_bearer_tech_type_ex */
#endif /* FEATURE_DATA_WLAN_MAPCON */
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_EMPTY_WATERMARK()

DESCRIPTION
  This is the function flushes the uplink WM.
      
PARAMETERS
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_empty_watermark
(
  ds_bearer_context_s  *bearer_context_p
)
{
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }
  dsm_empty_queue ( &(bearer_context_p->tx_wm.wm_item) );
} /* ds_eps_bearer_cntxt_empty_watermark */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_SUSPEND_IND_HDLR

DESCRIPTION

  This function registers each_enq_func_cback when suspend is received in LTE.
  
  
PARAMETERS
   bearer_context_p 
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None 

===========================================================================*/
void ds_eps_bearer_cntxt_suspend_ind_hdlr
(
  ds_bearer_context_s  *bearer_context_p
)
{
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return ;
  }
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  DS_LTE_MSG1_HIGH_EX(subs_id,
                   "Setting each_enqueue_func_ptr in eps_suspend_hdlr "
                   "bearer_inst: %d",
                   bearer_context_p->ds_bearer_context_dyn_p->index);
  (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =
               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
  (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data =
     (void*) bearer_context_p;
} /* ds_eps_bearer_cntxt_suspend_ind_hdlr */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_RESUME_IND_HDLR()

DESCRIPTION
  This function is used to handle resume indication from CM.
  This function is also called on sys_change to LTE in connected mode scenarios
  
PARAMETERS
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_resume_ind_hdlr
(
  ds_bearer_context_s     * bearer_context_p
)
{
  ds_flow_context_s        *flow_context_p  = NULL;
  ds_pdn_context_s         *pdn_cntxt_p = NULL;
  ds_bearer_context_s      *def_bearer_context_ptr = NULL;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE; 
  uint32                    filter_preced[DS_FI_PRECED_ARR_SIZE]; 
  /*---------------------------------------------------------------------------*/

  memset(filter_preced,0,(sizeof(uint32)*DS_FI_PRECED_ARR_SIZE));
  DS_3GPP_MSG0_LOW("In ds_eps_bearer_cntxt_resume_ind_hdlr");
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }
 

  pdn_cntxt_p = (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntxt_p))
  {
    return;
  }

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntxt_p,&subs_id))
  {
    return;
  }

  if (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP)
  {
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr = NULL;
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data = NULL;
  }

  /*-----------------------------------------------------------------------------
    Do not teardown the dedicated bearer in normal suspend/resume 
    case(eg: L->OOS->L) NSAPI will be non-zero in IRAT case(W->L). It will be 
    zero only when UE goes to L->OOS->L
     
    Teardown decitaed bearer in W->L scenario for the following conditions: 
      (a) if it L to W QoS continuity NV is disabled 
      or 
     (b) if it doesn't have a valid default PDN (primary PDP) associated with it
  -----------------------------------------------------------------------------*/

  def_bearer_context_ptr = pdn_cntxt_p->ds_pdn_context_dyn_p->def_bearer_context_ptr;

  if( (bearer_context_p->ds_bearer_context_dyn_p->nsapi != 0) &&
      (bearer_context_p->ds_bearer_context_dyn_p->is_default == FALSE))
  {
      if((def_bearer_context_ptr == NULL)||
       (ds_bearer_cntx_validate_bearer_context(def_bearer_context_ptr)&&
           (def_bearer_context_ptr->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_DOWN)))
  {  
    flow_context_p = ds_bearer_flow_manager_find_flow_context(
                                   bearer_context_p,
                                   DS_3GPP_V4_FLOW_TYPE | DS_3GPP_V6_FLOW_TYPE,
                                   FALSE);
    if(ds_flow_context_validate_flow(flow_context_p))
    {
      DS_3GPP_ERROR_LOG_1_EX(subs_id, "resume_ind_hdlr: secondary without a primary bearer, ending call id:d",
                          bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);
      ds_flow_context_ps_flow_go_null_cmd(flow_context_p);

     ds_bearer_cntxt_phys_link_down_cmd_hdlr(
          (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr,
          NULL);
    }
    if(ds_bearer_cntx_validate_bearer_context(def_bearer_context_ptr)&&
      (def_bearer_context_ptr->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_DOWN)) // clang_sa_ignore [core.NullDereference]
    {
        ds_bearer_cntxt_phys_link_down_cmd_hdlr(
                                    (uint32)def_bearer_context_ptr->\
                                    ds_bearer_context_dyn_p->client_data_ptr,
                                    NULL);
    }
  }
      else if(ds_3gpp_nv_manager_get_l_to_w_qos_continuity_nv() == FALSE)
      {      
        DS_3GPP_MSG1_HIGH("Tearing down ded bearer 0x%x",bearer_context_p);

        if (ds_flow_manager_teardown_bearer((void*)bearer_context_p,
                                            filter_preced,
                                            FALSE,
                                            FALSE) != 0)
        {
          DS_3GPP_MSG0_HIGH("Tearing down ded bearer cntxt failed");
        }
      }
      else 
      {
        ds_eps_bearer_cntxt_performResume(bearer_context_p);
      }
  }
  else 
  {
     ds_eps_bearer_cntxt_performResume(bearer_context_p);
  }

  return;

}

/*=============================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PERFORMRESUME

DESCRIPTION
  This is the function that is registered for EPS in the bearer context
  function table. The registration happens in eps_init.
  This function is called when the resume_ind_hdlr is invoked from DSMGR.
  This function is responsible for resuming connection to the lower layers.
  dswpsd_resume_pdp_proto_stack
      
PARAMETERS
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
==============================================================================*/
LOCAL void ds_eps_bearer_cntxt_performResume
(
  ds_bearer_context_s     * bearer_context_p
)
{
 
  ds_umts_pdp_type_enum_type    pdp_type;
  ps_iface_type                *iface_p;           /* Interface ptr for the call */
  int16                         ps_errno;
  dsm_watermark_type           *tx_wm_p= NULL;       /* Watermark pointer      */   
  ds_bearer_flow_manager_s     *bflow_manager_p = NULL;   
  phys_link_state_type          phys_link_state = PHYS_LINK_NULL;
#ifdef FEATURE_QXDM_WM_TRACING
  uint32                        callid;
#endif /* FEATURE_QXDM_WM_TRACING */
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  /*-----------------------------------------------------------------------*/

  DS_LTE_MSG1_HIGH("Resuming connection to LTE Radio Layer Inst:0x%x ",
                   bearer_context_p);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }


  bflow_manager_p = ds_bearer_flow_manager_find_flow_manager
    (bearer_context_p);

  if(!ds_bearer_flow_manager_validate_flow_manager(bflow_manager_p))
  {
    ds_bearer_cntxt_phys_link_down_cmd_hdlr(
                                    (uint32)bearer_context_p->\
                                    ds_bearer_context_dyn_p->client_data_ptr,
                                    NULL);  
    return;  
  }
      
  /*---------------------------------------------------------------------
    We need to know what kind of PDP call we are dealing with.
    This is required for configuring the PROTOCOL stacks appropriately
  ---------------------------------------------------------------------*/

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn
       ((bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr), &subs_id))
  {
    return;
  }

  pdp_type = ((ds_pdn_context_s*)(bearer_context_p->
                 ds_bearer_context_dyn_p->pdn_context_ptr))->
                   ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;

  if (pdp_type == DS_UMTS_PDP_PPP)
  {
    /*---------------------------------------------------------------
        Initiate call end for PDP-PPP call. We shall come here only
        if we started out in WCDMA mode and a RAT change happened to
        EPS/LTE mode.
        Clear uplink and downlink PDCP watermarks.
     ---------------------------------------------------------------*/

     ds_eps_bearer_cntxt_cleanup_pdcp_watermarks(bearer_context_p);
     /*------------------------------------------------------------------
        PDP-PPP i.e. Tear down the V4 iface
     ------------------------------------------------------------------*/
     iface_p = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(
                   (ds_pdn_context_s*)bearer_context_p->
                   ds_bearer_context_dyn_p->pdn_context_ptr);
     (void)ps_iface_tear_down_cmd(iface_p, &ps_errno, NULL);
     return;
  }
#ifdef FEATURE_QXDM_WM_TRACING
  else
  {
    callid = ds_bearer_cntxt_get_call_id(bearer_context_p);
    fc_post_modify_cmd_wm_trace(callid, 
                                UNIFIED_3GPP_MODE_HANDLER_CLIENT, LTE_TECH);
  }
#endif /* FEATURE_QXDM_WM_TRACING */
  
  if(bearer_context_p->ds_bearer_context_dyn_p->
     call_mode != ds3gpp_get_current_network_mode(subs_id))
  {
    DS_LTE_MSG1_HIGH_EX(
       subs_id,
      "Bearer inst:%d in mode:%d sys_mode:%d not requesting DRB reestab",
      (uint32)bearer_context_p->ds_bearer_context_dyn_p->
                     client_data_ptr);
  }

  /*---------------------------------------------------------------------  
    update flow if necessary
  ---------------------------------------------------------------------*/
  if (ds_eps_bearer_flow_manager_update_flow_on_resume(bflow_manager_p) 
                                                                  == FALSE)
  {
    DS_LTE_MSG1_HIGH_EX(subs_id,
                     "Flow update error on bearer instance 0x%d, skip",
                     (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  }

  /*---------------------------------------------------------------------  
    Enable flow on the phys link.        
  ---------------------------------------------------------------------*/
  phys_link_state = PS_PHYS_LINK_GET_STATE(&(bearer_context_p->
                                            ds_bearer_context_dyn_p->phys_link));
  /*---------------------------------------------------------------------  
    Check phys link state and post phys link up. After getting resume, we 
    cannot blindly post flow_enable() as it will not take effect if phys
    link is in transition states.
  ---------------------------------------------------------------------*/  
  if (!( ( PHYS_LINK_UP == phys_link_state) ||
         ( PHYS_LINK_DOWN == phys_link_state) ) )
  {
    ps_phys_link_down_ind( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link) );
  }
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                   DS_FLOW_UMTS_MASK, FALSE);

  if(bearer_context_p->ds_bearer_context_dyn_p->state == 
     DS_BEARER_CONTEXT_STATE_UP_DORMANT)
  { 
    /*-------------------------------------------------------------------
      If data is sitting in the Watermarks (PS to PDCP), trigger 
      rab re-establishment immly. 
    -------------------------------------------------------------------*/
    tx_wm_p = &(bearer_context_p->tx_wm.wm_item);//VRK
    if( tx_wm_p->current_cnt > 0 )
    {
      DS_LTE_MSG1_HIGH_EX(subs_id,
               "Data in WM on resume Bearer inst %d",
               (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
      ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL); 
    }
    else
    {
      /*-----------------------------------------------------------------  
        Register each enqueue function ptr.
      -----------------------------------------------------------------*/
      tx_wm_p->each_enqueue_func_ptr =
               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
      tx_wm_p->each_enqueue_func_data = (void*) bearer_context_p;
    }
  }/*  UP_DORMANT_REESTAB */
  return;
} /* ds_eps_bearer_cntxt_resume_ind_hdlr() */

/*=============================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PERFORMRESUMEVALIDATION()

DESCRIPTION
 This function evaluates if performResume must be called.
  A Resummption on LTE must be made in connected mode scenarios where the
  suspend flag is not set.

PARAMETERS
  new_mode: New Mode
  old_mode: Old Mode
  subs_id : Subscription Id
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS


===============================================================================*/
LOCAL void ds_eps_bearer_cntxt_performResumeValidation
(
  sys_sys_mode_e_type     new_mode,
  sys_sys_mode_e_type     old_mode,
  sys_modem_as_id_e_type  subs_id
)
{
  ds_bearer_context_s     *bearer_context_p = NULL;
  uint32                   bearer_cntx_inst = 0;
  boolean                  ps_data_suspend = 
                             ds3gpp_get_ps_data_suspend_flag(subs_id);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DS_3GPP_MSG3_LOW("In performResumeValidation,new_mode:%d,old_mode:%d "
                   "data_suspend %d",
                   new_mode, old_mode, ps_data_suspend);
  /*---------------------------------------------------------------------- 
   Invoke performResume only for PSHO case if
   1. Suspended flag is FALSE
   2. New mode is one of W/G/T
   3. Old mode was LTE (This would indicate IRAT)
   -----------------------------------------------------------------------*/
  
  if( (!ps_data_suspend) &&
      (old_mode == SYS_SYS_MODE_GSM ||
       old_mode == SYS_SYS_MODE_WCDMA
#ifdef FEATURE_TDSCDMA
       ||
       old_mode == SYS_SYS_MODE_TDS
#endif /* FEATURE_TDSCDMA */
       )                 &&
      (new_mode == SYS_SYS_MODE_LTE)
    )
  {
    /*----------------------------------------------------------------------
     This is one of the modes that we will be interested in
    ----------------------------------------------------------------------*/
    for( bearer_cntx_inst=0;
         bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;
         bearer_cntx_inst++)
    {
      bearer_context_p = 
        ds_bearer_cntxt_get_instance_ptr((uint8)bearer_cntx_inst);
  
      if (!ds_bearer_cntxt_validate_bearer_subscription_internal
            (bearer_context_p, subs_id))
      {
        continue;
      }

      if((bearer_context_p->ds_bearer_context_dyn_p->state ==
            DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) ||
         (bearer_context_p->ds_bearer_context_dyn_p->state ==
            DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
         (bearer_context_p->ds_bearer_context_dyn_p->state ==
            DS_BEARER_CONTEXT_STATE_UP))
      {  
        ds_eps_bearer_cntxt_resume_ind_hdlr(bearer_context_p);
      }
    }
  }

  return;

} /* ds_eps_bearer_cntxt_performResumeValidation */

#if defined(FEATURE_DATA_LTE) && (defined(FEATURE_GSM_GPRS) || defined(FEATURE_DATA_WCDMA_PS) || defined(FEATURE_TDSCDMA))

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_INIIT_BEARER_THROUGHPUT_STATS

DESCRIPTION
  This function Initializes the bearer throughput stats by querying
  from the LTE lower layers
      
PARAMETERS
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
void ds_eps_bearer_cntxt_init_bearer_throughput_stats
(
  ds_bearer_context_s     *bearer_context_p
)
{
  ds_eps_bearer_context_s   *eps_bearer_context_p   = NULL;
  uint8                      eps_bearer_id          = 0;
  sys_modem_as_id_e_type     subs_id =  SYS_MODEM_AS_ID_NONE;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &subs_id))
  {
    return;
  }
  /*------------------------------------------------------------------------- 
    Validate EPS bearer context
    -------------------------------------------------------------------------*/
  eps_bearer_context_p = bearer_context_p->ds_bearer_context_dyn_p->
                          vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return;
  }
  eps_bearer_id = eps_bearer_context_p->ds_eps_bearer_context_dyn_p->
                  eps_bearer_id;

  /*------------------------------------------------------------------------- 
    Initialize the previous received count with the current water mark
    count. When the timer expires, we get a better estimate of the
    difference in the byte count to calculate the actual throughput
    -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
       bearer_throughput_stats.actual_dl_rate  = 0;
  bearer_context_p->ds_bearer_context_dyn_p->
       bearer_throughput_stats.actual_ul_rate  = 0;

  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_rx_wm_cnt 
                              = bearer_context_p->rx_wm.wm_item.total_rcvd_cnt;

  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_tx_wm_cnt 
                              = lte_pdcpul_get_pdu_bytes_tx(eps_bearer_id,
                                                            (uint8)subs_id
                                                            );
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.prev_tx_wm_cnt_for_bidir 
                              = lte_pdcpul_get_pdu_bytes_tx(eps_bearer_id,
                                                            (uint8)subs_id
                                                            );

  DS_3GPP_MSG3_HIGH("Bearer call mode %d Init bearer throughput stats"
                    " rx WM cnt %lu tx WM Cnt %lu",
                    bearer_context_p->ds_bearer_context_dyn_p->call_mode,
                    bearer_context_p->ds_bearer_context_dyn_p->
                      bearer_throughput_stats.total_prev_recd_rx_wm_cnt,
                    bearer_context_p->ds_bearer_context_dyn_p->
                      bearer_throughput_stats.total_prev_recd_tx_wm_cnt);

  return;
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_COPY_UL_PACKET_METRICS

DESCRIPTION
  This function copies the UL packet metrics in the structure to be
  returned to client
      
PARAMETERS 
  L2_Tx_Delay: Contents from lower layer about RTT
  Rm_L2_Delay: Queueing delay from lower layers
  delay_metris_ptr: The struct that will contain RTT & Queueing metrics
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void ds_eps_bearer_cntxt_copy_ul_packet_metrics
(
  lte_rlc_delay_metric_s                      *l2_tx_delay_ptr,
  lte_rlc_delay_metric_s                      *rm_l2_queueing_delay_ptr,
  ps_iface_ioctl_per_bearer_ul_packet_delay_metrics_type *delay_metrics_ptr
)
{
  if(l2_tx_delay_ptr == NULL ||
     rm_l2_queueing_delay_ptr == NULL ||
     delay_metrics_ptr == NULL)
  {
    DS_LTE_MSG3_ERROR("Invalid Input Params,L2 Tx delay: 0xx RM L2 Delay: 0xx"
                      " Delay Metrics ptr: 0xx", l2_tx_delay_ptr, 
                      rm_l2_queueing_delay_ptr, delay_metrics_ptr);
    return;
  }

  delay_metrics_ptr->l2_tx_delay.avg = l2_tx_delay_ptr->avg;
  delay_metrics_ptr->l2_tx_delay.min = l2_tx_delay_ptr->min;
  delay_metrics_ptr->l2_tx_delay.max = l2_tx_delay_ptr->max;
  delay_metrics_ptr->l2_tx_delay.num_samples = l2_tx_delay_ptr->num_samples;
 
  delay_metrics_ptr->rm_l2_queueing_delay.avg = rm_l2_queueing_delay_ptr->avg;
  delay_metrics_ptr->rm_l2_queueing_delay.min = rm_l2_queueing_delay_ptr->min;
  delay_metrics_ptr->rm_l2_queueing_delay.max = rm_l2_queueing_delay_ptr->max;
  delay_metrics_ptr->rm_l2_queueing_delay.num_samples = 
                                         rm_l2_queueing_delay_ptr->num_samples; 
}

/*=============================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_UL_PACKET_METRICS

DESCRIPTION
  This function queries the LTE PDCP API to get UL Packet Metrics for a
  particular bearer, both Queueing delay and RTT are requested
      
PARAMETERS 
  bearer_context_ptr: Bearer whose metrics need to be queried
  delay_metris_ptr: The struct that will contain RTT & Queueing metrics
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: On successful metric collection from lower layers
  FALSE: Otherwise
  
SIDE EFFECTS
  None
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_get_ul_packet_metrics
(
  ds_bearer_context_s                                    *bearer_context_ptr,
  ps_iface_ioctl_per_bearer_ul_packet_delay_metrics_type *delay_metrics_ptr
)
{
  boolean                      ret_val = FALSE;
  ds_eps_bearer_context_s     *eps_bearer_context_ptr = NULL;
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;
  lte_rlc_delay_metric_s       lte_l2_tx_delay;
  lte_rlc_delay_metric_s       lte_rm_l2_queuing_delay;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr) ||
     delay_metrics_ptr == NULL)
  {
    DS_LTE_MSG2_ERROR_EX(subs_id, "Invalid Input params, bearer context & delay metrics: "
                      "0xx & 0xx",bearer_context_ptr, delay_metrics_ptr);
    return ret_val;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id);
  
  /*------------------------------------------------------------------------- 
    Validate EPS bearer context
  -------------------------------------------------------------------------*/
  eps_bearer_context_ptr = (ds_eps_bearer_context_s *)bearer_context_ptr->
               ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_ptr))
  {
    return ret_val;
  }

  delay_metrics_ptr->bearer_id = eps_bearer_context_ptr->
                                   ds_eps_bearer_context_dyn_p->eps_bearer_id;

  memset(&lte_l2_tx_delay, 0, sizeof(lte_rlc_delay_metric_s));
  memset(&lte_rm_l2_queuing_delay, 0, sizeof(lte_rlc_delay_metric_s));

  /*------------------------------------------------------------------------- 
    invoke LTE APIs and fill in the metrics
  -------------------------------------------------------------------------*/

  lte_rlc_get_delay_metric (&lte_l2_tx_delay, 
                            &lte_rm_l2_queuing_delay,
                            delay_metrics_ptr->bearer_id,
                            (uint8)subs_id
                            );

  ds_eps_bearer_cntxt_copy_ul_packet_metrics(&lte_l2_tx_delay, 
                                             &lte_rm_l2_queuing_delay,
                                             delay_metrics_ptr);
  ret_val = TRUE;
  return ret_val;
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_RESET_UL_PACKET_METRICS

DESCRIPTION
  This function calls the LTE PDCP API to reset UL Packet Metrics for a
  particular bearer, both Queueing delay and RTT are reset
      
PARAMETERS 
  bearer_context_ptr: Bearer whose metrics need to be queried
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: On successful metric reset message being conveyed to lower layers
  FALSE: Otherwise
  
SIDE EFFECTS
  None
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_reset_ul_packet_metrics
(
  ds_bearer_context_s        *bearer_context_ptr
)
{
  ds_eps_bearer_context_s                 *eps_bearer_context_ptr = NULL;
  sys_modem_as_id_e_type                   subs_id = SYS_MODEM_AS_ID_NONE;
  lte_rlcul_delay_metric_reset_req_msg_s   lte_rlc_ul_reset_metrics_req_msg;
  boolean                                  rtval = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
  {
    return rtval;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id);
  
  /*------------------------------------------------------------------------- 
    Validate EPS bearer context
  -------------------------------------------------------------------------*/
  eps_bearer_context_ptr = (ds_eps_bearer_context_s *)bearer_context_ptr->
               ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_ptr))
  {
    return rtval;
  }

  memset(&lte_rlc_ul_reset_metrics_req_msg, 0, 
         sizeof(lte_rlcul_delay_metric_reset_req_msg_s));
  /*------------------------------------------------------------------------- 
    Send MSG to LTE RLC to reset the metrics
  -------------------------------------------------------------------------*/
  lte_rlc_ul_reset_metrics_req_msg.eps_id = eps_bearer_context_ptr->
                                 ds_eps_bearer_context_dyn_p->eps_bearer_id;

  rtval = ds3gmsgr_msg_send_ext
            (LTE_RLCUL_REQ_DELAY_METRIC_RESET, 
             MSGR_DS_MSGRRECV,
             (msgr_hdr_struct_type*)(&lte_rlc_ul_reset_metrics_req_msg),
             sizeof(lte_rlcul_delay_metric_reset_req_msg_s),
             subs_id);

  if(rtval == FALSE) 
  {
    DS_LTE_MSG0_LOW(" Unable to send message to DSMSGR");
  }
  
  return rtval;
}
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_TRANSFER_RELEASE_HDLR()

DESCRIPTION
  This is the function that is registered for EPS in the bearer context
  function table. The registration happens in eps_init.
  This function is called when the resume_ind_hdlr is invoked from DSMGR.
  This function is responsible for resuming connection to the lower layers.
  dswpsd_resume_pdp_proto_stack
      
PARAMETERS
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_transfer_release_hdlr
(
  ds_bearer_context_s*       bearer_context_p
)
{
  ds_eps_bearer_context_s         *eps_bearer_context_p = NULL;  
  ds_pdn_context_s*                pdn_cntx_p = NULL;
  sys_modem_as_id_e_type           subs_id  = SYS_MODEM_AS_ID_NONE;
  ds_eps_attach_sm_s              *attach_sm_ptr = NULL;

  /*-----------------------------------------------------------------------*/    
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return ;
  }
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
  /*----------------------------------------------------------------------
        Get the mode specific blob this bearer
  --------------------------------------------------------------------*/
  eps_bearer_context_p = (ds_eps_bearer_context_s *)\
                 (bearer_context_p->ds_bearer_context_dyn_p->
                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);  
  
  if(! bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW)
  {
    DS_LTE_MSG1_HIGH_EX(subs_id,"bearer_inst:%d is not transferable to GW",
             (uint32)(bearer_context_p->ds_bearer_context_dyn_p->
                     client_data_ptr));
    return;
  }

  DS_LTE_MSG3_HIGH_EX(subs_id,"Processing cntxt_transfer_ind for bearer_inst:%d "
                   "Release connection to LTE Radio Layer Inst:0x%x "
                   "UM watermark cnt:%d", 
                   (uint32)bearer_context_p->ds_bearer_context_dyn_p->
                    client_data_ptr,
                    bearer_context_p,
                    bearer_context_p->tx_wm.wm_item.current_cnt);

  /*----------------------------------------------------------------------
        Get the mode specific blob this bearer
  --------------------------------------------------------------------*/
  eps_bearer_context_p = (ds_eps_bearer_context_s *)\
                 (bearer_context_p->ds_bearer_context_dyn_p->
                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return;
  }

  pdn_cntx_p = (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }
  attach_sm_ptr = ds_eps_get_attach_sm_per_sub_id(subs_id);
  if(attach_sm_ptr == NULL)
  {
    DS_LTE_MSG1_LOW("Invalid Attach SM ptr for Subs Id(CM) %d",subs_id);
    return;
  }
  /*------------------------------------------------------------------------ 
    Clear GW originated flag, Can we reset attach_sm_ptr here ? 
    ------------------------------------------------------------------------*/
  attach_sm_ptr->is_gw_originated = FALSE;

  DS_LTE_MSG2_LOW("Bearer_inst:%d UM watermark cnt:%d",
                   (uint32)bearer_context_p->ds_bearer_context_dyn_p->
                   client_data_ptr,bearer_context_p->tx_wm.wm_item.current_cnt);
  return;
} /* ds_eps_bearer_cntxt_transfer_release_hdlr() */
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REFRESH_ACTUAL_THROUGHPUT

DESCRIPTION
  This function refreshes the actual throughput numbers for all the
  active EPS bearers
  
PARAMETERS
   ds_bearer_context_s        *bearer_context_p,
   sys_modem_as_id_e_type      cm_subs_id, 
   uint32                      sampling_interval (msec)
DEPENDENCIES
  None.

RETURN VALUE
  TRUE    - If the actual throughput has been refreshed successfully
  FALSE   - Otherwise
  
SIDE EFFECTS
  
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_refresh_actual_throughput
(
  ds_bearer_context_s        *bearer_context_p,
  sys_modem_as_id_e_type      cm_subs_id, 
  uint32                      sampling_interval
)
{
  boolean                     ret_val               = FALSE;
  uint64                      prev_tx_wm_cnt        = 0;
  uint64                      current_tx_wm_cnt     = 0;
  uint64                      prev_rx_wm_cnt        = 0;
  uint64                      current_rx_wm_cnt     = 0;  
  uint64                      prev_tx_running_avg   = 0;
  uint64                      prev_rx_running_avg   = 0;
  ds_eps_bearer_context_s    *eps_bearer_context_p  = NULL;
  uint8                       eps_bearer_id         = 0;
  const uint8                 MOVING_AVG_COEFF      = 40;
  sys_modem_as_id_e_type      subs_id =  SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------- 
    Check if the sampling interval is valid to refresh actual throughput
    -------------------------------------------------------------------------*/
  if(sampling_interval == 0)
  {
    DS_3GPP_MSG0_HIGH("Not a valid sampling Interval to calculate actual tput");
    return ret_val;
  }

  /*------------------------------------------------------------------------- 
    Validate Incoming bearer context
    -------------------------------------------------------------------------*/
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &subs_id))
  {
    return ret_val;
  }

  /*------------------------------------------------------------------------- 
    Validate EPS bearer context for the LTE bearer context
    -------------------------------------------------------------------------*/
  eps_bearer_context_p = bearer_context_p->ds_bearer_context_dyn_p->
                          vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ];

  if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
  {
    return ret_val;
  }

  prev_rx_wm_cnt        = bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.total_prev_recd_rx_wm_cnt;
  current_rx_wm_cnt     = bearer_context_p->rx_wm.wm_item.total_rcvd_cnt;

  prev_tx_wm_cnt        = bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.total_prev_recd_tx_wm_cnt;  
  eps_bearer_id         = eps_bearer_context_p->ds_eps_bearer_context_dyn_p\
                          ->eps_bearer_id;
  /*------------------------------------------------------------------------- 
    Get Num bytes transferred over UL from PDCP for the current EPS Bearer ID
    -------------------------------------------------------------------------*/
  current_tx_wm_cnt     = lte_pdcpul_get_pdu_bytes_tx(eps_bearer_id,
                                                      (uint8)subs_id
                                                      );

  prev_rx_running_avg   = bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.actual_dl_rate;
  prev_tx_running_avg   = bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.actual_ul_rate;

  /*------------------------------------------------------------------------- 
    Calculate new UL actual throughput rate
    -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
     bearer_throughput_stats.actual_ul_rate = ds_util_calculate_throughput_moving_avg
                                              (
                                                 (uint32)prev_tx_running_avg,
                                                 prev_tx_wm_cnt,
                                                 current_tx_wm_cnt,
                                                 sampling_interval,
                                                 MOVING_AVG_COEFF
                                              );
  DS_3GPP_MSG6_MED("Prev UL Bytes Cnt transf 0x%lx%lx LTE ML1 reports Current"
                   " byte cnt 0x%lx%lx Calculated Avg actual t'put 0x%lx%lx kbps",
                   QWORD_HIGH(prev_tx_wm_cnt),
                   QWORD_LOW(prev_tx_wm_cnt),
                   QWORD_HIGH(current_tx_wm_cnt),
                   QWORD_LOW(current_tx_wm_cnt),
                   QWORD_HIGH(bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.actual_ul_rate),
                   QWORD_LOW(bearer_context_p->ds_bearer_context_dyn_p->
                          bearer_throughput_stats.actual_ul_rate));

  /*------------------------------------------------------------------------- 
    Calculate the new DL actual throughput rate
    -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
  bearer_throughput_stats.actual_dl_rate    = ds_util_calculate_throughput_moving_avg
                                              (
                                                 (uint32)prev_rx_running_avg,
                                                 prev_rx_wm_cnt,
                                                 current_rx_wm_cnt,
                                                 sampling_interval,
                                                 MOVING_AVG_COEFF
                                              );

  /*------------------------------------------------------------------------- 
    Store the new DL and UL Rx WM byte count for the next run
    -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_rx_wm_cnt = current_rx_wm_cnt;

  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_tx_wm_cnt = current_tx_wm_cnt;

  ret_val = TRUE;

  return ret_val;
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_TRANSFER_SETUP_HDLR()

DESCRIPTION
  This is the function that is registered for EPS in the bearer context
  function table. The registration happens in eps_init.
  This function is called when the context_transfer_ind_hdlr is invoked.
  This function is responsible for setting up the stack for the new RAT.
  dswpsd_resume_pdp_proto_stack
      
PARAMETERS
  bearer_context_p
 
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  
  
===========================================================================*/
LOCAL void ds_eps_bearer_cntxt_transfer_setup_hdlr
(
  ds_bearer_context_s  *bearer_context_p
)
{
 
  ds_umts_pdp_type_enum_type    pdp_type;
  uint32                        eps_bearer_index; 
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;  

  /*-----------------------------------------------------------------------*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS3GPP_ASSERT(0);
    return ;
  }
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
  /*---------------------------------------------------------------------
    Associate the mode specific blob that is going to be populated to the 
    bearer context pointer.
  ---------------------------------------------------------------------*/
  eps_bearer_index = (uint32)bearer_context_p->
    ds_bearer_context_dyn_p->client_data_ptr;
  bearer_context_p->ds_bearer_context_dyn_p->
    vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ] =\
  (void*)&eps_bearer_context_tbl[eps_bearer_index];


   if(!ds_eps_bearer_cntxt_validate_bearer_cntxt
      (&eps_bearer_context_tbl[eps_bearer_index]))
  {
    return;
  }
  /*--------------------------------------------------------------------
    Associate this EPS bearer context with bearer context
  ---------------------------------------------------------------------*/
  eps_bearer_context_tbl[eps_bearer_index].ds_eps_bearer_context_dyn_p->
    bearer_context_p = bearer_context_p;

  /*--------------------------------------------------------------------- 
    Update the EPS bearer id for this bearer
  ---------------------------------------------------------------------*/
  if( (eps_bearer_context_tbl[eps_bearer_index].
       ds_eps_bearer_context_dyn_p->eps_bearer_id == 0)&&
      (bearer_context_p->ds_bearer_context_dyn_p->nsapi != 0) )
  {        
    eps_bearer_context_tbl[eps_bearer_index].ds_eps_bearer_context_dyn_p->eps_bearer_id =
                        bearer_context_p->ds_bearer_context_dyn_p->nsapi;
  }
  
  DS_LTE_MSG3_HIGH_EX(subs_id,"Context transfer to LTE Radio Layer Inst:0x%x "
                   "Using eps_bearer_id %d for BearerInst:%d",
                   bearer_context_p,
                   eps_bearer_context_tbl[eps_bearer_index].ds_eps_bearer_context_dyn_p->
                   eps_bearer_id,
                  (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  
  /*---------------------------------------------------------------------  
    Set the PS->PDCP watermark.
  ---------------------------------------------------------------------*/
  ds_eps_bearer_cntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p);            
   
 /*   We need to know what kind of PDP call we are dealing with.
    This is required for configuring the PROTOCOL stacks appropriately
  ---------------------------------------------------------------------*/
  
  if(!ds_3gpp_pdn_cntx_validate_pdn_context(bearer_context_p->
                                 ds_bearer_context_dyn_p->pdn_context_ptr))
  {
    return;
  }

  pdp_type = ((ds_pdn_context_s*)(bearer_context_p->
                                 ds_bearer_context_dyn_p->pdn_context_ptr))->
    ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;

  if( (pdp_type == DS_UMTS_PDP_IPV4)
#ifdef FEATURE_DATA_PS_IPV6
       ||
       (pdp_type == DS_UMTS_PDP_IPV6)
       ||
       (pdp_type == DS_UMTS_PDP_IPV4V6)
#endif /* FEATURE_DATA_PS_IPV6 */
    )
  {
    
  if (ds_3gpp_nv_manager_get_data_shark() == TRUE)
  {
    DS_LTE_MSG0_HIGH_EX(subs_id,"DS_3GPP_DATA_SHARK_ENABLE NV enabled. "
                     "Registering loopback tx cb");
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                 ds_3gpp_loopback_pdp_data_ctrl_setup_cb,
                                 bearer_context_p);
  }
  else
  {
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                 ds_eps_bearer_cntxt_pdp_ip_tx_um_data_cb,
                                 bearer_context_p);
  }

  ds_eps_tlb_validate_set_tlb_tx_function(bearer_context_p);


    /*-------------------------------------------------------------------------
      Set the handler for the downlink signal for the call.
      PS handles are assigned when the bearer is allocated, and invalidated
      when the bearer is deallocated. The original handle is passed to the 
      callback to make sure that only bearers with valid handles are processed.
    -------------------------------------------------------------------------*/
    (void)ps_set_sig_handler(bearer_context_p->ds_bearer_context_dyn_p->rx_sig,
                    ds_eps_bearer_cntxt_pdp_ip_rx_um_data_cb, 
                  (void *)bearer_context_p->ds_bearer_context_dyn_p->ps_handle);

                 
  }/* (pdp_type != DS_UMTS_PDP_PPP) */    

  

  return;
} /* ds_eps_bearer_cntxt_transfer_setup_hdlr() */

#endif /* defined(FEATURE_DATA_LTE) && defined(FEATURE_GSM_GPRS || FEATURE_DATA_WCDMA_PS || FEATURE_TDSCDMA) */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_INIT

DESCRIPTION
  This function is used to initialize the EPS bearer context. 

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_init( void )
{
  ds_bearer_context_ftbl_s    *eps_bearer_context_ftbl_p;
  ds3gsubsmgr_subs_id_e_type   subs_indx;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset((void*)eps_bearer_context_tbl,
         0x0,
         (sizeof(ds_eps_bearer_context_s) * DS_3GPP_MAX_BEARER_CONTEXT)
        );

  for(subs_indx = DS3GSUBSMGR_SUBS_ID_MIN;
      subs_indx < DS3GSUBSMGR_SUBS_ID_MAX;
      subs_indx++)
  {
    ds_3gpp_div_duplex[subs_indx] = SYS_DIV_DUPLEX_NONE;
    ds_eps_bearer_cntxt_ue_cat_id[subs_indx] = LTE_UE_CLASSES_DEFAULT;
  }

  eps_bearer_context_ftbl_p = &(ds_bearer_context_ftbl[DS_3GPPI_SYS_MODE_LTE]);

  memset((void*)eps_bearer_context_ftbl_p,
         0x0,
         (sizeof(ds_bearer_context_ftbl_s)));

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, dormancyReorig, \
                        ds_bearer_cntxt_reorig_dorm);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, getOrigParams, \
                        ds_eps_bearer_cntxt_get_orig_params);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, deactivateCnf, \
                        ds_eps_bearer_cntxt_deactivate_cnf);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, bearerActivateInd, \
                        ds_eps_bearer_cntxt_bearer_activate_ind);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, bring_up, \
                        ds_eps_bearer_cntxt_bring_up_pdn_connection);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, tear_down, \
                        ds_eps_bearer_cntxt_tear_down_pdn_connection);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, rab_re_estab_ind, \
                        ds_eps_bearer_cntxt_call_rab_re_estab_ind_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, rab_rel_ind, \
                        ds_eps_bearer_cntxt_call_rab_rel_ind_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_bearer_tech_type, \
                        ds_eps_bearer_cntxt_get_bearer_tech_type);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_div_duplex, \
                        ds_eps_bearer_cntxt_get_div_duplex);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, set_div_duplex, \
                        ds_eps_bearer_cntxt_set_div_duplex);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, allocate_eps_bearer, \
                            ds_eps_bearer_cntxt_alloc_dyn_mem);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, free_eps_bearer, \
                          ds_eps_bearer_cntxt_free_dyn_mem);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, suspend_ind, \
                        ds_eps_bearer_cntxt_suspend_ind_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, resume_ind, \
                                    ds_eps_bearer_cntxt_resume_ind_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, resume_validate, \
                        ds_eps_bearer_cntxt_performResumeValidation);

#ifdef FEATURE_DATA_WLAN_MAPCON
  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, getSoMask, \
                        ds_eps_bearer_cntxt_get_so_mask);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_bearer_tech_type_ex, \
                        ds_eps_bearer_cntxt_get_bearer_tech_type_ex);
#endif /* FEATURE_DATA_WLAN_MAPCON */

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_data_trfr_activity, \
                        ds_eps_bearer_cntxt_get_data_trfr_activity);

 DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_bearer_rate, \
                        ds_eps_bearer_cntxt_get_bearer_rate);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, examine_if_throt_needed, \
                        ds_eps_bearer_cntxt_examine_if_throt_needed);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, redial, \
                                    ds_eps_bearer_cntxt_redial_hdlr);  

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, go_null, \
                                    ds_eps_bearer_cntxt_go_null_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, dereg_pdcp, \
                                     ds_eps_bearer_cntxt_dereg_pdcp);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, get_ul_packet_metrics, \
                                    ds_eps_bearer_cntxt_get_ul_packet_metrics);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, reset_ul_packet_metrics, \
                                    ds_eps_bearer_cntxt_reset_ul_packet_metrics);

#if defined(FEATURE_DATA_LTE) && (defined(FEATURE_GSM_GPRS) || defined(FEATURE_DATA_WCDMA_PS) || defined(FEATURE_TDSCDMA))

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, cntxt_transfer_ind_setup, \
                                    ds_eps_bearer_cntxt_transfer_setup_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, cntxt_transfer_ind_release, \
                                    ds_eps_bearer_cntxt_transfer_release_hdlr);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, refresh_actual_throughput, \
                        ds_eps_bearer_cntxt_refresh_actual_throughput);

  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, init_bearer_throughput_stats, \
                        ds_eps_bearer_cntxt_init_bearer_throughput_stats);

#ifdef FEATURE_QXDM_WM_TRACING
  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, report_wm_stats, \
                        ds_eps_bearer_cntxt_report_wm_stats);
#endif /* FEATURE_QXDM_WM_TRACING */
  DS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_LTE, empty_watermark, \
                                    ds_eps_bearer_cntxt_empty_watermark);

#endif /* defined(FEATURE_DATA_LTE) && defined(FEATURE_GSM_GPRS || FEATURE_DATA_WCDMA_PS || FEATURE_TDSCDMA) */
  /*------------------------------------------------------------------------
    Register the PDCP MSGRTR for UL PDCP CNF
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPUL_RAB_REGISTER_CNF,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpul_rab_reg_cnf_msg_hdlr
  );

  /*------------------------------------------------------------------------
    Register the PDCP MSGRTR for DL PDCP CNF
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPDL_RAB_REGISTER_CNF,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpdl_rab_reg_cnf_msg_hdlr
  );

  /*------------------------------------------------------------------------
    Register the PDCP MSGRTR for UL PDCP  DEREG CNF
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPUL_RAB_DEREGISTER_CNF,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpul_rab_dereg_cnf_msg_hdlr
  );

  /*------------------------------------------------------------------------
    Register the PDCP MSGRTR for DL PDCP  DEREG CNF
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPDL_RAB_DEREGISTER_CNF,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpdl_rab_dereg_cnf_msg_hdlr
  );

  /*------------------------------------------------------------------------
    Register the PDCP MSGRTR for PKT DICARD IND
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPUL_PKT_DISCARD_IND,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpul_pkt_discard_ind_msg_hdlr
  );

  /*------------------------------------------------------------------------
    Register the RRC MSGRTR for UE CAP INFO IND
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_RRC_UE_CAP_INFO_IND,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_ue_cap_info_ind_msg_hdlr
  );

#ifdef FEATURE_LAPP
  /*---------------------------------------------------------------------------
    Register for PDCP MSG to listen to implicit WM deregister request
  ---------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_PDCPUL_IMPLICIT_DEREG_REQ,
            (ds3g_msgr_client_func_ptr)\
            ds_eps_bearer_cntxt_pdcpul_implicit_dereg_req_msg_hdlr
  );
#endif /* FEATURE_LAPP */

#ifdef FEATURE_DATA_LTE_RAN_ASSIST_CODEC_RATE
  /*------------------------------------------------------------------------
    Register the LTE REC BIT RATE for recommanded bit rate indication.
  ------------------------------------------------------------------------*/
  ds3gmsgr_reg_msgr_cb(
            LTE_MAC_REC_BIT_RATE_IND,
            (ds3g_msgr_client_func_ptr)ds_eps_bearer_cntxt_ex_rec_bit_rate_ind_hdlr
  );
#endif /*FEATURE_DATA_LTE_RAN_ASSIST_CODEC_RATE*/
  /*------------------------------------------------------------------------
    Initialize TLB 
  ------------------------------------------------------------------------*/
  dsEpsTlb_init();
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GETFLOWDIRECTION

DESCRIPTION
  This function returns the direction of the flow in a particular bearer.

PARAMETERS
  bearer_context_p - pointer to the bearer context
DEPENDENCIES
  None.

RETURN VALUE
ds_eps_bearer_context_flow_dir_e - Flow direction for this bearer
  
SIDE EFFECTS
  None.

===========================================================================*/
ds_eps_bearer_context_flow_dir_e ds_eps_bearer_cntxt_getFlowDirection
(
  ds_bearer_context_s*  bearer_context_p
)
{
  ds_bearer_flow_manager_s *bflow_manager_p = NULL;
  ds_eps_bearer_flow_manager_s *eps_bflow_manager_p = NULL;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return DS_EPS_FLOW_DIR_INVALID;
  }

  // default flow is always bidirectional
  if (bearer_context_p->ds_bearer_context_dyn_p->is_default)
  {
    return DS_EPS_FLOW_DIR_BIDIRECTIONAL;
  }
  else
  {
    bflow_manager_p = ds_bearer_flow_manager_find_flow_manager(bearer_context_p);
    if(!ds_bearer_flow_manager_validate_flow_manager(bflow_manager_p))
    {
      return DS_EPS_FLOW_DIR_INVALID;
    }
    eps_bflow_manager_p = 
     (ds_eps_bearer_flow_manager_s *)(bflow_manager_p->ds_bearer_flow_manager_dyn_p
                                      ->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

    if(!ds_eps_bearer_flow_manager_validate_eps_bearer_flow_manager(eps_bflow_manager_p))
    {
      DS_LTE_MSG0_LOW("eps_bearer_context_p is NULL");
      return DS_EPS_FLOW_DIR_INVALID;
    }
    
    if ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
        >=(byte)(LTE_QCI_5) &&
        eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci
           <=(byte)(LTE_QCI_9) )||
        ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
            ==(byte)(LTE_QCI_69)) ||
         (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
            ==(byte)(LTE_QCI_70)) ||
         (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
            ==(byte)(LTE_QCI_79)))
        )
    {
      // if the QCI is non-GBR, it is a bidirectional flow
      return DS_EPS_FLOW_DIR_BIDIRECTIONAL;
    }
    else if ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
              >=(byte)(LTE_QCI_1) &&
             eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
                 <=(byte)(LTE_QCI_4))||
             ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
                 ==(byte)(LTE_QCI_65)) ||
             (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
                ==(byte)(LTE_QCI_66)) ||
              (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.qci 
                ==(byte)(LTE_QCI_75)))
             )
    {
      // QCI is GBR, decide based on UL and DL rate
      if ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_dl
            != DS_EPS_0KB_BITRATE_VALUE) &&
          (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_ul 
           != DS_EPS_0KB_BITRATE_VALUE))
      {
        return DS_EPS_FLOW_DIR_BIDIRECTIONAL;
      }
      else if ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_dl 
                != DS_EPS_0KB_BITRATE_VALUE) &&
               (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_ul 
                == DS_EPS_0KB_BITRATE_VALUE))
      {
        return DS_EPS_FLOW_DIR_DOWNLINK;
      }
      else if ((eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_dl
                 == DS_EPS_0KB_BITRATE_VALUE) &&
               (eps_bflow_manager_p->eps_bearer_flow_manager_dyn_p->sdf_qos.gbr_ul 
                != DS_EPS_0KB_BITRATE_VALUE))
      {
        return DS_EPS_FLOW_DIR_UPLINK;
      }
      else
      {
        return DS_EPS_FLOW_DIR_INVALID;
      }
    }/* else if */
    else
    {
      return DS_EPS_FLOW_DIR_INVALID;
    }
  }/* non-default bearer*/
  //return DS_EPS_FLOW_DIR_INVALID;
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_ISIM_REFERSH_VOTING_HDLR

DESCRIPTION
  This function handles isim refresh voting event for LTE mode.

PARAMETERS
  bearer_context_p - pointer to the bearer context
  subs_id          - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
boolean - Whether ISIM fresh can proceed or not
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_isim_refresh_voting_hdlr
(
  sys_modem_as_id_e_type      subs_id
)
{
  uint32                pdn_cntx_inst;
  uint8                 iface_index, bearer_index;
  ds_3gpp_iface_s      *ds_iface_ptr = NULL;
  ps_iface_type        *iface_ptr = NULL;
  ds_bearer_context_s  *bearer_context_ptr = NULL;
  ds_pdn_context_s     *pdn_context_ptr = NULL;
  
  /*--------------------------------------------------------------------------
    For each active PDN, we check the following:
    If all bearer it has are in down or dormant state, we all refresh
    If all the ifaces are out of use
  --------------------------------------------------------------------------*/
  for( pdn_cntx_inst=0;
       pdn_cntx_inst<DS_3GPP_MAX_PDN_CONTEXT;
       pdn_cntx_inst++ )
  {
    pdn_context_ptr = &(ds_pdn_context_tbl[pdn_cntx_inst]);
    if ((!ds_pdn_cntxt_validate_pdn_subscription_internal(pdn_context_ptr, subs_id)) || 
        (pdn_context_ptr->ds_pdn_context_dyn_p->state != DS_PDN_CONTEXT_STATE_UP &&
        pdn_context_ptr->ds_pdn_context_dyn_p->state != 
         DS_PDN_CONTEXT_STATE_COMING_UP ))
    {
      continue;
    }
    // check if any of the iface is in us, i.e. pdn is in use by an app
    for (iface_index = 0; 
         iface_index < DS_3GPP_MAX_IFACE_PER_PDN_CONTEXT; 
         iface_index++)
    {
      /*--------------------------------------------------------------------------- 
        Get the DS Iface
      ---------------------------------------------------------------------------*/
      ds_iface_ptr = pdn_context_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[iface_index];
      if (ds_iface_ptr == NULL)
      {
        continue;
      }
      iface_ptr = &(ds_iface_ptr->ps_iface);
      if (iface_ptr == NULL)
      {
        continue;
      }
      if (PS_IFACE_IS_IN_USE(iface_ptr))
      {
        DS_LTE_MSG2_HIGH_EX(subs_id,"LTE pdn cntx %d iface:%d is in use while refresh voting",
                         pdn_cntx_inst, iface_ptr);
        // if iface is in use, we see if rab is dormant
        for(bearer_index = 0;
            bearer_index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;
            bearer_index++)
        {
          bearer_context_ptr =
              (ds_bearer_context_s*)pdn_context_ptr->ds_pdn_context_dyn_p
            ->bearer_context_ptr_tbl[bearer_index];

          if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_ptr))
          {
            /*----------------------------------------------------------------------- 
              This entry in the bearer context table is NULL. Other entries could
              have non-NULL values.
            -----------------------------------------------------------------------*/
            continue;
          }
          if (bearer_context_ptr->ds_bearer_context_dyn_p->state == 
              DS_BEARER_CONTEXT_STATE_UP ||
              bearer_context_ptr->ds_bearer_context_dyn_p->state ==
               DS_BEARER_CONTEXT_STATE_COMING_UP)
          {
            DS_LTE_MSG1_HIGH_EX(subs_id,"LTE ISIM refresh handler voted NO for pdn cntx=%d",
                             pdn_cntx_inst);
            return FALSE;
          }
        }
      }
    }  
  }
  return TRUE;
}

#ifdef FEATURE_QXDM_WM_TRACING
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REPORT_WM_STATS

DESCRIPTION
  This function stores the watermark status in wm_stat_ptr.

PARAMETERS
  bearer_instance - bearer context instance 
  wm_stat_ptr     - where the wm values are stored

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_eps_bearer_cntxt_report_wm_stats
(
  uint32          bearer_instance,
  fc_wm_stat_type *wm_stat_ptr
)
{
  ds_bearer_context_s         *bearer_context_p;
  uint32                       call_id;
  int                          index;
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;

  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/
  if( bearer_instance >= DS_3GPP_MAX_BEARER_CONTEXT )
  {
     DS_LTE_MSG0_ERROR("bearer_instance is greater than or equal to "
               "DS_3GPP_MAX_BEARER_CONTEXT ");
     return;
  }
  else
  {
     bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);
  }

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
  /*------------------------------------------------------------
    Get the call ID 
  --------------------------------------------------------------*/
  call_id = ds_bearer_cntxt_get_call_id(bearer_context_p);

  /*---------------------------------------------------------------------------
    WM_DIR_A2_PS_DL
  ----------------------------------------------------------------------------*/
  /* Index to wm_info array provided by FC */
  index = wm_stat_ptr->num_wms;
  if(index >= MAX_WM_CNT) 
  {
      DS_LTE_MSG0_ERROR_EX(subs_id, "Max WM count reached. Cannot report statistics");
      return;
  }

  DS_LTE_MSG0_LOW("Reporting statistics for WM_DIR_A2_PS_DL/WM_DIR_PDCP_PS_UL");
 
 /* Report the technology */
  wm_stat_ptr->wm_info[index].tech_id = LTE_TECH;

  /* Report the Call ID */
  wm_stat_ptr->wm_info[index].call_id = call_id;

  /* Specify the WM for which the stats are being reported */
  wm_stat_ptr->wm_info[index].wm_id   = WM_DIR_A2_PS_DL;

  /* query DSM for wm stats */
  dsm_get_wm_stats(&(bearer_context_p->rx_wm.wm_item), 
                   &(wm_stat_ptr->wm_info[index].wm_counts));
  wm_stat_ptr->num_wms++;

  /*---------------------------------------------------------------------------
    WM_DIR_PDCP_PS_UL
  ----------------------------------------------------------------------------*/
  /* Index to wm_info array provided by FC */
  index = wm_stat_ptr->num_wms;
  if(index >= MAX_WM_CNT) 
  {
     DS_LTE_MSG0_ERROR_EX(subs_id, "Max WM count reached. Cannot report statistics");
     return;
  }

  /* Report the technology */
  wm_stat_ptr->wm_info[index].tech_id = LTE_TECH;

  /* Report the Call ID */
  wm_stat_ptr->wm_info[index].call_id = call_id;

  /* Specify the WM for which the stats are being reported */
  wm_stat_ptr->wm_info[index].wm_id   = WM_DIR_PDCP_PS_UL;

  /* query DSM for wm stats */
  dsm_get_wm_stats(&(bearer_context_p->ds_bearer_context_dyn_p->
                     tx_wm.wm_item), 
                   &(wm_stat_ptr->wm_info[index].wm_counts));
  wm_stat_ptr->num_wms++;

} /* ds_eps_bearer_cntxt_report_wm_stats */
#endif /* FEATURE_QXDM_WM_TRACING */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_REQ_DEACT_FOR_PENDING_BEARERS

DESCRIPTION
  This function initiates call end for bearers that are in COMING_UP state. 
  
PARAMETERS  
  subs_id: Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns TRUE if call end was initiated with CM for any pending bearers.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_req_deact_for_pending_bearers
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s              * bearer_context_p= NULL;
  uint32                             bearer_cntx_inst= 0;
  boolean                            any_pending_bearers = FALSE;
  ds_pdn_context_s                 * pdn_context_p = NULL;
  ds_umts_pdp_context_type           attach_pdp_context;
  ds_umts_pdp_profile_status_etype   status = DS_UMTS_PDP_FAIL;
  uint16                             def_profile_num = 0;
  boolean                            found_match = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = 
      ds_bearer_cntxt_get_instance_ptr((uint8)bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p, subs_id))
    {
      continue;
    }

    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_COMING_UP) 
    {   
      DS_LTE_MSG2_HIGH_EX(subs_id,
                       "Bearer_inst:%d, call_id:%d in COMING_UP state, ending call",
                       (uint32)bearer_cntx_inst,
                       bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);        

      ds_bearer_cntxt_phys_link_down_cmd_hdlr(bearer_cntx_inst, NULL);

      /*---------------------------------------------------------------------
        If bearer was locally deactivated in DS then state will be DOWN and
        not GOING_DOWN. We do not redial calls for this local deactivation
        in DS scenario
      ---------------------------------------------------------------------*/ 
      /* If it is Local Deactivation bearer would have been freed already,
         so Validate the bearer context before accessing it  */
      if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
      {
        continue;
      }
      if(bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_GOING_DOWN )
      {
        any_pending_bearers = TRUE;

        /* we only support silent redial on default pdn*/
        pdn_context_p = (ds_pdn_context_s*)(bearer_context_p->
                                    ds_bearer_context_dyn_p->pdn_context_ptr);

        def_profile_num = ds_eps_get_attach_prof(subs_id);

        status = ds_umts_get_pdp_profile_context_info_per_subs( 
                                         def_profile_num,
                                         dsumts_subs_mgr_get_subs_id(subs_id),
                                         &attach_pdp_context );

        if ((status == DS_UMTS_PDP_SUCCESS) &&
            (attach_pdp_context.pdp_type == 
             pdn_context_p->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type) &&
            (strncasecmp((char*)pdn_context_p->ds_pdn_context_dyn_p->
             pdp_profile.context.apn, (char*)attach_pdp_context.apn, 
             DS_UMTS_MAX_APN_STRING_LEN+1) == 0))
        {
          found_match = TRUE;
        }

        if ((bearer_context_p->ds_bearer_context_dyn_p->is_default == TRUE) && 
            (ds_pdn_cntxt_is_default(pdn_context_p)) &&
            (ds_eps_pdn_cntxt_is_silent_redial_allowed(pdn_context_p)) &&
            (found_match == TRUE))
        {
          ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                           DS_BEARER_CONTEXT_STATE_PEND_REDIAL);

          bearer_context_p->ds_bearer_context_dyn_p->call_mode = SYS_SYS_MODE_LTE;
          
          DS_LTE_MSG2_HIGH_EX(subs_id,"Changed the bearer call mode to LTE. \n "
                  "initiated call end for call_id%d Bearer_inst:%d pending redial",
                  bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                  (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
        }
      }
    }
  /*---------------------------------------------------------------------
  If bearer is in going down state cache attach SM state so that we can start 
   attach procedure after ending this call successfully
  ---------------------------------------------------------------------*/
    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_GOING_DOWN) 
    {
      DS_LTE_MSG0_HIGH_EX(subs_id,"bearer is in going down state, return pending bearers TRUE"
                       " to cache attach request");
      any_pending_bearers = TRUE;
    }
  }
  return any_pending_bearers;

} /* ds_bearer_cntxt_req_deact_for_pending_bearers */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_SEQ_NUM

DESCRIPTION
  This function returns the sequence number that would be sent as a part of
  PDN connectivity request
  
PARAMETERS  
  None.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns the seq_num
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
static uint8 ds_eps_bearer_cntxt_get_seq_num
(
   sys_modem_as_id_e_type  subs_id
)
{
  ds_eps_attach_sm_s *attach_sm_ptr = NULL;
  uint8 seq_num = 0;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  attach_sm_ptr = ds_eps_get_attach_sm_per_sub_id(subs_id);
  if(attach_sm_ptr == NULL)
  {
    DS_3GPP_MSG1_LOW("Invalid Attach SM ptr for Subs Id (CM) %d",subs_id);
    return 0;
  }

  /*-------------------------------------------------------------------------
    Populate the sequence number.
    If the attach_sm_ptr state is attached then the PDN conn request is for addtl
    PDN, in this case populate the sequence number to be 0 always.
    For the attach case populate the attach_sm_ptr's seq_num. 
    Assumption: For addtl PDN conn requests we must always be attached 
  ---------------------------------------------------------------------------*/
  if(ds_eps_attach_sm_get_state(subs_id) != DS_EPS_ATTACHED)
  {
    seq_num = attach_sm_ptr->seq_num;
  }

  DS_LTE_MSG2_HIGH_EX(subs_id,"Sequence number returned is %d on Subs Id(CM) %d",
                    seq_num,subs_id);
  return seq_num;
}

/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_send_QOS_indication

DESCRIPTION
  This function sends tehe QCI information of a particular bearer to interested
  parties.
  
PARAMETERS  
    bearer_context_p - Bearer Context.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_send_QOS_indication
(
   ds_bearer_context_s *bearer_context_p
)
{ 
  ds_eps_bearer_context_s     *eps_bearer_context_p = NULL;
  ds_3gpp_lte_qos_params_type  lte_netw_alloc_qos = {0};  
  sys_modem_as_id_e_type       subs_id;


  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  

  ds_eps_bearer_flow_manager_get_lte_netw_alloc_qos(bearer_context_p,
                                                    &lte_netw_alloc_qos);

  eps_bearer_context_p = (ds_eps_bearer_context_s*)
 (bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);  

  if ((eps_bearer_context_p != NULL) && (lte_netw_alloc_qos.valid_flg == TRUE)
      && (ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p)))
  {    
    ds_3gpp_bearer_qos_info_msg_s qos_info_msg = {{0}};

    qos_info_msg.current_bearer_rat = DS_RAT_LTE;
    qos_info_msg.u.lte_qos_info.eps_bearer_id = eps_bearer_context_p
                                      ->ds_eps_bearer_context_dyn_p->eps_bearer_id;

    DS_LTE_MSG1_MED_EX(subs_id,"QCI of bearer is %d",lte_netw_alloc_qos.qci);   
    qos_info_msg.u.lte_qos_info.qci_value = 
                        (ds_3gpp_lte_qci_enum_type)lte_netw_alloc_qos.qci;

    (void) ds3gmsgr_msg_send_ext( 
       DS_3GPP_BEARER_QOS_INFO_IND,
       MSGR_DS_3GPP, 
       (msgr_hdr_struct_type*)(&qos_info_msg),
       sizeof(qos_info_msg),
       subs_id
    );    
  }

}


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_DL_BYTE_COUNT

DESCRIPTION
  This function is used to get number of dl byte count on the bearer

PARAMETERS
  eps_bearer_id - eps_bearer_id_T

DEPENDENCIES
  None.

RETURN VALUE
  Number of bytes received on the beaerer id
  
SIDE EFFECTS
  None.

===========================================================================*/

static uint32 ds_eps_bearer_cntxt_get_dl_byte_count
(
  eps_bearer_id_T eps_bearer_id
)
{
  uint8  bearer_cntx_inst = 0;
  uint32 received_byte_count = 0;
  ds_bearer_context_s       *bearer_context_p = NULL;
  ds_eps_bearer_context_s           *eps_bearer_context_p = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_LTE_MSG1_LOW("ds_eps_bearer_cntxt_get_dl_byte_count for bear %d", eps_bearer_id);
  for(bearer_cntx_inst = 0;\
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;\
      bearer_cntx_inst++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      continue;
    }

    eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->
                                  vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
    {
      continue;
    }

    if (eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id == 
        eps_bearer_id) 
    {
       received_byte_count = 
           bearer_context_p->ds_bearer_context_dyn_p->total_bytes_received;
       break;
    }

  }
  return received_byte_count;
}


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_SET_DL_BYTE_COUNT

DESCRIPTION
  This function is used to get number of dl byte count on the bearer

PARAMETERS
  eps_bearer_id - eps_bearer_id_T

DEPENDENCIES
  None.

RETURN VALUE
  Number of bytes received on the beaerer id
  
SIDE EFFECTS
  None.

===========================================================================*/

static void ds_eps_bearer_cntxt_set_dl_byte_count
(
  eps_bearer_id_T eps_bearer_id,
  uint32 received_byte_count
)
{
  uint8  bearer_cntx_inst = 0;
  ds_bearer_context_s       *bearer_context_p = NULL;
  ds_eps_bearer_context_s           *eps_bearer_context_p = NULL;

  /*----------------------------------------------------------------------*/

  DS_LTE_MSG1_LOW("ds_eps_bearer_cntxt_set_dl_byte_count for bear %d", eps_bearer_id);
  for(bearer_cntx_inst = 0;\
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;\
      bearer_cntx_inst++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    eps_bearer_context_p = 
    (ds_eps_bearer_context_s *)&eps_bearer_context_tbl[bearer_cntx_inst];

    if(!(ds_eps_bearer_cntxt_validate_bearer_cntxt (eps_bearer_context_p)))
    {
      continue;
    }

    if (eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id == 
        eps_bearer_id) 
    {
       bearer_context_p->ds_bearer_context_dyn_p->total_bytes_received =  
          received_byte_count;
       break;
    }

  }
}

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_TOTAL_NEW_DL_BYTE_COUNT

DESCRIPTION
  This function is used to sum up all the bytes received for particular
  subscription accross all bearer

PARAMETERS
  requested_subs_id - CM Subscription ID for the request

DEPENDENCIES
  None.

RETURN VALUE
  Returns all total number of bytes in all UP eps bearers 
  
SIDE EFFECTS
  None.

===========================================================================*/
uint64 ds_eps_bearer_cntxt_get_total_new_dl_byte_count
(
 sys_modem_as_id_e_type  requested_subs_id
)
{
  uint8  index = 0;
  ds_bearer_context_s  *bearer_context_p = NULL;
  uint64 summation_of_dl_bytes = 0;
  uint32 bearer_old_byte_count = 0;
  uint32 bearer_new_byte_count = 0;
  sys_modem_as_id_e_type  subs_id =  SYS_MODEM_AS_ID_NONE;
  lte_pdcpdl_activity_stats_s lte_pdcpdl_activity_stats;
  boolean lte_pdcpdl_stats_status = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(requested_subs_id))
  {
    return 0;
  }

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    
    if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &subs_id))
    {
      continue;
    }

    if ((requested_subs_id == subs_id) &&
        (ds_eps_bearer_cntxt_validate_bearer_cntxt(&eps_bearer_context_tbl[index]) &&
        (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP))) 
    {

      bearer_old_byte_count = ds_eps_bearer_cntxt_get_dl_byte_count(
                         eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id);
      lte_pdcpdl_stats_status = lte_pdcpdl_get_activity_statistics(eps_bearer_context_tbl[index].
                                         ds_eps_bearer_context_dyn_p->eps_bearer_id,
                                         &lte_pdcpdl_activity_stats,
                                         (uint8)subs_id
                                         );

      if (lte_pdcpdl_stats_status == FALSE) 
      {
         DS_LTE_MSG1_LOW("Could not get dl statistics for eps_bearer_id: %d",
                           eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id);
         continue;
      }
      bearer_new_byte_count = lte_pdcpdl_activity_stats.num_bytes;

      summation_of_dl_bytes += bearer_new_byte_count;
      if (bearer_new_byte_count < bearer_old_byte_count) 
      {
         DS_LTE_MSG1_LOW("new byte count is smaller than old byte count for eps_id : %d"
                           "calculation overflow",
                           eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id);
         summation_of_dl_bytes += (UINT32_MAX - bearer_old_byte_count);

      }
      else
      {
         summation_of_dl_bytes -= bearer_old_byte_count;
      }
      DS_LTE_MSG4_HIGH_EX(subs_id,"bearer_id %d, bearer_old_byte_count is %lu," 
                       "bearer_new_byte_count is %lu, summation_of_dl_bytes is %lu", 
                        eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id, 
                        bearer_old_byte_count, bearer_new_byte_count, 
                        summation_of_dl_bytes);
      
      ds_eps_bearer_cntxt_set_dl_byte_count(
                  eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id,
                  bearer_new_byte_count);        
    }
  }
  return summation_of_dl_bytes;

}/*ds_eps_bearer_cntxt_get_total_new_dl_byte_count*/


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_RESET_TO_CURRENT_DL_BYTE_COUNT

DESCRIPTION
  This function is used to reset all the bytes received for particular 
  subscription accross all bearer with current pdcp values

PARAMETERS
  requested_subs_id - CM Subscription ID for the request

DEPENDENCIES
  None.

RETURN VALUE
  Returns all total number of bytes in all UP eps bearers 
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_reset_to_current_dl_byte_count
(
 sys_modem_as_id_e_type  requested_subs_id
)
{
  uint8  index = 0;
  ds_bearer_context_s  *bearer_context_p = NULL;
  uint32 bearer_new_byte_count = 0;
  sys_modem_as_id_e_type  subs_id =  SYS_MODEM_AS_ID_NONE;
  lte_pdcpdl_activity_stats_s lte_pdcpdl_activity_stats;
  boolean lte_pdcpdl_stats_status = FALSE;
  time_type time_type_now;
  uint64 current_time = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(requested_subs_id))
  {
    return;
  }

  time_get_ms( time_type_now );
  current_time = QW_CVT_Q2N(time_type_now); 
  ds_eps_dl_tput_est_tbl[requested_subs_id].time_since_last_estimatation = current_time;

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p,&subs_id);

    if ((requested_subs_id == subs_id) &&
        (ds_eps_bearer_cntxt_validate_bearer_cntxt(&eps_bearer_context_tbl[index]) &&
        (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP))) 
    {
      lte_pdcpdl_stats_status = lte_pdcpdl_get_activity_statistics(eps_bearer_context_tbl[index].
                                         ds_eps_bearer_context_dyn_p->eps_bearer_id,
                                         &lte_pdcpdl_activity_stats,
                                         (uint8)subs_id
                                         );

      if (lte_pdcpdl_stats_status == FALSE) 
      {
         DS_LTE_MSG1_LOW("Could not get dl statistics for eps_bearer_id: %d",
                           eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id);
         continue;
      }
      bearer_new_byte_count = lte_pdcpdl_activity_stats.num_bytes;

      DS_LTE_MSG2_LOW("bearer_new_byte_count for bear_id %d  is %lu ", 
                eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id, 
                bearer_new_byte_count);

      
      ds_eps_bearer_cntxt_set_dl_byte_count(
                  eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id,
                  bearer_new_byte_count);        
    }
  }

}/*ds_eps_bearer_cntxt_reset_to_current_dl_byte_count*/

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_RESET_DL_BYTE_COUNT

DESCRIPTION
  This function is used to clear bearer dl byte count with current pdcp values

PARAMETERS
  bearer_context_p - bearer context pointer

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_reset_dl_byte_count
(
 ds_bearer_context_s  *bearer_context_p
)
{
  uint8  index = 0;
  ds_bearer_context_s *local_bearer_context_p;
  lte_pdcpdl_activity_stats_s lte_pdcpdl_activity_stats;
  uint32 bearer_new_byte_count = 0;
  boolean lte_pdcpdl_stats_status = FALSE;
  sys_modem_as_id_e_type subs_id =  SYS_MODEM_AS_ID_NONE;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &subs_id))
  {
    return;
  }

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    local_bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    if (local_bearer_context_p != bearer_context_p) 
    {
        continue;
    }

    if (ds_eps_bearer_cntxt_validate_bearer_cntxt(&eps_bearer_context_tbl[index]))
    {
      lte_pdcpdl_stats_status = lte_pdcpdl_get_activity_statistics(eps_bearer_context_tbl[index].
                                         ds_eps_bearer_context_dyn_p->eps_bearer_id,
                                         &lte_pdcpdl_activity_stats,
                                         (uint8)subs_id
                                         );

      if (lte_pdcpdl_stats_status == FALSE) 
      {
         DS_LTE_MSG1_LOW("Could not get dl statistics for eps_bearer_id: %d",
                           eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id);
         continue;
      }
      bearer_new_byte_count = lte_pdcpdl_activity_stats.num_bytes;

      DS_LTE_MSG3_HIGH_EX(subs_id,
                "Reset dl count for bearer context 0x %x "
                "bearer_new_byte_count for bear_id %d  is %lu ",
                bearer_context_p, 
                eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id, 
                bearer_new_byte_count);

      
      ds_eps_bearer_cntxt_set_dl_byte_count(
                  eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p->eps_bearer_id,
                  bearer_new_byte_count);        
      }
  }

}/*ds_eps_bearer_cntxt_reset_dl_byte_count*/


/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_alloc_dyn_mem

DESCRIPTION
  This function allocates dyn memory.
 
PARAMETERS
  index   - index where the memory would be allocated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

=============================================================================*/
boolean ds_eps_bearer_cntxt_alloc_dyn_mem
(
  int index
)
{
  boolean ret_val = FALSE;
  ds_eps_bearer_context_dyn_s *eps_bearer_context_dyn_p = NULL;
  ds_bearer_context_s *bearer_cntxt_p = NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((index < 0) || (index >= DS_3GPP_MAX_BEARER_CONTEXT))
  {
    DS_LTE_MSG0_LOW("Index invalid, return FALSE");
    return ret_val;
  }

  eps_bearer_context_dyn_p = eps_bearer_context_tbl[index].
                                                ds_eps_bearer_context_dyn_p;

  if (eps_bearer_context_dyn_p != NULL)
  {
    bearer_cntxt_p = eps_bearer_context_dyn_p->bearer_context_p;
    if (ds_bearer_cntx_validate_bearer_context(bearer_cntxt_p))
    {
      DS_LTE_MSG2_ERROR("EPS bearer cntxt at index: d is bound to a different bearer cntxt, index: d",
                         index, (uint32)(bearer_cntxt_p->ds_bearer_context_dyn_p->client_data_ptr));
      DS3GPP_ASSERT(0);
      return ret_val;
    }
    else
    {
      DS_LTE_MSG1_HIGH("Freeing the EPS bearer context for re-allocation, index: %d", index);
      modem_mem_free(eps_bearer_context_dyn_p,MODEM_MEM_CLIENT_DATA);
      eps_bearer_context_dyn_p = NULL;
      eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p = NULL;
    }
  }

  if (eps_bearer_context_dyn_p == NULL)
  {
    eps_bearer_context_dyn_p = 
      (ds_eps_bearer_context_dyn_s *)
        modem_mem_alloc(sizeof(ds_eps_bearer_context_dyn_s),MODEM_MEM_CLIENT_DATA);

    if (eps_bearer_context_dyn_p != NULL)
    {
      memset(eps_bearer_context_dyn_p,0,sizeof(ds_eps_bearer_context_dyn_s));
      eps_bearer_context_dyn_p->cookie = EPS_BEARER_CNTXT_VALID_COOKIE;
      eps_bearer_context_dyn_p->index = index;
      eps_bearer_context_dyn_p->is_registered_pdcp = FALSE;
      eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p = 
        eps_bearer_context_dyn_p;

#ifdef FEATURE_LAPP_UFS
      #error code not present
#endif /* FEATURE_LAPP_UFS  */

      ret_val = TRUE;
      DS_LTE_MSG1_MED("EPS Bearer Dynamic Mem allocated at index:%d",index);
    }
    else
    {
      DS_LTE_MSG0_LOW("Heap Failure to allocate EPS bearer dyn mem");
    }
  }

  return ret_val;

}
/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_validate_bearer_cntxt

DESCRIPTION
  This function checks if the eps bearer contextv has been allocated dynamic
  memory

PARAMETERS
  eps_bearer_context_p   -  eps bearer context to be validated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_eps_bearer_cntxt_validate_bearer_cntxt
(
   ds_eps_bearer_context_s *eps_bearer_context_p
)
{
  ds_eps_bearer_context_dyn_s *eps_bearer_context_dyn_p = NULL;
  boolean ret_val = FALSE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/
  if (eps_bearer_context_p == NULL) 
  {
    DS_LTE_MSG0_LOW("Input eps bearer conetxt passed is invalid");
    return ret_val;
  }

  eps_bearer_context_dyn_p = eps_bearer_context_p->ds_eps_bearer_context_dyn_p;

  if ((eps_bearer_context_dyn_p != NULL) &&
    (eps_bearer_context_dyn_p->cookie == EPS_BEARER_CNTXT_VALID_COOKIE))
  {
    DS_LTE_MSG1_LOW("EPS Bearer context has dyn memory allocated at index:%d",
                     eps_bearer_context_dyn_p->index);
    ret_val = TRUE;
  }
  else
  {
    DS_LTE_MSG1_LOW("EPS bearer context:0x%x is INVALID",
                      eps_bearer_context_p);
  }

  return ret_val;
}

/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_free_dyn_mem

DESCRIPTION
  This function frees dyn memory.
 
PARAMETERS
  bearer_cntxt_p - bearer context which fetches the eps bearer context
                   whose memory needs to be freed.
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

=============================================================================*/
boolean ds_eps_bearer_cntxt_free_dyn_mem
(
   ds_bearer_context_s *bearer_context_p
)
{
  ds_eps_bearer_context_dyn_s *eps_bearer_context_dyn_p = NULL;
  ds_eps_bearer_context_s     *eps_bearer_context_p = NULL;
  boolean                      ret_val = FALSE;
  int                          index = -1;
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --*/

/*-------------------------------------------------------------------------- 
  Don't care about the cookie here since we are anyways freeing the memory
  -------------------------------------------------------------------------*/
  if((bearer_context_p == NULL) || 
     (bearer_context_p->ds_bearer_context_dyn_p == NULL ))
  {
    DS_LTE_MSG0_ERROR("Bearer context is not valid");
    return ret_val;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  eps_bearer_context_p = (ds_eps_bearer_context_s *)
    DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);

  if ((eps_bearer_context_p == NULL) || 
      (eps_bearer_context_p->ds_eps_bearer_context_dyn_p == NULL))
  {
    index = (uint32)(bearer_context_p->ds_bearer_context_dyn_p->index);
    
    if ((index < 0) || (index >= DS_3GPP_MAX_BEARER_CONTEXT))
    {
      DS_LTE_MSG1_ERROR_EX(subs_id, "Bearer index invalid: d, return FALSE", index);
      return ret_val;
    }

    eps_bearer_context_dyn_p = eps_bearer_context_tbl[index].ds_eps_bearer_context_dyn_p;

    if (eps_bearer_context_dyn_p == NULL)
    {
      DS_LTE_MSG0_ERROR_EX(subs_id,"EPS bearer context is not valid");
      return ret_val;
    }
  }
  else
  {
    eps_bearer_context_dyn_p = eps_bearer_context_p->ds_eps_bearer_context_dyn_p;
    eps_bearer_context_p->ds_eps_bearer_context_dyn_p = NULL;
  }

  modem_mem_free(eps_bearer_context_dyn_p,MODEM_MEM_CLIENT_DATA);
  return TRUE;
}
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_DL_WM_LEVELS_PER_DL_RATE

DESCRIPTION
  This function returns the statically configured downlink watermark levels
  for a given  downlink data rate level.

PARAMETERS
  dl_data_rate: DL data rate passed by ML1
  eps_wm_levels_p: Out parameter filled with WM levels

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_get_dl_wm_levels_per_dl_rate
(
  uint32                               dl_data_rate,
  ds_eps_bearer_cntxt_wm_levels_type   *eps_wm_levels_p
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( eps_wm_levels_p  != NULL )
  {
   
    if( dl_data_rate <= DSEPS_DL_RATE_150_M )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_LO_150M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_HI_150M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DNE_150M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DNE_QCNT_150M ;
    }
    else if( (dl_data_rate <= DSEPS_DL_RATE_300_M) &&
             (dl_data_rate > DSEPS_DL_RATE_150_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_LO_300M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_HI_300M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DNE_300M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DNE_QCNT_300M ;
    }
    else if( (dl_data_rate <= DSEPS_DL_RATE_450_M) &&
             (dl_data_rate > DSEPS_DL_RATE_300_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_LO_450M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_HI_450M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DNE_450M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DNE_QCNT_450M ;
    }
    else if( (dl_data_rate <= DSEPS_DL_RATE_600_M) &&
             (dl_data_rate > DSEPS_DL_RATE_450_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_LO_600M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_HI_600M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DNE_600M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DNE_QCNT_600M ;
    }
    else if( dl_data_rate > DSEPS_DL_RATE_600_M )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_LO_1000M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_HI_1000M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DNE_1000M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DNE_QCNT_1000M ;
    }
    else
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_DEFAULT_LO ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_DEFAULT_HI ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DEFAULT_DNE ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DEFAULT_DNE_QCNT ;
    }

    /*backword compatability*/
    if(dl_data_rate == 0)
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_DEFAULT_LO ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_DEFAULT_HI ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_DEFAULT_DNE ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_DEFAULT_DNE_QCNT ;
    }

    /*PC Loopback*/
    if (TRUE == ds_3gpp_nv_manager_get_data_shark())
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_DL_WM_PC_LB_LO ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_DL_WM_PC_LB_HI ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_DL_WM_PC_LB_DNE ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_DL_WM_PC_LB_DNE_QCNT ;
    }

    DS_LTE_MSG3_HIGH("DL LOW %d HIGH %d DNE %d",eps_wm_levels_p->lo_wm_level,
                     eps_wm_levels_p->hi_wm_level,
                     eps_wm_levels_p->dne_wm_level);
  }
  return;
} /* ds_eps_bearer_cntxt_get_dl_wm_levels_per_dl_rate */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_UL_WM_LEVELS_PER_UL_RATE

DESCRIPTION
  This function returns the statically configured downlink watermark levels
  for a given  downlink data rate level.

PARAMETERS
  dl_data_rate: DL data rate passed by ML1
  eps_wm_levels_p: Out parameter filled with WM levels

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_get_ul_wm_levels_per_ul_rate
(
  uint32                               ul_data_rate,
  ds_eps_bearer_cntxt_wm_levels_type   *eps_wm_levels_p
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( eps_wm_levels_p  != NULL )
  {
   
    if( ul_data_rate <= DSEPS_UL_RATE_50_M )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_50M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_50M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_50M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_50M ;
    }
    else if( (ul_data_rate <= DSEPS_UL_RATE_75_M) &&
             (ul_data_rate > DSEPS_UL_RATE_50_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_75M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_75M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_75M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_75M ;
    }
    else if( (ul_data_rate <= DSEPS_UL_RATE_100_M) &&
             (ul_data_rate > DSEPS_UL_RATE_75_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_100M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_100M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_100M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_100M ;
    }
    else if( (ul_data_rate <= DSEPS_UL_RATE_150_M) &&
             (ul_data_rate > DSEPS_UL_RATE_100_M) )
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_150M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_150M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_150M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_150M ;
    }
    else
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_150M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_150M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_150M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_150M ;
    }

   /*backword compatability*/
    if( ul_data_rate == 0)
    {
      eps_wm_levels_p->lo_wm_level  = DSLTEPS_UL_WM_LO_150M ;
      eps_wm_levels_p->hi_wm_level  = DSLTEPS_UL_WM_HI_150M ;
      eps_wm_levels_p->dne_wm_level = DSLTEPS_UL_WM_DNE_150M ;
      eps_wm_levels_p->dne_q_cnt    = DSLTEPS_UL_WM_DNE_QCNT_150M ;
    }
    DS_LTE_MSG3_HIGH("UL LOW %d HIGH %d DNE %d",eps_wm_levels_p->lo_wm_level,
                     eps_wm_levels_p->hi_wm_level,
                     eps_wm_levels_p->dne_wm_level);
  }
  return;
} /* ds_eps_bearer_cntxt_get_ul_wm_levels_per_ul_rate */


/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_UE_CAT

DESCRIPTION
  This function returns the LTE UE cateogry for the pass subs
  
PARAMETERS 
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  ue category number
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
uint8 ds_eps_bearer_cntxt_get_ue_cat
(
  sys_modem_as_id_e_type subs_id
)
{
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id = DS3GSUBSMGR_SUBS_ID_INVALID;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return LTE_UE_CLASSES_DEFAULT;
  }

  ds3g_subs_id = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  return ds_eps_bearer_cntxt_ue_cat_id[ds3g_subs_id];
} /* ds_eps_bearer_cntxt_get_ue_cat */

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_EPS_BEARER_CNTXT_FROM_EPS_ID

DESCRIPTION
  This function returns a pointer to the eps bearer context corresponding to
  the eps bearer ID passed to it.
  
PARAMETERS 
  eps_id  - EPS ID for which the eps bearer context needs to be retrieved
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Pointer to the eps bearer context 
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_eps_bearer_context_s* ds_eps_bearer_cntxt_get_eps_bearer_cntxt_from_eps_id
(
  eps_bearer_id_T                      eps_id,
  sys_modem_as_id_e_type               subs_id
)
{
  ds_bearer_context_s               *bearer_context_p = NULL;
  ds_eps_bearer_context_s           *eps_bearer_context = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  do 
  {
    /*----------------------------------------------------------------------
       Get bearer context from eps bearer id
    ----------------------------------------------------------------------*/
    bearer_context_p =
      ds_bearer_cntxt_get_bearer_from_eps_id(eps_id, subs_id);

    if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
    {    
      break;
    }

    /*----------------------------------------------------------------------
      Get eps bearer context information from bearer context 
    ----------------------------------------------------------------------*/
    if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE)
    {
      eps_bearer_context = (ds_eps_bearer_context_s *)
         DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);
    }

    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context))
    {
      break;
    }

  }while (0); 

  return eps_bearer_context;
}/* ds_eps_bearer_cntxt_get_eps_bearer_cntxt_from_eps_id */

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_PDCPUL_IMPLICIT_DEREG_REQ_MSG_HDLR

DESCRIPTION
  This is the message handler function that will be called after L2 moves data
  back into watermark and deregisters the watermark. As part of this message
  handler DS will make a note of all the EPS IDs for which WMs are deregistered
  and sends a confirmation to L2.
  
PARAMETERS 
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE   -- If handling is successful
  FALSE  -- Otherwise
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_bearer_cntxt_pdcpul_implicit_dereg_req_msg_hdlr
(  
  msgr_umid_type                                 msgrtype,
  sys_modem_as_id_e_type                         subs_id,
  const msgr_hdr_struct_type                    *dsmsg_ptr
)
{
  ds_bearer_context_s                          *bearer_context_p = NULL;
  eps_bearer_id_T                               eps_bearer_id = 0;
  ds_3gppi_local_err_val_e_type                 local_err = 
                                                  DS_3GPPI_LOCAL_ERR_VAL_NONE;
  lte_pdcpul_implicit_deregister_wm_req_msg_s   *dereg_wm_info_ptr = NULL;
  lte_pdcpul_implicit_deregister_wm_cnf_msg_s    dereg_wm_cnf_info;
  uint16                                         base_bmask = 0x1;
  uint8                                          index;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do 
  {
    if ((dsmsg_ptr == NULL) || 
        (msgrtype != LTE_PDCPUL_IMPLICIT_DEREG_REQ))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;      
    }
    
    dereg_wm_info_ptr = 
       (lte_pdcpul_implicit_deregister_wm_req_msg_s *)dsmsg_ptr;

    DS_LTE_MSG1_HIGH("Implicit Dereg called. EPS ID bmask: %d", 
                     dereg_wm_info_ptr->eps_bmsk);

    /*------------------------------------------------------------------------
      Loop through all the bearers specified in the bmask and mark that bearer
      as deregistered with PDCP
    -------------------------------------------------------------------------*/
    for (index = 0; index < 16; index++)
    {
      if (dereg_wm_info_ptr->eps_bmsk & (base_bmask << index))
      {

        /*---------------------------------------------------------------------
           bit 0  - eps_id = 1
           bit 1  - eps_id = 2
           bit 2  - eps_id = 3
           ...
           bit 15 - eps_id = 16 
        ---------------------------------------------------------------------*/          
        eps_bearer_id = index + 1;

        bearer_context_p = 
           ds_bearer_cntxt_get_bearer_from_eps_id(eps_bearer_id, subs_id);

        if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
        {
          continue;
        }

        bearer_context_p->ds_bearer_context_dyn_p->ul_registered_with_ltepdcp
            = FALSE;
      }
    }/* for */

    /*-----------------------------------------------------------------------
      Send confirmation back to PDCP 
    -------------------------------------------------------------------------*/ 
    memset((void *)&dereg_wm_cnf_info, 
           0, 
           sizeof(lte_pdcpul_implicit_deregister_wm_cnf_msg_s));

    dereg_wm_cnf_info.eps_bmsk = dereg_wm_info_ptr->eps_bmsk;

    if(ds3gmsgr_msg_send_ext(LTE_PDCPUL_IMPLICIT_DEREG_CNF, 
                             MSGR_DS_MSGRRECV,
                             (msgr_hdr_struct_type*)(&dereg_wm_cnf_info),
                             sizeof(lte_pdcpul_implicit_deregister_wm_cnf_msg_s),
                             subs_id))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    } 
         
  }while (0); 

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1 ("Implicit Dereg msg hdlr, local err: d", local_err);    
  }

  return TRUE; 

} /* ds_eps_bearer_cntxt_pdcpul_implicit_dereg_req_msg_hdlr */
#endif /*  FEATURE_LAPP */

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /* FEATURE_LAPP_UFS */

/*===========================================================================
FUNCTION     DS_EPS_BEARER_CNTXT_MAP_ESM_TO_PS_DOWN_REASON
   
DESCRIPTION
  This util function maps ESM cause code to PS net down reason
  
PARAMETERS 
  ps_net_down_output_ptr  -- pointer to be filled with PS net down reason
  esm_cause               -- ESM cause code
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  PS net down reason
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_map_esm_to_ps_down_reason
(
  ps_iface_net_down_reason_type  *ps_net_down_output_ptr,
  esm_cause_enum_T                esm_cause
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if (NULL == ps_net_down_output_ptr)
  {
    return;
  }

  switch(esm_cause)
  {
    case NAS_ESM_OPTR_DETERMINED_BARRING:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_OPERATOR_DETERMINED_BARRING;
      break;
    case NAS_ESM_INSUFFICIENT_RESOURCES:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_INSUFFICIENT_RESOURCES;
      break;
    case NAS_ESM_UNKNOWN_MISSING_APN:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNKNOWN_APN;
      break;
    case NAS_ESM_UNKNOWN_PDN_TYPE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNKNOWN_PDP;
      break;
    case NAS_ESM_USER_AUTHENTICATION_FAILED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_AUTH_FAILED;
      break;
    case NAS_ESM_OPN_REJ_SERVICING_GW_OR_PDN_GW:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_GGSN_REJECT;
      break;
    case NAS_ESM_ACTIVATION_REJECTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ACTIVATION_REJECT;
      break;
    case NAS_ESM_SERV_OPT_NOT_SUPPORTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_OPTION_NOT_SUPPORTED;
      break;
    case NAS_ESM_REQ_SERV_OPT_NOT_SPECIFIED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_OPTION_UNSUBSCRIBED;
      break;
    case NAS_ESM_SERV_OPT_TEMP_OUT_OF_ORDER:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_OPTION_TEMP_OOO;
      break;
    case NAS_ESM_PTI_ALREADY_IN_USE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NSAPI_ALREADY_USED;
      break;
    case NAS_ESM_REGULAR_DEACTIVATION:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_REGULAR_DEACTIVATION;
      break; 
    case NAS_ESM_EPS_QOS_NOT_ACCEPTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_QOS_NOT_ACCEPTED;
      break; 
    case NAS_ESM_NETWORK_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NETWORK_FAILURE;
      break;
    case NAS_ESM_REACTIVATION_REQUIRED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UMTS_REATTACH_REQ;
      break;
    case NAS_ESM_FEATURE_NOT_SUPPORTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_FEATURE_NOT_SUPPORTED;
      break; 
    case NAS_ESM_SEMANTIC_ERR_IN_TFT_OPERATION:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_TFT_SEMANTIC_ERROR;
      break;
    case NAS_ESM_SYNTACTICAL_ERR_IN_TFT_OPERATION:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_TFT_SYNTAX_ERROR;
      break;
    case NAS_ESM_UNKNOWN_EPS_BEARER_CONTEXT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNKNOWN_PDP_CONTEXT;
      break;
    case NAS_ESM_SYMANTIC_ERR_IN_PKT_FILTER:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_FILTER_SEMANTIC_ERROR;
      break;
    case NAS_ESM_SYNTACTICAL_ERR_IN_PKT_FILTER:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_FILTER_SYNTAX_ERROR;
      break;
    case NAS_ESM_EPS_BEARER_CONT_WO_TFT_ALREADY_ACT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PDP_WITHOUT_ACTIVE_TFT;
      break; 
    case NAS_ESM_PTI_MISMATCH:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PTI_MISMATCH;
      break;
    case NAS_ESM_LAST_PDN_DISCONNECTED_NOT_ALLOWED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LAST_PDN_DISCONN_NOT_ALLOWED;
      break;    
    case NAS_ESM_PDN_TYPE_IPV4_ONLY_ALLOWED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IP_V4_ONLY_ALLOWED;
      break;
    case NAS_ESM_PDN_TYPE_IPV6_ONLY_ALLOWED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IP_V6_ONLY_ALLOWED;
      break;
    case NAS_ESM_SINGLE_ADR_BEARERS_ONLY_ALLOWED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_SINGLE_ADDR_BEARER_ONLY;
      break;
    case NAS_ESM_MAX_PDP_CONTEXT_REACHED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MAX_ACTIVE_PDP_CONTEXT_REACHED;
      break;
    case NAS_ESM_APN_NOT_SUPPORTED_IN_PLMN_RAT_COMBINATION:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNSUPPORTED_APN_IN_CURRENT_PLMN;
      break;       
    case NAS_ESM_ESM_INFO_NOT_RECEIVED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_INFO_NOT_RECEIVED;
      break;
    case NAS_ESM_PDN_CONN_DOES_NOT_EXIST:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PDN_CONN_DOES_NOT_EXIST;
      break;
    case NAS_ESM_MULTIPLE_PDN_CONN_FOR_GIVEN_APN_NOT_ALLOWED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MULTI_CONN_TO_SAME_PDN_NOT_ALLOWED;
      break;
    case NAS_ESM_COLLISION_WITH_NW_INIT_REQ:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_COLLISION_WITH_NW_INIT_REQ;
      break;
    case NAS_ESM_UNSUPPORTED_QCI_VALUE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNSUPPORTED_QCI_VALUE;
      break;     
    case NAS_ESM_BEARER_HANDLING_NOT_SUPPORTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_BEARER_HANDLING_NOT_SUPPORTED;
      break;      
    case NAS_ESM_INVALID_PTI_VALUE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_INVALID_TRANSACTION_ID;
      break;
    case NAS_ESM_SYMANTICALLY_INVALID_MSG:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MESSAGE_INCORRECT_SEMANTIC;
      break;
    case NAS_ESM_INVALID_MANDATORY_INFORMATION:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_INVALID_MANDATORY_INFO;
      break;
    case NAS_ESM_MSG_TYPE_NON_EXISTANT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MESSAGE_TYPE_UNSUPPORTED;
      break;
    case NAS_ESM_MSG_TYPE_INCOMPATIBLE_PROT_STATE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MSG_TYPE_NONCOMPATIBLE_STATE;
      break;
    case NAS_ESM_INFO_ELEMENT_NON_EXISTANT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_UNKNOWN_INFO_ELEMENT;
      break;
    case NAS_ESM_CONDITIONAL_IE_ERR:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_CONDITIONAL_IE_ERROR;
      break;
    case NAS_ESM_MSG_INCOMPATIBLE_WITH_PRO_STATE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE;
      break;
    case NAS_ESM_PROTOCOL_ERROR:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PROTOCOL_ERROR;
      break;
    case NAS_ESM_APN_RESTRICT_VAL_INCOMPAT_WITH_ACT_CONTEXT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_APN_TYPE_CONFLICT;
      break;
    default:
      DS_LTE_MSG0_ERROR("EVENT_CAUSE_CODES: Unable to map DS Int to PS_INDR, Unknown code!");
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
      break;
  }/* End of switch */
  
}/* ds_eps_bearer_cntxt_map_esm_to_ps_down_reason */

/*===========================================================================
FUNCTION     DS_EPS_BEARER_CNTXT_MAP_ESM_LOCAL_TO_PS_DOWN_REASON
   
DESCRIPTION
  This util function maps ESM local cause code to PS net down reason
  
PARAMETERS 
  ps_net_down_output_ptr  -- pointer to be filled with PS net down reason
  esm_local_cause         -- ESM local cause code
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  PS net down reason
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_map_esm_local_to_ps_down_reason
(
  ps_iface_net_down_reason_type    *ps_net_down_output_ptr,
  lte_nas_local_cause_enum_T        esm_local_cause
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == ps_net_down_output_ptr)
  {
    return;
  }

  switch(esm_local_cause)
  {
    case LTE_NAS_ILLEGAL_UE_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ILLEGAL_MS;
      break;

    case LTE_NAS_ILLEGAL_ME_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ILLEGAL_ME;
      break;

    case LTE_NAS_EPS_SERVICES_NOT_ALLOWED_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_DETACH_EPS_SERVICES_NOT_ALLOWED;
      break;

    case LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK;
      break;

    case LTE_NAS_IMPLICITLY_DETACHED_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_IMPLICITLY_DETACHED;
      break;

    case LTE_NAS_PLMN_NOT_ALLOWED_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_PLMN_NOT_ALLOWED;
      break;

    case LTE_NAS_TRACKING_AREA_NOT_ALLOWED_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_TRACKING_AREA_NOT_ALLOWED;
      break;

    case LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA;
      break;

    case LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
      break;

    case LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG_LOCAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_AUTHORIZED_FOR_THIS_CSG;
      break;

    case LTE_NAS_ESM_UNKNOWN_EPS_BEARER_CONTEXT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_UNKNOWN_EPS_BEARER_CONTEXT;
      break;

    case DRB_RELEASED_AT_RRC:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_DRB_RELEASED_AT_RRC;
      break;

    case NAS_SIG_CONN_RELEASED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NAS_SIG_CONN_RELEASED;
      break;

    case EMM_DETACHED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_DETACHED;
      break;

    case EMM_ATTACH_FAILED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_ATTACH_FAILED;
      break;

    case EMM_ATTACH_STARTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_ATTACH_STARTED;
      break;

    case LTE_NAS_SERVICE_REQ_FAILED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED;
      break;

    case ESM_ACTIVE_DEDICATED_BEARER_REACTIVATED_BY_NW:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_ACTIVE_DEDICATED_BEARER_REACTIVATED_BY_NW;
      break;

    case ESM_LOWER_LAYER_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_LOWER_LAYER_FAILURE;
      break;

    case ESM_SYNC_UP_WITH_NW:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_SYNC_UP_WITH_NW;
      break;

    case ESM_NW_ACTIVATED_DED_BEARER_WITH_ID_OF_DEF_BEARER:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_NW_ACTIVATED_DED_BEARER_WITH_ID_OF_DEF_BEARER;
      break;

    case ESM_BAD_OTA_MESSAGE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_BAD_OTA_MESSAGE;
      break;

    case ESM_DS_REJECTED_THE_CALL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_DS_REJECTED_THE_CALL;
      break;

    case ESM_CONTEXT_TRANSFERED_DUE_TO_IRAT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_CONTEXT_TRANSFERED_DUE_TO_IRAT;
      break;

    case DS_EXPLICIT_DEACT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_DS_EXPLICIT_DEACT;
      break;

    case ESM_LOCAL_CUASE_NONE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ESM_LOCAL_CAUSE_NONE;
      break;

    case ESM_LOCAL_CC_REJECT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_CALL_CONTROL_REJECTED;
      break;

    case LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE;
      break;

    case ACL_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_ACL_FAILURE;
      break;

    case LTE_NAS_SERVICE_REQ_FAILED_DS_DISALLOW:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LTE_NAS_SERVICE_REQ_FAILED_DS_DISALLOW;
      break;

    case EMM_T3417_EXPIRED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_T3417_EXPIRED;
      break;

    case EMM_T3417_EXT_EXPIRED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_EMM_T3417_EXT_EXPIRED;
      break;

    case LTE_NAS_LRRC_UL_DATA_CNF_FAILURE_TXN:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_TXN;
      break;

    case LTE_NAS_LRRC_UL_DATA_CNF_FAILURE_HO:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_HO;
      break;

    case LTE_NAS_LRRC_UL_DATA_CNF_FAILURE_CONN_REL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CONN_REL;
      break;

    case LTE_NAS_LRRC_UL_DATA_CNF_FAILURE_RLF:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_RLF;
      break;

    case LTE_NAS_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_ABORTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_ABORTED;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_ACCESS_BARRED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_ACCESS_BARRED;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_CELL_RESEL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CELL_RESEL;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_LINK_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_LINK_FAILURE;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_NOT_CAMPED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_NOT_CAMPED;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_SI_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_SI_FAILURE;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_CONN_REJECT:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_EST_FAILURE_CONN_REJECT;
      break;

    case LTE_NAS_LRRC_CONN_REL_NORMAL:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_NORMAL;
      break;

    case LTE_NAS_LRRC_CONN_REL_RLF:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_RLF;
      break;

    case LTE_NAS_LRRC_CONN_REL_CRE_FAILURE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_CRE_FAILURE;
      break;

    case LTE_NAS_LRRC_CONN_REL_OOS_DURING_CRE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_OOS_DURING_CRE;
      break;

    case LTE_NAS_LRRC_CONN_REL_ABORTED:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_ABORTED;
      break;

    case LTE_NAS_LRRC_CONN_REL_SIB_READ_ERROR:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_LRRC_CONN_REL_SIB_READ_ERROR;
      break;

    case LTE_NAS_DETACH_WITH_REATTACH_LTE_NW_DETACH:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_DETACH_WITH_REATTACH_LTE_NW_DETACH;
      break;

    case LTE_NAS_DETACH_WITH_OUT_REATTACH_LTE_NW_DETACH:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_DETACH_WITH_OUT_REATTACH_LTE_NW_DETACH;
      break;

    case LTE_NAS_LRRC_CONN_EST_FAILURE_RF_UNAVAILABLE:
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_RRC_CONN_EST_FAILURE_RF_UNAVAILABLE;
      break;

    case ESM_PROC_TIME_OUT:
      *ps_net_down_output_ptr =  PS_NET_DOWN_REASON_ESM_PROC_TIME_OUT;
      break;
    
  
    case ESM_LOCAL_DEACT:
      *ps_net_down_output_ptr =  PS_NET_DOWN_REASON_CLIENT_END;
      break;

    
    default:
      DS_LTE_MSG0_ERROR("EVENT_CAUSE_CODES: Unable to map DS Int to PS_INDR, Unknown code!");
      *ps_net_down_output_ptr = PS_NET_DOWN_REASON_NOT_SPECIFIED;
      break;

  }/* End of switch*/

}/* */
#endif /* FEATURE_DATA_LTE */
/*===========================================================================

                                UNIT TEST

===========================================================================*/
