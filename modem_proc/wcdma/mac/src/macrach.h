#ifndef MACRACH_H
#define MACRACH_H
/*=====================================================================

    R A C H    C O N F I G U R A T I O N    H E A D E R   F I L E
                
GENERAL DESCRIPTION
  
  This module contains the definition of the data types and functions 
  declared in "macrachcfg.c".
  
Copyright (c) 2001 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2007-2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=====================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macrach.h_v   1.3   19 Jun 2001 20:04:00   psuvarna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macrach.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     --------------------------------------------------------------------------
06/26/15   aa      Removing extern declarations from L2 code
04/25/15   scv     W+W Changes to have messages support SubID
01/29/15   kc      L2 global reorg for W+W
05/15/14   kc      Changes for enabling RxD for FACH Signaling when HS-RACH is configured
10/29/13   ts      Added API mac_fetch_random_number() which calls secapi_get_random() having very good cryptographic properties
07/16/12   mrg     MAC_I & HS-RACH Compilation Warnings fixed
08/13/12   mrg     HS-RACH & MAC-I Feature updated code check-in.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/29/12   mrg     HS-RACH Feature latest changes
06/19/12   mrg     HS-RACH & MAC-I Feature code integrated
06/02/11   kc      Added support for logical channel to transport channel mapping info
05/07/09   ssg     Updated Copyright Information
04/29/09   av      Fixed compiler warnings in L2 
09/26/07   mrg     Added get_rach_cfg_ptr function support for ATF under FEATURE_CCCH_IE_ENH  
                   which is called by eliminate_tfc_based_on_bo() 
06/19/01   ps      Converted UL TX Status and Build Frame Indications from commands to signals
04/04/01   ps      Initial release after merging "macrachcfg.h" and "mactxstatus.h"

=====================================================================*/

#include "macrrcif.h"

#ifdef FEATURE_WCDMA_HS_RACH
#include "l1macdata.h"
#endif /*FEATURE_WCDMA_HS_RACH*/
#include "maculhs.h"
/*=====================================================================
FUNCTION NAME	proc_rach_cctrch_cfg

PURPOSE			Processes the RACH type CCTrCh Config data
    
PARAMETERS		rachptr : Pointer to the RACH configuration data

DESCRIPTION		If CCCH is mapped to RACH, this function calls the
				procedure that processes the CCCH config data.
=====================================================================*/
void	proc_rach_cctrch_cfg 
(
	/* Pointer to RACH configuration data */
	mac_ul_rach_cctrch_config_type  *rachptr,
	mac_ul_table_type *ul_table_ptr
);

/*==============================================================================================
FUNCTION NAME	build_rach_tblks

DESCRIPTION		This function builds RACH Transport Blocks
				
===============================================================================================*/
void	build_rach_tblks
(
  mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
FUNCTION NAME	proc_rach_tx_parameters

DESCRIPTION		Processes RACH Transmit Control Parameters
===============================================================================================*/
void	proc_rach_tx_parameters 
(
	mac_ul_rach_txcntl_config_type		*rach_tx_prmtrs, mac_ul_table_type *ul_table_ptr
);


/*===============================================================================================
FUNCTION NAME	get_rach_cfg_ptr

DESCRIPTION	This function returns rach_cfg_ptr
===============================================================================================*/
mac_ul_rach_cctrch_config_type *get_rach_cfg_ptr
(
  mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
FUNCTION NAME mac_free_tx_dsm_items

DESCRIPTION   Frees Transmit DSM items
===============================================================================================*/
void  mac_free_tx_dsm_items
  (
/* Transmit Data Buffer Pointer */
  l1_ul_tx_data_type    *tx_buf_ptr, mac_ul_table_type *ul_table_ptr
  );

/*==============================================================================================
 FUNCTION NAME  l1_ul_trigger_tx_req

 DESCRIPTION  This function triggers L1 to enable / disable PRACH
===============================================================================================*/
void  l1_ul_trigger_tx_req
  (
/* L1 TX Trigger Request Type */
  l1_ul_phychan_ctrl_type   action, mac_ul_table_type *ul_table_ptr
  );

/*===============================================================================================
FUNCTION NAME mac_reset_rach

DESCRIPTION   Resets RACH state
===============================================================================================*/
void  mac_reset_rach
  (
  mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr
  );

/*===============================================================================================
FUNCTION NAME mac_send_tx_status_ind_to_rrc

DESCRIPTION   Sends CMAC_STATUS_IND to RRC
===============================================================================================*/
void  mac_send_tx_status_ind_to_rrc
  (
  sys_modem_as_id_e_type as_id,
  mac_status_ind_e_type status
  );

/*===============================================================================================
FUNCTION NAME pick_mlp

DESCRIPTION   Selects the maximum MAC logical channel priority.

RETURNS     1 - 8 Depending on the selected MLP
===============================================================================================*/
uint8 pick_mlp(rlc_ul_frame_format_type *rlc_chan_info_ptr, mac_ul_table_type *ul_table_ptr);

/*===============================================================================================
FUNCTION NAME persistence_is_ok

DESCRIPTION   Checks to see if the persistence is OK or not

RETURNS     TRUE  if the persistence is OK FALSE otherwise

===============================================================================================*/
boolean   persistence_is_ok ( mac_ul_table_type *ul_table_ptr );

/*================================================================================================
FUNCTION NAME rach_tx_control

DESCRIPTION   This function controls RACH transmission and is called once every TTI
================================================================================================*/
boolean   rach_tx_control
  (
  mac_ul_table_type *ul_table_ptr
  );

/*===============================================================================================
FUNCTION NAME mac_ul_check_rach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a RACH transport channel
===============================================================================================*/

log_wcdma_trch_type mac_ul_check_rach_trch
(
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
   rb_id_type rb_id,

   mac_ul_table_type *ul_table_ptr
);

/*===============================================================================================
FUNCTION NAME mac_fetch_random_number

DESCRIPTION Based on featurization, it calls ran_next() or secapi_get_random()
            secapi_get_random() has very good cryptographic properties

RETURNS     uint32 Random Number value
===============================================================================================*/
uint32 mac_fetch_random_number(sys_modem_as_id_e_type as_id);

#ifdef FEATURE_WCDMA_HS_RACH
boolean   hs_rach_tx_control(mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr);
void proc_hs_rach_cfg(mac_ul_config_type *cfgptr, mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr);
#endif /* FEATURE_WCDMA_HS_RACH */
void  mac_ul_trigger_tx_req
(
  dsm_watermark_type* wm_ptr,
  void*               func_data
);
#endif

