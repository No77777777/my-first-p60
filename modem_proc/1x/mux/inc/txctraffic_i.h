#ifndef TXCTRAFFIC_I_H
#define TXCTRAFFIC_I_H
/*===========================================================================

           C D M A   T R A N S M I T   T R A F F I C   M O D U L E

DESCRIPTION
  This header file contains definitions for the transmit traffic module  that
  are used by the txc unit.  This header file is internal to the txc unit.

  Copyright (c) 1997 - 2016 Qualcomm Technologies, Inc.
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

$PVCSPath: O:/src/asw/COMMON/vcs/txctraffic.h_v   1.18   15 Oct 2002 18:31:02   donb  $
$Header: //components/rel/1x.mpss/8.0/mux/inc/txctraffic_i.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/30/16   eye     1x2GTA : 0x18CD log packet changes.
05/18/16   srk     Remove unused SCR complete and FFPC freeze flags.
05/16/16   eye     Mainline FEATURE_MODEM_1X_IRAT_LTO1X for L compileout build.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
02/05/16   eye     1x2GTA Feature changes
07/31/15   agh     WTR Hopping changes for inter band CFS and RIF HHO
11/04/13   dkb     Clean up 1x RL and remove unsupported FW messages.
10/03/13   srk     Add support for Device only DTX for Full Rate frames
09/26/13   eye     T2P boost defaulted to 5dB and NV item removal.
09/10/13   dkb     Register Tx interrupts using m1x vstmr API
09/06/13   srk     Dynamic DTX control changes and unification of T2P boost
                   for RC3 and RC8.
08/21/13   srk     Update proper gain tables for RC3 DTX.
07/23/13   srk     Remove unnecessary global variables.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
05/14/13   eye     Unifying Device only DTX for RC3/4 and RC8.
11/02/12   vks     Initial support for antenna selection diversity feature.
10/19/12   srk     Add pilot gating mask for SB.
10/18/12   srk     Remove the global variable to enable or diable debug F3s for
                   device only DTX.
10/18/12   srk     Export txc_is_test_so() and txc_do_device_only_dtx()
10/03/12   srk     Remove the unused TX power thresholds and IIR coeficient
09/11/12   srk     Define mask for DTX disabled case.
08/27/12   srk     Apply T2P gain ratio boost of tx power boost.
08/23/12   srk     Stop DTX when SCH is active.
08/02/12   srk     Added Device only DTX algorithm for 1x Advanced calls.
02/23/12   srk     Mainlined FEATURE_IS2000_SCH and FEATURE_IS2000_P2
02/16/12   srk     Replaced FEATURE_LTE_TO_1X by FEATURE_MODEM_1X_IRAT_LTO1X.
01/23/12   srk     Feature Cleanup
01/18/12   srk     Feature Cleanup
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
08/04/11   jtm     Added TT tx power boost capability.
07/26/11   vks     Fix compiler warnings.
06/08/11   jtm     Added Quick Repeat capability.
05/23/11   jtm     Provided a mechanism to allow a configurable delay from TX on
                   until we send a signaling message.
05/20/11   jtm     Protect changes to SO and physical channel.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/25/11   jtm     Added Mux Vocoder interface module.
12/15/10   jtm     Feature cleanup.
10/30/10   jtm     Modem 1x SU API changes.
08/10/10   vks     Cleanup Genesis Revisit feature.
06/15/10   jtm     Added external declaration for txc_rpctrl_send_log and
                   relocated various logging related variables to
                   mux_logtypes.h.
06/09/10   jtm     Added declaration for txc_process_rcpm_event().
                   Corrected type of frame_rate_1xA and other clean up.
04/12/10   jtm     Exported 1xA utility functions and renamed 1xA specific
                   variables to distinguish that they are for 1xA purposes.
02/04/10   jtm     Removed FEATURE_PLT.
12/01/10   bb      Add support for LTE to 1X feature
11/24/09   jtm     Expanded the txtc structure to keep track of additional
                   1x Advanced information (PC mode, PC pattern, etc.)
11/07/09   vks     Grouped together mdsp writes for channel gains.
                   Remove tx_rate_adj variable. Use the one present in
                   channel_gains data structure (in muxmdsp.h) instead.
09/14/09   adw     Changes to support modem data header CMI refactoring.
08/13/09   jtm     Removed FEATURE_1X_DISABLE_CMI.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Removed unused bio.h include.
06/11/09   jtm     1x Advanced RL support. Added fch_blk_duty_cycle to txtc_type.
04/01/09   jtm     Added customer.h and comdefs.h
03/26/09   jtm     Featurize _v.h includes from subsystems outside of 1x.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code.
10/30/06   bn      Added function prototype txc_tt_tc_init() to support DO/UMTS
                   to 1x handoff
07/24/06   vlc     Removed extern prim_num_bits which is only used in txcmux.c.
06/09/06   trc     Remove T_MSM6700 featurization
09/01/04   jrp     Corrected compiler warning.
09/01/04   jrp     Added back pre-6700 interfaces.
08/16/04   sr      Decalred prototype for txc_mdsp_dv_app_enable()
08/16/04   ljl     Rel D reverse channels PLT.
07/28/04   jrp     Removed lint warning.
06/29/04   jrp     Added argument to txc_traffic_isr() to store last interrupt
                   bin.
05/07/04   vlc     Modified RLP logging extensively to decouple MUX processing
                   and RLP processing, moved RLP processing to RLP module.
04/20/04   jrp     Fixed lint warnings.
03/28/03   vlc     Added struct txc_rlp_sig_priority_type to keep track of
                   priorities in sending RLP control frames as secondary SO
                   in a SVD call.
10/15/02   dlb     Added new power control logging to combine power control
                   and frame type logging.
09/04/02   bn,hrk  Moved prim_dcch_data_ptr, prim_dcch_rate out of
                   FEATURE_IS2000_REL_A_SVD.
06/28/02   bn      Added new interface to support MUX PDU5
06/12/02   hrk     Added prototype for txc_tx_process_fundich_data3g().
06/05/02   hrk     Added extern declarations for globals used for Primary and
                   Secondary traffic types.
03/04/02   bgc     Changed features to IS2000_R_SCH where appropriate.
02/25/02   hjr     Fixed featurization of FEATURE_IS2000_SCH with
                   FEATURE_IS2000_DS_RS2 on.
02/25/02   bn      Added support for  MUX SVD signaling
10/30/01   bn      Added support for Release A RDCH logging
09/27/01   vlc     Added inclusion of datapump.h to resolve datapump externs
                   declared in this file (for PLT mode only.)
08/22/01   bn      Support DCCH in FTM mode
06/15/01   day     Mainlined FEATURE_IS2000
05/11/01   vlc     Merged in datapump changes for PLT (ks.)
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2
03/07/01   va      Added p_rev_in_use in the txtc_type structure.
03/01/01   tc      Merged MSM_MUX1X.00.00.15.
02/20/01   hrk     Added tx_rate_adj to txtc_type.
01/11/01   tc      Merged MSM_MUX1X.00.00.06.
11/23/00   tc      Fixed txc_dp_run for MSM5100 PLT.
12/07/00   bgc     Removed initialization of extern for PLT.
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common.h
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

//mux
#include "txc.h"
#include "txc_v.h"
#include "txc_i.h"
#include "txccommon_i.h"

//drivers
#include "enc.h"
#include "enc_v.h"
#include "enc_i.h"
#include "m1x_stmr_i.h"

//cp
#include "cai.h"

//other
#include "dsrlp_api.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

// Definitions for physical channels carrying signalling
typedef enum
{
  TXC_REV_FCH  = 0x1,
  TXC_REV_DCCH = 0x2
} txc_dsch_type;

/* Specific variables for the Traffic Channel */
typedef struct
{
  boolean            preambles;    /* Send Traffic Channel preambles  */
  word               num_preambles;/* # of Traffic Channel preambles  */
  boolean            nulls;        /* Send null data  */
  txc_tc_frame_type  frame;        /* Current frame to send */
  txc_tc_dcch_frame_type  dcch_frame;   /* Current DCCH frame to send */
  txc_dsch_type      dsch_map;     /* Channel mask of channels mapped to dedicated signalling channel */
  txc_dsch_type      cur_dsch;     /* DSCH currently used */
  cai_pilot_gating_rate_type gating_rate; /* Indicate if pilot gated and rate */
  byte               dim_delay;    /* Wait for correct rate vocoder frame */
  byte               sig_frame_delay; /* Wait for a few frames following TX OFF to ON before sending signaling.*/
  byte               quick_repeat_frame_cnt;
  byte               power;        /* Power level */
  enc_rate_type      rate;         /* current frame encoder rate */
  enc_rate_type      last_rate;    /* Rate of the last tc frame */
  word               sigs;         /* Signal to set when TX task has acquired
                                      timing has moved to traffic state */
  rex_tcb_type       *tcb_ptr;     /* Pointer to task to signal when TX task
                                      has moved to traffic state */
  boolean            ready_to_transmit;
                                   /* Flag to indicate if TC is ready to
                                      transmit (the preamble) during TC
                                      initialization.  We do not transmit
                                      anything before the 2 good frames are
                                      received in TCI. */
  txc_tc_supp_frame_type  supp_frame; /* current supp. frame to send */
  enc_sch_rate_type       supp_rate;

  int8               rl_gain_adj;
  boolean            fch_8th_rate_gating;
  byte               offset_pwr; /* turn around constant for
                                    Open Loop Power Estimation */

  boolean            sch_on;
  boolean            supp_turbo;    /* supp. channel Turbo?  */
  word               supp_ltu_size; /* supp. channel ltu size   */
  cai_mux_pdu_type_type mux_pdu_type; // 1,2, 3, 5
  uint8             num_ltu;          // Number of LTUs in an SDU

  boolean            txc_dtx_off;           /* DTX expired? */
  boolean            dtx_duration_included; /* DTX activate? */
  byte               dtx_duration_org;      /* original/reset value */
  uint16             dtx_duration;          /* active value */

  byte               pot_num_mux_pdus; /* # of MuxPDUs in a R-SCH frame    */
  byte               ok_num_mux_pdus;  /* allowed # mux=-pdus in a frame   */

  boolean            double_sized;  /* TRUE if double sized fr being tx */
  cai_radio_config_type   r_sch_rc; /* RC of R-SCH when sch_on is TRUE */

  byte               pwr_ctrl_step_size; /* Step size */
  boolean            fch_loopback;  /* fch loopback mode */
  /* Support FEATURE_DCCH */
  boolean            dcch_loopback;  /* dcch loopback mode */
  /* End of FEATURE_DCCH */
  boolean            sch_loopback;  /* sch loopback mode */

  byte p_rev_in_use;  /* P_REV in for needed for frame building */
  uint16 rev_mux;       /* for RDCH logging */

  m1x_stmr_event_type last_encoder_interrupt_bin;

  struct
  {
    /* Flag to indicate if the current frame is critical or not */
    boolean                           is_non_critical_frame;

    /* Flag to indicate if the current frame is a non-critical 1/8 rate and
       will be blanked based on the Smart Blanking algorithm */
    boolean                           blank_non_crit_fch_frame;

    /* Power control step size selected after Smart Blanking is applied */
    cai_pwr_cntl_step_type            pwr_ctrl_step_size;

    /* Power control mode selected after Smart Blanking is applied */
    cai_rcpm_pwr_cntl_mode_type       pwr_ctrl_mode;

    #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
    /* RPC Mask for RC8 frames */
    uint16                            rpc_mask;

    /* FPC Mask for RC8 frames */
    uint16                            fpc_mask;
    #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */
  } rc8_status;

} txtc_type;

