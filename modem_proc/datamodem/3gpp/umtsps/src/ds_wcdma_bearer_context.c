

/*!
  @file
  ds_wcdma_bearer_context.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2010-2019 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/umtsps/src/ds_wcdma_bearer_context.c#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/12   ss      Delegating RAT specific handling during call_end. Added
                   wm purge support for WCDMA.
08/08/12   ss      Optimization to trigger Dynamic BP only when threshold
                   num of acks are received.
07/04/12   ss      Freeing the BP timer handle in deact_cnf and avoiding
                   crash due to using stale timer handle.
06/28/12   ss      Fixed the issue of WCDMA context_transfer_hdlr not getting
                   registered.
03/27/12   ss      Fixed issue of RAB_REESTAB_IND received before resume ind
                   resulting in error in data path setup.
01/02/12   msh     Coan: Feature cleanup
12/21/11   ss      Added support for watermark tuning.
10/19/11   nd      Bearer rate querry uses NW granted bit rates.
11/21/11   nd      Changing watermark values to enable/disable back pressure
                   for TCP throughput optimization.
10/21/11   nd      Fixed compilation warnings.
10/20/11   ss      Dynamic back pressure for TCP throughput optimization.
09/22/11   am      Added pkt classification for DL aggregation.
07/21/11   sa      Added support for 64QAM configuration indication while roaming.
06/03/11   ttv     Added missing NULL checks.
03/17/11   sa      Added support for bearer tech change event indication.
03/02/11   ss      Added support for configurable RAB_REESTAB RETRY.
02/23/11   ttv     Added support for watermark tracing.
01/31/11   ss      Added support for reporting bearer tech type in WCDMA
                   and GPRS.
01/27/11   ss      Fixed the data path for PDP-PPP in resume hdlr.
12/01/10   ss      Support for SIM REFRESH VOTING for WCDMA and GPRS.

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "sys.h"

#ifdef FEATURE_TDSCDMA
#ifndef FEATURE_SEGMENT_LOADING
//#include "tdsrrcdata.h"
#include "tdsrrcdataext.h"
#endif /* FEATURE_SEGMENT_LOADING */
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_DATA_WCDMA_PS

#include "ds_3gpp_bearer_flow_manager.h"
#include "ds_3gpp_nv_manager.h"
#include "ds_wcdma_bearer_context.h"
#include "rrcdata.h"
#include "rlcdsapif.h"
#include "wl2api.h"
#ifndef FEATURE_SEGMENT_LOADING
#ifdef FEATURE_TDSCDMA
//#include "tdsrrcdata.h"
#endif /*FEATURE_TDSCDMA*/
#else
#include "IWCDMA.h"
#include "ITDSCDMA.h"
#endif /*FEATURE_SEGMENT_LOADING*/
#include "ds3gmgr.h"
#include "ds3gtputmgr.h"
#include "ds3gflowmgr.h"
#include "ds3gmshif.h"
#include "comptask_api.h"
#include "comptask_v.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_hdlr.h"
#include "ran.h"

#include "dsm_queue.h"
#include "pstimer.h"
#include "ps_sys_ioctl.h"
#include "ds_umts_bearer_context.h"
#include "ds_3gpp_bearer_context.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "ds_3gpp_loopback_hdlr.h"
#include "ds_umts_tlb_hdlr.h"
#include "ds_3gpp_flow_manager.h"

#ifdef FEATURE_DATA_LTE
#include "ds_eps_tlb.h"
#endif
#include "ps_pkt_info_utils.h"
#include "ds_3gpp_tput_hdlr.h"

/*===========================================================================
                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_SEGMENT_LOADING
/*---------------------------------------------------------------------------
TDSCDMA and WCDMA interface tables
---------------------------------------------------------------------------*/
interface_t *ds_3gpp_ps_tds_tbl;
interface_t *ds_3gpp_ps_wcdma_tbl;
#endif /* FEATURE_SEGMENT_LOADING */

extern ds_pdn_context_s ds_pdn_context_tbl[DS_3GPP_MAX_PDN_CONTEXT];

typedef union
{
#ifdef FEATURE_TDSCDMA
  tdsrrc_user_plane_lc_info_type tdsrrc_lc_info;
#endif
  rrc_user_plane_lc_info_type    rrc_lc_info;
}user_plane_lc_info_type;

/*---------------------------------------------------------------------------
  Struct to post DS_CMD_3GPP_WCDMA_CONFIG_UL_DATA_RATE command
---------------------------------------------------------------------------*/
typedef struct
{
  pdcp_rrc_data_rate_ind_e_type  pdcp_ind;
  uint32                         rb_id;
  uint8                          rlc_ul_id;
  uint32                         rab_ul_data_rate;
} ds_wcdma_bearer_cntxt_data_rate_ind_type;

/*---------------------------------------------------------------------------
  Struct to cache WCDMA RB data
---------------------------------------------------------------------------*/
typedef struct
{
  boolean  is_occupied;
  uint32   rb_id;
  uint8    rlc_ul_id;
  uint32   rab_ul_data_rate;
} ds_wcdma_bearer_cntxt_rb_data_type;

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/
/*----------------------------------------------------------------------------
  Cache to store the RB data sent from WCDMA RRC. This data will be used to
  setup WCDMA UL WM levels
----------------------------------------------------------------------------*/
static ds_wcdma_bearer_cntxt_rb_data_type
          ds_wcdma_bearer_cntxt_rb_data[DS_3GPP_MAX_BEARER_CONTEXT];

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_AVAIL_RB_DATA

DESCRIPTION
  This function returns the first available RB data

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  RB data

SIDE EFFECTS
  None.

===========================================================================*/
ds_wcdma_bearer_cntxt_rb_data_type* ds_wcdma_bearer_cntxt_get_avail_rb_data
(
  void
)
{
  uint8                                index = 0;
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for( index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    if( ds_wcdma_bearer_cntxt_rb_data[index].is_occupied == FALSE )
    {
      break;
    }
  }
  if( index < DS_3GPP_MAX_BEARER_CONTEXT)
  {
    rb_data_ptr = &ds_wcdma_bearer_cntxt_rb_data[index];
  }
  else
  {
    DS_WCDMA_MSG0_ERROR( "No free RB data available");
  }
  return rb_data_ptr;
} /* ds_wcdma_bearer_cntxt_get_avail_rb_data */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_FIND_RB_DATA

DESCRIPTION
  This function finds and returns the RB data

PARAMETERS
  rb_id: Radio Bearer ID
  rlc_ul_id: RLC UL ID

DEPENDENCIES
  None.

RETURN VALUE
  RB data

SIDE EFFECTS
  None.

===========================================================================*/
ds_wcdma_bearer_cntxt_rb_data_type* ds_wcdma_bearer_cntxt_find_rb_data
(
  uint8  rlc_ul_id
)
{
  uint8                                index = 0;
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for( index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    if( (ds_wcdma_bearer_cntxt_rb_data[index].is_occupied == TRUE)   &&
        (ds_wcdma_bearer_cntxt_rb_data[index].rlc_ul_id == rlc_ul_id)
      )
    {
      break;
    }
  }
  if( index < DS_3GPP_MAX_BEARER_CONTEXT)
  {
    rb_data_ptr = &ds_wcdma_bearer_cntxt_rb_data[index];
  }
  else
  {
    DS_WCDMA_MSG0_ERROR( "RB data could not be found");
  }
  return rb_data_ptr;
} /* ds_wcdma_bearer_cntxt_find_rb_data */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_RESET_RB_DATA

DESCRIPTION
  This function resets passed RB data

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_reset_rb_data
(
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_p
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( rb_data_p != NULL )
  {
    memset(rb_data_p, 0, sizeof(ds_wcdma_bearer_cntxt_rb_data_type));
  }
  return;
} /* ds_wcdma_bearer_cntxt_reset_rb_data */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_RESET_ALL_RB_DATA

DESCRIPTION
  This function resets all RB data

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_reset_all_rb_data( void )
{
  uint8  index = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for( index = 0; index < DS_3GPP_MAX_BEARER_CONTEXT; index++)
  {
    ds_wcdma_bearer_cntxt_reset_rb_data(&ds_wcdma_bearer_cntxt_rb_data[index]);
  }
  return;
} /* ds_wcdma_bearer_cntxt_reset_all_rb_data */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_SETUP_PDP_PS_TO_PDCP_WM

DESCRIPTION
  This function initializes the PS->PDCP watermark for PDP calls.

PARAMETERS
  bearer_context_p - Pointer to the bearer context

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_setup_pdp_ps_to_pdcp_wm
(
  ds_bearer_context_s     *bearer_context_p,
  sys_modem_as_id_e_type   subs_id,
  uint32                   w_rab_ul_data_rate
)
{
  ds_bearer_context_wm_s                         *watermark_p;
  ds_bearer_cntxt_um_wm_threshold_struct_type     um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type tds_um_wm_threshold;
  ds_wcdma_bearer_cntxt_wm_levels_type            wcdma_ul_wm_levels;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);
  tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);


  watermark_p = &bearer_context_p->tx_wm;

  if(bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS)
  {
    dsm_set_low_wm(&(watermark_p->wm_item),
                   tds_um_wm_threshold.tds_ul_lo);
    dsm_set_hi_wm(&(watermark_p->wm_item),
                  tds_um_wm_threshold.tds_ul_hi);
    dsm_set_dne(&(watermark_p->wm_item),
                tds_um_wm_threshold.tds_ul_dne);
    dsm_set_dne_q_cnt(&(watermark_p->wm_item),DSTPSD_UL_WM_DNE_QCNT);
  }
  else
  {
    /* If RAB UL data rate is not passed use default value */
    if( w_rab_ul_data_rate == 0 )
    {
      dsm_set_low_wm(&(watermark_p->wm_item),
                     um_wm_threshold.wcdma_ul_lo);
      dsm_set_hi_wm(&(watermark_p->wm_item),
                    um_wm_threshold.wcdma_ul_hi);
      dsm_set_dne(&(watermark_p->wm_item),
                  um_wm_threshold.wcdma_ul_dne);
      dsm_set_dne_q_cnt(&(watermark_p->wm_item),DSWPSD_UL_WM_DNE_QCNT);
    }
    else
    {
      /* Get WM levels based on UL data rate */
      ds_wcdma_bearer_cntxt_get_ul_wm_levels_per_ul_rate(w_rab_ul_data_rate,
                                                         &wcdma_ul_wm_levels);

      dsm_set_low_wm(&(watermark_p->wm_item),
                     wcdma_ul_wm_levels.lo_wm_level);
      dsm_set_hi_wm(&(watermark_p->wm_item),
                    wcdma_ul_wm_levels.hi_wm_level);
      dsm_set_dne(&(watermark_p->wm_item),
                  wcdma_ul_wm_levels.dne_wm_level);
      dsm_set_dne_q_cnt(&(watermark_p->wm_item),
                        wcdma_ul_wm_levels.dne_q_cnt);
    }
  }

  /*-----------------------------------------------------------------------
    Pass the call instance in watermarks registered so that we
    can identify the call when the callback is called
  -----------------------------------------------------------------------*/
  watermark_p->wm_item.lowater_func_ptr      =
            (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_enable_cb;
  watermark_p->wm_item.lowater_func_data     =
            (void*) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr;

  watermark_p->wm_item.hiwater_func_ptr      =
            (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_disable_cb;
  watermark_p->wm_item.hiwater_func_data     =
            (void*) bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr;

#ifdef FEATURE_DATA_FLOW_MGMT

  watermark_p->wm_item.dne_func_ptr = 
                         (wm_cb_type)ds_bearer_cntxt_ps_to_L2_flow_dne_disable_cb;


  watermark_p->wm_item.gone_empty_func_ptr = 
          (wm_cb_type)ds_bearer_cntxt_ps_to_L2_gone_empty_cb;

  watermark_p->wm_item.gone_empty_func_data = 
          (void*)bearer_context_p;
#endif /*FEATURE_DATA_FLOW_MGMT */

  DS_WCDMA_MSG3_HIGH("Uplink WM levels: DNE: %d HI: %d LO: %d",
                      watermark_p->wm_item.dont_exceed_cnt,
                      watermark_p->wm_item.hi_watermark,
                      watermark_p->wm_item.lo_watermark);

  return;
} /* dsWcdmaBearerCntxt_setup_pdp_ps_to_pdcp_wm() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_SETUP_PDP_PDCP_TO_PS_WM

DESCRIPTION
  This function initializes the PDCP->PS watermark. Used for pdp-ip & pdp-ppp
  calls

PARAMETERS
  bearer_context_p - Pointer to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_setup_pdp_dl_wm
(
  ds_bearer_context_s*    bearer_context_p
)
{
  ds_bearer_context_wm_s            *watermark_p;
  ds_pdn_context_s                  *pdn_cntx_p = NULL;
  sys_modem_as_id_e_type            subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_um_wm_threshold_struct_type  um_wm_threshold;
  ds_bearer_cntxt_tds_um_wm_threshold_struct_type  tds_um_wm_threshold;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return ;
  }

  pdn_cntx_p = bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;
  if (!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntx_p))
  {
    return;
  }

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(bearer_context_p->
                                             ds_bearer_context_dyn_p->pdn_context_ptr,
                                             &subs_id))
  {
    return;
  }

  watermark_p = &bearer_context_p->rx_wm;
  um_wm_threshold = ds_3gpp_nv_get_um_wm_threshold(subs_id);
  tds_um_wm_threshold =  ds_3gpp_nv_get_tds_wm_threshold(subs_id);


  if(bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS)
  {
    dsm_set_low_wm(&(watermark_p->wm_item),
                   tds_um_wm_threshold.tds_dl_lo);
    dsm_set_hi_wm(&(watermark_p->wm_item),
                  tds_um_wm_threshold.tds_dl_hi);
    dsm_set_dne(&(watermark_p->wm_item),
                tds_um_wm_threshold.tds_dl_dne);
    dsm_set_dne_q_cnt(&(watermark_p->wm_item),DSTPSD_DL_WM_DNE_QCNT);
  }
  else
  {
    dsm_set_low_wm(&(watermark_p->wm_item),
                   um_wm_threshold.wcdma_dl_lo);
    dsm_set_hi_wm(&(watermark_p->wm_item),
                  um_wm_threshold.wcdma_dl_hi);
    dsm_set_dne(&(watermark_p->wm_item),
                um_wm_threshold.wcdma_dl_dne);
    dsm_set_dne_q_cnt(&(watermark_p->wm_item),DSWPSD_DL_WM_DNE_QCNT);
  }
  /*-------------------------------------------------------------------------
    Reset the flag which keeps track if we have sent a close req to PDCP
    to tigger flow controlling the peer.
    - This is not used
  -------------------------------------------------------------------------*/
  // dswpsd_reset_sent_close_to_pdcp(call_instance);


  DS_WCDMA_MSG3_HIGH("Downlink  RX WM levels: DNE: %d HI: %d LO: %d",
                      watermark_p->wm_item.dont_exceed_cnt,
                      watermark_p->wm_item.hi_watermark,
                      watermark_p->wm_item.lo_watermark);

} /* dsWcdmaBearerCntxt_setup_pdp_pdcp_to_ps_wm() */

#ifdef FEATURE_WCDMA
/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_GET_RRC_USER_PLANE_INFO_FROM_NSAPI

DESCRIPTION
  This function retrieves the RRC user plane information from the nsapi

PARAMETERS   :
  nsapi           - NSAPI
  *lc_info_ptr    - Pointer to the RRC user plane information
  subs_id         - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS : Successful setup
  DS3G_FAILURE : Setup error

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL boolean dsWcdmaBearerCntxt_get_rrc_user_plane_info_from_nsapi
(
  uint8                         nsapi,
  rrc_user_plane_lc_info_type  *lc_info_ptr,
  sys_modem_as_id_e_type        subs_id
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( lc_info_ptr == NULL )
  {
    DS_WCDMA_MSG1_LOW("NULL Ptr passed lc_info_ptr:%d", lc_info_ptr );
    return DS3G_FAILURE;
  }

  /*-------------------------------------------------------------------------
   call RRC to get logical ch ID's
  -------------------------------------------------------------------------*/
  lc_info_ptr->rab_id = nsapi;

  if (ds_3gpp_loopback_is_enabled())
  {
    if(ds_3gpp_loopback_hdlr_rrc_return_lc_info_for_rab(lc_info_ptr) == RRC_RAB_NOT_FOUND )
    {
      return DS3G_FAILURE;
    }
  }
  else
  {
#ifdef FEATURE_SEGMENT_LOADING
#ifdef FEATURE_DUAL_WCDMA
    if ( ( ds_3gpp_ps_wcdma_tbl == NULL ) ||
         ( IWCDMA_rrc_return_lc_info_for_rab_sub(ds_3gpp_ps_wcdma_tbl, subs_id,
                             lc_info_ptr ) ==  RRC_RAB_NOT_FOUND ) )
#else
    if ( ( ds_3gpp_ps_wcdma_tbl == NULL ) ||
        (IWCDMA_rrc_return_lc_info_for_rab(ds_3gpp_ps_wcdma_tbl,
                             lc_info_ptr ) ==  RRC_RAB_NOT_FOUND ) )
#endif
#else
#ifdef FEATURE_DUAL_WCDMA
    if ( rrc_return_lc_info_for_rab_sub(
        subs_id, lc_info_ptr ) == RRC_RAB_NOT_FOUND )
#else
    if ( rrc_return_lc_info_for_rab( lc_info_ptr ) == RRC_RAB_NOT_FOUND )
#endif

#endif  /* FEATURE_SEGMENT_LOADING */
    {
      DS_WCDMA_MSG1_ERROR("Invalid RAB id %d",nsapi);  
      return  DS3G_FAILURE ;
    }
  }

  return DS3G_SUCCESS;
} /* dsWcdmaBearerCntxt_get_rrc_user_plane_info_from_nsapi() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_GET_RRC_USER_PLANE_INFO

DESCRIPTION
  This function retrieves the RRC user plane information from the call
  connected information passed in

PARAMETERS   :
  *mode_info_ptr  - Pointer to the mode-specific call information provided by
                    Call Manager
  *lc_info_ptr    - Pointer to the RRC user plane information
  subs_id         - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS : Successful setup
  DS3G_FAILURE : Setup error

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL boolean dsWcdmaBearerCntxt_get_rrc_user_plane_info
(
  const cm_call_mode_info_s_type  *mode_info_ptr,
  rrc_user_plane_lc_info_type     *lc_info_ptr,
  sys_modem_as_id_e_type           subs_id
)
{
  boolean ret_val = DS3G_FAILURE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (mode_info_ptr == NULL) || (lc_info_ptr == NULL) )
  {
    DS_WCDMA_MSG2_LOW("NULL Ptr passed mode_info_ptr:%d lc_info_ptr:%d",
                    mode_info_ptr, lc_info_ptr );
    return DS3G_FAILURE;
  }

  ret_val = dsWcdmaBearerCntxt_get_rrc_user_plane_info_from_nsapi(
              mode_info_ptr->info.gw_ps_call.nsapi.nsapi,
              lc_info_ptr,
              subs_id);

  return ret_val;
} /* dsWcdmaBearerCntxt_get_rrc_user_plane_info() */
#endif /* FEATURE_WCDMA*/

#ifdef FEATURE_TDSCDMA
/*===========================================================================
FUNCTION DS_TDS_BEARER_CNTXT_GET_RRC_USER_PLANE_INFO

DESCRIPTION
  This function retrieves the RRC user plane information from the call
  connected information passed in

PARAMETERS   :
  *mode_info_ptr  - Pointer to the mode-specific call information provided by
                    Call Manager
  *lc_info_ptr    - Pointer to the RRC user plane information

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS : Successful setup
  DS3G_FAILURE : Setup error

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL boolean ds_tds_bearer_cntxt_get_rrc_user_plane_info
(
  const cm_call_mode_info_s_type     *mode_info_ptr,
  tdsrrc_user_plane_lc_info_type     *lc_info_ptr,
  sys_modem_as_id_e_type              subs_id
)
{

  if ( (NULL == lc_info_ptr) || ( NULL == mode_info_ptr ))
  {
    return ( DS3G_FAILURE );
  }
  /*-------------------------------------------------------------------------
   Get PDCP rab_id's from call connected and call RRC to get logical ch ID's to
   give to PDCP : Get the corresponding PDCP ids for the g id
  -------------------------------------------------------------------------*/

  lc_info_ptr->rab_id = mode_info_ptr->info.gw_ps_call.nsapi.nsapi;

#ifdef FEATURE_SEGMENT_LOADING
#ifdef FEATURE_DUAL_WCDMA
  if ( (ds_3gpp_ps_tds_tbl == NULL)||
       (ITDSCDMA_tdsrrc_return_lc_info_for_rab_sub(ds_3gpp_ps_tds_tbl,
                       subs_id, lc_info_ptr) == TDSRRC_RAB_NOT_FOUND ) )
#else
  if ( (ds_3gpp_ps_tds_tbl == NULL) ||
       (ITDSCDMA_tdsrrc_return_lc_info_for_rab(ds_3gpp_ps_tds_tbl, lc_info_ptr) ==
                      TDSRRC_RAB_NOT_FOUND ) )
#endif
#else
  if ( tdsrrc_return_lc_info_for_rab( lc_info_ptr ) == TDSRRC_RAB_NOT_FOUND )
