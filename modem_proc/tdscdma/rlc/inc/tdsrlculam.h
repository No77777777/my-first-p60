#ifndef TDSRLCULAM_H
#define TDSRLCULAM_H
/*===========================================================================
                  U P L I N K   N A C K N O W L E D G E D   M O D E   

               R A D I O   L I N K   C O N T R O L : H E A D E R  F I L E

DESCRIPTION
  This file contains data types and function prototypes associated with the
  Uplink acknowledged Mode RLC module.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlculam.h_v   1.5   24 Apr 2002 22:18:10   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlculam.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------- 
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

/*---------------------------------------------------------------------------
  app_field(0)    : 1 - SDU cnf required, 0 - Otherwise
  app_field(1)    : 1 - Partly txd SDU, 0 - New SDU
  app_field(15-8) : MUI of the SDU.
  app_field(27-16): last_sn in which this SDU ends.
---------------------------------------------------------------------------*/
#define   TDSRLC_UL_AM_SDU_TX_CNF(x)             ((x) & 0x1)
#define   TDSRLC_UL_AM_GET_SDU_MUI(x)            (((x) & 0x0000FF00) >> 8)
#define   TDSRLC_UL_AM_GET_SDU_PDU_END_SN(x)     (((x) & 0x0FFF0000) >> 16)
#define   TDSRLC_UL_AM_SET_SDU_PDU_END_SN(x, y)  ((x) |= ((y) << 16))
#define   TDSRLC_UL_AM_SET_PARTLY_TXD_SDU(x)     ((x) |= 0x00000002)
#define   TDSRLC_UL_AM_PARTLY_TXD_SDU(x)         ((x) & 0x00000002)
#define   TDSRLC_UL_AM_NEWLY_TXD_SDU(x)          (!((x) & 0x00000002))
#if defined(FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE) || defined(FEATURE_TDSCDMA_PS_RLC_SDU_RESTORE)
#define   TDSRLC_UL_AM_SET_NEWLY_TXD_SDU(x)      ((x) &= (0xFFFFFFFD))
#endif

#if defined(FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE) || defined(FEATURE_TDSCDMA_PS_RLC_SDU_RESTORE)
/*---------------------------------------------------------------------------
  Extend app_field for indicating RRC CONNECTION RELEASE COMPLETE in SRB2 (AM mode)
  app_field(28)    : 1 - No need to restore after SRB RESET, 0 - Otherwise
---------------------------------------------------------------------------*/
#define   TDSRLC_UL_AM_SDU_NO_RESTORE(x)           ((x) & 0x10000000)
#endif


/*---------------------------------------------------------------------------
  PDU ctrl_info related definitions.
---------------------------------------------------------------------------*/
#define TDSRLC_SPL_LI_OCTET_LESS_IN_NEXT_PDU_MASK     0x00200
#define TDSRLC_SPL_LI_OCTET_LESS_IN_NEXT_PDU(a)       ((a) & TDSRLC_SPL_LI_OCTET_LESS_IN_NEXT_PDU_MASK)
#define TDSRLC_SET_SPL_LI_OCTET_LESS_IN_NEXT_PDU(a)   ((a) |= TDSRLC_SPL_LI_OCTET_LESS_IN_NEXT_PDU_MASK)

#define TDSRLC_PDU_FIT_MASK                0x00100
#define TDSRLC_PDU_FIT(a)                  ((a) & TDSRLC_PDU_FIT_MASK)
#define TDSRLC_SET_PDU_FIT(a)              ((a) |= TDSRLC_PDU_FIT_MASK)

#define TDSRLC_PDU_FIT_HE_MASK             0x00400
#define TDSRLC_PDU_FIT_HE(a)               ((a) & TDSRLC_PDU_FIT_HE_MASK)
#define TDSRLC_SET_FIT_HE(a)               ((a) |= TDSRLC_PDU_FIT_HE_MASK)

#define TDSRLC_SPL_LI_EXACT_IN_NEXT_PDU_MASK     0x0080
#define TDSRLC_SPL_LI_EXACT_IN_NEXT_PDU(a)       ((a) & TDSRLC_SPL_LI_EXACT_IN_NEXT_PDU_MASK)
#define TDSRLC_SET_SPL_LI_EXACT_IN_NEXT_PDU(a)   ((a) |= TDSRLC_SPL_LI_EXACT_IN_NEXT_PDU_MASK)