typedef enum
{
  TXC_REV_SCH_OFF = 0,
  TXC_REV_SCH_DTX,
  TXC_REV_SCH_ACTIVE
} txc_rsch_state;

extern txc_rsch_state sch_state;

#ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
/* Parameters for Device only DTX - NV Controlled */
typedef struct
{
  uint16    dtx_enabled;
} txctraffic_device_only_dtx_nv_params_type;

/* Parameters for Device only DTX - Statically Assigned */
typedef struct
{
  uint8     dtx_t2p_boost_frac_rate_dbq3;
  uint8     dtx_t2p_boost_full_rate_dbq3;
} txctraffic_device_only_dtx_static_params_type;

extern txctraffic_device_only_dtx_nv_params_type     txc_device_dtx_nv;
extern txctraffic_device_only_dtx_static_params_type txc_device_dtx_static;

/* Static define Dynamic DTX control thresholds */
typedef struct
{
  uint16  txpower_on_thresh_dbm10;   /* Tx power offset in dBm10 from Max
                                        Tx power to turn ON DTX  */
  uint16  txpower_off_thresh_dbm10;  /* Tx power offset in dBm10 from Max
                                        Tx power to turn OFF DTX */
  uint32  flsetpt_thresh_dbq3;       /* FL setpoint offset in dBq3 from Max
                                        FL setpoint to turn ON/OFF DTX */
} txctraffic_dynamic_dtx_thresh_type;

