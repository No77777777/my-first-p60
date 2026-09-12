/*===========================================================================
                  U P L I N K   T R A N S P A R E N T   M O D E   

                       R A D I O   L I N K   C O N T R O L 
DESCRIPTION
  This file provides the functions to process RLC Transparent Mode
  Uplink logical channels.
  
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

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcultm.c_v   1.10   10 Dec 2001 17:21:38   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcultm.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



#include "tdscdma_variation.h"
#include "tdsrlcultm.h"

/* =======================================================================
**                            Function Definitions
** ======================================================================= */



/*===========================================================================

FUNCTION RLCI_GET_TM_BUFFER_STATUS

DESCRIPTION
  Calculates the number of SDUs and size of the SDU. MAC uses this 
  information to determine the number of PDUs to give to the channel.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_get_tm_buffer_status
(
tdsrlci_ul_ctrl_blk_type     *ctrl_blk,  /* Pointer to Uplink Control Block */
tdsrlc_ul_lc_buf_status_type *status_ptr /* Pointer for buffer status       */
)
{
  dsm_item_type *item_ptr = NULL;

  /* initialize the pdu size  */
  status_ptr->pdu_size = 0;

  /*------------------------------------------------------------------------- 
    Get the count of the number of sdus in the queue
  -------------------------------------------------------------------------*/
  if (ctrl_blk->ul_wm_ptr == NULL || (ctrl_blk->ul_wm_ptr)->q_ptr == NULL)
  {
    TDSRLC_MSG3(MSG_LEGACY_ERROR,"TM-BO: watermark is NULL LC %d, No. of PDUs %d, Pdu Size %d",
           ctrl_blk->ctrl.tm_ctrl.rlc_id, status_ptr->npdus, status_ptr->pdu_size);    
    return;
  }

  /*------------------------------------------------------------------------- 
    Get the count of the number of sdus in the queue
  -------------------------------------------------------------------------*/
  status_ptr->npdus = (uint16)q_cnt((ctrl_blk->ul_wm_ptr)->q_ptr);

  if (status_ptr->npdus > 0)
  {
    /*-------------------------------------------------------------------------
      Get the number of bits from the first sdu in the queue and store 
      it in pdu_size. 
    -------------------------------------------------------------------------*/
    item_ptr = (dsm_item_type *)q_check((ctrl_blk->ul_wm_ptr)->q_ptr);
    if (item_ptr != NULL)
    {
      /* the SDU length in bit is stored in the app_filed of the 1st item   */
      /* of the DSM item chain.                                             */
      status_ptr->pdu_size = (uint16) item_ptr->app_field;
      if (status_ptr->pdu_size == 0)
      {
        item_ptr = (dsm_item_type *)dsm_dequeue(ctrl_blk->ul_wm_ptr);
        dsm_free_packet(&item_ptr); 
      }
    }
  }

  if (tdsrlc_debug_var & TDSRLC_DBG_UL_TM)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"TM-BO: LC %d, No. of PDUs %d, Pdu Size %d",
           ctrl_blk->ctrl.tm_ctrl.rlc_id, status_ptr->npdus, status_ptr->pdu_size);
  }
}/* tdsrlci_get_tm_buffer_status() */



/*===========================================================================

FUNCTION tdsrlci_ul_build_tm_pdus

DESCRIPTION
  Builds Uplink Transparent Mode PDUs.
  Gets the items from the Queue and sends it down to MAC.
  When segmentation is not performed, the SDU size should be equal to the 
  PDU size.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_build_tm_pdus
(
tdsrlci_ul_ctrl_blk_type      *ctrl_blk, /* Pointer to UL TM control Block  */
tdsrlc_ul_logchan_format_type *chan_fmt, /* PDU information                 */
tdsrlc_ul_logchan_data_type   *tdsdata_ptr  /* Pointer for storing PDUs        */
)
{
  dsm_item_type *item_ptr = NULL;
  int i = 0;
#ifdef FEATURE_TM_LB
  dsm_item_type *temp_ptr = NULL, *temp1_ptr = NULL;
  uint16 pdu_size, size_diff;
#endif /* FEATURE_TM_LB */

  /*-----------------------------------------------------------------------
    Get the RLC ID, and the PDU size and set the number of PDUS to 
    zero.
  -----------------------------------------------------------------------*/
  tdsdata_ptr->rlc_id = chan_fmt->rlc_id;

  /* initialize the num of PDUs in the PDU buffer */
  tdsdata_ptr->npdus = 0;
  tdsdata_ptr->pdu_size = chan_fmt->pdu_size;

  if (tdsrlc_debug_var & TDSRLC_DBG_UL_TM)
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"No. of PDUs requesed by MAC = %d,lc_id = %d",chan_fmt->npdus,chan_fmt->rlc_id);
  }

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  if (TRUE == chan_fmt->loop_back_lc_info.lb_configured_flag)
  {
    item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
    while (NULL != item_ptr)
    {
      if (TDSRLCI_SEQ_GT(chan_fmt->loop_back_lc_info.ul_cfn, 
         (((uint32)item_ptr->app_ptr + TDSMAC_LB_MAX_TTI_DELAY) & 255), 256))
      {
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"DELAY_DBG:Dropping late PDU dl_cfn %d, ul_cfn %d, const_delay %d",
            (uint32)item_ptr->app_ptr,chan_fmt->loop_back_lc_info.ul_cfn,
            chan_fmt->loop_back_lc_info.loop_back_constant_delay);
        dsm_free_packet(&item_ptr);
        item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
        continue;
      }
      break;
    }
  }
