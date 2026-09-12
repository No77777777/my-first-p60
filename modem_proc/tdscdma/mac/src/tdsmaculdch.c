/*===============================================================================================

              U L    D C H    H A N D L E R

GENERAL DESCRIPTION

  This module provides functions to :
  - process uplink DCH configuration data.
  - build DCH Transport Blocks

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===============================================================================================*/
/*===============================================================================================

                        EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculdch.c_v   1.9   10 Jul 2002 10:05:54   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculdch.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   -----------------------------------------------------------------------------
10/24/10  guo   Initial release Upmerged WCDMA to TDS

==============================================================================================*/

/*==============================================================================================
                     INCLUDE FILES
==============================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "tdsmacrrcif.h"
#include  "tdsl2ultask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacul.h"
#include  <string.h>
#ifdef FEATURE_DUMMY_SIGNALING
#error code not present
#endif
#include  "tdsmaculdch.h"

uint8   tdsmac_tm_bearers_count = 0;

/*---------------------------------------------------------------------------
  Contains the list of logical channels for which MAC needs the buffer status
---------------------------------------------------------------------------*/
extern tdsrlc_ul_lc_info_type              tdsmac_ul_lc_info;

/*==============================================================================================
MACRO NAME    TDSDCCH_DTCH_HDR_ON_UL_DCH

DESCRIPTION   This macro decides the header size and header value of DCCH/DTCH mapped to DCH

===============================================================================================*/
#define   TDSDCCH_DTCH_HDR_ON_UL_DCH\
\
  /* Multiple DCCH/DTCH mapped to DCH - C/T field exists */ \
  if (ndlchan > 1){ \
\
    /* Header consists of 4-bit C/T field */  \
        hdrval[0] = (uint8)((uint8)(oc_dlcptr->mac_id -1) << 4);   \
\
    /* Header size = 4 bits */  \
    int_lc_ptr->mac_hdr_size = 4; \
\
    /* Save header value */   \
    /*memcpy (int_lc_ptr->mac_hdr_value, hdrval, TDSMAC_UL_MAX_HDR_SIZE); */ \
    memscpy (int_lc_ptr->mac_hdr_value, TDSMAC_UL_MAX_HDR_SIZE, hdrval, TDSMAC_UL_MAX_HDR_SIZE);  \
  } \
  /* Only one DCCH/DTCH is mapped to DCH */ \
    else {  \
    /* Header Size = 0 bits */  \
    int_lc_ptr->mac_hdr_size = 0; \
  }

