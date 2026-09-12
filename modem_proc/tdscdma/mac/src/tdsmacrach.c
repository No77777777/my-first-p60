/*===========================================================================================
                               R A C H    H A N D L E R

GENERAL DESCRIPTION

  This module provides functions to handle RACh functions

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===============================================================================================*

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macrach.c_v   1.17   10 Jul 2002 10:01:34   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacrach.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who  what, where, why
--------  ---  -----------------------------------------------------------
10/14/10  cyx   Remove handle of RACH in BACKOFF state in tdsrach_tx_control, 
                for the state is used only in FDD when receive NACK.
                Remove globle variable backoff_val, it is used in FDD only.
10/14/10  cyx   Remove NACK handle in tdsproc_ul_tx_status_ind for NACK is only in FDD
10/08/10  cyx   Change DCCH_DTCH_HDR_WITH_CRNTI_ON_RACH to tds_dcch_dtch_hdr_with_crnti_on_rach
                and remove  variable  crnti in tdsproc_rach_cctrch_cfg
10/08/10  cyx   Change DCCH_DTCH_HDR_WITH_CRNTI_ON_RACH to handle TCTF in TDD and 
                make C/T field always exist in mac header
10/08/10  guo   Initial release Upmerged WCDMA to TDS

==============================================================================================*/

/*==============================================================================================
                                    INCLUDE FILES
==============================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include  "tdsmacrrcif.h"
#include  "tdsmacrlcif.h"
#include  "tdsl1sapcommon.h"
#include  "tdsl1mtask.h"
#include  "tdsl2ultask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacul.h"
#include  "tdsmaculdata.h"
#include  "ran.h"
#include  "tdsmaculsubtask.h"

#include "tdsl1macif.h"

#include  <string.h>
#include  "tdsmacrach.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#include  "secapi.h" 

/*===============================================================================================
                           DATA DECLARATIONS
===============================================================================================*/

/* Number of RACHs */
uint8   tdsnum_rachs;

/* RACH State */
tdsrach_state_e_type tdsrach_state = TDSRACH_IN_IDLE;

/* Number of RACH Preamble Cycles */
uint8 tdsnpreamble_cycles;

/* Ordered RACH transmit control parameters */
tdsmac_ul_rach_txcntl_config_type    tdsrach_cntl_info;

/* Pointer to ordered config data of TDSRACH */
tdsmac_ul_rach_cctrch_config_type    *tdsrach_cfg_ptr;

/* Layer 1 Transmit Data Buffer Pointer */
tdsl1_ul_tx_data_type    *tdsl1_tx_buf_ptr;

/* Number of Transport Blocks transmitted on TDSRACH */
uint16    tdsnum_tx_tbs = 0;

/* Pointer to ordered TFCS data */
extern tdsmac_ul_tfcs_type    *tdsoc_ul_tfcs_ptr;

/* Current TTI */
extern tdsl1_tti_enum_type    tdscurrent_tti;

/*Sarao: Systems dashboard local copy of counters*/
tds_l2_rach_stats_type tds_rach_stats;

/*---------------------------------------------------------------------------
  Contains the list of logical channels for which MAC needs the buffer status
---------------------------------------------------------------------------*/
extern tdsrlc_ul_lc_info_type              tdsmac_ul_lc_info;

/*===============================================================================================
FUNCTION NAME tdsmac_free_tx_dsm_items

DESCRIPTION   Frees Transmit DSM items
===============================================================================================*/
void  tdsmac_free_tx_dsm_items
  (
/* Transmit Data Buffer Pointer */
  tdsl1_ul_tx_data_type    *tx_buf_ptr
  )
{
  uint8 j;

  /* Transport channel TX info pointer */
  tdsl1_ul_trch_data_type  *trch_tx_ptr;

  /* Pointer to Transport channel info */
  trch_tx_ptr = &tx_buf_ptr->tx_data[0];

  TDSMAC_MSG0(MSG_LEGACY_HIGH,"Released DSM items,set Trch=0");

  tx_buf_ptr->num_trch = 0;

  /* Release the DSM item of each Transport Block */
  for (j=0; j < tdsnum_tx_tbs; j++)
  {

    tdsl1_l2_dsm_free_packet (&trch_tx_ptr->trch_data[j].data_ptr);

    TDSMAC_MSG1(MSG_LEGACY_LOW,"Released DSM item %d",j);
  }
}

/*===============================================================================================
FUNCTION NAME tdsmac_reset_rach

DESCRIPTION   Resets RACH state
===============================================================================================*/
void  tdsmac_reset_rach
  (
  void
  )
{
  TDSL2_MUTEX_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  /* Free the DSM items */
  tdsmac_free_tx_dsm_items (tdsl1_tx_buf_ptr);

  /* Set RACH state to IDLE */
  tdsrach_state = TDSRACH_IN_IDLE;

  /* Clear preamble ramping cycle count */
  tdsnpreamble_cycles = 0;


  /*-----------------------------------------------------------------------------
  MAC will not register the call back function with RLC. PRACH would always be
  enabled during the lifetime of the call.
  ------------------------------------------------------------------------------*/

  TDSL2_MUTEX_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
}

