#ifndef HDRRMAC4I_H
#define HDRRMAC4I_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       H D R   R E V E R S E   T R A F F I C   C H A N N E L   M A C   
                            
                             P R O T O C O L  (Subtype 4)      

GENERAL DESCRIPTION
  This module contains the internal implementation of the HDR Reverse Traffic 
  Channel Medium Access Control (MAC) Protocol for an HDR access terminal.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.

  Copyright (c) 2007 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmac4i.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/03/14   kss/wsh Added T2P boost for RTT reduction
12/10/13   kss     Added Rev B T2P Boost support.
04/04/14   tnp     Add back variables for FEATURE_HDR_MODEM_API_RL_INFO
03/20/14   rmv     FR16966 - Increased precision for RL rate calculations
11/26/13   rmg     Supported enabling carriers in FW without a frame delay.
10/02/13   rmg     Added support for RL rate estimate and RLP queue info for
                   modem API.
09/16/13   vke     Fix to prevent the bucket level to become zero 
                   after tune back
04/29/13   smd     Disable RevB code if FEATURE_HDR_REVB_SUPPORTED is not defined.
01/15/13   arm     Added changes for rmac tuneaway optimization. 
12/01/09   rmg     Added support to call a cb once TCA is processed by MC RMAC.
05/13/09   rmg     Added field marq_outstanding_pkts to per-chan structure.
03/23/09   wsh     CMI: featurized CMI under FEATURE_CMI
06/26/08   rmg     Removed force_slp_switch from hdrrmac4_vars_struct_type.
04/01/08   cc      Added sp_to_rlsi & sp_to_suspend fields in
                   hdrrmac4_phy_chan_vars_struct_type.
02/09/08   cc      Added force_slp_switch field in hdrrmac4_vars_struct_type.
12/26/07   cc      Removed test_ps_index_fixed field from
                   hdrrmac4_vars_struct_type.
12/13/07   cc      Fixed the bug that a single PilotStrengthPS (instead of
                   per RL) is used to update delta T2P.
11/20/07   cc      Removed "hho"marq" field in hdrrmac4_vars_struct_type.
10/31/07   cc      Added "hho_marq" field in hdrrmac4_vars_struct_type.
10/06/07   cc      Changed "bucket_factor" per flow per carrier.
09/28/07   cc      Added "normal_hdr_inactive" and "enc_status" logging support.
09/27/07   cc      Added RTCMAC and CAPUT priorities fields in 
                   hdrrmac4_phy_chan_vars_struct_type.
09/20/07   rmg     Fixed circular inclusions.
08/15/07   cc      DOrB updates; integrated with rmac4drv.
05/07/07   cc      Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "customer.h"


#ifdef FEATURE_HDR_REVB_SUPPORTED

#include "comdef.h"
#include "dsm.h"
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "hdrind.h"
#include "hdrhai.h"
#include "hdrrmaci.h"
#include "hdrrmac.h"
#include "hdrrmaccmn.h"
#include "hdrrmaccmni.h"
#include "hdrscmrmac4.h"
#include "hdrrmac4drv.h"

/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define HDRRMAC4_INVALID_CHAN_ID           0xFF

/* For interface between RLP and MAC for VT optimization */
typedef struct
{
  uint16 pa_headroom_payload_size;  /* PA headroom limited payload size in bytes */
  uint16 bucket_level_payload_size; /* BucketLevel equivalent payload size in bytes */
  uint16 t2p_inflow_payload_size;   /* T2PInflow equivalent payload size in bytes */
} hdrrmac4_mac_info_type;


#define HDRRMAC4_DATA_TOKEN_INFLOW_INFINITE     0xFFFFFFFF
#define HDRRMAC4_DATA_BUCKET_LEVEL_MAX_INFINITE 0x7FFFFFFF
  /* Most positive 32-bit signed number */


