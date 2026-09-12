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

Copyright (c) 2001 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002, 2004, 2007-2009 Qualcomm Technologies, Inc.  
All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcultm.c_v   1.10   10 Dec 2001 17:21:38   tliou  $
$Header: //components/rel/wcdma.mpss/8.4.0/rlc/src/rlcultm.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/16/18   sg      Made changes for to log interlayer commands for ML
08/31/15   as      Changes to indicate L1 to block QTA during CCCH data transmission
08/27/15   kc      Purge old/partial CCCH data only as part of PDU building.
08/18/15   as      RLC optimization to use 1 Large DSM items instead of 26 during R99 UL PDU building.
06/26/15   aa      Removing extern declarations from L2 code
06/01/15   scv     Code changes to support QSH
06/01/15   as      In FACH state MAC should send CMAC_STATUS_IND only for TM CCCH UL data.
04/25/15   scv     W+W Changes to have messages support SubID
04/10/15   kv      Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
03/19/15   aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
03/10/15   aa      Duplicated L2 task related globals and removed AS_ID hardcodings
02/04/15   kv      Fix KW errors from W+W
01/29/15   aa      L2 global reorg for W+W
07/26/14   rkmk    Reduce image size by using right macro depending upon number of parameters
05/06/14   geg     Use safer versions of memcpy() and memmove()
12/20/13   grk     Moved timer callback processing from timer task to L2 UL 
                   task for TM timer based SDU discard.
12/16/13   kc      HS_RACH: Fix to avoid resetting already freed CTRL DSM item
11/20/13   kv      Moved mask for MSG_4to9 from MSG_SSID_DIAG to MSG_SSID_WCDMA_RLC
05/10/13   kc      Added DC-HSUPA Changes
11/02/12   mrg     Add 1 byte CRC length to partial PDU info when CCCH needs to be segmented
09/21/12   as      Cleaned up code with proper identation and added missing function headers.
09/14/12   mrg     HSRACH: RLC TM mode – Don’t add the same DSM item to rlc_free_mac_i_ctrl_q
07/16/12   mrg     MAC_I & HS-RACH Compilation Warnings fixed
08/13/12   mrg     HS-RACH & MAC-I Feature updated code check-in.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/22/12   mrg     compilation fix
06/18/12   mrg     HS-RACH & MAC-I Feature code integrated
02/01/12   grk     Feature cleanup
08/25/11   as      Fix to avoid race condition of re-starting timer in Timer task 
                   when it is already freed in L2 UL task context.
01/21/11  vp/kvk   Added support for AMR WB packet simulation
12/07/10   vp      Added support for amr packet simulation in uplink
03/22/10   grk     Added debug prints for TM.
10/01/09   grk     Fixed KW warnings.
07/16/09   grk     Reverting back to dropping TM PDUs with UL cfn greater than
                   (DL cfn + max constant LB delay(10)) for LB2 test cases.
03/13/09   grk     Fix to drop TM PDUs with UL cfn greater than 
                   MAC_LB_MAX_TTI_DELAY(10 TTIs) for LB2 test cases.
03/13/09   grk     Fix to drop TM PDUs with UL cfn greater than (DL cfn + 
                   max constant LB delay) for LB2 test cases.
05/11/09   ssg     Updated Copyright Information
03/13/09   grk     Off-target lint warning fixes.
07/11/07   ssg     Added F3 msgs under FEATURE_RLC_TM_LOGGING.
07/06/07   ssg     Added fix for RLC appending data bits in case UL_tb_size
                   choosen is greater than SDU zise in loop back mode-2 under
                   FEATURE_TM_LB.
06/30/04   mv      Changed a MSG_HIGH to print the names of the arguments.
06/02/04   mv      Fixed lint errors and warnings.
07/31/02   ttl     Added initalization of status_ptr->pdu_size.
                   Added rlci_tm_handle_discard_timer_cb().
12/10/01   ttl     For WVS, removes empty dsm items from Watermark during 
                   buffer status report.
