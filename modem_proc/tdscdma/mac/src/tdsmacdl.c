/*========================================================================================

                D O W N L I N K   M A C

GENERAL DESCRIPTION

  This module contains the declaration of the data types and functions
  needed to process DL transport blocks

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


========================================================================================*/

/*===========================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdl.c_v   1.6   11 Jun 2002 17:43:38   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdl.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   --------------------------------------------------------------------------
10/24/10  guo   Initial release Upmerged WCDMA to TDS

===========================================================================================*/

/*===========================================================================================
                     INCLUDE FILES
===========================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "dsm.h"
#include  "task.h"
#include  "tdsmacrrcif.h"
#include  "tdsmacrlcif.h"
#include  "tdsl1sapcommon.h"
#include  "tdsmathutil.h"
#include  "tdsl1macdata.h"
#include  "tdsl2dltask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacdl.h"
#include  "tdsmacbch.h"
#include  "tdsmacpch.h"
#include  "tdsmacfach.h"
#include  "tdsmacdldch.h"
#include  "tdsmacdlhs.h"
#include  "tdsmacdlsubtask.h"
#include  "tdsl2ultask.h"

#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
 #include "tdsmacdllog.h"
#endif

  #include "tdsmaculdl.h"

#include "tdsmacdlhs.h"
#include "tdsmachstask.h"
#include "tdsmachslog.h"
#include "err.h"

#include "tdsl1macif.h"

#include "mcaltds_dec.h"
#include "tdsl1m_rcinit.h"

/*===========================================================================================
                            DATA DECLARATIONS
===========================================================================================*/

/* Loopback delay MAX possible TTIs as per 25.321spec */
#if (defined FEATURE_TDSCDMA_TC_CONSTANT_DELAY)
extern tdslb_delay_struct_type tdslb_delay_type;
extern tdsmac_loop_back_mode_e_type tdsloopback_test_mode;
#endif

/* BCH Internal Configuration Database */
tdsbch_config_info_type    tdsbch_cfg_info[TDSUE_MAX_CCTRCH];
tdspch_config_info_type    tdspch_cfg_info[TDSUE_MAX_CCTRCH];
tdsdl_dch_config_info_type   tdsdl_dch_cfg_info[TDSUE_MAX_TRCH];
tdsfach_config_info_type   tdsfach_cfg_info[TDSUE_MAX_TRCH];

/* Transport Channel ID to Index Mapping Table */
tdstrch_id_to_idx_map_type   tdsdl_dch_id_to_idx[MAX_TRCH];
tdstrch_id_to_idx_map_type   tdsfach_id_to_idx[2*MAX_TRCH];

/* RLC DL Frame data information */
tdsrlc_dl_frame_data_type      tdsrlc_data_frame;

/* Dowlink transport block information table */
tdsdl_tb_info_type       tdsdl_tb_info[TDSUE_MAX_DL_RLC_PDUS];

/* DL MAC HFN value */
uint32      tdsdl_hfn;

/* TRUE indicates that DL MAC CRNTI is valid */
/* FALSE otherwise */
boolean     tdsdl_mac_crnti_valid = FALSE;

/* TRUE indicates that DL MAC URNTI is valid */
/* FALSE otherwise */
boolean     tdsdl_mac_urnti_valid = FALSE;

/* DL MAC CRNTI value */
uint16      tdsdl_mac_crnti;

/* DL MAC URNTI value */
uint32      tdsdl_mac_urnti;

/* MAC dl config information */
tdsmac_dl_config_type  tdsdl_cfginfo;

/* TRUE implies MAC received a new configuration parameters from RRC and waiting    */
/* L1 signal to actually configure. FALSE implies there are no pending configuration*/
/* requests now */
boolean tdsdl_configuration_pending=FALSE;



extern uint8 tdsprev_print_cfn;
extern tdsmac_ciphering_table tdscfn_ciphering_table[256];
/* Ordered ciphering config data */
tdsmac_cipher_config_type    tdscur_dl_cipher_info[TDSMAX_CN_DOMAIN];
tdsmac_cipher_config_type    tdsnew_dl_cipher_info[TDSMAX_CN_DOMAIN];


#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
/*-----------------------------------------------------------------
Mark the cfn at which some data on a DCH transport channel is
received which needs to be looped back.So if there are transport blocks
received on multiple tranport channels in the same tti then note
the cfn only once. This flag will ensure that the cfn is marked only
once.
-----------------------------------------------------------------*/
boolean     tdscfn_marked = FALSE;
#endif

extern tdsmac_hs_dl_state_type tdsmac_hs_dl_curr_state, tdsmac_hs_dl_new_state;
extern tdsmac_hs_dl_config_req_type tdsmac_hs_dl_config;
extern q_type tdsmac_partial_tsn_q[TDSMAC_HS_DL_MAX_QUEUES];



extern boolean tdsmac_hs_reconfig_in_progress;
extern rex_crit_sect_type  tdsmac_hs_cfg_mutex;

extern rex_crit_sect_type tdsmac_hs_tsn_flush_mutex;

extern rex_crit_sect_type  tdsmac_ehs_recfg_mutex;

extern mac_hs_e_type tdsmac_hs_or_ehs;
 
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
 /** Information for all RLC PDUs that can be constructed out of
  *  multiple partial PDUs. */
 extern tdsl1_dec_rlc_pdu_concatenate_info_struct_type tdsmac_hs_dl_rlc_pdu_concatenate_info;
#endif  /* defined(FEATURE_TDSCDMA_HSDPA_MAC_EHS)*/



/*---------------------------------------------------------------------------
 *   Flag used to determine the test is related to GCF testing or not.
 *-------------------------------------------------------------------------*/
boolean         tdsmac_gcf_test_flag = FALSE;

/*============================================================================================
 FUNCTION NAME  tdsinit_dl_mac()

 DESCRIPTION  This function initializes the variables needed for ciphering

============================================================================================*/
void tdsinit_dl_mac(void)
{
  uint8 i;

  for (i=0;i<TDSMAX_CN_DOMAIN;i++)
  {
    /* Clear the memory */
    tdscur_dl_cipher_info[i].cipher_enable = FALSE;
    tdsnew_dl_cipher_info[i].cipher_enable = FALSE;
    tdscur_dl_cipher_info[i].key_idx = 0xFF;
    tdsnew_dl_cipher_info[i].key_idx = 0xFF;

  }

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  /* Initialize the first_tb boolean to TRUE so that UL MAC would use this
  flag to calculate the delay and subsequently this constant delay would
  be maintained for transmitting any block in the UL */
  tdslb_delay_type.first_tb = TRUE;
  tdslb_delay_type.start_idx = 0;
  tdslb_delay_type.cur_idx = 0;
#endif


}