/*===============================================================================================
FUNCTION NAME tdsmac_send_tx_status_ind_to_rrc

DESCRIPTION   Sends CMAC_STATUS_IND to RRC
===============================================================================================*/
void  tdsmac_send_tx_status_ind_to_rrc
  (
  tdsmac_status_ind_e_type status
  )
{
  tdsrrc_mac_cmd_type   *rrc_cmd_ptr;

  /* Failed to allocate memory for RRC command */
  if ((rrc_cmd_ptr = tdsrrc_get_mac_cmd_buf()) == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"NULL Ptr when sending CMAC_STATUS_IND");
  }
  /* Send TDSRRC_CMAC_STATUS_IND to RRC */
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CMAC_STATUS_IND to RRC");

    /* Fill in command header */
    rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CMAC_STATUS_IND;

    /* Fill in command data */
    rrc_cmd_ptr->cmd.cmac_status_ind = status;

    /* Send command to RRC */
    tdsrrc_put_mac_cmd(rrc_cmd_ptr);
  }
}

/*===============================================================================================
FUNCTION NAME tdsproc_rach_tx_parameters

DESCRIPTION   Processes RACH Transmit Control Parameters
===============================================================================================*/
void  tdsproc_rach_tx_parameters
  (
  tdsmac_ul_rach_txcntl_config_type    *rach_tx_prmtrs
  )
{
  /* Copy RACH transmit control parameters to local memory */
  //memcpy (&tdsrach_cntl_info, rach_tx_prmtrs, sizeof(tdsmac_ul_rach_txcntl_config_type));

	memscpy (&tdsrach_cntl_info, sizeof(tdsmac_ul_rach_txcntl_config_type), rach_tx_prmtrs, sizeof(tdsmac_ul_rach_txcntl_config_type));

  /* The number of ASC's should be atleast 1 */
  if (tdsrach_cntl_info.nASC == 0)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of ASC's is 0");
  }

}

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
/*===============================================================================================
FUNCTION NAME	tdsmac_ul_proc_rach_tx_update_parameters

DESCRIPTION		Update RACH Transmit Control Parameters
===============================================================================================*/
void tdsmac_ul_proc_rach_tx_update_parameters 
(
  tdsmac_ul_persistence_config_type		*persistence_info
)
{
  REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);

  /* Update RACH transmit control parameters to local memory */
  tdsrach_cntl_info.nASC = persistence_info->nASC;
  if(tdsrach_cntl_info.nASC < MAX_ASC)
  {
    //memcpy (tdsrach_cntl_info.asc, persistence_info->asc, sizeof(tdsmac_ul_asc_type)*tdsrach_cntl_info.nASC);
    memscpy (tdsrach_cntl_info.asc, sizeof(tdsmac_ul_asc_type)*MAX_ASC, persistence_info->asc, sizeof(tdsmac_ul_asc_type)*tdsrach_cntl_info.nASC);
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of ASC exceed MAX_ASC");
  }

  REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);

  /* The number of ASC's should be at least 1 */
  if (tdsrach_cntl_info.nASC == 0)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Number of ASC's is 0");
  }
}
#endif

/*===============================================================================================
FUNCTION NAME tdspick_mlp

DESCRIPTION   Selects the maximum MAC logical channel priority.

RETURNS     1 - 8 Depending on the selected MLP
===============================================================================================*/
uint8 tdspick_mlp(tdsrlc_ul_frame_format_type *rlc_chan_info_ptr)
{
  uint8 mlp = TDSMAC_UL_MAX_MLP_ID+1; // TDSMAC_UL_MAX_MLP_ID = 8
  uint8           i;
  tdsul_lc_info_type *int_lc_ptr;
  tdsrlc_ul_logchan_format_type    *lc_chan_ptr;

  /* Eliminate TFCs based on logical channel buffer occupancy */
  for (i=0 ; i < rlc_chan_info_ptr->nchan; i++)
  {
    lc_chan_ptr = &rlc_chan_info_ptr->chan_fmt[i];
    /* Get the logical channel ptr corresponding to the rlc_id */
    int_lc_ptr = TDSGET_UL_LC_INFO_PTR((rlc_lc_id_type)(lc_chan_ptr->rlc_id));

    /* The number of pdus is greater than 0 */
    if (lc_chan_ptr->npdus > 0)
    {
      if (mlp > int_lc_ptr->asc_or_mlp)
      {
        mlp = int_lc_ptr->asc_or_mlp;
      }
    }

  }
  TDSMAC_MSG1(MSG_LEGACY_HIGH,"Picked MLP %d",mlp);
  return mlp;
}


/*===============================================================================================
FUNCTION NAME asc_selection

DESCRIPTION   Selects an Access Service Class

RETURNS     0 - 7 Depending on the selected ASC
===============================================================================================*/
uint8 tdsmac_asc_selection (tdsrlc_ul_frame_format_type *chan_fmt)
{
  uint8 asc=0;

  uint8 mlp;

  /* If the ccch is enabled and if the number of dedicated logical
  channels mapped to ccch is 0 */
  if ((tdsrach_cfg_ptr->ccch_enable) && (tdsrach_cfg_ptr->ndlchan==0))
  {
    /* Since the value of asc is used as an index into tdsrach_cntl_info.asc array
    to get the persistence values, care should be taken not to access the invalid
    entries in the array. hence the following check. tdsrach_cntl_info.nASC gives the
    number of ASC's signalled to UE */
    if (tdsrach_cfg_ptr->ccch_asc >= tdsrach_cntl_info.nASC)
    {
      asc = tdsrach_cntl_info.nASC-1;
    }
    else /* its safe to use the ccch_asc specified */
    {
      asc = tdsrach_cfg_ptr->ccch_asc;
    }

    TDSMAC_MSG1(MSG_LEGACY_HIGH,"Picked ASC %d",asc);
  }
  else
  {
    asc = tdsrach_cntl_info.nASC;

    /* Pick the mlp with the highest priority */
    mlp = tdspick_mlp(chan_fmt);

    /* Among the ASC and MLP, pick the one with the highest priority */
    if (asc > mlp)
    {
      asc = mlp;
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"Picked ASC: %d MLP %d",asc,mlp);
    }
    else
    {
      /* Since the value of asc is used as an index into tdsrach_cntl_info.asc array
      to get the persistence values, care should be taken not to access the invalid
      entries in the array. hence the following check. tdsrach_cntl_info.nASC gives the
      number of ASC's signalled to UE */
      asc = tdsrach_cntl_info.nASC-1;
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"Picked ASC: %d MLP %d",asc,mlp);
    }
  }


  return(asc);
}