#endif /* FEATURE_SEGMENT_LOADING */
  {
      DS_WCDMA_MSG1_ERROR("Invalid RAB id %d",
                          mode_info_ptr->info.gw_ps_call.nsapi.nsapi );
      return ( DS3G_FAILURE );
  }

  return DS3G_SUCCESS;

} /* ds_tds_bearer_cntxt_get_rrc_user_plane_info() */
#endif /* FEATURE_TDSCDMA */

/*===========================================================================
FUNCTION DS_WCDMA_GET_COST_OF_MODEM_POWER

DESCRIPTION
  This returns the cost of modem power on the current subscription ID

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
int16 ds_wcdma_get_cost_of_modem_power
(
   void               *argval_ptr,
   int16              *ps_errno,
   uint8               subs_id
)
{
  int16                                         ret_val             = -1;
  ps_sys_modem_power_cost_enum_type            *cost_modem_pwr_p    = NULL;
#ifdef FEATURE_DATA_WCDMA_PS
  wl1_ul_modem_tx_power_enum_type               wl1_ul_mdm_pwr_cost;
  /*- - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - */
  if(ps_errno == NULL)
  {
    return ret_val;
  }

  if(argval_ptr == NULL)
  {
    DS_WCDMA_MSG0_LOW("Invalid args to get cost of mdm power");
    *ps_errno = DS_EFAULT;
    return ret_val;
  }

  cost_modem_pwr_p = (ps_sys_modem_power_cost_enum_type *)argval_ptr;
  /*-------------------------------------------------------------------------
    Query WCDMA ML1 to get modem power cost
    -------------------------------------------------------------------------*/
  wl1_ul_mdm_pwr_cost = wl1_query_modem_tx_power();

  switch (wl1_ul_mdm_pwr_cost)
  {
    case WCDMA_MODEM_TX_POWER_LOW:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_LOW;
       break;
    case WCDMA_MODEM_TX_POWER_MEDIUM:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_MEDIUM;
       break;
    case WCDMA_MODEM_TX_POWER_HIGH:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_HIGH;
       break;
    default:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_UNKNOWN;
      break;
  }
  *ps_errno = DS_ENOERR;
  ret_val = 0;
#else
  *ps_errno = DS_EOPNOTSUPP;
  ret_val = -1;
#endif
  DS_WCDMA_MSG3_HIGH("WCDMA ML1 reports cost of Modem power %d ret_val %d, ps_err %d",
                     *cost_modem_pwr_p,
                     ret_val,
                     *ps_errno);
  return ret_val;
}

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_REGISTER_WM_WITH_PDCP

DESCRIPTION
  This function registers the watermarks with PDCP for uplink & downlink data

PARAMETERS
   lc_info_ptr     - rrc or tdsrrc user plane info that was retrieved from CM
   to_ps_pkt_wm_item_ptr  - Pointer to WM used between PS & PDCP for
                    downlink data
   to_ps_post_rx_fn_ptr - callback Function installed in the watermark
                    for downlink data - called by PDCP when new data is in
   from_ps_pkt_wm_item_ptr - Pointer to WM used between PS & PDCP for
                    uplink data
   ds_bearer_context_s*  - Pointer to the bearer context being worked with.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_register_wm_with_pdcp
(
    user_plane_lc_info_type     *lc_info_ptr,
    dsm_watermark_type          *to_ps_pkt_wm_item_ptr,
    void                        (*to_ps_post_rx_fn_ptr)(byte, byte,void*),
    dsm_watermark_type          *from_ps_pkt_wm_item_ptr ,
    ds_bearer_context_s*        bearer_context_p
)
{
  comp_cmd_type *pdcp_cmd;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  /*-----------------------------------------------------------------------
   Register the UL watermarks with PDCP
    -----------------------------------------------------------------------*/
  if((pdcp_cmd = comp_get_cmd_buf()) == NULL)
  {
    /*-----------------------------------------------------------------------
      No free command buffers available, log an error.
    -----------------------------------------------------------------------*/
    DS_WCDMA_MSG0_ERROR( "No cmd buffers available in COMP task free cmd q");
    return;
  }

  pdcp_cmd->hdr.cmd_id = PDCP_REG_WM;
  pdcp_cmd->hdr.subs_id = subs_id;

#ifdef FEATURE_TDSCDMA
  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS)
  {
    pdcp_cmd->cmd.pdcp_reg_wm.ul_lc_id=
                  lc_info_ptr->tdsrrc_lc_info.ul_lc_id[0];
    pdcp_cmd->cmd.pdcp_reg_wm.dl_lc_id=
                  lc_info_ptr->tdsrrc_lc_info.dl_lc_id[0];
  }
  else if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA)
#endif /* FEATURE_TDSCDMA */
  {
    pdcp_cmd->cmd.pdcp_reg_wm.ul_lc_id =
                  lc_info_ptr->rrc_lc_info.ul_lc_id[0];
    pdcp_cmd->cmd.pdcp_reg_wm.dl_lc_id =
                  lc_info_ptr->rrc_lc_info.dl_lc_id[0];
  }

  pdcp_cmd->cmd.pdcp_reg_wm.bearer_call_mode
    = bearer_context_p->ds_bearer_context_dyn_p->call_mode;
  pdcp_cmd->cmd.pdcp_reg_wm.ps_to_pdcp_wm_ptr = from_ps_pkt_wm_item_ptr;
  pdcp_cmd->cmd.pdcp_reg_wm.pdcp_to_ps_wm_ptr = to_ps_pkt_wm_item_ptr;
  pdcp_cmd->cmd.pdcp_reg_wm.pdcp_to_ps_post_rx_func_ptr = to_ps_post_rx_fn_ptr;
  pdcp_cmd->cmd.pdcp_reg_wm.pdcp_to_ps_rx_func_para = (void *)bearer_context_p;

  /*
    VRK:TODO INTERRAT This is broken
    After EPS->UMTS handoff, we don't have QOS parameters, so this is passing
    junk values. The DS_3GPP_UMTS_VOBJ is memset on the handoff.
  */
#ifdef FEATURE_CPU_BASED_FLOW_CONTROL
  if (dsUmtsBearerCntxt_validatebearer(bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ]) )
  {
    pdcp_cmd->cmd.pdcp_reg_wm.traf_class =
                    ( (ds_umts_bearer_context_s*)\
                      (bearer_context_p->ds_bearer_context_dyn_p->vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ])\
                    )->ds_umts_bearer_context_dyn_p->umts_qos_val.traffic_class;
  }
  else
  {
    DS_WCDMA_MSG0_LOW("Not including traffic class QOS for PDCP WM registration");
  }

#endif
  comp_put_cmd(pdcp_cmd);

  return;
} /* dsWcdmaBearerCntxt_register_wm_with_pdcp() */

/*===========================================================================
FUNCTION DS_TDS_GET_COST_OF_MODEM_POWER

DESCRIPTION
  This returns the cost of modem power on the current subscription ID

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
int16 ds_tds_get_cost_of_modem_power
(
   void               *argval_ptr,
   int16              *ps_errno,
   uint8               subs_id
)
{

  int16                                         ret_val               = -1;
#ifdef FEATURE_TDSCDMA
  ps_sys_modem_power_cost_enum_type            *cost_modem_pwr_p      = NULL;
  tdsrrc_ul_tx_power_range_e_type               tds_ul_tx_power_range;
  /*- - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - */

  if(ps_errno == NULL)
  {
    return ret_val;
  }

  if(argval_ptr == NULL)
  {
    *ps_errno = DS_EFAULT;
    return ret_val;
  }
  cost_modem_pwr_p = (ps_sys_modem_power_cost_enum_type *)argval_ptr;
  /*-------------------------------------------------------------------------
    Query WCDMA ML1 to get modem power cost
    -------------------------------------------------------------------------*/
  tds_ul_tx_power_range = tdsrrc_get_ul_tx_power_range();

  switch (tds_ul_tx_power_range)
  {
    case TDSRRC_UL_TX_POWER_LOW:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_LOW;
       break;
    case TDSRRC_UL_TX_POWER_MEDIUM:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_MEDIUM;
       break;
    case TDSRRC_UL_TX_POWER_HIGH:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_HIGH;
       break;
    default:
      *cost_modem_pwr_p = DS_SYS_MODEM_POWER_COST_UNKNOWN;
      break;
  }
  *ps_errno = DS_ENOERR;
  ret_val = 0;
  DS_WCDMA_MSG3_HIGH("TDSCDMA ML1 reports cost of Modem power %d ret_val %d, ps_err %d",
                    *cost_modem_pwr_p,
                    ret_val,
                    *ps_errno);
#else
  *ps_errno = DS_EOPNOTSUPP;
  ret_val = -1;
#endif

#ifdef FEATURE_TDSCDMA
  DS_WCDMA_MSG3_HIGH("TDSCDMA ML1 reports cost of Modem power %d ret_val %d, ps_err %d",
                    *cost_modem_pwr_p,
                    ret_val,
                    *ps_errno);
#endif

  return ret_val;
}
/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_RX_DATA_AT_U_PDP_CB()

DESCRIPTION
  This function is called when there is data in the PDCP RX queue to signal PS
  that there is data for processing.
  This is called from the pdcp task context.

PARAMETERS
  pdcp_id: PDCP logical channel ID
  num_sdus: Number of SDUs queued into WM for each TTI
  callback_data : Callback data registered with PDCP: Call instance
  is registered with the PDCP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_rx_data_at_u_pdp_cb
(
  byte pdcp_id,
  byte num_sdus,
  void *callback_data
)
{
  ds_bearer_context_s* bearer_context_p;  /* Bearer context pointer */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
    Get bearer context pointer from the callbackdata passed in
  -------------------------------------------------------------------------*/
  bearer_context_p = (ds_bearer_context_s*) callback_data;
  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return;
  }

  /*-------------------------------------------------------------------------
    Set Rx Signal.
  -------------------------------------------------------------------------*/
  PS_SET_SIGNAL(bearer_context_p->ds_bearer_context_dyn_p->rx_sig);

  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
} /* dsWcdmaBearerCntxt_rx_data_at_u_pdp_cb() */

void ds_tds_w_cdma_register_wm
(
  ds_bearer_context_s*  bearer_context_p
)
{
  void     *lc_info = NULL;
  boolean  rrc_user_plane_info_valid = FALSE;
  void  (*rlc_rx_fn_ptr)(byte, byte,void*) = NULL;

#ifdef FEATURE_TDSCDMA
  cm_call_mode_info_s_type  mode_info;
#endif

  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode ==
  	  SYS_SYS_MODE_WCDMA)
  {
    lc_info = (rrc_user_plane_lc_info_type *)modem_mem_alloc(
		                      sizeof(rrc_user_plane_lc_info_type),
		                      MODEM_MEM_CLIENT_DATA);
    if (NULL != lc_info)
    {
	  memset(lc_info, 0, sizeof(rrc_user_plane_lc_info_type));
      rrc_user_plane_info_valid
        = dsWcdmaBearerCntxt_get_rrc_user_plane_info_from_nsapi(
                                bearer_context_p->ds_bearer_context_dyn_p->nsapi,
                                lc_info,
                                bearer_context_p->ds_bearer_context_dyn_p->subs_id);
    }
  }
  else if (bearer_context_p->ds_bearer_context_dyn_p->call_mode ==
  	       SYS_SYS_MODE_TDS)
  {
#ifdef FEATURE_TDSCDMA
    lc_info = (tdsrrc_user_plane_lc_info_type *)modem_mem_alloc(
		                      sizeof(tdsrrc_user_plane_lc_info_type),
		                      MODEM_MEM_CLIENT_DATA);

	if (NULL != lc_info)
    {
      memset(lc_info, 0, sizeof(tdsrrc_user_plane_lc_info_type));
	  memset(&mode_info, 0, sizeof(cm_call_mode_info_s_type));

	  mode_info.info.gw_ps_call.nsapi.nsapi = 
	  	        bearer_context_p->ds_bearer_context_dyn_p->nsapi;
	  mode_info.info.gw_ps_call.nsapi.valid = TRUE;

	  rrc_user_plane_info_valid
	  	= ds_tds_bearer_cntxt_get_rrc_user_plane_info((const cm_call_mode_info_s_type *)&mode_info,
	  	                                              lc_info,
	  	              bearer_context_p->ds_bearer_context_dyn_p->subs_id);
    }
#endif 
  }

  if( rrc_user_plane_info_valid != TRUE )
  {
    /* set the bearer to be in UP_DORMANT state */
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT);
    DS_WCDMA_MSG1_ERROR("Couldn't get rrc user plane info for nsapi: %d",
		                 bearer_context_p->ds_bearer_context_dyn_p->nsapi);
    /*-------------------------------------------------------------------------
      Register each enqueue function ptr.
      bearer cntx ptr will be passed as parameter when watermark cb functions
      take a parameter.
    -------------------------------------------------------------------------*/
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =
                               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data = (void*) bearer_context_p;

    return;
  }
  else
  {
#ifdef TEST_FRAMEWORK
  #error code not present
#else
  rlc_rx_fn_ptr = NULL;
#endif

    dsWcdmaBearerCntxt_register_wm_with_pdcp(lc_info,
		                                     &(bearer_context_p->rx_wm.wm_item),
		                                     rlc_rx_fn_ptr,
		                                     &(bearer_context_p->tx_wm.wm_item), //Big Bug found here
		                                     bearer_context_p
		                                    );
  }

  if (NULL != lc_info)
  {
    modem_mem_free(lc_info, MODEM_MEM_CLIENT_DATA);
	lc_info = NULL;
  }
}

#ifdef FEATURE_WCDMA

/*===========================================================================
  FUNCTION DS_WCDMA_BEARER_CNTXT_SETUP_PDP_PROTO_STACK

  DESCRIPTION
    This function executes the common code for  both IP and PPP call setup
    for WCDMA and TDSCDMA

  PARAMETERS
   *bearer_context_p - Pointer to the bearer context that we are working with
   *mode_info_ptr    - Pointer to the mode-specific call information provided
                       by Call Manager

  DEPENDENCIES
    None.

  RETURN VALUE
    DS3G_SUCCESS : Successful setup
    DS3G_FAILURE : Setup error

  SIDE EFFECTS
    None.

===========================================================================*/
LOCAL boolean ds_wcdma_bearer_cntxt_setup_pdp_proto_stack
(
  ds_bearer_context_s            *bearer_context_p,
  const cm_call_mode_info_s_type *mode_info_ptr
)
{
  ds_bearer_context_wm_s              *tx_watermark_p;
  ds_bearer_context_wm_s*              rx_watermark_p;
  user_plane_lc_info_type              lc_info;       /* lc info from rrc */
  ds_umts_bearer_context_s            *umts_bearer_context_ptr = NULL;
  sys_modem_as_id_e_type               subs_id = SYS_MODEM_AS_ID_NONE;
  ds_umts_pdp_type_enum_type           pdp_type;
  ds_pdn_context_s                    *pdn_context_ptr = NULL;
  boolean                              rrc_user_plane_info_valid = FALSE;
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_p = NULL;
  uint32                               rab_ul_data_rate = 0;
  boolean                              register_wm = TRUE;

  void  (*rlc_rx_fn_ptr)(byte, byte,void*) = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  DS_WCDMA_MSG1_HIGH("Bearer Inst:%d Setting WCDMA PDP-Proto common stack component",
            bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*-------------------------------------------------------------------------
   Get the Device state information for the call.
  -------------------------------------------------------------------------*/
  tx_watermark_p = &(bearer_context_p->tx_wm);
  rx_watermark_p = &(bearer_context_p->rx_wm);

  /*-------------------------------------------------------------------------
    Get RRC user plane info
  -------------------------------------------------------------------------*/
  memset(&lc_info, 0, sizeof(user_plane_lc_info_type));
  rrc_user_plane_info_valid
    = dsWcdmaBearerCntxt_get_rrc_user_plane_info(mode_info_ptr,
                                                 &(lc_info.rrc_lc_info),
                                                 subs_id);

  /*-------------------------------------------------------------------------
    Check if RB data from W RRC is present for the bearer
  -------------------------------------------------------------------------*/
  rb_data_p = ds_wcdma_bearer_cntxt_find_rb_data(lc_info.rrc_lc_info.ul_lc_id[0]);
  if( rb_data_p != NULL )
  {
    rab_ul_data_rate = rb_data_p->rab_ul_data_rate;
  }

  /*-------------------------------------------------------------------------
    Set the PS->PDCP watermark.
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p,
                                             subs_id,
                                             rab_ul_data_rate);

  /*-------------------------------------------------------------------------
    Cleanup the Tx watermark just in case any data got enqueued after
    the previous call ended.
  -------------------------------------------------------------------------*/
  dsm_empty_queue(&tx_watermark_p->wm_item);

    /*-------------------------------------------------------------------------
    Set the PDCP->PS watermark.
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_dl_wm(bearer_context_p);

  //rx_watermark_p->wm_item.total_rcvd_cnt       = 0;

  /*-------------------------------------------------------------------------
    Set the amount of data dropped due to PDCP rab re-estab process failing
    Hari: We still do not have a place holder for this.
  -------------------------------------------------------------------------*/
  //device_state_ptr->ps_total_tx_data_flushed          = 0;

  //tx_watermark_p->wm_item.total_rcvd_cnt       = 0;

  if( rrc_user_plane_info_valid != DS3G_SUCCESS )
  {
    /* set the bearer to be in UP_DORMANT state */
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT);

    /*-------------------------------------------------------------------------
      Register each enqueue function ptr.
      bearer cntx ptr will be passed as parameter when watermark cb functions
      take a parameter.
    -------------------------------------------------------------------------*/
    (tx_watermark_p->wm_item).each_enqueue_func_ptr =
                               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (tx_watermark_p->wm_item).each_enqueue_func_data =
                                                    (void*) bearer_context_p;

    return FALSE;
  }

  /* get the umts bearer pointer for the corresponding bearer context*/
  umts_bearer_context_ptr=(ds_umts_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p->\
                           vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ]);
  /*validate the umts bearer context pointer*/
  if(!dsUmtsBearerCntxt_validatebearer(umts_bearer_context_ptr))
  {
    DS_WCDMA_MSG0_LOW("UMTS Bearer context of current bearer inst is invalid");
    return FALSE;
  }

  /* Store LC_ID for WCDMA here */
  umts_bearer_context_ptr->ds_umts_bearer_context_dyn_p->u_lc_id = lc_info.rrc_lc_info.ul_lc_id[0];
  umts_bearer_context_ptr->ds_umts_bearer_context_dyn_p->d_lc_id = lc_info.rrc_lc_info.dl_lc_id[0];

  pdn_context_ptr = ((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr));

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == FALSE)
  {
    return DS3G_FAILURE;
  }

  pdp_type = pdn_context_ptr->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
   /*--------------------------------------------------------------------------
     in PDP type PPP case do not register with DPM as it will not brindged
     register rx_data_at_u_pdp_cb  with RLC to take SW path
   --------------------------------------------------------------------------*/
   if(pdp_type == DS_UMTS_PDP_PPP)
   {
     rlc_rx_fn_ptr = dsWcdmaBearerCntxt_rx_data_at_u_pdp_cb;
   }
   else if(!bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
   {
     rlc_rx_fn_ptr = NULL;
     ds_bearer_cntx_register_dpm(bearer_context_p);
   }

   register_wm = ds_umts_tlb_is_wm_reg_allowed(subs_id);

#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  /*-----------------------------------------------------------------------
   Register WM with PDCP
  -----------------------------------------------------------------------*/
  if (register_wm)
  {
  dsWcdmaBearerCntxt_register_wm_with_pdcp(
              &lc_info,
              &rx_watermark_p->wm_item,
              rlc_rx_fn_ptr,
              &tx_watermark_p->wm_item,
              bearer_context_p);
  }
  else
  {
    /* Watermark will be registered after loopback mode is closed */
    DS_WCDMA_MSG0_HIGH("UMTS tlb mode active, don't register WM now");
  }

  dsUmtsBearerCntxt_SendQOSIndication(bearer_context_p,
                                      mode_info_ptr->info.gw_ps_call.sys_rat);


 if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA )
  {
    ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT
    ds3gtputmgr_update_uplink_throughput_configure(
                                       ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                               FALSE);
#endif
    ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);
  }



 return DS3G_SUCCESS;

}  /* ds_wcdma_bearer_cntxt_setup_pdp_proto_stack() */

#endif /* FEATURE_WCDMA */

