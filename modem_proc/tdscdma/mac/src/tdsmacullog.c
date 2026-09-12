/*===========================================================================
               U P L I N K   M A C    L O G G I N G

DESCRIPTION
  This file provides the functions for MAC uplink logging.

EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macullog.c_v   1.6   20 Nov 2001 16:25:52   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacullog.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
10/08/10  cyx     Remove min_backoff, max_backoff from tdsmac_ul_send_rach_param_packet
                  for there is no min_backoff , max_backoff used in TDD
10/08/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/


#include "tdscdma_variation.h"
#include "tdsmacullog.h"
#include "msg.h"
#include "tdsmacinternal.h"
#include <string.h>

#include "event.h"
#include "event_defs.h"

#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
static uint8 tdsmac_tvm_packet_perdic_rpt_times[TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_TIMES]
= {1,2,4,8,16,32,64};

static uint16 tdsmac_tvm_packet_perdic_rpt_interval[TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_INTERVAL]
={0,250,500,1000,2000,3000,4000,6000,8000,12000,16000,20000,24000,28000,32000,64000};

static uint32 tdsmac_tvm_packet_report_threshold[TDSMAC_UL_TVM_PACKET_MAX_NUM_REPORT_THRESHOLD]
={8,16,32,64,128,256,512,1024,2*1024,3*1024,4*1024,6*1024,8*1024,12*1024,16*1024,24*1024,32*1024,48*1024,64*1024,
96*1024,128*1024,192*1024,256*1024,384*1024,512*1024,768*1024};


static uint16 tdsmac_tvm_packet_time_to_trigger[TDSMAC_UL_TVM_PACKET_MAX_NUM_TIMER_TO_TRIGGER]
={0,10, 20, 40,60, 80, 100,120, 160,200, 240,320, 640,1280, 2560,5000};

static uint16 tdsmac_tvm_packet_trigger_prohibit_tmr[TDSMAC_UL_TVM_PACKET_MAX_NUM_PROHIBIT_TMR]
={250,500,1000,2000, 4000,8000,16000};

static uint16 tdsmac_tvm_packet_tx_interrupt_tmr[TDSMAC_UL_TVM_PACKET_MAX_NUM_TX_INTERRUPT_TMR]
={250,500,1000,2000, 4000,8000,16000};

/*===========================================================================

FUNCTION MAC_UL_SEND_CFG_LOG_PACKET

DESCRIPTION
   This function prepares a log packet for configured Uplink MAC Transport
   Channels and sends the packet to log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_ul_send_cfg_log_packet
(
  tdsmac_ul_config_type **config_ptr   /* Address of Ptr to Uplink MAC config */
)
{
  TDSCDMA_UL_MAC_CFG_LOG_PACKET_type *ul_cfg_ptr;/* Ptr to UL MAC config log */
                                               /* Packet                   */
  tdsmac_ul_config_type               *cfgptr;    /* Ptr to UL Config Info    */
  tdsmac_ul_rach_cctrch_config_type   *rach_ptr = NULL;  /* Ptr to Rach Info  */
  tdsmac_ul_ded_logch_config_type     *dlc_ptr;   /* Ptr to log Chan info     */
  tdsmac_ul_dch_cctrch_config_type    *dch_ptr = NULL;   /* Ptr to DCH config */
  tdsmac_ul_dch_trch_config_type      *dch_info;  /* Ptr to DCH info          */
  uint8                            *ul_log_ptr = NULL;
                                               /* Temp ptr to Log packet   */
  uint8                            num_logchan = 0;
                                               /* Num of Logical Channels  */
  uint8                            tdsnum_trch = 0;
                                               /* Num of Transport Channels*/
  int i,k;

  /* Pointer to UL MAC config.*/
  cfgptr = *config_ptr;

  if (!cfgptr)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Input to config log packet null");
    return;
  }
  else if (!cfgptr->cctrch_ptr)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Input to config log packet null");
    return;
  }
  /*-------------------------------------------------------------------------
    Calculate the total number of logical channels
  -------------------------------------------------------------------------*/

  /* RACH Type CCTRCH */
  if (cfgptr->cctrch_ptr->cctrch_type == TDSRACH)
  {
    /* Pointer to UL RACH config.*/
    rach_ptr = &cfgptr->cctrch_ptr->rach_info;

    if (rach_ptr)
    {
      /* There's only one RACH Transport Channel */
      tdsnum_trch = 1;

      /* If CCCH is enabled */
      if (rach_ptr->ccch_enable == TRUE)
      {
        num_logchan++;
      }

      /* Number of DCHs mapped to RACH */
      if (rach_ptr->ndlchan > 0)
      {
        num_logchan += rach_ptr->ndlchan;
      }
    }
  }
  else if (cfgptr->cctrch_ptr->cctrch_type == TDSDCH)
  {
    /* Pointer to UL DCH config.*/
    dch_ptr = &cfgptr->cctrch_ptr->dch_info;

    /* Number of DCHs */
     tdsnum_trch = dch_ptr->ndchs;

    for (i = 0; (i < tdsnum_trch) && (i<TDSUE_MAX_TRCH);i++)
    {
      num_logchan += dch_ptr->dch_info[i].ndlchan;
    }
  }

  /* Allocate memory here.*/
  ul_cfg_ptr = (TDSCDMA_UL_MAC_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_UL_MAC_CFG_LOG_PACKET,
                               TDSMAC_UL_CFG_PACKET_LEN(tdsnum_trch, num_logchan));

  if (ul_cfg_ptr != NULL)
  {

    ul_cfg_ptr->version_num = TDSMAC_LOG_VERSION;
    ul_cfg_ptr->num_trch = tdsnum_trch;
    // Store ciphering parameters for CS domain only
    ul_cfg_ptr->ciphering_flag = cfgptr->cipher_info[0].cipher_enable;
    ul_cfg_ptr->ciphering_key_id = cfgptr->cipher_info[0].key_idx;
    ul_cfg_ptr->hfn = cfgptr->cipher_info[0].hfn;

    ul_log_ptr = (uint8 *)ul_cfg_ptr;
        ul_log_ptr += FPOS(TDSCDMA_DL_MAC_CFG_LOG_PACKET_type,trch_info);

    if (cfgptr->cctrch_ptr->cctrch_type == TDSRACH)
    {
            if (rach_ptr == NULL)
            {
                TDSMAC_MSG0(MSG_LEGACY_HIGH,"RACH ptr is NULL");
                /* Commit the buffer to log services.*/
                log_commit(ul_cfg_ptr);

                return;
            }
      /* Store RACH Parameters */

      /* Store the Transport Channel ID */
      *(ul_log_ptr++) = (uint8)rach_ptr->trch_id;

      /* Store the Transport Channel Type */
      *(ul_log_ptr++) = (uint8)UE_UL_TRCH_RACH;

      /* Store the number of logical channels mapped to RACH */
      *(ul_log_ptr++) = num_logchan;

      /* If CCCH is enabled, store CCCH parameters */
      if (rach_ptr->ccch_enable == TRUE)
      {
        /* Store the Logical Channel ID */
        *(ul_log_ptr++) = rach_ptr->ccch_id;

        /* Store the Logical Channel Type */
        *(ul_log_ptr++) = (uint8)UE_LOGCHAN_CCCH;

        /* Store the Logical Channel Mode */
        *(ul_log_ptr++) = (uint8)UE_MODE_TRANSPARENT;

        /* Radio Bearer ID not applicable - Set it to 0 */
        *(ul_log_ptr++) = 0;

        /* MAC ID not applicable - Set it to 0 */
        *(ul_log_ptr++) = 0;

        /* MAC Priority not applicable - Set it to 0  */
        *(ul_log_ptr++) = 0;
      }

      /* For all Dedicated logical channels mapped to RACH */
      for (i = 0; (i < rach_ptr->ndlchan) && (i<MAX_DED_LOGCHAN_PER_TRCH);i++)
      {
        dlc_ptr = &rach_ptr->dlchan_info[i];

        /* Store the Logical Channel ID */
        *(ul_log_ptr++) = dlc_ptr->rlc_id;

        /* Store the Logical Channel Type */
        *(ul_log_ptr++) = (uint8)dlc_ptr->chan_type;

        /* Store the Logical Channel Mode */
        *(ul_log_ptr++) = (uint8)dlc_ptr->rlc_mode;

        /* Store the Radio Bearer ID */
        *(ul_log_ptr++) = (uint8)dlc_ptr->rb_id;

        /* Store the MAC ID */
        *(ul_log_ptr++) = dlc_ptr->mac_id;

        /* Store the MAC Priority */
        *(ul_log_ptr++) = dlc_ptr->priority;
      }

      /* Commit the buffer to log services.*/
      log_commit(ul_cfg_ptr);
    }
    else if (cfgptr->cctrch_ptr->cctrch_type == TDSDCH)
    {
            if (dch_ptr == NULL)
            {
                TDSMAC_MSG0(MSG_LEGACY_HIGH,"DCH ptr is NULL");
                /* Commit the buffer to log services.*/
                log_commit(ul_cfg_ptr);
                return;
            }
      /* For all DCHs */
      for (i = 0; (i < dch_ptr->ndchs) && (i<TDSUE_MAX_TRCH);i++)
      {
        dch_info = &dch_ptr->dch_info[i];

        /* Store the Transport Channel ID */
        *(ul_log_ptr++) = (uint8)dch_info->trch_id;

        /* Store the Transport Channel Type */
        *(ul_log_ptr++) = (uint8)UE_TRCH_DCH;


        /* Store the Number of Logical Channels mapped to DCH */
        *(ul_log_ptr++) = (uint8)dch_info->ndlchan;

        for (k = 0; (k < dch_ptr->dch_info[i].ndlchan) && (k<MAX_DED_LOGCHAN_PER_TRCH);k++)
        {
          dlc_ptr = &dch_info->dlchan_info[k];

          /* Store the Logical Channel ID */
          *(ul_log_ptr++) = dlc_ptr->rlc_id;

          /* Store the Logical Channel Type */
          *(ul_log_ptr++) = (uint8)dlc_ptr->chan_type;

          /* Store the Logical Channel Mode */
          *(ul_log_ptr++) = (uint8)dlc_ptr->rlc_mode;

          /* Store the Radio Bearer ID */
          *(ul_log_ptr++) = (uint8)dlc_ptr->rb_id;

          /* Store the MAC ID */
          *(ul_log_ptr++) = dlc_ptr->mac_id;

          /* Store the MAC Priority */
          *(ul_log_ptr++) = dlc_ptr->priority;
        }
      }
      /* Commit the buffer to log services.*/
      log_commit(ul_cfg_ptr);
    }
    else
    {
       TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid CCTrCh type %d",cfgptr->cctrch_ptr->cctrch_type);
       /* Commit the buffer to log services.*/
       log_commit(ul_cfg_ptr);
    }
  }
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_UL_MAC_CFG_LOG_PACKET,log_status(TDSCDMA_UL_MAC_CFG_LOG_PACKET));
  }
}/* tdsmac_ul_send_cfg_log_packet() */


