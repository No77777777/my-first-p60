#ifndef HDRRMAC3DRV_H
#define HDRRMAC3DRV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    H D R   R T C   M  A  C   3  D R I V E R 
                    
                           H E A D E R    F I L E

GENERAL DESCRIPTION
  This module contains definitions and declarations for using lower layer
  related functions of the HDR Reverse Traffic Channel MAC protocol. 
  
  
  Copyright (c) 2005 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmac3drv.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/06/14   rmv     Handle FTCMAC supervision in RMAC to stop processing  packets
                   from Tx queues 
11/06/14   wsh     RTT:Fixed incorrectly reporting boosted T2P for RRI > 6 
06/04/14   arm     Changes to support DO to G QTA
11/25/13   rmg     Added changes to reduce idle subframes before tuneaway.
10/25/13   kss     Added enhanced permitted payload support.
10/25/13   rmg     Added T2P boost support.
10/09/13   rmg     Featurized removal of redundant definitions.
10/02/13   rmg     Added support for RL rate estimate for modem API.
10/02/13   rmg     Removed some redundant definitions to use those defined 
                   in hdrrmaccmn files.
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
07/10/13   arm     Supported traffic DTX.
05/15/13   kss     Use hdrts API to check slot time.
01/17/13   rmg     Used scheduled PA control instead of immediate for RLSI.
06/20/12   smd     Supported Device only DTX and FlexCONN.
03/19/12   ljl     Fixed Dime compiler errors.
01/26/12   ljl     Replaced tramp with DAL.
06/01/11   kss     Use RTC_ versions of REVMOD timing regs.
05/31/11   smd     Featurize inclusion of two header files for Nikel.
04/28/11   smd     Included two more header files. 
04/27/11   kss     Supported FW-controlled Tx enable.
05/18/10   rmg     Shifted DRC supervision timer management to FMAC: added 
                   APIs hdrrmac3drv_enter/exit_drc_supervision().
10/08/07   rmg     Fixed PA backoff subtraction in MaxT2P calculations.
08/23/07   rmg     Fixed lint warnings.
07/23/07   kss     Addwd hdrrmac3drv_get_current_tx_packet_info().
03/03/07   rmg     Added overhead channel gains compensation
01/03/07   rmg     Moved profiling macros from hdrrmac3drv.c to hdrrmac3drv.h
09/15/06   kss     Added hdrrmac3drv_start_open_state().
08/27/06   kss     Added hdrrmac3drv_shutdown() to support clean RTC disable.
08/22/06   kss     Added hdrrmac3drv_notify_suspend().
06/10/06   kss     Changed return value of hdrmac3drv_check_silence_interval.
12/13/05   etv     Included the header file hdrrmac3.h
12/12/05   etv     Removed hdrrmac3c.h header file inclusion.
10/28/05   kss     Added hdrrmac3drv_config_early_term() function;
                   add subframe_n to packet_status_type.
10/18/05   kss     Added PA availability enum and some general cleanup.
06/10/05   kss     Added header for hdrrmac3drv_exit().
05/20/05   kss     Added header for hdrrmac3drv_update_attributes().
04/15/05   kss     Created.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#ifdef FEATURE_HDR_REVA_L1
#include "dsm.h"
#ifdef FEATURE_HDR_DIME_MODEM
#include "msm.h"
#else
#ifdef T_MSM8960
#include "msmhwio.h"
#include "msmhwioreg_proc.h"
#endif /* T_MSM8960 */
#endif /* FEATURE_HDR_DIME_MODEM */
#include "hdrrmac3.h"
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Marker for idle packet (no active packet in subframe) */
#define HDRRMAC3DRV_IDLE_PACKET  0xFF

/* Packet ID Value used when no MARQ NAK has occured. */
#define HDRRMAC3DRV_NO_MARQ_NAK  0xFF


/* Profiling enable flag */
extern boolean hdrrmac3drv_enable_profiling;


