#ifndef TDSMACDL_H
#define TDSMACDL_H

/*=====================================================================

   D L   M A C   D A T A   I N D I C A T I O N   H E A D E R    F I L E

GENERAL DESCRIPTION

  This module contains the definition of the functions and data types
  declared in "macdldata.c".

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdl.h_v   1.2   12 Mar 2002 15:44:54   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdl.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS
=====================================================================*/

/*===========================================================================================
                            MACRO DEFINITIONS
===========================================================================================*/
/* Condition for invalid DL RB ID */
#define     TDSDL_RB_ID_IS_INVALID(rb_id)              (rb_id > TDSMAC_DL_MAX_RB_ID)

/* Condition for invalid DL RLC ID */

#define     TDSDL_RLC_ID_IS_INVALID(rlc_id)            (rlc_id > TDSMAC_DL_MAX_RLC_ID)

/* Condition for invalid DL CCTrCh ID */
#define     TDSDL_CCTRCH_ID_IS_INVALID(cctrch_id)      (cctrch_id > TDSMAC_DL_MAX_CCTRCH_ID)


/* Condition for invalid DL Transport Channel ID */
#define     TDSDL_TRCH_ID_IS_INVALID(tdstrch_id)\
            ((tdstrch_id < TDSMAC_DL_MIN_TRCH_ID) || (tdstrch_id > TDSMAC_DL_MAX_TRCH_ID))

/* Condition for invalid DL RLC Mode */
#define     TDSDL_RLC_MODE_IS_INVALID(rlc_mode)\
            (rlc_mode >= UE_MODE_MAX_NUM)

/* Condition for invalid DL MAC ID */
#define     TDSDL_MAC_ID_IS_INVALID(mac_id)\
            ((mac_id < TDSMAC_MIN_MAC_ID ) || (mac_id > TDSMAC_MAX_MAC_ID))

/* Condition for invalid C/T field */
#define     TDSDL_CT_FLD_IS_INVALID(tdsnum_dlc,ct_fld)\
            (ct_fld > TDSMAC_MAX_MAC_ID - 1)

/* Pointer to DL Transport Block Info */
extern      tdsdl_tb_info_type         tdsdl_tb_info[];
#define     TDSGET_DL_TB_INFO_PTR(tb_index)    &tdsdl_tb_info[tb_index]

/* Pointer to internal BCH config info */
extern      tdsbch_config_info_type        tdsbch_cfg_info[];
#define     TDSGET_BCH_INFO_PTR(cctrch_id)\
            &tdsbch_cfg_info[cctrch_id - TDSMAC_DL_MIN_CCTRCH_ID]

/* Pointer to internal PCH config info */
extern      tdspch_config_info_type        tdspch_cfg_info[];
#define     TDSGET_PCH_INFO_PTR(cctrch_id)\
            &tdspch_cfg_info[cctrch_id - TDSMAC_DL_MIN_CCTRCH_ID]

/* Pointer to internal DL TDSDCH config info */
extern      tdsdl_dch_config_info_type     tdsdl_dch_cfg_info[TDSUE_MAX_TRCH];
#define     TDSGET_DL_DCH_INFO_PTR(tdstrch_id)\
            &tdsdl_dch_cfg_info[tdsdl_dch_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID].idx]

/* Pointer to internal FACH config info */
extern      tdsfach_config_info_type       tdsfach_cfg_info[TDSUE_MAX_TRCH];
#define     TDSGET_FACH_INFO_PTR(tdstrch_id)\
            &tdsfach_cfg_info[tdsfach_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID].idx]

/* Transport Channel ID to Index Mapping Table */
extern      tdstrch_id_to_idx_map_type     tdsdl_dch_id_to_idx[MAX_TRCH];
extern      tdstrch_id_to_idx_map_type     tdsfach_id_to_idx[2*MAX_TRCH];

/* Condition for enabled DL TDSDCH */
#define     TDSDL_DCH_IS_ENABLED(tdstrch_id)\
            tdsdl_dch_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID].valid

/* DL TDSDCH ID to IDX Map Pointer */
#define     TDSDL_DCH_ID_TO_IDX_PTR(tdstrch_id)\
            &tdsdl_dch_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID]

/* Condition for enabled FACH */
#define     TDSFACH_IS_ENABLED(tdstrch_id)\
            tdsfach_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID].valid

/* FACH ID to IDX Map Pointer */
#define     TDSFACH_ID_TO_IDX_PTR(tdstrch_id)\
            &tdsfach_id_to_idx[tdstrch_id - TDSMAC_DL_MIN_TRCH_ID]

/* TRUE indicates that DL MAC Ciphering is enabled, FALSE otherwise */
extern  boolean     tdsdl_cipher_enable;

/* DL MAC Ciphering Key ID */
extern  uint8       tdsdl_cipher_key_id;

/* DL MAC HFN value */
extern  uint32      tdsdl_hfn;

/* TRUE indicates that DL MAC CRNTI is valid */
/* FALSE otherwise */
extern  boolean     tdsdl_mac_crnti_valid;

/* TRUE indicates that DL MAC URNTI is valid */
/* FALSE otherwise */
extern  boolean     tdsdl_mac_urnti_valid;

/* DL MAC CRNTI value */
extern  uint16      tdsdl_mac_crnti;

/* DL MAC URNTI value */
extern  uint32      tdsdl_mac_urnti;

#define     TDSDL_TB_CRNTI_IS_INVALID(crnti)\
    (!(tdsdl_mac_crnti_valid) || (crnti != tdsdl_mac_crnti))

#define     TDSDL_TB_URNTI_IS_INVALID(urnti)\
    (!(tdsdl_mac_urnti_valid) || (urnti != tdsdl_mac_urnti))


/*=============================================================================================
FUNCTION NAME   tdsconfig_dl_mac

DESCRIPTION     This function processes DL MAC Configaration Data received from RRC

=============================================================================================*/
void    tdsconfig_dl_mac
(
    /* Pointer to DL MAC Config Data */
    tdsmac_dl_config_type *cfgptr
);

#include "tdscdma_variation.h"
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*===========================================================================================
 FUNCTION NAME  tdsl1_phy_dl_query_allowed_buf_size_for_p_to_c

 DESCRIPTION  This function returns the buffersize used in L1 for 
              Partial to Complete PDU preparation

============================================================================================*/
uint16 tdsl1_phy_dl_query_allowed_buf_size_for_p_to_c(void);
#endif 

/*===========================================================================================
 FUNCTION NAME  tdsproc_dl_cipher_info

 DESCRIPTION  This function processes the Cipher information received from RRC.

============================================================================================*/
void    tdsproc_dl_cipher_info
  (
// Pointer to DL MAC Ciphering Config Info
  tdsmac_cipher_config_type   cipherptr[]
  );

/*============================================================================================
 FUNCTION NAME  tdsproc_dl_rnti_info

 DESCRIPTION  This function processes the RNTI information received from RRC.

============================================================================================*/
void    tdsproc_dl_rnti_info
  (
/* Pointer to DL RNTI Config Info */
  tdsmac_rnti_config_type   *rntiptr
  );

/*===============================================================================================
FUNCTION NAME tdsmac_dl_get_trch_type

DESCRIPTION   This function returns the down-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

log_tdscdma_trch_type tdsmac_dl_get_trch_type
  (
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
   rb_id_type rb_id
  );
#endif

