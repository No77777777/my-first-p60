/*=============================================================================================

                                    B C H   H A N D L E R

GENERAL DESCRIPTION

  This module contains functions to :
  - process BCH configuration data
  - process header information of BCH Transport Block Set

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


==============================================================================================*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macbch.c_v   1.4   07 Aug 2001 18:27:24   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacbch.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

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
#include  "tdsmacbch.h"
#include  "log_codes_tdscdma.h"

/*==============================================================================================
FUNCTION NAME   tdsprocess_bch_config

PURPOSE     This function processes BCH configuration data received from RRC.

==============================================================================================*/
void  tdsprocess_bch_config  
(
   /* Number of BCH type CCTrChs */
   uint8    num_bchs,

   /* Pointer to the first BCH Configuration Data */
   tdsmac_dl_bch_cctrch_config_type *oc_bch_info
)
{
   uint8   i;

   /* Pointer to BCH Config information table */
   tdsbch_config_info_type         *int_bch_ptr;

   /* Initially mark all BCHs as disabled */
   for (i=0; i<TDSUE_MAX_CCTRCH; i++) {
      tdsbch_cfg_info[i].bch_enable = FALSE;
   }

   /* Invalid number of BCHs */
   if (num_bchs > TDSMAC_DL_MAX_CCTRCH) {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of BCHs %d",num_bchs);
   }
   /* Ordered BCH Info pointer is NULL */
   else if (oc_bch_info == NULL) {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"BCH ordered config info pointer is NULL");
   }
   /* Valid Number of BCHs */
   else {
      /* Process each BCH */
      for (i=0; i<num_bchs; i++) {

         /* CCTrCh ID is invalid */
         if (TDSDL_CCTRCH_ID_IS_INVALID(oc_bch_info[i].cctrch_id)) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"BCH%d CCTrCh ID is invalid",i,oc_bch_info[i].cctrch_id);
         }
         /* BCCH ID is invalid */
         else if (TDSDL_RLC_ID_IS_INVALID(oc_bch_info[i].bcch_id)) {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"BCH%d BCCH ID %d is invalid",i,oc_bch_info[i].bcch_id);
         }
         /* BCH parameters are valid */
         else {
            /* Pointer to BCH config table entry */
            int_bch_ptr = TDSGET_BCH_INFO_PTR(oc_bch_info[i].cctrch_id);

            /* Save BCH info in config info table */
            int_bch_ptr->bch_enable = TRUE;
            int_bch_ptr->bcch_id = oc_bch_info[i].bcch_id;

            TDSMAC_MSG3(MSG_LEGACY_LOW,"BCH%d: CCTrCh ID = %d BCCH ID %d",
                     i, oc_bch_info[i].cctrch_id, oc_bch_info[i].bcch_id);
         }
      }
   }
}

/*==============================================================================================

FUNCTION NAME   tdsprocess_bch_tbs_hdr

PURPOSE     This function process BCH Transport Block Set Header Information

==============================================================================================*/
void  tdsprocess_bch_tbs_hdr
(
  /* Transport Block ID */
  uint8 tb_id,

  /* Pointer to internal BCH info */
  tdsbch_config_info_type    *int_bch_ptr,

  /* Pointer to Transport Block Set Header Information */
  tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
)
{
  uint8   i;

  /* Pointer to Transport block header information */
  tdsl1_dl_tb_hdr_struct_type    *tb_hdr_ptr;

  /* Pointer to DL Transport Block Info */
  tdsdl_tb_info_type       *tdsdl_tb_info_ptr;

  /* Process each transport block header information */
  for (i=0 ; i <tbs_hdr_ptr->num_tb; i++) {

    /* Transport block header data pointer */
        tb_hdr_ptr = &tbs_hdr_ptr->tb_hdr_info[i];

    /* TB Header Info pointer is NULL */
    if (tb_hdr_ptr == NULL){ /*lint !e774*/
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"TB%d Header Info pointer is NULL");
    }
    else {
      /* Set L1 parameters */
      tb_hdr_ptr->ciphering_param.ciphering_control = FALSE;
      tb_hdr_ptr->tb_id = (uint8)(tb_id + i);
      tb_hdr_ptr->valid = TRUE;
      tb_hdr_ptr->num_bit_offset_trblk = 0;

      /* Save RLC ID in Transport Block info table */
        tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_id + i);
      tdsdl_tb_info_ptr->valid = TRUE;
      tdsdl_tb_info_ptr->rlc_id = int_bch_ptr->bcch_id;

      TDSMAC_MSG3(MSG_LEGACY_LOW,"TB%d:TB ID = %d, RLC ID = %d",
            i, tb_hdr_ptr->tb_id, int_bch_ptr->bcch_id);
    }
  }
}

