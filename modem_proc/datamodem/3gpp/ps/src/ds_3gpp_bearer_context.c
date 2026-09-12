/*!
  @file
  ds_bearer_context.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2008-2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the expressf
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/src/ds_3gpp_bearer_context.c#7 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/13   ss      Cleanup bearers in PEND_REDIAL state upon sys_chg as 
                   lower layers wouldn't send cntxt_transfer when registration
                   is not successful on a RAT.
11/20/12   ss      Fixed the issue of BP removal NV read failure.
11/27/12   ss      Clearing service_req throttle flag when RAB_REESTAB_IND
                   is received from N/W (unsolicited).
08/13/12   ss      Moving efs write operation to dsutil performed during 
                   bootup.
08/10/12   ss      Delegating RAT specific handling during call_end.
08/08/12   ss      Optimization to trigger Dynamic BP only when threshold
                   num of acks are received.
06/19/12   nd      MT PDP pkt call.
07/04/12   ss      Freeing the BP timer handle in deact_cnf and avoiding
                   crash due to using stale timer handle.
06/07/12   ss      Enabled Dynamic BP by default and set the default tmr_val
                   to 2 seconds.
05/29/12   ss      Fix to not post multiple mem_ctrl_ev cmds to DS task.
05/24/12   kv      Avoid Rab reestab for GSM when we reorginate call from 
                   dormancy.
03/27/12   ss      Fixed issue of RAB_REESTAB_IND received before resume ind
                   resulting in error in data path setup.
03/22/12   ss      Addition of RAB_FAILURE and ESM local cause codes.
12/21/11   ss      Added support for watermark tuning.
11/17/11   sa      Added support for V6 Loopback Mode B.
10/21/11   nd      Fixed compilation warnings.
10/05/11   ttv     Added DSDS changes for new mode handler.
09/09/11   rr      Added support for fallback from Dual IP to single IP when nw rejects 
                   with specific cause codes in UMTS mode
04/07/11   ttv     Addec changes for IOCTL PS_IFACE_IOCTL_UMTS_GET_IM_CN_FLAG.
03/24/11   sa      Added MT PDP Auto answer support.
03/24/11   hs      Changed UL flow control to kick in from WM call back func.
03/17/11   sa      Added support for bearer tech change event indication.
03/02/11   ss      Added support for configurable RAB_REESTAB RETRY.
02/23/11   ttv     Added support for watermark tracing.
02/04/11   ss      Fixed an issue of data transfer not resuming on 2nd bearer
                   after RL failure and recovery.
05/17/10   hs      Added APN disable and PDN inactivity feature support.
04/03/10   vk      Added support for MSG2.0 based logging
04/02/10   vrk     UMTS interRAT support
01/23/10   vk      Added helper macros for virtual function dispatch
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "customer.h" 
#include "comdef.h"


#define FEATURE_DSM_WM_CB
#ifdef FEATURE_DATA_3GPP
#include "ds_3gpp_bearer_context.h"
#include "ds_eps_bearer_context_ex.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_pdn_context_ex.h"
#include "ds_3gpp_nv_manager.h"
#include "ds_3gpp_apn_table.h"
#include "ds_3gpp_hdlr.h"
#include "dstask_v.h"
#include "ds3gmgr.h"
#include "ds3gmshif.h"
#include "ds3gflowmgr.h"
#include "ds3gcfgmgr.h"
#include "ds_3gpp_dyn_mem_hdlr.h"
#include "ds_loopback_hdlr.h"
#ifdef FEATURE_DATA_EMBMS
#include "ds_3gpp_embms_hdlr.h"
#endif /*FEATURE_DATA_EMBMS*/
#ifdef FEATURE_DATA_WCDMA_PS
#include "ds_wcdma_bearer_context.h"
#endif
#include "ds_flow_control.h"
#include "ran.h"
#include "ps_utils.h"
#include "ds_3gpp_flow_context.h"
#include "ds_3gpp_bearer_flow_manager.h"
#include "ds_3gpp_flow_manager.h"
#include "ds_3gpp_msg.h"
#include "ds_dsd_apm_ext_i.h"
#include "ps_dpm.h"
#include "ps_sys.h"
#include "ds3gsubsmgr.h"
#include "ds_3gpp_loopback_hdlr.h"


#ifdef FEATURE_DATA_LTE
#include "ds_eps_bearer_context.h"
#include "ds_eps_pdn_context.h"
#include "ds_eps_throughput_hdlr.h"
#include "ds_eps_tlb.h"
#include "lte_rrc_ext_msg.h"
#endif /* FEATURE_DATA_LTE */

#ifdef FEATURE_DATA_WCDMA_PS
#include "ds_umts_bearer_context.h"  
#endif

#ifdef FEATURE_DATA_TE_MT_PDP
#include "dsumtspsmthdlr.h"
#endif /* FEATURE_DATA_TE_MT_PDP */
#include "ds_3gpp_throttle_sm.h"


#include "event_defs.h"
#include "ps_wmk_logging.h"
#include "ps_crit_sect.h"
#include "ps_iface_defs.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds_3gpp_network_override_hdlr.h"
#include "pstimer.h"
#include "ps_sys_conf.h"
#include "ps_handle_mgr.h"
#include "emm_irat_if_msg.h"
#include "ds_3gpp_srv_req_throt_sm.h"
#include "ds_3gpp_rab_reestab_sm.h"
#include "mcfg_nv_api.h"
#include "ds_3gpp_logging_util.h"
#include "ps_phys_link.h"
#include "ds3gutil.h"
#include "ds_3gpp_bearer_context_ex.h"
#include "ds_3gpp_pdn_context_ex.h"
#include "ps_stat_iface.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
#include "ds_3gpp_link_switch_mgr.h"

#include "ds_3gpp_flow_control.h"


#ifdef FEATURE_DATA_FLOW_MGMT
#include "ds_3gpp_burst_flow_hdlr.h"
#endif /*FEATURE_DATA_FLOW_MGMT */

#include "ps_sys_event.h"
#include "ds_3gppi_utils.h"
#include "ds3gdsdif.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define DS_BEARER_CTXT_VFR_CALL(rval, func, subs_id, ...)  \
          DS_3GPPI_VFR_CALL(ds_bearer_context_ftbl, rval, func, subs_id, \
                            __VA_ARGS__)

#define DS_BEARER_CTXT_VFR_CALL_NO_ARG(rval, func, subs_id)  \
          DS_3GPPI_VFR_CALL_NO_ARG(ds_bearer_context_ftbl, rval, func, subs_id)

#define DS_BEARER_CTXT_VF_CALL_NO_ARG(func, subs_id)  \
          DS_3GPPI_VF_CALL_NO_ARG(ds_bearer_context_ftbl, func, subs_id)

#define DS_BEARER_CTXT_VFR_CALL_PER_BEARER(rval, func, bearer_context_p, ...)  \
            DS_3GPPI_VFR_CALL_PER_BEARER(ds_bearer_context_ftbl, rval, func, bearer_context_p, __VA_ARGS__)

#define DS_BEARER_CTXT_VF_CALL_PER_BEARER(func, bearer_context_p, ...)  \
            DS_3GPPI_VF_CALL_PER_BEARER(ds_bearer_context_ftbl, func, bearer_context_p, __VA_ARGS__)

#define DS_BEARER_CTXT_VFR_CALL_NO_ARG_PER_BEARER(rval, func, bearer_context_p )  \
            DS_3GPPI_VFR_CALL_NO_ARG_PER_BEARER(ds_bearer_context_ftbl, rval, func, bearer_context_p)

#define DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER(func, bearer_context_p )  \
            DS_3GPPI_VF_CALL_NO_ARG_PER_BEARER(ds_bearer_context_ftbl, func, bearer_context_p)

/*  Default timer values for fast dormancy timers in Milli Seconds 
  if EFS Reading fails */
#define DS_3GPP_FD_DEFAULT_OVERALL_TIMER_VALUE   6000 

#define DS_3GPP_FD_DEFAULT_AGRESSIVE_TIMER_VALUE 1000

/*===========================================================================
   This Structure is used to maintain all global variables of
   ds_3gpp_bearer_context.
   PLEASE USE THIS STRUCTURE WHENEVER YOU WANT TO ADD A NEW GLOBAL VARIABLE.
===========================================================================*/
typedef struct
{
  ds_bearer_cntxt_per_subs_info_type *per_subs_info_p[DS3GSUBSMGR_SUBS_ID_MAX];

  /*----------------------------------------------------------------------------
  Bearer context table. This table contains all the bearers. It is this table
  that is updated whenever there is a change in a bearer context.

  Out of the DS_3GPP_MAX_BEARER_CONTEXT declared here, 
  DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT bearers are tied to one PDN cntx.
  Out of these bearers tied to the same PDN context, one is default and the 
  others are treated to be secondary calls.
  ----------------------------------------------------------------------------*/
  ds_bearer_context_s           bearer_context_tbl[DS_3GPP_MAX_BEARER_CONTEXT];
   
  /*-----------------------------------------------------------------------------
    This flag is used to keep track of mem_ctrl_ev posted to DS task. This is to 
    avoid posting multiple commands to DS task when a mem_ctrl_ev is already 
    pending.
   -----------------------------------------------------------------------------*/
  boolean                       mem_ctrl_ev_posted;
} ds_bearer_cntxt_info_type;

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/

extern ds_pdn_context_s ds_pdn_context_tbl[DS_3GPP_MAX_PDN_CONTEXT];

/*----------------------------------------------------------------------------
  EPS/umts specific function tables. We allocate memory for SYS_SYS_MODE_MAX
  table. However, we will be using only those corresponding to WCDMA, GPRS
  and EPS.
----------------------------------------------------------------------------*/
ds_bearer_context_ftbl_s ds_bearer_context_ftbl[DS_3GPPI_SYS_MODE_MAX] = {{NULL}};

/* structure to store fast dormancy relatedinformation   */
static ds_3gpp_fd_info_s      ds_3gpp_fd_info;



/*-----------------------------------------------------------------------------
  This table has pointers to the bearer up and down function call backs. 
  The declaration of this call back table and populating the same with the 
  appropriate function pointers happends in PDN context. These call back fns
  are also implemented in the PDN context. 
-----------------------------------------------------------------------------*/
extern bearer_context_notifier_cb_tbl_s bearer_context_notifier_cb_tbl;

/*===========================================================================
   This Structure is used to maintain all global variables of
   ds_3gpp_bearer_context.c
   PLEASE USE THIS STRUCTURE WHENEVER YOU WANT TO ADD A NEW GLOBAL VARIABLE.
===========================================================================*/
static ds_bearer_cntxt_info_type ds_bearer_cntxt_info = {{NULL}};

boolean access_barring_rab_retry = FALSE;
/*===========================================================================
  Cookie to ensure that dynamic memory allocated is valid
===========================================================================*/
#define DS_BEARER_CNTXT_PER_SUBS_INFO_COOKIE 0xdefdefde

static uint16  access_barring_rab_retry_timer = 
               DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME;

static uint16  access_barring_rab_retry_counter = 
               DS_BEARER_CNTXT_ACCESS_BARR_RAB_RETRY_COUNTER;

  /* Critical section for UL MH datapath*/
 rex_crit_sect_type   ds_3gpp_ul_crit_sect;

/* Critical section for DL MH Datapath */
 rex_crit_sect_type   ds_3gpp_dl_crit_sect;



/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_SET_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function sets the handle to the Bearer Context Information
  corresponding to the given Subscription.

PARAMETERS
  subs_id          : Subscription Id
  hndl             : Handle to the Bearer Context Information for
                     the given Subscription.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ds_bearer_cntxt_set_per_subs_info_hndl
(
  sys_modem_as_id_e_type               subs_id,
  ds_bearer_cntxt_per_subs_info_type  *hndl
);

#ifdef FEATURE_DATA_LTE

/*===========================================================================
FUNCTION ds_bearer_cntxt_lte_deactivate_req_hdlr

DESCRIPTION
  This function listens to LTE Deactivated Request from NAS . 
  DS use this as an indication of LTE de-activation and clears all pending  redial calls.

  
PARAMETERS  
  msgrtype - the id of the received message 
  dsmsg    - pointer to the received message 

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_lte_deactivate_req_hdlr
(
  msgr_umid_type               msgrtype,
  sys_modem_as_id_e_type       subs_id,
  const msgr_hdr_struct_type * dsmsg
);
#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION ds_bearer_cntxt_post_clr_pending_calls_cmd

DESCRIPTION
  This function posts a command to DS Task to clear pending calls if any.
  
PARAMETERS  
  subs_id: Subscription Id

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_post_clr_pending_calls_cmd
(
  sys_modem_as_id_e_type subs_id
);

 /*=============================================================================
FUNCTION DS_BEARER_CNTXT_DPM_EV_CB

DESCRIPTION
  This is the call back function that is called when the Dpm events are invoked.
  Based on these events; the watermark's nonemmpty function callback is set
  reset.
PARAMETERS
  IN
    dpm_event       - The event that happened. This is used to differentiate
                      the events that happened : registration, deregistration
                      reregistration
    event_info_ptr   - Passed in as void*, this gives us the event details 
            
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void ds_bearer_cntxt_dpm_ev_cb
(
  ps_dpm_um_ev_enum_type             dpm_event, 
  void                              *event_info_ptr
);


/*===========================================================================

FUNCTION DS_BEARER_CNTXT_FLUSH_PS_L2_WATERMARKS

DESCRIPTION
  This function flushes PS-L2 watermarks

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntxt_flush_ps_l2_watermarks
(
  void
);

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntx_alloc_dyn_mem

DESCRIPTION
  This function allocates dyn memory.
  
PARAMETERS  
  index   - index where the memory would be allocated
  subs_id - subscription id

DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

=============================================================================*/
static boolean ds_bearer_cntx_alloc_dyn_mem
(
  int                    index,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag

DESCRIPTION
  This function gets the Mem Ctrl Event posted Flag
 
PARAMETERS
  None
    
DEPENDENCIES
  None

RETURN VALUE
  TRUE - If Mem Cntrl Event was already posted
  FALSE- Otherwise
   
SIDE EFFECTS 
  None
  
=============================================================================*/
static boolean ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag
(
  void
);

/*===========================================================================
FUNCTION ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag

DESCRIPTION
  This function sets the Mem Ctrl Event posted Flag
 
PARAMETERS
  flag: Mem Cntrl Event posted Flag
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag
(
  boolean flag
);

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_OK_TO_RELEASE_CONNECTION

DESCRIPTION
  This function checks if it is OK to Relese signalling connection with NW 

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_ok_to_release_connection
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_IS_ANY_BEARER_UP

DESCRIPTION
  This function checks if any bearer is in UP state or not

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_any_bearer_up
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  ds_3gpp_bearer_cntxt_init_fast_domrnacy_params

DESCRIPTION
  This function will initialize the fast dormancy paramters. 
  Enabled flag and Timer values. If GCF NV is enabled and FD 
  is running few GCF test cases will fail

PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_init_fast_domrnacy_params
(
  void
);

#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_RESEND_CALL_END

DESCRIPTION
  This function sends pdn_disconnect as CM returned error for CALL_CMD_END
  with cause CM_CALL_CMD_ERR_MODE_INFO_CHG_LTE.

PARAMETERS 
  call_id - call_id for which CALL_END error is returned.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ds_bearer_cntxt_resend_call_end
(
  byte  call_id
);
#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_LTE_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date  date wm level value for LTE

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  um_wm_threshold_p - pointer to WM threshold NV structure
  subs_id       - subscription ID
  wm_type     - WM level index (LO,HI,DNE)

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_lte_ul_wm_level
(
  ds_bearer_context_s                          *bearer_cntxt_p,
  ds_bearer_cntxt_um_wm_threshold_struct_type  *um_wm_threshold_p,
  uint8                                         wm_level,
  sys_modem_as_id_e_type                        subs_id
);
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_WCDMA_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date wm level value for WCDMA

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  um_wm_threshold_p - pointer to WM threshold NV structure
   wm_type - WM level index

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_wcdma_ul_wm_level
(
  ds_bearer_context_s                          *bearer_cntxt_p,
  ds_bearer_cntxt_um_wm_threshold_struct_type  *um_wm_threshold_p,
  uint8                                         wm_level
);

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_TDSCDMA_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date wm level value for TDSCDMA

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  tds_um_wm_threshold_p - pointer to WM threshold NV structure
  wm_type - WM level

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_tdscdma_ul_wm_level
(
  ds_bearer_context_s                              *bearer_cntxt_p, 
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type  *tds_wm_threshold_p,
  uint8                                             wm_level
);


/*===========================================================================

                                FUNCTIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_bearer_cntxt_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_bearer_cntxt_qsh_mdump_collect()
{
  qsh_mdump_collect_high(&ds_3gpp_fd_info, sizeof(ds_3gpp_fd_info_s));
  qsh_mdump_collect_high(&ds_bearer_cntxt_info, sizeof(ds_bearer_cntxt_info_type));
}
#endif /* FEATURE_QSH_MDUMP */


/*===========================================================================
FUNCTION ds_bearer_cntxt_setup_sw_path

DESCRIPTION
  This function sets up the SW path instead of HW in case of DPM registration
  failure or Memory allocation failure while DPM registration
  
PARAMETERS
  bearer_context_p       - bearer_context to be registered

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntxt_setup_sw_path
(
   ds_bearer_context_s                  *bearer_context_ptr
)
{
  dsm_watermark_type                    *wm_ptr = NULL;
/*-------------------------------------------------------------------------*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
  {
    return;
  }

  switch (bearer_context_ptr->ds_bearer_context_dyn_p->call_mode)
  {
    case SYS_SYS_MODE_LTE:
      wm_ptr = &(bearer_context_ptr->lte_rx_wm.wm_item);
      break;

    case SYS_SYS_MODE_WCDMA:
    case SYS_SYS_MODE_GSM:
    case SYS_SYS_MODE_TDS:
      wm_ptr = &(bearer_context_ptr->rx_wm.wm_item);
      break;

    default:
      DS_3GPP_ERROR_LOG_0_EX(bearer_context_ptr->ds_bearer_context_dyn_p->subs_id,
                             "Invalid 3GPP RAT, SW path not setup");
      break;
  }

  if(wm_ptr != NULL)
  {
    wm_ptr->non_empty_func_ptr = 
      (wm_cb_type)ds_3gpp_bearer_cntxt_rx_data_at_u_pdp_cb;
    wm_ptr->non_empty_func_data = NULL;
  }
}

/*===========================================================================
FUNCTION   ds_bearer_cntxt_dpm_setup

DESCRIPTION
  This function is used to setup registration and re-registration info with
  DPM.
  
PARAMETERS  
  bearer_context_p:                 Bearer context
  info:                             Reg or Re-reg info to be setup

DEPENDENCIES 
  The caller of this local function must pass valid info ptr.
  
RETURN VALUE 
  TRUE:           If Successful
  FALSE:          On Failure
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_dpm_setup
(
  ds_bearer_context_s                *bearer_context_p,
  boolean                             api_based_flag,
  void                               *info
)
  {
  boolean                             rval = FALSE;
  ds_sys_rat_ex_enum_type             rat = DS_SYS_RAT_EX_3GPP_MAX;
  ps_dpm_um_rereg_bearer_info_type   *rereg_info = NULL;
  ps_dpm_um_reg_bearer_info_type     *reg_info = NULL;
  dsm_watermark_type                 *wm_ptr = NULL;
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;
/*------------------------------------------------------------------------*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return rval;
  }
  
  if (info == NULL)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Input Param: Info invalid");
    return rval;
  }

  switch( bearer_context_p->ds_bearer_context_dyn_p->call_mode)
  {
     case SYS_SYS_MODE_WCDMA:
      rat = DS_SYS_RAT_EX_3GPP_WCDMA;
      wm_ptr = &(bearer_context_p->rx_wm.wm_item);
      rval = TRUE;
       break;

     case SYS_SYS_MODE_TDS:
      rat = DS_SYS_RAT_EX_3GPP_TDSCDMA;
      wm_ptr = &(bearer_context_p->rx_wm.wm_item);
      rval = TRUE;
       break;

     case SYS_SYS_MODE_GSM:
      rat = DS_SYS_RAT_EX_3GPP_GERAN;
      wm_ptr = &(bearer_context_p->rx_wm.wm_item);
      rval = TRUE;
       break;

    case SYS_SYS_MODE_LTE:
      rat = DS_SYS_RAT_EX_3GPP_LTE;
      wm_ptr = &(bearer_context_p->lte_rx_wm.wm_item);
      rval = TRUE;
       break;

    default:
      DS_3GPP_ERROR_LOG_1_EX(subs_id, "Invalid SYS Mode d for Bearer",
                         bearer_context_p->ds_bearer_context_dyn_p->call_mode);
      break;
  }

  if(rval)
  {
    if(api_based_flag)
    {
      rereg_info = (ps_dpm_um_rereg_bearer_info_type *)info;
      rereg_info->rat = (ps_sys_rat_ex_enum_type)rat;
      rereg_info->l2_to_ps_wm_ptr =  wm_ptr;
    }
    else
    {
      reg_info = (ps_dpm_um_reg_bearer_info_type *)info;
      reg_info->rat = (ps_sys_rat_ex_enum_type)rat;
      reg_info->l2_to_ps_wm_ptr =  wm_ptr;
    }
  }
  return rval;
}

/*===========================================================================
FUNCTION ds_bearer_cntx_register_dpm

DESCRIPTION
  This function registers the bearer WM with dpm module

PARAMETERS
  bearer_context_p       - bearer_context to be registered

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntx_register_dpm
(
   ds_bearer_context_s                *bearer_context_p
)
{
  ds_pdn_context_s                     *pdn_cntx_p = NULL;
  ps_dpm_um_reg_bearer_info_type       *reg_info = NULL; 
  int32                                 ps_dpm_reg_ret = 0;
  boolean                               rval = FALSE;
  ds_3gppi_local_err_val_e_type         local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE;  
/*--------------------------------------------------------------------------*/

  do
  {
    if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }

    pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
    
    if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }

    reg_info = (ps_dpm_um_reg_bearer_info_type *)modem_mem_alloc 
               (sizeof(ps_dpm_um_reg_bearer_info_type), MODEM_MEM_CLIENT_DATA);

    if (reg_info == NULL)
    {
      /*-----------------------------------------------------------------------
      Memory allocation failure, Cannot Re-register with DPM so reverting to 
      SW path
      -----------------------------------------------------------------------*/
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      ds_bearer_cntxt_setup_sw_path (bearer_context_p);
      break;
    }
    
    memset(reg_info, 0, sizeof(ps_dpm_um_reg_bearer_info_type));

    reg_info->v4_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p);
    reg_info->v6_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p);

    rval = ds_bearer_cntxt_dpm_setup(bearer_context_p,FALSE,(void *)reg_info);

    if (!rval)
    {
      /*-----------------------------------------------------------------------
      Invalid Registration Info for DPM
      -----------------------------------------------------------------------*/
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      DS3GPP_ASSERT (0);
      break;
    }

    reg_info->ps_phys_link_ptr = &(bearer_context_p->ds_bearer_context_dyn_p->phys_link);
    reg_info->eps_id  = (uint8) PS_PHYS_LINK_GET_BEARER_ID(
       &(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
    ps_dpm_reg_ret = ps_dpm_um_reg_bearer(reg_info);

    if (DSS_SUCCESS == ps_dpm_reg_ret)
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Dpm Registration Succesful for Phys Link 0x%x",
                        reg_info->ps_phys_link_ptr);
      bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm = TRUE;
    }
    else
    {
      DS_3GPP_MSG1_LOW ("DPM Registration for Phys Link:0x%x failed",
                          reg_info->ps_phys_link_ptr);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      DS3GPP_ASSERT (0);
      ds_bearer_cntxt_setup_sw_path(bearer_context_p);
    }
  }while (0);

  if(reg_info != NULL)
  {
    modem_mem_free ((void *)reg_info, MODEM_MEM_CLIENT_DATA);
  }

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1 ("ds_bearer_cntx_register_dpm failed, local err: d",
                        local_err);
  }
} /*ds_bearer_cntx_register_dpm*/

/*===========================================================================
FUNCTION ds_bearer_cntx_register_dpm_tlb_mode

DESCRIPTION
  This function registers the bearer WM with dpm module in TLB Mode A

PARAMETERS
  bearer_context_p       - bearer_context to be registered

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntx_register_dpm_tlb_mode
(
   ds_bearer_context_s                *bearer_context_p
)
{
  ds_pdn_context_s                     *pdn_cntx_p = NULL;
  ps_dpm_um_reg_tlb_bearer_info_type   *tlb_reg_info = NULL; 
  int32                                 ps_dpm_reg_ret = 0;
  dsm_watermark_type                   *wm_ptr = NULL;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;
  sys_modem_as_id_e_type                subs_id= SYS_MODEM_AS_ID_NONE;
/*--------------------------------------------------------------------------*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  
  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }

  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);  
  wm_ptr = &(bearer_context_p->tlb_rx_wm.wm_item);

  dsm_empty_queue (wm_ptr);

  dsm_set_low_wm(wm_ptr, um_wm_threshold.lte_ul_lo);
  dsm_set_hi_wm(wm_ptr, um_wm_threshold.lte_ul_hi);
  dsm_set_dne(wm_ptr, DSLTEPS_TLB_WM_DEFAULT_DNE);
  dsm_set_dne_q_cnt(wm_ptr, DSLTEPS_TLB_WM_DEFAULT_DNE_QCNT);

  do
  {
    tlb_reg_info = (ps_dpm_um_reg_tlb_bearer_info_type *)modem_mem_alloc 
                   (sizeof(ps_dpm_um_reg_tlb_bearer_info_type), 
                    MODEM_MEM_CLIENT_DATA);

    if (tlb_reg_info == NULL)
    {
      DS_3GPP_ERROR_LOG_0_EX(subs_id, "Memory allocation failure, Cannot Re-register with "
                          "DPM");
      break;
    }
    
    memset(tlb_reg_info, 0, sizeof(ps_dpm_um_reg_tlb_bearer_info_type));

    tlb_reg_info->v4_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p);
    tlb_reg_info->v6_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p);

    tlb_reg_info->rat = PS_SYS_RAT_EX_3GPP_LTE;
    tlb_reg_info->l2_to_ps_wm_ptr = &(bearer_context_p->lte_rx_wm.wm_item);

    tlb_reg_info->ps_phys_link_ptr = &(bearer_context_p->ds_bearer_context_dyn_p->
                                                                           phys_link);
    tlb_reg_info->hw_to_ps_wm_ptr = &(bearer_context_p->tlb_rx_wm.wm_item);

    tlb_reg_info->eps_id  = (uint8) PS_PHYS_LINK_GET_BEARER_ID(
                              &(bearer_context_p->ds_bearer_context_dyn_p->phys_link)); 

    ps_dpm_reg_ret = ps_dpm_um_reg_tlb_bearer(tlb_reg_info);

    if (DSS_SUCCESS == ps_dpm_reg_ret)
    {
      bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm = TRUE;
    }
    DS_3GPP_MSG2_HIGH_EX(subs_id, "Dpm Registration Status: %d for Phys Link 0x%x",
                      (DSS_SUCCESS == ps_dpm_reg_ret),
                      tlb_reg_info->ps_phys_link_ptr);
  }while (0);

  if(tlb_reg_info != NULL)
  {
    modem_mem_free ((void *)tlb_reg_info, MODEM_MEM_CLIENT_DATA);
  }
}

/*===========================================================================
FUNCTION ds_bearer_cntx_deregister_dpm

DESCRIPTION
  This function deregisters the bearer with dpm module

PARAMETERS
  bearer_context_p       - bearer_context to be registered
    
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntx_deregister_dpm
(
  ds_bearer_context_s                  *bearer_context_p
)
{
  ds_pdn_context_s                     *pdn_cntx_p;
  ps_dpm_um_dereg_bearer_info_type     *dereg_info = NULL; 
  int32                                 ps_dpm_dereg_ret = 0;
  sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE;  
/*-------------------------------------------------------------------------*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

  if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }
  do
  {
    dereg_info = (ps_dpm_um_dereg_bearer_info_type *)modem_mem_alloc 
                 (sizeof(ps_dpm_um_dereg_bearer_info_type), 
                  MODEM_MEM_CLIENT_DATA);

    /*-------------------------------------------------------------------- 
     Is an Assert needed in case of Memory allocation failure, i feel just
     a memory allocation failure should not stop de-registration.
     ASSERT will help us catch us a Memory leak 
    ---------------------------------------------------------------------*/
    if (dereg_info == NULL)
    {
      DS_3GPP_ERROR_FATAL("Memory allocation failure, Cannot De-register with DPM");
      break;
    }

    memset (dereg_info, 0, sizeof(ps_dpm_um_dereg_bearer_info_type));
  
    dereg_info->v4_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p);
    dereg_info->v6_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p);
  
    dereg_info->ps_phys_link_ptr = &(bearer_context_p->ds_bearer_context_dyn_p->
                                                                      phys_link);
    ps_dpm_dereg_ret = ps_dpm_um_dereg_bearer(dereg_info); 

    DS_3GPP_MSG2_HIGH_EX(subs_id, "DPM De-Registration Status: %d for Phys Link: 0x%x",
                      (DSS_SUCCESS == ps_dpm_dereg_ret),
                      dereg_info->ps_phys_link_ptr);
  }while (0);

  if(dereg_info != NULL)
  {
    modem_mem_free ((void *)dereg_info, MODEM_MEM_CLIENT_DATA);
  }
}
/*===========================================================================
FUNCTION ds_bearer_cntx_deregister_dpm_iface

DESCRIPTION
  This function deregisters the iface with dpm module for the associated
  bearers

PARAMETERS
  bearer_context_p       - bearer_context to be registered
  iface_ptr              - iface to be derigstered
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void ds_bearer_cntx_deregister_dpm_iface
(
   ds_pdn_context_s                 *pdn_cntx_p,
   ps_iface_type                    *iface_ptr
)
{
  ps_dpm_um_dereg_iface_info_type   *dereg_info = NULL;
  int32                              ps_dpm_dereg_ret = 0;
  ds_3gppi_local_err_val_e_type      local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;  
/*--------------------------------------------------------------------------*/

  do
  {
      if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }

    if (iface_ptr == NULL)
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }

    dereg_info = (ps_dpm_um_dereg_iface_info_type *)modem_mem_alloc 
                 (sizeof(ps_dpm_um_dereg_iface_info_type), 
                  MODEM_MEM_CLIENT_DATA);
    
    if (dereg_info == NULL)
    {
      DS_3GPP_ERROR_FATAL("Unable to allocate heap memory, de-registration failure");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
    } 

    memset(dereg_info, 0, sizeof(ps_dpm_um_dereg_iface_info_type));
  
    if(iface_ptr == DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p))
    {
      dereg_info->v4_iface_ptr = iface_ptr;
    }
    else
    {
      dereg_info->v6_iface_ptr = iface_ptr;
    }

    ps_dpm_dereg_ret = ps_dpm_um_dereg_iface(dereg_info);

    if (DSS_SUCCESS == ps_dpm_dereg_ret)
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "DPM De-Registration Succesful for Iface: 0x%x",
                        iface_ptr);
    }
    else
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;
    }
  }while (0);

  if(dereg_info != NULL)
  {
    modem_mem_free ((void *)dereg_info, MODEM_MEM_CLIENT_DATA);
  }

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2 ("DPM De-registration failed for Iface: 0xx local err d",
                        iface_ptr, local_err);
  }
} /*ds_bearer_cntx_deregister_dpm_iface*/

/*===========================================================================
FUNCTION ds_bearer_cntx_reregister_dpm

DESCRIPTION
  This function reregisters the iface with dpm module
  in case of a context transfer
PARAMETERS
  bearer_context_p       - bearer_context to be registered
  iface_ptr              - iface to be derigstered     
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void  ds_bearer_cntx_reregister_dpm
(
  ds_bearer_context_s *bearer_context_p
)
{
  ps_dpm_um_rereg_bearer_info_type        *rereg_info;
  int32                                    ps_dpm_rereg_ret = 0;
  ds_pdn_context_s                        *pdn_cntx_p = NULL;
  boolean                                  rval = FALSE;
  ds_3gppi_local_err_val_e_type            local_err =
                                                  DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type                   subs_id = SYS_MODEM_AS_ID_NONE;  
/*-------------------------------------------------------------------------*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

  if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  } 

  do
  {
    rereg_info = (ps_dpm_um_rereg_bearer_info_type *)modem_mem_alloc 
                 (sizeof(ps_dpm_um_rereg_bearer_info_type), 
                  MODEM_MEM_CLIENT_DATA);

    if (rereg_info == NULL)
    {
      /*------------------------------------------------------------------
      Memory allocation failure, Cannot Re-register with DPM
      ------------------------------------------------------------------*/
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      ds_bearer_cntxt_setup_sw_path(bearer_context_p);
      break;
    }

    memset(rereg_info, 0, sizeof(ps_dpm_um_rereg_bearer_info_type));

    rereg_info->v4_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p);
    rereg_info->v6_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p);

    rval = ds_bearer_cntxt_dpm_setup(bearer_context_p,TRUE,(void *)rereg_info);

    if (!rval)
    {
      /*------------------------------------------------------------------
      Invalid Registration Info for DPM
      ------------------------------------------------------------------*/
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
    }

    rereg_info->ps_phys_link_ptr = &(bearer_context_p->ds_bearer_context_dyn_p->
                                                                        phys_link);

    rereg_info->eps_id  = (uint8) PS_PHYS_LINK_GET_BEARER_ID(
       &(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
    
    ps_dpm_rereg_ret = ps_dpm_um_rereg_bearer(rereg_info); 

    if (DSS_SUCCESS == ps_dpm_rereg_ret)
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Dpm ReRegistration Succesful for Phys Link 0x%x",
                        rereg_info->ps_phys_link_ptr);
    }
    else
    {
      /*------------------------------------------------------------------
      Re-registration with DPM failed during IRAT
      ------------------------------------------------------------------*/
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      ds_bearer_cntxt_setup_sw_path(bearer_context_p);
    }
  }while (0);

  if(rereg_info != NULL)
  {
    modem_mem_free ((void *)rereg_info, MODEM_MEM_CLIENT_DATA);
  }

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1 ("Re-registration with DPM failed: local err d",
                        local_err);
  }
}

/*===========================================================================
FUNCTION ds_bearer_cntxt_get_subs_id_from_bearer_internal

DESCRIPTION
  This function fetches the subs id to which the bearer belongs. There will
  be no F3 if bearer context is invalid
 
PARAMETERS
  bearer_context_p       - bearer_context
  subs_id_p              - subscription id
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if successfully fetched subs Id
  FALSE, otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntxt_get_subs_id_from_bearer_internal
(
  ds_bearer_context_s    *bearer_context_p,
  sys_modem_as_id_e_type *subs_id_p
)
{
  boolean                 ret_val = FALSE;
  /*----------------------------------------------------------------------*/ 

  if(ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
  {
    if (subs_id_p != NULL && 
          ds3gsubsmgr_is_subs_id_valid
            (bearer_context_p->ds_bearer_context_dyn_p->subs_id))
    {
      *subs_id_p = bearer_context_p->ds_bearer_context_dyn_p->subs_id ;
       ret_val = TRUE;
    }
  }
  return ret_val;
} /* ds_bearer_cntxt_get_subs_id_from_bearer_internal */

/*===========================================================================
FUNCTION ds_bearer_cntxt_get_subs_id_from_bearer

DESCRIPTION
  This function fetches the subs id to which the bearer belongs. Error F3 will
  be printed for error.
 
PARAMETERS
  bearer_context_p       - bearer_context
  subs_id_p              - subscription id
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if successfully fetched subs Id
  FALSE, otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntxt_get_subs_id_from_bearer
(
  ds_bearer_context_s    *bearer_context_p,
  sys_modem_as_id_e_type *subs_id_p
)
{
  boolean                 ret_val = FALSE;
  /*----------------------------------------------------------------------*/ 

  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    if (subs_id_p != NULL && 
          ds3gsubsmgr_is_subs_id_valid
            (bearer_context_p->ds_bearer_context_dyn_p->subs_id))
    {
      *subs_id_p = bearer_context_p->ds_bearer_context_dyn_p->subs_id ;
       ret_val = TRUE;
    }
  }
  return ret_val;
} /* ds_bearer_cntxt_get_subs_id_from_bearer */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CLEANUP_PENDING_CALLS

DESCRIPTION
  This function cleans up bearers in PEND_REDIAL state after a rat change. 
  
PARAMETERS  
  bearer_context_p - bearer context pointer
  new_mode         - new_rat
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
static void ds_bearer_cntxt_cleanup_pending_calls
(
  ds_bearer_context_s  *bearer_context_p,
  sys_sys_mode_e_type  new_mode
);

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_IS_ANY_OTHER_BEARER_UP

DESCRIPTION
  This function checks if any other bearer is in UP/UP_DORMANT state

PARAMETERS  
  Bearer Context ptr

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_is_any_other_bearer_up
(
  ds_bearer_context_s *bearer_context_p
);

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_FREE_BEARER_INST

DESCRIPTION
  This function cleans up the bearer instance on a call end.

PARAMETERS
  IN
    bearer_context_p - Pointer to the bearer context to be cleaned up.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_bearer_cntxt_free_bearer_inst
(
  ds_bearer_context_s* bearer_context_p
)
{
  sys_sys_mode_e_type    mode = SYS_SYS_MODE_NONE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }
  else
  {
    mode = ds3gpp_get_current_network_mode(subs_id);
  }


  if(bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
  {
    ds_bearer_cntx_deregister_dpm(bearer_context_p); 
  }

  ds3gpp_enter_global_crit_section();

  ds_3gpp_dyn_mem_hdlr_free_bearers_dyn_mem(bearer_context_p, subs_id);

  /*---------------------------------------------------------------------------
    Resetting all the fields to their default values.
  ---------------------------------------------------------------------------*/
  ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                   DS_BEARER_CONTEXT_STATE_DOWN);


  bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = CM_CALL_ID_INVALID;
  bearer_context_p->ds_bearer_context_dyn_p->nsapi = 0;
  bearer_context_p->ds_bearer_context_dyn_p->call_mode = mode;
  bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW = TRUE;
  bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm = FALSE;
  bearer_context_p->ds_bearer_context_dyn_p->
    ul_registered_with_ltepdcp = FALSE;
  bearer_context_p->ds_bearer_context_dyn_p->rm_flow_enabled = TRUE;
  bearer_context_p->ds_bearer_context_dyn_p->reorig_cmd_enqueued = FALSE;
#ifdef FEATURE_DATA_IMS
  bearer_context_p->ds_bearer_context_dyn_p->im_cn_subsystem_flag = FALSE;
#endif
  bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp = FALSE;
  /*------------------------------------------------------------------------
	 Set the timer to FALSE and Undefine the timer
  ------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running = FALSE;
  rex_undef_timer(&(bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer));
  ds_3gpp_flow_control_free_fcf_timer(bearer_context_p);
  ds_3gpp_flow_control_set_pending_disable_fcf(bearer_context_p, FALSE);
  ds_3gpp_flow_control_set_tcp_bi_direction(bearer_context_p, FALSE);


#ifdef FEATURE_DATA_FLOW_MGMT
  bearer_context_p->ds_bearer_context_dyn_p->flow_ack_timer_running = FALSE;
  rex_undef_timer(&(bearer_context_p->ds_bearer_context_dyn_p->flow_ack_timer));
  bearer_context_p->ds_bearer_context_dyn_p->gone_empty_cmd_enqueued = FALSE; 
  rex_undef_timer(&(bearer_context_p->flow_control_info.burst_prohibit_timer));
  /*------------------------------------------------------------------------
	 Memset flow control info
  ------------------------------------------------------------------------*/
  memset(&(bearer_context_p->flow_control_info),0,
             sizeof(ds3g_burst_based_flow_control_s));
#endif  /*FEATURE_DATA_FLOW_MGMT */

  /*Resetting the bearer id in the Phys Link strucute during bearer free*/
  PS_PHYS_LINK_SET_BEARER_ID(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),0);

  DS_3GPP_MSG2_HIGH_EX(subs_id, "Setting Physlink Pointer 0x%p to bearer_id %d",
                     &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                     0);

  /* 
    Set the SDF ID to be invalid. This would come in handy should there be
    a bug while reusing the bearer contexts
  */
  bearer_context_p->ds_bearer_context_dyn_p->sdf_id = 0xFFFF;
  /*-------------------------------------------------------------------------
    We should not set the in_use flag to FALSE here. It will always be done
    from the PDN context.
  -------------------------------------------------------------------------*/
  /*------------------------------------------------------------------------
    Reset the values of the byte counters to zero
  ------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->tx_bytes = 0;
  bearer_context_p->ds_bearer_context_dyn_p->rx_bytes = 0;

  bearer_context_p->ds_bearer_context_dyn_p->wm_flush_state 
    = DS_BEARER_CONTEXT_UM_WM_STATE_INVALID;

  /*------------------------------------------------------------------------
    Invalidate the ps handle so that signal callbacks will not be processed
  ------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->ps_handle 
    = DS_3GPP_BEARER_CTXT_INVALID_HANDLE;

  /*-----------------------------------------------------------------------
    Reset the rab_reestab_retry counter for this bearer.
  -----------------------------------------------------------------------*/
  DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);

  /*----------------------------------------------------------------------
    Set each enqueue func ptr to NULL here
  ----------------------------------------------------------------------*/
  (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr = NULL;
  (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data = NULL;

  /*----------------------------------------------------------------------
    Set dl's non empty func ptr to NULL here
  ----------------------------------------------------------------------*/
  (bearer_context_p->rx_wm.wm_item).non_empty_func_ptr = NULL;
  (bearer_context_p->rx_wm.wm_item).non_empty_func_data = NULL;

  (bearer_context_p->lte_rx_wm.wm_item).non_empty_func_ptr = NULL;
  (bearer_context_p->lte_rx_wm.wm_item).non_empty_func_data = NULL;

  (bearer_context_p->tlb_rx_wm.wm_item).non_empty_func_ptr = NULL;
  (bearer_context_p->tlb_rx_wm.wm_item).non_empty_func_data = NULL;
  ds3gpp_leave_global_crit_section();

} /* ds_bearer_cntxt_free_bearer_inst() */

/*===========================================================================
FUNCTION      DS_BEARER_CNTXT_PHYS_LINK_DOWN

DESCRIPTION
  Called by the embedded application.  Brings down the EPS/UMTS traffic 
  channel. It is also called on IFace Down command.

PARAMETERS
  phys_link_ptr - phys link ptr for the call
  info_ptr      - not used

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
int ds_bearer_cntxt_phys_link_down
(
  ps_phys_link_type *phys_link_ptr_i,
  void              *info_ptr
)
{
  const ps_phys_link_type    *phys_link_ptr = phys_link_ptr_i;
  ds_cmd_type                *cmd_ptr;                      /* DS Task message        */
  uint32                      call_instance;
  ds_3gpp_pdn_call_info_type *call_info_ptr = NULL;
  sys_modem_as_id_e_type      subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds_3gpp_bearer_cntxt_get_subs_id_from_physlink(phys_link_ptr_i,&subs_id))
  {
    return (-1);
  }

  call_instance =  (uint32)(phys_link_ptr->client_data_ptr);

  DS_3GPP_MSG1_HIGH_EX(subs_id,
                 "DSUMTSPS PHYS LINK DOWN CMD :Call Inst:%d", call_instance);

  /*------------------------------------------------------------------------
    Phys link down command handler for secondary. However reuse existing
    DS3GPP_PHYS_LINK_DOWN_CMD event. The flags set in the call table will
    give enough information to differentiate between a secondary and a
    primary
  ------------------------------------------------------------------------*/

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_pdn_call_info_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return (-1);
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PHYS_LINK_DOWN;
  call_info_ptr = (ds_3gpp_pdn_call_info_type*)cmd_ptr->cmd_payload_ptr;

  call_info_ptr->call_instance = call_instance;
  call_info_ptr->info_ptr = info_ptr;

    ds_put_cmd_ext(cmd_ptr);

  return(0);
} /* ds_bearer_cntxt_phys_link_down() */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_IS_ANY_BEARER_NOT_DOWN

DESCRIPTION
  This function checks if any of the bearer is not in DOWN state
  
PARAMETERS  
  subs_id: Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns TRUE if any bearer is not in down state
  FALSE - Otherwise
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_any_bearer_not_down
(
  sys_modem_as_id_e_type subs_id
)
{
  boolean                  ret_val = FALSE;
  uint8                    bearer_index = 0;
  ds_bearer_context_s     *bearer_context_p= NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( bearer_index = 0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p, subs_id))
    {
      continue;
    }

    DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer index:%d is in state:%d",bearer_index,
                       bearer_context_p->ds_bearer_context_dyn_p->state);

    if( (bearer_context_p->ds_bearer_context_dyn_p->state !=
         DS_BEARER_CONTEXT_STATE_DOWN) &&
        (bearer_context_p->ds_bearer_context_dyn_p->state !=
         DS_BEARER_CONTEXT_STATE_PEND_REDIAL) )
    {
      ret_val = TRUE;
      break;
    }
  }

  return ret_val;
}/*ds_3gpp_bearer_cntxt_is_any_bearer_not_down*/

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_IS_LAST_BEARER_DOWN_PER_SUB

DESCRIPTION
  This function checks if all bearers are down per sub
  
PARAMETERS  
  subs_id: Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns TRUE if last bearer is down
  FALSE - Otherwise
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_last_bearer_down_per_sub
(
  sys_modem_as_id_e_type subs_id
)
{
  boolean                  ret_val = TRUE;
  uint8                    bearer_index = 0;
  ds_bearer_context_s     *bearer_context_p= NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( bearer_index = 0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p, subs_id))
    {
      continue;
    }

    DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer index:%d is in state:%d",bearer_index,
                       bearer_context_p->ds_bearer_context_dyn_p->state);

    if (bearer_context_p->ds_bearer_context_dyn_p->state !=
         DS_BEARER_CONTEXT_STATE_DOWN)
    {
      ret_val = FALSE;
      break;
    }
  }

  return ret_val;
}/*ds_3gpp_bearer_cntxt_is_last_bearer_down_per_sub*/

/*===================================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_DATA_ACTIVITY_STATUS

DESCRIPTION
  This function handles getting data activity status for ULDL per sub
 
PARAMETERS
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled

DEPENDENCIES None

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS None
========================================================================================*/
int ds_3gpp_bearer_cntxt_get_data_activity_status
(
  void                    *arg_val_ptr, /*Pass in pointer for status*/
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
)
{
  ps_sys_ioctl_3gpp_get_data_activity_status_type *ps_data_activity_info_ptr
                                                                     = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return -1;
  }

  if ((arg_val_ptr == NULL) || (ps_errno == NULL))
  {
    if(ps_errno != NULL)
    {
      *ps_errno = (int16)DS_EFAULT;
    }
    DS_ERROR_LOG_0_EX(MSG_LEGACY_ERROR, subs_id,"Null Pointer passed");
    return -1;
  }

  ps_data_activity_info_ptr =
         (ps_sys_ioctl_3gpp_get_data_activity_status_type *)arg_val_ptr;

  memset(ps_data_activity_info_ptr,
         0,
         sizeof(ps_sys_ioctl_3gpp_get_data_activity_status_type));

  if (!ds_3gpp_bearer_cntxt_fill_data_activity_status
                   ( &ps_data_activity_info_ptr->data_activity_status,
                     subs_id))
  {
    *ps_errno = (int16)DS_EINVAL;
    return -1;
  }
  
  /*Success*/
  *ps_errno = 0;
  return 0;

} /* ds_3gpp_bearer_cntxt_get_data_activity_status */

/*===================================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_PROCESS_DATA_ACTIVITY_REGISTRATION

DESCRIPTION : IOCTL for letting MH know if clients have registered or 
              deregistered for data activity status notifcations 
 
PARAMETERS
  arg_val_ptr: contains argument for client status
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled

DEPENDENCIES None

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS None
========================================================================================*/
int ds_3gpp_bearer_cntxt_process_data_activity_registration
(
  void                    *arg_val_ptr, /*Pass in pointer for status*/
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
)
{
  ps_sys_ioctl_3gpp_data_activity_registration_status_type
                                            *ps_data_activity_reg_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return -1;
  }

  if ((arg_val_ptr == NULL) || (ps_errno == NULL))
  {
    if(ps_errno != NULL)
    {
      *ps_errno = (int16)DS_EFAULT;
    }
    DS_ERROR_LOG_0_EX(MSG_LEGACY_ERROR, subs_id,"Null Pointer passed");
    return -1;
  }

  ps_data_activity_reg_ptr =
      (ps_sys_ioctl_3gpp_data_activity_registration_status_type *)arg_val_ptr;


  if(!ds_3gpp_bearer_cntxt_handle_client_registration_status
                      (ps_data_activity_reg_ptr->is_client_registered,
                       subs_id))
  {
    *ps_errno = (int16)DS_EFAULT;
    return -1;
  }

  /*--------------------------------------------- 
    If we have reached here, return Success
    --------------------------------------------*/
  *ps_errno = 0;
  return 0;

} /* ds_3gpp_bearer_cntxt_process_data_activity_registration */

/*===================================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_HANDLE_CLIENT_REGISTRATION_STATUS

DESCRIPTION
  This function Starts/Stops the sampling timer depending if the client is registered
  or de-registered
 
PARAMETERS
  is_client_registered : boolean flag to indicate if first client is registered for
                         data ctivity status info for this sub or last client is
                          deregistered.
  subs_id :  The subscription for which the client is registered/deregistered

DEPENDENCIES None

RETURN VALUE
  TRUE - Successful handling
  FALSE - Some error has occured/Failure

SIDE EFFECTS None
========================================================================================*/
boolean ds_3gpp_bearer_cntxt_handle_client_registration_status
(
  boolean                  is_client_registered,
  sys_modem_as_id_e_type   subs_id
)
{
   ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return FALSE;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id %d",subs_id);
    return FALSE;
  }
   /*-------------------------------------------------------------- 
    If client is registered, start the sampling timer
    If deregistered, stop timer.
    We dont need to check per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_initialized
    here. Just go ahead and start the timer. You might get data after timer expires
    ---------------------------------------------------------------*/

  if (is_client_registered)
  {
    if (per_subs_info_hndl->is_client_reg)
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Duplicate client registration. Return False");
      return FALSE;
    }
    else
    {
      per_subs_info_hndl->is_client_reg = TRUE;
    }
    /*------------------------------------------------------ 
      Start timer only when the timer is not runnning.
      Fail Safe
    ---------------------------------------------------------*/
    if (!per_subs_info_hndl->is_sampling_timer_running)
    {
      DS_BEARER_CNTXT_SET_SAMPLING_TIMER(per_subs_info_hndl,
                                          DATA_ACTIVITY_SAMPLING_TIMER_DURATION);
    }
  }
  else
  {
    DS_3GPP_MSG1_HIGH_EX(subs_id, "Last client has deregistered for UIM stats on sub %d",
                      subs_id) ;

    per_subs_info_hndl->is_client_reg = FALSE;

    if (per_subs_info_hndl->is_sampling_timer_running)
    {
      DS_BEARER_CNTXT_CLR_SAMPLING_TIMER(per_subs_info_hndl);
    }
    /*---------------------------------------------------------------- 
      At this point store the latest stats info since sampling timer
      wont run till completion SO we wont get chance to store latest
      ULDL WM info
      ---------------------------------------------------------------*/
    if (per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up)
    {
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt =
                               ds3gpp_get_total_uldl_wm_cnt_hdlr(subs_id);
    }

  }
  return TRUE;
}/*ds_3gpp_bearer_cntxt_handle_client_registration_status*/

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/
/*===========================================================================
                             PHYS_LINK_DOWN, PHYS_LINK_NULL Functions
===========================================================================*/

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PHYS_LINK_DOWN_CMD_HDLR

DESCRIPTION
  Called in DS TASK context in response to a DS_BEARER_CNTXT_PHYS_LINK_DOWN 
  command on the UMTS/EPS iface. From the new implementation, it looks like
  this function need not be handled. There is no one who calls PHYS_LINK_DOWN
  Even the function body has been left empty for now. This function will be 
  removed if it is deemed unnecessary.

PARAMETERS
  call_instance - dummy variable.
  info_ptr      - void pointer

DEPENDENCIES
  Called in DS TASK context.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_phys_link_down_cmd_hdlr
(
  uint32 bearer_instance,
  void *info_ptr
)
{
  ds_bearer_context_s     *bearer_context_p;
  boolean                  retVal;
  ds_pdn_context_s        *pdn_cntx_p = NULL;
  sys_modem_as_id_e_type   subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS3GPP_ASSERT(bearer_instance < DS_3GPP_MAX_BEARER_CONTEXT);

  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/
  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  pdn_cntx_p =  bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  
  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p) &&
     (pdn_cntx_p->ds_pdn_context_dyn_p->num_active_bearer_context == 0))
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "bearer already cleaned up from PDN, returning ...");
    return;
  }
  
  retVal = ds_bearer_cntxt_tear_down(bearer_context_p, info_ptr);
  if(retVal != DS3G_SUCCESS)
  {
    if(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
       ->bearer_down_ind_cb != NULL)
    {
      ds_3gpp_down_reason_t  net_down_reason;
      net_down_reason.reason.ps_iface_down_reason = 
        PS_NET_DOWN_REASON_VALIDATION_FAILURE;
      net_down_reason.reason_T = DS_3GPP_PS_IFACE_NET_DOWN_REASON_T;
      bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr
        ->bearer_down_ind_cb(
                      bearer_context_p->ds_bearer_context_dyn_p->
                      pdn_context_ptr,\
                      bearer_context_p,
                      net_down_reason,
                      FALSE);
    }/* Post bearer down indication with call_was_connected as False*/
  }
} /* ds_bearer_cntxt_phys_link_down_cmd_hdlr() */


/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PHYS_LINK_FLOW_CMD_HDLR

DESCRIPTION
  Called in DS TASK context in response to DS3GPP_PHYS_LINK_FLOW_ENABLE_CMD
  or DS3GPP_PHYS_LINK_FLOW_DISABLE_CMD. Need to flow control the phys link
  appropriately.

PARAMETERS
   cmd_ptr - information relating to the DS comand.

DEPENDENCIES
  Called in DS TASK context. Cmd ptr and cmd payload ptr must be valid.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_phys_link_flow_cmd_hdlr
(
  const ds_cmd_type *cmd_ptr
)
{
  uint32    bearer_instance;
  ds_bearer_context_s             *bearer_cntxt_p = NULL;
  ds_3gpp_pdn_call_info_type      *call_info_ptr = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
  }

  call_info_ptr = (ds_3gpp_pdn_call_info_type*)cmd_ptr->cmd_payload_ptr;
  bearer_instance = call_info_ptr->call_instance;

  if (cmd_ptr->hdr.cmd_id == DS_CMD_3GPP_PHYS_LINK_FLOW_ENABLE) 
  {
    bearer_cntxt_p = ds_bearer_cntxt_get_instance_ptr((uint8)bearer_instance);
    if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntxt_p, &subs_id))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;
    }
    
    DS_3GPP_MSG1_HIGH_EX(subs_id, "PHYS LINK ENABLE FLOW on bearer context %d",bearer_instance);
    
    if (ds_bearer_cntx_validate_bearer_context(bearer_cntxt_p))
    {

      ds_3gpp_bearer_cntxt_enable_flow(bearer_cntxt_p, 
                                 (uint32)call_info_ptr->info_ptr, FALSE);
    }
    else 
    {
        // DS_3GPP_ERROR_LOG_1_EX(subs_id, "Bearer context invalid! bearer_instance:d",bearer_instance);
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
        break;
    }
  }
  else 
  {
      // DATA_MSG1_ERROR("Incorrect cmd_id = %d", cmd_ptr->hdr.cmd_id);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
    }
  } while (0);

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DATA_MSG1_ERROR("ds_bearer_cntxt_phys_link_flow_cmd_hdlr: local_err %d",
                    local_err);
    return;
  }

} /*ds_bearer_cntxt_phys_link_flow_cmd_hdlr*/

#ifdef FEATURE_UE_INITIATED_DORMANCY 
/*===========================================================================
 DS_3GPP_BEARER_CNTXT_HANDLE_GO_DORMANT_REQUEST
  
 DESCRIPTION
  This function handles go_dormant request from APPS.
  Checks if any data is pending in WM or any other beareris UP 
  other than the one on which go_dormant is requested
  
 PARAMETERS
  bearer_cntx_p: Bearer on which GO_DORMANT is requested
  bearer_cntx_inst: Bearer Instance
  ps_errno: Error cause to be returned
  
 DEPENDENCIES
  None.
  
 RETURN VALUE
  0- Dormancy request sent to NW
 -1 - if request is not honoured
  
 SIDE EFFECTS
  None.
  
 ===========================================================================*/
LOCAL int ds_3gpp_bearer_cntxt_handle_go_dormant_request
(
  sys_modem_as_id_e_type subs_id,
  ds_bearer_context_s *bearer_cntx_p,
  uint32               bearer_cntx_inst,
  ps_iface_fast_dormancy_status_type*  fd_status_p,
  int16                *ps_errno
)
{  
  boolean             index;
  ds_bearer_context_s *other_bearer_cntx_p;
  dsm_watermark_type  *tx_wm = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  int                  temp_num = 0;
  sys_modem_as_id_e_type  other_subs_id = SYS_MODEM_AS_ID_NONE;
  /*--------------------------------------------------------------------------  */
  if ( ps_errno == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(ds_bearer_cntx_validate_bearer_context(bearer_cntx_p))
  {
    if (bearer_cntx_p->ds_bearer_context_dyn_p->state != DS_BEARER_CONTEXT_STATE_UP)
    {
      DS_3GPP_MSG1_LOW("GO DORMANT IOCTL fail. bearer_state: %d "
                            ,bearer_cntx_p->ds_bearer_context_dyn_p->state);
     temp_num = bearer_cntx_p->ds_bearer_context_dyn_p->state;
     fd_status_p->dorm_status = PS_FAST_DORMANCY_STATUS_FAILURE;
     fd_status_p->failure_reason = PS_FAST_DORMANCY_STATUS_FAIL_INVALID_PDN_STATE;
     *ps_errno =  DS_EINVAL;
     local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
     goto func_exit;
    }
  }
  /* Check if any other PDPs are active i.e. not dormant */
  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    other_bearer_cntx_p = ds_bearer_cntxt_get_instance_ptr(index);
    if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(other_bearer_cntx_p, &other_subs_id) ||
        other_subs_id != subs_id )
    {
      continue;
    }
    if (other_bearer_cntx_p->ds_bearer_context_dyn_p->state == 
                                         DS_BEARER_CONTEXT_STATE_UP) 
    {
      /*-------------------------------------------------------------------
         If there is still data in watermark, then do not go dormant. 
          Fail IOCTL
          -------------------------------------------------------------------*/
      tx_wm = &(other_bearer_cntx_p->tx_wm.wm_item);
      if(tx_wm->current_cnt != 0)
      {
        DS_3GPP_MSG1_LOW("GO DORMANT IOCTL fail. Data present in WM for "
                            "bearer instance [%d].",index);
        fd_status_p->dorm_status = PS_FAST_DORMANCY_STATUS_FAILURE;
        fd_status_p->failure_reason = PS_FAST_DORMANCY_STATUS_FAIL_DATA_ACTIVITY;
        *ps_errno =  DS_EINVAL;
        temp_num = index;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        goto func_exit;
      }
    }
    if(((other_bearer_cntx_p->ds_bearer_context_dyn_p->state == 
                                   DS_BEARER_CONTEXT_STATE_UP) ||
       (other_bearer_cntx_p->ds_bearer_context_dyn_p->state == 
                     DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB))
       &&
      (index != bearer_cntx_inst) )
    {
      DS_3GPP_MSG2_LOW("GO DORMANT with other active bearer inst [%d] "
                         "is disallowed, ps_errno [%d]",
                          index, DS_EINVAL);
      fd_status_p->dorm_status = PS_FAST_DORMANCY_STATUS_FAILURE;
      fd_status_p->failure_reason = PS_FAST_DORMANCY_STATUS_FAIL_OTHER_PDN_UP;
      *ps_errno =  DS_EINVAL;
      temp_num = index;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      goto func_exit;
    }
  } /* for */
      
  /* send CM command */
  if (!ds3g_msh_go_dormant(subs_id))
  {
    *ps_errno =  DS_ETRYAGAIN;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
    goto func_exit;
  }  
  return 0;

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, "ds_3gpp_bearer_cntxt_handle_go_dormant_request: err d "
                       "temp_num d",
                       local_err, temp_num);
  }
  return -1;
}

/*===========================================================================
 DS_3GPP_BEARER_CNTXT_DORMANCY_DELAY_TMR_EXPIRY_CB
  
 DESCRIPTION
  This is the callback function for dormancy delay timer expiry
  checks if there is any data activity within this time period and
  takes necessary action
  
 PARAMETERS
callback_data: phys_link_ptr on which do_dormant is requested
  
 DEPENDENCIES
  None.
  
 RETURN VALUE
 None
  
 SIDE EFFECTS
  None.
  
 ===========================================================================*/
void ds_3gpp_bearer_cntxt_dormancy_delay_tmr_expiry_cb
(
  unsigned long callback_data
)
{
  ps_phys_link_type   *phys_link_ptr;
  uint32                bearer_cntx_inst;
  ds_bearer_context_s *bearer_cntx_p;
  int16                ps_errno;
  ds_3gpp_delay_dormancy_s* info_p = NULL;
  ps_iface_fast_dormancy_status_type  fd_status = {0};
  boolean                   gen_fd_event = FALSE;
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*---------------------------------------------------------------------*/

  phys_link_ptr = (ps_phys_link_type*)callback_data;

  DS3GPP_ASSERT ( phys_link_ptr != NULL );
  bearer_cntx_inst = (uint32)phys_link_ptr->client_data_ptr;
  bearer_cntx_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntx_p, &subs_id);

  DS_3GPP_MSG1_HIGH_EX(subs_id, "Dormancy Delay Timer Expiry cb, cb_data:0x%x",
                                                      phys_link_ptr);

  if ((!ds_3gpp_pdn_cntxt_validate_and_get_delay_dormancy_info_ptr(&info_p,subs_id)) 
      || (!ds_bearer_cntx_validate_bearer_context(bearer_cntx_p)))
  {
    ds_3gpp_pdn_cntxt_free_memory_for_delay_dormancy_stats(subs_id);
    return;
  }
   /* freeing timer */
  rex_delete_timer_ex(info_p->timer_p);
  
  if(PS_PHYS_LINK_GET_STATE(phys_link_ptr) == PHYS_LINK_DOWN)
  {
    if(bearer_cntx_p->ds_bearer_context_dyn_p->state != 
                               DS_BEARER_CONTEXT_STATE_UP)
  {
    DS_3GPP_MSG0_LOW("phys_link on which GO_DORMANT Requested already DOWN");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
  }
  else
  {
      fd_status.dorm_status = PS_FAST_DORMANCY_STATUS_SUCCESS;
      fd_status.failure_reason = PS_FAST_DORMANCY_STATUS_FAIL_NONE;
      gen_fd_event =  TRUE;
    }
  }
  else
  {
    /* check if data activity is present */
    if (TRUE == ds_3gpp_bearer_cntxt_init_or_check_data_statistics(
                                       DS_3GPP_CHECK_DATA_ACTIVITY))
    {
       DS_3GPP_MSG0_LOW("Data Activity is present, not honouring"
                                  "GO_DORMANT Request after Timer Expiry");
  
      fd_status.dorm_status = PS_FAST_DORMANCY_STATUS_FAILURE;
      fd_status.failure_reason = PS_FAST_DORMANCY_STATUS_FAIL_DATA_ACTIVITY;
      gen_fd_event =  TRUE;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    }
    else if(0 != ds_3gpp_bearer_cntxt_handle_go_dormant_request(subs_id, 
                                                           bearer_cntx_p,
                                                           bearer_cntx_inst,
                                                           &fd_status,
                                                           &ps_errno))
    {
      DS_3GPP_MSG0_LOW("Dormancy Request not honored");
      gen_fd_event =  TRUE;       
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    }
  }

  if (gen_fd_event == TRUE)
  {
    DS_3GPP_MSG2_LOW("generating dormancy status event, status:%d, cause:%d", 
                                    fd_status.dorm_status, fd_status.failure_reason);
    ds_3gpp_bearer_gen_fast_dormancy_event(bearer_cntx_p, subs_id, (void*)&fd_status );
  }

  ds_3gpp_pdn_cntxt_free_memory_for_delay_dormancy_stats(subs_id);

  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_4_EX(subs_id,
                   "ds_3gpp_bearer_cntxt_dormancy_delay_tmr_expiry_cb: err d"
                   " fdevgen d status d cause d",
                   local_err,
                   gen_fd_event, 
                   fd_status.dorm_status,
                   fd_status.failure_reason);
  }

  return;
} /*ds_3gpp_bearer_cntxt_dormancy_delay_tmr_expiry_cb*/

/*===========================================================================
FUNCTION ds_3gpp_bearer_gen_fast_dormancy_event()

DESCRIPTION
  This function is called to send fast dormancy status event to PS/QMI.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_bearer_gen_fast_dormancy_event
(
  ds_bearer_context_s               *bearer_context_ptr,
  sys_modem_as_id_e_type             subs_id,
  void                              *event_data_ptr
)
{
  ps_iface_type                     *iface_ptr = NULL;
  uint8                              iface_inst;
  ds_pdn_context_s                  *pdn_context_ptr;
  ds_3gpp_iface_s                   *ds_iface_ptr;
  /* ------------------------------------------------------*/  
  if(!ds_bearer_cntxt_validate_bearer_subscription
      (bearer_context_ptr, subs_id))
  {
    return;
  }

  pdn_context_ptr = (ds_pdn_context_s *) (bearer_context_ptr-> \
                                   ds_bearer_context_dyn_p->pdn_context_ptr);
  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr))
  {
    return;
  }
  if((bearer_context_ptr->ds_bearer_context_dyn_p->state 
          == DS_BEARER_CONTEXT_STATE_UP
       || bearer_context_ptr->ds_bearer_context_dyn_p->state 
          == DS_BEARER_CONTEXT_STATE_UP_DORMANT
       || bearer_context_ptr->ds_bearer_context_dyn_p->state
          == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) &&
      (bearer_context_ptr->ds_bearer_context_dyn_p->call_mode 
       == SYS_SYS_MODE_WCDMA) &&
       (bearer_context_ptr->ds_bearer_context_dyn_p->is_default))
  {
  /*-------------------------------------------------------------------------
    Generate technology changed event to PS, and retrieve the iface specific
    to a given call instance.
  -------------------------------------------------------------------------*/
    for (iface_inst = 0; 
                iface_inst < DS_3GPP_MAX_IFACE_PER_PDN_CONTEXT; 
                        iface_inst ++)
    {
      ds_iface_ptr = pdn_context_ptr->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[iface_inst];                            
  
      iface_ptr = (ds_iface_ptr != NULL) ? &(ds_iface_ptr->ps_iface) : NULL;
  
      if (PS_IFACE_IS_IN_USE(iface_ptr))                
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_3gpp_ps_data_generic_ind_hdlr: Posting iface generic"
                          "ind for iface %d", iface_inst);
        ps_iface_generic_ind (iface_ptr,
                              IFACE_FAST_DORMANCY_STATUS_EV,
                              event_data_ptr );
      }
    } /* for each iface*/

  } /* if baarer UP*/
  return;
}/* ds_3gpp_bearer_gen_fast_dormancy_event */

#endif /*FEATURE_UE_INITIATED_DORMANCY */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PHYS_LINK_IOCTL_HDLR

DESCRIPTION
  Callback called from application context for secondary PDP context.
  Central function for embedded applications to bring down or
  modify the secondary context.

PARAMETERS
  *phys_link_ptr    - Ptr to the phys link.
  ioctl_name        - Type of request
  argval_ptr        - QoS specified by the application
  *ps_errno         - Ptr to error code (if any)

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Calls into UMTSPS to bring up or down the secondary phys link handler.

===========================================================================*/
LOCAL int ds_bearer_cntxt_phys_link_ioctl_hdlr
(
  ps_phys_link_type        *phys_link_ptr_i,
  ps_phys_link_ioctl_type  ioctl_name,
  void                     *argval_ptr,
  int16                    *ps_errno
)
{
  ps_phys_link_type                               *phys_link_ptr = phys_link_ptr_i;
  uint32                                          bearer_cntx_inst = 0;
  ds_bearer_context_s                             *bearer_cntx_p;
#ifdef FEATURE_UE_INITIATED_DORMANCY 
  uint32                                          delay_timer_value = 0;
  ds_3gpp_delay_dormancy_s*                       info_p = NULL;
#endif /* FEATURE_UE_INITIATED_DORMANCY */
  int                                             ret_val = 0;
  int16                                           ps_errno_local=0;
  sys_modem_as_id_e_type                          subs_id = SYS_MODEM_AS_ID_NONE;
#ifdef FEATURE_QXDM_WM_TRACING
  uint32                                          *call_id;
#endif /* FEATURE_QXDM_WM_TRACING */
  ds_pdn_context_s                                *pdn_cntxt_p = NULL;
  ps_iface_fast_dormancy_status_type              fd_status;
  ds_3gppi_local_err_val_e_type                   local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  int                                             temp_num = -1;
#ifdef FEATURE_DATA_FLOW_MGMT
  ps_phys_link_ioctl_allowed_num_bytes_type       *allowed_bytes_info_ptr;
  ps_phys_link_ioctl_flow_ctl_type                *flow_ctl_type_ptr;
  uint8                                           flow_ctl_ack;
#endif /*FEATURE_DATA_FLOW_MGMT */
  /*-------------------------------------------------------------------------*/

  if ((NULL == phys_link_ptr) || (NULL == ps_errno))
  {
    DS3GPP_ASSERT(0);
    return -1;
  }

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section); 
  ds3gpp_enter_global_crit_section();
  
  bearer_cntx_inst = (uint32)phys_link_ptr->client_data_ptr;
  bearer_cntx_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntx_p))
  {
    *ps_errno = DS_EINVAL;

    ds3gpp_leave_global_crit_section();	
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section); 
    ret_val = -1;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  pdn_cntxt_p = (ds_pdn_context_s*)
                (bearer_cntx_p->ds_bearer_context_dyn_p->pdn_context_ptr);

  if (! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntxt_p,&subs_id))
  {
    *ps_errno = DS_EINVAL;

    ds3gpp_leave_global_crit_section();
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section); 
    ret_val = -1;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }
  /*-------------------------------------------------------------------------
    Switch on type of request
  -------------------------------------------------------------------------*/
  DS_3GPP_MSG2_HIGH_EX(subs_id, "Rcvd ds_bearer_cntxt_phys_link_ioctl_hdlr: ioctl %d bearer "
                   "cntx instance %d ",
                   ioctl_name, bearer_cntx_inst);
  switch (ioctl_name)
  {
    case PS_PHYS_LINK_IOCTL_GO_ACTIVE:      

      DS_3GPP_MSG1_LOW("Rcvd IOCTL_GO_ACTIVE for bearer cntx instance %d ",
               bearer_cntx_inst);

       /*-------------------------------------------------------------------
        If the context was dormant, bring up the physical link
      -------------------------------------------------------------------*/
      if (bearer_cntx_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP ||
          bearer_cntx_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT ||
          bearer_cntx_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)      
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "Processing GO_ACTIVE for bearer instance %d ",
                 bearer_cntx_inst);

        if(PS_PHYS_LINK_GET_STATE(phys_link_ptr) == PHYS_LINK_DOWN)
        {
          /*---------------------------------------------------------------
          Bring up the physical link.
          ---------------------------------------------------------------*/
          ret_val = ps_phys_link_up_cmd(phys_link_ptr,
                                    ps_errno,
                                    NULL);

          if (ret_val == 0)
          {
            /*-----------------------------------------------------------------
              An error occured. We just checked that phys link was down. We 
              should not have got 0 in ret_val.
            -----------------------------------------------------------------*/
            DS_3GPP_MSG1_LOW("Bringing up phys link that is already up %d",
                               (uint32)phys_link_ptr->client_data_ptr);
            ret_val = -1;
            local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
          }          
          else if (*ps_errno != DS_EWOULDBLOCK)
          {
            /*-----------------------------------------------------------------
              An error occured. End the call.
            -----------------------------------------------------------------*/
            ret_val = ps_phys_link_go_null_cmd(phys_link_ptr,
                                  &ps_errno_local,
                                  NULL);
            if (ps_errno_local != DS_EWOULDBLOCK) 
            {
              //VRK:TODO Need to check if we shd use CLOSENETINPROGRESS
              DS_3GPP_MSG2_LOW("Was expecting DS_EWOULDBLOCK (%d), got something else (%d)",
                    DS_EWOULDBLOCK,ps_errno);
              DS3GPP_ASSERT(0);  
              temp_num = *ps_errno;
              local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
            }
            ret_val=-1;
          }
          else{
            ret_val=0;
          }
        }
        else
        {
          DS_3GPP_MSG1_HIGH_EX(subs_id, "Phys link already up, send UpInd bearer ist:%d",
                    bearer_cntx_inst);
          ps_phys_link_up_ind(phys_link_ptr);

          /*-------------------------------------------------------------------
            Send PS phys link disable for the bearer if subs is coex flow
            disabled
          -------------------------------------------------------------------*/
          if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
          {
            ds_3gpp_bearer_cntxt_disable_flow(bearer_cntx_p, DS_FLOW_COEX_MASK, FALSE);
          }
        }
      }
      else
      {
        DS_3GPP_MSG1_LOW("GO ACTIVE called in wrong state %d", 
                       bearer_cntx_p->ds_bearer_context_dyn_p->state);
        *ps_errno = DS_EINVAL;
        ret_val = -1;
        temp_num = bearer_cntx_p->ds_bearer_context_dyn_p->state;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      }
      break;

    case PS_PHYS_LINK_IOCTL_RAN_ASST_MEDIA_ADAPT_UPDATE:
    {
      ret_val = ds_eps_bearer_cntxt_ex_ran_asst_media_adapt_update(
                                   phys_link_ptr, argval_ptr, subs_id, ps_errno);
      break;
    }

    case PS_PHYS_LINK_IOCTL_GET_RAN_ASSIT_MEDIA_ADAPT_INFO:
    {
      ps_phys_link_ran_asst_media_adapt_info_type   *rec_bit_rate_info;
      ds_eps_bearer_context_s                       *eps_bearer_context_p = NULL;
      if (argval_ptr == NULL)
      {
        *ps_errno = DS_EINVAL;
        ret_val = -1;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_16;
        break;
      }
      eps_bearer_context_p = (ds_eps_bearer_context_s *)
        DS_BEARER_CTXT_VOBJ(bearer_cntx_p, SYS_SYS_MODE_LTE);
      if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
      {
        *ps_errno = DS_EINVAL;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_18;
        ret_val = -1;
        break;
      }
      if(eps_bearer_context_p->ds_eps_bearer_context_dyn_p->rec_bit_rate_update)
      {
        /*The recommaneded bit rate has been updated before */
        rec_bit_rate_info = (ps_phys_link_ran_asst_media_adapt_info_type *)argval_ptr;
        memset(rec_bit_rate_info,
               0,
               sizeof(ps_phys_link_ran_asst_media_adapt_info_type));
        memscpy((void *)rec_bit_rate_info,
                sizeof(ps_phys_link_ran_asst_media_adapt_info_type),
                (void *)&eps_bearer_context_p->ds_eps_bearer_context_dyn_p->rec_bit_rate,
                sizeof(ps_phys_link_ran_asst_media_adapt_info_type));
            DS_LTE_MSG3_HIGH("bit rate indx %d, RoHC %d, direction %d", 
                     rec_bit_rate_info->bit_rate_info.bit_rate,
                     rec_bit_rate_info->is_rohc_enabled,
                     rec_bit_rate_info->bit_rate_info.direction);

        *ps_errno = DS_ENOERR;
      }
      else
      {
        *ps_errno = DS_EINVAL;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_17;
        ret_val = -1;
      }
      break;
    }
    
#ifdef FEATURE_UE_INITIATED_DORMANCY 
  case PS_PHYS_LINK_IOCTL_GO_DORMANT:
      DS_3GPP_MSG0_LOW("Rcvd PS_PHYS_LINK_IOCTL_GO_DORMANT ");      

      if((PS_PHYS_LINK_GET_STATE(phys_link_ptr) == PHYS_LINK_DOWN) &&
         (bearer_cntx_p->ds_bearer_context_dyn_p->state != 
                                 DS_BEARER_CONTEXT_STATE_UP))
      {
        DS_3GPP_MSG0_LOW("GO DORMANT when phys_link already DOWN, ");
        ret_val = 0;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_5;
        break;
      }

      /* GO_Dormant request with or without Delay timer value configured will 
              not be honored if Delay timer is already running. we can figure out this
               by checking if dynamic delay dormancy pointer is NULL or not */
      if (ds_3gpp_pdn_cntxt_validate_and_get_delay_dormancy_info_ptr(&info_p,subs_id))
      {
        DS_3GPP_MSG0_LOW("Delay Dormancy Tmr is Running, Not honoring" 
                                               " New GO_DORMANT Request ");
        *ps_errno =  DS_EINVAL;
         ret_val = -1;
         local_err = DS_3GPPI_LOCAL_ERR_VAL_6;
         break;
      }

      if(argval_ptr != NULL)
      {
        delay_timer_value = (*(uint32*)argval_ptr);
      }
      /* SOMC check if Dormancy Delay timer is configured */
      if (delay_timer_value > 0)
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "Delay Timer Included in GO_DORMANT Value: %dms",
                                                        delay_timer_value);
        /*SOMC  As delay timer is configured, start timer for that duration
                  and delay connection release */
        /*Init dynamic memory for data statistics */
        if ( NULL == ds_3gpp_pdn_cntxt_alloc_memory_for_delay_dormancy_stats())
        {
          DS_3GPP_MSG0_LOW("Dynamic memory initialization failed ");
          *ps_errno =  DS_NOMEMORY;
            ret_val = -1;
          local_err = DS_3GPPI_LOCAL_ERR_VAL_7;
            break;
        }

        if((!ds_3gpp_pdn_cntxt_validate_and_get_delay_dormancy_info_ptr(&info_p,subs_id))
            || ((info_p->timer_p =
         rex_create_timer_ex(ds_3gpp_bearer_cntxt_dormancy_delay_tmr_expiry_cb,
                            (unsigned long)phys_link_ptr))== NULL))
        {
          DS_3GPP_MSG0_LOW("Delay Dormancy Timer allocation failed ");
          *ps_errno =  DS_EINVAL;
          ret_val = -1; 
          local_err = DS_3GPPI_LOCAL_ERR_VAL_8;
          break;
        }
      
        /* Initilaize/cache current data stats before starting delay timer */
        ds_3gpp_bearer_cntxt_init_or_check_data_statistics(
                                                  DS_3GPP_INIT_DATA_STATS);
        rex_set_timer(info_p->timer_p,(rex_timer_cnt_type)delay_timer_value);
      }
      else
      {
        ret_val = ds_3gpp_bearer_cntxt_handle_go_dormant_request(subs_id,
                                                              bearer_cntx_p,
                                                              bearer_cntx_inst,
                                                              &fd_status,
                                                              ps_errno);
      }         
      break;

    case PS_PHYS_LINK_IOCTL_ABORT_GO_DORMANT:
      DS_3GPP_MSG0_LOW("Rcvd PS_PHYS_LINK_IOCTL_ABORT_GO_DORMANT ");

      if((ds_3gpp_pdn_cntxt_validate_and_get_delay_dormancy_info_ptr(&info_p,subs_id)) 
          && (0 != rex_get_timer(info_p->timer_p)))
      {
        DS_3GPP_MSG0_HIGH_EX(subs_id, "Dormancy Delay Timer running, Abort it ");
        /* Free the timer, it internally clears the timer and frees the memory  */
        rex_delete_timer_ex(info_p->timer_p);
        ds_3gpp_pdn_cntxt_free_memory_for_delay_dormancy_stats(subs_id);
      }         
      else
      {
         DS_3GPP_MSG0_LOW("Dormancy Delay Timer not running/Invalid," 
                                          "Ignoring ABORT Request");
        *ps_errno =  DS_EINVAL;
        ret_val = -1;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_9;
      }
      break;
#endif /* FEATURE_UE_INITIATED_DORMANCY */

#ifdef FEATURE_QXDM_WM_TRACING
    case PS_PHYS_LINK_IOCTL_GET_CALL_ID:
      DATA_MSG0_LOW("Call ID query");
      call_id = (uint32*)argval_ptr;
      *call_id = ds_bearer_cntxt_get_call_id(bearer_cntx_p);
      break;
#endif /* FEATURE_QXDM_WM_TRACING */
#ifdef FEATURE_DATA_FLOW_MGMT
    case PS_PHYS_LINK_IOCTL_FLOW_CTL_ACK: 
      /*Ack is needed currently only for switch scenario*/
      if(argval_ptr != NULL)
      {
        flow_ctl_ack = (*(uint8*)argval_ptr);
        ds_3gpp_burst_hdlr_post_qmap_ack_cnf_cmd(bearer_cntx_p,
                                                 flow_ctl_ack);
      }
      break;  
    case PS_PHYS_LINK_IOCTL_FLOW_CTL:
      if(argval_ptr != NULL)
      { 
        flow_ctl_type_ptr = (ps_phys_link_ioctl_flow_ctl_type  *)
          argval_ptr;
        if (flow_ctl_type_ptr->flow_ctl_req == PS_PHYS_LINK_IOCTL_FLOW_ENABLE ) 
        {
          ds_3gpp_burst_hdlr_post_flow_ctrl_cmd(bearer_cntx_p,
                                                 TRUE,
                                                 flow_ctl_type_ptr->flow_mask);
        }
        else
        {
          ds_3gpp_burst_hdlr_post_flow_ctrl_cmd(bearer_cntx_p,
                                                 FALSE,
                                                 flow_ctl_type_ptr->flow_mask);
        }
      }
      break;  
    case PS_PHYS_LINK_IOCTL_GET_NUM_ALLOWED_BYTES:
      if(argval_ptr != NULL)
      {
        /*Needs to reevaluate*/
        allowed_bytes_info_ptr = (ps_phys_link_ioctl_allowed_num_bytes_type  *)
          argval_ptr;
         allowed_bytes_info_ptr->num_of_bytes = 
          ds3g_burst_get_bytes_allowed( &(bearer_cntx_p->tx_wm.wm_item)); 
      } 
      break;  
#endif  /*FEATURE_DATA_FLOW_MGMT */
    default:
      DS_3GPP_MSG1_LOW("Invalid request from IOCTL: %d",ioctl_name);
      *ps_errno =  DS_EINVAL;
      ret_val = -1;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_10;
      break;
  }
  ds3gpp_leave_global_crit_section();
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section); 

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_4_EX(subs_id,
                       "ds_bearer_cntxt_phys_link_ioctl_hdlr: ioctl d "
                       "err d bearer_cntx_inst d temp_num d",
                       ioctl_name, local_err, bearer_cntx_inst, temp_num);
  }

  return ret_val;  
} /* ds_bearer_cntxt_phys_link_ioctl_hdlr() */

/*===========================================================================
FUNCTION DSUMTSPS_INITIALIZE_L2_WMS

DESCRIPTION
   This function initializes the SNDCP/PDCP UL and DL watermarks for a call
   instance.
  
PARAMETERS
  bearer_context_p - Pointer to the bearer instance to work on.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
LOCAL void ds_bearer_cntxt_initialize_l2_wms
(
  ds_bearer_context_s * bearer_context_p
)
{
  int dne_value = (int) 0xFFFFFFFF;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(bearer_context_p == NULL) 
  {
    DS_3GPP_ERROR_LOG_0("Bearer Context passed is invalid");
    return;
  }

  dsm_queue_init(&(bearer_context_p->tx_wm.wm_item),
                 dne_value,  /* No MAX queue size */
                 &(bearer_context_p->tx_wm.wm_queue));

  dsm_queue_init ( &(bearer_context_p->rx_wm.wm_item),
                   dne_value,  /* No MAX queue size */
                   &(bearer_context_p->rx_wm.wm_queue));
  dsm_queue_init ( &(bearer_context_p->lte_rx_wm.wm_item),
                   dne_value,  /* No MAX queue size */
                   &(bearer_context_p->lte_rx_wm.wm_queue));
  dsm_queue_init ( &(bearer_context_p->tlb_rx_wm.wm_item),
                   dne_value,  /* No MAX queue size */
                   &(bearer_context_p->tlb_rx_wm.wm_queue));

}/* ds_bearer_cntxt_initialize_l2_wms() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PROC_PCH_CMD

DESCRIPTION
  This function handles PCH indication from RRC.

PARAMETERS   
  *user_data - PCH indication
  sys_mode -   System mode in which PCH command is posted

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
static void ds_bearer_cntxt_proc_pch_cmd
(
  ds_3gpp_pch_event_e_type  pch_event,
  sys_sys_mode_e_type       sys_mode,
  sys_modem_as_id_e_type    subs_id
)
{
  int index = 0;
  ds_bearer_context_s     *bearer_context_p = NULL;
  ds_3gpp_fd_info_s       *report_p = NULL;
  int16                    dss_errno = 0;
  boolean                  modem_fd_indicated = FALSE;
  boolean                  modem_fd_enabled   = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG2_HIGH_EX(subs_id,
                 "PCH indication: %d RRC mode %d ",pch_event, sys_mode);

  modem_fd_enabled = ds_3gpp_bearer_cntxt_is_modem_fd_enabled();

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
  
      DS_3GPP_MSG3_LOW("PCH indication: RRC mode %d bearer inst [%d] state: %d ",
                         sys_mode, index, 
                         DS_BEARER_CONTEXT_STATE_DOWN);
    }
    else
    {
      DS_3GPP_MSG4_HIGH_EX(subs_id,
                      "PCH indication: RRC mode %d bearer inst [%d] state: %d "
                      "beaer call sys mode %d ",
                      sys_mode,
                      index,
                      (bearer_context_p->ds_bearer_context_dyn_p)->state,
                      (bearer_context_p->ds_bearer_context_dyn_p)->call_mode );

      if(((bearer_context_p->ds_bearer_context_dyn_p)->state 
                                       == DS_BEARER_CONTEXT_STATE_UP)&&
         ((bearer_context_p->ds_bearer_context_dyn_p)->call_mode
                                       == sys_mode))
      {     
        if (pch_event == DS_3GPP_EVENT_IN_PCH)
        {          
          ps_phys_link_down_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->
                                     phys_link), PS_EIC_QOS_NETWORK_L2_LINK_RELEASED );
          /*-------------------------------------------------------------------
            Register each enqueue function ptr.      
          -------------------------------------------------------------------*/
          (bearer_context_p->tx_wm.wm_item).
             each_enqueue_func_ptr =(wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
          (bearer_context_p->tx_wm.wm_item).
             each_enqueue_func_data =(void*) bearer_context_p;   

          DS_3GPP_MSG0_LOW("ds_bearer_cntxt_proc_pch_cmd: posted phys_link_down_ind");
        }
        else
        {
          if( (modem_fd_enabled == TRUE) &&
              !modem_fd_indicated &&
              (PS_PHYS_LINK_GET_STATE(&(bearer_context_p->
                      ds_bearer_context_dyn_p->phys_link)) == PHYS_LINK_DOWN) )
          {
            /*  give Indication to PS to start FD algorithm*/
            report_p = (ds_3gpp_fd_info_s*) 
                 modem_mem_alloc(sizeof(ds_3gpp_fd_info_s),MODEM_MEM_CLIENT_DATA);
  
            if (report_p != NULL)
            {
              memset(report_p, 0, sizeof(ds_3gpp_fd_info_s));
              report_p->timer_1_value = ds_3gpp_fd_info.timer_1_value;
              report_p->timer_2_value = ds_3gpp_fd_info.timer_2_value;
   
              DS_3GPP_MSG0_HIGH_EX(subs_id, "NW triggered DL data, Start FD Ind to PS");
              if (0 > ps_sys_conf_set_ex(PS_SYS_TECH_3GPP,
                                         PS_SYS_CONF_START_FAST_DORMANCY,
              (ps_sys_subscription_enum_type)ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                                         report_p,
                                         &dss_errno))
              {
                DS_3GPP_MSG0_HIGH_EX(subs_id, "Fast dormancy indication failed");
              }
     
              modem_mem_free(report_p, MODEM_MEM_CLIENT_DATA);
            }
            else
            {
             	DS_3GPP_MSG0_HIGH_EX(subs_id, "Fast dormancy indication failed");
            }

            modem_fd_indicated = TRUE;
          }

          ps_phys_link_up_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                   PS_EIC_QOS_NETWORK_L2_LINK_REESTAB_IND );
          /*-------------------------------------------------------------------
            De-register each enqueue function ptr.      
          -------------------------------------------------------------------*/
          (bearer_context_p->tx_wm.wm_item).
              each_enqueue_func_ptr = NULL;
          (bearer_context_p->tx_wm.wm_item).
              each_enqueue_func_data = NULL;
           DS_3GPP_MSG0_LOW("ds_bearer_cntxt_proc_pch_cmd: posted phys_link_up_ind");

          /*-------------------------------------------------------------------
            Send PS phys link disable for the bearer if subs is coex flow
            disabled
          -------------------------------------------------------------------*/
          if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
          {
            ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p, DS_FLOW_COEX_MASK, FALSE);
        }
      }
      }
    }//else
  }//for
}  /* ds_bearer_cntxt_proc_pch_cmd */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PROC_RRC_STATE_CMD

DESCRIPTION
  This function handles RRC state change info command

PARAMETERS   
  rrc_state_info - rrc state info Pointer

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
static void ds_bearer_cntxt_proc_rrc_state_cmd
(
  ds_3gpp_rrc_state_ind_cmd_type  *rrc_state_info
)
{
   ds3geventmgr_event_info_type         event_info;
   ds3geventmgr_filter_type             filter_info;
   boolean                              state_updated = FALSE;
   ds_bearer_cntxt_per_subs_info_type   *per_subs_info_hndl = NULL;
   ds_3gpp_pch_event_e_type             pch_event;   
   boolean                              pch_state_updated= FALSE; 
               


   if (rrc_state_info == NULL)
   {
      DS_3GPP_ERROR_LOG_0("Invalid RRC Info Pointer");
      return;
   }
 
   per_subs_info_hndl = ds_bearer_cntxt_get_per_subs_info_hndl(
                          rrc_state_info->subs_id);

   if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
   {
      DS_3GPP_ERROR_LOG_1_EX(rrc_state_info->subs_id, 
                             "Bearer Cntxt per subs info handle is invalid for"
                             "subs id",rrc_state_info->subs_id);
      return;
   }


   switch (rrc_state_info->rrc_state) 
   {
         case SYS_WCDMA_RRC_STATE_DISCONNECTED:
         case SYS_WCDMA_RRC_STATE_CELL_FACH:
         case SYS_WCDMA_RRC_STATE_CELL_DCH:
           pch_event= DS_3GPP_EVENT_OUT_OF_PCH;
           break;
         case SYS_WCDMA_RRC_STATE_CELL_PCH:
         case SYS_WCDMA_RRC_STATE_URA_PCH:
           pch_event= DS_3GPP_EVENT_IN_PCH;
           break;
   }


   if (per_subs_info_hndl->rrc_state != rrc_state_info->rrc_state) 
   {
       per_subs_info_hndl->rrc_state = rrc_state_info->rrc_state; 
       state_updated = TRUE;    
   }
   
   if (per_subs_info_hndl->pch_state != pch_event) 
   {
       per_subs_info_hndl->pch_state = pch_event; 
       pch_state_updated = TRUE;    
   }


   if (state_updated) 
   {
      memset(&event_info,0,sizeof(ds3geventmgr_event_info_type));
      memset(&filter_info, 0, sizeof(ds3geventmgr_filter_type));

      event_info.rrc_state_info.rrc_state = rrc_state_info->rrc_state;
      event_info.rrc_state_info.sys_mode =  rrc_state_info->sys_mode;

      filter_info.ps_subs_id =  (ps_sys_subscription_enum_type) 
                                    ds3gsubsmgr_subs_id_cm_to_ds
                                       (rrc_state_info->subs_id);
      filter_info.tech = PS_SYS_TECH_ALL;


      if(ds3geventmgr_set_event_info(DS3GEVENTMGR_RRC_STATE_CHANGED_EV,
                                  (void*)&event_info,
                                  &filter_info) == FALSE)
      {

        DS_3GPP_MSG0_HIGH_EX(rrc_state_info->subs_id, 
                             "Unable to set RRC State changed Event");
      }
   }

   if (pch_state_updated) 
   {
      ds_bearer_cntxt_proc_pch_cmd(pch_event,
                                   rrc_state_info->sys_mode,
                                   rrc_state_info->subs_id);
   }

   ds3gtputmgr_downlink_throughput_report_hdlr(rrc_state_info->subs_id);
   return;
}  /* ds_bearer_cntxt_proc_pch_cmd */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_TD_PROC_CMD

DESCRIPTION
  This function handles the request to update ul wm levels from
  TDRRC 

PARAMETERS
  rab_id  - Identifier used to uniquely identify a data bearer
  ul_data_rate - Updated data rate of the bearer
  subs_id      - Subscription on which this call is made

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_td_proc_cmd
(
  uint32 rab_id,
  uint32 ul_data_rate,
  sys_modem_as_id_e_type subs_id
)
{
  int index = 0;
  ds_pdn_context_s                                 *pdn_cntx_p = NULL;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type   tds_um_wm_threshold;
  ds_bearer_context_s                              *bearer_context_p = NULL;
  ds_bearer_context_wm_s                           *tx_wm_p = NULL;
  /*-----------------------------------------------------------------------*/

  DS_3GPP_MSG2_HIGH_EX(subs_id, "TD UL DATA RATE Indication rab_id %d data rate %d",
                    rab_id, ul_data_rate);


  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_context_p, subs_id))
    {

      DS_3GPP_MSG3_LOW("TD UL DATA RATE Indication Invalid BEARER: "
                       "rab_id %d data rate %d Subs Id(CM) %d",
                       rab_id, ul_data_rate,subs_id);
    }
    else
    {
      /* Get the PDN context */
      pdn_cntx_p = (ds_pdn_context_s *)
                     bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
      tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);
      tx_wm_p = &(bearer_context_p->tx_wm);
      if (bearer_context_p->ds_bearer_context_dyn_p->nsapi == rab_id)
      {
        if (ul_data_rate <= DSTPSD_UL_RATE_16)
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_16);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          DSTPSD_UL_WM_HI_16);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_16);

        }
        else if ((ul_data_rate <= DSTPSD_UL_RATE_32)&&
                 (ul_data_rate > DSTPSD_UL_RATE_16 ))
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_32);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          DSTPSD_UL_WM_HI_32);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_32);
        }
        else if ((ul_data_rate <= DSTPSD_UL_RATE_64 )&&
                 (ul_data_rate > DSTPSD_UL_RATE_32 ))
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_64);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          DSTPSD_UL_WM_HI_64);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_64);
        }
        else if ((ul_data_rate <= DSTPSD_UL_RATE_128 )&&
                 (ul_data_rate > DSTPSD_UL_RATE_64 ))
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_128);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          DSTPSD_UL_WM_HI_128);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_128);

        }
        else if ((ul_data_rate <= DSTPSD_UL_RATE_256)&&
                 (ul_data_rate > DSTPSD_UL_RATE_128))
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_256);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          DSTPSD_UL_WM_HI_256);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_256);
        }
        else if ((ul_data_rate <= DSTPSD_UL_RATE_384 )&&
                 (ul_data_rate > DSTPSD_UL_RATE_256))
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_LO_384);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                         DSTPSD_UL_WM_HI_384);
          dsm_set_dne(&tx_wm_p->wm_item,
                      DSTPSD_UL_WM_DNE_384);

        }        
        else if (ul_data_rate > DSTPSD_UL_RATE_384)
        {
          dsm_set_low_wm(&tx_wm_p->wm_item,
                         tds_um_wm_threshold.tds_ul_lo);
          dsm_set_hi_wm(&tx_wm_p->wm_item,
                          tds_um_wm_threshold.tds_ul_hi);
          dsm_set_dne(&tx_wm_p->wm_item,
                      tds_um_wm_threshold.tds_ul_dne);

        }

        DS_3GPP_MSG3_HIGH_EX(subs_id, "WM levels after update - Lo: %d, Hi: %d, DNE: %d",
                          tx_wm_p->wm_item.lo_watermark,
                          tx_wm_p->wm_item.hi_watermark,
                          tx_wm_p->wm_item.dont_exceed_cnt);
        /* Only one bearer context has the matching rab_id */
        break;
      }
      else
      {
         DS_3GPP_MSG1_LOW ("TD UL DATA RATE Indication: rab_id not found %d",
                           rab_id);
      }

    }
  }

  return;
}/*ds_bearer_cntxt_td_proc_cmd */

/*===========================================================================
FUNCTION      DS_BEARER_CNTXT_PHYS_LINK_UP

DESCRIPTION
  Brings up the UMTS traffic channel for primary and secondary contexts.

PARAMETERS
  phys_link_ptr - phys link ptr for the call
  info_ptr      - not used

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL int ds_bearer_cntxt_phys_link_up
(
  ps_phys_link_type *phys_link_ptr_i,
  void              *info_ptr
)
{
  const ps_phys_link_type    *phys_link_ptr = phys_link_ptr_i;
  ds_cmd_type                *cmd_ptr;
  uint32                      bearer_instance;
  ds_3gpp_pdn_call_info_type *call_info_ptr = NULL;
  sys_modem_as_id_e_type      subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds_3gpp_bearer_cntxt_get_subs_id_from_physlink(phys_link_ptr_i,&subs_id))
  {
    return (-1);
  }

  bearer_instance =  (uint32)(phys_link_ptr->client_data_ptr);

  DS_3GPP_MSG1_HIGH_EX(subs_id, 
          "DSUMTSPS PHYS LINK up cmd Bearer Inst:%d",bearer_instance);

  /*-------------------------------------------------------------------------
    The flags set in the call table will
    give enough information to differentiate between a secondary and a
    primary
  -------------------------------------------------------------------------*/
  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_pdn_call_info_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return (-1);
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PHYS_LINK_UP;
  call_info_ptr = (ds_3gpp_pdn_call_info_type*)cmd_ptr->cmd_payload_ptr;

  call_info_ptr->call_instance = bearer_instance;
  call_info_ptr->info_ptr = info_ptr;

    ds_put_cmd_ext(cmd_ptr);

  return(0);

} /* ds_bearer_cntxt_phys_link_up */

/*===========================================================================
FUNCTION  ds_3gpp_bearer_cntxt_pending_redial_timer_expiry_cb

DESCRIPTION
  This function will be a call back for pending redial Timer expiry and posts 
  a command to DS Task

PARAMETERS
  cb_data: Subscription Id passed as Callback Data

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_pending_redial_timer_expiry_cb
(
  unsigned long cb_data
)
{  
  sys_modem_as_id_e_type subs_id =  (sys_modem_as_id_e_type)cb_data;
  /*----------------------------------------------------------------------*/

  DS_3GPP_MSG0_HIGH_EX(subs_id, "Pending Redial Timer Expiry callback,"
                    "Posting DS_3GPP_CLR_PENDING_CALL_CMD");
  ds_bearer_cntxt_post_clr_pending_calls_cmd(subs_id);
}/* ds_3gpp_bearer_cntxt_pending_redial_timer_expiry_cb */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PER_SUBS_INFO_INIT

DESCRIPTION
  This function initializes the Bearer Context Information for the given
  subscription.
 
PARAMETERS
  subs_id : Subscriptio Id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_per_subs_info_init
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_cntxt_per_subs_info_type* per_subs_info_p = NULL;
  ds_3gppi_local_err_val_e_type       local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*-----------------------------------------------------------------------*/

  per_subs_info_p = ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  do
  {
    if (per_subs_info_p != NULL)
    {
      DS_3GPP_MSG1_LOW  ("Memory already allocated for Subs Id(CM): %d", 
                          subs_id );
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
    }
    else
    {
      per_subs_info_p = 
        (ds_bearer_cntxt_per_subs_info_type*)
          modem_mem_alloc (sizeof(ds_bearer_cntxt_per_subs_info_type),
                           MODEM_MEM_CLIENT_DATA);
      if (per_subs_info_p == NULL)
      {
        DS_3GPP_MSG1_LOW ("Memory allocation failed for Subs Id(CM): %d", 
                            subs_id );
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
        break;
      }

      per_subs_info_p->cookie = DS_BEARER_CNTXT_PER_SUBS_INFO_COOKIE;
      per_subs_info_p->subs_id = subs_id;

      per_subs_info_p->service_request_pending_flag = FALSE;
      per_subs_info_p->timer_running = FALSE;
      per_subs_info_p->pending_redial_tmr_running = FALSE;
      per_subs_info_p->uplink_data_bitmask = 0;
      per_subs_info_p->ds_3gpp_reg_in_progress = FALSE;
      per_subs_info_p->rab_reject_reason = PS_SYS_3GPP_RAB_REJECT_UNKNOWN;

      per_subs_info_p->rrc_state = SYS_WCDMA_RRC_STATE_DISCONNECTED;
      per_subs_info_p->pch_state = DS_3GPP_EVENT_OUT_OF_PCH;

      /*--------------------------------------------------------------------
        Init the parameters for data activity status notifications
      ------------------------------------------------------------------*/
      per_subs_info_p->ds_3gpp_uldl_wm_cnt.is_bearer_up = FALSE;
      per_subs_info_p->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt = 0;
      per_subs_info_p->is_sampling_timer_running = FALSE;
      per_subs_info_p->is_client_reg = FALSE;
#ifdef FEATURE_DATA_FLOW_MGMT
      per_subs_info_p->periodic_timer_running = FALSE;
#endif  /*FEATURE_DATA_FLOW_MGMT */
      /*-------------------------------------------------------------------------
        Define the timer for data activity sampling timer
      -------------------------------------------------------------------------*/
      rex_def_timer_ex
         (&(per_subs_info_p->data_activity_sampling_tmr),
         (rex_timer_cb_type) ds_bearer_cntxt_data_sampling_timer_cb,
         subs_id);

      /*-------------------------------------------------------------------------
        Define the timer for rab reestablishment
      -------------------------------------------------------------------------*/
      rex_def_timer_ex(&(per_subs_info_p->tmr_rab_reestab),
                       (rex_timer_cb_type) ds_bearer_cntxt_reorig_dorm_timer_cb,
                        subs_id);

#ifdef FEATURE_DATA_FLOW_MGMT
      rex_def_timer_ex(&(per_subs_info_p->periodic_timer),
                       (rex_timer_cb_type)ds_3gpp_burst_hdlr_periodic_timer_cb,
                       subs_id);
#endif  /*FEATURE_DATA_FLOW_MGMT */

     /* Allocating timer for cleaningup calls in Pending Redial State */
     rex_def_timer_ex
     (
       &(per_subs_info_p->pending_redial_tmr),
       (rex_timer_cb_type)ds_3gpp_bearer_cntxt_pending_redial_timer_expiry_cb,
       subs_id 
     );

     ds_bearer_cntxt_set_per_subs_info_hndl(subs_id, per_subs_info_p);
    }
  } while (0);

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2 ("per_subs_info_init: faile local_err d sub (CM): d",
                         local_err, subs_id );
  }
} /* ds_bearer_cntxt_per_subs_info_init */

#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================
FUNCTION  ds_3gpp_bearer_cntxt_init_fast_domrnacy_params

DESCRIPTION
  This function will initialize the fast dormancy paramters. 
  Enabled flag and Timer values. If GCF NV is enabled and FD 
  is running few GCF test cases will fail

PARAMETERS
  None
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_init_fast_domrnacy_params
(
  void
)
{
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*----------------------------------------------------------------*/
  ds_3gpp_bearer_cntxt_set_modem_fd_timer_1_value(
                                       DS_3GPP_FD_DEFAULT_OVERALL_TIMER_VALUE);
  ds_3gpp_bearer_cntxt_set_modem_fd_timer_2_value( 
                                     DS_3GPP_FD_DEFAULT_AGRESSIVE_TIMER_VALUE);
  /* Read GCF NV Item Status */
  if( mcfg_gcf_nv_get_status() )
  {
    /* as GCF NV is Enabled Modem Fast Dormancy should be Disabled 
       by default */
    ds_3gpp_bearer_cntxt_set_modem_fd_enabled_flag(FALSE);
  }
  else
  {
    /*  Read FD Enabled flag from EFS */
    ds3gcfgmgr_read_fast_dormancy_enabled_flag_from_efs();

    /* Read EFS and store Fast Dormancy  Timer information */ 
    if (TRUE != ds3gcfgmgr_read_fast_dormancy_info_from_efs())
    {
      /* Init to default Timer Values */
      DS_3GPP_MSG0_LOW("EFS reading failed for FD Timers, Inited to default");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    }

    DS_3GPP_MSG4_HIGH("FD_INFO:Enabled: %d, Timer_1_value: %d ms, Timer_2_value:"
                      "%d ms local code %d",
                      ds_3gpp_fd_info.is_FD_enabled,
                      ds_3gpp_fd_info.timer_1_value,
                      ds_3gpp_fd_info.timer_2_value,
                      local_err);
  }
}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_3GPP_INIT

DESCRIPTION
  This function initializes the 3GPP Packet-Switched Data mode-specific
  handlers. It is invoked during DS Task power-up.
  It creates interfaces for each pdn context and initializes the
  parameters for each pdn context.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_3gpp_init( void )
{ 
  uint32                  bearer_cntx_inst;
  ds_bearer_context_s     *bearer_context_ptr;
  nv_stat_enum_type       status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  char access_barring_rab_retry_timer_path[] = 
               "/nv/item_files/data/3gpp/access_barring_rab_retry_timer"; 

  char access_barring_rab_retry_counter_path[] = 
               "/nv/item_files/data/3gpp/access_barring_rab_retry_counter";

  
  /*--------------------------------------------------------------------------
    For each bearer in the bearer context table, initialize all the fields
    that are associated with the bearer.
  --------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_ptr = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
  
    memset( (void*)bearer_context_ptr, 0x0, (sizeof(ds_bearer_context_s)) );

    /* Initializes the SNDCP/PDCP UL and DL watermarks for each bearer 
       instance. */
    ds_bearer_cntxt_initialize_l2_wms(bearer_context_ptr);
  }

    /*------------------------------------------------------------------------:
    Read access_barring_rab_retry_timer NV setting.
  -------------------------------------------------------------------------*/
  
  status = ds3gcfgmgr_read_efs_nv(
                           access_barring_rab_retry_timer_path,
                           (void *)&access_barring_rab_retry_timer,
                           sizeof(uint16)
                          );

  if (status != NV_DONE_S)
  {
    access_barring_rab_retry_timer = 
        DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME;
  }
  else
  {
    if (access_barring_rab_retry_timer < 
        DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME )
    {
      access_barring_rab_retry_timer = 
        DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME;
    }
  }

  DS_3GPP_MSG2_HIGH("Access Barring RAB retry timer NV reading status %d "
                    "set to %d",
                    (status == NV_DONE_S),
                      access_barring_rab_retry_timer);


  /*------------------------------------------------------------------------:
    Read access_barring_rab_retry_counter NV setting.
  -------------------------------------------------------------------------*/
  
  status = ds3gcfgmgr_read_efs_nv(
                           access_barring_rab_retry_counter_path,
                           (void *) &access_barring_rab_retry_counter,
                           sizeof(uint16)
                          );

  if (status != NV_DONE_S)
  {
    access_barring_rab_retry_counter = 
      DS_BEARER_CNTXT_ACCESS_BARR_RAB_RETRY_COUNTER;
  }

  DS_3GPP_MSG2_HIGH("access_barring_rab_retry_counter NV reading status %d "
                    "set to %d",
                    (status == NV_DONE_S),
                      access_barring_rab_retry_counter);

#ifdef FEATURE_DATA_LTE

  ds3gmsgr_reg_msgr_cb(  
    LTE_RRC_DEACTIVATE_REQ,
    (ds3g_msgr_client_func_ptr)ds_bearer_cntxt_lte_deactivate_req_hdlr
  );
#endif /* FEATURE_DATA_LTE */

 ds_3gpp_bearer_cntxt_init_fast_domrnacy_params();

  /* double check if it needs to be done in init*/
  ps_dpm_um_ev_cback_reg(PS_SYS_TECH_3GPP, ds_bearer_cntxt_dpm_ev_cb);

  //ps_dpm_reg_recv_fc_cb(ds_bearer_cntxt_dpm_remote_fc_cb_fcn);

/* Register the callback here */
  ps_comp_logging_register_fetch_iface_cback(SYS_SYS_MODE_WCDMA,
                                             ds_3gpp_bearer_cntxt_comp_log_fetch_iface);

  /* Initialize the ps handles for 3GPP bearers */
  ps_handle_mgr_init_client(PS_HANDLE_MGR_CLIENT_3GPP,
                            DS_3GPP_MAX_BEARER_CONTEXT,
                            DS_3GPP_BEARER_CTXT_MIN_HANDLE,
                            DS_3GPP_BEARER_CTXT_MAX_HANDLE);

   /* Init MH data path critical sectoions */
   DS_3GPP_INIT_CRIT_SECTION( &ds_3gpp_dl_crit_sect);
   DS_3GPP_INIT_CRIT_SECTION( &ds_3gpp_ul_crit_sect);

} /* ds_bearer_cntxt_3gpp_init() */

/*=============================================================================
FUNCTION      DS_BEARER_CNTXT_DPM_EV_CB_HDLR

DESCRIPTION
  This is the call back function that is called when the Dpm events are invoked.
  Based on these events; the watermark's nonemmpty function callback is set
  reset.
PARAMETERS
  IN
    dpm_event       - The event that happened. This is used to differentiate
                      the events that happened : registration, deregistration
                      reregistration
   cback_info_ptr   - Passed in as void*, this gives us the event details 
            
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void ds_bearer_cntxt_dpm_ev_cb_hdlr
(
   ps_dpm_um_ev_enum_type                    dpm_event, 
   ps_dpm_um_ev_cback_info_type             *cback_info_ptr
)
{
  dsm_watermark_type                  *rx_wm_p = NULL; 
  dsm_watermark_type                  *irat_src_rx_wm_p = NULL;  
  ps_phys_link_type                   *phys_link_ptr = NULL;
  ds_bearer_context_s                 *bearer_context_ptr = NULL;
  int                                 loopback_index = -1;
  uint32                              phys_link_client_info = 0;
#ifdef FEATURE_DATA_EMBMS
  ds_3gpp_embms_ctrl_blk_type        *ds_3gpp_embms_blk = NULL;
#endif /*FEATURE_DATA_EMBMS*/
  ds_3gppi_local_err_val_e_type      local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type             subs_id = SYS_MODEM_AS_ID_NONE;
/*---------------------------------------------------------------------------*/

  if(cback_info_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("Event Info Pointer is Null");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  phys_link_ptr = cback_info_ptr->phys_link_ptr;
  if (!ds_3gpp_bearer_cntxt_get_subs_id_from_physlink(phys_link_ptr, &subs_id))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_6;
    goto func_exit;    
  }

#ifdef FEATURE_DATA_EMBMS
  if(cback_info_ptr->v4_iface_ptr == cback_info_ptr->v6_iface_ptr &&
     cback_info_ptr->v4_iface_ptr != NULL)
  {
    if(phys_link_ptr != NULL)
    {
      ds_3gpp_embms_blk = (ds_3gpp_embms_ctrl_blk_type *)
                                      phys_link_ptr->client_data_ptr;
      if(ds_3gpp_embms_blk != NULL)
      {
        ds_3gpp_embms_initialize_rlc_to_ps_wm (ds_3gpp_embms_blk);
      }
    }
    else
    {
      DS_3GPP_MSG0_LOW ("Invalid Phys Link Ptr passed by DPM for EMBMS");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }
    return;
  }
  else
#endif /*FEATURE_DATA_EMBMS*/
  {
    if(phys_link_ptr != NULL)
    {
      phys_link_client_info = (uint32)phys_link_ptr->client_data_ptr;
      DS_3GPP_MSG1_LOW ("Call Instance retrieved from Phys Link: %d",
                        phys_link_client_info);
      loopback_index = phys_link_client_info;
    }
    else
    {
      DS_3GPP_MSG1_LOW ("Invalid Phys Link Ptr passed by DPM, Event is %d",
                         dpm_event);
    }
    DS_3GPP_MSG2_MED_EX(subs_id, "Phys Link Ptr passed by DPM 0x%x, Event is %d",
                       phys_link_ptr, dpm_event);
  }

  /* Loopback scenario */
  if(ds_loopback_hdlr_is_instance_enabled(loopback_index))
  {
    switch(dpm_event)
    {
      case PS_DPM_UM_EV_BEARER_REG_COMPLETE:
        if(!cback_info_ptr->is_hw_accelerated)
        {          
          ds_loopback_hdlr_set_nonempty_callback(loopback_index);
        }
        break;

      case PS_DPM_UM_EV_BEARER_DEREG_COMPLETE:
        ds_loopback_hdlr_reset_nonempty_callback(loopback_index);
        break;

      case PS_DPM_UM_EV_IFACE_REG_COMPLETE:
      case PS_DPM_UM_EV_IFACE_DEREG_COMPLETE:
      case PS_DPM_UM_EV_BEARER_REREG_COMPLETE:
      default:
        DS_3GPP_MSG1_LOW("ds_bearer_cntxt_dpm_ev_cb: Un-handled event: %d."
                          " Ignoring",dpm_event);
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
    }
  }
  else
  {
    if(phys_link_ptr != NULL)
    {
      if(phys_link_client_info < DS_3GPP_MAX_BEARER_CONTEXT)
      {
        bearer_context_ptr = 
          ds_bearer_cntxt_get_instance_ptr(phys_link_client_info);
      }
              /* Regular scenario */
      if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
      {
        return;
      }
     
      /* rx_wm_p-> represents the rat associated with rat on which event
         happened ,src_wm_p -> represent the other wm; which we deregister
         in case of a rereg event */
      switch(cback_info_ptr->rat)
      {
        case DS_SYS_RAT_EX_3GPP_LTE:
            rx_wm_p = &(bearer_context_ptr->lte_rx_wm.wm_item);
            irat_src_rx_wm_p = &(bearer_context_ptr->rx_wm.wm_item);
           break;

         case DS_SYS_RAT_EX_3GPP_GERAN:
         case DS_SYS_RAT_EX_3GPP_WCDMA:
         case DS_SYS_RAT_EX_3GPP_TDSCDMA:
           rx_wm_p = &(bearer_context_ptr->rx_wm.wm_item);
           irat_src_rx_wm_p = &(bearer_context_ptr->lte_rx_wm.wm_item);
           break;

         default:
            DS_3GPP_MSG1_LOW("Unhandled rat: %d. Ignoring",cback_info_ptr->rat);
            local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
         break;
      }
    }

    if((rx_wm_p == NULL) ||
       (irat_src_rx_wm_p == NULL))
    {
       DS_3GPP_MSG0_LOW("rx_wm_p or irat_src_rx_wm_p is NULL");
       local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
       goto func_exit;
    }

    switch(dpm_event)
    {
      case PS_DPM_UM_EV_BEARER_REG_COMPLETE:

        if(!cback_info_ptr->is_hw_accelerated)
        {
          rx_wm_p->non_empty_func_ptr = 
               (wm_cb_type)ds_3gpp_bearer_cntxt_rx_data_at_u_pdp_cb;
          rx_wm_p->non_empty_func_data = (void*) bearer_context_ptr;
        }  
        break;

      case PS_DPM_UM_EV_BEARER_REREG_COMPLETE:
        if(!cback_info_ptr->is_hw_accelerated)
        {
           rx_wm_p->non_empty_func_ptr = 
                 (wm_cb_type)ds_3gpp_bearer_cntxt_rx_data_at_u_pdp_cb;
           rx_wm_p->non_empty_func_data = (void*) bearer_context_ptr;
        }
        break;

      case PS_DPM_UM_EV_BEARER_DEREG_COMPLETE:
      case PS_DPM_UM_EV_IFACE_REG_COMPLETE:
      case PS_DPM_UM_EV_IFACE_DEREG_COMPLETE:
        DS_3GPP_MSG1_HIGH_EX(subs_id, "Un-handled event: %d. Ignoring",dpm_event);
        break;  

#ifdef FEATURE_LAPP_UFS      
      #error code not present
#endif /* FEATURE_LAPP_UFS */
      default:
        DS_3GPP_MSG1_LOW("Un-expected event: %d. Ignoring",dpm_event);
        local_err = DS_3GPPI_LOCAL_ERR_VAL_5;
        break;
    }
  }
func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, "ds_bearer_cntxt_dpm_ev_cb: dpm event d err d",
                       dpm_event, local_err);
  }
  return;
} /* ds_bearer_cntxt_dpm_ev_cb_hdlr */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_REGISTER_PDN_CONTEXT

DESCRIPTION
  This fucntion is called by PDN context to register itself with the bearers
  associated with it. This function attaches the PDN, bearer contexts and the
  phys links associated with them. The bearer callbacks to the PDN context is
  also stored in the bearer context. This function is called per bearer.

PARAMETERS
  IN
    pdn_context_p     - pointer to the pdn context stored as void* in bearer.
    bearer_context_p  - pointer to bearer context
    pdn_context_cb_tbl_p
                      - pointer to the call call back table. Stored in bearer
                        to be called later when bearer goes up / down
    is_default        - Boolean that tells if the bearer is default or not.
DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_register_pdn_context
( 
  void                             *pdn_context_ptr,  
  ds_bearer_context_s              *bearer_context_ptr,
  boolean                          is_default
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == FALSE ||
      ds_bearer_cntx_validate_bearer_context(bearer_context_ptr) == FALSE )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  /*---------------------------------------------------------------------------
    Set the is_default flag of the bearer based on the argument passed into the
    function.
  ---------------------------------------------------------------------------*/
  bearer_context_ptr->ds_bearer_context_dyn_p->is_default = is_default;   

  /*---------------------------------------------------------------------------
    Initialize the other fields of the bearer context - like the phys link,
    pdn_context call back table pointer, pdn_context.
  ---------------------------------------------------------------------------*/

  bearer_context_ptr->ds_bearer_context_dyn_p->pdn_context_ptr = pdn_context_ptr;
  bearer_context_ptr->ds_bearer_context_dyn_p->rm_flow_enabled = TRUE; 
  return;
} /* ds_bearer_cntxt_register_pdn_context() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_DEREGISTER_PDN_CONTEXT

DESCRIPTION
  This fucntion is called by PDN context to deregister itself with the bearer
  associated with it. This function detaches the PDN, bearer contexts and the
  phys links associated with them. This function is called per bearer.

PARAMETERS
  IN
    bearer_context_p  - pointer to bearer context that is to be updated.

DEPENDENCIES
  None.

RETURN VALUE
  void.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_deregister_pdn_context( 
  ds_bearer_context_s              *bearer_context_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_bearer_cntx_validate_bearer_context(bearer_context_ptr) == FALSE)
  {
    DS3GPP_ASSERT(0);
    return;
  }

  /*---------------------------------------------------------------------------
    Initialize the other fields of the bearer context - like the phys link,
    pdn_context call back table pointer, pdn_context.
  ---------------------------------------------------------------------------*/
  bearer_context_ptr->ds_bearer_context_dyn_p->pdn_context_ptr = NULL;
  
  if(bearer_context_ptr->ds_bearer_context_dyn_p->is_default == TRUE)
  {
    /*-----------------------------------------------------------------------
      Enable Bridge events if it is the default bearer.
      We want only the default bearer to act on flow control on RM phys link
    -----------------------------------------------------------------------*/
    //ps_phys_link_free_event_cback_buf(bearer_context_ptr->ds_bearer_context_dyn_p->rm_flow_enable_buf_ptr);
    //bearer_context_ptr->ds_bearer_context_dyn_p->rm_flow_enabled = TRUE;
  }

  /*---------------------------------------------------------------------------
    Set the is_default flag of the bearer to TRUE
  ---------------------------------------------------------------------------*/
  bearer_context_ptr->ds_bearer_context_dyn_p->is_default = FALSE;

  return;
} /* ds_bearer_cntxt_deregister_pdn_context() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_BEARER_FROM_CALL_ID

DESCRIPTION
  This function is used to find the bearer context, given a CM call id

PARAMETERS
  IN
    cm_call_id_type - CM call ID

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the bearer context if there is a CM call ID match
  NULL if there is no match

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_bearer_from_call_id
(
  cm_call_id_type call_id
)
{
  int                    bearer_cntx_inst;
  ds_bearer_context_s    *bearer_context_p = NULL;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(bearer_cntx_inst = 0;\
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;\
      bearer_cntx_inst++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if( ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p) 
        && bearer_context_p->ds_bearer_context_dyn_p->cm_call_id == call_id)
    {
      DS_3GPP_MSG1_LOW("Found the bearer context with CM call id: %d",call_id);
      break;
    }
  }
  /*---------------------------------------------------------------------------
    If we are not able to find a match based on the call id, the following
    will be printed. NULL will be returned since bearer_context_p is NULL
  ---------------------------------------------------------------------------*/
  if(bearer_cntx_inst >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_MSG1_LOW("Cannot find the requested bearer context with call id: %d",call_id);
    bearer_context_p = NULL;
  }

  (void)ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  DS_3GPP_MSG3_HIGH_EX(subs_id, 
                    "ds_bearer_cntxt_get_bearer_from_call_id: Found bearer "
                    "context status %d bearer cntx inst %d subs id(cm): %d",
                    (bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT),
                    bearer_cntx_inst, call_id);

  return bearer_context_p;
} /* ds_bearer_cntxt_get_bearer_from_call_id() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_BEARER_CNTXT

DESCRIPTION
  This function is used to get the bearer context from the call info. It
  gets the call instance from the call info and returns a pointer to the
  bearer context.

PARAMETERS
  IN
    const void* mode_specific_info_p - This is type cast into the call info

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the bearer context if the call id is valid.
  NULL otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_bearer_cntxt
(
  const void* mode_specific_info_p
)
{
  int                      bearer_cntx_inst;
  ds_bearer_context_s     *bearer_context_p = NULL;
  sys_modem_as_id_e_type   subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( bearer_cntx_inst = 0;\
       bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if( ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p) && 
        bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr ==\
         (void*)((ds_3gpp_pdn_call_info_type*)mode_specific_info_p)->call_instance)
    {
      DS_3GPP_MSG1_LOW("Found the bearer context with instance id: %d",\
          ((ds_3gpp_pdn_call_info_type*)mode_specific_info_p)->call_instance);
      break;
    }
  }
  if(bearer_cntx_inst >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_MSG1_LOW("Cannot find the requested bearer context with instance: %d",\
        ((ds_3gpp_pdn_call_info_type*)mode_specific_info_p)->call_instance);
    bearer_context_p = NULL;
  }
  
  (void)ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
    
  DS_3GPP_MSG2_HIGH_EX(subs_id, "ds_bearer_cntxt_get_bearer_cntxt: Found bearer "
                    "context status %d bearer cntx inst %d",
                    (bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT),
                    bearer_cntx_inst);

  return bearer_context_p;
} /* ds_bearer_cntxt_get_bearer_cntxt() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_UPDATECALLID 

DESCRIPTION
  This function finds the bearer context based on the call info pointer
  passed into it and updates it with the call id. This function is called
  after we initiate a call with CM. CM gets back to 3gpp handler with
  call_id handler and this function is inturn called by 3gpp hdlr to
  update the bearer with the CM call id

PARAMETERS
  IN
    mode_specific_info_p - call info to identify the bearer context
    call_id              - CM call id to update the bearer with

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the bearer was found and the call Id was stored in it.
  FALSE if the bearer could not be found

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_bearer_cntxt_update_call_id
( 
  const void      *mode_specific_info_p,
  cm_call_id_type call_id
)
{
  ds_bearer_context_s *bearer_context_p;
  boolean             retVal = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS3GPP_ASSERT(mode_specific_info_p != NULL);
  bearer_context_p = ds_bearer_cntxt_get_bearer_cntxt(mode_specific_info_p);

  //If we are able to locate the bearer context, update it with the call id from CM
  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = call_id;
    retVal = TRUE;
  }
  else
  {
    DS_3GPP_MSG1_HIGH("Cannot locate the bearer context corresponding to the call inst :%d",\
        ((ds_3gpp_pdn_call_info_type*)mode_specific_info_p)->call_instance);
    retVal = FALSE;
  }
  return retVal;
}/* ds_bearer_cntxt_updateCallId() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_IS_DEFAULT

DESCRIPTION
  This function is an accessor function that tells if a given bearer context
  is default or not.

PARAMETERS
  IN
    bearer_context_p - pointer to the bearer context that we are interested in

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the bearer is default
  FALSE if the bearer is not default.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_is_default(ds_bearer_context_s* bearer_context_p)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  return bearer_context_p->ds_bearer_context_dyn_p->is_default;
} /* ds_bearer_cntxt_is_default() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_IS_DEFAULT_CALL_ID
DESCRIPTION
  This function is an accessor function that tells if a given call id maps to 
  default bearer or not.

PARAMETERS

    call ID  - call ID  that we are interested in

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the bearer is default
  FALSE if the bearer is not default.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_is_default_call_id(cm_call_id_type   call_id)
{

  ds_bearer_context_s* bearer_context_p;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  bearer_context_p = ds_bearer_cntxt_get_bearer_from_call_id(call_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  return bearer_context_p->ds_bearer_context_dyn_p->is_default;
} /* ds_bearer_cntxt_is_default() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GETPDNCNTXT

DESCRIPTION
  This function is used to get the PDN context from the bearer context.

PARAMETERS
  IN
    bearer_context_p - Pointer to the bearer context.

DEPENDENCIES
  None.

RETURN VALUE
  Void* - Pointer to the PDN context on Success
          NULL on failure

SIDE EFFECTS
  None.

===========================================================================*/
void* ds_bearer_cntxt_get_pdn_cntxt(ds_bearer_context_s* bearer_context_p)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return NULL;
  }

  return bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
} /* ds_bearer_cntxt_getPdnCntxt() */

#if 0 //unused so far
/* Function Header */
ds_umts_umts_qos_params_type* ds_bearer_cntxt_get_umts_qos_val
(
  ds_bearer_context_s* bearer_context_p
)
{
  return ( bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl
  [DS_3GPP_UMTS_VOBJ] == NULL? NULL:\
    &( ((ds_umts_bearer_context_s*)bearer_context_p->ds_bearer_context_dyn_p
    ->vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ])->\
    umts_qos_val) );
}

/* Function Header */
ds_umts_gprs_qos_params_type* ds_bearer_cntxt_get_gprs_qos_val
(
  ds_bearer_context_s* bearer_context_p
)
{
  return ( bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl
  [DS_3GPP_UMTS_VOBJ] == NULL? NULL:\
    &( ((ds_umts_bearer_context_s*)bearer_context_p->ds_bearer_context_dyn_p
    ->vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ])->\
    gprs_qos_val) );
}
#endif //#if 0

/*===========================================================================
FUNCTION  DS_3GPP_REPORT_BEARER_THROUGHPUT_STATS

DESCRIPTION 
 This function fills bearer throughput stats for the incoming bearer context

PARAMETERS 
  ds_bearer_context_s*                       - Pointer to the bearer context
 
  ps_sys_bearer_throughput_info_type*        - Pointer to fill in the bearer
                                               throughput stats
 
DEPENDENCIES 
  This utility function will be used only when
  * The bearer context is valid
        &
  * Throughput Info Indication timer is running
 
RETURN VALUE
  TRUE   - If the bearer throughput stats for the given bearer is reported
            successfully
 
  FALSE  - Otherwise
            
SIDE EFFECTS 
  None

===========================================================================*/
boolean ds_3gpp_report_bearer_throughput_stats
(
  ds_bearer_context_s                         *bearer_context_p,
  ps_sys_bearer_throughput_info_type          *bearer_info_p

)
{
  boolean                      ret_val              = FALSE;
#ifdef FEATURE_DATA_LTE
  ds_eps_bearer_context_s     *eps_bearer_cntxt_p   = NULL;
#endif /* FEATURE_DATA_LTE */
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*------------------------------------------------------------------------- 
    Validate the arguments
    -------------------------------------------------------------------------*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return ret_val;
  }

  if (bearer_info_p == NULL)
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "bearer Info ptr is NULL");
    return ret_val;
  }

  DS_3GPP_MSG2_MED_EX(subs_id,
                    "Bearer context 0x%x Bearer call mode %d",bearer_context_p,
                    bearer_context_p->ds_bearer_context_dyn_p->call_mode);
  /*-------------------------------------------------------------------------- 
    Use EPS bearer ID if the bearer call mode is LTE otherwise use 'nsapi'
    as the bearer ID
    --------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_LTE
  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE)
  {
    eps_bearer_cntxt_p = (ds_eps_bearer_context_s *)
                       (bearer_context_p->ds_bearer_context_dyn_p->
                        vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);
    /*----------------------------------------------------------------------- 
      validate EPS bearer context
      -----------------------------------------------------------------------*/
    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_cntxt_p))
    {
      return FALSE;
    }
    /*---------------------------------------------------------------------- 
      Bearer call mode LTE. Fill EPS bearer ID as the bearer ID
      ----------------------------------------------------------------------*/
    bearer_info_p->bearer_rlp_mac_id        = eps_bearer_cntxt_p->
                                              ds_eps_bearer_context_dyn_p->
                                              eps_bearer_id;
  }
  else
#endif /* FEATURE_DATA_LTE */
  {
    /*---------------------------------------------------------------------- 
      Bearer call mode UMTS. Fill nsapi as the bearer ID
      ----------------------------------------------------------------------*/
    bearer_info_p->bearer_rlp_mac_id        = bearer_context_p->
                                              ds_bearer_context_dyn_p->
                                              nsapi;
  }
  /*------------------------------------------------------------------------- 
    Get the latest bearer throughput stats which are cached in the
    bearer control block
    -------------------------------------------------------------------------*/
  bearer_info_p->bearer_uplink_actual_rate  = (uint32)bearer_context_p->
                                              ds_bearer_context_dyn_p->
                                              bearer_throughput_stats.actual_ul_rate;
  bearer_info_p->bearer_uplink_queue_size   = bearer_context_p->rx_wm.wm_item.current_cnt;
  bearer_info_p->is_default_bearer          = bearer_context_p->
                                              ds_bearer_context_dyn_p->
                                              is_default;
  ret_val = TRUE;

  return ret_val;
}
/*===========================================================================
FUNCTION  DS_3GPP_REPORT_BEARER_THROUGHPUT_STATS_FOR_PDN

DESCRIPTION 
 This utility function reports the bearer throughput statistics for all the
 bearers associated with the given PDN

PARAMETERS 
  
  ds_pdn_context_s*                        - Pointer to PDN context 
 
  ds_sys_bearer_throughput_info_list_type* - Pointer to fill in the
                                             the bearer throughput stats
                                             for all the bearers associated
                                             with the PDN context
 
DEPENDENCIES 
  This utility function will be used only when
  * There is atleast one bearer associated with the PDN
        &
  * Throughput Info Indication timer is running
 
RETURN VALUE
  TRUE   - If the bearer throughput stats is reported successfully for
           the Input PDN
 
  FALSE  - Otherwise
            
SIDE EFFECTS None

===========================================================================*/
boolean ds_3gpp_report_bearer_throughput_stats_for_pdn
(
  ds_pdn_context_s                          *pdn_context_p,
  ds_sys_bearer_throughput_info_list_type   *bearer_info_list_p
)
{
  uint8                               index                   = 0;
  boolean                             ret_val                 = FALSE;
  ds_bearer_context_s                *bearer_cntxt_p          = NULL;
  uint8                               bearer_info_list_index  = 0;
  ds_sys_bearer_throughput_info_type *bearer_info_p          = NULL;
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------- 
    Validate the input arguments
    -------------------------------------------------------------------------*/
  if (!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_context_p,&subs_id))
  {
    return ret_val;
  }

  if (bearer_info_list_p == NULL)
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Bearer Info List NULL. Not reporting bearer list");
    return ret_val;
  }
  /*------------------------------------------------------------------------- 
    Fill in bearer throughput statistics for all active bearers one bearer
    at a time
    -------------------------------------------------------------------------*/
  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;index++)
  {
    bearer_cntxt_p =(ds_bearer_context_s*)pdn_context_p->ds_pdn_context_dyn_p
                                            ->bearer_context_ptr_tbl[index];
    bearer_info_list_index = bearer_info_list_p->num_bearers;
    bearer_info_p = &bearer_info_list_p->
                                    bearer_trpt_info[bearer_info_list_index];
    /*----------------------------------------------------------------------- 
      Validate Bearer context
      -----------------------------------------------------------------------*/
    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_cntxt_p))
    {
      continue;
    }
    /*----------------------------------------------------------------------- 
      Get the bearer throughput statistics for the current bearer
      -----------------------------------------------------------------------*/
    if (TRUE==ds_3gpp_report_bearer_throughput_stats(bearer_cntxt_p,bearer_info_p))
    {
      bearer_info_list_p->num_bearers++;
    }
  }
  /*--------------------------------------------------------------------------- 
    Return True if there is atleast one block filled in the bearer info list
    ---------------------------------------------------------------------------*/
  if (bearer_info_list_p->num_bearers > 0)
  {
    ret_val = TRUE;
  }

  DS_3GPP_MSG2_MED_EX(subs_id, "%d Bearer(s) associated with PDN 0x%x",
                   bearer_info_list_p->num_bearers,pdn_context_p);
  return  ret_val;
}
/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CNTXT_INVOKE_BEARER_BRINGUP

DESCRIPTION
  This function calls the bearer bring_up function. It also cleans up the
  bearer and calls bearer_down_ind_cb() if bringup fails.

PARAMETERS
  bearer_instance  - call index to be able to locate the bearer
  info_ptr       - ptr to network_params_info_type

DEPENDENCIES
  Assumes bearer is already allocated.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_invoke_bearer_bringup
(
  uint32              bearer_instance,
  void                *info_ptr
)
{
  boolean                     useDefaultDialString = FALSE;
  ds_3gpp_pdn_call_info_type     call_info;
  boolean                     retVal = DS3G_SUCCESS;
  ds_3gpp_down_reason_t       net_down_reason;
  ds_bearer_context_s         *bearer_context_p;
  ds_pdn_context_s            *pdn_cntx_p;
  ds_bearer_flow_manager_s    *bflow_manager_p = NULL;  
  ds_flow_context_s           *flow_context_p = NULL;
#ifdef FEATURE_DATA_TE_MT_PDP
  cm_call_id_type             call_id = CM_CALL_ID_INVALID;
#endif /* FEATURE_DATA_TE_MT_PDP */
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;
  ds_3gppi_local_err_val_e_type  local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*-----------------------------------------------------------------------*/

  DS3GPP_ASSERT(bearer_instance < DS_3GPP_MAX_BEARER_CONTEXT);
  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/
  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;

  }
  pdn_cntx_p = (ds_pdn_context_s*) bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

  if(!(ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p, &subs_id)))
  {
     return;
  }

  /*-------------------------------------------------------------------------
    Save the call instance to pass to ds3g_msh_initiate_call(). This will
    be used in the dsumtsps_orig_call_hdlr() to determine which call
    was being originated.
  -------------------------------------------------------------------------*/

  call_info.call_instance    = bearer_instance;
  call_info.info_ptr         = info_ptr;
  call_info.mt_flag          = FALSE;

  /* new UE initiated call */
  useDefaultDialString = ds_pdn_cntxt_use_default_dial_string( pdn_cntx_p,
                                                          info_ptr );
#ifdef FEATURE_DATA_TE_MT_PDP
    /* Check if call orig is due to a MT request */
    if ( (call_id = dsumtsps_mt_check_call_orig(pdn_cntx_p)) 
         != CM_CALL_ID_INVALID )
    {
      DS_3GPP_MSG3_HIGH_EX(subs_id, "MT call orig, pdn_cntxt: %x, bearer_inst: %d, cm_call_id: %d",
                         pdn_cntx_p, bearer_instance, call_id);
      call_info.mt_flag          = TRUE;
      /* call_info.user_data        = (void *)call_id; */
    }
#endif /* FEATURE_DATA_TE_MT_PDP */
  retVal = ds_bearer_cntxt_bring_up(info_ptr,
                                  &call_info,
                                    useDefaultDialString,
                                    subs_id);

  if(retVal != DS3G_SUCCESS)
  {

    DS_3GPP_MSG1_LOW("Error bringing up Bearer Context for Call Inst %d",\
               bearer_instance);

    ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                             DS_FLOW_UMTS_MASK, FALSE);

    if (bearer_context_p->ds_bearer_context_dyn_p->is_default)
    {
      /*--------------------------------------------------------------------
        Error in initiating a primary call.
        Make sure that we call flow go null ind on default flows of all 
        ifaces associated with the PDN context
        Set the Bearer context to be in down state and call the bearer down
      call back to the PDN context.
      --------------------------------------------------------------------*/
      ds_pdn_cntxt_unbind_default_flows(bearer_context_p->ds_bearer_context_dyn_p
                                        ->pdn_context_ptr);        
      
      /* VRK:TODO This should not be under is_default */
      ps_phys_link_gone_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
      ds_bearer_cntxt_free_bearer_inst(bearer_context_p);
    }//default bearer
    else
    {
      DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
      /*-------------------------------------------------------------------------
        Update precedence_bit_mask and Release flow _context
        Bearer_down will release the bflowmgr and flowMgr          
      -------------------------------------------------------------------------*/    

      bflow_manager_p=ds_bearer_flow_manager_find_flow_manager(bearer_context_p);
      if(ds_bearer_flow_manager_validate_flow_manager(bflow_manager_p))
      {
        /*-----------------------------------------------------------------------
          There should be only one pending flow context in the bearer flow 
          manager list.
        -----------------------------------------------------------------------*/
        if(list_size(&(bflow_manager_p->ds_bearer_flow_manager_dyn_p->flow_context)) 
           != 1)
        {
          DS_3GPP_MSG0_LOW("Found more than one pending flow context");
          local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
        }
        flow_context_p = list_peek_front(&(bflow_manager_p->
                                           ds_bearer_flow_manager_dyn_p
                                           ->flow_context));          

      }
      else
      {
        DS_3GPP_MSG0_LOW("Cound not find associated bearer flow manager to clean up");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      }
      if(flow_context_p != NULL)
      {
        if(ds_flow_manager_update_precedence_bit_mask(
                        pdn_cntx_p,flow_context_p,FALSE,FALSE,TRUE)==FALSE)
        {
          DS_3GPP_MSG0_LOW("Error in clearing precedence bit mask");
          local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        }     
      }
      else
      {
        DS_3GPP_MSG0_LOW("Cound not find a pending flow context");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      }               
      ds_flow_context_release_flow_context(flow_context_p);
      DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
    }//dedicated bearer  

    DS_3GPP_MSG4_HIGH_EX(subs_id,
          "Error bringing up Bearer Context for Call Inst %d is_default %d"
          " bearer flow mgr 0x%x local code %d",
          bearer_instance,
          bearer_context_p->ds_bearer_context_dyn_p->is_default,
          bflow_manager_p,
          local_err);
  
    /*-----------------------------------------------------------------------
      We have already cleaned up at the bearer level. We should send
      bearer down indication to PDN context with the boolean call_was_conn
      set to False.
      This way the PDN context will be freed up as well.
    -----------------------------------------------------------------------*/
    if (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr-> // clang_sa_ignore [core.NullDereference]
        bearer_down_ind_cb != NULL)
    {             
      net_down_reason.reason.ps_iface_down_reason = 
                                          PS_NET_DOWN_REASON_VALIDATION_FAILURE;
      net_down_reason.reason_T = DS_3GPP_PS_IFACE_NET_DOWN_REASON_T;
      bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr->
        bearer_down_ind_cb(
                    bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,\
                  bearer_context_p,
                   net_down_reason,
                FALSE);
    }
  }
  else
  {
    /*------------------------------------------------------------------------
      Move the state of the bearer to COMING UP
    ------------------------------------------------------------------------*/
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_COMING_UP);

  }
}
/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PHYS_LINK_UP_CMD_HDLR

DESCRIPTION
  Called in DS TASK context in response to a phys up command on
  the UMTS iface

  This function handles both the reorigination from dormancy as well as new
  call bring up. The difference between these two cases is identified by the
  dormant_reorig flag.

PARAMETERS
  call_instance  - call index to be able to locate the bearer
  info_ptr       - Not used.
  dormant_reorig - if this is reorig due to dormancy

DEPENDENCIES
  Called in DS TASK context.  Assumes that PS_IFACE has already
  set phys_link state to COMING_UP.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void ds_bearer_cntxt_phys_link_up_cmd_hdlr
(
  uint32      bearer_instance,
  void        *info_ptr
)
{
  ps_iface_type               *iface_v4_ptr = NULL; /* v4 Interface for call */
#ifdef FEATURE_DATA_PS_IPV6
  ps_iface_type               *iface_v6_ptr = NULL; /* v6 Interface for call */
#endif
  ds_pdn_context_s*           pdn_cntx_p = NULL;
  ds_bearer_context_s         *bearer_context_p;
  boolean                     valid_iface_state = FALSE;
  ds_umts_pdp_type_enum_type  pdn_pdp_type;
  boolean                     dormant_reorig;
  sys_modem_as_id_e_type      subs_id  = SYS_MODEM_AS_ID_NONE;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
 ds_eps_tlb_per_subs_info_type*   eps_tlb_hndl = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  DS3GPP_ASSERT(bearer_instance < DS_3GPP_MAX_BEARER_CONTEXT);
 
  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/

  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  /* Get the PDN context */
  pdn_cntx_p = (ds_pdn_context_s*)
                   (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr);

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
     return;
  }

  DS_3GPP_MSG2_HIGH_EX(subs_id, "PHYS LINK UP CMD HDLR. Bearer state: %d, Bearer instance %d",
                    bearer_context_p->ds_bearer_context_dyn_p->state, 
                    bearer_instance);
  /* Post phys link up indication if we are in PCH state  */
  if ((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP) &&
          PS_PHYS_LINK_GET_STATE(&(bearer_context_p->ds_bearer_context_dyn_p->
                                                       phys_link)) != PHYS_LINK_UP) 
  { 
    DS_3GPP_MSG0_HIGH_EX(subs_id, "ds_bearer_cntxt_phys_link_up_cmd_hdlr: in PCH state ");
     ps_phys_link_up_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                   PS_EIC_QOS_NETWORK_L2_LINK_REESTAB_IND );
    /*-------------------------------------------------------------------
     De-register each enqueue function ptr.      
    -------------------------------------------------------------------*/
    (bearer_context_p->tx_wm.wm_item).
            each_enqueue_func_ptr = NULL;
    (bearer_context_p->tx_wm.wm_item).
            each_enqueue_func_data = NULL;
     DS_3GPP_MSG0_MED_EX(subs_id,
      "ds_bearer_cntxt_proc_pch_cmd: posted phys_link_up_ind");

    /*-------------------------------------------------------------------
      Send PS phys link disable for the bearer if subs is coex flow
      disabled
    -------------------------------------------------------------------*/
    if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
    {
        ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p,
                                          DS_FLOW_COEX_MASK, 
                                          FALSE);
    }
    /*--------------------------------------------------------------------- 
      if a PDN is in COMING_UP state, Flow mgr would be in ORIG state.
      Posting ACTIVE on the other PDN would result in starting the 
      flow monitor timer which could lead to data priority being lowered
      before the PDN is connected.
    ---------------------------------------------------------------------*/
    if (!dsPdnContext_is_any_normal_pdn_coming_up_ex(subs_id))
    {
      ds3geventmgr_notify_ps_call_event_per_sub(subs_id,
                                                DS3GEVENTMGR_PS_CALL_ACTIVE_EV,
                                                PS_SYS_NETWORK_3GPP);
    }
    return; 
  }

  /* ds_bearer_cntxt_reorig_dorm calls phys_link_up_cmd to put the phys link in
     PHYS_LINK_COMING_UP state. We are ignoring the callback here */
  if(bearer_context_p->ds_bearer_context_dyn_p->state == 
     DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) 
  {
    DS_3GPP_MSG0_LOW("Phys link coming up. Ignoring DS3GPP_PHYS_LINK_UP_CMD");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  /*-------------------------------------------------------------------------
    It is possible that we enqueue a PHYS_LINK_UP command on N/W init QOS calls
    In such a case, we do not have to do any origination with CM.

    The ps_phys_link_up_cmd is called only to ensure a proper state transition
    for the phys link.
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->state == 
     DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
  {
    DS_3GPP_MSG0_LOW("Network initiated call. No need to do any mode spec handling");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }

  if(bearer_context_p->ds_bearer_context_dyn_p->state == 
     DS_BEARER_CONTEXT_STATE_ANSWER)
  {
    if(ds_bearer_cntxt_answer_call(bearer_context_p) == DS3G_SUCCESS)
    {

      /*------------------------------------------------------------------------
        Move the state of the bearer to COMING UP
      ------------------------------------------------------------------------*/
      DS_3GPP_MSG2_LOW("Call %d answered, moving bearer 0x%x to coming up state",
                        bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                        bearer_context_p);
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_COMING_UP);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    }
    goto func_exit;
  }
  /*---------------------------------------------------------------------
    If we are getting out of dormancy, call state should be up.
    We are checking the call state from the pdn context and setting the
    reorig flag accordingly.
  ---------------------------------------------------------------------*/
  
  /* It could be that Physlink UP can happen during call cleanup and may lead to
        New PDN Request which is not the expectation */
        
  if(pdn_cntx_p->ds_pdn_context_dyn_p->tear_down_in_progress == TRUE)
  {
    DS_3GPP_MSG0_LOW("Cleanup in progress, return");
    PS_PHYS_LINKI_SET_STATE(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                                                        PHYS_LINK_DOWN);
    local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
    goto func_exit;
  }
  
  if ( bearer_context_p->ds_bearer_context_dyn_p->state != DS_BEARER_CONTEXT_STATE_DOWN &&
       bearer_context_p->ds_bearer_context_dyn_p->state != DS_BEARER_CONTEXT_STATE_PEND_REDIAL ) 
  {
    dormant_reorig = TRUE;
  }
  else
  {
    dormant_reorig = FALSE;
  }

  if(dormant_reorig == FALSE)
  {
     ds_3gpp_bearer_cntxt_invoke_bearer_bringup (bearer_instance, info_ptr);
  }
  else //Getting out of dormancy
  {
    DS_3GPP_MSG0_MED_EX(subs_id, "Reoriginating from dormancy");

#ifdef FEATURE_DATA_LTE
    eps_tlb_hndl = ds_eps_tlb_get_per_subs_info_hndl(subs_id);

    if((ds_eps_tlb_validate_per_subs_info_hndl(eps_tlb_hndl)) &&
         ((eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_B) ||
          (eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_C) ))
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Exiting phys link bring up due to tlb mode b/c");
      return;
    }
#endif

    /*---------------------------------------------------------------------
    Avoid handling dormancy reorig if the bearer is not dormant
    --------------------------------------------------------------------*/
    if( bearer_context_p->ds_bearer_context_dyn_p->state != 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT &&
        bearer_context_p->ds_bearer_context_dyn_p->state != 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB )
    {
      DS_3GPP_MSG2_LOW("Bearer 0x%x is in state %d. Invalid state to "
                                 "reorig from dormancy ", bearer_context_p,
                                                    bearer_context_p->
                         ds_bearer_context_dyn_p->state);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      goto func_exit;
    }
    /*--------------------------------------------------------------------------- 
      Get the Iface from profile.
    ---------------------------------------------------------------------------*/
    pdn_pdp_type = ds_pdn_cntxt_get_pdn_context_ip_support(pdn_cntx_p);
    if(pdn_pdp_type == DS_UMTS_PDP_PPP ||
       pdn_pdp_type == DS_UMTS_PDP_IPV4)
    {
      iface_v4_ptr = 
        &(pdn_cntx_p->ds_pdn_context_dyn_p
          ->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]->ps_iface);
    }
#ifdef FEATURE_DATA_PS_IPV6
    else if(pdn_pdp_type == DS_UMTS_PDP_IPV6)
    {
      iface_v6_ptr = 
        &(pdn_cntx_p->ds_pdn_context_dyn_p
          ->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]->ps_iface); 
    }
    else if(pdn_pdp_type == DS_UMTS_PDP_IPV4V6)
    {
      iface_v4_ptr = 
        &(pdn_cntx_p->ds_pdn_context_dyn_p->
          ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]->ps_iface);
      iface_v6_ptr = 
        &(pdn_cntx_p->ds_pdn_context_dyn_p->
          ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]->ps_iface);
    }
#endif /* FEATURE_DATA_PS_IPV6 */
    else
    {
      //DS_3GPP_ERROR_FATAL("PDP type is not valid.");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_6;
      DS3GPP_ASSERT(0);
      goto func_exit;
    }

    /*---------------------------------------------------------------------
      Check if iface state is valid, if so trigger dormancy re-orig  
    ---------------------------------------------------------------------*/
    if (((iface_v4_ptr != NULL) &&
        ((ps_iface_state(iface_v4_ptr) == IFACE_UP) ||
         (ps_iface_state(iface_v4_ptr) == IFACE_ROUTEABLE)))           
#ifdef FEATURE_DATA_PS_IPV6
        ||
        ((iface_v6_ptr != NULL) &&
                          ((ps_iface_state(iface_v6_ptr) == IFACE_UP) ||
         (ps_iface_state(iface_v6_ptr) == IFACE_ROUTEABLE)))
#endif
       )
    {
      valid_iface_state = TRUE;
    }          

    if ((valid_iface_state) && 
        (bearer_context_p->ds_bearer_context_dyn_p->state != 
         DS_BEARER_CONTEXT_STATE_DOWN) &&
        (bearer_context_p->ds_bearer_context_dyn_p->state != 
         DS_BEARER_CONTEXT_STATE_GOING_DOWN))
    {   
      /*-----------------------------------------------------------------
        If we are in data suspend mode we should not reoriginate a call. We
        should disable flow on the interface and inform NAS that data is
        available.
      -----------------------------------------------------------------*/
      if (ds3gpp_get_ps_data_suspend_flag(subs_id) == TRUE)
      {
        if((!ds3gpp_get_ps_data_available_flag(subs_id)) && 
           ds3gpp_is_ps_data_avail_ind_allowed(subs_id, 
            ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_p)))
        {
          DS_3GPP_MSG1_HIGH_EX(subs_id, "phys_link_up:%d data_suspend:1 sending data_available",\
                            (uint32) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
          ds3g_msh_ps_data_available_ind(subs_id);
          ds3gpp_set_ps_data_available_flag(TRUE, subs_id);
        }

        /*-----------------------------------------------------------------
          phys link state can be other than UP/DOWN in those cases flow enable
          will not get any effect so we will check phys link state in resume and
          post post phys link up before enabling the flow.
        -----------------------------------------------------------------*/
        /*--------------------------------------------------------------
          Disable flow on the interface
        --------------------------------------------------------------*/
        ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p, DS_FLOW_UMTS_MASK, FALSE);

        return;
      }
      ds_bearer_cntxt_dormancy_reorig(bearer_context_p);
    } // if(IFACE_UP || ROUTEABLE)
    else
    {
      DS_3GPP_MSG0_LOW("Invalid Iface/Bearer context state, cannot reorig");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_7;
    }
  } // is dormant_reorig

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_bearer_cntxt_phys_link_up_cmd_hdlr: err d",
                       local_err);
  }

  return;
} /* ds_bearer_cntxt_phys_link_up_cmd_hdlr() */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_FLOW_DISABLE_TIMER_CB

DESCRIPTION
  This function is called when the flow disable timer expires. This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  unsigned long - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_bearer_cntx_flow_disable_timer_cb(unsigned long callback_data)
{
  ds_cmd_type      *cmd_ptr = NULL;
  void  	  **data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG0_HIGH("Bearer Flow Disable timer call back");

  /*--------------------------------------------------------
    This function is executed in the timer call back, timer task.
    So all we do here is to put a command to DS and exit
   -------------------------------------------------------*/
  cmd_ptr = ds_allocate_cmd_buf(sizeof(void*));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
	ASSERT(0);
	return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_FLOW_DISABLE_TMR_EXP;
  data_ptr = (void**)cmd_ptr->cmd_payload_ptr;
  *data_ptr = (void*)callback_data;

  ds_put_cmd_ext(cmd_ptr);
} /* ds_3gpp_bearer_cntx_flow_disable_timer_cb */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_PROCESS_FLOW_DISABLE_TIMER_EXP_CMD

DESCRIPTION
  This function is called when the flow disable timer expires.

PARAMETERS
  ds_bearer_context_s - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_bearer_cntx_process_flow_disable_timer_exp_cmd
(
  ds_bearer_context_s* bearer_context_p
)
{
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  ds_pdn_context_s                *pdn_cntx_p = NULL;
  ds_3gpp_iface_s                 *ds_v4_iface_ptr = NULL;
  ds_3gpp_iface_s                 *ds_v6_iface_ptr = NULL;
  boolean                          disable_flow_cntrl = FALSE;
  sys_modem_as_id_e_type           subs_id = SYS_MODEM_AS_ID_NONE;  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DS_3GPP_MSG0_LOW("in ds_3gpp_bearer_cntx_process_flow_disable_timer_exp_cmd");

  if (bearer_context_p == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }
  
  /*------------------------------------------------------------------- 
    We are here which means the flow_disable_timer has run its course
    and an appropriate event has been posted to PS. Put the timer value
    to FALSE also for that bearer
    ------------------------------------------------------------------*/
   bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running = FALSE;
  /*-------------------------------------------------------------------------
    Get the PDN cntx assigned to this bearer cntx
  -------------------------------------------------------------------------*/
  pdn_cntx_p = (ds_pdn_context_s*)
                   ds_bearer_cntxt_get_pdn_cntxt(bearer_context_p);

  if ( FALSE == ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p) )
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    goto func_exit;
  }

  /*-------------------------------------------------------------------------
    From PDN context ptr , fetch the ds v4 and v4 iface pointer
  -------------------------------------------------------------------------*/

   ds_v4_iface_ptr = DSPDNCNTXT_GET_V4_DS_IFACE_PTR(pdn_cntx_p);
   ds_v6_iface_ptr = DSPDNCNTXT_GET_V6_DS_IFACE_PTR(pdn_cntx_p);

   /*--------------------------------------------------------------------- 
     Post event to PS framework for NON NULL ifaces
     -------------------------------------------------------------------*/

   if(ds_v4_iface_ptr ==  NULL && ds_v6_iface_ptr == NULL)
   {
     DS_3GPP_MSG1_HIGH_EX(subs_id, "v4 and v6 iface are NULL. Not posting ev %d",
                       IFACE_SET_OFFLOAD_FC_EV );
     local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
     goto func_exit;
   }

  /*-------------------------------------------------------------------- 
    Value: 0: offload flow control enabled
           1: offload flow control disabled
    -------------------------------------------------------------------*/
   if(ds_v4_iface_ptr !=  NULL )
   {
     ps_iface_generic_ind(&(ds_v4_iface_ptr->ps_iface),
                          IFACE_SET_OFFLOAD_FC_EV,
			  (void *)&disable_flow_cntrl);
   }

   if(ds_v6_iface_ptr !=  NULL )
   {
     ps_iface_generic_ind(&(ds_v6_iface_ptr->ps_iface),
		          IFACE_SET_OFFLOAD_FC_EV,
			  (void *)&disable_flow_cntrl);
   }
   ds_3gpp_flow_control_offload_fc_logging(
       DS_OFFLOAD_FC_EVENT,
       (uint32)bearer_context_p,
       0,
       subs_id);

func_exit:
  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id,
                   "ds_3gpp_bearer_cntx_process_flow_disable_timer_exp_cmd"
                   "failed with err %d", local_err);
  }
}/*ds_3gpp_bearer_cntx_process_flow_disable_timer_exp_cmd*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PREPARE_FOR_TEARDOWN

DESCRIPTION
  This function is used to prepare the bearer context for teardown, Certain
  values would be set and appropriate teardown parameters will be populated.
 

PARAMETERS
  bearer_context_p - pointer to the bearer on which call is to be ended
  info_ptr         - void pointer

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS (TRUE)
  DS3G_FAILURE (FALSE) on any failure

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_prepare_for_teardown
(
  ds_3gpp_pdn_call_info_type    *mode_sp_info,
  ds_bearer_context_s        *bearer_context_p
)
{
  boolean                    retVal = FALSE;
  sys_sys_mode_e_type        network_mode;  /* mode currently in */
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return retVal;
  }
          
  DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_bearer_cntxtt Tear down call :Inst:%d",
                    (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*-------------------------------------------------------------------------
    Get the mode that we are currently operating in. Ensure that it is one
    of the 3GPP modes
  -------------------------------------------------------------------------*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return retVal;
  }
  else
  {
    network_mode = ds3gpp_get_current_network_mode(subs_id);
  }
  
  if (mode_sp_info == NULL)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "mode_sp_info is NULL");
    return retVal;
  }

   if( !DS_3GPP_IS_3GPP_CURRENT_MODE(network_mode))
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "DS 3GPP Invalid network_mode:d",network_mode);
    return retVal;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW ("Bearer Cntxt per subs info handle is invalid"
                       "for subs id",subs_id);
    return retVal;
  }

  /*-------------------------------------------------------------------------
    Memset the mode specific info that is to be sent to CM to hang up call
  -------------------------------------------------------------------------*/
  memset(mode_sp_info,0,sizeof(ds_3gpp_pdn_call_info_type));

  /*-------------------------------------------------------------------------
    Intialize the mt_flag to FALSE
  -------------------------------------------------------------------------*/
  mode_sp_info->mt_flag = FALSE;

  /*-------------------------------------------------------------------------
    Save the call instance to pass to ds3g_msh_hangup_call(). We don't use
    this at present.
  -------------------------------------------------------------------------*/
  mode_sp_info->call_instance = (uint32)bearer_context_p->ds_bearer_context_dyn_p
    ->client_data_ptr;

  /*-------------------------------------------------------------------------
   * Regardless of whether there is uplink data to be sent or not, we unset
   * the bit field in the bit mask that is used to determine if we should
   * send re-orig request on timer expiry
  -------------------------------------------------------------------------*/
  DS_BEARER_CNTXT_UNSET_BITMASK (per_subs_info_hndl, bearer_context_p);

  /*-----------------------------------------------------------------------
    Reset the rab_reestab_retry counter for this bearer.
  -----------------------------------------------------------------------*/
  DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);

  retVal = DS3G_SUCCESS;
  return retVal;
}
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_TEARDOWN

DESCRIPTION
  This function is used to bring down a call on a bearer. The iface down cmd
  is processed in the PDN context and in turn calls this function. 

PARAMETERS
  bearer_context_p - pointer to the bearer on which call is to be ended
  info_ptr         - void pointer

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS (TRUE) on Successful call hang up with CM
  DS3G_FAILURE (FALSE) on any failure

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_tear_down(ds_bearer_context_s* bearer_context_p, void *info_ptr)
{
  boolean                    retVal = FALSE;
  ds_3gpp_pdn_call_info_type    mode_sp_info;   /* Information for a call    */

  ds_bearer_flow_manager_s   *bflow_manager_p = NULL;  
  ds_flow_context_s          *flow_context_p = NULL;
  ds_pdn_context_s           *pdn_context_ptr = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  int                         temp_num = -1;
  sys_modem_as_id_e_type      subs_id = SYS_MODEM_AS_ID_NONE;  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    DS3GPP_ASSERT(0);
    return FALSE;
  }

  retVal = ds_bearer_cntxt_prepare_for_teardown(&mode_sp_info,bearer_context_p);

  if (!retVal)
  {
    DS_3GPP_MSG0_LOW("ds_bearer_cntxt_prepare_for_teardown failed");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  /*----------------------------------------------------------------------------
     Reset retval for future usage
    ----------------------------------------------------------------------------*/
  retVal = FALSE;
  
  if (ds_bearer_cntxt_tear_down_call
      (
        bearer_context_p,
        &mode_sp_info,
        info_ptr
      ) == DS3G_FAILURE )
  {
    /*-----------------------------------------------------------------------
      The call has already ended. Do local cleanup
    -----------------------------------------------------------------------*/
    DS_3GPP_MSG1_HIGH_EX(subs_id, "Call already ended Bearer Inst:%d",
                      (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
    retVal = TRUE;
  }
  else
  {
    if(bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_DOWN)
    {
      /*-----------------------------------------------------------------------
        Do not move bearer to going down state if it already in down state, this 
        can be the default bearer which is in down state when SEC PDPs are up
       -----------------------------------------------------------------------*/
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Bearer is in down state already, not moving to going down");
      pdn_context_ptr = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
      if (ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr))
      {
        if (PS_PHYS_LINK_GET_STATE(&(bearer_context_p->ds_bearer_context_dyn_p->
                                                         phys_link)) == PHYS_LINK_DOWN)
        {
          DS3GPP_ASSERT((pdn_context_ptr->ds_pdn_context_dyn_p->num_active_bearer_context) > 1);
        }
        else
        {
         /*Phys link not in down state, so this is not a QChat scenario*/
          DS_3GPP_MSG1_HIGH_EX(subs_id, "Phys link state: %d,  do local clean up", 
                     PS_PHYS_LINK_GET_STATE(&(bearer_context_p->
                                                ds_bearer_context_dyn_p->phys_link)));
          retVal = TRUE;
        }
      }
      else
      {
        DS_3GPP_MSG0_LOW("PDN Context ptr is invalid");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      }
    }
    else
    {
      DS_3GPP_MSG1_LOW("Bearer state:%d", bearer_context_p->ds_bearer_context_dyn_p->state);
      temp_num = (int)bearer_context_p->ds_bearer_context_dyn_p->state;
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                       DS_BEARER_CONTEXT_STATE_GOING_DOWN);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    }
  }

  if (retVal)
  {
    DS_3GPP_MSG1_HIGH_EX(subs_id, "Error in tearing down bearer, do local cleanup: Bearer Inst:%d",
             (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

    if(bearer_context_p->ds_bearer_context_dyn_p->is_default)
    {
      ds_pdn_cntxt_unbind_default_flows(bearer_context_p->ds_bearer_context_dyn_p
                                        ->pdn_context_ptr);

      /*---------------------------------------------------------------------
        Disable signal if this is the last phys link. 
        Each enqueue func_ptr needs to be reset when bearer inst is freed.
        This needs to be done to avoid race conditions when phys_link_gone is
        posted and flow gets enabled on the phys_link leading to each_enq_f
        called when the bearer is getting freed..
       ---------------------------------------------------------------------*/
      ds_bearer_cntxt_free_bearer_inst(bearer_context_p);

      ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                  DS_FLOW_UMTS_MASK, FALSE);
      ps_phys_link_gone_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
    }
    else
    {
      //VRK:TODO make subfunction to call from phys_link_up_cmd_hdlr as well as tear_down
      //This code is executed only for UMTS
      DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
      /*-------------------------------------------------------------------------
        Update precedence_bit_mask and Release flow _context
        Bearer_down will release the bflowmgr and flowMgr          
      -------------------------------------------------------------------------*/    

      bflow_manager_p=ds_bearer_flow_manager_find_flow_manager(bearer_context_p);
      if(ds_bearer_flow_manager_validate_flow_manager(bflow_manager_p))
      {
        /*-----------------------------------------------------------------------
          There should be only one pending flow context in the bearer flow 
          manager list.
        -----------------------------------------------------------------------*/
        if(list_size(&(bflow_manager_p->ds_bearer_flow_manager_dyn_p->flow_context)) 
           != 1)
        {
          DS_3GPP_MSG0_LOW("Found more than one pending flow context");
          local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
        }
        flow_context_p = list_peek_front(&(bflow_manager_p->ds_bearer_flow_manager_dyn_p
                                           ->flow_context));          

      }
      else
      {
        DS_3GPP_MSG0_LOW("Cound not find associated bearer flow manager to clean up");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
      }
      if(flow_context_p != NULL)
      {
        if(ds_flow_manager_update_precedence_bit_mask(
                          (ds_pdn_context_s*)bearer_context_p->
                          ds_bearer_context_dyn_p->pdn_context_ptr,
                                     flow_context_p,FALSE,FALSE,TRUE)==FALSE)
        {
          DS_3GPP_MSG0_LOW("Error in clearing precedence bit mask");
          local_err = DS_3GPPI_LOCAL_ERR_VAL_5;
        }     
      }
      else
      {
        DS_3GPP_MSG0_LOW("Cound not find a pending flow context");      
        local_err = DS_3GPPI_LOCAL_ERR_VAL_6;
      }               
      ds_flow_context_release_flow_context(flow_context_p);
      DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
    }//dedicated bearer    
    
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, "ds_bearer_cntxt_tearDown: err d temp_num d",
                       local_err, temp_num);
  }

  return (!retVal);
}/* ds_bearer_cntxt_tearDown() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_BEARER_INST_TO_REORIG

DESCRIPTION
  This function is used to get a bearer instance on which reorigination is
  to be requested. This is used in the timer_expired command processing to
  get a bearer to work on.

  This uses the bit mask to determine a bearer that is still waiting on RAB
  reestablishment and returns it to the caller.

PARAMETERS
  Subs Id: Subscription Id

DEPENDENCIES
  Depends on whether the uplink_data_bitmask is null or non-null

RETURN VALUE
  Pointer to the bearer context awaiting RAB reestab if bitmask is non null
  NULL if bitmask is NULL

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_bearer_inst_to_reorig
(
  sys_modem_as_id_e_type subs_id
)
{
  uint32               index;
  boolean              found_bearer_instance = FALSE;
  ds_bearer_context_s* bearer_context_p;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
       (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",subs_id);
    return NULL;
  }

  /*-------------------------------------------------------------------------
    Loop through all the bearers. Query the bit field corresponding to this 
    bearer in the uplink_data_bitmask. If the field is set, then return this
    bearer to the calling function.
  -------------------------------------------------------------------------*/
  for(index=0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    
    if (!ds_bearer_cntxt_validate_bearer_subscription_internal
           (bearer_context_p, subs_id))
    {
      continue;
    }

    found_bearer_instance = 
      DS_BEARER_CNTXT_QUERY_BITMASK (per_subs_info_hndl, bearer_context_p);

    if (found_bearer_instance == TRUE)
    {
      if ((bearer_context_p->ds_bearer_context_dyn_p->
           rab_reestab_retry_cnt >= ds_3gpp_nv_get_max_rab_reestab_retry_cnt(subs_id)) &&
          (per_subs_info_hndl->ds_3gpp_reg_in_progress == FALSE))
      {
        ds_bearer_cntxt_handle_max_rab_reestab_retry_cnt(bearer_context_p);
      }
      else
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_bearer_cntxt_get_bearer_inst_to_reorig "
                          "found bearer_index:%d",index);
        break;

      }
    }
  }
  
  return(index < DS_3GPP_MAX_BEARER_CONTEXT? bearer_context_p: NULL);
} /* ds_bearer_cntxt_get_bearer_inst_to_reorig() */


/*=============================================================================
FUNCTION ds_bearer_cntxt_proc_purgecmdcbhdlr

DESCRIPTION

PARAMETERS
  ds3gpp_purge_cnf_ev which consist of
  cnf_status    - true if if purge status is RLC_PURGE_REQ_SUCCESS(success )             
                   false otherwise
  bearer_context_p - bearer context pointer
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
#ifdef FEATURE_WCDMA
void ds_bearer_cntxt_proc_purgecmdcbhdlr
(
  ds_bearer_context_purge_cnf_ev_type *ds3gpp_purge_cnf_ev
)
{
  ds_bearer_context_s    *bearer_context_p = NULL; 
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;
  /*--------------------------------------------------------------------------*/

  if (ds3gpp_purge_cnf_ev == NULL)
  {
    DS_3GPP_ERROR_LOG_0("ds3gpp_purge_cnf_ev is NULL");
    return;
  }

  bearer_context_p = ds3gpp_purge_cnf_ev->bearer_context_p;

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL
  (
    purgecmdcbhdr, 
    subs_id,
    ds3gpp_purge_cnf_ev
  );
} /* ds_bearer_cntxt_proc_purgecmdcbhdlr */

#endif /* FEATURE_WCDMA */

/*=============================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_FILL_DATA_ACTIVITY_STATS

DESCRIPTION
  This function will handle the data activity status indication from PS

PARAMETERS
  client_data_ptr: Client Data Ptr passed in

DEPENDENCIES
  None

RETURN VALUE
  TRUE : IF fucntion processign is successful
  FALSE: If any error occured

SIDE EFFECTS
  None.

=============================================================================*/
boolean ds_3gpp_bearer_cntxt_fill_data_activity_status
(
  boolean*                 client_data_ptr,
  sys_modem_as_id_e_type   subs_id
)
{
  uint64                               cur_uldl_wk_cnt = 0;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  ds_3gppi_local_err_val_e_type        local_err =
                                       DS_3GPPI_LOCAL_ERR_VAL_NONE;
  boolean                              ret_val = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (client_data_ptr == NULL ||
       !ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    ret_val = FALSE;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }

  /*----------------------------------------------------------------------- 
    Fetch the per sub info hndl and validate it
    ---------------------------------------------------------------------*/
  per_subs_info_hndl = ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl(per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id %d",subs_id);
    ret_val = FALSE;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
    goto func_exit;
  }
  
  /*----------------------------------------------------------------------- 
    Get current UL/ DL stats from WM
    ---------------------------------------------------------------------*/
  cur_uldl_wk_cnt = ds3gpp_get_total_uldl_wm_cnt_hdlr(subs_id);
  
  /*------------------------------------------------------------------------ 
    If wmk cnt is initialized at first bearer UP && different than previous,
    return TRUE in the pointer passed and update the global counter
    -----------------------------------------------------------------------*/
  if (per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up &&
      cur_uldl_wk_cnt != per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt)
  {
    *client_data_ptr = TRUE;
    per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt = cur_uldl_wk_cnt;
  }
  else
  {
    /*--------------------------------------------------------------------- 
      Upon returning FALSE, UIM registers with ds_sys/PS_sys for
      DS_SYS_EVENT_DATA_ACTIVITY_RESUMED so that it can be notified when to poll
      next based on any data activity occuring. MH will get
      DATA_ACTIVITY_INDICATION_REGISTRATION and needs to process it.
      ----------------------------------------------------------------------*/
    *client_data_ptr = FALSE;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_bearer_cntxt_fill_data_activity_status: err d "
                       "data_activity returns FALSE",local_err);
    ret_val = FALSE;
  }

  return ret_val;
}/*ds_3gpp_bearer_cntxt_fill_data_activity_status*/

/*=============================================================================
FUNCTION ds_bearer_cntxt_clr_pending_calls

DESCRIPTION
    This function will clear the calls in Pending Redial state if conditions are met.

PARAMETERS
  client_data_ptr: Subs Id passed as Client Data Ptr

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/

void ds_bearer_cntxt_clr_pending_calls
(
  void* client_data_ptr
)
{
  ds_bearer_context_s     * bearer_context_p= NULL;
  sys_sys_mode_e_type       new_network_mode;
  uint8                     bearer_cntx_inst;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  sys_modem_as_id_e_type              *subs_id_p = 
                                        (sys_modem_as_id_e_type*)client_data_ptr;
  /*---------------------------------------------------------------------------*/

  if (subs_id_p == NULL)
  {
    DS_3GPP_ERROR_LOG_0("subs_id_p is NULL");
    return;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(*subs_id_p);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",*subs_id_p);
    return;
  }

  /* Clearing Pending Redial Timer as LTE is Deactivated  */
  if(per_subs_info_hndl->pending_redial_tmr_running == TRUE)
  {
    DS_3GPP_MSG0_HIGH_EX(*subs_id_p,
                      "Clearing Pending Redial Timer as cleaningup"
                      "of Pending Calls");
    (void)rex_clr_timer(&(per_subs_info_hndl->pending_redial_tmr));
    per_subs_info_hndl->pending_redial_tmr_running = FALSE;
  }

  new_network_mode = ds3gpp_get_current_network_mode(*subs_id_p);

  for( bearer_cntx_inst=0;\
      bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
      bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if (!ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_context_p, *subs_id_p))
    {
      continue;
    }
    else
    {
      new_network_mode = ds3gpp_get_current_network_mode(*subs_id_p);
    }

    /*-----------------------------------------------------------------------
     Cleanup PEND_REDIAL bearers as lower layers wouldn't send CNTX_TRSFER
     when registration fails on a RAT. Example:
     1. UE on W, originates a call, lower layers camp on LTE and reject this
        call with GW_L_RAT_CHANGE. Bearer state: PEND_REDIAL.
     2. CNTXT_TRSFER to LTE. TAU doesn't succeed and registration fails.
     3. UE comes back to W, no CNTX_TRSFER in this case.
     4. Bearer remains in PEND_REDIAL till app initiates call teardown.
     -------------------------------------------------------------------------*/
    
     ds_bearer_cntxt_cleanup_pending_calls(bearer_context_p, new_network_mode);
  }
   
}/* ds_bearer_cntxt_clr_pending_calls */
/*===========================================================================
FUNCTION  ds_bearer_cntxt_proc_dereg_dpm_cmd

DESCRIPTION
  This function is called when we process the command to deregister with
  dpm as part of context transfer.

PARAMETERS   
  *cmd_ptr - ds cmd ptr

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
void ds_bearer_cntxt_proc_dereg_dpm_cmd
(
  const ds_cmd_type *cmd_ptr
)
{
  ds_bearer_context_s       *bearer_context_p = NULL;
  dsm_watermark_type        *tx_wm_p = NULL;
  sys_modem_as_id_e_type     subs_id = SYS_MODEM_AS_ID_NONE;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_3GPP_ERROR_LOG_0("NULL ptr passed, return");
    return;
  }

  bearer_context_p =(ds_bearer_context_s*)(*((void**)cmd_ptr->cmd_payload_ptr));
  
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
  (   
    cntxt_transfer_ind_setup,
    bearer_context_p
  );

  if(bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
  {
    DS_3GPP_MSG1_HIGH_EX(subs_id, "Context Transfer Dpm Reregistration: %d",\
                        bearer_context_p->ds_bearer_context_dyn_p->phys_link.bearer_id);
    ds_bearer_cntx_reregister_dpm(bearer_context_p);
  }

  /*--------------------------------------------------------------
    Reset waiting_for_gone_empty flag as the Rx watermark is 
    empty
  --------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->waiting_for_gone_empty = FALSE;

  if(bearer_context_p->ds_bearer_context_dyn_p->state == 
       DS_BEARER_CONTEXT_STATE_UP_DORMANT)
  { 
    /*-------------------------------------------------------------------
      If data is sitting in the Watermarks (PS to PDCP), trigger 
      rab re-establishment immediately.
    -------------------------------------------------------------------*/
    tx_wm_p = &(bearer_context_p->tx_wm.wm_item);

    if( tx_wm_p->current_cnt > 0 )
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Data in WM, attempting to resume from domancy, "
                       "Bearer inst %d",
                       (uint32)bearer_context_p->ds_bearer_context_dyn_p
                       ->client_data_ptr);
      ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL); 
    }
  }
}
#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
FUNCTION  ds_bearer_cntxt_proc_gone_empty_cmd

DESCRIPTION
  This function is called when we process the gone empty command for the
  uplink wm

PARAMETERS   
  *cmd_ptr - ds cmd ptr

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
void ds_bearer_cntxt_proc_gone_empty_cmd
(
  const ds_cmd_type *cmd_ptr
)
{
  ds_bearer_context_s  *bearer_context_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG0_HIGH("Processing gone emmpty command");

  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_3GPP_ERROR_LOG_0("NULL ptr passed, return");
    return;
  }

  bearer_context_p =(ds_bearer_context_s*)(*((void**)cmd_ptr->cmd_payload_ptr));
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  bearer_context_p->ds_bearer_context_dyn_p->gone_empty_cmd_enqueued = FALSE;
  
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                   DS_FLOW_LTE_PDCP_MASK|DS_FLOW_LTE_PDCP_DNE_MASK,
                                   FALSE);

}
#endif
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PROCESSPSCMD

DESCRIPTION
  This function processes the bearer context specific PS commands received 
  by the DS task

PARAMETERS
  IN
    const ds_cmd_type* - Pointer to the command buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_process_ps_cmd
(
  const ds_cmd_type *cmd_ptr                         /* Pointer to Command */
)
{
  ds_3gpp_rrc_pch_ind_proc_cmd_type   *rrc_pch_ind_proc_ptr = NULL;
  ds_3gpp_rrc_state_ind_cmd_type      *rrc_state_ind_ptr;
  ds_3gpp_ul_rab_info_type            *ul_rab_info_ptr = NULL;
  ds_3gpp_pdn_call_info_type          *call_info_ptr = NULL;
#ifdef FEATURE_DATA_LTE
  ds3g_call_info_cmd_type             *call_info_cmd_ptr = NULL;
#endif /* FEATURE_DATA_LTE */
#ifdef FEATURE_DATA_WCDMA_PS
  ds_bearer_context_purge_cnf_ev_type *purge_cnf_ev_ptr = NULL;
#endif /* FEATURE_DATA_WCDMA_PS */
  boolean                              cmd_payload_missing = FALSE;
#ifdef FEATURE_DATA_FLOW_MGMT
  ds_bearer_wm_flow_control_ack_type   *flow_control_ack_ptr = NULL;
#endif /*FEATURE_DATA_FLOW_MGMT*/
  ds_3gppi_local_err_val_e_type        local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  int                                  temp_num = -1;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if(cmd_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("NULL cmd_ptr passed, return");
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  /*-------------------------------------------------------------------------
    Invoke the appropriate function to process the command, based on the
    command id.
  -------------------------------------------------------------------------*/
  switch(cmd_ptr->hdr.cmd_id)
  {
  
    case DS_CMD_3GPP_PHYS_LINK_UP:
      /*---------------------------------------------------------------------
        Command to bring up the Traffic Channel
        the parameter is actually bearer_cntx_inst and not call_instance
        Called only for dormancy reoriginations.
      ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        call_info_ptr = 
          (ds_3gpp_pdn_call_info_type*)cmd_ptr->cmd_payload_ptr;
       
        if(call_info_ptr->call_instance >= DS_3GPP_MAX_BEARER_CONTEXT)
      {
        DS_3GPP_MSG1_LOW("Invalid bearer_cntx_inst %d",
                             call_info_ptr->call_instance);
        DS3GPP_ASSERT(0);
        temp_num = (int)call_info_ptr->call_instance;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
        goto func_exit;
      }
        ds_bearer_cntxt_phys_link_up_cmd_hdlr(call_info_ptr->call_instance,
                                              call_info_ptr->info_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

    case DS_CMD_3GPP_PHYS_LINK_DOWN:

      /*---------------------------------------------------------------------
        Command to tear down the Traffic Channel
        the parameter is actually bearer_cntx_inst and not call_instance
        Still unclear as to who would be the clients invoking this command.
      ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        call_info_ptr=(ds_3gpp_pdn_call_info_type*)cmd_ptr->cmd_payload_ptr;

        if(call_info_ptr->call_instance >= DS_3GPP_MAX_BEARER_CONTEXT)
      {
        DS_3GPP_MSG1_LOW("Invalid bearer_cntx_inst %d",
                             call_info_ptr->call_instance);
        DS3GPP_ASSERT(0);
        temp_num = (int)call_info_ptr->call_instance;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        goto func_exit;
      }
        ds_bearer_cntxt_phys_link_down_cmd_hdlr(call_info_ptr->call_instance,
                                                call_info_ptr->info_ptr);
      }
      break;    

    case DS_CMD_3GPP_PHYS_LINK_FLOW_ENABLE:
      /*---------------------------------------------------------------------
      Flow Control the phys link based on the posted command.
      ---------------------------------------------------------------------*/
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
      ds_bearer_cntxt_phys_link_flow_cmd_hdlr(cmd_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      
      break;

    case DS_CMD_3GPP_DORM_REORIG_TMR_EXP:
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_bearer_cntxt_proc_reorig_dormtmr_exp_cmd(cmd_ptr->cmd_payload_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

#ifdef FEATURE_DATA_FLOW_MGMT
    case DS_CMD_3GPP_PERIODIC_TMR_EXP:
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_3gpp_burst_hdlr_periodic_timer_exp_cmd(cmd_ptr->cmd_payload_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;
    case DS_CMD_3GPP_GONE_EMPTY_CB:
     DS_3GPP_MSG0_LOW("ds3gpp_processPSCmd: DS3GPP_GONE_EMPTY_CB");
     if ( cmd_ptr->cmd_payload_ptr != NULL)
     {
       ds_bearer_cntxt_proc_gone_empty_cmd(cmd_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;
#endif /*FEATURE_DATA_FLOW_MGMT */      

    case DS_CMD_3GPP_DATA_SAMPLING_TMR_EXP:
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_bearer_cntxt_data_activity_sampling_timer_exp_cmd(cmd_ptr->cmd_payload_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;
      
    case DS_CMD_3GPP_DPM_DEREG:
      DS_3GPP_MSG0_LOW("ds3gpp_processPSCmd: DS3GPP_DEREG_DPM");
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
      ds_bearer_cntxt_proc_dereg_dpm_cmd(cmd_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

    case DS_CMD_3GPP_DORM_REORIG:
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
      ds_bearer_cntxt_proc_dorm_reorig_cmd(cmd_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

    case DS_CMD_3GPP_DSM_MEM_CTRL_EV:
      ds3gpp_enter_global_crit_section();
      ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag(FALSE);
      ds3gpp_leave_global_crit_section();
      ds_bearer_cntxt_flush_ps_l2_watermarks();
      break;
    
    case DS_CMD_3GPP_RRC_PCH_IND_PROC:
      if( cmd_ptr->cmd_payload_ptr != NULL)
      {
        rrc_pch_ind_proc_ptr
          = (ds_3gpp_rrc_pch_ind_proc_cmd_type*)cmd_ptr->cmd_payload_ptr;
        ds_bearer_cntxt_proc_pch_cmd(rrc_pch_ind_proc_ptr->pch_event,
                                   rrc_pch_ind_proc_ptr->call_mode,
                                   rrc_pch_ind_proc_ptr->subs_id);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;
    case DS_CMD_3GPP_RRC_STATE_IND_PROC:
       if( cmd_ptr->cmd_payload_ptr != NULL)
       {
          rrc_state_ind_ptr
             = (ds_3gpp_rrc_state_ind_cmd_type*)cmd_ptr->cmd_payload_ptr;

          ds_bearer_cntxt_proc_rrc_state_cmd(rrc_state_ind_ptr);

       }
       break;

    case DS_CMD_3GPP_FLOW_DISABLE_TMR_EXP:
    /*---------------------------------------------------------------------
          Command to process flow disable timer expiry.
    ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_3gpp_bearer_cntx_process_flow_disable_timer_exp_cmd(
         (ds_bearer_context_s*)(*((void**)cmd_ptr->cmd_payload_ptr)));
      }
      break;

#ifdef FEATURE_DATA_FLOW_MGMT
    case DS_CMD_3GPP_FLOW_ACK_TMR_EXP:
    /*---------------------------------------------------------------------
        Command to process flow disable timer expiry.
    ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_3gpp_burst_hdlr_process_flow_ack_timer_exp_cmd(
         (ds_bearer_context_s*)(*((void**)cmd_ptr->cmd_payload_ptr)));
      }
      break;
    case DS_CMD_3GPP_PROH_TIMER_CMD:
    /*---------------------------------------------------------------------
        Command to process flow disable timer expiry.
    ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_3gpp_burst_hdlr_process_prohibit_timer_cmd(
         (ds_bearer_context_s*)(*((void**)cmd_ptr->cmd_payload_ptr)));
      }
      break; 
    case DS_CMD_3GPP_FLOW_QMAP_ACK_CNF:
    /*---------------------------------------------------------------------
        Command to process flow disable timer expiry.
    ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        flow_control_ack_ptr = (ds_bearer_wm_flow_control_ack_type *)cmd_ptr->cmd_payload_ptr;

        ds_3gpp_burst_hdlr_process_qmap_ack_cnf_cmd(
          flow_control_ack_ptr->bearer_context_p,
          flow_control_ack_ptr->ack);
      }
      break;
#endif  /*FEATURE_DATA_FLOW_MGMT */

    case DS_CMD_3GPP_TD_CONFIG_UL_DATA_RATE:
      if( cmd_ptr->cmd_payload_ptr != NULL )
      {
        ul_rab_info_ptr = (ds_3gpp_ul_rab_info_type*)cmd_ptr->cmd_payload_ptr;
         ds_bearer_cntxt_td_proc_cmd(ul_rab_info_ptr->rab_id,
                                     ul_rab_info_ptr->ul_data_rate,
                                     ul_rab_info_ptr->subs_id);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

#ifdef FEATURE_WCDMA
    case DS_CMD_3GPP_RLC_PURGE_CNF:
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        purge_cnf_ev_ptr = 
          (ds_bearer_context_purge_cnf_ev_type*)cmd_ptr->cmd_payload_ptr;

      ds_bearer_cntxt_proc_purgecmdcbhdlr(
                  (ds_bearer_context_purge_cnf_ev_type*)(purge_cnf_ev_ptr)); 
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;
#endif /* FEATURE_WCDMA */

    case DS_CMD_3GPP_CALL_PENDING_CLR:
      DS_3GPP_MSG0_LOW("ds3gpp_processPSCmd: Received DS_3GPP_CLR_PENDING_CALLS");
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
      ds_bearer_cntxt_clr_pending_calls (cmd_ptr->cmd_payload_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

#ifdef FEATURE_DATA_LTE
    case DS_CMD_3GPP_CALL_END_MODE_CHG_LTE:
      DS_3GPP_MSG0_LOW("ds3gpp_processPSCmd: Received DS_CMD_3GPP_CALL_END_MODE_CHG_LTE");
      call_info_cmd_ptr = (ds3g_call_info_cmd_type*)(cmd_ptr->cmd_payload_ptr);
      if (NULL != call_info_cmd_ptr)
      {
        ds_bearer_cntxt_resend_call_end(call_info_cmd_ptr->call_id);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;  
#endif /* FEATURE_DATA_LTE */

    case DS_CMD_3GPP_NON_EMPTY_CB_CMD:
      if ( cmd_ptr->cmd_payload_ptr != NULL)
      {
        ds_bearer_cntxt_proc_non_empty_cb_cmd(cmd_ptr);
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;

     case DS_CMD_3GPP_BEARER_CNTXT_TMR_DYN_BACK_PRESSURE:

      /*---------------------------------------------------------------------
        Command to tear down the Traffic Channel
        the parameter is actually bearer_cntx_inst and not call_instance
        Still unclear as to who would be the clients invoking this command.
      ---------------------------------------------------------------------*/
      if (cmd_ptr->cmd_payload_ptr != NULL)
      {
        DATA_MSG1_LOW("DS_CMD_3GPP_BEARER_CNTXT_TMR_DYN_BACK_PRESSURE 0x%x",
                        cmd_ptr->cmd_payload_ptr);
        ds_bearer_cntxt_dyn_back_pressure_exp_cmd_hdlr((ds_bearer_context_s *)
                                                        (cmd_ptr->cmd_payload_ptr));
      }
      else
      {
        cmd_payload_missing = TRUE;
      }
      break;    

    default:
      DATA_MSG1_LOW("Unknown cmd to UMTS mode Sp. hdlr %d", cmd_ptr->hdr.cmd_id);
      temp_num = (int)cmd_ptr->hdr.cmd_id;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;

  } /* switch( cmd_id ) */

  if (cmd_payload_missing == TRUE)
  {
    DS_3GPP_MSG1_LOW("ds_bearer_cntxt_processPSCmd: received %d payload ptr is NULL",
                       cmd_ptr->hdr.cmd_id);
    temp_num = (int)cmd_ptr->hdr.cmd_id;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_4;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_2("ds_bearer_cntxt_processPSCmd: err d temp_num d",
                       local_err, temp_num);
  }

  return;
} /* ds_bearer_cntxt_processPSCmd() */

#ifdef FEATURE_DATA_IMS
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_ISIMCNCALLUP

DESCRIPTION
  This fn is used during call orig to find out if there is already a call
  with the IM_CN_subsystem flag set to TRUE

PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the IMCN call is up, False otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_is_IMCN_call_up
(
  sys_modem_as_id_e_type subs_id
)
{
  int                   bearer_index;
  ds_bearer_context_s*  bearer_context_p;
  boolean               im_cn_flag = FALSE;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Check the bearer cntx table to see if there if any call has the IM CN flag 
    set.
  -------------------------------------------------------------------------*/
  for(bearer_index=0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;bearer_index++)
  {  

    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index); 
    
     if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                               subs_id))
     {
       continue;
     }
    /*-----------------------------------------------------------------------
      If a call is in UP state and IM CN flag is set, set im_cn_flag
      = TRUE.
    -----------------------------------------------------------------------*/
    if( (bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_UP ||
         bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_UP_DORMANT ||
         bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB ) && 
        (bearer_context_p->ds_bearer_context_dyn_p->im_cn_subsystem_flag == TRUE) )
    {
      DS_3GPP_MSG1_LOW ("Call with IM CN flag already up: bearer cntx inst:%d",
             bearer_index);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      im_cn_flag = TRUE;
      break;
    }
    else if((ds_3gpp_pdn_cntx_validate_pdn_context_internal(bearer_context_p->
             ds_bearer_context_dyn_p->pdn_context_ptr)) && 
             (bearer_context_p->ds_bearer_context_dyn_p->state 
              == DS_BEARER_CONTEXT_STATE_COMING_UP) &&
      (((ds_pdn_context_s*)(bearer_context_p->
       ds_bearer_context_dyn_p->pdn_context_ptr))->ds_pdn_context_dyn_p
       -> pdp_profile.im_cn_flag == TRUE) )
    {
      /*-----------------------------------------------------------------------
        Check the profile to see if there if any call in pending state
        with the IM CN flag.
      
        If the call is in coming up state, that means a call with IM CN
        flag is pending.
      -----------------------------------------------------------------------*/                                          
      DS_3GPP_MSG1_LOW("Call with IM CN flag pending: bearer cntx inst:%d",
          bearer_index);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
        im_cn_flag = TRUE;
        break;      
    }
  } /* for .. */

  DS_3GPP_MSG2_HIGH_EX(subs_id, "In ds_bearer_cntxt_isIMCNCallUp: returning %d UP cause %d",
                    im_cn_flag, local_err);

  return im_cn_flag;
} /* ds_bearer_cntxt_isIMCNCallUp() */

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_GET_IMCN_SUBSYSTEM_FLAG_FOR_IFACE

DESCRIPTION
   This function returns the value of IM CN subsystem flag for an iface. It
   looks up the call table to get the IM CN Subsystem flag.
   
PARAMETERS   :
  *iface_ptr     - Pointer to iface on which query is made
  im_cn_status   - boolean pointer to return the status of IM CN Flag.
  *ps_errno      - Error if any
  
DEPENDENCIES
  None.

RETURN VALUE
   0 - If the IM CN flag is successfully retrieved.
   -1 - If the iface is not valid.

SIDE EFFECTS
  None.

===========================================================================*/
int ds_bearer_cntxt_get_IMCN_subsystem_flag_for_iface
(
  const ps_iface_type      *iface_ptr,
  boolean                  *im_cn_status,
  sint15                   *ps_errno
)
{
  ds_pdn_context_s              *pdn_cntx_ptr = NULL;
  ds_bearer_context_s           *bearer_context_ptr = NULL;
  ds_3gppi_local_err_val_e_type  local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {
  if(ps_errno == NULL)
  {
      DS_3GPP_MSG0_LOW("ps_errno argument is NULL");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
  }

  if( (iface_ptr == NULL) || (im_cn_status == NULL) )
  {
      DS_3GPP_MSG3_LOW("Invalid arguments. iface_ptr = 0x%x, im_cn_status= 0x%x",
                       iface_ptr, im_cn_status, 0);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
  }

  subs_id = ds3gsubsmgr_subs_id_ds_to_cm(PS_IFACE_GET_SUBS_ID(iface_ptr));

  pdn_cntx_ptr = (ds_pdn_context_s*)(((ds_3gpp_iface_s*)
                         (iface_ptr->client_data_ptr))->client_data_ptr);

  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_ptr))
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
  }

  bearer_context_ptr = 
         (ds_bearer_context_s*)(pdn_cntx_ptr->ds_pdn_context_dyn_p->
                                 def_bearer_context_ptr);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;
  }

  *im_cn_status = bearer_context_ptr->ds_bearer_context_dyn_p->im_cn_subsystem_flag;

  return 0;
  } while (0);

  DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_bearer_cntxt_get_IMCN_subsystem_flag_for_iface: err d",
                    local_err);

  return -1;
}/* ds_bearer_cntxt_get_IMCN_subsystem_flag_for_iface */

#endif /* FEATURE_DATA_IMS */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_ALLOC

DESCRIPTION
  This function marks a bearer context to be in use by allocating a CM call
  id to it. There is no other processing done here. This is mainly used for
  MT calls in UMTS and in the default bearer activation in EPS.

PARAMETERS
  IN
    ds_bearer_context_s* - Pointer to the bearer that has to be allocated.
    cm_call_id_type      - This is the call id that is to be stored in bearer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_alloc
(
  ds_bearer_context_s* bearer_context_p,
  cm_call_id_type      cm_call_id
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = cm_call_id;
} /* ds_bearer_cntxt_alloc() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_INCOMINGCALL

DESCRIPTION
  This function results in a transition of the bearer context state.
  This is called from 3gpp handler if USER_ALERT is given to it by
  PDN context.

PARAMETERS
  IN
    ds_bearer_context_s* - The bearer whose state is to be changed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_incoming_call(ds_bearer_context_s* bearer_context_p)
{
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*
    Change the state of the bearer to COMING_UP
  */

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  
  DS_3GPP_MSG0_HIGH_EX(subs_id, "Changing bearer context state to coming UP");

  ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                   DS_BEARER_CONTEXT_STATE_COMING_UP);

} /* ds_bearer_cntxt_incomingCall() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_ANSWERCALL

DESCRIPTION
  This function is used to answer an incoming call. This is the point of
  convergence of the two different paths that an MT call could take (Auto
  answer and Application answer).

PARAMETERS
  IN
    ds_bearer_context_s* - The bearer on which the incoming call is to be
                           answered.

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS - if the call was initiated successfully or if hang up was
                 initiated successfully on a failed call initiation
  DS3G_FAILURE - if the hang up was unsuccessful. Local clean up happens in
                 this case.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_bearer_cntxt_answer_call(ds_bearer_context_s* bearer_context_p)
{
  ds_3gpp_pdn_call_info_type  call_info;
  boolean                     retVal = DS3G_SUCCESS; 
  ds_3gpp_down_reason_t       net_down_reason;
  sys_modem_as_id_e_type      subs_id = SYS_MODEM_AS_ID_NONE; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  DS_3GPP_MSG0_HIGH_EX(subs_id, "Call origination due to MT request");
  
  call_info.mt_flag = TRUE;
  /*-------------------------------------------------------------------
    If answer call failed, then need to send MT reject
  -------------------------------------------------------------------*/
  
  if (ds3g_msh_answer_call_ex
       (bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
        subs_id) == DS3G_FAILURE)
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, "Error in answering call, Bearer cntxt: d call_id d",
                       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr,
                       bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);

    /* Enable flow on the phys link*/
    ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                DS_FLOW_UMTS_MASK, FALSE);
    
    /* Send the phys link gone indication */
    ps_phys_link_gone_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
    net_down_reason.reason.ps_iface_down_reason = 
                                          PS_NET_DOWN_REASON_VALIDATION_FAILURE;
    net_down_reason.reason_T = DS_3GPP_PS_IFACE_NET_DOWN_REASON_T;

    /* answer failed, call bearer down to clean up*/
    if (bearer_context_p->ds_bearer_context_dyn_p
                        ->pdn_context_cb_tbl_ptr->bearer_down_ind_cb != NULL)
    {
      bearer_context_p->ds_bearer_context_dyn_p
                        ->pdn_context_cb_tbl_ptr->bearer_down_ind_cb(
                    bearer_context_p->ds_bearer_context_dyn_p
                        ->pdn_context_ptr,\
                    bearer_context_p,
                    net_down_reason,
                    FALSE);
    }
    if (ds3g_msh_hangup_call_ex(bearer_context_p->ds_bearer_context_dyn_p->
                               cm_call_id, &call_info, subs_id)== DS3G_FAILURE)
    {
      /*-----------------------------------------------------------------------
        The call has already ended.
      -----------------------------------------------------------------------*/
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Call already ended call_id :%d",\
                           bearer_context_p->ds_bearer_context_dyn_p
                        ->cm_call_id);

      /*Do a local clean up*/
      ds_bearer_cntxt_free_bearer_inst(bearer_context_p);

      retVal = DS3G_FAILURE;
    }
    else
    {
       ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                        DS_BEARER_CONTEXT_STATE_GOING_DOWN);
    }
  }
  return retVal;

} /* ds_bearer_cntxt_answerCall() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_ALLOCATE_NEW_BEARER

DESCRIPTION
  This function is used to return a bearer from the pool of available bearer
  contexts. The allocation and deallocation details are completely hidden from
  the caller. We could change the implementation of this function at a later
  point and make bearer allocations completely dynamic instead of allocating
  a free bearer from the pool of statically created bearers.

PARAMETERS
  Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_context_s*

SIDE EFFECTS
  None.
===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_allocate_new_bearer
(
  sys_modem_as_id_e_type subs_id
)
{
  int                  loop_index;
  ds_bearer_context_s* bearer_context_p = NULL;
  sys_sys_mode_e_type    mode = 
                           ds3gpp_get_current_network_mode(subs_id);
#if defined (FEATURE_DATA_WCDMA_PS) || defined (FEATURE_GSM_GPRS)
  sys_sys_mode_e_type  umts_network_mode = SYS_SYS_MODE_NONE;
#endif
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined FEATURE_WCDMA
  umts_network_mode = SYS_SYS_MODE_WCDMA;
#elif defined FEATURE_TDSCDMA
  umts_network_mode =  SYS_SYS_MODE_TDS;
#elif defined FEATURE_GSM_GPRS
  umts_network_mode = SYS_SYS_MODE_GSM;
#endif

  for (loop_index = 0; loop_index < DS_3GPP_MAX_BEARER_CONTEXT; loop_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(loop_index);

    if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      if(!ds_bearer_cntx_alloc_dyn_mem(loop_index, subs_id))
      {
        DS_3GPP_MSG0_LOW("Failure to allocate dynamic memory, Heap failure");
        bearer_context_p = NULL;
        local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
        goto func_exit;
      }

      /*Update the bearer context mode with the global ds_3gpp_sys_mode*/
      bearer_context_p->ds_bearer_context_dyn_p->call_mode = 
        mode;
      bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = 
        CM_CALL_ID_INVALID;
      bearer_context_p->ds_bearer_context_dyn_p->subs_id = subs_id;
      ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                        DS_BEARER_CONTEXT_STATE_DOWN);

      bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr = 
        (void*)loop_index;
      /* Initialize as non-default */
      bearer_context_p->ds_bearer_context_dyn_p->is_default = FALSE;
      /* Initialize SDF ID to be invalid */
      bearer_context_p->ds_bearer_context_dyn_p->sdf_id = 0xFFFF;
      /* If NW provided TI for GW, the bearer can be transfered to GW RAT. */
      bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW = TRUE;        
      
      bearer_context_p->ds_bearer_context_dyn_p->phys_link.client_data_ptr =\
           bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr;             

      bearer_context_p->ds_bearer_context_dyn_p->
        ul_registered_with_ltepdcp = FALSE;              

      /* Initialize call_mode to 3GPP sys mode */
     /*---------------------------------------------------------------------
        Initialize the signal for the bearer context for data in the DL dir.
        We are adding support for having one signal per bearer context. This
        simplifies the design to look for data in the DL direction.

        However if QOS will have to be implemented between the different WMs
        present in a PDN context, we will have to revisit this section.
      ---------------------------------------------------------------------*/
      /*  Move rx sig to static */
      bearer_context_p->ds_bearer_context_dyn_p->rx_sig = (ps_sig_enum_type)
      ((uint32)PS_3GPP_UM_RX_Q_SIGNAL_0 + (loop_index));
      
      /* Populate the PDN context call back pointers table */
      bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr = \
            &bearer_context_notifier_cb_tbl;

      /*------------------------------------------------------------------------
        Set the initial values of the byte counters to zero
      ------------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->tx_bytes = 0;
      bearer_context_p->ds_bearer_context_dyn_p->rx_bytes = 0;

      bearer_context_p->ds_bearer_context_dyn_p->wm_flush_state = 
                                        DS_BEARER_CONTEXT_UM_WM_STATE_INVALID;
      bearer_context_p->ds_bearer_context_dyn_p->teardown_on_modify_ind = FALSE;

      /*------------------------------------------------------------------------
        Set bearer_down_reason_rab_failure to 0
      ------------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->down_reason        = 
        PS_NET_DOWN_REASON_NOT_SPECIFIED;
      bearer_context_p->ds_bearer_context_dyn_p->down_reason_ps_eic = 
        PS_EIC_NOT_SPECIFIED;

      /*------------------------------------------------------------------------
        Define the bearer flow disable timer. Specify the call back function
        and the call back data to be called upon timer expiry
      ------------------------------------------------------------------------*/
      rex_def_timer_ex
         (&(bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer),
         (rex_timer_cb_type) ds_3gpp_bearer_cntx_flow_disable_timer_cb,
         (unsigned long)(bearer_context_p));
      /*------------------------------------------------------------------------
        Initialize timer running value to FALSE.
      ------------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running = FALSE;

      /*------------------------------------------------------------------------
        Assign a PS handle to the bearer. This keeps track of whether or not
        MH should process the PS signal callback for rx data for this bearer.
      ------------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->ps_handle = 
        ps_handle_mgr_get_handle(PS_HANDLE_MGR_CLIENT_3GPP, loop_index);

      /*-------------------------------------------------------------------------
        Now create the phys link that is associated with the bearer. We have only
        one phys link per bearer. Hence we call this API with the pointer to the
        phys link and 1 as the second argument.
     -------------------------------------------------------------------------*/
    if( ps_phys_link_create(&(bearer_context_p->\
            ds_bearer_context_dyn_p->phys_link), DS_3GPP_MAX_PHYS_LINK_PER_BEARER) != 0)
    {
      DS_3GPP_MSG0_LOW("Failure to create physlink");
      bearer_context_p = NULL;
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      goto func_exit;
    }
 
    PS_PHYS_LINK_SET_CAPABILITY( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                      (int)PS_PHYS_LINK_CAPABILITY_FLOW_DECOUPLED);
      /*lint +e774 +e506 +e641 */

#ifdef FEATURE_DATA_FLOW_MGMT
    /*------------------------------------------------------------------------
        Define the bearer flow ack timer. Specify the call back function
        and the call back data to be called upon timer expiry
    ------------------------------------------------------------------------*/
    rex_def_timer_ex
         (&(bearer_context_p->ds_bearer_context_dyn_p->flow_ack_timer),
         (rex_timer_cb_type) ds_3gpp_burst_hdlr_flow_ack_timer_cb,
         (unsigned long)(bearer_context_p));
    /*------------------------------------------------------------------------
        Initialize timer running value to FALSE.
    ------------------------------------------------------------------------*/
    bearer_context_p->ds_bearer_context_dyn_p->flow_ack_timer_running = FALSE;

    /*------------------------------------------------------------------------
	     Memset flow control info
    ------------------------------------------------------------------------*/
    memset(&(bearer_context_p->flow_control_info),0,
           sizeof(ds3g_burst_based_flow_control_s));

    bearer_context_p->flow_control_info.subs_id = subs_id;
    /* Store the corresponding physlink info in the flow params structure */
    bearer_context_p->flow_control_info.phys_link_ptr = 
       &(bearer_context_p->ds_bearer_context_dyn_p->phys_link);

   /*------------------------------------------------------------------------
        Define the bearer flow ack timer. Specify the call back function
        and the call back data to be called upon timer expiry
    ------------------------------------------------------------------------*/
    rex_def_timer_ex
       (&(bearer_context_p->flow_control_info.burst_prohibit_timer),
       (rex_timer_cb_type) ds_3gpp_burst_flow_proh_timer_cb,
       (unsigned long)(bearer_context_p));

#endif /*FEATURE_DATA_FLOW_MGMT */

       bearer_context_p->ds_bearer_context_dyn_p->
      phys_link.phys_link_down_cmd_f_ptr = ds_bearer_cntxt_phys_link_down; // clang_sa_ignore [core.NullDereference]

    bearer_context_p->ds_bearer_context_dyn_p->
      phys_link.phys_link_go_null_cmd_f_ptr = ds_bearer_cntxt_phys_link_down;

    bearer_context_p->ds_bearer_context_dyn_p->phys_link.phys_link_up_cmd_f_ptr =
           ds_bearer_cntxt_phys_link_up;
      /* Hari: TODO define qos release and modify handlers*/        
    bearer_context_p->ds_bearer_context_dyn_p->phys_link.ioctl_f_ptr =
           ds_bearer_cntxt_phys_link_ioctl_hdlr;

    bearer_context_p->ds_bearer_context_dyn_p->waiting_for_gone_empty = FALSE;

     /*-------------------------------------------------------------------------
      Associate the mode specific blob that is going to be populated to the 
      bearer context pointer.
     -------------------------------------------------------------------------*/
#ifdef FEATURE_LTE
     DS_BEARER_CTXT_VOBJ_REG
     (
       bearer_context_p, 
       SYS_SYS_MODE_LTE, 
       &eps_bearer_context_tbl[(uint32)bearer_context_p->ds_bearer_context_dyn_p->
       client_data_ptr] 
     );
      DS_3GPP_MSG0_LOW("Allocating LTE eps bearer context");
#endif

#if defined (FEATURE_DATA_WCDMA_PS) || defined (FEATURE_GSM_GPRS)
      DS_BEARER_CTXT_VOBJ_REG
     (
       bearer_context_p, 
       umts_network_mode, 
       &umts_bearer_context_tbl[(uint32)bearer_context_p->ds_bearer_context_dyn_p->
       client_data_ptr] 
     );
#endif

     DS_3GPP_MSG0_HIGH_EX(subs_id, "Allocating LTE and/or WCDMA umts bearer context");
     ds_3gpp_bearer_cntxt_initiate_uld_stats_per_bearer(bearer_context_p);

     break;
    }
  }

  if (loop_index == DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_MSG0_LOW("MAX Bearer contexts have been allocated");
    bearer_context_p = NULL;
    local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
  }

func_exit:
  if(local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_bearer_cntxt_allocate_new_bearer: err d",
                       local_err);
  }

  return (void*)bearer_context_p;
} /* ds_bearer_cntxt_allocate_new_bearer */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_RETURN_BEARER_TO_FREE_POOL

DESCRIPTION
  This function is used to return a bearer to the pool of available bearer
  contexts. This function just resets the in_use flag inside the bearer.

PARAMETERS
  IN
    bearer_context_p - pointer to the bearer to be marked as free.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_return_bearer_to_free_pool
(
  ds_bearer_context_s* bearer_context_p
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (bearer_context_p != NULL && 
      bearer_context_p->ds_bearer_context_dyn_p != NULL)
  {
    ps_phys_link_delete(&bearer_context_p->ds_bearer_context_dyn_p->phys_link,
                                           DS_3GPP_MAX_PHYS_LINK_PER_BEARER);
  
    memset(&bearer_context_p->ds_bearer_context_dyn_p->phys_link, 0, 
                                                 sizeof(ps_phys_link_type));
  }
  
  ds_bearer_cntx_free_dyn_mem(bearer_context_p);

}
/*===========================================================================
  FUNCTION DS_BEARER_CNTXT_REORIG_DORM

  DESCRIPTION
    This function is responsible for reoriginating the call from dormancy.
    This function ends up calling the DSMGR function for RAB reestab.
    The logic to check if there is a Rab-reestab request already pending is
    also present in this function.
  
  PARAMETERS   
   *bearer_context_p - Pointer to the bearer context that we are working on
   dsm_item_type*    - reorig packet pointer
                     
  DEPENDENCIES
    None.
  
  RETURN VALUE 
    DS3G_SUCCESS : Successful setup
    DS3G_FAILURE : Setup error
  
  SIDE EFFECTS 
    None.
  
===========================================================================*/
void ds_bearer_cntxt_reorig_dorm
(
  ds_bearer_context_s* bearer_context_p,
  dsm_item_type*       reorig_pkt_ptr
)
{
  cm_rab_reestab_params_s_type  rab_params;
  int16                         ps_errno = 0;
  ds_pdn_context_s*             pdn_cntx_p = NULL;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;

#ifdef FEATURE_DATA_LTE
  ds_eps_bearer_context_s             *eps_bearer_context_p = NULL;
  int                                 v4_iface_reestab_call_type = 
                                        SYS_LTE_REESTAB_CALLTYPE_NORMAL;
  int                                 v6_iface_reestab_call_type = 
                                        SYS_LTE_REESTAB_CALLTYPE_NORMAL;
#endif /* FEATURE_DATA_LTE */
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  ds_eps_tlb_per_subs_info_type        *eps_tlb_hndl = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if (!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  pdn_cntx_p = (ds_pdn_context_s*)
               bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",subs_id);
    return;
  }

#ifdef FEATURE_DATA_LTE
    eps_tlb_hndl = ds_eps_tlb_get_per_subs_info_hndl(subs_id);

    if((ds_eps_tlb_validate_per_subs_info_hndl(eps_tlb_hndl)) &&
         ((eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_B) ||
          (eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_C) ))
    {
      if (bearer_context_p->tx_wm.wm_item.current_cnt == 0) 
      {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Exiting phys link bring up due to tlb mode b/c");
      return;
      }
    } 
#endif
  /*-------------------------------------------------------------------------
    RAB-reestablishment can happen on the bearer only if it is DORMANT.
    If the bearer is in any other state, then we do nothing for the reorig
    request.
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT)
  {
    if(ds3gpp_get_ps_data_suspend_flag(subs_id) == TRUE)
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "UL data bearer:%d data_suspend:1 sending data_available",\
         (uint32) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

      if( (!ds3gpp_get_ps_data_available_flag(subs_id)) && 
           ds3gpp_is_ps_data_avail_ind_allowed(subs_id,
            ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_p)))
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "UL data bearer:%d data_suspend:1 sending data_available",\
                          (uint32) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
        ds3g_msh_ps_data_available_ind(subs_id);
        ds3gpp_set_ps_data_available_flag(TRUE, subs_id);
      }

      /*--------------------------------------------------------------
        Disable flow on the interface
      --------------------------------------------------------------*/
      ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p,
                                    DS_FLOW_UMTS_MASK, FALSE);    
      return;
    }
    else
    {
      /*--------------------------------------------------------------------
        RAB reestablishment can happen only for wcdma, lte and tds modes.
        For all other modes,eg Gsm, we don't establish Rab and hence
        return.
      --------------------------------------------------------------------*/
      if(ds3gpp_get_current_network_mode(subs_id) == SYS_SYS_MODE_GSM)
       {
         DS_3GPP_MSG0_HIGH_EX(subs_id, "ds_3gpp_sys_mode is GSM, returning");
         return;
       }
    } 

    if (bearer_context_p->ds_bearer_context_dyn_p->waiting_for_gone_empty == TRUE)
    {

      DS_3GPP_MSG0_MED_EX(subs_id, "Waiting for SRAT's Rx Wmk to drain. "
                       "Not resuming from dormancy yet.");
      return;
    }

    DS_3GPP_MSG3_HIGH_EX(subs_id, "RAB RETRY on bearer:%d,service_req_pending_flag:%d,rab_timmer_running:%d",
                       (uint32) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr,
                                per_subs_info_hndl->service_request_pending_flag,
                                per_subs_info_hndl->timer_running);
    
    if (ds_3gpp_nv_get_max_rab_reestab_retry_cnt(subs_id) == RAB_REESTAB_RETRY_CNT_INVALID)
    {
    /*-------------------------------------------------------------------------
      Disable flow on this phys link so that no further packets can be queued.
      Similarly the socket app should also not write data into this phys link.
      This way, we prevent more reorig commands from being put into DS task.
    -------------------------------------------------------------------------*/ 

      ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p,
                                   DS_FLOW_UMTS_MASK, FALSE);
    }

    /*-------------------------------------------------------------------------
      In the uplink_data_bitmask, we set the bit field corresponding to this
      bearer. This way we prevent a reorig request from being sent out due to
      the same bearer.
    -------------------------------------------------------------------------*/
    DS_BEARER_CNTXT_SET_BITMASK(per_subs_info_hndl, bearer_context_p);
    
    /*-------------------------------------------------------------------------
      If the timer to re-establish RABs is already running or if a service
      request to reestab rab has already been sent out, then we do nothing.
      We return from this function.
    -------------------------------------------------------------------------*/
    if( (per_subs_info_hndl->service_request_pending_flag == TRUE ) || 
        (per_subs_info_hndl->timer_running == TRUE)  )
    {
      return;
    }
    
    /*-------------------------------------------------------------------------
      Set the state of the bearer to RAB_REESTAB.
    -------------------------------------------------------------------------*/

    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB);

    /*--------------------------------------------------------------------- 
     Set Phys link to COMING UP state
     --------------------------------------------------------------------*/
    if( ps_phys_link_up_cmd(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link), 
                                             &ps_errno, NULL) != 0)
    {
       DS_3GPP_MSG1_HIGH_EX(subs_id, "phys_link_up_cmd failed errno: %d",
                          ps_errno);
    }
    
    /*-------------------------------------------------------------------------
      Clear the RAB reestablish timer. This will be started again either in
      rab_re_est_ind or in rab_re_est_rej handler or in the registered timer
      call back
    -------------------------------------------------------------------------*/

    DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl);
    ds_3gpp_rab_reestab_reset_state(subs_id);

    /*-------------------------------------------------------------------------
      Initialize rab_params.
    -------------------------------------------------------------------------*/
    memset(&rab_params,0, sizeof(cm_rab_reestab_params_s_type));
#ifdef FEATURE_DATA_LTE
    if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE)
    {
      eps_bearer_context_p = (ds_eps_bearer_context_s *)
        DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);

      if (!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context_p))
      {
        return;
      }

      rab_params.sys_mode = SYS_SYS_MODE_LTE;
      rab_params.rab_reestab_params.cm_lte_rab_reestab_params.eps_bearer_id = 
                   eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id;

      /*--------------------------------------------------------------------- 
        Determine RAB Re-establishment call type
        ---------------------------------------------------------------------*/
      v4_iface_reestab_call_type = pdn_cntx_p->ds_pdn_context_dyn_p->
                                  rab_reestab_call_type[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]; 
      v6_iface_reestab_call_type = pdn_cntx_p->ds_pdn_context_dyn_p->
                                  rab_reestab_call_type[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]; 
      /*--------------------------------------------------------------------- 
        If the RE-Estab Calltype is VOLTE in atleast one of the Ifaces then
        request for Re-Estab Call type as VOLTE
        ---------------------------------------------------------------------*/
      if ((v4_iface_reestab_call_type == PS_IFACE_3GPP_LTE_REESTAB_CALLTYPE_VOLTE)||
          (v6_iface_reestab_call_type == PS_IFACE_3GPP_LTE_REESTAB_CALLTYPE_VOLTE))
      {
        rab_params.sys_lte_reestab_calltype = SYS_LTE_REESTAB_CALLTYPE_VOLTE;
      }
      else
      {
        rab_params.sys_lte_reestab_calltype = SYS_LTE_REESTAB_CALLTYPE_NORMAL;
      }
      DS_3GPP_MSG2_HIGH_EX(subs_id, "Requesting reestab for bearer id %d with Re-estab Call type %d",
                        eps_bearer_context_p->ds_eps_bearer_context_dyn_p->eps_bearer_id,
                        rab_params.sys_lte_reestab_calltype);
    }
#endif /* FEATURE_DATA_LTE */

    if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA)
    {
      rab_params.sys_mode = SYS_SYS_MODE_WCDMA;
      rab_params.rab_reestab_params.cm_wcdma_rab_reestab_params.rabid = 
                        (rabm_rabid_T) bearer_context_p->ds_bearer_context_dyn_p->nsapi; 
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Requesting reestab for rab id %d",
                        bearer_context_p->ds_bearer_context_dyn_p->nsapi);
    }
#ifdef FEATURE_TDSCDMA
    else if(ds3gpp_get_current_network_mode(subs_id) == SYS_SYS_MODE_TDS)
    {
      rab_params.sys_mode = SYS_SYS_MODE_TDS;
      rab_params.rab_reestab_params.cm_wcdma_rab_reestab_params.rabid = 
                        (rabm_rabid_T) bearer_context_p->ds_bearer_context_dyn_p->nsapi; 
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Requesting reestab for rab id %d",
                        bearer_context_p->ds_bearer_context_dyn_p->nsapi);
    }
#endif /* FEATURE_TDSCDMA */
    /*------------------------------------------------------------------------- 
      Indicate to PS the Physlink pointer, Rx watermark count,
      Tx watermark count and dormancy status.
      Dormancy status is set to REORIG_FROM_DORMANCY
    ---------------------------------------------------------------------------*/
    ds_bearer_cntxt_report_ps_wmk_logging(bearer_context_p,
                                          PS_WMK_LOGGING_REORIG_FROM_DORMANCY);

      DS_3GPP_MSG1_HIGH_EX(subs_id, "WM logging Packet onBearer instance: %d",
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

    if ( reorig_pkt_ptr != NULL )
    {
      if (0 != ps_wmk_logging_reorig_pkt(DS_SYS_TECH_3GPP,
                                         PS_WMK_UPLINK_PKT,
                                         reorig_pkt_ptr))
      { 
        DS_3GPP_ERROR_LOG_1_EX(subs_id, "WM logging Packet Error onBearer instance: d",
          (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
      }
    }
    else
    {
      DS_3GPP_ERROR_LOG_0_EX(subs_id, "WM logging error, Invalid Pkt_ptr");
    }

    ds_3gpp_rab_reestab_transition_state_ev(DS_3GPP_PROC_RAB_REESTAB_REQ,
                                         NULL,
                                         subs_id);

    (void)ds3g_msh_call_rab_reestab_req(
                    bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                    &rab_params,
                    subs_id
                    );
    /*-------------------------------------------------------------------------
     * Set the service_request_pending_flag to TRUE so that we do not post 
     * another request while there is OTA service request still to be answered
    -------------------------------------------------------------------------*/
    per_subs_info_hndl->service_request_pending_flag = TRUE;

    /*-------------------------------------------------------------------------
     * Also mark the bearer context that was used to send out the service req
     * by setting the srv_rsp_exp to TRUE
    -------------------------------------------------------------------------*/
    bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp = TRUE;
  }
  else
  {
    if((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP) &&
       (PS_PHYS_LINK_GET_STATE(&(bearer_context_p->ds_bearer_context_dyn_p->
                                                          phys_link)) == PHYS_LINK_DOWN))
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "PCH state: phys link up cmd and clear enqueue fun ptr");

      /*--------------------------------------------------------------------- 
        Clear each enqueue function pointer for non GSM RATs. This is because
        the ownership of each_enqueue_func_ptr in case of GSM mode is with GSNDCP.
      --------------------------------------------------------------------*/
      if(ds3gpp_get_current_network_mode(subs_id) != SYS_SYS_MODE_GSM)
      {
       (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr = NULL;
       (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data = NULL;
      }
      
      /*--------------------------------------------------------------------- 
        Set Phys link to COMING UP state
      --------------------------------------------------------------------*/
      if( ps_phys_link_up_cmd(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link), 
                                                  &ps_errno, NULL) != 0)
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "phys_link_up_cmd failed errno: %d", ps_errno);
      }
      
      /*--------------------------------------------------------------------- 
        if a PDN is in COMING_UP state, Flow mgr would be in ORIG state.
        Posting ACTIVE on the other PDN would result in starting the 
        flow monitor timer which could lead to data priority being lowered
        before the PDN is connected.
      ---------------------------------------------------------------------*/
      if (!dsPdnContext_is_any_normal_pdn_coming_up_ex(subs_id))
      {
        ds3geventmgr_notify_ps_call_event_per_sub(subs_id,
                                                  DS3GEVENTMGR_PS_CALL_ACTIVE_EV,
                                                  PS_SYS_NETWORK_3GPP);
      }
    }
    DS_3GPP_MSG2_HIGH_EX(subs_id, 
       "Bearer not in DORMANT state. Bearer: %d, State:%d",
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr,
       bearer_context_p->ds_bearer_context_dyn_p->state);
  }
  return; 
} /* dsWcdmaBearerCntxt_reorig_dorm() */

/*===========================================================================
  FUNCTION DS_3GPP_BEARER_CNTXT_REORIG_ALL_DORM_BEARERS

  DESCRIPTION
    This function is responsible for reoriginating all the dormant calls.
  
  PARAMETERS   
    subs_id: Subscription Id
                     
  DEPENDENCIES
    None.
  
  RETURN VALUE 
    None.
  
  SIDE EFFECTS 
    None.
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_reorig_all_dorm_bearers
(
  sys_modem_as_id_e_type subs_id
)
{
  uint32               index;
  ds_bearer_context_s* bearer_context_p;
  boolean              found_bearer_instance = FALSE;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
       (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",subs_id);
    return;
  }

  /*-------------------------------------------------------------------------
    Loop through all the bearers. Query the bit field corresponding to this 
    bearer in the uplink_data_bitmask. If the field is set, then call the 
    reorig_dorm function.
  -------------------------------------------------------------------------*/
  for(index=0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_context_p, subs_id))
    {
      found_bearer_instance = \
        DS_BEARER_CNTXT_QUERY_BITMASK (per_subs_info_hndl, bearer_context_p);

      if((found_bearer_instance == TRUE) &&
         ((bearer_context_p->ds_bearer_context_dyn_p->state == 
           DS_BEARER_CONTEXT_STATE_UP_DORMANT) || 
          (bearer_context_p->ds_bearer_context_dyn_p->state == 
           DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)))
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "Rab-Reorig for Bearer Inst:%d",
                          (uint32)bearer_context_p->ds_bearer_context_dyn_p->
                          client_data_ptr);
        ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL);
      }
    }
  }
} /* ds_3gpp_bearer_cntxt_reorig_all_dorm_bearers */

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PROC_DORM_REORIG_CMD

DESCRIPTION
  This function is called when we process the command to reestablish RAB
  due to enqueuing data into the watermarks.

PARAMETERS   
  *cmd_ptr - ds cmd ptr

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
void ds_bearer_cntxt_proc_dorm_reorig_cmd
(
  const ds_cmd_type *cmd_ptr
)
{
  ds_bearer_context_s  *bearer_context_p = NULL;
  ds3gpp_per_subs_info_type* hndl = NULL;
  sys_modem_as_id_e_type   subs_id = SYS_MODEM_AS_ID_NONE;
  boolean                  ps_data_suspend = FALSE;
  boolean                  send_ps_data_avail = FALSE;
  ds_bearer_context_reorig_cmd_type*   data_ptr = NULL;
  ds_pdn_context_s*           pdn_cntx_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_3GPP_ERROR_LOG_0("NULL ptr passed, return");
    return;
  }

  data_ptr = (ds_bearer_context_reorig_cmd_type*)cmd_ptr->cmd_payload_ptr;

  bearer_context_p =(ds_bearer_context_s*)(data_ptr->bearer_context_p);
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /* free the packet here if valid */
    if (data_ptr->reorig_pkt_ptr != NULL)
    {
      dsm_free_packet( &data_ptr->reorig_pkt_ptr );
    }
    return;
  }

  pdn_cntx_ptr = (ds_pdn_context_s*)
                 bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  subs_id = bearer_context_p->ds_bearer_context_dyn_p->subs_id;
  ps_data_suspend = ds3gpp_get_ps_data_suspend_flag(subs_id);
  
  hndl = ds3gpp_get_per_subs_info_hndl(subs_id);
  
  /*Handle if suspend indication was recieved when there was no data in wm,
    will send data available indication to ds during enqueue*/
  if(ds3gpp_validate_per_subs_info_hndl(hndl) && ps_data_suspend)
  {
    if ((!hndl->sent_ps_data_available) &&
  	 PS_PHYS_LINK_FLOW_ENABLED(&(bearer_context_p->ds_bearer_context_dyn_p->
                                                                      phys_link)))
    {
      send_ps_data_avail = ds3gpp_is_ps_data_avail_ind_allowed
      (
         subs_id,
         ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_ptr)
       );
      ds_bearer_cntxt_suspend_ind( subs_id);
    }
  }

  /*-------------------------------------------------------------------------
   * Call the function to reorig
  -------------------------------------------------------------------------*/
  ds_bearer_cntxt_reorig_dorm(bearer_context_p, data_ptr->reorig_pkt_ptr);
  /* free the packet after logging  */
  if (data_ptr->reorig_pkt_ptr != NULL)
  {
    dsm_free_packet( &data_ptr->reorig_pkt_ptr );
  }

  /*-------------------------------------------------------------------------
    Set the reorig_cmd_enqueued to FALSE which indicates that we have 
    processed the reorig request on this bearer
  -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->reorig_cmd_enqueued = FALSE;  
} /*ds_bearer_cntxt_proc_dorm_reorig_cmd()*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_REORIG_DORM_TIMER_CB()

DESCRIPTION
  This function is registered as the timer call back. When the timer to do
  Rab_reestab retry expires, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS
   callback_data : Bearer context pointer registered with the timer

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_reorig_dorm_timer_cb
(
  void*              callback_data /* Pass Subs id here */
)
{
  ds_cmd_type               *cmd_ptr;
  sys_modem_as_id_e_type    *subs_id_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG0_HIGH_EX((sys_modem_as_id_e_type)(int)callback_data,
                       "Re orig dorm timer call back");

  /*
   * This function is executed in the timer call back.
   * So all we do here is to put a command to DS and exit
   */
  if( (cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type))) == NULL ||
       cmd_ptr->cmd_payload_ptr == NULL)
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DORM_REORIG_TMR_EXP;
  subs_id_p = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
  *subs_id_p = (sys_modem_as_id_e_type)(int)callback_data;
  ds_put_cmd_ext(cmd_ptr);

  return;
} /* ds_bearer_cntxt_reorig_dorm_timer_cb */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_DATA_SAMPLING_TIMER_CB

DESCRIPTION
  This function is registered as the timer call back. When the data sampling
  timer expires, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS
   callback_data : subs id 

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_data_sampling_timer_cb
(
  void*              callback_data /* Pass Subs id here */
)
{
  ds_cmd_type               *cmd_ptr;
  sys_modem_as_id_e_type    *subs_id_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*
   * This function is executed in the timer call back.
   * So all we do here is to put a command to DS and exit
   */
  if( (cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type))) == NULL ||
       cmd_ptr->cmd_payload_ptr == NULL)
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DATA_SAMPLING_TMR_EXP;
  subs_id_p = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
  *subs_id_p = (sys_modem_as_id_e_type)(int)callback_data;
  ds_put_cmd_ext(cmd_ptr);

  return;
} /* ds_bearer_cntxt_data_sampling_timer_cb */
/*===========================================================================
FUNCTION ds_bearer_cntxt_dpm_dereg_cb

DESCRIPTION
  This function is registered as the downlink watermark's gone empty function pointer.
  When the watermark is empty, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS
   callback_data : 

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_dpm_dereg_cb
(
  dsm_watermark_type *wm_p,
  void               *call_back_data
)
{
  ds_cmd_type         *cmd_ptr = NULL;
  void                **data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG0_HIGH("ds_bearer_cntxt_dpm_dereg_cb");

  if(wm_p == NULL)
  {
    DS_3GPP_ERROR_FATAL("Watermark Pointer is Null");
    return;
  }
  wm_p->gone_empty_func_ptr = NULL;
  wm_p->gone_empty_func_data = NULL; 
  /*
   * This function is executed in the timer call back.
   * So all we do here is to put a command to DS and exit
   */
  
  cmd_ptr = ds_allocate_cmd_buf(sizeof(void*));

  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DPM_DEREG;
  data_ptr = (void**)cmd_ptr->cmd_payload_ptr;
  *data_ptr = call_back_data;

    ds_put_cmd_ext(cmd_ptr);

  return;

} 


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_RESET_ACCESS_BARRING_RETRY

DESCRIPTION
  This function resets access_barring_retry 
      
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_reset_access_barring_retry
(
   void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  access_barring_rab_retry = FALSE;
}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CHECK_IF_MAX_RAB_RETRIES_PERFORMED

DESCRIPTION
  This function checks if maximum rab retries have been performed.
  The number is different if access_barring_rab_retry_counter is set
      
PARAMETERS
  ds_bearer_context_s* bearer_context_p

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_check_if_max_rab_retries_performed
(
  ds_bearer_context_s                 * bearer_context_p,
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl,
  sys_modem_as_id_e_type               subs_id
)
{
  boolean ret_val = FALSE;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return ret_val;
  }

  if (per_subs_info_hndl == NULL)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Per subs info hdl is NULL");
    return ret_val;
  }

  /*-------------------------------------------------------------------------- 
    If access barring retry is not enabled check for MAX_RAB_REQ_RETRY_COUNT
    However if access barring is enabled check for
    access_barring_rab_retry_counter
  --------------------------------------------------------------------------*/
  if((bearer_context_p->ds_bearer_context_dyn_p->rab_reestab_retry_cnt 
                  == ds_3gpp_nv_get_max_rab_reestab_retry_cnt(subs_id)) &&
                 (per_subs_info_hndl->ds_3gpp_reg_in_progress == FALSE)
                  && (!access_barring_rab_retry))
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Non access barring max rab retries attempted");
    ret_val = TRUE;
  }
  else if ((access_barring_rab_retry) && 
           bearer_context_p->ds_bearer_context_dyn_p->rab_reestab_retry_cnt 
           == access_barring_rab_retry_counter)
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Access barring max rab retries attempted");
    ret_val = TRUE;
  }

  return ret_val; 
}


/*===========================================================================
FUNCTION ds_bearer_cntxt_get_rab_retry_timer()

DESCRIPTION
  This function is to get Rab retry timer value from NV if available, 
  else a random timer value  between 100 and 400 ms.
  
PARAMETERS  
  subs_id         -   Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  random_timer val (uint32)
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
uint32 ds_bearer_cntxt_get_rab_retry_timer
(
  sys_modem_as_id_e_type     subs_id
)
{
  uint32 rab_retry_timer = 0;
  rab_retry_timer = ds_3gpp_nv_manager_get_rab_retry_timer (subs_id);

 /* If rab_retry_timer NV is valid set timer value with the NV value(in sec) */
  if (rab_retry_timer != 0)
  {
    rab_retry_timer = rab_retry_timer * 1000;
  }
  /*-------------------------------------------------------------------------
    else set the timer with the random value between
    DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME and 
    DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME * 4 to send out RAB
    reestablishment request again.
   -------------------------------------------------------------------------*/
  else
  {          
    rab_retry_timer = ps_utils_generate_16bit_rand_num_by_range(
          DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME, 
          DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME * 4 );
  }
  return rab_retry_timer;
} /*ds_bearer_cntxt_get_rab_retry_timer() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_PROC_REORIG_DORMTMR_EXP_CMD()

DESCRIPTION
  This function is called when we process the command posted to DS as a result
  of dormancy timer expiry. This simply calls the function to reestab RAB.
      
PARAMETERS
  client_data_ptr: Subs Id passed as Client Data Ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_proc_reorig_dormtmr_exp_cmd
(
  void* client_data_ptr 
)
{
  uint32 rab_retry_timer = 0;
  ds_bearer_context_s* bearer_context_p;/* Bearer context pointer */
  sys_sys_mode_e_type  current_sys_mode = SYS_SYS_MODE_NONE;
  sys_modem_as_id_e_type               *subs_id_p = 
                                         (sys_modem_as_id_e_type*)client_data_ptr;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (subs_id_p == NULL)
  {
    DS_3GPP_ERROR_LOG_0("subs_id_p is NULL");
    return;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(*subs_id_p);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",*subs_id_p);
    return;
  }  

  /* Set the timer_running flag to FALSE */
  per_subs_info_hndl->timer_running = FALSE;

  ds_3gpp_rab_reestab_transition_state_ev(DS_3GPP_PROC_RAB_REESTAB_RAND_TIMER_EXP,
                                       NULL,
                                       *subs_id_p);

  current_sys_mode = ds3g_get_current_network_mode_ex(*subs_id_p);

  if( DS_3GPP_IS_3GPP_CURRENT_MODE(current_sys_mode))
  {
    /*-------------------------------------------------------------------------
      If the OTA request to reestablish RAB is already pending, then we restart
      the timer, provided there are RABs that need to be set up. This is 
      determined by the uplink_data_bitmask being non-null.
    -------------------------------------------------------------------------*/
    if(per_subs_info_hndl->service_request_pending_flag == TRUE)
    {
      if(per_subs_info_hndl->uplink_data_bitmask != 0)
      {
        if (!access_barring_rab_retry)
        {
          rab_retry_timer = ds_bearer_cntxt_get_rab_retry_timer (*subs_id_p);
        }
        else
        {
          DS_3GPP_MSG1_MED_EX(*subs_id_p,
                              "Setting rab_retry_timer to ACCESS_BARRING "
                              "RETRY VAL of %d",
                              access_barring_rab_retry_timer);
          rab_retry_timer = access_barring_rab_retry_timer;
        }
    
        DS_BEARER_CNTXT_SET_TIMER (per_subs_info_hndl,rab_retry_timer);
      }
    }
    else
    {
      if(per_subs_info_hndl->uplink_data_bitmask != 0)
      {
        /*-----------------------------------------------------------------------
          Call the function to get a bearer context on which we can initiate a
          RAB reestablishment.
        -----------------------------------------------------------------------*/
        bearer_context_p = ds_bearer_cntxt_get_bearer_inst_to_reorig(*subs_id_p);
        if(ds_bearer_cntx_validate_bearer_context(bearer_context_p))
        {
          if (ds_3gpp_nv_get_max_rab_reestab_retry_cnt(*subs_id_p) != RAB_REESTAB_RETRY_CNT_INVALID)
          {
            
            /*-----------------------------------------------------------------
              Rab re-estab retry cnt mechanism will be triggerred only when 
              reject cause is not local reject from NAS.
            -----------------------------------------------------------------*/
            if (ds_bearer_cntxt_check_if_max_rab_retries_performed
               (bearer_context_p,per_subs_info_hndl, *subs_id_p))
            {
              ds_bearer_cntxt_handle_max_rab_reestab_retry_cnt(bearer_context_p);

              /*----------------------------------------------------------------- 
                Get the bearer context for the next bearer which has uplink 
                data to send.
              ------------------------------------------------------------------*/
              bearer_context_p = ds_bearer_cntxt_get_bearer_inst_to_reorig(*subs_id_p);
            }

            /*--------------------------------------------------------------------
              Check if the bearer_context is not NULL and uplink_data_bitmask is
              set. This is required to handle the scenario when multiple bearers
              have data to send and one bearer reaches max_retry_cnt, in such a 
              case DS would send service_request for the next bearer which has 
              uplink bitmask set.
            --------------------------------------------------------------------*/
            if (bearer_context_p != NULL && 
                per_subs_info_hndl->uplink_data_bitmask != 0)
            {
              DS_3GPP_MSG1_HIGH_EX(*subs_id_p, 
                                   "TIMER CB FOR Rab-Reorig called; Bearer Inst:%d",\
                                   (uint32)bearer_context_p->ds_bearer_context_dyn_p
                                   ->client_data_ptr);

              ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL);

              /*---------------------------------------------------------------
                Increment retry_cnt only if reject cause is not local
              ---------------------------------------------------------------*/
              if (per_subs_info_hndl->ds_3gpp_reg_in_progress == FALSE)
              {
                DS_BEARER_CNTXT_INC_RAB_REESTAB_RETRY_CNT(bearer_context_p);
              }
            }
          }
          else
          {
            DS_3GPP_MSG1_HIGH_EX(*subs_id_p,
                     "TIMER CB FOR Rab-Reorig called; Bearer Inst:%d",\
                     (uint32)bearer_context_p->ds_bearer_context_dyn_p
                              ->client_data_ptr);
            ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL);
          }
        }
        else
        {
          /*-------------------------------------------------------------------
            technically we should not reach here. If we do, the case is such
            that the uplink_data_bitmask is non null but we could not find a
            bearer that needs RAB to be set up. This could possibly be due to
            corruption of the bitmask. We ignore the reorig req and set the bitmask
            to NULL.
          -----------------------------------------------------------------------*/
          DS_3GPP_ERROR_LOG_0_EX(*subs_id_p, "Could not locate a bearer to reorig on. Ignoring cmd");
          per_subs_info_hndl->uplink_data_bitmask = 0;
        }
      }
    }
  }
  else
  {
    DS_3GPP_MSG0_HIGH_EX(*subs_id_p, "Sys mode non-3gpp, Not reorig'ing dormant bearer");
  }
  
  
  /*------------------------------------------------------------------------ 
    Reset the access barring rab retry 
    ------------------------------------------------------------------------*/
  ds_bearer_cntxt_reset_access_barring_retry();
 /*---------------------------------------------------------------------------- 
  Reset emm_reg flag here, So the next rab_rej is handled gracefully
 ----------------------------------------------------------------------------*/
  per_subs_info_hndl->ds_3gpp_reg_in_progress = FALSE;
  return;
} /* ds_bearer_cntxt_proc_reorig_dormtmr_exp_cmd() */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_DATA_ACTIVITY_SAMPLING_TIMER_EXP_CMD()

DESCRIPTION
  This function is called when we process the command posted to DS as a result
  of data sampling timer expiry. This simply calls the function to calculate
  if data activity has occured. If data acivity has not occured, we restart the
  timer, else return DATA_ACTIVITY_RESUMED and update UL/DL stats and STOP_TIMER
      
PARAMETERS
  client_data_ptr: Subs Id passed as Client Data Ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
 
  Things to take care here:
  1. Maybe during timer running,, all  bearers go DOWN -> clear timer
  2. maybe when timer is running, clients vote to deregister -> then clear
     timer, if running.PS will end an IOCTL for this.
  3. You will need to access the global stats in a critical section.
 
===========================================================================*/
void ds_bearer_cntxt_data_activity_sampling_timer_exp_cmd
(
  void* client_data_ptr 
)
{
  sys_modem_as_id_e_type              *subs_id_p = 
                                      (sys_modem_as_id_e_type*)client_data_ptr;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  uint64                               cur_uldl_wmk_cnt = 0;
  int16                                ps_error = DS_ENOERR;
  boolean                              is_data_resumed = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (subs_id_p == NULL)
  {
    DS_3GPP_ERROR_LOG_0("subs_id_p is NULL");
    return;
  }

  per_subs_info_hndl = ds_bearer_cntxt_get_per_subs_info_hndl(*subs_id_p);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl(per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id %d",*subs_id_p);
    return;
  }  
  
  if (per_subs_info_hndl->is_sampling_timer_running)
  {
    /*RESET TIMER*/
    DS_BEARER_CNTXT_CLR_SAMPLING_TIMER(per_subs_info_hndl);
  }
  else
  {
  /*------------------------------------------------------------------- 
    if sampling timer has been stopped already somewhere, then we have
    nothing to do
    ----------------------------------------------------------------*/
    DS_3GPP_MSG1_MED_EX(*subs_id_p,
                    "Sampling timer already stopped before timer expiry for"
                    "subs id %d",*subs_id_p);
    return;
  }

  

  if (!per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up)
  {
    /*------------------------------------------------------ 
     Not even initilized. i.e no bearer is UP
    ---------------------------------------------------------*/
    DS_3GPP_MSG0_LOW("NO bearer is UP yet");
    return;
  }

  /*---------------------------------------------------------- 
    If after timer expiry, the wmk cnt has changed, notify
    DATA_ACTIVITY_RESUMED and update UL/DL stats 
   
    if not , then restart the timer
    ---------------------------------------------------------*/
  cur_uldl_wmk_cnt = ds3gpp_get_total_uldl_wm_cnt_hdlr(*subs_id_p);

  DS_3GPP_MSG5_HIGH_EX(*subs_id_p,
                       "Current WM cnt 0x%x%x Prev WM cnt 0x%x%x for subs id %d",
                       QWORD_HIGH(cur_uldl_wmk_cnt),
                       QWORD_LOW(cur_uldl_wmk_cnt),
                       QWORD_HIGH(per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt),
                       QWORD_LOW(per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt),
                       *subs_id_p);

  if (cur_uldl_wmk_cnt != 
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt)
  {
    is_data_resumed = TRUE;

    /*-------------------------------------------------------- 
      Indicate PS_SYS_EVENT_DATA_ACTIVITY_RESUMED
      --------------------------------------------------------*/
     if (ps_sys_event_ind_ex(PS_SYS_TECH_3GPP,
                              PS_SYS_EVENT_DATA_ACTIVITY_RESUMED,
                             (ps_sys_subscription_enum_type)
                              ds3gsubsmgr_subs_id_cm_to_ds(*subs_id_p),
                              &is_data_resumed,
                              &ps_error) == -1 )
     {
       LOG_MSG_ERROR_1( "Error sending PS_SYS_EVENT_DATA_ACTIVITY_RESUMED ps_err %d",
                         ps_error);
     }
  }
  else
  {
     /*------------------------------------------------------- 
      Restart timer since data activity has not yet occured
      -------------------------------------------------------*/
    DS_BEARER_CNTXT_SET_SAMPLING_TIMER(per_subs_info_hndl,
                                       DATA_ACTIVITY_SAMPLING_TIMER_DURATION);
  }

  return;
} /* ds_bearer_cntxt_data_activity_sampling_timer_exp_cmd() */


#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_BEARER_FROM_EPS_ID

DESCRIPTION
  This function is used to find the bearer context, given a EPS bearer id

PARAMETERS
  IN
    eps_bearer_id_T - eps bearer id
    subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the bearer context if there is a EPS bearer ID match
  NULL if there is no match

SIDE EFFECTS
  None.

===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_bearer_from_eps_id
(
  eps_bearer_id_T        eps_bearer_id,
  sys_modem_as_id_e_type subs_id
)
{
  int                 bearer_cntx_inst;
  ds_bearer_context_s *bearer_context_p = NULL;
  ds_eps_bearer_context_s * eps_bearer_context = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(bearer_cntx_inst = 0;
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_inst++) 
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }

    eps_bearer_context = (ds_eps_bearer_context_s *)
      DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);

    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context))
    {
      continue;
    }

    if(eps_bearer_context->ds_eps_bearer_context_dyn_p->eps_bearer_id == eps_bearer_id)
    {
      DS_3GPP_MSG1_LOW("Found the bearer context with eps bearer id %d",eps_bearer_id);
      break;
    }
  }
  /*---------------------------------------------------------------------------
    If we are not able to find a match based on the call id, the following
    will be printed. NULL will be returned since bearer_context_p is NULL
  ---------------------------------------------------------------------------*/
  if(bearer_cntx_inst >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_MSG1_LOW("Cannot find the requested bearer context with eps bearer id: %d",
             eps_bearer_id);
    bearer_context_p = NULL;
  }

  return bearer_context_p;
} /* ds_bearer_cntxt_get_bearer_from_call_id() */

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_GET_RESEND_CALL_END

DESCRIPTION
  This function sends pdn_disconnect as CM returned error for CALL_CMD_END
  with cause CM_CALL_CMD_ERR_MODE_INFO_CHG_LTE.

PARAMETERS 
  call_id - call_id for which CALL_END error is returned.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ds_bearer_cntxt_resend_call_end
(
  byte  call_id
)
{
  boolean                 ret_val = FALSE;
  ds_bearer_context_s    *bearer_cntxt_p = NULL;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;

  bearer_cntxt_p = ds_bearer_cntxt_get_bearer_from_call_id((cm_call_id_type)call_id);

  if (NULL == bearer_cntxt_p)
  {
    DS_3GPP_ERROR_LOG_0("ds_bearer_cntxt_resend_call_end: Invalid bearer_cntxt_p");
    return;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntxt_p, &subs_id))
  {
    return;
  }

  ret_val = ds3g_msh_pdn_disconnect_req_ex(
                NULL,
                bearer_cntxt_p->ds_bearer_context_dyn_p->sdf_id,
                call_id,
                bearer_cntxt_p->ds_bearer_context_dyn_p->nsapi,
                FALSE,
                bearer_cntxt_p->ds_bearer_context_dyn_p->subs_id);

  DS_3GPP_MSG1_MED_EX(subs_id, "ds_bearer_cntxt_resend_call_end: PDN Disonnect returned %d",
                   ret_val);
} /* ds_bearer_cntxt_resend_call_end */

#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_STORE_CALL_ID 

DESCRIPTION
  This function stores the CM call id in the bearer context struct pointed to
  by bearer_context_p

PARAMETERS
  IN
    bearer_context_p     - Pointer to the bearer context
    call_id              - CM call id to update the bearer with

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_store_call_id
(
  ds_bearer_context_s *bearer_context_p,
  cm_call_id_type     call_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = call_id;
}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_BEARER_FROM_SDF_ID

DESCRIPTION
  This function is used to fetch the bearer context using the SDF Id.
  
PARAMETERS
  sdf_id - SDF ID to use to locate the bearer

DEPENDENCIES
  None.

RETURN VALUE
 bearer_context_p  - Pointer to Bearer context
 NULL if the bearer cannot be located

SIDE EFFECTS
  None.
===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_bearer_from_sdf_id(uint16 sdf_id)
{
  int loop_index;
  ds_bearer_context_s *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*------------------------------------------------------------------------
    Search into the table only if the SDF ID passed into this function is
    not invalid. 0xFFFF is treated as an invalid SDF Id. This is usually got
    during N/W init QOS calls. In such a case, we should not return a bearer
  ------------------------------------------------------------------------*/
  if(sdf_id != 0xFFFF)
  {
    /*------------------------------------------------------------------------
      Valid SDF ID. Perform search in the bearer context table.
    ------------------------------------------------------------------------*/
    for (loop_index = 0; loop_index < DS_3GPP_MAX_BEARER_CONTEXT; loop_index++)
    {
      bearer_context_p = ds_bearer_cntxt_get_instance_ptr(loop_index);

      if(ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p) && 
         bearer_context_p->ds_bearer_context_dyn_p->sdf_id == sdf_id)
      {
        break;
      }
    }/* FOR LOOP*/

    if (loop_index >= DS_3GPP_MAX_BEARER_CONTEXT)
    {
      bearer_context_p = NULL;
  }
  }

  return bearer_context_p;
} /* ds_bearer_cntxt_get_bearer_from_sdf_id */


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_SET_BEARER_STATE

DESCRIPTION
  This function is used to set the bearer to a particular state
  
PARAMETERS
  bearer_context_p  - Pointer to Bearer context
  state             - State to set the bearer context to

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_set_bearer_state
(
  ds_bearer_context_s* bearer_context_p,
  ds_bearer_context_state_e state
)
{
  sys_modem_as_id_e_type               bearer_subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  boolean                              is_data_resumed = FALSE;
  int16                                ps_error = DS_ENOERR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }
  bearer_context_p->ds_bearer_context_dyn_p->state = state;


#ifdef FEATURE_DATA_FLOW_MGMT
  if (state == DS_BEARER_CONTEXT_STATE_UP ) 
  {
    ds_3gpp_burst_hdlr_start_periodic_timer_if_required(
       bearer_context_p->ds_bearer_context_dyn_p->subs_id);    
  }

  if ((state == DS_BEARER_CONTEXT_STATE_DOWN) ||
      (state == DS_BEARER_CONTEXT_STATE_UP_DORMANT)) 
  {
    ds_3gpp_burst_hdlr_stop_periodic_timer_if_required(
       bearer_context_p->ds_bearer_context_dyn_p->subs_id);    
  }
#endif /*FEATURE_DATA_FLOW_MGMT */

  if (ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, &bearer_subs_id))
  {
    per_subs_info_hndl = ds_bearer_cntxt_get_per_subs_info_hndl(bearer_subs_id);

    if (!ds_bearer_cntxt_validate_per_subs_info_hndl(per_subs_info_hndl))
    {
      DS_3GPP_MSG1_LOW("Bearer Cntxt per subs info handle is invalid for"
                       "subs id %d",bearer_subs_id);
      return;
    }


    if (!per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up &&
         ds_3gpp_bearer_cntxt_is_any_bearer_up(bearer_subs_id) )
    {
      /*------------------------------------------------------- 
        Set the global ul dl stats counter to zero
        This is when first bearer comes up for this sub
        ------------------------------------------------------*/
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up = TRUE;
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt = 
                       ds3gpp_get_total_uldl_wm_cnt_hdlr(bearer_subs_id);

      /*--------------------------------------------------------------
        We will come to this condition when
        1. Client registers for data activity. Sampling timer starts
        2. 1st bearer comes up , doesnt send any data
        3. Bearer goes down. Since last bearer has gone down now
           we will clear the timer, but client is still registered
           for any data activity notifications.
        4. When a new bearer comes up, if this flag is TRUE and sampling
           timer is not running, restart sampling timer
           -----------------------------------------------------------*/
      if ( TRUE == per_subs_info_hndl->is_client_reg &&
           !per_subs_info_hndl->is_sampling_timer_running)
      {
        DS_BEARER_CNTXT_SET_SAMPLING_TIMER(per_subs_info_hndl,
                                           DATA_ACTIVITY_SAMPLING_TIMER_DURATION);
      }

      return;
    }

    /*----------------------------------------------------------- 
      During bearer bring up, it goes thru BEARER_DOWN STATE once.
      So we need to have the is_bearer_up if check so that we dont end
      up stopping the sampling timer unintentionally
      ----------------------------------------------------------*/

    if (per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up &&
        ds_3gpp_bearer_cntxt_is_last_bearer_down_per_sub(bearer_subs_id))
    {
      DS_3GPP_MSG1_HIGH_EX(bearer_subs_id,
           "All bearers down! Resetting UL DL Statistics for sub %d",
           bearer_subs_id);

      /*------------------------------------------------------------- 
        When last bearer is set to DOWN, if the timer is still running
        check for any data activity.
        Scenario:
        1. Timer starts runinng
        2. First bearer goes up and sends data
        3. Bearer goes down before timer expiry . We will clear timer
           and reset the global stats.
       
        Since last bearer goes down , we will clear the stats,
        but we need to inform client that data activity has occured (if any)
        and then clear the stats counter
        -----------------------------------------------------------------*/
      if (per_subs_info_hndl->is_sampling_timer_running)
      {
        if (per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt !=
             ds3gpp_get_total_uldl_wm_cnt_hdlr(bearer_subs_id))
        {
           /*-------------------------------------------------------- 
               Indicate PS_SYS_EVENT_DATA_ACTIVITY_RESUMED
             --------------------------------------------------------*/
          is_data_resumed = TRUE;
          if(ps_sys_event_ind_ex(PS_SYS_TECH_3GPP,
                                 PS_SYS_EVENT_DATA_ACTIVITY_RESUMED,
                                 (ps_sys_subscription_enum_type)
                                 ds3gsubsmgr_subs_id_cm_to_ds(bearer_subs_id),
                                 &is_data_resumed,
                                 &ps_error) == -1)
          {
            LOG_MSG_ERROR_1( "Error sending PS_SYS_EVENT_DATA_ACTIVITY_RESUMED ps_err %d",
                              ps_error);
          }
        }
          /*--------------------------------------------------------- 
           Reset the sampling timer if running because all bearers
           can go DOWN while timer is running
           ---------------------------------------------------------*/
        DS_BEARER_CNTXT_CLR_SAMPLING_TIMER(per_subs_info_hndl);
      }
      /*----------------------------------------------------------- 
        Re Set the ul dl stats counter since last standing Bearer is down
        ----------------------------------------------------------*/
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.is_bearer_up = FALSE;
      per_subs_info_hndl->ds_3gpp_uldl_wm_cnt.total_uldl_wmk_cnt = 0;
    }
  }
  ds_3gpp_log_active_bearers_info(bearer_context_p->
                                  ds_bearer_context_dyn_p->subs_id);
} /* ds_bearer_cntxt_set_bearer_state */

/*===========================================================================
FUNCTION ds_bearer_cntxt_CALL_RAB_RE_ESTAB_IND_HDLR

DESCRIPTION
  This function handles rab reject indication for 3GPP modes. This will in
  turn dispatch any mode specific functions
  
PARAMETERS  
  call_id         -   CM call id
  mode_info_ptr   -   mode specific call info
  end_params      -   Call end param
  subs_id         -   Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_call_rab_re_estab_ind_hdlr
(
  cm_call_id_type           call_id,
  cm_call_mode_info_s_type  *mode_info_ptr,
  cm_end_params_s_type      *end_params,
  sys_modem_as_id_e_type     subs_id
)
{
  ds_bearer_context_s                      *bearer_context_p = NULL;
  sys_sys_mode_e_type                       network_mode;
  ds_pdn_context_s                         *pdn_cntx_p = NULL;
  ps_iface_ioctl_bearer_tech_changed_type    tech_change_info;
  uint32                                    rab_retry_timer = 0;
  void                                     *sm_ptr = NULL;
  ds_bearer_cntxt_per_subs_info_type       *per_subs_info_hndl = NULL;
  ds_3gpp_fd_info_s*     report_p;
  int16                  dss_errno;
  boolean                                   ret_val = FALSE;
  boolean                                   is_mo_data_reestalish = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------    
    Get the 3GPP mode we are operating in
  -------------------------------------------------------------------------*/
  network_mode = ds3gpp_get_current_network_mode(subs_id);

  /*-------------------------------------------------------------------------    
    Fill in the call end parameters : DS manager uses passes this info to cm
    for a call release if we return an error from here\
    VRK:TODO move this to mode specific hdlrs
  -------------------------------------------------------------------------*/
  end_params->call_id   = call_id;

  if(network_mode == SYS_SYS_MODE_LTE)
  {
    end_params->info_type = CM_CALL_MODE_INFO_LTE;
  }
  else if(network_mode == SYS_SYS_MODE_WCDMA || 
          network_mode == SYS_SYS_MODE_GSM
#ifdef FEATURE_TDSCDMA
          ||
          network_mode == SYS_SYS_MODE_TDS
#endif
         )
  {
    end_params->info_type = CM_CALL_MODE_INFO_GW_PS;
  }

  /*-------------------------------------------------------------------------
    Get the bearer context from call id
  -------------------------------------------------------------------------*/
  bearer_context_p =  ds_bearer_cntxt_get_bearer_from_call_id(call_id);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return DS3G_FAILURE;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info hndl invalid for "
                       "Subs Id(CM):%d", subs_id);
    return DS3G_FAILURE;
  }
  if((bearer_context_p->ds_bearer_context_dyn_p->state == 
      DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)||
      ((bearer_context_p->tx_wm.wm_item.current_cnt != 0) &&
      ds_3gpp_bearer_cntxt_ex_is_bearer_dorm_or_suspend(bearer_context_p)))

  {
    is_mo_data_reestalish = TRUE;
  }

  if (bearer_context_p->ds_bearer_context_dyn_p->state == 
      DS_BEARER_CONTEXT_STATE_UP_DORMANT ||
      bearer_context_p->ds_bearer_context_dyn_p->state == 
      DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)
  {
    ds_bearer_cntxt_set_bearer_state(bearer_context_p, DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB);
    DS_3GPP_MSG2_HIGH_EX(subs_id, 
       "RAB reestab ind -from dormancy. call id:%d bearer instance:%d",
       call_id,
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  }
  else
  {
    DS_3GPP_MSG2_HIGH_EX(subs_id, 
       "RAB reestab ind call id:%d  bearer inst:%d during bearer setup",
       call_id,
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  }

  /*-------------------------------------------------------------------------
    Get the PDN cntx assigned to this bearer cntx
  -------------------------------------------------------------------------*/
  pdn_cntx_p = (ds_pdn_context_s*)
                   ds_bearer_cntxt_get_pdn_cntxt(bearer_context_p);

  if (!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p, &subs_id))
  {
    return DS3G_FAILURE;
  }
  
  /*-------------------------------------------------------------------------
   * Reset the service_request_pending_flag if the response if obtained 
   * as a result of trying to reorig using this bearer
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp == TRUE)
  {
    per_subs_info_hndl->service_request_pending_flag = FALSE;
    bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp = FALSE;
  }
  else
  {
    /*  checking if fast dormancy needs to be triggered for NW initiated 
        reestablishment */
    if ((ds_3gpp_bearer_cntxt_is_modem_fd_enabled()== TRUE)&&
        (network_mode != SYS_SYS_MODE_LTE)&&
        (bearer_context_p->ds_bearer_context_dyn_p->state == 
                          DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB))
    {
      /*  give Indication to PS to start FD algorithm*/
      report_p = (ds_3gpp_fd_info_s*) 
              modem_mem_alloc(sizeof(ds_3gpp_fd_info_s),MODEM_MEM_CLIENT_DATA);

      if (report_p != NULL)
      {
        report_p->timer_1_value = ds_3gpp_fd_info.timer_1_value;
        report_p->timer_2_value = ds_3gpp_fd_info.timer_2_value;
      }
      DS_3GPP_MSG0_HIGH_EX(subs_id, "NW Init RAB-Reestablishment, Start FD Ind to PS");
      if (0 > ps_sys_conf_set_ex(PS_SYS_TECH_3GPP,
                                 PS_SYS_CONF_START_FAST_DORMANCY,
       (ps_sys_subscription_enum_type)ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                                  report_p,
                                  &dss_errno))
     {
       DS_3GPP_MSG0_HIGH_EX(subs_id, "Sending Start fast dormancy Ind Failed");
     }
 
     if (report_p!=NULL)
     {
       modem_mem_free(report_p, MODEM_MEM_CLIENT_DATA);
     }
    }
  }


    /*--------------------------------------------------------------------------
      Get the throttling state macine and reset it. NULL checking need not be
      performed here as there is no dereferencing of the opaque handle in this
    function. 
    As per Latest VZW Spec, VZW LTE Data Retry Reqs spec:
    3.3.3 Per System Nature of Throttling, throttling SM needs to be cleared 
    on receiving unsolicited RAB_REESTAB_IND from NW.
    --------------------------------------------------------------------------*/
  sm_ptr = ds_3gpp_srv_req_get_throt_sm(FALSE, subs_id);
  ds_3gpp_srv_req_throt_reset(sm_ptr, subs_id);

  /*-------------------------------------------------------------------------
    Unset the bitmask corresponding to this bearer 
  -------------------------------------------------------------------------*/
  DS_BEARER_CNTXT_UNSET_BITMASK (per_subs_info_hndl, bearer_context_p);

  ds_3gpp_rab_reestab_transition_state_ev(DS_3GPP_PROC_RAB_REESTAB_IND,
                                       NULL,
                                       subs_id);
  /*-----------------------------------------------------------------------
    Reset the rab_reestab_retry counter for this bearer.
  -----------------------------------------------------------------------*/
  DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);

  if(per_subs_info_hndl->uplink_data_bitmask == 0)
  {
    if(per_subs_info_hndl->timer_running == TRUE)
    {
      DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl)
    }
  }
  else
  {
    if(per_subs_info_hndl->timer_running == FALSE)
    {
      rab_retry_timer = ds_bearer_cntxt_get_rab_retry_timer (subs_id);
      DS_BEARER_CNTXT_SET_TIMER (per_subs_info_hndl,rab_retry_timer)
    }
   }
  /*---------------------------------------------------------------------------
   * Now that we have the Reorigination done, we should reset the each enqueue 
   * function pointer to NULL so that sucessive enqueuing of packet to the WM 
   * do not cause a reorig request from being sent to NAS
  ---------------------------------------------------------------------------*/
  bearer_context_p->tx_wm.wm_item.each_enqueue_func_ptr = NULL;
  
  /*------------------------------------------------------------------------ 
    Reset the access barring rab retry 
  ------------------------------------------------------------------------*/
  ds_bearer_cntxt_reset_access_barring_retry();

  /*---------------------------------------------------------------------------
    Before proceeding, we have to make sure that there is no initiation to tear
    down the call. If there has been an attempt to tear down the call, we just
    have to return failure indicating CM that the lower layers will not be set
    up
  ---------------------------------------------------------------------------*/

  if(bearer_context_p->ds_bearer_context_dyn_p->state ==
     DS_BEARER_CONTEXT_STATE_GOING_DOWN)
  {
    DS_3GPP_MSG2_HIGH_EX(subs_id, 
       "Recd RAB REESTAB IND aft call tear down. Call Id:%d Bearer:%d",
       call_id,
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
    return DS3G_SUCCESS;
  }

  /*-------------------------------------------------------------------------    
    Fake out a bearer tech changed event for the PDN. This will cause QMI to
    query lower layers again for bearer rate and send a rate change ind to
    QMI clients. 
  -------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    fill in tech change info structure based for old mode 
  -------------------------------------------------------------------------*/
  ds_3gpp_pdn_cntx_get_bearer_tech_type(network_mode,
                                        &(tech_change_info.old_bearer_tech),
                                        subs_id);

  /*-------------------------------------------------------------------------
    fill in tech change info structure based for new mode 
  -------------------------------------------------------------------------*/
  ds_3gpp_pdn_cntx_get_bearer_tech_type(network_mode,
                                        &(tech_change_info.new_bearer_tech),
                                        subs_id);

  DS_3GPP_MSG0_HIGH_EX(subs_id, "Posting from RAB Re-estab indication");
  ds_3gpp_pdn_cntx_post_iface_generic_ind_for_pdn(pdn_cntx_p,
                                       IFACE_BEARER_TECH_CHANGED_EV,
                                       (void *)&tech_change_info);

  /* cache bearer tech change info at pdn context */
  ds_3gpp_pdn_cntx_cache_bearer_tech_info(pdn_cntx_p,
					  (void*)&tech_change_info);

  /*---------------------------------------------------------------------------
    Now dispatch the mode specific rab re-estab ind hdlr
  ---------------------------------------------------------------------------*/
  ret_val = ds_bearer_cntxt_rab_re_estab_ind(bearer_context_p, mode_info_ptr);

  /*-----------------------------------------------------------------------
    If no other bearer is active, It means this is the first bearer to go
    active. Hence send PS_CALL_ACTIVE_IND to DS3G flowmgr.
    This check is added to avoid sending duplicate indications to
    DS3Gflowmgr.

    if a PDN is in COMING_UP state, Flow mgr would be in ORIG state.
    Posting ACTIVE on the other PDN would result in starting the 
    flow monitor timer which could lead to data priority being lowered
    before the PDN is connected.
  -----------------------------------------------------------------------*/
  if( (ret_val == TRUE) &&
      (!ds_bearer_cntx_is_any_other_bearer_per_sub_active(bearer_context_p)) &&
      (!dsPdnContext_is_any_normal_pdn_coming_up_ex(subs_id)))
  {
    DS_3GPP_MSG0_MED_EX(subs_id, "PS_DATA_CALL_ACTIVE_IND");
    ds3geventmgr_notify_ps_call_event_per_sub(subs_id,
                                              DS3GEVENTMGR_PS_CALL_ACTIVE_EV,
                                              PS_SYS_NETWORK_3GPP);
  }
  if( (ret_val == TRUE) &&
      (!ds_bearer_cntx_is_any_other_bearer_per_sub_active(bearer_context_p)) )
  {

    /*-----------------------------------------------------------------------
      UE is entering into connect mode, notify so_mask updated to DSD
    -----------------------------------------------------------------------*/
    ds3gdsdif_notify_dsd_lte_mode_chg_event(subs_id, TRUE);

  }
  if((is_mo_data_reestalish == TRUE) &&
  	 (ret_val == TRUE))
  {
    ds3geventmgr_notify_mo_data_rab_reestab_event(
		                           ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                                   DS3GEVENTMGR_MO_DATA_RAB_REESTAB_PROC_START);
  }

  ds3gpp_enter_global_crit_section();
  /* Reset the last received rab reject reason */
  per_subs_info_hndl->rab_reject_reason = PS_SYS_3GPP_RAB_REJECT_UNKNOWN;
  ds3gpp_leave_global_crit_section();
  return ret_val;
} /* ds_bearer_cntxt_call_rab_re_estab_ind_hdlr */

/*===========================================================================
FUNCTION ds_bearer_cntxt_REORIG_DORM_CALL_CB

DESCRIPTION
  This function sends a command to DS task to trigger dormant re-orig. Called
  when data is enqueued into the Um Tx watermark. Since it runs in PS task,
  we are queuing a command to DS.
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.

===========================================================================*/
void ds_bearer_cntxt_reorig_dorm_call_cb
(
  dsm_watermark_type *wm_p,
  void               *call_back_data
)
{
  ds_cmd_type         *cmd_ptr = NULL;
  ds_bearer_context_s *bearer_context_p = NULL;
  ds_bearer_context_reorig_cmd_type *data_ptr = NULL;
  dsm_item_type                     *log_item = NULL; /* Packet to be logged */
  uint16                            logged_size = 0;
  boolean                           postDSCmd = TRUE;
  sys_modem_as_id_e_type            subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  bearer_context_p = (ds_bearer_context_s*)call_back_data;

  ds3gpp_enter_global_crit_section();
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }  
  
  /*------------------------------------------------------------------------
    No need to trigger dormant re-orig if the bearer is going down
  ------------------------------------------------------------------------*/
  if ( bearer_context_p->ds_bearer_context_dyn_p->state 
        == DS_BEARER_CONTEXT_STATE_GOING_DOWN)
  {
    DS_3GPP_C_MSG0(DS_3GPP_F3_MASK, "ds_bearer_cntxt_reorig_dorm_call_cb():"
                      "No re-orig as bearer is in GOING DOWN state");
    ds3gpp_leave_global_crit_section();
    return;
  }

  postDSCmd =
    (bearer_context_p->ds_bearer_context_dyn_p->reorig_cmd_enqueued == FALSE);

  if(postDSCmd)
  {
    DS_3GPP_C_MSG2(DS_3GPP_F3_MASK, "ds_bearer_cntxt_reorig_dorm_call_cb(): Each enqueue"
                  " function call back. Posting/Ignore [%d]"
                  " re-orig command on bearer %d to DS",
                  postDSCmd,
       (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));  
    /*------------------------------------------------------------------------
     * This function is executed in the PS context.
     * So all we do here is to put a command to DS and exit
    ------------------------------------------------------------------------*/
    cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_bearer_context_reorig_cmd_type));

    if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
    {
      ds3gpp_leave_global_crit_section();
      DS3GPP_ASSERT(0);
      return;
    }

    cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DORM_REORIG;
    data_ptr = (ds_bearer_context_reorig_cmd_type*)cmd_ptr->cmd_payload_ptr;
    data_ptr->bearer_context_p = (void*)bearer_context_p;
    data_ptr->reorig_pkt_ptr = NULL;

    /* Capture the first packet responsible for re-origination from dormancy for logging 
       purpose. This is done by Duping the first Packet in tx_wm (DSM Item) and passing 
       the same in Command Payload to command handler which does logging 
       Each enqueue is called as soon as Packet is enqueued and is protected under 
       DSM LOCK. So Duping the item here will make sure we have correct Packet info
       to log even if DSM item in WM gets freed somehow
       */
    if ((dsm_queue_cnt(&(bearer_context_p->tx_wm.wm_item)) == 1) &&
         (bearer_context_p->ds_bearer_context_dyn_p->state ==
                                      DS_BEARER_CONTEXT_STATE_UP_DORMANT))
    { 
       if((log_item = (dsm_item_type*)q_check(
           bearer_context_p->tx_wm.wm_item.q_ptr)) != NULL)
       {
           logged_size = dsm_dup_packet( &(data_ptr->reorig_pkt_ptr), log_item, 0, 
                                         PS_WMK_LOGGING_REORIG_PKT_LOG_SIZE );
           
           DS_3GPP_MSG1_HIGH_EX(subs_id, "Duping first re-orig packet from dormancy, size: %d",\
                                                                       logged_size);
       }
    }

    /*------------------------------------------------------------------------
      Here, we set the reorig_cmd_enqueued flag to true. This is to avoid
      multiple reorig commands from being enqueued for the same bearer when
      a bunch of packets are enqueued into the WM.

      This flag is cleared when we process this command which gives rise to a
      question as to what happens if a packet is subsequently enqueued into
      the WM. It is handled as follows: As soon as there is a reorig request,
      we disable flow on the phys link. Thus subsequent enqueuing of packets
      does not happen on the same bearer until the flow on the phys link is
      enabled. This way, we ensure that we do not overrun the DS command buf.

      Put the command only after setting reorig_cmd_enqueued flag to avoid 
      race condition between PS and DS task, race condition will happen if 
      DS processes the command before setting reorig_cmd_enqueued
    ------------------------------------------------------------------------*/
    bearer_context_p->ds_bearer_context_dyn_p->reorig_cmd_enqueued = TRUE;
    ds_put_cmd_ext(cmd_ptr);

  }
  ds3gpp_leave_global_crit_section();

} /* ds_bearer_cntxt_reorig_dorm_call_cb() */

/*===========================================================================
FUNCTION DS_BEARER_CONTEXT_NON_EMPTY_FUNC_CB

DESCRIPTION
  This function will be called in dsm when a bearer's tx wm become from empty 
  to non-empty.
  it will deregister non_empty_func_ptr and will send PS_CALL_ACTIVE_EV
  to ds3g
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.

===========================================================================*/
void ds_bearer_cntxt_non_empty_func_cb
(
  dsm_watermark_type *wm_p,
  void               *call_back_data
)
{
  ds_bearer_context_s    *bearer_context_p = NULL;
  ds_bearer_context_nonempty_cmd_type   *data_ptr = NULL;
  ds_cmd_type                           *cmd_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  bearer_context_p = (ds_bearer_context_s*)call_back_data;

  ds3gpp_enter_global_crit_section();
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }
  
  /*------------------------------------------------------------------------
     * This function is executed in the PS context.
     * So all we do here is to put a command to DS and exit
    ------------------------------------------------------------------------*/
  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_bearer_context_nonempty_cmd_type));

  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    ds3gpp_leave_global_crit_section();
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_NON_EMPTY_CB_CMD;
  data_ptr = (ds_bearer_context_nonempty_cmd_type*)cmd_ptr->cmd_payload_ptr;
  data_ptr->bearer_context_p = (void*)bearer_context_p;

  ds3gpp_leave_global_crit_section();
  ds_put_cmd_ext(cmd_ptr);
}/*ds_bearer_cntxt_non_empty_func_cb*/

/*===========================================================================
FUNCTION ds_bearer_cntxt_RAB_RELEASE_PDP_PROTO_STACK

DESCRIPTION
  This function registers a non_empty_function callback pointer in the
  PS to PDCP watermark used for PDP-IP calls. When data appears on the
  watermark again, the callback will be invoked to initiate  RAB
  re-establishment
  
PARAMETERS  
  bearer_context_p   - bearer context pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
LOCAL void ds_bearer_cntxt_rab_release_pdp_proto_stack
(
  ds_bearer_context_s        *bearer_context_p
)
{
  dsm_watermark_type    *tx_wm_p;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  DS_3GPP_MSG2_HIGH_EX(subs_id, "Uncoupling PDCP watermark for PDP-IP call Cid:%d Br In:%d",
      bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,(uint32)
                    bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*-------------------------------------------------------------------------  
    Get the PDCP state information for the call.
  -------------------------------------------------------------------------*/
  tx_wm_p = &(bearer_context_p->tx_wm.wm_item);

  /*-------------------------------------------------------------------------  
    Register each enqueue function ptr.
    Call id will be passed as parameter when watermark cb functions take a
    parameter.
  -------------------------------------------------------------------------*/  
  tx_wm_p->each_enqueue_func_ptr = ds_bearer_cntxt_reorig_dorm_call_cb;
  tx_wm_p->each_enqueue_func_data = (void*) bearer_context_p;
} /* dsBearer_rab_release_pdp_proto_stack() */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_RETRY_TIMER_VAL

DESCRIPTION
  This function gets the retry timer value for rab re-establishment
  
PARAMETERS  
  consult_throt_sm - boolean flag indicating if we should consult throt sm
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  timer val (uint32)
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
uint32 ds_3gpp_bearer_cntxt_get_retry_timer_val
(
  boolean                consult_throt_sm,
  sys_modem_as_id_e_type subs_id
)
{

  void                 *sm_ptr;      
  uint32               throt_timer_val;

  uint32               timer_val = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*----------------------------------------------------------------------------
    First get a random number between a 100 and 400 ms
    If access barring rab retry is supported simply set the rab retry timer to
    DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME
  ----------------------------------------------------------------------------*/
  if (!access_barring_rab_retry)
  {
    timer_val = ds_bearer_cntxt_get_rab_retry_timer (subs_id);
   
  }
  else
  {
    timer_val = access_barring_rab_retry_timer;
  }

  if(consult_throt_sm == TRUE)
  {
    /*----------------------------------------------------------------------
      We have to consult the SM since throttling has kicked in.
      Get a handle to the system wide throttling object. This is an opaque
      handle.
      Call this function with argument FALSE meaning we need not create a sm
      if it does not already exists. Here we are interested in only retrieving
      the SM for getting the back off timer
    ----------------------------------------------------------------------*/
    sm_ptr = ds_3gpp_srv_req_get_throt_sm(FALSE, subs_id);

    if(ds_3gpp_srv_req_throt_get_back_off_timer_val
         (sm_ptr,&throt_timer_val, subs_id) == TRUE)
    {
      if (throt_timer_val != 0) 
      {
        /*----------------------------------------------------------------------
          Throttling algorithm returned a non zero timer
          Use the value as dictated by the back-off algorithm

          If the throttling algorithm returned a zero timer, the random number
          between 100 and 400 ms will be used.
        ----------------------------------------------------------------------*/
        timer_val = throt_timer_val;
      }
      
    } // got a timer value from the throt sm
  }

  return timer_val;
} /* ds_3gpp_bearer_cntxt_get_retry_timer_val */


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_SEND_RAB_REJECT_NEEDED

DESCRIPTION
  Function to check if an IOCTL needs to be sent to the AP to inform about
  the rab reject cause code. This will only be called for certain operators
  that need the additional check.

PARAMETERS
  *emm_cause_ptr - cause-code to be evaluated

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_bearer_cntxt_send_rab_reject_needed
(
  cm_emm_failure_type  *emm_cause_ptr
)
{
  if (emm_cause_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("Null Pointer passed");
    return FALSE;
  }
  
  switch(emm_cause_ptr->cause_type)
  {
    case LTE_NAS_IRAT_CONN_EST_FAILURE:
    {
      if((emm_cause_ptr->detailed_cause== LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED) ||
         (emm_cause_ptr->detailed_cause== LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED_EAB) ||
          (emm_cause_ptr->detailed_cause== LTE_RRC_CONN_EST_FAILURE_CONN_REJECT))
      {
        if(emm_cause_ptr->access_barring_rem_time > 0)
        {
          return TRUE;
        }
        else
        {
          return FALSE;
        }
      }
      else
      {
        return TRUE;
      }
    }

    case LTE_NAS_IRAT_ACCESS_BARRING:
    {
      if (emm_cause_ptr->access_barring_rem_time > 0)
      {
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }

    default:
      return TRUE;
  }
} /* ds_3gpp_bearer_cntxt_send_rab_reject_needed */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_MAP_RAB_REJECT_CAUSE_CODES

DESCRIPTION
  Function to map the EMM cause codes for rab reject to 
  corresponding PS cause codes

PARAMETERS
  *rab_reject_reason_ptr  - ptr to converted rab reject cause code
  *emm_cause_ptr   - cause-code to be converted

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_bearer_cntxt_map_rab_reject_cause_codes
(
  cm_emm_failure_type  *emm_cause_ptr,
  ps_sys_3gpp_rab_reject_cause_code_type_enum  *rab_reject_reason_ptr
)
{
  if (rab_reject_reason_ptr == NULL || emm_cause_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("Null Pointer passed");
    return;
  }
  switch(emm_cause_ptr->cause_type)
  {
    case LTE_NAS_IRAT_SR_REJECT:
    {
      switch(emm_cause_ptr->detailed_cause)
      {
        case LTE_NAS_ILLEGAL_UE:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_ILLEGAL_MS;
          break;
        case LTE_NAS_ILLEGAL_ME:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_ILLEGAL_ME;
          break;
        case LTE_NAS_EPS_SERVICES_NOT_ALLOWED:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_EPS_SERVICES_NOT_ALLOWED;
          break;
        case LTE_NAS_UE_ID_CANNOT_BE_DERIVED_BY_NW:
          *rab_reject_reason_ptr = 
              PS_SYS_3GPP_RAB_REJECT_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK;
          break;
        case LTE_NAS_IMPLICITLY_DETACHED:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_IMPLICITLY_DETACHED;
          break;
        case LTE_NAS_PLMN_NOT_ALLOWED:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_PLMN_NOT_ALLOWED;
          break;
        case LTE_NAS_TRACKING_AREA_NOT_ALLOWED:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_TRACKING_AREA_NOT_ALLOWED;
          break;
        case LTE_NAS_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA:
          *rab_reject_reason_ptr = 
             PS_SYS_3GPP_RAB_REJECT_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA;
          break;
        case LTE_NAS_NO_SUITABLE_CELLS_IN_TRACKING_AREA:
          *rab_reject_reason_ptr = 
             PS_SYS_3GPP_RAB_REJECT_NO_SUITABLE_CELLS_IN_TRACKING_AREA;
          break;
        case LTE_NAS_CONGESTION:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_CONGESTION;
          break;
        case LTE_NAS_NOT_AUTHORIZED_FOR_THIS_CSG:
          *rab_reject_reason_ptr = 
             PS_SYS_3GPP_RAB_REJECT_NOT_AUTHORIZED_FOR_THIS_CSG;
          break;
        case LTE_NAS_NO_EPS_BEARER_CONTEXT_ACTIVATED:
          *rab_reject_reason_ptr = 
             PS_SYS_3GPP_RAB_REJECT_NO_EPS_BEARER_CONTEXT_ACTIVATED;
          break;
        default:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_UNKNOWN;
          break;
      }
      break;
    }

    case LTE_NAS_IRAT_CONN_EST_FAILURE:
    {
      switch(emm_cause_ptr->detailed_cause)
      {
        case LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED:
        case LTE_RRC_CONN_EST_FAILURE_ACCESS_BARRED_EAB:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_EMM_ACCESS_BARRED;
          break;
        case LTE_RRC_CONN_EST_FAILURE_CONN_REJECT:
          *rab_reject_reason_ptr = 
             PS_SYS_3GPP_RAB_REJECT_LRRC_CONN_EST_FAILURE_CONN_REJECT;
          break;
        default:
          *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_UNKNOWN;
          break;
      }
      break;
    }

    case LTE_NAS_IRAT_ACCESS_BARRING:
      *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_EMM_ACCESS_BARRED;
      break;

    case LTE_NAS_IRAT_T3417_EXPIRY:
      *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_EMM_T3417_EXPIRED;
      break;

    default:
      *rab_reject_reason_ptr = PS_SYS_3GPP_RAB_REJECT_UNKNOWN;
      break;
  }
} /* ds_3gpp_bearer_cntxt_map_rab_reject_cause_codes */

/*===========================================================================
FUNCTION      DS_3GPP_BEARER_CNTXT_GET_RAB_REJECT_REASON

DESCRIPTION
  Handler for the PS_SYS_IOCTL_3GPP_GET_LAST_RAB_REJECT_INFO IOCTL.
  Retrieves the latest rab reject cause code

PARAMETERS
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled



DEPENDENCIES
  None.

RETURN VALUE
  0  - Success
  -1 - Failure

SIDE EFFECTS
  None.
===========================================================================*/
int16 ds_3gpp_bearer_cntxt_get_rab_reject_reason
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
)
{
  ps_sys_3gpp_rab_reject_info_type  *rab_reject_info_ptr = NULL;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((arg_val_ptr == NULL) || (ps_errno == NULL))
  {
    if(ps_errno != NULL)
  {
    *ps_errno = DS_EFAULT;
    }
    DS_3GPP_MSG0_LOW("Null Pointer passed");
    return -1;
  }


  per_subs_info_hndl =
     ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",subs_id);
    return -1;
  }

  /*-----------------------------------------------------------------------
    Reset output buffer
   -----------------------------------------------------------------------*/
  rab_reject_info_ptr = (ps_sys_3gpp_rab_reject_info_type *)arg_val_ptr;
  memset(rab_reject_info_ptr, 0, sizeof(ps_sys_3gpp_rab_reject_info_type));

  ds3gpp_enter_global_crit_section();
  rab_reject_info_ptr->rab_reject_cause = per_subs_info_hndl->rab_reject_reason;
  ds3gpp_leave_global_crit_section();

  DS_3GPP_MSG1_HIGH ("Rab reject reason is %d",rab_reject_info_ptr->rab_reject_cause);

  *ps_errno = 0; /* No error */
  return 0;
} /*ds_3gpp_bearer_cntxt_get_rab_reject_reason() */


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_REORIG_FROM_DORM_ON_PEND_DATA_IN_TX_WM

DESCRIPTION
  This function attempts to reoriginate from dormancy if there is pending data
  in Tx WM.
  
PARAMETERS  
  bearer_context_ptr - Bearer context ptr
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  void 
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_reorig_from_dorm_on_pend_data_in_tx_wm
(
  ds_bearer_context_s *bearer_context_p
)
{
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  /*----------------------------------------------------------------------- 
    If there is pending data in Tx WM's trigger reorig request
    ----------------------------------------------------------------------*/

  if((bearer_context_p->tx_wm.wm_item.current_cnt != 0) 
      && (ds3gpp_get_ps_data_suspend_flag(subs_id) == FALSE))
  {
    DS_3GPP_MSG1_HIGH_EX(subs_id, "Data in Tx WM:Rab re-est immly:Bearer Ins:%d",
             (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
    ds_bearer_cntxt_reorig_dorm(bearer_context_p, NULL); 
  }

  return;

} /*ds_bearer_cntxt_reorig_from_dorm_on_pend_data_in_tx_wm */


/*===========================================================================
FUNCTION ds_bearer_cntxt_CALL_RAB_RELEASE_HDLR

DESCRIPTION
  This function handles rab release procedure for 3GPP modes.
  
PARAMETERS  
  call_id    - CM call id
  end_param  - call end parameters
  subs_id    - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_call_rab_release_hdlr
(
  cm_call_id_type           call_id,
  cm_end_params_s_type      *end_params,
  sys_modem_as_id_e_type     subs_id,
  lte_nas_local_cause_T     *lte_nas_local_cause
)
{
  ds_bearer_context_s    *bearer_context_p = NULL;
  boolean              retVal = DS3G_FAILURE;
  dsm_watermark_type   *tx_wm_ptr=NULL;       /* Watermark pointer      */
  int16                dss_errno;
  ds_3gpp_fd_info_s    *report_p;
  sys_sys_mode_e_type   network_mode = SYS_SYS_MODE_NONE;
  ds_pdn_context_s       *pdn_cntx_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
    Fill in the call end parameters : DS manager uses passes this info to cm
    for a call release if we return an error from here
  -------------------------------------------------------------------------*/
  network_mode = ds3gpp_get_current_network_mode(subs_id);
  end_params->call_id   = call_id;
  end_params->info_type = CM_CALL_MODE_INFO_LTE;

  /*-------------------------------------------------------------------------
    Get the bearer context from the call ID.
  -------------------------------------------------------------------------*/
  bearer_context_p = ds_bearer_cntxt_get_bearer_from_call_id(call_id);

  /*-------------------------------------------------------------------------
    Verify that the call instance is valid.
  -------------------------------------------------------------------------*/
  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_MSG2_HIGH_EX(subs_id, 
       "RAB Rel Hdlr: call id:%d Bearer instance:%d", call_id,
       (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

    /*-------------------------------------------------------------------- 
      if RAB release is received for OTASN PDN while timer is still running,
      immediately finish the refresh process
     
      ALso check if any other OTASN PDN is UP or not.If yes, we shall wait until
      timer expires
      -----------------------------------------------------------------*/
    pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
    ds_3gpp_pdn_cntxt_process_otasn_timer_cleanup(pdn_cntx_p, TRUE, subs_id);

    DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
    (
      dereg_pdcp, 
      bearer_context_p    
    );
    /* it is possible that we get bearer release in GSM mode after resume 
        during L2G CCO IRAT */
    if (network_mode == SYS_SYS_MODE_GSM) 
    {
       ds_bearer_cntxt_set_bearer_state(bearer_context_p, DS_BEARER_CONTEXT_STATE_UP_DORMANT);
       DS_3GPP_MSG0_HIGH_EX(subs_id, "RAB Rel Hdlr: in GSM mode, move bearer to UP_DORMANT");
       return DS3G_SUCCESS;
    }
    if(bearer_context_p->ds_bearer_context_dyn_p->state !=
       DS_BEARER_CONTEXT_STATE_UP_DORMANT)
    {
      /*---------------------------------------------------------------------
        Indicate to PS the Physlink pointer, Rx watermark count,
        Tx watermark count and dormancy status.
        Dormancy status is set to DORMANT
      ---------------------------------------------------------------------*/
      ds_bearer_cntxt_report_ps_wmk_logging(bearer_context_p,
                                            PS_WMK_LOGGING_DORMANT);

      /*---------------------------------------------------------------------
        Call is not dormant. Go ahead and put it to dormancy
      ---------------------------------------------------------------------*/
    ds_bearer_cntxt_rab_release_pdp_proto_stack(bearer_context_p);

      /*---------------------------------------------------------------------
      Send Phy link down ind to PS IFACE.
      ---------------------------------------------------------------------*/
      ps_phys_link_down_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                              PS_EIC_QOS_NETWORK_L2_LINK_RELEASED );

    tx_wm_ptr = &(bearer_context_p->tx_wm.wm_item);

    if(tx_wm_ptr->current_cnt < tx_wm_ptr->lo_watermark)
    {
      /*Need to enable flow as part of rab release so 
       that rab can be estabilished by socket app */
      ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p, 
        DS_FLOW_LTE_PDCP_MASK|DS_FLOW_LTE_PDCP_DNE_MASK|DS_FLOW_UMTS_MASK, FALSE);
    }
    /*-----------------------------------------------------------------------
        Change the state of the bearer context to reflect dormancy.

        However if the bearer is in PENDING RAB REG state, Post bearer up 
        indication so that we end up posting iface up ind. Also there is no
        need to re-orig immediately. This should happen before the bearer
        state is changed.
    -----------------------------------------------------------------------*/

      if(bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
      {
        /*-------------------------------------------------------------------
          Post bearer up indication to PDN context.
        -------------------------------------------------------------------*/
        bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr->bearer_up_ind_cb(\
                                 bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
                                 bearer_context_p, NULL);

        /*-------------------------------------------------------------------
          Change the state of the bearer context to reflect dormancy.
        -------------------------------------------------------------------*/
        ds_bearer_cntxt_set_bearer_state(bearer_context_p, DS_BEARER_CONTEXT_STATE_UP_DORMANT);
      }
      else
      {
        /*-------------------------------------------------------------------
         The Bearer is being released from a state other than PEND_RAB_REG

         If data is sitting in the Watermarks (PS to PDCP), trigger a rab 
         re-establishment immly. We could have lost Radio Link and
         transitioned to dormancy or the network explicily put us dormant
         when we had data to send. Just call the each enqueue function that
         would trigger rab re-estab 
        -------------------------------------------------------------------*/

        /*-------------------------------------------------------------------
          Change the state of the bearer context to reflect dormancy before 
          calling reorig_dorm. reorig_dorm checks if the bearer is in dormant
        -------------------------------------------------------------------*/
        ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                         DS_BEARER_CONTEXT_STATE_UP_DORMANT);
        ds_bearer_cntxt_reorig_from_dorm_on_pend_data_in_tx_wm(bearer_context_p);
      } // if bearer in PEND_RAB_REG state
    retVal = DS3G_SUCCESS;
  }
    else
    {
      /*---------------------------------------------------------------------
        Even if a RAB is received in DORMANT state check for any pending data
        and trigger re origination
      ---------------------------------------------------------------------*/
      DS_3GPP_MSG0_HIGH_EX(subs_id, "RAB release called while in Dormant state. "
                        "Ignoring event");

      ds_bearer_cntxt_reorig_from_dorm_on_pend_data_in_tx_wm(bearer_context_p);
      retVal = DS3G_SUCCESS;
    } /* Bearer state is not in UP DORMANT*/

    DS_BEARER_CTXT_VF_CALL_PER_BEARER
    (
      rab_rel_ind, 
      bearer_context_p,
      lte_nas_local_cause
    );


   if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA  ||
       bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE )
  {
    ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT 
    ds3gtputmgr_update_uplink_throughput_configure(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                                   FALSE);
#endif
  }

    /*-----------------------------------------------------------------------
      If no other bearer is active, It means this is the last bearer to go
      down. Hence send PS_CALL_INACTIVE_IND to DS3G flowmgr.
      This check is added to avoid sending duplicate indications to
      DS3Gflowmgr
    -----------------------------------------------------------------------*/
    if( !ds_bearer_cntx_is_any_other_bearer_per_sub_active(bearer_context_p) )
    {
      DS_3GPP_MSG0_MED_EX(subs_id, "PS_DATA_CALL_INACTIVE_IND");
      ds3geventmgr_notify_ps_call_event_per_sub(subs_id,
                                               DS3GEVENTMGR_PS_CALL_DORMANT_EV,
                                               PS_SYS_NETWORK_3GPP);

      /*-----------------------------------------------------------------------
       UE is exitting connect mode, update so_mask to DSD
      -----------------------------------------------------------------------*/
      ds3gdsdif_notify_dsd_lte_mode_chg_event(subs_id, FALSE);

    }

  } /* bearer context pointer is non-null*/
  
  /*  Stop Fast Dormancy if this bearer is the last Active bearer */ 
  if((ds_3gpp_bearer_cntxt_is_modem_fd_enabled() == TRUE)&&
     (network_mode != SYS_SYS_MODE_LTE)&&
     (ds_bearer_cntx_is_any_other_bearer_up(bearer_context_p) == FALSE))
  {  
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Last bearer Put into Dormant state, Ind to stop FD");
    report_p = (ds_3gpp_fd_info_s*) 
              modem_mem_alloc(sizeof(ds_3gpp_fd_info_s),MODEM_MEM_CLIENT_DATA);

    if (0 > ps_sys_conf_set_ex(PS_SYS_TECH_3GPP,
                               PS_SYS_CONF_STOP_FAST_DORMANCY,
    (ps_sys_subscription_enum_type)ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                               (void*)report_p,
                               &dss_errno))
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Sending Stop fast dormancy Ind Failed");
    }

    if (report_p != NULL)
    {
      modem_mem_free(report_p, MODEM_MEM_CLIENT_DATA);
    }
  }
  return retVal;
} /* ds_bearer_cntxt_call_rab_release_hdlr */

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_CALL_RAB_RE_ESTAB_FAIL_HDLR

DESCRIPTION
   This function is called by 3G DSManager when the RAB re-establishment
failure event is received.
RABM signalling procedures succeeded but NO RAB was established. Spec
does not have any procedures to recover from this. So go ahead and
start call release now by returning an error: 3G DS manager will initiate
call release when this fn returns an error.

PARAMETERS   
  call_id,        - Call ID 
  *end_params     - CM specific call end parameters
  subs_id         - Subscription Id
  
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_FAILURE : so that the call is torn down 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_call_rab_re_estab_fail_hdlr
(
  cm_call_id_type           call_id,
  cm_end_params_s_type      *end_params,
  sys_modem_as_id_e_type     subs_id
)
{
  ds_bearer_context_s*                 bearer_context_p; 
  uint32               ran_num;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Get the bearer context from the call id
  -------------------------------------------------------------------------*/
  bearer_context_p =  ds_bearer_cntxt_get_bearer_from_call_id(call_id); 

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_MSG1_LOW ("RAB reestab fail. Call id:%d. Could not locate bearer",
                       call_id);
    return DS3G_FAILURE;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for Subs Id(CM)"
                        ,subs_id);
    return DS3G_FAILURE;
  }

  DS_3GPP_MSG2_HIGH_EX(subs_id, 
     "RAB reestab fail call id:%d bearer instance:%d",
     call_id,(uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*-------------------------------------------------------------------------
   * Reset the service_request_pending_flag if the response if obtained as a 
   * result of trying to reorig using this bearer
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp == TRUE)
  {
    per_subs_info_hndl->service_request_pending_flag = FALSE;
    bearer_context_p->ds_bearer_context_dyn_p->srv_rsp_exp = FALSE;
  }

  /*-------------------------------------------------------------------------
   * Unset the bitmask corresponding to the call (bearer) that is to be ended
  -------------------------------------------------------------------------*/
  DS_BEARER_CNTXT_UNSET_BITMASK (per_subs_info_hndl, bearer_context_p);
  
  /*-----------------------------------------------------------------------
    Reset the rab_reestab_retry counter for this bearer.
  -----------------------------------------------------------------------*/
  DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);
 /*----------------------------------------------------------------------- 
  Reset Access Barring RAB retry
  -----------------------------------------------------------------------*/
  ds_bearer_cntxt_reset_access_barring_retry();
  if(per_subs_info_hndl->uplink_data_bitmask == 0)
  {
    if(per_subs_info_hndl->timer_running == TRUE)
    {
      DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl);
      ds_3gpp_rab_reestab_reset_state(subs_id);
    }
  }
  else
  {
    if(per_subs_info_hndl->timer_running == FALSE)
    {
      /*-----------------------------------------------------------------------    
        Set the timer with the random value between 
        DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME and 
        DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME * 4 to send out RAB
        reestablishment request again. Once the timer expires, the dormant call
        reorig function will be called and RAB reestablishment is tried again.
      -----------------------------------------------------------------------*/
      ran_num = ps_utils_generate_16bit_rand_num_by_range(
                          DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME, 
                          DS_BEARER_CNTXT_RAB_REESTABLISH_MIN_RETRY_TIME * 4);

      ds_3gpp_rab_reestab_transition_state_ev(
                       DS_3GPP_PROC_RAB_REESTAB_RAND_RETRY,
                                           (void *)ran_num,
                                            subs_id);
    }
  }
  
  ds_bearer_cntxt_set_bearer_state(bearer_context_p, DS_BEARER_CONTEXT_STATE_UP_DORMANT);
  DS_3GPP_MSG0_HIGH_EX(subs_id, "Setting each_enqueue_func_ptr in rab_re_estab_fail_hdlr");
 (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =
               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
 (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data = 
   (void*) bearer_context_p;


  DS_3GPP_MSG0_HIGH_EX(subs_id, "RAB re-estab fail: trigger call release");

  /* Call end reason is RAB failure */
  bearer_context_p->ds_bearer_context_dyn_p->down_reason =
     PS_NET_DOWN_REASON_RAB_FAILURE;
  bearer_context_p->ds_bearer_context_dyn_p->down_reason_ps_eic = 
    PS_EIC_NETWORK_RAB_FAILURE;
  
  /*-------------------------------------------------------------------------    
  Fill in the call end parameters : DS manager uses passes this info to cm
  for a call release. For now FAIL indication is given only in WCDMA. Hence
  we are filling the info type with code specific to GW.
  -------------------------------------------------------------------------*/
  end_params->call_id   = call_id;
  end_params->info_type = CM_CALL_MODE_INFO_GW_PS;
  
  return DS3G_FAILURE;

} /* dsWcdmaBearerCntxt_call_rab_re_estab_fail_hdlr() */
/*===========================================================================
FUNCTION  DS_3GPP_CC_SP_THROTTLING

DESCRIPTION
  This function is used for cause code specific throttling for UMTS 

PARAMETERS
rejected_pdn_cntx_p     - pointer to pdn_cntxt that got rejected
pdn_throt_p             - pointer to throttle sm
current_bearer_mode     - current mode of the bearer
down_reason             - pdn reject cause code given by the network

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_bearer_cntxt_cc_sp_throttling
(
  ds_pdn_context_s                 *rejected_pdn_cntx_p,
  sys_sys_mode_e_type               current_bearer_mode,
  ps_iface_net_down_reason_type     down_reason
)
{
  ds_umts_pdp_type_enum_type          pdn_pdp_type = DS_UMTS_PDP_MAX;
  ds_3gpp_pdn_throt_ident_info_type   throt_ident_info;
  sys_modem_as_id_e_type              subs_id;
  ds_3gpp_throttle_sm_cb_data_type    cb_data;
  /*--------------------------------------------------------------------------*/
  DS_3GPP_MSG0_LOW("in ds_bearer_cntxt_cc_sp_throttling");

  if (!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(rejected_pdn_cntx_p, &subs_id))
  {
    return;
  }
  
  pdn_pdp_type =
     rejected_pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
  
  if((pdn_pdp_type != DS_UMTS_PDP_IPV4) && 
     (pdn_pdp_type != DS_UMTS_PDP_IPV4V6) &&
     (pdn_pdp_type != DS_UMTS_PDP_IPV6) )
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "invalid pdn PDP type, returning");
    return;
  }
  if( (current_bearer_mode == SYS_SYS_MODE_WCDMA) ||
      (current_bearer_mode == SYS_SYS_MODE_GSM)   ||
      (current_bearer_mode == SYS_SYS_MODE_TDS) )
  {
    /*---------------------------------------------------------------------
      Perform regular throttling on the rejected PDP type -- IPV4V6
    -----------------------------------------------------------------------*/
     DS_3GPP_MSG0_HIGH_EX(subs_id, "Perform regular throt on both IP types");

     ds_3gpp_throt_fill_cb_data
       (&cb_data, (void*)current_bearer_mode, NULL, (void*)current_bearer_mode);

     ds_3gpp_pdn_throt_fill_ident_info_ex
       (&throt_ident_info, 
        rejected_pdn_cntx_p->ds_pdn_context_dyn_p->pdp_profile.context.apn,
        subs_id,
        FALSE,
        DS_3GPP_THROT_RAT_UMTS,
        DS_3GPP_PDN_THROT_PROC_GLOBAL,
        DS_3GPP_PDN_THROT_TYPE_CC_SP,
        DS_UMTS_PDP_IPV4V6,
        FALSE,
        down_reason,
        rejected_pdn_cntx_p,
        &cb_data); 
     
     ds_3gpp_pdn_throt_perform_reg_throttling(&throt_ident_info);

     ds_pdn_cntxt_recompute_ip_support_for_apn(rejected_pdn_cntx_p);
  }
}/*ds_bearer_cntxt_cc_sp_throttling*/

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_UPDATE_PHYS_LINK_FLOW_STATUS

DESCRIPTION
  This function enables/disables phys link flows for all active PDP contexts
  This function is called during InterRAT scenarios.  

PARAMETERS
  boolean disable_all_flow.
  subs_id Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_update_phys_link_flow_status
(
  boolean                disable_all_flow,
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s* bearer_context_p; //Pointer to bearer context
  uint32               bearer_cntx_inst;
  boolean              send_ps_data_avail = FALSE;
  ds_pdn_context_s*           pdn_cntx_ptr = NULL;
  /*-----------------------------------------------------------------------*/

  DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_bearer_cntxt_all_phys_link_flow_control,flow state: %d",
           disable_all_flow);
  /*-------------------------------------------------------------------------
    For all active bearer contexts, disable phys link flow
  -------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
     bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
     bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);    
            
    if(!ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_context_p, subs_id))
    {
      continue;
    }
            
    pdn_cntx_ptr = (ds_pdn_context_s*)
                 bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP ||
        bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT ||
        bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB )
    {

   
      DS_3GPP_MSG2_HIGH_EX(subs_id, "bearer_cntx_p 0x%x, flow state: %d",
               bearer_context_p, disable_all_flow);

      send_ps_data_avail = ds3gpp_is_ps_data_avail_ind_allowed
                           (
                              subs_id,
                              ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_ptr)
                            );

      if(!disable_all_flow && send_ps_data_avail )
      {
        /*-------------------------------------------------------------------
          Enable flow on the phys link.
        -------------------------------------------------------------------*/
        ds_3gpp_bearer_cntxt_enable_flow( bearer_context_p,
                                     DS_FLOW_UMTS_MASK, FALSE);

      }
      else
      {
        /*-------------------------------------------------------------------
          Disable flow on the phys link.
        -------------------------------------------------------------------*/
        ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p,
                                      DS_FLOW_UMTS_MASK, FALSE);
      }      

    }// valid for bearer state
    
  }// end for all bearer_contexts    

}/*ds_bearer_cntxt_update_phys_link_flow_status*/


/*===========================================================================
FUNCTION DSBEARERCNTXT_IS_PS_DATA_AVAILABLE

DESCRIPTION
   This function returns TRUE if data is available in any of the uplink
   buffers, otherwise, it returns FALSE.DS will check if there is UL data
  in the WM only if the bearer is in Dormant state Set Active_Flag to True;
  if there is data ;else False. 


PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If Data is available
  FALSE - If Data is not available

SIDE EFFECTS
  None.

===========================================================================*/
boolean dsBearerCntxt_is_ps_data_available
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s *bearer_context_p;
  uint32              bearer_cntx_inst;
  ps_phys_link_type   *phys_link_ptr = NULL;  
  boolean                         tlb_mode_b_c = FALSE;
#ifdef FEATURE_DATA_LTE
  ds_eps_tlb_per_subs_info_type*   eps_tlb_hndl = NULL;
#endif

  /*-----------------------------------------------------------------------*/  

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    VRK:TODO INTER-RAT Do we need this check?
    If there is a call in Coming or or going down state, return true so that 
    we don't loose PS signalling connection after RAU.
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_3GPP_MSG0_HIGH_EX(subs_id, "Checking for Uplink Data on DS UM watermark");
  /*--------------------------------------------------------------------------
    For each bearer in the bearer context table, check the UL WM.
  --------------------------------------------------------------------------*/

#ifdef FEATURE_DATA_LTE
    eps_tlb_hndl = ds_eps_tlb_get_per_subs_info_hndl(subs_id);

    if((ds_eps_tlb_validate_per_subs_info_hndl(eps_tlb_hndl)) &&
         ((eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_B) ||
          (eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_C) ))
    {
       tlb_mode_b_c = TRUE;
    }
#endif


  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
    ds3gpp_enter_global_crit_section();

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_context_p, subs_id))
    {
      ds3gpp_leave_global_crit_section(); 
      continue;
    }

    if((bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_PEND_REDIAL) ||
      (bearer_context_p->ds_bearer_context_dyn_p->state == 
       DS_BEARER_CONTEXT_STATE_COMING_UP))
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Bearer_inst:%d Pending Redial, ps_data_available",
                         bearer_cntx_inst);
      ds3gpp_leave_global_crit_section();
      return TRUE;
    }

    phys_link_ptr = &(bearer_context_p->ds_bearer_context_dyn_p->phys_link);

    if (PS_PHYS_LINK_GET_STATE(phys_link_ptr) == PHYS_LINK_RESUMING)
    {
      if (tlb_mode_b_c == FALSE) 
      {

      DS_3GPP_MSG2_HIGH_EX(subs_id, "Phys link: 0x%x is in RESUMING state, bearer_inst: %d",
                        phys_link_ptr,
                        bearer_context_p->ds_bearer_context_dyn_p->index);
      ds3gpp_leave_global_crit_section();
      return TRUE;
    }
    }

    DS_3GPP_MSG3_HIGH_EX(subs_id, "Bearer_inst:%d, state: %d, ul_reg_with_pdcp: %d",
                      bearer_cntx_inst,
                      bearer_context_p->ds_bearer_context_dyn_p->state,
                      bearer_context_p->ds_bearer_context_dyn_p->\
                                            ul_registered_with_ltepdcp);

   /*If all the bearers are in Up state ; do not check the WM always
      set Active_Flag to False.This is because; Active Flag in TAU will
        be used to reestabilish the bearers in dormant state.*/
    if(!bearer_context_p->ds_bearer_context_dyn_p->ul_registered_with_ltepdcp 
       && ( (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP)))
    {
    
      DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer_inst:%d UM watermark cnt:%d",
                         bearer_cntx_inst,
                         bearer_context_p->tx_wm.wm_item.current_cnt);
      if( bearer_context_p->tx_wm.wm_item.current_cnt > 0 )
      {
        DS_3GPP_MSG0_HIGH_EX(subs_id, "Uplink Data on DS UM watermark");
        ds3gpp_leave_global_crit_section();
        return TRUE;
      }
      
      DS_3GPP_MSG1_HIGH_EX(subs_id, "tx_wm wm_item each_enqueue func ptr is 0x%x",
                        (bearer_context_p->tx_wm.wm_item)
                                              .each_enqueue_func_ptr);
     
    } /*  UP_DORMANT_REESTAB || UP_DORMANT || UP*/

    ds3gpp_leave_global_crit_section();
  }/* for all bearer contexts */

  return FALSE;
} /* dsBearerCntxt_is_ps_data_available */



/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PS_DATA_FAIL_IND_HDLR

DESCRIPTION
  This function enables phys link flows for all active PDP contexts, if we 
  are allowed to send data_available_ind based on the srv status.
  This function is called during InterRAT scenarios.  

PARAMETERS
  boolean allowed_to_send_data_avail.
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_ps_data_fail_ind_hdlr
(
  boolean                allowed_to_send_data_avail,
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s* bearer_context_p; //Pointer to bearer context
  uint32               bearer_cntx_inst;
  phys_link_state_type phys_link_state = PHYS_LINK_NULL;
  /*-----------------------------------------------------------------------*/

  DS_3GPP_MSG0_HIGH_EX(subs_id, "ds_bearer_cntxt_ps_data_fail_ind_hdlr ");
  /*-------------------------------------------------------------------------
    For all active bearer contexts, disable phys link flow
  -------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
     bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
     bearer_cntx_inst++ )
  {

    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal
        (bearer_context_p, subs_id))
    {
      continue;
    }

    if (bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP ||
        bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT ||
        bearer_context_p->ds_bearer_context_dyn_p->state == 
        DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB )
    {    
      if(!(allowed_to_send_data_avail))
      {      
        /*-------------------------------------------------------------------
          Enable flow on the phys link.
          ps_data_available/ps_data_fail_ind will only enable/disable flow
        -------------------------------------------------------------------*/
        
        DS_3GPP_MSG1_HIGH_EX(subs_id, "bearer_cntx_p 0x%x, disabling flow",bearer_context_p);

        phys_link_state = PS_PHYS_LINK_GET_STATE(&(bearer_context_p->\
                                               ds_bearer_context_dyn_p->phys_link));

        /*-------------------------------------------------------------------
          Flow control is ineffective in transient states. So we need to move
          phys link state to down before enabling the flow
        -------------------------------------------------------------------*/
        /*---------------------------------------------------------------------  
          Check phys link state and post phys link up. After getting resume, we 
          cannot blindly post flow_enable() as it will not take effect if phys
          link is in transition states.
        ---------------------------------------------------------------------*/  
        if (!( ( PHYS_LINK_UP   == phys_link_state) || 
               ( PHYS_LINK_DOWN == phys_link_state) ) )
        {
          ps_phys_link_down_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->
                                     phys_link), PS_EIC_QOS_NETWORK_SVC_NOT_AVAILABLE);
        }
        ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p,
                                      DS_FLOW_UMTS_MASK, FALSE);
      }
    }// for UP, UP_DORMANT, UP_DORMANT_REESTAB bearers
    
  }// end for all bearer_contexts    

}/*ds_bearer_cntxt_ps_data_fail_ind_hdlr*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_SYS_CHANGE_HANDLER

DESCRIPTION
   This function updates the DSMGR call table with the new sys mode

PARAMETERS
   new_mode
   subscription id

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_sys_change_handler
(
  sys_sys_mode_e_type    new_mode,
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s *bearer_context_p;
  uint32              bearer_cntx_inst;
  /*-----------------------------------------------------------------------*/    

  /*--------------------------------------------------------------------------
    For each bearer in the bearer context table, update the nw mode
  --------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }

    /*---------------------------------------------------------------------
      If moving from L <-> G/W, do not update the call_mode for any active
      or redial pending call, call mode will be updated later
    ---------------------------------------------------------------------*/
    if ((new_mode == SYS_SYS_MODE_LTE)||(bearer_context_p->
                     ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE))
    {
      if ( bearer_context_p->ds_bearer_context_dyn_p->state != DS_BEARER_CONTEXT_STATE_DOWN )
      {
        continue;
      }
      else
      {
        DS_3GPP_MSG3_HIGH_EX(subs_id, "call id:%d call mode not changed, bearer state: %d, call mode: %d",
                          bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                          bearer_context_p->ds_bearer_context_dyn_p->state,                        
                          bearer_context_p->ds_bearer_context_dyn_p->call_mode);
      }
    }

    /*-------------------------------------------------------------------
      Update the call_mode in bearer context and the DSMGR call table,
      only if the bearer is not in COMING_UP or GOING_DOWN state.     
    -------------------------------------------------------------------*/
    if( bearer_context_p->ds_bearer_context_dyn_p->state != 
        DS_BEARER_CONTEXT_STATE_PEND_REDIAL ) 
    {
      bearer_context_p->ds_bearer_context_dyn_p->call_mode = new_mode; 
      if(bearer_context_p->ds_bearer_context_dyn_p->cm_call_id != CM_CALL_ID_INVALID)
      {   
          
        DS_3GPP_MSG2_HIGH_EX(subs_id, "Changing mode for call id:%d :mode:%d",
                  bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,new_mode);

        if( !ds3g_msh_set_new_call_mode(bearer_context_p->ds_bearer_context_dyn_p
                                        ->cm_call_id,
                new_mode ) )
        {
          DS_3GPP_ERROR_LOG_1_EX(subs_id, "Mode set failed for call id :d", 
                    bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);
        } 
       
      } /* !CM_CALL_ID_INVALID */
    }/* !DS_BEARER_CONTEXT_STATE_PEND_REDIAL */

  }/* for all bearer contexts */

  return;
} /* ds_bearer_cntxt_sys_change_handler */

/*===========================================================================
FUNCTION DS_BEARER_CNTX_ACTIVATE_CNF

DESCRIPTION
  This is the function that is called from 3gpp_hdlr on call connected.
  This in turn invoked GPRS or WCDMA bearer level functions.
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
  mode_info_p       - mode info params
  end_params_p      - call end parameters
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean 
ds_bearer_cntxt_activate_cnf
(
  ds_bearer_context_s             *bearer_context_p,
  const cm_call_mode_info_s_type  *mode_info_p,
  cm_end_params_s_type            *end_parms_p,
  sys_modem_as_id_e_type           subs_id
)
{  boolean rval = DS3G_FAILURE;
   /*------------------------------------------------------------------------*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return rval;
  }  

  DS_BEARER_CTXT_VFR_CALL_PER_BEARER
  (
    rval, 
    activateCnf, 
    bearer_context_p, 
    mode_info_p, 
    end_parms_p
  );

  return rval;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_DATA_TRFR_ACTIVITY

DESCRIPTION
  This function is used to find out if any data was transferred in the UL
  or DL direction in the previous polling period.

  This function just calls the mode specific function to query data activity
  in the lower layers
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
   TRUE - if any data was Tx or Rx in the polling interval
   FALSE - otherwise
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_bearer_cntxt_get_data_trfr_activity
(
  ds_bearer_context_s  *bearer_context_p
)
{
  boolean  rval = FALSE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return rval;
  }  

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    get_data_trfr_activity, 
    subs_id,
    bearer_context_p
  );

  return rval;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_GET_BEARER_TYPE

DESCRIPTION
  This function is used to get the underlying bearer type. This makes a call
  to the mode specific bearer function and populates the incoming param.
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
  bearer_type_p     - bearer_type structure that has to be filled in
  ps_errno          - error code to be returned to PS in case of failure.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
   Integer value is returned.
   0 - SUCCESS
  -1 - FAILURE
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
int ds_3gpp_bearer_cntxt_get_bearer_type
(
  ds_bearer_context_s             *bearer_context_p,
  ps_iface_bearer_technology_type *bearer_type_p,                          
  sint15                          *ps_errno
)
{
  int rval = 0;
  sys_sys_mode_e_type    network_mode = SYS_SYS_MODE_NONE;
  ds_pdn_context_s       *pdn_cntx_p = NULL;  
  sys_modem_as_id_e_type  subs_id  = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( bearer_type_p == NULL || 
      !ds_bearer_cntx_validate_bearer_context(bearer_context_p) ) 
  {
    DS_3GPP_ERROR_LOG_0("bearer_type_p is NULL or "
                        "Bearer Context passed is invalid");
    *ps_errno = DS_EFAULT;
    return -1;
  }

  pdn_cntx_p = (ds_pdn_context_s*)
               (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr);

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p, &subs_id))
  {
    *ps_errno = DS_EFAULT;
    return -1;
  }

  /*--------------------------------------------------------------------------
    We are in one of the 3GPP modes of operation
    Configure the current network to be 3GPP. This is just an alias to 
    PS_IFACE_NETWORK_UMTS
  --------------------------------------------------------------------------*/
  bearer_type_p->current_network = PS_IFACE_NETWORK_3GPP;

  /*--------------------------------------------------------------------------
    Initialize the RAT to be NULL. This is usually returned to the upper layer
    if we are "out of service"
  --------------------------------------------------------------------------*/
  bearer_type_p->data.umts_type.rat_mask = PS_IFACE_3GPP_NULL_BEARER;

  /*--------------------------------------------------------------------------
    Under the following conditions, 3GPP IOCTL hdlr should return -1
      * When the mode maintained by DSMGR is a non 3GPP mode
      * When we are not in service.
    Get the network mode and the service status from DSMGR

    If we are in one of the 3GPP modes but OOS, we return NULL BEARER RAT
  --------------------------------------------------------------------------*/
  network_mode = ds_3gpp_get_apn_current_mode (pdn_cntx_p);

  if( DS_3GPP_IS_3GPP_CURRENT_MODE(network_mode))
  {
    if(ds3g_get_current_srv_status_ex(subs_id) == SYS_SRV_STATUS_SRV)
    {
      /*------------------------------------------------------------------------
        We are in service.
        Invoke the mode specific function to fill in the RAT mask
      ------------------------------------------------------------------------*/
      DS_BEARER_CTXT_VFR_CALL
      (
        rval, 
        get_bearer_tech_type, 
        subs_id,
        bearer_type_p, 
        ps_errno,
        subs_id
      );
    }

  }

  return rval;
} /* ds_3gpp_bearer_cntxt_get_bearer_type */

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_GET_SO_MASK_PER_MODE

DESCRIPTION
  This function is used to fetch the SO mask info based on
  capability information such hsdpa_call_status for WCDMA or 
  egprs_support for GSM. 
  
PARAMETERS  
  subs_id: Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  SO mask
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
uint64 ds_3gpp_bearer_cntxt_get_so_mask_per_mode
(
  sys_sys_mode_e_type 	  mode,
  sys_modem_as_id_e_type  subs_id
)
{
  uint64 so_mask = DS_SYS_SO_EX_UNSPECIFIED;

  DS_BEARER_CTXT_VFR_CALL_PER_MODE
  (
    so_mask,
    getSoMask,
    mode,
    subs_id
  );

  return so_mask;
} /* ds_3gpp_bearer_cntxt_get_so_mask_per_mode */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_GET_SO_MASK

DESCRIPTION
This function is used to fetch the SO mask info based on
capability information such hsdpa_call_status for WCDMA or 
egprs_support for GSM. 

PARAMETERS  
subs_id: Subscription Id
  
DEPENDENCIES 
None.

RETURN VALUE 
SO mask
 
SIDE EFFECTS 
None.

===========================================================================*/
uint64 ds_3gpp_bearer_cntxt_get_so_mask
(
  sys_modem_as_id_e_type	subs_id
)
{
  uint64				      so_mask = DS_SYS_SO_EX_UNSPECIFIED;
  sys_sys_mode_e_type mode = ds3gpp_get_current_network_mode(subs_id);			 

  so_mask = ds_3gpp_bearer_cntxt_get_so_mask_per_mode(mode, subs_id);

  return so_mask;
} /* ds_3gpp_bearer_cntxt_get_so_mask */

#endif /* FEATURE_DATA_WLAN_MAPCON */

/*==========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_BEARER_RATE

DESCRIPTION This function returns the tx and rx rates of current call's
            channel.
    
PARAMETERS
   call_inst        - index into the call table
  *argval_ptr       - Bearer rate info will be put in here
  *ps_errno         - Ptr to error code (if any)
   subs_id          - Subscription Id
  
  
DEPENDENCIES
  None.
  
RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS 
  None
===========================================================================*/
int ds_3gpp_bearer_cntxt_get_bearer_rate
(
  uint32                   call_inst,
  void                     *argval_ptr,
  sint15                  *ps_errno,
  sys_modem_as_id_e_type   subs_id
)
{
  int rval = -1;

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    get_bearer_rate, 
    subs_id,
    call_inst,
    argval_ptr, 
    ps_errno,
    subs_id
  );
  
  return rval;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_EXAMINE_IF_THROT_NEEDED

DESCRIPTION
  This function is used to examine the cause code of rejection and determine
  if we need to perform throttling. This function in turn dispatches the mode
  specific handler to figure out if throttling is needed

PARAMETERS
  call_info_ptr  - Pointer to the call info to find out rejection cause
  subs_id        - Subscription Id
  rab throt info - Populate the throt info

DEPENDENCIES
  None.

RETURN VALUE
  void
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_bearer_cntxt_examine_if_throt_needed
(
  dsgwl_ps_call_info_u_type           *call_info_ptr,
  ds_3gpp_throt_rab_info_s            *rab_throt_info_ptr,
  sys_modem_as_id_e_type               subs_id
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*--------------------------------------------------------------------------
    Invoke the mode specific function that determines if we have to perform
    throttling.
  --------------------------------------------------------------------------*/
  DS_BEARER_CTXT_VF_CALL
  (
    examine_if_throt_needed, 
    subs_id,
    call_info_ptr,
    rab_throt_info_ptr,
    subs_id
  );

  return;
} /* ds_3gpp_bearer_cntxt_examine_if_throt_needed */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PS_TO_L2_FLOW_ENABLE_CB

DESCRIPTION
  This function sends a flow enable message to ds task
  
PARAMETERS  
  bearer_context_p   - bearer context pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_ps_to_L2_flow_enable_cb
(
 dsm_watermark_type *wm,
 /*lint -esym(715,wm) */ 
 /*lint -esym(818,wm) */ 
 void*              callback_data
)
{
  uint32                                           bearer_instance = 0;
  ds_bearer_context_s                              *bearer_context_p = NULL;
  sys_modem_as_id_e_type                           subs_id
                                                     = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type      um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type  tds_um_wm_threshold;
  ds_pdn_context_s                                 *pdn_cntx_p = NULL;
  ds_bearer_context_wm_s                           *tx_wm_p = NULL;
  uint32                                           wm_dne_level = 0;
  uint32                                           old_wm_dne_level = 0;
  uint32                                           wm_dne_q_level = 0;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  bearer_instance = (uint32) callback_data;

  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p
                  ->pdn_context_ptr;

  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }

  um_wm_threshold =  ds_3gpp_nv_get_um_wm_threshold(subs_id);
  tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);
  tx_wm_p = &(bearer_context_p->tx_wm);

  old_wm_dne_level = tx_wm_p->wm_item.dont_exceed_cnt;

  switch( bearer_context_p->ds_bearer_context_dyn_p->call_mode )
  {
    case SYS_SYS_MODE_LTE:
      wm_dne_level = ds_bearer_cntx_get_lte_ul_wm_level(bearer_context_p,
                                                        &um_wm_threshold,
                                                        UL_WM_DNE_INDEX,
                                                        subs_id);
      if(tx_wm_p->wm_item.dont_exceed_cnt != wm_dne_level)
      {
        dsm_set_dne(&tx_wm_p->wm_item, wm_dne_level);
      }
      wm_dne_q_level = ds_bearer_cntx_get_lte_ul_wm_level(bearer_context_p,
                                                        &um_wm_threshold,
                                                        UL_WM_DNE_QCNT_INDEX,
                                                        subs_id);
      if(tx_wm_p->wm_item.dne_q_cnt != wm_dne_q_level)
      {
        dsm_set_dne_q_cnt(&tx_wm_p->wm_item, wm_dne_q_level);
      }
      /*--------------------------------------------------------------------------
        remove flow control framework for Bi-Dir case after WM level < LOW
      --------------------------------------------------------------------------*/
      ds_3gpp_flow_control_disable_flow_control(bearer_context_p, subs_id);
      break;

    case SYS_SYS_MODE_GSM:
      if(tx_wm_p->wm_item.dont_exceed_cnt != um_wm_threshold.gprs_ul_dne)
      {
        dsm_set_dne(&tx_wm_p->wm_item, um_wm_threshold.gprs_ul_dne);
      }
      break;

    case SYS_SYS_MODE_TDS:
      wm_dne_level 
        = ds_bearer_cntx_get_tdscdma_ul_wm_level(bearer_context_p,
                                                 &tds_um_wm_threshold,
                                                 UL_WM_DNE_INDEX);
      if(tx_wm_p->wm_item.dont_exceed_cnt != wm_dne_level)
      {
        dsm_set_dne(&tx_wm_p->wm_item, wm_dne_level);
      }
      break;

    case SYS_SYS_MODE_WCDMA:
      wm_dne_level = ds_bearer_cntx_get_wcdma_ul_wm_level(bearer_context_p,
                                                          &um_wm_threshold,
                                                          UL_WM_DNE_INDEX);
      if(tx_wm_p->wm_item.dont_exceed_cnt != wm_dne_level)
      {
        dsm_set_dne(&tx_wm_p->wm_item, wm_dne_level);
      }
      break;

    default:
      break;
  }

  if( old_wm_dne_level != tx_wm_p->wm_item.dont_exceed_cnt )
  {
    DS_3GPP_MSG3_HIGH_EX(subs_id, "Uplink WM levels: DNE: %d HI: %d LO: %d",
                      tx_wm_p->wm_item.dont_exceed_cnt,
                      tx_wm_p->wm_item.hi_watermark,
                      tx_wm_p->wm_item.lo_watermark);
  }


#ifdef FEATURE_DATA_FLOW_MGMT
  ds_3gpp_burst_hdlr_post_flow_ctrl_cmd(bearer_context_p,
                                        TRUE,
                                        DS_FLOW_LTE_PDCP_MASK|DS_FLOW_LTE_PDCP_DNE_MASK);
#else
  /* No need to post message if wm is already in lo state 
  */
  if (!bearer_context_p->ds_bearer_context_dyn_p->tx_hi) 
  {
    DS_3GPP_MSG0_LOW("Not posting tx flow enable msg - state not hi");
    return;
  }

  /* Set tx wm state to lo
  */
  bearer_context_p->ds_bearer_context_dyn_p->tx_hi = FALSE;

  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                              DS_FLOW_LTE_PDCP_MASK, TRUE);
#endif /*FEATURE_DATA_FLOW_MGMT */

} /* ds_bearer_cntxt_ps_to_L2_flow_enable_cb() */
/*lint +esym(715,wm) */ 
/*lint +esym(818,wm) */ 

#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PS_TO_L2_FLOW_DISABLE_CB

DESCRIPTION
  This function is registered as the uplink watermark's dne function pointer.
  When the watermark's dne level is hit, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS 
   wm_p              -watermark pointer 
   callback_data     - callbvack data 
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/

void  ds_bearer_cntxt_ps_to_L2_flow_dne_disable_cb
(
 dsm_watermark_type   *wm,
 void*                callback_data
)
{
  uint32                                        bearer_index;
  ds_bearer_context_s                           *bearer_context_p = NULL;
  dsm_watermark_type                            *wm_ptr = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  for( bearer_index = 0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      continue;
    }

    wm_ptr= &(bearer_context_p->tx_wm.wm_item);
    
    if (wm_ptr == wm ) 
    {
      ds_3gpp_burst_hdlr_post_flow_ctrl_cmd(bearer_context_p,
                                            FALSE,
                                            DS_FLOW_LTE_PDCP_DNE_MASK);
    }
  }
} /* ds_3gpp_bearer_cntxt_ps_to_L2_flow_disable_cb() */
#endif 
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_PS_TO_L2_FLOW_DISABLE_CB

DESCRIPTION
  This function diables the Phys link flow. It also sets the tx_hi in bearer
  context structure

  Note: This function runs in PS task context since it is a WM call back fn.
  
  To reduce the latency of FC kicking in, this is being done this way today.
  
PARAMETERS  
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_ps_to_L2_flow_disable_cb
(
 dsm_watermark_type *wm,
 /*lint -esym(715,wm) */
 /*lint -esym(818,wm) */
 void*              callback_data
)
{
  uint32                                        bearer_instance;
  ds_bearer_context_s                           *bearer_context_p = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  bearer_instance = (uint32) callback_data;

  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }



#ifdef FEATURE_DATA_FLOW_MGMT
  ds_3gpp_burst_hdlr_post_flow_ctrl_cmd(bearer_context_p,
                                         FALSE,
                                         DS_FLOW_LTE_PDCP_MASK);
#else

  /* No need to post message if wm is already in hi state 
  */
  if (bearer_context_p->ds_bearer_context_dyn_p->tx_hi) 
  {
    DS_3GPP_MSG0_MED("Not posting tx flow disable msg - state not lo");
    return;
  }

  /* Set tx wm state to hi
  */
  bearer_context_p->ds_bearer_context_dyn_p->tx_hi = TRUE;
  /*----------------------------------------------------------------------------
    Disabling the phys link flow instead of sending a message and a command 
    subsequently.
  ----------------------------------------------------------------------------*/
  ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p,
                               DS_FLOW_LTE_PDCP_MASK, TRUE);
#endif  /*FEATURE_DATA_FLOW_MGMT */

} /* ds_3gpp_bearer_cntxt_ps_to_L2_flow_disable_cb() */


#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
FUNCTION ds_bearer_cntxt_ps_to_L2_gone_empty_cb

DESCRIPTION
  This function is registered as the uplink watermark's gone empty function pointer.
  When the watermark is empty, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS 
   wm_p              -watermark pointer 
   callback_data     -callback_data: 

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_bearer_cntxt_ps_to_L2_gone_empty_cb
(
  dsm_watermark_type *wm_p,
  void               *call_back_data
)
{
  ds_cmd_type         *cmd_ptr = NULL;
  void                **data_ptr = NULL;
  ds_bearer_context_s                     *bearer_context_p = NULL;
  boolean                                 postDSCmd = TRUE;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(wm_p == NULL)
  {
    DS_3GPP_ERROR_FATAL("Watermark Pointer is Null");
    return;
  }
 
  bearer_context_p = (ds_bearer_context_s*)call_back_data;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }  

  postDSCmd =
    (bearer_context_p->ds_bearer_context_dyn_p->gone_empty_cmd_enqueued == FALSE);
 
  if(postDSCmd)
  {
    cmd_ptr = ds_allocate_cmd_buf(sizeof(void*));
    if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
    {
      DS3GPP_ASSERT(0);
      return;
    }
    bearer_context_p->ds_bearer_context_dyn_p->gone_empty_cmd_enqueued = TRUE;
    //DS_3GPP_MSG0_HIGH("Posting cmd: ds_bearer_cntxt_ul_gone_empty_cb");
    cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_GONE_EMPTY_CB;
    data_ptr = (void**)cmd_ptr->cmd_payload_ptr;
    *data_ptr = call_back_data;
    ds_put_cmd_ext(cmd_ptr);
  }
  return;
}
#endif /*FEATURE_DATA_FLOW_MGMT */

/*lint +esym(715,wm) */ 
/*lint +esym(818,wm) */ 
#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION ds_bearer_cntxt_lte_deactivate_req_hdlr

DESCRIPTION
  This function processes LTE Deactivated Request from NAS and tries to clenup 
  Pending calls if any.
  
PARAMETERS  
  msgrtype - the id of the received message 
  dsmsg    - pointer to the received message 

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_lte_deactivate_req_hdlr
(
  msgr_umid_type              msgrtype,
  sys_modem_as_id_e_type      subs_id,
  const msgr_hdr_struct_type *dsmsg
)
{ 
  /*-----------------------------------------------------------------------*/

  if(msgrtype != LTE_RRC_DEACTIVATE_REQ )
  {
     return;
  }
  DS_3GPP_MSG0_HIGH_EX(subs_id,  " Rx'd LTE_RRC_DEACTIVATE_REQ from NAS ");   
  ds_bearer_cntxt_post_clr_pending_calls_cmd(subs_id);
  
}/*ds_bearer_cntxt_lte_deactivate_req_hdlr */
#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION ds_bearer_cntxt_post_clr_pending_calls_cmd

DESCRIPTION
  This function posts a command to DS Task to clear pending calls if any.
  
PARAMETERS  
  subs_id: Subscription Id

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_post_clr_pending_calls_cmd
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_cmd_type     *cmd_ptr;
  sys_modem_as_id_e_type *subs_id_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if( (cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type)) ) == NULL ||
       cmd_ptr->cmd_payload_ptr == NULL)
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_CALL_PENDING_CLR;
  DS_3GPP_MSG0_HIGH_EX(subs_id, "Posting CLR_PENDING_REDIAL_CMD to DS task");

  subs_id_p = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
  *subs_id_p = subs_id;
  ds_put_cmd( cmd_ptr );
     
  return;
} /* ds_bearer_cntxt_post_clr_pending_calls_cmd */

/*===========================================================================
FUNCTION ds_bearer_cntxt_GETORIGPARAMS

DESCRIPTION
  Populates applicable call origination params in ds_3gpp_orig_params_s, 
  including QOS and default (match all) TFT for default bearer. 
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
  mm_orig_params_p  - orig params
  ds3gi_call_info   - call info
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_get_orig_params
(
  ds_bearer_context_s           *bearer_context_p,
  ds_3gpp_orig_params_s         *mm_orig_params_p,                          
  const ds_3gpp_pdn_call_info_type *ds3gi_call_info
)
{
  boolean rval = DS3G_FAILURE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;
  /*-------------------------------------------------------------------------*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    getOrigParams, 
    subs_id,
    bearer_context_p, 
    mm_orig_params_p, 
    ds3gi_call_info
  );

  return rval;
}

/*===========================================================================
FUNCTION ds_bearer_cntxt_GETENDPARAMS

DESCRIPTION
  Populates call end params to use for bringing down the data call, 
  including reason code.
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
  end_params_p      - call end parameters
  ds3gi_call_info   - call info
  delay_disconnect  - disconnect to be delayed or not
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_get_end_params
(
  ds_bearer_context_s           *bearer_context_p,
  cm_end_params_s_type          *end_params,
  ds_3gpp_pdn_call_info_type       *ds3gi_call_info,
  boolean                       *delay_disconnect
)
{
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL_PER_BEARER
  (
    getEndParams, 
    bearer_context_p, 
    end_params, 
    ds3gi_call_info,
    delay_disconnect
  );
}

/*===========================================================================
FUNCTION ds_bearer_cntxt_deactivate_last_bearer_if_needed

DESCRIPTION
  This function checks 
  1. If all bearers are down
  2. If all PDN's are down

  If one of the above is not fulfilled a fres attach is not tried
 
PARAMETERS 
 bearer_context - The bearer which is being deactivated
 subs_id
 
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/

void ds_bearer_cntxt_deactivate_last_bearer_if_needed
(
   ds_bearer_context_s    *bearer_cntx_p,
   sys_modem_as_id_e_type  subs_id   
)
{
#ifdef FEATURE_DATA_LTE
  uint32                    bearer_cntx_inst= 0;  
  ds_bearer_context_s      *bearer_cntx_iter_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
    If GET_PDN_CONN_IND for LTE attach is cached then check if this is the
    last bearer that is getting decativated. If it is then post the command
    again for GET_PDN_CONN_IND
  -------------------------------------------------------------------------*/
  DS_3GPP_MSG3_HIGH_EX(subs_id,
                       "Subs Id(CM): %d, eps_attach_sm_state: %d,"
                       "Internal sys_mode: %d",
                       subs_id, ds_eps_attach_sm_get_state(subs_id),
                       ds3gpp_get_current_network_mode(subs_id));

  /*--------------------------------------------------------------------------
    If attach request is cached, attach SM can be in CACHED state or 
    Attach SM can be in NULL state and cached_seq_num would be valid, 
    this happens when Attach request is received while the PDN 
    is being torn down.
  ---------------------------------------------------------------------------*/
  if( (ds_eps_attach_sm_get_state(subs_id) == DS_EPS_CACHED_GET_PDN_CONN_IND) ||
      (ds_eps_pdn_cntx_is_pdn_conn_req_pending(subs_id)) )
  {
    DS_3GPP_MSG3_HIGH_EX(subs_id, "GET_PDN_CONN_IND for LTE Attach is cached,"
                      " checking for GOING_DOWN bearers",0,0,0);

    for( bearer_cntx_inst=0;\
         bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
         bearer_cntx_inst++ )
    {
      bearer_cntx_iter_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
  
     /* No need to consider bearer that got deactivate_cnf */
      if(!ds_bearer_cntxt_validate_bearer_subscription_internal
           (bearer_cntx_iter_p, subs_id))
      {
        continue;
      }
      
      /*------------------------------------------------------------------------ 
        If any bearer is not in down or going down or pending redial return
        If there exists any bearer UP/COMING_UP state then no point processing
        this function
        ------------------------------------------------------------------------*/
      if((bearer_cntx_iter_p->ds_bearer_context_dyn_p->state != 
          DS_BEARER_CONTEXT_STATE_DOWN) && 
         (bearer_cntx_iter_p->ds_bearer_context_dyn_p->state != 
          DS_BEARER_CONTEXT_STATE_GOING_DOWN) &&
         (bearer_cntx_iter_p->ds_bearer_context_dyn_p->state != 
          DS_BEARER_CONTEXT_STATE_PEND_REDIAL))
      {
        DS_3GPP_ERROR_LOG_2_EX(subs_id, "DEBUG_ATTACH: Bearer state is d for instance id d",
                           bearer_cntx_iter_p->ds_bearer_context_dyn_p->state,
                           (uint32)bearer_cntx_inst);
        return;
      }

      /*-------------------------------------------------------------------------
        If any other bearer is still stuck in GOING_DOWN state then we cannot
        process GET_PDN_CONN_IND yet
		Also if the current bearer in question is NULL and any other bearer
		is in PENDING REDIAL state, it means delay processing 
		GET_PDN_CONN_IND
      -------------------------------------------------------------------------*/
      if((bearer_cntx_iter_p->ds_bearer_context_dyn_p->state == 
          DS_BEARER_CONTEXT_STATE_GOING_DOWN)  ||
         ((bearer_cntx_p == NULL) && 
          (bearer_cntx_iter_p->ds_bearer_context_dyn_p->state == 
           DS_BEARER_CONTEXT_STATE_PEND_REDIAL)  &&
         (bearer_cntx_iter_p->ds_bearer_context_dyn_p->cm_call_id != CM_CALL_ID_INVALID)))        
      {
	DS_3GPP_MSG3_HIGH_EX(subs_id, "DEBUG_ATTACH: Bearer_inst:%d call_id:%d still waiting for call_end, "
                          "delaying GET_PDN_CONN_IND processing: State:%d",
                          (uint32)bearer_cntx_inst,
					      bearer_cntx_iter_p->ds_bearer_context_dyn_p->cm_call_id,
                          bearer_cntx_iter_p->
                          ds_bearer_context_dyn_p->state);
        return;
      }     

	  
      /*---------------------------------------------------------------------------
	    If any other bearer other than the one in question is in PENDING
		REDIAL state, delay GET_PDN_CONN because callend for the bearer in PENDING
		REDIAL needs to be received
	  ----------------------------------------------------------------------------*/

      if((bearer_cntx_iter_p->ds_bearer_context_dyn_p->state == 
          DS_BEARER_CONTEXT_STATE_PEND_REDIAL) &&
         (ds_bearer_cntxt_validate_bearer_subscription_internal
         (bearer_cntx_p, subs_id)) &&
         (bearer_cntx_iter_p->ds_bearer_context_dyn_p->index != 
          bearer_cntx_p->ds_bearer_context_dyn_p->index) &&
         (bearer_cntx_iter_p->ds_bearer_context_dyn_p->cm_call_id != CM_CALL_ID_INVALID))
      {
		DS_3GPP_MSG1_HIGH_EX(subs_id, "Still waiting for call end:%d in PENDING REDIAL state",
                          bearer_cntx_iter_p->ds_bearer_context_dyn_p->cm_call_id);
        return;
      }     
             
    }
    /*-------------------------------------------------------------------------
      No other bearer is in GOING_DOWN state, Also check if PDN context
      is in going down state,
      If PDN context is in going down state it means iface associated with the
      PDN needs to be brought down, Hence dont proprocess GET_PDN_CONN_IND here
      It will be processed when the PDN is cleaned up:w!
    -------------------------------------------------------------------------*/
    if (!ds_3gpp_pdn_cntxt_is_any_pdn_going_down(subs_id))
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "No Pending bearers, ready to process GET_PDN_CONN_IND");
      ds_eps_attach_sm_handle_last_bearer_deact(subs_id);    
    }
  }
#endif
  return;
} /*ds_bearer_cntxt_deactivate_last_bearer_if_needed*/

/*===========================================================================
FUNCTION ds_bearer_cntxt_DEACTIVATECNF

DESCRIPTION
   Called to indicate deactivate of Bearer Context. Tears down the data path
   including deregistration with lower layers if needed, and changes state to
   DOWN. Notifies PdnContext of state change. This is a virtual function and 
   calls mode specific function.
  
PARAMETERS  
  bearer_context_p   - bearer context obj ptr
  mode_info_p        - mode specific info from CM
  call_was_connected - was call connected
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_deactivate_cnf
(
  ds_bearer_context_s        *bearer_context_p,
  const ds3g_call_info_cmd_type  *call_info_p,
  uint8                      end_type,
  boolean                    call_was_connected
)
{  
#ifdef FEATURE_DATA_LTE
  boolean                                  is_dedicated_bearer = FALSE;
#endif /* FEATURE_DATA_LTE */
  ds_pdn_context_s*                        pdn_cntx_p = NULL;
  sys_modem_as_id_e_type                   subs_id  = SYS_MODEM_AS_ID_NONE;
  ps_sys_data_conn_pdn_status_enum_type    pdn_status = 
                                    PS_SYS_DATA_CONN_PDN_STATUS_UNSPECIFIED;
  /*-----------------------------------------------------------------------*/

  if (!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  pdn_cntx_p = (ds_pdn_context_s*)bearer_context_p->ds_bearer_context_dyn_p->
                pdn_context_ptr;

  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Free the PS timer allocated for FCF in TCP Bi-Direction case;
    It is possible that the call starts from G/W then IRAT to L, 
    so alloc/dealloc in 3gpp not eps
  -------------------------------------------------------------------------*/
   ds_3gpp_flow_control_free_fcf_timer(bearer_context_p);

  /*------------------------------------------------------------------------ 
    Before deactivating, check if pdn_contexts apn param change flag is set
    If set, make a few validations and make a decision, if the bearer and PDN
    should continue to remain in PENDING_REDIAL mode or move it to GOING_DOWN
    Basically if the validation fails, a redial on the same PDN is not
    possible and hence move the state to GOING DOWN.
   
    The redial mechanism is currently only supported for default bearers
  --------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_LTE
  if((pdn_cntx_p->ds_pdn_context_dyn_p->def_bearer_context_ptr != NULL) &&
    (memcmp(bearer_context_p, (ds_bearer_context_s *)
            pdn_cntx_p->ds_pdn_context_dyn_p->def_bearer_context_ptr, 
            sizeof(ds_bearer_context_s)) == 0))
  {
    if(pdn_cntx_p->ds_pdn_context_dyn_p->apn_param_chg_teardown == TRUE)
    {
      ds_eps_pdn_cntxt_move_bearer_from_pending_redial_to_down_if_needed
        (bearer_context_p);
    }
  }
  else
  {
    is_dedicated_bearer = TRUE;
  }
#endif /* FEATURE_DATA_LTE */

  /* Send Data connection status info to PS*/
  if ( bearer_context_p->ds_bearer_context_dyn_p->state == 
                         DS_BEARER_CONTEXT_STATE_COMING_UP)
  {
    pdn_status = PS_SYS_DATA_CONN_PDN_STATUS_REJECTED;
  }
  else
  {
    pdn_status = PS_SYS_DATA_CONN_PDN_STATUS_DEACTIVATED;
  }

  ds_3gpp_pdn_cntx_send_data_connection_status_info(bearer_context_p,
                                                    pdn_cntx_p,
                                                    &(call_info_p->mode_info),
                                                    pdn_status,
                                                    call_info_p->end_status);

  DS_BEARER_CTXT_VF_CALL_PER_BEARER
  (
    deactivateCnf, 
    bearer_context_p, 
    call_info_p,
    end_type,
    call_was_connected
  );

  if (ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    bearer_context_p->ds_bearer_context_dyn_p->cm_call_id = CM_CALL_ID_INVALID;
  }
  
  ds_bearer_cntxt_deactivate_last_bearer_if_needed(bearer_context_p,subs_id); 
  

#ifdef FEATURE_DATA_LTE
  
  /*-------------------------------------------------------------------------- 
    If pdn context is in PENDING REDIAL state and if apn param chg teardown is
    set to TRUE, send out a PDN connectivity request out.
   
    We need not send out redial request in case of dedicated bearers since
    they have to be torn down
    --------------------------------------------------------------------------*/
  if (ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p) &&
      ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    if((pdn_cntx_p->ds_pdn_context_dyn_p->state == 
        DS_PDN_CONTEXT_STATE_PEND_REDIAL) && 
       (pdn_cntx_p->ds_pdn_context_dyn_p->apn_param_chg_teardown) &&
       (is_dedicated_bearer == FALSE)
      )
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Sending out PDN connectivity req, Apn param chg");
      ds_eps_handle_apn_param_chg_redial(pdn_cntx_p,bearer_context_p);
    }
#endif /* FEATURE_DATA_LTE */

    if((pdn_cntx_p->ds_pdn_context_dyn_p->state == DS_PDN_CONTEXT_STATE_PEND_REDIAL) &&
        pdn_cntx_p->ds_pdn_context_dyn_p->redial_reason == DS_3GPP_REDIAL_OP_PCO_CHG)
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "Redial because of OP PCO chg");
      pdn_cntx_p->ds_pdn_context_dyn_p->redial_reason = DS_3GPP_NO_REDIAL;
      pdn_cntx_p->ds_pdn_context_dyn_p->state = DS_PDN_CONTEXT_STATE_COMING_UP;
      DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
      (
         redial,
         bearer_context_p    
      );
    }
  }
} /* ds_bearer_cntxt_deactivate_cnf */

/*===========================================================================
FUNCTION ds_bearer_cntxt_DORMANCYREORIG

DESCRIPTION
  Dormancy reorig actions happen here. This function pointer could be set to
  NULL if dormancy is not supported in some modes.
  
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_dormancy_reorig
(
  ds_bearer_context_s        *bearer_context_p
)
{
  
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL_PER_BEARER
  (
    dormancyReorig, 
    bearer_context_p,
    NULL
  );
}

/*===========================================================================
FUNCTION ds_bearer_cntxt_BEARERACTIVATEIND

DESCRIPTION
  Called to indicate berarer activation request indication.
  
PARAMETERS  
  call_id           - call ID
  mode_info_ptr_i   - mode info ptr
  subs_id           - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_bearer_activate_ind
(
  cm_call_id_type                  call_id,
  const cm_call_mode_info_s_type   *mode_info_ptr_i,
  sys_modem_as_id_e_type           subs_id
)
{
  boolean rval = DS3G_FAILURE;

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    bearerActivateInd, 
    subs_id,
    call_id, 
    mode_info_ptr_i
  );

  return rval;
}

/*===========================================================================
FUNCTION ds_bearer_cntxt_BRING_UP

DESCRIPTION
  This function is used to initiate a call. Since the CM API is different 
  for LTE and UMTS, this function pointer will be used to invoke the mode
  specific function to initiate a call in the corresponding mode.
  
PARAMETERS  
  info_p            - bearer context obj ptr
  call_info         - mode info params
  useDefaultDialStr - whether to use default dial string
  subs_id           - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_bring_up
(
  void                    *info_p,
  ds_3gpp_pdn_call_info_type *call_info,
  boolean                     useDefaultDialStr,
  sys_modem_as_id_e_type      subs_id
)
{
  boolean              rval = DS3G_FAILURE;


  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    bring_up, 
    subs_id,
    info_p, 
    call_info,
    useDefaultDialStr
  );

  return rval;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CONTEXT_GO_NULL

DESCRIPTION
  This function is used to cleanup the bearer. It calls the mode specific bearer
  to clean itself up.
  
PARAMETERS  
  bearer_cntx_p      - bearer context obj ptr
  down_reason        - reason for bearer deactivation
  call_was_connected - whether the call was connected or not.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None
 
SIDE EFFECTS 
 None 
===========================================================================*/
void ds_3gpp_bearer_cntxt_go_null
(
  ds_bearer_context_s * bearer_cntx_p,
  ds_3gpp_down_reason_t down_reason,
  boolean               call_was_connected
)
{
  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntx_p))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL_PER_BEARER
  (
    go_null,
    bearer_cntx_p, 
    down_reason, 
    call_was_connected
  );
  
  return;
}


/*===========================================================================
FUNCTION ds_bearer_cntxt_REDIAL

DESCRIPTION
  This function is used to redial a call. Since the CM API is different 
  for LTE and UMTS, this function pointer will be used to invoke the mode
  specific function to initiate a call in the corresponding mode.
  
PARAMETERS    
  call_info         - mode info params  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_redial
(  
  ds_bearer_context_s     *bearer_context_p
)
{
  boolean rval = DS3G_FAILURE;
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;
  /*------------------------------------------------------------------------*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return rval;
  }

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    redial, 
    subs_id,
    bearer_context_p    
  );

  return rval;
} /* ds_bearer_cntxt_redial */

/*===========================================================================
FUNCTION ds_bearer_cntxt_TEAR_DOWN

DESCRIPTION
  This function is used to initiate the tear down of a call. Since the APIs
  are different in UMTS and LTE modes, we need mode specific handlers. These
  function pointers are initialized in the corresponding mode specific files
  and are accessed while processing phys_link_down command
 
PARAMETERS  
  bearer_context_p  - bearer context obj ptr
  call_info         - call info params
  info_ptr          - void pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_tear_down_call
(
  ds_bearer_context_s     *bearer_context_p,
  ds_3gpp_pdn_call_info_type *call_info,
  void                    *info_ptr
)
{
  boolean rval = DS3G_FAILURE;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return rval;
  }

  DS_BEARER_CTXT_VFR_CALL_PER_BEARER
  (
    rval, 
    tear_down, 
    bearer_context_p, 
    call_info,
    info_ptr
  );

  return rval;
}

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_hs_status

DESCRIPTION
  This function returns the RAT MASK corresponding to the passed HS status.

PARAMETERS
  hs_status - the HS status
  subs_id   - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  RAT mask 

SIDE EFFECTS
  None.
  
===========================================================================*/
uint32 ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_hs_status
( 
  sys_hs_ind_e_type       hs_status,
  sys_modem_as_id_e_type  subs_id
)
{  
  uint32 rval = 0;

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    getratMaskforHs, 
    subs_id,
    hs_status,
    subs_id
  );

  return rval;
} /* ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_hs_status */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_get_cell_srv_ind

DESCRIPTION
  This function returns the cached cell srv ind.

PARAMETERS
  subs_id      - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  cell srv ind 

SIDE EFFECTS
  None.
  
===========================================================================*/
cm_cell_srv_ind_s_type ds_3gpp_bearer_cntxt_get_cell_srv_ind
(
  sys_modem_as_id_e_type  subs_id
)
{
  cm_cell_srv_ind_s_type rval = {-1,-1,-1,-1};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  DS_BEARER_CTXT_VFR_CALL
  (
    rval,
    getCellSrvInd,
    subs_id,
    subs_id
  );

  return rval;
} /* ds_3gpp_bearer_cntxt_get_cell_srv_ind */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_set_cell_srv_ind

DESCRIPTION
  This function sets the cached cell srv ind.

PARAMETERS
  cell_srv_ind - passed cell srv ind
  subs_id      - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_set_cell_srv_ind
(
  cm_cell_srv_ind_s_type  cell_srv_ind,
  sys_modem_as_id_e_type  subs_id
)
{
  DS_BEARER_CTXT_VF_CALL
  (
    setCellSrvInd, 
    subs_id,
    cell_srv_ind,
    subs_id
  );
} /* ds_3gpp_bearer_cntxt_set_cell_srv_ind */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_egprs_support

DESCRIPTION
  This function returns the RAT MASK corresponding to the passed egprs support.

PARAMETERS
  egprs_support - egprs supported or not
  subs_id       - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  RAT mask 

SIDE EFFECTS
  None.
  
===========================================================================*/
uint32 ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_egprs_support
( 
  sys_egprs_support_e_type  egprs_support,
  sys_modem_as_id_e_type    subs_id
)
{  
  uint32 rval = 0;

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    getratMaskforEgprsSupport, 
    subs_id,
    egprs_support,
    subs_id
  );

  return rval;
} /* ds_3gpp_bearer_cntxt_get_ps_rat_mask_for_egprs_support */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_get_prev_div_duplex

DESCRIPTION
  This function returns the previous lte div duplex reported by CM.

PARAMETERS
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  Last div duplex reported by CM.

SIDE EFFECTS
  None.
===========================================================================*/
sys_div_duplex_e_type ds_3gpp_bearer_cntxt_get_prev_div_duplex
(
  sys_modem_as_id_e_type  subs_id
)
{
  sys_div_duplex_e_type rval = 0;
/*-------------------------------------------------------------------------*/

  DS_BEARER_CTXT_VFR_CALL
  (
    rval, 
    get_div_duplex,
    subs_id,
    subs_id
  );

  return rval;
} /* ds_3gpp_bearer_cntxt_get_prev_div_duplex */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_set_prev_div_duplex

DESCRIPTION
  This function returns the previous lte div duplex reported by CM.

PARAMETERS
  div_duplex - div_duplex value
  subs_id    - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  Last lte div duplex reported by CM.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_set_prev_div_duplex
(
  sys_div_duplex_e_type   div_duplex,
  sys_modem_as_id_e_type  subs_id
)
{
  DS_BEARER_CTXT_VF_CALL
  (
    set_div_duplex,
    subs_id,
    div_duplex,
    subs_id
  );
} /* ds_3gpp_bearer_cntxt_set_prev_div_duplex */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_comp_log_fetch_iface

DESCRIPTION

  This Functions returns the iface corresponding to link control id

PARAMETERS
  logging_info_ptr - the Log information 
  ip_version       - ip version v4 or v6

DEPENDENCIES
  None.

RETURN VALUE
  iface  

SIDE EFFECTS
  None.
  
===========================================================================*/
ps_iface_type *ds_3gpp_bearer_cntxt_comp_log_fetch_iface
( 
  ps_comp_logging_instance_config_s *logging_info_ptr,
  ip_version_enum_type               ip_version
)
{  
  ps_iface_type *iface = NULL;

  /* Temporary hack needed to obtain subs_id. Remove this hack once we
     support W/TD + W/TD DSDA configuration */
  sys_modem_as_id_e_type  subs_id = 
                            ds3gpp_get_subs_id_for_w_td_mode();

  DS_BEARER_CTXT_VFR_CALL
  (
    iface,
    complogfetchiface,
    subs_id,
    logging_info_ptr, 
    ip_version
  );

  return iface;
} /* ds_3gpp_bearer_cntxt_comp_log_fetch_iface */

/*===========================================================================
FUNCTION ds_bearer_cntxt_RAB_RE_ESTAB_IND

DESCRIPTION
  This function is to perform any mode specific operations on Rab re-estab
  indication.
  
PARAMETERS  
  mode_info_ptr     - mode specific info from CM
  bearer_context_p  - bearer context obj ptr
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  DS3G_SUCCESS if success, DS3G_FAILURE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean 
ds_bearer_cntxt_rab_re_estab_ind
(
  ds_bearer_context_s* bearer_context_p,
  cm_call_mode_info_s_type  *mode_info_ptr
)
{
  boolean                   rval = DS3G_FAILURE;
  sys_modem_as_id_e_type    subs_id = SYS_MODEM_AS_ID_NONE;
  
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return rval;
  }

  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_GSM)
  {
    DS_3GPP_MSG1_HIGH_EX(subs_id, "bearer_mode is GSM sys_rat is %d. Changing bearer_mode",
                      (mode_info_ptr->info.gw_ps_call.sys_rat));

    switch(mode_info_ptr->info.gw_ps_call.sys_rat)
    {
#ifdef FEATURE_WCDMA
      case SYS_RAT_UMTS_RADIO_ACCESS:
        bearer_context_p->ds_bearer_context_dyn_p->call_mode = SYS_SYS_MODE_WCDMA;
        break;
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_TDSCDMA
      case SYS_RAT_TDS_RADIO_ACCESS:
        bearer_context_p->ds_bearer_context_dyn_p->call_mode = SYS_SYS_MODE_TDS;
        break;
#endif /* FEATURE_TDSCDMA */
      default:
        DS_3GPP_ERROR_LOG_1_EX(subs_id, "Invalid sys_rat: d in mode_info_p",
                            mode_info_ptr->info.gw_ps_call.sys_rat);
        return rval;
    }
  }

  DS_BEARER_CTXT_VFR_CALL_PER_BEARER
  (
    rval, 
    rab_re_estab_ind, 
    bearer_context_p,
    mode_info_ptr    
  );

  return rval;
}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_MODE_PREF_CHANGE

DESCRIPTION
  This Handler is triggered while mode_pref  changes. This is required when 
  IRAT happens due to mode change.

PARAMETERS
  subs_id             -- Subscription ID where mode_pref_change occurred.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_mode_pref_change
(
  sys_modem_as_id_e_type subs_id
)
{
  uint32              bearer_index = 0;
  ds_bearer_context_s *bearer_cntxt_p = NULL;
  /*-----------------------------------------------------------------------*/


  for (bearer_index = 0; bearer_index < DS_3GPP_MAX_BEARER_CONTEXT; bearer_index++)
  {
    bearer_cntxt_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if (ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_cntxt_p,
                                                              subs_id))
    {
      switch (bearer_cntxt_p->ds_bearer_context_dyn_p->state)
      {
        case DS_BEARER_CONTEXT_STATE_UP:
        case DS_BEARER_CONTEXT_STATE_UP_DORMANT:
        case DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB:
          DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
          (
            mode_pref_change, 
            bearer_cntxt_p   
          );
          break;

        default:
          DS_3GPP_ERROR_LOG_3_EX(subs_id, "Bearer mode_pref change for bearer_idx: d in state: d",
                              bearer_index, 
                              bearer_cntxt_p->ds_bearer_context_dyn_p->state, 0);
          break;
      }
    }
  }
} /* ds_bearer_cntxt_mode_pref_change */

/*===========================================================================
FUNCTION  DS3GPP_GET_V4_IFACE_PTR_FROM_NSAPI

DESCRIPTION
  This function returns the v4 PS IFace ptr corresponding to the NSAPI.
  This is used for the test framework.

PARAMETERS
  nsapi
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
ps_iface_type *ds_3gpp_bearer_cntxt_get_v4_iface_ptr_from_nsapi
(
  uint8                  nsapi,
  sys_modem_as_id_e_type subs_id
)
{
  ds_3gpp_iface_s       *ds_iface_p=NULL;
  int i = 0;
  ds_pdn_context_s *pdn_cntx_p = NULL;
  ps_iface_type *iface_ptr = NULL;
  ds_bearer_context_s   *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(i=0; i< DS_3GPP_MAX_BEARER_CONTEXT; i++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(i);

    if((ds_bearer_cntxt_validate_bearer_subscription_internal
          (bearer_context_p, subs_id)) &&
       (bearer_context_p->ds_bearer_context_dyn_p->nsapi == nsapi))
    {
      pdn_cntx_p = (ds_pdn_context_s *)
                     bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
    }
  }

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    ds_iface_p = pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
                   [DS_3GPP_PDN_CONTEXT_IPV4_IFACE];
    if(ds_iface_p != NULL)
  {
      iface_ptr = &(ds_iface_p->ps_iface);
    }
  }

  return iface_ptr;
} /*ds_3gpp_bearer_cntxt_get_v4_iface_ptr_from_nsapi */

/*===========================================================================
FUNCTION  DS3GPP_GET_V6_IFACE_PTR_FROM_NSAPI

DESCRIPTION
  This function returns the v6 PS IFace ptr corresponding to the NSAPI.
  This is used for the test framework.

PARAMETERS
  nsapi
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
ps_iface_type *ds_3gpp_bearer_cntxt_get_v6_iface_ptr_from_nsapi
(
  uint8                  nsapi,
  sys_modem_as_id_e_type subs_id
)
{
  ds_3gpp_iface_s      *ds_iface_p=NULL;
  int i = 0;
  ds_pdn_context_s *pdn_cntx_p = NULL;
  ps_iface_type *iface_ptr = NULL;
  ds_bearer_context_s  *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(i=0; i< DS_3GPP_MAX_BEARER_CONTEXT; i++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(i);

    if((ds_bearer_cntxt_validate_bearer_subscription_internal 
          (bearer_context_p, subs_id)) &&
       (bearer_context_p->ds_bearer_context_dyn_p->nsapi == nsapi))
    {
      pdn_cntx_p = (ds_pdn_context_s *)
                     bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
    }
  }

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    ds_iface_p = pdn_cntx_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
                   [DS_3GPP_PDN_CONTEXT_IPV6_IFACE];

    if(ds_iface_p != NULL)
  {
      iface_ptr = &(ds_iface_p->ps_iface);
    }
  }

  return iface_ptr;
} /*ds_3gpp_bearer_cntxt_get_v6_iface_ptr_from_nsapi */

/*===========================================================================
FUNCTION ds_bearer_cntxt_suspend_ind

DESCRIPTION
  This function is to perform any mode specific operations on data suspend
  indication.
  
PARAMETERS  
  send_ps_data_avail - we are not allowed to send data_available_ind
  if we are OOS or in limited service
  subs_id            - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_suspend_ind
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s      *bearer_context_p= NULL;
  uint32                    bearer_cntx_inst= 0;  
  dsm_watermark_type       *tx_wm_ptr=NULL; /* Watermark pointer      */
  boolean                   send_ps_data_avail= FALSE;
  ds_pdn_context_s*         pdn_cntx_ptr = NULL;
  /*-----------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------
    Call the mode specific suspend handler for each bearer.
  --------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if (!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                               subs_id))
    {
      continue;
    }
    pdn_cntx_ptr = (ds_pdn_context_s*)
                 bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;

    if((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP))
    {
      send_ps_data_avail = ds3gpp_is_ps_data_avail_ind_allowed
                           (
                             subs_id,
                             ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_ptr)
                           );

      /*---------------------------------------------------------------------
        If there is data in the tx watermark, send the data available
        indication to CM right away.
      ---------------------------------------------------------------------*/
      tx_wm_ptr = &(bearer_context_p->tx_wm.wm_item);
      if ((!ds3gpp_get_ps_data_available_flag(subs_id))   &&
          (tx_wm_ptr->current_cnt > 0) &&
          (send_ps_data_avail)
         )
      {
        DS_3GPP_MSG1_HIGH_EX(subs_id, "suspend_ind:%d sending data_available",\
                          (uint32) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
        ds3g_msh_ps_data_available_ind(subs_id);
        ds3gpp_set_ps_data_available_flag(TRUE, subs_id);
      }

      DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer_inst:%d UM watermark cnt:%d",
         (uint32)bearer_cntx_inst,bearer_context_p->
                        tx_wm.wm_item.current_cnt);

    }// if bearer is UP/UP_DORMANT/UP_DORMANT_REESTAB
  }//end for each bearer  

  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if (!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                               subs_id))
    {
      continue;
    }

    if((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
       (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP))
    {
      DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
      (    
        suspend_ind,   
        bearer_context_p        
      );

      /*---------------------------------------------------------------
        If ds_3gpp_data_available is set to TRUE, disable flow on the
        interface to stop too much data from accumulating in the
        watermark.
      ---------------------------------------------------------------*/
      if (!send_ps_data_avail || 
          (ds3gpp_get_ps_data_available_flag(subs_id) == TRUE))
      {
        /*-------------------------------------------------------------
          Disable flow with UMTS mask.
        -------------------------------------------------------------*/
        ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p,
                                      DS_FLOW_UMTS_MASK, FALSE);
      }
      else
      {
        /*-------------------------------------------------------------
         We are allowed to send data_available_ind and there is no 
         data in watermarks.     
        -------------------------------------------------------------*/

        ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                    DS_FLOW_UMTS_MASK, FALSE);
      }
      DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer_inst:%d UM watermark cnt:%d",
         (uint32)bearer_cntx_inst,bearer_context_p->tx_wm.wm_item.current_cnt);

    }// if bearer is UP/UP_DORMANT/UP_DORMANT_REESTAB
  }//end for each bearer  
} /* ds_bearer_cntxt_suspend_ind */

/*===========================================================================
FUNCTION ds_bearer_cntxt_RESUME_IND

DESCRIPTION
  This function is to perform any mode specific operations on data resume
  indication.
  
PARAMETERS  
  subs_id - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_resume_ind
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s      *bearer_context_p= NULL;
  uint32                    bearer_cntx_inst= 0;
  /*--------------------------------------------------------------------------
    Call the mode specific resume handler for each bearer.
  --------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
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
      DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
      (   
        resume_ind,
        bearer_context_p    
      );
    }// if bearer is UP/UP_DORMANT/UP_DORMANT_REESTAB
  }//end for each bearer

}/* ds_bearer_cntxt_resume_ind */

/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CNTXT_GET_PHYS_LINK_PTR_FROM_NSAPI

DESCRIPTION
  This function returns the UMTS PHYS Link ptr corresponding to the NSAPI.
  This is used for the test framework. 
  
PARAMETERS
  nsapi
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  Phys Link Ptr

SIDE EFFECTS
  None.

===========================================================================*/
ps_phys_link_type *ds_3gpp_bearer_cntxt_get_phys_link_ptr_from_nsapi
(
  uint8                  nsapi,
  sys_modem_as_id_e_type subs_id
)
{ 
  int index =0;
  ds_bearer_context_s     *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(index=0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                             subs_id))
    {
      if(bearer_context_p->ds_bearer_context_dyn_p->nsapi == nsapi)
      {
        return(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
      } /* bearer_id matches*/
    }  /* if valid index */
  } /* for index */
  return(NULL);
} /* ds_3gpp_bearer_cntxt_get_phys_link_ptr_from_nsapi */

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_FLUSH_UPLINK_WATERMARKS

DESCRIPTION
  This function flushes UL watermarks for a particular iface
 
PARAMETERS : pointer to iface

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
void ds_bearer_cntxt_flush_uplink_watermarks
(
  ps_iface_type             * iface_ptr
)
{
  uint8                    bearer_index;
  ds_pdn_context_s        *pdn_context_ptr = NULL;
  ds_bearer_context_s     *bearer_context_p = NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*------------------------------------------------------------------- 
    Get pdn context pointer from the iface ptr passed in.
    ------------------------------------------------------------------*/
  pdn_context_ptr = DSPDNCNTX_GET_PDN_FROM_IFACE(iface_ptr);

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == TRUE)
  {
    /*-----------------------------------------------------------------------
      Loop over all MAX applicable bearers for this PDN cntxt, find the valid
      ones and flush em.
      -----------------------------------------------------------------------*/
    for(bearer_index = 0;
        bearer_index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;
        bearer_index++)
    {
      /*------------------------------------------------------------------------ 
        fetch the bearer cntxt pointer for a particular bearer from the pdn
        context pointer
        ------------------------------------------------------------------------*/
      bearer_context_p =\
       (ds_bearer_context_s*)pdn_context_ptr->ds_pdn_context_dyn_p->bearer_context_ptr_tbl[bearer_index];
      
      if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
      {
       /*----------------------------------------------------------------------- 
        This entry in the bearer context table is NULL, meaning there is no bearer
        corresponding to the bearer index for this pdn context. Other entries could
        have non-NULL values.
        -----------------------------------------------------------------------*/
        continue;
      }

      /*------------------------------------------------------------------------- 
        Valid bearer cntxt found, flush UL WMs for it.
        1. If call mode is WCDMA , use below function to empty WMs
        2. For all other callmodes, use dsm_empty_queue to flush the UL WMs.
        ------------------------------------------------------------------------*/
      if (bearer_context_p->ds_bearer_context_dyn_p->call_mode ==
                                                            SYS_SYS_MODE_WCDMA)
      {
        ds_tds_w_cdma_bearer_cntxt_empty_watermark(bearer_context_p);
      }
      else
      { 
        dsm_empty_queue ( &(bearer_context_p->tx_wm.wm_item) );
      }
    }/*for loop end*/
  }/*if end*/
}/*ds_bearer_cntxt_flush_all_watermarks*/

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_FLUSH_PS_L2_WATERMARKS

DESCRIPTION
  This function flushes PS-L2 watermarks

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntxt_flush_ps_l2_watermarks
(
  void
)
{
  int index =0;
  ds_bearer_context_s   *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(index=0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    /* flush the watermark for this bearer */
    if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      continue;
    }

#if defined(FEATURE_TDSCDMA) || defined(FEATURE_DATA_WCDMA_PS)
    if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == 
                                                            SYS_SYS_MODE_WCDMA) 
    {
      ds_tds_w_cdma_bearer_cntxt_empty_watermark(bearer_context_p);
    }
    else
#endif /* FEATURE_TDSCDMA || FEATURE_DATA_WCDMA_PS */
    {
      dsm_empty_queue ( &(bearer_context_p->tx_wm.wm_item) );
    }
    dsm_empty_queue ( &(bearer_context_p->rx_wm.wm_item) );
  }
} /*ds_bearer_cntxt_flush_ps_l2_watermarks*/

/*===========================================================================

FUNCTION DS_3GPP_BEARER_CNTXT_MEM_CTRL_EV_CB

DESCRIPTION
  This function posts cmd to DS task to notify DSM Memory control event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_bearer_cntxt_mem_ctrl_ev_cb
(
  dsm_mempool_id_enum_type pool_id,            /* memory pool ID */
  dsm_mem_level_enum_type  event,              /* type of memory event */
  dsm_mem_op_enum_type     op
)
{
  ds_cmd_type *cmd_ptr = NULL;
  ds_bearer_context_event_type  *ds_bearer_context_event_ptr = NULL;
  boolean     post_cmd = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds3gpp_enter_global_crit_section();
  if (ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag() == FALSE)
  {
    post_cmd = TRUE;
  }
  ds3gpp_leave_global_crit_section();

  if (post_cmd == TRUE && 
      op == DSM_MEM_OP_NEW &&
      ((pool_id == DSM_DS_LARGE_ITEM_POOL) ||
       (pool_id == DSM_DS_SMALL_ITEM_POOL) || 
       (pool_id == DSM_DS_SL_ITEM_POOL)
       ))
  {

    if (pool_id == DSM_DS_SMALL_ITEM_POOL) 
    {
       /*if the pool id is small item pool, need to 
        only handle event SIO Few Event*/
       if (event != DSM_MEM_LEVEL_SIO_FEW ) 
       {
          return;
       }  
    }


    cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_bearer_context_event_type));

    if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
    {
      DS3GPP_ASSERT(0);
      return;
    }

    DS_3GPP_MSG3_HIGH("dsm mem ev cb: free [%d], event [%d], pool_id [%d]", 
                      DSM_POOL_FREE_CNT(pool_id), event, pool_id );

    cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DSM_MEM_CTRL_EV;
    ds_bearer_context_event_ptr = 
      (ds_bearer_context_event_type*)cmd_ptr->cmd_payload_ptr;

    ds_bearer_context_event_ptr->event_type = DS_BEARER_CONTEXT_EV_DSM_MEM_CTRL;
    ds_bearer_context_event_ptr->cmd_info.dsm_info.pool_id = pool_id;
    ds_bearer_context_event_ptr->cmd_info.dsm_info.event = event;
    ds_bearer_context_event_ptr->cmd_info.dsm_info.op = op;

    ds_put_cmd_ext( cmd_ptr );
  
    ds3gpp_enter_global_crit_section();
    ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag(TRUE);
    ds3gpp_leave_global_crit_section();
  }
  else
  {
    DS_3GPP_MSG3_HIGH("dsm mem ev cb: post cmd: %d, op: %d, pool_id: %d",
                       post_cmd,op,pool_id);
  }

  return;

} /*ds_3gpp_bearer_cntxt_mem_ctrl_ev_cb*/

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_LOCAL_CLEANUP

DESCRIPTION
  This function peforms local call cleanup

PARAMETERS  
  Bearer Context ptr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void ds_bearer_cntx_local_cleanup 
(
  ds_bearer_context_s *bearer_context_p
)
{
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;    
  /*-----------------------------------------------------------------------*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }
  
  DS_3GPP_MSG0_HIGH_EX(subs_id, "Performing local call cleanup ");

  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                              DS_FLOW_UMTS_MASK, FALSE);

  if (bearer_context_p->ds_bearer_context_dyn_p->is_default)
  {
    /*-------------------------------------------------------------------
      Error in setting up default bearer -> Make sure that we call flow 
      go null ind on default flows of all ifaces associated with the PDN 
      context          
    --------------------------------------------------------------------*/
    ds_pdn_cntxt_unbind_default_flows(bearer_context_p->
                                      ds_bearer_context_dyn_p->pdn_context_ptr);
  }

  /*-------------------------------------------------------------------
    Set the Bearer context to be in down state and call the bearer down
    call back to the PDN context.
  --------------------------------------------------------------------*/

  ps_phys_link_gone_ind(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link));
  /* Is it safe to free the bearer inst ?*/
  ds_bearer_cntxt_free_bearer_inst(bearer_context_p);

  /*-----------------------------------------------------------------------
    We have already cleaned up at the bearer level. We should send
    bearer down indication to PDN context.
    This way the PDN context will be freed up as well.
  -----------------------------------------------------------------------*/
  if (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_cb_tbl_ptr-> // clang_sa_ignore [core.NullDereference]
      bearer_down_ind_cb != NULL)
  {
    ds_3gpp_down_reason_t  net_down_reason;
    net_down_reason.reason.ps_iface_down_reason = 
                                PS_NET_DOWN_REASON_PENDING_REDIAL_CALL_CLEANUP;
    net_down_reason.reason_T = DS_3GPP_PS_IFACE_NET_DOWN_REASON_T;
    bearer_context_p->ds_bearer_context_dyn_p->
      pdn_context_cb_tbl_ptr->bearer_down_ind_cb(
                  bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr,
                  bearer_context_p,
                  net_down_reason,
                  FALSE);
  }
}

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_IS_ANY_OTHER_BEARER_UP

DESCRIPTION
  This function checks if any other bearer is in UP/UP_DORMANT state

PARAMETERS  
  Bearer Context ptr

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_is_any_other_bearer_up
(
  ds_bearer_context_s *bearer_context_p
)
{
  boolean ret_val = FALSE;
  ds_bearer_context_s *temp_bearer_context_p = NULL;
  uint8 bearer_cntx_index = 0;
  sys_modem_as_id_e_type    subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_context_state_e state = DS_BEARER_CONTEXT_STATE_DOWN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( !ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id) )
  {
    return ret_val;
  }

  for(bearer_cntx_index = 0; bearer_cntx_index < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_index++)
  {
    temp_bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_index);

    if(temp_bearer_context_p == bearer_context_p)
    {
      DS_3GPP_MSG2_LOW("temp_bearer_cntxt:0x%x, bearer_context_p:0x%x",
                       temp_bearer_context_p,bearer_context_p);
      continue;
    }
    else if (!ds_bearer_cntxt_validate_bearer_subscription_internal
               (temp_bearer_context_p, subs_id))
    {
      continue;
    }

    state = temp_bearer_context_p->ds_bearer_context_dyn_p->state;

    if((state == DS_BEARER_CONTEXT_STATE_UP ) ||
       (state == DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
       (state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB))
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Bearer index:%d is UP, found OTHER",bearer_cntx_index);
      ret_val = TRUE;
      break;
    }
    else
    {
      DS_3GPP_MSG1_MED_EX(subs_id, "Bearer context state is %d",
                        temp_bearer_context_p->ds_bearer_context_dyn_p->state);
    }
  }

  DS_3GPP_MSG1_LOW("ds_bearer_cntx_is_any_other_bearer_up returns %d",ret_val);
  return ret_val;
} /* ds_bearer_cntx_is_any_other_bearer_up */

/*===========================================================================

FUNCTION DS_BEARER_CNTXT_IS_ANY_OTHER_BEARER_PER_SUB_ACTIVE

DESCRIPTION
  This function checks if any other bearer is in UP state only for the SUBS
  of the bearer context

PARAMETERS  
  Bearer Context ptr

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_is_any_other_bearer_per_sub_active
(
  ds_bearer_context_s *bearer_context_p
)
{
  boolean                 ret_val = FALSE;
  ds_bearer_context_s    *temp_bearer_context_p = NULL;
  uint8                   bearer_cntx_index = 0;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;
  sys_modem_as_id_e_type  temp_subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( !ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id) )
  {
    return ret_val;
  }

  for(bearer_cntx_index = 0; bearer_cntx_index < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_index++)
  {
    temp_bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_index);

    if((temp_bearer_context_p == bearer_context_p) ||
       (!ds_bearer_cntx_validate_bearer_context_internal(temp_bearer_context_p)))
    {
      DS_3GPP_MSG2_LOW("temp_bearer_cntxt:0x%x, bearer_context_p:0x%x",
                       temp_bearer_context_p, bearer_context_p);
      continue;
    }

    /*-----------------------------------------------------------------------
      Check the temp subs_id, Perform this function check only for bearers
      of the same subscription.
    -----------------------------------------------------------------------*/
    if( !ds_bearer_cntxt_get_subs_id_from_bearer_internal(temp_bearer_context_p,
                                                          &temp_subs_id))
    {
      continue;
    }

    if (subs_id != temp_subs_id)
    {
      DS_3GPP_MSG2_MED_EX(subs_id, "Subs ID mismatch, continue "
                       "Temp Subs Id(CM):%d Subs Id(CM):%d",
                       temp_subs_id,subs_id);
      continue;
    }

    if(temp_bearer_context_p->ds_bearer_context_dyn_p->state ==
       DS_BEARER_CONTEXT_STATE_UP)
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Bearer index:%d is UP, found another active bearer",
                        bearer_cntx_index);
      ret_val = TRUE;
      break;
    }
    else
    {
      DS_3GPP_MSG1_MED_EX(subs_id, "Bearer context state is %d",
                       temp_bearer_context_p->ds_bearer_context_dyn_p->state);
    }
  }

  DS_3GPP_MSG1_LOW("ds_bearer_cntx_is_any_other_bearer_up returns %d",ret_val);
  return ret_val;
} /* ds_bearer_cntx_is_any_other_bearer_per_sub_active */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CONTEXT_INIT_BEARER_THROUGHPUT_STATS

DESCRIPTION
  This function initializes the bearer throughput statistics parameters
  for all active bearer contexts. This will be required to get a better
  estimate of the actual throughput rates on all active bearers
 
PARAMETERS  
  subs_id: Subscription Id
    
DEPENDENCIES 
  The bearer throughput stats are initialized before the timer that is
  used to refresh actual throughout timer starts.
  
RETURN VALUE 
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_bearer_context_init_bearer_throughput_stats
(
  sys_modem_as_id_e_type subs_id
)
{
  uint8                   index             = 0;
  ds_bearer_context_s    *bearer_context_p  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT;index++) 
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }
    /*----------------------------------------------------------------------- 
      Initialize Bearer throughput parameters for all active bearers based
      on their current bearer call mode
      -----------------------------------------------------------------------*/
    DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
    (
       init_bearer_throughput_stats,
       bearer_context_p
    );
  }

  return;
} /* ds_3gpp_bearer_context_init_bearer_throughput_stats */

#if defined(FEATURE_DATA_LTE) && (defined(FEATURE_GSM_GPRS) || defined(FEATURE_DATA_WCDMA_PS) || defined(FEATURE_TDSCDMA))

/*===========================================================================

FUNCTION ds_bearer_cntxt_transfer_ind

DESCRIPTION
  This function is to perform any mode specific operations on cntxt transfer
  indication.
  
PARAMETERS  
  None.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE,if atleast one Bearer was successfully transferred to target rat
  FALSE, otherwise 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean 
ds_bearer_cntxt_transfer_ind
(
  sys_sys_mode_e_type       target_rat,
  sys_modem_as_id_e_type    subs_id
)
{
  ds_bearer_context_s                *bearer_context_p= NULL;
  uint32                    bearer_cntx_inst= 0;
  boolean                   ret_val = FALSE;
  dsm_watermark_type                 *rx_wm_p= NULL;
  ds_bearer_cntxt_per_subs_info_type *per_subs_info_hndl = NULL;
  /*------------------------------------------------------------------------*/

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for "
                       "Subs Id(CM)",subs_id);
    return FALSE;
  }
  /*--------------------------------------------------------------------------
     Call the mode specific release and for each bearer for context
     transfer across LTE<->GW. Release hdlr will be called in source RAT,
     Setup handler will be called in Target RAT.
  --------------------------------------------------------------------------*/

  DS_3GPP_MSG1_HIGH_EX(subs_id, "cntxt_transfer_ind received for mode:%d",target_rat);
  
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }

    /*---------------------------------------------------------------------
      If moving from L to G/W and the bearer is not transferable to GW
    ---------------------------------------------------------------------*/
    if((bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE) &&
       (bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW == FALSE) &&
       ((target_rat == SYS_SYS_MODE_WCDMA)||(target_rat == SYS_SYS_MODE_GSM)||
        (target_rat == SYS_SYS_MODE_TDS)))
    {
      DS_3GPP_MSG2_HIGH_EX(subs_id, "call id:%d not transferable to GWT, maintaining mode:%d",
                        bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                        bearer_context_p->ds_bearer_context_dyn_p->call_mode);
      continue;
    }

    switch (bearer_context_p->ds_bearer_context_dyn_p->state)
    {
      case DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB:
      case DS_BEARER_CONTEXT_STATE_UP_DORMANT:
      case DS_BEARER_CONTEXT_STATE_UP:
      case DS_BEARER_CONTEXT_STATE_GOING_DOWN:
        
      {      
      
        /*---------------------------------------------------------------------
        Context transfer cannot be made if 
        current call mode is LTE and the received target rat is also LTE 
          current call mode is GWT and the received target rat is also GWT.

          The following scenario needs to be considered:
          1. A non-transferable bearer is established on LTE.
          2. Due to CSFB call, UE moves to GSM/WCDMA/TDSCDMA, 
             bearer_mode didn't change.
          3. CM SS event reports Full Srv on GSM, attach SM reset to NULL state.
             DSD notified.
          4. UE comes back to LTE, attach SM remains in NULL state and preferred system 
             doesn't change.

          To handle this scenario, the second context_transfer_ind is not
          ignored if bearer is not-transferable.
        ---------------------------------------------------------------------*/
        if ((bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE && 
             target_rat == SYS_SYS_MODE_LTE && 
             bearer_context_p->ds_bearer_context_dyn_p->is_transferable_to_GW == TRUE)||
            (bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE
             && target_rat != SYS_SYS_MODE_LTE))
        {      
           DS_3GPP_MSG3_HIGH_EX(subs_id, "call mode:%d not transferable to %d, maintaining mode:%d",
                        bearer_context_p->ds_bearer_context_dyn_p->call_mode,
                        target_rat,bearer_context_p->ds_bearer_context_dyn_p->call_mode);
           continue;
        }

        if (bearer_context_p->ds_bearer_context_dyn_p->call_mode != target_rat)
        {
          DS_3GPP_MSG2_HIGH_EX(subs_id, "cntxt_transfer_ind_release for Bearer_inst:%d, call_mode:%d",
           (uint32)bearer_cntx_inst,bearer_context_p->ds_bearer_context_dyn_p->call_mode);
          DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
          (   
            cntxt_transfer_ind_release,
            bearer_context_p
          );
        }
    
        bearer_context_p->ds_bearer_context_dyn_p->call_mode = target_rat;
    
        if(bearer_context_p->ds_bearer_context_dyn_p->cm_call_id != CM_CALL_ID_INVALID)
        {   
            
          DS_3GPP_MSG2_HIGH_EX(subs_id, "Changing mode for call id:%d :mode:%d",
                            bearer_context_p->ds_bearer_context_dyn_p->cm_call_id,
                            target_rat);
    
          if( !ds3g_msh_set_new_call_mode(bearer_context_p->ds_bearer_context_dyn_p->
                                          cm_call_id,
                    target_rat ) )
          {
            DS_3GPP_ERROR_LOG_1_EX(subs_id, "Mode set failed for call id :d", 
                      bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);
          } 
         
        } /* !CM_CALL_ID_INVALID */


        /*In case of Lte; we will wait for Wcdma/Gsm Watermark to go empty; 
        in case of wcdma/gsm/tds we will wait for Lte watermark to go empty*/
        if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE)
        {
          rx_wm_p = &(bearer_context_p->rx_wm.wm_item);
        }
        else
        {
          rx_wm_p = &(bearer_context_p->lte_rx_wm.wm_item);
        }

     
        if(bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
        {
          if(dsm_is_wm_empty(rx_wm_p))
          {     
            DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
            (   
              cntxt_transfer_ind_setup,
                bearer_context_p
            );

            DS_3GPP_MSG3_HIGH_EX(subs_id, "cntxt_transfer_ind_setup for"
                              "Bearer_inst:%d, call_mode:%d,"
                              "phys_link.bearer_id: %d",\
                              (uint32)bearer_cntx_inst,
                              bearer_context_p->ds_bearer_context_dyn_p->call_mode,
                              bearer_context_p->ds_bearer_context_dyn_p->phys_link.bearer_id);
            ds_bearer_cntx_reregister_dpm(bearer_context_p);
          }
          else
          {
            DS_3GPP_MSG1_HIGH_EX(subs_id, "Context Transfer Registering for Callback: %d",\
	                  bearer_context_p->ds_bearer_context_dyn_p->phys_link.bearer_id);   
            rx_wm_p->gone_empty_func_ptr = ds_bearer_cntxt_dpm_dereg_cb;
            rx_wm_p->gone_empty_func_data = (void*) bearer_context_p; 
            bearer_context_p->ds_bearer_context_dyn_p->waiting_for_gone_empty = TRUE;
          }
        }

        ret_val = TRUE;
        break;

      }// if bearer is UP/UP_DORMANT/UP_DORMANT_REESTAB

      case DS_BEARER_CONTEXT_STATE_PEND_REDIAL:
      {
         DS_3GPP_MSG3_HIGH_EX(subs_id, "Context Transfer Indication received in Pending Redial state, "
                           "bearer instance: %d,new_mode: %d, call id: %d", 
                            bearer_cntx_inst,
                            target_rat, 
                            bearer_context_p->ds_bearer_context_dyn_p->cm_call_id);
                                            
         /*----------------------------------------------------------------------
            Here we check whether the system moved from G/W to L and back to G/W. 
            We also check whether the call id is INVALID.
            If both conditions are true then we need to cleanup the call.  
            Otherwise the call will be stuck in Pending redial state.
      
            We need to handle the case of L -> G/W -> L once we support pending redial
            calls while moving from L -> G/W.
         -----------------------------------------------------------------------------*/ 
      
         /*------------------------------------------------------------------------------- 
           Also tear down a call in PENDING REDIAL STATE if another bearer is in UP state
           Ex of such a case
           Call 1 brought up on G/W/T on the non attach profile  --> Bearer is UP
           Call 2 in coming up state on G/W/T but on the attach profile, Now the call 2
           is marked as PENDING REDIAL but no redial ever happens on moving to LTE because
           call 1's bearer was UP, If not for this check this bearer will be left hanging
           in pending redial state on LTE.
           -------------------------------------------------------------------------------*/
        
         if (bearer_context_p->ds_bearer_context_dyn_p->cm_call_id == CM_CALL_ID_INVALID)
         {
           DS_3GPP_MSG0_LOW("Call id is invalid");
      
           if ((bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE &&
               (target_rat != SYS_SYS_MODE_LTE)) ||
               (ds_bearer_cntx_is_any_other_bearer_up(bearer_context_p) && 
                target_rat == SYS_SYS_MODE_LTE))
           {
             DS_3GPP_MSG1_HIGH_EX(subs_id, "Cleaning up bearer context:0x%x in PENDIGN REDIAL state",
                                bearer_context_p);
             ds_bearer_cntx_local_cleanup(bearer_context_p);
           }
         }
                                            
         break;
       } // if bearer is PEND_REDIAL
  
       default:
       {
         DS_3GPP_MSG1_HIGH_EX(subs_id, "Bearer transfer indication received in %d state, ignoring",
                            bearer_context_p->ds_bearer_context_dyn_p->state);
         break;
       }      
    } 
  }//end for each bearer

  return ret_val;
}/* ds_bearer_cntxt_transfer_ind */

/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CNTXT_RESET_SRV_REQ_THROT

DESCRIPTION
  This function resets service request throttling SM. This is called during
  RAT change. 

PARAMETERS
  sys_modem_as_id_e_type  - subscription ID. 
  sys_sys_mode_e_type     - new mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_reset_srv_req_throt
(
  sys_modem_as_id_e_type subs_id,
  sys_sys_mode_e_type    target_rat
)
{
  ds_cmd_type              *cmd_ptr = NULL;
  uint32                    timerval = 0;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
    sys_modem_as_id_e_type            *subs_id_p = NULL;
  
  /*---------------------------------------------------------------------
    Clear the service request throttling on context transfer from L to 
    W/TD/G(L -> W/TD/G IRAT) or W/TD -> L
  ---------------------------------------------------------------------*/
  ds_3gpp_srv_req_throt_go_null(subs_id, FALSE);

  timerval = ds_3gpp_bearer_cntxt_get_retry_timer_val(TRUE, subs_id);
  DS_3GPP_MSG3_HIGH_EX(subs_id, "Reset Srv Req throt: throttle timer is running "
                    "throttle timer value: %d", timerval, 0, 0);

  per_subs_info_hndl = ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
      (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "subs id",subs_id);
    return;
  } 

  if( (per_subs_info_hndl->timer_running == TRUE) && (timerval > 0) )
  {       
    /*---------------------------------------------------------------------
      Clear the RAB reestablish timer. This will be started again either in
      rab_re_est_ind or in rab_re_est_rej handler or in the registered timer
      call back.
      post DS3GPP_REORIG_DORM_TMR_EXP_CMD command for requesting rab 
      re-establishment on conncted handover               
    -----------------------------------------------------------------------*/
      DS_3GPP_MSG0_LOW("ds_bearer_cntxt_transfer_ind: clearing back off timer"
                      " to allow srv req on context transfer");
      DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl); 
      ds_3gpp_rab_reestab_reset_state(subs_id);

      ds_bearer_cntxt_reset_access_barring_retry();
    if( target_rat == SYS_SYS_MODE_GSM )
    {  
      return;
    }

      if( (cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type))) == NULL ||
           cmd_ptr->cmd_payload_ptr == NULL )
    {
        DS3GPP_ASSERT(0);
        return;
    }

      cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DORM_REORIG_TMR_EXP;
      subs_id_p = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
      *subs_id_p = subs_id;

      DS_3GPP_MSG0_HIGH_EX(subs_id, "ds_bearer_cntxt_transfer_ind: posting DS3GPP_REORIG"
                        "_DORM_TMR_EXP_CMD to trigger srv req on connected HO");

      ds_put_cmd_ext(cmd_ptr);
    }
} /* ds_3gpp_bearer_cntxt_reset_srv_req_throt */

#endif /*defined FEATURE_LTE && defined(FEATURE_GSM_GPRS || FEATURE_DATA_WCDMA_PS || FEATURE_TDSCDMA)*/

/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CONTEXT_REFRESH_ACTUAL_THROUGHPUT

DESCRIPTION
  This utility function invokes the mode specific functions to refresh the
  current actual UL throughput, based on the current system mode
 

PARAMETERS
  sys_modem_as_id_e_type  - subscription ID 
  uint32                  - sampling interval to calculate the actual
                            throughput rate
DEPENDENCIES
  Assumes bearer is already allocated.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_bearer_context_refresh_actual_throughput
(
  sys_modem_as_id_e_type    cm_subs_id,
  uint32                    sampling_interval
)
{
  boolean                   ret_val           = FALSE;
  uint8                     index             = 0;
  ds_bearer_context_s      *bearer_context_p  = NULL;
  boolean                   refresh_status    = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------- 
    If the timer has just started, initialize the baerer throughput stats
    to calculate a better estimate of the actual throughput when
    the timer expires
    -------------------------------------------------------------------------*/
  if (sampling_interval == 0)
  {
    DS_3GPP_MSG0_HIGH_EX(cm_subs_id, 
                         "Sampling Interval is 0 msec. Skip refreshing actual"
                         " throughput");
    /*----------------------------------------------------------------------- 
      Update the current actual throughput count numbers so that we may be
      able to get a better estimate of the actual throughput for all
      active bearers
      -----------------------------------------------------------------------*/
    ds_3gpp_bearer_context_init_bearer_throughput_stats(cm_subs_id);
    return ret_val;
  }

  /*------------------------------------------------------------------------- 
    Validate the subscription ID
    -------------------------------------------------------------------------*/
  if (!ds3gsubsmgr_is_subs_id_valid(cm_subs_id))
  {
    return ret_val;
  }
  /*-------------------------------------------------------------------------- 
    Invoke the mode specific function to refresh actual throughput
    --------------------------------------------------------------------------*/
  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if (!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                               cm_subs_id))
    {
      continue;
    }
    /*----------------------------------------------------------------------- 
      Refresh the actual throughput values, based on the current bearer
      call mode
      -----------------------------------------------------------------------*/
    DS_BEARER_CTXT_VFR_CALL_PER_BEARER
    (
       refresh_status,
       refresh_actual_throughput,
       bearer_context_p,
       cm_subs_id,
       sampling_interval
    );
    /*----------------------------------------------------------------------- 
      Return Success if atleast one bearer actual throughput stats are
      refreshed successfully
      -----------------------------------------------------------------------*/
    ret_val |= refresh_status;
  }

  if (ret_val == FALSE)
  {
    DS_3GPP_MSG1_HIGH_EX(cm_subs_id, 
                         "No active bearers on Subs Id(CM) %d",cm_subs_id);
  }
  return ret_val;
} /* ds_3gpp_bearer_context_refresh_actual_throughput */
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_REPORT_ACTUAL_ULDL_THROUGHPUT

DESCRIPTION
  This function refreshes the actual uldl throughput numbers for the ue 
  per subscription for the sampling interval
  
PARAMETERS
   sys_modem_as_id_e_type      cm_subs_id, 
   uint32                      sampling_interval (msec)
DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_report_actual_uldl_throughput
(
  sys_modem_as_id_e_type      subs_id, 
  uint32                      sampling_interval,
  ds3gtputmgr_throughput_report_client_e_type  client
)
{
  
  uint64                      prev_tx_wm_cnt        = 0;
  uint64                      current_tx_wm_cnt     = 0;
  uint64                      prev_rx_wm_cnt        = 0;
  uint64                      current_rx_wm_cnt     = 0;  
  uint64                      prev_tx_running_avg   = 0;
  uint64                      prev_rx_running_avg   = 0;
  uint64                      moving_avg_alpha      = 0;
  ds_bearer_context_s         *bearer_context_p = NULL;
  uint64                      average_ul_data_rate  = 0;
  uint64                      average_dl_data_rate  = 0;
  int                         index = 0;
  uint32                      frequency = 0;
  boolean                     uplink_data_transmission= FALSE;
  boolean                     downlink_data_transmission = FALSE;
  uint64                      total_uplink_dequeued_cnt=0;
  uint64                      total_downlink_dequeued_cnt=0;
  ds_pdn_context_s            *pdn_context_p = NULL;
  ps_iface_type               *v4_ps_iface_p = NULL;
  ps_iface_type               *v6_ps_iface_p = NULL;
  uint64                       temp_v4rxbytes = 0;
  uint64                       temp_v6rxbytes = 0;
  
  /*------------------------------------------------------------------------- 
    Check if the sampling interval is valid to refresh uldl throughput
    -------------------------------------------------------------------------*/

  if(sampling_interval == 0)
  {
    DS_3GPP_MSG0_LOW("Not a valid sampling Interval to calculate actual uldl tput");
    return;
  }

  frequency = ds3gtputmgr_get_uldl_throughput_frequency(
                client,
                ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));

  if(frequency == 0)
  {
    DS_3GPP_MSG0_LOW("Not a valid Frequency to calculate actual uldl tput");
    return;
  }

  moving_avg_alpha = (MAX_UL_DL_FREQUENCY/frequency);

  /*case where frequency is higher than max frequency; set alpha to 1*/
  if (moving_avg_alpha == 0)
  {
     moving_avg_alpha = 1;
  }

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    total_uplink_dequeued_cnt = 0;

    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    if (!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                               subs_id))
    {
      continue;
    }

    total_uplink_dequeued_cnt = 
      bearer_context_p->tx_wm.wm_item.total_rcvd_cnt - 
        bearer_context_p->tx_wm.wm_item.current_cnt;

    if (total_uplink_dequeued_cnt !=
        bearer_context_p->ds_bearer_context_dyn_p->
         bearer_throughput_stats_uldl.total_prev_recd_tx_wm_cnt)
    {
       uplink_data_transmission = TRUE;
    } 

  }

  for(index = 0;index < DS_3GPP_MAX_PDN_CONTEXT;index++)
    {
      total_downlink_dequeued_cnt = 0;
	  
    pdn_context_p = &(ds_pdn_context_tbl[index]);
	if (!ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_context_p))
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Invalid PDN for at index : %d "
	  	                "for downilnk data",
	  	                index);
      continue;
    }
															   
    v4_ps_iface_p = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_context_p);
	v6_ps_iface_p = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_context_p);
	
	if(!ps_stat_get_iface(PS_STAT_IFACE_BYTES_RX,
										v4_ps_iface_p,
										(void *)&(temp_v4rxbytes),
										sizeof(temp_v4rxbytes)))
    {
	  DS_3GPP_ERROR_LOG_0("Unable to get rx bytes for V4 Iface");
	}
	if(!ps_stat_get_iface(PS_STAT_IFACE_BYTES_RX,
										v6_ps_iface_p,
										(void *)&(temp_v6rxbytes),
										sizeof(temp_v6rxbytes)))
	{
	  DS_3GPP_ERROR_LOG_0("Unable to get rx bytes for V6 Iface");
    } 
										
    total_downlink_dequeued_cnt = temp_v4rxbytes+temp_v6rxbytes;
    
    if (total_downlink_dequeued_cnt != pdn_context_p->
	  	ds_pdn_context_dyn_p->throughput_stats.prev_rxed_data_pkt_cnt)
    {
       downlink_data_transmission = TRUE;
    } 
		
  }

  if ((uplink_data_transmission == FALSE) &&
      (downlink_data_transmission == FALSE))
  {
    /* Do not recalculate; use cached ul data rate
       dl data rate*/
    return;
  }

  bearer_context_p = NULL;


  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = NULL;
    prev_tx_wm_cnt =0;
    current_tx_wm_cnt =0;
    prev_tx_running_avg =0;

    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }

    /*if data is there in uplink direction in atleast one bearer; 
      else skip the tmeas interval and use the prev value*/
    if (uplink_data_transmission)
    {

      current_tx_wm_cnt = bearer_context_p->tx_wm.wm_item.total_rcvd_cnt - 
        bearer_context_p->tx_wm.wm_item.current_cnt ;

      prev_tx_wm_cnt = bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.total_prev_recd_tx_wm_cnt;  

      prev_tx_running_avg   = bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.actual_ul_rate;

      /*------------------------------------------------------------------------- 
        Calculate new UL actual throughput rate; based on previous average
        current wm count;prev recieved count and curren wm count
      -------------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.actual_ul_rate = 
          ds_util_calculate_throughput_moving_avg_uldl
          (
            prev_tx_running_avg,
            prev_tx_wm_cnt,
            current_tx_wm_cnt,
            sampling_interval,
            moving_avg_alpha
         );

      DS_3GPP_MSG6_HIGH_EX(subs_id, "Prev UL Bytes Cnt transf 0x%lx%lx Current"
                           " byte cnt 0x%lx%lx  Calculated Avg UL actual t'put 0x%lx%lx kbps",
                           QWORD_HIGH(prev_tx_wm_cnt),
                           QWORD_LOW(prev_tx_wm_cnt),
                           QWORD_HIGH(current_tx_wm_cnt),
                           QWORD_LOW(current_tx_wm_cnt),
                           QWORD_HIGH(bearer_context_p->ds_bearer_context_dyn_p->
                                         bearer_throughput_stats_uldl.actual_ul_rate),
                           QWORD_LOW(bearer_context_p->ds_bearer_context_dyn_p->
                                         bearer_throughput_stats_uldl.actual_ul_rate));
    

      bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.total_prev_recd_tx_wm_cnt = current_tx_wm_cnt;
      /*store current wm count in total_prev_recd_tx_wm_cnt for next calculations */

    }

    average_ul_data_rate = average_ul_data_rate + 
      bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.actual_ul_rate;

  }

  for(index = 0;index < DS_3GPP_MAX_PDN_CONTEXT;index++)
    {
    prev_rx_wm_cnt = 0;
    current_rx_wm_cnt =0;
	prev_rx_running_avg =0;
	pdn_context_p = &(ds_pdn_context_tbl[index]);

	if (!ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_context_p))
      {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Invalid PDN for at index : %d "
	  	                "for throughput calculation",
	  	                index);
      continue;
      }

    if (downlink_data_transmission)
      {
	
	  v4_ps_iface_p = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_context_p);
	  v6_ps_iface_p = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_context_p);
	
	  if(!ps_stat_get_iface(PS_STAT_IFACE_BYTES_RX,
										  v4_ps_iface_p,
										  (void *)&(temp_v4rxbytes),
										  sizeof(temp_v4rxbytes)))
	  {
	    DS_3GPP_ERROR_LOG_0("Unable to get rx bytes for V4 Iface");
      }
	  if(!ps_stat_get_iface(PS_STAT_IFACE_BYTES_RX,
										  v6_ps_iface_p,
										  (void *)&(temp_v6rxbytes),
										  sizeof(temp_v6rxbytes)))
	  {
	    DS_3GPP_ERROR_LOG_0("Unable to get rx bytes for V6 Iface");
	  }
  
	  current_rx_wm_cnt = temp_v4rxbytes+temp_v6rxbytes;

	  prev_rx_wm_cnt = pdn_context_p->
	  	ds_pdn_context_dyn_p->throughput_stats.prev_rxed_data_pkt_cnt;
	  
	  prev_rx_running_avg = pdn_context_p->
	  	ds_pdn_context_dyn_p->throughput_stats.actual_dl_rate;
  
     /*------------------------------------------------------------------------- 
        Calculate new DL actual throughput rate; based on previous average
        current wm count;prev recieved count and curren wm count
      -------------------------------------------------------------------------*/
  
    pdn_context_p->ds_pdn_context_dyn_p->throughput_stats.actual_dl_rate 
        = ds_util_calculate_throughput_moving_avg_uldl
         (
           prev_rx_running_avg,
           prev_rx_wm_cnt,
           current_rx_wm_cnt,
           sampling_interval,
           moving_avg_alpha
         );
  
  
      DS_3GPP_MSG6_HIGH_EX(subs_id, "Prev DL Bytes Cnt transf 0x%lx%lx Current"
                           " byte cnt 0x%lx%lx Calculated Avg DL actual t'put 0x%lx%lx kbps",
                           QWORD_HIGH(prev_rx_wm_cnt),
                           QWORD_LOW(prev_rx_wm_cnt),
                           QWORD_HIGH(current_rx_wm_cnt),
                           QWORD_LOW(current_rx_wm_cnt),
                           QWORD_HIGH(pdn_context_p->ds_pdn_context_dyn_p->throughput_stats.actual_dl_rate),
                           QWORD_LOW(pdn_context_p->ds_pdn_context_dyn_p->throughput_stats.actual_dl_rate));
  
  
	  prev_rx_wm_cnt = pdn_context_p->
	  	  ds_pdn_context_dyn_p->throughput_stats.prev_rxed_data_pkt_cnt 
	  	    = current_rx_wm_cnt;
    }
    average_dl_data_rate = average_dl_data_rate + 
	  pdn_context_p->
	  	ds_pdn_context_dyn_p->throughput_stats.actual_dl_rate;

  }

  ds3gtputmgr_set_actual_datarates(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                   average_ul_data_rate,
                                   average_dl_data_rate,
                                   client);


  DS_3GPP_MSG9_HIGH_EX(subs_id, 
                       "client: %d, Alpha value 0x%x%x, Data in (uplink:%d, downlink:%d)"
                       "direction," 
 	                   "UL actual T'put values 0x%x%x DL Actual T'put Values 0x%x%x",
                       client,
  	               QWORD_HIGH(moving_avg_alpha),
                       QWORD_LOW(moving_avg_alpha), 
                       uplink_data_transmission, 
                       downlink_data_transmission, 
                       QWORD_HIGH(average_ul_data_rate),
                       QWORD_LOW(average_ul_data_rate), 
                       QWORD_HIGH(average_dl_data_rate),
                       QWORD_LOW(average_dl_data_rate));

  return;
}
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_INITIATE_ACTUAL_ULDL_THROUGHPUT

DESCRIPTION
  This function initializes the actual uldl throughput numbers for all the
  active bearers and ue per susbscription
  
PARAMETERS
   sys_modem_as_id_e_type      cm_subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE    - If the actual throughput has been refreshed successfully
  FALSE   - Otherwise
  
SIDE EFFECTS
  
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_initiate_actual_uldl_throughput
(
  sys_modem_as_id_e_type      subs_id 
)
{
  ds_bearer_context_s        *bearer_context_p = NULL;
  int                         index = 0;

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = NULL;
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }
    ds_3gpp_bearer_cntxt_initiate_uld_stats_per_bearer(bearer_context_p);
  }
}
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_INITIATE_ULDL_THROUGHPUT

DESCRIPTION
  This function initializes the uldl stats per bearer.
  
PARAMETERS
   sys_modem_as_id_e_type      cm_subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE    - If the actual throughput has been refreshed successfully
  FALSE   - Otherwise
  
SIDE EFFECTS
  
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_initiate_uld_stats_per_bearer
(
   ds_bearer_context_s *bearer_context_p
)
{

  if( ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    /*------------------------------------------------------------------------- 
     Initialize the previous received count with the current water mark
     count. When the timer expires, we get a better estimate of the
     difference in the byte count to calculate the actual throughput
    -------------------------------------------------------------------------*/
    bearer_context_p->ds_bearer_context_dyn_p->
      bearer_throughput_stats_uldl.actual_dl_rate  = 0;
    bearer_context_p->ds_bearer_context_dyn_p->
      bearer_throughput_stats_uldl.actual_ul_rate  = 0;
    if (bearer_context_p->ds_bearer_context_dyn_p->call_mode  == 
        SYS_SYS_MODE_LTE)
    {
      if ((bearer_context_p->lte_rx_wm.wm_item.total_rcvd_cnt - 
           bearer_context_p->lte_rx_wm.wm_item.current_cnt) > 0)
      {
      bearer_context_p->ds_bearer_context_dyn_p->
        bearer_throughput_stats_uldl.total_prev_recd_rx_wm_cnt = 
            bearer_context_p->lte_rx_wm.wm_item.total_rcvd_cnt - 
              bearer_context_p->lte_rx_wm.wm_item.current_cnt;
    }
    else
    {
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.total_prev_recd_rx_wm_cnt = 0;

      }
    }
    else
    {
      if ((bearer_context_p->rx_wm.wm_item.total_rcvd_cnt - 
           bearer_context_p->rx_wm.wm_item.current_cnt) > 0)
      {
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.total_prev_recd_rx_wm_cnt = 
            bearer_context_p->rx_wm.wm_item.total_rcvd_cnt - 
              bearer_context_p->rx_wm.wm_item.current_cnt;
      }
      else
      {
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.total_prev_recd_rx_wm_cnt = 0;
      }
    }

    if ((bearer_context_p->tx_wm.wm_item.total_rcvd_cnt - 
           bearer_context_p->tx_wm.wm_item.current_cnt) > 0)
    {
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.total_prev_recd_tx_wm_cnt = 
            bearer_context_p->tx_wm.wm_item.total_rcvd_cnt - 
              bearer_context_p->tx_wm.wm_item.current_cnt;
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.prev_tx_wm_cnt_for_bidir = 
            bearer_context_p->tx_wm.wm_item.total_rcvd_cnt - 
              bearer_context_p->tx_wm.wm_item.current_cnt;
    }
    else
    {
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.total_prev_recd_tx_wm_cnt = 0;
        bearer_context_p->ds_bearer_context_dyn_p->
          bearer_throughput_stats_uldl.prev_tx_wm_cnt_for_bidir = 0;
    }
   }
}
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CLEANUP_PENDING_CALLS

DESCRIPTION
  This function cleans up bearers in PEND_REDIAL state after a rat change. 
  
PARAMETERS  
  bearer_context_p - bearer context pointer
  new_mode         - new_rat
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
static void ds_bearer_cntxt_cleanup_pending_calls
(
  ds_bearer_context_s  *bearer_context_p,
  sys_sys_mode_e_type  new_mode
)
{
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;
  /*----------------------------------------------------------------------*/    
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  if ( (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_PEND_REDIAL) &&
       (bearer_context_p->ds_bearer_context_dyn_p->cm_call_id == CM_CALL_ID_INVALID) )
  {
    /*------------------------------------------------------------------------------- 
      1. Here we check whether the system moved from G/W to L and back to G/W. 
         We also check whether the call id is INVALID.
         If both conditions are true then we need to cleanup the call.  
         Otherwise the call will be stuck in Pending redial state.
  
       - We need to handle the case of L -> G/W -> L once we support pending redial
         calls while moving from L -> G/W.
    
      2. Also tear down a call in PENDING REDIAL STATE if another bearer is in UP 
         state.
         Ex of such a case
         Call 1 brought up on G/W/T on the non attach profile  --> Bearer is UP
         Call 2 in coming up state on G/W/T but on the attach profile, 
         Now the call 2 is marked as PENDING REDIAL but no redial ever happens on 
         moving to LTE because call 1's bearer was UP, If not for this check this 
         bearer will be left hanging in pending redial state on LTE.
    -------------------------------------------------------------------------------*/
    if ( ((bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE) &&
         (new_mode != SYS_SYS_MODE_LTE)) ||
         (ds_bearer_cntx_is_any_other_bearer_up(bearer_context_p) && 
          new_mode == SYS_SYS_MODE_LTE) )
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Bearer_inst: %d is in PEND_REDIAL state, cleaning...",
                        (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));
      ds_bearer_cntx_local_cleanup(bearer_context_p);
    }
  }

} /* ds_bearer_cntxt_cleanup_pending_calls */


#ifdef FEATURE_QXDM_WM_TRACING
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_REPORT_WM_STATS

DESCRIPTION
  This function internally call umts or lte specific function to report wm 
  status. 
  
PARAMETERS  
  bearer_instance - bearer context instance
  wm_stat_ptr     - pointer where we store the wm values
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_report_wm_stats
(
  uint32          bearer_instance,
  fc_wm_stat_type *wm_stat_ptr
)
{
  ds_bearer_context_s    *bearer_context_p = NULL;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;
  /*----------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/
  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  DS_BEARER_CTXT_VF_CALL
  (    
    report_wm_stats,
    subs_id,
    bearer_instance,
    wm_stat_ptr
  );
} /* ds_bearer_cntxt_report_wm_stats */
#endif /* FEATURE_QXDM_WM_TRACING */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_LTE_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date  date wm level value for LTE

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  um_wm_threshold_p - pointer to WM threshold NV structure
  subs_id       - subscription ID
  wm_type     - WM level index (LO,HI,DNE)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_lte_ul_wm_level
(
  ds_bearer_context_s                         *bearer_cntxt_p, 
  ds_bearer_cntxt_um_wm_threshold_struct_type *um_wm_threshold_p,  
  uint8                                        wm_level_type,
  sys_modem_as_id_e_type                       subs_id
)
{
  uint32                                       wm_level = 0;
  uint32                                       uplink_data_rate = 0;
  uint32                                       downlink_data_rate = 0;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if((!ds3gsubsmgr_is_subs_id_valid(subs_id)) ||
     (bearer_cntxt_p==NULL)                   ||
     (um_wm_threshold_p == NULL)              ||
     ((wm_level_type < UL_WM_LO_INDEX) || (wm_level_type >= UL_WM_ARRAY_SIZE)))
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Input parameter is NULL");
    return wm_level;
  }

  /* Set based on NV if it is set, else set based on ue cat */
  if (um_wm_threshold_p->lte_wm_levels_valid == TRUE)
  {
    switch(wm_level_type)
    {
     case UL_WM_LO_INDEX:
       wm_level = um_wm_threshold_p->lte_ul_lo;
       break;
     case UL_WM_HI_INDEX:
       wm_level = um_wm_threshold_p->lte_ul_hi;
       break;
     case UL_WM_DNE_INDEX:
       wm_level = um_wm_threshold_p->lte_ul_dne;
       break;
     case UL_WM_DNE_QCNT_INDEX:
       wm_level = DSLTEPS_UL_WM_DEFAULT_DNE_QCNT;
       break;
     default:
       DS_3GPP_ERROR_LOG_1_EX(subs_id, "invalid UL data rate type passed: d", 
                          wm_level_type);
       break;
    }
  }
  else
  {
    switch(bearer_cntxt_p->tx_wm.wm_item.lo_watermark)
    {
      case DSLTEPS_UL_WM_LO_50M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSLTEPS_UL_WM_LO_50M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSLTEPS_UL_WM_HI_50M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSLTEPS_UL_WM_DNE_50M;
            break;
          case UL_WM_DNE_QCNT_INDEX:
            wm_level = DSLTEPS_UL_WM_DNE_QCNT_50M;
            break;
          default:
            break;
        }
        break;
      case DSLTEPS_UL_WM_LO_75M:
        switch(wm_level_type)
        {
           case UL_WM_LO_INDEX:
             wm_level = DSLTEPS_UL_WM_LO_75M;
             break;
           case UL_WM_HI_INDEX:
             wm_level = DSLTEPS_UL_WM_HI_75M;
             break;
           case UL_WM_DNE_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_75M;
             break;
           case UL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_QCNT_75M;
             break;
           default:
             break;
        }
        break;
      case DSLTEPS_UL_WM_LO_100M:
        switch(wm_level_type)
        {
           case UL_WM_LO_INDEX:
             wm_level = DSLTEPS_UL_WM_LO_100M;
             break;
           case UL_WM_HI_INDEX:
             wm_level = DSLTEPS_UL_WM_HI_100M;
             break;
           case UL_WM_DNE_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_100M;
             break;
           case UL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_QCNT_100M;
             break;
           default:
             break;
        }
        break;

     case DSLTEPS_UL_WM_LO_150M:
        switch(wm_level_type)
        {
           case UL_WM_LO_INDEX:
             wm_level = DSLTEPS_UL_WM_LO_150M;
             break;
           case UL_WM_HI_INDEX:
             wm_level = DSLTEPS_UL_WM_HI_150M;
             break;
           case UL_WM_DNE_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_150M;
             break;
           case UL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_UL_WM_DNE_QCNT_150M;
             break;
           default:
             break;
        }
        break;
    }
  }

   uplink_data_rate = ds_eps_get_configured_ul_data_rate(subs_id);
   downlink_data_rate = ds_eps_get_configured_dl_data_rate(subs_id);

   if ((uplink_data_rate <= DSEPS_UL_RATE_50_M ) &&
       (wm_level_type == UL_WM_DNE_INDEX))
   {
     if (ds3gsubsmgr_is_device_multi_sim_mode() &&
         !ds_3gpp_flow_control_get_lte_page_share_status()) 
     {
       um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);
       if(um_wm_threshold.lte_wm_levels_valid)
       {
         wm_level = um_wm_threshold.lte_ul_dne;
       }
       else
       {
         wm_level = DSLTEPS_UL_WM_DNE_50_MSIM_BIDIR;
       }
       DS_3GPP_MSG3_HIGH_EX(subs_id, "UL data dne level bidir : %d ul rate %d dl rate %d", 
                         wm_level,uplink_data_rate,downlink_data_rate);        
     }
   }
     
    if((bearer_cntxt_p->tx_wm.wm_item.hiwater_func_ptr == NULL) &&
       (wm_level_type == UL_WM_DNE_INDEX))
    {
      if (uplink_data_rate > DSEPS_UL_RATE_100_M )
      {
        if (downlink_data_rate > DSEPS_DL_RATE_300_M )
        {
          wm_level = DSLTEPS_UL_WM_150UL_450DL_DNE;
        }
        else if (downlink_data_rate > DSEPS_DL_RATE_450_M)
        {
          wm_level = DSLTEPS_UL_WM_150UL_600DL_DNE;
        }
        else
        {
          wm_level = DSLTEPS_UL_WM_150UL_DL_DNE;
        }
        
       }
        DS_3GPP_MSG3_HIGH_EX(subs_id, "UL data dne level bidir : %d ul rate %d dl rate %d", 
                          wm_level,uplink_data_rate,downlink_data_rate); 
      }

  return wm_level;
} /* ds_bearer_cntx_get_lte_ul_wm_level */


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_LTE_DL_WM_LEVEL

DESCRIPTION
  This function returns uplink date  date wm level value for LTE

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  um_wm_threshold_p - pointer to WM threshold NV structure
  subs_id       - subscription ID
  wm_type     - WM level index (LO,HI,DNE)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_lte_dl_wm_level
(
  ds_bearer_context_s                         *bearer_cntxt_p, 
  ds_bearer_cntxt_um_wm_threshold_struct_type *um_wm_threshold_p,  
  uint8                                        wm_level_type,
  sys_modem_as_id_e_type                       subs_id
)
{
  uint32  wm_level = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if((!ds3gsubsmgr_is_subs_id_valid(subs_id)) ||
     (bearer_cntxt_p==NULL)                   ||
     (um_wm_threshold_p == NULL)              ||
     ((wm_level_type < DL_WM_LO_INDEX)        || 
     (wm_level_type >= DL_WM_ARRAY_SIZE)))
  {
    return wm_level;
  }

  /* Set based on NV if it is set, else set based on ue cat */
  if (um_wm_threshold_p->lte_wm_levels_valid == TRUE)
  {
    switch(wm_level_type)
    {
     case DL_WM_LO_INDEX:
       wm_level = um_wm_threshold_p->lte_dl_lo;
       break;
     case DL_WM_HI_INDEX:
       wm_level = um_wm_threshold_p->lte_dl_hi;
       break;
     case DL_WM_DNE_INDEX:
       wm_level = um_wm_threshold_p->lte_dl_dne;
       break;
     case DL_WM_DNE_QCNT_INDEX:
       wm_level = DSLTEPS_DL_WM_DEFAULT_DNE_QCNT;
       break;
     default:
       DS_3GPP_ERROR_LOG_1_EX(subs_id, "invalid UL data rate type passed: d", 
                          wm_level_type);
       break;
    }
  }
  else
  {
    switch(bearer_cntxt_p->lte_rx_wm.wm_item.lo_watermark)
    {
      case DSLTEPS_DL_WM_LO_150M:
        switch(wm_level_type)
        {
          case DL_WM_LO_INDEX:
            wm_level = DSLTEPS_DL_WM_LO_150M;
            break;
          case DL_WM_HI_INDEX:
            wm_level = DSLTEPS_DL_WM_HI_150M ;
            break;
          case DL_WM_DNE_INDEX:
            wm_level = DSLTEPS_DL_WM_DNE_150M ;
            break;
          case DL_WM_DNE_QCNT_INDEX:
            wm_level = DSLTEPS_DL_WM_DNE_QCNT_150M;
            break;
          default:
            break;
        }
        break;
       case DSLTEPS_DL_WM_LO_300M:
         switch(wm_level_type)
         {
           case DL_WM_LO_INDEX:
             wm_level = DSLTEPS_DL_WM_LO_300M;
             break;
           case DL_WM_HI_INDEX:
             wm_level = DSLTEPS_DL_WM_HI_300M ;
             break;
           case DL_WM_DNE_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_300M ;
             break;
           case DL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_QCNT_300M;
             break;
           default:
             break;
         }
         break;
       case DSLTEPS_DL_WM_LO_450M:
         switch(wm_level_type)
         {
           case DL_WM_LO_INDEX:
             wm_level = DSLTEPS_DL_WM_LO_450M;
             break;
           case DL_WM_HI_INDEX:
             wm_level = DSLTEPS_DL_WM_HI_450M ;
             break;
           case DL_WM_DNE_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_450M ;
             break;
           case DL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_QCNT_450M;
             break;
           default:
             break;
         }
         break;
       case DSLTEPS_DL_WM_LO_600M:
         switch(wm_level_type)
         {
           case DL_WM_LO_INDEX:
             wm_level = DSLTEPS_DL_WM_LO_600M;
             break;
           case DL_WM_HI_INDEX:
             wm_level = DSLTEPS_DL_WM_HI_600M ;
             break;
           case DL_WM_DNE_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_600M ;
             break;
           case DL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_QCNT_600M;
             break;
           default:
             break;
         }
         break;
       case DSLTEPS_DL_WM_LO_1000M:
         switch(wm_level_type)
         {
           case DL_WM_LO_INDEX:
             wm_level = DSLTEPS_DL_WM_LO_1000M;
             break;
           case DL_WM_HI_INDEX:
             wm_level = DSLTEPS_DL_WM_HI_1000M ;
             break;
           case DL_WM_DNE_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_1000M ;
             break;
           case DL_WM_DNE_QCNT_INDEX:
             wm_level = DSLTEPS_DL_WM_DNE_QCNT_1000M;
             break;
           default:
             break;
         }
         break;
       default:
        switch(wm_level_type)
        {
          case DL_WM_LO_INDEX:
            wm_level = DSLTEPS_DL_WM_DEFAULT_LO;
            break;
          case DL_WM_HI_INDEX:
            wm_level = DSLTEPS_DL_WM_DEFAULT_HI;
            break;
          case DL_WM_DNE_INDEX:
         wm_level = DSLTEPS_DL_WM_DEFAULT_DNE;
         break;
          case DL_WM_DNE_QCNT_INDEX:
            wm_level = DSLTEPS_DL_WM_DEFAULT_DNE_QCNT;
            break;
          default:
            break;
        }
         break;
     }
  }

  /*PC Loopback*/
  if (TRUE == ds_3gpp_nv_manager_get_data_shark())
  {
    switch(wm_level_type)
    {
      case DL_WM_LO_INDEX:
           wm_level = DSLTEPS_DL_WM_PC_LB_LO;
           break;
      case DL_WM_HI_INDEX:
           wm_level = DSLTEPS_DL_WM_PC_LB_HI;
           break;
      case DL_WM_DNE_INDEX:
           wm_level = DSLTEPS_DL_WM_PC_LB_DNE;
           break;
      case DL_WM_DNE_QCNT_INDEX:
           wm_level = DSLTEPS_DL_WM_PC_LB_DNE_QCNT;
           break;
      default:
           break;
    }
  }

  return wm_level;
} /* ds_bearer_cntx_get_lte_dl_wm_level */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_WCDMA_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date wm level value for WCDMA

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  um_wm_threshold_p - pointer to WM threshold NV structure
   wm_type - WM level index

DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_wcdma_ul_wm_level
(
  ds_bearer_context_s                          *bearer_cntxt_p,
  ds_bearer_cntxt_um_wm_threshold_struct_type  *um_wm_threshold_p,
  uint8                                         wm_level_type
)
{
  uint32                  wm_level = 0;
  boolean                 get_default_rate = FALSE;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if((bearer_cntxt_p==NULL)                         ||
     (um_wm_threshold_p == NULL)                    ||
     ((wm_level_type <  UL_WM_LO_INDEX)         ||
      (wm_level_type >= UL_WM_DNE_QCNT_INDEX)))
  {
    DS_3GPP_ERROR_LOG_0("In put parameter is NULL");
    return wm_level;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntxt_p, &subs_id);

  if( ds_3gpp_nv_manager_get_enable_dynamic_wcdma_wm_tuning() == FALSE )
  {
     get_default_rate = TRUE;
  }
  else
  {
    switch(bearer_cntxt_p->tx_wm.wm_item.lo_watermark)
    {
      case DSWPSD_UL_WM_LO_0_7M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSWPSD_UL_WM_LO_0_7M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSWPSD_UL_WM_HI_0_7M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSWPSD_UL_WM_DNE_0_7M;
            break;
          default:
            break;
        }
        break;

      case DSWPSD_UL_WM_LO_1_44M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSWPSD_UL_WM_LO_1_44M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSWPSD_UL_WM_HI_1_44M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSWPSD_UL_WM_DNE_1_44M;
            break;
          default:
            break;
        }
        break;

      case DSWPSD_UL_WM_LO_2_88M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSWPSD_UL_WM_LO_2_88M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSWPSD_UL_WM_HI_2_88M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSWPSD_UL_WM_DNE_2_88M;
            break;
          default:
            break;
        }
        break;

      /*---------------------------------------------------------------------
         To avoid compilation error: Since WM levels map to same value
         Un-comment it if values change in future
      ---------------------------------------------------------------------*/
      #if 0
      case DSWPSD_UL_WM_LO_5_74M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSWPSD_UL_WM_LO_5_74M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSWPSD_UL_WM_HI_5_74M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSWPSD_UL_WM_DNE_5_74M;
            break;
          default:
            break;
        }
        break;
        #endif

      case DSWPSD_UL_WM_LO_11_4M:
        switch(wm_level_type)
        {
          case UL_WM_LO_INDEX:
            wm_level = DSWPSD_UL_WM_LO_11_4M;
            break;
          case UL_WM_HI_INDEX:
            wm_level = DSWPSD_UL_WM_HI_11_4M;
            break;
          case UL_WM_DNE_INDEX:
            wm_level = DSWPSD_UL_WM_DNE_11_4M;
            break;
          default:
            break;
        }
        break;

      default:
        get_default_rate = TRUE;
        break;
    }
  }

  if(get_default_rate)
  {
    switch(wm_level_type)
    {
      case UL_WM_LO_INDEX:
        wm_level = um_wm_threshold_p->wcdma_ul_lo;
        break;
      case UL_WM_HI_INDEX:
        wm_level = um_wm_threshold_p->wcdma_ul_hi;
        break;
      case UL_WM_DNE_INDEX:
        wm_level = um_wm_threshold_p->wcdma_ul_dne;
        break;
      default:
        DS_3GPP_ERROR_LOG_1_EX(subs_id, "invalid UL data rate type passed: d",
                           wm_level_type);
        break;
    }
  }
  return wm_level;
} /* ds_bearer_cntx_get_wcdma_ul_wm_level */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_TDSCDMA_UL_WM_LEVEL

DESCRIPTION
  This function returns uplink date wm level value for TDSCDMA

PARAMETERS
  *bearer_cntxt_p  - bearer_context pointer
  tds_um_wm_threshold_p - pointer to WM threshold NV structure
  wm_type - WM level

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
uint32 ds_bearer_cntx_get_tdscdma_ul_wm_level
(
  ds_bearer_context_s                              *bearer_cntxt_p,
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type  *tds_um_wm_threshold_p,
  uint8                                             wm_level_type
)
{
  uint32                  wm_level = 0;
  boolean                 get_default_rate = FALSE;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (bearer_cntxt_p == NULL)                       ||
      (tds_um_wm_threshold_p == NULL)                ||
      ((wm_level_type < UL_WM_LO_INDEX)          ||
       (wm_level_type >= UL_WM_DNE_QCNT_INDEX)))
  {
    DS_3GPP_ERROR_LOG_0("ds_bearer_cntx_get_tdscdma_ul_rate: Invalid input");
    return wm_level;
  }
  
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntxt_p, &subs_id);

  switch( bearer_cntxt_p->tx_wm.wm_item.lo_watermark )
  {
    case DSTPSD_UL_WM_LO_16:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_16;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_HI_16;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_16;
          break;
        default:
          break;
      }
      break;

    case DSTPSD_UL_WM_LO_32:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_32;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_HI_32;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_32;
          break;
        default:
          break;
      }
      break;

    case DSTPSD_UL_WM_LO_64:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_64;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_LO_64;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_64;
          break;
        default:
          break;
      }
      break;

    case DSTPSD_UL_WM_LO_128:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_128;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_HI_128;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_128;
          break;
        default:
          break;
      }
      break;

    case DSTPSD_UL_WM_LO_256:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_256;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_LO_256;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_256;
          break;
        default:
          break;
      }
      break;

    case DSTPSD_UL_WM_LO_384:
      switch(wm_level_type)
      {
        case UL_WM_LO_INDEX:
          wm_level = DSTPSD_UL_WM_LO_384;
          break;
        case UL_WM_HI_INDEX:
          wm_level = DSTPSD_UL_WM_HI_384;
          break;
        case UL_WM_DNE_INDEX:
          wm_level = DSTPSD_UL_WM_DNE_384;
          break;
        default:
          break;
      }
      break;

    default:
     get_default_rate = TRUE;
      break;
  }

  if(get_default_rate)
  {
    switch(wm_level_type)
    {
      case UL_WM_LO_INDEX:
        wm_level = tds_um_wm_threshold_p->tds_ul_lo;
        break;
      case UL_WM_HI_INDEX:
        wm_level = tds_um_wm_threshold_p->tds_ul_hi;
        break;
      case UL_WM_DNE_INDEX:
        wm_level = tds_um_wm_threshold_p->tds_ul_dne;
        break;
      default:
        DS_3GPP_ERROR_LOG_1_EX(subs_id, "invalid UL data rate type passed: d",
                           wm_level_type);
        break;
    }
  }
  return wm_level;
} /* ds_bearer_cntx_get_tdscdma_ul_wm_level */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_START_BP_TIMER

DESCRIPTION
  This function starts the BP removal timer

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer
  subs_id          - subscription id

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_bearer_cntxt_start_bp_timer
(
  ds_bearer_context_s    *bearer_cntxt_p,
  sys_modem_as_id_e_type  subs_id
)
{
  ps_timer_handle_type                  tmr_hndl_dyn_back_pressure;
  uint32                                bearer_inst;
  ds_bearer_cntxt_tput_opt_struct_type  tput_opt_params;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*------------------------------------------------------------------------
    The timer is started is in PS task context, this code is to 
    prevent a race condition between PS and DS task when timer starts at
    the same time when network ends the data call.
  ------------------------------------------------------------------------*/
  ds3gpp_enter_global_crit_section();
  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntxt_p))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }

  tput_opt_params = ds_3gpp_nv_get_back_pressure_params(subs_id);
  tmr_hndl_dyn_back_pressure = 
        bearer_cntxt_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure;
  bearer_inst = 
        (uint32)(bearer_cntxt_p->ds_bearer_context_dyn_p->client_data_ptr);
  ds3gpp_leave_global_crit_section();

  if (TRUE == ps_timer_is_running(tmr_hndl_dyn_back_pressure))
  {
    DS_3GPP_MSG0_LOW("BP timer is already running, not starting again");
    return;
  }

  /*--------------------------------------------------------------------------
    It is possible that the  IRAT from G to W, data was resumed and a
    DL TCP session started, in this case BP timer handle would not have been 
    allocated.
  --------------------------------------------------------------------------*/
  if (PS_TIMER_INVALID_HANDLE == tmr_hndl_dyn_back_pressure)
  {
    DS_3GPP_MSG0_LOW("disable_back_pressure(): timer_hdl not allocated yet."
                      "Allocate new hndl");

    tmr_hndl_dyn_back_pressure
        = ps_timer_alloc(ds_bearer_cntxt_tmr_dyn_back_pressure_exp_hdlr,
                         (void *)bearer_cntxt_p);

    if (PS_TIMER_INVALID_HANDLE == tmr_hndl_dyn_back_pressure)
    {
      DS_3GPP_ERROR_LOG_1_EX(subs_id, "Invalid PS timer handle for bearer inst: d",
                          bearer_inst);
      return;
    }
  }

  if (PS_TIMER_SUCCESS != 
      ps_timer_start(tmr_hndl_dyn_back_pressure, 
                     (1000 * tput_opt_params.timer_val)))
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "Failed to start timer for bearer inst: d",
                        bearer_inst);
  }
  else
  {
    DS_3GPP_MSG2_HIGH_EX(subs_id, "Started BP timer for 0x%x%x seconds", 
                         QWORD_HIGH(tput_opt_params.timer_val),
                         QWORD_LOW(tput_opt_params.timer_val));
  }

  ds3gpp_enter_global_crit_section();
  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntxt_p))
  {
    ps_timer_free(tmr_hndl_dyn_back_pressure);
    ds3gpp_leave_global_crit_section();
    return;
  }

  /* Update bearer_cntxt_p with new timer handle if its allocated here */
  bearer_cntxt_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure =
                                                   tmr_hndl_dyn_back_pressure;
  ds3gpp_leave_global_crit_section();

  return;
}/* ds_bearer_cntxt_start_bp_timer */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_STOP_BP_TIMER

DESCRIPTION
  This function stops the BP removal timer

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_bearer_cntxt_stop_bp_timer
(
  ds_bearer_context_s  *bearer_cntxt_p
)
{
  ps_timer_handle_type    tmr_hndl_dyn_back_pressure;
  uint32                  bearer_inst;
  ps_timer_error_type     timer_error;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*------------------------------------------------------------------------
    The timer is started is in PS task context, this code is to 
    prevent a race condition between PS and DS task when timer starts at
    the same time when network ends the data call.
  ------------------------------------------------------------------------*/
  ds3gpp_enter_global_crit_section();
  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntxt_p, &subs_id))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }

  tmr_hndl_dyn_back_pressure = 
        bearer_cntxt_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure;
  bearer_inst = 
        (uint32)(bearer_cntxt_p->ds_bearer_context_dyn_p->client_data_ptr);
  ds3gpp_leave_global_crit_section();

  if( FALSE == ps_timer_is_running(tmr_hndl_dyn_back_pressure) )
  {
    DS_3GPP_MSG1_LOW("BP timer is already stopped for bearer inst:%d",
                     bearer_inst);
    return;
  }

  timer_error = ps_timer_cancel(tmr_hndl_dyn_back_pressure);

  DS_3GPP_MSG2_MED_EX(subs_id, "Stop BP timer for bearer inst:%d success:%d",
                   bearer_inst, timer_error);

  /*----------------------------------------------------------------------
    Reset the Ack counters and enable back pressure
  ----------------------------------------------------------------------*/
  bearer_cntxt_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter =
       bearer_cntxt_p->ds_bearer_context_dyn_p->last_tcp_dl_ack_count = 0;

  bearer_cntxt_p->tx_wm.wm_item.hiwater_func_ptr =
                   (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_disable_cb;

  return;
} /* ds_bearer_cntxt_stop_bp_timer */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_TMR_DYN_BACK_PRESSURE_EXP_HDLR

DESCRIPTION
  This is the timer exp hdlr for dynamic back pressure disable. This would be 
  called in PS task context.

PARAMETERS
  user_data_ptr - user data.

DEPENDENCIES
  None.

RETURN VALUE
  None 

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_tmr_dyn_back_pressure_exp_hdlr
(
  void  *user_data_ptr
)
{
  ds_cmd_type *cmd_ptr = NULL;
  /*--------------------------------------------------------------------
         Get command buffer for new comman
  --------------------------------------------------------------------*/
  cmd_ptr = ds_allocate_cmd_buf(DS_CMD_PAYLOAD_SIZE_ZERO);
  if( cmd_ptr == NULL)
  {
    DS_3GPP_MSG0_HIGH("ds_bearer_cntxt_tmr_dyn_back_pressure_exp_hdlr no memory");
    return;
  }
	 
  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_BEARER_CNTXT_TMR_DYN_BACK_PRESSURE;
  /* Posting DS Cmd to process tmr_dyn_back_pressure_exp_hdlr */
  cmd_ptr->cmd_payload_ptr = user_data_ptr;
  ds_put_cmd(cmd_ptr);
}


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_DISABLE_BACK_PRESSURE

DESCRIPTION
  This function forwards pkts received to the lower layers for pdp-ip call.
  This function is registered with ps_iface for a PDP-IP call 

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_bearer_cntxt_disable_back_pressure
(
  ds_bearer_context_s  *bearer_cntxt_p
)
{
  uint32                                           wm_dne_level = 0;
  ds_bearer_context_wm_s                          *watermark_p = NULL;
  ds_pdn_context_s                                *pdn_cntx_p = NULL;
  sys_modem_as_id_e_type                           subs_id=SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type      um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type  tds_um_wm_threshold;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
    The timer is started is in PS task context, this code is to 
    prevent a race condition between PS and DS task when timer starts at
    the same time when network ends the data call.
  ------------------------------------------------------------------------*/
  ds3gpp_enter_global_crit_section();

  if(!ds_bearer_cntx_validate_bearer_context(bearer_cntxt_p))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }

  pdn_cntx_p = bearer_cntxt_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    ds3gpp_leave_global_crit_section();
    return;
  }

  DS_3GPP_MSG2_HIGH_EX(subs_id, 
     "Disable back pressure for bearer_inst: %d, bearer_mode: %d",
     (uint32)(bearer_cntxt_p->ds_bearer_context_dyn_p->client_data_ptr),
     (bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode));

  /*keep flow control only in LTE MSIM mode without page sharing*/
  if(!(ds3gsubsmgr_is_device_multi_sim_mode() &&
       (!ds_3gpp_flow_control_get_lte_page_share_status()) &&
       (ds3gpp_get_current_network_mode(subs_id) == SYS_SYS_MODE_LTE)))
  {
    /*-------------------------------------------------------------------------
      Disable back pressure as UL Acks are being received
    -------------------------------------------------------------------------*/
    bearer_cntxt_p->tx_wm.wm_item.hiwater_func_ptr = NULL;
  }
  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);
  tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);


  if (bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode ==
      SYS_SYS_MODE_LTE)
  {
     wm_dne_level = ds_bearer_cntx_get_lte_ul_wm_level(bearer_cntxt_p,
                                                       &um_wm_threshold,
                                                       UL_WM_DNE_INDEX,
                                                       subs_id);
  }
  else if (bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode ==
           SYS_SYS_MODE_WCDMA)
  {
    wm_dne_level= ds_bearer_cntx_get_wcdma_ul_wm_level(bearer_cntxt_p,
                                                       &um_wm_threshold,
                                                       UL_WM_DNE_INDEX);
  }
  else if (bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode ==
           SYS_SYS_MODE_GSM)
  {
    wm_dne_level = um_wm_threshold.gprs_ul_dne;
  }
#ifdef FEATURE_TDSCDMA
  else if (bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode ==
           SYS_SYS_MODE_TDS)
  {
    wm_dne_level = ds_bearer_cntx_get_tdscdma_ul_wm_level(bearer_cntxt_p,
                                                          &tds_um_wm_threshold,
                                                          UL_WM_DNE_INDEX);
  }
#endif /* FEATURE_TDSCDMA */
  else
  {
    DS_3GPP_ERROR_LOG_2_EX(subs_id, 
       "invalid call_mode: d in bearer_inst: d",
       bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode,
       (uint32)(bearer_cntxt_p->ds_bearer_context_dyn_p->client_data_ptr));
    ds3gpp_leave_global_crit_section();
    return;
  }

  /*-------------------------------------------------------------------------
    Update the last_tcp_ack_count and start the timer
  -------------------------------------------------------------------------*/
  bearer_cntxt_p->ds_bearer_context_dyn_p->last_tcp_dl_ack_count =
    bearer_cntxt_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter;

  ds3gpp_leave_global_crit_section();
  
  watermark_p = &(bearer_cntxt_p->tx_wm);
  if (NULL != watermark_p)
  {
    if (watermark_p->wm_item.dont_exceed_cnt < wm_dne_level )
    {
      DS_3GPP_MSG1_HIGH_EX(subs_id, "Setting UL WM DNE level: %d", wm_dne_level);
      dsm_set_dne(&(watermark_p->wm_item), wm_dne_level);
    }
  }

  return;
} /* ds_bearer_cntxt_disable_back_pressure */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CLEANUP_CALL_RESOURCES

DESCRIPTION
  This function clears bearer resources when call end is initiated.
  
PARAMETERS  
  pdn_cntx_p - PDN context for which the bearer needs to be cleaned.
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_bearer_cntxt_cleanup_call_resources
(
  ds_pdn_context_s  *pdn_cntx_p
)
{
  uint8                bearer_index;
  ds_bearer_context_s  *bearer_context_p = NULL;
  ds_bearer_cntxt_per_subs_info_type  *per_subs_info_hndl = NULL;
  sys_modem_as_id_e_type               subs_id = SYS_MODEM_AS_ID_NONE;
  /*------------------------------------------------------------------------*/

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p, &subs_id))
  {
     return;
  }

  per_subs_info_hndl = 
    ds_bearer_cntxt_get_per_subs_info_hndl(subs_id);

  if (!ds_bearer_cntxt_validate_per_subs_info_hndl
        (per_subs_info_hndl))
  {
    DS_3GPP_MSG1_LOW  ("Bearer Cntxt per subs info handle is invalid for"
                       "Subs Id(CM): %d",subs_id);
    return;
  }
  /*-------------------------------------------------------------------------
    Unset the bitmask for all bearer contexts in this pdn context
  -------------------------------------------------------------------------*/
  for(bearer_index = 0;
    bearer_index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;
    bearer_index++)
  {
    bearer_context_p =\
      (ds_bearer_context_s*)pdn_cntx_p->ds_pdn_context_dyn_p->bearer_context_ptr_tbl[bearer_index];

    if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      /*----------------------------------------------------------------------- 
      This entry in the bearer context table is NULL. Other entries could
      have non-NULL values.
    -----------------------------------------------------------------------*/
      continue;
    }

    DS_BEARER_CNTXT_UNSET_BITMASK(per_subs_info_hndl, bearer_context_p);

    /*-----------------------------------------------------------------------
      Reset the rab_reestab_retry counter for this bearer.
    -----------------------------------------------------------------------*/
    DS_BEARER_CNTXT_RESET_RAB_REESTAB_RETRY_CNT(bearer_context_p);

    /*----------------------------------------------------------------------
      Stop the Dynamic BP timer before initiating call end
    -----------------------------------------------------------------------*/
    if ( (PS_TIMER_INVALID_HANDLE != 
          bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure) &&
         (PS_TIMER_SUCCESS != 
          ps_timer_cancel(bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure)) )
    {
      DS_3GPP_ERROR_LOG_1_EX(subs_id, "Error in stopping BP timer with hndl: d",
                           bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure);
    }
    else
    {
      DS_3GPP_MSG0_HIGH_EX(subs_id, "BP timer stopped");
    }

    /* flush the watermark for this bearer */
    DS_BEARER_CTXT_VF_CALL_NO_ARG_PER_BEARER
    (
      empty_watermark,
      bearer_context_p    
    );
    /*-------------------------------------------------------------------------
      Stop the FCF timer 
    -------------------------------------------------------------------------*/
    ds_3gpp_flow_control_stop_fcf_timer(bearer_context_p);
    ds_3gpp_flow_control_set_pending_disable_fcf(bearer_context_p, FALSE);
    ds_3gpp_flow_control_set_tcp_bi_direction(bearer_context_p, FALSE);
  }

  if((per_subs_info_hndl->uplink_data_bitmask == 0) && 
     (per_subs_info_hndl->timer_running == TRUE))
  { 
    DS_BEARER_CNTXT_CLR_TIMER (per_subs_info_hndl);
    ds_3gpp_rab_reestab_reset_state(subs_id);
  }
} /* ds_bearer_cntxt_cleanup_call_resources */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_IS_ACTIVE_BEARER_CNTXT

DESCRIPTION
  This function returns TRUE if there is an active bearer context in the
  system. Else, it returns FALSE.
.
  
PARAMETERS  
  pdn_cntx_p - PDN context for which the bearer needs to be cleaned.
  subs_id    - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  boolean
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_is_active_bearer_cntxt
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s* bearer_context_p; //Pointer to bearer context
  uint32               bearer_cntx_inst;
  ds_bearer_context_state_e state = DS_BEARER_CONTEXT_STATE_DOWN;
  /*-----------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    loop through and check if there is an active bearer context.
  -------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;\
       bearer_cntx_inst<DS_3GPP_MAX_BEARER_CONTEXT;\
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
    
    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
       continue;
    }
            
    state = bearer_context_p->ds_bearer_context_dyn_p->state;

    if (state == DS_BEARER_CONTEXT_STATE_UP ||
        state == DS_BEARER_CONTEXT_STATE_UP_DORMANT || 
        state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB ||
        state == DS_BEARER_CONTEXT_STATE_PEND_RAB_REG)
    {
      return TRUE;
    }
  }
  return FALSE;
}/*ds_bearer_cntxt_is_active_bearer_cntxt*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_BEARERS_ON_LTE

DESCRIPTION
  This function returns TRUE if all the active bearers are in LTE mode.
  Else, it returns FALSE.
  
PARAMETERS  
  subs_id - Subscription Id
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  boolean
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_bearers_on_lte
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s* bearer_context_p; //Pointer to bearer context
  uint32               bearer_cntx_inst;
  boolean              bearer_valid_lte = FALSE;

  /*-------------------------------------------------------------------------
    loop through and check if there is any bearer which is not in LTE mode.
  -------------------------------------------------------------------------*/
  for( bearer_cntx_inst=0;
       bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_cntx_inst++ )
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
    
    if(ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                             subs_id))
    {
      bearer_valid_lte = TRUE;
          
     if ( (bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_LTE) )
     {
       return FALSE;
     }
    }
  }

  return bearer_valid_lte;

} /* ds_bearer_cntxt_bearers_on_lte */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntx_alloc_dyn_mem

DESCRIPTION
  This function allocates dyn memory.
 
PARAMETERS
  index   - index where the memory would be allocated
  subs_id - Subscription Id
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

=============================================================================*/
static boolean ds_bearer_cntx_alloc_dyn_mem
(
  int                    index,
  sys_modem_as_id_e_type subs_id
)
{
  boolean ret_val = FALSE;
  ds_bearer_context_dyn_s *bearer_context_dyn_p = NULL;
  ds_bearer_context_s      *bearer_context_p = NULL;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
  if ((index < 0) || (index >= DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT))
  {
      DS_3GPP_MSG1_LOW("Index:%d is out of bounds",index);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
  }

  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

  bearer_context_dyn_p = bearer_context_p->ds_bearer_context_dyn_p;

  if (bearer_context_dyn_p == NULL)
  {
    bearer_context_dyn_p = (ds_bearer_context_dyn_s *)modem_mem_alloc
                         (sizeof(ds_bearer_context_dyn_s),MODEM_MEM_CLIENT_DATA);

    if (bearer_context_dyn_p != NULL)
    {
      memset(bearer_context_dyn_p,0,sizeof(ds_bearer_context_dyn_s));
      bearer_context_dyn_p->cookie = BEARER_CNTXT_VALID_COOKIE;
      bearer_context_p->ds_bearer_context_dyn_p = bearer_context_dyn_p;
      bearer_context_dyn_p->index = index;
      DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer Context dyn memory allocated at index %d,"
                        "Subs Id(CM): %d", index, subs_id);
      ret_val = ds_3gpp_dyn_mem_hdlr_alloc_bearers_dyn_mem(index, subs_id);
      /*--------------------------------------------------------------------------- 
        If ret_val is not successful free umts/eps bearer contexts and then free
        bearer context too, We don't know what allocation failed and
        hence its safe to free everything
        ---------------------------------------------------------------------------*/
        if (ret_val == FALSE)
      {
        ds_3gpp_dyn_mem_hdlr_free_bearers_dyn_mem(bearer_context_p, subs_id);
        ds_bearer_cntx_free_dyn_mem(bearer_context_p);
          local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
          break;
      }
    }
    else
    {
        DS_3GPP_MSG0_LOW("Heap mem allocation failure");
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        break;
    }
  }
  else
  {
      DS_3GPP_MSG1_LOW("Bearer context already has dyn memory allocated at index:%d",
                       index);
      local_err = DS_3GPPI_LOCAL_ERR_VAL_3;
      break;
    }

  } while (0);

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_3_EX(subs_id, "ds_bearer_cntx_alloc_dyn_mem fails at index:d subs d err d",
                       index, subs_id, local_err);
  }

  return ret_val;
} /* ds_bearer_cntx_alloc_dyn_mem */


/*===========================================================================
FUNCTION DS_BEARER_CNTX_VALIDATE_BEARER_CONTEXT_INTERNAL

DESCRIPTION
  This function checks if the bearer context has been allocated dynamic memory

PARAMETERS
  bearer_context_p       - bearer_context to be validated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_validate_bearer_context_internal
(
  ds_bearer_context_s *bearer_context_p
)
{
  ds_bearer_context_dyn_s *bearer_context_dyn_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(bearer_context_p == NULL)
  {
    return FALSE;
  }

  bearer_context_dyn_p = bearer_context_p->ds_bearer_context_dyn_p;

  if ((bearer_context_dyn_p != NULL) && 
       bearer_context_dyn_p->cookie == BEARER_CNTXT_VALID_COOKIE)

  {
    return TRUE;
  }

  return FALSE;
} /*ds_bearer_cntx_validate_bearer_context_internal*/

/*===========================================================================
FUNCTION DS_BEARER_CNTX_VALIDATE_BEARER_CONTEXT

DESCRIPTION
  This function checks if the bearer context has been allocated dynamic memory
  but report for failure

PARAMETERS
  bearer_context_p       - bearer_context to be validated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_validate_bearer_context
(
  ds_bearer_context_s *bearer_context_p
)
{
  if(bearer_context_p == NULL ||
     !ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
  {
    DS_3GPP_ERROR_LOG_1("Bearer context:0xx is INVALID",bearer_context_p);
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================
FUNCTION ds_bearer_cntx_free_bearer_context

DESCRIPTION
  This function frees dynamic bearer context

PARAMETERS
  bearer_context_p       - bearer_context to be freed
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntx_free_dyn_mem
(
  ds_bearer_context_s *bearer_context_p
)
{
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*----------------------------------------------------------------------- 
    Don't care about the cookie here since we are freeing memory anyways.
   -----------------------------------------------------------------------*/
  if((bearer_context_p != NULL) && 
     (bearer_context_p->ds_bearer_context_dyn_p != NULL))
  {
    (void)ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
    DS_3GPP_MSG0_MED_EX(subs_id, "Freeing dyn allocated bearer pointer");
    if (bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_fcf != PS_TIMER_INVALID_HANDLE )
    {
       DS_3GPP_ERROR_LOG_1("Bearer PS Timer is not free yet 0x%x",bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_fcf);
    }
    modem_mem_free(bearer_context_p->ds_bearer_context_dyn_p,MODEM_MEM_CLIENT_DATA);
    bearer_context_p->ds_bearer_context_dyn_p = NULL;
  }
  else
  {
    DS_3GPP_ERROR_LOG_0("Bearer context passed is invalid");
  }
}
/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_rx_data_at_u_pdp_cb

DESCRIPTION
  This function is the nonempty callback function for the bearer's downlink
  watermark. As part of nonempty callback function processing we set the rx_sig

PARAMETERS
  bearer_context_p       - bearer_context to be freed
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_bearer_cntxt_rx_data_at_u_pdp_cb
(
 dsm_watermark_type *wm,
 void*              callback_data
)
{
  ds_bearer_context_s       *bearer_context_p;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Get call instance from the callbackdata passed in
  -------------------------------------------------------------------------*/
  bearer_context_p = (ds_bearer_context_s*) callback_data;
 
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Set Rx Signal.
  -------------------------------------------------------------------------*/
  /*lint -e641 */
  PS_SET_SIGNAL(bearer_context_p->ds_bearer_context_dyn_p->rx_sig);
  /*lint +e641 */
} /* ds_eps_bearer_cntxt_ipa_to_ps_data_cb() */
    

/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CNTXT_GET_NUM_FREE_BEARERS

DESCRIPTION
  This function returns the total number of free bearers in the
  bearer resource pool.
PARAMETERS 
  None
DEPENDENCIES
  None.

RETURN VALUE
  uint8     - total number of bearers in use

SIDE EFFECTS
  None.

===========================================================================*/
uint8 ds_3gpp_bearer_cntxt_get_num_free_bearers(void)
{
  int   num_free_bearers                    = 0;
  uint8 index                               = 0;  
  ds_bearer_context_s     *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (index =0; index <DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      num_free_bearers++;
    }
  }
  return (uint8)num_free_bearers;
} /* ds_3gpp_bearer_cntxt_get_num_free_bearers */

/*===========================================================================
FUNCTION  DS_3GPP_BEARER_CNTXT_GET_NUM_FREE_OR_GOING_DOWN_BEARERS

DESCRIPTION
    This function returns the total number of free bearers  or going down
    bearers in the bearer resource pool

PARAMETERS
  num_free_bearers_p:       Number of free bearers
  num_going_down_bearers_p: Number of going down bearers

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_bearer_cntxt_get_num_free_or_going_down_bearers
(
  uint8 *num_free_bearers_p,
  uint8 *num_going_down_bearers_p
)
{
  uint8                    index = 0;  
  ds_bearer_context_s     *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (num_free_bearers_p == NULL || num_going_down_bearers_p == NULL)
  {
    return;
  }

  for (index =0; index <DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      (*num_free_bearers_p)++;
    }
    else
    { 
      if (bearer_context_p->ds_bearer_context_dyn_p->state == 
            DS_BEARER_CONTEXT_STATE_GOING_DOWN)
      {
        (*num_going_down_bearers_p)++;
      }
    }
  }
  return;
} /* ds_3gpp_bearer_cntxt_get_num_free_or_going_down_bearers */

/*===========================================================================
FUNCTION ds_bearer_cntx_report_ps_wmk_logging

DESCRIPTION
  This function reports PS watermark per IFace

PARAMETERS
  bearer_context_p       - bearer_context to be freed
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntxt_report_ps_wmk_logging
(
  ds_bearer_context_s             *bearer_context_p,
  ps_wmk_logging_dorm_state_type     ps_event
)
{
  ds_pdn_context_s             *pdn_cntx_p = NULL;
  ps_iface_type                *ps_iface_v4_p = NULL; 
  ps_iface_type                *ps_iface_v6_p = NULL; 
  int                           api_ret_val_v4 = -1;
  int                           api_ret_val_v6 = -1;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  DS_3GPP_MSG3_LOW("WM logging onBearer instance: %d Tx WM count:%d,"
                    " Rx WM count:%d",
      (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr,
       bearer_context_p->tx_wm.wm_item.current_cnt,
       bearer_context_p->rx_wm.wm_item.current_cnt);

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }

  ps_iface_v4_p = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p);
  ps_iface_v6_p = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p);
 /*------------------------------------------------------------------------- 
   Indicate to PS the Physlink pointer, Rx watermark count,
   Tx watermark count and dormancy status.
   Dormancy status is set to REORIG_FROM_DORMANCY
 ---------------------------------------------------------------------------*/
  if (ps_iface_v4_p != NULL) 
  {
   /*------------------------------------------------------------------------- 
     Indicate to PS the Physlink pointer, Rx watermark count,
     Tx watermark count and dormancy status.
     Dormancy status is set to DORMANT
    -------------------------------------------------------------------------*/
    api_ret_val_v4 =  ps_wmk_logging_event_report_ex(
                                 &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                  EVENT_DS_UM_QUEUE_STATS_EX ,
                                  ps_event,
                                  bearer_context_p->rx_wm.wm_item.current_cnt,
                                  bearer_context_p->tx_wm.wm_item.current_cnt,
                                  ps_iface_v4_p
                                  );

    DS_3GPP_MSG3_LOW("Logging status %d for event %d IPV4 PS Iface 0x%x",
                      (api_ret_val_v4 == 0), ps_event,ps_iface_v4_p);

  }

  if (ps_iface_v6_p != NULL) 
  {
  /*------------------------------------------------------------------------- 
   Indicate to PS the Physlink pointer, Rx watermark count,
   Tx watermark count and dormancy status.
   Dormancy status is set to DORMANT
  -------------------------------------------------------------------------*/
  api_ret_val_v6 =  ps_wmk_logging_event_report_ex(
                                  &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                  EVENT_DS_UM_QUEUE_STATS_EX,
                                  ps_event,
                                  bearer_context_p->rx_wm.wm_item.current_cnt,
                                  bearer_context_p->tx_wm.wm_item.current_cnt,
                                  ps_iface_v6_p
                                  );

    DS_3GPP_MSG3_LOW("Logging status %d for event %d IPV4 PS Iface 0x%x",
                      (api_ret_val_v6 == 0), ps_event,ps_iface_v6_p);
  }

  DS_3GPP_MSG5_LOW("Logging for event %d IPV4 status %d PS v4 Iface 0x%x "
                    "IPV6 status %d PS v6 Iface 0x%x",
                    ps_event, (api_ret_val_v4 == 0), ps_iface_v4_p,
                    (api_ret_val_v6 == 0), ps_iface_v6_p);

}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_VALIDATE_BEARER_SUBSCRIPTION_INTERNAL

DESCRIPTION
  This function validates whether the subs id of the bearer matches the
  subs id that was passed in as parameter. No F3 if bearer context is invalid
  or does not belong to subs

PARAMETERS
  bearer_context_p : Bearer Context Ptr
  subs_id          : Subscription Id 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if Bearer Subscription Id is valid
  FALSE, otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_bearer_cntxt_validate_bearer_subscription_internal
(
  ds_bearer_context_s     *bearer_context_p,
  sys_modem_as_id_e_type   subs_id
)
{
  sys_modem_as_id_e_type   bearer_subs_id = SYS_MODEM_AS_ID_NONE;
  boolean                  ret_val = TRUE;
  /*-----------------------------------------------------------------------*/

  if(bearer_context_p == NULL ||
     !ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, 
                                                       &bearer_subs_id))
  {
    ret_val =  FALSE;
  }
  else if (bearer_subs_id != subs_id)
  {
    ret_val = FALSE;
  }

  return ret_val;
} /*ds_bearer_cntxt_validate_bearer_subscription_internal*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_VALIDATE_BEARER_SUBSCRIPTION

DESCRIPTION
  This function validates whether the subs id of the bearer matches the
  subs id that was passed in as parameter.

PARAMETERS
  bearer_context_p : Bearer Context Ptr
  subs_id          : Subscription Id 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if Bearer Subscription Id is valid
  FALSE, otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_bearer_cntxt_validate_bearer_subscription
(
  ds_bearer_context_s     *bearer_context_p,
  sys_modem_as_id_e_type   subs_id
)
{
  sys_modem_as_id_e_type   bearer_subs_id = SYS_MODEM_AS_ID_NONE;
  boolean                  ret_val = TRUE;
  /*-----------------------------------------------------------------------*/

  if(!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, 
                                                       &bearer_subs_id))
  {
    ret_val =  FALSE;
  }
  else if (bearer_subs_id != subs_id)
  {
    ret_val = FALSE;
  }

  return ret_val;
} /*ds_bearer_cntxt_validate_bearer_subscription*/

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function fetches a handle to the Bearer Context Information
  corresponding to the given Subscription.

PARAMETERS
  subs_id          : Subscription Id 

DEPENDENCIES
  None.

RETURN VALUE
  Handle to the Bearer Context Information for the given Subscription.

SIDE EFFECTS
  None.
===========================================================================*/
ds_bearer_cntxt_per_subs_info_type* ds_bearer_cntxt_get_per_subs_info_hndl
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_cntxt_per_subs_info_type *hndl = NULL;
  /*----------------------------------------------------------*/

  if (ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    hndl =  ds_bearer_cntxt_info.per_subs_info_p[subs_id];
  }

  return hndl;
} /* ds_bearer_cntxt_get_per_subs_info_hndl */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_SET_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function sets the handle to the Bearer Context Information
  corresponding to the given Subscription.

PARAMETERS
  subs_id          : Subscription Id
  hndl             : Handle to the Bearer Context Information for
                     the given Subscription.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_set_per_subs_info_hndl
(
  sys_modem_as_id_e_type        subs_id,
  ds_bearer_cntxt_per_subs_info_type  *hndl
)
{
  /*----------------------------------------------------------*/
  if (ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    ds_bearer_cntxt_info.per_subs_info_p[subs_id] = hndl;
  }
} /* ds_bearer_cntxt_set_per_subs_info_hndl */


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_VALIDATE_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function validates the handle to the Subscription Specific
  Bearer Context Information. 
  
PARAMETERS
  hndl  : Handle to the Subscription specific
          Bearer Context Information

DEPENDENCIES
  None.

RETURN VALUE
  TRUE,  if the handle is valid
  FALSE, if invalid

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_bearer_cntxt_validate_per_subs_info_hndl
(
  ds_bearer_cntxt_per_subs_info_type *hndl
)
{
  boolean ret_val = FALSE;
  /*----------------------------------------------------------*/

  if (hndl != NULL && hndl->cookie == DS_BEARER_CNTXT_PER_SUBS_INFO_COOKIE)
  {
    ret_val = TRUE;
  }
  else
  {
    DS_3GPP_ERROR_LOG_0("Bearer Cntxt per subs info handle is invalid");
  }
  
  return ret_val;
} /* ds_bearer_cntxt_validate_per_subs_info_hndl */



/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_ENABLE_FLOW

DESCRIPTION
  This function calls the ps_enable_flow command to enable the flow on
  the bearer. Clears flow_disable_timer if running
  
PARAMETERS 
  bearer_context_p: pointer to the bearer on which to disable flow on.
  flow_mask: bit mask that identifies the caller.
  send_cmd : send comand to PS instead of posting it in Ds task

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_enable_flow
(
  ds_bearer_context_s           *bearer_context_p,
  uint64                         flow_mask,
  boolean                        send_cmd
)
{
 ds_3gppi_local_err_val_e_type         local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
 sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE; 
  /* ----------------------------------------------------- */
  
  if (bearer_context_p == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }

  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

/*---------------------------------------------------------
  Few things to do here
  1. Check if for that particular bearer, timer is still running or not
  2. if running, reset timer and put the bool value to FALSE.
  If not , go ahead and enable flow on MH phys link
 
  if timer is not running, we have 2 cases:
  1. The timer had been running and it has expired now.
  2. The timer has not started yet i.e This is the first flow enable
  ------------------------------------------------------*/

  if ( bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running == TRUE)
  {
    DS_3GPP_MSG0_LOW ("Clearing flow_disable_timer");
    (void)rex_clr_timer(&(bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer));
    bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running =  FALSE;
    ds_3gpp_flow_control_offload_fc_logging(
       DS_OFFLOAD_FC_TIMER_CLEAR,
       (uint32)bearer_context_p,
       0,
       subs_id);
  }

#ifdef FEATURE_DATA_FLOW_MGMT
  ds_3gpp_burst_hdlr_get_grant_enable_flow(bearer_context_p,
                                             flow_mask);
#else
  if (send_cmd)
  {
    ps_phys_link_enable_flow_ex( 
       &(bearer_context_p->ds_bearer_context_dyn_p->phys_link), flow_mask );
  }
  else
  {
    ps_phys_link_enable_flow(
        &(bearer_context_p->ds_bearer_context_dyn_p->phys_link), flow_mask );
  }
#endif /*FEATURE_DATA_FLOW_MGMT */

func_exit:
  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_3gpp_bearer_cntxt_enable_flow failed, local err: %d",
                        local_err);
  }
} /* ds_3gpp_bearer_cntxt_enable_flow */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_DISABLE_FLOW

DESCRIPTION
  This function calls the ps_disable_flow command to disable the flow on
  the bearer and starts the flow disable timer if not already running.
  
PARAMETERS 
  bearer_context_p: pointer to the bearer on which to disable flow on.
  flow_mask: bit mask that identifies the caller.
  send_cmd : send comand to PS instead of posting it in Ds task

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_bearer_cntxt_disable_flow
(
  ds_bearer_context_s           *bearer_context_p,
  uint64                        flow_mask,
  boolean                        send_cmd
)
{
  ds_3gppi_local_err_val_e_type   local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  sys_modem_as_id_e_type          subs_id = SYS_MODEM_AS_ID_NONE;
  /* ---------------------------------------------------------------------- */

  if (bearer_context_p == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
    goto func_exit;
  }
  
  ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);
    
  /*------------------------------------------------------------------------
    Few things to do here
    1. if timer not running, start timer and set the flag to TRUE
    2. if timer IS running, do nothing with the timer, just go
     ahead and disable flow(since it might be for a different flow mask)
   -----------------------------------------------------------------------*/
  if ( bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running == FALSE )
  {
    /*------------------------------------------------------ 
     Start the timer. And put the flag to TRUE
     ----------------------------------------------------*/
    DS_3GPP_MSG1_LOW("Starting flow_disable_timer for %d ms",
                         ds3g_get_flow_disable_timer());

    (void)rex_set_timer
       (&(bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer),
         (rex_timer_cnt_type)ds3g_get_flow_disable_timer());

    bearer_context_p->ds_bearer_context_dyn_p->flow_disable_timer_running =  TRUE;

    ds_3gpp_flow_control_offload_fc_logging(
       DS_OFFLOAD_FC_TIMER_START,
       (uint32)bearer_context_p,
       ds3g_get_flow_disable_timer(),
       subs_id);
  }

#ifdef FEATURE_DATA_FLOW_MGMT
    ds_3gpp_burst_hdlr_sync_disable_flow(bearer_context_p,flow_mask);
#else
  if (send_cmd)
  {
    ps_phys_link_disable_flow_ex( 
       &(bearer_context_p->ds_bearer_context_dyn_p->phys_link), flow_mask );
  }
  else
  {
    ps_phys_link_disable_flow(
        &(bearer_context_p->ds_bearer_context_dyn_p->phys_link), flow_mask );
  }
#endif /*FEATURE_DATA_FLOW_MGMT */

func_exit:
  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "ds_3gpp_bearer_cntxt_disable_flow failed, local err: %d",
                        local_err);
  }
} /* ds_3gpp_bearer_cntxt_disable_flow */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_BEARER_CNTXT_FROM_IFACE_PTR

DESCRIPTION
  This function returns the bearer context pointer associated with
  the iface pointer.
  
PARAMETERS 
  iface_ptr : iface pointer for which we return the bearer_cntxt pointer

DEPENDENCIES
  None.

RETURN VALUE
  bearer context pointer if present
  NUll otherwise

SIDE EFFECTS
  None.
===========================================================================*/
ds_bearer_context_s *ds_3gpp_bearer_cntxt_get_bearer_cntxt_from_iface_ptr
(
  ps_iface_type             * iface_ptr
)
{
  ds_3gppi_local_err_val_e_type         local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  uint8                                 bearer_index;
  ds_pdn_context_s                     *pdn_context_ptr = NULL;
  ds_bearer_context_s                  *bearer_context_p = NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (iface_ptr == NULL)
  {
    local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
    goto func_exit;
  }
  /*------------------------------------------------------------------- 
    Get pdn context pointer from the iface ptr passed in.
    ------------------------------------------------------------------*/
  pdn_context_ptr = DSPDNCNTX_GET_PDN_FROM_IFACE(iface_ptr);

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == TRUE)
  {
    /*-----------------------------------------------------------------------
      Loop over all MAX applicable bearers for this PDN cntxt, find the valid
      ones
      -----------------------------------------------------------------------*/
    for(bearer_index = 0;
        bearer_index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;
        bearer_index++)
    {
      /*------------------------------------------------------------------------ 
        fetch the bearer cntxt pointer for a particular bearer from the pdn
        context pointer
        ------------------------------------------------------------------------*/
      bearer_context_p =\
       (ds_bearer_context_s*)pdn_context_ptr->ds_pdn_context_dyn_p->bearer_context_ptr_tbl[bearer_index];
      
      if(!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
      {
       /*----------------------------------------------------------------------- 
        This entry in the bearer context table is NULL, meaning there is no bearer
        corresponding to the bearer index for this pdn context. Other entries could
        have non-NULL values.
        -----------------------------------------------------------------------*/
        continue;
      }
      
      break;
    }/*for loop end*/
  }/*if end*/

func_exit:
  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1 ("ds_3gpp_bearer_cntxt_get_bearer_cntxt_from_iface_ptr"
                        " failed, local err: %d", local_err);
  }

  return bearer_context_p;
} /* ds_3gpp_bearer_cntxt_get_bearer_cntxt_from_iface_ptr */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_INSTANCE_PTR

DESCRIPTION
  This function fetches the Bearer Context Pointer corresponding to the
  given Bearer Instance.
 
PARAMETERS
  Bearer Instance

DEPENDENCIES
  None.

RETURN VALUE
  Bearer Context Pointer.

SIDE EFFECTS
  None.
===========================================================================*/
ds_bearer_context_s* ds_bearer_cntxt_get_instance_ptr
(
  uint8 instance
)
{
  return ((instance < DS_3GPP_MAX_BEARER_CONTEXT) ? 
            &(ds_bearer_cntxt_info.bearer_context_tbl[instance]): NULL);
} /* ds_bearer_cntxt_get_instance_ptr */

/*===========================================================================
FUNCTION ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag

DESCRIPTION
  This function gets the Mem Ctrl Event posted Flag
 
PARAMETERS
  None
    
DEPENDENCIES
  None

RETURN VALUE
  TRUE - If Mem Cntrl Event was already posted
  FALSE- Otherwise

SIDE EFFECTS
  None

=============================================================================*/
static boolean ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag
(
  void
)
{
  return ds_bearer_cntxt_info.mem_ctrl_ev_posted;
} /* ds_bearer_cntxt_get_mem_ctrl_ev_posted_flag */

/*===========================================================================
FUNCTION ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag

DESCRIPTION
  This function sets the Mem Ctrl Event posted Flag
 
PARAMETERS
  flag: Mem Cntrl Event posted Flag
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag
(
  boolean flag
)
{
  ds_bearer_cntxt_info.mem_ctrl_ev_posted = flag;
} /* ds_bearer_cntxt_set_mem_ctrl_ev_posted_flag */

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_is_modem_fd_enabled

DESCRIPTION
  This function returns if modem fast dormancy is enabled in EFS or not

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_modem_fd_enabled()
{
  /* ----------------------------------------------------- */
  return (ds_3gpp_fd_info.is_FD_enabled);
}

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_set_modem_fd_enabled_flag

DESCRIPTION
  This function sets modem fast dormancy is enabled flag

PARAMETERS
boolean value to enable/disable
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_bearer_cntxt_set_modem_fd_enabled_flag
(
  boolean enabled_flag
)
{
  /* ----------------------------------------------------- */
  ds_3gpp_fd_info.is_FD_enabled = enabled_flag;
}

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_set_modem_fd_timer_1_value

DESCRIPTION
  This function sets modem fast dormancy timer value

PARAMETERS
value 
    
DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_bearer_cntxt_set_modem_fd_timer_1_value
(
  uint32 timer_value
)
{
  ds_3gpp_fd_info.timer_1_value = timer_value;
}

/*===========================================================================
FUNCTION ds_3gpp_bearer_cntxt_set_modem_fd_timer_2_value

DESCRIPTION
  This function sets modem fast dormancy timer value

PARAMETERS
value 
    
DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_bearer_cntxt_set_modem_fd_timer_2_value
(
  uint32 timer_value
)
{
    ds_3gpp_fd_info.timer_2_value = timer_value;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_OK_TO_RELEASE_CONNECTION

DESCRIPTION
  This function checks if it is OK to Relese signalling connection with NW 

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_ok_to_release_connection
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_bearer_context_s*   bearer_cntxt_p = NULL;
  uint8                  index;
  uint8                  retVal = TRUE;
  dsm_watermark_type    *tx_wm = NULL;
  sys_modem_as_id_e_type  bearer_subs_id = SYS_MODEM_AS_ID_NONE;
  /* ----------------------------------------------------------------- */
  for(index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT;index++)
  {
    bearer_cntxt_p = ds_bearer_cntxt_get_instance_ptr(index);
    if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_cntxt_p, &bearer_subs_id) ||
        bearer_subs_id != subs_id )
    {
      continue;
    }
    tx_wm = &(bearer_cntxt_p->tx_wm.wm_item);

    /* Ccheck for bearer states and WN Counts for Pending data */
    if((bearer_cntxt_p->ds_bearer_context_dyn_p->state == 
                                       DS_BEARER_CONTEXT_STATE_COMING_UP)||
      (bearer_cntxt_p->ds_bearer_context_dyn_p->state == 
                                      DS_BEARER_CONTEXT_STATE_GOING_DOWN)||
      (dsm_queue_cnt(tx_wm) != 0) || 
      ((bearer_cntxt_p->ds_bearer_context_dyn_p->state == 
                                       DS_BEARER_CONTEXT_STATE_UP) && 
      (PS_PHYS_LINK_GET_STATE(&(bearer_cntxt_p->ds_bearer_context_dyn_p
                                         ->phys_link)) == PHYS_LINK_DOWN)))
    {
      DS_3GPP_MSG3_HIGH_EX(subs_id, "Inst: %d, bearer_state: %d, WM_Cnt: %d, PCH_State: TRUE",
                        index,
                        bearer_cntxt_p->ds_bearer_context_dyn_p->state,
                        dsm_queue_cnt(tx_wm));
      retVal = FALSE;
      break;
    }
  }
  return retVal; 
}/* ds_3gpp_bearer_cntxt_ok_to_release_connection*/

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_IS_ANY_BEARER_UP

DESCRIPTION
  This function checks if any bearer is in UP state or not

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_any_bearer_up(sys_modem_as_id_e_type subs_id)
{
  ds_bearer_context_s* bearer_cntxt_p = NULL;
  uint8                index;
  boolean              ret_val = FALSE; 
  sys_modem_as_id_e_type  bearer_subs_id = SYS_MODEM_AS_ID_NONE;
  /* ----------------------------------------------------------------------*/
   for(index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT;index++)
  {
    bearer_cntxt_p = ds_bearer_cntxt_get_instance_ptr(index);
 
    if(ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_cntxt_p, &bearer_subs_id)&&
       bearer_subs_id == subs_id &&
       (bearer_cntxt_p->ds_bearer_context_dyn_p->state == 
                                 DS_BEARER_CONTEXT_STATE_UP))
    {
      DS_3GPP_MSG1_MED_EX(subs_id, " Bearer Context is UP, Instance: %d",index);
      ret_val = TRUE;
      break;
    }
  }
  return ret_val;
}/* ds_3gpp_bearer_cntxt_is_any_bearer_up */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_IS_ANY_PDN_CLEAN_UP_IN_PROGRESS

DESCRIPTION
  This function checks if any PDN clean up is in progress

PARAMETERS
None
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- if YES
  FALSE- if NO

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_is_any_pdn_clean_up_in_progress
(
  sys_modem_as_id_e_type subs_id
)
{
  boolean                  ret_val = FALSE;
  uint8                    bearer_index = 0;
  ds_bearer_context_s     *bearer_context_p= NULL;
  ds_pdn_context_s        *pdn_cntx_p = NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( bearer_index = 0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(bearer_context_p,
                                                              subs_id))
    {
      continue;
    }

    DS_3GPP_MSG2_HIGH_EX(subs_id, "Bearer index:%d is in state:%d",bearer_index,
                       bearer_context_p->ds_bearer_context_dyn_p->state);

    if(bearer_context_p->ds_bearer_context_dyn_p->state == 
       DS_BEARER_CONTEXT_STATE_DOWN)   
    {
       pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
       if (ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_cntx_p))
       {
         if(pdn_cntx_p->ds_pdn_context_dyn_p->state != DS_PDN_CONTEXT_STATE_DOWN)
         {
           DS_3GPP_MSG1_HIGH_EX(subs_id, "Pdn :%d is in state:%d",
             pdn_cntx_p->ds_pdn_context_dyn_p->state);
           ret_val = TRUE;  
           break;
         }
       }
    }
    if((bearer_context_p->ds_bearer_context_dyn_p->state == 
         DS_BEARER_CONTEXT_STATE_GOING_DOWN)||
       ((bearer_context_p->ds_bearer_context_dyn_p->state == 
           DS_BEARER_CONTEXT_STATE_PEND_REDIAL)&&
        (bearer_context_p->ds_bearer_context_dyn_p->cm_call_id != CM_CALL_ID_INVALID)))
    {
       DS_3GPP_MSG1_HIGH_EX(subs_id, "default bearer:0x%p is in pending redial and call end not"
                         " yet received ", bearer_context_p);
       ret_val = TRUE;  
       break;
    }
  }

  return ret_val;
}

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_HANDLE_CONNECTION_RELEASE_REQUEST

DESCRIPTION
  This function handles connection release SYS IOCTL from PS and sends
  Signalling Connection Release Request to CM if applicable

PARAMETERS
  arg_val_ptr- carrys required information
  ps_errno- type of error
    
DEPENDENCIES
  None.

RETURN VALUE
 0- Success
 -1- Failure

SIDE EFFECTS
  None

===========================================================================*/
int ds_3gpp_bearer_cntxt_handle_connection_release_request
(
  sys_modem_as_id_e_type    subs_id,
  void                      *arg_val_ptr,
  int16                     *ps_errno
)
{
  int       ret_val = -1;
  ds_3gppi_local_err_val_e_type    local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  boolean                          req_ignored = FALSE;
  /* ------------------------------------------------------------------------ */
  do
  {
    /* check if any bearer is UP */
    if (ds_3gpp_bearer_cntxt_is_any_bearer_up(subs_id) == FALSE)
    {
      DS_3GPP_MSG0_LOW("No Bearers are UP, Ignoring Dormancy Request");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      req_ignored = TRUE;

      *ps_errno = DS_EINVAL;
      ret_val = 0; 
      break;
    }

    /* check if any bearer is COMING_UP State or has UL data pending in WM */
    if(ds_3gpp_bearer_cntxt_ok_to_release_connection(subs_id)== FALSE)
    {
      DS_3GPP_MSG0_LOW("Not OK to Reease Conection, Ignoring");
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      req_ignored = TRUE;

      *ps_errno = DS_EINVAL;
      break;
    }
 
    /* send CM command */
    if (!ds3g_msh_go_dormant(subs_id))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;

      *ps_errno =  DS_ETRYAGAIN;
      ret_val = -1; 
    } 
    else
    {
      DS_3GPP_MSG0_LOW("Connection Release Request sent to CM Successfully");
      *ps_errno =  DS_ENOERR;
      ret_val = 0; 
    }
  } while (0);

  DS_3GPP_MSG3_HIGH_EX(subs_id, "Connection Release Request sent to CM successfully: %d "
                    "err code otherwise %d and ignore %d",
                    (local_err == DS_3GPPI_LOCAL_ERR_VAL_NONE),
                    local_err, req_ignored);

  return ret_val;
}/*ds_3gpp_bearer_cntxt_handle_connection_release_request */
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_CHECK_IF_ANY_BEAR_UP_SYS_MODE

DESCRIPTION
  This function is used to look if any bearer is in UP state for the
  requested subscription

PARAMETERS
  requested_subs_id - CM Subscription ID for the request

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if any lte bearer is up , FALSE otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_check_if_any_bear_up_sys_mode
(
 sys_modem_as_id_e_type  requested_subs_id,
 sys_sys_mode_e_type     requested_sys_mode
)
{
  uint8                    index            = 0;
  ds_bearer_context_s     *bearer_context_p = NULL;
  sys_modem_as_id_e_type   subs_id          =  SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(requested_subs_id))
  {
    return FALSE;
  }

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    subs_id = SYS_MODEM_AS_ID_NONE;
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if (ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p,&subs_id) &&
        (requested_subs_id == subs_id)  &&
        (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP) &&
        (bearer_context_p->ds_bearer_context_dyn_p->call_mode == requested_sys_mode)) 
    {
      DS_3GPP_MSG1_LOW("Atleast one bearer is up n the sys mode %d",
                       requested_sys_mode);
      return TRUE;
    }
  }
  DS_3GPP_MSG1_LOW("No bearer is up n the sys mode %d",requested_sys_mode);
  return FALSE;

} 


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_CHECK_IF_ANY_BEAR_DORMANT_SYS_MODE

DESCRIPTION
  This function is used to look if any bearer is in DORMANT/RE-ESTAB state for the
  requested subscription

PARAMETERS
  requested_subs_id - CM Subscription ID for the request

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if any lte bearer is up , FALSE otherwise
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_check_if_any_bear_dormant_sys_mode
(
 sys_modem_as_id_e_type  requested_subs_id,
 sys_sys_mode_e_type     requested_sys_mode
)
{
  uint8                    index            = 0;
  ds_bearer_context_s     *bearer_context_p = NULL;
  sys_modem_as_id_e_type   subs_id          =  SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds3gsubsmgr_is_subs_id_valid(requested_subs_id))
  {
    return FALSE;
  }

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    subs_id = SYS_MODEM_AS_ID_NONE;
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    if (ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p,&subs_id) &&
        (requested_subs_id == subs_id)  &&
        ((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT) ||
         (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)) &&
        (bearer_context_p->ds_bearer_context_dyn_p->call_mode == requested_sys_mode))
    {
      DS_3GPP_MSG1_LOW("Atleast one bearer is dormant/reestab in the sys mode %d",
                       requested_sys_mode);
      return TRUE;
    }
  }
  DS_3GPP_MSG1_LOW("No bearer is dormant/reestab in the sys mode %d",
                   requested_sys_mode);
  return FALSE;

} 

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_BEARER_FROM_PS_HANDLE

DESCRIPTION
  This function finds the bearer associated with the passed ps handle. The
  ps handle is assigned to the bearer during allocation.

PARAMETERS
  ps_handle - a handle that is assigned to a bearer
    
DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_context_s* bearer_context_p - ptr to the bearer. NULL if not
  found.

SIDE EFFECTS
  None

===========================================================================*/
ds_bearer_context_s* ds_3gpp_bearer_cntxt_get_bearer_from_ps_handle
(
  int32 ps_handle
)
{
  int16                   bearer_index = 0xFF;
  ds_bearer_context_s    *bearer_context_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  bearer_index = ps_handle_mgr_get_index(PS_HANDLE_MGR_CLIENT_3GPP,
                                         ps_handle);

  if (bearer_index < 0 || bearer_index >= DS_3GPP_MAX_BEARER_CONTEXT)
  {
    DS_3GPP_ERROR_LOG_1("Invalid bearer index d returned from ps handle mgr",
                       bearer_index);
    return bearer_context_p;
  }

  bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

  return bearer_context_p;
} /* ds_3gpp_bearer_cntxt_get_bearer_from_ps_handle */

#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_EPS_ID_FROM_PHYSLINK

DESCRIPTION
  This function gets the LTE EPS beater ID of the bearer corresponding to the 
   physlink information passed

PARAMETERS
 eps_id_p - pointer for eps_id info
 phys_link_ptr - physlink pointer for whuch corresponding eps_id is required
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_get_subs_id_eps_id_from_physlink
(
  uint8*                  eps_id_p,
  sys_modem_as_id_e_type* subs_id,
  ps_phys_link_type*      phys_link_ptr
)
{
  uint8                 bearer_cntx_inst;
  ds_bearer_context_s   *bearer_context_p = NULL;
  ds_eps_bearer_context_s * eps_bearer_context = NULL;
  boolean                entry_found =  FALSE;
  
  if((NULL == eps_id_p)||(NULL == subs_id)||(NULL == phys_link_ptr))
  {
    return FALSE;
  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 /* check if there is an entry for physlink ptr passed and get correcponding 
    bearer contect ptr  */
  for(bearer_cntx_inst = 0;
      bearer_cntx_inst < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_inst++) 
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);

    if (ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p) &&
        (&(bearer_context_p->ds_bearer_context_dyn_p->phys_link) == phys_link_ptr))
    {
      entry_found = TRUE;
      //ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p,&subs_id );
      break;
    }
  }
  /* If physlink entry is found, check if curreent serving system is LTE or not.
        If LTE, then get the EPS_ID from eps bearer context ptr info.
        If not, ignore and return FALSE   */

  if (entry_found == TRUE && 
      (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_LTE)) /*TODO: call_mode or network mode which is better??? */
  {
    eps_bearer_context = (ds_eps_bearer_context_s *)
      DS_BEARER_CTXT_VOBJ(bearer_context_p, SYS_SYS_MODE_LTE);

    if(!ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_context))
    {
      return FALSE;
    }

    *eps_id_p =  eps_bearer_context->ds_eps_bearer_context_dyn_p->eps_bearer_id;

    if (!ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_context_p, subs_id))
    {
      return FALSE;
    }
    DS_3GPP_MSG2_HIGH_EX(*subs_id, "ds_3gpp_bearer_cntxt_get_subs_id_eps_id_from_physlink(): subs_id %deps_id: %d",
                         *subs_id, *eps_id_p);
    return TRUE;
  }
  return FALSE;
}/* ds_3gpp_bearer_cntxt_get_subs_id_eps_id_from_physlink */

/*===========================================================================
FUNCTION DS_3GPP_DPM_BEARER_ERR_CB

DESCRIPTION
  This is a callback function called from PS Task when malformed 
  number of packets on particular bearer reaches threshhold.

PARAMETERS
  phys_link_ptr - physical_link_ptr
  prev_resync_passed -  previous resync flag value passed
  
DEPENDENCIES
  None.

RETURN VALUE
  NoneE.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_bearer_cntxt_dpm_bearer_err_cb
(
  ps_phys_link_type* phys_link_ptr,
  boolean            prev_resync_passed
)
{ 
  ds_3gpp_bearer_err_info_type* bearer_err_info_p = NULL;
  uint8                         eps_id = 0;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  /*-------------------------------------------------------- */
  DS_3GPP_MSG2_HIGH("ds_3gpp_bearer_cntxt_dpm_bearer_err_cb(): Physlink_p: 0x%x,"
                    "prev_sync_flag: %d", phys_link_ptr, prev_resync_passed);
  
  bearer_err_info_p = ds_3gpp_get_bearer_err_info_ptr();
  
  if ((bearer_err_info_p != NULL) && (bearer_err_info_p->pdpc_cb_fptr != NULL))
  {
    if( ds_3gpp_bearer_cntxt_get_subs_id_eps_id_from_physlink(&eps_id,&subs_id, phys_link_ptr))
    {
     (void) bearer_err_info_p->pdpc_cb_fptr(eps_id, 
                                            prev_resync_passed,
                                            (uint8)subs_id);
    }
    else
    {
      DS_3GPP_ERROR_LOG_0("ds_3gpp_bearer_cntxt_dpm_bearer_err_cb():couldn't get EPS_ID");
    }
  }
}/*  ds_3gpp_bearer_cntxt_dpm_bearer_err_cb */

#endif /* FEATURE_DATA_LTE */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_UL_PACKET_METRICS_PER_RAT

DESCRIPTION
  This function gets the UL packet metrics for a given bearer, calls the
  appropriate RAT function depending on bearer call mode

PARAMETERS
  bearer_context_ptr - Bearer context 
  ul_packet_metrics_ptr - carrys required information
    
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_bearer_cntxt_get_ul_packet_metrics_per_rat
(
  ds_bearer_context_s                         *bearer_context_ptr,
  ps_iface_ioctl_ul_packet_delay_metrics_type *ul_packet_metrics_ptr
)
{
  boolean                        ret_val = FALSE;
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id) && 
     ul_packet_metrics_ptr != NULL)
  {
    DS_3GPP_MSG1_MED_EX(subs_id, "Bearer context for which metrics are queried: 0x%x", 
                     bearer_context_ptr);

    DS_BEARER_CTXT_VFR_CALL_PER_BEARER
    (
      ret_val,
      get_ul_packet_metrics,
      bearer_context_ptr,
      &(ul_packet_metrics_ptr->bearer_ul_packet_delay_metrics_arr
        [ul_packet_metrics_ptr->num_bearers])
    );

    if(ret_val == TRUE)
    {
      (ul_packet_metrics_ptr->num_bearers)++;
    }
  }
}

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_RESET_UL_PACKET_METRICS_PER_RAT

DESCRIPTION
  This function resets the UL packet metrics for a given bearer, calls the
  appropriate RAT function depending on bearer call mode

PARAMETERS
  bearer_context_ptr - Bearer context 
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: On successful metric reset communication to lower layers
  FALSE: Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntxt_reset_ul_packet_metrics_per_rat
(
  ds_bearer_context_s                       *bearer_context_ptr
)
{
  boolean                        ret_val = FALSE;
  sys_modem_as_id_e_type         subs_id = SYS_MODEM_AS_ID_NONE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id))
  {

    DS_3GPP_MSG1_MED_EX(subs_id, "Bearer context for which metrics are reset: 0x%x", 
                     bearer_context_ptr);

    DS_BEARER_CTXT_VFR_CALL_NO_ARG_PER_BEARER
    (
      ret_val,
      reset_ul_packet_metrics,
      bearer_context_ptr
    );
  }

  return ret_val;
}

#ifdef FEATURE_DATA_WCDMA_PS
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CONFIG_WCDMA_UL_DATA_RATE

DESCRIPTION
  This function configures the WCDMA UL data rate.

PARAMETERS
  command pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_config_wcdma_ul_data_rate
(
  uint32  rb_id,
  uint8   rlc_ul_id,
  uint32  rab_ul_data_rate
)
{
  int                                   index = 0;
  ds_bearer_context_s                  *bearer_context_p = NULL;
  ds_umts_bearer_context_s             *umts_bearer_context_p = NULL;
  int8                                  bearer_u_lc_id = 0;
  ds_bearer_context_wm_s               *tx_wm_p = NULL;
  ds_wcdma_bearer_cntxt_wm_levels_type  wm_levels;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG3_HIGH("ds_bearer_cntxt_config_wcdma_ul_data_rate "
                    "rb_id:%d rlc_ul_id:%d rab_ul_data_rate:%d",
                    rb_id,  rlc_ul_id, rab_ul_data_rate);

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    /* Validate bearer context ptr and bearer context dynamic ptr */
    if( (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p)) ||
        (bearer_context_p->ds_bearer_context_dyn_p == NULL) )
    {
      continue;
    }

    /* Check if bearer call mode is WCDMA */
    if( bearer_context_p->ds_bearer_context_dyn_p->call_mode !=
        SYS_SYS_MODE_WCDMA )
    {
      continue;
    }

    /* Get UMTS bearer context pointer */
    umts_bearer_context_p = bearer_context_p->ds_bearer_context_dyn_p->\
                              vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ];

    /* Validate UMTS bearer context ptr and UMTS bearer context dynamic ptr */
    if( (umts_bearer_context_p == NULL) ||
        (umts_bearer_context_p->ds_umts_bearer_context_dyn_p == NULL) )
    {
      continue;
    }

    /* Get UMTS bearer Uplink logical channel ID */
    bearer_u_lc_id = umts_bearer_context_p->\
                       ds_umts_bearer_context_dyn_p->u_lc_id;

    /* Check if the Uplink logical id matches */
    if( bearer_u_lc_id == rlc_ul_id )
    {
      DS_3GPP_MSG3_HIGH("Config UL WM levels bearer_context_p:0x%x "
                        "bearer_u_lc_id:%d ul_data_rate:%d",
                        bearer_context_p,
                        bearer_u_lc_id,
                        rab_ul_data_rate);

      tx_wm_p = &(bearer_context_p->tx_wm);

      /*---------------------------------------------------------------------
        Get watermark levels based on ul data rate
      ---------------------------------------------------------------------*/
      ds_wcdma_bearer_cntxt_get_ul_wm_levels_per_ul_rate(rab_ul_data_rate,
                                                         &wm_levels);

      /*---------------------------------------------------------------------
        Update WM levels
          - LO WM: Always update to new value.
          - HI WM: Always update to new value.
                  If back pressure is disabled, stop BP timer.
          - DNE: Use the highest value between old and new DNE values.
      ---------------------------------------------------------------------*/
      if (tx_wm_p->wm_item.lo_watermark !=  wm_levels.lo_wm_level)
      {
      dsm_set_low_wm(&tx_wm_p->wm_item, wm_levels.lo_wm_level);
      }

      if (tx_wm_p->wm_item.hi_watermark !=  wm_levels.hi_wm_level)
      {
      dsm_set_hi_wm(&tx_wm_p->wm_item, wm_levels.hi_wm_level);
      }

      if (tx_wm_p->wm_item.current_cnt <  tx_wm_p->wm_item.lo_watermark) 
      {
        dsm_set_dne(&tx_wm_p->wm_item, wm_levels.dne_wm_level);
      }
      else 
      {
        if( wm_levels.dne_wm_level > tx_wm_p->wm_item.dont_exceed_cnt )
        {
          dsm_set_dne(&tx_wm_p->wm_item, wm_levels.dne_wm_level);
        }
      }

      DS_3GPP_MSG3_HIGH("WCDMA UL WM levels after update - "
                        "DNE: %d, Hi: %d, LO: %d",
                        tx_wm_p->wm_item.dont_exceed_cnt,
                        tx_wm_p->wm_item.hi_watermark,
                        tx_wm_p->wm_item.lo_watermark);
    }
  }
  return;
} /* ds_bearer_cntxt_config_wcdma_ul_data_rate */
#endif /* FEATURE_DATA_WCDMA_PS */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CONFIG_LTE_UL_WM

DESCRIPTION
  This function configures the LTE UL Watermark for all the active bearers 
  based upon the uplink data rate.

PARAMETERS
  command pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_config_lte_ul_wm
(
  uint32                      ul_data_rate,
  sys_modem_as_id_e_type      subs_id
)
{
  int                                   index = 0;
  ds_bearer_context_s                  *bearer_context_p = NULL;
  ds_eps_bearer_context_s              *eps_bearer_context_p = NULL;
  ds_bearer_context_wm_s               *tx_wm_p = NULL;
  ds_eps_bearer_cntxt_wm_levels_type    wm_levels;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;  

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*---------------------------------------------------------------------
   Get watermark levels based on ul data rate
  ---------------------------------------------------------------------*/

  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);

  if (um_wm_threshold.lte_wm_levels_valid == TRUE)
  {
    return;
  }

  ds_eps_bearer_cntxt_get_ul_wm_levels_per_ul_rate(ul_data_rate,
                                                     &wm_levels);

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    /* Validate bearer context ptr and bearer context dynamic ptr */
    if( (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p)) ||
        (bearer_context_p->ds_bearer_context_dyn_p == NULL) )
    {
      continue;
    }

    /* Check if bearer call mode is WCDMA */
    if( bearer_context_p->ds_bearer_context_dyn_p->call_mode !=
        SYS_SYS_MODE_LTE )
    {
      continue;
    }

    if(bearer_context_p->ds_bearer_context_dyn_p->subs_id != subs_id)
    {
      continue;
    }

    /* Get EPS bearer context pointer */
    eps_bearer_context_p = bearer_context_p->ds_bearer_context_dyn_p->\
                              vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ];

    /* Validate EPS bearer context ptr and EPS bearer context dynamic ptr */
    if( (eps_bearer_context_p == NULL) ||
        (eps_bearer_context_p->ds_eps_bearer_context_dyn_p == NULL) )
    {
      continue;
    }

    tx_wm_p = &(bearer_context_p->tx_wm);

    /*---------------------------------------------------------------------
        Update WM levels
          - LO WM: Always update to new value.
          - HI WM: Always update to new value.
                  If back pressure is disabled, stop BP timer.
          - DNE: Use the highest value between old and new DNE values.
      ---------------------------------------------------------------------*/
    /*use flag to print the debug messages*/
    if (tx_wm_p->wm_item.lo_watermark !=  wm_levels.lo_wm_level)
    {
      dsm_set_low_wm(&tx_wm_p->wm_item, wm_levels.lo_wm_level);
    }

    if(tx_wm_p->wm_item.hiwater_func_ptr == NULL)
    {
      ds_bearer_cntxt_stop_bp_timer(bearer_context_p);
    }

    if (tx_wm_p->wm_item.hi_watermark !=  wm_levels.hi_wm_level)
    {
      dsm_set_hi_wm(&tx_wm_p->wm_item, wm_levels.hi_wm_level);
    }

    if(( wm_levels.dne_wm_level > tx_wm_p->wm_item.dont_exceed_cnt) || 
       (tx_wm_p->wm_item.dont_exceed_cnt == -1))
    {
      dsm_set_dne(&tx_wm_p->wm_item, wm_levels.dne_wm_level);
      dsm_set_dne_q_cnt(&tx_wm_p->wm_item, wm_levels.dne_q_cnt);
    }

    DS_3GPP_MSG3_HIGH_EX(subs_id, "LTE UL WM levels after update - "
                        "DNE: %d, Hi: %d, LO: %d",
                       tx_wm_p->wm_item.dont_exceed_cnt,
                       tx_wm_p->wm_item.hi_watermark,
                       tx_wm_p->wm_item.lo_watermark);
  }
} /* ds_bearer_cntxt_config_lte_ul_data_rate */
/*===========================================================================
FUNCTION DS_BEARER_CNTXT_CONFIG_LTE_DL_DATA_RATE

DESCRIPTION
  This function configures the LTE DL Watermark for all the active bearers 
  based upon the downlink data rate.

PARAMETERS
  command pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_config_lte_dl_wm
(
  uint32                      dl_data_rate,
  sys_modem_as_id_e_type      subs_id
)
{
  int                                   index = 0;
  ds_bearer_context_s                  *bearer_context_p = NULL;
  ds_eps_bearer_context_s              *eps_bearer_context_p = NULL;
  ds_bearer_context_wm_s               *rx_wm_p = NULL;
  ds_eps_bearer_cntxt_wm_levels_type    wm_levels;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;  

  /*---------------------------------------------------------------------
   Get watermark levels based on dl data rate
  ---------------------------------------------------------------------*/
  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);

  if (um_wm_threshold.lte_wm_levels_valid == TRUE)
  {
    return;
  }

  ds_eps_bearer_cntxt_get_dl_wm_levels_per_dl_rate(dl_data_rate,
                                                     &wm_levels);

  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);

    /* Validate bearer context ptr and bearer context dynamic ptr */
    if( (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p)) ||
        (bearer_context_p->ds_bearer_context_dyn_p == NULL) )
    {
      continue;
    }

    /* Check if bearer call mode is WCDMA */
    if( bearer_context_p->ds_bearer_context_dyn_p->call_mode !=
        SYS_SYS_MODE_LTE )
    {
      continue;
    }

    if(bearer_context_p->ds_bearer_context_dyn_p->subs_id != subs_id)
    {
      continue;
    }

    /* Get EPS bearer context pointer */
    eps_bearer_context_p = bearer_context_p->ds_bearer_context_dyn_p->\
                              vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ];

    /* Validate UMTS bearer context ptr and UMTS bearer context dynamic ptr */
    if( (eps_bearer_context_p == NULL) ||
        (eps_bearer_context_p->ds_eps_bearer_context_dyn_p == NULL) )
    {
      continue;
    }


    rx_wm_p = &(bearer_context_p->lte_rx_wm);

    /*---------------------------------------------------------------------
        Update WM levels
          - LO WM: Always update to new value.
          - HI WM: Always update to new value..
          - DNE: Use the highest value between old and new DNE values.
      ---------------------------------------------------------------------*/
    if (rx_wm_p->wm_item.lo_watermark !=  wm_levels.lo_wm_level)
    {
      dsm_set_low_wm(&rx_wm_p->wm_item, wm_levels.lo_wm_level);
    }
    if (rx_wm_p->wm_item.hi_watermark !=  wm_levels.hi_wm_level)
    {
      dsm_set_hi_wm(&rx_wm_p->wm_item, wm_levels.hi_wm_level);
    }
    if(( wm_levels.dne_wm_level > rx_wm_p->wm_item.dont_exceed_cnt ) || 
       (rx_wm_p->wm_item.dont_exceed_cnt == -1))
    {
      dsm_set_dne(&rx_wm_p->wm_item, wm_levels.dne_wm_level);
      dsm_set_dne_q_cnt(&rx_wm_p->wm_item, wm_levels.dne_q_cnt);
    }

    DS_3GPP_MSG3_HIGH_EX(subs_id, "LTE DL WM levels after update - "
                        "DNE: %d, Hi: %d, LO: %d",
                       rx_wm_p->wm_item.dont_exceed_cnt,
                       rx_wm_p->wm_item.hi_watermark,
                       rx_wm_p->wm_item.lo_watermark);
  }
  return;
} /* ds_bearer_cntxt_config_lte_dl_data_rate */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_IS_DEF_BEARER_COMING_UP

DESCRIPTION
  This function checks if the default bearer corresponding to the
  given PDN context is in COMING UP state.

PARAMETERS
  pdn_cntx_p - PDN Context Ptr

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if Bearer in COMING state
  FALSE, otherwise

SIDE EFFECTS 
  None.

===========================================================================*/
boolean ds_bearer_cntxt_is_def_bearer_coming_up
(
  ds_pdn_context_s  *pdn_cntx_p
)
{
  ds_bearer_context_s    *def_bearer_cntxt_ptr = NULL;
  boolean                 ret_val = FALSE;
  sys_modem_as_id_e_type  subs_id = SYS_MODEM_AS_ID_NONE;
  /*---------------------------------------------------------*/

  do
  {
    if (!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p, &subs_id))
    {
      break;
    }

    def_bearer_cntxt_ptr =
       (ds_bearer_context_s *)pdn_cntx_p->ds_pdn_context_dyn_p->def_bearer_context_ptr;

    if (!ds_bearer_cntxt_validate_bearer_subscription(def_bearer_cntxt_ptr,
                                                       subs_id))
    {
      break;
    } 

    if(def_bearer_cntxt_ptr->ds_bearer_context_dyn_p->state != 
         DS_BEARER_CONTEXT_STATE_COMING_UP)
    {
      break;
    }

    ret_val = TRUE;
  } while (0); 

  return ret_val;
} /* ds_bearer_cntxt_is_def_bearer_coming_up */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_GET_DNE_WM_LEVEL

DESCRIPTION
  This function retrieves the RAT specific DNE watermark

PARAMETERS
  bearer_cntxt_p
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  DNE watermark level - uint32.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_bearer_cntxt_get_dne_wm_level
(
  ds_bearer_context_s     *bearer_cntxt_p,
  sys_modem_as_id_e_type   subs_id
)
{
  
  uint32                                            wm_dne_level = 0;
  ds_bearer_cntxt_um_wm_threshold_struct_type       um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type   tds_um_wm_threshold;
  uint64                                            call_mode;
  
  if( !ds_bearer_cntx_validate_bearer_context_internal(bearer_cntxt_p) ||
      !ds3gsubsmgr_is_subs_id_valid(subs_id) )
  {
    return wm_dne_level;
  }
  
  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);
  tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);
  call_mode = bearer_cntxt_p->ds_bearer_context_dyn_p->call_mode;

  /*------------------------------------------------------------------------
    Reset DNE_WM to original value
  ------------------------------------------------------------------------*/
  if (call_mode == SYS_SYS_MODE_LTE)
  {
    wm_dne_level = ds_bearer_cntx_get_lte_ul_wm_level(bearer_cntxt_p,
                                                     &um_wm_threshold,
                                                     UL_WM_DNE_INDEX,
                                                     subs_id);
  }
  else if (call_mode == SYS_SYS_MODE_WCDMA)
  {
    wm_dne_level = ds_bearer_cntx_get_wcdma_ul_wm_level(bearer_cntxt_p,
                                                       &um_wm_threshold,
                                                       UL_WM_DNE_INDEX);
  }
  else if (call_mode == SYS_SYS_MODE_GSM)
  {
    wm_dne_level = um_wm_threshold.gprs_ul_dne;
  }
#ifdef FEATURE_TDSCDMA
  else if (call_mode == SYS_SYS_MODE_TDS)
  {
    wm_dne_level = ds_bearer_cntx_get_tdscdma_ul_wm_level(bearer_cntxt_p,
                                                         &tds_um_wm_threshold,
                                                         UL_WM_DNE_INDEX);
  }
#endif /* FEATURE_TDSCDMA */

  return wm_dne_level;
}

#ifdef FEATURE_DATA_RAVE_SUPPORT 
/*===========================================================================

FUNCTION:  DS_BEARER_CNTXT_GET_TX_WM_COUNT

DESCRIPTION
  This function will get the uplink wm count of all the bearers for the
  subscription id specified

PARAMETERS
  subs_id - Requested Subscription

DEPENDENCIES
  None.

RETURN VALUE
  sum of all uplink wm in kbps

SIDE EFFECTS
  None.  
===========================================================================*/
uint32 ds_bearer_cntxt_get_tx_wm_count
(
  sys_modem_as_id_e_type subs_id
)
{
  uint32                  current_wm_count  =0;
  uint8                   bearer_index      = 0;
  ds_bearer_context_s     *bearer_context_p = NULL;
  for( bearer_index = 0;bearer_index < DS_3GPP_MAX_BEARER_CONTEXT;
       bearer_index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_index);

    if(!ds_bearer_cntxt_validate_bearer_subscription_internal(
       bearer_context_p,subs_id))
    {
      continue;
    }

    current_wm_count += bearer_context_p->tx_wm.wm_item.current_cnt;
  }

  current_wm_count = DSUTIL_BYTES_TO_KBITS(current_wm_count);
  return current_wm_count;
}
#endif

/*=============================================================================
FUNCTION      DS_BEARER_CNTXT_DPM_EV_CB

DESCRIPTION
  This is the call back function that is called when the Dpm events are invoked.
  Based on these events; the watermark's nonemmpty function callback is set
  reset.
PARAMETERS
  IN
    dpm_event       - The event that happened. This is used to differentiate
                      the events that happened : registration, deregistration
                      reregistration
    event_info_ptr   - Passed in as void*, this gives us the event details 
            
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void ds_bearer_cntxt_dpm_ev_cb
(
   ps_dpm_um_ev_enum_type                    dpm_event, 
   void                                    *event_info_ptr
)
{
  ds_cmd_type                              *cmd_ptr = NULL;  
  ds_bearer_dpm_cb_info_s_type             *dpm_info_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_ptr = 
     ds_allocate_cmd_buf(sizeof(ds_bearer_dpm_cb_info_s_type));

  if((NULL == cmd_ptr) || (NULL == cmd_ptr->cmd_payload_ptr))
  {
    DS3GPP_ASSERT(0);
    return;
  }  

#ifdef FEATURE_LAPP_UFS 
  #error code not present
#endif /* FEATURE_LAPP_UFS */

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_DPM_INFO_CNF;

  dpm_info_ptr = (ds_bearer_dpm_cb_info_s_type *)cmd_ptr->cmd_payload_ptr;  

  memset(dpm_info_ptr, 0, sizeof(ds_bearer_dpm_cb_info_s_type));

  dpm_info_ptr->dpm_event = dpm_event;

  memscpy((void *)&(dpm_info_ptr->dpm_info), 
          sizeof(ps_dpm_um_ev_cback_info_type),
          event_info_ptr, 
          sizeof(ps_dpm_um_ev_cback_info_type));

  ds_put_cmd(cmd_ptr);

}/* ds_bearer_cntxt_dpm_ev_cb */
/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_PROC_NON_EMPTY_CB_CMD

DESCRIPTION
  This function is called when we process the non empty CB which is 
  triggered whenever a packet is enqueued in empty WM.

PARAMETERS   
  *cmd_ptr - ds cmd ptr

DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/

void ds_bearer_cntxt_proc_non_empty_cb_cmd
(
    const ds_cmd_type *cmd_ptr
)
{
  ds_bearer_context_s                *bearer_context_p = NULL;
  sys_modem_as_id_e_type              subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_context_nonempty_cmd_type  *data_ptr = NULL;
  ds_3gppi_local_err_val_e_type       local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_3GPP_ERROR_LOG_0("NULL ptr passed, return");
    return;
  }

  data_ptr = (ds_bearer_context_nonempty_cmd_type*)cmd_ptr->cmd_payload_ptr;
  bearer_context_p =(ds_bearer_context_s*)(data_ptr->bearer_context_p);

  do
  {
    if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
    {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;;
    }
  
    subs_id = bearer_context_p->ds_bearer_context_dyn_p->subs_id;
    if( bearer_context_p->ds_bearer_context_dyn_p->call_mode != SYS_SYS_MODE_GSM)
    {
      if ( bearer_context_p->ds_bearer_context_dyn_p->state !=
           DS_BEARER_CONTEXT_STATE_UP)
      {
        local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
        break;
      }
    }
  
    DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_bearer_cntxt_proc_non_empty_cb_cmd():"
                      "bearer from empty to non_empty, subs_id:%d",subs_id);
  
    bearer_context_p->tx_wm.wm_item.non_empty_cb_ptrs[0] = NULL;
    bearer_context_p->tx_wm.wm_item.non_empty_cb_data[0] = NULL;
  
    ds3geventmgr_notify_ps_call_event_per_sub(subs_id,
                                              DS3GEVENTMGR_PS_CALL_ACTIVE_EV,
                                              PS_SYS_NETWORK_3GPP);
  }while(0);

  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, 
                 "ds_bearer_cntxt_proc_non_empty_cb_cmd failed, local err: d",
                 local_err);
  }
}/* ds_bearer_cntxt_proc_non_empty_cb_cmd */

/*===========================================================================
FUNCTION      DS_3GPP_LOG_ACTIVE_BEARERS_INFO

DESCRIPTION   this function collect the active bearers info and call the 
              logging bearer state function 

DEPENDENCIES  None 
 
PARAMETERS 
  subs_id - subscription ID 

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_log_active_bearers_info
(
   sys_modem_as_id_e_type subs_id
)
{
  int                         index = 0;
  int                         s_index = 0;
  ds_bearer_context_s         *bearer_context_p = NULL;
  ds_pdn_context_s            *pdn_cntx_p = NULL;
  sys_modem_as_id_e_type      bearer_subs_id;
  uint64                      call_mode;  
  ds_eps_bearer_context_s     *eps_bearer_cntxt_p   = NULL;
  dsm_watermark_type          *wm_ptr = NULL;
  ps_iface_type               *v4_iface_ptr = NULL;
  ps_iface_type               *v6_iface_ptr = NULL;
  ds_3gpp_bearer_log_info     *bearer_info_ptr = NULL;
/* - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - */
  /* Memory Allocation for bearer_info will be freed at the end of the function*/
  bearer_info_ptr = (ds_3gpp_bearer_log_info *)modem_mem_alloc(
                    sizeof(ds_3gpp_bearer_log_info)*DS_3GPP_MAX_BEARER_CONTEXT,
                    MODEM_MEM_CLIENT_DATA);

  if ( NULL == bearer_info_ptr) 
  {
    DS_3GPP_ERROR_LOG_0("bearer_info_ptr memory allocation failed");
    return;
  }

  memset(bearer_info_ptr, 0, 
         sizeof(ds_3gpp_bearer_log_info)*DS_3GPP_MAX_BEARER_CONTEXT);

  /* Collect active bearers info*/
  for (index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(index);
    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      continue;
    }

    pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
    if (!ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_cntx_p))
    {
      continue;
    }

    bearer_subs_id = bearer_context_p->ds_bearer_context_dyn_p->subs_id;
    if ( subs_id != bearer_subs_id)
    { 
      continue;
    }

    call_mode = bearer_context_p->ds_bearer_context_dyn_p->call_mode;
    bearer_info_ptr[s_index].sub_id = bearer_subs_id;
    bearer_info_ptr[s_index].state = bearer_context_p->ds_bearer_context_dyn_p->state;
    bearer_info_ptr[s_index].cm_call_id = bearer_context_p->
                                      ds_bearer_context_dyn_p->cm_call_id;
    bearer_info_ptr[s_index].call_mode = call_mode;

    if (call_mode == SYS_SYS_MODE_LTE) 
    { 
      eps_bearer_cntxt_p = (ds_eps_bearer_context_s *)
                           (bearer_context_p->ds_bearer_context_dyn_p->
                           vobj_data_ptr_tbl[DS_3GPP_EPS_VOBJ]);

      if(ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_cntxt_p))
      {
        bearer_info_ptr[s_index].bearer_id = (uint8)eps_bearer_cntxt_p->
                                         ds_eps_bearer_context_dyn_p->
                                         eps_bearer_id;
      }
      else
      {
        bearer_info_ptr[s_index].bearer_id = 0;
      }
    }
    else
    {
      bearer_info_ptr[s_index].bearer_id = bearer_context_p->
                                           ds_bearer_context_dyn_p->nsapi;
    }

    bearer_info_ptr[s_index].pdn_context_inst = pdn_cntx_p->ds_pdn_context_dyn_p->index;
    bearer_info_ptr[s_index].is_default = bearer_context_p->
                                          ds_bearer_context_dyn_p->is_default;
    bearer_info_ptr[s_index].profile_id = pdn_cntx_p->ds_pdn_context_dyn_p->
                                          pdp_profile_num;

    if ( NULL != (v4_iface_ptr = DSPDNCNTXT_GET_V4_PS_IFACE_PTR(pdn_cntx_p))) 
    {
      bearer_info_ptr[s_index].v4_instance_id = v4_iface_ptr->instance;
    }
    else
    {
      bearer_info_ptr[s_index].v4_instance_id = 0xFF;
    }

    if ( NULL != (v6_iface_ptr = DSPDNCNTXT_GET_V6_PS_IFACE_PTR(pdn_cntx_p))) 
    {
      bearer_info_ptr[s_index].v6_instance_id = v6_iface_ptr->instance;
    }
    else
    {
      bearer_info_ptr[s_index].v6_instance_id = 0xFF;
    }

    wm_ptr = &(bearer_context_p->tx_wm.wm_item);
    bearer_info_ptr[s_index].ul_low_wm_level = wm_ptr->lo_watermark;
    bearer_info_ptr[s_index].ul_high_wm_level = wm_ptr->hi_watermark;
    bearer_info_ptr[s_index].ul_dne_wm_level = wm_ptr->dont_exceed_cnt;
    if ( bearer_info_ptr[s_index].ul_dne_wm_level == 0xFFFFFFFF) 
    {
       bearer_info_ptr[s_index].ul_dne_wm_level = 0;
    }
    bearer_info_ptr[s_index].ul_packet_drop = wm_ptr->dne_discard_pkt_cnt;

    s_index++;
  }

  ds_3gpp_logging_bearer_state(s_index, bearer_info_ptr, subs_id);

  modem_mem_free( bearer_info_ptr, MODEM_MEM_CLIENT_DATA );
}/* ds_3gpp_log_active_bearers_info */

/*===========================================================================
FUNCTION	 DS_3GPP_BEARER_CNTXT_REMOVE_BEARER_CNTXT_INFO

DESCRIPTION    
  This function takes care of cleaning up bearer related information as part
  of PDN context cleanup
 
PARAMETERS 
  bearer_context_p   -- Pointer to bearer context related information
 
DEPENDENCIES   
  NONE

RETURN VALUE   
  NONE

SIDE EFFECTS   
  NONE
===========================================================================*/
void ds_3gpp_bearer_cntxt_remove_bearer_cntxt_info
(
  ds_bearer_context_s               *bearer_context_p
)
{

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
  /*----------------------------------------------------------------------- 
    Release the bearer flow manager 
  -----------------------------------------------------------------------*/
  ds_bearer_flow_manager_release_flow_manager(bearer_context_p);
  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_flow_crit_sect);

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  ds3gpp_enter_global_crit_section();
  /*-----------------------------------------------------------------------
    Return the bearer context to the free pool
    This is the only place where the default bearer is marked as out of use
  -----------------------------------------------------------------------*/
  ds_bearer_cntxt_deregister_pdn_context(bearer_context_p);
  ds_bearer_cntxt_free_bearer_inst(bearer_context_p);

  /*-----------------------------------------------------------------------
    Return the bearer context to the free pool
    This is the only place where the default bearer is marked as out of use
  -----------------------------------------------------------------------*/
  ds_bearer_cntxt_return_bearer_to_free_pool(bearer_context_p);
  ds3gpp_leave_global_crit_section();
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

}/* ds_3gpp_bearer_cntxt_remove_bearer_cntxt_info */

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_IS_ANY_OTHER_BEARER_PER_SUB_ACTIVE

DESCRIPTION
  This function checks if any other bearer on the subscription passed is in 
  UP state 

PARAMETERS  
  subs_id - Subscription

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_bearer_cntx_is_any_bearer_per_sub_active
(
 sys_modem_as_id_e_type  subs_id
)
{
  boolean                 ret_val = FALSE;
  ds_bearer_context_s    *temp_bearer_context_p = NULL;
  uint8                   bearer_cntx_index = 0;
  sys_modem_as_id_e_type  temp_subs_id = SYS_MODEM_AS_ID_NONE;
  
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for(bearer_cntx_index = 0; bearer_cntx_index < DS_3GPP_MAX_BEARER_CONTEXT;
      bearer_cntx_index++)
  {
    temp_bearer_context_p = 
      ds_bearer_cntxt_get_instance_ptr(bearer_cntx_index);   
    /*-----------------------------------------------------------------------
     Check the temp subs_id, Perform this function check only for bearers
     of the same subscription.
    -----------------------------------------------------------------------*/
    if( !ds_bearer_cntxt_get_subs_id_from_bearer_internal(
         temp_bearer_context_p,&temp_subs_id))
    {
      continue;
    }

    if (subs_id != temp_subs_id)
    {
      DS_3GPP_MSG2_MED("Subs ID mismatch, continue "
                       "Temp Subs Id(CM):%d Subs Id(CM):%d",
                       temp_subs_id,subs_id);
      continue;
    }

    if(temp_bearer_context_p->ds_bearer_context_dyn_p->state ==
       DS_BEARER_CONTEXT_STATE_UP)
    {
      DS_3GPP_MSG1_LOW("Bearer index:%d is UP, found another active bearer",
                        bearer_cntx_index);
      ret_val = TRUE;
      break;
    }
    else
    {
      DS_3GPP_MSG1_LOW("Bearer context state is %d",
                       temp_bearer_context_p->ds_bearer_context_dyn_p->state);
    }
  }

  DS_3GPP_MSG1_LOW("ds_bearer_cntx_is_any_bearer_per_sub_active returns %d",
                    ret_val);

  return ret_val;
}


/*===========================================================================
  FUNCTION DS_3GPP_BEARER_PHYS_LINK_ENALE_FLOW_CB

  DESCRIPTION
    This function is the callback function for bearer phys link enable flow.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void ds_3gpp_bearer_cntxt_phys_link_enable_flow_cb
(
  ps_phys_link_type           *rm_phys_link_ptr,
  ps_iface_event_enum_type     event,
  ps_iface_event_info_u_type   event_info,
  void                        *user_data_ptr
)
{
  ds_bearer_context_s               *bearer_cntx_p = NULL;
  ds_eps_bearer_context_s           *eps_bearer_cntx_p = NULL;
  sys_modem_as_id_e_type             subs_id;
  ds_eps_tlb_per_subs_info_type     *eps_tlb_hndl = NULL;
  /*------------------------------------------------------------------------*/

  bearer_cntx_p = (ds_bearer_context_s *)user_data_ptr;

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_cntx_p, &subs_id))
  {
    return;
  }

  if (event == PHYS_LINK_FLOW_ENABLED_EV)
  {
    if ( bearer_cntx_p->
          ds_bearer_context_dyn_p->flow_disable_timer_running == TRUE)
    {
       DS_3GPP_MSG0_HIGH ("Clearing flow_disable_timer");
       (void)rex_clr_timer(&(bearer_cntx_p
              ->ds_bearer_context_dyn_p->flow_disable_timer));
       bearer_cntx_p->
         ds_bearer_context_dyn_p->flow_disable_timer_running =  FALSE;
    }
  }

  eps_tlb_hndl = ds_eps_tlb_get_per_subs_info_hndl(subs_id);

  if (!ds_eps_tlb_validate_per_subs_info_hndl(eps_tlb_hndl))
  {
    return;
  }

  if (event == PHYS_LINK_FLOW_ENABLED_EV)
  {
    if (bearer_cntx_p->ds_bearer_context_dyn_p->call_mode == 
         SYS_SYS_MODE_LTE)
    {
      eps_bearer_cntx_p = (ds_eps_bearer_context_s *)
        DS_BEARER_CTXT_VOBJ(bearer_cntx_p, SYS_SYS_MODE_LTE);
    }

    if (ds_eps_bearer_cntxt_validate_bearer_cntxt(eps_bearer_cntx_p))
    {
      if (eps_bearer_cntx_p->ds_eps_bearer_context_dyn_p->is_in_tlb == TRUE
        && eps_tlb_hndl->tlb_state.mode == LTE_TLB_MODE_A)
      {
        PS_SET_SIGNAL(bearer_cntx_p->ds_bearer_context_dyn_p->rx_sig);
      }

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /*FEATURE_LAPP_UFS */
    }
  }
} /* dsEpsTlb_bearer_phys_link_enable_flow_cb */


/*===========================================================================
FUNCTION  DS_3GPP_PDN_CNTX_SEND_DATA_CONNECTION_STATUS_INFO

DESCRIPTION    
  Set data connection status info and send sys event ind to PS
 
PARAMETERS 
  bearer_cntx_ptr: bearer context pointer
  pdn_cntx_ptr: pdn context pointer
  mode_info_ptr: mode info pointer
  pdn_status: pdn status
  end_status: call end status
 
DEPENDENCIES   
  NONE

RETURN VALUE   
  NONE

SIDE EFFECTS   
  NONE
===========================================================================*/
void ds_3gpp_pdn_cntx_send_data_connection_status_info
(
  ds_bearer_context_s                   *bearer_cntx_ptr,
  ds_pdn_context_s                      *pdn_cntx_ptr,
  const cm_call_mode_info_s_type        *mode_info_ptr,
  ps_sys_data_conn_pdn_status_enum_type  pdn_status,
  cm_call_end_e_type                     end_status
)
{
  ps_sys_event_3gpp_data_connection_status_info_type  
                                       *data_connection_status_info_ptr = NULL;
  int16                                 ps_errno = -1;
  ds_umts_pdp_type_enum_type            pdn_pdp_type = DS_UMTS_PDP_MAX;
  byte                                 *apn_name_ptr = NULL;
  sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_cntx_ptr) ||
     !ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_cntx_ptr, &subs_id))
  {
    return;
  }

  /*Allocate the memory */
  data_connection_status_info_ptr = 
  (ps_sys_event_3gpp_data_connection_status_info_type *)modem_mem_alloc
                     (sizeof(ps_sys_event_3gpp_data_connection_status_info_type), 
                      MODEM_MEM_CLIENT_DATA);
  if (data_connection_status_info_ptr == NULL)
  {
    return;
  }

  memset(data_connection_status_info_ptr, 
         0 ,
         sizeof(ps_sys_event_3gpp_data_connection_status_info_type));

  /**< Call ID */
  /**< PDN rejected, CM has no assigned Call ID. Fill in with 0xFF */
  if( pdn_status == PS_SYS_DATA_CONN_PDN_STATUS_REJECTED )
  {
    data_connection_status_info_ptr->call_id = 0XFF;
  }
  else
  {
    data_connection_status_info_ptr->call_id = bearer_cntx_ptr->
                                               ds_bearer_context_dyn_p->cm_call_id;
  }

  /**< RAT information */
  data_connection_status_info_ptr->rat = ds_3gpp_map_sys_mode_to_ds_sys_rat(
                                          bearer_cntx_ptr->ds_bearer_context_dyn_p->call_mode);
  

  /**< PDN Status */
  data_connection_status_info_ptr->pdn_status = pdn_status;

  /**< Cause Code */
  if (mode_info_ptr != NULL) 
  {
    /*LTE*/
    if( data_connection_status_info_ptr->rat == DS_SYS_RAT_EX_3GPP_LTE ) 
    {   
      if( TRUE == mode_info_ptr->info.lte_call.esm_cause.valid)
      {
        data_connection_status_info_ptr->cause_code = mode_info_ptr->info.
                                                    lte_call.esm_cause.esm_cause;
      }
      /*ESM cause is not valid, set cause code to 0xFF*/
      else
      {
        data_connection_status_info_ptr->cause_code = 0xFF;
      }
    }
    else
    {
      if (CM_CALL_END_NETWORK_END == end_status)
      {
        if(SM_NETWORK_CAUSE == mode_info_ptr->info.gw_ps_call.pdp_cause_type)
        {
          /* Network (external) cause */
          data_connection_status_info_ptr->cause_code = mode_info_ptr->info.
                                                      gw_ps_call.cause.ext_cause;
        }
        else
        {
          /* Internal cause, set cause code to 0xFF */
          data_connection_status_info_ptr->cause_code = 0xFF;
        }
      }
      /* Not network ends, set cause code to 0xFF */
      else
      {
        data_connection_status_info_ptr->cause_code = 0xFF;
      } 
    }
  }
  /*Cause code is not available, set to 0xFF*/
  else
  {
    data_connection_status_info_ptr->cause_code = 0xFF;
  }

  /**< APN Name */
  if((strlen((char*)pdn_cntx_ptr->ds_pdn_context_dyn_p-> 
                                  pdp_profile.context.apn) == 0)  && 
     (ds_pdn_cntxt_get_resolved_apn_name(  pdn_cntx_ptr, 
                                         &(apn_name_ptr))))
  {
    (void)strlcpy(data_connection_status_info_ptr->apn_name,
		  (char *)apn_name_ptr,
		  PS_SYS_MAX_APN_LEN);
  }
  else
  {
    (void)strlcpy(data_connection_status_info_ptr->apn_name,
                  (const char *)pdn_cntx_ptr->ds_pdn_context_dyn_p->pdp_profile.context.apn,
		  PS_SYS_MAX_APN_LEN);
  }

  /**< PDP type*/
  pdn_pdp_type = ds_pdn_cntxt_get_pdn_context_ip_support(pdn_cntx_ptr);
  data_connection_status_info_ptr->pdp_type = 
                 ds_3gpp_map_ds_umts_pdp_type_to_ps_sys_pdp_type(pdn_pdp_type);

  /* Send PS event*/
  ps_sys_event_ind_ex(PS_SYS_TECH_3GPP,
                      PS_SYS_EVENT_3GPP_DATA_CONNECTION_STATUS_CHANGE,
                      (ps_sys_subscription_enum_type)
                      ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                      (void*)data_connection_status_info_ptr,
                      &ps_errno);
  
  DS_3GPP_MSG6_LOW("Send Data Connection Status: "
                   "call_id: %d, rat: %d, pdn_status: %d,"
                   "cause_code: %d, pdp_type: %d, APN: %s",
                   data_connection_status_info_ptr->call_id,
                   data_connection_status_info_ptr->rat,
                   data_connection_status_info_ptr->pdn_status,
                   data_connection_status_info_ptr->cause_code,
                   data_connection_status_info_ptr->pdp_type,
                   (char*)data_connection_status_info_ptr->apn_name);


  /* Free the memory*/
  modem_mem_free ((void *)data_connection_status_info_ptr, MODEM_MEM_CLIENT_DATA);
}/* ds_3gpp_pdn_cntx_send_data_connection_status_info */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_EPS_ID_FROM_PHYSLINK

DESCRIPTION
  This function fetches the subs id to which the physlink belongs.

PARAMETERS
 phys_link_ptr - physlink pointer for whuch corresponding subs_id is required
 subs_id_p - pointer for the subs_id info
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_bearer_cntxt_get_subs_id_from_physlink
(
  ps_phys_link_type      *phys_link_ptr,
  sys_modem_as_id_e_type *subs_id_p
)
{
  uint8                 bearer_cntx_inst;
  ds_bearer_context_s  *bearer_context_p = NULL;
  boolean               ret_val = FALSE;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((phys_link_ptr != NULL) && (subs_id_p != NULL))
  {
    bearer_cntx_inst = (uint32)phys_link_ptr->client_data_ptr;
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_cntx_inst);
    if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, subs_id_p))
    {
      ret_val = FALSE;
    }
    else
    {
      ret_val = TRUE;
    }
  }
  return ret_val;
}/* ds_3gpp_bearer_cntxt_get_subs_id_from_physlink */

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_SET_BEARER_PDN_PEND_REDIAL_STATE

DESCRIPTION
  this function set the given bearer and PDN to pending redial state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_bearer_cntxt_set_bearer_pdn_pend_redial_state
(
  ds_bearer_context_s                   *bearer_cntx_ptr,
  ds_pdn_context_s                      *pdn_cntx_ptr  
)
{
  sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_3gpp_pdn_cntx_validate_pdn_context_internal(pdn_cntx_ptr) ||
     !ds_bearer_cntxt_get_subs_id_from_bearer_internal(bearer_cntx_ptr, &subs_id))
  {
    return;
  }
  
  ds_bearer_cntxt_set_bearer_state(bearer_cntx_ptr,
                              DS_BEARER_CONTEXT_STATE_PEND_REDIAL);
  
  pdn_cntx_ptr->ds_pdn_context_dyn_p->state =
                                 DS_PDN_CONTEXT_STATE_PEND_REDIAL;

  DS_LTE_MSG2_HIGH_EX(subs_id,
              "Bearer context:0x%x in pending redial state, "
              "Putting pdn context:0x%x in pending redial state",
              bearer_cntx_ptr,pdn_cntx_ptr);  
}/* ds_3gpp_bearer_cntxt_set_bearer_pdn_pend_redial_state */

#endif /* FEATURE_DATA_3GPP */
/*===========================================================================

                                UNIT TEST

===========================================================================*/