/*---------------------------------------------------------------------------
                             PROTOCOL VARIABLES
---------------------------------------------------------------------------*/
typedef struct
{
  int32                             bucket_factor;
  hdrrmaccmn_i32_tandem_struct_type bucket_level;
  uint32                            bucket_level_sat;
  hdrrmaccmn_u32_tandem_struct_type t2p_inflow;
  int32                             t2p_inflow_db;
  hdrrmaccmn_u16_tandem_struct_type tt2p_hold;
  int32                             delta_t2p_inflow;
  hdrrmaccmn_u32_tandem_struct_type t2p_outflow;
  uint32                            pot_t2p_outflow[HDRRMACCMN_NUM_TX_MODES];
} hdrrmac4_per_nc_bucket_struct_type;

/*---------------------------------*/
/* Per Channel Per Flow variables  */
/*---------------------------------*/
typedef struct
{
  hdrrmac4_per_nc_bucket_struct_type  bucket;
  hdrrmaccmn_queue_info_struct_type   queue_info;
  hdrrmac4drv_qrab_enum_type          qrab_flow;            /* QRAB[i] */
} hdrrmac4_per_nc_vars_struct_type;

typedef struct
{
  hdrpcp_pkt_handle_type        handle;
  dsm_item_type                *pkt_ptr;
  hdrrmac4drv_format_enum_type  format;
} hdrrmac4_pkt_info_struct_type;

typedef enum
{
  HDRRMAC4_CHAN_INACTIVE_STATE=0,
    /* channel is inactive */
  HDRRMAC4_CHAN_ADD_STATE=1,
    /* channel is to be added in the next subframe */
  HDRRMAC4_CHAN_ADD_DELETE_STATE=2,
    /* 
     * an existing channel is to be deleted and a new channel is to be added
     * in the next subframe
     */
  HDRRMAC4_CHAN_SETUP_STATE=3,
    /* channel is in setup state */
  HDRRMAC4_CHAN_OPEN_STATE=4,
    /* channel is in open state */
  HDRRMAC4_CHAN_DELETE_STATE=5
    /* channel is to be deleted in the next subframe */
} hdrrmac4_chan_state_enum_type;

/*------------------------*/
/* Per Channel variables  */
/*------------------------*/
typedef struct
{
  hdrrmac4_chan_state_enum_type       chan_state;
    /* Protocol State Variable for each channel */

  uint8                               log_chan_id;
    /* ReverseLogicalChannel value specified in the RTCAck Message */

  sys_channel_type                    chan_record;
    /* BandClass & ChannelNumber */

  hdrrmaccmn_u8_tandem_struct_type    dropping_rank;
    /* Dropping rank from TCA, lower number to be dropped first */

  hdrrmac_subaset_info_type           subaset_info;
    /* subactive set information for this rl */

  hdrrmac4_per_nc_vars_struct_type    flow[HDRSCMRMACCMN_MAX_NUM_MAC_FLOWS];
    /* Per flow per channel variables */

  hdrrmac4drv_sector_info_type        sector_info;
    /* Serving Best Sub-Active Set information */

  uint16                              max_pkt_size;          /* in octets */
  uint16                              actual_pkt_size;       /* in octets */
  hdrrmaccmn_payload_size_enum_type   pkt_size_index; 
  uint32                              tx_t2p;                /* linear units */
  hdrrmaccmn_tx_mode_enum_type        pkt_tx_mode;
    /* HiCap or LoLat transmissions mode decided by RTCMAC algorith, */

  uint32                              sum_q_outflow;
    /* Aggregate size of active MAC flows */

  uint32                              actual_sum_q_outflow;
    /* Aggregate size of eligible MAC flows */

  uint32                              sum_pot_t2p_outflow[HDRRMACCMN_NUM_TX_MODES];
    /* Aggregate T2P outflow of eligible MAC flows */

  hdrrmac4_pkt_info_struct_type       pkt_info;

  uint16                              curr_tx_pkt_id;
    /* Packet ID of the packet currently being transmitted over the air */

  boolean                             hdr_mode_active;
    /* TRUE if we have not tuned away and RLSI is not in effect */

  hdrrmac4drv_pa_availability_enum_type hc_subpackets_available;
    /* Number of subpackets available for HiCap transmission. */

  hdrrmac4drv_pa_availability_enum_type ll_subpackets_available;
    /* Number of subpackets available for LoLat transmission. */

  int                                   sp_to_rlsi;
    /* Number of subpackets available before RLSI */

  int                                   sp_to_suspend;
    /* Number of subpackets available before 1x Suspend */

  hdrrmaccmn_payload_size_enum_type     rtap_tgt_ps_index;

  uint8                                 rtcmac_pri;
    /* RTCMAC Priority */

  uint8                                 caput_pri;
    /* CAPUT Priority */

  hdrrmac4drv_hdr_mode_enum_type        hdr_inactive_reason;
    /* HDR inactive reason */

  boolean                               marq_outstanding_pkts;
    /* Flag to MARQ outstanding packets on the carrier. */

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
  boolean                             not_data_lmtd;
    /* TRUE if the current subframe is not data limited */

  uint32                              not_data_lmtd_factor;
    /* An IIR filtered value that approximates the percentage of time AT is
       not data limited when transmitting data. The value is stored in
       Q16.16 units. */

  uint32                              not_data_lmtd_tput;
    /* Served throughput in kbps calculated for packets formed when AT was
       not data limited. The value is stored in Q24.8 units. */

  uint8                               mac_lmtd_rri; 
    /* Mac limited RRI */

  uint32                              mac_lmtd_tput;
    /* An IIR filtered value of MAC limited throughput in kbps. The value
       is stored in Q24.8 units. */

  uint32                              tx_on_factor;
    /* An IIR filtered value that approximates the percentage of time
       AT is able to transmit since TX is turned ON.  The value is
       stored in Q16.16 units. */

  uint32                              rl_tput_est;
    /* An estimate of RL throughput in Q24.8 kbps units. */

  uint32                              rl_tput_curr;
    /* Current RL throughput in Q24.8 kbps units. */
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

} hdrrmac4_phy_chan_vars_struct_type;