/*===========================================================================================
 FUNCTION NAME  tdsproc_dl_cipher_info

 DESCRIPTION  This function processes the Cipher information received from RRC.

============================================================================================*/
void    tdsproc_dl_cipher_info
  (
// Pointer to DL MAC Ciphering Config Info
  tdsmac_cipher_config_type   cipherptr[]
  )
{
  uint8  i;

  if (cipherptr==NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"DL Cipher ptr is NULL");

  }
  else
  {

    for (i=0; i<TDSMAX_CN_DOMAIN ; i++)
    {
      if (cipherptr[i].cipher_enable)
      {
        /*-----------------------------------------------------------------------------
        If the ciphering was previously diabled and if the ciphering has been enabled
        now, then save the ciphering parameters given by RRC.
        Check if the ciphering parameters have changed. MAC knows if the parameters
        have changed by comparing the new key index with the one already exisiting
        in MAC. If the parameters have changed, then copy them to tdsnew_ul_cipher_info
        and switch to the new configuration at ciphering activation time which is
        provided by the CFN number.
        ---------------------------------------------------------------------------*/
        if (cipherptr[i].key_idx != tdscur_dl_cipher_info[i].key_idx)
        {
          // Copy the new ciphering parametrs received.
          //memcpy(&tdsnew_dl_cipher_info[i],&cipherptr[i],sizeof(tdsmac_cipher_config_type));
          memscpy(&tdsnew_dl_cipher_info[i],sizeof(tdsmac_cipher_config_type),&cipherptr[i],sizeof(tdsmac_cipher_config_type));

          // Indicate that there is a new config to which MAC has to switch to at
          // ciphering activation time.
          if (!tdscur_dl_cipher_info[i].cipher_enable)
            //memcpy(&tdscur_dl_cipher_info[i],&cipherptr[i],sizeof(tdsmac_cipher_config_type));
          memscpy(&tdscur_dl_cipher_info[i],sizeof(tdsmac_cipher_config_type),&cipherptr[i],sizeof(tdsmac_cipher_config_type));
        } // ciphering parameters have changed

      }// ciphering is enabled
      else
      {

        /* Clear the memory */
        tdscur_dl_cipher_info[i].cipher_enable = FALSE;
        tdsnew_dl_cipher_info[i].cipher_enable = FALSE;
        tdscur_dl_cipher_info[i].key_idx = 0xFF;
        tdsnew_dl_cipher_info[i].key_idx = 0xFF;

        if (i==0)
        {
          memset(tdscfn_ciphering_table, 0x00, 256 * sizeof(tdsmac_ciphering_table));
        }
      }
    }
  }
}


/*============================================================================================
 FUNCTION NAME  tdsproc_dl_rnti_info

 DESCRIPTION  This function processes the RNTI information received from RRC.

============================================================================================*/
void    tdsproc_dl_rnti_info
  (
/* Pointer to DL RNTI Config Info */
  tdsmac_rnti_config_type   *rntiptr
  )
{
  /* Process CRNTI info based on RNTI valid flag */
  switch (rntiptr->rnti_valid)
  {

  /* C-RNTI is valid */
  case TDSCRNTI_VALID:
    tdsdl_mac_crnti_valid = TRUE;
    tdsdl_mac_urnti_valid = FALSE;
    tdsdl_mac_crnti = rntiptr->crnti;
    TDSMAC_MSG1(MSG_LEGACY_LOW,"C-RNTI = %x",tdsdl_mac_crnti);
    break;

    /* U-RNTI is valid */
  case TDSURNTI_VALID:
    tdsdl_mac_urnti_valid = TRUE;
    tdsdl_mac_crnti_valid = FALSE;
    tdsdl_mac_urnti = rntiptr->urnti;
    TDSMAC_MSG1(MSG_LEGACY_LOW,"U-RNTI = %x",tdsdl_mac_urnti);
    break;

    /* C-RNTI and U-RNTI are valid */
  case TDSBOTH_VALID:
    tdsdl_mac_urnti_valid = TRUE;
    tdsdl_mac_crnti_valid = TRUE;
    tdsdl_mac_crnti = rntiptr->crnti;
    tdsdl_mac_urnti = rntiptr->urnti;
    TDSMAC_MSG1(MSG_LEGACY_LOW,"C-RNTI = %x",tdsdl_mac_crnti);
    TDSMAC_MSG1(MSG_LEGACY_LOW,"U-RNTI = %x",tdsdl_mac_urnti);
    break;

    /* No valid RNTI */
  case TDSNO_RNTI_VALID:
    tdsdl_mac_urnti_valid = FALSE;
    tdsdl_mac_crnti_valid = FALSE;
    TDSMAC_MSG0(MSG_LEGACY_LOW,"No valid RNTI");
    break;

    /* Invalid RNTI valid flag */
  default:
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Invalid DL RNTI Config");
    break;
  }
}


/*=============================================================================================
FUNCTION NAME tdsconfig_dl_mac

DESCRIPTION   This function processes DL MAC Configaration Data received from RRC

=============================================================================================*/
void    tdsconfig_dl_mac
  (
/* Pointer to DL MAC Config Data */
  tdsmac_dl_config_type *cfgptr
  )