#ifdef FEATURE_TDSCDMA
/*===========================================================================
  FUNCTION DS_TDSCDMA_BEARER_CNTXT_SETUP_PDP_PROTO_STACK

  DESCRIPTION
    This function executes the common code for  both IP and PPP call setup
    for TDSCDMA

  PARAMETERS
   *bearer_context_p - Pointer to the bearer context that we are working with
   *mode_info_ptr    - Pointer to the mode-specific call information provided
                       by Call Manager

  DEPENDENCIES
    None.

  RETURN VALUE
    DS3G_SUCCESS : Successful setup
    DS3G_FAILURE : Setup error

  SIDE EFFECTS
    None.

===========================================================================*/
LOCAL boolean ds_tdscdma_bearer_cntxt_setup_pdp_proto_stack
(
  ds_bearer_context_s             *bearer_context_p,
  const cm_call_mode_info_s_type  *mode_info_ptr
)
{
  ds_bearer_context_wm_s*      tx_watermark_p;
  ds_bearer_context_wm_s*      rx_watermark_p;
  user_plane_lc_info_type      lc_info;       /* lc info from rrc           */
  ds_umts_bearer_context_s    *umts_bearer_context_ptr = NULL;
  uint32                       rab_ul_data_rate;
  sys_modem_as_id_e_type       subs_id = SYS_MODEM_AS_ID_NONE;
  ds_umts_pdp_type_enum_type   pdp_type;
  ds_pdn_context_s            *pdn_context_ptr = NULL;
  void                        (*rlc_rx_fn_ptr)(byte, byte,void*) = NULL;
  boolean                      register_wm = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return FALSE;
  }

  DS_WCDMA_MSG1_HIGH("Bearer Inst:%d Setting TDSCDMA PDP-Proto common stack component",
            bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*-------------------------------------------------------------------------
   Get the Device state information for the call.
 -------------------------------------------------------------------------*/
  tx_watermark_p = &(bearer_context_p->tx_wm);
  rx_watermark_p = &(bearer_context_p->rx_wm);

  /*-------------------------------------------------------------------------
    Set the PS->PDCP watermark.
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p, subs_id, 0);

  /*-------------------------------------------------------------------------
    Cleanup the Tx watermark just in case any data got enqueued after
    the previous call ended.
  -------------------------------------------------------------------------*/
  dsm_empty_queue(&tx_watermark_p->wm_item);

    /*-------------------------------------------------------------------------
    Set the PDCP->PS watermark.
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_dl_wm(bearer_context_p);

  //rx_watermark_p->wm_item.total_rcvd_cnt       = 0;

  /*-------------------------------------------------------------------------
    Set the amount of data dropped due to PDCP rab re-estab process failing
    Hari: We still do not have a place holder for this.
  -------------------------------------------------------------------------*/
  //device_state_ptr->ps_total_tx_data_flushed          = 0;

  //tx_watermark_p->wm_item.total_rcvd_cnt       = 0;

  memset(&lc_info, 0, sizeof(user_plane_lc_info_type));

  if( ds_tds_bearer_cntxt_get_rrc_user_plane_info(
                                  mode_info_ptr, &(lc_info.tdsrrc_lc_info), subs_id ) != DS3G_SUCCESS )
  {
    /* set the bearer to be in UP_DORMANT state */
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT);

    /*-------------------------------------------------------------------------
      Register each enqueue function ptr.
      bearer cntx ptr will be passed as parameter when watermark cb functions
      take a parameter.
    -------------------------------------------------------------------------*/
    (tx_watermark_p->wm_item).each_enqueue_func_ptr =
                               (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (tx_watermark_p->wm_item).each_enqueue_func_data =
                                                    (void*) bearer_context_p;

    return DS3G_FAILURE;
  }
  umts_bearer_context_ptr=(ds_umts_bearer_context_s *)(bearer_context_p->ds_bearer_context_dyn_p-> \
                           vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ]);
  /* validate the umts bearer context pointer*/
  if(!dsUmtsBearerCntxt_validatebearer(umts_bearer_context_ptr))
  {
    DS_WCDMA_MSG0_LOW("UMTS Bearer context passed is Invalid");
    return FALSE;
  }
  /* Store LC_ID for TDSCDMA here */
  umts_bearer_context_ptr->ds_umts_bearer_context_dyn_p->u_lc_id = lc_info.tdsrrc_lc_info.ul_lc_id[0];
  umts_bearer_context_ptr->ds_umts_bearer_context_dyn_p->d_lc_id = lc_info.tdsrrc_lc_info.dl_lc_id[0];

  pdn_context_ptr = ((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr));

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == FALSE)
  {
    return DS3G_FAILURE;
  }

  pdp_type = pdn_context_ptr->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
  /*--------------------------------------------------------------------------
   in PDP type PPP case do not register with DPM as it will not brindged register
   rx_data_at_u_pdp_cb  with RLC to take SW path
  --------------------------------------------------------------------------*/
  if(pdp_type == DS_UMTS_PDP_PPP)
  {
    rlc_rx_fn_ptr = dsWcdmaBearerCntxt_rx_data_at_u_pdp_cb;
  }
  else if(!bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
  {
    rlc_rx_fn_ptr = NULL;
    ds_bearer_cntx_register_dpm(bearer_context_p);
  }

  register_wm = ds_umts_tlb_is_wm_reg_allowed(subs_id);

  /* Register the DL cback fn, so that from test framweork packets can be dirctly enqueued in the Rx Wmk */
#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  
  /*-----------------------------------------------------------------------
   Register WM with PDCP
  -----------------------------------------------------------------------*/
  if (register_wm)
  {
  dsWcdmaBearerCntxt_register_wm_with_pdcp(
              &lc_info,
              &rx_watermark_p->wm_item,
              rlc_rx_fn_ptr,
              &tx_watermark_p->wm_item,
              bearer_context_p);
  }
  else
  {
    /* Watermark will be registered after loopback mode is closed */
    DS_WCDMA_MSG0_HIGH("UMTS tlb mode active, don't register WM now");
  }

#ifdef FEATURE_SEGMENT_LOADING
   if(ds_3gpp_ps_tds_tbl == NULL)
   {
     DS_WCDMA_MSG0_ERROR("TDS not active");
     return DS3G_FAILURE;
   }
#endif /* FEATURE_SEGMENT_LOADING */


  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode
                 == SYS_SYS_MODE_TDS)
  {
#ifdef FEATURE_SEGMENT_LOADING
      rab_ul_data_rate = ITDSCDMA_tdsrrc_get_ul_data_rate_for_rab(
                           ds_3gpp_ps_tds_tbl,
                           bearer_context_p->ds_bearer_context_dyn_p->nsapi,
                           ds3g_get_ps_subs_id());
#else
      rab_ul_data_rate = tdsrrc_get_ul_data_rate_for_rab(
                           bearer_context_p->ds_bearer_context_dyn_p->nsapi,
                           ds3g_get_ps_subs_id());
#endif  /* FEATURE_SEGMENT_LOADING */
      ds_bearer_cntxt_td_proc_cmd(bearer_context_p->ds_bearer_context_dyn_p->nsapi,
                                  rab_ul_data_rate,
                                  ds3g_get_ps_subs_id());
  }

  dsUmtsBearerCntxt_SendQOSIndication(bearer_context_p,
                                      mode_info_ptr->info.gw_ps_call.sys_rat);
 return DS3G_SUCCESS;

}  /* ds_tdscdma_bearer_cntxt_setup_pdp_proto_stack() */
#endif /* FEATURE_TDSCDMA */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_PDP_IP_TX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received to the lower layers for
  pdp-ip call.
  This function is registered with ps_iface for a PDP-IP call

PARAMETERS
  *phys_link_ptr   - Interface ptr for the call.
  **item_ptr       - Payload
  *meta_info_ptr   - Not used.
  *pkt_instance    - Call instance.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_pdp_ip_tx_um_data_cb
(
  ps_phys_link_type *phys_link_ptr,
  dsm_item_type     **item_ptr,
  ps_meta_info_type *meta_info_ptr,
  void              *tx_info_ptr
)
{
  ds_bearer_context_s                  *bearer_context_p;
  ds_bearer_context_wm_s               *tx_watermark_ptr;
  sys_modem_as_id_e_type                subs_id = SYS_MODEM_AS_ID_NONE;
  boolean                               back_pressure_removal = TRUE;
  ds_bearer_cntxt_tput_opt_struct_type  tput_opt_params;
  errno_enum_type                       ps_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  bearer_context_p = (ds_bearer_context_s*)tx_info_ptr;

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    PS_META_INFO_FREE(&meta_info_ptr);
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
    return;
  }

  subs_id = bearer_context_p->ds_bearer_context_dyn_p->subs_id;

  /*-------------------------------------------------------------------------
    Get PDCP state information for call.
  -------------------------------------------------------------------------*/
  tx_watermark_ptr = &(bearer_context_p->tx_wm);

  if(ds_3gpp_nv_get_tput_opt_params_present_in_nv(subs_id))
  {
    tput_opt_params = ds_3gpp_nv_get_back_pressure_params(subs_id);
    back_pressure_removal = tput_opt_params.back_pressure_removal;
  }

  /*-------------------------------------------------------------------------
    Check for uplink Acks and increment the ack counter.
  -------------------------------------------------------------------------*/
  if(TRUE == back_pressure_removal &&
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
    
    /* Ue is in R99 , flow control is disabled, enqueue acks until 25000*/
    if( (tx_watermark_ptr->wm_item.hiwater_func_ptr == NULL ) &&
        (tx_watermark_ptr->wm_item.dont_exceed_cnt == DSWPSD_UL_WM_DNE_0_7M ) &&
        (tx_watermark_ptr->wm_item.current_cnt 
           < DSWPSD_UL_WM_0_7M_BIDIR_BUFFER) )
    {
      dsm_enqueue_special(&tx_watermark_ptr->wm_item, 
                          item_ptr,
                          DSM_WM_ENQ_RULE_IGNORE_DNE);
    }
    else
    {
      dsm_enqueue(&tx_watermark_ptr->wm_item, item_ptr);
    }
  }
  else
  {
     dsm_enqueue(&tx_watermark_ptr->wm_item, item_ptr);
  }

  /*-------------------------------------------------------------------------
    Enqueue this item to PDCP
  -------------------------------------------------------------------------*/
#ifdef DS_3GPP_RX_TX_DBG_MSG
  DS_WCDMA_MSG1_HIGH(
     "ip tx um data cb. Bearer Instance:%d ",
     (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
#endif /* DS_3GPP_RX_TX_DBG_MSG */

#ifdef FEATURE_DATA_FLOW_MGMT
  ds3g_burst_flow_compute_avg_latency(&bearer_context_p->flow_control_info);
#endif /* FEATURE_DATA_FLOW_MGMT */

  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);

  PS_META_INFO_FREE(&meta_info_ptr);

} /* dsWcdmaBearerCntxt_pdp_ip_tx_um_data_cb() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_PDP_IP_RX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received on Um interface to ps_iface for
  pdp-ip call.

PARAMETERS
  sig           - Rx signal for call
  *user_data_p  - stores bearer context information

DEPENDENCIES
  None.

RETURN VALUE
   TRUE - Success
   FALSE - Failure

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL boolean dsWcdmaBearerCntxt_pdp_ip_rx_um_data_cb
(
  ps_sig_enum_type sig,
  void             *user_data_p
)
{
  dsm_item_type          *item_ptr = NULL;          /* Pointer to DSM item         */
  ds_bearer_context_wm_s *rx_watermark_ptr = NULL;
  int32                  ps_handle = DS_3GPP_BEARER_CTXT_INVALID_HANDLE;
  ds_bearer_context_s    *bearer_context_p = NULL;  /* Pointer to bearer context   */
  boolean                result = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Get the bearer context from the ps handle
  -------------------------------------------------------------------------*/
  ps_handle = (int32)user_data_p;
  bearer_context_p = ds_3gpp_bearer_cntxt_get_bearer_from_ps_handle(ps_handle);

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return FALSE;
  }

  if(bearer_context_p->ds_bearer_context_dyn_p->ps_handle != ps_handle)
  {
    DS_WCDMA_MSG2_LOW("Bearer ps handle is not valid: "
                       "original handle %d, current handle %d. "
                       "No attempt made to dequeue Um data to PS",
                       ps_handle,
                       bearer_context_p->ds_bearer_context_dyn_p->ps_handle);
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return FALSE;
  }

#ifdef DS_3GPP_RX_TX_DBG_MSG
  DS_WCDMA_MSG1_HIGH(
     "ip rx um data cb. Bearer Instance:%d ",
     (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
#endif /* DS_3GPP_RX_TX_DBG_MSG */

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
    rx_watermark_ptr = &(bearer_context_p->rx_wm);

    /*-----------------------------------------------------------------------
      The PDCP wm item has IP packets queued up. De-queue and send it on the
      correct physlink.
    -----------------------------------------------------------------------*/
    item_ptr = (dsm_item_type *)dsm_dequeue(&(rx_watermark_ptr->wm_item));

    if (item_ptr != NULL)
    {
#ifdef DS_3GPP_RX_TX_DBG_MSG
      DS_WCDMA_MSG1_HIGH(
         "ip rx um data cb. Got data, Bearer Instance:%d ",
         (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
#endif /* DS_3GPP_RX_TX_DBG_MSG */
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

} /* dsWcdmaBearerCntxt_pdp_ip_rx_um_data_cb() */

/*===========================================================================
  FUNCTION DS_TDS_WCDMA_BEARER_CNTXT_SETUP_PDP_IP_PROTO_STACK

  DESCRIPTION
    This function initializes the protocol stack components specific to
    WCMDA for a PDP type IP call

  PARAMETERS
   *bearer_context_p - Pointer to the bearer context that we are working on
   *mode_info_ptr    - Pointer to the mode-specific call information provided
                       by Call Manager

  DEPENDENCIES
    None.

  RETURN VALUE
    DS3G_SUCCESS : Successful setup
    DS3G_FAILURE : Setup error

  SIDE EFFECTS
    None.

===========================================================================*/
LOCAL boolean ds_tds_w_cdma_bearer_cntxt_setup_pdp_ip_proto_stack
(
  ds_bearer_context_s            *bearer_context_p,
  const cm_call_mode_info_s_type *mode_info_ptr
)
{
  boolean                                return_type = DS3G_SUCCESS;
  ps_phys_link_link_protocol_handle_type protocol_handle;
  int                                    ret_val = 0;
  int16                                  ps_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  DS_WCDMA_MSG2_HIGH("Bearer Inst:%d Setting Mode %d spec:PDP-IP stack component",
            bearer_context_p->ds_bearer_context_dyn_p->call_mode,
            (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
  
#ifdef FEATURE_WCDMA
  if( bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA )
  {
    return_type = ds_wcdma_bearer_cntxt_setup_pdp_proto_stack(bearer_context_p,
                                                              mode_info_ptr);
  }
#endif /* FEATURE_WCDMA*/
#ifdef FEATURE_TDSCDMA
  else if( bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS )
  {
    return_type = ds_tdscdma_bearer_cntxt_setup_pdp_proto_stack(bearer_context_p,
                                                                mode_info_ptr);
  }
#endif /* FEATURE_TDSCDMA */
  else
  {
    return DS3G_FAILURE;
  }

  if (return_type == DS3G_FAILURE)
  {
    DS_WCDMA_MSG0_LOW("Could not set up lower layers. Call is declared to be dormant");
  }

  memset(&protocol_handle, 0x0, sizeof(ps_phys_link_link_protocol_handle_type));

  /*-------------------------------------------------------------------------
    Set the Tx function
  -------------------------------------------------------------------------*/
  if (ds_3gpp_nv_manager_get_data_shark() == TRUE)
  {
    DS_WCDMA_MSG0_HIGH("DS_3GPP_DATA_SHARK_ENABLE NV enabled. "
                       "Registering loopback tx cb");
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                 ds_3gpp_loopback_pdp_data_ctrl_setup_cb,
                                 bearer_context_p);
  }
  else
  {
    ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                 dsWcdmaBearerCntxt_pdp_ip_tx_um_data_cb,
                                 bearer_context_p);
  }


  /*---------------------------------------------------------------------------
   * Set the link layer protocol to be NONE and fill the protocol handle
   * correctly so that when a packet is input into this phys link, it is routed
   * to the approp iface.
  ---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------
   * We are filling the protocol handle structure with the iface pointers
  ---------------------------------------------------------------------------*/

  if(!ds_3gpp_pdn_cntx_validate_pdn_context((ds_pdn_context_s*)
                       (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr)))
  {
    return FALSE;
  }

  if(((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr))->
     ds_pdn_context_dyn_p->
     ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE] != NULL)
  {
    protocol_handle.none_handle.handle.ip_proto_handle.v4_iface_ptr
      = &(((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr))->
          ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE]->ps_iface);
  }

  if(((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr))->
     ds_pdn_context_dyn_p->
     ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE] != NULL)
  {
    protocol_handle.none_handle.handle.ip_proto_handle.v6_iface_ptr =
          &(((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr))->
            ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE]->ps_iface);
  }

  protocol_handle.none_handle.high_protocol
    = PS_PHYS_LINK_HIGHER_LAYER_PROTOCOL_IP;

  /*---------------------------------------------------------------------------
    Set the link layer protocol to NONE and pass the protocol handle into the
    function that will do this. The protocol handle has just been populated
    with the higher layer protocol and the pointers to the ifaces that might
    communicate over this physlink.
  ---------------------------------------------------------------------------*/
  (void) ps_phys_link_set_link_protocol(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                        PS_PHYS_LINK_LINK_PROTOCOL_NONE,
                                        protocol_handle);

  /*-------------------------------------------------------------------------
    Set the handler for the downlink signal for the call.
    PS handles are assigned when the bearer is allocated, and invalidated
    when the bearer is deallocated. The original handle is passed to the
    callback to make sure that only bearers with valid handles are processed.
  -------------------------------------------------------------------------*/
  (void)ps_set_sig_handler(bearer_context_p->ds_bearer_context_dyn_p->rx_sig,
                  dsWcdmaBearerCntxt_pdp_ip_rx_um_data_cb,
                  (void *)bearer_context_p->ds_bearer_context_dyn_p->ps_handle);

  /*-------------------------------------------------------------------------
    Enable the rx signal.
  -------------------------------------------------------------------------*/
  ps_enable_sig(bearer_context_p->ds_bearer_context_dyn_p->rx_sig);

  /*-------------------------------------------------------------------------
    Enable rm_flow_enabled
  -------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->rm_flow_enabled = TRUE;

  /*-------------------------------------------------------------------------
    Posting Bearer Up indication
  -------------------------------------------------------------------------*/
  if(dsUmtsBearerCntxt_activation(bearer_context_p) == DS3G_FAILURE)
  {
    DS_WCDMA_MSG0_ERROR("Bearer activation failed for UMTS_TECH, "
                        "Freeing resources, bring down physlink");
    ret_val = ps_phys_link_down_cmd(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                    &ps_errno,
                                    NULL);
    if((ret_val == 0) || (ps_errno != DS_EWOULDBLOCK))
    {
      DS_WCDMA_MSG0_LOW("Clean-up already in progress ");
    }
  }

  return DS3G_SUCCESS;

}  /* ds_tds_w_cdma_bearer_cntxt_setup_pdp_ip_proto_stack() */

/*===========================================================================
  FUNCTION DS_TDS_W_CDMA_BEARER_CNTXT_SETUP_PDP_PPP_PROTO_STACK

  DESCRIPTION
    This function initializes the protocol stack components specific to
    WCMDA for a PDP type PPP call

  PARAMETERS
   *bearer_context_p - Pointer to the bearer context that we are working on
   *mode_info_ptr    - Pointer to the mode-specific call information provided
                       by Call Manager

  DEPENDENCIES
    None.

  RETURN VALUE
    DS3G_SUCCESS : Successful setup
    DS3G_FAILURE : Setup error

  SIDE EFFECTS
    None.

===========================================================================*/
LOCAL boolean ds_tds_w_cdma_bearer_cntxt_setup_pdp_ppp_proto_stack
(
  ds_bearer_context_s            *bearer_context_p,
  const cm_call_mode_info_s_type *mode_info_ptr
)
{
  boolean return_type = DS3G_FAILURE;
  int                        ret_val = 0;
  int16                      ps_errno = 0;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_WCDMA_MSG2_HIGH("Bearer Inst:%d Setting W/T %d mode spec.PDP-PPP stack component",
            bearer_context_p->ds_bearer_context_dyn_p->call_mode,
            (uint32)bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);

  /*---------------------------------------------------------------------------
    Call the function that sets up the lower layers. This is the common stuff
    that has to happen in both PPP and IP type calls.
  ---------------------------------------------------------------------------*/
#ifdef FEATURE_WCDMA
  if( bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA )
  {
    return_type = ds_wcdma_bearer_cntxt_setup_pdp_proto_stack(bearer_context_p, mode_info_ptr);
  }
#endif /*FEATURE_WCDMA*/
#ifdef FEATURE_TDSCDMA
  else if( bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS )
  {
    return_type = ds_tdscdma_bearer_cntxt_setup_pdp_proto_stack(bearer_context_p, mode_info_ptr);
  }
#endif /* FEATURE_TDSCDMA */
  else
  {
    return DS3G_FAILURE;
  }

  if (return_type == DS3G_FAILURE)
  {
    DS_WCDMA_MSG0_HIGH("Could not set up lower layers. Call is declared to be dormant");
  }
/*-------------------------------------------------------------------------
    Posting Bearer Up indication
  -------------------------------------------------------------------------*/
  if(dsUmtsBearerCntxt_activation(bearer_context_p) == DS3G_FAILURE)
  {
    DS_WCDMA_MSG0_ERROR ("Bearer activation failed for UMTS_TECH, "
                         "Freeing resources");
    ret_val = ps_phys_link_down_cmd(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                    &ps_errno,
                                    NULL);
    if((ret_val == 0) || (ps_errno != DS_EWOULDBLOCK))
    {
      DS_WCDMA_MSG0_LOW("Clean-up already in progress ");
    }
  }
  return DS3G_SUCCESS;

}  /* ds_tds_w_cdma_bearer_cntxt_setup_pdp_ppp_proto_stack() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_ACTIVATECNF()

DESCRIPTION
  This is the function that is registered for WCDMA in the umts_bearer context
  function table. The registration happens in wcdma_init.
  This function is called when the call_connected_hdlr is invoked from DSMGR.
  This function is responsible for setting up the lower layers.

PARAMETERS
  *bearer_context_p - This is the bearer to be set up
  *mode_info_p      - The CM call mode info structure
  *end_params_p     - End params structure that is to be populated if there
                      is a failure somewhere.

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS or DS3G_FAILURE depending on the status of setting up lower
  layers.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL boolean ds_tds_w_cdma_bearer_cntxt_activate_cnf
(
  ds_bearer_context_s            *bearer_context_p,
  const cm_call_mode_info_s_type *mode_info_p,
  cm_end_params_s_type           *end_parms_p
)
{
  ds_umts_pdp_type_enum_type    pdp_type;
  boolean                       result;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  ds_bearer_cntxt_tput_opt_struct_type tput_opt_params;
/*--------------------------------------------------------------------------*/


  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    We need to know what kind of PDP call we originated in the first place.
    This is required for configuring the PROTOCOL stacks appropriately
  -------------------------------------------------------------------------*/
  if(!ds_3gpp_pdn_cntx_validate_pdn_context((ds_pdn_context_s*)
                (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr)))
  {
    return FALSE;
  }

  if(!ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(bearer_context_p->
                                             ds_bearer_context_dyn_p->pdn_context_ptr,
                                             &subs_id))
  {
    return FALSE;
  }
  tput_opt_params = ds_3gpp_nv_get_back_pressure_params(subs_id);
  pdp_type = ((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p
                                  ->pdn_context_ptr))->
    ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
  switch(pdp_type)
  {
    case DS_UMTS_PDP_PPP:
      result = ds_tds_w_cdma_bearer_cntxt_setup_pdp_ppp_proto_stack
                                                 (
                                                   bearer_context_p,
                                                   mode_info_p
                                                 );
      break;

    case DS_UMTS_PDP_IP :
#ifdef FEATURE_DATA_PS_IPV6
    case DS_UMTS_PDP_IPV6:
    case DS_UMTS_PDP_IPV4V6:
#endif
      {
        /*-------------------------------------------------------------------
          Setup IP stack only if it is not a Secondary call
        --------------------------------------------------------------------*/
        result = ds_tds_w_cdma_bearer_cntxt_setup_pdp_ip_proto_stack
                                                  (
                                                    bearer_context_p,
                                                    mode_info_p
                                                  );
      }

      DS_WCDMA_MSG3_HIGH("Throughput opt_params: BP_removal: %d, "
                         "Timer_val: %d, threshold_ack: %d",
                         tput_opt_params.back_pressure_removal,
                         tput_opt_params.timer_val,
                         tput_opt_params.tcp_threshold_ack_cnt);

      /*---------------------------------------------------------------------
        Allocate timer handle for dynamic back pressure disable
      ---------------------------------------------------------------------*/
      bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure
        = ps_timer_alloc(ds_bearer_cntxt_tmr_dyn_back_pressure_exp_hdlr,
                         (void *)bearer_context_p);

      if (PS_TIMER_INVALID_HANDLE ==
          bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure)
      {
        DS_WCDMA_MSG3_HIGH("Invalid PS timer handle for bearer inst: %d",
                           (uint32)bearer_context_p->ds_bearer_context_dyn_p
                           ->client_data_ptr, 0, 0);
      }

      bearer_context_p->ds_bearer_context_dyn_p->tcp_dl_ack_counter
        = bearer_context_p->ds_bearer_context_dyn_p->last_tcp_dl_ack_count = 0;

      break;

    default:
      DS_WCDMA_MSG2_ERROR(
         "Unknown PDP Type: pdp type:%d Bearer Inst:%d",
         pdp_type, bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr);
      result = DS3G_FAILURE;
  }
  return result;
} /*ds_tds_w_cdma_bearer_cntxt_activate_cnf()*/

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DS_W_TDS_CDMA_BEARER_CNTXT_GET_SO_MASK_FOR_HS_STATUS

