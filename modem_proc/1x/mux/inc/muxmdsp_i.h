#ifndef MUXMDSP_I_H
#define MUXMDSP_I_H

/*===========================================================================

                      MUX mDSP interface header File

General Description
  This file contains declarations for MUX mDSP interface data structures and
  function protopes.


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
  muxmdsp_get_tx_gain_adjust
  muxmdsp_set_tx_gain_adj_step_size
  muxmdsp_wr_tx_rate_adj_latch_offset
  muxmdsp_wr_fundFrameOffset
  muxmdsp_wr_fundFrameOffsetImmLoadCmd
  muxmdsp_set_agc_ctl_mask_delay
  muxmdsp_init
  muxmdsp_access_pwr_adjust
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
  muxmdsp_get_early_decode_pcg
  muxmdsp_enable_apf
  muxmdsp_set_rliFPCMask
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

  Copyright (c) 1993 - 2016 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/mux/inc/muxmdsp_i.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
06/24/15   srk     1x SW support for 1x FW DEMBACK changes in ATLAS
06/22/15   eye     Dont disable data bridge if SW DEMBACK is active.
03/18/15   eye     Clear muxmdsp_dac_trigger in Tx disable part of FTM mode.
02/12/15   jh      Add muxmdsp_extract_ch0_syms() API
                   Add muxmdsp_set_demback_mode() API
10/24/14   jh      Remove unused API
10/08/14   srk     FEATURE_MODEM_1X_UTX related cleanup.
08/15/14   as      Clean up rlFrameConfigParams for Jolokia
02/13/14   dkb     Add support for optimized Tx trigger.
11/21/13   vks     Add api to return tdec decob address
10/24/13   dkb     Add changes to address FL/RL discrepencies.
10/16/13   dkb     Add changes for R-FCH functionality without FL dependency.
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
10/08/13   vks     Update to new FW interface - fing config has the walsh code
                   and qof
09/30/13   dkb     Initial RL changes for new FW interface.
09/27/13   vks     Use rxccommon api to configure ch0 demod for sync/paging
09/26/13   dkb     Add support for FW diag buffer
09/13/13   vks     Add api to update code beta for demod channels
09/11/13   vks     Add api to update ch0,ch1,ch2,fpc,rpc and frame offset
09/06/13   srk     Remove unused APIs and global variables
08/27/13   eye     Add muxmdsp_init() for module initialization.
11/13/12   trc     Layering/Cleanup for Triton
11/09/12   srk     Remove unused API.
11/08/12   cjb     Making previous change backward compatible with Triton.
11/07/12   cjb     Switched to new FW interface which takes care of configuring
                   DEMBACK_MODE_SEL to proper values based on the active TECHs.
10/30/12   srk     Removed unused function export.
10/19/12   srk     Create an API to read frame sync'ed tx gain adjust from FW.
09/11/12   srk     Remove DTX frame flag and DTX t2p gain ratio boost.
08/27/12   srk     Apply T2P gain ratio boost of tx power boost.
08/16/12   jtm     Added TDEC bridge MSGR command.
08/02/12   srk     Added support for Device only DTX for 1x Advanced calls.
05/30/12   srk     Changes to remove half PCG offset in 1x advanced logs.
04/16/12   jtm     Moved QOF FCH Noise Estimation to SRCH.
01/11/12   srk     Feature Cleanup
12/01/11   srk     Added muxmdsp_config_qof_fch_noise_est function declaration
11/04/11   trc     Add muxmdsp_print_tx_timing_info() debug API
09/02/11   sk      Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/18/11   adw     Mainline 1x message router support.
08/12/11   jtm     Added muxmdsp_set_rliFPCMask.
07/28/11   vks/jtm Changes to get 1xA working on nikel.
06/20/11   jtm     Wrap APF support with FEATURE_IS2000_1X_ADV.
06/17/11   jtm     Added support to enable/disable Adaptive Pilot Filtering (APF)
06/13/11   adw     Added function to send Tx DAC stop message to FW.
05/09/11   trc     NikeL FW support
04/27/11   jtm     Added muxmdsp_get_early_decode_pcg.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
11/24/10   ag      Renamed pwr_cntl_step_zero_rate to pwr_cntl_step.
10/30/10   jtm     Modem 1x SU API changes.
09/23/10   adw     Added prototype for muxmdsp_get_tx_gain_adjust function.
08/23/10   jtm     Eliminated obsolete DV support. Changed macros to functions.
06/15/10   jtm     Cleaned up 1xA Logging. Changed muxmdsp_read_1xA_ack_log
                   to return the PCG that an ACK was received on the F-ACKCH.
                   Added capability to configure ACK/NAK test mode in FW.
06/10/10   jtm     Eliminated dependency on log_dmss.h.
05/19/10   vks     With the new MSGR interface, L1 needs to send FW a message
                   directly for the tx gain adj step size.
05/18/10   vks     Mainline FEATURE_MDSP_SUPPORTS_TX_AGC.
05/14/10   vks     Add function for setting agc ctl mask delay. This wrapper
                   function uses FW msg for MSGR interface, legacy MUXMSM_*
                   macro for legacy.
04/22/10   vks     Remove references to the hard coded fw shared memory addr.
                   Update MUX Mdsp module with the addr when app enabled.
03/25/10   vks     Send FW a message directly instead of using RF macros for
                   tx gain adjust override functionality.
04/12/10   jtm     Added FCH FPC setpoint freeze and RC8 MPP support.
                   Added precompiler check for CDMA1X_MOD_PRMBL_GAIN*
                   definitions before attempting to use them.
                   Removed frame number parameter from
                   muxmdsp_read_1xA_ack_log()
01/28/10   jtm     Corrected type for txc_tx_rate_adj to signed 16bit int in
                   muxmdsp_channel_gains_type.
12/21/09   jtm     Updated 1xA logging mechanism to align PCGs of each frame.
12/04/09   sst     Add modem_1x_defs.h include and fix a featurization
12/03/09   vks     Cleanup.
11/30/09   vks     Group together mdsp writes for 1xADV channel gain ratios.
11/17/09   vks     Delete MDSP_ACTIVE macro and general cleanup
11/12/09   vks     Add MSGR/Q6 framework support.
11/07/09   vks     Created a global data structure to contain all the channel
                   gains. Export API to set these variables.
                   Do not export muxmdsp_wr_mod_<channel>_gain() functions.
10/19/09   jtm     Added 1xA logging support.
10/19/09   vks     Moved mdsp read/writes in msm_drv.h to muxmdsp.h and
                   renamed these macros from demod* to muxmdsp*
10/08/09   vks     Moved mdsp read/writes in muxmsm.h to muxmdsp.h and
                   renamed these macros from muxmsm* to muxmdsp*
07/06/09   jtm     Update to use cai_rcp_rxc_txc_type.
06/11/09   jtm     Additional 1x Advanced RL support.
06/06/09   vlc     Added support for 1x Advanced (RC11.)
04/08/09   jtm     Initial 1X Advanced feature support.
04/01/09   jtm     Added customer.h
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code and obsolete prototypes.
07/01/08   vlc     Merged code changes from old tip archive.
02/23/08   vlc     Added new interface to MDSP to notify MDSP of entrance and
                   exit of RL access state mainly to fine tune access probe
                   power management (FEATURE_ACCESS_PROBE_PWR_ADJ.)
09/13/07   vlc     Increased MUXMDSP_TX_RATE_ADJ_LATCHED_OFFSET for targets
                   with HW controlled TXAGC.
02/09/07   vlc     Put inclusion of muxdvlog.h under REL C feature.
06/09/06   trc     Remove T_MSM6700 featurization
03/14/05   vlc     Fixed compiler warnings for MSM6700.
11/19/04   jyw     Removed the include to the "dv_swInteface.h"
11/08/04   sr      Added prototype for muxmdsp_wr_pdcchStartDelay()
10/18/04   vlc     Fixed typo, muxmdsp_rd_ackHistory is now muxmdsp_rd_ackchHistory.
09/10/04   ljl     Added the enum type muxmdsp_dvchannel_rl_gain_pdch_mode_type
                   and the function muxmdsp_wr_rlGainPdchMode().
09/01/04   jrp     Added 0.5x counters.
09/01/04   sr      Added MUX to list of mDSP Msg recipients
09/01/04   jrp     Conditionally compile entire file.
07/30/04   vlc     Added support for rel C TDSO.
07/29/04   jrp     Added interface for mdsp2arm_int_reg_clt_callback_function.
07/28/04   jrp     Renamed Rev D gain enums.  Removed lint warnings.
07/27/04   bn      Added the defined value for tx_rate_adj latched offset
07/26/04   bn      Added new function for tx_rate_adj latched frame offset
07/23/05   jyw     Added the definition of function
                   muxmdsp_wr_pdchInitialDescrambleState
07/21/04   bn      Added function for MDSP frame offset loading command
06/29/04   sr      Added prototypes for pass-through functions
06/24/04   sr      Added mdsp2arm_int_handler
06/18/04   bn      Added support for Rel D channel gains override
05/18/04   sr      Added prototype for startSwitchCmd
05/04/04   bn      Added the MDSP to ARM universal INT handler and registering utilizes
03/17/04   bn      Modified the register callback function to allow deregistering
03/09/04   sr/bkm  Featurized DV code
09/09/03   bn      Created MUX mDSP interface header File
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

//driver
#include "enc_i.h"

//mux
#include "ffpc_i.h"

//drivers
#include "dalint_1x.h"
#include "deint_i.h"

//cp
#include "cai.h"
#include "cai_v.h"

//feature dependent
#include "cdmafw_msg.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* This is the offset time between frame boundary and when MDSP updates digital
** channel gains and txRateAdj to HW.  Increase offset for targets with HW
** controlled TXAGC.  FW controlled TXAGC remains at 100 chips.
*/
/* define the TX_RATE_ADJ latched offset */
#define   MUXMDSP_TX_RATE_ADJ_LATCHED_OFFSET    100 /* 100 chips */