11/17/01   ttl     Free DSM item(s) if its lenght is 0 when building TM PDUs.
08/30/01   rc      Removed Watermark Check and cleaned up code.
08/27/01   rc      Set PDU size to 0 if item_ptr == NULL in 
                   rlci_get_tm_buffer_status().
08/10/01   rc      Changed some messages to fix the formatting problem in 
                   F3 messages.
06/28/01   rc      Changed MSG levels.
01/08/01   rc      Cleaned up code and added more comments.
11/22/00   rc      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



#include "wcdma_variation.h"
#include "rlcultm.h"

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

#define RLC_UL_PHY_EXTRACT_BIT(pattern, num_bit) (((pattern) >> (7 - num_bit)) & 0x1)

/*===========================================================================

   FUNCTION: rlc_crc_append_bit

===========================================================================*/
/*!
   @brief
   Computes CRC and appends it after TB 
*/
/*=========================================================================*/
void rlc_crc_append_bit
(
  uint8 *dest_addr, /*Where to write bit*/
  uint8 bit         /* Bit to write */
)
{
  /* Append bit for LSB */
  *dest_addr = (*dest_addr << 1) | (bit);
}

/* ===========================================================================*/
/*!
   @brief
   Computes CRC and appends it after TB 
*/
/*=========================================================================*/
void rlc_compute_and_append_crc_8bit
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint8* src_addr,
  uint8* dest_addr,
  uint32 size /* In bytes */
)
{
  uint32 crc_pattern =  0x19B; //0x1864CFB;
  uint32 num_bits_crc = 8;
  uint32 content_crc = 0;
  uint32 msb_mask = (1 << (num_bits_crc - 1));
  //uint32 lsb_mask = 0x01;
  uint32 code_blk_len = size;
  uint32 mask23bit = (uint32)(1 << num_bits_crc) - 1;
  uint32 index = 0;
  uint32 bit_index = 0;
  uint32 input_bit = 0;
  uint32 feedback_bit = 0;
  //uint32 num_bytes_crc = num_bits_crc / 8;
  
  for (index = 0; index < code_blk_len; index++)
  {
    for (bit_index = 0; bit_index < 8; bit_index++)
    {
      input_bit = RLC_UL_PHY_EXTRACT_BIT(src_addr[index], bit_index);
      feedback_bit = ((content_crc & msb_mask) > 0) ? (1 - input_bit): input_bit;
      if (feedback_bit > 0)
      {
        content_crc = mask23bit & ( (content_crc << 1) ^ crc_pattern);
      }
      else
      {
        content_crc = mask23bit & (content_crc << 1);
      }
    }
  }
  
  WL2_MSG_2(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: crc %d, index %d ",
                                             content_crc, index);
  
#if 0
  for (bit_index = 0; bit_index < 8; bit_index++)
  {
    rlc_crc_append_bit( &dest_addr[0], (uint8)(content_crc & lsb_mask));
    content_crc = (content_crc >> 1);
  }
#endif
  
  *dest_addr = content_crc;
  WL2_MSG_2(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: dest_addr[%d] 0x%x ",
                                             index, dest_addr[0]);
}

#ifdef FEATURE_WCDMA_HS_RACH

