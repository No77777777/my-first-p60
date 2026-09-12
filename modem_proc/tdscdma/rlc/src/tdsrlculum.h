#ifndef TDSRLCULUM_H
#define TDSRLCULUM_H
/*===========================================================================
                  U P L I N K   U N A C K N O W L E D G E D   M O D E   

               R A D I O   L I N K   C O N T R O L : H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  Uplink Unacknowledged Mode RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlculum.h_v   1.4   22 May 2001 14:54:12   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlculum.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10    guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdscdma_variation.h"
#include "tdsrlcul.h"
#include "tdsmacrlcif.h"

/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/
/*---------------------------------------------------------------------------
  Un acknowledge Mode definitions
---------------------------------------------------------------------------*/
#define TDSRLC_UM_NEW_SDU_MASK           0x0002

/*---------------------------------------------------------------------------
  Length Indicator related definitions
---------------------------------------------------------------------------*/
#define TDSRLC_UL_LI_START_SDU_7_BIT        0xF8    /* = (0x7C << 1)    */
#define TDSRLC_UL_LI_START_SDU_15_BIT       0xFFF8  /* = (0x7FFC << 1)  */


/*==========================================================================

FUNCTION RLCI_RE_ESTABLISH_UL_UM

DESCRIPTION
  Re-establish RLC UM Channel in Uplink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

==========================================================================*/
void tdsrlci_re_establish_ul_um
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_ul_um_channel_config_type *um_chan_info_ptr 
);

/*===========================================================================

FUNCTION RLCI_GET_UM_BUFFER_STATUS

DESCRIPTION
 This function calculates the buffer status of a Unacknowledged mode 
 channel. The number of bytes reported is the sum of:
 - Number of bytes in the uplink queue, 
 - The number of SDUs in the Uplink Queue (representative of the number of 
   Length Indicators) and
 - If the Special LI = TRUE, length of length indicator( 1 for 7bit LI and
   2 for 15-bit LI). 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_get_um_buffer_status
(
  tdsrlci_ul_ctrl_blk_type     *ctrl_blk,   /* Pointer to Uplink Control Blk  */
  tdsrlc_ul_lc_buf_status_type *status_ptr  /* Pointer to status bytes        */
);


/*===========================================================================

FUNCTION RLC_UL_BUILD_UM_PDUS

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  Builds PDU header, data and padding (if any).
  Calculates the count,updates sequence number, handles incrementing of HFN 
  when the sequence number wraps.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_build_um_pdus 
(
  tdsrlci_ul_ctrl_blk_type      *ctrl_blk,  /* Pointer to Uplink Control Blk  */
  tdsrlc_ul_logchan_format_type *chan_info, /* PDU information for the channel*/
  tdsrlc_ul_logchan_data_type   *tdsdata_ptr   /* PDUs built for the channel     */
);

/*===========================================================================

FUNCTION RLCI_UL_UM_INIT

DESCRIPTION
  Initialize the Up-link UM RLC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_um_init(void);

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION RLC_EUL_BUILD_UM_PDUS

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  Builds PDU header, data and padding (if any).
  Calculates the count,updates sequence number, handles incrementing of HFN 
  when the sequence number wraps.

  In building PDUs for EDCH (EUL), the RLC directly write the PDU built into
  HW via a wrapper function from L1.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_eul_build_um_pdus 
(
  tdsrlci_ul_ctrl_blk_type      *ctrl_blk,  /* Pointer to Uplink Control Blk   */
  tdsrlc_ul_logchan_format_type *chan_info, /* PDU information for the channel */
  boolean                     last_unit_of_tti
);
#endif //FEATURE_TDSCDMA_HSUPA


/*===========================================================================

FUNCTION  RLC_UL_ENH_BULD_UM_PDUS

DESCRIPTION

  Builds uplink E-DCH UM PDUs for transmission in a TTI. 

  PP.SW:
  RLC PDU payloads are provided as a set of IO vectors.  
  RLC HDR(+LIs) are stored in a header buffer as word format.
  
  7200A:
  RLC HDR(+LIs), RLC PDU payloads are all provided as set of IO vectors.

  The SDUs to be freed are stored in the free_sdu_q.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None  .
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_build_um_pdus
(
  tdsrlci_ul_ctrl_blk_type      *ctrl_blk,   /* Pointer to Uplink Control Blk   */
  tdsrlc_ul_logchan_format_type *chan_info,  /* PDU information for this LC   */
  tdsl1_l2_data_type           *l1_ul_tx_buf_ptr
                                          /* Buffer to store IO vectors    */
);

