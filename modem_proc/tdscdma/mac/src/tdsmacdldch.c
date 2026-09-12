/*=======================================================================================

                         D O W N L I N K   D C H   H A N D L E R


GENERAL DESCRIPTION

  This module contains functions to :
  - process downlink DCH configuration and data
  - process downlink DCH transport block set header information

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


============================================================================================*/

/*============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdldch.c_v   1.5   07 Aug 2001 18:28:52   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdldch.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     --------------------------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

============================================================================================*/

/*============================================================================================
                                 INCLUDE FILES
============================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "tdsmacrrcif.h"
#include  "tdsl2dltask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacdl.h"
#include  "tdsmathutil.h"
#include  "tdsmacdldch.h"
#include  "tdsl1m.h"


/*===========================================================================================
                         DATA DECLARATIONS
===========================================================================================*/

/* Poking this flag to TRUE at run time will provide debug messages to see where the
ciphering went wrong */

boolean tdsciphering_garbled = TRUE;

/* Once the ciphering_grabled message is set to TRUE the debug print messages
would eb printed every 500 ms. */
uint8 tdsmac_dl_debug_print_interval = 50;

uint8 tdsprev_print_cfn = 0xff;

/* Look up table for finding the hfn associated with the cfn. This table is updated
in tdsmacul.c */
extern uint32 tdscfn_hfn_table[TDSMAX_CN_DOMAIN][256];
extern tdsmac_ciphering_table       tdscfn_ciphering_table[256];

/* Number of active DL DCHs */
uint8    tdsnum_active_dl_dchs;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  #include "tdsmaculdl.h"
tdslb_delay_struct_type  tdslb_delay_type;
extern tdsmac_loop_back_mode_e_type tdsloopback_test_mode;
extern boolean                   tdscfn_marked;
#endif

/*==============================================================================================
FUNCTION NAME   tdsprocess_dl_dcch_dtch_dch_config

DESCRIPTION   This function processes config info of DCCH/DTCH that is maaped to DL DCH
==============================================================================================*/
void  tdsprocess_dl_dcch_dtch_dch_config
(
/* Ordered DCH config data pointer */
tdsmac_dl_dch_trch_config_type   *oc_dch_ptr,

/* Internal Dl DCH Config data pointer */
tdsdl_dch_config_info_type     *int_dch_ptr
)
{
  boolean  valid;
  uint8    i;
  uint8    tbl_index = 0;

  /* Pointer to ordered DCCH/DTCH config data */
  tdsmac_dl_ded_logch_config_type  *oc_dlc_ptr;

  /* Pointer to internal DCCH/DTCH config data */
  tdsdlc_dch_config_info_type      *int_dlc_ptr;

  /* Number of DCCH/DTCHs */
  int_dch_ptr->num_dlc = oc_dch_ptr->ndlchan;

  /* Set Dedicated Logical Channel Enable flag to FALSE and mark all dedicated logical
     channels as disabled */
  for (i=0; i < MAX_DED_LOGCHAN_PER_TRCH; i++)
    int_dch_ptr->dlcinfo[i].enable = FALSE;

  /* Process each DCCH/DTCH config information */
  for (i=0 ; i < oc_dch_ptr->ndlchan; i++)
  {

    /* Initially mark the parameter validity flag to FALSE */
    valid = FALSE;

    /* Pointer to the ordered DCCH/DTCH config data */
    oc_dlc_ptr = &oc_dch_ptr->dlchan_info[i];

    /* Invalid RLC ID */
    if (TDSDL_RLC_ID_IS_INVALID(oc_dlc_ptr->rlc_id))
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC ID %d",i,oc_dlc_ptr->rlc_id);
    }
    /* RLC Mode is invalid */
    else if (TDSDL_RLC_MODE_IS_INVALID(oc_dlc_ptr->rlc_mode))
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC Mode %d",i,oc_dlc_ptr->rlc_mode);
    }
    /* RB ID is invalid */
    else if (TDSDL_RB_ID_IS_INVALID(oc_dlc_ptr->rb_id))
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RB ID",i,oc_dlc_ptr->rb_id);
    }
    /* Only one DCCH/DTCH is mapped to FACH */
    else if (int_dch_ptr->num_dlc == 1)
    {
      tbl_index = 0;
      valid = TRUE;
    }
    /* Multiple DCCH/DTCHs are mapped to DL DCH */
    else if (int_dch_ptr->num_dlc > 1)
    {
      /* MAC ID is invalid */
      if (TDSDL_MAC_ID_IS_INVALID(oc_dlc_ptr->mac_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid MAC ID %d",i,oc_dlc_ptr->mac_id);
      }
      else
      {
        tbl_index = (uint8)(oc_dlc_ptr->mac_id - TDSMAC_MIN_MAC_ID);
        valid = TRUE;
      }
    }

    /* Config data is valid */
    if (valid == TRUE)
    {

      /* Pointer to internal DCCH/DTCH config data */
      int_dlc_ptr = &int_dch_ptr->dlcinfo[tbl_index];

      /* Save DCCH/DTCH info in FACH CONFIG table */
      int_dlc_ptr->enable = TRUE;
      int_dlc_ptr->rlc_id = oc_dlc_ptr->rlc_id;
      int_dlc_ptr->rb_id  = oc_dlc_ptr->rb_id;
      int_dlc_ptr->rlc_mode = oc_dlc_ptr->rlc_mode;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
      int_dlc_ptr->chan_type = oc_dlc_ptr->chan_type;
#endif
      TDSMAC_MSG3(MSG_LEGACY_LOW,"DCCH/DTCH%d: RLC ID =%d, Mode =%d",
      i, oc_dlc_ptr->rlc_id, oc_dlc_ptr->rlc_mode);
    }
  }
}

