
/*!
  @file
  ds_eps_bearer_context.h

  @brief
  Lower layer/Bearer Context interface

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2008 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps/inc/ds_eps_bearer_context.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/27/12   ss      Fixed issue of RAB_REESTAB_IND received before resume ind
                   resulting in error in data path setup.
01/02/12   msh     Coan: Feature cleanup
03/10/11   ttv     Added support for LTE watermark tracing.
===========================================================================*/

#ifndef DSEPSBEARERCONTEXT_H
#define DSEPSBEARERCONTEXT_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_LTE

#include "ds_3gpp_bearer_context.h"
#ifdef FEATURE_QXDM_WM_TRACING
#include "fc_wm.h"
#endif /* FEATURE_QXDM_WM_TRACING */
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

#ifdef FEATURE_LAPP_UFS 
#error code not present
#endif
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

typedef byte ds_rb_id_T;
/*-----------------------------------------------------------------------------
  This structure contains all info needed in the EPS bearer context.
-----------------------------------------------------------------------------*/
typedef struct
{  
  /*! Cookie to ensure dyn mem is valid */
  uint32               cookie;   
  /* eps bearer id */
  eps_bearer_id_T                       eps_bearer_id; 
  /* linked bearer id */
  eps_bearer_id_T                       lbi;      
  /* RBID corresponding to this EPS bearer */
  ds_rb_id_T                            rb_id;    
  /* Indicates if this bearer is in TLB mode */
  boolean                               is_in_tlb; 
  /*Indicates PDCP registration status*/
  boolean              is_registered_pdcp;

  /*Indicates if DL wm reg'ed with A2*/
  boolean                               dl_wm_reg_with_a2; 

  /* corresponding bearer context */
  ds_bearer_context_s                   *bearer_context_p; 

  /* index where the memory would be allocated */
  int                  index;

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */
  /*----------------------------------------------------------------------------
   Recommended bit rate info from MAC layer.
   ----------------------------------------------------------------------------*/
  ps_phys_link_ran_asst_media_adapt_info_type  rec_bit_rate;
  boolean                                      rec_bit_rate_update;
} ds_eps_bearer_context_dyn_s;

typedef struct
{
  uint32  lo_wm_level;
  uint32  hi_wm_level;
  uint32  dne_wm_level;
  uint32  dne_q_cnt;
} ds_eps_bearer_cntxt_wm_levels_type;


typedef struct
{
  ds_eps_bearer_context_dyn_s *ds_eps_bearer_context_dyn_p;
} ds_eps_bearer_context_s;

/*-----------------------------------------------------------------------------
  EPS bearer context table exposed to other files through header
-----------------------------------------------------------------------------*/
extern ds_eps_bearer_context_s eps_bearer_context_tbl[DS_3GPP_MAX_BEARER_CONTEXT];

/*-----------------------------------------------------------------------------
  enum to indentify the flow direction
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_EPS_FLOW_DIR_INVALID        = 0,
  DS_EPS_FLOW_DIR_DOWNLINK       = 1,
  DS_EPS_FLOW_DIR_UPLINK         = 2,
  DS_EPS_FLOW_DIR_BIDIRECTIONAL  = 3
} ds_eps_bearer_context_flow_dir_e;

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

/*-------------------------------------------------------------------------
Cookie to ensure that dynamic memory allocated is valid
---------------------------------------------------------------------------*/
#define EPS_BEARER_CNTXT_VALID_COOKIE  0xebeaebea

/*default LTE ue category CAT 4*/
#define LTE_UE_CLASSES_DEFAULT 4
#ifdef  FEATURE_XMOD
  #define LTE_UE_CLASSES_MAX 22
#else
  #define LTE_UE_CLASSES_MAX 11
#endif

#define UL_WM_LO_INDEX       0
#define UL_WM_HI_INDEX       1
#define UL_WM_DNE_INDEX      2 
#define UL_WM_DNE_QCNT_INDEX 3

