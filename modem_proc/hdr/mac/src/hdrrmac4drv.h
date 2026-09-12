#ifndef HDRRMAC4DRV_H
#define HDRRMAC4DRV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    H D R   R T C   M  A  C   4  D R I V E R 
                    
                           H E A D E R    F I L E

GENERAL DESCRIPTION
  This module contains definitions and declarations for using lower layer
  related functions of the HDR Reverse Traffic Channel MAC protocol. 
  
  
  Copyright (c) 2007 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmac4drv.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/06/14   rmv     Handle FTCMAC supervision in RMAC to stop processing  packets
                   from Tx queues 
11/06/14   wsh     RTT:Fixed incorrectly reporting boosted T2P for RRI > 6 
08/12/14   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold
02/03/14   rmv     Skip RTC Ack processing if it is for a disabled carrier 
06/04/14   arm     Changes to support DO to G QTA
12/10/13   kss     Added Rev B T2P Boost support.
11/25/13   rmg     Added changes to reduce idle subframes before tuneaway.
11/26/13   rmg     Supported enabling carriers in FW without a frame delay.
10/02/13   rmg     Added support for RL rate estimate for modem API.
09/04/13   rmg     Fixed issue with RTCAck arrival on disabled carriers.
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
07/10/13   arm     Supported traffic DTX. 
05/15/13   kss     Use hdrts API to check slot time.
06/03/13   rmg     Added MCDO connected setup enhancement support.
02/17/13   arm     Supported network dtx and device only dtx co-existence. 
04/29/13   smd     Disable RevB code if FEATURE_HDR_REVB_SUPPORTED is not defined.
02/20/12   smd     Handled the case of HHO during DRC supervision for DTX/FC.
01/25/13   smd     Removed hdrrmac4drv_all_carriers_exit_drc_supervision(). 
07/27/12   rmg     Added hdrrmac4drv_config_funnel_mode.
06/20/12   smd     Supported Device only DTX and FlexCONN.
03/19/12   ljl     Fixed Dime compiler errors.
06/01/11   kss     Use RTC_ versions of REVMOD timing regs.
05/31/11   smd     Featurize inclusion of two header files for Nikel.
04/28/11   smd     Included two more header files. 
05/18/10   rmg     Shifted DRC supervision timer management to FMAC: added 
                   APIs hdrrmac4drv_enter/exit_drc_supervision().
09/22/09   rmg     Added support for RL flow control.
06/15/09   rmg     Added support for carrier drop due to transmit pilot power
                   differential.
03/11/09   kss     Add Rev B support for hdrrmac_get_current_tx_packet_info().
10/13/08   kss     Lint fixes.
07/08/08   rmg     Added hdrrmac4drv_get_carrier_availability().
05/15/08   rmg     Added num_chans_prev field in prev_pkt_status struct.
01/10/08   rmg     get_prev_tx_status() cleanup.
12/13/07   cc      Removed reference_subaset field from
                   hdrrmac4drv_sector_info_type.
10/27/07   rmg     Added hdrrmac4drv_get_outstanding_packet_ids().
10/15/07   rmg     Added carrier request support.
10/09/07   rmg     Changed get_sector_info() to support RAB log pkt.
10/03/07   rmg     Added NEG_INFINITY_T2P support.
09/26/07   rmg     Made corrections based on code review comments.
09/25/07   rmg     Added encoder_window_missed support.
09/24/07   rmg     Added hdr_inactive_reason support.
09/20/07   rmg     Fixed circular inclusions.
09/14/07   rmg     Added to source control with MSM7500 support changes.
07/18/07   rmg     Created.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#ifdef FEATURE_HDR_REVB_SUPPORTED
#include "dsm.h"
#ifdef FEATURE_HDR_DIME_MODEM
#include "msm.h"
#else
#ifdef T_MSM8960
#include "msmhwio.h"
#include "msmhwioreg_proc.h"
#endif /* T_MSM8960 */
#endif /* FEATURE_HDR_DIME_MODEM */
#include "hdrrmac.h"
#include "hdrrmaci.h"
#include "hdrrmaccmn.h"
#include "hdrrmac4.h"
#include "hdrsrchmac.h"
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define HDRRMAC4DRV_MAX_NUM_REV_PHY_CHANS  3

/*------------------ RMG: Temporary - for compilation ---------------------*/
#define HDRSRCH_SUBASET_MAX_SIZE      HDRSRCH_ASET_MAX_SIZE

/*-------------------------------------------------------------------------*/

#define HDRRMAC4DRV_INVALID_DROP_RANK 0x0

/* Marker for idle packet (no active packet in subframe) */
#define HDRRMAC4DRV_IDLE_PACKET  0xFF

/* Packet ID Value used when no MARQ NAK has occured. */
#define HDRRMAC4DRV_NO_MARQ_NAK  0xFF

/* Negative infinity T2P */
#define HDRRMAC4DRV_NEG_INFINITY_T2P  0xFFFFFFFF


/* Profiling macros and definitions */
extern boolean hdrrmac4drv_enable_profiling;


