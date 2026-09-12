#ifndef HDRRMACCMNI_H
#define HDRRMACCMNI_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       H D R   R E V E R S E   T R A F F I C   C H A N N E L   M A C   
                            
                P R O T O C O L   H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains the implementation of the HDR Reverse Traffic Channel 
  Medium Access Control (MAC) Protocol for an HDR access terminal.  This
  module currently supports Subtype 3 and Subtype 4 RTCMAC Protocols.

EXTERNALIZED FUNCTIONS
    

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2007 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmaccmni.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/19/14   wsh     Disallow DTX if TxPwr is too high 
08/12/14   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold
04/03/14   kss/wsh Added T2P boost for RTT reduction
12/10/13   kss     Added Rev B T2P Boost support.
10/02/13   rmg     Added support for RL rate estimate for modem API.
08/28/13   rmg     Fixed a calculation error in T2P decay calculation.
03/16/12   arm     Resolve TCB errors for Dime. 
11/07/11   vlc     Fixed Klocwork warnings. 
01/11/08   cc      Removed hdrrmaccmn_pkt_state_struct_type definition.
09/24/07   cc      Added time stamp functions to support logging.
09/20/07   rmg     Fixed circular inclusions.
08/15/07   cc      DOrB updates; integrated with rmac4drv.
05/07/07   cc      Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "hdrtxi.h"
#include "hdrbit.h"
#include "hdrpcp.h"
#include "hdrrmac.h"
#include "hdrrmaccmn.h"
#include "hdrscmrmaccmn.h"
#include "hdrrmac4.h"

/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#define HDRRMACCMN_MAC_FLOW_ID_INVALID        0xFF

/*---------------------------------------------------------------------------
                             PROTOCOL MESSAGES
---------------------------------------------------------------------------*/

/* Macro to retrieve the Message ID field. */
#define HDRRMACCMN_GET_MSG_ID( src_ptr, dest_ptr )\
          HDRBIT_UNPACK8( src_ptr, dest_ptr, 0, 8 )

/* Macro to set the Message ID field. */
#define HDRRMACCMN_SET_MSG_ID( ptr, val )\
          ( *( (byte*)(ptr) ) = (byte)(val) )

/* Macro to retrieve the transaction ID */
#define HDRRMACCMN_GET_TRANS_ID( src_ptr, dest_ptr )\
          HDRBIT_UNPACK8( src_ptr, dest_ptr, 8, 8 )

/* Macro to set the transaction ID */
#define HDRRMACCMN_SET_TRANS_ID( ptr, val )\
          ( *( (byte*)(ptr) + 1 ) = (byte)(val) )

typedef uint8 hdrrmaccmn_msg_id_type;

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Task context block pointer. */
#define HDRRMACCMN_TASK_PTR        HDRTX_TASK_PTR

/* Task signals allocated to this protocol. */
#define HDRRMACCMN_ISR_SIG         HDRTX_RMAC_ISR_SIG

/* Maximum payload size in bytes */
#define HDRRMACCMN_MAX_PAYLOAD_SIZE 1532 

/* Time stamp sample point definitions to support profiling */
typedef enum
{
  HDRRMACCMN_TS_0         = 0,
    /* start of hdrrmac4drv_get_prev_tx_status() */
  HDRRMACCMN_TS_1         = 1,
    /* start of hdrpcp_report_prev_subframe_status() */
  HDRRMACCMN_TS_2         = 2,
    /* end of hdrpcp_report_prev_subframe_status() */
  HDRRMACCMN_TS_3         = 3,
    /* start of hdrpcp_get_packet_info() */
  HDRRMACCMN_TS_4         = 4,
    /* end of hdrpcp_get_packet_info() */
  HDRRMACCMN_TS_5         = 5,
    /* start of hdrpcp_get_packet() */
  HDRRMACCMN_TS_6         = 6,
    /* end of hdrpcp_get_packet() */
  HDRRMACCMN_TS_7         = 7,
    /* start of hdrrmac4drv_send_packet() */
  HDRRMACCMN_TS_8         = 8,
    /* end of hdrrmac4drv_send_packet() */
  HDRRMACCMN_TS_MAX       = 9
} hdrrmaccmn_ts_point_type;