/*===============================================================================================
FUNCTION NAME tdspersistence_is_ok

DESCRIPTION   Checks to see if the persistence is OK or not

RETURNS     TRUE  if the persistence is OK FALSE otherwise

===============================================================================================*/
boolean   tdspersistence_is_ok ()
{
  boolean rv = FALSE;




  uint32  random_num;

  /*-------------------------------------------------------------------------------------------
  ran_next() function is defined in ran.c. It generates a random number between 0 and 1, when
  the seed is an integer between 1 and 2**31-2.
  -------------------------------------------------------------------------------------------*/

  /* ran_next() Generates a random number between 0 and 1 */
#ifdef T_WINNT
  #error code not present
#else
  if (E_SUCCESS != secapi_get_random(SECAPI_SECURE_RANDOM, (uint8 *)&random_num, sizeof(*(&random_num))))
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"[MAC RACH]:Can't get random number");
	return(rv);
  }
#endif
  /* Normalize this random number to a value between 0 & 65535 */
  random_num = ran_dist(random_num, (uint16)0, (uint16)65535);

  /* If the random value is less than the persistence associated with the
  acess service class that was selected, then persistence test has passed */
  if (random_num <= tdsrach_cntl_info.asc[tdsl1_tx_buf_ptr->asc_num].pval)
  {
    rv = TRUE;
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"random_num %d,Persistence %d",random_num,tdsrach_cntl_info.asc[tdsl1_tx_buf_ptr->asc_num].pval);
  }
  /* Else the persistence test has failed and wait for the next TTI to
  generate a new random number ... */
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"rnum %d Pi %d",random_num,tdsrach_cntl_info.asc[tdsl1_tx_buf_ptr->asc_num].pval);
  }


  return(rv);
}
/*============================================================================================
FUNCTION NAME   tds_dcch_dtch_hdr_with_crnti_on_rach

DESCRIPTION     This macro decides MAC header value and size when DCCH/DTCH is mapped to
        RACH, and C-RNTI is used, MAC header size is 26-bits long and it consists of :
        - TCTF     : 4-bits long and its value = 0100
        - UE ID Type : 2-bits long and its value = 01
        - UE ID    : 16-bits long and its value is C-RNTI
        - C/T    : 4-bits long & value is one minus MAC Logical channel identity
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
symbol:    TCTF("#"),UE_ID type("%"),UE_ID("$"),C/T("@"),DATA("?")
                                                                  
mac header:  

|<- hdrval[0]   ->|  |<- hdrval[1]   ->| |<- hdrval[2]   ->| |<- hdrval[3]   ->|
| 7 6 5 4 3 2 1 0 |  | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 |
| # # # # % % $ $ |  | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ @ @ | | @ @ ? ? ? ? ? ? |
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                                             |7 6 5 4 3 2 1 0|
         hdrval[0]                         :  # # # # % % $ $
             =
DCCH_DTCH_HDR_WITH_TCTF_CRNTI_TYPE_ON_RACH :  # # # # % % 0 0
             +
         crnti >> 14                       :  0 0 0 0 0 0 $ $

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                               |<- hdrval[1]   ->|
                                           1 0 | 7 6 5 4 3 2 1 0 | 5 4 3 2 1 0
                                               *******************
         hdrval[1] =                       $ $ * $ $ $ $ $ $ $ $ * $ $ $ $ $ $
                                               *******************
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                             |7 6 5 4 3 2 1 0|
         hdrval[2]                         :  $ $ $ $ $ $ @ @
            =
     (crnti << 2) & 0xfc                   :  $ $ $ $ $ $ 0 0
            +
((oc_dlcptr->mac_id - 1) >> 2)&0x3)        :  0 0 0 0 0 0 @ @

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           
                                          |<- hdrval[3]   ->|
         hdrval[3] =                  1 0 | 7 6 5 4 3 2 1 0 | 
                                          ******
                                      @ @ * @ @*0 0 0 0 0 0 
                                          ******
============================================================================================*/
void tds_dcch_dtch_hdr_with_crnti_on_rach
(
    uint8                            *hdrval,
    tdsmac_ul_ded_logch_config_type  *oc_dlcptr,
    tdsul_lc_info_type               *int_lc_ptr
)
{
    uint16                    crnti;

    /* Get CRNTI value */
    crnti = TDSGET_UL_MAC_CRNTI;

    /* 1st header byte consists of TCTF, UE-ID type & 2 MSBits of UE-ID */
    hdrval[0] = (uint8)(DCCH_DTCH_HDR_WITH_TCTF_CRNTI_TYPE_ON_RACH + (crnti >> 14));

    /* 2nd header byte contains the next 8 MSBits of UE-ID */
    hdrval[1] = (uint8)(crnti >> 6);

    /* 6 MSBits of 3rd header byte contains 6 LSBits of UE ID; 2 LSBits of 3rd header byte contains 2 MSBits of C/T */
    hdrval[2] = ((uint8)(((crnti << 2) & 0xfc) | (((oc_dlcptr->mac_id - 1) >> 2)&0x3)));

	/* 2 MSBits of 4rd header byte contains 2 LSBits of C/T */
	hdrval[3] =  (uint8)(((oc_dlcptr->mac_id - 1) << 6)&0xc0);

	/* Header size = 26 bits */
	int_lc_ptr->mac_hdr_size = 26;

    /* Save header value */
    //memcpy (int_lc_ptr->mac_hdr_value, hdrval, TDSMAC_UL_MAX_HDR_SIZE);
    memscpy (int_lc_ptr->mac_hdr_value, TDSMAC_UL_MAX_HDR_SIZE, hdrval, TDSMAC_UL_MAX_HDR_SIZE);

}


