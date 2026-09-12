/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file together with gl1_hw_gsm.c and gl1_hw_gprs.c contains the
   procedures for the frame layer of non portable layer 1.   The procedures
   used herein are intended for sole use by the message layer functions and
   not intended for use by the portable layer 1 software.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_lif.c#3 $
$DateTime: 2024/06/25 00:50:16 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
19/06/24   hi       CR3845535 setting zif_to_lif workaround flag based on PS metric power dbm while populating lif metrics
14/12/21   sk       CR3088880 Disable ZIF->LIF WA if power level is <-105dBm for CS call
09/05/18   rv       CR2238600 F3 reduction TA.3.0.c1
07/09/16   sp       CR1064024: NV Control to Force LIF Offset for debug
12/07/16    sp      CR1037253: Change from ZIF to LIF in packet channels only if wbee/rssi > 0.5
24/05/16    br      CR1019033 Change in WBEE(Jammer detection) threshold during  voice call
01/09/15   jk       CR899990:Enable LIF by default in FTM mode 
23/02/15   sjv      CR799313: Using new mcfg APIs replacing efs_get for Geran 
13/03/15   jk       CR807434:Disable LIF when ACI400_Detected in frame >= 0.5
23/02/15   jk       CR793098:LIF additional logging changes
13/01/14   jk       CR781025:WBEE threshold defined as 3.2e6
13/11/14   jk       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "comdef.h"


#include "geran_multi_sim.h"

#include "gtmrs.h"
#include "gtmrs_g.h"         /* GSM timer services */
#include "gl1_hw.h"
#include "gl1_hw_lif.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_hw_debug.h"
#include "l1_utils.h"
#include "fs_public.h"
#include "fs_lib.h"
#include "l1_utils.h"
#include "geran_nv.h"

#ifdef GERAN_L1_ENHANCED_RX

static gl1_hw_lif_data_t gl1_hw_lif_data[NUM_GERAN_DATA_SPACES];

static gl1_hw_lif_data_t *gl1_hw_lif_data_ptr[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE_INDEXED( &gl1_hw_lif_data ) };


/*===========================================================================

                          FUNCTIONS

===========================================================================*/


/*===========================================================================

FUNCTION gl1_hw_efs_read_lif_feature_flag

DESCRIPTION
 Retrieve the LIF feature control flag from EFS NV item 73528. Feature is enabled if NV is set to 1
 and will be disabled if NV is set to 0.
===========================================================================*/
void gl1_hw_efs_read_lif_feature_flag(gas_id_t gas_id)
{
    int efs_error;
    uint32 lif_nv_read = 0;
    efs_error = geran_efs_read_primary(GL1_EFS_DEBUG_LIF_SUPPROT,&lif_nv_read,
                        sizeof(lif_nv_read));

    MSG_GERAN_HIGH_2_G("GL1:EFS MDSP LIF Feature Flag %d %d",efs_error,lif_nv_read);
    if(efs_error == -1)
      {
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_feature_flag = 0;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_enable = 0;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_sign = 0;
      }
    else
      {
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_feature_flag = (lif_nv_read & 0x01);
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_enable = (lif_nv_read & 0x02);
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_sign = (lif_nv_read & 0x4)?TRUE:FALSE;        
      }
}

/*===========================================================================

FUNCTION gl1_hw_get_lif_feature_flag

DESCRIPTION
Returns the LIF feature flag (NV 73528).
===========================================================================*/
uint32 gl1_hw_get_lif_feature_flag(gas_id_t gas_id)
{
   uint32 is_lif_enabled = 1;
   /*CR 708412:LIF is enabled by default. If the NV73528 is set to 1 LIF will get disabled.
       *Hence returning !gl1_lif_feature_flag*/
   is_lif_enabled = (gl1_hw_lif_data_ptr[gas_id]->gl1_lif_feature_flag == 0)?1:0;
   return (is_lif_enabled);
}