/*===========================================================================

FUNCTION RLCI_UL_TM_INIT

DESCRIPTION
  Initialize the Up-link TM RLC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_ul_tm_init(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  rlc_ul_table_ptr->rlc_free_tm_sdu_q.sdu_buffer_ptr = rlc_ul_table_ptr->free_tm_sdu_buffer;
  rlc_ul_table_ptr->rlc_free_tm_sdu_q.num_sdu = 0;
}

/*===========================================================================

FUNCTION RLCI_UL_UPDATE_FREE_TM_SDU_Q

DESCRIPTION
  API used to store the UL TM DSM item that needs to be freed in the next TTI.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_ul_update_free_tm_sdu_q(rlci_uplink_table_type *rlc_ul_table_ptr, dsm_item_type *tm_dsm_item)
{
  if(rlc_ul_table_ptr->rlc_free_tm_sdu_q.num_sdu < MAX_TM_SDU_PER_TTI_EDCH)
  {
    rlc_ul_table_ptr->rlc_free_tm_sdu_q.sdu_buffer_ptr[rlc_ul_table_ptr->rlc_free_tm_sdu_q.num_sdu] = tm_dsm_item;
    rlc_ul_table_ptr->rlc_free_tm_sdu_q.num_sdu++;
  }
  else
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "MAC_I: rlc_free_tm_sdu_q is FULL , num %d, max %d", rlc_ul_table_ptr->rlc_free_tm_sdu_q.num_sdu, MAX_TM_SDU_PER_TTI_EDCH, 0);
  }
}

#endif /* FEATURE_WCDMA_HS_RACH */

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
void rlci_get_tm_buffer_status
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_ul_ctrl_blk_type     *ctrl_blk,  /* Pointer to Uplink Control Block */
  rlc_ul_lc_buf_status_type *status_ptr, /* Pointer for buffer status       */
  rlc_ul_bo_query_source  query_from_bfi_flag
)
{
  dsm_item_type *item_ptr = NULL;
  uint16 npdus = 0;
  /*------------------------------------------------------------------------- 
    Get the count of the number of sdus in the queue
  -------------------------------------------------------------------------*/
  npdus = (uint16)q_cnt((ctrl_blk->ul_wm_ptr)->q_ptr);

  /* initialize the pdu size  */
  status_ptr->pdu_size = 0;

  if (npdus > 0)
  {
    /*-------------------------------------------------------------------------
      Get the number of bits from the first sdu in the queue and store 
      it in pdu_size. 
    -------------------------------------------------------------------------*/
    if ((UE_LOGCHAN_CCCH == ctrl_blk->lc_type) && (query_from_bfi_flag == BFI_QUERY))
    {
      WL2_MSG_3(HIGH, rlc_ul_table_ptr, "MAC_RACH: CCCH data info rlc_id %d, PS_info %d, npdus %d",
                ctrl_blk->ctrl.tm_ctrl.rlc_id, ctrl_blk->mac_i_ps_sn, npdus);

      while ((npdus > 1) && (item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr)) != NULL)
      {
        /* free the original TM DSM item from RLC WM */
        dsm_free_packet(&item_ptr);
        npdus--;
      }
      if (RLC_MAC_I_CCCH_PDU == ctrl_blk->mac_i_ps_sn)
      {
        ctrl_blk->mac_i_ps_sn = RLC_INVALID_SN;
        rlc_ul_clear_mac_i_ctrl_dsm_pool_per_lc(rlc_ul_table_ptr->as_id, ctrl_blk->ctrl.tm_ctrl.rlc_id);
        mac_hs_ul_clear_rlc_id_partial_info(ctrl_blk->ctrl.tm_ctrl.rlc_id, rlc_ul_table_ptr->as_id);
      }
    }
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
  status_ptr->npdus = npdus;

  if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
  {
    WL2_MSG_3(HIGH, rlc_ul_table_ptr, "TM-BO: LC %d, No. of PDUs %d, Pdu Size %d",
                                               ctrl_blk->ctrl.tm_ctrl.rlc_id,
                                               status_ptr->npdus,
                                               status_ptr->pdu_size);
  }
}/* rlci_get_tm_buffer_status() */

