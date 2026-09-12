/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              MUX mDSP interface drivers

GENERAL DESCRIPTION
  Contains implementation of the MUX mDSP drivers


EXTERNALIZED FUNCTIONS
  muxmdsp_get_pc_filt_gain
  muxmdsp_set_pc_filt_gain
  muxmdsp_get_nt_io_filt_gain
  muxmdsp_set_nt_io_filt_gain
  muxmdsp_get_eb_no_target_fch
  muxmdsp_get_eb_no_target_sch
  muxmdsp_get_sch_iq_acc
  muxmdsp_get_pc_acc
  muxmdsp_get_fch_nt_io
  muxmdsp_get_sch_nt_io
  muxmdsp_fpc_get_history
  muxmdsp_fpc_get_history_valid
  muxmdsp_rpc_get_history
  muxmdsp_rpc_get_history_valid
  muxmdsp_rpc_get_cell_history
  muxmdsp_get_fch_wtd_pilot
  muxmdsp_get_fch_rt_nt_io
  muxmdsp_get_sch_rt_nt_io
  muxmdsp_get_sch_wtd_pilot
  muxmdsp_update_fw_smem_addr
  muxmdsp_set_rc8_frame_config_parms
  muxmdsp_set_rc8_frame_config_parms_zero
  muxmdsp_wr_TxGainAdjStepSize
  muxmdsp_set_eb_no_target_fch
  muxmdsp_set_eb_no_target_sch
  muxmdsp_set_channel_gains
  muxmdsp_set_channel_gains_zero
  muxmdsp_set_tx_gain_adjust
  muxmdsp_set_tx_gain_adj_step_size
  muxmdsp_wr_tx_rate_adj_latch_offset
  muxmdsp_wr_fundFrameOffset
  muxmdsp_wr_fundFrameOffsetImmLoadCmd
  muxmdsp_wr_passThruEnable
  muxmdsp_set_agc_ctl_mask_delay
  muxmdsp_init
  muxmdsp_access_pwr_adjust
  muxmdsp_load_fw_rc11_rda_tables
  muxmdsp_rd_rc11_fch_rda_info
  muxmdsp_is_rc11_ffpc_frame_valid
  muxmdsp_update_rc11_params
  muxmdsp_set_rc11_mode
  muxmdsp_enable_1xA_logging
  muxmdsp_read_1xa_log_buffer
  muxmdsp_read_1xA_ack_log
  muxmdsp_fch_fpc_setpoint_is_frozen
  muxmdsp_fch_fpc_setpoint_freeze_config
  muxmdsp_rc8_max_power_protection_config
  muxmdsp_rl_ack_test_mode_config
  muxmdsp_enable_apf
  muxmdsp_enc_ch_en
  muxmdsp_set_enc_input_len
  muxmdsp_get_enc_input_len
  muxmdsp_set_frame_config_data_rate
  muxmdsp_set_enc_ch1_params
  muxmdsp_get_enc_ch1_params
  muxmdsp_set_enc_ch2_params
  muxmdsp_get_enc_ch2_params
  muxmdsp_set_enc_ch3_params
  muxmdsp_get_enc_ch3_params
  muxmdsp_set_rl_lc_state
  muxmdsp_get_rl_lc_state
  muxmdsp_set_rl_lc_mask
  muxmdsp_set_rl_spn_state
  muxmdsp_get_rl_spn_state
  muxmdsp_toggle_iram_index
  muxmdsp_set_mod_ch_cfg
  muxmdsp_set_tx_pcg_mask
  muxmdsp_send_frame_config
  muxmdsp_set_txfe_trigger
  muxmdsp_stop_txfe
  muxmdsp_get_lmem_ch_addr
  muxmdsp_update_fw_diag_buf
  muxmdsp_start_flex_capture

  Copyright (c) 2003 - 2020 Qualcomm Technologies, Inc.
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

                              Edit History

$Header: //components/rel/1x.mpss/8.0/mux/src/muxmdsp.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
01/19/18   eye     F3 logging reduction.
03/07/17   sjo     Disable logging Key info based on return value of API.
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
04/07/16   eye     Fixed QTA Tx power shoot up issue after tune back.
03/28/16   pk      Fixed RUMI compilation error
03/21/16   eye     Adjust tx trigger for RTC rollover scenario.
02/05/16   eye     1x2GTA Feature changes
12/24/15   eye     Added back FLEX addresses for TX RUMI capture.
11/06/15   srk     Fix potential saturation of I/Q symbols.
10/06/15   srk     DRX SRCH response deadlock design change and cleanup.
09/29/15   jh      Disable DRX during DEMOD_FRAME_OFFSET
08/14/15   srk     ATLAS pre-SI bringup changes - Flex capture.
06/24/15   srk     1x SW support for 1x FW DEMBACK changes in ATLAS
06/22/15   eye     Dont disable data bridge if SW DEMBACK is active.
06/08/15   eye     Added CSFB check for demod ch0 FW update.
06/02/15   srk     Revert the previous change.
06/01/15   sst     Disable SW Demback for FTM/NS modes
05/05/15   ab      Soft Demback changes for QTA
03/18/15   eye     Clear muxmdsp_dac_trigger in Tx disable part of FTM mode.
02/27/15   jh      Updates to work on Jolokia
02/12/15   jh      Add muxmdsp_extract_ch0_syms() API
                   Add blocking response handling for DEMBACK_SW_CONFIG
                   Add muxmdsp_set_demback_mode() API
01/06/15   srk     Wait for TX start/stop response before sending ASDIV command
11/07/14   srk     Register for FW response for DAC START for UTX targets.
10/30/14   jh      RF Cal crash in FTM mode
10/27/14   srk     Fix NULL pointer access crash in RF FTM mode.
10/27/14   srk     Add FW active check before sending RL_LEGACY_RC_FRAME_CONFIG
10/24/14   jh      Remove unused API
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
09/05/14   srk     Wait for FW response for the DAC stop in UTX targets.
08/15/14   as      Clean up rlFrameConfigParams for Jolokia
07/11/14   as      Clean up Jolokia revisits
06/12/14   vks     Do not send RL_FRAME_CFG message if past PCG 12
05/29/14   pap     Redistributing messages.
05/28/14   as      Jolokia compile clean-up
05/20/14   eye     Add delay of 300us after sending FPC command to FW.
03/27/14   vks     Set tdec mode register only after asking FW to setup tdec
02/13/14   dkb     Add support for optimized Tx trigger.
01/08/14   ab      DDR power management Support / Feature Cleanup
02/03/14   dkb     Feature guard Triton UTX code under FEATURE_MODEM_1X_UTX
01/28/14   eye     Set PCG mask to correct DTX/SB mask if the frame is valid.
12/18/13   cjb     Do not send redundant TXFE trigger/stop requests to FW
12/07/13   cjb     Changes to support DEMBACK_1
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature.
                   Mainlined FEATURE_MODEM_1X_TRITON.
11/21/13   vks     Add api to return tdec decob address
11/14/13   dkb     Change ovsf value for 2X SCH to correct value.
11/14/13   dkb     Clean up enum issues and correct erroneous comments.
11/05/13   cjb     Set PCG mask to 0xFFFF to not gate Tx if the frame is valid
11/04/13   dkb     Clean up 1x RL and remove unsupported FW messages.
11/01/13   dkb     Clean up enc test waveform and reduce the frequency of
                   sending frame config to FW.
10/24/13   dkb     Add RL clean up and flag to disable bit reversing LC.
10/24/13   pap     Removing all warnings for LLVM compiler.
10/24/13   dkb     Add changes to address FL/RL discrepencies.
10/16/13   dkb     Add changes for R-FCH functionality without FL dependency.
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
10/09/13   dkb     Add additional RL changes.
10/08/13   vks     Update to new FW interface - fing config has the walsh code
                   and qof
10/07/13   dkb     Update 1x Tx waveform for new FW interface.
10/03/13   srk     Add support for Device only DTX for Full Rate frames
09/30/13   dkb     Initial RL changes for new FW interface.
09/26/13   dkb     Add support for FW diag buffer
09/13/13   vks     Add api to update code beta for demod channels
09/11/13   vks     Add api to update ch0,ch1,ch2,fpc,rpc and frame offset
09/06/13   srk     Remove unused APIs and global variables
08/27/13   eye     Add muxmdsp_init() for module initialization.
08/21/13   srk     Use the correct flags to set the DTX mask
07/18/13   srk     De-couple 1xA smart blanking from coex manager.
06/26/13   eye     Removed support for Device only DTX for RC4.
06/25/13   eye     Moved DSDA feature check for Device Only DTX.
06/19/13   eye     Clean up Device Only DTX changes.
06/04/13   ab      Cleanup coex manager changes
05/14/13   eye     Unifying Device only DTX for RC3/4 and RC8.
05/22/13   trc     Add TDEC busy spin-drain loop before disabling bridge
05/15/13   ab      1X coex manager changes
05/06/13   cjb     Resolve compiler warnings
03/08/13   cjb/trc Added detailed logging for RTC MOD TX and MOD TX SYS TIME.
03/05/13   cjb     Cleaned-up TRITON revisit.
02/18/13   srk     AND the ACK mask against PCG mask to ensure proper ACK bhvr
                   for both DTX and SB.
02/15/13   srk     Replace mcs hwio with m1x hwio
01/17/13   bph     KW and compiler warning fixes
01/16/13   srk     Add transmit PCG mask to support device only DTX in
                   legacy RCs.
01/03/13   srk     Enable PA/TX ON gating for all RCs.
01/03/13   srk     Enable PA/TX ON gating for 1x Advanced traffic.
12/06/12   trc     Remove unused variable to suppress compiler warning
12/04/12   trc     AND the ACK mask against DTX mask to ensure proper ACK bhvr
11/13/12   trc     Layering/Cleanup for Triton
11/09/12   srk     Remove old interface for SB and use the new interface for
                   both SB and DTX
11/08/12   cjb     Making previous change backward compatible with Triton.
11/07/12   cjb     Switched to new FW interface which takes care of configuring
                   DEMBACK_MODE_SEL to proper values based on the active TECHs.
10/30/12   srk     Combine multiple debug messages into a single debug F3.
10/19/12   srk     Create an API to read frame sync'ed tx gain adjust from FW.
10/19/12   srk     Use common pilot gating mask for SB and DTX.
10/18/12   srk     Use 1xA debug F3s instead of diag commands.
10/18/12   srk     Do DTX only when the NV is set properly.
10/04/12   srk     Fix compiler warnings.
09/14/12   jtm     Feature cleanup.
09/11/12   srk     Remove DTX frame flag and DTX t2p gain ratio boost and set
                   DTX mask to 0xFFFF when DTX is disabled for the frame.
08/27/12   srk     Apply T2P gain ratio boost of tx power boost.
08/23/12   srk     Stop DTX when SCH is active.
08/20/12   vks     Fix compiler warnings.
08/16/12   jtm     Added TDEC bridge MSGR command.
08/02/12   srk     Added support for Device only DTX for 1x Advanced calls.
05/30/12   srk     Changes to remove half PCG offset in 1x advanced logs.
04/16/12   jtm     Moved QOF FCH Noise Estimation to SRCH.
03/26/12   srk     SW delivers the mask to FW based on RL SB being on/off for
                   determining early decode, but deliver RDA table based on FL
                   SB being on/off.
03/11/12   trc     Expand Tx timing dump to include VSRC/STMR diff
03/07/12   jtm     Load serFETThreshold table to shared memory.
02/24/12   vks     Revert previous change (CR330852 fix).
01/16/12   trc     Treat CAI_INIT_RC as RC1/2 for FW
01/11/12   srk     Feature Cleanup
12/01/11   srk     Added muxmdsp_config_qof_fch_noise_est and
                   muxmdsp_load_fw_rc11_rda_tables functions for Qof FCH Noise
                   Estimation support and RC11 Enhanced RDA respectively.
11/30/11   trc     Clean up some FW API usage
11/04/11   trc     Add muxmdsp_print_tx_timing_info() debug API
09/29/11   srk     Ack detect changed from 0xFFFF to 0x0001.
                   Log index is updated to read previous log buffer.
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/29/11   trc     Add cx8 FW interface for TX_DAC_START command
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API
08/04/11   jtm     Moved rliFPCMask programming to ffpc.c
07/28/11   vks/jtm Changes to get 1xA working on nikel.
06/20/11   jtm     Wrap APF support with FEATURE_IS2000_1X_ADV.
06/17/11   jtm     Added support to enable/disable Adaptive Pilot Filtering (APF)
06/13/11   adw     Added function to send Tx DAC stop message to FW.
06/09/11   vks     Check for valid device before obtaining tx closed loop power
06/03/11   adw     SV support.
05/09/11   trc     Add NikeL FW support
05/03/11   jtm     Temporarily use FEATURE_MODEM_1X_NIKEL_REVISIT to
                   compile out the use of the FW 1/2 pcg log buffer.
04/28/11   trc     NikeL Tx support
04/27/11   jtm     Expand MSGR support between Mux and FW.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/26/11   jtm     Use mdsp_read_offset to retrieve RPC cell history.
04/05/11   vks     Use new api for RF SVDO feature.
03/28/11   jtm     Removed deprecated RF includes.
02/24/11   vks     Check mdsp running before sending out a message.
02/03/11   vks     Fix featurization.
02/02/11   vks     Featurize RC8 config variables in muxmdsp_wr_rliRC8Enable
                   to use for Genesis only (nikel definitions have changed for
                   this).
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/25/11   vks     Update to new RF SVDO api for nikel
12/15/10   vks     Temporarily featurize register accesses for Nikel.
11/24/10   ag      Renamed pwr_cntl_step_zero_rate to pwr_cntl_step.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/23/10   jtm     Eliminated obsolete DV support. Changed macros to functions.
08/03/10   jtm     Fixed RC8 R-SCH linear gain ratio programming to FW.
07/01/10   jtm     Only include rf_mdsp_tx_agc.h if FEATURE_1X_SUPPORTS_MSGR
                   is *not* defined.
06/29/10   jtm     Fixed the agc control mask delay to provide the
                   correct value to FW when message router is used.
06/15/10   jtm     Cleaned up 1xA Logging. Added new 1xA Perf Log format and
                   FET Stats log. ACK/NAK test mode support added.
06/10/10   jtm     Eliminate dependency on log_dmss.h
06/09/10   jtm     Fixed merge and compiler errors.
05/27/10   jtm     Maintain a local copy of the gain adjust value to prevent
                   reduntant mDSP programming.
05/21/10   jtm     Corrected muxmdsp_set_tx_gain_adjust() to prevent a
                   potential race condition between FW and SW when not using
                   the MSGR interface.
05/19/10   vks     With the new MSGR interface, L1 needs to send FW a message
                   directly for the tx gain adj step size.
05/18/10   vks     Mainline FEATURE_MDSP_SUPPORTS_TX_AGC.
05/17/10   vks     Change api to use new RF api.
05/14/10   vks     Add function for setting agc ctl mask delay. This wrapper
                   function uses FW msg for MSGR interface, legacy MUXMSM_*
                   macro for legacy.
04/22/10   vks     Remove references to the hard coded fw shared memory addr.
                   Update MUX Mdsp module with the addr when app enabled.
04/12/10   jtm     Added clearing of CDMA1X_rliLogConfig when disabling
                   1xA FW logging so log are frame aligned when re-enabled.
                   Added FCH FPC setpoint freeze and RC8 MPP support.
                   Added precompiler check for CDMA1X_MOD_PRMBL_GAIN*
                   definitions before attempting to use them.
                   Provided correct masking for FL Ack masks prior to writing
                   them to FW. Removed frame number print out from
                   muxmdsp_read_1xA_ack_log().
03/25/10   vks     Send FW a message directly instead of using RF macros for
                   tx gain adjust override functionality.
03/04/10   vks     Call RF-FW update macro everytime a FW register is touched
                   using RF macros
12/09/09   jtm     Updated 1xA logging mechanism to align PCGs of each frame.
                   Check for 1xA FW support so the correct FW variable is used
                   for gain pass through capability.
12/01/09   vks     Remove NOTUSED definition. Use the MODEM_1X_UNUSED macro
                   defined in modem_1x_utils.h instead.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/12/09   vks     Add MSGR/Q6 framework support.
11/11/09   vks     Temporarily bring back txc_sch_gain for backwards
                   compatability.
11/07/09   vks     Created a global data structure to contain all the channel
                   gains. Export API to set these variables.
10/01/09   jtm     Added 1xA logging support
08/26/09   jtm     Update to write FL early decode PCG mask using values
                   provided in the RCPM.
07/29/09   jtm     Replace CDMA1X_rliPCPattern with new mDSP variables
                   CDMA1X_rliFPCMask and CDMA1X_rliRPCMask.
07/06/09   jtm     Update to use cai_rcp_rxc_txc_type.
06/11/09   jtm     Additional 1x Advanced RL support.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
04/08/09   jtm     Initial 1X Advanced feature support.
04/01/09   jtm     Featurized necessary includes.
03/27/09   jtm     Cleaned up includes.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code.
07/01/08   vlc     Merged code changes from old tip archive.
02/23/08   vlc     Added new interface to MDSP to notify MDSP of entrance and
                   exit of RL access state mainly to fine tune access probe
                   power management (FEATURE_ACCESS_PROBE_PWR_ADJ.)
01/09/07   vlc     Merged the following change to main line from SC2X branch.
                   Mainly, adding support for FEATURE_MDSP_SUPPORTS_TX_AGC.
           vm      Added the support for Firmware TX
09/05/06   vlc     Implemented TX gain enhancements to increase digital gain
                   resolution.
06/09/06   trc     Remove T_MSM6700 featurization
03/14/05   vlc     Fixed compiler warnings for MSM6700.
03/07/05   bn      Cleaned up debug F3 messages
12/09/04   jyw     Refeatureized to seperate the DV code and 1x code
                   under T_MSM6700
11/08/04   sr      Added muxmdsp_wr_pdcchStartDelay()
09/14/04   jrp     Update calls to DV_INFO header.
09/10/04   ljl     Added the function muxmdsp_wr_rlGainPdchMode().
09/01/04   jrp     Fixed Nak counter offsets.  Added counters for 0.5x rates.
                   Fixed compilation error for 6700 builds.
09/01/04   sr      Added handshaking between ARM and mDSP in
                   muxmdsp_dv_chan_disable()
09/01/04   jrp     Conditionally compile entire file.
08/30/04   jrp     Corrected offsets for statistics counters.
08/20/04   jrp     Added support for hidra dme buffer.
08/16/04   sr      Replaced mdsp_read() and mdsp_write() macros with
                   mdsp_read_offset() and mdsp_write_offset(), respectively,
                   for indexed reads and writes to mDSP registers
08/13/04   ljl     Added to enable R-PICH for CQI/ACK override PLT tests.
08/02/04   jrp     Changed dme base memory address for new target.
07/30/04   vlc     Added support for rel C TDSO.
                   Removed obsolete muxmdsp_rd_ackHistoryValid() and
                   muxmdsp_rd_ackHistoryState().
07/28/04   jyw     Fixed the bug in the muxmdsp_wr_pdchInitialDescrambleState
07/28/04   jrp     Renamed Rev D gain enums.
07/27/04   jrp     Moved non-DV functions outside ifdef.
07/26/04   bn      Added new function for tx_rate_adj latched frame offset
07/23/04   jyw     Modifed the interface of function muxmdsp_wr_pdchInitialDescrambleState
07/23/04   jyw     moved the function call of muxmdsp_wr_pdchInitialDescrambleState from
                   muxmdsp_dv_ctrl_init() to pdch_init();
07/21/04   bn      Added function for MDSP frame offset loading command
07/20/04   bn      Indicate to the MDSP to load the rev frame offset immediately
07/15/04   jrp     Added calls to muxmdsp_wr_passThruEnable() on DV channel
                   enable and disable.
06/29/04   jrp     Added pass-through and init functions.
06/24/04   sr      Updated muxmdsp_wr_walshinfo(), dme base address & fixed
                   some typos in decob interface functions
06/18/04   bn      Added support for Rel D channel gains override
05/19/04   sr      Updated to latest mDSP firmware and cleaned up warnings
05/04/04   bn      Added the MDSP to ARM universal INT handler and registering utilizes
03/17/04   bn      Modified the register callback function to allow deregistering
03/09/04   sr/bkm  Featurized DV code
09/09/03   bn      Created MUX mDSP interface file.

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "m1x_diag.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//cp
#include "cai.h"
#include "cai_v.h"
#include "mc_i.h"
#include "mccqsh_i.h"

//drivers
#include "dalint_1x.h"
#include "dec5000_v.h"
#include "deint_i.h"
#include "is2k_fl_util.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "m1x_hwio_mpss.h"
#include "enc_i.h"

//mux
#include "ffpc_i.h"
#include "mux.h"
#include "muxmdsp_i.h"
#include "muxmsgr.h"
#include "rlgc.h"
#include "rlgc_i.h"
#include "txccommon_i.h"
#include "txctraffic_i.h"
#include "rxctraffic.h"
#include "txc.h"

//diag
#include "cdma2kdiagi.h"
#include "onex_non_sig.h"

//srch
#include "srch_comb.h"
#include "srch_mux.h"
#ifdef FEATURE_MODEM_1X_COEXISTENCE
#include "srch_coex.h"
#endif /* FEATURE_MODEM_1X_COEXISTENCE  */
#include "srchmc_v.h"

//other
#include "msg.h"
#include "msgr_types.h"
#include "msgr.h"
#include "hexagon_protos.h"

//feature dependent
#include "cdmafw_msg.h"

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
#include "rfm_1x.h"
#include "rfm_device_types.h"
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Critical Section */
#define MUXMDSP_ENTER_CRITICAL_SECTION()  rex_enter_crit_sect(&muxmdsp_crit_sect)
#define MUXMDSP_LEAVE_CRITICAL_SECTION()  rex_leave_crit_sect(&muxmdsp_crit_sect)

