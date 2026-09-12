/*===========================================================================
               U P L I N K   M A C    L O G G I N G

DESCRIPTION
  This file provides the functions for MAC uplink logging.

EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2001 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002-2003, 2005-2009 Qualcomm Technologies, Incorporated.  
All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macullog.c_v   1.6   20 Nov 2001 16:25:52   rchar  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macullog.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
04/12/16  kv      FR 32607: Web Boosting to improve latency by initiating TVM so that NW can move UE to DCH
09/28/15  as      MAC to delay event 6D trigger by limiting the ETFCI selection in UL inorder to avoid 
                  UE hitting MTPL due to data traffic.
07/22/15  kv      FR 22377: RAVE-API
07/16/15  aa      L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
06/15/15  ac      Fix compilation errors for BO
06/08/15  as      W+W MAC UL/DL HS task variable doubling.
03/22/15  as      Added new NV item 73667 to control FR22545 changes.
03/03/15  as      SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
                  based upon L1 operating power and delay 6D trigger by limiting number of bits to be transmitted.
05/01/15  scv     W+W Changes to have log packets support SubID
04/25/15  scv     W+W Changes to have messages support SubID
04/10/15  kv      Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
04/08/15  as      L2-RRC interdependent changes for W+W
03/19/15  aa      Changes for new log packet 0x41CE
03/19/15  aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
03/10/15  aa      Duplicated L2 task related globals and removed AS_ID hardcodings
02/20/15  aa      Mainlined FEATURE_MAC_QXDM_LOGGING
01/29/15  kc      L2 global reorg for W+W
10/21/14  ts      SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
07/26/14  rkmk    Reduce image size by using right macro depending upon number of parameters
08/07/13  kc      Fixed Compilation Error
07/06/12  kc      Log packet support for UL MAC cipher parameters
04/29/13  kv      Moved F3s under debug flag & added QXDM support for tvm_debug
11/7/12   ts      Fixed KW errors
10/12/12  kc      HS_RACH MAC F3's moved under debug variable and x4314 log packet issue
07/13/12  grk     MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
01/02/12  grk     Feature cleanup
05/25/11  grk     Fix to log correct RACH persistence value.
11/03/10  grk     Added support to set RLC/MAC debug variables through QXDM command.
06/16/10  mrg     Lint warnings are fixed
05/11/10  mrg     In the TVM log packet 0x4002 the payload computation logics are fixed.
05/07/09  ssg     Updated Copyright Information
04/23/09  mrg     Function mac_ul_event_amr_rate() is removed.
10/27/08  ssg     Added function mac_ul_event_amr_rate(). When AMR rate is changed
                  EVENT_WCDMA_UL_AMR_RATE event is posted.
02/10/07  sk      Klocwork fixes
01/08/07  sk      Fixed the compiler warnings
02/05/06  sk      Cast the pointer to WCDMA_UL_MAC_CFG_LOG_PACKET_type to remove compiler
                  warnings.
09/21/05  gsc     Fixed coverity errors.
03/09/05  sk      Fixed critical lint errors.
11/10/03  sk      Added additional checks to see that the number of times the
                  FOR loop is executed is bounded by the UE_MAX value.
07/09/03  sk      Populated the ciphering related parameters for CS domain only.
06/17/03  sk      Populated the ciphering related parameters.
11/15/02  sk      Added function mac_ul_send_tvm_report_packet():
08/02/02  sk      mac_ul_send_cfg_log_packet(): Commented out code related to ciphering.
11/20/01  rc      Added fix to commit log packet when an Invalid CCTRCH type
                  is received.
10/24/01  rc      Changed assignment order of tfc_valid field and array of
                  TFIs with tfc_valid field now being assigned first in
                  mac_ul_send_tfcs_cfg_packet().
10/19/01  ps      Fixed compiler warnings
09/13/01  rc      Set identifier value to 0 in mac_ul_send_rach_param_packet
                  for each access service class.
08/28/01  rc      Fixed bug with multiple logical channels mapped to a
                  Transport Channel.
08/24/01  rc      Moved FEATURE definition to after the include definitions.
08/21/01  rc      Created file.

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/


#include "wcdma_variation.h"
#include "macdlhs.h"
#include "macullog.h"
#include "msg.h"
#include "macinternal.h"
#include "macrlcif.h"
#include <string.h>

#include "event.h"
#include "event_defs.h"
#include "maculhs.h"
#include "wcdmadiag.h"

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
void mac_ul_send_cfg_log_packet
(
  mac_ul_config_type **config_ptr,  /* Address of Ptr to Uplink MAC config */
  mac_ul_table_type *ul_table_ptr
)
{
  WCDMA_UL_MAC_CFG_LOG_PACKET_type *ul_cfg_ptr;/* Ptr to UL MAC config log */
                                               /* Packet                   */
  mac_ul_config_type               *cfgptr;    /* Ptr to UL Config Info    */
  mac_ul_rach_cctrch_config_type   *rach_ptr = NULL;  /* Ptr to Rach Info  */
  mac_ul_ded_logch_config_type     *dlc_ptr;   /* Ptr to log Chan info     */
  mac_ul_dch_cctrch_config_type    *dch_ptr = NULL;   /* Ptr to DCH config */
  mac_ul_dch_trch_config_type      *dch_info;  /* Ptr to DCH info          */
  uint8                            *ul_log_ptr = NULL;
                                               /* Temp ptr to Log packet   */
  uint8                            num_logchan = 0;
                                               /* Num of Logical Channels  */
  uint8                            num_trch = 0;
                                               /* Num of Transport Channels*/
  int i,k;

  /* Pointer to UL MAC config.*/
  cfgptr = *config_ptr;

  if (!cfgptr)
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "Input to config log packet null");
    return;
  }
  else if (!cfgptr->cctrch_ptr)
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "Input to config log packet null");
    return;
  }
  /*-------------------------------------------------------------------------
    Calculate the total number of logical channels
  -------------------------------------------------------------------------*/

  /* RACH Type CCTRCH */
  if (cfgptr->cctrch_ptr->cctrch_type == RACH)
  {
    /* Pointer to UL RACH config.*/
    rach_ptr = &cfgptr->cctrch_ptr->rach_info;

    if (rach_ptr)
    {
      /* There's only one RACH Transport Channel */
      num_trch = 1;

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
  else if (cfgptr->cctrch_ptr->cctrch_type == DCH)
  {
    /* Pointer to UL DCH config.*/
    dch_ptr = &cfgptr->cctrch_ptr->dch_info;

    /* Number of DCHs */
     num_trch = dch_ptr->ndchs;

    for (i = 0; (i < num_trch) && (i<UE_MAX_TRCH);i++)
    {
      num_logchan += dch_ptr->dch_info[i].ndlchan;
    }
  }

  /* Allocate memory here.*/
  ul_cfg_ptr = (WCDMA_UL_MAC_CFG_LOG_PACKET_type *)wcdma_log_alloc(WCDMA_UL_MAC_CFG_LOG_PACKET,
                               MAC_UL_CFG_PACKET_LEN(num_trch, num_logchan), ul_table_ptr->as_id);
                               
  if (ul_cfg_ptr != NULL)
  {
  
    ul_cfg_ptr->num_trch = num_trch;
    // Store ciphering parameters for CS domain only
    ul_cfg_ptr->ciphering_flag = cfgptr->cipher_info[0].cipher_enable;
    ul_cfg_ptr->ciphering_key_id = cfgptr->cipher_info[0].key_idx;
    ul_cfg_ptr->hfn = cfgptr->cipher_info[0].hfn;
    
    ul_log_ptr = (uint8 *)ul_cfg_ptr;
        ul_log_ptr += FPOS(WCDMA_DL_MAC_CFG_LOG_PACKET_type,trch_info);
        
    if (cfgptr->cctrch_ptr->cctrch_type == RACH)
    {
      if (rach_ptr == NULL)
      {
         WL2_MSG_0(HIGH, ul_table_ptr , "RACH ptr is NULL");
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
    else if (cfgptr->cctrch_ptr->cctrch_type == DCH)
    {
            if (dch_ptr == NULL)
            {
                WL2_MSG_0(HIGH, ul_table_ptr , "DCH ptr is NULL");
                /* Commit the buffer to log services.*/
                log_commit(ul_cfg_ptr);
                return;
            }
      /* For all DCHs */
      for (i = 0; (i < dch_ptr->ndchs) && (i<UE_MAX_TRCH);i++)
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
       WL2_MSG_1(HIGH, ul_table_ptr , "Invalid CCTrCh type %d",
                                                   cfgptr->cctrch_ptr->cctrch_type);
       /* Commit the buffer to log services.*/
       log_commit(ul_cfg_ptr);
    }
  }
  else
  {
    WL2_MSG_2(LOW, ul_table_ptr, "Dropped 0x%x, code enabled: %d\n",
                                              WCDMA_UL_MAC_CFG_LOG_PACKET,
                                              log_status(WCDMA_UL_MAC_CFG_LOG_PACKET));
  }
}/* mac_ul_send_cfg_log_packet() */


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
void mac_ul_send_rach_param_packet
(
  mac_ul_rach_txcntl_config_type *rach_tx_prmtrs, /* Ptr to RACH TX params  */
  mac_ul_table_type *ul_table_ptr
)
{
  WCDMA_MAC_RACH_CFG_LOG_PACKET_type *ul_rach_ptr;/* Ptr to Rach Tx params */
                                                  /* log packet            */
  int i;

  /* Allocate memory here */
  ul_rach_ptr = (WCDMA_MAC_RACH_CFG_LOG_PACKET_type *)wcdma_log_alloc(WCDMA_MAC_RACH_CFG_LOG_PACKET,
                             MAC_RACH_CFG_PACKET_LEN(rach_tx_prmtrs->nASC), ul_table_ptr->as_id);

  /* Store RACH parameters.*/
  if (ul_rach_ptr != NULL)
  {
    ul_rach_ptr->max_num_preamble_cycles = rach_tx_prmtrs->mmax;

    ul_rach_ptr->min_backoff = rach_tx_prmtrs->nbo1min;

    ul_rach_ptr->max_backoff = rach_tx_prmtrs->nbo1max;

    ul_rach_ptr->num_asc = rach_tx_prmtrs->nASC;

    /* Store Information for each Access service class */
    for (i = 0; i < rach_tx_prmtrs->nASC; i++)
    {
      ul_rach_ptr->asc_info[i].identifier = 0;
      /*Convert persistence value in the range 0 - 0xFFFF to 0 - 100*/
      ul_rach_ptr->asc_info[i].persistence =
                                          (uint8)((rach_tx_prmtrs->asc[i].pval * 100)/(0xFFFF));
    }
    /* Commit the buffer to log services.*/
    log_commit(ul_rach_ptr);
  }
  else
  {
    WL2_MSG_2(LOW, ul_table_ptr, "Dropped 0x%x, code enabled: %d\n",
                                              WCDMA_MAC_RACH_CFG_LOG_PACKET,
                                              log_status(WCDMA_MAC_RACH_CFG_LOG_PACKET));
  }
}/* mac_ul_send_rach_param_packet() */


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
void mac_ul_send_tfcs_cfg_packet
(
  mac_ul_tfcs_type *tfcs_ptr,             /* Ptr to TFCS info              */
  uint8            *trch_id,               /* Transport Channel IDs         */
  mac_ul_table_type *ul_table_ptr
)
{
  WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type *ul_tfcs_ptr;     /* Ptr to MAC TFCS    */
  uint8                                 *ul_log_ptr, tfi; /* Ptr for accessing  */
                                                          /* contiguous memory  */
  int i,k;

  /* Allocate memory here.*/
  ul_tfcs_ptr = (WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type *)wcdma_log_alloc(WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET,
                MAC_UL_TFCS_CFG_PACKET_LEN(tfcs_ptr->ntfc, tfcs_ptr->nchan), ul_table_ptr->as_id);

  if (ul_tfcs_ptr != NULL)
  {
    ul_log_ptr = (uint8 *)ul_tfcs_ptr;
    ul_log_ptr += FPOS(WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET_type,num_tfc);

    // Fill in num tfc
    *ul_log_ptr = (uint8)(tfcs_ptr->ntfc & 0xFF);
    *(ul_log_ptr+1) = (uint8)((tfcs_ptr->ntfc & 0xFF00)>>(uint8)8);
    
    ul_log_ptr += 2;
    
    // Fill in number of channels
    *(ul_log_ptr++) = tfcs_ptr->nchan;
    
    /* Store the Transport channels IDs */
    for (i = 0; (i < tfcs_ptr->nchan) && (i<UE_MAX_TRCH);i++)
    {
      *(ul_log_ptr++) = trch_id[i];
    }
    /* Store the TTI info for each transport channel */
    for (i = 0; (i < tfcs_ptr->nchan)&& (i<UE_MAX_TRCH);i++)
    {
      *(ul_log_ptr++) = (uint8)tfcs_ptr->tfs_info[i].tti_info;
    }
    
    for (i = 0; i < tfcs_ptr->ntfc; i++)
    {
      /* Store the TFC valid flag for each Transport Channel */
      *(ul_log_ptr++) = (uint8)tfcs_ptr->tfc_info[i].control_flag;
      
      /* For each Transport Format, store the TFI for each transport channel */
      for (k = 0; (k < tfcs_ptr->nchan) && (k<UE_MAX_TRCH);k++)
      {
        tfi = tfcs_ptr->tfc_info[i].tfi[k];
        /* Store the Transport Format Indicator */
        *(ul_log_ptr++) = tfcs_ptr->tfs_info[k].tf_info[tfi].ntblks;
      }
    }
    /* Commit the buffer to log services.*/
    log_commit(ul_tfcs_ptr);
  }
  else
  {
    WL2_MSG_2(LOW, ul_table_ptr, "Dropped 0x%x, code enabled: %d\n",
                                              WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET,
                                              log_status(WCDMA_MAC_UL_TFCS_CFG_LOG_PACKET));
  }
}/* mac_ul_send_tfcs_cfg_packet() */
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
uint32 mac_ul_convert_tvm_buf_payload
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
void mac_ul_send_tvm_report_packet
(
  mac_ul_traffic_meas_ind_type *tvm_rpt_ptr,
  uint16        nreports_sent,
  mac_ul_table_type *ul_table_ptr
)
{
  WCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type   *log_tvm_rpt_ptr;
  int i;
  
  /* Allocate memory here.*/
  log_tvm_rpt_ptr = (WCDMA_MAC_UL_TVM_REPORT_LOG_PACKET_type   *)wcdma_log_alloc(WCDMA_MAC_UL_TVM_REPORT_LOG_PACKET,
      MAC_UL_TVM_REPORT_PACKET_LEN(tvm_rpt_ptr->num_rbs), ul_table_ptr->as_id);
      
  if (log_tvm_rpt_ptr != NULL)
  {
    log_tvm_rpt_ptr->meas_id = (uint8)tvm_rpt_ptr->meas_id;
    log_tvm_rpt_ptr->num_rbs = (uint8)tvm_rpt_ptr->num_rbs;
    
    switch (tvm_rpt_ptr->report_mode)
    {
    case PERIODIC_MODE :
        log_tvm_rpt_ptr->num_reports_sent = nreports_sent;
        log_tvm_rpt_ptr->report_mode = 0;
        log_tvm_rpt_ptr->event_id = 0xff;
        log_tvm_rpt_ptr->trch_id = 0xff;
        log_tvm_rpt_ptr->trch_type = 0xff;
        
        break;
        
    case EVENT_TRIGGERED_MODE :
        log_tvm_rpt_ptr->report_mode = 1;
        log_tvm_rpt_ptr->event_id = (uint8)tvm_rpt_ptr->event_result.event_id-1 ;
        log_tvm_rpt_ptr->trch_id = (uint8)tvm_rpt_ptr->event_result.trch_info.trch_id;
        log_tvm_rpt_ptr->trch_type = (uint8)tvm_rpt_ptr->event_result.trch_info.trch_type;
        log_tvm_rpt_ptr->num_reports_sent = 0xFF;
        break;
        
    default:
        WL2_MSG_0(ERROR, ul_table_ptr, "Invalid Mode");
        break;
    }
    
    /* Store the Transport channels IDs */
    for (i = 0; i < tvm_rpt_ptr->num_rbs;i++)
    {
      if(i<MAX_RB)
      {
        log_tvm_rpt_ptr->meas_result_per_rb[i].rb_id = (uint32)tvm_rpt_ptr->rb_meas[i].rb_id;
        log_tvm_rpt_ptr->meas_result_per_rb[i].raw_payload =
            mac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].raw_rlc_payload);
        log_tvm_rpt_ptr->meas_result_per_rb[i].avg_payload =
            mac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].average_rlc_payload);
        log_tvm_rpt_ptr->meas_result_per_rb[i].var_payload =
            mac_ul_convert_tvm_buf_payload(tvm_rpt_ptr->rb_meas[i].variance_rlc_payload);
      }
      else
      {
        WL2_MSG_2(ERROR, ul_table_ptr, "Index %d for measurement result per radio bearer exceeds maximum no. of radio bearers %d",
                                                    i, (int)MAX_RB);
      }
    }
    
    /* Commit the buffer to log services.*/
    log_commit(log_tvm_rpt_ptr);
  }
  else
  {
    WL2_MSG_2(LOW, ul_table_ptr, "Dropped 0x%x, code enabled: %d\n",
                                              WCDMA_MAC_UL_TVM_REPORT_LOG_PACKET,
                                              log_status(WCDMA_MAC_UL_TVM_REPORT_LOG_PACKET));
  }
}/* mac_ul_send_tfcs_cfg_packet() */

