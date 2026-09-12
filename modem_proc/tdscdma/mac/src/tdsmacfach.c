/*==================================================================================================

                                          F A C H   H A N D L E R


GENERAL DESCRIPTION

  This module contains functions to process FACH configuration data

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


==================================================================================================*/

/*=================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macfach.c_v   1.8   12 Mar 2002 15:42:00   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacfach.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   ---------------------------------------------------------------------------------
10/08/10  cyx   Change TDSPROCESS_BCCH_TB_ON_FACH, TDSPROCESS_CCCH_TB_ON_FACH,
				TDSPROCESS_CTCH_TB_ON_FACH, TDSPROCESS_DCCH_DTCH_TB_WITH_URNTI_ON_FACH,
				TDSPROCESS_DCCH_DTCH_TB_WITH_CRNTI_ON_FACH to function with the same name
				change tdsprocess_fach_tbs_hdr to fix mac header in TDD mode
10/08/10  cyx   fix  C/T always exist when DTCH/DCCH to FACH
10/08/10  guo   Initial release Upmerged WCDMA to TDS

==================================================================================================*/

/*==================================================================================================
                     INCLUDE FILES
==================================================================================================*/
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "tdsmacrrcif.h"
#include  "tdsmacrlcif.h"
#include  "tdsl2dltask.h"
#include  "tdsmacinternal.h"
#include  "tdsmacdl.h"
#include  "tdsmacfach.h"
#include  "log_codes_tdscdma.h"
/*==================================================================================================
                            DATA DECLARATIONS
==================================================================================================*/

/* Number of active FACHs */
uint8 tdsnum_active_fachs;

/*==================================================================================================
MACRO NAME    TDSPROCESS_BCCH_FACH_CONFIG

DESCRIPTION   This macro processes configuration info of BCCH that is maaped to FACH
==================================================================================================*/
#define   TDSPROCESS_BCCH_FACH_CONFIG\
\
  /* Initially mark BCCH as disabled */ \
  int_fach_ptr->bcch_enable = FALSE;  \