/*==============================================================================================
FUNCTION NAME tdsproc_ul_dch_cctrch_cfg

DESCRIPTION   This function processes UL DCH CCTrCh Configuration Data
==============================================================================================*/
void  tdsproc_ul_dch_cctrch_cfg
(
  /* Pointer to ordered config data of UL DCH type CCTrCh */
  tdsmac_ul_dch_cctrch_config_type *ded_cctrch_ptr
)
{
    uint8 i, j;
  uint8 ndchs, ndlchan;
  uint8 num_active_ul_dchs;
  uint8   hdrval[TDSMAC_UL_MAX_HDR_SIZE];
    tdstrch_id_to_idx_map_type   *id_to_idx_ptr;

  /* Pointer to UL DCH ordered config data */
  tdsmac_ul_dch_trch_config_type   *oc_dch_ptr;

  /* Pointer to DCCH/DTCH ordered config data */
  tdsmac_ul_ded_logch_config_type  *oc_dlcptr;

  /* Pointer to internal UL DCH configuration data */
  tdsul_trch_info_type       *int_tc_ptr;

  /* Pointer to internal UL Logical Channel Information */
  tdsul_lc_info_type         *int_lc_ptr;

  tdsmaci_ul_trch_priority_info_type *priority_info_ptr;

  uint8     rlc_count = 0;
  /* Initialize */
  tdsmac_ul_lc_info.nchan = 0;
  num_active_ul_dchs = 0;

  tdsmac_tm_bearers_count = 0;

  /* CCTrCh ordered config pointer is NULL */
  if (ded_cctrch_ptr == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"CCTrCh Config pointer is NULL");
  }
  else if (ded_cctrch_ptr->ndchs > TDSUE_MAX_TRCH)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of DCHs per CCTrCh",ded_cctrch_ptr->ndchs);
  }
  else
  {
      /* Number of DCHs mapped to this CCTrCh */
    ndchs = ded_cctrch_ptr->ndchs;

        /* Process ordered config data of each dedicated transport channel */
    for (i=0; i<ndchs; i++)
    {
      /* Pointer to ordered config data of Dedicated Transport Channel */
      oc_dch_ptr = &ded_cctrch_ptr->dch_info[i];

      /* DCH ordered config data pointer is NULL */
      if (oc_dch_ptr == NULL) { /*lint !e774*/
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"DCH%d pointer is NULL",i);
      }
      /* Invalid Transport Channel ID */
      else if (TDSUL_TRCH_ID_IS_INVALID(oc_dch_ptr->trch_id)) {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCH%d with invalid TrCh ID %d",i,oc_dch_ptr->trch_id);
      }
      /* Invalid number of logical channels */
      else if (TDSUL_DCCH_DTCH_NUM_IS_INVALID(oc_dch_ptr->ndlchan)) {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCH%d with invalid number of DCCH/DTCHs %d",i,oc_dch_ptr->ndlchan);
      }
      /* Valid Transport channel parameters */
      else {
#ifdef FEATURE_DUMMY_SIGNALING
        #error code not present
#endif
        /* Pointernal to internal DCH config info */
        int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(num_active_ul_dchs);

        id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(oc_dch_ptr->trch_id);
        id_to_idx_ptr->valid = TRUE;
        id_to_idx_ptr->idx = num_active_ul_dchs;

        TDSMAC_MSG3(MSG_LEGACY_LOW,"DCH%d Configuration: TrCh ID %d, # of DCCH/DTCHs = %d",
              i, oc_dch_ptr->trch_id, oc_dch_ptr->ndlchan);

        /* Save Transport channel ID */
        int_tc_ptr->trch_id = oc_dch_ptr->trch_id;
        /* Indicates if the domain is cs or ps. Used when the ciphering is enabled*/
        int_tc_ptr->cs_or_ps_domain = (uint8)oc_dch_ptr->cs_or_ps_domain;
        /* Increment the active number of DCHs */
        num_active_ul_dchs++;

        /* # of DCCH/DTCHs mapped to this transport channel*/
        ndlchan = oc_dch_ptr->ndlchan;

        /* Process ordered config data of each dedicated logical channel */
        for (j=0; j <ndlchan; j++){

          /* Pointer to ordered config data of DCCH/DTCH */
          oc_dlcptr = &oc_dch_ptr->dlchan_info[j];

          /* DCCH/DTCh Ordered config pointer is NULL */
          if (oc_dlcptr == NULL) { /*lint !e774*/
            TDSMAC_MSG0(MSG_LEGACY_HIGH,"DCCH/DTCH%d config pointer is NULL");
          }
          /* Invalid RLC ID */
          else if (TDSUL_RLC_ID_IS_INVALID(oc_dlcptr->rlc_id)) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC ID %d",j,oc_dlcptr->rlc_id);
          }
          /* Invalid MAC ID */
          else if ((ndlchan >1) && (TDSUL_MAC_ID_IS_INVALID(oc_dlcptr->mac_id))) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid MAC ID %d",j,oc_dlcptr->mac_id);
          }
          /* Invalid RB ID */
          else if (TDSUL_RB_ID_IS_INVALID(oc_dlcptr->rb_id)) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RB ID %d",j,oc_dlcptr->rb_id);
          }
          /* Invalid MAC Priority */
          else if (TDSUL_LC_PRIORITY_IS_INVALID(oc_dlcptr->priority)) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid priority %d",j,oc_dlcptr->priority);
          }
          /* Invalid RLC Mode */
          else if (TDSUL_RLC_MODE_IS_INVALID(oc_dlcptr->rlc_mode)){
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"DCCH/DTCH%d with invalid RLC Mode %d",j,oc_dlcptr->rlc_mode);
          }
          /* All Parameters are valid */
          else {
#ifdef FEATURE_DUMMY_SIGNALING
            #error code not present
#endif
            /* Internal logical channel config info pointer */
            int_lc_ptr = TDSGET_UL_LC_INFO_PTR(oc_dlcptr->rlc_id);

            /* Clear header value */
            memset (hdrval, 0, TDSMAC_UL_MAX_HDR_SIZE);

            /* Process Dedicated Logical Channel Config Info */
            TDSDCCH_DTCH_HDR_ON_UL_DCH

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

            int_lc_ptr->rlc_size_info.all_tfi_valid = TRUE;

            /* Save Transport channel index */
            int_lc_ptr->trch_idx = i;

            // Get pointer to the transport channel trch_idx with priority level priority
            priority_info_ptr = TDSGET_UL_TRCH_PRIORITY_INFO_PTR(int_lc_ptr->trch_idx, oc_dlcptr->priority);

            // Populate the logical channel id which has this priority.
            priority_info_ptr->lc_id[priority_info_ptr->num_lc] = oc_dlcptr->rlc_id;

            priority_info_ptr->num_lc++;

            TDSMAC_MSG3(MSG_LEGACY_MED,"Pri %d, NChan %d, trch_idx %d",
            oc_dlcptr->priority,priority_info_ptr->num_lc,int_lc_ptr->trch_idx);

            /* Save the rlc size restrictions information */
            /*memcpy(&int_lc_ptr->rlc_size_info,&oc_dlcptr->rlc_size_info,
                                    sizeof(tdsmac_ul_rlc_size_info_type));*/
            memscpy(&int_lc_ptr->rlc_size_info,sizeof(tdsmac_ul_rlc_size_info_type),&oc_dlcptr->rlc_size_info,
                                    sizeof(tdsmac_ul_rlc_size_info_type));
            /* RLC Header Length */
            if (int_lc_ptr->rlc_mode == UE_MODE_TRANSPARENT)
            {
              int_lc_ptr->rlc_hdr_length = 0;
              tdsmac_tm_bearers_count++;
            }
            else if (int_lc_ptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
              int_lc_ptr->rlc_hdr_length = 8;
            else
              int_lc_ptr->rlc_hdr_length = 16;

            /* Save the Logical channel ID in transport channel information */
            /* Update logical channel count of transport channel info */
            int_tc_ptr->rlc_id[int_tc_ptr->nlchan] = oc_dlcptr->rlc_id;
            int_tc_ptr->nlchan++;

            if(rlc_count < TDSUE_MAX_UL_LOGICAL_CHANNEL)
            {
              tdsmac_ul_lc_info.rlc_id[rlc_count++] = oc_dlcptr->rlc_id;
              tdsmac_ul_lc_info.nchan = rlc_count;
            }

            TDSMAC_MSG3(MSG_LEGACY_LOW,"RLC%d:HdrSize= %d Hdr[0]= %x",
                  oc_dlcptr->rlc_id,
                  int_lc_ptr->mac_hdr_size,
                  int_lc_ptr->mac_hdr_value[0]);
            TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[1]= %x Hdr[2]= %x Hdr[3]= %x",
                  int_lc_ptr->mac_hdr_value[1],
                  int_lc_ptr->mac_hdr_value[2],
                  int_lc_ptr->mac_hdr_value[3]);
            TDSMAC_MSG3(MSG_LEGACY_LOW,"       :Hdr[4]= %x RB ID= %d RLC Mode= %d",
                  int_lc_ptr->mac_hdr_value[4],
                  int_lc_ptr->rb_id,
                  int_lc_ptr->rlc_mode);
          }
        }
      }
    }
    }
}

/*===============================================================================================
FUNCTION NAME tdsmac_ul_check_dch_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a UL-DCH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_ul_check_dch_trch(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;
  tdsul_lc_info_type *int_lc_ptr;

  /* Get the logical channel ptr corresponding to the rlc_id  */
  int_lc_ptr = TDSGET_UL_LC_INFO_PTR(rlc_id);

  if(int_lc_ptr == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"mac_ul_check_dch_trch: GET_UL_LC_INFO_PTR returns NULL");
  }
  /* Check if given RB ID mactch logical channel's RB ID */
  else if(int_lc_ptr->rb_id == rb_id)
  {
    return_trch_val = TDSCDMA_TRCH_DCH;
  }
  else
  { 
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"mac_ul_check_dch_trch: rrc_rb_id (%d) does not match l2_rb_id",rb_id,int_lc_ptr->rb_id); 
  }
  return return_trch_val;
}