/*===========================================================================

FUNCTION MAC_UL_SEND_CFN_LOG_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

===========================================================================*/
void mac_ul_send_cfn_log_packet(uint8 nsamples, mac_ul_table_type *ul_table_ptr)
{

  WCDMA_MAC_UL_CIPHER_LOG_PACKET_type *ul_cipher_ptr;
  int i;

  /* Allocate memory here */
  ul_cipher_ptr = (WCDMA_MAC_UL_CIPHER_LOG_PACKET_type *)wcdma_log_alloc(WCDMA_MAC_UL_CIPHER_LOG_PACKET,
                           MAC_UL_CIPHER_PKT_LEN(nsamples), ul_table_ptr->as_id);

  if (ul_cipher_ptr != NULL)
  {
    ul_cipher_ptr->version = 1;
    ul_cipher_ptr->nsamples = nsamples;
 
    for(i = 0; i < nsamples; i++)
    {
      ul_cipher_ptr->mac_ul_cipher_log[i].countc = ul_table_ptr->debug_info.cipher_log[i].countc;
      ul_cipher_ptr->mac_ul_cipher_log[i].cfn = ul_table_ptr->debug_info.cipher_log[i].cfn;
      ul_cipher_ptr->mac_ul_cipher_log[i].key_idx = ul_table_ptr->debug_info.cipher_log[i].key_idx;
    }
    /* Commit the buffer to log services.*/
    log_commit(ul_cipher_ptr);
  }
  else
  {
    WL2_MSG_1(LOW, ul_table_ptr, "Dropped 0x%x", WCDMA_MAC_UL_CIPHER_LOG_PACKET);
  }
}

