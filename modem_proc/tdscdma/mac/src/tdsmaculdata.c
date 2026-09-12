/*============================================================================================

                                U P L I N K   D A T A   H A N D L E R


GENERAL DESCRIPTION

  This module contains the functions needed to process the UL MAC data

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*===*===*===*===*===*===*/

/*==============================================================================================


                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculdata.c_v   1.29   10 Jul 2002 10:05:20   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculdata.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, whesre, why

--------  ---     -----------------------------------------------------------------------------
03/30/15  sarao    FR26213 for QSH support 
04/25/12  mr      Fixed Compiler warning
10/24/10  guo     Initial release Upmerged WCDMA to TDS

==============================================================================================*/


/*==============================================================================================
                     INCLUDE FILES
==============================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "dsm.h"
#include  "tdsmacrrcif.h"
#include  "tdsmacrlcif.h"
#include  "tdsl1sapcommon.h"
#include  "tdsl1macdata.h"
#include  "tdsmacinternal.h"
#include  "tdsl2ultask.h"
#include  "tdsmacultfcs.h"
#include  "tdsmacul.h"
#ifdef FEATURE_TDSCDMA_SILENT_FRAME
  #include  "tdsseq.h"
#endif
#include  "err.h"
#include "tdsl1macif.h"
#include "tdsmacul.h"
#include  <string.h>
#ifdef FEATURE_DUMMY_SIGNALING
  #error code not present
#endif
#include "tdsmactraffic.h"

#include "tdsmathutil.h"
#include "tdsmaculdata.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsmac_qsh_ext.h"
#endif
#include "tdsrrcdata.h"


#ifdef FEATURE_QSH_EVENT_METRIC
extern tdsmac_qsh_metric_r99_stat_s tdsmac_qsh_r99_metrics;
#endif /*FEATURE_QSH_EVENT_METRIC*/

/* Uplink RLC buffer status buffer */
tdsrlc_ul_buf_status_type    tdsul_buf_status;

/* UL RLC Frame Format information */
tdsrlc_ul_frame_format_type  tdsul_rlc_frame_fmt;

tdsrlc_ul_frame_data_type    *tdsrlc_ul_data_info_ptr;

tdsrlc_ul_frame_data_type    *tdsrlc_ul_data_ptr;

boolean tdsmac_rach_rxd_enable = FALSE;

/* Current TTI */
extern  tdsl1_tti_enum_type    tdscurrent_tti;


#ifdef  FEATURE_TDSCDMA_SILENT_FRAME 
tds_vfr_cb_type tds_vfr_cb_ptr; /*This is a local copy of cb_ptr for saving MVS CB ptr during registration*/
/* Saved copied of Client_data provide by MVS */
void * client_data_l;
tds_vfr_info_type tds_vfr_info_l;
#endif  /*FEATURE_TDSCDMA_SILENT_FRAME */



/*---------------------------------------------------------------------------
  Contains the list of logical channels for which MAC needs the buffer status
---------------------------------------------------------------------------*/
tdsrlc_ul_lc_info_type              tdsmac_ul_lc_info;


/* Stores the information about which of the signalling channel, there was
data built in the last TTI. This information will later be used at the
time when a transport block is trashed at L1 */
tdsrlc_status_report_req_type  tdsrlc_status_report_req_info;

boolean tds_rlc_buffer_call_from_ISR = FALSE;

