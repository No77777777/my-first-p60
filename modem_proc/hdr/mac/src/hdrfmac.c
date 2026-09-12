/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       H D R   F O R W A R D   T R A F F I C   C H A N N E L   M A C   
                            
                             P R O T O C O L       

GENERAL DESCRIPTION
  This file contains the implementation of the processing for commands,
  indications, messages and signals for the HDR Forward Traffic Channel 
  Media Access Control (MAC) protocol.
  
EXTERNALIZED FUNCTIONS
  hdrfmac_init
    Initialize the protocol.
    
  hdrfmac_activate
    Activate the protocol.

  hdrfmac_deactivate
    Deactivate the protocol.
  
  hdrfmac_is_active
    Determine if the protocol is active.

  hdrfmac_process_cmd
    Process commands received for the protocol.
    
  hdrfmac_process_sig
    Process signals received for the protocol.
    
  hdrfmac_process_msg
    Process messages received for the protocol.
    
  hdrfmac_msg_cb
    Callback function that queues received messages for the protocol.
      
 hdrfmac_put_pkt
    Process a received packet.
    
  hdrfmac_update_link_status 
    Inform the Forward Traffic Channel MAC protocol of the current Forward
    Traffic channel status received from the access network.
  
  hdrfmac_fac_gain_check_and_suspend
    Check and suspend FAC gain calculation before going to traffic suspend
    state.
 
  hdrfmac_fac_gain_check_and_resume
    Check and resume FAC gain calculation after returning from traffic suspend
    state.
 
INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before Forward Traffic 
  Channel packets can be processed.

  Copyright (c) 2000 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdrfmac.c_v   1.50   24 Feb 2003 12:59:42   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrfmac.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/20/18   vke     Changes to update the FMAC subtype to fw
02/02/16   vke     Changes to remove less significant F3s
10/16/15   vke     Fixed KW errors 
02/25/15   ljl     Fixed compiler warning. 
12/17/14   vke     Added API to handle the flow control command from DS
07/09/14   cnx     Support NV refresh without reset.
03/19/14   dsp     Moved initialization of timers out of featurization. 
03/10/14   rmv     Added API that returns whether jumpstart FAC gain is computed
03/19/14   rmv     Not to reinit FAC calc values on tuneaway till jumpstart 
                   FAC gain value is calculated
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
12/13/13   mbs     Removed restricting DRC rates to 2
11/14/13   cnx     Check FT valid until TCAssign is complete.
11/08/13   mbs     Restricted DRC rates to 2 to help Agilent sustain call
09/12/13   rmg     Added FAC gain jumpstart for faster gain settling. 
05/15/13   rmv     Added updates for Cost of Modem Power API support 
01/25/13   vke     Replaced msgr_send() by hdrmdsp_msgr_send() in 
                   hdrfmac_set_fc_pattern 
10/01/12   kss     Clear FMAC config on deactivate to ensure settings are 
                   not applied in idle.
09/05/12   smd     Fixes for FlexConn and DTX.
08/23/12   smd     Added updates for Device only DTX and FlexCONN.
08/17/12   ljl     Limited max DRC.
08/14/12   wsh     Reparse LoadInfo cache when ASET is updated
07/27/12   wsh     Fixed ARD STM Q full after back-to-back FL ACT ind.
07/16/12   vlc     Removed check for Multi Carrier for gRICE.       
06/20/12   smd     Supported Device only DTX and FlexCONN.
06/21/12   vlc     Added check for slot interrupt processing when suspending 
                   and resuming traffic state.       
03/29/12   kss     Disabled Partial Load RCL for MC gRICE.       
05/02/12   wsh     Merged NLB support (several fixes)
04/30/12   dsp     Fixed compiler warnings on Dime.
03/08/12   wsh     Merged NLB support
03/16/12   arm     Resolve TCB errors for Dime. 
03/01/12   rmg     Made changes to process set_fl_active_cb() in HDRRX task.
02/07/12   arm     Fixed thermal mitigation bug, check if HDR is active 
                   before queing any command.
01/30/12   smd     Assigned correct fmac subtype in FTM mode.
11/29/11   kss     Fixed compiler warning.
10/17/11   rmg     Removed featurization around hdrsrchmcpm calls.
09/16/11   lyl     Fixed ratio calculation overflow for FAC gain.
09/16/11   vlc/kss Added support for FAC gain calculation under feature 
                   FEATURE_HDR_GRICE.
09/16/11   rmg     Added workaround to ackOverride not taking an effect in
                   IS890_MODE_1 ack mode.
08/04/11   ssu     Reference ftm_mode using ftm_get_mode().       
08/04/11   ssu     Replace hdr_ftmi_xx with hdrhitmsg_xx using hdrhitmsg APIs.
07/22/11   arm     Added support for memory FC logging.
06/16/11   ssu     Removed inclusion of "hdrds.h" and "hdrrlp.h".
06/01/11   kss     Removed bringup change for NikeL.
05/24/11   ssu     Removed inclusion of "hdrpac.h"
04/27/11   lyl     Supported HDR FL data driven adaptive RxD. 
04/27/11   rmg     Added HDR MCPM support changes. 
03/22/11   arm     Added support for thermal pa backoff feature.
11/24/10   rmg     Fixed processing of flow control off command.
08/17/10   kss     Fix featurization error in SAR reporting.
08/06/10   kss     Supported reporting connect failure based on SAR.
08/06/10   kss     Use callbacks for early term dip switch support.       
06/24/10   rmg     Fixed compiler warnings.
06/23/10   rmg     Fixed a compilation error.
06/21/10   cnx     Fixed compiler warnings.
06/10/10   kss     Suppport drc cover override and flow control (legacy).
06/10/10   rmg     Propagated changes in DS707 defs used in flow control.
05/27/10   kss     Added API to return drc_length for given carrier.
05/20/10   kss     Set Normal SINR threshold for drcLock Backoff.       
05/18/10   rmg     Added DRC supervision timer management.
04/16/10   mbs     Use demod idx to write to FW instead of carrier index
02/02/10   kss     Remove the restriction on max DRC rate.       
01/14/09   etv     Process GAUP update for MC FMAC attributes.
12/02/09   kss     Only allow DRC 2 as our max FL rate.
10/20/09   etv     Set Max DRC for Enhanced rates when feature defined.
07/29/09   etv     Added support for connected state PER.
10/02/09   lyl     Updated for rumi bring up
07/28/09   etv     Included hdrfw_msg.h
07/15/09   etv     Moved sw module definitions to hdrmsg.h
05/22/09   etv     Added support for Q6 message interface
03/06/09   etv     Fixed bug with TAP fixed DRC Cover mode
12/23/08   etv     ByPass RCL Adjust to predictor SINR when ASP out of lock.
11/21/08   etv     Added support for flow control (graceful degradation).
07/02/08   etv     Removed invalid FMAC packet message.
06/11/08   etv     Set Max Traffic DRC to maximum Rel-B DRC.
06/13/08   etv     Fixed DRC to 2 during connection setup
05/14/08   etv     Update ACK mode macros.
03/31/08   etv     Rev-B Macro cleanup
03/13/08   etv     Removed RevB 7500 featurization.
02/11/08   etv     Fixed featurization issue in hdrfmac_set_drc_value
01/08/08   etv     Added API hdrfmac_serving_sector_locked_across_sub_asets.
12/12/07   etv     Added Multi-carrier AT command.
11/20/07   etv     Featurized SRCH API to get mac index for 7800 compilation.
11/05/07   etv     Set Demod to RevB for MC subtype.
11/02/07   etv     Used featurization to share VU betweem 7500 & 7800.
10/10/07   etv     Minor updates after code review comments.
09/20/07   mt      Pass num_active_users in serving sector to searcher.
09/15/07   etv     Fixed compiler warning. 
09/10/07   etv     Defined TAP interfaces.
09/05/07   etv     Fixed high lint warnings.
08/28/07   etv     Defined rev-B hdrfmac_set_channel_params interface.
07/28/07   etv     Forward the entire DSM pkt chain, one pkt at a time to PCP.
07/24/07   etv     Removed FEATURE_HDRRX_YIELD.
06/19/07   cc      Add HDRSCP_MC_FMAC protocol subtype for DOrB.
01/08/07   etv     Included hdrutil.h header file.
12/22/06   etv     Disable RCL if number of active users is over eight.
12/20/06   kss     Ensure DRC Boost length is set to zero for Rev 0.
12/07/06   kss     Set low SINR threshold for drcLock backoff in Rel 0. 
07/31/06   etv     Write ACK channel gain to the right firmware register.
06/30/06   etv     Supported DSP Handoff algorithm changes.
06/13/06   kss     Enable RCL A+ at FTC MAC activate.
05/05/06   etv     Code cleanup. Grouped all global vars in a static struct.
05/04/06   etv     Trigger AGC Tx attrib updates and wait for flag reset.
04/18/06   etv     Added support for DRC/DSCBoostLength & ChannelGainBoost.
12/13/05   pba     Modified SCMDB calls to use updated API's
12/05/05   vm      Added FTM support. 
11/30/05   etv     FT-Valid for Mac index above 64 shifted by right offset.
11/04/05   etv     Setting HO delays by subtracting drc and dsc length.
11/04/05   ksu/sy  Delete 5500-specific hdrfmac_rlp_reg_srvc().
10/12/05   etv     Fixed some L1 featurization bugs.
08/29/05   ksu/sy  Remove dsrlp_reg_hdlc() in favor of unit32 app_field
                   parameter to dsrlp_reg_srvc().
10/04/05   kss     For hdrfmac_set_drc_value(), check SCM's subtype version.
09/27/05   kss     Added parameter to error message in process_ind.
09/21/05   grl     Added break statement to hdrfmac_process_ind function.
09/06/05   etv     Added 0xF as a valid DRC in hdrfmac_set_drc_value function
                   to switch out of fixed DRC mode to variable mode.
09/06/05   etv     Created Library for Enhanced subtype.
08/31/05   etv     Updated hdrfmac_set_drc_value to return boolean.
08/22/05   etv     Added SCM support and AN Init GAUP support.
08/16/05   etv     Fixed bug in hdrfmac_current_subtype usage.
08/10/05   etv     Added DRC Range checking to hdrfmac_set_drc_value.
07/26/05   etv     Added support for Muxing based on packet type and filtering
                   SU/MU packets with invalid MAC trailers. 
07/22/05   etv     Added FTAP interfaces and added an interface to set MAX 
                   DRC value based on negotiated FMAC subtype.
07/13/05   etv     Added L1 Feature around set MOD and set NULL rate macros. 
06/23/05   etv     Added support for NullRateDRC38.4 attribute. 
06/22/05   etv     Added support for MUP and packet chaining. 
06/21/05   etv     Added support for DRC Translation Offset.
06/07/05   grl     Moved FTC and DRC supervision timeout logging into file.
05/26/05   etv     Added F3 message to print the current subtype. 
05/26/05   etv     Replaced hardcoded subtype with  hdrscp_get_current_subtype.
05/17/05   etv     Updated hdrpac_flow_is_enabled call in hdrfmac_put_chain.
05/16/05   etv     Modify the temporary copy in hdrfmac_update_link_status and 
                   leave function argument unchanged. 
05/16/05   etv     Invoked SET_DEMOD mode macros in fmac_activate alone rather
                   than both activate and deactivate. 
05/16/05   dna     dsc_channel_gain is unsigned
05/13/05   etv     Added Rev-A support. 
                   Moved hdrrx_put_chain functionality to hdrfmac_put_chain.
01/25/05   etv     Cleaned lint errors. 
08/02/04   kss     Set packet length on incoming packets.
04/24/04   kss     Added API for checking if flow control is currently enabled.
01/08/04   kss     Include state parameter in indication event.
09/10/03   mpa     Converted F3 messages to use new MSG2.0 SSID
07/30/03   kss     Print original FTValid field in case of FTValid failure.
05/01/03   kss     Add FTM support for DRC gating.
01/10/03   kss     Latch DRC/ACK gain values into DSP.
11/06/02    vr     Added support for CPU based flow control
10/25/02   kss     While checking FTValid, ignore MAC index > 63.
09/25/02   kss     While checking FTValid, ignore MAC index <= 4.
07/23/02   om      Fixed resetting of Ack-channel mode in dectivate() and 
                   allow multiple calls to en-/disable_flow_ctrl().
06/03/02   mpa     Mainlined FEATURE_HDR_7_4_DDR.
05/24/02   kss     Removed all calls to decoder. HDRSRCH will handle this.
05/13/02   vas     In hdrfmac_deactivate() removed call to set decoder stale
                   timeout for idle.
03/08/02   kss     Added hdrfmac_set_ack_mode() from hdrrx.
02/11/02   ss      Made changes to use new DSM interface for cb registration.
                   Also, separated DSM callbacks and flow ctrl functions.
12/09/01   vr      Added bit mask hdrfmac_drc_req_mask  for FL flow control
09/27/01   kss     Parse forward traffic valid bit field correctly.
09/26/01   kss     Set decoder stale timeout to zero while idle.
09/25/01   kss     Convert handoff delays to slots before calling search.
09/20/01   kss     Call hdrsrch_set_ho_delays() to set handoff delays.
09/15/01   kss     Enable Forward Traffic Valid supervision.
09/07/01   kss     Added drcLock interface with DSP.
09/07/01   kss     Removed drc_lock_field for DDR fix.
09/05/01   kss     Fixed sign extention problem with drc and ack offsets.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/23/01   kss     Added DRC supervision; Forward Traffic Valid monitoring
                   and session config support.
06/21/01   kss     Added initial flow control for forward link.
06/06/01   dna     new param for hmp_send_msg()
06/04/01   kss     Add check for MAC packet valid bit.
05/08/01   kss     Added hdrfmac_set_channel_params().
03/13/01   kss     Removed call to dsm_pullup_tail() (optimization).
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                     
===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#include "err.h"
#include "hdrdebug.h"
#include "rex.h"
#include "task.h"

#include "hdrmac.h"
#include "hdrfmac.h"
#include "hdrrx.h"
#include "hdrrxi.h"
#include "hdrfmaci.h"
#include "hdrrmac.h"
#include "hdrhmp.h"
#include "hdrbuf.h"
#include "hdrind.h"
#include "hdrhai.h"
#include "hdrmdspmcr.h"
#include "hdrpcp.h"
#include "hdrerrno.h"
#include "hdrtrace.h"
#include "hdrsrch.h"

#include "hdrscp.h" 
#include "hdrdsm.h"
#include "hdrts.h"
#include "hdrdec.h"

#ifdef FEATURE_HDR_IS890
#include "hdris890.h"
#include "hdrftap.h"
#endif
#include "hdrplog.h"


#ifdef FEATURE_FACTORY_TESTMODE
#include "hdrhitmsg.h"
#include "ftm.h"
#endif /* FEATURE_FACTORY_TESTMODE */

#include "hdrscmdb.h"
#include "hdrscmfmac.h"

#include "hdrfmacutil.h"

#include "hdrutil.h"
#include "hdrlog.h"
#include "hdrlmac.h"

#ifdef FEATURE_HDR_FL_FLOW_CONTROL
#include "ds707_ccm.h"
#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

#include "hdrmsg.h"
#include "hdrfw_msg.h"

#ifdef FEATURE_MDM_FUSION
#include "rfm_sv.h"
#endif /* FEATURE_MDM_FUSION */

#include "hdrsrchmcpm.h"

#include "hdrmc_v.h"
#ifdef FEATURE_HDR_REVC
#include "hdralmps.h"
#endif /* FEATURE_HDR_REVC */

/* Set to TRUE to bypass deregistering for FW messages */
boolean hdrfmac_bypass_dereg_fw_msgs = TRUE;

/* Max DRC when limited */
int hdrfmac_max_drc = 6;

/* Flag to limit max drc */
int hdrfmac_limit_max_drc = 0;

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/
LOCAL void hdrfmac_mem_low_event_cb
(
  dsm_mempool_id_enum_type pool_id,
  dsm_mem_level_enum_type level,
  dsm_mem_op_enum_type mem_op
);

LOCAL void hdrfmac_mem_high_event_cb
(
  dsm_mempool_id_enum_type pool_id,
  dsm_mem_level_enum_type level,
  dsm_mem_op_enum_type mem_op
); 

#ifdef FEATURE_HDR_FL_FLOW_CONTROL
/* Flow control step timer call back forward declaration */
void hdrfmac_fc_step_timer_cb( uint32 timer_id );

/* Flow control down step size */
#define HDRFMAC_FC_DOWN_STEP_SIZE                     2

/* Flow control up step size */
#define HDRFMAC_FC_UP_STEP_SIZE                       1

#define HDRFMAC_SAFE_FC_PATTERN_INC( pattern, step, ceil )            \
          ((pattern) =                                                \
           ((((pattern) + (step)) > (ceil))? (ceil): ((pattern) + (step))))

#define HDRFMAC_SAFE_FC_PATTERN_DEC( pattern, step, floor )           \
          ((pattern) =                                                \
           (((pattern) < ((floor) + (step)))? (floor): ((pattern) - (step))))

#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

LOCAL void hdrfmac_rtc_restart_timer_expired ( unsigned long rl_carrier_id );

#define HDRFMAC_TASK_PTR HDRRX_TASK_PTR

/* Macro to sign extend a 6-bit 2's comp field to 16 bits */
#define HDRFMAC_SIGN_EXTEND_6_TO_16( val )         \
      ((( (val) & 0x20 ) == 0 )? (val) :( (val)| (int16) 0xffc0 ))

/* Packet length (in bytes); exluding Format bits */
#define HDRFMAC_PKT_LENGTH  125

/* Macro to determine if the packet is format A. */
#define HDRFMAC_IS_FMT_A(fmac_chain_ptr) \
  ((((fmac_chain_ptr->data_ptr[125] & 0xc0) == 0x40)) ? TRUE:FALSE)

/* Value of Soft/Softer Handoff delay timers for default FMAC */
#define HDRFMAC_DEFAULT_FMAC_HO_DELAY_TIMER           0

/* Maximum number of users allowed per sector with Enhanced Rate control loop 
   enabled */
#define HDRFMAC_ENH_RCL_MAX_ALLOWED_USERS             8

#define HDRFMAC_ACH_OVERRIDE_TRANSMIT_ACK_EACH_EVEN_HS           0x8001
#define HDRFMAC_ACH_OVERRIDE_TRANSMIT_NAK_EACH_EVEN_HS           0x8005
#define HDRFMAC_ACH_OVERRIDE_DISABLE_ACK_TRANMISSION             0x8004
#define HDRFMAC_NO_ACK_OVERRIDE                                  0x0000

/* Reverse channel DRC supervision restart timer 
   T_FTCMPRestartTx = 12 control channel cycles = 5.12 seconds */
#define HDRFMAC_DRC_SUP_RESTART_TIMER_MSECS                      5120

#ifdef FEATURE_HDR_REVC
#define HDRFMAC_LOADADJ_CACHE_SIZE              3

#define HDRFMAC_LOADADJ_TIMERS_MS               5120

#define HDRFMAC_LOADADJ_TIMER_ID                0
#endif /* FEATURE_HDR_REVC */

  /* gRICE (guarded Robust Inteference Cancellation Equalizer */

#define HDRFMAC_NUM_SLOTS_FOR_HISTOGRAM               512   
  /* Number of slot elements received from MDSP, used to update the
     histogram.  Once this number is reached, the histogram is full and
     it's time to calculate the FAC gain estimate. */

#define HDRFMAC_NUM_SLOTS_GET_MDSP_DATA               4
  /* Number of slots between MDSP reads to get input data to calculate
     FAC gain estimate. */

#define HDRFMAC_NUM_HISTOGRAM_BINS_MAX                100
  /* Allocate Max total number of histogram bins for the array so the
     number of bins may be tweaked for performance evaluation. */

#define HDRFMAC_NUM_HISTOGRAM_BINS_DEFAULT            66
  /* Total number of histogram bins */

#define HDRFMAC_A0_RATIO_MIN                          61971  
  /* Minimum possible value assumed for ratio used in computing estimated FAC
     gain */

#define HDRFMAC_DELTA_HISTOGRAM_STEPSIZE              10486  
  /* Histogram step size */

#define HDRFMAC_ISKIP_NUM_SKIPPED_BINS                3      
  /* Number of bins skipped in calculation of the max value.
     This is done intentionally to account for cases
     where the serving sector is highly loaded and few idle mode
     samples are collected. */

#define HDRFMAC_K0_FP_CONST                           2688
  /* Constant K0_FP ( 4 * 7 * 96 ) used to compute estimated FAC gain */

#define HDRFMAC_BETA_CONST               \
          ( HDRFMAC_A0_RATIO_MIN - ( HDRFMAC_DELTA_HISTOGRAM_STEPSIZE / 2 ) )
  /* Constant BETA used to compute estimated FAC gain */

#define HDRFMAC_PREV_GAIN_FACTOR                      12
  /* Previous gain factor for IIR filter */

#define HDRFMAC_CURR_GAIN_FACTOR                      4
  /* Current gain factor for IIR filter */

#define HDRFMAC_FAC_GAIN_DEFAULT                      324    
  /* Default FAC gain in linear domain. 324 in Q10 corresponds to -10dB */

#define HDRMAC_FAC_GAIN_EST_MAX_ELEMENTS              12

#ifdef FEATURE_HDR_FLEX_CONN
#define HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT          100
   /* Forward link idle timer value -- for FlexConn. If we don't receive any FL packet in    
      this amount of time, we consider FL is idle 
   */
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
#define HDRFMAC_DTX_FL_INACTIVE_TIMEOUT          50
   /* Forward link idle timer value -- for DTX. If we don't receive any FL packet in    
      this amount of time, we consider FL is idle 
   */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

/* This struct holds soft preamble symbol estimates and preamble detection
   threshold from MDSP to calculate FAC (Forward Activity Channel) gain
   estimate.  After SW calculates FAC gain estimate, this value is given to
   MDSP for FAB (Forward Activity Bit) detection. */
typedef struct
{
  int32                     fab_metric[HDRMAC_FAC_GAIN_EST_MAX_ELEMENTS];  
    /* Soft preamble symbol estimate from MDSP */

  int32                     fab_cp[HDRMAC_FAC_GAIN_EST_MAX_ELEMENTS];      
    /* Preamble detection threshold from MDSP */

  uint32                    num_slots;      
    /* Number of slot elements from MDSP */

} 
hdrfmac_grice_data_struct;

/* This struct holds information to calculate FAC gain, including the histogram
   array. */
typedef struct
{
  uint32                     n_num_slots;
    /* Number of slot elements received from MDSP, used to update the
       histogram.  Once this number is reached, the histogram is full and
       it's time to calculate the FAC gain estimate. */

  uint32                     m_num_bins;
    /* Total number of histogram bins */

  uint32                     alpha0_ratio_min;
    /* Minimum possible value assumed for ratio used in computing estimated FAC
       gain */

  uint32                     i_skip;
    /* Number of bins skipped in calculation of the max value.
       This is done intentionally to account for cases
       where the serving sector is highly loaded and few idle mode
       samples are collected. */

  uint32                     beta;
    /* Used to compute estimated FAC gain */

  uint32                     prev_gain_fac;
    /* Previous gain factor for IIR filter */

  uint32                     curr_gain_fac;
    /* Current gain factor for IIR filter */

  boolean                    fac_gain_enable;
    /* Set to TRUE to enable FAC gain estimate calculation */

  boolean                    fac_gain_jumpstart;
    /* Set to TRUE to reinitialize FAC gain based on the first calculated
       gain value */

  uint32                     fac_gain_default;
    /* Default gain value, FAC gain is set to this value if FAC gain estimate
       calculation is not enabled */

  uint32                     drop_count;
    /* Counter for number of times MDSP input data is rejected due to not
       crossing A0 threshold */

  int32                      gain;           
    /* FAC_Gain to be given to MDSP */

  uint16                     hist_arr[ HDRFMAC_NUM_HISTOGRAM_BINS_MAX ];  
    /* Histogram array */

  uint16                     hist_idx;       
    /* Index of histogram array */

  uint16                     max_idx;        
    /* Index of winning histogram bin */

  uint16                     total_num_slots;
    /* Keep track of total number of slot elements received from MDSP so far */

  boolean                    grice_enabled;
    /* Indicate whether overall grice is enabled. */
}
hdrfmac_fac_gain_struct;

void hdrfmac_fac_gain_init( void );
void hdrfmac_process_mdsp_logging_event( hdrlog_mdsp_slot_logging_data_type *sl_params );
  /* FAC calculation function prototypes */