/* frame_build_time */
#define HDRRMACCMN_TS_FRAME_BUILD_START     HDRRMACCMN_TS_0
#define HDRRMACCMN_TS_FRAME_BUILD_END       HDRRMACCMN_TS_7

/* frame_load_time */
#define HDRRMACCMN_TS_FRAME_LOAD_START      HDRRMACCMN_TS_7
#define HDRRMACCMN_TS_FRAME_LOAD_END        HDRRMACCMN_TS_8

/* report_pkt_status_time */
#define HDRRMACCMN_TS_RPT_PKT_STATUS_START  HDRRMACCMN_TS_1
#define HDRRMACCMN_TS_RPT_PKT_STATUS_END    HDRRMACCMN_TS_2

/* get_pkt_info_time */
#define HDRRMACCMN_TS_GET_PKT_INFO_START    HDRRMACCMN_TS_3
#define HDRRMACCMN_TS_GET_PKT_INFO_END      HDRRMACCMN_TS_4

/* get_pkt_time */
#define HDRRMACCMN_TS_GET_PKT_START         HDRRMACCMN_TS_5
#define HDRRMACCMN_TS_GET_PKT_END           HDRRMACCMN_TS_6


typedef hdrrmac_app_flow_struct_type      hdrrmaccmn_app_flow_struct_type;

typedef struct
{
  uint8                             t2p_trans_subframes;
    /* 
     * Number of subframes to use the pre-transition T2P values for a given 
     * packet size. Out of the maximum 4 possible transmissions (subpackets) 
     * of a given packet this tells the AT at which subpacket it can 
     * transition its T2P.
     */

  uint8                             term_target;
    /* 
     * Expected number of subframes needed to achieve the target Physical
     * Layer erasure rate.
     */
  
  uint8                             t2p_pre_trans;
    /* 
     * T2P value to be used BEFORE transitioning to a possibly different 
     * (pre_transition) T2P value.
     */
  
  uint8                             t2p_post_trans;
    /* 
     * T2P value to be used AFTER transitioning to a possibly different 
     * (post_transition) T2P value.
     */

} hdrrmaccmn_power_params_struct_type;

/* Since the T2PUp/Down & BucketFactor grids have different units this
 * function is passed as an argument to the hdrrmaccmn_interpolate_2d()
 * routine so it can do the scaling properly
 */
typedef int32 ( *hdrrmaccmn_scaling_func_type )( void *input );

/* Bit map representation of set of packet sizes: {128, 255, ..., 12288}
 * Using this representation set operations (mainly set diff, required in 
 * the procedure to find the Physical Layer packet size) become bit-wise
 * operations
 */
typedef uint16 hdrrmaccmn_ps_set_type;

/* TODO make these consts */
#define HDRRMACCMN_FRAB_FILT_SCALING_FACTOR       1536
#define HDRRMACCMN_PILOT_STRENGTH_SCALING_FACTOR   256
#define HDRRMACCMN_QRAB_FILT_SCALING_FACTOR        256
#define HDRRMACCMN_T2P_FILT_SCALING_FACTOR         384
#define HDRRMACCMN_T2P_NO_TX_FILT_SCALING_FACTOR   768
#define HDRRMACCMN_ROUND_DOWN_LOOP_COUNT             3

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
/* Filter time constant in units of subframes for filtering quantities 
 * associated with ModemAPI implementation 
 */
#define HDRRMACCMN_MODEMAPI_FILT_TC                 64
#define HDRRMACCMN_MODEMAPI_FILT_SCALING_FACTOR      \
              HDRRMACCMN_T2P_NO_TX_FILT_SCALING_FACTOR

/* Once set, the plus flag for throughput estimate should be reset
 * after this many subframes
 */
#define HDRRMACCMN_PLUS_FLAG_RESET_CNT              64

/* Period in subframes between consecutive throughput value updates */
#define HDRRMACCMN_TPUT_UPDATE_PERIOD                8

/* Period in frames between consecutive throughput value updates */
#define HDRRMACCMN_REL0_TPUT_UPDATE_PERIOD (HDRRMACCMN_TPUT_UPDATE_PERIOD >> 2)