#endif
  /*-----------------------------------------------------------------------
    While there are PDUs to process, get the SDU from the uplink queue
    prepare it to be sent to MAC.
  -----------------------------------------------------------------------*/
  while (chan_fmt->npdus > 0)
  {
    /* Take an SDU out of the WM  */
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
    if (FALSE == chan_fmt->loop_back_lc_info.lb_configured_flag)
    {
#endif
    item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
    }
#endif
    if (item_ptr == NULL)
    {
      /* No SDU is in the WM. But the PDU buffer is filled up yet.  */
      /* release all prepared PDUs if any in the PDU buffer.        */
      TDSRLC_MSG0(MSG_LEGACY_HIGH,"UL TM - No items in Queue to process");
      for (i = 0; i < tdsdata_ptr->npdus; i++)
      {
        dsm_free_packet(&tdsdata_ptr->pdu_info[i].pdu_ptr);
      }
      tdsdata_ptr->npdus = 0;
      tdsdata_ptr->pdu_size = 0;
      break;
    }
    else
    {
#ifdef FEATURE_TM_LB
      pdu_size = (uint16)item_ptr->app_field;
#endif /* FEATURE_TM_LB */

      if (
#ifdef FEATURE_TM_LB
          (
#endif
           (((uint16)item_ptr->app_field) != chan_fmt->pdu_size)
#ifdef FEATURE_TM_LB
          && (ctrl_blk->ctrl.tm_ctrl.lpm != TDSRLC_LOOPBACK_MODE_2))
#endif /* FEATURE_TM_LB */
          || (dsm_length_packet(item_ptr) == 0))
      {
        /* the SDU bit size is not the same as asked.       */
        /* Or the actual packet size is zero, since the     */
        /* ARM might put an empty packet in the WM. We need */
        /* removed it.                                      */
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"UL TM SDU size != PDU size,Cannot Build");
        dsm_free_packet(&item_ptr);
      }
      else
      {
        if (tdsrlc_debug_var & TDSRLC_DBG_UL_TM)
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"TM PDU size = %d,MAC chosen size = %d",item_ptr->app_field,chan_fmt->pdu_size);
        }

