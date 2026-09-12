/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         U P L I N K    M A C   H S

GENERAL DESCRIPTION

    This module contains the data definitions and procedures for the performing
    the following functions

    1. HSUPA Configuration and setup


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2011-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                     EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculhslog.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
02/02/11   ps       Commenting out the log packet code for now.
                    will change later for HSUPA
10/24/10   guo      Initial release Upmerged WCDMA to TDS

===========================================================================*/


/*===========================================================================

                  INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_TDSCDMA_HSUPA
#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#include "task.h"
#include "err.h"
#include "tdsmacrrcif.h"
#include "tdsmacul.h"
#include "tdsmaculhs.h"
#include "tdsmaculhslog.h"
#include "tdsmaculsubtask.h"
#include "tdsmaculhssubtask.h"
/*===========================================================================

              DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
                         GLOBAL DATA
---------------------------------------------------------------------------*/

rex_timer_type tdsmac_hsupa_tmr_log_blk;    /* Rex timer used for qxdm loggin  */
TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type  tdscdma_mac_hs_ul_etfci_sts_log;
TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET_type tdscdma_mac_hs_ul_hdr_sts_log;

/*===========================================================================

                     GENERAL PURPOSE MACROS

===========================================================================*/

/*---------------------------------------------------------------------------

---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                         LOCAL FUNCTIONS
---------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION: tdscdma_mac_hs_ul_config_log_packet

DESCRIPTION:
   log out uplink hs config information

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/
void tdscdma_mac_hs_ul_config_log_packet(tdsmac_ul_config_type    *cfgptr)
{
#if 0 // for
  TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET_type   *hs_ul_config_log_packet = NULL;
  uint8 tmp_nasc = 0;
  uint8 tmp_num_macd_flows = 0;
  uint8 tmp_num_logch = 0;
  uint8 tmp_num_rlc_size_ddi_info = 0;


  tdsmac_hs_ul_log_mac_d_flow_info_s_type*    mac_log_d_flow_ptr = NULL;
  tdsmac_hs_ul_log_logch_info_s_type*         mac_hs_lch_log_ptr = NULL;
  tdsmac_hs_ul_log_rlc_size_ddi_info_s_type*  mac_hs_rlc_size_ddi_ptr = NULL;
  uint32  i,j,k;

  
  if((TDSMAC_EUL_STOP != cfgptr->mac_eul_action) &&
        (TDSMAC_EUL_NOOP != cfgptr->mac_eul_action))
  { /* 
      when action is not stop&&noop calculate sum of nasc,d-flowlch,rlc size ddi
      else there is no information to log out
    */
    tmp_nasc = cfgptr->mac_e_config_ptr->e_rucch_info.nASC;
    tmp_num_macd_flows =  cfgptr->mac_e_config_ptr->num_e_mac_d_flow;

    for(i=0; i < tmp_num_macd_flows; i++)
    {
      tmp_num_logch += 
         cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].num_logch;
      for(j = 0; j < cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].num_logch; j++)
      {
        tmp_num_rlc_size_ddi_info +=
          cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].num_rlc_size_ddi_info;
      }
    }
  }
  
  hs_ul_config_log_packet = 
      (TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET_type *)log_alloc(TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET,
          sizeof(TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET_type));

  if(hs_ul_config_log_packet != NULL)
  {
    hs_ul_config_log_packet->version = TDSMAC_HS_LOG_VERSION;

    /* 
       translate action mode value from tdsmac_eul_action_e_type to 
       TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET_type  
    */ 
    if(TDSMAC_EUL_CONFIG == cfgptr->mac_eul_action)
    {
      hs_ul_config_log_packet->mac_eul_action = TDSMAC_HS_UL_ACT_LOG_START;
    }
    else if(TDSMAC_EUL_START == cfgptr->mac_eul_action)
    {
      hs_ul_config_log_packet->mac_eul_action = TDSMAC_HS_UL_ACT_LOG_RECONFIG;
    }
    else if(TDSMAC_EUL_STOP == cfgptr->mac_eul_action)
    {
      hs_ul_config_log_packet->mac_eul_action = TDSMAC_HS_UL_ACT_LOG_STOP;
    }
    else if(TDSMAC_EUL_NOOP == cfgptr->mac_eul_action)
    {
      hs_ul_config_log_packet->mac_eul_action = TDSMAC_HS_UL_ACT_LOG_NOOP;
    }
    
    hs_ul_config_log_packet->mac_e_reset_ind = cfgptr->mac_e_reset_indicator;

    /*
      translate action mode value from tdsmac_e_tsn_action_e_type to 
      TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET_type 
    */
    if(TDSMAC_E_TSN_ACTION_BACKUP == cfgptr->mac_e_tsn_action)
    {
      hs_ul_config_log_packet->mac_e_tsn_action  = TDSMAC_HS_UL_TSN_ACT_LOG_BACKUP;
    }
    else if(TDSMAC_E_TSN_ACTION_REVERT == cfgptr->mac_e_tsn_action)
    {
      hs_ul_config_log_packet->mac_e_tsn_action  = TDSMAC_HS_UL_TSN_ACT_LOG_REVERT;
    }

    hs_ul_config_log_packet->nasc = tmp_nasc;
    hs_ul_config_log_packet->num_macd_flows = tmp_num_macd_flows; 
    
    if((TDSMAC_EUL_STOP != cfgptr->mac_eul_action)  &&
          (TDSMAC_EUL_NOOP != cfgptr->mac_eul_action))
    {
      /*
      if action mode is not stop&&noop then save information
      else skip there is not information to log out
      */
      hs_ul_config_log_packet->t_rucch_timer = cfgptr->mac_e_config_ptr->e_rucch_info.t_rucch_tmr;
      hs_ul_config_log_packet->ernti = cfgptr->mac_e_config_ptr->e_rucch_info.ernti;
      hs_ul_config_log_packet->nrucch = cfgptr->mac_e_config_ptr->e_rucch_info.n_rucch;
      hs_ul_config_log_packet->mmax  = cfgptr->mac_e_config_ptr->e_rucch_info.mmax;


      hs_ul_config_log_packet->si_alone_max_retran_tmr = 
                        cfgptr->mac_e_config_ptr->eul_si_info.e_retx_tmr_for_si_only;

      hs_ul_config_log_packet->wait_tmr = cfgptr->mac_e_config_ptr->eul_si_info.t_wait_tmr;

      hs_ul_config_log_packet->si_alone_max_retan = cfgptr->mac_e_config_ptr->
                eul_si_info.e_max_num_of_retx_for_si_only;
      hs_ul_config_log_packet->non_scded_peridic_si_tmr =  cfgptr->mac_e_config_ptr->
                eul_si_info.t_non_Sched_periodic_si_tmr;

      hs_ul_config_log_packet->periodic_si_tmr = cfgptr->mac_e_config_ptr->eul_si_info.t_Sched_periodic_si_tmr;
      hs_ul_config_log_packet->extended_est_tmr = cfgptr->mac_e_config_ptr->eul_si_info.ext_est_window;
      hs_ul_config_log_packet->serv_cell_change = cfgptr->mac_e_config_ptr->eul_si_info.srv_cell_or_work_freq_change;
      
      
      for(i = 0; i < tmp_nasc; i++)
      {
        hs_ul_config_log_packet->pval[i] = cfgptr->mac_e_config_ptr->e_rucch_info.asc[i].pval;
      }
      
      /* fill ernti_change  */
      hs_ul_config_log_packet->ernti_change  = cfgptr->mac_e_config_ptr->eul_si_info.ernti_change;

      /* fill num_macd_flows */
      hs_ul_config_log_packet->num_macd_flows  = tmp_num_macd_flows;
      mac_log_d_flow_ptr = hs_ul_config_log_packet->e_mac_d_flow_info;

      for(i = 0; i < tmp_num_macd_flows; i++)
      {
        /* uint8   e_mac_d_flow_id */
        mac_log_d_flow_ptr->e_mac_d_flow_id =
                        cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_mac_d_flow_id;

        /* uint8   e_mac_d_flow_power_offset */
        mac_log_d_flow_ptr->e_mac_d_flow_power_offset =
                        cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_mac_d_flow_power_offset;

        /* uint8   e_mac_d_flow_max_num_of_retx */
        mac_log_d_flow_ptr->e_mac_d_flow_max_num_of_retx = 
                      cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_mac_d_flow_max_num_of_retx;

        /* uint8   e_mac_d_flow_max_retran_tmr */
        mac_log_d_flow_ptr->e_mac_d_flow_max_retran_tmr =
                      cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_mac_d_flow_retx_tmr;

        /* uint8   e_mac_d_flow_mux_list */
        mac_log_d_flow_ptr->e_mac_d_flow_mux_list = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_mac_d_flow_mux_list;

        /* uint8   e_tx_grant_info */
        mac_log_d_flow_ptr->e_tx_grant_info = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_tx_grant_info;
        
          /* uint8   num_logch */
        mac_log_d_flow_ptr->num_logch = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].num_logch;
        mac_hs_lch_log_ptr = mac_log_d_flow_ptr->e_ul_logch_info;

        /* tdsmac_hs_ul_log_logch_info_s_type */
        for(j= 0; j < cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].num_logch; j++)
        {
          /* uint8  rb_id */
          mac_hs_lch_log_ptr->rb_id = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].rb_id;

          /* uint8   rlc_id */
          mac_hs_lch_log_ptr->rlc_id = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].rlc_id;

          /* uint8   priority */
          mac_hs_lch_log_ptr->priority = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].priority;

          /* uint8  chan_type */
          mac_hs_lch_log_ptr->chan_type = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].chan_type;

          /* uint8  rlc_mode */
          mac_hs_lch_log_ptr->rlc_mode = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].rlc_mode;

          /* boolean  include_in_scheduling_info */
          mac_hs_lch_log_ptr->include_in_scheduling_info = 
            cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].include_in_scheduling_info;

           /* uint8  num_rlc_size_ddi_info */
          mac_hs_lch_log_ptr->num_rlc_size_ddi_info = 
            cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].num_rlc_size_ddi_info;
          
          mac_hs_rlc_size_ddi_ptr = mac_hs_lch_log_ptr->rlc_size_ddi_info;
             
          for(k = 0; k < cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].num_rlc_size_ddi_info; k++)
          {
            /* uint8 ddi */
            mac_hs_rlc_size_ddi_ptr->ddi = cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].rlc_size_ddi_info[k].ddi;


            /* uint16 rlc_pdu_size_in_bits */
            mac_hs_rlc_size_ddi_ptr->rlc_pdu_size_in_bits = 
              cfgptr->mac_e_config_ptr->e_mac_d_flow_info[i].e_ul_logch_info[j].rlc_size_ddi_info[k].rlc_pdu_size_in_bits;
            mac_hs_rlc_size_ddi_ptr++;
          
          }/* loop  num_rlc_size_ddi_info */ 
          mac_hs_lch_log_ptr++;
        
        } /* loop num_logch */
        mac_log_d_flow_ptr++;
      
      }/* loop num_macd_flows */

    }/* TDSMAC_EUL_STOP != eul action */

  /* Commit the buffer to log services. */
  log_commit(hs_ul_config_log_packet);

  }/* hs_ul_config_log_packet != NULL */

  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET,log_status(TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET));
  }