#define TDSRLC_LI_EXACT_PRESENT_MASK       0x0040
#define TDSRLC_LI_EXACT_PRESENT(a)         ((a) & TDSRLC_LI_EXACT_PRESENT_MASK)
#define TDSRLC_SET_LI_EXACT_PRESENT(a)     ((a) |= TDSRLC_LI_EXACT_PRESENT_MASK)

#define TDSRLC_LI_OCTET_LESS_PRESENT_MASK  0x0020
#define TDSRLC_LI_OCTET_LESS_PRESENT(a)    ((a) & TDSRLC_LI_OCTET_LESS_PRESENT_MASK)
#define TDSRLC_SET_LI_OCTET_LESS_PRESENT(a) ((a) |= TDSRLC_LI_OCTET_LESS_PRESENT_MASK)

#define TDSRLC_PADDING_LI_PRESENT_MASK     0x0010
#define TDSRLC_PADDING_LI_PRESENT(a)       ((a) & TDSRLC_PADDING_LI_PRESENT_MASK)
#define TDSRLC_SET_PADDING_LI_PRESENT(a)   ((a) |= TDSRLC_PADDING_LI_PRESENT_MASK)

#define TDSRLC_NUM_SDUS_MASK               0x000F
#define TDSRLC_GET_NUM_SDUS(a)             ((a) & TDSRLC_NUM_SDUS_MASK)
#define TDSRLC_SET_NUM_SDUS(a,val)         ((a) |= ((val) & TDSRLC_NUM_SDUS_MASK))

#define TDSRLC_LI_PRESENT_MASK             0x0170
#define TDSRLC_LI_PRESENT(a)               (((a) & TDSRLC_LI_PRESENT_MASK))

#define TDSRLC_NO_PART_SDU_MASK            0x0390
#define TDSRLC_NO_PART_SDU(a)              ((a) & TDSRLC_NO_PART_SDU_MASK)

#ifdef FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE
#define TDSRLC_UL_AM_SEND_SDU_CNF(am_ctrl_ptr,sdu_ptr,status,fail_reason) \
do\
{\
  if ((sdu_ptr = TDSRLCI_Q_GET(&(am_ctrl_ptr->internal_q))) == NULL)\
  {\
     ERR_FATAL("RLC_ERR: RLC_ID %d Internal Q empty",am_ctrl_ptr->am_common->ul_data_id, 0, 0);\
  }\
  else\
  {\
    if ((TDSRLC_UL_AM_SDU_TX_CNF(sdu_ptr->app_field)) && \
        (am_ctrl_ptr->sdu_cnf_cb != NULL))\
    {\
      am_ctrl_ptr->sdu_cnf_cb(am_ctrl_ptr->am_common->ul_data_id, status, \
                            (uint8)TDSRLC_UL_AM_GET_SDU_MUI(sdu_ptr->app_field), fail_reason);\
    }\
    dsm_free_packet(&sdu_ptr);\
  }\
}while(0);
#endif
/*===========================================================================
** Public Data Declaration                 
**=========================================================================*/


/*===========================================================================
** Public Function Declaration                 
**=========================================================================*/
 /*===========================================================================
 
 FUNCTION TDSRLC_ENH_INITIALIZE_MUTEX
 
 DESCRIPTION   Initialize RLC Mutex
 
 DEPENDENCIES  None
 
 RETURN VALUE  None
 
 SIDE EFFECTS  None
 
 ===========================================================================*/
 void tdsrlc_enh_initialize_mutex(void);
/*===========================================================================

FUNCTION RLCI_UL_AM_INIT

DESCRIPTION
  Initialize the Up-link AM RLC.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_am_init(void);

/*===========================================================================

FUNCTION  RLCI_ESTABLISH_UL_AM

DESCRIPTION
  Config and establish an RLC AM in Uplink per RRC's request.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
byte tdsrlci_establish_ul_am
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_am_config_type *am_ptr
);

/*==========================================================================

FUNCTION RLCI_RE_ESTABLISH_UL_AM

DESCRIPTION
  Re-establish RLC AM entity in Uplink per RRC's request.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

==========================================================================*/
void tdsrlci_re_establish_ul_am
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_am_config_type *am_ptr
);