{

  if ( tdsmac_hs_dl_config.operation.save_non_hs_params)
  {

    TDSL2_ACQ_DL_LOCK();

    /* Process RNTI information */
    tdsproc_dl_rnti_info (&cfgptr->rnti_info);

    /* Process Ciphering information */
    tdsproc_dl_cipher_info (cfgptr->cipher_info);

    /* Process BCH configuration data */
    tdsprocess_bch_config (cfgptr->num_bchs, &cfgptr->bch_info[0]);

    /* Process PCH configuration data */
    tdsprocess_pch_config (cfgptr->num_pchs,&cfgptr->pch_info[0]);


    /* Process FACH configuration data */
    tdsprocess_fach_config (cfgptr->num_fachs,&cfgptr->fach_info[0]);
    /* Process DCH configuration data */
    tdsprocess_dl_dch_config (cfgptr->num_dchs,&cfgptr->dch_info[0]);

   TDSL2_ACQ_DL_UNLOCK();
#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
    /* Send MAC Configuration Data to QXDM*/
    tdsmac_dl_send_cfg_log_packet(cfgptr);
#endif


    TDSL2_ACQ_DL_LOCK();



    tdsmac_hs_dl_config.operation.save_non_hs_params = FALSE;

    TDSL2_ACQ_DL_UNLOCK();

  }


  if (((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5)) &&
     ( tdsmac_hs_dl_config.operation.perform_hs_reset ||
        tdsmac_hs_dl_config.operation.save_hs_params ))
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Configuring HS DSCH");

    /*----------------------------------------------------------------
     * There will be a context switch inside the function below where
     * L2-DL task signals MAC-HS task to pick up MAC-HS configuration.
     * For details discussion, please see the MAC-HS design document.
    ----------------------------------------------------------------*/
    REX_ISR_LOCK(&tdsmac_hs_cfg_mutex);
    tdsprocess_dl_hs_dsch_config(cfgptr);
    REX_ISR_UNLOCK(&tdsmac_hs_cfg_mutex);

    /*----------------------------------------------------------------
     * L2-DL task procesing will resume here
    ----------------------------------------------------------------*/

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if (log_status(TDSCDMA_MAC_HS_CONFIG_LOG_PACKET))
    {
      tdsmac_ehs_dl_send_hs_config_log_packet(cfgptr);
    }
#else
    if (log_status(TDSCDMA_MAC_HS_CONFIG_LOG_PACKET))
    {
      tdsmac_hs_dl_send_hs_config_log_packet(cfgptr);
    }
#endif /* ((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)) ) */
#endif
  }

}


 /*============================================================================================
 FUNCTION NAME  tdsl1_phy_dl_trblk_hs_data_ind

 DESCRIPTION  This function processes DL HS transport blocks received from 
              PHY layer as part of PP DL HW handling

 CONTEXT    Called by PHY layer MDM in interrupt context 
            (seperate from regular Data ISR which is only for R99 and non PP HS call)
=============================================================================================*/
void tdsl1_phy_dl_trblk_hs_data_ind(tdsl1_dl_hs_tti_enum_type tti_type)
{
  uint8   mac_hs_num_tsn_rcvd = 0; 

  if ((tti_type == TDS_MACDL_HS_CURR_TTI) || tti_type == TDS_MACDL_HS_CURR_PREV_TTI)
  {
    if (q_cnt(&mcaltds_dec_tsn_info_q) > 0)
    {
      if ((tdsmac_hs_dl_curr_state != TDSMAC_HS_ACTIVE)
           && !tdsmac_hs_reconfig_in_progress
         )
      {
        TDSMAC_MSG1(MSG_LEGACY_ERROR,"MAC_ERR: HS_DSCH not active; HS State = %d",tdsmac_hs_dl_curr_state);
      }
      else
      {
        REX_ISR_LOCK(&tdsmac_hs_cfg_mutex);
        tdsprocess_dl_hs_enh_dsch_tbs_data(&mac_hs_num_tsn_rcvd);
        REX_ISR_UNLOCK(&tdsmac_hs_cfg_mutex);
      }
    }
    /*
       If the TTI is current and if there are TSN which needs to be processed signal MAC HS task
    */
    if ((tti_type == TDS_MACDL_HS_CURR_TTI) && (mac_hs_num_tsn_rcvd > 0))
    {
      (void) rex_set_sigs( tds_mac_hs_dl_get_tcb(), TDSMAC_HS_DL_DATA_IND_SIG);
    }
  }
    
  /*
   * If any useful TSN rcvd or there are any P-to-C present, 
   * Signal the mac_dl_hs task to pick up the processing
   */
  if ((tti_type == TDS_MACDL_HS_PREV_TTI) || tti_type == TDS_MACDL_HS_CURR_PREV_TTI)
  {
    if (
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        (q_cnt(&mcaltds_dec_concatenated_pdu_tsn_q) > 0) ||
#endif
        (mac_hs_num_tsn_rcvd > 0))
    {
     #ifdef FEATURE_TDSCDMA_A2_HW_DL
      #ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus = 0;
     #endif
      #endif
      (void) rex_set_sigs( tds_mac_hs_dl_get_tcb(), TDSMAC_HS_DL_DATA_IND_SIG);
    }
  }
}

/*============================================================================================
 FUNCTION NAME  l1_phy_dl_flush_tsn

 DESCRIPTION  This function will flush the TSNs based on L1 request

 CONTEXT    Called by PHY layer in interrupt context
=============================================================================================*/
void  tdsl1_phy_dl_flush_tsn(void)
{
  tdsl1_dec_hs_tsn_info_type *tsn_info_ptr;
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  tdsl1_dec_concatenated_pdu_tsn_type *concatenated_pdu_tsn_info_ptr = NULL;
  int i;
#endif
  
  /*Lock the mutex*/
  TDSL2_MUTEX_LOCK(&tdsmac_hs_tsn_flush_mutex);
  
  /*Go over the dec_tsn_info_q*/
#ifdef FEATURE_TDSCDMA_L1_ZI_OPT
  while ((tsn_info_ptr = (tdsl1_dec_hs_tsn_info_type *) q_get((q_type *)mcaltds_dec_tsn_info_buf)) != NULL)
#else
  while ((tsn_info_ptr = (tdsl1_dec_hs_tsn_info_type *) q_get((q_type *)&mcaltds_dec_tsn_info_buf)) != NULL)
#endif
  {
    tdsmac_hs_dl_discard_tsn(tsn_info_ptr);
    tdsmac_hs_enqueue_free_tsn(tsn_info_ptr);
  }
  
  TDSL2_MUTEX_UNLOCK(&tdsmac_hs_tsn_flush_mutex);

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  REX_ISR_LOCK(&tdsmac_ehs_recfg_mutex);
  for (i = 0; i < TDSMAC_HS_DL_MAX_QUEUES; i++)
  {
    while ((tsn_info_ptr = (tdsl1_dec_hs_tsn_info_type *) q_get(&tdsmac_partial_tsn_q[i])) != NULL)
    {
      tdsmac_hs_dl_discard_tsn(tsn_info_ptr);
      tdsmac_hs_enqueue_free_tsn(tsn_info_ptr);
    }
  }
  /*Flush dec concantanated q*/
  while ((concatenated_pdu_tsn_info_ptr = 
              (tdsl1_dec_concatenated_pdu_tsn_type *) q_get(&mcaltds_dec_concatenated_pdu_tsn_q)) != NULL)
  {
    concatenated_pdu_tsn_info_ptr->tsn_info_buf = NULL;
    q_put(&mcaltds_dec_concatenated_pdu_tsn_free_q, &(concatenated_pdu_tsn_info_ptr->link));
  }
  REX_ISR_UNLOCK(&tdsmac_ehs_recfg_mutex);
#endif

  /*Flush reordering q*/
  TDSL2_ACQ_DL_LOCK();
  tdsmac_hs_tsn_empty_recovery();
  TDSL2_ACQ_DL_UNLOCK();
  /*Flush RLC tsn info q*/
  tdsrlc_enh_dl_tsn_flush_hs();
}