/*===========================================================================

FUNCTION MAC_UL_SEND_CIPHER_CONFIG_LOG_PACKET

DESCRIPTION
  Logs ciphering parameters for TM channels at MAC

===========================================================================*/
void mac_ul_send_cipher_config_log_packet(mac_cipher_config_type *cipher_ptr, mac_ul_table_type *ul_table_ptr)
{
  WCDMA_MAC_UL_CIPHER_CONFIG_LOG_PACKET_type *ul_cipher_ptr;
  uint8 *log_ptr;

  if (NULL != cipher_ptr)
  {
    /* Allocate memory here */
    ul_cipher_ptr = (WCDMA_MAC_UL_CIPHER_CONFIG_LOG_PACKET_type *)wcdma_log_alloc(WCDMA_MAC_UL_CIPHER_CONFIG_LOG_PACKET,
                           MAC_UL_CIPHER_CONFIG_PKT_LEN, ul_table_ptr->as_id);
    log_ptr = (uint8*)ul_cipher_ptr;
    if (NULL != log_ptr)
    {
      log_ptr += FPOS(WCDMA_MAC_UL_CIPHER_CONFIG_LOG_PACKET_type, version);

      /* Version */
      *log_ptr++ = 1;

      /* Cipher type and cipher algo */
      *log_ptr = cipher_ptr->cipher_cfg_type & 0x0F;
      *log_ptr |= (cipher_ptr->cipher_algo << 4) & 0xF0;
      log_ptr++;
	  
      /*Key ID*/
      *log_ptr++ = cipher_ptr->key_idx;

      /* HFN */
      *log_ptr++ = cipher_ptr->hfn & 0xFF;
      *log_ptr++ = (cipher_ptr->hfn & 0xFF00) >> 8;
      *log_ptr++ = (cipher_ptr->hfn & 0xFF0000) >> 16;
      *log_ptr++ = (cipher_ptr->hfn & 0xFF000000) >> 24;

      /* Activation time */
      *log_ptr++ = cipher_ptr->ciph_act_time & 0xFF;
      *log_ptr++ = (cipher_ptr->ciph_act_time & 0xFF00) >> 8;
      *log_ptr++ = (cipher_ptr->ciph_act_time & 0xFF0000) >> 16;
      *log_ptr++ = (cipher_ptr->ciph_act_time & 0xFF000000) >> 24;

      log_commit(ul_cipher_ptr);
    }
    else
    {
      WL2_MSG_1(LOW, ul_table_ptr, "Dropped 0x%x", WCDMA_MAC_UL_CIPHER_CONFIG_LOG_PACKET);
    }
  }
  else
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "NULL cipher ptr");
  }  
}