/* 256 * PI / 2 = 0x192 - part of the necessary scaling factor to
   convert the new magnitude values read from the MDSP in the macros
   MUXMDSP_GET_FCH_NT_IO AND MUXMDSP_GET_SCH_NT_IO */
#define MAG_SCALE                                           0x192

/* MDSP channel gains data structure */
typedef struct
{
  word txc_pch_gain;      /* 15 bit resolution rev. pilot ch gain            */
  word txc_fch_gain;      /* 15 bit resolution rev. fundamental ch gain      */
  word txc_sch_gain;      /* 15 bit resolution rev. dedicated control ch gain*/
  word txc_dcch_gain;     /* 15 bit resolution rev. supplemental ch gain     */
  int16 txc_tx_rate_adj;  /* Overall Tx power adjust gain, -1/640 dB units   */
  boolean frame_valid;    /* Used for PA/TX control in NikeL and beyond      */
} muxmdsp_channel_gains_type;

extern muxmdsp_channel_gains_type channel_gains;

#ifdef FEATURE_IS2000_1X_ADV
typedef struct
{
  word                        fch_ack_mask;
  word                        sch_ack_mask;
  word                        fch_linear_gain_ratio;
  word                        dcch_linear_gain_ratio;
  word                        sch_linear_gain_ratio;
  word                        fch_ack_linear_gain_ratio;
  word                        sch_ack_linear_gain_ratio;
  boolean                     blanked_frame;
  boolean                     frame_valid;
  cai_rcpm_pwr_cntl_mode_type rpc_mode;
} muxmdsp_rc8_frame_config_parms;