/* Threshold at which collected RL metrics are reset */
extern const uint32 hdrrmaccmn_stats_reset_threshold;

/* Reverse link information struct type */
typedef struct
{
  uint32                              rlp_q_size;
    /* RLP queue size in bits. */

  uint16                              rl_tput_est;
    /* An estimate of overall RL throughput in kbps units. */

  uint16                              rl_tput_curr;
    /* Current overal RL throughput in kbps units. */

  boolean                             plus_flag;
    /* TRUE if the actual throughput estimate may be greater than the
       provided value. */

} hdrrmaccmn_rl_info_struct_type;

/* Size of RL info buffer */
#define HDRRMACCMN_RL_INFO_BUF_SIZE              2

#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

/* Overall RMAC TX Status type */
typedef enum
{
  TX_START_PENDING,
  TX_STARTED,
  TX_SUSPEND_PENDING,
  TX_SUSPENDED
} hdrrmaccmn_tx_status_enum_type;

/* Tandem structure type (previous & current) for uint32 quantity */
typedef struct
{
    uint32                         prev;
    uint32                         curr;
} hdrrmaccmn_u32_tandem_struct_type;

/* Tandem structure type (previous & current) for int32 quantity */
typedef struct
{
    int32                          prev;
    int32                          curr;
} hdrrmaccmn_i32_tandem_struct_type;

/* Tandem structure type (previous & current) for uint16 quantity */
typedef struct
{
    uint16                         prev;
    uint16                         curr;
} hdrrmaccmn_u16_tandem_struct_type;

/* Tandem structure type (previous & current) for int16 quantity */
typedef struct
{
    int16                          prev;
    int16                          curr;
} hdrrmaccmn_i16_tandem_struct_type;

/* Tandem structure type (previous & current) for uint8 quantity */
typedef struct
{
    uint8                          prev;
    uint8                          curr;
} hdrrmaccmn_u8_tandem_struct_type;

/* Per flow (per channel) queue info. structure */
typedef struct
{
  uint32                           initial_size;   /* Q[i] in octets */
  uint32                           suggested_size; /* d'[i] in octets */
  uint32                           actual_size;    /* d[i] in octets */
  uint32                           q_outflow[HDRRMACCMN_NUM_TX_MODES];
} hdrrmaccmn_queue_info_struct_type;

/*
 * Structure to store information that can be used to report MARQ, packet
 * acceptance, update T2POutflow and BucketLevel from the previous subframes.
 */
typedef struct
{
  hdrpcp_mac_flow_allocation_type   flows[HDRSCMRMACCMN_MAX_NUM_MAC_FLOWS];
  uint8                             num_flows;
  hdrrmaccmn_tx_mode_enum_type      tx_mode;
  hdrrmaccmn_payload_size_enum_type pkt_size_index;
  uint16                            sum_payload;
  hdrpcp_pkt_handle_type            pkt_handle;
} hdrrmaccmn_decision_hist_struct_type;

/* Valid Physical Layer packet sizes (in octets) excluding Physical Layer
 * overhead
 */
extern const uint16 hdrrmaccmn_pkt_size_array[];

/* The UNIVERSAL set of packet sizes has 12 elements = {128, 256,...,12288} */
extern const hdrrmaccmn_ps_set_type hdrrmaccmn_univ_ps_set;

/* The NULL set of packet sizes has 0 elements */
extern const hdrrmaccmn_ps_set_type hdrrmaccmn_null_ps_set; 

extern const int32 hdrrmaccmn_q_20_scaling_factor;  /* 2^20 */
extern const int32 hdrrmaccmn_q_15_scaling_factor;  /* 2^15 */
extern const int32 hdrrmaccmn_q_10_scaling_factor;  /* 2^10 */
extern const int32 hdrrmaccmn_neg_infinity;         /* -2^31 */

/*---------------------------------------------------------------------------
                         RMAC ST-3/4 CONFIG ATTRIBS LUT
---------------------------------------------------------------------------*/
/* SupportGAUPMaxNumSubPackets */
extern const boolean hdrrmaccmn_is_max_sub_pkts_gaupable[];

/* T2PNoTxFilterTC in subframes */
extern const uint32 hdrrmaccmn_tx_t2p_no_tx_filter_tc[];