/*===========================================================================

FUNCTION wcdma_mac_set_dbg_var_through_qxdm

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
ex. commands
send_data 75 4 46 0 1 <val>   - for mac_r99_enable_debug_f3s
send_data 75 4 46 0 2 <val>   - for mac_eul_enable_debug_f3s
send_data 75 4 46 0 3 <val>   - for mac_dl_debug_print_interval
send_data 75 4 46 0 4 <val>   - for mac_ul_debug_print_interval
send_data 75 4 46 0 5 <val>   - for mac_hs_logging
send_data 75 4 46 0 6 <val>   - for mac_ehs_logging
send_data 75 4 46 0 7 <val>   - for mac_ehs_p_to_c_logging
send_data 75 4 46 0 8 <val>   - for mac_tvm logging
send_data 75 4 46 0 9 <val>   - for MAC_EUL_EVENT_6D_SUP_THRSHLD
send_data 75 4 46 0 10 <val>  - for MAC_EUL_EVENT_6D_CONSEC_TTI
send_data 75 4 46 0 11 <val>  - for MAC_EUL_EVENT_6D_SUP_REDUCTION
send_data 75 4 46 0 12 <val>  - for disabling\enabling RAVE ar W
send_data 75 4 46 0 13 <val>  - for setting RAVE params
send_data 75 4 46 0 16 <val>  - for setting e4b prohibit count

<val> - 32 bit value of debug variable
for providing hex value append val with 0x
===========================================================================*/

