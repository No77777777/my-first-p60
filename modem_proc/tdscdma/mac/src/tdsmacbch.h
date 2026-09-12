#ifndef TDSMACBCH_H
#define TDSMACBCH_H
/*===============================================================================================

                   B C H   H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the prototypes of the functions defined in "tdsmacbch.c"
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


==============================================================================================*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macbch.h_v   1.1   09 May 2001 18:13:16   psuvarna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacbch.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

==============================================================================================*/


/*==============================================================================================

FUNCTION NAME   tdsprocess_bch_config

DESCRIPTION		This function processes BCH configuration data received from RRC

==============================================================================================*/
extern	void	tdsprocess_bch_config 

(
    /* Number of BCH type Channels */
    uint8							num_bchs, 
	
    /* Pointer to Ordered BCH Configuration data */
    tdsmac_dl_bch_cctrch_config_type	*bch_info
);
  
/*==============================================================================================

FUNCTION		tdsprocess_bch_tbs_hdr

DESCRIPTION		This function processes BCH Transport Block Set header information
  
CONTEXT			Called by L1 decoder in its interrupt context 

==============================================================================================*/
extern	void	tdsprocess_bch_tbs_hdr 
(
	/* Transport Block ID */
	uint8 tb_id,

	/* Pointer to internal BCH info */
	tdsbch_config_info_type		*int_bch_ptr,

	/* Pointer to Transport Block Set Header Information */
	tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
);                   

#endif