/* PayloadThresh. Note: 0xE denotes infinite number of octets */
extern const hdrrmaccmn_payload_size_enum_type hdrrmaccmn_payload_thresh[];

/* MergeThreshold in octets. Note: 65535 denotes infinite octets */
extern const uint16 hdrrmaccmn_merge_thresh[];

/* FRABLow. {-0.2, -0.4, -0.6, -0.8, -1.0} in Q.15 */
extern const int16 hdrrmaccmn_frab_low[];

/* RPCStep field of CommonPowerParameters Attribute.
   enum {0, 1, 2, 3} to {0.5, 1.0, 1.5, 2.0} dB scaled by Q.2 */
extern const uint16 hdrrmaccmn_com_pow_rpc_step[];

/* Per-Flow Attributes */

/* Per-flow merge threshold in octets. Note: 65535 denotes infinite octets */
extern const uint16 hdrrmaccmn_pf_merge_thresh[];

/* Per-flow Burst Duration Factor in Sub-frames */
extern const uint16 hdrrmaccmn_pf_burst_duration_factor[];

/* Per-Flow T2PFilterTC in sub-frames */
extern const uint16 hdrrmaccmn_pf_t2p_filt_tc[];


/*===========================================================================

                      FUNCTION  DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION HDRRMACCMN_GET_CONFIGS_AND_VARS_MEM

DESCRIPTION
  This function is used by individual RTCMAC protocol subtype to retrieve
  the memory location to store the various configuration and computation
  variables.
  
DEPENDENCIES
  None.

PARAMETERS
  subtype           - Subtype of RTCMAC retrieving the information.
  scm_cfg_ptr_ptr   - Address of the pointer variable points to memory
                      location storing the protocol attributes in SCM OTA 
                      format.
  rmac_cfg_ptr_ptr  - Address of the pointer variable points to memory
                      location storing the protocol attributes in protocol 
                      internal format.
  rmac_var_ptr_ptr  - Address of the pointer variable points to memory
                      location storing the protocol computation variables.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_get_configs_and_vars_mem
(
  uint16  subtype,
  void    **scm_cfg_ptr_ptr,
  void    **ramc_cfg_ptr_ptr,
  void    **rmac_var_ptr_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_AUX_PILOT_CHAN_PARAMS

DESCRIPTION
  This function is used to translate the AuxiliaryPilotChannelParameters from
  OTA format to SW format.

  1. AuxiliaryPilotChannelGain     
     OTA format: 4 bit value in units of -1 dB
     SW Format:  RMAC4 DRIVER: 16 bit value in units of dB scaled by Q.2
                 RMAC4 ALGO:   Linear in Q6.10.

  2. AuxiliaryPilotChannelMinPayload
     OTA format: Encoded as index specified in table 10.11.7.2.2-1
     SW format:  16-bit value encoded by enum hdrrmaccmn_payload_size_enum_type 
  
DEPENDENCIES
  None.

PARAMETERS
  rmac_aux_pilot_info_ptr - pointer to the AuxiliaryPilotChannelParameters ub
                            RMAC SW internal format
  scm_aux_pilot_info_ptr  - pointer to the AuxiliaryPilotChannelParameters in
                            OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_aux_pilot_chan_params
(
  hdrrmaccmn_aux_pilot_struct_type    *rmac_aux_pilot_info_ptr,
  hdrscmrmaccmn_aux_pilot_struct_type *scm_aux_pilot_info_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_MAX_NUM_SUB_PKTS__CC

DESCRIPTION
  This function is used to translate the MaxNumSubPackets from OTA format to 
  SW format.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_max_num_sub_pkts__cc(void);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_PERMITTED_PAYLOAD

DESCRIPTION
  This function is used to translate the PermittedPayload from OTA format to 
  SW format.

  SW stores this attribute in the OTA format and converts it before use to
  conserve RAM. So just memcpy-ing.

DEPENDENCIES
  None.

PARAMETERS
  rmac_permitted_payload_ptr  - Pointer to the PermittedPayload in RMAC SW
                                internal format
  scm_permitted_payload_ptr   - Pointer to the PermittedPayload in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_permitted_payload
(
  hdrrmaccmn_permitted_payload_size_type  *rmac_permitted_payload_ptr,
  hdrscmrmaccmn_permitted_payload_type    *scm_permitted_payload_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_PILOT_STRENGTH__CONFIG_XX

DESCRIPTION
  This function is used to translate the PilotStrength from OTA format to 
  SW format. This function also sorts the pilot strength axis values in 
  increasing order.

DEPENDENCIES
  None.

PARAMETERS
  rmac_pilot_strength_ptr - Pointer to the PilotStrength in RMAC SW
                            internal format
  scm_pilotstrength_ptr   - Pointer to the PilotStrength in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_pilot_strength__config_xx
(
  hdrrmaccmn_pilot_strength_grid_struct_type    *rmac_pilotstrength_ptr,
  hdrscmrmaccmn_pilot_strength_grid_struct_type *scm_pilotstrength_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_TXT2PMAX__CONFIG_XX

DESCRIPTION
  This function is used to translate the TxT2PMax from OTA format to 
  SW format. This function also sorts the pilotstrength axis values in 
  increasing order.

DEPENDENCIES
  None.

PARAMETERS
  rmac_txt2p_max_ptr  - Pointer to the TxT2PMax in RMAC SW internal format
  scm_txt2pmax_ptr    - Pointer to the TxT2PMax in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_txt2pmax__config_xx
(
  hdrrmaccmn_txt2p_max_grid_struct_type     *rmac_txt2pmax_ptr,
  hdrscmrmaccmn_txt2p_max_grid_struct_type  *scm_txt2pmax_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_REQ_PARAMS

DESCRIPTION
  This function is used to translate the RequestParameters from OTA format to 
  SW format.

  1. MaxRequestInterval
     OTAFormat: 4-bit number in units of frames. Range: 1-15 frames
     SW Format: 8-bit number in units of frames. Range: 1-15 frames.

  2. ReqRatio
     OTAFormat: 4-bit value in units of 0.005. Range: 0 - 0.075
     SWFormat:  8-bit value in units of 0.005. 

DEPENDENCIES
  None.

PARAMETERS
  rmac_req_params_ptr - Pointer to the RequestParameters in RMAC SW internal
                        format
  scm_req_params_ptr  - Pointer to the RequestParameters in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_req_params
(
  hdrrmaccmn_req_params_struct_type     *rmac_req_params_ptr,
  hdrscmrmaccmn_req_params_struct_type  *scm_req_params_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_BUCKET_LEVEL_MAX_NN

DESCRIPTION
  This function is used to translate the BucketLevelMaxNN from OTA format to 
  SW format.

DEPENDENCIES
  None.

PARAMETERS
  rmac_bucket_level_max_ptr - Pointer to the BucketLevelMaxNN in RMAC SW
                              internal format
  scm_bucket_level_max_ptr  - Pointer to the BucketLevelMaxNN in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_bucket_level_max_nn
(
  uint32    *rmac_bucket_level_max_ptr,
  uint8     *scm_bucket_leve_max_ptr
);

 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_BUCKET_FACTOR_NN

DESCRIPTION
  This function is used to translate the BucketFactorNN from OTA format to 
  SW format.

  SW stores this attribute in the OTA format and converts it before use to
  conserve RAM. So just memcpy-ing.

DEPENDENCIES
  None.

PARAMETERS
  rmac_bf_ptr - Pointer to the BucketFactorNN in RMAC SW internal format
  scm_bf_ptr  - Pointer to the BucketFactorNN in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_bucket_factor_nn
(
  hdrrmaccmn_t2p_frab_grid_struct_type    *rmac_bf_ptr,
  hdrscmrmaccmn_t2p_frab_grid_struct_type *scm_bf_ptr
);

 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_GU_GD_NN__CONFIG_XX

DESCRIPTION
  This function is used to translate the T2PTransitionFunctionNN from OTA 
  format to SW format.

  SW stores this attribute in the OTA format and converts it before use to
  conserve RAM. So just memcpy-ing.

DEPENDENCIES
  None.

PARAMETERS
  rmac_gu_gd_ptr  - Pointer to the T2PTransitionFunction in RMAC SW internal
                    format
  scm_gu_gd_ptr   - Pointer to the T2PTransitionFunction in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_gu_gd_nn__config_xx
(
  hdrrmaccmn_gu_gd_struct_type    *rmac_gu_gd_ptr,
  hdrscmrmaccmn_gu_gd_struct_type *scm_gu_gd_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_T2P_INFLOW_RANGE_NN__CONFIG_XX

DESCRIPTION
  This function is used to translate the T2PInflowRangeNN from OTA 
  format to SW format.

  1. T2PInflowMin
     OTA Format: 8-bit value in units of 0.25 dB
     SW Format:  32-bit unsigned linear value from dB scaled by Q22.10

  2. T2PInflowMax
     OTA Format: 8-bit value in units of 0.25 dB
     SW Format:  32-bit unsigned linear value from dB scaled by Q22.10

DEPENDENCIES
  None.

PARAMETERS
  rmac_t2p_inflow_range_ptr - Pointer to the T2PInflowRange in RMAC SW internal
                              format
  scm_t2p_inflow_range_ptr  - Pointer to the T2PInflowRange in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_t2p_inflow_range_nn__config_xx
(
  hdrrmaccmn_t2p_inflow_range_struct_type     *rmac_t2p_inflow_range_ptr,
  hdrscmrmaccmn_t2p_inflow_range_struct_type  *scm_t2p_inflow_range_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_TRANSLATE_ASSOCIATED_FLOWS_NN

DESCRIPTION
  This function is used to translate the AssociatedFlowsNN from OTA 
  format to SW format.

DEPENDENCIES

PARAMETERS
  flow_id - MAC Flow ID NN of the attribute specified by attribID 0xXXNN
  rmac_flow_mapping_ptr - Pointer to the AssociatedFlowsNN in RMAC SW internal
                          format
  scm_flow_mapping_ptr -  Pointer to the AssociatedFlowsNN in OTA format

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_translate_associated_flows_nn
(
  uint8                                       flow_id,
  hdrrmaccmn_flow_mapping_struct_type         *rmac_flow_mapping_ptr,
  hdrscmrmaccmn_associated_flows_struct_type  *scm_flow_mapping_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRRMACCMN_CALC_T2P_INFO

DESCRIPTION
  This function calculates the T2P conversion (from T2P to flow size) 
  factors for LoLat and HiCap modes

  This function *must* be called whenever a PowerParameters attribute *or* 
  the AuxliaryPilotChannelParameters attribute changes.
  
DEPENDENCIES
  The parameter must not be NULL.

PARAMETERS
  power_params_ptr - PowerParamters attribute array. 
  aux_pilot_info   - Auxiliary Pilot Channel Gain and Minimum Payload
  t2p_info         - T2P information array to hold calculated T2P info for 
                     each payload size and conversion factor for each 
                     transmission mode.
  t2p_boost_gain   - Additional gain in Q.2 dB to added to T2P gain. 
  boost_tx_t2p_info - Table to be filled in with boost gains.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_calc_t2p_info
(
  hdrscmrmaccmn_power_params_struct_type 
         power_params_ptr[][HDRRMACCMN_NUM_TX_MODES],
    /* Source structure array holding PowerParamterNN data. */

  hdrrmaccmn_aux_pilot_struct_type *aux_pilot_info,
    /* Aux Pilot channel parameter used in computation */

  hdrrmaccmn_t2p_info_type t2p_info[],
    /* T2P Info array to hold the calculated T2P info for each payload size
       and conversion factor for each transmission mode */

  uint8  t2p_boost_gain,
     /* Additional gain in Q.2 dB to added to T2P gain */ 

  uint8  t2p_deboost_gain, 
     /* Additional gain in Q.2 dB to substracted from T2P gain */ 

  hdrrmaccmn_tx_t2p_info_type* boost_tx_t2p_info
    /* Table to be filled in with boost gains. */

);


 /* EJECT */