/*------------------------------------------------------------------------
                          GLOBAL VARIABLES
 -----------------------------------------------------------------------*/
/* Protocol Configuration parameters */
LOCAL hdrscmfmac_config_struct_type hdrfmac_config_params;

LOCAL struct
{
  uint16 current_subtype;
    /* Current negotiated FMAC subtype initialized to default */

  uint32 drc_req_mask;
    /* FL flow control request bit mask */

  hdrscmfmac_config_struct_type *cfg;
    /* Convenience pointer. */

  hdrfmac_ack_mode_enum_type ack_mode;
    /* Ack mode for test app */

  hdrfmac_tcam_params_type tcam_params;
    /* Forward traffic channel parameters received in Traffic channel 
       Assignment message  */

  boolean enable_partial_load_rcl;
    /* Is Partial load RCL enabled? */

  boolean first_time_after_activate;
    /* Is this the first time after activate? */

  rex_timer_type drc_sup_restart_timer[HDRRMAC_MAX_NUM_RL_CARRIERS];
    /* Reverse channel DRC supervision restart timer */

#ifdef FEATURE_HDR_FL_FLOW_CONTROL

  hdrfmac_flow_control_params_type fc_params;
    /* Flow control configuration parameters */

  ds707_ccm_flow_control_cmd_enum_type fc_cmd;
    /* Last flow control command received */

  hdrfmac_flow_control_pattern_enum_type fc_pattern;
    /* Current Flow control pattern */

  boolean fc_is_on;
    /* Flow control state variable which indicates whether flow control is 
       turned ON or OFF at any point of time. */

  rex_timer_type fc_step_timer;
    /*  Timer to apply Flow control command on a periodic basis */

  rex_crit_sect_type fc_crit_sect; 
    /* Critical Section for protecting flow control global state */

  uint8 fc_down_step_size;
    /* Flow control step size */

  uint8 fc_up_step_size;
    /* Flow control step size */

#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

  boolean early_term_override;
    /* Disables early termination if true, overriding any TAP setting */

#if defined( FEATURE_HDR_FL_DATA_ADAPTIVE_RXD ) || \
    defined( FEATURE_HDR_FLEX_CONN )  || defined ( FEATURE_HDR_DEVICE_ONLY_DTX )
  hdrfmac_fl_active_cb_list_type fl_active_cb_list;
    /* FL active call back list */
  uint16 last_fl_packet_hstr;
    /* hstr of last received FL packet */    
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD ||      
          FEATURE_HDR_FLEX_CONN || FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_FLEX_CONN
  rex_timer_type fl_activity_flexconn_timer;
    /* Forward Link Activity timer -- for FlexConn */
  uint32 fl_inactive_ms;
    /* Forward Link inactive time in ms, when suspending the timer happens */
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  rex_timer_type fl_activity_dtx_timer;
    /* Forward Link Activity timer -- for DTX */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_REVC
  boolean load_info_msg_rcvd;
    /* Indicate whether a Load Information message is received from
       current serving sector */ 

  hdrfmac_loadadj_cache_type loadadj[HDRFMAC_LOADADJ_CACHE_SIZE];

  rex_timer_type loadadj_timer;
#endif /* FEATURE_HDR_REVC */  

  boolean awaiting_tc_assign_complete;
    /* Whether HDRFMAC still waiting for TCA assignment to be completed
       by HDR SRCH */
} hdrfmac;


hdrfmac_fac_gain_struct hdrfmac_fac_gain_ctrl;
  /* Holds information to calculate FAC (Forward Activity Channel) gain
     estimate for MDSP for gRICE (guarded Robust Inteference
     Cancellation Equalizer.) */

hdrfmac_grice_data_struct hdrfmac_fac_gain_from_mdsp;
  /* Holds FAC gain input data received from MDSP to calculate FAC gain
     estimate for MDSP for gRICE. */


/*------------------------------------------------------------------------
                              LOOKUP TABLES
 -----------------------------------------------------------------------*/
/* DRC length table */
LOCAL const uint8 hdrfmac_drc_length_table[] = { 1, 2, 4, 8 };
/* DRC lock length and lock period for Default FMAC */
LOCAL const uint8 hdrfmac_drc_lock_length_table[] = { 4, 8, 16, 32 };
LOCAL const uint8 hdrfmac_drc_lock_period_table[] = { 8, 16 };

#ifdef FEATURE_HDR_FL_FLOW_CONTROL

/* DRC Null Pattern Length in blocks for each pattern */
LOCAL const uint16 
  hdrfmac_fc_drc_null_pattern_len[HDRFMAC_NUM_FLOW_CONTROL_PATTERNS] = 
  { 0, 10, 5, 10, 5, 6, 5, 4, 5, 10, 1 };

/* Nullcover bitmask bitmask for each pattern where each bit represents 
   whether NULL cover is 'on' during the block. FW scans from bit 15 and 
   proceeds to next bit every block until Bit(15-length+1) */
LOCAL const uint16 
  hdrfmac_fc_drc_null_pattern[HDRFMAC_NUM_FLOW_CONTROL_PATTERNS] = 
  { 0x0,    0x0040, 0x0800, 0x01C0, 0x1800, 
    0x1C00, 0x3800, 0x7000, 0x7800, 0x7FC0, 0x8000 };

/* Block Size in DRC Length ((8 / DRCLength) - 1)*/
LOCAL const uint16 hdrfmac_fc_blk_size[] = { 7, 3, 1, 0 };

/* Null cover percentile for each pattern */
LOCAL const uint16 
  hdrfmac_fc_pattern_nc_percentile[HDRFMAC_NUM_FLOW_CONTROL_PATTERNS] = 
  { 0, 10, 20, 30, 40, 50, 60, 75, 80, 90, 100 };

#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_EARLY_TERMINATION_ENABLE                           EXTERNAL

DESCRIPTION
  
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_early_termination_enable  ( void )
{

  hdrfmac.early_term_override = FALSE;
  hdrfmac_set_ack_mode(IS856_COMPLIANT);

  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Early termination enabled. (Normal)");

} /* hdrfmac_early_termination_enable  */


/*===========================================================================

FUNCTION HDRFMAC_EARLY_TERMINATION_DISABLE                           EXTERNAL

DESCRIPTION
  
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_early_termination_disable ( void )
{

  hdrfmac_set_ack_mode(IS856_NO_ACK);
  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Early termination disabled.");

  hdrfmac.early_term_override = TRUE;

} /* hdrfmac_early_termination_disable  */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_CHANNEL_PARAMS                                 EXTERNAL

DESCRIPTION
  This function sets the FMAC channel parameters drc_channel_gain, 
  ack_channel_gain, and drc_length.
  If the negotiated FMAC subtype is enhanced, this function also sets the DSC
  channel gain, DSC length, DSC timing registers namely dscHalfSlotCount 
  and dscHalfSlotTrigger.

TASK CONTEXT
  HDRMC

DEPENDENCIES
  The parameter must not be NULL.
  
PARAMETERS
  tca_params   - The FMAC channel parameter structure including parameters
                 received over Traffic channel Assignment message namely,
                    * DRC Length in slots 
                    * DRC Channel Gain in half dB 
                    * Ack Channel Gain in half dB 
                    * DSC Channel Gain in negative half dB (only for 
                      Enhanced FMAC)
  frame_offset - Frame Offset AT shall use in transmitting the reverse traffic
                 channel in slots.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  The forward channel parameters are written to corresponding mDSP registers.

===========================================================================*/
void hdrfmac_set_channel_params
(            
  hdrfmac_tcam_params_type *tcam_params
)
{

  uint8 c_idx = 0;
    /* Carrier Index */

  uint8 curr_demod_idx = 0;
   /* Demod Index */

  hdrfmac_per_sub_aset_tca_info_type *saset_info_ptr;
    /* Convenience pointer */

  hdrfw_msg_u fw_cmd;
    /* Firmware command message to program the TCAM parameters */

  errno_enum_type estatus;
    /* Error status */

  uint16 carrier_mask = 0;
    /* Carrier Mask */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u ) );
    /* Initialize the message data with zeroes */

#ifdef FEATURE_MEMCPY_REMOVAL
  hdrfmac.tcam_params = *tcam_params;
#else
  memcpy ((void*) &hdrfmac.tcam_params, 
          (void*) tcam_params, 
          sizeof (hdrfmac_tcam_params_type)); 
#endif /* FEATURE_MEMCPY_REMOVAL */
    /* Copy the TCAM params into FMAC's instance */

  msgr_init_hdr (
    &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FTCMAC_CHANNEL_CFG_CMD );

  fw_cmd.ftcmac_channel_cfg.frameOffset = hdrfmac.tcam_params.frame_offset;

  /* For each demod carrier, convert TxAGC params to desired units and write 
     mDSP registers */
  for (c_idx = 0; c_idx < hdrfmac.tcam_params.num_carriers; ++c_idx)
  {
    saset_info_ptr = &hdrfmac.tcam_params.sub_aset[c_idx];
      /* Initialize the convenience pointer */

    /*  Read current demod Index */
    curr_demod_idx = saset_info_ptr->demod_id;

    carrier_mask |= (1 << c_idx );

    saset_info_ptr->drc_channel_gain = 
      ( 2 * HDRFMAC_SIGN_EXTEND_6_TO_16( saset_info_ptr->drc_channel_gain ) );
    /* Sign extend 6 bits to 16 and convert units from half dB to quarter dB */

    saset_info_ptr->ack_channel_gain = 
      ( 2 * HDRFMAC_SIGN_EXTEND_6_TO_16( saset_info_ptr->ack_channel_gain ) );
      /* Sign extend 6 bits to 16 and convert units from half dB to quarter dB */

    fw_cmd.ftcmac_channel_cfg.fmacGains[curr_demod_idx].drcLength = 
       hdrfmac_drc_length_table[saset_info_ptr->drc_length];

    fw_cmd.ftcmac_channel_cfg.fmacGains[curr_demod_idx].drcToPilot = 
      saset_info_ptr->drc_channel_gain;
      /* Inform DSP of DRC channel gain in 1/4th dB */

    fw_cmd.ftcmac_channel_cfg.fmacGains[curr_demod_idx].ackToPilot = 
      saset_info_ptr->ack_channel_gain;
    /* Inform DSP of ACK channel gain in 1/4th dB */

  } /* for each demod carrier */

  fw_cmd.ftcmac_channel_cfg.carrierMask = carrier_mask;
    /* Set the carrier mask */

#ifdef FEATURE_FACTORY_TESTMODE
  if( ftm_get_mode() != FTM_MODE )
  {
#endif
    hdrfmac.current_subtype = 
      hdrscp_get_current_subtype( HDRHAI_FTC_MAC_PROTOCOL );
#ifdef FEATURE_FACTORY_TESTMODE
  }
  else 
  {
    if( hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 0 )
    {
      hdrfmac.current_subtype = HDRSCP_DEFAULT_FMAC;
    }
    else if( hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 1 )
    {
      hdrfmac.current_subtype = HDRSCP_ENH_FMAC;
    }
    else if(hdrhitmsg_get_rmac_params_ptr()->modem_fwd_rev_link_type == 2)
    {
      hdrfmac.current_subtype = HDRSCP_MC_FMAC;
    }
  }
#endif  

#ifdef FEATURE_HDR_REVA_L1
  if ((hdrfmac.current_subtype == HDRSCP_ENH_FMAC)
#ifdef FEATURE_HDR_REVB
      ||
      (hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
     )
  {
    hdrfmacutil_set_rev_a_channel_params ( &fw_cmd, &hdrfmac.tcam_params );
  }
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_FACTORY_TESTMODE
  /* If we are in FTM, don't set up DRC gating. This will be done
     externally */
  if( ftm_get_mode() != FTM_MODE )
  {
#endif /* FEATURE_FACTORY_TESTMODE */

    fw_cmd.ftcmac_channel_cfg.drcGating = hdrfmac.cfg->drc_gating;

#ifdef FEATURE_FACTORY_TESTMODE
  } 
#endif /* FEATURE_FACTORY_TESTMODE */

  estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
    /* Send the message */

  ASSERT ( estatus == E_SUCCESS );

} /* hdrfmac_set_channel_params */

/*===========================================================================
FUNCTION HDRFMAC_HANDOFF_OVERRIDE                                  EXTERNAL

DESCRIPTION
  Overrides the ASP decision by fw and replaces with specified ASP index.
 
DEPENDENCIES
  None.

PARAMETERS
  override - TRUE:  Override with given ASP
             FALSE: Return to fw control
       
  override_asp - ASP to use for override 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_handoff_override
(
  uint8 rl_carrier_id,
    /* Carrier on which to apply override */

  boolean override,
    /* Whether to override ASP  */

  uint8 override_asp
    /* asp_idx to us to override */
)
{

  hdrfw_msg_u fw_cmd;
    /* Firmware command message  */

  errno_enum_type estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {

    memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

    msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_HANDOFF_OVERRIDE_CMD );

    fw_cmd.handoff_override.handoffOverride[rl_carrier_id].carrierValid = 1;
    fw_cmd.handoff_override.handoffOverride[rl_carrier_id].handoffOverride.override = override;
    if (override)
    {
      fw_cmd.handoff_override.handoffOverride[rl_carrier_id].bestASPDict = override_asp;
    }
    
    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );

    ASSERT ( estatus == E_SUCCESS );

  } /* hdrfmac_is_active */

} /* hdrfmac_handoff_override */

/*===========================================================================
FUNCTION HDRFMAC_SET_FLOW CONTROL                                   EXTERNAL

DESCRIPTION
  Enables or disables legacy (null cover) flow control. 
 
DEPENDENCIES
  None.

PARAMETERS
  enable - TRUE:  enable legacy flow control
           FALSE: disable legacy flow control

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_set_flow_control
(
  boolean enable
    /* Enable or disable flow control */
)
{

    hdrfw_msg_u fw_cmd;
    /* Firmware command message  */

  errno_enum_type estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {

    memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_LEGACY_FLOW_CONTROL_CMD );
      /* Initialize the message header */

    fw_cmd.legacy_flow_control.flowControlOn = enable;

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );

  } /* hdrfmac_is_active */

} /* hdrfmac_set_flow_control */

/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_MEM_LOW_EVENT_CB

DESCRIPTION
  This is a callback function registered with the DSM memory pool to be 
  called when the available memory is lower than a specified level.
 
DEPENDENCIES
  None.

PARAMETERS
  pool_id - The memory pool id for which this cb has been invoked
  level   - The memory level corresponding to this callback
  mem_op  - The memory operation (NEW, in this case) related to the event
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_mem_low_event_cb  
(
  dsm_mempool_id_enum_type pool_id,
  dsm_mem_level_enum_type level,
  dsm_mem_op_enum_type mem_op
)
{
  dsm_unreg_mem_event_cb(pool_id, level, mem_op);
  dsm_reg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL, DSM_MEM_LEVEL_HDR_MANY,
                       DSM_MEM_OP_FREE, hdrfmac_mem_high_event_cb);
  
#if defined(FEATURE_HDR_FL_FLOW_CONTROL)

  ds707_ccm_mem_send_flow_control_cmd( DS707_CCM_MEM_DSM, DS707_CCM_FLOW_DOWN );

#else

  hdrfmac_enable_flow_control( HDRFMAC_DRC_REQ_DSMWM );

#endif

} /* hdrfmac_mem_low_event_cb */


/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_MEM_HIGH_EVENT_CB

DESCRIPTION
  This is a callback function registered with the DSM memory pool to be 
  called when the number of available items reaches a specified level.
  
DEPENDENCIES
  None.

PARAMETERS
  pool_id - The memory pool id for which this cb has been invoked
  level   - The memory level corresponding to this callback
  mem_op  - The memory operation (FREE, in this case) related to the event
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_mem_high_event_cb
(
  dsm_mempool_id_enum_type pool_id,
  dsm_mem_level_enum_type level,
  dsm_mem_op_enum_type mem_op
) 
{
  dsm_unreg_mem_event_cb(pool_id, level, mem_op);
  dsm_reg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL, DSM_MEM_LEVEL_HDR_FEW,
                       DSM_MEM_OP_NEW, hdrfmac_mem_low_event_cb);
#if defined(FEATURE_HDR_FL_FLOW_CONTROL)

  ds707_ccm_mem_send_flow_control_cmd( DS707_CCM_MEM_DSM, DS707_CCM_FLOW_OFF );

#else

  hdrfmac_disable_flow_control(HDRFMAC_DRC_REQ_DSMWM);

#endif



} /* hdrfmac_mem_high_event_cb */


/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_ENABLE_FLOW_CONTROL

DESCRIPTION
  This function will set a NULL cover for FL flow control. It maintains a
  bit mask indicating which entities have requested flow control. NULL cover
  will be set if at least one entity has requested flow control.
 
DEPENDENCIES
  None.

PARAMETERS
  mask - The entity requesting the flow control
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_enable_flow_control(uint32 mask)
{
  if ( hdrfmac.drc_req_mask & mask ) return;

  hdrfmac.drc_req_mask |= mask;
 
  hdrfmac_set_flow_control( TRUE );

  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Flow control: req mask %x stopping FL",
                  hdrfmac.drc_req_mask ); 
  return;

} /* hdrfmac_enable_flow_control */


/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_DISABLE_FLOW_CONTROL

DESCRIPTION
  This function will remove NULL cover for FL flow control. It maintains a
  bit mask indicating which entities have requested flow control. NULL cover
  will be removed only if no entity has requested flow control.
  
DEPENDENCIES
  None.

PARAMETERS
  mask - The entity requesting the flow control disabling
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_disable_flow_control(uint32 mask)
{
  if ( ! (hdrfmac.drc_req_mask & mask) ) return;

  hdrfmac.drc_req_mask &= ~mask;
  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "req mask %x",hdrfmac.drc_req_mask );

  /* Disable flow control only if no one needs it*/ 
  if(hdrfmac.drc_req_mask == 0)
  {
    hdrfmac_set_flow_control( FALSE );

    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Flow control: resuming FL" ); 
  }

} /* hdrfmac_disable_flow_control */

#ifdef FEATURE_HDR_FL_FLOW_CONTROL

/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_SET_FC_PATTERN                               INTERNAL

DESCRIPTION
  This function is used to configure a flow control pattern for all carriers.

  This function will run in the same task context as CCM.
    
DEPENDENCIES
  None.

PARAMETERS
  pattern - Pattern to configure

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

static void hdrfmac_set_fc_pattern
( 
  hdrfmac_flow_control_pattern_enum_type pattern
)
{

  uint8 demod_idx = 0;
    /* Loop index */

  uint8 drc_length = hdrfmac.tcam_params.sub_aset[demod_idx].drc_length;
    /* DRC Length */

  hdrfw_msg_u fw_cmd;
    /* Firmware command message to program flow control parameters */

  errno_enum_type estatus;
    /* Error status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u ) );
    /* Initialize the message data with zeroes */

  msgr_init_hdr (
    &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FLOW_CONTROL_CFG_CMD );

    /* Make sure pattern is within admissible range */
    if (pattern < HDRFMAC_NUM_FLOW_CONTROL_PATTERNS)
    {
      for (demod_idx = 0; demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx)
      {
        fw_cmd.flow_control_cfg.carrierMask |= (1 << demod_idx);

        fw_cmd.flow_control_cfg.fcCfg[demod_idx].DRCNullPatternLen = 
          hdrfmac_fc_drc_null_pattern_len[pattern];

        fw_cmd.flow_control_cfg.fcCfg[demod_idx].DRCNullPattern = 
          hdrfmac_fc_drc_null_pattern[pattern];

        fw_cmd.flow_control_cfg.fcCfg[demod_idx].BlkSizeInDRCLenM1 = 
          hdrfmac_fc_blk_size[drc_length];
      }

    estatus = hdrmdsp_msgr_send(&fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
        /* Send the message */

      HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Current flow control pattern: %d", pattern);

    }
    else
    {
      HDR_MSG_PROT_1( MSG_LEGACY_ERROR, "FC pattern %d out of range", pattern);
    }

} /* hdrfmac_set_fc_pattern */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_FIND_PATTERN_FROM_NC_PERCENTILE                   INTERNAL

DESCRIPTION

  This function will run in the same task context as CCM.
    
DEPENDENCIES
  None.

PARAMETERS
  pattern - Pattern to configure

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
static hdrfmac_flow_control_pattern_enum_type 
  hdrfmac_find_pattern_from_nc_percentile ( uint16 percentile )
{
  hdrfmac_flow_control_pattern_enum_type pattern = 
    HDRFMAC_LAST_FLOW_CONTROL_PATTERN;
    /* Flow control pattern */

  hdrfmac_flow_control_pattern_enum_type p_ind;
    /* Pattern loop index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* for each pattern */
  for ( p_ind = HDRFMAC_FIRST_FLOW_CONTROL_PATTERN; 
        p_ind < HDRFMAC_NUM_FLOW_CONTROL_PATTERNS; ++p_ind )
  {
    if (hdrfmac_fc_pattern_nc_percentile[p_ind] >= percentile)
    {
      pattern = p_ind;
      break;
    }
  }

  return pattern;

} /* hdrfmac_find_pattern_from_nc_percentile */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_CONFIGURE_FLOW_CONTROL                           EXTERNAL

DESCRIPTION
  This interface is used to configure the parameters specific to flow control
  algorithm (initial pattern and maximum reduction pattern)

  This function will run in the same task context as CCM.
    
DEPENDENCIES
  None.

PARAMETERS
  params - Pointer to struct containing flow control configuration parameters.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_configure_flow_control_cb 
(
  ds707_ccm_fmac_flow_control_params_type *params
)
{

  /* Null Check */
  if (params != NULL)
  {
    /* Convert null cover percentile to Pattern index */
    hdrfmac.fc_params.initial_pattern = 
      hdrfmac_find_pattern_from_nc_percentile(params->init_reduction_pattern);
    hdrfmac.fc_params.max_reduction_pattern = 
      hdrfmac_find_pattern_from_nc_percentile(params->max_reduction_pattern);

    hdrfmac.fc_params.period_ms = (params->period_ms + params->delay_time_ms);

    HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                    "FC config(initP: %d, maxP: %d, period_ms: %d)",
                    hdrfmac.fc_params.initial_pattern,
                    hdrfmac.fc_params.max_reduction_pattern,
                    hdrfmac.fc_params.period_ms);

  } /* Null check */

} /* hdrfmac_configure_flow_control */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_CONTROL_FLOW                                    EXTERNAL