extern muxmdsp_rc8_frame_config_parms rc8_frame_config_parms;
#endif /* FEATURE_IS2000_1X_ADV */

typedef struct
{
  uint16 pcFiltGain;
  uint16 ntIoFiltGain;
  uint16 ebNtMagTargetFch;
  uint16 ebNtMagTargetSch;
} muxmdsp_sw_cfg_fw_variables_type;

extern muxmdsp_sw_cfg_fw_variables_type muxmdsp_sw_cfg_fw_variables;

/* FW shared memory address */
extern cdmafw_shared_memory_interface_t* muxmdsp_fw_smem_addr;

typedef enum
{
  MUXMDSP_DEMOD_CH0    = 0,
  MUXMDSP_DEMOD_CH1    = 1,
  MUXMDSP_DEMOD_CH2    = 2,
  MUXMDSP_DEMOD_MAX_CH = 3
} muxmdsp_demod_num_ch_enum_type;

typedef enum
{
  MUXMDSP_WL_MASK_4    = 0,
  MUXMDSP_WL_MASK_8    = 1,
  MUXMDSP_WL_MASK_16   = 2,
  MUXMDSP_WL_MASK_32   = 3,
  MUXMDSP_WL_MASK_64   = 4,
  MUXMDSP_WL_MASK_128  = 5
} muxmdsp_wl_mask_enum_type;

