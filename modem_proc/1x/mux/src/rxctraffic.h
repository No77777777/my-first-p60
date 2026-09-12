#ifndef RXCTRAFFIC_H
#define RXCTRAFFIC_H
/*===========================================================================

            C D M A    R E C E I V E   T R A F F I C    M O D U L E

DESCRIPTION
  This header file contains definitions for the receive traffic module
  that are used by the rxc unit.  This header file is internal to the rxc
  unit.

  Copyright (c) 1997 - 2015 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$PVCSPath: O:/src/asw/COMMON/vcs/rxctraffic.h_v   1.26   15 Oct 2002 18:30:56   donb  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxctraffic.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/20/17   ab      1x2GTA : 0x18CD log packet changes.
02/12/16   eye     1x2GTA FFPC set point changes.
02/05/16   eye     1x2GTA Feature changes
03/12/15   eye     Added DRX changes to avoid race conditions with SRCH.
02/27/15   eye     Added initial changes for 1x/1xA DRX feature for THOR.
11/12/14   eye     Added API to get and set MVS variable & reset in call end.
03/21/14   vks     No need to send RXC_TC_F command to PC isr for timetransfer
01/28/14   pap     Aggregating the variables into global structures.
01/08/14   pap     Resolving Strip/pack issue.
11/18/13   eye     Added sch0 metrics to demod stats sub packet.
11/11/13   eye     Externalize 0x14ED logging function.
08/27/13   pap     Mainlining features FEATURE_MODEM_1X_USE_ATMR_IN_RC11.
07/23/13   srk     Remove unnecessary global variables.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
10/22/12   jtm     Suspend power measurement and fade checking while a TT HHO
                   is in progress.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
06/21/12   jtm     Mainlined FEATURE_IS2000_REL_A_CC.
04/16/12   jtm     Export rxc_tc_isr_select.
03/28/12   srk     Mainlined FEATURE_DS
02/23/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A feature clean up.
12/13/11   srk     Feature clean up: Mainlined FEATURE_GPSONE_OVERRIDE_RDA and
                   changed all FEATURE_IS2000_REL_A_* to FEATURE_IS2000_REL_A
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
09/28/11   srk     Mainlined Always On features and removed Always Off Features
07/26/11   vks     Fix compiler warnings.
04/27/11   jtm     Export rxc_mdsp_1xa_logging_isr.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/25/11   jtm     Added Mux Vocoder interface module.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/15/10   jtm     Feature cleanup.
12/13/10   jtm     Feature guard FEATURE_MVS_MIGRATE with
                   FEATURE_MODEM_1X_SUPPORTS_VOICE.
12/02/10   jtm     Added the capability to track the synchronization
                   of MVS and TXC and report errors.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
09/08/10   jtm     Added frame_number to the rxc_rc11_dec_type.
08/10/10   jtm     Added RC11 entries to rxc_spr_rate_tab and rxc_rnd_factor_tab.
06/15/10   jtm     Added is_guaranteed parameter to fade_check().
                   Renamed valid_frame to is_guaranteed_frame.
                   Added is_guaranteed_frame paramter to fade_check_fch().
                   Added extern to rxc_rc11 and rxc_tx_in_traffic_on. Moved
                   definitions of rxc_mdsp_dec_type and rxc_rc11_dec_type here.
                   Changed fackch_ack_or_nak_test_mode from boolean to byte.
                   1xA FET logging added and 1xA clean up.
06/10/10   jtm     Eliminate dependency on log_dmss.h
02/01/10   jtm     Removed feature TMOBILE.
08/05/09   jtm     Lint fixes.
05/11/09   jtm     Added include for log_dmss.h to support diag decoupling.
04/01/09   jtm     changed dsm_pool.h to dsm.h.
03/26/09   jtm     Eliminated implicit includes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/16/08   jtm     Added prototype for rxc_configure_ffpc_channel() and defines
                   for spreading rate and rounding factor array sizes.
06/26/06   vlc     Added extern for rxc_sent_valid_rpt.
12/07/04   ejv     Added extern for rxc_spr_rate_tab and rxc_rnd_factor_tab.
06/16/04   sr      Merged in changes from 6500 5004 build
06/14/04   vlc     Added log_pdch_rlp_bits to rxc_rlp_logging_chan_info_type.
05/07/04   vlc     Implemented new RLP interface for EV-DV.  The new interface
                   replaces the current interface for 1x channels as well.
                   Also, modified RLP logging extensively to work with new
                   interface.
04/27/04   vlc     Added function rxc_check_for_sch_update() to fix rate change
                   issue for bursts with 1 frame gap.
04/20/04   jrp     Fixed lint warnings.
05/29/03   vlc     For SVD, created fade_check_fch() and fade_check_dcch() to
                   replace fade_check().
12/12/02   ph      New argument to include channel frame received on rxc_tc_parse()
11/19/02   hxw     Added externed rxc_gps_state;
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
08/08/02   vlc     Fixed compiler error when FEATURE_IS95B_MAHHO is turned off.
07/16/02   bgc     Featurized sch externs inside FEATURE_IS2000_F_SCH.
07/11/02   bn      Added rxc_set_tx_in_traffic_on function prototype
06/04/02   vlc     Fixed minor typo in rxc_rda_log_frame_info() prototype.
06/04/02   bn      Modified the FDCH and RDA logging function parameters for VP2
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
05/13/02   vlc     Added support for simultaneous DCCH & FCH channel set up
                   (feature FEATURE_IS2000_REL_A_VP2.)
02/27/02   lad     Merged IS2000 Release A Common Chanels:
                   Moved TCI code to rxc_tci_init()
12/20/01   bn      Corrected the function name for logging RDA
12/10/01   hrk     Renamed rxc_erasure_flags_type to rxc_eib_flags_type and
                   renamed rxc_erasure_flags to rxc_eib_flags.
11/20/01   bn      Added RDA logging logic
11/13/01   bn      Added the enhance DCCH DTX detection and channel supervision logics
10/30/01   bn      Added support for Release A FDCH logging
09/12/01   bgc     Exported SCH decoder status for use in rxcrda module.
08/31/01   bn      Included the rxc_set_dcch_loopback() function prototype
08/28/01   lh      Back out SI bits changes for Release A Addendum 2.
08/22/01   bn      Support DCCH in FTM mode
06/15/01   day     Mainlined FEATURE_IS2000
06/04/01   vlc     Changed FEATURE_GPSONE to FEATURE_GPS_MODE.
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2.
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           lcc     Changed rxc_stored_sch_packet to use type mctrans_rxc_f_sch_action_type.
03/07/01   va      MOved rxc_tc_var_type into this header file as this is needed by
                   rxcpaging module also.
02/01/01   tc      Updated to rxc_rifhho_cfs_gps_in_progress.
01/30/01   tc      Merged GPS support.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
11/23/00   tc      Fixed MSM5100 PLT rxc_set_fch_loopback, rxc_set_sch_loopback.
11/13/00   bgc     Added accessor functions for FTM loopback flags.
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"

//mux
#include "rxc_i.h"
#include "rxcmc_i.h"
#include "mux_logtypes.h"
#include "ffpc_i.h"
#include "rxccommon_i.h"

//drivers
#include "dec5000_v.h"

//cp
#include "rxtx_v.h"
#include "mctrans_i.h"
#include "cai.h"
#include "cai_v.h"

//other
#include "log.h"
#include "queue.h"
#include "dsm.h"
#include "srchmc_v.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

  /* Indicator for a Return-If-Fail HHO or CFS                            */
  typedef struct {
    boolean previous;
    boolean current;
  } rxc_eib_flags_type;