/* Packet format type */
typedef enum
{
  HDRRMAC3DRV_FORMAT_A_PACKET = 0,
  HDRRMAC3DRV_FORMAT_B_PACKET = 1
} hdrrmac3drv_format_enum_type;

/* Sector loaded/not loaded */
typedef enum
{
  HDRRMAC3DRV_QRAB_LOADED = 1,
  HDRRMAC3DRV_QRAB_UNLOADED = -1
} hdrrmac3drv_qrab_enum_type;


/* Structure used for sector information */
typedef struct
{
  hdrrmac3drv_qrab_enum_type  qrab;
    /* Current QRAB value */

  int16                    qrab_ps;
    /* QRABps (pilot strength) */

  int16                    frab;
    /* Current FRAB value */

  int32                   pilot_strength;
    /* Pilot strength for best ASP */

} hdrrmac3drv_sector_info_type;


/* Structure used for previous packet status */
typedef struct
{
  boolean   prev_pkt_accepted;
    /* Whether the packet loaded in the last subframe was loaded. */

  uint16    prev_pkt_id;
    /* The previous packet's id */

} hdrrmac3drv_packet_info_struct_type;


/* Structure used for previous and current packet status */
typedef struct
{

  hdrrmac3drv_packet_info_struct_type packet_info;
    /* Previoud packet status */

  uint32   tx_t2p;
    /* The tx_t2p value currently being sent (linear). */

  uint16   tx_pkt_id;
    /* The id of the packet currently being sent. */

  uint16   tx_interlace;
    /* The interlace of the packet currently being sent. */

  uint16   tx_subpacket;
    /* The subpacket of the packet currently being sent. */

  uint16   marq_nak_id;
    /* Id of marq'd packet, if MARQ occured. */

  uint16   subframe_n;
    /* Subframe number */

} hdrrmac3drv_tx_pkt_status_type;


/* Structure for max T2P */
typedef struct
{
  uint32 t2p_max_headroom;
    /* Headroom limited max T2P in Q.2 dB */

  uint32 t2p_min_db;
    /* T2PMin in Q.2 dB */

  uint32 t2p_max_ps_db;
    /* TxT2PMax(pilotstrength) in Q.2 dB */

} hdrrmac3drv_max_t2p_type;


/* PA availability enum.  To indicate to the main RTCMAC algorithm 
   when PA needs to turn off at a known time.  */
typedef enum
{
  HDRRMAC3DRV_PA_NOT_AVAILABLE          = 0,
  HDRRMAC3DRV_PA_1_SUBPACKET_AVAILABLE  = 1,
  HDRRMAC3DRV_PA_2_SUBPACKETS_AVAILABLE = 2,
  HDRRMAC3DRV_PA_3_SUBPACKETS_AVAILABLE = 3,
  HDRRMAC3DRV_PA_4_SUBPACKETS_AVAILABLE = 4,
  HDRRMAC3DRV_PA_AVAILABLE              = 4
} hdrrmac3drv_pa_availability_enum_type;

/* Typedef for hdrrmac3drv_shutdown callback function. */
typedef void (*hdrrmac3drv_shutdown_cb_type)( void );



/*===========================================================================

                      FUNCTION  DECLARATIONS

===========================================================================*/