extern txctraffic_dynamic_dtx_thresh_type txc_dynamic_dtx_thresh;
#endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

typedef struct
{
  /* Flag to indicate if the current frame is DTXd or not */
  boolean   dtx_frame;

  /* DTX Mask for the current frame */
  uint16    dtx_mask;

  /* FCH2PICH gain ratio boost in dBq3 */
  uint8     dtx_t2p_boost_dbq3;
} txctraffic_device_dtx_param_type;

extern txctraffic_device_dtx_param_type txc_device_only_dtx_param;

extern txtc_type txtc;
extern byte *prim_data_ptr;           /* Pointer to primary data */
extern cai_data_rate_type prim_rate;  /* Primary data rate */

/* Final rate transmitted this frame. We save the value here for
   logging purposes since the value in txtc.rate is changed in the
   next encoder bin for building the next Tx frame */
extern enc_rate_type tx_frame_rate;

#ifdef FEATURE_IS2000_REL_A_SVD
extern byte *sec_fch_data_ptr;        /* Pointer to FCH secondary data */
extern word sec_fch_num_bits;         /* Num of bits in FCH secondary frame */

extern byte *sec_dcch_data_ptr;       /* Pointer to DCCH secondary data */
extern word sec_dcch_num_bits;        /* Num of bits in DCCH secondary frame */
#endif /* FEATURE_IS2000_REL_A_SVD */