#if 0   /////invalid ,for l1 not provide wl1_rxd_fach_signalling_data_available, 2012.4.11
/*===============================================================================================
 FUNCTION NAME  : tdsmac_rach_srb_rxd_manage

 DESCRIPTION    :  * If there is any data on RACH in UL, Call the RxD FACH function 
                   * which enables the RxD (if not yet enabled) and manages FMO 
===============================================================================================*/
void tdsmac_rach_srb_rxd_manage(boolean tdsdata_present_on_srb)
{
  
  if (TDSGET_UL_CCTRCH_TYPE == TDSRACH)
  {
    if (tdsdata_present_on_srb == FALSE)
    {
      if (tdsmac_rach_rxd_enable == TRUE)
      {
        tdsmac_rach_rxd_enable = FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC_RACH: Call RxD FACH Disable as RACH-SRB data NOT available");
        wl1_rxd_fach_signalling_data_available(FALSE);
      }
    }
    else
    {
      /* If there is any data on RACH in UL, Call the RxD FACH function 
       * which enables the RxD (if not yet enabled) and manages FMO */
      if (tdsmac_rach_rxd_enable == FALSE)
      {
        tdsmac_rach_rxd_enable = TRUE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC_RACH: Call RxD FACH Enable as RACH-SRB data available");
        wl1_rxd_fach_signalling_data_available(TRUE);
      }
    }
  }
}
#endif