#define MUXMDSP_10_TO_16_BIT( val )      ( val << 6 )

/* MDSP channel gains */
muxmdsp_channel_gains_type channel_gains = {0,0,0,0,0};

#ifdef FEATURE_IS2000_1X_ADV
muxmdsp_rc8_frame_config_parms rc8_frame_config_parms;

typedef enum
{
  RC11_FW_RDA_SB_ENABLED = 0,
  RC11_FW_RDA_SB_DISABLED = 1,
  RC11_FW_RDA_NUM_OPTIONS
} muxmdsp_rc11_fw_rda_mode_type;

#define RC11_FW_RDA_SLOPE_THRESH_SIZE         (CDMAFW_NUM_RATE_HYPOTHESES * CDMAFW_NUM_DECODE_ATTEMPTS * CDMAFW_NUM_RATE_HYPOTHESES)
#define RC11_FW_RDA_INTERCEPT_THRESH_SIZE     (CDMAFW_NUM_RATE_HYPOTHESES * CDMAFW_NUM_DECODE_ATTEMPTS * CDMAFW_NUM_RATE_HYPOTHESES)
#define RC11_FW_RDA_ENERGY_SELF_THRESH_SIZE   (CDMAFW_NUM_RATE_HYPOTHESES * CDMAFW_NUM_DECODE_ATTEMPTS)
#define RC11_FW_RDA_SER_THRESH_SIZE           (CDMAFW_NUM_RATE_HYPOTHESES * CDMAFW_NUM_DECODE_ATTEMPTS)
#define RC11_FW_RDA_FRAME_END_SER_THRESH_SIZE (CDMAFW_NUM_RATE_HYPOTHESES)

static const struct
{
  uint16 slopeThreshold      [ RC11_FW_RDA_SLOPE_THRESH_SIZE ];
  uint16 interceptThreshold  [ RC11_FW_RDA_INTERCEPT_THRESH_SIZE ];
  uint16 energySelfThreshold [ RC11_FW_RDA_ENERGY_SELF_THRESH_SIZE ];
  uint16 serThreshold        [ RC11_FW_RDA_SER_THRESH_SIZE ];
  uint16 serFETThreshold     [ RC11_FW_RDA_SER_THRESH_SIZE ];
  uint16 frameEndSerThrsh    [ RC11_FW_RDA_FRAME_END_SER_THRESH_SIZE ];
}
rc11_fw_rda_tables[ RC11_FW_RDA_NUM_OPTIONS ] =
{
  {
    // [ RC11_FW_RDA_SB_ENABLED ].slopeThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000F, 0x0010, 0x0013,
      0x000F, 0x0000, 0x000C, 0x001C,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000A, 0x000B, 0x0017,
      0x000D, 0x0000, 0x000D, 0x0016,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000D, 0x000C, 0x0013,
      0x000D, 0x0000, 0x000C, 0x0015,
      0x0009, 0x000E, 0x0000, 0x001A,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000C, 0x000A, 0x0017,
      0x000C, 0x0000, 0x000D, 0x0010,
      0x001B, 0x000D, 0x0000, 0x0015,
      0x0000, 0xFFFE, 0x0000, 0x0000,
      0x0000, 0x000E, 0x000D, 0x0011,
      0x000C, 0x0000, 0x000D, 0x0016,
      0x0009, 0x000D, 0x0000, 0x0013,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000C, 0x000B, 0x000F,
      0x000D, 0x0000, 0x000B, 0x000F,
      0x000C, 0x0010, 0x0000, 0x0012,
      0x0018, 0x0017, 0x0012, 0x0000,
      0x0000, 0x000D, 0x000B, 0x000C,
      0x000B, 0x0000, 0x000C, 0x000A,
      0x000C, 0x000C, 0x0000, 0x0010,
      0x0014, 0x0011, 0x0010, 0x0000,
      0x0000, 0x000C, 0x000C, 0x000E,
      0x000C, 0x0000, 0x000C, 0x000C,
      0x0010, 0x000F, 0x0000, 0x0010,
      0x0015, 0x0014, 0x0010, 0x0000,
      0x0000, 0x000C, 0x000B, 0x0008,
      0x000E, 0x0000, 0x000F, 0x000C,
      0x0010, 0x0017, 0x0000, 0x000F,
      0x0013, 0x0011, 0x0011, 0x0000,
      0x0000, 0x000A, 0x000A, 0x0008,
      0x000F, 0x0000, 0x000C, 0x0010,
      0x000D, 0x000F, 0x0000, 0x000F,
      0x0013, 0x000E, 0x0011, 0x0000,
      0x0000, 0x000A, 0x000A, 0x0008,
      0x000C, 0x0000, 0x000B, 0x000B,
      0x0011, 0x000C, 0x0000, 0x000F,
      0x0012, 0x0010, 0x0011, 0x0000,
      0x0000, 0x000A, 0x0007, 0x0009,
      0x0014, 0x0000, 0x0012, 0x0007,
      0x0010, 0x000E, 0x0000, 0x000A,
      0x0012, 0x0014, 0x0012, 0x0000,
      0x0000, 0x000C, 0x000A, 0x0009,
      0x0013, 0x0000, 0x000D, 0x000D,
      0x0018, 0x0018, 0x0000, 0x000D,
      0x001B, 0x0019, 0x0014, 0x0000
    },
    // [ RC11_FW_RDA_SB_ENABLED ].interceptThreshold =
    {
      0x0000, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x0000, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x0000, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0xFFFE, 0xFFFC, 0x000E,
      0x0040, 0x0000, 0x0053, 0xFFF7,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x0037, 0x000F, 0xFFF7,
      0x0067, 0x0000, 0x0024, 0x0066,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x0011, 0xFFF4, 0xFFE2,
      0x006A, 0x0000, 0x0032, 0x005E,
      0x00D5, 0x005D, 0x0000, 0x0055,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x0013, 0x001B, 0xFF94,
      0x0071, 0x0000, 0x0013, 0x00C7,
      0xFFB1, 0x0084, 0x0000, 0xFF31,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x0005, 0xFFE4, 0xFFB5,
      0x007C, 0x0000, 0x001A, 0xFFFA,
      0x00FA, 0x009C, 0x0000, 0xFF91,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x001E, 0xFFFE, 0xFFDA,
      0x0083, 0x0000, 0x003B, 0x0035,
      0x00D4, 0x004B, 0x0000, 0x008C,
      0x001E, 0xFFD6, 0xFFD4, 0x0000,
      0x0000, 0xFFF7, 0xFFE0, 0xFFD5,
      0x009B, 0x0000, 0x0018, 0x0053,
      0x00DB, 0x00B0, 0x0000, 0x0010,
      0x00BA, 0x00AB, 0x003F, 0x0000,
      0x0000, 0x0018, 0xFFDB, 0xFF7F,
      0x008F, 0x0000, 0x0027, 0xFFEE,
      0x0098, 0x0060, 0x0000, 0xFF96,
      0x008F, 0x003E, 0x0054, 0x0000,
      0x0000, 0x001A, 0xFFF0, 0x0030,
      0x0092, 0x0000, 0xFFC3, 0xFFCD,
      0x009B, 0xFFA0, 0x0000, 0xFFC1,
      0x00BF, 0x00A5, 0x0031, 0x0000,
      0x0000, 0x0034, 0xFFF5, 0x000B,
      0x0064, 0x0000, 0x0034, 0xFED5,
      0x00E9, 0x0052, 0x0000, 0xFF50,
      0x00D7, 0x0106, 0x0052, 0x0000,
      0x0000, 0x005E, 0x0014, 0x0010,
      0x00AB, 0x0000, 0x0046, 0xFFD6,
      0x007B, 0x00B7, 0x0000, 0xFF6A,
      0x0104, 0x00C4, 0x0040, 0x0000,
      0x0000, 0x0049, 0x007A, 0xFFF2,
      0x0062, 0x0000, 0xFFA8, 0x00D9,
      0x00C9, 0x009B, 0x0000, 0x00DA,
      0x0125, 0x0041, 0x0022, 0x0000,
      0x0000, 0xFFF7, 0xFFE7, 0xFFAB,
      0x001D, 0x0000, 0xFFF9, 0xFF78,
      0x0029, 0xFF90, 0x0000, 0x0010,
      0x0089, 0x001D, 0x0016, 0x0000
    },
    // [ RC11_FW_RDA_SB_ENABLED ].energySelfThreshold =
    {
      0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF,
      0x00D9, 0x0195, 0x3FFF, 0x3FFF,
      0x00D7, 0x0101, 0x3FFF, 0x3FFF,
      0x00EE, 0x0116, 0x0248, 0x3FFF,
      0x012D, 0x0137, 0x0480, 0x3FFF,
      0x0104, 0x0181, 0x0486, 0x3FFF,
      0x0113, 0x03A1, 0x041D, 0x0DD2,
      0x0128, 0x01B2, 0x0225, 0x0672,
      0x015B, 0x0226, 0x0320, 0x0500,
      0x0169, 0x01A3, 0x02E2, 0x04B0,
      0x0179, 0x01C0, 0x041B, 0x050D,
      0x0150, 0x01A9, 0x028E, 0x04E4,
      0x011A, 0x0160, 0x02B0, 0x04B8,
      0x00E6, 0x0118, 0x01F4, 0x0316
    },
    // [ RC11_FW_RDA_SB_ENABLED ].serThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0008, 0x0000, 0x0000, 0x0000,
      0x000B, 0x0003, 0x0000, 0x0000,
      0x0008, 0x0006, 0x0000, 0x0000,
      0x000A, 0x000D, 0x0004, 0x0000,
      0x000A, 0x0010, 0x0008, 0x0000,
      0x0009, 0x0010, 0x000B, 0x0000,
      0x000A, 0x0010, 0x000C, 0x0004,
      0x000A, 0x0010, 0x0011, 0x0007,
      0x000B, 0x000E, 0x0012, 0x0009,
      0x000B, 0x000F, 0x0013, 0x0010,
      0x000B, 0x000F, 0x001B, 0x0016,
      0x000B, 0x000D, 0x0018, 0x0021,
      0x000C, 0x000E, 0x0015, 0x0019,
      0x000B, 0x0011, 0x0015, 0x001C
    },
    // [ RC11_FW_RDA_SB_ENABLED ].serFETThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000F, 0x0004, 0x0000,
      0x0000, 0x0013, 0x000A, 0x0000,
      0x0000, 0x0012, 0x000A, 0x0000,
      0x0000, 0x0010, 0x0012, 0x0005,
      0x0000, 0x0013, 0x0015, 0x000C,
      0x0000, 0x0010, 0x0016, 0x000E,
      0x0000, 0x0013, 0x001C, 0x0017,
      0x0000, 0x0013, 0x001D, 0x0021,
      0x0000, 0x0011, 0x001E, 0x002A,
      0x0000, 0x0011, 0x001C, 0x0034,
      0x0000, 0x0000, 0x0000, 0x0000
    },
    // [ RC11_FW_RDA_SB_ENABLED ].frameEndSerThrsh =
    {
      0x000D, 0x0014, 0x0021, 0x0036
    }
  },
  {
    // [ RC11_FW_RDA_SB_DISABLED ].slopeThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000E, 0x000E, 0x0014,
      0x000E, 0x0000, 0x000E, 0x001C,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000C, 0x000C, 0x001A,
      0x000B, 0x0000, 0x000D, 0x0017,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000E, 0x000B, 0x0019,
      0x000D, 0x0000, 0x000D, 0x0013,
      0x0011, 0x0012, 0x0000, 0x0011,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000D, 0x000B, 0x0013,
      0x000D, 0x0000, 0x000E, 0x0015,
      0x000E, 0x000E, 0x0000, 0x001E,
      0x0000, 0xFFFE, 0x0000, 0x0000,
      0x0000, 0x000F, 0x000E, 0x0014,
      0x000E, 0x0000, 0x000E, 0x0015,
      0x0012, 0x0011, 0x0000, 0x0011,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x000B, 0x000B, 0x0012,
      0x000E, 0x0000, 0x000D, 0x0017,
      0x000F, 0x000F, 0x0000, 0x0013,
      0x0017, 0x0014, 0x0010, 0x0000,
      0x0000, 0x000C, 0x000B, 0x000B,
      0x000F, 0x0000, 0x000D, 0x000B,
      0x0016, 0x0011, 0x0000, 0x000F,
      0x0016, 0x0013, 0x0014, 0x0000,
      0x0000, 0x000F, 0x000C, 0x000E,
      0x000F, 0x0000, 0x000D, 0x000A,
      0x0010, 0x000F, 0x0000, 0x0010,
      0x0015, 0x0011, 0x000F, 0x0000,
      0x0000, 0x000E, 0x000A, 0x0008,
      0x000C, 0x0000, 0x000D, 0x000B,
      0x0018, 0x0012, 0x0000, 0x0010,
      0x0017, 0x0013, 0x0014, 0x0000,
      0x0000, 0x000C, 0x000C, 0x000A,
      0x000E, 0x0000, 0x000D, 0x000E,
      0x0012, 0x0011, 0x0000, 0x0011,
      0x0013, 0x000E, 0x0011, 0x0000,
      0x0000, 0x000D, 0x000C, 0x000C,
      0x0011, 0x0000, 0x000C, 0x000B,
      0x0014, 0x0013, 0x0000, 0x000F,
      0x0015, 0x0017, 0x0012, 0x0000,
      0x0000, 0x000D, 0x000E, 0x000A,
      0x000F, 0x0000, 0x0011, 0x000A,
      0x0013, 0x0011, 0x0000, 0x000C,
      0x0013, 0x0015, 0x0012, 0x0000,
      0x0000, 0x000E, 0x000C, 0x000A,
      0x000E, 0x0000, 0x000C, 0x000A,
      0x0015, 0x0012, 0x0000, 0x000D,
      0x001B, 0x0012, 0x000F, 0x0000
    },
    // [ RC11_FW_RDA_SB_DISABLED ].interceptThreshold =
    {
      0x0000, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x0000, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x0000, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0xFFFC, 0xFFF1, 0xFFD8,
      0x003E, 0x0000, 0x001C, 0xFFF7,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x001E, 0xFFFF, 0xFF86,
      0x0069, 0x0000, 0x0024, 0x0020,
      0x3FFF, 0x3FFF, 0x0000, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0xFFF6, 0x0005, 0xFF35,
      0x0060, 0x0000, 0x0010, 0x0056,
      0x007C, 0x001D, 0x0000, 0xFFD1,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0xFFFC, 0xFFFA, 0xFFBA,
      0x0064, 0x0000, 0x0008, 0x0035,
      0x0096, 0x0065, 0x0000, 0xFF80,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0xFFD9, 0xFFB9, 0xFF6D,
      0x0052, 0x0000, 0xFFF2, 0xFFD2,
      0x006E, 0x0033, 0x0000, 0xFFD8,
      0x3FFF, 0x3FFF, 0x3FFF, 0x0000,
      0x0000, 0x0024, 0xFFE7, 0xFF7D,
      0x0058, 0x0000, 0x0011, 0xFFA0,
      0x00A4, 0x0057, 0x0000, 0x0048,
      0x00B3, 0x007B, 0x0081, 0x0000,
      0x0000, 0xFFFB, 0xFFE2, 0xFFBB,
      0x005A, 0x0000, 0x0007, 0x0046,
      0x003D, 0x003F, 0x0000, 0xFFB8,
      0x00B3, 0x009B, 0xFFDF, 0x0000,
      0x0000, 0xFFDC, 0xFFC1, 0xFF47,
      0x005D, 0x0000, 0xFFFB, 0x005D,
      0x0086, 0x0060, 0x0000, 0xFF96,
      0x008F, 0x00C4, 0x008E, 0x0000,
      0x0000, 0xFFE4, 0xFFDD, 0x0012,
      0x0085, 0x0000, 0xFFE8, 0x0049,
      0x0014, 0x002C, 0x0000, 0xFFB5,
      0x0095, 0x0093, 0xFFF3, 0x0000,
      0x0000, 0xFFEF, 0xFFBA, 0xFF67,
      0x0058, 0x0000, 0x0002, 0xFF44,
      0x008C, 0x003E, 0x0000, 0xFF65,
      0x00D7, 0x0106, 0x0052, 0x0000,
      0x0000, 0xFFE7, 0xFF9F, 0xFF13,
      0x002A, 0x0000, 0x0022, 0xFFD0,
      0x004D, 0x001D, 0x0000, 0xFF6A,
      0x00C8, 0x0032, 0x0045, 0x0000,
      0x0000, 0xFFD4, 0xFF53, 0xFFA3,
      0x0055, 0x0000, 0xFF44, 0x0013,
      0x0088, 0x0038, 0x0000, 0x005D,
      0x00EF, 0x0058, 0x0022, 0x0000,
      0x0000, 0xFFB3, 0xFF97, 0xFF50,
      0x004C, 0x0000, 0x0027, 0xFFEF,
      0x0040, 0x001B, 0x0000, 0xFFC5,
      0x007D, 0x00A4, 0x00A2, 0x0000
    },
    // [ RC11_FW_RDA_SB_DISABLED ].energySelfThreshold =
    {
      0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF,
      0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF,
      0x00C0, 0x00FF, 0x3FFF, 0x3FFF,
      0x00BC, 0x0150, 0x3FFF, 0x3FFF,
      0x0108, 0x01C2, 0x03F9, 0x3FFF,
      0x010B, 0x015B, 0x0317, 0x3FFF,
      0x0122, 0x019F, 0x041A, 0x3FFF,
      0x00F7, 0x01C4, 0x0264, 0x0B64,
      0x012C, 0x01A4, 0x0344, 0x05DC,
      0x0136, 0x01C0, 0x0303, 0x0626,
      0x0137, 0x01B9, 0x0286, 0x0457,
      0x0156, 0x01CD, 0x0291, 0x04E4,
      0x012C, 0x01A9, 0x0292, 0x042E,
      0x018E, 0x019E, 0x02A8, 0x03FF,
      0x0103, 0x00FA, 0x01C2, 0x02BC
    },
    // [ RC11_FW_RDA_SB_DISABLED ].serThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0008, 0x0000, 0x0000, 0x0000,
      0x000A, 0x0003, 0x0000, 0x0000,
      0x0008, 0x0006, 0x0000, 0x0000,
      0x000A, 0x000D, 0x0004, 0x0000,
      0x000A, 0x000F, 0x0009, 0x0000,
      0x000A, 0x0010, 0x000B, 0x0000,
      0x000A, 0x0010, 0x000C, 0x0004,
      0x000B, 0x0010, 0x0011, 0x0007,
      0x000B, 0x000E, 0x0012, 0x0009,
      0x000C, 0x000F, 0x0013, 0x0010,
      0x000C, 0x000F, 0x001B, 0x0016,
      0x000C, 0x000D, 0x0018, 0x0021,
      0x000B, 0x000E, 0x0015, 0x0019,
      0x000B, 0x0011, 0x0015, 0x001C
    },
    // [ RC11_FW_RDA_SB_DISABLED ].serFETThreshold =
    {
      0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000,
      0x000A, 0x0004, 0x0000, 0x0000,
      0x000B, 0x0009, 0x0000, 0x0000,
      0x000C, 0x000F, 0x0006, 0x0000,
      0x000D, 0x000F, 0x000B, 0x0000,
      0x000A, 0x0012, 0x000E, 0x0000,
      0x000D, 0x0013, 0x000F, 0x0005,
      0x000E, 0x0011, 0x0018, 0x000B,
      0x000D, 0x0013, 0x0018, 0x000E,
      0x000E, 0x0015, 0x001A, 0x0017,
      0x000F, 0x0013, 0x001F, 0x0021,
      0x000E, 0x0011, 0x0020, 0x0029,
      0x000F, 0x0014, 0x001E, 0x0038,
      0x0000, 0x0000, 0x0000, 0x0000
    },
    // [ RC11_FW_RDA_SB_DISABLED ].frameEndSerThrsh =
    {
      0x0011, 0x0014, 0x0023, 0x0036
    }
  }
};
#endif /* FEATURE_IS2000_1X_ADV */

/* SW configured FW variables */
muxmdsp_sw_cfg_fw_variables_type muxmdsp_sw_cfg_fw_variables;

/* FW Shared memory address */
cdmafw_shared_memory_interface_t* muxmdsp_fw_smem_addr = NULL;

/* Declare a critical section for MUXMDSP */
static rex_crit_sect_type muxmdsp_crit_sect;

/* MUXMDSP MSGR client & queue for single-point waits */
static msgr_client_t muxmdsp_msgr_client;
static msgr_id_t muxmdsp_msgr_queue;

typedef struct
{
  cdmafw_demod_walsh_beta_cfg_t walshBeta[MUXMDSP_DEMOD_MAX_CH];
  cdmafw_demod_ch0_ctrl_t       ch0;
  cdmafw_demod_ch1_ctrl_t       ch1;
  cdmafw_demod_ch2_ctrl_t       ch2;
} muxmdsp_demod_ch_type;

muxmdsp_demod_ch_type muxmdsp_demod_ch = { {{0}} };

/* Flag to remember TXFE/TXDAC state */
boolean muxmdsp_tx_triggered = FALSE;

#ifdef FEATURE_MODEM_1X_DRX
#define DRX_DISABLE_MASK 0xFFFF
#define DRX_LEGACY_MASK  0xFFCC
#define DRX_1XADV_MASK   0xFC44
#endif /* FEATURE_MODEM_1X_DRX */

#ifdef FEATURE_MODEM_1X_SW_DEMBACK
boolean muxmdsp_is_sw_demback_active = FALSE;
#endif /* FEATURE_MODEM_1X_SW_DEMBACK */