/*===========================================================================

FUNCTION RLCI_RELEASE_UL_AM

DESCRIPTION
  Release an RLC AM entity in Uplink per RRC's request.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
byte tdsrlci_release_ul_am
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr
);

#if defined(FEATURE_TDSCDMA_SRB_RLC_SDU_RESTORE) || defined(FEATURE_TDSCDMA_PS_RLC_SDU_RESTORE)
/*===========================================================================

FUNCTION TDSRLCI_UL_AM_ENQ_SRB_TO_WM_HEAD

DESCRIPTION
  API used to enqueue the data to head of the queue 
  by manipulating the priority field.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_am_enq_srb_to_wm_head
(
  dsm_watermark_type *wm_ptr,
  dsm_item_type **pkt_head_ptr
);

/*===========================================================================

FUNCTION TDSRLC_UL_AM_ENQ_PENDING_SDU_TO_WM

DESCRIPTION
  API used to enqueue loop through the pending data that are 
  ACKed and enqueue them back to the UL WM queue.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_am_enq_pending_sdu_to_wm(tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr);
#endif

/*===========================================================================

FUNCTION RLCI_MODIFY_UL_AM

DESCRIPTION
  Modify an RLC AM entity in Uplink per RRC's request.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_modify_ul_am
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  tdsrlc_am_config_type *am_ptr
);

/*===========================================================================

FUNCTION RLCI_GET_AM_BUFFER_STATUS

DESCRIPTION
 This function gets the total number of bytes needed to be transmitted in 
 AM Uplink.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_get_am_buffer_status
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_lc_buf_status_type *status_ptr
);

/*===========================================================================

FUNCTION RLC_UL_BUILD_AM_PDUS

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI for AM RLC. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_build_am_pdus 
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_logchan_format_type *chan_info,
  tdsrlc_ul_logchan_data_type *data_ptr
);

/*===========================================================================

FUNCTION RLCI_RESET_UL_AM

DESCRIPTION
  Reset UL AM RLC. If the dl_reset_req is TRUE, post a RESET request message
  to the DL AM RLC as well.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_reset_ul_am
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  boolean dl_reset_req
);


/*===========================================================================

FUNCTION RLCI_AM_RESET

DESCRIPTION
  Reset RLC AM (UL&DL). This function is excuted in the task context.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_am_reset
(
  void
);

#include "tdscdma_variation.h"
#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION BUILD_STATUS_PDU

DESCRIPTION
  Builds status PDU for AM in Uplink.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type *tdsbuild_status_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  uint16  pdu_size                      /* PDU size in bits */
);

/*===========================================================================

FUNCTION BUILD_RETX_DATA_PDU

DESCRIPTION
  Builds rexmit data PDU for AM in Uplink.
  
  For NAK LIST type: The SN is the one that is missing. The distance is the
    additional number of missing PDUs following the SN.
  
  For NAK BITMAP type: The SN is the first bit in the bitmap.
  
DEPENDENCIES
  Assume that the PDU size is always bigger or exact the size of the retx PDU.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type *tdsbuild_retx_data_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_pdu_data_type  *pdu_data_ptr,
  uint16                 pdu_size,                   /* PDU size in bytes  */
  boolean                *error
);

/*===========================================================================

FUNCTION BUILD_NEW_DATA_PDU

DESCRIPTION
  Builds new data PDU for AM in Uplink.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type *tdsbuild_new_data_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_pdu_data_type  *pdu_data_ptr,
  uint16  pdu_size                     /* PDU size in bytes  */
);

/*===========================================================================

FUNCTION BUILD_TXD_DATA_PDU

DESCRIPTION
  Builds data PDU that is txd before. 
  
  When polling timer expired and there is no new data to go, a poll needs 
  to be sent the peer side for status report. We decide to send the latest
  sent PDU (VT_S -1) to remind the peer what is the max number of PDUs sent.
  
DEPENDENCIES
  Assume that the PDU size is always bigger or exact the size of the retx PDU.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type *tdsbuild_txd_data_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_pdu_data_type  *pdu_data_ptr,
  uint16  pdu_size                      /* PDU size in bytes  */
);
#endif //FEATURE_TDSCDMA_HSUPA