extern byte *prim_dcch_data_ptr;      /* Pointer to DCCH primary data */
extern cai_data_rate_type prim_dcch_rate; /* Primary DCCH data rate */

/* Holding buffer for TX data on the reverse FCH*/
extern txc_tc_frame_type  txc_ftm_rfch_data;
extern enc_rate_type txc_ftm_rfch_rate;

/* Holding buffer for TX data on the reverse DCCH*/
extern txc_tc_frame_type  txc_ftm_rdcch_data;
extern enc_rate_type txc_ftm_rdcch_rate;
extern boolean txc_ftm_dtx_dcch;

/* Holding buffer for TX data on the reverse SCH*/
extern txc_tc_supp_frame_type  txc_ftm_rsch_data;
extern enc_sch_rate_type txc_ftm_rsch_rate;

/* Handoff Action Time information */
extern boolean txc_use_action_time;
  /* TRUE --- explicit action time;
   * FALSE --- implicit action time.
   */

extern boolean txc_second_frm_sig_enabled;
  /* When "txc_second_frm_sig_enabled" is TRUE, TXC will report
   * "MCC_TXC_FRAME_SIG" to MC at the next "TXC_INT_SIG" after
   * TXC sends the first "MCC_TXC_FRAME_SIG" so that MC can tune
   * to different frequency after the transmitter is turned off
   */

extern boolean txc_connect_priso;  /* for service option that associated with primary data */

#ifdef FEATURE_IS2000_REL_A_SVD
extern boolean txc_connect_secso;  /* for service option that associated with secondary data */
#endif /* FEATURE_IS2000_REL_A_SVD */

extern boolean txc_hho_preamble_pcg_units;
extern int16  tx_extra_gain;

#ifdef FEATURE_IS2000_REL_A_SVD
/* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
** frames when RLP is the secondary SO in a SVD call.  The purpose of the
** implementation is to improve voice quality in a SVD call.
*/
typedef struct
{
  dsrlp_tx_sig_enum_type prev_fr_val;
  dsrlp_tx_sig_enum_type curr_fr_val;
  word delay_fr_cnt;

} txc_rlp_sig_priority_type;