/*===========================================================================

                        FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION MUXMDSP_INIT_MSGR_BLOCKING_CLIENT

DESCRIPTION
  Initialize the MUXMDSP MSGR client to be used for single-point, blocking
  waits on responses to FW msgs.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void muxmdsp_init_msgr_blocking_client( void )
{
  errno_enum_type status;

  /*-----------------------------------------------------------------------*/

  do
  {
    /* Create MSGR client */
    status = msgr_client_create( &muxmdsp_msgr_client );
    if ( status != E_SUCCESS )
    {
      M1X_MSG(MUX,DBG_MDSP,
              "Failed to create MUXMDSP MSGR client");
      break;
    }

    /* Add a message queue of sufficient size to handle 1 FW RSP of any type */
    status = msgr_client_add_mq( "1x MUXMDSP",
                                 &muxmdsp_msgr_client,
                                 MSGR_PRIORITY_HIGH,
                                 1,
                                 sizeof(cdmafw_msg_rsp_u),
                                 &muxmdsp_msgr_queue );
    if ( status != E_SUCCESS )
    {
      ONEX_ERR_FATAL( "Failed to add MQ to MUXMDSP MSGR client", 0, 0, 0 );
      break;
    }

    /* Register early for *ALL* potential responses, since this is an
       expensive MSGR call.  Note that this assumes there should be NO
       interleaving of responses (i.e. using this functionality in muxmdsp
       is not thread-safe, it should only be done one at a time. */
    #ifdef FEATURE_MODEM_1X_SW_DEMBACK
    status = msgr_register( MSGR_ONEX_MUXMDSP,
                            &muxmdsp_msgr_client,
                            muxmdsp_msgr_queue,
                            CDMA_FW_DEMBACK_SW_CONFIG_RSP );
    if ( status != E_SUCCESS )
    {
      ONEX_ERR_FATAL( "Failed to register for CDMA_FW_DEMBACK_SW_CONFIG_RSP",
         0, 0, 0 );
      break;
    }
    #endif /* FEATURE_MODEM_1X_SW_DEMBACK */

    #ifdef FEATURE_MODEM_1X_DRX
    /* DRX Response */
    status = msgr_register( MSGR_ONEX_MUXMDSP,
                            &muxmdsp_msgr_client,
                            muxmdsp_msgr_queue,
                            CDMA_FW_FL_DRX_CONFIG_RSP );

    if ( status != E_SUCCESS )
    {
      ONEX_ERR_FATAL( "Failed to register for CDMA_FW_FL_DRX_CONFIG_RSP",
         0, 0, 0 );
      break;
    }
    #endif /* FEATURE_MODEM_1X_DRX */

  } while( 0 );

} /* muxmdsp_init_msgr_blocking_client */

/*===========================================================================

FUNCTION MUXMDSP_MSGR_SEND_AND_WAIT_FOR_RESPONSE

DESCRIPTION
  Send a MSGR message and wait for the corresponding response.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void muxmdsp_msgr_send_and_wait_for_response
(
  msgr_hdr_struct_type*      msg_hdr_ptr,        /* Pointer to MSG Header  */
  msgr_umid_type             send_umid,          /* UMID of the sent message */
  int32                      size,               /* Size of the sent message */
  msgr_umid_type             recv_umid
)
{
  cdmafw_msg_rsp_u response;
  uint32 rsp_size;
  errno_enum_type status;
  uint32 start_time, stop_time;

  /*-----------------------------------------------------------------------*/

  /* Init the MSGR header */
  msgr_init_hdr( msg_hdr_ptr,
                 MSGR_ONEX_MUXMDSP,
                 send_umid );

  /* Send the msg to FW, and upon success wait for the response.  Time the
     blocking response from the point of msgr_send()'s return through
     msgr_receive()'s return. */
  status = msgr_send( msg_hdr_ptr, size );
  start_time = is2k_fl_timestamp_ext();
  if ( status == E_SUCCESS )
  {
    M1X_MSG( MUX, DBG_MDSP,
       "Sent UMID 0x%X size %u",
       send_umid,
       size );

    /* Perform a blocking wait on the (expected) response */
    status = msgr_receive( &muxmdsp_msgr_client,
                           (uint8*)&response,
                           sizeof(response),
                           &rsp_size );
    stop_time = is2k_fl_timestamp_ext();

    /* Handle any rollover cases, where finish could be smaller than start */
    if ( stop_time < start_time )
    {
      stop_time = stop_time + is2k_fl_timestamp_rollover();
    }

    M1X_MSG( MUX, DBG_MDSP,
       "Received/Expected UMID response (0x%X/0x%X) size %u "
       "(status=%d,blocked=%u xo ticks)",
       response.hdr.id,
       recv_umid,
       rsp_size,
       status,
       stop_time - start_time );
  }
  else
  {
    M1X_MSG( MUX, DBG_MDSP,
       "Unable to send UMID 0x%x size %u (status=%d), not waiting for rsp",
       send_umid,
       size,
       status );
  }

} /* muxmdsp_msgr_send_and_wait_for_response() */

/*===========================================================================

FUNCTION MUXMDSP_UPDATE_FW_SMEM_ADDR

DESCRIPTION
  This function is used by SRCH MDSP module to update the mdsp shared
  memory address to the MUX module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_update_fw_smem_addr
(
  cdmafw_shared_memory_interface_t* smem_addr  /* FW Shared memory address */
)
{

 /*-----------------------------------------------------------------------*/

  /* Update the MUX MDSP module with the shared memory address. Note that
     the srch_mdsp module will send the address as NULL when SRCH requests
     for DEMOD APP disable */
  muxmdsp_fw_smem_addr = smem_addr;

  /* Update DEC with the DEMBACK_ID for the current run */
  /* At MDSP Standby->Active transition, FW does not know, whether it is a
   * legacy wakeup or QTA wakeup so FW cannot decide whether FW can access
   * Demback at that time. Hence FW made change to request demback for both
   * legacy and QTA cases at the end of RX_START processing.
   * We would be setting DEMBACK_ID in rx_proc_rf_tune_comp() of SRCH_RX_SM */
  if( !muxmdsp_fw_smem_addr )
  {
    dec_clear_demback_id();
  }
} /* muxmdsp_update_fw_smem_addr */

/*===========================================================================

FUNCTION muxmdsp_update_demod_walsh_beta_cfg

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_demod_walsh_beta_cfg
(
  muxmdsp_demod_num_ch_enum_type ch,
  muxmdsp_demod_walsh_beta_type   *walsh_beta_ptr
)
{
  /*-----------------------------------------------------------------------*/

  if( ch < MUXMDSP_DEMOD_MAX_CH )
  {
    muxmdsp_demod_ch.walshBeta[ch] = *walsh_beta_ptr;
  }

} /* muxmdsp_update_demod_walsh_beta_cfg */

/*===========================================================================

FUNCTION muxmdsp_update_demod_ch0

DESCRIPTION
  Notify the mDSP with the demod ch0 configuration information.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_demod_ch0
(
  muxmdsp_demod_ch0_type *ch0_ptr
)
{
  cdmafw_demod_channel0_cfg_msg_t demod_channel0_cfg_m = { {0} };

  /*-----------------------------------------------------------------------*/

  /* Update global structure */
  muxmdsp_demod_ch.ch0 = *ch0_ptr;

  /* Copy over to the parameters to the message payload */
  demod_channel0_cfg_m.cfg       = muxmdsp_demod_ch.ch0;
  demod_channel0_cfg_m.walshBeta = muxmdsp_demod_ch.walshBeta[MUXMDSP_DEMOD_CH0];

  if( CSFB_CALL_PENDING_NONE != mc_get_csfb_call_status() )
  {
    /* Update at the frame boundary for CSFB calls */
    demod_channel0_cfg_m.updateImmed   = FALSE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "CSFB call, CHO config apply on frame boundary" );
  }
  else
  {
    /* Update immediately for non-csfb calls */
    demod_channel0_cfg_m.updateImmed   = TRUE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Non-CSFB call, CHO config apply immediate" );
  }

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_CHANNEL0_CFG_CMD (CombEnabe=%d) to FW",
     muxmdsp_demod_ch.ch0.combEnable );

  muxmsgr_send_msg( RX_TASK,
                    &demod_channel0_cfg_m.hdr,
                    CDMA_FW_DEMOD_CHANNEL0_CFG_CMD,
                    sizeof(cdmafw_demod_channel0_cfg_msg_t) );

} /* muxmdsp_update_demod_ch0 */

/*===========================================================================

FUNCTION muxmdsp_update_demod_ch1

DESCRIPTION
  Notify the mDSP with the demod ch1 configuration information.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_demod_ch1
(
  muxmdsp_demod_ch1_type *ch1_ptr
)
{
  cdmafw_demod_channel1_cfg_msg_t demod_channel1_cfg_m = { {0} };

  /*-----------------------------------------------------------------------*/

  /* Update global structure */
  muxmdsp_demod_ch.ch1 = *ch1_ptr;

  /* Copy over to the parameters to the message payload */
  demod_channel1_cfg_m.cfg         = muxmdsp_demod_ch.ch1;
  demod_channel1_cfg_m.walshBeta   = muxmdsp_demod_ch.walshBeta[MUXMDSP_DEMOD_CH1];
  demod_channel1_cfg_m.updateImmed = TRUE;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_CHANNEL1_CFG_CMD (CombEnabe=%d) to FW",
     muxmdsp_demod_ch.ch1.combEnable );

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &demod_channel1_cfg_m.hdr,
                    CDMA_FW_DEMOD_CHANNEL1_CFG_CMD,
                    sizeof(cdmafw_demod_channel1_cfg_msg_t) );

} /* muxmdsp_update_demod_ch1 */

/*===========================================================================

FUNCTION muxmdsp_update_demod_ch2

DESCRIPTION
  Notify the mDSP with the demod ch2 configuration information.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_demod_ch2
(
  muxmdsp_demod_ch2_type *ch2_ptr
)
{
  cdmafw_demod_channel2_cfg_msg_t demod_channel2_cfg_m = { {0} };

  /*-----------------------------------------------------------------------*/

  muxmdsp_demod_ch.ch2 = *ch2_ptr;

  /* Copy over to the parameters to the message payload */
  demod_channel2_cfg_m.cfg         = muxmdsp_demod_ch.ch2;
  demod_channel2_cfg_m.walshBeta   = muxmdsp_demod_ch.walshBeta[MUXMDSP_DEMOD_CH2];
  demod_channel2_cfg_m.updateImmed = TRUE;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_CHANNEL2_CFG_CMD (CombEnabe=%d) to FW",
     muxmdsp_demod_ch.ch2.combEnable );

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &demod_channel2_cfg_m.hdr,
                    CDMA_FW_DEMOD_CHANNEL2_CFG_CMD,
                    sizeof(cdmafw_demod_channel2_cfg_msg_t) );

} /* muxmdsp_update_demod_ch2 */

/*===========================================================================

FUNCTION muxmdsp_update_pwr_ctrl_rpc

DESCRIPTION
  Notify the mDSP with the reverse power control configuration parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_pwr_ctrl_rpc
(
  deint_rc_type rc
)
{
  cdmafw_demod_rpc_cfg_msg_t demod_rpc_cfg_m = { {0} };
  boolean send_msg = TRUE;

  /*-----------------------------------------------------------------------*/

  switch(rc)
  {
    case DEINT_RC1:
    {
      demod_rpc_cfg_m.rpcCfg.rpcPct        = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcPuncPosSel = TRUE;
      demod_rpc_cfg_m.rpcCfg.rpcEven       = FALSE;
      break;
    }
    case DEINT_RC2:
    {
      demod_rpc_cfg_m.rpcCfg.rpcPct        = TRUE;
      demod_rpc_cfg_m.rpcCfg.rpcPuncPosSel = TRUE;
      demod_rpc_cfg_m.rpcCfg.rpcEven       = FALSE;
      break;
    }
    case DEINT_RC3:
    {
      demod_rpc_cfg_m.rpcCfg.rpcPct        = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcPuncPosSel = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcEven       = FALSE;
      break;
    }
    case DEINT_RC4:
    #ifdef FEATURE_IS2000_1X_ADV
    case DEINT_RC11:
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      demod_rpc_cfg_m.rpcCfg.rpcPct        = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcPuncPosSel = TRUE;
      demod_rpc_cfg_m.rpcCfg.rpcEven       = TRUE;
      break;
    }
    case DEINT_RC5:
    {
      demod_rpc_cfg_m.rpcCfg.rpcPct        = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcPuncPosSel = FALSE;
      demod_rpc_cfg_m.rpcCfg.rpcEven       = FALSE;
      break;
    }
    default:
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Illegal RC " );
      send_msg = FALSE;
    }
  } /* end switch */

  demod_rpc_cfg_m.rpcCfg.rpcEraseEn = TRUE;
  demod_rpc_cfg_m.updateImmed       = TRUE;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_RPC_CFG_CMD (RC=%d) to FW",
     rc );

  if( send_msg )
  {
    /* Send message */
    muxmsgr_send_msg( RX_TASK,
                      &demod_rpc_cfg_m.hdr,
                      CDMA_FW_DEMOD_RPC_CFG_CMD,
                      sizeof(cdmafw_demod_rpc_cfg_msg_t) );
  }

} /* muxmdsp_update_pwr_ctrl_rpc */


/*===========================================================================

FUNCTION muxmdsp_update_pwr_ctrl_fpc

DESCRIPTION
  Notify the mDSP with the forward power control configuration parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_pwr_ctrl_fpc
(
  boolean fpcEnable,
  uint16  fpcSel
)
{
  cdmafw_demod_fpc_cfg_msg_t demod_fpc_cfg_m = { {0} };

  /*-----------------------------------------------------------------------*/

  demod_fpc_cfg_m.fpcCfg.fpcEnable = fpcEnable;
  demod_fpc_cfg_m.fpcCfg.fpcSel    = fpcSel;
  demod_fpc_cfg_m.updateImmed      = TRUE;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_FPC_CFG_CMD (FPC En=%d, FPC Sel=%d) to FW and "
     "waiting for 300 us",
     fpcEnable, fpcSel );

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &demod_fpc_cfg_m.hdr,
                    CDMA_FW_DEMOD_FPC_CFG_CMD,
                    sizeof(cdmafw_demod_fpc_cfg_msg_t) );

  /* Adding wait of 300us to ensure back to back FFPC commands
     serviced sequentially */
  DALSYS_BusyWait( 300 );

} /* muxmdsp_update_pwr_ctl_fpc */

/*===========================================================================

FUNCTION muxmdsp_update_frame_offset

DESCRIPTION
  Notify the mDSP with the frame offset.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_update_frame_offset
(
  uint16 frameOffset
)
{
  cdmafw_demod_frame_offset_msg_t demod_frame_offset_m = { {0} };

  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_DRX
  /* DRX should be disabled now */
  srch_mux_update_drx_state( TIME_DRX_DISABLE );
  #endif /* FEATURE_MODEM_1X_DRX */

  /* Update the message payload with the setpt */
  demod_frame_offset_m.frameOffset = frameOffset;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_DEMOD_FRAME_OFFSET_CMD (FrameOffset=%d) to FW",
     frameOffset );

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &demod_frame_offset_m.hdr,
                    CDMA_FW_DEMOD_FRAME_OFFSET_CMD,
                    sizeof(cdmafw_demod_frame_offset_msg_t) );

  #ifdef FEATURE_MODEM_1X_DRX
  /* It's now safe to enable DRX */
  srch_mux_update_drx_state( TIME_DRX_ENABLE );
  #endif /* FEATURE_MODEM_1X_DRX */

} /* muxmdsp_update_frame_offset */

/*===========================================================================

FUNCTION muxmdsp_configure_rl_rc

DESCRIPTION
  Notify the mDSP which RC configuration we are operating in.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void muxmdsp_configure_rl_rc( cai_radio_config_type rc )
{
  /*-----------------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RL Legacy RC frame Config message */
    cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

    /* Update the message payload with the rliRCConfig */
    if( rc == CAI_RC_8 )
    {
      #ifdef FEATURE_IS2000_1X_ADV
      muxmdsp_rc8_max_power_protection_config( TRUE, 0, 0 );
      #endif /* FEATURE_IS2000_1X_ADV */

      rl_signaling_m.rliRCConfig = CDMAFW_RC_CONFIG__RC8;
    }
    else
    {
      #ifdef FEATURE_IS2000_1X_ADV
      muxmdsp_rc8_max_power_protection_config( FALSE, 0, 0 );
      #endif /* FEATURE_IS2000_1X_ADV */

      if( (rc == CAI_RC_1) || (rc == CAI_RC_2) )
      {
        rl_signaling_m.rliRCConfig = CDMAFW_RC_CONFIG__RC12;
      }
      #ifndef FEATURE_MODEM_1X_UTX
      else if( rc == CAI_INIT_RC )
      {
        rl_signaling_m.rliRCConfig = CDMAFW_RC_CONFIG__R_ACH;
      }
      #endif /* !FEATURE_MODEM_1X_UTX */
      else
      {
        /* all other RCs */
        rl_signaling_m.rliRCConfig = CDMAFW_RC_CONFIG__OTHER;
      }
    }

    /* PA/TX ON gating enabled */
    rl_signaling_m.rliDtxRfGatingEn = 1;
    rl_signaling_m.rliDtxRfGatingEn_Update = 1;
    rl_signaling_m.rliRCConfig_Update = TRUE;

    M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_RL_SIGNALING_CMD (RC=%d, RC Config=%d) to FW",
     rc, rl_signaling_m.rliRCConfig );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_signaling_m.hdr,
                      CDMA_FW_RL_SIGNALING_CMD,
                      sizeof(cdmafw_rl_signaling_msg_t) );
  }
} /* muxmdsp_configure_rl_rc */

/*===========================================================================

FUNCTION MUXMDSP_SET_EB_NO_TARGET_FCH

DESCRIPTION
  This functions sets the fundamental Eb/No target.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_eb_no_target_fch( uint16 target )
{
  /* FCH Target Setpt message */
  cdmafw_fch_target_setpoint_msg_t fch_setpoint_m = { { 0 } };

  /*-----------------------------------------------------------------------*/

  /* Update the message payload with the setpt */
  fch_setpoint_m.ebNtMagTargetFch = target;

  /* Update the local copy as well */
  muxmdsp_sw_cfg_fw_variables.ebNtMagTargetFch = target;

  /* Send message */
  muxmsgr_send_msg( TX_TASK,
                    &fch_setpoint_m.hdr,
                    CDMA_FW_FCH_TARGET_SETPOINT_CMD,
                    sizeof(cdmafw_fch_target_setpoint_msg_t) );

} /* muxmdsp_set_eb_no_target_fch */

/*===========================================================================

FUNCTION MUXMDSP_SET_EB_NO_TARGET_SCH

DESCRIPTION
  This functions sets the supplemental Eb/No target.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_eb_no_target_sch( uint16 target )
{
  /* SCH Target Setpt message */
  cdmafw_sch_target_setpoint_msg_t sch_setpoint_m = { { 0 } };

  /*-----------------------------------------------------------------------*/

  /* Update the message payload with the setpt */
  sch_setpoint_m.ebntMagTargetSch = target;

  /* Update the local copy as well */
  muxmdsp_sw_cfg_fw_variables.ebNtMagTargetSch = target;

  /* Send message */
  muxmsgr_send_msg( TX_TASK,
                    &sch_setpoint_m.hdr,
                    CDMA_FW_SCH_TARGET_SETPOINT_CMD,
                    sizeof(cdmafw_sch_target_setpoint_msg_t) );

} /* muxmdsp_set_eb_no_target_sch */

