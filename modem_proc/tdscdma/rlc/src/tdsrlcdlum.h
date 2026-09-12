#ifndef TDSRLCDLUM_H
#define TDSRLCDLUM_H
/*===========================================================================
            D O W N L I N K   U N A C K N O W L E D G E D   M O D E 

           R A D I O   L I N K   C O N T R O L : H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  Downlink Unacknowledged Mode RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcdlum.h_v   1.7   08 May 2002 19:13:42   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcdlum.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS
===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdsrlcdl.h"
#include "tdsmacrlcif.h"


/*==========================================================================

FUNCTION RLCI_ESTABLISH_DL_UM

DESCRIPTION
  Establish RLC UM Channel in Downlink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

==========================================================================*/
void tdsrlci_establish_dl_um
(
  tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
  tdsrlc_dl_um_channel_config_type *chan_info_ptr,
  tdsrlc_lc_action_type *act_ptr
);

/*==========================================================================

FUNCTION RLCI_RE_ESTABLISH_DL_UM

DESCRIPTION
  Re-establish RLC UM Channel in Downlink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

==========================================================================*/
void tdsrlci_re_establish_dl_um
(
  tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
  tdsrlc_dl_um_channel_config_type *chan_info_ptr  
);

/*===========================================================================

FUNCTION RLCI_MODIFY_DL_UM

DESCRIPTION
  Modify an RLC UM entity in Downlink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_modify_dl_um
(
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_dl_um_channel_config_type *chan_info_ptr,
  tdsrlc_lc_action_type *act_ptr  
);

/*===========================================================================

FUNCTION RLCI_RELEASE_DL_UM

DESCRIPTION
  Releases RLC UM Channel in Downlink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_release_dl_um
(
  tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr
);

/*===========================================================================

FUNCTION RLCI_DL_REASSEMBLE_UM_PDUS

DESCRIPTION
  Reassembles PDUs that arrive from MAC in the TTI. If the PDUs arrive
  out of order, the PDUs for the SDU(s) are discrded. If a PDU arrives 
  in-sequence, and has a length indicator, extract the first LI and its
  associated data(if any), chain it to the pdu packet chain, and enqueue
  it on to the downlink watermark for the channel. In addition, if there 
  are more length indicators in the PDU, extract them and enqueue the 
  SDUs on to the watermark. If the PDU arrives in sequence, and does not 
  have a length indicator, just chain the PDU to the packet chain.
  If the PDU arrives out of sequence, and the PDU does not have a length
  indicator, just chain the PDU to the packet and set discard flag to TRUE.
  IPDU arrives out of sequence and the PDU has a length indicator, extract
  the first LI and its associated data and discard it and discard the PDUs 
  in the packet chain. If there are more length indicators in the PDU, 
  extract them, and enqueue them on to the watermark.
 
DEPENDENCIES
  None.
  
RETURN VALUE
  Number of SDUs enqueued into WM.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 tdsrlci_dl_reassemble_um_pdus
(
  tdsrlci_dl_ctrl_blk_type  *ctrl_blk,   /* Pointer to Downlink Control Block */
  tdsrlci_dl_chan_data_type *tdsdata_ptr    /* Downlink PDU informationn         */
);

/*===========================================================================

FUNCTION RLCI_DL_UM_CIPHER_PDU

DESCRIPTION
  Check if this receiving PDU needs to be de-cipher.
  
  There is no retx PDU in UM. So, when cipher_mode changes from
  TDSCIPHER_MOD to TDSCIPHER_ON, there is no need to remember the old 
  cipher config.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the PDU needs to be de-ciphering. FALSE, otherwise 
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_dl_um_cipher_pdu
(
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_dl_tb_cipher_info_type *cipher_ptr,
  uint16 rx_sn
);

/*===========================================================================

FUNCTION RLCI_DL_UM_GET_COUNT_C

DESCRIPTION
  Obtain the Count_c of ciphering for this SN.
  
  If there is a wrap around, the real distance between the rx_sn and
  the VR_UR will be greater than .
  
  If rx_sn is greater than VR_UR, there is a forward wrap around.
  If rx_sn is less than VR_UR, there is a backward wrap around. 
  
DEPENDENCIES
  None.

RETURN VALUE
  Count_c for ciphering.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrlci_dl_um_get_count_c
(
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr,
  uint16 rx_sn
);



/*===========================================================================

FUNCTION rlci_enh_update_partial_sdu_ptr

DESCRIPTION
  Update the Partial SDU Contents

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_update_um_partial_sdu_ptr(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
                                     tdsrlci_dl_temp_pdu_info_type *dl_pdu_info_ptr,
                                     dsm_item_type *last_pdu_item_ptr);

/*===========================================================================

FUNCTION tdsrlci_enh_dl_process_um_pdus

DESCRIPTION
    - Process UM data 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_dl_process_um_pdus(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
                                 tds_complete_pdu_type *complete_pdu_ptr);

/*===========================================================================

FUNCTION tdsrlci_enh_discard_flag_handling

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_enh_discard_flag_handling(tdsrlci_dl_ctrl_blk_type *ctrl_blk);

/*===========================================================================

FUNCTION tdsrlci_enh_get_temp_tdspdu_info_ptr

DESCRIPTION
   Get the pointer to global variable

DEPENDENCIES
  None.

RETURN VALUE
  Returns the pointer to global variable

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_dl_temp_pdu_info_type *tdsrlci_enh_get_temp_pdu_info_ptr(void);

/*===========================================================================

FUNCTION tdsrlc_enh_establish_dl_um

DESCRIPTION
  Clear the contents of tdsdl_um_pdu_info.
  Partial SDU related pointers are set to NULL.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_establish_dl_um(tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr);

/*===========================================================================

FUNCTION tdsrlci_enh_um_discard_flag_handling

DESCRIPTION
  Discard the partial SDU contents and temp PDU Info contents
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_um_discard_flag_handling(tdsrlci_dl_ctrl_blk_type *ctrl_blk);

/*===========================================================================

FUNCTION tdsrlci_enh_dl_um_reassemble_data_pdus

DESCRIPTION
    - Complete the reassembly functionality for this PDU

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_dl_um_reassemble_data_pdus(tdsrlci_dl_ctrl_blk_type *ctrl_blk,
                                         tdsrlc_pdu_info_type     *walk_item_ptr);


#endif