/*===========================================================================

FUNCTION  RLCI_UL_INIT_RESET_PARAM

DESCRIPTION
This function will initialize the uplink reset parameters to their default
values.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_init_reset_param
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr
);


/*===========================================================================

FUNCTION RLCI_ENH_GET_AM_BUFFER_STATUS

DESCRIPTION
 This function gets the total number of bytes needed to be transmitted in
 AM Uplink.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_get_am_buffer_status
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_lc_buf_status_type *status_ptr
);




/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_AM_PDUS

DESCRIPTION

  This prepares the new data, re-tx, ctrl pdu for a AM LC mapped to a
  E-DCH transport channel.

  PP.SW:
  RLC PDU payloads are provided as a set of IO vectors.  
  RLC HDR(+LIs) are stored in a header buffer as word format.
  
  7200A:
  RLC HDR(+LIs), RLC PDU payloads are all provided as set of IO vectors.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_build_am_pdus
(
  tdsrlci_ul_ctrl_blk_type       *ctrl_blk,  /* Pointer to ctrl_blk           */
  tdsrlc_ul_logchan_format_type  *chan_info, /* Pointer to channel information*/
  tdsl1_l2_data_type            *l1_ul_tx_buf_ptr
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  , boolean                    log_allowed
#endif  
                                          /* Place to store IO vectors     */
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NON_EDCH_AM_PDUS

DESCRIPTION

  This prepares the new data, re-tx, ctrl pdu for a AM LC.

  The set of io vectors formed will be copied into a pre-allocated PDU
  dsm item. This dsm is passed to the lower layers.
  
DEPENDENCIES
  None.
  
RETURN VALUE

  uint16 - Number of PDUs prepared.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_enh_build_non_edch_am_pdus
(
  tdsrlci_ul_ctrl_blk_type       *ctrl_blk,  /* Pointer to ctrl_blk           */
  tdsrlc_ul_logchan_format_type  *chan_info, /* Pointer to channel information*/
  tdsrlc_ul_logchan_data_type    *data_ptr   /* Pointer to data information   */
);


/*===========================================================================

FUNCTION RLC_UL_AM_FLUSH_INTERNAL_Q

DESCRIPTION

  This flushes the internal Q and sends a SDU confimation.
  
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_am_flush_internal_q
(
  tdsrlci_ul_am_ctrl_type          *am_ctrl_ptr, /* pointer to am_ctrl   */
  rlc_sdu_tx_fail_reason_e_type reason        /* SDU cnf reason       */
);

/*===========================================================================

FUNCTION RLC_UL_AM_SEND_SDU_CNF

DESCRIPTION

  This sends the SDU cnf to the upper layers with in the SN 
  old_vt_a to vt_a .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_am_send_sdu_cnf
(
  tdsrlci_ul_am_ctrl_type          *am_ctrl_ptr, /* Pointer to am_ctrl */
  uint16                        prev_vt_a,    /* Start SN           */
  uint16                        new_vt_a,     /* End SN             */
  uecomdef_status_e_type        status,       /* success/failue     */
  rlc_sdu_tx_fail_reason_e_type fail_reason   /* SDU cnf reason     */
);

/*===========================================================================

FUNCTION RLCi_DUMP_PDU_DATA

DESCRIPTION

  Dumps the enitre PDU contents
  
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dump_pdu_data
(
  tdsl1_l2_data_type  *l1_ul_tx_buf_ptr, 
  uint16            src_desc_idx
);


/*===========================================================================

FUNCTION RLCI_ENH_GET_DATA_NUM_BYTES

DESCRIPTION
  This function reports the total number of data bytes that need to be
  transmitted by RLC. This includes new data and retx data.
  If TDSRLC_NO_DISCARD is configured, then there is a check to see if
  maxdat is going to be hit for VT_A or VT_S. If number of re-transmissions
  of VT_A is maxdat -1 and there is a NAK for VT_A sitting in the nak
  buffer,then initate a reset.However, if maxreset is set to 1, then post a
  reset failure to RRC. VT_S is re-transmitted when there is no other data
  to send and polling is triggered. If VT_S hits maxdat -1 and polling is
  triggered, initiate a reset if maxreset is greater than 1 otherwise
  post a RRC reset failure.This is to prevent an extra padding PDU from
  being sent since reset takes place in the task context and the reset PDU
  goes out in the next TTI. However, this does not address the problem
  yet for MRW procedure. If MRW is triggered on hitting maxdat,a padding
  PDU goes out in that TTI and MRW SUFI is sent in the next TTI.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  Can cause a potential problem when MRW is configured since a padding
  PDU goes out on hitting maxdat before a MRW SUFI is sent in the next TTI.

===========================================================================*/
void tdsrlci_enh_get_data_num_bytes
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_lc_buf_status_type *status_ptr
);