/*===========================================================================

FUNCTION  RLC_UL_ENH_BULD_NON_EDCH_UM_PDUS

DESCRIPTION
  
  Builds uplink NON_E_DCH UM PDUs for transmission in a TTI. 

  The set of io vectors formed will be copied into a pre-allocated PDU
  dsm item. This dsm is passed to the lower layers.

  The SDUs to be freed are stored in the free_sdu_q.

DEPENDENCIES
  None.
  
RETURN VALUE
  uint16 - Num of PDUs built.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_enh_build_non_edch_um_pdus 
(
  tdsrlci_ul_ctrl_blk_type      *ctrl_blk,   /* Pointer to Uplink Control Blk */
  tdsrlc_ul_logchan_format_type *chan_info,  /* PDU information for this LC   */  
  tdsrlc_ul_logchan_data_type   *tdsdata_ptr    /* valid if chnl_type = NON_E_DCH*/
);

/*===========================================================================

FUNCTION  RLC_UM_FREE_SDU_Q

DESCRIPTION
  
  This will free the SDU dsm chains transmitted in the previous TTI.
  This will execute in L2ul-task context.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_um_free_sdu_q(tdsrlc_ul_chnl_e_type ul_chnl_type);

/*===========================================================================

FUNCTION  RLC_UM_FORM_PAD_PDU_HDR

DESCRIPTION
    This will form the PDU header for the PAdding PDUs
    This also inserts the special LIs (if any) in the current PDU
    
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_um_form_pad_pdu
(
  tdsrlci_ul_ctrl_blk_type   *ctrl_blk,    /* Pointer to Uplink Control Blk */
  uint16                  rem_pdu_size,
  tdsl1_l2_data_type        *l1_ul_tx_buf_ptr,
  tdsrlc_ul_pdu_data_type      *tdspdu_info   /* valid if chnl_type = NON_E_DCH */
);

/*===========================================================================

FUNCTION RLC_UM_GET_SDU_FREE_Q

DESCRIPTION

  Returns the pointer to the global header free SDU queue.
  Initialize the buffer_ptr of the SDU queue to the 
  corresponding Buffer (EDCH/ Non-EDCH) based on channel type

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_um_sdu_q_type* tdsrlc_um_get_sdu_free_q(tdsrlc_ul_chnl_e_type chnl_type);




#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION  RLC_UL_STORE_UM_CIPHER_LOG

DESCRIPTION

  Stores the ciphering related information for UM log packets.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.  

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_store_um_cipher_log
(
  rlc_lc_id_type          rlc_id,     /* rlc_id of the LC     */
  tdscipher_mode_e_type      mode,       /* Ciphering mode       */
  uint32                  key_id,     /* Ciphering key-id     */
  uint32                  cnt_c,      /* Ciphering count_c    */
  uecomdef_umts_cipher_algo_e_type  cipher_algo /* Ciphering algorithm  */
);

#endif

/*===========================================================================

FUNCTION  RLC_UL_UM_GET_CIPHER_PARAMS

DESCRIPTION
  
  This function computes the UM ciphering related parameters.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  o/p parameters: key_index, enabled, cnt_c
  
SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_umts_cipher_algo_e_type tdsrlc_ul_um_get_cipher_params
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,  /* Pointer to ctrl_blk      */
  uint32                *key_index,     /* Pointer to key Id        */
  boolean               *enabled,       /* Pointer to enabled flag  */
  uint32                *cnt_c          /* Pointer to count_c       */
);


/*===========================================================================

FUNCTION RLC_UL_UM_FORM_PDU_PAYLOAD

DESCRIPTION

  This will form the set of IO vectors for the UM PDU payload.

  E_DCH:
  For a E_DCH channel, IO vectors are store in L1 buffer

  NON_E_DCH:
  For a NON_E_DCH channel, Io vectors are store in RLC global array

DEPENDENCIES
  None.

RETURN VALUE
  uint16 - padding length in this PDU

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_um_form_pdu_payload
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,            /* Pointer to ctrl_blk          */
  uint16                pdu_size_bytes,       /* PDU size in bytes            */  
  tdsrlc_buff_type         *hdr_list,            /* Pointer to local header list */ 
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr,     /* Place to store IO vectors    */
  tdsrlc_ul_chnl_e_type    ul_chnl_type,         /* Channel type, EDCH / NonEDCH */
  boolean               *build_status        /* Returns PDU building status*/
);

/*===========================================================================

FUNCTION    RLC_UL_ENH_UM_ALT_E_BIT_LI

DESCRIPTION

  This function computes if a complete SDU fits in a PDU in alternate e-bit
  interpretation for different cases like
  1. E = 0
  2. E = 1, LI = 0x7d/0x7ffd, LI = 0x7FFA
  needs to inserted in this PDU.

DEPENDENCIES
  None.

RETURN VALUE
  uint16  - No. of bytes(LI) inseretd.
            0xFFFF if doesnot meet any conditions.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_enh_um_alt_e_bit_li
(
  uint16                payload_size,        /* Size of PDU excluding header */
  tdsrlc_buff_type         *hdr_list,     /* Length of LI                 */
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr    /* Pointer to ctrl_blk          */
);



#endif