typedef enum
{
  MUXMDSP_FFEBIAS_WL_MASK_64  = 13,
  MUXMDSP_FFEBIAS_WL_MASK_128 = 14
} muxmdsp_ffebias_wl_mask_enum_type;

typedef cdmafw_demod_ch0_ctrl_t       muxmdsp_demod_ch0_type;
typedef cdmafw_demod_ch1_ctrl_t       muxmdsp_demod_ch1_type;
typedef cdmafw_demod_ch2_ctrl_t       muxmdsp_demod_ch2_type;
typedef cdmafw_demod_walsh_beta_cfg_t muxmdsp_demod_walsh_beta_type;

/* Flag to remember TXFE/TXDAC state */
extern boolean muxmdsp_tx_triggered;

/*===========================================================================

                        FUNCTIONS

===========================================================================*/

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
);

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
);

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
);

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
);

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
);

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
  uint16 fpcSel
);

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
);

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
void muxmdsp_update_tx_open_loop_override
(
  uint16 override_mask
);

/*===========================================================================

FUNCTION        muxmdsp_get_open_loop_tx_power

DESCRIPTION     This returns the Open loop TX power from FW smem

DEPENDENCIES    None.

RETURN VALUE    Open Loop TX power in dBm.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_open_loop_tx_power( void );

/*===========================================================================

FUNCTION         muxmdsp_get_demback_id_index

DESCRIPTION     This returns the DEMBACK ID index in FW.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    DEMBACK ID Index

SIDE EFFECTS    None.

===========================================================================*/
uint32 muxmdsp_get_demback_id_index( void );

/*===========================================================================

FUNCTION muxmdsp_get_demback_irq

DESCRIPTION     This returns the DEMBACK IRQ for the DEMBACK that's currently
                being used for 1x operation.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    DEMBACK IRQ

SIDE EFFECTS    None.

===========================================================================*/
dalint_1x_irq_types muxmdsp_get_demback_irq( void );

/*===========================================================================

FUNCTION muxmdsp_get_demback_tdec_decob_addr

DESCRIPTION     This returns the tdec decob address that's currently being
                used for 1x operation.

DEPENDENCIES    FW should be enabled.

RETURN VALUE    tdec decob address

SIDE EFFECTS    None.

===========================================================================*/
uint32* muxmdsp_get_demback_tdec_decob_addr( void );

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
);
#endif /* FEATURE_MODEM_1X_DRX */