\
    /* BCCH is enabled */ \
    if (oc_fach_ptr->bcch_enable){  \
\
        /* Invalid BCCH ID */ \
        if (TDSDL_RLC_ID_IS_INVALID(oc_fach_ptr->bcch_id)){  \
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid BCCH ID %d",oc_fach_ptr->bcch_id);  \
    } \
    /* Process BCCH/FACH Config Data */ \
    else {  \
      /* Set BCCH Enable flag to TRUE */  \
        int_fach_ptr->bcch_enable = TRUE; \
\
      /* Save BCCH ID */  \
        int_fach_ptr->bcch_id = oc_fach_ptr->bcch_id; \
\
        } \
    }

/*==================================================================================================
MACRO NAME    TDSPROCESS_CCCH_FACH_CONFIG

DESCRIPTION   This macro processes configuration info of CCCH that is maaped to FACH
==================================================================================================*/
#define   TDSPROCESS_CCCH_FACH_CONFIG\
\
  /* Initially mark the CCCH as disabled */ \
  int_fach_ptr->ccch_enable = FALSE;  \
\
    /* Ordered to enable CCCH */  \
    if (oc_fach_ptr->ccch_enable){  \
\
        /* Invalid CCCH ID */ \
        if (TDSDL_RLC_ID_IS_INVALID(oc_fach_ptr->ccch_id)){  \
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid CCCH ID %d",oc_fach_ptr->ccch_id);  \
    } \
    /* Save CCCH info in internal config data table */  \
    else {  \
      /* Set CCCH Enable flag */  \
      int_fach_ptr->ccch_enable = TRUE; \
\
      /* Save CTCH ID */  \
      int_fach_ptr->ccch_id = oc_fach_ptr->ccch_id; \
        } \
    }

/*==================================================================================================
MACRO NAME    TDSPROCESS_CTCH_FACH_CONFIG

DESCRIPTION   This macro processes configuration info of CTCH that is maaped to FACH
==================================================================================================*/
#define   TDSPROCESS_CTCH_FACH_CONFIG\
\
  /* Initially mark the CTCH as disabled */ \
  int_fach_ptr->ctch_enable = FALSE;  \
\
    /* CTCH is enabled */ \
    if (oc_fach_ptr->ctch_enable){  \
\
    /* CTCH ID is valid */  \
    if (TDSDL_RLC_ID_IS_INVALID(oc_fach_ptr->ctch_id)){  \
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid CTCH ID %d",oc_fach_ptr->ctch_id); \
    } \
    else {  \
      /* Set CTCH as enabled */ \
        int_fach_ptr->ctch_enable = TRUE; \
\
      /* Save CTCH ID */  \
      int_fach_ptr->ctch_id = oc_fach_ptr->ctch_id; \
\
    } \
  }

/*==================================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_fach_config

DESCRIPTION   This function processes config info of DCCH/DTCH that is maaped to FACH
==================================================================================================*/
void  tdsprocess_dcch_dtch_fach_config
  (
/* Ordered FACH config data pointer */
  tdsmac_dl_fach_trch_config_type      *oc_fach_ptr,

/* Internal FACH Config data pointer */
  tdsfach_config_info_type             *int_fach_ptr
  )
{
  boolean valid = FALSE;
  uint8 i;
  uint8 tbl_index = 0;

  /* Pointer to ordered DCCH/DTCH config data */
  tdsmac_dl_ded_logch_config_type    *oc_dlc_ptr;

  /* Number of DCCH/DTCHs */
  int_fach_ptr->num_dlc = oc_fach_ptr->ndlchan;

  /* Set Dedicated Logical Channel Enable flag to FALSE and mark all dedicated logical
     channels as disabled */
  int_fach_ptr->dlc_enable = FALSE;
  for (i=0; i < MAX_DED_LOGCHAN_PER_TRCH; i++)

    int_fach_ptr->dlcinfo[i].enable = FALSE;

  /* Process each DCCH/DTCH config information */
  for (i=0 ; i < oc_fach_ptr->ndlchan; i++)
  {

    valid = FALSE;

    /* Pointer to the ordered DCCH/DTCH config data */
    oc_dlc_ptr = &oc_fach_ptr->dlchan_info[i];

    /* Invalid RLC ID */
    if (TDSDL_RLC_ID_IS_INVALID(oc_dlc_ptr->rlc_id))
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"invalid DCCH/DTCH%d ID %d",i,oc_dlc_ptr->rlc_id);
    }
    /* Only one DCCH/DTCH is mapped to FACH */
    else if (int_fach_ptr->num_dlc == 1)
    {
      tbl_index = 0;
      valid = TRUE;
    }
    /* Multiple DCCH/DTCHs are mapped to FACH */
    else
    {
      /* MAC ID is invalid */
      if (TDSDL_MAC_ID_IS_INVALID(oc_dlc_ptr->mac_id))
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid MAC ID %d",oc_dlc_ptr->mac_id);
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
      /* Save DCCH/DTCH info in FACH CONFIG table */
      int_fach_ptr->dlc_enable = TRUE;
      int_fach_ptr->dlcinfo[tbl_index].enable = TRUE;
      int_fach_ptr->dlcinfo[tbl_index].rlc_id = oc_dlc_ptr->rlc_id;
      TDSMAC_MSG1(MSG_LEGACY_LOW,"FACH/DCCH or DTCH ID = %d",oc_dlc_ptr->rlc_id);
    }
  }
}


/*============================================================================================
FUNCTION NAME   tdsprocess_bcch_tb_on_fach

DESCRIPTION     This function processes header info of FACH TB that is mapped to BCCH
============================================================================================*/
boolean    tdsprocess_bcch_tb_on_fach
(
/* The number of tb in tbs  */
    uint8   tb_num,

/* Pointer to internal FACH config info */
    tdsfach_config_info_type   *int_fach_ptr,

/* rlc_id */
    rlc_lc_id_type             *rlc_id_ptr,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset_ptr
)
{
  /* tb valid flag */
    boolean                    valid = FALSE;

  /* If BCCH is enabled, set L1 parameters */ 
  if (int_fach_ptr->bcch_enable == TRUE) 
  {  
    /* RLC PDU Offset */
    *rlc_pdu_offset_ptr = 3;

    /* RLC ID */
    *rlc_id_ptr = int_fach_ptr->bcch_id;

    valid = TRUE;
  }
  /* BCCH is not enabled */
  else
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"TB%d BCCH not enabled",tb_num );
  }
  return valid;
}



/*============================================================================================
FUNCTION NAME   tdsprocess_ccch_tb_on_fach

DESCRIPTION     This function processes header info of FACH TB that is mapped to CCCH
============================================================================================*/
boolean   tdsprocess_ccch_tb_on_fach
(
/* The number of tb in tbs  */
    uint8   tb_num,

/* Pointer to internal FACH config info */
    tdsfach_config_info_type   *int_fach_ptr,

/* rlc_id */
    rlc_lc_id_type             *rlc_id_ptr,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset_ptr
)
{
  /* tb valid flag */
  boolean                    valid = FALSE;
  
  /* If CCCH is anabled, set L1 parameters */
  if (int_fach_ptr->ccch_enable == TRUE) 
  {
  
    /* RLC PDU Offset */
    *rlc_pdu_offset_ptr = 3;
    
    /* RLC ID */  \
    *rlc_id_ptr = int_fach_ptr->ccch_id;
    
    valid = TRUE;
  }
  /* CCCH not enabled */
  else 
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"TB%d CCCH not enabled",tb_num );
  }
  return valid;
}