DESCRIPTION
  This function returns the SO MASK corresponding to the passed HS status.

PARAMETERS
  hs_status

DEPENDENCIES
  None.

RETURN VALUE
  so mask

SIDE EFFECTS
  None.
===========================================================================*/
uint64 ds_w_tds_cdma_bearer_cntxt_get_so_mask_for_hs_status
(
  sys_hs_ind_e_type hs_status
)
{
  uint64 so_mask = 0;

  switch (hs_status)
  {
    case SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL:
      break;
    case SYS_HS_IND_HSDPA_SUPP_CELL:
      so_mask |= DS_SYS_SO_EX_3GPP_HSDPA;
      break;
    case SYS_HS_IND_HSUPA_SUPP_CELL:
      so_mask |= DS_SYS_SO_EX_3GPP_HSUPA;
      break;
    case SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_HSDPA | DS_SYS_SO_EX_3GPP_HSUPA);
      break;
    case SYS_HS_IND_HSDPAPLUS_SUPP_CELL:
      so_mask |= DS_SYS_SO_EX_3GPP_HSDPAPLUS;
      break;
    case SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_HSDPAPLUS | DS_SYS_SO_EX_3GPP_HSUPA);
      break;
    case SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL:
      so_mask |= DS_SYS_SO_EX_3GPP_DC_HSDPAPLUS;
      break;
    case SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_DC_HSDPAPLUS | DS_SYS_SO_EX_3GPP_HSUPA);
      break;
    case SYS_HS_IND_DC_HSDPAPLUS_DC_HSUPA_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_DC_HSDPAPLUS | DS_SYS_SO_EX_3GPP_DC_HSUPA);
      break;
    case SYS_HS_IND_HSDPAPLUS_64QAM_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_HSDPAPLUS|DS_SYS_SO_EX_3GPP_64_QAM);
      break;
    case SYS_HS_IND_HSDPAPLUS_64QAM_HSUPA_SUPP_CELL:
      so_mask |= (DS_SYS_SO_EX_3GPP_HSDPAPLUS | DS_SYS_SO_EX_3GPP_HSUPA
                  |DS_SYS_SO_EX_3GPP_64_QAM);
      break;
    default:
      DS_WCDMA_MSG1_LOW("hs_status value [%d] unrecognized!", hs_status);
      break;
  }

  return so_mask;
} /* ds_wcdma_bearer_cntxt_get_so_mask_for_hs_status */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_SO_MASK

DESCRIPTION
  This function returns the SO MASK for WCDMA.

PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  so mask

SIDE EFFECTS
  None.
===========================================================================*/
uint64 ds_wcdma_bearer_cntxt_get_so_mask
(
  sys_modem_as_id_e_type  subs_id
)
{
  uint64 so_mask = DS_SYS_SO_EX_3GPP_WCDMA;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cm_cell_srv_ind_s_type cell_srv_ind = dsUmtsBearerCntxtGetCellSrvInd(subs_id);

  so_mask |= ds_w_tds_cdma_bearer_cntxt_get_so_mask_for_hs_status(
               ds_wcdma_bearer_cntxt_get_hs_status_from_cell_srv_ind(
                  &cell_srv_ind));

  return so_mask;
} /* ds_wcdma_bearer_cntxt_get_so_mask */

/*===========================================================================
FUNCTION DS_TDSCDMA_BEARER_CNTXT_GET_SO_MASK

DESCRIPTION
  This function returns the SO MASK for TDSCDMA.

PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  so mask

SIDE EFFECTS
  None.
===========================================================================*/
uint64 ds_tds_bearer_cntxt_get_so_mask
(
  sys_modem_as_id_e_type  subs_id
)
{
  uint64 so_mask = DS_SYS_SO_EX_3GPP_TDSCDMA;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cm_cell_srv_ind_s_type cell_srv_ind = dsUmtsBearerCntxtGetCellSrvInd(subs_id);

  so_mask |= ds_w_tds_cdma_bearer_cntxt_get_so_mask_for_hs_status(
               ds_wcdma_bearer_cntxt_get_hs_status_from_cell_srv_ind(
                  &cell_srv_ind));

  return so_mask;
} /* ds_tdscdma_bearer_cntxt_get_so_mask */

#endif /* FEATURE_DATA_WLAN_MAPCON */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_DEREGISTER_WM_WITH_PDCP()

DESCRIPTION
  This function is used to deregister watermarks with PDCP

PARAMETERS
  *bearer_context_p - This is the bearer to work with.

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS or DS3G_FAILURE depending on the status of setting up lower
  layers.

SIDE EFFECTS
  None.

===========================================================================*/
void dsWcdmaBearerCntxt_deregister_wm_with_pdcp
(
  ds_bearer_context_s* bearer_context_p
)
{
  comp_cmd_type *pdcp_cmd;
  sys_modem_as_id_e_type        subs_id = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Deregister the PDCP UL & DL watermarks
  -------------------------------------------------------------------------*/
  if((pdcp_cmd = comp_get_cmd_buf()) == NULL)
  {
    /*-----------------------------------------------------------------------
      No free command buffers available, log an error.
    -----------------------------------------------------------------------*/
    DS_WCDMA_MSG0_ERROR( "No cmd buffers available in COMP task free cmd q" );
    return;
  }

  pdcp_cmd->hdr.cmd_id = PDCP_DEREG_WM;
  pdcp_cmd->hdr.subs_id = subs_id;
  pdcp_cmd->cmd.pdcp_dereg_wm.ps_to_pdcp_wm_ptr = &(bearer_context_p->tx_wm.wm_item);
  pdcp_cmd->cmd.pdcp_dereg_wm.pdcp_to_ps_wm_ptr = &(bearer_context_p->rx_wm.wm_item);
  pdcp_cmd->cmd.pdcp_reg_wm.bearer_call_mode
    = bearer_context_p->ds_bearer_context_dyn_p->call_mode;

  comp_put_cmd(pdcp_cmd);
} /* dsWcdmaBearerCntxt_deregister_wm_with_pdcp() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_CLEANUP_PDCP_WATERMARKS()

DESCRIPTION
  This function is used to clean up watermarks with PDCP

PARAMETERS
  *bearer_context_p - This is the bearer to work with.

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS or DS3G_FAILURE depending on the status of setting up lower
  layers.

SIDE EFFECTS
  None.

//VRK:TODO move this to bearer context, not mode specific
===========================================================================*/
void dsWcdmaBearerCntxt_cleanup_pdcp_watermarks
(
  ds_bearer_context_s* bearer_context_p
)
{

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  DS_WCDMA_MSG2_HIGH("WM_DEBUG in UL: DNE discard count: %d Highest_cnt: %d",
            bearer_context_p->tx_wm.wm_item.dne_discard_pkt_cnt,
            bearer_context_p->tx_wm.wm_item.highest_cnt);
  ds_tds_w_cdma_bearer_cntxt_empty_watermark(bearer_context_p);
  dsm_empty_queue ( &(bearer_context_p->rx_wm.wm_item) );

} /* dsWcdmaBearerCntxt_cleanup_pdcp_watermarks() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_DEACTIVATECNF()

DESCRIPTION
  This is the function that is registered for WCDMA in the umts_bearer context
  function table. The registration happens in wcdma_init.
  This function is called when the call_ended_hdlr is invoked from DSMGR.
  This function is responsible for cleanin up the lower layers.

PARAMETERS
  *bearer_context_p - This is the bearer to be set up

DEPENDENCIES
  None.

RETURN VALUE
  DS3G_SUCCESS or DS3G_FAILURE depending on the status of setting up lower
  layers.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void ds_tds_w_cdma_bearer_cntxt_deactivate_cnf
(
  ds_bearer_context_s* bearer_context_p
)
{
  sys_modem_as_id_e_type          subs_id = SYS_MODEM_AS_ID_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  (void)ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id);

  /*-------------------------------------------------------------------------
    Deregister UL and DL watermarks with PDCP
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_deregister_wm_with_pdcp(bearer_context_p);

  if(bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm)
  {
    ds_bearer_cntx_deregister_dpm(bearer_context_p);
  }

  bearer_context_p->ds_bearer_context_dyn_p->is_registered_with_dpm = FALSE;

  /*-------------------------------------------------------------------------
    Cleanup UL and DL PDCP watermarks.
  -------------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_cleanup_pdcp_watermarks(bearer_context_p);

  /*-------------------------------------------------------------------------
    Free the PS timer allocated for dynamic BP disable
  -------------------------------------------------------------------------*/
  if (PS_TIMER_SUCCESS !=
      ps_timer_free(bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure))
  {
    DS_WCDMA_MSG1_ERROR("Failed to deallocate timer for bearer inst: %d",
                        (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));
  }


  /*-------------------------------------------------------------------------
    Clear the timer handle as stale handle could cause issues during inter-RAT
  --------------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->tmr_hndl_dyn_back_pressure = PS_TIMER_INVALID_HANDLE;

  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA )
  {
    ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT
    ds3gtputmgr_update_uplink_throughput_configure(
                                       ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                               FALSE);
#endif
    ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);
  }


} /* ds_tds_w_cdma_bearer_cntxt_deactivate_cnf() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_PDP_PPP_RX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received on Um interface to Rm PPP layer for
  PDP-PPP call.

PARAMETERS
  user_ptr - Store pointer to bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL dsm_item_type* dsWcdmaBearerCntxt_pdp_ppp_rx_um_data_cb
(
  void *user_ptr
)
{
  ds_bearer_context_s* bearer_context_p; // Pointer to bearer context
  dsm_item_type*       dsm_item_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  bearer_context_p = (ds_bearer_context_s*)user_ptr;

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
    return NULL;
  }

  /*-------------------------------------------------------------------------
    RM_FLOW_ENABLED flag is set to TRUE always for Embedded calls.
    For Laptop calls, this flag is set based on the flow events happening on
    the Rm Phys link
  -------------------------------------------------------------------------*/
  if(bearer_context_p->ds_bearer_context_dyn_p->rm_flow_enabled == TRUE)
  {
    /*-------------------------------------------------------------------------
     The PDCP wm item has IP packets queued up.
    -------------------------------------------------------------------------*/
    dsm_item_p =  (dsm_item_type*)dsm_dequeue(&(bearer_context_p->rx_wm.wm_item));
  }
  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_dl_crit_sect);
  return dsm_item_p;

} /* dsWcdmaBearerCntxt_pdp_ppp_rx_um_data_cb() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_PDP_PPP_TX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received on Rm interface to Um  for
  pdp-PPP call.
  This function is registered with PPP for a PDP-PPP call (TE+MT originated)

PARAMETERS
  item_ptr - Payload
  user_ptr - pointer to the bearer context on which data is available

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void dsWcdmaBearerCntxt_pdp_ppp_tx_um_data_cb
(
  dsm_item_type **item_ptr,
  ps_meta_info_type *meta_info_ptr, /* not used */
  void          *user_ptr
)
{
  ds_bearer_context_s             *bearer_context_p;
  ds_pdn_context_s*                pdn_cntx_p = NULL;
  sys_modem_as_id_e_type           subs_id  = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the bearer context pointer from the user data passed in */
  bearer_context_p = (ds_bearer_context_s*)user_ptr;

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_ul_crit_sect);

  if (!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
    return;
  }
  pdn_cntx_p = (ds_pdn_context_s*)
               bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr;


  if(! ds_3gpp_pdn_cntxt_get_subs_id_from_pdn(pdn_cntx_p,&subs_id))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
    return;
  }
  /*-------------------------------------------------------------------------
    If data available indication has not been sent to CM and we are
    suspended, send it and set the boolean dsumtsps_data_available to TRUE.
    This is done so that the indication is sent to CM only once during
    inter-RAT change.
  -------------------------------------------------------------------------*/
  if (ds3gpp_get_ps_data_suspend_flag(subs_id) == TRUE)
  {
    //VRK:TODO Does this get called in PS task context, do we a critical section?
    if((!ds3gpp_get_ps_data_available_flag(subs_id)) && 
         ds3gpp_is_ps_data_avail_ind_allowed(subs_id,
              ds_3gpp_pdn_cntxt_is_ims_pdn((void*)pdn_cntx_p)))
    {
      DS_WCDMA_MSG1_HIGH("Data available Ind sent to CM for Sub id %d", subs_id);
      ds3g_msh_ps_data_available_ind(subs_id);
      ds3gpp_set_ps_data_available_flag(TRUE, subs_id);
    }

    /*-----------------------------------------------------------------------
      Disable flow on all active interfaces to prevent flooding of data on the
      PDCP UL watermark.
    -----------------------------------------------------------------------*/
    ds_bearer_cntxt_update_phys_link_flow_status(TRUE, subs_id);    
  }

  /* Get the bearer context pointer from the user data passed in */
  bearer_context_p = (ds_bearer_context_s*)user_ptr;

  /*-------------------------------------------------------------------------
    Enqueue this to PDCP
  -------------------------------------------------------------------------*/
  dsm_enqueue(&(bearer_context_p->tx_wm.wm_item), item_ptr);

#ifdef FEATURE_DATA_FLOW_MGMT
  ds3g_burst_flow_compute_avg_latency(&bearer_context_p->flow_control_info);
#endif /* FEATURE_DATA_FLOW_MGMT */

  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_ul_crit_sect);
  return;

} /* dsWcdmaBearerCntxt_pdp_ppp_tx_um_data_cb() */

#if defined(FEATURE_TDSCDMA) || defined(FEATURE_DATA_WCDMA_PS)
/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_CONFIGURE_PDP_PPP

DESCRIPTION
  This is the function that is registered for WCDMA in the umts_bearer context
  function table. The registration happens in wcdma_init.
  This function is called when the PPP call is to be configured. RMSM calls
  PDN context which in turn calls the UMTS bearer context function to conf
  PPP call. This function is called from UMTS bearer context.
  This is used to fill up the PPP options with tx and rx function call backs.

PARAMETERS
  *bearer_context_p  - This is the bearer to be set up
  *ppp_config_info_p - The PPP config info pointer to be filled up.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
LOCAL void ds_tds_w_cdma_bearer_cntxt_configure_pdp_ppp
(
  ds_bearer_context_s *bearer_context_p,
  ppp_dev_opts_type   *ppp_config_info_p
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ppp_config_info_p->rx_f_ptr       = dsWcdmaBearerCntxt_pdp_ppp_rx_um_data_cb;
  ppp_config_info_p->tx_f_ptr       = (tx_f_ptr_type)dsWcdmaBearerCntxt_pdp_ppp_tx_um_data_cb;
} /*ds_tds_w_cdma_bearer_cntxt_configure_pdp_ppp*/

/*===========================================================================
FUNCTION  ds_tds_w_cdma_bearer_cntxt_rab_reestab_ind_hdlr

DESCRIPTION
  This function is called by 3G DSManager when the RAB is re-established
  A New PDCP connection is setup by SM layer. Bind the PDCP to the Watermarks
  so that data transfers can take place

PARAMETERS
  call_id,        - Call ID
  *mode_info_ptr  - Pointer to the mode-specific call information provided by
                    Call Manager
  *end_params     - CM specific call end parameters

DEPENDENCIES
  None

RETURN VALUE
  DS3G_SUCCESS : Re-establishment was successfull PDCP bound to PS watermarks
  DS3G_FAILURE : rab re-establishment cannot be handled

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_tds_w_cdma_bearer_cntxt_rab_reestab_ind_hdlr
(
  ds_bearer_context_s       *bearer_context_p,
  cm_call_mode_info_s_type  *mode_info_ptr
)
{
  user_plane_lc_info_type        lc_info;        /* LC info from RRC      */
  phys_link_state_type           phys_link_state = PHYS_LINK_NULL;
  boolean result = DS3G_FAILURE;
  sys_modem_as_id_e_type          subs_id = SYS_MODEM_AS_ID_NONE;
  void                        (*rlc_rx_fn_ptr)(byte, byte,void*) = NULL;
  ds_umts_pdp_type_enum_type           pdp_type;
  ds_pdn_context_s                    *pdn_context_ptr = NULL;
  boolean                        register_wm = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return DS3G_FAILURE;
  }

  if(mode_info_ptr == NULL)
  {
    return DS3G_FAILURE;
  }

  /*-------------------------------------------------------------------------
    Get PDCP channel information for the new RAB
  -------------------------------------------------------------------------*/
  memset(&lc_info, 0, sizeof(user_plane_lc_info_type));
#ifdef FEATURE_WCDMA
  if( mode_info_ptr->info.gw_ps_call.sys_rat == SYS_RAT_UMTS_RADIO_ACCESS )
  {
    result =
          dsWcdmaBearerCntxt_get_rrc_user_plane_info(mode_info_ptr,&(lc_info.rrc_lc_info),
                                                     subs_id);
  }
#endif /* FEATURE_WCDMA*/
#ifdef FEATURE_TDSCDMA
  else if( mode_info_ptr->info.gw_ps_call.sys_rat == SYS_RAT_TDS_RADIO_ACCESS )
  {
    result =
         ds_tds_bearer_cntxt_get_rrc_user_plane_info(mode_info_ptr,&(lc_info.tdsrrc_lc_info),
                                                     subs_id);
  }