/*===========================================================================

FUNCTION     : HDRRMACCMN_FIND_FLOW_BINDING_CHANGE

DESCRIPTION  : 
  Each MAC flow could be bound to a set of App flows.

  Given two flow Mapping instances 'af_subset' and 'af_set', this function 
  finds if af_subset is a proper subset of af_set. i.e. if all the App
  flows present in 'af_subset' are present in 'af_set'. If not, the
  elements in af_subset that are not in af_set are copied to flow mapping
  'af_changes'.

  This function is used to find the App flows that are bound/unbound to a 
  particular MAC flow as an effect of GAUP.

  If the af_subset is Inuse instance and af_set is GAUPed instance, the 
  af_changes would have the App flows unbound from the MAC flow.

  On the other hand, if the af_subset if GAUPed instance and af_set is 
  InUse instance, the af_changes would have App flows that are newly bound to
  the MAC flow.

Note: 
  1. This function is called from DS task.
  2. The af_changes only has the App flows that are either bound/unbound due
     to recent GAUPing and not all the App flows that are bound to a MAC flow.

DEPENDENCIES : None

PARAMETERS   : 
  af_subset  - Flow mapping instance with set of App flows that is searched for
  af_set     - Flow mapping instance with a set of App flows that acts as a 
               search domain.
  af_changes - Flow mapping instance that holds the mapping changes ( a list of 
               either bound or unbound flows).

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrrmaccmn_find_flow_binding_change
(
  hdrrmaccmn_flow_mapping_struct_type const *af_subset,
  hdrrmaccmn_flow_mapping_struct_type const *af_set,
  hdrrmaccmn_flow_mapping_struct_type *af_changes
);


 /* EJECT */