/*============================================================================================
FUNCTION NAME   tdsprocess_ctch_tb_on_fach

DESCRIPTION     This function processes header info of FACH TB that is mapped to CTCH
============================================================================================*/
boolean   tdsprocess_ctch_tb_on_fach
(
/* The number of tb in tbs  */
    uint8   tb_num,

/* Pointer to internal FACH config info */
    tdsfach_config_info_type   *int_fach_ptr,

/* rlc_id */
    rlc_lc_id_type             *rlc_id_ptr,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset_ptr
)
{
  /* tb valid flag */
  boolean                    valid = FALSE;

  /* If CTCH is anabled, set L1 parameters */
  if (int_fach_ptr->ctch_enable == TRUE) 
  {

    /* RLC PDU Offset */
    *rlc_pdu_offset_ptr = 3;

    /* RLC ID */  \
    *rlc_id_ptr = int_fach_ptr->ctch_id;

    valid = TRUE;
  }
  /* CTCH not enabled */
  else 
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"TB%d CTCH not enabled",tb_num );
  }
    return valid;
}


/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti

DESCRIPTION     This function processes urnti in header info of FACH TB that is mapped to
        DCCH/DTCH with URNTI
                                                                            *********
        symbol:    TCTF("#"),UE_ID type("%"),UE_ID("$"),C/T("@"),DATA("?"), * URNTI *
                                                                            *********
        mac header:  

        |<- header[0]   ->|  |<- header[1]   ->| |<- header[2]   ->| |<- header[3]   ->| |<- header[4]   ->| |<- header[5]   ->|
        | 7 6 5 4 3 2 1 0 |  | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 |
                       **********************************************************************************
        | # # # # # % %*$ |  | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $*@ | | @ @ @ ? ? ? ? ?|
                       **********************************************************************************
                        31  ----------------------------URNTI----------------------------------------> 0

                          ************************************************************************
        header[0] << 31 : * $ 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 |0 0 0 0 0 0 0 0 *
              +           ************************************************************************
        header[1] << 23 : * 0 $ $ $ $ $ $ $ | $ 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 |0 0 0 0 0 0 0 0 *
              +           ************************************************************************
        header[2] << 15 : * 0 0 0 0 0 0 0 0 | 0 $ $ $ $ $ $ $ | $ 0 0 0 0 0 0 0 |0 0 0 0 0 0 0 0 *
              +           ************************************************************************
        header[3] << 7  : * 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 | 0 $ $ $ $ $ $ $ |$ 0 0 0 0 0 0 0 *
              +           ************************************************************************
        header[4] >> 1  : * 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 |0 $ $ $ $ $ $ $ *
                          ************************************************************************
        = URNTI           * $ $ $ $ $ $ $ $ | $ $ $ $ $ $ $ $ | $ $ $ $ $ $ $ $ |$ $ $ $ $ $ $ $ *
                          ************************************************************************

============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr
)
{
    uint32        urnti;

    /* tb valid flag */
    boolean                       valid  = FALSE;

    /* URNTI field of TB */
    urnti = (uint32)((tb_hdr_ptr->tb_header[0] << 31) + (tb_hdr_ptr->tb_header[1] << 23) +
      (tb_hdr_ptr->tb_header[2] << 15) + (tb_hdr_ptr->tb_header[3] << 7) +
      (tb_hdr_ptr->tb_header[4] >> 1));

  /* DL TB URNTI is invalid */
    if (TDSDL_TB_URNTI_IS_INVALID(urnti))
    {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with invalid URNTI %x",tb_num,urnti);
    }
    else
    {
        valid  = TRUE;
    }
    return valid;
}
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct

DESCRIPTION     This function processes C/T in header info of FACH TB that is mapped to
        DCCH/DTCH with urnti
                                                                            *********
        symbol:    TCTF("#"),UE_ID type("%"),UE_ID("$"),C/T("@"),DATA("?"), *  C/T  *
                                                                            *********
        mac header:  

        |<- header[0]   ->|  |<- header[1]   ->| |<- header[2]   ->| |<- header[3]   ->| |<- header[4]   ->| |<- header[5]   ->|
        | 7 6 5 4 3 2 1 0 |  | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 |
                                                                                                       *************
        | # # # # # % %$ |  | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $*@ | | @ @ @*? ? ? ? ?|
                                                                                                       *************
                                                                          
                                      *******************
           (header[4] & 0x01) << 3 :  * 0 0 0 0 @ 0 0 0 *
                |                     *******************
           (header[5] & 0xe0)>>5    : * 0 0 0 0 0 @ @ @ *
                                      *******************
              = *ct_field_ptr       : * 0 0 0 0 @ @ @ @ *
                                      *******************

============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr,
    /* ct field info */
    uint8                    *ct_field_ptr
)
{
    /* tb valid flag */
    boolean                   valid = FALSE;

    /* C/T field of TB */
    *ct_field_ptr = ((uint8)(((tb_hdr_ptr->tb_header[4] & 0x01) << 3) |
					((tb_hdr_ptr->tb_header[5] & 0xe0)>>5)));

    if (TDSDL_CT_FLD_IS_INVALID(int_fach_ptr->num_dlc,*ct_field_ptr))
    {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with invalid C/T %x",tb_num ,*ct_field_ptr);
    }
    else
    {
        valid  = TRUE;
    }
    return valid;
}
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_rlc

DESCRIPTION     This function processes rlc info in header info of FACH TB that is mapped to
        DCCH/DTCH with urnti
============================================================================================*/
boolean tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_rlc
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to internal FACH config info */
    tdsfach_config_info_type   *int_fach_ptr,

    /* ct field info */
    uint8                    ct_field,

    /* rlc_id */
    rlc_lc_id_type             *rlc_id_ptr,

    /* rlc header offset  */
    uint8                      *rlc_pdu_offset_ptr
 )
{
    tdsdlc_fach_config_info_type *dlcptr = NULL;

    /* tb valid flag */
    boolean                    valid = FALSE;

    /* Pointer to internal DCCH/DTCH config info */
    dlcptr = &int_fach_ptr->dlcinfo[ct_field];

    /* DCCH/DTCH is enabled */
    if (dlcptr->enable == TRUE)
    {

     /* RLC PDU Offset */
      *rlc_pdu_offset_ptr = 43;

     /* RLC ID */
      *rlc_id_ptr = dlcptr->rlc_id;

      valid = TRUE;
    }
    /* DCCH/DTCH not enabled */
    else
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with disabled DCCH/DTCH",tb_num ,ct_field);
    }
    return valid;

}

/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti

DESCRIPTION     This function processes crnti in header info of FACH TB that is mapped to
        DCCH/DTCH with CRNTI
                                                                            *********
        symbol:    TCTF("#"),UE_ID type("%"),UE_ID("$"),C/T("@"),DATA("?"), * CRNTI *
                                                                            *********
        mac header:  

        |<- header[0]   ->|  |<- header[1]   ->| |<- header[2]   ->| |<- header[3]   ->|
        | 7 6 5 4 3 2 1 0 |  | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 |
                       ******************************************
        | # # # # # % %*$ |  | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $*@ | | @ @ @ ? ? ? ? ?|
                       ******************************************
                        15  -------------CRNTI---------------> 0


                         *************************************
        header[0] << 15 :* $ 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 *
              +          *************************************
        header[1] << 7 : * 0 $ $ $ $ $ $ $ | $ 0 0 0 0 0 0 0 *
              +          *************************************
        header[2] >> 1 : * 0 0 0 0 0 0 0 0 | 0 $ $ $ $ $ $ $ *
                         *************************************
        = CRNTI          * $ $ $ $ $ $ $ $ | $ $ $ $ $ $ $ $ *
                         *************************************
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti
(
    /* The number of tb in tbs  */
    uint8   tb_num,

    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr 
)
{
    uint16        crnti;
    /* tb valid flag */
    boolean                    valid = FALSE;

    /* CRNTI of TB */
    crnti = (uint16)((tb_hdr_ptr->tb_header[0] << 15) + (tb_hdr_ptr->tb_header[1] <<  7) +
      (tb_hdr_ptr->tb_header[2] >>  1));

    /* If C-RNTI is valid, check the C/T field */ \
    if (TDSDL_TB_CRNTI_IS_INVALID(crnti))
    {
	    TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with invalid CRNTI %x",tb_num ,crnti);
    }
    else
    {
        valid  = TRUE;
    }
    return valid;
}