#endif /* FEATURE_TDSCDMA */
  else
  {
    DS_WCDMA_MSG2_ERROR("bearer cntx:%x Invalid sys_rat:%d",
                        bearer_context_p,
                        (mode_info_ptr->info.gw_ps_call.sys_rat));
    return DS3G_FAILURE;
  }

  phys_link_state = PS_PHYS_LINK_GET_STATE(&(bearer_context_p->
                                             ds_bearer_context_dyn_p->phys_link));

  if( result != DS3G_SUCCESS)
  {
    /*--------------------------------------------------------------------------
      Could not set up the lower layers. The action that we wanna take now is
      to set the call in UP_DORMANT and return DS3G_SUCCESS to dsmgr so that the
      call is not torn down.
      We however do not send UP indication on the Phys link
    --------------------------------------------------------------------------*/

    /* set the bearer to be in UP_DORMANT state */
    ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                     DS_BEARER_CONTEXT_STATE_UP_DORMANT);

    /*---------------------------------------------------------------------
      Check phys link state and post phys link up. We cannot blindly post
      flow_enable() as it will not take effect if phys link is in transition
      states.
    ---------------------------------------------------------------------*/
    if (!( ( PHYS_LINK_UP == phys_link_state) ||
           ( PHYS_LINK_DOWN == phys_link_state) ) )
    {
      ps_phys_link_down_ind( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link) );
    }
    /* Enable the flow on the phys link so that the app /  TE can write data */
    ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                     DS_FLOW_UMTS_MASK, FALSE);

    /*-------------------------------------------------------------------------
      Register each enqueue function ptr.
    -------------------------------------------------------------------------*/
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_ptr =
      (wm_cb_type)ds_bearer_cntxt_reorig_dorm_call_cb;
    (bearer_context_p->tx_wm.wm_item).each_enqueue_func_data =
      (void*) bearer_context_p;

    return DS3G_SUCCESS;
  }

   pdn_context_ptr = ((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr));

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_ptr) == FALSE)
  {
    return DS3G_FAILURE;
  }
  pdp_type = pdn_context_ptr->ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;
  if(pdp_type == DS_UMTS_PDP_PPP)
  {
    rlc_rx_fn_ptr = dsWcdmaBearerCntxt_rx_data_at_u_pdp_cb;
  }
  else
  {
    rlc_rx_fn_ptr = NULL;
  }
  /*---------------------------------------------------------------------------
    Register the Watermark with PDCP.
  ---------------------------------------------------------------------------*/
  if(mode_info_ptr->info.gw_ps_call.sys_rat == SYS_RAT_UMTS_RADIO_ACCESS 
#ifdef FEATURE_TDSCDMA
    || mode_info_ptr->info.gw_ps_call.sys_rat == SYS_RAT_TDS_RADIO_ACCESS
#endif /* FEATURE_TDSCDMA */
  )
  {
    register_wm = ds_umts_tlb_is_wm_reg_allowed(subs_id);

    if (register_wm)
    {
    dsWcdmaBearerCntxt_register_wm_with_pdcp(
                &lc_info,
                &(bearer_context_p->rx_wm.wm_item),
                rlc_rx_fn_ptr,  
                &(bearer_context_p->tx_wm.wm_item),
                bearer_context_p);
  }
  else
  {
      DS_WCDMA_MSG0_HIGH("UMTS tlb mode active, don't register WM now");
    }
  }
  else
  {
    DS_WCDMA_MSG2_LOW("Could not reg WM with PDCP, bearer cntx:%x "
                        "Invalid sys_rat:%d",
                        bearer_context_p,
                        (mode_info_ptr->info.gw_ps_call.sys_rat));
    return DS3G_FAILURE;
  }
  /*-------------------------------------------------------------------------
    Change the bearer context state to UP
  -------------------------------------------------------------------------*/
  ds_bearer_cntxt_set_bearer_state(bearer_context_p,
                                   DS_BEARER_CONTEXT_STATE_UP);
  /*-----------------------------------------------------------------------
    Phys Link status needs to be updated If it is already UP no need to post
    again as data rates changes are indicatde via bearer tech change.
  -----------------------------------------------------------------------*/
  if ( !( PHYS_LINK_UP == phys_link_state ) )
  {
    ps_phys_link_up_ind_ex( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                PS_EIC_QOS_NETWORK_L2_LINK_REESTAB_IND );
  }
  /* Enable the flow on the phys link so that the app /  TE can write data */
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                   DS_FLOW_UMTS_MASK,FALSE);

  /*-------------------------------------------------------------------------
    Send PS phys link disable for the bearer if subs is coex flow disabled
  -------------------------------------------------------------------------*/

    if( !ds3gflowmgr_get_coex_data_flow_status(subs_id) )
    {
      ds_3gpp_bearer_cntxt_disable_flow(bearer_context_p,
                                DS_FLOW_COEX_MASK, FALSE);
    }


  dsUmtsBearerCntxt_SendQOSIndication(bearer_context_p,
                                      mode_info_ptr->info.gw_ps_call.sys_rat);

  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA )
  {
    ds3gtputmgr_update_tput_uldl_timer(ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id));
#ifdef FEATURE_DATA_RAVE_SUPPORT
    ds3gtputmgr_update_uplink_throughput_configure(
                                       ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id),
                                               FALSE);
#endif
    ds_3gpp_tput_hdlr_downlink(subs_id, FALSE);
  }

  return ( DS3G_SUCCESS );

} /* ds_tds_w_cdma_bearer_cntxt_rab_reestab_ind_hdlr() */
/*===========================================================================
FUNCTION ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr()

DESCRIPTION
  This is the function that is registered for WCDMA in the umts_bearer context
  function table. The registration happens in wcdma_init.
  This function is called when the context_transfer_ind_hdlr is invoked from
  DSMGR. This function is responsible for setting up the stack in TARGET RAT
  during L-GW interRAT.


PARAMETERS
  bearer_context_p

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
LOCAL void ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr
(
  ds_bearer_context_s  *bearer_context_p
)
{
#ifdef FEATURE_QXDM_WM_TRACING
  uint32                                callid;
#endif /* FEATURE_QXDM_WM_TRACING */
  ds_umts_pdp_type_enum_type           pdp_type;
  user_plane_lc_info_type              lc_info;  /* lc info from rrc */
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_p = NULL;
  uint32                               rab_ul_data_rate = 0;
  boolean                              rrc_user_plane_info_valid = FALSE;
  sys_modem_as_id_e_type               subs_id;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

  if(!ds_3gpp_pdn_cntx_validate_pdn_context((ds_pdn_context_s*)
                  (bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr)))
  {
    return;
  }

  DS_WCDMA_MSG2_HIGH("Setting up stack for WCDMA Radio Layer Inst:0x%x "
                     "subs_id:%d",
                     bearer_context_p, subs_id);

  /*-------------------------------------------------------------------------
    Get RRC user plane info
  -------------------------------------------------------------------------*/
  memset(&lc_info, 0, sizeof(user_plane_lc_info_type));
  rrc_user_plane_info_valid
    = dsWcdmaBearerCntxt_get_rrc_user_plane_info_from_nsapi(
        bearer_context_p->ds_bearer_context_dyn_p->nsapi,
        &(lc_info.rrc_lc_info),
        subs_id);

  /*-------------------------------------------------------------------------
    Check if RB data from W RRC is present for the bearer
  -------------------------------------------------------------------------*/
  if( rrc_user_plane_info_valid == TRUE )
  {
    rb_data_p = ds_wcdma_bearer_cntxt_find_rb_data(lc_info.rrc_lc_info.ul_lc_id[0]);
    if( rb_data_p != NULL )
    {
      rab_ul_data_rate = rb_data_p->rab_ul_data_rate;
    }
  }

  /*---------------------------------------------------------------------
    Set the PS->PDCP watermark.
  ---------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_ps_to_pdcp_wm(bearer_context_p,
                                             subs_id,
                                             rab_ul_data_rate);

  /*---------------------------------------------------------------------
   Set the PDCP->PS watermark.
  ---------------------------------------------------------------------*/
  dsWcdmaBearerCntxt_setup_pdp_dl_wm(bearer_context_p);

  /*---------------------------------------------------------------------
    We need to know what kind of PDP call we are dealing with.
    This is required for configuring the PROTOCOL stacks appropriately
  ---------------------------------------------------------------------*/
  pdp_type = ((ds_pdn_context_s*)(bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr))->
    ds_pdn_context_dyn_p->pdp_profile.context.pdp_type;

  if(pdp_type != DS_UMTS_PDP_PPP)
  {
    if (ds_3gpp_nv_manager_get_data_shark() == TRUE)
    {
      DS_WCDMA_MSG0_HIGH("DS_3GPP_DATA_SHARK_ENABLE NV enabled. "
                         "Registering loopback tx cb");
      ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                   ds_3gpp_loopback_pdp_data_ctrl_setup_cb,
                                   bearer_context_p);
    }
    else
    {
      ps_phys_link_set_tx_function(&(bearer_context_p->ds_bearer_context_dyn_p->phys_link),
                                   dsWcdmaBearerCntxt_pdp_ip_tx_um_data_cb,
                                   bearer_context_p);
    }

#ifdef FEATURE_DATA_LTE
    ds_eps_tlb_validate_set_tlb_tx_function(bearer_context_p);
#endif
    /*-------------------------------------------------------------------------
      Set the handler for the downlink signal for the call.
      PS handles are assigned when the bearer is allocated, and invalidated
      when the bearer is deallocated. The original handle is passed to the
      callback to make sure that only bearers with valid handles are processed.
    -------------------------------------------------------------------------*/
    (void)ps_set_sig_handler(bearer_context_p->ds_bearer_context_dyn_p->rx_sig,
                    dsWcdmaBearerCntxt_pdp_ip_rx_um_data_cb,
                  (void *)bearer_context_p->ds_bearer_context_dyn_p->ps_handle);
#ifdef FEATURE_QXDM_WM_TRACING
    callid = ds_bearer_cntxt_get_call_id(bearer_context_p);
    fc_post_modify_cmd_wm_trace(callid, UNIFIED_3GPP_MODE_HANDLER_CLIENT, UMTS_TECH);
#endif /* FEATURE_QXDM_WM_TRACING */
  }/* (pdp_type != DS_UMTS_PDP_PPP) */

  return;
} /* ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr() */

/*===========================================================================
FUNCTION DS_TDS_W_CDMA_BEARER_CNTXT_EMPTY_WATERMARK()

DESCRIPTION
  This is the function that purges the data in the UL watermark when call end is initiated.

PARAMETERS
  bearer_context_p

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void ds_tds_w_cdma_bearer_cntxt_empty_watermark
(
  ds_bearer_context_s  *bearer_context_p
)
{
#ifdef  FEATURE_WCDMA
  rrc_user_plane_lc_info_type lc_info;
  wcdma_l2_ul_cmd_type        *l2_ul_cmd_p;
#endif /* FEATURE_WCDMA */
  sys_modem_as_id_e_type subs_id = SYS_MODEM_AS_ID_NONE;

  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  if (!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_p, &subs_id))
  {
    return;
  }

 DS_WCDMA_MSG2_HIGH("WM_DEBUG in UL: DNE discard count: %d Highest_cnt: %d",
                     bearer_context_p->tx_wm.wm_item.dne_discard_pkt_cnt,
                     bearer_context_p->tx_wm.wm_item.highest_cnt);

#ifdef FEATURE_WCDMA
  memset(&lc_info, 0, sizeof(rrc_user_plane_lc_info_type));

  if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_WCDMA)
  {
    lc_info.rab_id = bearer_context_p->ds_bearer_context_dyn_p->nsapi;

    /*------------------------------------------------------------------------
      If there are no est_rbs at RRC, empty the WM as RLC has no context.
      Also, if RRC has the rb_id established and returns lc_info, still RLC
      might not have WM registered as DS registers WM on RAB_REESTAB_IND
    ------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
#ifdef FEATURE_DUAL_WCDMA
  if ( (ds_3gpp_ps_wcdma_tbl == NULL) ||
      (IWCDMA_rrc_return_lc_info_for_rab_sub(ds_3gpp_ps_wcdma_tbl, subs_id,
                           &lc_info ) ==  RRC_RAB_NOT_FOUND ) )
#else
  if ( (ds_3gpp_ps_wcdma_tbl == NULL) ||
       ( IWCDMA_rrc_return_lc_info_for_rab(ds_3gpp_ps_wcdma_tbl,
                           &lc_info ) ==  RRC_RAB_NOT_FOUND ) )
#endif
#else
#ifdef FEATURE_DUAL_WCDMA
  if (rrc_return_lc_info_for_rab_sub(subs_id, &lc_info) == RRC_RAB_NOT_FOUND)
#else
    if (rrc_return_lc_info_for_rab(&lc_info) == RRC_RAB_NOT_FOUND)
#endif

#endif /* FEATURE_SEGMENT_LOADING */
    {
      DS_WCDMA_MSG2_ERROR("rab_id: %d not found, emptying WM for bearer_inst: %d",
                           bearer_context_p->ds_bearer_context_dyn_p->nsapi,
                           (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr));
      dsm_empty_queue(&(bearer_context_p->tx_wm.wm_item));
    }
    else
    {
      DS_WCDMA_MSG2_HIGH(
         "Purging data in bearer_inst: %d, bearer state: %d",
         (uint32)(bearer_context_p->ds_bearer_context_dyn_p->client_data_ptr),
         bearer_context_p->ds_bearer_context_dyn_p->state);

#ifdef FEATURE_SEGMENT_LOADING
      if ( (l2_ul_cmd_p = IWCDMA_wcdma_l2_ul_get_cmd_buf(ds_3gpp_ps_wcdma_tbl)) == NULL)
#else
      if ( (l2_ul_cmd_p = wcdma_l2_ul_get_cmd_buf()) == NULL )
#endif  /* FEATURE_SEGMENT_LOADING */
      {
        DS_WCDMA_MSG0_ERROR("Couldn't get L2 UL cmd buf");
        return;
      }

      if(bearer_context_p->tx_wm.wm_queue.cnt > 0)
       {
         /*----------------------------------------------------------------------------
          disbling flow before posting purge comand to ensure that
          no data will be enqueued further
          ----------------------------------------------------------------------------*/
          ds_3gpp_bearer_cntxt_disable_flow( bearer_context_p, DS_FLOW_UMTS_MASK, FALSE);
          l2_ul_cmd_p->cmd_hdr.cmd_id = RLC_UL_PURGE_WM_REQ;
          l2_ul_cmd_p->cmd_data.ul_purge_wm_ind.lc_id[0] = lc_info.ul_lc_id[0];
          l2_ul_cmd_p->cmd_data.ul_purge_wm_ind.num_lc_id = 1;
          l2_ul_cmd_p->cmd_data.ul_purge_wm_ind.num_sdus[0] =
                        bearer_context_p->tx_wm.wm_queue.cnt;
          l2_ul_cmd_p->cmd_data.ul_purge_wm_ind.purge_ind_cb =
                                                &ds_tds_w_cdma_bearer_cntxt_purge_status_cb;
          l2_ul_cmd_p->cmd_data.ul_purge_wm_ind.bearer_context = (void*)bearer_context_p;
        }
#ifdef FEATURE_SEGMENT_LOADING
      IWCDMA_wcdma_l2_ul_put_cmd(ds_3gpp_ps_wcdma_tbl, l2_ul_cmd_p);
#else
      wcdma_l2_ul_put_cmd(l2_ul_cmd_p);
#endif /* FEATURE_SEGMENT_LOADING */
    }
#endif /* FEATURE_WCDMA */
  }
  else if (bearer_context_p->ds_bearer_context_dyn_p->call_mode == SYS_SYS_MODE_TDS)
  {
    dsm_empty_queue(&(bearer_context_p->tx_wm.wm_item));
  }
  else
  {
    DS_WCDMA_MSG1_LOW("Invalid call_mode: %d",
                         bearer_context_p->ds_bearer_context_dyn_p->call_mode);
  }
} /* ds_tds_w_cdma_bearer_cntxt_empty_watermark */

/*===========================================================================
FUNCTION DS_TDS_W_CDMA_BEARER_CNTXT_PURGE_STATUS_CB

DESCRIPTION
  This function is used to know the status of the purge command posted to RLC

PARAMETERS
   lc_id            - logical channel ID
   purge_status     - status of purge request
   bearer_context_p - bearer context pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tds_w_cdma_bearer_cntxt_purge_status_cb
(
  rlc_lc_id_type                lc_id,
  rlc_ul_wm_purge_rsp_status    purge_status,
  void                          *context
)
{

  ds_cmd_type                         *cmd_ptr;        /* DS Task message */
  ds_bearer_context_s                 *bearer_context_p =
                                                  (ds_bearer_context_s*)context;
  boolean                             purge_cmd_status = FALSE;
  ds_bearer_context_purge_cnf_ev_type *purge_cnf_ev_prt = NULL;

  DS_WCDMA_MSG2_HIGH("DS3GPP PURGE status:%d for lc_id: %d ",
                     purge_status,lc_id);

  if(purge_status == RLC_PURGE_REQ_SUCCESS)
  {
    purge_cmd_status = TRUE;
  }
  /*------------------------------------------------------------------------
    Post command to DS task to process in the proper task context
   ------------------------------------------------------------------------*/

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_bearer_context_purge_cnf_ev_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_RLC_PURGE_CNF;
  purge_cnf_ev_prt =
    (ds_bearer_context_purge_cnf_ev_type*)cmd_ptr->cmd_payload_ptr;

  purge_cnf_ev_prt->bearer_context_p = bearer_context_p;
  purge_cnf_ev_prt->cnf_status = purge_cmd_status;

  ds_put_cmd(cmd_ptr);
} /* ds_tds_w_cdma_bearer_cntxt_purge_status_cb */

/*===========================================================================
FUNCTION DS_TDS_W_CDMA_BEARER_CNTXT_PURGE_CNF_HDLR

DESCRIPTION
  This function is used to Handle the purge status

PARAMETERS
  ds3gpp_purge_cnf_ev which consist of
  cnf_status    - true if if purge status is RLC_PURGE_REQ_SUCCESS(success )
                   false otherwise
  bearer_context_p - bearer context pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tds_w_cdma_bearer_cntxt_purge_cnf_hdlr
(
  ds_bearer_context_purge_cnf_ev_type *ds3gpp_purge_cnf_ev
)
{
 ds_bearer_context_s  *bearer_context_p = ds3gpp_purge_cnf_ev->bearer_context_p;

 if(NULL == bearer_context_p)
 {
   return;
 }
 /*----------------------------------------------------------------------------
  enabling flow back again which got disabled before posting purge comand
 ----------------------------------------------------------------------------*/
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                  DS_FLOW_UMTS_MASK, FALSE);
  if(ds_bearer_cntx_validate_bearer_context(bearer_context_p)&&
     ((bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP)||
      (bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_COMING_UP)))
  {
    /*----------------------------------------------------------------------------
    new call is getting brought up on this bearer instance, So don't empty the WM,
    this Data can be sent through this bearer
    ----------------------------------------------------------------------------*/
    return;
  }
 if(ds3gpp_purge_cnf_ev->cnf_status != TRUE)
 {
   DS_WCDMA_MSG1_ERROR("PURGE Command failure:%d recieved from RLC, empting dsm queue",
                        ds3gpp_purge_cnf_ev->cnf_status);
   dsm_empty_queue(&(bearer_context_p->tx_wm.wm_item));
   return;
 }
  else
  {
   /* This will be cleaned up on next call origination on this bearer instance */
   DS_WCDMA_MSG1_HIGH("PURGE success but not empty tx WM qcnt:%d, Do not empty WM.",
                       bearer_context_p->tx_wm.wm_queue.cnt);
   return;
  }
}/*ds_tds_w_bearer_cntxt_purge_cnf_hdlr*/
/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_RESUMEINDHDLR()

DESCRIPTION
  This is the function that is registered for WCDMA in the umts_bearer context
  function table. The registration happens in wcdma_init.
  This function is called when the resume_ind_hdlr is invoked from DSMGR.
  This function is responsible for resuming connection to the lower layers.
  dswpsd_resume_pdp_proto_stack