/*===========================================================================
FUNCTION HDRRMAC3DRV_SET_SECTOR_INFO

DESCRIPTION
  Returns information on the active sectors, such is RAB and pilot stength.
  
DEPENDENCIES
  None.

PARAMETERS
  sector_info - This structure is filled in with sector information.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_get_sector_info
(
  hdrrmac3drv_sector_info_type *sector_info
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_GET_PREV_PACKET_STATUS

DESCRIPTION
  Returns status information on packets that may have been loaded in the 
  previous subframe.
  
DEPENDENCIES
  None.

PARAMETERS
  packet_status - This structure is filled in with packet status on return.

RETURN VALUE
  The current subframe ID. (This will become the id of any packet loaded
  this subframe.)

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrrmac3drv_get_prev_packet_status
(
  hdrrmac3drv_tx_pkt_status_type * packet_status
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_SENT_PACKET

DESCRIPTION
  Loads a frame into the encoder buffer for possible transmission.
  
DEPENDENCIES
  None.

PARAMETERS
  data_length           - Number of bytes to load.
  pkt_ptr               - Pointer to the dsm item containing the packet data.
  conn_format           - Connection format: Format A or Format B
  pkt_size_index        - RRI of packet to be sent
  transmission_mode     - Transmission mode of packet (HiCap or LoLat)
  not_data_lmtd         - TRUE if the current subframe is not data limited
  packet_is_t2p_boosted - TRUE if T2P for the loaded packet is to be looked
                          up from boosted T2P table

RETURN VALUE
  Returns the packet id associated with this frame.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrrmac3drv_send_packet
(
  uint16                     data_length,
    /* Length of data to send, in bytes */

  dsm_item_type              *pkt_ptr,
    /* Pointer to dsm item chain with data to send */

  hdrrmac3drv_format_enum_type  format,  
    /* Format A/B */               

  uint8                         pkt_size_index,
    /* Indicates packet size to use (RRI) */

  hdrrmac3_tx_mode_enum_type transmission_mode
    /* Indicates HiCap or LoLat mode */

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
  ,boolean                      not_data_lmtd
    /* TRUE if the current subframe is not data limited */
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */

#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST
  ,boolean                      packet_is_t2p_boosted
    /* TRUE if T2P for the loaded packet is to be looked
       up from boosted T2P table */
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */
#ifdef FEATURE_HDR_RMAC_RTT_REDUCTION_T2P_BOOST
  ,boolean                      rtt_boost_enabled 
    /* if boost is applied due to RTT. */
#endif /* FEATURE_HDR_RMAC_RTT_REDUCTION_T2P_BOOST */
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_CONFIG_MODULATOR

DESCRIPTION
  Configures the HDR modulator for a Tx call by setting the frame offset and
  ...
  
DEPENDENCIES
  None.

PARAMETERS
  ati_value    - Current ATI value. Used to ...
  frame_offset - Frame offset for call.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_config_modulator