#endif
}

/*===========================================================================
FUNCTION: tdscdma_mac_hs_ul_header_status_log_packet

DESCRIPTION:
    This fucntion is called from tdsl2ultask.c when the status log timer expires

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/
void tdscdma_mac_hs_ul_header_status_log_packet(void)
{
  TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET_type* tdsmac_hs_ul_hdr_sts_log_ptr = NULL;
  tdsmac_hs_ul_log_header_status_pkt_type*         hs_ul_header_status_pkt_ptr = NULL;
  uint32 j,k;
  uint32 num_mac_hdrs = 0;
  uint32 num_si = 0;

  for(j = 0;j < tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl; j++)
  {
    num_mac_hdrs +=  tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].num_mac_hdrs;

    if(tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_present)
    {
      num_si++;
    }
  }

  tdsmac_hs_ul_hdr_sts_log_ptr = (TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET_type *)
    log_alloc(TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET,
        sizeof(TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET_type));
  
  if(NULL != tdsmac_hs_ul_hdr_sts_log_ptr)
  {
    tdsmac_hs_ul_hdr_sts_log_ptr->nsamp_tti_etfci_tbl =
        tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl;

    tdsmac_hs_ul_hdr_sts_log_ptr->start_fn =
        tdscdma_mac_hs_ul_hdr_sts_log.start_fn;

    tdsmac_hs_ul_hdr_sts_log_ptr->start_harq_id_start_fn = 
        tdscdma_mac_hs_ul_hdr_sts_log.start_harq_id_start_fn;

    hs_ul_header_status_pkt_ptr = tdsmac_hs_ul_hdr_sts_log_ptr->hdr_pkt_info;
    
    for(j= 0; j < tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl; j++)
    {
      /* uint8 tx_type */
      hs_ul_header_status_pkt_ptr->tx_type =
                  tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].tx_type;

      /* uint8     etfci */
      hs_ul_header_status_pkt_ptr->etfci =
                  tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].etfci;

      /* uint8     num_mac_hdrs */
      hs_ul_header_status_pkt_ptr->num_mac_hdrs = 
                  tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].num_mac_hdrs;

      /* uint8     si_present   */
      hs_ul_header_status_pkt_ptr->si_present = 
                  tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_present;

      for(k = 0; k < tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].num_mac_hdrs; k++)
      {
        /* uint8     mac_e_hdr[TDSMAC_HS_UL_NUM_MAC_E_HDRS][3] */
        hs_ul_header_status_pkt_ptr->mac_e_hdr[k][0] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].mac_e_hdr[k][0];

        hs_ul_header_status_pkt_ptr->mac_e_hdr[k][1] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].mac_e_hdr[k][1];

        hs_ul_header_status_pkt_ptr->mac_e_hdr[k][2] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].mac_e_hdr[k][2];
      }/* k < num_mac_hdrs_si_present */

      if(tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_present)
      {
        /* uint8     si_content[4] */
        hs_ul_header_status_pkt_ptr->si_content[0] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_content[0];
        hs_ul_header_status_pkt_ptr->si_content[1] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_content[1];
        hs_ul_header_status_pkt_ptr->si_content[2] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_content[2];
        hs_ul_header_status_pkt_ptr->si_content[3] = 
          tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[j].si_content[3];
      }
      hs_ul_header_status_pkt_ptr++;

    }/* j < nsamp_tti_etfci_tbl */

    /* Commit the buffer to log services.*/
    log_commit(tdsmac_hs_ul_hdr_sts_log_ptr);

  }/* NULL != tdsmac_hs_ul_hdr_sts_log_ptr */
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET,log_status(TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET));
  }

  memset(&tdscdma_mac_hs_ul_hdr_sts_log,0,sizeof(TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET_type));

}/* tdscdma_mac_hs_ul_header_status_log_packet */
/*===========================================================================
FUNCTION        tdsmac_ulhs_get_si_cause

DESCRIPTION
  This function calulcates the HLID and HLID buffer status,num of lch that 
  have data.

RETURN VALUE
  none

SIDE EFFECTS
none
===========================================================================*/
void tdsmac_ulhs_get_si_cause
(
   tdsl1_eul_etfci_enum_type trans_type
)
{
  uint8 index;
  uint8 *si_ptr;

  index = tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl;
  
  si_ptr = &tdscdma_mac_hs_ul_etfci_sts_log.etfci_status_pkt_info[index].si_cause;

  if(((EUL_MAC_E_PDU_ON_EPUCH == trans_type)&&(tdsmac_ulhs_int_bld_mace_pdu_info.si_included))
    || (EUL_SI_ALONE_ON_EPUCH == trans_type) || (EUL_GRANT_SI_ON_ERUCCH == trans_type) 
    || (EUL_ERUCCH_GRANT_SI_AND_RTX_EPUCH == trans_type))
  {
    switch(tdsmac_ulhs_si_info.current_si_trigger_reason)
    {
    case SI_INVAID_REASON:
      break;
    case SI_TEBS_NOW_ABVE_ZERO:
      *si_ptr = TDSMAC_HS_SI_LOG_TEBS_NOW_ABVE_ZERO;
      break;
    case SI_NO_GRANT_TEBS_ABOVE_ZERO:
      *si_ptr = TDSMAC_HS_SI_LOG_WAIT_TMR_EXPIRY;
      break;
    case SI_SRV_CELL_CHANGE:
      *si_ptr = TDSMAC_HS_SI_LOG_SRVCELL_CHANGE;
      break;
    case SI_ERNTI_CHANGE:
      *si_ptr = TDSMAC_HS_SI_LOG_ERNTI_CHANGE;
      break;
    case SI_PERIODIC_TMR_EXP:
      *si_ptr = TDSMAC_HS_SI_LOG_PERIODIC_TMR_EXP;
      break;
    case SI_HARQ_FAIL_RETRANS:
      *si_ptr = TDSMAC_HS_SI_LOG_HARQ_RETRANSMIT;
      break;
    case SI_QUANTISATION_OF_DATA:
      *si_ptr = TDSMAC_HS_SI_LOG_QUANTISATION_OF_DATA;
      break;
    case SI_HIGH_PRIO_LOG_CHAN:
      *si_ptr = TDSMAC_HS_SI_HIGH_PRIO_LOG_CHAN;
      break;
    }
  }
   
}
 /*===========================================================================
FUNCTION        tdsmac_ulhs_get_lch_info

DESCRIPTION
  This function calulcates the HLID and HLID buffer status,num of lch that 
  have data.

RETURN VALUE
  none

SIDE EFFECTS
none
===========================================================================*/
void tdsmac_ulhs_get_lch_info
(
 uint32 index
)
{
  /* Stores the rlc_id */
  rlc_lc_id_type               rlc_id;
  
  uint8 lc_id;
  uint32 i;
  uint8 find_flag = 0;
  uint8 num_lid = 0;
  tdsmac_hs_ul_log_etfci_status_pkt_type  *etfci_status_ptr = NULL;
  etfci_status_ptr = &tdscdma_mac_hs_ul_etfci_sts_log.etfci_status_pkt_info[index];

  for (i=0;i< tdsmac_hs_ul_lc_info.nchan;i++)
  {
    rlc_id = tdsmac_hs_ul_lc_info.rlc_id[i];
    
    /*Get LC-ID provoded by the N\W*/
    
    lc_id = tdsmac_ulhs_rlc_id_mac_dflow_lc_id_mapping[rlc_id].lc_id;
    
    /*Since tdsmac_hs_ul_lc_info is stored in the ascending order of Prio i.e. the highest Prio is stored in idx 0
    * and so.. The first LC which has data will be HLID as thats the highest prio LC
    */
    if(tdsmac_hs_ul_lc_buf_status.chan_info[rlc_id].nbytes > 0)
    {
      if(0 == find_flag)
      {
        etfci_status_ptr->etfci_status_info.high_log_chan_buffer = tdsmac_hs_ul_lc_buf_status.chan_info[rlc_id].nbytes << 3;
        etfci_status_ptr->etfci_status_info.high_log_chan_id = lc_id;
        find_flag++;
      }
      num_lid++;
    
    }
  }
  etfci_status_ptr->etfci_status_info.num_log_chan_having_data = num_lid;
}
/*===========================================================================
FUNCTION: tdsmac_log_fill_si_in_hdr_sts

DESCRIPTION:
    This fucntion collect si info save into globle varbile 
 tdscdma_mac_hs_ul_hdr_sts_log.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/
void tdsmac_log_fill_si_in_hdr_sts(uint8 hlid,uint8 tebs,uint8 hlbs,uint8 uph, uint8 snpl)
{
  uint32 index;

  tdsmac_hs_ul_log_header_status_pkt_type*   hs_ul_hdr_sts_ptr = NULL;

  index = tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl;

  if(index >= TDSMAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT)
  {
    return;
  }

  hs_ul_hdr_sts_ptr = &tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[index];

  if(TRUE == hs_ul_hdr_sts_ptr->si_present)
  {
    /* hlid : .  4bit
       tebs : *  5bit
       hlbs : !  4bit
       uph  : #  5bit
       snpl : %  5bit
       SI trigger : & 1bit  (filled in tdsmac_log_fill_hs_ul_hdr_sts)
       SI padding : ^ 1bit  (filled in tdsmac_log_fill_hs_ul_hdr_sts)

              0             1       2         3      
    si:   | ....**** | *!!!!### | ##%%%%%& |^0000000 |

    */
    hs_ul_hdr_sts_ptr->si_content[0] |= ((hlid << 4)|(tebs >> 1));
    hs_ul_hdr_sts_ptr->si_content[1] |= ((tebs << 7)|(hlbs << 3)|(uph >> 2));
    hs_ul_hdr_sts_ptr->si_content[2] |= ((uph << 6) | (snpl << 1));
    tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl++;
  }  
}
/*===========================================================================
FUNCTION: tdsmac_log_fill_hs_ul_hdr_sts

DESCRIPTION:
    This fucntion collect hdr info save into globle varbile 
 tdscdma_mac_hs_ul_hdr_sts_log.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/
void tdsmac_log_fill_hs_ul_hdr_sts(
   tdsl1_eul_edch_ctrl_info_struct_type *tdsmac_eul_etfci_return_info_ptr,
   tdsl1_eul_sg_upd_info_struct_type    *sg_info_ptr)
{
  uint32 index,i;

  tdsmac_hs_ul_log_header_status_pkt_type*   hs_ul_hdr_sts_ptr = NULL;

  if(tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl < 
      TDSMAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT)
  {
    index = tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl;

    if(!index)
    {
      /* this is the first frame  number in this log packet */
      tdscdma_mac_hs_ul_hdr_sts_log.start_fn = tdsmac_hs_ul_cur_cfn;

      tdscdma_mac_hs_ul_hdr_sts_log.start_harq_id_start_fn =
        sg_info_ptr->harq_id;
    }

    hs_ul_hdr_sts_ptr = &tdscdma_mac_hs_ul_hdr_sts_log.hdr_pkt_info[index];
    hs_ul_hdr_sts_ptr->tx_type = sg_info_ptr->tx_status;
    hs_ul_hdr_sts_ptr->etfci = tdsmac_eul_etfci_return_info_ptr->etfci_status;
    hs_ul_hdr_sts_ptr->si_present = FALSE;

    if(EUL_MAC_E_PDU_ON_EPUCH == tdsmac_eul_etfci_return_info_ptr->etfci_status)
    {
      hs_ul_hdr_sts_ptr->num_mac_hdrs = 
              tdsmacul_hs_ddi_info.rlc_pdu_info.nchan;
      
      for(i = 0; i < hs_ul_hdr_sts_ptr->num_mac_hdrs && i < TDSMAX_HSUPA_LOGCHAN; i++)
      {
        /* DDI */
        hs_ul_hdr_sts_ptr->mac_e_hdr[i][0] = tdsmacul_hs_ddi_info.mac_e_hdr_info[2*i];
        /* TSN */
        hs_ul_hdr_sts_ptr->mac_e_hdr[i][1] = (tdsmacul_hs_ddi_info.rlc_pdu_info.chan_fmt[i].mac_es_hdr >> 2);
        /* N */
        hs_ul_hdr_sts_ptr->mac_e_hdr[i][2] = tdsmacul_hs_ddi_info.mac_e_hdr_info[2*i+1];
      }

      if(tdsmac_ulhs_int_bld_mace_pdu_info.si_included)
      {
        hs_ul_hdr_sts_ptr->si_present = TRUE;

        if (SI_QUANTISATION_OF_DATA  == tdsmac_ulhs_si_info.current_si_trigger_reason)
        {
          /* si_trig is 0, LSB in si_content[2] */
          /* si_pad is 1, MSB in si_content[3]  */
          hs_ul_hdr_sts_ptr->si_content[3] = TDSMAC_SI_PAD_TRIGGER;
        }
        else
        {
          /* si_trig is 1, LSB in si_content[2] */
          hs_ul_hdr_sts_ptr->si_content[2] = TDSMAC_SI_EVENT_TRIGGER;
          /* si_pad is 0, MSB in si_content[3]  */
        }
      }

    }
    else if(EUL_SI_ALONE_ON_EPUCH == tdsmac_eul_etfci_return_info_ptr->etfci_status)
    {
      hs_ul_hdr_sts_ptr->si_present = TRUE;

      /* si_trig is 0, LSB in si_content[2] */
      /* si_pad is 1, MSB in si_content[3]  */
      hs_ul_hdr_sts_ptr->si_content[3] = TDSMAC_SI_PAD_TRIGGER;
    }
    else if((EUL_GRANT_SI_ON_ERUCCH  == tdsmac_eul_etfci_return_info_ptr->etfci_status)||
      (EUL_NON_GRANT_SI_ON_ERUCCH  == tdsmac_eul_etfci_return_info_ptr->etfci_status) ||
      (EUL_ERUCCH_GRANT_SI_AND_RTX_EPUCH == tdsmac_eul_etfci_return_info_ptr->etfci_status))
    {

      hs_ul_hdr_sts_ptr->si_present = TRUE;

      /* si_trig is 1, LSB in si_content[2] */
      hs_ul_hdr_sts_ptr->si_content[2] = TDSMAC_SI_EVENT_TRIGGER;
      /* si_pad is 0, MSB in si_content[3]  */;
    }
    else if(EUL_NO_MAC_E_PDU == tdsmac_eul_etfci_return_info_ptr->etfci_status)
    {
     
    }

    if(FALSE == hs_ul_hdr_sts_ptr->si_present)
    {
      tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl++;
    }

  }//tdscdma_mac_hs_ul_hdr_sts_log.nsamp_tti_etfci_tbl < TDSMAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT

}
/*===========================================================================
FUNCTION: tdsmac_log_fill_hs_ul_etfci_sts

DESCRIPTION:
    This fucntion collect etfci info save into globle varbile 
 tdscdma_mac_hs_ul_etfci_sts_log.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/
void tdsmac_log_fill_hs_ul_etfci_sts(
   tdsl1_eul_edch_ctrl_info_struct_type *tdsmac_eul_etfci_return_info,
   tdsl1_eul_sg_upd_info_struct_type    *sg_info)
{
  uint32 index;
  uint32 mac_dflow_sel;
  tdsmac_hs_ul_log_etfci_status_pkt_type  *etfci_status_ptr = NULL;

  if(tdscdma_mac_hs_ul_etfci_sts_log.nsamp_tti_etfci_tbl < 
    TDSMAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT)
  {
    index = tdscdma_mac_hs_ul_etfci_sts_log.nsamp_tti_etfci_tbl;

    if(!index)
    {
      /* this is the first frame  number in this log packet */
      tdscdma_mac_hs_ul_etfci_sts_log.start_fn = tdsmac_hs_ul_cur_cfn;
      tdscdma_mac_hs_ul_etfci_sts_log.start_harq_id_start_fn = sg_info->harq_id;
    }

    etfci_status_ptr = &tdscdma_mac_hs_ul_etfci_sts_log.etfci_status_pkt_info[index];

    etfci_status_ptr->tx_type =  sg_info->tx_status;

    etfci_status_ptr->etfci_ret_status = tdsmac_eul_etfci_return_info->etfci_status;

    /* fill si_cause */
    tdsmac_ulhs_get_si_cause(tdsmac_eul_etfci_return_info->etfci_status);

    etfci_status_ptr->asc_info = tdsmac_eul_etfci_return_info->asc_index;

    etfci_status_ptr->sched_buf_status = tdsmac_ulhs_int_status_info.sched_buf_status;

    etfci_status_ptr->non_sched_buf_status = tdsmac_ulhs_int_status_info.non_sched_buf_status;

    etfci_status_ptr->etfci_status_info.etfci = tdsmac_eul_etfci_return_info->etfci;

    etfci_status_ptr->etfci_status_info.timeslot = sg_info->timeslot_available;

    etfci_status_ptr->etfci_status_info.min_etfci = tdsmac_ulhs_int_bld_mace_pdu_info.min_etfci_idx_sel;

    etfci_status_ptr->etfci_status_info.max_etfci = tdsmac_ulhs_int_bld_mace_pdu_info.max_etfci_idx_sel;

    mac_dflow_sel = tdsmac_ulhs_int_bld_mace_pdu_info.highest_prio_mac_dflow_sel;

    etfci_status_ptr->etfci_status_info.dflow_mux_list=
       tdsmaculhs_config_info.maculhs_mac_dflow_info[mac_dflow_sel].mux_list;
    
    etfci_status_ptr->etfci_status_info.dummy_wrd = 0;
    
    tdsmac_ulhs_get_lch_info( index);

    tdscdma_mac_hs_ul_etfci_sts_log.nsamp_tti_etfci_tbl++;
  }
}
/*===========================================================================
FUNCTION: tdscdma_mac_hs_ul_etfci_status_log_packet

DESCRIPTION:
    This fucntion is called from tdsl2ultask.c when the status log timer expires


DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

===========================================================================*/