/*===========================================================================

FUNCTION RLCI_ENH_GET_STATUS_NUM_BYTES

DESCRIPTION
 This function gets the total number of bytes needed to be transmitted in
 AM Uplink.

 Priorities of status SUFIs:
 1. RESET/RESET ACK PDUs.
 2. MRW/MRW_ACK SUFIs.
 3. NAK SUFIs.


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsrlci_enh_get_status_num_bytes
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  tdsrlc_ul_lc_buf_status_type  *status_ptr
);


/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_PADDING_ACK_PDU

DESCRIPTION

  Builds ACK status PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_build_padding_ack_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,    /* Pointer to thectrl_blk         */
  uint16                pdu_size,         /* PDU size in bits               */
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr /* Place to store Io vectors      */
 #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
 , boolean  log_allowed
 #endif  
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NON_EDCH_PADDING_ACK_PDU

DESCRIPTION

  Builds ACK status PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_build_non_edch_padding_ack_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,    /* Pointer to thectrl_blk         */
  uint16                pdu_size,         /* PDU size in bits               */
  tdsrlc_ul_pdu_data_type  *data_ptr         /* Valid if chnl_type = NON_E_DCH */  
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_CTRL_PDU

DESCRIPTION

  This prepares the ctrl pdu for a particular AM LC.

  The control PDU is prepared using a pre-allocated ctrl_pdu_dsm_item.
  ctrl_pdu_dsm_item is shared by all AM LC's who ever wants to prepare 
  control PDU in this TTI. If the 1st item is exhanused, control PDUs
  are built uisng the second item.

  E_DCH:
  The control PDU is provided as single IO vector.
  
  NON_E_DCH:
  The single io vector formed will be copied into a pre-allocated PDU
  dsm item. This dsm is passed to the lower layers.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  Boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_ctrl_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,    /* pointer to uplink control     */
  uint16                pdu_size,         /* pdu size in bits              */
  boolean               first_status_pdu, /* TRUE - 1st status PDU for this*/ 
                                          /* LC. FALSE - other wise        */
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr
                                          /* Place to store IO vectors     */
 #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  , boolean log_allowed
 #endif
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NON_EDCH_CTRL_PDU

DESCRIPTION

  This prepares the ctrl pdu for a particular AM LC.

  The control PDU is prepared using a pre-allocated pdu_dsm_pool.
  The prepared dsm_item is paased to the lower layers.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  Boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_non_edch_ctrl_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,    /* pointer to uplink control     */
  uint16                pdu_size,         /* pdu size in bits              */
  boolean               first_status_pdu, /* TRUE - 1st status PDU for this*/ 
                                          /* LC. FALSE - other wise        */
  tdsrlc_ul_pdu_data_type  *data_ptr
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_RETX_DATA_PDU

DESCRIPTION

  This builds the re-tx PDU using the stored book-keeping information.

  PP.SW:
  RLC PDU payloads are provided as a set of IO vectors.  
  RLC HDR(+LIs) are stored in a header buffer as word format.
  
  7200A:
  RLC HDR(+LIs), RLC PDU payloads are all provided as set of IO vectors.

DEPENDENCIES
  None.
  
RETURN VALUE
  boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_retx_data_pdu