/*===========================================================================

FUNCTION MAC_UL_SEND_RACH_PARAM_PACKET

DESCRIPTION
   This function prepares a log packet for RACH Transmit parameters and sends
   the packet to log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_ul_send_rach_param_packet
(
  tdsmac_ul_rach_txcntl_config_type *rach_tx_prmtrs /* Ptr to RACH TX params  */
)
{
  TDSCDMA_MAC_RACH_CFG_LOG_PACKET_type *ul_rach_ptr;/* Ptr to Rach Tx params */
                                                  /* log packet            */
  int i;

  /* Allocate memory here */
  ul_rach_ptr = (TDSCDMA_MAC_RACH_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_MAC_RACH_CFG_LOG_PACKET,
                             TDSMAC_RACH_CFG_PACKET_LEN(rach_tx_prmtrs->nASC));

  /* Store RACH parameters.*/
  if (ul_rach_ptr != NULL)
  {
    ul_rach_ptr->version_num = TDSMAC_LOG_VERSION;
    ul_rach_ptr->max_num_preamble_cycles = rach_tx_prmtrs->mmax;

    ul_rach_ptr->num_asc = rach_tx_prmtrs->nASC;

    /* Store Information for each Access service class */
    for (i = 0; i < rach_tx_prmtrs->nASC; i++)
    {
      ul_rach_ptr->asc_info[i].identifier = 0;
      ul_rach_ptr->asc_info[i].persistence =
                                          (uint8)((rach_tx_prmtrs->asc[i].pval * 100)/(0xFFFF));
    }
    /* Commit the buffer to log services.*/
    log_commit(ul_rach_ptr);
  }
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_RACH_CFG_LOG_PACKET,log_status(TDSCDMA_MAC_RACH_CFG_LOG_PACKET));
  }
}/* tdsmac_ul_send_rach_param_packet() */