/* RMAC Subtype 4 DataTokenBucket variables */
typedef struct
{
  hdrrmaccmn_i32_tandem_struct_type data_bucket_level;
    /* DataBucketLevel, in Octets */

  int32                             pot_data_bucket_outflow;
    /* PotentialDataBucketOutflow, in Octets */

  uint32                            sum_transmitted_data;
    /* SumTransmittedData, in Octets */

} hdrrmac4_flow_data_token_vars_struct_type;

/* Non-channel dependent flow variables */
typedef struct
{
  uint32                          bucket_level_max;
  uint32                          last_bucket_level_max;
} hdrrmac4_flow_bucket_vars_struct_type;

/* Non-channel dependent queue variables */
typedef struct
{
  uint32                          initial_size;
    /* Q[i][n] before allocated to any reverse channel */
  uint32                          remaining_size;
    /* Q[i][n] remains to be allocated */
} hdrrmac4_flow_queue_info_struct_type;

/*---------------------*/
/* Per Flow variables  */
/*---------------------*/
typedef struct
{
  hdrrmac4_flow_data_token_vars_struct_type data_token;

  hdrrmac4_flow_bucket_vars_struct_type     bucket;

  hdrrmac4_flow_queue_info_struct_type      queue_info;

} hdrrmac4_flow_vars_struct_type;

/* Logical Channel Structure */
typedef struct
{
  uint8                                     phy_chan_id;
    /* 
     * Value is used to select the physical channel.
     * HDRRMAC4_INVALID_CHAN_ID indicates that the channel is not assigned 
     */
} hdrrmac4_log_chan_struct_type;