/* Packet format type */
typedef enum
{
  HDRRMAC4DRV_FORMAT_A_PACKET = 0,
  HDRRMAC4DRV_FORMAT_B_PACKET = 1
} hdrrmac4drv_format_enum_type;

/* Sector loaded/not loaded */
typedef enum
{
  HDRRMAC4DRV_QRAB_LOADED = 1,
  HDRRMAC4DRV_QRAB_UNLOADED = -1
} hdrrmac4drv_qrab_enum_type;


/* Structure used for sector information */
typedef struct
{
  hdrrmac4drv_qrab_enum_type  qrab;
    /* Current QRAB value */

  int16                       qrab_ps;
    /* QRABps (pilot strength) */

  int16                       frab;
    /* Current FRAB value */

  int32                       pilot_strength;
    /* PilotStrengthPS for best ASP */

} hdrrmac4drv_sector_info_type;


/* Structure used for channel and sector information */
typedef struct
{
  uint8 phy_chan_id;
    /* Physical channel id */

  hdrrmac4drv_sector_info_type *sector_info;
    /* Pointer to sector information */

} hdrrmac4drv_channel_sector_record_type;


/* Structure for per-carrier previous packet status */
typedef struct
{
  /* Physical channel index */
  uint8   phy_chan_idx;

  /* Packet accepted status of the packet loaded in the last subframe */
  boolean prev_pkt_accepted;

  /* Packet id of the packet loaded in the last subframe */
  uint16  prev_pkt_id;

  /* The TxT2P value currently being sent (linear) */
  uint32  tx_t2p;

} hdrrmac4drv_per_carrier_prev_pkt_status_type;


/* Structure for previous packet status */
typedef struct
{
  /* Number of channels for which previous packet status is reported */
  uint8 num_chans;

  /* Number of channels for which new data was loaded in the encoder 
     in the previous subframe */
  uint8 num_chans_prev;

  /* Per-carrier information */
  hdrrmac4drv_per_carrier_prev_pkt_status_type 
    c[HDRRMAC4DRV_MAX_NUM_REV_PHY_CHANS];

} hdrrmac4drv_prev_pkt_status_type;


/* Structure for per-carrier MARQ status */
typedef struct
{
  /* Physical channel index */
  uint8 phy_chan_idx;

  /* ID of MARQ'd packet if MARQ occured */
  uint16 marq_nak_id;

} hdrrmac4drv_per_carrier_marq_status_type;


/* Structure for MARQ status */
typedef struct
{
  /* Number of channels with MARQ reported */
  uint8 num_chans;

  /* Per-carrier information */
  hdrrmac4drv_per_carrier_marq_status_type 
    c[HDRRMAC4DRV_MAX_NUM_REV_PHY_CHANS];

} hdrrmac4drv_marq_status_type;


/* Structed to return status of currently transmitted and MARQ'd packets */  
typedef struct
{
  /* Subframe number */
  uint16 subframe_n;

  /* Status of packets loaded in previous subframe */
  hdrrmac4drv_prev_pkt_status_type prev_pkt_status;

  /* MARQ status */
  hdrrmac4drv_marq_status_type     marq_status;

} hdrrmac4drv_tx_status_type;


/* Structure for packet to be encoded */ 
typedef struct
{
  /* Physical channel index */
  uint8                              phy_chan_idx;

  /* Number of bytes to load */
  uint16                             data_length;

  /* Pointer to the dsm item containing the packet data */
  dsm_item_type                      *pkt_ptr;

  /* Connection format: Format A or Format B */
  hdrrmac4drv_format_enum_type       conn_format;

  /* RRI of packet to be sent */
  uint8                              pkt_size_index;

  /* Transmission mode of packet (HiCap or LoLat) */
  hdrrmaccmn_tx_mode_enum_type       transmission_mode;

#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST
  boolean                            packet_is_t2p_boosted;
    /* TRUE if T2P for the loaded packet is to be looked
       up from boosted T2P table */
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
  /* TRUE if the packet size is not data limited */
  boolean                            not_data_lmtd;
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

} hdrrmac4drv_pkt_enc_info_struct_type;


/* PA availability enum.  To indicate to the main RTCMAC algorithm 
   when PA needs to turn off at a known time.  */
/* RMG: HDRRMAC4DRV_PA_*_SUBPACKETS_AVAILABLE not used currently */
typedef enum
{
  HDRRMAC4DRV_PA_NOT_AVAILABLE          = 0,
  HDRRMAC4DRV_PA_1_SUBPACKET_AVAILABLE  = 1,
  HDRRMAC4DRV_PA_2_SUBPACKETS_AVAILABLE = 2,
  HDRRMAC4DRV_PA_3_SUBPACKETS_AVAILABLE = 3,
  HDRRMAC4DRV_PA_4_SUBPACKETS_AVAILABLE = 4,
  HDRRMAC4DRV_PA_AVAILABLE              = 4
} hdrrmac4drv_pa_availability_enum_type;