/*============================================================================================
 FUNCTION NAME  tdsl1_phy_dl_trblk_data_ind

 DESCRIPTION  This function processes DL transport blocks received from PHY layer

 CONTEXT    Called by PHY layer in interrupt context
=============================================================================================*/
void    tdsl1_phy_dl_trblk_data_ind
  (
/* Pointer to TB Data Information */
  tdsl1_dl_tb_data_struct_type *dl_tb_data_info
  )
{
  uint8   i,j;

  /* TB index */
  uint8   tb_index;

  uint8   mac_hs_num_tsn_rcvd = 0;


  tdsdl_tb_info_type   *tdsdl_tb_info_ptr;

  /* Pointer to Transport Block Set info */
  tdsl1_dl_tb_set_data_struct_type   *tbsptr = NULL;

  /* Pointer to Transport Block Data info */
  tdsl1_dl_tb_data_in_tbset_struct_type  *tbptr;

  /* Number of transport block sets */
  uint8   num_tb_set;

  uint16 length;
  boolean first = TRUE;
  tds_complete_pdu_type *complete_pdu_ptr;
  tdsrlc_enh_data_ind_type *rlc_enh_data_ind_ptr = NULL;
  boolean is_tm_mode = FALSE;
  uint8 num_tb = 0;
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  uint8 cfn_idx = 0;
#endif
  
  /* TB Data Info Pointer is NULL */
  if (dl_tb_data_info == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"TB Data info pointer is NULL");
  }
  /* Valid number of TB Data Blocks */
  else
  {
    /* Number of transport block sets */
    num_tb_set = dl_tb_data_info->num_tb_set;

    /* Process each TBS Data info */
    for (i=0 ;i <num_tb_set; i++)
    {

      /* Pointer to transport block set */
      tbsptr = &dl_tb_data_info->tb_set_data[i];

      /* TBS Data pointer is NULL */
      if (tbsptr == NULL) /*lint !e774*/
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"TBS%d Data pointer is NULL",i);
      }
      /* Invalid number of transport blocks */
      else if (num_tb + tbsptr->num_tb > TDSUE_OPT_MAX_DL_RLC_PDUS)
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d Data with inv number of TBs %d",i,tbsptr->num_tb);
      }
      /* Process Transport Blocks */
      else
      {
        for (j=0 ;j< tbsptr->num_tb; j++)
        {

          /* Pointer to Transport Block Data info */
          tbptr = &tbsptr->tb_data_info[j];
          /* Invalid TB Data info pointer */
          if (tbptr == NULL) /*lint !e774*/
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d TB%d pointer is NULL",i,j);
            continue;
          }

          /* Get internal TB Data info pointer */
          tb_index = (uint8) (tbsptr->tb_data_info[j].tb_id - TDSMAC_DL_MIN_TB_ID);
          tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_index);

          /* TB is invalid */
          if (tdsdl_tb_info_ptr->valid == FALSE)
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d TB%d is not valid",i,j);
          }
          /* Invalid PDU size */
          else if (tbptr->data_size == 0)
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d PDU%d size is zero",i,j);
          }
          /* Invalid PDU pointer */
          else if (tbptr->tb_data_buffer == NULL)
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d PDU%d pointer is NULL",i,i);
          }
          else
          {
            if (first == TRUE)
            {
              /* Get free data entry to update the RLC PDU info */
              rlc_enh_data_ind_ptr = tdsrlc_enh_get_free_data_q_entry     ();
              if (rlc_enh_data_ind_ptr != NULL)
              {
                rlc_enh_data_ind_ptr->num_complete_pdus = 0;
              }
              else
              {
                TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC_ERR: Ran out of rlc_enh_data_queue entries");
                continue;
              }
              first = FALSE;
            }

            complete_pdu_ptr = &(rlc_enh_data_ind_ptr->TDS_COMPLETE_PDU[rlc_enh_data_ind_ptr->num_complete_pdus]);

            complete_pdu_ptr->rlc_id = tdsdl_tb_info_ptr->rlc_id;
            complete_pdu_ptr->rlc_hdr_data =
              (uint32) ((tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[0]<<8) |
              (uint32) (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[1];

            length = tdsrlc_enh_calc_rlc_hdr_length       (tdsdl_tb_info_ptr->rlc_id,
                         (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[0], &is_tm_mode);
            if (length > 0)
            {
              (tbsptr->tb_data_info[j].tb_data_buffer)->used -= length;
              (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr += length;
            }
            /*
             * Assuming that, In case of non-HS all the RLC PDU from Layer1
             * will come as "one PDU - one DSM" - No Chaining expected
             */
            complete_pdu_ptr->iovec.ptr = tbsptr->tb_data_info[j].tb_data_buffer;
            complete_pdu_ptr->iovec.offset = 0;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
            if ((tdsloopback_test_mode != TDSMAC_LOOPBACK_NA) && 
                (is_tm_mode == TRUE))
            {
              /* Set app_ptr to DL-CFN only for LB-2 case */
              if (tdslb_delay_type.cur_idx == 0)
              {
                cfn_idx = TDSMAX_NUM_CFN - 1;
              }
              else
              {
                cfn_idx = tdslb_delay_type.cur_idx - 1;
              }
              complete_pdu_ptr->iovec.ptr->app_ptr = NULL;
              /*Using memcpy to avoid compiler waring on 8200*/
              /*memcpy(&complete_pdu_ptr->iovec.ptr->app_ptr,
                &tdslb_delay_type.dl_cfn[cfn_idx],sizeof(uint16));*/

              memscpy(&complete_pdu_ptr->iovec.ptr->app_ptr,sizeof(uint16),
                &tdslb_delay_type.dl_cfn[cfn_idx],sizeof(uint16));
            }
#endif

            if (is_tm_mode == TRUE)
            {
              complete_pdu_ptr->iovec.ptr->app_field |= 0x1; /* for R99 path */
              complete_pdu_ptr->iovec.length = (uint16)((tbsptr->tb_data_info[j].data_size ));
              is_tm_mode = FALSE;
            }
            else
            {
              complete_pdu_ptr->iovec.ptr->app_field = 1; /* for R99 path */
              complete_pdu_ptr->iovec.length = (uint16)((tbsptr->tb_data_info[j].data_size >> 3) - length);
            }
            num_tb++;
            rlc_enh_data_ind_ptr->num_complete_pdus++;
          }
        }
      }
    } /* for loop */
  }

  if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) &&
      (q_cnt(&mcaltds_dec_tsn_info_q) > 0))
  {
    if ((tdsmac_hs_dl_curr_state != TDSMAC_HS_ACTIVE)
         && !tdsmac_hs_reconfig_in_progress
       )
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"HS_DSCH not active; HS State = %d",tdsmac_hs_dl_curr_state);
    }
    else
    {
      REX_ISR_LOCK(&tdsmac_hs_cfg_mutex);
      tdsprocess_dl_hs_enh_dsch_tbs_data(&mac_hs_num_tsn_rcvd);
      REX_ISR_UNLOCK(&tdsmac_hs_cfg_mutex);
    }
  }

  if (rlc_enh_data_ind_ptr != NULL)
  {
    if (rlc_enh_data_ind_ptr->num_complete_pdus > 0)
    {
      tdsrlc_enh_dl_pdu_data_ind           (rlc_enh_data_ind_ptr);
      }
    else
    {
      /* return the entry to free queue */
      tdsrlc_enh_add_to_free_data_q_entry  (rlc_enh_data_ind_ptr);
    }
  }


  if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) && 
      ((mac_hs_num_tsn_rcvd) 
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
      || (q_cnt(&mcaltds_dec_concatenated_pdu_tsn_q) > 0)
#endif
      ))
  {
    /*
     * Signal the mac_dl_hs task to pick up the processing
     */
    (void) rex_set_sigs( tds_mac_hs_dl_get_tcb(), TDSMAC_HS_DL_DATA_IND_SIG);
  }
}

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*============================================================================================
FUNCTION NAME   tdsl1_phy_dl_query_allowed_buf_size_for_p_to_c