DESCRIPTION
  This interface processes flow control command from CCM. 
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Flow control command to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_control_flow_cb (ds707_ccm_flow_control_cmd_enum_type cmd)
{
  boolean restart_step_timer = FALSE;
    /* Restart Step timer? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the AT is in traffic */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) ) 
  {
    /* Entering critical section... */
    rex_enter_crit_sect(&hdrfmac.fc_crit_sect);

    hdrfmac.fc_cmd = cmd;

    if (!hdrfmac.fc_is_on)
    {
      /* Flow control is not turned ON yet */
      if (cmd == DS707_CCM_FLOW_DOWN)
      {
        hdrfmac.fc_is_on = TRUE;

        /* Start with configured initial pattern for sudden congestion relief */
        hdrfmac.fc_pattern = hdrfmac.fc_params.initial_pattern;

        /* Restart the step timer to enable periodic flow decrease */
        restart_step_timer = TRUE;
      }
      else
      {
        /* Only DOWN command allowed when OFF */
        HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Unexpected FC cmd: %d", cmd);
      }
    }
    else
    {
      /* Flow control is ON already */
      switch(cmd)
      {
        case DS707_CCM_FLOW_DOWN:
          /* Restrict flow if there is room to restrict further */
            HDRFMAC_SAFE_FC_PATTERN_INC(
            hdrfmac.fc_pattern, 
              hdrfmac.fc_down_step_size,
            hdrfmac.fc_params.max_reduction_pattern );

          if (hdrfmac.fc_pattern < hdrfmac.fc_params.max_reduction_pattern)
          {
            /* Restart the step timer to enable periodic flow decrease */
            restart_step_timer = TRUE;
          }

          break;

        case DS707_CCM_FLOW_UP:
          /* Increase flow if there is room to increase further */
            HDRFMAC_SAFE_FC_PATTERN_DEC( 
            hdrfmac.fc_pattern, 
              hdrfmac.fc_up_step_size, 
            HDRFMAC_NO_FLOW_CONTROL );

          if (hdrfmac.fc_pattern > HDRFMAC_NO_FLOW_CONTROL)
          {
            /* Restart the step timer to enable periodic flow increase */
            restart_step_timer = TRUE;
          }

          break;

        case DS707_CCM_FLOW_OFF:
          /* FC pattern should be HDRFMAC_NO_FLOW_CONTROL here. If the 
             command is OFF, turn off flow control entirely and stop 
             the step timer */
          hdrfmac.fc_pattern = HDRFMAC_NO_FLOW_CONTROL;
          rex_clr_timer(&hdrfmac.fc_step_timer);
          hdrfmac.fc_is_on = FALSE;

          break;

        case DS707_CCM_FLOW_FREEZE:
          /* There is no use for step timer when the command is FREEZE; So, stop
             the step timer */
          rex_clr_timer(&hdrfmac.fc_step_timer);
          break;

        default:
          HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Unexpected FC cmd: %d", cmd);
      }

      HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "FC Cmd: %d", cmd);
    }

    if (cmd != DS707_CCM_FLOW_FREEZE)
    {
      /* Program mDSP with the initial pattern */
      hdrfmac_set_fc_pattern(hdrfmac.fc_pattern);
    }

    if (restart_step_timer)
    {
      /* start the step timer */
      rex_set_timer( &hdrfmac.fc_step_timer, hdrfmac.fc_params.period_ms );
    }

    /* Leave Critical section */
    rex_leave_crit_sect(&hdrfmac.fc_crit_sect);

  } /* If the AT is in traffic */

} /* hdrfmac_control_flow_cb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_FC_STEP_TIMER_CB                                    EXTERNAL

DESCRIPTION
 This function is the callback for the step timer.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_fc_step_timer_cb( uint32 timer_id )
{
  boolean restart_step_timer = FALSE;
    /* Restart Step timer? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the AT is in traffic */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) ) 
  {
    /* Entering critical section... */
    rex_enter_crit_sect(&hdrfmac.fc_crit_sect);

    if (hdrfmac.fc_cmd == DS707_CCM_FLOW_DOWN)
    {
      /* Restrict flow if there is room to restrict further, */
        HDRFMAC_SAFE_FC_PATTERN_INC(
        hdrfmac.fc_pattern, 
          hdrfmac.fc_down_step_size, 
        hdrfmac.fc_params.max_reduction_pattern );

      if (hdrfmac.fc_pattern < hdrfmac.fc_params.max_reduction_pattern)
      {
        /* Restart the step timer to enable periodic flow decrease */
        restart_step_timer = TRUE;
      }
    }
    else if (hdrfmac.fc_cmd == DS707_CCM_FLOW_UP)
    {
      /* Increase flow if there is room to increase further, */
        HDRFMAC_SAFE_FC_PATTERN_DEC(
        hdrfmac.fc_pattern, 
          hdrfmac.fc_up_step_size, 
        HDRFMAC_NO_FLOW_CONTROL );

      if (hdrfmac.fc_pattern > HDRFMAC_NO_FLOW_CONTROL)
      {
        /* Restart the step timer to enable periodic flow increase */
        restart_step_timer = TRUE;
      }
    }
    else if (hdrfmac.fc_cmd == DS707_CCM_FLOW_OFF)
    {
      /* FC pattern should be HDRFMAC_NO_FLOW_CONTROL here. If the 
         command is OFF, turn off flow control entirely and don't 
         restart the step timer */
      hdrfmac.fc_pattern = HDRFMAC_NO_FLOW_CONTROL;
      hdrfmac.fc_is_on = FALSE;
    }

    hdrfmac_set_fc_pattern(hdrfmac.fc_pattern);

    if (restart_step_timer)
    {
      /* start the step timer */
      rex_set_timer( &hdrfmac.fc_step_timer, hdrfmac.fc_params.period_ms );
    }

    /* Leave Critical section */
    rex_leave_crit_sect(&hdrfmac.fc_crit_sect);

  }

} /* hdrfmac_fc_step_timer_cb */

#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

/*===========================================================================

FUNCTION HDRFMAC_CONTROL_FLOW                              EXTERNAL

DESCRIPTION
  This interface processes flow control command given by DS.
  Runs in DS task context.
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Memory Flow control command

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_control_flow (uint32 cmd)
{
#ifdef FEATURE_HDR_HANDLE_FC_IN_HDRTX
  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {
    HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrfmac_control_flow" );
  
    cmd_buf.hdr.cmd = HDRFMAC_FLOW_CONTROL_CMD;
    cmd_buf.flow_contol.cmd = (ds707_ccm_flow_control_cmd_enum_type) cmd;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_LOW, "Flow Control Cmd: %d,"
                                   " ignored FMAC is not active", cmd);
  }
#else
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrfmac_control_flow is not supported" );
#endif /* FEATURE_HDR_HANDLE_FC_IN_HDRTX */
} /* hdrfmac_control_flow */


/*===========================================================================

FUNCTION HDRFMAC_GET_FC_PATTERN                                     EXTERNAL

DESCRIPTION
  Returns the current flow control pattern.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  The current FC pattern.

SIDE EFFECTS
  None.
===========================================================================*/

hdrfmac_flow_control_pattern_enum_type hdrfmac_get_fc_pattern( void )
{

#ifdef FEATURE_HDR_FL_FLOW_CONTROL
  HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "hdrfmac_get_fc_pattern: %d", hdrfmac.fc_pattern);
  return hdrfmac.fc_pattern;

#else
  HDR_MSG_PROT(MSG_LEGACY_HIGH, "hdrfmac_get_fc_pattern: "
                                  "FEATURE_HDR_FL_FLOW_CONTROL not defined");
  return HDRFMAC_NO_FLOW_CONTROL;
#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

}

#ifdef FEATURE_HDR_THERMAL_BACKOFF
/*===========================================================================

FUNCTION HDRFMAC_CONTROL_THERMAL_DIV_CMD                             EXTERNAL

DESCRIPTION
  This interface processes thermal diversity command from CCM in HDRRX
  task context.
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Thermal flow control command to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_control_thermal_div_cmd (ds707_ccm_flow_control_cmd_enum_type cmd)
{

  boolean div_enabled = FALSE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the AT is in traffic */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) ) 
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Div Ctrl Cmd: %d", cmd);

    switch(cmd)
    {
      case DS707_CCM_FLOW_DOWN_MORE:

        div_enabled = FALSE;
        hdrsrch_thermal_diversity_pref_ctrl(div_enabled);
        break;

      case DS707_CCM_FLOW_OFF:

        div_enabled = TRUE;
        hdrsrch_thermal_diversity_pref_ctrl(div_enabled);
        break;

      case DS707_CCM_FLOW_SHUT_DOWN:

        div_enabled = FALSE;
        hdrsrch_thermal_diversity_pref_ctrl(div_enabled);
        break;

      default:
          HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Unexpected Thermal Div Ctrl cmd: %d", cmd);
    }

    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Div Ctrl, div_enabled: %d", div_enabled);
     
  } /* If the AT is in traffic */
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Thermal Div Ctrl Cmd: %d,"
                                    " ignored FMAC is not active", cmd);
  }

} /* hdrfmac_control_thermal_div_cmd */

/*===========================================================================

FUNCTION HDRFMAC_CONTROL_THERMAL_DIV_CB                              EXTERNAL

DESCRIPTION
  This interface processes thermal flow control command from CCM.
  Runs in CCM task context.
  
DEPENDENCIES
  None.

PARAMETERS
  cmd - Thermal flow control command to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrfmac_control_thermal_div_cb (ds707_ccm_flow_control_cmd_enum_type cmd)
{

  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {
    HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrfmac_control_thermal_diversity" );
  
    cmd_buf.hdr.cmd = HDRFMAC_THERMAL_DIV_CMD;
    cmd_buf.thermal_div.thermal_div_cmd = cmd;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_LOW, "Thermal Div Ctrl Cmd: %d,"
                                   " ignored FMAC is not active", cmd);
  }
  
} /* hdrfmac_control_thermal_div_cmd */

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_INIT_TIMERS                                        INTERNAL

DESCRIPTION
  This function initializes DRC supervision timers.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_init_timers(void)
{

  uint8 channel_id;
    /* Reverse channel ID */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Define DRC supervision restart timer for each reverse channel */
  for (channel_id = 0; channel_id < HDRRMAC_MAX_NUM_RL_CARRIERS; channel_id++)
{
    rex_def_timer_ex( &hdrfmac.drc_sup_restart_timer[channel_id],
                      hdrfmac_rtc_restart_timer_expired,
                      (unsigned long)channel_id ); /* cb_param */
}

  return;

} /* hdrfmac_init_timers */

#ifdef FEATURE_HDR_REVC
/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_LOAD_INFO_TIMER_CB                                    EXTERNAL

DESCRIPTION
 This function is the callback for the LoadInfo timer.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_loadadj_timer_cb
( 
  uint32 timer_id 
)
{
  hdrrx_queue_timer(HDRHAI_FTC_MAC_PROTOCOL, timer_id);
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_CHECK_AND_SET_NLB_ENBLED

DESCRIPTION
  This function checks all conditions and enable/disable NLB if needed
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
boolean hdrfmac_check_and_set_nlb_enbled( void )
{
  static boolean prev_nlb_enabled = FALSE;
  boolean loadinfo, fmac, nlb_enabled;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  loadinfo = hdrfmac_is_loadinfo_supported();
  fmac = hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL);

  /* If LoadInfo supported and FMAC is active, should enable 3 CC decoding */
  nlb_enabled = loadinfo && fmac;

  HDR_MSG_PROT_4(MSG_LEGACY_MED, "NLB:%d=>%d LoadInfo:%d QoS:%d FMAC:%d",
                 prev_nlb_enabled, nlb_enabled, loadinfo, fmac);

  if (nlb_enabled != prev_nlb_enabled)
  {
    hdrsrch_enable_nlb(nlb_enabled);
    prev_nlb_enabled = nlb_enabled;
  }

  return nlb_enabled;
}
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_FLEX_CONN
/*===========================================================================

FUNCTION HDRFMAC_FL_ACTIVITY_FLEXCONN_TIMER_CB                       INTERNAL

DESCRIPTION
 This function is the callback for the FL activity timer for FLEXCONN
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
static void hdrfmac_fl_activity_flexconn_timer_cb( uint32 timer_id )
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "FLEXCONN: FL timer expires ");
  if ( hdrfmac.fl_active_cb_list.flexconn_fl_active_cb != NULL )
  { 
    /* FL has been inactive for the timeout period. Need to notify the client */
    hdrfmac.fl_active_cb_list.flexconn_fl_active_cb( FALSE );
  }
  hdrfmac.fl_active_cb_list.flexconn_fl_inactive_is_notified = TRUE;
} /* hdrfmac_fl_activity_flexconn_timer_cb */
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
/*===========================================================================

FUNCTION HDRFMAC_FL_ACTIVITY_DTX_TIMER_CB                       INTERNAL

DESCRIPTION
 This function is the callback for the FL activity timer for FLEXCONN
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
static void hdrfmac_fl_activity_dtx_timer_cb( uint32 timer_id )
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "DTX: FL timer expires ");
  if (  hdrfmac.fl_active_cb_list.dtx_fl_active_cb != NULL )
  {
    /* FL has been inactive for the timeout period. Need to notify the client */
    hdrfmac.fl_active_cb_list.dtx_fl_active_cb( FALSE );
  }
  hdrfmac.fl_active_cb_list.dtx_fl_inactive_is_notified = TRUE;
} /* hdrfmac_fl_activity_dtx_timer_cb */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#if defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||defined(FEATURE_HDR_FLEX_CONN)|| \
    defined(FEATURE_HDR_DEVICE_ONLY_DTX)
/*===========================================================================

FUNCTION HDRFMAC_FL_ACTIVITY_INIT                                  INTERNAL

DESCRIPTION
 This function is to initialize variables related to FL activitity CBs.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
static void hdrfmac_fl_acitivity_init( void )
{

#ifdef FEATURE_HDR_FLEX_CONN
  if ( hdrmc_is_first_init() )
  {
    /* Initialize timer for FL activity */
    rex_def_timer_ex(&hdrfmac.fl_activity_flexconn_timer,
                     hdrfmac_fl_activity_flexconn_timer_cb,
                     0);
  }
  hdrfmac.fl_active_cb_list.flexconn_fl_active_cb = NULL;
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  if ( hdrmc_is_first_init() )
  {
    rex_def_timer_ex(&hdrfmac.fl_activity_dtx_timer,
                     hdrfmac_fl_activity_dtx_timer_cb,
                     0);
  }
  hdrfmac.fl_active_cb_list.dtx_fl_active_cb = NULL;
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  hdrfmac.fl_active_cb_list.ard_fl_active_cb = NULL;
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

}/* hdrfmac_fl_acitivity_init */
#endif /* defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||
          defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_POWERUP_INIT

DESCRIPTION
  This function initializes the protocol prior to use.  It should be called
  once upon system startup. 
  
DEPENDENCIES
  None.
  
PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_powerup_init ( void )
{
  
  hdrscmfmac_verno_type hdrfmac_ver_no;
    /* uint8 Array to hold the version numbers of FTC MAC config attribs */

#ifdef FEATURE_HDR_FL_FLOW_CONTROL
  ds707_ccm_hdlr_func_tbl_type hdlr_tbl;
    /* Handler functions for CCM */
#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

#ifdef FEATURE_HDR_THERMAL_BACKOFF
  ds707_ccm_hdlr_func_comm_tbl_type hdlr_tbl_thermal_bo;
    /* Handler functions for CCM */
#endif /* FEATURE_HDR_THERMAL_BACKOFF */

  uint64 mac_debug_mask;

#ifdef FEATURE_HDR_REVC
  uint8 i;
#endif /* FEATURE_HDR_REVC */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Global variable initialization */
  hdrfmac.current_subtype = HDRSCP_DEFAULT_FMAC;
  hdrfmac.cfg = &hdrfmac_config_params;
  hdrfmac.ack_mode = IS856_COMPLIANT;

  /* Get the current attrib value and the version number */
  hdrscmdb_get_inuse_config_and_verno( 
    HDRHAI_FTC_MAC_PROTOCOL,
    hdrfmac.cfg,
    hdrfmac_ver_no,
    HDRSCMFMAC_MAX_NUM_CONFIG_ATTRIBS );

#ifdef FEATURE_HDR_REVA_L1 

  /* FMAC util powerup init */
  hdrfmacutil_powerup_init(hdrfmac.cfg);

  /* Recorded the version numbers that we need */
  hdrfmacutil_record_version_nums(hdrfmac_ver_no);

#endif /* FEATURE_HDR_REVA_L1 */

  HDRTRACE_INIT_DEFAULT(HDRHAI_FTC_MAC_PROTOCOL);

  if ( hdrmc_is_first_init() )
  {

    /* Initialize DRC supervision timers */
    hdrfmac_init_timers();
  }

#ifdef FEATURE_HDR_FL_FLOW_CONTROL

  /* Initialize working variables indicating flow control state to 
     default values */
  hdrfmac.fc_is_on = FALSE;
  hdrfmac.fc_cmd =  DS707_CCM_FLOW_OFF;
  hdrfmac.fc_pattern = HDRFMAC_NO_FLOW_CONTROL;
  hdrfmac.fc_down_step_size = HDRFMAC_FC_DOWN_STEP_SIZE;
  hdrfmac.fc_up_step_size = HDRFMAC_FC_UP_STEP_SIZE;

  /* Initialize to default. Will be configured by CCM later */
  hdrfmac.fc_params.initial_pattern = HDRFMAC_FLOW_CONTROL_EIGHT_TENTH;
  hdrfmac.fc_params.max_reduction_pattern = HDRFMAC_FLOW_CONTROL_EIGHT_TENTH;

  if ( hdrmc_is_first_init() )
  {
    /* Initialize Response timer identified by 'timer_id' */
    rex_def_timer_ex(&hdrfmac.fc_step_timer,
                     hdrfmac_fc_step_timer_cb,
                     0);
    /* Initialize the flow control critical section variable */
    rex_init_crit_sect(&hdrfmac.fc_crit_sect);
  }

  hdlr_tbl.ds707_ccm_config_fmac_func = hdrfmac_configure_flow_control_cb;
  hdlr_tbl.ds707_ccm_contrl_fmac_func = hdrfmac_control_flow_cb;

  ds707_ccm_register_handler(DS707_CCM_FTC_MAC_FC, &hdlr_tbl);
#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

#if defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||defined(FEATURE_HDR_FLEX_CONN) \
    ||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
  /* Initialize variables for FL activity callbacks */
  hdrfmac_fl_acitivity_init();
#endif /* defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||
          defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

#ifdef FEATURE_HDR_REVC
  if ( hdrmc_is_first_init() )
  {
    rex_def_timer_ex(&hdrfmac.loadadj_timer,
                     hdrfmac_loadadj_timer_cb,
                     HDRFMAC_LOADADJ_TIMER_ID);
  }
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_THERMAL_BACKOFF
  
  hdlr_tbl_thermal_bo.ds707_ccm_comm_config_func = NULL;
  hdlr_tbl_thermal_bo.ds707_ccm_comm_contrl_func = hdrfmac_control_thermal_div_cb;
  ds707_ccm_common_register_handler(DS707_CCM_THERMAL_DIVERSITY_FC, &hdlr_tbl_thermal_bo);
  
#endif /* FEATURE_HDR_THERMAL_BACKOFF */
    
  hdrlog_set_dipsw_cb(HDRLOG_DIPSW_EARLY_TERM_OFF,
    hdrfmac_early_termination_disable,
    hdrfmac_early_termination_enable);


  if (hdrsrch_is_grice_enabled() == TRUE )
  {
    hdrfmac_fac_gain_ctrl.grice_enabled = TRUE;
      
    /* Init and get enable status and initial value from NV */
    hdrfmac_fac_gain_ctrl.fac_gain_enable    = TRUE;
    hdrfmac_fac_gain_ctrl.fac_gain_jumpstart = TRUE;
    hdrfmac_fac_gain_ctrl.fac_gain_default   = HDRFMAC_FAC_GAIN_DEFAULT;
  
    if ( hdrlmac_get_user_mac_debug_mask(&mac_debug_mask)==TRUE )
    {
      /* Check calc disable bit */
      if ((mac_debug_mask & HDRLMAC_USER_MAC_DEBUG_DISABLE_FAC_GAIN_CALC_V) == 
          HDRLMAC_USER_MAC_DEBUG_DISABLE_FAC_GAIN_CALC_V)
      {
        hdrfmac_fac_gain_ctrl.fac_gain_enable  = FALSE;
      }
  
      /* Check init bit */
      if ((mac_debug_mask & HDRLMAC_USER_MAC_DEBUG_ENABLE_FAC_GAIN_INIT_V) == 
          HDRLMAC_USER_MAC_DEBUG_ENABLE_FAC_GAIN_INIT_V)
      {
        hdrfmac_fac_gain_ctrl.fac_gain_default = (uint32)(mac_debug_mask & 
          HDRLMAC_USER_MAC_DEBUG_FAC_GAIN_INITIAL_VALUE_M) >> 
          HDRLMAC_USER_MAC_DEBUG_FAC_GAIN_INITIAL_VALUE_S;
      }
    }
  
    hdrfmac_fac_gain_ctrl.n_num_slots      = HDRFMAC_NUM_SLOTS_FOR_HISTOGRAM;
    hdrfmac_fac_gain_ctrl.m_num_bins       = HDRFMAC_NUM_HISTOGRAM_BINS_DEFAULT;
    hdrfmac_fac_gain_ctrl.alpha0_ratio_min = HDRFMAC_A0_RATIO_MIN;
    hdrfmac_fac_gain_ctrl.i_skip           = HDRFMAC_ISKIP_NUM_SKIPPED_BINS;
    hdrfmac_fac_gain_ctrl.beta             = HDRFMAC_BETA_CONST;
    hdrfmac_fac_gain_ctrl.prev_gain_fac    = HDRFMAC_PREV_GAIN_FACTOR;
    hdrfmac_fac_gain_ctrl.curr_gain_fac    = HDRFMAC_CURR_GAIN_FACTOR;
      /* Initialize variables used in FAC gain estimate calc to their default
       values */
    
    hdrfmac_fac_gain_ctrl.gain             = hdrfmac_fac_gain_ctrl.fac_gain_default;
      /* At power up, initialize FAC gain to the default value recommended in the
       FW FAC Gain Estimation System document.  Since FAC gain estimation is
       filtered, the previous FAC gain is fed back to the filter to
       calculate the current FAC gain.  To start, the FAC gain must contain 
       a meaningful value.  */
    
    hdrfmac_fac_gain_init();
      /* Initialize variables used in FAC gain estimation.  In addition to power
       up, this function is called every time we start a new FAC gain
       estimation. */
    
  }
  else
  {
    hdrfmac_fac_gain_ctrl.grice_enabled = FALSE;
    hdrfmac_fac_gain_ctrl.fac_gain_enable  = FALSE;
  }

#ifdef FEATURE_HDR_REVC
  for (i=0; i<HDRFMAC_LOADADJ_CACHE_SIZE; i++)
  {
    hdrfmac.loadadj[i].state = HDRFMAC_LOADADJ_CACHE_INVALID;
  }
#endif /* FEATURE_HDR_REVC */
} /* hdrfmac_powerup_init */


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_ENABLE_ACK_MODE                                   INTERNAL

DESCRIPTION
  Enables ack mode on FMAC activation.

DEPENDENCIES
  None

PARAMETERS
  rl_carrier_id - Reverse link Carrier ID
  ack_mode - The mode to enable.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_enable_ack_mode
( 
  uint8 rl_carrier_id,
  hdrfmac_ack_mode_enum_type ack_mode
)
{
  hdrfw_msg_u fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */

  boolean send_message = TRUE;
    /* Is there a need to send the message? */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );

  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FMAC_ACK_MODE_CFG_CMD );
    /* Initialize firmware command for ackmode message */

  fw_cmd.fmac_ack_mode_cfg.carrierIdx = rl_carrier_id;

  switch (ack_mode) 
  {
    /* IS856 - Early decode disabled */
    case IS856_NO_ACK:

      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 0;
      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 1;
      fw_cmd.fmac_ack_mode_cfg.ackOverride = HDRFMAC_NO_ACK_OVERRIDE;
        /* @todo Legacy code clears only MSB by &= 0x7FFF. Is a read/write 
           needed here? */

      /* Set the ACK channel gain to Single-user ACK channel gain */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain;

      break;

#if defined (FEATURE_HDR_IS890) || defined (FEATURE_HDR_IS890A)

 /*-------------------------------------------------------------------------- 
   * The modes IS890A_MODE_ACK_BPSK, IS890A_MODE_ACK_OOK, IS890A_MODE_NAK_BPSK,
   * IS890A_MODE_NAK_OOK are added to support "ACK Channel Modulation Type 
   * Fixed Mode" in Forward Enhanced Test Application. The TAS mandates that
   * "IF THE ACK CHANNEL MODULATION TYPE FIXED MODE IS ENABLED, THEN THE ACK 
   * CHANNEL BIT FIXED MODE SHALL ALSO BE ENABLED"
   * Firmware doesn't provide explicit override control over the ACK channel 
   * modulation schemes. However, the ACK channel gain could be tweaked to 
   * achieve the desired effect. 
   *
   * Modulation Schemes:
   * Bi-polar Keying (BPSK): When acknowledging a Single user packet, the ACK
   * channel uses BPSK, with a '1' representing ACK, and -1 representing NAK. 
   *
   *                |                                 |
   *           Nak  |  Ack                           Nak Ack
   *          ---x--+--x----                    ------x--x---
   *            -1  |  1                              0  1
   *                |                                 |
   *              BPSK                               OOK 
   *
   * ON-OFF Keying (OOK): When acknowledging a MUP, the ACK channel uses OOK 
   * modulation with a 1 (ON) representing ACK and a 0 (OFF) representing a NAK.
   * 
   * From the above description, it is evident that ACK is same in both BPSK
   * and OOK except that ACK in BPSK would have single-user ACK channel gain 
   * and the ACK in OOK would have multi-user ACK channel gain. 
   *
   * However, for a NAK, OOK sends NOTHING in the ack channel. So, for 
   * IS890A_MODE_NAK_OOK, ACK channel would have to be completely disabled. 
   *
   * NAK in BPSK uses single-user ACK channel gain. 
   * ------------------------------------------------------------------------*/
#ifdef FEATURE_HDR_IS890A
    
    /* Always ACK with BPSK Modulation.  
     * The IS890A_MODE_ACK_BPSK is functionally same as IS890_MODE_0. Hence the
     * case statement has been intentionally allowed to fall through. */
    case IS890A_MODE_ACK_BPSK:

#endif /* FEATURE_HDR_IS890A */

    /* IS890 mode 0 - First slot decode mode; ACK each slot  */
    case IS890_MODE_0:

      /* Set the ACK channel gain to Single-user ACK channel gain */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain;
      
      /* Enable Single Slot decode, So DSP stops decoding after the first 
       * slot */ 
      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 1;

      /* Disable Early decode */
      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 1;

      /* Override control of ACK channel, transmit ACK every even half slot */
      fw_cmd.fmac_ack_mode_cfg.ackOverride = 
        HDRFMAC_ACH_OVERRIDE_TRANSMIT_ACK_EACH_EVEN_HS;

      break;

#ifdef FEATURE_HDR_IS890A
    /* Always NAK with BPSK Modulation.  
     * The IS890A_MODE_NAK_BPSK is functionally same as IS890_MODE_1. Hence the
     * case statement has been intentionally allowed to fall through. */
    case IS890A_MODE_NAK_BPSK:

#endif /* FEATURE_HDR_IS890A */

    /* IS856 mode 1 - Full packet decode mode; NAK each slot */
    case IS890_MODE_1:

      /* Set the ACK channel gain to Single-user ACK channel gain */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain;
      
      /* Disabling single-slot decode forces DSP to attempt decode the 
       * entire transmit duration */
      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 0;

      /* Disable Early decode */
      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 1;

      /* Override control of ACK channel, transmit NAK every even half slot */
      fw_cmd.fmac_ack_mode_cfg.ackOverride = 
        HDRFMAC_ACH_OVERRIDE_TRANSMIT_NAK_EACH_EVEN_HS;

      HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Ack Override = %x", 
        fw_cmd.fmac_ack_mode_cfg.ackOverride );

      break;