/*===========================================================================

FUNCTION MAC_UL_SEND_TFCS_CFG_PACKET

DESCRIPTION
   This function prepares a log packet for Uplink MAC TFCS configuration
   and sends the packet to log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_ul_send_tfcs_cfg_packet
(
  tdsmac_ul_tfcs_type *tfcs_ptr,             /* Ptr to TFCS info              */
  uint8            *tdstrch_id               /* Transport Channel IDs         */
)
{
  TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type *ul_tfcs_ptr;/* Ptr to MAC TFCS    */
  uint8                                 *ul_log_ptr; /* Ptr for accessing  */
                                                     /* contiguous memory  */
  int i,k;

  /* Allocate memory here.*/
  ul_tfcs_ptr = (TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET,
                TDSMAC_UL_TFCS_CFG_PACKET_LEN(tfcs_ptr->ntfc, tfcs_ptr->nchan));

  if (ul_tfcs_ptr != NULL)
  {
    ul_tfcs_ptr->version_num = TDSMAC_LOG_VERSION;
    ul_log_ptr = (uint8 *)ul_tfcs_ptr;
    ul_log_ptr += FPOS(TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type,num_tfc);

    // Fill in num tfc
    *ul_log_ptr = (uint8)(tfcs_ptr->ntfc & 0xFF);
    *(ul_log_ptr+1) = (uint8)((tfcs_ptr->ntfc & 0xFF00)>>(uint8)8);


    ul_log_ptr += 2;

    // Fill in number of channels
    *(ul_log_ptr++) = tfcs_ptr->nchan;

    /* Store the Transport channels IDs */
    for (i = 0; (i < tfcs_ptr->nchan) && (i<TDSUE_MAX_TRCH);i++)
    {
      *(ul_log_ptr++) = tdstrch_id[i];
    }
    /* Store the TTI info for each transport channel */
    for (i = 0; (i < tfcs_ptr->nchan)&& (i<TDSUE_MAX_TRCH);i++)
    {
      *(ul_log_ptr++) = (uint8)tfcs_ptr->tfs_info[i].tti_info;
    }

    for (i = 0; i < tfcs_ptr->ntfc; i++)
    {
      /* Store the TFC valid flag for each Transport Channel */
      *(ul_log_ptr++) = (uint8)tfcs_ptr->tfc_info[i].control_flag;

      /* For each Transport Format, store the TFI for each transport channel */
      for (k = 0; (k < tfcs_ptr->nchan) && (k<TDSUE_MAX_TRCH);k++)
      {
        /* Store the Transport Format Indicator */
        *(ul_log_ptr++) = tfcs_ptr->tfc_info[i].tfi[k];
      }
    }
    /* Commit the buffer to log services.*/
    log_commit(ul_tfcs_ptr);
  }
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET,log_status(TDSCDMA_MAC_UL_TFCS_CFG_LOG_PACKET));
  }
}/* tdsmac_ul_send_tfcs_cfg_packet() */
/*===========================================================================

FUNCTION MAC_UL_CONVERT_TVM_BUF_PAYLOAD

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
uint32 tdsmac_ul_convert_tvm_buf_payload
(
  uint32    payload
)
{
  uint32    ret_value=2;
  
  if (payload==0)
  {
    ret_value=0;
  }
  
  while(payload>0)
  {
    payload = payload>>1;
    ret_value *= 2;
  }
  
  return ret_value;
}

/*===========================================================================

FUNCTION MAC_UL_SEND_TVM_REPORT_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void tdsmac_ul_send_tvm_report_packet
    (
    tdsmac_ul_traffic_meas_ind_type *tvm_rpt_ptr,
    uint16        nreports_sent
    )
{
    TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type   *log_tvm_rpt_ptr;
    int i;

    /* Allocate memory here.*/
    log_tvm_rpt_ptr = (TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type   *)log_alloc(TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET,
        TDSMAC_UL_TVM_REPORT_PACKET_LEN(tvm_rpt_ptr->num_rbs));

    if (log_tvm_rpt_ptr != NULL)
    {
        log_tvm_rpt_ptr->version_num = TDSMAC_LOG_VERSION;
        log_tvm_rpt_ptr->meas_id = (uint8)tvm_rpt_ptr->meas_id;
        log_tvm_rpt_ptr->num_rbs = (uint8)tvm_rpt_ptr->num_rbs;

        switch (tvm_rpt_ptr->report_mode)
        {
        case TDSPERIODIC_MODE :
            log_tvm_rpt_ptr->num_reports_sent = nreports_sent;
            log_tvm_rpt_ptr->report_mode = 0;
            log_tvm_rpt_ptr->event_id = 0xff;
            log_tvm_rpt_ptr->trch_id = 0xff;
            log_tvm_rpt_ptr->trch_type = 0xff;

            break;

        case TDSEVENT_TRIGGERED_MODE :
            log_tvm_rpt_ptr->report_mode = 1;
            log_tvm_rpt_ptr->event_id = (uint8)tvm_rpt_ptr->event_result.event_id-1 ;
            log_tvm_rpt_ptr->trch_id = (uint8)tvm_rpt_ptr->event_result.trch_info.trch_id;
            log_tvm_rpt_ptr->trch_type = (uint8)tvm_rpt_ptr->event_result.trch_info.trch_type;
            log_tvm_rpt_ptr->num_reports_sent = 0xFF;
            break;

        default:
            TDSMAC_MSG0(MSG_LEGACY_ERROR,"Invalid Mode");
            break;
        }

        /* Store the Transport channels IDs */
        for (i = 0; (i < tvm_rpt_ptr->num_rbs) && (i < TDS_MAX_RB);i++)
        {
            log_tvm_rpt_ptr->meas_result_per_rb[i].rb_id = (uint32)tvm_rpt_ptr->rb_meas[i].rb_id;
            log_tvm_rpt_ptr->meas_result_per_rb[i].raw_payload =
                tdsmac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].raw_rlc_payload);
            log_tvm_rpt_ptr->meas_result_per_rb[i].avg_payload =
                tdsmac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].average_rlc_payload);
            log_tvm_rpt_ptr->meas_result_per_rb[i].var_payload =
                tdsmac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].variance_rlc_payload);
        }

        /* Commit the buffer to log services.*/
        log_commit(log_tvm_rpt_ptr);
    }
    else
    {
        TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET,log_status(TDSCDMA_MAC_UL_TVM_REPORT_LOG_PACKET));
    }
}/* tdsmac_ul_send_tfcs_cfg_packet() */