/*===========================================================================

FUNCTION muxmdsp_get_pc_filt_gain

DESCRIPTION     Read the PC Filter Gain from MDSP.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_pc_filt_gain( void );

/*===========================================================================

FUNCTION muxmdsp_set_pc_filt_gain

DESCRIPTION     Sets the filter gain for the forward link PC bit

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
void muxmdsp_set_pc_filt_gain( uint16 gain );

/*===========================================================================

FUNCTION muxmdsp_get_nt_io_filt_gain

DESCRIPTION

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_nt_io_filt_gain( void );

/*===========================================================================

FUNCTION muxmdsp_set_nt_io_filt_gain

DESCRIPTION     This macro sets the filter gain for the forward link Nt/Io

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
void muxmdsp_set_nt_io_filt_gain( uint16 gain );

/*===========================================================================

FUNCTION muxmdsp_get_eb_no_target_fch

DESCRIPTION     Read the Eb/No target for FCH from the mDSP.

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_eb_no_target_fch( void );

/*===========================================================================

FUNCTION muxmdsp_get_eb_no_target_sch

DESCRIPTION     Read the Eb/No target for SCH from the mDSP.

DEPENDENCIES

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_eb_no_target_sch( void );


/*===========================================================================

FUNCTION muxmdsp_get_sch_iq_acc

DESCRIPTION     This macro returns the IQ accumulation of channel 2
                over a frame.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_iq_acc( void );

/*===========================================================================

FUNCTION muxmdsp_get_pc_acc

DESCRIPTION     This returns the PC bit accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_pc_acc( void );

/*===========================================================================

FUNCTION muxmdsp_get_fch_nt_io

DESCRIPTION     This returns the fundamental weighted Nt/Io
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_fch_nt_io( void );

/*===========================================================================

FUNCTION muxmdsp_get_sch_nt_io

DESCRIPTION     This returns the supplemental weighted Nt/Io
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_sch_nt_io( void );

/*===========================================================================

FUNCTION muxmdsp_fpc_get_history

DESCRIPTION     Returns a sample of the FPC history.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_fpc_get_history( void );

/*===========================================================================

FUNCTION muxmdsp_fpc_get_history_valid

DESCRIPTION     Returns whether the FPC history is valid.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_fpc_get_history_valid( void );

/*===========================================================================

FUNCTION muxmdsp_rpc_get_history

DESCRIPTION     Returns a sample of the RPC history.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_history( void );

/*===========================================================================

FUNCTION muxmdsp_rpc_get_history_valid

DESCRIPTION     Returns whether the RPC history is valid.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_history_valid( void );

/*===========================================================================

FUNCTION muxmdsp_rpc_get_cell_history

DESCRIPTION     Return a sample of the RPC history for the
                specific cell.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_rpc_get_cell_history( uint16 cell );

/*===========================================================================

FUNCTION        muxmdsp_get_frame_sync_tx_gain_adjust_fw

DESCRIPTION     Returns the Frame Synchronized Tx Gain Adjust from FW.

DEPENDENCIES    None.

RETURN VALUE    Tx Gain Adjust in -1/4 dB.
                0x8000 - Error or invalid value.

SIDE EFFECTS    None.

===========================================================================*/
int16 muxmdsp_get_frame_sync_tx_gain_adjust_fw( void );

