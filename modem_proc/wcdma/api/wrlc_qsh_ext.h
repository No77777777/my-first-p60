#ifndef RLC_QSH_EXT_H
#define RLC_QSH_EXT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WRLC QSH EXT . H                 

GENERAL DESCRIPTION
   Contains declarations for function which are QUALCOMM proprietary 
   and may or may not be shipped as source code.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/28/15   aa      Adding events and handler support to QSH
06/01/15   scv     Code changes for QSH
06/01/15   scv     Initial version for wrlc_qsh_ext.h

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qsh.h"

#define WRLC_QSH_MAJOR_VER 0
#define WRLC_QSH_MINOR_VER 1

typedef enum
{
  WRLC_QSH_METRIC_RLC_STAT_E,
  WRLC_QSH_METRIC_MAX_STAT_E
}wrlc_qsh_metric_rlc_e;

typedef struct
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */

  /* Indicates number of RESET PDU received in DL */
  uint16 num_rlc_rst_dl;

  /* Indicates number of RESET PDU sent in UL */
  uint16 num_rlc_rst_ul;
  uint16 rlc_fc_win_size;
  uint16 num_rlc_urecov_err;
  uint32 num_pdu_bytes_tx;
  uint32 num_pdu_bytes_retx;
  uint32 rlc_dl_tput;
  uint32 rlc_ul_tput;
}wrlc_qsh_metric_data_statistics_s;

/* Number of Logical Channels to log as part of reduced global logging effort */
#define UE_MAX_RLC_ENTITIES_REDUCED_LOG    5

/*---------------------------------------------------------------------------
  Enumerated type that defines the RLC Uplink Logical Channel states.
---------------------------------------------------------------------------*/
typedef enum {
  WRLC_QSH_UL_NULL_STATE = 0,                    /* Logical channel is currently not*/
                                        /* in use.                         */
  WRLC_QSH_UL_DATA_TRANSFER_READY,               /* Logical channel has been        */
                                        /* established, and data transfer  */
                                        /* is allowed.                     */
  WRLC_QSH_UL_WAITING_TO_SUSPEND,                /* RRC has sent a suspend request  */
                                        /* with N value greater than 0, and*/
                                        /* RLC has not yet transmitted     */
                                        /* SN + N PDUs. Only applicable to */
                                        /* UM and AM channels.             */
  WRLC_QSH_UL_SUSPENDED = 0x04,                  /* Data transfer on the logical    */
                                        /* channel has been temporarily    */
                                        /* suspended. Only applicable to UM*/
                                        /* and AM channels.                */
  WRLC_QSH_UL_RESET_PENDING = 0x08,              /* RESET has been sent to the peer */
                                        /* entity, and no data transfer is */
                                        /* allowed until a RESET_ACK is    */
                                        /* received. Only applicable to AM */
                                        /* channels.                       */
  WRLC_QSH_UL_CONFIG_PENDING = 0x10,             /* Waiting for the DL side to      */
                                        /* complete its config             */
  WRLC_QSH_UL_STOP = 0x20                        /* Stop all of the transmission    */ 
} wrlc_qsh_ul_state_e;


typedef enum {
  WRLC_QSH_DL_NULL_STATE = 0,                    /* Logical channel is currently not */
                                        /* in use.                          */
  WRLC_QSH_DL_DATA_TRANSFER_READY,               /* Logical Channel has been         */
                                        /* established, and data transfer   */
                                        /* is allowed.                      */
  WRLC_QSH_DL_RESET_PENDING,                     /* RESET has been sent to the peer  */
                                        /* RLC entity,and no data transfer  */
                                        /* is allowed unitl a RESET ACK is  */
                                        /* received. Only applicable for    */
                                        /* AM channels.                     */
  WRLC_QSH_DL_STOP = 0x04                        /* Stop to receive any transmission */
} wrlc_qsh_dl_state_e;


/*--------------------------------------------------------------------------- 
  Type to establish, release or re-configure a single acknowledged 
  mode entity.
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint16 ul_rlc_size;         /* Max. PDU size.         */
  uint8  ul_li_size;          /* LI size                         */
  uint16 ul_flex_min_pdu_size;/* Minimum PDU size                */
  uint16 ul_flex_max_pdu_size;/* Maximum PDU size                */
} wrlc_qsh_ul_am_config_type_s;

/*--------------------------------------------------------------------------- 
  Used to store the necessary up-link control state variables 
  for an RLC AM entity.
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint16  vt_s; /* VT(S) - Send State              */
  uint16  vt_a; /* VT(A) - Acked State             */
  uint16  vt_wins; /* Tx_Window_size                  */
} wrlc_qsh_ul_am_state_type_s;

/*--------------------------------------------------------------------------- 
  Store reduced up-link control information for an RLC AM entity.
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  wrlc_qsh_ul_am_config_type_s ul_cnfg_var;     /* pointer to config information from RRC.  */
  wrlc_qsh_ul_am_state_type_s  ul_state;        /* Control state variables.      */
} wrlc_qsh_ul_am_ctrl_type_s;

/*--------------------------------------------------------------------------- 
  Typedef for storing reduced parameters specific to unacknowledged mode.
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint8   li_length;                    /* Length of the length indicator  */
                                        /* - 7 or 15 bits                  */
  uint8   vt_us;                        /* Sequence Number of the next in- */
                                        /* Sequence Pdu to be transmitted. */
} wrlc_qsh_ul_um_ctrl_type_s;