#ifdef FEATURE_WCDMA_HS_RACH
/*===========================================================================

FUNCTION RLCI_UL_UPDATE_FREE_TM_SDU_Q

DESCRIPTION
  API used to flush the UL TM DSM items during  RL_FAILURE or 
  PHY_CHAN_EST_FAILURE free the TM PDU (CCCH data) 

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlci_enh_ul_hsrach_flush_tm_pdus(sys_modem_as_id_e_type as_id, rlc_lc_id_type  rlc_id)
{
  dsm_item_type *item_ptr = NULL;
  rlci_ul_ctrl_blk_type  *ctrl_blk_ptr = NULL;
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  
  if(NULL == (ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)))
  {
    WL2_MSG_0(ERROR, rlc_ul_table_ptr, "ctrl_blk_ptr is NULL");
    return;
  }
  
  if (ctrl_blk_ptr->mac_i_ps_sn == RLC_MAC_I_CCCH_PDU)
  {
    rlc_ul_clear_mac_i_ctrl_dsm_pool_per_lc(rlc_ul_table_ptr->as_id, rlc_id);
    ctrl_blk_ptr->mac_i_ps_sn = RLC_INVALID_SN;
    /* For the case of AM & UM - 
     *   - Data is yet to be written to ERAM. DSM can be freed in the next TTI
     * For TM Flush case - 
     *   - As there is no need to delay one more TTI, clear the in_use flag right away
     */
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: DSM item freed - its in partial MAC_I queue");
  }
  
  /* We may be sucessfull in Preamble Tx but still PDU might have not built and RL Failure happened 
   * To avoid any stale data remaining in WM, discard the WM SDU (discussed with RRC team) 
   * RRC will take care of re-Tx of the SDU as per the (RRC) Timer expiry handler 
   */
  {
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH:Free the DSM item from the TM WM- rlc_id %d",
                                               rlc_id);
    
    /* Take an SDU out of the WM  */
    while ((item_ptr = dsm_dequeue(ctrl_blk_ptr->ul_wm_ptr)) != NULL)
    {
      /* free the original TM DSM item from RLC WM */
      dsm_free_packet(&item_ptr);
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: DSM item found in TM WM and flushed out - rlc_id %d",
                                                 rlc_id);
    }
  }
}