/*===========================================================================

FUNCTION muxmdsp_get_fch_wtd_pilot


DESCRIPTION     This returns the fundamental weighted pilot magnitude
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_fch_wtd_pilot( void );

/*===========================================================================

FUNCTION muxmdsp_get_fch_rt_nt_io


DESCRIPTION     This returns the fundamental weighted sqrt(Nt/Io)
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_fch_rt_nt_io( void );

/*===========================================================================

FUNCTION muxmdsp_get_sch_rt_nt_io

DESCRIPTION     This macro returns the supplemental weighted sqrt(Nt/Io)
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_rt_nt_io( void );

/*===========================================================================

FUNCTION muxmdsp_get_sch_wtd_pilot

DESCRIPTION     This returns the supplemental weighted pilot magnitude
                accumulation over a frame

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
uint16 muxmdsp_get_sch_wtd_pilot( void );

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
);

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION muxmdsp_set_rc8_frame_config_parms

DESCRIPTION
  Writes the following mdsp registers:
  1. CDMA1X_rliFchAckMask
  2. CDMA1X_rliSchAckMask
  3. CDMA1X_rliFchToPich
  4. CDMA1X_rliDcchToPich
  5. CDMA1X_rliSchToPich
  6. CDMA1X_rliFchAckchToPich
  7. CDMA1X_rliSchAckchToPich
  8. CDMA1X_rliBlankedFrame
  9. CDMA1X_rliRPCMask
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
void muxmdsp_set_rc8_frame_config_parms( void );

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
void muxmdsp_set_rliFPCMask( fpc_mode_type fpc_mode );

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
void muxmdsp_set_rc8_frame_config_parms_zero( void );

#endif /* FEATURE_IS2000_1X_ADV */

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
void muxmdsp_set_eb_no_target_fch( uint16 target );

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
void muxmdsp_set_eb_no_target_sch( uint16 target );

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
void muxmdsp_set_channel_gains( void );

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
void muxmdsp_set_channel_gains_zero( void );

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
void muxmdsp_set_tx_gain_adjust( int16 gain );

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
int16 muxmdsp_get_tx_gain_adjust( void );

/*===========================================================================

FUNCTION MUXMDSP_SET_TX_GAIN_ADJ_STEP_SIZE

DESCRIPTION
  This function sends to the FW the current TX Gain Adjust Step size.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void muxmdsp_set_tx_gain_adj_step_size
(
  cai_pwr_cntl_step_type step_size              /* Step Size               */
);

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
void muxmdsp_wr_tx_rate_adj_latch_offset( word tx_rate_adj_latch_offset );

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
void muxmdsp_wr_fundFrameOffset( uint16  fundoffset_value );

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
void muxmdsp_set_agc_ctl_mask_delay( uint8 delay );

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
void muxmdsp_init( void );

#ifdef FEATURE_IS2000_1X_ADV
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
cai_data_rate_type muxmdsp_rd_rc11_fch_rda_info( void );

/*===========================================================================

FUNCTION muxmdsp_is_rc11_ffpc_frame_valid

DESCRIPTION
  This function reads FCH valid FFPC frame indicator for RC11 from MDSP.  It is
  called when SW receives an MDSP FCH decode done interrupt.

  This variable is used to determine whether this frame RDA result should be
  used to update FFPC setpoint.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:   Use this frame RDA result to update FFPC setpoint (read mon-zero value
          from MDSP.)
  FALSE:  Do not update FFPC setpoint this frame (read zero from MDSP.)

SIDE EFFECTS
  None.

===========================================================================*/
boolean muxmdsp_is_rc11_ffpc_frame_valid( void );

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
void muxmdsp_update_rc11_params ( cai_rcp_rxc_txc_type *rc11_rpc_data );

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
void muxmdsp_set_rc11_mode ( boolean enable_flag );

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
void muxmdsp_enable_1xA_logging ( boolean enable_flag );

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

 To line up with PCG 0 of the current frame this function skips the first
 half PCG's worth of data (6 32bit values) of buffer0. The log is filled in
 subsequently following buffer0's data. This ultimately leaves room for the
 last half PCG of the frame to be appended to the log
 (handled by another subroutine).

DEPENDENCIES
  MDSP must be running.

RETURN VALUE
  None.

SIDE EFFECTS
  If the mdsp buffer is buffer0, 32bit values 6-91 are copied to the log buffer.
  For buffer1, 32bit values 0-91 are copied to the log buffer at an index offset
  by one half pcg's worth of data (6 32bit values).
===========================================================================*/
void muxmdsp_read_1xa_log_buffer
(
  uint16 *entry_index,
  uint16 max_log_size,
  uint32 *sw_log_buf,
  uint8 mdsp_buff_num
);

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
uint8 muxmdsp_read_1xA_ack_log ( void );

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
boolean muxmdsp_fch_fpc_setpoint_is_frozen(void);


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
);

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
);


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
);

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
uint16 muxmdsp_get_early_decode_pcg ( void );

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
uint8 muxmdsp_get_1xa_log_buff_index ( void );

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
void muxmdsp_enable_apf( boolean enable );
#endif /* FEATURE_IS2000_1X_ADV */


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
void muxmdsp_configure_rl_rc ( cai_radio_config_type rc );

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
void muxmdsp_send_txdac_trigger( cfw_tx_dac_start_msg_t* txdac_trigger_m );

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
void muxmdsp_stop_txdac( void );