/* HDR mode enum. */
typedef enum
{
  HDRRMAC4DRV_HDR_MODE_NORMAL             = 0x00,
  HDRRMAC4DRV_HDR_MODE_NO_FINGER_LOCK     = 0x01,
  HDRRMAC4DRV_HDR_MODE_DRC_SUPERVISION    = 0x02,
  HDRRMAC4DRV_HDR_MODE_RLSI               = 0x04,
  HDRRMAC4DRV_HDR_MODE_RLSI_IMMINENT      = 0x08,
  HDRRMAC4DRV_HDR_MODE_TUNE_AWAY_IMMINENT = 0x10,
  HDRRMAC4DRV_HDR_MODE_0_T2P              = 0x20,
  HDRRMAC4DRV_HDR_MODE_CPU_FLOW_CTRL      = 0x40
} hdrrmac4drv_hdr_mode_enum_type;


/* Typedef for hdrrmac3drv_shutdown callback function. */
typedef void (*hdrrmac4drv_shutdown_cb_type)( void );



/*===========================================================================

                      FUNCTION  DECLARATIONS

===========================================================================*/


/*===========================================================================
FUNCTION HDRRMAC4DRV_CONFIG_PARAMS                                  EXTERNAL

DESCRIPTION
  To be called during powerup. Makes the configuration data in 
  hdrrmac4_config accessible via hdrrmac4drv_config.
  
DEPENDENCIES
  None.

PARAMETERS
  config - Pointer to RMAC4 configuration data

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_config_params
( 
  hdrrmac4_config_struct_type * config
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_ATTRIBUTES                              

DESCRIPTION
  To be called at activation. Updates attribute (gaupable and non-gaupable)
  configuration information maintained by RMAC4DRV.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_update_attributes (void);


/*===========================================================================
FUNCTION HDRRMAC4DRV_CONFIG_EARLY_TERMINATION

DESCRIPTION
  Configures early termination of reverse link packets.
  This is a test mode function.  

DEPENDENCIES
  None.

PARAMETERS
  config_mode: 0 - Default behavior, RL early termination on.
               1 - Forced early termination after 1 subpacket.
               2 - Forced early termination after 2 subpackets.
               3 - Forced early termination after 3 subpackets.
               4 - Forced early termination after 4 subpackets.

RETURN VALUE
  TRUE (config mode accepted),
  FALSE (config mode not valid).

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac4drv_config_early_termination
(
  int config_mode /* RMG: Make this uint16 if possible */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_RESET_RTC_RATE_COUNT