/*===========================================================================

FUNCTION  gl1_hw_update_lif_metrics

DESCRIPTION
   This function updates the LIF metrics for CS/PS slots 
===========================================================================*/
void gl1_hw_update_lif_metrics(gl1_defs_metrics_rpt *metrics_ptr,gl1_defs_rx_pkt_metrics_type **metrics_info,
                                        ARFCN_T arfcn,gl1_msg_lif_chan_type channel_type,uint8 NoOfSlots,
                                        uint32 fn,gl1_hw_update_lif_metrics_t update_metrics,gas_id_t gas_id)
{
  uint8 i = 0;
  gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.arfcn = arfcn;

  switch(update_metrics)
  {
  case UPDATE_LIF_CS_METRICS:
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.updated_cs_metrics = TRUE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.fn_cs_metrics      = fn;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.NoOfSlots = NoOfSlots;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].channel_type          = channel_type;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].arfcn                 = arfcn_to_word(arfcn);
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].rssi                  = metrics_ptr->rssi;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].pwr_dBm_x16           = metrics_ptr->pwr_dBm_x16;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].aci_filter_used       = metrics_ptr->aci_filter_used;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].ACI400Detected        = metrics_ptr->aci400Detected;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].NoOfACI400_frame      = 0;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].freq_offset           = metrics_ptr->freq_offset;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].timing_offset         = metrics_ptr->timing_offset;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].snr                   = metrics_ptr->snr;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].gain_state            = (byte)metrics_ptr->rx_gain_range;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Prx              = metrics_ptr->WBEE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Drx              = metrics_ptr->WBEE_DRx;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Avg              = metrics_ptr->WBEE;
	 if((metrics_ptr->pwr_dBm_x16 > (-100*16))&& (channel_type!= GL1_MSG_CS_BCCH) && (channel_type!= GL1_MSG_CS_CCCH))
	 {   
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Threshold = GL1_LIF_WBEE_THRESHOLD2;	
	 }
	 else
	 {
	 	gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Threshold = GL1_LIF_WBEE_THRESHOLD1;
	 }
	 	
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].lif_freq_offset_frame = 0;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].nextLifFreq           = metrics_ptr->lif_offset_freq;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].curLifFreq            = metrics_ptr->curLifFreqKhz;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].jammer_detected       = FALSE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].is_lif_on             = FALSE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].accISqr               = metrics_ptr->accISqr;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].accQSqr               = metrics_ptr->accQSqr;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].accIQ                 = metrics_ptr->accIQ;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].wbdcEstI_PRx          = metrics_ptr->wbdcEstI_PRx;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].wbdcEstQ_PRx          = metrics_ptr->wbdcEstQ_PRx;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].wbdcEstI_DRx          = metrics_ptr->wbdcEstI_DRx;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].wbdcEstQ_DRx          = metrics_ptr->wbdcEstQ_DRx;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].rsbImgAciMetric       = metrics_ptr->rsbImgAciMetric;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].jdet_value            = metrics_ptr->jdet_value;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].SawlessHLLLState      = metrics_ptr->SawlessHLLLState;
     break;

  case UPDATE_LIF_PS_METRICS:
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.NoOfSlots = 0;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.updated_ps_metrics               = FALSE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.fn_ps_metrics                    = fn;
     for(i= 0;(i<NoOfSlots) && (NoOfSlots <= GL1_DEFS_MAX_DL_MSGS);i++)
     {
       if((metrics_info[i]->schedule_status & GL1_DEFS_RX_SCHEDULED) && (metrics_info[i]->valid))
       {
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.NoOfSlots++;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].channel_type          = channel_type;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].arfcn                 = arfcn_to_word(arfcn);
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].rssi                  = metrics_info[i]->rssi;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].pwr_dBm_x16           = metrics_info[i]->pwr_dBm_x16;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].aci_filter_used       = metrics_info[i]->aci_filter_used;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].ACI400Detected        = metrics_info[i]->aci400Detected;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].NoOfACI400_frame      = 0;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].freq_offset           = metrics_info[i]->freq_offset;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].timing_offset         = metrics_info[i]->timing_offset;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].snr                   = metrics_info[i]->snr;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].gain_state            = (byte)metrics_info[i]->gain_range;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].WBEE_Prx              = metrics_info[i]->WBEE;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].WBEE_Drx              = metrics_info[i]->WBEE_DRx;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].WBEE_Avg              = metrics_info[i]->WBEE;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].WBEE_Threshold = GL1_LIF_WBEE_THRESHOLD1;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].lif_freq_offset_frame = 0;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].nextLifFreq           = metrics_info[i]->lif_offset_freq;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].curLifFreq            = metrics_info[i]->curLifFreqKhz;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].jammer_detected       = FALSE;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].is_lif_on             = FALSE;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].accISqr               = metrics_info[i]->accISqr;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].accQSqr               = metrics_info[i]->accQSqr;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].accIQ                 = metrics_info[i]->accIQ;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].wbdcEstI_PRx          = metrics_info[i]->wbdcEstI_PRx;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].wbdcEstQ_PRx          = metrics_info[i]->wbdcEstQ_PRx;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].wbdcEstI_DRx          = metrics_info[i]->wbdcEstI_DRx;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].wbdcEstQ_DRx          = metrics_info[i]->wbdcEstQ_DRx;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].rsbImgAciMetric       = metrics_info[i]->rsbImgAciMetric;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].jdet_value            = metrics_info[i]->jdet_value;
        gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[i].SawlessHLLLState      = metrics_info[i]->SawlessHLLLState;
        }
       }
     if(gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.NoOfSlots)
     {
       gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.updated_ps_metrics = TRUE;
       gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.fn_ps_metrics      = fn;
     }
     break;

  default:
     MSG_GERAN_HIGH_0_G("LIF metrics were not updated");
     break;
  }
}