/*===============================================================================================
 FUNCTION NAME  tdsattach_mac_parameters

 DESCRIPTION  Attaches MAC parameters to each UL RLC PDU
================================================================================================*/
void  tdsattach_mac_parameters
(
/* RLC Frame Data Pointer */
tdsrlc_ul_frame_data_type  *rlc_frame_data_ptr,

/* Shared Transmit Data Buffer Pointer */
tdsl1_ul_tx_data_type    *txbufptr
)
{
  uint8                     i, j;
  tdsrlc_ul_logchan_data_type  *lc_data_ptr;
  tdsl1_ul_trch_data_type      *l1_trch_data_ptr;
  tdsl1_ul_tb_data_type        *tbptr;
  tdsul_lc_info_type           *lc_info_ptr;
  boolean                   trch_select_flag[TDSUE_MAX_TRCH];
  tdsmac_ul_tfc_type           *oc_tfc_ptr;
  tdsmac_ul_tf_type            *oc_tf_ptr;
  tdsul_trch_info_type         *int_tc_ptr;

  uint8                     trch_idx;

  uint8                     index;

  uint16                    n_tm_pdus=0;
  uint16                    last_tm_idx=0;


  /* Selected Transport Format Combination Pointer */
  oc_tfc_ptr = &tdsoc_ul_tfcs_ptr->tfc_info[TDSGET_SELECTED_TFCI];
#ifdef FEATURE_TDSCDMA_DSDA
  txbufptr->srb_data_num_tti = 0;
  txbufptr->srb_data_present = FALSE;
#endif

  /* Clear Transport Channel Select Flags */
  for (i=0; i <TDSUE_MAX_TRCH; i++)
    trch_select_flag[i] = FALSE;

  /* Initialize this sturtcure. Thsi will be populated below
  as we go through each logical channel */
  tdsrlc_status_report_req_info.nchan = 0;

  /* Number of uplink RLC logical channels */
  for (i=0; i < rlc_frame_data_ptr->nchan; i++)
  {

    /* Pointer to logical channel PDU information */
    lc_data_ptr = &rlc_frame_data_ptr->chan_info[i];

    n_tm_pdus = 0;
    /* Valid RLC ID */
    if (!TDSUL_RLC_ID_IS_INVALID(lc_data_ptr->rlc_id))
    {

      /* Logical channel information pointer */
      lc_info_ptr = TDSGET_UL_LC_INFO_PTR(lc_data_ptr->rlc_id);

      /* get pointer to internal transport channel info */
      int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(lc_info_ptr->trch_idx);

      /* Pointer to Transport Channel info */
      l1_trch_data_ptr = &txbufptr->tx_data[lc_info_ptr->trch_idx];

      /* TB Size */
      l1_trch_data_ptr->tb_size = (uint16)(lc_info_ptr->mac_hdr_size + lc_data_ptr->pdu_size);


      /* If there was any data built from this channel */
      if (lc_data_ptr->npdus > 0)
      {
        /* Only for the signalling channels we want the status report
        to be sent */
        if (lc_info_ptr->lc_type == UE_LOGCHAN_DCCH)
        {
          if(tdsrlc_status_report_req_info.nchan < TDSUE_MAX_UL_LOGICAL_CHANNEL)
          {
            /* Increment the number of channels and udate the channel id number */
            tdsrlc_status_report_req_info.rlc_id_list[tdsrlc_status_report_req_info.nchan++] =
              lc_data_ptr->rlc_id;
          }
          else
          {
            TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in tdsrlc_status_report_req_info.rlc_id_list[] exceed TDSUE_MAX_UL_LOGICAL_CHANNEL");
          }
        } // DCCH
      } // npdus > 0


      for (j=0 ;j <lc_data_ptr->npdus; j++)
      {

        /* TB data pointer */
        tbptr = &l1_trch_data_ptr->trch_data[l1_trch_data_ptr->num_tb+j];

        /* Populate fields of transport block info */
        tbptr->data_ptr = (void *)lc_data_ptr->pdu_info[j].pdu_ptr;
        //memcpy (tbptr->mac_hdr, lc_info_ptr->mac_hdr_value,TDSMAC_UL_MAX_HDR_SIZE);
        memscpy (tbptr->mac_hdr, TDS_L1_UL_MAC_MAX_HDR_SIZE,lc_info_ptr->mac_hdr_value,TDSMAC_UL_MAX_HDR_SIZE);				
        tbptr->mac_hdr_size = lc_info_ptr->mac_hdr_size;

        if (lc_info_ptr->enable)
        {
          /* In 1x0 case, ciphering is not done */
          if (l1_trch_data_ptr->tb_size == 0)
          {
            tbptr->ciphering_on = FALSE;
          }
          else if (TDSGET_UL_CCTRCH_TYPE != TDSINVALID_UL_CCTRCH )
          {
            tbptr->ciphering_on = FALSE;

            /*---------------------------------------------------------------------------
            Fill in the ciphering parameters if the ciphering is enabled. For TM mode
            radio bearers ciphering is performed in MAC. For UM, AM mode RB's ciphering
            is done in RLC.
            ---------------------------------------------------------------------------*/
            switch (lc_info_ptr->rlc_mode)
            {
            case UE_MODE_TRANSPARENT:
              /* Get the index */
              index = int_tc_ptr->cs_or_ps_domain;

              /*--------------------------------------------------------------------------
              Copy the ciphering parameters related to TM mode. If ciphering is enabled, check
              if the algorithm to be used is UE_1 or UE_2, if yes, then perform ciphering.
              ---------------------------------------------------------------------------*/
              TDSL2_ACQ_CIPHER_PARAM_LOCK();
              if (tdscur_ul_cipher_info[index].cipher_enable &&
              (tdscur_ul_cipher_info[i].cipher_cfg_type != TDSSMC_INITIATED_NON_HO_CIPHER_CFG) &&
              ((tdscur_ul_cipher_info[index].cipher_algo == UE_UMTS_CIPHER_ALGO_UEA1)
              || (tdscur_ul_cipher_info[index].cipher_algo == UE_UMTS_CIPHER_ALGO_UEA2)
              ))
              {
                if (lc_info_ptr->rb_id != 0)
                {
                  tbptr->ciphering_on = TRUE;
                  tbptr->cipher_algo = (uint32)tdscur_ul_cipher_info[index].cipher_algo;
                  tbptr->ciphering_key_id = tdscur_ul_cipher_info[index].key_idx;
                  tbptr->count = tdscount_c[index];
                  tbptr->radio_bearer_id = (uint8)lc_info_ptr->rb_id;
                  tbptr->offset = 0;
                }
              }
              TDSL2_ACQ_CIPHER_PARAM_UNLOCK();

              /* TM ciphering state */
              tbptr->tm_cipher_state = ((n_tm_pdus==0)?TDSL1_TB_TM_CIPHER_START:TDSL1_TB_TM_CIPHER_CONT);
              n_tm_pdus++;
              last_tm_idx=j;

              break;
            case UE_MODE_ACKNOWLEDGED:
            case UE_MODE_ACKNOWLEDGED_DATA:
            case UE_MODE_UNACKNOWLEDGED:
              if (lc_data_ptr->cipher_flag)
              {
                if (lc_data_ptr->pdu_info[j].pdu_cipher_enable)
                {
                  tbptr->offset = lc_data_ptr->cipher_offset;
                  tbptr->radio_bearer_id = (uint8)lc_data_ptr->rb_id;
                  tbptr->ciphering_key_id = (uint8)lc_data_ptr->pdu_info[j].cipher_key_id;
                  tbptr->cipher_algo = lc_data_ptr->pdu_info[j].cipher_algo;
                  tbptr->count = lc_data_ptr->pdu_info[j].count_c;
                  tbptr->ciphering_on=TRUE;
                }

              }

              /* TM ciphering state */
              tbptr->tm_cipher_state = TDSL1_TB_TM_CIPHER_NOT_APPLICABLE;

              break;
            default:
              break;
            } /* switch rlc mode */
          } /* Trch Type is RACH or DCH */

        }// logical channel is enabled

        /* Set transport channel select flag */
        trch_select_flag[lc_info_ptr->trch_idx] = TRUE;
      }

      /* Get the last TB data pointer and update its cipherign state information */
      tbptr = &l1_trch_data_ptr->trch_data[l1_trch_data_ptr->num_tb+last_tm_idx];

      if (n_tm_pdus == 1)
      {
        tbptr->tm_cipher_state = TDSL1_TB_TM_CIPHER_SINGLE;
      }
      else if (n_tm_pdus > 1)
      {
        tbptr->tm_cipher_state = TDSL1_TB_TM_CIPHER_STOP;

      }

      /* Update Transport Channel TB count value */
      l1_trch_data_ptr->num_tb = (uint8)(l1_trch_data_ptr->num_tb + lc_data_ptr->npdus);

      trch_idx = lc_info_ptr->trch_idx;

      /* Transport Format Pointer */
      oc_tf_ptr=
      &tdsoc_ul_tfcs_ptr->tfs_info[trch_idx].tf_info[oc_tfc_ptr->tfi[trch_idx]];
#ifdef FEATURE_TDSCDMA_DSDA
      if ((UE_LOGCHAN_CCCH == lc_info_ptr->lc_type) ||
          (UE_LOGCHAN_DCCH == lc_info_ptr->lc_type))
      {
        txbufptr->srb_data_present = TRUE;
        txbufptr->srb_data_num_tti = tdsoc_ul_tfcs_ptr->tfs_info[trch_idx].tti_info;
      }
#endif
      if (l1_trch_data_ptr->num_tb != oc_tf_ptr->ntblks)
      {
        TDSMAC_MSG3(MSG_LEGACY_ERROR,"Mismatch in num_tb %d for LC %d, TFCI %d",
        l1_trch_data_ptr->num_tb, lc_data_ptr->rlc_id, TDSGET_SELECTED_TFCI);
      }
    } /* Valid RLC ID */

    /* Invalid RLC ID */
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Invalid RLC ID in UL RLC Frame Data");
    }
  }
  /* Determine the number of active transport channels */
  /* Clear the TB count and TB Size values of inactive channels */
  for (i=0; i <TDSL1_UL_MAX_TRCH; i++)
  {
    /* Transport Format Pointer */
    oc_tf_ptr= &tdsoc_ul_tfcs_ptr->tfs_info[i].tf_info[oc_tfc_ptr->tfi[i]];

    /* Transport channel has data */
    if (trch_select_flag[i] == TRUE)
    {
      /* Update active transport channel count */
      txbufptr->num_trch++;


      int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(i);
      l1_trch_data_ptr = &txbufptr->tx_data[i];

      if ((l1_trch_data_ptr->num_tb > TDSUE_MAX_UL_RLC_PDUS) ||
          (l1_trch_data_ptr->tb_size > TDSL1_UL_MAX_TB_SIZE))
      {
        txbufptr->num_trch = 0;
        ERR_FATAL("Mismatch in num_tb %d tb_size %d, TFCI %d",
        l1_trch_data_ptr->num_tb,l1_trch_data_ptr->tb_size,TDSGET_SELECTED_TFCI);
      }
    }
    /* Check if the TF has non-zero number of TBs and TB size of 0 */
    else if ((oc_tf_ptr->ntblks >0) && (oc_tf_ptr->tb_size ==0) &&
    (tdscurrent_tti >= tdsoc_ul_tfcs_ptr->tfs_info[i].tti_info))
    {
      txbufptr->num_trch++;
      txbufptr->tx_data[i].num_tb =(uint8) oc_tf_ptr->ntblks;
      txbufptr->tx_data[i].tb_size = 0;

      /* Set DSM pointers to NULL */
      for (j=0; j < oc_tf_ptr->ntblks; j++)
      {
        /* Set the ciphering flag to FALSE for 1x0 tf's */
        txbufptr->tx_data[i].trch_data[j].ciphering_on = FALSE;
        txbufptr->tx_data[i].trch_data[j].data_ptr = NULL;
        txbufptr->tx_data[i].trch_data[j].mac_hdr_size = 0;
      }
    }
    /* Transport channel has no data */
    else
    {
      txbufptr->tx_data[i].num_tb = 0;
      txbufptr->tx_data[i].tb_size = 0;
    }
  }
}

