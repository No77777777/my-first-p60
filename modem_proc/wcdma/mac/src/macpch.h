#ifndef MACPCH_H
#define MACPCH_H
/*=====================================================================

     P C H    C O N F I G U R A T I O N    H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in "macpchcfg.c"
  
Copyright (c) 2001 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2008-2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macpch.h_v   1.1   09 May 2001 18:15:44   psuvarna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macpch.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------
06/26/15   aa      Removing extern declarations from L2 code
01/29/15   kc      L2 global reorg for W+W
05/07/09   ssg     Updated Copyright Information
05/02/01	ps	   Changed few functions to MACROs
04/04/01	ps	   Initial release after merging "macpchcfg.c" and "macpchhdr.c"

=====================================================================*/

/*==============================================================================================

FUNCTION NAME   process_pch_config

DESCRIPTION		This function processes PCH Configuration Data received from RRC

==============================================================================================*/
void process_pch_config 
(
	/* Number of PCHs */
	uint8   num_pchs,
	
	/* Pointer to PCH ordered config data */
	mac_dl_pch_cctrch_config_type	*pch_info,

	mac_dl_table_type *dl_table_ptr
);

/*==============================================================================================

FUNCTION		process_pch_tbs_hdr

DESCRIPTION		This function processes PCH TBS Header information

CONTEXT			Layer 1 Interrupt context 

==============================================================================================*/
void	process_pch_tbs_hdr 
(
	/* First PCH TB ID */
	uint8	tb_id,

	/* Pointer to internal PCH info */
	pch_config_info_type			*int_pch_ptr,
	
	/* Pointer to PCH TBS Header Info */
	l1_dl_tb_set_hdr_struct_type	*tbs_hdr_ptr,

	mac_dl_table_type *dl_table_ptr
);          

#endif