/* States of Traffic Channel parsing state machine */
typedef enum
{
  TC_HUNT_STATE,
  TC_MSG_STATE,
  TC_FLUSH_STATE
}rxc_tc_state_type;


typedef struct
{
  rxc_tc_state_type  state;
    /* State of Traffic Channel state machine */
  word  msg_inx;   /* index into message buffer */
  word  msg_len;   /* length of message in bits */
  rxtx_rx_msg_type *msg_ptr;
    /* pointer to buffer to place received message for Layer 2 task */
  boolean fch_loopback;
  /* Support FEATURE_FTM_DCCH */
  boolean dcch_loopback;
  /* End of FEATURE_FTM_DCCH */
  boolean sch_loopback;
#ifdef FEATURE_IS2000_REL_A
  byte p_rev_in_use; /* p_rev in use needed for frame unpacking */
  byte temp_p_rev_in_use; /* Store p_rev_in_use temporarily just before
                           the "action" frame arrives */
  boolean in_p_rev_transition; /* Indicates if p_rev is about to change */
  qword p_rev_action_frame; /* Action time of frame when p_rev changes */
#endif /* FEATURE_IS2000_REL_A */
} rxc_tc_var_type;

/* TC state varaiables */
typedef struct
{
  rxc_frame_type rxc_sch_frame_status;

  rxc_tc_var_type rxc_tc_var;

  dec_sch_status_type rxc_sch_status;

  /* indicator of whether to treat blank and burst as erasures */
  boolean rxc_erase;

  /* Indicator of whether the vocoder has been configured for voice
     service option */
  boolean rxc_voc_config_done;

  word rxc_fch_rate; /* To store the FCC rate in rxc_tc_isr */

} rxc_traffic_frames_status;