/*============================================================================================
FUNCTION NAME   tdsprocess_dl_dch_config

DESCRIPTION   This function processes DL DCH configuration data

============================================================================================*/
void  tdsprocess_dl_dch_config
(
/* Number of DCHs */
uint8  num_dchs,

/* Pointer to ordered DCH configuration data list */
tdsmac_dl_dch_trch_config_type *oc_dch_ptr_list[]
)
{
  uint8   i;

  /* ID to Index Table Pointer */
  tdstrch_id_to_idx_map_type     *id_to_idx_ptr;

  /* Pointer to DCH TrCh config info */
  tdsmac_dl_dch_trch_config_type    *oc_dch_ptr;

  /* Pointer to config info in DCH Config table */
  tdsdl_dch_config_info_type      *int_dch_ptr;

  /* Set number of active DCHs to zero */
  tdsnum_active_dl_dchs = 0;

  /* Initialize DCH ID to Index Mapping Table */
  for (i=0; i<MAX_TRCH; i++)
  {
    tdsdl_dch_id_to_idx[i].valid = FALSE;
  }

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  /* Initialize the first_tb boolean to TRUE so that UL MAC would use this
  flag to calculate the delay and subsequently this constant delay would
  be maintained for transmitting any block in the UL */
  memset(&tdslb_delay_type, 0x00, sizeof(tdslb_delay_struct_type));
  tdslb_delay_type.first_tb = TRUE;

  for(i= 0; i< TDSMAX_NUM_CFN; i++)
  {
    tdslb_delay_type.dl_cfn[i] = TDSLB_DELAY_INVALID_CFN;
  }
#endif

  /* Invalid number of DCHs */
  if (num_dchs > TDSUE_MAX_TRCH)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of DCHs %d",num_dchs);
  }
  /* Process DCH config data */
  else
  {

    for (i=0; i < num_dchs; i++)
    {

      /* Pointer to DCH CCTrCh info */
      oc_dch_ptr = oc_dch_ptr_list[i];

      /* DCH ordered config data pointer is NULL */
      if (oc_dch_ptr == NULL)
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH%d config pointer is NULL",i);
      }
      /* Invalid TrCh ID */
      else if (TDSDL_TRCH_ID_IS_INVALID(oc_dch_ptr->trch_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCH%d with invalid TrCh ID %d",i,oc_dch_ptr->trch_id);
      }
      /* Process DCH config data */
      else
      {
        /* Pointer to DCH config table entry */
        int_dch_ptr = &tdsdl_dch_cfg_info[tdsnum_active_dl_dchs];

        /* Get Transport ID to Index map Table */
        id_to_idx_ptr = TDSDL_DCH_ID_TO_IDX_PTR(oc_dch_ptr->trch_id);
        id_to_idx_ptr->valid = TRUE;
        id_to_idx_ptr->idx = tdsnum_active_dl_dchs;

        /* Save CCTrCh ID in internal DCH Config Table */
        int_dch_ptr->cctrch_id = oc_dch_ptr->cctrch_id;
        int_dch_ptr->trch_id = oc_dch_ptr->trch_id;

        TDSMAC_MSG3(MSG_LEGACY_LOW,"DL DCH%d: TrChID=%d CCTrChID=%d",
        i, int_dch_ptr->trch_id, int_dch_ptr->cctrch_id);

        /* Process DCCH/DTCH data */
        tdsprocess_dl_dcch_dtch_dch_config (oc_dch_ptr, int_dch_ptr);

        /* Increment the number of active DL DCHs */
        tdsnum_active_dl_dchs++;
      }
    }
  }
}