/*===============================================================================================
 FUNCTION NAME  tdsmac_compute_pdus_from_rlc_status

 DESCRIPTION  Compute the nbytes and npdus needed per LC based on RLC BO calculations
================================================================================================*/
void tdsmac_compute_pdus_from_rlc_status(uint8  nlchan, rlc_lc_id_type   *rlc_id_ptr, 
tdsrlc_ul_buf_status_type    *ul_buf_status_ptr,
                                      boolean *data_present_on_srb_ptr)
{

  uint32                      i, divisor, npdus, total_pdus;
  rlc_lc_id_type              rlc_id;
  tdsul_lc_info_type            *int_lc_ptr;
  tdsrlc_ul_lc_buf_status_type  *buf_status_ptr;
  
  npdus = 0;
  total_pdus = 0;

  if(TDSUE_MAX_UL_LOGICAL_CHANNEL < nlchan)
  {
	TDSMAC_MSG1(MSG_LEGACY_ERROR,"number of channel %d exceed TDSUE_MAX_UL_LOGICAL_CHANNEL",nlchan);
	return;
  }
  
  /* Make a copy of the buffer status so that this can be updated
  after ETFCI selection and can be used for SI reporting */
  for (i=0;i< nlchan;i++)
  {
    rlc_id = rlc_id_ptr[i];
    int_lc_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);
    buf_status_ptr = &(ul_buf_status_ptr->chan_info[rlc_id]);
  
    if (buf_status_ptr->type == TDSFIXED_PDUS)
    {
      if ((int_lc_ptr->lc_type == UE_LOGCHAN_CCCH) && (buf_status_ptr->npdus > 0))
      {
        *data_present_on_srb_ptr = TRUE;
      }
     continue;
    }
  
    if (buf_status_ptr->suspend_pdu_build_f)
    {
      buf_status_ptr->nbytes = 0;
    }
    else
    {
      if (int_lc_ptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
      {
        // effect_win_size, retx_bytes, ctrl_bytes matter only for am
        buf_status_ptr->nbytes = buf_status_ptr->new_data_bytes;
        
        // Get the divisor value
        divisor = (int_lc_ptr->pdu_size - int_lc_ptr->rlc_hdr_length);
        total_pdus = tdsmath_ceil(buf_status_ptr->new_data_bytes, divisor);
      }
      else
      {
         // First set nbytes to retx bytes
         buf_status_ptr->nbytes = buf_status_ptr->retx_data_bytes;
  
         // Get the divisor value
         divisor = (int_lc_ptr->pdu_size - int_lc_ptr->rlc_hdr_length);
  
         total_pdus = tdsmath_ceil(buf_status_ptr->retx_data_bytes, divisor);
         
         if (buf_status_ptr->ctrl_data_bytes && divisor)
         {
           /* Round off the ctrl bytes to number of PDUS.
            * ctrl_data_bytes includes rlc_hdr_lenght also, so 
            * npdus = ceil(ctrl_data_bytes,pdu_size).
            */
           npdus = tdsmath_ceil((buf_status_ptr->ctrl_data_bytes << 3), int_lc_ptr->pdu_size);
  
           // Add to the number of bytes the value of ctrl bytes
           buf_status_ptr->nbytes += ((npdus * divisor) >> 3);
           
           total_pdus += npdus;
         }
  
         if (buf_status_ptr->new_data_bytes && divisor)
         {
           // Get the number of PDUs that can be formed with
           npdus = tdsmath_ceil((buf_status_ptr->new_data_bytes << 3),divisor);
  
           npdus = MIN(npdus, buf_status_ptr->effect_win_size);
  
           // Add to the number of bytes the value of new bytes
           buf_status_ptr->nbytes += ((npdus * divisor) >> 3);
         }
      }
      
      if (buf_status_ptr->nbytes)
      {
        if (int_lc_ptr->lc_type == UE_LOGCHAN_DCCH)
        {
          *data_present_on_srb_ptr = TRUE;
        }
        MSG_9(MSG_SSID_TDSCDMA_MAC,MSG_LEGACY_HIGH,
        "DBG_BUF_STATUS: Rlc_id %d, new_data %d bytes, retx_data %d bytes,ctrl_data %d bytes, pdu_size %d, hdr_len %d, win_size %d, nbytes %d, npdus %d",
        rlc_id, buf_status_ptr->new_data_bytes, buf_status_ptr->retx_data_bytes,
        buf_status_ptr->ctrl_data_bytes, int_lc_ptr->pdu_size, int_lc_ptr->rlc_hdr_length,
        buf_status_ptr->effect_win_size, buf_status_ptr->nbytes, npdus);
      }
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      tdsrlc_ul_statistic_rlc_buffer_status(rlc_id, buf_status_ptr->nbytes);
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
		if(buf_status_ptr->nbytes != 0)
			{
	  			tdsmac_qsh_r99_metrics.mac_r99ul_bo_bytes += (buf_status_ptr->nbytes);
				tdsmac_qsh_r99_metrics.mac_r99ul_bo_bytes = (tdsmac_qsh_r99_metrics.mac_r99ul_bo_bytes)/2;
				//TDSMAC_MSG1(MSG_LEGACY_HIGH,"MAC_QSH:: R99 Buffer occupancy is %d ",tdsmac_qsh_r99_metrics.mac_r99ul_bo_bytes);
			}
#endif


    }
  }
}