extern txc_rlp_sig_priority_type txc_rlp_sig_priority;
#endif /* FEATURE_IS2000_REL_A_SVD */

/* Number of frames to wait for Vocoder to act on a DIM request. */
#define TXC_VOC_DIM_DELAY 3

extern volatile uint8 txc_sig_frame_delay_value;
extern volatile uint8 txc_sig_frame_delay_for_tc_value;
extern volatile uint8 txc_sig_frame_delay_for_tt_value;

extern volatile uint8 txc_sig_quick_repeat_value;
extern volatile uint8 txc_sig_quick_repeat_for_tc_value;
extern volatile uint8 txc_sig_quick_repeat_for_tt_value;

extern volatile uint8 txc_tt_tx_adj_boost;

typedef struct
{
  boolean    ta_start;              /* Is TA started */
  boolean    ta_comp;               /* Is last TA completed */
  boolean    ta_tc_init_done;       /* TC_F processed before TX OPT jump */
  boolean    ta_hs_jump_complete;   /* Jump to hyperspace complete */
  int16      ta_rpc_skip_frames;    /* number of frames to skip rpc update */
  uint32     lta_start_sclk;        /* LTA start sclk time stamp */
  uint16     lta_gap_frames;        /* TX frames missed during LTA */
  int16      lta_ol_tx_pwr;         /* TX OL POWER saved before LTA start */

  /* Time logging */
  boolean    r_fch_ta_log;
  boolean    r_sch_ta_log;
  uint32     ta_start_sclk;
  uint32     r_fch_ta_comp_sclk;
  uint32     r_sch_ta_comp_sclk;

  /* Save Tx Adj step size before LTA starts and reuse after LTA compltes */
  cai_pwr_cntl_step_type tx_gain_adj_step_size;
} txc_ta_type;

extern txc_ta_type txc_ta_info;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TXC_TRAFFIC

DESCRIPTION
  Process traffic channel state

DEPENDENCIES
  None
RETURN VALUE
  Next state.
SIDE EFFECTS
  None
===========================================================================*/
extern txc_state_type txc_traffic( txc_state_type state );

/*========================================================================
FUNCTION TXC_RLGC_TRANS_CMD

DESCRIPTION
   The RLGC transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void txc_rlgc_trans_cmd( mctrans_action_packet_type *pkt_ptr );

/*========================================================================
FUNCTION TXC_HO_TRANS_CMD

DESCRIPTION
   The HO transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void txc_ho_trans_cmd( mctrans_action_packet_type *pkt_ptr );

/*========================================================================
FUNCTION TXC_SCR_TRANS_CMD

DESCRIPTION
   The SCR transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void txc_scr_trans_cmd( mctrans_action_packet_type *pkt_ptr );

/*===========================================================================

FUNCTION TXC_TRAFFIC_INIT

DESCRIPTION
  Initialization section for traffic channel.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
extern void txc_traffic_init( void );

/*========================================================================
FUNCTION TXC_REV_SCH_TRANS_CMD

DESCRIPTION
   The reverse SCH transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void txc_rev_sch_trans_cmd( mctrans_action_packet_type *pkt_ptr );

/*===========================================================================

FUNCTION TXC_TRAFFIC_ISR

DESCRIPTION
  Interrupt handler for the traffic channel.  Merge the vocoder and
  signaling data and give them to the encoder.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_traffic_isr
(
  m1x_stmr_event_type pcg
);

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION TXC_TX_PROCESS_FUNDICH_DATA3G

DESCRIPTION
  This function sets up all information needed for RLP to build data frames
  for R-FCH and/or R-DCCH channels.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_tx_process_fundich_data3g
(
  word,
  dsrlp_rate_enum_type,
  dsrlp_srvc_enum_type
);
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION TXC_TT_TC_INIT

DESCRIPTION
  When transition from TXC_TT_S to TXC_TRAFFIC_S, this function will be called
  to initialize the RF to be ready for traffic.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_tt_tc_init( boolean turn_on_tx );

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION TXC_FRAME_IS_GUARANTEED

DESCRIPTION
  This function determines if the current frame (based on frame number) is
  guaranteed based on the following criteria:

      - Current RC is 8 (1x Advanced)
      - Primary SO supports RL blanking.
      - R-SCH is not assigned (No RLP control frames);
      - (FRAME_NUMBER + FRAME_OFFSET) mod BLANKING_DUTY_CYCLE != 0

  A frame *MUST* be sent when any of the above criteria is FALSE.

DEPENDENCIES
  1x should already be in the traffic state.

RETURN VALUE
  True if the frame is a guaranteed frame, False otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean txc_frame_is_guaranteed
(
  uint32 frame_num,
  cai_fch_blanking_dutycycle_type duty_cycle
);

/*===========================================================================

FUNCTION TXC_PROCESS_RCPM_EVENT

DESCRIPTION
  This function is called whenever new RCPM data is provided by Call Processing.
  If various parameters of the RCPM change, this function will generate
  an event indicating this.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_process_rcpm_event
(
  cai_rcp_rxc_txc_type old_rcpm,
  cai_rcp_rxc_txc_type new_rcpm
);
#endif /* FEATURE_IS2000_1X_ADV */

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_RPCTRL_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated reverse power control frames log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A reverse power control log may be sent.