/*==============================================================================================
 FUNCTION NAME    tdsproc_rach_cctrch_cfg

 DESCRIPTION    This function processes ordered config data of RACH
==============================================================================================*/
void  tdsproc_rach_cctrch_cfg
  (
/* Pointer to ordered config data of RACH */
  tdsmac_ul_rach_cctrch_config_type    *oc_rachptr
  )
{
  uint8 i, ndlchan, hdrval[TDSMAC_UL_MAX_HDR_SIZE];
  tdsmac_ul_ded_logch_config_type  *oc_dlcptr;
  tdsul_lc_info_type         *int_lc_ptr;
  tdsul_trch_info_type       *int_tc_ptr;
  //boolean                   valid; /*lint -e550*/
  tdstrch_id_to_idx_map_type   *id_to_idx_ptr;

  tdsmaci_ul_trch_priority_info_type  *priority_info_ptr;

  uint8     rlc_count=0;
  /* Initialize */
  tdsmac_ul_lc_info.nchan = 0;

  /* Get pointer to internal Transport Channel information */
  /* Right now, it is assumed that there is only one RACH */
  int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(0);
  id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(oc_rachptr->trch_id);
  /* Populate the trch_id_to_idx_table */
  id_to_idx_ptr->idx = 0;
  id_to_idx_ptr->valid = TRUE;

  TDSMAC_MSG0(MSG_LEGACY_LOW,"RACH Configuration Parameters:");

  /* Save RACH Ordered Config Data pointer */
  tdsrach_cfg_ptr = oc_rachptr;

  /* Transport channel ID */
  int_tc_ptr->trch_id = oc_rachptr->trch_id;

  /* CCCH is mapped to RACH */
  if (oc_rachptr->ccch_enable)
  {

    /* CCCH ID is invalid */
    if (TDSUL_RLC_ID_IS_INVALID(oc_rachptr->ccch_id))
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"CCCH ID is invalid %d",oc_rachptr->ccch_id);
    }
    /* Access Service Class is invalid */
    else if (TDSUL_ASC_IS_INVALID(oc_rachptr->ccch_asc))
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"ASC is invalid %d",oc_rachptr->ccch_asc);
    }
    /* CCCH parameters are valid */
    else
    {
      /* RLC info pointer */
      int_lc_ptr = TDSGET_UL_LC_INFO_PTR(oc_rachptr->ccch_id);

      /* Set Enable flag to TRUE */
      int_lc_ptr->enable = TRUE;

      /* Logical channel type is CCCH */
      int_lc_ptr->lc_type = UE_LOGCHAN_CCCH;

      /* Set RLC mode to TM_MODE */
      int_lc_ptr->rlc_mode = UE_MODE_TRANSPARENT;

      /* Save the rlc size restriction information */
      int_lc_ptr->rlc_size_info.all_tfi_valid = FALSE;
      int_lc_ptr->rlc_size_info.ntfi_per_lc = 1;
    {
      int_lc_ptr->rlc_size_info.lc_tfi_list[0] = 0;
    }

    /* Access Service Class */
    int_lc_ptr->asc_or_mlp = oc_rachptr->ccch_asc;

    /* MAC Header Size = 2 bits */
    int_lc_ptr->mac_hdr_size = 2;

    /* Save the Logical channel ID in transport channel information */
    int_tc_ptr->rlc_id[int_tc_ptr->nlchan] = oc_rachptr->ccch_id;

    /* Update logical channel count of transport channel info */
    int_tc_ptr->nlchan++;

      if (rlc_count < TDSUE_MAX_UL_LOGICAL_CHANNEL)
      {
        tdsmac_ul_lc_info.rlc_id[rlc_count++] = oc_rachptr->ccch_id;
      }
      tdsmac_ul_lc_info.nchan = rlc_count;

    TDSMAC_MSG3(MSG_LEGACY_LOW,"CCCH: ASC= %d RLC Mode= %d RLC ID= %d",
      int_lc_ptr->asc_or_mlp, int_lc_ptr->rlc_mode, oc_rachptr->ccch_id);


      // Get the priority information of the transport channel 0 with priority
      // as indicated by the ccch_asc field.
      if (oc_rachptr->ccch_asc == 0)
      {
        priority_info_ptr = &tdsint_ul_trch_priority_info[0][0];
      }
      else
      {
        priority_info_ptr = TDSGET_UL_TRCH_PRIORITY_INFO_PTR(0,int_lc_ptr->asc_or_mlp);
      }
      // Populate the logical channel id which has this priority.
      priority_info_ptr->lc_id[priority_info_ptr->num_lc] = oc_rachptr->ccch_id;
      priority_info_ptr->num_lc++;

      TDSMAC_MSG2(MSG_LEGACY_HIGH,"Pri %d,NChan %d",oc_rachptr->ccch_asc,priority_info_ptr->num_lc);


    }
  }

  /* Number of DCCH/DTCHs */
  ndlchan = oc_rachptr->ndlchan;

  if (ndlchan >0 )
  {

    /* Valid number of DCCH/DTCHs mapped to TDSRACH */
    if (ndlchan > MAX_DED_LOGCHAN_PER_TRCH)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of DCCH/DTCHs %d",ndlchan);
    }
    /* Invalid C-RNTI */
    else if (TDSUL_MAC_CRNTI_IS_VALID == FALSE)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"UE has no valid CRNTI");
    }
    /* Process config info of each DCCH/DTCH */
    else
    {
      for (i=0; i < ndlchan; i++)
      {

        /* Initially mark the parameter validity flag to FALSE */
        //valid = FALSE;

        /* Pointer to ordered config parameters of DCCH/DTCH */
        oc_dlcptr = &oc_rachptr->dlchan_info[i];

        /* Invalid RLC ID */
        if (TDSUL_RLC_ID_IS_INVALID(oc_dlcptr->rlc_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC ID %d",i,oc_dlcptr->rlc_id);
        }
        /* Invalid RB ID */
        else if (TDSUL_RB_ID_IS_INVALID(oc_dlcptr->rb_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RB ID %d",i,oc_dlcptr->rb_id);
        }
        /* Invalid MAC Priority */
        else if (TDSUL_LC_PRIORITY_IS_INVALID(oc_dlcptr->priority))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid priority %d",i,oc_dlcptr->priority);
        }
        /* Invalid RLC Mode */
        else if (TDSUL_RLC_MODE_IS_INVALID(oc_dlcptr->rlc_mode))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC Mode %d",i,oc_dlcptr->rlc_mode);
        }
        /* Invalid MAC ID */
        else if ((ndlchan > 1) && TDSUL_MAC_ID_IS_INVALID(oc_dlcptr->mac_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid MAC ID %d",i,oc_dlcptr->mac_id);
        }
        /* Otherwise, parameters are valid */
        else
        {
          /* Uplink logical channel information table entry pointer */
          int_lc_ptr = TDSGET_UL_LC_INFO_PTR(oc_dlcptr->rlc_id);

          /* Clear header value */
          memset (hdrval, 0,TDSMAC_UL_MAX_HDR_SIZE);

          /* MAC Header Size and Header Value */
          tds_dcch_dtch_hdr_with_crnti_on_rach(hdrval,oc_dlcptr,int_lc_ptr);

          /* Set Enable flag to TRUE */
          int_lc_ptr->enable = TRUE;

          /* Logical channel type */
          int_lc_ptr->lc_type = oc_dlcptr->chan_type;

          /* Save RLC mode */
          int_lc_ptr->rlc_mode = oc_dlcptr->rlc_mode;

          /* Save RB ID */
          int_lc_ptr->rb_id = oc_dlcptr->rb_id;

          /* Save priority */
          int_lc_ptr->asc_or_mlp = oc_dlcptr->priority;

          if (rlc_count < TDSUE_MAX_UL_LOGICAL_CHANNEL)
          {
            tdsmac_ul_lc_info.rlc_id[rlc_count++] = oc_dlcptr->rlc_id;
          }
          tdsmac_ul_lc_info.nchan = rlc_count;


          /* Save the rlc size restriction information */
          /*memcpy(&int_lc_ptr->rlc_size_info,&oc_dlcptr->rlc_size_info,
            sizeof(tdsmac_ul_rlc_size_info_type));*/

          memscpy(&int_lc_ptr->rlc_size_info,
                  sizeof(tdsmac_ul_rlc_size_info_type), 
                  &oc_dlcptr->rlc_size_info,
            sizeof(tdsmac_ul_rlc_size_info_type));

          /* RLC Header Length */
          if (int_lc_ptr->rlc_mode == UE_MODE_TRANSPARENT)
            int_lc_ptr->rlc_hdr_length = 0;
          else if (int_lc_ptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
            int_lc_ptr->rlc_hdr_length = 8;
          else
            int_lc_ptr->rlc_hdr_length = 16;


          priority_info_ptr = TDSGET_UL_TRCH_PRIORITY_INFO_PTR(0, oc_dlcptr->priority);

          // Populate the logical channel id which has this priority.
          priority_info_ptr->lc_id[priority_info_ptr->num_lc] = oc_dlcptr->rlc_id;
          priority_info_ptr->num_lc++;
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"Pri %d,NChan %d",oc_rachptr->ccch_asc,priority_info_ptr->num_lc);



          /* Save the Logical channel ID in transport channel information */
          /* Update logical channel count of transport channel info */
          int_tc_ptr->rlc_id[int_tc_ptr->nlchan] = oc_dlcptr->rlc_id;
          int_tc_ptr->nlchan++;

          TDSMAC_MSG3(MSG_LEGACY_LOW,"DCCH/DTCH%d:HdrSize= %d Hdr[0]= %x",
            i, int_lc_ptr->mac_hdr_size, int_lc_ptr->mac_hdr_value[0]);
          TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[1]= %x Hdr[2]= %x Hdr[3]= %x",
            int_lc_ptr->mac_hdr_value[1], int_lc_ptr->mac_hdr_value[2],
            int_lc_ptr->mac_hdr_value[3]);
          TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[4]= %x RB ID= %d RLC Mode= %d",
            int_lc_ptr->mac_hdr_value[4], int_lc_ptr->rb_id, int_lc_ptr->rlc_mode);
        }
      }
    }
  }
}



