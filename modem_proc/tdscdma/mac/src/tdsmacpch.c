
/*==============================================================================================

                P C H   H A N D L E R

GENERAL DESCRIPTION

  This module contains functions to :
  - process PCH configuration data
  - process PCH Transport Block Set Header information

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


==============================================================================================*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macpch.c_v   1.4   07 Aug 2001 18:29:14   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacpch.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   ----------------------------------------------------
10/24/10  guo   Initial release Upmerged WCDMA to TDS

==============================================================================================*/

/*==============================================================================================
                     INCLUDE FILES
==============================================================================================*/
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "tdsmacrrcif.h"
#include  "tdsl2dltask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacdl.h"
#include  "tdsmacpch.h"
#include  "log_codes_tdscdma.h"

/*==============================================================================================

FUNCTION NAME   tdsprocess_pch_config

DESCRIPTION   This function processes PCH Configuration Data received from RRC

==============================================================================================*/
void  tdsprocess_pch_config
(
  /* Number of PCHs */
  uint8   num_pchs,

  /* Pointer to ordered PCH config info */
  tdsmac_dl_pch_cctrch_config_type *oc_pch_info
)
{
    uint8   i;

  /* Pointer to internal PCH config info */
  tdspch_config_info_type  *int_pch_ptr;

  /* Initially mark all PCHs as disabled */
  for (i=0; i<TDSUE_MAX_CCTRCH; i++)
  {
    tdspch_cfg_info[i].pch_enable = FALSE;
  }

  /* PCH ordered config data pointer is NULL */
  if (oc_pch_info == NULL)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"PCH%d ordered config pointer is NULL",i);
  }
  /* Invalid number of CCTrChs */
  else if (num_pchs > TDSMAC_DL_MAX_CCTRCH)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of PCH Type CCTrChs",num_pchs);
  }
  /* Process each CCTrCh */
  else
  {
    for (i=0; i<num_pchs; i++)
    {
      /* Invalid CCTrCh ID */
      if (TDSDL_CCTRCH_ID_IS_INVALID(oc_pch_info[i].cctrch_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"PCH%d with invalid CCTrCh ID",i,oc_pch_info[i].cctrch_id);
      }
      /* Invalid RLC ID */
      else if (TDSDL_RLC_ID_IS_INVALID(oc_pch_info[i].pcch_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"PCH%d with invalid PCCH ID %d",i,oc_pch_info[i].pcch_id);
      }
      /* Save PCH info in local table */
      else
      {
        /* Pointer to the PCH config table entry */
        int_pch_ptr = TDSGET_PCH_INFO_PTR(oc_pch_info[i].cctrch_id);

        /* Save PCH info in config info table */
        int_pch_ptr->pch_enable = TRUE;
        int_pch_ptr->pcch_id = oc_pch_info[i].pcch_id;

        TDSMAC_MSG3(MSG_LEGACY_LOW,"PCH%d: CCTrCh ID=%d PCCH ID=%d",
              i, oc_pch_info[i].cctrch_id, oc_pch_info[i].pcch_id);
      }
    }
  }
}

/*===========================================================================================
FUNCTION    tdsprocess_pch_tbs_hdr

DESCRIPTION   This function processes PCH TBS Header information
        Called by Layer 1 in interrupt context
============================================================================================*/
void  tdsprocess_pch_tbs_hdr
(
  /* First PCH TB ID */
  uint8 tb_id,

  /* Pointer to internal PCH info */
  tdspch_config_info_type      *int_pch_ptr,

  /* Pointer to PCH TBS Header Info */
  tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
)
{
  uint8           i;

  /* Pointer to internal DL TB Info */
  tdsdl_tb_info_type       *tdsdl_tb_info_ptr;

  /* Pointer to Transport block header information */
  tdsl1_dl_tb_hdr_struct_type    *tb_hdr_ptr;

  /* Process each TB */
  for (i=0 ; i <tbs_hdr_ptr->num_tb; i++)
  {
    /* Transport block header data pointer */
    tb_hdr_ptr = &tbs_hdr_ptr->tb_hdr_info[i];

    if (tb_hdr_ptr == NULL) /* lint !e774 */
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"TB%d Header pointer is NULL",i);
    }
    else
    {
      /* Set L1 parameters */
      tb_hdr_ptr->ciphering_param.ciphering_control = FALSE;
      tb_hdr_ptr->tb_id = (uint8)(tb_id + i);
      tb_hdr_ptr->valid = TRUE;
      tb_hdr_ptr->num_bit_offset_trblk = 0;

      /* Save RLC ID of the transport block in Transport Block info table */
      tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_id+i);
      tdsdl_tb_info_ptr->valid = TRUE;
      tdsdl_tb_info_ptr->rlc_id = int_pch_ptr->pcch_id;

      TDSMAC_MSG3(MSG_LEGACY_LOW,"TB%d:TB ID=%d, RLC ID=%d", i, tb_hdr_ptr->tb_id,
               int_pch_ptr->pcch_id);
    }
  }
}