extern rxc_traffic_frames_status rxc_frame_data;

/* Contains various channel information used in RLP 3 payload logging.
*/
typedef struct
{
  word log_fch_rlp_bits;   /* Num rlp bits on FCH */
  word log_dcch_rlp_bits;  /* Num rlp bits on DCCH */
  word log_pdch_rlp_bits;  /* Num rlp bits on PDCH (encapsulated PDUs only ) */
  boolean log_turbo_sch_erasure; /* Turbo SCH reports erasure frame */
  boolean first_entry;     /* This entry is first entry in the 20ms frame */

} rxc_rlp_logging_chan_info_type;
typedef struct
{
  /* Decision history for FFPC logging */
  word rxc_fpc_history;

  /* Decision history for rev pctrl */
  word rxc_rpc_history; /*lint -e552 need this to debug RL power control */

  /* Forward frame types log accumulation buffer */
  /* Accumulation buffer */
  mux_log_fwd_frame_types_type           rxc_log_frm_types_buf;
  /* Sub-record pointer */
  mux_log_fwd_frame_types_subrecord_type *rxc_log_frm_types_sr_ptr;

  /* Contains various channel information used in RLP 3 payload logging.
  */
  rxc_rlp_logging_chan_info_type rxc_rlp_logging_chan_info;

  mux_log_mar_type rxc_log_mar_buf; /* Log buffer for accumulated Markov frame log */
} rxc_bufs_qs_vars;

extern rxc_bufs_qs_vars rxc_bufs_qs_data;
extern cai_data_rate_type rxc_frame_rate;      /* Current vocoder frame rate */

/*-------------------------------------------------------------------------*/
/* Variables for Fade timers                                               */
/*-------------------------------------------------------------------------*/
typedef struct
{
  word rxc_good_frames; /* number of non-category 10 framesreceived after
                              at least one category 10 frame */

  word rxc_good_guaranteed_frames; /* number of non-category 10 guaranteed
                                          frames received after at least one
                                          category 10 frame */

  word rxc_t5m_bad_frames;  /* number of consecutive category 10frames */
  word rxc_n2m_bad_frames;  /* number of consecutive category 10frames */
  word rxc_ho_cnt;      /* number of frames before handoff processing */
  boolean rxc_fade; /* indicator of whether fade processingis in
                            progress */
  boolean rxc_sent_bad_rpt;
  boolean rxc_sent_pwron_rpt;            /* used for hard handoff */

  boolean rxc_sent_valid_rpt;           /* RXC_VALID_R sent indicator*/

/*TRUE = prevent fade checking during TT prior to HHO_BEGIN action.
  FALSE = permit fade checking
*/
  boolean rxc_pre_tt_ho;

} rxc_fade_timers_vars;

extern rxc_fade_timers_vars rxc_fade_data;