/*===========================================================================

FUNCTION MUXMDSP_SET_CHANNEL_GAINS

DESCRIPTION
  This functions writes to the MDSP the current values of the channel gains
  and the txRateAdj register. For MSGR interface, we would be sending a
  message consisting of the channel gains instead.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_channel_gains( void )
{
  #ifdef FEATURE_MODEM_1X_UTX
  /* RL Legacy RC frame Config message */
  cdmafw_rl_legacy_rc_frame_config_msg_t frame_config_m = { { 0 } };
  #endif /* FEATURE_MODEM_1X_UTX */

  uint16 tx_rate_adj;
  int16  tx_gain_adj;

  /* Set the DTX mask according to the DTX decision */
  uint16 dtx_mask       = txc_device_only_dtx_param.dtx_mask;
  boolean ta_is_enabled = txc_is_ta_enabled();

  /*-----------------------------------------------------------------------*/

  tx_rate_adj =
    MUXMDSP_10_TO_16_BIT( rpc_calculate_tx_rate_adj( channel_gains.txc_tx_rate_adj ) );

  if( 0 == channel_gains.frame_valid )
  {
    /* If this is not a valid frame, set PCG mask to 0 to gate Tx */
    dtx_mask = TX_PCG_MASK_DISABLED;
  }

  #ifdef FEATURE_MODEM_1X_UTX
  /* In FTM mode, MDSP MUST already be enabled by RF (without 1xL1 awareness).
   * And, we need to send CDMA_FW_RL_LEGACY_RC_FRAME_CONFIG_CMD only when FW
   * is active. */
  if( srch_mux_mdsp_demod_running() || onex_nonsig_is_ftm_mode() )
  {
    /* Write the gains to be latched at last PCG of fundicated frame
       by mDSP and latched to HW on the fundicated frame boundary */
    if( !ta_is_enabled )
    {
      if( txc_ta_info.ta_rpc_skip_frames > 0 )
      {
        txc_ta_info.ta_rpc_skip_frames--;

        //tx_gain_adj = muxmdsp_get_tx_gain_adjust();
        tx_gain_adj = (int16)txtc.power;
        tx_gain_adj = (tx_gain_adj << 8);

        if( txc_ta_info.ta_rpc_skip_frames != 0 )
        {
          #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
          rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                           RFM_1X_TX_CL_OVERRIDE,
                                           tx_gain_adj );
          #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

          M1X_MSG( MUX, DBG_MDSP,
             "1x2GTA: Tx power override Tx power %d, Skip frames %d",
             tx_gain_adj,
             txc_ta_info.ta_rpc_skip_frames );
        }

        if( txc_ta_info.ta_rpc_skip_frames == 0 )
        {
          #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
          rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                           RFM_1X_TX_CL_INIT,
                                           tx_gain_adj );
          #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

          M1X_MSG( MUX, LEGACY_HIGH,
             "1x2GTA: Tx power INIT Tx power %d, Skip frames %d",
             tx_gain_adj,
             txc_ta_info.ta_rpc_skip_frames );
        }
      }

      frame_config_m.MOD_PCH_GAIN  = channel_gains.txc_pch_gain;
      frame_config_m.MOD_SCH_GAIN  = channel_gains.txc_sch_gain;
      frame_config_m.MOD_DCCH_GAIN = channel_gains.txc_dcch_gain;
      frame_config_m.MOD_FCH_GAIN  = channel_gains.txc_fch_gain;
      frame_config_m.cTxRateAdjWr  = tx_rate_adj;

      /* Send the correct PCG mask to FW */
      frame_config_m.rliTransmitPCGMask = dtx_mask;

      /* PA/TX control are done via FW in NikeL and beyond */
      frame_config_m.cTxFrameValid  = channel_gains.frame_valid;
      frame_config_m.immediateLatch = FALSE;
    }
    else
    {
      frame_config_m.MOD_PCH_GAIN  = channel_gains.txc_pch_gain;
      frame_config_m.MOD_SCH_GAIN  = channel_gains.txc_sch_gain;
      frame_config_m.MOD_DCCH_GAIN = channel_gains.txc_dcch_gain;
      frame_config_m.MOD_FCH_GAIN  = channel_gains.txc_fch_gain;
      frame_config_m.cTxRateAdjWr  = tx_rate_adj;

      if( rxc_ta_info.ta_rsn == SRCH_TA_LTA )
      {
        frame_config_m.rliTransmitPCGMask = DTX_SB_MASK_DISABLED;
        frame_config_m.cTxFrameValid      = FALSE;
        frame_config_m.immediateLatch     = TRUE;
      }
      else if( rxc_ta_info.ta_rsn == SRCH_TA_QTA )
      {
        frame_config_m.rliTransmitPCGMask = dtx_mask;
        frame_config_m.cTxFrameValid      = channel_gains.frame_valid;
        frame_config_m.immediateLatch     = FALSE;
      }
      else
      {
        frame_config_m.rliTransmitPCGMask = DTX_SB_MASK_DISABLED;
        frame_config_m.cTxFrameValid      = FALSE;
        frame_config_m.immediateLatch     = FALSE;
      }
    }

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &frame_config_m.hdr,
                      CDMA_FW_RL_LEGACY_RC_FRAME_CONFIG_CMD,
                      sizeof(cdmafw_rl_legacy_rc_frame_config_msg_t) );
  } /* srch_mux_mdsp_demod_running() || onex_nonsig_is_ftm_mode() */
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP is not awake in online mode" );
  }
  #else /* !FEATURE_MODEM_1X_UTX */
  if( muxmdsp_fw_smem_addr )
  {
    /* Shared memory RL legacy modulator gains */
    cdmafw_legacy_mod_gains_t * mod_gains;

    /* Write channel gains to FW shared memory */
    mod_gains = &( muxmdsp_fw_smem_addr->
                   write.rlFrameConfigParams.modParams.legacyGains );

    /* Write the gains to be latched at last PCG of fundicated frame by mDSP
       and latched to HW on the fundicated frame boundary. */
    if( !ta_is_enabled )
    {
      if( txc_ta_info.ta_rpc_skip_frames > 0 )
      {
        txc_ta_info.ta_rpc_skip_frames--;

        //tx_gain_adj = muxmdsp_get_tx_gain_adjust();
        tx_gain_adj = (int16)txtc.power;
        tx_gain_adj = (tx_gain_adj << 8);

        if( txc_ta_info.ta_rpc_skip_frames != 0 )
        {
          #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
          rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                           RFM_1X_TX_CL_OVERRIDE,
                                           tx_gain_adj );
          #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

          M1X_MSG( MUX, DBG_MDSP,
             "1x2GTA: Tx power override Tx power %d, Skip frames %d",
             tx_gain_adj,
             txc_ta_info.ta_rpc_skip_frames );
        }

        if( txc_ta_info.ta_rpc_skip_frames == 0 )
        {
          #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
          rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                           RFM_1X_TX_CL_INIT,
                                           tx_gain_adj );
          #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

          M1X_MSG( MUX, DBG_MDSP,
             "1x2GTA: Tx power INIT Tx power %d, Skip frames %d",
             tx_gain_adj,
             txc_ta_info.ta_rpc_skip_frames );
        }
      }

      mod_gains->pich       = channel_gains.txc_pch_gain;
      mod_gains->sch        = channel_gains.txc_sch_gain;
      mod_gains->dcch       = channel_gains.txc_dcch_gain;
      mod_gains->fch        = channel_gains.txc_fch_gain;
      mod_gains->txRateAdj  = tx_rate_adj;

      /* Send the correct PCG mask to FW */
      muxmdsp_set_tx_pcg_mask( dtx_mask );
    }
    else
    {
      mod_gains->pich       = channel_gains.txc_pch_gain;
      mod_gains->sch        = channel_gains.txc_sch_gain;
      mod_gains->dcch       = channel_gains.txc_dcch_gain;
      mod_gains->fch        = channel_gains.txc_fch_gain;
      mod_gains->txRateAdj  = tx_rate_adj;

      if( rxc_ta_info.ta_rsn == SRCH_TA_LTA )
      {
        muxmdsp_set_tx_pcg_mask( TX_PCG_MASK_DISABLED );
      }
      else if( rxc_ta_info.ta_rsn == SRCH_TA_QTA )
      {
        /* Send the correct PCG mask to FW */
        muxmdsp_set_tx_pcg_mask( dtx_mask );
      }
      else
      {
        muxmdsp_set_tx_pcg_mask( TX_PCG_MASK_DISABLED );
      }
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }
  #endif /* FEATURE_MODEM_1X_UTX */

  DTX_DBG_MSG(
      "1x DTX: PCH = 0x%04X, SCH = 0x%04X, FCH = 0x%04X, DCCH = 0x%04X, "
      "tx_rate_adj = 0x%04X, Frame Valid = %d, PCG Mask = 0x%04X",
      channel_gains.txc_pch_gain,
      channel_gains.txc_sch_gain,
      channel_gains.txc_fch_gain,
      channel_gains.txc_dcch_gain,
      tx_rate_adj,
      channel_gains.frame_valid,
      dtx_mask );

} /* muxmdsp_set_channel_gains */

/*===========================================================================

FUNCTION MUXMDSP_SET_CHANNEL_GAINS_ZERO

DESCRIPTION
  This functions sets all the channel gains and the txRateAdj register
  to zero.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_channel_gains_zero( void )
{
  boolean ta_is_enabled = txc_is_ta_enabled();

  /*-----------------------------------------------------------------------*/

  if( !ta_is_enabled )
  {
    /* Reset the global variables */
    channel_gains.txc_pch_gain    = 0;
    channel_gains.txc_sch_gain    = 0;
    channel_gains.txc_fch_gain    = 0;
    channel_gains.txc_dcch_gain   = 0;
    channel_gains.txc_tx_rate_adj = 0;
  }

  muxmdsp_set_channel_gains();

} /* muxmdsp_set_channel_gains_zero */

/*===========================================================================

FUNCTION MUXMDSP_SET_TX_GAIN_ADJUST

DESCRIPTION
  This function has been modified for FW based-TXAGC targets. RPC Disable
  dipswitch is checked here instead of reading the current value of
  the CDMA1X_cTxGainAdjOverride register on the MDSP.

  The sequence of instructions has been reordered to work with MDSP
  memory banks and MDSP read sequence.

  1. Adjust tx_gain_adj_val as required by MDSP
  2. Check dip switch to check if RPC is disabled
  3. If RPC is enabled, tell MDSP to do normal close loop PC
     after TX Gain Adjust Override
  4. Write to tx gain adjust override register on the MDSP
  3. Write new tx_gain_adj_val to MDSP

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_tx_gain_adjust( int16 gain )
{
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  int16          tx_gain_adj_val;     /* Gain value adjusted                */
  boolean        rl_pwr_ctl_disabled; /* Boolean to indicate if RL power
                                         ctrl dip switch has been set       */
  /*-----------------------------------------------------------------------*/

  rl_pwr_ctl_disabled =
    cdma2kdiag_get_switch( CDMA2KDIAG_SW_DISABLE_RL_PWR_CTRL );

  tx_gain_adj_val = (int16)(gain << 8);

  /* The second parameter to this function is the clroverride flag.
     IF RPC is disabled, we want the clrOverride flag to be 0 so that
     FW doesn't resume RPC. If RPC is not disabled, then we want
     clrOverride to be 1 so that FW can resume RPC after this single
     override */
  if( rl_pwr_ctl_disabled )
  {
    rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                     RFM_1X_TX_CL_OVERRIDE,
                                     tx_gain_adj_val );
  }
  else
  {
    rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                     RFM_1X_TX_CL_INIT,
                                     tx_gain_adj_val );
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
} /* muxmdsp_set_tx_gain_adjust */

/*===========================================================================

FUNCTION muxmdsp_get_tx_gain_adjust

DESCRIPTION
  This functions retrieves the tx gain adjust.

  The power control enable bit is cleared first, so this write
  takes effect.  Power control is then restored to its previous
  state.

DEPENDENCIES
  This function must be called from inside the context of a
  *_LOCK / *_UNLOCK pair to ensure proper operation.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
int16 muxmdsp_get_tx_gain_adjust( void )
{
  int16 gain_adjust = 0;
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  rfm_device_enum_type device = txc_get_tx_device();

  /*-----------------------------------------------------------------------*/

  if( device != RFM_INVALID_DEVICE )
  {
    gain_adjust =
     ( (((int2)rfm_1x_get_tx_closed_loop(device))>> 8) & 0xff );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "TX device not yet set, returning 0 for tx_gain_adjust" );
    gain_adjust = 0;
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

  return gain_adjust;

} /* muxmdsp_get_tx_gain_adjust */

/*===========================================================================

FUNCTION MUXMDSP_SET_TX_GAIN_ADJ_STEP_SIZE

DESCRIPTION
  This function sends to the FW the current TX Gain Adjust Step size.

  FW latches the configuration from SW at the frame boundary.

  Convert the HW Configured Step Size to the value in MDSP
  which is in units of 1/512 of dB

  Conversions:
  0 = 1.00dB = 0x200
  1 = 0.50dB = 0x100
  2 = 0.25dB = 0x080
  3 = 1.50dB = 0x300
  4 = 2.00dB = 0x400

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_tx_gain_adj_step_size( cai_pwr_cntl_step_type step_size )
{
  uint16 dsp_step_size;

  /* RL Signaling message */
  cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

  /*-----------------------------------------------------------------------*/

  /* Convert the HW Configured Step Size to the value in MDSP
   which is in units of 1/512 of dB */

  switch( step_size )
  {
    case CAI_PWR_CNTL_STEP_1_DB:
      dsp_step_size = 0x200;
      break;

    case CAI_PWR_CNTL_STEP_0_5_DB:
      dsp_step_size = 0x100;
      break;

    case CAI_PWR_CNTL_STEP_0_25_DB:
      dsp_step_size = 0x80;
      break;

    case CAI_PWR_CNTL_STEP_1_5_DB:
      dsp_step_size = 0x300;
      break;

    case CAI_PWR_CNTL_STEP_2_0_DB:
      dsp_step_size = 0x400;
      break;

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid TxGainAdjStepSize, %d! Defaulting to 1dB",
        step_size);
      dsp_step_size = 0x200;        /* Default to 1dB step size         */
      return;
  }

  /* Update Step Size */
  rl_signaling_m.cTxGainAdjStepSize = dsp_step_size;
  rl_signaling_m.cTxGainAdjStepSize_Update = TRUE;

  txc_ta_info.tx_gain_adj_step_size = dsp_step_size;

  /* Send message */
  muxmsgr_send_msg( TX_TASK,
                    &rl_signaling_m.hdr,
                    CDMA_FW_RL_SIGNALING_CMD,
                    sizeof(cdmafw_rl_signaling_msg_t) );

  M1X_MSG( MUX, DBG_MDSP,
    "StepSize = %d, dsp_tep_size",
    step_size,
    dsp_step_size);

} /* muxmdsp_set_tx_gain_adj_step_size */

/*===========================================================================

FUNCTION muxmdsp_wr_tx_rate_adj_latch_offset

DESCRIPTION
  The function updates offset parameter which spcecifies the number of chips before
  the fundicated frame boundary that the modulator HW latches TX_RATE_ADJ

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_wr_tx_rate_adj_latch_offset( word tx_rate_adj_latch_offset )
{
  /*-----------------------------------------------------------------------*/

  /* This MDSP register is removed in Q6/MSGR interface */
  MODEM_1X_UNUSED( tx_rate_adj_latch_offset );

} /* muxmdsp_wr_tx_rate_adj_latch_offset */

/*===========================================================================

FUNCTION MUXMDSP_WR_FUNDFRAMEOFFSET

DESCRIPTION
  This funtion sets the fundicated frame offset to the mDSP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_wr_fundFrameOffset( uint16 fundoffset_value )
                                         /* value range from 0 to 15 PCG*/
{
  /*-----------------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RL Legacy RC frame Config message */
    cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

    /* Update the message payload with the fundFrameOffset */
    rl_signaling_m.fundFrameOffset = fundoffset_value;
    rl_signaling_m.fundFrameOffset_Update = TRUE;

    M1X_MSG( MUX, LEGACY_HIGH,
      "Sending CDMA_FW_RL_SIGNALING_CMD (FundFrameOffset=%d) to FW",
      fundoffset_value );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_signaling_m.hdr,
                      CDMA_FW_RL_SIGNALING_CMD,
                      sizeof(cdmafw_rl_signaling_msg_t) );

  }
} /* muxmdsp_wr_fundFrameOffset */

/*===========================================================================

FUNCTION muxmdsp_set_agc_ctl_mask_delay

DESCRIPTION
  This function is called by MUX to set agc control mask delay to FW.

DEPENDENCIES
  None.

RETURN VALUE
  None
SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_agc_ctl_mask_delay( uint8 delay )
{
  /* RL Legacy RC frame Config message */
  cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

  /*-----------------------------------------------------------------------*/

  /* Update the message payload with the agc control mask delay */
  rl_signaling_m.cTxMaskDelayWr = (uint16) delay + 1;
  rl_signaling_m.cTxMaskDelayWr_Update = TRUE;

  rxc_ta_info.rev_pwr_ctrl_delay = delay;

  M1X_MSG( MUX, LEGACY_HIGH,
     "Sending CDMA_FW_RL_SIGNALING_CMD (AGC Control Mask Delay=%d) to FW",
     rl_signaling_m.cTxMaskDelayWr );

  /* Send message */
  muxmsgr_send_msg( TX_TASK,
                    &rl_signaling_m.hdr,
                    CDMA_FW_RL_SIGNALING_CMD,
                    sizeof(cdmafw_rl_signaling_msg_t) );

} /* muxmdsp_set_agc_ctl_mask_delay */

/*===========================================================================

FUNCTION muxmdsp_update_tx_open_loop_override

DESCRIPTION
  This function updates the TX Open loop override to FW.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_update_tx_open_loop_override( uint16 override_mask )
{
  /* TX Open Loop Override message */
  cdmafw_tx_open_loop_override_msg_t ol_ovrd = { { 0 } };

  /*-----------------------------------------------------------------------*/

  /* Update the message payload with the agc control mask delay */
  ol_ovrd.txOpenLoopOverride = override_mask;
  ol_ovrd.txOpenLoopWr       = txc_ta_info.lta_ol_tx_pwr;

  M1X_MSG( MUX, DBG_MDSP,
     "Sending CDMA_FW_TX_OPEN_LOOP_OVERRIDE_CMD (Override=%d, Power=%d) to FW",
     override_mask,
     txc_ta_info.lta_ol_tx_pwr );

  /* Send message */
  muxmsgr_send_msg( TX_TASK,
                    &ol_ovrd.hdr,
                    CDMA_FW_TX_OPEN_LOOP_OVERRIDE_CMD,
                    sizeof( ol_ovrd ) );

} /* muxmdsp_update_tx_open_loop_override */

/*===========================================================================

FUNCTION        muxmdsp_get_open_loop_tx_power

DESCRIPTION     This returns the Open loop TX power from FW smem

DEPENDENCIES    None.

RETURN VALUE    Open Loop TX power in dBm.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_open_loop_tx_power( void )
{
  int16 open_loop_tx_power = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      open_loop_tx_power = muxmdsp_fw_smem_addr->read.rf_rd.cTxOpenLoopVal;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return open_loop_tx_power;

} /* muxmdsp_get_open_loop_tx_power */

/*===========================================================================

FUNCTION         muxmdsp_get_demback_id_index

DESCRIPTION     This returns the DEMBACK ID index in FW.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    DEMBACK ID Index

SIDE EFFECTS    None.

===========================================================================*/
uint32 muxmdsp_get_demback_id_index( void )
{
  uint32 index = 1;

  /*-----------------------------------------------------------------*/

  index = muxmdsp_fw_smem_addr->read.dembackId.index;

  return index;

} /* muxmdsp_get_demback_id_index */

/*===========================================================================

FUNCTION muxmdsp_get_demback_irq

DESCRIPTION     This returns the DEMBACK IRQ for the DEMBACK that's currently
                being used for 1x operation.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    DEMBACK IRQ

SIDE EFFECTS    None.

===========================================================================*/
dalint_1x_irq_types muxmdsp_get_demback_irq( void )
{
  dalint_1x_irq_types demback_irq = DALINT_1X_MAX_IRQ;
  uint32              index       = muxmdsp_get_demback_id_index();

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      /* Demback index to IRQ mapping */
      switch( index )
      {
        case 0:
        {
          demback_irq = DALINT_1X_RX_DB0_ISR;
          break;
        }

        case 1:
        {
          demback_irq = DALINT_1X_RX_DB1_ISR;
          break;
        }

        default:
        {
          ONEX_ERR_FATAL( "Unknown Demback index = %u",
                     index, 0, 0 );
          break;
        }

      } /* end of switch */
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    ONEX_ERR_FATAL( "FW disabled. DEMBACK ID is invalid", 0, 0, 0 );
  }

  M1X_MSG( MUX, LEGACY_MED,
           "muxmdsp_get_demback_irq() returned DEMBACK_ID = %u",
           demback_irq );

  return demback_irq;

} /* muxmdsp_get_demback_irq */

/*===========================================================================

FUNCTION         muxmdsp_get_demback_id_tdec_address

DESCRIPTION     This returns the DEMBACK ID TDEC address in FW.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    DEMBACK ID TDEC address

SIDE EFFECTS    None.

===========================================================================*/
uint32* muxmdsp_get_demback_id_tdec_address( void )
{
  uint32* tdec_decob_addr = NULL;

  /*-----------------------------------------------------------------*/

  tdec_decob_addr = (uint32*)muxmdsp_fw_smem_addr->read.dembackId.tdecAddr;

  return tdec_decob_addr;

} /* muxmdsp_get_demback_id_tdec_address */

/*===========================================================================

FUNCTION muxmdsp_get_demback_tdec_decob_addr

DESCRIPTION     This returns the tdec decob address that's currently being
                used for 1x operation.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    tdec decob address

SIDE EFFECTS    None.

===========================================================================*/
uint32* muxmdsp_get_demback_tdec_decob_addr( void )
{
  uint32* tdec_decob_addr = NULL;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      tdec_decob_addr = muxmdsp_get_demback_id_tdec_address();
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    ONEX_ERR_FATAL( "FW disabled. tdec decob addr is invalid", 0, 0, 0 );
  }

  return tdec_decob_addr;

} /* muxmdsp_get_demback_tdec_decob_addr */

#ifdef FEATURE_MODEM_1X_DRX
/*===========================================================================
FUNCTION       MUXMDSP_UPDATE_DRX_CONFIG

DESCRIPTION    This function sends message to FW to enable or disable DRX
               feature and waits for response from FW.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void muxmdsp_update_drx_config
(
  boolean enable_drx
)
{
  cdmafw_fl_drx_config_msg_t fw_drx_config = { { 0 } };

  /*-----------------------------------------------------------------*/

  if( enable_drx )
  {
    fw_drx_config.config.enable     = 1;
    fw_drx_config.config.legacyMask = DRX_LEGACY_MASK;
    fw_drx_config.config.fetMask    = DRX_1XADV_MASK;
  }
  else
  {
    fw_drx_config.config.enable     = 0;
    fw_drx_config.config.legacyMask = DRX_DISABLE_MASK;
    fw_drx_config.config.fetMask    = DRX_DISABLE_MASK;
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "DRX: Sending CDMA_FW_FL_DRX_CONFIG_CMD (enable=%d) to FW and "
    "waiting for response",
    enable_drx );

  /* Send the message and block until the response comes back */
  muxmdsp_msgr_send_and_wait_for_response( &fw_drx_config.hdr,
                                           CDMA_FW_FL_DRX_CONFIG_CMD,
                                           sizeof(cdmafw_fl_drx_config_msg_t),
                                           CDMA_FW_FL_DRX_CONFIG_RSP );

  M1X_MSG( MUX, LEGACY_HIGH,
    "DRX: Got CDMA_FW_FL_DRX_CONFIG_RSP from FW" );

} /* muxmdsp_update_drx_config */
#endif /* FEATURE_MODEM_1X_DRX */

/*===========================================================================

FUNCTION muxmdsp_get_pc_filt_gain

DESCRIPTION     Read the PC Filter Gain from MDSP.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_pc_filt_gain( void )
{
  uint16 gain = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    gain = muxmdsp_sw_cfg_fw_variables.pcFiltGain;
  }

  return gain;
} /* muxmdsp_get_pc_filt_gain */