/*==============================================================================================
 FUNCTION NAME    tdsproc_update_rach_mac_header

 DESCRIPTION    This function update MAC header for each logical channel
==============================================================================================*/
void  tdsproc_update_rach_mac_header
  (
/* Pointer to ordered config data of RACH */
  tdsmac_ul_rach_cctrch_config_type    *oc_rachptr
  )
{
  uint8 i, ndlchan, hdrval[TDSMAC_UL_MAX_HDR_SIZE];
  tdsmac_ul_ded_logch_config_type  *oc_dlcptr;
  tdsul_lc_info_type         *int_lc_ptr;

  TDSMAC_MSG0(MSG_LEGACY_HIGH,"Update RACH MAC header for logical channels");
	
  /* Number of DCCH/DTCHs */
  ndlchan = oc_rachptr->ndlchan;

  if (ndlchan >0 )
  {

    /* Valid number of DCCH/DTCHs mapped to TDSRACH */
    if (ndlchan > MAX_DED_LOGCHAN_PER_TRCH)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of DCCH/DTCHs %d",ndlchan);
    }
    /* Invalid C-RNTI */
    else if (TDSUL_MAC_CRNTI_IS_VALID == FALSE)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"UE has no valid CRNTI");
    }
    /* Process config info of each DCCH/DTCH */
    else
    {
      for (i=0; i < ndlchan; i++)
      {

        /* Initially mark the parameter validity flag to FALSE */
        //valid = FALSE;

        /* Pointer to ordered config parameters of DCCH/DTCH */
        oc_dlcptr = &oc_rachptr->dlchan_info[i];

        /* Invalid RLC ID */
        if (TDSUL_RLC_ID_IS_INVALID(oc_dlcptr->rlc_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC ID %d",i,oc_dlcptr->rlc_id);
        }
        /* Invalid RB ID */
        else if (TDSUL_RB_ID_IS_INVALID(oc_dlcptr->rb_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RB ID %d",i,oc_dlcptr->rb_id);
        }
        /* Invalid MAC Priority */
        else if (TDSUL_LC_PRIORITY_IS_INVALID(oc_dlcptr->priority))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid priority %d",i,oc_dlcptr->priority);
        }
        /* Invalid RLC Mode */
        else if (TDSUL_RLC_MODE_IS_INVALID(oc_dlcptr->rlc_mode))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC Mode %d",i,oc_dlcptr->rlc_mode);
        }
        /* Invalid MAC ID */
        else if ((ndlchan > 1) && TDSUL_MAC_ID_IS_INVALID(oc_dlcptr->mac_id))
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid MAC ID %d",i,oc_dlcptr->mac_id);
        }
        /* Otherwise, parameters are valid */
        else
        {
          /* Uplink logical channel information table entry pointer */
          int_lc_ptr = TDSGET_UL_LC_INFO_PTR(oc_dlcptr->rlc_id);

          /* Clear header value */
          memset (hdrval, 0,TDSMAC_UL_MAX_HDR_SIZE);

          /* MAC Header Size and Header Value */
          tds_dcch_dtch_hdr_with_crnti_on_rach(hdrval,oc_dlcptr,int_lc_ptr);

          TDSMAC_MSG3(MSG_LEGACY_LOW,"DCCH/DTCH%d:HdrSize= %d Hdr[0]= %x",
            i, int_lc_ptr->mac_hdr_size, int_lc_ptr->mac_hdr_value[0]);
          TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[1]= %x Hdr[2]= %x Hdr[3]= %x",
            int_lc_ptr->mac_hdr_value[1], int_lc_ptr->mac_hdr_value[2],
            int_lc_ptr->mac_hdr_value[3]);
          TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[4]= %x RB ID= %d RLC Mode= %d",
            int_lc_ptr->mac_hdr_value[4], int_lc_ptr->rb_id, int_lc_ptr->rlc_mode);
        }
      }
    }
  }
}