#endif /* (FEATURE_HDR_IS890) || (FEATURE_HDR_IS890A) */

#if defined(FEATURE_HDR_IS890A) && defined (FEATURE_HDR_REVA_L1)
    
    /* Always ACK using OOK modulation */
    case IS890A_MODE_ACK_OOK:
      
      /* The only difference between OOK ACK and BPSK ACK is in setting the 
       * Multi-user packet ACK channel gain instead of single-user pkt
       * ACK channel gain */ 

      /* Since AN specifies the MUP Ack channel gain relative to Ack channel
       * gain, adding delta_mup_ack_chan_gain to ack_channel_gain. Inform 
       * DSP of MUP Ack channel gain in one-fourth dB  */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        ( (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain + 
         (hdrfmac.cfg->delta_mup_ack_chan_gain * 2) );

      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 1;
      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 1;

      /* Override control of ACK channel, transmit ACK every even half slot */
      fw_cmd.fmac_ack_mode_cfg.ackOverride = 
        HDRFMAC_ACH_OVERRIDE_TRANSMIT_ACK_EACH_EVEN_HS;

      break;

    /* Always NAK using OOK modulation */
    case IS890A_MODE_NAK_OOK:

      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 0;
      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 1;

      /* Disable transmission of ACK channel */
      fw_cmd.fmac_ack_mode_cfg.ackOverride = 
        HDRFMAC_ACH_OVERRIDE_DISABLE_ACK_TRANMISSION;

      /* Set the ACK channel gain to Single-user ACK channel gain */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain;

      break;

#endif /* FEATURE_HDR_IS890A && FEATURE_HDR_REVA_L1 */

    /* IS856 compliant operation: early decoder enabled, no ACK override */
    case IS856_COMPLIANT:
      /* Set the ACK channel gain to Single-user ACK channel gain */
      fw_cmd.fmac_ack_mode_cfg.ackToPilot = 
        (int) hdrfmac.tcam_params.sub_aset[rl_carrier_id].ack_channel_gain;

      fw_cmd.fmac_ack_mode_cfg.disableEarlyDecode = 0;
      fw_cmd.fmac_ack_mode_cfg.singleSlotDecode = 0;
      fw_cmd.fmac_ack_mode_cfg.ackOverride = HDRFMAC_NO_ACK_OVERRIDE; 
      break;

    default:
      send_message = FALSE;
      ERR("Test mode not recognized",0,0,0);
  }

  if ( send_message )
  {
    estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
    ASSERT ( estatus == E_SUCCESS );
  }

} /* HDRFMAC_ENABLE_ACK_MODE */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_IS_ACTIVE

DESCRIPTION
  This function queries the current state of the protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  caller - The protocol calling this externalized function (for sw trace)

RETURN VALUE
  TRUE  = protocol is Active.
  FALSE = protocol is Inactive.

SIDE EFFECTS
  None.
  
===========================================================================*/
boolean hdrfmac_is_active 
( 
  hdrhai_protocol_name_enum_type caller 
    /*lint -esym(715,caller)
     * Ignore the argument not used(caller) lint
     * error #715 */
)
{
  return( (hdrfmacsm_get_state() != HDRFMAC_INACTIVE_STATE) ? TRUE : FALSE );

} /* hdrfmac_is_active */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_ACTIVATE

DESCRIPTION
  This command transitions the protocol to the Variable Rate state.  If the 
  protocol is already in the Variable Rate or Fixed Rate state then no action 
  is taken.
  
DEPENDENCIES
  The protocol must be initialized by calling the hdrfmac_init function prior
  to activation.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_activate ( hdrhai_protocol_name_enum_type sender )
{
  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

  boolean aset_has_more_than_one_cell;
    /* Active set has sectors from more than one cell? */

  hdrfw_msg_u fw_cmd, fw_cmd_ind;
    /* Firmware commands */

  errno_enum_type estatus;
    /* Error status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrfmac_activate" );

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRFMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRFMAC_ACTIVATE_CMD;
    cmd_buf.hdr.sender = sender;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    HDRTRACE_CMD( HDRHAI_FTC_MAC_PROTOCOL, 
      HDRFMAC_ACTIVATE_CMD, sender, hdrfmacsm_get_state() );

    if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) == FALSE ) 
    {

      hdrfmac.first_time_after_activate = TRUE;

      memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );
      memset ( &fw_cmd_ind, 0, sizeof (hdrfw_msg_u) );

      msgr_init_hdr (
        &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FMAC_ACTIVATE_CMD );
      msgr_init_hdr ( 
        &fw_cmd_ind.hdr, MSGR_HDR_RX, HDR_FW_INDICATION_CFG_CMD );

      /* Ideally this should be called right before TC assign is queued to
       * HDRSRCH. But since there is always a TC assign right after FMAC
       * activation, we set the flag here to eliminate the race condition. 
       */ 
      hdrfmac_set_awaiting_tc_assign_complete(TRUE);

      hdrfmacsm_change_state( HDRFMAC_VAR_RATE_STATE );

      hdrfmac.drc_req_mask = 0;
      hdrfmac_set_flow_control( FALSE );
        /* Make sure flow control is disabled */

      dsm_reg_mem_event_cb ( DSM_DS_SMALL_ITEM_POOL, 
                             DSM_MEM_LEVEL_HDR_FEW,
                             DSM_MEM_OP_NEW, 
                             hdrfmac_mem_low_event_cb );

      dsm_reg_mem_event_cb ( DSM_DS_SMALL_ITEM_POOL, 
                             DSM_MEM_LEVEL_HDR_MANY, 
                             DSM_MEM_OP_FREE, 
                             hdrfmac_mem_high_event_cb );

      /* Get the currently negotiated FMAC subtype from SCP */
      hdrfmac.current_subtype =
        hdrscp_get_current_subtype(HDRHAI_FTC_MAC_PROTOCOL);

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "FMAC Subtype = %d", 
                                        hdrfmac.current_subtype );

      /* Set configuration items here */


#ifdef FEATURE_HDR_REVA_L1

      if ((hdrfmac.current_subtype == HDRSCP_ENH_FMAC)
#ifdef FEATURE_HDR_REVB
          ||
          (hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
         )
      {
        hdrfmacutil_activate_enhanced_subtype ( &fw_cmd, 
                                                hdrfmac.current_subtype );

        if( hdrfmac.current_subtype == HDRSCP_MC_FMAC )
        {
          hdrsrchmcpm_set_enh_rates_status( TRUE );
            /* Convey RevB enhanced rates enable status to MCPM */
        }
        else
        {
          hdrsrchmcpm_set_enh_rates_status( FALSE );
            /* Convey to MCPM that RevB enhanced rates are not enabled */
        }

      }
      else
      {
        fw_cmd.fmac_activate.hdrSystemModeConfig = 0;
        /* Enable Default Forward Link (IS-856 Legacy mode) in mDSP */

        /* Make sure DRC Boost Length is 0 for Rev 0 calls. */

        fw_cmd.fmac_activate.drcBoostLength = 0;
        fw_cmd.fmac_activate.dscBoostLength = 0;

#endif /* FEATURE_HDR_REVA_L1 */

        hdrsrchmcpm_set_enh_rates_status( FALSE );
          /* Convey to MCPM that RevB enhanced rates are not enabled */

        fw_cmd.fmac_activate.softHandoffCreditThresh = 
          ( ( hdrfmac.cfg->handoff_delays.soft_handoff_delay << 3 ) + 2 );
        /* Set Soft Handoff Credit Threshold */

        fw_cmd.fmac_activate.softerHandoffCreditThresh = 
          ( ( hdrfmac.cfg->handoff_delays.softer_handoff_delay << 3 ) + 2 );
        /* Set Softer Handoff Credit Threshold */

        aset_has_more_than_one_cell = 
          hdrsrch_aset_has_more_than_one_cell(HDRMDSP_DEMOD_CARRIER_0);

        /* If the active set has more than one cell */
        if (aset_has_more_than_one_cell)
        {
          fw_cmd.fmac_activate.maxCredit = 
            ( ( hdrfmac.cfg->handoff_delays.soft_handoff_delay << 3 ) + 32 );
          /* Program Max credit using soft handoff delay */
        }
        else
        {
          fw_cmd.fmac_activate.maxCredit = 
            ( ( hdrfmac.cfg->handoff_delays.softer_handoff_delay << 3 ) + 32 );
          /* Program Max credit using softer handoff delay */
        }

        fw_cmd.fmac_activate.softHandoffDelayTimer = 
          HDRFMAC_DEFAULT_FMAC_HO_DELAY_TIMER;
        /* Set SoftHandoffDelayTimer */

        fw_cmd.fmac_activate.softerHandoffDelayTimer = 
          HDRFMAC_DEFAULT_FMAC_HO_DELAY_TIMER;
        /* Set SofterHandoffDelayTimer */

        fw_cmd.fmac_activate.drcLockBackOffLowSinr = 
          HDRFMAC_DRC_LOCK_BACKOFF_20_DB_V;
        /* Set low SINR threshold for drcLock backoff in Rel 0. */

        fw_cmd.fmac_activate.drcLockBackOffNormalSinr = 
          HDRFMAC_DRC_LOCK_BACKOFF_20_DB_V;
          /* Set Normal SINR threshold for drcLock Backoff. Use 20 dB.  */

        fw_cmd.fmac_activate.drcLockLength = 
          hdrfmac_drc_lock_length_table[hdrfmac.cfg->drc_lock.length];
        fw_cmd.fmac_activate.drcLockPeriod = 
          hdrfmac_drc_lock_period_table[hdrfmac.cfg->drc_lock.period];

        fw_cmd.fmac_activate.nFTCMPRestartTx = 
          HDRFMAC_DRC_SLOTS_NEEDED_FOR_RESTART; 
          /* Set DRC supervision timers. Start Tx after 16 good slots. */

        fw_cmd.fmac_activate.nFTCMDRCSupervision = 
          HDRFMAC_DRC_SUPERVISION_SLOTS;
        /* Stop Tx after 144 bad slots */
        
#ifdef FEATURE_HDR_REVA_L1 

        fw_cmd.fmac_activate.nullRateDRC384 = 
          hdrfmac.cfg->null_rate_drc_38_4_enable;
        /* Configure FW to detect DRC 0 packets at 38.4 kbps if the attribute
             enabled. */

      } /* if Subtype == default */

#endif  /* FEATURE_HDR_REVA_L1 */

      fw_cmd.fmac_activate.enableRCL = 0xC000;
      /* Turn on RCL-A+ for Rev A calls */

      hdrfmac_enable_ack_mode( HDRMDSP_DEMOD_CARRIER_0, hdrfmac.ack_mode );
        /* Set ack mode */

#ifdef FEATURE_HDR_REVA_L1

      if (( hdrfmac.current_subtype == HDRSCP_ENH_FMAC )
#ifdef FEATURE_HDR_REVB
          ||
          (hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
         )
      {
        /* Register callback to RX based on negotiated subtype */
        hdrrx_register_fmac_pkt_cb ( hdrfmacutil_put_rev_a_pkt );
      }
      else

#endif /* FEATURE_HDR_REVA_L1 */
      {
        hdrrx_register_fmac_pkt_cb ( hdrfmac_put_pkt_chain );
      }

      fw_cmd.fmac_activate.maxDRC = HDRHAI_DRC_76_8;
      /* Set the MAX allowed DRC to 2. Since AT is not allowed to request DRC 
         greater than 2 until we receive RTC ACK. */

      estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
        /* Send the message */

      ASSERT ( estatus == E_SUCCESS );

      /* Enable reception of DRC supervision indication from FW */
      fw_cmd_ind.indication_cfg.type = HDR_FW_IND__DRC_SUPERVISION;
      fw_cmd_ind.indication_cfg.enable = 1;
  
      estatus = msgr_send ( &fw_cmd_ind.hdr, sizeof ( hdrfw_msg_u ) );
        /* Send the indication config msg */
  
      ASSERT ( estatus == E_SUCCESS );
  
      /* Reset Conneted state PER statistics for new traffic session */
      hdrrx_reset_connected_state_per();

#ifdef FEATURE_HDR_REVC
      (void) hdrfmac_check_and_set_nlb_enbled();
#endif /* FEATURE_HDR_REVC */
    }
    else
    {
      HDRTRACE_CMD_IGNORED_STATE( HDRHAI_FTC_MAC_PROTOCOL, 
        HDRFMAC_ACTIVATE_CMD, sender, hdrfmacsm_get_state() );
    }


    hdrfmac_fac_gain_ctrl.drop_count = 0;
      /* Reset counter for number of times MDSP input data is rejected due to not
         crossing A0 threshold */

    if ( hdrfmac_fac_gain_ctrl.fac_gain_enable == TRUE )
      /* Allow flexibility to turn off FAC gain estimate calculation via QXDM */
    {
      /*-------------------------------------------------------------------------
        Register mdsp slot logging cb.  This call back is used to read slot
        elements from MDSP to calculate the FAC (Forward Activity Channel)
        gain.  This gain will eventually be given to MDSP for FAB (Forward
        Activity Bit) detection.
      -------------------------------------------------------------------------*/
      hdrlog_register_slot_logging_cb( HDRLOG_FMAC_MDSP_SLOT_LOGGING_CLIENT,
                                       hdrfmac_process_mdsp_logging_event );
        
      hdrfmac_fac_gain_init();
        /* Reset variables used in FAC gain estimation. */
 
      HDR_MSG_PROT_1( 
        MSG_LEGACY_HIGH, "Register FAC Gain mdsp logging,curr gain=%d to MDSP",
        hdrfmac_fac_gain_ctrl.gain );
    }
    else
    {
      hdrfmac_fac_gain_ctrl.gain             = hdrfmac_fac_gain_ctrl.fac_gain_default;
        /* At power up, initialize FAC gain to the default value recommended in the
           FW FAC Gain Estimation System document.  Since FAC gain estimation is
           filtered, the previous FAC gain is fed back to the filter to
           calculate the current FAC gain.  To start, the FAC gain must contain 
           a meaningful value.  */

      HDR_MSG_PROT_1( 
        MSG_LEGACY_HIGH, "FAC Gain calc is disabled, set to default %d",
          hdrfmac_fac_gain_ctrl.gain );
    }

    memset( &fw_cmd, 0, sizeof( hdrfw_msg_u ) );

    msgr_init_hdr( &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_GRICE_SET_FAC_GAIN_CMD );

    fw_cmd.grice_set_fac_gain.facGain = hdrfmac_fac_gain_ctrl.gain;
      /* Output FAC gain to FW */

    estatus = msgr_send( &fw_cmd.hdr, sizeof( hdrfw_msg_u ) );
      /* Send the FW message */

    ASSERT ( estatus == E_SUCCESS );

  }

} /* hdrfmac_activate */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_UPDATE_SUBTYPE_TO_FW

DESCRIPTION
  This function sends the FMAC subtype to fw. In cases when TCA is received 
  from AN long after connection request then HDR initiates this message 
  to make FW insync with SW

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_update_subtype_to_fw ( void )
{
  hdrfw_msg_u fw_cmd;
    /* Firmware commands */

  uint16 current_subtype;
    /* Current negotiated subtype */

  errno_enum_type estatus;
    /* Error status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u) );

  msgr_init_hdr ( &fw_cmd.hdr,
                MSGR_HDR_FMAC,
                HDR_FW_FMAC_MODE_UPDATE_CMD );

  current_subtype = hdrscp_get_current_subtype( HDRHAI_FTC_MAC_PROTOCOL );
    /* Get the latest subtype */

  fw_cmd.fmac_mode_update_cfg.reserved  = 0;

#ifdef FEATURE_HDR_REVB
  if (current_subtype == HDRSCP_MC_FMAC)
  {
    fw_cmd.fmac_mode_update_cfg.hdrFmacModeConfig = 2;
      /* Set HDR System to DORB mode */
  }
  else if ( current_subtype == HDRSCP_ENH_FMAC )
#else
  if ( current_subtype == HDRSCP_ENH_FMAC )
#endif /* FEATURE_HDR_REVB */
  {
    fw_cmd.fmac_mode_update_cfg.hdrFmacModeConfig = 1;
      /* Enable Enhanced Forward Link (IS-856A) in mDSP */
  }
  else
  {
    fw_cmd.fmac_mode_update_cfg.hdrFmacModeConfig = 0;
      /* Default Forward Link in mDSP */
  }

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "Sending FMAC Subtype = %d to fw(%d)", 
    current_subtype, fw_cmd.fmac_mode_update_cfg.hdrFmacModeConfig );

  estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );

  ASSERT ( estatus == E_SUCCESS );
}


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_DEACTIVATE

DESCRIPTION
  This command transitions the protocol to the Inactive state.  If the 
  protocol is already in the Inactive state then no action is taken.
  
DEPENDENCIES
  None.

PARAMETERS
  sender - The protocol giving this command (only for software trace)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_deactivate ( hdrhai_protocol_name_enum_type sender )
{
  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

  hdrfw_msg_u fw_cmd_ind;
  hdrfw_msg_u fw_cmd;
    /* Firmware commands */

  errno_enum_type estatus;
    /* Error status */

  uint8 ch;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrfmac_deactivate" );

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRFMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRFMAC_DEACTIVATE_CMD;
    cmd_buf.hdr.sender = sender;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    HDRTRACE_CMD( HDRHAI_FTC_MAC_PROTOCOL, 
      HDRFMAC_DEACTIVATE_CMD, sender, hdrfmacsm_get_state() );

    if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) == TRUE ) 
    {
      hdrfmacsm_change_state( HDRFMAC_INACTIVE_STATE );

#ifdef FEATURE_HDR_REVC
      (void) hdrfmac_check_and_set_nlb_enbled();
#endif /* FEATURE_HDR_REVC */

      /* Clear reverse channel DRC supervision restart timers */
      for (ch = 0; ch < HDRRMAC_MAX_NUM_RL_CARRIERS; ch++)
      {
        (void) rex_clr_timer( &hdrfmac.drc_sup_restart_timer[ch] );
      
      } /* for all RL carriers */


      /* Make sure flow control is disabled */
      hdrfmac_set_flow_control( FALSE );
      
      /* Reset the Ack channel mode to default */
      hdrfmac_enable_ack_mode( HDRMDSP_DEMOD_CARRIER_0, IS856_COMPLIANT );

      dsm_unreg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL,
                             DSM_MEM_LEVEL_HDR_MANY,
                             DSM_MEM_OP_FREE);

      dsm_unreg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL,
                             DSM_MEM_LEVEL_HDR_FEW,
                             DSM_MEM_OP_NEW);

      /* Disable reception of DRC supervision indication from FW */
      fw_cmd_ind.indication_cfg.type = HDR_FW_IND__DRC_SUPERVISION;
      fw_cmd_ind.indication_cfg.enable = 0;

      if (!hdrfmac_bypass_dereg_fw_msgs)
      {
        estatus = msgr_send ( &fw_cmd_ind.hdr, sizeof ( hdrfw_msg_u ) );
          /* Send the indication config msg */

        ASSERT ( estatus == E_SUCCESS );
      }

      /* Clear fmac configuration on exit */
      memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

      msgr_init_hdr (
        &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FMAC_ACTIVATE_CMD );

      estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
        /* Send the message */

      ASSERT ( estatus == E_SUCCESS );

    }
    else
    {
      HDRTRACE_CMD_IGNORED_STATE( HDRHAI_FTC_MAC_PROTOCOL, 
        HDRFMAC_DEACTIVATE_CMD, sender, hdrfmacsm_get_state() );
    }

    hdrfmac_set_awaiting_tc_assign_complete(FALSE);

#ifdef FEATURE_HDR_FL_FLOW_CONTROL

    /* Re-initialize Flow control params */
    hdrfmac.fc_is_on = FALSE;
    hdrfmac.fc_cmd =  DS707_CCM_FLOW_OFF;
    hdrfmac.fc_pattern = HDRFMAC_NO_FLOW_CONTROL;

#endif /* FEATURE_HDR_FL_FLOW_CONTROL */

#ifdef FEATURE_HDR_THERMAL_BACKOFF

    /* Go Back to default diversity state */
    hdrsrch_thermal_diversity_pref_ctrl(TRUE);

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

    hdrlog_register_slot_logging_cb( HDRLOG_FMAC_MDSP_SLOT_LOGGING_CLIENT,
                                     NULL );

    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                    "Stop FAC Gain calc, curr gain=%d to MDSP",
                    hdrfmac_fac_gain_ctrl.gain );
  }

} /* hdrfmac_deactivate */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_UPDATE_LINK_STATUS

DESCRIPTION
  This function alerts the protocol to an update of the ForwardTrafficValid 
  field received in the QuickConfig message.  A ForwardTrafficValid bit with 
  a value of '0' will cause the protocol to give the SupervisionFailed 
  indication and deactivate. 
  
DEPENDENCIES
  None.

PARAMETERS
  chan_valid_bits_63_to_0   - QuickConfig.ForwardChannelValid field.
  chan_valid_bits_127_to_64 - QuickConfig.ForwardChannelValid field.
  link_id                   - Link ID for QuickConfig message.
  sender                    - Name of protocol giving calling this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_update_link_status 