/*-------------------------------------------------------------------------*/
/* Variables for Hard Handoff with Return-on-Failure                       */
/*-------------------------------------------------------------------------*/
typedef struct
{
  word rxc_lost_frames;
/* Lost frame count during a Return-If-Fail HHO                         */
  word rxc_rif_hho_timeout_frames;
/* Equivalent of the Return-If-Fail hard handoff timer                  */
  boolean rxc_delay_after_hho_failure;

  boolean rxc_hard_handoff_in_progress;  /* fade due to hard handoff */

  // CFS tuneback count



  /* Indicator for a Return-If-Fail HHO or CFS                            */
  rxc_eib_flags_type rxc_eib_flags;
#ifdef FEATURE_IS2000_REL_A
  rxc_eib_flags_type rxc_sch_eib_flags;
  rxc_eib_flags_type rxc_qib_flags;
  boolean rxc_use_stored_qibs;
  boolean rxc_use_stored_sch_eibs;
#endif /* FEATURE_IS2000_REL_A */

  boolean rxc_use_stored_eibs;
  rxc_gps_state_type rxc_gps_state;
  int8 rxc_gps_tuneback_frames;
} rxc_hho_vars_type;

extern rxc_hho_vars_type rxc_hho_data;

/* Lost frame count during a Return-If-Fail HHO                         */
/* Equivalent of the Return-If-Fail hard handoff timer                  */

extern boolean rxc_rifhho_cfs_gps_in_progress;

#define SCH_MAX_FWD_PDU 8                     /* Max # of F-SCH MuxPDUs*/
#define RXC_DSM_ARR_NOT_ALLOC 0xFF               /* Const for invalid arr*/

/*-------------------------------------------------------------------------*/
/*  Supplemental Channel variable                                          */
/*-------------------------------------------------------------------------*/
extern dec_sch_cfg_type rxc_sch;

  /* indicator of whether to treat blank and burst as erasures */


/* Lookup tables for spreading rate and rounding factor */
#define NUM_SPR_RATE_RC     4
#define NUM_RND_FACT_RC     4
#define NUM_SPR_RATES       6
#define NUM_RND_FACT        6

extern uint8 const rxc_spr_rate_tab[NUM_SPR_RATE_RC][NUM_SPR_RATES];
extern uint8 const rxc_rnd_factor_tab[NUM_RND_FACT_RC][NUM_RND_FACT];

typedef struct
{
  boolean rxc_log_ffpc_sch_data;

  byte rxc_dsm_arr_cnt;    /* Current index in arr */

  dsm_item_type * rxc_dsm_arr[ SCH_MAX_FWD_PDU];

  mctrans_rxc_f_sch_action_type rxc_stored_sch_packet;
} rxc_log_sch_vars;

extern rxc_log_sch_vars rxc_log_sch_data;


#ifdef FEATURE_IS2000_REL_A_SVD
/*-------------------------------------------------------------------------*/
/*  Simultaneous Voice and Data variable                                   */
/*-------------------------------------------------------------------------*/
/* Used in simultaneous F-FCH and F-DCCH channel set up.  Indicates which
** channel handles FFPC.
*/
extern cai_fpc_pri_chan_type ffpc_channel;
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_1X_ADV
/* This structure is used for RC11 decoded frame.
   RDA information comes in the MDSP early decoding interrupt.
   Decoder frame data and decoder frame status is read from the decoder output buffer
   in the same interrupt.
*/
typedef struct
{
  cai_data_rate_type  mdsp_dec_rate;       /* rate determination read from MDSP */
  rxc_dec_tc_type     mdsp_tc_buf;         /* FCH data read from Dec output buffer */
  dec_fch_status_type mdsp_dec_fch_status; /* FCH status read from Dec output buffer */

} rxc_mdsp_dec_type;