void wcdma_mac_set_dbg_var_through_qxdm
(
  sys_modem_as_id_e_type as_id,
  uint8 debug_var_type,
  uint32 debug_var_val
)
{
  mac_dl_table_type *dl_table_ptr;
  mac_ul_table_type *ul_table_ptr;
  mac_hs_table_type *hs_table_ptr;
  mac_eul_table_type *eul_table_ptr;

  l2_validate_as_id(as_id);
  
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);
  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id); 
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  eul_table_ptr = GET_MAC_EUL_TABLE_PTR(as_id);
  
  WL2_MSG_2(HIGH, ul_table_ptr, "called wcdma_mac_set_dbg_var_through_qxdm, debug_var_type 0x%x, debug_var_val 0x%x",
                                             debug_var_type, debug_var_val);
  
  switch((wcdma_mac_debug_var_type)debug_var_type)
  {
    case MAC_R99_ENABLE_DEBUG_F3S:
      ul_table_ptr->debug_info.mac_r99_enable_debug_f3s = (uint8)debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_r99_enable_debug_F3s = 0x%x",
                                                 (uint8)debug_var_val);
    break;

    case MAC_EUL_ENABLE_DEBUG_F3S:
      eul_table_ptr->eul_log_info.mac_eul_enable_debug_f3s = debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_eul_enable_debug_F3s = 0x%x",
                                                 debug_var_val);
    break;
    
    case MAC_DL_DEBUG_PRINT_INTERVAL:
      dl_table_ptr->mac_dl_debug_print_interval = (uint8)debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_dl_debug_print_interval = %d",
                                                 (uint8)debug_var_val);
    break;
    
    case MAC_UL_DEBUG_PRINT_INTERVAL:
      ul_table_ptr->debug_info.mac_ul_debug_print_interval = (uint8)debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_ul_debug_print_interval = %d",
                                                 (uint8)debug_var_val);
    break;
    
    case MAC_HS_LOGGING:
      hs_table_ptr->mac_hs_logging = (mac_hs_dbg_enum_type)debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_hs_logging = %d",
                                                 (mac_hs_dbg_enum_type)debug_var_val);
    break;
    
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))

    case MAC_EHS_LOGGING:
      hs_table_ptr->mac_ehs_logging = debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_ehs_logging = %d",
                                                 debug_var_val);
    break;
    
    case MAC_EHS_P_TO_C_LOGGING:
      hs_table_ptr->mac_ehs_p_to_c_logging = debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_ehs_p_to_c_logging = %d",
                                                 debug_var_val);
    break;