/*=============================================================================================================
FUNCTION    tdsprocess_dl_dch_tbs_hdr

DESCRIPTION   This function processes the header information of DCH transport block set

==============================================================================================================*/
void  tdsprocess_dl_dch_tbs_hdr
(
/* transport Block ID */
uint8 tb_id,

/* Pointer to internal DCH config info */
tdsdl_dch_config_info_type   *int_dch_ptr,

/* Pointer to the header info of Transport Block Set */
tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
)
{
  boolean   valid, status;

  uint8 i, ct_field, num_tb, dlcidx = 0;

  /* Bit offset of RLC PDU */
  uint8 rlc_pdu_offset = 0;

  /* Pointer to DL TB info */
  tdsdl_tb_info_type       *tdsdl_tb_info_ptr;

  /* Pointer to internal DCCH/DTCH config info */
  tdsdlc_dch_config_info_type  *dlcptr = NULL;

  /* Pointer to TB header information */
  tdsl1_dl_tb_hdr_struct_type    *tb_hdr_ptr;

  /* RLC Header Info */
  tdsrlc_dl_tb_hdr_info_type   rlc_hdr_info;


  /* Current CFN for a transport block */
  uint8 current_tbs_cfn, index;

  uint32                    cipher_seg[3];

  /* RLC Ciphering Info */
  tdsrlc_dl_tb_cipher_info_type  rlc_cipher_info;

  /* Pointer to TB Ciphering Parameters */
  tdsl1_dl_ciphering_param_struct_type *tb_cipher_ptr;

  /* COUNT_C value to be given to Layer 1 */
  uint32  countc;
  uint32 temp_tb_header = 0;

  memset(&rlc_hdr_info, 0, sizeof(tdsrlc_dl_tb_hdr_info_type));

  if (tbs_hdr_ptr == NULL)
  {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"TBS_HDR_PTR NULL\n");
      return;
  }

  /* Get the current CFN number for the transport block set */
  current_tbs_cfn = tbs_hdr_ptr->info.non_hs.cfn;



  /* Number of transport blocks */
  num_tb = tbs_hdr_ptr->num_tb;

  /* Process each transport block of the set */
  for (i=0 ; i < num_tb; i++)
  {
    /* Initially set valid flag to FALSE */
    valid = FALSE;

    /* Get pointer to transport block information */
    tb_hdr_ptr = &tbs_hdr_ptr->tb_hdr_info[i];

    /* TB Header Pointer is NULL */
    if (tb_hdr_ptr == NULL) /*lint !e774*/
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH TB%d header pointer is NULL",i);
    }
    /* Only one DCCH/DTCH is mapped to DCH */
    else
    {
      
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
      
      if ((tdsloopback_test_mode == TDSMAC_LOOPBACK_MODE_2) &&
          (int_dch_ptr->dlcinfo[0].rlc_mode == UE_MODE_TRANSPARENT) 
           && (tbs_hdr_ptr->info.non_hs.tb_num_bits == 0)
         )
      {
        /* Case where - TB data of 0 bits is received. No Hdr to be processed.
         * Return valid as TRUE - so that, L1 will pass the CRC bits alone to 
         * MAC for loopback 
         */
        tb_hdr_ptr->ciphering_param.ciphering_control = FALSE;
        
        /* Save RLC ID in Transport Block info table */
        tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_id + i);
        
        /* Set other header parameters of TB */
        tb_hdr_ptr->tb_id = (uint8)(tb_id + i); /*lint !e613*/
        tb_hdr_ptr->valid = TRUE; /*lint !e613*/
        tb_hdr_ptr->num_bit_offset_trblk = rlc_pdu_offset; /*lint !e613*/
        
        tdsdl_tb_info_ptr->valid = TRUE;
        tdsdl_tb_info_ptr->rlc_id = int_dch_ptr->dlcinfo[0].rlc_id;
        
        dlcidx = 0;
        /* Pointer to internal DCCH/DTCH config info */
        dlcptr = &int_dch_ptr->dlcinfo[dlcidx];
        
        /* DCCH/DTCH is enabled */
        if (dlcptr->enable == TRUE)
        {
          /* Constant Loopback Delay has to be maintained for DCH type transport channel
          and when the loopback test mode is either of type 1 or 2 */
          /*---------------------------------------------------------------------------------------
           register the downlink CFN when the transport block was received in the
           downlink. This block would be transmitted in the uplink after applying
           the constant delay. If this cfn is already registered in the array then do not
           re-enter it. If tdscfn_marked is set to TRUE then this cfn exists in the database already
           ---------------------------------------------------------------------------------------*/
          if (tdscfn_marked == FALSE)
          {
            if(tdslb_delay_type.cur_idx < TDSMAX_NUM_CFN)
            {
              tdslb_delay_type.dl_cfn[tdslb_delay_type.cur_idx] = current_tbs_cfn;
              tdslb_delay_type.cur_idx++;
              tdslb_delay_type.cur_idx = tdslb_delay_type.cur_idx % TDSMAX_NUM_CFN;
              TDSMAC_MSG3(MSG_LEGACY_HIGH,"cur_dl_cfn %d ntb %d rlc_id %d", current_tbs_cfn,num_tb,rlc_hdr_info.rlc_id);
              tdscfn_marked = TRUE;
            }
            else
            {
              TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in tdslb_delay_type.dl_cfn[] exceed TDSMAX_NUM_CFN");
            }
          }
        }
        
        continue;
      }