/*==============================================================================================
 FUNCTION NAME  tdsbuild_ul_tblks

 DESCRIPTION  This function builds UL Transport Blocks
===============================================================================================*/
boolean   tdsbuild_ul_tblks
(
/* Pointer to Build Frame Indication Parameters */
tdsmac_ul_build_frame_ind_type   *bld_frame_ind_ptr
)
{
  uint16  i;
  uint8   asc=0;

  /* Pointer to internal TrCh information */
  tdsul_trch_info_type   *int_tc_ptr;

  /* Ponter to ordered TFC config data */
  tdsmac_ul_tfc_type     *oc_tfc_ptr;

  boolean data_present_on_srb = FALSE;

  /* Clear uplink buffer status info */
  memset (&tdsul_buf_status, 0, sizeof (tdsrlc_ul_buf_status_type));


  tdsrlc_ul_data_ptr = tdsrlc_ul_data_info_ptr;


  /* Clear RLC Data Count */
  tdsrlc_ul_data_ptr->nchan = 0;



  /*------------------------------------------------------------------------
  Get the logical channel buffer status from RLC
  tdsmac_hs_ul_lc_info contains the list of logical channels for which buffer
  status is requested for. tdsmac_hs_ul_lc_info structure is populated at
  configuration time by MAC.
  mac_hs_ul_ls_buf_status structure is populated by RLC. It contains the
  buffer status for the UM/AM logical channels in bytes.
  ------------------------------------------------------------------------*/
  if (tdsmac_ul_lc_info.nchan > 0 )
  {
    tds_rlc_buffer_call_from_ISR = TRUE;  
    tdsrlc_ul_enh_buffer_status(&tdsmac_ul_lc_info,&tdsul_buf_status);
    tds_rlc_buffer_call_from_ISR = FALSE; 
    
 if((TRUE == tdsl2_ivoice.enable_flag) && (NULL != tdsl2_ivoice.ivoice_cb) && (TDSGET_UL_CCTRCH_TYPE==TDSDCH))
    {
      if(bld_frame_ind_ptr->tti >= TDSL1_TTI_20MS)
      {
        tdscdma_ivoicel2_event_vfr_notification_t params;
 
        params.tds_system_time = tdsseq_get_full_sys_cnt();
        tdsl2_ivoice.ivoice_cb(tdsrrc_get_as_id(), TDSCDMA_IVOICEL2_EVENT_VFR_NOTIFICATION, &params, sizeof(tdscdma_ivoicel2_event_vfr_notification_t));
      }
    }
    tdsmac_compute_pdus_from_rlc_status(tdsmac_ul_lc_info.nchan, tdsmac_ul_lc_info.rlc_id, &tdsul_buf_status, &data_present_on_srb);

   } // nchan > 0

#if 0   /////invalid ,for l1 not provide wl1_rxd_fach_signalling_data_available, 2012.4.11
 if (TDSGET_UL_CCTRCH_TYPE == TDSRACH)
  {
    tdsmac_rach_srb_rxd_manage(data_present_on_srb);
  }
#endif  

  if (tdsblock_transmission && (TDSGET_UL_CCTRCH_TYPE==TDSRACH))
    tdsproc_block_dtch_tx_on_rach(&tdsul_buf_status);


  /* Select suitable TFC */
  tdsselect_tfc (bld_frame_ind_ptr->tti, &tdsul_buf_status, &tdsul_rlc_frame_fmt);

  /* Number of RLC Logical Channels selected is greater than zero */
  if (tdsul_rlc_frame_fmt.nchan >0)
  {
    if (TDSGET_UL_CCTRCH_TYPE == TDSRACH)
    {
      /* Selected ASC */
      asc = tdsmac_asc_selection(&tdsul_rlc_frame_fmt);

      /* Query L1 to check if the ASC has any assigned sub channel number ?
      If the ASC chosen doesnt have any assigned subchannel number then
      the l1_ul_tx_buf->num_trch will be set to 0 so that we dont ask RLC
      to build the pdu. */

      if ( (asc < TDSL1_UL_ASC_MAX) && (tdsl1_is_sub_chan_avail_for_asc(asc)))
      {
        /* Clear the memory */
        memset(tdsrlc_ul_data_ptr, 0x00, sizeof(tdsrlc_ul_frame_data_type));

        /* Get uplink RLC PDUs */
        tdsrlc_ul_build_pdus (&tdsul_rlc_frame_fmt, tdsrlc_ul_data_ptr);
      }
      else
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"There is no assigned subchan for this ASC %d",asc);
        bld_frame_ind_ptr->tx_buf_ptr->num_trch = 0;
      }
    }
    else
    {
      /* Clear the memory */
      memset(tdsrlc_ul_data_ptr, 0x00, sizeof(tdsrlc_ul_frame_data_type));

      /* Get uplink RLC PDUs */
      tdsrlc_ul_build_pdus (&tdsul_rlc_frame_fmt, tdsrlc_ul_data_ptr);
    }
  }

  /* Attach MAC header and ciphering parameters */
  tdsattach_mac_parameters ( tdsrlc_ul_data_ptr, bld_frame_ind_ptr->tx_buf_ptr);