===========================================================================*/
void txc_rpctrl_send_log( void );

/*===========================================================================

FUNCTION TXC_SET_PRI_CHAN_CONFIG

DESCRIPTION
  Set the currently assigned primary traffic channel, includes physical channel,
  service option, etc.

    Physical channels
    FCH  0x01
    DCCH 0x02
    SCH  0x04


DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_set_pri_chan_config( uint16 phy_channels, uint16 so );

/*===========================================================================

FUNCTION TXC_GET_PRI_PHY_CHAN

DESCRIPTION
  Get the currently assigned primary physical channels for traffic.

  FCH  0x01
  DCCH 0x02
  SCH  0x04

DEPENDENCIES
  None

RETURN VALUE
  physical channel mask.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_pri_phy_chan( void );

/*===========================================================================

FUNCTION TXC_GET_PRI_SO

DESCRIPTION
  Get the currently assigned primary service option.

DEPENDENCIES
  None

RETURN VALUE
  primary service option

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_pri_so( void );

/*===========================================================================

FUNCTION TXC_SET_SEC_CHAN_CONFIG

DESCRIPTION
  Set the currently assigned secondary traffic channel, includes physical channel,
  service option, etc.

    Physical channels
    FCH  0x01
    DCCH 0x02
    SCH  0x04

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_set_sec_chan_config( uint16 phy_channels, uint16 so );

/*===========================================================================

FUNCTION TXC_GET_SEC_PHY_CHAN

DESCRIPTION
  Get the currently assigned secondary physical channels for traffic.

  FCH  0x01
  DCCH 0x02
  SCH  0x04

DEPENDENCIES
  None
RETURN VALUE
  physical channel mask.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_sec_phy_chan( void );


/*===========================================================================

FUNCTION TXC_GET_SEC_SO

DESCRIPTION
  Get the currently assigned secondary service option.

DEPENDENCIES
  None

RETURN VALUE
  secondary service option

SIDE EFFECTS
  None.
===========================================================================*/
uint16 txc_get_sec_so( void );

/*===========================================================================

FUNCTION TXC_IS_TEST_SO

DESCRIPTION
  Returns if the current SO is a test SO or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Test SOs.
  FALSE: Non-Test SOs.

SIDE EFFECTS
  None.
===========================================================================*/
boolean txc_is_test_so( void );

#ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
/*===========================================================================

FUNCTION TXC_DEVICE_ONLY_DTX_IS_ENABLED

DESCRIPTION
  Returns if the Device only DTX is enabled depending on the NV setting and the
  current SO. (NV# 70193)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  If overall device only DTX criteria is met.
  FALSE: Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean txc_device_only_dtx_is_enabled( void );

/*===========================================================================

FUNCTION TXC_DEVICE_ONLY_DTX_DECISION

DESCRIPTION
  This function decides if the current frame should be DTX'd for transmission.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void txc_device_only_dtx_decision( boolean sb_is_on );
#endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

#endif /*TXCTRAFFIC_I_H */