/* This structure is used for RC11 related information. */
typedef struct
{
  /* RXC specific information from RCPM (RC parameters message.)  CP uses default
     RC parameters prior to receiving RCPM, then switch to the non-default RC parameters
     when a RCPM is received.
  */
  cai_rcp_rxc_txc_type    rcp;

  /* TRUE if RXC traffic ISR source is the MDSP early decoding ISR.  Legacy RCs use
     the decoder ISR for this purpose.  For RC11, HW routes the FCH decoder interrupt to
     MDSP.  MDSP performs RDA processing then interrupts SW to read data from the decoder
     output buffer.  This indicator is necessary for RXC to read data from the appropriate
     source.
  */
  boolean             tc_source_mdsp_isr;

  /* Storage for decoder and RDA information from MDSP Interrupt.  This is used in the RXC
     traffic ISR to mimic the processing of legacy RCs as much as possible.
  */
  rxc_mdsp_dec_type   mdsp_dec_save_info;

  /* ACKCH test directive configuration information. This is only used during
      eMSO (SO74) and eLSO (SO75) calls.*/
  byte fackch_ack_or_nak_test_mode;

  struct{
    /* Used to capture the pcg that frame early termination occured.*/
    byte early_decode_pcg;

    /* If set to FALSE, do not use this frame RDA result to update FFPC setpoint */
    boolean is_guaranteed_frame;

    /* If set to TRUE, do not use this frame RDA result to update FCH FFPC setpoint */
    boolean ffpc_setpoint_frozen;

    /* PCG that the ack was received on the F-ACKCH. */
    byte ack_received_pcg;

    /* PCG that the ack was transmitted on the R-ACKCH1. */
    byte ack_transmitted_pcg;

  } status;

} rxc_rc11_dec_type;

/* RC11 related information */
extern rxc_rc11_dec_type rxc_rc11;
#endif /* FEATURE_IS2000_1X_ADV */

extern boolean rxc_tx_in_traffic_on;

#ifdef FEATURE_IS2000_1X_ADV
#define RXC_MAX_N2M_SIZE 4
#endif

typedef struct 
{
  /* this flag indicated the traffic status */
  boolean rxc_tx_in_traffic_on;

  /* Save timing information of demuxing operation for debugging purpose.
  ** Used to determine if RXC task time line is OK.
  */
  qword  rxc_dec_int0_fr_time;
  qword  rxc_dec_int1_fr_time;

  /* Save Deint Task ID for debugging pupose */
  byte   saved_dec_int_task_id; /*lint -e552 */

  #ifdef FEATURE_IS2000_1X_ADV

  /* RC11 related information */
  rxc_rc11_dec_type rxc_rc11;

  /* Lookup table for RC11 N2M values
     These values correspond to the number of frames received by the MS with insufficient
     signal quality before the MS disables the transmitter.
  */
  const word rxc_rc11_n2m_val [RXC_MAX_N2M_SIZE];
  #endif /* FEATURE_IS2000_1X_ADV */
} rxc_traffic_status_vars_type;

extern rxc_traffic_status_vars_type rxc_traffic_status;

typedef struct
{
  timer_type ta_fade_timer;       /* Fade timer during TA GAP */
  uint32     lta_gap_ms;          /* LTA GAP in ms */
  uint32     lta_start_sclk;      /* LTA start sclk time stamp */
  boolean    ta_start;            /* Is TA started */
  boolean    ta_comp;             /* Is last TA completed */

  /* Back Up parameters which will be lost during LTA GAP */
  byte       frame_offset;
  uint8      rev_pwr_ctrl_delay;
  boolean    fch_olpc_enabled;
  byte       fch_incl_status;
  boolean    dcch_olpc_enabled;
  byte       dcch_incl_status;
  boolean    sch_olpc_enabled;
  byte       sch_incl_status;

  /* Time logging */
  boolean    f_fch_ta_log;
  boolean    f_sch_ta_log;
  uint32     ta_start_sclk;
  uint32     f_fch_ta_comp_sclk;
  uint32     f_sch_ta_comp_sclk;

  srch_ta_rsn_type  ta_rsn;       /* TA reason : LTA/QTA */
} rxc_ta_type;

extern rxc_ta_type rxc_ta_info;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifndef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION RXC_FRAME_TYPES_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated forward frame types log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A forward frame types log may be sent.

===========================================================================*/
extern void rxc_frame_types_send_log(void);
#else
/*===========================================================================

FUNCTION RXC_FDCH_FRAME_INFO_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated FDCH frame information log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A FDCH frame information log may be sent.

===========================================================================*/
void rxc_fdch_frame_info_send_log(void);
#endif /* FEATURE_IS2000_REL_A */