(
  tdsrlci_ul_ctrl_blk_type     *ctrl_blk,  /* pointer to uplink control      */
  uint16                    retx_sn,    /* re-tx PDU SN */
  uint16                    pdu_size,   /* pdu size in bits               */
  tdsl1_l2_data_type          *l1_ul_tx_buf_ptr
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  , boolean                 log_allowed
#endif   
                                        /* Place to store IO vectors      */
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NON_EDCH_RETX_DATA_PDU

DESCRIPTION

  This builds the re-tx PDU using the stored book-keeping information.

  NON_E_DCH:
  The set of io vectors formed will be copied into a pre-allocated PDU
  dsm item. This dsm is passed to the lower layers.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_non_edch_retx_data_pdu
(
  tdsrlci_ul_ctrl_blk_type     *ctrl_blk,  /* pointer to uplink control      */
  uint16                    retx_sn,    /* re-tx PDU SN */
  uint16                    pdu_size,   /* pdu size in bits               */
  tdsrlc_ul_pdu_data_type      *data_ptr   /* Place to store prepared PDUS   */
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NEW_DATA_PDU

DESCRIPTION

  This builds the New PDU using the stored book-keeping information.

  PP.SW:
  RLC PDU payloads are provided as a set of IO vectors.  
  RLC HDR(+LIs) are stored in a header buffer as word format.
  
  7200A:
  RLC HDR(+LIs), RLC PDU payloads are all provided as set of IO vectors.

DEPENDENCIES
  None.
  
RETURN VALUE
  boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_new_data_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,        /* pointer to uplink control      */
  uint16                pdu_size,         /* PDU size in bits               */
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr /* Valid if chnl_type = E_DCH     */ 
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
   , boolean log_allowed
#endif   
);

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_NON_EDCH_NEW_DATA_PDU

DESCRIPTION

  This builds the new PDU using the stored book-keeping information.

  NON_E_DCH:
  The set of io vectors formed will be copied into a pre-allocated PDU
  dsm item. This dsm is passed to the lower layers.
  
DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE/FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_enh_build_non_edch_new_data_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,        /* pointer to uplink control      */
  uint16                pdu_size,         /* PDU size in bits               */
  tdsrlc_ul_pdu_data_type  *data_ptr         /* Place to store prepared PDUS   */  
);


/*===========================================================================

FUNCTION RLCI_RESET_STATISTICS_VARIABLES

DESCRIPTION
  The function resets the number of mrw acks sent and number of reset acks
  sent out. These variables are used for reporting events for logs.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Only tdsno_mrw_ack_sent, tdsno_reset_sent and tdsno_reset_ack_sent are reset.

===========================================================================*/

void tdsrlci_reset_statistics_variables
( void
);

/*===========================================================================

FUNCTION RLCI_GET_RETX_COUNT_C

DESCRIPTION
  Obtain the Count_c of ciphering for this SN.

  Use act_old_cfg and act_sn to determine which config to use. (old or
  current)

  If there is a wrap around, the real distance between the tx_sn and
  the VT_S will be greater than 2048.

  It assumes that tx_sn is always less than VT_S, since the tx_sn is
  the SN for a retx PDU.

DEPENDENCIES
  None.

RETURN VALUE
  Count_c for ciphering.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_get_retx_count_c
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  uint16 tx_sn,
  uint32                *key_index,
  boolean               *enabled,
  uecomdef_umts_cipher_algo_e_type *cipher_algo,
  uint32                *cnt_c  
);


/*===========================================================================

FUNCTION RLCI_DISCARD_SDU_IN_WM

DESCRIPTION
  Remove the unfinished SDU from WM.
  This function will be called when performing RESET or generating MRW.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_discard_sdu_in_wm
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk
);

/*===========================================================================

FUNCTION RLCI_RESET_UL_AM_CTL_BLK

DESCRIPTION
  Reset the UL AM RLC control block.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_reset_ul_am_ctl_blk
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  boolean IsRelease                    /* TRUE for the reset being asked  */
                                       /* by the release procedure.       */
);

/*===========================================================================

FUNCTION BUILD_RESET_PDU

DESCRIPTION
  Builds RESET PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsbuild_reset_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  dsm_item_type *pdu_ptr,
  uint16 bit_offset,
  uint16 pdu_size                             /* PDU size in bits */
);

