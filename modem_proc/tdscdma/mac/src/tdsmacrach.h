#ifndef TDSMACRACH_H
#define TDSMACRACH_H
/*=====================================================================

    R A C H    C O N F I G U R A T I O N    H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the definition of the data types and functions 
  declared in "macrachcfg.c".
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macrach.h_v   1.3   19 Jun 2001 20:04:00   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacrach.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     --------------------------------------------------------------------------
10/22/10   cyx     declare tds_dcch_dtch_hdr_with_crnti_on_rach,
                    DCCH_DTCH_HDR_WITH_TCTF_CRNTI_TYPE_ON_RACH
10/22/10   guo     Initial release Upmerged WCDMA to TDS

=====================================================================*/

/*============================================================================================
MACRO NAME    DCCH_DTCH_HDR_WITH_TCTF_CRNTI_TYPE_ON_RACH

DESCRIPTION   This macro decides 6 MSB of MAC header  when DCCH/DTCH is mapped to
        RACH, and C-RNTI is used, it consists of :
        - TCTF     : 4-bits long and its value = 0100
        - UE ID Type : 2-bits long and its value = 01,
          byte value is  010001##
============================================================================================*/
#define   DCCH_DTCH_HDR_WITH_TCTF_CRNTI_TYPE_ON_RACH 0x44

/*============================================================================================
Systems dash board structure definitions */
/* Sarao: Data Structure for L2 RACH stats */
typedef struct
{
  /* Count of random access procedure success since last power up */
  uint32 rach_success_cnt;
  /* Count of random access procedure failure due to RACH transmission was aborted by L1 */
  uint32 rach_failure_due_to_abort_cnt;
  /* Count of random access procedure failure due to Max RACH Attempts */   
  uint32 rach_failure_due_to_max_rach_attempts_cnt;
  /* Count of random access procedure failure due to other reasons */
  uint32 rach_failure_due_to_other_reasons_cnt;
}tds_l2_rach_stats_type;


/*============================================================================================*/

/*============================================================================================
FUNCTION NAME   tds_dcch_dtch_hdr_with_crnti_on_rach

DESCRIPTION     This macro decides MAC header value and size when DCCH/DTCH is mapped to
        RACH, and C-RNTI is used, MAC header size is 26-bits long and it consists of :
        - TCTF     : 4-bits long and its value = 0100
        - UE ID Type : 2-bits long and its value = 01
        - UE ID    : 16-bits long and its value is C-RNTI
        - C/T    : 4-bits long & value is one minus MAC Logical channel identity
============================================================================================*/
void tds_dcch_dtch_hdr_with_crnti_on_rach
(
    uint8                            *hdrval,
    tdsmac_ul_ded_logch_config_type  *oc_dlcptr,
    tdsul_lc_info_type               *int_lc_ptr
);
/*=====================================================================
FUNCTION NAME	tdsproc_rach_cctrch_cfg

PURPOSE			Processes the RACH type CCTrCh Config data
    
PARAMETERS		rachptr : Pointer to the RACH configuration data

DESCRIPTION		If CCCH is mapped to RACH, this function calls the
				procedure that processes the CCCH config data.
=====================================================================*/
void tdsproc_rach_cctrch_cfg 
(
  /* Pointer to RACH configuration data */
  tdsmac_ul_rach_cctrch_config_type  *rachptr
);

/*==============================================================================================
 FUNCTION NAME    tdsproc_update_rach_mac_header

 DESCRIPTION    This function update MAC header for each logical channel
==============================================================================================*/
void  tdsproc_update_rach_mac_header
  (
/* Pointer to ordered config data of RACH */
  tdsmac_ul_rach_cctrch_config_type    *oc_rachptr
  );

/*==============================================================================================
FUNCTION NAME	tdsbuild_rach_tblks

DESCRIPTION		This function builds RACH Transport Blocks
				
===============================================================================================*/
void tdsbuild_rach_tblks
(
  /* Pointer to Build Frame Indication Parameters */
  tdsmac_ul_build_frame_ind_type  *bld_frame_ind_ptr

);