/*===========================================================================

FUNCTION TDSCDMA_MAC_UL_TVM_LOG_FIND_PERDIC_RPT_TIMES

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of report times.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_perdic_rpt_times(tdsmac_ul_traffic_periodic_report_cfg_type *periodic_report)
{
  uint8 i;
  
  if(periodic_report->report_infinity)
  {
    /* periodic report is infinity */
    return TDSMAC_UL_TVM_PACKET_PERDIC_RPT_TIMES_INFINITY;
  }

  for(i=0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_TIMES;i++)
  {
    if(periodic_report->num_reports == tdsmac_tvm_packet_perdic_rpt_times[i])
    {
      return i;
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid periodic report num in TVM config req command:%d",periodic_report->num_reports);
  
  return TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_TIMES;
}
/*===========================================================================

FUNCTION TDSCDMA_MAC_UL_TVM_LOG_FIND_PERDIC_RPT_INTERVAL

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of report interval.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_perdic_rpt_interval(tdsmac_ul_traffic_periodic_report_cfg_type *periodic_report)
{
  uint8 i;

  for(i=0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_INTERVAL;i++)
  {
    if(periodic_report->report_interval == tdsmac_tvm_packet_perdic_rpt_interval[i])
    {
      return i;
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid periodic report interval in TVM config req command:%d",periodic_report->report_interval);

  return TDSMAC_UL_TVM_PACKET_MAX_NUM_PERDIC_RPT_INTERVAL;
}
/*===========================================================================

FUNCTION tdsmac_ul_tvm_log_find_report_threshold

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of report threshold.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_report_threshold(uint32    report_threshold)
{
  uint8 i;
  
  for(i = 0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_REPORT_THRESHOLD; i++)
  {
    if(report_threshold == tdsmac_tvm_packet_report_threshold[i])
    {
      return i;
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid report threshold in TVM config req command:%d",report_threshold);

  return TDSMAC_UL_TVM_PACKET_MAX_NUM_REPORT_THRESHOLD;
}
/*===========================================================================

FUNCTION tdsmac_ul_tvm_log_find_time_to_trigger

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of time to trigger.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_time_to_trigger(uint16    time_to_trigger)
{
  uint8 i;
  
  for(i = 0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_TIMER_TO_TRIGGER; i++)
  {
    if(time_to_trigger == tdsmac_tvm_packet_time_to_trigger[i])
    {
      return i;
    }
  }
  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid time to trigger in TVM config req command:%d",time_to_trigger);

  return TDSMAC_UL_TVM_PACKET_MAX_NUM_TIMER_TO_TRIGGER;
}
/*===========================================================================

FUNCTION tdsmac_ul_tvm_log_find_prohibit_tmr

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of prohibit tmr.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_prohibit_tmr(uint16    prohibit_tmr)
{
  uint8 i;
  
  for(i = 0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_PROHIBIT_TMR; i++)
  {
    if(prohibit_tmr == tdsmac_tvm_packet_trigger_prohibit_tmr[i])
    {
      return i;
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid prohibit tmr in TVM config req command:%d",prohibit_tmr);

  return TDSMAC_UL_TVM_PACKET_MAX_NUM_PROHIBIT_TMR;
}
/*===========================================================================

FUNCTION tdsmac_ul_tvm_log_find_tx_interrupt_tmr

DESCRIPTION
    
DEPENDENCIES
    None.

RETURN VALUE
    enum value of tx interrupt tmr.

SIDE EFFECTS
    None.

===========================================================================*/
uint8 tdsmac_ul_tvm_log_find_tx_interrupt_tmr(uint16    tx_interrupt_tmr)
{
  uint8 i;
  
  for(i = 0;i< TDSMAC_UL_TVM_PACKET_MAX_NUM_TX_INTERRUPT_TMR; i++)
  {
    if(tx_interrupt_tmr == tdsmac_tvm_packet_tx_interrupt_tmr[i])
    {
      return i;
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Invalid tx interrupt tmr in TVM config req command:%d",tx_interrupt_tmr);

  return TDSMAC_UL_TVM_PACKET_MAX_NUM_TX_INTERRUPT_TMR;
}
/*===========================================================================

FUNCTION TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void tdsmac_ul_tvm_cfg_log_packet
(
 tdsmac_ul_traffic_meas_config_type const *tvm_cfg_ptr
 )
{

  int i,j;
  uint8 num_event = 0;
  uint8 num_trch  = 0;
  uint8 num_meas_obj = 0;

  tdsmac_ul_traffic_event_trigger_cfg_type    *tvm_config_event_trigger = NULL;
  tdsmac_ul_traffic_trch_event_trigger_param  *tvm_config_event_param = NULL;
  TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type      *log_tvm_cfg_ptr = NULL;
  tdsmac_log_meas_event_info_type             *meas_event_info_ptr = NULL;

  if((TDSMAC_TRAFFIC_MEAS_START != tvm_cfg_ptr->meas_cmd) &&
      (TDSMAC_TRAFFIC_MEAS_MODIFY != tvm_cfg_ptr->meas_cmd))
  {
    log_tvm_cfg_ptr = (TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET,
        sizeof(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type));
    
    if (log_tvm_cfg_ptr != NULL)
    {
      log_tvm_cfg_ptr->version_num = TDSMAC_LOG_VERSION;
      log_tvm_cfg_ptr->meas_action = (uint8)tvm_cfg_ptr->meas_cmd;
      if(TDSMAC_TRAFFIC_MEAS_DELETE_ALL != tvm_cfg_ptr->meas_cmd)
      {
        log_tvm_cfg_ptr->meas_id = (uint8)tvm_cfg_ptr->meas_id;
      }
      log_commit(log_tvm_cfg_ptr);
    }
    else
    {
      TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET,log_status(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET));
    }
    return;
  }
  

  if(TDSEVENT_TRIGGERED_MODE == tvm_cfg_ptr->meas_report_mode.report_mode)
  {
    if(tvm_cfg_ptr->meas_report_criteria.event_cfg.trch_info_incl) 
    {
      /* event trigger for listed trch */
      num_trch = tvm_cfg_ptr->meas_report_criteria.event_cfg.num_trch;

      for(i = 0; i < num_trch; i++)
      {
        num_event += tvm_cfg_ptr->meas_report_criteria.event_cfg.trch_event_cfg[i].num_events;
      }

    }/*  trch_info_incl is true */   
    else
    {
      /* event trigger for all trch */
      num_trch = 1;  
      num_event += tvm_cfg_ptr->meas_report_criteria.event_cfg.trch_event_cfg[0].num_events;
    }

  }/* report_mode is event trigger */

  if (tvm_cfg_ptr->meas_obj_incl)
  {
    /*  meas obj info is included in config */
    num_meas_obj = tvm_cfg_ptr->meas_obj_list.num_trch;
  }

  /* Allocate memory here.*/
  log_tvm_cfg_ptr = (TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET,
        sizeof(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET_type));

  if (log_tvm_cfg_ptr != NULL)
  {
    log_tvm_cfg_ptr->version_num            = TDSMAC_LOG_VERSION;
    log_tvm_cfg_ptr->meas_id                = (uint8)tvm_cfg_ptr->meas_id;
    log_tvm_cfg_ptr->meas_action            = (uint8)tvm_cfg_ptr->meas_cmd;

    
    if(tvm_cfg_ptr->meas_qty_incl)
    {
      /* measurement quantity information is included */
      
      log_tvm_cfg_ptr->meas_quant_type        = (uint8)tvm_cfg_ptr->meas_qty.meas_qty_enum;
      log_tvm_cfg_ptr->time_interval          = (uint8)tvm_cfg_ptr->meas_qty.meas_interval;
    }
    else
    {
      /* Not indicated  */
      log_tvm_cfg_ptr->meas_quant_type        = TDSMAC_UL_TVM_CFG_LOG_NO_INDICATE;
      log_tvm_cfg_ptr->time_interval          = TDSMAC_UL_TVM_CFG_LOG_NO_INDICATE;
    }

    log_tvm_cfg_ptr->meas_report_quan_type  = (tvm_cfg_ptr->meas_report_qty.rlc_payload) +
                        (tvm_cfg_ptr->meas_report_qty.avg_payload >> TDSMAC_UL_TVM_PACKET_MASK_AVG_PAYLOAD) +
                               (tvm_cfg_ptr->meas_report_qty.var_payload >> TDSMAC_UL_TVM_PACKET_MASK_VAR_PAYLOAD) ;

    log_tvm_cfg_ptr->trans_mode             = (uint8)tvm_cfg_ptr->meas_report_mode.transfer_mode;

    log_tvm_cfg_ptr->report_mode            = (uint8)tvm_cfg_ptr->meas_report_mode.report_mode;

    if (tvm_cfg_ptr->meas_obj_incl)
    {
      /* measurement object information is included */
      log_tvm_cfg_ptr->num_trch               = num_meas_obj;
    }
    else
    {
      /* apply the measurement reporting criteria to all uplink */
      log_tvm_cfg_ptr->num_trch               = TDSMAC_UL_TVM_CFG_LOG_ALL_TRCH;
    }

    log_tvm_cfg_ptr->num_trch_event         = num_trch;

    if(TDSPERIODIC_MODE == tvm_cfg_ptr->meas_report_mode.report_mode)
    {
      /* save perdic measurement info */
      log_tvm_cfg_ptr->periodic_info.reporting_amt = 
          tdsmac_ul_tvm_log_find_perdic_rpt_times((tdsmac_ul_traffic_periodic_report_cfg_type *)&(tvm_cfg_ptr->meas_report_criteria.periodic_cfg));

      log_tvm_cfg_ptr->periodic_info.reporting_interval = 
          tdsmac_ul_tvm_log_find_perdic_rpt_interval((tdsmac_ul_traffic_periodic_report_cfg_type *)&(tvm_cfg_ptr->meas_report_criteria.periodic_cfg));
    }
    else
    {
      /* save event measurement info */
      if( log_tvm_cfg_ptr->num_trch != TDSMAC_UL_TVM_CFG_LOG_ALL_TRCH)
      { 
	  	if(TDSUE_MAX_TRCH < log_tvm_cfg_ptr->num_trch)
        {
          TDSMAC_MSG1(MSG_LEGACY_ERROR,"num_trch %d exceed TDSUE_MAX_TRCH",log_tvm_cfg_ptr->num_trch);
          return;
        }
        /* save meas_obj[TDSUE_MAX_TRCH] */
        for(i = 0; i < log_tvm_cfg_ptr->num_trch; i++)
        {
          if(TDSEDCH == tvm_cfg_ptr->meas_obj_list.trch_info[i].trch_type)
          {
            log_tvm_cfg_ptr->meas_obj[i].trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_USCH;
            log_tvm_cfg_ptr->meas_obj[i].trch_id   = TDSMAC_UL_TVM_PACKET_EDCH_ID;
          }
          else
          {
            if(TDSRACH == tvm_cfg_ptr->meas_obj_list.trch_info[i].trch_type)
            {
              log_tvm_cfg_ptr->meas_obj[i].trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_RACH;
            }
            else if (TDSDCH == tvm_cfg_ptr->meas_obj_list.trch_info[i].trch_type)
            {
              log_tvm_cfg_ptr->meas_obj[i].trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_DCH;
            }

            log_tvm_cfg_ptr->meas_obj[i].trch_id   = tvm_cfg_ptr->meas_obj_list.trch_info[i].trch_id;

          }/* no edch type */

        }/* save meas_obj[TDSUE_MAX_TRCH] */

      }/* num_trch != TDSCDMA_MAC_UL_TVM_CFG_LOG_ALL_TRCH */
     
      /* save event_info[TDSUE_MAX_TRCH] */
      
      meas_event_info_ptr = log_tvm_cfg_ptr->event_info;
      
      tvm_config_event_trigger = (tdsmac_ul_traffic_event_trigger_cfg_type *)&(tvm_cfg_ptr->meas_report_criteria.event_cfg);
     
      for(i = 0; i < num_trch; i++)
      {
        if(TDSEDCH == tvm_config_event_trigger->trch_event_cfg[i].trch_info.trch_type)
        {
          meas_event_info_ptr->trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_USCH;
          meas_event_info_ptr->trch_id = TDSMAC_UL_TVM_PACKET_EDCH_ID;
        }
        else
        {
          if(TDSRACH == tvm_config_event_trigger->trch_event_cfg[i].trch_info.trch_type)
          {
            meas_event_info_ptr->trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_RACH;
          }
          else
          {
            meas_event_info_ptr->trch_type = TDSMAC_UL_TVM_PACKET_TRCH_TYPE_DCH;
          }
         
          meas_event_info_ptr->trch_id = tvm_config_event_trigger->trch_event_cfg[i].trch_info.trch_id;
        }
        meas_event_info_ptr->num_event = tvm_config_event_trigger->trch_event_cfg[i].num_events;

        for(j= 0; j <tvm_config_event_trigger->trch_event_cfg[i].num_events; j++)
        {
          
          tvm_config_event_param = &tvm_config_event_trigger->trch_event_cfg[i].event_param[j];
          
          if(TDSEVENT_4A == tvm_config_event_param->event_id) 
          {
            meas_event_info_ptr->trch_event_info.event_id = TDSMAC_UL_TVM_PACKET_EVENT_4A;
          }
          else
          {
            meas_event_info_ptr->trch_event_info.event_id = TDSMAC_UL_TVM_PACKET_EVENT_4B;
          }
          
          /* threshold (0..25) to enumerate  */
          meas_event_info_ptr->trch_event_info.report_threshold =
            tdsmac_ul_tvm_log_find_report_threshold(tvm_config_event_param->report_threshold);

          /* Time to trigger (0..15) to enumerate  */
          meas_event_info_ptr->trch_event_info.time_to_trigger = 
            tdsmac_ul_tvm_log_find_time_to_trigger(tvm_config_event_param->time_to_trigger);
          
          /* Pending time after trigger (0..6) */
          meas_event_info_ptr->trch_event_info.pending_time = 
            tdsmac_ul_tvm_log_find_prohibit_tmr(tvm_config_event_param->trigger_prohibit_tmr);
          
          /* Tx interruption after trigger (0..6) */
          meas_event_info_ptr->trch_event_info.tx_intrupt = 
            tdsmac_ul_tvm_log_find_tx_interrupt_tmr(tvm_config_event_param->tx_interrupt_tmr);

        }/* for( log_meas_event_info[i].num_event ) */

        meas_event_info_ptr++;

      }/* for( tvm_cfg_ptr->meas_report_criteria.event_cfg.num_trch )  */
    }/* save event measurement info */

    /* Commit the buffer to log services.*/
    log_commit(log_tvm_cfg_ptr);

  }/* log_tvm_cfg_ptr != NULL */
  else
  {
      TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET,log_status(TDSCDMA_MAC_UL_TVM_CFG_LOG_PACKET));
  }

}/* tdsmac_ul_tvm_cfg_log_packet() */


#endif /* FEATURE_TDSCDMA_MAC_QXDM_LOGGING */
