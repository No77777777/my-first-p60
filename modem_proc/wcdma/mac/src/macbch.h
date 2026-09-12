#ifndef MACBCH_H
#define MACBCH_H
/*===============================================================================================

                   B C H   H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the prototypes of the functions defined in "macbch.c"
  
Copyright (c) 2001 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2008-2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

==============================================================================================*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macbch.h_v   1.1   09 May 2001 18:13:16   psuvarna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macbch.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------
06/26/15   aa      Removing extern declarations from L2 code
01/29/15   kc      L2 global reorg for W+W
05/07/09   ssg     Updated Copyright Information
05/02/01	ps	   Changed few functions to MACROs and cleaned up the debug log code
04/03/01	ps	   Initial release after merging macbchcfg.h and macbchhdr.h

==============================================================================================*/


/*==============================================================================================

FUNCTION NAME   process_bch_config

DESCRIPTION		This function processes BCH configuration data received from RRC

==============================================================================================*/
void process_bch_config 

(
    /* Number of BCH type Channels */
    uint8							num_bchs, 
	
    /* Pointer to Ordered BCH Configuration data */
    mac_dl_bch_cctrch_config_type	*bch_info,

    mac_dl_table_type *dl_table_ptr
);
  
/*==============================================================================================

FUNCTION		process_bch_tbs_hdr

DESCRIPTION		This function processes BCH Transport Block Set header information
  
CONTEXT			Called by L1 decoder in its interrupt context 

==============================================================================================*/
void process_bch_tbs_hdr 
(
	/* Transport Block ID */
	uint8 tb_id,

	/* Pointer to internal BCH info */
	bch_config_info_type		*int_bch_ptr,

	/* Pointer to Transport Block Set Header Information */
	l1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr,

	mac_dl_table_type *dl_table_ptr
);                   

#endif