#ifdef FEATURE_DUMMY_SIGNALING
  #error code not present
#endif

  /* Selected TFC pointer */
  oc_tfc_ptr = &tdsoc_ul_tfcs_ptr->tfc_info[TDSGET_SELECTED_TFCI];

  /* Set the transport channel tx flags */
  for (i=0; i <TDSUE_MAX_TRCH; i++)
  {
    int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(i);
    if (int_tc_ptr->prev_tx_flag == FALSE)
    {
      int_tc_ptr->prev_tx_flag =
      (tdsoc_ul_tfcs_ptr->tfs_info[i].tf_info[oc_tfc_ptr->tfi[i]].ntblks >0) ? TRUE:FALSE;
      int_tc_ptr->prev_tfi = oc_tfc_ptr->tfi[i];
    }
  }

  /* Set TFCI value */
  bld_frame_ind_ptr->tx_buf_ptr->TFCI = TDSGET_SELECTED_TFCI;
  bld_frame_ind_ptr->tx_buf_ptr->asc_num = asc;

  TDSMAC_MSG3(MSG_LEGACY_LOW,"TFCI(%d) PDU Count (%d/%d)",
  bld_frame_ind_ptr->tx_buf_ptr->TFCI,
  tdsrlc_ul_data_ptr->nchan, tdsul_rlc_frame_fmt.nchan);

  TDSMAC_MSG2(MSG_LEGACY_LOW,"TFCI %d,ASC %d",bld_frame_ind_ptr->tx_buf_ptr->TFCI,bld_frame_ind_ptr->tx_buf_ptr->asc_num);
  return(TRUE);
}

