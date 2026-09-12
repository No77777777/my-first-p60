#ifndef MACULDATA_H
#define MACULDATA_H
/*========================================================================

          U P L I N K   M A C  D A T A   H E A D E R   F I L E 
                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in "maculdata.c"
  
Copyright (c) 2001 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2008-2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*========*/

/*=========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculdata.h_v   1.6   09 May 2001 18:17:18   psuvarna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/maculdata.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------
06/26/15   aa      Removing extern declarations from L2 code
01/29/15   kc      L2 global reorg for W+W
12/09/13   as      Code change to EDRX full sleep if there pending partials at MAC. 
11/08/13   as      Added code change not to disable RACH if there is any pending data in DL 
                   that needs to be received which in turn would not trigger EDRX full sleep.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/05/12   mrg     HS-RACH & MAC-I Feature code integrated
01/02/12   grk     Feature cleanup
01/09/12   sk      MAC changes to support RxD in FACH
12/21/11   mrg     R99 profiling support added in MAC
10/22/11   sk      NikeL UL CM workaround under feature FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND
05/07/09   ssg     Updated Copyright Information
04/29/09   av      Fixed compiler warnings in L2 
10/17/08   grk     Merged taxis changes to mainline.
==============================================================================================*/
#include "maculhs.h"
#ifdef __GNUC__
#define		GET_UL_LC_BUF_STATUS_PTR(rlc_index)		&data_table_ptr->ul_buf_status.chan_info[rlc_index]
#else /* __GNUC__*/
#define		GET_UL_LC_BUF_STATUS_PTR(rlc_index)		&data_table_ptr->ul_buf_status.chan_info[##rlc_index##]
#endif /* __GNUC__ */

extern mac_rlc_data_table_type    mac_data_table[];
//#define     GET_MAC_RLC_DATA_TABLE_PTR(as_id)    &mac_data_table[as_id]

/* Profiling samples structure to capture the snapshot of the times stamps*/


/*==============================================================================================
 FUNCTION NAME	:	rlc_buffers_are_not_empty

 DESCRIPTION	:	This function checks whether RLC Logical Channel Buffers are empty or not.
					Returns TRUE if the buffers are not empty. FALSE otherwise
===============================================================================================*/
boolean   rlc_buffers_are_not_empty
(
  rlc_ul_lc_info_type       *mac_ul_lc_info_ptr,
  boolean                   *rlc_wait_for_ack,
  mac_ul_table_type         *ul_table_ptr
);

/*==============================================================================================
 FUNCTION NAME	build_ul_tblks

 DESCRIPTION	This function builds UL Transport Blocks
===============================================================================================*/
boolean build_ul_tblks 
(
  mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
 FUNCTION NAME  : mac_ignore_ul_srbs

 DESCRIPTION  : This function clears the buffer status of UL Signal Radio Bearers
===============================================================================================*/
void    mac_ignore_ul_srbs
(
/* Uplink Buffer Status Pointer */
rlc_ul_buf_status_type        *rlc_ul_buf_status,
mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
 FUNCTION NAME  attach_mac_parameters

 DESCRIPTION  Attaches MAC parameters to each UL RLC PDU
================================================================================================*/
void  attach_mac_parameters
(
/* RLC Frame Data Pointer */
rlc_ul_frame_data_type  *rlc_frame_data_ptr,

/* Shared Transmit Data Buffer Pointer */
l1_ul_tx_data_type    *txbufptr,

mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
 FUNCTION NAME  mac_compute_pdus_from_rlc_status

 DESCRIPTION  Compute the nbytes and npdus needed per LC based on RLC BO calculations
================================================================================================*/
void mac_compute_pdus_from_rlc_status(uint8  nlchan, rlc_lc_id_type   *rlc_id_ptr,
                                      rlc_ul_buf_status_type    *ul_buf_status_ptr, 
                                      boolean *data_present_on_srb_ptr,
                                      mac_ul_table_type *ul_table_ptr);

#endif