( 
  qword chan_valid_bits_63_to_0,
  qword chan_valid_bits_127_to_64,
  uint8 link_id,
  hdrhai_protocol_name_enum_type sender
)
{
  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

  boolean is_fwd_chan_valid;      /* valid status for this access terminal */

  int16  mac_index;                  /* Current MAC index */

  qword valid_bits_63_to_0_copy;    /* Copy of chan valied bits from 63-0 */

#ifdef FEATURE_HDR_REVA_L1
  qword valid_bits_127_to_64_copy;  /* Copy of chan valid bits from 127-64 */

  boolean enhanced;                 /* Is the negotiated subtype enhanced? */
#endif

  uint32 num_active_users = 0;
    /* Number of active users in a sector */

  hdrfw_msg_u fw_cmd;
    /* Firmware command message to program partial load RCL configuration */

  errno_enum_type estatus;
    /* Error status */

  boolean enable_partial_load_rcl;
    /* Is partial load RCL enabled? */

#ifdef FEATURE_MDM_FUSION
  hdrind_ind_data_union_type ind_data;
    /* Indication data */
#endif /* FEATURE_MDM_FUSION */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrfmac_update_link_status" );

#ifdef FEATURE_HDR_REVA_L1
  /* Check if the negotiated FMAC subtype is enhanced */  
  enhanced = 
        ( ((hdrfmac.current_subtype == HDRSCP_ENH_FMAC)
#ifdef FEATURE_HDR_REVB
           ||
           (hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
           )? TRUE: FALSE );
#endif 

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRFMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd          = HDRFMAC_SUPERVISION_CMD;
    cmd_buf.hdr.sender       = sender;

    /* Copy the parameter to command buffer */
    qw_equ( cmd_buf.supv.chan_valid_bits_63_to_0, chan_valid_bits_63_to_0);

#ifdef FEATURE_HDR_REVA_L1
    if ( enhanced )
    {
      qw_equ( cmd_buf.supv.chan_valid_bits_127_to_64, 
              chan_valid_bits_127_to_64 );
    }
#endif
    cmd_buf.supv.link_id          = link_id;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    /* If awaiting_tc_assign_complete is TRUE, meaning HDR SRCH is still
     * processing TC Assign command. We should not check the FT valid until 
     * SRCH finishes processing TC assign cmd. Otherwise it is likely that 
     * QC is received on old channel and FT validation might fail. 
     */ 
    if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) &&
         !hdrfmac.awaiting_tc_assign_complete ) 
    {
      /* Copy the channel valid fields for F3 message display */
      qw_equ( valid_bits_63_to_0_copy, chan_valid_bits_63_to_0 );

#ifdef FEATURE_HDR_REVA_L1
      qw_equ( valid_bits_127_to_64_copy, chan_valid_bits_127_to_64 );
#endif

      mac_index = hdrsrch_get_rl_mac_index ( link_id );
        /* Retrieve the current MAC index from SRCH */

#ifdef FEATURE_HDR_REVA_L1
      /* If the subtype is Enhanced */
      if ( enhanced )
      {
        /* Valid range for Enhanced FTC Mac index is 5-63 and 66-127 
           mac_index is an int8 and cannot go beyond 127. Hence commented out
           the last condition to avoid lint warning. */
        if ( (mac_index < 4) ||
            ((mac_index > 63) && (mac_index <= 65)) /*|| (mac_index > 127)*/ )
          return;
      }
      else
#endif
      {
        if ( (mac_index < 4) || (mac_index > 63) ) return;
          /* Valid range for Default FTC Mac index is 5-63 */
      }

#ifdef FEATURE_HDR_REVA_L1
      /* Since mac_index is valid, any mac_index greater than 64 has to
         be of enhanced FMAC subtype */
      if ( mac_index >= 64 )
      {
        qw_shift( valid_bits_127_to_64_copy, -(mac_index-64) );
          /* Find bit associated with our mac_index */

        is_fwd_chan_valid =
         ( ( ( qw_lo(valid_bits_127_to_64_copy) & 0x01 ) != 0 )? TRUE: FALSE );
          /* Determine the Forward Traffic Valid status */
      }
      else 
#endif
      {
        qw_shift( valid_bits_63_to_0_copy, -mac_index );
          /* Find bit associated with our mac_index */

      is_fwd_chan_valid = 
          ( ( ( qw_lo(valid_bits_63_to_0_copy) & 0x01 ) != 0 )? TRUE: FALSE );
          /* Determine Forward Traffic Valid status */
      }

      /* If the Forward traffic channel is invalid */
      if ( !is_fwd_chan_valid )
      {
        hdrplog_inc_conn_term_stats_counter(
          HDRPLOG_FTCMAC_SUPERVISION_FAILURE_COUNT, 1 );
          /* Increment the number of FTCMAC supervision failures for logging 
             purposes. */

#ifdef FEATURE_HDR_REVA_L1
        if ( mac_index >= 64 )
        {
          HDR_MSG_PROT_5( MSG_LEGACY_HIGH, 
                          "Enh FT Valid lost!: %x %x %x %x Mac%d",
                          qw_hi(chan_valid_bits_127_to_64), 
                          qw_lo(chan_valid_bits_127_to_64), 
                          qw_hi(chan_valid_bits_63_to_0), 
                          qw_lo(chan_valid_bits_63_to_0), 
                          mac_index );
        }
        else 
#endif
        {
          HDR_MSG_PROT_3( MSG_LEGACY_HIGH, 
                          "FT Valid lost!: %x %x Mac%d",
                          qw_hi(chan_valid_bits_63_to_0), 
                          qw_lo(chan_valid_bits_63_to_0), 
                          mac_index );
        }

#ifdef FEATURE_MDM_FUSION
        if (rfm_sv_get_sar_pwr_limits_status())  
        { 
          /* Indicate SAR limit (with 1x) is likely reason for failure. */
          ind_data.sar_power_limited = TRUE;
        }
        else
        {
          ind_data.sar_power_limited = FALSE;
        }

        /* Give FTCMAC supervision failed indication... */
        hdrind_give_ind( HDRIND_FTCMAC_SUPERVISION_FAILED, (void*) &ind_data );
#else
        /* Give FTCMAC supervision failed indication... */
        hdrind_give_ind( HDRIND_FTCMAC_SUPERVISION_FAILED, NULL );
#endif /* FEATURE_MDM_FUSION */

        /* ...and deactivate. */
        hdrfmac_deactivate(HDRHAI_FTC_MAC_PROTOCOL);

      } /* !is_fwd_chan_valid */
      else
      {
        /*---------------------------------------------------------------------
          FTC Valid!!! Check the number of active users to see if we are still 
          allowed to use Enhanced Rate Control Loop.
        ----------------------------------------------------------------------*/
        num_active_users = 
          hdrutil_bit_count32( qw_hi(chan_valid_bits_63_to_0) ) + 
          hdrutil_bit_count32( qw_lo(chan_valid_bits_63_to_0) );
          /* Find the number of active users in the sector */

  #ifdef FEATURE_HDR_REVA_L1
        if ( enhanced )
        {
          num_active_users += 
            ( hdrutil_bit_count32( qw_hi(chan_valid_bits_127_to_64) ) +
              hdrutil_bit_count32( qw_lo(chan_valid_bits_127_to_64) ) );
            /* Included the number of Rev-A users also in the count */
        }
  #endif /* FEATURE_HDR_REVA_L1 */

        if (hdrfmac_fac_gain_ctrl.grice_enabled)
        {
          /* Disable rate control loop enhancement when gRice is enabled */
          enable_partial_load_rcl = FALSE;
        }
        else

        /* If the number of active users in the sector is greater than maximum
           allowed... */ 
        if ( num_active_users > HDRFMAC_ENH_RCL_MAX_ALLOWED_USERS )
        {
          enable_partial_load_rcl = FALSE;
            /* Disable rate control loop Enhancement */

          HDR_MSG_PROT(MSG_LEGACY_HIGH,
                       "No of active users in sector > 8. Disabled RCL Enh");
        }
        else
        {
          enable_partial_load_rcl = TRUE;
            /* Enable rate control loop Enhancement */

          HDR_MSG_PROT(MSG_LEGACY_HIGH,
                       "No of active users in sector <= 8. Enabled RCL Enh");
        }

        /* Send message only during first timer after activate or if there 
           is a change */
        if ( (hdrfmac.first_time_after_activate) ||
             (hdrfmac.enable_partial_load_rcl != enable_partial_load_rcl) )
        {
          memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

          hdrfmac.first_time_after_activate = FALSE;
          hdrfmac.enable_partial_load_rcl = enable_partial_load_rcl;

          msgr_init_hdr (
            &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_PARTIAL_LD_RCL_CFG_CMD );
            /* Initialze message header */

          fw_cmd.partial_ld_rcl_cfg.enablePartialLoadRCL = 
            hdrfmac.enable_partial_load_rcl;

          estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
            /* Send the message */

          ASSERT ( estatus == E_SUCCESS );
        }

        hdrsrch_update_num_sector_users( num_active_users );
          /* update searcher with the number of active users in sector */
#ifdef FEATURE_HDR_FLEX_CONN
        hdrrmac_update_num_mac_indexes( num_active_users );
#endif /* FEATURE_HDR_FLEX_CONN */
      }

    } /* If FMAC protocol is in active state */

  } /* In home task */

} /* hdrfmac_update_link_status */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_CMD

DESCRIPTION
  This function processes the commands for this protocol that were queued
  on the task command queue from a different task context.
  
DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - pointer to received command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_cmd 
( 
  hdrfmac_cmd_union_type *cmd_ptr                 
)
{ 
  switch ( cmd_ptr->hdr.cmd )
  {
    case HDRFMAC_ACTIVATE_CMD:
      hdrfmac_activate(cmd_ptr->hdr.sender);
      break;

    case HDRFMAC_DEACTIVATE_CMD:
      hdrfmac_deactivate(cmd_ptr->hdr.sender);
      break;

    case HDRFMAC_SUPERVISION_CMD:
      hdrfmac_update_link_status( cmd_ptr->supv.chan_valid_bits_63_to_0,
                                  cmd_ptr->supv.chan_valid_bits_127_to_64,
                                  cmd_ptr->supv.link_id,
                                  cmd_ptr->hdr.sender );
      break;

#ifdef FEATURE_HDR_THERMAL_BACKOFF

    case HDRFMAC_THERMAL_DIV_CMD:
      hdrfmac_control_thermal_div_cmd(cmd_ptr->thermal_div.thermal_div_cmd);
      break;

#endif /* FEATURE_HDR_THERMAL_BACKOFF */


#if defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD) || \
    defined(FEATURE_HDR_FLEX_CONN) || defined(FEATURE_HDR_DEVICE_ONLY_DTX)

    case HDRFMAC_SET_FL_ACTIVE_CB_CMD:
      hdrfmac_set_fl_active_cb ( cmd_ptr ->set_active_cb.cb,cmd_ptr ->set_active_cb.client_id,
                                 cmd_ptr ->set_active_cb.payload );
      break;

#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD || FEATURE_HDR_FLEX_CONN || FEATURE_HDR_DEVICE_ONLY_DTX */

#ifdef FEATURE_HDR_HANDLE_FC_IN_HDRTX
    case HDRFMAC_FLOW_CONTROL_CMD:
      hdrfmac_control_flow_cb( cmd_ptr->flow_contol.cmd );
      break;
#endif /* FEATURE_HDR_HANDLE_FC_IN_HDRTX */

    default:
      ERR("Invalid command! = 0x%h",cmd_ptr->hdr.cmd, 0, 0);
  }

} /* hdrfmac_process_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_SIG

DESCRIPTION
  This function handles the task signals assigned to this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  signal - task signal to be processed.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_sig 
( 
  rex_sigs_type signal                          
)
{

  ERR("Invalid signal! = 0x%h",signal,0,0);

} /* hdrfmac_process_sig */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_FIX_MODE_RSP_MSG

DESCRIPTION
  This function processes the Fixed Mode Response message.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_fix_mode_rsp_msg ( void ) 
{
  /* -- Fixed mode not supported yet -- */

} /* hdrfmac_process_fix_mode_rsp_msg */

#ifdef FEATURE_HDR_REVC
/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_START_LOADADJ_TIMER

DESCRIPTION
  This function starts a LoadAdj timer for the oldest entry
  
DEPENDENCIES
  None.

PARAMETERS
  timer_id - ID of the timer just expired.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_start_loadadj_timer
( 
  time_type cur_time 
)
{
  hdrfmac_loadadj_cache_type *loadadj_ptr = NULL;
  uint8 i;
  uint32 time_elapsed = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  rex_clr_timer(&hdrfmac.loadadj_timer);

  for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
  {
    /* find the oldest valid entry */
    if (hdrfmac.loadadj[i].state != HDRFMAC_LOADADJ_CACHE_INVALID &&
        (loadadj_ptr == NULL ||
         qw_cmp(loadadj_ptr->timestamp, 
                hdrfmac.loadadj[i].timestamp) > 0))
    {
      loadadj_ptr = &hdrfmac.loadadj[i];
    }
  }

  /* restart timer for the oldest entry, which will expire soonest */
  if (loadadj_ptr != NULL)
  {
    time_elapsed = (uint32) hdrutil_calc_duration(loadadj_ptr->timestamp,
                                                  cur_time);

    if (time_elapsed < HDRFMAC_LOADADJ_TIMERS_MS)
    {
      rex_set_timer(&hdrfmac.loadadj_timer,
                    HDRFMAC_LOADADJ_TIMERS_MS - time_elapsed);

      HDR_MSG_PROT_3(MSG_LEGACY_MED, "start_load_timer ch %d pn %d for %dms",
                     loadadj_ptr->serving_sector.channel.chan_num,
                     loadadj_ptr->serving_sector.pilot_pn,
                     HDRFMAC_LOADADJ_TIMERS_MS - time_elapsed);
    }
    else
    {
      HDR_MSG_PROT(MSG_LEGACY_MED, "load timer expired on all CC");
    }
  }
}


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_UPDATE_LOADADJ

DESCRIPTION
  This function combines PN info from OVHD NLB cache with the LoadAdj
  from FMAC, and sends LoadAdj update to SRCH
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_update_loadadj
(
  hdrfmac_loadadj_cache_type* loadadj_ptr
)
{
  uint8 i;
  hdrsrch_loadadj_type loadadj_update;
  uint8 pos[HDRFMAC_ASET_PER_CHAN];
  errno_enum_type err;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT(loadadj_ptr != NULL);

  if (!hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL))
  {
    HDR_MSG_PROT(MSG_LEGACY_MED, 
                 "not connected, ignore update");
    return;
  }

  if(loadadj_update.subaset[0].loadadj_cnt > HDRSRCH_MAX_SUBASET_SIZE)
  {
    HDR_MSG_PROT_1( MSG_LEGACY_ERROR, 
                    "loadadj_cnt (%d) more than allowed. Ignoring update LoadAdj request",
                    loadadj_update.subaset[0].loadadj_cnt);

    return;
  }

  if (loadadj_ptr->state != HDRFMAC_LOADADJ_CACHE_INVALID)
  {
    loadadj_update.subaset_cnt = 1;
#ifdef FEATURE_MEMCPY_REMOVAL
    loadadj_update.subaset[0].serving_sector = loadadj_ptr->serving_sector;
#else
    memcpy(&loadadj_update.subaset[0].serving_sector,
           &loadadj_ptr->serving_sector, 
           sizeof(sys_link_type));
#endif /* FEATURE_MEMCPY_REMOVAL */
  
    loadadj_update.subaset[0].loadadj_serving = 
      loadadj_ptr->loadadj_serving;
  
    err = 
      hdrovhd_get_nlb_aset_neighbors(&loadadj_ptr->serving_sector, 
                                     &loadadj_update.subaset[0].loadadj_cnt,
                                     loadadj_update.subaset[0].pn,
                                     pos);

    if (err == E_SUCCESS)
    {
      loadadj_ptr->state = HDRFMAC_LOADADJ_CACHE_OVHD_RCVD;

      for (i=0; i<loadadj_update.subaset[0].loadadj_cnt; i++)
      {
        if (pos[i] < loadadj_ptr->loadadj_cnt)
        {
          loadadj_update.subaset[0].loadadj[i] = 
            loadadj_ptr->loadadj[pos[i]];

          HDR_MSG_PROT_2(MSG_LEGACY_MED,
                         "update_loadadj pn %d adj %d",
                         loadadj_update.subaset[0].pn[i],
                         loadadj_update.subaset[0].loadadj[i]);
        }
        else
        {
          HDR_MSG_PROT_2(MSG_LEGACY_ERROR, 
                         "update_loadadj pos %d > loadadj_cnt %d",
                         pos[i], loadadj_ptr->loadadj_cnt);

          loadadj_update.subaset[0].loadadj[i] = loadadj_ptr->loadadj_serving;
        }
      }

      /* report send update to search only if in traffic */
      hdrsrch_update_loadadj(&loadadj_update);
    }
    else
    {
      loadadj_ptr->state = HDRFMAC_LOADADJ_CACHE_WAITING_OVHD;
    }
  }

  for (i=0; i<loadadj_update.subaset[0].loadadj_cnt; i++)
  {
    HDR_MSG_PROT_2(MSG_LEGACY_MED,
                   "update_loadadj pn %d adj %d",
                   loadadj_update.subaset[0].pn[i],
                   loadadj_update.subaset[0].loadadj[i]);
  }
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_LOAD_INFO_MSG

DESCRIPTION
  This function processes the LoadInfo message.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_load_info_msg 
(  
  dsm_item_type *msg_ptr
) 
{  
  uint8 msg_offset = 8; /* MsgID length */
  hdrsrch_link_id_type link_id;
  sys_link_type serving_sector;

  hdrfmac_loadadj_cache_type *loadadj_cache_ptr;
    
  uint8 i;
  errno_enum_type err;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT(msg_ptr != NULL);

  link_id = (hdrsrch_link_id_type) HDRDSM_GET_LINK_ID( msg_ptr );
  hdrlmac_get_link_info( link_id, &serving_sector );

  if (!hdrlmac_pilot_is_in_aset(&serving_sector.channel, 
                                serving_sector.pilot_pn))
  {
    HDR_MSG_PROT_2(MSG_LEGACY_MED,
                   "proc_load_info non ASET load info ch %d pn %d",
                   serving_sector.channel.chan_num,
                   serving_sector.pilot_pn);
    return;
  }

  loadadj_cache_ptr = hdrfmac_find_loadadj_cache(&serving_sector.channel);

  if (loadadj_cache_ptr == NULL)
  {
    loadadj_cache_ptr = hdrfmac_find_free_loadadj_cache();
    ASSERT(loadadj_cache_ptr != NULL);
  }

#ifdef FEATURE_MEMCPY_REMOVAL
  loadadj_cache_ptr->serving_sector = serving_sector;
#else
  memcpy(&loadadj_cache_ptr->serving_sector,
         &serving_sector, 
         sizeof(sys_link_type));
#endif /* FEATURE_MEMCPY_REMOVAL */

  err = HDRBIT_UNPACK8(msg_ptr, &loadadj_cache_ptr->loadadj_serving, 
                       msg_offset, HDRFMAC_LOAD_ADJ_LEN);
  msg_offset += HDRFMAC_LOAD_ADJ_LEN;

  loadadj_cache_ptr->loadadj_cnt = 0;
  err |= HDRBIT_UNPACK8(msg_ptr, &loadadj_cache_ptr->loadadj_cnt, 
                       msg_offset, HDRFMAC_NEIGHBOR_COUNT_LEN);
  msg_offset += HDRFMAC_NEIGHBOR_COUNT_LEN;

  HDR_MSG_PROT_3(MSG_LEGACY_MED, "proc_load_info LoadAdj %d cnt %d err %d",
                loadadj_cache_ptr->loadadj_serving, 
                loadadj_cache_ptr->loadadj_cnt, err);

  if(loadadj_cache_ptr->loadadj_cnt > HDRHAI_MAX_SP_NEIGHBORS)
  {
    HDR_MSG_PROT_1( MSG_LEGACY_ERROR, 
                    "loadadj_cnt (%d) more than allowed. Ignore process_load_info_msg() ",
                    loadadj_cache_ptr->loadadj_cnt);

    return;
  }

  for (i=0; i<loadadj_cache_ptr->loadadj_cnt; i++)
  {
    err |= HDRBIT_UNPACK8(msg_ptr, &loadadj_cache_ptr->loadadj[i], 
                          msg_offset, HDRFMAC_LOAD_ADJ_LEN);
    msg_offset += HDRFMAC_LOAD_ADJ_LEN;

    HDR_MSG_PROT_3(MSG_LEGACY_MED, "proc_load_info LoadAdj %d @ %d err %d",
                   loadadj_cache_ptr->loadadj[i], i, err);
  }

  if (err == E_SUCCESS)
  {
    time_get(loadadj_cache_ptr->timestamp);
    loadadj_cache_ptr->state = HDRFMAC_LOADADJ_CACHE_WAITING_OVHD;
    hdrfmac_start_loadadj_timer(loadadj_cache_ptr->timestamp);

    if (!hdrmc_is_qos_flow_active() &&
        hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL))
    {
      hdrfmac_update_loadadj(loadadj_cache_ptr);
    }
  }
  else
  {
    loadadj_cache_ptr->state = HDRFMAC_LOADADJ_CACHE_INVALID;
  }
} /* hdrfmac_process_load_info_msg */

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_OVHD_NLB_CACHE_UPDATED

DESCRIPTION
  This function processes OVHD_NLB_CACHE_UPDATED indication. It goes
  thru all valid LoadAdj cache and updates HDRSRCH.
  
DEPENDENCIES
  None.

PARAMETERS
  serving_sector - Sector on which the overhead cache is updated. If chan/pn
                   are set to 0, all cache are updated.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_ovhd_nlb_cache_updated
( 
  sys_link_type *serving_sector
)
{
  uint8 i;
  hdrfmac_loadadj_cache_type *loadadj_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT(serving_sector != NULL);

  HDR_MSG_PROT(MSG_LEGACY_MED, "proc_ovhd_nlb_cache_updated");

  if (!hdrfmac_is_loadinfo_supported() ||
      !hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) ||
      hdrmc_is_qos_flow_active())
  {
    return;
  }

  if (serving_sector->channel.chan_num == 0 &&
      serving_sector->pilot_pn == 0)
  {
    /* all cache updated */
    for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
    {
      loadadj_ptr = &hdrfmac.loadadj[i];

      if (loadadj_ptr->state != HDRFMAC_LOADADJ_CACHE_INVALID &&
          hdrlmac_pilot_is_in_aset(&loadadj_ptr->serving_sector.channel,
                                   loadadj_ptr->serving_sector.pilot_pn))
      {
        hdrfmac_update_loadadj(loadadj_ptr);
      }
    }
  }
  else
  {
    loadadj_ptr = hdrfmac_find_loadadj_cache(&serving_sector->channel);
    if (loadadj_ptr != NULL &&
        loadadj_ptr->state != HDRFMAC_LOADADJ_CACHE_INVALID &&
        loadadj_ptr->serving_sector.pilot_pn == serving_sector->pilot_pn)
    {
      hdrfmac_update_loadadj(loadadj_ptr);
    }
  }
}

/*===========================================================================
 
FUNCTION hdrfmac_reset_loadadj_cache

DESCRIPTION
  This function combines the neighbors info from OVHD with load_info, and
  sends the update to HDRSRCH, which in turn sends to FW
 
DEPENDENCIES
  None.

PARAMETERS
  load_info_ptr - Pointer to load_info table to send

RETURN VALUE
  None.

SIDE EFFECTS
  None.
 
===========================================================================*/
void hdrfmac_reset_loadadj_cache
(
  hdrfmac_loadadj_cache_type* loadadj_ptr
)
{
  hdrsrch_loadadj_type loadadj_update;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  ASSERT(loadadj_ptr != NULL);

  loadadj_ptr->loadadj_cnt = 0;
  loadadj_ptr->loadadj_serving = 0;

  if (hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL))
  {
    loadadj_update.subaset_cnt = 1;
    /* disable NLB on this carrier if LoadInfo timer expired */
    loadadj_update.subaset[0].disable_nlb = TRUE;
#ifdef FEATURE_MEMCPY_REMOVAL
    loadadj_update.subaset[0].serving_sector = loadadj_ptr->serving_sector;
#else
    memcpy(&loadadj_update.subaset[0].serving_sector,
           &loadadj_ptr->serving_sector,
           sizeof(sys_link_type));
#endif /* FEATURE_MEMCPY_REMOVAL */
    loadadj_update.subaset[0].loadadj_serving = 0;
    loadadj_update.subaset[0].loadadj_cnt = 0;

    hdrsrch_update_loadadj(&loadadj_update);
  }

  loadadj_ptr->state = HDRFMAC_LOADADJ_CACHE_INVALID;
}

/*===========================================================================
FUNCTION HDRFMAC_IS_LOADINFO_SUPPORTED

DESCRIPTION
  This function checks if LoadInfo is negotiated

DEPENDENCIES 
  None 
 
PARAMETERS 
  None
 
RETURN VALUE
  True  - if LoadInfo supported
  False - if LoadInfo is not supported

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrfmac_is_loadinfo_supported( void )
{
  uint8 fmac_subtype;
  boolean load_info_supported = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  fmac_subtype = hdrscp_get_current_subtype(HDRHAI_FTC_MAC_PROTOCOL);

  if (fmac_subtype != HDRSCP_DEFAULT_FMAC)
  {
    (void) hdrscmdb_get_inuse_attrib(HDRHAI_FTC_MAC_PROTOCOL, 
                                     HDRSCMFMAC_ATTRIB_LOAD_INFO_SUPPORTED,
                                     &load_info_supported,
                                     fmac_subtype);
  }

  return load_info_supported;
}

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_QOS_FLOW_STATUS_CHANGED

DESCRIPTION
  This function processes QOS_FLOW_STATUS_CHANGED indication. If QoS
  changed from inactive to active, nothing needs to be done. If QoS
  changed from active to inactive, sends update to HDRSRCH to clear
  LoadAdj bias
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_qos_flow_status_changed( void )
{
  uint8 i;
  hdrfmac_loadadj_cache_type* loadadj_ptr;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (!hdrfmac_is_loadinfo_supported() ||
      !hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) ||
      !hdrmc_is_qos_flow_active())
  {
    return;
  }

  /* if QoS is inactive, send FMAC update */

  for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
  {
    loadadj_ptr = &hdrfmac.loadadj[i];

    if (loadadj_ptr->state != HDRFMAC_LOADADJ_CACHE_INVALID &&
        hdrlmac_pilot_is_in_aset(&loadadj_ptr->serving_sector.channel,
                                 loadadj_ptr->serving_sector.pilot_pn))
    {
      hdrfmac_update_loadadj(loadadj_ptr);
    }
  }
}
#endif /* FEATURE_HDR_REVC */

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_MSG