/*===========================================================================

FUNCTION muxmdsp_set_pc_filt_gain

DESCRIPTION     Sets the filter gain for the forward link PC bit

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
void muxmdsp_set_pc_filt_gain( uint16 gain )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* With Q6/MSGR interface, this register is no longer needed to be written
       to FW because it has only one configuration. */
    muxmdsp_sw_cfg_fw_variables.pcFiltGain = gain;
  }
} /* muxmdsp_set_pc_filt_gain */

/*===========================================================================

FUNCTION muxmdsp_get_nt_io_filt_gain

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_nt_io_filt_gain( void )
{
  uint16 gain = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    gain = muxmdsp_sw_cfg_fw_variables.ntIoFiltGain;
  }

  return gain;
} /* muxmdsp_get_nt_io_filt_gain */

/*===========================================================================

FUNCTION muxmdsp_set_nt_io_filt_gain

DESCRIPTION     This macro sets the filter gain for the forward link Nt/Io

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
void muxmdsp_set_nt_io_filt_gain( uint16 gain )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    muxmdsp_sw_cfg_fw_variables.ntIoFiltGain = gain;
  }
} /* muxmdsp_set_nt_io_filt_gain */

/*===========================================================================

FUNCTION muxmdsp_get_eb_no_target_fch

DESCRIPTION     Read the Eb/No target for FCH from the mDSP.

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_eb_no_target_fch( void )
{
  uint16 target_fch = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    target_fch = muxmdsp_sw_cfg_fw_variables.ebNtMagTargetFch;
  }

  return target_fch;
} /* muxmdsp_get_eb_no_target_fch */

/*===========================================================================

FUNCTION muxmdsp_get_eb_no_target_sch

DESCRIPTION     Read the Eb/No target for SCH from the mDSP.

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_eb_no_target_sch( void )
{
  uint16 target_sch = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    target_sch = muxmdsp_sw_cfg_fw_variables.ebNtMagTargetSch;
  }

  return target_sch;
} /* muxmdsp_get_eb_no_target_sch */

/*===========================================================================

FUNCTION muxmdsp_get_sch_iq_acc

DESCRIPTION     This returns the IQ accumulation of channel 2
                over a frame.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_iq_acc( void )
{
  uint16 sch_iq_acc = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      sch_iq_acc = muxmdsp_fw_smem_addr->read.muxRead.ch2AccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return sch_iq_acc;

} /* muxmdsp_get_sch_iq_acc */

/*===========================================================================

FUNCTION muxmdsp_get_pc_acc

DESCRIPTION     This returns the PC bit accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_pc_acc( void )
{
  uint16 pc_acc = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pc_acc = muxmdsp_fw_smem_addr->read.muxRead.pcBitAccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_acc;

} /* muxmdsp_get_pc_acc */

/*===========================================================================

FUNCTION muxmdsp_get_fch_nt_io

DESCRIPTION     This returns the fundamental weighted Nt/Io
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_fch_nt_io( void )
{
  int16 fch_nt_io = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      fch_nt_io = ((MAG_SCALE *
           (int32)muxmdsp_fw_smem_addr->read.muxRead.fundWeightedNtMagAccFrame  *
           (int32)muxmdsp_fw_smem_addr->read.muxRead.fundWeightedNtMagAccFrame) >>16);
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return fch_nt_io;

} /* muxmdsp_get_fch_nt_io */

/*===========================================================================

FUNCTION muxmdsp_get_sch_nt_io

DESCRIPTION     This returns the supplemental weighted Nt/Io
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_sch_nt_io( void )
{
  int16 sch_nt_io = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      sch_nt_io = ((MAG_SCALE *
       (int32)muxmdsp_fw_smem_addr->read.muxRead.suppWeightedNtMagAccFrame *
       (int32)muxmdsp_fw_smem_addr->read.muxRead.suppWeightedNtMagAccFrame) >>16);
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return sch_nt_io;

} /* muxmdsp_get_sch_nt_io */

/*===========================================================================

FUNCTION muxmdsp_fpc_get_history

DESCRIPTION     Returns a sample of the FPC history.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_fpc_get_history( void )
{
  uint16 pc_hist = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pc_hist = muxmdsp_fw_smem_addr->read.powerControlStats.fpcHistory;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_hist;

} /* muxmdsp_fpc_get_history */

/*===========================================================================

FUNCTION muxmdsp_fpc_get_history_valid

DESCRIPTION     Returns whether the FPC history is valid.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_fpc_get_history_valid( void )
{
  uint16 pc_valid = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pc_valid = muxmdsp_fw_smem_addr->read.powerControlStats.fpcHistoryValid;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_valid;

} /* muxmdsp_fpc_get_history_valid */

/*===========================================================================

FUNCTION muxmdsp_rpc_get_history

DESCRIPTION     Returns a sample of the RPC history.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_history( void )
{
  uint16 pc_hist = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pc_hist = muxmdsp_fw_smem_addr->read.powerControlStats.rpcHistory;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_hist;

} /* muxmdsp_rpc_get_history */

/*===========================================================================

FUNCTION muxmdsp_rpc_get_history_valid

DESCRIPTION     Returns whether the RPC history is valid.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_history_valid( void )
{
  uint16 pc_valid = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pc_valid = muxmdsp_fw_smem_addr->read.powerControlStats.rpcHistoryValid;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_valid;

} /* muxmdsp_rpc_get_history_valid */

/*===========================================================================

FUNCTION muxmdsp_rpc_get_cell_history

DESCRIPTION     Return a sample of the RPC history for the
                specific cell.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_cell_history( uint16 cell )
{
  uint16 pc_hist = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      if(  cell < CDMAFW_NUM_PWR_CTL_CELLS )
      {
        pc_hist = muxmdsp_fw_smem_addr->read.powerControlStats.rpcHistoryCell[( cell )];
      }
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return pc_hist;
} /* muxmdsp_rpc_get_cell_history */

/*===========================================================================

FUNCTION        muxmdsp_get_frame_sync_tx_gain_adjust_fw

DESCRIPTION     Returns the Frame Synchronized Tx Gain Adjust from FW.

DEPENDENCIES    None.

RETURN VALUE    Tx Gain Adjust in -1/4 dB.
                0x8000 - Error or invalid value.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_frame_sync_tx_gain_adjust_fw( void )
{
  int16 tx_adj = 0x8000;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      /* FW returns the Frame synchronized tx gain adjust in -1/512 dB.
         But SW needs the value in -1/4 dB. Hence right shift by 7 to
         convert -1/512 to -1/4 dB. */
      tx_adj = muxmdsp_fw_smem_addr->read.powerControlStats.cTxGainAdjVal >> 7;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return tx_adj;
} /* muxmdsp_get_frame_sync_tx_gain_adjust_fw */

/*===========================================================================

FUNCTION muxmdsp_get_fch_wtd_pilot

DESCRIPTION     This returns the fundamental weighted pilot
                magnitude accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_fch_wtd_pilot( void )
{
  uint16 fch_wtd_pilot = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      fch_wtd_pilot = muxmdsp_fw_smem_addr->read.muxRead.fundWeightedPilotAccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return fch_wtd_pilot;

} /* muxmdsp_get_fch_wtd_pilot */

/*===========================================================================

FUNCTION muxmdsp_get_fch_rt_nt_io


DESCRIPTION     This returns the fundamental weighted sqrt(Nt/Io)
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_fch_rt_nt_io( void )
{
  uint16 nt_mag_acc = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      nt_mag_acc = muxmdsp_fw_smem_addr->read.muxRead.fundWeightedNtMagAccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return nt_mag_acc;
} /* muxmdsp_get_fch_rt_nt_io */

/*===========================================================================

FUNCTION muxmdsp_get_sch_rt_nt_io

DESCRIPTION     This returns the supplemental weighted sqrt(Nt/Io)
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_rt_nt_io( void )
{
  uint16 sch_rt_nt_io = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      sch_rt_nt_io = muxmdsp_fw_smem_addr->read.muxRead.suppWeightedNtMagAccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return sch_rt_nt_io;

} /* muxmdsp_get_sch_rt_nt_io */

/*===========================================================================

FUNCTION muxmdsp_get_sch_wtd_pilot

DESCRIPTION     This returns the supplemental weighted pilot magnitude
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_wtd_pilot( void )
{
  uint16 sch_wtd_pilot = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      sch_wtd_pilot = muxmdsp_fw_smem_addr->read.muxRead.suppWeightedPilotAccFrame;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return sch_wtd_pilot;
} /* muxmdsp_get_sch_wtd_pilot */

/*===========================================================================

FUNCTION muxmdsp_init

DESCRIPTION
  This function is called for muxmdsp module specific initialization.

DEPENDENCIES
  None.

RETURN VALUE
  None
SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_init( void )
{

  /*-----------------------------------------------------------------*/

  /* Initialize critical section */
  rex_init_crit_sect( &muxmdsp_crit_sect );

  /* Initialize private MSGR client for blocking waits to responses */
  muxmdsp_init_msgr_blocking_client();

} /* muxmdsp_init */

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION muxmdsp_load_fw_rc11_rda_tables

DESCRIPTION
  This function is loads FW with a set of thresholds used for improved RDA
  performance in RC11 for both Smart Blanking enabled and disabled cases.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void muxmdsp_load_fw_rc11_rda_tables
(
  muxmdsp_rc11_fw_rda_mode_type rda_mode
)
{
  uint32                  i            = 0;
  cdmafw_rda_config_msg_t rda_config_m = { { 0 } };

  /* Snapshot the start time */
  timetick_type           stopwatch    = timetick_get();

  /*-----------------------------------------------------------------*/

  /* If the mode is bogus, default to 'safe' SB-disabled mode */
  if( rda_mode >= RC11_FW_RDA_NUM_OPTIONS )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Illegal RC11 FW RDA mode %d, using RC11_FW_RDA_DISABLED",
      rda_mode);
    rda_mode = RC11_FW_RDA_SB_DISABLED;
  }

  /* Load slopeThreshold table */
  for( i=0 ; i < RC11_FW_RDA_SLOPE_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.slopeThreshold
      [ i / (CDMAFW_NUM_RATE_HYPOTHESES)][ i % (CDMAFW_NUM_RATE_HYPOTHESES)] =
      rc11_fw_rda_tables[rda_mode].slopeThreshold[i];
  }

  /* Load interceptThreshold table */
  for( i=0 ; i < RC11_FW_RDA_INTERCEPT_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.interceptThreshold
      [ i / (CDMAFW_NUM_RATE_HYPOTHESES)][ i % (CDMAFW_NUM_RATE_HYPOTHESES)] =
      rc11_fw_rda_tables[rda_mode].interceptThreshold[i];
  }


  /* Load energySelfThreshold table */
  for( i=0 ; i < RC11_FW_RDA_ENERGY_SELF_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.energySelfThreshold
      [ i / (CDMAFW_NUM_RATE_HYPOTHESES)][ i % (CDMAFW_NUM_RATE_HYPOTHESES)] =
      rc11_fw_rda_tables[rda_mode].energySelfThreshold[i];
  }

  /* Load serThreshold table */
  for( i=0 ; i < RC11_FW_RDA_SER_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.serThreshold
      [ i / (CDMAFW_NUM_RATE_HYPOTHESES)][ i % (CDMAFW_NUM_RATE_HYPOTHESES)] =
      rc11_fw_rda_tables[rda_mode].serThreshold[i];
  }

  /* Load serFETThreshold table */
  for( i=0 ; i < RC11_FW_RDA_SER_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.serFETThreshold
      [ i / (CDMAFW_NUM_RATE_HYPOTHESES)][ i % (CDMAFW_NUM_RATE_HYPOTHESES)] =
      rc11_fw_rda_tables[rda_mode].serFETThreshold[i];
  }

  /* Load frameEndSerThrsh table */
  for( i=0 ; i < RC11_FW_RDA_FRAME_END_SER_THRESH_SIZE ; ++i )
  {
    muxmdsp_fw_smem_addr->write.rda_data.serFrameEndThreshold[ i ] =
      rc11_fw_rda_tables[rda_mode].frameEndSerThrsh[i];
  }

  rda_config_m.loadRdaDataTables =
    (rda_mode == RC11_FW_RDA_SB_DISABLED) ? FALSE : TRUE;

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &rda_config_m.hdr,
                    CDMA_FW_RDA_CONFIG_CMD,
                    sizeof(cdmafw_rda_config_msg_t) );

  /* Get the elapsed time in slow clock units (30.5us) */
  stopwatch = timetick_get_elapsed( stopwatch, T_SCLK );

  /* Report total timeticks */
  M1X_MSG( MUX, DBG_MDSP,
    "RC11 FW RDA table %d loaded in %d timeticks",
    rda_mode,
    stopwatch );

} /* muxmdsp_load_fw_rc11_rda_tables */

/*===========================================================================

FUNCTION muxmdsp_set_rc8_frame_config_parms

DESCRIPTION
  Writes the following mdsp registers:
  1.  CDMA1X_rliFchAckMask
  2.  CDMA1X_rliSchAckMask
  3.  CDMA1X_rliFchToPich
  4.  CDMA1X_rliDcchToPich
  5.  CDMA1X_rliSchToPich
  6.  CDMA1X_rliFchAckchToPich
  7.  CDMA1X_rliSchAckchToPich
  8.  CDMA1X_rliBlankedFrame
  9.  CDMA1X_rliRPCMask
  10. CDMA1X_rliDTXFrame
  11. CDMA1X_rliDTXPattern
  12. CDMA1X_rliDTXPowerBoost
  13. CDMA1X_rliFPCMask

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rc8_frame_config_parms( void )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RC8 Frame Signalling Message */
    cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

    #ifdef FEATURE_MODEM_1X_UTX
    /* RC8 Frame Config Message */
    cdmafw_rl_rc8_frame_config_msg_t rl_rc8_cfg_m   = { { 0 } };
    #else /* FEATURE_MODEM_1X_UTX */
    cdmafw_rl_frame_config_params_t  *rl_frame_cfg_params;

    cdmafw_rc8_t2p_gains_t           *rc8_gains;
    #endif /* FEATURE_MODEM_1X_UTX */

    /* No DTX or SB - Mask is set to 0xFFFF by default. Will be overwritten by
       proper mask in case of SB or DTX */
    uint16 dtx_sb_mask = DTX_SB_MASK_DISABLED;

    /* Default RPC mask to 200 Hz. Will be overwritten by
       proper mask in case of SB or DTX */
    uint16 rpc_mask = RPC_MASK_200_HZ;

    /* Load the default FCH 2 PICH linear gain ratio. Will be overwritten by
       proper mask in case of SB or DTX */
    word f2p_linear_gain_ratio = rc8_frame_config_parms.fch_linear_gain_ratio;

    /* Fill the RL signalling message parameters */
    rl_signaling_m.rliAckMask_Update = 1;
    rl_signaling_m.rliFchAckMask = rc8_frame_config_parms.fch_ack_mask & 0x7FFE;
    rl_signaling_m.rliSchAckMask = rc8_frame_config_parms.sch_ack_mask & 0x7FFE;

    #ifdef FEATURE_MODEM_1X_UTX
    /* ----------------------------------------------------------
       Write the Channel to Pilot linear gain ratios in Q11.
       ---------------------------------------------------------- */
    rl_rc8_cfg_m.rliFchToPich   = f2p_linear_gain_ratio;
    rl_rc8_cfg_m.rliDcchToPich  = rc8_frame_config_parms.dcch_linear_gain_ratio;
    rl_rc8_cfg_m.rliSchToPich   = rc8_frame_config_parms.sch_linear_gain_ratio;
    rl_rc8_cfg_m.rliAckchToPich = rc8_frame_config_parms.fch_ack_linear_gain_ratio;
    //NOT SUPPORTED: rl_rc8_cfg_m.rliSchAckchToPich = rc8_frame_config_parms.sch_ack_linear_gain_ratio;

    #else /* FEATURE_MODEM_1X_UTX */

    rc8_gains = &( muxmdsp_fw_smem_addr->
                   write.rlFrameConfigParams.modParams.rc8T2P );

    rl_frame_cfg_params = &( muxmdsp_fw_smem_addr->
                             write.rlFrameConfigParams );

    /* Update Channel to Pilot linear gain ratios in FW smem */
    rc8_gains->fch      = f2p_linear_gain_ratio;
    rc8_gains->dcch     = rc8_frame_config_parms.dcch_linear_gain_ratio;
    rc8_gains->sch      = rc8_frame_config_parms.sch_linear_gain_ratio;
    rc8_gains->fchackch = rc8_frame_config_parms.fch_ack_linear_gain_ratio;
    //NOT SUPPORTED: rc8_gains->schackch = rc8_frame_config_parms.sch_ack_linear_gain_ratio;
    #endif /* FEATURE_MODEM_1X_UTX */

    /* ----------------------------------------------------------
       Write the reverse link FCH Blanking flag to the mDSP.
       ---------------------------------------------------------- */
    if( rc8_frame_config_parms.blanked_frame == TRUE )
    {
      /* Smart Blanked - Mask is set to 0x9999 */
      dtx_sb_mask = SB_MASK_ENABLED;

      /* Network should set the FCH Ack Mask for FET correctly in SB.
         If it doesn't, prinit an error message. We will correct the
         Ack Mask below after checking for DTX */
      if( (rl_signaling_m.rliFchAckMask & SB_MASK_ENABLED)
          != rl_signaling_m.rliFchAckMask )
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "FCH Ack Mask (0x%04X) not orthogonal to SB Mask (0x%04X)",
          rl_signaling_m.rliFchAckMask,
          SB_MASK_ENABLED );
      }

      /* FCHtoPICH should be zero for SB */
      f2p_linear_gain_ratio = 0;
    }

    /* ----------------------------------------------------------
       Write the reverse link PC Pattern to the mDSP.
       ---------------------------------------------------------- */
    if( rc8_frame_config_parms.rpc_mode == CAI_PWR_CNTL_200_OR_400_HZ )
    {
      /* RPC 400 Hz - PCGs 1, 3, 5, 7, 9, 11, 13, 15 valid */
      rpc_mask = RPC_MASK_200_400_HZ;
    }
    else if ( rc8_frame_config_parms.rpc_mode == CAI_PWR_CNTL_200_HZ )
    {
      /* RPC 200 Hz - PCGs 1, 5, 9, 13 valid */
      rpc_mask = RPC_MASK_200_HZ;
    }

    /* Set FPC mask to default to the one specified in RCPM - Set it
       every frame. It will be overwritten below incase Device only DTX
       is enabled. FPC update flag will also be set then to enable FW to
       update the FPC mask */
    rl_signaling_m.rliFPCMask =
          (ffpc_get_current_fpc_mode() == FPC_MODE_1XA_PRI_200Hz) ?
          FPC_MASK_200_HZ : FPC_MASK_400_HZ;

    /* ------------------------------------------------------------
       Write the reverse link DTX Pattern to the mDSP.
       ------------------------------------------------------------ */
    #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
    if( txc_device_only_dtx_is_enabled() )
    {
      if( txc_device_only_dtx_param.dtx_mask != DTX_SB_MASK_DISABLED )
      {
        /* DTX'd - Set the DTX Mask */
        dtx_sb_mask = txc_device_only_dtx_param.dtx_mask;

        /* Set the RPC Mask */
        rpc_mask = txtc.rc8_status.rpc_mask;

        /* Set the FPC Mask */
        rl_signaling_m.rliFPCMask_Update = TRUE;
        rl_signaling_m.rliFPCMask        = txtc.rc8_status.fpc_mask;
      }
    }
    #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

    if( 0 == rc8_frame_config_parms.frame_valid )
    {
      /* If this is not a valid frame, set PCG mask to 0 to gate Tx */
      dtx_sb_mask = TX_PCG_MASK_DISABLED;
    }

    /* Update the gains and masks altered by DTX and SB */
    #ifdef FEATURE_MODEM_1X_UTX
    rl_rc8_cfg_m.rliTransmitPCGMask   = dtx_sb_mask;
    rl_rc8_cfg_m.rliFchToPich         = f2p_linear_gain_ratio;
    rl_rc8_cfg_m.rliPCPattern         = rpc_mask;
    #else /* !FEATURE_MODEM_1X_UTX */
    muxmdsp_set_tx_pcg_mask(dtx_sb_mask);
    rc8_gains->fch = f2p_linear_gain_ratio;
    rl_frame_cfg_params->rc8PcPattern = rpc_mask;
    #endif /* FEATURE_MODEM_1X_UTX */

    /* AND the ACK masks with respective PCG masks to ensure ACK can
       be sent during DTX and SB. Technically, DTX and SB shouldn't
       happen during SCH, but will modify it as well just for
       completeness */
    rl_signaling_m.rliFchAckMask &= dtx_sb_mask;
    rl_signaling_m.rliSchAckMask &= dtx_sb_mask;

    DTX_DBG_MSG( "1x DTX: Pilot Gating Mask: 0x%04X, "
                 "FPC Mask: 0x%04X, "
                 "RPC Mask: 0x%04X, "
                 "R-FCH ACK Mask: 0x%04X, "
                 "R-SCH ACK Mask: 0x%04X, "
                 "FCH2PICH Gain Ratio: 0x%04X, "
                 "SCH2PICH Gain Ratio: 0x%04X",
                 dtx_sb_mask,
                 rl_signaling_m.rliFPCMask,
                 rpc_mask,
                 rl_signaling_m.rliFchAckMask,
                 rl_signaling_m.rliSchAckMask,
                 f2p_linear_gain_ratio,
                 rc8_frame_config_parms.sch_linear_gain_ratio
                 );


    #ifdef FEATURE_MODEM_1X_UTX
    /* PA/TX control are done via FW in NikeL and beyond */
    rl_rc8_cfg_m.cTxFrameValid = rc8_frame_config_parms.frame_valid;

    /* Send messages */
    muxmsgr_send_msg( TX_TASK,
                      &rl_rc8_cfg_m.hdr,
                      CDMA_FW_RL_RC8_FRAME_CONFIG_CMD,
                      sizeof(cdmafw_rl_rc8_frame_config_msg_t) );
    #endif /* FEATURE_MODEM_1X_UTX */

    muxmsgr_send_msg( TX_TASK,
                      &rl_signaling_m.hdr,
                      CDMA_FW_RL_SIGNALING_CMD,
                      sizeof(cdmafw_rl_signaling_msg_t) );
  }
} /* muxmdsp_set_rc8_frame_config_parms */