/*===========================================================================

FUNCTION  gl1_hw_populate_lif_metrics_dtm

DESCRIPTION
   This function populates the LIF metrics for CS+PS slots in the LIF table
===========================================================================*/
void gl1_hw_populate_lif_metrics(uint32 fn,gas_id_t gas_id)
{

  uint8  i = 0;
  uint8  j = 0;
  uint8  k = 0;
  uint32  index     = 0;
  uint8   NoOfSlots = 0;
  uint32  wbee_avg  = 0;
  uint32 rssi_avg = 0;
  boolean  enable_lif           = FALSE;
  boolean  lif_freq_offset_sign = FALSE;
  int32    lif_offset_freq_avg  = 0;
  uint32 aci400_frame = 0,nonaci400_frame = 0;
  gl1_hw_lif_table_t curr_lif_table;
  boolean zif_to_lif_workaround = FALSE;
  uint32 threshold = 0;

  gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.NoOfSlots = 0;

  if((gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.updated_cs_metrics)
      &&
     (fn == gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.fn_cs_metrics))
  {
   gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.NoOfSlots += gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.NoOfSlots;
   
   for(i=0;i<gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.NoOfSlots;i++)
     {
        gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i] = gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[i];
     }
      
    threshold = gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].WBEE_Threshold;
    
    if ( ( gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_cs_metrics.gl1_log_lif_params[0].pwr_dBm_x16/16 ) <= -102 )
    {
      zif_to_lif_workaround = FALSE;
    }
    else
    {
      zif_to_lif_workaround = TRUE;
    }
  }

  if((gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.updated_ps_metrics)
      &&
     (fn == gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.fn_ps_metrics))
   {
   gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.NoOfSlots += gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.NoOfSlots;
   
   for(j=i,k=0;k<gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.NoOfSlots;j++,k++)
     {
        gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[j] = gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[k];
     }
     threshold = gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[0].WBEE_Threshold;
      
      if ( ( gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.gl1_log_lif_ps_metrics.gl1_log_lif_params[0].pwr_dBm_x16/16 ) <= -102 )
      {
        zif_to_lif_workaround = FALSE;
      }
      else
      {
        zif_to_lif_workaround = TRUE;
      }
   }

   NoOfSlots =  gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.NoOfSlots;
   
   for(i= 0;(i<NoOfSlots) && (NoOfSlots <= MAX_NUMBER_OF_RX_SLOTS);i++)
   {
     wbee_avg            += gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].WBEE_Prx;
       aci400_frame += (gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].ACI400Detected == TRUE)?1:0;
     lif_offset_freq_avg += gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].nextLifFreq;
       rssi_avg += gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].rssi; //raw_rssi
   }

   if(gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.NoOfSlots)
   {
    wbee_avg            = wbee_avg/NoOfSlots;
      nonaci400_frame = NoOfSlots - aci400_frame;
    lif_offset_freq_avg = lif_offset_freq_avg/NoOfSlots;
      rssi_avg = rssi_avg/NoOfSlots;

      index = gl1_hw_get_arfcn_index(gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.arfcn,gas_id);

      gl1_hw_get_lif_status(&curr_lif_table, index,gas_id);

      if (zif_to_lif_workaround == FALSE)
      {
         if (aci400_frame >= nonaci400_frame || (wbee_avg > threshold))
         {
            enable_lif = FALSE;
         }
         else
    {
      enable_lif = TRUE;
    }
      }

      if (zif_to_lif_workaround == TRUE)
      {
         if (aci400_frame >= nonaci400_frame)
         {
            enable_lif = FALSE;
         }         
         else if (curr_lif_table.is_lif_enabled && (wbee_avg > threshold))
    {
      enable_lif = FALSE;
    }
         else if (curr_lif_table.is_lif_enabled == FALSE)
         {
            if(wbee_avg <  (rssi_avg >>1))
    {
      enable_lif = TRUE;
    }
    else
    {
			  enable_lif = FALSE;
			}
            MSG_GERAN_MED_3_G("wbee_avg:%d, rssi_avg:%d, enable_lif:%d", wbee_avg, rssi_avg, enable_lif);
         }
         else
         {
      enable_lif = TRUE;
    }
      }

    lif_freq_offset_sign = (lif_offset_freq_avg >= 0)?TRUE:FALSE;

   if(enable_lif)
   {
      MSG_GERAN_HIGH_5_G("gl1_hw_lif_data_store wbee_avg %d, rssi_avg:%d, threshold %d, curr_lif:%d, voice=%d",
                                        wbee_avg,rssi_avg,
                                        threshold, 
                                        curr_lif_table.is_lif_enabled,
                                        zif_to_lif_workaround
                                        );	   
   }

  /*Force NV Settings*/
   if(gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_enable)
   {
     MSG_GERAN_HIGH_2_G("Force LIF Sign %d, Actual LIF polarity:%d", gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_sign, lif_freq_offset_sign); 
     enable_lif = TRUE;
     lif_freq_offset_sign = gl1_hw_lif_data_ptr[gas_id]->gl1_lif_force_sign;
     
   }

   index = gl1_hw_get_arfcn_index( gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics.arfcn,
                                   gas_id
                                 );

  gl1_hw_update_lif_table( index, enable_lif, lif_freq_offset_sign, gas_id );

   for(i=0;(i<NoOfSlots) && ((NoOfSlots <= MAX_NUMBER_OF_RX_SLOTS));i++)
   {
         gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].NoOfACI400_frame = aci400_frame;
     gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].WBEE_Avg              = wbee_avg;
     gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].lif_freq_offset_frame = (lif_freq_offset_sign == TRUE)?GL1_LIF_FREQ_OFFSET:-(GL1_LIF_FREQ_OFFSET);
     gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].jammer_detected       = (enable_lif == TRUE)?FALSE:TRUE;
     gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics.gl1_log_lif_params[i].is_lif_on             = enable_lif;
   }

     gl1_hw_log_lif_metrics( &(gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics),gas_id);
   }
  
   /*Clear LIF metrics in DTM mode after updating the LIF table*/
   gl1_hw_reset_lif_dtm_metrics(gas_id);
}