/*===========================================================================

FUNCTION RLCI_ENH_UL_BUILD_TM_PDUS

DESCRIPTION
  API to build Uplink Transparent Mode PDUs in case of HS-RACH.
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
void rlci_enh_ul_build_tm_pdus (rlci_uplink_table_type *rlc_ul_table_ptr,
                                rlci_ul_ctrl_blk_type      *ctrl_blk, /* Pointer to UL TM control Block */
                                rlc_ul_logchan_format_type *chan_fmt, /* PDU information                */
                                l1_l2_data_type *l1_ul_tx_buf  /* Pointer for storing PDUs       */)
{
  dsm_item_type *item_ptr = NULL;
  rlc_ctrl_pdu_type *mac_i_ctrl_pdu_ptr = NULL;
  uint16 pdu_size_to_send = 0, pdu_size_to_send_bytes = 0;
  uint16 offset = 0;
  rlc_ul_hdr_buff_type  *ul_hdr_ptr         = rlc_get_ul_hdr(rlc_ul_table_ptr, TRUE); 
  
  /*-----------------------------------------------------------------------
    Get the RLC ID, and the PDU size and set the number of PDUS to 
    zero.
  -----------------------------------------------------------------------*/

  if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: No. of PDUs requesed by MAC = %d, lc_id = %d",
                                               chan_fmt->npdus,
                                               chan_fmt->rlc_id);
  }
  if(is_maci_debug_f3_enabled(rlc_ul_table_ptr->as_id) & DEBUG_EUL_PDU_BUILDING)
  {
    WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Copy Eng->Mac_Es_Hdr: hdr_len_bits %d, offset_len_bits 0, Mac_Es_Hdr 0x%x 0x%x",
                                              chan_fmt->mac_es_hdr_size,
                                              chan_fmt->mac_es_hdr[0],
                                              chan_fmt->mac_es_hdr[1]);
  }
  mac_rlc_build_copy_engine_ctrl_info(chan_fmt->mac_es_hdr_size, 0, 0);

    /*-----------------------------------------------------------------------
      Write the MAC-es header into the ul_hdr.
    -----------------------------------------------------------------------*/
    ul_hdr_ptr->buff[ul_hdr_ptr->num_valid_words++] = rlc_copy_to_word(&(chan_fmt->mac_es_hdr[0]));

    mac_rlc_build_cipher_engine_ctrl_info(0, 0, UE_WCDMA_CIPHER_ALGO_NONE, 0, 0, 
                                       FALSE, FALSE, RLC_INVALID_COUNT_C);
    
  /* Case for Pm or Pe */
  if ((chan_fmt->mac_segment_needed & MAC_I_PARTIAL_END) && (chan_fmt->pe_pm_size_bits > 0))
  {
    mac_i_ctrl_pdu_ptr = (rlc_ctrl_pdu_type *) rlc_ul_get_mac_i_rlc_id_ctrl_dsm_ptr(rlc_ul_table_ptr, &(rlc_ul_table_ptr->mac_i_ctrl_pdu_dsm_pool), chan_fmt->rlc_id);
    pdu_size_to_send = chan_fmt->pe_pm_size_bits;
    pdu_size_to_send_bytes = CONVERT_BITS_TO_BYTES(pdu_size_to_send);
    offset = CONVERT_BITS_TO_BYTES(chan_fmt->pe_pm_offset_bits);
    item_ptr = mac_i_ctrl_pdu_ptr->item_ptr;
    if(chan_fmt->pe_pm_middle == FALSE)
    {
      /* If Pe is formed in this TTI - next TTI free this DSM item */
      rlc_ul_update_mac_i_ctrl_dsm_item (rlc_ul_table_ptr, mac_i_ctrl_pdu_ptr);
      ctrl_blk->mac_i_ps_sn = RLC_INVALID_SN;
    }
  }
  else
  {
    /*-----------------------------------------------------------------------
      While there are PDUs to process, get the SDU from the uplink queue
      prepare it to be sent to MAC.
    -----------------------------------------------------------------------*/
    if ((chan_fmt->npdus > 0) || (chan_fmt->mac_segment_needed & MAC_I_PARTIAL_START))
    {
      /* Take an SDU out of the WM  */
      
      item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
      if (item_ptr == NULL)
      {
        WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "HS_RACH: NULL TM SDU Ptr", 0, 0, 0);
      }
      else
      {
        offset = 0;
        if (chan_fmt->mac_segment_needed == MAC_I_COMPLETE_PDU)
        {
          /* Nothing to do here */
          pdu_size_to_send = chan_fmt->pdu_size[0];
          pdu_size_to_send_bytes = CONVERT_BITS_TO_BYTES(pdu_size_to_send);
          /* If Pc is formed in this TTI - next TTI free this DSM item */
          rlci_ul_update_free_tm_sdu_q(rlc_ul_table_ptr, item_ptr);
          ctrl_blk->mac_i_ps_sn = RLC_INVALID_SN;
        }
        else if (chan_fmt->mac_segment_needed & MAC_I_PARTIAL_START)
        {
          if (chan_fmt->ps_size_bits > 0)
          {
            rlc_ul_enh_store_mac_i_ctrl_pdu(rlc_ul_table_ptr, MAC_I_PARTIAL_START, ctrl_blk, CONVERT_BITS_TO_BYTES(item_ptr->app_field), (item_ptr->data_ptr + offset));
            mac_i_ctrl_pdu_ptr = rlc_ul_get_mac_i_rlc_id_ctrl_dsm_ptr(rlc_ul_table_ptr, &(rlc_ul_table_ptr->mac_i_ctrl_pdu_dsm_pool), chan_fmt->rlc_id);
            
            /* Append the 8bit(1Byte) CRC and increment the used size for this DSM */
            /* Default - follow the L1 spec - CRC is reversed and appended */
            rlc_compute_and_append_crc_8bit(rlc_ul_table_ptr,
                                            mac_i_ctrl_pdu_ptr->item_ptr->data_ptr, 
                                            mac_i_ctrl_pdu_ptr->item_ptr->data_ptr + mac_i_ctrl_pdu_ptr->pdu_size, 
                                            mac_i_ctrl_pdu_ptr->pdu_size);
            mac_i_ctrl_pdu_ptr->item_ptr->used += 1;
            mac_i_ctrl_pdu_ptr->pdu_size += 1;
            
            pdu_size_to_send = chan_fmt->ps_size_bits;
            pdu_size_to_send_bytes = CONVERT_BITS_TO_BYTES(pdu_size_to_send);
            
            /* free the original TM DSM item from RLC WM */
            dsm_free_packet(&item_ptr);
                
            item_ptr = mac_i_ctrl_pdu_ptr->item_ptr;
                
            if(is_maci_debug_f3_enabled(rlc_ul_table_ptr->as_id)  & DEBUG_EUL_PDU_BUILDING)
            {
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: SS %d, ps_size_bits %d , pdu_size_to_send %d",
                                                         chan_fmt->mac_segment_needed,
                                                         chan_fmt->ps_size_bits,
                                                         pdu_size_to_send);
            }
          }
          else
          {
            WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "MAC_HS_RACH: Partial Start with Zero Size is invalid", 0, 0, 0);
          }
        }
      }
    } 
  }

  rlc_ul_form_io_vec_per_sdu(rlc_ul_table_ptr, item_ptr, offset, pdu_size_to_send_bytes, l1_ul_tx_buf);
  
  #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  rlc_qsh_metric_global[rlc_ul_table_ptr->as_id].num_pdu_bytes_tx += pdu_size_to_send_bytes;
  #endif
  
  if(is_maci_debug_f3_enabled(rlc_ul_table_ptr->as_id) & DEBUG_EUL_PDU_BUILDING)
  {
    WL2_MSG_3(HIGH, rlc_ul_table_ptr, "MAC_HS_RACH: SS %d, offset %d, pdu_size_to_send %d",
                                                 chan_fmt->mac_segment_needed,
                                                 offset,
                                                 pdu_size_to_send);

    if(mac_i_ctrl_pdu_ptr != NULL)
    {
      WL2_MSG_9(HIGH, rlc_ul_table_ptr,
          "MAC_HS_RACH: TM PDU Payload: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr), 
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+4), 
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+8),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+12),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+16),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+20),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+24),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+28),
          rlc_copy_to_word(mac_i_ctrl_pdu_ptr->item_ptr->data_ptr+32) );
    }
    else
    {
       WL2_MSG_0(ERROR, rlc_ul_table_ptr, "mac_i_ctrl_pdu_ptr is NULL");
    }
    
      WL2_MSG_9(HIGH, rlc_ul_table_ptr,
          "MAC_HS_RACH: IOVEC - TM PDU Payload: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr), 
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+4), 
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+8),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+12),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+16),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+20),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+24),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+28),
          rlc_copy_to_word(l1_ul_tx_buf->cur_ul_buf_ptr->data_seg_list_ptr+32) );
  }
  
  mac_rlc_build_copy_engine_ctrl_info(0, pdu_size_to_send, 0);
  mac_rlc_build_cipher_engine_ctrl_info(0, 0, UE_WCDMA_CIPHER_ALGO_NONE, 0, 0, 
                                       FALSE, FALSE, RLC_INVALID_COUNT_C);