/*---------------------------*/
/* RMAC Subtype 4 variables  */
/*---------------------------*/
typedef struct
{
  hdrrmac4_phy_chan_vars_struct_type  
    phy_chan[HDRRMAC4DRV_MAX_NUM_REV_PHY_CHANS];

  hdrrmac4_flow_vars_struct_type      data_flow[HDRSCMRMACCMN_MAX_NUM_MAC_FLOWS];

  hdrrmac4_log_chan_struct_type       log_chan[HDRSCMRMAC4_MAX_NUM_REV_LOG_CHANS];
    /* 
     * Logical channel to physical channel mapping.  Indexed by the
     * ReverseLogicalChannel in the RTCAck Message.
     */

  uint8                               frame_offset;
    /* Transmit frame offset.  */

  uint8                               num_open_state_chans;
    /* Total number of channels that are currently in the OPEN state */

  uint8                               num_inactive_state_chans;
    /* Total number of channels that are currently in the INACTIVE state */

  uint8                               slp_carrier_idx;
    /* Physical channel index of the designated SLP carrier */

  hdrrmaccmn_tx_status_enum_type      tx_status;
    /* Status of RF and PA. */

  boolean                             data_allowed;
    /* Overall gatekeeper of whether data is allowed. This flag indicates if
     * we have tuned away from HDR
     */

  boolean                             disallow_slp_switch;
    /* Flag to indicate if SLP carrier change is allowed */

  hdrhai_ati_record_type              ati_record;
    /* Access Terminal Identifier (ATI) record. */

  boolean                             enc_window_missed;
    /* Encoder window missed flag */

  hdrrmac_tca_processed_cb_type      tca_processed_cb_ptr;
    /* Pointer to callback to be called once TCA is processed by MC RMAC */

  uint8                               min_term_target; 
    /* Minimum termination targets across both transmission modes
       and all packet sizes */
  boolean                             graceful_ta_started;
    /* Point when rmac stops sending packets because they cannot be completed
       before actual tuneaway */

  boolean                             graceful_ta_done;
    /* Flag to indicate that now the graceful TA procesing is done */

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
  uint32                              rlp_q_size;
    /* RLP queue size in bits. */

  uint32                              rl_tput_est;
    /* An estimate of overall RL throughput in Q24.8 kbps units. */

  uint32                              rl_tput_curr;
    /* Current overal RL throughput in Q24.8 kbps units. */

  uint8                               rl_info_update_counter;
    /* Estimated and current RL throughput values are updated periodically
       with the help of this counter. */

  boolean                             plus_flag;
    /* TRUE if the actual throughput estimate may be greater than the
       provided value. */

  uint8                               plus_flag_reset_counter;
    /* The plus flag is valid for a set number of subframes.  It should be
       reset once this counter goes to zero. */

  rex_crit_sect_type                  rl_info_crit_sect;
    /* Critical section for management of RL rate quantities */

  uint8                               rl_info_read_buffer_idx;
    /* Index of buffer from which RL rates can be read */

  hdrrmaccmn_rl_info_struct_type      rl_info[HDRRMACCMN_RL_INFO_BUF_SIZE];
    /* Double buffer for RL info storage and reporting */
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST
  hdrrmaccmn_t2p_boost_struct_type    t2p_boost;
    /* Storage for T2P boost related data */

  boolean                             pre_ta_opt_enabled;
    /* TRUE if pre-tuneaway optimization is enabled */

  boolean                             post_tb_opt_enabled;
    /* TRUE if post-tuneback optimization is enabled */

#ifdef FEATURE_HDR_RMAC_RTT_REDUCTION_T2P_BOOST
  boolean                             rtt_reduct_opt_enabled;
    /* TRUE if rtt reduction is enabled. Note it depends on
       overall boost feature */  
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */

  boolean                             t2p_boost_enabled;
    /* Overall boost enable. Updates based on NV and valid 
       configuration. */

  uint16                              boosted_rri_mask[HDRRMAC4DRV_MAX_NUM_REV_PHY_CHANS];
    /* Mask with bit positions corresponding to T2P boosted RRIs
       in current subframe set */

#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */

#ifdef FEATURE_HDR_BOLT_MODEM
  uint8                               carrier_wav_enable_mask;
    /* Mask to enable carrier in FW microkernel */
#endif /* FEATURE_HDR_BOLT_MODEM */

} hdrrmac4_vars_struct_type;


#endif /* FEATURE_HDR_REVB_SUPPORTED */

#endif /* HDRRMAC4I_H */