#endif /* FEATURE_TDSCDMA_TC_CONSTANT_DELAY */
      
      if (int_dch_ptr->num_dlc == 1)
      {
        dlcidx = 0;
        rlc_pdu_offset = 0;
        valid = TRUE;
      }
      /* Multiple DCCH/DTCHs mapped to DCH */
      else if (int_dch_ptr->num_dlc > 1)
      {
        /* C/T field */
        ct_field = (uint8)(tb_hdr_ptr->tb_header[0] >> 4);

        /* Invalid C/T field */
        if (TDSDL_CT_FLD_IS_INVALID(int_dch_ptr->num_dlc,ct_field))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with invalid C/T %x",tb_id,ct_field);

        }
        /* Valid C/T field */
        else
        {
          dlcidx = ct_field;
          rlc_pdu_offset = 4;
          valid = TRUE;

          /* MAC drops the PDU with all zero contents */
          if (!ct_field && !tb_hdr_ptr->tb_header[0] && !tb_hdr_ptr->tb_header[1] && !tb_hdr_ptr->tb_header[2]
              && !tb_hdr_ptr->tb_header[3] && !tb_hdr_ptr->tb_header[4] && !tb_hdr_ptr->tb_header[5])
          {
            /* Pointer to internal DCCH/DTCH config info */
            dlcptr = &int_dch_ptr->dlcinfo[dlcidx];

            if ((dlcptr->chan_type==UE_LOGCHAN_DCCH) &&(dlcptr->rlc_mode == UE_MODE_UNACKNOWLEDGED))
            {
              valid = FALSE;
              TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH Block headers are 0,discarding %d tb",i);
            }
          }
        }
      }

      /* Initialize the ciphering _control flag to FLASE */
      tb_hdr_ptr->ciphering_param.ciphering_control = FALSE;

    } /* TB HDR not null */

    /* TB Parameters are valid */
    if (valid == TRUE)
    {
      /* Pointer to internal DCCH/DTCH config info */
      dlcptr = &int_dch_ptr->dlcinfo[dlcidx];

      /* DCCH/DTCH is enabled */
      if (dlcptr->enable == TRUE)
      {
        /* Get RLC Header info */
        rlc_hdr_info.rlc_id = dlcptr->rlc_id;
        rlc_hdr_info.rlc_pdu_start_offset = rlc_pdu_offset;
        rlc_hdr_info.hdr_ptr = (uint8 *)&tb_hdr_ptr->tb_header[0];


        rlc_cipher_info.cipher_flag = FALSE;
        status = FALSE;

        if (tbs_hdr_ptr->info.non_hs.tb_num_bits > 0)
        {
          status = tdsrlc_dl_decode_pdu_hdr (&rlc_hdr_info, &rlc_cipher_info);
        }
        
        /*Decode the PDU header*/
        if (status == TRUE)
        {
          /* DCCH/DTCH is in Transparent Mode */
          tb_cipher_ptr = &tb_hdr_ptr->ciphering_param;/*lint !e613*/

          // Set ciphering control to FALSE.
          tb_cipher_ptr->ciphering_control = FALSE;

          TDSMAC_MSG2(MSG_LEGACY_LOW,"TrchId->%d;TrchType->%d;",int_dch_ptr->trch_id,dlcptr->chan_type);

#ifdef FEATURE_SRB5_SUPPORT
          if (dlcptr->chan_type != UE_LOGCHAN_TM_DCCH_DL)
          {
#endif /* FEATURE_SRB5_SUPPORT */
            if (dlcptr->rlc_mode == UE_MODE_TRANSPARENT)
            {
              /* Get the domain this channel belongs to */
              index = int_dch_ptr->cs_or_ps_domain;

              tbs_hdr_ptr->tb_in_tm_mode = TRUE;
              /* If ciphering is enabled */
              if ((tdscfn_ciphering_table[current_tbs_cfn].cipher_enable) &&
                  ((tdscfn_ciphering_table[current_tbs_cfn].cipher_algo == UE_UMTS_CIPHER_ALGO_UEA1) 
                  ||(tdscfn_ciphering_table[current_tbs_cfn].cipher_algo == UE_UMTS_CIPHER_ALGO_UEA2)
              ))
              {

                tb_cipher_ptr->ciphering_control = TRUE;

                /* Copy the ciphering related parameters to pass to Layer 1 */
                countc =  ((tdscfn_hfn_table[index][current_tbs_cfn] << 8) | current_tbs_cfn);

                /* Setting this flag to TRUE at run time will provide debug messages to
                figure out what is going wrong with ciphering */
                if ((i==0) && (tdsciphering_garbled &&
                               (current_tbs_cfn % tdsmac_dl_debug_print_interval == 0)) &&
                               (current_tbs_cfn != tdsprev_print_cfn))
                {
                  TDSMAC_MSG3(MSG_LEGACY_HIGH,"DL tdscount_c 0x%x dl_cfn 0x%x key 0x%x",
                  countc,current_tbs_cfn,tdscfn_ciphering_table[current_tbs_cfn].key_idx);

                  temp_tb_header = 0;
                  temp_tb_header = ((0x000000FF & tb_hdr_ptr->tb_header[0])<<24)|((0x000000FF & tb_hdr_ptr->tb_header[1])<<16)|\
                              ((0x000000FF & tb_hdr_ptr->tb_header[2])<<8)|(0x000000FF & tb_hdr_ptr->tb_header[3]);
                  cipher_seg[0] = BIG_TO_LITTLE_ENDIAN_32(temp_tb_header);

                  temp_tb_header = 0;
                  temp_tb_header = ((0x000000FF & tb_hdr_ptr->tb_header[4])<<24)|((0x000000FF & tb_hdr_ptr->tb_header[5])<<16)|\
                              ((0x000000FF & tb_hdr_ptr->tb_header[6])<<8)|(0x000000FF & tb_hdr_ptr->tb_header[7]);
                  cipher_seg[1] = BIG_TO_LITTLE_ENDIAN_32(temp_tb_header);

                  temp_tb_header = 0;
                  temp_tb_header = ((0x000000FF & tb_hdr_ptr->tb_header[8])<<24)|((0x000000FF & tb_hdr_ptr->tb_header[9])<<16)|\
                              ((0x000000FF & tb_hdr_ptr->tb_header[10])<<8)|(0x000000FF & tb_hdr_ptr->tb_header[11]);
                  cipher_seg[2] = BIG_TO_LITTLE_ENDIAN_32(temp_tb_header);

                  TDSMAC_MSG3(MSG_LEGACY_HIGH,"PDU %x %x %x",cipher_seg[0], cipher_seg[1], cipher_seg[2]);

                }

                tb_cipher_ptr->ciphering_algo = tdscfn_ciphering_table[current_tbs_cfn].cipher_algo;
                tb_cipher_ptr->ciphering_key_id  = tdscfn_ciphering_table[current_tbs_cfn].key_idx;
                tb_cipher_ptr->ciphering_count   = countc;

                tb_cipher_ptr->ciphering_bit_offset = rlc_pdu_offset;
                tb_cipher_ptr->ciphering_radio_bearerid = (uint8)dlcptr->rb_id;
                tdsprev_print_cfn = current_tbs_cfn;

              }
            }
            /*DCCH/DTCH in non-Transparent Mode */
            else
            {
              tbs_hdr_ptr->tb_in_tm_mode = FALSE;
              /* Copy the cipherign parameters given by RRC */
              if (rlc_cipher_info.cipher_flag)
              {
                tb_cipher_ptr->ciphering_control = TRUE;

                tb_cipher_ptr->ciphering_algo = rlc_cipher_info.ciphering_algo;
				
                tb_cipher_ptr->ciphering_key_id = (uint8)rlc_cipher_info.cipher_key_id;
                tb_cipher_ptr->ciphering_count = rlc_cipher_info.count_c;
                tb_cipher_ptr->ciphering_radio_bearerid = (uint8)rlc_cipher_info.rb_id;
                tb_cipher_ptr->ciphering_bit_offset = rlc_cipher_info.cipher_offset;
              }
            }
#ifdef FEATURE_SRB5_SUPPORT
          } //UE_LOGCHAN_TM_DCCH_DL
#endif /* FEATURE_SRB5_SUPPORT */
        }
        /* Save RLC ID in Transport Block info table */
        tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_id + i);
        /*If the PDU decoding succeds then populate the TB strcuture*/
        if (status == TRUE)
        {
        
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
          /* Constant Loopback Delay has to be maintained for DCH type transport channel
          and when the loopback test mode is either of type 1 or 2 */
          if ((dlcptr->chan_type==UE_LOGCHAN_DTCH) &&
             (((dlcptr->rlc_mode == UE_MODE_TRANSPARENT) &&(tdsloopback_test_mode != TDSMAC_LOOPBACK_NA))
             || (((dlcptr->rlc_mode != UE_MODE_TRANSPARENT))&&(tdsloopback_test_mode == TDSMAC_LOOPBACK_MODE_2))))
          {
            /*---------------------------------------------------------------------------------------
            register the downlink CFN when the transport block was received in the
            downlink. This block would be transmitted in the uplink after applying
            the constant delay. If this cfn is already registered in the array then do not
            re-enter it. If tdscfn_marked is set to TRUE then this cfn exists in the database already
            ---------------------------------------------------------------------------------------*/
            if (tdscfn_marked == FALSE)
            {
              tdslb_delay_type.dl_cfn[tdslb_delay_type.cur_idx] = current_tbs_cfn;
              tdslb_delay_type.cur_idx++;
              tdslb_delay_type.cur_idx = tdslb_delay_type.cur_idx % TDSMAX_NUM_CFN;
              TDSMAC_MSG3(MSG_LEGACY_HIGH,"cur_dl_cfn %d ntb %d rlc_id %d", current_tbs_cfn,num_tb,rlc_hdr_info.rlc_id);
              tdscfn_marked = TRUE;
            }
          }/*lint !e613*/
#endif /* fetaure_tc_constant_delay */
          
          /* Set other header parameters of TB */
          tb_hdr_ptr->tb_id = (uint8)(tb_id + i); /*lint !e613*/
          tb_hdr_ptr->valid = TRUE; /*lint !e613*/
          tb_hdr_ptr->num_bit_offset_trblk = rlc_pdu_offset; /*lint !e613*/

          tdsdl_tb_info_ptr->valid = TRUE;
          tdsdl_tb_info_ptr->rlc_id = dlcptr->rlc_id;
        }
        else
        {
           /*If the PDU hdr decoding fails mark the TB hdr as TDSINVALID*/
           tb_hdr_ptr->valid = FALSE;
           tdsdl_tb_info_ptr->valid = FALSE;
        }
      } /* If DCCH/DTCH is enabled */
    } /* IF the TB parameters are valid */
  } /* For all the transport blocks */
}/* tdsprocess_dl_dch_tbs_hdr */

/*===============================================================================================
FUNCTION NAME tdsmac_dl_check_dch_trch

DESCRIPTION   This function checks if given rlc-id and rb-id pair map to a DL-DCH transport channel
===============================================================================================*/
log_tdscdma_trch_type tdsmac_dl_check_dch_trch(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;
  int i, j;

  for(i = 0; i < tdsnum_active_dl_dchs; i++)
  {
    for(j = 0; j < tdsdl_dch_cfg_info[i].num_dlc; j++)
    {
      if((tdsdl_dch_cfg_info[i].dlcinfo[j].enable == TRUE) &&
         (tdsdl_dch_cfg_info[i].dlcinfo[j].rlc_id == rlc_id) &&
         (tdsdl_dch_cfg_info[i].dlcinfo[j].rb_id == rb_id))
      {
        return_trch_val = TDSCDMA_TRCH_DCH;
        break;
      }
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"function mac_dl_check_dch_trch returning %d",return_trch_val);
  return return_trch_val;
}