/*===========================================================================

FUNCTION muxmdsp_set_rliFPCMask

DESCRIPTION

     For 1xAdvanced FPC if the mode is 200Hz primary
     then set the FPC mask to use PCGs 3, 7, 11, 15.

     For all other modes default to 1, 3, 5, 7, 9, 11, 13, 15.
     This only applies to RC11 but defaulting to this value
     puts FW in a known good state until the FPC mode is changed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rliFPCMask( fpc_mode_type fpc_mode )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RC8 Frame Signalling Message */
    cdmafw_rl_signaling_msg_t rl_signaling_m = { { 0 } };

    rl_signaling_m.rliFPCMask_Update = TRUE;

    if( fpc_mode == FPC_MODE_1XA_PRI_200Hz )
    {
      rl_signaling_m.rliFPCMask = FPC_MASK_200_HZ;
    }
    else
    {
      rl_signaling_m.rliFPCMask = FPC_MASK_400_HZ;
    }

    M1X_MSG( MUX, LEGACY_HIGH,
      "FPC mask %d FPC mode %d",rl_signaling_m.rliFPCMask,fpc_mode );

    muxmsgr_send_msg( TX_TASK,
                      &rl_signaling_m.hdr,
                      CDMA_FW_RL_SIGNALING_CMD,
                      sizeof(cdmafw_rl_signaling_msg_t) );

  }
} /* muxmdsp_set_rliFPCMask */

/*===========================================================================

FUNCTION muxmdsp_set_rl_rc8_frame_config_parms_zero

DESCRIPTION
  Writes zero to the following mdsp registers:
  1. CDMA1X_rliFchAckMask
  2. CDMA1X_rliSchAckMask
  3. CDMA1X_rliFchToPich
  4. CDMA1X_rliDcchToPich
  5. CDMA1X_rliSchToPich
  6. CDMA1X_rliFchAckchToPich
  7. CDMA1X_rliSchAckchToPich
  8. CDMA1X_rliBlankedFrame
  9. CDMA1X_rliRPCMask

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rc8_frame_config_parms_zero( void )
{

  /*-----------------------------------------------------------------*/

  rc8_frame_config_parms.fch_linear_gain_ratio     = 0;
  rc8_frame_config_parms.dcch_linear_gain_ratio    = 0;
  rc8_frame_config_parms.sch_ack_linear_gain_ratio = 0;
  rc8_frame_config_parms.fch_ack_linear_gain_ratio = 0;
  rc8_frame_config_parms.sch_ack_linear_gain_ratio = 0;
  rc8_frame_config_parms.fch_ack_mask              = 0;
  rc8_frame_config_parms.sch_ack_mask              = 0;
  rc8_frame_config_parms.blanked_frame             = FALSE;
  rc8_frame_config_parms.rpc_mode = CAI_PWR_CNTL_200_OR_400_HZ;

  muxmdsp_set_rc8_frame_config_parms();
} /* muxmdsp_set_rc8_frame_config_parms_zero */

/*===========================================================================

FUNCTION muxmdsp_rd_rc11_fch_rda_info

DESCRIPTION
  This function reads FCH rate determination information for RC11 from MDSP.
  It is called when SW receives an MDSP FCH decode done interrupt.

  The frame rate on the FCH assumes four possible values: Full, Half, Quarter
  and Eighth rate. The task of a Rate Determination Algorithm (RDA) is to determine
  whether the frame is received with sufficient quality, and in case it is, to
  correctly classify the rate of the received frame. If the frame does not fit in
  one of these categories, it is classified as a blank frame.

DEPENDENCIES
  None.

RETURN VALUE
  Frame rate, as determined by MDSP.

SIDE EFFECTS
  None.

===========================================================================*/
cai_data_rate_type muxmdsp_rd_rc11_fch_rda_info ( void )
{
  cai_data_rate_type rate = CAI_BLANK_RATE;

  /*-----------------------------------------------------------------*/

  /* Read RDA information for RC11 FCH. */
  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      rate = (cai_data_rate_type) muxmdsp_fw_smem_addr->read.adv1xDecodeDoneInfo.fliRC11FchRateInfo;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return rate;

} /* end muxmdsp_rd_rc11_fch_rda_info */

/*===========================================================================

FUNCTION muxmdsp_update_rc11_params

DESCRIPTION
  This function updates the RC11 parameters based on the Radio Configuration
  Parameter message (RCPM.) It is called when RXC enters traffic state, as well
  as when RXC receives RCP trans command from CP (upon receipt of a new RCPM
  from base station.)

  RC11 is added to reduce the average power needed per user for higher capacity.
  This RC has the following new features which is performed in MDSP with
  parameters reveived from SW (RCPM.)
  1.  1/8 rate smart blanking to reduce transmit power used for background noise
  2.  Reduced power control rate for power savings from overhead
  3.  Frame early termination to further reduce transmit power

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_update_rc11_params( cai_rcp_rxc_txc_type *rc11_rpc_data )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RC8 Frame Signalling Message */
    cdmafw_fl_signaling_msg_t fl_signaling_m = { { 0 } };

    /* The ack masks indicate which PCG MDSP can transmit an ACK for early decoding.
       Before writing RC11 ack masks to MDSP, clear bits 0 and 15. PCG 0 and 15 are not
       valid PCGs to send ACK. Also for FCH ACK mask, mask with 0xAAAA since only
       even PCGs are allowed. */
    fl_signaling_m.fliFchAckMask_Update = 1;
    fl_signaling_m.fliFchAckMask = (rc11_rpc_data->rev_fch_ack_mask & 0xAAAA);

    fl_signaling_m.fliSchAckMask_Update = 1;
    fl_signaling_m.fliSchAckMask = (rc11_rpc_data->rev_sch_ack_mask & 0x7FFE);

    /* Decoding triggers at ACK PCG n-1, so subtract 1 from FCH ack mask.
       Before writing decode PCG mask to MDSP, clear bits 0 and 15. PCG 0 and 15 are not
       valid PCGs to perform early decoding. */
    if( rc11_rpc_data->rev_fch_blanking_dutycycle == CAI_FCH_BLANKING_DISABLED )
    {
      fl_signaling_m.fliFchEarlyDecodePCGMask_Update = 1;
      fl_signaling_m.fliFchEarlyDecodePCGMask        =
        ( ( rc11_rpc_data->for_fch_ack_mask_no_rl_blanking << 1 ) & 0x3FFC );
    }
    else /* blanking enabled 1-in-4 or 1-in-8 */
    {
      fl_signaling_m.fliFchEarlyDecodePCGMask_Update = 1;
      fl_signaling_m.fliFchEarlyDecodePCGMask        =
        ( ( rc11_rpc_data->for_fch_ack_mask_rl_blanking << 1 ) & 0x3FFC );
    }

    /* Modify FCH blanking duty cycle to what is expected by MDSP. No blanking
       means RDA information is always used to update FFPC setpoint. Per MDSP
       FW/SW interface, MDSP prefers 3 for CAI_FCH_N_1_OF_4_FRAMES and 7 for
       CAI_FCH_N_1_OF_8_FRAMES.  No FL blanking stays the same at 0. */
    if (rc11_rpc_data->for_fch_blanking_dutycycle == CAI_FCH_N_1_OF_4_FRAMES)
    {
      fl_signaling_m.fliBlankingPeriod_Update = 1;
      fl_signaling_m.fliBlankingPeriod = 3;
      muxmdsp_load_fw_rc11_rda_tables( RC11_FW_RDA_SB_ENABLED );
    }
    else if (rc11_rpc_data->for_fch_blanking_dutycycle == CAI_FCH_N_1_OF_8_FRAMES)
    {
      fl_signaling_m.fliBlankingPeriod_Update = 1;
      fl_signaling_m.fliBlankingPeriod = 7;
      muxmdsp_load_fw_rc11_rda_tables( RC11_FW_RDA_SB_ENABLED );
    }
    else
    {
      fl_signaling_m.fliBlankingPeriod_Update = 1;
      fl_signaling_m.fliBlankingPeriod = 0;
      muxmdsp_load_fw_rc11_rda_tables( RC11_FW_RDA_SB_DISABLED );
    }

    M1X_MSG( MUX, LEGACY_MED,
       "Sending CDMA_FW_FL_SIGNALING_CMD (FET Mask=0x%04X) to FW",
       fl_signaling_m.fliFchEarlyDecodePCGMask );

    /* Send message */
    muxmsgr_send_msg( RX_TASK,
                      &fl_signaling_m.hdr,
                      CDMA_FW_FL_SIGNALING_CMD,
                      sizeof(cdmafw_fl_signaling_msg_t) );
  }

} /* muxmdsp_update_rc11_params */

/*===========================================================================

FUNCTION muxmdsp_set_rc11_mode

DESCRIPTION
  This function enables/disables RC11 operation in the MDSP.

  RC11 is added to reduce the average power needed per user for higher capacity.
  This RC has the following new features which is performed in MDSP with
  parameters reveived from SW.
  1.  1/8 rate smart blanking to reduce transmit power used for background noise
  2.  Reduced power control rate for power savings from overhead
  3.  Frame early termination to further reduce transmit power

DEPENDENCIES
  MDSP must be running.

RETURN VALUE
  None.

SIDE EFFECTS
  MDSP ISR controls RXC FCH frame processing.

===========================================================================*/
void muxmdsp_set_rc11_mode( boolean enable_flag )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RC8 Frame Signalling Message */
    cdmafw_fl_signaling_msg_t        fl_signaling_m = { { 0 } };

    fl_signaling_m.fliRC11Enable_Update = 1;

    if ( enable_flag )
    {
      fl_signaling_m.fliRC11Enable = 1;
    }

    /* Send message */
    muxmsgr_send_msg( RX_TASK,
                      &fl_signaling_m.hdr,
                      CDMA_FW_FL_SIGNALING_CMD,
                      sizeof(cdmafw_fl_signaling_msg_t) );

  }
} /* muxmdsp_set_rc11_mode */

/*===========================================================================

FUNCTION muxmdsp_enable_1xA_logging

DESCRIPTION
  This function takes a boolean that enables 1xA logging in the MDSP when
  true is passed in and disables logging otherwise.

DEPENDENCIES
  MDSP must be running. Current RC's should be 1xA relevant. SW ISR
  handler should be registered.

RETURN VALUE
  None.

SIDE EFFECTS
  MDSP logging and log interrupt will be enabled/disabled. If enabled and
  the SW ISR handler corresponding to the FW log interrupt is registered,
  an interrupt will occur every 10ms.

===========================================================================*/
void muxmdsp_enable_1xA_logging( boolean enable_flag )
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* 1xA log */
    cdmafw_adv1x_log_config_msg_t adv1x_log_cfg_m = { { 0 } };

    if ( enable_flag )
    {
      adv1x_log_cfg_m.logEnable = 1;
      adv1x_log_cfg_m.logMsgEnable = 1;
    }

    /* Send message */
    muxmsgr_send_msg( RX_TASK,
                      &adv1x_log_cfg_m.hdr,
                      CDMA_FW_ADV1X_LOG_CONFIG_CMD,
                      sizeof(cdmafw_adv1x_log_config_msg_t) );

  }
} /* muxmdsp_enable_1xA_logging */

/*===========================================================================

FUNCTION muxmdsp_read_1xa_log_buffer

DESCRIPTION
  This function reads the contents of the mDSP log specified by the mDSP at
  the time of the call and copies its contents to the pointed to
  buffer (*log_buf).

  The mDSP switches between 2 buffers. 1 buffer per interrupt.
  Each buffer holds a half-frame's worth of data.

  buffer0:      (second half pcg, pcg 15, frame n-1),
                (first half pcg, pcg 0, frame n),
                .....,
                (first half pcg, pcg 7, frame n)

  buffer1:      (second half pcg, pcg 7, frame n),
                (first half pcg, pcg 8, frame n),
                .....,
                (first half pcg, pcg 15, frame n)

 This function aligns the start of the pointed to log with the start of
 the frame using the above PCG information.

DEPENDENCIES
  MDSP must be running. Current RC's should be 1xA relevant. SW ISR
  handler should be registered.

RETURN VALUE
  None.

SIDE EFFECTS
  mDSP items are read and copied to the SW log.
===========================================================================*/
void muxmdsp_read_1xa_log_buffer
(
  uint16 *entry_index,
  uint16 max_log_size,
  uint32 *sw_log_buf,
  uint8 mdsp_buff_num
)
{
  uint16 item_cnt;
  const uint8 mdsp_buff_size = 96;

  /*-------------------------------------------------------------------------*/

  if( ( sw_log_buf != NULL ) &&
      ( entry_index != NULL ) &&
      ( muxmdsp_fw_smem_addr != NULL ) )
  {
    /* Use the configurations from above and read the data from the mDSP. */
    for( item_cnt = 0;
         (item_cnt < mdsp_buff_size) && (*entry_index < max_log_size);
         item_cnt++ )
    {
      if( ( mdsp_buff_num < CDMAFW_LOG_NUM_BUFFERS ) &&
          ( item_cnt < CDMAFW_LOG_BUFFER_SIZE ) )
      {
        sw_log_buf[*entry_index] =
          muxmdsp_fw_smem_addr->read.adv1xHalfPcgLogBuffer[mdsp_buff_num][item_cnt];
      }
      *entry_index += 1;
    }
  }
} /* muxmdsp_read_1xa_log_buffer */

/*===========================================================================
FUNCTION muxmdsp_read_1xA_ack_log

DESCRIPTION
  This function reads the contents of the mDSP ack log contents specified by
  the mDSP at the time of the call and prints them if debugging is enabled.

  This function also scans through the log contents to determine when an
  ACK was first detected on the F-ACKCH.

  Since ACKs can only be recieved in even PCGs and in the range of 2-14,
  a value of 15 or more indicates no ACK was received this frame.

  Each logBuf holds 96 32-bit entries.
  This holds 8 even pcgs in a single frame:
    - Each pcg has 6 cells
    - Each cell has 2 32bit entries
        - 2 32-bit entries hold:
            16-bit ack symbol data |  16 bit th1 data
            16-bit ack decision    |  16 bit thb data

DEPENDENCIES
  MDSP must be running. Current RC's should be 1xA relevant.

RETURN VALUE
  PCG that ACK was received.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 muxmdsp_read_1xA_ack_log( void )
{
  uint8 log_index = 0;
  uint8 loop_count;
  uint32 ack_decision = 0;
  uint16 ack_decision_index = 0;
  uint8 ack_pcg = 15; /*initialize to an invalid ACK PCG*/

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    /* Need to flip this as the current log being read is the one that is being
       filled by FW. We need to read the previous one */
    log_index = muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBufIndex^1;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!");
    return ack_pcg;
  }

  /* The ACKs can only be received on even PCGs. There are two 32 bit values
     per Cell per PCG. We are only reading from cell 0 here for logging so we
     offset by 12 to get the first entry of each PCG. The second 32bit entry
     contains the ACK decision made. If the the upper 16bits are zero then no
     ACK was detected. If they are all 1's then an ACK was detected.
  */
  for(loop_count = 0; loop_count < 8; loop_count++)
  {
    ack_decision_index = ((loop_count * 12) + 1);

    if( (ack_decision_index < CDMAFW_ACK_FRAME_LOG_SIZE) &&
        (log_index < CDMAFW_NUM_ACK_LOG_BUFFERS) )
    {
      ack_decision =
        muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][ack_decision_index];
    }

    if( (ack_decision >> 16) == 0x0001 )
    {
      /* An ACK was detected this PCG!

         The MDSP only keeps track of even PCGs, so double the loop count to
         save the correct even PCG value where the ACK was received.
      */
      ack_pcg = loop_count * 2;
      break;
    }
    /*else no ACK detected.*/
  }

  ADV_DBG_MSG(
            "[0] 0x%08X 0x%08X, [2] 0x%08X 0x%08X, "
            "[4] 0x%08X 0x%08X, [6] 0x%08X 0x%08X",
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][0],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][1],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][12],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][13],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][24],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][25],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][36],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][37] );

  ADV_DBG_MSG(
            "[8] 0x%08X 0x%08X, [10] 0x%08X 0x%08X, "
            "[12] 0x%08X 0x%08X, [14] 0x%08X 0x%08X",
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][48],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][49],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][60],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][61],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][72],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][73],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][84],
            muxmdsp_fw_smem_addr->read.ackLogBuffer.ackLogBuf[log_index][85] );

  return ack_pcg;

} /* muxmdsp_read_1xA_ack_log */

/*===========================================================================

FUNCTION muxmdsp_fch_fpc_setpoint_is_frozen

DESCRIPTION
  This function reads FCH FPC Freeze indicator from MDSP.  It is
  called when SW receives an MDSP FCH decode done interrupt.

  This variable is used to determine whether FFPC setpiont updates
  should be frozen.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:   FFPC setpoint is frozen (read zero from MDSP).
  FALSE:  Update FFPC setpoint this frame (read non-zero from MDSP).

SIDE EFFECTS
  None.

===========================================================================*/
boolean muxmdsp_fch_fpc_setpoint_is_frozen( void )
{
  boolean is_frozen = FALSE;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      if (muxmdsp_fw_smem_addr->read.flFpcFreezeInfo.fliFchFpcFreezeFlag != 0)
      {
        is_frozen = TRUE;
      }
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return is_frozen;

} /* muxmdsp_fch_fpc_setpoint_is_frozen */

/*===========================================================================

FUNCTION muxmdsp_fch_fpc_setpoint_freeze_config

DESCRIPTION
  This function enables/disables FCH FFPC setpoint freezing and provides
  the configurable number of continuous UP commands limit to the mDSP.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  F-FCH FFPC setpoint freezing will either be enabled and configured or
  disabled.

===========================================================================*/
void muxmdsp_fch_fpc_setpoint_freeze_config
(
  boolean enable_freeze,
  uint16 freeze_limit
)
{
  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* FPC Freeze limit message */
    cdmafw_fpc_freeze_cfg_msg_t fpc_freeze_cfg_m = { { 0 } };

    fpc_freeze_cfg_m.fliFchFpcFreezeEnable = enable_freeze;
    fpc_freeze_cfg_m.fliFchFpcFreezeLimit = freeze_limit;

    /* Send message */
    muxmsgr_send_msg( RX_TASK,
                      &fpc_freeze_cfg_m.hdr,
                      CDMA_FW_FPC_FREEZE_CONFIG_CMD,
                      sizeof(cdmafw_fpc_freeze_cfg_msg_t) );

  }
} /* muxmdsp_fch_fpc_setpoint_freeze_config */

/*===========================================================================

FUNCTION muxmdsp_rc8_max_power_protection_config

DESCRIPTION
  This function enables/disables RC8 Max Power Protection. When this is enabled
  at the mDSP, SW will program minimum channel to pilot linear power ratios for
  R-SCH and R-FCHACKCH. When MPP is disabled, SW will program 0 for these ratios.

DEPENDENCIES
  MDSP controlled MPP is only valid in Rev RC8.

RETURN VALUE
  None.

SIDE EFFECTS
  MPP will either be enabled or disabled based on the proved enable parameter.

===========================================================================*/
void muxmdsp_rc8_max_power_protection_config
(
  boolean enable,
  uint32 fchAckT2Pmin,
  uint32 schT2Pmin
)
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RL Max Power Protection CFG message */
    cdmafw_rl_mpp_cfg_msg_t rl_mpp_cfg_m = { { 0 } };

    if ( enable )
    {
      rl_mpp_cfg_m.rliEnableMPP = 1;
      rl_mpp_cfg_m.rliFchAckchT2PMin = fchAckT2Pmin;
      rl_mpp_cfg_m.rliSchT2PMin = schT2Pmin;
    }

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_mpp_cfg_m.hdr,
                      CDMA_FW_RL_MPP_CONFIG_CMD,
                      sizeof(cdmafw_rl_mpp_cfg_msg_t) );
  }
} /* muxmdsp_rc8_max_power_protection_config */

/*===========================================================================

FUNCTION muxmdsp_rl_ack_test_mode_config

DESCRIPTION
  This function enables/disables reverse link ACK channel test mode for
  1x Advanced. If enabled, the MDSP will begin transmitting an ACK or NAK
  (as specified) in a single PCG per frame. The change takes effect in the
  next PCG.

DEPENDENCIES
  ACK test mode is only valid in Rev RC8.

RETURN VALUE
  None.

SIDE EFFECTS
  Test mode is enabled or disabled.

===========================================================================*/
void muxmdsp_rl_ack_test_mode_config
(
  boolean enable,
  boolean send_ack
)
{

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* RL ack test mode CFG message */
    cdmafw_rl_ack_test_mode_cfg_msg_t rl_ack_tm_cfg_m = { { 0 } };

    if ( enable )
    {
      rl_ack_tm_cfg_m.rliAckTestMode = 0x8000;
      rl_ack_tm_cfg_m.rliAckTestModeValue = (send_ack & 0x1);
    }

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_ack_tm_cfg_m.hdr,
                      CDMA_FW_RL_ACK_TEST_MODE_CONFIG_CMD,
                      sizeof(cdmafw_rl_ack_test_mode_cfg_msg_t) );

  }
} /* muxmdsp_rl_ack_test_mode_config */

/*===========================================================================

FUNCTION muxmdsp_get_early_decode_pcg

DESCRIPTION
  This function retrieves the PCG prior to when the early decode attempt
  completed and adds 1 to convert it to the actual early decode PCG.

DEPENDENCIES
  Must be in traffic on forward RC11

RETURN VALUE
  PCG the early decode completed.

SIDE EFFECTS
  None

===========================================================================*/
uint16 muxmdsp_get_early_decode_pcg( void )
{
  uint16 pcg = 0;

  /*-----------------------------------------------------------------*/

  /* Save early decode pcg from mDSP. mDSP returns the PCG number
     prior to successful early decode, so add 1 to the value. */
  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr )
    {
      pcg = muxmdsp_fw_smem_addr->read.adv1xDecodeDoneInfo.fliFchDecodePCGIndexMinusOne;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!");
    }
  }

  return (pcg + 1);

} /* muxmdsp_get_early_decode_pcg */