PARAMETERS
  bearer_context_p

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
LOCAL void ds_tds_w_cdma_bearer_cntxt_resume_ind_hdlr
(
  ds_bearer_context_s  *bearer_context_p
)
{
  dsm_watermark_type      *tx_wm_p         = NULL;   /* Watermark pointer      */
  phys_link_state_type     phys_link_state = PHYS_LINK_NULL;
  /*-----------------------------------------------------------------------*/


  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return;
  }

  DS_WCDMA_MSG1_HIGH("Resuming connection to WCDMA Radio Layer Inst:0x%x ",
           bearer_context_p);

  ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr(bearer_context_p);

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
  if (!( ( PHYS_LINK_UP   == phys_link_state) ||
         ( PHYS_LINK_DOWN == phys_link_state) ) )
  {
    ps_phys_link_down_ind( &(bearer_context_p->ds_bearer_context_dyn_p->phys_link) );
  }
  ds_3gpp_bearer_cntxt_enable_flow(bearer_context_p,
                                   DS_FLOW_UMTS_MASK, FALSE);

  if(bearer_context_p->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP_DORMANT) // clang_sa_ignore [core.NullDereference]
  {
    /*-------------------------------------------------------------------
      If data is sitting in the Watermarks (PS to PDCP), trigger
      rab re-establishment immly.
    -------------------------------------------------------------------*/
    tx_wm_p = &(bearer_context_p->tx_wm.wm_item);//VRK
    if( tx_wm_p->current_cnt > 0 )
    {
      DS_WCDMA_MSG1_HIGH(
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
} /* ds_tds_w_cdma_bearer_cntxt_resume_ind_hdlr() */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_BEARERTECHTYPEHDLR()

DESCRIPTION
  This function gets the tech type for the
  PS_IFACE_IOCTL_DATA_BEARER_TECH IOCTL.

PARAMETERS
  bearer_tech_ptr - bearer_tech_ptr.
  ps_errno        - returns error reason.
  subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS
  None.
===========================================================================*/
int ds_tds_w_cdma_bearer_cntxt_bearer_tech_type_hdlr
(
  ps_iface_bearer_technology_type    *bearer_tech_ptr,
  sint15                             *ps_errno,
  sys_modem_as_id_e_type              subs_id
)
{
#if defined(FEATURE_HSDPA) && defined(FEATURE_HSPA_CALL_STATUS_IND)
  cm_cell_srv_ind_s_type cell_srv_ind;
#endif /* defined(FEATURE_HSDPA) && defined(FEATURE_HSPA_CALL_STATUS_IND) */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DS_WCDMA_MSG1_HIGH("Bearer tech ptr is 0x%x", bearer_tech_ptr);

  if (( bearer_tech_ptr == NULL ) || ( ps_errno == NULL ) )
  {
     if ( ps_errno != NULL )
     {
      *ps_errno = DS_EFAULT;
     }
    return -1;
  }

  /*-------------------------------------------------------------------------
    Start off with a clean RAT MASK
  -------------------------------------------------------------------------*/
  bearer_tech_ptr->data.umts_type.rat_mask = 0x0;
#ifdef FEATURE_WCDMA

  /*-------------------------------------------------------------------------
    Check with RRC if WCDMA/TDSCDMA is ACTIVE
  -------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
  if( (ds_3gpp_ps_wcdma_tbl != NULL) &&
#ifdef FEATURE_DUAL_WCDMA
      (IWCDMA_rrc_is_wcdma_active_sub(ds_3gpp_ps_wcdma_tbl, subs_id) == TRUE) )
#else
      (IWCDMA_rrc_is_wcdma_active(ds_3gpp_ps_wcdma_tbl) == TRUE) )
#endif
#else
#ifdef FEATURE_DUAL_WCDMA
  if(rrc_is_wcdma_active_sub(subs_id) == TRUE)
#else
  if(rrc_is_wcdma_active() == TRUE)
#endif
#endif /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG0_LOW("WCDMA is active");
    bearer_tech_ptr->data.umts_type.rat_mask |= PS_IFACE_UMTS_WCDMA;
  }
#endif /* FEATURE_WCDMA */
#ifdef FEATURE_TDSCDMA
#ifdef FEATURE_SEGMENT_LOADING
  else if( (ds_3gpp_ps_tds_tbl != NULL) && (ITDSCDMA_rrc_is_tds_active(ds_3gpp_ps_tds_tbl) == TRUE) )
#else
  else if(rrc_is_tds_active() == TRUE)
#endif /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG0_LOW("TDSCDMA is active");
    bearer_tech_ptr->data.umts_type.rat_mask |= PS_IFACE_3GPP_TDSCDMA;
  /*-------------------------------------------------------------------------
    Check with TDSRRC if HSDPA is currently in ACTIVE or SUSPEND state
  -------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
  if ((ITDSCDMA_tdsrrc_get_hsdpa_status(ds_3gpp_ps_tds_tbl) == TDSHSDPA_ACTIVE) ||
      (ITDSCDMA_tdsrrc_get_hsdpa_status(ds_3gpp_ps_tds_tbl) == TDSHSDPA_SUSPEND))
#else
  if ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) ||
      (tdsrrc_get_hsdpa_status() == TDSHSDPA_SUSPEND))
#endif /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG0_HIGH("HSDPA is active or suspended ");
    bearer_tech_ptr->data.umts_type.rat_mask |= PS_IFACE_UMTS_HSDPA;
  }
#ifdef FEATURE_SEGMENT_LOADING
    if ((ITDSCDMA_tdsrrc_get_hsupa_status(ds_3gpp_ps_tds_tbl) == TDSHSUPA_ACTIVE))
#else
    if ((tdsrrc_get_hsupa_status() == TDSHSUPA_ACTIVE))
#endif /* FEATURE_SEGMENT_LOADING */
    {
    DS_WCDMA_MSG0_HIGH("TD HSUPA is active");
    bearer_tech_ptr->data.umts_type.rat_mask |= PS_IFACE_UMTS_HSUPA;
    }
  }
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_HSDPA
#ifndef FEATURE_HSPA_CALL_STATUS_IND
  /*-------------------------------------------------------------------------
    Check with RRC if HSDPA is currently in ACTIVE or SUSPEND state
  -------------------------------------------------------------------------*/
#ifdef FEATURE_DUAL_WCDMA
  if ((rrc_get_hsdpa_status_sub(subs_id) == HSDPA_ACTIVE) ||
      (rrc_get_hsdpa_status_sub(subs_id) == HSDPA_SUSPEND))
#else
  if ((rrc_get_hsdpa_status() == HSDPA_ACTIVE) ||
      (rrc_get_hsdpa_status() == HSDPA_SUSPEND))
#endif
  {
    DS_WCDMA_MSG0_HIGH("HSDPA is active or suspended ");
    bearer_tech_ptr->data.umts_type.rat_mask |= PS_IFACE_UMTS_HSDPA;
  }
#else
  /*-------------------------------------------------------------------------
  Use last reported hspa call status indication
  -------------------------------------------------------------------------*/
  cell_srv_ind = dsUmtsBearerCntxtGetCellSrvInd(subs_id);
  bearer_tech_ptr->data.umts_type.rat_mask =
    ds_wcdma_bearer_cntxt_get_ps_rat_mask_for_hs_status(
       ds_wcdma_bearer_cntxt_get_hs_status_from_cell_srv_ind(&cell_srv_ind),
       subs_id);
#endif /* FEATURE_HSPA_CALL_STATUS_IND */
#endif /* FEATURE_HSDPA */


  DS_WCDMA_MSG1_HIGH("setting rat mask to %d",
            bearer_tech_ptr->data.umts_type.rat_mask);

  return 0;
} /* ds_tds_w_cdma_bearer_cntxt_bearer_tech_type_hdlr */

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_BEARERTECHTYPEHDLR()

DESCRIPTION
  This function gets the tech type for the
  PS_IFACE_IOCTL_DATA_BEARER_TECH IOCTL.

PARAMETERS
  bearer_tech_ptr - bearer_tech_ptr.
  subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS
  None.
===========================================================================*/
int ds_wcdma_bearer_cntxt_bearer_tech_type_ex_hdlr
(
  ds_sys_system_status_info_type    *bearer_tech_ex_ptr,
  sys_modem_as_id_e_type             subs_id
)
{
  DS_WCDMA_MSG0_LOW("Mode passed in is WCDMA");

  DS3GPP_ASSERT(bearer_tech_ex_ptr != NULL);

  bearer_tech_ex_ptr->technology = DS_SYS_NETWORK_3GPP;
  bearer_tech_ex_ptr->rat_value = DS_SYS_RAT_EX_3GPP_WCDMA;

#ifndef FEATURE_HSPA_CALL_STATUS_IND
  bearer_tech_ex_ptr->so_mask = DS_SYS_SO_EX_3GPP_WCDMA;
#else
  bearer_tech_ex_ptr->so_mask =  ds_3gpp_bearer_cntxt_get_so_mask(subs_id);
#endif /*FEATURE_HSPA_CALL_STATUS_IND*/

  return 0;
} /* ds_wcdma_bearer_cntxt_bearer_tech_type_ex_hdlr */

/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_BEARERTECHTYPEHDLR()

DESCRIPTION
  This function gets the tech type for the
  PS_IFACE_IOCTL_DATA_BEARER_TECH IOCTL.

PARAMETERS
  bearer_tech_ptr - bearer_tech_ptr.
  subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS
  None.
===========================================================================*/
int ds_tds_bearer_cntxt_bearer_tech_type_ex_hdlr
(
  ds_sys_system_status_info_type    *bearer_tech_ex_ptr,
  sys_modem_as_id_e_type             subs_id
)
{
  DS_WCDMA_MSG0_LOW("Mode passed in is TDSCDMA");

  DS3GPP_ASSERT(bearer_tech_ex_ptr != NULL);

  bearer_tech_ex_ptr->technology = DS_SYS_NETWORK_3GPP;
  bearer_tech_ex_ptr->rat_value = DS_SYS_RAT_EX_3GPP_TDSCDMA;

#ifndef FEATURE_HSPA_CALL_STATUS_IND
  bearer_tech_ex_ptr->so_mask = DS_SYS_SO_EX_3GPP_TDSCDMA;
#else
  bearer_tech_ex_ptr->so_mask = ds_3gpp_bearer_cntxt_get_so_mask(subs_id);
#endif

  return 0;
} /* ds_tds_w_cdma_bearer_cntxt_bearer_tech_type_hdlr */
#endif /*FEATURE_DATA_WLAN_MAPCON*/
/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_INIT_BEARER_THROUGHPUT_STATS

DESCRIPTION
  This function Initializes the throughput stats for the WCDMA bearers

PARAMETERS
  ds_bearer_context_s - Bearer context ptr

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tds_w_bearer_cntxt_init_bearer_throughput_stats
   (
   ds_bearer_context_s * bearer_context_p
   )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_p))
  {
    return ;
  }
  /*-----------------------------------------------------------------------
    Initialize the previous received count with the current water mark
    count. When the timer expires, we get a better estimate of the
    difference in the byte count to calculate the actual throughput
    -----------------------------------------------------------------------*/
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.actual_dl_rate  = 0;
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.actual_ul_rate  = 0;
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_rx_wm_cnt
                            = bearer_context_p->rx_wm.wm_item.total_rcvd_cnt;
  bearer_context_p->ds_bearer_context_dyn_p->
    bearer_throughput_stats.total_prev_recd_tx_wm_cnt
      = bearer_context_p->tx_wm.wm_item.total_rcvd_cnt;

  DS_WCDMA_MSG3_HIGH("Initializing bearer throughput stats for WCDMA Bearer Cntx "
                     "0x%x  rx WM cnt %lu tx WM Cnt %lu",bearer_context_p,
                     bearer_context_p->ds_bearer_context_dyn_p->
                     bearer_throughput_stats.total_prev_recd_rx_wm_cnt,
                     bearer_context_p->ds_bearer_context_dyn_p->
                     bearer_throughput_stats.total_prev_recd_tx_wm_cnt);
  return;
}

/*=============================================================================
FUNCTION DS_TDS_W_BEARER_CNTXT_COPY_UL_PACKET_METRICS

DESCRIPTION
  This function copies the UL packet metrics returned by WCDMA to structure
  that is passed by Client who queries the metrics

PARAMETERS
  rlc_rtt_delay_ptr: RLC RTT metrics
  rlc_queueing_delay_ptr: RLC queueing delay metrics
  delay_metris_ptr: The struct that will contain RTT & Queueing metrics

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_tds_w_bearer_cntxt_copy_ul_packet_metrics
(
  rlc_traffic_time_metrics                      *rlc_rtt_delay_ptr,
  rlc_traffic_time_metrics                      *rlc_queueing_delay_ptr,
  ps_iface_ioctl_per_bearer_ul_packet_delay_metrics_type  *delay_metrics_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(rlc_rtt_delay_ptr == NULL ||
     rlc_queueing_delay_ptr == NULL ||
     delay_metrics_ptr == NULL)
  {
    DS_WCDMA_MSG3_LOW("Invalid Input Paramters RLC RTT Delay: 0x%x "
                        "RLC Queueing Delay: 0x%x "
                        "Delay Metric: 0x%x",
                        rlc_rtt_delay_ptr, rlc_queueing_delay_ptr,
                        delay_metrics_ptr);
    return;
  }

  delay_metrics_ptr->l2_tx_delay.avg = rlc_rtt_delay_ptr->average_delay_in_ms;

  delay_metrics_ptr->l2_tx_delay.max = rlc_rtt_delay_ptr->max_delay_in_ms;

  delay_metrics_ptr->l2_tx_delay.min = rlc_rtt_delay_ptr->min_delay_in_ms;

  delay_metrics_ptr->l2_tx_delay.num_samples = rlc_rtt_delay_ptr->num_samples;


  delay_metrics_ptr->rm_l2_queueing_delay.avg =
                                  rlc_queueing_delay_ptr->average_delay_in_ms;

  delay_metrics_ptr->rm_l2_queueing_delay.max =
                                  rlc_queueing_delay_ptr->max_delay_in_ms;

  delay_metrics_ptr->rm_l2_queueing_delay.min =
                                  rlc_queueing_delay_ptr->min_delay_in_ms;

  delay_metrics_ptr->rm_l2_queueing_delay.num_samples =
                                  rlc_queueing_delay_ptr->num_samples;
}

/*=============================================================================
FUNCTION DS_TDS_W_BEARER_CNTXT_GET_UL_PACKET_METRICS

DESCRIPTION
  This function queries the WCDMA L2 API to get UL Packet Metrics for a
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
boolean ds_tds_w_bearer_cntxt_get_ul_packet_metrics
(
  ds_bearer_context_s                                    *bearer_context_ptr,
  ps_iface_ioctl_per_bearer_ul_packet_delay_metrics_type *delay_metrics_ptr
)
{
  boolean                                  ret_val = FALSE;
  sys_modem_as_id_e_type                   subs_id = SYS_MODEM_AS_ID_NONE;
  delay_metric_type                        rlc_ul_packet_metric = RTT_DELAY;
  rlc_traffic_time_metrics                 rlc_rtt_delay;
  rlc_traffic_time_metrics                 rlc_queueing_delay;
  rrc_user_plane_lc_info_type              lc_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr) ||
     delay_metrics_ptr == NULL)
  {
    DS_WCDMA_MSG2_LOW("Invalid Inputr params to get UL packet metrics, "
                       "bearer context:0x%x or delay metrics pointer:0x%x",
                       bearer_context_ptr, delay_metrics_ptr);
    return ret_val;
  }

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id))
  {
    return ret_val;
  }

  DS_WCDMA_MSG1_HIGH("Bearer call mode %d ",
                     bearer_context_ptr->ds_bearer_context_dyn_p->call_mode);

  if(bearer_context_ptr->ds_bearer_context_dyn_p->call_mode !=
                                                      SYS_SYS_MODE_WCDMA)
  {
    DS_WCDMA_MSG0_LOW ("No support currently available for the above "
                        "call Mode");
    return ret_val;
  }
  /*-----------------------------------------------------------------------
   Here RRC has no information about NSAPI so we extract the Logical channel
   info (LC) using RRC API and pass the UL LC ID to then for mapping to the
   correct bearer
  -----------------------------------------------------------------------*/
  delay_metrics_ptr->bearer_id = bearer_context_ptr->ds_bearer_context_dyn_p
                                                                       ->nsapi;
  memset(&lc_info, 0, sizeof(rrc_user_plane_lc_info_type));
  lc_info.rab_id = delay_metrics_ptr->bearer_id;

#ifdef FEATURE_SEGMENT_LOADING
  if ( IWCDMA_rrc_return_lc_info_for_rab(ds_3gpp_ps_wcdma_tbl, &lc_info ) ==
                                         RRC_RAB_NOT_FOUND )
#else
  if ( rrc_return_lc_info_for_rab(&lc_info) == RRC_RAB_NOT_FOUND )
#endif  /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG1_ERROR("Invalid RAB id %d",delay_metrics_ptr->bearer_id );
    return ret_val;
  }

  memset(&rlc_rtt_delay, 0, sizeof(rlc_traffic_time_metrics));
  memset(&rlc_queueing_delay, 0, sizeof(rlc_traffic_time_metrics));

  /*-------------------------------------------------------------------------
    invoke WCDMA APIs and fill in the metrics
  -------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
  IWCDMA_rlc_query_traffic_time_metrics(ds_3gpp_ps_wcdma_tbl,
                                        subs_id,
                                        lc_info.ul_lc_id[0],
                                        rlc_ul_packet_metric,
                                        &rlc_rtt_delay);
#else
  rlc_query_traffic_time_metrics(subs_id,
                                 lc_info.ul_lc_id[0],
                                 rlc_ul_packet_metric,
                                 &rlc_rtt_delay);
#endif /*FEATURE_SEGMENT_LOADING*/


  rlc_ul_packet_metric = QUEUING_DELAY;

#ifdef FEATURE_SEGMENT_LOADING
  IWCDMA_rlc_query_traffic_time_metrics(ds_3gpp_ps_wcdma_tbl,
                                        subs_id,
                                        lc_info.ul_lc_id[0],
                                        rlc_ul_packet_metric,
                                        &rlc_rtt_delay);
#else
  rlc_query_traffic_time_metrics(subs_id,
                                 lc_info.ul_lc_id[0],
                                 rlc_ul_packet_metric,
                                 &rlc_rtt_delay);
#endif /*FEATURE_SEGMENT_LOADING*/


  ds_tds_w_bearer_cntxt_copy_ul_packet_metrics(&rlc_rtt_delay,
                                               &rlc_queueing_delay,
                                               delay_metrics_ptr);
  ret_val = TRUE;
  return ret_val;
}

/*===========================================================================
FUNCTION DS_TDS_W_BEARER_CNTXT_RESET_UL_PACKET_METRICS

DESCRIPTION
  This function queries the WCDMA L2 API to reset UL Packet Metrics for a
  particular bearer, both Queueing delay and RTT are requested

PARAMETERS
  bearer_context_ptr: Bearer whose metrics need to be queried

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: On successful metric reset being conveyed to lower layers
  FALSE: Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_tds_w_bearer_cntxt_reset_ul_packet_metrics
(
  ds_bearer_context_s                     *bearer_context_ptr
)
{
  boolean                                  ret_val = FALSE;
  sys_modem_as_id_e_type                   subs_id = SYS_MODEM_AS_ID_NONE;
  rrc_user_plane_lc_info_type              lc_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds_bearer_cntx_validate_bearer_context(bearer_context_ptr))
  {
    return ret_val;
  }

  if(!ds_bearer_cntxt_get_subs_id_from_bearer(bearer_context_ptr, &subs_id))
  {
    return ret_val;
  }

  DS_WCDMA_MSG1_HIGH("Bearer call mode %d ",
                     bearer_context_ptr->ds_bearer_context_dyn_p->call_mode);

  if(bearer_context_ptr->ds_bearer_context_dyn_p->call_mode !=
                                                      SYS_SYS_MODE_WCDMA)
  {
    DS_WCDMA_MSG0_LOW ("No support currently available for the above "
                        "call Mode");
    return ret_val;
  }
  /*-----------------------------------------------------------------------
   Here RRC has no information about NSAPI so we extract the Logical channel
   info (LC) using RRC API and pass the UL LC ID to then for mapping to the
   correct bearer
  -----------------------------------------------------------------------*/
  memset(&lc_info, 0, sizeof(rrc_user_plane_lc_info_type));
  lc_info.rab_id = bearer_context_ptr->ds_bearer_context_dyn_p->nsapi;

#ifdef FEATURE_SEGMENT_LOADING
  if ( IWCDMA_rrc_return_lc_info_for_rab(ds_3gpp_ps_wcdma_tbl, &lc_info ) ==
                                         RRC_RAB_NOT_FOUND )
#else
  if ( rrc_return_lc_info_for_rab(&lc_info) == RRC_RAB_NOT_FOUND )
#endif  /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG1_ERROR("Invalid RAB id %d",
                        bearer_context_ptr->ds_bearer_context_dyn_p->nsapi );
    return ret_val;
  }
  /*-------------------------------------------------------------------------
    invoke WCDMA APIs and fill in the metrics, we always select index zero
    because currently only 1 logical channel is supported per bearer
  -------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
  IWCDMA_rlc_reset_traffic_time_metrics(ds_3gpp_ps_wcdma_tbl, subs_id,
                                        lc_info.ul_lc_id[0]);
#else
  (void)rlc_reset_traffic_time_metrics(subs_id,
                                       lc_info.ul_lc_id[0]);
#endif  /* FEATURE_SEGMENT_LOADING */

  ret_val = TRUE;
  return ret_val;

}
/*===========================================================================
FUNCTION DSWCDMABEARERCNTXT_BEARERRATEHDLR()

DESCRIPTION  This function gets the max channel rate values for the
             PS_IFACE_IOCTL_DATA_BEARER_RATE IOCTL.

PARAMETERS
  call_inst  - index into the call table
  argval_ptr - the bearer rate values will be added here.
  ps_errno   - returns error reason.
  subs_id    - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  0 - SUCCESS
  -1 - FAILURE

SIDE EFFECTS
  None.
===========================================================================*/
int ds_tds_w_cdma_bearer_cntxt_bearer_rate_hdlr
(
  uint32                    call_inst,
  void                     *argval_ptr,
  sint15                   *ps_errno,
  sys_modem_as_id_e_type    subs_id
)
{
  ds_bearer_flow_manager_s         *bearer_flow_manager = NULL;
  ds_bearer_context_s              *ds_bearer_context_p =
                                      ds_bearer_cntxt_get_instance_ptr((uint8)call_inst);
  ps_iface_ioctl_data_bearer_rate  *rate_info =
                                      (ps_iface_ioctl_data_bearer_rate *)argval_ptr;


  if(!ds_bearer_cntx_validate_bearer_context(ds_bearer_context_p))
  {
    return -1 ;
  }

  /*-------------------------------------------------------------------------
   Get bearer from call table and find the corresponding flow manager.
  -------------------------------------------------------------------------*/
  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_flow_crit_sect);

  bearer_flow_manager = ds_bearer_flow_manager_find_flow_manager
                                 ( ds_bearer_context_p );

  if ( bearer_flow_manager == NULL )
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
    DS_WCDMA_MSG0_ERROR( "No bearer associated with this flow");
    rate_info->max_rx_bearer_data_rate = 0;
    rate_info->max_tx_bearer_data_rate = 0;
    return -1 ;
  }
  else if ( (ds_bearer_context_p->ds_bearer_context_dyn_p->state ==
                          DS_BEARER_CONTEXT_STATE_UP)            ||
            (ds_bearer_context_p->ds_bearer_context_dyn_p->state ==
                          DS_BEARER_CONTEXT_STATE_UP_DORMANT)    ||
            (ds_bearer_context_p->ds_bearer_context_dyn_p->state ==
                          DS_BEARER_CONTEXT_STATE_UP_DORMANT_REESTAB)  )
  {
    (void)ds_bearer_flow_manager_get_bearer_qos(
                          bearer_flow_manager, argval_ptr, subs_id);

    DS_WCDMA_MSG2_HIGH( "Set max_rx: %d max_tx: %d ",
                         rate_info->max_rx_bearer_data_rate,
                         rate_info->max_tx_bearer_data_rate);
  }
  else
  {
    rate_info->max_rx_bearer_data_rate = 0;
    rate_info->max_tx_bearer_data_rate = 0;
  }
  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_flow_crit_sect);
  /*-------------------------------------------------------------------------
    Support for avg and current are not available from lower layers, so
    setting to -1 meaning unknown.
  -------------------------------------------------------------------------*/
  rate_info->avg_tx_bearer_data_rate = -1;
  rate_info->avg_rx_bearer_data_rate = -1;
  rate_info->current_tx_bearer_data_rate = -1;
  rate_info->current_rx_bearer_data_rate = -1;

  return 0;
} /* ds_tds_w_cdma_bearer_cntxt_bearer_rate_hdlr */