/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct

DESCRIPTION     This function processes C/T in header info of FACH TB that is mapped to
        DCCH/DTCH with crnti
                                                                            *********
        symbol:    TCTF("#"),UE_ID type("%"),UE_ID("$"),C/T("@"),DATA("?"), * C/T   *
                                                                            *********
        mac header:  

        |<- header[0]   ->|  |<- header[1]   ->| |<- header[2]   ->| |<- header[3]   ->|
        | 7 6 5 4 3 2 1 0 |  | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 | | 7 6 5 4 3 2 1 0 |
                                                                *************
        | # # # # # % % $ |  | $ $ $ $ $ $ $ $ | | $ $ $ $ $ $ $*@ | | @ @ @*? ? ? ? ?|
                                                                *************
                                                                          
                                    *******************
           (header[2] & 0x01) << 3 :* 0 0 0 0 @ 0 0 0 *
              |                     *******************
           (header[3] & 0xe0) >>5  :* 0 0 0 0 0 @ @ @ *
                                    *******************
         = C/T                      * 0 0 0 0 @ @ @ @ *
                                    *******************
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr,
    /* ct field info */
    uint8                    *ct_field_ptr
 )
{
    /* tb valid flag */
    boolean                    valid = FALSE;

    *ct_field_ptr = ((uint8)(((tb_hdr_ptr->tb_header[2] & 0x01) << 3) |
					((tb_hdr_ptr->tb_header[3] & 0xe0)>>5)));

    if (TDSDL_CT_FLD_IS_INVALID(int_fach_ptr->num_dlc,*ct_field_ptr))
    {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with invalid C/T %x",tb_num ,*ct_field_ptr);
    }
    else
    {
       valid  = TRUE;
    }
    return    valid;
}

/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_rlc

DESCRIPTION     This function processes rlc info in header info of FACH TB that is mapped to
        DCCH/DTCH with crnti