/*===========================================================================

FUNCTION RXC_RDA_FRAME_INFO_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated RDA frame information log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A RDA frame information log may be sent.

===========================================================================*/
extern void rxc_rda_frame_info_send_log(void);

/*=========================================================================
FUNCTION RXC_RDA_LOG_FRAME_INFO

DESCRIPTION
  This function populates RDA frame information logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
#ifdef FEATURE_IS2000_REL_A_SVD
extern void rxc_rda_log_frame_info (cai_data_rate_type fch_dec_rate, 
                                    cai_data_rate_type dcch_dec_rate);
#else
extern void rxc_rda_log_frame_info (cai_data_rate_type dec_rate);
#endif /* FEATURE_IS2000_REL_A_SVD */


/*===========================================================================

FUNCTION RXC_TCI

DESCRIPTION
  Traffic Channel Initialization processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern rxc_state_type rxc_tci ( void );

/*===========================================================================

FUNCTION RXC_TC

DESCRIPTION
  Traffic Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern rxc_state_type rxc_tc( void );

/*===========================================================================

FUNCTION RXC_TA_TOTAL_SKIP_FRAME_COUNT_INC

DESCRIPTION
  Increment total FRAME COUNT for FCH/DCCH/SCH only outside TA GAP
  Increment skipped FRAME COUNT for FCH/DCCH/SCH during TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
extern void rxc_ta_total_skip_frame_count_inc( word* total_frame_count, word* skip_frame_count );

/*===========================================================================

FUNCTION RXC_TA_BAD_FRAME_COUNT_INC

DESCRIPTION
  Update BAD FRAME COUNT for FCH/DCCH/SCH only outside TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
extern void rxc_ta_bad_frame_count_inc( word* bad_frame_count );

/*===========================================================================

FUNCTION RXC_TA_FER_UPDATE

DESCRIPTION
  Update FCH,DCCH,SCH frame counts after TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
extern void rxc_ta_fer_update( boolean sch, uint8 sch_id );

/*===========================================================================

FUNCTION RXC_IS_TA_ENABLED

DESCRIPTION
  Returns status of TA GAP

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  : If 1x is in TA GAP
  FALSE : If 1x is not in TA GAP

SIDE EFFECTS
  None

===========================================================================*/
extern boolean rxc_is_ta_enabled( void );

/*===========================================================================

FUNCTION RXC_TA_FADE_CHECK

DESCRIPTION
  Sets or resets fade flag depending upon TA GAP.

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/
extern void rxc_ta_fade_check( void );

/*===========================================================================

FUNCTION RXC_TA

DESCRIPTION
  Long Tune Away during 1x2GTA processing state

DEPENDENCIES
  None.

RETURN VALUE
  Current RXC state.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern rxc_state_type rxc_ta( void );

/*===========================================================================

FUNCTION RXC_TX_TA_SRCH_LOG_UPDATE

DESCRIPTION
  Thi API updates TA durations to SRCH module

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rxc_tx_ta_srch_log_update( void );

/*===========================================================================

FUNCTION RXC_LOG_MAR

DESCRIPTION
  This function logs the Forward Traffic Channel Markov test data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_log_mar(byte mar,byte ber);

/*===========================================================================

FUNCTION RXC_TC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Traffic Channel.
  Due to timing requirements imposed by the 13k decoder and the erasure
  indicator bit turnaround time we need to first process the decoded frame
  and decide what rate it is before building the final tx frame at this
  point in time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_isr( void );

/*===========================================================================

FUNCTION PWR_MSR_CHECK

DESCRIPTION
  This procedure processes the updated rxc_pwr structure to determine if a
  power measurement report should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A power measurement report may be sent to MC.

===========================================================================*/
extern void pwr_msr_check( void );