#endif /* FEATURE_TDSCDMA || FEATURE_DATA_WCDMA_PS */

#ifdef FEATURE_WCDMA
/*===========================================================================
FUNCTION DS_BEARER_CONTEXT_WCDMA_INIT

DESCRIPTION
  This function is used to init the WCDMA bearer context. This function
  registers the WCDMA specific functions with UMTS bearer context function
  table.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void dsWcdmaBearerCntxt_init( void )
{
  ds_umts_bearer_context_ftbl_s  *wcdma_bearer_context_ftbl_p;

  wcdma_bearer_context_ftbl_p = &(umts_bearer_context_ftbl[DS_3GPPI_SYS_MODE_WCDMA]);
  memset(
      (void*)wcdma_bearer_context_ftbl_p,
      0x0,
      (sizeof(ds_umts_bearer_context_ftbl_s))
      );

  memset(
     &ds_wcdma_bearer_cntxt_rb_data,
     0,
     sizeof(ds_wcdma_bearer_cntxt_rb_data_type) * DS_3GPP_MAX_BEARER_CONTEXT);

  /*---------------------------------------------------------------------------
    Fill up the WCDMA function table with the functions implemented in this
    file.
  ---------------------------------------------------------------------------*/
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, activateCnf, \
                             ds_tds_w_cdma_bearer_cntxt_activate_cnf);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, dormancyReorig, \
                        ds_bearer_cntxt_reorig_dorm);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, deactivateCnf, \
                        ds_tds_w_cdma_bearer_cntxt_deactivate_cnf);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, configurePdpPpp, \
                             ds_tds_w_cdma_bearer_cntxt_configure_pdp_ppp);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, rab_re_estab_ind, \
                        ds_tds_w_cdma_bearer_cntxt_rab_reestab_ind_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, resume_ind, \
                        ds_tds_w_cdma_bearer_cntxt_resume_ind_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, getBearerTechType,
                        ds_tds_w_cdma_bearer_cntxt_bearer_tech_type_hdlr);
#ifdef FEATURE_DATA_WLAN_MAPCON
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, getBearerTechTypeEx,
                        ds_wcdma_bearer_cntxt_bearer_tech_type_ex_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, getSoMask,
                        ds_wcdma_bearer_cntxt_get_so_mask);
#endif /*FEATURE_DATA_WLAN_MAPCON*/
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, getBearerRate, \
                        ds_tds_w_cdma_bearer_cntxt_bearer_rate_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, empty_watermark, \
                        ds_tds_w_cdma_bearer_cntxt_empty_watermark);

#if defined(FEATURE_DATA_LTE)  && defined(FEATURE_DATA_WCDMA_PS)
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, cntxt_transfer_ind_setup, \
                        ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr);

#endif /* defined(FEATURE_DATA_LTE) && defined(FEATURE_DATA_WCDMA_PS) */

#ifdef FEATURE_QXDM_WM_TRACING
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, report_wm_stats, \
                        ds_tds_w_cdma_bearer_cntxt_report_wm_stats);
#endif /* FEATURE_QXDM_WM_TRACING */

  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, getratMaskforHs, \
                        ds_wcdma_bearer_cntxt_get_ps_rat_mask_for_hs_status);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, purgecmdcbhdr, \
                        ds_tds_w_cdma_bearer_cntxt_purge_cnf_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, complogfetchiface, \
                        ds_tds_w_cdma_bearer_cntxt_comp_log_fetch_iface);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, refresh_actual_throughput, \
                                    ds_tds_w_bearer_cntxt_calculate_actual_throughput);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, init_bearer_throughput_stats, \
                                    ds_tds_w_bearer_cntxt_init_bearer_throughput_stats);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, get_ul_packet_metrics, \
                                    ds_tds_w_bearer_cntxt_get_ul_packet_metrics);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, reset_ul_packet_metrics, \
                                    ds_tds_w_bearer_cntxt_reset_ul_packet_metrics);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_WCDMA, register_wm, \
  	                         ds_tds_w_cdma_register_wm);
#ifdef FEATURE_SEGMENT_LOADING
  ds_3gpp_ps_tds_tbl   = NULL;
  ds_3gpp_ps_wcdma_tbl = NULL;
  /*-------------------------------------------------------------------------
    Initialize TDSCDMA and WCDMA tables.
  -------------------------------------------------------------------------*/
  create_instance(CLSID_TDSCDMA, &ds_3gpp_ps_tds_tbl);
  create_instance(CLSID_WCDMA, &ds_3gpp_ps_wcdma_tbl);
  if( (ds_3gpp_ps_tds_tbl == NULL) && (ds_3gpp_ps_wcdma_tbl == NULL) )
  {
    DS_WCDMA_MSG0_LOW("Neither TDS nor WCDMA active");
  }
  if( (ds_3gpp_ps_tds_tbl != NULL) && (ds_3gpp_ps_wcdma_tbl != NULL) )
  {
    DS_WCDMA_MSG0_LOW("Both TDS and WCDMA active");
  }
#endif /* FEATURE_SEGMENT_LOADING */

  return;
} /*dsWcdmaBearerCntxt_init()*/
#endif /*FEATURE_WCDMA*/


#ifdef FEATURE_TDSCDMA
/*===========================================================================
FUNCTION DS_TDS_BEARER_CNTXT_INIT

DESCRIPTION
  This function is used to init the TDSCDMA bearer context. This function
  registers the TDSCDMA specific functions with UMTS bearer context function
  table.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tds_bearer_cntxt_init( void )
{
  ds_umts_bearer_context_ftbl_s *tds_bearer_context_ftbl_p;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  tds_bearer_context_ftbl_p = &(umts_bearer_context_ftbl[DS_3GPPI_SYS_MODE_TDS]);
  memset(
      (void*)tds_bearer_context_ftbl_p,
      0x0,
      (sizeof(ds_umts_bearer_context_ftbl_s))
      );

  /*---------------------------------------------------------------------------
    Fill up the TDSCDMA function table with the functions implemented in this
    file.
  ---------------------------------------------------------------------------*/
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, activateCnf, \
                        ds_tds_w_cdma_bearer_cntxt_activate_cnf);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, dormancyReorig, \
                        ds_bearer_cntxt_reorig_dorm);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, deactivateCnf, \
                        ds_tds_w_cdma_bearer_cntxt_deactivate_cnf);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, configurePdpPpp, \
                        ds_tds_w_cdma_bearer_cntxt_configure_pdp_ppp);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, rab_re_estab_ind, \
                        ds_tds_w_cdma_bearer_cntxt_rab_reestab_ind_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, resume_ind, \
                        ds_tds_w_cdma_bearer_cntxt_resume_ind_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, getBearerTechType,
                        ds_tds_w_cdma_bearer_cntxt_bearer_tech_type_hdlr);
#ifdef FEATURE_DATA_WLAN_MAPCON
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, getBearerTechTypeEx,
                        ds_tds_bearer_cntxt_bearer_tech_type_ex_hdlr);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, getSoMask,
                        ds_tds_bearer_cntxt_get_so_mask);
#endif /*FEATURE_DATA_WLAN_MAPCON*/
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, getBearerRate, \
                        ds_tds_w_cdma_bearer_cntxt_bearer_rate_hdlr);

#if defined(FEATURE_DATA_LTE) && defined(FEATURE_TDSCDMA)
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, cntxt_transfer_ind_setup, \
                        ds_tds_w_cdma_bearer_cntxt_transfer_setup_hdlr);

#endif /* defined(FEATURE_DATA_LTE) && defined(FEATURE_TDSCDMA) */

#ifdef FEATURE_QXDM_WM_TRACING
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, report_wm_stats, \
                        ds_tds_w_cdma_bearer_cntxt_report_wm_stats);
#endif /* FEATURE_QXDM_WM_TRACING */
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, getratMaskforHs, \
                        ds_wcdma_bearer_cntxt_get_ps_rat_mask_for_hs_status);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, complogfetchiface, \
                             ds_tds_w_cdma_bearer_cntxt_comp_log_fetch_iface);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, refresh_actual_throughput, \
                                    ds_tds_w_bearer_cntxt_calculate_actual_throughput);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, init_bearer_throughput_stats, \
                                    ds_tds_w_bearer_cntxt_init_bearer_throughput_stats);
  DS_UMTS_BEARER_CTXT_VF_REG(SYS_SYS_MODE_TDS, register_wm, \
  	                         ds_tds_w_cdma_register_wm);
} /*ds_tds_bearer_cntxt_init()*/
#endif /* FEATURE_TDSCDMA */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CONTEXT_REFRESH_VOTING_HDLR

DESCRIPTION
  This function checks the state of all WCDMA PS calls and decides whether an
  SIM refresh will be allowed to proceed.

PARAMETERS
  subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - if SIM Refresh can proceed from mode handler perspective.
  FALSE - if SIM Refresh cannot proceed from mode handler perspective.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dsWcdmaBearerCntxt_refresh_voting_hdlr
(
  sys_modem_as_id_e_type      subs_id
)
{
  uint32                 pdn_cntx_inst;
  uint8                  iface_index, bearer_index;
  ds_3gpp_iface_s        *ds_iface_ptr = NULL;
  ps_iface_type          *iface_ptr = NULL;
  ds_bearer_context_s    *bearer_context_ptr = NULL;
  ds_pdn_context_s       *pdn_context_ptr = NULL;

  /*--------------------------------------------------------------------------
    For each active PDN, we check the following:
    If all the bearers are in down or dormant state, we vote YES for refresh.
    If all the ifaces are out of use, we vote YES for refresh.
  --------------------------------------------------------------------------*/
  for( pdn_cntx_inst=0;
       pdn_cntx_inst<DS_3GPP_MAX_PDN_CONTEXT;
       pdn_cntx_inst++ )
  {
    pdn_context_ptr = &(ds_pdn_context_tbl[pdn_cntx_inst]);

    if(ds_pdn_cntxt_validate_pdn_subscription_internal(pdn_context_ptr, subs_id))
    {
      if (pdn_context_ptr->ds_pdn_context_dyn_p->state !=
          DS_PDN_CONTEXT_STATE_UP &&
          pdn_context_ptr->ds_pdn_context_dyn_p->state !=
          DS_PDN_CONTEXT_STATE_COMING_UP )
      {
        continue;
      }

    /* check if any of the iface is in use, i.e. pdn is in use by an app */
    for (iface_index = 0;
         iface_index < DS_3GPP_MAX_IFACE_PER_PDN_CONTEXT;
         iface_index++)
    {
      /*---------------------------------------------------------------------------
        Get the DS Iface
      ---------------------------------------------------------------------------*/
        ds_iface_ptr = pdn_context_ptr->ds_pdn_context_dyn_p->
          ds_3gpp_iface_tbl[iface_index];

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
        DS_WCDMA_MSG2_HIGH("WCDMA pdn cntx %d iface:%d is in use while refresh voting",
                            pdn_cntx_inst,
                            iface_ptr);
        /* if iface is in use, we see if rab is dormant */
        for(bearer_index = 0;
            bearer_index < DS_3GPP_MAX_BEARER_CONTEXT_PER_PDN_CONTEXT;
            bearer_index++)
        {
          bearer_context_ptr =
                (ds_bearer_context_s*)pdn_context_ptr->ds_pdn_context_dyn_p->
                                                    bearer_context_ptr_tbl[bearer_index];

          if(bearer_context_ptr == NULL)
          {
            /*-----------------------------------------------------------------------
              This entry in the bearer context table is NULL. Other entries could
              have non-NULL values.
            -----------------------------------------------------------------------*/
            continue;
          }
            if (bearer_context_ptr->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_UP ||
                bearer_context_ptr->ds_bearer_context_dyn_p->state == DS_BEARER_CONTEXT_STATE_COMING_UP)
          {
            DS_WCDMA_MSG1_HIGH("WCDMA SIM refresh handler voted NO for pdn cntx=%d",
                                pdn_cntx_inst);
            return FALSE;
          }
        }
      }
    }
  }
  }
  return TRUE;
}

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_HS_STATUS_FROM_CELL_SRV_IND

DESCRIPTION
  This function returns the hs_status from the cell service ind

PARAMETERS
  cm_cell_srv_ind_s_type - cell_srv_ind

DEPENDENCIES
  None.

RETURN VALUE
  hs_status

SIDE EFFECTS
  None.
===========================================================================*/
sys_hs_ind_e_type ds_wcdma_bearer_cntxt_get_hs_status_from_cell_srv_ind
(
  cm_cell_srv_ind_s_type  *cell_srv_ind_ptr
)
{
  sys_hs_ind_e_type  hs_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(cell_srv_ind_ptr == NULL)
  {
    return  hs_status;
  }

  if(cell_srv_ind_ptr->hs_call_status != SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL)
  {
    hs_status = cell_srv_ind_ptr->hs_call_status;
  }
  else
  {
    hs_status = cell_srv_ind_ptr->hs_ind;
  }

  DS_WCDMA_MSG1_LOW("hs_status value [%d] ", hs_status);

  return hs_status;
} /* ds_wcdma_bearer_cntxt_get_hs_status_from_cell_srv_ind */

/*===========================================================================
FUNCTION DS_W_TDS_CDMA_BEARER_CNTXT_GET_RAT_MASK_FOR_HS_STATUS

DESCRIPTION
  This function returns the RAT MASK corresponding to the passed HS status.

PARAMETERS
  hs_status

DEPENDENCIES
  None.

RETURN VALUE
  rat mask

SIDE EFFECTS
  None.
===========================================================================*/
uint32 ds_w_tds_cdma_bearer_cntxt_get_rat_mask_for_hs_status
(
  sys_hs_ind_e_type hs_status
)
{
  uint32 rat_mask = 0;

  switch (hs_status)
  {
    case SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL:
      break;
    case SYS_HS_IND_HSDPA_SUPP_CELL:
      rat_mask |= PS_IFACE_3GPP_HSDPA;
      break;
    case SYS_HS_IND_HSUPA_SUPP_CELL:
      rat_mask |= PS_IFACE_3GPP_HSUPA;
      break;
    case SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL:
      rat_mask |= (PS_IFACE_3GPP_HSDPA | PS_IFACE_3GPP_HSUPA);
      break;
    case SYS_HS_IND_HSDPAPLUS_SUPP_CELL:
      rat_mask |= PS_IFACE_3GPP_HSDPAPLUS;
      break;
    case SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL:
      rat_mask |= (PS_IFACE_3GPP_HSDPAPLUS | PS_IFACE_3GPP_HSUPA);
      break;
    case SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL:
      rat_mask |= PS_IFACE_3GPP_DC_HSDPAPLUS;
      break;
    case SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL:
      rat_mask |= (PS_IFACE_3GPP_DC_HSDPAPLUS | PS_IFACE_3GPP_HSUPA);
      break;
    case SYS_HS_IND_HSDPAPLUS_64QAM_SUPP_CELL:
      rat_mask |= (PS_IFACE_3GPP_HSDPAPLUS|PS_IFACE_3GPP_64_QAM);
      break;
    case SYS_HS_IND_HSDPAPLUS_64QAM_HSUPA_SUPP_CELL:
      rat_mask |= (PS_IFACE_3GPP_HSDPAPLUS | PS_IFACE_3GPP_HSUPA
                  |PS_IFACE_3GPP_64_QAM);
      break;
    default:
      DS_WCDMA_MSG1_LOW("hs_status value [%d] unrecognized!", hs_status);
      break;
  }

  return rat_mask;
}

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_PS_RAT_MASK

DESCRIPTION
  This function returns the RAT MASK corresponding to the passed sys_mode,
  HS ind, and call_status.

PARAMETERS
  sys_mode - CM sys mode
  hs_status - the HS status

DEPENDENCIES
  None.

RETURN VALUE
  RAT mask

SIDE EFFECTS
  None.
===========================================================================*/
uint32 ds_wcdma_bearer_cntxt_get_ps_rat_mask
(
  sys_sys_mode_e_type      sys_mode,
  sys_hs_ind_e_type        hs_status,
  sys_modem_as_id_e_type   subs_id  
)
{
  uint32  rat_mask = PS_IFACE_3GPP_NULL_BEARER;

  if(sys_mode == SYS_SYS_MODE_WCDMA)
  {
    rat_mask = PS_IFACE_3GPP_WCDMA;
  }
  else if(sys_mode == SYS_SYS_MODE_TDS)
  {
    rat_mask = PS_IFACE_3GPP_TDSCDMA;
  }
  else
  {
    /* Set rat_mask as NULL bearer */
    rat_mask = PS_IFACE_3GPP_NULL_BEARER;
  }

  /* if rat_mask is NULL bearer, skip setting hs_status */
  if (rat_mask != PS_IFACE_3GPP_NULL_BEARER)
  {
    rat_mask |= ds_w_tds_cdma_bearer_cntxt_get_rat_mask_for_hs_status(hs_status);
  }
  DS_WCDMA_MSG3_MED_EX(subs_id, "CM sys_mode: %d, hs_status: %d rat mask %d",
                     sys_mode, hs_status, rat_mask);
  return rat_mask;
}

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_PS_RAT_MASK_FOR_HS_STATUS

DESCRIPTION
  This function returns the RAT MASK corresponding to the passed HS status.

PARAMETERS
  hs_status       - the HS status
  subs_id         - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  RAT mask

SIDE EFFECTS
  None.
===========================================================================*/
uint32 ds_wcdma_bearer_cntxt_get_ps_rat_mask_for_hs_status
(
  sys_hs_ind_e_type       hs_status,
  sys_modem_as_id_e_type  subs_id
)
{
  uint32                 rat_mask = PS_IFACE_3GPP_NULL_BEARER;
  sys_sys_mode_e_type    curr_3gpp_mode =
                           ds3gpp_get_current_network_mode(subs_id);

  /*-------------------------------------------------------------------------
  Check with RRC if WCDMA/TDSCDMA is ACTIVE
  -------------------------------------------------------------------------*/
#ifdef FEATURE_SEGMENT_LOADING
  if( (ds_3gpp_ps_wcdma_tbl != NULL) &&
#ifdef FEATURE_DUAL_WCDMA
      (IWCDMA_rrc_is_wcdma_active_sub(ds_3gpp_ps_wcdma_tbl, subs_id) == TRUE) )
#else
      (IWCDMA_rrc_is_wcdma_active(ds_3gpp_ps_wcdma_tbl) == TRUE) )
#endif
#else
  #ifdef FEATURE_DUAL_WCDMA
  if(rrc_is_wcdma_active_sub(subs_id) == TRUE)
#else
  if(rrc_is_wcdma_active() == TRUE)
#endif
#endif /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG0_LOW("WCDMA is active");
    rat_mask = PS_IFACE_3GPP_WCDMA;
  }
#ifdef FEATURE_TDSCDMA
#ifdef FEATURE_SEGMENT_LOADING
  if(ds_3gpp_ps_tds_tbl != NULL &&
          ITDSCDMA_rrc_is_tds_active(ds_3gpp_ps_tds_tbl) == TRUE)
#else
  if(rrc_is_tds_active() == TRUE)
#endif /* FEATURE_SEGMENT_LOADING */
  {
    DS_WCDMA_MSG0_LOW("TDSCDMA is active");
    rat_mask = PS_IFACE_3GPP_TDSCDMA;
  }
#endif /* FEATURE_TDSCDMA */
  else
  {
    DS_WCDMA_MSG2_LOW("3GPP sys_mode: %d, hs_status: %d",
                      curr_3gpp_mode,
                      hs_status);

    /* DS 3GPP internal mode to update the RAT mask. */
    if(curr_3gpp_mode == SYS_SYS_MODE_WCDMA)
    {
      rat_mask = PS_IFACE_3GPP_WCDMA;
    }
    else if(curr_3gpp_mode == SYS_SYS_MODE_TDS)
    {
      rat_mask = PS_IFACE_3GPP_TDSCDMA;
    }
    else
    {
      /* Set rat_mask as NULL bearer */
      rat_mask = PS_IFACE_3GPP_NULL_BEARER;
    }
  }

  /* if rat_mask is NULL bearer, skip setting hs_status */
  if (rat_mask != PS_IFACE_3GPP_NULL_BEARER)
  {
    rat_mask |= ds_w_tds_cdma_bearer_cntxt_get_rat_mask_for_hs_status(hs_status);
  }

  return rat_mask;
}

