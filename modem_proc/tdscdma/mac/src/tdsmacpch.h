#ifndef TDSMACPCH_H
#define TDSMACPCH_H
/*=====================================================================

     P C H    C O N F I G U R A T I O N    H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in "macpchcfg.c"
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macpch.h_v   1.1   09 May 2001 18:15:44   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacpch.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

=====================================================================*/

/*==============================================================================================

FUNCTION NAME   tdsprocess_pch_config

DESCRIPTION		This function processes PCH Configuration Data received from RRC

==============================================================================================*/
extern	void	tdsprocess_pch_config 
(
	/* Number of PCHs */
	uint8   num_pchs,
	
	/* Pointer to PCH ordered config data */
	tdsmac_dl_pch_cctrch_config_type	*pch_info
);

/*==============================================================================================

FUNCTION		tdsprocess_pch_tbs_hdr

DESCRIPTION		This function processes PCH TBS Header information

CONTEXT			Layer 1 Interrupt context 

==============================================================================================*/
void	tdsprocess_pch_tbs_hdr 
(
	/* First PCH TB ID */
	uint8	tb_id,

	/* Pointer to internal PCH info */
	tdspch_config_info_type			*int_pch_ptr,
	
	/* Pointer to PCH TBS Header Info */
	tdsl1_dl_tb_set_hdr_struct_type	*tbs_hdr_ptr
);          

#endif