#ifdef FEATURE_DUAL_SIM  
  if (UE_LOGCHAN_CCCH == ctrl_blk->lc_type)
  {
    wl1_ds_block_qta_for_ul_ccch_msg(rlc_ul_table_ptr->as_id);
  }
#endif
}
#endif

/*===========================================================================

FUNCTION rlci_ul_build_tm_pdus

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
void rlci_ul_build_tm_pdus
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_ul_ctrl_blk_type      *ctrl_blk, /* Pointer to UL TM control Block  */
  rlc_ul_logchan_format_type *chan_fmt, /* PDU information                 */
  rlc_ul_logchan_data_type   *data_ptr  /* Pointer for storing PDUs        */
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
  data_ptr->rlc_id = chan_fmt->rlc_id;

  /* initialize the num of PDUs in the PDU buffer */
  data_ptr->npdus = 0;
  data_ptr->pdu_size = chan_fmt->pdu_size[0];

  if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "No. of PDUs requesed by MAC = %d, lc_id = %d",
                                               chan_fmt->npdus,
                                               chan_fmt->rlc_id);
  }

#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
  if (TRUE == chan_fmt->loop_back_lc_info.lb_configured_flag)
  {
    item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
    while (NULL != item_ptr)
    {
      if (RLCI_SEQ_GT(chan_fmt->loop_back_lc_info.ul_cfn, 
         (((uint32)item_ptr->app_ptr + MAC_LB_MAX_TTI_DELAY) & 255), 256))
      {
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "DELAY_DBG:Dropping late PDU dl_cfn %d, ul_cfn %d, const_delay %d",
                                                   (uint32)item_ptr->app_ptr,
                                                   chan_fmt->loop_back_lc_info.ul_cfn,
            chan_fmt->loop_back_lc_info.loop_back_constant_delay);
        dsm_free_packet(&item_ptr);
        item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
        continue;
      }
      break;
    }
  }