#ifdef FEATURE_TDSCDMA_SILENT_FRAME

void tdsmac_save_vfr_cb(
/* Call Back function for VFR reg/De-reg */ 
tds_vfr_cb_type vfr_cb_ptr
)
{
	if (!vfr_cb_ptr)
  	{
	    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame:Received NULL pointer for tds_vfr_cb_type, de-registration of VFR call back complete");
		//TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame: Received NULL pointer for tds_vfr_cb_type,de-registration of VFR call back complete ");
		tds_vfr_cb_ptr = NULL;
  	}
  	else
  	{
		TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame:Received CB pointer for tds_vfr_cb_type, Registration of VFR call back complete");
		//TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame: Received CB pointer for tds_vfr_cb_type,Registration of VFR call back complet ");
		tds_vfr_cb_ptr = vfr_cb_ptr;
  	}


}

void tdsmac_save_vfr_client_data(
	/* Client data provided by MVS */
	 void * clnt_data
)
{
	
	//save the client data.
	client_data_l = clnt_data;
	TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame:Saving the Client data complete");

}


/* This stub is for testing silent frame feature.
void test_call_back(tds_vfr_info_type vfr_info, void* client_data)
{

	TDSMAC_MSG0(MSG_LEGACY_HIGH," Silent frame: Inside dummy call back function ");
	TDSMAC_MSG0(MSG_LEGACY_HIGH,"Silent frame: Inside dummy call back function ");

}*/

#endif /*FEATURE_TDSCDMA_SILENT_FRAME*/

