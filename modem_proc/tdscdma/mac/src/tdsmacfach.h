#ifndef MACFACH_H
#define MACFACH_H
/*=====================================================================

     F A C H    C O N F I G U R A T I O N    H E A D E R   F I L E

GENERAL DESCRIPTION

  This module contains the declaration of the functions and data types
  defined in "macfachcfg.c"

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macfach.h_v   1.2   09 May 2001 18:15:14   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacfach.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------
10/22/10   cyx     declare tdsprocess_bcch_tb_on_fach,tdsprocess_ccch_tb_on_fach,tdsprocess_ctch_tb_on_fach,
                   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti,
                   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct,
                   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_rlc,
                   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti,
                   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct,
                   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_rlc,
                   MSB3_MAC_HDR,MSB7_MAC_HDR,TCTF_BCCH_ON_FACH,TCTF_CCCH_ON_FACH,
                   TCTF_CTCH_ON_FACH,TCTF_UEID_TYPE_DCCH_DTCH_WITH_URNTI_ON_FACH,
                   TCTF_UEID_TYPE_DCCH_DTCH_WITH_CRNTI_ON_FACH
10/22/10   guo     Initial release Upmerged WCDMA to TDS


=====================================================================*/

/*============================================================================================
MACRO NAME    MSB3_MAC_HDR

DESCRIPTION   This macro decides 3 MSB of MAC header
============================================================================================*/
#define   MSB3_MAC_HDR 0xe0

/*============================================================================================
MACRO NAME    MSB7_MAC_HDR

DESCRIPTION   This macro decides 7 MSB of MAC header
============================================================================================*/
#define   MSB7_MAC_HDR 0xfe

/*============================================================================================
MACRO NAME    TCTF_BCCH_ON_FACH

DESCRIPTION   This macro decides TCTF of MAC header  when BCCH is mapped to FACH
        - TCTF     : 3-bits long and its value = 000 ,
        byte value is  000#####
============================================================================================*/
#define   TCTF_BCCH_ON_FACH 0x0 

/*============================================================================================
MACRO NAME    TCTF_CCCH_ON_FACH

DESCRIPTION   This macro decides TCTF of MAC header  when CCCH is mapped to FACH
        - TCTF     : 3-bits long and its value = 001, 
        byte value is  001#####
============================================================================================*/
#define   TCTF_CCCH_ON_FACH 0x20 

/*============================================================================================
MACRO NAME    TCTF_CTCH_ON_FACH

DESCRIPTION   This macro decides TCTF of MAC header  when CTCH is mapped to FACH
        - TCTF     : 3-bits long and its value = 010, 
        byte value is  010#####
============================================================================================*/
#define   TCTF_CTCH_ON_FACH 0x40 


/*============================================================================================
MACRO NAME    TCTF_UEID_TYPE_DCCH_DTCH_WITH_URNTI_ON_FACH

DESCRIPTION    This macro decides 7 MSB of MAC header  when DCCH/DTCH is mapped to
        FACH, and U-RNTI is used,it consists of :
        - TCTF     : 5-bits long and its value = 01100
        - UE ID Type : 2-bits long and its value = 00, 
        byte value is  0110000#
============================================================================================*/
#define   TCTF_UEID_TYPE_DCCH_DTCH_WITH_URNTI_ON_FACH 0x60

/*============================================================================================
MACRO NAME    TCTF_UEID_TYPE_DCCH_DTCH_WITH_CRNTI_ON_FACH

DESCRIPTION    This macro decides 7 MSB of MAC header  when DCCH/DTCH is mapped to
        FACH, and C-RNTI is used,it consists of :
        - TCTF     : 5-bits long and its value = 01100
        - UE ID Type : 2-bits long and its value = 01, 
        byte value is  0110001#
============================================================================================*/
#define   TCTF_UEID_TYPE_DCCH_DTCH_WITH_CRNTI_ON_FACH 0x62

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
    rlc_lc_id_type             *rlc_id,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset
);
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
    rlc_lc_id_type             *rlc_id,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset
);
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
    rlc_lc_id_type             *rlc_id,

/* rlc header offset  */
    uint8                      *rlc_pdu_offset
);
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti

DESCRIPTION     This function processes urnti in header info of FACH TB that is mapped to
        DCCH/DTCH with URNTI
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_urnti
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr
);
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct

DESCRIPTION     This function processes C/T in header info of FACH TB that is mapped to
        DCCH/DTCH with urnti
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_urnti_on_fach_compare_ct
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr,
    /* ct field info */
    uint8                    *ct_field
);
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
    rlc_lc_id_type             *rlc_id,

    /* rlc header offset  */
    uint8                      *rlc_pdu_offset
 );
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti

DESCRIPTION     This function processes crnti in header info of FACH TB that is mapped to
        DCCH/DTCH with CRNTI
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_crnti
(
    /* The number of tb in tbs  */
    uint8   tb_num,

    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr 
);
/*============================================================================================
FUNCTION NAME   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct

DESCRIPTION     This function processes C/T in header info of FACH TB that is mapped to
        DCCH/DTCH with crnti
============================================================================================*/
boolean   tdsprocess_dcch_dtch_tb_with_crnti_on_fach_compare_ct
(
    /* The number of tb in tbs  */
    uint8   tb_num,
    /* Pointer to Transport block header information */
    tdsl1_dl_tb_hdr_struct_type      *tb_hdr_ptr,
    /* ct field info */
    uint8                    *ct_field
 );
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
    uint8                    ct_field,

    /* rlc_id */
    rlc_lc_id_type             *rlc_id,

    /* rlc header offset  */
    uint8                      *rlc_pdu_offset
 );
/*============================================================================================
FUNCTION NAME   tdsprocess_fach_tbs_hdr

DESCRIPTION     This function processes FACH TBS Header information
============================================================================================*/
void    tdsprocess_fach_tbs_hdr
(
    /* First TB ID */
    uint8   tb_id,

    /* Pointer to internal FACH config info */
    tdsfach_config_info_type       *int_fach_ptr,

    /* Pointer to FACH TBS Header Info */
    tdsl1_dl_tb_set_hdr_struct_type    *tbs_hdr_ptr
);

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
  );

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
);

/*===============================================================================================
FUNCTION NAME tdsmac_dl_check_fach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a FACH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_dl_check_fach_trch
(
	/* RLC logical channel buffer identifier (0-20) */
	rlc_lc_id_type rlc_id, 

	/* Radio Bearer Identifier (0-31) */
	rb_id_type rb_id
);
#endif