#else /* FEATURE_MODEM_1X_UTX */
/*===========================================================================

FUNCTION MUXMDSP_ENC_CH_EN

DESCRIPTION
  This function writes to FW shared memory to enable encoder channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_enc_ch_en( uint32 ch, boolean enable );

/*===========================================================================

FUNCTION MUXMDSP_SET_ENC_INPUT_LEN

DESCRIPTION
  This function writes the encoder input length to FW shared memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_enc_input_len( uint32 channel, uint32 len );

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
uint32 muxmdsp_get_enc_input_len( uint32 channel );

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
void muxmdsp_set_frame_config_data_rate( uint32 rate );

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
void muxmdsp_set_enc_ch1_params( cdmafw_rl_enc_ch1_cfg_t * parm );

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
void muxmdsp_get_enc_ch1_params( cdmafw_rl_enc_ch1_cfg_t * parm );

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
void muxmdsp_set_enc_ch2_params( cdmafw_rl_enc_ch2_cfg_t * parm );

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
void muxmdsp_get_enc_ch2_params( cdmafw_rl_enc_ch2_cfg_t * parm );

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
void muxmdsp_set_enc_ch3_params( cdmafw_rl_enc_ch3_cfg_t * parm );

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
void muxmdsp_get_enc_ch3_params( cdmafw_rl_enc_ch3_cfg_t * parm );

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
void muxmdsp_set_rl_lc_state( uint64 user_pn_state );

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
uint64 muxmdsp_get_rl_lc_state( void );

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
void muxmdsp_set_rl_lc_mask( uint64 user_pn_mask );

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
void muxmdsp_set_rl_spn_state( uint32 i_pn_state, uint32 q_pn_state );

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
enc_spn_state_type muxmdsp_get_rl_spn_state( void );

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
uint32 muxmdsp_toggle_iram_index( void );

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
);

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
void muxmdsp_set_tx_pcg_mask( uint32 pattern );

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
void muxmdsp_send_frame_config( void );

/*===========================================================================

FUNCTION MUXMDSP_SEND_TXFE_TRIGGER

DESCRIPTION
  This function sends the TxFE trigger point to FW.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_send_txfe_trigger( cdmafw_rl_txfe_trigger_msg_t* txfe_trigger_m );

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
void muxmdsp_stop_txfe( void );

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
uint32 * muxmdsp_get_lmem_ch_addr( enc_chan_number_type ch );
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
);

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
void muxmdsp_print_tx_timing_info(  boolean show_tx_trigger_info );

/*===========================================================================

FUNCTION MUXMDSP_SET_DEMBACK_MODE

DESCRIPTION
  This function instructs FW whether to use Demback HW or not,
  and if not, what frame sizes are being used.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_set_demback_mode(boolean enable_hw, boolean is_20ms_frame);

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
                              uint32 max_symbols );

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
int muxmdsp_update_fw_diag_buf( uint8 * data, uint16 len );

/*===========================================================================

FUNCTION MUXMDSP_START_FLEX_CAPTURE

DESCRIPTION
  This function writes to QDSP6 addr 0xEC981174 to trigger the FLEX capture.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxmdsp_start_flex_capture( void );

#if defined( FEATURE_MODEM_1X_SW_DEMBACK ) && !defined( FEATURE_MODEM_1X_ATLAS_REVISIT )
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
boolean muxmdsp_get_is_sw_demback_active( void );
#endif /* FEATURE_MODEM_1X_SW_DEMBACK && !FEATURE_MODEM_1X_ATLAS_REVISIT */
#endif /* MUXMDSP_I_H */