/*--------------------------------------------------------------------------- 
  Typedef for storing reduced logging parameters 
---------------------------------------------------------------------------*/
typedef union {
  wrlc_qsh_ul_um_ctrl_type_s um_ctrl;        /* Unacknowledged Mode parameters.  */
  wrlc_qsh_ul_am_ctrl_type_s am_ctrl;        /* Acknowledged Mode parameters.    */
} wrlc_qsh_ul_ctrl_u_type_s;

/*---------------------------------------------------------------------------
  Used to store reduced information about a uplink logical channel for MAV
---------------------------------------------------------------------------*/
typedef  struct {
  wrlc_qsh_ul_state_e lc_state;        /* Uplink Logical channel state    */
  uint8 rlc_id;
  wrlc_qsh_ul_ctrl_u_type_s ctrl;     /* Union of TM, UM and AM specific */
                                        /* parameters.                     */
} wrlc_qsh_ul_ctrl_blk_type_s;


typedef PACKED struct PACKED_POST {
  
  uint8 active_ch;                      /* Number of active uplink logical */
                                        /* channels.                       */
  uint8 num_act_tm_ch;

  wrlc_qsh_ul_ctrl_blk_type_s    ctrl_blk[UE_MAX_RLC_ENTITIES_REDUCED_LOG];
                                        /* Data structure used to store all*/ 
                                        /* the information for the uplink  */
                                        /* logical channel.                */
} wrlc_qsh_ul_table_type_s;


typedef PACKED struct PACKED_POST {
  uint8 li_length;                      /* Length of the length indicator  */
                                        /* - 7 or 15 bits                  */
  uint8 vr_ur;                          /* Sequence Number of the next Pdu */
                                        /* to be received. This value is   */
                                        /* set to RLC_SN + 1 upon reception*/
                                        /* of a PDU                        */
} wrlc_qsh_dl_um_ctrl_type_s;


typedef PACKED struct PACKED_POST {
  uint32  dl_rlc_size;                   /* Max. PDU size.                  */
  boolean am_pdu_size_flexbile;
  uint8   am_li_type;
} wrlc_qsh_dl_am_config_type_s;

/*---------------------------------------------------------------------------
  Used to store the necessary down-link control state information for an
  RLC AM entity.
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint16 old_vr_r;
  uint16 vr_r;                          /* VR(R) -- Rx state               */
  uint16 vr_h;                          /* VR(H) -- Expected Rx state      */
  uint16 prev_vr_h;                     /* PREV_VR_H - Store the old value */
                                        /* of VR(H)                        */
  uint8 li_length;                      /* Length of the length indicator  */
                                        /* value 1 means 7 bits            */
                                        /* Value 2 means 15 bits           */
  uint16 last_sn_processed;
} wrlc_qsh_dl_am_state_type_s;

typedef PACKED struct PACKED_POST {
  wrlc_qsh_dl_am_config_type_s  dl_ctl_cnfg;   /* pointer to dl control config variables.    */
  wrlc_qsh_dl_am_state_type_s   dl_state_var;  /* dl control state variables.     */
} wrlc_qsh_dl_am_ctrl_type_s;

/*---------------------------------------------------------------------------
  Union for storing reduced parameters specific to a mode.
---------------------------------------------------------------------------*/
typedef union {
  wrlc_qsh_dl_um_ctrl_type_s um_ctrl;         /* Unacknowledged Mode parameters. */
  wrlc_qsh_dl_am_ctrl_type_s am_ctrl;         /* Acknowledged Mode parameters.   */
} wrlc_qsh_dl_ctrl_u_type_s;


/*---------------------------------------------------------------------------
  Used to store reduced information about a downlink logical channel.
---------------------------------------------------------------------------*/
typedef  struct {
  wrlc_qsh_dl_state_e lc_state;        /* Downlink Logical channel state  */
  uint8 rlc_id;
  wrlc_qsh_dl_ctrl_u_type_s  ctrl;     /* Union of UM and AM specific     */
                                                /* parameters.                     */
} wrlc_qsh_dl_ctrl_blk_type_s;


/*---------------------------------------------------------------------------
  Database that is used to store reduced information about active downlink 
  logical channels. Maximum number of channels is 8 for reduced logging.
---------------------------------------------------------------------------*/

typedef PACKED struct PACKED_POST {
   
  uint8 active_ch;                      /* Num of active logical channels. */
  uint8 num_act_tm_ch;
  wrlc_qsh_dl_ctrl_blk_type_s ctrl_blk[UE_MAX_RLC_ENTITIES_REDUCED_LOG];
                                        /* Data structure used to store reduced */
} wrlc_qsh_dl_table_type_s;

typedef enum
{
  WRLC_QSH_DUMP_TAG_WCDMA_RLC_E,
}wrlc_qsh_dump_tag_e;

typedef enum{
  WRLC_QSH_EVENT_MAX_RESET = 0, /*(RARE)        MAX RESET Event*/
  WRLC_QSH_EVENT_MAX_EVENT      /*Internal - for bounds checking*/
}wrlc_qsh_event_e;

typedef PACKED struct PACKED_POST
{
  /* this header will have a tag and length */
  qsh_dump_tag_hdr_s hdr;
  uint8              as_id;
  wrlc_qsh_ul_table_type_s wcdma_rlc_ul;
  wrlc_qsh_dl_table_type_s wcdma_rlc_dl;
} wrlc_qsh_dump_tag_ul_dl_s;

#endif