/*===============================================================================================
FUNCTION NAME tdsproc_rach_tx_parameters

DESCRIPTION   Processes RACH Transmit Control Parameters
===============================================================================================*/
void tdsproc_rach_tx_parameters 
(
tdsmac_ul_rach_txcntl_config_type  *rach_tx_prmtrs
);

#include "tdscdma_variation.h"
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
/*===============================================================================================
FUNCTION NAME	tdsmac_ul_proc_rach_tx_update_parameters

DESCRIPTION		Update RACH Transmit Control Parameters
===============================================================================================*/
void tdsmac_ul_proc_rach_tx_update_parameters 
(
  tdsmac_ul_persistence_config_type  *persistence_info
);
#endif


/*===============================================================================================
FUNCTION NAME tdsmac_free_tx_dsm_items

DESCRIPTION   Frees Transmit DSM items
===============================================================================================*/
void  tdsmac_free_tx_dsm_items
  (
/* Transmit Data Buffer Pointer */
  tdsl1_ul_tx_data_type    *tx_buf_ptr
  );

/*===============================================================================================
FUNCTION NAME tdsmac_reset_rach

DESCRIPTION   Resets RACH state
===============================================================================================*/
void  tdsmac_reset_rach
  (
  void
  );

/*===============================================================================================
FUNCTION NAME tdsmac_send_tx_status_ind_to_rrc

DESCRIPTION   Sends CMAC_STATUS_IND to RRC
===============================================================================================*/
void  tdsmac_send_tx_status_ind_to_rrc
  (
  tdsmac_status_ind_e_type status
  );

/*===============================================================================================
FUNCTION NAME tdspick_mlp

DESCRIPTION   Selects the maximum MAC logical channel priority.

RETURNS     1 - 8 Depending on the selected MLP
===============================================================================================*/
uint8 tdspick_mlp(tdsrlc_ul_frame_format_type *rlc_chan_info_ptr);

/*===============================================================================================
FUNCTION NAME tdspersistence_is_ok

DESCRIPTION   Checks to see if the persistence is OK or not

RETURNS     TRUE  if the persistence is OK FALSE otherwise

===============================================================================================*/
boolean   tdspersistence_is_ok ( void );

/*================================================================================================
FUNCTION NAME tdsrach_tx_control

DESCRIPTION   This function controls RACH transmission and is called once every TTI
================================================================================================*/
boolean   tdsrach_tx_control
  (
  void
  );

/*===============================================================================================
FUNCTION NAME tdsmac_ul_check_rach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a RACH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_ul_check_rach_trch
(
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
	rb_id_type rb_id
);
#endif

/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_success_cnt

DESCRIPTION   This function returns the RACH success count value
===============================================================================================*/
uint32 tdsl2_get_rach_success_cnt
(
   void
);


/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_abort_cnt

DESCRIPTION   This function returns the RACH FAILURE Abort count
===============================================================================================*/
uint32  tdsl2_get_rach_failure_due_to_abort_cnt
(
   void
);

/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_max_rach_attempts_cnt

DESCRIPTION   This function returns RACH FAILURE Max Rach attempts
===============================================================================================*/
uint32 tdsl2_get_rach_failure_due_to_max_rach_attempts_cnt
(
   void
);


/*===============================================================================================
FUNCTION NAME tdsl2_get_rach_failure_due_to_other_reasons_cnt

DESCRIPTION   This function returns RACH FAILURE for other reasons
===============================================================================================*/

uint32 tdsl2_get_rach_failure_due_to_other_reasons_cnt
(
   void
);

/*===============================================================================================
FUNCTION NAME tdsl2_clear_rach_stats

DESCRIPTION   This function clears all the counters
===============================================================================================*/
void tdsl2_clear_rach_stats
(
   void
);