/*===========================================================================

FUNCTION  gl1_hw_get_arfcn_index

DESCRIPTION
   This function returns the index to the LIF table where the LIF decision for a particular ARFCN is stored
===========================================================================*/
uint32 gl1_hw_get_arfcn_index(ARFCN_T arfcn, gas_id_t gas_id)
{
  uint32 index = INVALID_INDEX;
  sys_band_T band = arfcn.band;
  uint16 arfcn_num = arfcn.num;
  
  switch(band)
  {
  case SYS_BAND_EGSM_900:
  case SYS_BAND_PGSM_900:
    if((arfcn_num <= 124))
     {
     index = arfcn_num;
     }
    else if((arfcn_num >= 975) && (arfcn_num <= 1023))
     {
     index = 125 + (arfcn_num-975);
     }
    break;
  case SYS_BAND_CELL_850:
    if((arfcn_num >= 128) && (arfcn_num <= 251))
     {
     index = 174 + (arfcn_num-128);
     }
    break;

  case SYS_BAND_DCS_1800:
    if((arfcn_num >= 512) && (arfcn_num <= 885))
     {
     index = 298 + (arfcn_num-512);
     }
    break;

  case SYS_BAND_PCS_1900:
    if((arfcn_num >= 512) && (arfcn_num <= 810))
     {
     index = 672 + (arfcn_num-512);
     }
    break;

  default:
    MSG_GERAN_ERROR_1_G("Invalid ARFCN:%d",arfcn_to_word(arfcn));
    break;
  }

  return index;
  
}

/*===========================================================================

FUNCTION  gl1_hw_update_lif_table

DESCRIPTION
   This function updates the LIF table (based on index) with the LIF status for a particular ARFCN
===========================================================================*/
void gl1_hw_update_lif_table(uint32 index, boolean enable_lif, boolean lif_freq_offset_sign,gas_id_t gas_id)
{
  if(index != INVALID_INDEX)
   {
   gl1_hw_lif_data_ptr[gas_id]->gl1_hw_lif_table[index].is_lif_enabled = enable_lif;
   gl1_hw_lif_data_ptr[gas_id]->gl1_hw_lif_table[index].lif_freq_offset_sign = lif_freq_offset_sign;
   }
  else
   {
   MSG_GERAN_ERROR_1_G("Invalid index %d used to retrieve LIF table",index);
   }
}