/*=============================================================================
FUNCTION hdrrmaccmn_get_rand                                           EXTERNAL

DESCRIPTION
  This function returns a uniform random number in the range [-256, +255]

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Integer in range [-256,255] inclusive

SIDE EFFECTS
  None.
==============================================================================*/
int16 hdrrmaccmn_get_rand( void );


 /* EJECT */
/*=============================================================================
FUNCTION hdrrmaccmn_ts_get_time                                        EXTERNAL

DESCRIPTION
  This function takes a time stamp of the current modulator time.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
==============================================================================*/
void hdrrmaccmn_ts_get_time
(
  hdrrmaccmn_ts_point_type  sample_point
);


 /* EJECT */
/*=============================================================================
FUNCTION hdrrmaccmn_ts_calc_elapsed_time                               EXTERNAL

DESCRIPTION
  This function takes the start and end points and calculate the elapsed time in
  unit of chips.

DEPENDENCIES
  None.

PARAMETERS
  start_time - Starting point of the elapsed time.
  end_time - Ending point of the elapsed time.

RETURN VALUE
  elapsed_time - Elapsed time in unit of chips.

SIDE EFFECTS
  None.
==============================================================================*/
uint16 hdrrmaccmn_ts_calc_elapsed_time
(
  hdrrmaccmn_ts_point_type  start_time,
  hdrrmaccmn_ts_point_type  end_time
);