(
  uint32 ati_value 
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_START

DESCRIPTION
  Installs the indicated function to be called each subframe. 
  
DEPENDENCIES
  None.

PARAMETERS
  subframe_isr - isr to run each subframe.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_start
(
  void (*isr_ptr)(void)
    /* Function to be called when subframe interrupt occurs. */
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_START_OPEN_STATE

DESCRIPTION
  This function performs any low level init/config for open state 
  transition.  Currently this is enabling RAB.

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_start_open_state( void );



/*===========================================================================
FUNCTION HDRRMAC3DRV_GET_MAX_PERMITTED_PKT_SIZE

DESCRIPTION
  The function returns the maximum physical layer packet size (indexed) 
  that can be sent in the subframe of interest. 
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  RRI - max packet size index (RRI) permitted.

SIDE EFFECTS
  None.
===========================================================================*/
hdrrmac3_payload_size_enum_type hdrrmac3drv_get_max_permitted_pkt_size( void );



/*===========================================================================
FUNCTION HDRRMAC3DRV_CHECK_RLSI_CONFLICT

DESCRIPTION
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  

SIDE EFFECTS
  None.
==========================================================================*/
uint16 hdrrmac3drv_check_rlsi_conflict( void );



/*===========================================================================
FUNCTION HDRRMAC3DRV_GET_MAX_T2P

DESCRIPTION
  The function returns 
  - Headroom limited maximum T2P value
  - T2PMin
  - TxT2PMax (PilotStrength)
  All T2P values are returned in Q.2 dB
  
DEPENDENCIES
  None.

PARAMETERS
  max_t2p_info - Output parameter; points to location where max T2P info is
    returned.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_get_max_t2p
(
  hdrrmac3drv_max_t2p_type *max_t2p_info
);




/*===========================================================================
FUNCTION HDRRMAC3_UPDATE_THROTTLING                                 INTERNAL

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
extern void hdrrmac3drv_update_throttling( void );



/*===========================================================================

FUNCTION HDRRMAC3_SET_FRAME_OFFSET                                    EXTERNAL

DESCRIPTION
  This function sets the RMAC frame offset.

DEPENDENCIES
  The parameter must not be NULL.

PARAMETERS
  frame_offset - the Frame offset

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_set_frame_offset
(
  uint8 frame_offset
);



/*===========================================================================

FUNCTION HDRRMAC3DRV_SET_CHANNEL_GAINS                               EXTERNAL

DESCRIPTION
  This function stores the channel gain information.

DEPENDENCIES
  None.
  
PARAMETERS
  channel_gains - DRC, ACK and DSC channel gains

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_set_channel_gains
(            
  hdrrmac_channel_gains_struct_type *channel_gains
);



/*===========================================================================

FUNCTION HDRRMAC3_COMPUTE_SILENCE_TIME

DESCRIPTION
  Computes the next Reverse Silence Interval time

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  hdrrmac3.silence_begin_time is updated with the frame time for the
  next silence interval.

===========================================================================*/
extern void hdrrmac3drv_compute_silence_time( void );


/*===========================================================================

FUNCTION HDRRMAC3DRV_IS _SILENCE_FRAME

DESCRIPTION
  Checks whether the given frame is in Silence Interval

DEPENDENCIES
  None.

PARAMETERS
  frame_start   -  frame which needs to be checked for RLSI

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrrmac3drv_is_silence_frame
(
  qword frame_start   /* frame which needs to be checked for RLSI */
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_PA_OFF

DESCRIPTION
  Turns the reverse link off.

DEPENDENCIES
  None.

PARAMETERS
  immediate   - TRUE  => turn PA off immediately
                FALSE => turn PA off at slot indicated by the 'slot' parameter
  slot        - Slot offset at which to disable PA if not immediately
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_pa_off
(
  boolean  immediate,
    /* Whether to disable immediately */
  uint8    slot
    /* slot on which to disable */
#ifdef FEATURE_HDR_QTA 
  , boolean is_qta
#endif /* FEATURE_HDR_QTA */
);



/*===========================================================================
FUNCTION HDRRMAC3_PA_ON

DESCRIPTION
  Turns the reverse link on.

DEPENDENCIES
  None.

PARAMETERS
  immediate   - TRUE  => turn PA on immediately
                FALSE => turn PA on at slot indicated by the 'slot' parameter
  slot        - Slot offset at which to enable PA if not immediately

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_pa_on
(
  boolean immediate,
    /* Whether to enable as soon as possible */
  uint8 slot
    /* slot on which to enable */
);


/*===========================================================================
FUNCTION HDRRMAC3_SYNC_START_PA

DESCRIPTION
  Little helper function to schedule the PA to turn on at the frame boundary.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_sync_start_pa( void );



/*===========================================================================

FUNCTION HDRRMAC3_SYNC_SHUTDOWN_PA

DESCRIPTION
  Little helper function to enable the PA shutdown ISR on the given frame
  offset.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_sync_shutdown_pa( void );



/*===========================================================================
FUNCTION HDRRMAC3DRV_CHECK_DRC_SUPERVISION

DESCRIPTION

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac3drv_check_drc_supervision( void );

/*===========================================================================
FUNCTION HDRRMAC3DRV_CHECK_SILENCE_INTERVAL

DESCRIPTION
  Checks for the Reverse Link Silence Interval. Computes the number of
  subpackets that could be sent on the next interlace as to not collide with
  RLSI.  (The range of subpackets reported is 0 to 4.)

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  The number of subpackets that can be used on the current interlace.

SIDE EFFECTS
  None.
===========================================================================*/
extern int hdrrmac3drv_check_silence_interval( void );


/*===========================================================================
FUNCTION HDRRMAC3DRV_NOTIFY_SUSPEND                                EXTERNAL

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
extern void hdrrmac3drv_notify_suspend
( 
  boolean cancel_suspend,
    /* TRUE when scheduled suspend is to be cancelled */

  uint32  suspend_slot
    /* Slot when link is scheduled to be suspended. */
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_GET_PA_AVAILABILITY

DESCRIPTION
  Returns the expected PA availability, in subframes. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Number of subframes PA is available for.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern hdrrmac3drv_pa_availability_enum_type hdrrmac3drv_get_pa_availability
(
  int * subpackets_until_rlsi,
  int * subpackets_until_suspend
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_SHUTDOWN

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
extern void hdrrmac3drv_shutdown
(
  hdrrmac3drv_shutdown_cb_type shutdown_cb
);



/*===========================================================================
FUNCTION HDRRMAC3DRV_STOP

DESCRIPTION
  This function turns the PA off, disables the frame interrupt,
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
extern void hdrrmac3drv_stop
(
#ifdef FEATURE_HDR_QTA 
   boolean is_qta /* Is power up tx after QTA? */
#else
  void
#endif /* FEATURE_HDR_QTA */
);


/*===========================================================================
FUNCTION HDRRMAC3_RESET_RTC_RATE_COUNT                                EXTERNAL

DESCRIPTION
  This function resets the count of reverse traffic frames of each rate.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_reset_rtc_rate_count( void );


/*===========================================================================
FUNCTION HDRRMAC3_LOG_RTC_RATE_COUNT                                  EXTERNAL

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
===========================================================================*/
extern void hdrrmac3drv_log_rtc_rate_count( void );


/*===========================================================================
FUNCTION HDRRMAC3DRV_CONFIG_PARAMS

DESCRIPTION
  To be called during powerup. Makes the configuration data in hdrrmac3_cfg 
  accessible via hdrrmac3drv_config.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_config_params
( 
  hdrrmac3_config_struct_type * config
);


/*===========================================================================
FUNCTION HDRRMAC3DRV_UPDATE_ATTRIBUTES

DESCRIPTION
  To be called at activation. Updates non-gaupable attributes.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_update_attributes( void );


/*===========================================================================
FUNCTION HDRRMAC3DRV_EXIT

DESCRIPTION
  Perforns exit processing when RTCMAC is deactivated.  
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_exit( void );


/*===========================================================================
FUNCTION HDRRMAC3DRV_PA_IS_AVAILABLE                                EXTERNAL

DESCRIPTION
  This function queries the current availability of the PA.

DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = PA is available
  FALSE = PA is not currently available, due to RTC MAC is DRC supervision.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac3drv_pa_is_available ( void );


/*===========================================================================
FUNCTION HDRRMAC3DRV_INIT

DESCRIPTION
  Performs any start up initialization needed.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_init
(
  hdrrmac_common_type * common_parameters
);

/*===========================================================================
FUNCTION HDRRMACDRV_CONFIG_EARLY_TERMINATION                      

DESCRIPTION
  Configures early termination on the RL link. This is a test mode function.  

DEPENDENCIES
  None.

PARAMETERS
  config_mode: 0 - Default behavior, RL early termination on.
               1 - Forced early termination after 1 subpacket.
               2 - Forced early termination after 2 subpackets.
               3 - Forced early termination after 3 subpackets.
               4 - Forced termination after 4 subpackets.

RETURN VALUE
  TRUE (config mode accepted),
  FALSE (config mode not valid).

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean hdrrmac3drv_config_early_termination
(
  int config_mode
);

/*===========================================================================
FUNCTION HDRRMAC3DRV_GET_CURRENT_TX_PACKET_INFO

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
extern boolean hdrrmac3drv_get_current_tx_packet_info
(
  hdrrmac_current_tx_packet_info_type * pkt_info
);


/*===========================================================================
FUNCTION HDRRMAC3DRV_ENTER_DRC_SUPERVISION

DESCRIPTION
  Performs drc supervision entry processing - disables RL transmission and 
  reverse power control.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_enter_drc_supervision( void );


/*===========================================================================
FUNCTION HDRRMAC3_EXIT_DRC_SUPERVISION

DESCRIPTION
  Performs drc supervision exit processing - re-enables RL transmission and
  reverse power control.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrrmac3drv_exit_drc_supervision( void );

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
/*===========================================================================
FUNCTION HDRRMAC3DRV_RL_IS_ACTIVE                                    EXTERN

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
extern boolean hdrrmac3drv_rl_is_active ( void );
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

#ifdef FEATURE_HDR_TRAFFIC_DTX
/*===========================================================================
FUNCTION HDRRMAC3DRV_MONITOR_PA_MODE                          EXTERNAL

DESCRIPTION
  This function is a registered callback to monitor changes to PA mode.
  
DEPENDENCIES
  None.

PARAMETERS
  enabled - Indicated whether PA is on/off.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrrmac3drv_monitor_pa_mode ( boolean pa_on );
#endif /* FEATURE_HDR_TRAFFIC_DTX */

#if defined FEATURE_HDR_DEVICE_ONLY_DTX|| defined FEATURE_HDR_TRAFFIC_DTX
/*===========================================================================
FUNCTION HDRRMAC3DRV_UPDATE_DTX_MODE                          EXTERNAL

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
void hdrrmac3drv_update_dtx_mode ( void );
#endif /*  FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_TRAFFIC_DTX */

#ifdef FEATURE_HDR_FLEX_CONN
/*==========================================================================

FUNCTION  HDRRMAC3DRV_SET_FLEXCONN_MODE

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
void hdrrmac3drv_set_flexconn_mode( boolean flexconn_mode );
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_MODEM_API_RL_INFO
/*===========================================================================
FUNCTION HDRRMACDRV_GET_NOT_DATA_LMTD_TPUT                          EXTERNAL

DESCRIPTION
  Returns served throughput calculated based on filtered value of
  not-data-limited packet bits acknowledged per unit time.  

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Throughput value in Q24.8 kbps units.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmac3drv_get_not_data_lmtd_tput( void );


/*===========================================================================
FUNCTION HDRRMACDRV_GET_CURRENT_TPUT                                EXTERNAL

DESCRIPTION
  Returns served throughput calculated based on filtered value of
  bits acknowledged per unit time.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Throughput value in Q24.8 kbps units.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmac3drv_get_current_tput( void );
#endif /* FEATURE_HDR_MODEM_API_RL_INFO */


#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST
/*===========================================================================
FUNCTION HDRRMACDRV_SET_T2P_BOOST_INFO                              EXTERNAL

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
void hdrrmac3drv_set_t2p_boost_info
(
  hdrrmaccmn_tx_t2p_info_type* tx_t2p_info_ptr
);
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_T2P_BOOST */


#ifdef FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_PERM_PYLD
/*=========================================================================
FUNCTION     : HDRRMAC3DRV_CLEAR_CARRIER_HISTORY

DESCRIPTION  : Clear carrier history, typically for new call or HHO. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/
void hdrrmac3drv_clear_carrier_history( void );


/*===========================================================================
FUNCTION HDRRMACDRV_ENABLE_PERMITTED_PAYLOAD_OPTIMIZATION           EXTERNAL

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
void hdrrmac3drv_enable_permitted_payload_optimization
(
  boolean enable
);
#endif /* FEATURE_HDR_RMAC_TUNEAWAY_OPTIMIZATION_PERM_PYLD */

/*===========================================================================

FUNCTION HDRRMAC3DRV_DISABLE_FRAME_ISR                               EXTERNAL

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
void hdrrmac3drv_disable_frame_isr( void );
#endif /* FEATURE_HDR_REVA_L1 */

#endif /* HDRRMAC3DRV_H */