/*===============================================================================================
 FUNCTION NAME  proc_tx_status

 DESCRIPTION  This function processes transmit status received from PHY layer

===============================================================================================*/
void  tdsproc_ul_tx_status_ind(tdsl1_prach_status_enum_type   rach_tx_status_ind)
{
  if (tdsrach_state == TDSRACH_IN_IDLE)
{
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"RACH is in IDLE State - Not expected response %d",rach_tx_status_ind);
    return;
  }
  /* RACH is waiting for Status from Layer 1 */
  switch (rach_tx_status_ind)
  {

  /* Received Tx Status is ACK */
  case TDSL1_PRACH_MSG_DONE:
    /* RACH is not waiting for status */
    if (tdsrach_state != TDSRACH_WAITING_FOR_STATUS)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"rcvd Tx status %x in wrong rach state %x",rach_tx_status_ind);
    }
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"rcvd L1_PRACH_ACK from L1");

      /* Incrementing the counter for RACH TX success for systems dashboard*/
      tds_rach_stats.rach_success_cnt++;

      /* Send sucess indication to RRC */
      tdsmac_send_tx_status_ind_to_rrc(TDSRACH_TX_SUCCESS);
        
      /* Reset RACH */
      tdsmac_reset_rach();
    }
    break;

  case TDSL1_PRACH_DELAY:
    /* RACH is not waiting for status */
    if (tdsrach_state != TDSRACH_WAITING_FOR_STATUS)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"rcvd Tx status %x in wrong rach state %x",rach_tx_status_ind);
    }
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"rcvd TDSL1_PRACH_DELAY from L1");

      /*Increment the counter for random access procedure failure due to other reasons*/
      tds_rach_stats.rach_failure_due_to_other_reasons_cnt++;

      /* Set RACH state to RCVD_NO_ACK */
      tdsrach_state = TDSRACH_RCVD_NOACK;
    }
    break;

    /* Received TX Status is NO ACK */
  case TDSL1_PRACH_NO_ACK:

    /* RACH is not waiting for status */
    if (tdsrach_state != TDSRACH_WAITING_FOR_STATUS)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"rcvd Tx status %x in wrong rach state %x",rach_tx_status_ind);
    }
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"rcvd TDSL1_PRACH_NO_ACK from L1");

      /* Preamble Ramping cycles exceeded maximum count */
      if (tdsnpreamble_cycles >= tdsrach_cntl_info.mmax)
      {

        TDSMAC_MSG2(MSG_LEGACY_HIGH,"Reached max no. of Preamble cycles(%d)",tdsnpreamble_cycles,tdsrach_cntl_info.mmax);

        /* Increment the counter for random access procedure failure due to Max RACH Attempts for systems dashboard*/
        tds_rach_stats.rach_failure_due_to_max_rach_attempts_cnt++;
            
        /* Send CMAC_STATUS_IND to RRC */
        tdsmac_send_tx_status_ind_to_rrc (TDSMAX_RACH_CYCLES_REACHED);

        /* Reset MAC */
        tdsmac_reset_rach();
      }
      else
      {
        /* Set RACH state to RCVD_NO_ACK */
        tdsrach_state = TDSRACH_RCVD_NOACK;

      }
    }
    break;

    /* Received Abort Request from L1 */
  case TDSL1_PRACH_ABORT:

    /* RACH in IDLE */

    {

      TDSMAC_MSG0(MSG_LEGACY_HIGH,"rcvd TDSL1_PRACH_ABORT from L1");

      /*Sarao: Increment the RACh Abort counter for the systems dashboard */
      tds_rach_stats.rach_failure_due_to_abort_cnt++;

      /* Send CMAC_STATUS_IND to RRC */
      tdsmac_send_tx_status_ind_to_rrc (TDSRACH_TX_ABORTED);

      /* Reset MAC */
      tdsmac_reset_rach();
    }
    break;

    /* Invalid TX Status */
  default:
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"rcvd invalid Tx status (%d)",rach_tx_status_ind);
    break;
  }
}