DESCRIPTION
  This function processes incoming messages for this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - pointer to receive message buffer.
  protocol_instance - Instance of the protocol the message is for

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_msg 
( 
  dsm_item_type *msg_ptr,                  
  hdrhai_protocol_instance_enum_type protocol_instance
)
{
  hdrfmac_msg_id_type  msg_id;                         /* Message ID field */

#ifdef FEATURE_HDR_REVC
  uint8 sub_type = hdrscp_get_current_subtype(HDRHAI_FTC_MAC_PROTOCOL);
#endif /* FEATURE_HDR_REVC */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT((protocol_instance == HDRHAI_IN_USE) ||
         (protocol_instance == HDRHAI_IN_CONFIG)); 

  /*-------------------------------------------------------------------------
   Extract the Message ID field. 
  -------------------------------------------------------------------------*/
  if ( HDRFMAC_GET_MSG_ID( msg_ptr, msg_id ) != E_SUCCESS )
  {
    ERR("Message ID extraction failed.", 0, 0, 0);
    dsm_free_packet( &msg_ptr );
    return;
  }

  HDRTRACE_MSG( HDRHAI_FTC_MAC_PROTOCOL, msg_id, 
    hdrfmacsm_get_state() );
  /*-------------------------------------------------------------------------
   Perform the corresponding message processing. 
  -------------------------------------------------------------------------*/
  if (protocol_instance == HDRHAI_IN_USE)
  {
    switch( msg_id )
    {
      case HDRFMAC_FIX_MODE_RSP_MSG_ID:
        HDR_MSG_PROT( MSG_LEGACY_HIGH,"Fixed mode response message received" );
        hdrfmac_process_fix_mode_rsp_msg();
        break;
  
#ifdef FEATURE_HDR_REVC
      case HDRFMAC_LOAD_INFO_MSG_ID:

        /* Load Information message received */
        HDR_MSG_PROT( MSG_LEGACY_MED, "LoadInfoMsg received" );

        hdrfmac_set_load_info_msg_rcvd(TRUE);
        
        if (sub_type != HDRSCP_DEFAULT_FMAC)
        {          
          if (hdrfmac_is_loadinfo_supported())              
          {
            hdrfmac_process_load_info_msg(msg_ptr);
          }
          else
          {
            HDR_MSG_PROT( MSG_LEGACY_ERROR,
                          "Recv LoadInfoMsg when LoadInfo not supported" );

            /* If LoadInformationSupported attribute is not supported, check 
               the conditions of updating LoadInformationSupported attribute 
               using GAUP. Conditions are:
               1. have received a Load Information message from current serving
                  sector in either CONNECTED or IDLE state;
               2. after entering current subnet, the AN hasn't rejected the
                  LoadInformationSupported attribute proposed (either by
                  Config or GAUP) by the AT;
               3. the phone is in connected state. 
            */
            if ( hdrfmac_is_load_info_msg_rcvd() &&
                 !hdrscmfmac_is_load_info_attrib_rejected() &&
                 hdralmps_get_state() == HDRALMPS_CONNECTED )
            {
              if ( hdrscmfmac_gaup_load_info_supported() != E_SUCCESS)
              {
                HDR_MSG_PROT( MSG_LEGACY_ERROR,
                              "LoadInfoSupported update request failed" );
              }
            }
          }
        }
        break;
#endif /* FEATURE_HDR_REVC */

      default:
        ERR("Invalid in use message ID %d", msg_id, 0, 0);
    }
  }
  
  /*-------------------------------------------------------------------------
   Free the message buffer memory. 
  -------------------------------------------------------------------------*/
  dsm_free_packet( &msg_ptr );

} /* hdrfmac_process_msg */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_MSG_CB

DESCRIPTION
  This callback function queues incoming messages for this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_msg_cb 
( 
  dsm_item_type *msg_ptr
)
{
  hdrrx_queue_msg( HDRHAI_FTC_MAC_PROTOCOL, HDRHAI_IN_USE, msg_ptr ); 

} /* hdrfmac_msg_cb */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_CONFIG_MSG_CB

DESCRIPTION
  This callback function queues incoming messages for the in config instance
  of this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - pointer to receive message buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_config_msg_cb 
( 
  dsm_item_type *msg_ptr
)
{
  hdrrx_queue_msg( HDRHAI_FTC_MAC_PROTOCOL, HDRHAI_IN_CONFIG, msg_ptr ); 

} /* hdrfmac_config_msg_cb */


/* EJECT */

#ifndef MSM5500_IRAM_FWD
/*===========================================================================

FUNCTION HDRFMAC_PUT_PKT_CHAIN

DESCRIPTION
  This function breaks a rel-0 forward traffic channel packet from a chain of 
  DSM items, parses the MAC trailer and then forwards the connection layer 
  format information along with the enclosed Security Layer packet to the 
  Security Layer for processing.

  Note: No security layer support yet! So forwarded to Packet Consolidation 
  Protocol (Connection Layer).
   
DEPENDENCIES
  None.

PARAMETERS
  rx_pkt_chain - pointer to buffered Forward Traffic Channel packet chain.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_put_pkt_chain
( 
  dsm_item_type *rx_pkt_chain
)
{ 
  /* Forward Traffic Channel packet tail bytes buffer. */
  byte     ftc_pkt_tail[ HDRFMAC_PKT_TAIL_LEN ];

  /* Security Layer packet size included status. */
  boolean  is_size_inc;                    

  /* Temporary pointer to forward traffic channel packet */
  dsm_item_type *ftc_pkt_ptr = NULL;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   Determine if the protocol is in the Active state.  If so, forward the
   Forward Traffic Channel packet for processing.  Otherwise, discard the
   Forward Traffic Channel packet.
  -------------------------------------------------------------------------*/
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) == TRUE )
  {
    /*-----------------------------------------------------------------------
     Protocol in the Active state.  Extract the packet tail bytes which 
     include the Format bit field (FMT), FCS field and TAIL bits.  Determine 
     the states of the Authenticated, Encrypted and SizeIncluded format bits
     and forward the result for further processing.
    -----------------------------------------------------------------------*/

#if defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)|| \
    defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
    hdrfmac_fl_activity_monitor();
#endif /* defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||
          defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

    /* Decouple FTC packets, which are stored as linked dsm items */
    while (rx_pkt_chain)
    {
      /* Break one FMAC packet at a time from the chain */
      ftc_pkt_ptr = rx_pkt_chain;
      rx_pkt_chain = rx_pkt_chain->pkt_ptr;
      ftc_pkt_ptr->pkt_ptr = NULL;

      /* Cheat (CPU optimization): 
         We know that this is not a dsm chain, so just grab the bytes */ 
      //dsm_pullup_tail( &ftc_pkt_ptr, ftc_pkt_tail, HDRFMAC_PKT_TAIL_LEN );

      /* Grab byte; set packet length */
      ftc_pkt_tail[0] = ftc_pkt_ptr->data_ptr[125];
      ftc_pkt_ptr->used = HDRFMAC_PKT_LENGTH;

      if (HDRFMAC_PKT_IS_MAC_VALID( ftc_pkt_tail )) 
      {
        is_size_inc = HDRFMAC_PKT_IS_SIZE( ftc_pkt_tail );

        hdrpcp_put_pkt( ftc_pkt_ptr, is_size_inc );
      }
      else
      {
        /* Discard empty packet */
        dsm_free_packet ( &ftc_pkt_ptr );
      }

    } /* while */

  } 
  else
  {
    /*-----------------------------------------------------------------------
     Protocol in the Inactive state.  Discard the packet.
    -----------------------------------------------------------------------*/
    HDRTRACE_FMAC_DROP_PKT();
    dsm_free_packet ( &rx_pkt_chain );

  }/* if ( hdrfmac_is_active ) */

} /* hdrfmac_put_pkt_chain() */


#endif /* MSM5500_IRAM_FWD */


/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_IND 

DESCRIPTION
  This function processes indications given to FMAC.
  
DEPENDENCIES
  None.

PARAMETERS
  ind_name - Name of the indication for FMAC to process
  ind_data_ptr - pointer to data associated with the indication
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
  hdrind_ind_data_union_type * ind_data_ptr
)
{

  hdrscmfmac_verno_type hdrfmac_ver_no;
    /* uint8 Array to hold the version numbers of FTC MAC config attribs */

#ifdef FEATURE_HDR_REVC
  uint8 fmac_sub_type = hdrscp_get_current_subtype(HDRHAI_FTC_MAC_PROTOCOL);
    /* current fmac subtype */
#endif /* FEATURE_HDR_REVC */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  

  HDRTRACE_IND(HDRHAI_FTC_MAC_PROTOCOL, ind_name, hdrfmacsm_get_state() );

  switch (ind_name)
  {
    case HDRIND_SCM_SESSION_CHANGED:

      /* Update the current subtype after session change */
      hdrfmac.current_subtype = 
        hdrscp_get_current_subtype( HDRHAI_FTC_MAC_PROTOCOL );

      /* Get the current value and the version number for each attribute */
      hdrscmdb_get_inuse_config_and_verno( 
        HDRHAI_FTC_MAC_PROTOCOL,
        hdrfmac.cfg,
        hdrfmac_ver_no,
        HDRSCMFMAC_MAX_NUM_CONFIG_ATTRIBS );

#ifdef FEATURE_HDR_REVC      
      /* Check whether LoadInformationSupported is accepted in the session
         configuration negotiation */
      if ( fmac_sub_type != HDRSCP_DEFAULT_FMAC &&
           ind_data_ptr->change_reason == HDRHAI_SCHG_OPENED_NORMAL &&
           !hdrfmac.cfg->load_info_supported )
      {
        hdrscmfmac_set_load_info_attrib_rejected(TRUE);
      }

      /* refresh QoS status in case prev indication was ignored due to
         LoadInfo wasn't supported */
      hdrfmac_process_qos_flow_status_changed();
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_REVA_L1

      hdrfmacutil_init_gaup_attrib_lut( hdrfmac.current_subtype, hdrfmac.cfg );

      /* Record the version numbers that we need */
      hdrfmacutil_record_version_nums(hdrfmac_ver_no);

      break;

    case HDRIND_SCM_FMAC_GAUP:

      hdrfmacutil_process_attrib_update(&hdrfmac.tcam_params);

#endif /* FEATURE_HDR_REVA_L1 */

      break;

#ifdef FEATURE_HDR_REVC

    case HDRIND_SCM_FMAC_AT_GAUP_ACCEPTED:

      HDR_MSG_PROT( MSG_LEGACY_HIGH,
                    "LoadInformationSupported update req accepted by AN" );

      if ( fmac_sub_type != HDRSCP_DEFAULT_FMAC )
      {
        /* Synchronize attribute's local copy with scmdb */
        (void) hdrscmdb_get_inuse_attrib(HDRHAI_FTC_MAC_PROTOCOL, 
                                         HDRSCMFMAC_ATTRIB_LOAD_INFO_SUPPORTED,
                                         &hdrfmac.cfg->load_info_supported,
                                         fmac_sub_type);
      }

      break;

    case HDRIND_SCM_FMAC_AT_GAUP_REJECTED:

      HDR_MSG_PROT( MSG_LEGACY_HIGH,
                    "LoadInformationSupported update req rejected by AN" );

      hdrscmfmac_set_load_info_attrib_rejected(TRUE);

      break;

    case HDRIND_SCM_FMAC_AT_GAUP_FAILED:

      HDR_MSG_PROT( MSG_LEGACY_ERROR,
                    "Update LoadInformationSupported failed" );

      break;

    case HDRIND_OVHD_NLB_CACHE_UPDATED:
      hdrfmac_process_ovhd_nlb_cache_updated(
        &ind_data_ptr->nlb_serving_sector);

      break;

    case HDRIND_MC_QOS_FLOW_STATUS_CHANGED:
      hdrfmac_process_qos_flow_status_changed();
      break;

#endif /* FEATURE_HDR_REVC */

    default:
      ERR("Invalid indication: %d", ind_name, 0, 0);
  }

} /* hdrfmac_process_ind */


 /* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_IND_CB                                              EXTERNAL

DESCRIPTION
  This function processes incoming indications for this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  ind_name     - Name of the indication from hdrhai.h
  ind_data_ptr - Pointer to any indication-specific data
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_ind_cb
(
  hdrind_ind_name_enum_type ind_name,
  void *ind_data
)
{
  
  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdrfmac_ind_cb %d", ind_name );

  if (rex_self () == HDRFMAC_TASK_PTR)
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdrfmac_process_ind(ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    hdrrx_queue_ind (HDRHAI_FTC_MAC_PROTOCOL, ind_name, ind_data);
  }  

} /* hdrfmac_ind_cb */


 /* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_DRC_SUPERVISION_FAILURE                           EXTERNAL

DESCRIPTION
  The function is used to tell the FTCMAC that DRC supervision failure
  has occured, since the FTCMAC does not actually monitor this.
  
DEPENDENCIES
  None.

PARAMETERS
  None.  
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_drc_supervision_failure( void )
{
  /* Increment the number of DRC supervision failures for 
   * logging purposes. 
   */
  hdrplog_inc_conn_term_stats_counter(
    HDRPLOG_DRC_SUPERVISION_FAILURE_COUNT, 1 );

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "DRC Supervision Failure!" );

  hdrind_give_ind( HDRIND_FTCMAC_SUPERVISION_FAILED, NULL );
  hdrfmac_deactivate(HDRHAI_FTC_MAC_PROTOCOL);

} /* hdrfmac_drc_supervision_failure */


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_MC_ACK_MODE                                 EXTERNAL

DESCRIPTION
  This function transitions the Rx packet processing into one of several
  ack test modes defined by the hdrfmac_ack_mode_enum_type enumeration.

DEPENDENCIES
  None

PARAMETERS
  rl_carrier_id - Reverse link carrier id.
  ack_mode - The mode to enable.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_mc_ack_mode
( 
  uint8 rl_carrier_id,
  hdrfmac_ack_mode_enum_type ack_mode
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Override setting if override is enabled */
  if (hdrfmac.early_term_override == TRUE)
  {
    ack_mode = IS856_NO_ACK;
  }

  /* Remember ack mode for when we open a traffic channel */
  hdrfmac.ack_mode = ack_mode;

  /* If FMAC is active, go ahead and set the ACK mode... */
  if (hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL)) 
  {
    /* Enable ACK mode */
    hdrfmac_enable_ack_mode( rl_carrier_id, ack_mode );

  } /* If FMAC active */

} /* HDRFMAC_SET_ACK_MODE */

/* EJECT */  
/*===========================================================================

FUNCTION HDRFMAC_SET_ACK_MODE                                       EXTERNAL

DESCRIPTION
  This function transitions the Rx packet processing into one of several
  ack test modes defined by the hdrfmac_ack_mode_enum_type enumeration.

DEPENDENCIES
  None

PARAMETERS
  ack_mode - The mode to enable.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_ack_mode
( 
  hdrfmac_ack_mode_enum_type ack_mode
)
{

  hdrfmac_set_mc_ack_mode (0, ack_mode);

} /* hdrfmac_set_ack_mode */

 /* EJECT */


 /* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_IS_FLOW_CTRL_ENABLED()

DESCRIPTION
  Returns TRUE/FALSE depending on whether forward link flow control is 
  currently enabled.
  
DEPENDENCIES
  NONE

PARAMETERS
  None.

RETURN VALUE
  TRUE - the HDR forward link is currently being flow controled.
  FALSE - normal operation.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrfmac_is_flow_ctrl_enabled()
{ 
  return( (hdrfmac.drc_req_mask != 0) ? TRUE : FALSE );

} /* hdrfmac_is_flow_ctrl_enabled */

#if defined (FEATURE_HDR_IS890) || defined ( FEATURE_HDR_IS890A)

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_MC_DRC_COVER                                 EXTERNAL

DESCRIPTION
  This interface is used by FTAP to set AT's transmitted DRC cover (including
  NULL COVER). When a non-NULL cover is sent, the Test APP switches to 
  DRCCoverFixedMode. This function can also be used to switch out of 
  DRCCoverFixedMode operation by setting the drc_cover to 
  HDRFMAC_HANDOFF_CTRL_TO_DSP.

DEPENDENCIES
  None

PARAMETERS
  drc_cover - Could be set to one of the following values.
              1. HDRFMAC_DRC_NULL_COVER - Sets NULL cover
              2. HDRFMAC_HANDOFF_CTRL_TO_DSP - Switches out of 
                 DRCCoverFixedMode operation. DSP assumes control of choosing
                 the DRC cover. 
              3. Any other valid DRC cover values - would be mapped to 
                 asp_index and the uP assumes control and fixes the drc cover
                 to this mapped asp_index. 
  rl_carrier_id - Reverse link Carrier ID

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_mc_drc_cover
( 
  uint8 rl_carrier_id,
  uint8 drc_cover 
)
{
  uint8 asp_index;
    /* Active set pilot index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  switch ( drc_cover )
  {
    case HDRFMAC_DRC_NULL_COVER:

      /* Set DRC cover to NULL cover */
      hdrfmac_enable_flow_control( HDRFMAC_DRC_REQ_NULL_COVER );
      break;

    case HDRFMAC_HANDOFF_CTRL_TO_DSP:

      /* Disable flow control requested by NULL cover entity */
      hdrfmac_disable_flow_control( HDRFMAC_DRC_REQ_NULL_COVER );

      /* Let the DSP decide where to point the DRC to */
      hdrfmac_handoff_override( rl_carrier_id, FALSE, 0);

      break;

    default:

      /* Map the DRC cover to an Active set pilot index */
      asp_index = hdrsrch_get_asp_index( rl_carrier_id, drc_cover );

      /* If the Macro returns a ASP index of 0 then the DRC cover is 
       * not present in the active set. */
      if ( asp_index == HDRSRCH_INVALID_ASP_INDEX )
      {
        HDR_MSG_PROT( MSG_LEGACY_ERROR, 
          "Error: DRC cover not found in Active set ");
        break;
      }

      /* Disable flow control requested by NULL cover entity */
      hdrfmac_disable_flow_control( HDRFMAC_DRC_REQ_NULL_COVER );
  
      /* Override DSP pointing */
      hdrfmac_handoff_override( rl_carrier_id, TRUE, asp_index);

  }

} /* hdrfmac_set_mc_drc_cover */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_DRC_COVER                                       EXTERNAL

DESCRIPTION
  This interface is used by FTAP to set AT's transmitted DRC cover (including
  NULL COVER). When a non-NULL cover is sent, the Test APP switches to 
  DRCCoverFixedMode. This function can also be used to switch out of 
  DRCCoverFixedMode operation by setting the drc_cover to 
  HDRFMAC_HANDOFF_CTRL_TO_DSP.

DEPENDENCIES
  None

PARAMETERS
  drc_cover - Could be set to one of the following values.
              1. HDRFMAC_DRC_NULL_COVER - Sets NULL cover
              2. HDRFMAC_HANDOFF_CTRL_TO_DSP - Switches out of 
                 DRCCoverFixedMode operation. DSP assumes control of choosing
                 the DRC cover. 
              3. Any other valid DRC cover values - would be mapped to 
                 asp_index and the uP assumes control and fixes the drc cover
                 to this mapped asp_index. 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_drc_cover( uint8 drc_cover )
{

  hdrfmac_set_mc_drc_cover(0, drc_cover);

} /* hdrfmac_set_drc_cover */


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_MC_DRC_VALUE                                EXTERNAL

DESCRIPTION
  This interface is used to set AT's transmitted DRC value. The given DRC is
  set if it is valid for the current negotiated FMAC subtype and ignored
  otherwise.

DEPENDENCIES
  None

PARAMETERS
  rl_carrier_id - Reverse link Carrier ID.
  drc - DRC value to set.

RETURN VALUE
  TRUE If the given DRC is valid for current subtype.
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrfmac_set_mc_drc_value
( 
  uint8 rl_carrier_id,
  uint8 drc 
)
{

  uint8 max_drc = HDRHAI_MAX_REL_0_DRC;
    /* Maximum DRC set to Release-0 Max DRC */

  boolean is_drc_valid = TRUE;
    /* Is the given DRC valid for current subtype */

  uint16 subtype = hdrscp_get_current_subtype( HDRHAI_FTC_MAC_PROTOCOL );
    /* FTCMAC subtype */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

#ifdef FEATURE_HDR_REVA_L1 
  /* If the negotiated subtype is enhanced, reset the max DRC to Rev-A Max */
  if ( ( subtype == HDRSCP_ENH_FMAC ) 
#ifdef FEATURE_HDR_REVB
       || ( subtype == HDRSCP_MC_FMAC ) 
#endif /* FEATURE_HDR_REVB */
     )
  {
#ifdef FEATURE_HDR_REVB_ENH_RATES
    max_drc = HDRHAI_MAX_REL_B_DRC;
#else
    max_drc = HDRHAI_MAX_REL_A_DRC;
#endif /* FEATURE_HDR_REVB_ENH_RATES */
  }
#endif /* FEATURE_HDR_REVA_L1 */
  
  /* Check if the given DRC is within valid range */ 
  if ((drc == HDRFMAC_VAR_RATE_DRC) || (drc <= max_drc))
  { 
    /* Set the DRC value */
    hdrsrch_at_set_drc(rl_carrier_id, drc);

  }
  else
  {
    is_drc_valid = FALSE;

    HDR_MSG_PROT_1( MSG_LEGACY_ERROR, "Invalid DRC: %d", drc );
  }

  return is_drc_valid;

} /* hdrfmac_set_mc_drc_value */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_DRC_VALUE                                       EXTERNAL

DESCRIPTION
  This interface is used to set AT's transmitted DRC value. The given DRC is
  set if it is valid for the current negotiated FMAC subtype and ignored
  otherwise.

DEPENDENCIES
  None

PARAMETERS
  drc - DRC value to set.

RETURN VALUE
  TRUE If the given DRC is valid for current subtype.
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrfmac_set_drc_value( uint8 drc )
{

  boolean status = TRUE;

#ifdef FEATURE_HDR_REVB
  uint8 demod_idx = 0;
    /* Demod Index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrfmac.current_subtype == HDRSCP_MC_FMAC )
  {
    for ( demod_idx = 0;
          ((status) && (demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT)); 
          ++demod_idx )
    {
      status = hdrfmac_set_mc_drc_value (demod_idx, drc);
    }
  }
  else
#endif /* FEATURE_HDR_REVB */
  {
    status = hdrfmac_set_mc_drc_value ( HDRMDSP_DEMOD_CARRIER_0, drc );
  }
 
  return status;

} /* hdrfmac_set_drc_value */

#endif /* (FEATURE_HDR_IS890) || ( FEATURE_HDR_IS890A) */


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_TRAFFIC_MAX_DRC                                 EXTERNAL

DESCRIPTION
  This function is used by RMAC to ramp up the Max allowed DRC to 12 when 
  FMAC subtype is Default and 14 when FMAC subtype is enhanced, after 
  reception of RTC ACK.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_traffic_max_drc( void )
{
  hdrfw_msg_u fw_cmd;
    /* Firmware command message to program max DRC */

  errno_enum_type estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {

    memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_MAX_DRC_CFG_CMD );
      /* Initialize the message header */

#ifdef FEATURE_HDR_REVA_L1
    /* If the negotiated FMAC subtype is enhanced */
    if (( hdrfmac.current_subtype == HDRSCP_ENH_FMAC )
#ifdef FEATURE_HDR_REVB
        ||
        ( hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
       )
    {

#ifdef FEATURE_HDR_REVB
      fw_cmd.max_drc_cfg.maxDRC = ( hdrfmac_limit_max_drc == 0 ) ? HDRHAI_MAX_REL_B_DRC : hdrfmac_max_drc;
      /* The max allowed DRC is 27. The max DRC requested by AT is still gated 
         by the AuxDRCCover, and MaxOptionalDataRate which needs to be set 
         appropriately to request Rev-B Rates */
#else
      fw_cmd.max_drc_cfg.maxDRC = ( hdrfmac_limit_max_drc == 0 ) ? HDRHAI_MAX_REL_A_DRC : hdrfmac_max_drc;
      /* The max allowed DRC is 14 */
#endif /* FEATURE_HDR_REVB */

    }
    else  
#endif /* FEATURE_HDR_REVA_L1 */

    /* else, the negotiated subtype is default */
    {

      fw_cmd.max_drc_cfg.maxDRC = ( hdrfmac_limit_max_drc == 0 ) ? HDRHAI_MAX_REL_0_DRC : hdrfmac_max_drc;
      /* The max allowed DRC is 12 */
  }

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );

  } /* hdrfmac_is_active */

} /* hdrfmac_set_traffic_max_drc */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_UPDATE_HANDOFF_REGISTERS