DESCRIPTION   This function returns the buffersize used in L1 for 
              Partial to Complete PDU preparation

CONTEXT     L1 interrupt context
=============================================================================================*/
uint16 tdsdummy_buf_size = 25000;
uint16 tdsl1_phy_dl_query_allowed_buf_size_for_p_to_c()
{
  //uint16 tdsdummy_buf_size = 3000;
  return (tdsdummy_buf_size);
}
#endif /* ((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)) ) */

/*============================================================================================
FUNCTION NAME   tdsl1_phy_dl_trblk_header_ind

DESCRIPTION   This function decodes the header (includes MAC as well as RLC) and
        decides the ciphering parameters of each transport block.

CONTEXT     L1 interrupt context
=============================================================================================*/
void  tdsl1_phy_dl_trblk_header_ind
  (
/* Pointer to DL Transport Block Set Header Info received from Layer 1 */
  tdsl1_dl_tb_hdr_decode_struct_type   *hdrptr
  )
{
  uint8   i, j, num_tb_set, tb_id, num_tb;

  uint8   tsn_to_queue_map_idx = 0;

  /* Pointer to internal BCH info */
  tdsbch_config_info_type    *int_bch_ptr;

  /* Pointer to internal PCH info */
  tdspch_config_info_type    *int_pch_ptr;

  /* Pointer to internal FACH config info */
  tdsfach_config_info_type   *int_fach_ptr;

  /* Pointer to internal DCH config info */
  tdsdl_dch_config_info_type   *int_dch_ptr;

  /* Pointer to header info of transpport block set */
  tdsl1_dl_tb_set_hdr_struct_type    *tbs_hdr_ptr;
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  mac_hs_e_type int_hs_type = MAC_HS_TYPE_INVALID;
#endif 
  /* Transport Block Index = 0 */
  tb_id = 0;


  /* Header indication data pointer is NULL */
  if (hdrptr == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Header Indication Data pointer is NULL");
  }
  /* Invalid number of transport block sets */
  else if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) &&
           (hdrptr->num_tb_set > TDSMAC_HS_MAX_NUM_TB_SET_PER_10_MS_FRAME))
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of TB sets %d",hdrptr->num_tb_set);
  }
  else if ((((tdsl2_nv_rel_indicator == TDSL2_REL_INDICATOR_R4)) &&
            (hdrptr->num_tb_set > TDSUE_MAX_TRCH)))
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of TB sets %d",hdrptr->num_tb_set);
  }
  /* Process transport block set header information */
  else
  {
    /* Number of Transport Block Sets in Header Info */
    num_tb_set = hdrptr->num_tb_set;

    /*-----------------------------------------------------------------
    Mark the cfn at which some data on a DCH transport channel is
    received which needs to be looped back.So if there are transport blocks
    received on multiple tranport channels in the same tti then note
    the cfn only once. This flag will ensure that the cfn is marked only
    once. Initialize the flag to FALSE.
    -----------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
    tdscfn_marked = FALSE;
#endif

    /* Processes each transport block set (i.e.transport channel) */
    for (i=0;(i < num_tb_set); i++)
    {
      /* Get pointer to header info of transport channel */
      tbs_hdr_ptr = &hdrptr->tb_set_hdr[i];

      /* TBS Header Info Pointer is NULL */
      if (tbs_hdr_ptr == NULL) /*lint !e774*/
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"TBS%d Header Pointer is NULL",i);
      }
      else if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) &&
               ((tbs_hdr_ptr->trch_ref_type != TDSL1_DL_TRCH_HS_DSCH_TYPE) 
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
               && (tbs_hdr_ptr->trch_ref_type != TDSL1_DL_TRCH_EHS_DSCH_TYPE)
#endif
               )
               &&
               ((tb_id + tbs_hdr_ptr->num_tb) >= TDSUE_MAX_DL_RLC_PDUS ))
      {
         /* Mark each TB of the set as invalid */
         for (j=0 ; j < tbs_hdr_ptr->num_tb; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
         TDSMAC_MSG3(MSG_LEGACY_HIGH,"TrCh%d with invalid number of TBs %d %d",
                   tbs_hdr_ptr->info.non_hs.trch_id, tb_id, tbs_hdr_ptr->num_tb);
      }
      else if (((tdsl2_nv_rel_indicator == TDSL2_REL_INDICATOR_R4)) &&
               ((tb_id + tbs_hdr_ptr->num_tb) >= TDSUE_MAX_DL_RLC_PDUS))
      {
        /* Mark each TB of the set as invalid */
        for (j=0 ; j < tbs_hdr_ptr->num_tb; j++)
          tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"TrCh%d with invalid number of TBs %d %d",
                  tbs_hdr_ptr->info.non_hs.trch_id, tb_id, tbs_hdr_ptr->num_tb);
      }

      /* Process TBS Header Info */
      else
      {
        /* Get number of transport blocks */
        num_tb = tbs_hdr_ptr->num_tb;

        if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5))
        {

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
          TDSL2_MUTEX_LOCK(&tdsmac_hs_cfg_mutex);
          int_hs_type = tdsmac_hs_or_ehs;
          TDSL2_MUTEX_UNLOCK(&tdsmac_hs_cfg_mutex);
          if ((tbs_hdr_ptr->trch_ref_type == TDSL1_DL_TRCH_EHS_DSCH_TYPE) &&
              (int_hs_type == MAC_EHS))
          {

            /* Maximum of 26 pdus per mac-ehs pdu */
            if (TDSMAC_EHS_DL_NUM_TB_PER_TSN_IS_INVALID(tbs_hdr_ptr->info.ehs.num_reorder_sdu))
            {
              TDSMAC_MSG2(MSG_LEGACY_ERROR,"MAC_ERR: TBS Id %d - Invalid num_reorder_sdu %d rcvd",i,tbs_hdr_ptr->info.ehs.num_reorder_sdu);
              tsn_to_queue_map_idx++;
              continue;
            }
            /* Mark each TB of the set as invalid */
            for (j=0 ; j <tbs_hdr_ptr->info.ehs.num_reorder_sdu; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;

          }
          else
#endif 
          {
            TDSL2_MUTEX_LOCK(&tdsmac_hs_cfg_mutex);
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
            int_hs_type = tdsmac_hs_or_ehs;
#endif
            TDSL2_MUTEX_UNLOCK(&tdsmac_hs_cfg_mutex);
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
            if((tbs_hdr_ptr->trch_ref_type == TDSL1_DL_TRCH_HS_DSCH_TYPE) && (int_hs_type == MAC_HS))
#else
			if(tbs_hdr_ptr->trch_ref_type == TDSL1_DL_TRCH_HS_DSCH_TYPE)                
#endif
            {
              /*
               * Maximum of 70 mac-d pdus per mac-hs pdu
               */
              if (TDSMAC_HS_DL_NUM_TB_PER_TSN_IS_INVALID(num_tb))
              {
                TDSMAC_MSG2(MSG_LEGACY_ERROR,"TBS Id %d; Invalid num_tb %d rcvd",i,num_tb);
                tsn_to_queue_map_idx++;
                continue;
              }
            }
            /* Mark each TB of the set as invalid */
            for (j=0 ; j <num_tb; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
          }
        }
        else
        {
          /* Mark each TB of the set as invalid */
          for (j=0 ; j <num_tb; j++)
            tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
        }


        /* Process header info based on transport channel reference type */
        switch (tbs_hdr_ptr->trch_ref_type)
        {

        /* BCH Type TrCh */
        case TDSL1_DL_TRCH_SBCH_TYPE:
        case TDSL1_DL_TRCH_NBCH_TYPE:
          /* CCTrCh is invalid */
          if (TDSDL_CCTRCH_ID_IS_INVALID(tbs_hdr_ptr->cctrch_id))
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"BCH with invalid CCTrCh ID",tbs_hdr_ptr->cctrch_id);
          }
          /* Valid CCTrCh ID */
          else
          {
            /* Pointer to BCH config table entry */
            int_bch_ptr = TDSGET_BCH_INFO_PTR(tbs_hdr_ptr->cctrch_id);

            /* BCH is not enabled in MAC */
            if (int_bch_ptr->bch_enable != TRUE)
            {
              TDSMAC_MSG1(MSG_LEGACY_HIGH,"BCH%d not enabled",tbs_hdr_ptr->cctrch_id);
            }
            else
            {
              tdsprocess_bch_tbs_hdr (tb_id, int_bch_ptr, tbs_hdr_ptr);
            }
          }
          break;

          /* PCH Type TrCh */
        case TDSL1_DL_TRCH_PCH_TYPE:
          /* CCTrCh is invalid */
          if (TDSDL_CCTRCH_ID_IS_INVALID(tbs_hdr_ptr->cctrch_id))
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"PCH with invalid CCTrCh ID",tbs_hdr_ptr->cctrch_id);
          }
          /* Valid CCTrCh ID */
          else
          {
            /* Pointer to PCH config table entry */
            int_pch_ptr = TDSGET_PCH_INFO_PTR(tbs_hdr_ptr->cctrch_id);

            /* PCH is not enabled in MAC */
            if (int_pch_ptr->pch_enable != TRUE)
            {
              TDSMAC_MSG1(MSG_LEGACY_HIGH,"PCH%d not enabled",tbs_hdr_ptr->cctrch_id);
            }
            else
            {
              tdsprocess_pch_tbs_hdr (tb_id, int_pch_ptr, tbs_hdr_ptr);
            }
          }
          break;

          /* FACH type TrCh */
        case TDSL1_DL_TRCH_FACH_TYPE:
           /* Transport Channel ID is invalid */
           if (TDSDL_TRCH_ID_IS_INVALID(tbs_hdr_ptr->info.non_hs.trch_id))
           {
             TDSMAC_MSG2(MSG_LEGACY_HIGH,"FACH%d with invalid TrCh ID %d",i,tbs_hdr_ptr->info.non_hs.trch_id);
           }
           else
           {
             /* Get transport chanel config info pointer */
             int_fach_ptr = TDSGET_FACH_INFO_PTR(tbs_hdr_ptr->info.non_hs.trch_id);

             /* FACH is not enabled */
             if (TDSFACH_IS_ENABLED(tbs_hdr_ptr->info.non_hs.trch_id) == FALSE)
             {
               TDSMAC_MSG1(MSG_LEGACY_HIGH,"FACH%d not enabled",tbs_hdr_ptr->info.non_hs.trch_id);
             }
             /* Process FACH TBS Header Information */
             else
             {
               tdsprocess_fach_tbs_hdr (tb_id, int_fach_ptr, tbs_hdr_ptr);
             }
           }

          break;

          /* DCH type TrCh */
        case TDSL1_DL_TRCH_DCH_TYPE:
           /* Transport Channel ID is invalid */
           if (TDSDL_TRCH_ID_IS_INVALID(tbs_hdr_ptr->info.non_hs.trch_id))
           {
             TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH%d with invalid TrCh ID",tbs_hdr_ptr->info.non_hs.trch_id);
           }
           else
           {
              /* Get transport chanel config info pointer */
              int_dch_ptr = TDSGET_DL_DCH_INFO_PTR(tbs_hdr_ptr->info.non_hs.trch_id);

              /* DCH is not enabled */
              if (TDSDL_DCH_IS_ENABLED(tbs_hdr_ptr->info.non_hs.trch_id) == FALSE)
              {
                 TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH%d not enabled",tbs_hdr_ptr->info.non_hs.trch_id);
              }
              /* Process DL DCH TBS Header Information */
              else
              {
                 tdsprocess_dl_dch_tbs_hdr (tb_id, int_dch_ptr, tbs_hdr_ptr);
              }
           }
          break;


          /* HS-DSCH type TrCh */
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
        case TDSL1_DL_TRCH_EHS_DSCH_TYPE:
#endif           
        case TDSL1_DL_TRCH_HS_DSCH_TYPE:

           if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5))
           {
             /* DCH is not enabled */
             if ((tdsmac_hs_dl_curr_state != TDSMAC_HS_ACTIVE )
                 && !tdsmac_hs_reconfig_in_progress
                 )
             {
               TDSMAC_MSG1(MSG_LEGACY_HIGH,"HS_DSCH not active; HS State = %d",tdsmac_hs_dl_curr_state);
             }
             /* Process DL DCH TBS Header Information */
             else
             {
               if (tsn_to_queue_map_idx >= TDSMAC_HS_DL_MAX_TSN_PER_10_MS_FRAME)
               {
                  TDSMAC_MSG1(MSG_LEGACY_ERROR,"Rcvd %d TSNs in 10 ms frame",tsn_to_queue_map_idx);
               }
               else
               {
                 TDSL2_MUTEX_LOCK(&tdsmac_hs_cfg_mutex);
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
                 if (tdsmac_hs_or_ehs == MAC_EHS)
                 {
                   tdsprocess_dl_ehs_dsch_tbs_hdr(i, &tsn_to_queue_map_idx, tbs_hdr_ptr);
                   tsn_to_queue_map_idx++;
                 }
                 else if (tdsmac_hs_or_ehs == MAC_HS)
#endif
                 {
                   tdsprocess_dl_hs_dsch_tbs_hdr (i, tsn_to_queue_map_idx, tbs_hdr_ptr);
                   tsn_to_queue_map_idx++;
                 }
                 TDSL2_MUTEX_UNLOCK(&tdsmac_hs_cfg_mutex);

               }
             }
           }
           break;

          /* Invalid transport channel type */
        default:
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"TBS%d:invalid TrCh type %d",i,tbs_hdr_ptr->trch_ref_type);
          break;
        }

        if ((tbs_hdr_ptr->trch_ref_type != TDSL1_DL_TRCH_HS_DSCH_TYPE)
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
            && (tbs_hdr_ptr->trch_ref_type != TDSL1_DL_TRCH_EHS_DSCH_TYPE)
#endif
           )
        {
           /* Update TB ID value */
           tb_id = (uint8)(tb_id + num_tb);
        }
      }
    }

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
 #ifndef FEATURE_TDSCDMA_A2_HW_DL
    TDSL2_MUTEX_LOCK(&tdsmac_hs_cfg_mutex);
    if ((tdsmac_hs_or_ehs == MAC_EHS) &&
        (tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus > 0) && 
        (tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus < 
           MCALTDS_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI))
    {
      /*Copy the p-t-c structure to L1*/
      REX_ISR_LOCK(&tdsmac_ehs_recfg_mutex);
      hdrptr->rlc_pdu_concatenate_info.num_complete_pdus =
                          tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus;
      /*memcpy((void *)hdrptr->rlc_pdu_concatenate_info.rlc_pdu_hdr_info, 
                    (void *)tdsmac_hs_dl_rlc_pdu_concatenate_info.rlc_pdu_hdr_info, 
                    sizeof(tdsl1_dl_partial_rlc_pdu_hdr_struct_type) * 
                    tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus);*/

	  memscpy((void *)hdrptr->rlc_pdu_concatenate_info.rlc_pdu_hdr_info,
                    sizeof(tdsl1_dl_partial_rlc_pdu_hdr_struct_type) * TDSL1_DEC_MAX_COMPLETE_PDU_FROM_PREV_TTI,
                    (void *)tdsmac_hs_dl_rlc_pdu_concatenate_info.rlc_pdu_hdr_info, 
                    sizeof(tdsl1_dl_partial_rlc_pdu_hdr_struct_type) * 
                    tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus);
     /*Reset the num complete PDUs so that we can MAC-hs can form p-t-c */
      tdsmac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus = 0;
     /*(void)memcpy((void *)hdrptr->rlc_pdu_concatenate_info.src_desc, 
                  (void *)tdsmac_hs_dl_rlc_pdu_concatenate_info.src_desc,
                   sizeof(tdsmac_hs_dl_rlc_pdu_concatenate_info.src_desc));*/

      memscpy((void *)hdrptr->rlc_pdu_concatenate_info.src_desc,
                  sizeof(hdrptr->rlc_pdu_concatenate_info.src_desc),
                  (void *)tdsmac_hs_dl_rlc_pdu_concatenate_info.src_desc,
                   sizeof(tdsmac_hs_dl_rlc_pdu_concatenate_info.src_desc));
      REX_ISR_UNLOCK(&tdsmac_ehs_recfg_mutex);

    }
	TDSL2_MUTEX_UNLOCK(&tdsmac_hs_cfg_mutex);