/*================================================================================================
FUNCTION NAME tdsrach_tx_control

DESCRIPTION   This function controls RACH transmission and is called once every TTI
================================================================================================*/
boolean   tdsrach_tx_control
  (
  void
  )
{
  /* Initially set OK to transmit flag to FALSE */
  boolean   rach_tx_is_ok = FALSE;

  /* RACH Tx Control is performed based on RACH state */
  switch (tdsrach_state)
  {
    /* RACH in RCVD_NO_ACK state or PERSISTENCE FAILED state */
  case TDSRACH_WAITING_FOR_RIGHT_TTI:
  case TDSRACH_RCVD_NOACK:
  case TDSRACH_PERSISTENCE_FAILED:
    /* IF the persistence test has passed */
    if (tdspersistence_is_ok () == TRUE)
    {
      /* Check if this is the right TTI to transmit */
      if (tdscurrent_tti >= tdsoc_ul_tfcs_ptr->tfs_info[0].tti_info)
      {
        rach_tx_is_ok = TRUE;
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Found right TTI to transmit : cur_tti %d",tdscurrent_tti);
      }
      else
      {
        tdsrach_state = TDSRACH_WAITING_FOR_RIGHT_TTI;
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"TDSRACH_WAITING_FOR_RIGHT_TTI to transmit : cur_tti %d",tdscurrent_tti);
      }
    }
    else
      tdsrach_state = TDSRACH_PERSISTENCE_FAILED;
    break;

    /* RACH in Waiting for Status mode */
  case TDSRACH_WAITING_FOR_STATUS:
    break;

  default:
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid RACH state",tdsrach_state);
    break;
  }

  return(rach_tx_is_ok);
}