============================================================================================*/
boolean tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_rlc
(
    /* The number of tb in tbs  */
    uint8   tb_num,

    /* Pointer to internal FACH config info */
    tdsfach_config_info_type   *int_fach_ptr,

    /* ct field info */
    uint8                     ct_field,

    /* rlc_id */
    rlc_lc_id_type             *rlc_id_ptr,

    /* rlc header offset  */
    uint8                      *rlc_pdu_offset_ptr
 )
{
    tdsdlc_fach_config_info_type *dlcptr = NULL;
    
    /* tb valid flag */
    boolean                    valid = FALSE;;

    dlcptr = &int_fach_ptr->dlcinfo[ct_field]; 
    /* DCCH/DTCH is enabled */
    if (dlcptr->enable == TRUE)
    {

      /* RLC PDU Offset */
      *rlc_pdu_offset_ptr = 27;

      /* RLC ID */
      *rlc_id_ptr = dlcptr->rlc_id;

      valid = TRUE;
    } 
    /* DCCH/DTCH not enabled */
    else
    {

      TDSMAC_MSG2(MSG_LEGACY_HIGH,"TB%d with disabled DCCH/DTCH",tb_num ,ct_field);
    }
    return    valid;
}
/*======================================================================================================
FUNCTION NAME   tdsprocess_fach_tbs_hdr

DESCRIPTION   This function processes FACH TBS Header information
======================================================================================================*/
void  tdsprocess_fach_tbs_hdr
  (
/* First TB ID */
  uint8 tb_id,

/* Pointer to internal FACH config info */
  tdsfach_config_info_type   *int_fach_ptr,

/* Pointer to FACH TBS Header Info */
  tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
  )
{
  uint8       i, num_tb, first_hdr_byte, ct_field, rlc_pdu_offset = 0;
  boolean       valid, status = TRUE;
  rlc_lc_id_type    rlc_id = 0;


  /* Pointer to DL TB info */
  tdsdl_tb_info_type       *tdsdl_tb_info_ptr;

  /* RLC Header Info */
  tdsrlc_dl_tb_hdr_info_type   rlc_hdr_info;

  /* RLC Ciphering Info */
  tdsrlc_dl_tb_cipher_info_type  rlc_cipher_info;

  /* Pointer to TB Ciphering Parameters */
  tdsl1_dl_ciphering_param_struct_type *tb_cipher_ptr;

  /* Pointer to Transport block header information */
  tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr;

    if (tbs_hdr_ptr==NULL)
    {
        TDSMAC_MSG0(MSG_LEGACY_HIGH,"TBS_HDR_PTR NULL\n");
        return;
    }

  /* Number of Transport Blocks */
  num_tb = tbs_hdr_ptr->num_tb;

  /* Process each transport block */
  for (i=0 ; i <num_tb; i++)
  {
    /* Set tb_in_tm_mode to FALSE */
    tbs_hdr_ptr->tb_in_tm_mode = FALSE;
    /* Get pointer to transport block information */
    tb_hdr_ptr = &tbs_hdr_ptr->tb_hdr_info[i];

    /* TB Header pointer is NULL */
    if (tb_hdr_ptr == NULL) /*lint !e774*/
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"TB%d header pointer is NULL",i);
    }
    else
    {

      /* Initially mark the TB config parameters as invalid */
      valid = FALSE;

      /* Initially mark the ciperhing control flag to FALSE */
      tb_hdr_ptr->ciphering_param.ciphering_control = FALSE;

      tb_hdr_ptr->ciphering_param.ciphering_algo = UE_UMTS_CIPHER_ALGO_UEA0;

      /* Get first byte of header to decide logical channel type */
      first_hdr_byte = tb_hdr_ptr->tb_header[0];

      /* If three MSBits are 000, then it is BCCH */
      if (int_fach_ptr->bcch_enable && ((first_hdr_byte & MSB3_MAC_HDR) == TCTF_BCCH_ON_FACH))
      {
        valid = tdsprocess_bcch_tb_on_fach(i, int_fach_ptr, &rlc_id, &rlc_pdu_offset);
      }
      /* If three MSBits are 001, then it is CCCH */
      else if (int_fach_ptr->ccch_enable && ((first_hdr_byte & MSB3_MAC_HDR) == TCTF_CCCH_ON_FACH))
      {
        valid = tdsprocess_ccch_tb_on_fach(i, int_fach_ptr, &rlc_id, &rlc_pdu_offset);
      }
      /* If three MSBits are 010, then it is CTCH */
      else if (int_fach_ptr->ctch_enable && ((first_hdr_byte & MSB3_MAC_HDR) == TCTF_CTCH_ON_FACH))
      {
        valid = tdsprocess_ctch_tb_on_fach(i, int_fach_ptr, &rlc_id, &rlc_pdu_offset);
      }
      /* if seven MSBits are 0110000, then it is DCCH/DTCH with U-RNTI */
      else if (int_fach_ptr->dlc_enable && ((first_hdr_byte & MSB7_MAC_HDR) == TCTF_UEID_TYPE_DCCH_DTCH_WITH_URNTI_ON_FACH))
      {
        if(TRUE == tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti(i, tb_hdr_ptr))
        {
            if(TRUE == tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct(i, tb_hdr_ptr, &ct_field))
            {
                if(TRUE == tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_rlc(i, int_fach_ptr, ct_field, &rlc_id, &rlc_pdu_offset))
                {
                    valid = TRUE;
                }
            }
        }
      }
      /* if seven MSBits are 0110001, it is DCCH/DTCH with C-RNTI */
      else if (int_fach_ptr->dlc_enable && ((first_hdr_byte & MSB7_MAC_HDR) == TCTF_UEID_TYPE_DCCH_DTCH_WITH_CRNTI_ON_FACH))
      {
        if(TRUE == tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti(i, tb_hdr_ptr))
        {
            if(TRUE == tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct(i, tb_hdr_ptr, &ct_field))
            {
                if(TRUE == tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_rlc(i, int_fach_ptr, ct_field, &rlc_id, &rlc_pdu_offset))
                {
                    valid = TRUE;
                }
            }
        }
        TDSMAC_MSG3(MSG_LEGACY_LOW,"DCCH TB valid %d off %d rlc ID %d", valid, rlc_pdu_offset, rlc_id);
      }

      /* TB parameters are valid */
      if (valid == TRUE)
      {
        /* Get RLC Header info */
        rlc_hdr_info.rlc_id =rlc_id;
        rlc_hdr_info.rlc_pdu_start_offset = rlc_pdu_offset;
        rlc_hdr_info.hdr_ptr = (uint8 *)&tb_hdr_ptr->tb_header[0];
        /*Decode the RLC PDU header*/
        if ((status = tdsrlc_dl_decode_pdu_hdr (&rlc_hdr_info, &rlc_cipher_info)) == TRUE)
        {
          /* Set Ciphering parameters of TB */
          tb_cipher_ptr = &tb_hdr_ptr->ciphering_param;
          tb_cipher_ptr->ciphering_control = FALSE;

          if (rlc_cipher_info.cipher_flag)
          {
            tb_cipher_ptr->ciphering_control = TRUE;

             tb_cipher_ptr->ciphering_algo = rlc_cipher_info.ciphering_algo;
            
            tb_cipher_ptr->ciphering_bit_offset = rlc_cipher_info.cipher_offset;
            tb_cipher_ptr->ciphering_key_id = (uint8)rlc_cipher_info.cipher_key_id;
            tb_cipher_ptr->ciphering_count = rlc_cipher_info.count_c;
            tb_cipher_ptr->ciphering_radio_bearerid = (uint8)rlc_cipher_info.rb_id;
          }
          /* Set other header parameters of TB */
          tb_hdr_ptr->tb_id = (uint8)(tb_id + i);
          tb_hdr_ptr->valid = TRUE;
          tb_hdr_ptr->num_bit_offset_trblk = rlc_pdu_offset;
        }
        else
        {
          /*If the header decoding fails the mark the TB as invalid*/
          tb_hdr_ptr->valid = FALSE;          
        }
  
        /* Save RLC ID in Transport Block info table */
        tdsdl_tb_info_ptr = TDSGET_DL_TB_INFO_PTR(tb_id + i);
        if (status == TRUE)
        {
          tdsdl_tb_info_ptr->valid = TRUE;
          tdsdl_tb_info_ptr->rlc_id = rlc_id;
        }
        else
        {
          tdsdl_tb_info_ptr->valid = FALSE;
        }
      }
    }
  }
}

