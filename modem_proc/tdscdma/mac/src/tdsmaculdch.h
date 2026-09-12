#ifndef TDSMACULDCH_H
#define TDSMACULDCH_H
/*===============================================================================================

							U L    D C H    H A N D L E R
                
GENERAL DESCRIPTION
  
  This module contains the definition of the data types and functions 
  declared in "maculdchcfg.c".
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===============================================================================================*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculdch.h_v   1.1   09 May 2001 18:21:20   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculdch.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===============================================================================================*/

/*==============================================================================================

FUNCTION NAME	tdsproc_ul_dch_cctrch_cfg 

DESCRIPTION		This function processes UL DCH CCTrCh Configuration Data

=================================================================================================*/
extern  void  tdsproc_ul_dch_cctrch_cfg 
(
  /* Pointer to ordered config data of UL DCH type CCTrCh */ 
  tdsmac_ul_dch_cctrch_config_type  *ded_cctrch_ptr
);

/*==============================================================================================
FUNCTION NAME	tdsbuild_rach_tblks

DESCRIPTION		This function builds RACH Transport Blocks
===============================================================================================*/
extern  void  tdsbuild_dch_tblks 
(
  /* Pointer to Build Frame Indication Parameters */
  tdsmac_ul_build_frame_ind_type  *bld_frame_ind_ptr
);

/*===============================================================================================
FUNCTION NAME tdsmac_ul_check_dch_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a UL-DCH transport channel
===============================================================================================*/

log_tdscdma_trch_type tdsmac_ul_check_dch_trch
(
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
   rb_id_type rb_id
);

#endif