void tdscdma_mac_hs_ul_etfci_status_log_packet( void)
{
  uint32 j;
  TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type* tdsmac_hs_ul_etfci_sts_log_ptr = NULL;
  TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type* tbl_hs_ul_etfci_sts_ptr = NULL;
  tdsmac_hs_ul_log_etfci_status_pkt_type*         hs_ul_etfci_status_pkt_ptr = NULL;
  
  tbl_hs_ul_etfci_sts_ptr = &(tdscdma_mac_hs_ul_etfci_sts_log);

  tdsmac_hs_ul_etfci_sts_log_ptr = (TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type *)
      log_alloc(TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET,
          sizeof(TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type));

  if(NULL != tdsmac_hs_ul_etfci_sts_log_ptr)
  {
    tdsmac_hs_ul_etfci_sts_log_ptr->nsamp_tti_etfci_tbl = 
                    tbl_hs_ul_etfci_sts_ptr->nsamp_tti_etfci_tbl;

    tdsmac_hs_ul_etfci_sts_log_ptr->start_fn = tbl_hs_ul_etfci_sts_ptr->start_fn;

    tdsmac_hs_ul_etfci_sts_log_ptr->start_harq_id_start_fn = 
                        tbl_hs_ul_etfci_sts_ptr->start_harq_id_start_fn;

    hs_ul_etfci_status_pkt_ptr = tdsmac_hs_ul_etfci_sts_log_ptr->etfci_status_pkt_info;

    for(j = 0; j < tbl_hs_ul_etfci_sts_ptr->nsamp_tti_etfci_tbl; j++)
    {
      /* uint8 tx_type */
      hs_ul_etfci_status_pkt_ptr->tx_type =
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].tx_type;

      /* uint8 etfci_ret_status */
      hs_ul_etfci_status_pkt_ptr->etfci_ret_status =
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_ret_status;

      /* uint8 si_cause */
      hs_ul_etfci_status_pkt_ptr->si_cause = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].si_cause;

      /* uint8 asc_info */
      hs_ul_etfci_status_pkt_ptr->asc_info = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].asc_info;

      /* uint32    sched_buf_status */
      hs_ul_etfci_status_pkt_ptr->sched_buf_status = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].sched_buf_status;

      /* uint32    non_sched_buf_status */
      hs_ul_etfci_status_pkt_ptr->non_sched_buf_status = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].non_sched_buf_status;

      /* uint8 etfci */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.etfci = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.etfci;

      /* uint8 timeslot */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.timeslot = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.timeslot;

      /* uint8 min_etfci */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.min_etfci = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.min_etfci;

      /*  uint8 max_etfci */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.max_etfci = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.max_etfci;

      /* uint8 dflow_mux_list */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.dflow_mux_list = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.dflow_mux_list;

      /* uint8 high_log_chan_id */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.high_log_chan_id = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.high_log_chan_id;

	  /* uint16 dummy_wrd */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.dummy_wrd = 0;
        
      /* uint32 high_log_chan_buffer */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.high_log_chan_buffer = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.high_log_chan_buffer;

      /* uint8 num_log_chan_having_data */
      hs_ul_etfci_status_pkt_ptr->etfci_status_info.num_log_chan_having_data = 
        tbl_hs_ul_etfci_sts_ptr->etfci_status_pkt_info[j].etfci_status_info.num_log_chan_having_data;
      hs_ul_etfci_status_pkt_ptr++;
    }
    /* Commit the buffer to log services.*/
    log_commit(tdsmac_hs_ul_etfci_sts_log_ptr);

  }/* NULL != tdsmac_hs_ul_hdr_sts_log_ptr */
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET,log_status(TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET));
  }
  memset(&tdscdma_mac_hs_ul_etfci_sts_log,0,sizeof(TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET_type));
}/* tdscdma_mac_hs_ul_etfci_status_log_packet */


#endif // FEATURE_MAC_HS_UL_QXDM_LOGGING
#endif // FEATURE_TDSCDMA_HSUPA