#endif
    case MAC_TVM_LOGGING:
      ul_table_ptr->traffic_info.tvm_debug = (uint16)debug_var_val;
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: mac_hs_rach_f3_enable = %d",
                                                 (uint16)debug_var_val);
    break;
#if (defined FEATURE_WCDMA_EVT6D_ENH) && (defined FEATURE_WCDMA_DC_HSUPA)
    case MAC_EUL_EVENT_6D_SUP_THRSHLD:
      eul_table_ptr->mac_eul_6d_sup_thru_qxdm = (uint8)(debug_var_val & 0xFF);
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: EVT6D: mac_eul_6d_sup_tshld = %d", eul_table_ptr->mac_eul_6d_sup_thru_qxdm);
    break;
    
    case MAC_EUL_EVENT_6D_CONSEC_TTI:
      eul_table_ptr->mac_eul_6d_consec_tti = (uint8)(debug_var_val & 0xFF);
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: EVT6D: mac_eul_6d_consec_tti = %d", eul_table_ptr->mac_eul_6d_consec_tti);
    break;
   
    case MAC_EUL_EVENT_6D_SKIP_COUNT:
      eul_table_ptr->mac_eul_6d_max_skip_count = (uint8)(debug_var_val & 0xFF);
      WL2_MSG_1(HIGH, ul_table_ptr,"Through qxdm: EVT6D: mac_eul_6d_max_skip_count = %d", eul_table_ptr->mac_eul_6d_max_skip_count);
    break;
    case MAC_EUL_EVENT_6D_REINIT_COUNT:
      eul_table_ptr->mac_eul_6d_reinit_timer = (debug_var_val);
      WL2_MSG_1(HIGH, ul_table_ptr,"Through qxdm: EVT6D: mac_eul_6d_reinit_timer = %d", eul_table_ptr->mac_eul_6d_reinit_timer);
    break;
   
    case MAC_EUL_EVENT_6D_SUP_REDUCTION:
      eul_table_ptr->mac_eul_6d_sup_reductn = (uint8)(debug_var_val & 0xFF);
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: EVT6D: mac_eul_6d_sup_reductn = %d", eul_table_ptr->mac_eul_6d_sup_reductn);
    break;