DESCRIPTION
  This function resets the count of reverse traffic frames of each rate.
  The RL rate loggic metrics structure [hdrrmac4drv_metrics] is cleared.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_reset_rtc_rate_count( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_LOG_RTC_RATE_COUNT

DESCRIPTION
  This function logs the current rate count.  Note that this may be more
  appropriately implemented as a request/response type message.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

NOTE
  RMG: - Log record LOG_HDR_REVB_RL_PKT_METRICS_C to be defined in hdrlogi.h
       - Reverse link multi-carrier packet metrics [rl_mc_pkt_metrics] to
         be defined in hdrlogi.h - should be exactly same as 
         hdrrmac4drv_metrics_type
       - If the log packet is too big, we might need to split it into 3 per 
         carrier RL metrics for logging
===========================================================================*/
extern void hdrrmac4drv_log_rtc_rate_count( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_SET_FRAME_OFFSET

DESCRIPTION
  This function sets the RMAC frame offset.

DEPENDENCIES
  None.

PARAMETERS
  frame_offset - the Frame offset

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_set_frame_offset
(
  uint8 frame_offset
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_INIT

DESCRIPTION
  Performs any start up initialization needed.
  
DEPENDENCIES
  None.

PARAMETERS
  Common parameters

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_init
(
  hdrrmac_common_type * common_parameters
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_START_OPEN_STATE

DESCRIPTION
  This function performs any low level init/config for open state 
  transition for the specified physical channel.  This function currently
  enables RAB.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Physical channel index
  chan_cfg_ptr - Pointer to the configuration structure of the logical 
                 channel associated with the above physical channel.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_start_open_state
(
  uint8                                phy_chan_idx,
  hdrrmac4_channel_config_struct_type  *chan_cfg_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_TX_IS_AVAILABLE

DESCRIPTION
  This function checks if packets can be sent on the specified carrier.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_id - Physical channel ID

RETURN VALUE
  TRUE  - if packets can be sent on the specified reverse channel.
  FALSE - otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac4drv_tx_is_available
(
  uint8 phy_chan_id
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_CONFIG_MODULATOR

DESCRIPTION
  Configures the HDR modulator for the specified modulator channels.
  
DEPENDENCIES
  None.

PARAMETERS
  ati_value     - Current ATI value. 
  phy_chan_mask - Physical channel mask

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_config_modulator
(
  uint32 ati_value
#ifdef FEATURE_HDR_BOLT_MODEM
  , uint8  phy_chan_mask
#endif /* FEATURE_HDR_BOLT_MODEM */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_RL_INFO

DESCRIPTION
  Updates reverse link information.  Currently it updates DRC and ACK gains
  for the specified channel
  
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_id    - Physical channel index
  channel_record - Reverse link channel record [will be used to program
                   rotator in future]
  subaset_info   - Sub-Active set information structure
  dropping_rank  - Carrier dropping rank
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_update_rl_info
(
  uint8                           phy_chan_id,
  sys_channel_type                *channel_record,
  hdrrmac_subaset_info_type       *subaset_info,
  uint8                           dropping_rank
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_DROP_RANKS

DESCRIPTION
  Updates reverse link drop rank information.  This information is combined 
  with the RTCMAC priority to obtain overall carrier drop ranks which are 
  written to the MDSP every subframe.
  
DEPENDENCIES
  None.

PARAMETERS
  drop_ranks_ptr - Pointer to MAX_NUM_PHY_CHANS instances of drop ranks 
                   listed by physical channel order.  The drop ranks 
                   corresponding to unassigned channels are set to an 
                   invalid value 0xFF

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_update_drop_ranks
(
  uint8 *drop_ranks_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_SET_DSC_GAIN_BASE

DESCRIPTION
  Updates DSC gains for all the carriers.
  
DEPENDENCIES
  None.

PARAMETERS
  dsc_gain_base  - DSC channel gain in unit of -0.5 dB

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_set_dsc_gain_base
(
  uint8 dsc_gain_base
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_START

DESCRIPTION
  Installs the indicated function to be called each subframe. 
  
DEPENDENCIES
  None.

PARAMETERS
  isr_ptr - Function to be called when subframe interrupt occurs

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_start
(
  void (*isr_ptr)(void)
    /* Function to be called when subframe interrupt occurs. */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_START_CHANNELS

DESCRIPTION
  Enables the channels specified by the channel mask.
  
DEPENDENCIES
  Uses demod carrier information (fwd_link_index) obtained form 
  update_rl_info() for enabling channels.

PARAMETERS
  phy_chan_mask - Physical channel ID mask
                  Set bit 'n' to enable channel 'n'
  new_connection_setup - TRUE if new connection setup procedure needs to be
                  followed for turning on RL carriers 

RETURN VALUE
  Number of active channels.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_start_channels
(
  uint16 phy_chan_mask,
    /* Physical channel mask */

  boolean new_connection_setup
    /* TRUE if AT is starting a new connection */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_STOP

DESCRIPTION
  Disables channels specified by the channel mask.  If all the channels are
  disabled, it turns the PA off, disables the frame interrupt,
  and disables DRC.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_stop
( 
#ifdef FEATURE_HDR_QTA 
   boolean is_qta /* Is power up tx after QTA? */
#else
  void
#endif /* FEATURE_HDR_QTA */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_STOP_CHANNELS

DESCRIPTION
  Disables the channels specified by the channel mask.
  
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_mask - Physical channel ID mask
                  Set bit 'n' to disable channel 'n'

RETURN VALUE
  Number of active channels

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_stop_channels
(
  uint16 phy_chan_mask
    /* Physical channel mask */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_EXIT

DESCRIPTION
  Performs exit processing when RTCMAC is deactivated.    
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_exit( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_SHUTDOWN

DESCRIPTION
  This function starts shutdown procedure.  When all packets in transmit 
  have been completed, the callback will be called.  No new packets will
  be sent after this call (unless RTC MAC is re-activated).

DEPENDENCIES
  None.

PARAMETERS
  Callback function to call when all interlaces have completed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_shutdown
(
  hdrrmac4drv_shutdown_cb_type shutdown_cb
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_PA_AVAILABILITY

DESCRIPTION
  Returns 
  - expected PA availability in units of subframes.
  - HDR inactive reason if HDR is to be switched to inactive state.
        
DEPENDENCIES
  None.

PARAMETERS
  hdr_inactive_reason - This is an output parameter.  It is populated with 
    one or more of the following:
    0         => HDR Normal Mode
    Bit 2 = 1 => HDR Inactive on the given channel due to RLSI
  subpackets_until_rlsi - This is an output parameter.  It returns 
    subpackets until rlsi, on one interlace
  subpkts_until_suspend - This is an output parameter.  It returns 
    subpackets until suspend, on one interlace

RETURN VALUE
  Number of subframes PA is available for.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern hdrrmac4drv_pa_availability_enum_type hdrrmac4drv_get_pa_availability
(
  hdrrmac4drv_hdr_mode_enum_type *hdr_inactive_reason,
  int                            *subpackets_until_rlsi,
  int                            *subpackets_until_suspend
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_CARRIER_AVAILABILITY

DESCRIPTION
  Populates HDR inactive reason for the specified channel.
        
DEPENDENCIES
  hdrrmac4drv_get_pa_availability() must be called before this function.

PARAMETERS
  phy_chan_id - Physical Channel id
  hdr_inactive_reason - This is an input/output parameter.  It is updated
    with one or more of the following:
    Bit 0 = 1 => HDR Inactive on the given channel due to finger out of lock
    Bit 1 = 1 => HDR Inactive on the given channel due to DRC Supervision

RETURN VALUE
  FALSE - Carrier is unavailable due to fwd link finger out-of-lock or
          DRC supervision.
  TRUE  - Otherwise

SIDE EFFECTS
  May turn PA on, the case the PA should be available but is not yet on.
  
===========================================================================*/
boolean hdrrmac4drv_get_carrier_availability
(
  uint8                           phy_chan_id,
  hdrrmac4drv_hdr_mode_enum_type *hdr_inactive_reason
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_PREV_TX_STATUS

DESCRIPTION
  Retrieves status of last packet to be loaded into the encoder.  The
  channel packet status information is arranged by channel CAPUT priority 
  order; the first being the highest priority channel.  Only the first
  num_chans entries in the list are valid, where num_chans is the number
  of active channels.

DEPENDENCIES
  None.

PARAMETERS
  enc_window_missed_ptr
                - Pointer to encoder window missed flag.  It is set if 
                  one of the following is true:
                  Interrupt for the current subframe arrived late
                  Previous subframe was not accepted because SW was late
                  Previous subframe was not accepted because FW was late
  packet_status - Holds information on packets loaded during previous 
                  subframe and are currently being transmitted.
  
RETURN VALUE
  Return Code: FALSE == error has occuring and caller should abort 
                        processing the subframe.
               TRUE  == success.

SIDE EFFECTS
  Initializes hdrrmac4drv.subframe_id, and interlace on first time through.
  
===========================================================================*/
extern boolean hdrrmac4drv_get_prev_tx_status
(
  boolean                    * enc_window_missed_ptr,
  hdrrmac4drv_tx_status_type * tx_status_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_SEND_PACKET

DESCRIPTION
  Sends a Reverse Traffic packet(s) to the encoder.

DEPENDENCIES
  MUST be called after hdrrmac3drv_get_prev_tx_status() within a given
  subframe.  The packet information for the enabled channels in pkt_enc_ptr
  must be arranged according to the channel CAPUT priorities.
    
PARAMETERS
  enc_window_missed_ptr
                    - Pointer to encoder window missed flag.  It is set if 
                      one of the following is true:
                      Packet wasn't loaded because SW was late
                      Packet wasn't loaded because DSP was not ready
  num_chans         - Number of records contained in pkt_enc_ptr
  pkt_enc_ptr       - Pointer to num_chans instances of packet information

RETURN VALUE
  The current subframe ID. (This will become the id of any packet loaded
  this subframe.)

SIDE EFFECTS
  None.
===========================================================================*/
extern uint16 hdrrmac4drv_send_packet
(
  boolean                              *enc_window_missed_ptr,
  uint8                                 num_chans,
  hdrrmac4drv_pkt_enc_info_struct_type *pkt_enc_ptr
#ifdef FEATURE_HDR_RMAC_RTT_REDUCTION_T2P_BOOST
  ,boolean                             rtt_boost_enabled 
    /* if boost is applied due to RTT. */
#endif /* FEATURE_HDR_RMAC_RTT_REDUCTION_T2P_BOOST */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_MAX_PERMITTED_PKT_SIZE

DESCRIPTION
  Returns the maximum permitted packet size for the next subpacket on all
  the enabled channels.

DEPENDENCIES
  Must be called after get_prev_tx_status.

PARAMETERS
  max_pkt_size_ptr - Pointer to MAX_NUM_PHY_CHANS instances of max_pkt_size.
                     The entries corresponding to the un-assigned channels
                     will be ignored.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_get_max_permitted_pkt_size
(
  hdrrmaccmn_payload_size_enum_type * max_pkt_size_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_NOTIFY_SUSPEND

DESCRIPTION
  Notifies RTC MAC of upcoming suspend event so that it may ramp down
  RL transmissions if necessary.

DEPENDENCIES
  None.

PARAMETERS
  cancel_suspend - TRUE when scheduled suspend is to be cancelled
  suspend_slot   - If cancel_suspend is FALSE, slot when link is scheduled
                   to be suspended.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_notify_suspend
( 
  boolean cancel_suspend,
    /* TRUE when scheduled suspend is to be cancelled */

  uint32  suspend_slot
    /* Slot when link is scheduled to be suspended. */
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_SECTOR_INFO

DESCRIPTION
  Returns sector information of the subactive set mapped to the requested
  RL channel.  The mapping is obtained in hdrrmac4drv_update_rl_info().

DEPENDENCIES
  None

PARAMETERS
  num_chans - Number of open channels; records contained in 
    channel_sector_record_ptr  
  channel_sector_record - Points to num_chans instances of channel sector
    record.
    channel_sector_record[].phy_chan_id is populated by the calling
    function
    channel_sector_record[].sector_info points to the location where sector
    info correspoinding to phy_chan_id is returned.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_get_sector_info
(
  /* Number of records contained in channel_sector_record_ptr */
  uint8 num_chans,

  /* Points to num_chans instances of channel_sector_record information */
  hdrrmac4drv_channel_sector_record_type * channel_sector_record
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_THROTTLING

DESCRIPTION
  This function checks whether Reverse Link throttling should be disabled
  and enables/disables based on this. (Note that normal operation has RL
  throttling on as per standard.)

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_update_throttling( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_MAX_T2P

DESCRIPTION
  The function returns the maximum T2P value that can be supported for the
  next subpacket for all the assigned channels.
  
DEPENDENCIES
  None.

PARAMETERS
  Pointer to MAX_NUM_PHY_CHANS instances of max_t2p.  The filled values will 
  be in Q.2 dB.  The entries corresponding to unassigned channels will be 
  ignored.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_get_max_t2p
(
  uint32 *max_t2p_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_SLP_CARRIER

DESCRIPTION
  Returns the physical channel index of the carrier designated to carry 
  SLP data.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  Physical channel index of SLP carrier.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_get_slp_carrier( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_PRIORITIZED_CARRIERS

DESCRIPTION
  Returns list of carriers sorted by their RTCMAC priorities; the first
  in the list being the highest priority carrier.  The RTCMAC priority
  of a carriers is inversely proportional to its average tx pilot powers.  
  Because priorities are only assigned to the enabled channels, only the 
  first num_chans entries in the table are valid.
  
DEPENDENCIES
  None.

PARAMETERS
  prioritized_carriers - Pointer to uint8.  It is set to the base address
                         of the table containing RTCMAC prioritized 
                         carriers.

RETURN VALUE
  num_chans - Num of enabled carriers.  The first num_chans entries in the 
              prioritized carriers table are valid.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_get_prioritized_carriers
( 
  uint8* prioritized_carriers
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_RL_ADD_DROP_INFO

DESCRIPTION
  Returns channel add and drop information.  If the entry corresponding to
  a channel in channel_dropped[] table is TRUE, the channel needs to be 
  dropped.

DEPENDENCIES
  None.

PARAMETERS
  channel_dropped - Pointer to MAX_NUM_PHY_CHANS instances of channel 
                    dropped statuses arranged in physical channle order.  
                    If the entry corresponding to a channel is TRUE, 
                    the channel needs to be dropped.

RETURN VALUE
  total_supported_carriers - Total number of estimated reverse link
    carriers (including the currently assigned ones) that can be supported
    by the current PA headroom.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_get_rl_add_drop_info( boolean *channel_dropped );


/*===========================================================================
FUNCTION HDRRMAC4DRV_FLOW0_HAS_DATA

DESCRIPTION
  Informs driver layer that there is SLP data to be transmitted in this 
  subframe.
  
DEPENDENCIES
  None.

PARAMETERS
  has_data - if TRUE, indicated that the SLP flow has data to transmit.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_flow0_has_data( boolean has_data );


/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_SUM_POT_T2P_OUTFLOWS

DESCRIPTION
  Updates SumPotentialT2POutflow information for all assigned channels.
  
DEPENDENCIES
  None.

PARAMETERS
  sum_pot_t2p_outflows_ptr - Pointer to MAX_NUM_PHY_CHANS instances of 
                             SumPotentialT2POutflows

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_update_sum_pot_t2p_outflows
(
  uint32 *sum_pot_t2p_outflows_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_TOTAL_TX_PILOT_PWR

DESCRIPTION
  The function returns sum of tx pilot power over all enabled channels.  The
  returned power is in 0.5 dBm units in 2's complement format.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Sum of tx pilot power over all enabled channels.

SIDE EFFECTS
  None.
===========================================================================*/
extern int8 hdrrmac4drv_get_total_tx_pilot_pwr( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_CAPUT_PRIORITIZED_CARRIERS

DESCRIPTION
  Returns list of carriers sorted by their CAPUT priorities; the first
  in the list being the carrier with highest CAPUT priority.
  
DEPENDENCIES
  None.

PARAMETERS
  prioritized_carriers - Pointer to uint8.  It is set to the base address
                         of the table containing CAPUT prioritized 
                         carriers.

RETURN VALUE
  num_chans - Num of enabled carriers.  The first num_chans entries in the 
              prioritized carriers table are valid.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 hdrrmac4drv_get_caput_prioritized_carriers
(
  uint8 *prioritized_carriers
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_OUTSTANDING_PACKET_IDS

DESCRIPTION
  Returns outstanding packet ids on all the interlaces for a given channel.
  All the outstanding packets on the channel are treated as MARQ'ed for 
  logging purposes.  The channel should be disabled after calling this
  function (hdrrmac4drv_stop_channels()) before 
  hdrrmac4drv_get_prev_tx_status() is called.
  
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Physical channel index of the channel to be looked up.
  pkt_id_ptr   - Output parameter.  Points to an array of size 
    HDRSCMRMACCMN_MAX_NUM_INTERLACES instances.  Outstanding packet ids
    on all the interlaces on the given channel are returned here.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_get_outstanding_packet_ids
(
  /* Physical channel index */
  uint8 phy_chan_idx,

  /* Pointer to return location where outstanding packet ids are returned. */
  uint8 *pkt_id_ptr
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_CURRENT_TX_PACKET_INFO

DESCRIPTION
  Returns the currently transmitting RRI or packet size. 
  
DEPENDENCIES
  None.

PARAMETERS
  Point to packet info structure to be filled out.

RETURN VALUE
  TRUE if valid data was filled in. 
  FALSE if there was no data (due to PA not on, or RTCMAC not 
    active, for instance).

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac4drv_get_current_tx_packet_info
(
  hdrrmac_current_tx_packet_info_type * pkt_info
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_ENTER_DRC_SUPERVISION

DESCRIPTION
  Performs drc supervision entry processing for the passed RL carrier - 
  disables RL transmission and reverse power control.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Physical channel index of carrier entering DRC supervision.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_enter_drc_supervision
(
  /* Physical channel index */
  uint8 phy_chan_idx
);

/*===========================================================================
FUNCTION HDRRMAC4DRV_IS_POWER_DIFF_CARRIER_DROP

DESCRIPTION
  Returns if a carrier drop due to pilot power differential is needed.  If
  so, physical carrier index of the carrier to be dropped is also returned.

DEPENDENCIES
  None.
  
PARAMETERS
  phy_chan_idx - pointer to location where physical channel index of 
    the carrier to be dropped is returned.

RETURN VALUE
  TRUE  - A carrier drop due to pilot power differential is needed.
  FALSE - Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac4drv_is_power_diff_carrier_drop
(
  uint8 * phy_chan_idx
);

/*===========================================================================
FUNCTION HDRRMAC4DRV_EXIT_DRC_SUPERVISION

DESCRIPTION
  Performs drc supervision exit processing for the passed RL carrier - 
  re-enables RL transmission and reverse power control.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Physical channel index of carrier exiting DRC supervision.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_exit_drc_supervision
(
  /* Physical channel index */
  uint8 phy_chan_idx
);


/*===========================================================================
FUNCTION HDRRMAC4DRV_START_FLOW_CONTROL

DESCRIPTION
  This function stops RMAC from making packets on non-SLP carriers by 
  reporting FALSE for non-SLP carriers when 
  hdrrmac4drv_get_carrier_availability() is called.  
  hdrrmac4_stop_flow_control must be called to re-enable data on non-SLP 
  carriers.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_start_flow_control( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_STOP_FLOW_CONTROL

DESCRIPTION
  This function brings RMAC out of flow control mode and 
  re-enables data on non-SLP carriers.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_stop_flow_control( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_CONFIG_FUNNEL_MODE                             EXTERNAL

DESCRIPTION
  When in FTM mode, this function configures funnel mode in FW.

DEPENDENCIES
  None.

PARAMETERS
  enable - TRUE  => Enable Funnel mode
           FALSE => Disable funnel mode
  data_active_carrier_idx 
         - Index of data active carrier to be conveyed to FW

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac4drv_config_funnel_mode
(
  boolean  enable,
  uint8    data_active_carrier_idx
);


#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
/*===========================================================================
FUNCTION HDRRMAC4DRV_RL_IS_ACTIVE                                    EXTERN

DESCRIPTION
  Returns TRUE if there is RL transmission.
    
DEPENDENCIES
  None.
  
PARAMETERS
  None.

RETURN VALUE
  TRUE/FALSE based on whether thre is RL transmission

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac4drv_rl_is_active ( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_NO_CARRIER_IN_DRC_SUPERVISION

DESCRIPTION
  Returns TRUE if all active carriers are not in DRC supervision.
    
DEPENDENCIES
  None.
  
PARAMETERS
  None.

RETURN VALUE
  TRUE/FALSE based on whether all active carriers are not in DRC supervion.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac4drv_no_carrier_in_drc_supervision ( void );
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

#ifdef FEATURE_HDR_REVB_DTX_MODE
/*===========================================================================
FUNCTION HDRRMAC4DRV_MONITOR_DTX_MODE                          EXTERNAL

DESCRIPTION
  This function is a registered callback to monitor changes to DTXMode.
  
DEPENDENCIES
  None.

PARAMETERS
  enabled - Indicated whether DTXMode is enabled/disabled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_monitor_dtx_mode ( boolean enabled );
#endif /* FEATURE_HDR_REVB_DTX_MODE */

#if defined FEATURE_HDR_REVB_DTX_MODE || defined FEATURE_HDR_TRAFFIC_DTX
/*===========================================================================
FUNCTION HDRRMAC4DRV_MONITOR_PA_MODE                          EXTERNAL

DESCRIPTION
  This function is a registered callback to monitor changes to PAMode.
  
DEPENDENCIES
  None.

PARAMETERS
  enabled - Indicated whether PAMode is ON/OFF.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_monitor_pa_mode ( boolean pa_on );
#endif /* FEATURE_HDR_REVB_DTX_MODE || FEATURE_HDR_TRAFFIC_DTX */

#if defined(FEATURE_HDR_DEVICE_ONLY_DTX)|| defined FEATURE_HDR_REVB_DTX_MODE\
 || defined FEATURE_HDR_TRAFFIC_DTX
/*===========================================================================
FUNCTION HDRRMAC4DRV_UPDATE_DTX_MODE                          EXTERNAL

DESCRIPTION
  This function programs DTXMode to FW.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_update_dtx_mode ( void );
#endif /*  FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_REVB_DTX_MODE 
|| FEATURE_HDR_TRAFFIC_DTX */

#ifdef FEATURE_HDR_FLEX_CONN
/*==========================================================================

FUNCTION  HDRRMAC4DRV_SET_FLEXCONN_MODE

DESCRIPTION
  This function sends a message to FW to enable or disable FLEXCONN mode
    
DEPENDENCIES
  None

INPUTS
  flexconn_mode:  flexconn mode, TRUE means enabling flexconn
  
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrrmac4drv_set_flexconn_mode( boolean flexconn_mode );
#endif /* FEATURE_HDR_FLEX_CONN */


/*===========================================================================
FUNCTION HDRRMAC4DRV_SET_CHAN_REMOVED_BY_TCA

DESCRIPTION
  This function conveys the physical channel id of the carrier that is
  removed by TCA.  Such carriers should not be enabled when RMAC is
  started after retune for TCA processing.
  
  This function is called when a carrier enters DELETE state. A carrier in 
  this state can go through one of the following paths:
  1. DELETE -> ADD_DELETE -> SETUP state. This happens when the carrier 
     replaced with another carrier in TCA
  2. DELETE -> INACTIVE state. This happens when the carrier is not replaced
     with any other carrier and is removed by RMAC.
  The flag set by this function, removed_by_tca, is reset when a carrier 
  enters SETUP or INACTIVE states.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Channel index of carrier removed by TCA

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_set_chan_removed_by_tca
(
  /* Physical channel index */
  uint8 phy_chan_idx
);

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_NOT_DATA_LMTD_TPUT   

DESCRIPTION
  Returns served throughput calculated based on filtered value of
  not-data-limited packet bits acknowledged per unit time.  

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Index of channel for which data limited throughput is to
                 be reported
 
RETURN VALUE
  Throughput value in Q24.8 kbps units.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmac4drv_get_not_data_lmtd_tput
(
  uint8    phy_chan_idx
);


/*===========================================================================
FUNCTION HDRRMACDRV_GET_CURRENT_TPUT                      

DESCRIPTION
  Returns served throughput calculated based on filtered value of
  bits acknowledged per unit time.

DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx - Index of channel for which data limited throughput is to
                 be reported

RETURN VALUE
  Throughput value in Q24.8 kbps units.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmac4drv_get_current_tput
(
  uint8    phy_chan_idx
);
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST
/*===========================================================================
FUNCTION HDRRMAC4DRV_SET_T2P_BOOST_INFO                              EXTERNAL

DESCRIPTION
  Initializes pointer to retrieve T2P boost info.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to T2P boost info.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_set_t2p_boost_info
(
  hdrrmaccmn_tx_t2p_info_type* tx_t2p_info_ptr
);
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */


#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_PERM_PYLD
/*=========================================================================
FUNCTION     : HDRRMAC4DRV_CLEAR_CARRIER_HISTORY

DESCRIPTION  : Clear carrier history, typically for new call or HHO. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/
void hdrrmac4drv_clear_carrier_history( void );


/*===========================================================================
FUNCTION HDRRMAC4DRV_ENABLE_PERMITTED_PAYLOAD_OPTIMIZATION           EXTERNAL

DESCRIPTION
  Enables/disables permitted payload optimization.

DEPENDENCIES
  None.

PARAMETERS
  TRUE  - Enable permitted payload optimization
  FALSE - Disable permitted payload optimization

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_enable_permitted_payload_optimization
(
  boolean enable
);

/*===========================================================================
FUNCTION HDRRMAC4DRV_GET_FUNNEL_MODE

DESCRIPTION
  Checks if if RMAC4 is in funnel mode

DEPENDENCIES
  None.

PARAMETERS
  None
 
RETURN VALUE
  Whether RMAC4 is in funnel mode

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac4drv_get_funnel_mode( void );

#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_PERM_PYLD */

/*===========================================================================
FUNCTION HDRRMAC4DRV_IS_CARRIER_IN_SETUP_STATE                       EXTERNAL

DESCRIPTION
  Returns whether the carrier state for the given physical channel index
  is HDRRMAC4DRV_CH_ST_SETUP or not.
  
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_idx  - Physical channel index.
 
RETURN VALUE
  TRUE - Carrier is in HDRRMAC4DRV_CH_ST_SETUP state
  FALSE - Carrier is not in HDRRMAC4DRV_CH_ST_SETUP state

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrrmac4drv_is_carrier_in_setup_state
( 
   uint8 phy_chan_idx
);

/*===========================================================================

FUNCTION HDRRMAC4DRC_IS _SILENCE_FRAME

DESCRIPTION
  Checks whether the given frame is in Silence Interval

DEPENDENCIES
  None.

PARAMETERS
  frame_start   - frame which needs to be checked for RLSI

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrrmac4drv_is_silence_frame
(
  qword frame_start   /* frame which needs to be checked for RLSI */
);

/*===========================================================================

FUNCTION HDRRMAC4DRV_DISABLE_FRAME_ISR                               EXTERNAL

DESCRIPTION
  This function uninstall/Disable the RMAC frame ISR.

DEPENDENCIES
  None
  
PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac4drv_disable_frame_isr( void );
#endif /* FEATURE_HDR_REVB_SUPPORTED */
#endif /* HDRRMAC4DRV_H */