#ifdef FEATURE_TM_LB
        if((ctrl_blk->ctrl.tm_ctrl.lpm == TDSRLC_LOOPBACK_MODE_2) &&
           (pdu_size < chan_fmt->pdu_size))
        {
          /*-----------------------------------------------------------------------
            Compute the extra bits need to added. If they cannot fit in
            the last DSM item, allocate new DSM item to accomodate them.
          -----------------------------------------------------------------------*/
          size_diff = (chan_fmt->pdu_size - pdu_size);

          temp_ptr = item_ptr;

          /* Get the last item of packet */
          while (temp_ptr->pkt_ptr != NULL)
          {
            temp_ptr = temp_ptr->pkt_ptr;
            pdu_size -= (temp_ptr->size << 3);
          }

          pdu_size += size_diff;
          while (pdu_size > 0)
          {
            if(pdu_size > (temp_ptr->size<<3))
            {
              temp_ptr->used = temp_ptr->size;
              pdu_size -= (temp_ptr->used <<3);
            }
            else
            {
              if(pdu_size == (temp_ptr->size<<3))
              {
                temp_ptr->used = temp_ptr->size;
              }
              else
              {
                temp_ptr->used = (pdu_size >> 3) + 1;
              }

              pdu_size = 0;
              break;
            }

            /*-----------------------------------------------------------------------
              If there is no space left in the dsm item, get a new item.
            -----------------------------------------------------------------------*/
            if ((temp1_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL)) == NULL)
            {
              dsm_free_packet(&item_ptr);
              return;
            }
            temp_ptr->pkt_ptr = temp1_ptr;
            temp_ptr = temp_ptr->pkt_ptr;
          }
        }
   #endif /* FEATURE_TM_LB */
        if(tdsdata_ptr->npdus < TDSUE_MAX_UL_RLC_PDUS)
        {
          /* Put this SDU into PDU buffer */
          tdsdata_ptr->pdu_info[tdsdata_ptr->npdus].pdu_ptr = item_ptr;
          tdsdata_ptr->npdus++;
          chan_fmt->npdus--;
          if (tdsrlc_debug_var & TDSRLC_DBG_UL_TM)
          {
            TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL TM PDU Payload: 0x%x  0x%x  0x%x",
  	       tdsrlc_copy_to_word(item_ptr->data_ptr), 
	       tdsrlc_copy_to_word(item_ptr->data_ptr+4), 
               tdsrlc_copy_to_word(item_ptr->data_ptr+8));
          }
        }
        else
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"number of PDUs %d exceed MAX %d",tdsdata_ptr->npdus,TDSUE_MAX_UL_RLC_PDUS);
        }
      }
    }
  } /* while  */
}/* tdsrlci_ul_build_tm_pdus() */


/*===========================================================================

FUNCTION  RLCI_TM_HANDLE_DISCARD_TIMER_CB

DESCRIPTION
  This callback function is called when the SDU discard timer expired.
  This function scan the WM to see whether there are SDUs needed to be 
  removed since they stay in the WM for too long. Also, it put the SDU
  discard time stamp on each new incoming SDU.
  
  The SDU discard time stamp is put in the 18th bit of the app_field, 
  since the lower 16 bits are used as the bit count of the DSM packet 
  chain.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_tm_handle_discard_timer_cb(uint32 cb_param)
{
  rlc_lc_id_type rlc_id = (rlc_lc_id_type) cb_param;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  dsm_item_type *sdu_ptr = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;

  /* Obtain control block pointer according to    */
  /* the passed-in RLC logical channel ID.        */
  if ((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id)) == NULL)
  {
    return;
  }

  TDSRLC_MSG2(MSG_LEGACY_LOW,"Set Discard timer in CB,%d %d",rlc_id,ctl_blk_ptr->ctrl.tm_ctrl.timer_discard);
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);

  if(NULL != ctl_blk_ptr->tmr_discard_ptr)
  {
  /* Set this SDU discard timer tick again */
  (void) rex_set_timer(ctl_blk_ptr->tmr_discard_ptr, (uint32)ctl_blk_ptr->ctrl.tm_ctrl.timer_discard); 
 }
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  /* scan the WM to see any SDU being expired,    */
  /* otherwise set timer stamp on the scaned SDU. */ 
  if (ctl_blk_ptr->ul_wm_ptr != NULL &&
      (sdu_ptr = (dsm_item_type *)q_check(ctl_blk_ptr->ul_wm_ptr->q_ptr)) != NULL)
  {
    while (sdu_ptr != NULL)
    {
      /* By checking the SDU discard time stamp (the 18th bit of the app_field, */
      /* we'll know whether the SDU has stayed in the WM for too long and       */
      /* needed to be removed.                                                  */
      if (sdu_ptr->app_field & 0x020000)
      {
        /* remove this SDU from the WM  */
        sdu_ptr = dsm_dequeue(ctl_blk_ptr->ul_wm_ptr);
        /* Give these DSM items back to free queue. */
        dsm_free_packet(&sdu_ptr);
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Discard SDU,LC %d",rlc_id);
        sdu_ptr = (dsm_item_type *)q_check(ctl_blk_ptr->ul_wm_ptr->q_ptr);
      }
      /* This SDU hasn't showed up yet at the time of previou discard */
      /* expiration. Put the discard timer stamp on it.               */
      else
      {
        /* Put the SDU discard time stamp on the 18th bit of the app_field  */
        sdu_ptr->app_field |= 0x020000;
        sdu_ptr = (dsm_item_type *)q_next(ctl_blk_ptr->ul_wm_ptr->q_ptr, &(sdu_ptr->link));
      }
    } /* while (sdu_ptr != NULL */
  } /* if (ctl_blk_ptr->ul_wm_ptr != NULL && */

}/* tdsrlci_tm_handle_discard_timer_cb() */