#define DL_WM_LO_INDEX       0
#define DL_WM_HI_INDEX       1
#define DL_WM_DNE_INDEX      2 
#define DL_WM_DNE_QCNT_INDEX 3

#define UL_WM_ARRAY_SIZE     4
#define DL_WM_ARRAY_SIZE     4

/* UL data rates in Kbps*/
#define DSEPS_UL_RATE_50_M           51024
#define DSEPS_UL_RATE_75_M           75376
#define DSEPS_UL_RATE_100_M         102048
#define DSEPS_UL_RATE_150_M         150752
 
/* DL data rates in Kbps*/
#define DSEPS_DL_RATE_150_M          150752
#define DSEPS_DL_RATE_300_M          301504
#define DSEPS_DL_RATE_450_M          452256
#define DSEPS_DL_RATE_600_M          603008
#define DSEPS_DL_RATE_1000_M        1051360

/*---------------------------------------------------------------------------
  Below table is used to determine the LTE UL watermark levels based on
  ue category. LO = 160000, HI = 265000, DNE = 345000, DNE QCNT = 690
  are used as default values (50 Mbps rates). New increases are made on top
  of these levels.
---------------------------------------------------------------------------*/
extern uint32 ds_eps_bearer_ul_wm_level_tbl[LTE_UE_CLASSES_MAX][UL_WM_ARRAY_SIZE];

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_qsh_mdump_collect
  
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
void ds_eps_bearer_cntxt_qsh_mdump_collect
(
  void
);
#endif

/*===========================================================================
FUNCTION DSEPSBEARERCNTXT_INIT

DESCRIPTION
  

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_bearer_cntxt_init( void );

/*===========================================================================
FUNCTION DSEPSBEARERCNTXT_TEAR_DOWN_PDN_CONNECTION

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
);

/*===========================================================================
FUNCTION DSEPSBEARERCNTXT_GETFLOWDIRECTION

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
);

/*===========================================================================
FUNCTION DSEPSBEARERCNTX_PS_TO_PDCP_FLOW_ENABLE_CB

DESCRIPTION
  This function sends a flow enable command to ds task
  
PARAMETERS  
  bearer_context_p   - bearer context pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_eps_bearer_cntxt_ps_to_pdcp_flow_enable_cb
(
 dsm_watermark_type *wm,
 /*lint -esym(715,wm) */ 
 /*lint -esym(818,wm) */ 
 void*              callback_data
);

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
);

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
  ds_bearer_context_s           *bearer_context_p
);

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
);
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
);

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
);

/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_send_QOS_indication

DESCRIPTION
  This function sends the QCI information of a particular bearer to interested
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
void ds_eps_bearer_cntxt_send_QOS_indication(ds_bearer_context_s *bearer_context_p);

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
);

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
);

/*===========================================================================
FUNCTION ds_eps_bearer_cntxt_free_dyn_mem

DESCRIPTION
  This function frees dyn memory.
 
PARAMETERS
  bearer_cntxt_p
 
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
);
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
);

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_UL_WM_LEVELS_PER_UL_RATE

DESCRIPTION
  This function returns the statically configured uplink watermark levels
  for a given  uplink data rate level.

PARAMETERS
  ul_data_rate: UL data rate passed by ML1
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
);
/*==========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_EXAMINE_IF_EMM_CAUSE_THROT_NEEDED

DESCRIPTION 
  This function checks whether EMM cause throttling is needed or not.
    
PARAMETERS
  LTE Call Info Ptr
  *rab_throt_info,

  
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
  cm_lte_call_info_s_type  *lte_call_info_ptr,
  ds_3gpp_throt_rab_info_s *rab_throt_info,
  sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_GET_LTE_DIV_DUPLEX

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
);

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
);

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
);

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
);

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
);


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
);


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
);


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
);


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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif /* FEATURE_DATA_LTE */
#endif /* DSEPSBEARERCONTEXT_H */