/*===========================================================================

FUNCTION BUILD_NAK_SUFI

DESCRIPTION
  Builds NAK PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsbuild_nak_sufi
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  dsm_item_type *pdu_ptr,
  uint16 bit_offset,
  uint16 pdu_size,                         /* PDU size in bits */
  boolean *nak_built
 , uint16 *first_nak_sn
);

/*===========================================================================

FUNCTION BUILD_MRW_SUFI

DESCRIPTION
  Builds MRW SUFI in a PDU for AM in Uplink.
  We only send one MRW SUFI at a time and wait until rx'd the corresponding
  ACK of this MRW from the peer side.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsbuild_mrw_sufi
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  dsm_item_type *pdu_ptr,
  uint16 bit_offset,
  uint16 pdu_size                     /* PDU size in bits */
);

/*===========================================================================

FUNCTION BUILD_MRW_ACK_SUFI

DESCRIPTION
  Builds NAK PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsbuild_mrw_ack_sufi
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  dsm_item_type *pdu_ptr,
  uint16 bit_offset,
  uint16 pdu_size                       /* PDU size in bits */
);


/*===========================================================================

FUNCTION RESET_RETX_SIZE

DESCRIPTION
  Reset retx_size if the rtx_q is empty.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsreset_retx_size
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk
);

/*===========================================================================

FUNCTION UPDATE_RETX_Q

DESCRIPTION
  When received an ACK, look into the rtx_q to remove all the NAK requests
  that is ACKed now. Update the number of bytes for retransmission.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsupdate_retx_q
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  uint16  ack_sn
);

/*===========================================================================

FUNCTION GENERATE_MRW

DESCRIPTION
  Discard PDUs associated w/ an SDU. Generate MRW for this discarding.
  Enqueue this MRW into mrw_q for next TTI to tx.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsgenerate_mrw
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,
  uint16                rlc_sn,
  boolean               discard_first_sdu_only
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  ,boolean               task_context_flg
#endif
);


/*===========================================================================

FUNCTION UPDATE_MRW_Q

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsupdate_mrw_q
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  uint16  ack_sn
);

/*===========================================================================

FUNCTION REPORT_SDU_TX_FAILURE

DESCRIPTION
  Frees PDUs in txq from vt_a to mrw_sn. Markes the SDU that has been
  reported to the upper layer.
  Updates VT(A) and VT(MS) w/ mrw_sn.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsreport_sdu_tx_failure
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr,
  uint16  mrw_sn,
  uint8   mrw_n
);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_UL_STORE_AM_PDU_LOG_DATA

DESCRIPTION

 Stores the information for AM PDU log packets.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_store_am_pdu_log_data
(
  tdsrlci_ul_am_ctrl_type  *am_ctrl_ptr,
  uint32                log_data,
  uint16                pdu_size
);

/*===========================================================================

FUNCTION RLC_UL_STORE_AM_CIPHER_LOG

DESCRIPTION

 Stores the information for AM PDU cipher log packets.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_store_am_cipher_log
(
  tdsrlci_ul_am_ctrl_type    *am_ctrl_ptr,
  uint32                  key_id,
  uint32                  cnt_c,
  uecomdef_umts_cipher_algo_e_type  cipher_algo,
 // rlc_cipher_algo_e_type  cipher_algo,
  uint16                  rlc_sn
);

#endif

/*===========================================================================

FUNCTION RLC_UL_GET_CTRL_DSM_POOL

DESCRIPTION

  Get one DSM item from the control PDU DSM pool.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_ctrl_pdu_type* tdsrlc_ul_get_ctrl_dsm_pool(void);

/*===========================================================================

FUNCTION  RLC_BUILD_PADDING_ACK_PDU

DESCRIPTION

  Builds Padding ACK PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean: TRUE  - PDU prepared
           FALSE - PDU not prepared.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_build_padding_ack_pdu
(
  tdsrlci_ul_am_ctrl_type  *am_ctrl_ptr,
  uint16                pdu_size,
  uint16                bit_offset,
  dsm_item_type         *pdu_ptr 
);

/*===========================================================================

FUNCTION  RLC_BUILD_CTRL_PDU

DESCRIPTION

  Builds Control PDU for AM in Uplink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_build_ctrl_pdu
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,    /* Poninter to control block     */
  uint16                pdu_size,         /* PDU size in bits              */
  dsm_item_type         *pdu_ptr,         /* Dsm where control PDU is built*/
  uint16                bit_offset,       /* Bit offset with in pdu_ptr    */
  boolean               first_status_pdu  /* TRUE - 1st status PDU for this*/ 
                                          /* LC. FALSE - other wise        */
);