#endif/*FEATURE_TDSCDMA_A2_HW_DL*/
#endif /* ((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS))  */

#if defined(FEATURE_TDSCDMA_RLC_QXDM_LOGGING)
    /*-----------------------------------------------------------------------
      Set the TDSRLC_DL_LOG_CIPHER_PKT_SIG signal.
    -----------------------------------------------------------------------*/
    tdsrlc_dl_set_cipher_logging         ();
#endif
  }
}


/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_commands

 DESCRIPTION  Processes DL MAC commands and responses received from other tasks

==============================================================================================*/
void  tdsproc_dl_mac_commands
  (
/* Command ID */
  tdsl2_dl_cmd_enum_type   cmd_id,

/* Pointer to command data */
  tdsl2_dl_cmd_data_type   *cmd_data_ptr

  )
{
  /* Process the command data based on command ID */
  switch (cmd_id)
  {

  /* DL MAC Congiguration Request */
  case  TDSCMAC_DL_CONFIG_REQ:
    tdsl2_get_nv_rel_ver_opt_mask();
   tdsmac_hs_dl_validate_config_req(&cmd_data_ptr->mac_config_info);
 TDSL2_MSG3(MSG_LEGACY_MED,"MAC config validate, config hs params %d non hs params %d reset %d", 
            tdsmac_hs_dl_config.operation.save_hs_params,
            tdsmac_hs_dl_config.operation.save_non_hs_params,
            tdsmac_hs_dl_config.operation.perform_hs_reset);

    /*-----------------------------------------------------------------------------
    MAC sets dl_configuration to TRUE when RRC asks MAC to wait for the signal
    from L1.
    ------------------------------------------------------------------------------*/
    if (cmd_data_ptr->mac_config_info.wait_for_l1_sig)
    {
      /* Clear the memory */
      memset(&tdsdl_cfginfo,0,sizeof(tdsmac_dl_config_type));

      /* Copy the configuration information received from RRC*/
      //memcpy(&tdsdl_cfginfo,&cmd_data_ptr->mac_config_info, sizeof(tdsmac_dl_config_type));
      memscpy(&tdsdl_cfginfo,sizeof(tdsmac_dl_config_type),&cmd_data_ptr->mac_config_info, sizeof(tdsmac_dl_config_type));

      /*Set the boolean to TRUE indicating that MAC DL is now waiting for the signal
      from PHY layer to switch to the new configuration */
      tdsdl_configuration_pending=TRUE;

      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Wait for L1 signal to reconfig MAC");
    }
    else // Switch to the new configuration NOW
    {
      tdsconfig_dl_mac (&cmd_data_ptr->mac_config_info);

      if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5))
      {
        if (tdsmac_hs_dl_new_state != TDSMAC_HS_INVALID)
        {
          tdsmac_hs_dl_curr_state = tdsmac_hs_dl_new_state;
        }
      }
    }
    
    break;

    /* Invalid command */
  default:
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"rcvd invalid command %d",cmd_id);
    break;
  }
}