DESCRIPTION
  This function sets MaxCredit (and SofterHandoffCreditThresh) only if the 
  FMAC subtype is default. This function sets the Max credit based on soft 
  handoff delay if the ASET has sectors from more than one cell or sets Max 
  credit based on Softer Handoff delay if the ASET has sectors from exactly 
  one cell. For the latter case, it also adjusts softer HO credit threshold 
  for MPS testing. 

  Note: If the current subtype of FMAC is Enhanced, this function does nothing.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_update_handoff_registers( void )
{
  uint8         drc_length;
    /* DRC length parameter */

  uint8         drc_request;
    /* DRC decision by  MDSP */

  uint16        softer_ho_credit_thresh;
    /* Softer Handoff Credit Threshold */

  boolean aset_has_more_than_one_cell;
    /* Active set has sectors from more than one cell */

  hdrfw_msg_u fw_cmd;
    /* Firmware command message to setup handoff registers */

  errno_enum_type estatus;
    /* Error status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If current FMAC subtype is default */
  if (hdrfmac.current_subtype == HDRSCP_DEFAULT_FMAC)
  {
    memset ( &fw_cmd, 0, sizeof (hdrfw_msg_u) );

    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_FMAC_HANDOFF_CFG_CMD );

    aset_has_more_than_one_cell = 
      hdrsrch_aset_has_more_than_one_cell(HDRMDSP_DEMOD_CARRIER_0);

    if (aset_has_more_than_one_cell)
    {
      /*-------------------------------------------------------------------
                ASET HAS SECTORS FROM MORE THAN ONE CELL
       -------------------------------------------------------------------*/

      /* Program Max credit using soft handoff delay */
      fw_cmd.fmac_handoff_cfg.maxCredit = 
        ( ( hdrfmac.cfg->handoff_delays.soft_handoff_delay << 3 ) + 32 );
    }
    else
    {
      /*--------------------------------------------------------------------
                    ASET HAS SECTORS FROM EXACTLY ONE CELL 
       --------------------------------------------------------------------*/

      /* Program Max credit using softer handoff delay */
      fw_cmd.fmac_handoff_cfg.maxCredit = 
        ( ( hdrfmac.cfg->handoff_delays.softer_handoff_delay << 3 ) + 32 );

      /*--------------------------------------------------------------------
         SPECIAL CASE ADJUSTMENT OF SOFTER-HANDOFF-CREDIT-THRESH FOR MPS
       -------------------------------------------------------------------*/

      drc_length =
        hdrfmac.tcam_params.sub_aset[ HDRMDSP_DEMOD_CARRIER_0 ].drc_length;
        /* Get the DRC length */

      /** @todo figure out the source of this field */
      drc_request = HDRDRC_GET_DRC_RL_MC(HDRMDSP_DEMOD_CARRIER_0);
        /* Get DRC requested by MDSP */

      if ( ( drc_length   == 1 ) &&
           ( drc_request  >= 3 )  )
      {
        softer_ho_credit_thresh = 
          MIN ( hdrfmac.cfg->handoff_delays.soft_handoff_delay, 
                MAX ( hdrfmac.cfg->handoff_delays.softer_handoff_delay, 16 ) );
      }
      else
      {
        softer_ho_credit_thresh = hdrfmac.cfg->handoff_delays.soft_handoff_delay;
      }

      fw_cmd.fmac_handoff_cfg.softerHandoffCreditThresh = 
        softer_ho_credit_thresh;
      /* Set softer handoff credit threshold */

    } /* ASET has sectors from exactly one cell */

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );

  } /* If default FMAC */

} /* hdrfmac_update_handoff_registers */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_CONFIGURE

DESCRIPTION
  This function is called by SCP to start AT-initiated configuration.

DEPENDENCIES
  None

PARAMETERS
  sender - The calling protocol.

RETURN VALUE
  TRUE if the configuration is required; FALSE if configuration is not
  needed.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrfmac_configure
(
  hdrhai_protocol_name_enum_type sender
)
{
  boolean ret_val = FALSE;
    /* Return Value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SYS_ARG_NOT_USED( sender );
    /* mark the unused variable to avoid the lint error */

  if ( hdrscmfmac_config_is_needed() )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "FMAC Config is required" );

    ret_val = hdrscmfmac_configure();
  }

  return ret_val;

} /* hdrfmac_configure */


#ifdef FEATURE_HDR_REVB

/* <EJECT> */
/*===========================================================================

FUNCTION HDRFMAC_RTC_RESTART_TIMER_EXPIRED

DESCRIPTION
  This function is called by RMAC4DRV when the ReverseTrafficChannel Restart 
  timer expires. This function gives an indication to be processed by RMAC4, 
  which drops the carrier and sends a carrier dropped message to AN.

DEPENDENCIES
  None

PARAMETERS
  rl_carrier_id - ID of the RL carrier whose RTC Restart timer expired.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_rtc_restart_timer_expired ( unsigned long rl_carrier_id )
{

  hdrind_ind_data_union_type ind_data;
    /* Indication data */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store the carrier ID as indication data */
  ind_data.carrier_id = (uint8)rl_carrier_id;

  /* Give Restart timer expired indication */
  hdrind_give_ind(HDRIND_RTC_RESTART_TIMER_EXPIRED, (void*) &ind_data);

} /* hdrfmac_rtc_restart_timer_expired */


/*===========================================================================

FUNCTION HDRFMAC_SERVING_SECTOR_LOCKED_ACROSS_SUB_ASETS         EXTERNAL

DESCRIPTION
  Can AT select a different serving sector on each of the Active sets?
  
DEPENDENCIES
  None.

PARAMETERS
  locked - Pointer to the boolean where the result is stored.
           Set to TRUE for locked Mode and FALSE otherwise. 

RETURN VALUE
  E_SUCCESS     - If the session is open and current FMAC subtype is 
                  Multi-carrier FTCMAC (Subtype 2).
  E_NOT_ALLOWED - Otherwise.

SIDE EFFECTS
  None.
  
===========================================================================*/
hdrerrno_enum_type hdrfmac_serving_sector_locked_across_sub_asets
( 
  boolean *locked
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  return hdrfmacutil_serving_sector_locked_across_sub_asets( locked );

} /* hdrfmac_serving_sector_locked_across_sub_asets */

#endif /* FEATURE_HDR_REVB */


 /* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_ENTER_DRC_SUPERVISION                              EXTERNAL

DESCRIPTION
  The function is used to the following when entering DRC supervision:
  - Gives enter DRC supervision indication to RTCMAC.
  - Starts DRC supervision restart timer for time T_FTCMPRestartTx 
    (= 12 control channel cycles = 5.12 seconds).
  
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_mask - Physical channel ID mask
                  Set 'n'th bit to indicate that channel 'n' is in 
                  DRC supervision

RETURN VALUE
  None.
  
SIDE EFFECTS 
  None.
===========================================================================*/
void hdrfmac_enter_drc_supervision
(
  uint16 phy_chan_mask
)
{
  uint8 ch;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  for (ch = 0; ch < HDRRMAC_MAX_NUM_RL_CARRIERS; ch++)
{
    if ((phy_chan_mask & (1<<ch)) != 0)
  {
      /* Start reverse channel DRC supervision restart timer */
      (void) rex_set_timer( &hdrfmac.drc_sup_restart_timer[ch],
        HDRFMAC_DRC_SUP_RESTART_TIMER_MSECS );
  }

  } /* for all RL carriers */

  /* Notify RL carrier's DRC supervision status to RMAC */
  hdrrmac_enter_drc_supervision( 
    HDRHAI_FTC_MAC_PROTOCOL,
    phy_chan_mask );

} /* hdrfmac_enter_drc_supervision */


/* EJECT */
/*===========================================================================
FUNCTION HDRFMAC_EXIT_DRC_SUPERVISION                               EXTERNAL

DESCRIPTION
  The function is used to the following when exiting DRC supervision:
  - Resets DRC supervision restart timer.
  - Gives exit DRC supervision indication to RTCMAC.
 
DEPENDENCIES
  None.

PARAMETERS
  phy_chan_mask - Physical channel ID mask
                  Set 'n'th bit to indicate that channel 'n' is now out 
                  of DRC supervision

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrfmac_exit_drc_supervision
(
  uint16 phy_chan_mask
)
{
  uint8 ch;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 
  
  for (ch = 0; ch < HDRRMAC_MAX_NUM_RL_CARRIERS; ch++)
  {
    if ((phy_chan_mask & (1<<ch)) != 0)
    {      
      /* Stop reverse channel DRC supervision restart timer */
      (void) rex_clr_timer( &hdrfmac.drc_sup_restart_timer[ch] );
  }

  } /* for all RL carriers */

  /* Notify RL carrier's DRC supervision exit status to RMAC */
  hdrrmac_exit_drc_supervision( 
    HDRHAI_FTC_MAC_PROTOCOL,
    phy_chan_mask );

} /* hdrfmac_exit_drc_supervision */

/*===========================================================================
FUNCTION HDRFMAC_GET_DRC_LENGTH                                     EXTERNAL

DESCRIPTION
  Returns the drc_length from the indicated carrier.
  Returns 0 if call is not active or carrier is not valid.
 
DEPENDENCIES
  None.

PARAMETERS
  carrier - The carrier id from TCA

RETURN VALUE
  The drc_length for the carrier.

SIDE EFFECTS
  None.
===========================================================================*/
int hdrfmac_get_drc_length
( 
  uint8 carrier 
    /* Carrier id from TCA */
)
{
  int drc_length = 0;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {
    if (carrier < hdrfmac.tcam_params.num_carriers)
    {
      drc_length = hdrfmac_drc_length_table[hdrfmac.tcam_params.sub_aset[ carrier ].drc_length];
    }
  }
  return drc_length;
}

#ifdef FEATURE_HDR_FLEX_CONN


/*===========================================================================
FUNCTION HDRFMAC_GET_DSC_LENGTH_IN_SLOT                               EXTERNAL

DESCRIPTION
  Get dsc length in slot
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  The dsc_length in slots.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrfmac_get_dsc_length_in_slot( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  return ( hdrfmac.cfg->dsc_length << 3 );
    /* Multiply 8 (from standard) to get slot number */

} /*  hdrfmac_get_dsc_length_in_slot */
/*===========================================================================
FUNCTION HDRFMAC_FLEXCONN_CHECK_DRC_LOCK_LEN                               EXTERNAL

DESCRIPTION
  Check if drc lock length <= dsc length (both in slots)
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  Whether drc_lock_len <= dsc_len

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrfmac_flexconn_check_drc_lock_len( void )
{
  uint16 drc_lock_len_in_slots, dsc_len_in_slots;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  drc_lock_len_in_slots = hdrfmac_drc_lock_length_table[hdrfmac.cfg->drc_lock_length];
    /* get drc_lock_length from default FTC MAC encoding table*/

  if ((hdrfmac.current_subtype == HDRSCP_ENH_FMAC)
#ifdef FEATURE_HDR_REVB
      ||
      (hdrfmac.current_subtype == HDRSCP_MC_FMAC)
#endif /* FEATURE_HDR_REVB */
      )
  {
    /* DRCLockLength Encoding is (from standard) :
         0 -- 4,1 -- 8, 2 -- 16, 3 -- 32 for default FTC MAC
         0 -- 8,1 -- 16,2 -- 32, 3 -- 64 for enhanced FTC MAC or MC FTC MAC
    
    */
    drc_lock_len_in_slots *= 2;
      /* need to multiply it by 2 for enhanced FTC MAC or MC FTC MAC */
  }
  drc_lock_len_in_slots *= 4;
    /* Since DRCLock bit is transmitted once every 4 slots, multiply by 4 here */

  dsc_len_in_slots = hdrfmac.cfg->dsc_length << 3;
    /* multiply 8 (from standard) to get dsc length in slot */

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "FLEXCONN: DRCLockLength=%d DSCLength=%d",
                drc_lock_len_in_slots, dsc_len_in_slots );

  return ( drc_lock_len_in_slots <= dsc_len_in_slots );
} /* hdrfmac_flexconn_check_drc_lock_len */

#endif /* FEATURE_HDR_FLEX_CONN */

#if defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
/*===========================================================================

FUNCTION HDRFMAC_FL_ACTIVITY_MONITOR

DESCRIPTION
  Notify the client that FL is active again and reset the timer to track FL
  activities
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_fl_activity_monitor( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  hdrfmac.last_fl_packet_hstr = HDRHSTR_GET_COUNT();
    /* Store the hstr of last received FL packet with good CRC. Only FL packet 
       with good CRC will be forwarded to FMAC.
    */
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  if ( hdrfmac.fl_active_cb_list.ard_fl_active_cb != NULL )
  {
    hdrfmac.fl_active_cb_list.ard_fl_active_cb( TRUE );
      /* Tell ARD client that there is FL packet coming.*/
    hdrfmac.fl_active_cb_list.ard_fl_active_cb = NULL;
      /* One indication to ARD to notify that FL is active is enough. That's why
         we reset it to NULL after one call. Sometimes FMAC get a burst of
         FL packets, we don't want send multiple indications to ard. It might make
         srch internal command queue overflow because too many commands coming almost
         at the same time.
      */
  }
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#ifdef FEATURE_HDR_FLEX_CONN
  if ( hdrfmac.fl_active_cb_list.flexconn_fl_active_cb != NULL )
  { /* FLEXCONN client has registered callback */

    if ( hdrfmac.fl_active_cb_list.flexconn_fl_inactive_is_notified )
    {
      /* FLEXCONN client has been notified with FL inactive in last time. Now a FL
         packet is received. FMAC should notify FLEXCONN client that FL is active again.
         And FMAC should start the timer to monitor FL activity. If there is no data
         coming in the timeout period, FMAC should notifiy the client with FL inactive
      */
      hdrfmac.fl_active_cb_list.flexconn_fl_active_cb( TRUE );
        /* Tell FLEXCONN client that FL is active again */
      hdrfmac.fl_active_cb_list.flexconn_fl_inactive_is_notified = FALSE;
        /* Need to notify client that FL is inactive when the timer expires */
    }
    rex_set_timer( &hdrfmac.fl_activity_flexconn_timer, HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT );
      /* start the FL activity timer for FLEXCONN  since a new FL packet comes */
  }
#endif /* FEATURE_HDR_FLEX_CONN */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX

  if ( hdrfmac.fl_active_cb_list.dtx_fl_active_cb != NULL )
  { /* DTX client has registered callback */

    if ( hdrfmac.fl_active_cb_list.dtx_fl_inactive_is_notified )
    {
      /* DTX  client has been notified with FL inactive in last time. Now a FL
         packet is received. FMAC should notify DTX client that FL is active again.
         And FMAC should start the timer to monitor FL activity. If there is no data
         coming in the timeout period, FMAC should notifiy the client with FL inactive
      */
       
      hdrfmac.fl_active_cb_list.dtx_fl_active_cb( TRUE );
        /* Tell client that FL is active again */
      hdrfmac.fl_active_cb_list.dtx_fl_inactive_is_notified = FALSE;
        /* Need to notify client that FL is inactive when the timer expires */
    }

    rex_set_timer( &hdrfmac.fl_activity_dtx_timer, HDRFMAC_DTX_FL_INACTIVE_TIMEOUT );
       /* start the FL activity timer for DTX since a new FL packet coming */
  }
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

} /* hdrfmac_fl_activity_monitor */

/*===========================================================================

FUNCTION HDRFMAC_SET_FL_ACTIVE_CB

DESCRIPTION
  Set FMAC call back 
 
DEPENDENCIES
  None.

PARAMETERS
  fl_active_cb_ptr - call back once FL is active.
  client_id

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrfmac_set_fl_active_cb
( 
  hdrfmac_fl_active_cb_type          fl_active_cb_ptr,
    /* FL active call back function ptr */
  hdrfmac_fl_active_cb_client_type   client_id,
    /* Client id */
  uint32                             payload
    /* payload indicates inact_ms if cb_ptr is null, or
       payload indicates whether to resume timer if cb_ptr is not null
    */
)
{
  hdrfmac_cmd_union_type cmd_buf;              /* Temporary command buffer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRFMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRFMAC_SET_FL_ACTIVE_CB_CMD;
    cmd_buf.hdr.sender = HDRHAI_FTC_MAC_PROTOCOL;
    cmd_buf.set_active_cb.cb = fl_active_cb_ptr;
    cmd_buf.set_active_cb.client_id = client_id;
    cmd_buf.set_active_cb.payload = payload;
    hdrrx_queue_cmd( HDRHAI_FTC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
      /* Save the registered cb function */
    switch ( client_id )
    {
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
      case HDRFMAC_FL_ACTIVE_CB_ARD :
        hdrfmac.fl_active_cb_list.ard_fl_active_cb = fl_active_cb_ptr;
        break;
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#ifdef FEATURE_HDR_FLEX_CONN
      case HDRFMAC_FL_ACTIVE_CB_FLEXCONN :
        hdrfmac.fl_active_cb_list.flexconn_fl_active_cb = fl_active_cb_ptr;
          /* store callback function pointer */

        if ( fl_active_cb_ptr != NULL )
        {
          /* FLEXCONN client wants to register callback, to start monitoring FL
             traffic
           */
          if ( payload == 0 )
          {
            /* payload == 0 means we don't want to resume FL activity timer */

          rex_set_timer( &hdrfmac.fl_activity_flexconn_timer, 
                         HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT );
          }
          else
          {
            /* payload !=0 means we want to resume FL activity timer */
            rex_set_timer( &hdrfmac.fl_activity_flexconn_timer, 
                           HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT - hdrfmac.fl_inactive_ms );
            /* start the FL activity timer for FLEXCONN */
             HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "FLEXCONN: resume fl activity timer %d hstr %d",
                             HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT - hdrfmac.fl_inactive_ms,
                             HDRHSTR_GET_COUNT() );
  }

          hdrfmac.fl_active_cb_list.flexconn_fl_inactive_is_notified = FALSE;
            /* Have not notify the client about FL inactive */
        }
        else
        {
          /* FLEXCONN client wants to deregister callback */
          if ( payload > HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT )
          {
            /* payload is FL inactive time in ms */
            HDR_MSG_PROT_1( MSG_LEGACY_ERROR,
                            "FLEXCONN: fl_inact_ms=%d is too big when suspending FL activity timer",
                            payload );
            payload = 0;
          }
          hdrfmac.fl_inactive_ms = payload;
            /* Store the current FL inactive time. We will resume the timer later
               if flexconn client requests it
            */

          /* FLEXCONN client wants to deregister callback */
          rex_clr_timer( &hdrfmac.fl_activity_flexconn_timer );
            /* stop FL activity timer for FLEXCONN since we don't need to
               monitor FL
             */
        }
        break;
#endif /* FEATURE_HDR_FLEX_CONN */


#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
      case HDRFMAC_FL_ACTIVE_CB_DEVICE_DTX :
        hdrfmac.fl_active_cb_list.dtx_fl_active_cb = fl_active_cb_ptr;
          /* store callback function pointer */

        /* For DTX we always reset FL activity timer. So we don't check payload here */
        if ( fl_active_cb_ptr != NULL )
        {
          /* Start monitoring FL for DTX
          */
          rex_set_timer( &hdrfmac.fl_activity_dtx_timer, 
                         HDRFMAC_DTX_FL_INACTIVE_TIMEOUT );
            /* start the FL activity timer for DTX */

          hdrfmac.fl_active_cb_list.dtx_fl_inactive_is_notified = FALSE;
           /* Need to notify client that FL is inactive when the timer expires */
        }
        else
        {

          /* DTX client wants to deregister callback */
          rex_clr_timer( &hdrfmac.fl_activity_dtx_timer );
            /* stop FL activity timer for DTX since we don't need to monitor FL */
        }
        break;
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */

      default :
        HDR_MSG_PROT_1( MSG_LEGACY_ERROR, "reg,unknown fl_active_cb client %d", client_id);
        break;

    }
  }
} /* hdrfmac_set_fl_active_cb */


/*===========================================================================

FUNCTION HDRFMACREG_FL_ACTIVE_CB

DESCRIPTION
  Register FMAC call back once FL is active.
  
DEPENDENCIES
  None.

PARAMETERS
  fl_active_cb_ptr - call back once FL is active.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrfmac_reg_fl_active_cb
( 
  hdrfmac_fl_active_cb_type          fl_active_cb_ptr,
    /* FL active call back function ptr */
  hdrfmac_fl_active_cb_client_type   client_id,
    /* Client id */
  boolean                            resume_fl_timer
    /* Whether to resume FL activity timer when register callback */
)
{

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH,
                  "FLEXCONN/DTX/ARD: register fl_activity_cb, client=%d, resume_timer %d ",
                  client_id, resume_fl_timer );

  if ( fl_active_cb_ptr != NULL )
  {
    if ( resume_fl_timer == FALSE )
    {
      hdrfmac_set_fl_active_cb( fl_active_cb_ptr, client_id, 0 );
    }
    else
    {
      hdrfmac_set_fl_active_cb( fl_active_cb_ptr, client_id, 1 );
    }
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, "FLEXCONN/DTX/ARD: register fl_activity_cb error,null cb");
  }
} /* hdrfmac_reg_fl_active_cb */

/*===========================================================================

FUNCTION HDRFMAC_DEREG_FL_ACTIVE_CB

DESCRIPTION
  De-register FMAC call back once FL is active.
  
DEPENDENCIES
  None.

PARAMETERS
  fl_active_cb_ptr - call back once FL is active.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_dereg_fl_active_cb
( 
  hdrfmac_fl_active_cb_client_type   client_id,
    /* Client id */
  boolean                            suspend_fl_timer
    /* whether to suspend FL activity timer when dereg the callback */
)
{

  uint16 cur_hstr;
  uint16 last_fl_act_hstr;
  uint16 inact_hstr;
  uint32 inact_ms = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
                  "FLEXCONN/DTX/ARD: deregister fl_activity_cb, client=%d, suspend_timer %d",
                  client_id, suspend_fl_timer );

  if ( suspend_fl_timer )
  {
    cur_hstr = HDRHSTR_GET_COUNT(); 
    last_fl_act_hstr = hdrfmac.last_fl_packet_hstr;
      /* Get the hstr of last received FL packet with good CRC. We don't want to 
         disable flexconn or dtx mode if a FL packet with bad CRC is reveived. Disabling
         them won't help with FL packet receiving.
      */

    inact_hstr = ( cur_hstr - last_fl_act_hstr ) & 0xFFFF;
    inact_ms =  inact_hstr * 5 / 6;
      /* convert half slots to ms */

    HDR_MSG_PROT_4( MSG_LEGACY_HIGH, "FlexConn: suspend timer,inact time %d-%d=>%d half slot=%d ms",
                    cur_hstr, last_fl_act_hstr,
                    inact_hstr, inact_ms );
#ifdef FEATURE_HDR_FLEX_CONN
    if ( ( inact_ms > HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT ) &&
         ( client_id == HDRFMAC_FL_ACTIVE_CB_FLEXCONN ) )
    {
      inact_ms = HDRFMAC_FLEXCONN_FL_INACTIVE_TIMEOUT;
    }
#endif /* FEATURE_HDR_FLEX_CONN */
  }

  hdrfmac_set_fl_active_cb( NULL, client_id, inact_ms );
} /* hdrfmac_dereg_fl_active_cb */

#endif /* defined(FEATURE_HDR_FL_DATA_ADAPTIVE_RXD)||
          defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)*/

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_INIT

DESCRIPTION 
  This function is called to initialize variables used in calculating the
  FAC gain estimate.  FAC gain estimation is required to detect the FAB
  (Forward Activity Bit) transmitted on the FL using a preamble based
  signal.  FAB detection is done in MDSP, while FAC gain estimation is
  computed in SW.
 
  This function is called at Power up and also every time the histogram used
  to calculate FAC gain is full.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_fac_gain_init( void )
{
  int i;              /* For loop counter */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  for ( i = 0; i < hdrfmac_fac_gain_ctrl.m_num_bins; i++)
    /* Initialize histogram */
  {
    hdrfmac_fac_gain_ctrl.hist_arr[ i ] = 0;
  }

  hdrfmac_fac_gain_ctrl.hist_idx = 0;
  hdrfmac_fac_gain_ctrl.max_idx  = 0;
    /* Initialize histogram indexes */

  hdrfmac_fac_gain_ctrl.total_num_slots = 0;              
    /* Initialize total number of slot elements received from MDSP */
  
} /* hdrfmac_fac_gain_init */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_FIND_MAX_HISTOGRAM                                   