/*===========================================================================

FUNCTION muxmdsp_get_1xa_log_buff_index

DESCRIPTION
  This function retrieves the index into the FW 1xA log ping-pong buffer
  to process this time around.

DEPENDENCIES
  None

RETURN VALUE
  mdsp log buffer index

SIDE EFFECTS
  None

===========================================================================*/
uint8 muxmdsp_get_1xa_log_buff_index( void )
{
  uint8 index = 0;

  /*-----------------------------------------------------------------*/

  /* Read the current half frame index as specified by the mDSP.
     Make sure that the index is in the valid range */

  return index;

} /* muxmdsp_get_1xa_log_buff_index */

/*===========================================================================

FUNCTION muxmdsp_enable_apf

DESCRIPTION
  This function enables/disables adaptive pilot filtering (APF) in firmware.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_enable_apf( boolean enable )
{
  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    /* APF enable message */
    cdmafw_apf_enable_msg_t apf_enable_m = { { 0 } };

    apf_enable_m.apfEnable = enable;
    apf_enable_m.apfStep   = 16;

    M1X_MSG( MUX, DBG_MDSP,
      "APF enable = %d, step = %d",
      enable,
      apf_enable_m.apfStep );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &apf_enable_m.hdr,
                      CDMA_FW_APF_ENABLE_CMD,
                      sizeof(cdmafw_apf_enable_msg_t) );
  }

} /* end muxmdsp_configure_apf */
#endif /* FEATURE_IS2000_1X_ADV */

#ifdef FEATURE_MODEM_1X_UTX
/*===========================================================================

FUNCTION MUXMDSP_SEND_TXDAC_TRIGGER

DESCRIPTION
  This function sends the TxDAC trigger to FW

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_send_txdac_trigger( cfw_tx_dac_start_msg_t* txdac_trigger_m )
{
  /*-----------------------------------------------------------------*/

  if( txdac_trigger_m )
  {
    /* Register for a response */
    muxmsgr_register_msg( TX_TASK, CDMA_FW_TX_DAC_START_RSP );

    muxmsgr_set_start_rsp_flag();

    M1X_MSG( MUX, LEGACY_HIGH,
       "Sending CDMA_FW_TX_DAC_START_CMD to FW" );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &(txdac_trigger_m->hdr),
                      CDMA_FW_TX_DAC_START_CMD,
                      sizeof(cfw_tx_dac_start_msg_t) );

    muxmdsp_tx_triggered = TRUE;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed NULL pointer for TxDAC trigger" );
  }

} /* muxmdsp_set_txdac_trigger */

/*===========================================================================

FUNCTION MUXMDSP_STOP_TXDAC

DESCRIPTION
  This function stops the TxDAC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_stop_txdac( void )
{
  /* TX DAC Stop Message */
  cfw_tx_dac_stop_msg_t txdac_stop_m = { { 0 } };

  /*-----------------------------------------------------------------*/

  if ( muxmdsp_tx_triggered )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
        "Sending CDMA_FW_TX_DAC_STOP_CMD to FW" );

    /* Register for a response */
    muxmsgr_register_msg( TX_TASK, CDMA_FW_TX_DAC_STOP_RSP );

    muxmsgr_set_stop_rsp_flag();

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &txdac_stop_m.hdr,
                      CDMA_FW_TX_DAC_STOP_CMD,
                      sizeof(cfw_tx_dac_stop_msg_t) );

    M1X_MSG( MUX, LEGACY_HIGH,
      "Waiting for CDMA_FW_TX_DAC_STOP_RSP from FW" );

    /* Wait for a response from FW. FW will wait until the upcoming frame
       boundary to execute CDMA_FW_TX_DAC_STOP_RSP. Waiting for FW to send
       a response could delay up to an entire frame. If this is an issue
       this should be revisted and optimized. */
    muxmsgr_wait_on_stop_rsp();

    M1X_MSG( MUX, LEGACY_HIGH,
      "Got CDMA_FW_TX_DAC_STOP_RSP from FW" );

    /* Remember that TXDAC has been stopped */
    muxmdsp_tx_triggered = FALSE;

    /* Deregister from the message */
    muxmsgr_deregister_msg( TX_TASK, CDMA_FW_TX_DAC_STOP_RSP );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Skipping TXDAC stop. TXDAC not triggered." );
  }

} /* muxmdsp_stop_txdac */

#else /* FEATURE_MODEM_1X_UTX */
/*===========================================================================

FUNCTION MUXMDSP_ENC_CH_EN

DESCRIPTION
  This function writes to FW shared memory to enable/disable encoder channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_enc_ch_en( uint32 ch, boolean enable )
{
  uint32* ch_en;

  /*-----------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_MED,
    "MDSP: CH%d enable -> %d",
    (ch+1),
    enable );

  if( muxmdsp_fw_smem_addr )
  {
    ch_en = &( muxmdsp_fw_smem_addr->write.rlFrameConfigParams.chEnableMask );
    *ch_en &= ~( 1 << ch );               /* clear old value */
    *ch_en |= ( enable << ch );           /* set new value  */
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
    "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_enc_ch_en */

/*===========================================================================

FUNCTION MUXMDSP_SET_ENC_INPUT_LEN

DESCRIPTION
  This function writes len to the current encoder input length
  in FW shared memory.

  It's assumed that the data written to LMEM has been zero padded to
  account for uneven word sizes.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_enc_input_len( uint32 channel, uint32 len )
{
  uint32 num_words = len;

  /*-----------------------------------------------------------------*/

  /* ensure num_words is an even word multiple */
  num_words = ( num_words + 3 ) >> 2;

  M1X_MSG( MUX, DBG_MDSP,
    "MDSP: enc ch %u write %u words",
    channel + 1,
    num_words );

  if( muxmdsp_fw_smem_addr )
  {
    muxmdsp_fw_smem_addr->write
      .rlFrameConfigParams.encParams.encInWords[channel] = num_words;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_set_enc_input_len */

/*===========================================================================

FUNCTION MUXMDSP_GET_ENC_INPUT_LEN

DESCRIPTION
  This function returns the encoder input len for a given channel
  in FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 muxmdsp_get_enc_input_len( uint32 channel )
{
  uint32 num_words = 0;

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    num_words = muxmdsp_fw_smem_addr->write
                .rlFrameConfigParams.encParams.encInWords[channel];
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }

  return num_words;

} /* muxmdsp_get_enc_input_len */

/*===========================================================================

FUNCTION MUXMDSP_SET_FRAME_CONFIG_DATA_RATE

DESCRIPTION
  This function sets the frame data rate for legacy RC 1/2
  in FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_frame_config_data_rate( uint32 rate )
{

  /*-----------------------------------------------------------------*/

  if( rate > (uint32)CDMAFW_DATA_RATE__FULL )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid frame data rate %u", rate );
  }
  else
  {
    if( muxmdsp_fw_smem_addr )
    {
      muxmdsp_fw_smem_addr->write.rlFrameConfigParams.dataRate = rate;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
} /* muxmdsp_set_frame_config_data_rate */

/*===========================================================================

FUNCTION MUXMDSP_SET_ENC_CH1_PARAMS

DESCRIPTION
  This function writes to FW shared memory channel 1 encoder parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_enc_ch1_params( cdmafw_rl_enc_ch1_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch1 = *parm;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 1 cfg param pointer is NULL!" );
  }

} /* muxmdsp_set_enc_ch1_params */

/*===========================================================================

FUNCTION MUXMDSP_GET_ENC_CH1_PARAMS

DESCRIPTION
  This function reads the ch1 parameter contents of shared mem into parm.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_get_enc_ch1_params( cdmafw_rl_enc_ch1_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      *parm = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch1;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 1 cfg param pointer is NULL!" );
  }

} /* muxmdsp_get_enc_ch1_params */

/*===========================================================================

FUNCTION MUXMDSP_SET_ENC_CH2_PARAMS

DESCRIPTION
  This function writes to FW shared memory channel 2 encoder parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_enc_ch2_params( cdmafw_rl_enc_ch2_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch2 = *parm;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 2 cfg param pointer is NULL!" );
  }

} /* muxmdsp_set_enc_ch2_params */

/*===========================================================================

FUNCTION MUXMDSP_GET_ENC_CH2_PARAMS

DESCRIPTION
  This function reads the ch2 parameter contents of shared mem into parm.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_get_enc_ch2_params( cdmafw_rl_enc_ch2_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      *parm = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch2;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 2 cfg param pointer is NULL!" );
  }

} /* muxmdsp_get_enc_ch2_params */

/*===========================================================================

FUNCTION MUXMDSP_SET_ENC_CH3_PARAMS

DESCRIPTION
  This function writes to FW shared memory channel 3 encoder parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_enc_ch3_params( cdmafw_rl_enc_ch3_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch3 = *parm;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 3 cfg param pointer is NULL!" );
  }

} /* muxmdsp_set_enc_ch3_params */

/*===========================================================================

FUNCTION MUXMDSP_GET_ENC_CH3_PARAMS

DESCRIPTION
  This function reads the ch1 parameter contents of shared mem into parm.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_get_enc_ch3_params( cdmafw_rl_enc_ch3_cfg_t * parm )
{

  /*-----------------------------------------------------------------*/

  if( parm )
  {
    if( muxmdsp_fw_smem_addr )
    {
      *parm = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.encParams.ch3;
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed Ch. 3 cfg param pointer is NULL!" );
  }

} /* muxmdsp_get_enc_ch3_params */

/*===========================================================================

FUNCTION MUXMDSP_SET_RL_LC_STATE

DESCRIPTION
  This function writes the user long code state to FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rl_lc_state( uint64 user_pn_state )
{
  uint64 translated_ulpn = 0;

  /*-----------------------------------------------------------------*/

  /* Bolt LC is bit reversed, readjust 42-bits LC to be aligned with bit 0
     64 bits - 42 bits = 22 bits of LSB 0's after bit reversal
     Right shift by 22 bits to align LC 42-bits */
  translated_ulpn = Q6_P_brev_P( user_pn_state ) >> 22;

  if( muxmdsp_fw_smem_addr )
  {
    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.lcState0 = translated_ulpn & 0xFFFFFFFF;

    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.lcState1 = ( translated_ulpn >> 32 ) & 0x3FF;

    /* The information related to encryption/decryption and other
     * key related information will be logged only if the below
     * API retuns TRUE. This is to avoid key information leakage
     * and other potential threats. */ 
     
    if(mmgsdi_is_test_mode_logging_enabled_for_subscription(mcc_asid) == TRUE)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "MDSP: ulpn 0x%08X%08X brev_ulpn 0x%08X%08X",
        (uint32)( ( user_pn_state >> 32 ) & 0x3FF ),
        (uint32)( user_pn_state & 0xFFFFFFFF ),
        (uint32)( ( translated_ulpn >> 32 ) & 0x3FF ),
        (uint32)( translated_ulpn & 0xFFFFFFFF ) );
    }
    else
    {
      M1X_MSG( MUX, DBG_MDSP,
        "Disabled logging of secured information");
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_set_rl_lc_state */

/*===========================================================================

FUNCTION MUXMDSP_GET_RL_LC_STATE

DESCRIPTION
  This function reads the user long code state from FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  Current user long code pn state in FW shared memory.

SIDE EFFECTS
  None.

===========================================================================*/
uint64 muxmdsp_get_rl_lc_state( void )
{
  uint64 user_pn_state = 0;

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    user_pn_state  = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
                                                .pnParams.lcState1 & 0x3FF;
    user_pn_state  = user_pn_state << 32;
    user_pn_state |= muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
                                                .pnParams.lcState0;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }

  return user_pn_state;
} /* muxmdsp_get_rl_lc_state */

/*===========================================================================

FUNCTION MUXMDSP_SET_RL_LC_MASK

DESCRIPTION
  This function writes the user long code mask to FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rl_lc_mask( uint64 user_pn_mask )
{
  uint64 translated_ulpn_mask = 0;

  /*-----------------------------------------------------------------*/

  /* Bolt LC is bit reversed, readjust 42-bits LC to be aligned with bit 0
     64 bits - 42 bits = 22 bits of LSB 0's after bit reversal
     Right shift by 22 bits to align LC 42-bits */
  translated_ulpn_mask = Q6_P_brev_P( user_pn_mask ) >> 22;

  if( muxmdsp_fw_smem_addr )
  {
    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.lcMask0 = translated_ulpn_mask & 0xFFFFFFFF;

    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.lcMask1 = ( translated_ulpn_mask >> 32 ) & 0x3FF;

    /* The information related to encryption/decryption and other
     * key related information will be logged only if the below
     * API retuns TRUE. This is to avoid key information leakage
     * and other potential threats. */ 
     
    if(mmgsdi_is_test_mode_logging_enabled_for_subscription(mcc_asid) == TRUE)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "MDSP: mask 0x%08X%08X brev_mask 0x%08X%08X",
        (uint32)( ( user_pn_mask >> 32 ) & 0x3FF ),
        (uint32)( user_pn_mask & 0xFFFFFFFF ),
        (uint32)( ( translated_ulpn_mask >> 32 ) & 0x3FF ),
        (uint32)( translated_ulpn_mask & 0xFFFFFFFF ) );
    }
    else
    {
      M1X_MSG( MUX, DBG_MDSP,
        "Disabled logging of secured information");
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_set_rl_lc_mask */

/*===========================================================================

FUNCTION MUXMDSP_SET_RL_SPN_STATE

DESCRIPTION
  This function writes the IQ short PN state to FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_rl_spn_state( uint32 i_pn_state, uint32 q_pn_state )
{

  uint32 trans_i_pn_state, trans_q_pn_state;

  /*-----------------------------------------------------------------*/

  trans_i_pn_state = (uint32)i_pn_state;
  trans_q_pn_state = (uint32)q_pn_state;

  /* Use Hexagon 32-brev instr, right shift by 17 bits to align 15-bit SPN */
  trans_i_pn_state = Q6_R_brev_R( trans_i_pn_state ) >> 17;
  trans_q_pn_state = Q6_R_brev_R( trans_q_pn_state ) >> 17;

  if( muxmdsp_fw_smem_addr )
  {
    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.spnState.I_PN_STATE_01 = trans_i_pn_state & 0x7FFF;

    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
      .pnParams.spnState.Q_PN_STATE_01 = trans_q_pn_state & 0x7FFF;

    /* The information related to encryption/decryption and other
     * key related information will be logged only if the below
     * API retuns TRUE. This is to avoid key information leakage
     * and other potential threats. */ 
     
    if(mmgsdi_is_test_mode_logging_enabled_for_subscription(mcc_asid) == TRUE)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
              "MDSP: SPN state I=0x%04X Q=0x%04X",
              (uint16)( trans_i_pn_state & 0x7FFF ),
              (uint16)( trans_q_pn_state & 0x7FFF ) );
    }
    else
    {
      M1X_MSG( MUX, DBG_MDSP,
        "Disabled logging of secured information");
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_set_rl_spn_state */

/*===========================================================================

FUNCTION MUXMDSP_GET_RL_SPN_STATE

DESCRIPTION
  This function reads the IQ short PN state from FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  structure with smem spn values

SIDE EFFECTS
  None.

===========================================================================*/
enc_spn_state_type muxmdsp_get_rl_spn_state( void )
{
  enc_spn_state_type spn = { 0 };

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    spn.spn_i = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
          .pnParams.spnState.I_PN_STATE_01 & 0x7FFF;
    spn.spn_q = muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams
          .pnParams.spnState.Q_PN_STATE_01 & 0x7FFF;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }

  return spn;

} /* muxmdsp_get_rl_spn_state */

/*===========================================================================

FUNCTION MUXMDSP_TOGGLE_IRAM_INDEX

DESCRIPTION
  This function toggles the IRAM index for the ping pong buffer in LMEM.

DEPENDENCIES
  None.

RETURN VALUE
  Current IRAM index.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 muxmdsp_toggle_iram_index( void )
{
  static uint32 iram_idx = 0;

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    /* toggle iram_idx */
    iram_idx = !iram_idx;

    M1X_MSG( MUX, DBG_MDSP,
      "MDSP toggle iram index %u",
      iram_idx );

    /* Set modParams IRAM index to new ping/pong index */
    muxmdsp_fw_smem_addr->
      write.rlFrameConfigParams.modParams.iramIndex = iram_idx;

    /* Set encParams IRAM index to new ping/pong index */
    muxmdsp_fw_smem_addr->
      write.rlFrameConfigParams.encParams.iramIndex = iram_idx;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }

  return iram_idx;

} /* muxmdsp_toggle_iram_index */

/*===========================================================================

FUNCTION MUXMDSP_SET_MOD_CH_CFG

DESCRIPTION
  This function sends updates channel 2 (FCH) modulator configurations
  in FW shared memory based on RC.

  Modulation
        31 => MULTI_BPSK_ON_I  (CH1)
        33 => MULTI_BPSK_ON_IQ (CH2_IS95a)
        32 => MULTI_BPSK_ON_Q  (CH2_IS95c, CH3)

  OVS Num
        0 : IS95a FCH, IS95a ACH
        1 : CH1, CH3_SF2_SCH, CH3_SF4_SCH
        2 : CH2_IS95C_FCH, CH3_SF8_EACH, CH3_SF16_SCH
        3 : CH3_SF8_SCH

  Spreading Factor
        1   => SF8
        2   => SF16
        3   => SF32
        4   => SF64
        5   => SF128
        6   => SF256
        8   => SF1
        9   => SF2
        10  => SF4
     others => Invalid

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_mod_ch_cfg
(
   enc_chan_number_type ch,
   enc_ovsf_type ovsf,
   enc_sprd_factor_type sprd,
   enc_modulation_type mod
)
{

  /*-----------------------------------------------------------------*/

  if( ( ch < ENC_CHAN_1 ) && ( ch > ENC_CHAN_3 ) )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid channel number - %d",
      ch );
  }
  else if( ( ovsf < ENC_OVSF_0 ) && ( ovsf > ENC_OVSF_4 ) )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid ovsf number - %d",
      ovsf );
  }
  else if( ( sprd < ENC_SF_8 ) && ( sprd > ENC_SF_4 ) )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Invalid sprd factor number - %d",
      sprd );
  }
  else
  {
    if( muxmdsp_fw_smem_addr )
    {
      cdmafw_rl_frame_mod_cfg_params_t* mod_ch_cfg;

      mod_ch_cfg = &(muxmdsp_fw_smem_addr->write.rlFrameConfigParams.modParams);

      mod_ch_cfg->channelCfg[ch].ovsfNum    = (uint16)ovsf;
      mod_ch_cfg->channelCfg[ch].sprdFactor = (uint16)sprd;
      mod_ch_cfg->channelCfg[ch].modulation = (uint16)mod;

      M1X_MSG( MUX, DBG_MDSP,
        "mod cfg: ch %u ovsf %u SF %u mod %u",
        ch,
        ovsf,
        sprd,
        mod );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
    }
  }
} /* muxmdsp_set_mod_ch_cfg */

/*===========================================================================

FUNCTION MUXMDSP_SET_TX_PCG_MASK

DESCRIPTION
  This function writes the transmit PCG mask to FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_tx_pcg_mask( uint32 pattern )
{

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    muxmdsp_fw_smem_addr->write.rlFrameConfigParams.transmitPCGMask = pattern;
  }
  else
  {
   M1X_MSG( MUX, LEGACY_ERROR,
     "MDSP shared memory ptr is NULL!" );
  }
} /* muxmdsp_set_tx_pcg_mask */

/*===========================================================================

FUNCTION MUXMDSP_SEND_FRAME_CONFIG

DESCRIPTION
  This function sends the FRAME CONFIG CMD message to FW to commit configured
  frame config params in shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_send_frame_config( void )
{
  uint32                       pcg_num        = 0;
  cdmafw_rl_frame_config_msg_t rl_frame_cfg_m = { { 0 } };

  /*-----------------------------------------------------------------*/

  m1x_stmr_get_frame_and_pcg_num_with_frame_offset( NULL, &pcg_num );

  if( pcg_num <= TXC_MAX_ALLOWED_PCG_FOR_FW_CONFIG )
  {
    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &rl_frame_cfg_m.hdr,
                      CDMA_FW_RL_FRAME_CONFIG_CMD,
                      sizeof(cdmafw_rl_frame_config_params_t) );

    M1X_MSG( MUX, LEGACY_MED,
      "Sent CDMA_FW_RL_FRAME_CONFIG_CMD in pcg %u",
      pcg_num );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Skipped sending CDMA_FW_RL_FRAME_CONFIG_CMD as pcg is %u past %d",
      pcg_num,
      TXC_MAX_ALLOWED_PCG_FOR_FW_CONFIG );
  }

} /* muxmdsp_send_frame_config */

/*===========================================================================

FUNCTION MUXMDSP_SET_TXFE_TRIGGER

DESCRIPTION
  This function sets the TxFE trigger point.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_send_txfe_trigger( cdmafw_rl_txfe_trigger_msg_t* txfe_trigger_m )
{
  /*-----------------------------------------------------------------*/

  if( txfe_trigger_m )
  {
    /* Register for a response */
    muxmsgr_register_msg( TX_TASK, CDMA_FW_RL_TXFE_TRIGGER_RSP );

    muxmsgr_set_start_rsp_flag();

    M1X_MSG( MUX, LEGACY_HIGH,
      "Sending CDMA_FW_RL_TXFE_TRIGGER_CMD to FW" );

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &(txfe_trigger_m->hdr),
                      CDMA_FW_RL_TXFE_TRIGGER_CMD,
                      sizeof(cdmafw_rl_txfe_trigger_msg_t) );

    /* Remember that the TXFE has been triggered */
    muxmdsp_tx_triggered = TRUE;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Passed NULL pointer for TxFE trigger" );
  }

} /* muxmdsp_send_txfe_trigger */