/*===============================================================================================
FUNCTION NAME tdsbuild_rach_tblks

DESCRIPTION   This function builds RACH Transport Blocks
===============================================================================================*/
void  tdsbuild_rach_tblks
  (
/* Pointer to Build Frame Indication Parameters */
  tdsmac_ul_build_frame_ind_type   *bld_frame_ind_ptr

  )
{

  uint8           selected_tfi;

  /* Layer 1 Transmit Data Buffer Pointer */
  tdsl1_tx_buf_ptr = bld_frame_ind_ptr->tx_buf_ptr;

  /* RACH in idle state and hence build new Transport blocks */
  if (tdsrach_state == TDSRACH_IN_IDLE)
  {

    /* Build UL Transport Blocks */
    (void)tdsbuild_ul_tblks (bld_frame_ind_ptr);

    if (tdsl1_tx_buf_ptr->num_trch == 1)
    {

      /* Selected ASC */
      //l1_tx_buf_ptr->asc_num = tdsmac_asc_selection();

      /* Selected TFCI */
      tdsl1_tx_buf_ptr->TFCI = TDSGET_SELECTED_TFCI;

      /* Persistence check has failed, set RACH state to TDSRACH_PERSISTENCE_FAILED and */
      /* UE is not allowed to transmit on TDSRACH */
      if (tdspersistence_is_ok() == FALSE)
      {
        tdsrach_state = TDSRACH_PERSISTENCE_FAILED;
        tdsl1_tx_buf_ptr->num_trch = 0;
      }
      /* Persistence check has passed. It is OK to transmit on TDSRACH */
      else
      {

        /* Set preamble ramping cycle count to one */
        tdsnpreamble_cycles = 1;

        tdsl1_tx_buf_ptr->new_rach_tx = TRUE;

        /* Set RACH state to TDSRACH_WAITING_FOR_STATUS */
        tdsrach_state = TDSRACH_WAITING_FOR_STATUS;

        TDSMAC_MSG0(MSG_LEGACY_LOW,"UL Frame Queued on RACH");
      }
    }
  }
  /* OK to transmit in this TTI */
  else if (tdsrach_tx_control())
  {

    /* Set RACH state to TDSRACH_WAITING_FOR_STATUS */
    tdsrach_state = TDSRACH_WAITING_FOR_STATUS;

    if(tdsnpreamble_cycles == 0)
    {
      tdsl1_tx_buf_ptr->new_rach_tx = TRUE;
    }
    else
    {
      tdsl1_tx_buf_ptr->new_rach_tx = FALSE;
    }

    /* Increment number of preamble ramping cycle count */
    tdsnpreamble_cycles++;

    TDSMAC_MSG1(MSG_LEGACY_HIGH,"tdsnpreamble_cycles %d",tdsnpreamble_cycles);

    /* Set Active Transport Channel Count to 1 */
    tdsl1_tx_buf_ptr->num_trch = 1;

    /* Selected Transport Format Combination Pointer */
    selected_tfi = tdsoc_ul_tfcs_ptr->tfc_info[TDSGET_SELECTED_TFCI].tfi[0];

    /* IF it is OK to transmit in this TTI then assign the transport blocks
    and the transport block information */
    tdsl1_tx_buf_ptr->tx_data[0].num_tb  = (uint8)tdsoc_ul_tfcs_ptr->tfs_info[0].tf_info[selected_tfi].ntblks;
    tdsl1_tx_buf_ptr->tx_data[0].tb_size = tdsoc_ul_tfcs_ptr->tfs_info[0].tf_info[selected_tfi].tb_size ;

    TDSMAC_MSG0(MSG_LEGACY_LOW,"UL Frame Requeued on RACH");
  }

  /* Save Transport Block Info */
  if (tdsl1_tx_buf_ptr->num_trch ==1)
  {
    tdsnum_tx_tbs = tdsl1_tx_buf_ptr->tx_data[0].num_tb;
  }
}
/*===============================================================================================
FUNCTION NAME tdsmac_ul_check_rach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a RACH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_ul_check_rach_trch(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;
  tdsul_lc_info_type *int_lc_ptr;

  /* Get the logical channel ptr corresponding to the rlc_id  */
  int_lc_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);

  if(int_lc_ptr == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"mac_ul_check_rach_trch: GET_UL_LC_INFO_PTR returns NULL");
  }
  /* Check if given RB ID mactch logical channel's RB ID */
  else if(int_lc_ptr->rb_id == rb_id)
  {
    return_trch_val = TDSCDMA_TRCH_RACH;
  }
  else
  { 
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"mac_ul_check_rach_trch: rrc_rb_id (%d) does not match l2_rb_id",rb_id,int_lc_ptr->rb_id); 
  }

  return return_trch_val;
}

/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_success_cnt

DESCRIPTION   This function returns the RACH success count value
===============================================================================================*/
uint32 tdsl2_get_rach_success_cnt(void)
{
    return tds_rach_stats.rach_success_cnt;

}


/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_abort_cnt

DESCRIPTION   This function returns the RACH FAILURE Abort count
===============================================================================================*/
uint32  tdsl2_get_rach_failure_due_to_abort_cnt(void)
{
    return tds_rach_stats.rach_failure_due_to_abort_cnt;

}

/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_max_rach_attempts_cnt

DESCRIPTION   This function returns RACH FAILURE Max Rach attempts
===============================================================================================*/
uint32 tdsl2_get_rach_failure_due_to_max_rach_attempts_cnt(void)
{
    return tds_rach_stats.rach_failure_due_to_max_rach_attempts_cnt;
}


/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_other_reasons_cnt

DESCRIPTION   This function returns RACH FAILURE for other reasons
===============================================================================================*/

uint32 tdsl2_get_rach_failure_due_to_other_reasons_cnt(void)
{
    return tds_rach_stats.rach_failure_due_to_other_reasons_cnt;
}

/*===============================================================================================
FUNCTION NAME tdsl2_clear_rach_stats

DESCRIPTION   This function clears all the counters
===============================================================================================*/

void tdsl2_clear_rach_stats(void)
{
    tds_rach_stats.rach_failure_due_to_abort_cnt=0;
    tds_rach_stats.rach_failure_due_to_max_rach_attempts_cnt=0;
    tds_rach_stats.rach_failure_due_to_other_reasons_cnt=0;
    tds_rach_stats.rach_success_cnt=0;

}