/*============================================================================================
FUNCTION NAME   tdsprocess_fach_config

DESCRIPTION   This function processes FACH configuration data

============================================================================================*/
void  tdsprocess_fach_config
(
  /* Number of FACHs */
  uint8  num_fachs,

  /* Pointer to ordered FACH configuration data list */
  tdsmac_dl_fach_trch_config_type  *oc_fach_ptr_list[]
)
{
  uint8   i;

  /* ID to Index Table Pointer */
  tdstrch_id_to_idx_map_type     *id_to_idx_ptr;

  /* Pointer to FACH TrCh config info */
  tdsmac_dl_fach_trch_config_type  *oc_fach_ptr;

  /* Pointer to config info in FACH CONFIG table */
  tdsfach_config_info_type     *int_fach_ptr;

  /* Set number of active FACHs to zero */
  tdsnum_active_fachs = 0;

  for (i=0;i<2*MAX_TRCH;i++)
  {
    tdsfach_id_to_idx[i].valid = FALSE;
  }

  /* Invalid number of FACHs */
  if (num_fachs > TDSUE_MAX_TRCH)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid number of FACHs %d",num_fachs);
  }
  /* Process each FACH config data */
  else
  {
    for (i=0; i < num_fachs; i++)
    {
      /* Pointer to the FACH CCTrCh info */
      oc_fach_ptr = oc_fach_ptr_list[i];

      /* FACH ordered config data pointer is NULL */
      if (oc_fach_ptr == NULL)
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"FACH%d config pointer is NULL",i);
      }
      /* Invalid TrCh ID */
      else if (TDSDL_TRCH_ID_IS_INVALID(oc_fach_ptr->trch_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"FACH%d with invalid TrCh ID %d",i,oc_fach_ptr->trch_id);
      }
      /* Invalid CCTrCh ID */
      else if (TDSDL_CCTRCH_ID_IS_INVALID(oc_fach_ptr->cctrch_id))
      {
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"FACH%d with invalid CCTrCh ID",i,oc_fach_ptr->ctch_id);
      }
      /* Process FACH config data */
      else
      {
        /* Pointer to the FACH config table entry */
        int_fach_ptr = &tdsfach_cfg_info[tdsnum_active_fachs];

        /* Get Transport ID to Index map Table */
        id_to_idx_ptr = TDSFACH_ID_TO_IDX_PTR(oc_fach_ptr->trch_id);
        id_to_idx_ptr->valid = TRUE;
        id_to_idx_ptr->idx = tdsnum_active_fachs;

        /* Save CCTrCh ID in internal FACH Config Table */
        int_fach_ptr->cctrch_id = oc_fach_ptr->cctrch_id;
        int_fach_ptr->trch_id = oc_fach_ptr->trch_id;

        TDSMAC_MSG3(MSG_LEGACY_LOW,"FACH%d Config Data : TrChID=%d CCTrChID=%d",
          i, int_fach_ptr->trch_id, int_fach_ptr->cctrch_id);

        /* Process BCCH data */
        TDSPROCESS_BCCH_FACH_CONFIG
          TDSMAC_MSG1(MSG_LEGACY_LOW,"FACH/BCCH id = %d",int_fach_ptr->bcch_id);

        /* Process CCCH data if any */
        TDSPROCESS_CCCH_FACH_CONFIG
          TDSMAC_MSG1(MSG_LEGACY_LOW,"FACH/CCCH id = %d",int_fach_ptr->ccch_id);

        /* Process CTCH data if any */
        TDSPROCESS_CTCH_FACH_CONFIG
          TDSMAC_MSG1(MSG_LEGACY_LOW,"FACH/CTCH id = %d",int_fach_ptr->ctch_id);


        /* Process DCCH/DTCH data */
        tdsprocess_dcch_dtch_fach_config (oc_fach_ptr, int_fach_ptr);

        /* Increment the number of active FACHs */
        tdsnum_active_fachs++;
      }

    }
  }
}