#ifdef FEATURE_IS2000_REL_A
/*=========================================================================
FUNCTION RXC_FDCH_LOG_FRAME_INFO

DESCRIPTION
  This function populates FDCH frame information logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
#ifdef FEATURE_IS2000_REL_A_SVD
void rxc_fdch_log_frame_info (byte fch_mux, byte dcch_mux ); /* mux type for FCH/DCCH logging */
#else
void rxc_fdch_log_frame_info (byte mux ); /* mux type for FCH/DCCH logging */
#endif /* FEATURE_IS2000_REL_A_SVD */
#else
/*=========================================================================
FUNCTION RXC_LOG_FRAME_TYPE

DESCRIPTION
  This function populates frame type logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
extern void rxc_log_frame_type
(
  int16 frame_rate,                /* FCH  Frame rate */
  mux_log_fch_type_enum_type fch_type, /* FCH  Frame type */
  mux_log_sch_rate_enum_type sch0_type /* SCH0 Frame type */
  , cai_chind_type ch_ind     /* indicate which channels are active */
  , byte mux           /* mux type for DCCH logging */
);

#endif /* FEATURE_IS2000_REL_A */
/*===========================================================================

FUNCTION RXC_TC_PARSE

DESCRIPTION
  This function collects the signaling bits of the Traffic Channel.
  When it determines that an entire Traffic Channel message has been
  received and the message CRC checks, it will place the message on
  a queue for the Layer 2 task and signal the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicator of whether message data was good.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_tc_parse
(
  qword         frame_num,
  byte          frame[],    /* Address of 1st byte of frame. */
  word          frame_len,  /* Number of bits in frame */
  rxc_dsch_type frame_chan  /* Channel frame received on */
);

/*===========================================================================

FUNCTION RXC_TC_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_init
(
  rxc_state_type curr_state   /* Current state of RXC state machine */
);

/*===========================================================================

FUNCTION RXC_TCI_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel initialization state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern rxc_state_type rxc_tci_init
(
  rxc_state_type curr_state,
  rxc_cmd_msg_type *cmd_ptr,
  boolean skip_isr_cmd
);


#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION RXCTRAFFIC_FRAME_BND_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel processing state at the
  frame boundary.  Note this is called in ISR context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxctraffic_frame_bnd_init (void);
#endif /* FEATURE_IS2000_REL_A */


#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION FADE_CHECK_FCH

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.  This procedure is called when the primary power
  control channel is FCH.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
extern void fade_check_fch
(
  byte mux,
  boolean is_guaranteed
);

/*===========================================================================

FUNCTION FADE_CHECK_DCCH

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.  This procedure is called when the primary power
  control channel is DCCH.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
extern void fade_check_dcch
(
  rxc_dcch_quality_type  dcch_chan_quality
);

#else /* else of FEATURE_IS2000_REL_A_SVD */
/*===========================================================================

FUNCTION FADE_CHECK

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
extern void fade_check
(
  byte mux,
  boolean is_guaranteed
  /* category of received frame */
  ,rxc_dcch_quality_type  dcch_chan_quality
);
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION RXC_FCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether FCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_fch_loopback_query( void );

/* Support FEATURE_FTM_DCCH */
/*===========================================================================

FUNCTION RXC_DCCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether DCCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_dcch_loopback_query( void );
/* End of FEATURE_FTM_DCCH */

/*===========================================================================

FUNCTION RXC_SCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether SCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_sch_loopback_query( void );

/*===========================================================================

FUNCTION RXC_SET_FCH_LOOPBACK

DESCRIPTION
  This procedure sets FCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
extern void rxc_set_fch_loopback( boolean );

/*===========================================================================

FUNCTION RXC_SET_DCCH_LOOPBACK

DESCRIPTION
  This procedure sets DCCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
extern void rxc_set_dcch_loopback( boolean );

/*===========================================================================

FUNCTION RXC_SET_SCH_LOOPBACK

DESCRIPTION
  This procedure sets SCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
extern void rxc_set_sch_loopback( boolean );

/*=============================================================================
FUNCTION RXC_SET_TX_IN_TRAFFIC_ON

DESCRIPTION
  This function is called to set/reset the rxc_tx_in_traffic_on flag if TX is turned
  on/off

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void rxc_set_tx_in_traffic_on(boolean tx_status);

/*===========================================================================

FUNCTION rxc_check_for_sch_update

DESCRIPTION
  This function checks for SCH updates in case of 1 frame gap.  This function
  is called AFTER the current frame has been demuxed and is ready for RLP,
  and BEFORE RLP is called.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_check_for_sch_update (void);

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION RXC_CONFIGURE_FFPC_CHANNEL

DESCRIPTION
  Used in simultaneous F-FCH and F-DCCH channel set up.  Indicates
  which channel handles FFPC. Also resets the data valid flag and
  counters for FCH and DCCH in VP2

DEPENDENCIES


RETURN VALUE
  Current Radio Configuration

SIDE EFFECTS
  None
===========================================================================*/
cai_radio_config_type rxc_configure_ffpc_channel
(
  rxc_cmd_msg_type *cmd_ptr
);
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION RXC_LOG_1XA_FET_STATS