#endif
  
  #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  rlc_qsh_metric_global[rlc_ul_table_ptr->as_id].num_pdu_bytes_tx += (chan_fmt->npdus*data_ptr->pdu_size);
  #endif
  
  /*-----------------------------------------------------------------------
    While there are PDUs to process, get the SDU from the uplink queue
    prepare it to be sent to MAC.
  -----------------------------------------------------------------------*/
  while (chan_fmt->npdus > 0)
  {
    /* Take an SDU out of the WM  */
#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
    if (FALSE == chan_fmt->loop_back_lc_info.lb_configured_flag)
    {
#endif
    item_ptr = dsm_dequeue(ctrl_blk->ul_wm_ptr);
#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
    }
#endif
    if (item_ptr == NULL)
    {
      /* No SDU is in the WM. But the PDU buffer is filled up yet.  */
      /* release all prepared PDUs if any in the PDU buffer.        */
      WL2_MSG_0(HIGH, rlc_ul_table_ptr, "UL TM - No items in Queue to process");
      for (i = 0; i < data_ptr->npdus; i++)
      {
        dsm_free_packet(&data_ptr->pdu_info[i].pdu_ptr);
      }
      data_ptr->npdus = 0;
      data_ptr->pdu_size = 0;
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
           (((uint16)item_ptr->app_field) != chan_fmt->pdu_size[0])
#ifdef FEATURE_TM_LB
          && (ctrl_blk->ctrl.tm_ctrl.lpm != RLC_LOOPBACK_MODE_2))
#endif /* FEATURE_TM_LB */
          || (dsm_length_packet(item_ptr) == 0))
      {
        /* the SDU bit size is not the same as asked.       */
        /* Or the actual packet size is zero, since the     */
        /* ARM might put an empty packet in the WM. We need */
        /* removed it.                                      */
        ERR("UL TM SDU size != PDU size, Cannot Build",0,0,0);
        dsm_free_packet(&item_ptr);
      }
      else
      {
        if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
        {
          WL2_MSG_2(HIGH, rlc_ul_table_ptr, "TM PDU size = %d, MAC chosen size = %d",
                                                     item_ptr->app_field,
                                                     chan_fmt->pdu_size[0]);
        }

#ifdef FEATURE_TM_LB
        if((ctrl_blk->ctrl.tm_ctrl.lpm == RLC_LOOPBACK_MODE_2) &&
           (pdu_size < chan_fmt->pdu_size[0]))
        {
          /*-----------------------------------------------------------------------
            Compute the extra bits need to added. If they cannot fit in
            the last DSM item, allocate new DSM item to accomodate them.
          -----------------------------------------------------------------------*/
          size_diff = (chan_fmt->pdu_size[0] - pdu_size);

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
        if(data_ptr->npdus < UE_MAX_UL_RLC_PDUS)
        {
          /* Put this SDU into PDU buffer */
          data_ptr->pdu_info[data_ptr->npdus].pdu_ptr = item_ptr;
          data_ptr->pdu_info[data_ptr->npdus].pdu_offset = 0;
          data_ptr->npdus++;
          chan_fmt->npdus--;
          if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
          {
            WL2_MSG_3(HIGH, rlc_ul_table_ptr, "UL TM PDU Payload: 0x%x  0x%x  0x%x",
            rlc_copy_to_word(item_ptr->data_ptr), 
            rlc_copy_to_word(item_ptr->data_ptr+4), 
            rlc_copy_to_word(item_ptr->data_ptr+8));
          }
        }
        else
        {
          WL2_MSG_2(HIGH, rlc_ul_table_ptr, "number of PDUs %d exceed MAX %d",
                                                     data_ptr->npdus,
                                                     UE_MAX_UL_RLC_PDUS);
        }
      }
    }
  } /* while  */