#ifdef FEATURE_QXDM_WM_TRACING
/*===========================================================================
FUNCTION DS_BEARER_CONTEXT_WCDMA_REPORT_WM_STATS

DESCRIPTION
  This function stores the current watermark status.

PARAMETERS
  bearer_instance - bearer context instance
  call_id         - call id
  wm_stat_ptr     - where the wm values are stored

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tds_w_cdma_bearer_cntxt_report_wm_stats
(
  uint32          bearer_instance,
  uint32          call_id,
  fc_wm_stat_type *wm_stat_ptr
)
{
  ds_bearer_context_s  *bearer_context_p;
  /* Index to wm_info array provided by FC */
  int index = wm_stat_ptr->num_wms;


  /*---------------------------------------------------------------------------
    Get the bearer context pointer
  ---------------------------------------------------------------------------*/
  if( bearer_instance >= DS_3GPP_MAX_BEARER_CONTEXT )
  {
    DS_WCDMA_MSG0_LOW("bearer_instance is greater than or equal to "
              "DS_3GPP_MAX_BEARER_CONTEXT ");
    return;
  }

    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_instance);

  /*---------------------------------------------------------------------------
    WM_DIR_PDCP_PS_DL
  ----------------------------------------------------------------------------*/
    /* Index to wm_info array provided by FC */
  index = wm_stat_ptr->num_wms;

  if(index >= MAX_WM_CNT)
  {
    DS_WCDMA_MSG0_ERROR("Max WM count reached. Cannot report statistics");
    return;
  }

  DS_WCDMA_MSG0_MED("Reporting statistics for WM_DIR_PDCP_PS_DL");

  /* Report the technology */
  wm_stat_ptr->wm_info[index].tech_id = UMTS_TECH;

  /* Report the Call ID */
  wm_stat_ptr->wm_info[index].call_id = call_id;

  /* Specify the WM for which the stats are being reported */
  wm_stat_ptr->wm_info[index].wm_id = WM_DIR_PDCP_PS_DL;

  /* Query DSM for wm stats */
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
    DS_WCDMA_MSG0_ERROR("Max WM count reached. Cannot report statistics");
    return;
  }

  DS_WCDMA_MSG0_MED("Reporting statistics for WM_DIR_PDCP_PS_UL");

  /* Report the technology */
  wm_stat_ptr->wm_info[index].tech_id=UMTS_TECH;

  /* Report the Call ID */
  wm_stat_ptr->wm_info[index].call_id = call_id;

  /* Specify the WM for which the stats are being reported */
  wm_stat_ptr->wm_info[index].wm_id = WM_DIR_PDCP_PS_UL;

  /* Query DSM for wm stats */
  dsm_get_wm_stats(&(bearer_context_p->tx_wm.wm_item),
                   &(wm_stat_ptr->wm_info[index].wm_counts));
  wm_stat_ptr->num_wms++;
} /* ds_tds_w_cdma_bearer_cntxt_report_wm_stats */
#endif /* FEATURE_QXDM_WM_TRACING */

/*===========================================================================
FUNCTION DS_TDS_W_CDMA_BEARER_CNTXT_COMP_LOG_FETCH_IFACE

DESCRIPTION
  This Functions returns the iface corresponding to link control id.

PARAMETERS
  logging_info_ptr - the Log information.
  ip_version       - ip version v4 or v6.

DEPENDENCIES
  None.

RETURN VALUE
  Iface.

SIDE EFFECTS
  None.

===========================================================================*/
ps_iface_type* ds_tds_w_cdma_bearer_cntxt_comp_log_fetch_iface
(
  ps_comp_logging_instance_config_s *logging_info_ptr,
  ip_version_enum_type              ip_version
)
{
  uint8                     bearer_idx;
  ds_bearer_context_s      *bearer_context_p;
  ds_pdn_context_s         *pdn_context_p;
  ds_3gpp_iface_s          *ds_iface_ptr;
  ds_umts_bearer_context_s *umts_bearer_contxt_ptr;
  ps_iface_type            *iface_ptr = NULL;
  boolean                   valid_bearer = FALSE;
/*--------------------------------------------------------------------------------*/

  /* Validate logging_info_ptr */
  if(logging_info_ptr == NULL)
  {
    return NULL;
  }
  /** check for IP version **/
  if(!((ip_version == IP_V4) || (ip_version == IP_V6)))
  {
    DS_WCDMA_MSG1_ERROR("Invalid IP Version passed %d", ip_version);
    return NULL;
  }

  for (bearer_idx = 0; bearer_idx < DS_3GPP_MAX_BEARER_CONTEXT; bearer_idx++)
  {
    bearer_context_p = ds_bearer_cntxt_get_instance_ptr(bearer_idx);

    if (!ds_bearer_cntx_validate_bearer_context_internal(bearer_context_p))
    {
      DS_WCDMA_MSG1_LOW("Bearer context: %d is Invalid", bearer_idx);
      continue;
    }

    umts_bearer_contxt_ptr=
      (ds_umts_bearer_context_s *)(bearer_context_p-> ds_bearer_context_dyn_p->
         vobj_data_ptr_tbl[DS_3GPP_UMTS_VOBJ]);
    if(!dsUmtsBearerCntxt_validatebearer(umts_bearer_contxt_ptr))
    {
      DS_WCDMA_MSG1_LOW("umts bearer context %d is Invalid", bearer_idx);
      continue;
    }
    if ((logging_info_ptr->logging_info.wcdma_lcid)==
           umts_bearer_contxt_ptr->ds_umts_bearer_context_dyn_p->u_lc_id)
    {
      valid_bearer = TRUE;
      break;
    }/* valid UMTS matching LC id */
  }/* For .. */

  if ( TRUE == valid_bearer )
  {
    pdn_context_p=bearer_context_p->ds_bearer_context_dyn_p->pdn_context_ptr ;
    if(!ds_3gpp_pdn_cntx_validate_pdn_context(pdn_context_p))
    {
      /* Print bearer instance */
      return NULL;
    }
    if (ip_version == IP_V4)
    {
      ds_iface_ptr = pdn_context_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV4_IFACE];
      iface_ptr = &(ds_iface_ptr->ps_iface);
    }
    else if (ip_version == IP_V6)
    {
      ds_iface_ptr = pdn_context_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl[DS_3GPP_PDN_CONTEXT_IPV6_IFACE];
      iface_ptr = &(ds_iface_ptr->ps_iface);
    }
    DS_WCDMA_MSG3_ERROR(
       "The ip version is : %d with iface as %d and lc id as %d",
       ip_version,
       iface_ptr,
       umts_bearer_contxt_ptr->ds_umts_bearer_context_dyn_p->u_lc_id);
    return iface_ptr;
  }
  else
  {
    DS_WCDMA_MSG0_ERROR("No Valid bearer where found");
    return NULL ;
  }

}/* ds_wcdma_comp_log_fetch_iface */

/*===========================================================================
FUNCTION DS_WCDMA_GET_ACHIEVABLE_UL_DATA_RATE

DESCRIPTION
  This function is used to get the achievable ul data rate

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  uint32- achievable ul data rate

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_wcmda_get_achievable_ul_data_rate(void)
{
  wl1_eul_stats_struct_type   wcdma_eul_stats = {0};
  /*-------------------------------------------------------------------------
      Call WLI API to get UL stats
  -------------------------------------------------------------------------*/
  if (wl1_eul_stats(&wcdma_eul_stats) == TRUE)
  {
    return wcdma_eul_stats.achievable_power_limited_datarate;
  }
  else
  {
    return 0;
  }

}
/*===========================================================================
FUNCTION DS_WCDMA_GET_ACHIEVABLE_dL_DATA_RATE

DESCRIPTION
  This function is used to get the achievable dl data rate

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  uint32- achievable ul data rate

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_wcmda_get_achievable_dl_data_rate(void)
{
  wl1_hsdpa_stats_struct_type hsdpa_stats;
  /*-------------------------------------------------------------------------
      Call WLI API to get UL stats
  -------------------------------------------------------------------------*/
  if (wl1_get_hsdpa_stats(&hsdpa_stats) == TRUE)
  {
    return hsdpa_stats.available_throughput;
  }
  else
  {
    return 0;
  }

}
/*===========================================================================
FUNCTION DS_WCDMA_ACHIEVABLE_TPUT_STARTSTOP

DESCRIPTION
  This function is used to get the achievable ul data rate

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  uint32- achievable ul data rate

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_achievable_tput_startstop
(
  boolean start
)
{
  if (start)
  {
    wl1_start_tput_data_reporting_ind();
    DS_WCDMA_MSG0_HIGH("Sending WCDMA ULDL Achievable throughput start");
  }
  else
  {
    wl1_stop_tput_data_reporting_ind();
    DS_WCDMA_MSG0_HIGH("Sending WCDMA ULDL Achievable throughput stop");
  }

}
#ifdef FEATURE_DATA_RAVE_SUPPORT
/*===========================================================================
FUNCTION DS_WCDMA_START_UPLINK_THROUGHPUT_REPORTING

DESCRIPTION
  This function is used to send the indication to wcdma l2 to start uplink
  throughput reporting

PARAMETERS
   sys_modem_as_id_e_type  subs_id,
   boolean                 start_stop
   uint32                  reporting_interval

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 ds_wcdma_start_stop_uplink_throughput_reporting
(
   sys_modem_as_id_e_type  subs_id,
   boolean                 start_stop,
   uint32                  reporting_interval
)
{
  uint32 uplink_wcdma_taccum =0;
  DS_WCDMA_MSG1_HIGH("Uplink Throughput reporting start_stop %d",start_stop);


  uplink_wcdma_taccum = ds_3gpp_nv_manager_get_uplink_tput_tacccum_wcdma(subs_id);

  if (uplink_wcdma_taccum > reporting_interval)
  {
    reporting_interval = uplink_wcdma_taccum;
  }

  if (reporting_interval > DS3G_UPLINK_TPUT_WCDMA_TACCUM_MAX_VALUE)
  {
    reporting_interval = DS3G_UPLINK_TPUT_WCDMA_TACCUM_MAX_VALUE;
  }

  #ifdef FEATURE_SEGMENT_LOADING
    if(ds_3gpp_ps_wcdma_tbl != NULL)
    {
       IWCDMA_wcdma_l2_start_uplink_rate_calculations(
         ds_3gpp_ps_wcdma_tbl,
         subs_id,
         start_stop,
         reporting_interval);
    }
  #else
    wcdma_l2_start_uplink_rate_calculations(
       subs_id,
       start_stop,
       reporting_interval);
  #endif /* FEATURE_SEGMENT_LOADING */

  return reporting_interval;

}
#endif

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_RRC_DATA_RATE_IND_CB

DESCRIPTION
  This callback function is called by PDCP whenever it receives RRC
  Data Rate indications, RELEASE_PDCP or RELEASE_ALL_PDCP  indication.

PARAMETERS
  pdcp_action: PDCP action
  rb_id: Radio Bearer ID
  rlc_ul_id: RLC UL ID
  rab_ul_data_rate: RAB UL data rate

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_rrc_data_rate_ind_cb
(
  pdcp_rrc_data_rate_ind_e_type  pdcp_ind,
  uint32                         rb_id,
  uint8                          rlc_ul_id,
  uint32                         rab_ul_data_rate
)
{
  ds_cmd_type                               *cmd_ptr = NULL;
  ds_wcdma_bearer_cntxt_data_rate_ind_type  *data_rate_ind_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_WCDMA_MSG3_HIGH("ds_wcdma_bearer_cntxt_rrc_data_rate_ind_cb: rb_id:%d, "
                     "rlc_ul_id:%d, rab_ul_rate:%d",
                     rb_id, rlc_ul_id, rab_ul_data_rate);

  /* Allocate a command and post */
  cmd_ptr = ds_allocate_cmd_buf(
              sizeof(ds_wcdma_bearer_cntxt_data_rate_ind_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PROC_WRRC_DATA_RATE_IND;
  data_rate_ind_p =
    (ds_wcdma_bearer_cntxt_data_rate_ind_type*)cmd_ptr->cmd_payload_ptr;

  data_rate_ind_p->pdcp_ind         = pdcp_ind;
  data_rate_ind_p->rb_id            = rb_id;
  data_rate_ind_p->rlc_ul_id        = rlc_ul_id;
  data_rate_ind_p->rab_ul_data_rate = rab_ul_data_rate;
  ds_put_cmd_ext( cmd_ptr );

  return;
} /* ds_wcdma_bearer_cntxt_rrc_data_rate_ind_cb */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_CACHE_WCDMA_RB_DATA

DESCRIPTION
  This function caches the WCDMA RB data

PARAMETERS
  pdcp_action: PDCP action
  rb_id: Radio Bearer ID
  rlc_ul_id: RLC UL ID
  rab_ul_data_rate: RAB UL data rate

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_cache_wcdma_rb_data
(
  pdcp_rrc_data_rate_ind_e_type  pdcp_ind,
  uint32                         rb_id,
  uint8                          rlc_ul_id,
  uint32                         rab_ul_data_rate
)
{
  ds_wcdma_bearer_cntxt_rb_data_type  *rb_data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  switch( pdcp_ind )
  {
    case PDCP_DATA_RATE_IND:
      rb_data_ptr = ds_wcdma_bearer_cntxt_find_rb_data(rlc_ul_id);
      if( rb_data_ptr != NULL )
      {
        rb_data_ptr->rab_ul_data_rate = rab_ul_data_rate;
      }
      else
      {
        rb_data_ptr = ds_wcdma_bearer_cntxt_get_avail_rb_data();
        if( rb_data_ptr != NULL )
        {
          rb_data_ptr->is_occupied = TRUE;
          rb_data_ptr->rb_id = rb_id;
          rb_data_ptr->rlc_ul_id = rlc_ul_id;
          rb_data_ptr->rab_ul_data_rate = rab_ul_data_rate;
        }
      }
      break;

    case PDCP_RELEASE:
      rb_data_ptr = ds_wcdma_bearer_cntxt_find_rb_data(rlc_ul_id);
      ds_wcdma_bearer_cntxt_reset_rb_data(rb_data_ptr);
      break;

    case PDCP_RELEASE_ALL:
      ds_wcdma_bearer_cntxt_reset_all_rb_data();
      break;

    default:
      break;
  }
  return;
} /* ds_wcdma_bearer_cntxt_cache_wcdma_rb_data */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_REGISTER_RRC_DATA_RATE_IND

DESCRIPTION
  This function is used to register to Data rate indication notified from W-RRC
  from PDCP.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_register_rrc_data_rate_ind( void )
{

  /*-----------------------------------------------------------------------
     If NV enabled, Register for RB_RECONFIG indications with PDCP
  -----------------------------------------------------------------------*/
  if( ds_3gpp_nv_manager_get_enable_dynamic_wcdma_wm_tuning() == TRUE )
  {
    pdcp_reg_rrc_data_rate_ind(ds_wcdma_bearer_cntxt_rrc_data_rate_ind_cb);
  }

  return;
} /* ds_wcdma_bearer_cntxt_register_rrc_data_rate_ind */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_PROC_WRRC_DATA_RATE_IND_CMD

DESCRIPTION
  This function processes WRRC Data Rate IND command.

PARAMETERS
  command pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_proc_wrrc_data_rate_ind_cmd
(
  const ds_cmd_type  *cmd_ptr
)
{
  ds_wcdma_bearer_cntxt_data_rate_ind_type  *data_rate_ind_p =NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS_WCDMA_MSG0_LOW("NULL cmd_ptr or payload ptr passed, return");
    return;
  }

  data_rate_ind_p
    = (ds_wcdma_bearer_cntxt_data_rate_ind_type*)cmd_ptr->cmd_payload_ptr;

  DS_WCDMA_MSG3_HIGH("ds_wcdma_bearer_cntxt_proc_wrrc_data_rate_ind_cmd "
                     "rb_id:%d rlc_ul_id:%d rab_ul_data_rate:%d",
                     data_rate_ind_p->rb_id,
                     data_rate_ind_p->rlc_ul_id,
                     data_rate_ind_p->rab_ul_data_rate);

  /* cache rb data based on action */
  ds_wcdma_bearer_cntxt_cache_wcdma_rb_data(data_rate_ind_p->pdcp_ind,
                                            data_rate_ind_p->rb_id,
                                            data_rate_ind_p->rlc_ul_id,
                                            data_rate_ind_p->rab_ul_data_rate);

  if( data_rate_ind_p->pdcp_ind == PDCP_DATA_RATE_IND )
  {
    ds_bearer_cntxt_config_wcdma_ul_data_rate(
       data_rate_ind_p->rb_id,
       data_rate_ind_p->rlc_ul_id,
       data_rate_ind_p->rab_ul_data_rate);
  }

  return;
} /* ds_wcdma_bearer_cntxt_proc_wrrc_data_rate_ind_cmd */

/*===========================================================================
FUNCTION DS_WCDMA_BEARER_CNTXT_GET_UL_WM_LEVELS_PER_UL_RATE

DESCRIPTION
  This function returns the statically configured uplink watermark levels
  for a given iven Uplink data rate level.

PARAMETERS
  rab_ul_data_rate: UL data rate passed by RRC
  wcdma_wm_levels_p: Out parameter filled with WM levels

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_bearer_cntxt_get_ul_wm_levels_per_ul_rate
(
  uint32                                 rab_ul_data_rate,
  ds_wcdma_bearer_cntxt_wm_levels_type  *wcdma_wm_levels_p
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( wcdma_wm_levels_p  != NULL )
  {
    /*---------------------------------------------------------------------
      UL WMs based on UL Data rate indicated by lower layers

    =======================================================================
      DATA RATE                  WM_LO     WM_HI     WM_DNE     DNE_QCNT
                                                               (WM_DNE/500)
    =======================================================================
      <= 0.7Mbps                 4500      7500      350000        700

      > 0.7Mbps && <=1.44Mbps    38000     40000     350000        700

      > 1.44Mbps && <=2.88Mbps   45000     65000     350000        700

      > 2.88Mbps && <=5.74Mbps   45000     65000     350000        700

      > 5.74Mbps                 48000     120000    350000        700
    =======================================================================
    ---------------------------------------------------------------------*/

    /* UL data rate upto 0.7 Mbps */
    if( rab_ul_data_rate <= DSWPSD_UL_RATE_0_7M )
    {
      wcdma_wm_levels_p->lo_wm_level  = DSWPSD_UL_WM_LO_0_7M;
      wcdma_wm_levels_p->hi_wm_level  = DSWPSD_UL_WM_HI_0_7M;
      wcdma_wm_levels_p->dne_wm_level = DSWPSD_UL_WM_DNE_0_7M;
      wcdma_wm_levels_p->dne_q_cnt    = DSWPSD_UL_WM_DNE_QCNT_0_7M;
    }
    /* UL data rate > 0.7 Mbps and <= 1.44 Mbps */
    else if( (rab_ul_data_rate <= DSWPSD_UL_RATE_1_44M) &&
             (rab_ul_data_rate > DSWPSD_UL_RATE_0_7M) )
    {
      wcdma_wm_levels_p->lo_wm_level  = DSWPSD_UL_WM_LO_1_44M;
      wcdma_wm_levels_p->hi_wm_level  = DSWPSD_UL_WM_HI_1_44M;
      wcdma_wm_levels_p->dne_wm_level = DSWPSD_UL_WM_DNE_1_44M;
      wcdma_wm_levels_p->dne_q_cnt    = DSWPSD_UL_WM_DNE_QCNT_1_44M;
    }
    /* UL data rate > 1.44 Mbps and <= 2.88 Mbps */
    else if( (rab_ul_data_rate <= DSWPSD_UL_RATE_2_88M) &&
             (rab_ul_data_rate > DSWPSD_UL_RATE_1_44M) )
    {
      wcdma_wm_levels_p->lo_wm_level  = DSWPSD_UL_WM_LO_2_88M;
      wcdma_wm_levels_p->hi_wm_level  = DSWPSD_UL_WM_HI_2_88M;
      wcdma_wm_levels_p->dne_wm_level = DSWPSD_UL_WM_DNE_2_88M;
      wcdma_wm_levels_p->dne_q_cnt    = DSWPSD_UL_WM_DNE_QCNT_2_88M;
    }
    /* UL data rate > 2.88 Mbps and <= 5.74 Mbps */
    else if( (rab_ul_data_rate <= DSWPSD_UL_RATE_5_74M) &&
             (rab_ul_data_rate > DSWPSD_UL_RATE_2_88M) )
    {
      wcdma_wm_levels_p->lo_wm_level  = DSWPSD_UL_WM_LO_5_74M;
      wcdma_wm_levels_p->hi_wm_level  = DSWPSD_UL_WM_HI_5_74M;
      wcdma_wm_levels_p->dne_wm_level = DSWPSD_UL_WM_DNE_5_74M;
      wcdma_wm_levels_p->dne_q_cnt    = DSWPSD_UL_WM_DNE_QCNT_5_74M;
    }
    /* UL data rate > 5.74 Mbps (currently for 11.4 Mbps) */
    else
    {
      wcdma_wm_levels_p->lo_wm_level  = DSWPSD_UL_WM_LO_11_4M;
      wcdma_wm_levels_p->hi_wm_level  = DSWPSD_UL_WM_HI_11_4M;
      wcdma_wm_levels_p->dne_wm_level = DSWPSD_UL_WM_DNE_11_4M;
      wcdma_wm_levels_p->dne_q_cnt    = DSWPSD_UL_WM_DNE_QCNT_11_4M;
    }
  }
  return;
} /* ds_wcdma_bearer_cntxt_get_wm_levels */

/*===========================================================================

                                UNIT TEST

===========================================================================*/

#endif /* FEATURE_DATA_WCDMA_PS */