#ifdef FEATURE_HDR_MODEM_API_RL_INFO
/*=============================================================================
FUNCTION hdrrmaccmn_get_tput_from_rri_and_sp

DESCRIPTION
  This function returns throughput in Q24.8 kbps for the provided RRI and
  the termination subpacket number.

DEPENDENCIES
  None.

PARAMETERS
  rri - RRI / packet size index of packet
  sp  - Termination subpacket

RETURN VALUE
  Throughput value in Q24.8 kbps units

SIDE EFFECTS
  None.
==============================================================================*/
uint32 hdrrmaccmn_get_tput_from_rri_and_sp
(
  uint8 rri,
  uint8 sp
);


/*=============================================================================
FUNCTION hdrrmaccmn_get_tput_from_rri

DESCRIPTION
  This function returns throughput in Q24.8 kbps for the given RRI assuming
  nominal packet termination.

DEPENDENCIES
  None.

PARAMETERS
  rri - RRI / packet size index of packet

RETURN VALUE
  Throughput value in Q24.8 kbps units

SIDE EFFECTS
  None.
==============================================================================*/
uint32 hdrrmaccmn_get_tput_from_rri
(
  uint8 rri
);


/*=============================================================================
FUNCTION hdrrmaccmn_get_rel0_tput_from_rri

DESCRIPTION
  This function returns Rel0 RMAC throughput in Q24.8 kbps for the given 
  transmit RRI.

DEPENDENCIES
  None.

PARAMETERS
  rri - RRI / packet size index of packet

RETURN VALUE
  Throughput value in Q24.8 kbps units

SIDE EFFECTS
  None.
==============================================================================*/
uint32 hdrrmaccmn_get_rel0_tput_from_rri
(
  uint8 rri
);