/*===========================================================================

FUNCTION RLC_UL_AM_GET_RETX_PDU_CIPHER_PARAMS

DESCRIPTION

  This computes the ciphering parameters for the re-tx SN.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_am_get_retx_pdu_cipher_params
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,  /* Pointer to ctrl_blk        */
  uint16                retx_sn,        /* re-tx SN                   */
  uint32                *key_index,     /* Key id                     */
  boolean               *enabled,       /* TRUE - ciphering activated */
                                        /* FALSE - Otherwise          */
  uecomdef_umts_cipher_algo_e_type *cipher_algo,
  uint32                *cnt_c          /* COUNT_C                    */
);

/*===========================================================================

FUNCTION RLC_UL_AM_GET_NEW_PDU_CIPHER_PARAMS

DESCRIPTION

  This computes the ciphering parameters for the new DATA PDU.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
//uecomdef_wcdma_cipher_algo_e_type tdsrlc_ul_am_get_new_pdu_cipher_params
uecomdef_umts_cipher_algo_e_type tdsrlc_ul_am_get_new_pdu_cipher_params
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,  /* Pointer to ctrl_blk        */
  uint32                *key_index,     /* Key id                     */
  boolean               *enabled,       /* TRUE - ciphering activated */
                                        /* FALSE - Otherwise          */
  uint32                *cnt_c          /* COUNT_C                    */
);


/*===========================================================================

FUNCTION RLC_UL_AM_COMPUTE_POLL

DESCRIPTION

  Compute whether polling needs to be set in this PDU or not.

DEPENDENCIES
  None.

RETURN VALUE
 boolean - TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_am_compute_poll
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,  /* Pointer to ctrl_blk      */
  uint16                sn,             /* SN of the PDU            */
  tdsrlc_am_pdu_e_type     pdu_type        /* re-tx, control, new PDU  */        
);


/*===========================================================================

FUNCTION RLC_UL_AM_CHECK_SDU_DISCARD

DESCRIPTION

  Validates if this PDU hits MAX_DAT and initiates MRW procedure.

DEPENDENCIES
  None.

RETURN VALUE

  boolean: TRUE  - PDU should be discarded
           FALSE - PDU is valid
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_am_check_sdu_discard
(
  tdsrlci_ul_ctrl_blk_type   *ctrl_blk_ptr,
  uint16                  retx_sn
);

/*===========================================================================

FUNCTION RLC_UL_AM_GET_RETX_SN

DESCRIPTION

  This will compute the next re-tx SN to be transmitted.
  The total_retx_size is also updated accordingly.
  If this SN hits MAX_DAT, and discard mode = TDSRLC_MAXDAT, 
  then MRW is generated.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  uint16 - re-tx SN
  
SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_am_get_retx_sn
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,  /* pointer to uplink control  */
  uint16                pdu_size,        /* PDU size in bits           */
  tdsrlci_missing_list_type  **retxq_next_ptr
);

/*===========================================================================

FUNCTION RLC_UL_AM_FORM_RE_TX_PAYLOAD

DESCRIPTION

  This builds the payload of re-tx PDU using the stored book-keeping
  information.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_am_form_re_tx_payload
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,        /* Pointer to ctrl_blk          */
  uint16                retx_sn,          /* re-tx SN                     */
  tdsrlc_buff_type         *hdr_list,        /* Local place to store headers */
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr /* Place to store IO vectors    */
);

boolean tdsrlc_ul_am_form_new_pdu_payload
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk,        /* Pointer to ctrl_blk          */
  uint16                 pdu_size_bytes,  /* PDU size in bytes            */  
  tdsrlc_buff_type         *hdr_list,        /* Local place to store headers */
  tdsl1_l2_data_type      *l1_ul_tx_buf_ptr /* Place to store IO vectors    */
);


#endif