DESCRIPTION 
  This function is called to find the winning histogram bin.
 
DEPENDENCIES
  None

RETURN VALUE 
  Index of the winning histogram bin

SIDE EFFECTS 
  None 

===========================================================================*/

uint16 hdrfmac_fac_gain_find_max_histogram( void )
{
  uint16 i;              /* For loop counter */
  uint16 temp_max_val;   /* Value of winning histogram bin */
  uint16 temp_max_idx;   /* Winning hisgram bin index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  temp_max_val = 
    hdrfmac_fac_gain_ctrl.hist_arr[ hdrfmac_fac_gain_ctrl.i_skip ];
    /* Max value is set to the content of the bin following the first
       few bins which are intentionally skipped to make for cases
       where the serving sector is highly loaded and few idle mode
       samples are collected. */

  temp_max_idx = hdrfmac_fac_gain_ctrl.i_skip;
    /* Array index starts after skipping the first few bins */

  for ( i = hdrfmac_fac_gain_ctrl.i_skip + 1; 
        i < hdrfmac_fac_gain_ctrl.m_num_bins; i++ )
    /* Loop to find the index of the biggest histogram bin */
  {
    if ( temp_max_val < hdrfmac_fac_gain_ctrl.hist_arr[ i ] )
    {
      temp_max_val = hdrfmac_fac_gain_ctrl.hist_arr[ i ];
      temp_max_idx = i;
    }
  }

  return temp_max_idx;
    /* Index of the biggest histogram bin */

} /* hdrfmac_fac_gain_find_max_histogram */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_CALC

DESCRIPTION 
  This function updates the histogram bins.  Once the max number of slot
  elements have been received from MDSP, this function calculates the
  FAC (Forward Activity Channel) gain to be given to MDSP for FAB (Forward
  Activity Bit) detection.
 
  FAB detection and FAC gain estimation are needed for a gRICE (guided
  Robust Interference Cancellation Equalizer) based receiver
  since they determine the level of interference covariance that needs to be
  added before computing the MMSE equalizer coefficients. In general, FAB
  detection needs to be performed for each sector that is included in the
  active set of gRICE. 
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_fac_gain_calc( void )
{
  uint16 i = 0;           /* For loop counter to process slot data from MDSP */
  int64 ratio = 0;        /* Un-normalized delta ratio value in Q20 */
  int64 temp_gain = 0;    /* Temp holding location for gain component */
  hdrfw_msg_u fw_cmd;     /* Firmware commands */
  errno_enum_type estatus;/* Error status */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  if ( hdrfmac_fac_gain_ctrl.total_num_slots < hdrfmac_fac_gain_ctrl.n_num_slots )
    /* Still need to fill the histogram, so run the for loop for the number
       of slot elements received from the MDSP this time */
  {
    for ( i = 0; i < hdrfmac_fac_gain_from_mdsp.num_slots; i++ )
      /* Run for loop until SW has 1024 slot elements */
    {
      if ( ( hdrfmac_fac_gain_from_mdsp.fab_cp[ i ] ) != 0 )
        /* Guard against divide by 0 */
      {
        ratio = ( int64 )
          ( (int64)( HDRFMAC_K0_FP_CONST * (int64)hdrfmac_fac_gain_from_mdsp.fab_metric[ i ] ) / 
            ( hdrfmac_fac_gain_from_mdsp.fab_cp[ i ] ) 
          ) * 1024;
          /* Compute un-normalized version of delta ratio value in Q20
             representation .  This computation is required to select the
             winning histogram bin.  Multiply by 1024 is the same as << 10
             and get around compiler error of shift operations on 64 bit
             values. */
      }
      else
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                      "Error, cannot calculate ratio, CP is 0" );

          /* Exit for loop.  Since this for loop processes 4 slot worth of data:
             - If this is iteration 0, all 4 slot worth of data is dropped 
             - If this is iteration 1, 3 slot worth of data is dropped starting 
               with the bad slot
             - If this is iteration 2, 2 slot worth of data is dropped starting 
               with the bad slot
             - If this is iteration 3, the bad slot data is dropped
             It will require just a few more slot worth of data to calculate the
             FAC gain.  The actual FAC gain calculation done after this for loop
             is not affected as the bad slot data won't be accounted for in the
             histogram bin selection.  The error message is set to FATAL to
             indicate this error as it is not expected. */
        break;

      }

      if ( ratio <= ( int64 ) hdrfmac_fac_gain_ctrl.alpha0_ratio_min )
        /* If ratio is <= minimum value */
      {
        hdrfmac_fac_gain_ctrl.hist_idx = 0;
          /* Histogram lower boundary condition */

        hdrfmac_fac_gain_ctrl.drop_count++;
          /* Increment counter for number of times MDSP input data is rejected
             due to not crossing A0 threshold */
      }
      else
      {
        hdrfmac_fac_gain_ctrl.hist_idx =  ( int )
          ( ratio - hdrfmac_fac_gain_ctrl.beta ) / HDRFMAC_DELTA_HISTOGRAM_STEPSIZE;
          /* Compute index of the winning histogram bin */

        if ( hdrfmac_fac_gain_ctrl.hist_idx >= hdrfmac_fac_gain_ctrl.m_num_bins )
        {
          hdrfmac_fac_gain_ctrl.hist_idx = hdrfmac_fac_gain_ctrl.m_num_bins - 1;
            /* Histogram upper boundary condition */
        }

        hdrfmac_fac_gain_ctrl.total_num_slots++;
          /* Increase the number of slot elements received so far */
      }

      hdrfmac_fac_gain_ctrl.hist_arr[ hdrfmac_fac_gain_ctrl.hist_idx ]++;
        /* Add 1 to the winning histogram bin */

      if ( hdrfmac_fac_gain_ctrl.total_num_slots == 
             hdrfmac_fac_gain_ctrl.n_num_slots )
      {
        break;
          /* Reached the Max number of slots for the histogram, exit
             for loop */
      }

    } /* end for loop */
  }

  if ( hdrfmac_fac_gain_ctrl.total_num_slots >=  
         hdrfmac_fac_gain_ctrl.n_num_slots )
    /* Check if SW has received enough data to calculate FAC Gain */
  {
    hdrfmac_fac_gain_ctrl.max_idx = 
      hdrfmac_fac_gain_find_max_histogram();
      /* Histogram is full, find winning bin to calculate new FAC Gain */

    if ( hdrfmac_fac_gain_ctrl.max_idx != hdrfmac_fac_gain_ctrl.i_skip )
      /* Skipping the first few bins to account for cases where the serving
         sector is higly loaded and few idle mode samples are collected.
         If Max index falls within the skipped bins, FAC Gain remains
         unchanged. */
    {
      temp_gain = ( int64 )
        ( hdrfmac_fac_gain_ctrl.alpha0_ratio_min + 
          ( hdrfmac_fac_gain_ctrl.max_idx * HDRFMAC_DELTA_HISTOGRAM_STEPSIZE ) ) 
            / 1024;
        /* Calculate new FAC gain */

      if ( hdrfmac_fac_gain_ctrl.fac_gain_jumpstart )
      {
        hdrfmac_fac_gain_ctrl.gain = ( int32 ) temp_gain;
          /* Jumpstart FAC gain by overwriting it with the calculated
             gain value */

        hdrfmac_fac_gain_ctrl.fac_gain_jumpstart = FALSE;
          /* Reset jumpstart flag */
      }
      else 
      {
        hdrfmac_fac_gain_ctrl.gain = ( int32 ) (
          ( hdrfmac_fac_gain_ctrl.prev_gain_fac * hdrfmac_fac_gain_ctrl.gain ) + 
          ( hdrfmac_fac_gain_ctrl.curr_gain_fac * temp_gain ) ) / 16;
          /* Filter new gain value */
      }
    }

    HDR_MSG_PROT_4( MSG_LEGACY_HIGH, 
                    "FAC Gain %d, slots %d, winning bin %d, drop count = %d", 
                    hdrfmac_fac_gain_ctrl.gain,
                    hdrfmac_fac_gain_ctrl.total_num_slots,
                    hdrfmac_fac_gain_ctrl.max_idx,
                    hdrfmac_fac_gain_ctrl.drop_count );

    memset( &fw_cmd, 0, sizeof( hdrfw_msg_u ) );

    msgr_init_hdr( &fw_cmd.hdr, MSGR_HDR_FMAC, HDR_FW_GRICE_SET_FAC_GAIN_CMD );
    
    fw_cmd.grice_set_fac_gain.facGain = hdrfmac_fac_gain_ctrl.gain;
      /* Output FAC gain to FW */

    estatus = msgr_send( &fw_cmd.hdr, sizeof( hdrfw_msg_u ) );
      /* Send the FW message */

    ASSERT ( estatus == E_SUCCESS );

    hdrfmac_fac_gain_ctrl.drop_count = 0;
      /* Reset counter for number of times MDSP input data is rejected due to not
         crossing A0 threshold */

    hdrfmac_fac_gain_init();
      /* Reset variables used in FAC gain estimation. */

  }

} /* hdrfmac_fac_gain_calc */


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_READ_MDSP_DATA

DESCRIPTION 
  This function reads the MDSP's FAC gain input data.  FAC gain estimate
  processing gets activated if MDSP has 4 slot worth of FAC gain input data
  for SW to process.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_fac_gain_read_mdsp_data( uint8 read_buf_idx )
{
  uint32 num_elements;          /* Number of slot elements ready to be read */
  uint16 element_idx;              /* For loop counter */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  num_elements = HDRMDSP_GET_FAC_GAIN_NUM_ELEMENTS( read_buf_idx );

  hdrfmac_fac_gain_from_mdsp.num_slots = num_elements;
    /* MDSP data is valid and is ready to be processed */

  for ( element_idx = 0; element_idx < num_elements; element_idx++ )
  {
    hdrfmac_fac_gain_from_mdsp.fab_metric[ element_idx ] = 
      HDRMDSP_GET_FAC_GAIN_FABMETRIC( read_buf_idx, element_idx );
      /* Store FAC gain Metric data from MDSP */

    hdrfmac_fac_gain_from_mdsp.fab_cp[ element_idx ] = 
      HDRMDSP_GET_FAC_GAIN_FABCP( read_buf_idx, element_idx );
      /* Store FAC gain Cp data from MDSP */
  }

} /* hdrfmac_fac_gain_read_mdsp_data */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PROCESS_MDSP_LOGGING_EVENT   

DESCRIPTION 
  This function is the call back from mdsp logging event for FAC gain 
  calculation.
 
  FAB detection and FAC gain estimation are needed for a gRICE (guided
  Robust Interference Cancellation Equalizer) based receiver
  since they determine the level of interference covariance that needs to be
  added before computing the MMSE equalizer coefficients. In general, FAB
  detection needs to be performed for each sector that is included in the
  active set of gRICE. 
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_process_mdsp_logging_event
( 
  hdrlog_mdsp_slot_logging_data_type *sl_params 
)
{
  uint8 buffer_idx = sl_params->rx_log_buff_idx;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrfmac_fac_gain_read_mdsp_data( buffer_idx );

  if ( hdrfmac_fac_gain_from_mdsp.num_slots != 0 ) 
    /* MDSP data is valid and is ready to be processed */
  {
    hdrfmac_fac_gain_calc();
      /* Perform FAC gain estimate calculation */
  }
} /* hdrfmac_process_mdsp_logging_event */

/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_CHECK_AND_SUSPEND

DESCRIPTION 
  This function is called to check if it is necessary to suspend the FAC
  gain slot interrupt from FW.
 
  This function is called when DO is transitioning from traffic to traffic
  suspend state.  FAC calculation is not performed in traffic suspend
  state.  The information provided by the FAC gain slot interrupt is not
  used.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_fac_gain_check_and_suspend( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  hdrlog_register_slot_logging_cb( HDRLOG_FMAC_MDSP_SLOT_LOGGING_CLIENT,
                                   NULL );

  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "Suspend FAC Gain calc if enabled, latest gain=%d to MDSP",
                  hdrfmac_fac_gain_ctrl.gain );

} /* hdrfmac_fac_gain_check_and_suspend */

/*===========================================================================

FUNCTION HDRFMAC_FAC_GAIN_CHECK_AND_RESUME

DESCRIPTION 
  This function is called to check if it is necessary to resume the FAC
  gain slot interrupt from FW.
 
  This function is called when DO is transitioning from traffic suspend state
  to traffic state.  If FAC calculation is enabled, FW provides FAC calculation
  parameters in the FAC gain slot interrupt.  FAC gain calculation only runs
  in traffic state.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrfmac_fac_gain_check_and_resume( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  if ( hdrfmac_fac_gain_ctrl.fac_gain_enable == TRUE )
    /* Allow flexibility to turn off FAC gain estimate calculation via QXDM */
  {
    /*-------------------------------------------------------------------------
      Register mdsp slot logging cb.  This call back is used to read slot
      elements from MDSP to calculate the FAC (Forward Activity Channel)
      gain.  This gain will eventually be given to MDSP for FAB (Forward
      Activity Bit) detection.
    -------------------------------------------------------------------------*/
    hdrlog_register_slot_logging_cb( HDRLOG_FMAC_MDSP_SLOT_LOGGING_CLIENT,
                                     hdrfmac_process_mdsp_logging_event );

    /* Not to reset FAC gain variables till we get the jumpstart FAC gain*/
    if (hdrfmac_fac_gain_ctrl.fac_gain_jumpstart == FALSE)
    {
    hdrfmac_fac_gain_init();
      /* Reset variables used in FAC gain estimation. */
    }
  
    HDR_MSG_PROT_1( 
      MSG_LEGACY_HIGH, "Resume FAC Gain mdsp logging,curr gain=%d to MDSP",
      hdrfmac_fac_gain_ctrl.gain );
  }

} /* hdrfmac_fac_gain_check_and_resume */

/*===========================================================================

FUNCTION HDRFMAC_IS_FAC_GAIN_EST_AVAILABLE

DESCRIPTION 
  This function is called to check whether FAC Gain estimated value is computed
  after Power Up.
 
  This function is called by searcher when DO enters traffic. Searcher checks
  several conditions before enabling D-LNA, and this is one of those checks.
 
DEPENDENCIES
  None

RETURN VALUE 
  TRUE - If FAC gain estimate is available
  FALSE - If FAC gain estimate is NOT available

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrfmac_is_fac_gain_est_available ( void )
{
   return (!hdrfmac_fac_gain_ctrl.fac_gain_jumpstart);
}

#ifdef FEATURE_HDR_REVC
/*===========================================================================

FUNCTION HDRFMAC_IS_LOAD_INFO_MSG_RCVD

DESCRIPTION
  Check whether any loading information message is received from current
  serving sector.
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  TRUE  - A load information message has been received from current
          serving sector in the CONNECTED or IDLE state
  FALSE - None of loading information message has been received.

SIDE EFFECTS
  None.
  
===========================================================================*/
boolean hdrfmac_is_load_info_msg_rcvd ( void )
{
  return hdrfmac.load_info_msg_rcvd;
} /* hdrfmac_Is_load_info_msg_rcvd  */


/*===========================================================================
 
FUNCTION HDRFMAC_SET_LOAD_INFO_MSG_RCVD

DESCRIPTION
  This function resets the indicator which shows whether load information
  message is received in the CONNECTED OR IDLE state from the current serving
  sector.
 
DEPENDENCIES
  None.

PARAMETERS
  load_info_msg_rcvd - TRUE, if Load Information message has been received
                       in the CONNECTED OR IDLE state from the current serving
                       sector. FALSE, if not.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
 
===========================================================================*/
void hdrfmac_set_load_info_msg_rcvd
(
  boolean load_info_msg_rcvd 
)
{
  hdrfmac.load_info_msg_rcvd = load_info_msg_rcvd;
}

/*===========================================================================
 
FUNCTION HDRFMAC_FIND_LOADADJ_CACHE

DESCRIPTION
  This function searches load_info table based on given chan_ptr
  Note that this function only searches based on given chan. Caller
  need to check if the pn matches.
 
DEPENDENCIES
  None.

PARAMETERS
  chan_ptr - pointer to channel to search for

RETURN VALUE
  index into the load_info table. negative if not found

SIDE EFFECTS
  None.
 
===========================================================================*/
hdrfmac_loadadj_cache_type* hdrfmac_find_loadadj_cache
(
  sys_channel_type* chan
)
{
  hdrfmac_loadadj_cache_type* loadadj_ptr = NULL;
  int8 i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 
  ASSERT(chan != NULL);
  
  for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
  {
    if (hdrfmac.loadadj[i].state != HDRFMAC_LOADADJ_CACHE_INVALID &&
        hdrfmac.loadadj[i].serving_sector.channel.band == chan->band &&
        hdrfmac.loadadj[i].serving_sector.channel.chan_num == chan->chan_num)
    {
      loadadj_ptr = &hdrfmac.loadadj[i];
      break;
    }
  }

  if (loadadj_ptr != NULL)
  {
    HDR_MSG_PROT_4(MSG_LEGACY_MED, "find_loadadj ch %d pn %d st %d @ %d",
                   chan->chan_num, 
                   loadadj_ptr->serving_sector.pilot_pn,
                   loadadj_ptr->state, i);
  }
  else
  {
    HDR_MSG_PROT_1(MSG_LEGACY_MED, "find_loadadj ch %d not found",
                   chan->chan_num);
  }


  return loadadj_ptr;
}

/*===========================================================================
 
FUNCTION HDRFMAC_FIND_FREE_LOADADJ_CACHE

DESCRIPTION
  This function searches for a free entry in load_info table. It will
  return the first INVALID cache entry if found, or the oldest one
  in case there is no INVALID entry.
 
DEPENDENCIES
  None.

PARAMETERS
  

RETURN VALUE
  index into the load_info table. negative if not found

SIDE EFFECTS
  None.
 
===========================================================================*/
hdrfmac_loadadj_cache_type* hdrfmac_find_free_loadadj_cache( void )
{
  int8 i;
  hdrfmac_loadadj_cache_type* loadadj_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */  
  loadadj_ptr = &hdrfmac.loadadj[0];

  for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
  {
    if (hdrfmac.loadadj[i].state == HDRFMAC_LOADADJ_CACHE_INVALID)
    {
      loadadj_ptr = &hdrfmac.loadadj[i];
      break;
    }

    if (qw_cmp(loadadj_ptr->timestamp, 
               hdrfmac.loadadj[i].timestamp) > 0)
    {
      loadadj_ptr = &hdrfmac.loadadj[i];
    }
  }

  HDR_MSG_PROT_4(MSG_LEGACY_MED, "find_free_loadadj ch %d pn %d st %d @ %d",
                 loadadj_ptr->serving_sector.channel.chan_num,
                 loadadj_ptr->serving_sector.pilot_pn,
                 loadadj_ptr->state, i);

  loadadj_ptr->state = HDRFMAC_LOADADJ_CACHE_INVALID;

  return loadadj_ptr;
}
/* EJECT */
/*===========================================================================

FUNCTION HDRCMAC_PROCESS_TIMER

DESCRIPTION
  This function handles the timer expiry events assigned to this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  timer_id - ID of the timer just expired.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_loadadj_timer( void )
{
  time_type cur_time;
  uint8 i;
  boolean all_expired = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  if (!hdrfmac_is_loadinfo_supported())
  {
    return;
  }

  time_get(cur_time);

  for (i=0;i<HDRFMAC_LOADADJ_CACHE_SIZE;i++)
  {
    /* if entry has expired */
    if (hdrfmac.loadadj[i].state != HDRFMAC_LOADADJ_CACHE_INVALID)
    {
      if (hdrutil_calc_duration(hdrfmac.loadadj[i].timestamp, cur_time) >= 
                                HDRFMAC_LOADADJ_TIMERS_MS)
      {      
        HDR_MSG_PROT_2(MSG_LEGACY_MED, "proc_loadadj_timer ch %d pn %d",
                       hdrfmac.loadadj[i].serving_sector.channel.chan_num,
                       hdrfmac.loadadj[i].serving_sector.pilot_pn);
           
        hdrfmac_reset_loadadj_cache(&hdrfmac.loadadj[i]);
      }
      else
      {
        all_expired = FALSE;
      }
    }
  }

  if (!all_expired)
  {
    hdrfmac_start_loadadj_timer( cur_time );
  }
}

/* EJECT */ 
/*===========================================================================

FUNCTION HDRCMAC_PROCESS_TIMER

DESCRIPTION
  This function handles the timer expiry events assigned to this protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  timer_id - ID of the timer just expired.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmac_process_timer
( 
  uint32 timer_id
)
{
  switch(timer_id)
  {
    case HDRFMAC_LOADADJ_TIMER_ID:
      hdrfmac_process_loadadj_timer();
      break;

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_ERROR, "unknown timer_id", timer_id);
      break;
  }
}
#endif /* FEATURE_HDR_REVC */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_SET_AWAITING_TC_ASSIGN_COMPLETE                     EXTERNAL

DESCRIPTION
  This interface is used to set the flag indicating whether HDRFMAC is still
  waiting TCA assignment to be completed by HDR SRCH.

DEPENDENCIES
  None

PARAMETERS
  value - boolean value to set the flag.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrfmac_set_awaiting_tc_assign_complete
( 
  boolean value
)
{
  HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
                  "FMAC awaiting_tc_assign_complete, curr %d, to be set %d", 
                  hdrfmac.awaiting_tc_assign_complete,
                  value );
  hdrfmac.awaiting_tc_assign_complete = value;
} /* hdrfmac_set_awaiting_tc_assign_complete() */

 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_GET_AWAITING_TC_ASSIGN_COMPLETE                     EXTERNAL

DESCRIPTION
  This interface is used to get the flag indicating whether HDRFMAC is still
  waiting TCA assignment to be completed by HDR SRCH.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  value - boolean value to set the flag.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrfmac_get_awaiting_tc_assign_complete( void )
{
  return hdrfmac.awaiting_tc_assign_complete;
} /* hdrfmac_get_awaiting_tc_assign_complete() */

#ifdef FEATURE_HDR_MODEM_API
/*===========================================================================
FUNCTION HDRFMAC_GET_DRC_CHANNEL_GAIN                                EXTERNAL

DESCRIPTION
  Returns the drc_channel_gain from the indicated carrier.
  Returns HDRFMAC_CHAN_GAIN_INVALID if call is not active or
  carrier is not valid.
 
DEPENDENCIES
  None.

PARAMETERS
  carrier - The carrier id from TCA

RETURN VALUE
  The drc_channel_gain for the carrier.

SIDE EFFECTS
  None.
===========================================================================*/
extern int8 hdrfmac_get_drc_channel_gain
( 
  uint8 carrier 
    /* Carrier id from TCA */
)
{
   /* Initialize to some error value */
   int8 drc_channel_gain = HDRFMAC_CHAN_GAIN_INVALID;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {
    if (carrier < hdrfmac.tcam_params.num_carriers)
    {
      drc_channel_gain = hdrfmac.tcam_params.
         sub_aset[carrier].drc_channel_gain;
    }
  }

  return drc_channel_gain;
}

/*===========================================================================
FUNCTION HDRFMAC_GET_ACK_CHANNEL_GAIN                                EXTERNAL

DESCRIPTION
  Returns the ack_channel_gain from the indicated carrier.
  Returns HDRFMAC_CHAN_GAIN_INVALID if call is not active or
  carrier is not valid.
 
DEPENDENCIES
  None.

PARAMETERS
  carrier - The carrier id from TCA

RETURN VALUE
  The ack_channel_gain for the carrier.

SIDE EFFECTS
  None.
===========================================================================*/
extern int8 hdrfmac_get_ack_channel_gain
( 
  uint8 carrier 
    /* Carrier id from TCA */
)
{
   /* Initialize to some error value */
   int8 ack_channel_gain = HDRFMAC_CHAN_GAIN_INVALID;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If FMAC is active */
  if ( hdrfmac_is_active(HDRHAI_FTC_MAC_PROTOCOL) )
  {
    if (carrier < hdrfmac.tcam_params.num_carriers)
    {
      ack_channel_gain = hdrfmac.tcam_params.
         sub_aset[carrier].ack_channel_gain;
    }
  }
  
  return ack_channel_gain;
}
#endif /* FEATURE_HDR_MODEM_API */