DESCRIPTION:
  When LOG_1X_ADV_FET_STATS_C is enabled, log the current frame early termination
  statistics and report them.

DEPENDENCIES
  Currently requires LOG_1X_ADV_L1_PERF_C to be enabled since information
  is extracted from the mdsp_buf used by that log.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_log_1xa_fet_stats(void);

/*===========================================================================

FUNCTION RXC_1X_ADV_FET_STATS_FLUSH_LOG

DESCRIPTION
  Terminate and flush the accumulated 1x Advanced FET Stats Log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A 1x Advanced FET Stats log may be flushed.

===========================================================================*/
void rxc_1x_adv_fet_stats_flush_log(void);

/*===========================================================================

FUNCTION RXC_MDSP_1XA_LOGGING_ISR

DESCRIPTION:
  This function is a call back function which processes MDSP interrupts for
  1x Advanced logging in traffic state. This ISR fires every half frame.

DEPENDENCIES
  MDSP must be running and the ISR must have been previously registered.
  E and I flags (bits 15 and 0) of mDSP variable CDMA_rliLogConfig must be
  enabled (set to 1).

RETURN VALUE
  None.

SIDE EFFECTS
  1x Advanced data is read from the MDSP and stored for logging purposes.

===========================================================================*/
void rxc_mdsp_1xa_logging_isr (int16 mdsp_buffer_index);

/*===========================================================================

FUNCTION RXC_TC_ISR_SELECT

DESCRIPTION
  This function selects the appropriate RXC 20ms frame processing ISR.
  RC 11 uses the MDSP ISR for this purpose while other RCs use the traditional
  Decoder ISR. RC 11 is different because it involves early frame determination
  and MDSP performs rate determination algorithm rather than SW.

  For RC11, this function also sets the MDSP to the right state to process RC11.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Either ATMR ISR or DEC ISR is used to drive RXC frame processing.  Both ISRs
  execute around PCG 0 of the frame.

===========================================================================*/
void rxc_tc_isr_select (void);

/*===========================================================================

FUNCTION  RXC_UPDATE_1XA_LOGGING_STATUS

DESCRIPTION
  When enabled == true, this function will configure the mDSP to begin
  collecting data for 1xA calls if logging is permitted.
  It will also register the callback that the mDSP will use to provide
  the data to Mux.

  When enabled == false, this function will deregister the interrupt
  callback and disable logging at the mDSP.

  Redundant calls are ignored.

DEPENDENCIES
  RL must be in traffic.
  F-FCH RC must be RC11.

RETURN VALUE
  None.

SIDE EFFECTS
  The 1xA logging mechanism may be enabled or disabled.

===========================================================================*/
void rxc_update_1xA_logging_status (boolean enable);
#endif /* FEATURE_IS2000_1X_ADV */

/*========================================================================

FUNCTION RXC_IS_DATA_CALL

DESCRIPTION This function checks the primary and secondary service options
            for the current call and returns a boolean indicating whether
            or not the SO is Data related.

DEPENDENCIES None

RETURN VALUE True for Data SOs, False otherwise.

SIDE EFFECTS None

=========================================================================*/
extern boolean rxc_is_data_call( void );

#ifdef FEATURE_MODEM_1X_DRX
/*===========================================================================

FUNCTION RXC_MUX_DRX_ENABLE

DESCRIPTION
  This function is called by SRCH to enable or disable DRX.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void rxc_mux_drx_enable( boolean srch_drx_enable );
#endif /* FEATURE_MODEM_1X_DRX */

#endif /* RXCTRAFFIC_H */