/*===========================================================================

FUNCTION MUXMDSP_STOP_TXFE

DESCRIPTION
  This message tells FW to stop TX. This command will execute in the upcoming
  frame boundary which means that FW will complete sending out the current
  frame in which this command is received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_stop_txfe( void )
{
  cdmafw_rl_txfe_stop_msg_t txfe_stop_m = { { 0 } };

  /*-----------------------------------------------------------------*/

  /* Stop TXFE only if triggered earlier */
  if( muxmdsp_tx_triggered )
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "Sending CDMA_FW_RL_TXFE_STOP_CMD" );

    /* Register for a response */
    muxmsgr_register_msg( TX_TASK, CDMA_FW_RL_TXFE_STOP_RSP );

    muxmsgr_set_stop_rsp_flag();

    /* Send message */
    muxmsgr_send_msg( TX_TASK,
                      &txfe_stop_m.hdr,
                      CDMA_FW_RL_TXFE_STOP_CMD,
                      sizeof(cdmafw_rl_txfe_stop_msg_t) );

    M1X_MSG( MUX, LEGACY_HIGH,
      "Waiting for CDMA_FW_RL_TXFE_STOP_RSP from FW" );

    /* Wait for a response from FW. FW will wait until the upcoming frame
       boundary to execute CDMA_FW_RL_TXFE_STOP_CMD. Waiting for FW to send
       a response could delay up to an entire frame. If this is an issue
       this should be revisted and optimized. */
    muxmsgr_wait_on_stop_rsp();

    M1X_MSG( MUX, LEGACY_HIGH,
      "Got CDMA_FW_RL_TXFE_STOP_RSP from FW" );

    /* Remember that TXFE has been stopped */
    muxmdsp_tx_triggered = FALSE;

    /* Deregister from the message */
    muxmsgr_deregister_msg( TX_TASK, CDMA_FW_RL_TXFE_STOP_RSP );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Skipping TXFE stop. TXFE not triggered." );
  }

} /* muxmdsp_stop_txfe */

/*===========================================================================

FUNCTION MUXMDSP_GET_LMEM_CH_ADDR

DESCRIPTION
  This function returns a pointer to where encoder data should be written
  in LMEM for a given channel.

DEPENDENCIES
  None.

RETURN VALUE
  Address of encoder channel memory in LMEM

SIDE EFFECTS
  None.

===========================================================================*/
uint32 * muxmdsp_get_lmem_ch_addr( enc_chan_number_type ch )
{
  uint32 * lmem_addr = NULL;

  /*-----------------------------------------------------------------*/

  if( muxmdsp_fw_smem_addr )
  {
    switch( ch )
    {
      case ENC_CHAN_1:
        lmem_addr = muxmdsp_fw_smem_addr->read.encInLMEM0AXIAddr
                                               [ENC_CHAN_1];
        break;
      case ENC_CHAN_2:
        lmem_addr = muxmdsp_fw_smem_addr->read.encInLMEM0AXIAddr
                                               [ENC_CHAN_2];
        break;
      case ENC_CHAN_3:
        lmem_addr = muxmdsp_fw_smem_addr->read.encInLMEM0AXIAddr
                                               [ENC_CHAN_3];
        break;
      default:
        break;
    }
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "MDSP shared memory ptr is NULL!" );
  }

  return lmem_addr;
} /* muxmdsp_get_lmem_ch_addr */
#endif /* !FEATURE_MODEM_1X_UTX */

/*===========================================================================

FUNCTION MUXMDSP_DATA_BRDG_ENABLE

DESCRIPTION
  This function informs FW about data call(SCH). For Turbo calls, it also
  takes care of enabling/disabling TDEC bridge. The message is used by FW to
  configure DEMBACK_MODE_SEL register to correct value based on active TECHs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_data_brdg_enable
(
  boolean data_enable,
  boolean tdec_enable
)
{
  /* Remember TDEC bridge state */
  static boolean tdec_brdg_enabled = FALSE;

  /* DEMBACK Mode Config Message */
  cdmafw_demback_mode_config_msg_t demback_mode_config_m = { { 0 } };

  /*-----------------------------------------------------------------*/

  MUXMDSP_ENTER_CRITICAL_SECTION();

  if( !data_enable && tdec_enable )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Trying to enable TDEC without data enabled." );

    /* Disable TDEC bridge always if data_enable=0 & Tdec bridge is enabled */
    demback_mode_config_m.tdecEnable_Update = tdec_brdg_enabled;
    demback_mode_config_m.tdecEnable = 0;

    /* Remember the new Tdec bridge state */
    tdec_brdg_enabled = FALSE;
  }
  else if( tdec_brdg_enabled != tdec_enable )
  {
    /* Update TDEC bridge as requested */
    demback_mode_config_m.tdecEnable_Update = TRUE;
    demback_mode_config_m.tdecEnable = tdec_enable;

    /* Remember the new Tdec bridge state */
    tdec_brdg_enabled = tdec_enable;
  }

  MUXMDSP_LEAVE_CRITICAL_SECTION();

  /* If we're disabling the TDEC bridge, make sure TDEC is idle first,
     since it could be writing out data to mempool.  Stopping the bridge
     in the middle of that will wedge TDEC HW. */
  if ( (demback_mode_config_m.tdecEnable_Update == TRUE) &&
       (demback_mode_config_m.tdecEnable == FALSE) )
  {
    /* Print some status for the logs */
    M1X_MSG( MUX, LEGACY_MED,
      "TDEC: idle=0x%x decib=0x%x brdg=0x%x",
      GET_DEMBACK_REG( TDEC_HW_IDLE ),
      #ifdef FEATURE_MODEM_1X_JOLOKIA
      0,
      #else
      GET_DEMBACK_REG( TDECIB_CLK_STATUS ),
      #endif
      GET_DEMBACK_REG( DEMBACK_BRDG_STATUS ) );

    if ( (GET_DEMBACK_REG_F( TDEC_HW_IDLE, TD_IDLE ) == 0)
         #ifndef FEATURE_MODEM_1X_JOLOKIA
         || (GET_DEMBACK_REG( TDECIB_CLK_STATUS ) != 0x00FFFFFF)
         #endif
       )
    {
      int countdown = 100; /* stop limit on spinwait iterations */

      /* Spin until TD_IDLE AND DECIB become idle */
      while ( ( (GET_DEMBACK_REG_F( TDEC_HW_IDLE, TD_IDLE ) == 0)
                #ifndef FEATURE_MODEM_1X_JOLOKIA
                || (GET_DEMBACK_REG( TDECIB_CLK_STATUS ) != 0x00FFFFFF)
                #endif
              ) &&
              (countdown-- > 0) )
      {
        DALSYS_BusyWait( 10 );
      }

      /* Extra drain time in case mempool flush hasn't finished yet */
      DALSYS_BusyWait( 100 );

      /* Print some status for the logs */
      M1X_MSG( MUX, LEGACY_MED,
        "TDEC was busy, now: idle=0x%x decib=0x%x brdg=0x%x count=%d",
        GET_DEMBACK_REG( TDEC_HW_IDLE ),
        #ifdef FEATURE_MODEM_1X_JOLOKIA
        0,
        #else
        GET_DEMBACK_REG( TDECIB_CLK_STATUS ),
        #endif
        GET_DEMBACK_REG( DEMBACK_BRDG_STATUS ),
        countdown );
    }
  }

  /* Update Data enable as per request */
  demback_mode_config_m.dataEnable = data_enable;

  /* Send message */
  muxmsgr_send_msg( RX_TASK,
                    &demback_mode_config_m.hdr,
                    CDMA_FW_DEMBACK_MODE_CONFIG_CMD,
                    sizeof(demback_mode_config_m) );

  /* wait for some time to have FW receive the above message and process it */
  DALSYS_BusyWait( 300 );

} /* muxmdsp_data_brdg_enable */

/*===========================================================================

FUNCTION MUXMDSP_PRINT_TX_TIMING_INFO

DESCRIPTION
  This function dumps out FW information regarding TxDAC and Tx timing

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_print_tx_timing_info( boolean show_tx_trigger_info )
{
  uint32 frame_count     = 0;
  uint32 pcg_num         = 0;
  #ifdef FEATURE_MODEM_1X_UTX
  uint32 mod_tx_sys_time = 0;
  uint32 mod_frame       = 0;
  uint32 mod_pcg         = 0;
  uint32 mod_cx1         = 0;
  uint32 sync80_rtc      = 0;
  boolean ta_is_enabled  = txc_is_ta_enabled();
  #endif /* FEATURE_MODEM_1X_UTX */

  /*-----------------------------------------------------------------*/

  /* If there's valid shared memory, dump out what FW has to say about
     Tx timing */
  if( muxmdsp_fw_smem_addr )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "1X TX: advCnt=%u retCnt=%u Diff.frmCnt=%d "
      "Diff.pcgCnt=%d Diff.chipCnt=%d, "
      "1X RX: stmrVsrcDiffCx8k[0/1] = %d / %d",
      muxmdsp_fw_smem_addr->read.txTimingInfo.advanceCount,
      muxmdsp_fw_smem_addr->read.txTimingInfo.retardCount,
      muxmdsp_fw_smem_addr->read.txTimingInfo.stmrSampleCntDiff.frameCount,
      muxmdsp_fw_smem_addr->read.txTimingInfo.stmrSampleCntDiff.pcgCount,
      muxmdsp_fw_smem_addr->read.txTimingInfo.stmrSampleCntDiff.chipCount,
      muxmdsp_fw_smem_addr->read.stmrVsrcDiffCx8k[0],
      muxmdsp_fw_smem_addr->read.stmrVsrcDiffCx8k[1] );
  }

  /* Get the PCG # and Frame counts */
  m1x_stmr_get_frame_and_pcg_num_with_frame_offset( &frame_count, &pcg_num );

  #ifdef FEATURE_MODEM_1X_UTX
  mod_tx_sys_time = HWIO_IN( TX_UNIFIED_SAMPLE_CNT_READ );
  mod_frame = (mod_tx_sys_time >> 17) & 0xFF;
  mod_pcg = (mod_tx_sys_time >> 13) & 0xF;
  mod_cx1 = mod_tx_sys_time & 0x1FFF;
  srch_mux_get_rtc_lcstate_at_sync80( ta_is_enabled, &sync80_rtc, NULL );

  M1X_MSG( MUX, LEGACY_MED,
     "SYNC80 RTC = 0x%08x, "
     "MOD_TX = [%03u][%02u][%04u], "
     "[FRAME][PCG] = [%03u][%02u]",
     sync80_rtc,
     mod_frame,
     mod_pcg,
     mod_cx1,
     frame_count,
     pcg_num );
  #else /* !FEATURE_MODEM_1X_UTX */

  M1X_MSG( MUX, LEGACY_MED,
     "RTC_CX8 = 0x%08x, "
     "SYNC80 [PHS][SCOMB_CX8] = [%02u][0x%08x], "
     "[FRAME][PCG] = [%03u][%02u]",
     m1x_stmr_get_rtc_count_cx8(),
     m1x_stmr_get_scomb_phase(),
     m1x_stmr_get_scomb_chipx8_count(),
     frame_count,
     pcg_num );
  #endif /* FEATURE_MODEM_1X_UTX */

} /* muxmdsp_print_tx_timing_info */

#ifdef FEATURE_MODEM_1X_SW_DEMBACK
/*===========================================================================

FUNCTION MUXMDSP_SET_IS_SW_DEMBACK_ACTIVE

DESCRIPTION
  This function sets the variable which indicates if SW DEMBACK is active or not

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_is_sw_demback_active( boolean sw_db_active )
{
  muxmdsp_is_sw_demback_active = sw_db_active;

  M1X_MSG( MUX, LEGACY_MED,
    "WRITE : IS_SW_DEMBACK in Use %d", muxmdsp_is_sw_demback_active );

} /* muxmdsp_set_is_sw_demback_active */

/*===========================================================================

FUNCTION MUXMDSP_GET_IS_SW_DEMBACK_ACTIVE

DESCRIPTION
  This function gets the variable which indicates if SW DEMBACK is active or not

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean muxmdsp_get_is_sw_demback_active( void )
{
  M1X_MSG( MUX, LEGACY_MED,
    "READ : IS_SW_DEMBACK in Use %d", muxmdsp_is_sw_demback_active );

  return( muxmdsp_is_sw_demback_active );

} /* muxmdsp_get_is_sw_demback_active */
#endif /* FEATURE_MODEM_1X_SW_DEMBACK */

/*===========================================================================

FUNCTION MUXMDSP_SET_DEMBACK_MODE

DESCRIPTION
  This function instructs FW whether to use Demback HW or not,
  and if not, what frame sizes are being used.  Blocks on response msg
  from FW.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_demback_mode(boolean enable_hw, boolean is_20ms_frame)
{
  #ifndef FEATURE_MODEM_1X_SW_DEMBACK
  MODEM_1X_UNUSED( enable_hw );
  MODEM_1X_UNUSED( is_20ms_frame );
  #else /* supports SW_DEMBACK */
  cdmafw_demback_sw_config_msg_t demback_sw_config = { { 0 }, 0 };

  /*-----------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_HIGH,
    "Set 1x Demback Mode(hw=%u,20ms=%u)",
    enable_hw,
    is_20ms_frame );

  demback_sw_config.debugModeEn = FALSE;

  /* Only enable SW demback if HW enable flag is false */
  demback_sw_config.swDembackEn_Update = TRUE;
  demback_sw_config.swDembackEn = !enable_hw;

  muxmdsp_set_is_sw_demback_active( !enable_hw );

  /* Only set 20ms vs 80ms symbol buffer ping/pong rate in SW demback mode */
  if ( demback_sw_config.swDembackEn )
  {
    demback_sw_config.syncStateEn_Update = TRUE;
    demback_sw_config.syncStateEn = !is_20ms_frame;
  }

  /* Send the message and block until the response comes back */
  muxmdsp_msgr_send_and_wait_for_response( &demback_sw_config.hdr,
                                           CDMA_FW_DEMBACK_SW_CONFIG_CMD,
                                           sizeof(demback_sw_config),
                                           CDMA_FW_DEMBACK_SW_CONFIG_RSP );
  #endif /* FEATURE_MODEM_1X_SW_DEMBACK */

} /* muxmdsp_set_demback_mode */

/*===========================================================================

FUNCTION MUXMDSP_EXTRACT_CH0_SYMS

DESCRIPTION
  This function extracts the soft decision symbols for either the current
  or previous frame, when FW is in soft demback mode.  Max number of symbols
  is passed to limit the size of the copy, if desired.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
int muxmdsp_extract_ch0_syms( boolean is_previous_frame,
                              boolean is_i_only,
                              int8 *pSymbols,
                              uint32 max_symbols )
{
  int num_symbols = 0;

  /*-----------------------------------------------------------------*/

  if( srch_mux_mdsp_demod_running() )
  {
    if( muxmdsp_fw_smem_addr && pSymbols )
    {
      uint32 current_sym, current_buffer, symbols_to_read;

      M1X_MSG( MUX, DBG_MDSP,
               "DEBUG: fwBufferWrIdx=%u fwSymbolWrIdx=%03u fwFrameIdx=%u",
               muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwBufferWrIdx,
               muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwSymbolWrIdx,
               muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwFrameIdx );

      /* Handle current vs. previous frame indexing */
      if( is_previous_frame )
      {
        /* It could be that FW has written the last symbol of the previous
           frame, but hasn't yet flipped the ping/pong index to the current
           frame.  Handle that case by detecting last symbol index written,
           meaning that FW is, in effect, done writing the previous frame. */
        if ( muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwSymbolWrIdx ==
             (CDMAFW_NUM_SYMBOLS_PER_FRAME - 1) )
        {
          current_buffer =
            muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwBufferWrIdx;
        }
        else
        {
          current_buffer =
            !muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwBufferWrIdx;
        }
        symbols_to_read = CDMAFW_NUM_SYMBOLS_PER_FRAME;
      }
      else
      {
        current_buffer =
          muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwBufferWrIdx;
        symbols_to_read =
          muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.fwSymbolWrIdx;
      }

      /* Read the FW packed symbols and convert to unpacked int8 output
         format in the order I Q I Q ...  (with Q possibly being suppressed) */
      for( current_sym = 0;
           ( current_sym < symbols_to_read ) && ( num_symbols < max_symbols );
            current_sym++ )
      {
        /* Grab the packed I/Q symbol and scale it to fit a signed int8 */
        uint16 packed_sym =
          muxmdsp_fw_smem_addr->read.ch0SymBufferInfo.
            symbolBuffer[current_buffer][current_sym] << 2;

        /* Read the i symbol */
        pSymbols[num_symbols] = (int8)(packed_sym >> 8);
        pSymbols[num_symbols++] >>= 2;

        /* Read & sign-extend the q symbol, if desired */
        if( !is_i_only )
        {
          pSymbols[num_symbols] = (int8)packed_sym;
          pSymbols[num_symbols++] >>= 2;
        }
      }

      M1X_MSG( MUX, DBG_MDSP,
               "DEBUG: Extracted %u symbols from frame idx = %u",
               num_symbols,current_buffer );

      M1X_MSG( MUX, DBG_MDSP,
               "DEBUG: symbols[0..8] = %d,%d,%d,%d,%d,%d,%d,%d,%d",
               pSymbols[0],pSymbols[1],pSymbols[2],
               pSymbols[3],pSymbols[4],pSymbols[5],
               pSymbols[6],pSymbols[7],pSymbols[8] );
    }
    else
    {
      M1X_MSG( MUX, DBG_MDSP,
        "MDSP shared memory ptr or destination pointer is NULL!");
      num_symbols = -1;
    }
  }
  else
  {
    num_symbols = -1;
  }

  return num_symbols;

} /* muxmdsp_extract_ch0_syms */

/*===========================================================================

FUNCTION MUXMDSP_UPDATE_FW_DIAG_BUF

DESCRIPTION
  This function dumps data bytes sent accross DIAG to a FW shared memory
  diag buffer.

DEPENDENCIES
  None.

RETURN VALUE
  -1 - FAILURE
   0 - SUCCESS

SIDE EFFECTS
  None.

===========================================================================*/
int muxmdsp_update_fw_diag_buf( uint8 * data, uint16 len )
{
  cdmafw_qxdm_diag_data_msg_t qxdm_diag_data_m = { { 0 } };
  int ret_val = 0;

  /*-----------------------------------------------------------------*/

  if( len > CDMAFW_QXDM_DIAG_DATA_BUF_SIZE_BYTES )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Diag payload > qxdmDiagBuffer: %u > %u",
      len,
      CDMAFW_QXDM_DIAG_DATA_BUF_SIZE_BYTES );

    ret_val = -1;
  }
  else
  {
    if( muxmdsp_fw_smem_addr )
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Writing %u bytes to FW qxdmDiagBuffer",
        len );

      qxdm_diag_data_m.lengthBytes = len;

      memscpy( muxmdsp_fw_smem_addr->write.qxdmDiagDataBuffer,
               CDMAFW_QXDM_DIAG_DATA_BUF_SIZE_BYTES,
               data,
               len );

      /* Send message */
      muxmsgr_send_msg( RX_TASK,
                        &qxdm_diag_data_m.hdr,
                        CDMA_FW_QXDM_DIAG_DATA_CMD,
                        sizeof(qxdm_diag_data_m) );
    }
    else
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "MDSP shared memory ptr is NULL!" );
      ret_val = -1;
    }
  }

  return ret_val;
} /* muxmdsp_update_fw_diag_buf */

/*===========================================================================

FUNCTION MUXMDSP_START_FLEX_CAPTURE

DESCRIPTION
  This function writes to QDSP6 Flex address to trigger the FLEX capture.
  For BOLT and THOR, Bit 15 is used for starting tx_capture.
  For Jolokia, Bit 20 is used for starting tx_capture.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_start_flex_capture( void )
{
  #ifdef FEATURE_MODEM_1X_RUMI
  static boolean tx_capture_triggered = FALSE;

  /*-----------------------------------------------------------------------*/

  if( tx_capture_triggered == FALSE )
  {
    volatile uint32 *flex_addr;

    M1X_MSG(MUX, LEGACY_HIGH,
      "Triggering Flex capture" );

    #ifdef FEATURE_MODEM_1X_JOLOKIA
    flex_addr = (volatile uint32 *)0xEC18F800;
    /* Trigger flex to start recording */
    *flex_addr= 0x100000;
    #elif defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
    flex_addr = (volatile uint32 *)0xEC1AA120;
    /* Trigger flex to start recording */
    *flex_addr= 0x8000;
    #elif defined( FEATURE_MODEM_1X_THOR )
    flex_addr = (volatile uint32 *)0xEC1AA0FC;
    /* Trigger flex to start recording */
    *flex_addr= 0x8000;
    #elif defined( FEATURE_MODEM_1X_BOLT )
    flex_addr = (volatile uint32 *)0xEC981174;
    /* Trigger flex to start recording */
    *flex_addr= 0x8000;
    #endif /* FEATURE_MODEM_1X_BOLT */

    tx_capture_triggered = TRUE;
  }
  #endif /* FEATURE_MODEM_1X_RUMI */

} /* muxmdsp_start_flex_capture */

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_mux_data

DESCRIPTION
This function dumps the mux data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_mux_data()
{
  qsh_mdump_collect_high(&muxmdsp_tx_triggered,sizeof(muxmdsp_tx_triggered));
  mc_qsh_mdump_collect_rxccommon_data();
  mc_qsh_mdump_collect_rxctraffic_data();
  mc_qsh_mdump_collect_txccommon_data();
  mc_qsh_mdump_collect_txctraffic_data();
}
#endif /* FEATURE_QSH_MDUMP */