#endif
#ifdef FEATURE_DATA_RAVE_SUPPORT 
    case MAC_EUL_ENABLE_RAVE_AT_W:
      eul_table_ptr->mac_eul_rave_info.enabled_at_w = debug_var_val & 0x00000001;
      WL2_MSG_1(HIGH, eul_table_ptr, "Through Qxdm: RAVE: simutlate_enable_at_W - %d",
  	                              eul_table_ptr->mac_eul_rave_info.enabled_at_w);
    break;
    case MAC_EUL_SET_RAVE_PARAMS:      
      wcdma_l2_start_uplink_rate_calculations(as_id, debug_var_val & 0x00000001, (debug_var_val & 0x0000FFF0) >> 4);
      eul_table_ptr->mac_eul_rave_info.alpha = (debug_var_val & 0x00FF0000) >> 16;
      WL2_MSG_3(HIGH, eul_table_ptr, "Through qxdm: RAVE: simulate_enabled_by_ds - %d, Interval - %d, Alpha - %d",  	                              
                               	      debug_var_val & 0x00000001,
              	                      (debug_var_val & 0x0000FFF0) >> 4,
                                      eul_table_ptr->mac_eul_rave_info.alpha);
    break;
#endif
#ifdef FEATURE_WCDMA_DS_WEB_BOOST
    case MAC_TVM_SET_E4B_PROHIBIT_CNT:
      ul_table_ptr->traffic_info.trig_tvm_params.max_prohibit_e4b = (uint16)(debug_var_val / MAC_UL_TRAFFIC_VOLUME_TMR_BASE);
      WL2_MSG_1(HIGH, ul_table_ptr, "Through qxdm: MAC_FRC_TVM: Modify prohibit_e4b_cnt to - %d", 
	                             ul_table_ptr->traffic_info.trig_tvm_params.max_prohibit_e4b);
    break;
#endif
    default:
      WL2_MSG_2(HIGH, ul_table_ptr, "Invalid debug MAC code %d, val %d",
                                                 debug_var_type,
                                                 debug_var_val);
      break;
  }
  return;
}