/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.
===============================================================================================*/
void tdsproc_dl_mac_config_signal ()
{
  if (tdsdl_configuration_pending)
  {
    /* Configure MAC only after this signal is recvd from L1 */
    tdsconfig_dl_mac(&tdsdl_cfginfo);

    if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5))
    {
      if (tdsmac_hs_dl_new_state != TDSMAC_HS_INVALID)
      {
        tdsmac_hs_dl_curr_state = tdsmac_hs_dl_new_state;
        tdsmac_hs_dl_new_state = TDSMAC_HS_INVALID;
      }
    }
    /* Post a sucess message to L1 task */
    tdsmaci_post_l1_config_ind(TDSL1_CPHY_MAC_DL_CONFIG_CNF);

    /*set configuration_pending to false */
    tdsdl_configuration_pending=FALSE;
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Recvd L1 sig,NO configuration pending !!");
  }
}


/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.

            NOTE -- mac_hs_reset will be performed in L2-DL task context rather than mac-hs task
            context.
===============================================================================================*/
void tdsproc_dl_mac_config_at_l1_failure_signal ()
{
  if (tdsdl_configuration_pending)
  {
    REX_ISR_LOCK(&tdsmac_hs_cfg_mutex);
    /* If we were to perform hs_reset and we received a config failed sig from L1*/
    if (tdsmac_hs_dl_config.operation.perform_hs_reset)
    {
      /* Start mac_hs_reset processing */
      tdsmac_hs_dl_start_reset_req();

      /* Complete mac_hs_reset processing */
      tdsmac_hs_dl_finish_reset_req();

      /* Reset local data structure that saves what operations need to be performed */
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
      
      TDSHSPA_PLUS_MAC_HS_TYPE_VALID_CHECK(tdsmac_hs_dl_config.mac_hs_or_ehs)
      
      if(tdsmac_hs_dl_config.mac_hs_or_ehs == MAC_EHS)
      {
        tdsmac_enhs_dl_reset_local_config_data();
      }
      else
#endif
      {
        tdsmac_hs_dl_reset_local_config_data();
      }
    }
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Rcvd L1 config fail sig,no hs-reset to be performed");
    }
    REX_ISR_UNLOCK(&tdsmac_hs_cfg_mutex);
    /* Post a sucess message to L1 task */
    tdsmaci_post_l1_config_ind(TDSL1_CPHY_MAC_DL_CONFIG_AT_L1_FAIL_CNF);

    /*set configuration_pending to false */
    tdsdl_configuration_pending = FALSE;
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Recvd L1 sig,NO configuration pending !!");
  }
}


/*==============================================================================================
 FUNCTION NAME  proc_dl_hs_config_signal

 FUNCTION   This function processes the HS config signal received from the physical layer.
===============================================================================================*/
void tdsproc_dl_mac_hs_config_signal ()
{
  REX_ISR_LOCK(&tdsmac_hs_cfg_mutex);
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

  TDSHSPA_PLUS_MAC_HS_TYPE_VALID_CHECK(tdsmac_hs_dl_config.mac_hs_or_ehs)
  
  if ((tdsmac_hs_dl_config.mac_hs_or_ehs != MAC_HS_TYPE_INVALID) &&
#else
  if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) &&
#endif
      ( tdsmac_hs_dl_config.operation.perform_hs_reset ||
        tdsmac_hs_dl_config.operation.save_hs_params ))
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Pre-configuring HS DSCH/HSPA+");

    /*----------------------------------------------------------------
     *  There will be a context switch inside the function below where
     *  L2-DL task signals MAC-HS task to pick up MAC-HS configuration.
     *  For details discussion, please see the MAC-HS design document.
     ----------------------------------------------------------------*/
    tdsprocess_dl_hs_dsch_config(&tdsdl_cfginfo);

    /*----------------------------------------------------------------
     * L2-DL task procesing will resume here
     ----------------------------------------------------------------*/

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if (log_status(TDSCDMA_MAC_HS_CONFIG_LOG_PACKET))
    {
      tdsmac_ehs_dl_send_hs_config_log_packet(&tdsdl_cfginfo);
    }
#else
    if (log_status(TDSCDMA_MAC_HS_CONFIG_LOG_PACKET))
    {
      tdsmac_hs_dl_send_hs_config_log_packet(&tdsdl_cfginfo);
    }
#endif /*((defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS))  */
#endif

    if (tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5)
    {
      if (tdsmac_hs_dl_new_state != TDSMAC_HS_INVALID)
      {
        tdsmac_hs_dl_curr_state = tdsmac_hs_dl_new_state;
        tdsmac_hs_dl_new_state = TDSMAC_HS_INVALID;
      }
    }
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Recvd HS config sig,HS config not pending !!");
  }
  REX_ISR_UNLOCK(&tdsmac_hs_cfg_mutex);
}
/*===============================================================================================
FUNCTION NAME tdsmac_dl_get_trch_type

DESCRIPTION   This function returns the down-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

log_tdscdma_trch_type tdsmac_dl_get_trch_type(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;

  return_trch_val = tdsmac_dl_check_dch_trch(rlc_id, rb_id);
  if(return_trch_val != TDSCDMA_TRCH_INVALID)
  {
    return return_trch_val;
  }

  return_trch_val = tdsmac_dl_check_fach_trch(rlc_id, rb_id);
  if(return_trch_val != TDSCDMA_TRCH_INVALID)
  {
    return return_trch_val;
  }


 TDSMAC_MSG2(MSG_LEGACY_HIGH,"mac_dl_get_trch_type: returning invalid for rlc-id:%d rb-id:%d",rlc_id,rb_id);
  return return_trch_val;
}