/*===========================================================================

FUNCTION  gl1_hw_get_lif_status

DESCRIPTION
   This function returns  the LIF status stored in LIF table (based on index) for a particular ARFCN
===========================================================================*/
void gl1_hw_get_lif_status(gl1_hw_lif_table_t *gl1_hw_lif_table,uint32 index,gas_id_t gas_id)
{
  if(index != INVALID_INDEX)
   {
   gl1_hw_lif_table->is_lif_enabled = gl1_hw_lif_data_ptr[gas_id]->gl1_hw_lif_table[index].is_lif_enabled;
   gl1_hw_lif_table->lif_freq_offset_sign = gl1_hw_lif_data_ptr[gas_id]->gl1_hw_lif_table[index].lif_freq_offset_sign;
   }  
  else
   {
   MSG_GERAN_ERROR_1_G("Invalid index %d used to retrieve LIF table",index);
   }
}


/*===========================================================================

FUNCTION  gl1_hw_reset_lif_dtm_metrics

DESCRIPTION
   This function resets the structure which holds LIF metrics in DTM mode
===========================================================================*/
void gl1_hw_reset_lif_dtm_metrics(gas_id_t gas_id)
{
  memset(&gl1_hw_lif_data_ptr[gas_id]->gl1_lif_dtm_metrics,0,sizeof(gl1_lif_dtm_metrics_t));
  MSG_GERAN_LOW_1_G("LIF metrics in DTM mode are cleared in FN:%d",
                     GSTMR_GET_FN_GERAN(gas_id));
}


/*===========================================================================

FUNCTION  gl1_hw_reset_lif_table

DESCRIPTION
   This function resets the ARFCN based LIF table
===========================================================================*/
void gl1_hw_reset_lif_table(gas_id_t gas_id)
{
 memset(&gl1_hw_lif_data_ptr[gas_id]->gl1_hw_lif_table, 0, (RR_MAX_PSCAN_FREQ_SCAN*sizeof(gl1_hw_lif_table_t)));
}

/*===========================================================================

FUNCTION  gl1_hw_lif_data_init

DESCRIPTION
   This function resets the relevant parameters in LIF data structure gl1_hw_lif_data
===========================================================================*/
void gl1_hw_lif_data_init(gas_id_t gas_id)
{
   gl1_hw_lif_data_ptr[gas_id]->gl1_enable_lif_in_ftm_cal = FALSE;
   memset(&gl1_hw_lif_data_ptr[gas_id]->gl1_log_lif_metrics, 0, (sizeof(gl1_log_lif_metrics_t)));
   gl1_hw_reset_lif_table(gas_id);
   gl1_hw_reset_lif_dtm_metrics(gas_id);
   MSG_GERAN_MED_0_G("gl1_hw_lif_data_store initialized");
   }

/*===========================================================================

FUNCTION  gl1_hw_enable_lif_in_ftm_cal

DESCRIPTION
   This function stores gl1_enable_lif_in_ftm_cal, if LIF needs to be enabled by default for all TCH and PDTCH bursts in 
   FTM cal mode. In non-signalling mode LIF is enabled simialr to the normal or online mode 
===========================================================================*/

void gl1_hw_enable_lif_in_ftm_cal(boolean enable_lif, gas_id_t gas_id)
   {
  gl1_hw_lif_data_ptr[gas_id]->gl1_enable_lif_in_ftm_cal = enable_lif;
  MSG_GERAN_HIGH_1_G("gl1_enable_lif_in_ftm_cal:%d",gl1_hw_lif_data_ptr[gas_id]->gl1_enable_lif_in_ftm_cal);

   }

/*===========================================================================

FUNCTION  gl1_hw_get_enable_lif_in_ftm_cal

DESCRIPTION
   This function returns gl1_enable_lif_in_ftm_cal, if LIF needs to be enabled by default for all TCH and PDTCH bursts in 
   FTM cal mode. In non-signalling mode LIF is enabled simialr to the normal or online mode 
===========================================================================*/

boolean gl1_hw_get_enable_lif_in_ftm_cal(gas_id_t gas_id)
{
  return(gl1_hw_lif_data_ptr[gas_id]->gl1_enable_lif_in_ftm_cal);
}

#endif /*GERAN_L1_ENHANCED_RX*/

/*EOF*/