/*===============================================================================================
FUNCTION NAME tdsmac_dl_check_fach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a FACH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_dl_check_fach_trch(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;
  int i, j;

  for(i = 0; i < tdsnum_active_fachs; i++) 
  {

    do 
    {
      /* Case when BCCH is mapped on to FACH */
      if((tdsfach_cfg_info[i].bcch_enable == TRUE) &&
          (tdsfach_cfg_info[i].bcch_id == rlc_id))
      {
          return_trch_val = TDSCDMA_TRCH_FACH;
          break;        
      }
      /* Case when CCCH is mapped on to FACH */
      if((tdsfach_cfg_info[i].ccch_enable == TRUE) &&
          (tdsfach_cfg_info[i].ccch_id == rlc_id))
      {
          return_trch_val = TDSCDMA_TRCH_FACH;
          break;        
      }
      /* Case when CTCH is mapped on to FACH */
      if((tdsfach_cfg_info[i].ctch_enable == TRUE) &&
          (tdsfach_cfg_info[i].ctch_id == rlc_id))
      {
          return_trch_val = TDSCDMA_TRCH_FACH;
          break;        
      }           
    } while (0);

    if(return_trch_val != TDSCDMA_TRCH_INVALID)
    {
      break;
    }

      /* Case when DCCH/DTCH is mapped on to FACH */
    for(j = 0; j < tdsfach_cfg_info[i].num_dlc; j++)
    {
      if((tdsfach_cfg_info[i].dlcinfo[j].enable == TRUE) && 
         (tdsfach_cfg_info[i].dlcinfo[j].rlc_id == rlc_id))
       {
          return_trch_val = TDSCDMA_TRCH_FACH;
          break;
       }
    }
  }

  TDSMAC_MSG1(MSG_LEGACY_HIGH,"function mac_dl_check_fach_trch returning %d",return_trch_val);
  return return_trch_val;
}