#ifdef FEATURE_DUAL_SIM  
  if ((UE_LOGCHAN_CCCH == ctrl_blk->lc_type) && (data_ptr->npdus))
  {
    wl1_ds_block_qta_for_ul_ccch_msg(rlc_ul_table_ptr->as_id);
  }
#endif
}/* rlci_ul_build_tm_pdus() */


/*===========================================================================

FUNCTION  RLCI_TM_HANDLE_DISCARD_TIMER_CB

DESCRIPTION
  Post command to L2 UL task to process timer discard event for timer based
  SDU discard for TM.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_tm_handle_discard_timer_cb(uint32 cb_param)
{
  wcdma_l2_ul_cmd_type  *rlc_ul_cmd_buffer;
  sys_modem_as_id_e_type as_id = cb_param & 0x1;  /*BIT 0*/
  rlc_lc_id_type rlc_id = (cb_param >> 1) & 0xFF; /*BITS 1:8*/

  /* Get L2 UL command buffer */
  if ((rlc_ul_cmd_buffer = wcdma_l2_ul_get_cmd_buf()) == NULL)
  {
    WL2_MSG_1(ERROR, RLCI_GET_UL_TBL_PTR(as_id), "NULL Ptr when sending RLC_HANDLE_SDU_DISCARD_TIMEOUT_EVENT LD ID = %d",
                                                rlc_id);
    return;
  }

  /* Fill in the Command Header */
  rlc_ul_cmd_buffer->cmd_hdr.cmd_id = RLC_HANDLE_TM_TIMER_BASED_SDU_DISCARD_CB_IND;
  rlc_ul_cmd_buffer->cmd_hdr.as_id = as_id;
  
  /* Fill in the data */
  rlc_ul_cmd_buffer->cmd_data.ul_rlc_data_id = rlc_id;
  
  WL2_MSG_3(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "Sending RLC_HANDLE_SDU_DISCARD_TIMEOUT_EVENT LC ID = %d, AS ID %d, cb param = %d",
                                               rlc_id,
                                               as_id,
                                               cb_param);
  
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  /* Send command to L2 UL Task */
  wcdma_l2_ul_put_cmd(rlc_ul_cmd_buffer);

}

/*===========================================================================

FUNCTION  RLCI_TM_PROCESS_DISCARD_TIMER_CB

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
void rlci_tm_process_discard_timer_cb(rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type rlc_id)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  dsm_item_type *sdu_ptr = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;

  /* Obtain control block pointer according to    */
  /* the passed-in RLC logical channel ID.        */
  if ((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)) == NULL)
  {
    return;
  }

  WL2_MSG_2(LOW, rlc_ul_table_ptr, "Set Discard timer in CB, %d %d",
                                            rlc_id,
                                            ctl_blk_ptr->ctrl.tm_ctrl.timer_discard);

  datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);

  if(NULL != ctl_blk_ptr->tmr_discard_ptr)
  {
    /* Set this SDU discard timer tick again */
    (void) rex_set_timer(ctl_blk_ptr->tmr_discard_ptr, (uint32)ctl_blk_ptr->ctrl.tm_ctrl.timer_discard); 
  }

  L2_ACQ_UL_LW_UNLOCK(datapath_mutex);

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
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Discard SDU, LC %d",
                                                   rlc_id);
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
}/* rlci_tm_handle_discard_timer_cb() */