/*=============================================================================
FUNCTION hdrrmaccmn_get_bits_from_rri

DESCRIPTION
  This function returns bits in scaled up units for the given RevA/B
  transmit RRI. The return value is scaled up by 307 to facilitate
  throughput calculation.

DEPENDENCIES
  None.

PARAMETERS
  rri - RRI / packet size index of packet

RETURN VALUE
  Bits, value scaled up by 307

SIDE EFFECTS
  None.
==============================================================================*/
uint32 hdrrmaccmn_get_bits_from_rri
(
  uint8 rri
);


/*=============================================================================
FUNCTION hdrrmaccmn_get_rel0_bits_from_rri

DESCRIPTION
  This function returns bits in scaled up units for the given Rel 0
  transmit RRI. The return value is scaled up by 307 to facilitate
  throughput calculation.

DEPENDENCIES
  None.

PARAMETERS
  rri - RRI / packet size index of packet

RETURN VALUE
  Bits, value scaled up by 307

SIDE EFFECTS
  None.
==============================================================================*/
uint32 hdrrmaccmn_get_rel0_bits_from_rri
(
  uint8 rri
);

#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

/*===========================================================================
FUNCTION HDRRMACCMN_VALIDATE_BOOST_GAIN

DESCRIPTION
  This function validates the boost gain feature by checking currently
  negotiated parameters to see if they are compatible with boost gain.
  If not, the boost gain value returned is 0. If so, boost gain is 
  returned unaltered.

DEPENDENCIES
  The parameter must not be NULL.

PARAMETERS
  power_params_ptr - PowerParamters attribute array. 
  t2p_boost_gain   - Additional gain in Q.2 dB to added to T2P gain.

RETURN VALUE
  t2p_boost_gain

SIDE EFFECTS
  None.
===========================================================================*/

uint8 hdrrmaccmn_validate_boost_gain
(
  hdrscmrmaccmn_power_params_struct_type 
         power_params_ptr[][HDRRMACCMN_NUM_TX_MODES],
    /* Source structure array holding PowerParamterNN data. */

  uint8  t2p_boost_gain
     /* Additional gain in Q.2 dB to added to T2P gain */ 
);

/*===========================================================================
FUNCTION HDRRMACCMN_FW_PWR_2_DB10                                 

DESCRIPTION
  This function converts FW power reading into dBm*10 format

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  dBm*10 format

SIDE EFFECTS
  None.
===========================================================================*/
int32 hdrrmaccmn_fw_pwr_2_db10
(
  int16 pwr,
  int16 k
);

#if defined(FEATURE_HDR_DEVICE_ONLY_DTX) || defined(FEATURE_HDR_FLEX_CONN)
/*===========================================================================
FUNCTION HDRRMACCMN_UPDATE_DTX_TX_LIMIT                                 

DESCRIPTION
  This function checks and update the internal flag whether DTX is
  tx limited

DEPENDENCIES
  Must be called before is_dtx_tx_limited() for the same subframe

PARAMETERS 
  funnel_mode - Whether RMAC is in funnel mode
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmaccmn_update_dtx_tx_limit
(
  boolean funnel_mode
);

/*===========================================================================
FUNCTION HDRRMACCMN_IS_DTX_TX_LIMITED                                 

DESCRIPTION
  This function checks if DTX is allowed based on TxPwr

DEPENDENCIES
  Must be called after update_dtx_tx_limited() is called for the
  same subframe.

PARAMETERS 
  None 
  
RETURN VALUE
  True  - if TxPwr is higher than MaxTx - StopThres and DTX is disallowed
  False - if TxPwr is lower than MaxTx - StartThres and DTX is allowed

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmaccmn_is_dtx_tx_limited( void );
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_FLEX_CONN */
#endif /* HDRRMACCMNI_H */

