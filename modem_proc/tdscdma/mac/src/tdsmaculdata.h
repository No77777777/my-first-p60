#ifndef TDSMACULDATA_H
#define TDSMACULDATA_H
/*========================================================================

          U P L I N K   M A C  D A T A   H E A D E R   F I L E 
                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in "tdsmaculdata.c"
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*========*/

/*=========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculdata.h_v   1.6   09 May 2001 18:17:18   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculdata.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

==============================================================================================*/

/* MACRO for UL LC Buffer Status Pointer */
extern  tdsrlc_ul_buf_status_type   tdsul_buf_status;   
#ifdef __GNUC__
#define  TDSGET_UL_LC_BUF_STATUS_PTR(rlc_index)  &tdsul_buf_status.chan_info[rlc_index]
#else /* __GNUC__*/
#define  TDSGET_UL_LC_BUF_STATUS_PTR(rlc_index)  &tdsul_buf_status.chan_info[##rlc_index##]
#endif /* __GNUC__ */

extern boolean tdsmac_rach_rxd_enable;
/*==============================================================================================
 FUNCTION NAME		tdsbuild_ul_frame

 DESCRIPTION		This function builds an UL frame

===============================================================================================*/
extern  void    tdsbuild_ul_frame 
(
  /* Pointer to Build Frame Indication Parameters */
  tdsmac_ul_build_frame_ind_type  *bld_frame_ind_ptr
);

/*==============================================================================================
 FUNCTION NAME	tdsbuild_ul_tblks

 DESCRIPTION	This function builds UL Transport Blocks
===============================================================================================*/
extern	boolean		tdsbuild_ul_tblks 
(
  /* Pointer to Build Frame Indication Parameters */
  tdsmac_ul_build_frame_ind_type  *bld_frame_ind_ptr
);

#ifdef FEATURE_TDSCDMA_SILENT_FRAME

/*==============================================================================================
 FUNCTION NAME	tdsmac_save_vfr_cb

 DESCRIPTION	This function saves the MVS VFR Cb func pointer
===============================================================================================*/
extern void tdsmac_save_vfr_cb
(
	/* Call Back function for VFR reg/De-reg */ 
	tds_vfr_cb_type vfr_cb_ptr
);

/*==============================================================================================
 FUNCTION NAME	tdsmac_save_vfr_client_data

 DESCRIPTION	This function saves the MVS client data
===============================================================================================*/
extern void tdsmac_save_vfr_client_data
(
	/* Client data provided by MVS */
		void * clnt_data
);

//extern void test_call_back(tds_vfr_info_type vfr_info, void* client_data);

#endif /*FEATURE_TDSCDMA_SILENT_FRAME*/

/*===============================================================================================
 FUNCTION NAME  tdsattach_mac_parameters

 DESCRIPTION  Attaches MAC parameters to each UL RLC PDU
================================================================================================*/
void  tdsattach_mac_parameters
(
/* RLC Frame Data Pointer */
tdsrlc_ul_frame_data_type  *rlc_frame_data_ptr,

/* Shared Transmit Data Buffer Pointer */
tdsl1_ul_tx_data_type    *txbufptr
);

/*===============================================================================================
 FUNCTION NAME  tdsmac_compute_pdus_from_rlc_status

 DESCRIPTION  Compute the nbytes and npdus needed per LC based on RLC BO calculations
================================================================================================*/
void tdsmac_compute_pdus_from_rlc_status(uint8  nlchan, rlc_lc_id_type   *rlc_id_ptr,
                                       tdsrlc_ul_buf_status_type    *ul_buf_status_ptr, boolean *data_present_on_srb_ptr);
#endif

